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

#ifndef CRCORE_CRBARRIER_H
#define CRCORE_CRBARRIER_H

#include <CRCore/crExport.h>
namespace CRCore {

class CR_EXPORT crBarrier
{
public:
    crBarrier(int numThreads=0);
    virtual ~crBarrier();
    /**
     *  Reset the barrier to it's original state.
     */
    virtual void reset();
    /**
     *  Block until numThreads threads have entered the barrier.
     */
    virtual void block(unsigned int numThreads=0);
    /**
     *  Release the barrier, now.
     */
    virtual void release();
    /**
     *  Return the number of threads currently blocked in the barrier,
     *  Return -1 if error.
     */ 
    virtual int numThreadsCurrentlyBlocked();

    void invalidate();
private:
    /**
     *  Private copy constructor, to prevent tampering.
     */
	crBarrier(const crBarrier &/*b*/) {};
    /**
     *  Private copy assignment, to prevent tampering.
     */
    crBarrier &operator=(const crBarrier &/*b*/) {return *(this);};
    /*
     *  Implementation-specific private data.
     */
    void *m_prvData;
    bool m_valid;
};

}

#endif

