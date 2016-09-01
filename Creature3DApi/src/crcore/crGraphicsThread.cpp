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
#include <CRCore/crGraphicsThread.h>
#include <CRCore/crGraphicsContext.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

struct ThreadExitTidyUp
{
    ThreadExitTidyUp(CRCore::crGraphicsContext* context, bool closeContextOnExit):
        m_context(context),
        m_closeContextOnExit(closeContextOnExit)
    {
        //CRCore::notify(CRCore::INFO)<<"starting thread context "<<m_context<<std::endl;
    }

    ~ThreadExitTidyUp()
    {
       // CRCore::notify(CRCore::INFO)<<"exit thread"<<std::endl;
        if (m_context)
        {
            if (m_closeContextOnExit)
            {
               // CRCore::notify(CRCore::INFO)<<"    - close context "<<m_context<<std::endl;

                m_context->closeImplementation();

               // CRCore::notify(CRCore::INFO)<<"    - done close context "<<m_context<<std::endl;
            }
            //else
            //{
            //    CRCore::notify(CRCore::INFO)<<"    - releaseContext "<<m_context<<std::endl;

                //m_context->releaseContext();
           // }
        }
    }
    
    CRCore::crGraphicsContext* m_context;
    bool m_closeContextOnExit;
    
    
};

struct BlockOperation : public crGraphicsThread::Operation, public crBlock
{
    BlockOperation():
        crGraphicsThread::Operation("crBlock",false)
    {
        reset();
    }

    virtual void release()
    {
        crBlock::release();
    }

    virtual void operator () (crGraphicsContext*)
    {
        glFlush();
        crBlock::release();
    }
};


crGraphicsThread::crGraphicsThread():
    m_graphicsContext(0),
    m_done(false)
{
    m_operationsBlock = new crBlock;
}

crGraphicsThread::~crGraphicsThread()
{
    //CRCore::notify(CRCore::INFO)<<"Destructing graphics thread"<<std::endl;

    cancel();
}

void crGraphicsThread::setDone(bool done)
{
    if (m_done==done) return;

    m_done = true;

    if (done)
    {
        //CRCore::notify(CRCore::INFO)<<"set done "<<this<<std::endl;
        
        {
            CRCore::ScopedLock<crMutex> lock(m_operationsMutex);
            if (m_currentOperation.valid())
            {
                //CRCore::notify(CRCore::INFO)<<"releasing "<<m_currentOperation.get()<<std::endl;
                m_currentOperation->release();
            }
        }

        m_operationsBlock->release();
    }

}

int crGraphicsThread::cancel()
{
    //CRCore::notify(CRCore::INFO)<<"Cancelling graphics thread "<<this<<std::endl;

    int result = 0;
    if( isRunning() )
    {
    
        m_done = true;

       // CRCore::notify(CRCore::INFO)<<"   Doing cancel "<<this<<std::endl;

        for(OperationQueue::iterator itr = m_operations.begin();
            itr != m_operations.end();
            ++itr)
        {
            (*itr)->release();
        }
        
        // release the frameBlock and _databasePagerThreadBlock incase its holding up thread cancelation.
        m_operationsBlock->release();

        // then wait for the the thread to stop running.
        while(isRunning())
        {
            m_operationsBlock->release();

            {
                CRCore::ScopedLock<crMutex> lock(m_operationsMutex);

                for(OperationQueue::iterator itr = m_operations.begin();
                    itr != m_operations.end();
                    ++itr)
                {
                    (*itr)->release();
                }

                if (m_currentOperation.valid()) m_currentOperation->release();
            }

            // commenting out debug info as it was cashing crash on exit, presumable
            // due to CRCore::notify or std::cout destructing earlier than this destructor.
            //CRCore::notify(CRCore::INFO)<<"   Waiting for crGraphicsThread to cancel "<<this<<std::endl;
			CRCore::crThread::sleep(10);
        }
    }

    return result;
}

void crGraphicsThread::add(Operation* operation, bool waitForCompletion)
{
    //CRCore::notify(CRCore::INFO)<<"Doing add"<<std::endl;

    ref_ptr<BlockOperation> block = 0;

    {
        // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
        CRCore::ScopedLock<crMutex> lock(m_operationsMutex);

        // add the operation to the end of the list
        m_operations.push_back(operation);

        if (waitForCompletion)
        {
            block = new BlockOperation;
            m_operations.push_back(block.get());
        }
        
        m_operationsBlock->set(true);
    }
    
    if (block.valid())
    {
        // now we wait till the barrier is joined by the graphics thread.
        block->block();
    }
}

void crGraphicsThread::remove(Operation* operation)
{
    //CRCore::notify(CRCore::INFO)<<"Doing remove operation"<<std::endl;

    // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
    CRCore::ScopedLock<crMutex> lock(m_operationsMutex);

    for(OperationQueue::iterator itr = m_operations.begin();
        itr!=m_operations.end();)
    {
        if ((*itr)==operation) itr = m_operations.erase(itr);
        else ++itr;
    }
}

void crGraphicsThread::remove(const std::string& name)
{
    //CRCore::notify(CRCore::INFO)<<"Doing remove named operation"<<std::endl;
    
    // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
    CRCore::ScopedLock<crMutex> lock(m_operationsMutex);

    // find the remove all operations with specificed name
    for(OperationQueue::iterator itr = m_operations.begin();
        itr!=m_operations.end();)
    {
        if ((*itr)->getName()==name) itr = m_operations.erase(itr);
        else ++itr;
    }
}

void crGraphicsThread::removeAllOperations()
{
   // CRCore::notify(CRCore::INFO)<<"Doing remove all operations"<<std::endl;

    CRCore::ScopedLock<crMutex> lock(m_operationsMutex);
    m_operations.clear();
}


void crGraphicsThread::run()
{
    bool contextRealizedInThisThread = false;

    // make the graphics context current.
    if (m_graphicsContext)
    {
        if (!m_graphicsContext->isRealized())
        {
            m_graphicsContext->realize();
            contextRealizedInThisThread = true;
        }
    
       // CRCore::notify(CRCore::INFO)<<"Doing make current"<<std::endl;
        m_graphicsContext->makeCurrent();
    }

    // create a local object to clean up once the thread is cancelled.
    ThreadExitTidyUp threadExitTypeUp(m_graphicsContext, contextRealizedInThisThread);

    //CRCore::notify(CRCore::INFO)<<"Doing run"<<std::endl;

    bool firstTime = true;

    OperationQueue::iterator itr = m_operations.begin();

    do
    {
        //CRCore::notify(CRCore::INFO)<<"In main loop "<<this<<std::endl;

        if (m_operations.empty())
        {
            m_operationsBlock->block();
            
            // exit from loop if m_done is set.
            if (m_done) break;
            
            itr = m_operations.begin();
        }
        else
        {
            if  (itr == m_operations.end()) itr = m_operations.begin();
        }

       // CRCore::notify(CRCore::INFO)<<"get op "<<m_done<<" "<<this<<std::endl;

        // get the front of the file request list.
        {
            CRCore::ScopedLock<crMutex> lock(m_operationsMutex);
            if (!m_operations.empty())
            {
                // get the next item
                m_currentOperation = *itr;
                
                if (!m_currentOperation->getKeep())
                {
                   // CRCore::notify(CRCore::INFO)<<"removing "<<m_currentOperation->getName()<<std::endl;

                    // remove it from the opeations queue
                    itr = m_operations.erase(itr);

                   // CRCore::notify(CRCore::INFO)<<"size "<<m_operations.size()<<std::endl;

                    if (m_operations.empty())
                    {
                      // CRCore::notify(CRCore::INFO)<<"setting block "<<m_operations.size()<<std::endl;
                       m_operationsBlock->set(false);
                    }
                }
                else
                {
                   // CRCore::notify(CRCore::INFO)<<"increment "<<m_currentOperation->getName()<<std::endl;

                    // move on to the next operation in the list.
                    ++itr;
                }
                

            }
            
        }
        
        if (m_currentOperation.valid())
        {
           // CRCore::notify(CRCore::INFO)<<"Doing op "<<m_currentOperation->getName()<<" "<<this<<std::endl;

            // call the graphics operation.
            (*m_currentOperation)(m_graphicsContext);

            {            
                CRCore::ScopedLock<crMutex> lock(m_operationsMutex);
                m_currentOperation = 0;
            }
        }

        if (firstTime)
        {
            // do a yield to get round a peculiar thread hang when testCancel() is called 
            // in certain cirumstances - of which there is no particular pattern.
			CRCore::crThread::sleep(10);
            firstTime = false;
        }

    } while (!testCancel() && !m_done);

   // CRCore::notify(CRCore::INFO)<<"exit loop "<<this<<std::endl;

}
 
void SwapBuffersOperation::operator () (crGraphicsContext* context)
{
    if (context)
    {
        context->swapBuffersImplementation();
    }
}

void crBarrierOperation::release()
{
    crBarrier::release();
}

void crBarrierOperation::operator () (crGraphicsContext*)
{
    if (m_preBlockOp==GL_FLUSH) glFlush();
    if (m_preBlockOp==GL_FINISH) glFinish();
    
    block();
}

void crReleaseContext_Block_MakeCurrentOperation::release()
{
    crBlock::release();
}


void crReleaseContext_Block_MakeCurrentOperation::operator () (crGraphicsContext* context)
{
    if (!context) return;
    
    // release the graphics context.
    context->releaseContext();
    
    // reset the block so that it the next call to block() 
    reset();
    
    // block this thread, untill the block is released externally.
    block();
    
    // re aquire the graphcis context.
    context->makeCurrent();
}
