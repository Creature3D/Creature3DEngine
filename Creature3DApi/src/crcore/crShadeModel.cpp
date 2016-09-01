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

#include <CRCore/crShadeModel.h>

using namespace CRCore;

crShadeModel::crShadeModel(Mode mode)
{
  m_mode = mode;
}


crShadeModel::~crShadeModel()
{
}

//void crShadeModel::apply(State&) const
//{
//  glShadeModel((GLenum)_mode);
//}

void crShadeModel::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("FLAT")==0)
			m_mode = FLAT;
		else if(str.compare("SMOOTH")==0)
			m_mode = SMOOTH;
		break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
#include <Driver/GLDrv/crShadeModelDrv.h>