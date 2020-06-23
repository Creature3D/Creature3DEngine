/**********************************************************************
*
*	文件:	   appMethod2.h
*
*	描述:	   
*
*	作者:	   李婧
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD2_H
#define JXJ_APPMETHOD2_H
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
///点击创建按钮
class crJXJUIOnClickRegisterMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIOnClickRegisterMethod();
	crJXJUIOnClickRegisterMethod(const crJXJUIOnClickRegisterMethod& handle);
	MethodClass(JXJ, JXJUIOnClickRegister)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;

};

///移动窗口时每帧更新步长
class crJXJUIRegisterMoveUpdateMethod :public CRCore::crMethod
{//Client Method
	enum WindowType 
	{
		REGISTER_CHARACTER = 1,  //左边角色选择
		SELECT_POWER,  //右边势力选择
		DIALOG  ///右下对话框
	};
public:
	crJXJUIRegisterMoveUpdateMethod();
	crJXJUIRegisterMoveUpdateMethod(const crJXJUIRegisterMoveUpdateMethod& handle);
	MethodClass(JXJ, JXJUIRegisterMoveUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	float m_fStepLength;
	WindowType m_eWindowType;
	float m_fEndPosition;
	float m_fArrowPosX;
	float m_fArrowPosY;
	float m_fArrowPosBack;
	float m_fArrowShift;
};

///发包到服务端，判断角色是否已经存在
class crJXJCheckPlayerNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckPlayerNameMethod();
	crJXJCheckPlayerNameMethod(const crJXJCheckPlayerNameMethod& handle);
	MethodClass(JXJ, JXJCheckPlayerName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};

///随机并显示角色名
class crJXJUIDisplayPlayerNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIDisplayPlayerNameMethod();
	crJXJUIDisplayPlayerNameMethod(const crJXJUIDisplayPlayerNameMethod& handle);
	MethodClass(JXJ, JXJUIDisplayPlayerName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};


///循环条件IN,OUT
class crJXJUIRandomNameConditionMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIRandomNameConditionMethod();
	crJXJUIRandomNameConditionMethod(const crJXJUIRandomNameConditionMethod& handle);
	MethodClass(JXJ, JXJUIRandomNameCondition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	bool m_bCondition;
	std::string m_InOutFlag;
};

class crJXJUIGetRandomNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGetRandomNameMethod();
	crJXJUIGetRandomNameMethod(const crJXJUIGetRandomNameMethod& handle);
	MethodClass(JXJ, JXJUIGetRandomName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_radio;
};
class crJXJNetReturnRandomNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNetReturnRandomNameMethod();
	crJXJNetReturnRandomNameMethod(const crJXJNetReturnRandomNameMethod& handle);
	MethodClass(JXJ, JXJNetReturnRandomName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_input;
	std::string m_btn;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJShowHeadCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowHeadCanvasMethod();
	crJXJShowHeadCanvasMethod(const crJXJShowHeadCanvasMethod& handle);
	MethodClass(JXJ, JXJShowHeadCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	std::string m_headCanvasName;
	std::string m_inputid;
	int m_buildingnameposz;
	bool m_needCopy;
	int m_buildingid;
	CRCore::ref_ptr<CRCore::crCanvasNode> m_copiedCanvas;
	std::string m_target_canvas;
	std::string m_canvassw;
};
class crJXJBuildNameTouchMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJBuildNameTouchMethod();
	crJXJBuildNameTouchMethod(const crJXJBuildNameTouchMethod& handle);
	MethodClass(JXJ, JXJBuildNameTouch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};
class crJXJGameChatTextSendMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameChatTextSendMethod();
	crJXJGameChatTextSendMethod(const crJXJGameChatTextSendMethod& handle);
	MethodClass(JXJ, JXJGameChatTextSend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvasChat;
	std::string m_chatText;
	std::string m_chatinfo;
	std::string m_chatMode;
	std::string m_sizesw;
	std::string m_unfoldbtn;
	char		m_inRoom;
	std::string m_strLastDisplayMessage;
private:
	void display_message(std::string &msg);
};
class crJXJGameChatTextSuccessMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameChatTextSuccessMethod();
	crJXJGameChatTextSuccessMethod(const crJXJGameChatTextSuccessMethod& handle);
	MethodClass(JXJ, JXJGameChatTextSuccess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasChat;
	std::string m_chatinfo;
	std::string m_chattext;
	std::string m_curmode;
};
class crJXJGameChatTextFailureMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameChatTextFailureMethod();
	crJXJGameChatTextFailureMethod(const crJXJGameChatTextFailureMethod& handle);
	MethodClass(JXJ, JXJGameChatTextFailure)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvasChat;
	std::string m_chatinfo;
};
class crJXJRecvGameChatMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvGameChatMethod();
	crJXJRecvGameChatMethod(const crJXJRecvGameChatMethod& handle);
	MethodClass(JXJ, JXJRecvGameChat)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_chat_canvas;
	std::string m_chat_console;
	std::string m_chat_select;
	std::string m_chat_input;
	std::string m_RoomChatCanvas;
};
class crJXJGameChatModeSelectMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameChatModeSelectMethod();
	crJXJGameChatModeSelectMethod(const crJXJGameChatModeSelectMethod& handle);
	MethodClass(JXJ, JXJGameChatModeSelect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_ModeList;
};
enum JXJChatModeSeletCanvas
{
	ChatModeSelectClose,  //聊天频道选择窗口关闭
	ChatModeSelectOpen    //聊天频道选择窗口打开
};
class crJXJGameChatModeSetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameChatModeSetMethod();
	crJXJGameChatModeSetMethod(const crJXJGameChatModeSetMethod& handle);
	MethodClass(JXJ, JXJGameChatModeSet)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_ModeList;
	std::string m_mode;
	std::string m_curmode;
	std::string m_chatinfo;
	std::string m_transversemode;
};
class crJXJCheckResBuildingLevelMethod : public CRCore::crMethod
{//Server Method
public:
	crJXJCheckResBuildingLevelMethod();
	crJXJCheckResBuildingLevelMethod(const crJXJCheckResBuildingLevelMethod& handle);
	MethodClass(JXJ, JXJCheckResBuildingLevel)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJRecvCheckResBuildingLevelMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvCheckResBuildingLevelMethod();
	crJXJRecvCheckResBuildingLevelMethod(const crJXJRecvCheckResBuildingLevelMethod& handle);
	MethodClass(JXJ, JXJRecvCheckResBuildingLevel)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCheckIFBuildingOpenMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckIFBuildingOpenMethod();
	crJXJCheckIFBuildingOpenMethod(const crJXJCheckIFBuildingOpenMethod& handle);
	MethodClass(JXJ, JXJCheckIFBuildingOpen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
};
//class crJXJCheckIFBuildingOpen2Method :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCheckIFBuildingOpen2Method();
//	crJXJCheckIFBuildingOpen2Method(const crJXJCheckIFBuildingOpen2Method& handle);
//	MethodClass(JXJ, JXJCheckIFBuildingOpen2)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//};
class crJXJUIMarketSaleChangePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMarketSaleChangePageMethod();
	crJXJUIMarketSaleChangePageMethod(const crJXJUIMarketSaleChangePageMethod& handle);
	MethodClass(JXJ, JXJUIMarketSaleChangePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
};
class crJXJUIMarketMySaleChangePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMarketMySaleChangePageMethod();
	crJXJUIMarketMySaleChangePageMethod(const crJXJUIMarketMySaleChangePageMethod& handle);
	MethodClass(JXJ, JXJUIMarketMySaleChangePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
};
class crJXJUIGotoMakeEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGotoMakeEquipMethod();
	crJXJUIGotoMakeEquipMethod(const crJXJUIGotoMakeEquipMethod& handle);
	MethodClass(JXJ, JXJUIGotoMakeEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_equiplist;
	std::string m_scrollbar;
	std::string m_radio;//UI_GBRaido工坊
	std::string m_armyradio;//armyRaido兵营
};
class crJXJCheckScrollBarPosMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckScrollBarPosMethod();
	crJXJCheckScrollBarPosMethod(const crJXJCheckScrollBarPosMethod& handle);
	MethodClass(JXJ, JXJCheckScrollBarPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_equiplist;
	std::string m_scrollbar;
};
class crJXJBuildBackPackListMethod : public CRCore::crMethod
{//GameClient Method 背包
public:
	crJXJBuildBackPackListMethod();
	crJXJBuildBackPackListMethod(const crJXJBuildBackPackListMethod& handle);
	MethodClass(CRNetApp, JXJBuildBackPackList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class crJXJUIFriendChangePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFriendChangePageMethod();
	crJXJUIFriendChangePageMethod(const crJXJUIFriendChangePageMethod& handle);
	MethodClass(JXJ, JXJUIFriendChangePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
	int m_curpageid;
	int m_totalpageid;
};
class crJXJUISendMsgToFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISendMsgToFriendMethod();
	crJXJUISendMsgToFriendMethod(const crJXJUISendMsgToFriendMethod& handle);
	MethodClass(JXJ, JXJUISendMsgToFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvasName;
	std::string m_input;
	std::string m_chatmode;
	std::string m_friendlist;
	std::string m_radio;
	std::string m_battleCanvasName;
	int m_index;
};
class crJXJAddFriendPromptMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAddFriendPromptMethod();
	crJXJAddFriendPromptMethod(const crJXJAddFriendPromptMethod& handle);
	MethodClass(JXJ, JXJAddFriendPrompt)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};
class crJXJGameAddFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameAddFriendMethod();
	crJXJGameAddFriendMethod(const crJXJGameAddFriendMethod& handle);
	MethodClass(JXJ, JXJGameAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	std::string m_tipsinput;
};
class crJXJRecvGameAddFriendMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvGameAddFriendMethod();
	crJXJRecvGameAddFriendMethod(const crJXJRecvGameAddFriendMethod& handle);
	MethodClass(JXJ, JXJRecvGameAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_addcanvas;
};
class crJXJIFAddFriendSuccessMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIFAddFriendSuccessMethod();
	crJXJIFAddFriendSuccessMethod(const crJXJIFAddFriendSuccessMethod& handle);
	MethodClass(JXJ, JXJIFAddFriendSuccess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_text;
};
class crJXJRecvRequestAddFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvRequestAddFriendMethod();
	crJXJRecvRequestAddFriendMethod(const crJXJRecvRequestAddFriendMethod& handle);
	MethodClass(JXJ, JXJRecvRequestAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_sendname;
	std::string m_text;
	std::string m_lastname;
};
class crJXJAddFriendRequestUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAddFriendRequestUpdateMethod();
	crJXJAddFriendRequestUpdateMethod(const crJXJAddFriendRequestUpdateMethod& handle);
	MethodClass(JXJ, JXJAddFriendRequestUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	std::string m_canvas;
	std::string m_sendname;
	std::string m_text;
	std::string m_title;
	std::string m_taskbarcanvas;
	std::string m_addfriendhint;
	std::string m_sequence;
	std::string m_addfriendsw;
};
class crJXJGameSendYesToAddFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameSendYesToAddFriendMethod();
	crJXJGameSendYesToAddFriendMethod(const crJXJGameSendYesToAddFriendMethod& handle);
	MethodClass(JXJ, JXJGameSendYesToAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	std::string m_yesorno;
};
class crJXJRecvGameSendYesToAddFriendMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvGameSendYesToAddFriendMethod();
	crJXJRecvGameSendYesToAddFriendMethod(const crJXJRecvGameSendYesToAddFriendMethod& handle);
	MethodClass(JXJ, JXJRecvGameSendYesToAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_addcanvas;
};
class crJXJRecvGameSendNoToAddFriendMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvGameSendNoToAddFriendMethod();
	crJXJRecvGameSendNoToAddFriendMethod(const crJXJRecvGameSendNoToAddFriendMethod& handle);
	MethodClass(JXJ, JXJRecvGameSendNoToAddFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
};
class crJXJUIQueryFriendListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIQueryFriendListMethod();
	crJXJUIQueryFriendListMethod(const crJXJUIQueryFriendListMethod& handle);
	MethodClass(JXJ, JXJUIQueryFriendList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_mySaleList;
	std::string m_pagenum;
	std::string m_icon1;
	std::string m_icon2;
	std::string m_icon3;
	std::string m_icon4;
	std::string m_icon5;
	std::string m_icon6;
	std::string m_icon7;
	std::string m_fsxxBtn;
	std::string m_fsyjBtn;
	std::string m_schyBtn;
	//int m_lastsec;
	std::string m_hmdTitleSwitch;
	std::string m_contentSwitch;
};
class crJXJUIFriendListUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFriendListUpdateMethod();
	crJXJUIFriendListUpdateMethod(const crJXJUIFriendListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIFriendListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_mySaleList;
	std::string m_pagenum;
	std::string m_icon[7];
	std::string m_buttonsw;
};

class crJXJGameDeleteFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameDeleteFriendMethod();
	crJXJGameDeleteFriendMethod(const crJXJGameDeleteFriendMethod& handle);
	MethodClass(JXJ, JXJGameDeleteFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_friendlist;
	int m_index;
	std::string m_groupcanvas;
};
class crJXJUIRecvGameDeleteFriendMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJUIRecvGameDeleteFriendMethod();
	crJXJUIRecvGameDeleteFriendMethod(const crJXJUIRecvGameDeleteFriendMethod& handle);
	MethodClass(JXJ, JXJUIRecvGameDeleteFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_text;
};
class crJXJIfClickOtherNameMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJIfClickOtherNameMethod();
	crJXJIfClickOtherNameMethod(const crJXJIfClickOtherNameMethod& handle);
	MethodClass(JXJ, JXJIfClickOtherName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRText::crText *m_text;
	std::string m_canvas;
};
class crJXJGameAddFriend2Method :public CRCore::crMethod
{//Client Method
public:
	crJXJGameAddFriend2Method();
	crJXJGameAddFriend2Method(const crJXJGameAddFriend2Method& handle);
	MethodClass(JXJ, JXJGameAddFriend2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJFriendOtherOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFriendOtherOnShowMethod();
	crJXJFriendOtherOnShowMethod(const crJXJFriendOtherOnShowMethod& handle);
	MethodClass(JXJ, JXJFriendOtherOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	std::string m_canvas;
};
class crJXJChatTimerUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJChatTimerUpdateMethod();
	crJXJChatTimerUpdateMethod(const crJXJChatTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJChatTimerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float *m_dt;
};
class crJXJUIKnapsackListOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnapsackListOnShowMethod();
	crJXJUIKnapsackListOnShowMethod(const crJXJUIKnapsackListOnShowMethod& handle);
	MethodClass(JXJ, JXJUIKnapsackListOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_itemtype;
	std::string m_curpagenum;
};
class crJXJUIKnapsackListUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnapsackListUpdateMethod();
	crJXJUIKnapsackListUpdateMethod(const crJXJUIKnapsackListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIKnapsackListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_curpagenum;
	std::string m_defaulticon;
};
typedef std::multimap<int,CRCore::ref_ptr<crBagItemData>,std::less<int> >BagItemMultiMap;//itemid,data
typedef std::multimap<_crInt64,CRCore::ref_ptr<crBagItemData>,std::greater<_crInt64> >BagItemMultiMap2;//(itemid,品质,类别),data
class crJXJUIArrangeKnapsackListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIArrangeKnapsackListMethod();
	crJXJUIArrangeKnapsackListMethod(const crJXJUIArrangeKnapsackListMethod& handle);
	MethodClass(JXJ, JXJUIArrangeKnapsackList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_curpagenum;
};
class crJXJRecvArrangeKnapsackListMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvArrangeKnapsackListMethod();
	crJXJRecvArrangeKnapsackListMethod(const crJXJRecvArrangeKnapsackListMethod& handle);
	MethodClass(JXJ, JXJRecvArrangeKnapsackList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_jkCanvas;
};

class crJXJCountryVoteGMData: public CRCore::Referenced
{
public:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_name;
	int m_voteNum;
};

class crJXJRecvGMOrderMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvGMOrderMethod();
	crJXJRecvGMOrderMethod(const crJXJRecvGMOrderMethod& handle);
	MethodClass(JXJ, JXJRecvGMOrder)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_strBattleCanvas;
	std::string m_strServerCycleInfoFile;
};
class crJXJUIKnapsackItemTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnapsackItemTipInfoMethod();
	crJXJUIKnapsackItemTipInfoMethod(const crJXJUIKnapsackItemTipInfoMethod& handle);
	MethodClass(JXJ, JXJUIKnapsackItemTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
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
class crJXJUILordEquipUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUILordEquipUpdateMethod();
	crJXJUILordEquipUpdateMethod(const crJXJUILordEquipUpdateMethod& handle);
	MethodClass(JXJ, JXJUILordEquipUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_armyRadio;
};
class crJXJUILairdEquipUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUILairdEquipUpdateMethod();
	crJXJUILairdEquipUpdateMethod(const crJXJUILairdEquipUpdateMethod& handle);
	MethodClass(JXJ, JXJUILairdEquipUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_GLList;
	std::string m_money;
	std::string m_material1;
	std::string m_material2;
	std::string m_material3;
	std::string m_moneycount;
	std::string m_matecount1;
	std::string m_matecount2;
	std::string m_matecount3;
	std::string m_equippic;
	std::string m_makeBtn;
	std::string m_info;
	std::string m_tips;
	std::string m_tiptext;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	std::string m_lordEquipLv1;
	std::string m_lordEquipLv2;
	std::string m_lordEquipLv3;
	std::string m_lordEquipLv4;
	std::string m_lordEquipLv5;
	std::string m_lordEquipLv6;
	std::string m_buybtn[3];
};
class crJXJUIMakeLordEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMakeLordEquipMethod();
	crJXJUIMakeLordEquipMethod(const crJXJUIMakeLordEquipMethod& handle);
	MethodClass(JXJ, JXJUIMakeLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_equipList;
	std::string m_makeCount;
};
class crJXJRecvMakeLordEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvMakeLordEquipMethod();
	crJXJRecvMakeLordEquipMethod(const crJXJRecvMakeLordEquipMethod& handle);
	MethodClass(JXJ, JXJRecvMakeLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	unsigned char getRefidID(int refineindex,CRCore::crVector2i vec2,CRCore::crVector3ub refine3ub);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJUIShowItemCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShowItemCanvasMethod();
	crJXJUIShowItemCanvasMethod(const crJXJUIShowItemCanvasMethod& handle);
	MethodClass(JXJ, JXJUIShowItemCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
	std::string m_canvas;
	std::string m_canvas2;
	std::string m_equipList;
	std::string m_input;
	std::string m_inputmoney;
	std::string m_switchTips;
	std::string m_tipsText;
	unsigned short m_itemId;//行囊令itemId
};
class crJXJUISetSaleCountMaxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetSaleCountMaxMethod();
	crJXJUISetSaleCountMaxMethod(const crJXJUISetSaleCountMaxMethod& handle);
	MethodClass(JXJ, JXJUISetSaleCountMax)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_equipList;
	std::string m_canvas;
	std::string m_input;
	std::string m_canvasbb;
	std::string m_sellok;
};
class crJXJUISetSaleCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetSaleCountMethod();
	crJXJUISetSaleCountMethod(const crJXJUISetSaleCountMethod& handle);
	MethodClass(JXJ, JXJUISetSaleCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	std::string m_step;
};
class crJXJUISaleItemEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISaleItemEquipMethod();
	crJXJUISaleItemEquipMethod(const crJXJUISaleItemEquipMethod& handle);
	MethodClass(JXJ, JXJUISaleItemEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
};
class crJXJRecvSaleItemEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSaleItemEquipMethod();
	crJXJRecvSaleItemEquipMethod(const crJXJRecvSaleItemEquipMethod& handle);
	MethodClass(JXJ, JXJRecvSaleItemEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_fubenRushCanvas;
	std::string m_fubenRushInput;
	std::string m_cfg;
};
class crJXJUIOnFriendMailingMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIOnFriendMailingMethod();
	crJXJUIOnFriendMailingMethod(const crJXJUIOnFriendMailingMethod& handle);
	MethodClass(JXJ,JXJUIOnFriendMailing)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_friendcanvas;
	std::string m_friendlist;
	std::string m_mailcanvas;
	std::string m_mailswitch;
	std::string m_mailnameinput;
	std::string m_mailtitle1;
	std::string m_mailtitle2;
};
class crJXJUICheckInputNumMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICheckInputNumMethod();
	crJXJUICheckInputNumMethod(const crJXJUICheckInputNumMethod& handle);
	MethodClass(JXJ, JXJUICheckInputNum)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};
class crJXJUIKnapsackBuyOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnapsackBuyOnShowMethod();
	crJXJUIKnapsackBuyOnShowMethod(const crJXJUIKnapsackBuyOnShowMethod& handle);
	MethodClass(JXJ, JXJUIKnapsackBuyOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_image[8];
	std::string m_time[8];
};
class crJXJUIKnapsackBuyItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnapsackBuyItemMethod();
	crJXJUIKnapsackBuyItemMethod(const crJXJUIKnapsackBuyItemMethod& handle);
	MethodClass(JXJ, JXJUIKnapsackBuyItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
};
class crJXJRecvKnapsackBuyItemMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvKnapsackBuyItemMethod();
	crJXJRecvKnapsackBuyItemMethod(const crJXJRecvKnapsackBuyItemMethod& handle);
	MethodClass(JXJ, JXJRecvKnapsackBuyItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
};
class crJXJItemSaleTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJItemSaleTimerMethod();
	crJXJItemSaleTimerMethod(const crJXJItemSaleTimerMethod& handle);
	MethodClass(JXJ, JXJItemSaleTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
};
class crJXJRefreshSaleItemTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRefreshSaleItemTimeMethod();
	crJXJRefreshSaleItemTimeMethod(const crJXJRefreshSaleItemTimeMethod& handle);
	MethodClass(JXJ, JXJRefreshSaleItemTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuyKnapsackBagMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyKnapsackBagMethod();
	crJXJUIBuyKnapsackBagMethod(const crJXJUIBuyKnapsackBagMethod& handle);
	MethodClass(JXJ, JXJUIBuyKnapsackBag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_inputcount;
	std::string m_inputmoney;
	std::string m_moneycanvas;
	unsigned short m_itemId;//使用道具开启背包
};
class crJXJRecvBuyKnapsackBagMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyKnapsackBagMethod();
	crJXJRecvBuyKnapsackBagMethod(const crJXJRecvBuyKnapsackBagMethod& handle);
	MethodClass(JXJ, JXJRecvBuyKnapsackBag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tips;
	std::string m_canvas;
	std::string m_input;
};
class crJXJUIChangeItemPosMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeItemPosMethod();
	crJXJUIChangeItemPosMethod(const crJXJUIChangeItemPosMethod& handle);
	MethodClass(JXJ, JXJUIChangeItemPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_curpagenum;
	unsigned char m_type;
	std::string m_image[5];
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJRecvChangeItemPosMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeItemPosMethod();
	crJXJRecvChangeItemPosMethod(const crJXJRecvChangeItemPosMethod& handle);
	MethodClass(JXJ, JXJRecvChangeItemPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tips;
	std::string m_input;
};
class crJXJPutOnEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPutOnEquipMethod();
	crJXJPutOnEquipMethod(const crJXJPutOnEquipMethod& handle);
	MethodClass(JXJ, JXJPutOnEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_bagcanvas;
	std::string m_allusecheck;
	std::string m_loadingcanvas;
	bool m_isconfirm;
};
class crJXJRecvPutOnEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvPutOnEquipMethod();
	crJXJRecvPutOnEquipMethod(const crJXJRecvPutOnEquipMethod& handle);
	MethodClass(JXJ, JXJRecvPutOnEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_tips;
	//std::string m_input;
};
class crJXJUISplitItemEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISplitItemEquipMethod();
	crJXJUISplitItemEquipMethod(const crJXJUISplitItemEquipMethod& handle);
	MethodClass(JXJ, JXJUISplitItemEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	std::string m_tips;
	std::string m_inputtips;
};
class crJXJRecvSplitItemEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSplitItemEquipMethod();
	crJXJRecvSplitItemEquipMethod(const crJXJRecvSplitItemEquipMethod& handle);
	MethodClass(JXJ, JXJRecvSplitItemEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tips;
	std::string m_input;
};
class crJXJUIChangeBagPageMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeBagPageMethod();
	crJXJUIChangeBagPageMethod(const crJXJUIChangeBagPageMethod& handle);
	MethodClass(JXJ, JXJUIChangeBagPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	//CRCore::crCanvasNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_radio;
	std::string m_myBagList;
};
class crJXJUISetAcceptDragMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetAcceptDragMethod();
	crJXJUISetAcceptDragMethod(const crJXJUISetAcceptDragMethod& handle);
	MethodClass(JXJ, JXJUISetAcceptDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode *m_this;
};

class crJXJUIJiangkaListUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiangkaListUpdateMethod();
	crJXJUIJiangkaListUpdateMethod(const crJXJUIJiangkaListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIJiangkaListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_itemtype2;
	std::string m_itemtype3;
	//std::string m_sellcanvas;
	//std::string m_upgradecanvas;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_defaulticon;
	//std::string m_whiteframe;
	//std::string m_greenframe;
	//std::string m_blueframe;
	//std::string m_purpleframe;
	//std::string m_orangeframe;
	//std::string m_orangeupgradecanvas;
	std::string m_frame[10];
};
class crJXJUIjiangkaTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIjiangkaTipInfoMethod();
	crJXJUIjiangkaTipInfoMethod(const crJXJUIjiangkaTipInfoMethod& handle);
	MethodClass(JXJ, JXJUIjiangkaTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
	std::string m_tips;
	std::string m_card;
	std::string m_backboard;
	std::string m_guojia;
	std::string m_shuxing;
	std::string m_bingzhong;
	std::string m_leixing;
	std::string m_jinengmiaoshu;
	std::string m_nameimage;
	std::string m_jineng;
	std::string m_cardlvsw;
};
class crJXJUIJiangkaBagPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiangkaBagPageMethod();
	crJXJUIJiangkaBagPageMethod(const crJXJUIJiangkaBagPageMethod& handle);
	MethodClass(JXJ, JXJUIJiangkaBagPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
};
class crJXJAddOneJiangkaMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAddOneJiangkaMethod();
	crJXJAddOneJiangkaMethod(const crJXJAddOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJAddOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio1;
	std::string m_radio2;
	//std::string m_radio3;
	int m_index;
};
class crJXJUIAddJiangKaMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIAddJiangKaMethod();
	crJXJUIAddJiangKaMethod(const crJXJUIAddJiangKaMethod& handle);
	MethodClass(JXJ, JXJUIAddJiangKa)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_canvas;
	std::string m_formationbtn[6];
	std::string m_formationsw[6];
};
//class crJXJRecvAddJiangKaMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvAddJiangKaMethod();
//	crJXJRecvAddJiangKaMethod(const crJXJRecvAddJiangKaMethod& handle);
//	MethodClass(JXJ, JXJRecvAddJiangKa)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_tips;
//	std::string m_input;
//};
class crJXJUIAddJiangKaDragMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAddJiangKaDragMethod();
	crJXJUIAddJiangKaDragMethod(const crJXJUIAddJiangKaDragMethod& handle);
	MethodClass(JXJ, JXJUIAddJiangKaDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	//CRCore::crCanvasNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_canvas;
	std::string m_btn[9];
	std::string m_colorbox[6];
	//std::string m_canvas3;
	std::string m_cardUpgradeSw;
	std::string m_updatecardiconname;
	std::string m_needcardiconname1;
	std::string m_needcardiconname2;
	std::string m_needcardiconname3;
	//std::string m_orangecardupgradecanvas;
	std::string m_strPersuadeCanvas;
	std::string m_strImageBox;
	std::string m_strPersuadeSw;
	std::string m_strPersuadeSw_Time;
	std::string m_strTimeText;
	std::string m_strPersuadeSw_Level;
	std::string m_strLunHuiCanvas;
	//std::string m_strSwIsOpen[5];
	std::string m_strImgCard[CycleCardOnePageCount][2];
	std::string m_strSwCardLv[CycleCardOnePageCount];
	std::string m_Jk_Yanshi;
};
class crJXJShowKnapsackCanvasMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJShowKnapsackCanvasMethod();
	crJXJShowKnapsackCanvasMethod(const crJXJShowKnapsackCanvasMethod& handle);
	MethodClass(JXJ, JXJShowKnapsackCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio;
};
class crJXJUIFeudalLordUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFeudalLordUpdateMethod();
	crJXJUIFeudalLordUpdateMethod(const crJXJUIFeudalLordUpdateMethod& handle);
	MethodClass(JXJ, JXJUIFeudalLordUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	//////////////////////////////////////////////////////////////////////////
	std::string m_name;
	std::string m_guoJia;
	std::string m_level;
	std::string m_guanzhi;
	std::string m_gongxun;
	std::string m_bingfa;
	std::string m_zhenfa;
	std::string m_tongshuai;
	std::string m_dongcha;
	std::string m_equip[5];
	std::string m_touxiang;
	std::string m_exp;
	std::string m_defaultname[5];
	std::string m_btnAdd[4];
	std::string m_radioBtn[4];
	std::string m_radioGroup;
	std::string m_zhan;
};
//class crJXJGetOneJiangkaPromptMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJGetOneJiangkaPromptMethod();
//	crJXJGetOneJiangkaPromptMethod(const crJXJGetOneJiangkaPromptMethod& handle);
//	MethodClass(JXJ, JXJGetOneJiangkaPrompt)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_radio;
//	unsigned char m_btnindex;
//	std::string m_promptTitle;
//	std::string m_promptCanvas;
//	std::string m_information;
//	std::string m_OKbtn;
//	std::string m_font;
//	int m_characterSize;
//	CRCore::crVector4f m_textColor;
//	CRCore::crVector4f m_goldColor;
//};
//class crJXJGetOneJiangkaMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJGetOneJiangkaMethod();
//	crJXJGetOneJiangkaMethod(const crJXJGetOneJiangkaMethod& handle);
//	MethodClass(JXJ, JXJGetOneJiangka)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_radio;
//	unsigned char m_btnindex;
//};
//class crJXJRecvGetJiangKaMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvGetJiangKaMethod();
//	crJXJRecvGetJiangKaMethod(const crJXJRecvGetJiangKaMethod& handle);
//	MethodClass(JXJ, JXJRecvGetJiangKa)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_tips;
//	std::string m_input;
//};
class crJXJTestNodeSizeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTestNodeSizeMethod();
	crJXJTestNodeSizeMethod(const crJXJTestNodeSizeMethod& handle);
	MethodClass(JXJ, JXJTestNodeSize)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_equipList;
	float m_sizex,m_sizey;
	float m_betweenx,m_betweeny;
	std::string m_scrollbar;
};
class crJXJTestNodeSize2Method : public CRCore::crMethod
{//Client Method
public:
	crJXJTestNodeSize2Method();
	crJXJTestNodeSize2Method(const crJXJTestNodeSize2Method& handle);
	MethodClass(JXJ, JXJTestNodeSize2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_equipList[9];
	float m_charactersize;
	float m_sizex,m_sizey;
	float m_linespacing;
	float m_underlinespacing;
	std::string m_scrollbar;
	std::string m_table;
};
class crJXJBattleScrollBarMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleScrollBarMethod();
	crJXJBattleScrollBarMethod(const crJXJBattleScrollBarMethod& handle);
	MethodClass(JXJ, JXJBattleScrollBar)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_listctrl;
	std::string m_scrollbar;
	int m_linevalue;
};
class crJXJShowSystemNewsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJShowSystemNewsMethod();
	crJXJShowSystemNewsMethod(const crJXJShowSystemNewsMethod& handle);
	MethodClass(JXJ, JXJShowSystemNews)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
	float m_interval;
};
class crJXJUIUpdateSystemNewsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateSystemNewsMethod();
	crJXJUIUpdateSystemNewsMethod(const crJXJUIUpdateSystemNewsMethod& handle);
	MethodClass(JXJ, JXJUIUpdateSystemNews)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};
class crJXJRecvSystemNewsMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvSystemNewsMethod();
	crJXJRecvSystemNewsMethod(const crJXJRecvSystemNewsMethod& handle);
	MethodClass(JXJ, JXJRecvSystemNews)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
};
typedef std::vector< CRCore::ref_ptr<CRUI::crButtonWidgetNode> >ButtonVec;
typedef std::vector< CRCore::ref_ptr<CRCore::crWidgetNode> >WidgetVec;
typedef std::vector< CRCore::ref_ptr<CRUI::crStaticTextWidgetNode> >NameVec;
typedef std::vector< CRCore::crVector2f >PosVec;
class crJXJUIBigMapUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBigMapUpdateMethod();
	crJXJUIBigMapUpdateMethod(const crJXJUIBigMapUpdateMethod& handle);
	MethodClass(JXJ, JXJUIBigMapUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_citysmall;
	std::string m_citymedian;
	std::string m_citybig;
	std::string m_cityflag;
	std::string m_citynamewhite;
	std::string m_citynameyellow;
	std::string m_gate;
	std::string m_gateflag;
	std::string m_gatename;
	std::string m_backboard;
	CRCore::crVector2 m_mapsize;
	CRCore::crVector2 m_mapscale;
	ButtonVec m_btnVec;
	WidgetVec m_flagVec;
	NameVec m_nameVec;
	bool m_init;
};
class crJXJUIUpdateBattleListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateBattleListMethod();
	crJXJUIUpdateBattleListMethod(const crJXJUIUpdateBattleListMethod& handle);
	MethodClass(JXJ, JXJUIUpdateBattleList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_gamelist;
};
class crJXJUISetBuildCanvasNameMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISetBuildCanvasNameMethod();
	crJXJUISetBuildCanvasNameMethod(const crJXJUISetBuildCanvasNameMethod& handle);
	MethodClass(JXJ, JXJUISetBuildCanvasName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_name;
	std::string m_info;
	std::string m_buildingname;
};
class crJXJUISetDefaultRecruitNumMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISetDefaultRecruitNumMethod();
	crJXJUISetDefaultRecruitNumMethod(const crJXJUISetDefaultRecruitNumMethod& handle);
	MethodClass(JXJ, JXJUISetDefaultRecruitNum)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
	int m_fomationsize;
};
class crJXJListCtrlScrollMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJListCtrlScrollMethod();
	crJXJListCtrlScrollMethod(const crJXJListCtrlScrollMethod& handle);
	MethodClass(JXJ, JXJListCtrlScroll)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_srollbar;
	unsigned char m_type;
};
class crJXJResetScrollBarValueMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetScrollBarValueMethod();
	crJXJResetScrollBarValueMethod(const crJXJResetScrollBarValueMethod& handle);
	MethodClass(JXJ, JXJResetScrollBarValue)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_srollbar;
};
class crJXJAutoMakeEquipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAutoMakeEquipsMethod();
	crJXJAutoMakeEquipsMethod(const crJXJAutoMakeEquipsMethod& handle);
	MethodClass(JXJ, JXJAutoMakeEquips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_inputneed;
	std::string m_msgbox;
	std::string m_inputMoney;
	std::string m_inputWood;
	std::string m_inputIron;
	std::string m_inputHorse;
	std::string m_inputFood;
	std::string m_inputRecruitTime;
	std::string m_inputRecruitTime_2;
	CRCore::crVector4 m_copperColor;
	std::string m_recruitcount;
};
class crJXJUIAutoMakeTroopsEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIAutoMakeTroopsEquipMethod();
	crJXJUIAutoMakeTroopsEquipMethod(const crJXJUIAutoMakeTroopsEquipMethod& handle);
	MethodClass(JXJ, JXJUIAutoMakeTroopsEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJSetJiangkaOpenCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetJiangkaOpenCanvasMethod();
	crJXJSetJiangkaOpenCanvasMethod(const crJXJSetJiangkaOpenCanvasMethod& handle);
	MethodClass(JXJ, JXJSetJiangkaOpenCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_canvas2;
	std::string m_radio1;
	std::string m_radio2;
	std::string m_radio3;
	std::string m_formcanvas;
	std::string m_orangeaddstarcanvas;
};
//class crJXJSetInviteTypeMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJSetInviteTypeMethod();
//	crJXJSetInviteTypeMethod(const crJXJSetInviteTypeMethod& handle);
//	MethodClass(JXJ, JXJSetInviteType)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	unsigned char m_invitetype;
//};
class crJXJSetCityFlagMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetCityFlagMethod();
	crJXJSetCityFlagMethod(const crJXJSetCityFlagMethod& handle);
	MethodClass(JXJ, JXJSetCityFlag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_flagname;
	std::string m_weiflag;
	std::string m_shuflag;
	std::string m_wuflag;
	std::string m_qunflag;
};
class crJXJSetBarrackFlagMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetBarrackFlagMethod();
	crJXJSetBarrackFlagMethod(const crJXJSetBarrackFlagMethod& handle);
	MethodClass(JXJ, JXJSetBarrackFlag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_flagname;
	std::string m_weiflag;
	std::string m_shuflag;
	std::string m_wuflag;
};
class crJXJUIGongfangEquipWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongfangEquipWalkMethod();
	crJXJUIGongfangEquipWalkMethod(const crJXJUIGongfangEquipWalkMethod& handle);
	MethodClass(JXJ, JXJUIGongfangEquipWalk)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_step;
	std::string m_makeequiptype;
	std::string m_equiptype;
	std::string m_equiplevel;
};
class crJXJUIGongfangEquipResetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongfangEquipResetMethod();
	crJXJUIGongfangEquipResetMethod(const crJXJUIGongfangEquipResetMethod& handle);
	MethodClass(JXJ, JXJUIGongfangEquipReset)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJUIGongfangEquipUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongfangEquipUpdateMethod();
	crJXJUIGongfangEquipUpdateMethod(const crJXJUIGongfangEquipUpdateMethod& handle);
	MethodClass(JXJ, JXJUIGongfangEquipUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_ForgeTypeRadio;
	std::string m_EquipRadio;
	std::string m_ArmyEquipRadio;
	std::string m_LordEquipRadio;
	std::string m_armUp;
	std::string m_armDown;
	std::string m_pagenum;
};
class crJXJUIEquipListUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIEquipListUpdateMethod();
	crJXJUIEquipListUpdateMethod(const crJXJUIEquipListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIEquipListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_troopsList;
	std::string m_jinglianBtn;
	std::string m_fulianBtn;
	std::string m_ronglianBtn;
};
class crJXJUISellCardUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISellCardUpdateMethod();
	crJXJUISellCardUpdateMethod(const crJXJUISellCardUpdateMethod& handle);
	MethodClass(JXJ, JXJUISellCardUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_cardList;
	std::string m_sellcardbtn;
	std::string m_cardframe[10];
	std::string m_jianghun;
};
class crJXJUISellOneJiangkaMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISellOneJiangkaMethod();
	crJXJUISellOneJiangkaMethod(const crJXJUISellOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJUISellOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_cardType;
	std::string m_cardList;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
//	std::string m_sellCanvas;
//	std::string m_sellList;
};
class crJXJUISellAllSelJiangkaMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISellAllSelJiangkaMethod();
	crJXJUISellAllSelJiangkaMethod(const crJXJUISellAllSelJiangkaMethod& handle);
	MethodClass(JXJ, JXJUISellAllSelJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_copperColor;
};
class crJXJUISellAllSelJiangkaOKMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISellAllSelJiangkaOKMethod();
	crJXJUISellAllSelJiangkaOKMethod(const crJXJUISellAllSelJiangkaOKMethod& handle);
	MethodClass(JXJ, JXJUISellAllSelJiangkaOK)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJRecvSellJiangkaOKMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvSellJiangkaOKMethod();
	crJXJRecvSellJiangkaOKMethod(const crJXJRecvSellJiangkaOKMethod& handle);
	MethodClass(JXJ, JXJRecvSellJiangkaOK)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_canvas;
	//std::string m_input;
};
class crJXJUIUndoSellOneJiangkaMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUndoSellOneJiangkaMethod();
	crJXJUIUndoSellOneJiangkaMethod(const crJXJUIUndoSellOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJUIUndoSellOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_cardType;
	std::string m_cardList;
	std::string m_cardCanvas;
};
class crJXJUIJiangkaUpgradeOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiangkaUpgradeOnShowMethod();
	crJXJUIJiangkaUpgradeOnShowMethod(const crJXJUIJiangkaUpgradeOnShowMethod& handle);
	MethodClass(JXJ, JXJUIJiangkaUpgradeOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_icon[5];
	std::string m_input[3];
	std::string m_btn;
	std::string m_pic;
};
class crJXJUIJiangkaUpgradeUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiangkaUpgradeUpdateMethod();
	crJXJUIJiangkaUpgradeUpdateMethod(const crJXJUIJiangkaUpgradeUpdateMethod& handle);
	MethodClass(JXJ, JXJUIJiangkaUpgradeUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_icon[5];
	std::string m_switch[5];
	std::string m_input[3];
	std::string m_btn;
	//std::string m_cardcanvas;
	std::string m_pic;
	std::string m_costinput;
	std::string m_cardlvsw[2];
	std::string m_cardUpgradeSw;
};
class crJXJIconjiangkaTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIconjiangkaTipInfoMethod();
	crJXJIconjiangkaTipInfoMethod(const crJXJIconjiangkaTipInfoMethod& handle);
	MethodClass(JXJ, JXJIconjiangkaTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_card;
	std::string m_backboard;
	std::string m_guojia;
	std::string m_shuxing;
	std::string m_bingzhong;
	std::string m_leixing;
	std::string m_jinengmiaoshu;
	std::string m_nameimage;
	std::string m_jineng;
	int m_infotype;
	int m_cardindex;
	std::string m_ExchangeCanvas;
	std::string m_mailCanvas;
	int m_mailCardIndex;
	int m_onlineRewardIndex;
	std::string m_cardlvsw;
};
class crJXJUIJiangkaUpgradeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiangkaUpgradeMethod();
	crJXJUIJiangkaUpgradeMethod(const crJXJUIJiangkaUpgradeMethod& handle);
	MethodClass(JXJ, JXJUIJiangkaUpgrade)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_orangecanvas;
	std::string m_orangedonghua[5];
};
class crJXJRecvJiangkaUpgradeMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvJiangkaUpgradeMethod();
	crJXJRecvJiangkaUpgradeMethod(const crJXJRecvJiangkaUpgradeMethod& handle);
	MethodClass(JXJ, JXJRecvJiangkaUpgrade)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_card;
	std::string m_nameimage;
	std::string m_backboard;
	std::string m_guojia;
	std::string m_cardlvsw;
	std::string m_orangecanvas;
	std::string m_orangedonghua[5];
};
class crJXJSetCanvasStartPosMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetCanvasStartPosMethod();
	crJXJSetCanvasStartPosMethod(const crJXJSetCanvasStartPosMethod& handle);
	MethodClass(JXJ, JXJSetCanvasStartPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	float m_startpos;
};
class crJXJCheckBattleFubenBtnMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckBattleFubenBtnMethod();
	crJXJCheckBattleFubenBtnMethod(const crJXJCheckBattleFubenBtnMethod& handle);
	MethodClass(JXJ, JXJCheckBattleFubenBtn)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;//
	std::string m_btn;
	std::string m_canvas2;
	std::string m_btn2;
};	
class crJXJWriteLetterSwitchMethod:public CRCore::crMethod
{
public:
	crJXJWriteLetterSwitchMethod();
	crJXJWriteLetterSwitchMethod(const crJXJWriteLetterSwitchMethod& handle);
	MethodClass(JXJ, JXJWriteLetterSwitch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_send_to;
	std::string m_title;
	std::string m_switch;
	std::string m_nameinput;
	std::string m_nameinput2;
	std::string m_titleinput;
	std::string m_titleinput2;
	int m_index;
};
class crJXJSetMailSendNameMethod:public CRCore::crMethod
{
public:
	crJXJSetMailSendNameMethod();
	crJXJSetMailSendNameMethod(const crJXJSetMailSendNameMethod& handle);
	MethodClass(JXJ, JXJSetMailSendName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListBoxWidgetNode *m_this;
	std::string m_recvname;
	std::string m_friendlist;
	std::string m_friendtab;
};
class crJXJCheckOneMailSuccessMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckOneMailSuccessMethod();
	crJXJCheckOneMailSuccessMethod(const crJXJCheckOneMailSuccessMethod& handle);
	MethodClass(JXJ, JXJCheckOneMailSuccess)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crWidgetNode* m_this;
	std::string m_mailchoice[5];//
	std::string m_mailinput[5];
	std::string m_iconSwitch[5];
	std::string m_newmailflag[6];
	std::string m_mailcanvas;
	std::string m_switch;
	std::string m_itemIcon[5];
	std::string m_cardIcon[5];
	std::string m_cardSwitch[5];
	//int m_mailID;
};	
class crJXJCheckOneMailMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckOneMailMethod();
	crJXJCheckOneMailMethod(const crJXJCheckOneMailMethod& handle);
	MethodClass(JXJ, JXJCheckOneMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioWidgetNode* m_this;
	int m_index;
	std::string m_deletebtn;
	std::string m_friendList;
	std::string m_mailradio[5];
};	
class crJXJMailBodyRequestMethod:public CRCore::crMethod
{// client and server
public:
	crJXJMailBodyRequestMethod();
	crJXJMailBodyRequestMethod(const crJXJMailBodyRequestMethod& handle);
	MethodClass(JXJ, JXJMailBodyRequest)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData				*m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
	int										m_net_type;
	std::string m_mail_canvas;
	std::string m_input_body;
};
class crJXJMailBodyQueryingMethod:public CRCore::crMethod
{// server
public:
	crJXJMailBodyQueryingMethod();
	crJXJMailBodyQueryingMethod(const crJXJMailBodyQueryingMethod& handle);
	MethodClass(JXJ, JXJMailBodyQuerying)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//float m_avail_time;
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< CRCore::ref_ptr<crMailQuery> >m_taskVec;
};
class crJXJReturnToCheckSwitchMethod:public CRCore::crMethod
{
public:
	crJXJReturnToCheckSwitchMethod();
	crJXJReturnToCheckSwitchMethod(const crJXJReturnToCheckSwitchMethod& handle);
	MethodClass(JXJ, JXJReturnToCheckSwitch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_switch;
};
class crJXJCheckMailWriteSizeMethod:public CRCore::crMethod
{
public:
	crJXJCheckMailWriteSizeMethod();
	crJXJCheckMailWriteSizeMethod(const crJXJCheckMailWriteSizeMethod& handle);
	MethodClass(JXJ, JXJCheckMailWriteSize)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	std::string m_sizeinput;
	std::string m_precontent;
};
class crJXJRecvSystemMailMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvSystemMailMethod();
	crJXJRecvSystemMailMethod(const crJXJRecvSystemMailMethod& handle);
	MethodClass(JXJ, JXJRecvSystemMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJClientRecvSystemMailMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJClientRecvSystemMailMethod();
	crJXJClientRecvSystemMailMethod(const crJXJClientRecvSystemMailMethod& handle);
	MethodClass(JXJ, JXJClientRecvSystemMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJUIDeleteOneMailMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIDeleteOneMailMethod();
	crJXJUIDeleteOneMailMethod(const crJXJUIDeleteOneMailMethod& handle);
	MethodClass(JXJ, JXJUIDeleteOneMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	//std::string m_mailchoice[5];
};
class crJXJDeleteOneMailMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDeleteOneMailMethod();
	crJXJDeleteOneMailMethod(const crJXJDeleteOneMailMethod& handle);
	MethodClass(JXJ, JXJDeleteOneMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvDeleteOneMailMethod:public CRCore::crMethod
{// client and server
public:
	crJXJRecvDeleteOneMailMethod();
	crJXJRecvDeleteOneMailMethod(const crJXJRecvDeleteOneMailMethod& handle);
	MethodClass(JXJ, JXJRecvDeleteOneMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData				*m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
	int										m_net_type;
	std::string m_tipcanvas;
	std::string m_title;
	std::string m_inputbody;
};
class crJXJDeleteMailDisposeMethod:public CRCore::crMethod
{// server
public:
	crJXJDeleteMailDisposeMethod();
	crJXJDeleteMailDisposeMethod(const crJXJDeleteMailDisposeMethod& handle);
	MethodClass(JXJ, JXJDeleteMailDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//float m_avail_time;
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< CRCore::ref_ptr<crMailQuery> >m_taskVec;
};
class crJXJResetMailCurPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetMailCurPageMethod();
	crJXJResetMailCurPageMethod(const crJXJResetMailCurPageMethod& handle);
	MethodClass(JXJ, JXJResetMailCurPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvUpdateLordLvInfoMethod:public CRCore::crMethod
{// client
public:
	crJXJRecvUpdateLordLvInfoMethod();
	crJXJRecvUpdateLordLvInfoMethod(const crJXJRecvUpdateLordLvInfoMethod& handle);
	MethodClass(JXJ, JXJRecvUpdateLordLvInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData				*m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
	int										m_net_type;
	std::string m_donghuacanvas[3];
	std::string m_donghuasequence[3];
};
class crJXJShowUIFxMethod:public CRCore::crMethod
{// client
public:
	crJXJShowUIFxMethod();
	crJXJShowUIFxMethod(const crJXJShowUIFxMethod& handle);
	MethodClass(JXJ, JXJShowUIFx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fxcanvas;
	float m_time;
	int m_lv;
};
class crJXJUIFxFrameMethod:public CRCore::crMethod
{// client
public:
	crJXJUIFxFrameMethod();
	crJXJUIFxFrameMethod(const crJXJUIFxFrameMethod& handle);
	MethodClass(JXJ, JXJUIFxFrame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float* m_dt;
	std::string m_btn;
};
class crJXJSelCurPageAllMailMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelCurPageAllMailMethod();
	crJXJSelCurPageAllMailMethod(const crJXJSelCurPageAllMailMethod& handle);
	MethodClass(JXJ, JXJSelCurPageAllMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_mailchoice[5];
	std::string m_deletebtn;
};
class crJXJCheckNewMailMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckNewMailMethod();
	crJXJCheckNewMailMethod(const crJXJCheckNewMailMethod& handle);
	MethodClass(JXJ, JXJCheckNewMail)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};	
class crJXJNewMailRequestMethod:public CRCore::crMethod
{// client and server
public:
	crJXJNewMailRequestMethod();
	crJXJNewMailRequestMethod(const crJXJNewMailRequestMethod& handle);
	MethodClass(JXJ, JXJNewMailRequest)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData				*m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
	int										m_net_type;
	std::string m_canvas;
	std::string m_mailbtn;
	std::string m_mailplay;
	std::string m_strMailCanvas;
};
class crJXJNewMailQueryingMethod:public CRCore::crMethod
{// server
public:
	crJXJNewMailQueryingMethod();
	crJXJNewMailQueryingMethod(const crJXJNewMailQueryingMethod& handle);
	MethodClass(JXJ, JXJNewMailQuerying)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_avail_time;
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJSetMakeEquipNumMaxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetMakeEquipNumMaxMethod();
	crJXJSetMakeEquipNumMaxMethod(const crJXJSetMakeEquipNumMaxMethod& handle);
	MethodClass(JXJ, JXJSetMakeEquipNumMax)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_numinput;
};
class crJXJUIFubenUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFubenUpdateMethod();
	crJXJUIFubenUpdateMethod(const crJXJUIFubenUpdateMethod& handle);
	MethodClass(JXJ, JXJUIFubenUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_modeRaido;
	std::string m_chapterRadioGroup;
	std::string m_fubenRadio;
	std::string m_hardlvRadio;
	std::string m_resetBtn;
	std::string m_fubenopensw[6];
	std::string m_chapteropensw[11];
	std::string m_formationicon[3];
	std::string m_formationinput[3];
	std::string m_fubenbackpic[6];
	std::string m_fubenrushbtn[6];
	std::string m_fubenenterbtn[6];
	std::string m_cardcolor[6];
	std::string m_cardpic[6];
	std::string m_cardsw[3];
	std::string m_fubensw[3];
	std::string m_ratesw[3];
	std::string m_pageupbtn;
	std::string m_pagedownbtn;
	std::string m_fubenpic;
	std::string m_bosspic;
	std::string m_enter;
	std::string m_sweep;
	std::string m_chapterscroll;
	std::string m_defaultpic;
	int m_chapterindex;
	int m_hardlvindex;
	int m_chapterfubensize;
	int m_chaptersize;
	int m_value;
	int m_modeindex;
	std::string m_pagesw;
	std::string m_pagesw2;
	std::string m_mingjianginput;
	std::string m_chongzhiinput;
	std::string m_mingjiangBtn;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	std::string m_strBtZuDui;
};
class crJXJUISelectFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectFubenMethod();
	crJXJUISelectFubenMethod(const crJXJUISelectFubenMethod& handle);
	MethodClass(JXJ, JXJUISelectFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJUICheckFubenNeedMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICheckFubenNeedMethod();
	crJXJUICheckFubenNeedMethod(const crJXJUICheckFubenNeedMethod& handle);
	MethodClass(JXJ, JXJUICheckFubenNeed)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_enterFuben;
	std::string m_info;
};
//class crJXJRecvvCheckFubenNeedMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvvCheckFubenNeedMethod();
//	crJXJRecvvCheckFubenNeedMethod(const crJXJRecvvCheckFubenNeedMethod& handle);
//	MethodClass(JXJ, JXJRecvvCheckFubenNeed)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_enterFuben;
//	std::string m_info;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//};
class crJXJUIDeductJunlingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIDeductJunlingMethod();
	crJXJUIDeductJunlingMethod(const crJXJUIDeductJunlingMethod& handle);
	MethodClass(JXJ, JXJUIDeductJunling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioWidgetNode* m_this;
	std::string m_enterFuben;
	std::string m_info;
};
class crJXJRecvDeductJunlingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvDeductJunlingMethod();
	crJXJRecvDeductJunlingMethod(const crJXJRecvDeductJunlingMethod& handle);
	MethodClass(JXJ, JXJRecvDeductJunling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuyJunlingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyJunlingMethod();
	crJXJUIBuyJunlingMethod(const crJXJUIBuyJunlingMethod& handle);
	MethodClass(JXJ, JXJUIBuyJunling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_copperColor;
};
class crJXJBuyJunlingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyJunlingMethod();
	crJXJBuyJunlingMethod(const crJXJBuyJunlingMethod& handle);
	MethodClass(JXJ, JXJBuyJunling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioWidgetNode* m_this;
	std::string m_enterFuben;
	std::string m_info;
};
class crJXJRecvBuyJunlingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyJunlingMethod();
	crJXJRecvBuyJunlingMethod(const crJXJRecvBuyJunlingMethod& handle);
	MethodClass(JXJ, JXJRecvBuyJunling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJCheckItemTypeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckItemTypeMethod();
	crJXJCheckItemTypeMethod(const crJXJCheckItemTypeMethod& handle);
	MethodClass(JXJ, JXJCheckItemType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_usebtn;
	std::string m_displaybtn;
	std::string m_salebtn;
	std::string m_splitbtn;
};
class crJXJFormationEditMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFormationEditMethod();
	crJXJFormationEditMethod(const crJXJFormationEditMethod& handle);
	MethodClass(JXJ, JXJFormationEdit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	std::string m_scrollbar;
	std::string m_totallead;
	std::string m_totaltroops;
	std::string m_perlead;
	std::string m_deductlead1;
	std::string m_deductlead2;
	int m_index;
};
class crJXJFormationjiangkaTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFormationjiangkaTipInfoMethod();
	crJXJFormationjiangkaTipInfoMethod(const crJXJFormationjiangkaTipInfoMethod& handle);
	MethodClass(JXJ, JXJFormationjiangkaTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_card;
	std::string m_backboard;
	std::string m_guojia;
	std::string m_shuxing;
	std::string m_bingzhong;
	std::string m_leixing;
	std::string m_jinengmiaoshu;
	std::string m_nameimage;
	std::string m_jineng;
	std::string m_cardlvsw;
	int m_row;
	int m_col;
};
//class crJXJUIUpdateTechLvMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIUpdateTechLvMethod();
//	crJXJUIUpdateTechLvMethod(const crJXJUIUpdateTechLvMethod& handle);
//	MethodClass(JXJ, JXJUIUpdateTechLv)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	int m_index;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//};
//class crJXJRecvUpdateTechLvMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvUpdateTechLvMethod();
//	crJXJRecvUpdateTechLvMethod(const crJXJRecvUpdateTechLvMethod& handle);
//	MethodClass(JXJ, JXJRecvUpdateTechLv)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//};
//class crJXJUpdateTechCanvasMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUpdateTechCanvasMethod();
//	crJXJUpdateTechCanvasMethod(const crJXJUpdateTechCanvasMethod& handle);
//	MethodClass(JXJ, JXJUpdateTechCanvas)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_techwidget[16];
//	std::string m_techupgradebtn[16];
//	std::string m_techlvinput[16];
//	std::string m_achievementinput;
//};
class crJXJUITaskListUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUITaskListUpdateMethod();
	crJXJUITaskListUpdateMethod(const crJXJUITaskListUpdateMethod& handle);
	MethodClass(JXJ, JXJUITaskListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void displayTaskName(bool expand,std::string title,CRNetApp::crGameTask *task,bool &titleresult,int &i,unsigned char mode,int &count);
	CRCore::crCanvasNode *m_this;
	std::string m_foldbtn[11];
	std::string m_taskinput[11];
	std::string m_taskradio;
	std::string m_taskinfo;
	std::string m_taskgoal;
	std::string m_taskreward;
	std::string m_rewardicon[4];
	std::string m_getrewardbtn;
	CRCore::ref_ptr<CRUI::crCheckBoxWidgetNode> m_foldbtnWidget[11];
	CRCore::ref_ptr<CRUI::crStaticTextWidgetNode> m_taskinputWidget[11];
	CRCore::crVector4f m_titleColor;
	CRCore::crVector4f m_taskColor;
};
class crJXJUIChooseOneTaskMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIChooseOneTaskMethod();
	crJXJUIChooseOneTaskMethod(const crJXJUIChooseOneTaskMethod& handle);
	MethodClass(JXJ, JXJUIChooseOneTask)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_foldbtn[11];
	std::string m_taskinput[11];
	int m_index;
};
class crJXJGetTechResProductPercentMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetTechResProductPercentMethod();
	crJXJGetTechResProductPercentMethod(const crJXJGetTechResProductPercentMethod& handle);
	MethodClass(JXJ, JXJGetTechResProductPercent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
	unsigned char m_thingtype;
};
class crJXJUIAppointJangkaListUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAppointJangkaListUpdateMethod();
	crJXJUIAppointJangkaListUpdateMethod(const crJXJUIAppointJangkaListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIAppointJangkaListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_defaulticon;
	std::string m_lineoneswitch;
	std::string m_linetwoswitch;
	std::string m_linethreeswitch;
	std::string m_appointcardicon1;
	std::string m_appointcardicon2;
	std::string m_appointcardicon3;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_defaulticon2;
	//std::string m_whiteframe;
	//std::string m_greenframe;
	//std::string m_blueframe;
	//std::string m_purpleframe;
	//std::string m_orangeframe;
	std::string m_infoinput[3];
	std::string m_colorswitch[3];
	std::string m_cardtitle[3];
	std::string m_frame[10];
	std::string m_curPageNum;
	std::string m_buyswitch[3];
	std::string m_buyBtn[3];
};
class crJXJUIAppointOneJiangkaMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAppointOneJiangkaMethod();
	crJXJUIAppointOneJiangkaMethod(const crJXJUIAppointOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJUIAppointOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_namecolor;
	std::string m_image[3];
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJAppointCardValidTimeMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJAppointCardValidTimeMethod();
	crJXJAppointCardValidTimeMethod(const crJXJAppointCardValidTimeMethod& handle);
	MethodClass(JXJ, JXJAppointCardValidTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_totaltime;
};
class crJXJRecvAppointValidTimeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvAppointValidTimeMethod();
	crJXJRecvAppointValidTimeMethod(const crJXJRecvAppointValidTimeMethod& handle);
	MethodClass(JXJ, JXJRecvAppointValidTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUpdateAppointCardValidTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateAppointCardValidTimeMethod();
	crJXJUpdateAppointCardValidTimeMethod(const crJXJUpdateAppointCardValidTimeMethod& handle);
	MethodClass(JXJ, JXJUpdateAppointCardValidTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_timelimitinput[3];
	int m_oldvalidtime[3];
	int m_newvalidtime[3];
	std::string m_curPageNum;
};
class crJXJAppointOneJiangkaMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAppointOneJiangkaMethod();
	crJXJAppointOneJiangkaMethod(const crJXJAppointOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJAppointOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvAppointOneJiangkaMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAppointOneJiangkaMethod();
	crJXJRecvAppointOneJiangkaMethod(const crJXJRecvAppointOneJiangkaMethod& handle);
	MethodClass(JXJ, JXJRecvAppointOneJiangka)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
//class crUIShowTaskMethod : public CRCore::crMethod
//{//Client UI Method
//public:
//	crUIShowTaskMethod();
//	crUIShowTaskMethod(const crUIShowTaskMethod& handle);
//	MethodClass(CRNetApp, UIShowTask)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	int m_taskid;
//	std::string* m_canvas;
//	std::string m_frameStaticText;
//	std::string m_desTextHypertext;
//	std::string m_progressHypertext;
//	std::string m_item1ImageBox;
//	std::string m_item2ImageBox;
//	std::string m_item3ImageBox;
//	std::string m_item4ImageBox;
//	std::string m_expStaticText;
//	std::string m_exp2StaticText;
//	std::string m_goldStaticText;
//	int m_waitTime;
//};
//class crUIAwardItemTipInfoMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crUIAwardItemTipInfoMethod();
//	crUIAwardItemTipInfoMethod(const crUIAwardItemTipInfoMethod& handle);
//	MethodClass(CRNetApp, UIAwardItemTipInfo)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	char m_id;
//	std::string m_tipCanvasName;
//	std::string m_tipWidgetName;
//	CRProducer::crGUIEventAdapter* m_ea;
//};
//class crUIShowTask2Method : public CRCore::crMethod
//{//Client UI Method
//public:
//	crUIShowTask2Method();
//	crUIShowTask2Method(const crUIShowTask2Method& handle);
//	MethodClass(CRNetApp, UIShowTask2)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CREncapsulation::crListBoxWidgetNode> m_this;
//	int m_taskid;
//	std::string m_canvas;
//};
//class crUIUpdateTaskTraceMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crUIUpdateTaskTraceMethod();
//	crUIUpdateTaskTraceMethod(const crUIUpdateTaskTraceMethod& handle);
//	MethodClass(CRNetApp, UIUpdateTaskTrace)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode* m_this;
//	std::string m_traceWidget;
//};
class crJXJServerActiveTaskMethod :public CRCore::crMethod
{//GameServer Method服务器自动激活任务，每次玩家刚登录领地的时候，或者玩家等级升级的时候检查一次
public:
	crJXJServerActiveTaskMethod();
	crJXJServerActiveTaskMethod(const crJXJServerActiveTaskMethod& handle);
	MethodClass(JXJ, JXJServerActiveTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
};
class crJXJUIUndoAppointCardMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUndoAppointCardMethod();
	crJXJUIUndoAppointCardMethod(const crJXJUIUndoAppointCardMethod& handle);
	MethodClass(JXJ, JXJUIUndoAppointCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_namecolor;
	int m_index;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
	std::string m_appointRadio2;
};
class crJXJUndoAppointCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUndoAppointCardMethod();
	crJXJUndoAppointCardMethod(const crJXJUndoAppointCardMethod& handle);
	MethodClass(JXJ, JXJUndoAppointCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_index;
	std::string m_appointRadio2;
};
class crJXJRecvUndoAppointCardMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvUndoAppointCardMethod();
	crJXJRecvUndoAppointCardMethod(const crJXJRecvUndoAppointCardMethod& handle);
	MethodClass(JXJ, JXJRecvUndoAppointCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
//class crJXJShowNewPlayerTaskCanvasMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJShowNewPlayerTaskCanvasMethod();
//	crJXJShowNewPlayerTaskCanvasMethod(const crJXJShowNewPlayerTaskCanvasMethod& handle);
//	MethodClass(JXJ, JXJShowNewPlayerTaskCanvas)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	int m_index;
//	std::string m_canvas;
//	std::string m_input;
//};
//class crJXJUIRollCardsMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIRollCardsMethod();
//	crJXJUIRollCardsMethod(const crJXJUIRollCardsMethod& handle);
//	MethodClass(JXJ, JXJUIRollCards)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_msgbox;
//	std::string m_title;
//	std::string m_msg;
//	std::string m_ok;
//	CRCore::crVector4 m_namecolor;
//	std::string m_tavernRaido;
//	std::string m_input;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//	std::string m_page1;
//};
//class crJXJRollCardsMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRollCardsMethod();
//	crJXJRollCardsMethod(const crJXJRollCardsMethod& handle);
//	MethodClass(JXJ, JXJRollCards)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_tavernRaido;
//	std::string m_page1;
//};
//class crJXJRecvRollCardsMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvRollCardsMethod();
//	crJXJRecvRollCardsMethod(const crJXJRecvRollCardsMethod& handle);
//	MethodClass(JXJ, JXJRecvRollCards)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_jiusicanvas;
//	std::string m_cardswitch;
//	std::string m_promptinput;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//	std::string m_page1;
//};
//class crJXJUIPickCardsMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIPickCardsMethod();
//	crJXJUIPickCardsMethod(const crJXJUIPickCardsMethod& handle);
//	MethodClass(JXJ, JXJUIPickCards)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	unsigned char m_index;
//	std::string m_cardbtn[3];
//};
//class crJXJRecvPickCardsMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvPickCardsMethod();
//	crJXJRecvPickCardsMethod(const crJXJRecvPickCardsMethod& handle);
//	MethodClass(JXJ, JXJRecvPickCards)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_jiusicanvas;
//	std::string m_cardswitch;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//	std::string m_pickswitch;
//};
//class crJXJLuckyCardTimerMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJLuckyCardTimerMethod();
//	crJXJLuckyCardTimerMethod(const crJXJLuckyCardTimerMethod& handle);
//	MethodClass(JXJ, JXJLuckyCardTimer)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_switch;
//	std::string m_tavernradio;
//	std::string m_playswitch;
//	std::string m_btn[3];
//	std::string m_zhitianming;
//	std::string m_seq[4];
//	std::string m_startbtn;
//	std::string m_pickswitch;
//	std::string m_page1;
//	int m_refreshtime;
//	int m_pushtime;
//};
class crJXJUIKnowLuckyCardsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnowLuckyCardsMethod();
	crJXJUIKnowLuckyCardsMethod(const crJXJUIKnowLuckyCardsMethod& handle);
	MethodClass(JXJ, JXJUIKnowLuckyCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_namecolor;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJKnowLuckyCardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJKnowLuckyCardsMethod();
	crJXJKnowLuckyCardsMethod(const crJXJKnowLuckyCardsMethod& handle);
	MethodClass(JXJ, JXJKnowLuckyCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tavernRaido;
};
class crJXJRecvKnowCardsMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvKnowCardsMethod();
	crJXJRecvKnowCardsMethod(const crJXJRecvKnowCardsMethod& handle);
	MethodClass(JXJ, JXJRecvKnowCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_jiusicanvas;
	std::string m_knowcardbtn;
	std::string m_promptinput;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJUIKnowCardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIKnowCardsMethod();
	crJXJUIKnowCardsMethod(const crJXJUIKnowCardsMethod& handle);
	MethodClass(JXJ, JXJUIKnowCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_switch;
	std::string m_btn[3];
};
class crJXJShowLuckyCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowLuckyCardMethod();
	crJXJShowLuckyCardMethod(const crJXJShowLuckyCardMethod& handle);
	MethodClass(JXJ, JXJShowLuckyCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_switch[3];
	std::string m_cardpic[3];
	std::string m_cardinfo[3];
	std::string m_textinfo[3];
	std::string m_pickbtn[3];
};
class crJXJRecvSaleStateChangeMethod :public CRCore::crMethod
{//Client
public:
	crJXJRecvSaleStateChangeMethod();
	crJXJRecvSaleStateChangeMethod(const crJXJRecvSaleStateChangeMethod& handle);
	MethodClass(JXJ, JXJRecvSaleStateChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJBeautyDialogueInitMethod :public CRCore::crMethod
{//Client
public:
	crJXJBeautyDialogueInitMethod();
	crJXJBeautyDialogueInitMethod(const crJXJBeautyDialogueInitMethod& handle);
	MethodClass(JXJ, JXJBeautyDialogueInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::crDataObject *m_this2;
	bool m_thisIsWidget;
	std::string m_beautydialogue;
	int m_taskid;
};
class crJXJBeautyDialogueUpdateMethod :public CRCore::crMethod
{//Client
public:
	crJXJBeautyDialogueUpdateMethod();
	crJXJBeautyDialogueUpdateMethod(const crJXJBeautyDialogueUpdateMethod& handle);
	MethodClass(JXJ, JXJBeautyDialogueUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
	std::string m_newhandcanvas;
	bool m_next;
};
//class crJXJJiusiCostCountTimerMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJJiusiCostCountTimerMethod();
//	crJXJJiusiCostCountTimerMethod(const crJXJJiusiCostCountTimerMethod& handle);
//	MethodClass(JXJ, JXJJiusiCostCountTimer)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	int m_dt;
//	int m_sec;
//};
//class crJXJRecvJiusiCostCountRefreshMethod :public CRCore::crMethod
//{//Client
//public:
//	crJXJRecvJiusiCostCountRefreshMethod();
//	crJXJRecvJiusiCostCountRefreshMethod(const crJXJRecvJiusiCostCountRefreshMethod& handle);
//	MethodClass(JXJ, JXJRecvJiusiCostCountRefresh)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	std::string m_input;
//	std::string m_newhandcanvas;
//};
class crJXJRenWuCanvasFoldMethod :public CRCore::crMethod
{//Client
public:
	crJXJRenWuCanvasFoldMethod();
	crJXJRenWuCanvasFoldMethod(const crJXJRenWuCanvasFoldMethod& handle);
	MethodClass(JXJ, JXJRenWuCanvasFold)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJSetGameCardBuffTimeMethod :public CRCore::crMethod
{//Client
public:
	crJXJSetGameCardBuffTimeMethod();
	crJXJSetGameCardBuffTimeMethod(const crJXJSetGameCardBuffTimeMethod& handle);
	MethodClass(JXJ, JXJSetGameCardBuffTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_node[9];
	std::string m_fx[9];
	std::vector<float> m_percentVec;
};
class crJXJResetCardBagPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetCardBagPageMethod();
	crJXJResetCardBagPageMethod(const crJXJResetCardBagPageMethod& handle);
	MethodClass(JXJ, JXJResetCardBagPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//class crJXJCheckLordEquipSuitMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCheckLordEquipSuitMethod();
//	crJXJCheckLordEquipSuitMethod(const crJXJCheckLordEquipSuitMethod& handle);
//	MethodClass(JXJ, JXJCheckLordEquipSuit)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRole* m_this;
//};
class crJXJSetWidgetEnableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetWidgetEnableMethod();
	crJXJSetWidgetEnableMethod(const crJXJSetWidgetEnableMethod& handle);
	MethodClass(JXJ, JXJSetWidgetEnable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_button;
	bool m_enable;
};
class crJXJSetWidgetVisiableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetWidgetVisiableMethod();
	crJXJSetWidgetVisiableMethod(const crJXJSetWidgetVisiableMethod& handle);
	MethodClass(JXJ, JXJSetWidgetVisiable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_button;
	bool m_visiable;
};
class crJXJSetShowParamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetShowParamMethod();
	crJXJSetShowParamMethod(const crJXJSetShowParamMethod& handle);
	MethodClass(JXJ, JXJSetShowParam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_widget[6];
	unsigned int m_index;
};
class crJXJUISetRadioSelectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetRadioSelectMethod();
	crJXJUISetRadioSelectMethod(const crJXJUISetRadioSelectMethod& handle);
	MethodClass(JXJ, JXJUISetRadioSelect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_radiogroup;
	unsigned int m_index;
	std::string m_radiogroup_2;
};
class crJXJUISetActiveSwitchSetMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetActiveSwitchSetMethod();
	crJXJUISetActiveSwitchSetMethod(const crJXJUISetActiveSwitchSetMethod& handle);
	MethodClass(JXJ, JXJUISetActiveSwitchSet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_sw;
	int m_value;
	std::string m_sw2;
	int m_value2;
};
class crJXJShowSetImageParamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowSetImageParamMethod();
	crJXJShowSetImageParamMethod(const crJXJShowSetImageParamMethod& handle);
	MethodClass(JXJ, JXJShowSetImageParam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	int getTextureDetail();
	int getMaxAnisotropy();
	int getShadowDetail();
	int getNumMultiSamples();
	int getHDR();
	bool getEnableAlphaShadow();
	int getHideModel();
	int getAutoBattleSetting();
	int getFPSControl();
	float getUIScaleFactor();
protected:
	rcfg::ConfigScript m_cfg_script;
	CRCore::crCanvasNode *m_this;
	std::string m_widget[6];
	std::string m_gamelist[6];
	std::string m_paraminput[6];
	std::string m_combobox[6];
	std::string m_checkbox[3];
	std::string m_brightnessbar;
	std::string m_radio;
	std::string m_fullscreencheck;
	std::string m_windowMaxsizeCheck;
	std::string m_hideModelCheck;
	std::string m_autoBattleSettingCheck;
	std::string m_uiscaleSlider;
	//std::string m_uiscaleText;
};
class crJXJUpdateInitCfgMethod : public CRCore::crMethod
{//ButtonClick Callback
public:
	crJXJUpdateInitCfgMethod();
	crJXJUpdateInitCfgMethod(const crJXJUpdateInitCfgMethod& handle);
	MethodClass(CREncapsulation, JXJUpdateInitCfg)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_resolutionCombo;//分辨率
	std::string m_brightnessSlider;//亮度
	std::string m_textureDetailCombo;//贴图精度
	std::string m_textureFilterCombo;//贴图过滤
	std::string m_shadowDetailCombo;//阴影
	std::string m_alphaShadowCombo;//阴影
	std::string m_antialiasingCombo;//抗锯齿
	std::string m_HDRCombo; //HDR
	std::string m_FPSCombo; //帧率控制
	std::string m_motionBlurCheck;//运动模糊
	std::string m_depthOfFieldCheck;//景深模糊
	std::string m_alphaShadowCheck;//阴影半透
	std::string m_fullScreenCheck;//全屏
	std::string m_windowMaxsizeCheck;//窗口模式最大化
	std::string m_hideModelCheck;//战场屏蔽NPC和玩家模型
	std::string m_autoBattleSettingCheck;//战场自动低配
	std::string m_uiscaleSlider;
	//typedef std::vector<std::string> ExitString;
	//ExitString m_exitStrings;
};
class crJXJUISystemShowUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISystemShowUpdateMethod();
	crJXJUISystemShowUpdateMethod(const crJXJUISystemShowUpdateMethod& handle);
	MethodClass(JXJ, JXJUISystemShowUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_radiogroup;
	std::string m_brightnessbar;
	std::string m_combobox[6];
	std::string m_checkbox[3];
};
class crJXJNewhandSetfubenidMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewhandSetfubenidMethod();
	crJXJNewhandSetfubenidMethod(const crJXJNewhandSetfubenidMethod& handle);
	MethodClass(JXJ, JXJNewhandSetfubenid)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_taskid;
	//std::string m_taskcanvas;
};
class crCheckRegisterEditBoxMethod :public CRCore::crMethod
{//Client Method
public:
	crCheckRegisterEditBoxMethod();
	crCheckRegisterEditBoxMethod(const crCheckRegisterEditBoxMethod& handle);
	MethodClass(JXJ, CheckRegisterEditBox)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	std::string m_checksw[4];
	std::string m_resbtn;
	std::string m_password;
	std::string m_password2;
	int m_index;///0:account,1:Password1,2:Password2,3:Email
	//std::string m_taskcanvas;
};
class crCheckRegisterBtnMethod :public CRCore::crMethod
{//Client Method
public:
	crCheckRegisterBtnMethod();
	crCheckRegisterBtnMethod(const crCheckRegisterBtnMethod& handle);
	MethodClass(JXJ, CheckRegisterBtn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_checksw[4];
	std::string m_resbtn;
};
class crJXJSetRegisterAccountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetRegisterAccountMethod();
	crJXJSetRegisterAccountMethod(const crJXJSetRegisterAccountMethod& handle);
	MethodClass(JXJ, JXJSetRegisterAccount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_registeraccount;
	std::string m_logincanvas;
	std::string m_loginaccount;
};
class crJXJRecvUpdateFriendDataMethod :public CRCore::crMethod
{//Client
public:
	crJXJRecvUpdateFriendDataMethod();
	crJXJRecvUpdateFriendDataMethod(const crJXJRecvUpdateFriendDataMethod& handle);
	MethodClass(JXJ, JXJRecvUpdateFriendData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJShowCityMimiMenuMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowCityMimiMenuMethod();
	crJXJShowCityMimiMenuMethod(const crJXJShowCityMimiMenuMethod& handle);
	MethodClass(JXJ, JXJShowCityMimiMenu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_citymenu;
	std::string m_citylistcanvas;
	std::string m_entercanvas;
	std::string m_nameinput;
	std::string m_strPingPan;
	std::string m_strEnter;
};
class crJXJBattleRadioAddCityNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleRadioAddCityNameMethod();
	crJXJBattleRadioAddCityNameMethod(const crJXJBattleRadioAddCityNameMethod& handle);
	MethodClass(JXJ, JXJBattleRadioAddCityName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_cityradio;
	std::string m_colormaskgroup;
	CRCore::crVector3f m_scale;
};
class crJXJGameShowFormationInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGameShowFormationInfoMethod();
	crJXJGameShowFormationInfoMethod(const crJXJGameShowFormationInfoMethod& handle);
	MethodClass(JXJ, JXJGameShowFormationInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationsw;
	std::string m_icon[3];
	std::string m_input[3];
	std::string m_progress[3];
};
class crJXJBattleListCtrlScrollMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBattleListCtrlScrollMethod();
	crJXJBattleListCtrlScrollMethod(const crJXJBattleListCtrlScrollMethod& handle);
	MethodClass(JXJ, JXJBattleListCtrlScroll)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crCanvasNode *m_this;
	std::string m_listcontrol1;
	std::string m_listcontrol2;
	unsigned char m_type;
};
class crJXJChangeChatTextSizeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeChatTextSizeMethod();
	crJXJChangeChatTextSizeMethod(const crJXJChangeChatTextSizeMethod& handle);
	MethodClass(JXJ, JXJChangeChatTextSize)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_unfoldbtn;
	std::string m_packupbtn;
	std::string m_sizesw;
	int m_index;
	int m_nowsize;
};
class crJXJCheckPlayerGameNameNULLMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckPlayerGameNameNULLMethod();
	crJXJCheckPlayerGameNameNULLMethod(const crJXJCheckPlayerGameNameNULLMethod& handle);
	MethodClass(JXJ, JXJCheckPlayerGameNameNULL)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_nameinput;
};
class crJXJDeductLijinGoldMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJDeductLijinGoldMethod();
	crJXJDeductLijinGoldMethod(const crJXJDeductLijinGoldMethod& handle);
	MethodClass(JXJ, JXJDeductLijinGold)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::MoneyChangeData* m_moneydata;//输入：m_moneydata->first<0表示只扣除元宝，>0表示优先扣除礼金 输出：m_moneydata->first=0表示操作成功，-1表示失败
	bool* m_output;//返回true表示扣除了元宝，false表示为只扣除了礼金
};
class crJXJCheckLijinGoldMethod :public CRCore::crMethod
{//GameClient & GameServer Method
public:
	crJXJCheckLijinGoldMethod();
	crJXJCheckLijinGoldMethod(const crJXJCheckLijinGoldMethod& handle);
	MethodClass(JXJ, JXJCheckLijinGold)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_needdeduct;
};
class crJXJRecvDeductLijinGoldMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvDeductLijinGoldMethod();
	crJXJRecvDeductLijinGoldMethod(const crJXJRecvDeductLijinGoldMethod& handle);
	MethodClass(JXJ, JXJRecvDeductLijinGold)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJOnBattleUIMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOnBattleUIMethod();
	crJXJOnBattleUIMethod(const crJXJOnBattleUIMethod& handle);
	MethodClass(JXJ, JXJOnBattleUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvOnBattleUIMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOnBattleUIMethod();
	crJXJRecvOnBattleUIMethod(const crJXJRecvOnBattleUIMethod& handle);
	MethodClass(JXJ, JXJRecvOnBattleUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_battlecanvas;
};
class crJXJCityInBattleInfoSendDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCityInBattleInfoSendDisposeMethod();
	crJXJCityInBattleInfoSendDisposeMethod(const crJXJCityInBattleInfoSendDisposeMethod& handle);
	MethodClass(JXJ, JXJCityInBattleInfoSendDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJRecvCityInBattleInfoMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvCityInBattleInfoMethod();
	crJXJRecvCityInBattleInfoMethod(const crJXJRecvCityInBattleInfoMethod& handle);
	MethodClass(JXJ, JXJRecvCityInBattleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_battlecanvas;
};
class crJXJLeaveBattleUIMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeaveBattleUIMethod();
	crJXJLeaveBattleUIMethod(const crJXJLeaveBattleUIMethod& handle);
	MethodClass(JXJ, JXJLeaveBattleUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvLeaveBattleUIMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvLeaveBattleUIMethod();
	crJXJRecvLeaveBattleUIMethod(const crJXJRecvLeaveBattleUIMethod& handle);
	MethodClass(JXJ, JXJRecvLeaveBattleUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRememberMyAccountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRememberMyAccountMethod();
	crJXJRememberMyAccountMethod(const crJXJRememberMyAccountMethod& handle);
	MethodClass(JXJ, JXJRememberMyAccount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox;
	std::string m_account;
	std::string m_password;
};
class crSetRememberAccountMethod :public CRCore::crMethod
{//Client Method
public:
	crSetRememberAccountMethod();
	crSetRememberAccountMethod(const crSetRememberAccountMethod& handle);
	MethodClass(JXJ, SetRememberAccount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_checkbox;
	std::string m_account;
};
class crJXJUILordEquipTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUILordEquipTipInfoMethod();
	crJXJUILordEquipTipInfoMethod(const crJXJUILordEquipTipInfoMethod& handle);
	MethodClass(JXJ, JXJUILordEquipTipInfo)
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
	std::string m_govshopcanvas;
	int m_imageindex;
	int m_index;
	int m_mailIndex;
	std::string m_mailCanvas;
	std::string m_GongfangCanvas;
	std::string m_GongfangRadio;
	int m_onlineRewardIndex;
	std::string m_workshopRadio[2];
};
class crJXJTakeOffLordEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTakeOffLordEquipMethod();
	crJXJTakeOffLordEquipMethod(const crJXJTakeOffLordEquipMethod& handle);
	MethodClass(JXJ, JXJTakeOffLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_index;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
	std::string m_name;
};
class crJXJRecvTakeOffLordEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvTakeOffLordEquipMethod();
	crJXJRecvTakeOffLordEquipMethod(const crJXJRecvTakeOffLordEquipMethod& handle);
	MethodClass(JXJ, JXJRecvTakeOffLordEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
};
class crJXJOnChengfangUIMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOnChengfangUIMethod();
	crJXJOnChengfangUIMethod(const crJXJOnChengfangUIMethod& handle);
	MethodClass(JXJ, JXJOnChengfangUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvOnChengfangUIMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOnChengfangUIMethod();
	crJXJRecvOnChengfangUIMethod(const crJXJRecvOnChengfangUIMethod& handle);
	MethodClass(JXJ, JXJRecvOnChengfangUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJChengfangInfoSendDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJChengfangInfoSendDisposeMethod();
	crJXJChengfangInfoSendDisposeMethod(const crJXJChengfangInfoSendDisposeMethod& handle);
	MethodClass(JXJ, JXJChengfangInfoSendDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJRecvChengfangInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvChengfangInfoMethod();
	crJXJRecvChengfangInfoMethod(const crJXJRecvChengfangInfoMethod& handle);
	MethodClass(JXJ, JXJRecvChengfangInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_battlecanvas;
};
class crJXJLeaveChengfangUIMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeaveChengfangUIMethod();
	crJXJLeaveChengfangUIMethod(const crJXJLeaveChengfangUIMethod& handle);
	MethodClass(JXJ, JXJLeaveChengfangUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvLeaveChengfangUIMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvLeaveChengfangUIMethod();
	crJXJRecvLeaveChengfangUIMethod(const crJXJRecvLeaveChengfangUIMethod& handle);
	MethodClass(JXJ, JXJRecvLeaveChengfangUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJBattleLineUpCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleLineUpCanvasUpdateMethod();
	crJXJBattleLineUpCanvasUpdateMethod(const crJXJBattleLineUpCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJBattleLineUpCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_nameinput;
	std::string m_gongtotal;
	std::string m_shoutotal;
	std::string m_chengfanginput;
	std::string m_playerinput[12];
	std::string m_battlenameinput[12];
	std::string m_battleinfo[12];
	std::string m_lineupbtn;
	std::string m_battlingsw[12];
	std::string m_battlewinsw[12];
	std::string m_shilichengfangsw[3];
	std::string m_shilichengfanginput[3];
	std::string m_lineupsw;
	std::string m_defendshilisw;
	std::string m_defendshu1;
	std::string m_defendwu1;
	std::string m_defendwei1;
	std::string m_defendshu2;
};
class crJXJBattleTimerUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleTimerUpdateMethod();
	crJXJBattleTimerUpdateMethod(const crJXJBattleTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJBattleTimerUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_battleTimer;
	std::string m_battlecanvas;
	int m_mintime;
};
class crJXJCloseAllSelectCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCloseAllSelectCanvasMethod();
	crJXJCloseAllSelectCanvasMethod(const crJXJCloseAllSelectCanvasMethod& handle);
	MethodClass(JXJ, JXJCloseAllSelectCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::vector<std::string> m_canvasVec;
};
class crJXJBattleCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleCanvasUpdateMethod();
	crJXJBattleCanvasUpdateMethod(const crJXJBattleCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJBattleCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas[2];
};
class crJXJUIClearPunishTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIClearPunishTimeMethod();
	crJXJUIClearPunishTimeMethod(const crJXJUIClearPunishTimeMethod& handle);
	MethodClass(JXJ, JXJUIClearPunishTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_tipcanvas;
	std::string m_tipcontent;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
};
class crJXJClearPunishTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClearPunishTimeMethod();
	crJXJClearPunishTimeMethod(const crJXJClearPunishTimeMethod& handle);
	MethodClass(JXJ, JXJClearPunishTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvClearPunishTimeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvClearPunishTimeMethod();
	crJXJRecvClearPunishTimeMethod(const crJXJRecvClearPunishTimeMethod& handle);
	MethodClass(JXJ, JXJRecvClearPunishTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tipcontent;
	std::string m_punishcanvas;
	int m_delayHideWidgetTime;//延迟自动隐藏控件时间
	std::string m_manorTaskBar_cavas;//任务栏窗口 用于调整tipcanvas位置
};
class crJXJUIGameDeleteFriendMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGameDeleteFriendMethod();
	crJXJUIGameDeleteFriendMethod(const crJXJUIGameDeleteFriendMethod& handle);
	MethodClass(JXJ, JXJUIGameDeleteFriend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	std::string m_friendlist;
	int m_index;
	std::string m_groupcanvas;
};
class crJXJBattleSetMultiSwitchActiveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleSetMultiSwitchActiveMethod();
	crJXJBattleSetMultiSwitchActiveMethod(const crJXJBattleSetMultiSwitchActiveMethod& handle);
	MethodClass(JXJ, JXJBattleSetMultiSwitchActive)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_switch;
	std::string m_zoom;
	std::string m_enlarge;
};
class crJXJUISystemUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISystemUpdateMethod();
	crJXJUISystemUpdateMethod(const crJXJUISystemUpdateMethod& handle);
	MethodClass(JXJ, JXJUISystemUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_checkbox;
	std::string m_bkscroll;
	std::string m_efxscroll;
};
class crJXJUISystemOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISystemOnShowMethod();
	crJXJUISystemOnShowMethod(const crJXJUISystemOnShowMethod& handle);
	MethodClass(JXJ, JXJUISystemOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_checkbox;
	std::string m_bkscroll;
	std::string m_efxscroll;
	std::string m_strCanvas;
};
class crJXJSetBattleNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetBattleNameMethod();
	crJXJSetBattleNameMethod(const crJXJSetBattleNameMethod& handle);
	MethodClass(JXJ, JXJSetBattleName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasname;
	std::string m_battlename;
};
class crJXJUIPunishtimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPunishtimerMethod();
	crJXJUIPunishtimerMethod(const crJXJUIPunishtimerMethod& handle);
	MethodClass(JXJ, JXJUIPunishtimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_inputtime;
	int m_serverpunishtime;
	int m_lastpunishtime;
};
class crJXJUIReleaseRecruitInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIReleaseRecruitInfoMethod();
	crJXJUIReleaseRecruitInfoMethod(const crJXJUIReleaseRecruitInfoMethod& handle);
	MethodClass(JXJ, JXJUIReleaseRecruitInfo)
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
};
class crJXJGetFukuLimitAddMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetFukuLimitAddMethod();
	crJXJGetFukuLimitAddMethod(const crJXJGetFukuLimitAddMethod& handle);
	MethodClass(JXJ, JXJGetFukuLimitAdd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
};
class crJXJUIUndoAddJiangKaDragMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUndoAddJiangKaDragMethod();
	crJXJUIUndoAddJiangKaDragMethod(const crJXJUIUndoAddJiangKaDragMethod& handle);
	MethodClass(JXJ, JXJUIUndoAddJiangKaDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_cardcanvas;
	std::string m_btn[6];
	std::string m_colorbox[6];
	std::string m_tipcanvas;
	std::string m_tipcontent;
	std::string m_jiicon;
	std::string m_mouicon;
};
class crJXJWidgetDoEventMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWidgetDoEventMethod();
	crJXJWidgetDoEventMethod(const crJXJWidgetDoEventMethod& handle);
	MethodClass(JXJ, JXJWidgetDoEvent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crWidgetNode *m_this;
	std::string m_canvasname;
	std::string m_widgetname;
	int m_buttonindex;
	int m_eventindex;
};
class crJXJCheckLordLevelMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckLordLevelMethod();
	crJXJCheckLordLevelMethod(const crJXJCheckLordLevelMethod& handle);
	MethodClass(JXJ, JXJCheckLordLevel)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_index;
	std::string m_tips;
	std::string m_tipstext;
};
class crJXJShowVersionInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowVersionInfoMethod();
	crJXJShowVersionInfoMethod(const crJXJShowVersionInfoMethod& handle);
	MethodClass(JXJ, JXJShowVersionInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_hypertext;
	std::string m_filename;
};
class crJXJFubenRushReturnTimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushReturnTimerMethod();
	crJXJFubenRushReturnTimerMethod(const crJXJFubenRushReturnTimerMethod& handle);
	MethodClass(JXJ, JXJFubenRushReturnTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_inputtime;
	std::string m_rushcanvas;
	std::string m_fubencanvas;
	std::string m_sequence;
	int m_lastpunishtime;
};
class crJXJFubenRushEditCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushEditCheckMethod();
	crJXJFubenRushEditCheckMethod(const crJXJFubenRushEditCheckMethod& handle);
	MethodClass(JXJ, JXJFubenRushEditCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	std::string m_inputcount;
	std::string m_rushradio;
};
class crJXJFubenRushOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushOnShowMethod();
	crJXJFubenRushOnShowMethod(const crJXJFubenRushOnShowMethod& handle);
	MethodClass(JXJ, JXJFubenRushOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_fubeninput;   //副本名称
	std::string m_rushradio;    //默认0
	std::string m_rushcountedit;   //输入扫荡次数
	std::string m_junlingcountinput;	//计算总军令	
	std::string m_startrushbtn;	//开始扫荡按钮	
	std::string m_junlingtips;	//军令不足弹窗	
	std::string m_checkkbox[7];
	std::string m_strText[7];
	std::string m_scrollbar;
	std::string m_hyperText;
};
class crJXJFubenRushUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushUpdateMethod();
	crJXJFubenRushUpdateMethod(const crJXJFubenRushUpdateMethod& handle);
	MethodClass(JXJ, JXJFubenRushUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_rushradio;    //默认0
	std::string m_rushcountedit;   //输入扫荡次数
	std::string m_junlingcountinput;	//计算总军令	
	std::string m_startrushbtn;	//开始扫荡按钮	
	std::string m_junlingtips;	//军令不足弹窗			
};
class crJXJFubenRushStartMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushStartMethod();
	crJXJFubenRushStartMethod(const crJXJFubenRushStartMethod& handle);
	MethodClass(JXJ, JXJFubenRushStart)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_leftswitch;    
	std::string m_rightswitch;   
	std::string m_rushradio;    
	std::string m_rushcountedit;   //输入扫荡次数	
	std::string m_hyperText;
	std::string m_cfg;
	std::string m_table;
	std::string m_itemCheckBox[7];
	std::string m_inputStr[7];
	std::string m_check;
	std::string m_check_sw;
};
class crJXJRecvFubenRushStartMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFubenRushStartMethod();
	crJXJRecvFubenRushStartMethod(const crJXJRecvFubenRushStartMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushStart)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas; 
	std::string m_leftswitch;    
	std::string m_rightswitch;  
	std::string m_numbersw1[3];
	std::string m_numbersw2[3];
	std::string m_tipcanvas;
	std::string m_tipcontent;
};
class crJXJFubenRushPauseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushPauseMethod();
	crJXJFubenRushPauseMethod(const crJXJFubenRushPauseMethod& handle);
	MethodClass(JXJ, JXJFubenRushPause)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_leftswitch; 
	std::string m_rightswitch; 	
};
class crJXJRecvFubenRushPauseMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFubenRushPauseMethod();
	crJXJRecvFubenRushPauseMethod(const crJXJRecvFubenRushPauseMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushPause)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas; 
	std::string m_leftswitch;    
	std::string m_rightswitch;  
	std::string m_tipcanvas;
	std::string m_tipcontent;
};
class crJXJFubenRushContinueMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushContinueMethod();
	crJXJFubenRushContinueMethod(const crJXJFubenRushContinueMethod& handle);
	MethodClass(JXJ, JXJFubenRushContinue)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_leftswitch;    
	std::string m_rightswitch; 
};
class crJXJRecvFubenRushContinueMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFubenRushContinueMethod();
	crJXJRecvFubenRushContinueMethod(const crJXJRecvFubenRushContinueMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushContinue)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas; 
	std::string m_leftswitch;    
	std::string m_rightswitch;  
	std::string m_tipcanvas;
	std::string m_tipcontent;
};
class crJXJFubenRushEndMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushEndMethod();
	crJXJFubenRushEndMethod(const crJXJFubenRushEndMethod& handle);
	MethodClass(JXJ, JXJFubenRushEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_leftswitch;    
	std::string m_rightswitch;   
	std::string m_rushradio;    
	std::string m_rushcountedit; 
	std::string m_rushjunlinginput; 
	std::string m_list;
};
class crJXJRecvFubenRushEndMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFubenRushEndMethod();
	crJXJRecvFubenRushEndMethod(const crJXJRecvFubenRushEndMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJFubenRushDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJFubenRushDisposeMethod();
	crJXJFubenRushDisposeMethod(const crJXJFubenRushDisposeMethod& handle);
	MethodClass(JXJ, JXJFubenRushDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJRecvFubenRushInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvFubenRushInfoMethod();
	crJXJRecvFubenRushInfoMethod(const crJXJRecvFubenRushInfoMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;  
	std::string m_leftswitch;    
	std::string m_rightswitch;   
	std::string m_rushradio;    
	std::string m_rushcountedit; 
	std::string m_rushjunlinginput; 
	std::string m_numbersw1[3]; 
	std::string m_numbersw2[3]; 
};
class crJXJChapterRadioAddNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChapterRadioAddNameMethod();
	crJXJChapterRadioAddNameMethod(const crJXJChapterRadioAddNameMethod& handle);
	MethodClass(JXJ, JXJChapterRadioAddName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_chapterradio;
	std::string m_statename[11];
};
class crJXJUIWidgetChangePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIWidgetChangePageMethod();
	crJXJUIWidgetChangePageMethod(const crJXJUIWidgetChangePageMethod& handle);
	MethodClass(JXJ, JXJUIWidgetChangePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
	int m_curpageid;
	int m_totalpageid;
	std::string m_needCanvas;
	int m_extraPageId;
};
class crJXJUIShowTipsCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShowTipsCanvasMethod();
	crJXJUIShowTipsCanvasMethod(const crJXJUIShowTipsCanvasMethod& handle);
	MethodClass(JXJ, JXJUIShowTipsCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_tipcanvas;
	std::string m_tipcontent;
	int m_noticeindex;
	bool m_isModal;
};

class crJXJUICreateGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICreateGroupMethod();
	crJXJUICreateGroupMethod(const crJXJUICreateGroupMethod& handle);
	MethodClass(JXJ, JXJUICreateGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_inputname;    
	unsigned char m_index;
};
class crJXJRecvCreateGroupMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCreateGroupMethod();
	crJXJRecvCreateGroupMethod(const crJXJRecvCreateGroupMethod& handle);
	MethodClass(JXJ, JXJRecvCreateGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCreateGroupDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreateGroupDisposeMethod();
	crJXJCreateGroupDisposeMethod(const crJXJCreateGroupDisposeMethod& handle);
	MethodClass(JXJ, JXJCreateGroupDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< std::pair<int ,std::string> >m_taskVec;
};
class crJXJUIGroupListOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupListOnShowMethod();
	crJXJUIGroupListOnShowMethod(const crJXJUIGroupListOnShowMethod& handle);
	MethodClass(JXJ, JXJUIGroupListOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_step;
	int m_curpageid;
	int m_totalpageid;
};
class crJXJUIGroupListUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupListUpdateMethod();
	crJXJUIGroupListUpdateMethod(const crJXJUIGroupListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIGroupListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_disapplybtn[10];
	std::string m_checkinfobtn[10];
	std::string m_grouplist;
	std::string m_pageinput;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_recuitNotice;
	std::string m_info;
};



class crJXJQueryGroupListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGroupListMethod();
	crJXJQueryGroupListMethod(const crJXJQueryGroupListMethod& handle);
	MethodClass(JXJ, JXJQueryGroupList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;   
	std::string m_checkbox;     
};
class crJXJRecvQueryGroupListMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvQueryGroupListMethod();
	crJXJRecvQueryGroupListMethod(const crJXJRecvQueryGroupListMethod& handle);
	MethodClass(JXJ, JXJRecvQueryGroupList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQueryGroupDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryGroupDisposeMethod();
	crJXJQueryGroupDisposeMethod(const crJXJQueryGroupDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryGroupDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_validterm;//
	typedef std::map< CRCore::crVector2i,std::pair<CRCore::ref_ptr<CRCore::crStreamBuf>,float> > GroupRecordMap;//startid,count
	GroupRecordMap m_groupRecordMap;
	float *m_dt;
};
class crJXJClientRecvGroupListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvGroupListMethod();
	crJXJClientRecvGroupListMethod(const crJXJClientRecvGroupListMethod& handle);
	MethodClass(JXJ, JXJClientRecvGroupList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJCheckBagEmptyBagMethod :public CRCore::crMethod
{//Client Method & GameServer Method
public:
	crJXJCheckBagEmptyBagMethod();
	crJXJCheckBagEmptyBagMethod(const crJXJCheckBagEmptyBagMethod& handle);
	MethodClass(JXJ, JXJCheckBagEmptyBag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_tipcanvas;
	std::string m_tipcontent;
	int *m_count;
};
class crJXJSetCurSelectGroupIDMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetCurSelectGroupIDMethod();
	crJXJSetCurSelectGroupIDMethod(const crJXJSetCurSelectGroupIDMethod& handle);
	MethodClass(JXJ, JXJSetCurSelectGroupID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas; 
	std::string m_gamelist; 
	int m_index;
};
class crJXJApplyJionGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJApplyJionGroupMethod();
	crJXJApplyJionGroupMethod(const crJXJApplyJionGroupMethod& handle);
	MethodClass(JXJ, JXJApplyJionGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;  
};
class crJXJRecvApplyJionGroupMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvApplyJionGroupMethod();
	crJXJRecvApplyJionGroupMethod(const crJXJRecvApplyJionGroupMethod& handle);
	MethodClass(JXJ, JXJRecvApplyJionGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_groupcanvas;
	std::string m_myGroupcanvas;
};
class crJXJUIApplyListUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIApplyListUpdateMethod();
	crJXJUIApplyListUpdateMethod(const crJXJUIApplyListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIApplyListUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_applybtn[10];
	std::string m_disapplybtn[10];
	std::string m_grouplist;
	std::string m_groupignoreall;
	std::string m_pageinput;
	std::string m_farleftbtn;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_farrightbtn;
};
class crJXJResetCurPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetCurPageMethod();
	crJXJResetCurPageMethod(const crJXJResetCurPageMethod& handle);
	MethodClass(JXJ, JXJResetCurPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	int m_msg;
};
class crJXJQueryGroupApplyListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGroupApplyListMethod();
	crJXJQueryGroupApplyListMethod(const crJXJQueryGroupApplyListMethod& handle);
	MethodClass(JXJ, JXJQueryGroupApplyList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;       
};
class crJXJRecvQueryGroupApplyListMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryGroupApplyListMethod();
	crJXJRecvQueryGroupApplyListMethod(const crJXJRecvQueryGroupApplyListMethod& handle);
	MethodClass(JXJ, JXJRecvQueryGroupApplyList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};
class crJXJGroupSetCurSelectPlayerIDMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupSetCurSelectPlayerIDMethod();
	crJXJGroupSetCurSelectPlayerIDMethod(const crJXJGroupSetCurSelectPlayerIDMethod& handle);
	MethodClass(JXJ, JXJGroupSetCurSelectPlayerID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas; 
	std::string m_gamelist; 
	int m_index;
};
class crJXJAgreeGroupApplyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAgreeGroupApplyMethod();
	crJXJAgreeGroupApplyMethod(const crJXJAgreeGroupApplyMethod& handle);
	MethodClass(JXJ, JXJAgreeGroupApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;   
	std::string m_gameList;     
};
class crJXJRecvAgreeGroupApplyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAgreeGroupApplyMethod();
	crJXJRecvAgreeGroupApplyMethod(const crJXJRecvAgreeGroupApplyMethod& handle);
	MethodClass(JXJ, JXJRecvAgreeGroupApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};



class crJXJClientJoinGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientJoinGroupMethod();
	crJXJClientJoinGroupMethod(const crJXJClientJoinGroupMethod& handle);
	MethodClass(JXJ, JXJClientJoinGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};
class crJXJLeaveMyGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeaveMyGroupMethod();
	crJXJLeaveMyGroupMethod(const crJXJLeaveMyGroupMethod& handle);
	MethodClass(JXJ, JXJLeaveMyGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;     
};


class crJXJUIKickMemberOutofGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIKickMemberOutofGroupMethod();
	crJXJUIKickMemberOutofGroupMethod(const crJXJUIKickMemberOutofGroupMethod& handle);
	MethodClass(JXJ, JXJUIKickMemberOutofGroup)
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
	std::string m_groupcanvas;
};
class crJXJKickMemberOutofGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJKickMemberOutofGroupMethod();
	crJXJKickMemberOutofGroupMethod(const crJXJKickMemberOutofGroupMethod& handle);
	MethodClass(JXJ, JXJKickMemberOutofGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;     
	std::string m_groupList;
};
class crJXJRecvKickMemberOutofGroupMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvKickMemberOutofGroupMethod();
	crJXJRecvKickMemberOutofGroupMethod(const crJXJRecvKickMemberOutofGroupMethod& handle);
	MethodClass(JXJ, JXJRecvKickMemberOutofGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};



class crJXJClientLeaveGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientLeaveGroupMethod();
	crJXJClientLeaveGroupMethod(const crJXJClientLeaveGroupMethod& handle);
	MethodClass(JXJ, JXJClientLeaveGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_groupcanvas;
};
class crJXJCheckMyGroupIDMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckMyGroupIDMethod();
	crJXJCheckMyGroupIDMethod(const crJXJCheckMyGroupIDMethod& handle);
	MethodClass(JXJ, JXJCheckMyGroupID)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
};
class crJXJUICreateGroupCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICreateGroupCanvasUpdateMethod();
	crJXJUICreateGroupCanvasUpdateMethod(const crJXJUICreateGroupCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJUICreateGroupCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_nameInput; 
	std::string m_infoInput;   
	std::string m_coppercreatbtn;
	std::string m_goldcreatbtn; 
};
class crJXJUIShowGroupCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShowGroupCanvasMethod();
	crJXJUIShowGroupCanvasMethod(const crJXJUIShowGroupCanvasMethod& handle);
	MethodClass(JXJ, JXJUIShowGroupCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_grouplistcanvas; 
	std::string m_mygroupcanvas;   
	std::string m_strLoseGroupCanvas;
};
class crJXJQueryMyGroupInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyGroupInfoMethod();
	crJXJQueryMyGroupInfoMethod(const crJXJQueryMyGroupInfoMethod& handle);
	MethodClass(JXJ, JXJQueryMyGroupInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJRecvQueryMyGroupInfoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyGroupInfoMethod();
	crJXJRecvQueryMyGroupInfoMethod(const crJXJRecvQueryMyGroupInfoMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyGroupInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
	std::string m_lastNotice;
	std::string m_lastcreatorname;
	std::string m_lastgroupname;
	std::string m_lastgroupcreatedata;
};
class crJXJMyGroupInfoCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyGroupInfoCanvasUpdateMethod();
	crJXJMyGroupInfoCanvasUpdateMethod(const crJXJMyGroupInfoCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyGroupInfoCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_groupnameinput;   //军团名称
	std::string m_creatornameinput;   //军团长名称
	std::string m_creatortimeinput;   //创建时间
	std::string m_pagesw;   //切页sw
	std::string m_radio;   //切页radio
	std::string m_examinebtn;   //审核按钮
};
class crJXJMyGroupInfoPageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyGroupInfoPageUpdateMethod();
	crJXJMyGroupInfoPageUpdateMethod(const crJXJMyGroupInfoPageUpdateMethod& handle);
	MethodClass(JXJ, JXJMyGroupInfoPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_grouplv; 
	std::string m_notice;  
	std::string m_membercount;
	std::string m_editnotice; 
	std::string m_noticesw; 
	std::string m_savenotice; 
	std::string m_cancelnotice; 
	std::string m_groupranking; 
	std::string m_storeleft; 
};
class crJXJDisAgreeGroupApplyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDisAgreeGroupApplyMethod();
	crJXJDisAgreeGroupApplyMethod(const crJXJDisAgreeGroupApplyMethod& handle);
	MethodClass(JXJ, JXJDisAgreeGroupApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;   
	std::string m_groupList;
};
class crJXJRecvDisAgreeGroupApplyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvDisAgreeGroupApplyMethod();
	crJXJRecvDisAgreeGroupApplyMethod(const crJXJRecvDisAgreeGroupApplyMethod& handle);
	MethodClass(JXJ, JXJRecvDisAgreeGroupApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
	std::string m_applycanvas2;
};
class crJXJCancelApplyJionGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCancelApplyJionGroupMethod();
	crJXJCancelApplyJionGroupMethod(const crJXJCancelApplyJionGroupMethod& handle);
	MethodClass(JXJ, JXJCancelApplyJionGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;     
};
class crJXJRecvCancelApplyJionGroupMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCancelApplyJionGroupMethod();
	crJXJRecvCancelApplyJionGroupMethod(const crJXJRecvCancelApplyJionGroupMethod& handle);
	MethodClass(JXJ, JXJRecvCancelApplyJionGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_groupcanvas;
};
class crJXJCheckGroupInfoCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckGroupInfoCanvasUpdateMethod();
	crJXJCheckGroupInfoCanvasUpdateMethod(const crJXJCheckGroupInfoCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJCheckGroupInfoCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_textinput[9];
	std::string m_memberlist;
	std::string m_applybtn;
	std::string m_disapplybtn;
	std::string m_pageinput;
	std::string m_farleftbtn;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_farrightbtn;
	std::string m_canvas;
};
class crJXJCheckInvalidWordsMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJCheckInvalidWordsMethod();
	crJXJCheckInvalidWordsMethod(const crJXJCheckInvalidWordsMethod& handle);
	MethodClass(JXJ, JXJCheckInvalidWords)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_checkstring;
	int *m_success;
};
class crJXJQueryMyApplyGroupVecMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyApplyGroupVecMethod();
	crJXJQueryMyApplyGroupVecMethod(const crJXJQueryMyApplyGroupVecMethod& handle);
	MethodClass(JXJ, JXJQueryMyApplyGroupVec)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJRecvQueryMyApplyGroupVecMethodMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyApplyGroupVecMethodMethod();
	crJXJRecvQueryMyApplyGroupVecMethodMethod(const crJXJRecvQueryMyApplyGroupVecMethodMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyApplyGroupVecMethod)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};
class crJXJQueryMyGroupMemberListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyGroupMemberListMethod();
	crJXJQueryMyGroupMemberListMethod(const crJXJQueryMyGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJQueryMyGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;   
};
class crJXJQueryGroupMemberListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGroupMemberListMethod();
	crJXJQueryGroupMemberListMethod(const crJXJQueryGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJQueryGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;   
	std::string m_grouplistcanvas;     
	int m_offset;
};
class crJXJRecvQueryGroupMemberListMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvQueryGroupMemberListMethod();
	crJXJRecvQueryGroupMemberListMethod(const crJXJRecvQueryGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJRecvQueryGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQueryGroupMemberDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryGroupMemberDisposeMethod();
	crJXJQueryGroupMemberDisposeMethod(const crJXJQueryGroupMemberDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryGroupMemberDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_validterm;//
	typedef std::map< CRCore::crVector2i,std::pair<CRCore::ref_ptr<CRCore::crStreamBuf>,float> > GroupRecordMap;//startid,count
	GroupRecordMap m_groupRecordMap;
	GroupRecordMap m_mygroupRecordMap;
	float *m_dt;
};
class crJXJClientRecvGroupMemberListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvGroupMemberListMethod();
	crJXJClientRecvGroupMemberListMethod(const crJXJClientRecvGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJClientRecvGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;//查看军团界面
};
class crJXJClientRecvMyGroupMemberListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvMyGroupMemberListMethod();
	crJXJClientRecvMyGroupMemberListMethod(const crJXJClientRecvMyGroupMemberListMethod& handle);
	MethodClass(JXJ, JXJClientRecvMyGroupMemberList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;//我的军团界面
	std::string m_renmingcanvas;//我的军团任命界面
};
class crJXJUIChangePageShowWidgetMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangePageShowWidgetMethod();
	crJXJUIChangePageShowWidgetMethod(const crJXJUIChangePageShowWidgetMethod& handle);
	MethodClass(JXJ, JXJUIChangePageShowWidget)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_curpageid;
	int m_totalpageid;
	std::string m_farleftbtn; 
	std::string m_leftbtn; 
	std::string m_rightbtn; 
	std::string m_farrightbtn; 
	std::string m_needCanvas;
};

class crJXJClientDesposeEventMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientDesposeEventMethod();
	crJXJClientDesposeEventMethod(const crJXJClientDesposeEventMethod& handle);
	MethodClass(JXJ, JXJClientDesposeEvent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};
class crJXJDoClientDesposeEventMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDoClientDesposeEventMethod();
	crJXJDoClientDesposeEventMethod(const crJXJDoClientDesposeEventMethod& handle);
	MethodClass(JXJ, JXJDoClientDesposeEvent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_applycanvas;
};
class crJXJIgnoreAllApplyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIgnoreAllApplyMethod();
	crJXJIgnoreAllApplyMethod(const crJXJIgnoreAllApplyMethod& handle);
	MethodClass(JXJ, JXJIgnoreAllApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_validtime;
	int m_lasttime;
};
class crJXJRecvIgnoreAllApplyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvIgnoreAllApplyMethod();
	crJXJRecvIgnoreAllApplyMethod(const crJXJRecvIgnoreAllApplyMethod& handle);
	MethodClass(JXJ, JXJRecvIgnoreAllApply)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJUIButtonTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIButtonTipMethod();
	crJXJUIButtonTipMethod(const crJXJUIButtonTipMethod& handle);
	MethodClass(JXJ, JXJUIButtonTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_noticeid;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};
class crJXJRecvClientNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvClientNoticeMethod();
	crJXJRecvClientNoticeMethod(const crJXJRecvClientNoticeMethod& handle);
	MethodClass(JXJ, JXJRecvClientNotice)
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
};
class crJXJQueryPlayerRankingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryPlayerRankingMethod();
	crJXJQueryPlayerRankingMethod(const crJXJQueryPlayerRankingMethod& handle);
	MethodClass(JXJ, JXJQueryPlayerRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode *m_this;
	std::string m_canvasName;
	std::string m_combobox[2];
	std::string m_paraminput;
	unsigned char m_issearch;
	//int m_queryInterval;
	//unsigned char m_queryCount;
};
class crJXJRecvGovPostRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGovPostRankingMethod();
	crJXJRecvGovPostRankingMethod(const crJXJRecvGovPostRankingMethod& handle);
	MethodClass(JXJ, JXJRecvGovPostRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
	std::string m_canvasName1;
};
class crJXJRecvAccomplishRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAccomplishRankingMethod();
	crJXJRecvAccomplishRankingMethod(const crJXJRecvAccomplishRankingMethod& handle);
	MethodClass(JXJ, JXJRecvAccomplishRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJRecvTroopsRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvTroopsRankingMethod();
	crJXJRecvTroopsRankingMethod(const crJXJRecvTroopsRankingMethod& handle);
	MethodClass(JXJ, JXJRecvTroopsRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJRecvCountrycontributeRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCountrycontributeRankingMethod();
	crJXJRecvCountrycontributeRankingMethod(const crJXJRecvCountrycontributeRankingMethod& handle);
	MethodClass(JXJ, JXJRecvCountrycontributeRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJRecvPlayerLevelRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvPlayerLevelRankingMethod();
	crJXJRecvPlayerLevelRankingMethod(const crJXJRecvPlayerLevelRankingMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerLevelRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJRecvManorLvRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvManorLvRankingMethod();
	crJXJRecvManorLvRankingMethod(const crJXJRecvManorLvRankingMethod& handle);
	MethodClass(JXJ, JXJRecvManorLvRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJUIStartRollCardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIStartRollCardsMethod();
	crJXJUIStartRollCardsMethod(const crJXJUIStartRollCardsMethod& handle);
	MethodClass(JXJ, JXJUIStartRollCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJPlayerRankingPageUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerRankingPageUpdateMethod();
	crJXJPlayerRankingPageUpdateMethod(const crJXJPlayerRankingPageUpdateMethod& handle);
	MethodClass(JXJ,JXJPlayerRankingPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void displayRankingList(CRCore::ref_ptr<crPlayerRanking> playerRank, int row);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gamelist;
	std::string m_shilicombobox;
	std::string m_typecombobox;
};
class crJXJCountryRankingPageUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCountryRankingPageUpdateMethod();
	crJXJCountryRankingPageUpdateMethod(const crJXJCountryRankingPageUpdateMethod& handle);
	MethodClass(JXJ,JXJCountryRankingPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void displayRankingList(CRCore::ref_ptr<crJXJCountryInfo> countryinfo, int row);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gamelist;
	std::string m_typecombobox;
};
class crJXJGroupRankingPageUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGroupRankingPageUpdateMethod();
	crJXJGroupRankingPageUpdateMethod(const crJXJGroupRankingPageUpdateMethod& handle);
	MethodClass(JXJ,JXJGroupRankingPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void displayRankingList(CRCore::ref_ptr<crJXJGroupInfo> playerRank, int row);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gamelist;
	std::string m_shilicombobox;
	std::string m_typecombobox;
};
class crJXJRankingCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRankingCanvasUpdateMethod();
	crJXJRankingCanvasUpdateMethod(const crJXJRankingCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJRankingCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_pageinput;
	std::string m_radio;
	std::string m_pagesw;
	std::string m_myrankinput[2];
	std::string m_combobox[5];
	std::string m_mygroupcanvas;
};
class crJXJRankingCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRankingCanvasInitMethod();
	crJXJRankingCanvasInitMethod(const crJXJRankingCanvasInitMethod& handle);
	MethodClass(JXJ, JXJRankingCanvasInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_pageinput;
	std::string m_combobox[5];
	std::string m_gamelist[5];
	std::string m_paraminput[5];
};
class crJXJPutItemInBagMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJPutItemInBagMethod();
	crJXJPutItemInBagMethod(const crJXJPutItemInBagMethod& handle);
	MethodClass(JXJ, JXJPutItemInBag)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int getFirstEmptyIndex(JXJItemBagVec *bagvec);
	int getItemIndex(JXJItemBagVec *bagvec,int itemid,int equipmagic,int beginindex);
	bool isNotice(int tianshu);
	CRNetApp::crPlayerGameData *m_this;
	crBagItemData *m_bagitem;
	unsigned char m_type;//0.新获得1.整理背包
	unsigned char m_cycle;//0非轮回,1轮回获得
};
class crJXJRecvPutItemInBagMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPutItemInBagMethod();
	crJXJRecvPutItemInBagMethod(const crJXJRecvPutItemInBagMethod& handle);
	MethodClass(JXJ, JXJRecvPutItemInBag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJProduceCardValidTimeMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJProduceCardValidTimeMethod();
	crJXJProduceCardValidTimeMethod(const crJXJProduceCardValidTimeMethod& handle);
	MethodClass(JXJ, JXJProduceCardValidTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
};
class crJXJRecvProduceCardValidTimeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvProduceCardValidTimeMethod();
	crJXJRecvProduceCardValidTimeMethod(const crJXJRecvProduceCardValidTimeMethod& handle);
	MethodClass(JXJ, JXJRecvProduceCardValidTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUseImpressionBuyCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUseImpressionBuyCardMethod();
	crJXJUseImpressionBuyCardMethod(const crJXJUseImpressionBuyCardMethod& handle);
	MethodClass(JXJ, JXJUseImpressionBuyCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioWidgetNode* m_this;
	std::string m_enterFuben;
	std::string m_info;
};
class crJXJRecvUseImpressionBuyCardMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvUseImpressionBuyCardMethod();
	crJXJRecvUseImpressionBuyCardMethod(const crJXJRecvUseImpressionBuyCardMethod& handle);
	MethodClass(JXJ, JXJRecvUseImpressionBuyCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIResetFubenCompleteCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIResetFubenCompleteCountMethod();
	crJXJUIResetFubenCompleteCountMethod(const crJXJUIResetFubenCompleteCountMethod& handle);
	MethodClass(JXJ, JXJUIResetFubenCompleteCount)
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
class crJXJResetFubenCompleteCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetFubenCompleteCountMethod();
	crJXJResetFubenCompleteCountMethod(const crJXJResetFubenCompleteCountMethod& handle);
	MethodClass(JXJ, JXJResetFubenCompleteCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioWidgetNode* m_this;
	std::string m_enterFuben;
	std::string m_info;
};
//class crJXJRecvResetFubenCompleteCountMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvResetFubenCompleteCountMethod();
//	crJXJRecvResetFubenCompleteCountMethod(const crJXJRecvResetFubenCompleteCountMethod& handle);
//	MethodClass(JXJ, JXJRecvResetFubenCompleteCount)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_tipcanvas;
//	std::string m_tiptitle;
//	std::string m_tipcontent;
//};
//class crJXJCardShopOnShowMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCardShopOnShowMethod();
//	crJXJCardShopOnShowMethod(const crJXJCardShopOnShowMethod& handle);
//	MethodClass(JXJ, JXJCardShopOnShow)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//};
//class crJXJCardShopUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCardShopUpdateMethod();
//	crJXJCardShopUpdateMethod(const crJXJCardShopUpdateMethod& handle);
//	MethodClass(JXJ, JXJCardShopUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_typeradio;   //将卡类型
//	std::string m_colorradio;    //将卡颜色
//	std::string m_cardpiecesinput;   //将魂数量
//	std::string m_pagenuminput;   //页码
//	std::string m_priceinput[8];	//出售价格	
//	std::string m_purchasebtn[8]; //兑换按钮
//	std::string m_cardicon[8];	//将卡icon		
//};
//class crJXJUIPurChaseCardMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIPurChaseCardMethod();
//	crJXJUIPurChaseCardMethod(const crJXJUIPurChaseCardMethod& handle);
//	MethodClass(JXJ, JXJUIPurChaseCard)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	unsigned char m_index;
//	std::string m_promptCanvas;
//	std::string m_information;
//	std::string m_okbtn;
//	std::string m_font;
//	int m_characterSize;
//	CRCore::crVector4f m_textColor;
//	CRCore::crVector4f m_goldColor;
//};
//class crJXJPurChaseCardMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPurChaseCardMethod();
//	crJXJPurChaseCardMethod(const crJXJPurChaseCardMethod& handle);
//	MethodClass(JXJ, JXJPurChaseCard)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_enterFuben;
//	std::string m_info;
//};
//class crJXJRecvPurChaseCardMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvPurChaseCardMethod();
//	crJXJRecvPurChaseCardMethod(const crJXJRecvPurChaseCardMethod& handle);
//	MethodClass(JXJ, JXJRecvPurChaseCard)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJUIRollCardsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRollCardsMethod();
	crJXJUIRollCardsMethod(const crJXJUIRollCardsMethod& handle);
	MethodClass(JXJ, JXJUIRollCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_namecolor;
	int m_index;
};
class crJXJRollCardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRollCardsMethod();
	crJXJRollCardsMethod(const crJXJRollCardsMethod& handle);
	MethodClass(JXJ, JXJRollCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_progresscanvas;
};
class crJXJRecvRollCardsMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvRollCardsMethod();
	crJXJRecvRollCardsMethod(const crJXJRecvRollCardsMethod& handle);
	MethodClass(JXJ, JXJRecvRollCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	unsigned char getImpressionCrit();
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_jiusicanvas;
	std::string m_cardswitch;
	std::string m_promptinput;
	std::string m_tipcanvas;
	std::string m_tiptitle;
	std::string m_tipcontent;
	std::string m_page1;
	std::string m_progresscanvas;
	std::string m_purplecanvas;
	std::string m_orangecanvas;
	std::string m_baojicanvas;
	std::string m_baojisw;
	std::string m_rollcardBtn[3];//
};
class crJXJUIPickCardsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIPickCardsMethod();
	crJXJUIPickCardsMethod(const crJXJUIPickCardsMethod& handle);
	MethodClass(JXJ, JXJUIPickCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_namecolor;
	unsigned int m_index;
};
;class crJXJPickCardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPickCardsMethod();
	crJXJPickCardsMethod(const crJXJPickCardsMethod& handle);
	MethodClass(JXJ, JXJPickCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode *m_this;
	unsigned char m_index;
};
class crJXJRecvPickCardsMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvPickCardsMethod();
	crJXJRecvPickCardsMethod(const crJXJRecvPickCardsMethod& handle);
	MethodClass(JXJ, JXJRecvPickCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerPickCardsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerPickCardsMethod();
	crJXJServerPickCardsMethod(const crJXJServerPickCardsMethod& handle);
	MethodClass(JXJ, JXJServerPickCards)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int *m_itemid;
};
class crJXJUIJiushiUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiushiUpdateMethod();
	crJXJUIJiushiUpdateMethod(const crJXJUIJiushiUpdateMethod& handle);
	MethodClass(JXJ, JXJUIJiushiUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_impressionprogress;
	std::string m_impressioninput;
	std::string m_getgift;
	std::string m_coppercard;
	std::string m_goldcard;
	std::string m_getcards;
	std::string m_pickcards[3];
	std::string m_vipinfosw;
	std::string m_freecountinput;
	std::string m_cardshopbtn;
	std::string m_cardSwitch;
	std::string m_colorSwitch;
	std::string m_cardDonghua;
	std::string m_blueDonghua;
	std::string m_purpleDonghua;
	std::string m_orangeDonghua;
	std::string m_switch;
};
class crJXJCheckJiangkaBagEmptyMethod :public CRCore::crMethod
{//Client Method & GameServer Method
public:
	crJXJCheckJiangkaBagEmptyMethod();
	crJXJCheckJiangkaBagEmptyMethod(const crJXJCheckJiangkaBagEmptyMethod& handle);
	MethodClass(JXJ, JXJCheckJiangkaBagEmpty)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_tipcanvas;
	std::string m_tipcontent;
	int *m_count;
};
class crJXJUIJiusiTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiusiTipInfoMethod();
	crJXJUIJiusiTipInfoMethod(const crJXJUIJiusiTipInfoMethod& handle);
	MethodClass(JXJ, JXJUIJiusiTipInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info1;
	std::string m_sw;
	int m_index;
	int m_kejiindex;
};
class crJXJUIUndoFormationAddMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUndoFormationAddMethod();
	crJXJUIUndoFormationAddMethod(const crJXJUIUndoFormationAddMethod& handle);
	MethodClass(JXJ, JXJUIUndoFormationAdd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crImageBoxWidgetNode *m_this;
	int m_index;
	std::string m_curlead[3];
	std::string m_curcount[3];
	std::string m_curpower[3];
};
class crJXJUIOnFormationMaxMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIOnFormationMaxMethod();
	crJXJUIOnFormationMaxMethod(const crJXJUIOnFormationMaxMethod& handle);
	MethodClass(JXJ, JXJUIOnFormationMax)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_slide[3];
	std::string m_maxinput[3];
};
class crJXJDoEventProgressOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJDoEventProgressOnShowMethod();
	crJXJDoEventProgressOnShowMethod(const crJXJDoEventProgressOnShowMethod& handle);
	MethodClass(JXJ, JXJDoEventProgressOnShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_progress;
	int m_flg;//0：背包，1：将卡
};
//class crJXJDoEventProgressUpdateMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJDoEventProgressUpdateMethod();
//	crJXJDoEventProgressUpdateMethod(const crJXJDoEventProgressUpdateMethod& handle);
//	MethodClass(JXJ, JXJDoEventProgressUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	int m_index;
//	std::string m_progress;
//	int m_totalcount;
//};
class crJXJProgressFrameUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJProgressFrameUpdateMethod();
	crJXJProgressFrameUpdateMethod(const crJXJProgressFrameUpdateMethod& handle);
	MethodClass(JXJ, JXJProgressFrameUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_dt;
	std::string m_progress;
	int m_flg;//0：背包，1：将卡
	float m_netspeed;
};
class crJXJParentCanvasDoEventMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJParentCanvasDoEventMethod();
	crJXJParentCanvasDoEventMethod(const crJXJParentCanvasDoEventMethod& handle);
	MethodClass(JXJ, JXJParentCanvasDoEvent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_buttonindex;
	int m_eventindex;
};
class crJXJQueryCountryInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryInfoMethod();
	crJXJQueryCountryInfoMethod(const crJXJQueryCountryInfoMethod& handle);
	MethodClass(JXJ, JXJQueryCountryInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJRecvQueryCountryInfoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryCountryInfoMethod();
	crJXJRecvQueryCountryInfoMethod(const crJXJRecvQueryCountryInfoMethod& handle);
	MethodClass(JXJ, JXJRecvQueryCountryInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJGovPostShopOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGovPostShopOnShowMethod();
	crJXJGovPostShopOnShowMethod(const crJXJGovPostShopOnShowMethod& handle);
	MethodClass(JXJ, JXJGovPostShopOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_scrollbar;
};
//class crJXJGovPostShopUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJGovPostShopUpdateMethod();
//	crJXJGovPostShopUpdateMethod(const crJXJGovPostShopUpdateMethod& handle);
//	MethodClass(JXJ, JXJGovPostShopUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_itemicon[5];	//道具icon	
//	std::string m_nameinput[5];	//道具名称	
//	std::string m_priceinput[5]; //出售价格		
//	std::string m_infoinput[5]; //道具说明
//	std::string m_govpostinput[5]; //需求官职
//	std::string m_purchasebtn[5]; //购买按钮
//	std::string m_priceinfoinput[5]; //消耗国家贡献
//	std::string m_pageinput;   //页码
//	std::string m_curmoney;   //当前拥有贡献
//	std::string m_checkbox;   //是否只显示当前官职，1是，0否
//};
class crJXJUIPurChaseGovPostItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPurChaseGovPostItemMethod();
	crJXJUIPurChaseGovPostItemMethod(const crJXJUIPurChaseGovPostItemMethod& handle);
	MethodClass(JXJ, JXJUIPurChaseGovPostItem)
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
class crJXJPurChaseGovPostItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPurChaseGovPostItemMethod();
	crJXJPurChaseGovPostItemMethod(const crJXJPurChaseGovPostItemMethod& handle);
	MethodClass(JXJ, JXJPurChaseGovPostItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvPurChaseGovPostItemMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvPurChaseGovPostItemMethod();
	crJXJRecvPurChaseGovPostItemMethod(const crJXJRecvPurChaseGovPostItemMethod& handle);
	MethodClass(JXJ, JXJRecvPurChaseGovPostItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJQueryConsortiaRankingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryConsortiaRankingMethod();
	crJXJQueryConsortiaRankingMethod(const crJXJQueryConsortiaRankingMethod& handle);
	MethodClass(JXJ, JXJQueryConsortiaRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvasName;
	std::string m_combobox[2];
	std::string m_paraminput;
	unsigned char m_issearch;
	//int m_queryInterval;
	//unsigned char m_queryCount;
};
class crJXJRecvConsortialvRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvConsortialvRankingMethod();
	crJXJRecvConsortialvRankingMethod(const crJXJRecvConsortialvRankingMethod& handle);
	MethodClass(JXJ, JXJRecvConsortialvRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJRecvConsortiaMemberRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvConsortiaMemberRankingMethod();
	crJXJRecvConsortiaMemberRankingMethod(const crJXJRecvConsortiaMemberRankingMethod& handle);
	MethodClass(JXJ, JXJRecvConsortiaMemberRanking)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};
class crJXJDeductCopperCashMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJDeductCopperCashMethod();
	crJXJDeductCopperCashMethod(const crJXJDeductCopperCashMethod& handle);
	MethodClass(JXJ, JXJDeductCopperCash)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_needdeduct;
};
class crJXJCheckCopperCashMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJCheckCopperCashMethod();
	crJXJCheckCopperCashMethod(const crJXJCheckCopperCashMethod& handle);
	MethodClass(JXJ, JXJCheckCopperCash)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_needdeduct;
};
class crJXJRecvDeductCopperCashMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvDeductCopperCashMethod();
	crJXJRecvDeductCopperCashMethod(const crJXJRecvDeductCopperCashMethod& handle);
	MethodClass(JXJ, JXJRecvDeductCopperCash)
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
