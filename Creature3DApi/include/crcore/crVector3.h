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
#ifndef CRCORE_VEC3
#define CRCORE_VEC3 1

#include <CRCore/crExport.h>
//#include <CRCore/crMath.h>
#include <iostream>

namespace CRCore {

  /** General purpose T triple for use as vertices, vectors and normals.
  Provides general maths operations from addition through to cross products.
  No support yet added for T * crVector3_T<T> - is it necessary?
  Need to define a non-member non-friend operator*  etc.
  crVector3_T<T> * T is okay
  */
template<class T>
class crVector3_T
{
  public:

    crVector3_T() { _v[0]=0; _v[1]=0; _v[2]=0;}
    crVector3_T(T x,T y,T z) { _v[0]=x; _v[1]=y; _v[2]=z; }

	crVector3_T(const crVector3_T<int>& v) { _v[0]=v._v[0];  _v[1]=v._v[1];  _v[2]=v._v[2]; }
	crVector3_T(const crVector3_T<float>& v) { _v[0]=v._v[0];  _v[1]=v._v[1];  _v[2]=v._v[2]; }
	crVector3_T(const crVector3_T<double>& v) { _v[0]=v._v[0];  _v[1]=v._v[1];  _v[2]=v._v[2]; }

    T _v[3];

	inline crVector3_T<T>& operator = (const crVector3_T<T>& v) { _v[0]=v._v[0];  _v[1]=v._v[1];  _v[2]=v._v[2]; return (*this); }

    inline bool operator == (const crVector3_T<T>& v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2]; }

    inline bool operator != (const crVector3_T<T>& v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2]; }

    inline bool operator <  (const crVector3_T<T>& v) const
    {
      if (_v[0]<v._v[0]) return true;
      else if (_v[0]>v._v[0]) return false;
      else if (_v[1]<v._v[1]) return true;
      else if (_v[1]>v._v[1]) return false;
      else return (_v[2]<v._v[2]);
    }
    inline bool operator <= (const crVector3_T<T>& v) const { return *this<v || *this==v; }

	inline bool operator >  (const crVector3_T<T>& v) const
	{
		return !(*this<=v);
	}

    inline T* ptr() { return _v; }
    inline const T* ptr() const { return _v; }

    inline void set( T x, T y, T z)
    {
      _v[0]=x; _v[1]=y; _v[2]=z;
    }

    inline T& operator [] (int i) { return _v[i]; }
    inline T operator [] (int i) const { return _v[i]; }

    inline T& x() { return _v[0]; }
    inline T& y() { return _v[1]; }
    inline T& z() { return _v[2]; }

    inline T x() const { return _v[0]; }
    inline T y() const { return _v[1]; }
    inline T z() const { return _v[2]; }

    inline bool valid() const { return !isNaN(); }
    inline bool isNaN() const { return CRCore::isNaN(_v[0]) || CRCore::isNaN(_v[1]) || CRCore::isNaN(_v[2]); }

    /// dot product
    inline T operator * (const crVector3_T<T>& rhs) const
    {
      return _v[0]*rhs._v[0]+_v[1]*rhs._v[1]+_v[2]*rhs._v[2];
    }

    /// cross product
    inline const crVector3_T<T> operator ^ (const crVector3_T<T>& rhs) const
    {
      return crVector3_T<T>(_v[1]*rhs._v[2]-_v[2]*rhs._v[1],
        _v[2]*rhs._v[0]-_v[0]*rhs._v[2] ,
        _v[0]*rhs._v[1]-_v[1]*rhs._v[0]);
    }

    /// multiply by scalar
    inline const crVector3_T<T> operator * (T rhs) const
    {
      return crVector3_T<T>(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs);
    }

    /// unary multiply by scalar
    inline crVector3_T<T>& operator *= (T rhs)
    {
      _v[0]*=rhs;
      _v[1]*=rhs;
      _v[2]*=rhs;
      return *this;
    }

    /// divide by scalar
    inline const crVector3_T<T> operator / (T rhs) const
    {
      return crVector3_T<T>(_v[0]/rhs, _v[1]/rhs, _v[2]/rhs);
    }

    /// unary divide by scalar
    inline crVector3_T<T>& operator /= (T rhs)
    {
      _v[0]/=rhs;
      _v[1]/=rhs;
      _v[2]/=rhs;
      return *this;
    }

    /// binary vector add
    inline const crVector3_T<T> operator + (const crVector3_T<T>& rhs) const
    {
      return crVector3_T<T>(_v[0]+rhs._v[0], _v[1]+rhs._v[1], _v[2]+rhs._v[2]);
    }

    /** unary vector add.  Slightly more efficient because no temporary
    intermediate object*/
    inline crVector3_T<T>& operator += (const crVector3_T<T>& rhs)
    {
      _v[0] += rhs._v[0];
      _v[1] += rhs._v[1];
      _v[2] += rhs._v[2];
      return *this;
    }

    /// binary vector subtract
    inline const crVector3_T<T> operator - (const crVector3_T<T>& rhs) const
    {
      return crVector3_T<T>(_v[0]-rhs._v[0], _v[1]-rhs._v[1], _v[2]-rhs._v[2]);
    }

    /// unary vector subtract
    inline crVector3_T<T>& operator -= (const crVector3_T<T>& rhs)
    {
      _v[0]-=rhs._v[0];
      _v[1]-=rhs._v[1];
      _v[2]-=rhs._v[2];
      return *this;
    }

    /// negation operator.  Returns the negative of the crVector3_T<T>
    inline const crVector3_T<T> operator - () const
    {
      return crVector3_T<T> (-_v[0], -_v[1], -_v[2]);
    }

    /// Length of the vector = sqrt( vec . vec )
    inline T length() const
    {
      return sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );
    }

    /// Length squared of the vector = vec . vec
    inline T length2() const
    {
      return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2];
    }

    /** normalize the vector so that it has length unity
    returns the previous length of the vector*/
    inline const crVector3_T<T>& normalize()
    {
      T norm = crVector3_T<T>::length();
      if (norm>0.0f)
      {
        T inv = 1.0f/norm;
        _v[0] *= inv;
        _v[1] *= inv;
        _v[2] *= inv;
      }                
      return *this;
    }

    friend inline std::ostream& operator << (std::ostream& output, const crVector3_T<T>& vec)
	{
		output << vec._v[0] << " "
			<< vec._v[1] << " "
			<< vec._v[2];
		return output; 	// to enable cascading

	}

};	// end of class crVector3_T<T>

  typedef crVector3_T<float>  crVector3;
  typedef crVector3_T<int>    crVector3i;
  typedef crVector3_T<float>  crVector3f;
  typedef crVector3_T<double> crVector3d;

  typedef crVector3_T<char> crVector3b;
  typedef crVector3_T<unsigned char> crVector3ub;
  typedef crVector3_T<short> crVector3s;

  const crVector3 X_AXIS(1.0f,0.0f,0.0f);
  const crVector3 Y_AXIS(0.0f,1.0f,0.0f);
  const crVector3 Z_AXIS(0.0f,0.0f,1.0f);

}	// end of namespace crCore

#endif
