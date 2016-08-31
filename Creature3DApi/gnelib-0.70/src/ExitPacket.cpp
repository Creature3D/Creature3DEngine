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
#include "../include/gnelib/ExitPacket.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/Buffer.h"

namespace GNE {

const int ExitPacket::ID = 1;//2;

ExitPacket::ExitPacket() : Packet(ID) {
}

ExitPacket::~ExitPacket() {
}

int ExitPacket::getSize() const {
  return Packet::getSize() + 2;
}

void ExitPacket::writePacket(Buffer& raw) const {
  Packet::writePacket(raw);
  gint16 size = 0;
  raw << size;
}

void ExitPacket::readPacket(Buffer& raw) {
  Packet::readPacket(raw);
  gint16 size;
  raw >> size;
}

}
