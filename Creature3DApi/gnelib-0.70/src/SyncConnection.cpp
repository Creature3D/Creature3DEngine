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
#include "../include/gnelib/SyncConnection.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/ConditionVariable.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/ConnectionParams.h"
#include "../include/gnelib/ClientConnection.h"
#include "../include/gnelib/PacketStream.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Lock.h"

namespace GNE {

SyncConnection::SyncConnection( const SmartPtr<Connection>& target )
: currError(Error::NoError), conn(target), released(false),
connectMode(false) {
  gnedbgo(5, "created");
  gnedbgo1(2, "Wrapping Connection %x into a SyncConnection.", conn);
  oldListener = conn->getListener();
}

SyncConnection::sptr SyncConnection::create( const SmartPtr<Connection>& target ) {
  sptr ret( new SyncConnection( target ) );
  ret->thisPtr = ret;
  target->setListener( ret );
  return ret;
}

SyncConnection::~SyncConnection() {
  try { 
    release();
  } catch ( const Error& ) {
  } catch ( ... ) {
    assert( false );
  }
  gnedbgo(5, "destroyed");
}

Connection::sptr SyncConnection::getConnection() const {
  return conn;
}

/**
 * \todo Check out possibility of a dynamic cast.
 */
void SyncConnection::open(const Address& dest, const ConnectionParams& params) {
  assert(!isReleased());
  //We want to "pirate" the caller's listener change request so that we remain
  //in charge as we are wrapped around the ClientConnection.
  oldListener = params.getListener();
  ConnectionParams p(params);
  sptr temp = thisPtr.lock();
  assert( temp );
  p.setListener( temp );
  //Perform the actual open
  ClientConnection::sptr cliConn = static_pointer_cast<ClientConnection>( conn );
  if ( cliConn->open(dest, p) )
    throw Error( Error::CouldNotOpenSocket );
}

/**
 * \todo Check out possibility of a dynamic cast.
 */
void SyncConnection::connect() {
  assert(!isReleased());
  ClientConnection::sptr cliConn = static_pointer_cast<ClientConnection>( conn );
  sptr temp = thisPtr.lock();
  assert( temp );
  cliConn->connect( temp );
  cliConn->join();
  checkError();
}

void SyncConnection::disconnect() {
  release();
  conn->disconnectSendAll();
}

void SyncConnection::startConnect() {
  assert(!isReleased());
  sync.acquire();
  connectMode = true;
  sync.release();
}

void SyncConnection::endConnect(bool passEvents) {
  assert(connectMode);
  //We use a LockMutex to lock sync so that if an exception occurs, sync will
  //automatically be unlocked.
  LockMutex lock(sync);

  //If the connection failed we need to ignore any further events as
  //onNewConn or onConnect did not successfully finish and that listener
  //shouldn't get onFailure or onDisconnect.
  if (!passEvents)
    oldListener = ConnectionListener::getNullListener();
  connectMode = false;
  try {
    doRelease();
  } catch (Error& e) {
    if (passEvents)
      throw e;
  }
  catch (...) {
	Error e(Error::User);
	if (passEvents)
		throw e;
  }
}

void SyncConnection::release() {
  //We use a LockMutex to lock sync so that if an exception occurs, sync will
  //automatically be unlocked.
  LockMutex lock(sync);
  doRelease();
}

void SyncConnection::doRelease() {
  if (!isReleased() && !connectMode) {
    //If we are not already released and we are not holding events
    gnedbgo1(2, "Releasing Connection %x", conn);
    released = true;
    
    //Notify any receivers there was an error, and set release error if there
    //was no error already, otherwise don't change it.
    recvNotify.acquire();
    if (currError.getCode() == Error::NoError) {
      currError.setCode(Error::SyncConnectionReleased);
      recvNotify.broadcast(); //Let people waiting for data get the error.
      recvNotify.release();
    } else {
      recvNotify.broadcast(); //Let people waiting for data get the error.
      recvNotify.release();
      conn->setListener(oldListener);
      throw currError;
    }

    //Notify the old listener for onReceive and onDoneWriting if needed, and
    //there are no errors that invalidated the stream (detected above).
    if (conn->stream().isNextPacket())
      conn->onReceive();

    //Then we can start the events up again knowing that if there was a
    //failure that onDisconnect will go to the original listener.
    conn->setListener(oldListener);
    //From this point we cannot access conn as once onDisconnect is called
    //conn could be deleted at any time.
  }
}

bool SyncConnection::isReleased() const {
  return released;
}

/**
 * \todo consider timed waits in the future, but this won't be needed when
 *       the %GNE protocol implementation is finished and detects timeouts.
 * \todo This probably could be optimized quite a bit -- each packet gets
 *       copied twice!
 */
SyncConnection& SyncConnection::operator >> (Packet& packet) {
  //We have to acquire the mutex now so that an error cannot occur between
  //checkError and our wait.
  {
    LockCV lock( recvNotify );

    checkError();
    assert(!isReleased());
    while (!conn->stream().isNextPacket()) {
      recvNotify.wait();
      //Check if we were woken up due to an error.
      checkError();
    }
  }

  //Now that we have some data, do the actual receiving.
  Packet* recv = conn->stream().getNextPacket();
  assert(recv != NULL);  //There had better be some data!
  if (recv->getType() != packet.getType()) {
    gnedbgo2(1, "Packet type mismatch.  Got %d, expected %d.",
                recv->getType(), packet.getType());
    throw PacketTypeMismatch( recv->getType() );
  }

  //Copy the packet.
  //The original method was to use operator=() but doing it this way makes
  //things complicated in several ways, including how to overload it, and the
  //fact that you can't copy the children to children w/o another operator.
  //This should be optimized in the future.  This new method will be easier
  //to optimize later to allow better optimizations.
  Buffer temp;
  recv->writePacket(temp);
  temp.flip();
  NLubyte dummy;
  temp >> dummy; //Skip the ID code.
  packet.readPacket(temp);

  return *this;
}

SyncConnection& SyncConnection::operator << (const Packet& packet) {
  checkError();
  assert(!isReleased());

  conn->stream().writePacket(packet, true);

  return *this;
}

void SyncConnection::onNewConn(SyncConnection& newConn) {
  //newConn should be this object.  We don't do any double wrapping.
  assert(this == &newConn);
  assert(!isReleased());
  oldListener->onNewConn(newConn);
}

void SyncConnection::onConnect(SyncConnection& newConn) {
  //newConn should be this object.  We don't do any double wrapping.
  assert(this == &newConn);
  assert(!isReleased());
  oldListener->onConnect(newConn);
}

void SyncConnection::onConnectFailure( Connection& conn2, const Error& error ) {
  assert( conn.get() == &conn2 );
  setError(error);
}

void SyncConnection::onDisconnect( Connection& conn2 ) {
  assert( conn.get() == &conn2 );
  //This should never happen.  An error should occur first, and at that time
  //we are released, and the onDisconnect event should be sent to the
  //original listener.
  assert(false);
}

void SyncConnection::onError( Connection& conn2, const Error& error ) {
  assert( conn.get() == &conn2 );
  conn2.disconnect();
  //Turn errors on a SyncConnection into a failure.
  onFailure( conn2, error);
}

void SyncConnection::onFailure( Connection& conn2, const Error& error ) {
  assert( conn.get() == &conn2 );
  setError(error);
  //Stop the event thread until release properly restarts it.
  conn2.setListener( ConnectionListener::sptr() );
}

void SyncConnection::onExit( Connection& conn2 ) {
  assert( conn.get() == &conn2 );
  setError( Error(Error::ExitNoticeReceived) );
  //Stop the event thread until release properly restarts it.
  conn2.setListener( ConnectionListener::sptr() );
}

void SyncConnection::onReceive( Connection& conn2 ) {
  assert( conn.get() == &conn2 );

  //Notify anyone who is waiting for data to come in (namely operator <<).
  LockCV lock( recvNotify );
  recvNotify.signal();
}

void SyncConnection::checkError() {
  recvNotify.acquire();
  bool isErr = (currError.getCode() != Error::NoError);
  recvNotify.release();

  if (isErr) {
    //We release when we discover the error rather than when we set it
    //because we can't release from the event thread.
    release();
    //We can throw currError (which will make a copy) because once an error
    //is set it does not change.
    throw currError;
  }
}

void SyncConnection::setError( const Error& error ) {
  //We don't need to lock sync here because events can't occur while release
  //is setting its "released" error since setListener blocks.  We couldn't
  //lock it anyways for the same reason as well.
  recvNotify.acquire();
  if (currError.getCode() == Error::NoError)
    currError = error;
  recvNotify.broadcast(); //Let people waiting for data get the error.
  recvNotify.release();
}

} //namespace GNE
