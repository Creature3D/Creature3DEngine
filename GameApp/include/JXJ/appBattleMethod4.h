
/**********************************************************************
*
*	文件:	   appBattleMethod4.h
*
*	描述:	   
*
*	作者:	   刘超
*					
*
**********************************************************************/
#ifndef JXJ_APPBATTLEMETHOD4_H
#define JXJ_APPBATTLEMETHOD4_H
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crRange.h>
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
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <JXJ/appDefine.h>
namespace JXJ{


class crJXJRoomElapseTimeIsGreaterMethod : public CRCore::crMethod
{//SceneServer Method 进入room的世界是否大于某个值
public:
	crJXJRoomElapseTimeIsGreaterMethod ();
	crJXJRoomElapseTimeIsGreaterMethod (const crJXJRoomElapseTimeIsGreaterMethod & handle);
	MethodClass(JXJ, JXJRoomElapseTimeIsGreater)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	float m_scriptTime;
};

class crJXJAlterRelivedNpcAttrFromTableMethod : public CRCore::crMethod
{//SceneServer Method 改变Npc 血，攻，防，armycount
public:
	typedef std::vector<float> AttrFactorVec;
	crJXJAlterRelivedNpcAttrFromTableMethod ();
	crJXJAlterRelivedNpcAttrFromTableMethod (const crJXJAlterRelivedNpcAttrFromTableMethod & handle);
	MethodClass(JXJ, JXJAlterRelivedNpcAttrFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableid;
	AttrFactorVec m_attrFactorVec;
};

class crJXJCheckItemsLivedFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckItemsLivedFromTabMethod ();
	crJXJCheckItemsLivedFromTabMethod (const crJXJCheckItemsLivedFromTabMethod & handle);
	MethodClass(JXJ, JXJCheckItemsLivedFromTab)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_id;
	bool m_orOrAnd;		// 0 表示或，1表示与
};

class crJXJNPCCloseToPlayerFromTableMethod : public CRCore::crMethod
{//SceneServer Method npc是否接近player
public:
	crJXJNPCCloseToPlayerFromTableMethod ();
	crJXJNPCCloseToPlayerFromTableMethod (const crJXJNPCCloseToPlayerFromTableMethod & handle);
	MethodClass(JXJ, JXJNPCCloseToPlayerFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	float m_distance;
};

class crJXJAlterNPCRTHPFromTableMethod : public CRCore::crMethod
{//SceneServer Method 直接改变npc的血量
public:
	crJXJAlterNPCRTHPFromTableMethod ();
	crJXJAlterNPCRTHPFromTableMethod (const crJXJAlterNPCRTHPFromTableMethod & handle);
	MethodClass(JXJ, JXJAlterNPCRTHPFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	float m_factor;	// 改变的系数
};

class crJXJRoomProgressIsBelowValMethod : public CRCore::crMethod
{//SceneServer Method		// 判断当前是处在以下进度中
public:
	crJXJRoomProgressIsBelowValMethod ();
	crJXJRoomProgressIsBelowValMethod (const crJXJRoomProgressIsBelowValMethod & handle);
	MethodClass(JXJ, JXJRoomProgressIsBelowVal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	typedef std::vector<short> ProgressVec;
protected:
	CRNetApp::crRoom *m_this;
	ProgressVec m_progressVec;
};

class crJXJNPCCloseToNPCFromTableMethod : public CRCore::crMethod
{//SceneServer Method npc是否接近npc
public:
	crJXJNPCCloseToNPCFromTableMethod ();
	crJXJNPCCloseToNPCFromTableMethod (const crJXJNPCCloseToNPCFromTableMethod & handle);
	MethodClass(JXJ, JXJNPCCloseToNPCFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID1;
	int m_tableID2;
	float m_distance;
};

class crJXJAlterPlayerRTHPMethod : public CRCore::crMethod
{//SceneServer Method  让玩家直接掉血 比例
public:
	crJXJAlterPlayerRTHPMethod ();
	crJXJAlterPlayerRTHPMethod (const crJXJAlterPlayerRTHPMethod & handle);
	MethodClass(JXJ, JXJAlterPlayerRTHP)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	float m_factor;
};


class crJXJNPCCloseToPointFromTableMethod : public CRCore::crMethod
{//SceneServer Method npc是否接近 某一点
public:
	crJXJNPCCloseToPointFromTableMethod ();
	crJXJNPCCloseToPointFromTableMethod (const crJXJNPCCloseToPointFromTableMethod & handle);
	MethodClass(JXJ, JXJNPCCloseToPointFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	CRCore::crVector2f m_point;
	float m_distance;
};

class crJXJPlayerCloseToPointMethod : public CRCore::crMethod
{//SceneServer Method 玩家是否接近某一点
public:
	crJXJPlayerCloseToPointMethod ();
	crJXJPlayerCloseToPointMethod (const crJXJPlayerCloseToPointMethod & handle);
	MethodClass(JXJ, JXJPlayerCloseToPoint)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	CRCore::crVector2f m_point;
	float m_distance;
};

class crJXJNPCSurvivalNumIsNotLessMethod : public CRCore::crMethod
{//SceneServer Method 判断NPC是否 有指定个数存活
public:
	crJXJNPCSurvivalNumIsNotLessMethod ();
	crJXJNPCSurvivalNumIsNotLessMethod (const crJXJNPCSurvivalNumIsNotLessMethod & handle);
	MethodClass(JXJ, JXJNPCSurvivalNumIsNotLess)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	unsigned char m_num;
};

class crJXJExtraGainMpPerAttackMethod : public CRCore::crMethod
{//SceneServer & GameClient Method  增加每次攻击获得的士气
public:
	crJXJExtraGainMpPerAttackMethod ();
	crJXJExtraGainMpPerAttackMethod (const crJXJExtraGainMpPerAttackMethod & handle);
	MethodClass(JXJ, JXJExtraGainMpPerAttack)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

class crJXJExtraGainMpPerTimerMethod: public CRCore::crMethod
{//SceneServer & GameClient Method		每一段时间增加士气值
public:
	crJXJExtraGainMpPerTimerMethod();
	crJXJExtraGainMpPerTimerMethod(const crJXJExtraGainMpPerTimerMethod& handle);
	MethodClass(JXJ, JXJExtraGainMpPerTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	float m_extraValue;
	//short m_addValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
	float m_interval;
};

class crJXJExtraReduceMpPerTimerAndRemoveExtraMethod: public CRCore::crMethod
{//SceneServer & GameClient Method		每一段时间减少士气值 不足时删除这个buf
public:
	crJXJExtraReduceMpPerTimerAndRemoveExtraMethod();
	crJXJExtraReduceMpPerTimerAndRemoveExtraMethod(const crJXJExtraReduceMpPerTimerAndRemoveExtraMethod& handle);
	MethodClass(JXJ, JXJExtraReduceMpPerTimerAndRemoveExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	float m_extraValue;
	//short m_addValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
	float m_interval;
	bool *m_output;
};


class crJXJExtraReduceMpPerSkillDamageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method  减少每次技能释放消耗的士气
public:
	crJXJExtraReduceMpPerSkillDamageMethod ();
	crJXJExtraReduceMpPerSkillDamageMethod (const crJXJExtraReduceMpPerSkillDamageMethod & handle);
	MethodClass(JXJ, JXJExtraReduceMpPerSkillDamage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

class crJXJGetGainRTMPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetGainRTMPMethod ();
	crJXJGetGainRTMPMethod (const crJXJGetGainRTMPMethod & handle);
	MethodClass(JXJ, JXJGetGainRTMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
	int m_type;	// 0,伤害，1 攻击
	CRCore::Timer_t m_lasttime;
};

class crJXJGetSkillNeedRTMPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSkillNeedRTMPMethod ();
	crJXJGetSkillNeedRTMPMethod (const crJXJGetSkillNeedRTMPMethod & handle);
	MethodClass(JXJ, JXJGetSkillNeedRTMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_skillThis;
	float *m_output;
};

class crJXJExtraDonotGetMPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method  一定时间内无法获得士气
public:
	crJXJExtraDonotGetMPMethod ();
	crJXJExtraDonotGetMPMethod (const crJXJExtraDonotGetMPMethod & handle);
	MethodClass(JXJ, JXJExtraDonotGetMP)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

//class crJXJReduceRTMPDirectlyMethod: public CRCore::crMethod
//{//SceneServer & GameClient Method		直接减少士气值
//public:
//	crJXJReduceRTMPDirectlyMethod();
//	crJXJReduceRTMPDirectlyMethod(const crJXJReduceRTMPDirectlyMethod& handle);
//	MethodClass(JXJ, JXJReduceRTMPDirectly)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
//	float m_extraValue;
//};
class crJXJHitParamRTMPMethod: public CRCore::crMethod
{//SceneServer Method		直接增加士气值
public:
	crJXJHitParamRTMPMethod();
	crJXJHitParamRTMPMethod(const crJXJHitParamRTMPMethod& handle);
	MethodClass(JXJ, JXJHitParamRTMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	int m_flg;//0攻击者，1表示受击者
	float m_extraValue;//+表示加士气，－表示减士气
};

class crJXJClearTargetExtraPerTimeMethod : public CRCore::crMethod
{//SceneServer ChildItem Method  清除自身负面效果
public: 
	crJXJClearTargetExtraPerTimeMethod ();
	crJXJClearTargetExtraPerTimeMethod (const crJXJClearTargetExtraPerTimeMethod & handle);
	MethodClass(JXJ, JXJClearTargetExtraPerTime)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	//CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_exceptBufType;
	float m_duration;
	float m_timer;
	float m_interval;
	float *m_dt;
	float m_dtrec;
	bool m_start;
};

class crJXJChangeNPCShiliFromTableMethod : public CRCore::crMethod
{//SceneServer Method 改变npc势力 
public:
	crJXJChangeNPCShiliFromTableMethod ();
	crJXJChangeNPCShiliFromTableMethod (const crJXJChangeNPCShiliFromTableMethod & handle);
	MethodClass(JXJ, JXJChangeNPCShiliFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	char m_scriptVal;
	bool m_initdata;//是否满血
};

class crJXJExtraGainMpPerDamageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method  增加每次收到伤害获得的士气
public:
	crJXJExtraGainMpPerDamageMethod ();
	crJXJExtraGainMpPerDamageMethod (const crJXJExtraGainMpPerDamageMethod & handle);
	MethodClass(JXJ, JXJExtraGainMpPerDamage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

}

#endif