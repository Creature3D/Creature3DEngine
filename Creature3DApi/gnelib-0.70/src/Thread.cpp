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
#include "../include/gnelib/Thread.h"
#include "../include/gnelib/ConditionVariable.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/Timer.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/GNE.h"
#include "../include/gnelib/Lock.h"
#include <time.h>
namespace GNE {

#ifdef WIN32
  typedef DWORD ID;
#else
  typedef pthread_t ID;
#endif

typedef std::map<ID, Thread::sptr> ThreadMap;
typedef ThreadMap::iterator ThreadMapIter;

static ThreadMap threads;
static Mutex mapSync;

class SafeCount {
public:
  SafeCount() : count( 0 ) {};

  void operator ++ () {
    LockMutex lock( mtx );
    ++count;
  }

  void operator -- () {
    LockMutex lock( mtx );
    --count;
  }

  bool operator <= ( const int& rhs ) {
    LockMutex lock( mtx );
    return count <= rhs;
  }

private:
  Mutex mtx;
  int count;
};

//We can't rely on the threads map to tell if threads are alive, because remove
//has to run the destructor after mapSync is released.
static SafeCount liveThreads;

const int Thread::DEF_PRI = 0;
const int Thread::LOW_PRI = -1;
const int Thread::LOWER_PRI = -2;
const int Thread::HIGH_PRI = 1;
const int Thread::HIGHER_PRI = 2;

const std::string Thread::DEF_NAME = "Thread";

struct Thread::ThreadIDData {
  ID thread_id;
#ifdef WIN32
  HANDLE hThread;
  ThreadIDData() : thread_id(0), hThread(0) {}
#else
  ThreadIDData() : thread_id(0) {}
#endif
};

//The thread entry point -- two prototypes, one for Win32, the other for
//POSIX.  The parameter passed in is a pointer to a WeakPtr to the thread.
#ifdef WIN32
unsigned __stdcall Thread::threadStart(void* thread) {
#else
void* Thread::threadStart(void* thread) {
#endif
  Thread::sptr thr = ((Thread::wptr*)thread)->lock();
  //Makes sure the map has been updated before we start.
  //mapSync.acquire();
  //mapSync.release();
  srand(time(0));
  try {
	gnedbg2( 5, "%x: Thread %s Starting", thr.get(), thr->getName().c_str() );
    thr->run();
    thr->end();
  } catch (Error& e) {
    gnedbg2(1, "Unhandled exception, thread terminated. Error %d: %s",
      e.getCode(), e.toString().c_str());
  }
  catch (...) {
	  Error e(Error::User);
	  gnedbg2(1, "Unhandled exception, thread terminated. Error %d: %s",
		  e.getCode(), e.toString().c_str());
  }
  //We don't do a catch all because interestingly enough, the MSVC debugger
  //is started by throwing an exception, and placing a catch all here will
  //keep the debugger from starting.

  gnedbg2( 5, "%x: Thread %s Ending", thr.get(), thr->getName().c_str() );
  ThreadIDData idData = *(thr->id);
  Thread::remove( idData );
  thr.reset();
  //--liveThreads;//delete by wch

  return 0;
}

Thread::Thread(std::string name2, int priority2) : shutdown(false), type(USER),
name(name2), started(false), running(false), joined(false), priority(priority2) {
  id = new ThreadIDData();
  gnedbgo( 5, "Thread created" );
}

Thread::~Thread() {
  assert(!isRunning());

  if ( started && !joined )
    detach();

#ifdef WIN32
  CloseHandle( id->hThread );
#endif
  delete id;

  gnedbgo( 5, "Thread destroyed" );
}

Thread::sptr Thread::currentThread() {
  mapSync.vanillaAcquire();
#ifdef WIN32
  ID id = GetCurrentThreadId();
#else
  ID id = pthread_self();
#endif

  ThreadMapIter iter = threads.find( id );
  if ( iter != threads.end() ) {
    Thread::sptr ret = (*iter).second;
    mapSync.vanillaRelease();
    return ret;
  } else {
    mapSync.vanillaRelease();
    return Thread::sptr();
  }
}

void Thread::sleep(int ms) {
  assert(ms >= 0);
  if (ms > 0) {
#ifdef WIN32
    Sleep(ms);
#else
    ConditionVariable sleeper;
    sleeper.acquire();
    sleeper.timedWait(ms);
    sleeper.release();
#endif
  } else if ( ms == 0 )
    yield();
}

void Thread::yield() {
#ifdef WIN32
  Sleep(0);
#else
  sched_yield();
#endif
}

bool Thread::waitForAllThreads( int ms ) {
  //we can only call this from the main thread.
  assert( !Thread::currentThread() );

  if (ms > INT_MAX / 1000)
    ms = INT_MAX / 1000;

  Time t = Timer::getCurrentTime();
  t += ms * 1000;

  bool ret = false;
  bool timeout = false;
  while (!ret) {
    ret = timeout = (Timer::getCurrentTime() >= t);
    if (!timeout) {
      //Take into accout the CEG thread.
#ifdef OLD_CPP
      //DUMB workaround for GCC 2.9x.  This is insane.
      SmartPtr<GNE::ConnectionEventGenerator> temp = eGen;
      ret = (liveThreads <= ((temp) ? 1 : 0) );
#else
      ret = (liveThreads <= ((eGen) ? 1 : 0 ) );
#endif
    }
    if (!ret)
      sleep(20);
  }

  return timeout;
}

void Thread::requestAllShutdown( ThreadType threadType ) {
  //We need to make a copy, because when mapSync is locked, ALL mutexes are
  //locked because of the debug code.  That and it's unhealthy to leave
  //mapSync locked for long, and this is causing deadlocks due to code in
  //shutdown...

  //Thank GOD we have sptr's now!  Without them I don't think this would be
  //possible because threads may die after we copy them out.
  std::vector< sptr > threadsCopy;

  {
    LockMutex lock( mapSync );

    ThreadMapIter iter = threads.begin();
    for ( ; iter != threads.end(); ++iter ) {
      if ( threadType == ALL || iter->second->type == threadType )
        threadsCopy.push_back( iter->second );
    }
  }

  std::vector< sptr >::iterator iter = threadsCopy.begin();
  for ( ; iter != threadsCopy.end(); ++iter )
    (*iter)->shutDown();
}

std::string Thread::getName() const {
  return name;
}

void Thread::shutDown() {
  shutdown = true;
}

void Thread::join(unsigned int ms) {
  assert( started );
  if ( started ) {
    LockMutex lock( joinSync );
    if ( !joined ) {
#ifdef WIN32
      valassert(WaitForSingleObject( id->hThread, /*INFINITE*/ms ), WAIT_OBJECT_0);
#else
      valassert(pthread_join( id->thread_id, NULL ), 0);
#endif
      joined = true;
    }
  }
}

void Thread::detach() {
#ifndef WIN32
  //We only need to detach on POSIX systems
  valassert(pthread_detach( id->thread_id ), 0);
#endif
}

void Thread::end() {
  running = false;
}

bool Thread::hasStarted() const {
  return started;
}

bool Thread::isRunning() const {
  return running;
}

void Thread::start() {
  assert( !started );

  Thread::sptr this_ = thisThread.lock();
  assert( this_ ); //thisThread must have been set.

  shutdown = false;
  running = true;
  started = true;
  //LockMutex lock( mapSync );

#ifdef WIN32
  id->hThread = (HANDLE)_beginthreadex(
    NULL, 0, &threadStart, (void*)&thisThread, 0,
    reinterpret_cast<unsigned*>(&id->thread_id) );

  assert( id->hThread != 0 );

  //Set the thread priority
  switch (priority) {
    case LOWER_PRI:
      SetThreadPriority(id->hThread, THREAD_PRIORITY_LOWEST);
      break;
    case LOW_PRI:
      SetThreadPriority(id->hThread, THREAD_PRIORITY_BELOW_NORMAL);
      break;
    case HIGH_PRI:
	  //SetThreadPriority(id->hThread, THREAD_PRIORITY_NORMAL);
      SetThreadPriority(id->hThread, THREAD_PRIORITY_ABOVE_NORMAL);
      break;
    case HIGHER_PRI:
      SetThreadPriority(id->hThread, THREAD_PRIORITY_HIGHEST);
      break;
  };
#else
  pthread_create( &id->thread_id, NULL, Thread::threadStart, (void*)&thisThread );
#endif

  //Important note: because after start is called the caller can lose the
  //reference at any time, the sptr we store here may be the only sptr ref to
  //this object.
  mapSync.acquire();
  threads[ id->thread_id ] = this_;
  ++liveThreads;
  mapSync.release();

  gnedbgo1( 5, "Starting Thread %s", name.c_str() );
}

int Thread::getPriority() const {
  return priority;
}

void Thread::setThisPointer( const wptr& thisPtr ) {
  thisThread = thisPtr;
}

Thread::sptr Thread::getThisPointer() const {
  return thisThread.lock();
}

void Thread::setType( ThreadType newType ) {
  type = newType;
}

void Thread::remove( const ThreadIDData& d ) {
  LockMutex lock( mapSync );
  //mapSync.vanillaAcquire();
  //We need to delay the possible destructor until AFTER erase finishes,
  //because the destructor can invoke currentThread if it runs a debug output
  //statement, and Bad Things happen since currentThread finds an element that
  //doesn't really exist.
  sptr temp;
  ThreadMapIter iter = threads.find( d.thread_id);
  assert( iter != threads.end() );
  temp = iter->second;
  threads.erase(d.thread_id);
  --liveThreads;//add by wch
  //mapSync.vanillaRelease();

  //we let temp die after the mutex release, to prevent deadlocks from anything
  //strange that might go on in that dtor.  We REALLY want to execute that dtor
  //while in mapSync but we can get unexpected deadlocks.
}

}
