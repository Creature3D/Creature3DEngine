/**********************************************************************
*
*	文件:	   appMethod8.h
*
*	描述:	   
*
*	作者:	   wxd
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD8_H
#define JXJ_APPMETHOD8_H
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
#include <CRText/crText.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <CRUI/crWidgetExtend.h>
namespace JXJ {
//class crJXJBossMarkMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJBossMarkMethod();
//	crJXJBossMarkMethod(const crJXJBossMarkMethod& handle);
//	MethodClass(JXJ, JXJBossMark)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_strCanvas;
//	std::string m_strMiniMap;
//	std::string m_strRotMap;
//	std::string m_strBossIcon;
//	std::string m_myIconWidget;
//};
class crJXJNotInSightMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJNotInSightMethod();
	crJXJNotInSightMethod(const crJXJNotInSightMethod& handle);
	MethodClass(JXJ, JXJNotInSight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void displayIcon(
		const float posx, 
		const float posy, 
		CRCore::crCanvasNode* canvas, 
		CRUI::crImageBoxWidgetNode* mapWidget, 
		CRCore::crWidgetNode* rotMapWidget, 
		CRCore::crWidgetNode* widget);
//	CRNetApp::crPlayerGameData *m_this;
	CRCore::crCanvasNode* m_this; 
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strMiniMap;
	std::string m_strRotMap;
	std::string m_strIcon;
	float m_invertal;
	int m_countOfWidgets;
};
class crJXJNotInSight2Logic : public CRCore::crNodeLogic
{//Client Method
public:
	crJXJNotInSight2Logic();
	crJXJNotInSight2Logic(const crJXJNotInSight2Logic& handle);
	LogicClass(JXJ, JXJNotInSight2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform *m_this;
	CREPARAM m_param;
	CRCore::ref_ptr<CRCore::crNodeLogic> m_playSceneFx;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPosition;
	std::string m_fileName;
};
class crJXJNotInSightUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJNotInSightUpdateMethod();
	crJXJNotInSightUpdateMethod(const crJXJNotInSightUpdateMethod& handle);
	MethodClass(JXJ, JXJNotInSightUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float* m_dt;
};
class crJXJVIPRevenuesBonusMethod : public CRCore::crMethod
{//Server Method
public:
	crJXJVIPRevenuesBonusMethod();
	crJXJVIPRevenuesBonusMethod(const crJXJVIPRevenuesBonusMethod& handle);
	MethodClass(JXJ, JXJVIPRevenuesBonus)
	virtual void operator()(crHandle& handle);
	virtual void inputParam(int i, void* param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int* m_cash1Ptr;
	int* m_cashPtr;
};
class crJXJVIPSacrificeBonusMethod : public CRCore::crMethod
{//Server Method
public:
	crJXJVIPSacrificeBonusMethod();
	crJXJVIPSacrificeBonusMethod(const crJXJVIPSacrificeBonusMethod& handle);
	MethodClass(JXJ, JXJVIPSacrificeBonus)
	virtual void operator()(crHandle& handle);
	virtual void inputParam(int i, void* param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int* m_cash1Ptr;
	int* m_cashPtr;
};
class crJXJVIPRecruitBonusMethod : public CRCore::crMethod
{//Server Method
public:
	crJXJVIPRecruitBonusMethod();
	crJXJVIPRecruitBonusMethod(const crJXJVIPRecruitBonusMethod& handle);
	MethodClass(JXJ, JXJVIPRecruitBonus)
	virtual void operator()(crHandle& handle);
	virtual void inputParam(int i, void* param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int* m_cash1Ptr;
	int* m_cashPtr;
};
class crJXJAdjustToushicheBulletSpeedMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJAdjustToushicheBulletSpeedMethod();
	crJXJAdjustToushicheBulletSpeedMethod(const crJXJAdjustToushicheBulletSpeedMethod& handle);
	MethodClass(JXJ, JXJAdjustToushicheBulletSpeed)
	virtual void operator()(crHandle& handle);
	virtual void inputParam(int i, void* param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRNetApp::crInstanceItem *m_firer;
	float m_zoffset;
	CRCore::crVector2 m_speedRange;
};
class crJXJConnectWithStationCheckMethod :public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJConnectWithStationCheckMethod();
	crJXJConnectWithStationCheckMethod(const crJXJConnectWithStationCheckMethod& handle);
	MethodClass(JXJ, JXJConnectWithStationCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool search(unsigned short searchid);
	CRNetApp::crPlayerGameData *m_this;
	unsigned short m_chengchiid;
	bool *m_output;
	std::set<unsigned short> m_myCitySet;
	std::set<unsigned short> m_searchSet;
};
class crJXJShowCityInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowCityInfoMethod();
	crJXJShowCityInfoMethod(const crJXJShowCityInfoMethod& handle);
	MethodClass(JXJ, JXJShowCityInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode *m_this;
	std::string m_mainCanvas;
	std::string m_tip1;
	std::string m_tip2;
	std::string m_chengchi_chanliang;
	std::string m_guishu_juntuan;
	std::string m_chengchi_techan;
	std::string m_xiangdui_dengji;
	std::string m_ling_name;
	std::string m_chengchi_type;
	std::string m_zhangguan_shenfen;
	std::string m_guojia;
	std::string m_zhoujun;
	std::string m_cityMap;
	std::string m_terrainInfo;
};
class crJXJStoveInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJStoveInitMethod();
	crJXJStoveInitMethod(const crJXJStoveInitMethod& handle);
	MethodClass(JXJ, JXJStoveInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_locks[3];
};
class crJXJFriendHmdMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFriendHmdMethod();
	crJXJFriendHmdMethod(const crJXJFriendHmdMethod& handle);
	MethodClass(JXJ, JXJFriendHmd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_hmdTitleSwitch;
	std::string m_contentSwitch;
};
class crJXJFriendHmdAddMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFriendHmdAddMethod();
	crJXJFriendHmdAddMethod(const crJXJFriendHmdAddMethod& handle);
	MethodClass(JXJ, JXJFriendHmdAdd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_thisCanvas;
	std::string m_titleSwitch;

};
}
#endif
