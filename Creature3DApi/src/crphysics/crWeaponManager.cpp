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
#include <CRPhysics\crWeaponManager.h>
#include <CRPhysics\crViewMatterObject.h>
#include <CRCore\crIntersectVisitor.h>
#include <CRCore/crBrain.h>
#include <CRCore\crNotify.h>
using namespace CRCore;
using namespace CRPhysics;

ref_ptr<crWeaponManager> crWeaponManager::m_instance = NULL;

crWeaponManager::crWeaponManager()
{
}
crWeaponManager::~crWeaponManager()
{
	//for( WeaponModelVec::iterator itr = m_weapons.begin();
	//	 itr != m_weapons.end();
	//	 ++itr )
	//{
	//	(*itr)->releaseObjects();
	//}
	//for( BulletModelVec::iterator itr = m_bullets.begin();
	//	itr != m_bullets.end();
	//	++itr )
	//{
	//	(*itr)->releaseObjects();
	//}
	//for( CaissonModelVec::iterator itr = m_caissons.begin();
	//	itr != m_caissons.end();
	//	++itr )
	//{
	//	(*itr)->releaseObjects();
	//}
	//for( HpModelVec::iterator itr = m_hps.begin();
	//	itr != m_hps.end();
	//	++itr )
	//{
	//	(*itr)->releaseObjects();
	//}
	//for( ArmorModelVec::iterator itr = m_armors.begin();
	//	itr != m_armors.end();
	//	++itr )
	//{
	//	(*itr)->releaseObjects();
	//}
	//for( BulletMatterObjectMap::iterator itr = m_bulletMatterObjectMap.begin();
	//	itr != m_bulletMatterObjectMap.end();
	//	++itr )
	//{
	//	itr->second->releaseObjects();
	//}

	//CRCore::notify(CRCore::ALWAYS)<<"~crWeaponManager()"<< std::endl;
}

crWeaponManager* crWeaponManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crWeaponManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crWeaponManager::clear()
{
	m_instance = NULL;
}
void crWeaponManager::releaseObjects(crState *state)
{
	for (WeaponModelVec::iterator itr = m_weapons.begin();
		 itr != m_weapons.end();
		++itr)
	{
		(*itr)->releaseObjects(state);
	}
	for (BulletModelVec::iterator itr = m_bullets.begin();
		itr != m_bullets.end();
		++itr)
	{
		(*itr)->releaseObjects(state);
	}
	for (CaissonModelVec::iterator itr = m_caissons.begin();
		itr != m_caissons.end();
		++itr)
	{
		(*itr)->releaseObjects(state);
	}
	for (HpModelVec::iterator itr = m_hps.begin();
		itr != m_hps.end();
		++itr)
	{
		(*itr)->releaseObjects(state);
	}
	for (ArmorModelVec::iterator itr = m_armors.begin();
		itr != m_armors.end();
		++itr)
	{
		(*itr)->releaseObjects(state);
	}
	for (BulletMatterObjectMap::iterator itr = m_bulletMatterObjectMap.begin();
		itr != m_bulletMatterObjectMap.end();
		++itr)
	{
		itr->second->releaseObjects(state);
	}
	for (WeaponMatterObjectMap::iterator itr = m_weaponMatterObjectMap.begin();
		itr != m_weaponMatterObjectMap.end();
		++itr)
	{
		itr->second->releaseObjects(state);
	}
}
int crWeaponManager::addWeapon(crWeaponObject *weapon)
{
	bool find = false;
	int index = 0;
	for( WeaponModelVec::iterator itr = m_weapons.begin();
		itr != m_weapons.end();
		++itr,++index )
	{
		if(*itr == weapon) 
		{
			find = true;
			break;
		}
	}

	if(!find) m_weapons.push_back(weapon);
	return index;
}

int crWeaponManager::addBullet(crBulletObject *bullet)
{
	bool find = false;
	int index = 0;
	for( BulletModelVec::iterator itr = m_bullets.begin();
		itr != m_bullets.end();
		++itr,++index )
	{
		if(*itr == bullet) 
		{
			find = true;
			break;
		}
	}

	if(!find) m_bullets.push_back(bullet);
	return index;
}

int crWeaponManager::addCaisson(crCaissonObject *caisson)
{
	bool find = false;
	int index = 0;
	for( CaissonModelVec::iterator itr = m_caissons.begin();
		itr != m_caissons.end();
		++itr,++index )
	{
		if(*itr == caisson) 
		{
			find = true;
			break;
		}
	}

	if(!find) m_caissons.push_back(caisson);
	return index;
}

int crWeaponManager::addHp(crHpObject *hp)
{
	bool find = false;
	int index = 0;
	for( HpModelVec::iterator itr = m_hps.begin();
		itr != m_hps.end();
		++itr,++index )
	{
		if(*itr == hp) 
		{
			find = true;
			break;
		}
	}

	if(!find) m_hps.push_back(hp);
	return index;
}

int crWeaponManager::addArmor(crArmorObject *armor)
{
	bool find = false;
	int index = 0;
	for( ArmorModelVec::iterator itr = m_armors.begin();
		itr != m_armors.end();
		++itr,++index )
	{
		if(*itr == armor) 
		{
			find = true;
			break;
		}
	}

	if(!find) m_armors.push_back(armor);
	return index;
}

crWeaponObject *crWeaponManager::getWeapon(int index)
{
    if(index>=0 && index < m_weapons.size())
		return m_weapons[index].get();
	return NULL;
}

crBulletObject *crWeaponManager::getBullet(int index)
{
	if(index>=0 && index < m_bullets.size())
		return m_bullets[index].get();
	return NULL;
}

crCaissonObject *crWeaponManager::getCaisson(int index)
{
	if(index>=0 && index < m_bullets.size())
		return m_caissons[index].get();
	return NULL;
}

crHpObject *crWeaponManager::getHp(int index)
{
	if(index>=0 && index < m_hps.size())
		return m_hps[index].get();
	return NULL;
}

crArmorObject *crWeaponManager::getArmor(int index)
{
	if(index>=0 && index < m_armors.size())
		return m_armors[index].get();
	return NULL;
}

crBulletMatterObject *crWeaponManager::getOrCreateIdleBulletMatterObject(int bulletObjectIndex)
{
	//CRCore::notify(CRCore::FATAL)<<"crWeaponManager::getOrCreateIdleBulletMatterObject bulletObjectIndex = "<<bulletObjectIndex<<std::endl;
	crBulletMatterObject *defaultBulletMatterObject;
	BulletMatterObjectMap::iterator itr = m_bulletMatterObjectMap.find(bulletObjectIndex);
	defaultBulletMatterObject = itr->second.get();
	for( ; itr != m_bulletMatterObjectMap.end() && itr->first == bulletObjectIndex; ++itr )
	{
        if(itr->second->isBulletMatterObjectIdle())
		{
			//CRCore::notify(CRCore::FATAL)<<"crWeaponManager::getOrCreateIdleBulletMatterObject"<<std::endl;
			return itr->second.get();
		}
	}

	//crBulletMatterObject *bulletMatterObject = new CRPhysics::crBulletMatterObject;
	//bulletMatterObject->setCollideCallback(defaultBulletMatterObject->getCollideCallback());
	//bulletMatterObject->addChild(defaultBulletMatterObject->getChild(0));
	//bulletMatterObject->setMass(defaultBulletMatterObject->getMass());
	//bulletMatterObject->setDescriptionsInString(bulletMatterObject->getBulletObject()->getDescriptionsInString());
	//bulletMatterObject->setName(bulletMatterObject->getBulletObject()->getName());

	crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(defaultBulletMatterObject->clone(crCopyOp::SHALLOW_COPY));
	bulletMatterObject->setVisiable(false);
	defaultBulletMatterObject->getParent(0)->addChild(bulletMatterObject);
	m_bulletMatterObjectMap.insert(std::make_pair(bulletObjectIndex,bulletMatterObject));
	//CRCore::crIntersectVisitor::insertStaticAvoidIntersectNode(bulletMatterObject);
    
	//CRCore::notify(CRCore::FATAL)<<"crWeaponManager::getOrCreateIdleBulletMatterObject ----size = "<<m_bulletMatterObjectMap.size()<<std::endl;

	return bulletMatterObject;
}

void crWeaponManager::addBulletMatterObject( int index, crBulletMatterObject *bulletMatterObject )
{
	m_bulletMatterObjectMap.insert(std::make_pair(index,bulletMatterObject));
	//CRCore::crIntersectVisitor::insertStaticAvoidIntersectNode(bulletMatterObject);
}

crWeaponManager::BulletMatterObjectMap& crWeaponManager::getBulletMatterObjectMap()
{
	return m_bulletMatterObjectMap;
}

void crWeaponManager::insertWeaponMatterObject(crWeaponMatterObject *weapon)
{
	if(!weapon) return;
	m_weaponMatterObjectMap[weapon->getName()] = weapon;
}

crWeaponMatterObject *crWeaponManager::getWeaponMatterObject(const std::string &weaponName)
{
	if(m_weaponMatterObjectMap.empty()) return NULL;
	WeaponMatterObjectMap::iterator itr = m_weaponMatterObjectMap.find(weaponName);
	return itr == m_weaponMatterObjectMap.end()?m_weaponMatterObjectMap.begin()->second.get():itr->second.get();
}

crWeaponMatterObject *crWeaponManager::createWeaponMatterObject(crWeaponMatterObject *weapon)
{
	//crWeaponMatterObject *newWeapon = new crWeaponMatterObject;
	//newWeapon->setName(weapon->getName());
	//newWeapon->setBulletCount(weapon->getBulletCount());
	//newWeapon->setBulletsInWeapon(weapon->getBulletsInWeapon());
	//newWeapon->addChild(weapon->getChild(0));
	//newWeapon->setMass(weapon->getMass());
	//newWeapon->setBulletObjectIndex(weapon->getBulletObjectIndex());
 //   newWeapon->setDescriptionsInString(weapon->getDescriptionsInString());
	//newWeapon->setWeaponRackName(weapon->getWeaponRackName());

	//for( crMatterObject::EventMap::iterator itr = weapon->getEventMap().begin();
	//	itr != weapon->getEventMap().end();
	//	++itr )
	//{
	//	newWeapon->insertEvent(itr->first,itr->second->clone());
	//}
	return dynamic_cast<crWeaponMatterObject *>(weapon->clone(crCopyOp::SHALLOW_COPY));
}