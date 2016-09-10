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
#include <stdio.h>
#include <CRCore/crQuat.h>
#include <CRCore/crVector4.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMatrixf.h>
#include <CRCore/crMatrixd.h>

#include <math.h>


using namespace CRCore;


void crQuat::set(const crMatrixf& matrix)
{
  matrix.get(*this);
}

void crQuat::set(const crMatrixd& matrix)
{
  matrix.get(*this);
}

void crQuat::get(crMatrixf& matrix) const
{
  matrix.set(*this);
}

void crQuat::get(crMatrixd& matrix) const
{
  matrix.set(*this);
}


/// Set the elements of the crQuat to represent a rotation of angle
/// (radians) around the axis (x,y,z)
void crQuat::makeRotate( value_type angle, value_type x, value_type y, value_type z )
{
  value_type inversenorm  = 1.0/sqrt( x*x + y*y + z*z );
  value_type coshalfangle = cos( 0.5*angle );
  value_type sinhalfangle = sin( 0.5*angle );

  _v[0] = x * sinhalfangle * inversenorm;
  _v[1] = y * sinhalfangle * inversenorm;
  _v[2] = z * sinhalfangle * inversenorm;
  _v[3] = coshalfangle;
}


void crQuat::makeRotate( value_type angle, const crVector3& vec )
{
  makeRotate( angle, vec[0], vec[1], vec[2] );
}


void crQuat::makeRotate ( value_type angle1, const crVector3& axis1, 
                       value_type angle2, const crVector3& axis2,
                       value_type angle3, const crVector3& axis3)
{
  crQuat q1; q1.makeRotate(angle1,axis1);
  crQuat q2; q2.makeRotate(angle2,axis2);
  crQuat q3; q3.makeRotate(angle3,axis3);

  *this = q1*q2*q3;
}                        

// Make a rotation crQuat which will rotate vec1 to vec2
// Generally take adot product to get the angle between these
// and then use a cross product to get the rotation axis
// Watch out for the two special cases of when the vectors
// are co-incident or opposite in direction.
void crQuat::makeRotate( const crVector3& from, const crVector3& to )
{
  const value_type epsilon = 0.00001;

  value_type length1  = from.length();
  value_type length2  = to.length();

  // dot product vec1*vec2
  value_type cosangle = from*to/(length1*length2);

  if ( fabs(cosangle - 1) < epsilon )
  {
    // cosangle is close to 1, so the vectors are close to being coincident
    // Need to generate an angle of zero with any vector we like
    // We'll choose (1,0,0)
    makeRotate( 0.0, 1.0, 0.0, 0.0 );
  }
  else
    if ( fabs(cosangle + 1.0) < epsilon )
    {
      // vectors are close to being opposite, so will need to find a
      // vector orthongonal to from to rotate about.
      CRCore::crVector3 tmp;
      if (fabs(from.x())<fabs(from.y()))
        if (fabs(from.x())<fabs(from.z())) tmp.set(1.0,0.0,0.0); // use x axis.
        else tmp.set(0.0,0.0,1.0);
      else if (fabs(from.y())<fabs(from.z())) tmp.set(0.0,1.0,0.0);
      else tmp.set(0.0,0.0,1.0);

      // find orthogonal axis.
      crVector3 axis(from^tmp);
      axis.normalize();

      _v[0] = axis[0]; // sin of half angle of PI is 1.0.
      _v[1] = axis[1]; // sin of half angle of PI is 1.0.
      _v[2] = axis[2]; // sin of half angle of PI is 1.0.
      _v[3] = 0; // cos of half angle of PI is zero.

    }
    else
    {
      // This is the usual situation - take a cross-product of vec1 and vec2
      // and that is the axis around which to rotate.
      crVector3 axis(from^to);
      value_type angle = acos( cosangle );
      makeRotate( angle, axis );
    }
}


void crQuat::getRotate( value_type& angle, crVector3& vec ) const
{
  value_type x,y,z;
  getRotate(angle,x,y,z);
  vec[0]=x;
  vec[1]=y;
  vec[2]=z;
}


// Get the angle of rotation and axis of this crQuat object.
// Won't give very meaningful results if the crQuat is not associated
// with a rotation!
void crQuat::getRotate( value_type& angle, value_type& x, value_type& y, value_type& z ) const
{
  value_type sinhalfangle = sqrt( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );

  angle = 2.0 * atan2( sinhalfangle, _v[3] );
  if(sinhalfangle)
  {
    x = _v[0] / sinhalfangle;
    y = _v[1] / sinhalfangle;
    z = _v[2] / sinhalfangle;
  }
  else
  {
    x = 0.0;
    y = 0.0;
    z = 1.0;
  }

}

void crQuat::getRadians( float& x, float& y, float& z ) const
{
	x=asin(2.0*(_v[1]*_v[2]-_v[0]*_v[3]));
	y=atan(-2.0*(_v[2]*_v[3]+_v[0]*_v[1])/(_v[0]*_v[0]-_v[1]*_v[1]+_v[2]*_v[2]-_v[3]*_v[3]));
	z=atan(-2.0*(_v[1]*_v[3]+_v[0]*_v[2])/(_v[0]*_v[0]+_v[1]*_v[1]-_v[2]*_v[2]-_v[3]*_v[3]));
}

void crQuat::getAngles( float& x, float& y, float& z ) const
{
	x=RadiansToDegrees(asin(2.0*(_v[1]*_v[2]-_v[0]*_v[3])));
    y=RadiansToDegrees(atan(-2.0*(_v[2]*_v[3]+_v[0]*_v[1])/(_v[0]*_v[0]-_v[1]*_v[1]+_v[2]*_v[2]-_v[3]*_v[3])));
	z=RadiansToDegrees(atan(-2.0*(_v[1]*_v[3]+_v[0]*_v[2])/(_v[0]*_v[0]+_v[1]*_v[1]-_v[2]*_v[2]-_v[3]*_v[3])));
}


/// Spherical Linear Interpolation
/// As t goes from 0 to 1, the crQuat object goes from "from" to "to"
/// Reference: Shoemake at SIGGRAPH 89
/// See also
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
void crQuat::slerp( value_type t, const crQuat& from, const crQuat& to )
{
  const double epsilon = 0.00001;
  double omega, cosomega, sinomega, scale_from, scale_to ;

  CRCore::crQuat quatTo(to);
  // this is a dot product

  cosomega = from.asVec4() * to.asVec4();

  if ( cosomega <0.0 )
  { 
    cosomega = -cosomega; 
    quatTo = -to;
  }

  if( (1.0 - cosomega) > epsilon )
  {
    omega= acos(cosomega) ;  // 0 <= omega <= Pi (see man acos)
    sinomega = sin(omega) ;  // this sinomega should always be +ve so
    // could try sinomega=sqrt(1-cosomega*cosomega) to avoid a sin()?
    scale_from = sin((1.0-t)*omega)/sinomega ;
    scale_to = sin(t*omega)/sinomega ;
  }
  else
  {
    /* --------------------------------------------------
    The ends of the vectors are very close
    we can use simple linear interpolation - no need
    to worry about the "spherical" interpolation
    -------------------------------------------------- */
    scale_from = 1.0 - t ;
    scale_to = t ;
  }

  *this = (from*scale_from) + (quatTo*scale_to);

  // so that we get a crVector4
}


#define QX  _v[0]
#define QY  _v[1]
#define QZ  _v[2]
#define QW  _v[3]


#ifdef CRCORE_USE_UNIT_TESTS
void test_Quat_Eueler(value_type heading,value_type pitch,value_type roll)
{
  CRCore::crQuat q;
  q.makeRotate(heading,pitch,roll);

  CRCore::Matrix q_m;
  q.get(q_m);

  CRCore::crVector3 xAxis(1,0,0);
  CRCore::crVector3 yAxis(0,1,0);
  CRCore::crVector3 zAxis(0,0,1);

  cout << "heading = "<<heading<<"  pitch = "<<pitch<<"  roll = "<<roll<<endl;

  cout <<"q_m = "<<q_m;
  cout <<"xAxis*q_m = "<<xAxis*q_m << endl;
  cout <<"yAxis*q_m = "<<yAxis*q_m << endl;
  cout <<"zAxis*q_m = "<<zAxis*q_m << endl;

  CRCore::Matrix r_m = CRCore::Matrix::rotate(roll,0.0,1.0,0.0)*
    CRCore::Matrix::rotate(pitch,1.0,0.0,0.0)*
    CRCore::Matrix::rotate(-heading,0.0,0.0,1.0);

  cout << "r_m = "<<r_m;
  cout <<"xAxis*r_m = "<<xAxis*r_m << endl;
  cout <<"yAxis*r_m = "<<yAxis*r_m << endl;
  cout <<"zAxis*r_m = "<<zAxis*r_m << endl;

  cout << endl<<"*****************************************" << endl<< endl;

}

void test_Quat()
{

  test_Quat_Eueler(CRCore::DegreesToRadians(20),0,0);
  test_Quat_Eueler(0,CRCore::DegreesToRadians(20),0);
  test_Quat_Eueler(0,0,CRCore::DegreesToRadians(20));
  test_Quat_Eueler(CRCore::DegreesToRadians(20),CRCore::DegreesToRadians(20),CRCore::DegreesToRadians(20));
  return 0;
}
#endif
