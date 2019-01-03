#ifndef _CHANNELPROVIDER_H_
#define _CHANNELPROVIDER_H_

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

#include "Mutex.h"
#include <list>

namespace GNE {
class Connection;
class Packet;
class ChannelPacket;

/**
 * @ingroup highlevel
 *
 * The ChannelProvider provides for a set of sets of Connection objects, which
 * are used as broadcast channels to broadcast packets to. A single Connection
 * can be in zero or more channels.
 *
 * This class is part of the high-level GNE API, and is mostly meant to be
 * used by the Server class, but users of the mid-level API who do not wish to
 * use the Server/Client API may also find this class useful as well.
 *
 * The valid range of channels is from 0 to 255, both sides inclusive.  These
 * values are reflected in the MIN_CHANNEL and MAX_CHANNEL static members.
 *
 * This class is entirely thread-safe.  The methods can all be called from
 * multiple threads concurrently, and will retain their expected behaviors in
 * all cases.  The obvious exception is that no threads can be accessing the
 * object if another thread is destroying or has destroyed the object.
 */
class ChannelProvider {
public:
  /**
   * The minimum valid channel number.
   */
  static const int MIN_CHANNEL;

  /**
   * The maximum valid channel number.
   */
  static const int MAX_CHANNEL;
  
  /**
   * Creates a new ChannelProvider, with no active channels.
   */
  ChannelProvider();

  ~ChannelProvider();

  /**
   * Adds a Connection to a channel.  If the channel does not exist, one is
   * transparently created (in other words, you do not need to explicitly open
   * new channels).
   *
   * If one of the two parameters are invalid, no action is taken.  In the
   * debugging version of the library, an assert will fail in that case.
   *
   * @param channel a channel from 0 to 255 (MIN_CHANNEL to MAX_CHANNEL)
   * @param conn a Connection to add, cannot be NULL.
   */
  void addConnection( int channel, Connection* conn );

  /**
   * Removes a Connection from a channel.  If the channel does not exist, or
   * if the Connection is not a member of the channel, this method will have
   * no effect.
   *
   * If one of the two parameters are invalid, no action is taken.  In the
   * debugging version of the library, an assert will fail in that case.
   *
   * @param channel a channel from 0 to 255 (MIN_CHANNEL to MAX_CHANNEL)
   * @param conn a Connection to remove, cannot be NULL.
   */
  void removeConnection( int channel, Connection* conn );

  /**
   * Removes the given Connection from all of the channels.  Useful for when
   * the Connection shuts down.
   */
  void removeFromAll( Connection* conn );

  /**
   * Removes all of the Connections from a channel.  If the channel parameter
   * is invalid, no action is taken, but in the debugging version of the
   * library, an assert will fail.
   * 
   * @param channel a channel from 0 to 255 (MIN_CHANNEL to MAX_CHANNEL)
   */
  void disbandChannel( int channel );

  /**
   * Returns the number of Connections currently part of the given channel.
   */
  int numConnections( int channel ) const;

  /**
   * Sends a packet to everyone on a channel.  Creates a ChannelPacket with
   * the specified channel and sends it to all Connections, except the one
   * specified.  Excluding a Connection is useful for when you want to send to
   * everyone but the Connection who sent the packet.
   *
   * It is acceptable if the channel does not exist -- in this case no packets
   * will be sent.  It is also acceptable if the exclude parameter does not
   * exist in the channel -- in this case, the packet will be sent to all
   * Connections.
   *
   * If the channel parameter is invalid, no action is taken, but in the
   * debugging version of the library, an assert will fail.
   *
   * @param channel the channel to send to.
   * @param from source information.
   * @param packet the packet to send.
   * @param exclude does not send packets to this player.  Can be NULL so that
   * no Connection is excluded.
   * @param reliable true if the packet should be sent reliably.
   */
  void sendToChannel( int channel, int from, const Packet& packet,
    Connection* exclude, bool reliable ) const;

  /**
   * Works just like #sendToChannel( int, int, Packet&, Connection* ), except
   * it takes an already created ChannelPacket.
   */
  void sendToChannel( ChannelPacket& packet, Connection* exclude,
    bool reliable ) const;
  
private:
  //ChannelProvider is not copyable
  ChannelProvider( const ChannelProvider& o );
  ChannelProvider& operator=( const ChannelProvider& rhs );

  struct Channel {
    std::list<Connection*> conns;
  };

  Channel* channels[255];
  mutable Mutex sync;
};

} //namespace GNE

#endif

