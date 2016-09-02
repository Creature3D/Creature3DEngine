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
#include <CRCore/crHint.h>
#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crHint::crHint():
m_hintTarget(POLYGON_SMOOTH_HINT),
m_hintMode(DONT_CARE)
{
}

crHint::crHint(unsigned int hintTarget, unsigned int hintMode):
m_hintTarget(hintTarget),
m_hintMode(hintMode)
{
}

crHint::~crHint()
{
}

void crHint::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("PERSPECTIVE_CORRECTION_HINT") == 0)
			m_hintTarget = PERSPECTIVE_CORRECTION_HINT;
		else if (str.compare("POINT_SMOOTH_HINT") == 0)
			m_hintTarget = POINT_SMOOTH_HINT;
		else if (str.compare("LINE_SMOOTH_HINT") == 0)
			m_hintTarget = LINE_SMOOTH_HINT;
		else if (str.compare("POLYGON_SMOOTH_HINT") == 0)
			m_hintTarget = POLYGON_SMOOTH_HINT;
		else if (str.compare("FOG_HINT") == 0)
			m_hintTarget = FOG_HINT;
		else if (str.compare("PHONG_HINT") == 0)
			m_hintTarget = PHONG_HINT;
		else if (str.compare("FRAGMENT_SHADER_DERIVATIVE_HINT") == 0)
			m_hintTarget = FRAGMENT_SHADER_DERIVATIVE_HINT;
		break;
	case 1:
		if(str.compare("DONT_CARE") == 0)
			m_hintMode = DONT_CARE;
		else if (str.compare("FASTEST") == 0)
			m_hintMode = FASTEST;
		else if (str.compare("NICEST") == 0)
			m_hintMode = NICEST;
		break;
	}

	return crStateAttribute::addAttributeParam(i-2,str);
}
#include <Driver/GLDrv/crHintDrv.h>