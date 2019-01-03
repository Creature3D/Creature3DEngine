#ifndef CONNECTIONLISTENER_H_INCLUDED_C43554C0
#define CONNECTIONLISTENER_H_INCLUDED_C43554C0

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
#include "SmartPtr.h"
#include "WeakPtr.h"

namespace GNE {
class Error;
class SyncConnection;
class Connection;

/**
 * @ingroup midlevel
 *
 * This is an abstract base class representing an interface to the Connection,
 * ClientConnection, and ServerConnection classes.  All of the events that
 * those three classes generate are in this class, and classes listening to
 * the events on these classes will inherit from this class.
 *
 * Override the events you want to receive with this listener.  The provided
 * functions do nothing, so that event overloading is optional in all cases.
 *
 * onConnect and onConnectFailure are generated only by ClientConnection and
 * onNewConn is generated only by ServerConnection.
 *
 * Note that there is only one event thread per Connection, so one one event
 * will be active at a time.  Take this into consideration that you may be
 * able to eliminated some syncronization and mutexes that would otherwise be
 * needed.  onNewConn or onConnect or onConnectFailure are guaranteed to be
 * the first events from a connection, and won't be called multiple times.
 * onDisconnect will always be the last called (except after
 * onConnectFailure, where there never was a connection) and will only be
 * called once.  Since only one event can be active at a time, you will want
 * to return quickly so other events can be processed, but note that if you
 * have a PacketFeeder set up for a connection, those events are independent
 * of ConnectionListener's events, so both types of events can be occuring
 * at the same time.
 *
 * @see ServerConnectionListener::onListenFailure
 */
class GNE_EXPORT ConnectionListener {
public: //typedefs
  typedef SmartPtr<ConnectionListener> sptr;
  typedef WeakPtr<ConnectionListener> wptr;

public:
  virtual ~ConnectionListener();

  /**
   * A Connection must always have a non-NULL listener if it is to be used,
   * but if you will only be using a SyncConnection for the whole time, there
   * is no need to create your own listener.  This listener will ignore all
   * events.
   */
  static sptr getNullListener();

  /**
   * Event triggered after there is a successful connection.  The connection
   * process will not be considered complete until this function completes.
   *
   * If an error occurs then the connection needs to be killed, so conn can
   * throw its Error outside your function.  Catch it if you need to clean up
   * anything you were doing, but remember to rethrow it.  If this is the
   * case, onConnectFailure but not onDisconnect will be generated, so
   * any needed cleanup needs to be done in your exception handler (or class
   * destructor).
   *
   * You can also choose to refuse a connection by throwing an Error with an
   * error code of Error::ConnectionRefused, and will it will make the
   * connection act as if an error just occured, so onListenFailure will be
   * called and the above section of text still applies.
   *
   * This event does not have the "non-blocking" requirement that most GNE
   * events have.  Take as long as needed to establish a connection, such as
   * transmitting initial game parameters or game state.
   *
   * Note: Only ClientConnection generates this event.  The SyncConnection
   * is currently wrapped around a ClientConnection, and you should use
   * SyncConnection::getConnection to get the ClientConnection.
   *
   * @throw Error if an error occurs and the connection was not successful.
   */
  virtual void onConnect(SyncConnection& conn);

  /**
   * Event triggered when a connection failed before or during the onConnect
   * event.
   *
   * After a connection failure, the connection is as if disconnect() was
   * called, therefore you cannot reconnect this connection instance.
   *
   * Note that for this case, the event onDisconnect IS NOT CALLED, since
   * the Connection was never in a connected state.
   *
   * This event's purpose is to notify this listener if needed.  If you are
   * waiting for the connection to complete by using join in that other
   * thread, you would probably not need to listen for this event (see
   * the example exhello).
   *
   * Note: Only ClientConnection generates this event.
   * @see Connection::disconnect()
   */
  virtual void onConnectFailure(Connection& conn, const Error& error);

  /**
   * Event triggered when a new connection has been negotiated and error
   * checked.  This object is a newly allocated object created from your
   * ServerConnectionCreator object, and this function will be the first time
   * you code has "seen" this object, so you will have to register it into
   * some internal list if you want to interact with it later.
   *
   * If an error occurs then the connection needs to be killed, so conn can
   * throw its Error outside your function.  Catch it if you need to clean up
   * anything you were doing, but remember to rethrow it.  If this is the
   * case, ServerConnectionListner::onListenFailure will be generated but not
   * onDisconnect, so any needed cleanup needs to be done in your exception
   * handler or in onListenFailure.  See ServerConnection::run() for an event
   * generation summary.
   *
   * You can also choose to refuse a connection by throwing an Error with an
   * error code of Error::ConnectionRefused, and will it will make the
   * connection act as if an error just occured, so onListenFailure will be
   * called and the above section of text still applies.
   *
   * Note: Only ServerConnection generates this function.  The SyncConnection
   * is currently wrapped around a ServerConnection, and you should use
   * SyncConnection::getConnection to get the ServerConnection.
   *
   * If the connection failed before this event, though, the function
   * ServerConnectionListener::onListenFailure instead of this function is
   * called.  If onNewConn succeeds, ServerConnectionListener::onListenSucess
   * is called, and the connection process will be completed after that event
   * completes.
   *
   * This event does not have the "non-blocking" requirement that most GNE
   * events have.  Take as long as needed to establish a connection, such as
   * transmitting initial game parameters or game state.
   *
   * @throw Error if an error occurs and the connection was not successful.
   */
  virtual void onNewConn(SyncConnection& newConn);

  /**
   * An event triggered when a Connection is closing for any reason.  This
   * event is always called once and only once if a socket was connected.
   * At the time this event is called, the sockets are disconnected, so you
   * can't get stats or addresses from the connection.
   *
   * The PacketStream is still valid, and there still may be data in the
   * queue for you to read, even if you pick up all data in the onReceive
   * events, there can be incoming data between the last onReceive and
   * onDisconnect which will be very possible espically in planned
   * disconnects.
   *
   * onDisconnect will always be the last event called on this listener, so
   * you can destroy this object after onDisconnect is called.  Before
   * onDisconnect was called either onFailure or onExit were called if you
   * did not terminate the connection yourself using Connection::disconnect.
   *
   * onDisconnect is only called if onNewConn or onConnect totally completed.
   * If the connection process fails before onNewConn or onConnect, the
   * connect or listen fail events are generated.  If an error occurs DURING
   * onNewConn or onConnect, if any cleanup needs to be done, it needs to be
   * done in the exception handler, and no other failure event will be
   * generated.  After onNewConn or onConnect completes, then and only then
   * might a onDisconnect event be generated.
   */
  virtual void onDisconnect( Connection& conn );

  /**
   * This event is triggered when the remote end has gracefully closed the
   * connection.  The connection will soon be disconnected and the next
   * event you will receive is onDisconnect.  The connection should be
   * considered in an disconnected state.
   *
   * This event must be "non-blocking" -- like most GNE events -- as there
   * is only a single event thread per connection.  Therefore, no other
   * events will be called until this function completes for this connection.
   */
  virtual void onExit( Connection& conn );

  /**
   * An onTimeout event occurs ms milliseconds after the last onReceive event,
   * or the last onTimeout event.  The value of ms is set by calling
   * Connection::setTimeout.  This allows you a way to detect idle
   * connections that have become unresponsive due to loss of network
   * connection, or to send out keep alive packets.
   *
   * If your programs are constantly sending data back and forth, then on the
   * first onTimeout you might want to declare the connection lost and call
   * disconnect and clean up.  If there is normally idle time during your
   * connection you may want to send out a PingPacket and see if you get a
   * response before the onTimeout event occurs again before declaring the
   * connection lost.
   *
   * Remember that GNE does not automatically send PingPackets to give you the
   * choice for measuring ping times, so if you want to use PingPackets to
   * keep onTimeout events from occuring, you have to send them yourself and
   * respond to them yourself as well.
   */
  virtual void onTimeout( Connection& conn );

  /**
   * This event is triggered when a non-fatal error occurs in a connection
   * that does not force the connection to close, for example an unknown
   * packet encounted is an error, but the connection can still proceed.
   *
   * After this event is processed, connections resume normally.
   * Connection::disconnect() may also be called at this point if you wish
   * to terminate the connection anyways.
   *
   * This event must be "non-blocking" -- like most GNE events -- as there
   * is only a single event thread per connection.  Therefore, no other
   * events will be called until this function completes for this connection.
   *
   * @see onFailure()
   * @see disconnect()
   */
  virtual void onError( Connection& conn, const Error& error);

  /**
   * This event is triggered when a fatal error occurs in a connection.
   * When a fatal error occurs, communications cannot contiune and the
   * socket will be disconnected.  An onDisconnect() event will occur
   * immediately after this event completes.  Most errors in GNE are fatal.
   *
   * This event must be "non-blocking" -- like most GNE events -- as there
   * is only a single event thread per connection.  Therefore, no other
   * events will be called until this function completes for this connection.
   *
   * @see onError()
   */
  virtual void onFailure( Connection& conn, const Error& error);

  /**
   * Event triggered when one or more packets have been recieved.  It is
   * possible to get more packets while in this function, but if packets
   * arrive during this event, it will be called again, so you don't have to
   * make sure they have all been processed (there is no sure way to do
   * that.).
   *
   * This event must be "non-blocking" -- like most GNE events -- as there
   * is only a single event thread per connection.  Therefore, no other
   * events will be called until this function completes for this connection.
   */
  virtual void onReceive( Connection& conn );
  virtual void clear(){}
};

} // namespace GNE

#endif /* CONNECTIONLISTENER_H_INCLUDED_C43554C0 */
