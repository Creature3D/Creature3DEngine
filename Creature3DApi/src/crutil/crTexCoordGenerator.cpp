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
#include <CRUtil/crTexCoordGenerator.h>

using namespace CRCore;
using namespace CRUtil;

void crTexCoordGenerator::apply(CRCore::crObject &geode)
{
    const CRCore::crBoundingSphere &bsphere = geode.getBound();
    float scale = m_scale;
	float radius = bsphere.radius();
    if (radius != 0) 
	{
        scale = m_scale / radius;
    }
    for (unsigned i=0; i<geode.getNumDrawables(); ++i) 
	{
        CRCore::crGeometry *geo = dynamic_cast<CRCore::crGeometry *>(geode.getDrawable(i));
        if (geo)
		{
			CRCore::ref_ptr<CRCore::Vec2Array> tc = generate_coords(geo->getVertexArray(), geo->getNormalArray(), scale);
			geo->setTexCoordArray(m_du, tc.get());
			geo->setTexCoordArray(m_nu, tc.get());
		}
    }
    crNodeVisitor::apply(geode);
}

void crTexCoordGenerator::apply(CRCore::crGeometry &geo)
{
	const CRCore::crBoundingBox &bbx = geo.getBound();
	float scale = m_scale;
	float radius = bbx.radius2();
	if ( radius != 0) 
	{
		scale = m_scale / radius;
	}

	CRCore::ref_ptr<CRCore::Vec2Array> tc = generate_coords(geo.getVertexArray(), geo.getNormalArray(), m_scale);
	geo.setTexCoordArray(m_du, tc.get());
	geo.setTexCoordArray(m_nu, tc.get());
}

CRCore::Vec2Array *crTexCoordGenerator::generate_coords(CRCore::Array *vx, CRCore::Array *nx, float scale)
{
    CRCore::Vec2Array *v2a = dynamic_cast<CRCore::Vec2Array *>(vx);
    CRCore::Vec3Array *v3a = dynamic_cast<CRCore::Vec3Array *>(vx);
    CRCore::Vec4Array *v4a = dynamic_cast<CRCore::Vec4Array *>(vx);
    CRCore::Vec2Array *n2a = dynamic_cast<CRCore::Vec2Array *>(nx);
    CRCore::Vec3Array *n3a = dynamic_cast<CRCore::Vec3Array *>(nx);
    CRCore::Vec4Array *n4a = dynamic_cast<CRCore::Vec4Array *>(nx);

    CRCore::ref_ptr<CRCore::Vec2Array> tc = new CRCore::Vec2Array;
    for (unsigned i=0; i<vx->getNumElements(); ++i) 
	{
        CRCore::crVector3 P;
        if (v2a) P.set((*v2a)[i].x(), (*v2a)[i].y(), 0);
		if (v3a) P.set((*v3a)[i].x(), (*v3a)[i].y(), (*v3a)[i].z());
		if (v4a) P.set((*v4a)[i].x(), (*v4a)[i].y(), (*v4a)[i].z());

		CRCore::crVector3 N(0, 0, 1);
		if (n2a) N.set((*n2a)[i].x(), (*n2a)[i].y(), 0);
		if (n3a) N.set((*n3a)[i].x(), (*n3a)[i].y(), (*n3a)[i].z());
		if (n4a) N.set((*n4a)[i].x(), (*n4a)[i].y(), (*n4a)[i].z());

		int axis = 0;
		if (N.y() > N.x() && N.y() > N.z()) axis = 1;
		if (-N.y() > N.x() && -N.y() > N.z()) axis = 1;
		if (N.z() > N.x() && N.z() > N.y()) axis = 2;
		if (-N.z() > N.x() && -N.z() > N.y()) axis = 2;

		CRCore::crVector2 uv;

		switch (axis) 
		{
		case 0: uv.set(P.y(), P.z()); break;
		case 1: uv.set(P.x(), P.z()); break;
		case 2: uv.set(P.x(), P.y()); break;
		default: ;
		}

		tc->push_back(uv * scale);
	}
	return tc.take();
}