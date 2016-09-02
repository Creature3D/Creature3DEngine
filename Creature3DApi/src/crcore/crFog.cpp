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
//Modified by 吴财华
#include <CRCore/crFog.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crFog::crFog()
{
    m_mode = EXP;
    m_density = 1.0f;
    m_start   = 0.0f;
    m_end     = 1.0f;
    m_color.set( 0.0f, 0.0f, 0.0f, 0.0f);
    m_fogCoordinateSource = FRAGMENT_DEPTH;
}


crFog::~crFog()
{
}

void crFog::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("LINEAR") == 0)
			m_mode = LINEAR;
		else if (str.compare("EXP") == 0)
			m_mode = EXP;
		else if (str.compare("EXP2") == 0)
			m_mode = EXP2;
		break;
	case 1:
		m_density = atof(str.c_str());
		break;
	case 2:
		m_start = atof(str.c_str());
		break;
	case 3:
        m_end = atof(str.c_str());
		break;
	case 4:
        crArgumentParser::appAtoVec(str,m_color);
		break;
	case 5:
		if(str.compare("FOG_COORDINATE") == 0)
			m_fogCoordinateSource = FOG_COORDINATE;
		else if (str.compare("FRAGMENT_DEPTH") == 0)
			m_fogCoordinateSource = FRAGMENT_DEPTH;
		break;
	}

	return crStateAttribute::addAttributeParam(i-6,str);
}
#include <Driver/GLDrv/crFogDrv.h>