/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRNETAPP_METHODCLASS_H
#define CRNETAPP_METHODCLASS_H 1

#include <CRNetApp/appCharacterUpdateMethod.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRText/crText.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRCore/crUniversalHandles.h>
#include <CRPhysics/crCamera.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crShapeDrawable.h>
#include <CRUtil/crSetObjectColorVisitor.h>
#include <CRUI/crWidgetExtend.h>
#include <CREncapsulation/crTableIO.h>
#include <CRNetApp/appNetCallbacks.h>
#include <ode/ode.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{

//class crRoleEventContainerMethod : public CRCore::crBaseEventContainerMethod
//{//
//public:
//	crRoleEventContainerMethod();
//	crRoleEventContainerMethod(const crRoleEventContainerMethod& handle);
//	MethodClass(CRNetApp, RoleEventContainer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};
//class crSelectRolesEventMethod : public CRCore::crMethod
//{//
//public:
//	crSelectRolesEventMethod();
//	crSelectRolesEventMethod(const crSelectRolesEventMethod& handle);
//	MethodClass(CRNetApp, SelectRolesEvent)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crMatrixTransform* m_this;
//	_crInt64 m_param;
//};
class crQuickKeyPressedMethod : public CRCore::crMethod
{//GameClient Method
public:
	crQuickKeyPressedMethod();
	crQuickKeyPressedMethod(const crQuickKeyPressedMethod& handle);
	MethodClass(CRNetApp, QuickKeyPressed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_itemid;
};
class crInitNetMethod : public CRCore::crMethod
{//
public:
	crInitNetMethod();
	crInitNetMethod(const crInitNetMethod& handle);
	MethodClass(CRNetApp, InitNet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crFiniNetMethod : public CRCore::crMethod
{//
public:
	crFiniNetMethod();
	crFiniNetMethod(const crFiniNetMethod& handle);
	MethodClass(CRNetApp, FiniNet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crConnectToLoginServerNewMethod : public CRCore::crMethod
{//
public:
	crConnectToLoginServerNewMethod();
	crConnectToLoginServerNewMethod(const crConnectToLoginServerNewMethod& handle);
	MethodClass(CRNetApp, ConnectToLoginServerNew)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
};
class crLoginMethod : public CRCore::crMethod
{//
public:
	crLoginMethod();
	crLoginMethod(const crLoginMethod& handle);
	MethodClass(CRNetApp, Login)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
};
class crRegisterAccountMethod : public CRCore::crMethod
{//
public:
	crRegisterAccountMethod();
	crRegisterAccountMethod(const crRegisterAccountMethod& handle);
	MethodClass(CRNetApp, RegisterAccount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
	std::string m_password2Widget;
	std::string m_emailWidget;
	std::string m_cdkeyWidget;
};
class crConnectToLoginServerNew2Method : public CRCore::crMethod
{//
public:
	crConnectToLoginServerNew2Method();
	crConnectToLoginServerNew2Method(const crConnectToLoginServerNew2Method& handle);
	MethodClass(CRNetApp, ConnectToLoginServerNew2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
	int m_serverCount;
	std::string m_password2Widget;
	std::string m_emailWidget;
};

//class crConnectToDownloadServerMethod : public CRCore::crMethod
//{//
//public:
//	crConnectToDownloadServerMethod();
//	crConnectToDownloadServerMethod(const crConnectToDownloadServerMethod& handle);
//	MethodClass(CRNetApp, ConnectToDownloadServer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	char m_flg;//0：启动下载，1：EnterGame下载
//};
//
//class crDisconnectDownloadServerMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crDisconnectDownloadServerMethod();
//	crDisconnectDownloadServerMethod(const crDisconnectDownloadServerMethod& handle);
//	MethodClass(CRNetApp, DisconnectDownloadServer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};

//class crLoginGameDownloadServerMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crLoginGameDownloadServerMethod();
//	crLoginGameDownloadServerMethod(const crLoginGameDownloadServerMethod& handle);
//	MethodClass(CRNetApp, LoginGameDownloadServer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};

class crLoginGameSceneMethod : public CRCore::crMethod
{//GameClient Method
public:
	crLoginGameSceneMethod();
	crLoginGameSceneMethod(const crLoginGameSceneMethod& handle);
	MethodClass(CRNetApp, LoginGameScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crLoginGameScene2Method : public CRCore::crMethod
{//GameClient Method
public:
	crLoginGameScene2Method();
	crLoginGameScene2Method(const crLoginGameScene2Method& handle);
	MethodClass(CRNetApp, LoginGameScene2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crDisconnectGameServerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crDisconnectGameServerMethod();
	crDisconnectGameServerMethod(const crDisconnectGameServerMethod& handle);
	MethodClass(CRNetApp, DisconnectGameServer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crLoginChatMethod : public CRCore::crMethod
{//
public:
	crLoginChatMethod();
	crLoginChatMethod(const crLoginChatMethod& handle);
	MethodClass(CRNetApp, LoginChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_serverid;
};

class crRecvChatFriendListMethod : public CRCore::crMethod
{//收到DataStream
public:
	crRecvChatFriendListMethod();
	crRecvChatFriendListMethod(const crRecvChatFriendListMethod& handle);
	MethodClass(CRNetApp, RecvChatFriendList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crRecvChatGroupListMethod : public CRCore::crMethod
{//收到DataStream
public:
	crRecvChatGroupListMethod();
	crRecvChatGroupListMethod(const crRecvChatGroupListMethod& handle);
	MethodClass(CRNetApp, RecvChatGroupList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crRecvChatGroupMemberListMethod : public CRCore::crMethod
{//收到DataStream
public:
	crRecvChatGroupMemberListMethod();
	crRecvChatGroupMemberListMethod(const crRecvChatGroupMemberListMethod& handle);
	MethodClass(CRNetApp, RecvChatGroupMemberList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

//class crChatDataTransferEndMethod : public CRCore::crMethod
//{//
//public:
//	crChatDataTransferEndMethod();
//	crChatDataTransferEndMethod(const crChatDataTransferEndMethod& handle);
//	MethodClass(CRNetApp, ChatDataTransferEnd)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_stageName;
//	std::string m_friendTreeElementName;
//	std::string m_groupTreeElementName;
//};

class crRecvChatStateChangeMethod : public CRCore::crMethod
{//
public:
	crRecvChatStateChangeMethod();
	crRecvChatStateChangeMethod(const crRecvChatStateChangeMethod& handle);
	MethodClass(CRNetApp, RecvChatStateChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_chatMain;
	std::string m_friendTreeElement;
	std::string m_chatGroup;
	std::string m_groupTreeElement;
};

class crBuildFriendTreeMethod : public CRCore::crMethod
{//
public:
	crBuildFriendTreeMethod();
	crBuildFriendTreeMethod(const crBuildFriendTreeMethod& handle);
	MethodClass(CRNetApp, BuildFriendTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crTreeElement> m_friendTreeElement;
};

class crBuildGroupTreeMethod : public CRCore::crMethod
{//
public:
	crBuildGroupTreeMethod();
	crBuildGroupTreeMethod(const crBuildGroupTreeMethod& handle);
	MethodClass(CRNetApp, BuildGroupTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crTreeElement> m_groupTreeElement;
};

class crInfoTextMethod : public CRCore::crMethod
{//
public:
	crInfoTextMethod();
	crInfoTextMethod(const crInfoTextMethod& handle);
	MethodClass(CRNetApp, InfoText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_infoTextElement;
	std::string m_infoText;
	int m_flg;
};

class crConnectToLoginServer2Method : public CRCore::crMethod
{//
public:
	crConnectToLoginServer2Method();
	crConnectToLoginServer2Method(const crConnectToLoginServer2Method& handle);
	MethodClass(CRNetApp, ConnectToLoginServer2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_serverid;
};

//class crRegisterAccountMethod : public CRCore::crMethod
//{//
//public:
//	crRegisterAccountMethod();
//	crRegisterAccountMethod(const crRegisterAccountMethod& handle);
//	MethodClass(CRNetApp, RegisterAccount)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRGUI::crElement> m_element;
//	std::string m_userNameElement;
//	std::string m_userPasswordElement;
//};

class crQueryOnlinePlayerListMethod : public CRCore::crMethod
{//
public:
	crQueryOnlinePlayerListMethod();
	crQueryOnlinePlayerListMethod(const crQueryOnlinePlayerListMethod& handle);
	MethodClass(CRNetApp, QueryOnlinePlayerList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crOnlinePlayerListMethod : public CRCore::crMethod
{//
public:
	crOnlinePlayerListMethod();
	crOnlinePlayerListMethod(const crOnlinePlayerListMethod& handle);
	MethodClass(CRNetApp, OnlinePlayerList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crAddChatFriendMethod : public CRCore::crMethod
{//
public:
	crAddChatFriendMethod();
	crAddChatFriendMethod(const crAddChatFriendMethod& handle);
	MethodClass(CRNetApp, AddChatFriend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_columnListElement;
};

class crInsertToChatFriendTreeMethod : public CRCore::crMethod
{//
public:
	crInsertToChatFriendTreeMethod();
	crInsertToChatFriendTreeMethod(const crInsertToChatFriendTreeMethod& handle);
	MethodClass(CRNetApp, InsertToChatFriendTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_friendTreeElementName;
	int m_playerid;
};

class crCreateChatGroupMethod : public CRCore::crMethod
{//
public:
	crCreateChatGroupMethod();
	crCreateChatGroupMethod(const crCreateChatGroupMethod& handle);
	MethodClass(CRNetApp, CreateChatGroup)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_groupNameElement;
	std::string m_groupIntroduceElement;
};

class crInsertToChatGroupTreeMethod : public CRCore::crMethod
{//
public:
	crInsertToChatGroupTreeMethod();
	crInsertToChatGroupTreeMethod(const crInsertToChatGroupTreeMethod& handle);
	MethodClass(CRNetApp, InsertToChatGroupTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_groupTreeElementName;
	int m_groupid;
};

class crChatWithFriendMethod : public CRCore::crMethod
{//
public:
	crChatWithFriendMethod();
	crChatWithFriendMethod(const crChatWithFriendMethod& handle);
	MethodClass(CRNetApp, ChatWithFriend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crTreeElement> m_treeElement;
	std::string m_stageName;
	std::string m_titleElement;
	std::string m_chatRecElement;
};

class crQueryChatGroupListMethod : public CRCore::crMethod
{//
public:
	crQueryChatGroupListMethod();
	crQueryChatGroupListMethod(const crQueryChatGroupListMethod& handle);
	MethodClass(CRNetApp, QueryChatGroupList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crChatGroupListMethod : public CRCore::crMethod
{//
public:
	crChatGroupListMethod();
	crChatGroupListMethod(const crChatGroupListMethod& handle);
	MethodClass(CRNetApp, ChatGroupList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crSendChatMethod : public CRCore::crMethod
{//
public:
	crSendChatMethod();
	crSendChatMethod(const crSendChatMethod& handle);
	MethodClass(CRNetApp, SendChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_chatInputElement;
	std::string m_chatRecElement;
};

class crRecvFriendChatMethod : public CRCore::crMethod
{//
public:
	crRecvFriendChatMethod();
	crRecvFriendChatMethod(const crRecvFriendChatMethod& handle);
	MethodClass(CRNetApp, RecvFriendChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_stageName;
	std::string m_titleElement;
    std::string m_chatRecElement;
};
class crLoadRecMethod : public CRCore::crMethod
{//
public:
	crLoadRecMethod();
	crLoadRecMethod(const crLoadRecMethod& handle);
	MethodClass(CRNetApp, LoadRec)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_chatRecElement;
};
class crSendSceneChatMethod : public CRCore::crMethod
{//GameClient Method 将聊天信息发送给所有和我在相同游戏相同场景里的人 从SceneServer发送
public:
	crSendSceneChatMethod();
	crSendSceneChatMethod(const crSendSceneChatMethod& handle);
	MethodClass(CRNetApp, SendSceneChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_chatInputElement;
	std::string m_chatRecElement;
};
class crSendSceneChat2Method : public CRCore::crMethod
{//GameClient Method 将聊天信息发送给所有和我在相同游戏相同场景里的人 从SceneServer发送
public:
	crSendSceneChat2Method();
	crSendSceneChat2Method(const crSendSceneChat2Method& handle);
	MethodClass(CRNetApp, SendSceneChat2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_chatInput;
	std::string m_chatRec;
};
class crSendGameChatMethod : public CRCore::crMethod
{//GameClient Method 将聊天信息发送给所有和我在相同游戏相同场景里的人 从SceneServer发送
public:
	crSendGameChatMethod();
	crSendGameChatMethod(const crSendGameChatMethod& handle);
	MethodClass(CRNetApp, SendGameChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_chatInputElement;
	std::string m_chatRecElement;
};
class crRecvGameChatMethod : public CRCore::crMethod
{//GameClient Method，收到场景里的消息
public:
	crRecvGameChatMethod();
	crRecvGameChatMethod(const crRecvGameChatMethod& handle);
	MethodClass(CRNetApp, RecvGameChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_stageName;
	std::string m_chatRecElement;
};
class crRecvGameChat2Method : public CRCore::crMethod
{//GameClient Method，收到场景里的消息
public:
	crRecvGameChat2Method();
	crRecvGameChat2Method(const crRecvGameChat2Method& handle);
	MethodClass(CRNetApp, RecvGameChat2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvasName;
	std::string m_chatRec;
};
class crAddChatFriendSystemMsgMethod : public CRCore::crMethod
{//ChatServer & GameClient_Chat Method
public:
	crAddChatFriendSystemMsgMethod();
	crAddChatFriendSystemMsgMethod(const crAddChatFriendSystemMsgMethod& handle);
	MethodClass(CRNetApp, AddChatFriendSystemMsg)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	CRCore::ref_ptr<crChatServerPlayerData> m_recvPlayerData;
	std::string m_string;
	std::string m_stageName;
	std::string m_msgTextElement;
};
//////NetGame
//class crInitCheckAndDownloadFilesMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crInitCheckAndDownloadFilesMethod();
//	crInitCheckAndDownloadFilesMethod(const crInitCheckAndDownloadFilesMethod& handle);
//	MethodClass(CRNetApp, InitCheckAndDownloadFiles)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crInitCheckFileListMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crInitCheckFileListMethod();
//	crInitCheckFileListMethod(const crInitCheckFileListMethod& handle);
//	MethodClass(CRNetApp, InitCheckFileList)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crInitCheckEndMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crInitCheckEndMethod();
//	crInitCheckEndMethod(const crInitCheckEndMethod& handle);
//	MethodClass(CRNetApp, InitCheckEnd)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	static void needUpdate(int flg);
//protected:
//	static int s_needUpdate;//1：重启 2：重启，更新程序
//};
class crRecvGameListMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvGameListMethod();
	crRecvGameListMethod(const crRecvGameListMethod& handle);
	MethodClass(CRNetApp, RecvGameList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
};

class crRecvGameList2Method : public CRCore::crMethod
{//Client Method
public:
	crRecvGameList2Method();
	crRecvGameList2Method(const crRecvGameList2Method& handle);
	MethodClass(CRNetApp, RecvGameList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_tableWidget;
};

class crSetSelectedGameMethod : public CRCore::crMethod
{//
public:
	crSetSelectedGameMethod();
	crSetSelectedGameMethod(const crSetSelectedGameMethod& handle);
	MethodClass(CRNetApp, SetSelectedGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_columnListElement;
};

class crSetSelectedGame2Method : public CRCore::crMethod
{//
public:
	crSetSelectedGame2Method();
	crSetSelectedGame2Method(const crSetSelectedGame2Method& handle);
	MethodClass(CRNetApp, SetSelectedGame2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_columnListWidget;
};

class crExternSelectedGameMethod : public CRCore::crMethod
{//
public:
	crExternSelectedGameMethod();
	crExternSelectedGameMethod(const crExternSelectedGameMethod& handle);
	MethodClass(CRNetApp, ExternSelectedGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

//class crEnterGameDownloadFilesMethod : public CRCore::crMethod
//{//
//public:
//	crEnterGameDownloadFilesMethod();
//	crEnterGameDownloadFilesMethod(const crEnterGameDownloadFilesMethod& handle);crCheckFileListMethod
//	MethodClass(CRNetApp, EnterGameDownloadFiles)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crRecvingDownLoadFileMethod : public CRCore::crMethod
//{//收到DataStream
//public:
//	crRecvingDownLoadFileMethod();
//	crRecvingDownLoadFileMethod(const crRecvingDownLoadFileMethod& handle);
//	MethodClass(CRNetApp, RecvingDownLoadFile)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<crRecvFileStream> m_recvFileStream;
//	std::string m_notifyStage;
//	std::string m_notifyTextElement;
//};
//class crRecvingDownLoadFile2Method : public CRCore::crMethod
//{//收到DataStream
//public:
//	crRecvingDownLoadFile2Method();
//	crRecvingDownLoadFile2Method(const crRecvingDownLoadFile2Method& handle);
//	MethodClass(CRNetApp, RecvingDownLoadFile2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<crRecvFileStream> m_recvFileStream;
//};
//class crCheckFileListMethod : public CRCore::crMethod
//{//GameClient
//public:
//	crCheckFileListMethod();
//	crCheckFileListMethod(const crCheckFileListMethod& handle);
//	MethodClass(CRNetApp, CheckFileList)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crEnterGameMethod : public CRCore::crMethod
{//GameClient
public:
	crEnterGameMethod();
	crEnterGameMethod(const crEnterGameMethod& handle);
	MethodClass(CRNetApp, EnterGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crEnterSceneMethod : public CRCore::crMethod
{//GameClient
public:
	crEnterSceneMethod();
	crEnterSceneMethod(const crEnterSceneMethod& handle);
	MethodClass(CRNetApp, EnterScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//class crCheckRoleUpdatecfgMethod : public CRCore::crMethod
//{//GameClient
//public:
//	crCheckRoleUpdatecfgMethod();
//	crCheckRoleUpdatecfgMethod(const crCheckRoleUpdatecfgMethod& handle);
//	MethodClass(CRNetApp, CheckRoleUpdatecfg)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crEnterSceneSucessMethod : public CRCore::crMethod
{//GameClient
public:
	crEnterSceneSucessMethod();
	crEnterSceneSucessMethod(const crEnterSceneSucessMethod& handle);
	MethodClass(CRNetApp, EnterSceneSucess)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvedItemMethod : public CRCore::crMethod
{//GameClient
public:
	crRecvedItemMethod();
	crRecvedItemMethod(const crRecvedItemMethod& handle);
	MethodClass(CRNetApp, RecvedItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	short m_id;
};
//class crRecvedItem2Method : public CRCore::crMethod
//{//GameClient
//public:
//	crRecvedItem2Method();
//	crRecvedItem2Method(const crRecvedItem2Method& handle);
//	MethodClass(CRNetApp, RecvedItem2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crRecvedItemChildMethod : public CRCore::crMethod
{//GameClient
public:
	crRecvedItemChildMethod();
	crRecvedItemChildMethod(const crRecvedItemChildMethod& handle);
	MethodClass(CRNetApp, RecvedItemChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	short m_id;
};
//class crRecvedItemChild2Method : public CRCore::crMethod
//{//GameClient
//public:
//	crRecvedItemChild2Method();
//	crRecvedItemChild2Method(const crRecvedItemChild2Method& handle);
//	MethodClass(CRNetApp, RecvedItemChild2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crDownloadFileEndMethod : public CRCore::crMethod
//{//
//public:
//	crDownloadFileEndMethod();
//	crDownloadFileEndMethod(const crDownloadFileEndMethod& handle);
//	MethodClass(CRNetApp, DownloadFileEnd)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	std::string m_notifyStage;
//	std::string m_notifyTextElement;
//};

//class crCheckAndDownloadFileMethod : public CRCore::crMethod
//{//
//public:
//	crCheckAndDownloadFileMethod();
//	crCheckAndDownloadFileMethod(const crCheckAndDownloadFileMethod& handle);
//	MethodClass(CRNetApp, CheckAndDownloadFile)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_fileName;
//	//std::string m_fileModifyTime;
//	CRCore::Timer_t m_fileModifyTime;
//	unsigned short m_downloadFlg;
//};

//class crWaitLoginGameReturnMethod : public CRCore::crMethod
//{//
//public:
//	crWaitLoginGameReturnMethod();
//	crWaitLoginGameReturnMethod(const crWaitLoginGameReturnMethod& handle);
//	MethodClass(CRNetApp, WaitLoginGameReturn)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	static void loginGameReturn(char loginCode);
//protected:
//	//static CRCore::crCondition s_loginGameCondition;
//	//static CRCore::crMutex s_loginGameMutex;
//	static CRCore::crBarrier s_loginGameBarrier;
//	static char s_loginCode;
//};
class crLoginGameMethod : public CRCore::crMethod
{//
public:
	crLoginGameMethod();
	crLoginGameMethod(const crLoginGameMethod& handle);
	MethodClass(CRNetApp, LoginGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crLoginGameReturnMethod : public CRCore::crMethod
{//
public:
	crLoginGameReturnMethod();
	crLoginGameReturnMethod(const crLoginGameReturnMethod& handle);
	MethodClass(CRNetApp, LoginGameReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_loginCode;
};
class crNotifyMethod : public CRCore::crMethod
{//
public:
	crNotifyMethod();
	crNotifyMethod(const crNotifyMethod& handle);
	MethodClass(CRNetApp, Notify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_type;
	std::string m_notify;
};

class crRecvGameRoleListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvGameRoleListMethod();
	crRecvGameRoleListMethod(const crRecvGameRoleListMethod& handle);
	MethodClass(CRNetApp, RecvGameRoleList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
};

class crRecvGameRoleList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crRecvGameRoleList2Method();
	crRecvGameRoleList2Method(const crRecvGameRoleList2Method& handle);
	MethodClass(CRNetApp, RecvGameRoleList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_columnListWidget;
};

class crRecvGameRoleListStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvGameRoleListStreamMethod();
	crRecvGameRoleListStreamMethod(const crRecvGameRoleListStreamMethod& handle);
	MethodClass(CRNetApp, RecvGameRoleListStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crRecvCreateRoleStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvCreateRoleStreamMethod();
	crRecvCreateRoleStreamMethod(const crRecvCreateRoleStreamMethod& handle);
	MethodClass(CRNetApp, RecvCreateRoleStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crRecvCreateMainRoleStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvCreateMainRoleStreamMethod();
	crRecvCreateMainRoleStreamMethod(const crRecvCreateMainRoleStreamMethod& handle);
	MethodClass(CRNetApp, RecvCreateMainRoleStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crSetSelectRolePositionMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetSelectRolePositionMethod();
	crSetSelectRolePositionMethod(const crSetSelectRolePositionMethod& handle);
	MethodClass(CRNetApp, SetSelectRolePosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crInstanceItem> m_role;
	int m_roleIndex;
	CRCore::crVector3 m_dir;//朝向
	typedef std::vector<CRCore::crVector3> PositionVec;
	PositionVec m_positionVec;
};
class crCheckAndLoadGameRolesMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCheckAndLoadGameRolesMethod();
	crCheckAndLoadGameRolesMethod(const crCheckAndLoadGameRolesMethod& handle);
	MethodClass(CRNetApp, CheckAndLoadGameRoles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetSelectedRoleMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetSelectedRoleMethod();
	crSetSelectedRoleMethod(const crSetSelectedRoleMethod& handle);
	MethodClass(CRNetApp, SetSelectedRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_columnListElement;
};

class crSetSelectedRole2Method : public CRCore::crMethod
{//GameClient Method
public:
	crSetSelectedRole2Method();
	crSetSelectedRole2Method(const crSetSelectedRole2Method& handle);
	MethodClass(CRNetApp, SetSelectedRole2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_columnListWidget;
};

class crQuerySceneInfoMethod : public CRCore::crMethod
{//
public:
	crQuerySceneInfoMethod();
	crQuerySceneInfoMethod(const crQuerySceneInfoMethod& handle);
	MethodClass(CRNetApp, QuerySceneInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crQuerySceneInfo2Method : public CRCore::crMethod
{//RoomGame
public:
	crQuerySceneInfo2Method();
	crQuerySceneInfo2Method(const crQuerySceneInfo2Method& handle);
	MethodClass(CRNetApp, QuerySceneInfo2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

//class crLoginSceneMethod : public CRCore::crMethod
//{//
//public:
//	crLoginSceneMethod();
//	crLoginSceneMethod(const crLoginSceneMethod& handle);
//	MethodClass(CRNetApp, LoginScene)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};

class crLoginSceneReturnMethod : public CRCore::crMethod
{//
public:
	crLoginSceneReturnMethod();
	crLoginSceneReturnMethod(const crLoginSceneReturnMethod& handle);
	MethodClass(CRNetApp, LoginSceneReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_loginCode;
};
class crIsNetReturnSuccessMethod : public CRCore::crMethod
{//
public:
	crIsNetReturnSuccessMethod();
	crIsNetReturnSuccessMethod(const crIsNetReturnSuccessMethod& handle);
	MethodClass(CRNetApp, IsNetReturnSuccess)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crIsSuccessMethod : public CRCore::crMethod
{//
public:
	crIsSuccessMethod();
	crIsSuccessMethod(const crIsSuccessMethod& handle);
	MethodClass(CRNetApp, IsSuccess)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_code;
};
class crRecvSceneItemListMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvSceneItemListMethod();
	crRecvSceneItemListMethod(const crRecvSceneItemListMethod& handle);
	MethodClass(CRNetApp, RecvSceneItemList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crQueryRoleInfoMethod : public CRCore::crMethod
{//
public:
	crQueryRoleInfoMethod();
	crQueryRoleInfoMethod(const crQueryRoleInfoMethod& handle);
	MethodClass(CRNetApp, QueryRoleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crRecvRoleInfo2Method : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvRoleInfo2Method();
	crRecvRoleInfo2Method(const crRecvRoleInfo2Method& handle);
	MethodClass(CRNetApp, RecvRoleInfo2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crRecvMainRoleMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvMainRoleMethod();
	crRecvMainRoleMethod(const crRecvMainRoleMethod& handle);
	MethodClass(CRNetApp, RecvMainRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crServerCheckRoleDataMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crServerCheckRoleDataMethod();
	crServerCheckRoleDataMethod(const crServerCheckRoleDataMethod& handle);
	MethodClass(CRNetApp, ServerCheckRoleData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
};
class crRecvRoleInfoMethod : public CRCore::crMethod
{//
public:
	crRecvRoleInfoMethod();
	crRecvRoleInfoMethod(const crRecvRoleInfoMethod& handle);
	MethodClass(CRNetApp, RecvRoleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crRecvItemMethod : public CRCore::crMethod
{//
public:
	crRecvItemMethod();
	crRecvItemMethod(const crRecvItemMethod& handle);
	MethodClass(CRNetApp, RecvItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crInitNetGameMethod : public CRCore::crMethod
{//
public:
	crInitNetGameMethod();
	crInitNetGameMethod(const crInitNetGameMethod& handle);
	MethodClass(CRNetApp, InitNetGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crFinitNetGameMethod : public CRCore::crMethod
{//
public:
	crFinitNetGameMethod();
	crFinitNetGameMethod(const crFinitNetGameMethod& handle);
	MethodClass(CRNetApp, FinitNetGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crRunNetGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRunNetGameMethod();
	crRunNetGameMethod(const crRunNetGameMethod& handle);
	MethodClass(CRNetApp, RunNetGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	//std::string m_loadingDlgID;
	//std::string m_loadingProgress;
};
class crSetRunSceneIDMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetRunSceneIDMethod();
	crSetRunSceneIDMethod(const crSetRunSceneIDMethod& handle);
	MethodClass(CRNetApp, SetRunSceneID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	short m_sceneid;
};
class crSetLoadingProgressMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetLoadingProgressMethod();
	crSetLoadingProgressMethod(const crSetLoadingProgressMethod& handle);
	MethodClass(CRNetApp, SetLoadingProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_progress;
};
class crRunGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRunGameMethod();
	crRunGameMethod(const crRunGameMethod& handle);
	MethodClass(CRNetApp, RunGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
//protected:
	//std::string m_sceneFile;
	//std::string m_uifile;
	//std::string m_brainScript;
	//std::string m_loadingDlgID;
	//std::string m_loadingProgress;
};
class crRunGame2Method : public CRCore::crMethod
{//GameClient Method
public:
	crRunGame2Method();
	crRunGame2Method(const crRunGame2Method& handle);
	MethodClass(CRNetApp, RunGame2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_loadingDlgID;
//	std::string m_loadingProgress;
};
class crRunNetGame2Method : public CRCore::crMethod
{//GameClient Method
public:
	crRunNetGame2Method();
	crRunNetGame2Method(const crRunNetGame2Method& handle);
	MethodClass(CRNetApp, RunNetGame2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_loadingDlgID;
//	std::string m_loadingProgress;
};
class crPlayerInRangeTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crPlayerInRangeTestMethod();
	crPlayerInRangeTestMethod(const crPlayerInRangeTestMethod& handle);
	MethodClass(CRNetApp, PlayerInRangeTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
	crSightInfo* m_sight;
	//CRCore::ref_ptr<crSceneServerPlayerData> m_player2;
	crRole *m_role2;
};

class crLoadItemMethod : public CRCore::crMethod
{//GameClient Method
public:
	crLoadItemMethod();
	crLoadItemMethod(const crLoadItemMethod& handle);
	MethodClass(CRNetApp, LoadItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crInstanceItem> m_item;
};

class crItemInRangeTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crItemInRangeTestMethod();
	crItemInRangeTestMethod(const crItemInRangeTestMethod& handle);
	MethodClass(CRNetApp, ItemInRangeTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
	crSightInfo *m_sight;
	crInstanceItem *m_item;
};

//class crRoomPlayerInRangeTestMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crRoomPlayerInRangeTestMethod();
//	crRoomPlayerInRangeTestMethod(const crRoomPlayerInRangeTestMethod& handle);
//	MethodClass(CRNetApp, RoomPlayerInRangeTest)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crRole *m_this;
//	CRCore::ref_ptr<crSceneServerPlayerData> m_player;
//	//CRCore::ref_ptr<crSceneServerPlayerData> m_player2;
//	RolePlayerPair *m_rolePlayer2;
//};
//
//class crRoomItemInRangeTestMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crRoomItemInRangeTestMethod();
//	crRoomItemInRangeTestMethod(const crRoomItemInRangeTestMethod& handle);
//	MethodClass(CRNetApp, RoomItemInRangeTest)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crRole *m_this;
//	CRCore::ref_ptr<crSceneServerPlayerData> m_player;
//	CRCore::ref_ptr<crInstanceItem> m_item;
//};

class crLoadRoleDataMethod : public CRCore::crMethod
{//Client Method
public:
	crLoadRoleDataMethod();
	crLoadRoleDataMethod(const crLoadRoleDataMethod& handle);
	MethodClass(CRNetApp, LoadRoleData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crSendToBotMethod : public CRCore::crMethod
{//
public:
	crSendToBotMethod();
	crSendToBotMethod(const crSendToBotMethod& handle);
	MethodClass(CRNetApp, SendToBot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	_crInt64 *m_param;
};
//class crSendToNetMethod : public CRCore::crMethod
//{//
//public:
//	crSendToNetMethod();
//	crSendToNetMethod(const crSendToNetMethod& handle);
//	MethodClass(CRNetApp, SendToNet)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	_crInt64 *m_param;
//};
class crNetPickTargetMethod : public CRCore::crMethod
{//
public:
	crNetPickTargetMethod();
	crNetPickTargetMethod(const crNetPickTargetMethod& handle);
	MethodClass(CRNetApp, NetPickTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
    CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::ref_ptr<CRCore::crNode> m_root;
};
class crNodeMouseEventMethod : public CRCore::crMethod
{//
public:
	crNodeMouseEventMethod();
	crNodeMouseEventMethod(const crNodeMouseEventMethod& handle);
	MethodClass(CRNetApp, NodeMouseEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crNetControlMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crNetControlMethod();
	crNetControlMethod(const crNetControlMethod& handle);
	MethodClass(CRNetApp, NetControl)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	float m_delta;
	float m_attackDelta;//攻击距离校验
};
class crServerItemMoveMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crServerItemMoveMethod();
	crServerItemMoveMethod(const crServerItemMoveMethod& handle);
	MethodClass(CRNetApp, ServerItemMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_taskPointRange;
};
class crRevertUpdateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRevertUpdateMethod();
	crRevertUpdateMethod(const crRevertUpdateMethod& handle);
	MethodClass(CRNetApp, RevertUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
};
class crItemUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crItemUpdateMethod();
	crItemUpdateMethod(const crItemUpdateMethod& handle);
	MethodClass(CRNetApp, ItemUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
};
//crBotCoordToRoleMethod
class crNodeCoordToItemMethod : public CRCore::crMethod
{
public:
	crNodeCoordToItemMethod();
	crNodeCoordToItemMethod(const crNodeCoordToItemMethod& handle);
	MethodClass(CRNetApp, NodeCoordToItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_node;
};
//crRoleCoordToBotMethod
class crItemCoordToNodeMethod : public CRCore::crMethod
{
public:
	crItemCoordToNodeMethod();
	crItemCoordToNodeMethod(const crItemCoordToNodeMethod& handle);
	MethodClass(CRNetApp, ItemCoordToNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crNode* m_node;
	int m_mode;
};
class crNetMoveToTargetMethod : public CRCore::crMethod
{//
public:
	crNetMoveToTargetMethod();
	crNetMoveToTargetMethod(const crNetMoveToTargetMethod& handle);
	MethodClass(CRNetApp, NetMoveToTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void init();
	CRPhysics::crViewMatterObject* m_this;
	CRCore::crVector3 m_targetPosition;

	CRCore::ref_ptr<CRCore::crAITask> m_aiTask;
	float m_velocity;
	//CRCore::Timer_t m_time;
	bool m_init;
};
class crNetMoveToTarget2Method : public CRCore::crMethod
{//
public:
	crNetMoveToTarget2Method();
	crNetMoveToTarget2Method(const crNetMoveToTarget2Method& handle);
	MethodClass(CRNetApp, NetMoveToTarget2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void init();
	CRPhysics::crViewMatterObject* m_this;
	CRCore::crVector3 m_targetPosition;

	CRCore::ref_ptr<CRCore::crAITask> m_aiTask;
	float m_velocity;
	bool m_init;
};
class crCreateRoleMethod : public CRCore::crMethod
{
public:
	crCreateRoleMethod();
	crCreateRoleMethod(const crCreateRoleMethod& handle);
	MethodClass(CRNetApp, CreateRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_metierElement;
	std::string m_botElement;
	std::string m_roleNameElement;
};
class crCreateRole2Method : public CRCore::crMethod
{
public:
	crCreateRole2Method();
	crCreateRole2Method(const crCreateRole2Method& handle);
	MethodClass(CRNetApp, CreateRole2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_metierWidget;
	std::string m_roleNameWidget;
	int m_metierid;
};
class crQueryGameRoleListMethod : public CRCore::crMethod
{
public:
	crQueryGameRoleListMethod();
	crQueryGameRoleListMethod(const crQueryGameRoleListMethod& handle);
	MethodClass(CRNetApp, QueryGameRoleList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

//class crWaitStartGameMethod : public CRCore::crMethod
//{//
//public:
//	crWaitStartGameMethod();
//	crWaitStartGameMethod(const crWaitStartGameMethod& handle);
//	MethodClass(CRNetApp, WaitStartGame)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	static void startGame(char code);
//protected:
//	//static CRCore::crCondition s_startGameCondition;
//	//static CRCore::crMutex s_startGameMutex;
//	static CRCore::crBarrier s_startGameBarrier;
//	static char s_code;
//};
//class crStartGameMethod : public CRCore::crMethod
//{//
//public:
//	crStartGameMethod();
//	crStartGameMethod(const crStartGameMethod& handle);
//	MethodClass(CRNetApp, StartGame)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	char m_code;
//};

class crNetMouseControlViewMethod : public CRCore::crMethod
{
public:
	crNetMouseControlViewMethod();
	crNetMouseControlViewMethod(const crNetMouseControlViewMethod& handle);
	MethodClass(CRNetApp, NetMouseControlView)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	enum RotMode
	{
		XRot = 0x1,
		YRot = 0x2,
		XYRot = XRot|YRot
	};
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	
	CRCore::crVector2f m_turn_angle;
	float m_time;
	RotMode m_rotMode;
};

class crNetStopMoveMethod : public CRCore::crMethod
{
public:
	crNetStopMoveMethod();
	crNetStopMoveMethod(const crNetStopMoveMethod& handle);
	MethodClass(CRNetApp, NetStopMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crRecvStopMoveMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvStopMoveMethod();
	crRecvStopMoveMethod(const crRecvStopMoveMethod& handle);
	MethodClass(CRNetApp, RecvStopMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//crRecvItemRTDataMethod
class crRecvItemRTDataMethod : public CRCore::crMethod
{//
public:
	crRecvItemRTDataMethod();
	crRecvItemRTDataMethod(const crRecvItemRTDataMethod& handle);
	MethodClass(CRNetApp, RecvItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	bool m_staticRoot;
};
class crRecvItemChildDataMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvItemChildDataMethod();
	crRecvItemChildDataMethod(const crRecvItemChildDataMethod& handle);
	MethodClass(CRNetApp, RecvItemChildData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crNodeInRangeMethod : public CRCore::crMethod
{//
public:
	crNodeInRangeMethod();
	crNodeInRangeMethod(const crNodeInRangeMethod& handle);
	MethodClass(CRNetApp, NodeInRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	short m_frameDelay;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
};

class crNodeOutRangeMethod : public CRCore::crMethod
{//
public:
	crNodeOutRangeMethod();
	crNodeOutRangeMethod(const crNodeOutRangeMethod& handle);
	MethodClass(CRNetApp, NodeOutRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	short m_frameDelay;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
};

class crRecvCreateItemChildMethod : public CRCore::crMethod
{//
public:
	crRecvCreateItemChildMethod();
	crRecvCreateItemChildMethod(const crRecvCreateItemChildMethod& handle);
	MethodClass(CRNetApp, RecvCreateItemChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};

class crLoadItemChildMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crLoadItemChildMethod();
	crLoadItemChildMethod(const crLoadItemChildMethod& handle);
	MethodClass(CRNetApp, LoadItemChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crItemChild> m_itemchild;
	CRCore::ref_ptr<crInstanceItem> m_rootitem;
};
class crUseItemCheckMethod : public CRCore::crMethod
{//Client & SceneServer Method
public:
	crUseItemCheckMethod();
	crUseItemCheckMethod(const crUseItemCheckMethod& handle);
	MethodClass(CRNetApp, UseItemCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};
class crUseItemMethod : public CRCore::crMethod
{//Client Method
public:
	crUseItemMethod();
	crUseItemMethod(const crUseItemMethod& handle);
	MethodClass(CRNetApp, UseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};

class crServerUseItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crServerUseItemMethod();
	crServerUseItemMethod(const crServerUseItemMethod& handle);
	MethodClass(CRNetApp, ServerUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	UseItemParam* m_useItemParam;
	unsigned short* m_useResult;
};

class crNetUseItemMethod : public CRCore::crMethod
{//Client Method
public:
	crNetUseItemMethod();
	crNetUseItemMethod(const crNetUseItemMethod& handle);
	MethodClass(CRNetApp, NetUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	UseItemParam* m_useItemParam;
	unsigned char m_usercdid;
	float m_actdt;
};

class crNetServerUseItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crNetServerUseItemMethod();
	crNetServerUseItemMethod(const crNetServerUseItemMethod& handle);
	MethodClass(CRNetApp, NetServerUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	UseItemParam* m_useItemParam;
};
class crCheckCurrentAttackWeaponMethod : public CRCore::crMethod
{//Client Method
public:
	crCheckCurrentAttackWeaponMethod();
	crCheckCurrentAttackWeaponMethod(const crCheckCurrentAttackWeaponMethod& handle);
	MethodClass(CRNetApp, CheckCurrentAttackWeapon)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crItemUseMethod : public CRCore::crMethod
{//Client Method
public:
	crItemUseMethod();
	crItemUseMethod(const crItemUseMethod& handle);
	MethodClass(CRNetApp, ItemUse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	//ItemUsePair *m_itemUsePair;
	CRCore::ref_ptr<ItemUseParam> m_itemUseParam;
};

class crBindWeaponMatrixMethod : public CRCore::crMethod
{//Client Method
public:
	crBindWeaponMatrixMethod();
	crBindWeaponMatrixMethod(const crBindWeaponMatrixMethod& handle);
	MethodClass(CRNetApp, BindWeaponMatrix)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_bot;
	CRCore::crVector3 m_offset;
	CRCore::crVector3 m_scale;
	float m_rotAngle;
	std::string m_boneName;
	CRCore::crVector3 m_dir;
};
class crUnBindWeaponMatrixMethod : public CRCore::crMethod
{//Client Method
public:
	crUnBindWeaponMatrixMethod();
	crUnBindWeaponMatrixMethod(const crUnBindWeaponMatrixMethod& handle);
	MethodClass(CRNetApp, UnBindWeaponMatrix)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crWeaponMatterObject* m_this;
};
class crShowAimUIMethod : public CRCore::crMethod
{//Client Method
public:
	crShowAimUIMethod();
	crShowAimUIMethod(const crShowAimUIMethod& handle);
	MethodClass(CRNetApp, ShowAimUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRPhysics::crWeaponMatterObject> m_weapon;
	bool m_show;
	std::string m_aimui;
};
class crBotCollideMethod : public CRCore::crMethod
{//Client Method
public:
	crBotCollideMethod();
	crBotCollideMethod(const crBotCollideMethod& handle);
	MethodClass(CRNetApp, BotCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::crNode* m_collideNode;
	dContact *m_contactInfo;
	float m_hurtActTime;
};
//在玩家被技能或者使用物品击中,在SceneServer端计算作用效果及伤害,并反馈回客户端.
class crNodeCollideWithItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crNodeCollideWithItemMethod();
	crNodeCollideWithItemMethod(const crNodeCollideWithItemMethod& handle);
	MethodClass(CRNetApp, NodeCollideWithItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crDamageCollideFormula1Method : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害公式1，base+lv*lvscalar+parent-defend
	crDamageCollideFormula1Method();
	crDamageCollideFormula1Method(const crDamageCollideFormula1Method& handle);
	MethodClass(CRNetApp, DamageCollideFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<HitParam> m_hitParam;
    unsigned char m_dmtype;
	float m_lvfactor;
};

class crGetDamageFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_attack+力量*1.1*（0.16*LV+4）+装备加成
	crGetDamageFormula1Method();
	crGetDamageFormula1Method(const crGetDamageFormula1Method& handle);
	MethodClass(CRNetApp, GetDamageFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3 m_factor;
	//CRCore::crVector2i m_dice;
	float *m_output;
};
class crGetDamageFormula2Method : public CRCore::crMethod
{//SceneServer & Client Method
public://装备附加MagicDM
	crGetDamageFormula2Method();
	crGetDamageFormula2Method(const crGetDamageFormula2Method& handle);
	MethodClass(CRNetApp, GetDamageFormula2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_formulaid;
	float *m_output;
};
class crGetDefendFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_defend+敏捷*1.1*（0.16*LV+4）+装备加成
	crGetDefendFormula1Method();
	crGetDefendFormula1Method(const crGetDefendFormula1Method& handle);
	MethodClass(CRNetApp, GetDefendFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3 m_factor;
	float *m_output;
};
class crGetDefendFormula2Method : public CRCore::crMethod
{//SceneServer & Client Method
public://装备附加MagicDefend
	crGetDefendFormula2Method();
	crGetDefendFormula2Method(const crGetDefendFormula2Method& handle);
	MethodClass(CRNetApp, GetDefendFormula2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_formulaid;
	float *m_output;
};
class crGetSpeedFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_speed+敏捷*m_factor+装备加成
	crGetSpeedFormula1Method();
	crGetSpeedFormula1Method(const crGetSpeedFormula1Method& handle);
	MethodClass(CRNetApp, GetSpeedFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_factor;
	float *m_output;
};
class crGetHPFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_hp+力量*1.1*（LV*0.8+20）+装备加成
	crGetHPFormula1Method();
	crGetHPFormula1Method(const crGetHPFormula1Method& handle);
	MethodClass(CRNetApp, GetHPFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3 m_factor;
	float *m_output;
};
class crGetMPFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_mp+智力*0.6（0.8*LV+20）+装备加成
	crGetMPFormula1Method();
	crGetMPFormula1Method(const crGetMPFormula1Method& handle);
	MethodClass(CRNetApp, GetMPFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3 m_factor;
	float *m_output;
};
class crGetHPRevertFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://rthp*m_factor+装备加成
	crGetHPRevertFormula1Method();
	crGetHPRevertFormula1Method(const crGetHPRevertFormula1Method& handle);
	MethodClass(CRNetApp, GetHPRevertFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_factor;
	float *m_output;
};
class crGetMPRevertFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://rtmp*scalar+装备加成
	crGetMPRevertFormula1Method();
	crGetMPRevertFormula1Method(const crGetMPRevertFormula1Method& handle);
	MethodClass(CRNetApp, GetMPRevertFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_factor;
	float *m_output;
};
class crAttackSpeedFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://m_attackSpeed-敏捷*0.1*（0.16*LV+0.1）-装备加成
	crAttackSpeedFormula1Method();
	crAttackSpeedFormula1Method(const crAttackSpeedFormula1Method& handle);
	MethodClass(CRNetApp, AttackSpeedFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3 m_factor;
	float *m_output;
};

class crAddDamageFormula1Method : public CRCore::crMethod
{//SceneServer Method
public:
	crAddDamageFormula1Method();
	crAddDamageFormula1Method(const crAddDamageFormula1Method& handle);
	MethodClass(CRNetApp, AddDamageFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_damage;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
};

class crRecvDamageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvDamageMethod();
	crRecvDamageMethod(const crRecvDamageMethod& handle);
	MethodClass(CRNetApp, RecvDamage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crRecvItemDeadMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvItemDeadMethod();
	crRecvItemDeadMethod(const crRecvItemDeadMethod& handle);
	MethodClass(CRNetApp, RecvItemDead)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crShowStateTextMethod : public CRCore::crNodeMethod
{//GameClient Method
public:
	crShowStateTextMethod();
	crShowStateTextMethod(const crShowStateTextMethod& handle);
	MethodClass(CRNetApp, ShowStateText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);

	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRPhysics::crViewMatterObject* m_this;
	short m_value;
	short m_index;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
	CRCore::crVector2 m_zRange;
	float m_riseSpeed;
	float m_maxRange;
	//text属性
	float m_characterSize;
	unsigned char m_alignmentType;
	CRCore::crVector4 m_color;
	std::string m_font;
	//
	std::string m_prestr;
	float m_visiableTime;//0,上升到m_zRange[1]后消失,否则计时消失
	bool m_textRectSetted;//
	CRCore::crVector4 m_textRect;//如果未指定,就显示在BOT头顶
	typedef std::pair< CRCore::ref_ptr<CRText::crText>,CRCore::crVector2 > TextPair;//text,rize,timer
	typedef std::vector< TextPair > TextVec;
	mutable TextVec m_textVec;
	_crInt32 m_msg;
};

class crShowHeadTextMethod : public CRCore::crNodeMethod
{//GameClient Method
public:
	crShowHeadTextMethod();
	crShowHeadTextMethod(const crShowHeadTextMethod& handle);
	MethodClass(CRNetApp, ShowHeadText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);

	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crMatrixTransform* m_this;
	float m_maxRange;
	float m_zoffset;
	//text属性
	float m_characterSize;
	unsigned char m_alignmentType;
	CRCore::crVector4 m_color;
	std::string m_font;
	//
	mutable CRCore::ref_ptr<CRText::crText> m_text;
};
class crLoadAndCloneNodeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crLoadAndCloneNodeMethod();
	crLoadAndCloneNodeMethod(const crLoadAndCloneNodeMethod& handle);
	MethodClass(CRNetApp, LoadAndCloneNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_dataFile;
	std::string *m_addDescription;
	unsigned short m_copyop;
};
class crCreateDecalMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCreateDecalMethod();
	crCreateDecalMethod(const crCreateDecalMethod& handle);
	MethodClass(CRNetApp, CreateDecal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_textureFile;
	crInstanceItem *m_Ithis;
	std::string m_createNodeName;
};
class crCreateLightMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCreateLightMethod();
	crCreateLightMethod(const crCreateLightMethod& handle);
	MethodClass(CRNetApp, CreateLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	unsigned char m_lightType;
	CRCore::crVector3 m_ambient;
	CRCore::crVector3 m_diffuse;
	CRCore::crVector3 m_specular;
	CRCore::crVector3 m_position;
	CRCore::crVector3 m_direction;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;
	float m_spotExponent;
	float m_spotCutoff;
	float m_lowestLum;
};
class crLoadParticleMethod : public CRCore::crMethod
{//
public:
	crLoadParticleMethod();
	crLoadParticleMethod(const crLoadParticleMethod& handle);
	MethodClass(CRNetApp, LoadParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_dataFile;
};
class crLoadSoundMethod : public CRCore::crMethod
{//
public:
	crLoadSoundMethod();
	crLoadSoundMethod(const crLoadSoundMethod& handle);
	MethodClass(CRNetApp, LoadSound)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_dataFile;
	bool m_stream;
};
class crInPatrolTestMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crInPatrolTestMethod();
	crInPatrolTestMethod(const crInPatrolTestMethod& handle);
	MethodClass(CRNetApp, InPatrolTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	//_crInt64 m_distEnmity;
	int m_dist;
};
class crCalcEnmityByDistRateMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crCalcEnmityByDistRateMethod();
	crCalcEnmityByDistRateMethod(const crCalcEnmityByDistRateMethod& handle);
	MethodClass(CRNetApp, CalcEnmityByDistRate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_distFactor;
	float m_distRate;
};
//class crGetEnemy1Method : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crGetEnemy1Method();
//	crGetEnemy1Method(const crGetEnemy1Method& handle);
//	MethodClass(CRNetApp, GetEnemy1)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//};
//class crSelectSkill1Method : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crSelectSkill1Method();
//	crSelectSkill1Method(const crSelectSkill1Method& handle);
//	MethodClass(CRNetApp, SelectSkill1)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<crInstanceItem> m_item;
//};
class crPatrolMethod : public CRCore::crMethod
{//SceneServer
public:
	crPatrolMethod();
	crPatrolMethod(const crPatrolMethod& handle);
	MethodClass(CRNetApp, Patrol)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_taskPointRange;
};
class crRecvPatrolMethod : public CRCore::crMethod
{//SceneServer & Client
public:
	crRecvPatrolMethod();
	crRecvPatrolMethod(const crRecvPatrolMethod& handle);
	MethodClass(CRNetApp, RecvPatrol)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crHideBodyMethod : public CRCore::crMethod
{//GameClient Method
public:
	crHideBodyMethod();
	crHideBodyMethod(const crHideBodyMethod& handle);
	MethodClass(CRNetApp, HideBody)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
    bool m_visiable;
	bool m_intersectable;
};

class crRecvTargetExtraMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvTargetExtraMethod();
	crRecvTargetExtraMethod(const crRecvTargetExtraMethod& handle);
	MethodClass(CRNetApp, RecvTargetExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvUserExtraMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvUserExtraMethod();
	crRecvUserExtraMethod(const crRecvUserExtraMethod& handle);
	MethodClass(CRNetApp, RecvUserExtra)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crExtraUpdateMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crExtraUpdateMethod();
	crExtraUpdateMethod(const crExtraUpdateMethod& handle);
	MethodClass(CRNetApp, ExtraUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	_crInt64 m_param;
	float m_time;
	float m_interval;
};
class CRNETAPP_EXPORT crExtraLifeTimeMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crExtraLifeTimeMethod();
	crExtraLifeTimeMethod(const crExtraLifeTimeMethod& handle);
	MethodClass(CRNetApp, ExtraLifeTime)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	float getRemainTime(){ return m_timer; }//0.0f表示无穷
protected:
	crInstanceItem* m_this;
	float *m_dt;
	bool* m_output;
	float m_duration;//0.0f表示无穷

	bool m_start;
	float m_timer;
	short m_overlapCount;
};
//class crExtraDefendMethod : public CRCore::crMethod
//{//SceneServer & GameClient Method
//public:
//	crExtraDefendMethod();
//	crExtraDefendMethod(const crExtraDefendMethod& handle);
//	MethodClass(CRNetApp, ExtraDefend)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	float *m_dt;
//	bool* m_output;
//	float m_duration;
//	short m_extraValue;
//	
//	bool m_start;
//	float m_timer;
//};
//class crExtraGuiseMethod : public CRCore::crMethod
//{//SceneServer & GameClient Method
//public:
//	crExtraGuiseMethod();
//	crExtraGuiseMethod(const crExtraGuiseMethod& handle);
//	MethodClass(CRNetApp, ExtraGuise)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	float *m_dt;
//	bool *m_output;
//	float m_duration;
//	unsigned int m_guise;
//
//	bool m_start;
//	float m_timer;
//};
class crExtraRadiusMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crExtraRadiusMethod();
	crExtraRadiusMethod(const crExtraRadiusMethod& handle);
	MethodClass(CRNetApp, ExtraRadius)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float *m_dt;
	bool *m_output;
	float m_duration;
	char m_radius;

	bool m_start;
	float m_timer;
};
class crEnableBulletVolumeMethod : public CRCore::crMethod
{//Client Method
public:
	crEnableBulletVolumeMethod();
	crEnableBulletVolumeMethod(const crEnableBulletVolumeMethod& handle);
	MethodClass(CRNetApp, EnableBulletVolume)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crVolumeNode* m_this;
    bool m_enable;
};
class crNodeInBulletVolumeMethod : public CRCore::crMethod
{//Client Method
public:
	crNodeInBulletVolumeMethod();
	crNodeInBulletVolumeMethod(const crNodeInBulletVolumeMethod& handle);
	MethodClass(CRNetApp, NodeInBulletVolume)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crVolumeNode* m_this;
	CRCore::ref_ptr<CRPhysics::crMatterObject> m_inVolumeNode;
	bool *m_isvalid;
	unsigned char m_targetType;
};
//class crCloseTargetItemMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crCloseTargetItemMethod();
//	crCloseTargetItemMethod(const crCloseTargetItemMethod& handle);
//	MethodClass(CRNetApp, CloseTargetItem)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	float m_dt;
//	float m_interval;
//	float m_time;
//};
class crServerCloseTargetItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crServerCloseTargetItemMethod();
	crServerCloseTargetItemMethod(const crServerCloseTargetItemMethod& handle);
	MethodClass(CRNetApp, ServerCloseTargetItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_interval;
	float m_time;
};
class crRecvCloseTargetMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvCloseTargetMethod();
	crRecvCloseTargetMethod(const crRecvCloseTargetMethod& handle);
	MethodClass(CRNetApp, RecvCloseTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUseItemAndSendMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crUseItemAndSendMethod();
	crUseItemAndSendMethod(const crUseItemAndSendMethod& handle);
	MethodClass(CRNetApp, UseItemAndSend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_itemid;
	bool m_isAboutToUseItem;
};
//class crNetControl2Method : public CRCore::crMethod
//{//Server & Client Method
//public:
//	crNetControl2Method();
//	crNetControl2Method(const crNetControl2Method& handle);
//	MethodClass(CRNetApp, NetControl2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crGetItemRTDataMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crGetItemRTDataMethod();
	crGetItemRTDataMethod(const crGetItemRTDataMethod& handle);
	MethodClass(CRNetApp, GetItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> *m_outstream;
};
class crGetItemChildDataMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crGetItemChildDataMethod();
	crGetItemChildDataMethod(const crGetItemChildDataMethod& handle);
	MethodClass(CRNetApp, GetItemChildData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> *m_outstream;
	int m_itemid;
};
class crNpcDeadMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crNpcDeadMethod();
	crNpcDeadMethod(const crNpcDeadMethod& handle);
	MethodClass(CRNetApp, NpcDead)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_killerItem;
};
class crAddExperienceMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crAddExperienceMethod();
	crAddExperienceMethod(const crAddExperienceMethod& handle);
	MethodClass(CRNetApp, AddExperience)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_experience;
};
class crAddExperience2Method : public CRCore::crMethod
{//SceneServer Method RoomGame
public:
	crAddExperience2Method();
	crAddExperience2Method(const crAddExperience2Method& handle);
	MethodClass(CRNetApp, AddExperience2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData* m_this;
	int m_experience;
};
class crAddGoldMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crAddGoldMethod();
	crAddGoldMethod(const crAddGoldMethod& handle);
	MethodClass(CRNetApp, AddGold)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_gold;
};
class crAddGold2Method : public CRCore::crMethod
{//SceneServer Method RoomGame
public:
	crAddGold2Method();
	crAddGold2Method(const crAddGold2Method& handle);
	MethodClass(CRNetApp, AddGold2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData* m_this;
	int m_gold;
};
class crRecvExperienceMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvExperienceMethod();
	crRecvExperienceMethod(const crRecvExperienceMethod& handle);
	MethodClass(CRNetApp, RecvExperience)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvExperience2Method : public CRCore::crMethod
{//Client Method
public:
	crRecvExperience2Method();
	crRecvExperience2Method(const crRecvExperience2Method& handle);
	MethodClass(CRNetApp, RecvExperience2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvGoldMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvGoldMethod();
	crRecvGoldMethod(const crRecvGoldMethod& handle);
	MethodClass(CRNetApp, RecvGold)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvGold2Method : public CRCore::crMethod
{//Client Method
public:
	crRecvGold2Method();
	crRecvGold2Method(const crRecvGold2Method& handle);
	MethodClass(CRNetApp, RecvGold2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crLvUpFormula1Method : public CRCore::crMethod
{//SceneServer Method
public://ilv*ilv*ilv*m_factor[0]+ilv*m_factor[1]+m_factor[2];
	crLvUpFormula1Method();
	crLvUpFormula1Method(const crLvUpFormula1Method& handle);
	MethodClass(CRNetApp, LvUpFormula1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3i m_factor;
	unsigned char m_attrPointGrow;
	unsigned char m_skillPointGrow;
};
class crRecvLvUpMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvLvUpMethod();
	crRecvLvUpMethod(const crRecvLvUpMethod& handle);
	MethodClass(CRNetApp, RecvLvUp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crDeadEventMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crDeadEventMethod();
	crDeadEventMethod(const crDeadEventMethod& handle);
	MethodClass(CRNetApp, DeadEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_corpseInterval;
	float m_reliveInterval;//<0表示不会自动复活
	float m_timer;
private:
	int m_step;
};
class crSinkMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSinkMethod();
	crSinkMethod(const crSinkMethod& handle);
	MethodClass(CRNetApp, Sink)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	float m_sinkSpeed;
};
class crResetBotMethod : public CRCore::crMethod
{//GameClient Method
public:
	crResetBotMethod();
	crResetBotMethod(const crResetBotMethod& handle);
	MethodClass(CRNetApp, ResetBot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
};
class crUIRelivePlayerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIRelivePlayerMethod();
	crUIRelivePlayerMethod(const crUIRelivePlayerMethod& handle);
	MethodClass(CRNetApp, UIRelivePlayer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
public:
	unsigned char m_reliveFlg;
};
class crRecvRelivePlayerMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvRelivePlayerMethod();
	crRecvRelivePlayerMethod(const crRecvRelivePlayerMethod& handle);
	MethodClass(CRNetApp, RecvRelivePlayer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
public:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crSetKeyboardMouseModeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetKeyboardMouseModeMethod();
	crSetKeyboardMouseModeMethod(const crSetKeyboardMouseModeMethod& handle);
	MethodClass(CRNetApp, SetKeyboardMouseMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
public:
	crRole* m_this;
	unsigned short m_mode;
};
class crBulletCollideTestMethod : public CRCore::crMethod
{//Client Method
public:
	crBulletCollideTestMethod();
	crBulletCollideTestMethod(const crBulletCollideTestMethod& handle);
	MethodClass(CRNetApp, BulletCollideTest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crBulletMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_collideNode;
	int *m_collideTest;
	unsigned char m_targetType;
};
//class crEnemyCheckMethod : public CRCore::crMethod
//{//SceneServer & GameClient Method
//public:
//	crEnemyCheckMethod();
//	crEnemyCheckMethod(const crEnemyCheckMethod& handle);
//	MethodClass(CRNetApp, EnemyCheck)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<crInstanceItem> m_targetItem;
//	char *m_isEnemy;//-1:敌人，0中立，1队友
//};
class crEnemyCheckMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crEnemyCheckMethod();
	crEnemyCheckMethod(const crEnemyCheckMethod& handle);
	MethodClass(CRNetApp, EnemyCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_targetItem;
	char *m_isEnemy;//-1:敌人，0中立，1队友
};
class crRequestItemRTDataMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRequestItemRTDataMethod();
	crRequestItemRTDataMethod(const crRequestItemRTDataMethod& handle);
	MethodClass(CRNetApp, RequestItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
};
//class crGetRoleZMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crGetRoleZMethod();
//	crGetRoleZMethod(const crGetRoleZMethod& handle);
//	MethodClass(CRNetApp, GetRoleZ)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
class crTransportCollideMethod : public CRCore::crMethod
{//Client Method
public:
	crTransportCollideMethod();
	crTransportCollideMethod(const crTransportCollideMethod& handle);
	MethodClass(CRNetApp, TransportCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	CRCore::ref_ptr<CRCore::crNode> m_collideNode;
	dContact *m_contactInfo;

	int m_sceneid;
	unsigned short m_layerid;
	CRCore::crVector2 m_coord;
};
class crRecvTransportMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvTransportMethod();
	crRecvTransportMethod(const crRecvTransportMethod& handle);
	MethodClass(CRNetApp, RecvTransport)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crExitSceneMethod : public CRCore::crMethod
{//GameClient Method
public:
	crExitSceneMethod();
	crExitSceneMethod(const crExitSceneMethod& handle);
	MethodClass(CRNetApp, ExitScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_exitCode;
};
class crUIExitSceneMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIExitSceneMethod();
	crUIExitSceneMethod(const crUIExitSceneMethod& handle);
	MethodClass(CRNetApp, UIExitScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_exitCode;
};
class crDisconnectSceneMethod : public CRCore::crMethod
{//GameClient Method
public:
	crDisconnectSceneMethod();
	crDisconnectSceneMethod(const crDisconnectSceneMethod& handle);
	MethodClass(CRNetApp, DisconnectScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crParseExitCodeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crParseExitCodeMethod();
	crParseExitCodeMethod(const crParseExitCodeMethod& handle);
	MethodClass(CRNetApp, ParseExitCode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crParseExitCode2Method : public CRCore::crMethod
{//GameClient Method
public:
	crParseExitCode2Method();
	crParseExitCode2Method(const crParseExitCode2Method& handle);
	MethodClass(CRNetApp, ParseExitCode2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crShowStatsMethod : public CRCore::crMethod
{//GameClient Method
public:
	crShowStatsMethod();
	crShowStatsMethod(const crShowStatsMethod& handle);
	MethodClass(CRNetApp, ShowStats)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_show;
};
class crSnapImageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSnapImageMethod();
	crSnapImageMethod(const crSnapImageMethod& handle);
	MethodClass(CRNetApp, SnapImage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crRecordMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecordMethod();
	crRecordMethod(const crRecordMethod& handle);
	MethodClass(CRNetApp, Record)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crUIUpdateRoleMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateRoleMethod();
	crUIUpdateRoleMethod(const crUIUpdateRoleMethod& handle);
	MethodClass(CRNetApp, UIUpdateRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crImageStage* m_this;
	std::string m_name;
	std::string m_lv;
	std::string m_lvAdd;
	std::string m_exp;
	std::string m_gold;
	std::string m_hp;
	std::string m_mp;
	std::string m_attack;
	std::string m_defend;
	std::string m_strength;
	std::string m_strengthAdd;
	std::string m_agility;
	std::string m_agilityAdd;
	std::string m_intellect;
	std::string m_intellectAdd;
	std::string m_attrpoint;
	std::string m_skillpoint;
	std::string m_attrok;
};
class crGetNextLvExpMethod : public CRCore::crMethod
{//GameClient Method
public:
	crGetNextLvExpMethod();
	crGetNextLvExpMethod(const crGetNextLvExpMethod& handle);
	MethodClass(CRNetApp, GetNextLvExp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector3i m_factor;
	int *m_output;
};
class crItemDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crItemDoEventMethod();
	crItemDoEventMethod(const crItemDoEventMethod& handle);
	MethodClass(CRNetApp, ItemDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crNodeDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crNodeDoEventMethod();
	crNodeDoEventMethod(const crNodeDoEventMethod& handle);
	MethodClass(CRNetApp, NodeDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crMyRoleDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crMyRoleDoEventMethod();
	crMyRoleDoEventMethod(const crMyRoleDoEventMethod& handle);
	MethodClass(CRNetApp, MyRoleDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_roleindex;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crMyMetierEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crMyMetierEventMethod();
	crMyMetierEventMethod(const crMyMetierEventMethod& handle);
	MethodClass(CRNetApp, MyMetierEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crGetTaskMethod : public CRCore::crMethod
{//GameClient & SceneServer Method 
public:
	crGetTaskMethod();
	crGetTaskMethod(const crGetTaskMethod& handle);
	MethodClass(CRNetApp, GetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
};
class crAdvanceTaskMethod : public CRCore::crMethod
{//GameClient & SceneServer Method 
public:
	crAdvanceTaskMethod();
	crAdvanceTaskMethod(const crAdvanceTaskMethod& handle);
	MethodClass(CRNetApp, AdvanceTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	int m_taskid;
	unsigned char m_stepType;
	bool m_noStreamLock;
};
class crReferTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crReferTaskMethod();
	crReferTaskMethod(const crReferTaskMethod& handle);
	MethodClass(CRNetApp, ReferTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
};
class crKillerMetierEventMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crKillerMetierEventMethod();
	crKillerMetierEventMethod(const crKillerMetierEventMethod& handle);
	MethodClass(CRNetApp, KillerMetierEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crRole> m_killerRole;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crRecvGetTaskMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvGetTaskMethod();
	crRecvGetTaskMethod(const crRecvGetTaskMethod& handle);
	MethodClass(CRNetApp, RecvGetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvAdvanceTaskMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvAdvanceTaskMethod();
	crRecvAdvanceTaskMethod(const crRecvAdvanceTaskMethod& handle);
	MethodClass(CRNetApp, RecvAdvanceTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	bool m_noStreamLock;
};
class crTaskAwardMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crTaskAwardMethod();
	crTaskAwardMethod(const crTaskAwardMethod& handle);
	MethodClass(CRNetApp, TaskAward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<crGameTask> m_gameTask;
	bool *m_output;
};
class crRecvTaskAwardItemMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvTaskAwardItemMethod();
	crRecvTaskAwardItemMethod(const crRecvTaskAwardItemMethod& handle);
	MethodClass(CRNetApp, RecvTaskAwardItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvReferTaskMethod : public CRCore::crMethod
{//SceneServer && GameClient Method 
public:
	crRecvReferTaskMethod();
	crRecvReferTaskMethod(const crRecvReferTaskMethod& handle);
	MethodClass(CRNetApp, RecvReferTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvCorrectPositionMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crRecvCorrectPositionMethod();
	crRecvCorrectPositionMethod(const crRecvCorrectPositionMethod& handle);
	MethodClass(CRNetApp, RecvCorrectPosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvAbstractItemMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvAbstractItemMethod();
	crRecvAbstractItemMethod(const crRecvAbstractItemMethod& handle);
	MethodClass(CRNetApp, RecvAbstractItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
//class crDropItemUpdateMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crDropItemUpdateMethod();
//	crDropItemUpdateMethod(const crDropItemUpdateMethod& handle);
//	MethodClass(CRNetApp, DropItemUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	float m_dt;
//	float m_intevel;
//	float m_time;
//};
class crNetPickItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crNetPickItemMethod();
	crNetPickItemMethod(const crNetPickItemMethod& handle);
	MethodClass(CRNetApp, NetPickItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crRecvNetPickItemMethod : public CRCore::crMethod
{//SceneServer && GameClient Method
public:
	crRecvNetPickItemMethod();
	crRecvNetPickItemMethod(const crRecvNetPickItemMethod& handle);
	MethodClass(CRNetApp, RecvNetPickItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crInsertItemToBackPackMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 
public:
	crInsertItemToBackPackMethod();
	crInsertItemToBackPackMethod(const crInsertItemToBackPackMethod& handle);
	MethodClass(CRNetApp, InsertItemToBackPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	char *m_output;
};
class crUISaleItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUISaleItemMethod();
	crUISaleItemMethod(const crUISaleItemMethod& handle);
	MethodClass(CRNetApp, UISaleItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_backPackControl;
};
class crUISaleItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUISaleItem2Method();
	crUISaleItem2Method(const crUISaleItem2Method& handle);
	MethodClass(CRNetApp, UISaleItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
//class crUISaleCountItemMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crUISaleCountItemMethod();
//	crUISaleCountItemMethod(const crUISaleCountItemMethod& handle);
//	MethodClass(CRNetApp, UISaleCountItem)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//	std::string m_saleCountWidget;
//	std::string m_priceWidget;
//};
class crRecvSaleItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvSaleItemMethod();
	crRecvSaleItemMethod(const crRecvSaleItemMethod& handle);
	MethodClass(CRNetApp, RecvSaleItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
    int m_netType;
};
class crUIRemoveItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIRemoveItemMethod();
	crUIRemoveItemMethod(const crUIRemoveItemMethod& handle);
	MethodClass(CRNetApp, UIRemoveItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_backPackControl;
};
class crUIRemoveItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIRemoveItem2Method();
	crUIRemoveItem2Method(const crUIRemoveItem2Method& handle);
	MethodClass(CRNetApp, UIRemoveItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRemoveItemFromBackPackMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 
public:
	crRemoveItemFromBackPackMethod();
	crRemoveItemFromBackPackMethod(const crRemoveItemFromBackPackMethod& handle);
	MethodClass(CRNetApp, RemoveItemFromBackPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	char *m_output;//返回-1失败，0减数量，1删除
};
class crGetItemCountInBackPackMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crGetItemCountInBackPackMethod();
	crGetItemCountInBackPackMethod(const crGetItemCountInBackPackMethod& handle);
	MethodClass(CRNetApp, GetItemCountInBackPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	short *m_output;//返回数量
};
class crBuildBackPackListMethod : public CRCore::crMethod
{//
public:
	crBuildBackPackListMethod();
	crBuildBackPackListMethod(const crBuildBackPackListMethod& handle);
	MethodClass(CRNetApp, BuildBackPackList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_backPackList;
};
class crBuildBackPackList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crBuildBackPackList2Method();
	crBuildBackPackList2Method(const crBuildBackPackList2Method& handle);
	MethodClass(CRNetApp, BuildBackPackList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class crWaitNetReturnStreamLockMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crWaitNetReturnStreamLockMethod();
	crWaitNetReturnStreamLockMethod(const crWaitNetReturnStreamLockMethod& handle);
	MethodClass(CRNetApp, WaitNetReturnStreamLock)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crUIDropItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIDropItemMethod();
	crUIDropItemMethod(const crUIDropItemMethod& handle);
	MethodClass(CRNetApp, UIDropItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_backPackControl;
};
class crUIDropItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIDropItem2Method();
	crUIDropItem2Method(const crUIDropItem2Method& handle);
	MethodClass(CRNetApp, UIDropItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crRecvDropItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvDropItemMethod();
	crRecvDropItemMethod(const crRecvDropItemMethod& handle);
	MethodClass(CRNetApp, RecvDropItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	CRCore::rangei m_dropRangei;
};
class crBuildSaleItemListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBuildSaleItemListMethod();
	crBuildSaleItemListMethod(const crBuildSaleItemListMethod& handle);
	MethodClass(CRNetApp, BuildSaleItemList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_saleItemList;
};
class crBuildSaleItemList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crBuildSaleItemList2Method();
	crBuildSaleItemList2Method(const crBuildSaleItemList2Method& handle);
	MethodClass(CRNetApp, BuildSaleItemList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_canvasName;
	std::string m_saleItemList;
};
class crUIShowPriceMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIShowPriceMethod();
	crUIShowPriceMethod(const crUIShowPriceMethod& handle);
	MethodClass(CRNetApp, UIShowPrice)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_itemListElement;
	std::string m_priceText;
	char m_flg;//0,售价 1,回收价格
};
class crUIBuyItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIBuyItemMethod();
	crUIBuyItemMethod(const crUIBuyItemMethod& handle);
	MethodClass(CRNetApp, UIBuyItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listControl;
};
class crUIBuyItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIBuyItem2Method();
	crUIBuyItem2Method(const crUIBuyItem2Method& handle);
	MethodClass(CRNetApp, UIBuyItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	bool m_emporium;
};
//class crUIBuyCountItemMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crUIBuyCountItemMethod();
//	crUIBuyCountItemMethod(const crUIBuyCountItemMethod& handle);
//	MethodClass(CRNetApp, UIBuyCountItem)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//	std::string m_buyCountWidget;
//	std::string m_priceWidget;
//};
class crRecvBuyItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvBuyItemMethod();
	crRecvBuyItemMethod(const crRecvBuyItemMethod& handle);
	MethodClass(CRNetApp, RecvBuyItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crBuyItemNotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crBuyItemNotifyMethod();
	crBuyItemNotifyMethod(const crBuyItemNotifyMethod& handle);
	MethodClass(CRNetApp, BuyItemNotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crUISetKeyboardMouseModeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUISetKeyboardMouseModeMethod();
	crUISetKeyboardMouseModeMethod(const crUISetKeyboardMouseModeMethod& handle);
	MethodClass(CRNetApp, UISetKeyboardMouseMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
public:
	unsigned short m_mode;
};
class crGetSaleItemMapMethod : public CRCore::crMethod
{//GameClient Method
public:
	crGetSaleItemMapMethod();
	crGetSaleItemMapMethod(const crGetSaleItemMapMethod& handle);
	MethodClass(CRNetApp, GetSaleItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crUIGetSaleItemMapMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIGetSaleItemMapMethod();
	crUIGetSaleItemMapMethod(const crUIGetSaleItemMapMethod& handle);
	MethodClass(CRNetApp, UIGetSaleItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_emporium;
};
class crRecvSaleItemMapMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvSaleItemMapMethod();
	crRecvSaleItemMapMethod(const crRecvSaleItemMapMethod& handle);
	MethodClass(CRNetApp, RecvSaleItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvDownLoadItemChildsMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvDownLoadItemChildsMethod();
	crRecvDownLoadItemChildsMethod(const crRecvDownLoadItemChildsMethod& handle);
	MethodClass(CRNetApp, RecvDownLoadItemChilds)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvItemEventStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvItemEventStreamMethod();
	crRecvItemEventStreamMethod(const crRecvItemEventStreamMethod& handle);
	MethodClass(CRNetApp, RecvItemEventStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crRecvPlayerEventStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvPlayerEventStreamMethod();
	crRecvPlayerEventStreamMethod(const crRecvPlayerEventStreamMethod& handle);
	MethodClass(CRNetApp, RecvPlayerEventStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crRecvPlayerMetierEventStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvPlayerMetierEventStreamMethod();
	crRecvPlayerMetierEventStreamMethod(const crRecvPlayerMetierEventStreamMethod& handle);
	MethodClass(CRNetApp, RecvPlayerMetierEventStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
};
class crBeginExchangeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crBeginExchangeMethod();
	crBeginExchangeMethod(const crBeginExchangeMethod& handle);
	MethodClass(CRNetApp, BeginExchange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crUIBeginExchangeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIBeginExchangeMethod();
	crUIBeginExchangeMethod(const crUIBeginExchangeMethod& handle);
	MethodClass(CRNetApp, UIBeginExchange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_emporium;
};
class crEndExchangeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crEndExchangeMethod();
	crEndExchangeMethod(const crEndExchangeMethod& handle);
	MethodClass(CRNetApp, EndExchange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crUIEndExchangeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIEndExchangeMethod();
	crUIEndExchangeMethod(const crUIEndExchangeMethod& handle);
	MethodClass(CRNetApp, UIEndExchange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_emporium;
};
class crRecvBeginExchangeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvBeginExchangeMethod();
	crRecvBeginExchangeMethod(const crRecvBeginExchangeMethod& handle);
	MethodClass(CRNetApp, RecvBeginExchange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvEndExchangeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvEndExchangeMethod();
	crRecvEndExchangeMethod(const crRecvEndExchangeMethod& handle);
	MethodClass(CRNetApp, RecvEndExchange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvSaleItemMapChangeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvSaleItemMapChangeMethod();
	crRecvSaleItemMapChangeMethod(const crRecvSaleItemMapChangeMethod& handle);
	MethodClass(CRNetApp, RecvSaleItemMapChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crUICloseTimerMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUICloseTimerMethod();
	crUICloseTimerMethod(const crUICloseTimerMethod& handle);
	MethodClass(CRNetApp, UICloseTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	float m_interval;
	float m_timer;
	float m_dt;
};
class crUINotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUINotifyMethod();
	crUINotifyMethod(const crUINotifyMethod& handle);
	MethodClass(CRNetApp, UINotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_textElement;
	std::string *m_notice;
};
class crRefashSaleMapMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRefashSaleMapMethod();
	crRefashSaleMapMethod(const crRefashSaleMapMethod& handle);
	MethodClass(CRNetApp, RefashSaleMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_interval;
	float m_timer;
	typedef std::vector<CRCore::crVector2i> SaleItemCountVec;
	SaleItemCountVec m_saleItemCountVec;
};
class crRecvRefashSaleMapMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvRefashSaleMapMethod();
	crRecvRefashSaleMapMethod(const crRecvRefashSaleMapMethod& handle);
	MethodClass(CRNetApp, RecvRefashSaleMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crGetPriceFormula1Method : public CRCore::crMethod
{//SceneServer & Client Method
public://ItemPrice+(ItemPrice+x)*y*(LV+1) 技能升级所需金币
	crGetPriceFormula1Method();
	crGetPriceFormula1Method(const crGetPriceFormula1Method& handle);
	MethodClass(CRNetApp, GetPriceFormula1)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::crVector2 m_factor;
	int *m_output;
};
class crBuildLearnSkillListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBuildLearnSkillListMethod();
	crBuildLearnSkillListMethod(const crBuildLearnSkillListMethod& handle);
	MethodClass(CRNetApp, BuildLearnSkillList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_learnSkillList;
};
class crBuildLearnSkillList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crBuildLearnSkillList2Method();
	crBuildLearnSkillList2Method(const crBuildLearnSkillList2Method& handle);
	MethodClass(CRNetApp, BuildLearnSkillList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_canvasName;
	std::string m_learnSkillList;
};
class crUILearnSkillMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUILearnSkillMethod();
	crUILearnSkillMethod(const crUILearnSkillMethod& handle);
	MethodClass(CRNetApp, UILearnSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listControl;
};
class crUILearnSkill2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUILearnSkill2Method();
	crUILearnSkill2Method(const crUILearnSkill2Method& handle);
	MethodClass(CRNetApp, UILearnSkill2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crRecvLearnSkillMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvLearnSkillMethod();
	crRecvLearnSkillMethod(const crRecvLearnSkillMethod& handle);
	MethodClass(CRNetApp, RecvLearnSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crLearnSkillCreaterMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crLearnSkillCreaterMethod();
	crLearnSkillCreaterMethod(const crLearnSkillCreaterMethod& handle);
	MethodClass(CRNetApp, LearnSkillCreater)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	crInstanceItem::CreateItemChildVec *m_createItemChildVec;
};
class crRecvLearnSkillCreaterMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvLearnSkillCreaterMethod();
	crRecvLearnSkillCreaterMethod(const crRecvLearnSkillCreaterMethod& handle);
	MethodClass(CRNetApp, RecvLearnSkillCreater)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crLearnSkillNotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crLearnSkillNotifyMethod();
	crLearnSkillNotifyMethod(const crLearnSkillNotifyMethod& handle);
	MethodClass(CRNetApp, LearnSkillNotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crUIAttrPointLRMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIAttrPointLRMethod();
	crUIAttrPointLRMethod(const crUIAttrPointLRMethod& handle);
	MethodClass(CRNetApp, UIAttrPointLR)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_attrpoint;
	std::string m_attr;
	std::string m_attrok;
	int m_step;
	int m_attrType;
};
class crUIAttrPointLR2Method : public CRCore::crMethod
{//GameClient Method
public:
	crUIAttrPointLR2Method();
	crUIAttrPointLR2Method(const crUIAttrPointLR2Method& handle);
	MethodClass(CRNetApp, UIAttrPointLR2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_attrpoint;
	std::string m_attr;
	std::string m_attrok;
	int m_step;
	int m_attrType;
};
class crUIAttrPointOKMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIAttrPointOKMethod();
	crUIAttrPointOKMethod(const crUIAttrPointOKMethod& handle);
	MethodClass(CRNetApp, UIAttrPointOK)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_attrpoint;
	std::string m_strength;
	std::string m_agility;
	std::string m_intellect;
};
class crUIAttrPointOK2Method : public CRCore::crMethod
{//GameClient Method
public:
	crUIAttrPointOK2Method();
	crUIAttrPointOK2Method(const crUIAttrPointOK2Method& handle);
	MethodClass(CRNetApp, UIAttrPointOK2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_attrpoint;
	std::string m_strength;
	std::string m_agility;
	std::string m_intellect;
};
class crRecvAttrChangeMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvAttrChangeMethod();
	crRecvAttrChangeMethod(const crRecvAttrChangeMethod& handle);
	MethodClass(CRNetApp, RecvAttrChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crBuildSkillListMethod : public CRCore::crMethod
{//GameClientMethod
public:
	crBuildSkillListMethod();
	crBuildSkillListMethod(const crBuildSkillListMethod& handle);
	MethodClass(CRNetApp, BuildSkillList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_skillList;
};
class crBuildSkillList2Method : public CRCore::crMethod
{//GameClientMethod
public:
	crBuildSkillList2Method();
	crBuildSkillList2Method(const crBuildSkillList2Method& handle);
	MethodClass(CRNetApp, BuildSkillList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class crUIEquipItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIEquipItemMethod();
	crUIEquipItemMethod(const crUIEquipItemMethod& handle);
	MethodClass(CRNetApp, UIEquipItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_backPackControl;
};
class crUIEquipItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIEquipItem2Method();
	crUIEquipItem2Method(const crUIEquipItem2Method& handle);
	MethodClass(CRNetApp, UIEquipItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crRecvEquipOnItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvEquipOnItemMethod();
	crRecvEquipOnItemMethod(const crRecvEquipOnItemMethod& handle);
	MethodClass(CRNetApp, RecvEquipOnItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crComposeEquipCreaterMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crComposeEquipCreaterMethod();
	crComposeEquipCreaterMethod(const crComposeEquipCreaterMethod& handle);
	MethodClass(CRNetApp, ComposeEquipCreater)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	crInstanceItem::CreateItemChildVec *m_createItemChildVec;
};
class crRecvComposeEquipCreaterMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvComposeEquipCreaterMethod();
	crRecvComposeEquipCreaterMethod(const crRecvComposeEquipCreaterMethod& handle);
	MethodClass(CRNetApp, RecvComposeEquipCreater)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crEquipOnItemNotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crEquipOnItemNotifyMethod();
	crEquipOnItemNotifyMethod(const crEquipOnItemNotifyMethod& handle);
	MethodClass(CRNetApp, EquipOnItemNotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crBuildEquipOnListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBuildEquipOnListMethod();
	crBuildEquipOnListMethod(const crBuildEquipOnListMethod& handle);
	MethodClass(CRNetApp, BuildEquipOnList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_equiponList;
};
class crBuildEquipOnList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crBuildEquipOnList2Method();
	crBuildEquipOnList2Method(const crBuildEquipOnList2Method& handle);
	MethodClass(CRNetApp, BuildEquipOnList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_equiponList;
};
class crUIBuildBackPackListMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIBuildBackPackListMethod();
	crUIBuildBackPackListMethod(const crUIBuildBackPackListMethod& handle);
	MethodClass(CRNetApp, UIBuildBackPackList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crUIBuildSkillListMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIBuildSkillListMethod();
	crUIBuildSkillListMethod(const crUIBuildSkillListMethod& handle);
	MethodClass(CRNetApp, UIBuildSkillList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crBuildQuickListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBuildQuickListMethod();
	crBuildQuickListMethod(const crBuildQuickListMethod& handle);
	MethodClass(CRNetApp, BuildQuickList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_stageName;
	std::string m_quickList;
};
class crBuildQuickList2Method : public CRCore::crMethod
{//GameClient Method
public:
	crBuildQuickList2Method();
	crBuildQuickList2Method(const crBuildQuickList2Method& handle);
	MethodClass(CRNetApp, BuildQuickList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_quickList;
};
class crEquipValueMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crEquipValueMethod();
	crEquipValueMethod(const crEquipValueMethod& handle);
	MethodClass(CRNetApp, EquipValue)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_bot;
	float m_value;
	float *m_output;
};
class crUISetUseButtonTextMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUISetUseButtonTextMethod();
	crUISetUseButtonTextMethod(const crUISetUseButtonTextMethod& handle);
	MethodClass(CRNetApp, UISetUseButtonText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_itemListElement;
	std::string m_useButtonText;
	std::string m_thingText;
	std::string m_equipText;
};
class crUIUnEquipItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUnEquipItemMethod();
	crUIUnEquipItemMethod(const crUIUnEquipItemMethod& handle);
	MethodClass(CRNetApp, UIUnEquipItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_equipOnElement;
};
class crUIUnEquipItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUnEquipItem2Method();
	crUIUnEquipItem2Method(const crUIUnEquipItem2Method& handle);
	MethodClass(CRNetApp, UIUnEquipItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crRecvUnEquipItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvUnEquipItemMethod();
	crRecvUnEquipItemMethod(const crRecvUnEquipItemMethod& handle);
	MethodClass(CRNetApp, RecvUnEquipItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUnEquipItemNotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUnEquipItemNotifyMethod();
	crUnEquipItemNotifyMethod(const crUnEquipItemNotifyMethod& handle);
	MethodClass(CRNetApp, UnEquipItemNotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRecvEquipQuickMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvEquipQuickMethod();
	crRecvEquipQuickMethod(const crRecvEquipQuickMethod& handle);
	MethodClass(CRNetApp, RecvEquipQuick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUIUnEquipQuickMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUnEquipQuickMethod();
	crUIUnEquipQuickMethod(const crUIUnEquipQuickMethod& handle);
	MethodClass(CRNetApp, UIUnEquipQuick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_quickElement;
};
class crUIUnEquipQuick2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUnEquipQuick2Method();
	crUIUnEquipQuick2Method(const crUIUnEquipQuick2Method& handle);
	MethodClass(CRNetApp, UIUnEquipQuick2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crRecvUnEquipQuickMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvUnEquipQuickMethod();
	crRecvUnEquipQuickMethod(const crRecvUnEquipQuickMethod& handle);
	MethodClass(CRNetApp, RecvUnEquipQuick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUnEquipQuickNotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUnEquipQuickNotifyMethod();
	crUnEquipQuickNotifyMethod(const crUnEquipQuickNotifyMethod& handle);
	MethodClass(CRNetApp, UnEquipQuickNotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crUISetQuickKeyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUISetQuickKeyMethod();
	crUISetQuickKeyMethod(const crUISetQuickKeyMethod& handle);
	MethodClass(CRNetApp, UISetQuickKey)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_quickElement;
	wchar_t m_key;
};
class crUISetQuickKey2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUISetQuickKey2Method();
	crUISetQuickKey2Method(const crUISetQuickKey2Method& handle);
	MethodClass(CRNetApp, UISetQuickKey2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	wchar_t m_key;
};
class crRecvSetQuickKeyMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvSetQuickKeyMethod();
	crRecvSetQuickKeyMethod(const crRecvSetQuickKeyMethod& handle);
	MethodClass(CRNetApp, RecvSetQuickKey)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUIShowItemInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIShowItemInfoMethod();
	crUIShowItemInfoMethod(const crUIShowItemInfoMethod& handle);
	MethodClass(CRNetApp, UIShowItemInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_itemListElement;
	std::string m_stageName;
	std::string m_itemIcon;
	std::string m_infoText;
	char m_flg;//0,saleParam 1,item
};
class crItemInfoMethod : public CRCore::crMethod
{//GameClient Method
public:
	crItemInfoMethod();
	crItemInfoMethod(const crItemInfoMethod& handle);
	MethodClass(CRNetApp, ItemInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::vector<std::string> m_infoArray;
	std::vector<std::string> *m_output;
};
class crUIShowTaskInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIShowTaskInfoMethod();
	crUIShowTaskInfoMethod(const crUIShowTaskInfoMethod& handle);
	MethodClass(CRNetApp, UIShowTaskInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_stageName;
	std::string m_infoText;
};
class crUIShowNextTaskInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIShowNextTaskInfoMethod();
	crUIShowNextTaskInfoMethod(const crUIShowNextTaskInfoMethod& handle);
	MethodClass(CRNetApp, UIShowNextTaskInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_infoText;
	bool m_nextOrPre;
};
class crUIUseItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUseItemMethod();
	crUIUseItemMethod(const crUIUseItemMethod& handle);
	MethodClass(CRNetApp, UIUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListControlElement> m_itemListElement;
};
class crUIUseItem2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUseItem2Method();
	crUIUseItem2Method(const crUIUseItem2Method& handle);
	MethodClass(CRNetApp, UIUseItem2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crNetUseThingMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crNetUseThingMethod();
	crNetUseThingMethod(const crNetUseThingMethod& handle);
	MethodClass(CRNetApp, NetUseThing)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
};
class crRecvNetUseThingMethod : public CRCore::crMethod
{//SceneServer && GameClient Method
public:
	crRecvNetUseThingMethod();
	crRecvNetUseThingMethod(const crRecvNetUseThingMethod& handle);
	MethodClass(CRNetApp, RecvNetUseThing)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUTCureHPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crUTCureHPMethod();
	crUTCureHPMethod(const crUTCureHPMethod& handle);
	MethodClass(CRNetApp, UTCureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_user;
	short m_curehp;
};
class crUTCureMPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crUTCureMPMethod();
	crUTCureMPMethod(const crUTCureMPMethod& handle);
	MethodClass(CRNetApp, UTCureMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_user;
	short m_curemp;
};
//class crUTClientCureHPMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crUTClientCureHPMethod();
//	crUTClientCureHPMethod(const crUTClientCureHPMethod& handle);
//	MethodClass(CRNetApp, UTClientCureHP)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<crInstanceItem> m_user;
//	short m_hp;
//};
//class crUTClientCureMPMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crUTClientCureMPMethod();
//	crUTClientCureMPMethod(const crUTClientCureMPMethod& handle);
//	MethodClass(CRNetApp, UTClientCureMP)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<crInstanceItem> m_user;
//	short m_mp;
//};
class crCancelUseSkillMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCancelUseSkillMethod();
	crCancelUseSkillMethod(const crCancelUseSkillMethod& handle);
	MethodClass(CRNetApp, CancelUseSkill)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
};
class crShowTargetInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crShowTargetInfoMethod();
	crShowTargetInfoMethod(const crShowTargetInfoMethod& handle);
	MethodClass(CRNetApp, ShowTargetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_targetItem;
	std::string m_stageName;
};
class crUIUpdateTargetMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateTargetMethod();
	crUIUpdateTargetMethod(const crUIUpdateTargetMethod& handle);
	MethodClass(CRNetApp, UIUpdateTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crImageStage* m_this;
	std::string m_itemIcon;
	std::string m_name;
	std::string m_lv;
	std::string m_hp;
	std::string m_addGroup;
	std::string m_addFriend;
	std::string m_chatWithFriend;
};
class crCureHPCollideMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害公式1，base+lv*lvscalar
	crCureHPCollideMethod();
	crCureHPCollideMethod(const crCureHPCollideMethod& handle);
	MethodClass(CRNetApp, CureHPCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_hitItem;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
	float m_lvfactor;
};
class crCureMPCollideMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public://伤害公式1，base+lv*lvscalar
	crCureMPCollideMethod();
	crCureMPCollideMethod(const crCureMPCollideMethod& handle);
	MethodClass(CRNetApp, CureMPCollide)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_hitItem;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
	float m_lvfactor;
};
class crCureHPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crCureHPMethod();
	crCureHPMethod(const crCureHPMethod& handle);
	MethodClass(CRNetApp, CureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_curehp;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
};
class crRecvCureHPMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvCureHPMethod();
	crRecvCureHPMethod(const crRecvCureHPMethod& handle);
	MethodClass(CRNetApp, RecvCureHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crCureMPMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crCureMPMethod();
	crCureMPMethod(const crCureMPMethod& handle);
	MethodClass(CRNetApp, CureMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_curemp;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
};
class crRecvCureMPMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvCureMPMethod();
	crRecvCureMPMethod(const crRecvCureMPMethod& handle);
	MethodClass(CRNetApp, RecvCureMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crServerRefuseMethod : public CRCore::crMethod
{//GameClient Method
public:
	crServerRefuseMethod();
	crServerRefuseMethod(const crServerRefuseMethod& handle);
	MethodClass(CRNetApp, ServerRefuse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_code;	
	int m_netType;
};
class crUTTransportToBaseMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crUTTransportToBaseMethod();
	crUTTransportToBaseMethod(const crUTTransportToBaseMethod& handle);
	MethodClass(CRNetApp, UTTransportToBase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_user;
};
class crUIShowStaticTextMethod : public CRCore::crMethod
{//
public:
	crUIShowStaticTextMethod();
	crUIShowStaticTextMethod(const crUIShowStaticTextMethod& handle);
	MethodClass(CRNetApp, UIShowStaticText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_textElement;
	std::string m_text;
};
class crUIShowStaticText2Method : public CRCore::crMethod
{//
public:
	crUIShowStaticText2Method();
	crUIShowStaticText2Method(const crUIShowStaticText2Method& handle);
	MethodClass(CRNetApp, UIShowStaticText2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_textElement;
	std::string m_text;
};
class crUIAddGameGroupMethod : public CRCore::crMethod
{//GameClient Method 游戏组队，为临时组队，保存与RoleData的GroupMemberSet
public:
	crUIAddGameGroupMethod();
	crUIAddGameGroupMethod(const crUIAddGameGroupMethod& handle);
	MethodClass(CRNetApp, UIAddGameGroup)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crUIAddGameFriendMethod : public CRCore::crMethod
{//GameClient Method 加聊天好友
public:
	crUIAddGameFriendMethod();
	crUIAddGameFriendMethod(const crUIAddGameFriendMethod& handle);
	MethodClass(CRNetApp, UIAddGameFriend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crUIChatWithFriendMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIChatWithFriendMethod();
	crUIChatWithFriendMethod(const crUIChatWithFriendMethod& handle);
	MethodClass(CRNetApp, UIChatWithFriend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_stageName;
	std::string m_titleElement;
	std::string m_chatRecElement;
};
class crExitProgramMethod : public CRCore::crMethod
{//GameClient Method
public:
	crExitProgramMethod();
	crExitProgramMethod(const crExitProgramMethod& handle);
	MethodClass(CRNetApp, ExitProgram)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crRpgChangeCameraMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRpgChangeCameraMethod();
	crRpgChangeCameraMethod(const crRpgChangeCameraMethod& handle);
	MethodClass(CRNetApp, RpgChangeCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector2 m_normal;
	CRCore::crVector2 m_near;
	unsigned char m_current;//0:normal 1:near
};

class crCameraShakeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCameraShakeMethod();
	crCameraShakeMethod(const crCameraShakeMethod& handle);
	MethodClass(CRNetApp, CameraShake)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_range;
	float m_interval;
};

class crUseItemProtectTimeUpdateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crUseItemProtectTimeUpdateMethod();
	crUseItemProtectTimeUpdateMethod(const crUseItemProtectTimeUpdateMethod& handle);
	MethodClass(CRNetApp, UseItemProtectTimeUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
};
////////////开房间式网游
class crQuerySceneListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crQuerySceneListMethod();
	crQuerySceneListMethod(const crQuerySceneListMethod& handle);
	MethodClass(CRNetApp, QuerySceneList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvSceneListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvSceneListMethod();
	crRecvSceneListMethod(const crRecvSceneListMethod& handle);
	MethodClass(CRNetApp, RecvSceneList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
};
class crSetSelectedSceneMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetSelectedSceneMethod();
	crSetSelectedSceneMethod(const crSetSelectedSceneMethod& handle);
	MethodClass(CRNetApp, SetSelectedScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_columnListElement;
};
class crQuerySceneRoomsMethod : public CRCore::crMethod
{//GameClient Method
public:
	crQuerySceneRoomsMethod();
	crQuerySceneRoomsMethod(const crQuerySceneRoomsMethod& handle);
	MethodClass(CRNetApp, QuerySceneRooms)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvSceneRoomsMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvSceneRoomsMethod();
	crRecvSceneRoomsMethod(const crRecvSceneRoomsMethod& handle);
	MethodClass(CRNetApp, RecvSceneRooms)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_tableWidget;
};
//class crCreateSceneRoomMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crCreateSceneRoomMethod();
//	crCreateSceneRoomMethod(const crCreateSceneRoomMethod& handle);
//	MethodClass(CRNetApp, CreateSceneRoom)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRGUI::crElement> m_element;
//	std::string m_roomNameElement;
//	std::string m_gameModeElement;
//	std::string m_passwordElement;
//	std::string m_maxPlayerCountElement;
//};
class crCreateSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCreateSceneRoomMethod();
	crCreateSceneRoomMethod(const crCreateSceneRoomMethod& handle);
	MethodClass(CRNetApp, CreateSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_battleMode;
	std::string m_battleName;
	std::string m_timeLimit;
	std::string m_friendlyFire;
	std::vector<short> m_timeLimits;
	bool m_autoStart;
	float m_autoStartTime;
};
class crRecvCreateSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvCreateSceneRoomMethod();
	crRecvCreateSceneRoomMethod(const crRecvCreateSceneRoomMethod& handle);
	MethodClass(CRNetApp, RecvCreateSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_success;
};
class crEnterRoomReturnMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crEnterRoomReturnMethod();
	crEnterRoomReturnMethod(const crEnterRoomReturnMethod& handle);
	MethodClass(CRNetApp, EnterRoomReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//class crEnterSceneRoomMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crEnterSceneRoomMethod();
//	crEnterSceneRoomMethod(const crEnterSceneRoomMethod& handle);
//	MethodClass(CRNetApp, EnterSceneRoom)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRGUI::crElement> m_element;
//	std::string m_columnListElement;
//};
class crPreEnterSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crPreEnterSceneRoomMethod();
	crPreEnterSceneRoomMethod(const crPreEnterSceneRoomMethod& handle);
	MethodClass(CRNetApp, PreEnterSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvEnterSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvEnterSceneRoomMethod();
	crRecvEnterSceneRoomMethod(const crRecvEnterSceneRoomMethod& handle);
	MethodClass(CRNetApp, RecvEnterSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_success;
};
class crRoomInfoChangeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRoomInfoChangeMethod();
	crRoomInfoChangeMethod(const crRoomInfoChangeMethod& handle);
	MethodClass(CRNetApp, RoomInfoChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
	std::string m_columnListElement;
};

class crUIRoomInfoUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIRoomInfoUpdateMethod();
	crUIRoomInfoUpdateMethod(const crUIRoomInfoUpdateMethod& handle);
	MethodClass(CRNetApp, UIRoomInfoUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crImageStage* m_this;
	std::string m_playerListElement;
};

//class crLeaveSceneRoomMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crLeaveSceneRoomMethod();
//	crLeaveSceneRoomMethod(const crLeaveSceneRoomMethod& handle);
//	MethodClass(CRNetApp, LeaveSceneRoom)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};

//class crRecvRoomPlayerDataMethod : public CRCore::crMethod
//{//GameClient Method
//public:
//	crRecvRoomPlayerDataMethod();
//	crRecvRoomPlayerDataMethod(const crRecvRoomPlayerDataMethod& handle);
//	MethodClass(CRNetApp, RecvRoomPlayerData)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<crRecvDataStream> m_recvDataStream;
//};

class crStartRoomGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crStartRoomGameMethod();
	crStartRoomGameMethod(const crStartRoomGameMethod& handle);
	MethodClass(CRNetApp, StartRoomGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crRecvStartRoomGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvStartRoomGameMethod();
	crRecvStartRoomGameMethod(const crRecvStartRoomGameMethod& handle);
	MethodClass(CRNetApp, RecvStartRoomGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crRecvEndRoomGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRecvEndRoomGameMethod();
	crRecvEndRoomGameMethod(const crRecvEndRoomGameMethod& handle);
	MethodClass(CRNetApp, RecvEndRoomGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crCheckAndRackEquipOnMeshMethod : public CRCore::crMethod
{//GameClient Method 检查并换装
public:
	crCheckAndRackEquipOnMeshMethod();
	crCheckAndRackEquipOnMeshMethod(const crCheckAndRackEquipOnMeshMethod& handle);
	MethodClass(CRNetApp, CheckAndRackEquipOnMesh)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	class MeshScript :public CRCore::Referenced
	{
	public:
		MeshScript():m_partid(0){}
		void setPath(const std::string &path){ m_path = path; }
		const std::string &getPath()const{ return m_path; }
		void setRbodyFile(const std::string &file){ m_rbodyFile = file; }
		const std::string &getRbodyFile()const{ return m_rbodyFile; }
		void setPartID(int partid) { m_partid = partid; }
		int getPartID() { return m_partid;}
		void setName(const std::string &name){ m_name = name; }
		const std::string &getName()const { return m_name; }
		void setFileName(const std::string &filename) { m_filename = filename; }
		const std::string &getFileName()const { return m_filename; }
	protected:
		virtual ~MeshScript(){}
		int m_partid;
		std::string m_path;
		std::string m_rbodyFile;
		std::string m_name;
		std::string m_filename;
	};
	typedef std::map< int, CRCore::ref_ptr<MeshScript> >EquipOnMeshMap;
protected:
	crInstanceItem* m_this;
	CRCore::crVector2b m_partRange;
};
//class crCheckAndLoadQuickAnimMethod : public CRCore::crMethod
//{//GameClient Method 检查并载入动作
//public:
//	crCheckAndLoadQuickAnimMethod();
//	crCheckAndLoadQuickAnimMethod(const crCheckAndLoadQuickAnimMethod& handle);
//	MethodClass(CRNetApp, CheckAndLoadQuickAnim)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//};
class crLenFadeInitMethod : public CRCore::crMethod
{//Client Method
public://镜头特效
	crLenFadeInitMethod();
	crLenFadeInitMethod(const crLenFadeInitMethod& handle);
	MethodClass(CRNetApp, LenFadeInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector4 m_color;
};
class crLenFadeInit2Method : public CRCore::crMethod
{//Client Method
public://镜头特效
	crLenFadeInit2Method();
	crLenFadeInit2Method(const crLenFadeInit2Method& handle);
	MethodClass(CRNetApp, LenFadeInit2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	CRCore::crVector4 m_color;
};
class crLenFadeUpdateMethod : public CRCore::crMethod
{//Client Method
public://镜头特效
	crLenFadeUpdateMethod();
	crLenFadeUpdateMethod(const crLenFadeUpdateMethod& handle);
	MethodClass(CRNetApp, LenFadeUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	float m_speed;
	float *m_dt;
};
class crAttachCameraToNodeMethod : public CRCore::crMethod
{//Client Method
public://将相机邦定到节点
	crAttachCameraToNodeMethod();
	crAttachCameraToNodeMethod(const crAttachCameraToNodeMethod& handle);
	MethodClass(CRNetApp, AttachCameraToNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_cameraName;
	CRCore::crNode* m_this;//暂定
};
class crChangeCameraMethod : public CRCore::crMethod
{//Client Method
public:
	crChangeCameraMethod();
	crChangeCameraMethod(const crChangeCameraMethod& handle);
	MethodClass(CRNetApp, ChangeCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_cameraName;
};
class crIfInVolumeNodeIsMeMethod : public CRCore::crMethod
{//Client Method
public:
	crIfInVolumeNodeIsMeMethod();
	crIfInVolumeNodeIsMeMethod(const crIfInVolumeNodeIsMeMethod& handle);
	MethodClass(CRNetApp, IfInVolumeNodeIsMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_inVolumeNode;
};
class crBorrowCameraMethod : public CRCore::crMethod
{//Client Method
public:
	crBorrowCameraMethod();
	crBorrowCameraMethod(const crBorrowCameraMethod& handle);
	MethodClass(CRNetApp, BorrowCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;//first player Bot
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
	CRCore::ref_ptr<CRPhysics::crCamera> m_borrower;
	float m_timeDelay;
};
class crCameraBorrowUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crCameraBorrowUpdateMethod();
	crCameraBorrowUpdateMethod(const crCameraBorrowUpdateMethod& handle);
	MethodClass(CRNetApp, CameraBorrowUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
};
class crReturnCameraMethod : public CRCore::crMethod
{//Client Method
public:
	crReturnCameraMethod();
	crReturnCameraMethod(const crReturnCameraMethod& handle);
	MethodClass(CRNetApp, ReturnCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crExecuteActionMethod : public CRCore::crMethod
{//Client Method
public:
	crExecuteActionMethod();
	crExecuteActionMethod(const crExecuteActionMethod& handle);
	MethodClass(CRNetApp, ExecuteAction)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
	std::string m_anim;
};
class crBotTurnToTargetMethod : public CRCore::crMethod
{//Client Method
public:
	crBotTurnToTargetMethod();
	crBotTurnToTargetMethod(const crBotTurnToTargetMethod& handle);
	MethodClass(CRNetApp, BotTurnToTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
};
class crItemTurnToMeMethod : public CRCore::crMethod
{//Client Method
public:
	crItemTurnToMeMethod();
	crItemTurnToMeMethod(const crItemTurnToMeMethod& handle);
	MethodClass(CRNetApp, ItemTurnToMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
};
class crHideItemMethod : public CRCore::crMethod
{//Client Method
public:
	crHideItemMethod();
	crHideItemMethod(const crHideItemMethod& handle);
	MethodClass(CRNetApp, HideItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
};
class crAdvanceSlotProgressMethod : public CRCore::crMethod
{//Client Method
public:
	crAdvanceSlotProgressMethod();
	crAdvanceSlotProgressMethod(const crAdvanceSlotProgressMethod& handle);
	MethodClass(CRNetApp, AdvanceSlotProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crAutoPreviewImageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crAutoPreviewImageMethod();
	crAutoPreviewImageMethod(const crAutoPreviewImageMethod& handle);
	MethodClass(CRNetApp, AutoPreviewImage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageBoxElement> m_imageBoxElement;
	float m_interval;
	typedef std::vector<std::string> FileVec;
	FileVec m_imageFileVec;
	float m_timer;
	int m_currentImage;
};
class crNodeSetVisiableMethod : public CRCore::crMethod
{//Client Method
public:
	crNodeSetVisiableMethod();
	crNodeSetVisiableMethod(const crNodeSetVisiableMethod& handle);
	MethodClass(CRNetApp, NodeSetVisiable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	unsigned char m_nodeType;
	bool m_visiable;
	CRCore::crNode *m_node;
};
class crMatrixNodeSetPositionMethod : public CRCore::crMethod
{//Client Method
public:
	crMatrixNodeSetPositionMethod();
	crMatrixNodeSetPositionMethod(const crMatrixNodeSetPositionMethod& handle);
	MethodClass(CRNetApp, MatrixNodeSetPosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	CRCore::crVector3 m_position;
	CRCore::crMatrixTransform *m_node;
};
class crSetViewOffsetRangeMethod : public CRCore::crMethod
{//Client Method
public:
	crSetViewOffsetRangeMethod();
	crSetViewOffsetRangeMethod(const crSetViewOffsetRangeMethod& handle);
	MethodClass(CRNetApp, SetViewOffsetRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector2 m_range;
};
class crStopSoundMethod : public CRCore::crMethod
{//Client Method
public:
	crStopSoundMethod();
	crStopSoundMethod(const crStopSoundMethod& handle);
	MethodClass(CRNetApp, StopSound)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_soundName;
};
class crSetNodeMaterialMethod : public CRCore::crNodeMethod
{//Client Method
public:
	crSetNodeMaterialMethod();
	crSetNodeMaterialMethod(const crSetNodeMaterialMethod& handle);
	MethodClass(CRNetApp, SetNodeMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void releaseObjects(CRCore::crState* state);
protected:
	CRCore::crNode *m_this;
	CRCore::crVector4 m_diffuse;
	CRCore::crVector4 m_emissive;
	CRCore::ref_ptr<CRCore::crMaterial> m_material;
	CRCore::ref_ptr<CRCore::crStateSet> m_stateset;
	bool m_init;
};
class crRemoveNodeMaterialMethod : public CRCore::crMethod
{//Client Method
public:
	crRemoveNodeMaterialMethod();
	crRemoveNodeMaterialMethod(const crRemoveNodeMaterialMethod& handle);
	MethodClass(CRNetApp, RemoveNodeMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode *m_this;
};
class crRunFrequencyControlMethod : public CRCore::crMethod
{//Client & Server Method
public:
	crRunFrequencyControlMethod();
	crRunFrequencyControlMethod(const crRunFrequencyControlMethod& handle);
	MethodClass(CRNetApp, RunFrequencyControl)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_interval;
	float m_time;
	float* m_dt;
};
class crMouseFrequencyControlMethod : public CRCore::crMethod
{//Client Method
public:
	crMouseFrequencyControlMethod();
	crMouseFrequencyControlMethod(const crMouseFrequencyControlMethod& handle);
	MethodClass(CRNetApp, MouseFrequencyControl)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_interval;
	double m_lastupdtime;
	double m_time;
};
class crSetNodeColorMethod : public CRCore::crMethod
{//Client Method
public:
	crSetNodeColorMethod();
	crSetNodeColorMethod(const crSetNodeColorMethod& handle);
	MethodClass(CRNetApp, SetNodeColor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode *m_this;
	CRCore::crVector4 m_color;
	CRUtil::crSetObjectColorVisitor m_setColorVisitor;
};
class crServerReadPathMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crServerReadPathMethod();
	crServerReadPathMethod(const crServerReadPathMethod& handle);
	MethodClass(CRNetApp, ServerReadPath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
};

class crClientReadPathMethod : public CRCore::crMethod
{//Client Method
public:
	crClientReadPathMethod();
	crClientReadPathMethod(const crClientReadPathMethod& handle);
	MethodClass(CRNetApp, ClientReadPath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	//CRCore::ref_ptr<crRpgGameCharacterUpdateMethod> m_characterUpdateMethod;
};
class crCloseToSyncPositionMethod : public CRCore::crMethod
{//Client & SceneServer Method
public:
	crCloseToSyncPositionMethod();
	crCloseToSyncPositionMethod(const crCloseToSyncPositionMethod& handle);
	MethodClass(CRNetApp, CloseToSyncPosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
};
class crItemCutTalkMethod : public CRCore::crMethod
{//Client Method
public:
	crItemCutTalkMethod();
	crItemCutTalkMethod(const crItemCutTalkMethod& handle);
	MethodClass(CRNetApp, ItemCutTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	unsigned short m_talkid;
};
class crItemCutSceneMethod : public CRCore::crMethod
{//Client Method
public:
	crItemCutSceneMethod();
	crItemCutSceneMethod(const crItemCutSceneMethod& handle);
	MethodClass(CRNetApp, ItemCutScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	unsigned short m_id;
};
class crCutShowNodeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCutShowNodeMethod();
	crCutShowNodeMethod(const crCutShowNodeMethod& handle);
	MethodClass(CRNetApp, CutShowNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	bool m_visiable;
	bool m_intersectable;
};
class crItemSetActStateMethod : public CRCore::crMethod
{//Client Method
public:
	crItemSetActStateMethod();
	crItemSetActStateMethod(const crItemSetActStateMethod& handle);
	MethodClass(CRNetApp, ItemSetActState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	unsigned short m_specActState;
	float m_actDuration;
};
class crChangeActStateMethod : public CRCore::crMethod
{//Client Method
public:
	crChangeActStateMethod();
	crChangeActStateMethod(const crChangeActStateMethod& handle);
	MethodClass(CRNetApp, ChangeActState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	unsigned short m_actState;
	float m_actDuration;
};
class crItemControllerMethod : public CRCore::crMethod
{//Client Method
public:
	crItemControllerMethod();
	crItemControllerMethod(const crItemControllerMethod& handle);
	MethodClass(CRNetApp, ItemController)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	unsigned char m_control;
};
class crDoItemControllerAndSendMethod : public CRCore::crMethod
{//Client Player Method
public:
	crDoItemControllerAndSendMethod();
	crDoItemControllerAndSendMethod(const crDoItemControllerAndSendMethod& handle);
	MethodClass(CRNetApp, DoItemControllerAndSend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_length;
};
class crPlayerFindPathToMethod : public CRCore::crMethod
{//Client Player Method
public:
	crPlayerFindPathToMethod();
	crPlayerFindPathToMethod(const crPlayerFindPathToMethod& handle);
	MethodClass(CRNetApp, PlayerFindPathTo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::vector<crInstanceItem::WayPoint> m_waypoints;
	crInstanceItem::WayPoint m_loadpoint;
};
class crRecvFindPathToMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvFindPathToMethod();
	crRecvFindPathToMethod(const crRecvFindPathToMethod& handle);
	MethodClass(CRNetApp, RecvFindPathTo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvItemStateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvItemStateMethod();
	crRecvItemStateMethod(const crRecvItemStateMethod& handle);
	MethodClass(CRNetApp, RecvItemState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crPlayerTransportMethod : public CRCore::crMethod
{//Client Player Method
public:
	crPlayerTransportMethod();
	crPlayerTransportMethod(const crPlayerTransportMethod& handle);
	MethodClass(CRNetApp, PlayerTransport)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_sceneid;
	unsigned short m_layerid;
	CRCore::crVector2 m_coord;
};
class crRecvTargetMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvTargetMethod();
	crRecvTargetMethod(const crRecvTargetMethod& handle);
	MethodClass(CRNetApp, RecvTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crITouchMethod : public CRCore::crMethod
{//Client Method
public:
	crITouchMethod();
	crITouchMethod(const crITouchMethod& handle);
	MethodClass(CRNetApp, ITouch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	int m_targetid;
};
//class crITransportMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crITransportMethod();
//	crITransportMethod(const crITransportMethod& handle);
//	MethodClass(CRNetApp, ITransport)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	int m_sceneid;
//	unsigned short m_layerid;
//	//bool m_isClient;
//};
class crPlayerSetGuiseStateMethod : public CRCore::crMethod
{//Client Player Method
public:
	crPlayerSetGuiseStateMethod();
	crPlayerSetGuiseStateMethod(const crPlayerSetGuiseStateMethod& handle);
	MethodClass(CRNetApp, PlayerSetGuiseState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	unsigned int m_guiseState;
};
class crRecvGuiseStateMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvGuiseStateMethod();
	crRecvGuiseStateMethod(const crRecvGuiseStateMethod& handle);
	MethodClass(CRNetApp, RecvGuiseState)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crCheckItemTouchDistanceMethod : public CRCore::crMethod
{//Client Player Method
public:
	crCheckItemTouchDistanceMethod();
	crCheckItemTouchDistanceMethod(const crCheckItemTouchDistanceMethod& handle);
	MethodClass(CRNetApp, CheckItemTouchDistance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
};
class crThisIsMeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crThisIsMeMethod();
	crThisIsMeMethod(const crThisIsMeMethod& handle);
	MethodClass(CRNetApp, ThisIsMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crPlayerMoveToMethod : public CRCore::crMethod
{//Client Player Method
public:
	crPlayerMoveToMethod();
	crPlayerMoveToMethod(const crPlayerMoveToMethod& handle);
	MethodClass(CRNetApp, PlayerMoveTo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole *m_this;
	CRCore::crVector3 m_targetPos;
	bool m_inputPos;
};
class crGetStorageItemMapMethod : public CRCore::crMethod
{//GameClient Method
public:
	crGetStorageItemMapMethod();
	crGetStorageItemMapMethod(const crGetStorageItemMapMethod& handle);
	MethodClass(CRNetApp, GetStorageItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crRecvStorageItemMapMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvStorageItemMapMethod();
	crRecvStorageItemMapMethod(const crRecvStorageItemMapMethod& handle);
	MethodClass(CRNetApp, RecvStorageItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crBuildStorageItemListMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBuildStorageItemListMethod();
	crBuildStorageItemListMethod(const crBuildStorageItemListMethod& handle);
	MethodClass(CRNetApp, BuildStorageItemList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class crInsertItemToStorageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 
public:
	crInsertItemToStorageMethod();
	crInsertItemToStorageMethod(const crInsertItemToStorageMethod& handle);
	MethodClass(CRNetApp, InsertItemToStorage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	char *m_output;
};
class crRemoveItemFromStorageMethod : public CRCore::crMethod
{//SceneServer & GameClient Method 
public:
	crRemoveItemFromStorageMethod();
	crRemoveItemFromStorageMethod(const crRemoveItemFromStorageMethod& handle);
	MethodClass(CRNetApp, RemoveItemFromStorage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<crInstanceItem> m_item;
	char *m_output;//返回-1失败，0减数量，1删除
};
class crRecvDepositItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvDepositItemMethod();
	crRecvDepositItemMethod(const crRecvDepositItemMethod& handle);
	MethodClass(CRNetApp, RecvDepositItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crRecvTakeoutItemMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crRecvTakeoutItemMethod();
	crRecvTakeoutItemMethod(const crRecvTakeoutItemMethod& handle);
	MethodClass(CRNetApp, RecvTakeoutItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUIDepositItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIDepositItemMethod();
	crUIDepositItemMethod(const crUIDepositItemMethod& handle);
	MethodClass(CRNetApp, UIDepositItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crUIGetStorageItemMapMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIGetStorageItemMapMethod();
	crUIGetStorageItemMapMethod(const crUIGetStorageItemMapMethod& handle);
	MethodClass(CRNetApp, UIGetStorageItemMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crUITakeoutItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUITakeoutItemMethod();
	crUITakeoutItemMethod(const crUITakeoutItemMethod& handle);
	MethodClass(CRNetApp, UITakeoutItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crLoginReturnMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crLoginReturnMethod();
	crLoginReturnMethod(const crLoginReturnMethod& handle);
	MethodClass(CRNetApp, LoginReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_loginCode;
};
class crDisconnectLoginMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crDisconnectLoginMethod();
	crDisconnectLoginMethod(const crDisconnectLoginMethod& handle);
	MethodClass(CRNetApp, DisconnectLogin)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRegisterAccountReturnMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crRegisterAccountReturnMethod();
	crRegisterAccountReturnMethod(const crRegisterAccountReturnMethod& handle);
	MethodClass(CRNetApp, RegisterAccountReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crLoginDownloadReturnMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crLoginDownloadReturnMethod();
	crLoginDownloadReturnMethod(const crLoginDownloadReturnMethod& handle);
	MethodClass(CRNetApp, LoginDownloadReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_loginCode;
};
class crLoginDownloadCaseMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crLoginDownloadCaseMethod();
	crLoginDownloadCaseMethod(const crLoginDownloadCaseMethod& handle);
	MethodClass(CRNetApp, LoginDownloadCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_case;
};
class crIsRuningMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crIsRuningMethod();
	crIsRuningMethod(const crIsRuningMethod& handle);
	MethodClass(CRNetApp, IsRuning)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crGetRunCodeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crGetRunCodeMethod();
	crGetRunCodeMethod(const crGetRunCodeMethod& handle);
	MethodClass(CRNetApp, GetRunCode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetRunCodeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crSetRunCodeMethod();
	crSetRunCodeMethod(const crSetRunCodeMethod& handle);
	MethodClass(CRNetApp, SetRunCode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_runcode;
};
class crSetRunningMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crSetRunningMethod();
	crSetRunningMethod(const crSetRunningMethod& handle);
	MethodClass(CRNetApp, SetRunning)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_running;
};
class crTankControllerMethod : public CRCore::crMethod
{//Client Method
public:
	crTankControllerMethod();
	crTankControllerMethod(const crTankControllerMethod& handle);
	MethodClass(CRNetApp, TankController)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	char m_control;
};
class crDoTankControllerAndSendMethod : public CRCore::crMethod
{//Client Player Method
public:
	crDoTankControllerAndSendMethod();
	crDoTankControllerAndSendMethod(const crDoTankControllerAndSendMethod& handle);
	MethodClass(CRNetApp, DoTankControllerAndSend)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_interval;
	float m_time;
};
class crNetControlTankMethod : public CRCore::crMethod
{//Server & Client Method
public:
	crNetControlTankMethod();
	crNetControlTankMethod(const crNetControlTankMethod& handle);
	MethodClass(CRNetApp, NetControlTank)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crCarMoveMethod : public CRCore::crMethod
{//Client Method
public:
	crCarMoveMethod();
	crCarMoveMethod(const crCarMoveMethod& handle);
	MethodClass(CRNetApp, CarMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	CRCore::ref_ptr<crRpgGameCharacterUpdateMethod> m_characterUpdateMethod;
	float m_k;//斜坡速度因子
};
class crTankMoveMethod : public CRCore::crMethod
{//Client Method
public:
	crTankMoveMethod();
	crTankMoveMethod(const crTankMoveMethod& handle);
	MethodClass(CRNetApp, TankMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	CRCore::ref_ptr<crRpgGameCharacterUpdateMethod> m_characterUpdateMethod;
	float m_k;//斜坡速度因子
	//bool m_isClient;
	CRCore::crVector4 m_viewOffsetParams;//0ffset,Scale,Times,MaxRange
	float m_interval;
	float m_time;
};
class crRotateEmplacementLogic : public CRCore::crLogic
{//Client Method
public:
	crRotateEmplacementLogic();
	crRotateEmplacementLogic(const crRotateEmplacementLogic& handle);
	LogicClass(CRNetApp, RotateEmplacement)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	crInstanceItem* m_this;
	float m_dt;
	std::string m_boneName;
	CRCore::crVector2 m_pitchRange;
	CRCore::crVector2 m_rollRange;

	CRCore::ref_ptr<CRCore::crHandle> m_increaseAim;
private:
	float m_lastroll;
	float m_lastpitch;
};
class crSwitchCursorMethod : public CRCore::crMethod
{//Client Method
public:
	crSwitchCursorMethod();
	crSwitchCursorMethod(const crSwitchCursorMethod& handle);
	MethodClass(CRNetApp, SwitchCursor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_currentCursor;
	typedef std::vector<std::string> CursorVec;
	CursorVec m_cursorVec;
};
class crAttackMethod : public CRCore::crMethod
{//Client Method
public:
	crAttackMethod();
	crAttackMethod(const crAttackMethod& handle);
	MethodClass(CRNetApp, Attack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRole* m_this;
	short m_backspeed;
	float m_aimScale;
};
class crRecvAttackMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvAttackMethod();
	crRecvAttackMethod(const crRecvAttackMethod& handle);
	MethodClass(CRNetApp, RecvAttack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crTankChangeCameraMethod : public CRCore::crMethod
{//Client Method
public:
	crTankChangeCameraMethod();
	crTankChangeCameraMethod(const crTankChangeCameraMethod& handle);
	MethodClass(CRNetApp, TankChangeCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	static char m_type;
	CRCore::crVector2 m_rollPitchSpeed;
	CRCore::crVector2 m_heightOffsetSpeed;
	CRCore::crVector4 m_heightOffset;
};
class crIncreaseAimMethod : public CRCore::crMethod
{//Client Method
public:
	crIncreaseAimMethod();
	crIncreaseAimMethod(const crIncreaseAimMethod& handle);
	MethodClass(CRNetApp, IncreaseAim)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_speed;
};
class crSetAimUIMethod : public CRCore::crMethod
{//Client Method
public:
	crSetAimUIMethod();
	crSetAimUIMethod(const crSetAimUIMethod& handle);
	MethodClass(CRNetApp, SetAimUI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_aimScale;
	CRCore::crVector2 m_aimTrans;
	std::string m_aimui;
};
class crRecvCreateRoleMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvCreateRoleMethod();
	crRecvCreateRoleMethod(const crRecvCreateRoleMethod& handle);
	MethodClass(CRNetApp, RecvCreateRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_sucess;
};
class crCheckGameRoleCountMethod : public CRCore::crMethod
{//Client Method
public:
	crCheckGameRoleCountMethod();
	crCheckGameRoleCountMethod(const crCheckGameRoleCountMethod& handle);
	MethodClass(CRNetApp, CheckGameRoleCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_maxrole;
};
class crCarSlopeAdaptMethod : public CRCore::crMethod
{//Client Method
public:
	crCarSlopeAdaptMethod();
	crCarSlopeAdaptMethod(const crCarSlopeAdaptMethod& handle);
	MethodClass(CRNetApp, CarSlopeAdapt)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crWheelAndTrackSpeedMethod : public CRCore::crMethod
{//Client Method
public:
	crWheelAndTrackSpeedMethod();
	crWheelAndTrackSpeedMethod(const crWheelAndTrackSpeedMethod& handle);
	MethodClass(CRNetApp, WheelAndTrackSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_wheelActName;
	float m_wheelActSpeedScale;
	std::string m_trackObjName;
	float m_trackSpeedScale;
private:
	CRCore::ref_ptr<CRCore::crObject> m_trackObj;
	CRCore::crVector3 m_stepTrans;
};
class crViewPitchWobbleMethod : public CRCore::crMethod
{//Client Method
public:
	crViewPitchWobbleMethod();
	crViewPitchWobbleMethod(const crViewPitchWobbleMethod& handle);
	MethodClass(CRNetApp, ViewPitchWobble)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_wobble;
	float m_comebackTime;
};
//class crCreateTankAimMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crCreateTankAimMethod();
//	crCreateTankAimMethod(const crCreateTankAimMethod& handle);
//	MethodClass(CRNetApp, CreateTankAim)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	float m_r;
//	int m_step;
//};
class crRemoveRoleMethod : public CRCore::crMethod
{//Client Method
public:
	crRemoveRoleMethod();
	crRemoveRoleMethod(const crRemoveRoleMethod& handle);
	MethodClass(CRNetApp, RemoveRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvRemoveRoleMethod : public CRCore::crMethod
{//Client Method
public:
	crRecvRemoveRoleMethod();
	crRecvRemoveRoleMethod(const crRecvRemoveRoleMethod& handle);
	MethodClass(CRNetApp, RecvRemoveRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_roleid;
	bool m_sucess;
};
class crCheckGameRolesIsLoadedMethod : public CRCore::crMethod
{//Client Method
public:
	crCheckGameRolesIsLoadedMethod();
	crCheckGameRolesIsLoadedMethod(const crCheckGameRolesIsLoadedMethod& handle);
	MethodClass(CRNetApp, CheckGameRolesIsLoaded)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetSelectedRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetSelectedRoomMethod();
	crSetSelectedRoomMethod(const crSetSelectedRoomMethod& handle);
	MethodClass(CRNetApp, SetSelectedRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crTableWidgetNode* m_this;
};
class crEnterSceneRoomMethod : public CRCore::crMethod
{//GameClient Method
public:
	crEnterSceneRoomMethod();
	crEnterSceneRoomMethod(const crEnterSceneRoomMethod& handle);
	MethodClass(CRNetApp, EnterSceneRoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvAboutToUseItemMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crRecvAboutToUseItemMethod();
	crRecvAboutToUseItemMethod(const crRecvAboutToUseItemMethod& handle);
	MethodClass(CRNetApp, RecvAboutToUseItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crItemUpdateTravelMethod : public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crItemUpdateTravelMethod();
//	crItemUpdateTravelMethod(const crItemUpdateTravelMethod& handle);
//	MethodClass(CRNetApp, ItemUpdateTravel)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	float m_dt;
//};
class crCheckPlayerGameNameMethod : public CRCore::crMethod
{//Client Method
public:
	crCheckPlayerGameNameMethod();
	crCheckPlayerGameNameMethod(const crCheckPlayerGameNameMethod& handle);
	MethodClass(CRNetApp, CheckPlayerGameName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_nameWidget;
};
class crNetReturnStreamMethod : public CRCore::crMethod
{//Client Method
public:
	crNetReturnStreamMethod();
	crNetReturnStreamMethod(const crNetReturnStreamMethod& handle);
	MethodClass(CRNetApp, NetReturnStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crChangeWidgetTextColorMethod : public CRCore::crMethod
{//Client Method
public:
	crChangeWidgetTextColorMethod();
	crChangeWidgetTextColorMethod(const crChangeWidgetTextColorMethod& handle);
	MethodClass(CRNetApp, ChangeWidgetTextColor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_nameWidget;
	CRCore::crVector4 m_color;
};
class crChangeWidgetTextColor2Method : public CRCore::crMethod
{//Client Method
public:
	crChangeWidgetTextColor2Method();
	crChangeWidgetTextColor2Method(const crChangeWidgetTextColor2Method& handle);
	MethodClass(CRNetApp, ChangeWidgetTextColor2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crTextAttrWidgetNode* m_this;
	CRCore::crVector4 m_color;
};
class crChangeWidgetTextColor3Method : public CRCore::crMethod
{//Client Method
public:
	crChangeWidgetTextColor3Method();
	crChangeWidgetTextColor3Method(const crChangeWidgetTextColor3Method& handle);
	MethodClass(CRNetApp, ChangeWidgetTextColor3)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_nameWidget;
	CRCore::crVector4 m_color;
};
class crEditWidgetInputSizeCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crEditWidgetInputSizeCheckMethod();
	crEditWidgetInputSizeCheckMethod(const crEditWidgetInputSizeCheckMethod& handle);
	MethodClass(CRNetApp, EditWidgetInputSizeCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode* m_this;
	int m_maxsize;
};
class crCreatePlayerGameDataMethod : public CRCore::crMethod
{//Client Method
public:
	crCreatePlayerGameDataMethod();
	crCreatePlayerGameDataMethod(const crCreatePlayerGameDataMethod& handle);
	MethodClass(CRNetApp, CreatePlayerGameData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crPlayerDataCreateStreamMethod : public CRCore::crMethod
{//Client Method
public:
	crPlayerDataCreateStreamMethod();
	crPlayerDataCreateStreamMethod(const crPlayerDataCreateStreamMethod& handle);
	MethodClass(CRNetApp, PlayerDataCreateStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crLoadLocalSceneMethod : public CRCore::crMethod
{//Client Method
public:
	crLoadLocalSceneMethod();
	crLoadLocalSceneMethod(const crLoadLocalSceneMethod& handle);
	MethodClass(CRNetApp, LoadLocalScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecordMousePosMethod : public CRCore::crMethod
{//Client Method
public:
	crRecordMousePosMethod();
	crRecordMousePosMethod(const crRecordMousePosMethod& handle);
	MethodClass(CRNetApp, RecordMousePos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};
class crInvalidMousePosMethod : public CRCore::crMethod
{//Client Method
public:
	crInvalidMousePosMethod();
	crInvalidMousePosMethod(const crInvalidMousePosMethod& handle);
	MethodClass(CRNetApp, InvalidMousePos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};
class crDrawDragRectMethod : public CRCore::crNodeMethod
{//Client Method
public:
	crDrawDragRectMethod();
	crDrawDragRectMethod(const crDrawDragRectMethod& handle);
	MethodClass(CRNetApp, DrawDragRect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void releaseObjects(CRCore::crState* state);
	virtual void swapBuffers(int frameNumber);
protected:
	void init();
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crVector4 m_lineColor;
	CRCore::ref_ptr<CRCore::crGeometry> m_rectGeo;
	CRCore::Vec3Array* m_coords;
	CRCore::Vec3Array* m_coords_buf;
	bool m_init;
};
class crRectSelectItemMethod : public CRCore::crMethod
{//Client Method
public:
	crRectSelectItemMethod();
	crRectSelectItemMethod(const crRectSelectItemMethod& handle);
	MethodClass(CRNetApp, RectSelectItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::Timer_t m_timer;
};
class crRectSelectNodeMethod : public CRCore::crMethod
{//Editor Method
public:
	crRectSelectNodeMethod();
	crRectSelectNodeMethod(const crRectSelectNodeMethod& handle);
	MethodClass(CRNetApp, RectSelectNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::Timer_t m_timer;
};
class crWaypointLinkMethod : public CRCore::crMethod
{//Editor Method
public:
	crWaypointLinkMethod();
	crWaypointLinkMethod(const crWaypointLinkMethod& handle);
	MethodClass(CRNetApp, WaypointLink)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_connect;//true表示connect,false表示disconnect
};
class crWaypointSaveMethod : public CRCore::crMethod
{//
public:
	crWaypointSaveMethod();
	crWaypointSaveMethod(const crWaypointSaveMethod& handle);
	MethodClass(CRNetApp, WaypointSave)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_filePath;
};
class crWaypointAutoSaveMethod : public CRCore::crMethod
{//
public:
	crWaypointAutoSaveMethod();
	crWaypointAutoSaveMethod(const crWaypointAutoSaveMethod& handle);
	MethodClass(CRNetApp, WaypointAutoSave)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fileName;
};
class crWaypointLoadMethod : public CRCore::crMethod
{//Editor Method
public:
	crWaypointLoadMethod();
	crWaypointLoadMethod(const crWaypointLoadMethod& handle);
	MethodClass(CRNetApp, WaypointLoad)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_filePath;
};
class crWaypointLinkDrawMethod : public CRCore::crNodeMethod
{//Editor Method
public:
	crWaypointLinkDrawMethod();
	crWaypointLinkDrawMethod(const crWaypointLinkDrawMethod& handle);
	MethodClass(CRNetApp, WaypointLinkDraw)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void releaseObjects(CRCore::crState* state);
	virtual void swapBuffers(int frameNumber);
protected:
	void init();
	CRCore::crMatrixTransform *m_this;
	CRCore::crVector4 m_lineColor;
	CRCore::ref_ptr<CRCore::crGeometry> m_rectGeo;
	CRCore::ref_ptr<CRCore::DrawArrays> m_drawArrays;
	CRCore::Vec3Array* m_coords;
	CRCore::Vec3Array* m_coords_buf;
	bool m_init;
	float m_zoffset;
};
class crWaypointPannedMethod : public CRCore::crMethod
{//Editor Method
public:
	crWaypointPannedMethod();
	crWaypointPannedMethod(const crWaypointPannedMethod& handle);
	MethodClass(CRNetApp, WaypointPanned)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crWaypointDeleteMethod : public CRCore::crMethod
{//Editor Method
public:
	crWaypointDeleteMethod();
	crWaypointDeleteMethod(const crWaypointDeleteMethod& handle);
	MethodClass(CRNetApp, WaypointDelete)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetWaypointMethod : public CRCore::crMethod
{//Editor Method
public:
	crSetWaypointMethod();
	crSetWaypointMethod(const crSetWaypointMethod& handle);
	MethodClass(CRNetApp, SetWaypoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crCameraFocusMethod : public CRCore::crMethod
{//Client Camera Method
public:
	crCameraFocusMethod();
	crCameraFocusMethod(const crCameraFocusMethod& handle);
	MethodClass(CRNetApp, CameraFocus)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRCore::crVector3 m_targetPos;
	bool m_inputPos;
};
class crCreateSightMethod : public CRCore::crMethod
{//Client Method
public:
	crCreateSightMethod();
	crCreateSightMethod(const crCreateSightMethod& handle);
	MethodClass(CRNetApp, CreateSight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvCreateSightMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crRecvCreateSightMethod();
	crRecvCreateSightMethod(const crRecvCreateSightMethod& handle);
	MethodClass(CRNetApp, RecvCreateSight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crSightFogUpdateMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crSightFogUpdateMethod();
//	crSightFogUpdateMethod(const crSightFogUpdateMethod& handle);
//	MethodClass(CRNetApp, SightFogUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	void init();
//	CRCore::ref_ptr<CRCore::crObject> m_object;
//	CRCore::ref_ptr<CRCore::crTessellationHints> m_hints;
//	std::string m_canvas;
//	std::string m_imageBox;
//	CRCore::crVector2i m_texSize;
//	float m_updInterval;
//	CRCore::crVector4 m_clearColor;
//	std::string m_textureFile;
//	float m_heightOffset;
//	bool m_init;
//};
class crSightFogInitMethod : public CRCore::crMethod
{//Client Method
public:
	crSightFogInitMethod();
	crSightFogInitMethod(const crSightFogInitMethod& handle);
	MethodClass(CRNetApp, SightFogInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector2i m_texSize;
	float m_updInterval;
	float m_precision;
	CRCore::crVector4f m_clearColor;
	std::string m_canvasName;
	std::string m_mapWidgetName;
};
class crSightFogUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crSightFogUpdateMethod();
	crSightFogUpdateMethod(const crSightFogUpdateMethod& handle);
	MethodClass(CRNetApp, SightFogUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void init();
	CRCore::ref_ptr<CRCore::crTessellationHints> m_hints;
	//void setEyeField(CRCore::crVector4f &eye,CRCore::crHeightField *eyeField,CRCore::crVector2 &origin);
	CRCore::ref_ptr<CRCore::crObject> m_object;
	CRCore::crVector4f m_sightColor;
	bool m_init;
};
//class crRecvPathFindingMethod : public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crRecvPathFindingMethod();
//	crRecvPathFindingMethod(const crRecvPathFindingMethod& handle);
//	MethodClass(CRNetApp, RecvPathFinding)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crCheckPlayerDelayTimeMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crCheckPlayerDelayTimeMethod();
	crCheckPlayerDelayTimeMethod(const crCheckPlayerDelayTimeMethod& handle);
	MethodClass(CRNetApp, CheckPlayerDelayTime)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData* m_this;
};
class crPositionSyncMethod : public CRCore::crMethod
{//Client & SceneServer Method
public:
	crPositionSyncMethod();
	crPositionSyncMethod(const crPositionSyncMethod& handle);
	MethodClass(CRNetApp, PositionSync)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	float m_dt;
	float m_interval;
	float m_time;
	CRCore::crVector3 m_lastSyncPos;
};
class crRecvPositionSyncMethod : public CRCore::crMethod
{//Client & Server Method
public:
	crRecvPositionSyncMethod();
	crRecvPositionSyncMethod(const crRecvPositionSyncMethod& handle);
	MethodClass(CRNetApp, RecvPositionSync)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crUseItemRecordMapUpdateMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crUseItemRecordMapUpdateMethod();
	crUseItemRecordMapUpdateMethod(const crUseItemRecordMapUpdateMethod& handle);
	MethodClass(CRNetApp, UseItemRecordMapUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crServerBrainHandle* m_this;
	float *m_dt;
	float m_delay;
};
//class crRecvStopPositionMethod : public CRCore::crMethod
//{//SceneServer & Client Method
//public:
//	crRecvStopPositionMethod();
//	crRecvStopPositionMethod(const crRecvStopPositionMethod& handle);
//	MethodClass(CRNetApp, RecvStopPosition)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem* m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crIsMainAIMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crIsMainAIMethod();
	crIsMainAIMethod(const crIsMainAIMethod& handle);
	MethodClass(CRNetApp, IsMainAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class crStopMyRolesMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crStopMyRolesMethod();
	crStopMyRolesMethod(const crStopMyRolesMethod& handle);
	MethodClass(CRNetApp, StopMyRoles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crInitRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crInitRoomProgressMethod();
	crInitRoomProgressMethod(const crInitRoomProgressMethod& handle);
	MethodClass(CRNetApp, InitRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
};
class crGetRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crGetRoomProgressMethod();
	crGetRoomProgressMethod(const crGetRoomProgressMethod& handle);
	MethodClass(CRNetApp, GetRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
};
class crAdvanceRoomProgressMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crAdvanceRoomProgressMethod();
	crAdvanceRoomProgressMethod(const crAdvanceRoomProgressMethod& handle);
	MethodClass(CRNetApp, AdvanceRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
};
class crRecvAdvanceRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crRecvAdvanceRoomProgressMethod();
	crRecvAdvanceRoomProgressMethod(const crRecvAdvanceRoomProgressMethod& handle);
	MethodClass(CRNetApp, RecvAdvanceRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crSetRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crSetRoomProgressMethod();
	crSetRoomProgressMethod(const crSetRoomProgressMethod& handle);
	MethodClass(CRNetApp, SetRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
	short m_progress;
};
class crReliveItemsMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crReliveItemsMethod();
	crReliveItemsMethod(const crReliveItemsMethod& handle);
	MethodClass(CRNetApp, ReliveItems)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	typedef std::vector<int>IDVec;
	IDVec m_itemidVec;
};
class crKillItemsMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crKillItemsMethod();
	crKillItemsMethod(const crKillItemsMethod& handle);
	MethodClass(CRNetApp, KillItems)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	typedef std::vector<int>IDVec;
	IDVec m_itemidVec;
};
class crNextRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crNextRoomProgressMethod();
	crNextRoomProgressMethod(const crNextRoomProgressMethod& handle);
	MethodClass(CRNetApp, NextRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
};
class crRoomSetRoomProgressMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crRoomSetRoomProgressMethod();
	crRoomSetRoomProgressMethod(const crRoomSetRoomProgressMethod& handle);
	MethodClass(CRNetApp, RoomSetRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	short m_progress;
};
class crTriggerSetRoomProgressMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crTriggerSetRoomProgressMethod();
	crTriggerSetRoomProgressMethod(const crTriggerSetRoomProgressMethod& handle);
	MethodClass(CRNetApp, TriggerSetRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	short m_progress;
};
class crRecvTriggerSetRoomProgressMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crRecvTriggerSetRoomProgressMethod();
	crRecvTriggerSetRoomProgressMethod(const crRecvTriggerSetRoomProgressMethod& handle);
	MethodClass(CRNetApp, RecvTriggerSetRoomProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crRoomProgressEventMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crRoomProgressEventMethod();
	crRoomProgressEventMethod(const crRoomProgressEventMethod& handle);
	MethodClass(CRNetApp, RoomProgressEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
};
class crScenarioEventMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crScenarioEventMethod();
	crScenarioEventMethod(const crScenarioEventMethod& handle);
	MethodClass(CRNetApp, ScenarioEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	int m_msgid;
};
class crIFItemsDeadMethod : public CRCore::crMethod
{//SceneServer Method 
public:
	crIFItemsDeadMethod();
	crIFItemsDeadMethod(const crIFItemsDeadMethod& handle);
	MethodClass(CRNetApp, IFItemsDead)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	typedef std::vector<int>IDVec;
	IDVec m_itemidVec;
};
class crShowSceneNodesMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crShowSceneNodesMethod();
	crShowSceneNodesMethod(const crShowSceneNodesMethod& handle);
	MethodClass(CRNetApp, ShowSceneNodes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<std::string>StrVec;
	int m_show;
	int m_nodetype;
	StrVec m_nodeNameVec;
};
class crClientAddEyePointsMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crClientAddEyePointsMethod();
	crClientAddEyePointsMethod(const crClientAddEyePointsMethod& handle);
	MethodClass(CRNetApp, ClientAddEyePoints)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<CRCore::crVector2i>CoordVec;
	CoordVec m_coordVec;
};
class crClientRemoveEyePointsMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crClientRemoveEyePointsMethod();
	crClientRemoveEyePointsMethod(const crClientRemoveEyePointsMethod& handle);
	MethodClass(CRNetApp, ClientRemoveEyePoints)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<CRCore::crVector2i>CoordVec;
	CoordVec m_coordVec;
};
class crSetRoomAutoStartMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crSetRoomAutoStartMethod();
	crSetRoomAutoStartMethod(const crSetRoomAutoStartMethod& handle);
	MethodClass(CRNetApp, SetRoomAutoStart)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	bool m_autoStart;
	float m_autoStartTime;
	float m_removeTime;
};
//class crDynamicCollideCheckMethod :public CRCore::crMethod
//{//GameClient Method
//public:
//	crDynamicCollideCheckMethod();
//	crDynamicCollideCheckMethod(const crDynamicCollideCheckMethod& handle);
//	MethodClass(CRNetApp, DynamicCollideCheck)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	crInstanceItem *m_this;
//	unsigned char m_guisestate;
//	float m_dt;
//	float m_noneblockInterval;
//	float m_timer;
//};
class crSyncPosEmptyMethod :public CRCore::crMethod
{//GameClient Method
public:
	crSyncPosEmptyMethod();
	crSyncPosEmptyMethod(const crSyncPosEmptyMethod& handle);
	MethodClass(CRNetApp, SyncPosEmpty)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
};
class crDynamicChangePathMethod :public CRCore::crMethod
{//GameClient Method
public:
	crDynamicChangePathMethod();
	crDynamicChangePathMethod(const crDynamicChangePathMethod& handle);
	MethodClass(CRNetApp, DynamicChangePath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
};
class crSetRemainTimeMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crSetRemainTimeMethod();
	crSetRemainTimeMethod(const crSetRemainTimeMethod& handle);
	MethodClass(CRNetApp, SetRemainTime)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
	float m_remainTime;
};
class crIsRoomPlayerEmptyMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crIsRoomPlayerEmptyMethod();
	crIsRoomPlayerEmptyMethod(const crIsRoomPlayerEmptyMethod& handle);
	MethodClass(CRNetApp, IsRoomPlayerEmpty)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom *m_this;
};
class crQueryServerListMethod :public CRCore::crMethod
{//GameClient Method
public:
	crQueryServerListMethod();
	crQueryServerListMethod(const crQueryServerListMethod& handle);
	MethodClass(CRNetApp, QueryServerList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRecvConnectServerMethod :public CRCore::crMethod
{//GameClient Method
public:
	crRecvConnectServerMethod();
	crRecvConnectServerMethod(const crRecvConnectServerMethod& handle);
	MethodClass(CRNetApp, RecvConnectServer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crLoginGameNewMethod : public CRCore::crMethod
{//GameClient Method
public:
	crLoginGameNewMethod();
	crLoginGameNewMethod(const crLoginGameNewMethod& handle);
	MethodClass(CRNetApp, LoginGameNew)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_callback;
	std::string m_servername;
	std::string m_password;
};
class crInitDownloadMethod : public CRCore::crMethod
{//GameClient Method
public:
	crInitDownloadMethod();
	crInitDownloadMethod(const crInitDownloadMethod& handle);
	MethodClass(CRNetApp, InitDownload)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetDownloadThreadCountMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetDownloadThreadCountMethod();
	crSetDownloadThreadCountMethod(const crSetDownloadThreadCountMethod& handle);
	MethodClass(CRNetApp, SetDownloadThreadCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_threadcount;
};
class crFiniDownloadMethod : public CRCore::crMethod
{//GameClient Method
public:
	crFiniDownloadMethod();
	crFiniDownloadMethod(const crFiniDownloadMethod& handle);
	MethodClass(CRNetApp, FiniDownload)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crDownloadFilesMethod : public CRCore::crMethod
{//GameClient Method
public:
	crDownloadFilesMethod();
	crDownloadFilesMethod(const crDownloadFilesMethod& handle);
	MethodClass(CRNetApp, DownloadFiles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<std::string> StrVec;
	StrVec m_fileList;
};
class crCreRestartMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCreRestartMethod();
	crCreRestartMethod(const crCreRestartMethod& handle);
	MethodClass(CRNetApp, CreRestart)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_code;
};
class crCloseRunProtectedHandleMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCloseRunProtectedHandleMethod();
	crCloseRunProtectedHandleMethod(const crCloseRunProtectedHandleMethod& handle);
	MethodClass(CRNetApp, CloseRunProtectedHandle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crDownloadUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crDownloadUpdateMethod();
	crDownloadUpdateMethod(const crDownloadUpdateMethod& handle);
	MethodClass(CRNetApp, DownloadUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crLoadingProgressMethod : public CRCore::crMethod
{//GameClient Method
public:
	crLoadingProgressMethod();
	crLoadingProgressMethod(const crLoadingProgressMethod& handle);
	MethodClass(CRNetApp, LoadingProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crProgressElement* m_this;
	float m_dt;
	float m_speed;
};
class crSetFocusNodeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetFocusNodeMethod();
	crSetFocusNodeMethod(const crSetFocusNodeMethod& handle);
	MethodClass(CRNetApp, SetFocusNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_focusWidget;
};
class crSetDefFocusIndexMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetDefFocusIndexMethod();
	crSetDefFocusIndexMethod(const crSetDefFocusIndexMethod& handle);
	MethodClass(CRNetApp, SetDefFocusIndex)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	int m_index;
};
class crSendRoomTimeRemainMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crSendRoomTimeRemainMethod();
	crSendRoomTimeRemainMethod(const crSendRoomTimeRemainMethod& handle);
	MethodClass(CRNetApp, SendRoomTimeRemain)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom* m_this;
};
class crRoomUpdate2Method : public CRCore::crMethod
{//SceneServer Method
public:
	crRoomUpdate2Method();
	crRoomUpdate2Method(const crRoomUpdate2Method& handle);
	MethodClass(CRNetApp, RoomUpdate2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crRoom* m_this;
	float *m_dt;
	crSceneServerCallback *m_sc;
};
class crDynamicCollideExtraMethod :public CRCore::crMethod
{//GameClient Method
public:
	crDynamicCollideExtraMethod();
	crDynamicCollideExtraMethod(const crDynamicCollideExtraMethod& handle);
	MethodClass(CRNetApp, DynamicCollideExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
	float* m_dt;
	float m_timer;
	float m_interval;
};
class crItemDeadCheckMethod :public CRCore::crMethod
{//Server & GameClient Method
public:
	crItemDeadCheckMethod();
	crItemDeadCheckMethod(const crItemDeadCheckMethod& handle);
	MethodClass(CRNetApp, ItemDeadCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem *m_this;
};
class crRobotLoginGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRobotLoginGameMethod();
	crRobotLoginGameMethod(const crRobotLoginGameMethod& handle);
	MethodClass(CRNetApp, RobotLoginGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void doConnect();
protected:
	std::string m_callback;
	std::string m_servername;
	std::string m_password;
	int m_count;
	int m_threadcount;
};
class crRegisterTestAccountMethod : public CRCore::crMethod
{//GameClient Method
public:
	crRegisterTestAccountMethod();
	crRegisterTestAccountMethod(const crRegisterTestAccountMethod& handle);
	MethodClass(CRNetApp, RegisterTestAccount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_preName;
	int m_start;
	int m_end;
	std::string m_password;
};
class crWriteServerInfoMethod : public CRCore::crMethod
{//Server Method
public:
	crWriteServerInfoMethod();
	crWriteServerInfoMethod(const crWriteServerInfoMethod& handle);
	MethodClass(CRNetApp, WriteServerInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int get_cpu_usage();
	int get_memory_usage();
	typedef std::vector<std::string> ServerInfoVec;
	int m_minCount;
	int m_writeCount;
	bool m_init;
	std::string m_filename;
	std::string m_filepath;
	CREncapsulation::crTableIO::DataVec m_serverInfoData;
	int m_columncount;
};
class crSetGamelogPathMethod : public CRCore::crMethod
{//Server Method
public:
	crSetGamelogPathMethod();
	crSetGamelogPathMethod(const crSetGamelogPathMethod& handle);
	MethodClass(CRNetApp, SetGamelogPath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_filepath;
	std::string m_serverName;
};
class crGameTaskUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crGameTaskUpdateMethod();
	crGameTaskUpdateMethod(const crGameTaskUpdateMethod& handle);
	MethodClass(CRNetApp, GameTaskUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float* m_dt;
};
class crShellExecuteMethod :public CRCore::crMethod
{//Client Method
public:
	crShellExecuteMethod();
	crShellExecuteMethod(const crShellExecuteMethod& handle);
	MethodClass(CRNetApp, ShellExecute)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_url;
	int m_show;
};
class crClipCursorMethod :public CRCore::crMethod
{//Client Method
public:
	crClipCursorMethod();
	crClipCursorMethod(const crClipCursorMethod& handle);
	MethodClass(CRNetApp, ClipCursor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_lock;
};
class crIsStartupLoadingMethod : public CRCore::crMethod
{//Client Method
public:
	crIsStartupLoadingMethod();
	crIsStartupLoadingMethod(const crIsStartupLoadingMethod& handle);
	MethodClass(CRNetApp, IsStartupLoading)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crStartupLoadingInitMethod : public CRCore::crMethod
{//Client Method
public:
	crStartupLoadingInitMethod();
	crStartupLoadingInitMethod(const crStartupLoadingInitMethod& handle);
	MethodClass(CRNetApp, StartupLoadingInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_progressTextWidget;
	std::string m_progressWidget;
};
class crStartupLoadingProgressMethod : public CRCore::crMethod
{//Client Method
public:
	crStartupLoadingProgressMethod();
	crStartupLoadingProgressMethod(const crStartupLoadingProgressMethod& handle);
	MethodClass(CRNetApp, StartupLoadingProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_dt;
	std::string m_canvasName;
	std::string m_progressTextWidget;
	std::string m_progressWidget;
	float m_speed;
};
class crSetFpsControlMethod : public CRCore::crMethod
{//Client Method
public:
	crSetFpsControlMethod();
	crSetFpsControlMethod(const crSetFpsControlMethod& handle);
	MethodClass(CRNetApp, SetFpsControl)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_fps;
};
class crStartupSceneDownloadMethod : public CRCore::crMethod
{//GameClient Method
public:
	crStartupSceneDownloadMethod();
	crStartupSceneDownloadMethod(const crStartupSceneDownloadMethod& handle);
	MethodClass(CRNetApp, StartupSceneDownload)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_step;//0 1
	std::string m_frontname;
};
class crIsStartupSceneDownloadingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crIsStartupSceneDownloadingMethod();
	crIsStartupSceneDownloadingMethod(const crIsStartupSceneDownloadingMethod& handle);
	MethodClass(CRNetApp, IsStartupSceneDownloading)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_step;//0 1
};
class crBeginRunGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crBeginRunGameMethod();
	crBeginRunGameMethod(const crBeginRunGameMethod& handle);
	MethodClass(CRNetApp, BeginRunGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//class crRechargeDisposeMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crRechargeDisposeMethod();
//	crRechargeDisposeMethod(const crRechargeDisposeMethod& handle);
//	MethodClass(CRNetApp, RechargeDispose)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	int m_count;
//};
class crMoneyChangeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crMoneyChangeMethod();
	crMoneyChangeMethod(const crMoneyChangeMethod& handle);
	MethodClass(CRNetApp, MoneyChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crPlayerGameData *m_this;
	MoneyChangeData m_moneydata;//+表示获得，－表示消费
	bool *m_output;
};
class crIsWebLoginMethod : public CRCore::crMethod
{//GameClient Method
public:
	crIsWebLoginMethod();
	crIsWebLoginMethod(const crIsWebLoginMethod& handle);
	MethodClass(CRNetApp, IsWebLogin)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crDoWebLoginMethod : public CRCore::crMethod
{//GameClient Method
public:
	crDoWebLoginMethod();
	crDoWebLoginMethod(const crDoWebLoginMethod& handle);
	MethodClass(CRNetApp, DoWebLogin)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSetVolumeEnableMethod : public CRCore::crMethod
{//Client Method
public:
	crSetVolumeEnableMethod();
	crSetVolumeEnableMethod(const crSetVolumeEnableMethod& handle);
	MethodClass(CRNetApp, SetVolumeEnable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_name;
	bool m_enable;
};
class crIsAlphaShadowDisabledMethod : public CRCore::crMethod
{//Client Method
public:
	crIsAlphaShadowDisabledMethod();
	crIsAlphaShadowDisabledMethod(const crIsAlphaShadowDisabledMethod& handle);
	MethodClass(CRNetApp, IsAlphaShadowDisabled)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crStopAllSceneFxMethod : public CRCore::crMethod
{//Client Method
public:
	crStopAllSceneFxMethod();
	crStopAllSceneFxMethod(const crStopAllSceneFxMethod& handle);
	MethodClass(CRNetApp, StopAllSceneFx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crGainBirthPointMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crGainBirthPointMethod();
	crGainBirthPointMethod(const crGainBirthPointMethod& handle);
	MethodClass(CRNetApp, GainBirthPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	crRole *m_role;
	int m_index;
};
class crLockKeyboardMouseMethod : public CRCore::crMethod
{//Client Method
public:
	crLockKeyboardMouseMethod();
	crLockKeyboardMouseMethod(const crLockKeyboardMouseMethod& handle);
	MethodClass(CRNetApp, LockKeyboardMouse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_lock;
};
class crSetLightValidDistanceMethod : public CRCore::crMethod
{//Client Method
public:
	crSetLightValidDistanceMethod();
	crSetLightValidDistanceMethod(const crSetLightValidDistanceMethod& handle);
	MethodClass(CRNetApp, SetLightValidDistance)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_dist;
};
class crShowEnginStatsMethod : public CRCore::crMethod
{//Client Method
public:
	crShowEnginStatsMethod();
	crShowEnginStatsMethod(const crShowEnginStatsMethod& handle);
	MethodClass(CRNetApp, ShowEnginStats)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crVector2i m_pos;
	void init();
	bool m_init;
	CRCore::ref_ptr<CRText::crText> m_fps;
	CRCore::ref_ptr<CRText::crText> m_fpsTime;
	CRCore::ref_ptr<CRText::crText> m_drawTime;
	CRCore::ref_ptr<CRText::crText> m_updTime;
	CRCore::ref_ptr<CRText::crText> m_cullTime;
	CRCore::ref_ptr<CRText::crText> m_phyUpdTime;
	CRCore::ref_ptr<CRText::crText> m_characterUpdTime;
	CRCore::ref_ptr<CRText::crText> m_particleUpdTime;
	CRCore::ref_ptr<CRText::crText> m_keyboardMouseTime;
	CRCore::ref_ptr<CRText::crText> m_swapTime;
	CRCore::ref_ptr<CRText::crText> m_numStates;
	CRCore::ref_ptr<CRText::crText> m_numDrawables;
	CRCore::ref_ptr<CRText::crText> m_primCount;
	CRCore::ref_ptr<CRText::crText> m_vtxCount;
	CRCore::ref_ptr<CRText::crText> m_particleCount;
	CRCore::ref_ptr<CRText::crText> m_lightCount;
	CRCore::ref_ptr<CRText::crText> m_phyCount;
	CRCore::ref_ptr<CRText::crText> m_textureCount;
};
}
#endif