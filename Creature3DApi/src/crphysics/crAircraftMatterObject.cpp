/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <CRPhysics\crAircraftMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRCore\crMath.h>

using namespace CRPhysics;
using namespace CRCore;

crAircraftMatterObject::crAircraftMatterObject()
{
}

crAircraftMatterObject::crAircraftMatterObject(const crAircraftMatterObject &matterObject,const CRCore::crCopyOp& copyop):
	crViewMatterObject(matterObject,copyop)
{
}

CRCore::crMatrixd crAircraftMatterObject::getViewMatrix()
{
	return m_matrix;
}

CRCore::crVector3f crAircraftMatterObject::getViewDirection()
{//
	CRCore::crMatrixd rot_matrix;
	rot_matrix.makeRotate(-PI/160.0f,1,0,0);
	return getDirection() * rot_matrix;
}

void crAircraftMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	dMass m;
	m_bodyID = dBodyCreate(worldID);
	dMassSetBox (&m,m_density,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dMassAdjust (&m,m_mass);
	dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
	dBodySetMass (m_bodyID,&m);

	m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dGeomSetBody (m_geomID,m_bodyID);

	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	//CRCore::notify(CRCore::ALWAYS)<<"crAircraftMatterObject::_create "<<m_initPosition<<std::endl;

	crViewMatterObject::_create(worldID,spaceID);
}

void crAircraftMatterObject::releaseObjects(CRCore::crState* state)
{
	crViewMatterObject::releaseObjects(state);
}

void crAircraftMatterObject::releaseOde()
{
	crViewMatterObject::releaseOde();
}

void crAircraftMatterObject::setEnableIntersect( bool enableIntersect)
{
	crViewMatterObject::setEnableIntersect(enableIntersect);
}
