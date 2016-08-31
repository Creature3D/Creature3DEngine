
// The following symbol has a underscore suffix for compatibility.
#ifndef CRUTIL_CREXPORT_H
#define CRUTIL_CREXPORT_H 1

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
	#pragma warning( disable : 4244 )
	#pragma warning( disable : 4251 )
	#pragma warning( disable : 4267 )
	#pragma warning( disable : 4275 )
	#pragma warning( disable : 4290 )
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4305 )
	#pragma warning( disable : 4006 )
	#pragma warning( disable : 4312 )
	#pragma warning( disable : 4311 )
	#pragma warning( disable : 4018 )
    #pragma warning( disable : 4800 )
	#pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
	#  ifdef CRUTIL_LIBRARY
	#    define CRUTIL_EXPORT   __declspec(dllexport)
	#  else
	#    define CRUTIL_EXPORT   __declspec(dllimport)
	#endif /* CRUTIL_LIBRARY */
#else
	#define CRUTIL_EXPORT 
#endif 

#endif

