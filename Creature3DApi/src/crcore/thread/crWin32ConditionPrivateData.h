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
// WIN32ConditionPrivateData.h - Private data structure for crCondition
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
#ifndef CRCORE_CRWIN32CONDITIONPRIVATEDATA_H
#define CRCORE_CRWIN32CONDITIONPRIVATEDATA_H

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <CRCore/thread/crScopedLock.h>

#include <CRCore/thread/crWin32ThreadPrivateData.h>
#include <CRCore/thread/crHandleHolder.h>

#define InterlockedGet(x) InterlockedExchangeAdd(x,0)

namespace CRCore {

class crCondition;

class Win32ConditionPrivateData 
{
public:
    friend class crCondition;
    /// number of waiters.
    long m_waiters;

    Win32ConditionPrivateData ()
        :m_waiters(0), 
         m_sema(CreateSemaphore(NULL,0,0x7fffffff,NULL)),
         m_waiters_done(CreateEvent(NULL,FALSE,FALSE,NULL))
    {
    }

    ~Win32ConditionPrivateData ();
	inline bool haswaiters()
	{
		return InterlockedGet(&m_waiters)>0;
	}
    inline int broadcast ()
    {
        int have_waiters = 0;
        long w = InterlockedGet(&m_waiters);

        if (w > 0)
        {
          // we are broadcasting.  
          m_was_broadcast = 1;
          have_waiters = 1;
        }

        int result = 0;
        if (have_waiters)
        {
            // Wake up all the waiters.
            ReleaseSemaphore(m_sema.get(),m_waiters,NULL);

			cooperativeWait(m_waiters_done.get(), INFINITE);

            //end of broadcasting
            m_was_broadcast = 0;
        }
        return result;
    }

    inline int signal()
    {
        long w = InterlockedGet(&m_waiters);
        int have_waiters = w > 0;
 
        int result = 0;

        if (have_waiters)
        {
            if( !ReleaseSemaphore(m_sema.get(),1,NULL) )
                result = -1;
        }
        return result;
    }

    inline int wait (crMutex& external_mutex, long timeout_ms)
    {
    
        // Prevent race conditions on the <m_waiters> count.
        InterlockedIncrement(&m_waiters);

        int result = 0;

        ReverseScopedLock<crMutex> lock(external_mutex);

        // wait in timeslices, giving testCancel() a change to
        // exit the thread if requested.
		try {
			DWORD dwResult = 	cooperativeWait(m_sema.get(), timeout_ms);
		    if(dwResult != WAIT_OBJECT_0)
				result = (int)dwResult;
		}
		catch(...){
			// thread is canceled in cooperative wait , do cleanup
		    InterlockedDecrement(&m_waiters);
			long w = InterlockedGet(&m_waiters);
			int last_waiter = m_was_broadcast && w == 0;

			if (last_waiter)  SetEvent(m_waiters_done.get());
			// rethrow
			throw;
		}

		
        // We're ready to return, so there's one less waiter.
        InterlockedDecrement(&m_waiters);
        long w = InterlockedGet(&m_waiters);
        int last_waiter = m_was_broadcast && w == 0;

        if (result != -1 && last_waiter)
            SetEvent(m_waiters_done.get());

        return result;
    }

protected:

  /// Serialize access to the waiters count.
  /// crMutex waiters_lock_;
  /// Queue up threads waiting for the condition to become signaled.
  HandleHolder m_sema;
  /**
   * An auto reset event used by the broadcast/signal thread to wait
   * for the waiting thread(s) to wake up and get a chance at the
   * semaphore.
   */
  HandleHolder m_waiters_done;
  /// Keeps track of whether we were broadcasting or just signaling.
  size_t m_was_broadcast;
};

#undef InterlockedGet

}

#endif 



