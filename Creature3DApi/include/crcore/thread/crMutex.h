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

#ifndef CRCORE_MUTEX_H
#define CRCORE_MUTEX_H

#include <CRCore/crExport.h>

namespace CRCore {

/**
 *  @class crMutex
 *  @brief  This class provides an object-oriented thread mutex interface.
 */
class CR_EXPORT crMutex
{
    friend class Condition;
public:
    crMutex();
    virtual ~crMutex();

    /**
     *  Lock the mutex
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int lock();

    /**
     *  Unlock the mutex
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int unlock();

    /**
     *  Test if mutex can be locked.
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int trylock();

private:
    /**
     *  Private copy constructor, to prevent tampering.
     */
    crMutex(const crMutex &/*m*/) {};

    /**
     *  Private copy assignment, to prevent tampering.
     */
    crMutex &operator=(const crMutex &/*m*/) {return *(this);};

    /**
     *  Implementation-specific private data.
     */
    void *m_prvData;

};

}

#endif // _OPENTHREADS_MUTEX_
