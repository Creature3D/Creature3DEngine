#ifndef THREAD_H_INCLUDED_C51E3746
#define THREAD_H_INCLUDED_C51E3746

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
#include "Mutex.h"
#include "SmartPtr.h"
#include "WeakPtr.h"
#include <string>
#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif
namespace GNE {
/**
 * @ingroup threading
 *
 * A class resembling a thread.
 * Derive your own classes from this class that implements the run function.
 * Execution begins in the run method.  All methods of this class are thread-
 * safe.
 *
 * \todo Priorities are not yet implemented.
 */
class GNE_EXPORT Thread {
public: //typedefs
  typedef SmartPtr<Thread> sptr;
  typedef WeakPtr<Thread> wptr;

public:
  /**
   * An enum for specifing the thread type.  You should never need to use this
   * directly, since anything you create should have type USER.
   */
  enum ThreadType {
    USER,      /**< this thread is a user-created thread. */
    TIMER,     /**< this thread is a Timer thread. */
    SYSTEM,    /**< this thread is a %GNE-created thread. */
    CONNECTION,/**< this thread is for a Connection. */
    ALL        /**< used only as a parameter to some functions. */
  };

public:
  /**
   * Creates a new thread, ready to run but not yet running.
   * @param name the name of this thread
   * @param priority the priority this thread has
   */
  Thread(std::string name = DEF_NAME, int priority = DEF_PRI);

  virtual ~Thread();

  /**
   * Returns the Thread object that represents this calling thread.  This will
   * return a SmartPtr to the calling thread, or an empty SmartPtr if the
   * calling thread is the main thread or another thread created outside of
   * the %GNE API.
   */
  static sptr currentThread();

  /**
   * The currently running thread sleeps for the time given in milliseconds.
   */
  static void sleep(int ms);

  /**
   * Makes a request to the operating system to give up the remainder of this
   * thread's timeslice to another appropriate thread.  This function does
   * nothing if the operating system does not support the call.  Note that
   * even if the OS supports the call, it may choose not to end your
   * timeslice.
   *
   * There are certain few cases where yielding may improve performance or
   * response, but most of the time you are likely wanting to use a
   * ConditionVariable to wait for an event or a Timer.  You should never use
   * yielding as mitigation for a busy-wait, because you will still use 100%
   * CPU and essentially you will perform a busy-wait and just effectively
   * lower the scheduling priority of your thread/process.
   */
  static void yield();

  /**
   * This method will wait for all threads that have a type != SYSTEM.  This
   * includes threads from connections and threads from timers, so if you have
   * any active connections or timers this method will almost always timeout.
   * Therefore this method is best meant for making sure that after you have
   * shutdown all of the connections, timers, and threads you have created
   * that you are guaranteed that they have ended, so that you may freely
   * release any resources they were using.
   *
   * GNE calls this method for you implicitly when GNE is shutdown.  This may
   * be a problem since by default GNE is shutdown with an atexit routine that
   * runs after main ends.  If this is a problem, you should explicitly call
   * GNE::shutdownGNE.
   *
   * The implementation of this method is simple and therefore not intended
   * to be used to create a program where you create detached threads and
   * then use the main thread to sit in this function until the "real"
   * program completes.  It is meant solely as a method of definitvely
   * verifying the completion of temporary threads, and waiting short times
   * for these threads to finish if needed.  If you need to wait a long time
   * (over 10 seconds) use join on the threads you've made, as join is much
   * more efficient.
   *
   * You may want to execute the requestAllShutdown method before calling
   * this method.
   *
   * @param ms the amount of time to wait for the threads before giving up.
   *           This is used so a stalled or crashed thread will still allow
   *           you to terminate the program.
   * @return false if all threads have completed, or true if there are still
   *              some threads running and the method timed out.
   *
   * @see requestAllShutdown
   * @see Connection::disconnectAll
   * @see GNE::shutdownGNE
   * @see Timer::stopAll
   */
  static bool waitForAllThreads(int ms);

  /**
   * Calls the shutDown method of all running threads of the given type.
   * This is simply a request to the thread that it should shut down -- it
   * is up to the implementation whether to ignore or heed this advice, and
   * how quickly.  You can pass the special type ALL to this function.
   *
   * Only %GNE itself should call requestAllShutdown with a parameter
   * besides "USER".  If you call this function, you should only use the
   * parameter USER.  If you want to shut down timers or connections, use
   * the appopriate functions of those classes.
   *
   * Threads marked as SYSTEM should never be touched by the user.  They will
   * shutdown when GNE exits.
   *
   * @see Connection::disconnectAll
   * @see Timer::stopAll
   */
  static void requestAllShutdown( ThreadType threadType );

  /**
   * Returns the name of this thread.
   */
  std::string getName() const;

  /**
   * Makes a request that this thread should shutdown.
   * Tells this thread to shutdown, if it is in an infinite loop.  You will
   * probably want to call join right after calling this to wait for the
   * shutdown to complete which is dependant on the thread you are shutting
   * down.
   *
   * This function is virtual if the thread needs any additional actions to
   * notify itself to shutdown, for example if it is waiting for some event on
   * a ConditionVariable.
   *
   * You will want to call this function from the override to make sure that
   * shutdown is set to true.
   *
   * This function is safe to call multiple times, but you cannot undo a
   * shutdown once it has been called.
   */
  virtual void shutDown();

  /**
   * A blocking call that returns when this thread terminates, or returns
   * immediately if the thread has terminated.
   */
  void join(unsigned int ms = INFINITE);

  /**
   * Returns true if this Thread has ever been started.  This remains true if
   * even after the Thread has stopped running.
   */
  bool hasStarted() const;

  /**
   * Determine the running state of the thread.  If running is true, then
   * started must be true.
   */
  bool isRunning() const;

  /**
   * Starts this thread running.  The thread will be marked as running before
   * this function completes.  Once a thread has stopped it can never be
   * restarted, so only one call to start can ever be made.
   *
   * @pre hasStarted returns false
   */
  void start();
  
  /**
   * Returns the priority of this class.
   */
  int getPriority() const;

  /**
   * The default priority of a thread.
   */
  static const int DEF_PRI;

  /**
   * The default name for a thread.
   */
  static const std::string DEF_NAME;

  /**
   * A lowered priority for a thread.  Might be good for background
   * operations like loading and saving files or something so it does not
   * interfer with the user interface (be it a GUI or a game or whatever).
   */
  static const int LOW_PRI;

  /**
   * A lower priority for a thread than LOW_PRI.
   */
  static const int LOWER_PRI;

  /**
   * A "boosted priority" for a thread.  This is optimized to give some extra
   * priority for the network threads to reduce network latency in favor of a
   * little in-game fps.
   */
  static const int HIGH_PRI;

  /**
   * Even higher priority thread than HIGH_PRI.  Used typically for the timer
   * threads.
   */
  static const int HIGHER_PRI;

protected:
  /**
   * IMPORTANT: call this method in your static create function.
   * Unfortunately there is no way to force an object to be managed by
   * a smart pointer, so when you derive from Thread you must do this
   * yourself, preferably through a public static create method.  See the
   * threading examples for this.
   *
   * IF YOU DO NOT SET THIS YOUR PROGRAM WILL CRASH!  You must set it before
   * any Thread methods are called (excluding the Thread constructor).
   *
   * @see getThisPointer
   */
  void setThisPointer( const wptr& thisPtr );

  /**
   * Returns a SmartPtr to this object.
   *
   * @see setThisPointer
   */
  sptr getThisPointer() const;

  /**
   * Sets this Thread's type.  You shouldn't call this function, since the
   * thread type USER is default, and only threads created by GNE should be
   * anything but USER.
   */
  void setType( ThreadType newType );

  /**
   * This variable is set to true when this thread should exit.  Users that
   * implement a Thread class must respond to this if the thread wants to
   * wait in an infinite loop.  Threads that will eventually stop can respond
   * only optionally to this, but if they last for a long time, shutting down
   * all threads may take awhile.  Responding to this flag usually takes
   * nothing more than a while (!shutdown) {} loop.
   */
  volatile bool shutdown;

  /**
   * This function is the starting point for this thread.
   * Derive your own class from Thread, and implement this function.
   * When this function returns, the thread ends.
   */
  virtual void run() = 0;

private:
  struct ThreadIDData;
  ThreadIDData* id;

#ifdef WIN32
  static unsigned __stdcall threadStart(void* thread);
#else
  static void* threadStart(void* thread);
#endif

  /**
   * Detaches the thread if it wasn't joined on when it is destructed.
   */
  void detach();

  /**
   * This function is called internally and automatically after run ends and
   * is used for internal library cleanup.
   */
  void end();

  static void remove( const ThreadIDData& d );

  wptr thisThread;

  ThreadType type;

  std::string name;

  bool started;

  bool running;

  bool joined;

  int priority;

  Mutex joinSync; //needed for multiple threads joining.
};

}
#endif /* THREAD_H_INCLUDED_C51E3746 */
