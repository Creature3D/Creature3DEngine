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
#include <CRCore/crMatrixd.h>
#include <CRCore/crMatrixf.h>

// specialise Matrix_implementaiton to be crMatrixd
#define  Matrix_implementation crMatrixd

CRCore::crMatrixd::crMatrixd( const CRCore::crMatrixf& mat )
:crBase(mat)
{
    set(mat.ptr());
}

CRCore::crMatrixd& CRCore::crMatrixd::operator = (const CRCore::crMatrixf& rhs)
{
    set(rhs.ptr());
    return *this;
}

void CRCore::crMatrixd::set(const CRCore::crMatrixf& rhs)
{
    set(rhs.ptr());
}

// now compile up Matrix via Matrix_implementation
#include <CRCore/Matrix_implementation.cpp>
