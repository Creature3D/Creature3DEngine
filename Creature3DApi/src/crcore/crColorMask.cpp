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
#include <CRCore/crColorMask.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crColorMask::crColorMask()
{
    m_red = true;
    m_green = true;
    m_blue = true;
    m_alpha = true;
}


crColorMask::~crColorMask()
{
}

void crColorMask::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_red = atoi(str.c_str());
		break;
	case 1:
		m_green = atoi(str.c_str());
		break;
	case 2:
		m_blue = atoi(str.c_str());
		break;
	case 3:
		m_alpha = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-4,str);
}
#include <Driver/GLDrv/crColorMaskDrv.h>
