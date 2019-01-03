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
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Errors.h"
#include <sstream>

namespace GNE {

/*** LowLevelError Implementation ***/

LowLevelError::LowLevelError(ErrorCode newCode)
: Error(newCode), hawkError(NL_NO_ERROR), sysError(0) {

  NLenum error = nlGetError();
  if (error == NL_SOCKET_ERROR || error == NL_SOCK_DISCONNECT)
    sysError = nlGetSystemError();
  else
    hawkError = error;
}

LowLevelError::~LowLevelError() {
}

std::string LowLevelError::toString() const {
  std::ostringstream ret;
  ret << Error::toString();

  if (hawkError != NL_NO_ERROR && hawkError != NL_SOCKET_ERROR)
    ret << " HawkNL error " << hawkError << ": " << nlGetErrorStr(hawkError);
  if (sysError != 0)
    ret << " System error " << sysError << ": " << nlGetSystemErrorStr(sysError);
  
  return ret.str();
}

/*** WrongGame Implementation ***/

WrongGame::WrongGame(std::string GameName)
: Error(Error::WrongGame), gameName(GameName) {
}

WrongGame::~WrongGame() {
}

std::string WrongGame::getWrongGame() const {
  return gameName;
}

std::string WrongGame::toString() const {
  return Error::toString() + " Their game name is " + gameName + '.';
}

/*** UnknownPacket Implementation ***/

std::string UnknownPacket::toString() const {
  std::ostringstream ret;
  ret << Error::toString() << " Unknown packet ID is " << type << '.';
  return ret.str();
}

/*** ProtocolViolation Implementation ***/
/*
    OtherViolation,
    InvalidCRP,
    InvalidCAP,
    InvalidUnreliableInfo
*/

const std::string ViolationStrings[] = {
  "",
  " Client sent an invalid request packet.",
  " Server sent an invalid connection response packet.",
  " Client sent an invalid or corrupted unreliable information packet."
};

ProtocolViolation::ProtocolViolation(ViolationType T)
: Error(Error::ProtocolViolation), t(T) {
}

ProtocolViolation::~ProtocolViolation() {
}

ProtocolViolation::ViolationType ProtocolViolation::getViolationType() const {
  return t;
}

void ProtocolViolation::setViolationType(ViolationType T) {
  t = T;
}

std::string ProtocolViolation::toString() const {
  return Error::toString() + ViolationStrings[t];

}

}
