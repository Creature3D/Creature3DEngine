#ifndef CRTERRAIN_EXPORT_H
#define CRTERRAIN_EXPORT_H 1

#if defined(_MSC_VER)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4305 )
    #pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)
    #  if defined( CR_LIBRARY_STATIC )
    #    define CRTERRAIN_EXPORT
    #  elif defined( CRTERRAIN_LIBRARY )
    #    define CRTERRAIN_EXPORT   __declspec(dllexport)
    #  else
    #    define CRTERRAIN_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define CRTERRAIN_EXPORT
#endif 

#endif
