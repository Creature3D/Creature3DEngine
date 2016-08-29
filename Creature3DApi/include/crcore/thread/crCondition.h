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

#ifndef CRCORE_CRCONDITION_H
#define CRCORE_CRCONDITION_H

#include <CRCore/crExport.h>
#include <CRCore/thread/crMutex.h>
namespace CRCore {

class CR_EXPORT crCondition
{
public:
    crCondition();
    virtual ~crCondition();

    /**
     *  Wait on a mutex.
     */
    virtual int wait(crMutex *mutex);

    /**
     *  Wait on a mutex for a given amount of time (ms)
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int wait(crMutex *mutex, unsigned long int ms);

    /**
     *  Signal a SINGLE thread to wake if it's waiting.
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int signal();

    /**
     *  Wake all threads waiting on this condition.
     *
     *  @return 0 if normal, -1 if errno set, errno code otherwise.
     */
    virtual int broadcast();
	virtual bool haswaiters();
private:
    /**
     *  Private copy constructor, to prevent tampering.
     */
    crCondition(const crCondition &/*c*/) {};

    /**
     *  Private copy assignment, to prevent tampering.
     */
    crCondition &operator=(const crCondition &/*c*/) {return *(this);};

    /**
     *  Implementation-specific data
     */
    void *m_prvData;

};

}

#endif // !_OPENTHREADS_CONDITION_
