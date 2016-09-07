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
#include <CRPhysics/crGeneralParamTransferPPC.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crGameBodyInfo.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crWeaponNode.h>

#include <CRCore/crMatrix.h>
#include <ode/ode.h>

using namespace CRCore;
using namespace CRPhysics;
//sendEventMessage(nervecell->getAttachedEventGroup(),WCH_GENERALPARAMTRANSFER,MAKEINT64(WCH_SQUAT,NULL),NULL);
bool crGeneralParamTransferPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		if(eg.getCurrentMsg()==WCH_GENERALPARAMTRANSFER)
		{
			_crInt64 wparam = eg.getCurrentRelayMsg()->m_wparam;
			_crInt64 lparam = eg.getCurrentRelayMsg()->m_lparam;

			switch(LOINT64(wparam)) 
			{
			case WCH_VIEWMATRIXPARAM:
				{
					crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
					if(!vo)
						return false;

                    crVector2d *angle = (crVector2d *)(LOINT64(lparam));
				    vo->updateViewPitch((*angle)[1]);
				}
				break;
			case WCH_VIEWMATRIXOFFSET:
				{
					crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
					if(!vo)
						return false;

					float offset = *(float *)(LOINT64(lparam));
					vo->setViewOffset(offset);
					//if(offset == 1.5f) vo->setVisiable(false);
					//else vo->setVisiable(true);
				}
				break;
			case WCH_VIEWMATRIX:
				{
					crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
					if(!vo)
						return false;

					crMatrix *matrix = (crMatrix *)(LOINT64(lparam));
					vo->preMult(*matrix);
				}
				break;
			case WCH_VIEWMOVE:
				{
					//crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
					//if(!vo)
					//	return false;
					//vo->setGroundDetect(HIINT64(wparam));
					//vo->setVelocity(*(crVector2f *)(LOINT64(lparam)),HIINT64(lparam));
				}
				break;
			case WCH_VIEWROTATE:
				{
					crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
					if(!vo)
						return false;
                    vo->rotate(*(crVector2d *)(LOINT64(lparam)));
				}
				break;
			//case WCH_JUMP:
			//	{
			//		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
			//		if(!vo)
			//			return false;
			//		//if(vo->getCameraBind())
			//		//{
			//			CRAL::crSoundState *soundState = CRAL::crSoundManager::instance()->findSoundState("jump");
			//			if(soundState && !soundState->isPlaying()) 
			//			{
			//				CRCore::crVector3f pos = vo->getMatterObjectPosition();
			//				soundState->setPosition(pos);
			//				CRCore::crVector3f listener;
			//				CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
			//				CRCore::crVector3f dir = pos - listener;
			//				dir.normalize();
			//				soundState->setDirection(dir);
			//				soundState->setPlay(true);
			//			}
			//		//}
			//		vo->setLinearVel(crVector3f(0.0f,0.0f,3.5f), 0.8f);
			//	}
			//	break;
			//case WCH_SQUAT:
			//	{
			//		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(getMatterObject(matterGroup,0));
			//		if(!vo)
			//			return false;
			//		vo->squat();
			//	}
			}
			return true;
		}
	}

	return false;
}

 crMatterObject *crGeneralParamTransferPPC::getMatterObject(crMatterGroup *matterGroup,int id)
 {
	 if(id <0 || id >= matterGroup->getNumChildren()) 
		 return 0;
	 return dynamic_cast< crMatterObject * >(matterGroup->getChild(id));
 }

crEventMsg *crGeneralParamTransferPPC::makeEventMsg()
{
	crEventMsg* eventMsg = new crEventMsg(WCH_GENERALPARAMTRANSFER);
    CRCore::ref_ptr<crEvent> event = new crEvent(0,-1); 
	CRCore::ref_ptr<crGeneralParamTransferPPC> ecb = new crGeneralParamTransferPPC;

	event->setParseParamCallback(ecb.get());
	eventMsg->m_event = event;

	return eventMsg;
}
