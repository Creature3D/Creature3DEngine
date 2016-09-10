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
#include <CRPhysics/crSetGravityModePPC.h>
#include <CRPhysics\crMatterGroup.h>
#include <CRPhysics\crMatterObject.h>
#include <ode/ode.h>

using namespace CRCore;
using namespace CRPhysics;

class SetGravityModeVisitor : public CRCore::crNodeVisitor
{
public:

	SetGravityModeVisitor(int mode):
	  crNodeVisitor(PHYSICS_UPDATE_VISITOR,TRAVERSE_ALL_CHILDREN),
	  m_mode(mode){}

	  virtual ~SetGravityModeVisitor(){}

	  virtual void apply(crMatterGroup& node){ traverse(node); }
	  virtual void apply(crMatterObject& node)
	  {
		  dBodySetGravityMode(node.getBodyID(),m_mode);
	  }
	  void setMode( int mode ) { m_mode = mode; }
protected:
    int m_mode;
};

bool crSetGravityModePPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		if(eg.getCurrentMsg()==WCH_SETGRAVITYMODE)
		{
			SetGravityModeVisitor setGravityModeVisitor((int)(eg.getParam()));
			matterGroup->accept(setGravityModeVisitor);
			return true;
		}
	}

	return false;
}

crEventMsg *crSetGravityModePPC::makeEventMsg()
{
	crEventMsg* s_eventMsg = new crEventMsg(WCH_SETGRAVITYMODE);
	CRCore::ref_ptr<crEvent> s_event = new crEvent(0,-1); 
	CRCore::ref_ptr<crSetGravityModePPC> s_ecb = new crSetGravityModePPC;

	s_event->setParseParamCallback(s_ecb.get());
	s_eventMsg->m_event = s_event;

	return s_eventMsg;
}