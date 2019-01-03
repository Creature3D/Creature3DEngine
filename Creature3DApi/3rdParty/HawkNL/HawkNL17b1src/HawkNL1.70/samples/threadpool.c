/*
  Sample program for the HawkNL cross platform network library
  Copyright (C) 2000-2002 Phil Frisbie, Jr. (phil@hawksoft.com)
*/
/*
  This app shows a multithreaded client/server app with thread pooling.

  Choose the network type to use with the command line:
  
  clientserver NL_IP

  The default is NL_IP. Valid network types are: NL_IP and NL_LOOP_BACK 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nl.h"
#include "hawkthreads.h"

#if defined WIN32 || defined WIN64
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define sleep(x)    Sleep(1000 * (x))
#else
#include <unistd.h>
#endif

#define MAX_CLIENTS     8
#define MAX_SERVER      4
#define MAX_CONNECTS    100

HTmutex     startmutex, socketmutex;
NLenum      socktype = NL_TCP_PACKETS;
NLushort    serverport = 25000;
HTcond      servercond;
NLsocket    clientsock = NL_INVALID;
NLboolean   shutdown = NL_FALSE;


void printErrorExit(int error)
{
    NLenum err = nlGetError();

    if(error != 0)
    {
        printf("System error: %s\n", nlGetSystemErrorStr(error));
    }
    err = nlGetError();
    if(err == NL_SYSTEM_ERROR)
    {
        printf("System error: %s\n", nlGetSystemErrorStr(nlGetSystemError()));
    }
    else
    {
        printf("HawkNL error: %s\n", nlGetErrorStr(err));
    }
    nlShutdown();
    exit(1);
}

static void *mainServerLoop(void *t)
{
    NLint       thread = (NLint)t;
    int         result;

    /* wait for start */
    (void)htMutexLock(&startmutex);
    (void)htMutexUnlock(&startmutex);
    htThreadYield();
    /* wait on the condition forever */
    while((result = htCondWait(&servercond, 0)) == 0)
    {
        NLbyte      string[NL_MAX_STRING_LENGTH];
        NLsocket    s;

        if(shutdown == NL_TRUE)
        {
            printf("SERVER: thread %d asked to shut down\n", thread);
            return NULL;
        }
        (void)htMutexLock(&socketmutex);
        if(clientsock == NL_INVALID)
        {
            printf("SERVER: thread %d, invalid client socket\n", thread);
            (void)htMutexUnlock(&socketmutex);
            continue;
        }
        s = clientsock;
        clientsock = NL_INVALID;
        (void)htMutexUnlock(&socketmutex);
        if(nlRead(s, string, sizeof(string)) > 0)
        {
            printf("SERVER: thread %d, processed client thread %d\n", thread, atoi(string));
            nlWrite(s, string, strlen(string) + 1);
        }
        else
        {
            printf("SERVER: thread %d, nlRead error\n", thread);
        }
        /* we are done, so close the socket */
        nlClose(s);
    }
    /* if we got here, there was a problem with the condition */
    /* or we are shutting down */

    printf("SERVER: thread %d, system error: %s\n", thread, nlGetSystemErrorStr(result));
    printf("SERVER: thread %d exiting\n", thread);
    return NULL;
}

static void *mainClientLoop(void *t)
{
    NLint       thread = (NLint)t;
    NLsocket    sock;
    NLaddress   addr;
    NLbyte		str[NL_MAX_STRING_LENGTH];
    
    while(1)
    {
        sock = nlOpen(0, socktype);
        if(sock == NL_INVALID)
        {
            printf("CLIENT: thread %d nlOpen error\n", thread);
            if(nlGetError() == NL_NO_NETWORK)
            {
                /* we are shutting down */
                break;
            }
            continue;
        }
        /* now connect */
        nlGetLocalAddr(sock, &addr);
        nlSetAddrPort(&addr, serverport);
        if(!nlConnect(sock, &addr))
        {
            printf("CLIENT: thread %d nlConnect error\n", thread);
            nlClose(sock);
            printf("CLIENT: thread %d closed\n", thread);
            break;
        }
        printf("CLIENT: thread %d connected to %s\n", thread, nlAddrToString(&addr, str));
        sprintf(str, "%d Client thread says hello", thread);
        nlWrite(sock, str, strlen(str) + 1);
        while(nlRead(sock, str, sizeof(str)) > 0)
        {
            printf("CLIENT: thread %d received \"%s\"\n", thread, str);
        }
        nlClose(sock);
        htThreadYield();
    }
    printf("CLIENT: thread %d exiting\n", thread);
    return NULL;
}

int main(int argc, char **argv)
{
    NLsocket        serversock;
    NLenum          type = NL_IP;/* default network type */
    NLint           connects = 0;
    NLint           i;
    int             result;

    if(!nlInit())
        printErrorExit(0);
    
    printf("nlGetString(NL_VERSION) = %s\n\n", nlGetString(NL_VERSION));
    printf("nlGetString(NL_NETWORK_TYPES) = %s\n\n", nlGetString(NL_NETWORK_TYPES));

    if (argc == 2)
    {
        if(strcmp(argv[1], "NL_LOOP_BACK") == 0)
        {
            type = NL_LOOP_BACK;
        }
        else if(strcmp(argv[1], "NL_IP") == 0)
        {
            type = NL_IP;
        }
    }

    if(!nlSelectNetwork(type))
        printErrorExit(0);

    nlEnable(NL_BLOCKING_IO);

    if((result = htCondInit(&servercond)) != 0)
        printErrorExit(result);

	(void)htMutexInit(&startmutex);
	(void)htMutexInit(&socketmutex);

    /* create the server socket */
    serversock = nlOpen(serverport, socktype); /* just a random port number ;) */
    
    if(serversock == NL_INVALID)
        printErrorExit(0);
    
    if(!nlListen(serversock))       /* let's listen on this socket */
    {
        nlClose(serversock);
        printErrorExit(0);
    }
    /* create the server threads */
    (void)htMutexLock(&startmutex);
    for(i=0;i<MAX_SERVER;i++)
    {
        /* pass the thread number */
        (void)htThreadCreate(mainServerLoop, (void *)i, HT_FALSE);
    }
    /* now release the threads */
    (void)htMutexUnlock(&startmutex);
    htThreadYield();

    /* create the client threads */
    for(i=0;i<MAX_CLIENTS;i++)
    {
        /* pass the thread number */
        (void)htThreadCreate(mainClientLoop, (void *)i, HT_FALSE);
    }

    printf("Testing htCondSignal\n\n");
    /* main dispatch loop */
    while(connects < MAX_CONNECTS)
    {
        NLsocket newsock = nlAcceptConnection(serversock);
        
        if(newsock != NL_INVALID)
        {
            int result;

            /* wake up one thread */
            printf("MAIN: waking up one thread for socket %d, connection %d\n", newsock, connects);
			(void)htMutexLock(&socketmutex);
            clientsock = newsock;
			(void)htMutexUnlock(&socketmutex);
            htThreadYield();
            if((result = htCondSignal(&servercond)) != 0)
            {
                printf("MAIN: htCondSignal error\n");
                printErrorExit(result);
            }
            connects++;
        }
    }

    printf("\nTesting htCondBroadcast\n\n");
    connects = 0;
    while(connects < MAX_CONNECTS)
    {
        NLsocket newsock = nlAcceptConnection(serversock);
        
        if(newsock != NL_INVALID)
        {
            int result;

            /* wake up one thread */
            printf("MAIN: waking up all threads for socket %d, connection %d\n", newsock, connects);
			(void)htMutexLock(&socketmutex);
            clientsock = newsock;
			(void)htMutexUnlock(&socketmutex);
            htThreadYield();
            if((result = htCondBroadcast(&servercond)) != 0)
            {
                printf("MAIN: htCondBroadcast error\n");
                printErrorExit(result);
            }
            connects++;
        }
    }
    /* shutdown the thread pool */
    printf("\nMAIN: Begin shutdown\n\n");
    shutdown = NL_TRUE;
    htCondBroadcast(&servercond);
    /* this will cause the client threads to exit */
    printf("\nMAIN: Close serversock\n\n");
    nlClose(serversock);
    /* wait for all the threads to exit */
    sleep(4);
    printf("\nMAIN: Call nlShutdown\n\n");
    nlShutdown();
    printf("\nMAIN: nlShutdown finished\n\n");
	(void)htMutexDestroy(&startmutex);
	(void)htMutexDestroy(&socketmutex);
    (void)htCondDestroy(&servercond);
    return 0;
}

