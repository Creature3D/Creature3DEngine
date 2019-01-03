#ifndef _CUSTOMPACKET_H_
#define _CUSTOMPACKET_H_

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

#include "Packet.h"
#include "Buffer.h"

namespace GNE {

/**
 * @ingroup midlevel
 *
 * Many times, espically during connection, you want to send some more
 * "free-form" data that may or may not all be related and will only be sent
 * once.  It may not make sense to create a completely new packet type just
 * to send a few things one time.  This packet type will allow you to send
 * whatever you want.  Basically, CustomPacket is just a packet that contains
 * a Buffer.  You can use the Buffer to put in data and pull it out.
 * Remember Buffer does endian and processor-type conversions for you.
 *
 * The maximum amount of data that can be stored in the CustomPacket is
 * defined by its Buffer's capacity, which at the current time is 3 bytes
 * smaller than Buffer::RAW_PACKET_LEN.
 *
 * See the documentation for Packet for more info on some of these functions.
 */
class CustomPacket : public Packet {
public: //typedefs
  typedef SmartPtr<CustomPacket> sptr;
  typedef WeakPtr<CustomPacket> wptr;

public:
  CustomPacket();

  CustomPacket( const CustomPacket& o );

  virtual ~CustomPacket();

  /**
   * Returns the capacity of Buffers created by CustomPacket made for writing.
   */
  static int getMaxUserDataSize();

  /**
   * The ID for this type of packet.
   */
  static const int ID;

  /**
   * Returns the Buffer for reading or writing.  You should not keep the
   * returned reference longer than the packet's destruction, or a reset or
   * readPacket call.
   *
   * When writing data to the buffer, do not call "flip" after writing data.
   * The position should be left at the point after your last write call.
   *
   * After a CustomPacket has been read, and you are pulling data out of it,
   * the position and the limit will be the number of bytes stored in this
   * CustomPacket.  You need to call rewind or flip (at this point, both are
   * equivalent) on the buffer before pulling data from it, or a buffer
   * overflow exception will result.
   */
  Buffer& getBuffer();

  /**
   * If you want to reuse a CustomPacket after using it for reading or
   * writing, you should call clear which will reset this object as if it were
   * newly constructed with the default constructor.
   */
  void clear();

  /**
   * @see Packet::getSize()
   */
  virtual int getSize() const;

  /**
   * Writes a CustomPacket to the given Buffer.  Flip will be called on the
   * Buffer then it will be written to the passed Buffer, so the final result
   * will be an unchanged position, but limit == position.
   */
  virtual void writePacket( Buffer& raw ) const;

  /**
   * Reads a CustomPacket from the given Buffer.  The data in this packet
   * will be erased and replaced by the new data.  Then flip is called on the
   * Buffer, so position is 0 and the bytes this CustomPacket contains is the
   * Buffer's limit.
   */
  virtual void readPacket( Buffer& raw );

private:
  //This may be changed to a Buffer* to conserve memory like the old CustomPacket
  //did when it was using RawPacket.
  mutable Buffer buf;
};

} //namespace GNE

#endif //#ifndef _CUSTOMPACKET_H_
