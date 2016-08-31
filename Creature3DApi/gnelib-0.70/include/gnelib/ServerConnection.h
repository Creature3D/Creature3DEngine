#ifndef SERVERCONNECTION_H_INCLUDED_C4FE6FF3
#define SERVERCONNECTION_H_INCLUDED_C4FE6FF3

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
#include "SmartPtr.h"
#include "WeakPtr.h"

namespace GNE {
class ConnectionListener;
class ServerConnectionListener;
class ServerConnectionParams;
class Error;
class ConnectionParams;

/**
 * @ingroup internal
 *
 * A GNE "internal" class.  Users will use this class, but probably only as
 * its base class -- a Connection.  This class is created by
 * ServerConnectionListener when incoming connections are comming in.
 */
class GNE_EXPORT ServerConnection : public Connection, private Thread {
protected:
  /**
   * We need information gained from setThisPointer to initialize, so the real
   * work is done in the init method, which should be called right after
   * constructon and setThisPointer has been called.
   */
  ServerConnection();

  void init(const ConnectionParams& p, NLsocket rsocket,
            const SmartPtr<ServerConnectionListener>& creator );

public:
  typedef SmartPtr<ServerConnection> sptr;
  typedef WeakPtr<ServerConnection> wptr;

  /**
   * Intializes this class.  Note that you won't create a ServerConnection
   * directly.  The ServerConnectionListener does that for you.
   *
   * @param rsocket2 the reliable socket received from the accept command.
   * @param creator the ServerConnectionListener that created us, so that we
   *                may call its onListenFailure event.  This strong pointer
   *                will be released after the connection finished, as to not
   *                worry about cycles.
   * @see ServerConnectionListener
   */
  static sptr create(const ConnectionParams& p, NLsocket rsocket,
                     const SmartPtr<ServerConnectionListener>& creator);

  /**
   * Destructs this ServerConnection object.  The user need not worry about
   * the fact that ServerConnection is a thread (in the sense that the user
   * never need to call detach or join), to do a proper cleanup.
   */
  virtual ~ServerConnection();

  /**
   * Starts the connection process.
   */
  using Thread::start;
protected:
  /**
   * This thread performs the connection process.  If an error occurs:
   *
   * Before onNewConn: ServerConnectionListener::onListenFailure() is called.
   *
   * During onNewConn: Only onNewConn is called and is reponsible for catching
   *                   the exception and cleaning up anything it has done.
   *                   onDisconnect will NOT be called, but onListenFailure
   *                   will be.
   *
   * After onNewConn:  onFailure then onDisconnect.
   *
   * After onNewConn is called successfully, then
   * ServerConnectionListener::onListenSuccess is called.
   */
  void run();

  void shutDown();

private:
  /**
   * @throw Error if an error occurs.
   */
  void doHandshake();

  /**
   * @throw Error if an error occurs.
   */
  void getCRP();

  /**
   * @throw Error if an error occurs.
   */
  void sendRefusal();

  /**
   * @throw Error if an error occurs.
   */
  void sendCAP();

  /**
   * @throw Error if an error occurs.
   */
  void getUnreliableInfo();

  /**
   * calls onConnectFailure, checking shutdown
   */
  void doFailure( const SmartPtr< ServerConnectionListener >& l,
                                  const Error& e,
                                  const Address& addr,
                                  const SmartPtr< ConnectionListener >& listener );

  //Temporary storage to hold variables before and during connecting.
  typedef SmartPtr< ServerConnectionParams > ParamsSPtr;
  ParamsSPtr params;

  //sigh.  Perhaps requiring inheritance from Thread was a bad idea.
  friend class boost::weak_ptr< Thread >;
  friend class boost::shared_ptr< Thread >;
  friend class boost::weak_ptr< Connection >;
  friend class boost::shared_ptr< Connection >;
};

}
#endif /* SERVERCONNECTION_H_INCLUDED_C4FE6FF3 */
