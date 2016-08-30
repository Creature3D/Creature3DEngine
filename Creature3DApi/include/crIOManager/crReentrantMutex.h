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
#ifndef CRIOMANAGER_REENTRANTMUTEX
#define CRIOMANAGER_REENTRANTMUTEX

#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crMutex.h>

#include <CRIOManager/crExport.h>

namespace CRIOManager {

class CRIOMANAGER_EXPORT crReentrantMutex : public CRCore::crMutex
{
    public:

        crReentrantMutex();

        virtual ~crReentrantMutex();

        virtual int lock();

        virtual int unlock();

        virtual int trylock();

    private:

        crReentrantMutex(const crReentrantMutex &):CRCore::crMutex() {}

        crReentrantMutex &operator=(const crReentrantMutex &) {return *(this);}
        
        CRCore::crThread* m_threadHoldingMutex;
        unsigned int         m_lockCount;

};

}

#endif
