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
#include "../include/gnelib/ObjectBroker.h"
#include "../include/gnelib/NetworkObject.h"
#include "../include/gnelib/Lock.h"

namespace GNE {

int ObjectBroker::numObjects() const {
  LockMutex lock(sync);
  return objects.size();
}

NetworkObject* ObjectBroker::getObjectById( int objId ) {
  LockMutex lock(sync);
  ObjectsIter iter = objects.find( objId );
  if ( iter == objects.end() )
    return NULL;
  else
    return (*iter).second;
}

void ObjectBroker::deregisterObject( NetworkObject& obj ) {
  assert( obj.hasValidId() );

  sync.acquire();
  int oldId = obj.getObjectId();
  assert( exists( oldId ) );
  objects.erase( oldId );
  assignId( obj, -1 );
  sync.release();

  obj.onDeregistration( oldId );
}

bool ObjectBroker::exists( int objId ) {
  return (objects.find( objId ) != objects.end());
}

void ObjectBroker::assignId( NetworkObject& o, int newId ) {
  o.setObjectId( newId );
}

} //namespace GNE
