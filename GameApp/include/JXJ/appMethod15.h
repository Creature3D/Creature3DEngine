/**********************************************************************
*
*	文件:	   appMethod15.h
*
*	描述:	   
*
*	作者:	   邹俊
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD15_H
#define JXJ_APPMETHOD15_H
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
#include <JXJ/appData3.h>
namespace JXJ{

/////////////////////////////////////////
//
//crJXJGetStartedDaysMethod
//取得开服天数
//
/////////////////////////////////////////
class crJXJGetStartedDaysMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetStartedDaysMethod();
	crJXJGetStartedDaysMethod(const crJXJGetStartedDaysMethod& handle);
	MethodClass(JXJ, JXJGetStartedDays)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	int *m_output;
};
////////////////////////////////
//
//crJXJCountryOfficialUpdateMethod
//国家官员选举结果
///////////////////////////////////
class crJXJCountryOfficialUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCountryOfficialUpdateMethod();
	crJXJCountryOfficialUpdateMethod(const crJXJCountryOfficialUpdateMethod& handle);
	MethodClass(CRNetApp, JXJCountryOfficialUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_PrimeMinister;
	std::string m_GreatMinister;
	std::string m_Daisyougun;
};
////////////////////////////////
//
//crJXJXuanJuTiShiUpdateMethod
//国家官员选举提示
///////////////////////////////////
class crJXJXuanJuTiShiUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJXuanJuTiShiUpdateMethod();
	crJXJXuanJuTiShiUpdateMethod(const crJXJXuanJuTiShiUpdateMethod& handle);
	MethodClass(CRNetApp, JXJXuanJuTiShiUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_Title;
	std::string m_Rule;
};
/////////////////////////////////////////
//
//crJXJCountryOfficialPollsUpdateMethod
//国家官员选举票数公布
/////////////////////////////////////////
class crJXJCountryOfficialPollsUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryOfficialPollsUpdateMethod();
	crJXJCountryOfficialPollsUpdateMethod(const crJXJCountryOfficialPollsUpdateMethod& handle);
	MethodClass(JXJ, JXJCountryOfficialPollsUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_Crops;//信息列表
	std::string m_CandidateControl;
};
////////////////////////////////////////
//
//crJXJCountryOfficialElectUpdateMethod
//国家官员选举
/////////////////////////////////////////
class crJXJCountryOfficialElectUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryOfficialElectUpdateMethod();
	crJXJCountryOfficialElectUpdateMethod(const crJXJCountryOfficialElectUpdateMethod& handle);
	MethodClass(JXJ, JXJCountryOfficialElectUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_Crops;//信息列表
	std::string m_Select[10];
	std::string m_Recommend;//选举按钮
	std::string m_CandidateControl;
	std::string m_checkbox_sw[10];
	std::string m_Recommend_sw;
};
/////////////////////////////////////////
//
//crJXJNewSelectCountryLeaderMethod
//
/////////////////////////////////////////
class crJXJNewSelectCountryLeaderMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewSelectCountryLeaderMethod();
	crJXJNewSelectCountryLeaderMethod(const crJXJNewSelectCountryLeaderMethod& handle);
	MethodClass(JXJ, JXJNewSelectCountryLeader)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox[10];
	std::string m_Recommend;
	std::string m_CandidateControl;
	std::string m_tableWidget;
};
/////////////////////////////////////////
//
//crJXJUINewChooseCheckBoxMethod
//
/////////////////////////////////////////
class crJXJUINewChooseCheckBoxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewChooseCheckBoxMethod();
	crJXJUINewChooseCheckBoxMethod(const crJXJUINewChooseCheckBoxMethod& handle);
	MethodClass(JXJ, JXJUINewChooseCheckBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox[10];
	std::string m_checkbox_sw[10];
	std::string m_Recommend;
	std::string m_CandidateControl;
	std::string m_Recommend_sw;
	std::string m_tableWidget;
};
/////////////////////////////////////////
//
//crJXJStartXuanJuTimerMethod
//选举开始时间
/////////////////////////////////////////
class crJXJStartXuanJuTimerMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJStartXuanJuTimerMethod();
	crJXJStartXuanJuTimerMethod(const crJXJStartXuanJuTimerMethod& handle);
	MethodClass(JXJ, JXJStartXuanJuTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_CandidateZiJiantimer;
};
////////////////////////////////
//
//crJXJHouXuanRenTipsUpdateMethod
//国家官员候选人自荐提示
///////////////////////////////////
class crJXJHouXuanRenTipsUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJHouXuanRenTipsUpdateMethod();
	crJXJHouXuanRenTipsUpdateMethod(const crJXJHouXuanRenTipsUpdateMethod& handle);
	MethodClass(CRNetApp, JXJHouXuanRenTipsUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_Title;
};
/////////////////////////////////////////
//
//crJXJGiveUpCountryCandidateMethod
//放弃成为国家官员候选人
/////////////////////////////////////////
class crJXJGiveUpCountryCandidateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGiveUpCountryCandidateMethod();
	crJXJGiveUpCountryCandidateMethod(const crJXJGiveUpCountryCandidateMethod& handle);
	MethodClass(JXJ, JXJGiveUpCountryCandidate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
/////////////////////////////////////////
//
//crJXJConfirmCountryCandidateMethod
//确认成为国家官员候选人
/////////////////////////////////////////
class crJXJConfirmCountryCandidateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJConfirmCountryCandidateMethod();
	crJXJConfirmCountryCandidateMethod(const crJXJConfirmCountryCandidateMethod& handle);
	MethodClass(JXJ, JXJConfirmCountryCandidate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
/**************************************************************************

 crJXJElectIconMethod
 //选举图标显示
**************************************************************************/
class crJXJElectIconMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJElectIconMethod ();
	crJXJElectIconMethod (const crJXJElectIconMethod & handle);
	MethodClass(JXJ,JXJElectIcon)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
/////////////////////////////////////////
//
//crJXJUIOfficialTipMethod
//官员领导力加成与职能提示
/////////////////////////////////////////
class crJXJUIOfficialTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIOfficialTipMethod();
	crJXJUIOfficialTipMethod(const crJXJUIOfficialTipMethod& handle);
	MethodClass(JXJ, JXJUIOfficialTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_noticeid;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};
/////////////////////////////////////////
//
//crJXJCountryCanvasCancleMethod
//州郡管理取消驻扎
/////////////////////////////////////////
class crJXJCountryCanvasCancleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryCanvasCancleMethod ();
	crJXJCountryCanvasCancleMethod (const crJXJCountryCanvasCancleMethod & handle);
	MethodClass(JXJ, JXJCountryCanvasCancle)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattlePreventAccMethod
//国战踢小号（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
class crJXJUIBattlePreventAccMethod:public CRCore::crMethod
{//Client
public:
	crJXJUIBattlePreventAccMethod();
	crJXJUIBattlePreventAccMethod(const crJXJUIBattlePreventAccMethod& handle);
	MethodClass(JXJ,JXJUIBattlePreventAcc)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_drag;
	std::string m_input[4];
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleCheckIdentifyingCodeMethod
//国战踢小号验证确认（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
class crJXJBattleCheckIdentifyingCodeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleCheckIdentifyingCodeMethod ();
	crJXJBattleCheckIdentifyingCodeMethod (const crJXJBattleCheckIdentifyingCodeMethod & handle);
	MethodClass(JXJ, JXJBattleCheckIdentifyingCode)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_edit;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvBattleCheckIdentifyingCodeMethod
//国战踢小号验证确认（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
class crJXJRecvBattleCheckIdentifyingCodeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvBattleCheckIdentifyingCodeMethod();
	crJXJRecvBattleCheckIdentifyingCodeMethod(const crJXJRecvBattleCheckIdentifyingCodeMethod& handle);
	MethodClass(JXJ, JXJRecvBattleCheckIdentifyingCode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRNetApp::crSceneServerPlayerData* m_player;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）消费提示
//////////////////////////////////////////////////////////////////////////
class crJXJUIBuyAppointColunmsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyAppointColunmsMethod();
	crJXJUIBuyAppointColunmsMethod(const crJXJUIBuyAppointColunmsMethod& handle);
	MethodClass(JXJ, JXJUIBuyAppointColunms)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	unsigned char m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）
//////////////////////////////////////////////////////////////////////////
class crJXJBuyAppointColunmsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyAppointColunmsMethod();
	crJXJBuyAppointColunmsMethod(const crJXJBuyAppointColunmsMethod& handle);
	MethodClass(JXJ, JXJBuyAppointColunms)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）
//////////////////////////////////////////////////////////////////////////
class crJXJRecvBuyAppointColunmsMethod :public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJRecvBuyAppointColunmsMethod();
	crJXJRecvBuyAppointColunmsMethod(const crJXJRecvBuyAppointColunmsMethod& handle);
	MethodClass(JXJ, JXJRecvBuyAppointColunms)
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
//crJXJDelayCanvasCloseMethod
//窗口延时关闭
//////////////////////////////////////////////////////////////////////////
class crJXJDelayCanvasCloseMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJDelayCanvasCloseMethod();
	crJXJDelayCanvasCloseMethod(const crJXJDelayCanvasCloseMethod &handle);
	MethodClass(JXJ,JXJDelayCanvasClose)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
};
/////////////////////////////////////////
//
//crJXJCameraOperatorDefaultMethod
//鼠标切换为默认状态
/////////////////////////////////////////
class crJXJCameraOperatorDefaultMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCameraOperatorDefaultMethod();
	crJXJCameraOperatorDefaultMethod(const crJXJCameraOperatorDefaultMethod& handle);
	MethodClass(JXJ, JXJCameraOperatorDefault)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
/////////////////////////////////////////
//
//crJXJCheckIsMyselfMethod
//检查是否刷新为玩家自己的装备信息
/////////////////////////////////////////
class crJXJCheckIsMyselfMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckIsMyselfMethod();
	crJXJCheckIsMyselfMethod(const crJXJCheckIsMyselfMethod& handle);
	MethodClass(JXJ, JXJCheckIsMyself)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowZhengzhaoBtnCanvasMethod
//征召按钮窗口显示
//////////////////////////////////////////////////////////////////////////
class crJXJShowZhengzhaoBtnCanvasMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJShowZhengzhaoBtnCanvasMethod();
	crJXJShowZhengzhaoBtnCanvasMethod(const crJXJShowZhengzhaoBtnCanvasMethod &handle);
	MethodClass(JXJ,JXJShowZhengzhaoBtnCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_Canvas;
	std::string m_minimapCanvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckZhengZhaoBtnMethod
//征召按钮控制
//////////////////////////////////////////////////////////////////////////
class crJXJCheckZhengZhaoBtnMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckZhengZhaoBtnMethod();
	crJXJCheckZhengZhaoBtnMethod(const crJXJCheckZhengZhaoBtnMethod& handle);
	MethodClass(JXJ, JXJCheckZhengZhaoBtn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_guojiazhengzhao;
	std::string m_juntuanzhengzhao;
	std::string m_juntuan_sw;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJZhengZhaoFaBuCanvasMethod
//征召发布窗口
//////////////////////////////////////////////////////////////////////////
class crJXJZhengZhaoFaBuCanvasMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJZhengZhaoFaBuCanvasMethod();
	crJXJZhengZhaoFaBuCanvasMethod(const crJXJZhengZhaoFaBuCanvasMethod& handle);
	MethodClass(JXJ, JXJZhengZhaoFaBuCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	unsigned char m_index;
	std::string m_mark;
	std::string m_newcanvas;
	std::string m_inputAttack;
	std::string m_tips;
	std::string m_inputDefend;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJReleaseGuoJiaZhengZhaoMethod
//发布国家(军团)征召
//////////////////////////////////////////////////////////////////////////
class crJXJReleaseGuoJiaZhengZhaoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJReleaseGuoJiaZhengZhaoMethod();
	crJXJReleaseGuoJiaZhengZhaoMethod(const crJXJReleaseGuoJiaZhengZhaoMethod& handle);
	MethodClass(JXJ, JXJReleaseGuoJiaZhengZhao)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_fuyan;
	std::string m_mark;
	std::string m_canvasname;
	std::string m_chatmodesw;
	std::string m_chatinput;
	std::string m_sendbtn;
	std::string m_switch;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvReleaseGuoJiaZhengZhaoMethod
//发布国家征召
//////////////////////////////////////////////////////////////////////////
class crJXJRecvReleaseGuoJiaZhengZhaoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvReleaseGuoJiaZhengZhaoMethod();
	crJXJRecvReleaseGuoJiaZhengZhaoMethod(const crJXJRecvReleaseGuoJiaZhengZhaoMethod& handle);
	MethodClass(JXJ, JXJRecvReleaseGuoJiaZhengZhao)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_netType;
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvasname;
	std::string m_chatmodesw;
	std::string m_chatinput;
	std::string m_sendbtn;
	std::string m_switch;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvReleaseJuanTuanZhengZhaoMethod
//发布军团征召
//////////////////////////////////////////////////////////////////////////
class crJXJRecvReleaseJuanTuanZhengZhaoMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvReleaseJuanTuanZhengZhaoMethod();
	crJXJRecvReleaseJuanTuanZhengZhaoMethod(const crJXJRecvReleaseJuanTuanZhengZhaoMethod& handle);
	MethodClass(JXJ, JXJRecvReleaseJuanTuanZhengZhao)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_netType;
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGainZhengZhaoLingVecMethod
//生成征召令信息
//////////////////////////////////////////////////////////////////////////
class crJXJGainZhengZhaoLingVecMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGainZhengZhaoLingVecMethod();
	crJXJGainZhengZhaoLingVecMethod(const crJXJGainZhengZhaoLingVecMethod& handle);
	MethodClass(JXJ, JXJGainZhengZhaoLingVec)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_netType;
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJShowYingZhengCanvasMethod
//显示国家应征窗口
//////////////////////////////////////////////////////////////////////////
class crJXJShowYingZhengCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowYingZhengCanvasMethod();
	crJXJShowYingZhengCanvasMethod(const crJXJShowYingZhengCanvasMethod& handle);
	MethodClass(JXJ, JXJShowYingZhengCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_type;
	std::string m_publisher;
	std::string m_battleName;
	std::string m_army[3];
	std::string m_army_sw[3];
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
	std::string m_armyCount[3];
	std::string m_rewardInstruction;
	std::string m_timer;
	std::string m_message;
	std::string m_newcanvas;
	std::string m_formationIcon[3];
	std::string m_newArmyCount[3];
	std::string m_leftPageBtn;
	std::string m_rightPageBtn;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGuanYuanLingTimerUpdateMethod
//官员令存在时间刷新
//////////////////////////////////////////////////////////////////////////
class crJXJGuanYuanLingTimerUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJGuanYuanLingTimerUpdateMethod();
	crJXJGuanYuanLingTimerUpdateMethod(const crJXJGuanYuanLingTimerUpdateMethod &handle);
	MethodClass(JXJ,JXJGuanYuanLingTimerUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_guanyuanling;
	std::string m_juntuanling;
	std::string m_count_1;
	std::string m_count_2;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClickZhengZhaoLingBtnMethod
//单击征召令弹窗
//////////////////////////////////////////////////////////////////////////
class crJXJClickZhengZhaoLingBtnMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJClickZhengZhaoLingBtnMethod();
	crJXJClickZhengZhaoLingBtnMethod(const crJXJClickZhengZhaoLingBtnMethod &handle);
	MethodClass(JXJ,JXJClickZhengZhaoLingBtn)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	unsigned char m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJYingZhengCanvasTurnPageMethod
//应征窗口翻页
//////////////////////////////////////////////////////////////////////////
class crJXJYingZhengCanvasTurnPageMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJYingZhengCanvasTurnPageMethod();
	crJXJYingZhengCanvasTurnPageMethod(const crJXJYingZhengCanvasTurnPageMethod &handle);
	MethodClass(JXJ,JXJYingZhengCanvasTurnPage)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_mark;
	unsigned char m_index; //1表示左翻页，2表示右翻页
};
//////////////////////////////////////////////////////////////////////////
//crJXJClearSameBattleInfoMethod
//清除征召令缓存列表同一战场的邀请信息
//////////////////////////////////////////////////////////////////////////
class crJXJClearSameBattleInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClearSameBattleInfoMethod();
	crJXJClearSameBattleInfoMethod(const crJXJClearSameBattleInfoMethod& handle);
	MethodClass(JXJ, JXJClearSameBattleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJInputZhengZhaoBattleInfoMethod
//设置应征战场信息
//////////////////////////////////////////////////////////////////////////
class crJXJInputZhengZhaoBattleInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJInputZhengZhaoBattleInfoMethod();
	crJXJInputZhengZhaoBattleInfoMethod(const crJXJInputZhengZhaoBattleInfoMethod& handle);
	MethodClass(JXJ, JXJInputZhengZhaoBattleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJYingZhengEnterBattleDequeMethod
//应征进入战场排队
//////////////////////////////////////////////////////////////////////////
class crJXJYingZhengEnterBattleDequeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJYingZhengEnterBattleDequeMethod();
	crJXJYingZhengEnterBattleDequeMethod(const crJXJYingZhengEnterBattleDequeMethod& handle);
	MethodClass(JXJ, JXJYingZhengEnterBattleDeque)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCheckYingzhengRewardMethod
//检查是否发放应征奖励
//////////////////////////////////////////////////////////////////////////
class crJXJCheckYingzhengRewardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckYingzhengRewardMethod();
	crJXJCheckYingzhengRewardMethod(const crJXJCheckYingzhengRewardMethod& handle);
	MethodClass(JXJ, JXJCheckYingzhengReward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJYingzhengRewardMethod
//应征出兵奖励
//////////////////////////////////////////////////////////////////////////
class crJXJYingzhengRewardMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJYingzhengRewardMethod();
	crJXJYingzhengRewardMethod(const crJXJYingzhengRewardMethod& handle);
	MethodClass(JXJ, JXJYingzhengReward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowYingZhengTimerMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJShowYingZhengTimerMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJShowYingZhengTimerMethod();
	crJXJShowYingZhengTimerMethod(const crJXJShowYingZhengTimerMethod &handle);
	MethodClass(JXJ,JXJShowYingZhengTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRUI::crStaticTextWidgetNode *m_this;
};
/////////////////////////////////////////
//
//crJXJCheckOrangeSuitEquipMethod
//橙色套装成就（传奇神兵）
/////////////////////////////////////////
class crJXJCheckOrangeSuitEquipMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckOrangeSuitEquipMethod();
	crJXJCheckOrangeSuitEquipMethod(const crJXJCheckOrangeSuitEquipMethod& handle);
	MethodClass(JXJ, JXJCheckOrangeSuitEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_itemtype;
	int m_equipcolor;
};
/////////////////////////////////////////
//
//crJXJSearchOrangeSuitEquipMethod
//检查橙色套装
/////////////////////////////////////////
class crJXJSearchOrangeSuitEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchOrangeSuitEquipMethod();
	crJXJSearchOrangeSuitEquipMethod(const crJXJSearchOrangeSuitEquipMethod& handle);
	MethodClass(JXJ, JXJSearchOrangeSuitEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_itemtype;
	int m_equipcolor;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSetGuoZhanVictoryTimesMethod
//记录国战胜利次数（分为攻城胜利和守城胜利）
//////////////////////////////////////////////////////////////////////////
class crJXJSetGuoZhanVictoryTimesMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJSetGuoZhanVictoryTimesMethod();
	crJXJSetGuoZhanVictoryTimesMethod(const crJXJSetGuoZhanVictoryTimesMethod& handle);
	MethodClass(JXJ, JXJSetGuoZhanVictoryTimes)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
////////////////////////////////////////////////
//
//crJXJCheckGuoZhanAttackVictoryTimesMethod
//国战攻城胜利次数
///////////////////////////////////////////////
class crJXJCheckGuoZhanAttackVictoryTimesMethod :public CRCore::crMethod
{//GameServer Method 成就关于国战攻城胜利
public:
	crJXJCheckGuoZhanAttackVictoryTimesMethod();
	crJXJCheckGuoZhanAttackVictoryTimesMethod(const crJXJCheckGuoZhanAttackVictoryTimesMethod& handle);
	MethodClass(JXJ, JXJCheckGuoZhanAttackVictoryTimes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
////////////////////////////////////////////////
//
//crJXJSearchGuoZhanAttackVictoryTimesMethod
//国战攻城胜利次数
///////////////////////////////////////////////
class crJXJSearchGuoZhanAttackVictoryTimesMethod :public CRCore::crMethod
{//Client Method 成就关于国战攻城胜利
public:
	crJXJSearchGuoZhanAttackVictoryTimesMethod();
	crJXJSearchGuoZhanAttackVictoryTimesMethod(const crJXJSearchGuoZhanAttackVictoryTimesMethod& handle);
	MethodClass(JXJ, JXJSearchGuoZhanAttackVictoryTimes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
////////////////////////////////////////////////
//
//crJXJCheckGuoZhanDefendVictoryTimesMethod
//国战守城胜利次数
///////////////////////////////////////////////
class crJXJCheckGuoZhanDefendVictoryTimesMethod :public CRCore::crMethod
{//GameServer Method 成就关于国战守城胜利
public:
	crJXJCheckGuoZhanDefendVictoryTimesMethod();
	crJXJCheckGuoZhanDefendVictoryTimesMethod(const crJXJCheckGuoZhanDefendVictoryTimesMethod& handle);
	MethodClass(JXJ, JXJCheckGuoZhanDefendVictoryTimes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
////////////////////////////////////////////////
//
//crJXJSearchGuoZhanDefendVictoryTimesMethod
//国战守城胜利次数
///////////////////////////////////////////////
class crJXJSearchGuoZhanDefendVictoryTimesMethod :public CRCore::crMethod
{//Client Method 成就关于国战守城胜利
public:
	crJXJSearchGuoZhanDefendVictoryTimesMethod();
	crJXJSearchGuoZhanDefendVictoryTimesMethod(const crJXJSearchGuoZhanDefendVictoryTimesMethod& handle);
	MethodClass(JXJ, JXJSearchGuoZhanDefendVictoryTimes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
/////////////////////////////////////////
//
//crJXJRecvTouchRandomExtraMethod
//获取随机BUFF
/////////////////////////////////////////
class crJXJRecvTouchRandomExtraMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvTouchRandomExtraMethod();
	crJXJRecvTouchRandomExtraMethod(const crJXJRecvTouchRandomExtraMethod& handle);
	MethodClass(JXJ, JXJRecvTouchRandomExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	float m_range;
	std::vector<short> m_idvec;
};
/////////////////////////////////////////
//
//crJXJCollideItemMethod
//
/////////////////////////////////////////
class crJXJCollideItemMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCollideItemMethod();
	crJXJCollideItemMethod(const crJXJCollideItemMethod& handle);
	MethodClass(JXJ, JXJCollideItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRNetApp::crInstanceItem* m_item;
};
/////////////////////////////////////////
//
//crJXJRecvCollideRandomExtraMethod
//获取随机BUFF
/////////////////////////////////////////
class crJXJRecvCollideRandomExtraMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvCollideRandomExtraMethod();
	crJXJRecvCollideRandomExtraMethod(const crJXJRecvCollideRandomExtraMethod& handle);
	MethodClass(JXJ, JXJRecvCollideRandomExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
/////////////////////////////////////////
//
//crJXJAcceptZhengZhaoEnterBattleMethod
//玩家通过应征进入战场时发出通告
/////////////////////////////////////////
class crJXJAcceptZhengZhaoEnterBattleMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJAcceptZhengZhaoEnterBattleMethod();
	crJXJAcceptZhengZhaoEnterBattleMethod(const crJXJAcceptZhengZhaoEnterBattleMethod& handle);
	MethodClass(JXJ, JXJAcceptZhengZhaoEnterBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crSceneServerPlayerData> m_playerData;
};
/////////////////////////////////////////
//
//crJXJReliveItemsFromTabExMethod
//根据当前波数加载对应的NPC
/////////////////////////////////////////
class crJXJReliveItemsFromTabExMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJReliveItemsFromTabExMethod ();
	crJXJReliveItemsFromTabExMethod (const crJXJReliveItemsFromTabExMethod & handle);
	MethodClass(JXJ, JXJReliveItemsFromTabEx)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crRoom* m_this;
	std::string m_nameid;
};
/////////////////////////////////////////
//
//crJXJCheckItemsDeadFromTabExMethod
//根据杀死NPC的时间来判定是否跳跃波数
/////////////////////////////////////////
class crJXJCheckItemsDeadFromTabExMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crJXJCheckItemsDeadFromTabExMethod();
	crJXJCheckItemsDeadFromTabExMethod(const crJXJCheckItemsDeadFromTabExMethod& handle);
	MethodClass(JXJ, JXJCheckItemsDeadFromTabEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	std::string m_item;
};
/////////////////////////////////////////
//
//crJXJSetWidgetEnableExOpenLvMethod
//根据功能开放等级设置相应控件是否可用
/////////////////////////////////////////
class crJXJSetWidgetEnableExOpenLvMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetWidgetEnableExOpenLvMethod();
	crJXJSetWidgetEnableExOpenLvMethod(const crJXJSetWidgetEnableExOpenLvMethod& handle);
	MethodClass(JXJ, JXJSetWidgetEnableExOpenLv)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_button;
	bool m_enable;
	int m_index;
};
/////////////////////////////////////////
//
//crJXJSyncScenarioVariableMethod
//sceneServer变量值同步到客户端
/////////////////////////////////////////
class crJXJSyncScenarioVariableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSyncScenarioVariableMethod();
	crJXJSyncScenarioVariableMethod(const crJXJSyncScenarioVariableMethod& handle);
	MethodClass(JXJ, JXJSyncScenarioVariable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
/////////////////////////////////////////
//
//crJXJPlayerAutoBattleMethod
//玩家自动战斗
/////////////////////////////////////////
class crJXJPlayerAutoBattleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerAutoBattleMethod();
	crJXJPlayerAutoBattleMethod(const crJXJPlayerAutoBattleMethod& handle);
	MethodClass(JXJ, JXJPlayerAutoBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_canvas;
	std::string m_newCanvas;
	std::string m_autoBattleBtn;
	std::string m_cameraLockBtn;
	bool m_cancleCameraLock;
};
/////////////////////////////////////////
//
//crJXJStartAutoBattleMethod
//通过快捷方式（ctrl+A）启用自动战斗
/////////////////////////////////////////
class crJXJStartAutoBattleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJStartAutoBattleMethod();
	crJXJStartAutoBattleMethod(const crJXJStartAutoBattleMethod& handle);
	MethodClass(JXJ, JXJStartAutoBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_canvas;
	std::string m_autoBattleBtn;
	CRProducer::crGUIEventAdapter* m_ea;
	char m_key;
};
/////////////////////////////////////////////////////////////////////////
//
//crJXJShowAutoBattleCanvasMethod
//
/////////////////////////////////////////////////////////////////////////
class crJXJShowAutoBattleCanvasMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJShowAutoBattleCanvasMethod();
	crJXJShowAutoBattleCanvasMethod(const crJXJShowAutoBattleCanvasMethod &handle);
	MethodClass(JXJ,JXJShowAutoBattleCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_noticeCanvas;
	std::string m_showCanvas;
	std::string m_autoBattleCanvas;
	std::string m_autoBattleBtn;
	std::string m_zhenglvCanvas;
	std::string m_fubenCanvas;
};
/////////////////////////////////////////
//
//crJXJAutoBattleButtonTipsMethod
//自动战斗提示
/////////////////////////////////////////
class crJXJAutoBattleButtonTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAutoBattleButtonTipsMethod();
	crJXJAutoBattleButtonTipsMethod(const crJXJAutoBattleButtonTipsMethod& handle);
	MethodClass(JXJ, JXJAutoBattleButtonTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_headCanvas;
	std::string m_title;
	std::string m_text;
	std::string m_textbk;
	std::string m_textdi;
};
/////////////////////////////////////////
//
//crJXJGameIntroduceCanvasUpdateMethod
//游戏宝典窗口刷新
/////////////////////////////////////////
class crJXJGameIntroduceCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameIntroduceCanvasUpdateMethod();
	crJXJGameIntroduceCanvasUpdateMethod(const crJXJGameIntroduceCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJGameIntroduceCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_pageradio; 
	std::string m_pagesw;
	std::string m_pageName_1;
	std::string m_pageName_2;
	std::string m_pageName_3;
	std::string m_pageName_4;
	std::string m_canvas;
	std::string m_checkBoxNoTips; //游戏宝典不再显示
};
/////////////////////////////////////////
//
//crJXJLevelUpIntroduceCanvasUpdateMethod
//升级指南页面刷新
/////////////////////////////////////////
class crJXJLevelUpIntroduceCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLevelUpIntroduceCanvasUpdateMethod();
	crJXJLevelUpIntroduceCanvasUpdateMethod(const crJXJLevelUpIntroduceCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJLevelUpIntroduceCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_title; 
	std::string m_introduction;
	std::string m_image1_sw;
	std::string m_image2_sw;
	std::string m_image3_sw;
	std::string m_image4_sw;
	std::string m_pageBtn_R;
	std::string m_pageBtn_L;
	std::string m_canvas;
	std::string m_imagePage_R;
	std::string m_imagePage_L;
};
/////////////////////////////////////////
//
//crJXJCombatEffectivenessIntroduceCanvasUpdateMethod
//战斗力提升来源页面刷新
/////////////////////////////////////////
class crJXJCombatEffectivenessIntroduceCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCombatEffectivenessIntroduceCanvasUpdateMethod();
	crJXJCombatEffectivenessIntroduceCanvasUpdateMethod(const crJXJCombatEffectivenessIntroduceCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJCombatEffectivenessIntroduceCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_title;
	std::string m_childCanvas_sw;
	std::string m_canvas;
};
/////////////////////////////////////////
//
//crJXJCountryBattleIntroduceCanvasUpdateMethod
//国战指南页面刷新
/////////////////////////////////////////
class crJXJCountryBattleIntroduceCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryBattleIntroduceCanvasUpdateMethod();
	crJXJCountryBattleIntroduceCanvasUpdateMethod(const crJXJCountryBattleIntroduceCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJCountryBattleIntroduceCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_title; 
	std::string m_image_sw;
	std::string m_pageBtn_R;
	std::string m_pageBtn_L;
	std::string m_canvas;
};
/////////////////////////////////////////
//
//crJXJAdvancedMasterCanvasUpdateMethod
//高手进阶页面刷新
/////////////////////////////////////////
class crJXJAdvancedMasterCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAdvancedMasterCanvasUpdateMethod();
	crJXJAdvancedMasterCanvasUpdateMethod(const crJXJAdvancedMasterCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJAdvancedMasterCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_title_1; 
	std::string m_title_2;
	std::string m_title_3; 
	std::string m_title_4;
	std::string m_content1_1;
	std::string m_content1_2;
	std::string m_content1_3;
	std::string m_content1_4;
	std::string m_content2_1;
	std::string m_content2_2;
	std::string m_content2_3;
	std::string m_content2_4;
	std::string m_content3_1;
	std::string m_content3_2;
	std::string m_content3_3;
	std::string m_content3_4;
	std::string m_content4_1;
	std::string m_content4_2;
	std::string m_canvas;
	std::string m_mainTitle;
};
/////////////////////////////////////////
//
//crJXJGameIntroduceCanvasTurnPageMethod
//新手攻略翻页
/////////////////////////////////////////
class crJXJGameIntroduceCanvasTurnPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameIntroduceCanvasTurnPageMethod();
	crJXJGameIntroduceCanvasTurnPageMethod(const crJXJGameIntroduceCanvasTurnPageMethod& handle);
	MethodClass(JXJ, JXJGameIntroduceCanvasTurnPage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	unsigned char m_index; 
	std::string m_swnode;
};
/////////////////////////////////////////
//
//crJXJIconDetailIntroduceMethod
//战斗力提升页面点击Icon弹窗
/////////////////////////////////////////
class crJXJIconDetailIntroduceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIconDetailIntroduceMethod();
	crJXJIconDetailIntroduceMethod(const crJXJIconDetailIntroduceMethod& handle);
	MethodClass(JXJ, JXJIconDetailIntroduce)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	unsigned char m_index;
	std::string m_main_sw;
	std::string m_image_sw;
	std::string m_introduction;
};
/////////////////////////////////////////
//
//crJXJAdvancedMasterForumNavigatorMethod
//高手进阶论坛导航
/////////////////////////////////////////
class crJXJAdvancedMasterForumNavigatorMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAdvancedMasterForumNavigatorMethod();
	crJXJAdvancedMasterForumNavigatorMethod(const crJXJAdvancedMasterForumNavigatorMethod& handle);
	MethodClass(JXJ, JXJAdvancedMasterForumNavigator)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_url;
	std::string m_txUrl;
};
/////////////////////////////////////////
//
//crJXJHideGameIntroductionIconMethod
//隐藏领地界面新手宝典图标
/////////////////////////////////////////
class crJXJHideGameIntroductionIconMethod:public CRCore::crMethod
{//Client
public:
	crJXJHideGameIntroductionIconMethod();
	crJXJHideGameIntroductionIconMethod(const crJXJHideGameIntroductionIconMethod& handle);
	MethodClass(JXJ, JXJHideGameIntroductionIcon)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	int m_netType;
	std::string m_noTipsCheckBox;
};
///////////////////////////////////////////////////
//
//crJXJRecvHideGameIntroductionIconMethod
//隐藏领地界面新手宝典图标
//////////////////////////////////////////////////
class crJXJRecvHideGameIntroductionIconMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvHideGameIntroductionIconMethod();
	crJXJRecvHideGameIntroductionIconMethod(const crJXJRecvHideGameIntroductionIconMethod& handle);
	MethodClass(JXJ, JXJRecvHideGameIntroductionIcon)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
/////////////////////////////////////////
//
//crJXJGainConsortiaStartPromotionVecMethod
//生成军团开服活动满足资格成员列表
/////////////////////////////////////////
class crJXJGainConsortiaStartPromotionVecMethod: public CRCore::crMethod
{//GameServer Method
public:
	crJXJGainConsortiaStartPromotionVecMethod();
	crJXJGainConsortiaStartPromotionVecMethod(const crJXJGainConsortiaStartPromotionVecMethod& handle);
	MethodClass(JXJ,JXJGainConsortiaStartPromotionVec)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
// class crJXJCheckNotificationCanvasIndexMethod:public CRCore::crMethod
// {//Client
// public:
// 	crJXJCheckNotificationCanvasIndexMethod();
// 	crJXJCheckNotificationCanvasIndexMethod(const crJXJCheckNotificationCanvasIndexMethod& handle);
// 	MethodClass(JXJ, JXJCheckNotificationCanvasIndex)
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// 	virtual void operator()(crHandle &handle);
// protected:
// 	CRNetApp::crPlayerGameData *m_this;
// 	int m_netType;
// 	std::string m_noticeCanvas;
// };
////////////////////////////////////////////////////
//crJXJQueryConsortiaPromotionDataMethod
//查询军团开服活动数据 (Client)
////////////////////////////////////////////////////
class crJXJQueryConsortiaPromotionDataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryConsortiaPromotionDataMethod();
	crJXJQueryConsortiaPromotionDataMethod(const crJXJQueryConsortiaPromotionDataMethod& handle);
	MethodClass(JXJ, JXJQueryConsortiaPromotionData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
////////////////////////////////////////////////////
//crJXJRecvQueryConsortiaPromotionDataMethod
//查询军团开服活动数据 (Server)
////////////////////////////////////////////////////
class crJXJRecvQueryConsortiaPromotionDataMethod:public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJRecvQueryConsortiaPromotionDataMethod();
	crJXJRecvQueryConsortiaPromotionDataMethod(const crJXJRecvQueryConsortiaPromotionDataMethod& handle);
	MethodClass(JXJ, JXJRecvQueryConsortiaPromotionData)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
/////////////////////////////////////////
//
//crJXJConsortiaStartedPromotionCanvasUpdateMethod
//军团开服活动窗口刷新
/////////////////////////////////////////
class crJXJConsortiaStartedPromotionCanvasUpdateMethod: public CRCore::crMethod
{//Client Method
public:
	crJXJConsortiaStartedPromotionCanvasUpdateMethod();
	crJXJConsortiaStartedPromotionCanvasUpdateMethod(const crJXJConsortiaStartedPromotionCanvasUpdateMethod& handle);
	MethodClass(JXJ,JXJConsortiaStartedPromotionCanvasUpdate)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_consortiaName_rank[5];
	std::string m_consortiaLeader_rank[5];
	std::string m_leaderExtraReward[5];
	std::string m_getRewardBtn[5];
	std::string m_icon[5][4];
	std::string m_itemCount[5][4];
	std::string m_introductionText;
	std::string m_timeText;
	std::string m_newCanvas;
};
///////////////////////////////////////////////////
//
//crJXJUIGetConsortiaPromotionAwardMethod
//Client领取军团开服活动奖励
//////////////////////////////////////////////////
class crJXJUIGetConsortiaPromotionAwardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGetConsortiaPromotionAwardMethod();
	crJXJUIGetConsortiaPromotionAwardMethod(const crJXJUIGetConsortiaPromotionAwardMethod& handle);
	MethodClass(JXJ, JXJUIGetConsortiaPromotionAward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	char m_index;
};
///////////////////////////////////////////////////
//
//crJXJRecvGetConsortiaPromotionAwardMethod
//发放军团开服活动奖励
//////////////////////////////////////////////////
class crJXJRecvGetConsortiaPromotionAwardMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGetConsortiaPromotionAwardMethod();
	crJXJRecvGetConsortiaPromotionAwardMethod(const crJXJRecvGetConsortiaPromotionAwardMethod& handle);
	MethodClass(JXJ, JXJRecvGetConsortiaPromotionAward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////
//
//crJXJConsortiaPromotionCanvasShowTimerMethod
//军团活动界面倒计时
//////////////////////////////////////////////////////
class crJXJConsortiaPromotionCanvasShowTimerMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJConsortiaPromotionCanvasShowTimerMethod();
	crJXJConsortiaPromotionCanvasShowTimerMethod(const crJXJConsortiaPromotionCanvasShowTimerMethod &handle);
	MethodClass(JXJ,JXJConsortiaPromotionCanvasShowTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRUI::crHypertextWidgetNode *m_this;
};
/////////////////////////////////////////////////////
//
//crJXJCheckManorSceneCouldGetAwardMethod
//检查是否有可领取的俸禄，赋税，免费刷将等奖励
////////////////////////////////////////////////////
class crJXJCheckManorSceneCouldGetAwardMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJCheckManorSceneCouldGetAwardMethod();
	crJXJCheckManorSceneCouldGetAwardMethod(const crJXJCheckManorSceneCouldGetAwardMethod &handle);
	MethodClass(JXJ, JXJCheckManorSceneCouldGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_textInfo[3];
	std::string m_canvasChildNode;
};
/////////////////////////////////////////////////////
//
//crJXJShowRelationAwardCanvasMethod
//打开俸禄，赋税，活跃度，免费刷将窗口
////////////////////////////////////////////////////
class crJXJShowRelationAwardCanvasMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJShowRelationAwardCanvasMethod();
	crJXJShowRelationAwardCanvasMethod(const crJXJShowRelationAwardCanvasMethod &handle);
	MethodClass(JXJ, JXJShowRelationAwardCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	char m_index;
	std::string m_canvas[4];
	int m_itemId;
	int m_itemIdshiji;
};
///////////////////////////////////////////////
//
//crJXJBackCityButtonCoolTimerUpdateMethod
//回城按钮冷却时间检测
//////////////////////////////////////////////
class crJXJBackCityButtonCoolTimerUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJBackCityButtonCoolTimerUpdateMethod();
	crJXJBackCityButtonCoolTimerUpdateMethod(const crJXJBackCityButtonCoolTimerUpdateMethod &handle);
	MethodClass(JXJ, JXJBackCityButtonCoolTimerUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_backCityButton; //回城按钮
	std::string m_buttonSequence; //回城按钮冷却动画
	std::string m_timeText; //倒计时
};
///////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerTransportationMethod
//回城技能使用
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerTransportationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerTransportationMethod();
	crJXJPlayerTransportationMethod(const crJXJPlayerTransportationMethod& handle);
	MethodClass(JXJ, JXJPlayerTransportation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_abstractId;
	std::string m_canvas;
};
/////////////////////////////////////////
//
//crJXJUIBattleTransportationTipMethod
//战场回城tips
/////////////////////////////////////////
class crJXJUIBattleTransportationTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBattleTransportationTipMethod();
	crJXJUIBattleTransportationTipMethod(const crJXJUIBattleTransportationTipMethod& handle);
	MethodClass(JXJ,JXJUIBattleTransportationTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_strBt;
	std::string m_tips;
	std::string m_tipstext;
	int m_str;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUICreateGroupCostTipsMethod
//元宝创建军团消费 二次提示Client
//////////////////////////////////////////////////////////////////////////
class crJXJUICreateGroupCostTipsMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUICreateGroupCostTipsMethod();
	crJXJUICreateGroupCostTipsMethod(const crJXJUICreateGroupCostTipsMethod& handle);
	MethodClass(JXJ,JXJUICreateGroupCostTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUISaveEquipRefineTipsMethod
//工坊保存装备精炼属性 二次提示Client
//////////////////////////////////////////////////////////////////////////
class crJXJUISaveEquipRefineTipsMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUISaveEquipRefineTipsMethod();
	crJXJUISaveEquipRefineTipsMethod(const crJXJUISaveEquipRefineTipsMethod& handle);
	MethodClass(JXJ,JXJUISaveEquipRefineTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJKickMemberOutofGroupTipsMethod
//移除军团成员 二次提示Client
//////////////////////////////////////////////////////////////////////////
class crJXJKickMemberOutofGroupTipsMethod:public CRCore::crMethod
{//Client 
public:
	crJXJKickMemberOutofGroupTipsMethod();
	crJXJKickMemberOutofGroupTipsMethod(const crJXJKickMemberOutofGroupTipsMethod& handle);
	MethodClass(JXJ,JXJKickMemberOutofGroupTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJDelayHideStaticTextWidgetMethod
//控件延时隐藏
//////////////////////////////////////////////////////////////////////////
class crJXJDelayHideStaticTextWidgetMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJDelayHideStaticTextWidgetMethod();
	crJXJDelayHideStaticTextWidgetMethod(const crJXJDelayHideStaticTextWidgetMethod &handle);
	MethodClass(JXJ,JXJDelayHideStaticTextWidget)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_Widget;
};
///////////////////////////////////////////////
//
//crJXJConsortiaCopyBattleProgressCanvasUpdateMethod
//军团副本boss存活时间刷新
//////////////////////////////////////////////
class crJXJConsortiaCopyBattleProgressCanvasUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJConsortiaCopyBattleProgressCanvasUpdateMethod();
	crJXJConsortiaCopyBattleProgressCanvasUpdateMethod(const crJXJConsortiaCopyBattleProgressCanvasUpdateMethod &handle);
	MethodClass(JXJ, JXJConsortiaCopyBattleProgressCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_remainTimer;
	short m_limitTimer_1;//跳十波需满足的时间限制
	short m_limitTimer_2;//达到这个时间限制自动刷新下一波
	short m_limitBossCircles;//当前boss波数达到m_limitBossCircles不再跳波，
	short m_totalBossCircles;//boss波数合计
	std::string m_variableName;//传入波数变量名
	std::string m_variableName_2;//传入计时器变量名
	unsigned char m_addCircles;//满足条件时跳波数
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJDelayHideSwitchCanvasTipsMethod
//战场界面 Alt+Tab切换提示窗口 显示十秒后自动隐藏
//////////////////////////////////////////////////////////////////////////
class crJXJDelayHideSwitchCanvasTipsMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJDelayHideSwitchCanvasTipsMethod();
	crJXJDelayHideSwitchCanvasTipsMethod(const crJXJDelayHideSwitchCanvasTipsMethod &handle);
	MethodClass(JXJ,JXJDelayHideSwitchCanvasTips)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_Widget;
	unsigned short m_delayTimer;//指定窗口延迟关闭时间
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJAutoShowFestivalHuodongCanvasMethod
//玩家每日首次登录时检测是否有处于激活状态的节日活动，有就自动弹出活动窗口
//////////////////////////////////////////////////////////////////////////
class crJXJAutoShowFestivalHuodongCanvasMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJAutoShowFestivalHuodongCanvasMethod();
	crJXJAutoShowFestivalHuodongCanvasMethod(const crJXJAutoShowFestivalHuodongCanvasMethod &handle);
	MethodClass(JXJ,JXJAutoShowFestivalHuodongCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_Widget;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvCheckFestivalActivityCompleteMethod
//检查一组节日活动下的三个活动是否都完成
//////////////////////////////////////////////////////////////////////////
class crJXJRecvCheckFestivalActivityCompleteMethod :public CRCore::crMethod
{//GameServer
public:
	crJXJRecvCheckFestivalActivityCompleteMethod ();
	crJXJRecvCheckFestivalActivityCompleteMethod (const crJXJRecvCheckFestivalActivityCompleteMethod & handle);
	MethodClass(JXJ, JXJRecvCheckFestivalActivityComplete)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_canvas_sw;
};
}
#endif
