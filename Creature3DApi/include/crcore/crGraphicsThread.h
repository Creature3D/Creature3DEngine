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
#ifndef CRCORE_CRGRAPHICSTHREAD
#define CRCORE_CRGRAPHICSTHREAD 1

#include <CRCore/crState.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#include <CRCore/thread/crCondition.h>
#include <CRCore/thread/crScopedLock.h>
#include <list>

namespace CRCore {

// forward declare crGraphicsContext
class crGraphicsContext;    
    
class CR_EXPORT crBlock: virtual public CRCore::Referenced {
    public:
        crBlock():m_released(false) {}

        inline void block()
        {
            CRCore::ScopedLock<crMutex> mutlock(m_mut);
            if( !m_released )
                m_cond.wait(&m_mut);
        }

        inline void release()
        {
            CRCore::ScopedLock<crMutex> mutlock(m_mut);
            if (!m_released)
            {
                m_released = true;
                m_cond.broadcast();
            }
        }

        inline void reset()
        {
            CRCore::ScopedLock<crMutex> mutlock(m_mut);
            m_released = false;
        }
        
        inline void set(bool doRelease)
        {
            if (doRelease!=m_released)
            {
                if (doRelease) release();
                else reset();
            }
        }

    protected:

        ~crBlock()
        {
            release();
        }

    private:
        crMutex m_mut;
        CRCore::crCondition m_cond;
        bool m_released;
};

/** crGraphicsThread is a helper class for running OpenGL GraphicsOperation within a single thread assigned to a specific crGraphicsContext.*/
class CR_EXPORT crGraphicsThread : public CRCore::crThread, public CRCore::Referenced
{
    public:
        crGraphicsThread();

        /** Base class for implementing crGraphicsThread operations.*/
        struct CR_EXPORT Operation : virtual public Referenced
        {
            Operation(const std::string& name, bool keep):
                m_name(name),
                m_keep(keep) {}
                
            /** Set the human readable name of the operation.*/
            void setName(const std::string& name) { m_name = name; }

            /** Get the human readable name of the operation.*/
            const std::string& getName() const { return m_name; }
        
            /** Set whether the operation should be kept once its been applied.*/ 
            void setKeep(bool keep) { m_keep = keep; }

            /** Get whether the operation should be kept once its been applied.*/ 
            bool getKeep() const { return m_keep; }

            /** if this operation is a barrier then release it.*/
            virtual void release() {}

            /** Do the actual task of this operation.*/ 
            virtual void operator () (crGraphicsContext*) {}
            
            std::string m_name;
            bool        m_keep;
        };

        /** Add operation to end of OperationQueue, this will be 
          * executed by the graphics thread once this operation gets to the head of the queue.*/
        void add(Operation* operation, bool waitForCompletion=false);
        
        /** Remove operation from OperationQueue.*/
        void remove(Operation* operation);

        /** Remove named operation from OperationQueue.*/
        void remove(const std::string& name);

        /** Remove all operations from OperationQueue.*/
        void removeAllOperations();
        
        /** Get the operation currently being run.*/
        CRCore::ref_ptr<Operation> getCurrentOperation() { return m_currentOperation; }

        /** Run does the graphics thread run loop.*/        
        virtual void run();
        
        void setDone(bool done);
        bool getDone() const { return m_done; }

        /** Cancel this graphics thread.*/        
        virtual int cancel();

    protected:
    
        virtual ~crGraphicsThread();
        
        friend class crGraphicsContext;
        crGraphicsContext*    m_graphicsContext;

        typedef std::list< ref_ptr<Operation> > OperationQueue;

        bool                        m_done;

        crMutex          m_operationsMutex;
        CRCore::ref_ptr<CRCore::crBlock>    m_operationsBlock;
        OperationQueue              m_operations;
        CRCore::ref_ptr<Operation>     m_currentOperation;

};


/** SwapBufferOperation calls swap buffers on the crGraphicsContext.*/
struct CR_EXPORT SwapBuffersOperation : public crGraphicsThread::Operation
{
    SwapBuffersOperation():
        crGraphicsThread::Operation("SwapBuffers",true) {}

    virtual void operator () (crGraphicsContext* context);
};

/** crBarrierOperation allows one to syncronize multiple GraphicsThreads with each other.*/
struct CR_EXPORT crBarrierOperation : public crGraphicsThread::Operation, public CRCore::crBarrier
{
    enum PreBlockOp
    {
        NO_OPERATION,
        GL_FLUSH,
        GL_FINISH
    };

    crBarrierOperation(int numThreads, PreBlockOp op=NO_OPERATION):
        crGraphicsThread::Operation("Barrier", true),
        CRCore::crBarrier(numThreads),
        m_preBlockOp(op) {}

    virtual void release();

    virtual void operator () (crGraphicsContext* context);
    
    PreBlockOp m_preBlockOp;
};

/** crReleaseContext_Block_MakeCurrentOperation releases the context for another thread to aquire, 
  * then blocks waiting for context to be released, once the block is release the context is re-aqquired.*/
struct CR_EXPORT crReleaseContext_Block_MakeCurrentOperation : public crGraphicsThread::Operation, public crBlock
{
    crReleaseContext_Block_MakeCurrentOperation():
        crGraphicsThread::Operation("ReleaseContext_Block_MakeCurrent", false) {}

    virtual void release();

    virtual void operator () (crGraphicsContext* context);
};

}

#endif
