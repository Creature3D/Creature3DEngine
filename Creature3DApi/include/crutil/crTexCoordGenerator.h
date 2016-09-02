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
#ifndef CRUTIL_TEXCOORDGENERATOR_H
#define CRUTIL_TEXCOORDGENERATOR_H

#include <CRUtil/crExport.h>

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crBoundingSphere.h>

namespace CRUtil
{

    // this visitor generates texture coordinates for all geometries in a
    // subgraph. It is used only for demo purposes.
    class CRUTIL_EXPORT crTexCoordGenerator: public CRCore::crNodeVisitor 
	{
    public:
        crTexCoordGenerator(int du = 1, int nu = 2, float scale = 1.0f): 
		    crNodeVisitor(crNodeVisitor::TRAVERSE_ALL_CHILDREN), 
		    m_du(du), 
			m_nu(nu),
			m_scale(scale) {}
		
		void setDiffuseUnit(int du){ m_du = du; }
		int getDiffuseUnit(){ return m_du; }

		void setNormalUnit(int nu){ m_nu = nu; }
		int getNormalUnit(){ return m_nu; }

		void setScale(float scale){ m_scale = scale; }
		float getScale(){ return m_scale; }

        virtual void apply(CRCore::crObject &geode);

		virtual void apply(CRCore::crGeometry &geo);

    protected:
        CRCore::Vec2Array *generate_coords(CRCore::Array *vx, CRCore::Array *nx, float scale);

	private:
		int   m_du;
		int   m_nu;
		float m_scale;
	};

}

#endif
