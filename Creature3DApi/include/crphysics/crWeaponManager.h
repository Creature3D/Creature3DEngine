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
#ifndef CRPHYSICS_CRWEAPONMANAGER_H
#define CRPHYSICS_CRWEAPONMANAGER_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crNode.h>
#include <CRPhysics/crWeaponNode.h>
#include <vector>
namespace CRPhysics{

class CRPHYSICS_EXPORT crWeaponManager : public CRCore::Referenced
{
public :
  crWeaponManager();
  static crWeaponManager *getInstance();

  //crWeaponManager(const crWeaponManager&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY){}

//  META_Node(CRPhysics, crWeaponManager);
  
  typedef std::vector< CRCore::ref_ptr<crWeaponObject> > WeaponModelVec;
  typedef std::vector< CRCore::ref_ptr<crBulletObject> > BulletModelVec;
  typedef std::vector< CRCore::ref_ptr<crCaissonObject> > CaissonModelVec;
  typedef std::vector< CRCore::ref_ptr<crHpObject> > HpModelVec;
  typedef std::vector< CRCore::ref_ptr<crArmorObject> > ArmorModelVec;

  typedef std::multimap< int, CRCore::ref_ptr<crBulletMatterObject> > BulletMatterObjectMap;
  typedef std::map< std::string, CRCore::ref_ptr<crWeaponMatterObject> > WeaponMatterObjectMap;

  int addWeapon(crWeaponObject *weapon);
  int addBullet(crBulletObject *bullet);
  int addCaisson(crCaissonObject *caisson);
  int addHp(crHpObject *hp);
  int addArmor(crArmorObject *armor);

  crWeaponObject *getWeapon(int index);
  crBulletObject *getBullet(int index);
  crCaissonObject *getCaisson(int index);
  crHpObject *getHp(int index);
  crArmorObject *getArmor(int index);

  crBulletMatterObject *getOrCreateIdleBulletMatterObject(int bulletObjectIndex);
  void addBulletMatterObject( int index, crBulletMatterObject *bulletMatterObject );
  BulletMatterObjectMap& getBulletMatterObjectMap();

  void insertWeaponMatterObject(crWeaponMatterObject *weapon);
  crWeaponMatterObject *getWeaponMatterObject(const std::string &weaponName);
  static crWeaponMatterObject *createWeaponMatterObject(crWeaponMatterObject *weapon);

  virtual void clear();
protected:
  virtual ~crWeaponManager(); 
  static CRCore::ref_ptr<crWeaponManager> m_instance;
  WeaponModelVec m_weapons;
  BulletModelVec m_bullets;
  CaissonModelVec m_caissons;
  HpModelVec m_hps;
  ArmorModelVec m_armors;

  BulletMatterObjectMap m_bulletMatterObjectMap;
  WeaponMatterObjectMap m_weaponMatterObjectMap;
};

}

#endif