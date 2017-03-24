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
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/ClientConnection.h"
#include "../include/gnelib/ConnectionParams.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/SyncConnection.h"
#include "../include/gnelib/EventThread.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/PacketParser.h"

namespace GNE {

/**
 * Simply just a class to temporarily hold connection parameters until the
 * ClientConnection is connected, then it is useless.
 */
class ClientConnectionParams {
public:
  Address dest;
  ConnectionParams cp;
  SmartPtr<SyncConnection> sConnPtr;
};

ClientConnection::ClientConnection() : Thread("CliConn", Thread::DEF_PRI) {
  gnedbgo(5, "created");
  setType( CONNECTION );
}

ClientConnection::sptr ClientConnection::create() {
  sptr ret( new ClientConnection() );
  ret->Thread::setThisPointer( ret );
  ret->Connection::setThisPointer( ret );
  return ret;
}

ClientConnection::~ClientConnection() {
  gnedbgo(5, "destroyed");
}

bool ClientConnection::open(const Address& dest,
                            const ConnectionParams& p) {
  assert( getState() == NeedsInitialization );

  if (!dest || p)
    return true;
  else {
    params = ParamsSPtr( new ClientConnectionParams );
    params->dest = dest;
    params->cp = p;
    setListener(p.getListener());
    setTimeout(p.getTimeout());

    sockets.r = nlOpen(p.getLocalPort(), NL_RELIABLE_PACKETS);
  }
  if ( sockets.r == NL_INVALID ) {
    return true;
  } else {
    finishedInit();
    return false;
  }
}

void ClientConnection::connect() {
  connect( SyncConnection::sptr() );
}

void ClientConnection::connect( const SyncConnection::sptr& wrapped ) {
  assert( sockets.r != NL_INVALID );
  assert( params );
  assert( params->dest );
  assert( !params->cp );
  assert( getState() == ReadyToConnect );

  params->sConnPtr = wrapped;
  startConnecting();
  start();
}

Error ClientConnection::waitForConnect(unsigned int ms) {
  join(ms);
  return connError;
}

/**
 * \todo check to see how well things work if GNE is shutdown while a connection
 *       is in progress.
 *
 * \bug The PacketFeeder is set after onNewConn, so it is possible that if
 *      the user sets a different PacketFeeder right after connecting, it
 *      might "get lost" and if set during onNewConn, is definitely lost.
 *      I do provide a check do this is only a problem if a feeder is set
 *      in the ConnectionParams AND in onNewConn.
 */
void ClientConnection::run() {
  assert( getListener() );
  gnedbgo1(1, "Trying to connect to %s", params->dest.toString().c_str());
  //endConnect will set the null listener to discard the events, so we
  //have to cache the current listener.
  ConnectionListener::sptr origListener = getListener();

  NLaddress temp = params->dest.getAddress();
  NLboolean check = nlConnect(sockets.r, &temp);

  if (check == NL_TRUE) {
    //Try to connect using the GNE protocol before we mess with any of the
    //user stuff.
    try {
      doHandshake();
    } catch (Error& e) {
      gnedbgo1(1, "Connection failure during GNE handshake: %s", e.toString().c_str());
      doFailure( origListener, e );
      return;
    }
	catch (...) {
		Error e(Error::User);
		gnedbgo1(1, "Connection failure during GNE handshake: %s", e.toString().c_str());
		doFailure( origListener, e );
		return;
	}
    gnedbgo(2, "GNE Protocol Handshake Successful.");

    //We don't want to doubly-wrap SyncConnections, so we check for a wrapped
    //one here and else make our own.
    bool ourSConn = false;
    if (!params->sConnPtr) {
      params->sConnPtr = SyncConnection::create( this_.lock() );
      ourSConn = true;
    } else
      assert(params->sConnPtr == getListener());
    //The sConn reference variable is used only for syntactical convienence.
    SyncConnection& sConn = *params->sConnPtr;
    
    bool onConnectFinished = false;
    try {
      //We only want to hold events on our own SyncConnection.  On a user
      //supplied SyncConnection, when it fails we will fail, and
      //SyncConnection::connect() will throw an error.
      if (ourSConn)
        sConn.startConnect();
      startThreads();
      reg(true, (sockets.u != NL_INVALID));

      //Setup the packet feeder
      ps->setFeederTimeout( params->cp.getFeederTimeout() );
      ps->setLowPacketThreshold( params->cp.getLowPacketThreshold() );
	  ps->setBufSizeScale(params->cp.getBufSizeScale());

      gnedbgo2(2, "Starting onConnect r: %i, u: %i", sockets.r, sockets.u);
      getListener()->onConnect(sConn); //SyncConnection will relay this
      onConnectFinished = true;
      finishedConnecting();

      if (ourSConn) {
        sConn.endConnect(true);
        sConn.release();
      }

      //Setup the packet feeder
      if ( params->cp.getFeeder() )
        ps->setFeeder( params->cp.getFeeder() );

	  nlSetSocketOpt(sockets.r,NL_BLOCKING_IO,false);
	  nlSetSocketOpt(sockets.r,NL_TCP_NO_DELAY,true);
    } catch (Error& e) {
      if (!onConnectFinished) {
        if (ourSConn)
          sConn.endConnect(false);
        
        doFailure( origListener, e );
      }
	  disconnect();
    }
	catch (...) {
		Error e(Error::User);
		if (!onConnectFinished) {
			if (ourSConn)
				sConn.endConnect(false);

			doFailure( origListener, e );
		}
	    disconnect();
	}

  } else { //nlConnect failed
    LowLevelError err = LowLevelError(Error::ConnectionTimeOut);
    gnedbgo1(1, "Connection failure: %s", err.toString().c_str());
    doFailure( origListener, err );
	disconnect();
  }

  //Save some memory by doing an early explicit reset.
  params.reset();
}

void ClientConnection::shutDown() {
  Thread::shutDown();
  disconnect();
}

void ClientConnection::doHandshake() {
  //Start the GNE protocol connection process.
  //The first packet is from client to server, and is the connection
  //request packet (CRP).
  gnedbgo(2, "Sending the CRP.");
  sendCRP();
  
  //Now we expect to receive the connection accepted packet (CAP) or the
  //refused connection packet, and then based on that set up the
  //unreliable connection.
  gnedbgo(2, "Waiting for the CAP.");
  Address temp = getCAP();
  
  if (params->cp.getUnrel()) {
    gnedbgo(2, "Setting up the unreliable connection");
    setupUnreliable(temp);
  } else {
    gnedbgo(2, "Unreliable connection not requested.");
  }
}

void ClientConnection::sendCRP() {
  Buffer crp;
  //addHeader(crp);
  //addVersions(crp);
  crp << 0;//id
  crp << (gint16)5;//size
  crp << (guint32)params->cp.getInRate();//4
  crp << ((params->cp.getUnrel()) ? gTrue : gFalse);//1
  crp << PacketParser::END_OF_PACKET;//4

  int check = sockets.rawWrite(true, crp);
  //The write should succeed and have sent all of our data.
  if (check != crp.getPosition())
    throw LowLevelError(Error::Write);
}

const int MINLEN = 11;//8;
const int REFLEN = 11;//44;
const int CAPLEN = 19;//12;

Address ClientConnection::getCAP() {
  Buffer cap( 64 );
  //int sockstat;
  //int check = NL_INVALID;
  //while(1)
  //{
	 // sockstat = nlPollSocket(sockets.r, NL_READ_STATUS, 0);
	 // if(sockstat == NL_TRUE)
	 // {
		//  check = sockets.rawRead(true, cap);
		//  break;
	 // }
	 // else if(sockstat == NL_INVALID)
	 // {
		//  check = NL_INVALID;
		//  break;
	 // }
  //}
  int check = sockets.rawRead(true, cap);
  if (check == NL_INVALID)
    throw LowLevelError(Error::Read);
  //The packet must be at least as large to check if it is a CAP or a
  //refusal packet.
  if (check < MINLEN) {
    gnedbgo1(1, "Response to CRP is too small (size is %d).", check);
    throw ProtocolViolation(ProtocolViolation::InvalidCAP);
  }

  //Now parse the CAP (or refusal packet)
  //checkHeader(cap, ProtocolViolation::InvalidCAP);
  gint32 id;
  cap >> id;
  gint16 size;
  cap >> size;

  gbool isCAP;
  cap >> isCAP;

  if (isCAP) {
    //Check to make sure packet sizes match.
    if (check != CAPLEN){
      gnedbgo2(1, "Expected a CAP of size %d but got %d bytes instead.",
        CAPLEN, check);
      throw ProtocolViolation(ProtocolViolation::InvalidCAP);
    }

    //Get the max rate the remote end allows.
    guint32 maxOutRate;
    cap >> maxOutRate;

    //Now we have enough info to create our PacketStream.
    ps = PacketStream::create(params->cp.getOutRate(), maxOutRate, *this);

    //Get the unreliable connection information.  A port of less than 0 means
    //we didn't request an unreliable conn, or it we refused to us.
    gint32 portNum;
    cap >> portNum;
    Address ret = params->dest;

    if (portNum > 65535) {
      gnedbgo1(1, "Invalid port number %d given.", portNum);
      throw ProtocolViolation(ProtocolViolation::InvalidCAP);

    } else if (portNum > 0) {
      ret.setPort((int)portNum);

    } else {
      if (params->cp.getUnrel()) {
        gnedbgo(2, "The server refused our request for an unreliable connection.");
        params->cp.setUnrel(false);
      }
    }

    return ret;
  }

  //else we got a refusal packet
  if (check != REFLEN) {
    gnedbgo2(1, "Expected a refusal of size %d but got %d bytes instead.",
      REFLEN, check);
    throw ProtocolViolation(ProtocolViolation::InvalidCAP);
  }

  //We got a refusal packet, so let's check the versions.
  //checkVersions(cap);

  //If the version numbers are all the same our connection was simply
  //just refused.
  throw Error(Error::ConnectionRefused);

  //We should never reach this point
  assert(false);
  return Address();
}

void ClientConnection::setupUnreliable(const Address& dest) {
  assert(dest);
  sockets.u = nlOpen(0, NL_UNRELIABLE);
  sockets.releaseudp = true;
  if (sockets.u == NL_INVALID)
    throw LowLevelError(Error::CouldNotOpenSocket);
  NLaddress temp = dest.getAddress();
  //modify by wch
  //nlSetRemoteAddr(sockets.u, &temp);
  sockets.setUDPRemoteAddr(temp);
  assert( sockets.getLocalAddress(false) );

  //Now send back our local info, and send a dummy packet out first to open
  //up any possible firewalls or gateways.
  int check = 0;

  Buffer resp;
  //resp << sockets.getLocalAddress(false).getPort();
  //check = sockets.rawWrite(true, resp);
  //if (check != resp.getPosition() || check != sizeof(gint32))
  //  throw LowLevelError(Error::Write);

  //resp.clear();
  resp << PacketParser::END_OF_PACKET;
  check = sockets.rawWrite(false, resp);
  if (check != resp.getPosition() || check != sizeof(PacketParser::END_OF_PACKET))
    throw LowLevelError(Error::Write);
}

void ClientConnection::doFailure( const SmartPtr< ConnectionListener >& l, const Error& e ) {
  if ( shutdown ) {
    Error err( Error::ConnectionAborted );
    l->onConnectFailure( *this, err );
    connError = err;

  } else { 
    l->onConnectFailure( *this, e );
    connError = e;
  }
}

}
