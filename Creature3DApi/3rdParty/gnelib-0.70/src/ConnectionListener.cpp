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
#include "../include/gnelib/Error.h"
#include "../include/gnelib/ConnectionListener.h"

namespace GNE {

ConnectionListener::~ConnectionListener() {
}

ConnectionListener::sptr ConnectionListener::getNullListener() {
  static ConnectionListener nullListener;
  return sptr( &nullListener, NullDeleter() );
}

void ConnectionListener::onConnect(SyncConnection& conn) {
}

void ConnectionListener::onConnectFailure( Connection& conn, const Error& error ) {
}

void ConnectionListener::onNewConn(SyncConnection& newConn) {
}

void ConnectionListener::onDisconnect( Connection& conn ) {
}

void ConnectionListener::onExit( Connection& conn ) {
}

void ConnectionListener::onTimeout( Connection& conn ) {
}

void ConnectionListener::onError( Connection& conn, const Error& error ) {
}

void ConnectionListener::onFailure( Connection& conn, const Error& error ) {
}

void ConnectionListener::onReceive( Connection& conn ) {
}

} //namespace GNE




