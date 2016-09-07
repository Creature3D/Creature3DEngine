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


#include <CRCore/thread/crScopedLock.h>
#include <CRProducer/Producer/RenderSurface>

#ifdef _X11_IMPLEMENTATION
#include <stdio.h>
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/cursorfont.h>


using namespace Producer; 

// HP, SGI and SUN don't implement glXGetProcAddressARB...
#if defined (__sgi) || defined (sun) || defined( __hpux ) 
#include <dlfcn.h>
void (*glXGetProcAddressARB(const GLubyte *procName))(void)
{
    static void *handle = dlopen( (const char *)0L, RTLD_LAZY );
    return (void (*)(void))dlsym(handle,(const char *)procName);
}
#endif

#if (defined(__APPLE__) || defined(macintosh)) & defined (GLX_VERSION_1_4)
#define glXGetProcAddressARB glXGetProcAddress
#endif


#if (defined(__APPLE__) || defined(macintosh)) || defined(_AIX)
  #define USE_LOCAL_glXGetFBConfigFromVisual 
#endif

#ifdef USE_LOCAL_glXGetFBConfigFromVisual
static GLXFBConfig _glXGetFBConfigFromVisual(::Display *dpy, XVisualInfo *visinfo)
{
    int screen = visinfo->screen;
    int nelements;
    GLXFBConfig *configs = glXGetFBConfigs(dpy, screen, &nelements);
    for( int i = 0; i < nelements; i++ )
    {
        int visual_id;
        if( glXGetFBConfigAttrib( dpy, configs[i], GLX_VISUAL_ID, &visual_id ) == 0 )
        {
            if( (unsigned int)visual_id == visinfo->visualid )
                return configs[i];
        }
    }
    return NULL;
}
#endif

unsigned int RenderSurface::getNumberOfScreens()
{
    if( _numScreens == UnknownAmount )
    {
        Display *dpy = XOpenDisplay("");
        if( dpy == NULL )
        {
            std::cerr << "Unable to open display \"" << XDisplayName( ":0.0") << "\".  Is the DISPLAY environmental variable set?\n";
            return 0;
        }
        _numScreens = ScreenCount(dpy); 
        XCloseDisplay(dpy);
    }
    return _numScreens;
}

static CRCore::crMutex realize_mutex;

bool RenderSurface::realize( VisualChooser *vc, GLContext sharedGLContext )
{
    if( _realized ) return _realized;

    CRCore::ScopedLock<CRCore::crMutex> lock(realize_mutex);

    if( vc != NULL )
        _visualChooser = vc;


    if( sharedGLContext != 0L )
        _sharedGLContext = sharedGLContext;
    else
        _sharedGLContext = _globallySharedGLContext;

    if( _init() == false )
        return false;

    if( _drawableType == DrawableType_Window )
    {
        if( _useConfigEventThread )
        {
            startThread();
            _threadReady->block();
        }

        XMapWindow( _dpy, _win );
        Window temp = _win;
        XSetWMColormapWindows( _dpy,  _win, &temp, 1);
    }
    makeCurrent();
#if 0
    // NVidia 1.0-4496 driver breaks (once again), glXGetVideoSync.  This problem was seen
    // earlier on laptops (Geforce4Go), but seems to be present on desktops as
    // wells now.  A bug will be filed with NVidia again.
    //
    // glXWaitVideoSyncSGI is used for synchronizing the CPU bound process to
    // video retrace signal and is ultimately not necessary, but would provide
    // consistent frame rates in the CPU if it was present.
    //
    // Note that testVSync hangs if the window has not been mapped.  If the window
    // has been mapped, glXGetVideoSync() fails
#endif
    testVSync();

    if( _overrideRedirectFlag == true )
    {
        XFlush(_dpy);
        XSync(_dpy,0);
        usleep(500000); // Hack to allow window manager to catch up 
        XSetInputFocus(  _dpy, _win, RevertToParent, CurrentTime );
        XFlush(_dpy);
        XSync(_dpy,0);
    }

    _realized = true;
 
    std::vector <Producer::ref_ptr<Callback> >::iterator p;
    for( p = _realizeCallbacks.begin(); p != _realizeCallbacks.end(); p++ )
    {
        if( (*p).valid() )
            (*(*p).get())( *this );
    }
    _realizeCallbacks.clear();
    _realizeBlock->release();

    return _realized;
}
void RenderSurface::setParentWindow( Window parent )
{
	_parent = parent;
}
bool RenderSurface::_init()
{
	if(_inited) return true;
    char dpyname[128];
    bool _dpy_created_locally = false;

    if( _hostname.empty() )
        sprintf( dpyname, ":%d.%d\0",  _displayNum, _screen );
    else
        sprintf( dpyname, "%s:%d.%d\0", _hostname.c_str(), _displayNum, _screen );

    // Set Display
    if( _dpy == NULL )
    {
        if( (_dpy = XOpenDisplay( dpyname )) == (Display *)0L )
        {
            std::cerr << "RenderSurface::realize() - unable to open display \""
                <<XDisplayName( dpyname) << "\"" << std::endl;
            return false;
        }
        _dpy_created_locally = true;
    }

    _screenWidth = DisplayWidth(_dpy, _screen );
    _screenHeight = DisplayHeight(_dpy, _screen );

        // Query for GLX extension
    int erb, evb;
    if( glXQueryExtension( _dpy, &erb, &evb)  == False )
    {
        std::cerr << "RenderSurface::realize() " << 
                    XDisplayName( dpyname) <<
                    " has no GLX extension." << std::endl;
         if( _dpy_created_locally )
         {
             XCloseDisplay( _dpy );
             _dpy = 0;
         }
         return false;
    }
    
    if( _visualInfo == NULL )
    {
    // If Window was already defined, lets grab that visual
        if( _win != 0 )
        {
            XWindowAttributes watt;
            XGetWindowAttributes( _dpy, _win, &watt );
            XVisualInfo temp;
            temp.visualid = XVisualIDFromVisual(watt.visual);
            int n;
            _visualInfo = XGetVisualInfo( _dpy, VisualIDMask, &temp, &n );
        }
        else
        {
            if( !_visualChooser.valid() )
            {
                ref_ptr<VisualChooser> lvc = new VisualChooser;
                lvc->setSimpleConfiguration(_drawableType != DrawableType_PBuffer);
                _visualInfo = lvc->choose(_dpy, _screen);
            }
            else
            {
                _visualInfo = _visualChooser->choose(_dpy,_screen);
            }
        }
    }

    if( _visualInfo == NULL )
    {
        if( _dpy_created_locally )
        {
            XCloseDisplay( _dpy );
            _dpy = 0;
        }
        return false;
    }


    if((_glcontext = glXCreateContext( _dpy, _visualInfo, _sharedGLContext, GL_TRUE ))==(GLXContext) 0L)
    {
        std::cerr << "RenderSurface:realize() - Can't create GLX Context." << std::endl;
        if( _dpy_created_locally )
        {
            XCloseDisplay( _dpy );
            _dpy = 0L;
        }
        return false;
    }

    // Initialize globallySharedGLContext if global sharing is requested and 
    // the global variable is still NULL.
    // Caveat is if the user attempted to share GL contexts already through realize()
    if( _shareAllGLContexts == true && 
            _globallySharedGLContext == 0L &&
            _sharedGLContext == 0L )
    {
        _globallySharedGLContext = _sharedGLContext = _glcontext;
    }

    if( _win == 0 )
    {
        if( _drawableType == DrawableType_Window )
        {
            if( _parent == (Window)0L )
            {
                _parent = RootWindow( _dpy, _screen );
                _mayFullScreen = true;
            }
            else
            {
                _mayFullScreen = false;
            }
    
            XWindowAttributes watt;
            XGetWindowAttributes( _dpy, _parent, &watt );
            _parentWindowHeight = watt.height;
    
            if( _windowWidth == UnknownDimension || (_mayFullScreen && _isFullScreen))
            {
                //if( _useCustomFullScreen )
                //{
                //    _windowX = _customFullScreenOriginX;
                //    _windowWidth = _customFullScreenWidth;
                //}
                //else    
                //{
                    _windowX = 0;
                    _windowWidth = watt.width;
                //}
                _decorations = false;
            }
    
            if( _windowHeight == UnknownDimension || (_mayFullScreen && _isFullScreen))
            {
                //if( _useCustomFullScreen )
                //{
                //    _windowY = _customFullScreenOriginY;
                //    _windowHeight = _customFullScreenHeight;
                //}
                //else    
                //{
                    _windowY = 0;
                    _windowHeight = watt.height;
                //}
                _decorations = false;
            }
    
            XSetWindowAttributes swatt;
            swatt.colormap = XCreateColormap( _dpy, RootWindow( _dpy, _screen ),
                                            _visualInfo->visual, AllocNone);
        //swatt.colormap = DefaultColormap( _dpy, 0 );
            swatt.background_pixel = 0;
            swatt.border_pixel = 0;
            swatt.event_mask =  0;
            unsigned long mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;
            if( _overrideRedirectFlag == true )
            {
                swatt.override_redirect = true;
                mask |= CWOverrideRedirect;
            }
            _win = XCreateWindow( _dpy, _parent,
                                  _windowX,
                                  _parentWindowHeight - (_windowHeight + _windowY),
                                  _windowWidth, _windowHeight, 0,
                                  _visualInfo->depth, InputOutput,
                                  _visualInfo->visual, mask, &swatt );
    
            // This positions the window at _windowX, _windowY
            XSizeHints sh;
            sh.flags = 0;
            sh.flags |= USSize;
            sh.flags &= 0x7;
            sh.flags |= USPosition;
            sh.flags &= 0xB;
            sh.x = _windowX;
            sh.y = _parentWindowHeight - (_windowHeight + _windowY);
            sh.width  = _windowWidth;
            sh.height = _windowHeight;
            XSetStandardProperties( _dpy, _win, _windowName.c_str(), _windowName.c_str(), None, 0, 0, &sh);
    
            // This turns off border decorations
            if( _decorations == false )
                _setBorder(_decorations);

            // Create  default Cursor
            {
                _defaultCursor = XCreateFontCursor( _dpy, XC_left_ptr );
            }
    
            // Create Null Cursor
            {
                Pixmap pixmap;
                static char buff[2] = {0,0};
                static XColor ncol = {0,0,0,0,DoRed|DoGreen|DoBlue,0};
    
                pixmap = XCreateBitmapFromData( _dpy, DefaultRootWindow(_dpy), buff, 1, 1);
               _nullCursor = XCreatePixmapCursor( _dpy, pixmap, pixmap, &ncol, &ncol, 0, 0 );
            }
            _setCursorToDefault();
        }
        else if( _drawableType == DrawableType_PBuffer )
        {
#ifdef GLX_VERSION_1_3
#ifdef USE_LOCAL_glXGetFBConfigFromVisual 
            GLXFBConfig fbconfig = _glXGetFBConfigFromVisual( _dpy, _visualInfo );
#else
            GLXFBConfig fbconfig = glXGetFBConfigFromVisualSGIX( _dpy, _visualInfo );
#endif
             if( _windowWidth == UnknownDimension )
                 _windowWidth = _screenWidth;

             if( _windowHeight == UnknownDimension )
                 _windowHeight = _screenHeight;
                                                                                                                              
            std::vector <int> attributeList(_user_pbattr);
            attributeList.push_back( GLX_PBUFFER_WIDTH );
            attributeList.push_back( _windowWidth );
            attributeList.push_back( GLX_PBUFFER_HEIGHT );
            attributeList.push_back( _windowHeight );
            attributeList.push_back( 0L );

            _win = glXCreatePbuffer(_dpy, fbconfig, &attributeList.front() );
#else
            _win = 0L;
            return false;
#endif
        }
    }
    else
    {
        XWindowAttributes watt;
        XGetWindowAttributes( _dpy, _win, &watt );
        _windowX = watt.x;
        _windowY = watt.y;
        _windowWidth = watt.width;
        _windowHeight = watt.height;

        // We can't know if the imposed window may resize or not
        _mayFullScreen = false;
        
        // We must not check for events.  Leave that up to the externally created window
        _checkOwnEvents = false;

        // Set Screen Number
        _screen = -1;
        for( int i = 0; i < ScreenCount(_dpy); i++ )
        {
            if( watt.screen == ScreenOfDisplay(_dpy,i) )
            {
                _screen = i;
                break;
            }
        }
        if( _screen == -1 )
        {
            // A warning might be appropriate here
            _screen = 0;
        }
    }


    if( _windowHeight == _screenHeight && _windowWidth == _screenWidth )
        _isFullScreen = true;
    //else if( _useCustomFullScreen &&
    //        _windowHeight == _customFullScreenHeight && 
    //        _windowWidth == _customFullScreenWidth )
    //    _isFullScreen = true;
    else
        _isFullScreen = false;

    if( !_useConfigEventThread && _drawableType == DrawableType_Window )
        XSelectInput( _dpy, _win, ExposureMask | StructureNotifyMask );

    XFlush( _dpy );
    XSync( _dpy, 0 );
	_inited = true;
    return true;
}

bool RenderSurface::makeCurrent( void ) const
{
    if( _readDrawableRenderSurface != 0L && _readDrawableRenderSurface->isRealized())
    {
#ifdef GLX_VERSION_1_3
        glXMakeContextCurrent(_dpy, _win, _readDrawableRenderSurface->getWindow(), _glcontext );
#else
       glXMakeCurrent( _dpy, _win, _glcontext );
#endif
    }
    else
        glXMakeCurrent( _dpy, _win, _glcontext );
    return true;
}

bool RenderSurface::releaseCurrent( void )
{
	glXMakeCurrent( _dpy, 0L, 0L );
	return true;
}

void RenderSurface::swapBuffers( void )
{
    if( !_realized ) return;

    //makeCurrent();
    glXSwapBuffers( _dpy, _win );
    if( _checkOwnEvents && _useConfigEventThread == false )
    {
        while( XPending(_dpy) )
           _checkEvents(_dpy);
    }
}


void RenderSurface::sync( int divisor )
{
    if( !_realized ) return;

    // In spite of repeated efforts to get Nvidia and the ARB to 
    // Communicate on this issue, the ARB definition of glXWaitVideoSyncSGI()
    // is ambiguous and NVidia's implementation of it favors the least
    // likely interpretation.
    // So, it is necessary to call GetVideoSync before EVERY WaitVideoSync
    // if you want WaitVideoSync to work correctly when divisor = 1, on
    // NVidia.  On SGI, the penalty is an extra call to GetVideoSync 
    if( __glxGetVideoSyncSGI != NULL && __glxWaitVideoSyncSGI != NULL )
    {
        // This may be fixed now...
        // 5/16/03  Keep this in.  If divisor is anything other than
        // 0 this still does not work
        __glxGetVideoSyncSGI( &_frameCount );
        __glxWaitVideoSyncSGI( divisor, 0, &_frameCount );
    }
    // else do nothing.  Warning already printed in testVSync.
}


void RenderSurface::testVSync( void )
{
    unsigned int frame = 0;
    __glxGetRefreshRateSGI = reinterpret_cast<int (*)(unsigned int *)>
        (glXGetProcAddressARB((GLubyte *)"glXGetRefreshRateSGI" ));
    __glxWaitVideoSyncSGI = reinterpret_cast<int(*)(int,int,unsigned int *)>
        (glXGetProcAddressARB((GLubyte *)"glXWaitVideoSyncSGI" ));
    __glxGetVideoSyncSGI = reinterpret_cast<int(*)(unsigned int *)>
        (glXGetProcAddressARB((GLubyte * )"glXGetVideoSyncSGI"));

    if( __glxGetRefreshRateSGI != NULL )
    __glxGetRefreshRateSGI(&_refreshRate);

    std::string emsg(
        "Warning:  Producer::RenderSurface, under the X11 environment\n"
        "          has tested the glXWaitVideoSyncSGI extension and found it\n"
        "          to not be working properly.  This does not necessarily mean\n"
        "          that glXSwapBuffers will not block on VSYNC, but it does mean\n"
        "          that the CPU bound application will not be able to synchronize\n"
        "          with the graphics video retrace signal.  Most operations will work\n"
        "          fine.");
        
    // Prior reference to a bug here in the nVidia drivers is moot.
    // Bug now seems to be fixed.
    if( __glxGetVideoSyncSGI != NULL && __glxWaitVideoSyncSGI != NULL )
    {
        __glxGetVideoSyncSGI(&frame);
        if( frame != 0 )
        {
            __glxWaitVideoSyncSGI( 1, 0, &frame );
            _frameCount = frame;
            __glxWaitVideoSyncSGI( 1, 0, &frame );
            __glxWaitVideoSyncSGI( 1, 0, &frame );

            if( frame != (_frameCount + 2) )
                std::cerr << emsg << std::endl;
        }
        else
            std::cerr << emsg << std::endl;
    }
    else
        std::cerr << emsg << std::endl;

    _frameCount = frame;
}

bool RenderSurface::_checkEvents( Display *dpy )
{
    bool ret = true;
    XEvent ev;
    XNextEvent( dpy, &ev );

// Window manager will reparent and reset the x,y location relative
// to the window manager's border window.  On a reparent, we set 
// this flag.  Seems to work but .....
static bool _ignore_resize_flag_hack = false;
    switch( ev.type )
    {
        case Expose :
          break;

        case ConfigureNotify :
           if( !_isFullScreen && !_ignore_resize_flag_hack )
           {
                _windowWidth = ev.xconfigure.width;
                _windowHeight = ev.xconfigure.height;
                _windowX = ev.xconfigure.x;
                _windowY = DisplayHeight(dpy,_screen) - (ev.xconfigure.y+_windowHeight);

                if( _bindInputRectangleToWindowSize == true )
                    _inputRectangle.set( 0.0, float(_windowWidth), 0.0, float (_windowHeight));

           }
_ignore_resize_flag_hack = false;
        break;

        case GravityNotify :
        break;

        case MapNotify :
          {
             XWindowAttributes watt;
            do
                XGetWindowAttributes(dpy, _win, &watt );
            while( watt.map_state != IsViewable );

            XSetInputFocus( dpy, _win, RevertToNone, CurrentTime );
            XFlush(dpy); XSync(dpy,0);
          }
            break;

        case UnmapNotify :
            break;

        case ReparentNotify:
            _ignore_resize_flag_hack = true;    
            break;

        case DestroyNotify :
            _realized =  false;
            ret = false;
            break;
    }
    return ret;
}

void RenderSurface::run( void )
{
    Display *ldpy;
    char dpyname[128];
    if( _hostname.empty() )
        sprintf( dpyname, ":%d.%d\0",  _displayNum, _screen );
    else
        sprintf( dpyname, "%s:%d.%d\0", _hostname.c_str(), _displayNum, _screen );
    if( (ldpy = XOpenDisplay( dpyname )) == (Display *)0L )
    {
        std::cerr << "RenderSurface() : Reconfigure event thread - "
                     "Unable to open display \"" 
                  << XDisplayName( dpyname ) << "\"" << std::endl;
        //return (void *)1;
        return;
    }

    const unsigned long mask = ExposureMask | StructureNotifyMask;
    XSelectInput( ldpy, _win,  mask );

    if( _threadReady != 0L )
        _threadReady->block();

    for( ;; )
    {
#if 0
        fd_set rfds;
        int fd = ConnectionNumber(ldpy);
        FD_ZERO(&rfds);
        FD_SET( fd, &rfds );
        if( select(fd+1,&rfds,0,0,0) < 0 )
            testCancel();
#endif
        do
        {
            _checkEvents(ldpy);
            testCancel();
        } while(XPending(ldpy));
    }

    XCloseDisplay( ldpy );
}



void RenderSurface::fullScreen( bool flag )
{
    if( _drawableType != DrawableType_Window )
        return;

    if( flag == _isFullScreen ) 
        return;

    if( flag )
    {
        if( _mayFullScreen == false || !_realized ) 
        {
            if( !_realized )
                std::cerr << "RenderSurface::fullScreen() - Cannot set full screen before realization\n";
            return;
        }

        _isFullScreen = true;
        _setBorder(false);

        //if( _useCustomFullScreen )
        //{
        //    XMoveResizeWindow( _dpy, _win, 
        //            _customFullScreenOriginX,
        //            _screenHeight - (_customFullScreenOriginY + _customFullScreenHeight),
        //            _customFullScreenWidth,
        //            _customFullScreenHeight );
        //}
        //else
        //{
            XMoveResizeWindow( _dpy, _win, 0, 0, _screenWidth, _screenHeight );
        //}

        XFlush(_dpy); XSync(_dpy,0);
        usleep(100000); // Hack to allow window manager to catch up 
    }
    else
    {
        _isFullScreen = false;
        _resizeWindow();
        _setBorder(_decorations);
        XFlush(_dpy); XSync(_dpy,0);
        usleep(100000); // Hack to allow window manager to catch up 
    }
}

void RenderSurface::_resizeWindow()
{
    if( _drawableType != DrawableType_Window )
        return;

    XMoveResizeWindow( _dpy, _win, 
                    _windowX, 
                    _screenHeight - (_windowY + _windowHeight), 
                    _windowWidth, _windowHeight );
    XFlush(_dpy); XSync(_dpy,0);
    usleep(100000); // Hack to allow window manager to do its thing.
}

void RenderSurface::_setBorder( bool flag )
{
    if( _drawableType != DrawableType_Window )
        return;

    Atom atom;
    if( (atom = XInternAtom( _dpy, "_MOTIF_WM_HINTS", 0 )) != None )
    {
// Hack for sending 64 bit atom to Xserver
#if defined( _MIPS_SIM) && (_MIPS_SIM == _MIPS_SIM_ABI64) || defined ( __ia64 ) || defined (__amd64 ) || defined(__x86_64__)
        struct {
            CARD32 flags0;
            CARD32 flags1;
            CARD32 functions0;
            CARD32 functions1;
            CARD32 decorations0;
            CARD32 decorations1;
            INT32 input_mode0;
            INT32 input_mode1;
            CARD32 status0;
            CARD32 status1;
        } wmHints;

#if defined( __ia64 ) || defined (__amd64)  || defined (__x86_64__)
        wmHints.flags0       = (1L << 1);
        wmHints.functions0   = 0;
        wmHints.decorations0 = flag;
        wmHints.input_mode0  = 0;

#else
        wmHints.flags1       = (1L << 1);
        wmHints.functions1   = 0;
        wmHints.decorations1 = flag;
        wmHints.input_mode1  = 0;
#endif

#else

        struct {
            CARD32 flags;
            CARD32 functions;
            CARD32 decorations;
            INT32 input_mode;
            CARD32 status;
        } wmHints;

        wmHints.flags       = (1L << 1);
        wmHints.functions   = 0;
        wmHints.decorations = flag;
        wmHints.input_mode  = 0;
#endif
        XUnmapWindow(_dpy, _win );
        XChangeProperty( _dpy, _win, atom, atom, 32, PropModeReplace,
                (unsigned char *)&wmHints,  5 );
        XMapWindow(_dpy, _win );

        XFlush(_dpy);
        XSync(_dpy,0);
    }
    else
        std::cerr << 
         "RenderSurface::setBorder(" << flag << ") - Warning : Couldn't change decorations." << std::endl;
}

void RenderSurface::_setWindowName( const std::string &name )
{
    if( !_realized )
        return;

    if( _drawableType != DrawableType_Window )
        return;


    char *slist[] = { (char *)name.c_str(), 0L };
    XTextProperty xtp;

    XStringListToTextProperty( slist, 1, &xtp );
    XSetWMName( _dpy, _win, &xtp );
    XFlush(_dpy); XSync(_dpy,0);
}

void RenderSurface::_useCursor(bool flag)
{
    if( _drawableType != DrawableType_Window )
        return;

    _useCursorFlag = flag;
    if( _realized )
        _setCursor(_currentCursor);
}

void RenderSurface::_setCursor(Cursor cursor)
{
    if( _drawableType != DrawableType_Window )
        return;

    if( _useCursorFlag == false )
    {
        XDefineCursor( _dpy, _win, _nullCursor );
    }
    else
    {
        _currentCursor = cursor ? cursor : _nullCursor;
            XDefineCursor( _dpy, _win, _currentCursor );
    }
    XFlush(_dpy);
    XSync(_dpy,0);
}

void RenderSurface::_setCursorToDefault()
{
    _setCursor(_defaultCursor);
    //XUndefineCursor( _dpy, _win );
}

void RenderSurface::_positionPointer( int x, int y )
{
    if( !_realized )
        return;

    if( _drawableType != DrawableType_Window )
        return;

    if( _isFullScreen )
        y = _screenHeight - y;
    else
        y = _windowHeight - y;
    XWarpPointer( _dpy, 
                    None,
                    _win, 
                    0, 0, 0, 0,
                    x, y );
    XFlush(_dpy);
    XSync(_dpy,0);
}

void RenderSurface::_computeScreenSize( unsigned int &width, unsigned int &height ) const
{
    Display *dpy = XOpenDisplay("");
    width = DisplayWidth(dpy, _screen);
    height = DisplayHeight(dpy,_screen);
    XCloseDisplay(dpy);
}


void RenderSurface::mapWindow()
{
    if( _drawableType != DrawableType_Window )
        return;

    if( _realized && _parent == RootWindow( _dpy, _screen ) )
    {
        XMapWindow( _dpy, _win );
        XFlush(_dpy);
        XSync(_dpy,0);
    }
}

void RenderSurface::unmapWindow()
{
    if( _drawableType != DrawableType_Window )
        return;

    if( _parent == RootWindow( _dpy, _screen ) )
    {
        XUnmapWindow( _dpy, _win );
        XFlush(_dpy);
        XSync(_dpy,0);
    }
}


void RenderSurface::_fini()
{
    if( _realized )
    {
        if( _drawableType == DrawableType_Window )
        {
            //glXDestroyContext( _dpy, _glcontext );
            XDestroyWindow(_dpy, _win );
        }
        else if( _drawableType == DrawableType_PBuffer )
        {
#ifdef GLX_VERSION_1_3
            glXDestroyPbuffer( _dpy, _win );
#endif
        }
        XFlush( _dpy );
        XSync(_dpy,0);
    }
    if( _visualInfo != NULL )
        XFree(_visualInfo);
	_inited = false;
}

void RenderSurface::bindPBufferToTexture(BufferType buffer) const
{
    glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _windowWidth, _windowHeight);
}

void RenderSurface::_useOverrideRedirect( bool flag)
{
    if( _drawableType != DrawableType_Window )
        return;

    if( _realized )
    {
        std::cerr << "RenderSurface::_useOverrideRedirect(" << flag << "): WARNING - Has no effect after realize() has been called" << std::endl;
        return;
    }
    else
        _overrideRedirectFlag = flag;
}

void RenderSurface::_initThreads()
{
    // For multi-threaded X environment we must initialize the
    // multi-threaded support before we do anything else -- LJE
    static bool initialized = false;    // We shouldn't do this multiple times...

    if( initialized )
        return;

    initialized = true;

#ifdef GLX_SUN_init_threads
    Display *dpy = XOpenDisplay(":0.0");
    if( dpy == NULL )
    {
        std::cerr << "Unable to open display \"" << XDisplayName( ":0.0") << "\".  Is the DISPLAY environmental variable set?\n";
        exit(1);
    }
    if ( strstr ( glXGetClientString (dpy, GLX_EXTENSIONS ), "GLX_SUN_init_threads" ) )
    {
        if ( ! glXInitThreadsSUN ( ) )
        {
            std::cerr <<  "Unable to init GL for multithreaded rendering\n" << std::endl;
            exit(1);
        }
        //else
        //{
        //    std::cout << "Initialized Threads...\n";
        //}
    }
    XCloseDisplay(dpy);
#else
     // InitThreads is only required for Sun (for some unknown reason).  
     // XInitThreads man page reads:
     //
     //   It is only necessary to call this function if multiple threads might use Xlib concurrently.  If all
     //   calls to Xlib functions are protected by some other access mechanism (for example, a mutual exclusion
     //   lock in a toolkit or through explicit client programming), Xlib thread initialization is not required.
     //   It is recommended that single-threaded programs not call this function.
     //
     // Since Producer enforces a policy of one-thread per client, this is not necessary and, potentially, detrimental
     // Don Burns - May 3, 2005

#if 0
    if (XInitThreads() == 0)
    {
        std::cerr << "Error in RenderSurface::InitThreads(): XInitThreads() failed. Aborting." << std::endl;
        exit(1);
    }
    //else
    //{
    //    std::cout << "RenderSurface::InitThreads(): multi-threaded X support initialized.\n";
    //}
#endif

#endif

}

#endif

