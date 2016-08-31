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
#include "../include/gnelib/ObjectBrokerPacket.h"
#include "../include/gnelib/ObjectCreationPacket.h"
#include "../include/gnelib/ObjectUpdatePacket.h"
#include "../include/gnelib/ObjectDeathPacket.h"
#include "../include/gnelib/ObjectBrokerClient.h"
#include "../include/gnelib/NetworkObject.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Lock.h"

namespace GNE {

static ObjCreationFunc funcs[256];
static Mutex staticSync;

void ObjectBrokerClient::staticInit() {
  for ( int i=0; i<256; ++i)
    funcs[i] = NULL;
}

ObjectBrokerClient::ObjectBrokerClient() {
}

ObjectBrokerClient::~ObjectBrokerClient() {
}

void ObjectBrokerClient::registerObject( guint8 id, ObjCreationFunc createFunc ) {
  LockMutex lock(staticSync);
  assert(funcs[id] == NULL);
  funcs[id] = createFunc;
}

/**
 * \todo optimize container usage (exists, find, erase, etc).  Applies to
 * other ObjectBroker (base, client, and server) methods.
 */
NetworkObject* ObjectBrokerClient::usePacket( const Packet& packet,
                                              bool ignoreUpdateError) {
  int type = packet.getType();
  int objectId = -1;
  NetworkObject* ret = NULL;

  if ( type == ObjectCreationPacket::ID ) {
    const ObjectCreationPacket& ocp = static_cast<const ObjectCreationPacket&>(packet);
    objectId = ocp.getObjectId();
    assert( ocp.getData() != NULL );

    staticSync.acquire();
    ObjCreationFunc func = funcs[ocp.getData()->getType()];
    staticSync.release();
    if ( func == NULL )
      throw Error( Error::InvalidCreationPacketType );

    LockMutex lock(sync);
    if ( exists( objectId ) )
      throw Error( Error::DuplicateObjectId );
    ret = func( objectId, *ocp.getData() );
    assert ( ret != NULL );
    assert ( ret->getObjectId() == objectId );

    objects[objectId] = ret;

  } else if ( type == ObjectUpdatePacket::ID ) {
    const ObjectUpdatePacket& oup = static_cast<const ObjectUpdatePacket&>(packet);
    objectId = oup.getObjectId();
    assert( oup.getData() != NULL );

    LockMutex lock(sync);
    ret = getObjectById( objectId );
    if ( ret == NULL )
      throw Error( Error::UnknownObjectId );
    ret->incomingUpdatePacket( *oup.getData() );

  } else if ( type == ObjectDeathPacket::ID ) {
    const ObjectDeathPacket& odp = static_cast<const ObjectDeathPacket&>(packet);
    objectId = odp.getObjectId();

    LockMutex lock(sync);
    ret = getObjectById( objectId );
    if ( ret == NULL )
      throw Error( Error::UnknownObjectId );
    ret->incomingDeathPacket( odp.getData() );

    deregisterObject( *ret );

  } else
    throw Error( Error::InvalidObjectPacket );

  assert ( ret != NULL || ignoreUpdateError );
  return ret;
}

NetworkObject& ObjectBrokerClient::usePacket( const Packet& packet ) {
  NetworkObject* ret = usePacket( packet, false );
  assert ( ret != NULL );
  return *ret;
}

} //namespace GNE
