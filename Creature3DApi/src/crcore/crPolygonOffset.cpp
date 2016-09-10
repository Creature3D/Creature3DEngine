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
#include <CRCore/crPolygonOffset.h>

using namespace CRCore;

crPolygonOffset::crPolygonOffset():
    m_factor(0.0f),
    m_units(0.0f)
{

}

crPolygonOffset::crPolygonOffset(float factor, float units):
    m_factor(factor),
    m_units(units)
{

}

crPolygonOffset::~crPolygonOffset()
{

}

void crPolygonOffset::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_factor = atof(str.c_str());
		break;
	case 1:
		m_units = atof(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-2,str);
}
#include <Driver/GLDrv/crPolygonOffsetDrv.h>