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
#ifndef CRPHYSICS_CRGENERALPARAMTRANSFERPPC_H
#define CRPHYSICS_CRGENERALPARAMTRANSFERPPC_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crEventMsg.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>
namespace CRPhysics{

class CRPHYSICS_EXPORT crGeneralParamTransferPPC : public CRCore::ParseParamCallback
{
public:
	crGeneralParamTransferPPC(){}
	crGeneralParamTransferPPC(const crGeneralParamTransferPPC& callback,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	  CRCore::ParseParamCallback(callback,copyop){}

	META_PPC(CRPhysics,crGeneralParamTransferPPC)

	virtual bool parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv );

    static CRCore::crEventMsg *makeEventMsg();
    crMatterObject *getMatterObject(crMatterGroup *matterGroup,int id = 0);
};
}

#endif