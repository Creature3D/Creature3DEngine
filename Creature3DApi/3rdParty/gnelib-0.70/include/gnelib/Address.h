#ifndef ADDRESS_H_INCLUDED_C446A6BC
#define ADDRESS_H_INCLUDED_C446A6BC

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
#include <nl.h>
#include <string>
class ostream;

namespace GNE {

/**
 * @ingroup midlevel
 *
 * A wrapper class for the HawkNL NLaddress.  This class is used many times
 * around GNE and is made for ease of use and cleaner code.
 */
class GNE_EXPORT Address {
public:
  /**
   * Constructs an invalid address.
   */
  Address();

  /**
   * Creates a new Address, resolving the name in address if necessary.  The
   * format is the same as HawkNL's nlGetAddrFromName, for IP this format is
   * xxx.xxx.xxx.xxx:ppppp where ppppp is the optional port and
   * xxx.xxx.xxx.xxx can be a hostname, i.e.:<br>
   * localhost:1678<br>
   * 129.21.138.180:5555<br>
   * www.mozilla.com:80<br>
   * www.hawksoft.com  (when port is empty, resulting port will be 0).
   *
   * If no string is supplied, the default is 0.0.0.0:0.  Note that the name
   * resolution may take some time and this constructor will block.  If the
   * resolution failed, then the Address is not valid (!isValid()).
   *
   * @see isValid()
   */
  Address(std::string address);

  /**
   * Alternate version for better implicit conversion
   */
  Address(const char* address);

  /**
   * Creates a new Address with the HawkNL NLaddress.
   */
  Address(NLaddress address);

  ~Address();

  /**
   * Does the same work as the constructor.
   */
  void setAddressByName(const std::string& address);

  /**
   * Returns the reverse DNS lookup based on this address.  This method will
   * block.  If the name doesn't exist, the IP address will be returned.
   * The port from this address will not be part of the name in either case.
   */
  std::string getNameByAddress() const;

  /** 
   * Returns a copy of the actual HawkNL address struct.
   */
  NLaddress getAddress() const;

  /**
   * Sets the address based on the given HawkNL address.
   */
  void setAddress(NLaddress address);

  /**
   * Returns a string representation of this address in the format
   * xxx.xxx.xxx.xxx:ppppp, but if the port is 0, the colon and port number
   * are empty.
   */
  std::string toString() const;

  /**
   * Returns the port for this address.
   */
  int getPort() const;

  /**
   * Sets the port for this address.  This is a number 0..65535.
   */
  void setPort(int port);

  /**
   * Returns if this address is in a valid state.  And address will become
   * invalid if a name could not be resolved, or a port sent was out of range.
   */
  bool isValid() const;

  /**
   * Another method for testing isValid().
   * @see isValid()
   */
  operator bool() const;

  /**
   * Tests if two addresses are equal based on the HawkNL function
   * nlCompareAddr.  This does not take validity into consideration.
   */
  bool operator ==(const Address& rhs) const;

private:
  NLaddress addr;

};

} // namespace GNE

/**
 * Sends the result of toString to the ostream.
 */
std::ostream& operator << (std::ostream& o, const GNE::Address& addr);

#endif /* ADDRESS_H_INCLUDED_C446A6BC */
