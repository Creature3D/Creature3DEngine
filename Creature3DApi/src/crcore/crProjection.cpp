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

#include <CRCore/crProjection.h>

using namespace CRCore;

crProjection::crProjection()
{
}

crProjection::crProjection(const crProjection& projection,const crCopyOp& copyop):
    crGroup(projection,copyop),
    m_matrix(projection.m_matrix)
{    
}

crProjection::crProjection(const crMatrix& mat )
{
    m_matrix = mat;
}


crProjection::~crProjection()
{
}
