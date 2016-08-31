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
#include "../include/gnelib/Timer.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/Lock.h"
#include "../include/gnelib/TimerCallback.h"

namespace GNE {

Timer::Timer(const SmartPtr<TimerCallback>& callback, int rate)
: Thread("Timer", Thread::HIGHER_PRI), callbackRate(rate*1000),
  listener(callback) {

  setType(TIMER);
}

void Timer::stopAll() {
  Thread::requestAllShutdown( Thread::TIMER );
}

Timer::sptr Timer::create(const SmartPtr<TimerCallback>& callback, int rate) {
  sptr ret( new Timer( callback, rate ) );
  ret->setThisPointer( ret );
  return ret;
}

Timer::~Timer() {
}

#ifndef WIN32
//For the gettimeofday function.
#include <sys/time.h>
#endif

Time Timer::getCurrentTime() {
  Time ret;
#ifdef WIN32
  LARGE_INTEGER t, freq;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&t);
  ret.setSec(int(t.QuadPart / freq.QuadPart));
  ret.setuSec(int((t.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart));
#else
  timeval tv;
  gettimeofday(&tv, NULL);
  ret.setSec(tv.tv_sec);
  ret.setuSec(tv.tv_usec);
#endif
  return ret;
}

Time Timer::getAbsoluteTime() {
#ifdef WIN32
  Time ret;
  _timeb t;
  _ftime(&t);
  ret.setSec(t.time);
  ret.setuSec(t.millitm * 1000);
  return ret;
#else
  return getCurrentTime();
#endif
}

TimerCallback::sptr Timer::getCallback() const {
  LockMutex lock( sync );

  return listener;
}

void Timer::startTimer() {
  if (!isRunning()) {
    nextTime = getCurrentTime();
    nextTime += callbackRate;
    start();
  }
}

void Timer::shutDown() {
  stopTimer( false );
}

void Timer::stopTimer(bool waitForEnd) {
  LockMutexEx lock( sync );

  if (shutdown && !waitForEnd)
    return;

  if (isRunning()) {
    Thread::shutDown();
    if (waitForEnd) {
      assert(Thread::currentThread().get() != this);
      if ( Thread::currentThread().get() != this ) {
        lock.release();
        join();
      }
    }
  }
}

void Timer::run() {
  while (!shutdown) {
    Time currTime, sleepTime;
    currTime = getCurrentTime();
    //We add 500 to currTime because we are waiting in milliseconds, so this
    //is essentially a "round."  If we have less than 1000 microseconds left,
    //there is little use in another sleep call which almost certainly won't
    //return faster than this, so we just call it now.
    while (nextTime > currTime + 1000) {
      sleepTime = nextTime - currTime;
      Thread::sleep(sleepTime.getTotaluSec() / 1000);
      currTime = getCurrentTime();
    }
    listener->timerCallback();
    nextTime += callbackRate;
  }

  LockMutex lock( sync );
  listener.reset();
}

}
