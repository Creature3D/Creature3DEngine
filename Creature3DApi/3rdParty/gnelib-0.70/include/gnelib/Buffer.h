#ifndef BUFFER_H_LRP401
#define BUFFER_H_LRP401

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
#include "gnetypes.h"

namespace GNE {
  class Time;
  class Packet;

/**
 * The Buffer class provides functionality to pull data types out of a raw
 * binary data buffer.  This class is the replacement for the Buffer class
 * that used to exist.  There are a few reasons for this rename.  The first
 * being that I wanted to fix up the interface -- it had changed so much I
 * didn't want people getting confused so it's a new class now.  The second
 * reason is because people were getting confused thinking Buffer was a
 * Packet type since all Packets typically are named WhateverPacket.  The
 * third reason is because I chose to somewhat base my interface off a class
 * by the same name in Java, java.nio.Buffer.
 *
 * Advantages over Buffer include easier and much much safer memory
 * allocation and management.  Previously in Buffer there was no knowledge
 * about the buffer size and it was very easy to overflow or underflow the
 * buffer.
 *
 * This Buffer class should be very safe in that it won't allow you under or
 * overflow.
 *
 * The Buffer has 3 key concepts, the position, the limit, and the capacity.
 *
 * The position is the byte offset where the next byte will be read to or
 * written from.  The limit is first byte that is invalid to access.  The
 * capacity is the size of the underlying byte array allocated to this Buffer.
 *
 * It is always true that 0 <= position <= limit <= capacity.
 *
 * Data is always written to the Buffer in %GNE's network data format.  And when
 * reading it is assumed to be in %GNE's network data format.  Obviously, when
 * using the rawRead and rawWrite methods no endian or format conversions are
 * applied to the data read or written in those methods.
 *
 * Most of the methods in Buffer throw BufferError when an error occurs,
 * usually because of buffer overflow or underflow.  Unless otherwise stated,
 * if an exception is thrown during a Buffer operation, the Buffer is
 * unchanged.  Exceptions include the read packet and write packet operations.
 */
class GNE_EXPORT Buffer {
public:
  /**
   * Creates a new Buffer of RAW_PACKET_LEN capacity.
   *
   * @post position == 0
   * @post limit == RAW_PACKET_LEN
   * @post capacity == RAW_PACKET_LEN
   */
  Buffer();

  /**
   * Creates a new Buffer of size capacity.
   *
   * @post position == 0
   * @post limit == size
   * @post capacity == size
   */
  Buffer( int size );

  /**
   * Creates a Buffer that is a copy of the passed Buffer.  Works similarly
   * to operator = for Buffer, in that 2 identical but independent Buffers are
   * created as a result of this operation, but the capacities will be the same
   * after the operation.
   */
  Buffer( const Buffer& o );

  /**
   * Destructor.
   */
  ~Buffer();

  /**
   * Copies the contents of the right side Buffer to the left side.  Produces
   * two Buffers with the same data, position, and limit.  Changes made to one
   * will not affect the other.
   *
   * To make this operation efficent, the backing array for this Buffer is only
   * recreated if the capacity of the left Buffer is smaller than the capacity
   * of the right.  Thus the resulting Buffer's capacity is equal to or greater
   * than the capacities of the two Buffers before the operation.
   */
  Buffer& operator = ( const Buffer& rhs );

  /**
   * Returns a pointer to the start of the backing byte buffer for this object.
   * Unfortunately this method is a necessary evil since at some point the
   * Buffer's data needs to be passed into some low-level system I/O function.
   * It is suggested that this method be used only when necessary, to benefit
   * from the overflow/underflow detection that the Buffer class provides.
   */
  gbyte* getData();

  /**
   * const form of the normal getData method.
   */
  const gbyte* getData() const;

  /**
   * Returns the current position for the Buffer -- this is the next byte that
   * will be read from or written to.  Its value is always between 0 and limit,
   * inclusive.
   */
  int getPosition() const;

  /**
   * Sets the new position for the Buffer.
   *
   * @throws BufferError with InvalidBufferPosition code if
   *   newPosition > limit or if newPosition < 0.
   */
  void setPosition( int newPosition );

  /**
   * A shortcut for setPosition( 0 ).
   */
  void rewind();

  /**
   * Returns the limit for this buffer.  The limit is the first byte of data
   * that may not be accessed in any way.
   */
  int getLimit() const;

  /**
   * Sets the limit for the Buffer.
   *
   * @throws BufferError with InvalidBufferLimit code if newLimit is less than
   *   position or greater than capacity.
   */
  void setLimit( int newLimit );

  /**
   * Returns the size in bytes of the backing byte buffer.
   */
  int getCapacity() const;

  /**
   * Returns limit - position.
   */
  int getRemaining() const;

  /**
   * Readies the buffer for writing.  The bytes in the buffer are not actually
   * cleared by this method.
   *
   * @post position == 0
   * @post limit == capacity
   * @post capacity is unchanged
   */
  void clear();

  /**
   * After the data has been written into the Buffer, readies it for reading.
   * The current position for this Buffer becomes the new limit, and position
   * is set back to 0.
   *
   * @post position == 0
   * @post limit == current position
   * @post capacity is unchanged
   */
  void flip();

  /**
   * Returns the maximum possible serialized size of a string of the given
   * length in ASCII characters.
   */
  static int getStringSize(int x) { return x + 1; }

  /**
   * Returns the serialized size in bytes of the given variable, to be used
   * in your overridden Packet::getSize method.
   */
  static int getSizeOf(const std::string& x) { return (int)(x.size() + 1); }
  static int getSizeOf(gint8 x) { return sizeof(x); }
  static int getSizeOf(guint8 x) { return sizeof(x); }
  static int getSizeOf(gint16 x) { return sizeof(x); }
  static int getSizeOf(guint16 x) { return sizeof(x); }
  static int getSizeOf(gint32 x) { return sizeof(x); }
  static int getSizeOf(guint32 x) { return sizeof(x); }
  static int getSizeOf(gsingle x) { return sizeof(x); }
  static int getSizeOf(gdouble x) { return sizeof(x); }
  static int getSizeOf(const Time& x) { return sizeof(gint32)*2; }

  /**
   * Writes all remaining bytes in Buffer src to this Buffer.  This is the
   * same as calling writeBuffer( src, src.getRemaining() ).
   *
   * Only an overflow Error can be thrown from this method.
   *
   * @throws BufferError if this Buffer cannot hold all of the data remaining
   *   in src.
   */
  void writeBuffer( Buffer& src );

  /**
   * Writes length bytes from the Buffer src into this Buffer.  This is done
   * by reading length bytes from src, then writing length bytes into this
   * object.  The position of each Buffer is increased by length.
   *
   * If an Error is thrown because of underflow on src or overflow on this
   * object, no changes are made to either Buffer.
   *
   * @throws BufferError if this Buffer cannot hold length more data, or if
   *   the buffer src does not have length more data to read.
   */
  void writeBuffer( Buffer& src, int length );

  /**
   * Writes raw data starting at the current position.  This acts similar to
   * the memcpy function.  The position is moved up by length.
   *
   * @see readRaw
   */
  void writeRaw(const gbyte* block, int length);

  /**
   * Like writeRaw, but just the other way around.  The position is moved up
   * by length.
   *
   * @see writeRaw
   */
  void readRaw(gbyte* block, int length);

  /**
   * Stream operators for writing to this Buffer.  All data is converted
   * when appropriate into little endian format, and whatever other conversions
   * are required to place the data into %GNE network format.
   *
   * Any of the write operators will increase the position and will throw
   * a BufferError with code BufferOverflow if there is not room in the buffer.
   */
  Buffer& operator << (gint8 x);
  /**
   * This operator also works for gbool and gbyte.
   */
  Buffer& operator << (guint8 x);
  Buffer& operator << (gint16 x);
  Buffer& operator << (guint16 x);
  Buffer& operator << (gint32 x);
  Buffer& operator << (guint32 x);
  Buffer& operator << (gsingle x);
  Buffer& operator << (gdouble x);
  Buffer& operator << (const std::string& x);
  Buffer& operator << (const Time& x);

  /**
   * Writes a packet to the Buffer.  This function will simply call the
   * packet's writePacket function.  If the writePacket method throws an
   * exception, the buffer may have been modified if the packet successfully
   * completed some of its write operations.
   *
   * An Error with code BufferOverflow will be thrown if there is not enough
   * room in the buffer for packet.getSize() more bytes.
   *
   * @see Packet::writePacket
   */
  Buffer& operator << (const Packet& packet);

  /**
   * Stream operators for reading from this Buffer.
   *
   * Any of the read operators will increase the position and will throw
   * a BufferError with code BufferUnderflow if there is no more data to read.
   */
  Buffer& operator >> (gint8& x);
  /**
   * This operator also works for gbool and gbyte.
   */
  Buffer& operator >> (guint8& x);
  Buffer& operator >> (gint16& x);
  Buffer& operator >> (guint16& x);
  Buffer& operator >> (gint32& x);
  Buffer& operator >> (guint32& x);
  Buffer& operator >> (gsingle& x);
  Buffer& operator >> (gdouble& x);
  Buffer& operator >> (std::string& x);
  Buffer& operator >> (Time& x);

  /**
   * This function calls the packet's readPacket function.  You will already
   * need to know what kind of packet is next.  If you don't, use
   * PacketPaser::parseNextPacket.  If an exception occurs during the
   * readPacket method, then the Buffer may have been modified if any calls to
   * the Buffer succeeded.
   *
   * @see PacketParser::parseNextPacket
   * @see Packet::readPacket
   */
  Buffer& operator >> (Packet& packet);

  /**
   * The max length of a %GNE network packet.  This constant is used mostly by
   * %GNE's internal code to create packet combonation buffers.
   */
  static const int RAW_PACKET_LEN;

  //gint8 peekgint8();
private:
  int position;
  int limit;
  int capacity;

  gbyte* data;
};

}

#endif
