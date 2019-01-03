#ifndef CLIENTCONNECTION_H_INCLUDED_C51A7F46
#define CLIENTCONNECTION_H_INCLUDED_C51A7F46

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
#include "Connection.h"
#include "Thread.h"
#include "Address.h"
#include "SmartPtr.h"
#include "WeakPtr.h"

namespace GNE {
class ConnectionListener;
class Error;
class ClientConnectionParams;
class ConnectionParams;
class SyncConnection;

/**
 * @ingroup midlevel
 *
 * A class that can connect to a remote target.
 */
class GNE_EXPORT ClientConnection : public Connection, private Thread {
protected:
  /**
   * Initializes this ClientConnection.
   */
  ClientConnection();

public:
  typedef SmartPtr<ClientConnection> sptr;
  typedef WeakPtr<ClientConnection> wptr;

  /**
   * Creates a new ClientConnection object, managed by a SmartPtr.  The listener
   * is passed in the open method.
   */
  static sptr create();

  virtual ~ClientConnection();

  /**
   * Opens the socket, ready for connect, but does not yet connect.  All of
   * the relvant parameters for establishing the connection are passed into
   * this function.  If there is an error, the function returns true.
   *
   * @pre Connection's state is NeedsInitialization
   * @post Connection's state is ReadyToConnect
   *
   * @param dest the destination address.
   */
  bool open(const Address& dest, const ConnectionParams& p);

  /**
   * Starts connection to the specified target.  This method does not block,
   * and a thread will be started to handle the connection process.
   * onConnect() or onConnectFailure() will be called depending on the
   * outcome of this process.  The exception is that if an error occurs
   * during onConnect or it chooses to reject the connection,
   * onConnectFailure will also be called.
   *
   * You can call waitForConnect after connect to wait until the connection is
   * finished, when onConnect or onConnectFailure will be called.  When the
   * called function exits, the thread will stop and waitForConnect will return.
   *
   * The version number for the library and your own version number will be
   * checked during this phase.  If either protocol versions mismatch,
   * onConnectFailure() will be triggered.
   *
   * @pre Connection's state is ReadyToConnect
   * @pre Connection's state is Connecting (but may change immedately after to
   *   another state).
   * @pre the open method succeeded.
   *
   * @see ConnectionListener#onConnect()
   * @see ConnectionListener#onConnectFailure()
   */
  void connect();

  /**
   * Waits indefinitely for the connection attempt started by connect to
   * finish.  When the attempt finishes, the result of the connection attempt
   * is returned.  If the connection was successful, an error code of
   * Error::NoError is returned.
   *
   * This is the same Error that is passed to onConnectFailure.
   *
   * Multiple threads may call this method at any time, even after connection
   * completes (and the return will always be the same).  If the connection
   * has already completed this method will return instantly.
   *
   * @pre connect has been called on this object successfully.
   */
  Error waitForConnect(unsigned int ms = INFINITE);
  
private:
  friend class SyncConnection;
  //This method only to be called by SyncConnection
  //raw pointer we know is OK since SyncConnection joins on this, the raw ptr
  //is guaranteed to be valid through the whole connect process.
  void connect( const SmartPtr<SyncConnection>& );

private: //thread functions
  /**
   * Connection starts a new thread lasting only while it is connecting.
   */
  virtual void run();

  /**
   * This will disconnect the connection in progress, to cause the connection
   * process to fail.  This method is called when %GNE shuts down.
   */
  virtual void shutDown();

private:
  /**
   * Does the GNE protocol handshake, throwing an Error on error.
   */
  void doHandshake();

  /**
   * Sends the CRP, throwing an Error on error.
   */
  void sendCRP();

  /**
   * Tries to get the CAP (or refusal packet), throwing an Error on error or
   * if the connection was refused.
   * @return an address to connect to the remote unreliable connection.  If
   *         no unreliable connection was requested, the value of the
   *         returned address is undefined.
   */
  Address getCAP();

  /**
   * Sets up the unreliable connection, throwing an Error on error.
   */
  void setupUnreliable(const Address& dest);

  /**
   * calls onConnectFailure then sets connError.
   */
  void doFailure( const SmartPtr< ConnectionListener >& l, const Error& e );

  /**
   * What error occured while connecting?
   */
  Error connError;

  //Temp storage of connection params.
  typedef SmartPtr< ClientConnectionParams > ParamsSPtr;
  ParamsSPtr params;

  //sigh.  Perhaps requiring inheritance from Thread was a bad idea.
  friend class boost::weak_ptr< Thread >;
  friend class boost::shared_ptr< Thread >;
  friend class boost::weak_ptr< Connection >;
  friend class boost::shared_ptr< Connection >;
};

}
#endif /* CLIENTCONNECTION_H_INCLUDED_C51A7F46 */
