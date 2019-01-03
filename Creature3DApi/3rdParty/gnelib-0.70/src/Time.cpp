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
#include "../include/gnelib/Time.h"
//#include <iostream>
//#include <iomanip>
//#include <sstream>

//std::ostream& operator << (std::ostream& o, const GNE::Time& time) {
//  return o << time.toString();
//}

namespace GNE {

Time::Time() : sec(0), microsec(0) {
}

Time::Time( const Time& t ) : sec(t.sec), microsec(t.microsec) {
}

Time::Time(int seconds, int microseconds) 
: sec(seconds), microsec(microseconds) {
  normalize();
}

Time::~Time() {
}

int Time::getSec() const {
  return sec;
}

int Time::getuSec() const {
  return microsec;
}

int Time::getTotaluSec() const {
  return (sec * 1000000 + microsec);
}

int Time::getTotalmSec() const {
  return ( (sec * 1000) + (microsec / 1000) );
}

void Time::setSec(int seconds) {
  sec = seconds;
}

void Time::setuSec(int microseconds) {
  microsec = microseconds;
  normalize();
}

Time Time::diff(const Time& rhs) const {
  Time ret = operator-(rhs);
  ret.sec = labs(ret.sec);
  return ret;
}

//std::string Time::toString() const {
//  std::stringstream ret;
//  if ( sec >= 0 )
//    ret << sec << '.' << std::setfill('0') << std::setw(6) << microsec;
//  else {
//    ret << '-' << abs(sec+1) << '.' << std::setfill('0') << std::setw(6)
//      << 1000000 - microsec;
//  }
//  return ret.str();
//}

Time& Time::operator = (const Time& rhs) {
  sec = rhs.sec;
  microsec = rhs.microsec;
  return *this;
}

bool Time::operator ==(const Time& rhs) const {
  return (microsec == rhs.microsec && sec == rhs.sec);
}

bool Time::operator !=(const Time& rhs) const {
  return (microsec != rhs.microsec || sec != rhs.microsec);
}

bool Time::operator < (const Time& rhs) const {
  return (sec < rhs.sec || ((sec == rhs.sec) && (microsec < rhs.microsec)));
}

bool Time::operator <= (const Time& rhs) const {
  return operator < (rhs) || operator == (rhs);
}

bool Time::operator > (const Time& rhs) const {
  return (sec > rhs.sec || ((sec == rhs.sec) && (microsec > rhs.microsec)));
}

bool Time::operator >= (const Time& rhs) const {
  return operator > (rhs) || operator == (rhs);
}

Time Time::operator+(int rhs) const {
  //written this way to avoid overflow.
  Time ret( *this );
  ret += rhs;
  return ret;
}

Time& Time::operator+=(int rhs) {
  //We have to deal with overflows.  This is a little strange, but it lets me
  //reuse the code in normalize.
  int old_microsec = microsec;
  microsec = rhs;
  normalize();
  microsec += old_microsec;
  normalize();
  return *this;
}

Time& Time::operator+=(const Time& rhs) {
  //can't have overflow here
  microsec += rhs.microsec;
  sec += rhs.sec;
  normalize();
  return *this;
}

Time Time::operator+(const Time& rhs) const {
  return Time(sec + rhs.sec, microsec + rhs.microsec);
}

Time Time::operator-(const Time& rhs) const {
  return Time(sec - rhs.sec, microsec - rhs.microsec);
}

void Time::doMultiply( int rhs ) {
  convert();

  microsec *= rhs;
  sec *= rhs;
  normalize();
}

Time& Time::operator*=(int rhs) {
  if ( rhs < -2147 ) {
    doMultiply( -2147 );
    rhs /= -2147;
  }
  while ( rhs > 2147 ) {
    doMultiply( 2147 );
    rhs /= 2147;
  }
  doMultiply( rhs );

  return *this;
}

Time Time::operator*(int rhs) const {
  Time ret( sec, microsec );
  ret *= rhs;
  return ret;
}

Time& Time::operator/=(int rhs) {
  convert();
  sec /= rhs;
  microsec /= rhs;
  normalize();
  return *this;
}

Time Time::operator/(int rhs) const {
  Time ret( sec, microsec );
  ret /= rhs;
  return ret;
}

void Time::normalize() {
  if (microsec > 999999) {
    sec += (microsec / 1000000);
    microsec = microsec % 1000000;
  } else if (microsec < 0) {
    sec += microsec / 1000000 - 1;
    microsec *= -1;
    microsec = 1000000 - (microsec % 1000000);
  }
}

void Time::convert() {
  if ( sec < 0 ) {
    ++sec;
    microsec -= 1000000;
  }
}

}
