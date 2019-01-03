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
#include "../include/gnelib/Mutex.h"
#include "../include/gnelib/MutexData.h"
#include "../include/gnelib/Thread.h"

namespace GNE {

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

Mutex::Mutex() {
  data = new MutexData();
#ifdef WIN32
  InitializeCriticalSection(&data->mutex);
#else
  pthread_mutexattr_t attr;
  valassert(pthread_mutexattr_init(&attr), 0);
  valassert(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE), 0);
  valassert(pthread_mutex_init( &data->mutex, &attr ), 0);
  valassert(pthread_mutexattr_destroy(&attr), 0);
#endif

#ifdef _DEBUG
  data->lockCount = 0;
  data->owner = NULL;
  #ifdef WIN32
    InitializeCriticalSection(&data->dbgMutex); 
  #else
    valassert(pthread_mutex_init( &data->dbgMutex, NULL ), 0);
  #endif
#endif
}

Mutex::~Mutex() {
#ifdef WIN32
  DeleteCriticalSection(&data->mutex);
#else
  valassert(pthread_mutex_destroy( &data->mutex ), 0);
#endif

#ifdef _DEBUG
  assert( data->lockCount == 0 );
  assert( data->owner == NULL );
  #ifdef WIN32
    DeleteCriticalSection(&data->dbgMutex);
  #else
    valassert(pthread_mutex_destroy( &data->dbgMutex ), 0);
  #endif
#endif
	delete data;
}

void Mutex::acquire() {
  lowLevelAcquire( &data->mutex );

#ifdef _DEBUG
  markAcquired();
#endif
}

void Mutex::release() {
#ifdef _DEBUG
  markReleased();
#endif

  lowLevelRelease( &data->mutex );
}

#ifdef _DEBUG
void Mutex::markAcquired() {
  lowLevelAcquire( &data->dbgMutex );
  data->lockCount++;
  //assert( data->lockCount != 1 || data->owner == NULL || data->owner == Thread::currentThread() );
  data->owner = Thread::currentThread().get();
  lowLevelRelease( &data->dbgMutex );
}

void Mutex::markReleased() {
  lowLevelAcquire( &data->dbgMutex );
  assert( data->lockCount > 0 );
  assert( data->owner == Thread::currentThread().get() );
  data->lockCount--;
  if ( data->lockCount == 0 )
    data->owner = NULL;
  lowLevelRelease( &data->dbgMutex );
}
#endif

void Mutex::vanillaAcquire() {
  lowLevelAcquire( &data->mutex );
}

void Mutex::vanillaRelease() {
  lowLevelRelease( &data->mutex );
}

}





