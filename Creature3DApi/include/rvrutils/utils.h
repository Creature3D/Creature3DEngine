#ifndef _rvrutils_utils_h_
#define _rvrutils_utils_h_
#include <algorithm>
#include <string>
#include <cctype>

#include "rvrutils/export.h"

#if defined(_MSC_VER)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 ) // Disable warnings about long names
    #pragma warning( disable : 4788 ) // Disable warnings about ...
#endif

/* Define NULL pointer value */
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif // __cplusplus
#endif

#ifdef _WIN32
#include <math.h>
//VRUTILS_API float rint(float x);
VRUTILS_API double drand48( void );
#define srand48 srand
#define snprintf _snprintf
#endif


#ifdef _WIN32
#include <float.h> // for _isnan
#endif
#include <stdexcept>
#include <string>

#include <iterator>

#include <stdio.h>
#include <math.h>

#define PURGE(Seq) \
void purge(Seq& c) {\
    Seq::iterator i;\
    for(i = c.begin(); i != c.end(); i++) {\
        delete *i;\
        *i = 0;\
    }\
}

#ifndef CLAMP
#define CLAMP( x, min, max ) \
    ( ((x) > (max)) ? (max) : ( ((x) < (min)) ? (min) : (x) ))
#endif    

#ifndef MAX
#define MAX( x, y )    ((x) > (y) ? x : (y))
#endif

#ifndef MIN
#define MIN( x, y )    ((x) < (y) ? x : (y))
#endif

#define VAR_OUT( x )    #x " = " << x

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace rvrutils {

/// My round
VRUTILS_API int myround( float n );

#if defined(WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)
    /// Check if float is NaN
    inline bool isNaN(float v) { return _isnan(v)!=0; }
    /// Check if double is NaN
    inline bool isNaN(double v) { return _isnan(v)!=0; }
#else
    /// Check if float is NaN
    inline bool isNaN(float v) { return isnan(v); }
    /// Check if double is NaN
    inline bool isNaN(double v) { return isnan(v); }
#endif

template<class Seq> void cleanup(Seq& c) {
    typename Seq::iterator i;
    for(i = c.begin(); i != c.end(); i++) {
        c.erase( i );
    }
}


inline int cmp_nocase(const std::string& s, const std::string& s2) {
    std::string::const_iterator p= s.begin();
    std::string::const_iterator p2 = s2.begin();

    while (p!= s.end() & p2 != s2.end()) 
    {
        if (std::toupper(*p) != std::toupper(*p2)) return (std::toupper(*p)<std::toupper(*p2)) ? -1:1;
        ++p;
        ++p2;
    }
    return (s2.size() == s.size()) ? 0 : (s.size()<s2.size()) ? -1 : 1; 
}


/// Transform string s to uppercase
inline void toupper(std::string& s) {
    // HACK - Don't know why it does not work without this / Daniel
    int (*tmp)(int) = std::toupper;
    std::transform (s.begin(), s.end(), // source
            s.begin(),          // destination 
            tmp);      // operation
}

/// Transform string s to lowercase
inline void tolower(std::string& s) {
    // HACK - Don't know why it does not work without this / Daniel
    int (*tmp)(int) = std::tolower;
    std::transform (s.begin(), s.end(), // source
            s.begin(),          // destination 
            tmp);      // operation
}


VRUTILS_API double randInterval(double low, double high);


} // namespace end

#endif

