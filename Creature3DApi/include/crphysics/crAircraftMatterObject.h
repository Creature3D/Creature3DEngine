/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#ifndef CRPHYSICS_CRAIRCRAFTMATTEROBJECT_H
#define CRPHYSICS_CRAIRCRAFTMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crViewMatterObject.h>

namespace CRPhysics{
class CRPHYSICS_EXPORT crAircraftMatterObject : public crViewMatterObject
{
public:
	crAircraftMatterObject();
	crAircraftMatterObject(const crAircraftMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crAircraftMatterObject);

	virtual CRCore::crMatrixd getViewMatrix();
	virtual CRCore::crVector3f getViewDirection();//返回视点方向向量，可用于如射击方向定位

	virtual void releaseObjects(CRCore::crState* state);
	virtual void releaseOde();
	virtual void setEnableIntersect( bool enableIntersect);
    
	//void 
    
protected:
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
    float m_thrust;//推进动力
	float m_k0;//加速时空气阻力系数，空气阻力 f = kv
	float m_k1;//减速时空气阻力系数，空气阻力 f = kv
	float m_k2;//爬升时空气阻力系数，空气阻力 f = kv
	float m_k3;//府冲时空气阻力系数，空气阻力 f = kv
	float m_k4;//转弯时空气阻力系数，空气阻力 f = kv
	mutable dGeomID   m_lWingGeomID;
	mutable dGeomID   m_RWingGeomID;
	mutable dGeomID   m_lEmpennageGeomID;
	mutable dGeomID   m_REmpennageGeomID;
	mutable dGeomID   m_upEmpennageGeomID;
	mutable dBodyID   m_lWingBodyID;
	mutable dGeomID   m_RWingBodyID;
	mutable dGeomID   m_lEmpennageBodyID;
	mutable dGeomID   m_REmpennageBodyID;
	mutable dGeomID   m_upEmpennageBodyID;

};
}
#endif