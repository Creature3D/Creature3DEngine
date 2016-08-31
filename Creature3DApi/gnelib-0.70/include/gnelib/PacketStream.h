#ifndef PACKETSTREAM_H_INCLUDED_C51CCBFF
#define PACKETSTREAM_H_INCLUDED_C51CCBFF

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

#include "ConditionVariable.h"
#include "Mutex.h"
#include "Thread.h"
#include "Time.h"
#include "SmartPointers.h"

#include <queue>
//#include <CRCore/thread/crCondition.h>
//#include <CRCore/thread/crMutex.h>
namespace GNE {
class Packet;
class Connection;
class Buffer;
class PacketFeeder;

/**
 * @ingroup midlevel
 *
 * This class resembles a packet stream through a connection.  This class
 * is maintained by the Connection class, and you shouldn't have to create
 * your own PacketStreams.  You should always retrieve it through the
 * Connection::stream method.
 *
 * NOTE: all functions in this class are thread safe, since this class uses
 *       its own mutexes internally.  Note that data in the class may change
 *       between calls, if another thread changes its state.
 */
class GNE_EXPORT PacketStream : public Thread {
protected:
  /**
   * @see create
   */
  PacketStream(int reqOutRate, int maxOutRate, Connection& ourOwner);

public:
  typedef SmartPtr<PacketStream> sptr;
  typedef WeakPtr<PacketStream> wptr;

  /**
   * Creates a new PacketStream with the given flow control parameters.
   * Passing a value 0 for a rate is interpreted as "unlimited" or
   * unrestricted rates.  Passing a value less than 0 is not allowed.
   *
   * By default no PacketFeeder is set.
   *
   * @param reqOutRate This is the out rate that we are requesting, or in
   *                   other words, the maximum rate we are willing to send.
   * @param maxOutRate The maximum rate the remote machine is letting us
   *                   send.  The actual outgoing rate, therefore, is the
   *                   minimum of the two outgoing rate values.
   */
  static sptr create(int reqOutRate, int maxOutRate, Connection& ourOwner);

  /**
   * Destroys this object.  Any data left remaining in the in or out queues
   * is destroyed as well.
   */
  virtual ~PacketStream();

  /**
   * Returns the current incoming queue length in packets.
   */
  int getInLength() const;

  /**
   * Returns the current outgoing queue length in packets.  This is meant
   * as a possible hint for your application to tune its performance by
   * knowning when the queues are starting to fill up.
   *
   * @param reliable true for the outgoing reliable packet queue.<br>
   *                 false for the outgoing unreliable packet queue.
   */
  int getOutLength(bool reliable) const;

  /**
   * Sets a new PacketFeeder that this class uses to generate onLowPackets
   * events to.  This function blocks until the feeder has been changed.
   * When the PacketStream starts with no packets and a feeder becomes set
   * the first onLowPackets will be generated, so that your PacketFeeder can
   * be used even to generate the initial packets after the connection event.
   *
   * The onLowPackets event will be flagged to be triggered when this
   * function is called if conditions are proper, disregarding the timeout
   * -- meaning if you set a feeder for a connection with no packets in the
   * outgoing queue, an onLowPackets event will be generated.
   *
   * The passed newFeeder may be NULL in which case onLowPackets events will
   * not be generated.
   *
   * After the connection has been disconnected, the reference to the feeder
   * is dropped to prevent cycles.  After disconnection, this method no longer
   * has any effect.
   */
  void setFeeder(const SmartPtr<PacketFeeder>& newFeeder);

  /**
   * When the number of packets in this PacketStream falls below limit, at
   * least one onLowPackets event is generated.  If this value is 0, then the
   * event will only be generated when the queue empties entirely.
   *
   * The conditions for an onLowPackets event will be reevaluated when this
   * method is called without regard to the timeout, and an onLowPackest event
   * will be generated if the conditions are proper.
   */
  void setLowPacketThreshold(int limit);

  /**
   * Returns the current low packet threshold.
   */
  int getLowPacketThreshold() const;

  /**
   * Sets the feeder timeout in milliseconds.  If the feeder chooses not to
   * send packets when the threshold is low, then the PacketStream will run
   * out of packets and go to sleep.  This timeout is an approximate time the
   * PacketStream will wait before generating a new onLowPackets event if
   * still no packets are ready for writing.  If this value is 0, then
   * onLowPackets will never be called again until packets are written from
   * an external thread.  A value less than 0 is invalid.  There is no
   * guarantee made about how accurate the callback rate will actually be,
   * except that one will eventually happen with a non-zero timeout and that
   * it will likely be called too soon rather than too late, since some method
   * invocations on PacketStream will trigger a premature timeout.
   */
  void setFeederTimeout(int ms);

  /**
   * Returns the set feeder timeout.
   */
  int getFeederTimeout() const;

  /**
   * Is there at least one packet in the incoming queue?  Note that this does
   * not guarantee that getNextPacket will return a non-NULL value if it is
   * possible for any other thread to try to get the data between your call
   * and getNextPacket.  This is only useful if you want to passively check
   * for incoming data.  If you are wanting to do anything with that data it
   * is suggested that you use getNextPacket and if it is non-NULL then
   * process the data.  That method is thread safe.
   */
  bool isNextPacket() const;

  /**
   * Returns the next packet from the queue, removing it from that queue.
   * It is your responsibility to deallocate the memory for this packet as
   * the calling code becomes the owner of the memory the returned packet
   * occupies.  You deallocate the Packet by using the
   * PacketStream::destroyPacket function.
   *
   * @return A pointer to the next packet, which you are responsible for
   *         deleting, or NULL if there is no next packet.
   */
  Packet* getNextPacket();

  /**
   * Returns the next packet from the queue, removing it from that queue.
   * The Sp stands for "SmartPtr" and returns a SmartPtr that will handle the
   * PacketStream::destroyPacket call for you.
   *
   * @return A pointer to the next packet, or NULL if there is no next packet.
   */
  SmartPtr<Packet> getNextPacketSp();

  /**
   * Adds a packet to the outgoing queue.  The packet given will be copied.
   * @param packet the packet to send.
   * @param should this packet be sent reliably if the connection supports it?
   */
  void writePacket(const Packet& packet, bool reliable);

  /**
   * Adds a packet to the outgoing queue.  The packet given will be copied.
   * This works identical to the other writePacket but takes a SmartPtr to a
   * Packet.
   *
   * @param packet the packet to send.
   * @param should this packet be sent reliably if the connection supports it?
   */
  void writePacket(const SmartPtr<Packet>& packet, bool reliable);

  /**
   * Returns the actual outgoing data rate, which may be the same or less
   * that what was originally requested on connection.  This value is the
   * minimum between the max rate limit from the remote computer, and our
   * maximum outgoing rate.  A value of 0 means there is no outgoing rate
   * limit.
   *
   * If the requested rate changes, or if the remote computer changes its
   * max allowed limit, this number will change to the new minimum between
   * these rates.
   */
  int getCurrOutRate() const;

  /**
   * Returns the maximum outgoing rate that we are requesting.  The value 0
   * means that no rate limit was requested.
   * @see getCurrOutRate
   * @see setRates
   */
  int getReqOutRate() const;

  /**
   * Returns the maximum outgoing rate that the remote computer is allowing
   * us to send to it.  This value can change if the remote machine changes
   * its rates through the setRates function.  The value 0 means that no
   * limit was requested.
   * @see getCurrOutRate
   * @see setRates
   */
  int getRemoteOutLimit() const;

  /**
   * Sets new values that we are willing to send or receive.  See the
   * constructor for more information.  Pass a value less than 0 to leave one
   * of the rates unchanged.  Pass the value 0 for "unrestricted" rates.
   * Changing the rates might cause a packet to get added to the outgoing
   * packet stream to communicate this change to the other side.
   *
   * @see PacketStream::PacketStream
   */
  void setRates(int reqOutRate2, int maxInRate2);

  /**
   * Blocks on this PacketStream until all packets have been sent.  Note that
   * if you have set an active packet feeder, and it is constaly adding
   * packets to the queue, this function is likely not to return until it
   * times out, so you may want to communicate with that feeder somehow or
   * turn off the feeder (by setting a NULL feeder) so more packets are not
   * being continuously added.
   * @param waitTime the max amount of time in ms to wait for the outgoing
   *        packet queue to clear.
   */
  void waitToSendAll(int waitTime = 10000) const;

  /**
   * Overrides Thread::shutDown so that the PacketStream daemon thread will
   * be woken up since it might be waiting on a ConditionVariable.
   */
  void shutDown();

  /**
   * Add the given packet to the incoming queue.  This is normalled used
   * internally by the Connection class to add the packets, but it is safe
   * for the user to call, if they want to delay processing of the packets
   * for a later time and see what other packets are available.\n
   * Remember queues are LIFO, meaning this inserted packet will be the last
   * in the queue after addition.\n
   * Also note that when you pass this object into the queue, you will
   * eventually get it back from getNextPacket(), so you will want to handle
   * deallocation at that point.
   */
  void addIncomingPacket(Packet* packet);

  void setBufSizeScale(int scale);
protected:
  /**
   * This thread handles throttled writes to the socket.
   */
  void run();

private:

  void prepareSend(/*std::queue<Packet*>& q*/bool reliable, Buffer& raw);

  Connection& owner;

  std::queue<Packet*> in;

  std::queue<Packet*> outUnrel;

  std::queue<Packet*> outRel;

  int inSize;
  int outUnrelSize;
  int outRelSize;

  int maxOutRate;

  int reqOutRate;

  //This is the precalculated min of maxOutRate and reqOutRate.
  int currOutRate;

  /**
   * This is the current number of bytes we are allowed to send without
   * waiting.  This value should increase per the outRateStep until it
   * reaches a maximum value of currOutRate.
   */
  int outRemain;

  /**
   * This is the "step" that the out value increases for every step in
   * time.  The size of the step is defined by the code.
   */
  int outRateStep;

  /**
   * The last time the rate was calculated.
   */
  Time lastTime;

  /**
   * Calculates the current rate and step based on the current values for
   * maxOutRate and reqOutRate.
   */
  void setupCurrRate();

  /**
   * Discovers the amount of time passed and updates the currOutRemain so we
   * know much data we can send.  This should be called almost everytime
   * before we use currOutRemain.  outQCtrl MUST be acquired when you call
   * this function.
   */
  void updateRates();

  //These 3 variables syncronized by outQCtrl, and must be since the writer
  //thread has to wait on conditions of the feeder.
  SmartPtr<PacketFeeder> feeder;
  bool feederAllowed; //set false after thread dies so feeder is never set again.
  bool onLowPacketsEvent;
  int feederTimeout;
  int lowPacketsThreshold;

  /**
   * Generates and processes the onLowPackets event immedately if conditions
   * are appropriate, returning when the event has completed.
   */
  void onLowPackets( int numPackets );

  /**
   * These are set to be mutable because of the const functions need
   * non-const access to these objects, but they can still be called const
   * because the object's state is the same before and after the method.
   */
  mutable Mutex inQCtrl;
  mutable ConditionVariable outQCtrl;

  //mutable CRCore::crMutex m_inQCtrl;
  //mutable CRCore::crMutex m_outQCtrl;
  //CRCore::crCondition m_feedCondition;
  //CRCore::crMutex m_feedMutex;

  //static int MaxPacketBufSize;//当超出MaxPacketBufSize则丢掉后来的包
  int m_bufSizeScale;
  int m_maxPacketBufSize;
};

}
#endif /* PACKETSTREAM_H_INCLUDED_C51CCBFF */
