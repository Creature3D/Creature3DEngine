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
#include <CRCore/crMatrixf.h>
#include <CRCore/crMatrixd.h>

// specialise Matrix_implementaiton to be crMatrixf
#define  Matrix_implementation crMatrixf

CRCore::crMatrixf::crMatrixf( const CRCore::crMatrixd& mat )
:crBase(mat)
{
    set(mat.ptr());
}

CRCore::crMatrixf& CRCore::crMatrixf::operator = (const CRCore::crMatrixd& rhs)
{
    set(rhs.ptr());
    return *this;
}

void CRCore::crMatrixf::set(const CRCore::crMatrixd& rhs)
{
    set(rhs.ptr());
}

// now compile up Matrix via Matrix_implementation
#include <CRCore/Matrix_implementation.cpp>
