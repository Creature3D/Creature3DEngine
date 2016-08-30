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
#ifndef CRNETAPP_NETROLEMANIPULATORPPC_H
#define CRNETAPP_NETROLEMANIPULATORPPC_H 1

#include <CRNetApp\appExport.h>
#include <CRCore\crEventMsg.h>
#include <CRCore\crTimer.h>
#include <CRCore\crVector2.h>
#include <CRAI/crNerveCellGroup.h>
#include <CRAI/crBodyManipulatorPPC.h>
#include <CRAI/crMoveTaskManager.h>
namespace CRNetApp{

class CRNETAPP_EXPORT crNetRoleManipulatorPPC : public CRCore::ParseParamCallback
{
public:
	crNetRoleManipulatorPPC();
	crNetRoleManipulatorPPC(const crNetRoleManipulatorPPC& callback,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY):
	  CRCore::ParseParamCallback(callback,copyop){}

	META_PPC(CRNetApp,crNetRoleManipulatorPPC)

	virtual bool parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv );

	virtual CRCore::crEventMsg *virtualMakeEventMsg();
	static CRCore::crEventMsg *makeEventMsg();
	virtual void addParam(int i, const std::string &str);
protected:
	virtual ~crNetRoleManipulatorPPC();
};
}

#endif