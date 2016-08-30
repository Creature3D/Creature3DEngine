#ifndef CRNET_CREXPORT_H
#define CRNET_CREXPORT_H 1

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
    #pragma warning( disable : 4276 )
	#pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
	#  ifdef CRNET_DLL
	#    define CRNET_EXPORT   __declspec(dllexport)
	#  elif !defined(CRNET_LIB)
	#    define CRNET_EXPORT   __declspec(dllimport)
	#endif
#endif
#if !defined(CRNET_EXPORT)
#define CRNET_EXPORT
#endif
//// Global variables (evil).
//static const unsigned short DEFPORT = 5050;
//static const int MAXPKTSZ = 65536;
//
//enum {
//	SELECT = 1,
//	TP,
//	WFMO
//};
//
//#define KBEvent 0x1
//#define CreMatrix 0x2
//#define Connect 0x2
//#define EventGroup 0x4

#endif

