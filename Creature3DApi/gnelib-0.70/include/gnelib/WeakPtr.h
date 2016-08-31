#ifndef WEAKPTR_H_NOP203
#define WEAKPTR_H_NOP203

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

#include <boost/weak_ptr.hpp>
#include "SmartPtr.h"

namespace GNE {

/**
 * @ingroup midlevel
 *
 * The WeakPtr class is a weak pointer to an object managed by SmartPtr.
 * Weak pointers do not prevent the objects they refer to from being destroyed
 * when all strong pointers dissapear.
 *
 * You cannot access the object directly through the weak pointer.  You have
 * to construct a strong pointer (SmartPtr) in order to use the object.  If
 * the object pointed to has been destructed, then the created strong pointer
 * will be empty ("null").  You can do this through the lock method.
 *
 * For now the implementation of WeakPtr is based on boost::weak_ptr.  You
 * can see details at http://www.boost.org/libs/smart_ptr/shared_ptr.htm
 * This class is CopyConstructable and Assignable so it can be used in the
 * standard container objects (such as std::vector).
 *
 * See the expointers example for usage of weak_ptr.
 */
template <class T>
class WeakPtr : public boost::weak_ptr<T> {
public:
  WeakPtr() : boost::weak_ptr<T>() {}
  ~WeakPtr() {}

  template<class Y> WeakPtr( const boost::shared_ptr<Y>& o ) : boost::weak_ptr<T>( o ) {}
  template<class Y> WeakPtr( const boost::weak_ptr<T>& o ) : boost::weak_ptr<T>( o ) {}

  template<class Y> WeakPtr& operator =( const boost::weak_ptr<Y>& r ) {
    boost::weak_ptr<T>& this_ = *this;
    this_ = r;
    return *this;
  }

  template<class Y> WeakPtr& operator =( const boost::shared_ptr<Y>& r ) {
    boost::weak_ptr<T>& this_ = *this;
    this_ = r;
    return *this;
  }
};

template<class T> inline void swap(WeakPtr<T> & a, WeakPtr<T> & b)
{
    a.swap(b);
}

}; //namespace GNE

#endif //#define WEAKPTR_H_NOP203
