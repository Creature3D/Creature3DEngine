#ifndef _EXITPACKET_H_
#define _EXITPACKET_H_

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
#include "export.h"
#include "Packet.h"

namespace GNE {

/**
 * @ingroup midlevel internal
 *
 * The ExitPacket is sent automatically by GNE when you disconnect in a
 * proper manner.  You need not use this class.  When you receive an
 * ExitPacket, though, a graceful disconnect is initiated that generates an
 * onExit event.
 */
class GNE_EXPORT ExitPacket : public Packet {
public: //typedefs
  typedef SmartPtr<ExitPacket> sptr;
  typedef WeakPtr<ExitPacket> wptr;

public:
  ExitPacket();

  virtual ~ExitPacket();

  /**
   * The ID for this type of packet.
   */
  static const int ID;

  /**
   * Returns the current size of this packet in bytes.
   */
  virtual int getSize() const;

  /**
   * Writes the packet to the given Buffer. 
   */
  virtual void writePacket(Buffer& raw) const;

  /**
   * Reads this packet from the given Buffer.
   */
  virtual void readPacket(Buffer& raw);

private:
};

}
#endif
