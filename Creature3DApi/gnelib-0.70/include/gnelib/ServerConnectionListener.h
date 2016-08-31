#ifndef _SERVERCONNECTIONLISTENER_H_
#define _SERVERCONNECTIONLISTENER_H_

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
#include "ReceiveEventListener.h"
#include "Connection.h"
#include "SmartPtr.h"
#include "WeakPtr.h"
#include "Mutex.h"

namespace GNE {
class Address;
class ConnectionListener;
class ServerConnection;
class ConnectionParams;

/**
 * @ingroup midlevel
 *
 * A connection that listens for other connections.  Inherit from this class,
 * overriding the functions from Connection and ClientConnection based on the
 * events you wish to respond to.
 *
 * You need to create a static "create" method like other classes in GNE since
 * ServerConnectionListener is managed by a SmartPtr.  There exists a master
 * list of ServerConnectionListener objects that are actively listening.
 * Therefore you don't need to keep the reference around to keep the object
 * from getting prematurely deleted.
 *
 * When %GNE closes, all active listeners will be shutdown, so the close
 * operation is optional.  You can close early by calling the close method.
 * The static method closeAllListeners is also available to you.
 *
 * All of the methods in this class are safe to call from multiple threads at
 * the same time unless otherwise noted.
 */
class GNE_EXPORT ServerConnectionListener {
public:
  typedef SmartPtr<ServerConnectionListener> sptr;
  typedef WeakPtr<ServerConnectionListener> wptr;

protected:
  /**
   * Initalizes this class.
   */
  ServerConnectionListener();

public:
  /**
   * Destructor.  Closes the listening connection if it is open.
   */
  virtual ~ServerConnectionListener();

  /**
   * For all active listeners, calls close on them.  This method is implicitly
   * called when GNE shuts down.
   */
  static void closeAllListeners();

  /**
   * Opens a socket ready for listening, but not yet listening.  If the socket
   * has already been opened, this method has no effect and returns false.
   * This fact may be an issue, since if you call open with one port, calling
   * open again won't change the port unless you close.
   *
   * Unless you restrict your usage of the open function to a single thread or
   * use an external mutex, it is impossible to safely change port.  This is
   * usually not a problem as most users will initialize the listener from only
   * a single thread.
   *
   * @param port the port to listen on.
   * @return true if could not open a socket on the port.
   */
  bool open(int port);

  /**
   * Closes the listening connection.  It is OK to call close at any time,
   * even if already closed or never opened.  You can start the listener again
   * by calling open and listen.
   *
   * The close operation is optional.  %GNE will shut down active listeners
   * when it is shut down.
   *
   * Note that a listener can be closed while an event is being processed.
   * This behaviour was chosen because it matches the behaviour of the
   * previous %GNE 0.55 API, and because it avoids using mutexes, which keep
   * the events from running in parallel even if they are capable.  This
   * should not affect the events except that the getLocalAddress method may
   * return an invalid address.
   */
  void close();

  /**
   * Starts this socket listening.  onNewConn will be called when a new
   * connection has been negotiated and error checked.
   *
   * This method also registers the listener into the master list, so you after
   * this method is called you no longer need to keep the reference.
   *
   * If already listening, this method has no effect and returns false.
   *
   * While a listener is listening, it is added to a master list so that it
   * won't be destroyed prematurely.  It is removed from the list when the
   * listener is closed (this includes when all listeners are shutdown when
   * %GNE is shut down).
   *
   * @pre listener has been opened with open.
   * @see onListenFailure
   * @see onListenSuccess
   * @see open
   * @return true, if there was an error starting the listen
   */
  bool listen();

  /**
   * Returns true if this ServerConnectionListener is active.
   */
  bool isListening() const;

  /**
   * Returns the address of the listening socket.  If the listener has not
   * been opened or is not listening, an invalid address is returned.
   *
   * Most notably, a listener can be closed while an event is being processed.
   *
   * @see close
   */
  Address getLocalAddress() const;

protected:
  /**
   * You must call this from your create function BEFORE exiting it.
   */
  void setThisPointer( const sptr& thisPointer );

  /**
   * There was a failure when trying to listen on this socket.  This is not
   * called when the actual low-level listen fails (that error is returned
   * from listen), but instead high-level errors while connecting such as a
   * version mismatch are sent here.  Only errors that occur before or during
   * the ConnectionListener::onNewConn event are sent here.
   *
   * This function may be called from multiple threads at the same time, and
   * may block if it wishes; it does not have the "non-blocking" restriction
   * most GNE events have.
   *
   * Depending on when and under what circumstances the failure happened, in
   * any case, from might be an invalid address, and listener may be NULL.
   *
   * @param error the type of error
   * @param from the remote address the failed attempt came from
   * @param listener the unused ConnectionListener that was obtained from
   *                 getNewConnectionParams.  If it is NULL, then
   *                 getNewConnectionParams was never called.
   */
  virtual void onListenFailure(const Error& error, const Address& from,
                               const SmartPtr<ConnectionListener>& listener) = 0;

  /**
   * This is an optional event to catch which is called after
   * ConnectionListener::onNewConn.  The default behavior for this event is
   * to do nothing, but you may overload it to catch it.
   *
   * This method may block, but until it completes the connection will be
   * marked as connecting and not as connected, and listener will not start
   * to receive any events (besides the onNewConn it just got) until this
   * function completes.
   *
   * See the expong example of a case where the onListenSuccess event can be
   * useful, where the same implementation using only onNewConn would be
   * harder to program and understand.
   *
   * This function can be called from multiple threads at the same time.
   *
   * @param listener The listener than you returned from
   *                 getNewConnectionParams.
   */
  virtual void onListenSuccess(const SmartPtr<ConnectionListener>& listener);

  /**
   * A new connection is starting, and now we want the parameters for this
   * new connection.  The parameters passed should be modified to give
   * the new connection's flow control parameters and listener.
   *
   * This function can be called from multiple threads at the same time.
   */
  virtual void getNewConnectionParams(ConnectionParams& params) = 0;

  virtual bool tryAddNewConnection(NLsocket socket) = 0;
  virtual void removeNewConnectionTry(NLsocket socket) = 0;
private:
  //interface functions solely for ServerConnection
  friend class ServerConnection;

  //performs the actual close operation w/o removing from list.
  void rawClose();

  //These methods relay the message on.
  void processOnListenFailure( const Error& error, const Address& from,
                               const SmartPtr<ConnectionListener>& listener);

  void processOnListenSuccess( const SmartPtr<ConnectionListener>& listener );

private:
  class ServerListener : public ReceiveEventListener {
  public:
    typedef SmartPtr<ServerListener> sptr;
    typedef WeakPtr<ServerListener> wptr;

  public:
    ServerListener(const ServerConnectionListener::sptr& listener);

    virtual ~ServerListener();

    void onReceive();

  private:
    ServerConnectionListener::sptr conn;

  };
  friend class ServerListener;
  friend class ServerConnection;

  void onReceive();

  wptr this_;

  bool listening;

  //modify by wch
  //NLsocket socket;
  SocketPair sockets;

  mutable Mutex sync;
};

}
#endif
