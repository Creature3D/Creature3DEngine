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
#include "../include/gnelib/EventThread.h"
#include "../include/gnelib/ConnectionListener.h"
#include "../include/gnelib/Connection.h"
#include "../include/gnelib/Thread.h"
#include "../include/gnelib/Timer.h"
#include "../include/gnelib/Time.h"
#include "../include/gnelib/Error.h"
#include "../include/gnelib/ConditionVariable.h"
#include "../include/gnelib/Lock.h"
//#include <CRCore/thread/crScopedLock.h>
namespace GNE {

EventThread::EventThread( const Connection::sptr& conn )
: Thread("EventThr", Thread::DEF_PRI), ourConn(conn),
onReceiveEvent(false), onTimeoutEvent(false),
onDisconnectEvent(false), onExitEvent(false), failure(NULL) {
  gnedbgo(5, "created");
  setType( CONNECTION );
}

EventThread::sptr EventThread::create( const Connection::sptr& conn ) {
  sptr ret( new EventThread( conn ) );
  ret->setThisPointer( ret );
  return ret;
}

EventThread::~EventThread() {
  //we shouldn't have to lock anything since only one thread should ever be here.

  while (!eventQueue.empty()) {
    delete eventQueue.front();
    eventQueue.pop();
  }

  delete failure;

	//for( EventQueue::iterator itr = m_eventQueue.begin();
	//	 itr != m_eventQueue.end();
	//	 ++itr )
	//{
	//	delete (*itr);
	//}
	//m_eventQueue.clear();
	//m_eventSwapMutex.lock();
	//for( EventQueue::iterator itr = m_inEventQueue.begin();
	//	itr != m_inEventQueue.end();
	//	++itr )
	//{
	//	delete (*itr);
	//}
	//m_inEventQueue.clear();
	//m_eventSwapMutex.unlock();

  gnedbgo(5, "destroyed");
}

ConnectionListener::sptr EventThread::getListener() const 
{
  LockCV lock( listenSync );
  //CRCore::ScopedLock<CRCore::crMutex> lock(m_listenMutex);
  return eventListener;
}

void EventThread::setListener( const ConnectionListener::sptr& listener) 
{
  //CRCore::ScopedLock<CRCore::crMutex> lock(m_listenMutex);
  LockCV lock( listenSync );

  ////Acquire eventSync because of the wait loop check
  LockCV lock2( eventSync );
  eventListener = listener;

  ////Signal the event thread in case it is waiting for a listener.
  eventSync.signal();
}

int EventThread::getTimeout() const 
{
  return timeout.getTotalmSec();
}

void EventThread::setTimeout(int ms) 
{
  int microsec;
  if (ms > INT_MAX / 1000)
    microsec = INT_MAX / 1000;
  else
    microsec = ms * 1000;

  {
    LockMutex lock( timeSync );
	//CRCore::ScopedLock<CRCore::crMutex> lock(m_timeSync);
    if (ms != 0) {
      timeout = Time(0, microsec);
      nextTimeout = Timer::getAbsoluteTime() + timeout;
    } else {
      nextTimeout = timeout = Time();
    }
  }

  //Wake up the event thread if it is sleeping, which is needed if there is
  //no timeout currently and the event thread is waiting forever on eventSync.
  eventSync.signal();
  //m_eventCondition.signal();
}

void EventThread::onDisconnect() 
{
  gnedbgo(1, "onDisconnect Event triggered.");
  ////We acquire the mutex to avoid the possiblity of a deadlock between the
  //// test for the shutdown variable and the wait.
  LockCV lock( eventSync );
  onDisconnectEvent = true;
  eventSync.signal();
//	pushEvent(EDisconnect);
}

void EventThread::onExit()
{
  gnedbgo(1, "onExit Event triggered.");
  ////Guarantee that either onExit or onFailure will be called, never both.
  LockCV lock( eventSync );
  if ( !failure && !onDisconnectEvent ) {
    onExitEvent = true;
    eventSync.signal();
  } else {
    gnedbgo(1, "onExit event ignored due to failure or disconnect.");
  }
  //pushEvent(EExit);
}

void EventThread::onFailure(Error* error) 
{
  gnedbgo1(1, "onFailure Event: %s, NLError = %d", error->toString().c_str());
  //Guarantee that either onExit or onFailure will be called, never both.
  LockCV lock( eventSync );
  if ( !onExitEvent && !onDisconnectEvent ) {
    failure = error;//new Error(error);
    eventSync.signal();
  } else {
    gnedbgo(1, "onFailure event ignored due to onExit or disconnect.");
  }
  //pushEvent(EFailure,error);
}

void EventThread::onError(Error* error) {
  gnedbgo1(1, "onError Event: %s", error->toString().c_str());

  LockCV lock( eventSync );
  eventQueue.push(/*new Error(error)*/error);
  eventSync.signal();

  //pushEvent(EError,error);
}

//void EventThread::pushEvent(EventCode eventCode, Error *error)
//{
//	Event *_event = new Event(eventCode);
//	_event->m_error = error;
//
//	m_eventSwapMutex.lock();
//	m_inEventQueue.push_back(_event);
//	m_eventSwapMutex.unlock();
//
//	m_eventCondition.signal();
//}

//void EventThread::run() 
//{
//	//resetTimeout();//add by wch
//	while ( !shutdown && eventListener) 
//	{
//		m_eventSwapMutex.lock();
//		if(!m_inEventQueue.empty()) m_eventQueue.swap(m_inEventQueue);
//
//		if(m_eventQueue.empty())
//		{
//			if ( timeout == Time() ) 
//			{
//				m_eventSwapMutex.unlock();
//				m_eventCondition.wait(&m_eventMutex);
//			} 
//			else 
//			{
//				m_eventSwapMutex.unlock();
//				m_eventCondition.wait(&m_eventMutex,timeout.getTotalmSec());
//			}
//			m_eventSwapMutex.lock();
//			if(!m_inEventQueue.empty()) m_eventQueue.swap(m_inEventQueue);
//		}
//		m_eventSwapMutex.unlock();
//
//		checkForTimeout();
//
//		if(!m_eventQueue.empty())
//		{
//			ConnectionListener::sptr listener = getListener();
//			Event *_event;
//			for( EventQueue::iterator itr = m_eventQueue.begin();
//				itr != m_eventQueue.end();
//				++itr )
//			{
//				_event = *itr;
//				switch (_event->m_code)
//				{
//				case EReceive:
//					//if(!firstPacketReved)
//					//{
//					//	printf("收到第一个包");
//					//    firstPacketReved = true;
//					//}
//					//firstPacketReved = true;
//					//ourConn->onReceivePacket();
//					listener->onReceive( *ourConn );
//					break;
//				case EFailure:
//					listener->onFailure( *ourConn, *(_event->m_error) );
//					ourConn->disconnect();
//					break;
//				case EDisconnect:
//					listener->onDisconnect( *ourConn );
//					shutdown = true;
//					break;
//				case EError:
//					listener->onError( *ourConn, *(_event->m_error) );
//					break;
//				case EExit:
//					listener->onExit( *ourConn );
//					ourConn->disconnect();
//					break;
//				case ETimeout:
//					listener->onTimeout( *ourConn );
//					//ourConn->disconnect();
//					break;
//				}
//
//				delete _event;
//			}
//			m_eventQueue.clear();
//		}
//	}
//}
//

void EventThread::onReceive() 
{
  gnedbgo(4, "onReceive event triggered.");
  //printf("onReceive event triggered.");
  //reset the timeout counter
  resetTimeout();

  LockCV lock( eventSync );
  onReceiveEvent = true;
  eventSync.signal();

  //pushEvent(EReceive);
}

void EventThread::shutDown() 
{
  ////Yep.  No setting of shutdown.  We want to try to close gracefully.  If we
  ////can't do that we couldn't respond to shutdown either.
  ourConn->disconnect();

  LockCV lock( eventSync );
  eventSync.signal();
//	pushEvent(EDisconnect);
}

void EventThread::run() {

  while ( eventListener ) {
    //Yup.  No checking of shutdown.  When shutDown is called we call disconnect
    //on our connection, which should lead to a graceful shutdown.
    LockCVEx eventLock( eventSync );
    //Wait while we have no listener and/or we have no events.
	//modify by wch
    while (/*!eventListener ||*/ (!onReceiveEvent && !failure &&
           !onDisconnectEvent && eventQueue.empty() &&
           !onExitEvent && !onTimeoutEvent) ) {
      //Calculate the time to wait
      if ( timeout == Time() ) {
        //wait "forever"
        eventSync.wait();
      } else {
        //wait until a timeout would occur
        eventSync.timedWait(nextTimeout);
        checkForTimeout();
      }
    }
    eventLock.release();

    checkForTimeout();

    //To prevent deadlocks, we copy our listener, so that we don't need to hold
    //listenSync during the event.
    LockCVEx listenLock( listenSync );
    ConnectionListener::sptr listener = eventListener;
    listenLock.release();

    //Check for events, processing them if events are pending
    if (onExitEvent) {
      listener->onExit( *ourConn );
      ourConn->disconnect();
      onExitEvent = false; //set this after onDisconnectEvent is set
      //we want to reevaluate listener (because of SyncConnection), so we don't
      //directly call onDisconnect here.

    } else if (failure) {
      listener->onFailure( *ourConn, *failure );
      ourConn->disconnect();
      delete failure;
      failure = NULL; //set this after onDisconnectEvent is set

    } else if (onDisconnectEvent) {
      listener->onDisconnect( *ourConn );
      return;  //terminate this thread since there are no other events to
      //process -- onDisconnect HAS to be the last.

    } else if (onReceiveEvent) {
      //This is set to false before in case we get more packets during the
      //onReceive event.
      onReceiveEvent = false;
      listener->onReceive( *ourConn );

    } else if (onTimeoutEvent) {
      onTimeoutEvent = false;
      listener->onTimeout( *ourConn );

    } else {
      LockCVEx lock( eventSync );
      assert(!eventQueue.empty());
      Error* e = eventQueue.front();
      eventQueue.pop();
      lock.release();

      //When we get here this is the only reason left why we were woken up!
      listener->onError( *ourConn, *e );
      delete e;
    }
  }
}

void EventThread::checkForTimeout() {
  LockMutexEx lock( timeSync );
  //CRCore::ScopedLock<CRCore::crMutex> lock(m_timeSync);
  if ( timeout == Time() )
    return;

  Time t = nextTimeout;
  lock.release();
  //m_timeSync.unlock();

  if ( Timer::getAbsoluteTime() > t )
    onTimeout();
}

void EventThread::resetTimeout() {
  LockMutex lock( timeSync );
  //CRCore::ScopedLock<CRCore::crMutex> lock(m_timeSync);

  if ( timeout != Time() ) {
    nextTimeout = Timer::getAbsoluteTime() + timeout;
  }
}

void EventThread::onTimeout() {
  gnedbgo(4, "onTimeout event triggered.");

  //reset the timeout counter
  resetTimeout();

  LockCV lock( eventSync );
  onTimeoutEvent = true;
  eventSync.signal();
  //pushEvent(ETimeout);
}

} //namespace GNE
