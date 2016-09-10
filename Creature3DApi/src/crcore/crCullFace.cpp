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
#include <CRCore/crCullFace.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crCullFace::~crCullFace()
{
}
void crCullFace::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("FRONT")==0)
		{
			m_mode = FRONT;
		}
		else if(str.compare("BACK")==0)
		{
			m_mode = BACK;
		}
		else if(str.compare("FRONT_AND_BACK")==0)
		{
			m_mode = FRONT_AND_BACK;
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
#include <Driver/GLDrv/crCullFaceDrv.h>