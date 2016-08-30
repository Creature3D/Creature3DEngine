
#ifndef CRCORE_GL
#define CRCORE_GL 1

#ifndef WIN32

    // non windows, doesn't require nonsense as seen below :-)    
    #ifndef __gl_h_
        #ifdef __DARWIN_OSX__
            #include <OpenGL/gl.h>
        #else
            #include <GL/gl.h>
        #endif
    #endif

    // required for compatibility with glext.h sytle function definitions of 
    // OpenGL extensions, such as in src/osg/Point.cpp.
    #ifndef APIENTRY
        #define APIENTRY
    #endif

#else // WIN32

    #if defined(__CYGWIN__) || defined(__MINGW32__)

        #ifndef APIENTRY
            #define GLUT_APIENTRY_DEFINED
            #define APIENTRY    __stdcall
        #endif
               // XXX This is from Win32's <winnt.h> 
        #ifndef CALLBACK
            #define CALLBACK __stdcall
        #endif

    #else // ! __CYGWIN__

        // Under windows avoid including <windows.h>
        // to avoid name space pollution, but Win32's <GL/gl.h> 
        // needs APIENTRY and WINGDIAPI defined properly. 
           // XXX This is from Win32's <windef.h> 
        #ifndef APIENTRY
            #define GLUT_APIENTRY_DEFINED
            #if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
                #define WINAPI        __stdcall
                #define APIENTRY    WINAPI
            #else
                #define APIENTRY
            #endif
        #endif

         // XXX This is from Win32's <winnt.h> 
        #ifndef CALLBACK
            #if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
                #define CALLBACK __stdcall
            #else
                #define CALLBACK
            #endif
        #endif

    #endif // __CYGWIN__

           // XXX This is from Win32's <wingdi.h> and <winnt.h> 
    #ifndef WINGDIAPI
        #define GLUT_WINGDIAPI_DEFINED
        #define DECLSPEC_IMPORT    __declspec(dllimport)
        #define WINGDIAPI DECLSPEC_IMPORT
    #endif
           // XXX This is from Win32's <ctype.h>

    #if !defined(_WCHAR_T_DEFINED) && !(defined(__GNUC__)&&(__GNUC__ == 3))
        typedef unsigned short wchar_t;
        #define _WCHAR_T_DEFINED
    #endif

    #ifndef __gl_h_
        #include <GL/gl.h>
    #endif

#endif // WIN32

    inline void glLoadMatrix(const float* mat) { glLoadMatrixf((GLfloat*)mat); }
    inline void glLoadMatrix(const double* mat) { glLoadMatrixd((GLdouble*)mat); }
    inline void glMultMatrix(const float* mat) { glMultMatrixf((GLfloat*)mat); }
    inline void glMultMatrix(const double* mat) { glMultMatrixd((GLdouble*)mat); }
    
#pragma comment(lib,"opengl32.lib")              //链接时使用Opengl32.lib
#pragma comment(lib,"glu32.lib")                 //链接时使用GLu32.lib 
//#pragma comment(lib,"glaux.lib")                 //链接时使用GLaux.lib

#endif  //
