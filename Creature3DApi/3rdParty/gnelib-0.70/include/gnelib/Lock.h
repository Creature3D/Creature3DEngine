#ifndef LOCK_H_KPQ301
#define LOCK_H_KPQ301

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

namespace GNE {
  class Mutex;
  class ConditionVariable;
  class SynchronizedObject;

/**
 * @ingroup threading
 *
 * A small helper class that locks the object in its ctor and unlocks it in
 * its destructor.  This is handy if a largish function has multiple exit
 * points espically if it throws exceptions.  This class makes sure that once
 * you lock the object it will be unlocked when you leave the scope.
 *
 * A Lock instance may not be copied, and you must specify a object to lock in
 * its constructor.
 *
 * Lock objects can work for any object that has the methods "acquire" and
 * "release".  In %GNE these are Mutexes, ConditionVariables, and classes that
 * inherit from SynchronizedObject.
 *
 * @see LockMutex
 * @see LockObject
 * @see LockCV
 */
template <class T>
class Lock {
public:
  Lock(T& objToLock) : obj(objToLock) {
    obj.acquire();
  }

  ~Lock() {
    obj.release();
  }
private:
  //LockMutex cannot be copied or constructed defaultly.
  Lock();
  Lock(Lock&);
  Lock& operator= (Lock& rhs);

  T& obj;
};

typedef Lock<Mutex> LockMutex;
typedef Lock<ConditionVariable> LockCV;

/**
 * Locks a SynchronizedObject.  The Console::gout object is such an example.
 * You can use <code>LockObject lock(gout);</code> as a safer alternative to
 * the stream manipulators Console::acquire and Console::release.
 */
typedef Lock<SynchronizedObject> LockObject;

/**
 * @ingroup threading
 *
 * Works exactly like Lock but allows early release.  Two separate classes
 * were provided because on many compilers Lock has absolutely 0 overhead
 * while when trying to provide early release, there may be some overhead.
 *
 * Since those cases are rare, the Ex version can be used when early release
 * is needed.
 *
 * @see Lock
 * @see LockMutexEx
 * @see LockObjectEx
 * @see LockCVEx
 */
template <class T>
class LockEx {
public:
  LockEx(T& objToLock) : obj(objToLock), released(false) {
    obj.acquire();
  }

  void release() {
    if ( !released ) {
      obj.release();
      released = true;
    }
  }

  ~LockEx() {
    if ( !released )
      obj.release();
  }
private:
  //LockMutexEx cannot be copied or constructed defaultly.
  LockEx();
  LockEx(LockEx&);
  LockEx& operator= (LockEx& rhs);

  T& obj;

  bool released;
};

typedef LockEx<Mutex> LockMutexEx;
typedef LockEx<ConditionVariable> LockCVEx;
typedef LockEx<SynchronizedObject> LockObjectEx;

} //namespace GNE

#endif
