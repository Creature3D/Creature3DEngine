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
#ifndef CRNETAPP_LOGICCLASS_H
#define CRNETAPP_LOGICCLASS_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRCore/crLightSource.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crSoundNode.h>
#include <CRSound/crSound.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRUI/crWidgetExtend.h>
#include <ode/ode.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{
//class crDownloadedUpdatecfgLogic : public CRCore::crLogic
//{//
//public:
//	crDownloadedUpdatecfgLogic();
//	crDownloadedUpdatecfgLogic(const crDownloadedUpdatecfgLogic& handle);
//	LogicClass(CRNetApp, DownloadedUpdatecfg)
//	virtual void operator()(crHandle &handle);                   
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	std::string *m_fileName;
//	short m_netType;
//	unsigned short m_downloadFlg;
//	bool m_needDownload;
//	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
//};
class crRecvSceneInfoLogic : public CRCore::crLogic
{//
public:
	crRecvSceneInfoLogic();
	crRecvSceneInfoLogic(const crRecvSceneInfoLogic& handle);
	LogicClass(CRNetApp, RecvSceneInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
};
//class crDownloadRoleFilesLogic : public CRCore::crLogic
//{//
//public:
//	crDownloadRoleFilesLogic();
//	crDownloadRoleFilesLogic(const crDownloadRoleFilesLogic& handle);
//	LogicClass(CRNetApp, DownloadRoleFiles)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	CRCore::ref_ptr<crRole> m_role;
//	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
//	bool m_needDownload;
//	std::string m_notifyStage;
//	std::string m_notifyTextElement;
//};
class crDownloadItemLogic : public CRCore::crLogic
{//
public:
	crDownloadItemLogic();
	crDownloadItemLogic(const crDownloadItemLogic& handle);
	LogicClass(CRNetApp, DownloadItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<crInstanceItem> m_item;
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
	unsigned short m_downloadFlg;
};
class crNetMousePickLogic : public CRCore::crLogic
{//
public:
	crNetMousePickLogic();
	crNetMousePickLogic(const crNetMousePickLogic& handle);
	LogicClass(CRNetApp, NetMousePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crRole* m_this;
	CREPARAM m_param;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class crDownloadItemChildLogic : public CRCore::crLogic
{//
public:
	crDownloadItemChildLogic();
	crDownloadItemChildLogic(const crDownloadItemChildLogic& handle);
	LogicClass(CRNetApp, DownloadItemChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<crItemChild> m_itemchild;
	CRCore::ref_ptr<crInstanceItem> m_rootitem;
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
};
class crBulletCollideLogic : public CRCore::crLogic
{//Client Method
public:
	crBulletCollideLogic();
	crBulletCollideLogic(const crBulletCollideLogic& handle);
	LogicClass(CRNetApp, BulletCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRPhysics::crBulletMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_collideNode;
	dContact *m_contactInfo;
	CRCore::ref_ptr<CRCore::crHandle> m_collideFxHandle;
};
class crBulletCollideSequenceLogic : public CRCore::crLogic
{//
public:
	crBulletCollideSequenceLogic();
	crBulletCollideSequenceLogic(const crBulletCollideSequenceLogic& handle);
	LogicClass(CRNetApp, BulletCollideSequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
    CRPhysics::crBulletMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_collideNode;
	dContact *m_contactInfo;
	CRCore::ref_ptr<CRCore::crHandle> m_loadAndCloneNodeHandle;
	CRCore::crVector3 m_offsetTrans;
	CRCore::crVector3 m_rotAngles;
	typedef std::pair< std::string,CRCore::ref_ptr<CRCore::crSequence> > FileSeqPair;
	typedef std::map<std::string,FileSeqPair> AttrSeqMap;
	AttrSeqMap m_attrSeqMap;
	CRCore::crNode *m_loadNode;
};
class crBulletCollideSequence2Logic : public CRCore::crLogic
{//
public:
	crBulletCollideSequence2Logic();
	crBulletCollideSequence2Logic(const crBulletCollideSequence2Logic& handle);
	LogicClass(CRNetApp, BulletCollideSequence2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRPhysics::crBulletMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_collideNode;
	dContact *m_contactInfo;
	CRCore::ref_ptr<CRCore::crHandle> m_loadAndCloneNodeHandle;
	CRCore::crVector3 m_offsetTrans;
	CRCore::crVector3 m_scale;
	typedef std::pair< std::string, std::pair< CRCore::ref_ptr<CRCore::crMatrixTransform>,CRCore::ref_ptr<CRCore::crSequence> > > FileMatSeqPair;
	typedef std::map<std::string,FileMatSeqPair> AttrSeqMap;
	AttrSeqMap m_attrSeqMap;
	CRCore::crNode *m_loadNode;
};
class crWeaponFireLightLogic : public CRCore::crNodeLogic
{//
public:
	crWeaponFireLightLogic();
	crWeaponFireLightLogic(const crWeaponFireLightLogic& handle);
	LogicClass(CRNetApp, WeaponFireLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);

	virtual void releaseObjects(CRCore::crState* state);//release ls
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRCore::crVector3 m_gunpoint;
	CRCore::crVector3 m_dir;
	mutable CRCore::ref_ptr<CRCore::crLightSource> m_ls;
	CRCore::ref_ptr<CRCore::crHandle> m_createLightHandle;
    float m_duration;//持续时间
};
class crBulletFlyParticleLogic : public CRCore::crNodeLogic
{//
public:
	crBulletFlyParticleLogic();
	crBulletFlyParticleLogic(const crBulletFlyParticleLogic& handle);
	LogicClass(CRNetApp, BulletFlyParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);

	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRPhysics::crBulletMatterObject* m_this;
	float m_particleFlyOffset;
	std::string m_filename;
	mutable CRCore::ref_ptr<CRCore::crMatrixTransform> m_particleEffectGroup;
	CRCore::ref_ptr<CRCore::crHandle> m_loadParticleHandle;
};
//class crNpcEnmityTestLogic: public CRCore::crLogic
//{//SceneServer Method
//public:
//	crNpcEnmityTestLogic();
//	crNpcEnmityTestLogic(const crNpcEnmityTestLogic& handle);
//	LogicClass(CRNetApp, NpcEnmityTest)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	crInstanceItem* m_this;
//	crInstanceItem* m_item;
//	float m_dist;
//	CRCore::ref_ptr<CRCore::crHandle> m_isInPatrolHandle;//检查是否在巡逻范围内
//	bool m_isInPatrol;
//	CRCore::ref_ptr<CRCore::crHandle> m_calcEnmityHandle;
//	float m_enmity;
//};
class crNpcAI1Logic: public CRCore::crLogic
{//SceneServer Method
public:
	crNpcAI1Logic();
	crNpcAI1Logic(const crNpcAI1Logic& handle);
	LogicClass(CRNetApp, NpcAI1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crInstanceItem* m_this;
	float m_dt;
	CRCore::ref_ptr<CRCore::crHandle> m_getEnemy1Handle;
	CRCore::ref_ptr<CRCore::crHandle> m_selectSkillHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_useItemAndSendHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_patrolHandle;
	_crInt64 m_enmityid;
	bool m_isAboutToUseItem;
	float m_aiInterval;
	float m_aitime;
	unsigned short m_useResult;
};
class crPlayerAI1Logic: public CRCore::crLogic
{//GameClient Method
public:
	crPlayerAI1Logic();
	crPlayerAI1Logic(const crPlayerAI1Logic& handle);
	LogicClass(CRNetApp, PlayerAI1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_time;
	float m_interval;
	float m_autoFightTime;
	float m_autoFightInterval;
	float m_selectTargetRange;
	unsigned short m_attackResult;
	short m_attackFailed;
	short m_attackFailedRange;
	CRCore::crVector2 m_autoFightIntervalVec;
	CRCore::ref_ptr<CRCore::crHandle> m_useItemAndSendHandle;
};
class crExtraEffectLogic : public CRCore::crNodeLogic
{//
public:
	crExtraEffectLogic();
	crExtraEffectLogic(const crExtraEffectLogic& handle);
	LogicClass(CRNetApp, ExtraEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_bot;
	CRCore::ref_ptr<CRCore::crHandle> m_playSceneFx;
	bool m_init;
	unsigned int m_gs;
};
class crVolumeEffectLogic : public CRCore::crNodeLogic
{//
public:
	crVolumeEffectLogic();
	crVolumeEffectLogic(const crVolumeEffectLogic& handle);
	LogicClass(CRNetApp, VolumeEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRPhysics::crVolumeNode* m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crVector3 m_positionOffset;
};
//class crHitTargetEffectLogic : public CRCore::crNodeLogic
//{//
//public:
//	crHitTargetEffectLogic();
//	crHitTargetEffectLogic(const crHitTargetEffectLogic& handle);
//	LogicClass(CRNetApp, HitTargetEffect)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
//	CRCore::ref_ptr<CRCore::crHandle> m_playSceneFx;
//};
class crGameTaskCheckLogic : public CRCore::crLogic
{
public:
	crGameTaskCheckLogic();
	crGameTaskCheckLogic(const crGameTaskCheckLogic& handle);
	LogicClass(CRNetApp, GameTaskCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crInstanceItem* m_this;
	int m_taskid;
	CRCore::ref_ptr<CRCore::crHandle> m_referTaskHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_showTalkStage0;//未接任务
	CRCore::ref_ptr<CRCore::crHandle> m_showTalkStage1;//未完成任务
	CRCore::ref_ptr<CRCore::crHandle> m_showTalkStage2;//已完成任务
};
class CRNETAPP_EXPORT crWaitNetReturnStreamLogic : public CRCore::crMethod
{//GameClient Method 
public:
	crWaitNetReturnStreamLogic();
	crWaitNetReturnStreamLogic(const crWaitNetReturnStreamLogic& handle);
	LogicClass(CRNetApp, WaitNetReturnStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);

	static void getLock();
	static void netReturn(CRCore::crStreamBuf *stream);
	static void simpleWait(int ms);
protected:
	static CRCore::crCondition s_condition;
	static CRCore::crMutex s_mutex;
	static CRCore::ref_ptr<CRCore::crStreamBuf> s_netReturnStream;
	int m_waitTime;
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
	static CRCore::crMutex s_protectMutex;
	static int s_lostcount;
};
class crUseThingParticleLogic : public CRCore::crNodeLogic
{//
public:
	crUseThingParticleLogic();
	crUseThingParticleLogic(const crUseThingParticleLogic& handle);
	LogicClass(CRNetApp, UseThingParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);

	virtual void releaseObjects(CRCore::crState* state);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_user;
	std::string m_filename;
	mutable CRCore::ref_ptr<CRCore::crMatrixTransform> m_particleEffectGroup;
	CRCore::ref_ptr<CRCore::crHandle> m_loadParticleHandle;
};

class crSlotTimerLogic : public CRCore::crLogic
{//
public:
	crSlotTimerLogic();
	crSlotTimerLogic(const crSlotTimerLogic& handle);
	LogicClass(CRNetApp, SlotTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt32 m_msg;
	unsigned short m_id;
	float m_interval;
	unsigned short m_inputid;
	unsigned short m_inputInterval;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
	CRCore::ref_ptr<CRCore::crHandle> m_beginEvent;//开始事件
	CRCore::ref_ptr<CRCore::crHandle> m_persistEvent;//持续事件
	CRCore::ref_ptr<CRCore::crHandle> m_endEvent;//结束事件
};

class crSlotProgressLogic : public CRCore::crLogic
{//
public:
	crSlotProgressLogic();
	crSlotProgressLogic(const crSlotProgressLogic& handle);
	LogicClass(CRNetApp, SlotProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	static void advance();
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt32 m_msg;
	unsigned short m_id;
	unsigned short m_inputid;
	static char s_advance;//0，没有激活，1激活，2推进
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
	CRCore::ref_ptr<CRCore::crHandle> m_beginEvent;//开始事件
	CRCore::ref_ptr<CRCore::crHandle> m_persistEvent;//持续事件
	CRCore::ref_ptr<CRCore::crHandle> m_endEvent;//结束事件
};

class crCreateSoundLogic : public CRCore::crLogic
{//
public:
	crCreateSoundLogic();
	crCreateSoundLogic(const crCreateSoundLogic& handle);
	LogicClass(CRNetApp, CreateSound)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRSound::crSound> m_sound;
	CRCore::ref_ptr<CRCore::crHandle> m_loadSoundHandle;
	//bool m_createSoundNode;
	std::string m_filename; 
	bool m_ambient;
	float m_gain;
	bool m_play;
	bool m_looping;
	float m_minDistance;
	float m_maxDistance;
	float m_pitch;
	bool m_inputPos;
	CRCore::crVector3 m_position;
	CRCore::crVector3 m_dir;
};

class crPlaySceneFxLogic : public CRCore::crNodeLogic
{//
public:
	crPlaySceneFxLogic();
	crPlaySceneFxLogic(const crPlaySceneFxLogic& handle);
	LogicClass(CRNetApp, PlaySceneFx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crNode* m_this;
	CRCore::ref_ptr<crInstanceItem> m_ithis;
	CRCore::ref_ptr<CRCore::crHandle> m_loadAndCloneNodeHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_placeLoadedNodeHandle;
	CRCore::ref_ptr<CRCore::crNode> m_loadNode;
	std::string m_fileName;
	float m_validTime;
	float m_visiableTime;
	CRCore::crVector3 m_position;
	CRCore::crVector3 m_dir;
	CRCore::ref_ptr<CRCore::crNode> m_attachToNode;
	std::string m_attachBoneName;//BBOX_CENTER BBOX_TOP BBOX_BOTTOM
	CRCore::crVector3 m_attachOffset;
	CRCore::crVector3 m_attachScale;
	CRCore::crVector3 m_attachDir;
	float m_attachRotAngle;
	bool m_attachToThis;
	bool m_ithisValid;
	std::string m_createNodeName;
	bool m_resetFX;//如果为true则m_actFx的VisiableOrUnload属性为Visiable否则为Unload
	unsigned short m_actFx;//需要有crInstanceItem，当item在该act状态下特效持续，当item在该act特效结束
};
class crAttachCallbackMethod : public CRCore::crMethod
{//给模型增加一个UpdateCallback/CullCallback实现某种特效
public:
	crAttachCallbackMethod();
	crAttachCallbackMethod(const crAttachCallbackMethod& handle);
	MethodClass(CRNetApp, AttachCallback)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crNode* m_this;
	crInstanceItem* m_ithis;
	CRCore::ref_ptr<CRCore::crNode> m_attachToNode;
	bool m_ithisValid;
	int m_callbacktype;//0UpdateCallback 1CullCallback
	std::string m_callbackName;
	typedef std::vector<std::string> ParamVec;
	ParamVec m_paramVec;
	CRCore::ref_ptr<CRCore::crNodeCallback> m_callback;
};
class crPlaceLoadedNode2Logic : public CRCore::crLogic
{//
public:
	crPlaceLoadedNode2Logic();
	crPlaceLoadedNode2Logic(const crPlaceLoadedNode2Logic& handle);
	LogicClass(CRNetApp, PlaceLoadedNode2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
	CRCore::crVector3 m_position;
	CRCore::crVector3 m_dir;
private:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
class crSelectRolePickLogic : public CRCore::crLogic
{//
public:
	crSelectRolePickLogic();
	crSelectRolePickLogic(const crSelectRolePickLogic& handle);
	LogicClass(CRNetApp, SelectRolePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crRole* m_this;
	CREPARAM m_param;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class crCreateRoleInitLogic : public CRCore::crLogic
{//
public:
	crCreateRoleInitLogic();
	crCreateRoleInitLogic(const crCreateRoleInitLogic& handle);
	LogicClass(CRNetApp, CreateRoleInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	std::string m_roletabfile;
	std::string m_attachToNodeName;
	std::string m_attachBoneName;
	CRCore::crVector3 m_attachOffset;
	CRCore::crVector3 m_attachScale;
	float m_attachRotAngle;
	CRCore::ref_ptr<CRCore::crHandle> m_addModelHandle;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
};
class crCreateRoleSelectLogic : public CRCore::crLogic
{//
public:
	crCreateRoleSelectLogic();
	crCreateRoleSelectLogic(const crCreateRoleSelectLogic& handle);
	LogicClass(CRNetApp, CreateRoleSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crWidgetNode* m_this;
	bool m_left;//left or right
	std::string m_headImageWidget;
	CRCore::ref_ptr<CRCore::crHandle> m_addModelHandle;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
};
class crItemEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crItemEffectLogic();
	crItemEffectLogic(const crItemEffectLogic& handle);
	LogicClass(CRNetApp, ItemEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crVector3 m_positionOffset;
	bool m_attachToNode;
};
class crParamItemEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crParamItemEffectLogic();
	crParamItemEffectLogic(const crParamItemEffectLogic& handle);
	LogicClass(CRNetApp, ParamItemEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::ref_ptr<crInstanceItem>m_item;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crVector3 m_positionOffset;
	bool m_attachToNode;
};
class crWeaponFireEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crWeaponFireEffectLogic();
	crWeaponFireEffectLogic(const crWeaponFireEffectLogic& handle);
	LogicClass(CRNetApp, WeaponFireEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crVector3 m_offset;
	CRCore::crVector3 m_scale;
	float m_rotAngle;
	std::string m_bonename;
};
class crBotEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crBotEffectLogic();
	crBotEffectLogic(const crBotEffectLogic& handle);
	LogicClass(CRNetApp, BotEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform* m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crVector3 m_positionOffset;
	bool m_attachToNode;
};
}
#endif