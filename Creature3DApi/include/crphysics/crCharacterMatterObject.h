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
#ifndef CRPHYSICS_CRCHARACTERMATTEROBJECT_H
#define CRPHYSICS_CRCHARACTERMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRCore/crMath.h>
#include <CRCore/crObject.h>
#include <CRCore/crMatrix.h>

namespace CRPhysics{

class CRPHYSICS_EXPORT crCharacterMatterObject : public crViewMatterObject
{
public:
	crCharacterMatterObject();
	crCharacterMatterObject(const crCharacterMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCharacterMatterObject);

	virtual void releaseOde();
	virtual void updateOdeMatrix();
	
	enum DetectMode
	{
		None,
		DetectGeom,
		DetectBlock,
		DetectBody
	};
	inline DetectMode getDetectMode(dGeomID id)
	{
		if(id == m_geomID) 
			return DetectGeom;
		else if(id == m_blockGeomID)
			return DetectBlock;
		return None;
	}

	virtual void setEnableIntersect( bool enableIntersect);

	virtual void traverse(CRCore::crNodeVisitor& nv);
    
	void setBlockRadius(float blockRadius);
	//void setBlockHeight(float blockHeight);
protected:
	virtual ~crCharacterMatterObject(){}
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

	mutable dGeomID   m_blockGeomID;
	float m_blockRadius;
	//float m_blockHeight;
};

class CRPHYSICS_EXPORT crCharacterObjectNearCallback : public crMatterGroup::NearCallback
{
public:
	crCharacterObjectNearCallback();
	virtual crCharacterObjectNearCallback* cloneType() const { return NULL; };
	virtual const char* libraryName() const { return "CRPhysics"; }
	virtual const char* className() const { return "crCharacterObjectNearCallback"; }

	virtual void nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 );

	static crCharacterObjectNearCallback* instance();
	void setFriendDamage(bool friendDamage);
protected:
	bool m_friendDamage;
};

}

#endif