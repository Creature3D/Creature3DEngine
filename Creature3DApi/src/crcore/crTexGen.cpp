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
#include <CRCore/crTexGen.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;

crTexGen::crTexGen()
{
    m_mode = OBJECT_LINEAR;
    m_plane_s.set(1.0f, 0.0f, 0.0f, 0.0f);
    m_plane_t.set(0.0f, 1.0f, 0.0f, 0.0f);
    m_plane_r.set(0.0f, 0.0f, 1.0f, 0.0f);
    m_plane_q.set(0.0f, 0.0f, 0.0f, 1.0f);
}


crTexGen::~crTexGen()
{
}


void crTexGen::setPlane(Coord which, const crPlane& plane)
{
    switch( which )
    {
        case S : m_plane_s = plane; break;
        case T : m_plane_t = plane; break;
        case R : m_plane_r = plane; break;
        case Q : m_plane_q = plane; break;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexGen::setPlane("<<(unsigned int)which<<","<<plane<<")"<<std::endl; break;
    }
}

const crPlane& crTexGen::getPlane(Coord which) const
{
    switch( which )
    {
        case S : return m_plane_s;
        case T : return m_plane_t;
        case R : return m_plane_r;
        case Q : return m_plane_q;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexGen::getPlane(which)"<<std::endl; return m_plane_r;
    }
}

crPlane& crTexGen::getPlane(Coord which)
{
    switch( which )
    {
        case S : return m_plane_s;
        case T : return m_plane_t;
        case R : return m_plane_r;
        case Q : return m_plane_q;
        default : notify(WARN)<<"Error: invalid 'which' passed crTexGen::getPlane(which)"<<std::endl; return m_plane_r;
    }
}

void crTexGen::setPlanesFromMatrix(const crMatrixd& matrix)
{
	m_plane_s.set(matrix(0,0),matrix(1,0),matrix(2,0),matrix(3,0));
	m_plane_t.set(matrix(0,1),matrix(1,1),matrix(2,1),matrix(3,1));
	m_plane_r.set(matrix(0,2),matrix(1,2),matrix(2,2),matrix(3,2));
	m_plane_q.set(matrix(0,3),matrix(1,3),matrix(2,3),matrix(3,3));    
}

//void crTexGen::apply(crState& state) const
//{
//	if(m_setAttributeCallback.valid())
//		m_setAttributeCallback->setAttribute( this, state );
//}

void crTexGen::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("OBJECT_LINEAR")==0)
			m_mode = OBJECT_LINEAR;
		else if(str.compare("EYE_LINEAR")==0)
			m_mode = EYE_LINEAR;
		else if(str.compare("SPHERE_MAP")==0)
			m_mode = SPHERE_MAP;
		else if(str.compare("NORMAL_MAP")==0)
			m_mode = NORMAL_MAP;
		else if(str.compare("REFLECTION_MAP")==0)
			m_mode = REFLECTION_MAP;
		break;
	case 1:
		{
			crVector4 fv;
		    crArgumentParser::appAtoVec(str,fv);
            m_plane_s.set(fv);
		}
		break;
	case 2:
		{
			crVector4 fv;
			crArgumentParser::appAtoVec(str,fv);
			m_plane_t.set(fv);
		}
		break;
	case 3:
		{
			crVector4 fv;
			crArgumentParser::appAtoVec(str,fv);
			m_plane_r.set(fv);
		}
		break;
	case 4:
		{
			crVector4 fv;
			crArgumentParser::appAtoVec(str,fv);
			m_plane_q.set(fv);
		}
		break;
	}

	return crStateAttribute::addAttributeParam(i-5,str);
}
#include <Driver/GLDrv/crTexGenDrv.h>