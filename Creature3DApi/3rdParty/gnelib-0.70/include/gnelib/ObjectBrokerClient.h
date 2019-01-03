#ifndef _OBJECTBROKERCLIENT_H_DFWER2
#define _OBJECTBROKERCLIENT_H_DFWER2

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

namespace GNE {
  //bool initGNE(NLenum networkType, int (*atexit_ptr)(void (*func)(void)), int);

/**
 * @ingroup highlevel
 *
 * The ObjectBrokerClient takes packets generated from the ObjectBrokerServer
 * and manages the object creations, updates, and deletes specified.  All
 * methods in ObjectBrokerClient are thread safe.
 */
class ObjectBrokerClient : public ObjectBroker {
public:
  /**
   * Default ctor.
   */
  ObjectBrokerClient();

  /**
   * Default dtor.
   */
  ~ObjectBrokerClient();

  /**
   * Registers a new object type with the ObjectBrokerClient by relating a
   * packet ID with a function that creates a NetworkObject.  This is similar
   * to PacketParser::registerPacket.
   *
   * While it is intended that a single creation function associated with a
   * packet ID returns always the same type of object, this is by no means a
   * requirement of the creation function -- it may return different
   * subclasses of NetworkObjects different times it is called.  This can be
   * useful if the same packet type might describe a few very similar types of
   * objects, or if there are more types of objects than there exist packet
   * IDs.
   * 
   * @param id the Packet ID of a packet which has all of the needed
   * information to create some NetworkObject.
   * @param createFunc the function which returns a new NetworkObject instance
   * given a packet of the type specified by the id parameter, and an
   * objectId.  The create function must assign the given objectId to the
   * object when it creates it.
   */
  static void registerObject( guint8 id, ObjCreationFunc createFunc );

  /**
   * Uses the given Packet, which must be an ObjectBrokerPacket subclass
   * (ObjectCreationPacket, ObjectUpdatePacket, ObjectDeathPacket), returns
   * the associated object from the packet.
   *
   * If the passed packet is an ObjectCreationPacket, a new object is created
   * with the registered create function and returned.  If an object with the
   * ID already exists, then an Error is thrown with code
   * Error::DuplicateObjectID.  If the packet type used to create the object
   * has not been registered, then an Error with the ID
   * Error::InvalidCreationPacketType is thrown.
   *
   * If the passed packet is an ObjectUpdatePacket, the specified object is
   * given the packet to update itself with, and that object is returned.  If
   * no objects with the ID in the packet exist, an Error is thrown with code
   * Error::UnknownObjectId.  If ignoreUpdateError is set to true, then NULL
   * instead is returned, and no action is taken on the packet.  This
   * particular flag is useful, because if updates are sent unreliably, then
   * it is possible for an update for an object to arrive BEFORE its creation
   * packet.
   *
   * If the passed packet is an ObjectDeathPacket, the specified object's
   * incomingDeathPacket and onDeregistration events are called, and that
   * object is returned.  If no objects with the ID in the packet exist, an
   * Error is thrown with code Error::UnknownObjectId.
   *
   * If the passed packet is not any one of these types, then an Error with
   * code Error::InvalidObjectPacket is thrown.
   *
   * In all cases, either a valid NetworkObject is returned, or an Error is
   * thrown.  The single exception is that NULL can be returned if
   * ignoreUpdateError is passed as true, and ObjectUpdatePacket processing
   * failed because of an unknown packet ID, which should be the only typical
   * error case.
   */
  NetworkObject* usePacket( const Packet& packet,
                            bool ignoreUpdateError); /* throw Error */

  /**
   * Calls usePacket( const Packet&, bool ) with the second parameter as being
   * false, thus this method always returns a valid NetworkObject reference,
   * or throws an exception.
   */
  NetworkObject& usePacket( const Packet& packet ); /* throw Error */

	/**
	* Initializes the static packet ID lookup, called only by initGNE.
	*/
	static void staticInit();
private:


 // friend bool GNE::initGNE(NLenum, int (*)(void (*)(void)), int);

private:
};

} //namespace GNE

#endif

