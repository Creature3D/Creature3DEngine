#ifndef _CHANNELPACKET_H_254jfa
#define _CHANNELPACKET_H_254jfa

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
#include "WrapperPacket.h"

namespace GNE {

/**
 * @ingroup highlevel
 *
 * A ChannelPacket wraps around a normal Packet to give that Packet a channel
 * destination and information about its source.  This is a packet type meant
 * to be used with the high-level API, and provides routing information for
 * the Server class.
 */
class ChannelPacket : public WrapperPacket {
public:
  /**
   * Default constructor for the create function.  The constructed
   * ChannelPacket is valid only for a readPacket call.  You must use the
   * constructor with parameters to create a ChannelPacket for sending.
   */
  ChannelPacket();

  /**
   * Inits a new ChannelPacket for the given channel, and from the given
   * source ID.  A copy of the passed packet is made and stored inside the
   * ChannelPacket.  This copy is made using the Packet::makeClone method.
   */
  ChannelPacket( int channel, int from, const Packet& packet );

  virtual ~ChannelPacket();

  /**
   * The ID for this type of packet.
   */
  static const int ID;

  /**
   * Returns the channel this packet is meant to be sent to, or came in from,
   * depending on the context.
   */
  int getChannel() const;

  /**
   * Sets the channel this Packet is meant for.  If the channel number given
   * is out of range, this method has no effect, but in the debugging version
   * of the library, an assert will be triggered.
   *
   * @param chan the channel number in the range of [0..255]
   */
  void setChannel( int chan );

  /**
   * Returns the source ID for this packet.  A source ID of 0 is commonly
   * considered to be "unknown."
   */
  int getSource() const;

  /**
   * Sets the source ID for this packet.
   * @param source the source ID in the range [0..255]
   */
  void setSource( int source );

  /**
   * Returns a newly allocated exact copy of this packet.
   */
  virtual Packet* makeClone() const;

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
   * Returns a new instance of this class suitable only to call readPacket on.
   */
  static Packet* create();

private:
  int channel;
  int from;
};

} //namespace GNE

#endif
