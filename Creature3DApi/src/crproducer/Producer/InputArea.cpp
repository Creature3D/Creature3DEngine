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

#ifndef WIN32
#include <unistd.h>
#endif

#include <CRProducer/Producer/Referenced>
#include <CRProducer/Producer/InputArea>
#include <CRProducer/Producer/RenderSurface>

using namespace Producer;

InputArea::InputArea(): _initialized(false)
{
    _minX = _maxX = _minY = _maxY = 0.0f;
}

InputArea::~InputArea()
{
	//std::cerr << "~InputArea()" << std::endl;
}

void InputArea::addRenderSurface( RenderSurface *rs )
{
    // Useless to addInputRectangle after initialization
    if( _initialized ) return;
    _mapdata.push_back( rs );

    RenderSurface::InputRectangle ir = rs->getInputRectangle();

    if(_minX == 0.0f || _minX > ir.left() )
        _minX = ir.left();

    if(_maxX == 0.0f || _maxX < (ir.left()+ir.width()) )
        _maxX = ir.left() + ir.width();

    if(_minY == 0.0f || _minY > ir.bottom() )
        _minY = ir.bottom();

    if(_maxY == 0.0f || _maxY < (ir.bottom()+ir.height()) )
        _maxY = ir.bottom() + ir.height();
}

bool InputArea::_init()
{
    if( _initialized ) return _initialized;

    // Wait for realiziation
    while( _waitForRealize() == false )
           ;
    return (_initialized = true);
}


bool InputArea::waitForRealize()
{
    return _waitForRealize();
}


bool InputArea::_waitForRealize()
{
    std::vector <RenderSurface *>::iterator p;
    for( p = _mapdata.begin(); p != _mapdata.end(); p++ )
    {
        RenderSurface *rs = (*p);
        if( rs->waitForRealize() == false )
            return false;
    }
    return true;
}

bool InputArea::isRealized()
{
    bool retval = true;
    std::vector <RenderSurface *>::iterator p;
    for( p = _mapdata.begin(); p != _mapdata.end(); p++ )
    {
        RenderSurface *rs = (*p);
        if( rs->isRealized() == false )
        {
            retval = false;
            break;
        }
    }
    return retval;
}

void InputArea::normalizeMouseMotion( Window win, int mx, int my, float &nmx, float &nmy)
{
    transformMouseMotion(win,mx,my,nmx,nmy);
    normalizeXY(nmx,nmy);
}

void InputArea::transformMouseMotion( Window win, int mx, int my, float &tmx, float &tmy)
{
    if( !_initialized  && !_init() )
    {
        return;
    }

    std::vector <RenderSurface *>::iterator p;
    for( p = _mapdata.begin(); p != _mapdata.end(); p++ )
    {
        if ((*p)->getWindow() == win)
        {
            break;
        }
    }

    
#if defined(_OSX_AGL_IMPLEMENTATION) || defined(_OSX_CGL_IMPLEMENTATION)  

    // follows is a code block to compenstate for the fack that mouse coordinates
    // under OSX are relative to a global system rather than local to window coordinates,
    // since InputArea is set up so that each RenderSurface sets its coordinates system
    // we must make sure the the mouse coordinates are first within this local RenderSurface
    // area.  This is complicated by the fact that when in fullscreen mode there isn't
    // any window to querry the coordinates of, so we are forced to get the displays bounds.

    int positionLeft = 0;
    int positionTop = 0;
    int windowWidth = 0;
    int windowHeight = 0;

    if (win == NULL)
    {
        // no window handle, so we must be full screen, now iterate through each render surface
        // display to find the one that encloses the mouse pointer


        for( p = _mapdata.begin(); p != _mapdata.end(); p++ )
        {
            if ((*p)->isFullScreen())
            {
                // for full screen render surfaces get the bounds of the complete screen
                // and check to see if mouse pointer is within this.
                Display* display = (*p)->getDisplay();
                CGRect theBounds = CGDisplayBounds(*display);
                if (mx >= theBounds.origin.x && 
                    my >= theBounds.origin.y &&
                    mx <= (theBounds.origin.x + theBounds.size.width) &&
                    my <= (theBounds.origin.y + theBounds.size.height))
                {
                    positionLeft = static_cast<int>(theBounds.origin.x);
                    positionTop = static_cast<int>(theBounds.origin.y);
                    windowWidth = static_cast<int>(theBounds.size.width);
                    windowHeight = static_cast<int>(theBounds.size.height);

                    break;
                }
            }
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

    if (p ==_mapdata.end())
    {
        // nothing found, can't modify mouse coords.
        return;
    }

    RenderSurface *rs = (*p);

#else

    if (p ==_mapdata.end())
    {
        // nothing found, can't modify mouse coords.
        return;
    }
    
    RenderSurface *rs = (*p);
    
    int positionLeft = 0;
    int positionTop = 0;
    int windowWidth = rs->getWindowWidth();
    int windowHeight = rs->getWindowHeight();

#endif
        
    // do the transformation from mouse coords
    // to input area coords.
    
    RenderSurface::InputRectangle ir = rs->getInputRectangle();

    tmx = ir.left() + (float(mx-positionLeft)/float(windowWidth-1)) * ir.width();
    my = windowHeight - (my - positionTop + 1);
    tmy = ir.bottom() + (float(my)/float(windowHeight-1)) * ir.height() + 1.0f / windowHeight;
}

RenderSurface *InputArea::getRenderSurface(unsigned int index)
{
    if (index<=_mapdata.size()) 
    {
        return _mapdata[index];
    }
    else
    {
        return 0;
    }
}

Producer::Window InputArea::getWindow(unsigned int index)
{
    if( !_initialized ) _init();
    
    if (index<= _mapdata.size()) 
    {
        return _mapdata[index]->getWindow();
    }
    else
    {
        return 0;
    }
}

unsigned int InputArea::getNumRenderSurfaces()
{
    if( !_initialized ) _init();
    return _mapdata.size();
}

unsigned int InputArea::getNumWindows()
{
    return getNumRenderSurfaces();
}

void InputArea::getExtents( float &minX, float &maxX, float &minY, float &maxY )
{
    minX = _minX;
    maxX = _maxX;
    minY = _minY;
    maxY = _maxY;
}

void InputArea::getCenter( float &cx, float &cy )
{
    cx = (_minX + _maxX) * 0.5;
    cy = (_minY + _maxY) * 0.5;
}

void InputArea::normalizeXY( float &x, float &y )
{
    float cx, cy;
    getCenter(cx,cy);
    float width = _maxX - _minX;
    float height = _maxY - _minY;
    x = (x - cx)/(width*0.5);
    y = (y - cy)/(height*0.5);
}
