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
#ifndef CRRODUCER_ProducerEventAdapter
#define CRRODUCER_ProducerEventAdapter 1

#include <CRProducer/crExport.h>

#include <CRProducer/crGUIEventAdapter.h>
#include <CRProducer/Producer/KeyboardMouse>


namespace CRProducer {

class CRPRODUCER_EXPORT crEventAdapter : public crGUIEventAdapter
{

    public:
        crEventAdapter();
        virtual ~crEventAdapter() {}

        /** Get the EventType of the GUI event.*/
        inline virtual EventType getEventType() const { return m_eventType; }

		inline void setKey(int key) { m_key = key; }

        /** key pressed, return -1 if inappropriate for this event. */
        inline virtual int getKey() const { return m_key; }
		inline virtual wchar_t getChar() const { return m_char; }

        /** button pressed/released, return -1 if inappropriate for this event.*/
        inline virtual int getButton() const { return m_button; }

        /** window minimum x. */
        inline virtual float getXmin() const  { return m_Xmin; }

        /** window maximum x. */
        inline virtual float getXmax() const  { return m_Xmax; }

        /** window minimum y. */
        inline virtual float getYmin() const { return m_Ymin; }

        /** window maximum y. */
        inline virtual float getYmax() const { return m_Ymax; }

		inline void setX(float x) { m_mx = x; }

        /** current mouse x position.*/
        inline virtual float getX() const { return m_mx; }

		inline void setY(float y) { m_my = y; }
        /** current mouse y position.*/
        inline virtual float getY() const { return m_my; }

		inline void setButtonMak(unsigned int mask) { m_buttonMask = mask; }
        /** current mouse button state */
        inline virtual unsigned int getButtonMask() const { return m_buttonMask; }

        /** time in seconds of event. */
        inline virtual double time() const { return m_time; }

        inline virtual unsigned int getModKeyMask() const { return m_modKeyMask; }

		//inline virtual int getRestored() const { return m_restored; }
		//inline virtual __int32 getTransferwin() const { return m_transferwin; }

        /** static method for setting window dimensions.*/
        static void setWindowSize(float Xmin, float Ymin, float Xmax, float Ymax);

        /** static method for setting button state.*/
        static void setButtonMask(unsigned int buttonMask);

		//static void setWindowFocus(int focuse, __int32 transferwin);

        /** method for adapting resize events. */
        void adaptResize(double t, float Xmin, float Ymin, float Xmax, float Ymax);

        /** method for adapting mouse scroll wheel events. */
	    void adaptMouseScroll(double t, Producer::KeyboardMouseCallback::ScrollingMotion sm);

        /** method for adapting mouse motion events whilst mouse buttons are pressed.*/
        void adaptMouseMotion(double t, float x, float y);

        void adaptButtonPress(double t,float x, float y, unsigned int button);
		void adaptDoubleButtonPress(double t,float x, float y, unsigned int button);
        void adaptButtonRelease(double t,float x, float y, unsigned int button);

        /** method for adapting keyboard events.*/
        void adaptKeyPress( double t, Producer::KeySymbol key);

        void adaptKeyRelease( double t, Producer::KeySymbol key);
		
		void adaptInputChar( double t, wchar_t c);

        /** method for adapting frame events, i.e. idle/display callback.*/
        void adaptFrame(double t);

		//void windowFocus(double time, int focus, __int32 transferwin );
		//void adaptWindowRestored(double t, int flg);

		void copyStaticVariables();
		static void reset();
    protected:
    

        EventType m_eventType;
        int m_key;
        int m_button;
        float m_Xmin,m_Xmax;
        float m_Ymin,m_Ymax;
        float m_mx;
        float m_my;
        unsigned int m_buttonMask;
        unsigned int m_modKeyMask;
        double m_time;

		//int m_restored;
		wchar_t m_char;
		//__int32 m_transferwin;
    public:

        // used to accumulate the button mask state, it represents
        // the current button mask state, which is modified by the
        // adaptMouse() method which then copies it to value m_buttonMask
        // which required the mouse buttons state at the time of the event.
        static unsigned int m_s_accumulatedButtonMask;
        
	    // used to store current button value
	    static int m_s_button;

        // used to store window min and max values.
        static float m_s_Xmin;
        static float m_s_Xmax;
        static float m_s_Ymin;
        static float m_s_Ymax;
        static float m_s_mx;
        static float m_s_my;
        static int m_s_modKeyMask;

		//static int m_s_restored;
		//static __int32 m_s_transferwin;
};

}

#endif
