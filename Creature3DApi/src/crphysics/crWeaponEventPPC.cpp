/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRPhysics/crWeaponEventPPC.h>
#include <CRPhysics/crGameBodyInfo.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics\crWeaponNode.h>
using namespace CRCore;
using namespace CRPhysics;

bool crWeaponEventPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR&&eg.getCurrentMsg()==WCH_WEAPONEVENT)
	{
		_crInt64 lparam = eg.getCurrentRelayMsg()->m_lparam;

		crViewMatterObject *firePerson = dynamic_cast<crViewMatterObject *>(matterGroup->getMainMatterObject());
		//nervecell->sendEventMessage(nervecell->getAttachedEventGroup(),WCH_WEAPONEVENT,NULL,MAKEINT64(WCH_WEAPON_FIRE,&m_mousePickPoint));		
		
		if(firePerson)
		{
			crWeaponMatterObject * weapon = firePerson->getCurrentWeaponMatterObject();
			if(weapon)
			{
				switch(LOINT64(lparam)) 
				{
				case WCH_WEAPON_FIRE:
					{
						//weapon->setGunPoint(firePerson->getMatterObjectPosition());
						crVector3 mousePickPoint = *(crVector3 *)(HIINT64(lparam));
						weapon->fire(matterGroup,mousePickPoint/*-firePerson->getViewDirection()*/,crFrameStamp::getInstance()->getReferenceTime());
						//firePerson->refreshBulletCountInfo();
					}
					return true;
				case WCH_WEAPON_ESPECIAL:
					{
						//weapon->setGunPoint(firePerson->getMatterObjectPosition());
						crVector3 mousePickPoint = *(crVector3 *)(HIINT64(lparam));
						weapon->especialFire(matterGroup,mousePickPoint/*-firePerson->getViewDirection()*/,crFrameStamp::getInstance()->getReferenceTime());
						//firePerson->refreshBulletCountInfo();
					}
					return true;
				case WCH_WEAPON_REPLACECLIP:
					weapon->replaceClip(matterGroup,crFrameStamp::getInstance()->getReferenceTime());
					//firePerson->refreshBulletCountInfo();
					return true;
				case WCH_WEAPON_FIRE_LT:
					{
						crGameBodyInfo *bodyInfo = firePerson->getGameBodyInfo();
						if(bodyInfo) 
						{
                            //crViewMatterObject *target = bodyInfo->getCurrentLockedTarget();
							//bodyInfo->lockBodyInfo();
							crGameBodyInfo::LockedTargetSet &lockedSet = bodyInfo->getLockedTargetSet();
							//CRCore::notify(CRCore::ALWAYS)<<"WCH_WEAPONEVENT WCH_WEAPON_FIRE_LT firePerson = "<<firePerson->getName()<<" targetsize = "<<lockedSet.size()<<std::endl;
							if(!lockedSet.empty())
							{
								if(weapon->getBulletObject()->getAttrMask() & crBulletObject::Track)
								{
									weapon->fire(matterGroup,lockedSet,crFrameStamp::getInstance()->getReferenceTime());
								}
								else
								{
                                    weapon->fire(matterGroup,(*lockedSet.begin())->getBound().center()/* + bodyInfo->getTargetOffset()*/,crFrameStamp::getInstance()->getReferenceTime());
									//weapon->fire_noTarget(matterGroup,bodyInfo->getTargetOffset(),nv.getFrameStamp()->getReferenceTime());
								}
							}
							else
							{//滤镜出现没有攻击目标的提示
								//CRCore::notify(CRCore::ALWAYS)<<"WCH_WEAPON_FIRE_LT no target "<<std::endl;
								weapon->fire_noTarget(matterGroup,bodyInfo->getTargetOffset(),crFrameStamp::getInstance()->getReferenceTime());
							}
			                //bodyInfo->unlockBodyInfo();
							//firePerson->refreshBulletCountInfo();
						}
					}
					return true;
				case WCH_WEAPON_FIRE_T:
					{
						if(HIINT64(lparam)) 
						{
							crViewMatterObject* target = (crViewMatterObject *)(HIINT64(lparam));
							if(weapon->getBulletObject()->getAttrMask() & crBulletObject::Track)
							{
								weapon->fire(matterGroup,target,crFrameStamp::getInstance()->getReferenceTime());
							}
							else
							{
								weapon->fire(matterGroup,target->getTrans(),crFrameStamp::getInstance()->getReferenceTime());
							}
							//bodyInfo->unlockBodyInfo();
							//firePerson->refreshBulletCountInfo();
						}
					}
					return true;
				}
			}
			if(LOINT64(lparam) == WCH_WEAPON_SELECT)
			{
				firePerson->selectWeaponMatterObject(*(std::string*)(HIINT64(lparam)));
				return true;
		    }
		}
		return false;
	}

	return false;
}

crEventMsg *crWeaponEventPPC::makeEventMsg()
{
	crEventMsg* eventMsg = new crEventMsg(WCH_WEAPONEVENT);
    CRCore::ref_ptr<crEvent> event = new crEvent(0,-1); 
	CRCore::ref_ptr<crWeaponEventPPC> ecb = new crWeaponEventPPC;

	event->setParseParamCallback(ecb.get());
	eventMsg->m_event = event;

	return eventMsg;
}

////////////////////////
//
//crWeaponFiringPPC
//
///////////////////////////
bool crWeaponFiringPPC::parseParamImplementation( CRCore::crEventGroup& eg, CRCore::crNodeVisitor& nv )
{
	CRPhysics::crMatterGroup *matterGroup = dynamic_cast<CRPhysics::crMatterGroup*>(&eg);
	if(!matterGroup) return false;

	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR&&eg.getCurrentMsg()==WCH_WEAPONFIRING)
	{
		_crInt64 lparam = eg.getCurrentRelayMsg()->m_lparam;

		crViewMatterObject *firePerson = dynamic_cast<crViewMatterObject *>(matterGroup->getMainMatterObject());

		if(firePerson)
		{
			crWeaponMatterObject * weapon = firePerson->getCurrentWeaponMatterObject();
			if(weapon)
			{
				switch(LOINT64(lparam)) 
				{
				case WCH_WEAPON_FIRE:
					{
						//weapon->setGunPoint(firePerson->getMatterObjectPosition());
						crVector3 mousePickPoint = *(crVector3 *)(HIINT64(lparam));
						weapon->fire(matterGroup,mousePickPoint/*-firePerson->getViewDirection()*/,crFrameStamp::getInstance()->getReferenceTime());
						//firePerson->refreshBulletCountInfo();
					}
					return true;
				case WCH_WEAPON_ESPECIAL:
					{
						//weapon->setGunPoint(firePerson->getMatterObjectPosition());
						crVector3 mousePickPoint = *(crVector3 *)(HIINT64(lparam));
						weapon->especialFire(matterGroup,mousePickPoint/*-firePerson->getViewDirection()*/,crFrameStamp::getInstance()->getReferenceTime());
						//firePerson->refreshBulletCountInfo();
					}
					return true;
				case WCH_WEAPON_FIRE_LT:
					{
						crGameBodyInfo *bodyInfo = firePerson->getGameBodyInfo();
						if(bodyInfo) 
						{
							//crViewMatterObject *target = bodyInfo->getCurrentLockedTarget();
				            //bodyInfo->lockBodyInfo();
							crGameBodyInfo::LockedTargetSet &lockedSet = bodyInfo->getLockedTargetSet();
							//CRCore::notify(CRCore::ALWAYS)<<"WCH_WEAPON_FIRE_LT firePerson = "<<firePerson->getName()<<" targetsize = "<<lockedSet.size()<<std::endl;
							//return true;
							if(!lockedSet.empty())
							{
								if(weapon->getBulletObject()->getAttrMask() & crBulletObject::Track)
								{
									weapon->fire(matterGroup,lockedSet,crFrameStamp::getInstance()->getReferenceTime());
								}
								else
								{
									weapon->fire(matterGroup,(*lockedSet.begin())->getBound().center()/* + bodyInfo->getTargetOffset()*/,crFrameStamp::getInstance()->getReferenceTime());
									//weapon->fire_noTarget(matterGroup,bodyInfo->getTargetOffset(),nv.getFrameStamp()->getReferenceTime());
								}
							}
							else
							{//滤镜出现没有攻击目标的提示
								//CRCore::notify(CRCore::ALWAYS)<<"WCH_WEAPON_FIRE_LT no target "<<std::endl;
								weapon->fire_noTarget(matterGroup,bodyInfo->getTargetOffset(),crFrameStamp::getInstance()->getReferenceTime());
							}
						    //bodyInfo->unlockBodyInfo();
							//firePerson->refreshBulletCountInfo();
						}
					}
					return true;
				case WCH_WEAPON_FIRE_T:
					{
						if(HIINT64(lparam)) 
						{
							crViewMatterObject* target = (crViewMatterObject *)(HIINT64(lparam));
							if(weapon->getBulletObject()->getAttrMask() & crBulletObject::Track)
							{
								weapon->fire(matterGroup,target,crFrameStamp::getInstance()->getReferenceTime());
							}
							else
							{
								weapon->fire(matterGroup,target->getTrans(),crFrameStamp::getInstance()->getReferenceTime());
							}
							//bodyInfo->unlockBodyInfo();
							//firePerson->refreshBulletCountInfo();
						}
					}
					return true;
				}
			}
		}
		return false;
	}

	return false;
}

crEventMsg *crWeaponFiringPPC::makeEventMsg()
{
	crEventMsg* eventMsg = new crWhile_EventMsg(WCH_WEAPONFIRING);
	CRCore::ref_ptr<crEvent> event = new crEvent(0,-1); 
	CRCore::ref_ptr<crWeaponFiringPPC> ecb = new crWeaponFiringPPC;

	event->setParseParamCallback(ecb.get());
	eventMsg->m_event = event;

	return eventMsg;
}
