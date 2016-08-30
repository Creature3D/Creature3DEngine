#ifndef CRPARTICLE_EXPORT_
#define CRPARTICLE_EXPORT_ 1

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4305 )
    #pragma warning( disable : 4996 )
#endif


#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( CR_LIBRARY_STATIC )
    #    define CRPARTICLE_EXPORT
    #  elif defined( CRPARTICLE_LIBRARY )
    #    define CRPARTICLE_EXPORT   __declspec(dllexport)
    #  else
    #    define CRPARTICLE_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define CRPARTICLE_EXPORT
#endif
  
/**

\namespace CRParticle

The CRParticle is a NodeKit library that extends the core scene graph to support particle effects.
*/


#endif
