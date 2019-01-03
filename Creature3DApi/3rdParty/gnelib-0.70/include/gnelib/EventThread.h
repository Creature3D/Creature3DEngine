#ifndef _EVENTTHREAD_H_
#define _EVENTTHREAD_H_

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
#include "Error.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include "Time.h"
#include "SmartPtr.h"
#include "WeakPtr.h"
//#include <vector>
//#include <CRCore/thread/crCondition.h>
//#include <CRCore/thread/crMutex.h>
namespace GNE {
class ConnectionListener;
class Connection;

/**
 * @ingroup internal
 *
 * Internal class used by GNE to manage Connection events.
 *
 * Each Connection has an EventThread.  This is used internally by the
 * Connection class to act as a proxy to dispatch events to the
 * ConnectionListener.  Because of this, only one event per Connection will
 * be active at any one time.  Another appropriate name that might describe
 * this would be an EventQueue.  But to improve efficency and stability,
 * some events may be reordered or combined.  Remember that onReceive means 1
 * or more packets have been received.  If multiple onReceive or
 * onDoneWriting occur, they will probably be combined.  Error events will
 * always have first priority, and if the error leads to a disconnect,
 * pending events after that will not be called (except for onDisconnect).
 *
 * EventThread was created to solve several things:
 * <ul>
 * <li>Complexities due to multiple threads calling the ConnectionListener at
 *   the same time.</li>
 * <li>Serialized events and all events executing in the same thread is easy
 *   to control and eliminates lots of previous ways for syncronization errors
 *   to creep in.</li>
 * <li>Events taking a while to execute or block (although this is an error),
 *   won't stop the rest of GNE from functioning, but will only stop a single
 *   Connection.</li>
 * <li>Multiple event threads take better advantage of multiprocessor
 *   machines.</li>
 * </ul>
 */
class GNE_EXPORT EventThread : public Thread {
protected:
  /**
   * @see create
   */
  EventThread( const SmartPtr<Connection>& conn );

public:
  typedef SmartPtr<EventThread> sptr;
  typedef WeakPtr<EventThread> wptr;

  /**
   * Initializes this class as a event thread for a listener.
   * The conn pointer is used to call disconnect when an onFailure
   * event is finally processed.  This is to assure that disconnect is called
   * from a safe thread that won't lead to deadlock when a failure occurs.
   *
   * The SmartPtr also keeps the Connection alive if it is still active.
   */
  static sptr create( const SmartPtr<Connection>& conn );

  virtual ~EventThread();

  /**
   * Use GNE::Connection::getListener.
   */
  SmartPtr<ConnectionListener> getListener() const;
  
  /**
   * Use GNE::Connection::setListener.
   */
  void setListener( const SmartPtr<ConnectionListener>& listener );

  /**
   * Use GNE::Connection::getTimeout.
   */
  int getTimeout() const;

  /**
   * Use GNE::Connection::setTimeout.
   */
  void setTimeout(int ms);

  /**
   * For more information about these events, see ConnectionListener.  The
   * processing of an onDisconnect event will be the last, and the thread
   * will essentially stop when onDisconnect completes.
   */
  void onDisconnect();

  /**
   * For more information about these events, see ConnectionListener.
   */
  void onExit();

  /**
   * For more information about these events, see ConnectionListener.
   */
  void onFailure(Error* error);

  /**
   * For more information about these events, see ConnectionListener.
   */
  void onError(Error* error);

  /**
   * For more information about these events, see ConnectionListener.
   */
  void onReceive();

  /**
   * Overrides Thread::shutDown so that the daemon thread will
   * be woken up since it might be waiting on a ConditionVariable.  Once it
   * shuts down it should not be activated again.
   */
  void shutDown();

  //enum EventCode
  //{
	 // EReceive,
	 // EFailure,
	 // EDisconnect,
	 // EError,
	 // EExit,
	 // ETimeout
  //};
  //struct Event
  //{
	 // Event(EventCode code):m_code(code),m_error(NULL){}
	 // ~Event(){ if(m_error) delete m_error; }
  //    EventCode m_code;
  //    Error *m_error;
  //};
  //typedef std::vector<Event *> EventQueue;
  //void pushEvent(EventCode eventCode, Error *error = NULL);
protected:
  /**
   * This thread serializes events for a Connection.
   */
  void run();

private:
  /**
   * Checks for timeout, triggering an onTimeout event and handling the time
   * variables, if needed.
   */
  void checkForTimeout();

  /**
   * Resets the timeout counter by making nextTimeout be the current absolute
   * time + the timeout interval.
   */
  void resetTimeout();

  void onTimeout();

  //See the ctor for more information about ourConn.
  SmartPtr<Connection> ourConn;

  //The listener for our events.  All on* events go here.  This is protected
  //so ClientConnection can send events as well.
  SmartPtr<ConnectionListener> eventListener;

  ConditionVariable eventSync;
  //CRCore::crCondition m_eventCondition;
  //CRCore::crMutex m_eventMutex;

  //mutable CRCore::crMutex m_listenMutex;
  mutable ConditionVariable listenSync;

  //Variables for handling onTimeout events.
  //CRCore::crMutex m_timeSync;
  Mutex timeSync;
  Time timeout;
  Time nextTimeout;

  volatile bool onReceiveEvent;
  volatile bool onTimeoutEvent;

  ////If this is true, we should not receive any more events.  It should be the
  ////next event called, and everything else should stop.
  volatile bool onDisconnectEvent;

  ////If this is true, an graceful exit notice was received, and the next event
  ////is an onDisconnectEvent.
  volatile bool onExitEvent;

  ////Wish we could combine error events and report the last error, but this
  ////really isn't what the API specifies, so we should keep track of every
  ////error.
  std::queue<Error*> eventQueue;

  ////If this is true, we have a onFailure event which takes precedence over
  ////everything.
  Error* failure;

  //EventQueue m_inEventQueue;
  //EventQueue m_eventQueue;
  //CRCore::crMutex m_eventSwapMutex;
};

}
#endif /* _EVENTTHREAD_H_ */
