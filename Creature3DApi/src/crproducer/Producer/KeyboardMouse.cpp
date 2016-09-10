/* -*-c++-*- Producer - Copyright (C) 2001-2004  Don Burns
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

#include <CRProducer/Producer/Export>
#include <CRProducer/Producer/Types>
#include <CRProducer/Producer/Referenced>
#include <CRProducer/Producer/KeyboardMouse>
#include <CRProducer/Producer/RenderSurface>
#include <CRProducer/Producer/Events>
#include <CRProducer/Producer/Math>

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#endif

#ifdef _X11_IMPLEMENTATION
#include <X11/XKBlib.h>
#endif

#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>

#include <float.h>

#define TIMER_ID        555

namespace Producer {

class KeyboardMouseImplementationBase : public Referenced
{
    public :
        KeyboardMouseImplementationBase(RenderSurface *rs): 
            _rs(rs), 
            _inputArea(NULL),
            _cb(NULL),
            _canceled(false)
        {
            _rs->setUseDefaultEsc(false);
        }

        KeyboardMouseImplementationBase(InputArea *inputArea): 
            _rs(NULL), 
            _inputArea(inputArea),
            _cb(NULL),
            _canceled(false)
        {}

        float mx() { return _mx; }
        float my() { return _my; }
        unsigned int mbutton() { return _mbutton; }

        virtual bool update(KeyboardMouseCallback &, bool ) = 0;
        virtual void startTimer() = 0;
        void setCallback(KeyboardMouseCallback *cb) { _cb = cb; }
        virtual void setAutoRepeatMode( bool ) {}
        virtual bool getAutoRepeatMode() {return true;}

        void cancel()
        {
            _canceled = true;
        }

        virtual void positionPointer( float x, float y )
        {
            if( _inputArea != NULL )
            {
                float minL = FLT_MAX;
                int bestInputRectangle = 0;
                for(unsigned int i=0;i<_inputArea->getNumRenderSurfaces();++i)
                {
                    RenderSurface::InputRectangle ir = _inputArea->getRenderSurface(i)->getInputRectangle();
                    float dx = (x-(ir.left()+ir.width()*0.5f));
                    float dy = (y-(ir.bottom()+ir.height()*0.5f));
                    float dl = sqrt(dx*dx+dy*dy);
                    if (dl<minL)
                    {
                        minL = dl;
                        bestInputRectangle = i;
                    }
                }
                
                RenderSurface* rs = _inputArea->getRenderSurface(bestInputRectangle);
                RenderSurface::InputRectangle ir = rs->getInputRectangle();

                int wx, wy;
                unsigned int w, h;
                rs->getWindowRectangle( wx, wy, w, h );
                
                int ix = int( (float)(w-1)*(x - ir.left())/ir.width() + 0.5f );
                int iy = int( (float)(h-1)*(y - ir.bottom())/ir.height() + 0.5f );

                rs->positionPointer(ix, iy );

            }
            else
            {
                RenderSurface::InputRectangle ir = _rs->getInputRectangle();
                unsigned int ww = _rs->getWindowWidth();
                unsigned int wh = _rs->getWindowHeight();
                int ix = int(((x - ir.left())/ir.width()) * float(ww-1)+ 0.5f );
                int iy = int(((y - ir.bottom())/ir.height()) * float(wh-1)+ 0.5f );
                _rs->positionPointer(ix, iy );
            }
        }

        bool isCanceled()
        {
            return _canceled;
        }

        virtual void init() = 0;
        virtual void fini() = 0;

    protected:

        virtual ~KeyboardMouseImplementationBase() {}

        RenderSurface *_rs;
        InputArea *_inputArea;
        KeyboardMouseCallback *_cb;

        float _mx, _my;
        unsigned int _mbutton;


        void transformMouseMotion( Window win, int wx, int wy)
        {
            if( _inputArea )
            {
                _inputArea->transformMouseMotion( win, wx, wy, _mx, _my);
            }
            else
            {
            
#if defined(_OSX_AGL_IMPLEMENTATION) || defined(_OSX_CGL_IMPLEMENTATION)  
                int positionLeft = 0;
                int positionTop = 0;
                int windowWidth = _rs->getWindowWidth();
                int windowHeight = _rs->getWindowHeight();

                if (win == NULL)
                {
                    if (!_rs->isFullScreen())
                    {
                        // mouse pointer can't be in this window.
                        // so don't update _mx, _my.
                        return;
                    }
                
                    // for full screen render surfaces get the bounds of the complete screen
                    // and check to see if mouse pointer is within this.
                    Display* display = _rs->getDisplay();
                    CGRect theBounds = CGDisplayBounds(*display);
                    if (wx >= theBounds.origin.x && 
                        wy >= theBounds.origin.y &&
                        wx <= (theBounds.origin.x + theBounds.size.width) &&
                        wy <= (theBounds.origin.y + theBounds.size.height))
                    {
                        positionLeft = static_cast<int>(theBounds.origin.x);
                        positionTop = static_cast<int>(theBounds.origin.y);
                        windowWidth = static_cast<int>(theBounds.size.width);
                        windowHeight = static_cast<int>(theBounds.size.height);
                    }
                    else
                    {
                        // mouse pointer can't be in this window
                        // so don't update _mx, _my.
                        return;
                    }
                } 
                else
                {
                    Rect theBounds;
                    GetWindowBounds (win, kWindowContentRgn, &theBounds);

                    positionLeft = theBounds.left;
                    positionTop = theBounds.top;
                    windowWidth = theBounds.right-theBounds.left;
                    windowHeight = theBounds.bottom-theBounds.top;

                }
#else
                int positionLeft = 0;
                int positionTop = 0;
                int windowWidth = _rs->getWindowWidth();
                int windowHeight = _rs->getWindowHeight();
#endif

                RenderSurface::InputRectangle ir = _rs->getInputRectangle();
                _mx = ir.left() + (float(wx-positionLeft)/float(windowWidth))*ir.width();
				if(windowHeight%2 == 1)
					_my = ir.bottom() + (float(windowHeight-(wy-positionTop+1))/float(windowHeight-1))*ir.height()/* + 1.0f / windowHeight*/;
				else
					_my = ir.bottom() + (float(windowHeight-(wy-positionTop))/float(windowHeight))*ir.height();
				//_my = ir.bottom() + (float(windowHeight-(wy-positionTop+1))/float(windowHeight-1))*ir.height() + 1.0f / windowHeight;
            }
        }

        bool _canceled;

};

}

#ifdef _X11_IMPLEMENTATION

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xmu/WinUtil.h>


namespace Producer {

class KeyboardMouseImplementation : public KeyboardMouseImplementationBase
{
    public:
        KeyboardMouseImplementation(InputArea *ia):KeyboardMouseImplementationBase(ia) 
        {
            init();
        }

        KeyboardMouseImplementation(RenderSurface *rs):KeyboardMouseImplementationBase(rs) 
        {
            init();
        }


        bool update( KeyboardMouseCallback &cb, bool block=false)
        {
            if( block )
            {
                do{ 
                    process_event(cb);
                }while( XPending(dpy));
            }
            else
            {
                while( XPending(dpy))
                    process_event(cb);
            }
            cb.idle();

            return true;
        }

        void startTimer()
        {
            struct sigaction act;
            sigset_t ss;
            sigemptyset(&ss);
            act.sa_handler = 0L;
            act.sa_sigaction = s_trap;
            act.sa_mask = ss;
            act.sa_flags = SA_SIGINFO;
            sigaction( SIGALRM, &act, 0L );

            struct itimerval itv;
            itv.it_interval.tv_sec  = 0;
            itv.it_interval.tv_usec = 10000;
            itv.it_value = itv.it_interval;
            setitimer(ITIMER_REAL, &itv, (struct itimerval *)0 );
        }

        virtual void setAutoRepeatMode( bool flag )
        {
            int supported;
            if( flag == false )
                XkbSetDetectableAutoRepeat( dpy, true, &supported );
            else
                XkbSetDetectableAutoRepeat( dpy, false, &supported );

            if( supported == 0 )
            {
                std::cerr << "KeyboardMouse::setAutoRepeateMode("<<flag<<"): Warning- not supported." << std::endl;
            }
            else
                _autoRepeatMode = flag;
        }

        virtual bool getAutoRepeatMode()
        {
            return _autoRepeatMode;
        }

        virtual void init()    
        {
        // This implementation needs to be made more robust for 
        // use with multiple servers and multiple hosts.  Currently
        // assume multiple screens but only one server and host.
            char dpyname[128];
            if( _inputArea != NULL )
                _rs = _inputArea->getRenderSurface(0);

            std::string hostname = _rs->getHostName();
            if( hostname.empty() )
                sprintf( dpyname, ":%d.%d\0",  _rs->getDisplayNum(), _rs->getScreenNum() );
            else
                sprintf( dpyname, "%s:%d.%d\0",  
                        hostname.c_str(), 
                        _rs->getDisplayNum(), _rs->getScreenNum() );
        
            if((dpy = XOpenDisplay(dpyname)) == NULL)
            {
                std::cerr << 
                    "KeyboardMouse() unable to open display " << 
                    XDisplayName("") << std::endl;
                return;
            }

            _inputMask = StructureNotifyMask |
                         KeyPressMask | KeyReleaseMask |
                         PointerMotionMask  | ButtonPressMask | ButtonReleaseMask ;
                         //ButtonMotionMask  | ButtonPressMask | ButtonReleaseMask ;

            if( _inputArea != NULL )
            {
                 for( int i = 0; i < _inputArea->getNumWindows(); i++ )
                 {
                     Window win = _inputArea->getWindow(i);
                     XSelectInput( dpy, win,  _inputMask );
                }
            }
            else
            {
                Window win = _rs->getWindow();
                XSelectInput( dpy, win, _inputMask );
            }
            _mbutton = 0;

            //XGetKeyboardControl( dpy, &_originalKBState );
            //_changeKBStateMask = 0L;
        
            // Possible inconsistency if global mode is set to off.
            // Assumption here is that autoRepeatMode is true if we will 
            // be testing at the client level for repeated keys.
            _autoRepeatMode = true;
        }

        virtual void fini() 
        {
            /*
            if( _changeKBStateMask )
            {
                XKeyboardControl kc;
                if( _changeKBStateMask & KBAutoRepeatMode )
                    kc.auto_repeat_mode = _originalKBState.global_auto_repeat;
                XChangeKeyboardControl( dpy, _changeKBStateMask, &kc );
            }
            */

            // This should only be called from the run() thread if 
            // we are multi-threaded
            /*
             * commenting out these calls as they are suspect 
             * 
            XSync(dpy,0);
            XCloseDisplay(dpy);
            cancel();
            */
        }


    protected:

        virtual ~KeyboardMouseImplementation()
        {}

    private:
        Display *dpy;
        long _inputMask;
        bool _autoRepeatMode;

        class KeymapState {
            public:
                KeymapState()
                {
                    memset( _keys,0, sizeof(_keys));
                }

                KeymapState(char keys[32])
                {
                    memcpy( _keys, keys, sizeof( _keys ));
                }

                KeymapState( const KeymapState &ks )
                {
                    memcpy( _keys, ks._keys, sizeof( _keys ));
                }

                void operator = (char keys[32])
                {
                    memcpy( _keys, keys, sizeof( _keys ));
                }

                bool operator == (char keys[32])
                {
                    unsigned int *lhs = (unsigned int *)&_keys[0];
                    unsigned int *rhs = (unsigned int *)&keys[0];

                    return (lhs[0] == rhs[0] &&
                            lhs[1] == rhs[1] &&
                            lhs[2] == rhs[2] &&
                            lhs[3] == rhs[3] &&
                            lhs[4] == rhs[4] &&
                            lhs[5] == rhs[5] &&
                            lhs[6] == rhs[6] &&
                            lhs[7] == rhs[7] );
                }
            private:
                char _keys[32];
        };

        KeymapState _keymapState;


        void process_event( KeyboardMouseCallback &cb)
        {
            XEvent ev;
            XNextEvent( dpy, &ev );
            switch( ev.type )
            {
                case DestroyNotify:
                    cb.shutdown();
                    break;

                case ConfigureNotify:
                    break;
    
                case MotionNotify :
                  {
                    int  wx, wy;
                    Window win = 0L;
                    if( ev.xmotion.same_screen )
                    {
                        wx = ev.xmotion.x;
                        wy = ev.xmotion.y;
                        win = ev.xmotion.window;
                    }
                    else
                    {
                        Window root;
                        int rx, ry;
                        unsigned int buttons;
                        Window parent = 0L;

                        for( int i= 0; i < ScreenCount(dpy); i++ )
                        {
                            if( XQueryPointer( dpy, RootWindow(dpy, i),
                                  &root, &win, &rx, &ry, &wx, &wy, &buttons) )
                            {
                                parent = win;
                                win = XmuClientWindow( dpy, parent );
                                break;
                            }
                        }
                        XWindowAttributes watt;
                        XGetWindowAttributes(dpy,parent,&watt);
                        wx -= watt.x;
                        wy -= watt.y;
                    }

                    transformMouseMotion(win, wx, wy );

                    if( _mbutton )
                        cb.mouseMotion(_mx, _my);
                    else
                        cb.passiveMouseMotion(_mx, _my);
                    }
                    break;
    
                case ButtonPress :
                    if( ev.xbutton.button == Button4 )
                        cb.mouseScroll(KeyboardMouseCallback::ScrollUp);
                    else if( ev.xbutton.button == Button5)
                        cb.mouseScroll(KeyboardMouseCallback::ScrollDown);
                    else
                    {
                        transformMouseMotion( ev.xmotion.window, ev.xbutton.x, ev.xmotion.y );
                        cb.buttonPress( _mx, _my, ev.xbutton.button );
                        _mbutton |= (1<<(ev.xbutton.button-1));
                    }
                    break;
    
                case ButtonRelease :
                    if( ev.xbutton.button != Button4 && ev.xbutton.button != Button5 )
                    {
                        transformMouseMotion( ev.xmotion.window, ev.xbutton.x, ev.xmotion.y);
                        cb.buttonRelease( _mx, _my, ev.xbutton.button );
                        _mbutton &= ~(1<<(ev.xbutton.button-1));
                    }
                    break;
    
                case KeyPress:
                    {
                        // Check for auto repeated keys and autoRepeat flag;
                        if( _autoRepeatMode == false )
                        {
                            char keys[32];
                            XQueryKeymap( dpy, keys );

                            if( _keymapState == keys ) // We have an autorepeat.
                               break;
                           _keymapState = keys; 
                        }

                        KeyCharacter keychar = Keyboard::getSingleton()->xkeyEventToKeyChar(dpy, ev.xkey);
                        if( keychar != KeyChar_Unknown )
                        {
                            if( keychar & 0xFF00 )
                                cb.specialKeyPress(keychar);
                            else
                                cb.keyPress(keychar);
                        }
                    }
                    break;
    
                case KeyRelease:
                    {
                        if( _autoRepeatMode == false )
                        {
                            char keys[32];
                            XQueryKeymap( dpy, keys );
                           _keymapState = keys; 
                        }

                        KeyCharacter keychar = Keyboard::getSingleton()->xkeyEventToKeyChar(dpy, ev.xkey);
                        if( keychar != KeyChar_Unknown )
                        {
                            if( keychar & 0xFF00 )
                                cb.specialKeyRelease(keychar);
                            else
                                cb.keyRelease(keychar);
                        }
                    }
                    break;
            }
        }

        static void s_trap( int, siginfo_t *, void *) {}
};

}
#endif

#ifdef _WIN32_IMPLEMENTATION

namespace Producer {

class KeyboardMouseImplementation : public KeyboardMouseImplementationBase
{
    public:
        KeyboardMouseImplementation(InputArea *ia):KeyboardMouseImplementationBase(ia)
        { init(); }
            
        KeyboardMouseImplementation(RenderSurface *rs):KeyboardMouseImplementationBase(rs) 
        { init(); }


        bool update( KeyboardMouseCallback &cb, bool block=false)
        {/* no blocking for now
            if( block ) 
            {
                q->waitWhileEmpty();
            }
            */
            //HACK!!! - This is a work-around for background threads that continuously
            // poll the input queue as the above "waitWhileEmpty()" is not fully functional.
            if( block )
                Sleep(16);
            std::vector< Producer::ref_ptr<EventQueue> >::iterator p;
            for( p = qs.begin(); p != qs.end(); p++ )
            {
                EventQueue *q = (*p).get();
                while( ! q->empty() )
                {
                    Event *ev = q->front().get();
                    switch( ev->type() )
                    {
					//case Event::WindowFocus:
					//	{
					//		WindowFocusEvent *wf = dynamic_cast<WindowFocusEvent *>(ev);
					//		if( wf != NULL )
					//		{
					//			cb.windowFocus(wf->focus(),wf->getTransferWin());
					//			// NOt using these now...
					//		}
					//	}
					//	break;
                    case Event::WindowConfig :
                      {
                        WindowConfigEvent *wc = dynamic_cast<WindowConfigEvent *>(ev);
                        if( wc != NULL )
                        {
                            cb.windowConfig(wc->x(),wc->y(),wc->winWidth(),wc->winHeight());
                        }
                      }
                      break;

                    case Event::KeyPress :
                      {
                        KeyPressEvent *kp = dynamic_cast<KeyPressEvent *>(ev);
                        if( kp != NULL )
                        {
                            KeyCharacter kchar = KeyCharacter(kp->key());
                            if( kchar & 0xFF00 )
                                cb.specialKeyPress( kchar );
                            else
                                cb.keyPress( kchar );
                        }
                      }
                      break;

                    case Event::KeyRelease :
                      {
                        KeyReleaseEvent *kr = dynamic_cast<KeyReleaseEvent *>(ev);
                        if( kr != NULL )
                        {
                            KeyCharacter kchar = KeyCharacter(kr->key());
                            if( kchar & 0xFF00 )
                                cb.specialKeyRelease(kchar);
                            else
                                cb.keyRelease(kchar);
                        }
                      }
                      break;
					case Event::InputChar :
						{
							InputCharEvent *ic = dynamic_cast<InputCharEvent *>(ev);
							if( ic != NULL )
							{
								cb.inputChar(ic->getChar());
							}
						}
						break;
                    case Event::ButtonPress :
                      {
                        ButtonPressEvent *bp = dynamic_cast<ButtonPressEvent *>(ev);
						if( bp != NULL )
						{
							transformMouseMotion( bp->window(), bp->mx(), bp->my() );

							_mbutton |= (1<<(bp->button()-1));

							cb.buttonPress( _mx, _my, bp->button() );
						}
                      }
                      break;

                    case Event::ButtonRelease :
                      {
                        ButtonReleaseEvent *br = dynamic_cast<ButtonReleaseEvent *>(ev);
                        if( br != NULL )
                        {
                            transformMouseMotion( br->window(), br->mx(), br->my() );
                            _mbutton &= ~(1<<(br->button()-1));

                            cb.buttonRelease( _mx, _my, br->button() );
                        }
                      }
                      break;
					case Event::DoubleButtonPress :
						{
							DoubleButtonPressEvent *br = dynamic_cast<DoubleButtonPressEvent *>(ev);
							if( br != NULL )
							{
								transformMouseMotion( br->window(), br->mx(), br->my() );
								_mbutton &= ~(1<<(br->button()-1));

								cb.doubleButtonPress ( _mx, _my, br->button() );
							}
						}
						break;

                    case Event::MouseScroll:
                        {
                            MouseScrollEvent *mse = dynamic_cast<MouseScrollEvent *>(ev);
                            if( mse != NULL )
                            {
                                cb.mouseScroll(mse->scrollingMotion() );
                            }
                        }
                        break;

                    case Event::MouseMotion :
                      {
                        MouseMotionEvent *mm = dynamic_cast<MouseMotionEvent *>(ev);
                        if( mm != NULL )
                        {
							transformMouseMotion( mm->window(), mm->mx(), mm->my() );

                            if( _mbutton )
                                cb.mouseMotion(_mx, _my);
                            else
                                cb.passiveMouseMotion(_mx, _my);
                         }
                      }
                      break;
                      
                    case Event::Timer :
                          // Nothing to do, really.
                        break;

                    case Event::Shutdown:
						{
							ShutdownEvent *se = dynamic_cast<ShutdownEvent *>(ev);
							cb.shutdown(se->flg());
						}
                        break;

                    case Event::ButtonMotion :
                        break;
					case Event::WindowRestored:
						{
							WindowRestoredEvent *se = dynamic_cast<WindowRestoredEvent *>(ev);
							cb.windowRestored(se->flg());
						}
						break;
                        
                    }
                    q->pop_front();
                }
                cb.idle();
            }    
            return true;
        }

        virtual void startTimer() 
        {
            ::SetTimer(_rs->getWindow(),TIMER_ID, 10, NULL );
        }

        virtual void init()    
        {
            unsigned int mask = 
                        Event::KeyPressMask   |
                        Event::KeyReleaseMask |
                        Event::ButtonPressMask |
						Event::DoubleButtonPressMask |
                        Event::ButtonReleaseMask |
                        Event::ButtonMotionMask |
                        Event::MouseMotionMask|
                        Event::MouseScrollMask |
                        Event::WindowConfigMask|
                        Event::ShutdownMask|
						Event::InputCharMask|
						Event::WindowRestoredMask;

            if( _inputArea != NULL )
            {
            
                for( unsigned int i = 0; i < _inputArea->getNumRenderSurfaces(); i++ )
                {
                    RenderSurface *rs = _inputArea->getRenderSurface(i);
                    EventQueue *q = rs->selectInput(mask);
                    qs.push_back(q);
                }
            }
            else
            {
                EventQueue *q = _rs->selectInput(mask);
                qs.push_back(q);
            }
               
            _mbutton = 0;
        }

        void fini()
        {
            cancel();
        }


    protected:

        virtual ~KeyboardMouseImplementation() 
        {}

    private:



        std::vector< Producer::ref_ptr<EventQueue> > qs;
};

}

#endif

#if defined(_OSX_AGL_IMPLEMENTATION) || defined(_OSX_CGL_IMPLEMENTATION) 

namespace Producer {

class KeyboardMouseImplementation : public KeyboardMouseImplementationBase
{
    public:
        KeyboardMouseImplementation(InputArea *ia):KeyboardMouseImplementationBase(ia)
        { init(); }
            
        KeyboardMouseImplementation(RenderSurface *rs):KeyboardMouseImplementationBase(rs) 
        { init(); }

        virtual ~KeyboardMouseImplementation() {}

        bool handleMouseEvent(KeyboardMouseCallback &cb, EventRef theEvent)
        {
            
            // mouse down event                         
            Point wheresMyMouse;
            GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(wheresMyMouse), NULL, &wheresMyMouse);

            WindowRef win;
            int fwres = FindWindow(wheresMyMouse, &win);

            if (fwres == inDrag)
            {
                return false;
            }
            else
            {

                EventMouseButton mouseButton;
                GetEventParameter (theEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(mouseButton), NULL, &mouseButton);

                // swap right and middle buttons so that middle button is 2, right button is 3.
                if (mouseButton==3) mouseButton = 2;
                else if (mouseButton==2) mouseButton = 3;

                switch(GetEventKind(theEvent))
                {
                    case kEventMouseDown:
                        if (mouseButton==1) _actual_button1Pressed = true;
                        if (mouseButton==2) _actual_button2Pressed = true;
                        if (mouseButton==3) _actual_button3Pressed = true;
                        updateMouseButtonState(cb);
                        break;
                    case kEventMouseUp:
                        if (mouseButton==1) _actual_button1Pressed = false;
                        if (mouseButton==2) _actual_button2Pressed = false;
                        if (mouseButton==3) _actual_button3Pressed = false;
                        updateMouseButtonState(cb);
                        break;
                    case kEventMouseDragged:
                        break;
                    case kEventMouseMoved:
                        break;
                    default:
                        return false;
                }
                
                updateMouseButtonState(cb);
            
                if( _ox != wheresMyMouse.h || _oy != wheresMyMouse.v )
                {
                    _ox = wheresMyMouse.h;
                    _oy = wheresMyMouse.v;
                    
                    transformMouseMotion(win, _ox, _oy);
                    
                    if( _mbutton )
                         cb.mouseMotion(_mx, _my);
                    else
                         cb.passiveMouseMotion(_mx, _my);
                }            
                return true;
            }
        }
        
        bool handleKeyboardEvent(KeyboardMouseCallback &cb, EventRef theEvent)
        {
            OSStatus status; 

            UInt32 modifierKeys;
            GetEventParameter (theEvent,kEventParamKeyModifiers,typeUInt32, NULL,sizeof(modifierKeys), NULL,&modifierKeys);

            UInt32 dataSize;
            /* jbw check return status so that we don't allocate a huge array */
            status = GetEventParameter( theEvent, kEventParamKeyUnicodes, typeUnicodeText, NULL, 0, &dataSize, NULL );
            if (status != noErr) return false;
            if (dataSize<=1) return false;
            
            UniChar* uniChars = new UniChar[dataSize+1];
            GetEventParameter( theEvent, kEventParamKeyUnicodes, typeUnicodeText, NULL, dataSize, NULL, (void*)uniChars );
           
            KeyCharacter keychar = Keyboard::getSingleton()->akeyEventToKeyChar(static_cast<unsigned long>(uniChars[0]));
            
            switch(GetEventKind(theEvent))
            {
                case kEventRawKeyDown:
                {
                     if( keychar & 0xFF00 )
                        cb.specialKeyPress(keychar);
                     else
                        cb.keyPress(keychar);
                     break;
                }
                case kEventRawKeyRepeat:
                {
                     if( keychar & 0xFF00 )
                        cb.specialKeyPress(keychar);
                     else
                        cb.keyPress(keychar);
                     break;
                }
                case kEventRawKeyUp:
                {                 
                     if( keychar & 0xFF00 )
                        cb.specialKeyRelease(keychar);
                     else
                        cb.keyRelease(keychar);
                     break;
                }
                case kEventRawKeyModifiersChanged:
                     updateMouseButtonState(cb);
                     break;
                case kEventHotKeyPressed:
                     break;
                case kEventHotKeyReleased:
                     break;
                default:
                     break;
            
            }

            delete[] uniChars;

            return true;
        }

        bool update( KeyboardMouseCallback &cb, bool block=false)
        {
            EventRef theEvent;
            EventTargetRef theTarget = GetEventDispatcherTarget();
            while (ReceiveNextEvent(0, NULL, 0,true, &theEvent)== noErr)
            {
                switch(GetEventClass(theEvent))
                {
                    case kEventClassMouse:
                        if (!handleMouseEvent(cb, theEvent)) {}
                            SendEventToEventTarget (theEvent, theTarget);
                        break;
                        
                    case kEventClassKeyboard:
                        handleKeyboardEvent(cb, theEvent);
                        break;
                        
                    default:
                        SendEventToEventTarget (theEvent, theTarget);
                        break;
                }
                        
                ReleaseEvent(theEvent);
            }  
            
            return true;
            
        }

        virtual void startTimer() {}

        virtual void init()    
        {
               
            _actual_button1Pressed = false;
            _actual_button2Pressed = false;
            _actual_button3Pressed = false;

            _computed_button1Pressed = false;
            _computed_button2Pressed = false;
            _computed_button3Pressed = false;

            _previous_button1Pressed = false;
            _previous_button2Pressed = false;
            _previous_button3Pressed = false;

        }

        virtual void fini()
        {
        }


        void updateMouseButtonState(KeyboardMouseCallback &cb)
        {
            _computed_button2Pressed = _actual_button2Pressed;
            _computed_button3Pressed = _actual_button3Pressed;

            if (_actual_button1Pressed)
            {
                UInt32 modKey = GetCurrentKeyModifiers();
                if (modKey & 0x100)
                {
                    _computed_button3Pressed = true;
                    _computed_button1Pressed = false;
                }
                else if (modKey & 0x800)
                {
                    _computed_button2Pressed = true;
                    _computed_button1Pressed = false;
                }
                else
                {
                    _computed_button1Pressed = true;
                }
            }
            else
            {
                _computed_button1Pressed = false;
            }
            
            if (_previous_button1Pressed != _computed_button1Pressed)
            {
                if (_computed_button1Pressed) cb.buttonPress(_mx,_my, 1);
                else cb.buttonRelease(_mx,_my, 1);
                
                _previous_button1Pressed = _computed_button1Pressed;
            }
            
            if (_previous_button2Pressed != _computed_button2Pressed)
            {
                if (_computed_button2Pressed) cb.buttonPress(_mx,_my, 2);
                else cb.buttonRelease(_mx,_my, 2);
                
                _previous_button2Pressed = _computed_button2Pressed;
            }
            
            if (_previous_button3Pressed != _computed_button3Pressed)
            {
                if (_computed_button3Pressed) cb.buttonPress(_mx,_my, 3);
                else cb.buttonRelease(_mx,_my, 3);
                
                _previous_button3Pressed = _computed_button3Pressed;
            }
            
        }

    private:

        short _ox, _oy;
        
        bool _actual_button1Pressed;
        bool _actual_button2Pressed;
        bool _actual_button3Pressed;

        bool _computed_button1Pressed;
        bool _computed_button2Pressed;
        bool _computed_button3Pressed;

        bool _previous_button1Pressed;
        bool _previous_button2Pressed;
        bool _previous_button3Pressed;
};

}

#endif


using namespace Producer;

KeyboardMouse::KeyboardMouse(Producer::RenderSurface *rs) : 
    _implementation(NULL),
    _initialized(false)
{
    _rs = rs;
}

KeyboardMouse::KeyboardMouse(Producer::InputArea *inputArea) : 
    _implementation(NULL),
    _initialized(false) 
{
    _inputArea = inputArea;
} 

KeyboardMouse::~KeyboardMouse()
{
    if( /*threadI*/isRunning() )
    {
        if( _implementation != NULL )
            _implementation->cancel();
    }
    
    // This is the 
    //cancel();

    while (isRunning())
    {
        CRCore::crThread::yieldCurrentThread();
    }
}

bool KeyboardMouse::init( void )
{
    if( _initialized ) 
        return _initialized;

    if( _inputArea != NULL )
    {
        while( _inputArea->isRealized() == false ) 
            _inputArea->waitForRealize();

        _implementation = new KeyboardMouseImplementation(_inputArea.get() );
    }
    else
    {
        _rs->waitForRealize();
        if( _rs->isRealized() == false ) 
            return false;
        _implementation = new KeyboardMouseImplementation(_rs.get() );
    }

    _initialized = true;
    return _initialized;
}

void KeyboardMouse::update(KeyboardMouseCallback &cb, bool block)
{
    if( !_initialized && !init() ) 
        return;
    _implementation->update(cb, block);
}

void KeyboardMouse::setCallback( KeyboardMouseCallback *cb )
{
    _cb = cb;
}

void KeyboardMouse::positionPointer( float x, float y )
{
    if( !_initialized && !init() ) 
            return;
    _implementation->positionPointer( x, y );
}

bool KeyboardMouse::computePixelCoords(float x, float y, RenderSurface* rs, float& pixel_x, float& pixel_y)
{
    if( _inputArea != NULL )
    {
        // first locate which InputRectange is appropriate for specified RenderSurface.
        unsigned int i;
        for(i=0;i<_inputArea->getNumRenderSurfaces();++i)
        {
            if (_inputArea->getRenderSurface(i)==rs) break;
        }
        
        // the RenderSurface isn't in the InputArea list.. therefore cannot make mouse coords to it.
        if (i==_inputArea->getNumRenderSurfaces()) return false;

        // we have a valid InputRectangle
        RenderSurface::InputRectangle ir = _inputArea->getRenderSurface(i)->getInputRectangle();

        // clip out coords which arn't in this InputRectangle.
        if (x<ir.left()) return false;
        if (x>(ir.left()+ir.width())) return false;
        if (y<ir.bottom()) return false;
        if (y>(ir.bottom()+ir.height())) return false;

        float rx = (x-ir.left())/ir.width();
        float ry = (y-ir.bottom())/ir.height();
        
        int wx, wy;
        unsigned int w, h;
        rs->getWindowRectangle( wx, wy, w, h );

        pixel_x = (float)wx + ((float)w)* rx;
        pixel_y = (float)wy + ((float)h)* ry;

        return true;

    }
    else
    {
        // clip out coords that arn't on RenderSurface.
        if (x<-1.0f) return false;
        if (x>1.0f) return false;

        if (y<-1.0f) return false;
        if (y>1.0f) return false;

        float rx = (x+1.0f)*0.5f;
        float ry = (y+1.0f)*0.5f;

        int wx, wy;
        unsigned int w, h;
        _rs->getWindowRectangle( wx, wy, w, h );

        pixel_x = (float)wx + ((float)w)* rx;
        pixel_y = (float)wy + ((float)h)* ry;

        return true;
        
    }
}

void KeyboardMouse::setAutoRepeatMode( bool flag )
{
    if( !_initialized && !init() ) 
            return;
    _implementation->setAutoRepeatMode(flag);
}

bool KeyboardMouse::getAutoRepeatMode()
{
    if( !_initialized && !init() ) 
            return false;
    return _implementation->getAutoRepeatMode();
}



void KeyboardMouse::run()
{
    if( _cb == NULL )
    {
        std::cerr << "KeyboardMouse: internal thread cannot start because\n"
                     "no callback has been specified.  Please specify a \n"
                     "callback with KeyboardMouse::setCallback() first.\n";
        return;
    }

    if( !_initialized  )
    {
        if( !init() )
        {
            std::cerr << "KeyboardMouse::run() - Internal error!" << std::endl;
            return;
        }
    }

    _implementation->setCallback( _cb.get() );

    while( !_implementation->isCanceled() )
    {
        _implementation->update(*_cb,true);
        testCancel();
    }

    _implementation->fini();
}


