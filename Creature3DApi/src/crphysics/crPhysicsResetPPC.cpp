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
#include <CRPhysics/crPhysicsResetPPC.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>

using namespace CRCore;
using namespace CRPhysics;

bool crPhysicsResetPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR&&eg.getCurrentMsg()==WCH_PHYSICSRESET)
	{
		crJoint *joint_dest;
		crJoint *joint_src;
		_crInt64 param = eg.getCurrentRelayMsg()->m_lparam;

		joint_src = (crJoint*)(LOINT64(param));

		crMatterGroup::JointMap &jointMap = matterGroup->getJointMap();

		for( crMatterGroup::JointMap::iterator itr = jointMap.begin(); 
			 itr != jointMap.end(); 
			 ++itr )
		{//reset joint
			joint_dest = itr->second.get();
			joint_dest->setJointParameters( joint_src->getJointParameter1(),
				joint_src->getJointParameter2(),
				joint_src->getJointParameter3() );
			joint_dest->registJointParam();
		}	

		//reset rotation/Position
       
		crMatterObject *mo;
        CRCore::crMatrixd newmat;
		for( CRCore::NodeArray::iterator nitr = matterGroup->getChildArray().begin();
			 nitr != matterGroup->getChildArray().end();
			 ++nitr )
		{
			mo = dynamic_cast<crMatterObject *>(nitr->get());

			if(mo)
			{
                newmat.setTrans(mo->getMatrix().getTrans());
				mo->setMatrix(newmat);
			}
		}
		return true;
	}

	return false;
}

crEventMsg *crPhysicsResetPPC::makeEventMsg()
{
	crEventMsg* s_eventMsg = new crEventMsg(WCH_PHYSICSRESET);
    CRCore::ref_ptr<crEvent> s_event = new crEvent(0,-1); 
	CRCore::ref_ptr<crPhysicsResetPPC> s_ecb = new crPhysicsResetPPC;

	s_event->setParseParamCallback(s_ecb.get());
	s_eventMsg->m_event = s_event;

	return s_eventMsg;
}
