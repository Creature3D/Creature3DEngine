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
#include <CRCore/crLightModel.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crVector4.h>
using namespace CRCore;

crLightModel::crLightModel():
          crStateAttribute(),
          m_ambient(0.2f,0.2f,0.2f,1.0f),
          m_colorControl(crLightModel::SINGLE_COLOR),
          m_localViewer(false),
          m_twoSided(false)
{
}


crLightModel::~crLightModel()
{
}

void crLightModel::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		{
			crVector4 vec4;
			crArgumentParser::appAtoVec(str,vec4);
			m_ambient = vec4;
		}
		break;
	case 1:
		if(str.compare("SEPARATE_SPECULAR_COLOR") == 0)
			m_colorControl = SEPARATE_SPECULAR_COLOR;
		else m_colorControl = SINGLE_COLOR;
		break;
	case 2:
		m_localViewer = atoi(str.c_str());
		break;
	case 3:
		m_twoSided = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-4,str);
}
#include <Driver/GLDrv/crLightModelDrv.h>