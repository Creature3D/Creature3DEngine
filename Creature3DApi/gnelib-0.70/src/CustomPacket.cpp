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
#include "../include/gnelib/CustomPacket.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Packet.h"

namespace GNE {

const int CustomPacket::ID = 1;

CustomPacket::CustomPacket() : Packet(ID), buf( getMaxUserDataSize() ) {
}

CustomPacket::CustomPacket( const CustomPacket& o ) : Packet(ID), buf( o.buf ) {
}

CustomPacket::~CustomPacket() {
}

int CustomPacket::getMaxUserDataSize() {
  Packet packet;
  return Buffer::RAW_PACKET_LEN - packet.getSize() - Buffer::getSizeOf( guint16(0) );
}

Buffer& CustomPacket::getBuffer() {
  return buf;
}

void CustomPacket::clear() {
  buf.clear();
}

int CustomPacket::getSize() const {
  return Packet::getSize() + Buffer::getSizeOf( guint16(0) ) + buf.getPosition();
}

void CustomPacket::writePacket( Buffer& raw ) const {
  buf.flip();
  int pos = buf.getRemaining();

  assert(pos > 0 && pos <= getMaxUserDataSize());

  Packet::writePacket(raw);
  raw << (guint16)pos;

  raw.writeBuffer( buf );
}

void CustomPacket::readPacket( Buffer& raw ) {
  Packet::readPacket(raw);
  buf.clear();
  
  guint16 temp;
  raw >> temp;

  buf.writeBuffer( raw, (int)temp );
  buf.setLimit( temp );
}

} //namespace GNE
