#ifndef _PACKETPARSER_H_
#define _PACKETPARSER_H_

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
//////////////////////////////////////////////////////////////////////////
//
//Wucaihua
//引入PacketFuncsMap，扩展Packet ID为int型
//
//////////////////////////////////////////////////////////////////////////
#include "export.h"
#include "gnetypes.h"
#include <limits.h>

namespace GNE {
class Packet;
class Buffer;

/**
 * @ingroup midlevel
 *
 * A namespace containing functions handling the parsing of packets.  Also
 * contains important constants to be aware of for assigning your packets
 * IDs.
 */
namespace PacketParser {

/**
 * The first number suitable for the user to give IDs to their packets.  Any
 * numbers between MIN_USER_ID and MAX_USER_ID inclusive belong to
 * the user.  It is suggested that the user assign packet numbers by adding
 * to MIN_USER_ID.  (i.e. MIN_USER_ID + 5)
 */
const int MIN_USER_ID = 5;

/**
 * @see MIN_USER_ID
 */
const int MAX_USER_ID = INT_MAX;

/**
 * The network packet ends with this byte, meaning that no more GNE packets
 * exist in this network packet (end-of-data).
 */
//const guint8 END_OF_PACKET = 255;
const int END_OF_PACKET = INT_MAX;

/**
 * A function pointer to a function that creates a new subclass of a Packet.
 * The return type is of the type registered with this function.
 */
typedef Packet* (*PacketCreateFunc)();

/**
 * A function pointer to a function to clone a Packet.  NULL must not be passed
 * into this function.  The passed Packet must be of the type registered with
 * this function.
 */
typedef Packet* (*PacketCloneFunc)( const Packet* );

/**
 * A function pointer to a function to destroy a Packet.  The passed Packet
 * must be of the type registered with this function.
 */
typedef void (*PacketDestroyFunc)( Packet* );

/**
 * Registers a new type of packet, so GNE can recognize it.  In order for GNE
 * to recognize, create, and parse your packets derived from the Packet
 * class, you should register it here, preferably right after initalizing
 * GNE.
 *
 * You can only register packets from MIN_USER_ID to MAX_USER_ID,
 * inclusive.  You may not register a packet multiple times.
 *
 * Unless you want to create your own memory allocation system, you should
 * consider using the defaultRegisterPacket function.
 */
void GNE_EXPORT registerPacket( int id,
                     PacketCreateFunc createFunc,
                     PacketCloneFunc cloneFunc,
                     PacketDestroyFunc destroyFunc );

/**
 * Default packet creation function using the "new" operator and the default
 * constructor.
 */
template <class T>
Packet* defaultPacketCreateFunc() {
  return new T();
}

/**
 * Default packet clone function using the "new" operator and the copy
 * constructor.
 */
template <class T>
Packet* defaultPacketCloneFunc( const Packet* p ) {
  const T* other = static_cast<const T*>( p );
  return new T( *other );
}

/**
 * Default packet destroy function using the "delete" operator.
 */
template <class T>
void defaultPacketDestroyFunc( Packet* p ) {
  delete p;
}

/**
 * Default registration function that works for typical packets without any
 * custom memory allocation.  This function is a template, and is equivalent
 * to:
 * <pre>
 * registerPacket( T::id,
 *                 defaultPacketCreateFunc<T>,
 *                 defaultPacketCloneFunc<T>,
 *                 defaultPacketDestroyFunc<T> );
 * </pre>
 */
template <class T>
void defaultRegisterPacket() {
  registerPacket(
    T::ID,
    defaultPacketCreateFunc<T>,
    defaultPacketCloneFunc<T>,
    defaultPacketDestroyFunc<T> );
}

/**
 * Calls the packet creation function registered for the given ID.
 * The ID passed MUST be registered.
 */
Packet* createPacket( int id );

/**
 * Calls the packet clone function registered for the passed packet type.
 * This is based on the Packet::getType() method.
 */
Packet* clonePacket( const Packet* p );

/**
 * Calls the packet deletion function registered for the passed packet type.
 * This is based on the Packet::getType() method.
 */
void destroyPacket( Packet* p );

/**
 * Parses the next packet from the given Buffer.  Returns NULL if no more
 * packets remain in the Buffer.
 * 
 * @throws Error if the next packet's type is unknown, or an error occurs
 *   during the readPacket method.
 *
 * @return a newly allocated Packet of the type of the next packet in the
 *         Buffer, else NULL on end of Buffer.
 */
Packet* parseNextPacket( Buffer& raw );

} //namespace PacketParser
} //namespace GNE

#endif
