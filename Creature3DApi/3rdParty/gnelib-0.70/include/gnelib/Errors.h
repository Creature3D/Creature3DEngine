#ifndef _ERRORS_H_
#define _ERRORS_H_

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

//This file contains various errors derived from Error.
#include "export.h"
#include "Error.h"
#include <nl.h>
#include <string>
#include "gnetypes.h"

namespace GNE {

/**
 * @ingroup midlevel
 *
 * A simple error class with a default ErrorCode of User for quick error
 * messages.  This class is not used by %GNE, but you are welcome to use it to
 * throw your own exceptions, perhaps from Packet::readPacket.
 */
class GNE_EXPORT UserError : public Error {
public:
  UserError( const std::string& msg ) : Error( User ), msg( msg ) {}

  virtual ~UserError() {}

  /**
   * Returns the message set in the constructor.
   */
  virtual std::string toString() const { return msg; }

private:
  std::string msg;
};

/**
 * @ingroup midlevel
 *
 * Adds low level HawkNL and possibly system error information to any other
 * error.
 */
class GNE_EXPORT LowLevelError : public Error {
public:
  /**
   * Creates a normal error, but picks up any error information reported
   * currently by the underlying network library implementation.
   */
  LowLevelError(ErrorCode newCode = OtherLowLevelError);

  virtual ~LowLevelError();

  virtual std::string toString() const;

private:
  /**
   * A possible error code for HawkNL.
   */
  NLint hawkError;

  /**
   * A possible error code for the system given by HawkNL.
   */
  int sysError;
};

/**
 * @ingroup midlevel
 *
 * An error that represents a mismatch in the games during the connection
 * process.  If two GNE programs try to connect that are different games,
 * this is the error you will get.
 */
class GNE_EXPORT WrongGame : public Error {
public:
  WrongGame(std::string GameName);

  virtual ~WrongGame();

  /**
   * Returns the mismatched game name.
   */
  std::string getWrongGame() const;

  virtual std::string toString() const;

private:
  std::string gameName;
};

/**
 * @ingroup midlevel
 *
 * An Error when a Packet is received from the network that has an ID that is
 * not registered with the PacketParser.
 */
class GNE_EXPORT UnknownPacket : public Error {
public:
  UnknownPacket( int type ) : type( type ) {}

  virtual ~UnknownPacket() {}

  int getUnknownType() { return type; }

  virtual std::string toString() const;

private:
  int type;
};

/**
 * @ingroup midlevel
 *
 * An error thrown by SyncConnection when you get a packet other than the one
 * you are trying to receive.
 */
class GNE_EXPORT PacketTypeMismatch : public Error {
public:
  PacketTypeMismatch(int OtherID)
    : Error(Error::PacketTypeMismatch), otherID(OtherID) {}

  virtual ~PacketTypeMismatch() {}

  /**
   * Returns the offending packet's ID.
   */
  int getWrongID() const { return otherID; }

private:
  int otherID;
};

/**
 * @ingroup midlevel
 *
 * An error that occurs during the connecting process if the user versions
 * are different.  The version is passed to you because GNE does not know
 * what format your version is -- only that is is not equal.
 */
class GNE_EXPORT UserVersionMismatch : public Error {
public:
  UserVersionMismatch(guint32 OtherVer)
    : Error(Error::UserVersionMismatch), otherVer(OtherVer) {}

  virtual ~UserVersionMismatch() {}

  /**
   * Returns the offending user version.
   */
  guint32 getWrongVer() const { return otherVer; }

private:
  guint32 otherVer;
};

/**
 * @ingroup midlevel
 *
 * An error that occurs during the connection process if the remote machine
 * sends incorrect or corrupted packets to us.  This might be possible if
 * they were using a pre-alpha version of GNE, or if the connection came from
 * a non-GNE program.
 */
class GNE_EXPORT ProtocolViolation : public Error {
public:
  enum ViolationType {
    OtherViolation = 0,
    InvalidCRP,
    InvalidCAP,
    InvalidUnreliableInfo
  };

  ProtocolViolation(ViolationType T = OtherViolation);

  virtual ~ProtocolViolation();

  ViolationType getViolationType() const;
  void setViolationType(ViolationType T);

  virtual std::string toString() const;

private:
  ViolationType t;
};

/**
 * @ingroup midlevel
 *
 * An error that occurs during an operation on Buffer.
 *
 * @see Buffer
 */
class GNE_EXPORT BufferError : public Error {
public:
  BufferError( ErrorCode code ) : Error( code ) {
  }

  virtual ~BufferError() {}
};

}

#endif
