#ifndef SMARTPTR_H_KPW312
#define SMARTPTR_H_KPW312

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
#include <boost/shared_ptr.hpp>

namespace GNE {

/**
 * @ingroup midlevel
 *
 * The SmartPtr class is a reference-counted smart pointer implementation for
 * general use
 *
 * These smart pointers hold a reference count to their object.  When the last
 * handle to the object is lost, then the object is automatically destroyed
 * using the deleter object (the default object simply calls "delete").  This
 * works if there are no "cycles".  Use weak pointers to break cycles (see URL
 * below for details).
 *
 * The reference counting makes it easy to manage objects in multiple threads.
 * It is safe to use handles to the same object in different threads.  It is
 * NOT safe to share the exact same SmartPtr instance between threads, but
 * this should not be a problem since SmartPtrs are meant to be copied.
 *
 * Because the smart pointers are CopyConstructable, Assignable and implement
 * operator &lt;, they can be used in the STL containers, including std::map.
 *
 * An object managed by any of the smart pointer classes must be on the heap
 * when using the default deleter.
 *
 * Very simple example usage (see expointers example for more):
 * <pre>
 * void func() {
 *   SmartPtr<MyObject> oPtr ( new MyObject() );
 *   oPtr->method();
 * } //oPtr will automatically destroy its object.
 * </pre>
 *
 * The reason for not using boost::shared_ptr directly is because I wanted to
 * abstract the implementation of the pointer.  Right now it uses a passive
 * reference count but an intrusive reference count may have better
 * performance so I may switch in the future, but for now it just inherits
 * from another implementation.
 *
 * So although the methods aren't all listed here, I am supporting the
 * following methods: (you can see details at
 * http://www.boost.org/libs/smart_ptr/shared_ptr.htm)
 *
 * <ul><li>SmartPtr::element_type</li>
 * <li>default constructor</li>
 * <li>constructor taking a T*</li>
 * <li>constructor taking a T* and deleter object</li>
 * <li>public destructor</li>
 * <li>copy constructor</li>
 * <li>operator =</li>
 * <li>get</li>
 * <li>reset</li>
 * <li>reset( T* )</li>
 * <li>reset( T*, deleter )</li>
 * <li>operator * (dereference NOT multiplication!)</li>
 * <li>operator-></li>
 * <li>conversion to bool</li>
 * <li>operator !</li>
 * <li>swap</li>
 * <li>operator ==</li>
 * <li>operator !=</li>
 * <li>operator &lt;</li>
 * <li>static_pointer_cast</li>
 * <li>dynamic_pointer_cast</li></ul>
 *
 * @see GNE::WeakPtr
 * @see NullDeleter
 */
template <class T>
class SmartPtr : public boost::shared_ptr<T> {
public:
  SmartPtr() {}
  template<class Y> explicit SmartPtr( Y* p ) : boost::shared_ptr<T>( p ) {}
  template<class Y, class D> SmartPtr( Y* p, D d ) : boost::shared_ptr<T>( p, d ) {}

  SmartPtr( const boost::shared_ptr<T>& o ) : boost::shared_ptr<T>( o ) {}
  template<class Y> SmartPtr( const SmartPtr<Y>& o ) : boost::shared_ptr<T>( o ) {}

  template<class Y> SmartPtr& operator =( const boost::shared_ptr<Y>& r ) {
    boost::shared_ptr<T>& this_ = *this;
    this_ = r;
    return *this;
  }

  //This is not needed, but I provide it so MSVC's intellisense can recongize
  //accesses to T, otherwise it does absolutely nothing.
  T* operator->() const {
    const boost::shared_ptr<T>& this_ = *this;
    return this_.operator ->();
  }

  ~SmartPtr() {}
};

template<class T> inline void swap(SmartPtr<T> & a, SmartPtr<T> & b)
{
    a.swap(b);
}

template<class T, class U> SmartPtr<T> static_pointer_cast(SmartPtr<U> const & r)
{
    return SmartPtr<T>( boost::static_pointer_cast<T>( r ) );
}

template<class T, class U> SmartPtr<T> dynamic_pointer_cast(SmartPtr<U> const & r)
{
    return SmartPtr<T>( boost::dynamic_pointer_cast<T>( r ) );
}

/**
 * Deleter that does nothing for the SmartPtr.  Useful for making SmartPtrs to
 * static variables.
 */
class NullDeleter {
public:
  void operator() (void const *) const {
  }
};

};

#endif //#define GNESMARTPTR_H_KPW312
