/**********************************************************************
*
*	文件:	   appBattleMethod2.h
*
*	描述:	   
*
*	作者:	   曹霖麟
*					
*
**********************************************************************/
#ifndef JXJ_APPBATTLEMETHOD2_H
#define JXJ_APPBATTLEMETHOD2_H
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
#include <JXJ/appMethod9.h>

namespace JXJ{
// class crJXJShowFaceIconMethod : public CRCore::crMethod
// {//SceneServer Method
// public:
// 	crJXJShowFaceIconMethod();
// 	crJXJShowFaceIconMethod(const crJXJShowFaceIconMethod& handle);
// 	MethodClass(CRNetApp, JXJShowFaceIcon)
// 	virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// 	CRCore::crCanvasNode *m_this;
// 	std::string m_armyRadio;
// 	std::string m_faceicon2;
// 	std::string m_faceicon3;
// };
class crJXJShowJKIconMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJShowJKIconMethod();
	crJXJShowJKIconMethod(const crJXJShowJKIconMethod& handle);
	MethodClass(CRNetApp, JXJShowJKIcon)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_mainIconQ;
	std::string m_mainIconW;
	std::string m_mainIconE;
	std::string m_iconQ;
	std::string m_iconW;
	std::string m_iconE;
	std::string m_mainIconQSwitch;
	std::string m_mainIconWSwitch;
	std::string m_mainIconESwitch;
	std::string m_iconQSwitch;
	std::string m_iconWSwitch;
	std::string m_iconESwitch;
	std::string m_strArmyName[3];
};

class crJXJUpdateJKIconMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateJKIconMethod();
	crJXJUpdateJKIconMethod(const crJXJUpdateJKIconMethod& handle);
	MethodClass(CRNetApp, JXJUpdateJKIcon)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_isDeadSwitchQ;
	std::string m_isDeadSwitchW;
	std::string m_isDeadSwitchE;
	std::string m_shiqi[3];
	std::string m_shiqi_input[3];
};
/////////////////////////////////////////////////////////////////////////
//提示
//////////////////////////////////////////////////////////////////////////
class crJXJShowTipsMethod :public CRCore::crMethod
{
public:
	crJXJShowTipsMethod();
	crJXJShowTipsMethod(const crJXJShowTipsMethod &handle);
	MethodClass(CRNetApp,JXJShowTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	int m_show;
	float m_offsetx;
	float m_offsety;
	std::string m_name;
	int m_str;
};

//////////////////////////////////////////////////////////////////////////
//兵种属性
//////////////////////////////////////////////////////////////////////////
class  crJXJShowTargetInfoMethod :public CRCore::crMethod
{
public:
	crJXJShowTargetInfoMethod ();
	crJXJShowTargetInfoMethod (const crJXJShowTargetInfoMethod &handle);
	MethodClass(CRNetApp,JXJShowTargetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
private:
	void CreateInfoHyperText(rcfg::ConfigScript &cfg_script,int baseValue,int extraValue);
	void CreateInfoHyperText(rcfg::ConfigScript &cfg_script,float baseValue,float extraValue);
private:
	CRCore::crCanvasNode  *m_this;
	std::string m_strName;
	std::string m_strHp;
	std::string m_strAttack;
	std::string m_strDefence;
	std::string m_strATipCanvas;
	std::string m_strTipAttack;
	std::string m_strAttackSpeed;
	std::string m_strAttackScope;
	std::string m_strCrit;
	std::string m_strDTipCanvas;
	std::string m_strTipDefence;
	std::string m_strMoJia;
	std::string m_strDodge;
	std::string m_strParry;
	std::string m_strHPTipCanvas;
	std::string m_strArmyCount;
	std::string m_strPerHp;
};
///////////////////////////////////////////////////////////////////////////
//crJXJButtonAttackToPosMethod
//////////////////////////////////////////////////////////////////////////
class crJXJButtonAttackToPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJButtonAttackToPosMethod();
	crJXJButtonAttackToPosMethod(const crJXJButtonAttackToPosMethod& handle);
	MethodClass(JXJ, JXJButtonAttackToPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crMatrixTransform *m_this;
};


 //////////////////////////////////////////////////////////////////////////
 //crJXJSetRoleStopMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJSetRoleStopMethod:public CRCore::crMethod
 {
 public:
	 crJXJSetRoleStopMethod();
	 crJXJSetRoleStopMethod(const crJXJSetRoleStopMethod &handle);
	 MethodClass(JXJ,JXJSetRoleStop)
	 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 private:
	 //unsigned char m_stopstate;
	 bool m_garrison;
 };

 class crJXJTouXiangShowMethod:public CRCore::crMethod
 {
 public:
	 crJXJTouXiangShowMethod();
	 crJXJTouXiangShowMethod(const crJXJTouXiangShowMethod &handle);
	 MethodClass(JXJ,JXJTouXiangShow)
		 void virtual operator()(crHandle &handle);
	 void virtual inputParam(int i, void *param);
	 void virtual addParam(int i, const std::string& str);
	 
 private:
	 CRCore::crCanvasNode * m_this;
	 std::string m_canvas;
	 std::string m_imageBox_F1;
	 std::string m_imageBox_F2;
	 std::string m_imageBox_F3;
	 std::string m_progress_F1;
	 std::string m_progress_F2;
	 std::string m_progress_F3;
 };
 class crJXJTouXiangUIUpdateMethod:public CRCore::crMethod
 {
 public:
	 crJXJTouXiangUIUpdateMethod();
	 crJXJTouXiangUIUpdateMethod(const crJXJTouXiangUIUpdateMethod &handle);
	 MethodClass(JXJ,JXJTouXiangUIUpdate)
		 void virtual operator()(crHandle &handle);
	 void virtual inputParam(int i, void *param);
	 void virtual addParam(int i, const std::string& str);

 private:
	 CRCore::crCanvasNode * m_this;
	 std::string m_switch_F1;
	 std::string m_switch_F2;
	 std::string m_switch_F3;
 };
 //头像切换RADIO
 class crJXJFaceIconShowRadioMethod:public CRCore::crMethod
 {
 public:
	 crJXJFaceIconShowRadioMethod();
	 crJXJFaceIconShowRadioMethod(const crJXJFaceIconShowRadioMethod &handle);
	 MethodClass(JXJ,JXJFaceIconShowRadio)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 private:
	 CRCore::crCanvasNode *m_this; 
	 std::string m_radio;
 };


 //////////////////////////////////////////////////////////////////////////
 //
 //自己队伍
 //
 //////////////////////////////////////////////////////////////////////////
 class crJXJMyArmyInfoShowMethod:public CRCore::crMethod
 {
 public:
	 crJXJMyArmyInfoShowMethod();
	 crJXJMyArmyInfoShowMethod(const crJXJMyArmyInfoShowMethod &handle);
	 MethodClass(JXJ,JXJMyArmyInfoShow)
		 virtual void operator()(CRCore::crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 private:
	 CRCore::crCanvasNode *m_this;
	 std::string m_imageFirst;
	 std::string m_imageSecond;
	 std::string m_imageThrid;
	 std::string m_barFirst;
	 std::string m_barSecond;
	 std::string m_barThird;
	 std::string m_hpbar;
 };
 ///////////////////////////////////////////////////////////////////////////
 //crJXJGetBattleRadioGroupValueMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJGetBattleRadioGroupValueMethod:public CRCore::crMethod
 {
 public:
	 crJXJGetBattleRadioGroupValueMethod();
	 crJXJGetBattleRadioGroupValueMethod(const crJXJGetBattleRadioGroupValueMethod& handle);
	 MethodClass(JXJ, JXJGetBattleRadioGroupValue)
		 virtual void operator()(CRCore::crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 CRCore::crCanvasNode *m_this;
	 std::string m_radioGroup;
 };
 
 //////////////////////////////////////////////////////////////////////////
 //crJXJBattleTotalArmyCountMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJBattleTotalArmyCountMethod:public CRCore::crMethod
 {//SceneServer Method
 public:
	 crJXJBattleTotalArmyCountMethod();
	 crJXJBattleTotalArmyCountMethod(const crJXJBattleTotalArmyCountMethod &handle);
	 MethodClass(JXJ,JXJBattleTotalArmyCount)
		 virtual void operator()(CRCore::crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i,const std::string& str);
 protected:
	 CRNetApp::crRoom *m_this;
	 unsigned char m_totalPlayers;
	 unsigned short m_totalRoles;
 }; 
 //////////////////////////////////////////////////////////////////////////
 class crJXJRecvBattleTotalArmyCountMethod:public CRCore::crMethod
 {//Client Method
 public:
	 crJXJRecvBattleTotalArmyCountMethod();
	 crJXJRecvBattleTotalArmyCountMethod(const crJXJRecvBattleTotalArmyCountMethod &handle);
	 MethodClass(JXJ,JXJRecvBattleTotalArmyCount)
		 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
	 virtual void operator()(crHandle &handle);
 protected:
	 CRNetApp::crPlayerGameData *m_this;
	 CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	 int m_netType;
	 std::string m_canvas;
	 std::string m_strMyToTalPlayers;
	 std::string m_strEnemyTotalPlayers;
	 std::string m_strMyTotalRoles;
	 std::string m_strEnemyTotalRoles;
	 std::string m_progressbarsw[2];
	 std::string m_progressbar[7];
 };

 //////////////////////////////////////////////////////////////////////////
 //crJXJButtonMoveToPosMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJButtonMoveToPosMethod :public CRCore::crMethod
 {//Client Method
 public:
	 crJXJButtonMoveToPosMethod();
	 crJXJButtonMoveToPosMethod(const crJXJButtonMoveToPosMethod& handle);
	 MethodClass(JXJ, JXJButtonMoveToPos)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 //CRCore::crMatrixTransform *m_this;
 };

 class crJXJCancelMoveToPosMethod :public CRCore::crMethod
 {//Client Method
 public:
	 crJXJCancelMoveToPosMethod();
	 crJXJCancelMoveToPosMethod(const crJXJCancelMoveToPosMethod& handle);
	 MethodClass(JXJ, JXJCancelMoveToPos)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 CRCore::crMatrixTransform *m_this;
 };

///////////////////////////////////////////////////////////////////////////
//crJXJMinMapSignalPosMethod
//////////////////////////////////////////////////////////////////////////
 class crJXJMinMapSignalPosMethod:public CRCore::crMethod
 {//SceneServer Method
 public:
	 crJXJMinMapSignalPosMethod();
	 crJXJMinMapSignalPosMethod(const crJXJMinMapSignalPosMethod &handle);
	 MethodClass(JXJ,JXJMinMapSignalPos)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 CRUI::crButtonWidgetNode* m_this;
	 CRProducer::crGUIEventAdapter* m_ea;
 };

 class crJXJIsSignalMethod :public CRCore::crMethod
 {//Client Method
 public:
	 crJXJIsSignalMethod();
	 crJXJIsSignalMethod(const crJXJIsSignalMethod& handle);
	 MethodClass(JXJ, JXJIsSignal)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 };
 class crJXJIsGameServerMethod :public CRCore::crMethod
 {//Gamesrever Client Method
 public:
	 crJXJIsGameServerMethod();
	 crJXJIsGameServerMethod(const crJXJIsGameServerMethod& handle);
	 MethodClass(JXJ, JXJIsGameServer)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 };
 //////////////////////////////////////////////////////////////////////////
 //crJXJCancelPatrolMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJCancelPatrolMethod :public CRCore::crMethod
 {//Client Method
 public:
	 crJXJCancelPatrolMethod();
	 crJXJCancelPatrolMethod(const crJXJCancelPatrolMethod& handle);
	 MethodClass(JXJ, JXJCancelPatrol)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 CRCore::crMatrixTransform *m_this;
 };
 //////////////////////////////////////////////////////////////////////////
 //crJXJCancelSignalMethod
 //////////////////////////////////////////////////////////////////////////
 class crJXJCancelSignalMethod :public CRCore::crMethod
 {//Client Method
 public:
	 crJXJCancelSignalMethod();
	 crJXJCancelSignalMethod(const crJXJCancelSignalMethod& handle);
	 MethodClass(JXJ, JXJCancelSignal)
		 virtual void operator()(crHandle &handle);
	 virtual void inputParam(int i, void *param);
	 virtual void addParam(int i, const std::string& str);
 protected:
	 CRCore::crMatrixTransform *m_this;
 };
 //////////////////////////////////////////////////////////////////////////
 //crJXJSignalShowControlMethod
 //////////////////////////////////////////////////////////////////////////
class crJXJSignalShowControlMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJSignalShowControlMethod();
	crJXJSignalShowControlMethod(const crJXJSignalShowControlMethod & handle);
	MethodClass(JXJ,JXJSignalShowControl)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvSignalsPosMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvSignalsPosMethod:public CRCore::crMethod
{//scene Client Method
public:
	crJXJRecvSignalsPosMethod();
	crJXJRecvSignalsPosMethod(const crJXJRecvSignalsPosMethod &handle);
	MethodClass(JXJ,JXJRecvSignalsPos)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strMiniMap;
	std::string m_strSignal;
	std::string m_strRotMap;
};
////////////////////////////////////////////////////////////////////////////
////
////crJXJWindowMouseHook
////
////////////////////////////////////////////////////////////////////////////
//typedef bool (*INITPROC)(HWND);
//typedef void (*ENDHOOKPROC)();
//class crJXJWindowMouseHookMethod:public CRCore::crMethod
//{//Client Method
//public:
//	crJXJWindowMouseHookMethod();
//	crJXJWindowMouseHookMethod(const crJXJWindowMouseHookMethod &handle);
//	MethodClass(JXJ,JXJWindowMouseHook)
//		virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//private:
//	int m_bHook;
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJButtonRolePatrolMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJButtonRolePatrolMethod:public CRCore::crMethod
{
public:
	crJXJButtonRolePatrolMethod::crJXJButtonRolePatrolMethod();
	crJXJButtonRolePatrolMethod::crJXJButtonRolePatrolMethod(const crJXJButtonRolePatrolMethod &handle);
	MethodClass(JXJ,JXJButtonRolePatrol);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
private:
};

//////////////////////////////////////////////////////////////////////////
//
//crBattleBirthPointMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChooseBirthPointTimerMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChooseBirthPointTimerMethod();
	crJXJChooseBirthPointTimerMethod(const crJXJChooseBirthPointTimerMethod &handle);
	MethodClass(JXJ,JXJChooseBirthPointTimer)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRUI::crStaticTextWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCardInfoShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCardInfoShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardInfoShowMethod();
	crJXJCardInfoShowMethod(const crJXJCardInfoShowMethod& handle);
	MethodClass(JXJ, JXJCardInfoShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crImageBoxWidgetNode *m_this;	
	int m_equipIndex;
	bool m_isShow;
	unsigned short m_index;
	std::string m_canvas;
	std::string m_name;
	std::string m_SkillName;
	std::string m_information;
	std::string m_skillCdTime;
	std::string m_skilHotkey;
	std::string m_inputCd;
	std::string m_inputHotKey;
	float m_offsetx;
	float m_offsety;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCardPushMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCardPushMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardPushMethod();
	crJXJCardPushMethod(const crJXJCardPushMethod& handle);
	MethodClass(JXJ, JXJCardPush)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_index;
};
///////////////////////////////////////////////////////////////////////////
//
//crJXJCardPushUseSkillMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCardPushUseSkillMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardPushUseSkillMethod();
	crJXJCardPushUseSkillMethod(const crJXJCardPushUseSkillMethod& handle);
	MethodClass(JXJ, JXJCardPushUseSkill)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
////////////////////////////////////////////////////////////////////////////
//
//crJXJCardUseSkillDecalUpdateLogic
//
//////////////////////////////////////////////////////////////////////////
class crJXJCardUseSkillDecalUpdateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJCardUseSkillDecalUpdateLogic();
	crJXJCardUseSkillDecalUpdateLogic(const crJXJCardUseSkillDecalUpdateLogic& handle);
	LogicClass(JXJ, JXJCardUseSkillDecalUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRUI::crImageBoxWidgetNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx_range;
	CRCore::crNode *m_decalNode;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJChooseBirthPointButtonMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChooseBirthPointButtonMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChooseBirthPointButtonMethod();
	crJXJChooseBirthPointButtonMethod(const crJXJChooseBirthPointButtonMethod& handle);
	MethodClass(JXJ, JXJChooseBirthPointButton)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJConjureCDTimerMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJConjureCDTimerMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJConjureCDTimerMethod();
	crJXJConjureCDTimerMethod(const crJXJConjureCDTimerMethod& handle);
	MethodClass(JXJ, JXJConjureCDTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_cdinputQ;
	std::string m_sequenceQ;
	std::string m_cdinputW;
	std::string m_sequenceW;
	std::string m_cdinputE;
	std::string m_sequenceE;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRandomBirthPointMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRandomBirthPointMethod:public CRCore::crMethod
{//client method
public:
	crJXJRandomBirthPointMethod();
	crJXJRandomBirthPointMethod(const crJXJRandomBirthPointMethod& handle);
	MethodClass(JXJ, JXJRandomBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJIsAllDeadShowMethod
//
////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::pair<int,/*unsigned short*/int>> AddArmyInfoVec;
class crJXJIsAllDeadShowMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJIsAllDeadShowMethod();
	crJXJIsAllDeadShowMethod(const crJXJIsAllDeadShowMethod& handle);
	MethodClass(JXJ, JXJIsAllDeadShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	std::string m_canvas;
	std::string m_canvasAdd;
	std::string m_strInfo;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJShowTimerMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJShowTimerMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJShowTimerMethod();
	crJXJShowTimerMethod(const crJXJShowTimerMethod &handle);
	MethodClass(JXJ,JXJShowTimer)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRUI::crStaticTextWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJMainHallDamageSignalMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJMainHallDamageSignalMethod:public CRCore::crMethod
{//Client
public:
	crJXJMainHallDamageSignalMethod();
	crJXJMainHallDamageSignalMethod(const crJXJMainHallDamageSignalMethod& handle);
	MethodClass(JXJ,JXJMainHallDamageSignal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crWidgetNode>m_signal;
	std::string m_canvas;
	std::string m_seq;
	std::string m_image;
	std::string m_rotate;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattleAddArmyInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIBattleAddArmyInitMethod:public CRCore::crMethod
{//Client
public:
	crJXJUIBattleAddArmyInitMethod();
	crJXJUIBattleAddArmyInitMethod(const crJXJUIBattleAddArmyInitMethod& handle);
	MethodClass(JXJ,JXJUIBattleAddArmyInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_drag;
	std::string m_icon[3];
	std::string m_input[3];
	std::string m_strBtMakeStrong;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattleAddArmyMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIBattleAddArmyMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIBattleAddArmyMethod();
	crJXJUIBattleAddArmyMethod(const crJXJUIBattleAddArmyMethod& handle);
	MethodClass(JXJ,JXJUIBattleAddArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
	int m_addArmyCount;
/*	int m_index;*/
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleAddArmyMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleAddArmyMethod:public CRCore::crMethod
{//Client 
public:
	crJXJBattleAddArmyMethod();
	crJXJBattleAddArmyMethod(const crJXJBattleAddArmyMethod& handle);
	MethodClass(JXJ,JXJBattleAddArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleAddArmyMethod
//
//////////////////////////////////////////////////////////////////////////
struct NeedArmyInfo
{
	int roleid;
	int abstarctid;
	unsigned short needcount;
};
class crJXJRecvBattleAddArmyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBattleAddArmyMethod();
	crJXJRecvBattleAddArmyMethod(const crJXJRecvBattleAddArmyMethod& handle);
	MethodClass(JXJ, JXJRecvBattleAddArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strText;
};
class crJXJVIPAddArmyMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJVIPAddArmyMethod();
	crJXJVIPAddArmyMethod(const crJXJVIPAddArmyMethod& handle);
	MethodClass(JXJ, JXJVIPAddArmy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_code;
};
//////////////////////////////////////////////////////////////////////////
//
//crUIJXJRecvDamageMethod
//
//////////////////////////////////////////////////////////////////////////
class crUIJXJRecvDamageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIJXJRecvDamageMethod();
	crUIJXJRecvDamageMethod(const crUIJXJRecvDamageMethod& handle);
	MethodClass(JXJ, UIJXJRecvDamage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	std::string m_f1sequence;
	std::string m_f2sequence;
	std::string m_f3sequence;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCameraKeyMoveMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCameraKeyMoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraKeyMoveMethod();
	crJXJCameraKeyMoveMethod(const crJXJCameraKeyMoveMethod& handle);
	MethodClass(JXJ, JXJCameraKeyMove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	int m_director;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJMouseOnRoleMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJMouseOnRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMouseOnRoleMethod();
	crJXJMouseOnRoleMethod(const crJXJMouseOnRoleMethod& handle);
	MethodClass(JXJ, JXJMouseOnRole)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClearMouseOnDecalMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClearMouseOnDecalMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClearMouseOnDecalMethod();
	crJXJClearMouseOnDecalMethod(const crJXJClearMouseOnDecalMethod& handle);
	MethodClass(JXJ, JXJClearMouseOnDecal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJMouseOnRoleDecalUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJMouseOnRoleDecalUpdateMethod : public CRCore::crMethod
{
public:
	crJXJMouseOnRoleDecalUpdateMethod();
	crJXJMouseOnRoleDecalUpdateMethod(const crJXJMouseOnRoleDecalUpdateMethod& handle);
	MethodClass(JXJ, JXJMouseOnRoleDecalUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_addRadius;
	CRCore::crVector4 m_myColor;
	CRCore::crVector4 m_friendColor;
	CRCore::crVector4 m_enemyColor;
	CRCore::crVector4 m_otherColor;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUseSkillRangeDecalUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUseSkillRangeDecalUpdateMethod : public CRCore::crMethod
{
public:
	crJXJUseSkillRangeDecalUpdateMethod();
	crJXJUseSkillRangeDecalUpdateMethod(const crJXJUseSkillRangeDecalUpdateMethod& handle);
	MethodClass(JXJ, JXJUseSkillRangeDecalUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_zoffset;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerRewardInfoMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvPlayerRewardInfoMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvPlayerRewardInfoMethod();
	crJXJRecvPlayerRewardInfoMethod(const crJXJRecvPlayerRewardInfoMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerRewardInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<crPlayerRewardData> m_playerRewardData;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCreateRewardMailMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCreateRewardMailMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreateRewardMailMethod();
	crJXJCreateRewardMailMethod(const crJXJCreateRewardMailMethod& handle);
	MethodClass(JXJ, JXJCreateRewardMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<crPlayerRewardData> m_playerRewardData;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJWriteRewardMailMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJWriteRewardMailMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJWriteRewardMailMethod();
	crJXJWriteRewardMailMethod(const crJXJWriteRewardMailMethod& handle);
	MethodClass(JXJ, JXJWriteRewardMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<crPlayerRewardData> m_giftsPack;
	std::string m_mailCfg;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvExperienceMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvExperienceMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvExperienceMethod();
	crJXJRecvExperienceMethod(const crJXJRecvExperienceMethod& handle);
	MethodClass(JXJ, JXJRecvExperience)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_experience;
	int m_distillLv;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvExperienceMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvExperienceMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvExperienceMethod();
	crJXJClientRecvExperienceMethod(const crJXJClientRecvExperienceMethod& handle);
	MethodClass(JXJ, JXJClientRecvExperience)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvExploitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvExploitMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvExploitMethod();
	crJXJRecvExploitMethod(const crJXJRecvExploitMethod& handle);
	MethodClass(JXJ, JXJRecvExploit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_exploit;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvExploitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvExploitMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvExploitMethod();
	crJXJClientRecvExploitMethod(const crJXJClientRecvExploitMethod& handle);
	MethodClass(JXJ, JXJClientRecvExploit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvAchievementMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvAchievementMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvAchievementMethod();
	crJXJRecvAchievementMethod(const crJXJRecvAchievementMethod& handle);
	MethodClass(JXJ, JXJRecvAchievement)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_achievement;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFoodMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvFoodMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvFoodMethod();
	crJXJRecvFoodMethod(const crJXJRecvFoodMethod& handle);
	MethodClass(JXJ, JXJRecvFood)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_food;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvWoodMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvWoodMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvWoodMethod();
	crJXJRecvWoodMethod(const crJXJRecvWoodMethod& handle);
	MethodClass(JXJ, JXJRecvWood)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_wood;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvIronMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvIronMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvIronMethod();
	crJXJRecvIronMethod(const crJXJRecvIronMethod& handle);
	MethodClass(JXJ, JXJRecvIron)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_iron;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvHorseMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvHorseMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvHorseMethod();
	crJXJRecvHorseMethod(const crJXJRecvHorseMethod& handle);
	MethodClass(JXJ, JXJRecvHorse)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_horse;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvAchievementMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvAchievementMethod :public CRCore::crMethod
{// Client Method
public:
	crJXJClientRecvAchievementMethod();
	crJXJClientRecvAchievementMethod(const crJXJClientRecvAchievementMethod& handle);
	MethodClass(JXJ, JXJClientRecvAchievement)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvRewardItemsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRewardItemsMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvRewardItemsMethod();
	crJXJRecvRewardItemsMethod(const crJXJRecvRewardItemsMethod& handle);
	MethodClass(JXJ, JXJRecvRewardItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	RewardItemVec* m_rewardItems;
	unsigned char m_type ;
	unsigned char m_cycle;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvRewardItemsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvRewardItemsMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvRewardItemsMethod();
	crJXJClientRecvRewardItemsMethod(const crJXJClientRecvRewardItemsMethod& handle);
	MethodClass(JXJ, JXJClientRecvRewardItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_jiangkabag;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvRewardEquipsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRewardEquipsMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvRewardEquipsMethod();
	crJXJRecvRewardEquipsMethod(const crJXJRecvRewardEquipsMethod& handle);
	MethodClass(JXJ, JXJRecvRewardEquips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	RewardEquipsVec* m_rewardEquips;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCityBelongCheckMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCityBelongCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCityBelongCheckMethod();
	crJXJCityBelongCheckMethod(const crJXJCityBelongCheckMethod& handle);
	MethodClass(JXJ, JXJCityBelongCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	_crInt32 m_battleid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSelectRoleInfoTipsShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSelectRoleInfoTipsShowMethod:public CRCore::crMethod
{//Client
public:
	crJXJSelectRoleInfoTipsShowMethod();
	crJXJSelectRoleInfoTipsShowMethod(const crJXJSelectRoleInfoTipsShowMethod &handle);
	MethodClass(JXJ,JXJSelectRoleInfoTipsShow)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
private:
	CRUI::crImageBoxWidgetNode *m_this;
	std::string m_friendCanvas;
	std::string m_enemyCanvas;
	bool m_isShow;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUISelectRoleInfoTipsInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUISelectRoleInfoTipsInitMethod:public CRCore::crMethod
{//Client
public:
	crJXJUISelectRoleInfoTipsInitMethod();
	crJXJUISelectRoleInfoTipsInitMethod(const crJXJUISelectRoleInfoTipsInitMethod &handle);
	MethodClass(JXJ,JXJUISelectRoleInfoTipsInit)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
private:
	CRCore::crCanvasNode *m_this;
	std::string m_shili;
	std::string m_grade;
};
/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleAddFriendMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleAddFriendMethod:public CRCore::crMethod
{//Client
public:
	crJXJBattleAddFriendMethod();
	crJXJBattleAddFriendMethod(const crJXJBattleAddFriendMethod &handle);
	MethodClass(JXJ,JXJBattleAddFriend)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
};

/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleSendMsgToFriendMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleSendMsgToFriendMethod:public CRCore::crMethod
{//Client
public:
	crJXJBattleSendMsgToFriendMethod();
	crJXJBattleSendMsgToFriendMethod(const crJXJBattleSendMsgToFriendMethod &handle);
	MethodClass(JXJ,JXJBattleSendMsgToFriend)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvasName;
	std::string m_input;
	std::string m_chatmode;
	std::string m_radio;
};
/////////////////////////////////////////////////////////////////////////
//
//crJXJUIInitBattleInfoCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIInitBattleInfoCanvasMethod:public CRCore::crMethod
{//Client
public:
	crJXJUIInitBattleInfoCanvasMethod();
	crJXJUIInitBattleInfoCanvasMethod(const crJXJUIInitBattleInfoCanvasMethod &handle);
	MethodClass(JXJ,JXJUIInitBattleInfoCanvas)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strName;
	std::string m_strAShili;
	std::string m_strDShili;
	std::string m_kaoshang1;
	std::string m_kaoshang2;
	std::string m_donghuaA;
	std::string m_donghuaD;
	std::string m_terrainInfo;
};
/////////////////////////////////////////////////////////////////////////
//
//crJXJUIInitBattleTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIInitBattleTimerUpdateMethod:public CRCore::crMethod
{//Client
public:
	crJXJUIInitBattleTimerUpdateMethod();
	crJXJUIInitBattleTimerUpdateMethod(const crJXJUIInitBattleTimerUpdateMethod &handle);
	MethodClass(JXJ,JXJUIInitBattleTimerUpdate)
		virtual void operator()(CRCore::crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strTimer[4];
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJCardPushSelectRoleMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCardPushSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardPushSelectRoleMethod();
	crJXJCardPushSelectRoleMethod(const crJXJCardPushSelectRoleMethod& handle);
	MethodClass(JXJ, JXJCardPushSelectRole)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIInitExitBattleShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIInitExitBattleShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIInitExitBattleShowMethod();
	crJXJUIInitExitBattleShowMethod(const crJXJUIInitExitBattleShowMethod& handle);
	MethodClass(JXJ, JXJUIInitExitBattleShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_input;
	int m_codetext;
	short m_timer;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTimeUpAutoExitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTimeUpAutoExitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTimeUpAutoExitMethod();
	crJXJTimeUpAutoExitMethod(const crJXJTimeUpAutoExitMethod& handle);
	MethodClass(JXJ, JXJTimeUpAutoExit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_input;
};
////////////////////////////////////////////////////////////////////////////
////
////crJXJInitPassUICanvasMethod
////
////////////////////////////////////////////////////////////////////////////
//class crJXJInitPassUICanvasMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJInitPassUICanvasMethod();
//	crJXJInitPassUICanvasMethod(const crJXJInitPassUICanvasMethod& handle);
//	MethodClass(JXJ, JXJInitPassUICanvas)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode * m_this;
//	std::string m_input_a;
//	std::string m_input_d;
//	std::string m_progress;
//	std::string m_seqRed;
//	std::string m_seqBlue;
//	std::string m_smallProgressF;
//	std::string m_smallProgressS;
//	std::string m_smallProgressT;
//	std::string m_canvas;
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIServerDisconnectMessageBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIServerDisconnectMessageBoxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIServerDisconnectMessageBoxMethod();
	crJXJUIServerDisconnectMessageBoxMethod(const crJXJUIServerDisconnectMessageBoxMethod& handle);
	MethodClass(JXJ, JXJUIServerDisconnectMessageBox)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_msgCanvas;
	std::string m_message;
	int m_messageIndex;
};
////////////////////////////////////////////////////////////////////////////
//
//crJXJCameraMouseOnEdgeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCameraMouseOnEdgeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraMouseOnEdgeMethod();
	crJXJCameraMouseOnEdgeMethod(const crJXJCameraMouseOnEdgeMethod& handle);
	MethodClass(JXJ,JXJCameraMouseOnEdge)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	float m_delta;
	float m_delta_extra;
	int m_up;
	int m_down;
	int m_left;
	int m_right;
	int m_upleft;
	int m_upright;
	int m_downleft;
	int m_downright;
};
////////////////////////////////////////////////////////////////////////////
////
////crJXJPassCanavsUIUpdateMethod
////
////////////////////////////////////////////////////////////////////////////
//class crJXJPassCanavsUIUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPassCanavsUIUpdateMethod();
//	crJXJPassCanavsUIUpdateMethod(const crJXJPassCanavsUIUpdateMethod& handle);
//	MethodClass(JXJ, JXJPassCanavsUIUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode * m_this;
//	std::string m_inputRed;
//	std::string m_inputBlue;
//	std::string m_progressBar;
//	std::string m_seqRed;
//	std::string m_seqBlue;
//	std::string m_imageArrow;
//	std::string m_smallProgress1;
//	std::string m_smallProgress2;
//	std::string m_smallProgress3;
//	std::string m_input1;
//	std::string m_input2;
//	std::string m_input3;
//	std::string m_arrow1;
//	std::string m_arrow2;
//	std::string m_arrow3;
//	std::string m_canvas;
//	std::vector<short> m_scoreVec;
//	std::vector<float> m_offsetVec;
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFuBenReplayMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFuBenReplayMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFuBenReplayMethod();
	crJXJFuBenReplayMethod(const crJXJFuBenReplayMethod& handle);
	MethodClass(JXJ, JXJFuBenReplay)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFuBenRestartMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvFuBenRestartMethod :public CRCore::crMethod
{//Scene Method
public:
	crJXJRecvFuBenRestartMethod();
	crJXJRecvFuBenRestartMethod(const crJXJRecvFuBenRestartMethod& handle);
	MethodClass(JXJ, JXJRecvFuBenRestart)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvRoomChatMessageMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRoomChatMessageMethod:public CRCore::crMethod
{//scene Client Method
public:
	crJXJRecvRoomChatMessageMethod();
	crJXJRecvRoomChatMessageMethod(const crJXJRecvRoomChatMessageMethod &handle);
	MethodClass(JXJ,JXJRecvRoomChatMessage)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetMailItemMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetMailItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGetMailItemMethod();
	crJXJGetMailItemMethod(const crJXJGetMailItemMethod& handle);
	MethodClass(JXJ, JXJGetMailItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crHypertextWidgetNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
};
//////////////////////////////////////////////////////////////////////////
//crJXJChatInputRecordReshowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChatInputRecordReshowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChatInputRecordReshowMethod();
	crJXJChatInputRecordReshowMethod(const crJXJChatInputRecordReshowMethod& handle);
	MethodClass(JXJ, JXJChatInputRecordReshow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_index;
	std::string m_input;
	int m_step;
};
class crJXJRecvMailRewardMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvMailRewardMethod();
	crJXJRecvMailRewardMethod(const crJXJRecvMailRewardMethod& handle);
	MethodClass(JXJ, JXJRecvMailReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_mailCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJMouseDragCanvasMoveMethod
//////////////////////////////////////////////////////////////////////////
class crJXJMouseDragCanvasMoveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMouseDragCanvasMoveMethod();
	crJXJMouseDragCanvasMoveMethod(const crJXJMouseDragCanvasMoveMethod& handle);
	MethodClass(JXJ, JXJMouseDragCanvasMove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_minimapcanvas;
	std::string m_frame;
	std::string m_imagebox;
	std::string m_strBigImg;
	std::string m_strminimapcanvas2;
	std::string m_strimagebox2;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCityMapMousePushPosMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCityMapMousePushPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCityMapMousePushPosMethod();
	crJXJCityMapMousePushPosMethod(const crJXJCityMapMousePushPosMethod& handle);
	MethodClass(JXJ, JXJCityMapMousePushPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCityMapCanDragMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCityMapCanDragMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCityMapCanDragMethod();
	crJXJCityMapCanDragMethod(const crJXJCityMapCanDragMethod& handle);
	MethodClass(JXJ, JXJCityMapCanDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	bool m_canBe;
};
//////////////////////////////////////////////////////////////////////////
//crJXJUICityMiniMapInitMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUICityMiniMapInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICityMiniMapInitMethod();
	crJXJUICityMiniMapInitMethod(const crJXJUICityMiniMapInitMethod& handle);
	MethodClass(JXJ, JXJUICityMiniMapInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string			m_strBigImg;
	std::string			m_miniMap;
	std::string			m_wei;
	std::string			m_shu;
	std::string			m_wu;
	std::string			m_npc;
	std::string			m_frame;
	std::string			m_imagebox;
	bool				m_init;
	int m_type;
};
//////////////////////////////////////////////////////////////////////////
//crJXJUICityMiniMapZoomMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUICityMiniMapZoomMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICityMiniMapZoomMethod();
	crJXJUICityMiniMapZoomMethod(const crJXJUICityMiniMapZoomMethod& handle);
	MethodClass(JXJ, JXJUICityMiniMapZoom)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_strBigMapCanvas;
	std::string m_strBigMapImage;
	std::string m_strOtherMapCanvas;
	std::string m_strOtherMapImage;
	std::string m_strFrame;
};
//////////////////////////////////////////////////////////////////////////
//crJXJMouseDragMinimapFrameMoveMethod
//////////////////////////////////////////////////////////////////////////
class crJXJMouseDragMinimapFrameMoveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMouseDragMinimapFrameMoveMethod();
	crJXJMouseDragMinimapFrameMoveMethod(const crJXJMouseDragMinimapFrameMoveMethod& handle);
	MethodClass(JXJ, JXJMouseDragMinimapFrameMove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_frame;
	std::string m_imageBox;
	std::string m_bigmap;
	std::string m_strBigMapImg;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJPassMainUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJPassMainUpdateMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJPassMainUpdateMethod();
	crJXJPassMainUpdateMethod(const crJXJPassMainUpdateMethod& handle);
	MethodClass(JXJ, JXJPassMainUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	float *m_dt;
};
////////////////////////////////////////////////////////////////////////////
////crJXJISFuBenInviteMethod
////////////////////////////////////////////////////////////////////////////
//class crJXJISFuBenInviteMethod:public CRCore::crMethod
//{//Client Method
//public:
//	crJXJISFuBenInviteMethod();
//	crJXJISFuBenInviteMethod(const crJXJISFuBenInviteMethod& handle);
//	MethodClass(JXJ,JXJISFuBenInvite)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvInviteTypeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvInviteTypeMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvInviteTypeMethod();
	crJXJRecvInviteTypeMethod(const crJXJRecvInviteTypeMethod& handle);
	MethodClass(JXJ,JXJRecvInviteType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJHidePreUseSkillRangeDecalMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHidePreUseSkillRangeDecalMethod();
	crJXJHidePreUseSkillRangeDecalMethod(const crJXJHidePreUseSkillRangeDecalMethod& handle);
	MethodClass(JXJ,JXJHidePreUseSkillRangeDecal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJPreUseSkillRangeDecalCreateLogic
//
//////////////////////////////////////////////////////////////////////////
class crJXJPreUseSkillRangeDecalCreateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJPreUseSkillRangeDecalCreateLogic();
	crJXJPreUseSkillRangeDecalCreateLogic(const crJXJPreUseSkillRangeDecalCreateLogic& handle);
	LogicClass(JXJ, JXJPreUseSkillRangeDecalCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNode> m_decalNode;
	float m_zoffset;
	std::string m_filename;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJPreUseSkillDecalCreateLogic
//
//////////////////////////////////////////////////////////////////////////
class crJXJPreUseSkillDecalCreateLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJPreUseSkillDecalCreateLogic();
	crJXJPreUseSkillDecalCreateLogic(const crJXJPreUseSkillDecalCreateLogic& handle);
	LogicClass(JXJ, JXJPreUseSkillDecalCreate)
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
	std::string m_filename;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvDamageMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvDamageMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvDamageMethod();
	crJXJClientRecvDamageMethod(const crJXJClientRecvDamageMethod& handle);
	MethodClass(JXJ, JXJClientRecvDamage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	int m_damage;
	unsigned char m_type;
	char m_ifire;
	std::string m_normalInjure;
	std::string m_normalInput;
	std::string m_critInjure;
	std::string m_critInput;
	std::string m_skillInjure;
	std::string m_skillInput;
	std::string m_prray;
	std::string m_dodge;
	std::string m_prrayOther;
	std::string m_dodgeOther;
	std::string m_deviate;
	std::string m_cureHPInjure;
	std::string m_cureHPInput;
	std::string m_jfscInjure;
	std::string m_jfscInput;
	std::string m_jfbjInjure;
	std::string m_jfbjInput;
	std::string m_jffsInjure;
	std::string m_jffsInput;
	std::string m_yfscInjure;
	std::string m_yfscInput;
	std::string m_yfbjInjure;
	std::string m_yfbjInput;
	std::string m_yffsInjure;
	std::string m_yffsInput;
};
////////////////////////////////////////////////////////////////////////////
////crJXJRecvDemageNodeShowMethod
////////////////////////////////////////////////////////////////////////////
//class crJXJRecvDemageNodeShowMethod:public CRCore::crMethod
//{// client
//public:
//	crJXJRecvDemageNodeShowMethod();
//	crJXJRecvDemageNodeShowMethod(const crJXJRecvDemageNodeShowMethod& handle);
//	MethodClass(JXJ, JXJRecvDemageNodeShow)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crMatrixTransform *m_this;
//};

//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvDamageUIUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvDamageUIUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvDamageUIUpdateMethod();
	crJXJClientRecvDamageUIUpdateMethod(const crJXJClientRecvDamageUIUpdateMethod& handle);
	MethodClass(JXJ, JXJClientRecvDamageUIUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_offset;
	float *m_dt;
	float m_maxoffset;
	float m_speed;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvZeroDamageUIUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvZeroDamageUIUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvZeroDamageUIUpdateMethod();
	crJXJClientRecvZeroDamageUIUpdateMethod(const crJXJClientRecvZeroDamageUIUpdateMethod& handle);
	MethodClass(JXJ, JXJClientRecvZeroDamageUIUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_offset;
	float *m_dt;
	float m_maxoffset;
	float m_speed;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvTrackFireMissUIUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvTrackFireMissUIUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvTrackFireMissUIUpdateMethod();
	crJXJClientRecvTrackFireMissUIUpdateMethod(const crJXJClientRecvTrackFireMissUIUpdateMethod& handle);
	MethodClass(JXJ, JXJClientRecvTrackFireMissUIUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_offset;
	float *m_dt;
	float m_maxoffset;
	float m_speed;
};
//////////////////////////////////////////////////////////////////////////
//crJXJUIShiliShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUIShiliShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIShiliShowMethod();
	crJXJUIShiliShowMethod(const crJXJUIShiliShowMethod& handle);
	MethodClass(JXJ, JXJUIShiliShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_defence;
	std::string m_attack;
	std::string m_strAddFriendSeq;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJKeyTabMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJKeyTabMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJKeyTabMethod();
	crJXJKeyTabMethod(const crJXJKeyTabMethod& handle);
	MethodClass(JXJ, JXJKeyTab)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSelectAllRolesMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSelectAllRolesMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectAllRolesMethod();
	crJXJSelectAllRolesMethod(const crJXJSelectAllRolesMethod& handle);
	MethodClass(JXJ, JXJSelectAllRoles)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJHideNPCHeadCanvasMethod
//////////////////////////////////////////////////////////////////////////
// class crJXJHideNPCHeadCanvasMethod :public CRCore::crMethod
// {//Client Method
// public:
// 	crJXJHideNPCHeadCanvasMethod();
// 	crJXJHideNPCHeadCanvasMethod(const crJXJHideNPCHeadCanvasMethod& handle);
// 	MethodClass(JXJ, JXJHideNPCHeadCanvas)
// 		virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// };
//////////////////////////////////////////////////////////////////////////
//crJXJChangeMoveSpeedStateMethod
//////////////////////////////////////////////////////////////////////////
// class crJXJChangeMoveSpeedStateMethod :public CRCore::crMethod
// {//Client Method
// public:
// 	crJXJChangeMoveSpeedStateMethod();
// 	crJXJChangeMoveSpeedStateMethod(const crJXJChangeMoveSpeedStateMethod& handle);
// 	MethodClass(JXJ, JXJChangeMoveSpeedState)
// 		virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// };
//////////////////////////////////////////////////////////////////////////
//crJXJShowItemInPacketMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowItemInPacketMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowItemInPacketMethod();
	crJXJShowItemInPacketMethod(const crJXJShowItemInPacketMethod& handle);
	MethodClass(JXJ, JXJShowItemInPacket)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_chatCanvas;
	std::string m_chatInput;
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowItemLinkClickMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowItemLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowItemLinkClickMethod();
	crJXJShowItemLinkClickMethod(const crJXJShowItemLinkClickMethod& handle);
	MethodClass(JXJ, JXJShowItemLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	std::string m_tips;
	std::string m_name;
	std::string m_info1;
	std::string m_info2;
	std::string m_itemlv;
	std::string m_itemicon;
	std::string m_itemequipon;
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	std::string m_JKtips;
	std::string m_card;
	std::string m_backboard;
	std::string m_guojia;
	std::string m_shuxing;
	std::string m_bingzhong;
	std::string m_leixing;
	std::string m_jinengmiaoshu;
	std::string m_nameimage;
	std::string m_jineng;
	int m_itemid;
	int m_equipMagic;
	std::string m_cardlvsw;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSelectBirthPointMapTypeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSelectBirthPointMapTypeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectBirthPointMapTypeMethod();
	crJXJSelectBirthPointMapTypeMethod(const crJXJSelectBirthPointMapTypeMethod& handle);
	MethodClass(JXJ, JXJSelectBirthPointMapType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJChatInputGetFocusMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChatInputGetFocusMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChatInputGetFocusMethod();
	crJXJChatInputGetFocusMethod(const crJXJChatInputGetFocusMethod& handle);
	MethodClass(JXJ, JXJChatInputGetFocus)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_edit;
};
//////////////////////////////////////////////////////////////////////////
//crJXJIsPlayerInFuBenMethod
//////////////////////////////////////////////////////////////////////////
class crJXJIsPlayerInFuBenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsPlayerInFuBenMethod();
	crJXJIsPlayerInFuBenMethod(const crJXJIsPlayerInFuBenMethod& handle);
	MethodClass(JXJ, JXJIsPlayerInFuBen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJIsAllDeadInFubenMethod
//////////////////////////////////////////////////////////////////////////
class crJXJIsAllDeadInFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsAllDeadInFubenMethod();
	crJXJIsAllDeadInFubenMethod(const crJXJIsAllDeadInFubenMethod& handle);
	MethodClass(JXJ, JXJIsAllDeadInFuben)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_input;
	std::string m_victoryCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJChangeBirthPointMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChangeBirthPointMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeBirthPointMethod();
	crJXJChangeBirthPointMethod(const crJXJChangeBirthPointMethod& handle);
	MethodClass(JXJ, JXJChangeBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenReliveArmyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenReliveArmyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenReliveArmyMethod();
	crJXJFubenReliveArmyMethod(const crJXJFubenReliveArmyMethod& handle);
	MethodClass(JXJ, JXJFubenReliveArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenSettle2Method
//////////////////////////////////////////////////////////////////////////
// class crJXJFubenSettle2Method :public CRCore::crMethod
// {//SceneServer Method
// public:
// 	crJXJFubenSettle2Method();
// 	crJXJFubenSettle2Method(const crJXJFubenSettle2Method& handle);
// 	MethodClass(JXJ, JXJFubenSettle2)
// 		virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// 	CRNetApp::crRoom *m_this;
// 	short m_progress;
// };
class crJXJRecvFubenSettle2Method :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvFubenSettle2Method();
	crJXJRecvFubenSettle2Method(const crJXJRecvFubenSettle2Method& handle);
	MethodClass(JXJ, JXJRecvFubenSettle2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJFubenDropItemsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJFubenDropItemsMethod();
	crJXJFubenDropItemsMethod(const crJXJFubenDropItemsMethod& handle);
	MethodClass(JXJ, JXJFubenDropItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	unsigned short m_fubenid;
	FubenDropRewardVec *m_dropVec;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvFubenDropMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvFubenDropMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvFubenDropMethod();
	crJXJClientRecvFubenDropMethod(const crJXJClientRecvFubenDropMethod& handle);
	MethodClass(JXJ, JXJClientRecvFubenDrop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryFailOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryFailOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryFailOnShowMethod();
	crJXJFubenVictoryFailOnShowMethod(const crJXJFubenVictoryFailOnShowMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryFailOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_switch;
	std::string m_exp;
	std::string m_copper;
	std::string m_stars;
	std::string m_canvasSwitch;
	std::string m_button;
	std::string m_strExploit;
	std::string m_strGiftgold;
	std::string m_strCanvas;
	std::string m_strStarSwitch;
	std::string m_strStarInput;
	std::string m_strStarSeq;
	std::string m_strSeq[3];
	std::string m_strBtSw;
	std::string m_strbtDouble2;
	std::string m_strbtDouble3;
	std::string m_strbtTriple;
	std::string m_strbtTriple2;
	std::string m_strFourfold;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryChooseRewardItemsMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryChooseRewardItemsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryChooseRewardItemsMethod();
	crJXJFubenVictoryChooseRewardItemsMethod(const crJXJFubenVictoryChooseRewardItemsMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryChooseRewardItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode* m_this;
	int m_index;
	std::string m_strswitch;
	std::string m_stricon[3];
	std::string m_strseq[3];
	std::string m_strinput[3];
	std::string m_strfoodIcon;
	std::string m_strwoodIcon;
	std::string m_strironIcon;
	std::string m_strhorseIcon;
	std::string m_strcopperIcon;
	std::string m_strexpIcon;
	std::string m_strgongxunIcon;
	std::string m_strgiftgoldIcon;
	std::string m_strgongxianIcon;
	std::string m_strzhangongIcon;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryDoubleTipsShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryDoubleTipsShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryDoubleTipsShowMethod();
	crJXJFubenVictoryDoubleTipsShowMethod(const crJXJFubenVictoryDoubleTipsShowMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryDoubleTipsShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	std::string m_canvas;
	std::string m_strInput;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryFailCanvasUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryFailCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryFailCanvasUpdateMethod();
	crJXJFubenVictoryFailCanvasUpdateMethod(const crJXJFubenVictoryFailCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryFailCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryFailCanvasCloseMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryFailCanvasCloseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryFailCanvasCloseMethod();
	crJXJFubenVictoryFailCanvasCloseMethod(const crJXJFubenVictoryFailCanvasCloseMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryFailCanvasClose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenVictoryDropItemDoubleMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenVictoryDropItemDoubleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenVictoryDropItemDoubleMethod();
	crJXJFubenVictoryDropItemDoubleMethod(const crJXJFubenVictoryDropItemDoubleMethod& handle);
	MethodClass(JXJ, JXJFubenVictoryDropItemDouble)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	unsigned char m_Index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvClientChooseFubenDropMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvClientChooseFubenDropMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvClientChooseFubenDropMethod();
	crJXJRecvClientChooseFubenDropMethod(const crJXJRecvClientChooseFubenDropMethod& handle);
	MethodClass(JXJ, JXJRecvClientChooseFubenDrop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenDropClearMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenDropClearMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJFubenDropClearMethod();
	crJXJFubenDropClearMethod(const crJXJFubenDropClearMethod& handle);
	MethodClass(JXJ, JXJFubenDropClear)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenRushRewardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFubenRushRewardMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJFubenRushRewardMethod();
	crJXJFubenRushRewardMethod(const crJXJFubenRushRewardMethod& handle);
	MethodClass(JXJ, JXJFubenRushReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	unsigned short m_fubenID;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvFubenRushRewardMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvFubenRushRewardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvFubenRushRewardMethod();
	crJXJClientRecvFubenRushRewardMethod(const crJXJClientRecvFubenRushRewardMethod& handle);
	MethodClass(JXJ, JXJClientRecvFubenRushReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_checkBox;
	std::string m_text;
	std::string m_chatcfg;
	std::string m_list;
	std::string m_strItemCountInput[7];
	std::string m_inputItemsStr[7];
	std::string m_scrollbar;
private:
	void CreateHyperText(CRUI::crHypertextWidgetNode *hyperNode,unsigned short fubenid,int copper,int exp,int archi,RewardItemVec& itemVec );
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvFubenReliveMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvFubenReliveMethod:public CRCore::crMethod
{//scene Client Method
public:
	crJXJRecvFubenReliveMethod();
	crJXJRecvFubenReliveMethod(const crJXJRecvFubenReliveMethod &handle);
	MethodClass(JXJ,JXJRecvFubenRelive)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJInitFubenPlayerReliveCountMethod
//////////////////////////////////////////////////////////////////////////
class crJXJInitFubenPlayerReliveCountMethod:public CRCore::crMethod
{//scene  Method
public:
	crJXJInitFubenPlayerReliveCountMethod();
	crJXJInitFubenPlayerReliveCountMethod(const crJXJInitFubenPlayerReliveCountMethod &handle);
	MethodClass(JXJ,JXJInitFubenPlayerReliveCount)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenReliveCheckMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenReliveCheckMethod:public CRCore::crMethod
{//scene  Method
public:
	crJXJFubenReliveCheckMethod();
	crJXJFubenReliveCheckMethod(const crJXJFubenReliveCheckMethod &handle);
	MethodClass(JXJ,JXJFubenReliveCheck)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom *m_this;
	CRNetApp::crSceneServerPlayerData * m_playerData;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleShowChatCanvasMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleShowChatCanvasMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJBattleShowChatCanvasMethod();
	crJXJBattleShowChatCanvasMethod(const crJXJBattleShowChatCanvasMethod &handle);
	MethodClass(JXJ,JXJBattleShowChatCanvas)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_chatCanvas;
	std::string m_minimapCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleChatCanvasOnEnterShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleChatCanvasOnEnterShowMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJBattleChatCanvasOnEnterShowMethod();
	crJXJBattleChatCanvasOnEnterShowMethod(const crJXJBattleChatCanvasOnEnterShowMethod &handle);
	MethodClass(JXJ,JXJBattleChatCanvasOnEnterShow)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_chatCanvas;
	std::string m_switch;
	int m_value;
};
//////////////////////////////////////////////////////////////////////////
//crJXJLoadKeyboardLayoutMethod
//////////////////////////////////////////////////////////////////////////
class crJXJLoadKeyboardLayoutMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJLoadKeyboardLayoutMethod();
	crJXJLoadKeyboardLayoutMethod(const crJXJLoadKeyboardLayoutMethod &handle);
	MethodClass(JXJ,JXJLoadKeyboardLayout)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	int m_key;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleChatCanvasAutoHideMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleChatCanvasAutoHideMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJBattleChatCanvasAutoHideMethod();
	crJXJBattleChatCanvasAutoHideMethod(const crJXJBattleChatCanvasAutoHideMethod &handle);
	MethodClass(JXJ,JXJBattleChatCanvasAutoHide)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_switch;
	std::string m_input;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenReliveCountHasUsedMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenReliveCountHasUsedMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJFubenReliveCountHasUsedMethod();
	crJXJFubenReliveCountHasUsedMethod(const crJXJFubenReliveCountHasUsedMethod &handle);
	MethodClass(JXJ,JXJFubenReliveCountHasUsed)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_button; 
};
//////////////////////////////////////////////////////////////////////////
//crJXJSkillProtectTimerProgressShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSkillProtectTimerProgressShowMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJSkillProtectTimerProgressShowMethod();
	crJXJSkillProtectTimerProgressShowMethod(const crJXJSkillProtectTimerProgressShowMethod &handle);
	MethodClass(JXJ,JXJSkillProtectTimerProgressShow)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_imageBox;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSkillProtectTimerProgressUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSkillProtectTimerProgressUpdateMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJSkillProtectTimerProgressUpdateMethod();
	crJXJSkillProtectTimerProgressUpdateMethod(const crJXJSkillProtectTimerProgressUpdateMethod &handle);
	MethodClass(JXJ,JXJSkillProtectTimerProgressUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_progress;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSkillBufferShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSkillBufferShowMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJSkillBufferShowMethod();
	crJXJSkillBufferShowMethod(const crJXJSkillBufferShowMethod &handle);
	MethodClass(JXJ,JXJSkillBufferShow)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);

	bool isInspire(int id);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[3][8];
	std::string m_strInput[3][8];
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientUserRecvSomeValueMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientUserRecvSomeValueMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJClientUserRecvSomeValueMethod();
	crJXJClientUserRecvSomeValueMethod(const crJXJClientUserRecvSomeValueMethod &handle);
	MethodClass(JXJ,JXJClientUserRecvSomeValue)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crBrain *m_this;
	int m_id;
	int m_value;
	std::string m_canvas;
	std::string m_switch;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientUserRecvSomeItemsMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientUserRecvSomeItemsMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJClientUserRecvSomeItemsMethod();
	crJXJClientUserRecvSomeItemsMethod(const crJXJClientUserRecvSomeItemsMethod &handle);
	MethodClass(JXJ,JXJClientUserRecvSomeItems)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crBrain *m_this;
	JXJItemBagVec * m_itemVec;
	JXJItemBagVec * m_cardVec;
	std::string m_canvas;
	std::string m_switch;
};
class crJXJClientRecvFoodMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvFoodMethod();
	crJXJClientRecvFoodMethod(const crJXJClientRecvFoodMethod& handle);
	MethodClass(JXJ, JXJClientRecvFood)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJClientRecvWoodMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvWoodMethod();
	crJXJClientRecvWoodMethod(const crJXJClientRecvWoodMethod& handle);
	MethodClass(JXJ, JXJClientRecvWood)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJClientRecvIronMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvIronMethod();
	crJXJClientRecvIronMethod(const crJXJClientRecvIronMethod& handle);
	MethodClass(JXJ, JXJClientRecvIron)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJClientRecvHorseMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvHorseMethod();
	crJXJClientRecvHorseMethod(const crJXJClientRecvHorseMethod& handle);
	MethodClass(JXJ, JXJClientRecvHorse)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJIsPlayerHasCardGuideShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJIsPlayerHasCardGuideShowMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJIsPlayerHasCardGuideShowMethod();
	crJXJIsPlayerHasCardGuideShowMethod(const crJXJIsPlayerHasCardGuideShowMethod& handle);
	MethodClass(JXJ, JXJIsPlayerHasCardGuideShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasFirst;
	std::string m_canvasSecod;
	std::string m_canvasCard;
	std::string m_icon[3];
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowUserHelpCanvasMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowUserHelpCanvasMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShowUserHelpCanvasMethod();
	crJXJShowUserHelpCanvasMethod(const crJXJShowUserHelpCanvasMethod& handle);
	MethodClass(JXJ, JXJShowUserHelpCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_hypertext;
	std::string m_cfg;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvOnlineRewardQueryMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvOnlineRewardQueryMethod :public CRCore::crMethod
{//GameServer/Client  Method
public:
	crJXJRecvOnlineRewardQueryMethod();
	crJXJRecvOnlineRewardQueryMethod(const crJXJRecvOnlineRewardQueryMethod& handle);
	MethodClass(JXJ, JXJRecvOnlineRewardQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_icon[4];
	std::string m_input[4];
	std::string m_imageName;
	std::string m_mainCanvas;
	std::string m_btOnlineReward;
	std::string m_timerInput;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvOnlineRewardQueryMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvOnlineRewardQueryMethod :public CRCore::crMethod
{//GameServer/Client  Method
public:
	crJXJClientRecvOnlineRewardQueryMethod();
	crJXJClientRecvOnlineRewardQueryMethod(const crJXJClientRecvOnlineRewardQueryMethod& handle);
	MethodClass(JXJ, JXJClientRecvOnlineRewardQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJOnlineRewardCanvasOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJOnlineRewardCanvasOnShowMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJOnlineRewardCanvasOnShowMethod();
	crJXJOnlineRewardCanvasOnShowMethod(const crJXJOnlineRewardCanvasOnShowMethod& handle);
	MethodClass(JXJ, JXJOnlineRewardCanvasOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon1;
	std::string m_icon2;
	std::string m_icon3;
	std::string m_icon4;
	std::string m_input1;
	std::string m_input2;
	std::string m_input3;
	std::string m_input4;
	std::string m_switch1;
	std::string m_switch2;
	std::string m_switch3;
	std::string m_switch4;
	std::string m_switch5;
	std::string m_switch6;
	std::string m_btGet;
	std::string m_iconName;
};
//////////////////////////////////////////////////////////////////////////
//crJXJOnlineRewardCanvasUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJOnlineRewardCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJOnlineRewardCanvasUpdateMethod();
	crJXJOnlineRewardCanvasUpdateMethod(const crJXJOnlineRewardCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJOnlineRewardCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_switch1;
	std::string m_switch2;
	std::string m_switch3;
	std::string m_switch4;
	std::string m_switch5;
	std::string m_switch6;
	std::string m_btGet;
};
//////////////////////////////////////////////////////////////////////////
//crJXJOnlineRewardTimerUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJOnlineRewardTimerUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJOnlineRewardTimerUpdateMethod();
	crJXJOnlineRewardTimerUpdateMethod(const crJXJOnlineRewardTimerUpdateMethod & handle);
	MethodClass(JXJ, JXJOnlineRewardTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crStaticTextWidgetNode *m_this;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientOnlineRewardQueryMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientOnlineRewardQueryMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientOnlineRewardQueryMethod();
	crJXJClientOnlineRewardQueryMethod(const crJXJClientOnlineRewardQueryMethod & handle);
	MethodClass(JXJ, JXJClientOnlineRewardQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJPlayerOnlineRewardTimerUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerOnlineRewardTimerUpdateMethod :public CRCore::crMethod
{//GameServer  Method
public:
	crJXJPlayerOnlineRewardTimerUpdateMethod();
	crJXJPlayerOnlineRewardTimerUpdateMethod(const crJXJPlayerOnlineRewardTimerUpdateMethod & handle);
	MethodClass(JXJ, JXJPlayerOnlineRewardTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRecvOnlineRewardTimerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvOnlineRewardTimerMethod :public CRCore::crMethod
{//GameServer  Method
public:
	crJXJClientRecvOnlineRewardTimerMethod();
	crJXJClientRecvOnlineRewardTimerMethod(const crJXJClientRecvOnlineRewardTimerMethod & handle);
	MethodClass(JXJ, JXJClientRecvOnlineRewardTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientOnlineRewardTimerUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientOnlineRewardTimerUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientOnlineRewardTimerUpdateMethod();
	crJXJClientOnlineRewardTimerUpdateMethod(const crJXJClientOnlineRewardTimerUpdateMethod & handle);
	MethodClass(JXJ, JXJClientOnlineRewardTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJSystermMailSendMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSystermMailSendMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJSystermMailSendMethod();
	crJXJSystermMailSendMethod(const crJXJSystermMailSendMethod & handle);
	MethodClass(JXJ, JXJSystermMailSend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	std::string m_allCountry;
	std::string m_wei;
	std::string m_shu;
	std::string m_wu;
	std::string m_title;
	std::string m_content;
	std::string m_item1;
	std::string m_item2;
	std::string m_item3;
	std::string m_item4;
	std::string m_toPlayer;
	std::string m_playerID;
private:
	void parseString(std::string &str);
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvSystermMailMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvSystermMailMethod :public CRCore::crMethod
{//Server  Method
public:
	crJXJRecvSystermMailMethod();
	crJXJRecvSystermMailMethod(const crJXJRecvSystermMailMethod & handle);
	MethodClass(JXJ, JXJRecvSystermMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSystermSendMailToPlayerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSystermSendMailToPlayerMethod :public CRCore::crMethod
{//Server  Method
public:
	crJXJSystermSendMailToPlayerMethod();
	crJXJSystermSendMailToPlayerMethod(const crJXJSystermSendMailToPlayerMethod & handle);
	MethodClass(JXJ, JXJSystermSendMailToPlayer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<crJXJSystermMailInfo> m_systermMailInfo;
	std::string m_mailCfg;
private:
	void CreataSysMail(int playerID,crJXJSystermMailInfo *sysMailInfo,CreateGiftsPackDeque *giftsPacketDeque,MailTaskDeque *mail_deque);
};

//////////////////////////////////////////////////////////////////////////
//crJXJSystermMailDisposeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSystermMailDisposeMethod :public CRCore::crMethod
{//Server  Method
public:
	crJXJSystermMailDisposeMethod();
	crJXJSystermMailDisposeMethod(const crJXJSystermMailDisposeMethod & handle);
	MethodClass(JXJ, JXJSystermMailDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//CRCore::ref_ptr<crJXJSystermMailInfo> m_systermMailInfo;
	std::string m_mailCfg;
	float m_availableTime;
private:
	void CreataSysMail(int playerID,crJXJSystermMailInfo *sysMailInfo,CreateGiftsPackDeque *giftsPacketDeque,MailTaskDeque *mail_deque);
};

//////////////////////////////////////////////////////////////////////////
//crJXJShowRecvBagItemMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowRecvBagItemMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShowRecvBagItemMethod();
	crJXJShowRecvBagItemMethod(const crJXJShowRecvBagItemMethod & handle);
	MethodClass(JXJ, JXJShowRecvBagItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crBrain *m_this;
	std::string m_canvasInput;
	std::string m_canvasImage;
	std::string m_input;
	std::string m_image;
	std::string m_beibaoParentCanvas;
	std::string m_beibao;
	CRCore::crVector3 m_pos;
	float m_speed;
	CRCore::crVector3 m_inputPos;
	CRCore::crVector3 m_inputTargetPos;
	float m_inputSpeed;
	std::string m_canvasJK;
	std::string m_imageJK;
	std::string m_switchJK;
	std::string m_jkBeiBao;
	CRCore::crVector3 m_JKpos;
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowRecvBagItemCanvasUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowRecvBagItemCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShowRecvBagItemCanvasUpdateMethod();
	crJXJShowRecvBagItemCanvasUpdateMethod(const crJXJShowRecvBagItemCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJShowRecvBagItemCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_dt;
	int m_index;
	float m_rotate;
};
//////////////////////////////////////////////////////////////////////////
//crJXJIsGetSelectRolesSpeedMethod
//////////////////////////////////////////////////////////////////////////
class crJXJIsGetSelectRolesSpeedMethod :public CRCore::crMethod
{//Client & Server  Method
public:
	crJXJIsGetSelectRolesSpeedMethod();
	crJXJIsGetSelectRolesSpeedMethod(const crJXJIsGetSelectRolesSpeedMethod & handle);
	MethodClass(JXJ, JXJIsGetSelectRolesSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_isCheckSelectRoles;
	std::string m_strCanvas;
	std::string m_strCheckBox;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetSelectRolesSpeedMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetSelectRolesSpeedMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJGetSelectRolesSpeedMethod();
	crJXJGetSelectRolesSpeedMethod(const crJXJGetSelectRolesSpeedMethod & handle);
	MethodClass(JXJ, JXJGetSelectRolesSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSystermNoticeCreateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSystermNoticeCreateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSystermNoticeCreateMethod();
	crJXJSystermNoticeCreateMethod(const crJXJSystermNoticeCreateMethod& handle);
	MethodClass(JXJ, JXJSystermNoticeCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	crJXJChatMessageInfo *m_chatInfo;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBroadcastMessageDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBroadcastMessageDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJBroadcastMessageDisposeMethod();
	crJXJBroadcastMessageDisposeMethod(const crJXJBroadcastMessageDisposeMethod& handle);
	MethodClass(JXJ, JXJBroadcastMessageDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_id;
	//float *m_dt;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGroupMessageDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGroupMessageDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGroupMessageDisposeMethod();
	crJXJGroupMessageDisposeMethod(const crJXJGroupMessageDisposeMethod& handle);
	MethodClass(JXJ, JXJGroupMessageDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//float *m_dt;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTeamMessageDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTeamMessageDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJTeamMessageDisposeMethod();
	crJXJTeamMessageDisposeMethod(const crJXJTeamMessageDisposeMethod& handle);
	MethodClass(JXJ, JXJTeamMessageDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	short m_countPerFrame;
	std::vector< CRCore::ref_ptr<crJXJChatMessageInfo> > m_taskVec;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerMessageDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerMessageDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJPlayerMessageDisposeMethod();
	crJXJPlayerMessageDisposeMethod(const crJXJPlayerMessageDisposeMethod& handle);
	MethodClass(JXJ, JXJPlayerMessageDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	short m_countPerFrame;
	std::vector< CRCore::ref_ptr<crJXJChatMessageInfo> > m_taskVec;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJVIPCanvasOnShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJVIPCanvasOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJVIPCanvasOnShowMethod();
	crJXJVIPCanvasOnShowMethod(const crJXJVIPCanvasOnShowMethod& handle);
	MethodClass(JXJ, JXJVIPCanvasOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_levelText;
	std::string m_levelSwitch;
	std::string m_radiowidget;
	std::string m_switch;
	std::string m_vipProgressBar;
	std::string m_vipProgressValue;
	std::string m_vipList;
	std::string m_btUpLevel;
	std::string m_radioSvWidget;
	std::string m_NeizhengRadio;
	std::string m_NeizhengIcon[4];
	std::string m_NeizhengProgress[4];
	std::string m_NeizhengAdd[4];
	std::string m_NeizhengCost[4];
	std::string m_NeizhengUpgradebtn[4];
	std::string m_NeizhengScrollBar;
	std::string m_levelsw[4];
	std::string m_namesw;
	std::string m_vipinfolist;
	std::string m_needCharge;
	std::string m_upgradeVipLvText;
	std::string m_leftbtn; 
	std::string m_rightbtn;
	std::string m_lingqubtn;
	std::string m_rewardicon[5];
	std::string m_rewardnum[5];
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJVIPCanvasSwitchChangeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJVIPCanvasSwitchChangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJVIPCanvasSwitchChangeMethod();
	crJXJVIPCanvasSwitchChangeMethod(const crJXJVIPCanvasSwitchChangeMethod& handle);
	MethodClass(JXJ, JXJVIPCanvasSwitchChange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_switch;
	int m_value;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJVIPLevelUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJVIPLevelUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJVIPLevelUpdateMethod();
	crJXJVIPLevelUpdateMethod(const crJXJVIPLevelUpdateMethod& handle);
	MethodClass(JXJ, JXJVIPLevelUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvVIPUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvVIPUpdateMethod :public CRCore::crMethod
{//Server Client Method
public:
	crJXJRecvVIPUpdateMethod();
	crJXJRecvVIPUpdateMethod(const crJXJRecvVIPUpdateMethod& handle);
	MethodClass(JXJ, JXJRecvVIPUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJAddVipExpMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJAddVipExpMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJAddVipExpMethod();
	crJXJAddVipExpMethod(const crJXJAddVipExpMethod& handle);
	MethodClass(JXJ, JXJAddVipExp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_expValue;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvVIPExpUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvVIPExpUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvVIPExpUpdateMethod();
	crJXJRecvVIPExpUpdateMethod(const crJXJRecvVIPExpUpdateMethod& handle);
	MethodClass(JXJ, JXJRecvVIPExpUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJItemTypeCheckMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJItemTypeCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJItemTypeCheckMethod();
	crJXJItemTypeCheckMethod(const crJXJItemTypeCheckMethod& handle);
	MethodClass(JXJ, JXJItemTypeCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crBrain *m_this;
	int m_itemID;
	int *m_type;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFaceIconCanvasInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFaceIconCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFaceIconCanvasInitMethod();
	crJXJFaceIconCanvasInitMethod(const crJXJFaceIconCanvasInitMethod& handle);
	MethodClass(JXJ, JXJFaceIconCanvasInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_chatCanvas;
	std::string m_faceIconBtn;
	std::string m_hyperText;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJChangeBirthPointButtonStateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChangeBirthPointButtonStateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeBirthPointButtonStateMethod();
	crJXJChangeBirthPointButtonStateMethod(const crJXJChangeBirthPointButtonStateMethod& handle);
	MethodClass(JXJ, JXJChangeBirthPointButtonState)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTouchRoleShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTouchRoleShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTouchRoleShowMethod();
	crJXJTouchRoleShowMethod(const crJXJTouchRoleShowMethod& handle);
	MethodClass(JXJ, JXJTouchRoleShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	//double * m_time;
	std::string m_canvas;
	std::string m_strbt_a;
	std::string m_strbt_m;
	std::string m_strSwitch;
	std::string m_strUp;
	std::string m_strDown;
	std::string m_strEquips;
	std::string m_strName;
	std::string m_strGrade;
	std::string m_strPower;
	std::string m_strTouxiang;
	std::string m_strSwPlane;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvGetRoomMemberInfoMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvGetRoomMemberInfoMethod :public CRCore::crMethod
{//Server Client Method
public:
	crJXJRecvGetRoomMemberInfoMethod();
	crJXJRecvGetRoomMemberInfoMethod(const crJXJRecvGetRoomMemberInfoMethod& handle);
	MethodClass(JXJ, JXJRecvGetRoomMemberInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleRoomMemberInfoCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleRoomMemberInfoCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleRoomMemberInfoCanvasMethod();
	crJXJBattleRoomMemberInfoCanvasMethod(const crJXJBattleRoomMemberInfoCanvasMethod& handle);
	MethodClass(JXJ, JXJBattleRoomMemberInfoCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_playerIcon;
	std::string m_playerNameInput;
	std::string m_playerLvInput;
	std::string m_playerBingFa;
	std::string m_playerZhenFa;
	std::string m_playerDongCha;
	std::string m_strZhanDouLi;
	std::string m_strRoleName;
	std::string m_equip[5];
	std::string m_defaultname[5];
	std::string m_strUp;
	std::string m_strSwPlane;
	std::string m_strNpcTouXiang;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleLordShowEquipMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleLordShowEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleLordShowEquipMethod();
	crJXJBattleLordShowEquipMethod(const crJXJBattleLordShowEquipMethod& handle);
	MethodClass(JXJ, JXJBattleLordShowEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	std::string m_strSwitch;
	std::string m_strBt;
	int m_nIndex;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSetLordEquipCanvasCloseMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSetLordEquipCanvasCloseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetLordEquipCanvasCloseMethod();
	crJXJSetLordEquipCanvasCloseMethod(const crJXJSetLordEquipCanvasCloseMethod& handle);
	MethodClass(JXJ, JXJSetLordEquipCanvasClose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
};
class crJXJBattleLordEquipTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleLordEquipTipInfoMethod();
	crJXJBattleLordEquipTipInfoMethod(const crJXJBattleLordEquipTipInfoMethod& handle);
	MethodClass(JXJ, JXJBattleLordEquipTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info1;
	std::string m_info2;
	std::string m_itemlv;
	std::string m_itemicon;
	std::string m_itemequipon;
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJOpenRebelTimerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJOpenRebelTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJOpenRebelTimerMethod();
	crJXJOpenRebelTimerMethod(const crJXJOpenRebelTimerMethod& handle);
	MethodClass(JXJ, JXJOpenRebelTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	time_t m_timer;
	int m_nRandomTimer;
	//std::string m_chengchi2TabFile;
	//short m_settleTime;
};

//////////////////////////////////////////////////////////////////////////
//crJXJRandomRebelCityMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRandomRebelCityMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRandomRebelCityMethod();
	crJXJRandomRebelCityMethod(const crJXJRandomRebelCityMethod& handle);
	MethodClass(JXJ, JXJRandomRebelCity)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvRandomRebelCityMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRandomRebelCityMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvRandomRebelCityMethod();
	crJXJRecvRandomRebelCityMethod(const crJXJRecvRandomRebelCityMethod& handle);
	MethodClass(JXJ, JXJRecvRandomRebelCity)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_cityMapCanvas;
	std::string m_rebelIcon;
	std::string m_rebelCanvas;
	std::string m_strEnterCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityCanvasInitMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRebelCityCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRebelCityCanvasInitMethod();
	crJXJRebelCityCanvasInitMethod(const crJXJRebelCityCanvasInitMethod& handle);
	MethodClass(JXJ, JXJRebelCityCanvasInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strCanvas;
	std::string m_strTitle;
	std::string m_strIntro;
	std::string m_strCopper;
	std::string m_strIcon;
	std::string m_strSwitch;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityStopMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRebelCityStopMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRebelCityStopMethod();
	crJXJRebelCityStopMethod(const crJXJRebelCityStopMethod& handle);
	MethodClass(JXJ, JXJRebelCityStop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strCanvas;
	//std::string m_strSwitch;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvStopCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvStopCounterinsurgencyMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvStopCounterinsurgencyMethod();
	crJXJRecvStopCounterinsurgencyMethod(const crJXJRecvStopCounterinsurgencyMethod& handle);
	MethodClass(JXJ, JXJRecvStopCounterinsurgency)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityCanvasInitMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRebelCityCanvasFrameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRebelCityCanvasFrameMethod();
	crJXJRebelCityCanvasFrameMethod(const crJXJRebelCityCanvasFrameMethod& handle);
	MethodClass(JXJ, JXJRebelCityCanvasFrame)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strCanvas;
	std::string m_strTitle;
	std::string m_strBtn1;
	std::string m_strBtn2;
	std::string m_strProgress;
	std::string m_strInput;
	float *m_dt;
};
////////////////////////////////////////////////////////////////////////////
//crJXJRebelProgressFrameUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRebelProgressFrameUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRebelProgressFrameUpdateMethod();
	crJXJRebelProgressFrameUpdateMethod(const crJXJRebelProgressFrameUpdateMethod& handle);
	MethodClass(JXJ, JXJRebelProgressFrameUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_dt;
	std::string m_strCityMap;
	std::string m_strProgress;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCounterinsurgencyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCounterinsurgencyMethod();
	crJXJCounterinsurgencyMethod(const crJXJCounterinsurgencyMethod& handle);
	MethodClass(JXJ, JXJCounterinsurgency)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	std::string m_strCanvas;
	std::string m_strMsgCanvas;
	std::string m_strMsg;
	std::string m_strSwitch;
	bool m_bIsPiliang;
	std::string m_strProgress;
};
//////////////////////////////////////////////////////////////////////////
//crJXJDoCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJDoCounterinsurgencyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDoCounterinsurgencyMethod();
	crJXJDoCounterinsurgencyMethod(const crJXJDoCounterinsurgencyMethod& handle);
	MethodClass(JXJ, JXJDoCounterinsurgency)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crButtonWidgetNode *m_this;
	std::string m_strCanvas;
	std::string m_strSwitch;
	bool m_bIsPiLiang;
	std::string m_strProgress;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvCounterinsurgencyMethod :public CRCore::crMethod
{//GameServer/Client  Method
public:
	crJXJRecvCounterinsurgencyMethod();
	crJXJRecvCounterinsurgencyMethod(const crJXJRecvCounterinsurgencyMethod& handle);
	MethodClass(JXJ, JXJRecvCounterinsurgency)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_strCanvas;
	std::string m_strSwitch;
	std::string m_strProgress;
	std::string m_strEnterCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCounterinsurgencyDisposeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCounterinsurgencyDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCounterinsurgencyDisposeMethod();
	crJXJCounterinsurgencyDisposeMethod(const crJXJCounterinsurgencyDisposeMethod& handle);
	MethodClass(JXJ, JXJCounterinsurgencyDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityTimerUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRebelCityTimerUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRebelCityTimerUpdateMethod();
	crJXJRebelCityTimerUpdateMethod(const crJXJRebelCityTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJRebelCityTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//JXJRebelCityIconUpdate
//////////////////////////////////////////////////////////////////////////
class crJXJRebelCityIconUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRebelCityIconUpdateMethod();
	crJXJRebelCityIconUpdateMethod(const crJXJRebelCityIconUpdateMethod& handle);
	MethodClass(JXJ, JXJRebelCityIconUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strIcon;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBingzhengBirthPointChangeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBingzhengBirthPointChangeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBingzhengBirthPointChangeMethod();
	crJXJBingzhengBirthPointChangeMethod(const crJXJBingzhengBirthPointChangeMethod& handle);
	MethodClass(JXJ, JXJBingzhengBirthPointChange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	short m_fubenprogress;
	//bool m_battleClosed;//false:表示战场可以进入，true：表示攻方不可进入
};

////////////////////////////////////////////////////////////////////////////
////crJXJRecvCampBirthPointChangeMethod
////////////////////////////////////////////////////////////////////////////
class crJXJRecvCampBirthPointChangeMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvCampBirthPointChangeMethod();
	crJXJRecvCampBirthPointChangeMethod(const crJXJRecvCampBirthPointChangeMethod &handle);
	MethodClass(JXJ,JXJRecvCampBirthPointChange)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleInspireMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleInspireMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJBattleInspireMethod();
	crJXJBattleInspireMethod(const crJXJBattleInspireMethod &handle);
	MethodClass(JXJ,JXJBattleInspire)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	unsigned short m_itemId;//鼓舞令物品ID
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvInspireMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvInspireMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvInspireMethod();
	crJXJRecvInspireMethod(const crJXJRecvInspireMethod &handle);
	MethodClass(JXJ,JXJRecvInspire)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strInput;
	std::string m_strBt;
	std::string m_strInput_defend;
	std::string m_strBt_defend;
};
//////////////////////////////////////////////////////////////////////////
//crJXJDoAcceptInviteMethod
//////////////////////////////////////////////////////////////////////////
class crJXJDoAcceptInviteMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJDoAcceptInviteMethod();
	crJXJDoAcceptInviteMethod(const crJXJDoAcceptInviteMethod &handle);
	MethodClass(JXJ,JXJDoAcceptInvite)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJQureyRebelTimerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJQureyRebelTimerMethod:public CRCore::crMethod
{//Game /client Method
public:
	crJXJQureyRebelTimerMethod();
	crJXJQureyRebelTimerMethod(const crJXJQureyRebelTimerMethod &handle);
	MethodClass(JXJ,JXJQureyRebelTimer)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strCityMapCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetRoleFightingPowerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetRoleFightingPowerMethod:public CRCore::crMethod
{//Game /client Method
public:
	crJXJGetRoleFightingPowerMethod();
	crJXJGetRoleFightingPowerMethod(const crJXJGetRoleFightingPowerMethod &handle);
	MethodClass(JXJ,JXJGetRoleFightingPower)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem *m_this;
	float *m_fightingPower;
	//crFormationInfo *formationinfo;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetPlayerFightingPowerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetPlayerFightingPowerMethod:public CRCore::crMethod
{//Game /client Method
public:
	crJXJGetPlayerFightingPowerMethod();
	crJXJGetPlayerFightingPowerMethod(const crJXJGetPlayerFightingPowerMethod &handle);
	MethodClass(JXJ,JXJGetPlayerFightingPower)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int *m_fightingPower;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFightingPowerShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFightingPowerShowMethod:public CRCore::crMethod
{//Game /client Method
public:
	crJXJFightingPowerShowMethod();
	crJXJFightingPowerShowMethod(const crJXJFightingPowerShowMethod &handle);
	MethodClass(JXJ,JXJFightingPowerShow)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_str_1;
	std::string m_str_2;
	std::string m_str_3;
	std::string m_str_4;
	std::string m_str_5;
	std::string m_str_6;
	std::string m_str_7;
};
class crJXJRecvPlayerFightPowerMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvPlayerFightPowerMethod();
	crJXJRecvPlayerFightPowerMethod(const crJXJRecvPlayerFightPowerMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerFightPower)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJListCtrlScrollNewMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJListCtrlScrollNewMethod();
	crJXJListCtrlScrollNewMethod(const crJXJListCtrlScrollNewMethod& handle);
	MethodClass(JXJ, JXJListCtrlScrollNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_srollbar;
	unsigned char m_type;
};
class crJXJMouseSettingOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJMouseSettingOnShowMethod();
	crJXJMouseSettingOnShowMethod(const crJXJMouseSettingOnShowMethod& handle);
	MethodClass(JXJ, JXJMouseSettingOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strCheckBox;
	std::string m_progressBar;
};
class crJXJSaveMouseSettingMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSaveMouseSettingMethod();
	crJXJSaveMouseSettingMethod(const crJXJSaveMouseSettingMethod & handle);
	MethodClass(JXJ, JXJSaveMouseSetting)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_strCheckBox;
	std::string m_progressBar;
	std::string m_strTableName;
};
class crJXJCheckBoxSelectMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckBoxSelectMethod();
	crJXJCheckBoxSelectMethod(const crJXJCheckBoxSelectMethod & handle);
	MethodClass(JXJ, JXJCheckBoxSelect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	std::string m_strCheckBox;
	std::string m_newCanvas;
	std::string m_newCanvas_sw;
	unsigned char m_change;
};
class crJXJPlaySignalFxLogic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJPlaySignalFxLogic();
	crJXJPlaySignalFxLogic(const crJXJPlaySignalFxLogic & handle);
	LogicClass(JXJ, JXJPlaySignalFx)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNode> m_fxNode;
	CRCore::crVector3 m_targetPosition;
	std::string m_fileName;
	std::string m_flagName;
};
//class crJXJGameServerTimerMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJGameServerTimerMethod();
//	crJXJGameServerTimerMethod(const crJXJGameServerTimerMethod & handle);
//	MethodClass(JXJ, JXJGameServerTimer)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//	int m_timer;
//	int m_totalTimer;
//};
//class crJXJRecvServerTimerMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvServerTimerMethod();
//	crJXJRecvServerTimerMethod(const crJXJRecvServerTimerMethod & handle);
//	MethodClass(JXJ, JXJRecvServerTimer)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	int m_validTime;
//	std::string m_strCanvas;
//	std::string m_strInput;
//};
//class crJXJClientTimerUpdateMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJClientTimerUpdateMethod();
//	crJXJClientTimerUpdateMethod(const crJXJClientTimerUpdateMethod & handle);
//	MethodClass(JXJ, JXJClientTimerUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	//int m_netType;
//};
}
#endif