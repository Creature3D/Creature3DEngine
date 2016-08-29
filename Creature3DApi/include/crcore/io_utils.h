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
#ifndef CRCORE_IO_UTILS
#define CRCORE_IO_UTILS 1

#include <ostream>
#include <istream>

#include <CRCore/crVector4.h>
#include <CRCore/crMatrixf.h>
#include <CRCore/crMatrixd.h>
#include <CRCore/crPlane.h>

namespace CRCore {


//////////////////////////////////////////////////////////////////////////
// crVector2f streaming operators
inline std::ostream& operator << (std::ostream& output, const crVector2f& vec)
{
    output << vec._v[0] << " " << vec._v[1];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector2f& vec)
{
    input >> vec._v[0] >> vec._v[1];
    return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector2d steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector2d& vec)
{
    output << vec._v[0] << " " << vec._v[1];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector2d& vec)
{
    input >> vec._v[0] >> vec._v[1];
    return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector3f steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector3f& vec)
{
    output << vec._v[0] << " "
           << vec._v[1] << " "
           << vec._v[2];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector3f& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2];
    return input;
}


//////////////////////////////////////////////////////////////////////////
// crVector3d steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector3d& vec)
{
    output << vec._v[0] << " "
           << vec._v[1] << " "
           << vec._v[2];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector3d& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2];
    return input;
}


//////////////////////////////////////////////////////////////////////////
// crVector3f steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4f& vec)
{
    output  << vec._v[0] << " "
            << vec._v[1] << " "
            << vec._v[2] << " "
            << vec._v[3];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector4f& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
    return input;
}



//////////////////////////////////////////////////////////////////////////
// crVector4d steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4d& vec)
{
    output << vec._v[0] << " "
           << vec._v[1] << " "
           << vec._v[2] << " "
           << vec._v[3];
    return output;     // to enable cascading
}
inline std::istream& operator >> (std::istream& input, crVector4d& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
    return input;
}


//////////////////////////////////////////////////////////////////////////
// crVector2b steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector2b& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector2b& vec)
{
	input >> vec._v[0] >> vec._v[1];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector3b steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector3b& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector3b& vec)
{
	input >> vec._v[0] >> vec._v[1]  >> vec._v[2];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector4b steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4b& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2] << " "
		<< (int)vec._v[3];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector4b& vec)
{
	input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
	return input;
}


//////////////////////////////////////////////////////////////////////////
// crVector2s steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector2s& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector2s& vec)
{
	input >> vec._v[0] >> vec._v[1];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector3s steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector3s& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector3s& vec)
{
	input >> vec._v[0] >> vec._v[1]  >> vec._v[2];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector4s steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4s& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2] << " "
		<< (int)vec._v[3];
	return output; // to enable cascading
}

//////////////////////////////////////////////////////////////////////////
// crVector2i steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector2i& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector2i& vec)
{
	input >> vec._v[0] >> vec._v[1];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector3i steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector3i& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector3i& vec)
{
	input >> vec._v[0] >> vec._v[1]  >> vec._v[2];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// crVector4i steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4i& vec)
{
	output << (int)vec._v[0] << " "
		<< (int)vec._v[1] << " "
		<< (int)vec._v[2] << " "
		<< (int)vec._v[3];
	return output; // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector4i& vec)
{
	input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
	return input;
}

//////////////////////////////////////////////////////////////////////////
// Matrxf steaming operators.
inline std::ostream& operator<< (std::ostream& os, const crMatrixf& m )
{
    os << "{"<<std::endl;
    for(int row=0; row<4; ++row) {
        os << "\t";
        for(int col=0; col<4; ++col)
            os << m(row,col) << " ";
        os << std::endl;
    }
    os << "}" << std::endl;
    return os;
}


//////////////////////////////////////////////////////////////////////////
// Matrxd steaming operators.
inline std::ostream& operator<< (std::ostream& os, const crMatrixd& m )
{
    os << "{"<<std::endl;
    for(int row=0; row<4; ++row) {
        os << "\t";
        for(int col=0; col<4; ++col)
            os << m(row,col) << " ";
        os << std::endl;
    }
    os << "}" << std::endl;
    return os;
}

//////////////////////////////////////////////////////////////////////////
// crVector4ub steaming operators.
inline std::ostream& operator << (std::ostream& output, const crVector4ub& vec)
{
    output << (int)vec._v[0] << " "
           << (int)vec._v[1] << " "
           << (int)vec._v[2] << " "
           << (int)vec._v[3];
    return output; 	// to enable cascading
}

inline std::istream& operator >> (std::istream& input, crVector4ub& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
    return input;
}


//////////////////////////////////////////////////////////////////////////
// crQuat steaming operators.
inline std::ostream& operator << (std::ostream& output, const crQuat& vec)
{
    output << vec._v[0] << " "
       << vec._v[1] << " "
       << vec._v[2] << " "
       << vec._v[3];
    return output;     // to enable cascading
}

inline std::istream& operator >> (std::istream& input, crQuat& vec)
{
    input >> vec._v[0] >> vec._v[1] >> vec._v[2] >> vec._v[3];
    return input;
}



//////////////////////////////////////////////////////////////////////////
// crPlane steaming operators.
inline std::ostream& operator << (std::ostream& output, const crPlane& pl)
{
    output << pl[0] << " "
	   << pl[1] << " "
	   << pl[2] << " "
	   << pl[3];
    return output; 	// to enable cascading
}

inline std::istream& operator >> (std::istream& input, crPlane& vec)
{
    input >> vec[0] >> vec[1] >> vec[2] >> vec[3];
    return input;
}

}    // end of namespace CRCore
#endif
