/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@mail.rit.edu)
 * Project website: http://www.rit.edu/~jpw9607/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "../include/gnelib/gneintern.h"
#include "../include/gnelib/ServerConnectionListener.h"
#include "../include/gnelib/ConnectionEventGenerator.h"
#include "../include/gnelib/ServerConnection.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/ConnectionParams.h"
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/Lock.h"
//#include <iostream>
//#include <fstream>
#include <winsock2.h>
namespace GNE {

typedef std::vector<ServerConnectionListener::sptr> SCLList;
typedef SCLList::iterator SCLListIter;

static SCLList listeners;
static Mutex listSync;

ServerConnectionListener::ServerConnectionListener()
: listening(false)/*, socket( NL_INVALID )*/ {//modify by wch
  gnedbgo(5, "created");
}

ServerConnectionListener::~ServerConnectionListener() {
  //We shouldn't be listening
  assert( !listening );

  //However... open could have been called but never listen.
  //This could cause a problem if called after GNE shutdown.
  rawClose();

  gnedbgo(5, "destroyed");
}

void ServerConnectionListener::closeAllListeners() {
  LockMutex lock( listSync );

  SCLListIter iter = listeners.begin();
  for ( ; iter != listeners.end(); ++iter )
    (*iter)->rawClose();

  listeners.clear();
}

bool ServerConnectionListener::open(int port) {
  LockMutex lock(sync);

  //if ( socket == NL_INVALID ) {
  //  socket = nlOpen(port, NL_RELIABLE_PACKETS);
  //  return (socket == NL_INVALID);
  //} else
  //  return false;

	bool failed = false;
	if ( sockets.r == NL_INVALID ) 
	{
		sockets.r = nlOpen(port, NL_RELIABLE_PACKETS);
		if(sockets.r!=NL_INVALID)
		{
			ConnectionParams params;
			getNewConnectionParams(params);
			if(params.getUnrel())
			{
				if ( sockets.u == NL_INVALID ) 
				{
					sockets.u = nlOpen(port, NL_UNRELIABLE);
					sockets.releaseudp = true;
					failed = (sockets.u == NL_INVALID);
				}
			}
		}
		else
		{
			failed = true;
		}
	}
	return failed;
}

void ServerConnectionListener::close() {
  //must lock listSync before sync always.
  LockMutex lock( listSync );
  LockMutex lock2( sync );

  SCLList& l = listeners;
  l.erase( std::remove( l.begin(), l.end(), this_.lock() ), l.end() );

  rawClose();
}

bool ServerConnectionListener::listen() {
  //must lock listSync before sync always, so we are FORCED to do a defensive
  //lock on listSync.
  LockMutex lock( listSync );
  LockMutex lock2( sync );
  //modify by wch
  if ( sockets.r == NL_INVALID )
    return true;

  NLboolean ret = nlListen(sockets.r);
  if (ret == NL_TRUE) {

    gnedbgo1(3, "Registering listen socket %i", sockets.r);

    //Lock our strong pointer, making sure setThisPointer was called.
    sptr this_strong = this_.lock();//BOOST_HAS_THREADS
    assert( this_strong );

    //Do the actual register.
    GNE::eGen->reg(sockets.r, ServerListener::sptr( new ServerListener( this_strong ) ) );
    listening = true;

    //We shouldn't already be in this list...
    assert( std::find( listeners.begin(), listeners.end(), this_strong ) == listeners.end() );

    //Add us to the list.
    listeners.push_back( this_strong );

    //success
    return false;
  }
  return true;
}

bool ServerConnectionListener::isListening() const {
  LockMutex lock(sync);

  return listening;
}

void ServerConnectionListener::onReceive() {
	LockMutex lock(sync);

	//modify by wch
	//NLsocket sock = nlAcceptConnection(socket);
	NLsocket sock = nlAcceptConnection(sockets.r);
	if (sock != NL_INVALID) 
	{
		ConnectionParams params;
		getNewConnectionParams(params);

		if (!params)
		{
			if(tryAddNewConnection(sock)) 
			{
				//If the params were valid
				assert( !this_.expired() );
				//{//处理CLOSE_WAIT
					// int nREUSEADDR = 1;
					// setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const char*)&nREUSEADDR,sizeof(int));

					// linger m_sLinger;
					// m_sLinger.l_onoff = 1;  // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
					// m_sLinger.l_linger = 0; // (容许逗留的时间为0秒)
					// setsockopt(sock,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(linger));
				//}
				ServerConnection::sptr newConn = ServerConnection::create(params, sock, this_.lock());
				gnedbgo2(4, "Spawning a new ServerConnection %x on socket %i",
					newConn, sock);
				//Thread::sleep(10);
				newConn->start();
			}
			else
			{
				removeNewConnectionTry(sock);
				//If the params are not valid, report the error
				onListenFailure( Error(Error::OtherGNELevelError), Address(),
					params.getListener());
				nlClose(sock);
			}
		} 
		else 
		{
			//If the params are not valid, report the error
			onListenFailure( Error(Error::OtherGNELevelError), Address(),
				params.getListener());
			nlClose(sock);
		}
	} 
	else 
	{
		LowLevelError err = LowLevelError();
		gnedbgo1(1, "Listening failure (accept failed): %s",
		err.toString().c_str());
		onListenFailure( err, Address(), ConnectionListener::sptr() );
	}
}

Address ServerConnectionListener::getLocalAddress() const {
  LockMutex lock(sync);
//modify by wch
  //if (socket != NL_INVALID) {
  //  NLaddress ret;
  //  nlGetLocalAddr(socket, &ret);
  //  return Address(ret);
  //} else {
  //  return Address();
  //}
  if (sockets.r != NL_INVALID) {
    NLaddress ret;
    nlGetLocalAddr(sockets.r, &ret);
    return Address(ret);
  } else {
    return Address();
  }
}

void ServerConnectionListener::setThisPointer( const sptr& thisPointer ) {
  this_ = thisPointer;
}

void ServerConnectionListener::rawClose() {
  LockMutex lock(sync);
///////modify by wch
  //if (listening) {
  //  gnedbgo1(3, "Unregistering listen socket %i", socket);
  //  GNE::eGen->unreg(socket);
  //  listening = false;
  //}
  //
  //if ( socket != NL_INVALID ) {
  //  nlClose(socket);
  //  socket = NL_INVALID;
  //}

  if (listening) {
    gnedbgo1(3, "Unregistering listen socket %i", sockets.r);
    GNE::eGen->unreg(sockets.r);
	sockets.r = NL_INVALID;
    listening = false;
  }
  
  //sockets.disconnect();
}

void ServerConnectionListener::processOnListenFailure( const Error& error, const Address& from, const ConnectionListener::sptr& listener) {
  onListenFailure( error, from, listener );
}

void ServerConnectionListener::processOnListenSuccess( const ConnectionListener::sptr& listener ) {
  onListenSuccess( listener );
}

void ServerConnectionListener::onListenSuccess(const ConnectionListener::sptr& listener) {
  //The default behavior for this event is to do nothing.
}

ServerConnectionListener::ServerListener::ServerListener(const ServerConnectionListener::sptr& listener)
: conn(listener) {
}

ServerConnectionListener::ServerListener::~ServerListener() {
}

void ServerConnectionListener::ServerListener::onReceive() {
  conn->onReceive();
}

}
