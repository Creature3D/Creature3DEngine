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
#ifndef CRCORE_CRGRAPHICSCONTEXT
#define CRCORE_CRGRAPHICSCONTEXT 1

#include <CRCore/crState.h>
#include <CRCore/crGraphicsThread.h>

namespace CRCore {

/** Base class for providing Windowing API agnostic access to creating and managing graphics context.*/
class CR_EXPORT crGraphicsContext : public Referenced
{
    public:
    
        /** crGraphicsContext Traits object provides the specification of what type of graphics context is required.*/
        struct Traits : public CRCore::Referenced
        {
            Traits():
                m_displayNum(0),
                m_screenNum(0),
                m_x(0),
                m_y(0),
                m_width(0),
                m_height(0),
                m_windowDecoration(false),
                m_supportsResize(false),
                m_red(8),
                m_blue(8),
                m_green(8),
                m_alpha(0),
                m_depth(24),
                m_stencil(0),
                m_pbuffer(false),
                m_quadBufferStereo(false),
                m_doubleBuffer(false),
                m_target(0),
                m_level(0),
                m_face(0),
                m_mipMapGeneration(false),
                m_sharedContext() {}
                
            // where graphic context is be hosted.                
            std::string  m_hostName;
            unsigned int m_displayNum;
            unsigned int m_screenNum;
            
            // graphics context orginal and size
            unsigned int m_x;
            unsigned int m_y;
            unsigned int m_width;
            unsigned int m_height;
            
            // window decoration and baviour
            std::string m_windowName;
            bool        m_windowDecoration;
            bool        m_supportsResize;
            
            // buffer depths, 0 equals off.
            unsigned int m_red;
            unsigned int m_blue;
            unsigned int m_green;
            unsigned int m_alpha;
            unsigned int m_depth;
            unsigned int m_stencil;
            
            // buffer configuration
            bool m_pbuffer;
            bool m_quadBufferStereo;
            bool m_doubleBuffer;

            // render to texture
            GLenum          m_target;
            unsigned int    m_level;
            unsigned int    m_face;
            unsigned int    m_mipMapGeneration;
            
            // shared context
            crGraphicsContext* m_sharedContext;
        };
    
    
        /** Callback to be implemented to provide access to Windowing API's ability to create Windows/pbuffers.*/
        struct CreateGraphicContextCallback : public CRCore::Referenced
        {
            virtual crGraphicsContext* createGraphicsContext(Traits* traits) = 0;
            
            virtual ~CreateGraphicContextCallback() {};
        };
    
    
        /** Set the create graphics context callback - this callback should be supplied by the windows toolkit. */
        static void setCreateGraphicsContextCallback(CreateGraphicContextCallback* callback);
        
        /** Get the create graphics context callback*/
        static CreateGraphicContextCallback* getCreateGraphicsContextCallback();
    
        /** Create a graphics context for a specified set of traits.*/
        static crGraphicsContext* createGraphicsContext(Traits* traits);
        
        /** Create a contextID for a new graphics context, this contextID is used to set up the CRCore::crState associate with context.
          * Automatically increments the usage count of the contextID to 1.*/
        static unsigned int createNewContextID();

        /** Increment the usage count associate with a contextID. The usage count speficies how many graphics contexts a specific contextID is shared between.*/
        static void incrementContextIDUsageCount(unsigned int contextID);

        /** Decrement the usage count associate with a contextID. Once the contextID goes to 0 the contextID is then free to be reused.*/
        static void decrementContextIDUsageCount(unsigned int contextID);
    
    public:
    
        /** Get the traits of the crGraphicsContext.*/
        inline const Traits* getTraits() const { return m_traits.get(); }


        /** Set the crState object which tracks the current OpenGL state for this graphics context.*/
        inline void setState(crState* state) { m_state = state; }
        
        /** Get the crState object which tracks the current OpenGL state for this graphics context.*/
        inline crState* getState() { return m_state.get(); }
        
        /** Get the const crState object which tracks the current OpenGL state for this graphics context.*/
        inline const crState* getState() const { return m_state.get(); }
        

        /** Realise the crGraphicsContext.*/
        bool realize();

        /** close the graphics context.
          * close(bool) stops any associated graphics threads, releases the contextID for the crGraphicsContext then
          * optional calls closeImplementation() to do the actual deletion of the graphics.  This call is made optional
          * as there are times when the graphics context has already been deleted externally and only the OSG side
          * of the its data need to be closed down. */
        void close(bool callCloseImplementation=true);

        /** swap the front and back buffers.*/
        void swapBuffers();

        /** Return true if the graphics context has been realised and is ready to use.*/
        inline bool isRealized() const { return isRealizedImplementation(); }


        /** Make this graphics context current.
          * Implementated by first aquiring a lock of the crGraphicsContext mutex, and then doing a call to makeCurrentImplementation(). */
        void makeCurrent();
        
        /** Make this graphics context current with specified read context.
          * Implementated by first aquiring a lock of the crGraphicsContext mutex, and then doing a call to makeContextCurrentImplementation(). */
        void makeContextCurrent(crGraphicsContext* readContext);
        
        /** Release the graphics context by unlocking the crGraphicsContext mutex.*/
        void releaseContext();
        
        /** Return true if the current thread has this OpenGL graphics context.*/
        inline bool isCurrent() const { return m_threadOfLastMakeCurrent == CRCore::crThread::currentThread(); }

        /** Bind the graphics context to associated texture.*/
        inline void bindPBufferToTexture(GLenum buffer) { bindPBufferToTextureImplementation(buffer); }



        /** Create a graphics thread to the graphics context, so that the thread handles all OpenGL operations.*/
        void createGraphicsThread();

        /** Assign a graphics thread to the graphics context, so that the thread handles all OpenGL operations.*/
        void setGraphicsThread(crGraphicsThread* gt);

        /** Get the graphics thread assigned the graphics context.*/
        crGraphicsThread* getGraphicsThread() { return m_graphicsThread.get(); }

        /** Get the const graphics thread assigned the graphics context.*/
        const crGraphicsThread* getGraphicsThread() const { return m_graphicsThread.get(); }


        /** Realise the crGraphicsContext implementation, 
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual bool realizeImplementation() = 0;

        /** Return true if the graphics context has been realised, and is ready to use, implementation.
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual bool isRealizedImplementation() const = 0;

        /** Close the graphics context implementation.
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual void closeImplementation() = 0;

        /** Make this graphics context current implementation.
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual void makeCurrentImplementation() = 0;
        
        /** Make this graphics context current with specified read context implementation.
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual void makeContextCurrentImplementation(crGraphicsContext* readContext) = 0;

        /** Pure virtual, Bind the graphics context to associated texture implementation.
          * Pure virtual - must be implemented by concrate implementations of crGraphicsContext. */
        virtual void bindPBufferToTextureImplementation(GLenum buffer) = 0;

        /** Swap the front and back buffers implementation.
          * Pure virtual - must be implemented by Concrate implementations of crGraphicsContext. */
        virtual void swapBuffersImplementation() = 0;


    protected:
        
        crGraphicsContext();

        virtual ~crGraphicsContext();


        ref_ptr<Traits>         m_traits;        
        ref_ptr<crState>          m_state;
        crMutex      m_mutex;
        CRCore::crThread*    m_threadOfLastMakeCurrent;
        
        ref_ptr<crGraphicsThread> m_graphicsThread;
        
};


}

#endif
