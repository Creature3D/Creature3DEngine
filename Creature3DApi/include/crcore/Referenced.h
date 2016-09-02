/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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
//Modified by 吴财华
#ifndef CRCORE_REFERENCED_H
#define CRCORE_REFERENCED_H 1

#include <CRCore/crExport.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crMutex.h>
namespace CRCore {

class CR_EXPORT Referenced
{
    public:
        Referenced() 
        {
           m_refCount=0;
        }
        Referenced(const Referenced&) {
            m_refCount=0;
        }

        inline Referenced& operator = (const Referenced&) { return *this; }

        /** increment the reference count by one, indicating that 
            this object has another pointer which is referencing it.*/
        inline void ref() const 
		{
			m_refMutex.lock();
			++m_refCount;
			m_refMutex.unlock();
		}		
        /** decrement the reference count by one, indicating that 
            a pointer to this object is referencing it.  If the
            reference count goes to zero, it is assumed that this object
            is no longer referenced and is automatically deleted.*/
        inline void unref() const;
        
        /** decrement the reference count by one, indicating that 
            a pointer to this object is referencing it.  However, do
            not delete it, even if ref count goes to 0.  Warning, unref_nodelete() 
            should only be called if the user knows exactly who will
            be resonsible for, one should prefer unref() over unref_nodelete() 
            as the later can lead to memory leaks.*/
        inline void unref_nodelete() const { m_refMutex.lock(); --m_refCount; m_refMutex.unlock(); }
        
        /** return the number pointers currently referencing this object. */
        inline int referenceCount() const { return m_refCount; }
		inline int referenceCount() { return m_refCount; }

		virtual void clear(){}//for static ManagerInstance clear
    protected:
        virtual ~Referenced();
        mutable volatile int m_refCount;
		mutable crMutex      m_refMutex;
};

inline void Referenced::unref() const
{
	m_refMutex.lock();
    --m_refCount;
	int refcount = m_refCount;
	m_refMutex.unlock();
    if (refcount==0)
    {
        delete this;
    }
    else if (refcount<0)
    {
		notify(WARN)<<"Warning: deleting still referenced object "<<this<<" of type '"<<typeid(this).name()<<"'"<<std::endl;
        //throw 2325;
    }
}
}
#endif
