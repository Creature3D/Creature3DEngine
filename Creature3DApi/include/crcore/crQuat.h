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
#ifndef CRCORE_CRQUAT_H
#define CRCORE_CRQUAT_H 1

#include <CRCore/crExport.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMath.h>
namespace CRCore {

  class crMatrixf;
  class crMatrixd;

  /** A quaternion class. It can be used to represent an orientation in 3D space.*/
  class CR_EXPORT crQuat
  {

  public:

    typedef double value_type;

    value_type  _v[4];	// a four-vector

    inline crQuat() { _v[0]=0.0; _v[1]=0.0; _v[2]=0.0; _v[3]=0.0; }

    inline crQuat( value_type x, value_type y, value_type z, value_type w )
    {
      _v[0]=x;
      _v[1]=y;
      _v[2]=z;
      _v[3]=w;
    }

    inline crQuat( const crVector4& v )
    {
      _v[0]=v.x();
      _v[1]=v.y();
      _v[2]=v.z();
      _v[3]=v.w();
    }

    inline crQuat( value_type angle, const crVector3& axis)
    {
      makeRotate(angle,axis);
    }

    inline crQuat( value_type angle1, const crVector3& axis1, 
      value_type angle2, const crVector3& axis2,
      value_type angle3, const crVector3& axis3)
    {
      makeRotate(angle1,axis1,angle2,axis2,angle3,axis3);
    }

    inline bool operator == (const crQuat& v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]; }

    inline bool operator != (const crQuat& v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]; }

    inline bool operator <  (const crQuat& v) const
    {
      if (_v[0]<v._v[0]) return true;
      else if (_v[0]>v._v[0]) return false;
      else if (_v[1]<v._v[1]) return true;
      else if (_v[1]>v._v[1]) return false;
      else if (_v[2]<v._v[2]) return true;
      else if (_v[2]>v._v[2]) return false;
      else return (_v[3]<v._v[3]);
    }

    /* ----------------------------------
    Methods to access data members
    ---------------------------------- */

    inline crVector4 asVec4() const
    {
      return crVector4(_v[0], _v[1], _v[2], _v[3]);
    }

    inline crVector3 asVec3() const
    {
      return crVector3(_v[0], _v[1], _v[2]);
    }

    inline void set(value_type x, value_type y, value_type z, value_type w)
    {
      _v[0]=x;
      _v[1]=y;
      _v[2]=z;
      _v[3]=w;
    }

    inline void set(const CRCore::crVector4& v)
    {
      _v[0]=v.x();
      _v[1]=v.y();
      _v[2]=v.z();
      _v[3]=v.w();
    }

    void set(const crMatrixf& matrix);

    void set(const crMatrixd& matrix);

    void get(crMatrixf& matrix) const;

    void get(crMatrixd& matrix) const;


    inline value_type & operator [] (int i) { return _v[i]; }
    inline value_type   operator [] (int i) const { return _v[i]; }

    inline value_type & x() { return _v[0]; }
    inline value_type & y() { return _v[1]; }
    inline value_type & z() { return _v[2]; }
    inline value_type & w() { return _v[3]; }

    inline float x() const { return _v[0]; }
    inline float y() const { return _v[1]; }
    inline float z() const { return _v[2]; }
    inline float w() const { return _v[3]; }

    /** return true if the crQuat represents a zero rotation, and therefore can be ignored in computations.*/
    bool zeroRotation() const { return _v[0]==0.0 && _v[1]==0.0 && _v[2]==0.0 && _v[3]==1.0; } 


    /* ------------------------------------------------------------- 
    BASIC ARITHMETIC METHODS		    
    Implemented in terms of Vec4s.  Some crVector4 operators, e.g.
    operator* are not appropriate for quaternions (as
    mathematical objects) so they are implemented differently.
    Also define methods for conjugate and the multiplicative inverse.		    
    ------------------------------------------------------------- */
    /// Multiply by scalar 
    inline const crQuat operator * (float rhs) const
    {
      return crQuat(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
    }

    /// Unary multiply by scalar 
    inline crQuat& operator *= (float rhs)
    {
      _v[0]*=rhs;
      _v[1]*=rhs;
      _v[2]*=rhs;
      _v[3]*=rhs;
      return *this;		// enable nesting
    }

    /// Binary multiply 
    inline const crQuat operator*(const crQuat& rhs) const
    {
      return crQuat( rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1],
        rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0],
        rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3],
        rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2] );
    }

    /// Unary multiply 
    inline crQuat& operator*=(const crQuat& rhs)
    {
      value_type x = rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1];
      value_type y = rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0];
      value_type z = rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3];
      _v[3]   = rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2];

      _v[2] = z;
      _v[1] = y;
      _v[0] = x;

      return (*this);			// enable nesting
    }

    /// Divide by scalar 
    inline crQuat operator / (float rhs) const
    {
      value_type div = 1.0/rhs;
      return crQuat(_v[0]*div, _v[1]*div, _v[2]*div, _v[3]*div);
    }

    /// Unary divide by scalar 
    inline crQuat& operator /= (float rhs)
    {
      value_type div = 1.0/rhs;
      _v[0]*=div;
      _v[1]*=div;
      _v[2]*=div;
      _v[3]*=div;
      return *this;
    }

    /// Binary divide 
    inline const crQuat operator/(const crQuat& denom) const
    {
      return ( (*this) * denom.inverse() );
    }

    /// Unary divide 
    inline crQuat& operator/=(const crQuat& denom)
    {
      (*this) = (*this) * denom.inverse();
      return (*this);			// enable nesting
    }

    /// Binary addition 
    inline const crQuat operator + (const crQuat& rhs) const
    {
      return crVector4(_v[0]+rhs._v[0], _v[1]+rhs._v[1],
        _v[2]+rhs._v[2], _v[3]+rhs._v[3]);
    }

    /// Unary addition
    inline crQuat& operator += (const crQuat& rhs)
    {
      _v[0] += rhs._v[0];
      _v[1] += rhs._v[1];
      _v[2] += rhs._v[2];
      _v[3] += rhs._v[3];
      return *this;			// enable nesting
    }

    /// Binary subtraction 
    inline const crQuat operator - (const crQuat& rhs) const
    {
      return crQuat(_v[0]-rhs._v[0], _v[1]-rhs._v[1],
        _v[2]-rhs._v[2], _v[3]-rhs._v[3] );
    }

    /// Unary subtraction 
    inline crQuat& operator -= (const crQuat& rhs)
    {
      _v[0]-=rhs._v[0];
      _v[1]-=rhs._v[1];
      _v[2]-=rhs._v[2];
      _v[3]-=rhs._v[3];
      return *this;			// enable nesting
    }

    /** Negation operator - returns the negative of the quaternion.
    Basically just calls operator - () on the crVector4 */
    inline const crQuat operator - () const
    {
      return crQuat (-_v[0], -_v[1], -_v[2], -_v[3]);
    }

    /// Length of the quaternion = sqrt( vec . vec )
    value_type length() const
    {
      return sqrt( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);
    }

    /// Length of the quaternion = vec . vec
    value_type length2() const
    {
      return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3];
    }

    /// Conjugate 
    inline crQuat conj () const
    { 
      return crQuat( -_v[0], -_v[1], -_v[2], _v[3] );
    }

    /// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
    inline const crQuat inverse () const
    {
      return conj() / length2();
    }

    /* -------------------------------------------------------- 
    METHODS RELATED TO ROTATIONS
    Set a quaternion which will perform a rotation of an
    angle around the axis given by the vector (x,y,z).
    Should be written to also accept an angle and a crVector3?

    Define Spherical Linear interpolation method also

    Not inlined - see the crQuat.cpp file for implementation
    -------------------------------------------------------- */
    void makeRotate( value_type  angle, 
      value_type  x, value_type  y, value_type  z );
    void makeRotate ( value_type  angle, const crVector3& vec );

    void makeRotate ( value_type  angle1, const crVector3& axis1, 
      value_type  angle2, const crVector3& axis2,
      value_type  angle3, const crVector3& axis3);

    /** Make a rotation crQuat which will rotate vec1 to vec2.
    Generally take adot product to get the angle between these
    and then use a cross product to get the rotation axis
    Watch out for the two special cases of when the vectors
    are co-incident or opposite in direction.*/
    void makeRotate( const crVector3& vec1, const crVector3& vec2 );

    /** Return the angle and vector components represented by the quaternion.*/
    void getRotate ( value_type & angle, value_type & x, value_type & y, value_type & z ) const;
    /** Return the angle and vector represented by the quaternion.*/
    void getRotate ( value_type & angle, crVector3& vec ) const;
	void getRadians( float& x, float& y, float& z ) const;
	void getAngles( float& x, float& y, float& z ) const;
    /** Spherical Linear Interpolation.
    As t goes from 0 to 1, the crQuat object goes from "from" to "to". */
    void slerp   ( value_type  t, const crQuat& from, const crQuat& to);

    friend inline std::ostream& operator << (std::ostream& output, const crQuat& vec);

  protected:

  };	// end of class prototype

  inline std::ostream& operator << (std::ostream& output, const crQuat& vec)
  {
    output << vec._v[0] << " "
      << vec._v[1] << " "
      << vec._v[2] << " "
      << vec._v[3];
    return output; 	// to enable cascading
  }

}	// end of namespace

#endif 
