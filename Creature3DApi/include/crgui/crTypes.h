/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRGUI_CRTYPES
#define CRGUI_CRTYPES 1

#include <CRGUI\crExport.h>

#ifdef _WIN32_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <GL/gl.h>

    namespace CRGUI {

        typedef HDC      Display;
        typedef HWND     Window;
        typedef HGLRC    GLContext;
        typedef WPARAM   KeySymbol;
        typedef HCURSOR  Cursor;
        typedef PIXELFORMATDESCRIPTOR  VisualInfo;
    
    }
#endif

#ifdef _X11_IMPLEMENTATION

    #include <X11/Xlib.h>  
    #include <X11/Xutil.h>
    #include <GL/glx.h>

    namespace CRGUI {

        // Display, Window and Cursor natively defined
        typedef GLXContext GLContext;
        typedef GLXPbuffer Pbuffer;
        typedef KeySym    KeySymbol;
        typedef XVisualInfo VisualInfo;

    }

#endif

#endif
