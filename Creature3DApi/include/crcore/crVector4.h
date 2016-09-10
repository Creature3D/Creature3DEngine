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
#ifndef CRCORE_VEC4
#define CRCORE_VEC4 1

#include <CRCore/crVector3.h>

#include <iostream>

namespace CRCore {

  /** General purpose T quad, uses include representation
  of colour coordinates.
  No support yet added for T * crVector4_T<T> - is it necessary?
  Need to define a non-member non-friend operator*  etc.
  crVector4_T<T> * T is okay
  */
template<class T>
class crVector4_T
{
  public:

    // Methods are defined here so that they are implicitly inlined

    crVector4_T() { _v[0]=0; _v[1]=0; _v[2]=0; _v[3]=0;}

    crVector4_T(T x, T y, T z, T w)
    {
      _v[0]=x;
      _v[1]=y;
      _v[2]=z;
      _v[3]=w;
    }

    crVector4_T(const crVector3_T<int>& v3,T w){ _v[0]=v3._v[0]; _v[1]=v3._v[1]; _v[2]=v3._v[2]; _v[3]=w; }
	crVector4_T(const crVector3_T<float>& v3,T w){ _v[0]=v3._v[0]; _v[1]=v3._v[1]; _v[2]=v3._v[2]; _v[3]=w; }
	crVector4_T(const crVector3_T<double>& v3,T w){ _v[0]=v3._v[0]; _v[1]=v3._v[1]; _v[2]=v3._v[2]; _v[3]=w;	}

	crVector4_T(const crVector4_T<int>& v4){ _v[0]=v4._v[0]; _v[1]=v4._v[1]; _v[2]=v4._v[2]; _v[3]=v4._v[3]; }
	crVector4_T(const crVector4_T<float>& v4){ _v[0]=v4._v[0]; _v[1]=v4._v[1]; _v[2]=v4._v[2]; _v[3]=v4._v[3]; }
	crVector4_T(const crVector4_T<double>& v4){ _v[0]=v4._v[0]; _v[1]=v4._v[1]; _v[2]=v4._v[2]; _v[3]=v4._v[3]; }

    T _v[4];

	inline crVector4_T<T>& operator = (const crVector4_T<T>& v) { _v[0]=v._v[0];  _v[1]=v._v[1];  _v[2]=v._v[2];  _v[3]=v._v[3]; return (*this); }

    inline bool operator == (const crVector4_T<T>& v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]; }

    inline bool operator != (const crVector4_T<T>& v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]; }

    inline bool operator <  (const crVector4_T<T>& v) const
    {
      if (_v[0]<v._v[0]) return true;
      else if (_v[0]>v._v[0]) return false;
      else if (_v[1]<v._v[1]) return true;
      else if (_v[1]>v._v[1]) return false;
      else if (_v[2]<v._v[2]) return true;
      else if (_v[2]>v._v[2]) return false;
      else return (_v[3]<v._v[3]);
    }

    inline bool operator <= (const crVector4_T<T>& v) const { return *this<v || *this==v; }

	inline bool operator >  (const crVector4_T<T>& v) const
	{
        return !(*this<=v);
	}

    inline T* ptr() { return _v; }
    inline const T* ptr() const { return _v; }

    inline void set( T x, T y, T z, T w)
    {
      _v[0]=x; _v[1]=y; _v[2]=z; _v[3]=w;
    }

    inline T& operator [] (unsigned int i) { return _v[i]; }
    inline T  operator [] (unsigned int i) const { return _v[i]; }

    inline T& x() { return _v[0]; }
    inline T& y() { return _v[1]; }
    inline T& z() { return _v[2]; }
    inline T& w() { return _v[3]; }

    inline T x() const { return _v[0]; }
    inline T y() const { return _v[1]; }
    inline T z() const { return _v[2]; }
    inline T w() const { return _v[3]; }

	inline T& r() { return _v[0]; }
	inline T& g() { return _v[1]; }
	inline T& b() { return _v[2]; }
	inline T& a() { return _v[3]; }

	inline T r() const { return _v[0]; }
	inline T g() const { return _v[1]; }
	inline T b() const { return _v[2]; }
	inline T a() const { return _v[3]; }

    inline unsigned long asABGR() const
    {
      return (unsigned long)clampTo((_v[0]*255.0f),0.0f,255.0f)<<24 |
        (unsigned long)clampTo((_v[1]*255.0f),0.0f,255.0f)<<16 |
        (unsigned long)clampTo((_v[2]*255.0f),0.0f,255.0f)<<8  |
        (unsigned long)clampTo((_v[3]*255.0f),0.0f,255.0f);
    }

    inline unsigned long asRGBA() const
    {
      return (unsigned long)clampTo((_v[3]*255.0f),0.0f,255.0f)<<24 |
        (unsigned long)clampTo((_v[2]*255.0f),0.0f,255.0f)<<16 |
        (unsigned long)clampTo((_v[1]*255.0f),0.0f,255.0f)<<8  |
        (unsigned long)clampTo((_v[0]*255.0f),0.0f,255.0f);
    }

    inline bool valid() const { return !isNaN(); }
    inline bool isNaN() const { return CRCore::isNaN(_v[0]) || CRCore::isNaN(_v[1]) || CRCore::isNaN(_v[2]) || CRCore::isNaN(_v[3]); }

    /// dot product
    inline T operator * (const crVector4_T<T>& rhs) const
    {
      return _v[0]*rhs._v[0]+
        _v[1]*rhs._v[1]+
        _v[2]*rhs._v[2]+
        _v[3]*rhs._v[3] ;
    }

    /// multiply by scalar
    inline crVector4_T<T> operator * (T rhs) const
    {
      return crVector4_T<T>(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
    }

    /// unary multiply by scalar
    inline crVector4_T<T>& operator *= (T rhs)
    {
      _v[0]*=rhs;
      _v[1]*=rhs;
      _v[2]*=rhs;
      _v[3]*=rhs;
      return *this;
    }

    /// divide by scalar
    inline crVector4_T<T> operator / (T rhs) const
    {
      return crVector4_T<T>(_v[0]/rhs, _v[1]/rhs, _v[2]/rhs, _v[3]/rhs);
    }

    /// unary divide by scalar
    inline crVector4_T<T>& operator /= (T rhs)
    {
      _v[0]/=rhs;
      _v[1]/=rhs;
      _v[2]/=rhs;
      _v[3]/=rhs;
      return *this;
    }

    /// binary vector add
    inline crVector4_T<T> operator + (const crVector4_T<T>& rhs) const
    {
      return crVector4_T<T>(_v[0]+rhs._v[0], _v[1]+rhs._v[1],
        _v[2]+rhs._v[2], _v[3]+rhs._v[3]);
    }

    /** unary vector add.  Slightly more efficient because no temporary
    intermediate object*/
    inline crVector4_T<T>& operator += (const crVector4_T<T>& rhs)
    {
      _v[0] += rhs._v[0];
      _v[1] += rhs._v[1];
      _v[2] += rhs._v[2];
      _v[3] += rhs._v[3];
      return *this;
    }

    /// binary vector subtract
    inline crVector4_T<T> operator - (const crVector4_T<T>& rhs) const
    {
      return crVector4_T<T>(_v[0]-rhs._v[0], _v[1]-rhs._v[1],
        _v[2]-rhs._v[2], _v[3]-rhs._v[3] );
    }

    /// unary vector subtract
    inline crVector4_T<T>& operator -= (const crVector4_T<T>& rhs)
    {
      _v[0]-=rhs._v[0];
      _v[1]-=rhs._v[1];
      _v[2]-=rhs._v[2];
      _v[3]-=rhs._v[3];
      return *this;
    }

    /// negation operator.  Returns the negative of the crVector4_T<T>
    inline const crVector4_T<T> operator - () const
    {
      return crVector4_T<T> (-_v[0], -_v[1], -_v[2], -_v[3]);
    }

    /// Length of the vector = sqrt( vec . vec )
    inline T length() const
    {
      return sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);
    }

    /// Length squared of the vector = vec . vec
    inline T length2() const
    {
      return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3];
    }

    /** normalize the vector so that it has length unity
    returns the previous length of the vector*/
    inline const crVector4_T<T>& normalize()
    {
      T norm = crVector4_T<T>::length();
      if (norm>0.0f)
      {
        T inv = 1.0f/norm;
        _v[0] *= inv;
        _v[1] *= inv;
        _v[2] *= inv;
        _v[3] *= inv;
      }
      return *this;
    }

    friend inline std::ostream& operator << (std::ostream& output, const crVector4_T<T>& vec)
    {
      output << vec._v[0] << " "
        << vec._v[1] << " "
        << vec._v[2] << " "
        << vec._v[3];
      return output; 	// to enable cascading
    }

};	// end of class crVector4_T<T>
/** Compute the dot product of a (crVector3,1.0) and a crVector4_T<T>.*/
template<class T>
inline T operator * (const crVector3_T<T>& lhs,const crVector4_T<T>& rhs)
{
	return lhs[0]*rhs[0]+lhs[1]*rhs[1]+lhs[2]*rhs[2]+rhs[3];
}

/** Compute the dot product of a crVector4_T<T> and a (crVector3,1.0).*/
template<class T>
inline T operator * (const crVector4_T<T>& lhs,const crVector3_T<T>& rhs)
{
	return lhs[0]*rhs[0]+lhs[1]*rhs[1]+lhs[2]*rhs[2]+lhs[3];
}

  typedef crVector4_T<float>  crVector4;
  typedef crVector4_T<int>    crVector4i;
  typedef crVector4_T<float>  crVector4f;
  typedef crVector4_T<double> crVector4d;

  typedef crVector4_T<char> crVector4b;
  typedef crVector4_T<unsigned char> crVector4ub;
  typedef crVector4_T<short> crVector4s;

}	// end of namespace CRCore

#endif
