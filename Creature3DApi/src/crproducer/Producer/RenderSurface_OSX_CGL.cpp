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

#include <CRProducer/Producer/Export>
#include <CRProducer/Producer/RenderSurface>

#ifdef _OSX_CGL_IMPLEMENTATION

#include <Carbon/Carbon.h>
#include <OpenGL/OpenGL.h>

using namespace Producer;

bool RenderSurface::realize(Producer::VisualChooser *vc, GLContext shared_context)
{
    _sharedGlcontext = shared_context;

    _init();

    _realized = true;
    _realizeBlock->release();

    std::vector <Producer::ref_ptr<Callback> >::iterator p;
    for( p = _realizeCallbacks.begin(); p != _realizeCallbacks.end(); p++ )
    {
        if( (*p).valid() )
            (*(*p).get())( *this );
    }
    _realizeCallbacks.clear();

    return _realized;

}

bool RenderSurface::makeCurrent() const
{
    CGLSetCurrentContext(_glcontext);
    return true;
}

void RenderSurface::swapBuffers()
{
    CGLFlushDrawable(_glcontext);
}

void RenderSurface::run()
{
}

void RenderSurface::sync(int)
{
}

void RenderSurface::fullScreen( bool flag )
{
}

unsigned int RenderSurface::getNumberOfScreens()
{
    /*  From James Hopper:
        change it to just return 1.  my previous patch only grabs 1 screen and
        so it works right with multiscreen macs.  but if some of the osgsamples
        blow up because they end up calling this and getting the wrong number
        of screens.
        */
    /*
    if( _numScreens == UnknownAmount )
    {
        if( CGGetActiveDisplayList( 0, NULL, &_numScreens ) != CGDisplayNoErr )
        {
            std::cerr << "CGGetActiveDisplayList() falied\n";
            _numScreens = UnknownAmount;
            return 0;
        }
    }
    */
    _numScreens = 1;
    return _numScreens;
}

#if 0
static Display *_openDisplay(unsigned int num)
{
    static Display *displays = NULL;
    static CGDisplayCount numDisplays = RenderSurface::getNumberOfScreens();

    if( displays == NULL )
    {
        if( numDisplays == 0 )
        {
            std::cerr << "openDisplay: No Displays!\n";
            return NULL;
        }

        displays = new Display[numDisplays];
        if( CGGetActiveDisplayList( numDisplays, displays, &numDisplays ) != CGDisplayNoErr )
        {
            std::cerr << "CGGetActiveDisplayList() falied\n";
            return NULL;
        }
    }

    if( num > numDisplays )
    {
        std::cerr << "OpenDisplay(): " << num << " is out of range.\n";
        return NULL;
    }

    return &displays[num];
}
#endif

// get a double value from a dictionary
static double getDictDouble (CFDictionaryRef refDict, CFStringRef key)
{
   double double_value;
   CFNumberRef number_value = (CFNumberRef) CFDictionaryGetValue(refDict, key);
   if (!number_value) // if can't get a number for the dictionary
       return -1;  // fail
   if (!CFNumberGetValue(number_value, kCFNumberDoubleType, &double_value)) // or if cant convert it
        return -1; // fail
    return double_value; // otherwise return the long value
}
void RenderSurface::setParentWindow( Window parent )
{
     _parent = parent;
}
bool RenderSurface::_init()
{
	if(_inited) return true;
    s_ini_mutex.lock();

    _dpy = new CGDirectDisplayID;     
    *_dpy = CGMainDisplayID();         
    CGDisplayCapture(*_dpy);

    // Set up the VisualChooser
    VisualChooser *vc
    if(_visualChooser.valid()) vc = _visualChooser.get();
    else
    {
      vc = new VisualChooser;
      vc->setSimpleConfiguration(_drawableType != DrawableType_PBuffer);
    }

    if( _windowWidth == UnknownDimension )
        _windowWidth = CGDisplayPixelsWide(*_dpy);

    if( _windowHeight == UnknownDimension )
        _windowHeight = CGDisplayPixelsHigh(*_dpy);

       // add next line and on following line replace hard coded depth and refresh rate
    CGRefreshRate refresh =  getDictDouble (CGDisplayCurrentMode (*_dpy), kCGDisplayRefreshRate);  
    CFDictionaryRef display_mode_values =
    CGDisplayBestModeForParametersAndRefreshRate(*_dpy, CGDisplayBitsPerPixel (*_dpy), 
                    _windowWidth,_windowHeight,  
                    refresh,  
                    NULL);

    CGDisplaySwitchToMode(*_dpy, display_mode_values);

    _screenWidth = _windowWidth  =  CGDisplayPixelsWide(*_dpy);
    _screenHeight = _windowHeight =  CGDisplayPixelsHigh(*_dpy);

    VisualInfo *vinfo = vc->choose(_dpy, 0); // screen arg doesn't matter
    if(vinfo)
    {
      CGLCreateContext( *vinfo, NULL, &_glcontext );

      CGLDestroyPixelFormat( *vinfo );
      makeCurrent();

      CGLSetFullScreen(_glcontext);

      // Add sync support by default:
      GLint swap = 1;
      CGLSetParameter (_glcontext, kCGLCPSwapInterval, &swap);
    }

    s_init_mutex.unlock();
	_inited = true;
    return true;
}

void RenderSurface::_fini()
{
    
    if( _glcontext )
    {
        CGLClearDrawable(_glcontext);                                                   
        CGLDestroyContext(_glcontext);                                                  
        CGLSetCurrentContext(NULL);                                                      
    }
    if( _dpy )
        CGDisplayRelease(*_dpy);
	_inited = false;
}

void RenderSurface::_setBorder(bool)
{
}

void RenderSurface::_useCursor(bool)
{
}

void RenderSurface::_setCursor( Cursor )
{
}

void RenderSurface::_setCursorToDefault()
{
}

void RenderSurface::_resizeWindow()
{
    _windowWidth =  CGDisplayPixelsWide(*_dpy);
    _windowHeight =  CGDisplayPixelsHigh(*_dpy);
}

void RenderSurface::_positionPointer(int, int)
{
}


void RenderSurface::_computeScreenSize(unsigned int &width, unsigned int &height) const
{
    width =  CGDisplayPixelsWide(*_dpy);
    height =  CGDisplayPixelsHigh(*_dpy);
}

void RenderSurface::bindPBufferToTexture(BufferType /*buffer*/) const
{
    // Ok... a bit of a hack.
    glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _windowWidth, _windowHeight);
}

void _setCursor()
{
}

void _positionPointer(int x, int y)
{
}

void _resizeWindow()
{
}

void _fini()
{
}

void RenderSurface::_useOverrideRedirect( bool )
{
    // Ignored on OSX CGL.  Flag value remains false.
}

void RenderSurface::_setWindowName( const std::string &name )
{
    if( !_realized )
        return;
}

void RenderSurface::_initThreads()
{
    // Ignored...
}


#endif

