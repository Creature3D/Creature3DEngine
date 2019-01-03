#ifndef ERROR_H_INCLUDED_C4514CDC
#define ERROR_H_INCLUDED_C4514CDC

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
#include <string>

namespace GNE {

/**
 * @ingroup midlevel
 *
 * This class resembles some sort of error in GNE, with the ability to obtain
 * a textual description of the error.  Objects of type Error are not thread
 * safe.  This is usually OK since Error objects are not typically shared by
 * threads.
 *
 * If you want to define your own classes from Error, feel free to do so.
 * This may be useful if you want to throw Errors during readPacket or
 * writePacket.  If you inherit from Error, unless one of the current error
 * codes apply, you should give the Error a code of User and overload the
 * toString method to provide your own error description.
 *
 * @see Packet#readPacket
 * @see Packet#writePacket
 */
class GNE_EXPORT Error {
public:
  /**
   * An enum describing connection failure type.
   */
  enum ErrorCode {
    NoError = 0,
    ProtocolViolation,
    GNETheirVersionLow,
    GNETheirVersionHigh,
    WrongGame,
    UserVersionMismatch,
    CouldNotOpenSocket,
    ConnectionTimeOut,
    ConnectionAborted,
    ConnectionRefused,
    ConnectionDropped,
    SyncConnectionReleased,
    ExitNoticeReceived,
    Read,
    Write,
    UnknownPacket,
    PacketTypeMismatch,
    DuplicateObjectId,
    UnknownObjectId,
    InvalidObjectPacket,
    InvalidCreationPacketType,
    BufferUnderflow,
    BufferOverflow,
    InvalidBufferPosition,
    InvalidBufferLimit,
    OtherGNELevelError,
    OtherLowLevelError,
    User /**< Useful for user-defined classes that inherit from Error */
  };

  /**
   * Create a new object representing a certain error, given by the passed
   * ErrorCode.  If no error code is passed, NoError is assumed.
   */
  Error(ErrorCode ec = NoError);

  virtual ~Error();

  /**
   * Returns the error code for this error.
   */
  ErrorCode getCode() const;

  /**
   * Sets the error code for this error, useful after a call to
   * createLowLevelError() to specify a more appropriate high-level error
   * code.  The Error object still retains the low-level specific error
   * information.
   */
  const void setCode(ErrorCode newCode);

  /**
   * Returns a string description of this error.  When possible, the string
   * is "programmer-friendly," so you will probably want to only use this to
   * write a debug message or out to a log, and tell the user more about the
   * error.
   */
  virtual std::string toString() const;

  /**
   * Returns getCode() != NoError, a handy shortcut so you can test for
   * error by if (error).
   */
  operator bool() const;

  /**
   * A handy shortcut for (error.getCode() == rhs).
   * @param rhs an errorCode to test with.
   */
  bool operator == (const ErrorCode& rhs) const;

private:
  ErrorCode code;
};

} // namespace GNE

/**
 * Sends the result of toString to the ostream.
 */
std::ostream& operator << (std::ostream& o, const GNE::Error& err);

#endif /* ERROR_H_INCLUDED_C4514CDC */
