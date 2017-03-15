/**********************************************************************
*
*	文件:	   appBattleMethod3.h
*
*	描述:	   
*
*	作者:	   曹霖麟
*					
*
**********************************************************************/
#ifndef JXJ_APPBATTLEMETHOD3_H
#define JXJ_APPBATTLEMETHOD3_H
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
#include <crphysics/crVolumeNode.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <JXJ/appDefine.h>
namespace JXJ{

class crJXJQueryBattleBirthPointMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQueryBattleBirthPointMethod();
	crJXJQueryBattleBirthPointMethod(const crJXJQueryBattleBirthPointMethod& handle);
	MethodClass(JXJ, JXJQueryBattleBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJRecvQueryBirthPointMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryBirthPointMethod();
	crJXJRecvQueryBirthPointMethod(const crJXJRecvQueryBirthPointMethod& handle);
	MethodClass(JXJ, JXJRecvQueryBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas_1;
	std::string m_strCanvas_2;
	std::string m_strCanvas_3;
};
class crJXJQueryGameServerTimerMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGameServerTimerMethod();
	crJXJQueryGameServerTimerMethod(const crJXJQueryGameServerTimerMethod& handle);
	MethodClass(JXJ, JXJQueryGameServerTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

//////////////////////////////////////////////////////////////////////////
//crJXJShowItemLinkClickMethod
//////////////////////////////////////////////////////////////////////////
class crJXJInviteLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJInviteLinkClickMethod();
	crJXJInviteLinkClickMethod(const crJXJInviteLinkClickMethod& handle);
	MethodClass(JXJ, JXJInviteLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	unsigned char m_inviteType;
	int m_invitePlayerId;
	unsigned short m_inviteid;
};

//////////////////////////////////////////////////////////////////////////
//crJXJEnterBattleDequeClickMethod
//////////////////////////////////////////////////////////////////////////
class crJXJEnterBattleDequeClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJEnterBattleDequeClickMethod();
	crJXJEnterBattleDequeClickMethod(const crJXJEnterBattleDequeClickMethod& handle);
	MethodClass(JXJ, JXJEnterBattleDequeClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_strCityName;
	time_t m_time;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJChatReshowMethod
// On  : reshow 
// Desp: 重新显示chat 内容
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class crJXJPutInChatRecordDequeMethod:public CRCore::crMethod
{// client
public:
	crJXJPutInChatRecordDequeMethod();
	crJXJPutInChatRecordDequeMethod(const crJXJPutInChatRecordDequeMethod& handle);
	MethodClass(JXJ, JXJPutInChatRecordDeque)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void analysisChatText(std::string text);
protected:
	CRCore::crBrain * m_this;
	CRCore::ref_ptr<crJXJChatRecord> m_chatInfo;
	std::string m_chat_canvas;
	std::string m_chat_show;
	std::string m_chat_mode;
	std::string m_chat_cfg;
	std::string m_input;
	std::string m_scroll;
	std::string m_RoomChatCanvas;
	int m_textRow;
	typedef std::vector<std::pair<std::pair<int,int>,std::string>> ChatItemInfoVec;//itemid,equipmagic,name
	typedef std::vector<std::pair<std::pair<int,int>,std::string>> ChatAchieveInfoVec;//achieveid,step,name
	typedef std::vector<std::pair<unsigned char,std::string>> SystemChatInfoVec;//超链接ID，text
	typedef std::vector<std::pair<InviteInfo,std::string>> InviteChatInfoVec;//超链接ID，text
	typedef std::vector< std::pair<std::pair<std::string, int>, std::string> > BattleReportInfoVec;//playerid,reportid,text
	typedef std::vector<std::pair<CallForHelpInfo, std::string>> CallForHelpInfoVec;//battleroomid,fubenid
	typedef std::vector<std::pair<int,std::string>> PraiseInfoVec;//playerid,text
	ChatItemInfoVec m_strVec; //是否为item名，字符串--
	ChatAchieveInfoVec m_strAchieveVec;
	SystemChatInfoVec m_strNoticeVec;
	InviteChatInfoVec m_strInviteVec;
	BattleReportInfoVec m_strReportVec;
	CallForHelpInfoVec m_strCallForHelpVec;
	PraiseInfoVec m_strPraiseVec;
	std::map<int,std::pair<int,int>> m_linkIdMap;
	std::map<int,InviteInfo> m_inviteIdMap;
	std::map<int,std::string> m_noticeIdMap;//城战通告超链接
	std::map<int, std::pair<std::string, int>> m_reportIdMap;//战报通告连接
	std::map<int, int> m_praiseIdMap;//赞通告链接
	std::map<int, std::pair<int, int>> m_helpIdMap;//救援连接
	std::map<int, std::pair<int, int>> m_achieveIdMap;//成就展示连接
	static int m_slinkIndex;
	int m_characterSize;
	std::string m_strChatSetCanvas;
	//std::string m_playername;
};
//////////////////////////////////////////////////////////////////////////
//crJXJNewMailSwitchControlMethod
//////////////////////////////////////////////////////////////////////////
class crJXJNewMailSwitchControlMethod:public CRCore::crMethod
{// client
public:
	crJXJNewMailSwitchControlMethod();
	crJXJNewMailSwitchControlMethod(const crJXJNewMailSwitchControlMethod& handle);
	MethodClass(JXJ, JXJNewMailSwitchControl)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_mailplay;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCampAddArmyMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCampAddArmyMethod:public CRCore::crMethod
{// client
public:
	crJXJCampAddArmyMethod();
	crJXJCampAddArmyMethod(const crJXJCampAddArmyMethod& handle);
	MethodClass(JXJ, JXJCampAddArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

//////////////////////////////////////////////////////////////////////////
//crJXJCheckCampAddArmyEnableMethod
//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::pair<int,/*unsigned short*/int>> AddArmyInfoVec;
class crJXJCheckCampAddArmyEnableMethod:public CRCore::crMethod
{// client
public:
	crJXJCheckCampAddArmyEnableMethod();
	crJXJCheckCampAddArmyEnableMethod(const crJXJCheckCampAddArmyEnableMethod& handle);
	MethodClass(JXJ, JXJCheckCampAddArmyEnable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem * m_this;
	std::string m_strCanvas;
	float m_distance;;
};
//////////////////////////////////////////////////////////////////////////
//crJXJChangeCameraFollowStateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChangeCameraFollowStateMethod:public CRCore::crMethod
{// client
public:
	crJXJChangeCameraFollowStateMethod();
	crJXJChangeCameraFollowStateMethod(const crJXJChangeCameraFollowStateMethod& handle);
	MethodClass(JXJ, JXJChangeCameraFollowState)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crMatrixTransform *m_this;
	bool m_needChange;
	std::string m_strCanvas;
	std::string m_strCheckBox;
	std::string m_newCanvas;
	std::string m_newCanvas_sw;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetCameraFollowStateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetCameraFollowStateMethod:public CRCore::crMethod
{// client
public:
	crJXJGetCameraFollowStateMethod();
	crJXJGetCameraFollowStateMethod(const crJXJGetCameraFollowStateMethod& handle);
	MethodClass(JXJ, JXJGetCameraFollowState)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	std::string m_strCheckBox;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetCameraSpeedConfigMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetCameraSpeedConfigMethod:public CRCore::crMethod
{// client
public:
	crJXJGetCameraSpeedConfigMethod();
	crJXJGetCameraSpeedConfigMethod(const crJXJGetCameraSpeedConfigMethod& handle);
	MethodClass(JXJ, JXJGetCameraSpeedConfig)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJCampAddArmyCanvasOnshowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCampAddArmyCanvasOnshowMethod:public CRCore::crMethod
{// client
public:
	crJXJCampAddArmyCanvasOnshowMethod();
	crJXJCampAddArmyCanvasOnshowMethod(const crJXJCampAddArmyCanvasOnshowMethod& handle);
	MethodClass(JXJ, JXJCampAddArmyCanvasOnshow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strtext;
};
//////////////////////////////////////////////////////////////////////////
//crJXJPlayerCampAddArmyTimerUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerCampAddArmyTimerUpdateMethod:public CRCore::crMethod
{// client
public:
	crJXJPlayerCampAddArmyTimerUpdateMethod();
	crJXJPlayerCampAddArmyTimerUpdateMethod(const crJXJPlayerCampAddArmyTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJPlayerCampAddArmyTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_strCanvas;
	std::string m_strInput;
	float *m_dt;
};

class crJXJRecvTouchCampMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvTouchCampMethod();
	crJXJRecvTouchCampMethod(const crJXJRecvTouchCampMethod& handle);
	MethodClass(JXJ, JXJRecvTouchCamp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	float m_range;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleInfoCenterNoticeCreateMethod
//persuade
//////////////////////////////////////////////////////////////////////////
class crJXJBattleInfoCenterNoticeCreateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJBattleInfoCenterNoticeCreateMethod();
	crJXJBattleInfoCenterNoticeCreateMethod(const crJXJBattleInfoCenterNoticeCreateMethod& handle);
	MethodClass(JXJ, JXJBattleInfoCenterNoticeCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_battleid;
	bool m_iswin;
};
class crJXJUISelectPersuadeFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectPersuadeFubenMethod();
	crJXJUISelectPersuadeFubenMethod(const crJXJUISelectPersuadeFubenMethod& handle);
	MethodClass(JXJ, JXJUISelectPersuadeFuben)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strCanvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenPersuadeCanvasShowMethod
//
//////////////////////////////////////////////////////////////////////////
//class crJXJFubenPersuadeCanvasShowMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJFubenPersuadeCanvasShowMethod();
//	crJXJFubenPersuadeCanvasShowMethod(const crJXJFubenPersuadeCanvasShowMethod& handle);
//	MethodClass(JXJ, JXJFubenPersuadeCanvasShow)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_strDesText;
//	std::string m_strPersuadeSw_level;
//	std::string m_strPersuadeSw_jk;
//	std::string m_strPersuadeSw_Time;
//	std::string m_strJkImageBox;
//	std::string m_strBt;
//	std::string m_strImgaeBk;
//};
class crJXJShowJKPacketCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowJKPacketCanvasMethod();
	crJXJShowJKPacketCanvasMethod(const crJXJShowJKPacketCanvasMethod& handle);
	MethodClass(JXJ, JXJShowJKPacketCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strCanvas;
};
class crJXJClosePersuadeCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClosePersuadeCanvasMethod();
	crJXJClosePersuadeCanvasMethod(const crJXJClosePersuadeCanvasMethod& handle);
	MethodClass(JXJ, JXJClosePersuadeCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJDoFubenPersuadeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJDoFubenPersuadeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDoFubenPersuadeMethod();
	crJXJDoFubenPersuadeMethod(const crJXJDoFubenPersuadeMethod& handle);
	MethodClass(JXJ, JXJDoFubenPersuade)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFubenPersuadeMethod
//
//////////////////////////////////////////////////////////////////////////
//class crJXJRecvFubenPersuadeMethod :public CRCore::crMethod
//{//GameServer/Client Method
//public:
//	crJXJRecvFubenPersuadeMethod();
//	crJXJRecvFubenPersuadeMethod(const crJXJRecvFubenPersuadeMethod& handle);
//	MethodClass(JXJ, JXJRecvFubenPersuade)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_strCanvas;
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFubenPersuadeSucessMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvFubenPersuadeSucessMethod :public CRCore::crMethod
{//GameServer/Client Method
public:
	crJXJRecvFubenPersuadeSucessMethod();
	crJXJRecvFubenPersuadeSucessMethod(const crJXJRecvFubenPersuadeSucessMethod& handle);
	MethodClass(JXJ, JXJRecvFubenPersuadeSucess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strTaskCanvas;
	std::string m_strPlay;
	std::string m_strPersuadeRewardCanvas;
	std::string m_strText1;
	std::string m_strText2;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenPersuadeTimeUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFubenPersuadeTimeUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenPersuadeTimeUpdateMethod();
	crJXJFubenPersuadeTimeUpdateMethod(const crJXJFubenPersuadeTimeUpdateMethod& handle);
	MethodClass(JXJ, JXJFubenPersuadeTimeUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_persuadeSize;
	std::string m_strTaskCanvas;
	std::string m_strPlay;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenPersuadeSucessMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFubenPersuadeSucessMethod :public CRCore::crMethod
{//GameServer/Client Method
public:
	crJXJFubenPersuadeSucessMethod();
	crJXJFubenPersuadeSucessMethod(const crJXJFubenPersuadeSucessMethod& handle);
	MethodClass(JXJ, JXJFubenPersuadeSucess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenPersuadeUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJFubenPersuadeUIUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenPersuadeUIUpdateMethod();
	crJXJFubenPersuadeUIUpdateMethod(const crJXJFubenPersuadeUIUpdateMethod& handle);
	MethodClass(JXJ, JXJFubenPersuadeUIUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strTimeText;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckGameServerCycleStartMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCheckGameServerCycleStartMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckGameServerCycleStartMethod();
	crJXJCheckGameServerCycleStartMethod(const crJXJCheckGameServerCycleStartMethod& handle);
	MethodClass(JXJ, JXJCheckGameServerCycleStart)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	char m_cityLose;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvChangeAllCityShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvChangeAllCityShiliMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvChangeAllCityShiliMethod();
	crJXJRecvChangeAllCityShiliMethod(const crJXJRecvChangeAllCityShiliMethod& handle);
	MethodClass(JXJ, JXJRecvChangeAllCityShili)
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
//crJXJRecvServerCycleStartMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvServerCycleStartMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvServerCycleStartMethod();
	crJXJRecvServerCycleStartMethod(const crJXJRecvServerCycleStartMethod& handle);
	MethodClass(JXJ, JXJRecvServerCycleStart)
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
//crJXJGameServerCycleTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGameServerCycleTimerUpdateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerCycleTimerUpdateMethod();
	crJXJGameServerCycleTimerUpdateMethod(const crJXJGameServerCycleTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJGameServerCycleTimerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float *m_dt;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGameClientCycleTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGameClientCycleTimerUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameClientCycleTimerUpdateMethod();
	crJXJGameClientCycleTimerUpdateMethod(const crJXJGameClientCycleTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJGameClientCycleTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRNetApp::crPlayerGameData *m_this;
};


//////////////////////////////////////////////////////////////////////////
//
//crJXJTreasonCountryCanvasShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTreasonCountryCanvasShowMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJTreasonCountryCanvasShowMethod();
	crJXJTreasonCountryCanvasShowMethod(const crJXJTreasonCountryCanvasShowMethod& handle);
	MethodClass(JXJ, JXJTreasonCountryCanvasShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strSwitch;
	std::string m_strRadioGroup;
	std::string m_strRadioWidget_wei;
	std::string m_strRadioWidget_shu;
	std::string m_strRadioWidget_wu;
	std::string m_strSwitch_wei;
	std::string m_strSwitch_shu;
	std::string m_strSwitch_wu;
	std::string m_strText;
	std::string m_powersw_wei;
	std::string m_powersw_shu;
	std::string m_powersw_wu;
	//std::string m_strShiLiS[3];
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerTryToChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerTryToChangeShiliMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerTryToChangeShiliMethod();
	crJXJPlayerTryToChangeShiliMethod(const crJXJPlayerTryToChangeShiliMethod& handle);
	MethodClass(JXJ, JXJPlayerTryToChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_strRadioGroup;
	std::string m_strMsgCanvas;
	std::string m_strMsg;
	std::string m_citywei;
	std::string m_cityshu;
	std::string m_citywu;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerChangeShiliMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerChangeShiliMethod();
	crJXJPlayerChangeShiliMethod(const crJXJPlayerChangeShiliMethod& handle);
	MethodClass(JXJ, JXJPlayerChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_strRadioGroup;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvPlayerChangeShiliMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvPlayerChangeShiliMethod();
	crJXJRecvPlayerChangeShiliMethod(const crJXJRecvPlayerChangeShiliMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string  m_strJueSeCanvas;
	std::string	 m_strCountry;
	std::string  m_strtaskCanvas;
	std::string  m_strSwitch;
	std::string  m_strWidget;
	std::string m_strCountryCanvas;
	std::string  m_huodongcanvas;
	//std::string  m_chenzhanbtn;
	std::string m_panguoCanvas;
	std::string  m_citywei;
	std::string  m_cityshu;
	std::string m_citywu;
	int m_punishtime;
	int m_coolingtime;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChangeShiliMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJChangeShiliMethod();
	crJXJChangeShiliMethod(const crJXJChangeShiliMethod& handle);
	MethodClass(JXJ, JXJChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	unsigned char m_shiliid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckCountryLoseMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCheckCountryLoseMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckCountryLoseMethod();
	crJXJCheckCountryLoseMethod(const crJXJCheckCountryLoseMethod& handle);
	MethodClass(JXJ, JXJCheckCountryLose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	bool * m_bLoseCountry;
	unsigned char m_shiliid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientCheckCountryLoseMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientCheckCountryLoseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientCheckCountryLoseMethod();
	crJXJClientCheckCountryLoseMethod(const crJXJClientCheckCountryLoseMethod& handle);
	MethodClass(JXJ, JXJClientCheckCountryLose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	bool  * m_bLoseCountry;
	unsigned char m_shiliid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJIsMyCountryLoseMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJIsMyCountryLoseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsMyCountryLoseMethod();
	crJXJIsMyCountryLoseMethod(const crJXJIsMyCountryLoseMethod& handle);
	MethodClass(JXJ, JXJIsMyCountryLose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientShowChangeShiliCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientShowChangeShiliCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientShowChangeShiliCanvasMethod();
	crJXJClientShowChangeShiliCanvasMethod(const crJXJClientShowChangeShiliCanvasMethod& handle);
	MethodClass(JXJ, JXJClientShowChangeShiliCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crBrain *m_this;
	std::string m_strCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvGetLandRewardMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvGetLandRewardMethod :public CRCore::crMethod
{//GameServer/Client Method
public:
	crJXJRecvGetLandRewardMethod();
	crJXJRecvGetLandRewardMethod(const crJXJRecvGetLandRewardMethod& handle);
	MethodClass(JXJ, JXJRecvGetLandReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strSwitch;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvSendLandRewardToPlayerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvSendLandRewardToPlayerMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvSendLandRewardToPlayerMethod();
	crJXJRecvSendLandRewardToPlayerMethod(const crJXJRecvSendLandRewardToPlayerMethod& handle);
	MethodClass(JXJ, JXJRecvSendLandRewardToPlayer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//crJXJLandRewardDropClearMethod
//////////////////////////////////////////////////////////////////////////
class crJXJLandRewardDropClearMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJLandRewardDropClearMethod();
	crJXJLandRewardDropClearMethod(const crJXJLandRewardDropClearMethod& handle);
	MethodClass(JXJ, JXJLandRewardDropClear)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};

//////////////////////////////////////////////////////////////////////////
//crJXJLandRewardCanvasShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJLandRewardCanvasShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLandRewardCanvasShowMethod();
	crJXJLandRewardCanvasShowMethod(const crJXJLandRewardCanvasShowMethod& handle);
	MethodClass(JXJ, JXJLandRewardCanvasShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strTitle;
	std::string m_strSeq[3];
	std::string m_strImage_1;
	std::string m_strImage_2;
	std::string m_strMultiImage;
	std::string m_strStopSw[3];
	std::string m_strName[3];
	std::string m_strNumber[3];
	std::string m_strSwitch;
	std::string m_strButton;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClientRequestLandRewardMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClientRequestLandRewardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRequestLandRewardMethod();
	crJXJClientRequestLandRewardMethod(const crJXJClientRequestLandRewardMethod& handle);
	MethodClass(JXJ, JXJClientRequestLandReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strSeq[3];
	std::string m_strStopSw[3];
	std::string m_strName[3];
	std::string m_strNumber[3];
	std::string m_strSwitch;
};

//////////////////////////////////////////////////////////////////////////
//crJXJUILandRewardUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUILandRewardUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUILandRewardUpdateMethod();
	crJXJUILandRewardUpdateMethod(const crJXJUILandRewardUpdateMethod& handle);
	MethodClass(JXJ, JXJUILandRewardUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strSeq[3];
	std::string m_strImage_1;
	std::string m_strImage_2;
	std::string m_strMultiImage;
	std::string m_strStopSw[3];
	std::string m_strName[3];
	std::string m_strNumber[3];
	std::string m_strFoodImage;//粮草
	std::string m_strWoodImage;//木材
	std::string m_strIronImage;//铁矿
	std::string m_strHorseImage;//马匹
	std::string m_strCopperImage;//铜钱
	std::string m_strExpImage;//经验
	std::string m_strAchievementImage;//功勋
	std::string m_strGiftGoldImage;//礼金
	std::string m_strContributeImage;//城防值
	std::string m_strExploitImage;//战功
	std::string m_strBt;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGameClientLandRewardTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGameClientLandRewardTimerUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameClientLandRewardTimerUpdateMethod();
	crJXJGameClientLandRewardTimerUpdateMethod(const crJXJGameClientLandRewardTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJGameClientLandRewardTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJResetLunHuiPageMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJResetLunHuiPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetLunHuiPageMethod();
	crJXJResetLunHuiPageMethod(const crJXJResetLunHuiPageMethod& handle);
	MethodClass(JXJ, JXJResetLunHuiPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strSwitch;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJLunHuiCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJLunHuiCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLunHuiCanvasUpdateMethod();
	crJXJLunHuiCanvasUpdateMethod(const crJXJLunHuiCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJLunHuiCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strRadioGroup;
	std::string m_strSwitch;
	std::string m_strFinishSwitch[5];
	std::string m_strItemName[5];
	std::string m_strItemNum[5];
	std::string m_strItemImg[5];
	std::string m_strListScroll;

	std::string m_strCardShowSwitch[5];
	std::string m_strItemShowSwitch[5];
	std::string m_strCardSwitch[6];
	std::string m_strStoreCardImage[6];
	std::string m_strStoreItem[6];
	std::string m_strBtCard;
	std::string m_strBtItem;

	std::string m_strGetImage;
	std::string m_strNumberSwitch[8];

	std::string m_strlvSwitch[2];
	std::string m_strDefaultCardImg;
	std::string m_strDefaultItemImg;
	//std::string m_strlist;

};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGameServerCycleMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGameServerCycleMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerCycleMethod();
	crJXJGameServerCycleMethod(const crJXJGameServerCycleMethod& handle);
	MethodClass(JXJ, JXJGameServerCycle)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_validCycleLv;//有效轮回等级
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerStoreMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvPlayerStoreMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvPlayerStoreMethod();
	crJXJRecvPlayerStoreMethod(const crJXJRecvPlayerStoreMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerStore)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCreatePlayerStoreDisposeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCreatePlayerStoreDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreatePlayerStoreDisposeMethod();
	crJXJCreatePlayerStoreDisposeMethod(const crJXJCreatePlayerStoreDisposeMethod& handle);
	MethodClass(JXJ, JXJCreatePlayerStoreDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< CRCore::ref_ptr<crJXJPlayerStoreInfo> >m_taskVec;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCreatePlayerStoreRecordDisposeMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCreatePlayerStoreRecordDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreatePlayerStoreRecordDisposeMethod();
	crJXJCreatePlayerStoreRecordDisposeMethod(const crJXJCreatePlayerStoreRecordDisposeMethod& handle);
	MethodClass(JXJ, JXJCreatePlayerStoreRecordDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< CRCore::ref_ptr<crJXJPlayerStoreInfo> >m_taskVec;
};
//////////////////////////////////////////////////////////////////////////
//crJXJCanvasOnShowQueryMethod
//////////////////////////////////////////////////////////////////////////
class crJXJCanvasOnShowQueryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCanvasOnShowQueryMethod();
	crJXJCanvasOnShowQueryMethod(const crJXJCanvasOnShowQueryMethod& handle);
	MethodClass(JXJ, JXJCanvasOnShowQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryPlayerGameStoreMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJQueryPlayerGameStoreMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryPlayerGameStoreMethod();
	crJXJQueryPlayerGameStoreMethod(const crJXJQueryPlayerGameStoreMethod& handle);
	MethodClass(JXJ, JXJQueryPlayerGameStore)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGetPlayerGameStoreMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGetPlayerGameStoreMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGetPlayerGameStoreMethod();
	crJXJGetPlayerGameStoreMethod(const crJXJGetPlayerGameStoreMethod& handle);
	MethodClass(JXJ, JXJGetPlayerGameStore)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//int m_netType;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvGetPlayerStoreFailMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvGetPlayerStoreFailMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvGetPlayerStoreFailMethod();
	crJXJRecvGetPlayerStoreFailMethod(const crJXJRecvGetPlayerStoreFailMethod& handle);
	MethodClass(JXJ, JXJRecvGetPlayerStoreFail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_strCanvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJDoStorePlayerInfoMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJDoStorePlayerInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDoStorePlayerInfoMethod();
	crJXJDoStorePlayerInfoMethod(const crJXJDoStorePlayerInfoMethod& handle);
	MethodClass(JXJ, JXJDoStorePlayerInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJUIAddItemDragMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUIAddItemDragMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAddItemDragMethod();
	crJXJUIAddItemDragMethod(const crJXJUIAddItemDragMethod& handle);
	MethodClass(JXJ, JXJUIAddItemDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	//CRCore::crCanvasNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_strCanvas;
	std::string m_strImgItem[10];
	std::string m_strSwIsOpen[5];
};
//////////////////////////////////////////////////////////////////////////
//crJXJUIAddItemDragMethod
//////////////////////////////////////////////////////////////////////////
class crJXJUILunHuiCanvasCloseMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUILunHuiCanvasCloseMethod();
	crJXJUILunHuiCanvasCloseMethod(const crJXJUILunHuiCanvasCloseMethod& handle);
	MethodClass(JXJ, JXJUILunHuiCanvasClose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode * m_this;
	std::string m_strCanvas;
	std::string m_strJKSwitch[6];
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleAddFriendNewMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleAddFriendNewMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleAddFriendNewMethod();
	crJXJBattleAddFriendNewMethod(const crJXJBattleAddFriendNewMethod& handle);
	MethodClass(JXJ, JXJBattleAddFriendNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJTaskBarCanvasOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJTaskBarCanvasOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTaskBarCanvasOnShowMethod();
	crJXJTaskBarCanvasOnShowMethod(const crJXJTaskBarCanvasOnShowMethod& handle);
	MethodClass(JXJ,JXJTaskBarCanvasOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strSwitch;
	std::string m_strSequence;
};
//////////////////////////////////////////////////////////////////////////
//crJXJGetJiangHunShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJGetJiangHunShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGetJiangHunShowMethod();
	crJXJGetJiangHunShowMethod(const crJXJGetJiangHunShowMethod& handle);
	MethodClass(JXJ,JXJGetJiangHunShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strSwitch;
	std::string m_strSequence;
};
//////////////////////////////////////////////////////////////////////////
//crJXJSkillBufferUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSkillBufferUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSkillBufferUpdateMethod();
	crJXJSkillBufferUpdateMethod(const crJXJSkillBufferUpdateMethod& handle);
	MethodClass(JXJ,JXJSkillBufferUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	float *m_dt;
	std::string m_strImgBox[3][8];
};

//////////////////////////////////////////////////////////////////////////
//crJXJWidgetAlphaflashMethod
//////////////////////////////////////////////////////////////////////////
class crJXJWidgetAlphaflashMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJWidgetAlphaflashMethod();
	crJXJWidgetAlphaflashMethod(const crJXJWidgetAlphaflashMethod& handle);
	MethodClass(JXJ, JXJWidgetAlphaflash)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crImageBoxWidgetNode * m_this;
	bool m_curState;
	float	m_value;
};

//////////////////////////////////////////////////////////////////////////
//crJXJAddBlackName2Method
//////////////////////////////////////////////////////////////////////////
class crJXJAddBlackName2Method : public CRCore::crMethod
{//Client Method
public:
	crJXJAddBlackName2Method();
	crJXJAddBlackName2Method(const crJXJAddBlackName2Method& handle);
	MethodClass(JXJ, JXJAddBlackName2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	//std::string m_armytyperadio;
	//int m_index;
};
//////////////////////////////////////////////////////////////////////////
//crJXJChangeMulitswitchValueMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChangeMulitswitchValueMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJChangeMulitswitchValueMethod();
	crJXJChangeMulitswitchValueMethod(const crJXJChangeMulitswitchValueMethod& handle);
	MethodClass(JXJ, JXJChangeMulitswitchValue)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strMulitswitch;
	int m_value;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleTalkWithFriendMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleTalkWithFriendMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleTalkWithFriendMethod();
	crJXJBattleTalkWithFriendMethod(const crJXJBattleTalkWithFriendMethod& handle);
	MethodClass(JXJ, JXJBattleTalkWithFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strChatCanvas;
	std::string m_strMulitswitch;
	std::string m_strChatInput;
	//int m_value;
};
//////////////////////////////////////////////////////////////////////////
//crJXJClearChengChiOrFubenIDMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClearChengChiOrFubenIDMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClearChengChiOrFubenIDMethod();
	crJXJClearChengChiOrFubenIDMethod(const crJXJClearChengChiOrFubenIDMethod& handle);
	MethodClass(JXJ, JXJClearChengChiOrFubenID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvFubenKillInfoMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvFubenKillInfoMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvFubenKillInfoMethod();
	crJXJRecvFubenKillInfoMethod(const crJXJRecvFubenKillInfoMethod& handle);
	MethodClass(JXJ, JXJRecvFubenKillInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;

	std::string m_strCanvas;
// 	std::string m_strSwitch[3];
// 	std::string m_strInput[3];
// 	std::string m_strSeq[3];
};
//////////////////////////////////////////////////////////////////////////
//crJXJChangeBattleStarUIMethod
//////////////////////////////////////////////////////////////////////////
class crJXJChangeBattleStarUIMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJChangeBattleStarUIMethod();
	crJXJChangeBattleStarUIMethod(const crJXJChangeBattleStarUIMethod& handle);
	MethodClass(JXJ, JXJChangeBattleStarUI)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strMulitswitch;
	//std::string m_value;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleStarCanvasOnShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleStarCanvasOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleStarCanvasOnShowMethod();
	crJXJBattleStarCanvasOnShowMethod(const crJXJBattleStarCanvasOnShowMethod& handle);
	MethodClass(JXJ, JXJBattleStarCanvasOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_strCheckBox;
	std::string m_strMulitswitch;
	std::string m_strSwitch[3];
	std::string m_strInput[3];
	std::string m_strSeq[3];
	//std::string m_value;
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleStarCanvasUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleStarCanvasUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleStarCanvasUpdateMethod();
	crJXJBattleStarCanvasUpdateMethod(const crJXJBattleStarCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJBattleStarCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_strSwitch[3];
	std::string m_strInput[3];
	std::string m_strSeq[3];
};
//////////////////////////////////////////////////////////////////////////
//crJXJBattleStarCanvasInitMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleStarCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleStarCanvasInitMethod();
	crJXJBattleStarCanvasInitMethod(const crJXJBattleStarCanvasInitMethod& handle);
	MethodClass(JXJ, JXJBattleStarCanvasInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	CRCore::crVector2i m_vec[3];
};

//////////////////////////////////////////////////////////////////////////
//crJXJSetShiLiWidgetEnableMethod
//////////////////////////////////////////////////////////////////////////
class crJXJSetShiLiWidgetEnableMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetShiLiWidgetEnableMethod();
	crJXJSetShiLiWidgetEnableMethod(const crJXJSetShiLiWidgetEnableMethod& handle);
	MethodClass(JXJ, JXJSetShiLiWidgetEnable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_strbt[3];
	bool m_visiable;
};
//////////////////////////////////////////////////////////////////////////
//crJXJFubenRushItemShowMethod
//////////////////////////////////////////////////////////////////////////
class crJXJFubenRushItemShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushItemShowMethod();
	crJXJFubenRushItemShowMethod(const crJXJFubenRushItemShowMethod& handle);
	MethodClass(JXJ, JXJFubenRushItemShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_strNameInput[7];
	std::string m_strCountInput[7];
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattleInspireMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIBattleInspireMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIBattleInspireMethod();
	crJXJUIBattleInspireMethod(const crJXJUIBattleInspireMethod& handle);
	MethodClass(JXJ,JXJUIBattleInspire)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
	unsigned short m_itemId;//鼓舞令itemId
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJChatChannelDisableMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChatChannelDisableMethod:public CRCore::crMethod
{//Client 
public:
	crJXJChatChannelDisableMethod();
	crJXJChatChannelDisableMethod(const crJXJChatChannelDisableMethod& handle);
	MethodClass(JXJ,JXJChatChannelDisable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strCountryBt;
	std::string m_strJunTuanBt;
	std::string m_strCountryRd;
	std::string m_strJunTuanRd;
};
class crJXJServerUseJunlingCardMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseJunlingCardMethod();
	crJXJServerUseJunlingCardMethod(const crJXJServerUseJunlingCardMethod& handle);
	MethodClass(JXJ, JXJServerUseJunlingCard)
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
class crJXJServerUseAddFreeCountItemMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseAddFreeCountItemMethod();
	crJXJServerUseAddFreeCountItemMethod(const crJXJServerUseAddFreeCountItemMethod& handle);
	MethodClass(JXJ, JXJServerUseAddFreeCountItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
	int m_index;
};
class crBigMapMoveToMyCityPosMethod :public CRCore::crMethod
{//Client Method
public:
	crBigMapMoveToMyCityPosMethod();
	crBigMapMoveToMyCityPosMethod(const crBigMapMoveToMyCityPosMethod& handle);
	MethodClass(JXJ, BigMapMoveToMyCityPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strBigMapImage;
	std::string m_strOtherMapCanvas;
	std::string m_strOtherMapImage;
	std::string m_strFrame;
};
class crJXJXingjiTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJXingjiTipsMethod();
	crJXJXingjiTipsMethod(const crJXJXingjiTipsMethod& handle);
	MethodClass(JXJ, JXJXingjiTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strTipsCanvas;
	std::string m_strinput;
	std::string m_strSw;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCreateCountryLoseNoticeMethod
//persuade
//////////////////////////////////////////////////////////////////////////
class crJXJCreateCountryLoseNoticeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreateCountryLoseNoticeMethod();
	crJXJCreateCountryLoseNoticeMethod(const crJXJCreateCountryLoseNoticeMethod& handle);
	MethodClass(JXJ, JXJCreateCountryLoseNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_shiliid;
	bool m_isCycle;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJChangeChengchiIDMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJChangeChengchiIDMethod :public CRCore::crMethod
{//GameServer Client Method
public:
	crJXJChangeChengchiIDMethod();
	crJXJChangeChengchiIDMethod(const crJXJChangeChengchiIDMethod& handle);
	MethodClass(JXJ, JXJChangeChengchiID)
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
//crJXJPlayerChengchiIDCheckMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerChengchiIDCheckMethod :public CRCore::crMethod
{//Gameserver Method
public:
	crJXJPlayerChengchiIDCheckMethod();
	crJXJPlayerChengchiIDCheckMethod(const crJXJPlayerChengchiIDCheckMethod& handle);
	MethodClass(JXJ, JXJPlayerChengchiIDCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJHideAddArmyTimerMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJHideAddArmyTimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHideAddArmyTimerMethod();
	crJXJHideAddArmyTimerMethod(const crJXJHideAddArmyTimerMethod& handle);
	MethodClass(JXJ, JXJHideAddArmyTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strSw;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSetBattleCameraMoveableMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSetBattleCameraMoveableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetBattleCameraMoveableMethod();
	crJXJSetBattleCameraMoveableMethod(const crJXJSetBattleCameraMoveableMethod& handle);
	MethodClass(JXJ, JXJSetBattleCameraMoveable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_bEnable;
};

///////////////////////
class crJXJCritExtraAttackMethod : public CRCore::crMethod
{//SceneServer Method 攻击者处于暴击状态
public:
	crJXJCritExtraAttackMethod();
	crJXJCritExtraAttackMethod(const crJXJCritExtraAttackMethod& handle);
	MethodClass(JXJ, JXJCritExtraAttack)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
};

//////////////////////////////////////////////////////////////////////////
class crJXJRecvBattleBrocastMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleBrocastMethod();
	crJXJRecvBattleBrocastMethod(const crJXJRecvBattleBrocastMethod &handle);
	MethodClass(JXJ,JXJRecvBattleBrocast)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
class crJXJCheckInspireBtEnableMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJCheckInspireBtEnableMethod();
	crJXJCheckInspireBtEnableMethod(const crJXJCheckInspireBtEnableMethod &handle);
	MethodClass(JXJ,JXJCheckInspireBtEnable)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strBt;
	std::string m_strimg[24];
	std::string m_canvas;
	std::string m_strBt_defend;//防守方鼓舞按钮
};
//////////////////////////////////////////////////////////////////////////
class crJXJChatChannelSetInitMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChatChannelSetInitMethod();
	crJXJChatChannelSetInitMethod(const crJXJChatChannelSetInitMethod &handle);
	MethodClass(JXJ,JXJChatChannelSetInit)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
class crJXJChatChannelSetCanvasMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChatChannelSetCanvasMethod();
	crJXJChatChannelSetCanvasMethod(const crJXJChatChannelSetCanvasMethod &handle);
	MethodClass(JXJ,JXJChatChannelSetCanvas)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
class crJXJChatChannelSetCanvasUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChatChannelSetCanvasUpdateMethod();
	crJXJChatChannelSetCanvasUpdateMethod(const crJXJChatChannelSetCanvasUpdateMethod &handle);
	MethodClass(JXJ,JXJChatChannelSetCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strChatChannelRadio;
	std::string m_strCheckBox[9];
};
//////////////////////////////////////////////////////////////////////////
class crJXJChatChannelValueChangeMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJChatChannelValueChangeMethod();
	crJXJChatChannelValueChangeMethod(const crJXJChatChannelValueChangeMethod &handle);
	MethodClass(JXJ,JXJChatChannelValueChange)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strRadio;
	int m_index;
};
//////////////////////////////////////////////////////////////////////////
class crJXJSaveChatChannelSetMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJSaveChatChannelSetMethod();
	crJXJSaveChatChannelSetMethod(const crJXJSaveChatChannelSetMethod &handle);
	MethodClass(JXJ,JXJSaveChatChannelSet)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
class crJXJDefaultChatChannelSetMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJDefaultChatChannelSetMethod();
	crJXJDefaultChatChannelSetMethod(const crJXJDefaultChatChannelSetMethod &handle);
	MethodClass(JXJ,JXJDefaultChatChannelSet)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvSaveChatChannelSetMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvSaveChatChannelSetMethod :public CRCore::crMethod
{//GameServer/Client Method
public:
	crJXJRecvSaveChatChannelSetMethod();
	crJXJRecvSaveChatChannelSetMethod(const crJXJRecvSaveChatChannelSetMethod& handle);
	MethodClass(JXJ, JXJRecvSaveChatChannelSet)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	
};
class crJXJRecvBattleDisableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleDisableMethod();
	crJXJRecvBattleDisableMethod(const crJXJRecvBattleDisableMethod& handle);
	MethodClass(JXJ, JXJRecvBattleDisable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvTempClientNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvTempClientNoticeMethod();
	crJXJRecvTempClientNoticeMethod(const crJXJRecvTempClientNoticeMethod& handle);
	MethodClass(JXJ, JXJRecvTempClientNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_strChatCanvas;
	std::string m_strBattleChatCanvas;
	std::string m_strHyperTxt;
	int m_time;
};
class crJXJSycnCenterNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSycnCenterNoticeMethod();
	crJXJSycnCenterNoticeMethod(const crJXJSycnCenterNoticeMethod& handle);
	MethodClass(JXJ, JXJSycnCenterNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJPlayMiniMapSignalMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayMiniMapSignalMethod();
	crJXJPlayMiniMapSignalMethod(const crJXJPlayMiniMapSignalMethod& handle);
	MethodClass(JXJ, JXJPlayMiniMapSignal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	std::string m_strMinimap;
	std::string m_strWidget;
	std::string m_strRotateMap;
	std::vector<float> m_vecPos;
	float m_time;
	std::string m_strWidgetName;
};
class crJXJStopMiniMapSignalMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJStopMiniMapSignalMethod();
	crJXJStopMiniMapSignalMethod(const crJXJStopMiniMapSignalMethod& handle);
	MethodClass(JXJ, JXJStopMiniMapSignal)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strWidgetName;
};
class crJXJJuQingCanvasOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJJuQingCanvasOnShowMethod();
	crJXJJuQingCanvasOnShowMethod(const crJXJJuQingCanvasOnShowMethod& handle);
	MethodClass(JXJ, JXJJuQingCanvasOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strJuQingImg;
	std::string m_strJuQingName;
	std::string m_strJuQingInfo;
};
class crJXJJuQingCanvasFrameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJJuQingCanvasFrameMethod();
	crJXJJuQingCanvasFrameMethod(const crJXJJuQingCanvasFrameMethod& handle);
	MethodClass(JXJ, JXJJuQingCanvasFrame)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float *m_dt;
};

class crJXJShowJuQingCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowJuQingCanvasMethod();
	crJXJShowJuQingCanvasMethod(const crJXJShowJuQingCanvasMethod& handle);
	MethodClass(JXJ, JXJShowJuQingCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_strCanvas;
	std::string m_strImg;
	std::string m_strName;
	std::string m_strInfo;
	float m_time;
};

class crJXJMouseOnUnwalkableCroodMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMouseOnUnwalkableCroodMethod();
	crJXJMouseOnUnwalkableCroodMethod(const crJXJMouseOnUnwalkableCroodMethod& handle);
	MethodClass(JXJ, JXJMouseOnUnwalkableCrood)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
};
class crJXJInitCurrentInstanceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJInitCurrentInstanceMethod();
	crJXJInitCurrentInstanceMethod(const crJXJInitCurrentInstanceMethod& handle);
	MethodClass(JXJ, JXJInitCurrentInstance)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJChangeCurSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeCurSelectRoleMethod();
	crJXJChangeCurSelectRoleMethod(const crJXJChangeCurSelectRoleMethod& handle);
	MethodClass(JXJ, JXJChangeCurSelectRole)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_strCanvas;
};
class crJXJInsertBattleReportDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJInsertBattleReportDisposeMethod();
	crJXJInsertBattleReportDisposeMethod(const crJXJInsertBattleReportDisposeMethod& handle);
	MethodClass(JXJ, JXJInsertBattleReportDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_mailCfg;
};
class crJXJQueryBattleReportDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryBattleReportDisposeMethod();
	crJXJQueryBattleReportDisposeMethod(const crJXJQueryBattleReportDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryBattleReportDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< std::pair<int,int> >m_taskVec;
};
class crJXJMailQueryBattleReportMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMailQueryBattleReportMethod();
	crJXJMailQueryBattleReportMethod(const crJXJMailQueryBattleReportMethod& handle);
	MethodClass(JXJ, JXJMailQueryBattleReport)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crHypertextWidgetNode *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_strCanvas;
	std::string m_strBattleReportTab;
};
class crJXJRecvQueryBattleReportMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvQueryBattleReportMethod();
	crJXJRecvQueryBattleReportMethod(const crJXJRecvQueryBattleReportMethod& handle);
	MethodClass(JXJ, JXJRecvQueryBattleReport)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strTabFileName;
};
//////////////////////////////////////////////////////////////////////////
//crJXJShowBattleReportLinkClickMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowBattleReportLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowBattleReportLinkClickMethod();
	crJXJShowBattleReportLinkClickMethod(const crJXJShowBattleReportLinkClickMethod& handle);
	MethodClass(JXJ, JXJShowBattleReportLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	std::string m_playerName;
	int m_reportID;
	std::string m_strReportCanvas;
	std::string m_strBattleReportTab;
};
//////////////////////////////////////////////////////////////////////////
//crJXJUIZhanBaoUpdateMethod
//////////////////////////////////////////////////////////////////////////

struct ReportPlayerInfo
{
	int count;
	std::string name;

	
};
inline bool operator>(ReportPlayerInfo left,ReportPlayerInfo right)
{
	return left.count > right.count;
}
typedef std::set< ReportPlayerInfo, std::greater<ReportPlayerInfo> > ReportPlayerSet;


class crJXJUIZhanBaoUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIZhanBaoUpdateMethod();
	crJXJUIZhanBaoUpdateMethod(const crJXJUIZhanBaoUpdateMethod& handle);
	MethodClass(JXJ, JXJUIZhanBaoUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strAttackShiliSw;
	std::string m_strDefendShiliSw;
	std::string m_strWinSw;
	std::string m_strBattleName;
	std::string m_strTotal;
	std::string m_strShiliTotal;
	std::string m_strDeadTotal;
	std::string m_strshiliDeadTotal;
	std::string m_strkillList;
	std::string m_strDeadList;
	std::string m_strplayerTotal;
	std::string m_strplayerDead;
	std::string m_strplayerKill;
	std::string m_strexp;
	std::string m_strExploit;
	std::string m_strExpIcon;
	std::string m_strWxploitIcon;
	std::string m_strexpImgName;
	std::string m_strexploitImgName;
	std::string m_strShowBt;
	std::string m_strPlayerName;
};
class crJXJUIShowBattleReportInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShowBattleReportInfoMethod();
	crJXJUIShowBattleReportInfoMethod(const crJXJUIShowBattleReportInfoMethod& handle);
	MethodClass(JXJ, JXJUIShowBattleReportInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvasname;
	std::string m_chatmodesw;
	std::string m_chatinput;
	std::string m_sendbtn;
	std::string m_switch;
	std::string m_strBattleCanvas;
};
class crJXJAddStarToPlayerMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJAddStarToPlayerMethod();
	crJXJAddStarToPlayerMethod(const crJXJAddStarToPlayerMethod& handle);
	MethodClass(JXJ, JXJAddStarToPlayer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	unsigned char m_star;
	CRCore::crVector3b m_index;
};
class crJXJRecvFubenAddStarMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvFubenAddStarMethod();
	crJXJRecvFubenAddStarMethod(const crJXJRecvFubenAddStarMethod& handle);
	MethodClass(JXJ, JXJRecvFubenAddStar)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strSwitch[3];
	std::string m_strSeq[3];
};
class crJXJClickHeadCanvasSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClickHeadCanvasSelectRoleMethod();
	crJXJClickHeadCanvasSelectRoleMethod(const crJXJClickHeadCanvasSelectRoleMethod& handle);
	MethodClass(JXJ, JXJClickHeadCanvasSelectRole)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};
class crJXJFubenCallForHelpMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenCallForHelpMethod();
	crJXJFubenCallForHelpMethod(const crJXJFubenCallForHelpMethod& handle);
	MethodClass(JXJ, JXJFubenCallForHelp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_canvasname;
	std::string m_chatmodesw;
	std::string m_chatinput;
	std::string m_sendbtn;
	std::string m_switch;
};

class crJXJCallForHelpLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCallForHelpLinkClickMethod();
	crJXJCallForHelpLinkClickMethod(const crJXJCallForHelpLinkClickMethod& handle);
	MethodClass(JXJ, JXJCallForHelpLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	int m_roomid;
	int m_sceneId;
};

class crJXJSaveServerCycleInfoMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveServerCycleInfoMethod();
	crJXJSaveServerCycleInfoMethod(const crJXJSaveServerCycleInfoMethod & handle);
	MethodClass(JXJ, JXJSaveServerCycleInfo)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_strInfoFile;
};

class crJXJSkillEffectRangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSkillEffectRangeMethod();
	crJXJSkillEffectRangeMethod(const crJXJSkillEffectRangeMethod & handle);
	MethodClass(JXJ, JXJSkillEffectRange)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
	bool checkPointInRange(const CRCore::crVector3f & point, const CRCore::crVector3f & center, float range);
protected:
	CRCore::crMatrixTransform *m_this;
	CRCore::crVector3f m_centerPos;
	float m_range;
	unsigned char m_targetType;
};

class crJXJSkillEffectRectangleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSkillEffectRectangleMethod();
	crJXJSkillEffectRectangleMethod(const crJXJSkillEffectRectangleMethod & handle);
	MethodClass(JXJ, JXJSkillEffectRectangle)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
	bool checkPointInRect(const CRCore::crVector3f & point, CRPhysics::crMatrixVolumeNode * matrixVol);
protected:
	CRCore::crMatrixTransform *m_this;
	CRCore::ref_ptr<CRPhysics::crMatrixVolumeNode> m_matrixVolume;
	unsigned char m_targetType;
};

class crJXJCancelSkillEffectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCancelSkillEffectMethod();
	crJXJCancelSkillEffectMethod(const crJXJCancelSkillEffectMethod & handle);
	MethodClass(JXJ, JXJCancelSkillEffect)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
};
class crJXJNamedSignalUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNamedSignalUpdateMethod();
	crJXJNamedSignalUpdateMethod(const crJXJNamedSignalUpdateMethod& handle);
	MethodClass(JXJ, JXJNamedSignalUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerCycleCountMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerCycleCountMethod :public CRCore::crMethod
{//Gameserver Method
public:
	crJXJPlayerCycleCountMethod();
	crJXJPlayerCycleCountMethod(const crJXJPlayerCycleCountMethod& handle);
	MethodClass(JXJ, JXJPlayerCycleCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerCycleCountMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvPlayerCycleCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPlayerCycleCountMethod();
	crJXJRecvPlayerCycleCountMethod(const crJXJRecvPlayerCycleCountMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerCycleCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
}

#endif