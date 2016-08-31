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
#include "../include/gnelib/WrapperPacket.h"
#include "../include/gnelib/PacketParser.h"
#include "../include/gnelib/Buffer.h"

namespace GNE {
  
WrapperPacket::WrapperPacket() : packet( NULL ) {
}

WrapperPacket::WrapperPacket( int id ) : Packet( id ), packet( NULL ) {
}

WrapperPacket::WrapperPacket( int id, const Packet* packet )
: Packet( id ), packet( NULL ) {
  setData( packet );
}

WrapperPacket::WrapperPacket( const WrapperPacket& o ) : Packet( o ) {
  if ( o.getData() != NULL )
    packet = o.getData()->makeClone();
  else
    packet = NULL;
}

WrapperPacket::~WrapperPacket() {
}

int WrapperPacket::getSize() const {
  int temp = 1;
  if ( packet != NULL )
    temp = packet->getSize();
  return Packet::getSize() + temp;
}

const Packet* WrapperPacket::getData() const {
  return packet;
}

void WrapperPacket::setData( const Packet* packet ) {
  delete this->packet;
  if ( packet != NULL )
    this->packet = packet->makeClone();
  else
    this->packet = NULL;
}

void WrapperPacket::writePacket(Buffer& raw) const {
  Packet::writePacket( raw );
  if ( packet != NULL )
    packet->writePacket( raw );
  else
    raw << PacketParser::END_OF_PACKET;
}

void WrapperPacket::readPacket(Buffer& raw) {
  Packet::readPacket( raw );
  packet = PacketParser::parseNextPacket( raw );
}

WrapperPacket& WrapperPacket::operator= ( const WrapperPacket& rhs ) {
  Packet::operator =( rhs );
  setData( rhs.getData() );
  return *this;
}

} //namespace GNE
