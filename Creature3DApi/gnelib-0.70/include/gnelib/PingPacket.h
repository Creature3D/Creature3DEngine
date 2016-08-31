#ifndef _PINGPACKET_H_
#define _PINGPACKET_H_

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
#include "Mutex.h"
#include "Time.h"
#include <map>

namespace GNE {

/**
 * @ingroup midlevel
 *
 * Information returned by GNE::PingPacket::getPingInformation.
 */
struct PingInformation {
  PingInformation() : pingTime(0, 0), clockOffset(0, 0) {}
  
  /**
   * Round-trip ping time.
   */
  Time pingTime;

  /**
   * Clock offset.
   */
  Time clockOffset;
};

/**
 * @ingroup midlevel
 *
 * Using the PingPacket is one way to measure ping and clock offsets from a
 * remote machine.  Measuring ping and clock offset are very similar operations
 * in how they work, and because the extra overhead of measuring clock offset
 * is small, that functionality was added into PingPacket.  The effectiveness
 * of the clock synchronization is discussed in the GNE Protocol specification,
 * available on the GNE web site. Ping capability was not integrated into the
 * Connection class to give you the largest flexibility on how to measure
 * latency and ping in your games.
 *
 * A choice had to be made between synchronizing actual real clock times, or
 * synchronizing on an arbitrary server clock. Due to the fact that absolute
 * time clocks have less resolution in some platforms (ie Windows), and
 * because of possible difficulties surrounding differing timezones and
 * GMT-UTC differences, the clock synchronization will be based on an
 * arbitrary clock on the server. This allows the server to use the clock with
 * the highest resolution, and perhaps more importantly, the server can use
 * the same clock for synchronizing and for timing the game logic. The
 * important part to know from this is that the clock offsets may be extreme
 * (as GMT is used in Linux, and seconds since CPU powerup in Windows), and
 * thus may not fit into a 32-bit integer with microsecond accuracy.
 *
 * Since the PingPackets will be placed into the queue and received as
 * normal, this will be measuring effective ping rather than the actual time
 * it takes for the packet to travel there and back, giving a more realistic
 * view of lag.
 *
 * The way to use PingPackets:
 * <ul>
 *   <li>Create a new PingPacket.  The constructor will make a ping request
 *       with the proper unique request ID.</li>
 *   <li>Send the packet to whoever you want, any way you want.</li>
 *   <li>When reading the incoming packet stream, check for PingPackets.<br>
 *       Discover if they are requests or replies by calling isRequest.<br>
 *       If they are requests, call makeReply and send it back.<br>
 *       If they are replies, call getPing to measure the round trip time.</li>
 *   <li>If you send packets over an unreliable connection, call
 *       recoverLostRequests to declare late packets as "lost" and recover any
 *       used memory in the request table.</li>
 * </ul>
 *
 * Check out the exping example for an example and more explaination on how
 * to use PingPacket.
 */
class GNE_EXPORT PingPacket : public Packet {
public: //typedefs
  typedef SmartPtr<PingPacket> sptr;
  typedef WeakPtr<PingPacket> wptr;

public:
  /**
   * Creates a PingPacket with a preassigned request ID which is pulled from
   * an internal source.  Only ping requests are made using the constructor --
   * you should use getReply when you get a ping request to make the ping
   * response packet.
   *
   * The PingPacket::create function passes in false to get an uninitalized
   * PingPacket for reading.  You will probably only ever use the default
   * form of the constructor.  If you pass false, the state of this object is
   * undefined after creation and is suitable only to use readPacket on.
   */
  PingPacket(bool makeReq = true);

  virtual ~PingPacket();

  /**
   * The ID for this type of packet.
   */
  static const int ID;

  /**
   * Is this PingPacket a ping request?
   */
  bool isRequest();

  /**
   * Changes this PingPacket from a ping request to a ping reply.  If the
   * PingPacket is already a reply, this function has no net effect.
   */
  void makeReply();

  /**
   * If this packet is a ping reply that is from one of our requests, this
   * finds the round-trip time and clock offset.  Calling this function will
   * remove the request ID from the request table, so this function will only
   * work once for each reply.  If the request ID was not found, then times
   * of 0 will be returned.
   */
  PingInformation getPingInformation();

  /**
   * Every time you create a packet, the request ID is placed into a request
   * table so that the time difference can be measured later.  This takes up
   * some memory.  It is possible that a ping reply is never received,
   * espically if sending PingPackets over an unreliable connection.  This
   * function will remove any packets that have been pending for longer than
   * the time specified.  It will return the number of packets that were
   * considered lost based on the specified pending time limit and remove
   * these requests from the table.  If a reply for one of these lost
   * requests comes back, getPing will be unable to find them and return an
   * elapsed time of 0.
   *
   * When GNE is shutdown the request table will be cleared out so you don't
   * have to ever call this function unless you want to find out how many
   * more packets have been lost, or if you want to free some memory.
   */
  static int recoverLostRequests(Time limit);

  /**
   * Returns the number of pending requests.  This number increases by 1
   * every time a request is made, and goes down by one with a successful
   * call to getPing.  The number will also decrease if recoverLostReqests
   * is called and finds late requests.
   */
  static int reqsPending();

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
   * Returns a new instance of this class using the constructor to pass in
   * false, so this returns an object in an uninitialized state and suitable
   * only to call readPacket on.
   */
  static Packet* create();

private:
  guint32 reqId;

  /**
   * This is the time the remote end received the packet.
   */
  Time T2;

  /**
   * This is the time the remote end sent the reply.
   */
  Time T3;

  //Provides syncronization for nextReqId and requests
  static Mutex sync;

  static guint32 nextReqId;

  static std::map<guint32, Time> requests;
};

}
#endif
