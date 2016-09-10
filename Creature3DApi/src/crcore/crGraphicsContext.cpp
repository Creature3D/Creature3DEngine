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
//Modified by Wucaihua
#include <CRCore/crGraphicsContext.h>
#include <CRCore/crNotify.h>
#include <map>

using namespace CRCore;

static ref_ptr<crGraphicsContext::CreateGraphicContextCallback> s_createGraphicsContextCallback;

void crGraphicsContext::setCreateGraphicsContextCallback(CreateGraphicContextCallback* callback)
{
    s_createGraphicsContextCallback = callback;
}

crGraphicsContext::CreateGraphicContextCallback* crGraphicsContext::getCreateGraphicsContextCallback()
{
    return s_createGraphicsContextCallback.get();
}

crGraphicsContext* crGraphicsContext::createGraphicsContext(Traits* traits)
{
    if (s_createGraphicsContextCallback.valid())
        return s_createGraphicsContextCallback->createGraphicsContext(traits);
    else
        return 0;    
}


typedef std::map<unsigned int, unsigned int>  ContextIDMap;
static ContextIDMap s_contextIDMap;
static crMutex s_contextIDMapMutex;

unsigned int crGraphicsContext::createNewContextID()
{
    CRCore::ScopedLock<crMutex> lock(s_contextIDMapMutex);
    
    // first check to see if we can reuse contextID;
    for(ContextIDMap::iterator itr = s_contextIDMap.begin();
        itr != s_contextIDMap.end();
        ++itr)
    {
        if (itr->second == 0)
        {

            // reuse contextID;
            itr->second = 1;

            //CRCore::notify(CRCore::INFO)<<"crGraphicsContext::createNewContextID() reusing contextID="<<itr->first<<std::endl;

            return itr->first;
        }
    }

    unsigned int contextID = s_contextIDMap.size();
    s_contextIDMap[contextID] = 1;
    
    //CRCore::notify(CRCore::INFO)<<"crGraphicsContext::createNewContextID() creating contextID="<<contextID<<std::endl;
    

    if ( (contextID+1) > CRCore::crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts() )
    {
        //CRCore::notify(CRCore::INFO)<<"Updating the MaxNumberOfGraphicsContexts to "<<contextID+1<<std::endl;

        // update the the maximum number of graphics contexts, 
        // to ensure that texture objects and display buffers are configured to the correct size.
        CRCore::crDisplaySettings::instance()->setMaxNumberOfGraphicsContexts( contextID + 1 );
    }
    

    return contextID;    
}

void crGraphicsContext::incrementContextIDUsageCount(unsigned int contextID)
{
    CRCore::ScopedLock<crMutex> lock(s_contextIDMapMutex);
    
    //CRCore::notify(CRCore::INFO)<<"crGraphicsContext::incrementContextIDUsageCount("<<contextID<<") to "<<s_contextIDMap[contextID]<<std::endl;

    ++s_contextIDMap[contextID];
}

void crGraphicsContext::decrementContextIDUsageCount(unsigned int contextID)
{

    CRCore::ScopedLock<crMutex> lock(s_contextIDMapMutex);
    

    if (s_contextIDMap[contextID]!=0)
    {
        --s_contextIDMap[contextID];
    }
    else
    {
        CRCore::notify(CRCore::NOTICE)<<"Warning: decrementContextIDUsageCount("<<contextID<<") called on expired contextID."<<std::endl;
    } 

    //CRCore::notify(CRCore::INFO)<<"crGraphicsContext::decrementContextIDUsageCount("<<contextID<<") to "<<s_contextIDMap[contextID]<<std::endl;

}


crGraphicsContext::crGraphicsContext():
    m_threadOfLastMakeCurrent(0)
{
}

crGraphicsContext::~crGraphicsContext()
{
    close(false);
}

/** Realise the crGraphicsContext.*/
bool crGraphicsContext::realize()
{
    if (realizeImplementation())
    {
        if (m_graphicsThread.valid() && !m_graphicsThread->isRunning())
        {
            m_graphicsThread->startThread();
        }
        return true;
    }
    else
    {   
        return false;
    }
}

void crGraphicsContext::close(bool callCloseImplementation)
{
    // switch off the graphics thread...
    setGraphicsThread(0);
    
    if (callCloseImplementation) closeImplementation();

    if (m_state.valid())
    {
        decrementContextIDUsageCount(m_state->getContextID());
        
        m_state = 0;
    }
}


void crGraphicsContext::makeCurrent()
{
    crReleaseContext_Block_MakeCurrentOperation* rcbmco = 0;

    if (m_graphicsThread.valid() && 
        m_threadOfLastMakeCurrent == m_graphicsThread.get() &&
        m_threadOfLastMakeCurrent != CRCore::crThread::currentThread())
    {
        // create a relase contex, block and make current operation to stop the graphics thread while we use the graphics context for ourselves
        rcbmco = new crReleaseContext_Block_MakeCurrentOperation;
        m_graphicsThread->add(rcbmco);
    }

    if (!isCurrent()) m_mutex.lock();

    makeCurrentImplementation();
    
    m_threadOfLastMakeCurrent = CRCore::crThread::currentThread();
    
    if (rcbmco)
    {
        // Let the "relase contex, block and make current operation" proceed, which will now move on to trying to aquire the graphics
        // contex itself with a makeCurrent(), this will then block on the crGraphicsContext mutex till releaseContext() releases it.
        rcbmco->release();
    }
}

void crGraphicsContext::makeContextCurrent(crGraphicsContext* readContext)
{
    if (!isCurrent()) m_mutex.lock();

    makeContextCurrentImplementation(readContext);

    m_threadOfLastMakeCurrent = CRCore::crThread::currentThread();
}

void crGraphicsContext::releaseContext()
{
    m_mutex.unlock();
}

void crGraphicsContext::swapBuffers()
{
    if (isCurrent())
    {
        swapBuffersImplementation();
    }
    else if (m_graphicsThread.valid() && 
             m_threadOfLastMakeCurrent == m_graphicsThread.get())
    {
        m_graphicsThread->add(new SwapBuffersOperation);
    }
    else
    {
        makeCurrent();
        swapBuffersImplementation();
        releaseContext();
    }
}



void crGraphicsContext::createGraphicsThread()
{
    if (!m_graphicsThread)
    {
        setGraphicsThread(new crGraphicsThread);
    }
}

void crGraphicsContext::setGraphicsThread(crGraphicsThread* gt)
{
    if (m_graphicsThread==gt) return; 

    if (m_graphicsThread.valid()) 
    {
        // need to kill the thread in some way...
        m_graphicsThread->cancel();
        m_graphicsThread->m_graphicsContext = 0;
    }

    m_graphicsThread = gt;
    
    if (m_graphicsThread.valid()) 
    {
        m_graphicsThread->m_graphicsContext = this;
        
        if (!m_graphicsThread->isRunning())
        {
            m_graphicsThread->startThread();
        }
    }
}
