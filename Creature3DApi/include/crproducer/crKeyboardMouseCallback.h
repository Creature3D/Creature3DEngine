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
#ifndef CRPRODUCER_EVENTCALLBACK
#define CRPRODUCER_EVENTCALLBACK 1

#include <CRCore/thread/crMutex.h>

#include <CRProducer/crExport.h>      // To disable MSVC warnings

#include <CRProducer/Producer/RenderSurface> // For definition of KeySymbol
#include <CRProducer/Producer/KeyboardMouse>

#include <CRProducer/crEventAdapter.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/crTimer.h>

namespace CRProducer {


class CRPRODUCER_EXPORT crKeyboardMouseCallback : public Producer::KeyboardMouseCallback
{
    public:
        crKeyboardMouseCallback(Producer::KeyboardMouse* keyboardMouse, bool &done, bool escapeKeySetsDone=true) :
            Producer::KeyboardMouseCallback(),
            m_keyboardMouse(keyboardMouse),
            m_mx(0.0f),m_my(0.0f),m_mbutton(0),
            m_done(done),
            m_escapeKeySetsDone(escapeKeySetsDone)
			//m_alt_pressed(0.0)
            {}

        virtual ~crKeyboardMouseCallback() {}

        // override crKeyboardMouseCallback methods.
        virtual void mouseScroll( Producer::KeyboardMouseCallback::ScrollingMotion sm );
        virtual void mouseMotion( float mx, float my);
        virtual void passiveMouseMotion( float mx, float my);
        virtual void mouseWarp( float mx, float my);

        virtual void buttonPress( float mx, float my, unsigned int mbutton );
        virtual void doubleButtonPress( float mx, float my, unsigned int mbutton);
        virtual void buttonRelease( float mx, float my, unsigned int mbutton );

        virtual void keyPress( Producer::KeyCharacter key );
        virtual void keyRelease( Producer::KeyCharacter key );

        virtual void specialKeyPress( Producer::KeyCharacter key);
        virtual void specialKeyRelease( Producer::KeyCharacter key);

		virtual void inputChar( wchar_t c );

        virtual void windowConfig( int x, int y, unsigned int width, unsigned int height );
		//virtual void windowFocus( int focus, Producer::Window transferwin );
        
		virtual void shutdown(int flg);
		virtual void windowRestored(int flg);

        void setEscapeSetDone(bool esc) { m_escapeKeySetsDone = esc; }
        bool getEscapeSetDone() const { return m_escapeKeySetsDone; }


        // local methods and members
        typedef std::vector< CRCore::ref_ptr<crEventAdapter> > EventQueue;

        double getEventQueue(EventQueue& queue);
		double copyEventQueue(EventQueue& queue);
		double setEventQueue(EventQueue& queue);
		double appendEventQueue(EventQueue& queue);

        bool done() { return m_done; }
        float mx()  { return m_mx; }
        float my()  { return m_my; }
        unsigned int mbutton()  { return m_mbutton; }
        
        void setStartTick(CRCore::Timer_t tick) { m_startTick = tick; }
        CRCore::Timer_t getStartTick() const { return m_startTick; }

        double getTime() { return m_timer.delta_s(m_startTick,m_timer.tick()); }
        
        Producer::KeyboardMouse* getKeyboardMouse() { return m_keyboardMouse; }
        const Producer::KeyboardMouse* getKeyboardMouse() const { return m_keyboardMouse; }
         
        crEventAdapter* createEventAdapter();
      
		void clear();
    protected:
    
        Producer::KeyboardMouse* m_keyboardMouse;
        float m_mx, m_my;
        unsigned int m_mbutton;
        bool &m_done;
        bool m_escapeKeySetsDone;

        //double m_alt_pressed;//for catch alt+tab

        CRCore::Timer_t    m_startTick;
        CRCore::Timer      m_timer;
        CRCore::crMutex m_eventQueueMutex;
        EventQueue      m_eventQueue;
};

}

#endif
