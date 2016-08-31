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

#include "../include/gnelib/gneintern.h"
#include "../include/gnelib/Buffer.h"
#include "../include/gnelib/Packet.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/Errors.h"

namespace GNE {

const int Buffer::RAW_PACKET_LEN = NL_MAX_PACKET_LENGTH - 4;//NL_MAX_PACKET_LENGTH;//NL_MAX_PACKET_LENGTH 16384 16k

Buffer::Buffer() : position( 0 ), limit( RAW_PACKET_LEN ),
capacity( RAW_PACKET_LEN ), data( new gbyte[ RAW_PACKET_LEN ] ) {
  assert( data );
}

Buffer::Buffer( int size ) : position( 0 ), limit( size ),
capacity( size ), data( new gbyte[ size ] ) {
  assert( data );
}

Buffer::Buffer( const Buffer& o ) : position( o.position ), limit( o.limit ),
capacity( o.capacity ), data( new gbyte[ capacity ] ) {
  memcpy( data, o.data, capacity );
}

Buffer::~Buffer() {
  delete[] data;
}

Buffer& Buffer::operator = ( const Buffer& rhs ) {
  if ( capacity < rhs.capacity ) {
    //recreate data
    delete[] data;
    data = NULL; //in case of exception on next line.
    data = new gbyte[ rhs.capacity ];
    capacity = rhs.capacity;
  }

  position = rhs.position;
  limit = rhs.limit;

  memcpy( data, rhs.data, rhs.capacity );

  return *this;
}

gbyte* Buffer::getData() {
  return data;
}

const gbyte* Buffer::getData() const {
  return data;
}

int Buffer::getPosition() const {
  return position;
}

void Buffer::setPosition( int newPosition ) {
  if ( newPosition > limit || newPosition < 0 )
    throw BufferError( Error::InvalidBufferPosition );

  position = newPosition;
}

void Buffer::rewind() {
  position = 0;
}

int Buffer::getLimit() const {
  return limit;
}

void Buffer::setLimit( int newLimit ) {
  //if ( newLimit < position || newLimit > capacity )
  //  throw BufferError( Error::InvalidBufferLimit );

  limit = newLimit;
}

int Buffer::getCapacity() const {
  return capacity;
}

int Buffer::getRemaining() const {
  return limit - position;
}

void Buffer::clear() {
  position = 0;
  limit = capacity;
}

void Buffer::flip() {
  limit = position;
  position = 0;
}

//START OF RAW OPERATIONS

void Buffer::writeBuffer( Buffer& src ) {
  writeBuffer( src, src.getRemaining() );
}

void Buffer::writeBuffer( Buffer& src, int length ) {
  if ( src.getRemaining() < length )
    throw BufferError( Error::BufferUnderflow );
  if ( getRemaining() < length )
    throw BufferError( Error::BufferOverflow );

  const void* srcPtr = &src.data[ src.position ];
  void* destPtr      = &data[ position ];

  memcpy( destPtr, srcPtr, length );

  position += length;
  src.position += length;
}

void Buffer::writeRaw(const gbyte* block, int length) {
  if ( position + length > limit )
    throw BufferError( Error::BufferOverflow );

  writeBlock(data, position, block, length);
  assert(position <= RAW_PACKET_LEN);
}

void Buffer::readRaw(gbyte* block, int length) {
  if ( position + length > limit )
    throw BufferError( Error::BufferUnderflow );

  readBlock(data, position, block, length);
  assert(position <= RAW_PACKET_LEN);
}

//gint8 Buffer::peekgint8()
//{
//	gint8 x = 0;
//	if ( position + getSizeOf( x ) > limit )
//		throw BufferError( Error::BufferUnderflow );
//
//	readByte(data, position, x);
//	position--;
//	return x;
//}

//START OF WRITING OPERATORS

Buffer& Buffer::operator << (gint8 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeByte(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (guint8 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeByte(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (gint16 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeShort(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (guint16 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeShort(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (gint32 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeLong(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (guint32 x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeLong(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (gsingle x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeFloat(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (gdouble x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  writeDouble(data, position, x);
  return *this;
}

Buffer& Buffer::operator << (const std::string& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  assert(x.size() <= 255);
  guint8 length = (guint8)x.size();

  //Write the length byte
  *this << length;

  if (length) {
    //If the string contains data, copy it raw into the data buffer
    gbyte* start = &data[position];
    memcpy((void*)start, (const void*)x.data(), length);
    position += (int)length;
  }

  assert(position <= limit);
  return *this;
}

Buffer& Buffer::operator << (const Packet& x) {
  int newPos = position + x.getSize();
  if ( newPos > limit )
    throw BufferError( Error::BufferOverflow );

  x.writePacket(*this);
  assert( position <= newPos ); //If this fails, getSize lied.
  return *this;
}

Buffer& Buffer::operator << (const Time& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferOverflow );

  *this << x.getSec() << x.getuSec();
  return *this;
}

//START OF READING OPERATORS

Buffer& Buffer::operator >> (gint8& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readByte(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (guint8& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readByte(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (gint16& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readShort(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (guint16& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readShort(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (gint32& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readLong(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (guint32& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readLong(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (gsingle& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readFloat(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (gdouble& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  readDouble(data, position, x);
  return *this;
}

Buffer& Buffer::operator >> (std::string& x) {
  int oldPos = position;

  //Get the length byte of the string.
  guint8 length;
  *this >> length;

  if (length) {
    if ( position + (int)length > limit ) {
      position = oldPos;
      throw BufferError( Error::BufferUnderflow );
    }

    //If the string has a non-zero length, create a new string.
    std::string temp( (char*)&data[position], (int)length );
    x = temp;
    position += (int)length;
  } else {
    x = "";
  }

  assert(position <= limit);
  return *this;
}

Buffer& Buffer::operator >> (Packet& x) {
  x.readPacket(*this);
  return *this;
}

Buffer& Buffer::operator >> (Time& x) {
  if ( position + getSizeOf( x ) > limit )
    throw BufferError( Error::BufferUnderflow );

  gint32 val;

  *this >> val;
  x.setSec( val );

  *this >> val;
  x.setuSec( val );

  return *this;
}

} //namespace GNE
