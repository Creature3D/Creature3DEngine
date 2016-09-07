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

#ifdef WIN32

#ifndef PRODUCER_WGLEXTENSIONS_
#define PRODUCER_WGLEXTENSIONS_ 1

#include <CRProducer/Producer/Referenced>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <map>
#include <string>

#ifndef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer 1
DECLARE_HANDLE(HPBUFFERARB);
#define WGL_DRAW_TO_PBUFFER_ARB             0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB          0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB           0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB          0x2030
#define WGL_PBUFFER_LARGEST_ARB             0x2033
#define WGL_PBUFFER_WIDTH_ARB               0x2034
#define WGL_PBUFFER_HEIGHT_ARB              0x2035
#define WGL_PBUFFER_LOST_ARB                0x2036
#endif

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1
#define WGL_NUMBER_PIXEL_FORMATS_ARB        0x2000
#define WGL_DRAW_TO_WINDOW_ARB              0x2001
#define WGL_DRAW_TO_BITMAP_ARB              0x2002
#define WGL_ACCELERATION_ARB                0x2003
#define WGL_NEED_PALETTE_ARB                0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB         0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB          0x2006
#define WGL_SWAP_METHOD_ARB                 0x2007
#define WGL_NUMBER_OVERLAYS_ARB             0x2008
#define WGL_NUMBER_UNDERLAYS_ARB            0x2009
#define WGL_TRANSPARENT_ARB                 0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB       0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB     0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB      0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB     0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB     0x203B
#define WGL_SHARE_DEPTH_ARB                 0x200C
#define WGL_SHARE_STENCIL_ARB               0x200D
#define WGL_SHARE_ACCUM_ARB                 0x200E
#define WGL_SUPPORT_GDI_ARB                 0x200F
#define WGL_SUPPORT_OPENGL_ARB              0x2010
#define WGL_DOUBLE_BUFFER_ARB               0x2011
#define WGL_STEREO_ARB                      0x2012
#define WGL_PIXEL_TYPE_ARB                  0x2013
#define WGL_COLOR_BITS_ARB                  0x2014
#define WGL_RED_BITS_ARB                    0x2015
#define WGL_RED_SHIFT_ARB                   0x2016
#define WGL_GREEN_BITS_ARB                  0x2017
#define WGL_GREEN_SHIFT_ARB                 0x2018
#define WGL_BLUE_BITS_ARB                   0x2019
#define WGL_BLUE_SHIFT_ARB                  0x201A
#define WGL_ALPHA_BITS_ARB                  0x201B
#define WGL_ALPHA_SHIFT_ARB                 0x201C
#define WGL_ACCUM_BITS_ARB                  0x201D
#define WGL_ACCUM_RED_BITS_ARB              0x201E
#define WGL_ACCUM_GREEN_BITS_ARB            0x201F
#define WGL_ACCUM_BLUE_BITS_ARB             0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB            0x2021
#define WGL_DEPTH_BITS_ARB                  0x2022
#define WGL_STENCIL_BITS_ARB                0x2023
#define WGL_AUX_BUFFERS_ARB                 0x2024
#define WGL_NO_ACCELERATION_ARB             0x2025
#define WGL_GENERIC_ACCELERATION_ARB        0x2026
#define WGL_FULL_ACCELERATION_ARB           0x2027
#define WGL_SWAP_EXCHANGE_ARB               0x2028
#define WGL_SWAP_COPY_ARB                   0x2029
#define WGL_SWAP_UNDEFINED_ARB              0x202A
#define WGL_TYPE_RGBA_ARB                   0x202B
#define WGL_TYPE_COLORINDEX_ARB             0x202C
#endif

#ifndef WGL_ARB_render_texture
#define WGL_ARB_render_texture 1
#define WGL_BIND_TO_TEXTURE_RGB_ARB         0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB        0x2071
#define WGL_TEXTURE_FORMAT_ARB              0x2072
#define WGL_TEXTURE_TARGET_ARB              0x2073
#define WGL_MIPMAP_TEXTURE_ARB              0x2074
#define WGL_TEXTURE_RGB_ARB                 0x2075
#define WGL_TEXTURE_RGBA_ARB                0x2076
#define WGL_NO_TEXTURE_ARB                  0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB            0x2078
#define WGL_TEXTURE_1D_ARB                  0x2079
#define WGL_TEXTURE_2D_ARB                  0x207A
#define WGL_MIPMAP_LEVEL_ARB                0x207B
#define WGL_CUBE_MAP_FACE_ARB               0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB                  0x2083
#define WGL_FRONT_RIGHT_ARB                 0x2084
#define WGL_BACK_LEFT_ARB                   0x2085
#define WGL_BACK_RIGHT_ARB                  0x2086
#define WGL_AUX0_ARB                        0x2087
#define WGL_AUX1_ARB                        0x2088
#define WGL_AUX2_ARB                        0x2089
#define WGL_AUX3_ARB                        0x208A
#define WGL_AUX4_ARB                        0x208B
#define WGL_AUX5_ARB                        0x208C
#define WGL_AUX6_ARB                        0x208D
#define WGL_AUX7_ARB                        0x208E
#define WGL_AUX8_ARB                        0x208F
#define WGL_AUX9_ARB                        0x2090
#endif

namespace Producer
{    

    class WGLExtensions: public Referenced
    {
    public:

		enum SupportedExtension
		{
			ARB_pbuffer           = 1,
			ARB_render_texture    = 2,
			ARB_pixel_format      = 4,
			ARB_make_current_read = 8
		};

		// WGL_ARB_pbuffer

		typedef HPBUFFERARB (WINAPI * WGLCreatePBufferProc)    (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
		typedef HDC         (WINAPI * WGLGetPBufferDCProc)     (HPBUFFERARB hPbuffer);
		typedef int         (WINAPI * WGLReleasePBufferDCProc) (HPBUFFERARB hPbuffer, HDC hDC);
		typedef bool        (WINAPI * WGLDestroyPBufferProc)   (HPBUFFERARB hPbuffer);
		typedef bool        (WINAPI * WGLQueryPBufferProc)     (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

		WGLCreatePBufferProc    wglCreatePBuffer;
		WGLGetPBufferDCProc     wglGetPBufferDC;
		WGLReleasePBufferDCProc wglReleasePBufferDC;
		WGLDestroyPBufferProc   wglDestroyPBuffer;
		WGLQueryPBufferProc     wglQueryPBuffer;


		// WGL_ARB_render_texture

		typedef bool        (WINAPI * WGLBindTexImageProc)     (HPBUFFERARB hPbuffer, int iBuffer);
		typedef bool        (WINAPI * WGLReleaseTexImageProc)  (HPBUFFERARB hPbuffer, int iBuffer);
		typedef bool        (WINAPI * WGLSetPbufferAttribProc) (HPBUFFERARB hPbuffer, const int * piAttribList);

		WGLBindTexImageProc     wglBindTexImage;
		WGLReleaseTexImageProc  wglReleaseTexImage;
		WGLSetPbufferAttribProc wglSetPBufferAttrib;

		// WGL_ARB_pixel_format

		typedef bool        (WINAPI * WGLChoosePixelFormatProc) (HDC, const int *, const float *, unsigned int, int *, unsigned int *);

		WGLChoosePixelFormatProc wglChoosePixelFormat;

		// WGL_ARB_make_current_read

		typedef bool        (WINAPI * WGLMakeContextCurrentProc) (HDC, HDC, HGLRC);

		WGLMakeContextCurrentProc wglMakeContextCurrent;

		typedef bool (APIENTRY *WGLSwapIntervalEXTProc)(int);
		WGLSwapIntervalEXTProc wglSwapIntervalEXT;

		//typedef bool (APIENTRY *WGLSwapBuffersProc)(HDC);
		//WGLSwapBuffersProc wglSwapBuffers;

		// this static method returns an instance of WGLExtensions that
		// is unique for the active rendering context (queried via
		// wglGetCurrentContext()). If no rendering context is active, 
		// the global temporary window's context will be used. If the 
		// temporary window can't be created, then 0 is returned.
		static WGLExtensions *instance();

		inline bool isSupported(SupportedExtension ext) const { return (supp_ & ext) != 0; }

		static void clear();
		void setVSync(bool sync);
    protected:
		WGLExtensions();
		~WGLExtensions();

    private:
		typedef std::map<HGLRC, Producer::ref_ptr<WGLExtensions> > InstanceMap;
        static InstanceMap instances_;
		int supp_;
    };

    

	class TemporaryWindow: public Referenced
    {
    public:
		TemporaryWindow();
		TemporaryWindow(const TemporaryWindow &);

        HWND  getHandle() const    { return handle_; }
        HDC   getDC() const        { return dc_; }
        HGLRC getContext() const   { return context_; }

		bool makeCurrent();

	protected:
		~TemporaryWindow();
		TemporaryWindow &operator=(const TemporaryWindow &) { return *this; }

		void create();
		void kill();

    private:
        HWND handle_;
        HDC dc_;
        HGLRC context_;
        HINSTANCE instance_;
        std::string classname_;
    };

}

#endif

#endif

