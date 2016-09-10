/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
*/
#include  <CRCore/thread/crCriticalMutex.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <sys/timeb.h>
#include <windows.h>
#include <process.h>
#endif
#ifndef WIN32
#include <pthread.h>
#include <sched.h>
#endif

#ifdef WIN32
#define MUTEX_TYPE CRITICAL_SECTION
#define MUTEX_PTR LPCRITICAL_SECTION
#else
#define MUTEX_TYPE pthread_mutex_t
#define MUTEX_PTR pthread_mutex_t*
#endif

namespace CRCore {
struct crCriticalData {
	MUTEX_TYPE mutex;
};
}
using namespace CRCore;

static inline void lowLevelAcquire( MUTEX_PTR t ) {
#ifdef WIN32
  EnterCriticalSection(t);
#else
  valassert(pthread_mutex_lock( t ), 0);
#endif
}

static inline void lowLevelRelease( MUTEX_PTR t ) {
#ifdef WIN32
  LeaveCriticalSection(t);
#else
  valassert(pthread_mutex_unlock( t ), 0);
#endif
}

crCriticalMutex::crCriticalMutex() {
  data = new crCriticalData();
#ifdef WIN32
  InitializeCriticalSection(&data->mutex);
#else
  pthread_mutexattr_t attr;
  valassert(pthread_mutexattr_init(&attr), 0);
  valassert(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE), 0);
  valassert(pthread_mutex_init( &data->mutex, &attr ), 0);
  valassert(pthread_mutexattr_destroy(&attr), 0);
#endif
}

crCriticalMutex::~crCriticalMutex() {
#ifdef WIN32
  DeleteCriticalSection(&data->mutex);
#else
  valassert(pthread_mutex_destroy( &data->mutex ), 0);
#endif
	delete data;
}

void crCriticalMutex::acquire() {
  lowLevelAcquire( &data->mutex );
}

void crCriticalMutex::release() {
  lowLevelRelease( &data->mutex );
}

void crCriticalMutex::vanillaAcquire() {
  lowLevelAcquire( &data->mutex );
}

void crCriticalMutex::vanillaRelease() {
  lowLevelRelease( &data->mutex );
}





