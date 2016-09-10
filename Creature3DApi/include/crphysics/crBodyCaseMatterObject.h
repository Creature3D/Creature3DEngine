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
#ifndef CRPHYSICS_CRBODYCASEMATTEROBJECT_H
#define CRPHYSICS_CRBODYCASEMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crDOFTransform.h>

namespace CRPhysics{

class CRPHYSICS_EXPORT crMissileSetOrFlackMatterObject : public crViewMatterObject
{
public:
	crMissileSetOrFlackMatterObject();
	crMissileSetOrFlackMatterObject(const crMissileSetOrFlackMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crMissileSetOrFlackMatterObject);

	virtual CRCore::crVector3f getDirection();
	virtual CRCore::crMatrix getCameraMatrix()const;
	virtual void traverse(CRCore::crNodeVisitor& nv);

	typedef std::vector< CRCore::ref_ptr<CRCore::crDOFTransform> > DofVec;
	void addDofTransform(CRCore::crDOFTransform *dof);
protected:
	//virtual void updateWeaponMatrix();
	void computerRotMatrix();

	DofVec m_dofVec;
	CRCore::crMatrix m_rotmat;
};

class CRPHYSICS_EXPORT crBodyCaseMatterObject : public crViewMatterObject
{
public:
	crBodyCaseMatterObject();
	crBodyCaseMatterObject(const crBodyCaseMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crBodyCaseMatterObject);
	virtual void traverse(CRCore::crNodeVisitor& nv);

	typedef std::map< std::string, std::string > BodyRackNameMap;//racknodename,bodyname
	void insertBodyRackName(const std::string &rackName, const std::string &bodyName);
	typedef std::map< CRCore::ref_ptr<crNode>, CRCore::ref_ptr<crViewMatterObject> > BodyRackMap;
protected:
	void updateBodyMatrix();
	BodyRackMap m_bodyRackMap;
	BodyRackNameMap m_bodyRackNameMap;
};
}
#endif