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
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/ConnectionStats.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/ExitPacket.h"
#include "../include/gnelib/PacketParser.h"
#include "../include/gnelib/ConnectionEventGenerator.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Errors.h"
#include "../include/gnelib/SocketPair.h"
#include "../include/gnelib/Address.h"
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/EventThread.h"
#include "../include/gnelib/Lock.h"
//#include <CRCore/thread/crScopedLock.h>
namespace GNE {

Connection::Connection()
: state( NeedsInitialization ), timeout_copy( 0 ) {
	m_codeset = new CodeSet;
	m_codeset_buf = new CodeSet;
}

void Connection::disconnectAll() {
  Thread::requestAllShutdown( Thread::CONNECTION );
}

Connection::~Connection() {
  if ( state == ReadyToConnect ) //if only ClientConnection::open
    disconnect();
  {
	  LockMutex lock( m_codesetMutex );
	  m_codeset->clear();
	  m_codeset_buf->clear();
	  delete m_codeset;
	  delete m_codeset_buf;
  }
  assert( state == NeedsInitialization || state == Disconnected );
}

ConnectionListener::sptr Connection::getListener() const {
  LockMutex lock( sync );
  if ( eventThread )
    return eventThread->getListener();
  else
    return ConnectionListener::sptr();
}

void Connection::setListener(const ConnectionListener::sptr& listener) {
  LockMutex lock( sync );
  if ( eventThread )
    eventThread->setListener( listener );
}

int Connection::getTimeout() {
  LockMutex lock( sync );

  if ( eventThread )
    return eventThread->getTimeout();
  else
    return timeout_copy;
}

void Connection::setTimeout(int ms) {
  LockMutex lock( sync );
  if ( eventThread )
    eventThread->setTimeout(ms);
  timeout_copy = ms;
}

PacketStream& Connection::stream() {
  assert(ps);
  return *ps;
}

ConnectionStats Connection::getStats(int reliable) const {
  LockMutex lock( sync );
  return sockets.getStats(reliable);
}

Address Connection::getLocalAddress(bool reliable) const {
  LockMutex lock( sync );
  return Address(sockets.getLocalAddress(reliable));
}

Address Connection::getRemoteAddress(bool reliable) const {
  LockMutex lock( sync );
  return Address(sockets.getRemoteAddress(reliable));
}

Connection::State Connection::getState() const {
  return state;
}

bool Connection::isConnected() const {
  return state == Connected;
}

void Connection::disconnect() 
{
  LockMutex lock( sync );

  unreg(true, true);

  if ( state != Disconnected && state != Disconnecting ) 
  {
    state = Disconnecting;

    gnedbgo2(2, "disconnecting r: %i, u: %i", sockets.r, sockets.u);

    if ( ps && ps->hasStarted() ) 
	{
      ps->shutDown(); //PacketStream will try to send the required ExitPacket.

      //we have to release sync to prevent deadlock in case PacketStream decides
      //to lock sync (from processError).
      //sync.release();
      //ps->join(); //we have to join to wait for the ExitPacket to go out.
      //sync.acquire();
    }
	
	//Shutdown the EventThread.
	if (eventThread && eventThread->hasStarted())
	{
		eventThread->onDisconnect();
		//eventThread.reset(); //Kill the cycle we participate in
	}
  }
  //Even if the PS or ET aren't running, the low-level stuff in the connection
  //threads should get an error when we disconnect the actual sockets.
  //sockets.disconnect();

  state = Disconnected;
  //gnedbgo2(2, "Disconnected r: %i, u: %i", sockets.r, sockets.u);
}

void Connection::disconnectSendAll(int waitTime) {
  LockMutex lock( sync );
  if (isConnected())
    ps->waitToSendAll(waitTime);
  disconnect();
}

bool Connection::checkCode(int code)
{//code驻留1－2分钟
	LockMutex lock( m_codesetMutex );
	if(m_codeset->find(code) != m_codeset->end())
		return false;
	if(m_codeset_buf->find(code) != m_codeset_buf->end())
		return false;
	m_codeset->insert(code);
	return true;
}
void Connection::update()
{//每隔1分钟触发一次
	LockMutex lock( m_codesetMutex );
	CodeSet *tmp = m_codeset;
	m_codeset = m_codeset_buf;
	m_codeset_buf = tmp;
	m_codeset->clear();
}
void Connection::setThisPointer( const wptr& weakThis ) {
  assert( this_.expired() );
  this_ = weakThis;
  eventThread = EventThread::create( weakThis.lock() );
}

void Connection::addHeader(Buffer& raw) {
  raw << (gbyte)'G' << (gbyte)'N' << (gbyte)'E';
}

void Connection::addVersions(Buffer& raw) {
  GNEProtocolVersionNumber us = GNE::getGNEProtocolVersion();
  //Write the GNE version numbers.
  raw << us.version << us.subVersion << us.build;

  //Write the whole game name buffer
  raw.writeRaw((const gbyte*)GNE::getGameName(), GNE::MAX_GAME_NAME_LEN + 1);

  //Write the user version
  raw << GNE::getUserVersion();
}

void Connection::checkHeader(Buffer& raw,
                             ProtocolViolation::ViolationType t) {
  gbyte headerG, headerN, headerE;
  raw >> headerG >> headerN >> headerE;

  if (headerG != 'G' || headerN != 'N' || headerE != 'E')
    throw ProtocolViolation(t);
}

void Connection::checkVersions(Buffer& raw) {
  //Get the version numbers
  GNEProtocolVersionNumber them;
  raw >> them.version >> them.subVersion >> them.build;

  //Read the game name
  gbyte rawName[GNE::MAX_GAME_NAME_LEN + 1];
  raw.readRaw(rawName, GNE::MAX_GAME_NAME_LEN + 1);
  //And convert it to a string, making sure it is of the proper length and
  //NULL-terminated.
  char gameName[GNE::MAX_GAME_NAME_LEN + 1];
  strncpy(gameName, (const char*)rawName, GNE::MAX_GAME_NAME_LEN);

  //Read the user version number
  guint32 themUser;
  raw >> themUser;

  //This will throw an Error of the versions are wrong.
  GNE::checkVersions(them, gameName, themUser);
}

void Connection::onReceive() 
{
  LockMutex lock( sync );
  if( eventThread )
    eventThread->onReceive();
}

void Connection::finishedInit() {
  assert( state == NeedsInitialization );
  state = ReadyToConnect;
}

void Connection::startConnecting() {
  assert( state == ReadyToConnect );
  state = Connecting;
}

void Connection::startThreads() {
	LockMutex lock( sync );

  assert( state == Connecting );
  ps->start();
  eventThread->start();
  //Thread::sleep(10);
}

void Connection::finishedConnecting() {
  LockMutex lock( sync );

  assert( state != NeedsInitialization );
  assert( state != ReadyToConnect );
  if ( state == Connecting )
    state = Connected;
}
//void Connection::onReceive(bool reliable) 
//{
//	//CRCore::ScopedLock<CRCore::crMutex> lock(m_syncMutex);
//	Buffer buf;
//	int temp = 0;
//	if ( state == Connected/* || state == Connecting */)
//	{
//		//buf = new Buffer;
//	    temp = sockets.rawRead(reliable, buf);
//	}
//	else return; //ignore the event.
//
//	if (temp == NL_INVALID) 
//	{
//		NLenum error = nlGetError();
//		if (error == NL_MESSAGE_END) 
//		{
//			processError(new Error(Error::ConnectionDropped));
//		} 
//		else 
//		{
//			processError(new Error(LowLevelError(Error::Read)));
//		}
//	} 
//	else if (temp == 0) 
//	{
//		processError(new Error(Error::ConnectionDropped));
//	} 
//	else 
//	{
//		//m_swapMutex.lock();
//		//m_receiveBuf.push_back(buf);
//		//m_swapMutex.unlock();
//		try 
//		{
//			Packet* next = NULL;
//			while ((next = PacketParser::parseNextPacket(buf)) != NULL) 
//			{
//				if (next->getType() == ExitPacket::ID) 
//				{
//					if( eventThread )       //have we not disconnected?
//						eventThread->onExit();
//					PacketParser::destroyPacket( next );
//				} 
//				else
//					ps->addIncomingPacket(next);
//			}
//
//			if( eventThread )
//				eventThread->onReceive();
//		}
//		catch ( Error& err ) 
//		{
//			processError( new Error(err) );
//		}
//		catch ( ... ) 
//		{
//			processError( new Error(Error::User) );
//		}
//	}
//}

//void Connection::onReceivePacket()
//{//eventThread 线程调用
//	//m_swapMutex.lock();
//	//m_parseBuf.swap(m_receiveBuf);
//	//m_swapMutex.unlock();
//	Packet* next = NULL;
//	Buffer *buf;
//	for( ReceiveBufferQueue::iterator itr = m_parseBuf.begin();
//			itr != m_parseBuf.end();
//			++itr )
//	{
//		buf = *itr;
//		try 
//		{
//			while ((next = PacketParser::parseNextPacket(*buf)) != NULL) 
//			{
//				if (next->getType() == ExitPacket::ID) 
//				{
//					if( eventThread )       //have we not disconnected?
//						eventThread->onExit();
//					PacketParser::destroyPacket( next );
//				} 
//				else
//					ps->addIncomingPacket(next);
//			}
//		}
//		catch ( Error& err ) 
//		{
//			processError( new Error(err) );
//		}
//		catch ( ... ) 
//		{
//			processError( new Error(Error::User) );
//		}
//        delete buf;
//	}
//	m_parseBuf.clear();
//}

void Connection::onReceive(bool reliable) {
  Buffer buf;
  int temp = 0;

  //We have to assert that the connection is still active, since we can be
  //disconnected at any time.
  {
    LockMutex lock( sync );
    if ( state == Connected || state == Connecting )
      temp = sockets.rawRead(reliable, buf);
    else
      return; //ignore the event.
  }
  if (temp == NL_INVALID) {
    NLenum error = nlGetError();
    if (error == NL_MESSAGE_END) {
      //in HawkNL 1.4b4 and later, this means that the connection was
      //closed on the network-level because the client disconnected or
      //has dropped.  If we got an ExitPacket, this will be ignored.
      processError(new Error(Error::ConnectionDropped));
    } else {
      //This is some other bad error that we need to report
      processError(new Error(LowLevelError(Error::Read)));
    }
  } else if (temp == 0) {
    //In HawkNL 1.4b3 and earlier, this _USED_ to mean that...
    //This means the connection was closed on the network-level because
    //remote computer has purposely closed or has dropped.
    //We should never get this now, because HawkNL traps this message, but
    //for completeness we check for it anyways.
	  return;
    //processError(new Error(Error::ConnectionDropped));

  } else {
    //Stream read success
    //parse the packets and add them to the PacketStream
    try {
      Packet* next = NULL;
      while ((next = PacketParser::parseNextPacket(buf)) != NULL) {
        //We want to intercept ExitPackets, else we just add it.
        if (next->getType() == ExitPacket::ID) {
          //All further errors will be ignored after we call onExit, due to
          //contract of EventThread.
          {
            LockMutex lock( sync ); //protect on eventThread
            if( eventThread )       //have we not disconnected?
              eventThread->onExit();
          }

          PacketParser::destroyPacket( next );

        } else
          ps->addIncomingPacket(next);
      }

      //Notify that packets were received.
      onReceive();

    } catch ( Error& err ) {
      //if PacketParser fails or readPacket fails.
      processError( new Error(err) );
    }
	catch ( ... ) {
		//if PacketParser fails or readPacket fails.
		processError( new Error(Error::User) );
	}
  }
}

void Connection::processError(Error* error) {
  switch(error->getCode()) {

  case Error::UnknownPacket:
    {
      LockMutex lock( sync ); //protect on eventThread
      if( eventThread )       //have we not disconnected?
        eventThread->onError(error);
    }
    break;

  default:
    {
      LockMutex lock( sync ); //lock early since unreg needs it anyway.
//      gnedbgo1(1, "processError %s", error.toString().c_str());
//	  disconnect();
      //if we don't unreg we generate endless error messages because
      //we need to unregister immediately.  So we do that.
      unreg(true, true);

      if( eventThread )       //have we not disconnected?
        eventThread->onFailure(error);
    }
    break;
  }
}

Connection::Listener::Listener(const Connection::sptr& listener, bool isReliable) 
: conn(listener), reliable(isReliable) {
}

Connection::Listener::~Listener() {
}

void Connection::Listener::onReceive() {
  conn->onReceive(reliable);
}

void Connection::reg(bool reliable, bool unreliable) 
{
  LockMutex lock( sync );
  if ( reliable && sockets.r != NL_INVALID ) {
    eGen->reg( sockets.r, Listener::sptr( new Listener( this_.lock(), true ) ) );
    gnedbgo1(3, "Registered reliable socket %i", sockets.r);
  }
  if ( unreliable && sockets.u != NL_INVALID ) {
    eGen->reg( sockets.u, Listener::sptr( new Listener( this_.lock(), false ) ) );
    gnedbgo1(3, "Registered unreliable socket %i", sockets.u);
  }
}

void Connection::unreg(bool reliable, bool unreliable) 
{
  LockMutex lock( sync );
  if ( reliable && sockets.r != NL_INVALID ) {
    eGen->unreg(sockets.r);
	sockets.r = NL_INVALID;
    gnedbgo1(3, "Unregistered reliable socket %i", sockets.r);
  }
  if ( unreliable && sockets.u != NL_INVALID ) {
    eGen->unreg(sockets.u);
	sockets.u = NL_INVALID;
    gnedbgo1(3, "Unregistered unreliable socket %i", sockets.u);
  }
}

} //Namespace GNE
