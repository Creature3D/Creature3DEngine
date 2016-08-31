#ifndef CONDITIONVARIABLE_H_INCLUDED_C51DFF03
#define CONDITIONVARIABLE_H_INCLUDED_C51DFF03

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
namespace GNE {
class Mutex;
class Time;

/**
 * @ingroup threading
 *
 * A class for a conditional variable.  This is to be used when a thread needs
 * to wait for a condition that another thread will trigger.  The most common
 * usage of ConditionVariables:
 *
 * <pre>
 * cv.lock();
 * while (!condition) {
 *   cv.wait();
 * }
 * //do stuff
 * cv.unlock();
 * </pre>
 *
 * When cv.wait is called, the thread atomically releases the associated
 * mutex and waits for another thread to call signal or broadcast on the cv
 * object.  When this thread reawakens, it waits to lock the associated mutex
 * then exits the wait call.
 */
class GNE_EXPORT ConditionVariable {
public:
  /**
   * Initalizes this class creating a new mutex.
   */
  ConditionVariable();

  /**
   * Initalizes this class, with another mutex that the caller is
   * responsible for allocating and destroying.
   * @param m the mutex to be associated with this conditional variable.
   *          this mutex will not be deallocated.
   */
  ConditionVariable(Mutex* m);

  virtual ~ConditionVariable();

  /**
   * Locks the associated mutex for this condition variable.
   * @see Mutex#acquire()
   */
  void acquire();

  /**
   * Releases the associated mutex.
   * @see Mutex#release()
   */
  void release();

  /**
   * This method will block until the thread is woken up by a call to
   * signal or broadcast.  The associated mutex must be locked when wait()
   * is called.  wait() will unlock the mutex and block until woken up at
   * which point it will re-acquire the mutex.
   */
  void wait();

  /**
   * This method works identical to wait, except the thread will wake up
   * anyways if not woken up before the timeout.  The mutex will still be
   * reacquired.
   * @param timeout the time to wait in milliseconds
   */
  void timedWait(int ms);

  /**
   * Waits until the given time, specified in absolute time.
   */
  void timedWait(const Time& until);

  /**
   * Wakes up at least one thread waiting on this CV.  Which thread wakes up
   * is implementation dependant.
   */
  void signal();

  /**
   * Wakes up all threads waiting on this CV.
   */
  void broadcast();

private:
  struct ConditionVariableData;
  ConditionVariableData* data;

#ifdef WIN32
  //The Windows implementation of condition variables comes from the Boost
  //C++ library.
  void initBoostCode();
  void enter_wait();
#endif

  /**
   * is the mutex pointer we have our mutex we should delete?
   */
  bool ourMutex;

  /**
   * The mutex associated with this Conditional Variable
   */
  Mutex* mutex;
};

}
#endif /* CONDITIONVARIABLE_H_INCLUDED_C51DFF03 */
