#ifndef CONNECTION_H_INCLUDED_C51DC478
#define CONNECTION_H_INCLUDED_C51DC478

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
#include "Thread.h"
#include "PacketStream.h"
#include "ReceiveEventListener.h"
#include "Error.h"
#include "Errors.h"
#include "SocketPair.h"
#include "Address.h"
#include "ConnectionStats.h"
#include "SmartPtr.h"
#include "WeakPtr.h"
#include <vector>
#include <set>
//#include <CRCore/thread/crMutex.h>
namespace GNE {
class ConnectionListener;
class EventThread;
class SyncConnection;

/**
 * @ingroup midlevel
 *
 * A class resembling any type of connection to a remote computer.  A
 * connection also manages flow control through its PacketStream.
 *
 * When you create a new Connection, you will want to register a
 * ConnectionListener for it so you can receive its events.
 *
 * You should never create Connection objects directly.  You want to create
 * a ClientConnection (ServerConnection objects are created internally by
 * the library).
 *
 * @see ClientConnection::create
 */
class GNE_EXPORT Connection {
protected:
  /**
   * Intializes this class with a NULL listener, which must be set before any
   * operations are performed on this instance.
   *
   * THE CHILD CLASS IS RESPONSIBLE FOR SETTING THE THISPOINTER.  This is
   * because the EventThread needs a SmartPtr to this Connection, which is
   * unknown during construction.
   *
   * @see setThisPointer()
   */
  Connection();

public:
  typedef SmartPtr<Connection> sptr;
  typedef WeakPtr<Connection> wptr;

public: //static methods
  /**
   * Disconnects all active connections in the GNE system.
   */
  static void disconnectAll();

  /**
   * Represents the states a Connection object can be in.
   */
  enum State {
    NeedsInitialization, /**< Connection was just created but needs initializing. */
    ReadyToConnect, /**< Connection is ready to connect. */
    Connecting,     /**< Connection is in the middle of connecting. */
    Connected,      /**< Connection is connected. */
    Disconnecting,  /**< Connection is in the middle of disconnecting. */
    Disconnected    /**< Connection has finished and cannot be restarted. */
  };

public:

  /**
   * Destructor.
   */
  virtual ~Connection();

  /**
   * Returns the currently registered event Listener.  After GNE 0.55 this
   * method changed so that the return value of this method is either the
   * listener you have set or an empty SmartPtr.  It will return an empty
   * SmartPtr if the Connection has been disconnected, but since this can
   * happen at any moment, it can return an empty SmartPtr at any time.
   *
   * @deprecated The semantics of method have changed to pretty much make it
   *             useless.  It's probably dangerous to provide this, so I'm
   *             considering removing it.  If you have a strong case to
   *             leave it in, please tell me.
   */
  SmartPtr<ConnectionListener> getListener() const;

  /**
   * Sets a new event listener.  All new events will be sent to the new
   * listener.  This method has no effect on an event currently being processed
   * and this method will not block to wait until the listener change has
   * taken place.  The next method generated will go to the newly set listener.
   *
   * If the passed listener is NULL, this will hold the event thread.  This
   * does NOT stop event generation -- generated events still will enter the
   * queue but the actual event function will not be called until a proper
   * listener is set.  If you hold the event thread, a new listener should be
   * set quickly as the connection is basically "frozen" during this time.
   *
   * You MUST have an event listener set because the event thread must call
   * its required events before this object can be destroyed.
   *
   * If you care not to receive events because you are using SyncConnection,
   * you will probably want to set ConnectionListener::getNullListener as the
   * listener to discard all events before you wrap this Connection with a
   * SyncConnection.
   */
  void setListener( const SmartPtr<ConnectionListener>& listener);

  /**
   * Returns the timeout for this connection.
   *
   * @see setTimeout
   */
  int getTimeout();

  /**
   * Sets the timeout for this connection in milliseconds.  A value of 0
   * signifies that no timeouts should occur.  Values less than 0 are invalid.
   *
   * An onTimeout event occurs ms milliseconds after the last onReceive event,
   * the last onTimeout event, or since setTimeout occured.  Any of these
   * events sets the "timeout counter" back to zero to say it another way.
   *
   * @see GNE::ConnectionListener::onTimeout
   */
  void setTimeout(int ms);

  /**
   * Returns the PacketStream associated with this class.  Writing and
   * reading through this Connection is done through this PacketStream.  You
   * should not keep a reference to this PacketStream object any longer than
   * you keep the reference to the Connection object.
   */
  PacketStream& stream();

  /**
   * If stats is enabled, returns Connection stats.
   * @param reliable <ul>
   *                 <li>If greater than 0, returns reliable stats only.</li>
   *                 <li>If 0, returns unreliable transfer stats only.</li>
   *                 <li>If less than 0, returns cumulative stats for
   *                     both.</li></ul>
   * @return various connection stats.
   * @see ConnectionStats
   * @see GNE::enableStats
   */
  ConnectionStats getStats(int reliable) const;

  /**
   * Returns the local address of this connection.  If the requested socket
   * has not been opened, the returned Address will be invalid (!isValid()).
   * An invalid address is also returned if the Connection was disconnected,
   * which can happen at any time.
   *
   * @param reliable sometimes two sockets are used for reliable and
   *                 unreliable data.  Specify which low-level socket you
   *                 want the address of.
   * @see GNE::Address
   */
  Address getLocalAddress(bool reliable) const;

  /**
   * Returns the remote address of this connection.  If the requested socket
   * is not connected, the returned Address will be invalid (!isValid()).
   * An invalid address is also returned if the Connection was disconnected,
   * which can happen at any time.
   *
   * @param reliable sometimes two sockets are used for reliable and
   *                 unreliable data.  Specify which low-level socket you
   *                 want the address of.
   * @see GNE::Address
   */
  Address getRemoteAddress(bool reliable) const;

  /**
   * Returns the state this Connection is in.  Note that the state of the
   * connection can change at any moment after this call by %GNE if the state
   * is Connecting or Connected.
   *
   * @see State
   */
  State getState() const;

  /**
   * Returns true if this Connection is active and ready to send/recieve.
   * A Connection may become disconnected at any time, as another thread or
   * an event may cause a disconnect.
   *
   * This method returns true if the state is currently "Connected", false if
   * any other state.
   *
   * @see getState
   */
  bool isConnected() const;

  /**
   * Immediately disconnects this socket.  No more data will be recieved or
   * sent on this socket.  If you want to disconnect more nicely, use
   * disconnectSendAll.  It is okay to call this function even when this
   * Connection is already disconnected.  Note that even though the
   * connection is disconnected immediately, it is still a graceful close
   * such that an onExit event will be generated on the remote connection if
   * possible, but any packets pending to be sent won't get sent.
   *
   * It is valid to call disconnect while the Connection object is still
   * connecting -- this can be used to "abort" or cancel a connection attempt.
   *
   * NOTE: You may not reconnect this connection object after calling
   * disconnect.
   *
   * @see #disconnectSendAll()
   */
  void disconnect();

  /**
   * A blocking call that disconnects nicely.  This has the same effect as
   * disconnect, except that all data waiting to be sent will be sent if
   * possible.  If the connection is already disconnected, this function does
   * nothing.
   *
   * @param waitTime the max amount of time in ms to wait for the outgoing
   *        packet queue to clear.
   *
   * @see #disconnect()
   */
  void disconnectSendAll(int waitTime = 10000);

  //void onReceivePacket();
  typedef std::set<int> CodeSet;
  bool checkCode(int code);
  void update();
protected:
	Mutex m_codesetMutex;
	CodeSet *m_codeset;
	CodeSet *m_codeset_buf;
  /**
   * This method must be called and set to a weak pointer referencing this
   * object before the end of the static creat	e function of the child class.
   * This is needed since some things Connection registers with requires a
   * SmartPtr to itself.
   */
  void setThisPointer( const wptr& weakThis );

  /**
   * A weak pointer to this own object.
   */
  wptr this_;

  /**
   * A utility function for ServerConnection and ClientConnection to add the
   * standard header onto the connection packets.
   */
  void addHeader(Buffer& raw);

  /**
   * A utility function for ServerConnection and ClientConnection to add the
   * version information to a packet.
   */
  void addVersions(Buffer& raw);

  /**
   * A utility function for ServerConnection and ClientConnection to check to
   * verify the %GNE header of the connecting packets.  The Buffer is read
   * from so this function will move the reading position to the next element
   * after the header.  If it throws an exception, the read position is
   * undefined.
   *
   * @param t the type of ProtocolViolation to throw if it throws one.
   * @throw ProtocolViolation if the next bytes of the Buffer is not the
   *                          proper %GNE header.
   */
  void checkHeader(Buffer& raw,
    ProtocolViolation::ViolationType t = ProtocolViolation::OtherViolation);

  /**
   * A utility function for ServerConnection and ClientConnection to check
   * the GNE version, game name, and user versions in the Buffer.  This
   * function behaves similarly to checkHeader.
   *
   * @throw Error with a code of Error::GNETheirVersionHigh or
   *   Error::GNETheirVersionLow if the GNE protocol versions do not match.
   * @throw WrongGame if the game names don't match.
   * @throw UserVersionMismatch if the user version numbers don't match.
   */
  void checkVersions(Buffer& raw);

  //For information about events, see the ConnectionListener class.
  void onReceive();

  /**
   * The pair of sockets.
   */
  SocketPair sockets;

  /**
   * The PacketStream associated with this Connection.  This object also
   * contains information about the in and out connection rates.
   */
  PacketStream::sptr ps;

  /**
   * This SmartPtr participates in a cycle, so we have to be careful to reset
   * it when we disconnect.
   */
  SmartPtr<EventThread> eventThread;

  /**
   * The Connection has finished initializing and the state now can change
   * to ReadyToConnect.
   *
   * @pre state is now NeedsInitialization
   */
  void finishedInit();

  /**
   * Moves from ReadyToConnect to Connecting state.
   */
  void startConnecting();

  /**
   * Does the work needed to start up the PacketStream and EventThread thrads.
   *
   * @pre state must be Connecting.
   */
  void startThreads();

  /**
   * The connecting has just finished and the state needs to be changed.
   */
  void finishedConnecting();

  /**
   * Register this Connection object's sockets with the
   * ConnectionEventGenerator.  Pass true to register each socket type.
   * Usually both will be registered, but in the future there may exist a
   * new connection type that only uses one of the sockets.
   *
   * The internal class ConnectionListener will be used to trigger the proper
   * onReceive(bool) event.
   *
   * The sockets will be unregistered automatically when the object is
   * destroyed, but they may be unregistered explicitly at an earlier time
   * through unreg(bool, bool).
   *
   * Registering a socket already registered will have no effect.
   *
   * @see #onReceive(bool)
   * @see #unreg(bool, bool)
   */
  void reg(bool reliable, bool unreliable);

  /**
   * Unregistered the sockets which are specified by passing true to the
   * appropriate parameter.  This function call is optional -- when the
   * object is destroyed, unregistering is automatic if socket was
   * registered.  Trying to unregister sockets that are already unregistered
   * will result in no effect.
   */
  void unreg(bool reliable, bool unreliable);

  //typedef std::vector<Buffer*> ReceiveBufferQueue;//add by wch
private:
  class Listener : public ReceiveEventListener {
  public:
    typedef SmartPtr<Listener> sptr;
    typedef WeakPtr<Listener> wptr;

  public:
    Listener(const Connection::sptr& listener, bool isReliable);

    virtual ~Listener();

    void onReceive();

  private:
    Connection::sptr conn;

    bool reliable;

  };

  volatile State state;

  //a copy of the information set in setTimeout so we can return a value after
  //eventThread is reset.
  int timeout_copy;

  /**
   * Make Listener a friend so it can call our onRecieve(bool)
   * event, which will properly parse the packets.
   */
  friend class Listener;

  /**
   * Used to keep some functions thread-safe.  Guards access to sockets, and
   * eventThread.
   */
  mutable Mutex sync;
  //mutable CRCore::crMutex m_syncMutex;

  //PacketStream might call our processError function.
  friend class PacketStream;

  //SyncConnection might call our event functions.
  friend class SyncConnection;

  /**
   * Parses the packets recieved, then calls onReceive.
   */
  void onReceive(bool reliable);

  /**
   * Determines whether the error given is fatal or non-fatal, and calls the
   * appropriate event, and handles disconnects if necessary.
   */
  void processError(Error* error);

  //ReceiveBufferQueue m_receiveBuf;//add by wch
  //ReceiveBufferQueue m_parseBuf;//add by wch
  //CRCore::crMutex m_swapMutex;
};

}
#endif /* CONNECTION_H_INCLUDED_C51DC478 */
