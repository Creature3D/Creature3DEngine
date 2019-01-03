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
#include "../include/gnelib/ConnectionEventGenerator.h"
#include "../include/gnelib/ConnectionStats.h"
#include "../include/gnelib/PacketParser.h"
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/PingPacket.h"
//#include "../include/gnelib/ObjectBrokerClient.h"
#include "../include/gnelib/Timer.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/Console.h"
#include "../include/gnelib/ServerConnectionListener.h"

#ifndef WIN32
#include <signal.h>
#endif

namespace GNE {
  namespace PacketParser {
    //this is declared here only so the user cannot access it, and the "real"
    //function can do checking on the ID given to it.
    void registerGNEPackets();
  }

char gameNameBuf[32] = {0};
guint32 userVersion = 0;
ConnectionEventGenerator::sptr eGen;

static bool initialized = false;
static int timeToWait = 10000;

bool initGNE(NLenum networkType, int (*atexit_ptr)(void (*func)(void)), int timeToClose ) {
  if (!initialized) {
    gnedbg(1, "GNE initialized");
    PacketParser::registerGNEPackets();
    //ObjectBrokerClient::staticInit();
    timeToWait = timeToClose;

    //This is a little hacky, but I checked the HawkNL source to make sure this
    //worked before I did this.
    if (networkType != NO_NET) {
      if (nlInit() == NL_FALSE)
        return true;
      if (nlSelectNetwork(networkType) == NL_FALSE)
        return true;
	  
	  nlEnable(NL_LITTLE_ENDIAN_DATA);
	  //short x = nlSwaps(2816);
	  //printf("endian set 2816 = %d\n",x);
      nlEnable(NL_BLOCKING_IO);
      nlEnable(NL_TCP_NO_DELAY);
      //GNE sends its data in little endian format.
      nlDisable(NL_SOCKET_STATS);
	  //nlEnable(NL_SOCKET_STATS);

      eGen = ConnectionEventGenerator::create();
      eGen->start();
      initialized = true; //We need only to set this to true if we are using HawkNL
    }

#ifndef WIN32
    signal(SIGPIPE, SIG_IGN);
#endif

#ifdef _DEBUG
    //Register killDebug separately, so we can still print out debugging
    //messages until the last minute possible.
    atexit_ptr(killDebug);
#endif

    atexit_ptr(shutdownGNE);

    return false;
  }
  return false;
}

void shutdownGNE() {
  if ( eGen ) {
    gnedbg( 1, "Shutting down CEG." );
    eGen->shutDown();
    //I'd like to use a join because that's cleaner, but I want to make sure
    //the program does not block indefinitely when closing.
  }

  gnedbg( 1, "GNE Shutdown begin: Closing all listeners." );
  ServerConnectionListener::closeAllListeners();
  gnedbg( 1, "Shutting down all connections." );
  Connection::disconnectAll();
  gnedbg( 1, "Stopping all timers." );
  Timer::stopAll();
  gnedbg( 1, "Shutting down all user threads." );
  Thread::requestAllShutdown( Thread::USER );

  gnedbg1( 1, "Waiting up to %d ms for all threads to shutdown.", timeToWait );
  bool timeout = Thread::waitForAllThreads( timeToWait );
  if ( timeout ) {
    gnedbg( 1, "Wait timeout: NOT ALL THREADS SHUT DOWN!" );
  }

  if ( eGen && eGen->isRunning() ) {
    gnedbg( 1, "CEG failed to shut down properly!  Please file a bug report." );
  }
  eGen.reset();

  if (initialized) {
    gnedbg( 1, "Shutting down HawkNL." );
    nlShutdown();
    initialized = false;
  }

  Console::shutdownConsole();

  gnedbg(1, "GNE Shutdown");
}

Address getLocalAddress() {
  assert(initialized);
  NLaddress nlAddr;

  NLsocket temp = nlOpen(0, NL_RELIABLE);
  nlGetLocalAddr(temp, &nlAddr);
  nlClose(temp);

  Address ret(nlAddr);
  ret.setPort(0);
  return ret;
}

ConnectionStats getGlobalStats() {
  assert(initialized);
  ConnectionStats ret;
  ret.packetsSent = nlGetInteger(NL_PACKETS_SENT);
  ret.bytesSent = nlGetInteger(NL_BYTES_SENT);
  ret.avgBytesSent = nlGetInteger(NL_AVE_BYTES_SENT);
  ret.maxAvgBytesSent = nlGetInteger(NL_HIGH_BYTES_SENT);
  ret.packetsRecv = nlGetInteger(NL_PACKETS_RECEIVED);
  ret.bytesRecv = nlGetInteger(NL_BYTES_RECEIVED);
  ret.avgBytesRecv = nlGetInteger(NL_AVE_BYTES_RECEIVED);
  ret.maxAvgBytesRecv = nlGetInteger(NL_HIGH_BYTES_RECEIVED);
  ret.openSockets = nlGetInteger(NL_OPEN_SOCKETS);
  return ret;
}

void enableStats() {
  assert(initialized);
  nlEnable(NL_SOCKET_STATS);
}

void disableStats() {
  assert(initialized);
  nlDisable(NL_SOCKET_STATS);
}

void clearStats() {
  assert(initialized);
  nlClear(NL_ALL_STATS);
}

int getOpenConnections() {
  assert(initialized);
  return nlGetInteger(NL_OPEN_SOCKETS);
}

GNEProtocolVersionNumber getGNEProtocolVersion() {
  assert(initialized);

  GNEProtocolVersionNumber ret;
  ret.version = 0;
  ret.subVersion = 0;
  ret.build = 7;

  return ret;
}

const char* getGameName() {
  return gameNameBuf;
}

guint32 getUserVersion() {
  assert(initialized);
  return userVersion;
}

void setGameInformation(std::string gameName, guint32 version) {
  assert(initialized);
  assert(gameName.length() <= (std::string::size_type)GNE::MAX_GAME_NAME_LEN);
  //We do this assert since this function should only be called once.
  assert(gameNameBuf[0] == 0);

  userVersion = version;
  strncpy(gameNameBuf, gameName.c_str(), MAX_GAME_NAME_LEN);
}

void checkVersions(const GNEProtocolVersionNumber& otherGNE,
                   std::string otherName, guint32 otherUser) {
  GNEProtocolVersionNumber us = getGNEProtocolVersion();

  //Check the GNE version numbers
  if (otherGNE.version != us.version ||
      otherGNE.subVersion != us.subVersion ||
      otherGNE.build != us.build) {
    if ((otherGNE.version > us.version) ||
        (otherGNE.version == us.version && otherGNE.subVersion > us.subVersion) ||
        (otherGNE.subVersion == us.subVersion && otherGNE.build > us.build))
      throw Error(Error::GNETheirVersionHigh);
    else
      throw Error(Error::GNETheirVersionLow);
  }

  //Check the game name
  if (otherName != gameNameBuf)
    throw WrongGame(otherName);

  //Check the user version numbers
  if (userVersion != otherUser)
    throw UserVersionMismatch(otherUser);
}

}
