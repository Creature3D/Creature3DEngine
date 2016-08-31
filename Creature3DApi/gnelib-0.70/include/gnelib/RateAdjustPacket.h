#ifndef _RATEADJUSTPACKET_H_
#define _RATEADJUSTPACKET_H_

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
 * @ingroup internal
 *
 * The RateAdjustPacket is sent automatically by GNE when you change rates in
 * the PacketStream.  It is not a packet that you send, or that you will
 * see -- it is used only internally by GNE.
 */
class GNE_EXPORT RateAdjustPacket : public Packet {
public: //typedefs
  typedef SmartPtr<RateAdjustPacket> sptr;
  typedef WeakPtr<RateAdjustPacket> wptr;

public:
  RateAdjustPacket();

  virtual ~RateAdjustPacket();

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

  /**
   * The requested rate.
   */
  guint32 rate;

private:
};

}
#endif
