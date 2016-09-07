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
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore\crIntersectVisitor.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crMath.h>
#include <CRCore\crEffectSequenceManager.h>
#include <CRCore\crNotify.h>
#include <CRPhysics\crWeaponManager.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crSoundManager.h>
using namespace CRCore;
using namespace CRPhysics;
using namespace CRParticle;

//////////////////////////////////
//
//crBulletObject
//
////////////////////////////////
crBulletObject::crBulletObject():
    m_damageType(Direct),
    m_explodeRadius(0.0f),
	m_linearVel(0.0f),
	m_impetus(0.0f),
	m_k(0.0f),
	m_duration(1.0),
	m_attrMask(NoneAttr),
	m_executionPhysicsType(Type_Collide),
    m_executionPhysicsLength(0.1),
	m_modelPhysicsRadius(0.1),
	m_effectAttrMask(NoneEffectAttr),
	m_missileFlexibility(1.0f),
	m_particleFlyScale(0.0f),
	m_particleFlyOffset(0.0f),
	m_maxTriggerCount(0),
	m_reTriggerDelay(0.0f)
{
}

crBulletObject::crBulletObject(const crBulletObject &bulletObject,const CRCore::crCopyOp& copyop):
	CRCore::crGroup(bulletObject,copyop),    
	m_damageType(bulletObject.m_damageType),
	m_executionRange(bulletObject.m_executionRange),
	m_explodeRadius(bulletObject.m_explodeRadius),
    m_linearVel(bulletObject.m_linearVel),
	m_impetus(bulletObject.m_impetus),
	m_k(bulletObject.m_k),
	m_force(bulletObject.m_force),
	m_duration(bulletObject.m_duration),
	m_attrMask(bulletObject.m_attrMask),
	m_effectAttrMask(bulletObject.m_effectAttrMask),
	//m_collideSounName(bulletObject.m_collideSounName),
	//m_flyEffectTypeString(bulletObject.m_flyEffectTypeString),
	//m_collideEffectTypeString(bulletObject.m_collideEffectTypeString),
	m_executionPhysicsType(bulletObject.m_executionPhysicsType),
	m_executionPhysicsLength(bulletObject.m_executionPhysicsLength),
	m_modelPhysicsRadius(bulletObject.m_modelPhysicsRadius),
	m_missileFlexibility(bulletObject.m_missileFlexibility),
	m_particleFlyScale(bulletObject.m_particleFlyScale),
	m_particleFlyOffset(bulletObject.m_particleFlyOffset),
	m_maxTriggerCount(bulletObject.m_maxTriggerCount),
	m_reTriggerDelay(bulletObject.m_reTriggerDelay)
{

}

crBulletObject::~crBulletObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crBulletObject()"<< std::endl;
}

void crBulletObject::setDamageType(DamageType dt)
{
	m_damageType = dt;
}

crBulletObject::DamageType crBulletObject::getDamageType() const
{
    return m_damageType;
}

void crBulletObject::setExecutionRang(float min, float max)
{
   m_executionRange.set(min,max);
}
int crBulletObject::getExecution(float distance)const
{
	float executionRange = m_executionRange.get_random();
	if(m_damageType == Explode)
	{
		if(m_explodeRadius>distance)
		{
			float rate = (m_explodeRadius - distance) / m_explodeRadius;
			executionRange = executionRange * rate;
		}
		else executionRange = 0.0f;
	}
	return executionRange;
}

void crBulletObject::setExplodeRadius(float r)
{
    m_explodeRadius = r;
}

float crBulletObject::getExplodeRadius() const
{
    return m_explodeRadius;
}

void crBulletObject::setK(float k)
{
	m_k = k;
}

void crBulletObject::setImpetus(float f)
{
	m_impetus = f;
}

void crBulletObject::setForce(const CRCore::crVector3 &f)
{
	m_force = f;
}

void crBulletObject::setLinearVel(float linearVel)
{
	m_linearVel = linearVel;
}

void crBulletObject::setDuration(float duration)
{
    m_duration = duration;
}

void crBulletObject::setMissilePrecision(const CRCore::rangef& missilePrecision)
{
	m_missilePrecision = missilePrecision;
}

void crBulletObject::setMissileFlexibility(const float missileFlex)
{
    m_missileFlexibility = missileFlex;
}

void crBulletObject::setParticleFlyScale(const float particleFlyScale)
{
    m_particleFlyScale = particleFlyScale;
}

void crBulletObject::setParticleFlyOffset(const float particleFlyOffset)
{
	m_particleFlyOffset = particleFlyOffset;
}

//void crBulletObject::setCollideSoundName(const std::string &soundName)
//{
//	m_collideSounName = soundName;
//}
//
//const std::string &crBulletObject::getCollideSoundName() const
//{
//	return m_collideSounName;
//}

void crBulletObject::setAttrMask(AttrMask attrmask)
{
	m_attrMask = attrmask;
}

void crBulletObject::setEffectAttrMask(EffectAttrMask attrmask)
{
	m_effectAttrMask = attrmask;
}

void crBulletObject::setExecutionPhysicsType(ExecutionPhysicsType type)
{
    m_executionPhysicsType = type;
}

crBulletObject::ExecutionPhysicsType crBulletObject::getExecutionPhysicsType() const
{
    return m_executionPhysicsType;
}

void crBulletObject::setExecutionPhysicsLength(float length)
{
    m_executionPhysicsLength = length;
}

float crBulletObject::getExecutionPhysicsLength() const
{
    return m_executionPhysicsLength;
}

void crBulletObject::setModelPhysicsRadius(float r)
{
    m_modelPhysicsRadius = r;
}

float crBulletObject::getModelPhysicsRadius() const
{
    return m_modelPhysicsRadius;
}

void crBulletObject::setVolumeName(const std::string &volumeName)
{
    m_volumeName= volumeName;
}

void crBulletObject::setVolumeLength(const CRCore::crVector3 &volumeLength)
{
	m_volumeLength= volumeLength;
}
void crBulletObject::setMaxTriggerCount(int triggerCount)
{
	m_maxTriggerCount= triggerCount;
}
void crBulletObject::setReTriggerDelay(float delay)
{
	m_reTriggerDelay= delay;
}
//void crBulletObject::setFlyEffectTypeString(const std::string &str)
//{
//    m_flyEffectTypeString = str;
//}
//
//const std::string &crBulletObject::getFlyEffectTypeString() const
//{
//    return m_flyEffectTypeString;
//}
//
//void crBulletObject::setCollideEffectTypeString(const std::string &str)
//{
//    m_collideEffectTypeString = str;
//}
//
//const std::string &crBulletObject::getCollideEffectTypeString() const
//{
//    return m_collideEffectTypeString;
//}

//void crBulletObject::setAssistantNode(CRCore::crNode *node)
//{
//	m_assistantNode = node;
//}

//////////////////////////////////
//
//crWeaponObject
//
////////////////////////////////

crWeaponObject::crWeaponObject():
    m_maxBulletsInOneClip(30),
	m_interval(0.5f),
	m_replaceClipTime(2.0f),
	m_attackDistance(10000),
	m_effectAttrMask(NoneEffectAttr),
	m_attackMask(AttackAll)
{
}

crWeaponObject::crWeaponObject(const crWeaponObject &weaponObject, const CRCore::crCopyOp& copyop):
	CRCore::crGroup(weaponObject,copyop),    
	m_maxBulletsInOneClip(weaponObject.m_maxBulletsInOneClip),
	m_interval(weaponObject.m_interval),
	m_replaceClipTime(weaponObject.m_replaceClipTime),
	m_attackDistance(weaponObject.m_attackDistance),
	m_effectAttrMask(weaponObject.m_effectAttrMask),
	m_attackMask(weaponObject.m_attackMask)
	//m_shotSounName(weaponObject.m_shotSounName),
 //   m_collideSounName(weaponObject.m_collideSounName)
{
}

crWeaponObject::~crWeaponObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crWeaponObject()"<< std::endl;
}

void crWeaponObject::setEffectAttrMask(EffectAttrMask attrmask)
{
    m_effectAttrMask = attrmask;
}

void crWeaponObject::setAttackMask(unsigned int mask)
{
	m_attackMask = mask;
}

void crWeaponObject::setMaxBulletsInOneClip(int num)
{
    m_maxBulletsInOneClip = num;
}

int crWeaponObject::getMaxBulletsInOneClip() const
{
    return m_maxBulletsInOneClip;
}

void crWeaponObject::setInterval(float interval)
{
    m_interval = interval;
}

float crWeaponObject::getInterval() const
{
    return m_interval;
}

void crWeaponObject::setReplaceClipTime(float interval)
{
	m_replaceClipTime = interval;
}

float crWeaponObject::getReplaceClipTime() const
{
	return m_replaceClipTime;
}

void crWeaponObject::setAttackDistance(float attackDistance)
{
	m_attackDistance = attackDistance;
}

void crWeaponObject::setGunPoint(const CRCore::crVector3& gunpoint)
{
    m_gunPoint = gunpoint;
}

void crWeaponObject::setClipPos(const CRCore::crVector3& clippos)
{
    m_clipPos = clippos;
}

crBulletMatterObject::crBulletMatterObject():
    m_flying(false),
	m_bulletGeomID(0),
	m_fireStartTime(0),
	m_collided(false),
	m_explodeFlag(0),
	m_weaponMatrixID(0),
	m_fireWeapon(NULL),
	m_explodeRadius(1.0f)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	setAvoidIntersect(true);
}

crBulletMatterObject::crBulletMatterObject(const crBulletMatterObject&bulletMatterObject,const CRCore::crCopyOp& copyop):
    crMatterObject(bulletMatterObject,copyop),
	m_flying(false),
	m_bulletGeomID(0),
	m_fireStartTime(0),
	m_collided(false),
	m_explodeFlag(0),
	m_weaponMatrixID(0),
	m_fireWeapon(NULL),
	m_explodeRadius(1.0f)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	setAvoidIntersect(true);
}

crBulletMatterObject::~crBulletMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crBulletMatterObject()"<< std::endl;
}

void crBulletMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	const crBulletObject *bulletObject = getBulletObject();
	if(!bulletObject)
	{
		CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::_create() error "<<std::endl;
		return;
	}
	crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();
	if(pt == crBulletObject::Type_Volume)
		return;
	float r = bulletObject->getModelPhysicsRadius();
	if(pt == crBulletObject::Type_Weapon || pt == crBulletObject::Type_Shield_Box)
	{//box
		//dMass m;
		//m_bodyID = dBodyCreate(worldID);
		//dMassSetBox (&m,m_density,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
		//dMassAdjust (&m,m_mass);
		//dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		//dBodySetMass (m_bodyID,&m);

		if(!m_bbox.valid())
		{
			if(r<=0.0f) r = 0.1f;
			setBoundBox(crVector3(0.0f,0.0f,0.0f),crVector3(r,r,r));
		}
		m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
		//dGeomSetBody (m_geomID,m_bodyID);
		//dBodySetGravityMode(m_bodyID,getBulletObject()->getAttrMask() & crBulletObject::GravityEnable);
		dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
		//dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	}
	else
	{
		//CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::_create() error "<<r<<std::endl;
		m_geomID = dCreateSphere(spaceID,r);
		dMass m;
		m_bodyID = dBodyCreate(worldID);
		dMassSetSphere (&m,m_density,r);
		dMassAdjust (&m,m_mass);
		dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		dBodySetMass (m_bodyID,&m);
		dGeomSetBody (m_geomID,m_bodyID);
		dBodySetGravityMode(m_bodyID,getBulletObject()->getAttrMask() & crBulletObject::GravityEnable);
		setGravityMode(getBulletObject()->getAttrMask() & crBulletObject::GravityEnable);
		dGeomSetPosition(m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
		dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	}

	switch(pt)
	{
	case crBulletObject::Type_Explode:
	case crBulletObject::Type_TimeExplode:
		m_explodeRadius = 1.0f;
		m_bulletGeomID = dCreateSphere(spaceID,m_explodeRadius/*bulletObject->getExecutionPhysicsLength()*/);
		//crOdeWorld::getDynamicGeomMap()[m_bulletGeomID] = this;   
		crOdeWorld::dynamicGeoMapInsert(m_bulletGeomID,this);
		break;
	case crBulletObject::Type_Sphere:
		m_bulletGeomID = dCreateSphere(spaceID,bulletObject->getExecutionPhysicsLength());
		//crOdeWorld::getDynamicGeomMap()[m_bulletGeomID] = this;   
		crOdeWorld::dynamicGeoMapInsert(m_bulletGeomID,this);
		break;
	case crBulletObject::Type_Ray:
		m_bulletGeomID = dCreateRay(spaceID,bulletObject->getExecutionPhysicsLength());
		//crOdeWorld::getDynamicGeomMap()[m_bulletGeomID] = this;
		crOdeWorld::dynamicGeoMapInsert(m_bulletGeomID,this);
	    break;
	}

	setFlying(false);

	if(m_bulletGeomID)
	{
		dGeomDisable(m_bulletGeomID);
	}
}

bool crBulletMatterObject::validExecution(void *hitbody,void *o)
{
	switch(getBulletObject()->getExecutionPhysicsType())
	{
	case crBulletObject::Type_Explode:
	case crBulletObject::Type_TimeExplode:
		return o == m_bulletGeomID && validExplode(hitbody);
	case crBulletObject::Type_Sphere:
	case crBulletObject::Type_Ray:
		return o == m_bulletGeomID;
	case crBulletObject::Type_Collide:
	case crBulletObject::Type_Weapon:
		return o == m_geomID; 
	case crBulletObject::Type_Shield_Box:
	case crBulletObject::Type_Shield_Sphere:
        return false;
	default:
		return false;
	}
}

void crBulletMatterObject::setEnableIntersect( bool enableIntersect)
{
	//if(m_enableIntersect != enableIntersect)
	//{//该条件判断会引起m_bulletGeomID在初始化的时候没有被dGeomDisable的错误
		crMatterObject::setEnableIntersect(enableIntersect);
	//}
}

void crBulletMatterObject::releaseObjects(CRCore::crState* state)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crBulletMatterObject::releaseObjects "<<std::endl;

	crMatterObject::releaseObjects(state);

	//m_firePerson = NULL;
	m_fireWeapon = NULL;
	m_trackTarget = NULL;
	if(m_volumeNode.valid())
	{
		m_volumeNode->releaseObjects(state);
		m_volumeNode = NULL;
	}
}

void crBulletMatterObject::releaseOde()
{	
	//if(m_bodyID) 
	//{
	//	dBodyDestroy(m_bodyID);
	//	m_bodyID = NULL;
	//}

	//if(m_geomID)
	//{
	//	dGeomDestroy(m_geomID);
	//	m_geomID = NULL;
	//}
	if(m_bulletGeomID)
	{
		//dGeomDestroy(m_bulletGeomID);
		crOdeWorld::dynamicGeoMapErase(m_bulletGeomID);
		m_bulletGeomID = NULL;
	}
	crMatterObject::releaseOde();
}

void crBulletMatterObject::updateCreMatrix()
{//避免子弹在飞行过程中不规则旋转
	//crMatterObject::updateCreMatrix();
	if(!m_geomID) return;

	if(m_bodyID)
	{
	    const dReal *d = dGeomGetPosition(m_geomID);
	    //m_matrix_buf.setTrans(d[0],d[1],d[2]);
	    //m_matrix_buf = CRCore::crMatrixd::rotate(-Y_AXIS,m_fireDirect) * CRCore::crMatrixd::translate(d[0],d[1],d[2]);
		crVector3 newPos(d[0],d[1],d[2]);
		crVector3 pos = m_matrix.getTrans();
		crVector3 dir = newPos - pos;
		dir.normalize();
		m_fireDirect = dir;
		m_matrix_buf = CRCore::crMatrixd::rotate(-Y_AXIS,dir) * CRCore::crMatrixd::translate(d[0],d[1],d[2]);

		dBodySetForce(m_bodyID,0,0,0);
		dBodySetTorque(m_bodyID,0,0,0);
	}
	else
	{
		//const dReal *d = dGeomGetPosition(m_geomID);
		//const dReal *r = dGeomGetRotation(m_geomID);

		//m_matrix_buf.set( r[0], r[4], r[8], 0,
		//	r[1], r[5], r[9], 0,
		//	r[2], r[6], r[10], 0,
		//	d[0], d[1], d[2], 1.0 );
	}
	dirtyBound();
}

const crBulletObject *crBulletMatterObject::getBulletObject() const
{
	return dynamic_cast<const crBulletObject *>(m_childArray[0].get());
}

void crBulletMatterObject::setTrackTarget(CRCore::crNode *trackTarget)
{
	const crBulletObject *bulletObject = getBulletObject();
	if(bulletObject->getAttrMask() & crBulletObject::Track)
	{
		m_trackTarget = trackTarget;
	}
}

void crBulletMatterObject::fire( crWeaponMatterObject *fireWeapon, int weaponMatixID, crMatterGroup *firePerson, const CRCore::crVector3 &gunpoint, const CRCore::crVector3 &target, double startTime )
{//
	//CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::fire = "<<firePerson->getName()<<std::endl;
	if(!firePerson)
		CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::fire() firePerson == 0 "<<std::endl;

	m_fireWeapon = fireWeapon;
	m_weaponMatrixID = weaponMatixID;
	m_fireTarget = target;
	//m_firePerson = firePerson;
	m_gunpoint = gunpoint;
	m_fireDirect = target - m_gunpoint;
	m_fireDirect.normalize();
	//m_matrix = CRCore::crMatrixd::rotate(Y_AXIS,m_fireDirect) * CRCore::crMatrixd::translate(m_matrix.getTrans());
	m_fireStartTime = startTime;
	//m_flySoundTime = startTime;
	//m_matrix.setTrans(gunpoint);
	const crBulletObject *bulletObject = getBulletObject();
	if(!bulletObject)
	{
		CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::fire() error "<<std::endl;
		return;
	}
	crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();
	if(pt != crBulletObject::Type_Volume)
	{
		dSpaceID voSpace = firePerson->getSpaceID();
		if(!m_physicsInited)
			create(firePerson->getOdeWorld()->getWorld(),voSpace);
		dSpaceID originalSpace = dGeomGetSpace(m_geomID);
		if(originalSpace!=voSpace)
		{
			dSpaceRemove(originalSpace,m_geomID);
			dSpaceAdd(voSpace,m_geomID);
		}
		dGeomSetPosition (m_geomID,m_gunpoint[0],m_gunpoint[1],m_gunpoint[2]);
		m_matrix.setTrans(gunpoint);
		//修正matrix_buf的方向信息
		if(m_bodyID)
		{//子弹按照物理仿真飞行
			dBodySetPosition(m_bodyID,m_gunpoint[0],m_gunpoint[1],m_gunpoint[2]);
			//crVector3 resistance = direct * bulletObject->getResistance();
			//   dBodySetForce(m_bodyID,resistance[0],resistance[1],resistance[2]);
			//(dynamic_cast<crViewMatterObject *>(firePerson->getChild(0)))->addForce(-resistance,0.2f);

			crViewMatterObject *firevo = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
			float vel = bulletObject->getLinearVel();
			if(vel>0.0f)
			{
				crVector3 linearVel = m_fireDirect * vel;
				dBodySetLinearVel(m_bodyID,linearVel[0],linearVel[1],linearVel[2]);
				//20110302，取消子弹的后座力
				//linearVel = -linearVel * m_mass / firevo->getMass();
				//firevo->setLinearVel(linearVel,0.05f);
			}
			else
			{
				crVector3f& viewVel = firevo->getVelocity();
				crVector3 viewDir = -firevo->getViewDirection();
				crVector3 viewDirX = -firevo->getDirectionX();
				crVector3 vel = viewDir * viewVel[0] + viewDirX * viewVel[1];
				dBodySetLinearVel(m_bodyID,vel[0],vel[1],vel[2]);

				float impetus = bulletObject->getImpetus();
				if(impetus>0.0f)
				{
					crVector3 force = m_fireDirect * impetus;
					float k = bulletObject->getK();
					if(k>0.0f)
						force += -vel * k;
					dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
					//20110302，取消子弹的后座力
					//firevo->addForce(-force,0.05f);
				}
			}
		}

		//CRCore::notify(CRCore::FATAL)<<" crBulletMatterObject::fire pt = "<<pt<<std::endl;

		switch(pt)
		{
		case crBulletObject::Type_Explode:
		case crBulletObject::Type_TimeExplode:
			if(originalSpace!=voSpace)
			{
				dSpaceRemove(originalSpace,m_bulletGeomID);
				dSpaceAdd(voSpace,m_bulletGeomID);
			}
			break;
		case crBulletObject::Type_Sphere://子弹在target出爆炸，球体半径要求:球不可以包容人
			if(originalSpace!=voSpace)
			{
				dSpaceRemove(originalSpace,m_bulletGeomID);
				dSpaceAdd(voSpace,m_bulletGeomID);
			}
			dGeomSetPosition(m_bulletGeomID,m_fireTarget[0],m_fireTarget[1],m_fireTarget[2]);
			break;
		case crBulletObject::Type_Ray:
			if(originalSpace!=voSpace)
			{
				dSpaceRemove(originalSpace,m_bulletGeomID);
				dSpaceAdd(voSpace,m_bulletGeomID);
			}
			dGeomRaySet(m_bulletGeomID,m_fireTarget[0],m_fireTarget[1],m_fireTarget[2],m_fireDirect[0],m_fireDirect[1],m_fireDirect[2]);
			break;
		case crBulletObject::Type_ExplodeVolume:
			{
				const CRCore::crVector3& volumeLength = bulletObject->getVolumeLength(); 
				if(!m_volumeNode.valid())
				{
					if(volumeLength[0] == volumeLength[1]/* && volumeLength[0] == volumeLength[2]*/)
					{
						m_volumeNode = new crSphereVolumeNode;
						m_volumeNode->setBoundBox(m_fireTarget,volumeLength*0.707f);
					}
					else
					{
						m_volumeNode = new crMatrixVolumeNode;
						crMatrix mat;
						crVector3 dir = m_fireDirect;
						if(dir == crVector3(0.0f,0.0f,0.0f))
						{
							dir[1] = -1.0f;
						}
						if(dir[1] > 0.999)
						{
							mat.makeRotate(-CRCore::Y_AXIS,dir);
							CRCore::crVector3d ep,center,up;
							mat.getLookAt(ep,center,up);
							if(center[2]>0.0f)
							{
								center = -center;
							}
							mat.makeLookAt(ep,center,up);
						}
						else
							mat.makeRotate(-CRCore::Y_AXIS,dir);
						mat.setTrans(m_gunpoint+m_fireDirect*(volumeLength[1]-2.0f));
						dynamic_cast<crMatrixVolumeNode *>(m_volumeNode.get())->setEffectMatrix(mat);
						crVector3 centerpos = crVector3(0.0f,0.0f/*2.0f-volumeLength[1]*/,0.0f);
						m_volumeNode->setBoundBox(centerpos,volumeLength);
					}
				}
				else
				{
					if(dynamic_cast<crSphereVolumeNode *>(m_volumeNode.get()))
					{
						m_volumeNode->setBoundBox(m_fireTarget,volumeLength*0.707f);
					}
					else
					{
						crMatrix mat;
						crVector3 dir = m_fireDirect;
						if(dir == crVector3(0.0f,0.0f,0.0f))
						{
							dir[1] = -1.0f;
						}
						if(dir[1] > 0.999)
						{
							mat.makeRotate(-CRCore::Y_AXIS,dir);
							CRCore::crVector3d ep,center,up;
							mat.getLookAt(ep,center,up);
							if(center[2]>0.0f)
							{
								center = -center;
							}
							mat.makeLookAt(ep,center,up);
						}
						else
							mat.makeRotate(-CRCore::Y_AXIS,dir);
						mat.setTrans(m_gunpoint+m_fireDirect*(volumeLength[1]-2.0f));
						dynamic_cast<crMatrixVolumeNode *>(m_volumeNode.get())->setEffectMatrix(mat);
						crVector3 centerpos = crVector3(0.0f,0.0f/*2.0f-volumeLength[1]*/,0.0f);
						m_volumeNode->setBoundBox(centerpos,volumeLength);
					}
				}
				m_volumeNode->setName(bulletObject->getName());
				m_volumeNode->setMaxTriggerCount(bulletObject->getMaxTriggerCount());
				m_volumeNode->setReTriggerDelay(bulletObject->getReTriggerDelay());
				m_volumeNode->setMainBody(this);
				m_volumeNode->setDataClass(this->getDataClass());
			}
			break;
		}

		setFlying(true);

		if(m_bulletGeomID)
		{
			if(pt == crBulletObject::Type_Explode || pt == crBulletObject::Type_TimeExplode)
				dGeomDisable(m_bulletGeomID);
			else
				dGeomEnable(m_bulletGeomID);
		}
		//else if(pt == crBulletObject::Type_Sphere || pt == crBulletObject::Type_Ray)
		//{
		//	const std::string &effectTypeString = bulletObject->getCollideEffectTypeString();
		//	if(!effectTypeString.empty())
		//	{
		//		crParticleEffect *particleEffect = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectTypeString);
		//		if(particleEffect)
		//		{
		//			particleEffect->setEffectPosition(m_fireTarget);
		//			particleEffect->startEffect();
		//		}
		//	}
		//}
		m_collided = false;
	}
	else
	{//在目标位置建立一个volume区域，无需物理模型
		const CRCore::crVector3& volumeLength = bulletObject->getVolumeLength(); 
		if(!m_volumeNode.valid())
		{/////////////////////////////
			if(volumeLength[0] == volumeLength[1]/* && volumeLength[0] == volumeLength[2]*/)
			{
				m_volumeNode = new crSphereVolumeNode;
				m_volumeNode->setBoundBox(m_fireTarget,volumeLength*0.707f);
			}
			else
			{
				m_volumeNode = new crMatrixVolumeNode;
				crMatrix mat;
				crVector3 dir = m_fireDirect;
				if(dir == crVector3(0.0f,0.0f,0.0f))
				{
					dir[1] = -1.0f;
				}
				if(dir[1] > 0.999)
				{
					mat.makeRotate(-CRCore::Y_AXIS,dir);
					CRCore::crVector3d ep,center,up;
					mat.getLookAt(ep,center,up);
					if(center[2]>0.0f)
					{
						center = -center;
					}
					mat.makeLookAt(ep,center,up);
				}
				else
					mat.makeRotate(-CRCore::Y_AXIS,dir);
				mat.setTrans(m_gunpoint+m_fireDirect*(volumeLength[1]-2.0f));
				dynamic_cast<crMatrixVolumeNode *>(m_volumeNode.get())->setEffectMatrix(mat);
				crVector3 centerpos = crVector3(0.0f,0.0f/*2.0f-volumeLength[1]*/,0.0f);
				m_volumeNode->setBoundBox(centerpos,volumeLength);
			}
		}
		else
		{
			if(dynamic_cast<crSphereVolumeNode *>(m_volumeNode.get()))
			{
				m_volumeNode->setBoundBox(m_fireTarget,volumeLength*0.707f);
			}
			else
			{
				crMatrix mat;
				crVector3 dir = m_fireDirect;
				if(dir == crVector3(0.0f,0.0f,0.0f))
				{
					dir[1] = -1.0f;
				}
				if(dir[1] > 0.999)
				{
					mat.makeRotate(-CRCore::Y_AXIS,dir);
					CRCore::crVector3d ep,center,up;
					mat.getLookAt(ep,center,up);
					if(center[2]>0.0f)
					{
						center = -center;
					}
					mat.makeLookAt(ep,center,up);
				}
				else
					mat.makeRotate(-CRCore::Y_AXIS,dir);
				mat.setTrans(m_gunpoint+m_fireDirect*(volumeLength[1]-2.0f));
				dynamic_cast<crMatrixVolumeNode *>(m_volumeNode.get())->setEffectMatrix(mat);
				crVector3 centerpos = crVector3(0.0f,0.0f/*2.0f-volumeLength[1]*/,0.0f);
				m_volumeNode->setBoundBox(centerpos,volumeLength);
			}
		}
		m_volumeNode->setName(bulletObject->getName());
		m_volumeNode->setMaxTriggerCount(bulletObject->getMaxTriggerCount());
		m_volumeNode->setReTriggerDelay(bulletObject->getReTriggerDelay());
		m_volumeNode->setMainBody(this);
		m_volumeNode->setDataClass(this->getDataClass());
		m_volumeNode->tryEnableVolume(true);
		crVolumeNodeManager::getInstance()->wantAddVolumeNode(m_volumeNode.get());
		setFlying(true);
		m_collided = false;
	}
	doEvent(WCH_BULLET_FIRE);
}

bool crBulletMatterObject::isExploding()
{
    return m_explodeFlag != 0;
}

void crBulletMatterObject::explode(const CRCore::crVector3 &collidePos)
{
	m_fireTarget = collidePos;//保存爆炸体的球心
	m_explodeFlag = 1;
	m_explodeRadius = 1.0f;

	doEvent(WCH_BULLET_EXPLODE,MAKEINT64(&m_fireTarget,NULL));
	//const crBulletObject *bulletObject = getBulletObject();
	//crBulletObject::EffectAttrMask effectAttrMask = bulletObject->getEffectAttrMask();
	//std::string effectString = getName()+"_explode";
	//if(effectAttrMask & crBulletObject::Explode_Particle)
	//{
	//	m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
	//	if(m_particleEffectNode)
	//	{
	//		m_particleEffectNode->setEffectPosition(collidePos);
	//		m_particleEffectNode->startEffect();
	//	}
	//}
	//if(effectAttrMask & crBulletObject::Explode_Sequence)
	//{
	//	m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
	//	if(m_sequenceNode)
	//	{
	//		m_sequenceNode->setEffectPosition(collidePos);
	//		m_sequenceNode->startEffect();
	//	}
	//}
	//if(effectAttrMask & crBulletObject::Explode_Sound)
	//{
	//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
	//	m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(m_firePerson->getName(),effectString);
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
}

void crBulletMatterObject::explodeEnd()
{
	dGeomDisable(m_bulletGeomID);
	m_explodeFlag = 0;
	m_explodedSet.clear();
}
bool crBulletMatterObject::validExplode(void *hitbody)
{
	if(isExploding() && m_explodedSet.find(hitbody) == m_explodedSet.end())
	{
		m_explodedSet.insert(hitbody);
		return true;
	}
	return false;
}
//crMatterGroup *crBulletMatterObject::getFirePerson()
//{
//    return m_firePerson.get();
//}

crWeaponMatterObject *crBulletMatterObject::getFireWeapon()
{
    return m_fireWeapon; 
}

float crBulletMatterObject::getExecutionRate(const CRCore::crVector3 &collidePos) const
{
	const crBulletObject *bulletObject = getBulletObject();
	crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();
	float rate = 1.0f;
	if(pt == crBulletObject::Type_Explode || pt == crBulletObject::Type_TimeExplode)
	{
		float explodeRadius = bulletObject->getExecutionPhysicsLength();
		float distance = (collidePos - m_fireTarget).length();
		rate = (explodeRadius - distance) / explodeRadius;
		rate = CRCore::clampTo(rate,0.1f,1.0f);
	}
	return rate;
}

bool crBulletMatterObject::isFriendFire(crViewMatterObject *collideBot)
{
    const crViewMatterObject *firePerson = m_fireWeapon->getFireBody();//dynamic_cast<crViewMatterObject *>(getFirePerson()->getChild(0));
	return firePerson->checkTeam(collideBot->getTeam())>0;
}

void crBulletMatterObject::setFlying(bool flying)
{
    m_flying = flying;
	const crBulletObject *bulletObject = getBulletObject();

	if(!m_flying)
	{
		crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();	
		if(pt == crBulletObject::Type_Volume || pt == crBulletObject::Type_ExplodeVolume)
		{
			if(m_volumeNode.valid()) 
			{
				m_volumeNode->tryEnableVolume(false);
				crVolumeNodeManager::getInstance()->removeVolumeNode(m_volumeNode.get());
			}
		}
		setVisiable(false);
		setEnableIntersect(false);
		resetPhysicsSwap();

		if(m_bodyID)
		{
		    dBodySetForce(m_bodyID,0,0,0);
		    dBodySetTorque(m_bodyID,0,0,0);
		    dBodySetLinearVel(m_bodyID,0,0,0);
		}
	}
	else
	{
		setVisiable(true);
		setEnableIntersect(true);
        enablePhysics(bulletObject->getDuration());
		doEvent(WCH_BULLET_FLY,MAKEINT64(NULL,NULL));
	}
}

void crBulletMatterObject::bulletGeomCollide()
{
	//if(m_bulletGeomID)
	//{
	//	dGeomDisable (m_bulletGeomID);
	//}
}

bool crBulletMatterObject::getFlying()
{
    return m_flying;
}

bool crBulletMatterObject::isBulletMatterObjectIdle()
{
    return !m_flying && m_explodeFlag == 0;
}

bool crBulletMatterObject::getCollided()
{
    return m_collided;
}

void crBulletMatterObject::setCollided(bool collided)
{
    m_collided = collided;
}

void crBulletMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			const crBulletObject *bulletObject = getBulletObject();
			crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();

            crVector3 linearVel;
			if(bulletObject && m_flying)
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				crVector3 position = getTrans();//getBound().center();
				if(pt != crBulletObject::Type_Volume)
				{
					if(!m_bodyID)
					{//
						//CRPhysics::crWeaponMatterObject::WeaponMatrixVec matrixVec = m_fireWeapon->getWeaponMatrixVec();
						//if(matrixVec.empty())
							setMatrix(m_fireWeapon->getMatrix());
						//else
						//{
						//	setMatrix(matrixVec[m_weaponMatrixID].first);
						//}

						CRCore::crVector3 pos = m_matrix.getTrans();
						dGeomSetPosition (m_geomID,pos[0],pos[1],pos[2]);
						dMatrix3 r;
						crOdeWorld::creatureToOdeMat(m_matrix,r);
						dGeomSetRotation(m_geomID,r);
					}
					else
					{
						float impetus = bulletObject->getImpetus();
						//float lvel = bulletObject->getLinearVel();
						crVector3 force;
						const crVector3 &extForce = bulletObject->getForce();
						dBodySetForce(m_bodyID,extForce[0],extForce[1],extForce[2]);
						if(m_trackTarget.valid() && /*!m_trackTarget->isDead() &&*/ (bulletObject->getAttrMask() & crBulletObject::Track))
						{//加入跟踪导弹的算法
							if(m_trackTarget->getVisiable())
							{
								crVector3 trackTarget;
								if(dynamic_cast<crMatterObject *>(m_trackTarget.get()))
									trackTarget = dynamic_cast<crMatterObject *>(m_trackTarget.get())->getPhysicsCenter();
								else
									trackTarget = m_trackTarget->getBound().center();
								//if((position - m_gunpoint).length2() < (trackTarget - m_gunpoint).length2())
								{
									const CRCore::rangef& precision = bulletObject->getMissilePrecision();
									trackTarget += crVector3(precision.get_random(),precision.get_random(),precision.get_random());

									crVector3 fireDirect = trackTarget - position;
									fireDirect.normalize();

									float cos_a = fireDirect * m_fireDirect;
									if(cos_a>bulletObject->getMissileFlexibility())
									{//45度以内
										//m_fireDirect = fireDirect;
										const dReal* bodyVel = dBodyGetLinearVel(m_bodyID);
										crVector3 realVel(bodyVel[0],bodyVel[1],bodyVel[2]);
										linearVel = fireDirect * realVel.length();
										dBodySetLinearVel(m_bodyID,linearVel[0],linearVel[1],linearVel[2]);
									}
								}
							}

							if(impetus>0.0f)
							{
								force = m_fireDirect * impetus;
								dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
								//加入空气阻力所产生的浮力影响
							}
							//else if(lvel>0.0f)
							//{
							//	dBodySetLinearVel(m_bodyID,linearVel[0],linearVel[1],linearVel[2]);
							//crVector3 linearVel = m_fireDirect * lvel;
							//dBodySetLinearVel(m_bodyID,linearVel[0],linearVel[1],linearVel[2]);
							//}
							//linearVel = m_fireDirect * realVel.length();
							//dBodySetLinearVel(m_bodyID,linearVel[0],linearVel[1],linearVel[2]);

							//if(m_trackTarget->getCameraBind())
							//{//waring
							//	crDrawable *drawable = crFilterRenderManager::getInstance()->getDrawable("Warning");
							//	if(drawable) drawable->setVisiable(true);
							//}
						}
						else 
						{
							if(impetus>0.0f)
							{
								force = m_fireDirect * impetus;
								dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
								//加入空气阻力所产生的浮力影响
							}
						}
						float k = bulletObject->getK();
						if(k>0.0f)
						{
							force = -linearVel * k;
							dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
						}
					}
				}
				if((referenceTime - m_fireStartTime) > bulletObject->getDuration())
				{
					//CRCore::notify(CRCore::FATAL)<<"crBulletMatterObject::traverse() "<<std::endl;
					setFlying(false);
					//if((bulletObject->getAttrMask() & crBulletObject::Track))
					//{//提示miss
					//	crViewMatterObject *firePerson = dynamic_cast<crViewMatterObject *>(m_firePerson->getChild(0));
					//	if(firePerson->getCameraBind())
					//	{
					//		//crDrawable *drawable = crFilterRenderManager::getInstance()->getDrawable("Miss");
					//		//if(drawable) drawable->setVisiable(true);
					//		//crNode *node = crFilterRenderManager::getInstance()->getFilterNode("Miss");
					//		//if(node) node->setVisiable(true);
					//		if(!m_cautionSW.valid()) m_cautionSW = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode("Caution"));
					//		if(m_cautionSW.valid())
					//		{
					//			//if(!m_miss.valid()) m_miss = crFilterRenderManager::getInstance()->getFilterNode("Miss");
					//			//if(m_miss.valid()) m_miss->setVisiable(true);
					//			m_cautionSW->setActiveSwitchSet(5);
					//			m_cautionSW->getFirstActiveChild()->setVisiable(true);
					//		}
					//	}
					//}
					switch (bulletObject->getExecutionPhysicsType())
					{
					case crBulletObject::Type_TimeExplode:
					case crBulletObject::Type_ExplodeVolume:
						{
                            explode(position);
						}
						break;
					}
				}
				else
				{
					doEvent(WCH_BULLET_FLY,MAKEINT64(&linearVel,NULL));
				}
			}
			if(m_explodeFlag==1)
			{
				switch (bulletObject->getExecutionPhysicsType())
				{
				case crBulletObject::Type_Explode:
				case crBulletObject::Type_TimeExplode:
					dGeomSetPosition(m_bulletGeomID,m_fireTarget[0],m_fireTarget[1],m_fireTarget[2]);
					dGeomEnable(m_bulletGeomID);
					break;
				case crBulletObject::Type_ExplodeVolume:
					m_volumeNode->tryEnableVolume(true);
					crVolumeNodeManager::getInstance()->wantAddVolumeNode(m_volumeNode.get());
					break;
				}

				m_explodeFlag = 2;
			}
			else if(m_explodeFlag==2)
			{
				m_explodeRadius += 1.0f;
				if(m_explodeRadius<bulletObject->getExecutionPhysicsLength())
					dGeomSphereSetRadius(m_bulletGeomID,m_explodeRadius);
				else
				{
					m_explodeRadius = 1.0f;
					explodeEnd();
				}
			}
		}
		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

//////////////////////////////////
//
//crWeaponMatterObject
//
////////////////////////////////
#define EnableWeaponPhysics 0
crWeaponMatterObject::crWeaponMatterObject():
    m_bulletCount(0),
	m_bulletsInWeapon(0),
	m_comeForthInterval(0.0f),
	m_lastComeFortTime(0.0f),
	m_lastFireTime(0.0f),
	m_lastReplaceClipTime(0.0f),
	//m_bulletObjectIndex(0),
	m_state(0),
	m_attype(AT_FAR),
	m_fireBody(NULL)
	//m_particleEffectNode(NULL),
	//m_sequenceNode(NULL),
	//m_soundState(NULL)
{
	//setCullingActive(false);
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	setAvoidIntersect(true);
}

crWeaponMatterObject::crWeaponMatterObject(const crWeaponMatterObject& weapon,const crCopyOp& copyop):
    crMatterObject(weapon,copyop),
	m_bulletCount(weapon.m_bulletCount),
	m_bulletsInWeapon(weapon.m_bulletsInWeapon),
	m_comeForthInterval(weapon.m_comeForthInterval),
	m_lastComeFortTime(weapon.m_lastComeFortTime),
	m_lastFireTime(weapon.m_lastFireTime),
	m_lastReplaceClipTime(weapon.m_lastReplaceClipTime),
	//m_bulletObjectIndex(weapon.m_bulletObjectIndex),
	m_state(weapon.m_state),
	m_attype(weapon.m_attype),
    //m_weaponRackName(weapon.m_weaponRackName),
	m_fireBody(NULL)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	setAvoidIntersect(true);
}

crWeaponMatterObject::~crWeaponMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crWeaponMatterObject()"<< std::endl;
}

void crWeaponMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_comeForthInterval>0.0f && !getVisiable())
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				if(m_state==0)
				{
					m_lastComeFortTime = referenceTime;
					m_state++;
				}
				else if((referenceTime - m_lastComeFortTime) > m_comeForthInterval)
				{
					setVisiable(true);
					setEnableIntersect(true);
					m_state = 0;
				}
			}
		}
		break;
	//case crNodeVisitor::CULL_VISITOR:
	//	{
	//	    CRCore::notify(CRCore::ALWAYS)<<"crWeaponMatterObject::CULL_VISITOR "<<m_weaponMatrixVec.size()<<std::endl;

	//		for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
	//			 itr != m_weaponMatrixVec.end();
	//			 ++itr )
	//		{
	//			setMatrix(*itr);
	//		}
	//	}
	//	return;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

void crWeaponMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
#if EnableWeaponPhysics
	if(!m_bbox.valid())
	{
		setBoundBox(crVector3(0.0f,0.0f,0.0f),crVector3(0.1f,0.1f,0.1f));
	}
	m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
#endif
}

void crWeaponMatterObject::releaseObjects(CRCore::crState* state)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crWeaponMatterObject::releaseObjects"<<std::endl;

 //   crWeaponManager::BulletMatterObjectMap &bulletMatterObjectMap = crWeaponManager::getInstance()->getBulletMatterObjectMap();
	//crWeaponManager::BulletMatterObjectMap::const_iterator itr = crWeaponManager::getInstance()->getBulletMatterObjectMap().find(m_bulletObjectIndex);
	//for( ; itr != bulletMatterObjectMap.end() && itr->first == m_bulletObjectIndex; ++itr )
	//{
	//	if(itr->second->getFireWeapon() == this)
	//	{
	//		itr->second->releaseObjects(state);
	//	}
	//}
	for( BulletMatterObjectVec::iterator itr = m_bulletVec.begin();
		 itr != m_bulletVec.end();
		 ++itr )
	{
		(*itr)->releaseObjects(state);
	}

	crMatterObject::releaseObjects(state);
	m_fireBody = NULL;
}
void crWeaponMatterObject::releaseOde()
{
	for( BulletMatterObjectVec::iterator itr = m_bulletVec.begin();
		itr != m_bulletVec.end();
		++itr )
	{
		(*itr)->releaseOde();
	}
	crMatterObject::releaseOde();
}
void crWeaponMatterObject::setBulletCount(int bulletCount)
{
    m_bulletCount = bulletCount;
}

void crWeaponMatterObject::setBulletsInWeapon(int bulletsInWeapon)
{
    m_bulletsInWeapon = bulletsInWeapon;
}

bool crWeaponMatterObject::isBulletEmpty() const
{
	return m_bulletsInWeapon>-1 && (m_bulletCount + m_bulletsInWeapon == 0); 
}

void crWeaponMatterObject::setComeForthInterval(float interval)
{
	m_comeForthInterval = interval;
}

float crWeaponMatterObject::getComeForthInterval()const
{
	return m_comeForthInterval;
}

//void crWeaponMatterObject::setBulletObjectIndex( int index )
//{
//    m_bulletObjectIndex = index;
//}
//
//int crWeaponMatterObject::getBulletObjectIndex()const
//{
//    return m_bulletObjectIndex;
//}

const crWeaponObject *crWeaponMatterObject::getWeaponObject()const
{
    return dynamic_cast<const crWeaponObject *>(m_childArray[0].get());
}

const crBulletObject *crWeaponMatterObject::getBulletObject()const
{
    //return crWeaponManager::getInstance()->getBullet(m_bulletObjectIndex);
	const crBulletMatterObject *bullet = getBullect();
	if(bullet)
		return bullet->getBulletObject();
	return NULL;
}

void crWeaponMatterObject::setAttackType(AttackType mm)
{
	m_attype = mm;
}

//void crWeaponMatterObject::setWeaponRackName(const std::string &rackName)
//{
//    m_weaponRackName = rackName;
//}

//void crWeaponMatterObject::addBulletMatterObject(crBulletMatterObject *bulletMatterObject)
//{
//    m_bulletMatterObjects.push_back(bulletMatterObject);
//}

//crWeaponMatterObject::BulletMatterObjectVec &crWeaponMatterObject::getBulletMatterObjectVec()
//{
//    return m_bulletMatterObjects;
//}

bool crWeaponMatterObject::setBullet(crBulletMatterObject *bulletMatterObject)
{
    if(m_bulletVec.empty())
	{
		m_bulletVec.push_back(bulletMatterObject);
		return true;
	}
	return false;
}
const crBulletMatterObject *crWeaponMatterObject::getBullect() const
{
	if(!m_bulletVec.empty())
	{
		return m_bulletVec.front().get();
	}
	return NULL;
}
CRCore::crVector3 crWeaponMatterObject::getGunPoint()
{
	return getWeaponObject()->getGunPoint() * m_matrix;
}
crBulletMatterObject *crWeaponMatterObject::getOrCreateIdleBullet()
{
	const crBulletMatterObject *defaultBulletMatterObject = getBullect();
	if(defaultBulletMatterObject)
	{
		for( BulletMatterObjectVec::iterator itr = m_bulletVec.begin();
			 itr != m_bulletVec.end();
			 ++itr )
		{
			if((*itr)->isBulletMatterObjectIdle())
			{
				//CRCore::notify(CRCore::FATAL)<<"crWeaponMatterObject::getOrCreateIdleBullet find idle bullet"<<std::endl;
				return itr->get();
			}
		}
		crBulletMatterObject *bulletMatterObject = dynamic_cast<crBulletMatterObject *>(defaultBulletMatterObject->clone(crCopyOp::DEEP_COPY_DATAS));
		bulletMatterObject->setVisiable(false);
		(const_cast<crGroup *>(defaultBulletMatterObject->getParent(0)))->addChild(bulletMatterObject);
		m_bulletVec.push_back(bulletMatterObject);
		//CRCore::crIntersectVisitor::insertStaticAvoidIntersectNode(bulletMatterObject);

		return bulletMatterObject;
		//CRCore::notify(CRCore::FATAL)<<"crWeaponMatterObject::getOrCreateIdleBullet ----size = "<<m_bulletVec.size()<<std::endl;
	}
	return NULL;
}

crWeaponMatterObject::ReadyToFire crWeaponMatterObject::isReadytoFire(double currentTime)
{
    const crWeaponObject *weapon = getWeaponObject();
	if(!weapon)
	{
		return NotReady;
	}

	if((currentTime - m_lastFireTime)<weapon->getInterval()) return NotReady;

	if(m_bulletsInWeapon > -1)//无限弹药
	{
		if((currentTime - m_lastReplaceClipTime)<weapon->getReplaceClipTime()) return NotReady;

		if(m_bulletsInWeapon==0)
		{
			if(m_bulletCount>0)
				return NeedReplaceClip;
			else
				return NotReady;
		}
	}
    m_lastFireTime = currentTime;
	return Ready;
}

void crWeaponMatterObject::fire( crMatterGroup *firePerson, const CRCore::crVector3& target, double time )
{
	ReadyToFire readyToFire = isReadytoFire(time);
	if(readyToFire==NeedReplaceClip)
	{
		//CRCore::notify(CRCore::FATAL)<<" NeedReplaceClip "<<std::endl;
		replaceClip(firePerson,time);
		return;
	}
	else if(readyToFire==NotReady)
		return;

#if EnableWeaponPhysics
	if(!m_physicsInited)
	{
		//setEnableIntersect(true);
		create(firePerson->getOdeWorld()->getWorld(),firePerson->getSpaceID());
	}
	else
	{
		dSpaceID originalSpace = dGeomGetSpace(m_geomID);
		dSpaceID voSpace = firePerson->getSpaceID();
		if(originalSpace!=voSpace)
		{
			dSpaceRemove(originalSpace,m_geomID);
			dSpaceAdd(voSpace,m_geomID);
			//setEnableIntersect(true);
		}
	}
#endif

	m_fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
	m_fireBody->doEvent(WCH_WEAPON_FIRE,MAKEINT64(this,NULL));
	//doEvent(WCH_WEAPON_FIRE,MAKEINT64(&m_name,NULL));

	crBulletMatterObject *bulletMatterObject;
	crWeaponManager *weaponManager = crWeaponManager::getInstance();
	const crWeaponObject *weaponObject = getWeaponObject();
	crVector3 gunpoint;
	int id = 0;
	//for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
	//	itr != m_weaponMatrixVec.end();
	//	++itr,id++ )
	do
	{
	//	setMatrix(itr->second);
		///fire
		bulletMatterObject = getOrCreateIdleBullet();//weaponManager->getOrCreateIdleBulletMatterObject(m_bulletObjectIndex);
		//CRCore::notify(CRCore::FATAL)<<"weaponMatterObject "<<getName()<<std::endl;
		if(!bulletMatterObject) break;

		bulletMatterObject->setTrackTarget(NULL);
		gunpoint = weaponObject->getGunPoint() * m_matrix;
		_fire(bulletMatterObject,firePerson,gunpoint,target,id);
		if(m_bulletsInWeapon == 0) break;
	}while(0);
}

void crWeaponMatterObject::fire( crMatterGroup *firePerson, crGameBodyInfo::LockedTargetSet &targets,double time )
{
	ReadyToFire readyToFire = isReadytoFire(time);
	if(readyToFire==NeedReplaceClip)
	{
		//CRCore::notify(CRCore::FATAL)<<" NeedReplaceClip "<<std::endl;
		replaceClip(firePerson,time);
		return;
	}
	else if(readyToFire==NotReady)
		return;

#if EnableWeaponPhysics
	if(!m_physicsInited)
	{
		//setEnableIntersect(true);
		create(firePerson->getOdeWorld()->getWorld(),firePerson->getSpaceID());
	}
	else
	{
		dSpaceID originalSpace = dGeomGetSpace(m_geomID);
		dSpaceID voSpace = firePerson->getSpaceID();
		if(originalSpace!=voSpace)
		{
			dSpaceRemove(originalSpace,m_geomID);
			dSpaceAdd(voSpace,m_geomID);
			//setEnableIntersect(true);
		}
	}
#endif

	m_fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
	m_fireBody->doEvent(WCH_WEAPON_FIRE,MAKEINT64(this,NULL));
	//doEvent(WCH_WEAPON_FIRE,MAKEINT64(&m_name,NULL));

	crBulletMatterObject *bulletMatterObject;
	crVector3 fireDir;
	CRCore::ref_ptr<crViewMatterObject> setItr;
	crGameBodyInfo::LockedTargetSet::iterator targetItr = targets.begin();
	setItr = *targetItr;
	crViewMatterObject *target;
	//crViewMatterObject *fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
	const crWeaponObject *weaponObject = getWeaponObject();
	crVector3 gunpoint;
	int id = 0;
	//for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
	//	itr != m_weaponMatrixVec.end();
	//	++itr,id++ )
	do 
	{
		target = setItr.get();
		//setMatrix(itr->second);
		///fire
		bulletMatterObject = getOrCreateIdleBullet();//crWeaponManager::getInstance()->getOrCreateIdleBulletMatterObject(m_bulletObjectIndex);
		if(!bulletMatterObject) break;

		//CRCore::notify(CRCore::FATAL)<<"weaponMatterObject "<<getName()<<std::endl;
		bulletMatterObject->setTrackTarget(target);
		gunpoint = weaponObject->getGunPoint() * m_matrix;
		fireDir = -getDirection();
		_fire(bulletMatterObject,firePerson,gunpoint,gunpoint + fireDir,id);
		if(m_bulletsInWeapon == 0) break;
		//targetItr++;
		//if(targetItr == targets.end())
		//{
		//	//targetItr = targets.begin();
		//	break;
		//}
	}while (0);
}

void crWeaponMatterObject::fire( crMatterGroup *firePerson, CRCore::crNode *target,double time )
{
	ReadyToFire readyToFire = isReadytoFire(time);
	if(readyToFire==NeedReplaceClip)
	{
		//CRCore::notify(CRCore::FATAL)<<" NeedReplaceClip "<<std::endl;
		replaceClip(firePerson,time);
		return;
	}
	else if(readyToFire==NotReady)
		return;

#if EnableWeaponPhysics
	if(!m_physicsInited)
	{
		//setEnableIntersect(true);
		create(firePerson->getOdeWorld()->getWorld(),firePerson->getSpaceID());
	}
	else
	{
		dSpaceID originalSpace = dGeomGetSpace(m_geomID);
		dSpaceID voSpace = firePerson->getSpaceID();
		if(originalSpace!=voSpace)
		{
			dSpaceRemove(originalSpace,m_geomID);
			dSpaceAdd(voSpace,m_geomID);
			//setEnableIntersect(true);
		}
	}
#endif

	m_fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
	m_fireBody->doEvent(WCH_WEAPON_FIRE,MAKEINT64(this,NULL));
	//doEvent(WCH_WEAPON_FIRE,MAKEINT64(&m_name,NULL));

	crBulletMatterObject *bulletMatterObject;
	crVector3 fireDir;
	const crWeaponObject *weaponObject = getWeaponObject();
	crVector3 gunpoint;
	int id = 0;
	//for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
	//	itr != m_weaponMatrixVec.end();
	//	++itr,id++ )
	do 
	{
		//setMatrix(itr->second);
		///fire
		bulletMatterObject = getOrCreateIdleBullet();//crWeaponManager::getInstance()->getOrCreateIdleBulletMatterObject(m_bulletObjectIndex);
		if(!bulletMatterObject) break;

		//CRCore::notify(CRCore::FATAL)<<"weaponMatterObject "<<getName()<<std::endl;
		bulletMatterObject->setTrackTarget(target);
		gunpoint = weaponObject->getGunPoint() * m_matrix;
		fireDir = -getDirection();
		_fire(bulletMatterObject,firePerson,gunpoint,gunpoint+fireDir,id);
		if(m_bulletsInWeapon == 0) break;
	} while (0);
}

void crWeaponMatterObject::fire_noTarget( crMatterGroup *firePerson, const CRCore::crVector3& offset, double time )
{
	ReadyToFire readyToFire = isReadytoFire(time);
	if(readyToFire==NeedReplaceClip)
	{
		//CRCore::notify(CRCore::FATAL)<<" NeedReplaceClip "<<std::endl;
		replaceClip(firePerson,time);
		return;
	}
	else if(readyToFire==NotReady)
		return;

#if EnableWeaponPhysics
	if(!m_physicsInited)
	{
		//setEnableIntersect(true);
		create(firePerson->getOdeWorld()->getWorld(),firePerson->getSpaceID());
	}
	else
	{
		dSpaceID originalSpace = dGeomGetSpace(m_geomID);
		dSpaceID voSpace = firePerson->getSpaceID();
		if(originalSpace!=voSpace)
		{
			dSpaceRemove(originalSpace,m_geomID);
			dSpaceAdd(voSpace,m_geomID);
			//setEnableIntersect(true);
		}
	}
#endif

	m_fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
	m_fireBody->doEvent(WCH_WEAPON_FIRE,MAKEINT64(this,NULL));
	//doEvent(WCH_WEAPON_FIRE,MAKEINT64(&m_name,NULL));

	crBulletMatterObject *bulletMatterObject;
	crVector3 fireDir;
	const crWeaponObject *weaponObject = getWeaponObject();
	crVector3 gunpoint;
	int id = 0;
	//setMatrix(m_weaponMatrixVec.begin()->second);
	gunpoint = weaponObject->getGunPoint() * m_matrix;
	fireDir = -getDirection() - offset;
	fireDir.normalize();
	///fire
	bulletMatterObject = getOrCreateIdleBullet();//crWeaponManager::getInstance()->getOrCreateIdleBulletMatterObject(m_bulletObjectIndex);
	if(!bulletMatterObject) return;

	//CRCore::notify(CRCore::FATAL)<<"weaponMatterObject "<<getName()<<std::endl;
	bulletMatterObject->setTrackTarget(NULL);
	_fire(bulletMatterObject,firePerson,gunpoint,gunpoint + fireDir,id);
	//for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
	//	itr != m_weaponMatrixVec.end();
	//	++itr )
	//{
	//	setMatrix(*itr);
	//	fireDir = getMatterObjectPosition()-(getDirection() + offset);
	//	///fire
	//	bulletMatterObject = crWeaponManager::getInstance()->getOrCreateIdleBulletMatterObject(m_bulletObjectIndex);
	//	//CRCore::notify(CRCore::FATAL)<<"weaponMatterObject "<<getName()<<std::endl;
	//	bulletMatterObject->setTrackTarget(NULL);
	//	_fire(bulletMatterObject,firePerson,fireDir);
	//	if(m_bulletsInWeapon == 0) break;
	//}
}

void crWeaponMatterObject::_fire(crBulletMatterObject *bulletMatterObject, crMatterGroup *firePerson, const CRCore::crVector3& gunpoint, const CRCore::crVector3& target, int id)
{
	if(m_bulletsInWeapon>0) m_bulletsInWeapon--;
	//crVector3 dir = target - gunpoint;
	//dir.normalize();
	CRCore::crVector3 firetarget = target;
	doEvent(WCH_WEAPON_FIRE,MAKEINT64(&gunpoint,&firetarget));
	bulletMatterObject->fire(this,id,firePerson,gunpoint,firetarget,m_lastFireTime);
}

void crWeaponMatterObject::especialFire( crMatterGroup *firePerson, const CRCore::crVector3& target, double time )
{
}

void crWeaponMatterObject::replaceClip(crMatterGroup *firePerson, double time)
{
	if(m_bulletsInWeapon==-1)//无限弹药
		return;

	const crWeaponObject *weapon = getWeaponObject();
	if(weapon)
	{
		if((time - m_lastReplaceClipTime)<weapon->getReplaceClipTime()) return;

		if(m_bulletCount>0 && m_bulletsInWeapon<weapon->getMaxBulletsInOneClip())
		{
			crViewMatterObject *fireBody = dynamic_cast<crViewMatterObject *>(firePerson->getChild(0));
			fireBody->doEvent(WCH_WEAPON_REPLACECLIP,MAKEINT64(this,NULL));

			m_lastReplaceClipTime = time;
			m_bulletCount -= weapon->getMaxBulletsInOneClip() - m_bulletsInWeapon;
			m_bulletsInWeapon = weapon->getMaxBulletsInOneClip();
			if(m_bulletCount<0)
			{
				m_bulletsInWeapon += m_bulletCount;
				m_bulletCount = 0;
			}

			CRCore::crVector3f clippos;
			clippos = weapon->getClipPos() * m_matrix;
			doEvent(WCH_WEAPON_REPLACECLIP,MAKEINT64(&clippos,NULL));
			//for( WeaponMatrixVec::iterator itr = m_weaponMatrixVec.begin();
			//	itr != m_weaponMatrixVec.end();
			//	++itr )
			//{
   //             clippos = weapon->getClipPos() * (itr->second);
			//	doEvent(WCH_WEAPON_REPLACECLIP,MAKEINT64(&clippos,NULL));
			//}
		}
	}
}

//////////////////////////////////
//
//crCaissonObject
//
////////////////////////////////

crCaissonObject::crCaissonObject():
m_effectAttrMask(NoneEffectAttr)
{
}

crCaissonObject::crCaissonObject(const crCaissonObject &caissonObject, const CRCore::crCopyOp& copyop):
CRCore::crGroup(caissonObject,copyop),
m_weaponName(caissonObject.m_weaponName),
m_effectAttrMask(caissonObject.m_effectAttrMask)
//m_collideSounName(caissonObject.m_collideSounName)
{
}

crCaissonObject::~crCaissonObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crCaissonObject()"<< std::endl;
}

void crCaissonObject::setEffectAttrMask(EffectAttrMask attrmask)
{
	m_effectAttrMask = attrmask;
}

void crCaissonObject::setWeaponName(const std::string &weaponName)
{
	m_weaponName = weaponName;
}

const std::string &crCaissonObject::getWeaponName() const
{
	return m_weaponName;
}

//void crCaissonObject::setCollideSoundName(const std::string &soundName)
//{
//	m_collideSounName = soundName;
//}
//
//const std::string &crCaissonObject::getCollideSoundName() const
//{
//	return m_collideSounName;
//}

//////////////////////////////////
//
//crCaissonMatterObject
//
////////////////////////////////
crCaissonMatterObject::crCaissonMatterObject():
m_bulletCount(0),
m_comeForthInterval(0.0f),
m_lastComeFortTime(0.0f),
m_state(0)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crCaissonMatterObject::crCaissonMatterObject(const crCaissonMatterObject& caisson,const crCopyOp& copyop):
crMatterObject(caisson,copyop),
m_bulletCount(caisson.m_bulletCount)
{
}

crCaissonMatterObject::~crCaissonMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crCaissonMatterObject()"<< std::endl;
}

void crCaissonMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_comeForthInterval>0.0f && !getVisiable())
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				if(m_state==0)
				{
					m_lastComeFortTime = referenceTime;
					m_state++;
				}
				else if((referenceTime - m_lastComeFortTime) > m_comeForthInterval)
				{
					setVisiable(true);
					setEnableIntersect(true);
					m_state = 0;
				}
			}
		}
		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

void crCaissonMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

void crCaissonMatterObject::setBulletCount(int bulletCount)
{
	m_bulletCount = bulletCount;
}

int crCaissonMatterObject::getBulletCount()const
{
	return m_bulletCount;
}

void crCaissonMatterObject::setComeForthInterval(float interval)
{
	m_comeForthInterval = interval;
}

float crCaissonMatterObject::getComeForthInterval()const
{
	return m_comeForthInterval;
}

const crCaissonObject *crCaissonMatterObject::getCaissonObject()const
{
	return dynamic_cast<const crCaissonObject *>(m_childArray[0].get());
}

//////////////////////////////////
//
//crHpObject
//
////////////////////////////////

crHpObject::crHpObject():
    m_hpValue(20),
	m_effectAttrMask(NoneEffectAttr)
{
}

crHpObject::crHpObject(const crHpObject &hpObject, const CRCore::crCopyOp& copyop):
CRCore::crGroup(hpObject,copyop),
m_hpValue(hpObject.m_hpValue),
m_effectAttrMask(hpObject.m_effectAttrMask)
//m_collideSounName(hpObject.m_collideSounName)
{
}

crHpObject::~crHpObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crHpObject()"<< std::endl;
}

void crHpObject::setEffectAttrMask(EffectAttrMask attrmask)
{
	m_effectAttrMask = attrmask;
}

void crHpObject::setHpValue(int value)
{
	m_hpValue = value;
}

int crHpObject::getHpValue() const
{
	return m_hpValue;
}

//void crHpObject::setCollideSoundName(const std::string &soundName)
//{
//	m_collideSounName = soundName;
//}
//
//const std::string &crHpObject::getCollideSoundName() const
//{
//	return m_collideSounName;
//}

//////////////////////////////////
//
//crHpMatterObject
//
////////////////////////////////
crHpMatterObject::crHpMatterObject():
m_comeForthInterval(10.0f),
m_lastComeFortTime(0.0f),
m_state(0)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crHpMatterObject::crHpMatterObject(const crHpMatterObject& hp,const crCopyOp& copyop):
crMatterObject(hp,copyop)
{
}

crHpMatterObject::~crHpMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crHpMatterObject()"<< std::endl;
}

void crHpMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_comeForthInterval>0.0f && !getVisiable())
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				if(m_state==0)
				{
					m_lastComeFortTime = referenceTime;
					m_state++;
				}
				else if((referenceTime - m_lastComeFortTime) > m_comeForthInterval)
				{
					setVisiable(true);
					setEnableIntersect(true);
					m_state = 0;
				}
			}
		}
		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

void crHpMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	float radius = m_boundSphere.radius();
	radius *= radius < 0.8f ? 0.67382f : 0.58f;
	m_geomID = dCreateSphere(spaceID,radius);
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

void crHpMatterObject::setComeForthInterval(float interval)
{
	m_comeForthInterval = interval;
}

float crHpMatterObject::getComeForthInterval()const
{
	return m_comeForthInterval;
}

const crHpObject *crHpMatterObject::getHpObject()const
{
	return dynamic_cast<const crHpObject *>(m_childArray[0].get());
}

//////////////////////////////////
//
//crArmorObject
//
////////////////////////////////

crArmorObject::crArmorObject():
  m_armorValue(50),
  m_effectAttrMask(NoneEffectAttr)
{
}

crArmorObject::crArmorObject(const crArmorObject &armorObject, const CRCore::crCopyOp& copyop):
CRCore::crGroup(armorObject,copyop),
m_armorValue(armorObject.m_armorValue),
m_effectAttrMask(armorObject.m_effectAttrMask)
//m_collideSounName(armorObject.m_collideSounName)
{
}

crArmorObject::~crArmorObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crArmorObject()"<< std::endl;
}

void crArmorObject::setEffectAttrMask(EffectAttrMask attrmask)
{
	m_effectAttrMask = attrmask;
}

void crArmorObject::setArmorValue(int value)
{
	m_armorValue = value;
}

int crArmorObject::getArmorValue() const
{
	return m_armorValue;
}

//void crArmorObject::setCollideSoundName(const std::string &soundName)
//{
//	m_collideSounName = soundName;
//}
//
//const std::string &crArmorObject::getCollideSoundName() const
//{
//	return m_collideSounName;
//}

//////////////////////////////////
//
//crArmorMatterObject
//
////////////////////////////////
crArmorMatterObject::crArmorMatterObject():
m_comeForthInterval(20.0f),
m_lastComeFortTime(0.0f),
m_state(0)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crArmorMatterObject::crArmorMatterObject(const crArmorMatterObject& armor,const crCopyOp& copyop):
crMatterObject(armor,copyop)
{
}

crArmorMatterObject::~crArmorMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crArmorMatterObject()"<< std::endl;
}

void crArmorMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_comeForthInterval>0.0f && !getVisiable())
			{
				double referenceTime = crFrameStamp::getInstance()->getReferenceTime();
				if(m_state==0)
				{
					m_lastComeFortTime = referenceTime;
					m_state++;
				}
				else if((referenceTime - m_lastComeFortTime) > m_comeForthInterval)
				{
					setVisiable(true);
					setEnableIntersect(true);
					m_state = 0;
				}
			}
		}
		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

void crArmorMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

void crArmorMatterObject::setComeForthInterval(float interval)
{
	m_comeForthInterval = interval;
}

float crArmorMatterObject::getComeForthInterval()const
{
	return m_comeForthInterval;
}

const crArmorObject *crArmorMatterObject::getArmorObject()const
{
	return dynamic_cast<const crArmorObject *>(m_childArray[0].get());
}