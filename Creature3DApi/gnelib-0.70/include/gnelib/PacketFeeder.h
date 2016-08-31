#ifndef PACKETFEEDER_H_KOP131
#define PACKETFEEDER_H_KOP131

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
#include "SmartPointers.h"

namespace GNE {
  class PacketStream;

/**
 * @ingroup midlevel
 *
 * The PacketFeeder class is a callback for the PacketStream thread when it
 * is running low on packets.  How low before the callback occurs is defined
 * by a parameter set in the PacketStream class during connection creation or
 * through PacketStream::setLowPacketThreshold.  This class replaces the old
 * onDoneWriting event in ConnectionListener, because this method allows for
 * the writing to be stalled during this callback and also allows for the
 * callback to occur before all packets run out.
 *
 * This is a good thing.  It cuts out the extra latency from passing the
 * event to the EventThread and getting the packets back, and in addition
 * allows the PacketStream to never run out of data and therefore take
 * advantage of optimizing the packet stream through packet combining.  Since
 * the callback takes place in the writing thread itself, it cannot be
 * writing therefore you can send out a batch of packets with a better
 * assurance they will be combined and optimized.
 */
class GNE_EXPORT PacketFeeder {
public: //typedefs
  typedef SmartPtr<PacketFeeder> sptr;
  typedef WeakPtr<PacketFeeder> wptr;

public:
  virtual ~PacketFeeder() {}

  /**
   * The specified PacketStream is running low on packets, and you are given
   * the opportunity at this time to add more packets to its outgoing queue.
   * The callback will be called at least once when the PacketStream gets
   * sufficently low.  If you do not add enough packets it may be called
   * (actually very likely) multiple times.  If the PacketStream actually
   * runs out of packets the writer thread will wait and will attempt to
   * reprocess the event after the suggested timeout set by
   * PacketStream::setFeederTimeout.
   *
   * You can use the informational methods of ps to get information about
   * how many packets are left and such to better optimize your sending.
   */
  virtual void onLowPackets(PacketStream& ps) = 0;
};

} //namespace GNE

#endif
