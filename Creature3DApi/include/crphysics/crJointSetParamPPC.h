/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRPHYSICS_CRJOINTSETPARAMPPC_H
#define CRPHYSICS_CRJOINTSETPARAMPPC_H 1

#include <CRPhysics\crExport.h>
#include <CRCore\crEventMsg.h>

namespace CRPhysics{

class CRPHYSICS_EXPORT crJointSetParamPPC : public CRCore::ParseParamCallback
{
public:
	crJointSetParamPPC(){}
	crJointSetParamPPC(const crJointSetParamPPC& callback,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	    CRCore::ParseParamCallback(callback,copyop){}

	META_PPC(CRPhysics,crJointSetParamPPC)

	virtual bool parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv );

    static CRCore::crEventMsg *makeEventMsg();
};
}

#endif