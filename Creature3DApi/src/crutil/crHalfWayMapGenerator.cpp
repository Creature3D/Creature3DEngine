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
#include <CRUtil/crHalfWayMapGenerator.h>

using namespace CRUtil;

crHalfWayMapGenerator::crHalfWayMapGenerator(const CRCore::crVector3 &light_direction, int texture_size):	
    crCubeMapGenerator(texture_size),
	m_ldir(light_direction)
{
	m_ldir.normalize();
}

crHalfWayMapGenerator::crHalfWayMapGenerator(const crHalfWayMapGenerator &copy, const CRCore::crCopyOp &copyop):	
    crCubeMapGenerator(copy, copyop),
	m_ldir(copy.m_ldir)
{
}
