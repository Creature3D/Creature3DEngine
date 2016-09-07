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
#include <CRPhysics/crJointSetParamPPC.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>

using namespace CRCore;
using namespace CRPhysics;

bool crJointSetParamPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR&&eg.getCurrentMsg()==WCH_JOINTSETPARAM)
	{
		crJoint *joint_dest;
		crJoint *joint_src;
		_crInt64 param = eg.getCurrentRelayMsg()->m_lparam;
        
		_crInt32 msgid = LOINT64(param);
		
		crMatterGroup::JointMap &jointMap = matterGroup->getJointMap();
		crMatterGroup::JointMap::iterator itr = jointMap.find(msgid);
		if(itr==jointMap.end()) return false;

		joint_src = (crJoint*)(HIINT64(param));

		for( ; itr!=jointMap.end() && itr->first == msgid; ++itr )
		{
			joint_dest = itr->second.get();
			joint_dest->setJointParameters( joint_src->getJointParameter1(),
				joint_src->getJointParameter2(),
				joint_src->getJointParameter3() );
			joint_dest->registJointParam();
		}	

		return true;
	}

	return false;
}

crEventMsg *crJointSetParamPPC::makeEventMsg()
{
	crEventMsg* s_eventMsg = new crEventMsg(WCH_JOINTSETPARAM);
    CRCore::ref_ptr<crEvent> s_event = new crEvent(0,-1); 
	CRCore::ref_ptr<crJointSetParamPPC> s_ecb = new crJointSetParamPPC;

	s_event->setParseParamCallback(s_ecb.get());
	s_eventMsg->m_event = s_event;

	return s_eventMsg;
}
