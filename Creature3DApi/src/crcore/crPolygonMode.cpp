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
#include <CRCore/crPolygonMode.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

crPolygonMode::crPolygonMode()
{
  m_frontAndBack = true;
  m_frontMode = FILL;
  m_backMode = FILL;
}


crPolygonMode::~crPolygonMode()
{
}

void crPolygonMode::setMode(Face face,Mode mode)
{
  switch(face)
  {
  case(FRONT):
    m_frontAndBack = false;
    m_frontMode = mode;
    break;
  case(BACK):
    m_frontAndBack = false;
    m_backMode = mode;
    break;
  case(FRONT_AND_BACK):
    m_frontAndBack = true;
    m_frontMode = mode;
    m_backMode = mode;
    break;
  }    
}

crPolygonMode::Mode crPolygonMode::getMode(Face face) const
{
  switch(face)
  {
  case(FRONT):
    return m_frontMode;
  case(BACK):
    return m_backMode;
  case(FRONT_AND_BACK):
    return m_frontMode;
  }    
  notify(WARN)<<"Warning : invalid Face passed to crPolygonMode::getMode(Face face)"<<std::endl;
  return m_frontMode;
}

void crPolygonMode::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		m_frontAndBack = atoi(str.c_str());
		break;
	case 1:
		if(str.compare("POINT") == 0)
			m_frontMode = POINT;
		else if (str.compare("LINE") == 0)
			m_frontMode = LINE;
		else if (str.compare("FILL") == 0)
			m_frontMode = FILL;
		break;
	case 2:
		if(str.compare("POINT") == 0)
			m_backMode = POINT;
		else if (str.compare("LINE") == 0)
			m_backMode = LINE;
		else if (str.compare("FILL") == 0)
			m_backMode = FILL;
		break;
	}

	return crStateAttribute::addAttributeParam(i-3,str);
}

//void crPolygonMode::apply(crState&) const
//{
//  //if (_frontAndBack)
//  //{
//  //  glPolygonMode(GL_FRONT_AND_BACK,(GLenum)_modeFront);
//  //}
//  //else
//  //{
//  //  glPolygonMode(GL_FRONT,(GLenum)_modeFront);
//  //  glPolygonMode(GL_BACK,(GLenum)_modeBack);
//  //}
//}
#include <Driver/GLDrv/crPolygonModeDrv.h>