// $Id: memory_debug.h,v 1.1 2005/02/09 18:14:02 deepone Exp $

#ifndef __memory_debug_h__
#define __memory_debug_h__



#ifdef MEMORY_DEBUG

#ifndef WIN32
  #error "Memory debugging only working under windows"
#endif

  #define CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#endif

#ifdef MEMORY_DEBUG

#define MEMORY_DEBUG_STATE _CrtMemState

#define START_MEM_DEBUG(X)   _CrtMemCheckpoint(&X);
#else
#define START_MEM_DEBUG(X)
#define MEMORY_DEBUG_STATE void *
#endif


#ifdef MEMORY_DEBUG

#define END_MEM_DEBUG(X) \
  { \
    MEMORY_DEBUG_STATE end_state; \
    _CrtMemCheckpoint( &end_state ); \
  \
    { \
      MEMORY_DEBUG_STATE s3; \
  \
      if ( _CrtMemDifference( &s3, &X, &end_state) ) { \
        _CrtMemDumpStatistics( &s3 ); \
        _CrtMemDumpAllObjectsSince(&X); \
      } \
    } \
  }


#else
#define END_MEM_DEBUG(X)
#endif


#endif
