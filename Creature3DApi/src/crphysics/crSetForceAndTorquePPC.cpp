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
#include <CRPhysics/crSetForceAndTorquePPC.h>

#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>
#include <ode/ode.h>

using namespace CRCore;
using namespace CRPhysics;

bool crSetForceAndTorquePPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		if(eg.getCurrentMsg()==WCH_FORCEANDTORQUE)
		{
			_crInt64 lparam = eg.getCurrentRelayMsg()->m_lparam;
			_crInt64 wparam = eg.getCurrentRelayMsg()->m_wparam;

            _crInt32 index = LOINT64(wparam);

			crVector3f *force = (crVector3f*)(LOINT64(lparam));
			crVector3f *torque = (crVector3f*)(HIINT64(lparam));

			crMatterObject *matterObject = NULL;

			if(index == -1)
			{
				for( CRCore::NodeArray::iterator itr=matterGroup->getChildArray().begin();
					 itr!=matterGroup->getChildArray().end();
					 ++itr )
				{
                    matterObject = dynamic_cast<crMatterObject *>(itr->get());
					if(matterObject)
					{
						dBodyID bodyID = matterObject->getBodyID();

						if(force) dBodyAddForce(bodyID,force->x(),force->y(),force->z());
						if(torque) dBodyAddTorque(bodyID,torque->x(),torque->y(),torque->z());
					}
				}
			}
			else if(matterGroup->getNumChildren()>index)
			{
			    matterObject = dynamic_cast<crMatterObject *>(matterGroup->getChild(index));

			    if(matterObject)
				{
				    dBodyID bodyID = matterObject->getBodyID();

			        if(force) dBodyAddForce(bodyID,force->x(),force->y(),force->z());
                    if(torque) dBodyAddTorque(bodyID,torque->x(),torque->y(),torque->z());
				}
			}
			return true;
		}
	}

	return false;
}

crEventMsg *crSetForceAndTorquePPC::makeEventMsg()
{
	crEventMsg* s_eventMsg = new crEventMsg(WCH_FORCEANDTORQUE);
    CRCore::ref_ptr<crEvent> s_event = new crEvent(0,-1); 
	CRCore::ref_ptr<crSetForceAndTorquePPC> s_ecb = new crSetForceAndTorquePPC;

	s_event->setParseParamCallback(s_ecb.get());
	s_eventMsg->m_event = s_event;

	return s_eventMsg;
}
