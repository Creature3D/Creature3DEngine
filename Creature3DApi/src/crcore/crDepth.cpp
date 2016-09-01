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
#include <CRCore/crDepth.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crVector2.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crDepth::crDepth(Function func,double zNear, double zFar,bool writeMask):
    m_func(func),
    m_zNear(zNear),
    m_zFar(zFar),
    m_depthWriteMask(writeMask)
{
}

crDepth::~crDepth()
{
}


void crDepth::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("NEVER") == 0)
		    m_func = NEVER;
		else if (str.compare("LESS") == 0)
			m_func = LESS;
		else if (str.compare("EQUAL") == 0)
			m_func = EQUAL;		
		else if (str.compare("LEQUAL") == 0)
			m_func = LEQUAL;
		else if (str.compare("GREATER") == 0)
			m_func = GREATER;
		else if (str.compare("NOTEQUAL") == 0)
			m_func = NOTEQUAL;
		else if (str.compare("GEQUAL") == 0)
			m_func = GEQUAL;
		else if (str.compare("ALWAYS") == 0)
			m_func = ALWAYS;
		break;
	case 1:
		{
			crVector2 z;
            crArgumentParser::appAtoVec(str,z);
			m_zNear = z[0];
            m_zFar = z[1];
		}
		break;
	case 2:
		m_depthWriteMask = atoi(str.c_str());
		break;
	}

	return crStateAttribute::addAttributeParam(i-3,str);
}
#include <Driver/GLDrv/crDepthDrv.h>