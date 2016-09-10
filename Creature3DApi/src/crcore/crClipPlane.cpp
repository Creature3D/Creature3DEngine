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
#include <CRCore/crClipPlane.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crVector4.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crClipPlane::crClipPlane()
{
    m_clipPlane[0] = 0.0;
    m_clipPlane[1] = 0.0;
    m_clipPlane[2] = 0.0;
    m_clipPlane[3] = 0.0;
    m_clipPlaneNum = 0;
}


crClipPlane::~crClipPlane()
{
}


void crClipPlane::setClipPlaneNum(unsigned int num)
{
    m_clipPlaneNum = num;
}

unsigned int crClipPlane::getClipPlaneNum() const
{
    return m_clipPlaneNum;
}

void crClipPlane::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			m_clipPlane = vec4;
		}
		break;
	case 1:
		m_clipPlaneNum = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-2,str);
}
#include <Driver/GLDrv/crClipPlaneDrv.h>