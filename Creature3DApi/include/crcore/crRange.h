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
//Modified by Wucaihua
#ifndef CRCORE_RANGE
#define CRCORE_RANGE 1

// include crExport simply to disable Visual Studio silly warnings.
#include <CRCore/crExport.h>

#ifndef __sgi
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMath.h>
//using std::rand;

namespace CRCore
{

    /**
        A simple struct template useful to store ranges of values as min/max pairs.
        This struct template helps storing min/max ranges for values of any kind; class <CODE>ValueType</CODE> is
        the type of values to be stored, and it must support operations <CODE>ValueType + ValueType</CODE>, <CODE>ValueType - ValueType</CODE>, 
        and <CODE>ValueType * float</CODE>, otherwise the <CODE>geValueTyperandom()</CODE> method will not compile.
        This struct could be extended to customize the random number generator (now it uses only 
        <CODE>std::rand()</CODE>).
    */
    template<class ValueType> struct crRange
    {
    
        /// Lower bound.
        ValueType minimum;
        
        /// Higher bound.
        ValueType maximum;
        
		//float probability;

        /// Construct the object by calling default constructors for min and max.
        crRange() : minimum(ValueType()), maximum(ValueType()) {}
        
        /// Construct and initialize min and max directly.
        crRange(const ValueType& mn, const ValueType& mx) : minimum(mn), maximum(mx) {}
		
		crRange(const crRange<ValueType>& r) { minimum=r.minimum;  maximum=r.maximum; }

        /// Set min and max.
        void set(const ValueType& mn, const ValueType& mx) { minimum = mn; maximum = mx; }

        /// Get a random value between min and max.
		//∑µªÿ÷µ”Ú:[minimum,maximum)
        ValueType get_random() const
        {
			int r = rand();
			r = clampTo(r,0,RAND_MAX-1);
            return minimum + (maximum - minimum) * static_cast<float>(r) / static_cast<float>(RAND_MAX);
        }

        /// Get a random square root value between min and max.
        ValueType get_random_sqrtf() const
        {
			int r = rand();
			r = clampTo(r,0,RAND_MAX-1);
            return minimum + (maximum - minimum) * sqrtf( static_cast<float>(r) / static_cast<float>(RAND_MAX) );
        }
        
        ValueType mid() const
        {
            return (minimum+maximum)*0.5f;
        }

		inline crRange<ValueType>& operator = (const crRange<ValueType>& r) { minimum=r.minimum;  maximum=r.maximum; return (*this); }

    };

	typedef crRange<int> rangei;
    /// Range of floats.
    typedef crRange<float> rangef;
    
    /// Range of CRCore::Vec2s.
    typedef crRange<CRCore::crVector2> rangev2;
    
    /// Range of CRCore::Vec3s.
    typedef crRange<CRCore::crVector3> rangev3;
    
    /// Range of CRCore::Vec4s.
    typedef crRange<CRCore::crVector4> rangev4;

}

#endif
