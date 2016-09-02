/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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
//Modified by 吴财华
#ifndef CRCORE_MATH_H
#define CRCORE_MATH_H

#include <math.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>

#if defined(__GNUC__) && defined(WIN32)
#  include <iostream>
#endif

// for OSX users :
// comment in if you want backwards compatibility with 10.1.x versions
// otherwise you'll have problems with missing floorf and __isnan*()
// #define DARWIN_OSX_PRE_10_2

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
    #include <float.h>
#endif


#if (defined(WIN32) && !(defined(_MSC_VER) && (_MSC_VER >= 1300)) && !defined(__MINGW32__) ) || \
    defined (sun) || \
    defined (__DARWIN_OSX__)
    

    #include <float.h>


    // PJA MAC OSX
    // This appears to be the simplest way to get these defined under MACOSX
    // where they arent in math.h

    #ifndef acosf
    #define acosf (float)acos
    #endif

    #ifndef asinf
    #define asinf (float)asin
    #endif

    #ifndef cosf
    #define cosf (float)cos
    #endif

    #ifndef sinf
    #define sinf (float)sin
    #endif

    #ifndef logf
    #define logf (float)log
    #endif

    #ifndef powf
    #define powf (float)pow
    #endif

    #ifndef sqrtf
    #define sqrtf (float)sqrt
    #endif
    
    #ifndef fabsf
    #define fabsf (float)fabs
    #endif

#endif

#if (defined(WIN32) && !(defined(_MSC_VER) && (_MSC_VER >= 1300)) && !defined(__MINGW32__) ) || \
    defined (sun)

    #ifndef isnanf
    #define isnanf (float)isnan
    #endif

#endif

#if (defined(WIN32) && !(defined(_MSC_VER) && (_MSC_VER >= 1300)) && !defined(__MINGW32__) ) || \
    defined (sun) || \
    defined (__hpux__) || \
    defined (DARWIN_OSX_PRE_10_2)

    #ifndef floorf
    #define floorf (float)floor
    #endif

#endif

namespace CRCore {

// define the stand trig values
#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#undef PI_INV
#endif
const double PI   = 3.14159265358979323846;
const double PI_2 = 1.57079632679489661923;
const double PI_4 = 0.78539816339744830962;
const double PI_INV = 0.31830988618;

// Constants.
#define SMALL_NUMBER		(1.e-8)
#define KINDA_SMALL_NUMBER	(1.e-4)
#define BIG_NUMBER			(3.4e+38f)
// Magic numbers for numerical precision.
#define DELTA			(0.00001f)
#define SLERP_DELTA		(0.0001f)

#define NearColor(a,b) (fabs(a-b)<0.003f)
#define CollideMapNear(a,b) (fabs(a-b)<0.00002f)
//// Aux constants.
//#define PI 			(3.1415926535897932)
//#define INV_PI		(0.31830988618)
//#define PI_2			(1.57079632679)
//#define PI_4			(0.78539816339)



/** return the minimum of two values, equivilant to std::min.
  * std::min not used because of STL implementation under IRIX contains no std::min.*/
template<typename T>
inline T absolute(T v) { return v<(T)0?-v:v; }

/** return true if float lhs and rhs are equivalent, meaning that the difference between then is less than an epsilon value which default to 1e-6.*/
inline bool equivalent(float lhs,float rhs,float epsilon=1e-6) { float delta = rhs-lhs; return delta<0.0f?delta>=-epsilon:delta<=epsilon; }

/** return true if double lhs and rhs are equivalent, meaning that the difference between then is less than an epsilon value which default to 1e-6.*/
inline bool equivalent(double lhs,double rhs,double epsilon=1e-6) { double delta = rhs-lhs; return delta<0.0?delta>=-epsilon:delta<=epsilon; }

/** return the minimum of two values, equivilant to std::min.
  * std::min not used because of STL implementation under IRIX contains no std::min.*/
template<typename T>
inline T minimum(T lhs,T rhs) { return lhs<rhs?lhs:rhs; }

/** return the maximum of two values, equivilant to std::max.
  * std::max not used because of STL implementation under IRIX contains no std::max.*/
template<typename T>
inline T maximum(T lhs,T rhs) { return lhs>rhs?lhs:rhs; }

template<typename T>
inline T clampTo(T v,T minimum,T maximum) { return v<minimum?minimum:v>maximum?maximum:v; }

template<typename T>
inline T clampAbove(T v,T minimum) { return v<minimum?minimum:v; }

template<typename T>
inline T clampBelow(T v,T maximum) { return v>maximum?maximum:v; }

template<typename T>
inline T clampBetween(T v,T minimum, T maximum) { return clampBelow(clampAbove(v,minimum),maximum); }

template<typename T>
inline T sign(T v) { return v<(T)0?(T)-1:(T)1; }

template<typename T>
inline T square(T v) { return v*v; }

template<typename T>
inline T signedSquare(T v) { return v<(T)0?-v*v:v*v;; }

inline float inDegrees(float angle) { return angle*(float)PI/180.0f; }
inline double inDegrees(double angle) { return angle*PI/180.0; }

template<typename T>
inline T inRadians(T angle) { return angle; }

inline float DegreesToRadians(float angle) { return angle*(float)PI/180.0f; }
inline double DegreesToRadians(double angle) { return angle*PI/180.0; }

inline float RadiansToDegrees(float angle) { return angle*180.0f/(float)PI; }
inline double RadiansToDegrees(double angle) { return angle*180.0/PI; }

#if defined(WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)
    inline bool isNaN(float v) { return _isnan(v)!=0; }
    inline bool isNaN(double v) { return _isnan(v)!=0; }
#else
    # if defined(__APPLE__) && !defined (APPLE_PRE_10_2)
        inline bool isNaN(float v) { return __isnanf(v); }
        inline bool isNaN(double v) { return __isnand(v); }
    #else
        inline bool isNaN(float v) { return isnan(v); }
        inline bool isNaN(double v) { return isnan(v); }
    #endif
#endif

/** compute the volume of tetrahedron */
template<typename T>
inline float computeVolume(const T& a,const T& b,const T& c,const T& d)
{
    return fabsf(((b-c)^(a-b))*(d-b));
}

/** compute the volume of prism */
template<typename T>
inline float computeVolume(const T& f1,const T& f2,const T& f3,
                           const T& b1,const T& b2,const T& b3)
{
    return computeVolume(f1,f2,f3,b1)+
           computeVolume(b1,b2,b3,f2)+
		   computeVolume(b1,b3,f2,f3);
}
/////////////////////////////////////
inline float lerp(const float u, const float v, const float x){
	return u * (1.0f - x) + v * x;
}

inline crVector2 lerp(const crVector2 &u, const crVector2 &v, const float x){
	return u * (1.0f - x) + v * x;
}

inline CRCore::crVector3 lerp(const CRCore::crVector3 &u, const CRCore::crVector3 &v, const float x){
	return u * (1.0f - x) + v * x;
}

inline CRCore::crVector4 lerp(const CRCore::crVector4 &u, const CRCore::crVector4 &v, const float x){
	return u * (1.0f - x) + v * x;
}

}

#endif  // __CRCORE_MATH
