#ifndef CRGLTEXT_EXPORT_
#define CRGLTEXT_EXPORT_ 1

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
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)
    #  ifdef CRGLTEXT_LIBRARY
    #    define CRGLTEXT_EXPORT   __declspec(dllexport)
    #  else
    #    define CRGLTEXT_EXPORT   __declspec(dllimport)
    #  endif /* CRGLTEXT_LIBRARY */
#else
    #  define CRGLTEXT_EXPORT
#endif  

#endif
