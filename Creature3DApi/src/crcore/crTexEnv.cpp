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
#include <CRCore/crTexEnv.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;

crTexEnv::crTexEnv(Mode mode)
{
    m_mode = mode;
    m_color.set(0.0f,0.0f,0.0f,0.0f);
}


crTexEnv::~crTexEnv()
{
}

void crTexEnv::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("NONE")==0)
			m_mode = NONE;
		else if(str.compare("DECAL")==0)
			m_mode = DECAL;
		else if(str.compare("MODULATE")==0)
			m_mode = MODULATE;
		else if(str.compare("BLEND")==0)
			m_mode = BLEND;
		else if(str.compare("REPLACE")==0)
			m_mode = REPLACE;
		else if(str.compare("ADD")==0)
			m_mode = ADD;
		else if(str.compare("COLOR")==0)
			m_mode = COLOR;
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_color);
		break;
	}

	return crStateAttribute::addAttributeParam(i-2,str);
}
#include <Driver/GLDrv/crTexEnvDrv.h>