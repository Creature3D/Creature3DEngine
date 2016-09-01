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
#include <CRCore/crViewport.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;

crViewPort::crViewPort()
{
    m_x = 0;
    m_y = 0;
    m_width = 800;
    m_height = 600;
}


crViewPort::~crViewPort()
{

}

void crViewPort::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			m_x = vec4[0];
			m_y = vec4[1];
			m_width = vec4[2];
			m_height = vec4[3];
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
#include <Driver/GLDrv/crViewPortDrv.h>