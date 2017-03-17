/**********************************************************************
*
*	文件:	   appMethod13.h
*
*	描述:
*
*	作者:	   宋健、吴桐
*
*
**********************************************************************/
#ifndef JXJ_APPMETHOD13_H
#define JXJ_APPMETHOD13_H
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
#include <JXJ/appMethod9.h>
#include <JXJ/appMethod16.h>
#include <JXJ/appMethod14.h>
#include <JXJ/appData3.h>

namespace JXJ{
class crJXJShouchongCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShouchongCanvasUpdateMethod ();
	crJXJShouchongCanvasUpdateMethod (const crJXJShouchongCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJShouchongCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[6];
	std::string m_chongzhi;
	std::string m_lingqu;
};

class crJXJFubenRushEnd0Method :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushEnd0Method();
	crJXJFubenRushEnd0Method(const crJXJFubenRushEnd0Method& handle);
	MethodClass(JXJ, JXJFubenRushEnd0)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_leftswitch;    
	std::string m_rightswitch;   
	std::string m_rushradio;    
	std::string m_rushcountedit; 
	std::string m_rushjunlinginput; 
	std::string m_list;
};

class crJXJCheckJoinWarTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckJoinWarTaskMethod();
	crJXJCheckJoinWarTaskMethod(const crJXJCheckJoinWarTaskMethod& handle);
	MethodClass(JXJ, JXJCheckJoinWarTask)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJCheckJoinWarTask1Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckJoinWarTask1Method();
	crJXJCheckJoinWarTask1Method(const crJXJCheckJoinWarTask1Method& handle);
	MethodClass(JXJ, JXJCheckJoinWarTask1)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJIsChengZhanMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJIsChengZhanMethod();
	crJXJIsChengZhanMethod(const crJXJIsChengZhanMethod& handle);
	MethodClass(JXJ, JXJIsChengZhan)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crJXJMonitorPingPacketParseMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJMonitorPingPacketParseMethod();
	crJXJMonitorPingPacketParseMethod(const crJXJMonitorPingPacketParseMethod& handle);
	MethodClass(JXJ, JXJMonitorPingPacketParse)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
};

class crJXJCheckTechIsCompleteMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTechIsCompleteMethod();
	crJXJCheckTechIsCompleteMethod(const crJXJCheckTechIsCompleteMethod& handle);
	MethodClass(JXJ, JXJCheckTechIsComplete)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_techtype;
	char m_trueExecCount;//-1表示无穷
};

class crJXJIsSelectMyRolesMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsSelectMyRolesMethod();
	crJXJIsSelectMyRolesMethod(const crJXJIsSelectMyRolesMethod& handle);
	MethodClass(JXJ, JXJIsSelectMyRoles)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};

class crJXJBuyYuekaMethod :public CRCore::crMethod
{//GameClient Method 购买月卡
public:
	crJXJBuyYuekaMethod();
	crJXJBuyYuekaMethod(const crJXJBuyYuekaMethod& handle);
	MethodClass(JXJ, JXJBuyYueka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned char m_type;
};

class crJXJRecvBuyYuekaMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvBuyYuekaMethod ();
	crJXJRecvBuyYuekaMethod (const crJXJRecvBuyYuekaMethod & handle);
	MethodClass(JXJ, JXJRecvBuyYueka)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJYueKaCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJYueKaCanvasUpdateMethod ();
	crJXJYueKaCanvasUpdateMethod (const crJXJYueKaCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJYueKaCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_Goumai;
	std::string m_Lingqu;
	std::string m_Xufei;
	std::string m_sw2;
	std::string m_sw4;
	std::string m_sw9;
	std::string m_icon;
	std::string m_imagename;
};

class crJXJYueKaRewardsMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJYueKaRewardsMethod ();
	crJXJYueKaRewardsMethod (const crJXJYueKaRewardsMethod & handle);
	MethodClass(JXJ, JXJYueKaRewards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);

};

class crJXJRecvYueKaRewardsMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvYueKaRewardsMethod ();
	crJXJRecvYueKaRewardsMethod (const crJXJRecvYueKaRewardsMethod & handle);
	MethodClass(JXJ, JXJRecvYueKaRewards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJZiXuanLiBaoCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJZiXuanLiBaoCanvasUpdateMethod ();
	crJXJZiXuanLiBaoCanvasUpdateMethod (const crJXJZiXuanLiBaoCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJZiXuanLiBaoCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_chongzhi;
	std::string m_lingqu;
	std::string m_sw13;
	std::string m_icon[16];
	std::string m_nullImage;
	std::string m_input[16];
	std::string m_donghua[16];
	std::string m_radioGroup;
	std::string m_LingQuCiShu;
	std::string m_yichongzhi;
	std::string m_remainingtimes;
	std::string m_remainingtime;
	std::string m_yuanbaonum;
	std::string m_swnum[4];
	int m_lasthuodongID;
};

class crJXJClientLibaoHuodongGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientLibaoHuodongGetAwardsMethod ();
	crJXJClientLibaoHuodongGetAwardsMethod (const crJXJClientLibaoHuodongGetAwardsMethod & handle);
	MethodClass(JXJ, JXJClientLibaoHuodongGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
	std::string m_radioGroup;
};

class crJXJRecvLibaoHuodongGetAwardMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvLibaoHuodongGetAwardMethod ();
	crJXJRecvLibaoHuodongGetAwardMethod (const crJXJRecvLibaoHuodongGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecvLibaoHuodongGetAward)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};


class crJXJNewGetAwardsByActiveCodeMethod: public CRCore::crMethod
{
public:
	crJXJNewGetAwardsByActiveCodeMethod();
	crJXJNewGetAwardsByActiveCodeMethod(const crJXJNewGetAwardsByActiveCodeMethod & handle);
	MethodClass(JXJ,JXJNewGetAwardsByActiveCode)

		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRNetApp::crPlayerGameData *m_this;
	ActiveCodeData *m_activeCodeData;
	char *m_retVal;
};

class crJXJClientCodeHuodongGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientCodeHuodongGetAwardsMethod ();
	crJXJClientCodeHuodongGetAwardsMethod (const crJXJClientCodeHuodongGetAwardsMethod & handle);
	MethodClass(JXJ, JXJClientCodeHuodongGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
};

class crJXJRecvCodeHuodongGetAwardMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvCodeHuodongGetAwardMethod ();
	crJXJRecvCodeHuodongGetAwardMethod (const crJXJRecvCodeHuodongGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecvCodeHuodongGetAward)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJClientWeeklySpecialGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientWeeklySpecialGetAwardsMethod ();
	crJXJClientWeeklySpecialGetAwardsMethod (const crJXJClientWeeklySpecialGetAwardsMethod & handle);
	MethodClass(JXJ, JXJClientWeeklySpecialGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	unsigned char m_huodongtype;	//1 每周特惠单笔  2  每周特惠累计
};

class crJXJRecvWeeklySpecialGetAwardMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvWeeklySpecialGetAwardMethod ();
	crJXJRecvWeeklySpecialGetAwardMethod (const crJXJRecvWeeklySpecialGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecvWeeklySpecialGetAward)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJLeijichongzhiCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJLeijichongzhiCanvasUpdateMethod ();
	crJXJLeijichongzhiCanvasUpdateMethod (const crJXJLeijichongzhiCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJLeijichongzhiCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[15];
	std::string m_input[15];
	std::string m_donghua[5];
	std::string m_inputd[5];
	std::string m_input6;
	std::string m_lingqu;
	std::string m_radio;
	std::string m_swshuzi;
	std::string m_sw11;
	std::string m_chongzhi;
	unsigned char m_isxiaofei;
	std::string m_sw12;
	std::string m_sw13;
	std::string m_swy[7];
	std::string m_donghuay[7];
	int m_curselectid;
	std::string m_hdCanvas;
	
};

class crJXJLeijiChongzhiGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeijiChongzhiGetAwardsMethod ();
	crJXJLeijiChongzhiGetAwardsMethod (const crJXJLeijiChongzhiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJLeijiChongzhiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
	std::string m_radioGroup;
	unsigned char m_isxiaofei;
	std::string m_hdCanvas;
};

class crJXJRecvLeijiChongzhiGetAwardsMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvLeijiChongzhiGetAwardsMethod ();
	crJXJRecvLeijiChongzhiGetAwardsMethod (const crJXJRecvLeijiChongzhiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJRecvLeijiChongzhiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJDanbichongzhiCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJDanbichongzhiCanvasUpdateMethod ();
	crJXJDanbichongzhiCanvasUpdateMethod (const crJXJDanbichongzhiCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJDanbichongzhiCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[16];
	std::string m_input1[4];
	std::string m_input2[4];
	std::string m_remainingtime;
	std::string m_lingqubtn[4];	
	std::string m_donghua[16];
};

class crJXJDanbiChongzhiGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDanbiChongzhiGetAwardsMethod ();
	crJXJDanbiChongzhiGetAwardsMethod (const crJXJDanbiChongzhiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJDanbiChongzhiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
	std::string m_index;
};

class crJXJRecvDanbiChongzhiGetAwardsMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvDanbiChongzhiGetAwardsMethod ();
	crJXJRecvDanbiChongzhiGetAwardsMethod (const crJXJRecvDanbiChongzhiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJRecvDanbiChongzhiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJZhouKaCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJZhouKaCanvasUpdateMethod ();
	crJXJZhouKaCanvasUpdateMethod (const crJXJZhouKaCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJZhouKaCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[7];
	std::string m_input[14];;	
	std::string m_donghua[7];
	std::string m_chongzhi;
	std::string m_ingotnum;
	std::string m_remainingtime;
	std::string m_sw[7];
	std::string m_lingqubtn;
	std::string m_sw18;
};

class crJXJRecvUpdateZhouKaStatusMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvUpdateZhouKaStatusMethod ();
	crJXJRecvUpdateZhouKaStatusMethod (const crJXJRecvUpdateZhouKaStatusMethod & handle);
	MethodClass(JXJ, JXJRecvUpdateZhouKaStatus)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJZhouKaGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJZhouKaGetAwardsMethod ();
	crJXJZhouKaGetAwardsMethod (const crJXJZhouKaGetAwardsMethod & handle);
	MethodClass(JXJ, JXJZhouKaGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
};

class crJXJRecvZhouKaGetAwardsMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvZhouKaGetAwardsMethod ();
	crJXJRecvZhouKaGetAwardsMethod (const crJXJRecvZhouKaGetAwardsMethod & handle);
	MethodClass(JXJ, JXJRecvZhouKaGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJSetBillboardPageMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJSetBillboardPageMethod ();
	crJXJSetBillboardPageMethod (const crJXJSetBillboardPageMethod & handle);
	MethodClass(JXJ, JXJSetBillboardPage)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_billboardraido;
	int m_num;
};

class crJXJCheckMyApplyGroupIDSetMethod :public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckMyApplyGroupIDSetMethod();
	crJXJCheckMyApplyGroupIDSetMethod(const crJXJCheckMyApplyGroupIDSetMethod& handle);
	MethodClass(JXJ, JXJCheckMyApplyGroupIDSet)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned char m_type;
};

class crJXJRecvCheckMyApplyGroupIDSetMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvCheckMyApplyGroupIDSetMethod ();
	crJXJRecvCheckMyApplyGroupIDSetMethod (const crJXJRecvCheckMyApplyGroupIDSetMethod & handle);
	MethodClass(JXJ, JXJRecvCheckMyApplyGroupIDSet)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJFubenResurrectionCountMethod : public CRCore::crMethod
{//SceneServer Method 玩家征战天下复活次数
public:
	crJXJFubenResurrectionCountMethod();
	crJXJFubenResurrectionCountMethod(const crJXJFubenResurrectionCountMethod& handle);
	MethodClass(JXJ, JXJFubenResurrectionCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom  *m_this;
};

class crJXJClickLinkIntoFuBenMethod :public CRCore::crMethod
{
	//client
public:
	crJXJClickLinkIntoFuBenMethod ();
	crJXJClickLinkIntoFuBenMethod (const crJXJClickLinkIntoFuBenMethod & handle);
	MethodClass(JXJ, JXJClickLinkIntoFuBen)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJJuntuanCreateRoomReturnMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJJuntuanCreateRoomReturnMethod();
	crJXJJuntuanCreateRoomReturnMethod(const crJXJJuntuanCreateRoomReturnMethod& handle);
	MethodClass(JXJ, JXJJuntuanCreateRoomReturn)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_netType;
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJClickIntoJunTuanMultiFuBenMethod :public CRCore::crMethod
{	//client
public:
	crJXJClickIntoJunTuanMultiFuBenMethod ();
	crJXJClickIntoJunTuanMultiFuBenMethod (const crJXJClickIntoJunTuanMultiFuBenMethod & handle);
	MethodClass(JXJ, JXJClickIntoJunTuanMultiFuBen)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_mygroupcanvas;
	
};

class crJXJClientStartJuntuanFubenNotificationMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJClientStartJuntuanFubenNotificationMethod();
	crJXJClientStartJuntuanFubenNotificationMethod(const crJXJClientStartJuntuanFubenNotificationMethod& handle);
	MethodClass(JXJ, JXJClientStartJuntuanFubenNotification)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
	std::string m_mygroupcanvas;
};

class crJXJAppointedSecondGroupLeaderMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAppointedSecondGroupLeaderMethod();
	crJXJAppointedSecondGroupLeaderMethod(const crJXJAppointedSecondGroupLeaderMethod& handle);
	MethodClass(JXJ, JXJAppointedSecondGroupLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;     
	std::string m_groupList;
	unsigned char m_type;		//1.任命	 0.解除
};

class crJXJRecvAppointedSecondGroupLeaderMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAppointedSecondGroupLeaderMethod();
	crJXJRecvAppointedSecondGroupLeaderMethod(const crJXJRecvAppointedSecondGroupLeaderMethod& handle);
	MethodClass(JXJ, JXJRecvAppointedSecondGroupLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJGroupLevelUpMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupLevelUpMethod();
	crJXJGroupLevelUpMethod(const crJXJGroupLevelUpMethod& handle);
	MethodClass(JXJ, JXJGroupLevelUp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;     
};

class crJXJRecvGroupLevelUpMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGroupLevelUpMethod();
	crJXJRecvGroupLevelUpMethod(const crJXJRecvGroupLevelUpMethod& handle);
	MethodClass(JXJ, JXJRecvGroupLevelUp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJGroupEnfeoffChengChiMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupEnfeoffChengChiMethod ();
	crJXJGroupEnfeoffChengChiMethod (const crJXJGroupEnfeoffChengChiMethod & handle);
	MethodClass(JXJ, JXJGroupEnfeoffChengChi)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_table;
	unsigned char m_type;
};

class crJXJRecvGroupEnfeoffChengChiMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvGroupEnfeoffChengChiMethod();
	crJXJRecvGroupEnfeoffChengChiMethod(const crJXJRecvGroupEnfeoffChengChiMethod& handle);
	MethodClass(JXJ, JXJRecvGroupEnfeoffChengChi)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJTaoFaCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJTaoFaCanvasUpdateMethod ();
	crJXJTaoFaCanvasUpdateMethod (const crJXJTaoFaCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJTaoFaCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_fubenname;
	std::string m_nandu[5];;	
	std::string m_radio1;
	std::string m_times;
	std::string m_tuijiandengdi;
	std::string m_radio2;
	std::string m_taofadian;
	std::string m_taofaBtn;
	std::string m_canvas;
	std::string m_jiangliicon[4];
	std::string m_quanxiansw;
	std::string m_jinruBtn;
};

class crJXJCunBingCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJCunBingCanvasUpdateMethod ();
	crJXJCunBingCanvasUpdateMethod (const crJXJCunBingCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJCunBingCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[3];
	std::string m_ownnum[3];;	
	std::string m_cunrunum[3];
	std::string m_numscroll[3];
	std::string m_scroll;
	std::string m_limitnum;
	std::string m_querenBtn;
	std::string m_canvas;
};

class crJXJCunBingSetCountScrolPosMethod :public CRCore::crMethod
{
public:
	crJXJCunBingSetCountScrolPosMethod();
	crJXJCunBingSetCountScrolPosMethod(const crJXJCunBingSetCountScrolPosMethod& handle);
	MethodClass(JXJ, JXJCunBingSetCountScrolPos);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crScrollBarWidgetNode *m_this;
	unsigned char m_type;
	std::string m_icon[3];
	std::string m_input[3];
	std::string m_canvas;
	std::string m_limitnum;
};

class crJXJCunBingSetCountinputTextMethod :public CRCore::crMethod
{
public:
	crJXJCunBingSetCountinputTextMethod();
	crJXJCunBingSetCountinputTextMethod(const crJXJCunBingSetCountinputTextMethod& handle);
	MethodClass(JXJ, JXJCunBingSetCountinputText);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crStaticTextWidgetNode *m_this;
	unsigned char m_type;
	std::string m_numscroll[3];
	std::string m_icon[3];
	std::string m_canvas;
	std::string m_limitnum;
};

class crJXJConfirmdonateTroopsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJConfirmdonateTroopsMethod ();
	crJXJConfirmdonateTroopsMethod (const crJXJConfirmdonateTroopsMethod & handle);
	MethodClass(JXJ, JXJConfirmdonateTroops)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_limitnum;
};

class crJXJRecvConfirmdonateTroopsMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvConfirmdonateTroopsMethod();
	crJXJRecvConfirmdonateTroopsMethod(const crJXJRecvConfirmdonateTroopsMethod& handle);
	MethodClass(JXJ, JXJRecvConfirmdonateTroops)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJConfirmTakeTroopsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJConfirmTakeTroopsMethod ();
	crJXJConfirmTakeTroopsMethod (const crJXJConfirmTakeTroopsMethod & handle);
	MethodClass(JXJ, JXJConfirmTakeTroops)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_icon;
	std::string m_input;
};

class crJXJRecvConfirmTakeTroopsMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvConfirmTakeTroopsMethod();
	crJXJRecvConfirmTakeTroopsMethod(const crJXJRecvConfirmTakeTroopsMethod& handle);
	MethodClass(JXJ, JXJRecvConfirmTakeTroops)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJSetTakeTroopsIndexMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetTakeTroopsIndexMethod ();
	crJXJSetTakeTroopsIndexMethod (const crJXJSetTakeTroopsIndexMethod & handle);
	MethodClass(JXJ, JXJSetTakeTroopsIndex)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_idx;
};

class crJXJQuBingCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJQuBingCanvasUpdateMethod ();
	crJXJQuBingCanvasUpdateMethod (const crJXJQuBingCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJQuBingCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon;
	std::string m_quchunum;
	std::string m_totalgongxian;
	std::string m_numscroll;
	std::string m_limitnum;
	std::string m_querenBtn;
	std::string m_canvas;
	std::string m_curowngongxian;
};

class crJXJRenMingCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJRenMingCanvasUpdateMethod ();
	crJXJRenMingCanvasUpdateMethod (const crJXJRenMingCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJRenMingCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_renmingBtn[10];
	std::string m_jiechuBtn[10];	
	std::string m_renmingTab;
	std::string m_ListScroll;
	std::string m_canvas;
};

class crJXJEditWidgetSizeLimitMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJEditWidgetSizeLimitMethod ();
	crJXJEditWidgetSizeLimitMethod (const crJXJEditWidgetSizeLimitMethod & handle);
	MethodClass(JXJ, JXJEditWidgetSizeLimit)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRUI::crEditWidgetNode *m_this;
	unsigned short m_limit;
	
};

class crJXJCorps2Tips2UpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJCorps2Tips2UpdateMethod ();
	crJXJCorps2Tips2UpdateMethod (const crJXJCorps2Tips2UpdateMethod & handle);
	MethodClass(JXJ, JXJCorps2Tips2Update)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input1;
	std::string m_input2;
	int m_textid;
	
};

class crJXJISGroupFuBenOpenMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJISGroupFuBenOpenMethod();
	crJXJISGroupFuBenOpenMethod(const crJXJISGroupFuBenOpenMethod& handle);
	MethodClass(JXJ, JXJISGroupFuBenOpen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_radio1;
	std::string m_canvas;
};

class crJXJISUIButtonEnableMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJISUIButtonEnableMethod();
	crJXJISUIButtonEnableMethod(const crJXJISUIButtonEnableMethod& handle);
	MethodClass(JXJ, JXJISUIButtonEnable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvGroupContributionMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvGroupContributionMethod();
	crJXJRecvGroupContributionMethod(const crJXJRecvGroupContributionMethod& handle);
	MethodClass(JXJ, JXJRecvGroupContribution)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_groupcontribution;
};

class crJXJClientRecvGroupContributionMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvGroupContributionMethod();
	crJXJClientRecvGroupContributionMethod(const crJXJClientRecvGroupContributionMethod& handle);
	MethodClass(JXJ, JXJClientRecvGroupContribution)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJIsGroupFirstLeaderMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJIsGroupFirstLeaderMethod();
	crJXJIsGroupFirstLeaderMethod(const crJXJIsGroupFirstLeaderMethod& handle);
	MethodClass(JXJ, JXJIsGroupFirstLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_type;	// 1 是否是军团长或副团长 0 是否是军团长
};

class crJXJJiLuCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJJiLuCanvasUpdateMethod ();
	crJXJJiLuCanvasUpdateMethod (const crJXJJiLuCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJJiLuCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
	std::string m_scroll;
	std::string m_canvas;
};

class crJXJQueryGroupJiLuMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGroupJiLuMethod();
	crJXJQueryGroupJiLuMethod(const crJXJQueryGroupJiLuMethod& handle);
	MethodClass(JXJ, JXJQueryGroupJiLu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJRecvQueryGroupJiLuMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryGroupJiLuMethod();
	crJXJRecvQueryGroupJiLuMethod(const crJXJRecvQueryGroupJiLuMethod& handle);
	MethodClass(JXJ, JXJRecvQueryGroupJiLu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
	std::string m_canvas;
};

class crJXJFenFengCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJFenFengCanvasUpdateMethod ();
	crJXJFenFengCanvasUpdateMethod (const crJXJFenFengCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJFenFengCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gamelist;
	std::string m_input1;
	std::string m_querenBtn;
	std::string m_canvas;
};

class crJXJSetGroupFenFengChengChiIDMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetGroupFenFengChengChiIDMethod ();
	crJXJSetGroupFenFengChengChiIDMethod (const crJXJSetGroupFenFengChengChiIDMethod & handle);
	MethodClass(JXJ, JXJSetGroupFenFengChengChiID)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_idx;
	std::string m_canvas;
};

class crJXJRefreshGroupCantonSpecialtyMethod: public CRCore::crMethod
{
public:
	crJXJRefreshGroupCantonSpecialtyMethod();
	crJXJRefreshGroupCantonSpecialtyMethod(const crJXJRefreshGroupCantonSpecialtyMethod & handle);
	MethodClass(JXJ,JXJRefreshGroupCantonSpecialty)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRNetApp::crPlayerGameData *m_this;
	int *m_groupid;
	int *m_playerid;
};

class crJXJQueryCantonSpecialtyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCantonSpecialtyMethod();
	crJXJQueryCantonSpecialtyMethod(const crJXJQueryCantonSpecialtyMethod& handle);
	MethodClass(JXJ, JXJQueryCantonSpecialty)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	bool m_isrefresh;
};

class crJXJRecvQueryCantonSpecialtyMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvQueryCantonSpecialtyMethod();
	crJXJRecvQueryCantonSpecialtyMethod(const crJXJRecvQueryCantonSpecialtyMethod& handle);
	MethodClass(JXJ, JXJRecvQueryCantonSpecialty)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJRecvUpdateCantonSpecialtyMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvUpdateCantonSpecialtyMethod ();
	crJXJRecvUpdateCantonSpecialtyMethod (const crJXJRecvUpdateCantonSpecialtyMethod & handle);
	MethodClass(JXJ, JXJRecvUpdateCantonSpecialty)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJBuyCantonSpecialtyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyCantonSpecialtyMethod();
	crJXJBuyCantonSpecialtyMethod(const crJXJBuyCantonSpecialtyMethod& handle);
	MethodClass(JXJ, JXJBuyCantonSpecialty)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
};

class crJXJRecvBuyCantonSpecialtyMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBuyCantonSpecialtyMethod();
	crJXJRecvBuyCantonSpecialtyMethod(const crJXJRecvBuyCantonSpecialtyMethod& handle);
	MethodClass(JXJ, JXJRecvBuyCantonSpecialty)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJChengchiziyuanCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJChengchiziyuanCanvasUpdateMethod ();
	crJXJChengchiziyuanCanvasUpdateMethod (const crJXJChengchiziyuanCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJChengchiziyuanCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_position;
	std::string m_output;
	std::string m_specialty[4];
	std::string m_baguanBtn;
	std::string m_canvas;
};

class crJXJUIGroupCantonSpecialtyTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupCantonSpecialtyTipsMethod();
	crJXJUIGroupCantonSpecialtyTipsMethod(const crJXJUIGroupCantonSpecialtyTipsMethod& handle);
	MethodClass(JXJ, JXJUIGroupCantonSpecialtyTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info2;
	std::string m_itemicon;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_imageindex;
};

class crJXJUpdateExploitandTroopnumInPeopleMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateExploitandTroopnumInPeopleMapMethod();
	crJXJUpdateExploitandTroopnumInPeopleMapMethod(const crJXJUpdateExploitandTroopnumInPeopleMapMethod& handle);
	MethodClass(JXJ, JXJUpdateExploitandTroopnumInPeopleMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crJXJRecvpdateExploitandTroopnumInPeopleMapMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvpdateExploitandTroopnumInPeopleMapMethod();
	crJXJRecvpdateExploitandTroopnumInPeopleMapMethod(const crJXJRecvpdateExploitandTroopnumInPeopleMapMethod& handle);
	MethodClass(JXJ, JXJRecvpdateExploitandTroopnumInPeopleMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJRecvConsortiaExploitRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvConsortiaExploitRankingMethod();
	crJXJRecvConsortiaExploitRankingMethod(const crJXJRecvConsortiaExploitRankingMethod& handle);
	MethodClass(JXJ, JXJRecvConsortiaExploitRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};

class crJXJRecvConsortiaTroopsRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvConsortiaTroopsRankingMethod();
	crJXJRecvConsortiaTroopsRankingMethod(const crJXJRecvConsortiaTroopsRankingMethod& handle);
	MethodClass(JXJ, JXJRecvConsortiaTroopsRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};

class crJXJCiQuGuanZhiMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCiQuGuanZhiMethod();
	crJXJCiQuGuanZhiMethod(const crJXJCiQuGuanZhiMethod& handle);
	MethodClass(JXJ, JXJCiQuGuanZhi)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJRecvCiQuGuanZhiMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvCiQuGuanZhiMethod();
	crJXJRecvCiQuGuanZhiMethod(const crJXJRecvCiQuGuanZhiMethod& handle);
	MethodClass(JXJ, JXJRecvCiQuGuanZhi)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJGroupDismissNotificationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupDismissNotificationMethod();
	crJXJGroupDismissNotificationMethod(const crJXJGroupDismissNotificationMethod& handle);
	MethodClass(JXJ, JXJGroupDismissNotification)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJRecvGroupDismissNotificationMethod :public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvGroupDismissNotificationMethod();
	crJXJRecvGroupDismissNotificationMethod(const crJXJRecvGroupDismissNotificationMethod& handle);
	MethodClass(JXJ, JXJRecvGroupDismissNotification)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJShowChengChiRenmingTipsMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShowChengChiRenmingTipsMethod ();
	crJXJShowChengChiRenmingTipsMethod (const crJXJShowChengChiRenmingTipsMethod & handle);
	MethodClass(JXJ, JXJShowChengChiRenmingTips)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_input1;
	std::string m_input2;
};

class crJXJ7DayHuodongCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJ7DayHuodongCanvasUpdateMethod ();
	crJXJ7DayHuodongCanvasUpdateMethod (const crJXJ7DayHuodongCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJ7DayHuodongCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_timeInput;
	std::string m_contentInput;
	std::string m_title[7];
	std::string m_imageBox[5];
	std::string m_getBtn;
	std::string m_nullImage;
	std::string m_num[5];
	std::string m_title_sw[7];
	std::string m_hadGottenBtn;
	std::string m_scroll;
	std::string m_code_sw;
	std::string m_codeText;
	std::string m_huodongtitle;
	std::string m_huodongprocess;
	int m_3starstimes;	//征战天下3星副本通关次数
	std::string m_festival_Icon[3];
	std::string m_festival_Button[3];
	std::string m_festival_Introduction;//活动介绍
	std::string m_festival_Deadline;	//活动截止日期
	std::string m_festival_buttonSwitch[3];//领取按钮开关
	std::string m_chengchiNum;//强国之路，国家占领城池数量
	std::string m_qiangguoIcon[4];
	std::string m_qiangguoNum[4];
	std::string m_qiangguoStep[4];
	std::string m_qiangguoGet[4];
	std::string m_telephoneIcon;
	std::string m_telephoneGet;
};

class crJXJUpdate7DayHuoDongMethod :public CRCore::crMethod
{// GameServer	 
public:
	crJXJUpdate7DayHuoDongMethod ();
	crJXJUpdate7DayHuoDongMethod (const crJXJUpdate7DayHuoDongMethod & handle);
	MethodClass(JXJ, JXJUpdate7DayHuoDong)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_type;		//活动类型 0,征战天下 1.名将之路 2.攻城掠地 3.血战沙场 4.视死如归
	int m_chapterid;
};

class crJXJServerUseKaoShangFuMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseKaoShangFuMethod();
	crJXJServerUseKaoShangFuMethod(const crJXJServerUseKaoShangFuMethod& handle);
	MethodClass(JXJ, JXJServerUseKaoShangFu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
};

class crJXJRecvKaoShangFuUsedTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvKaoShangFuUsedTimeMethod();
	crJXJRecvKaoShangFuUsedTimeMethod(const crJXJRecvKaoShangFuUsedTimeMethod& handle);
	MethodClass(JXJ, JXJRecvKaoShangFuUsedTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tipcontent;
};

class crJXJServerUseQieYeFuMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseQieYeFuMethod();
	crJXJServerUseQieYeFuMethod(const crJXJServerUseQieYeFuMethod& handle);
	MethodClass(JXJ, JXJServerUseQieYeFu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
};

class crJXJServerUseXuLieFuMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseXuLieFuMethod();
	crJXJServerUseXuLieFuMethod(const crJXJServerUseXuLieFuMethod& handle);
	MethodClass(JXJ, JXJServerUseXuLieFu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
};

class crJXJRefreshZhengZhanTianHuoDongStepsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRefreshZhengZhanTianHuoDongStepsMethod();
	crJXJRefreshZhengZhanTianHuoDongStepsMethod(const crJXJRefreshZhengZhanTianHuoDongStepsMethod& handle);
	MethodClass(JXJ, JXJRefreshZhengZhanTianHuoDongSteps)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJRecvZhengZhanTianHuoDongStepsMethod :public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvZhengZhanTianHuoDongStepsMethod();
	crJXJRecvZhengZhanTianHuoDongStepsMethod(const crJXJRecvZhengZhanTianHuoDongStepsMethod& handle);
	MethodClass(JXJ, JXJRecvZhengZhanTianHuoDongSteps)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJUISpecialtyRefreshMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUISpecialtyRefreshMethod();
	crJXJUISpecialtyRefreshMethod(const crJXJUISpecialtyRefreshMethod& handle);
	MethodClass(JXJ,JXJUISpecialtyRefresh)
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

class crJXJBaoXiangProgressFrameUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBaoXiangProgressFrameUpdateMethod();
	crJXJBaoXiangProgressFrameUpdateMethod(const crJXJBaoXiangProgressFrameUpdateMethod& handle);
	MethodClass(JXJ, JXJBaoXiangProgressFrameUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float *m_dt;
	std::string m_progress;
	float m_speed;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_curItem;
};

class crJXJTouchSceneItemProgressMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTouchSceneItemProgressMethod();
	crJXJTouchSceneItemProgressMethod(const crJXJTouchSceneItemProgressMethod& handle);
	MethodClass(JXJ, JXJTouchSceneItemProgress)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	float m_interval;
};

class crJXJUpdateNewHuodongMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateNewHuodongMapMethod();
	crJXJUpdateNewHuodongMapMethod(const crJXJUpdateNewHuodongMapMethod& handle);
	MethodClass(JXJ, JXJUpdateNewHuodongMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJRecvNewHuodongMapMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvNewHuodongMapMethod();
	crJXJRecvNewHuodongMapMethod(const crJXJRecvNewHuodongMapMethod& handle);
	MethodClass(JXJ, JXJRecvNewHuodongMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
	std::string m_canvas;
};

class crJXJUIShuoMingTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShuoMingTipInfoMethod();
	crJXJUIShuoMingTipInfoMethod(const crJXJUIShuoMingTipInfoMethod& handle);
	MethodClass(JXJ, JXJUIShuoMingTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_header;
	std::string m_trunk;
	std::string m_title;
	std::string m_text;
	std::string m_textbk;
	std::string m_textdi;
	int m_titleid;
	int m_textid;
};

class crJXJMeiRiHaoLiCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJMeiRiHaoLiCanvasUpdateMethod ();
	crJXJMeiRiHaoLiCanvasUpdateMethod (const crJXJMeiRiHaoLiCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJMeiRiHaoLiCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_Text;
	std::string m_icon[4];
	std::string m_LingquBtn;
	std::string m_sw;
	std::string m_input[4];
};

class crJXJMeiRiHaoLiGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMeiRiHaoLiGetAwardsMethod ();
	crJXJMeiRiHaoLiGetAwardsMethod (const crJXJMeiRiHaoLiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJMeiRiHaoLiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
};

class crJXJRecvMeiRiHaoLiGetAwardsMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvMeiRiHaoLiGetAwardsMethod ();
	crJXJRecvMeiRiHaoLiGetAwardsMethod (const crJXJRecvMeiRiHaoLiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJRecvMeiRiHaoLiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJQueryCanFenFengGroupMemberListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCanFenFengGroupMemberListMethod ();
	crJXJQueryCanFenFengGroupMemberListMethod (const crJXJQueryCanFenFengGroupMemberListMethod & handle);
	MethodClass(JXJ, JXJQueryCanFenFengGroupMemberList)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
};

class crJXJRecvCanFenFengGroupMemberListMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCanFenFengGroupMemberListMethod();
	crJXJRecvCanFenFengGroupMemberListMethod(const crJXJRecvCanFenFengGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJRecvCanFenFengGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_renmingcanvas;
};

class crJXJWorldFubenSynchronouscoordMethod : public CRCore::crMethod
{//SceneServer Method 世界战场同步友军
public:
	crJXJWorldFubenSynchronouscoordMethod();
	crJXJWorldFubenSynchronouscoordMethod(const crJXJWorldFubenSynchronouscoordMethod& handle);
	MethodClass(JXJ, JXJWorldFubenSynchronouscoord)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom  *m_this;
};

class crJXJRecvWorldFubenSynchronouscoordMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvWorldFubenSynchronouscoordMethod();
	crJXJRecvWorldFubenSynchronouscoordMethod(const crJXJRecvWorldFubenSynchronouscoordMethod& handle);
	MethodClass(JXJ, JXJRecvWorldFubenSynchronouscoord)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJNewHuiKuiCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewHuiKuiCanvasInitMethod ();
	crJXJNewHuiKuiCanvasInitMethod (const crJXJNewHuiKuiCanvasInitMethod & handle);
	MethodClass(JXJ, JXJNewHuiKuiCanvasInit)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJNewHuiKuiCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJNewHuiKuiCanvasUpdateMethod ();
	crJXJNewHuiKuiCanvasUpdateMethod (const crJXJNewHuiKuiCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJNewHuiKuiCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
	std::string getremainingtime(CRCore::crVector3i endtimeVec);
	void setTitleText(int tiaojian1,int tiaojian2, int title, int neirong,int cishu, const CREncapsulation::crTableIO::StrVec &record, CRUI::crHypertextWidgetNode * hyp);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_titleBtn[10];
	std::string m_title[10];
	std::string m_icon[18];
	std::string m_input[18];
	std::string m_title1[3];
	std::string m_scroll;
	std::string m_lingquBtn[3];
	std::string m_nullImage;
	std::string m_sw[3];
	std::string m_goumaiBtn[3];
	std::string m_Dintu_sw;
	std::string m_shengyushijianText;
	std::string m_yichongzhi;
	std::string m_node[3];
	std::string m_circles[3];
	std::string m_lingqu_sw[3];
	std::string m_iconplane[18];
	std::string m_guojia_sw;
	std::string m_name_sw[10];
	std::string m_numinput[10];
	std::string m_sequence[3];
};

class crJXJSetCurrentHuiKuiHdIdMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetCurrentHuiKuiHdIdMethod ();
	crJXJSetCurrentHuiKuiHdIdMethod (const crJXJSetCurrentHuiKuiHdIdMethod & handle);
	MethodClass(JXJ, JXJSetCurrentHuiKuiHdId)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	unsigned short m_idx ;
};

class crJXJClientNewHuiKuiGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientNewHuiKuiGetAwardsMethod ();
	crJXJClientNewHuiKuiGetAwardsMethod (const crJXJClientNewHuiKuiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJClientNewHuiKuiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	unsigned char m_isgoumai;
};

class crJXJRecvClientNewHuiKuiGetAwardsMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvClientNewHuiKuiGetAwardsMethod ();
	crJXJRecvClientNewHuiKuiGetAwardsMethod (const crJXJRecvClientNewHuiKuiGetAwardsMethod & handle);
	MethodClass(JXJ, JXJRecvClientNewHuiKuiGetAwards)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crCheckHuiKuiHuoDongCompleteMethod : public CRCore::crMethod
{//Client
public:
	crCheckHuiKuiHuoDongCompleteMethod();
	crCheckHuiKuiHuoDongCompleteMethod(const crCheckHuiKuiHuoDongCompleteMethod& handle);
	MethodClass(CRNetApp, CheckHuiKuiHuoDongComplete)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_huodongcanvas;
	std::string m_countbackboard;
	std::string m_countinput;
	std::string m_huikuibtn;
	std::string m_donghua;
};

class crJXJServerOpenGroupFuBenMethod: public CRCore::crMethod
{
public:
	crJXJServerOpenGroupFuBenMethod();
	crJXJServerOpenGroupFuBenMethod(const crJXJServerOpenGroupFuBenMethod & handle);
	MethodClass(JXJ,JXJServerOpenGroupFuBen)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRNetApp::crServerBrainHandle *m_this;
	int *m_groupid;
	int *m_playerid;
	time_t m_lasttimer;

};

class crJXJUIXianShiTeMaiTipMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIXianShiTeMaiTipMethod();
	crJXJUIXianShiTeMaiTipMethod(const crJXJUIXianShiTeMaiTipMethod& handle);
	MethodClass(JXJ,JXJUIXianShiTeMaiTip)
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

class crJXJJunTuanFuBenXinHideMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJJunTuanFuBenXinHideMethod ();
	crJXJJunTuanFuBenXinHideMethod (const crJXJJunTuanFuBenXinHideMethod & handle);
	MethodClass(JXJ, JXJJunTuanFuBenXinHide)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_xinxicanvas;
	std::string m_canvas;
	unsigned char m_ishide;
};

class crCheck7DayHuoDongCompleteMethod : public CRCore::crMethod
{//Client
public:
	crCheck7DayHuoDongCompleteMethod();
	crCheck7DayHuoDongCompleteMethod(const crCheck7DayHuoDongCompleteMethod& handle);
	MethodClass(CRNetApp, Check7DayHuoDongComplete)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_huodongcanvas;
	std::string m_countbackboard;
	std::string m_countinput;
	std::string m_7DayhuodongBtn;
	std::string m_hddonghua;
};

}
#endif

