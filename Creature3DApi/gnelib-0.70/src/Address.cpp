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
#include "../include/gnelib/Address.h"
#include <iostream>

std::ostream& operator << (std::ostream& o, const GNE::Address& addr) {
  return o << addr.toString();
}

namespace GNE {

Address::Address() {
  addr.valid = NL_FALSE;
}

Address::Address(std::string address) {
  setAddressByName(address);
}

Address::Address(const char* address) {
  setAddressByName(address);
}

Address::Address(NLaddress address) : addr(address) {
}

Address::~Address() {
}

void Address::setAddressByName(const std::string& address) {
  nlGetAddrFromName((const NLchar*)address.c_str(), &addr);
}

std::string Address::getNameByAddress() const {
  if (isValid()) {
    char buf[NL_MAX_STRING_LENGTH];
    nlGetNameFromAddr(&const_cast<Address*>(this)->addr, (NLbyte*)buf);
    return std::string(buf);

  } else {
    return "invalid address";
  }
}

NLaddress Address::getAddress() const {
  return addr;
}

void Address::setAddress(NLaddress address) {
  addr = address;
}

//The totally lame const_casts are due to HawkNL's lack of const-correctness.
std::string Address::toString() const {
  if (isValid()) {
    char buf[NL_MAX_STRING_LENGTH];
    nlAddrToString(&const_cast<Address*>(this)->addr, (NLbyte*)buf);
    return std::string(buf);
  } else
    return "invalid address";
}

int Address::getPort() const {
  return (int)nlGetPortFromAddr(&const_cast<Address*>(this)->addr);
}

void Address::setPort(int port) {
  if (port < 0 || port > 65535)
    addr.valid = NL_FALSE;
  else
    nlSetAddrPort(&const_cast<Address*>(this)->addr, (NLushort)port);
}

bool Address::isValid() const {
  return (addr.valid == NL_TRUE);
}

Address::operator bool() const {
  return isValid();
}

bool Address::operator ==(const Address& rhs) const {
  return (nlAddrCompare(&const_cast<Address*>(this)->addr, &const_cast<Address&>(rhs).addr) == NL_TRUE);
}

} //namespace GNE




