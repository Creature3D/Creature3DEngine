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
#ifndef CRCORE_VEC2
#define CRCORE_VEC2 1

//#include <CRCore/crMath.h>

#include <iostream>

namespace CRCore {

  /** General purpose T pair, uses include representation of
  texture coordinates.
  No support yet added for T * crVector2_T<T> - is it necessary?
  Need to define a non-member non-friend operator*  etc.
  BTW:	     crVector2_T<T> * T is okay
  */
template<class T>
class crVector2_T
{
  public:

    crVector2_T() {_v[0]=0; _v[1]=0;}
    crVector2_T(T x,T y) { _v[0]=x; _v[1]=y; }
	
	crVector2_T(const crVector2_T<int>& v) { _v[0]=v._v[0]; _v[1]=v._v[1]; }
	crVector2_T(const crVector2_T<float>& v) { _v[0]=v._v[0]; _v[1]=v._v[1]; }
	crVector2_T(const crVector2_T<double>& v) { _v[0]=v._v[0]; _v[1]=v._v[1]; }
	
    T _v[2];

	inline crVector2_T<T>& operator = (const crVector2_T<T>& v) { _v[0]=v._v[0]; _v[1]=v._v[1]; return (*this); }

    inline bool operator == (const crVector2_T<T>& v) const { return _v[0]==v._v[0] && _v[1]==v._v[1]; }

    inline bool operator != (const crVector2_T<T>& v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1]; }

    inline bool operator <  (const crVector2_T<T>& v) const
    {
      if (_v[0]<v._v[0]) return true;
      else if (_v[0]>v._v[0]) return false;
      else return (_v[1]<v._v[1]);
    }

    inline bool operator <= (const crVector2_T<T>& v) const { return *this<v || *this==v; }

	inline bool operator >  (const crVector2_T<T>& v) const
	{
		return !(*this<=v);
	}

    inline T* ptr() { return _v; }
    inline const T* ptr() const { return _v; }

    inline void set( T x, T y ) { _v[0]=x; _v[1]=y; }

    inline T& operator [] (int i) { return _v[i]; }
    inline T operator [] (int i) const { return _v[i]; }

    inline T& x() { return _v[0]; }
    inline T& y() { return _v[1]; }

    inline T x() const { return _v[0]; }
    inline T y() const { return _v[1]; }

    inline bool valid() const { return !isNaN(); }
    inline bool isNaN() const { return CRCore::isNaN(_v[0]) || CRCore::isNaN(_v[1]); }

    /// dot product
    inline T operator * (const crVector2_T<T>& rhs) const
    {
      return _v[0]*rhs._v[0]+_v[1]*rhs._v[1];
    }

    /// multiply by scalar
    inline const crVector2_T<T> operator * (T rhs) const
    {
      return crVector2_T<T>(_v[0]*rhs, _v[1]*rhs);
    }

    /// unary multiply by scalar
    inline crVector2_T<T>& operator *= (T rhs)
    {
      _v[0]*=rhs;
      _v[1]*=rhs;
      return *this;
    }

    /// divide by scalar
    inline const crVector2_T<T> operator / (T rhs) const
    {
      return crVector2_T<T>(_v[0]/rhs, _v[1]/rhs);
    }

    /// unary divide by scalar
    inline crVector2_T<T>& operator /= (T rhs)
    {
      _v[0]/=rhs;
      _v[1]/=rhs;
      return *this;
    }

    /// binary vector add
    inline const crVector2_T<T> operator + (const crVector2_T<T>& rhs) const
    {
      return crVector2_T<T>(_v[0]+rhs._v[0], _v[1]+rhs._v[1]);
    }

    /** unary vector add.  Slightly more efficient because no temporary
    intermediate object.*/
    inline crVector2_T<T>& operator += (const crVector2_T<T>& rhs)
    {
      _v[0] += rhs._v[0];
      _v[1] += rhs._v[1];
      return *this;
    }

    /// binary vector subtract
    inline const crVector2_T<T> operator - (const crVector2_T<T>& rhs) const
    {
      return crVector2_T<T>(_v[0]-rhs._v[0], _v[1]-rhs._v[1]);
    }

    /// unary vector subtract
    inline crVector2_T<T>& operator -= (const crVector2_T<T>& rhs)
    {
      _v[0]-=rhs._v[0];
      _v[1]-=rhs._v[1];
      return *this;
    }

    /// negation operator.  Returns the negative of the crVector2_T<T>
    inline const crVector2_T<T> operator - () const
    {
      return crVector2_T<T> (-_v[0], -_v[1]);
    }

    /// Length of the vector = sqrt( vec . vec )
    inline T length() const
    {
      return sqrtf( _v[0]*_v[0] + _v[1]*_v[1] );
    }

    /// Length squared of the vector = vec . vec
    inline T length2( void ) const
    {
      return _v[0]*_v[0] + _v[1]*_v[1];
    }

    /** normalize the vector so that it has length unity
    returns the previous length of the vector*/
    inline const crVector2_T<T>& normalize()
    {
      T norm = crVector2_T<T>::length();
      if (norm>0.0f)
      {
        T inv = 1.0f/norm;
        _v[0] *= inv;
        _v[1] *= inv;
      }
      return *this;
    }

    friend inline std::ostream& operator << (std::ostream& output, const crVector2_T<T>& vec)
    {
      output << vec._v[0] << " "
        << vec._v[1];
      return output; 	// to enable cascading
    }

};	// end of class crVector2_T<T>

typedef crVector2_T<float>  crVector2;
typedef crVector2_T<int>    crVector2i;
typedef crVector2_T<unsigned int>    crVector2ui;
typedef crVector2_T<float>  crVector2f;
typedef crVector2_T<double> crVector2d;

typedef crVector2_T<char> crVector2b;
typedef crVector2_T<unsigned char> crVector2ub;
typedef crVector2_T<short> crVector2s;
typedef crVector2_T<unsigned short>    crVector2us;

}	// end of namespace CRCORE
#endif
