/**********************************************************************
*
*	文件:	   appBattleMethod.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPBATTLEMETHOD_H
#define JXJ_APPBATTLEMETHOD_H
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
#include <CRDownload/crDownload.h>
#include <JXJ/appData.h>
#include <JXJ/appData3.h>
namespace JXJ{
class crJXJCreateFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCreateFubenMethod();
	crJXJCreateFubenMethod(const crJXJCreateFubenMethod& handle);
	MethodClass(JXJ, JXJCreateFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJInitSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJInitSelectRoleMethod();
	crJXJInitSelectRoleMethod(const crJXJInitSelectRoleMethod& handle);
	MethodClass(JXJ, JXJInitSelectRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectRoleMethod();
	crJXJSelectRoleMethod(const crJXJSelectRoleMethod& handle);
	MethodClass(JXJ, JXJSelectRole)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	int m_index;
	CRProducer::crGUIEventAdapter* m_ea;
};
//class crJXJRecvBattleRMousePickMethod :public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crJXJRecvBattleRMousePickMethod();
//	crJXJRecvBattleRMousePickMethod(const crJXJRecvBattleRMousePickMethod& handle);
//	MethodClass(JXJ, JXJRecvBattleRMousePick)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJActUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJActUpdateMethod();
	crJXJActUpdateMethod(const crJXJActUpdateMethod& handle);
	MethodClass(JXJ, JXJActUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_dt;
	std::string m_currentAct;
	float m_attackDuration;
	int m_attacked;
	float m_deadActFactor;//0.1f
	std::vector<float> m_attackActRateVec;
	std::vector<float> m_critAttackActRateVec;
	std::vector<float> m_nearAttackActRateVec;
	bool m_init;
	void init();
	unsigned short m_lastAct;
	bool m_stagnate;
	float m_animSpeedrec;
};
class crJXJRecvItemUseMethod :public CRCore::crMethod
{//Client & SceneServer Method
public:
	crJXJRecvItemUseMethod();
	crJXJRecvItemUseMethod(const crJXJRecvItemUseMethod& handle);
	MethodClass(JXJ, JXJRecvItemUse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvItemUseUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvItemUseUpdateMethod();
	crJXJRecvItemUseUpdateMethod(const crJXJRecvItemUseUpdateMethod& handle);
	MethodClass(JXJ, JXJRecvItemUseUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJUseItemActMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUseItemActMethod();
	crJXJUseItemActMethod(const crJXJUseItemActMethod& handle);
	MethodClass(JXJ, JXJUseItemAct)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_cdtime;
	CRNetApp::crInstanceItem *m_item;
};
class crJXJTargetNearTestMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTargetNearTestMethod();
	crJXJTargetNearTestMethod(const crJXJTargetNearTestMethod& handle);
	MethodClass(JXJ, JXJTargetNearTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_dist;
	bool* m_isTargetNear;
};
class crJXJItemUseNoActMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJItemUseNoActMethod();
	crJXJItemUseNoActMethod(const crJXJItemUseNoActMethod& handle);
	MethodClass(JXJ, JXJItemUseNoAct)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJItemHurtActMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJItemHurtActMethod();
	crJXJItemHurtActMethod(const crJXJItemHurtActMethod& handle);
	MethodClass(JXJ, JXJItemHurtAct)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem *m_bulletItem;
	float m_hurtActTime;
};
//class crJXJLoginBattleSceneMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJLoginBattleSceneMethod();
//	crJXJLoginBattleSceneMethod(const crJXJLoginBattleSceneMethod& handle);
//	MethodClass(JXJ, JXJLoginBattleScene)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crJXJSearchBattleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchBattleMethod();
	crJXJSearchBattleMethod(const crJXJSearchBattleMethod& handle);
	MethodClass(JXJ, JXJSearchBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//class crJXJCreateBattleMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCreateBattleMethod();
//	crJXJCreateBattleMethod(const crJXJCreateBattleMethod& handle);
//	MethodClass(JXJ, JXJCreateBattle)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};
class crJXJTouchRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTouchRoleMethod();
	crJXJTouchRoleMethod(const crJXJTouchRoleMethod& handle);
	MethodClass(JXJ, JXJTouchRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crMatrixTransform *m_camera;
};
typedef std::vector <CRCore::ref_ptr<CRUI::crButtonWidgetNode>> BirthPointButtonVec;
class crJXJUISelectBirthPointInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectBirthPointInitMethod();
	crJXJUISelectBirthPointInitMethod(const crJXJUISelectBirthPointInitMethod& handle);
	MethodClass(JXJ, JXJUISelectBirthPointInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_timerWidget;
	std::string m_CityHallPoint;
	std::string m_battleMap;
	std::string m_StrongholdButton;
	std::string m_AttackButton;
	std::string m_DefenseButton;
	std::string m_StrongholdInput;
	std::string m_AttackInput;
	std::string m_DefenseInput;
	short m_selectTime;
	std::string m_strInfo;
	std::string m_strJuDian;
	std::string m_strRotMap;
};
class crJXJUISelectedBirthPointMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectedBirthPointMethod();
	crJXJUISelectedBirthPointMethod(const crJXJUISelectedBirthPointMethod& handle);
	MethodClass(JXJ, JXJUISelectedBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_birthPointRadioGroup;
};
class crJXJPlayerJoinRoomMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJPlayerJoinRoomMethod();
	crJXJPlayerJoinRoomMethod(const crJXJPlayerJoinRoomMethod& handle);
	MethodClass(JXJ, JXJPlayerJoinRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
};
class crJXJRecvBirthPointMethod :public CRCore::crMethod
{//SceneServer
public:
	crJXJRecvBirthPointMethod();
	crJXJRecvBirthPointMethod(const crJXJRecvBirthPointMethod& handle);
	MethodClass(JXJ, JXJRecvBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIMapViewportUpdateMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUIMapViewportUpdateMethod();
	crJXJUIMapViewportUpdateMethod(const crJXJUIMapViewportUpdateMethod& handle);
	MethodClass(JXJ, JXJUIMapViewportUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_mapWidget;
	std::string m_viewportIcon;
	std::string m_moveTargetIcon;
	std::string m_viewportParent;
	CRCore::crWidgetNode *m_viewportWidget;
};
class crJXJUIUpdateMapInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUIUpdateMapInfoMethod();
	crJXJUIUpdateMapInfoMethod(const crJXJUIUpdateMapInfoMethod& handle);
	MethodClass(JXJ, JXJUIUpdateMapInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_nameWidget;
	std::string m_mapWidget;
	std::string m_myIconWidget;
	std::string m_friendPlayerIconWidget;
	std::string m_threadPlayerIconWidget;
	std::string m_enemyPlayerIconWidget;

	std::string m_friendNpcIconWidget;
	std::string m_threadNpcIconWidget;
	std::string m_enemyNpcIconWidget;
	std::string m_enemyBossIconWidget;
	std::string m_enemySmallBossIconWidget;
	std::string m_itemIconWidget;

	std::string m_signalIcon;
	typedef std::vector< CRCore::crWidgetNode* >WidgetNodeVec;
	WidgetNodeVec m_myWidgetVec;
	WidgetNodeVec m_friendPlayerWidgetVec;
	WidgetNodeVec m_threadPlayerWidgetVec;
	WidgetNodeVec m_enemyPlayerWidgetVec;
	WidgetNodeVec m_friendNpcWidgetVec;
	WidgetNodeVec m_threadNpcWidgetVec;
	WidgetNodeVec m_enemyNpcWidgetVec;
	WidgetNodeVec m_enemyBossWidgetVec;
	WidgetNodeVec m_enemySmallBossWidgetVec;
	WidgetNodeVec m_itemWidgetVec;
	WidgetNodeVec m_signalWidgetVec;
	WidgetNodeVec m_myBulidingWidgetVec;
	WidgetNodeVec m_enemyBulidingWidgetVec;
	WidgetNodeVec m_thirdBulidingWidgetVec;

	std::string m_rotMapWidget;
	std::string m_myBulidingWidget;
	std::string m_enemyBulidingWidget;
	std::string m_thirdBulidingWidget;

	std::string m_otherIconRoot;
	std::string m_playerIconRoot;

	std::string m_buttonCanvas;
	std::string m_StrongholdButton;
	std::string m_AttackButton;
	std::string m_DefenseButton;
	std::vector< CRUI::crButtonWidgetNode*> m_strongholdWidgetVec;
	std::vector< CRUI::crButtonWidgetNode*> m_attackWidgetVec;
	std::vector< CRUI::crButtonWidgetNode*> m_defenseWidgetVec;
	
};
class crJXJMapHitMoveMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJMapHitMoveMethod();
	crJXJMapHitMoveMethod(const crJXJMapHitMoveMethod& handle);
	MethodClass(JXJ, JXJMapHitMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_rotMap;
	std::string m_miniMap;
	std::string m_signal;
};
class crJXJMapDragCameraMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJMapDragCameraMethod();
	crJXJMapDragCameraMethod(const crJXJMapDragCameraMethod& handle);
	MethodClass(JXJ, JXJMapDragCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_rotMap;
	std::string m_miniMap;
};
class crJXJSelectTargetRTTMethod : public CRCore::crMethod
{
public:
	crJXJSelectTargetRTTMethod();
	crJXJSelectTargetRTTMethod(const crJXJSelectTargetRTTMethod& handle);
	MethodClass(JXJ, JXJSelectTargetRTT)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_canvas;
	std::string m_imageBox;
	std::string m_backModel;
	CRCore::crVector2i m_texSize;
	float m_updInterval;
	CRCore::crVector2 m_distScale;
	CRCore::crRenderToTexture* m_rtt;
	int m_frameCounter;//
};
class crJXJSelectTargetDecalUpdateMethod : public CRCore::crMethod
{
public:
	crJXJSelectTargetDecalUpdateMethod();
	crJXJSelectTargetDecalUpdateMethod(const crJXJSelectTargetDecalUpdateMethod& handle);
	MethodClass(JXJ, JXJSelectTargetDecalUpdate)
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
class crJXJRecvRoleInfo2Method :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvRoleInfo2Method();
	crJXJRecvRoleInfo2Method(const crJXJRecvRoleInfo2Method& handle);
	MethodClass(JXJ, JXJRecvRoleInfo2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRNetApp::crSceneServerPlayerData* m_player;
	CRNetApp::crRole* m_role;
};
class crJXJSetInbornMethod :public CRCore::crMethod
{//SceneServer & Client Method 设置天赋
public:
	crJXJSetInbornMethod();
	crJXJSetInbornMethod(const crJXJSetInbornMethod& handle);
	MethodClass(JXJ, JXJSetInborn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void setMagicAttr(CRCore::crData *roleData,int startid,const CREncapsulation::crTableIO::StrVec &record);
	CRNetApp::crPlayerGameData* m_this;
	CRNetApp::crRole* m_role;
	CRCore::crVector2i m_card;
};
class crJXJGetResProductPercentMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetResProductPercentMethod();
	crJXJGetResProductPercentMethod(const crJXJGetResProductPercentMethod& handle);
	MethodClass(JXJ, JXJGetResProductPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
	unsigned char m_thingtype;
};
//////////////////数值公式
class crJXJGetBingFaMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetBingFaMethod();
	crJXJGetBingFaMethod(const crJXJGetBingFaMethod& handle);
	MethodClass(JXJ, JXJGetBingFa)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetZhenFaMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetZhenFaMethod();
	crJXJGetZhenFaMethod(const crJXJGetZhenFaMethod& handle);
	MethodClass(JXJ, JXJGetZhenFa)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetDongChaMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetDongChaMethod();
	crJXJGetDongChaMethod(const crJXJGetDongChaMethod& handle);
	MethodClass(JXJ, JXJGetDongCha)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetTongShuaiMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetTongShuaiMethod();
	crJXJGetTongShuaiMethod(const crJXJGetTongShuaiMethod& handle);
	MethodClass(JXJ, JXJGetTongShuai)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetLeaderPowerMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetLeaderPowerMethod();
	crJXJGetLeaderPowerMethod(const crJXJGetLeaderPowerMethod& handle);
	MethodClass(JXJ, JXJGetLeaderPower)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
//class crJXJGetTotalLeaderPowerMethod : public CRCore::crMethod
//{//GameServer & Client Method
//public:
//	crJXJGetTotalLeaderPowerMethod();
//	crJXJGetTotalLeaderPowerMethod(const crJXJGetTotalLeaderPowerMethod& handle);
//	MethodClass(JXJ, JXJGetTotalLeaderPower)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	float *m_output;
//};
//class crJXJGetAttackRangeExaltMethod : public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crJXJGetAttackRangeExaltMethod();
//	crJXJGetAttackRangeExaltMethod(const crJXJGetAttackRangeExaltMethod& handle);
//	MethodClass(JXJ, JXJGetAttackRangeExalt)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	float m_baseRange;
//	float *m_output;
//};
class crJXJGetSightRangeMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSightRangeMethod();
	crJXJGetSightRangeMethod(const crJXJGetSightRangeMethod& handle);
	MethodClass(JXJ, JXJGetSightRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetItemSightRangeMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetItemSightRangeMethod();
	crJXJGetItemSightRangeMethod(const crJXJGetItemSightRangeMethod& handle);
	MethodClass(JXJ, JXJGetItemSightRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetSpeedMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSpeedMethod();
	crJXJGetSpeedMethod(const crJXJGetSpeedMethod& handle);
	MethodClass(JXJ, JXJGetSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetStealthMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetStealthMethod();
	crJXJGetStealthMethod(const crJXJGetStealthMethod& handle);
	MethodClass(JXJ, JXJGetStealth)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetStealthSpyMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetStealthSpyMethod();
	crJXJGetStealthSpyMethod(const crJXJGetStealthSpyMethod& handle);
	MethodClass(JXJ, JXJGetStealthSpy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetDefendMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetDefendMethod();
	crJXJGetDefendMethod(const crJXJGetDefendMethod& handle);
	MethodClass(JXJ, JXJGetDefend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetMagicDefendMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetMagicDefendMethod();
	crJXJGetMagicDefendMethod(const crJXJGetMagicDefendMethod& handle);
	MethodClass(JXJ, JXJGetMagicDefend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetDamageMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetDamageMethod();
	crJXJGetDamageMethod(const crJXJGetDamageMethod& handle);
	MethodClass(JXJ, JXJGetDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetBuildDamageMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetBuildDamageMethod();
	crJXJGetBuildDamageMethod(const crJXJGetBuildDamageMethod& handle);
	MethodClass(JXJ, JXJGetBuildDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetHPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetHPMethod();
	crJXJGetHPMethod(const crJXJGetHPMethod& handle);
	MethodClass(JXJ, JXJGetHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetRTHPPercentMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetRTHPPercentMethod();
	crJXJGetRTHPPercentMethod(const crJXJGetRTHPPercentMethod& handle);
	MethodClass(JXJ, JXJGetRTHPPercent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
//class crJXJGetRTHPMethod : public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crJXJGetRTHPMethod();
//	crJXJGetRTHPMethod(const crJXJGetRTHPMethod& handle);
//	MethodClass(JXJ, JXJGetRTHP)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	float *m_output;
//};
class crJXJGetEquipAttrShortMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetEquipAttrShortMethod();
	crJXJGetEquipAttrShortMethod(const crJXJGetEquipAttrShortMethod& handle);
	MethodClass(JXJ, JXJGetEquipAttrShort)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_owner;
	float *m_output;
	int m_equipAttrID;
};
class crJXJGetEquipAttrUCharMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetEquipAttrUCharMethod();
	crJXJGetEquipAttrUCharMethod(const crJXJGetEquipAttrUCharMethod& handle);
	MethodClass(JXJ, JXJGetEquipAttrUChar)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_owner;
	unsigned char *m_output;
	int m_equipAttrID;
};
class crJXJGetEquipAttrIntMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetEquipAttrIntMethod();
	crJXJGetEquipAttrIntMethod(const crJXJGetEquipAttrIntMethod& handle);
	MethodClass(JXJ, JXJGetEquipAttrInt)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_owner;
	int *m_output;
	int m_equipAttrID;
};
class crJXJGetMPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetMPMethod();
	crJXJGetMPMethod(const crJXJGetMPMethod& handle);
	MethodClass(JXJ, JXJGetMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetHPRevertMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetHPRevertMethod();
	crJXJGetHPRevertMethod(const crJXJGetHPRevertMethod& handle);
	MethodClass(JXJ, JXJGetHPRevert)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_factor;
	float *m_output;
};
class crJXJGetMPRevertMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetMPRevertMethod();
	crJXJGetMPRevertMethod(const crJXJGetMPRevertMethod& handle);
	MethodClass(JXJ, JXJGetMPRevert)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_factor;
	float *m_output;
};
class crJXJGetCritMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetCritMethod();
	crJXJGetCritMethod(const crJXJGetCritMethod& handle);
	MethodClass(JXJ, JXJGetCrit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetParryMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetParryMethod();
	crJXJGetParryMethod(const crJXJGetParryMethod& handle);
	MethodClass(JXJ, JXJGetParry)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetDodgeMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetDodgeMethod();
	crJXJGetDodgeMethod(const crJXJGetDodgeMethod& handle);
	MethodClass(JXJ, JXJGetDodge)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetHitratioMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetHitratioMethod();
	crJXJGetHitratioMethod(const crJXJGetHitratioMethod& handle);
	MethodClass(JXJ, JXJGetHitratio)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetIgnoreDefendMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetIgnoreDefendMethod();
	crJXJGetIgnoreDefendMethod(const crJXJGetIgnoreDefendMethod& handle);
	MethodClass(JXJ, JXJGetIgnoreDefend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetIgnoreMagicDefendMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetIgnoreMagicDefendMethod();
	crJXJGetIgnoreMagicDefendMethod(const crJXJGetIgnoreMagicDefendMethod& handle);
	MethodClass(JXJ, JXJGetIgnoreMagicDefend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJAttackSpeedMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJAttackSpeedMethod();
	crJXJAttackSpeedMethod(const crJXJAttackSpeedMethod& handle);
	MethodClass(JXJ, JXJAttackSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
	float m_limit;
};
class crJXJSkillSpeedMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJSkillSpeedMethod();
	crJXJSkillSpeedMethod(const crJXJSkillSpeedMethod& handle);
	MethodClass(JXJ, JXJSkillSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
	float m_limit;
};
class crJXJGetGuiseStateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetGuiseStateMethod();
	crJXJGetGuiseStateMethod(const crJXJGetGuiseStateMethod& handle);
	MethodClass(JXJ, JXJGetGuiseState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	unsigned int *m_output;
};
class crJXJGetItemGuiseStateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetItemGuiseStateMethod();
	crJXJGetItemGuiseStateMethod(const crJXJGetItemGuiseStateMethod& handle);
	MethodClass(JXJ, JXJGetItemGuiseState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	unsigned int *m_output;
};
class crJXJGetRadiusMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetRadiusMethod();
	crJXJGetRadiusMethod(const crJXJGetRadiusMethod& handle);
	MethodClass(JXJ, JXJGetRadius)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
	float m_limit;
};
class crJXJGetAttackDistMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetAttackDistMethod();
	crJXJGetAttackDistMethod(const crJXJGetAttackDistMethod& handle);
	MethodClass(JXJ, JXJGetAttackDist)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetIgnoreDodgeMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetIgnoreDodgeMethod();
	crJXJGetIgnoreDodgeMethod(const crJXJGetIgnoreDodgeMethod& handle);
	MethodClass(JXJ, JXJGetIgnoreDodge)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	bool *m_output;
};
class crJXJGetCritSwordMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetCritSwordMethod();
	crJXJGetCritSwordMethod(const crJXJGetCritSwordMethod& handle);
	MethodClass(JXJ, JXJGetCritSword)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetAttackAbsorbHPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetAttackAbsorbHPMethod();
	crJXJGetAttackAbsorbHPMethod(const crJXJGetAttackAbsorbHPMethod& handle);
	MethodClass(JXJ, JXJGetAttackAbsorbHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetSkillAbsorbHPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSkillAbsorbHPMethod();
	crJXJGetSkillAbsorbHPMethod(const crJXJGetSkillAbsorbHPMethod& handle);
	MethodClass(JXJ, JXJGetSkillAbsorbHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetAttackDamageBackMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetAttackDamageBackMethod();
	crJXJGetAttackDamageBackMethod(const crJXJGetAttackDamageBackMethod& handle);
	MethodClass(JXJ, JXJGetAttackDamageBack)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetSkillDamageBackMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSkillDamageBackMethod();
	crJXJGetSkillDamageBackMethod(const crJXJGetSkillDamageBackMethod& handle);
	MethodClass(JXJ, JXJGetSkillDamageBack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetSputteringMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSputteringMethod();
	crJXJGetSputteringMethod(const crJXJGetSputteringMethod& handle);
	MethodClass(JXJ, JXJGetSputtering)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	_crInt32 *m_output;
};
class crJXJGetAttackDamageMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetAttackDamageMethod();
	crJXJGetAttackDamageMethod(const crJXJGetAttackDamageMethod& handle);
	MethodClass(JXJ, JXJGetAttackDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJGetSkillDamageMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetSkillDamageMethod();
	crJXJGetSkillDamageMethod(const crJXJGetSkillDamageMethod& handle);
	MethodClass(JXJ, JXJGetSkillDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
///////////////////////
class crJXJFireExtraAttackMethod : public CRCore::crMethod
{//SceneServer Method 攻击者处于GS_ExtraAttack
public:
	crJXJFireExtraAttackMethod();
	crJXJFireExtraAttackMethod(const crJXJFireExtraAttackMethod& handle);
	MethodClass(JXJ, JXJFireExtraAttack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJHitExtraAttackMethod : public CRCore::crMethod
{//SceneServer Method 受击者处于GS_ExtraAttack
public:
	crJXJHitExtraAttackMethod();
	crJXJHitExtraAttackMethod(const crJXJHitExtraAttackMethod& handle);
	MethodClass(JXJ, JXJHitExtraAttack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJBackstabExtraAttackMethod : public CRCore::crMethod
{//SceneServer Method 攻击者处于GS_Backstab,并且目标背对攻击者
public:
	crJXJBackstabExtraAttackMethod();
	crJXJBackstabExtraAttackMethod(const crJXJBackstabExtraAttackMethod& handle);
	MethodClass(JXJ, JXJBackstabExtraAttack)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJGetImmunoMethod : public CRCore::crMethod
{//SceneServer Method
//获得受击者免疫状态，0：无效，1：正常，2：物理免疫，3：魔法免疫，4：物理免疫+魔法免疫，5死亡或者凝滞
public:
	crJXJGetImmunoMethod();
	crJXJGetImmunoMethod(const crJXJGetImmunoMethod& handle);
	MethodClass(JXJ, JXJGetImmuno)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};

class crJXJAttackDamageMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害计算
	crJXJAttackDamageMethod();
	crJXJAttackDamageMethod(const crJXJAttackDamageMethod& handle);
	MethodClass(JXJ, JXJAttackDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJAttackExtraTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJAttackExtraTestMethod();
	crJXJAttackExtraTestMethod(const crJXJAttackExtraTestMethod& handle);
	MethodClass(JXJ, JXJAttackExtraTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	float m_damage;
};
class crJXJSkillExtraTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSkillExtraTestMethod();
	crJXJSkillExtraTestMethod(const crJXJSkillExtraTestMethod& handle);
	MethodClass(JXJ, JXJSkillExtraTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	float m_damage;
};
class crJXJExtraTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJExtraTestMethod();
	crJXJExtraTestMethod(const crJXJExtraTestMethod& handle);
	MethodClass(JXJ, JXJExtraTest)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	int m_msg;//WCH_UseSkillExtraTest WCH_DeadExtraTest	WCH_KillExtraTest
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJEquipAttackExtraTestMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害计算
	crJXJEquipAttackExtraTestMethod();
	crJXJEquipAttackExtraTestMethod(const crJXJEquipAttackExtraTestMethod& handle);
	MethodClass(JXJ, JXJEquipAttackExtraTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_hitrate;
};
class crJXJHitExtraTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJHitExtraTestMethod();
	crJXJHitExtraTestMethod(const crJXJHitExtraTestMethod& handle);
	MethodClass(JXJ, JXJHitExtraTest)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	int m_msg;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	float m_damage;
};
class crJXJDicesMethod : public CRCore::crMethod
{//Server Method
public://伤害计算
	crJXJDicesMethod();
	crJXJDicesMethod(const crJXJDicesMethod& handle);
	MethodClass(JXJ, JXJDices)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_rate;
};
class crJXJSkillDamageMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害计算
	crJXJSkillDamageMethod();
	crJXJSkillDamageMethod(const crJXJSkillDamageMethod& handle);
	MethodClass(JXJ, JXJSkillDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	unsigned short m_sputteringmode;//0:不溅射,1分摊溅射,2衰减溅射,3概率溅射
	unsigned short m_sputteringrange;//
	unsigned short m_sputteringparam;//衰减百分比或者概率百分比
};
class crJXJTargetExtraMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害计算
	crJXJTargetExtraMethod();
	crJXJTargetExtraMethod(const crJXJTargetExtraMethod& handle);
	MethodClass(JXJ, JXJTargetExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	short m_count;
	short m_maxcount;
};
class crJXJUserExtraMethod : public CRCore::crMethod
{//SceneServer ChildItem Method WCH_AttackExtraTest给攻击者上BUF
public://伤害计算
	crJXJUserExtraMethod();
	crJXJUserExtraMethod(const crJXJUserExtraMethod& handle);
	MethodClass(JXJ, JXJUserExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJExtraHandleMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJExtraHandleMethod();
	crJXJExtraHandleMethod(const crJXJExtraHandleMethod& handle);
	MethodClass(JXJ, JXJExtraHandle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	short m_extid;
	short m_extparam;
};
class crJXJRecvExtraHandleMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvExtraHandleMethod();
	crJXJRecvExtraHandleMethod(const crJXJRecvExtraHandleMethod& handle);
	MethodClass(JXJ, JXJRecvExtraHandle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRecvTargetExtraMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvTargetExtraMethod();
	crJXJRecvTargetExtraMethod(const crJXJRecvTargetExtraMethod& handle);
	MethodClass(CRNetApp, JXJRecvTargetExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRecvUserExtraMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvUserExtraMethod();
	crJXJRecvUserExtraMethod(const crJXJRecvUserExtraMethod& handle);
	MethodClass(CRNetApp, JXJRecvUserExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJComputeRTHPMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJComputeRTHPMethod();
	crJXJComputeRTHPMethod(const crJXJComputeRTHPMethod& handle);
	MethodClass(JXJ, JXJComputeRTHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJAddDamageMethod : public CRCore::crMethod
{//SceneServer Method, m_damage如果是负数则表示复活士兵
public:
	crJXJAddDamageMethod();
	crJXJAddDamageMethod(const crJXJAddDamageMethod& handle);
	MethodClass(JXJ, JXJAddDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	//float m_damage;
	DamagePair m_damagePair;//damage,type
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};
class crJXJSceneServerUpdateMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSceneServerUpdateMethod();
	crJXJSceneServerUpdateMethod(const crJXJSceneServerUpdateMethod& handle);
	MethodClass(JXJ, JXJSceneServerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float* m_dt;
	CRNetApp::crSceneServerPlayerData* m_player;
	float m_kickTime;
};
class crJXJRecvBattleValidateIDMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJRecvBattleValidateIDMethod();
	crJXJRecvBattleValidateIDMethod(const crJXJRecvBattleValidateIDMethod& handle);
	MethodClass(JXJ, JXJRecvBattleValidateID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvDamageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvDamageMethod();
	crJXJRecvDamageMethod(const crJXJRecvDamageMethod& handle);
	MethodClass(JXJ, JXJRecvDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJDamageTypeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJDamageTypeMethod();
	crJXJDamageTypeMethod(const crJXJDamageTypeMethod& handle);
	MethodClass(JXJ, JXJDamageType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	unsigned char m_damageType;
};
class crJXJRecvDamageTypeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvDamageTypeMethod();
	crJXJRecvDamageTypeMethod(const crJXJRecvDamageTypeMethod& handle);
	MethodClass(JXJ, JXJRecvDamageType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJTrackFireMissMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJTrackFireMissMethod();
	crJXJTrackFireMissMethod(const crJXJTrackFireMissMethod& handle);
	MethodClass(JXJ, JXJTrackFireMiss)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject *m_this;//firer
	CRPhysics::crBulletMatterObject *m_bullet;
	CRCore::crNode *m_collideNode;
	std::string m_canvas;
};
class crJXJCureHPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCureHPMethod();
	crJXJCureHPMethod(const crJXJCureHPMethod& handle);
	MethodClass(JXJ, JXJCureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_curehp;
	//CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};
class crJXJRecvCureHPMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvCureHPMethod();
	crJXJRecvCureHPMethod(const crJXJRecvCureHPMethod& handle);
	MethodClass(JXJ, JXJRecvCureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//class crJXJSceneSavePlayerDataMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJSceneSavePlayerDataMethod();
//	crJXJSceneSavePlayerDataMethod(const crJXJSceneSavePlayerDataMethod& handle);
//	MethodClass(JXJ, JXJSceneSavePlayerData)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//};
//class crJXJHitratioTestMethod : public CRCore::crMethod
//{//SceneServer & Method
//public:
//	crJXJHitratioTestMethod();
//	crJXJHitratioTestMethod(const crJXJHitratioTestMethod& handle);
//	MethodClass(JXJ, JXJHitratioTest)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//};
class crJXJMissCritTestMethod : public CRCore::crMethod
{//SceneServer & Method
public:
	crJXJMissCritTestMethod();
	crJXJMissCritTestMethod(const crJXJMissCritTestMethod& handle);
	MethodClass(JXJ, JXJMissCritTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJDodgeParryTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJDodgeParryTestMethod();
	crJXJDodgeParryTestMethod(const crJXJDodgeParryTestMethod& handle);
	MethodClass(JXJ, JXJDodgeParryTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
//class crJXJDodgeTestMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJDodgeTestMethod();
//	crJXJDodgeTestMethod(const crJXJDodgeTestMethod& handle);
//	MethodClass(JXJ, JXJDodgeTest)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//};
class crJXJRecvMissCritMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvMissCritMethod();
	crJXJRecvMissCritMethod(const crJXJRecvMissCritMethod& handle);
	MethodClass(JXJ, JXJRecvMissCrit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//class crJXJRecvCritMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJRecvCritMethod();
//	crJXJRecvCritMethod(const crJXJRecvCritMethod& handle);
//	MethodClass(JXJ, JXJRecvCrit)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
class crJXJRecvDodgeParryMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvDodgeParryMethod();
	crJXJRecvDodgeParryMethod(const crJXJRecvDodgeParryMethod& handle);
	MethodClass(JXJ, JXJRecvDodgeParry)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//class crJXJRecvDodgeMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJRecvDodgeMethod();
//	crJXJRecvDodgeMethod(const crJXJRecvDodgeMethod& handle);
//	MethodClass(JXJ, JXJRecvDodge)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
////////////////////////////////////////////
class crJXJHeadCanvasUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJHeadCanvasUpdateMethod();
	crJXJHeadCanvasUpdateMethod(const crJXJHeadCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJHeadCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_myCanvas;
	std::string m_friendPlayerCanvas;
	std::string m_thirdPlayerCanvas;
	std::string m_enemyPlayerCanvas;

	std::string m_friendNpcCanvas;
	std::string m_thirdNpcCanvas;
	std::string m_enemyNpcCanvas;
	std::string m_itemCanvas;
	std::string m_nameWidget;
	std::string m_strJKIcon;
	std::string m_hpWidget;
	std::string m_perHPWidget;
	std::string m_iconSW;
	std::string m_perHPWidget_0;
	std::string m_perHPWidget_1;
	std::string m_perHPWidget_2;
	std::string m_perHPWidget_Zhengting;
	std::string m_perHPWidget_JuDian;
	std::string m_nameNpc;
	std::string m_nameBoss;
	std::string m_nameTa;
	std::string m_strBossSw;
	typedef std::map<int,CRCore::ref_ptr<CRCore::crCanvasNode> >CanvasNodeMap;
	typedef std::map<_crInt64,CRCore::ref_ptr<CRCore::crCanvasNode> >PlayerCanvasNodeMap;
	CanvasNodeMap m_myCanvasMap;
	PlayerCanvasNodeMap m_friendPlayerCanvasMap;
	PlayerCanvasNodeMap m_thirdPlayerCanvasMap;
	PlayerCanvasNodeMap m_enemyPlayerCanvasMap;
	CanvasNodeMap m_friendNpcCanvasMap;
	CanvasNodeMap m_thirdNpcCanvasMap;
	CanvasNodeMap m_enemyNpcCanvasMap;
	CanvasNodeMap m_springNpcCanvasMap;
	CanvasNodeMap m_sourceCarNpcCanvasMap;
	CanvasNodeMap m_extraGNpcCanvasMap;
	CanvasNodeMap m_extraFNpcCanvasMap;
	CanvasNodeMap m_extraQNpcCanvasMap;

	typedef std::map<int,std::string > IconMap;
	IconMap m_jkIcon;
	//CanvasNodeMap m_itemCanvasMap;
	//CanvasNodeVec m_myCanvasVec;
	//CanvasNodeVec m_friendPlayerCanvasVec;
	//CanvasNodeVec m_thirdPlayerCanvasVec;
	//CanvasNodeVec m_enemyPlayerCanvasVec;
	//CanvasNodeVec m_friendNpcCanvasVec;
	//CanvasNodeVec m_thirdNpcCanvasVec;
	//CanvasNodeVec m_enemyNpcCanvasVec;
	//CanvasNodeVec m_itemCanvasVec;
	std::string m_strMapCanvas;
	std::string m_strCheckBox;
	std::string m_nameZhengting;
	std::string m_nameJuDian;
	std::string m_swJuDian;
	std::string m_itemSpringCanvas;
	std::string m_itemSourceCarCanvas;
	std::string m_itemSpringSw;
	std::string m_chengxiangziji;
	std::string m_chengxiangyou;
	std::string m_chengxiangdi;
	std::string m_ExtraGCanvas;
	std::string m_ExtraFCanvas;
	std::string m_ExtraQCanvas;
protected:
	void init();
	bool m_init;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_myCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_friendPlayerCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_thirdPlayerCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_enemyPlayerCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_friendNpcCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_thirdNpcCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_enemyNpcCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_springItemCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_souceCarItemCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_extraGCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_extraFCanvasTemp;
	CRCore::ref_ptr<CRCore::crCanvasNode > m_extraQCanvasTemp;
	typedef std::list<CRCore::ref_ptr<CRCore::crCanvasNode> >VisiableCanvasNodeList;
	VisiableCanvasNodeList m_visiableCanvasNodeList;
};
class crJXJSelectRoleUseSkillMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectRoleUseSkillMethod();
	crJXJSelectRoleUseSkillMethod(const crJXJSelectRoleUseSkillMethod& handle);
	MethodClass(JXJ, JXJSelectRoleUseSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	int m_index;
};
class crJXJGetRoleUseSkillMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGetRoleUseSkillMethod();
	crJXJGetRoleUseSkillMethod(const crJXJGetRoleUseSkillMethod& handle);
	MethodClass(JXJ, JXJGetRoleUseSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJHideUseSkillDecalMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHideUseSkillDecalMethod();
	crJXJHideUseSkillDecalMethod(const crJXJHideUseSkillDecalMethod& handle);
	MethodClass(JXJ, JXJHideUseSkillDecal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJUseSkillMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUseSkillMethod();
	crJXJUseSkillMethod(const crJXJUseSkillMethod& handle);
	MethodClass(JXJ, JXJUseSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJCancelUseSkillMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCancelUseSkillMethod();
	crJXJCancelUseSkillMethod(const crJXJCancelUseSkillMethod& handle);
	MethodClass(JXJ, JXJCancelUseSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJBattleFormationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleFormationMethod();
	crJXJBattleFormationMethod(const crJXJBattleFormationMethod& handle);
	MethodClass(JXJ, JXJBattleFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	char m_key;
};
class crJXJCameraFocusMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCameraFocusMethod();
	crJXJCameraFocusMethod(const crJXJCameraFocusMethod& handle);
	MethodClass(JXJ, JXJCameraFocus)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	bool m_onlyFocusMe;
};
class crJXJScenarioCameraFocusPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJScenarioCameraFocusPosMethod();
	crJXJScenarioCameraFocusPosMethod(const crJXJScenarioCameraFocusPosMethod& handle);
	MethodClass(JXJ, JXJScenarioCameraFocusPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector3 m_pos;
};
class crJXJScenarioCameraFocusMeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJScenarioCameraFocusMeMethod();
	crJXJScenarioCameraFocusMeMethod(const crJXJScenarioCameraFocusMeMethod& handle);
	MethodClass(JXJ, JXJScenarioCameraFocusMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJAttackToPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAttackToPosMethod();
	crJXJAttackToPosMethod(const crJXJAttackToPosMethod& handle);
	MethodClass(JXJ, JXJAttackToPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJIsAttackToPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsAttackToPosMethod();
	crJXJIsAttackToPosMethod(const crJXJIsAttackToPosMethod& handle);
	MethodClass(JXJ, JXJIsAttackToPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJCancelAttackToPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCancelAttackToPosMethod();
	crJXJCancelAttackToPosMethod(const crJXJCancelAttackToPosMethod& handle);
	MethodClass(JXJ, JXJCancelAttackToPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJCancelCameraOperatorMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCancelCameraOperatorMethod();
	crJXJCancelCameraOperatorMethod(const crJXJCancelCameraOperatorMethod& handle);
	MethodClass(JXJ, JXJCancelCameraOperator)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJRoleLoadDataAttrFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJRoleLoadDataAttrFromTabMethod();
	crJXJRoleLoadDataAttrFromTabMethod(const crJXJRoleLoadDataAttrFromTabMethod& handle);
	MethodClass(JXJ, JXJRoleLoadDataAttrFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	int m_tableid;
};
class crJXJRoleLoadInbornAttrFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJRoleLoadInbornAttrFromTabMethod();
	crJXJRoleLoadInbornAttrFromTabMethod(const crJXJRoleLoadInbornAttrFromTabMethod& handle);
	MethodClass(JXJ, JXJRoleLoadInbornAttrFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	int m_tableid;
	int m_inbornAbatractID;
};
class crJXJSkillLoadDataAttrFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJSkillLoadDataAttrFromTabMethod();
	crJXJSkillLoadDataAttrFromTabMethod(const crJXJSkillLoadDataAttrFromTabMethod& handle);
	MethodClass(JXJ, JXJSkillLoadDataAttrFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJEquipLoadDataAttrFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJEquipLoadDataAttrFromTabMethod();
	crJXJEquipLoadDataAttrFromTabMethod(const crJXJEquipLoadDataAttrFromTabMethod& handle);
	MethodClass(JXJ, JXJEquipLoadDataAttrFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJLoadNpcPatrolFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJLoadNpcPatrolFromTabMethod();
	crJXJLoadNpcPatrolFromTabMethod(const crJXJLoadNpcPatrolFromTabMethod& handle);
	MethodClass(JXJ, JXJLoadNpcPatrolFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJPlayerMoveToMethod : public CRCore::crMethod
{//Client Player Method
public:
	crJXJPlayerMoveToMethod();
	crJXJPlayerMoveToMethod(const crJXJPlayerMoveToMethod& handle);
	MethodClass(JXJ, JXJPlayerMoveTo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::crVector3 m_targetPos;
	bool m_inputPos;
};
class crJXJGetItemRTDataMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJGetItemRTDataMethod();
	crJXJGetItemRTDataMethod(const crJXJGetItemRTDataMethod& handle);
	MethodClass(JXJ, JXJGetItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> *m_outstream;
};
class crJXJRecvItemRTDataMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvItemRTDataMethod();
	crJXJRecvItemRTDataMethod(const crJXJRecvItemRTDataMethod& handle);
	MethodClass(JXJ, JXJRecvItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_strCanvas;
};
class crJXJSetFubenBirthPointMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetFubenBirthPointMethod();
	crJXJSetFubenBirthPointMethod(const crJXJSetFubenBirthPointMethod& handle);
	MethodClass(JXJ, JXJSetFubenBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_birthpoint;
};
class crJXJRecvFubenBirthPointMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvFubenBirthPointMethod();
	crJXJRecvFubenBirthPointMethod(const crJXJRecvFubenBirthPointMethod& handle);
	MethodClass(JXJ, JXJRecvFubenBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJAnimationEventTestMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJAnimationEventTestMethod();
	crJXJAnimationEventTestMethod(const crJXJAnimationEventTestMethod& handle);
	MethodClass(JXJ, JXJAnimationEventTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRCore::crAnimationPathCallback *m_animation;
	CRCore::crAnimationPath::ControlPoint *m_cp;
	std::string m_bot1;
	std::string m_bot2;
	CRCore::ref_ptr<rbody::CreBodyNode> m_body1;
	CRCore::ref_ptr<rbody::CreBodyNode> m_body2;
};
class crJXJTeamEnterRoomMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTeamEnterRoomMethod();
	crJXJTeamEnterRoomMethod(const crJXJTeamEnterRoomMethod& handle);
	MethodClass(JXJ, JXJTeamEnterRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvTeamEnterRoomMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvTeamEnterRoomMethod();
	crJXJRecvTeamEnterRoomMethod(const crJXJRecvTeamEnterRoomMethod& handle);
	MethodClass(JXJ, JXJRecvTeamEnterRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJPreEnterSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJPreEnterSceneRoomMethod();
	crJXJPreEnterSceneRoomMethod(const crJXJPreEnterSceneRoomMethod& handle);
	MethodClass(JXJ, JXJPreEnterSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJGameServerDropedMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerDropedMethod();
	crJXJGameServerDropedMethod(const crJXJGameServerDropedMethod& handle);
	MethodClass(JXJ, JXJGameServerDroped)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::crGameServerPlayerData *m_playerData;
};
class crJXJSceneServerDropedMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSceneServerDropedMethod();
	crJXJSceneServerDropedMethod(const crJXJSceneServerDropedMethod& handle);
	MethodClass(JXJ, JXJSceneServerDroped)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::crSceneServerPlayerData *m_playerData;
};
class crJXJRecvLeaveSceneMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvLeaveSceneMethod();
	crJXJRecvLeaveSceneMethod(const crJXJRecvLeaveSceneMethod& handle);
	MethodClass(JXJ, JXJRecvLeaveScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	CRCore::crStreamBuf * m_stream;
};
class crJXJLeaveBattleAndTeamMethod :public CRCore::crMethod
{//GameServer离开战场和组队
public:
	crJXJLeaveBattleAndTeamMethod();
	crJXJLeaveBattleAndTeamMethod(const crJXJLeaveBattleAndTeamMethod& handle);
	MethodClass(JXJ, JXJLeaveBattleAndTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJClientSceneServerAbnormalMethod :public CRCore::crMethod
{//GameClient
public:
	crJXJClientSceneServerAbnormalMethod();
	crJXJClientSceneServerAbnormalMethod(const crJXJClientSceneServerAbnormalMethod& handle);
	MethodClass(JXJ, JXJClientSceneServerAbnormal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJBattleRoomAheadCloseMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBattleRoomAheadCloseMethod();
	crJXJBattleRoomAheadCloseMethod(const crJXJBattleRoomAheadCloseMethod& handle);
	MethodClass(JXJ, JXJBattleRoomAheadClose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
};
//class crJXJBattleEnemyCheckMethod :public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJBattleEnemyCheckMethod();
//	crJXJBattleEnemyCheckMethod(const crJXJBattleEnemyCheckMethod& handle);
//	MethodClass(JXJ, JXJBattleEnemyCheck)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	std::pair<CRNetApp::crInstanceItem *,CRNetApp::crInstanceItem *> *m_itempair;
//	char *m_isEnemy;
//};
//class crJXJBattleDefenceSightMethod :public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJBattleDefenceSightMethod();
//	crJXJBattleDefenceSightMethod(const crJXJBattleDefenceSightMethod& handle);
//	MethodClass(JXJ, JXJBattleDefenceSight)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	typedef std::vector<int>IDVec;
//	IDVec m_itemidVec;
//};
//class crJXJBattleDefenceSightFromTabMethod :public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJBattleDefenceSightFromTabMethod();
//	crJXJBattleDefenceSightFromTabMethod(const crJXJBattleDefenceSightFromTabMethod& handle);
//	MethodClass(JXJ, JXJBattleDefenceSightFromTab)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	int m_id;
//};
//class crJXJCreateNpcSightFromTabMethod :public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJCreateNpcSightFromTabMethod();
//	crJXJCreateNpcSightFromTabMethod(const crJXJCreateNpcSightFromTabMethod& handle);
//	MethodClass(JXJ, JXJCreateNpcSightFromTab)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	int m_id;
//	unsigned char m_camp;
//};
class crJXJLoadBattleRoomDataMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadBattleRoomDataMethod();
	crJXJLoadBattleRoomDataMethod(const crJXJLoadBattleRoomDataMethod& handle);
	MethodClass(JXJ, JXJLoadBattleRoomData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJLoadFubenRoomDataMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadFubenRoomDataMethod();
	crJXJLoadFubenRoomDataMethod(const crJXJLoadFubenRoomDataMethod& handle);
	MethodClass(JXJ, JXJLoadFubenRoomData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJBattleKillStatsMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBattleKillStatsMethod();
	crJXJBattleKillStatsMethod(const crJXJBattleKillStatsMethod& handle);
	MethodClass(JXJ, JXJBattleKillStats)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	std::pair<CRNetApp::crInstanceItem*,CRNetApp::crInstanceItem*> *m_itempair;//deaditem,fireitem
	short m_deadArmy;
};
class crJXJBattlePlayerLeaveMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBattlePlayerLeaveMethod();
	crJXJBattlePlayerLeaveMethod(const crJXJBattlePlayerLeaveMethod& handle);
	MethodClass(JXJ, JXJBattlePlayerLeave)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_playerid;
};

class crJXJFubenPlayerLeaveMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJFubenPlayerLeaveMethod();
	crJXJFubenPlayerLeaveMethod(const crJXJFubenPlayerLeaveMethod& handle);
	MethodClass(JXJ, JXJFubenPlayerLeave)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_playerid;
};

struct PlayerBattleInfo
{
	int playerID;
	float playerExperience;
};
typedef std::multimap<float,PlayerBattleInfo,std::greater<float>> BattleSettleMap;//playerscore,

class crJXJBattleSettleMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBattleSettleMethod();
	crJXJBattleSettleMethod(const crJXJBattleSettleMethod& handle);
	MethodClass(JXJ, JXJBattleSettle)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	short m_progress;
};
class crJXJRecvBattleSettleMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBattleSettleMethod();
	crJXJRecvBattleSettleMethod(const crJXJRecvBattleSettleMethod& handle);
	MethodClass(JXJ, JXJRecvBattleSettle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvBattleSettleMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJClientRecvBattleSettleMethod();
	crJXJClientRecvBattleSettleMethod(const crJXJClientRecvBattleSettleMethod& handle);
	MethodClass(JXJ, JXJClientRecvBattleSettle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvas;
	std::string m_frameTitle;
	std::string m_settleList;
	//BattleSettleMap m_BattleSettleMapWinner;
	//BattleSettleMap m_BattleSettleMapLoser;
	std::string m_Title1;
	std::string m_Title2;
	std::string m_Title3;
	std::string m_Title4;
	std::string m_count1;
	std::string m_count2;
	std::string m_count3;
	std::string m_count4;
	std::string m_switch;
	float m_charactersize;
	float m_sizex,m_sizey;
	float m_linespacing;
	float m_underlinespacing;
	float m_uplinespacing;
	std::string m_list[5];
};
class crJXJFubenSettleMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJFubenSettleMethod();
	crJXJFubenSettleMethod(const crJXJFubenSettleMethod& handle);
	MethodClass(JXJ, JXJFubenSettle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	short m_progress;
};
class crJXJRecvFubenSettleMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvFubenSettleMethod();
	crJXJRecvFubenSettleMethod(const crJXJRecvFubenSettleMethod& handle);
	MethodClass(JXJ, JXJRecvFubenSettle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJClientRecvFubenSettleMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJClientRecvFubenSettleMethod();
	crJXJClientRecvFubenSettleMethod(const crJXJClientRecvFubenSettleMethod& handle);
	MethodClass(JXJ, JXJClientRecvFubenSettle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvas;
	std::string m_frameTitle;
	std::string m_VorFInput;
	std::string m_experienceInput;
	std::string m_magicBoxCountInput;
	std::string m_magicBoxImage;
	std::string m_btnAgainSwitch;
	std::string m_archimentInput;
};
//////////////////=关隘结算//////////////////////////////////
//class crJXJPassSettleMethod :public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJPassSettleMethod();
//	crJXJPassSettleMethod(const crJXJPassSettleMethod& handle);
//	MethodClass(JXJ, JXJPassSettle)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	short m_progress;
//};
//class crJXJRecvPassSettleMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvPassSettleMethod();
//	crJXJRecvPassSettleMethod(const crJXJRecvPassSettleMethod& handle);
//	MethodClass(JXJ, JXJRecvPassSettle)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
//class crJXJClientRecvPassSettleMethod :public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJClientRecvPassSettleMethod();
//	crJXJClientRecvPassSettleMethod(const crJXJClientRecvPassSettleMethod& handle);
//	MethodClass(JXJ, JXJClientRecvPassSettle)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	std::string m_canvas;
//	std::string m_frameTitle;
//	std::string m_VorFInput;
//	std::string m_experienceInput;
//	std::string m_magicBoxCountInput;
//	std::string m_magicBoxImage;
//	std::string m_btnAgain;
//};
///////////////////////////////////////////////////////////
class crJXJCameraOccludedMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJCameraOccludedMethod();
	crJXJCameraOccludedMethod(const crJXJCameraOccludedMethod& handle);
	MethodClass(JXJ, JXJCameraOccluded)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_interval;
	float m_alpha;
	float m_speed;
};
class crJXJReliveItemsMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJReliveItemsMethod();
	crJXJReliveItemsMethod(const crJXJReliveItemsMethod& handle);
	MethodClass(JXJ, JXJReliveItems)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	typedef std::vector< std::pair<int,int> >IDVec;//id,count
	IDVec m_itemidVec;
};
class crJXJReliveItemsFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJReliveItemsFromTabMethod();
	crJXJReliveItemsFromTabMethod(const crJXJReliveItemsFromTabMethod& handle);
	MethodClass(JXJ, JXJReliveItemsFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_id;
	bool m_clone;//是否克隆 默认0
	std::string m_varname;//NPC能力加成变量名
	CRCore::crVector3 m_varparam;//数量系数,HP系数,攻击系数
	bool m_idType;//m_id来源类型
	int m_boshu;//只在军团副本中使用，其他时候初始化为0
};
class crJXJCheckItemsDeadFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckItemsDeadFromTabMethod();
	crJXJCheckItemsDeadFromTabMethod(const crJXJCheckItemsDeadFromTabMethod& handle);
	MethodClass(JXJ, JXJCheckItemsDeadFromTab)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_id;
};
class crJXJCheckItemsDeadFromTab2Method : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckItemsDeadFromTab2Method();
	crJXJCheckItemsDeadFromTab2Method(const crJXJCheckItemsDeadFromTab2Method& handle);
	MethodClass(JXJ, JXJCheckItemsDeadFromTab2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	int m_id;
};
class crJXJNpcPatrolMethod : public CRCore::crMethod
{//SceneServer
public:
	crJXJNpcPatrolMethod();
	crJXJNpcPatrolMethod(const crJXJNpcPatrolMethod& handle);
	MethodClass(JXJ, JXJNpcPatrol)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	int m_taskPointRange;
};
class crJXJRolePatrolMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRolePatrolMethod();
	crJXJRolePatrolMethod(const crJXJRolePatrolMethod& handle);
	MethodClass(JXJ, JXJRolePatrol)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_taskPointRange;
};
class crJXJRecvServerListMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvServerListMethod();
	crJXJRecvServerListMethod(const crJXJRecvServerListMethod& handle);
	MethodClass(JXJ, JXJRecvServerList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvas;
	std::string m_serverNo;
	std::string m_serverName;
	std::string m_stats;
	std::string m_telecom;
	std::string m_cnc;
};
class crJXJEnterServerMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJEnterServerMethod();
	crJXJEnterServerMethod(const crJXJEnterServerMethod& handle);
	MethodClass(JXJ, JXJEnterServer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_isp;
	int m_btnid;
};
class crJXJWebLoginEnterServerMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJWebLoginEnterServerMethod();
	crJXJWebLoginEnterServerMethod(const crJXJWebLoginEnterServerMethod& handle);
	MethodClass(JXJ, JXJWebLoginEnterServer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvConnectServerMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvConnectServerMethod();
	crJXJRecvConnectServerMethod(const crJXJRecvConnectServerMethod& handle);
	MethodClass(CRNetApp, JXJRecvConnectServer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJFileVersioncheckThread: public CRCore::crThread, public CRCore::Referenced
{
public:
	crJXJFileVersioncheckThread();
	virtual void run();
	int getDone(){ return m_done; }
	float getProgress() { return m_progress; }
protected:
	virtual ~crJXJFileVersioncheckThread();
	bool m_done;
	float m_progress;
};
class crJXJMoveFileToDirectoryThread: public CRCore::crThread, public CRCore::Referenced
{
public:
	crJXJMoveFileToDirectoryThread();
	virtual void run();
	int getCase(){ return m_case; }
protected:
	virtual ~crJXJMoveFileToDirectoryThread();
	int m_case;
	bool binExtension(const std::string& extension) const;
};
class crJXJGameSetupThread: public CRCore::crThread, public CRCore::Referenced
{
public:
	crJXJGameSetupThread(const std::string &setupfile);
	virtual void run();
	int getDone(){ return m_done; }
protected:
	virtual ~crJXJGameSetupThread();
	bool m_done;
	std::string m_setupfile;
};
class crJXJStartupDownloadCaseMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJStartupDownloadCaseMethod();
	crJXJStartupDownloadCaseMethod(const crJXJStartupDownloadCaseMethod& handle);
	MethodClass(JXJ, JXJStartupDownloadCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool binExtension(const std::string& extension) const;
	int readVersion(const std::string &filename);
	//void fileVersioncheck();
	bool m_needRestart;
	int m_progress;
	std::string m_verini;
	//std::string m_startuptab;
	std::string m_versiontab;
	CRCore::ref_ptr<CRDownload::crDownload>m_download1;
	std::string m_canvasName;
	std::string m_progressTextWidget;
	std::string m_progressWidget;
	std::string m_progressHead;
	CRCore::crVector2 m_visiableRange;
	CRCore::ref_ptr<crJXJFileVersioncheckThread>m_fileversioncheckthread;
	CRCore::ref_ptr<crJXJMoveFileToDirectoryThread>m_movefilethread;
	CRCore::ref_ptr<crJXJGameSetupThread>m_gamesetupthread;
	float *m_dt;
	float m_speed;
	std::string m_setupfile;
};
class crJXJShowVersionMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJShowVersionMethod();
	crJXJShowVersionMethod(const crJXJShowVersionMethod& handle);
	MethodClass(JXJ, JXJShowVersion)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_versionCanvas;
	std::string m_newVersion;
	std::string m_iVersion;
};
class crJXJCheckAndDownloadFileMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJCheckAndDownloadFileMethod();
	crJXJCheckAndDownloadFileMethod(const crJXJCheckAndDownloadFileMethod& handle);
	MethodClass(JXJ, JXJCheckAndDownloadFile)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fileName;
	//unsigned short m_downloadFlg;
	int m_downloadFlg;
	bool m_downloadPri;
};
class crJXJUIDownloadUpdateMethod :public CRCore::crMethod
{//GameClient Method，下载任务的UI显示
public:
	crJXJUIDownloadUpdateMethod();
	crJXJUIDownloadUpdateMethod(const crJXJUIDownloadUpdateMethod& handle);
	MethodClass(JXJ, JXJUIDownloadUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_progressTextWidget;
	std::string m_progressWidget;
	std::string m_progressHead;
	CRCore::crVector2 m_visiableRange;
private:
	CRCore::ref_ptr<CRDownload::crDownload>m_download1;
};
class crJXJAddDownloadTaskMethod : public CRCore::crMethod
{//GameClient Method 普通更新下载，在领地场景时候开启
public:
	crJXJAddDownloadTaskMethod();
	crJXJAddDownloadTaskMethod(const crJXJAddDownloadTaskMethod& handle);
	MethodClass(JXJ, JXJAddDownloadTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_count;
};
class crJXJInitSceneMapMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJInitSceneMapMethod();
	crJXJInitSceneMapMethod(const crJXJInitSceneMapMethod& handle);
	MethodClass(JXJ, JXJInitSceneMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_mapWidgetName;
	std::string m_imageFile;
	float m_scale;
	CRCore::crVector2 m_scaleRange;
	//std::string m_myIconWidget;
	float m_imageSize;
	bool m_staticmap;
	std::string m_rotmapWidget;
};
class crJXJDamenCollideMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJDamenCollideMethod();
	crJXJDamenCollideMethod(const crJXJDamenCollideMethod& handle);
	MethodClass(JXJ, JXJDamenCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_item;
};
class crJXJRecvCollideStateMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJRecvCollideStateMethod();
	crJXJRecvCollideStateMethod(const crJXJRecvCollideStateMethod& handle);
	MethodClass(JXJ, JXJRecvCollideState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJDamenServerUpdateMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJDamenServerUpdateMethod();
	crJXJDamenServerUpdateMethod(const crJXJDamenServerUpdateMethod& handle);
	MethodClass(JXJ, JXJDamenServerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	unsigned int m_laststate;
	float *m_dt;
	float m_nonblockTimer;
	float m_nonblockInterval;
};
class crJXJDamenClientUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJDamenClientUpdateMethod();
	crJXJDamenClientUpdateMethod(const crJXJDamenClientUpdateMethod& handle);
	MethodClass(JXJ, JXJDamenClientUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJCloseTargetItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJCloseTargetItemMethod();
	crJXJCloseTargetItemMethod(const crJXJCloseTargetItemMethod& handle);
	MethodClass(JXJ, JXJCloseTargetItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_recloseDist;
	CRNetApp::crInstanceItem *m_lastTargetItem;
	CRCore::crVector3 m_lastTargetPos;
};
class crJXJSaveFubenProgressMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSaveFubenProgressMethod();
	crJXJSaveFubenProgressMethod(const crJXJSaveFubenProgressMethod& handle);
	MethodClass(JXJ, JXJSaveFubenProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_lastprogress;
};
class crJXJRecvSaveFubenProgressMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvSaveFubenProgressMethod();
	crJXJRecvSaveFubenProgressMethod(const crJXJRecvSaveFubenProgressMethod& handle);
	MethodClass(JXJ, JXJRecvSaveFubenProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSetFubenIDMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetFubenIDMethod();
	crJXJSetFubenIDMethod(const crJXJSetFubenIDMethod& handle);
	MethodClass(JXJ, JXJSetFubenID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvFubenIDMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvFubenIDMethod();
	crJXJRecvFubenIDMethod(const crJXJRecvFubenIDMethod& handle);
	MethodClass(JXJ, JXJRecvFubenID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJRecvPlayerLeaveSceneMethod :public CRCore::crMethod
//{//GameClient & GameServer Method
//public:
//	crJXJRecvPlayerLeaveSceneMethod();
//	crJXJRecvPlayerLeaveSceneMethod(const crJXJRecvPlayerLeaveSceneMethod& handle);
//	MethodClass(JXJ, JXJRecvPlayerLeaveScene)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJCheckFubenProgressMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCheckFubenProgressMethod();
//	crJXJCheckFubenProgressMethod(const crJXJCheckFubenProgressMethod& handle);
//	MethodClass(JXJ, JXJCheckFubenProgress)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crJXJSetFubenProgressMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSetFubenProgressMethod();
	crJXJSetFubenProgressMethod(const crJXJSetFubenProgressMethod& handle);
	MethodClass(JXJ, JXJSetFubenProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRNetApp::crSceneServerPlayerData *m_creator;
};
class crJXJFubenProgressAdvanceCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFubenProgressAdvanceCheckMethod();
	crJXJFubenProgressAdvanceCheckMethod(const crJXJFubenProgressAdvanceCheckMethod& handle);
	MethodClass(JXJ, JXJFubenProgressAdvanceCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	short m_progress;
};
class crJXJGainGiftsPackKeyMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJGainGiftsPackKeyMethod();
	crJXJGainGiftsPackKeyMethod(const crJXJGainGiftsPackKeyMethod& handle);
	MethodClass(JXJ, JXJGainGiftsPackKey)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string *m_output;
	unsigned short m_giftKey;
	unsigned short m_lastsavekey;
};
class crJXJCreateGiftsPackDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreateGiftsPackDisposeMethod();
	crJXJCreateGiftsPackDisposeMethod(const crJXJCreateGiftsPackDisposeMethod& handle);
	MethodClass(JXJ, JXJCreateGiftsPackDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_availableTime;
};
class crJXJQueryGiftsPackDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryGiftsPackDisposeMethod();
	crJXJQueryGiftsPackDisposeMethod(const crJXJQueryGiftsPackDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryGiftsPackDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector<CRCore::ref_ptr<crQueryGiftsPack> > m_taskVec;
	//float m_availableTime;
};
class crJXJGameServerMaintenanceMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerMaintenanceMethod();
	crJXJGameServerMaintenanceMethod(const crJXJGameServerMaintenanceMethod& handle);
	MethodClass(JXJ, JXJGameServerMaintenance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	time_t m_lasttimer;
};
class crJXJRankingSaveMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRankingSaveMethod();
	crJXJRankingSaveMethod(const crJXJRankingSaveMethod& handle);
	MethodClass(JXJ, JXJRankingSave)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJGiftspackTabMaintenanceMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGiftspackTabMaintenanceMethod();
	crJXJGiftspackTabMaintenanceMethod(const crJXJGiftspackTabMaintenanceMethod& handle);
	MethodClass(JXJ, JXJGiftspackTabMaintenance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJMailTabMaintenanceMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJMailTabMaintenanceMethod();
	crJXJMailTabMaintenanceMethod(const crJXJMailTabMaintenanceMethod& handle);
	MethodClass(JXJ, JXJMailTabMaintenance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJGamelogMaintenanceMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGamelogMaintenanceMethod();
	crJXJGamelogMaintenanceMethod(const crJXJGamelogMaintenanceMethod& handle);
	MethodClass(JXJ, JXJGamelogMaintenance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJExtraMoveSpeedPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraMoveSpeedPercentMethod();
	crJXJExtraMoveSpeedPercentMethod(const crJXJExtraMoveSpeedPercentMethod& handle);
	MethodClass(JXJ, JXJExtraMoveSpeedPercent)
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
	short m_overlapCount;
};
class crJXJExtraAttackPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackPercentMethod();
	crJXJExtraAttackPercentMethod(const crJXJExtraAttackPercentMethod& handle);
	MethodClass(JXJ, JXJExtraAttackPercent)
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
	short m_overlapCount;
};
class crJXJExtraGuiseMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraGuiseMethod();
	crJXJExtraGuiseMethod(const crJXJExtraGuiseMethod& handle);
	MethodClass(JXJ, JXJExtraGuise)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	unsigned int m_extraValue;

	bool m_start;
	float m_timer;
};
class crJXJExtraGuiseCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 当玩家具有该状态时返回true,否则返回false
public:
	crJXJExtraGuiseCheckMethod();
	crJXJExtraGuiseCheckMethod(const crJXJExtraGuiseCheckMethod& handle);
	MethodClass(JXJ, JXJExtraGuiseCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	unsigned int m_extraValue;
};
class crJXJExtraDefendPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraDefendPercentMethod();
	crJXJExtraDefendPercentMethod(const crJXJExtraDefendPercentMethod& handle);
	MethodClass(JXJ, JXJExtraDefendPercent)
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
	short m_overlapCount;
};
class crJXJExtraDefendPerSecondMethod : public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少点防御
public:
	crJXJExtraDefendPerSecondMethod();
	crJXJExtraDefendPerSecondMethod(const crJXJExtraDefendPerSecondMethod& handle);
	MethodClass(JXJ, JXJExtraDefendPerSecond)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
};
class crJXJExtraMagicDefendPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraMagicDefendPercentMethod();
	crJXJExtraMagicDefendPercentMethod(const crJXJExtraMagicDefendPercentMethod& handle);
	MethodClass(JXJ, JXJExtraMagicDefendPercent)
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
class crJXJExtraDamageMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJExtraDamageMethod();
	crJXJExtraDamageMethod(const crJXJExtraDamageMethod& handle);
	MethodClass(JXJ, JXJExtraDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_extraValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_overlapCount;
};
class crJXJExtraDamageAttackMethod : public CRCore::crMethod
{//SceneServer Method每秒受到相当于普通攻击百分比的伤害
public:
	crJXJExtraDamageAttackMethod();
	crJXJExtraDamageAttackMethod(const crJXJExtraDamageAttackMethod& handle);
	MethodClass(JXJ, JXJExtraDamageAttack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_extraValue;//百分比
	float m_baseDamage;//基础伤害

	bool m_start;
	float m_timer;
	float m_dtrec;
	float m_firedamage;
};
class crJXJExtraDamageAttackOnceMethod : public CRCore::crMethod
{//SceneServer Method 立即受到相当于普通攻击百分比的伤害，可用于判断寄生伤害
public:
	crJXJExtraDamageAttackOnceMethod();
	crJXJExtraDamageAttackOnceMethod(const crJXJExtraDamageAttackOnceMethod& handle);
	MethodClass(JXJ, JXJExtraDamageAttackOnce)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_extraValue;//百分比
	float m_baseDamage;//基础伤害

	char m_start;
	float m_firedamage;
};
class crJXJExtraCureHPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJExtraCureHPMethod();
	crJXJExtraCureHPMethod(const crJXJExtraCureHPMethod& handle);
	MethodClass(JXJ, JXJExtraCureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_extraValue;
	float m_percent;

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_overlapCount;
	float m_maxhp;
};
class crJXJExtraDodgePercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraDodgePercentMethod();
	crJXJExtraDodgePercentMethod(const crJXJExtraDodgePercentMethod& handle);
	MethodClass(JXJ, JXJExtraDodgePercent)
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
	short m_overlapCount;
};
class crJXJExtraDodgeMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraDodgeMethod();
	crJXJExtraDodgeMethod(const crJXJExtraDodgeMethod& handle);
	MethodClass(JXJ, JXJExtraDodge)
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
	short m_overlapCount;
};
class crJXJExtraAttackSpeedMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackSpeedMethod();
	crJXJExtraAttackSpeedMethod(const crJXJExtraAttackSpeedMethod& handle);
	MethodClass(JXJ, JXJExtraAttackSpeed)
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
	short m_overlapCount;
};

class crJXJExtraSkillSpeedMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSkillSpeedMethod();
	crJXJExtraSkillSpeedMethod(const crJXJExtraSkillSpeedMethod& handle);
	MethodClass(JXJ, JXJExtraSkillSpeed)
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
	short m_overlapCount;
};

class crJXJExtraHPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraHPMethod();
	crJXJExtraHPMethod(const crJXJExtraHPMethod& handle);
	MethodClass(JXJ, JXJExtraHP)
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
	short m_overlapCount;
};

class crJXJExtraRTHPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraRTHPMethod();
	crJXJExtraRTHPMethod(const crJXJExtraRTHPMethod& handle);
	MethodClass(JXJ, JXJExtraRTHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	bool* m_output;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

class crJXJExtraIgnoreDefendMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraIgnoreDefendMethod();
	crJXJExtraIgnoreDefendMethod(const crJXJExtraIgnoreDefendMethod& handle);
	MethodClass(JXJ, JXJExtraIgnoreDefend)
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
	short m_overlapCount;
};

class crJXJExtraIgnoreMagicDefendMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraIgnoreMagicDefendMethod();
	crJXJExtraIgnoreMagicDefendMethod(const crJXJExtraIgnoreMagicDefendMethod& handle);
	MethodClass(JXJ, JXJExtraIgnoreMagicDefend)
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
	short m_overlapCount;
};
class crJXJExtraAttackDamageBackMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackDamageBackMethod();
	crJXJExtraAttackDamageBackMethod(const crJXJExtraAttackDamageBackMethod& handle);
	MethodClass(JXJ, JXJExtraAttackDamageBack)
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
	short m_overlapCount;
};

class crJXJExtraSkillDamageBackMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSkillDamageBackMethod();
	crJXJExtraSkillDamageBackMethod(const crJXJExtraSkillDamageBackMethod& handle);
	MethodClass(JXJ, JXJExtraSkillDamageBack)
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
	short m_overlapCount;
};

class crJXJExtraAttackAbsorbHPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackAbsorbHPMethod();
	crJXJExtraAttackAbsorbHPMethod(const crJXJExtraAttackAbsorbHPMethod& handle);
	MethodClass(JXJ, JXJExtraAttackAbsorbHP)
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
	short m_overlapCount;
};

class crJXJExtraSkillAbsorbHPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSkillAbsorbHPMethod();
	crJXJExtraSkillAbsorbHPMethod(const crJXJExtraSkillAbsorbHPMethod& handle);
	MethodClass(JXJ, JXJExtraSkillAbsorbHP)
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
	short m_overlapCount;
};

class crJXJExtraSkillDemageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSkillDemageMethod();
	crJXJExtraSkillDemageMethod(const crJXJExtraSkillDemageMethod& handle);
	MethodClass(JXJ, JXJExtraSkillDemage)
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
	short m_overlapCount;
};

class crJXJExtraAttackDemageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackDemageMethod();
	crJXJExtraAttackDemageMethod(const crJXJExtraAttackDemageMethod& handle);
	MethodClass(JXJ, JXJExtraAttackDemage)
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
	short m_overlapCount;
};

//class crJXJFriendCollideExtraMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJFriendCollideExtraMethod();
//	crJXJFriendCollideExtraMethod(const crJXJFriendCollideExtraMethod& handle);
//	MethodClass(JXJ, JXJFriendCollideExtra)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_item;
//	CRCore::ref_ptr<CRCore::crHandle> m_extraHandle;
//};
class crJXJExtraHitRatioPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraHitRatioPercentMethod();
	crJXJExtraHitRatioPercentMethod(const crJXJExtraHitRatioPercentMethod& handle);
	MethodClass(JXJ, JXJExtraHitRatioPercent)
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
	short m_overlapCount;
};
class crJXJExtraHaloMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的友军及自己释放
public:
	crJXJExtraHaloMethod();
	crJXJExtraHaloMethod(const crJXJExtraHaloMethod& handle);
	MethodClass(JXJ, JXJExtraHalo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_haloInterval;//光环每隔多少秒作用一次
	float m_dices;//概率

	bool m_start;
	float m_timer;
	float m_dtrec;
};
class crJXJExtraDHaloMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的敌军释放
public:
	crJXJExtraDHaloMethod();
	crJXJExtraDHaloMethod(const crJXJExtraDHaloMethod& handle);
	MethodClass(JXJ, JXJExtraDHalo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_haloInterval;//光环每隔多少秒作用一次
	float m_dices;//概率

	bool m_start;
	float m_timer;
	float m_dtrec;
};
class crJXJExtraShootMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的单个友军释放
public:
	crJXJExtraShootMethod();
	crJXJExtraShootMethod(const crJXJExtraShootMethod& handle);
	MethodClass(JXJ, JXJExtraShoot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_shootInterval;//发射间隔
	short m_shootCount;//弹射次数

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_count;
};
class crJXJExtraDShootMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的单个敌军释放
public:
	crJXJExtraDShootMethod();
	crJXJExtraDShootMethod(const crJXJExtraDShootMethod& handle);
	MethodClass(JXJ, JXJExtraDShoot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_shootInterval;//发射间隔
	short m_shootCount;//弹射次数

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_count;
};
class crJXJExtraChainMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的单个友军释放,被发射的BUF的生命周期设置长些，防止被重复作用
public:
	crJXJExtraChainMethod();
	crJXJExtraChainMethod(const crJXJExtraChainMethod& handle);
	MethodClass(JXJ, JXJExtraChain)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_shootInterval;//发射间隔
	short m_shootCount;//弹射次数

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_count;
};
class crJXJExtraDChainMethod : public CRCore::crMethod
{//SceneServer Method,对m_this身边的单个敌军释放,被发射的BUF的生命周期设置长些，防止被重复作用
public:
	crJXJExtraDChainMethod();
	crJXJExtraDChainMethod(const crJXJExtraDChainMethod& handle);
	MethodClass(JXJ, JXJExtraDChain)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_range;//光环作用范围,米
	short m_haloExtraID;
	float m_shootInterval;//发射间隔
	short m_shootCount;//弹射次数

	bool m_start;
	float m_timer;
	float m_dtrec;
	short m_count;
};
class crJXJEquipExtraMethod : public CRCore::crMethod
{//GameClient & SceneServer Method
public:
	crJXJEquipExtraMethod();
	crJXJEquipExtraMethod(const crJXJEquipExtraMethod& handle);
	MethodClass(JXJ, JXJEquipExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJReliveExtraMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJReliveExtraMethod();
	crJXJReliveExtraMethod(const crJXJReliveExtraMethod& handle);
	MethodClass(JXJ, JXJReliveExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
//class crJXJRecvEquipExtraMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJRecvEquipExtraMethod();
//	crJXJRecvEquipExtraMethod(const crJXJRecvEquipExtraMethod& handle);
//	MethodClass(JXJ, JXJRecvEquipExtra)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
///////////////
class crJXJArmyTypeCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public://检查m_this的兵系
	crJXJArmyTypeCheckMethod();
	crJXJArmyTypeCheckMethod(const crJXJArmyTypeCheckMethod& handle);
	MethodClass(JXJ, JXJArmyTypeCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};

class crJXJHitArmyTypeCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJHitArmyTypeCheckMethod();
	crJXJHitArmyTypeCheckMethod(const crJXJHitArmyTypeCheckMethod& handle);
	MethodClass(JXJ, JXJHitArmyTypeCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};

class crJXJFireArmyTypeCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJFireArmyTypeCheckMethod();
	crJXJFireArmyTypeCheckMethod(const crJXJFireArmyTypeCheckMethod& handle);
	MethodClass(JXJ, JXJFireArmyTypeCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJArmyAbstractIDCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public://检查m_this的兵种
	crJXJArmyAbstractIDCheckMethod();
	crJXJArmyAbstractIDCheckMethod(const crJXJArmyAbstractIDCheckMethod& handle);
	MethodClass(JXJ, JXJArmyAbstractIDCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	std::vector<int> m_idvec;
};

class crJXJHitArmyAbstractIDCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJHitArmyAbstractIDCheckMethod();
	crJXJHitArmyAbstractIDCheckMethod(const crJXJHitArmyAbstractIDCheckMethod& handle);
	MethodClass(JXJ, JXJHitArmyAbstractIDCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	std::vector<int> m_idvec;
};

class crJXJFireArmyAbstractIDCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJFireArmyAbstractIDCheckMethod();
	crJXJFireArmyAbstractIDCheckMethod(const crJXJFireArmyAbstractIDCheckMethod& handle);
	MethodClass(JXJ, JXJFireArmyAbstractIDCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	std::vector<int> m_idvec;
};
class crJXJExtraCritPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraCritPercentMethod();
	crJXJExtraCritPercentMethod(const crJXJExtraCritPercentMethod& handle);
	MethodClass(JXJ, JXJExtraCritPercent)
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
	short m_overlapCount;
};

class crJXJExtraCritMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraCritMethod();
	crJXJExtraCritMethod(const crJXJExtraCritMethod& handle);
	MethodClass(JXJ, JXJExtraCrit)
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
	short m_overlapCount;
};

class crJXJExtraBuildingAttackPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraBuildingAttackPercentMethod();
	crJXJExtraBuildingAttackPercentMethod(const crJXJExtraBuildingAttackPercentMethod& handle);
	MethodClass(JXJ, JXJExtraBuildingAttackPercent)
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
	short m_overlapCount;
};

class crJXJExtraParrayPercentMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraParrayPercentMethod();
	crJXJExtraParrayPercentMethod(const crJXJExtraParrayPercentMethod& handle);
	MethodClass(JXJ, JXJExtraParrayPercent)
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
	short m_overlapCount;
};
class crJXJExtraParrayMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraParrayMethod();
	crJXJExtraParrayMethod(const crJXJExtraParrayMethod& handle);
	MethodClass(JXJ, JXJExtraParray)
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
	short m_overlapCount;
};
class crJXJExtraAttackDistMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraAttackDistMethod();
	crJXJExtraAttackDistMethod(const crJXJExtraAttackDistMethod& handle);
	MethodClass(JXJ, JXJExtraAttackDist)
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
	short m_overlapCount;
};
class crJXJCheckTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckTouchMethod();
	crJXJCheckTouchMethod(const crJXJCheckTouchMethod& handle);
	MethodClass(JXJ, JXJCheckTouch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	//CRProducer::crGUIEventAdapter* m_ea;
	//CRCore::crMatrixTransform *m_camera;
	float m_dist;
};
class crJXJTouchSceneItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTouchSceneItemMethod();
	crJXJTouchSceneItemMethod(const crJXJTouchSceneItemMethod& handle);
	MethodClass(JXJ, JXJTouchSceneItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJClientRecvTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvTouchMethod();
	crJXJClientRecvTouchMethod(const crJXJClientRecvTouchMethod& handle);
	MethodClass(JXJ, JXJClientRecvTouch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJSceneItemDropMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSceneItemDropMethod();
	crJXJSceneItemDropMethod(const crJXJSceneItemDropMethod& handle);
	MethodClass(JXJ, JXJSceneItemDrop)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	bool m_abstractdrop;
};
class crJXJClientRecvSceneItemDropMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvSceneItemDropMethod();
	crJXJClientRecvSceneItemDropMethod(const crJXJClientRecvSceneItemDropMethod& handle);
	MethodClass(JXJ, JXJClientRecvSceneItemDrop)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRecvSceneItemDropMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvSceneItemDropMethod();
	crJXJRecvSceneItemDropMethod(const crJXJRecvSceneItemDropMethod& handle);
	MethodClass(JXJ, JXJRecvSceneItemDrop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJIsNpcChengchiMethod :public CRCore::crMethod
{//GameClient & SceneServer Method
public:
	crJXJIsNpcChengchiMethod();
	crJXJIsNpcChengchiMethod(const crJXJIsNpcChengchiMethod& handle);
	MethodClass(JXJ, JXJIsNpcChengchi)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
};
class crJXJLoadNpcEquipsFromTabMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crJXJLoadNpcEquipsFromTabMethod();
	crJXJLoadNpcEquipsFromTabMethod(const crJXJLoadNpcEquipsFromTabMethod& handle);
	MethodClass(JXJ, JXJLoadNpcEquipsFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJGuiseStateRenderMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGuiseStateRenderMethod();
	crJXJGuiseStateRenderMethod(const crJXJGuiseStateRenderMethod& handle);
	MethodClass(JXJ, JXJGuiseStateRender)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	enum SpecialFlg
	{
		SF_Default,
		SF_Shijiemache,
	};
protected:
	void autoTrans(CRCore::crNode *relNode);
	CRNetApp::crInstanceItem *m_this;
	char m_lastTrans;//1隐身,2不显示
	float m_interval;
	float m_alpha;
	float m_speed;
	unsigned char m_specFlg;
};
struct NpcSelectSkillInfo
{
	int itemId;				//技能ID；
	int aroundCount;			//周围兵数
	unsigned short weights;		//权重
	unsigned char  markId;		//标志符
};
class crJXJSelectSkillMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSelectSkillMethod();
	crJXJSelectSkillMethod(const crJXJSelectSkillMethod& handle);
	MethodClass(JXJ, JXJSelectSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	//int m_enemycount;
};
//class crJXJNpcRevertUpdateMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJNpcRevertUpdateMethod();
//	crJXJNpcRevertUpdateMethod(const crJXJNpcRevertUpdateMethod& handle);
//	MethodClass(JXJ, JXJNpcRevertUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crInstanceItem* m_this;
//	float m_dt;
//};
class crJXJNpcRevertMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJNpcRevertMethod();
	crJXJNpcRevertMethod(const crJXJNpcRevertMethod& handle);
	MethodClass(JXJ, JXJNpcRevert)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_revertPercent;
};
class crJXJRecvRevertMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvRevertMethod();
	crJXJRecvRevertMethod(const crJXJRecvRevertMethod& handle);
	MethodClass(JXJ, JXJRecvRevert)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJItemHPRemainPercentMethod : public CRCore::crMethod
{//GameClient & SceneServer Method
public:
	crJXJItemHPRemainPercentMethod();
	crJXJItemHPRemainPercentMethod(const crJXJItemHPRemainPercentMethod& handle);
	MethodClass(JXJ, JXJItemHPRemainPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_percent;
};
class crJXJItemExecuteActionMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJItemExecuteActionMethod();
	crJXJItemExecuteActionMethod(const crJXJItemExecuteActionMethod& handle);
	MethodClass(JXJ, JXJItemExecuteAction)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	std::string m_anim;
	bool m_action;
};
class crJXJGetHitTypeMethod : public CRCore::crMethod
{//SceneServer Method
	//命中对象类型 0：敌人 1队友 2自己
public:
	crJXJGetHitTypeMethod();
	crJXJGetHitTypeMethod(const crJXJGetHitTypeMethod& handle);
	MethodClass(JXJ, JXJGetHitType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};
class crJXJTargetTypeMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
	//命中对象类型 0：敌人 1队友 2自己
public:
	crJXJTargetTypeMethod();
	crJXJTargetTypeMethod(const crJXJTargetTypeMethod& handle);
	MethodClass(JXJ, JXJTargetType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};
class crJXJServerNoticeMethod : public CRCore::crMethod
{//Server Method
public:
	crJXJServerNoticeMethod();
	crJXJServerNoticeMethod(const crJXJServerNoticeMethod& handle);
	MethodClass(JXJ, JXJServerNotice)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void parseText(std::string &text);
	int m_textid;//0:使用noticeString
	std::string m_noticeString;
	std::string m_strparam1;
	std::string m_strparam2;
	std::string m_strparam3;
	std::string m_strparam4;
	std::string m_strparam5;
	std::string m_strparam6;
	std::string *m_output;//输出string
};
class crJXJNoticeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJNoticeMethod();
	crJXJNoticeMethod(const crJXJNoticeMethod& handle);
	MethodClass(JXJ, JXJNotice)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void parseText(std::string &text);
	int m_textid;//0:使用noticeString
	int m_mode;//0:notice,1:doModal,2 outputstr,3messagebox
	std::string m_tipcanvas;
	std::string m_tipcontent;
	std::string m_noticeString;
	std::string m_strparam1;
	std::string m_strparam2;
	std::string m_strparam3;
	std::string m_strparam4;
	std::string m_strparam5;
	std::string m_strparam6;
	std::string *m_output;//输出string
};
class crJXJGlobalNoticeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJGlobalNoticeMethod();
	crJXJGlobalNoticeMethod(const crJXJGlobalNoticeMethod& handle);
	MethodClass(JXJ, JXJGlobalNotice)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_textid;
	std::string *m_str;
	std::string m_canvas;
	std::string m_widget;
};
class crJXJRecvChangechiChangeMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvChangechiChangeMethod();
	crJXJRecvChangechiChangeMethod(const crJXJRecvChangechiChangeMethod& handle);
	MethodClass(JXJ, JXJRecvChangechiChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCameraSelectUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraSelectUpdateMethod();
	crJXJCameraSelectUpdateMethod(const crJXJCameraSelectUpdateMethod& handle);
	MethodClass(JXJ, JXJCameraSelectUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJIsCameraFollowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJIsCameraFollowMethod();
	crJXJIsCameraFollowMethod(const crJXJIsCameraFollowMethod& handle);
	MethodClass(JXJ, JXJIsCameraFollow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_strCanvas;
	std::string m_strCheckBox;
};
class crJXJSetXumuFubenIDMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetXumuFubenIDMethod();
	crJXJSetXumuFubenIDMethod(const crJXJSetXumuFubenIDMethod& handle);
	MethodClass(JXJ, JXJSetXumuFubenID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJCreateXumuMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCreateXumuMethod();
	crJXJCreateXumuMethod(const crJXJCreateXumuMethod& handle);
	MethodClass(JXJ, JXJCreateXumu)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crVector4i m_formationInfo[3];
};
class crJXJRecvCreateXumuMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCreateXumuMethod();
	crJXJRecvCreateXumuMethod(const crJXJRecvCreateXumuMethod& handle);
	MethodClass(JXJ, JXJRecvCreateXumu)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJItemDeadChangeCampMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJItemDeadChangeCampMethod();
	crJXJItemDeadChangeCampMethod(const crJXJItemDeadChangeCampMethod& handle);
	MethodClass(JXJ, JXJItemDeadChangeCamp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};
class crJXJBingzhengFubenProgressMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBingzhengFubenProgressMethod();
	crJXJBingzhengFubenProgressMethod(const crJXJBingzhengFubenProgressMethod& handle);
	MethodClass(JXJ, JXJBingzhengFubenProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	short m_fubenprogress;
};
class crJXJCheckBingzhengCampMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCheckBingzhengCampMethod();
	crJXJCheckBingzhengCampMethod(const crJXJCheckBingzhengCampMethod& handle);
	MethodClass(JXJ, JXJCheckBingzhengCamp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJSwitchFubenProgressTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSwitchFubenProgressTimerMethod();
	crJXJSwitchFubenProgressTimerMethod(const crJXJSwitchFubenProgressTimerMethod& handle);
	MethodClass(JXJ, JXJSwitchFubenProgressTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	float *m_dt;
	float m_interval;
	float m_timer;
	std::vector<short> m_fubenProgressVec;
	int m_curindex;
};
class crJXJClientCheckItemsDeadMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClientCheckItemsDeadMethod();
	crJXJClientCheckItemsDeadMethod(const crJXJClientCheckItemsDeadMethod& handle);
	MethodClass(JXJ, JXJClientCheckItemsDead)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::vector<int> m_itemidVec;
	char m_trueExecCount;
};
class crJXJBingzhengBirthPointMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBingzhengBirthPointMethod();
	crJXJBingzhengBirthPointMethod(const crJXJBingzhengBirthPointMethod& handle);
	MethodClass(JXJ, JXJBingzhengBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJBingzhenReliveProtectMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJBingzhenReliveProtectMethod();
	crJXJBingzhenReliveProtectMethod(const crJXJBingzhenReliveProtectMethod& handle);
	MethodClass(JXJ, JXJBingzhenReliveProtect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
class crJXJGuiseSyncMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJGuiseSyncMethod();
	crJXJGuiseSyncMethod(const crJXJGuiseSyncMethod& handle);
	MethodClass(JXJ, JXJGuiseSync)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	float m_dt;
	float m_interval;
	float m_time;
	unsigned int m_lastGuise;
};
class crJXJRecvGuiseSyncMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvGuiseSyncMethod();
	crJXJRecvGuiseSyncMethod(const crJXJRecvGuiseSyncMethod& handle);
	MethodClass(JXJ, JXJRecvGuiseSync)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRecvTouchShuijingMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvTouchShuijingMethod();
	crJXJRecvTouchShuijingMethod(const crJXJRecvTouchShuijingMethod& handle);
	MethodClass(JXJ, JXJRecvTouchShuijing)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	float m_range;
	unsigned char m_indexType; //0表示治疗血量，1表示治疗士气
};
class crJXJFrameLowDetectMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJFrameLowDetectMethod();
	crJXJFrameLowDetectMethod(const crJXJFrameLowDetectMethod& handle);
	MethodClass(JXJ, JXJFrameLowDetect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static void setDetectEnable(bool enable);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_interval;
	float m_time;
	int m_minframe;
	std::string m_strMsgCanvas;
	std::string m_strMsg;
	static bool s_detectEnable;
};
class crJXJFrameLowSettingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJFrameLowSettingMethod();
	crJXJFrameLowSettingMethod(const crJXJFrameLowSettingMethod& handle);
	MethodClass(JXJ, JXJFrameLowSetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJFrameDetectEnableMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJFrameDetectEnableMethod();
	crJXJFrameDetectEnableMethod(const crJXJFrameDetectEnableMethod& handle);
	MethodClass(JXJ, JXJFrameDetectEnable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_enable;
};
class crJXJRecvPositionSyncMethod : public CRCore::crMethod
{//Client & Server Method
public:
	crJXJRecvPositionSyncMethod();
	crJXJRecvPositionSyncMethod(const crJXJRecvPositionSyncMethod& handle);
	MethodClass(JXJ, JXJRecvPositionSync)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	int m_counter;
};
class crJXJSprintMethod : public CRCore::crMethod
{//Client Method 向前冲刺多少米
public:
	crJXJSprintMethod();
	crJXJSprintMethod(const crJXJSprintMethod& handle);
	MethodClass(JXJ, JXJSprint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	float m_dist;
	CRCore::crVector3 m_gunpoint;
	CRCore::crVector3 *m_target;
};
class crJXJSprintTargetMethod : public CRCore::crMethod
{//Client Method 闪现到目的地
public:
	crJXJSprintTargetMethod();
	crJXJSprintTargetMethod(const crJXJSprintTargetMethod& handle);
	MethodClass(JXJ, JXJSprintTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRCore::crVector3 m_gunpoint;
	CRCore::crVector3 *m_target;
	float m_maxdist;
	bool m_ignoreMap;//忽视地形
};
class crJXJTransportationMethod : public CRCore::crMethod
{//Client Method 闪现到最近据点
public:
	crJXJTransportationMethod();
	crJXJTransportationMethod(const crJXJTransportationMethod& handle);
	MethodClass(JXJ, JXJTransportation)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRCore::crVector3 m_gunpoint;
	CRCore::crVector3 *m_target;
};
class crJXJSunSimulateLogic : public CRCore::crLogic
{//Client Method
public:
	crJXJSunSimulateLogic();
	crJXJSunSimulateLogic(const crJXJSunSimulateLogic& handle);
	LogicClass(JXJ, JXJSunSimulate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_dayNightHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_timeChangeHandle;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_sunTab;
	bool m_bIsDayTime;	//是否是白天
	float m_fTimeScale;
	int m_lastsecond;
	CRCore::Timer_t m_lasttick;
	int m_sundownid;//日出时间点0,日落时间点4
	bool m_disiable;
};
class crJXJSetNightLightMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetNightLightMethod();
	crJXJSetNightLightMethod(const crJXJSetNightLightMethod& handle);
	MethodClass(JXJ, JXJSetNightLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_bIsDayTime;//
	std::string m_nodename;
};
class crJXJSceneSetVisiableMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSceneSetVisiableMethod();
	crJXJSceneSetVisiableMethod(const crJXJSceneSetVisiableMethod& handle);
	MethodClass(JXJ, JXJSceneSetVisiable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_visiable;//
};
class crJXJBulletFireCaseMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJBulletFireCaseMethod();
	crJXJBulletFireCaseMethod(const crJXJBulletFireCaseMethod& handle);
	MethodClass(JXJ, JXJBulletFireCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crBulletMatterObject* m_this;
	std::map<int,int> m_fxidMap;//abstractid,case;
};
class crJXJUseItemCheckMethod : public CRCore::crMethod
{//Client & SceneServer Method
public:
	crJXJUseItemCheckMethod();
	crJXJUseItemCheckMethod(const crJXJUseItemCheckMethod& handle);
	MethodClass(JXJ, JXJUseItemCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};
class crJXJUseItemMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUseItemMethod();
	crJXJUseItemMethod(const crJXJUseItemMethod& handle);
	MethodClass(JXJ, JXJUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};

class crJXJServerUseItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerUseItemMethod();
	crJXJServerUseItemMethod(const crJXJServerUseItemMethod& handle);
	MethodClass(JXJ, JXJServerUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};

class crJXJNetUseItemMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJNetUseItemMethod();
	crJXJNetUseItemMethod(const crJXJNetUseItemMethod& handle);
	MethodClass(JXJ, JXJNetUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::UseItemParam* m_useItemParam;
	unsigned char m_usercdid;
	float m_actdt;
};
class crJXJExtraShiftingMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraShiftingMethod();
	crJXJExtraShiftingMethod(const crJXJExtraShiftingMethod& handle);
	MethodClass(JXJ, JXJExtraShifting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	float m_extraValue;//移动速度
	char m_flg;//0:向外,1:向内,2混乱移动,3朝目标点位移
	unsigned short m_act;
	float m_maxdist;//向外移动的最大距离限制,只有m_flg=0的时候有效
	bool m_ignoreMap;//忽视地形

	bool m_start;
	float m_timer;
	CRCore::crVector3 m_dir;
	CRCore::crVector3 m_firepos;
};
class crJXJExtraLiftMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJExtraLiftMethod();
	crJXJExtraLiftMethod(const crJXJExtraLiftMethod& handle);
	MethodClass(JXJ, JXJExtraLift)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	float m_extraValue;//移动速度(厘米)+向上,-向下
	float m_maxdist;//移动的最大距离限制

	bool m_start;
	float m_timer;
	float m_movedDist;
};
class crJXJExtraDelayControlMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraDelayControlMethod();
	crJXJExtraDelayControlMethod(const crJXJExtraDelayControlMethod& handle);
	MethodClass(JXJ, JXJExtraDelayControl)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_timer;
	float m_delay;
};
class crJXJNpcDeadDropItemMethod : public CRCore::crMethod
{//SceneServer Method,NPC死亡掉落
public:
	crJXJNpcDeadDropItemMethod();
	crJXJNpcDeadDropItemMethod(const crJXJNpcDeadDropItemMethod& handle);
	MethodClass(JXJ, JXJNpcDeadDropItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_killerItem;
	float m_interval;
};
class crJXJRecvAbstractItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvAbstractItemMethod();
	crJXJRecvAbstractItemMethod(const crJXJRecvAbstractItemMethod& handle);
	MethodClass(JXJ, JXJRecvAbstractItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRNetApp::crRecvDataStream> m_recvDataStream;
};
class crJXJRoomCheckHpPercentDownMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJRoomCheckHpPercentDownMethod();
	crJXJRoomCheckHpPercentDownMethod(const crJXJRoomCheckHpPercentDownMethod& handle);
	MethodClass(JXJ, JXJRoomCheckHpPercentDown)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_instanceid;
	float m_percent;
};
class crJXJCheckHpPercentDownMethod : public CRCore::crMethod
{//SceneServer & GameClient Method AddDamage or RecvDamage
public:
	crJXJCheckHpPercentDownMethod();
	crJXJCheckHpPercentDownMethod(const crJXJCheckHpPercentDownMethod& handle);
	MethodClass(JXJ, JXJCheckHpPercentDown)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	float m_percent;
};
class crJXJRemoveItemFormSceneMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJRemoveItemFormSceneMethod();
	crJXJRemoveItemFormSceneMethod(const crJXJRemoveItemFormSceneMethod& handle);
	MethodClass(JXJ, JXJRemoveItemFormScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_instanceid;
};
class crJXJCheckMyHpPercentDownMethod : public CRCore::crMethod
{//GameClient Method 检查玩家某一支部队血量少于
public:
	crJXJCheckMyHpPercentDownMethod();
	crJXJCheckMyHpPercentDownMethod(const crJXJCheckMyHpPercentDownMethod& handle);
	MethodClass(JXJ, JXJCheckMyHpPercentDown)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_percent;
};
class crJXJRoomCurePlayerHPMethod : public CRCore::crMethod
{//SceneServer Method 关卡内所有玩家部队回血
public:
	crJXJRoomCurePlayerHPMethod();
	crJXJRoomCurePlayerHPMethod(const crJXJRoomCurePlayerHPMethod& handle);
	MethodClass(JXJ, JXJRoomCurePlayerHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	float m_curehp;
};
class crJXJRoomCurePlayerMPMethod : public CRCore::crMethod
{//SceneServer Method 关卡内所有玩家部队回蓝
public:
	crJXJRoomCurePlayerMPMethod();
	crJXJRoomCurePlayerMPMethod(const crJXJRoomCurePlayerMPMethod& handle);
	MethodClass(JXJ, JXJRoomCurePlayerMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	float m_curemp;
};
class crJXJAddPlayerCureMPMethod : public CRCore::crMethod
{//SceneServer Method 关卡内所有玩家部队回蓝
public:
	crJXJAddPlayerCureMPMethod();
	crJXJAddPlayerCureMPMethod(const crJXJAddPlayerCureMPMethod& handle);
	MethodClass(JXJ, JXJAddPlayerCureMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crSceneServerPlayerData> m_player;
	float m_curemp;
};
class crJXJPlayerGainExtraMethod : public CRCore::crMethod
{//SceneServer Method 关卡内所有玩家部队回血
public:
	crJXJPlayerGainExtraMethod();
	crJXJPlayerGainExtraMethod(const crJXJPlayerGainExtraMethod& handle);
	MethodClass(JXJ, JXJPlayerGainExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	short m_extraid;
};
class crJXJPlayerRemoveExtraMethod : public CRCore::crMethod
{//SceneServer Method 关卡内所有玩家部队清除BUFF
public:
	crJXJPlayerRemoveExtraMethod();
	crJXJPlayerRemoveExtraMethod(const crJXJPlayerRemoveExtraMethod& handle);
	MethodClass(JXJ, JXJPlayerRemoveExtra)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	short m_extraid;
};
class crJXJRecvRemoveExtraMethod : public CRCore::crMethod
{//GameClient
public:
	crJXJRecvRemoveExtraMethod();
	crJXJRecvRemoveExtraMethod(const crJXJRecvRemoveExtraMethod& handle);
	MethodClass(JXJ, JXJRecvRemoveExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJCheckRoleDamageHpPercentMethod : public CRCore::crMethod
{//SceneServer Method玩家角色受伤,血量百分比
public:
	crJXJCheckRoleDamageHpPercentMethod();
	crJXJCheckRoleDamageHpPercentMethod(const crJXJCheckRoleDamageHpPercentMethod& handle);
	MethodClass(JXJ, JXJCheckRoleDamageHpPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	CRCore::ref_ptr<CRNetApp::crRole> m_role;
	float m_percent;
};
class crJXJRoomTriggerCountMethod : public CRCore::crMethod
{//GameClient & SceneServer Method关卡触发次数控制器
public:
	crJXJRoomTriggerCountMethod();
	crJXJRoomTriggerCountMethod(const crJXJRoomTriggerCountMethod& handle);
	MethodClass(JXJ, JXJRoomTriggerCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	int m_maxcount;//-1表示无穷
	int m_count;
	int m_resetInterval;//次数重置时间
	time_t m_lasttimer;
};
class crJXJRoomTriggerCountCaseMethod : public CRCore::crMethod
{//GameClient & SceneServer Method关卡触发次数控制器
public:
	crJXJRoomTriggerCountCaseMethod();
	crJXJRoomTriggerCountCaseMethod(const crJXJRoomTriggerCountCaseMethod& handle);
	MethodClass(JXJ, JXJRoomTriggerCountCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	int m_count;
};
class crJXJIfInVolumeNodeIsNpcMethod : public CRCore::crMethod
{//Client Method
public:
	typedef std::vector<int> NpcIDVec;
	crJXJIfInVolumeNodeIsNpcMethod();
	crJXJIfInVolumeNodeIsNpcMethod(const crJXJIfInVolumeNodeIsNpcMethod& handle);
	MethodClass(JXJ, JXJIfInVolumeNodeIsNpc)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_inVolumeNode;
	NpcIDVec m_npcVec;
	//int m_npcid;//instanceid

};
class crJXJSetNpcGuiseStateFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJSetNpcGuiseStateFromTabMethod();
	crJXJSetNpcGuiseStateFromTabMethod(const crJXJSetNpcGuiseStateFromTabMethod& handle);
	MethodClass(JXJ, JXJSetNpcGuiseStateFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_id;
	bool m_open;
	unsigned int m_state;
	//bool m_garrison;
};
class crJXJRemoveItemsFormSceneMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJRemoveItemsFormSceneMethod();
	crJXJRemoveItemsFormSceneMethod(const crJXJRemoveItemsFormSceneMethod& handle);
	MethodClass(JXJ, JXJRemoveItemsFormScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_tableid;//tableid
};
class crJXJCheckFireItemFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckFireItemFromTabMethod();
	crJXJCheckFireItemFromTabMethod(const crJXJCheckFireItemFromTabMethod& handle);
	MethodClass(JXJ, JXJCheckFireItemFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	int m_tableid;//tableid
};
class crJXJCheckHitItemFromTabMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckHitItemFromTabMethod();
	crJXJCheckHitItemFromTabMethod(const crJXJCheckHitItemFromTabMethod& handle);
	MethodClass(JXJ, JXJCheckHitItemFromTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_hitItem;
	int m_tableid;//tableid
};
class crJXJCheckFireItemIDMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckFireItemIDMethod();
	crJXJCheckFireItemIDMethod(const crJXJCheckFireItemIDMethod& handle);
	MethodClass(JXJ, JXJCheckFireItemID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	int m_itemid;//instanceid
};
class crJXJCheckHitItemIDMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckHitItemIDMethod();
	crJXJCheckHitItemIDMethod(const crJXJCheckHitItemIDMethod& handle);
	MethodClass(JXJ, JXJCheckHitItemID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_hitItem;
	int m_itemid;//instanceid
};
class crJXJChangeItemPatrolPointVecFormSceneMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	typedef std::vector<float> PointVec; 
	crJXJChangeItemPatrolPointVecFormSceneMethod ();
	crJXJChangeItemPatrolPointVecFormSceneMethod (const crJXJChangeItemPatrolPointVecFormSceneMethod & handle);
	MethodClass(JXJ, JXJChangeItemPatrolPointVecFormScene)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom *m_this;
	int m_tableid;
	PointVec m_pointVec;
};

class crJXJNpcGainExtraFromTableMethod : public CRCore::crMethod
{//SceneServer Method Npc 上buf
public:
	crJXJNpcGainExtraFromTableMethod ();
	crJXJNpcGainExtraFromTableMethod (const crJXJNpcGainExtraFromTableMethod & handle);
	MethodClass(JXJ, JXJNpcGainExtraFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableid;
	short m_extraid;
};
class crJXJNpcRemoveExtraFromTableMethod : public CRCore::crMethod
{//SceneServer Method Npc 移除buf
public:
	crJXJNpcRemoveExtraFromTableMethod ();
	crJXJNpcRemoveExtraFromTableMethod (const crJXJNpcRemoveExtraFromTableMethod & handle);
	MethodClass(JXJ, JXJNpcRemoveExtraFromTable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableid;
	short m_extraid;
};
class crJXJNpcLockPlayerMethod : public CRCore::crMethod
{//SceneServer Method Npc 锁定玩家
public:
	crJXJNpcLockPlayerMethod ();
	crJXJNpcLockPlayerMethod (const crJXJNpcLockPlayerMethod & handle);
	MethodClass(JXJ, JXJNpcLockPlayer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
};
class crJXJAddEyePointMethod : public CRCore::crMethod
{//SceneServer Method//插眼
public:
	crJXJAddEyePointMethod ();
	crJXJAddEyePointMethod (const crJXJAddEyePointMethod & handle);
	MethodClass(JXJ, JXJAddEyePoint)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_camp;//1:守方,2攻方
	std::vector<CRCore::crVector2i> m_eyepointVec;
};
class crJXJSetNpcStealthMethod : public CRCore::crMethod
{//SceneServer Method Npc  设置潜行
public:
	crJXJSetNpcStealthMethod ();
	crJXJSetNpcStealthMethod (const crJXJSetNpcStealthMethod & handle);
	MethodClass(JXJ, JXJSetNpcStealth)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_tableID;
	short m_stealth;
};

class crJXJSetPlayerSightRangeMethod : public CRCore::crMethod
{//SceneServer Method   设置视野
public:
	crJXJSetPlayerSightRangeMethod ();
	crJXJSetPlayerSightRangeMethod (const crJXJSetPlayerSightRangeMethod & handle);
	MethodClass(JXJ, JXJSetPlayerSightRange)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	short m_sight;
};
class crJXJAddScenarioTimerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJAddScenarioTimerMethod ();
	crJXJAddScenarioTimerMethod (const crJXJAddScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJAddScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	int m_msg;
	float m_interval;
	int m_id;
	int m_times;//-1表示无穷,默认1,0表示不启用
};
class crJXJRemoveScenarioTimerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRemoveScenarioTimerMethod ();
	crJXJRemoveScenarioTimerMethod (const crJXJRemoveScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJRemoveScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	int m_id;
};
class crJXJServerAddScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerAddScenarioTimerMethod ();
	crJXJServerAddScenarioTimerMethod (const crJXJServerAddScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJServerAddScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_msg;
	float m_interval;
	int m_id;
	int m_times;//-1表示无穷,默认1,0表示不启用
};
class crJXJServerEnableScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerEnableScenarioTimerMethod ();
	crJXJServerEnableScenarioTimerMethod (const crJXJServerEnableScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJServerEnableScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_id;
	int m_times;//-1表示无穷,默认1,0表示暂停计时器，-2将计时器清零并停止计时
};
class crJXJServerRemoveScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerRemoveScenarioTimerMethod ();
	crJXJServerRemoveScenarioTimerMethod (const crJXJServerRemoveScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJServerRemoveScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_id;
};
class crJXJRecvAddScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvAddScenarioTimerMethod ();
	crJXJRecvAddScenarioTimerMethod (const crJXJRecvAddScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJRecvAddScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRecvRemoveScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvRemoveScenarioTimerMethod ();
	crJXJRecvRemoveScenarioTimerMethod (const crJXJRecvRemoveScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJRecvRemoveScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJScenarioTimerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJScenarioTimerMethod ();
	crJXJScenarioTimerMethod (const crJXJScenarioTimerMethod & handle);
	MethodClass(JXJ, JXJScenarioTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	float *m_dt;
};
class crJXJPauseRoomMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJPauseRoomMethod ();
	crJXJPauseRoomMethod (const crJXJPauseRoomMethod & handle);
	MethodClass(JXJ, JXJPauseRoom)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	float m_pausetime;//0.0表示取消暂停
};
class crJXJClientPauseRoomMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJClientPauseRoomMethod();
	crJXJClientPauseRoomMethod(const crJXJClientPauseRoomMethod& handle);
	MethodClass(JXJ, JXJClientPauseRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom* m_this;
	short m_pausetime;//0表示取消暂停
};
class crJXJRecvPauseRoomMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvPauseRoomMethod();
	crJXJRecvPauseRoomMethod(const crJXJRecvPauseRoomMethod& handle);
	MethodClass(JXJ, JXJRecvPauseRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJRoomDoEventMethod :public CRCore::crMethod
{//GameClient SceneServer Method
public:
	crJXJRoomDoEventMethod();
	crJXJRoomDoEventMethod(const crJXJRoomDoEventMethod& handle);
	MethodClass(JXJ, JXJRoomDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_msg;
	_crInt64 m_param;
};
class crJXJExtraDamageMaxHPPercentMethod : public CRCore::crMethod
{//SceneServer Method 最大生命百分比掉血
public:
	crJXJExtraDamageMaxHPPercentMethod();
	crJXJExtraDamageMaxHPPercentMethod(const crJXJExtraDamageMaxHPPercentMethod& handle);
	MethodClass(JXJ, JXJExtraDamageMaxHPPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	short m_extraValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
};
class crJXJExtraDamageRTHPPercentMethod : public CRCore::crMethod
{//SceneServer Method 当前生命百分比掉血
public:
	crJXJExtraDamageRTHPPercentMethod();
	crJXJExtraDamageRTHPPercentMethod(const crJXJExtraDamageRTHPPercentMethod& handle);
	MethodClass(JXJ, JXJExtraDamageRTHPPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float *m_dt;
	float m_duration;
	short m_extraValue;

	bool m_start;
	float m_timer;
	float m_dtrec;
};
class crJXJServerSetScenarioVariableMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerSetScenarioVariableMethod ();
	crJXJServerSetScenarioVariableMethod (const crJXJServerSetScenarioVariableMethod & handle);
	MethodClass(JXJ, JXJServerSetScenarioVariable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::string m_nameid;
	float m_value;
	bool m_sync;//是否把当前数据同步给客户端（仅在对象创建和发生改变时触发）
};
class crJXJServerScenarioVariableOperatorMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerScenarioVariableOperatorMethod ();
	crJXJServerScenarioVariableOperatorMethod (const crJXJServerScenarioVariableOperatorMethod & handle);
	MethodClass(JXJ, JXJServerScenarioVariableOperator)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::string m_nameid;
	int m_op;//0:赋值,1:加法,2乘法
	float m_value;
	bool m_sync;//是否把当前数据同步给客户端（仅在对象创建和发生改变时触发）
};
class crJXJServerScenarioVariableCheckMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerScenarioVariableCheckMethod ();
	crJXJServerScenarioVariableCheckMethod (const crJXJServerScenarioVariableCheckMethod & handle);
	MethodClass(JXJ, JXJServerScenarioVariableCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::string m_nameid;
	int m_op;//0:是否存在,1等于,2大于,3大于等于,4小于,5小于等于
	float m_value;
};
class crJXJServerScenarioVariableCaseMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerScenarioVariableCaseMethod ();
	crJXJServerScenarioVariableCaseMethod (const crJXJServerScenarioVariableCaseMethod & handle);
	MethodClass(JXJ, JXJServerScenarioVariableCase)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::string m_nameid;
};
class crJXJServerScenarioVariableSaveMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerScenarioVariableSaveMethod ();
	crJXJServerScenarioVariableSaveMethod (const crJXJServerScenarioVariableSaveMethod & handle);
	MethodClass(JXJ, JXJServerScenarioVariableSave)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::vector<std::string> m_nameidVec;
	std::string m_filepath;
};
class crJXJServerScenarioVariableLoadMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJServerScenarioVariableLoadMethod ();
	crJXJServerScenarioVariableLoadMethod (const crJXJServerScenarioVariableLoadMethod & handle);
	MethodClass(JXJ, JXJServerScenarioVariableLoad)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::vector<std::string> m_nameidVec;
	std::string m_filepath;
	bool m_sync;
};
class crJXJExtraCritPowerMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraCritPowerMethod();
	crJXJExtraCritPowerMethod(const crJXJExtraCritPowerMethod& handle);
	MethodClass(JXJ, JXJExtraCritPower)
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
	short m_overlapCount;
};
class crJXJExtraSputteringMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSputteringMethod();
	crJXJExtraSputteringMethod(const crJXJExtraSputteringMethod& handle);
	MethodClass(JXJ, JXJExtraSputtering)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	CRCore::crVector2s m_extraValue;

	bool m_start;
	float m_timer;
	short m_overlapCount;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleAddArmMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleDeadAddArmyMethod:public CRCore::crMethod
{//Client 战场部队全死亡后触发
public:
	crJXJBattleDeadAddArmyMethod();
	crJXJBattleDeadAddArmyMethod(const crJXJBattleDeadAddArmyMethod& handle);
	MethodClass(JXJ,JXJBattleDeadAddArmy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleDeadAddArmyPromptMethod
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::pair<int,/*unsigned short*/int>> AddArmyInfoVec;
class crJXJBattleDeadAddArmyPromptMethod:public CRCore::crMethod
{// client
public:
	crJXJBattleDeadAddArmyPromptMethod();
	crJXJBattleDeadAddArmyPromptMethod(const crJXJBattleDeadAddArmyPromptMethod& handle);
	MethodClass(JXJ, JXJBattleDeadAddArmyPrompt)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void battleDeadAddArmyFaild(int code);
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_inputMoney;
	std::string m_inputWood;
	std::string m_inputIron;
	std::string m_inputHorse;
	std::string m_inputFood;
	std::string m_inputRecruitTime;
	std::string m_inputRecruitTime_2;
	std::string m_okbtn;
	std::string m_strCanvas;
	std::string m_strText;
};
class crJXJRecvAddArmyFaildMethod:public CRCore::crMethod
{// client
public:
	crJXJRecvAddArmyFaildMethod();
	crJXJRecvAddArmyFaildMethod(const crJXJRecvAddArmyFaildMethod& handle);
	MethodClass(JXJ, JXJRecvAddArmyFaild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	std::string m_strText;
};
class crJXJNpcUseSkillMethod : public CRCore::crMethod
{//SceneServer Method Npc放技能
public:
	crJXJNpcUseSkillMethod ();
	crJXJNpcUseSkillMethod (const crJXJNpcUseSkillMethod & handle);
	MethodClass(JXJ, JXJNpcUseSkill)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_itemid;
	std::vector<int> m_abstractIDVec;
	unsigned char m_mark;//
	float m_curemp;
};
class crJXJSceneCreateMainRoomMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSceneCreateMainRoomMethod ();
	crJXJSceneCreateMainRoomMethod (const crJXJSceneCreateMainRoomMethod & handle);
	MethodClass(JXJ, JXJSceneCreateMainRoom)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
};
class crJXJTimerEqualMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJTimerEqualMethod();
	crJXJTimerEqualMethod(const crJXJTimerEqualMethod& handle);
	MethodClass(JXJ, JXJTimerEqual)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_time;
	int m_delta;//允许服务器最大延时触发时间差
};
class crJXJKillNpcShiliGainItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJKillNpcShiliGainItemMethod();
	crJXJKillNpcShiliGainItemMethod(const crJXJKillNpcShiliGainItemMethod& handle);
	MethodClass(JXJ, JXJKillNpcShiliGainItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_deadItem;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	int m_dropitemid;
	bool m_isfirecamp;//
};
class crJXJRoomItemDrop2Method : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRoomItemDrop2Method();
	crJXJRoomItemDrop2Method(const crJXJRoomItemDrop2Method& handle);
	MethodClass(JXJ, JXJRoomItemDrop2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_dropitemid;
	unsigned char m_camp;//0表示所有
};
class crJXJSendItemposSignalToPlayerMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSendItemposSignalToPlayerMethod();
	crJXJSendItemposSignalToPlayerMethod(const crJXJSendItemposSignalToPlayerMethod& handle);
	MethodClass(JXJ, JXJSendItemposSignalToPlayer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_itemid;
	unsigned char m_camp;//0表示所有,10,11,12表示魏蜀吴
};
class crJXJSetGuiseMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJSetGuiseMethod();
	crJXJSetGuiseMethod(const crJXJSetGuiseMethod& handle);
	MethodClass(JXJ, JXJSetGuise)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	unsigned int m_guise;
	bool m_setflg;//1:设置,0:取消
};
class crJXJSetItemStateMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJSetItemStateMethod();
	crJXJSetItemStateMethod(const crJXJSetItemStateMethod& handle);
	MethodClass(JXJ, JXJSetItemState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	unsigned char m_state;
	float m_rthp;
};
class crJXJCheckNpcGuiseMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCheckNpcGuiseMethod();
	crJXJCheckNpcGuiseMethod(const crJXJCheckNpcGuiseMethod& handle);
	MethodClass(JXJ, JXJCheckNpcGuise)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_itemid;
	unsigned int m_guise;
};

class crJXJExtraDropItemMethod : public CRCore::crMethod
{//SceneServer Method 召唤类技能施法
public://伤害计算
	crJXJExtraDropItemMethod();
	crJXJExtraDropItemMethod(const crJXJExtraDropItemMethod& handle);
	MethodClass(JXJ, JXJExtraDropItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	CRCore::crVector3 m_itemUseTargetPos;

	int m_abstractid;
	unsigned char m_itemtype;//0：场景阻挡体，1：npc
	/*unsigned short*/int m_itemcount;
	float m_interval;
	short m_bufid;//给召唤物一个buf

	bool m_start;
};

class crJXJExtraPhantomMethod : public CRCore::crMethod
{//SceneServer Method,制造m_this的幻象
public:
	crJXJExtraPhantomMethod();
	crJXJExtraPhantomMethod(const crJXJExtraPhantomMethod& handle);
	MethodClass(JXJ, JXJExtraPhantom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
	float m_duration;//幻象持续时间
	float m_hpScale;//幻象生命百分比
	float m_damageScale;//幻象攻击力百分比
	short m_count;//幻象个数
	short m_bufid;//给幻象一个buf

	bool m_start;
};
class crJXJIsGainExtraMethod : public CRCore::crMethod
{//SceneServer Method 攻击者或者受击者正处于m_extraid的BUF下
public:
	crJXJIsGainExtraMethod();
	crJXJIsGainExtraMethod(const crJXJIsGainExtraMethod& handle);
	MethodClass(JXJ, JXJIsGainExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_type;//0:fireitem,1hititem
	std::vector<int> m_idvec;
};
class crJXJExtraIsGainExtraMethod : public CRCore::crMethod
{//SceneServer & GameClient Method,m_this是否正处于m_extraid的BUF下
public:
	crJXJExtraIsGainExtraMethod();
	crJXJExtraIsGainExtraMethod(const crJXJExtraIsGainExtraMethod& handle);
	MethodClass(JXJ, JXJExtraIsGainExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	std::vector<int> m_idvec;
};
class crJXJInCritTestMethod : public CRCore::crMethod
{//SceneServer Method 攻击者或者受击者正处于暴击状态
public:
	crJXJInCritTestMethod();
	crJXJInCritTestMethod(const crJXJInCritTestMethod& handle);
	MethodClass(JXJ, JXJInCritTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_type;//0:fireitem,1hititem
};
class crJXJInDodgeTestMethod : public CRCore::crMethod
{//SceneServer Method 攻击者或者受击者正处于闪避状态
public:
	crJXJInDodgeTestMethod();
	crJXJInDodgeTestMethod(const crJXJInDodgeTestMethod& handle);
	MethodClass(JXJ, JXJInDodgeTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_type;//0:fireitem,1hititem
};
class crJXJInParryTestMethod : public CRCore::crMethod
{//SceneServer Method 攻击者或者受击者正处于格挡状态
public:
	crJXJInParryTestMethod();
	crJXJInParryTestMethod(const crJXJInParryTestMethod& handle);
	MethodClass(JXJ, JXJInParryTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_type;//0:fireitem,1hititem
};
class crJXJIsMyRoleAllDeadMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJIsMyRoleAllDeadMethod();
	crJXJIsMyRoleAllDeadMethod(const crJXJIsMyRoleAllDeadMethod& handle);
	MethodClass(JXJ, JXJIsMyRoleAllDead)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJScenarioPlayerCountCheckMethod : public CRCore::crMethod
{//SceneServer Method判定关卡内玩家数量
public:
	crJXJScenarioPlayerCountCheckMethod ();
	crJXJScenarioPlayerCountCheckMethod (const crJXJScenarioPlayerCountCheckMethod & handle);
	MethodClass(JXJ, JXJScenarioPlayerCountCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	int m_op;//1等于,2大于,3大于等于,4小于,5小于等于
	int m_value;//数量
};
class crJXJExtraCorpseCountMethod : public CRCore::crMethod
{//SceneServer & GameClient Method周围尸体数量
public:
	crJXJExtraCorpseCountMethod();
	crJXJExtraCorpseCountMethod(const crJXJExtraCorpseCountMethod& handle);
	MethodClass(JXJ, JXJExtraCorpseCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_range;
};
class crJXJExtraHpPercentCountMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 血量低于百分比的倍数
public:
	crJXJExtraHpPercentCountMethod();
	crJXJExtraHpPercentCountMethod(const crJXJExtraHpPercentCountMethod& handle);
	MethodClass(JXJ, JXJExtraHpPercentCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_percent;
};
class crJXJExtraIsPlayerMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 返回true表示是玩家，false表示是NPC
public:
	crJXJExtraIsPlayerMethod();
	crJXJExtraIsPlayerMethod(const crJXJExtraIsPlayerMethod& handle);
	MethodClass(JXJ, JXJExtraIsPlayer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
};
class crJXJExtraHiddenMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 潜伏,当玩家一定时间内不动并且不攻击，则进入潜伏状态，进入潜伏状态返回true,否则返回false
public:
	crJXJExtraHiddenMethod();
	crJXJExtraHiddenMethod(const crJXJExtraHiddenMethod& handle);
	MethodClass(JXJ, JXJExtraHidden)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;//buf持续时间，0表示无穷
	float m_extraValue;//一定时间内不动
	bool m_start;
	float m_timer;
	float m_dtrec;
	CRCore::crVector3 m_lastposition;
};
class crJXJDebugInfoMethod : public CRCore::crMethod
{//Server & GameClient Method
public:
	crJXJDebugInfoMethod();
	crJXJDebugInfoMethod(const crJXJDebugInfoMethod& handle);
	MethodClass(JXJ, JXJDebugInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_str;
};
class crJXJExtraDamageCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 当玩家受到伤害>m_damageValue时返回true,否则返回false
public:
	crJXJExtraDamageCheckMethod();
	crJXJExtraDamageCheckMethod(const crJXJExtraDamageCheckMethod& handle);
	MethodClass(JXJ, JXJExtraDamageCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_rthp;
	float m_damageValue;
};
class crJXJExtraCancelMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 提前结束buf
public:
	crJXJExtraCancelMethod();
	crJXJExtraCancelMethod(const crJXJExtraCancelMethod& handle);
	MethodClass(JXJ, JXJExtraCancel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
protected:
	CRNetApp::crInstanceItem* m_this;
	bool* m_output;
};
}
#endif