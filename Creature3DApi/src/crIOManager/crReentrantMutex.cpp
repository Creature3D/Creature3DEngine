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
#include <CRIOManager/crReentrantMutex.h>


using namespace CRIOManager;
using namespace CRCore;

crReentrantMutex::crReentrantMutex():
    m_threadHoldingMutex(0),
    m_lockCount(0)
{
}

crReentrantMutex::~crReentrantMutex()
{
}

int crReentrantMutex::lock()
{
    if (m_threadHoldingMutex==CRCore::crThread::currentThread() && m_lockCount>0)
    {
        ++m_lockCount;
        return 0;
    }
    else
    {
        int result = crMutex::lock();
        if (result==0)
        {
            m_threadHoldingMutex = CRCore::crThread::currentThread();
            m_lockCount = 1;
        }
        return result;
    }
}

int crReentrantMutex::unlock()
{
    if (m_threadHoldingMutex==CRCore::crThread::currentThread() && m_lockCount>0)
    {
        --m_lockCount;
        if (m_lockCount<=0) return crMutex::unlock();
    }
    return 0;
}

int crReentrantMutex::trylock()
{
    if (m_threadHoldingMutex==CRCore::crThread::currentThread() && m_lockCount>0)
    {
        ++m_lockCount;
        return 0;
    }
    else
    {
        int result = crMutex::trylock();
        if (result==0)
        {
            m_threadHoldingMutex = CRCore::crThread::currentThread();
            m_lockCount = 1;
        }
        return result;
    }
}
