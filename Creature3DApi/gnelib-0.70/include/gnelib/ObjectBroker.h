#ifndef _OBJECTBROKER_H_234jf4
#define _OBJECTBROKER_H_234jf4

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

#include <map>
#include "Mutex.h"

namespace GNE {
  class Packet;
  class NetworkObject;

  /**
   * @ingroup highlevel
   *
   * The function type used to create an object from a given packet.
   */
  typedef NetworkObject* (*ObjCreationFunc)( int objectId, const Packet& );
  
/**
 * @ingroup highlevel
 *
 * Serves as a base class for ObjectBrokerServer and ObjectBrokerClient and
 * contains common functionality.  This class is abstract, so you cannot
 * instantiate it.
 */
class ObjectBroker {
public:
  /**
   * Returns the number of objects currently being managed by this
   * ObjectBroker.
   */
  int numObjects() const;

  /**
   * Returns the NetworkObject currently registered with the given object
   * ID.  Returns NULL if there is no object with that ID.
   */
  NetworkObject* getObjectById( int objId );

  /**
   * Deregisters the given object entirely from the ObjectBroker.  The
   * ID of the object is set to an invalid ID, and the onDeregistration event
   * is called.  Once deregistered, an object must be reregistered before it
   * can be used with the ObjectBroker again.
   *
   * Note that in ObjectBrokerClient, processing an ObjectDeathPacket
   * automatically deregisteres an object, so this method is most useful with
   * ObjectBrokerServer.
   * 
   * @param obj a valid NetworkObject (one that has been assigned a valid ID).
   * 
   * @see NetworkObject::onDeregistration
   */
  void deregisterObject( NetworkObject& obj );

protected:
  ObjectBroker() {}

  /**
   * Returns true if the object ID given exists.  The mutex sync must be locked
   * when this method is called.
   */
  bool exists( int objId );

  /**
   * Assigns the given ID to the object.
   */
  void assignId( NetworkObject& o, int newId );

  mutable Mutex sync;

  typedef std::map<int, NetworkObject*> ObjectsMap;

  typedef std::map<int, NetworkObject*>::iterator ObjectsIter;
  
  /**
   * Associates an integer object ID to a NetworkObject.
   */
  ObjectsMap objects;
};

} //namespace GNE

#endif
