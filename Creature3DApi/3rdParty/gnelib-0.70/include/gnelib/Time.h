#ifndef TIME_H_INCLUDED_C51412BE
#define TIME_H_INCLUDED_C51412BE

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

//This include file has to be named strangly because of a conflict with the
//standard C include time.h.  Both MSVC and GCC have given us problems with
//including this file, despite the usage between #include <> and #include ""
#include "export.h"
//class ostream;

namespace GNE {

/**
 * @ingroup midlevel
 *
 * A class representing a time relative to a point in the past.  The time
 * is always proper ("normalized"), meaning that 999,999 is the highest value
 * for microseconds.  Setting a value higher than this will increase seconds.
 *
 * To get the current time, check out the methods Timer::getCurrentTime and
 * Timer::getAbsoluteTime.
 */
class GNE_EXPORT Time {
public:
  /**
   * Initalizes this class with a time of 0.
   */
  Time();

  Time( const Time& t );

  /**
   * Initalizes this class with the time specified.  The given time need not
   * be normalized as this will be done automatically.  For example it is
   * acceptable to pass in 0 seconds and 2,000,000 microseconds and it will
   * be normalized to 2 seconds.
   */
  Time(int seconds, int microseconds);

  virtual ~Time();

  /**
   * Returns seconds.
   */
  int getSec() const;

  /**
   * Returns microseconds.
   */
  int getuSec() const;

  /**
   * Returns the total time in microseconds.  This function can be useful
   * for displaying the time represented by this object in a custom format
   * (such as xxxxx ms or xxxxx us, rather than xxx.xxx seconds).  Since an
   * int is returned, +/- 2147.483648 seconds is the maximum amount of time
   * that can be represented in a 32-bit integer in microseconds.
   *
   * Because of this time limitation, only small time differences can be
   * represented.  If a larger time difference is expected, getTotalmSec may
   * be a better choice.
   */
  int getTotaluSec() const;

  /**
   * Returns the total time in milliseconds.  Useful in the same situtations
   * as getTotaluSec, but can represent larger time differences.  A 32-bit
   * integer millisecond value can represent a time of about +/- 24.855 days.
   */
  int getTotalmSec() const;

  /**
   * Sets seconds.
   */
  void setSec(int seconds);

  /**
   * Sets microseconds.
   * @param microseconds the new value for microseconds.  If this value is
   *                     greater that 999,999, it will be normalized and the
   *                     value of seconds will increase.
   */
  void setuSec(int microseconds);

  /**
   * Returns the absolute value of the difference between these two times.
   */
  Time diff(const Time& rhs) const;

  /**
   * Returns a string representation of this time in seconds, for example
   * this might return 5.002052 if there was 5 seconds and 2052 microseconds.
   */
  //std::string toString() const;

  Time& operator = (const Time& rhs);

  /**
   * Equality operator that works as expected.
   */
  bool operator == (const Time& rhs) const;

  /**
   * Inequality operator that works as expected.
   */
  bool operator != (const Time& rhs) const;

  /**
   * Less-than operator that works as expected.
   */
  bool operator<(const Time& rhs) const;

  /**
   * Less-or-equal-than operator that works as expected.
   */
  bool operator <= (const Time& rhs) const;

  /**
   * Greater-than operator that works as expected.
   */
  bool operator>(const Time& rhs) const;

  /**
   * Greater-or-equal-than operator that works as expected.
   */
  bool operator >= (const Time& rhs) const;

  /**
   * Time addition, adding microseconds of time.
   */
  Time operator+(int rhs) const;

  /**
   * Time addition, adding microseconds of time to this object.
   */
  Time& operator+=(int rhs);

  /**
   * Time addition that works as expected to add two times.
   */
  Time& operator+=(const Time& rhs);

  /**
   * Time addition that works as expected.
   */
  Time operator+(const Time& rhs) const;

  /**
   * Time subtraction that works as expected.  You might get negative values
   * out of this.  Sometimes Time::diff will be what you want, instead.
   * @see diff
   */
  Time operator-(const Time& rhs) const;

  /**
   * Scalar multiplication operator.
   */
  Time& operator*=(int rhs);

  /**
   * Scalar multiplication operator.
   */
  Time operator*(int rhs) const;

  /**
   * Scalar division operator.
   */
  Time& operator/=(int rhs);

  /**
   * Scalar division operator.
   */
  Time operator/(int rhs) const;

protected:
  /**
   * make sure that microsec stays under a second, adding to sec if needed.
   */
  void normalize();

  /**
   * Temporarily converts the time values into a different format -- don't
   * know what to call it.  Just look at the formula.  It's needed for things
   * like division of negative numbers.  The method normalize MUST be called
   * before the method completes.
   */
  void convert();

private:
  /**
   * Performs the operation *= for numbers [-2147, 2147].
   */
  void doMultiply( int rhs );

  int sec;

  int microsec;
};

}

/**
 * Sends the result of toString to the ostream.
 */
//std::ostream& operator << (std::ostream& o, const GNE::Time& time);

#endif /* TIME_H_INCLUDED_C51412BE */
