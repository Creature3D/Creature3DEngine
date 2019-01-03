#ifndef _OBJECTBROKERSERVER_H_ASDF423
#define _OBJECTBROKERSERVER_H_ASDF423

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

#include "ObjectBroker.h"
#include "ObjectBrokerPacket.h"
#include "ObjectCreationPacket.h"
#include "ObjectUpdatePacket.h"
#include "ObjectDeathPacket.h"
#include "Mutex.h"

namespace GNE {
  class NetworkObject;

/**
 * @ingroup highlevel
 *
 * The ObjectBrokerServer assigns IDs to NetworkObject objects, and aids in
 * the creation of ObjectBrokerPacket packets.  When a new object is
 * registered with the broker, a new, unique, ID is assigned to that
 * NetworkObject and that ID is marked as a taken ID until it is unregistered.
 *
 * The packets that the ObjectBrokerServer returns about a particular object
 * must be sent to the clients in the same order that ObjectBrokerServer
 * gives them to you based on the API calls.  This is fairly self-evident, but
 * this means that the ObjectCreationPacket must be sent before
 * ObjectUpdatePacket packets and those before ObjectDeathPacket packets.  In
 * order to guarantee this, the creation and death packets must be sent
 * reliably.  Update packets may be sent unreliably, but you must be prepared
 * for the scenario that the update packet arrives before the creation packet,
 * and will probably need to be ignored.  This is possible because the
 * reliable and unreliable packet streams in GNE may be unrelated, and have no
 * guaranteed order in relation to each other.
 *
 * There are 65535 available object IDs, so you cannot have more objects than
 * that.
 *
 * Almost all methods return a smart pointer to a packet, which means you do
 * not need worry about memory allocation.
 */
  class ObjectBrokerServer : public ObjectBroker {
public:
  /**
   * Creates a new ObjectBrokerServer Object with no taken IDs.
   */
  ObjectBrokerServer();

  /**
   * Dtor.
   */
  ~ObjectBrokerServer();

  /**
   * Method for registering a new object, or getting a creation packet from an
   * already registered object.  If the given object has an ID, a new
   * ObjectCreationPacket is generated.  A new ID is assigned automatically if
   * the given object does not have an ID.
   *
   * If no IDs remain because there are too many objects, then a NULL pointer
   * is returned.
   */
  ObjectCreationPacket::sptr getCreationPacket( NetworkObject& obj );

  /**
   * Returns an ObjectUpdatePacket for the given object.  The passed
   * parameter, param, is passed directly into the NetworkObject's
   * createUpdatePacket method.
   *
   * @see NetworkObject::createUpdatePacket
   */
  ObjectUpdatePacket::sptr getUpdatePacket(
      NetworkObject& obj, const void* param );

  /**
   * Same as ObjectBrokerServer::getUpdatePacket( NetworkObject&, const void* ),
   * but passes in NULL as the const void* parameter.
   * 
   * @param obj a valid NetworkObject (one that has been assigned a valid ID
   * through getCreationPacket).
   */
  ObjectUpdatePacket::sptr getUpdatePacket( NetworkObject& obj );

  /**
   * Creates an ObjectDeathPacket for the given object, but does NOT
   * unregister the object, in case additional death packets need to be
   * created (to send to other, multiple clients).
   * 
   * @param obj a valid NetworkObject (one that has been assigned a valid ID
   * through getCreationPacket).
   */
  ObjectDeathPacket::sptr getDeathPacket( NetworkObject& obj );

private:
  /**
   * Returns the next valid ID, and marks it as taken.  The "sync" mutex must
   * be locked when this method is called.  Returns false if there are no IDs
   * remaining.
   */
  bool assignNextId( NetworkObject& o );
  
private:
  ObjectBrokerServer( const ObjectBrokerServer& );
  ObjectBrokerServer& operator=( const ObjectBrokerServer& rhs );

  int nextId;
};

} //namespace GNE

#endif

