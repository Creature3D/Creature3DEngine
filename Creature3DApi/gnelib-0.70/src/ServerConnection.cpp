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
#include "../include/gnelib/ServerConnection.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/ConnectionParams.h"
#include "../include/gnelib/SyncConnection.h"
#include "../include/gnelib/ServerConnectionListener.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/EventThread.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/SocketPair.h"
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/lock.h"
#include "../include/gnelib/PacketParser.h"
namespace GNE {

/**
 * Simply just a class to temporarily hold connection parameters until the
 * ClientConnection is connected, then it is useless.
 */
class ServerConnectionParams {
public:
  ConnectionParams cp;
  ServerConnectionListener::sptr creator;
  bool doJoin;
};

ServerConnection::ServerConnection()
: Thread("SrvrConn", Thread::DEF_PRI) {
  gnedbgo(5, "created");
}

void ServerConnection::init(
    const ConnectionParams& p, NLsocket rsocket2,
    const SmartPtr<ServerConnectionListener>& creator ) {

  setType( CONNECTION );
  assert(!p);
  sockets.r = rsocket2;
  finishedInit();  //we move right to ReadyToConnect

  setListener(p.getListener());
  setTimeout(p.getTimeout());

  params = ParamsSPtr( new ServerConnectionParams );
  params->cp = p;
  params->creator = creator;
  params->doJoin = true;

  startConnecting(); //we move right into Connecting state
}

ServerConnection::sptr
ServerConnection::create(const ConnectionParams& p, NLsocket rsocket,
                         const SmartPtr<ServerConnectionListener>& creator) {
  sptr ret( new ServerConnection() );
  ret->Thread::setThisPointer( ret );
  ret->Connection::setThisPointer( ret );
  ret->init( p, rsocket, creator );
  return ret;
}

ServerConnection::~ServerConnection() {
  gnedbgo(5, "destroyed");
}

/**
 * \todo better test GNE shutting down while connection is being made.
 *
 * \bug The PacketFeeder is set after onNewConn, so it is possible that if
 *      the user sets a different PacketFeeder right after connecting, it
 *      might "get lost" and if set during onNewConn, is definitely lost.
 *      I do provide a check do this is only a problem if a feeder is set
 *      in the ConnectionParams AND in onNewConn.
 */
void ServerConnection::run() {

  assert(sockets.r != NL_INVALID);
  assert(getListener());
  NLsocket socket = sockets.r;
  //endConnect will set the null listener to discard the events, so we
  //have to cache the current listener.
  ConnectionListener::sptr origListener = getListener();

  //We cache the remote address because after an error occurs we may not be
  //able to get it to pass to the listen failure function.
  Address rAddr = getRemoteAddress(true);
  gnedbgo1(1, "New connection incoming from %s", rAddr.toString().c_str());

  //Do the GNE protocol handshake.
  try {
    doHandshake();
  } catch (Error& e) {
    doFailure( params->creator, e, rAddr, origListener );
	params->creator->removeNewConnectionTry(socket);
    sockets.disconnect();
    return;
  }
  catch (...) {
	  Error e(Error::User);
	  doFailure( params->creator, e, rAddr, origListener );
	  params->creator->removeNewConnectionTry(socket);
	  sockets.disconnect();
	  return;
  }
  gnedbgo(2, "GNE Protocol Handshake Successful.");
  
  //Start up the connecting SyncConnection and start the onNewConn event.
  bool onNewConnFinished = false;
  SyncConnection::sptr sConnPtr = SyncConnection::create( this_.lock() );
  SyncConnection& sConn = *sConnPtr;

  try {
    sConn.startConnect();
    startThreads();
    reg(true, true);
	onNewConnFinished = true;

    //Setup the packet feeder
    ps->setFeederTimeout( params->cp.getFeederTimeout() );
    ps->setLowPacketThreshold( params->cp.getLowPacketThreshold() );
	ps->setBufSizeScale(params->cp.getBufSizeScale());

    gnedbgo2(2, "Starting onNewConn r: %i, u: %i", sockets.r, sockets.u);
    getListener()->onNewConn(sConn); //SyncConnection will relay this

    finishedConnecting(); //move state to Connected

    params->creator->onListenSuccess(origListener);

    //Start bringing connection to normal state.  SyncConnection will make
    //sure onDisconnect gets called starting with endConnect().
    sConn.endConnect(true);
    sConn.release();

    //Setup the packet feeder
    if ( params->cp.getFeeder() )
      ps->setFeeder( params->cp.getFeeder() );

	////modify by wch
	gnedbgo(2, "Got CRP, now sending CAP.");
	sendCAP();

	nlSetSocketOpt(sockets.r,NL_BLOCKING_IO,false);
	nlSetSocketOpt(sockets.r,NL_TCP_NO_DELAY,true);
  } 
  catch (Error& e) 
  {
	if (!onNewConnFinished) 
	{
		sConn.endConnect(false);
		doFailure( params->creator, e, rAddr, origListener );
		disconnect();
	}
	else
	{
		sConn.endConnect(false);
		if ( ps && ps->hasStarted() ) 
		{
		    ps->shutDown();
		}
		if ( eventThread && eventThread->hasStarted() ) 
		{
			eventThread->shutDown();
		}
		sockets.disconnect();
	}
  }
  catch (...) 
  {
	  if (!onNewConnFinished) 
	  {
		  sConn.endConnect(false);
		  disconnect();
	  }
	  else
	  {
		  sConn.endConnect(false);
		  if ( ps && ps->hasStarted() ) 
		  {
			  ps->shutDown();
		  }
		  if ( eventThread && eventThread->hasStarted() ) 
		  {
			  eventThread->shutDown();
		  }
		  sockets.disconnect();
	  }
  }
  params->creator->removeNewConnectionTry(socket);
  //Release memory for early cleanup.
  params.reset();
}

void ServerConnection::shutDown() {
  Thread::shutDown();
  disconnect();
}

void ServerConnection::doHandshake() {
  //Receive the client's CRP.
  gnedbgo(2, "Waiting for the client's CRP.");
  try {
    getCRP();
  } catch (Error& e) {
    if (e.getCode() == Error::GNETheirVersionHigh ||
      e.getCode() == Error::GNETheirVersionLow ||
      e.getCode() == Error::UserVersionMismatch ||
      e.getCode() == Error::WrongGame) {
      gnedbgo(2, "Versions don't match, sending refusal.");
      sendRefusal();
    }
    throw;
  }
  catch (...) {
	throw;
  }
  
  //Else, we send the CAP
  //gnedbgo(2, "Got CRP, now sending CAP.");
  //sendCAP();
  
  //Then we handle anything related to the unreliable connection if needed.
  if (params->cp.getUnrel()) {
    gnedbgo(2, "Unreliable requested.  Getting unrel info.");
    getUnreliableInfo();
  } else {
    gnedbgo(2, "Unreliable connection not requested or refused.");
  }
}

const int CRPLEN = 15;// 48; //Wch 不做Hearder和Version检查

void ServerConnection::getCRP() {
  Buffer crp( 64 );

  int sockstat;
  int check = NL_INVALID;
  int i = 0;
  while(i<10)
  {
	  sockstat = nlPollSocket(sockets.r, NL_READ_STATUS, 250);
	  if(sockstat == NL_TRUE)
	  {
		  check = sockets.rawRead(true, crp);
		  break;
	  }
	  else if(sockstat == NL_INVALID)
	  {
		  check = NL_INVALID;
		  break;
	  }
	  i++;
  }
  //int check = sockets.rawRead(true, crp);
  if (check != CRPLEN) {
    if (check == NL_INVALID) {
      gnedbgo(1, "nlRead error when trying to get CRP.");
      throw LowLevelError(Error::Read);
    } else {
      gnedbgo2(1, "Protocol violation trying to get CRP.  Got %d bytes expected %d",
        check, CRPLEN);
      throw ProtocolViolation(ProtocolViolation::InvalidCRP);
    }
  }

  //Now parse the CRP

  //Check the header and versions.  These will throw exceptions if there is
  //a problem.
  //checkHeader(crp, ProtocolViolation::InvalidCRP);
  //checkVersions(crp);

  gint32 id;
  crp >> id;
  gint16 size;
  crp >> size;

  guint32 maxOutRate;
  crp >> maxOutRate;

  gbool unreliable;
  crp >> unreliable;
  //We use the unreliable connection only if both sides allow it.
  params->cp.setUnrel(unreliable && params->cp.getUnrel());

  //Now that we know the versions are OK, make the PacketStream
  ps = PacketStream::create(params->cp.getOutRate(), maxOutRate, *this);
}

void ServerConnection::sendRefusal() {
  Buffer ref;
  //addHeader(ref);
  ref << 0;//id
  ref << (gint16)1;//size
  ref << gFalse;

  //addVersions(ref);
  ref << PacketParser::END_OF_PACKET;
  int check = sockets.rawWrite(true, ref);
  //We don't check for error because if there we don't really care since we
  //are refusing the connection there is nothing else we can do, but we will
  //do it just for the logs.
  if (check != ref.getPosition()) {
    gnedbgo1(1, "Writing the refusal packet failed, got a return of %d",
      check);
  }
}

void ServerConnection::sendCAP() {
  Buffer cap;
  //addHeader(cap);
  cap << 0;//id
  cap << (gint16)9;//size
  cap << gTrue;//1
  cap << params->cp.getInRate();//4
  if (params->cp.getUnrel()) {
    //If the client requested it and we allowed it, open an unreliable port
    //and send the port number to the client.
    //sockets.u = nlOpen(0, NL_UNRELIABLE);
    sockets.u = params->creator->sockets.u;
    cap << (gint32)sockets.getLocalAddress(false).getPort();//4
  } else {
    //Send -1 to tell the client there will be no unreliable port
    cap << (gint32)-1;
  }
  cap << PacketParser::END_OF_PACKET;
  int check = sockets.rawWrite(true, cap);
  gnedbgo1(5, "Sent a CAP with %d bytes.", check);
  //The write should succeed and have sent all of our data.
  if (check != cap.getPosition())
    throw LowLevelError(Error::Write);
}

void ServerConnection::getUnreliableInfo() {
  //Buffer raw( 64 );

  //int check = sockets.rawRead(true, raw);
  //if (check != sizeof(gint32)) {
  //  if (check == NL_INVALID) {
  //    gnedbgo(1, "nlRead error when trying to get unreliable info.");
  //    throw LowLevelError(Error::Read);
  //  } else {
  //    gnedbgo2(1, "Protocol violation trying to get unrel info.  Got %d bytes expected %d",
  //      check, sizeof(guint16));
  //    throw Error(Error::ProtocolViolation);
  //  }
  //}

  //gint32 portNum;
  //raw >> portNum;
  //if (portNum < 0 || portNum > 65535) {
  //  gnedbgo1(1, "Protocol violation: invalid port %d received", portNum);
  //  throw Error(Error::ProtocolViolation);
  //}

  //Address uDest = sockets.getRemoteAddress(true);
  //uDest.setPort((int)portNum);
  //NLaddress temp = uDest.getAddress();
  ////modify by wch
  ////nlSetRemoteAddr(sockets.u, &temp);
  //sockets.setUDPRemoteAddr(temp);
}

void ServerConnection::doFailure( const SmartPtr< ServerConnectionListener >& l,
                                  const Error& e,
                                  const Address& addr,
                                  const SmartPtr< ConnectionListener >& listener ) {
  if ( shutdown ) {
    Error err( Error::ConnectionAborted );
    l->onListenFailure( err, addr, listener );

  } else {
    l->onListenFailure( e, addr, listener );
  }
}

}
