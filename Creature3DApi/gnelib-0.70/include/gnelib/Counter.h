#ifndef COUNTER_H_INCLUDED_C517BD95
#define COUNTER_H_INCLUDED_C517BD95

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
#include "TimerCallback.h"
#include "ConditionVariable.h"

namespace GNE {

/**
 * @ingroup threading
 *
 * An implementation of a TimerCallback that provides a simple counter.  All
 * of the functions in this class are thread-safe so that multiple threads
 * can be accessing the Counter at the same time.
 */
class GNE_EXPORT Counter : public TimerCallback {
protected:
  Counter();

public:
  typedef SmartPtr<Counter> sptr;
  typedef WeakPtr<Counter> wptr;

  /**
   * Create a Counter.  Pass this Counter into a Timer class to use it.
   * @see TimerCallback
   */
  static sptr create();

  /**
   * Destructor.
   */
  virtual ~Counter();

  /**
   * The callback function that the Timer calls.  You should not call this
   * function directly.
   */
  void timerCallback();

  /**
   * Returns the current count of this Counter.
   */
  int getCount();

  /**
   * Sets the current count to newCount.
   * @param newCount the new value for the Counter
   */
  void setCount(int newCount);

  /**
   * Adjusts the counter's value by adding val to the current count.  This
   * means that negative numbers will subtract from the current count.
   */
  void adjustCount(int val);

  /**
   * This method blocks until the count is greater than zero.  If the count
   * is already greater than zero it returns immediately.  In either case, the
   * value of the counter is decremented.
   */
  void waitAndDecrement();

private:
  int count;

  ConditionVariable countSync;

};

}
#endif /* COUNTER_H_INCLUDED_C517BD95 */
