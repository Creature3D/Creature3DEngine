/* -*-c++-*- OpenThreads library, Copyright (C) 2002 - 2007  The Open Thread Group
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/
#ifdef WIN32

#include <CRCore/thread/crMutex.h>

namespace CRCore {

class Win32ConditionImpl
{
public:
	/// number of waiters.
	long m_waiters;

	Condition(long max = 0L)
	{
		m_waiters = 0;
		m_sema = CreateSemaphore(NULL,0,0x7fffffff,NULL);
		m_waiters_done = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	~Condition()
	{
	//	CloseHandle(m_sema);
	//	CloseHandle(m_waiters_done);
	}

	inline int broadcast ()
	{
		 m_waiters_lock.lock();
		 int have_waiters = 0;

		if (m_waiters > 0)
		{
	      // We are broadcasting, even if there is just one waiter...
		  // Record the fact that we are broadcasting.  This helps the
		  // wait() method know how to optimize itself.  Be sure to
		  // set this with the <m_waiters_lock> held.
	      m_was_broadcast = 1;
		  have_waiters = 1;
		}
		m_waiters_lock.unlock();
		
		int result = 0;
		if (have_waiters)
	    {
			// Wake up all the waiters.
			ReleaseSemaphore(m_sema,m_waiters,NULL);
			WaitForSingleObject(m_waiters_done,INFINITE) ;
			// This is okay, even without the <m_waiters_lock> held because
			// no other waiter threads can wake up to access it.
			m_was_broadcast = 0;
	    }
		return result;
	}

	inline int wait (Mutex& external_mutex)
	{
		// Prevent race conditions on the <m_waiters> count.
		m_waiters_lock.lock();
		m_waiters++;
		m_waiters_lock.unlock();
		
		int result = 0;
		
        external_mutex.unlock();
		
		DWORD dwResult = WaitForSingleObject(m_sema,INFINITE);
		if(dwResult != WAIT_OBJECT_0)
			result = (int)dwResult;
		
		// Reacquire lock to avoid race conditions on the <m_waiters> count.
		m_waiters_lock.lock();
		
		// We're ready to return, so there's one less waiter.
		m_waiters--;
		
		int last_waiter = m_was_broadcast && m_waiters == 0;
		
		// Release the lock so that other collaborating threads can make
		// progress.
		m_waiters_lock.unlock();
		
		if (result != -1 && last_waiter)
			SetEvent(m_waiters_done);
		
		external_mutex.lock();
		
		return result;
	}


protected:

  /// Serialize access to the waiters count.
  Mutex m_waiters_lock;

  /// Queue up threads waiting for the condition to become signaled.
  HANDLE m_sema;
  /**
   * An auto reset event used by the broadcast/signal thread to wait
   * for the waiting thread(s) to wake up and get a chance at the
   * semaphore.
   */
  HANDLE m_waiters_done;

  /// Keeps track of whether we were broadcasting or just signaling.
  size_t m_was_broadcast;

};
#else
//#include <pthread.h>
//
//namespace CRCore {
//
//class CR_EXPORT crCondition
//{
//public:
//	/// number of waiters.
//	crCondition(long max)
//	{
//        pthread_cond_init( &m_cond, 0L );
//	}
//
//	~crCondition()
//	{
//	}
//
//	inline int broadcast ()
//	{
//		return pthread_cond_broadcast(&m_cond);
//	}
//
//	inline int wait (Mutex& external_mutex)
//	{
//		return pthread_cond_wait(&m_cond);
//	}
//
//
//protected:
//   pthread_cond_t m_cond;
//};
#endif
}
#endif