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

#ifdef WIN32
//We have a special version of the threading API for Win32
#include "ConditionVariableWin32.inc"
#else

#include "../include/gnelib/ConditionVariable.h"
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/MutexData.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/Timer.h"

namespace GNE {

struct ConditionVariable::ConditionVariableData {
  pthread_cond_t cond;
};

ConditionVariable::ConditionVariable() {
  data = new ConditionVariableData();
  valassert(pthread_cond_init( &data->cond, NULL ), 0);
  ourMutex = true;
  mutex = new Mutex();
}

ConditionVariable::ConditionVariable(Mutex* m) {
  data = new ConditionVariableData();
  valassert(pthread_cond_init( &data->cond, NULL ), 0);
  mutex = m;
  ourMutex = false;
}

ConditionVariable::~ConditionVariable() {
  valassert(pthread_cond_destroy( &data->cond ), 0);
  if (ourMutex)
    delete mutex;
  delete data;
}

void ConditionVariable::acquire() {
  mutex->acquire();
}

void ConditionVariable::release() {
  mutex->release();
}

void ConditionVariable::wait() {
#ifdef _DEBUG
  mutex->markReleased();
#endif
  valassert(pthread_cond_wait(&data->cond, &mutex->data->mutex), 0);
#ifdef _DEBUG
  mutex->markAcquired();
#endif
}

void ConditionVariable::timedWait(int ms) {
  Time t = Timer::getAbsoluteTime();
  t += ms*1000;
  timedWait(t);
}

void ConditionVariable::timedWait(const Time& until) {
  timespec tv;
  tv.tv_sec = until.getSec();
  tv.tv_nsec = until.getuSec() * 1000;
  pthread_cond_timedwait(&data->cond, &(mutex->data->mutex), &tv);
}

void ConditionVariable::signal() {
  valassert(pthread_cond_signal( &data->cond ), 0);
}

void ConditionVariable::broadcast() {
  valassert(pthread_cond_broadcast( &data->cond ), 0);
}

}
#endif
