#ifndef _NETWORKOBJECT_H_234512
#define _NETWORKOBJECT_H_234512

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

namespace GNE {
  class Packet;
  class ObjectBroker;
  
/**
 * @ingroup highlevel
 *
 * Represents an object which can be replicated through the ObjectBroker.  All
 * of these methods should be called through the ObjectBroker, but it is
 * acceptable to call createCreationPacket, createUpdatePacket,
 * incomingUpdatePacket, and createDeathPacket once ObjectBroker has called
 * createCreationPacket for the first time (and therefore assigned this object
 * an ID).
 */
class NetworkObject {
public:
  /**
   * Initializes this NetworkObject to have an invalid (unassigned) ID.
   */
  NetworkObject();

  virtual ~NetworkObject();

  /**
   * Initializes this NetworkObject to have the given object ID.
   */
  explicit NetworkObject( int objectId );

  /**
   * Copies from the given NetworkObject, but does NOT copy the object ID!
   * Instead, an invalid object ID is assigned to this object.  This must be
   * done because two objects cannot share an object ID.
   */
  NetworkObject( const NetworkObject& o );

  /**
   * Care must be taken with this operator, because an object must be
   * unregistered with the ObjectBroker.  Therefore, this object retains its
   * old object ID.
   */
  NetworkObject& operator= ( const NetworkObject& rhs );

  /**
   * Returns the objectID for this object.  IDs are always positive.  A
   * negative value is an invalid ID, and means that this object has not yet
   * been given an ID by the ObjectBroker.
   */
  int getObjectId() const;

  /**
   * Returns true if this NetworkObject has a valid ID, which implies that is
   * currently under the management of an ObjectBroker.
   */
  bool hasValidId() const;
  
public: //Events
  /**
   * This method will be called when the remote side has not seen this object
   * before, and a Packet needs to be created that will completely express
   * this object so that it can be created on the remote side.  The type of
   * packet used to create an object must always be the same, because the
   * ObjectBroker looks up the object creation method based on the packet type
   * ID.
   *
   * The object ID for this object will be set, and available for viewing with
   * the getObjectID method when this method is called.  It must be called
   * through the ObjectBroker.
   *
   * The object returned should be a new Packet, for which GNE will become
   * responsible for deallocating the object.
   */
  virtual Packet* createCreationPacket() = 0;

  /**
   * The remote side has already seen this object before, and has created it
   * already, but a Packet is desired to send to the remote object so that it
   * may update its copy with new information.  Unlike the
   * createCreationPacket method, the Packet returned by this method may be of
   * any type, and the type of Packet returned need not be consistant, since
   * object updates are routed by object ID rather than by packet ID.
   *
   * The parameter given to this function exists to give the update function a
   * context if desired.  For example an update packet for only a certain part
   * of the data may be desired.
   *
   * The object returned should be a new Packet, for which GNE will become
   * responsible for deallocating the object.
   */
  virtual Packet* createUpdatePacket( const void* param ) = 0;

  /**
   * When this object is released from the ObjectBroker system, it may want to
   * send a death packet if the remote end needs to know when an object dies.
   * Sometimes creating a death packet is optional if the server and client
   * both agree that the object will die at a certain time (for example if an
   * object has a finite lifetime).  Even if a death packet is needed, it may
   * not need any associated data, so this method may return NULL if no
   * additional information is needed with the DeathPacket.
   *
   * This method may be called zero or more times before getting deregistered
   * from the ObjectBroker system, depending on the need for
   * ObjectDeathPackets for this NetworkObject.
   * 
   * The object returned should be a new Packet, or a NULL pointer, for which
   * GNE will become responsible for deallocating the object.
   */
  virtual Packet* createDeathPacket() = 0;

  /**
   * An optional event called when the object has been deregistered, and has
   * lost its ID.  This event is triggered on both the "server" and the
   * "client" sides.  This will be the last event called on this object from
   * the ObjectBroker.
   *
   * On the "server" side, this event is called after any possible
   * createDeathPacket calls.  On the "client" side, this event always
   * immedately follows incomingDeathPacket.
   *
   * The objectID for this object has been set to an invalid ID, indicating it
   * has been released from the ObjectBroker.  The ID that this object used to
   * have is passed in the parameter oldId.
   *
   * Once this method is called by the ObjectBroker, it will never access this
   * object again, and any references to this object are dropped, so after
   * this method, you may feel free to delete this object at any time.
   *
   * The default behavior for this event is to do nothing.
   *
   * @param oldId the ID this object used to have.
   */
  virtual void onDeregistration( int oldId );

  /**
   * This method is called by the ObjectBroker when it receives an
   * ObjectUpdatePacket for this object.
   */
  virtual void incomingUpdatePacket( const Packet& packet ) = 0;

  /**
   * This method is called by the ObjectBroker when it receives an
   * ObjectDeathPacket, and it is releasing the object from its control.  An
   * ObjectDeathPacket may or may not have a Packet of associated data with
   * it, so the passed pointer may be NULL.
   *
   * After the incomingDeathPacket event, an onDeregistration event is
   * immediately generated.
   */
  virtual void incomingDeathPacket( const Packet* packet ) = 0;

private:
  //ObjectBroker is a friend so it can set our ObjectID through the
  //provided method.
  friend class ObjectBroker;

  void setObjectId( int newId );

private:
  int objectId;
};

} //namespace GNE

#endif
