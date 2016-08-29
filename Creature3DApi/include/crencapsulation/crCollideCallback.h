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
#ifndef CRENCAPSULATION_CRCOLLIDECALLBACK_H
#define CRENCAPSULATION_CRCOLLIDECALLBACK_H 1

#include <CREncapsulation\crExport.h>
#include <CRCore/crCollideCallback.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <CRCore/crSequence.h>
#include <CRCore/crMultiSwitch.h>
//#include <CRAL/crSoundManager.h>
//namespace CRAL{
//	class crSoundState;
//}
namespace CREncapsulation {

class CRENCAPSULATION_EXPORT crCommonMatterObjectCollideCallback : public CRCore::crCollideCallback 
{
public :
	crCommonMatterObjectCollideCallback(){}
	crCommonMatterObjectCollideCallback(const crCommonMatterObjectCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation, CommonMatterObjectCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2,const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
protected:
	virtual ~crCommonMatterObjectCollideCallback() {}
};

class CRENCAPSULATION_EXPORT crBulletCollideCallback : public CRCore::crCollideCallback 
{
public :
	crBulletCollideCallback():
	  m_particleEffectNode(NULL),
	  m_sequenceNode(NULL)/*,
	  m_sound(NULL)*/{}
	crBulletCollideCallback(const crBulletCollideCallback&copy,const CRCore::crCopyOp&copyop):
    CRCore::crCollideCallback(copy,copyop) {}
    
	META_EventCallback(CREncapsulation,BulletCollide)
    
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crBulletCollideCallback *getInstance();
protected:
    virtual ~crBulletCollideCallback() {}
	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRSound::crSound* m_sound;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_cautionSW;
};
///////////////////////////
//crWeaponCollideCallback
/////////////////////////////
class CRENCAPSULATION_EXPORT crWeaponCollideCallback : public CRCore::crCollideCallback 
{
public :
	crWeaponCollideCallback():
	   m_particleEffectNode(NULL),
	   m_sequenceNode(NULL)/*,
	   m_soundState(NULL)*/ {}
	crWeaponCollideCallback(const crWeaponCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,WeaponCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crWeaponCollideCallback *getInstance();
protected:
	virtual ~crWeaponCollideCallback() {}
	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;
};
///////////////////////////
//crCaissonCollideCallback
/////////////////////////////
class CRENCAPSULATION_EXPORT crCaissonCollideCallback : public CRCore::crCollideCallback 
{
public :
	crCaissonCollideCallback():
	    m_particleEffectNode(NULL),
	    m_sequenceNode(NULL)/*,
        m_soundState(NULL)*/ {}
	crCaissonCollideCallback(const crCaissonCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,CaissonCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crCaissonCollideCallback *getInstance();
protected:
	virtual ~crCaissonCollideCallback() {}
	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;
};
///////////////////////////
//crHpCollideCallback
/////////////////////////////
class CRENCAPSULATION_EXPORT crHpCollideCallback : public CRCore::crCollideCallback 
{
public :
	crHpCollideCallback():
	   m_particleEffectNode(NULL),
	   m_sequenceNode(NULL)/*,
	   m_soundState(NULL)*/ {}
	crHpCollideCallback(const crHpCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,HpCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crHpCollideCallback *getInstance();
protected:
	virtual ~crHpCollideCallback() {}
	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;
};
///////////////////////////
//crArmorCollideCallback
/////////////////////////////
class CRENCAPSULATION_EXPORT crArmorCollideCallback : public CRCore::crCollideCallback 
{
public :
	crArmorCollideCallback():
	   m_particleEffectNode(NULL),
	   m_sequenceNode(NULL)/*,
	   m_soundState(NULL)*/ {}
	crArmorCollideCallback(const crArmorCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,ArmorCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crArmorCollideCallback *getInstance();
protected:
	virtual ~crArmorCollideCallback() {}
	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;
};

//class CRENCAPSULATION_EXPORT crViewObjectCollideCallback : public CRCore::crCollideCallback 
//{
//public :
//	crViewObjectCollideCallback(){}
//	crViewObjectCollideCallback(const crViewObjectCollideCallback&copy,const CRCore::crCopyOp&copyop):
//	CRCore::crCollideCallback(copy,copyop) {}
//	META_EventCallback(CREncapsulation,ViewObjectCollide)
//	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2,const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
//	//static crViewObjectCollideCallback *getInstance();
//protected:
//	virtual ~crViewObjectCollideCallback() {}
//	//CRCore::ref_ptr<CRCore::crDrawable> m_killInfo;
//};

//class CRENCAPSULATION_EXPORT crAircraftCollideCallback : public CRCore::crCollideCallback 
//{
//public :
//	crAircraftCollideCallback(){}
//	crAircraftCollideCallback(const crAircraftCollideCallback&copy,const CRCore::crCopyOp&copyop):
//	CRCore::crCollideCallback(copy,copyop) {}
//	META_EventCallback(CREncapsulation,AircraftCollide)
//	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2,const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
//	//static crAircraftCollideCallback *getInstance();
//protected:
//	virtual ~crAircraftCollideCallback() {}
//	//CRCore::ref_ptr<CRCore::crDrawable> m_killInfo;
//	CRParticle::crParticleEffect* m_particleEffectNode;
//	CRCore::crSequence* m_sequenceNode;
//	CRAL::crSoundState* m_soundState;
//};

//class CRENCAPSULATION_EXPORT crBodyCaseCollideCallback : public CRCore::crCollideCallback 
//{
//public :
//	crBodyCaseCollideCallback(){}
//	crBodyCaseCollideCallback(const crBodyCaseCollideCallback&copy,const CRCore::crCopyOp&copyop):
//	CRCore::crCollideCallback(copy,copyop) {}
//	META_EventCallback(CREncapsulation,crBodyCaseCollideCallback)
//		virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2,const CRCore::crVector3f& collidePos);
//	static crBodyCaseCollideCallback *getInstance();
//protected:
//	virtual ~crBodyCaseCollideCallback() {}
//	//CRCore::ref_ptr<CRCore::crDrawable> m_killInfo;
//	CRParticle::crParticleEffect* m_particleEffectNode;
//	CRCore::crSequence* m_sequenceNode;
//	CRAL::crSoundState* m_soundState;
//};

class CRENCAPSULATION_EXPORT crGroundBoxCollideCallback : public CRCore::crCollideCallback 
{
public :
	crGroundBoxCollideCallback(){}
	crGroundBoxCollideCallback(const crGroundBoxCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,GroundBoxCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crGroundBoxCollideCallback *getInstance();
protected:
	virtual ~crGroundBoxCollideCallback() {}
};

class CRENCAPSULATION_EXPORT crMouseManipularCollideCallback : public CRCore::crCollideCallback 
{
public :
	crMouseManipularCollideCallback(){}
	crMouseManipularCollideCallback(const crMouseManipularCollideCallback&copy,const CRCore::crCopyOp&copyop):
	CRCore::crCollideCallback(copy,copyop) {}
	META_EventCallback(CREncapsulation,MouseManipularCollide)
	virtual void operator()(CRCore::crNode* node1, CRCore::crNode* node2, void *o1, void *o2, const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal);
	//static crMouseManipularCollideCallback *getInstance();
protected:
	virtual ~crMouseManipularCollideCallback() {}
};
}

#endif

