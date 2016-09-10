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
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crFilterRenderManager.h>
#include <CREncapsulation/crCollideCallback.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crWeaponManager.h>
#include <CRPhysics/crBodyCaseMatterObject.h>
#include <CRCore/crTimer.h>
//#include <CRAL/crSoundManager.h>
//#include <CRParticle/crParticleEffect.h>
#include <rbody/Creature3D/CreBody.h>
#include <CREncapsulation/crSceneManager.h>

using namespace CREncapsulation;
using namespace CRParticle;
using namespace CRCore;
using namespace CRPhysics;
//using namespace CRAL;
using namespace rbody;

//////////////////////////
//
//crCommonMatterObjectCollideCallback
//
///////////////////////////
void crCommonMatterObjectCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crMatterObject *mo = dynamic_cast<crMatterObject *>(node1);
	if(mo)
	{
		crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(node2);
		if(bulletMatterObject)
		{//如果是爆炸类，可以给更多时间
			const crBulletObject *bulletObject = bulletMatterObject->getBulletObject();
			switch(bulletObject->getExecutionPhysicsType())
			{
			case crBulletObject::Type_Collide:
				mo->enablePhysics(5.0f);
				break;
			case crBulletObject::Type_Explode:
				mo->enablePhysics(10.0f);
				break;
			case crBulletObject::Type_TimeExplode:
				mo->enablePhysics(10.0f);
				break;
			case crBulletObject::Type_Sphere:
				mo->enablePhysics(5.0f);
				break;
			case crBulletObject::Type_Ray:
				mo->enablePhysics(5.0f);
				//mo->addForce(collideNormal * (-3.0f),5.0f);
				break;
			case crBulletObject::Type_Weapon:
				mo->enablePhysics(5.0f);
				break;
			case crBulletObject::Type_Shield_Box:
				mo->enablePhysics(5.0f);
				break;
			case crBulletObject::Type_Shield_Sphere:
				mo->enablePhysics(5.0f);
				break;
			case crBulletObject::Type_Volume:
				mo->enablePhysics(15.0f);
				break;
			}
			goto quit;
		}
		crViewMatterObject *viewMatterObject = dynamic_cast<crViewMatterObject *>(node2);
		if(viewMatterObject)
		{
			mo->addForce(collideNormal * (-10.0f),5.0f);
			goto quit;
		}
		crMatterObject *matterObject = dynamic_cast<crViewMatterObject *>(node2);
		if(matterObject)
		{
			mo->enablePhysics(5.0f);
			goto quit;
		}

	}
quit:
	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
}

//////////////////////////
//
//crBulletCollideCallback
//
///////////////////////////
//crBulletCollideCallback *crBulletCollideCallback::getInstance()
//{
//	static ref_ptr<crBulletCollideCallback> s_manager = new crBulletCollideCallback;
//	return s_manager.get();
//}

void crBulletCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	//if(dynamic_cast<crBulletMatterObject *>(node1) && dynamic_cast<crBulletMatterObject *>(node2)) 	
	//{
	//    //CRCore::notify(CRCore::ALWAYS)<<"crBulletCollideCallback"<< std::endl;
	//	return;
	//}
    crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(node1);
	if(bulletMatterObject /*&& bulletMatterObject->getGeomID()==o1*/ && !bulletMatterObject->getCollided())
	{
		const crBulletObject *bulletObject = bulletMatterObject->getBulletObject();
		if(bulletObject->getAttrMask() & crBulletObject::Track)
		{//跟踪导弹，提示击中目标
			ref_ptr<crViewMatterObject> firePerson = bulletMatterObject->getFireWeapon()->getFireBody();
			bool isFirstPlayer = firePerson == crSceneManager::getInstance()->getPlayer();
			if(isFirstPlayer)
			{
				//if(!m_cautionSW.valid()) m_cautionSW = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode("Caution"));
				if(m_cautionSW.valid())
				{
					if(/*const_cast<crViewMatterObject *>(*/bulletMatterObject->getTrackTarget() == node2)
					{//滤镜提示Hit
						/*if(!m_hit.valid()) m_hit = crFilterRenderManager::getInstance()->getFilterNode("Hit");
						if(m_hit.valid()) m_hit->setVisiable(true);*/
						m_cautionSW->setActiveSwitchSet(4);
						m_cautionSW->getFirstActiveChild()->setVisiable(true);
					}
					else
					{//滤镜提示Miss
						/*if(!m_miss.valid()) m_miss = crFilterRenderManager::getInstance()->getFilterNode("Miss");
						if(m_miss.valid()) m_miss->setVisiable(true);*/
						m_cautionSW->setActiveSwitchSet(5);
						m_cautionSW->getFirstActiveChild()->setVisiable(true);
					}
				}
			}
		}
		//else if(dynamic_cast<crBulletMatterObject *>(node2))
		//{//非导弹不可被子弹击中，即不判断普通子弹之间的碰撞
		//	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos);
		//	return;
		//}

		crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();

		crBodyCaseMatterObject *bodyCase = dynamic_cast<crBodyCaseMatterObject *>(node2);
		if(!bodyCase && (bulletObject->getAttrMask() & crBulletObject::VanishWhenCollide))
		{
			//CRCore::notify(CRCore::ALWAYS)<<"setFlying(false) "<< std::endl;
			//if(!(bodyCase && /*pt != crBulletObject::Type_Explode &&*/ pt != crBulletObject::Type_TimeExplode))
			if(bulletMatterObject->getGeomID()==o1)
			{
				bulletMatterObject->setFlying(false);
				bulletMatterObject->setCollided(true);
			}
			else
			{
				bulletMatterObject->bulletGeomCollide();
			}
		}

		////////effect
		float fparam[6];
		fparam[0] = collidePos[0];
		fparam[1] = collidePos[1];
		fparam[2] = collidePos[2];
		fparam[3] = collideNormal[0];
		fparam[4] = collideNormal[1];
		fparam[5] = collideNormal[2];

		bulletMatterObject->doEvent(WCH_BULLET_COLLIDE,MAKEINT64(node2,fparam));

		crBulletObject::EffectAttrMask effectAttrMask = bulletObject->getEffectAttrMask();
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "stone";
		std::string effectString = bulletMatterObject->getName()+"_"+attrStr;
		if(effectAttrMask & crBulletObject::Collide_Particle)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->setEffectDirection(collideNormal);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Particle2)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString + "2");
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->setEffectDirection(collideNormal);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Particle3)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString + "3");
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->setEffectDirection(collideNormal);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Sequence)
		{
			//CRCore::notify(CRCore::ALWAYS)<<"Collide_Sequence "<<effectString<< std::endl;
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"m_sequenceNode "<<effectString<< std::endl;
				m_sequenceNode->setEffectPosition(collidePos);
				m_sequenceNode->startEffect();
			}
		}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(bulletMatterObject->getFireWeapon()->getFireBody()->getName(),effectString);
		//	if(m_soundState)
		//	{
		//		m_soundState->setPosition(collidePos);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//		m_soundState->setPlay(true);
		//	}
		//}

		///////
		if(pt == crBulletObject::Type_Explode)
		{
			bulletMatterObject->explode(collidePos);
		}
		else if(pt == crBulletObject::Type_Volume)
		{
			bulletMatterObject->explode(collidePos);
		}

		//////////////////子弹碰撞后的序列帧特效

	/*	CRCore::crSequence *sequence = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
		if(sequence)
		{
            sequence->setPosition(collidePos);
			sequence->startEffect();
		}

		//////////////////子弹碰撞后的粒子特效
		crParticleEffect *particleEffect = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
		if(particleEffect)
		{
			particleEffect->setEffectPosition(collidePos);
			particleEffect->startEffect();
		}

		CRAL::crSoundState *soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
		if(soundState && !soundState->isPlaying()) 
		{
			soundState->setPosition(collidePos);
			CRCore::crVector3f listener;
			CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
			CRCore::crVector3f dir = collidePos - listener;
			dir.normalize();
			soundState->setDirection(dir);
			soundState->setPlay(true);
		}
		*/
		//////////////////////////////////////

		if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
	}
}

//////////////////////////
//
//crWeaponCollideCallback
//
///////////////////////////
//crWeaponCollideCallback *crWeaponCollideCallback::getInstance()
//{
//	static ref_ptr<crWeaponCollideCallback> s_manager = new crWeaponCollideCallback;
//	return s_manager.get();
//}

void crWeaponCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crWeaponMatterObject *weaponMatterObject = dynamic_cast<crWeaponMatterObject *>(node1);

	if(weaponMatterObject)
	{
		const crWeaponObject *weaponObject = weaponMatterObject->getWeaponObject();

		crWeaponObject::EffectAttrMask effectAttrMask = weaponObject->getEffectAttrMask();
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "stone";
		std::string effectString = weaponMatterObject->getName()+"_"+attrStr;
		if(effectAttrMask & crWeaponObject::Collide_Particle)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Sequence)
		{
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				m_sequenceNode->setEffectPosition(collidePos);
				m_sequenceNode->startEffect();
			}
		}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
		//	if(m_soundState)
		//	{
		//		m_soundState->setPosition(collidePos);
		//		if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
		//			m_soundState->setPlay(true);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//	}
		//}
		//////////////////////////////////////
		if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
	}
}
//////////////////////////
//
//crCaissonCollideCallback
//
///////////////////////////
//crCaissonCollideCallback *crCaissonCollideCallback::getInstance()
//{
//	static ref_ptr<crCaissonCollideCallback> s_manager = new crCaissonCollideCallback;
//	return s_manager.get();
//}

void crCaissonCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crCaissonMatterObject *caissonMatterObject = dynamic_cast<crCaissonMatterObject *>(node1);

	if(caissonMatterObject)
	{
		const crCaissonObject *caissonObject = caissonMatterObject->getCaissonObject();

		crCaissonObject::EffectAttrMask effectAttrMask = caissonObject->getEffectAttrMask();
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "stone";
		std::string effectString = caissonMatterObject->getName()+"_"+attrStr;
		if(effectAttrMask & crWeaponObject::Collide_Particle)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Sequence)
		{
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				m_sequenceNode->setEffectPosition(collidePos);
				m_sequenceNode->startEffect();
			}
		}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
		//	if(m_soundState)
		//	{
		//		m_soundState->setPosition(collidePos);
		//		if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
		//			m_soundState->setPlay(true);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//	}
		//}
		//////////////////////////////////////
		if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
	}
}
//////////////////////////
//
//crHpCollideCallback
//
///////////////////////////
//crHpCollideCallback *crHpCollideCallback::getInstance()
//{
//	static ref_ptr<crHpCollideCallback> s_manager = new crHpCollideCallback;
//	return s_manager.get();
//}

void crHpCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crHpMatterObject *hpMatterObject = dynamic_cast<crHpMatterObject *>(node1);

	if(hpMatterObject)
	{
		const crHpObject *hpObject = hpMatterObject->getHpObject();

		crHpObject::EffectAttrMask effectAttrMask = hpObject->getEffectAttrMask();
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "stone";
		std::string effectString = hpMatterObject->getName()+"_"+attrStr;
		if(effectAttrMask & crWeaponObject::Collide_Particle)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Sequence)
		{
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				m_sequenceNode->setEffectPosition(collidePos);
				m_sequenceNode->startEffect();
			}
		}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
		//	if(m_soundState)
		//	{
		//		m_soundState->setPosition(collidePos);
		//		if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
		//			m_soundState->setPlay(true);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//	}
		//}
		//////////////////////////////////////
		if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
	}
}
//////////////////////////
//
//crArmorCollideCallback
//
///////////////////////////
//crArmorCollideCallback *crArmorCollideCallback::getInstance()
//{
//	static ref_ptr<crArmorCollideCallback> s_manager = new crArmorCollideCallback;
//	return s_manager.get();
//}

void crArmorCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crArmorMatterObject *armorMatterObject = dynamic_cast<crArmorMatterObject *>(node1);

	if(armorMatterObject)
	{
		const crArmorObject *armorObject = armorMatterObject->getArmorObject();

		crArmorObject::EffectAttrMask effectAttrMask = armorObject->getEffectAttrMask();
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "stone";
		std::string effectString = armorMatterObject->getName()+"_"+attrStr;
		if(effectAttrMask & crWeaponObject::Collide_Particle)
		{
			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(collidePos);
			//	m_particleEffectNode->startEffect();
			//}
		}
		if(effectAttrMask & crBulletObject::Collide_Sequence)
		{
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				m_sequenceNode->setEffectPosition(collidePos);
				m_sequenceNode->startEffect();
			}
		}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
		//	if(m_soundState)
		//	{
		//		m_soundState->setPosition(collidePos);
		//		if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
		//			m_soundState->setPlay(true);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//	}
		//}
		//////////////////////////////////////
		if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
	}
}

//////////////////////////
//
//crViewObjectCollideCallback
//
///////////////////////////
//crViewObjectCollideCallback *crViewObjectCollideCallback::getInstance()
//{
//	static ref_ptr<crViewObjectCollideCallback> s_manager = new crViewObjectCollideCallback;
//	return s_manager.get();
//}

//void crViewObjectCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
//{
//	crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node1);
//	if(vo&&!vo->isDead())
//	{
//		crWeaponMatterObject *weaponMatterObject = dynamic_cast<crWeaponMatterObject *>(node2);
//		if(weaponMatterObject)
//		{
//			weaponMatterObject->setVisiable(false);
//			weaponMatterObject->setEnableIntersect(false);
//
//			if(!vo->testToAddWeaponMatterObject(weaponMatterObject))
//			{
//				vo->addWeaponMatterObject(crWeaponManager::createWeaponMatterObject(weaponMatterObject));
//			}
//			goto quite;
//		}
//		crCaissonMatterObject *caissonMatterObject = dynamic_cast<crCaissonMatterObject *>(node2);
//		if(caissonMatterObject)
//		{
//			if(vo->addCaisson(caissonMatterObject))
//			{
//				caissonMatterObject->setVisiable(false);
//				caissonMatterObject->setEnableIntersect(false);
//			}
//			goto quite;
//		}
//		crHpMatterObject *hpMatterObject = dynamic_cast<crHpMatterObject *>(node2);
//		if(hpMatterObject)
//		{			
//			if(vo->addHp(hpMatterObject))
//			{
//				hpMatterObject->setVisiable(false);
//				hpMatterObject->setEnableIntersect(false);
//			}
//			goto quite;
//		}
//		crArmorMatterObject *armorMatterObject = dynamic_cast<crArmorMatterObject *>(node2);
//		if(armorMatterObject)
//		{
//			if(vo->addArmor(armorMatterObject))
//			{
//				armorMatterObject->setVisiable(false);
//				armorMatterObject->setEnableIntersect(false);
//			}
//			goto quite;
//		}
//		crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(node2);
//		if(bulletMatterObject)
//		{////////////////////////////////
//			if(bulletMatterObject->validExecution(vo,o2))
//			{
//				//const crBulletObject *bulletObject = bulletMatterObject->getBulletObject();
//				//crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();
//				//if(pt != crBulletObject::Type_Collide && o2 == bulletMatterObject->getGeomID()) goto quite;
//				//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator() o1 = "<<o1<<" o2 = "<<o2<<" bulletMatterObject = "<<bulletMatterObject->getName()<<" vo = "<<vo->getName()<<std::endl;
//				crViewMatterObject *firePerson = const_cast<crViewMatterObject *>(bulletMatterObject->getFireWeapon()->getFireBody());
//				//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator() firePerson = "<<firePerson<<std::endl;
//				/*CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(vo->getActiveBodyFirstChild());
//				if(bodyNode)
//				{
//					CreBodyNode::DetailCollide dc = bodyNode->getDetailCollide(collidePos,vo->getMatrix());
//					switch (dc)
//					{
//					case CreBodyNode::Collide_None:
//						excution = 0;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_None");
//						break;
//					case CreBodyNode::Collide_Head:
//						vo->kill(5);
//						crFilterRenderManager::getInstance()->addDequeString("Collide_Head");
//						goto quite;
//						break;
//					case CreBodyNode::Collide_Body:
//						crFilterRenderManager::getInstance()->addDequeString("Collide_Body");
//						break;
//					case CreBodyNode::Collide_LArms:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_LArms");
//						break;
//					case CreBodyNode::Collide_RArms:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_RArms");
//						break;
//					case CreBodyNode::Collide_LLegs:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_LLegs");
//						break;
//					case CreBodyNode::Collide_RLegs:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_RLegs");
//						break;
//					case CreBodyNode::Collide_LFoots:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_LFoots");
//						break;
//					case CreBodyNode::Collide_RFoots:
//						excution *= 0.5f;
//						crFilterRenderManager::getInstance()->addDequeString("Collide_RFoots");
//						break;
//					}
//				}*/
//				float rate = 1.0f;
//				if(vo->getNumBotPhysicsObject()>0)
//				{
//					crViewMatterObject::BotPhysicsObject *botPhysicsObject = vo->getBotPhysicsObject(o1);
//					if(botPhysicsObject)
//					{
//						rate = botPhysicsObject->m_executionrate;
//						if(rate == -1)
//						{
//							vo->kill(5);
//							//crFilterRenderManager::getInstance()->addDequeString("Head Shot");
//							goto quite;
//						}
//					}
//					else goto quite;
//				}
//				int excution = bulletMatterObject->getExecution(vo->getBound().center());
//				excution *= rate;
//				if(firePerson == vo)
//				{
//					//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator()+++++++"<<std::endl;
//					//vo->enablePhysics(0.05f);
//					vo->addExcution(excution);//不能用collidepos传入
//					if(vo->isDead())
//					{
//						//if(!m_killInfo.valid())
//						//    m_killInfo = crFilterRenderManager::getInstance()->getDrawable("KillInfo");
//						//else
//						//{
//						//	m_killInfo->setText(" killed " + vo->getName());
//						//	m_killInfo->setVisiable(true);
//						//}
//						//crFilterRenderManager::getInstance()->addDequeString("killed " + vo->getName());
//					}
//				}
//				else
//				{
//					//int checkteam = firePerson->checkTeam(vo->getTeam());
//					//if(checkteam<0)
//					{//此处无需check
//						//vo->enablePhysics(0.05f);
//						vo->addExcution(excution,firePerson);
//						if(vo->isDead())
//						{
//							//if(!m_killInfo.valid())
//							//	m_killInfo = crFilterRenderManager::getInstance()->getDrawable("KillInfo");
//							//else
//							//{
//							//	m_killInfo->setText(bulletMatterObject->getFirePerson()->getName() + " killed " + vo->getName());
//							//	m_killInfo->setVisiable(true);
//							//}
//							//crFilterRenderManager::getInstance()->addDequeString(firePerson->getName() + " killed " + vo->getName());
//							firePerson->increaseKilledCount();
//						}
//					}
//				}
//			}
//			goto quite;
//		}
//
//		//crViewMatterObject *viewMatterObject2 = dynamic_cast<crViewMatterObject *>(node2);
//		//if(viewMatterObject2)
//		//{//角色与角色碰撞的事件
//		//}
//
//  //      if(vo->getCollideBody(collidePos) == Foots)
//		//{//用于着路时播放声音
//		//	std::string soundString;
//		//	CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",soundString);
//		//	if(!soundString.empty())
//		//		soundString = vo->getName() + "_fall_" + soundString;
//		//	else
//		//		soundString = vo->getName() + "_fall_stone";
//
//		//	//CRCore::notify(CRCore::FATAL)<<"m_groundDetectedObject soundString = "<<soundString<<std::endl;
//
//		//	CRAL::crSoundState *soundState = CRAL::crSoundManager::instance()->findSoundState(soundString);
//		//	if(soundState && !soundState->isPlaying()) 
//		//	{
//		//		soundState->setPosition(m_voNewPos);
//		//		soundState->setPlay(true);
//		//	}
//		//	goto quite;
//		//}
//	}
//quite:
//	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
//}

//////////////////////////
//
//crAircraftCollideCallback
//
///////////////////////////
//crAircraftCollideCallback *crAircraftCollideCallback::getInstance()
//{
//	static ref_ptr<crAircraftCollideCallback> s_manager = new crAircraftCollideCallback;
//	return s_manager.get();
//}

//void crAircraftCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
//{
//	crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node1);
//	if(vo&&!vo->isDead())
//	{
//		crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(node2);
//		if(bulletMatterObject)
//		{
//			if(bulletMatterObject->validExecution(o2))
//			{
//				//CRCore::notify(CRCore::FATAL)<<" crAircraftCollideCallback::operator()"<<std::endl;
//				crViewMatterObject *firePerson = const_cast<crViewMatterObject *>(bulletMatterObject->getFireWeapon()->getFireBody());
//				float rate = 1.0f;
//				if(vo->getNumBotPhysicsObject()>0)
//				{
//					crViewMatterObject::BotPhysicsObject *botPhysicsObject = vo->getBotPhysicsObject(o1);
//					if(botPhysicsObject)
//					{
//						rate = botPhysicsObject->m_executionrate;
//						if(rate == -1)
//						{
//							vo->kill(5);
//							//crFilterRenderManager::getInstance()->addDequeString("Head Shot");
//							goto quite;
//						}
//					}
//					else goto quite;
//				}
//				int excution = bulletMatterObject->getExecution(vo->getBound().center());
//				excution *= rate;
//				if(firePerson == vo)
//				{
//					//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator()+++++++"<<std::endl;
//					vo->enablePhysics(0.05f);
//					vo->addExcution(excution);//不能用collidepos传入
//					if(vo->isDead())
//					{
//						//if(!m_killInfo.valid())
//						//	m_killInfo = crFilterRenderManager::getInstance()->getDrawable("KillInfo");
//						//else
//						//{
//						//	m_killInfo->setText(" killed " + vo->getName());
//						//	m_killInfo->setVisiable(true);
//						//}
//						//crFilterRenderManager::getInstance()->addDequeString("killed " + vo->getName());
//					}
//				}
//				else
//				{
//					int checkteam = firePerson->checkTeam(vo->getTeam());
//					if(checkteam<0)
//					{
//						vo->enablePhysics(0.05f);
//						vo->addExcution(excution);
//						if(vo->isDead())
//						{
//							//if(!m_killInfo.valid())
//							//	m_killInfo = crFilterRenderManager::getInstance()->getDrawable("KillInfo");
//							//else
//							//{
//							//	m_killInfo->setText(bulletMatterObject->getFirePerson()->getName() + " killed " + vo->getName());
//							//	m_killInfo->setVisiable(true);
//							//}
//							//crFilterRenderManager::getInstance()->addDequeString(bulletMatterObject->getFireWeapon()->getFireBody()->getName() + " killed " + vo->getName());
//							firePerson->increaseKilledCount();
//						}
//					}
//				}
//			}
//			goto quite;
//		}
//		std::string attrStr;
//		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
//		//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator() "<<node2->className()<<" "<<node1<<" "<<node2<<std::endl;
//		if(attrStr.empty()) attrStr = "stone";
//		crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(node2);
//		if(bot)
//		{
//			vo->enablePhysics(0.05f);
//
//			//vo->addExcution(20.0f);//不能用collidepos传入
//			//if(vo->isDead())
//			//{
//			//	crFilterRenderManager::getInstance()->addDequeString("killed " + vo->getName());
//			//}
//		}
//		else
//		{
//			vo->kill(1);
//			vo->setVelocity(crVector3f(0.0f,0.0f,0.0f));
//			//vo->setVisiable(false);
//			//crFilterRenderManager::getInstance()->addDequeString("killed " + vo->getName());
//		}
//
//		std::string effectString = "aircraft_"+attrStr;
//		//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		//if(m_particleEffectNode)
//		//{
//		//	m_particleEffectNode->setEffectPosition(collidePos);
//		//	m_particleEffectNode->startEffect();
//		//}
//		m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		if(m_sequenceNode)
//		{
//			m_sequenceNode->setEffectPosition(collidePos);
//			m_sequenceNode->startEffect();
//		}
//		//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
//		m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(vo->getName(),effectString);
//		if(m_soundState)
//		{
//			m_soundState->setPosition(collidePos);
//			//CRCore::crVector3f listener;
//			//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
//			//CRCore::crVector3f dir = collidePos - listener;
//			//dir.normalize();
//			//m_soundState->setDirection(dir);
//			m_soundState->setPlay(true);
//		}
//
//		//crBodyCaseMatterObject *bodyCase = dynamic_cast<crBodyCaseMatterObject *>(node2);
//		//if(bodyCase)
//		//{
//  //          if(dynamic_cast<crMissileSetOrFlackMatterObject *>(node1))
//		//	{//不处理crBodyCaseMatterObject与crMissileSetOrFlackMatterObject的碰撞
//		//		goto quite;
//		//	}
//		//}
//	}
//quite:
//	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
//}

//////////////////////////
//
//crBodyCaseCollideCallback
//
///////////////////////////
//crBodyCaseCollideCallback *crBodyCaseCollideCallback::getInstance()
//{
//	static ref_ptr<crBodyCaseCollideCallback> s_manager = new crBodyCaseCollideCallback;
//	return s_manager.get();
//}
//
//void crBodyCaseCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos)
//{
//	//crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node1);
//	//if(vo)
//	//{
//		crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(node2);
//		if(bulletMatterObject)
//		{
//			if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos);
//            return;
//		}
//
//		std::string attrStr;
//		CRCore::crArgumentParser::readKeyValue(node2->getDescriptions(),"AttributeString",attrStr);
//		//CRCore::notify(CRCore::FATAL)<<" crViewObjectCollideCallback::operator() "<<node2->className()<<" "<<node1<<" "<<node2<<std::endl;
//		if(attrStr.empty()) attrStr = "stone";
//		std::string effectString = "bodycase_"+attrStr;
//		m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		if(m_particleEffectNode)
//		{
//			m_particleEffectNode->setEffectPosition(collidePos);
//			m_particleEffectNode->startEffect();
//		}
//		m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		if(m_sequenceNode)
//		{
//			m_sequenceNode->setEffectPosition(collidePos);
//			m_sequenceNode->startEffect();
//		}
//		m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
//		if(m_soundState)
//		{
//			m_soundState->setPosition(collidePos);
//			CRCore::crVector3f listener;
//			CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
//			CRCore::crVector3f dir = collidePos - listener;
//			dir.normalize();
//			m_soundState->setDirection(dir);
//			m_soundState->setPlay(true);
//		}
//	//}
//	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos);
//}
//////////////////////////
//
//crGroundBoxCollideCallback
//
///////////////////////////
//crGroundBoxCollideCallback *crGroundBoxCollideCallback::getInstance()
//{
//	static ref_ptr<crGroundBoxCollideCallback> s_manager = new crGroundBoxCollideCallback;
//	return s_manager.get();
//}

void crGroundBoxCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(node2);
	if(vo)
	{
		vo->kill(2);
		//crFilterRenderManager::getInstance()->addDequeString("killed " + vo->getName());
	}
	else node2->setVisiable(false);

    if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
}
//////////////////////////
//
//crMouseManipularCollideCallback
//
///////////////////////////
//crMouseManipularCollideCallback *crMouseManipularCollideCallback::getInstance()
//{
//	static ref_ptr<crMouseManipularCollideCallback> s_manager = new crMouseManipularCollideCallback;
//	return s_manager.get();
//}

void crMouseManipularCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(node1);
	if(body)
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		if(body == cameraBot)
		{
			crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
			if(bodyInfo->getManipulatorMode() == crGameBodyInfo::MM_RPG||bodyInfo->getManipulatorMode() == crGameBodyInfo::MM_RTS)
			{
				std::string groundString;
				bool canpass = false;
				if(crArgumentParser::readKeyValue(node2->getDescriptions(),"GroundString",groundString))
				{
					canpass = groundString.compare("canpass") == 0;
				}
				if(!canpass)
				{//stop go to point
					CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(body->getParent(0));
					crEventGroup *bodyNC = bodyMG->getAttachedEventGroup();
					if(!bodyNC)
					{
						CRCore::notify(CRCore::FATAL)<<"CRPhysics::crMouseManipularCollideCallback(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
						return;
					}
					bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVEND,NULL),MAKEINT64(NULL,NULL));//0:init movetask index
					//CRCore::notify(CRCore::FATAL)<<"CRPhysics::crMouseManipularCollideCallback()"<<std::endl;
				}
			}
		}
	}
	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
}