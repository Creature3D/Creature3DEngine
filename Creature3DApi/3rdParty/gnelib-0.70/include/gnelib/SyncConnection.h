#ifndef SYNCCONNECTION_H_INCLUDED_C43C0621
#define SYNCCONNECTION_H_INCLUDED_C43C0621

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
#include "export.h"
#include "Error.h"
#include "ConnectionListener.h"
#include "ConditionVariable.h"
#include "Mutex.h"

namespace GNE {
class Address;
class Connection;
class Packet;
class ClientConnection;
class ServerConnection;
class ConnectionParams;

/**
 * @ingroup midlevel
 *
 * A class for performing syncronous connections.  All methods of this class
 * except writing block until completion.  This is useful while
 * connecting or performing trivial connections/communications.  Many times
 * if you want to perform non-game transfers, for example a server-query
 * connection or another simple service it might be simpler to spawn a new
 * thread and run the SyncConnection while the rest of the program runs.
 *
 * SyncConnections throw an Error class on an error, so you must use
 * try/catch blocks.  Note that the Error class can't describe everything
 * about an error, so sometimes more specific information is available in the
 * debug logs (enable them with GNE::initDebug).
 *
 * All transfers with SyncConnection are reliable (using TCP, SPX, or similar
 * protocol).
 *
 * If an error occurs, the connection was terminated.  The underlying
 * connection is disconnected, and this SyncConnection becomes released.
 *
 * When you wrap a Connection with this class, the event listener for that
 * Connection is suspended and SyncConnection "takes over" until it is
 * release()d.  So while in syncronous mode you will receive no events
 * through your asyncronous event listener, with the exception of
 * onDisconnect and onConnect.  It is best only to use SyncConnection
 * while connecting, because of possible side-effects when you
 * wrap the Connection.  If there was already data pending that you did not
 * receive in asyncronous mode -- it was not lost, but you will get it from
 * the next packet read.  If you start out with a SyncConnection, then you
 * can be certain no unexpected packets will be arriving.
 *
 * See the example exsynchello for more help with the usage of this class.
 */
class GNE_EXPORT SyncConnection : public ConnectionListener {
public: //typedefs
  typedef SmartPtr<SyncConnection> sptr;
  typedef WeakPtr<SyncConnection> wptr;

private: //ctor is private because of member thisPtr
  /**
   * @see create
   */
  SyncConnection( const SmartPtr<Connection>& target );

public:
  /**
   * Creates a new SyncConnection.  Pass in the Connection that you want to
   * wrap.  See the details above for more information.
   */
  static sptr create( const SmartPtr<Connection>& target );

  /**
   * Destructs this SyncConnection, calling release() if necessary.  If
   * releasing would throw an Error, it is ignored.  If you wish to capture
   * all errors, you should call release yourself.
   *
   * @see release()
   */
  virtual ~SyncConnection();

  /**
   * Returns the underlying Connection.
   */
  SmartPtr<Connection> getConnection() const;
  
  /**
   * Just like ClientConnection::open, this will open the port, ready for
   * connection.  If the open failed, an Error is thrown.
   *
   * It is important that the wrapped Connection is a ClientConnection, 
   * otherwise undefined behavior (likely a crash) will result.
   *
   * @throw Error if the connection could not be opened.
   */
  void open(const Address& dest, const ConnectionParams& params);
  
  /**
   * Attempts to connect to the remote side, and waits for the connection to
   * complete, throwing an Error if it was unsuccessful.  Your event listener
   * will receive the onConnect event resulting from this -- it is not
   * repressed just like onDisconnect is not repressed.  Many times though,
   * when using this method you won't have a need for onConnect, but if you
   * do create one, connect will wait until onConnect is finished.
   *
   * It is important that the wrapped Connection is a ClientConnection, 
   * otherwise undefined behavior (likely a crash) will result.
   *
   * @throw Error if the connection failed.
   */
  void connect();
  
  /**
   * Disconnects the underlying Connection.  It is best to use this function
   * instead of getConnection()->disconnect() because this will make sure
   * any pending writes will have completed through a call to release().
   *
   * @throw Error if release throws an error, or if the disconnection was
   *              unsuccessful.
   */
  void disconnect();

  /**
   * Releases this SyncConnection, returning the underlying Connection to its
   * previous event handler.  Once a SyncConnection has been released, then
   * it is essentially in an invalid state and should not be used anymore
   * (with the exception of the dtor, getConnection(), isReleased(), and
   * this function).  This function throws an Error if some underlying
   * pending operations failed since the last call on this SyncConnection.
   *
   * If release is called when the SyncConnection is already released, no
   * errors will be thrown.
   *
   * onReceive will be called in the original listener after release if
   * necessary, and onDoneWriting will be called after release if any data
   * since writing packets.
   *
   * @throw Error if an error has occured since the last operation on this
   *              SyncConnection instance.
   */
  void release();
  
  /**
   * Returns true if release() has been called on this SyncConnection, and it
   * is not a valid object to use anymore.
   */
  bool isReleased() const;
  
  /**
   * Reads a packet from the connection.  You should provide an already
   * allocated packet whose Packet::readPacket function will be called.
   * There will be type checking performed before this call to make sure the
   * right packet is being read.  If there is a mismatch, an error is thrown.
   * The passed packet is untouched, and the connection remains connected;
   * however, the data just received (the incorrect packet) is lost.
   * The connection will remain connected in this case.
   *
   * @throw PacketTypeMismatch if the read packet was of the wrong type.
   * @throw Error if an error occured while reading, or an error occured
   *              since the last interaction with this object.
   */
  SyncConnection& operator >> (Packet& packet);
  
  /**
   * Writes a Packet to the connection by placing it in the outgoing queue.
   * This method actually doesn't block like every other SyncConnection
   * method, but on a write there is no reason to block, since it will not
   * effect the logic of the code.  This allows for packet caching to improve
   * network performance, and allows you to perform reads while the
   * connection is still writing.  All of this is transparent to your logic,
   * though.  release() will block until all writes are completed, and the
   * destructor and disconnect() function call release() if needed.
   *
   * A SyncConnection should be released or destroyed before the start of
   * %GNE shutdown if you want to guarantee all packet sends were attempted.
   *
   * @throw Error if an error occured while writing, or an error occured
   *              since the last interaction with this object.
   */
  SyncConnection& operator << (const Packet& packet);
  
private:
  /**
   * Not for use for the end-user API.  During connection, events need to be
   * treated a little differently, as well as releasing, because onNewConn or
   * onConnect may not have been called or are still processing, and we don't
   * want to release.  This is used only by the GNE protocol connecting code,
   * and is used there to help guarantee that onNewConn/onConnect is the
   * first event, and onDisconnect the last if onNewConn/onConnect ever
   * finished.  Basically this "delays" the release of this SyncConnection.
   *
   * This must be called BEFORE any events can possibly be received (so
   * before registration into ConnectionEventGenerator).
   */
  void startConnect();

  /**
   * Complement function with startConnect, this must be called if
   * startConnect was called.  Pass true to passEvents if the connection was
   * successful and onFailure and onDisconnect should be passed onto the old
   * listener.
   *
   * Pass false if the connection failed and the events should be discarded
   * and not passed on.  If false was passed, the listener for the connection
   * was set to ConnectionListener::getNullListener() to discard the events.
   * If false is passed, and release throws an Error, it is not passed
   * through.
   *
   * @throw Error if release throws.
   */
  void endConnect(bool passEvents);

  //Make friends so they can use startConnect and endConnect.
  friend class ServerConnection;
  friend class ClientConnection;
  
  /**
   * The actual releasing functionality, but in a separate function so that
   * calling functions can manipulate the mutexes, which are not necessarily
   * recursive.
   *
   * @throw Error if an error occured while releasing, or an error occured
   *              since the last interaction with this object.
   */
  void doRelease();

  /**
   * The event listeners for SyncConnection that will override the current
   * listener our connection has.
   */
  virtual void onConnect(SyncConnection&);
  virtual void onConnectFailure(Connection&, const Error&);
  virtual void onNewConn(SyncConnection&);
  virtual void onDisconnect( Connection& );
  virtual void onExit( Connection& );
  virtual void onError( Connection&, const Error&);
  virtual void onFailure( Connection&, const Error&);
  virtual void onReceive( Connection& );

  /**
   * A ConditionVariable we can wait on to wait for incoming data.
   * This is also used as a mutex for errors, as waiting for incoming data
   * is also sensitive to if an error happens -- it will stop waiting if the
   * connection failed.
   */
  ConditionVariable recvNotify;

  /**
   * Checks to see if an error has occured, and if so, throws it.
   *
   * @throw Error if an error has occured.
   */
  void checkError();

  /**
   * Sets the error.
   */
  void setError(const Error& error);

  /**
   * Weak pointer to this, set in create.
   */
  wptr thisPtr;

  /**
   * Syncronization for release and connecting events.
   */
  Mutex sync;

  /**
   * The current error state of this connection.  Error::NoError if there is
   * no error.  recvNotify acts to keep this variable from being accessed by
   * multiple threads.
   */
  Error currError;

  /**
   * The underlying Connection.
   */
  SmartPtr<Connection> conn;

  /**
   * The old listener for asyncronous communications that the Connection just
   * had.  If this is NULL, then this object has been released.
   */
  SmartPtr<ConnectionListener> oldListener;

  volatile bool released;

  /**
   * @see setConnectMode
   */
  volatile bool connectMode;
};

} // namespace GNE

#endif /* SYNCCONNECTION_H_INCLUDED_C43C0621 */
