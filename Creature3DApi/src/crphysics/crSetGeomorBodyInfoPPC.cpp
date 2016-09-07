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
#include <CRPhysics/crSetGeomorBodyInfoPPC.h>

#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>
#include <CRCore/crMatrix.h>
#include <ode/ode.h>

using namespace CRCore;
using namespace CRPhysics;

bool crSetGeomorBodyInfoPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		if(eg.getCurrentMsg()==WCH_GEOMORBODYINFO)
		{
			_crInt64 wparam = eg.getCurrentRelayMsg()->m_wparam;
			_crInt64 lparam = eg.getCurrentRelayMsg()->m_lparam;
			_crInt32 lwparam = LOINT64(wparam);
			_crInt32 hwparam = HIINT64(wparam);
           
			crMatterObject *matterObject = NULL;
			if(hwparam <0 || hwparam>= matterGroup->getNumChildren()) 
				return false;

			matterObject = dynamic_cast<crMatterObject *>(matterGroup->getChild(hwparam));

			if(!matterObject)
		        return false;

			switch(lwparam) 
			{
			case WCH_GEOMMATRIX:
				{
                    crMatrixf *matrix = (crMatrixf *)(LOINT64(lparam));
				    matterObject->setMatrix(*matrix);
				}
				break;
			case WCH_BODYMATRIX:
				{
				    crMatrixf *matrix = (crMatrixf *)(LOINT64(lparam));
				    matterObject->setMatrix(*matrix);
				}
				break;
			case WCH_GEOMBOXPARAM:
				{
                    dGeomID geom = matterObject->getGeomID();
					if(!geom) return false;
					crVector3f *vec3 = (crVector3f *)(LOINT64(lparam));
					dGeomBoxSetLengths(geom,(*vec3)[0],(*vec3)[1],(*vec3)[2]);
				}
				break;
			case WCH_BODYCHANGEMASS:
				{
					//Translate mass
					/*dMass *mass;
					dBodyGetMass(m_bodyID,mass);
					dVector3 vc3;
					dGeomBoxGetLengths(m_geomID,vc3);
					dMassTranslate(mass,0,0,-vc3[2]+0.001);*/

					//dBodyID body = matterObject->getBodyID();
					//if(!body) return false;

				 //   float *mass = (float *)(LOINT64(lparam));
					//dMass m;	
				 //   dMassAdjust(&m,*mass);
					//dBodySetMass(body,&m);
				}
				break;
			}

			return true;
		}
	}

	return false;
}

crEventMsg *crSetGeomorBodyInfoPPC::makeEventMsg()
{
	crEventMsg* eventMsg = new crEventMsg(WCH_GEOMORBODYINFO);
    CRCore::ref_ptr<crEvent> event = new crEvent(0,-1); 
	CRCore::ref_ptr<crSetGeomorBodyInfoPPC> ecb = new crSetGeomorBodyInfoPPC;

	event->setParseParamCallback(ecb.get());
	eventMsg->m_event = event;

	return eventMsg;
}
