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
#ifndef CRPHYSICS_CRCARMATTEROBJECT_H
#define CRPHYSICS_CRCARMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crViewMatterObject.h>

namespace CRPhysics{
class CRPHYSICS_EXPORT crCarMatterObject : public crViewMatterObject
{
public:
	crCarMatterObject();
	crCarMatterObject(const crCarMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCarMatterObject);

	virtual CRCore::crMatrixd getViewMatrix();
	virtual CRCore::crVector3f getViewDirection();//返回视点方向向量，可用于如射击方向定位

	virtual void releaseObjects(CRCore::crState* state);
	virtual void releaseOde();
	virtual void setEnableIntersect( bool enableIntersect);

protected:
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

};
}
#endif