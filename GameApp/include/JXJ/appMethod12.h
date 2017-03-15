/**********************************************************************
*
*	文件:	   appMethod12.h
*
*	描述:	   
*
*	作者:	   李婧
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD12_H
#define JXJ_APPMETHOD12_H
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
namespace JXJ{
	class crJXJNewGuideGetRenWuRewardMethod : public CRCore::crMethod
	{//GameClient Method 
	public:
		crJXJNewGuideGetRenWuRewardMethod();
		crJXJNewGuideGetRenWuRewardMethod(const crJXJNewGuideGetRenWuRewardMethod& handle);
		MethodClass(JXJ, JXJNewGuideGetRenWuReward)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		//CRNetApp::crGameTask *m_this;
		std::string m_canvas;
		std::string m_expCount;
		std::string m_copCashCount;
		std::string m_oneCount;
		std::string m_twoCount;
		std::string m_rewardstr[2];
	};
class crJXJNewGuideSearchBuildingLvMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewGuideSearchBuildingLvMethod();
	crJXJNewGuideSearchBuildingLvMethod(const crJXJNewGuideSearchBuildingLvMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchBuildingLv)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_buildingid;//
	int m_targetlv;
};
class crJXJNewGuideSearchRecruitCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchRecruitCountMethod();
	crJXJNewGuideSearchRecruitCountMethod(const crJXJNewGuideSearchRecruitCountMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchRecruitCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_armid;
	unsigned char m_count;
};
class crJXJNewGuideSearchFormationInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchFormationInfoMethod();
	crJXJNewGuideSearchFormationInfoMethod(const crJXJNewGuideSearchFormationInfoMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchFormationInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_armid;
	unsigned char m_count;
};
class crJXJNewGuideSearchFubenSuccessMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchFubenSuccessMethod();
	crJXJNewGuideSearchFubenSuccessMethod(const crJXJNewGuideSearchFubenSuccessMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchFubenSuccess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	unsigned short m_fubenid;
};
class crJXJNewGuideSearchFormationCardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchFormationCardMethod();
	crJXJNewGuideSearchFormationCardMethod(const crJXJNewGuideSearchFormationCardMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchFormationCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	CRCore::crVector2i m_armid;
	CRCore::crVector2i m_count;
	CRCore::crVector2i m_cardid;
};
class crJXJNewGuideSearchTroopsTechMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchTroopsTechMethod();
	crJXJNewGuideSearchTroopsTechMethod(const crJXJNewGuideSearchTroopsTechMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchTroopsTech)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_troopstechid;
	int m_troopstechlv;
};
class crJXJNewGuideSearchRollCardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchRollCardMethod();
	crJXJNewGuideSearchRollCardMethod(const crJXJNewGuideSearchRollCardMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchRollCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_needrollcardcount;
};
class crJXJTaskSearchRollCardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchRollCardMethod();
	crJXJTaskSearchRollCardMethod(const crJXJTaskSearchRollCardMethod& handle);
	MethodClass(JXJ, JXJTaskSearchRollCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_oldrollcount;
};
class crJXJNewGuideSearchUpgradeCardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchUpgradeCardMethod();
	crJXJNewGuideSearchUpgradeCardMethod(const crJXJNewGuideSearchUpgradeCardMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchUpgradeCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_needupgradecardcount;
};
class crJXJNewGuideSearchTechMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchTechMethod();
	crJXJNewGuideSearchTechMethod(const crJXJNewGuideSearchTechMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchTech)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_techid;
	int m_techlv;
};
class crJXJNewGuideSearchMakeLordEquipMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewGuideSearchMakeLordEquipMethod();
	crJXJNewGuideSearchMakeLordEquipMethod(const crJXJNewGuideSearchMakeLordEquipMethod& handle);
	MethodClass(JXJ, JXJNewGuideSearchMakeLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_needcount;
	int m_needequipid;
};
class crJXJTaskSearchWearLordEquipMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchWearLordEquipMethod();
	crJXJTaskSearchWearLordEquipMethod(const crJXJTaskSearchWearLordEquipMethod& handle);
	MethodClass(JXJ, JXJTaskSearchWearLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_equipid;
};
class crJXJTaskSearchTroopsTechlvMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchTroopsTechlvMethod();
	crJXJTaskSearchTroopsTechlvMethod(const crJXJTaskSearchTroopsTechlvMethod& handle);
	MethodClass(JXJ, JXJTaskSearchTroopsTechlv)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	unsigned char m_troopslv;
};
class crJXJTaskSearchItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchItemMethod();
	crJXJTaskSearchItemMethod(const crJXJTaskSearchItemMethod& handle);
	MethodClass(JXJ, JXJTaskSearchItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_itemid;
	int m_itemcount;
};
class crJXJTaskSearchAppointCardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchAppointCardMethod();
	crJXJTaskSearchAppointCardMethod(const crJXJTaskSearchAppointCardMethod& handle);
	MethodClass(JXJ, JXJTaskSearchAppointCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	int m_itemid;
	int m_itemcount;
};
class crJXJTaskSearchLevyCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchLevyCountMethod();
	crJXJTaskSearchLevyCountMethod(const crJXJTaskSearchLevyCountMethod& handle);
	MethodClass(JXJ, JXJTaskSearchLevyCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
};
class crJXJTaskSearchMarketExchangeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskSearchMarketExchangeMethod();
	crJXJTaskSearchMarketExchangeMethod(const crJXJTaskSearchMarketExchangeMethod& handle);
	MethodClass(JXJ, JXJTaskSearchMarketExchange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
};
/////////////////////////////////////////
//
//crJXJUINewGuideCheckMethod
//新手引导检测
//
/////////////////////////////////////////
class crJXJUINewGuideCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUINewGuideCheckMethod();
	crJXJUINewGuideCheckMethod(const crJXJUINewGuideCheckMethod& handle);
	MethodClass(JXJ, JXJUINewGuideCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};
class crJXJNewGuideBtnTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewGuideBtnTouchMethod();
	crJXJNewGuideBtnTouchMethod(const crJXJNewGuideBtnTouchMethod& handle);
	MethodClass(JXJ, JXJNewGuideBtnTouch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::Timer_t m_time;
};
class crJXJNewGuideHyperTextTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewGuideHyperTextTouchMethod();
	crJXJNewGuideHyperTextTouchMethod(const crJXJNewGuideHyperTextTouchMethod& handle);
	MethodClass(JXJ, JXJNewGuideHyperTextTouch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJNewGuideEmptyHyperTextTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewGuideEmptyHyperTextTouchMethod();
	crJXJNewGuideEmptyHyperTextTouchMethod(const crJXJNewGuideEmptyHyperTextTouchMethod& handle);
	MethodClass(JXJ, JXJNewGuideEmptyHyperTextTouch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvNewguideCompleteMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvNewguideCompleteMethod();
	crJXJRecvNewguideCompleteMethod(const crJXJRecvNewguideCompleteMethod& handle);
	MethodClass(JXJ, JXJRecvNewguideComplete)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJChangeDataDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJChangeDataDisposeMethod();
	crJXJChangeDataDisposeMethod(const crJXJChangeDataDisposeMethod& handle);
	MethodClass(JXJ, JXJChangeDataDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> getPlayerData(int playerid);
	bool changeMoney(int playerid, int changecount,const std::string &name,const std::string &advertid);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_count;
};
class crJXJRecvChangeDataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvChangeDataMethod();
	crJXJRecvChangeDataMethod(const crJXJRecvChangeDataMethod& handle);
	MethodClass(JXJ, JXJRecvChangeData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCheckBattlePunishTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckBattlePunishTimeMethod();
	crJXJCheckBattlePunishTimeMethod(const crJXJCheckBattlePunishTimeMethod& handle);
	MethodClass(JXJ, JXJCheckBattlePunishTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_punitshtimecanvas;
};
class crJXJScenarioTalkMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJScenarioTalkMethod();
	crJXJScenarioTalkMethod(const crJXJScenarioTalkMethod& handle);
	MethodClass(JXJ, JXJScenarioTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasname;
	std::string m_hypertextname;
	std::string m_imageboxname;
	int m_talkid;
	int m_endmsg;
	std::string m_statictextname;
	int m_second;//关闭窗口延迟时间
	std::string m_scenarioId;//用于军团副本,当前军团副本NPC波数id
};
class crJXJScenarioTalkTouchMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJScenarioTalkTouchMethod();
	crJXJScenarioTalkTouchMethod(const crJXJScenarioTalkTouchMethod& handle);
	MethodClass(JXJ, JXJScenarioTalkTouch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJShowScenarioTalkMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowScenarioTalkMethod();
	crJXJShowScenarioTalkMethod(const crJXJShowScenarioTalkMethod& handle);
	MethodClass(JXJ, JXJShowScenarioTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvasname;
	int m_scenarioValue;
};
class crJXJSelGateUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelGateUpdateMethod();
	crJXJSelGateUpdateMethod(const crJXJSelGateUpdateMethod& handle);
	MethodClass(JXJ, JXJSelGateUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_serverNo[6];
	std::string m_serverName[6];
	std::string m_stats[6];
	std::string m_telecom[6];
	std::string m_cnc[6];
	std::string m_backboard[6];
	std::string m_pagenum;
};
class crJXJRoomStateTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRoomStateTipsMethod();
	crJXJRoomStateTipsMethod(const crJXJRoomStateTipsMethod& handle);
	MethodClass(JXJ, JXJRoomStateTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tipscanvas;
	std::string m_tipinput;
};
class crJXJGetConsortiaNameMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGetConsortiaNameMethod();
	crJXJGetConsortiaNameMethod(const crJXJGetConsortiaNameMethod& handle);
	MethodClass(JXJ, JXJGetConsortiaName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_playerid;
	std::string *m_consortianame;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJUINewFunctionOpenCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUINewFunctionOpenCheckMethod();
	crJXJUINewFunctionOpenCheckMethod(const crJXJUINewFunctionOpenCheckMethod& handle);
	MethodClass(JXJ, JXJUINewFunctionOpenCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJUINewFunctionBtnTouchMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUINewFunctionBtnTouchMethod();
	crJXJUINewFunctionBtnTouchMethod(const crJXJUINewFunctionBtnTouchMethod& handle);
	MethodClass(JXJ, JXJUINewFunctionBtnTouch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvUpdateNewFunctionMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvUpdateNewFunctionMethod();
	crJXJRecvUpdateNewFunctionMethod(const crJXJRecvUpdateNewFunctionMethod& handle);
	MethodClass(JXJ, JXJRecvUpdateNewFunction)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJScenarioTalkFx1Method
//////////////////////////////////////////////////////////////////////////
class crJXJScenarioTalkFx1Method :public CRCore::crMethod
{//Client  Method
public:
	crJXJScenarioTalkFx1Method();
	crJXJScenarioTalkFx1Method(const crJXJScenarioTalkFx1Method & handle);
	MethodClass(JXJ, JXJScenarioTalkFx1)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float* m_dt;
	float m_rotate;
	float m_totaltime;
	float m_alpha;
	CRCore::crVector3 m_vector3;
	float m_maxscale;
	CRCore::crMatrix m_mat;
	float m_nowscale;
};
//////////////////////////////////////////////////////////////////////////
//crJXJScenarioTalkOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJScenarioTalkOnShowMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJScenarioTalkOnShowMethod();
	crJXJScenarioTalkOnShowMethod(const crJXJScenarioTalkOnShowMethod & handle);
	MethodClass(JXJ, JXJScenarioTalkOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJExchangeResCanvasInitMethod :public CRCore::crMethod
{// client
public:
	crJXJExchangeResCanvasInitMethod();
	crJXJExchangeResCanvasInitMethod(const crJXJExchangeResCanvasInitMethod& handle);
	MethodClass(JXJ, JXJExchangeResCanvasInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_resourcemenu[4];
	std::string m_resourcelist[4];
};
//////////////////////////////////////////////////////////////////////////
//crJXJScenarioTalkOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJMarketExchangeResourceMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJMarketExchangeResourceMethod();
	crJXJMarketExchangeResourceMethod(const crJXJMarketExchangeResourceMethod & handle);
	MethodClass(JXJ, JXJMarketExchangeResource)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_resourcecurcount[4];
	std::string m_resourcesellcount[4];
	std::string m_resourcemenu[4];
	std::string m_resourceget[4];
	std::string m_exchangebtn[4];
	std::string m_countscrollbar[4];
	std::string m_remainCount;
	std::string m_nextlvCount;
};
class crJXJMarketSetCountScrolPosMethod :public CRCore::crMethod
{
public:
	crJXJMarketSetCountScrolPosMethod();
	crJXJMarketSetCountScrolPosMethod(const crJXJMarketSetCountScrolPosMethod& handle);
	MethodClass(JXJ, JXJMarketSetCountScrolPos);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crScrollBarWidgetNode *m_this;
	float m_min;
	float m_max;
	int	  m_type;
	std::string m_scrollbar;
	std::string m_input_f;
	std::string m_input_w;
	std::string m_input_i;
	std::string m_input_h;
	CRUI::crEditWidgetNode *m_sourceCount;
};
class crJXJUIMarketExchangeResMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMarketExchangeResMethod();
	crJXJUIMarketExchangeResMethod(const crJXJUIMarketExchangeResMethod& handle);
	MethodClass(JXJ, JXJUIMarketExchangeRes)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_font;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
};
class crJXJMarketExchangeResMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMarketExchangeResMethod();
	crJXJMarketExchangeResMethod(const crJXJMarketExchangeResMethod& handle);
	MethodClass(JXJ, JXJMarketExchangeRes)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvMarketExchangeResMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvMarketExchangeResMethod();
	crJXJRecvMarketExchangeResMethod(const crJXJRecvMarketExchangeResMethod& handle);
	MethodClass(JXJ, JXJRecvMarketExchangeRes)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJHuoyueduUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJHuoyueduUpdateMethod();
//	crJXJHuoyueduUpdateMethod(const crJXJHuoyueduUpdateMethod& handle);
//	MethodClass(JXJ, JXJHuoyueduUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	float* m_dt;
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvHuoyueduMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvHuoyueduMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvHuoyueduMethod();
	crJXJRecvHuoyueduMethod(const crJXJRecvHuoyueduMethod& handle);
	MethodClass(JXJ, JXJRecvHuoyuedu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	unsigned short m_id;
	unsigned char m_addstep;
};
class crJXJRecvHuoyueduUpdateMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvHuoyueduUpdateMethod();
	crJXJRecvHuoyueduUpdateMethod(const crJXJRecvHuoyueduUpdateMethod& handle);
	MethodClass(JXJ, JXJRecvHuoyueduUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	float m_time;
	std::string m_canvas;
};
class crJXJGetHuoyueduRewardIDMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGetHuoyueduRewardIDMethod();
	crJXJGetHuoyueduRewardIDMethod(const crJXJGetHuoyueduRewardIDMethod& handle);
	MethodClass(JXJ, JXJGetHuoyueduRewardID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJGetHuoyueduRewardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGetHuoyueduRewardMethod();
	crJXJGetHuoyueduRewardMethod(const crJXJGetHuoyueduRewardMethod& handle);
	MethodClass(JXJ, JXJGetHuoyueduReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvHuoyueduRewardMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvHuoyueduRewardMethod();
	crJXJRecvHuoyueduRewardMethod(const crJXJRecvHuoyueduRewardMethod& handle);
	MethodClass(JXJ, JXJRecvHuoyueduReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crPageValueChangeNoUpdateMethod : public CRCore::crMethod
{//Client
public:
	crPageValueChangeNoUpdateMethod();
	crPageValueChangeNoUpdateMethod(const crPageValueChangeNoUpdateMethod& handle);
	MethodClass(CRNetApp, PageValueChangeNoUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode* m_this;
	std::string m_pageSW;
};
class crHuoyueduPageUpdateMethod : public CRCore::crMethod
{//Client
public:
	crHuoyueduPageUpdateMethod();
	crHuoyueduPageUpdateMethod(const crHuoyueduPageUpdateMethod& handle);
	MethodClass(CRNetApp, HuoyueduPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gamelist;
	std::string m_progress;
	std::string m_progressinput;
	std::string m_rewardicon[18];
	std::string m_rewardcountinput[18];
	std::string m_rewardbtnsw[6];
	std::string m_rewardbtn[6];
	std::string m_rewardinput[6];
	std::string m_lvcontrolsw;//
};
class crJXJLingquFengluMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLingquFengluMethod();
	crJXJLingquFengluMethod(const crJXJLingquFengluMethod& handle);
	MethodClass(JXJ, JXJLingquFenglu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvLingquFengluMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvLingquFengluMethod();
	crJXJRecvLingquFengluMethod(const crJXJRecvLingquFengluMethod& handle);
	MethodClass(JXJ, JXJRecvLingquFenglu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crXiaoguanjiaBaodianUpdateMethod : public CRCore::crMethod
{//Client
public:
	crXiaoguanjiaBaodianUpdateMethod();
	crXiaoguanjiaBaodianUpdateMethod(const crXiaoguanjiaBaodianUpdateMethod& handle);
	MethodClass(CRNetApp, XiaoguanjiaBaodianUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gongnengkaifanginput;
	std::string m_banbenxinxiinput;
};
class crCheckHuoyueduCompleteMethod : public CRCore::crMethod
{//Client
public:
	crCheckHuoyueduCompleteMethod();
	crCheckHuoyueduCompleteMethod(const crCheckHuoyueduCompleteMethod& handle);
	MethodClass(CRNetApp, CheckHuoyueduComplete)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_huodongcanvas;
	std::string m_countbackboard;
	std::string m_countinput;
	std::string m_xiaoguanjiabtn;
};
class crJXJGMMailDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGMMailDisposeMethod();
	crJXJGMMailDisposeMethod(const crJXJGMMailDisposeMethod& handle);
	MethodClass(JXJ, JXJGMMailDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> getPlayerData(int playerid, CRDataBase::crDataBase *globalSession);
	bool changeMoney(int playerid, int changecount, CRDataBase::crDataBase *globalSession);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_count;
};
class crJXJGMNoticeDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGMNoticeDisposeMethod();
	crJXJGMNoticeDisposeMethod(const crJXJGMNoticeDisposeMethod& handle);
	MethodClass(JXJ, JXJGMNoticeDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> getPlayerData(int playerid, CRDataBase::crDataBase *globalSession);
	bool changeMoney(int playerid, int changecount, CRDataBase::crDataBase *globalSession);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJCanvasTableWidgetInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCanvasTableWidgetInitMethod();
	crJXJCanvasTableWidgetInitMethod(const crJXJCanvasTableWidgetInitMethod& handle);
	MethodClass(JXJ, JXJCanvasTableWidgetInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_tablename;
	std::vector<std::string> m_listnamevec;
};
class crJXJUIUpgradeResAddBuildingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpgradeResAddBuildingMethod();
	crJXJUIUpgradeResAddBuildingMethod(const crJXJUIUpgradeResAddBuildingMethod& handle);
	MethodClass(JXJ, JXJUIUpgradeResAddBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_font;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
};
class crJXJUpgradeResAddBuildingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpgradeResAddBuildingMethod();
	crJXJUpgradeResAddBuildingMethod(const crJXJUpgradeResAddBuildingMethod& handle);
	MethodClass(JXJ, JXJUpgradeResAddBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvUpgradeResAddBuildingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvUpgradeResAddBuildingMethod();
	crJXJRecvUpgradeResAddBuildingMethod(const crJXJRecvUpgradeResAddBuildingMethod& handle);
	MethodClass(JXJ, JXJRecvUpgradeResAddBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJResAddBuildingCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResAddBuildingCanvasUpdateMethod();
	crJXJResAddBuildingCanvasUpdateMethod(const crJXJResAddBuildingCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJResAddBuildingCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_canvasname;
	std::string m_levelname;
	std::string m_buildingnamesw;
	std::string m_inputname[9];
	std::string m_btnname[3];
};
class crJXJBuyResAddBuildingTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyResAddBuildingTimeMethod();
	crJXJBuyResAddBuildingTimeMethod(const crJXJBuyResAddBuildingTimeMethod& handle);
	MethodClass(JXJ, JXJBuyResAddBuildingTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvBuyResAddBuildingTimeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBuyResAddBuildingTimeMethod();
	crJXJRecvBuyResAddBuildingTimeMethod(const crJXJRecvBuyResAddBuildingTimeMethod& handle);
	MethodClass(JXJ, JXJRecvBuyResAddBuildingTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJOneKeyResAddCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOneKeyResAddCardMethod();
	crJXJOneKeyResAddCardMethod(const crJXJOneKeyResAddCardMethod& handle);
	MethodClass(JXJ, JXJOneKeyResAddCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvOneKeyResAddCardMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOneKeyResAddCardMethod();
	crJXJRecvOneKeyResAddCardMethod(const crJXJRecvOneKeyResAddCardMethod& handle);
	MethodClass(JXJ, JXJRecvOneKeyResAddCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJBuyResAddBuildingTimeShowDialogMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyResAddBuildingTimeShowDialogMethod();
	crJXJBuyResAddBuildingTimeShowDialogMethod(const crJXJBuyResAddBuildingTimeShowDialogMethod& handle);
	MethodClass(JXJ, JXJBuyResAddBuildingTimeShowDialog)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::crVector4 m_copperColor;
	std::string m_msgbox;
	std::string m_msg;
	std::string m_ok;
};

}
#endif
