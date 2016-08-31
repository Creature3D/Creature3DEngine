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
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/PacketParser.h"
#include "../include/gnelib/Buffer.h"
//////////////////////////////////////////////////////////////////////////
//
//吴财华
//引入PacketFuncsMap，扩展Packet ID为int型
//
//////////////////////////////////////////////////////////////////////////
namespace GNE {

const int Packet::ID = 0;

Packet::Packet(int id) : type(id) {
}

Packet::Packet( const Packet& o ) : type(o.type) {
}

Packet::~Packet() {
}

Packet* Packet::makeClone() const {
  return PacketParser::clonePacket( this );
}

int Packet::getType() const {
  return type;
}

int Packet::getSize() const {
  return Buffer::getSizeOf(type);
}

void Packet::writePacket(Buffer& raw) const {
  raw << type;
}

void Packet::readPacket(Buffer& raw) {
  //don't read type here, because our parser does this.
}

Packet& Packet::operator = (const Packet& rhs) {
  //The type should already by the same
  assert(type == rhs.type);
  return *this;
}

}





