#ifndef PACKET_H_INCLUDED_C51B374A
#define PACKET_H_INCLUDED_C51B374A

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
//吴财华
//引入PacketFuncsMap，扩展Packet ID为int型
//
//////////////////////////////////////////////////////////////////////////
#include "export.h"
#include "SmartPtr.h"
#include "WeakPtr.h"

namespace GNE {
class Buffer;

/**
 * @ingroup midlevel
 *
 * The base packet class, used for dealing with all types of packets at a
 * fundamental level.  All types of packets must inherit from this class to
 * be recognized by %GNE.  %GNE already implmenets some types of packets.
 *
 * When you create a new packet you MUST define your own versions of all
 * virtual functions or the program will fail.
 *
 * See the example expacket on how to properly derive from a Packet class, or
 * look at the code for the other %GNE packets.
 */
class GNE_EXPORT Packet {
public: //typedefs
  typedef SmartPtr<Packet> sptr;
  typedef WeakPtr<Packet> wptr;

public:
  /**
   * Constructs a packet with the given ID.  If you pass no ID, the ID for an
   * empty packet is assumed.  Normally sending blank packets are not useful,
   * however.  If you are wanting to use a blank packet for some purpose such
   * as a end-of-data marker or for sending some message that requires no
   * data, it is suggested that you simply derive a class from Packet that
   * adds no data, but has a unique ID so it can be "recognized" easier.
   *
   * @param id a number from PacketParser::MIN_USER_ID to
   *   PacketParser::MAX_USER_ID, inclusive.  %GNE packet id's are less than
   *   MIN_USER_ID.
   */
  explicit Packet(int id = 0);

  /**
   * Copy constructor.  If your Packet is using the default packet clone
   * function in registration, remember that it uses the copy constructor so
   * if you need to override the default implementation, you must do it.
   */
  Packet( const Packet& o );

  virtual ~Packet();

  /**
   * The ID for this type of packet.
   */
  static const int ID;

  /**
   * Returns a newly allocated exact copy of this packet, using the
   * PacketParser::clonePacket function.
   */
  Packet* makeClone() const;

  /**
   * Returns the type of this instance.  This allows you to identify the type
   * of packet you have when you only have a Packet*.
   */
  int getType() const;

  /**
   * Returns the current size of this packet in bytes.  When overloading this
   * function, call getSize on the parent class then add the sizes of your
   * additional variables.  If the size cannot be determined, then getSize
   * should return a value <= Buffer::RAW_PACKET_LEN but >= its possible
   * size -- so in other words if the size cannot be determined, it should
   * return the largest possible size that given packet could be.  This is
   * discouraged as much as possible since GNE allocates packets in the data
   * stream based on this value, and large values will hinder performance.
   */
  virtual int getSize() const;

  /**
   * Writes the packet to the given Buffer.  You can continue writing more
   * packets to the Buffer after this method.  You must make sure there
   * is enough space in the Buffer to fit this new packet.  When
   * overloading this function, call writePacket on the parent class then
   * write your own variables.
   *
   * No more than getSize() bytes may be written to the Buffer.
   *
   * This method is allowed to throw any subclass of Error.  Typically this
   * only happens if accesses to Packet case a buffer overflow.
   */
  virtual void writePacket(Buffer& raw) const;

  /**
   * Reads this packet from the given Buffer.  When overloading this
   * function, call readPacket on the parent class then read your own
   * variables.
   *
   * This method is allowed to throw any subclass of Error.  This can happen
   * if accessing the Packet causes a buffer underflow, but an error might
   * occur in the format or consistancy of the data for the user's derived
   * class.  If you cannot construct a proper Packet of your type from the
   * data in raw, then you should throw a subclass of Error.
   */
  virtual void readPacket(Buffer& raw);

  /**
   * Copy operator you can use to help you in creating your own.
   * There is a debug assert in this function that checks to make sure the
   * types match.  Call this operator first from your copy operator.  Many
   * GNE packets may not support this operation, so check the documentation
   * first -- if no operator = exists, then assume you cannot copy packets
   * this way, unless the documentation says otherwise.
   *
   * If you can't use operator= on a packet, you can use makeClone to
   * achieve a nearly equivalent result.
   */
  Packet& operator = (const Packet& rhs);

private:
  /**
   * The type ID for this Packet.
   */
  int type;
};

}
#endif /* PACKET_H_INCLUDED_C51B374A */

