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
#include <CRProducer/crEventAdapter.h>
#include <CRCore/crNotify.h>

using namespace CRProducer;

// default to no mouse buttons being pressed.
unsigned int crEventAdapter::m_s_accumulatedButtonMask = 0;

int crEventAdapter::m_s_button = 0;
int crEventAdapter::m_s_modKeyMask = 0;
float crEventAdapter::m_s_Xmin = 0;
float crEventAdapter::m_s_Xmax = 1280;
float crEventAdapter::m_s_Ymin = 0;
float crEventAdapter::m_s_Ymax = 1024;
float crEventAdapter::m_s_mx = 0;
float crEventAdapter::m_s_my = 0;
//int crEventAdapter::m_s_restored = 1;
//__int32 crEventAdapter::m_s_transferwin = 0;
void crEventAdapter::reset()
{
	m_s_accumulatedButtonMask = 0;

	m_s_button = 0;
	m_s_modKeyMask = 0;
	m_s_Xmin = 0;
	m_s_Xmax = 1280;
	m_s_Ymin = 0;
	m_s_Ymax = 1024;
	m_s_mx = 0;
	m_s_my = 0;
	//m_s_restored = 1;
	//m_s_transferwin = 0;
}
crEventAdapter::crEventAdapter():
    crGUIEventAdapter(crGUIEventAdapter::Y_INCREASING_UPWARDS)
{
    m_eventType = NONE;           // adaptor does not encapsulate any events.
    m_key = -1;                   // set to 'invalid' key value.
    m_button = -1;                // set to 'invalid' button value.
    m_mx = -1;                    // set to 'invalid' position value.
    m_my = -1;                    // set to 'invalid' position value.
    m_buttonMask = 0;             // default to no mouse buttons being pressed.
    m_modKeyMask = 0;             // default to no mouse buttons being pressed.
    m_time = 0.0f;                // default to no time has been set.

	//m_restored = 1;
	//m_transferwin = 0;
	m_char = 0;
    copyStaticVariables();

}


void crEventAdapter::copyStaticVariables()
{
    m_buttonMask = m_s_accumulatedButtonMask;
    m_modKeyMask = m_s_modKeyMask;
    m_button = m_s_button;
    m_Xmin = m_s_Xmin;
    m_Xmax = m_s_Xmax;
    m_Ymin = m_s_Ymin;
    m_Ymax = m_s_Ymax;
    m_mx   = m_s_mx;
    m_my   = m_s_my;

	//m_restored = m_s_restored;
	//m_transferwin = m_s_transferwin;
}


void crEventAdapter::setWindowSize(float Xmin, float Ymin, float Xmax, float Ymax)
{
    m_s_Xmin = Xmin;
    m_s_Xmax = Xmax;
    m_s_Ymin = Ymin;
    m_s_Ymax = Ymax;

	//CRCore::notify(CRCore::FATAL)<<"crEventAdapter::setWindowSize(): Xmin = "<<Xmin<<" Ymin = "<<Ymin<<" Xmax = "<<Xmax<<" Ymax = "<<Ymax<<std::endl;
}

//void crEventAdapter::setWindowFocus(int focus, __int32 transferwin)
//{
//	m_s_focus = focus;
//	m_s_transferwin = transferwin;
//
//	//CRCore::notify(CRCore::FATAL)<<"crEventAdapter::setWindowFocus(): focuse = "<<focuse<<" transferwin = "<<transferwin<<std::endl;
//}


void crEventAdapter::setButtonMask(unsigned int buttonMask)
{
    m_s_accumulatedButtonMask = buttonMask;
}


void crEventAdapter::adaptResize(double time, float Xmin, float Ymin, float Xmax, float Ymax)
{
    setWindowSize(Xmin,Ymin,Xmax,Ymax);
    m_eventType = RESIZE;
    m_time = time;
    copyStaticVariables();
}

//void crEventAdapter::windowFocus(double time, int focus, __int32 transferwin )
//{
//	setWindowFocus(focus,transferwin);
//	m_eventType = WINFOCUS;
//	m_time = time;
//	copyStaticVariables();
//}

void crEventAdapter::adaptMouseScroll(double time, Producer::KeyboardMouseCallback::ScrollingMotion sm)
{
    m_time = time;

    if (sm == Producer::KeyboardMouseCallback::ScrollUp)
	    m_eventType = SCROLLUP;
    else if (sm == Producer::KeyboardMouseCallback::ScrollDown)
	    m_eventType = SCROLLDOWN;
    else
	    m_eventType = NONE;

    copyStaticVariables();
}

void crEventAdapter::adaptButtonPress(double time,float x, float y, unsigned int button)
{
    m_time = time;

    m_eventType = PUSH;
    m_button = button-1;

    switch(m_button)
    {
    case(0): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | LEFT_MOUSE_BUTTON; 
	    m_s_button = LEFT_MOUSE_BUTTON;
	    break;
    case(1): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | MIDDLE_MOUSE_BUTTON; 
	    m_s_button = MIDDLE_MOUSE_BUTTON;
	    break;
    case(2): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | RIGHT_MOUSE_BUTTON; 
	    m_s_button = RIGHT_MOUSE_BUTTON;
	    break;
    }

    m_s_mx = x;
    m_s_my = y;

    copyStaticVariables();
}

void crEventAdapter::adaptDoubleButtonPress(double time,float x, float y, unsigned int button)
{
	m_time = time;

	m_eventType = DOUBLECLICK;
	m_button = button-1;

	switch(m_button)
	{
	case(0): 
		m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | LEFT_MOUSE_BUTTON; 
		m_s_button = LEFT_MOUSE_BUTTON;
		break;
	case(1): 
		m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | MIDDLE_MOUSE_BUTTON; 
		m_s_button = MIDDLE_MOUSE_BUTTON;
		break;
	case(2): 
		m_s_accumulatedButtonMask = m_s_accumulatedButtonMask | RIGHT_MOUSE_BUTTON; 
		m_s_button = RIGHT_MOUSE_BUTTON;
		break;
	}

	m_s_mx = x;
	m_s_my = y;

	copyStaticVariables();
}

void crEventAdapter::adaptButtonRelease(double time,float x, float y, unsigned int button)
{
    m_time = time;

    m_eventType = RELEASE;
    m_button = button-1;

    switch(m_button)
    {
    case(0): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask & ~LEFT_MOUSE_BUTTON;
	    m_s_button = LEFT_MOUSE_BUTTON;
	    break;
    case(1): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask & ~MIDDLE_MOUSE_BUTTON; 
	    m_s_button = MIDDLE_MOUSE_BUTTON;
	    break;
    case(2): 
	    m_s_accumulatedButtonMask = m_s_accumulatedButtonMask & ~RIGHT_MOUSE_BUTTON; 
	    m_s_button = RIGHT_MOUSE_BUTTON;
	    break;
    }

    m_s_mx = x;
    m_s_my = y;

    copyStaticVariables();
}

/** method for adapting mouse motion events whilst mouse buttons are pressed.*/
void crEventAdapter::adaptMouseMotion(double time, float x, float y)
{
    
    m_eventType = (m_s_accumulatedButtonMask) ?
                 DRAG :
                 MOVE;

    m_time = time;
    m_s_mx = x;
    m_s_my = y;
    copyStaticVariables();

}


/** method for adapting keyboard events.*/
void crEventAdapter::adaptKeyPress( double time, Producer::KeySymbol key)
{
    m_eventType = KEYDOWN;
    m_time = time;
    m_key = key;
    
    switch(key)
    {
    case(KEY_Shift_L):      m_s_modKeyMask = MODKEY_LEFT_SHIFT | m_s_modKeyMask; break;
    case(KEY_Shift_R):      m_s_modKeyMask = MODKEY_RIGHT_SHIFT | m_s_modKeyMask; break;
    case(KEY_Control_L):    m_s_modKeyMask = MODKEY_LEFT_CTRL | m_s_modKeyMask; break;
    case(KEY_Control_R):    m_s_modKeyMask = MODKEY_RIGHT_CTRL | m_s_modKeyMask; break;
    case(KEY_Meta_L):       m_s_modKeyMask = MODKEY_LEFT_META | m_s_modKeyMask; break;
    case(KEY_Meta_R):       m_s_modKeyMask = MODKEY_RIGHT_META | m_s_modKeyMask; break;
    case(KEY_Alt_L):        m_s_modKeyMask = MODKEY_LEFT_ALT | m_s_modKeyMask; break;
    case(KEY_Alt_R):        m_s_modKeyMask = MODKEY_RIGHT_ALT | m_s_modKeyMask; break;

    case(KEY_Caps_Lock):
        {
            if ((m_s_modKeyMask & MODKEY_CAPS_LOCK)!=0) 
                m_s_modKeyMask = ~MODKEY_CAPS_LOCK & m_s_modKeyMask;
            else 
                m_s_modKeyMask = MODKEY_CAPS_LOCK | m_s_modKeyMask; 
            break;
        }
    case(KEY_Num_Lock):
        {
            if ((m_s_modKeyMask & MODKEY_NUM_LOCK)!=0)
                m_s_modKeyMask = ~MODKEY_NUM_LOCK & m_s_modKeyMask;
            else
                m_s_modKeyMask = MODKEY_NUM_LOCK | m_s_modKeyMask;
            break;
        }
    }        
    copyStaticVariables();

	if(m_key == KEY_F12 && getModKeyMask() & MODKEY_ALT)
		//_asm   int   3   //只是为了让程序崩溃
		__debugbreak();
}

void crEventAdapter::adaptKeyRelease( double time, Producer::KeySymbol key)
{
    // we won't handle this correctly right now.. crGUIEventAdapter isn't up to it
    m_eventType = KEYUP;
    m_time = time;
    m_key = key;

    switch(key)
    {
        case(KEY_Shift_L):      m_s_modKeyMask = ~MODKEY_LEFT_SHIFT & m_s_modKeyMask; break;
        case(KEY_Shift_R):      m_s_modKeyMask = ~MODKEY_RIGHT_SHIFT & m_s_modKeyMask; break;
        case(KEY_Control_L):    m_s_modKeyMask = ~MODKEY_LEFT_CTRL & m_s_modKeyMask; break;
        case(KEY_Control_R):    m_s_modKeyMask = ~MODKEY_RIGHT_CTRL & m_s_modKeyMask; break;
        case(KEY_Meta_L):       m_s_modKeyMask = ~MODKEY_LEFT_META & m_s_modKeyMask; break;
        case(KEY_Meta_R):       m_s_modKeyMask = ~MODKEY_RIGHT_META & m_s_modKeyMask; break;
        case(KEY_Alt_L):        m_s_modKeyMask = ~MODKEY_LEFT_ALT & m_s_modKeyMask; break;
        case(KEY_Alt_R):        m_s_modKeyMask = ~MODKEY_LEFT_ALT & m_s_modKeyMask; break;
    }        

    copyStaticVariables();
}

void crEventAdapter::adaptInputChar( double t, wchar_t c)
{
	m_eventType = INPUTCHAR;
	m_time = t;
	m_char = c;

	copyStaticVariables();
}

/** method for adapting frame events, i.e. iddle/display callback.*/
void crEventAdapter::adaptFrame(double time)
{
    m_eventType = FRAME;
    m_time = time;

    copyStaticVariables();
}

//void crEventAdapter::adaptWindowRestored(double t, int flg)
//{
//	m_eventType = WINRESTORED;
//	m_time = t;
//	m_restored = flg;
//
//	copyStaticVariables();
//}