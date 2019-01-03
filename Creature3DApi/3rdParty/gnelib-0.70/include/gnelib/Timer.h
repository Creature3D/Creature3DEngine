#ifndef TIMER_H_INCLUDED_C517B9FE
#define TIMER_H_INCLUDED_C517B9FE

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
#include "Thread.h"
#include "Time.h"
#include "Mutex.h"
#include "SmartPtr.h"
#include "WeakPtr.h"

namespace GNE {
class TimerCallback;

/**
 * @ingroup threading
 *
 * The timer class is used to get the current time and to provide callbacks.
 * A timer object calls its listeners back every so often based on the time
 * given.
 *
 * All of the methods in this class are safe to call from multiple threads at
 * the same time, and can also be called from the TimerCallback as well, with
 * a few (some obvious) exceptions.
 *
 * A Timer thread runs with a higher priority than the main thread, therefore
 * its callbacks are suitable for short, quick tasks.
 */
class GNE_EXPORT Timer : public Thread {
protected:
  Timer(const SmartPtr<TimerCallback>& callback, int rate);

public:
  typedef SmartPtr<Timer> sptr;
  typedef WeakPtr<Timer> wptr;

  /**
   * Stops all Timers.
   */
  static void stopAll();

  /**
   * Create a timer callback.  The first call to the callback will occur
   * after "rate" milliseconds, so this class is suitable for setting
   * timeouts for your operations.  Use the startTimer method to start this
   * timer.
   *
   * The callback is released when the Timer is stopped.  This allows the
   * callback to contain a reference to the Timer.
   *
   * @param callback A newly allocated object to perform callbacks on.
   * @param rate the callback rate in milliseconds.
   */
  static sptr create(const SmartPtr<TimerCallback>& callback, int rate);

  /**
   * Destructor.
   */
  virtual ~Timer();

  /**
   * Returns the current time from some arbitray point in the past.  This is
   * usually a very high precision timer that likely provides microsecond
   * or better resolution.
   */
  static Time getCurrentTime();

  /**
   * Returns the current time from the system clock.
   *
   * The time returned is an absolute time, relative to midnight,
   * Jan 1, 1970.
   */
  static Time getAbsoluteTime();

  /**
   * Returns the TimerCallback set in the constructor.  After the timer has
   * stopped, an empty SmartPtr is returned since the listener is released
   * when the timer stops.
   */
  SmartPtr<TimerCallback> getCallback() const;

  /**
   * Starts the timer running and calling the callback.  If the timer has
   * already started, this call will have no effect.  You cannot restart a
   * Timer once you have stopped it, you need to create a new Timer.
   */
  void startTimer();

  /**
   * Equivalent to calling stopTimer with false.
   */
  virtual void shutDown();

  /**
   * Stops the timer and stops calling the callback.  The timer will likely
   * be called one more time because the timer will actually stop at the end
   * of its most recent cycle.  If you want to wait until the callback is
   * called for the last time, pass true into this function.  Then this
   * function will block for at most the time of the callback rate plus the
   * time it takes for the callback to finish.
   *
   * This timer's callback can call this function, but obviously it must not
   * pass true to this function.
   *
   * If a Timer is already stopped, this function will have no effect.
   */
  void stopTimer(bool waitForEnd);

protected:
  /**
   * This is the thread that will perform the callbacks.
   */
  void run();

private:
  /**
   * Next time the callbacks will be activated.
   */
  Time nextTime;

  /**
   * The callback rate in microseconds.  This is different from the accepted
   * parameter in the constructor of milliseconds.
   */
  int callbackRate;

  SmartPtr<TimerCallback> listener;

  /**
   * Provides syncronization for some functions to make them thread safe.
   */
  mutable Mutex sync;

};

}
#endif /* TIMER_H_INCLUDED_C517B9FE */
