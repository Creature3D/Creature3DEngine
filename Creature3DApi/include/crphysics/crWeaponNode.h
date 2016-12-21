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
#ifndef CRPHYSICS_CRWEAPONNODE_H
#define CRPHYSICS_CRWEAPONNODE_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crMatterObject.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crVector3.h>
#include <CRCore/crTimer.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRCore/crRange.h>
#include <CRPhysics/crViewMatterObject.h>
//#include <CRParticle/crParticleEffect.h>
#include <CRCore/crSequence.h>
#include <CRCore/crRange.h>
#include <CRCore/crMultiSwitch.h>
#include <CRPhysics/crVolumeNode.h>
#include <string>
namespace CRPhysics{
//////////////////
//crBulletObject
////////////////////
class crBulletMatterObject;
class CRPHYSICS_EXPORT crBulletObject : public CRCore::crGroup
{///弹药模型
friend class crBulletMatterObject;
public:
	enum DamageType
	{
		Direct,
		Explode,
		Extra
	};

	crBulletObject();
	crBulletObject(const crBulletObject &bulletObject,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crBulletObject);

	void setDamageType(DamageType dt);
	DamageType getDamageType() const;

	void setExecutionRang(float min, float max);

	void setExplodeRadius(float r);
	float getExplodeRadius() const;

	void setK(float k);
	inline float getK() const{ return m_k; }

	void setImpetus(float f);
	float getImpetus() const{ return m_impetus; }

	void setForce(const CRCore::crVector3 &force);
	const CRCore::crVector3 &getForce() const{ return m_force; }

	void setLinearVel(float linearVel);
	inline float getLinearVel() const{ return m_linearVel; }

	void setDuration(float duration);
	inline float getDuration() const{ return m_duration; }

	enum AttrMask
	{
		NoneAttr = 0x0,
		GravityEnable = 0x1,
		VanishWhenCollide = 0x2,
        Track = 0x4//用于跟踪导弹，可跟踪目标
	};
    void setAttrMask(AttrMask attrmask);
	inline AttrMask getAttrMask()const { return m_attrMask; }

	enum EffectAttrMask
	{
		NoneEffectAttr = 0x0,
		Fly_Particle     = 0x001,
		Fly_Sequence     = 0x002,
		Fly_Sound        = 0x004,
		Collide_Particle = 0x008,
		Collide_Sequence = 0x010,
		Collide_Sound    = 0x020,
		Explode_Particle = 0x040,
		Explode_Sequence = 0x080,
		Explode_Sound    = 0x100,

		Collide_Particle2     = 0x200,
		Collide_Particle3     = 0x400,
	};
	void setEffectAttrMask(EffectAttrMask attrmask);
	inline EffectAttrMask getEffectAttrMask()const { return m_effectAttrMask; }

	enum ExecutionPhysicsType
	{
		Type_Collide,//不需要辅助模型，直接根据子弹飞行碰撞判断伤害
		Type_Explode,//需要辅助物理体，爆炸模型
		Type_TimeExplode,//需要辅助物理体，爆炸模型
		Type_Sphere,//不考虑子弹飞行碰撞，在目标点建立一个碰撞球体
		Type_Ray,//不考虑子弹飞行碰撞，在目标点建立一条射线
		Type_Weapon,//直接用武器攻击，比如刀、锯等,BOX
		Type_Shield_Box,//防护物，比如盾牌、防护罩等,BOX
		Type_Shield_Sphere,//防护物，比如盾牌、防护罩等,Sphere
		Type_Volume,
		Type_ExplodeVolume//爆炸形成能量场，模拟冲击波等效果使用crVolumeNode来模拟
		//Type_Box
	};

    void setExecutionPhysicsType(ExecutionPhysicsType type);
	ExecutionPhysicsType getExecutionPhysicsType() const;
    void setExecutionPhysicsLength(float length);
	float getExecutionPhysicsLength() const;

	void setModelPhysicsRadius(float r);
	float getModelPhysicsRadius() const;

	void setMissilePrecision(const CRCore::rangef& missilePrecision);
	inline const CRCore::rangef& getMissilePrecision() const{ return m_missilePrecision; }

	void setMissileFlexibility(const float missileFlex);
	inline float getMissileFlexibility() const{ return m_missileFlexibility; }

	void setParticleFlyScale(const float particleFlyScale);
	inline float getParticleFlyScale() const{ return m_particleFlyScale; }

	void setParticleFlyOffset(const float particleFlyOffset);
	inline float getParticleFlyOffset() const{ return m_particleFlyOffset; }

	void setVolumeName(const std::string &volumeName);
	inline const std::string& getVolumeName() const{ return m_volumeName; }

	void setVolumeLength(const CRCore::crVector3 &volumeLength);
	inline const CRCore::crVector3& getVolumeLength() const{ return m_volumeLength; }
	void setMaxTriggerCount(int triggerCount);
	inline int getMaxTriggerCount()const { return m_maxTriggerCount; }
	void setReTriggerDelay(float delay);
	inline float getReTriggerDelay()const { return m_reTriggerDelay; }
protected:
	virtual ~crBulletObject();
	int getExecution(float distance) const;

	DamageType m_damageType;
	CRCore::rangef m_executionRange;//伤害数值范围
	float m_explodeRadius;//爆炸半径
    float m_linearVel;//子弹飞行的起始速率
	float m_impetus;//子弹的推动力
	float m_k;//子弹的空气阻力系数 空气阻力 f = kv
	CRCore::crVector3 m_force;//额外的力
	float m_duration;//子弹持续时间
	CRCore::rangef m_missilePrecision;//跟踪导弹的命中精度
	float m_missileFlexibility;//跟踪导弹的机动性
	float m_particleFlyScale;//子弹粒子速度参数
	float m_particleFlyOffset;//子弹飞行粒子后置距离

	ExecutionPhysicsType m_executionPhysicsType;
	float       m_executionPhysicsLength;
	float       m_modelPhysicsRadius;
	CRCore::crVector3   m_volumeLength;
	int         m_maxTriggerCount;
	float       m_reTriggerDelay;

	AttrMask m_attrMask;
	EffectAttrMask m_effectAttrMask;
	std::string m_volumeName;
};

//////////////////
//crWeaponObject
////////////////////
class CRPHYSICS_EXPORT crWeaponObject : public CRCore::crGroup
{///武器模型
public :
	crWeaponObject();
	crWeaponObject(const crWeaponObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crWeaponObject);
    
	void setMaxBulletsInOneClip(int num);
	int getMaxBulletsInOneClip() const;

	void setInterval(float interval);
	float getInterval() const;

	void setReplaceClipTime(float time);
	float getReplaceClipTime() const;

	enum EffectAttrMask
	{
		NoneEffectAttr = 0x0,
		Fire_Particle     = 0x001,
		Fire_Sequence     = 0x002,
		Fire_Sound        = 0x004,
		Clip_Particle = 0x008,
		Clip_Sequence = 0x010,
		Clip_Sound    = 0x020,
		Collide_Particle = 0x040,
		Collide_Sequence = 0x080,
		Collide_Sound    = 0x100,
	};
	void setEffectAttrMask(EffectAttrMask attrmask);
	inline EffectAttrMask getEffectAttrMask()const { return m_effectAttrMask; }

	enum AttackMask
	{
		AttackAll = 0xFFFF,
		AttackGround = 0x01,
		AttackAir = 0x02,
		AttackDeepSea = 0x04
	};
	void setAttackMask(unsigned int mask);
	inline unsigned int getAttackMask()const { return m_attackMask; }

	void setAttackDistance(float attackDistance);
	inline float getAttackDistance() const{ return m_attackDistance; }

	void setGunPoint(const CRCore::crVector3& gunpoint);
	inline const CRCore::crVector3& getGunPoint() const{ return m_gunPoint; }

	void setClipPos(const CRCore::crVector3& clippos);
	inline const CRCore::crVector3&  getClipPos() const{ return m_clipPos; }
protected:
	virtual ~crWeaponObject();
	EffectAttrMask m_effectAttrMask;
	int m_maxBulletsInOneClip;//弹夹里最多能容纳的子弹数量
	float m_interval;//使用该武器所需的时间间隔
	float m_replaceClipTime;
	
	float m_attackDistance;//武器的有效攻击距离
	unsigned int m_attackMask;

	CRCore::crVector3 m_gunPoint;
	CRCore::crVector3 m_clipPos;
};

//////////////////
//crBulletMatterObject
////////////////////
class crWeaponMatterObject;
class CRPHYSICS_EXPORT crBulletMatterObject : public crMatterObject
{
public:
	crBulletMatterObject();
	crBulletMatterObject(const crBulletMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crBulletMatterObject);
	
	const crBulletObject *getBulletObject()const;
    
	void fire( crWeaponMatterObject *fireWeapon, int weaponMatixID, crMatterGroup *firePerson, const CRCore::crVector3 &gunpoint, const CRCore::crVector3 &target, double startTime );//攻击

	virtual void traverse(CRCore::crNodeVisitor& nv);
	virtual void updateCreMatrix();

	void setFlying(bool flying);
    bool getFlying();
	bool isBulletMatterObjectIdle();
	void bulletGeomCollide();

	bool getCollided();
	void setCollided(bool collided);

	//crMatterGroup *getFirePerson();
	crWeaponMatterObject *getFireWeapon();
	float getExecutionRate(const CRCore::crVector3 &collidePos) const;
	bool isFriendFire(crViewMatterObject *collideBot);

	virtual void setEnableIntersect( bool enableIntersect);
	virtual void releaseOde();
    
	typedef std::set< void * > ExplodedSet;
	void explode(const CRCore::crVector3 &collidePos);
    void explodeEnd();
	bool isExploding();

	virtual void releaseObjects(CRCore::crState* state);
	void setTrackTarget(CRCore::crNode *trackTarget);
	inline CRCore::crNode *getTrackTarget() { return m_trackTarget.get(); }

	bool validExecution(void *hitbody,void *o);

	const CRCore::crVector3& getFireDirect() const{ return m_fireDirect; }
	const CRCore::crVector3& getFireTarget() const{ return m_fireTarget; }
	const CRCore::crVector3& getGunpoint() const { return m_gunpoint; }
protected:
	virtual ~crBulletMatterObject();

	virtual void _create(dWorldID worldID, dSpaceID spaceID);
	bool validExplode(void *hitbody);

	mutable crWeaponMatterObject *m_fireWeapon;
	//mutable CRCore::ref_ptr<crMatterGroup> m_firePerson;//crAIGroup
	//mutable CRCore::ref_ptr<crWeaponMatterObject> m_fireWeapon;//crAIGroup
	CRCore::ref_ptr<CRCore::crNode> m_trackTarget;//跟踪目标
	int m_weaponMatrixID;//WeaponMatrixVec id

	CRCore::crVector3 m_gunpoint;
	bool m_flying;
	bool m_collided;
	int m_explodeFlag;
	double m_fireStartTime;
	//float m_flySoundTime;

	mutable dGeomID   m_bulletGeomID;//辅助物理体

	CRCore::crVector3 m_fireDirect;
    CRCore::crVector3 m_fireTarget;//用于计算爆炸伤害，该点为爆炸的球心
	//CRParticle::crParticleEffect* m_particleEffectNode;
	//CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;
	CRCore::ref_ptr<crVolumeNode> m_volumeNode;
	ExplodedSet m_explodedSet;
	float m_explodeRadius;
	//CRCore::ref_ptr<CRCore::crMultiSwitch> m_cautionSW;
	//CRCore::ref_ptr<CRCore::crNode> m_miss;
	//CRCore::ref_ptr<CRCore::crDrawable> m_
};

//////////////////
//crWeaponMatterObject
////////////////////
class CRPHYSICS_EXPORT crWeaponMatterObject : public crMatterObject
{
public:
	crWeaponMatterObject();

	crWeaponMatterObject(const crWeaponMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crWeaponMatterObject);

	virtual void traverse(CRCore::crNodeVisitor& nv);

	//inline void setGunPoint(const CRCore::crVector3& gunpoint) { m_gunpoint = gunpoint; }
	virtual void fire( crMatterGroup *firePerson, const CRCore::crVector3& target, double time );//攻击
	virtual void fire( crMatterGroup *firePerson, crGameBodyInfo::LockedTargetSet &targets, double time );//攻击
	virtual void fire( crMatterGroup *firePerson, CRCore::crNode *target, double time );//攻击
	virtual void fire_noTarget( crMatterGroup *firePerson, const CRCore::crVector3& offset, double time );//攻击

	virtual void especialFire( crMatterGroup *firePerson, const CRCore::crVector3& target, double time );//武器特殊功能
	virtual void replaceClip(crMatterGroup *firePerson, double time);//更换弹夹

	void setBulletCount(int bulletCount);
	inline void addBulletCount(int bulletCount) { m_bulletCount += bulletCount; }
	inline int getBulletCount()const{ return m_bulletCount; }

	void setBulletsInWeapon(int bulletsInWeapon);
	inline int getBulletsInWeapon()const{ return m_bulletsInWeapon; }
    
	bool isBulletEmpty() const;

	void setComeForthInterval(float interval);
	float getComeForthInterval()const;

	const crWeaponObject *getWeaponObject()const;
	const crBulletObject *getBulletObject()const;

	enum ReadyToFire
	{
        NotReady,
		NeedReplaceClip,
		Ready
	};

	enum AttackType
	{
        AT_NEAR,//近距离攻击武器
		AT_FAR//远距离攻击武器
	};
	void setAttackType(AttackType mm);
	inline AttackType getAttackType(){ return m_attype; }

	virtual void releaseObjects(CRCore::crState* state);
    virtual void releaseOde();

	//void setWeaponRackName(const std::string &rackName);
	//inline const std::string& getWeaponRackName()const{ return m_weaponRackName;}

	////typedef std::pair<CRCore::crMatrix,CRCore::crMatrix> WeaponMatrixPair;
	//typedef std::vector<CRCore::crMatrix/*WeaponMatrixPair*/> WeaponMatrixVec;
	//inline void clearWeaponMatrix(){ m_weaponMatrixVec.resize(0); }
	//inline void addWeaponMatrix(const CRCore::crMatrix &matrix/*,const CRCore::crMatrix &futureMatrix*/) 
	//{
	//	m_weaponMatrixVec.push_back(matrix/*WeaponMatrixPair(matrix,futureMatrix)*/);
	//	setMatrix(matrix);
	//}
	//inline WeaponMatrixVec &getWeaponMatrixVec() { return m_weaponMatrixVec; }

	inline crViewMatterObject *getFireBody() { return m_fireBody.get(); }

	typedef std::vector< CRCore::ref_ptr<crBulletMatterObject> > BulletMatterObjectVec;
	bool setBullet(crBulletMatterObject *bulletMatterObject);
	const crBulletMatterObject *getBullect() const;
	CRCore::crVector3 getGunPoint();
protected:
	virtual ~crWeaponMatterObject();

	virtual void _create(dWorldID worldID, dSpaceID spaceID);
	void _fire(crBulletMatterObject *bulletMatterObject, crMatterGroup *firePerson, const CRCore::crVector3& gunpoint, const CRCore::crVector3& target, int id);
	virtual ReadyToFire isReadytoFire(double currentTime);

	crBulletMatterObject *getOrCreateIdleBullet();

	int m_bulletCount;//弹药数量
	int m_bulletsInWeapon;//装在武器里的弹药数量 = -1 表示无限弹药
    double m_comeForthInterval;
	double m_lastComeFortTime;

	double m_lastFireTime;
	double m_lastReplaceClipTime;

	//std::string m_weaponRackName;
    //WeaponMatrixVec m_weaponMatrixVec;

	AttackType m_attype;

	CRCore::ref_ptr<crViewMatterObject> m_fireBody;

	BulletMatterObjectVec m_bulletVec;
private:
	int m_state;
};

//////////////////
//crCaissonObject
////////////////////
class CRPHYSICS_EXPORT crCaissonObject : public CRCore::crGroup
{///武器模型
public :
	crCaissonObject();
	crCaissonObject(const crCaissonObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCaissonObject);
    
	void setWeaponName(const std::string &weaponName);
	const std::string &getWeaponName() const;

	//void setCollideSoundName(const std::string &soundName);
	//const std::string &getCollideSoundName() const;

	enum EffectAttrMask
	{
		NoneEffectAttr = 0x0,
		Collide_Particle = 0x010,
		Collide_Sequence = 0x020,
		Collide_Sound    = 0x040,
	};
	void setEffectAttrMask(EffectAttrMask attrmask);
	inline EffectAttrMask getEffectAttrMask()const { return m_effectAttrMask; }
protected:
	virtual ~crCaissonObject();
	EffectAttrMask m_effectAttrMask;
	std::string m_weaponName;
	//std::string m_collideSounName;
};
//////////////////
//crCaissonMatterObject
////////////////////
class CRPHYSICS_EXPORT crCaissonMatterObject : public crMatterObject
{///武器模型
public :
	crCaissonMatterObject();
	crCaissonMatterObject(const crCaissonMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCaissonMatterObject);

	virtual void traverse(CRCore::crNodeVisitor& nv);
	
	const crCaissonObject *getCaissonObject()const;

	void setBulletCount(int bulletCount);
	int getBulletCount()const;
	void setComeForthInterval(float interval);
	float getComeForthInterval()const;

protected:
	virtual ~crCaissonMatterObject();
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

	int m_bulletCount;//弹药数量

	float m_comeForthInterval;
	double m_lastComeFortTime;
private:
	int m_state;
};

//////////////////
//crHpObject
////////////////////
class CRPHYSICS_EXPORT crHpObject : public CRCore::crGroup
{///武器模型
public :
	crHpObject();
	crHpObject(const crHpObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crHpObject);

	void setHpValue(int value);
	int getHpValue() const;

	//void setCollideSoundName(const std::string &soundName);
	//const std::string &getCollideSoundName() const;

	enum EffectAttrMask
	{
		NoneEffectAttr = 0x0,
		Collide_Particle = 0x010,
		Collide_Sequence = 0x020,
		Collide_Sound    = 0x040,
	};
	void setEffectAttrMask(EffectAttrMask attrmask);
	inline EffectAttrMask getEffectAttrMask()const { return m_effectAttrMask; }
protected:
	virtual ~crHpObject();
	int m_hpValue;
	EffectAttrMask m_effectAttrMask;
	//std::string m_collideSounName;
};
//////////////////
//crHpMatterObject
////////////////////
class CRPHYSICS_EXPORT crHpMatterObject : public crMatterObject
{///武器模型
public :
	crHpMatterObject();
	crHpMatterObject(const crHpMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crHpMatterObject);

	virtual void traverse(CRCore::crNodeVisitor& nv);

	const crHpObject *getHpObject()const;

	void setComeForthInterval(float interval);
	float getComeForthInterval()const;

protected:
	virtual ~crHpMatterObject();
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

	float m_comeForthInterval;
	float m_lastComeFortTime;
private:
	int m_state;
};

//////////////////
//crArmorObject
////////////////////
class CRPHYSICS_EXPORT crArmorObject : public CRCore::crGroup
{///武器模型
public :
	crArmorObject();
	crArmorObject(const crArmorObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crArmorObject);

	void setArmorValue(int value);
	int getArmorValue() const;

	//void setCollideSoundName(const std::string &soundName);
	//const std::string &getCollideSoundName() const;
	enum EffectAttrMask
	{
		NoneEffectAttr = 0x0,
		Collide_Particle = 0x010,
		Collide_Sequence = 0x020,
		Collide_Sound    = 0x040,
	};
	void setEffectAttrMask(EffectAttrMask attrmask);
	inline EffectAttrMask getEffectAttrMask()const { return m_effectAttrMask; }
protected:
	virtual ~crArmorObject();
	EffectAttrMask m_effectAttrMask;
	int m_armorValue;
	//std::string m_collideSounName;
};
//////////////////
//crArmorMatterObject
////////////////////
class CRPHYSICS_EXPORT crArmorMatterObject : public crMatterObject
{///武器模型
public :
	crArmorMatterObject();
	crArmorMatterObject(const crArmorMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crArmorMatterObject);

	virtual void traverse(CRCore::crNodeVisitor& nv);

	const crArmorObject *getArmorObject()const;

	void setComeForthInterval(float interval);
	float getComeForthInterval()const;

protected:
	virtual ~crArmorMatterObject();
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

	float m_comeForthInterval;
	double m_lastComeFortTime;
private:
	int m_state;
};

}

#endif