/**********************************************************************
*
*	文件:	   appLogic.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPLOGIC_H
#define JXJ_APPLOGIC_H
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crRange.h>
#include <CRCore/crVector2.h>
#include <CREncapsulation/crLoader.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
namespace JXJ{
class crJXJMousePickLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJMousePickLogic();
	crJXJMousePickLogic(const crJXJMousePickLogic& handle);
	LogicClass(JXJ, JXJMousePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class crJXJBattleLMousePickLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJBattleLMousePickLogic();
	crJXJBattleLMousePickLogic(const crJXJBattleLMousePickLogic& handle);
	LogicClass(JXJ, JXJBattleLMousePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx_Signal;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
	CRCore::ref_ptr<CRCore::crHandle> m_useItemAndSendHandle;
};
class crJXJBattleRMousePickLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJBattleRMousePickLogic();
	crJXJBattleRMousePickLogic(const crJXJBattleRMousePickLogic& handle);
	LogicClass(JXJ, JXJBattleRMousePick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class crJXJUseSkillDecalUpdateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJUseSkillDecalUpdateLogic();
	crJXJUseSkillDecalUpdateLogic(const crJXJUseSkillDecalUpdateLogic& handle);
	LogicClass(JXJ, JXJUseSkillDecalUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_decalNode;
	//double m_lastupdtime;
	//float m_delta;
};
class crJXJUseSkillRangeDecalCreateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJUseSkillRangeDecalCreateLogic();
	crJXJUseSkillRangeDecalCreateLogic(const crJXJUseSkillRangeDecalCreateLogic& handle);
	LogicClass(JXJ, JXJUseSkillRangeDecalCreate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_decalNode;
	float m_zoffset;
};
class crJXJBattleMouseMoveLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJBattleMouseMoveLogic();
	crJXJBattleMouseMoveLogic(const crJXJBattleMouseMoveLogic& handle);
	LogicClass(JXJ, JXJBattleMouseMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
	int m_enemyCursor;
	int m_friendCursor;
	int m_ismeCursor;
	int m_canbepickCursor;
	int m_controlableCursor;
	int m_defaultCursor;
	int m_canWalkCursor;
};
class crJXJMouseMoveLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJMouseMoveLogic();
	crJXJMouseMoveLogic(const crJXJMouseMoveLogic& handle);
	LogicClass(JXJ, JXJMouseMove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
};
class crJXJNpcAILogic: public CRCore::crLogic
{//SceneServer Method
public:
	crJXJNpcAILogic();
	crJXJNpcAILogic(const crJXJNpcAILogic& handle);
	LogicClass(JXJ, JXJNpcAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_dt;
	CRCore::ref_ptr<CRCore::crHandle> m_useItemAndSendHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_selectSkillHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_patrolHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_closeTargetHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_npcRevert;
	_crInt64 m_enmityid;
	bool m_isAboutToUseItem;
	float m_aiInterval;
	float m_aitime;
	unsigned short m_useResult;
	std::vector< CRCore::ref_ptr<CRNetApp::crInstanceItem> >m_attackTargetVec;
	int m_attackIndex;
	int m_attackTargetCount;
	float m_aiTargetInterval;//
	float m_aiTargetTimer;
};
class crJXJPlayerAILogic: public CRCore::crLogic
{//GameClient Method
public:
	crJXJPlayerAILogic();
	crJXJPlayerAILogic(const crJXJPlayerAILogic& handle);
	LogicClass(JXJ, JXJPlayerAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_dt;
	float m_time;
	float m_interval;
	//float m_autoFightTime;
	//float m_autoFightInterval;
	unsigned short m_useItemResult;
	//short m_attackFailed;
	//short m_attackFailedRange;
	//CRCore::crVector2 m_autoFightIntervalVec;
	CRCore::ref_ptr<CRCore::crHandle> m_useItemAndSendHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_patrolHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_closeTargetHandle;
	unsigned char m_itemstate_rec;
	CRCore::crVector3 m_attackToPos;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_aiTargetItem;
	float m_aiTargetInterval;//
	float m_aiTargetTimer;
	std::string m_progressCanvas[3]; //吟唱进度条
};

class crJXJDownloadFirstSceneFileLogic : public CRCore::crLogic
{//GameClient Method
public:
	crJXJDownloadFirstSceneFileLogic();
	crJXJDownloadFirstSceneFileLogic(const crJXJDownloadFirstSceneFileLogic& handle);
	LogicClass(JXJ, JXJDownloadFirstSceneFile)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
};
//class crJXJScriptDownloadTaskLogic : public CRCore::crLogic
//{//GameClient Method,脚本触发的下载任务
//public:
//	crJXJScriptDownloadTaskLogic();
//	crJXJScriptDownloadTaskLogic(const crJXJScriptDownloadTaskLogic& handle);
//	LogicClass(JXJ, JXJScriptDownloadTask)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	bool isModelFile(const std::string& extension) const;
//	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
//	bool m_needDownload;
//	typedef std::vector<std::string> StrVec;
//	StrVec m_fileList;
//};
class crJXJPreLoadDownloadTaskLogic : public CRCore::crLogic
{//GameClient Method,文件加载前的下载任务
public:
	crJXJPreLoadDownloadTaskLogic();
	crJXJPreLoadDownloadTaskLogic(const crJXJPreLoadDownloadTaskLogic& handle);
	LogicClass(JXJ, JXJPreLoadDownloadTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	bool isModelFile(const std::string& extension) const;
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
	std::string m_file;
};
class crJXJBattleMouseMoveNewLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJBattleMouseMoveNewLogic();
	crJXJBattleMouseMoveNewLogic(const crJXJBattleMouseMoveNewLogic& handle);
	LogicClass(JXJ, JXJBattleMouseMoveNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
	int m_mouseAttack;
	int m_mouseAttackSelf;
	int m_mouseAttackEnemy;
};
class crJXJBattleMouseMoveTargetSkillLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJBattleMouseMoveTargetSkillLogic();
	crJXJBattleMouseMoveTargetSkillLogic(const crJXJBattleMouseMoveTargetSkillLogic& handle);
	LogicClass(JXJ, JXJBattleMouseMoveTargetSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	unsigned char m_targetType;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
	int m_cannotmouse;
};
class crJXJBuildingItemEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJBuildingItemEffectLogic();
	crJXJBuildingItemEffectLogic(const crJXJBuildingItemEffectLogic& handle);
	LogicClass(JXJ, JXJBuildingItemEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_effectNode;
	CRCore::crVector3 m_positionOffset;
	bool m_visiable;
};
class crJXJMouseOnBuildingItemEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJMouseOnBuildingItemEffectLogic();
	crJXJMouseOnBuildingItemEffectLogic(const crJXJMouseOnBuildingItemEffectLogic& handle);
	LogicClass(JXJ, JXJMouseOnBuildingItemEffect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_effectNode;
	CRCore::crVector3 m_positionOffset;
	bool m_visiable;
};
class crJXJShuiJingRangeDecalCreateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJShuiJingRangeDecalCreateLogic();
	crJXJShuiJingRangeDecalCreateLogic(const crJXJShuiJingRangeDecalCreateLogic& handle);
	LogicClass(JXJ, JXJShuiJingRangeDecalCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_decalNode;
	float m_zoffset;
};

class crJXJJianTaRangeDecalCreateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJJianTaRangeDecalCreateLogic();
	crJXJJianTaRangeDecalCreateLogic(const crJXJJianTaRangeDecalCreateLogic& handle);
	LogicClass(JXJ, JXJJianTaRangeDecalCreate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_decalNode;
	float m_zoffset;
};

class crJXJDownloadUISceneFileLogic : public CRCore::crLogic
{//GameClient Method
public:
	crJXJDownloadUISceneFileLogic();
	crJXJDownloadUISceneFileLogic(const crJXJDownloadUISceneFileLogic& handle);
	LogicClass(JXJ, JXJDownloadUISceneFile)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownload;
	int m_downloadFlg;
	std::string m_file;
	//bool m_onlydownload;//默认false表示如果已经下载了，立即加载，如果没有下载，则下载后再加载|true表示如果已经下载了，不做加载，如果没有下载，则下载后再加载
};
class crJXJNextSceneDownloadLogic : public CRCore::crLogic
{//GameClient Method预下载下一个场景的资源
public:
	crJXJNextSceneDownloadLogic();
	crJXJNextSceneDownloadLogic(const crJXJNextSceneDownloadLogic& handle);
	LogicClass(JXJ, JXJNextSceneDownload)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_checkAndDownloadFileMathod;
	bool m_needDownloadUI;
	bool m_needDownload;
	typedef std::vector<std::string> StrVec;
	StrVec m_extraDownloadFiles;//额外需要下载的文件
};
class crJXJExtraShootEffectLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJExtraShootEffectLogic();
	crJXJExtraShootEffectLogic(const crJXJExtraShootEffectLogic & handle);
	LogicClass(JXJ, JXJExtraShootEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	CRCore::ref_ptr<CRCore::crHandle> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNode> m_fxNode;
	std::string m_closeToTargetCallback;
	std::string m_param;
	CRCore::ref_ptr<CRCore::crNodeCallback> m_callback;
};

class crJXJCampAddArmyEffectLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJCampAddArmyEffectLogic();
	crJXJCampAddArmyEffectLogic(const crJXJCampAddArmyEffectLogic& handle);
	LogicClass(JXJ, JXJCampAddArmyEffect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::crNode *m_effectNode;
	CRCore::crVector3 m_positionOffset;
	bool m_visiable;
};
class crJXJBulletCollideFxLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJBulletCollideFxLogic();
	crJXJBulletCollideFxLogic(const crJXJBulletCollideFxLogic& handle);
	LogicClass(JXJ, JXJBulletCollideFx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	typedef std::vector< CRCore::ref_ptr<CRCore::crNodeLogic> >LogicArray;
	LogicArray m_playSceneFxArray;
	CRCore::crVector3 m_collidePos;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_hititem;
};
//class crJXJExtraCorpseCountLogic : public CRCore::crLogic
//{//SceneServer Method, GameClient 周围尸体数量
//public:
//	crJXJExtraCorpseCountLogic();
//	crJXJExtraCorpseCountLogic(const crJXJExtraCorpseCountLogic& handle);
//	LogicClass(JXJ, JXJExtraCorpseCount)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
//	float *m_dt;
//	float m_duration;
//	float m_range;//范围,米
//	float m_haloInterval;//光环每隔多少秒作用一次
//
//	bool m_start;
//	float m_timer;
//	float m_dtrec;
//	CRCore::HandleVec m_handles;
//	short m_corpseCount;
//};
class crJXJExtraOverlapHaloLogic : public CRCore::crLogic
{//SceneServer Method, GameClient 叠加光环
public:
	crJXJExtraOverlapHaloLogic();
	crJXJExtraOverlapHaloLogic(const crJXJExtraOverlapHaloLogic& handle);
	LogicClass(JXJ, JXJExtraOverlapHalo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	float m_haloInterval;//光环每隔多少秒作用一次

	bool m_start;
	float m_timer;
	float m_dtrec;
	CRCore::ref_ptr<CRCore::crHandle> m_overlapCountHandle;
	CRCore::HandleVec m_handles;
	short m_overlapCount;
};
}
#endif