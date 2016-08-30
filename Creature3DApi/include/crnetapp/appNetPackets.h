/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#ifndef CRNETAPP_NETPACKET_H
#define CRNETAPP_NETPACKET_H 1

#include <CRNetApp/appExport.h>
#include <CRNet\crStreamPacket.h>
#include <CRNet\crNetContainer.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appAccountData.h>
#include <CRNetApp/appNetChatData.h>
namespace CRNetApp {
//服务器拒绝连接，客户端受到该包后停止自动重连机制
class CRNETAPP_EXPORT crAppServerRefusePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAppServerRefusePacket> sptr;
	typedef GNE::WeakPtr<crAppServerRefusePacket> wptr;
public :
	crAppServerRefusePacket();
	crAppServerRefusePacket( const crAppServerRefusePacket &p );

	virtual ~crAppServerRefusePacket() {}
public:
	static const int ID = WCHNET_APPSERVERREFUSE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crAppServerRefusePacket &packet, int recvid/*, const std::string &playerAddress*/);//-1表示同名客户端登陆
	static void buildRequestPacket2(crAppServerRefusePacket &packet,int code);//-1表示同名客户端登陆
};
///////crAppLogoutPacket
class CRNETAPP_EXPORT crAppLogoutPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAppLogoutPacket> sptr;
	typedef GNE::WeakPtr<crAppLogoutPacket> wptr;
public :
	crAppLogoutPacket();
	crAppLogoutPacket( const crAppLogoutPacket &p );

	virtual ~crAppLogoutPacket() {}
public:
	static const int ID = WCHNET_APPLOGOUT;
public:
	virtual void parsePacket(const std::string &sender);
};

class CRNETAPP_EXPORT crLogoutScenePacket : public CRNet::crStreamPacket
{//
public:
	typedef GNE::SmartPtr<crLogoutScenePacket> sptr;
	typedef GNE::WeakPtr<crLogoutScenePacket> wptr;
public :
	crLogoutScenePacket();
	crLogoutScenePacket( const crLogoutScenePacket &p );

	virtual ~crLogoutScenePacket() {}
public:
	static const int ID = WCHNET_LOGOUTSCENE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLogoutScenePacket &packet,int playerid);
};

///////crLoginPacket
class CRNETAPP_EXPORT crLoginPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginPacket> sptr;
	typedef GNE::WeakPtr<crLoginPacket> wptr;
public :
	crLoginPacket();
	crLoginPacket( const crLoginPacket &p );

	virtual ~crLoginPacket() {}
public:
	static const int ID = WCHNET_LOGIN;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginPacket &packet,std::string &userName,std::string &userPassword);
	static void buildRequestPacket2(crLoginPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
	//static void buildReplyPacket(crLoginPacket &packet,const std::string &address,char loginCode,int playerid,/*const std::string &playerName,*/const std::string &key,crAccountData *accountData);
	static void buildReplyPacket(crLoginPacket &packet,const std::string &address,char loginCode,int playerid,int lastserverid,const std::string &key);
	static void buildReplyPacket2(crLoginPacket &packet,CRCore::crStreamBuf *stream);
	//static int s_success;
};
////////crLoginChatPacket
class CRNETAPP_EXPORT crLoginChatPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginChatPacket> sptr;
	typedef GNE::WeakPtr<crLoginChatPacket> wptr;
public :
	crLoginChatPacket();
	crLoginChatPacket( const crLoginChatPacket &p );

	virtual ~crLoginChatPacket() {}
public:
	static const int ID = WCHNET_LOGINCHAT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginChatPacket &packet);
	static void buildRequestPacket2(crLoginChatPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crLoginChatPacket &packet,const std::string &address,char loginCode,crChatServerPlayerData *playerData);
	static void buildReplyPacket2(crLoginChatPacket &packet,char loginCode);
	void chatPlayerDataQuery(crChatServerPlayerData *playerData, CRDataBase::crDataBase *globalSession, CRDataBase::crDataBase *chatSession);
	//static int s_success;
};

class CRNETAPP_EXPORT crLoginParentPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginParentPacket> sptr;
	typedef GNE::WeakPtr<crLoginParentPacket> wptr;
public :
	crLoginParentPacket();
	crLoginParentPacket( const crLoginParentPacket &p );

	virtual ~crLoginParentPacket() {}
public:
	static const int ID = WCHNET_LOGINPARENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginParentPacket &packet,int playerid,const std::string &playerAddress);
};

//class CRNETAPP_EXPORT crLoginVoicePacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crLoginVoicePacket> sptr;
//	typedef GNE::WeakPtr<crLoginVoicePacket> wptr;
//public :
//	crLoginVoicePacket();
//	crLoginVoicePacket( const crLoginVoicePacket &p );
//
//	virtual ~crLoginVoicePacket() {}
//public:
//	static const int ID = WCHNET_LOGINVOICE;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crLoginVoicePacket &packet);
//	static void buildRequestPacket2(crLoginVoicePacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket(crLoginVoicePacket &packet,const std::string &address,char loginCode,crGameServerPlayerData *playerData);
//	static void buildReplyPacket2(crLoginVoicePacket &packet,char loginCode);
//	static int s_success;
//};
//
//class CRNETAPP_EXPORT crLoginCameraPacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crLoginCameraPacket> sptr;
//	typedef GNE::WeakPtr<crLoginCameraPacket> wptr;
//public :
//	crLoginCameraPacket();
//	crLoginCameraPacket( const crLoginCameraPacket &p );
//
//	virtual ~crLoginCameraPacket() {}
//public:
//	static const int ID = WCHNET_LOGINCAMERA;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crLoginCameraPacket &packet);
//	static void buildRequestPacket2(crLoginCameraPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket(crLoginCameraPacket &packet,const std::string &address,char loginCode,crGameServerPlayerData *playerData);
//	static void buildReplyPacket2(crLoginCameraPacket &packet,char loginCode);
//	static int s_success;
//};

//////chat packet
class CRNETAPP_EXPORT crChatStateChangePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatStateChangePacket> sptr;
	typedef GNE::WeakPtr<crChatStateChangePacket> wptr;
public :
	crChatStateChangePacket();
	crChatStateChangePacket( const crChatStateChangePacket &p );

	virtual ~crChatStateChangePacket() {}
public:
	static const int ID = WCHNET_CHATSTATECHANGE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatStateChangePacket &packet,int senderid, int recvid, char chatState,const std::string &nickName);
};

class CRNETAPP_EXPORT crCreateDataStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreateDataStreamPacket> sptr;
	typedef GNE::WeakPtr<crCreateDataStreamPacket> wptr;
public :
	crCreateDataStreamPacket();
	crCreateDataStreamPacket( const crCreateDataStreamPacket &p );

	virtual ~crCreateDataStreamPacket() {}
public:
	static const int ID = WCHNET_CREATEDATASTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateDataStreamPacket &packet,int recvid,int streamid/*or senderid*/,int packetid,int count);
	static void buildReplyPacket(crCreateDataStreamPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crDataStreamPacket : public CRNet::crStreamPacket
{//用于大数据流传递
public:
	typedef GNE::SmartPtr<crDataStreamPacket> sptr;
	typedef GNE::WeakPtr<crDataStreamPacket> wptr;
public :
	crDataStreamPacket();
	crDataStreamPacket( const crDataStreamPacket &p );

	virtual ~crDataStreamPacket() {}
public:
	static const int ID = WCHNET_DATASTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crDataStreamPacket &packet,int recvid,int streamid/*or senderid*/,int packetid,CRCore::crStreamBuf *buf);
	static void buildRequestPacket(crDataStreamPacket &packet,const std::string &address,int streamid, int packetid,CRCore::crStreamBuf *buf);
	static void buildReplyPacket(crDataStreamPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crChatFriendListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatFriendListPacket> sptr;
	typedef GNE::WeakPtr<crChatFriendListPacket> wptr;
public :
	crChatFriendListPacket();
	crChatFriendListPacket( const crChatFriendListPacket &p );

	virtual ~crChatFriendListPacket() {}
public:
	static const int ID = WCHNET_CHATFRIENDLIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatFriendListPacket &packet,int recvid, int streamid, int count);
	static void buildRequestPacket2(crChatFriendListPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crChatGroupListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatGroupListPacket> sptr;
	typedef GNE::WeakPtr<crChatGroupListPacket> wptr;
public :
	crChatGroupListPacket();
	crChatGroupListPacket( const crChatGroupListPacket &p );

	virtual ~crChatGroupListPacket() {}
public:
	static const int ID = WCHNET_CHATGROUPLIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatGroupListPacket &packet,int recvid,int streamid,int count);
	static void buildRequestPacket2(crChatGroupListPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crChatGroupMemberListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatGroupMemberListPacket> sptr;
	typedef GNE::WeakPtr<crChatGroupMemberListPacket> wptr;
public :
	crChatGroupMemberListPacket();
	crChatGroupMemberListPacket( const crChatGroupMemberListPacket &p );

	virtual ~crChatGroupMemberListPacket() {}
public:
	static const int ID = WCHNET_CHATGROUPMEMBERLIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatGroupMemberListPacket &packet,int recvid,int streamid,int count);
	static void buildRequestPacket2(crChatGroupMemberListPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crChatDataTransferEndPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatDataTransferEndPacket> sptr;
	typedef GNE::WeakPtr<crChatDataTransferEndPacket> wptr;
public :
	crChatDataTransferEndPacket();
	crChatDataTransferEndPacket( const crChatDataTransferEndPacket &p );

	virtual ~crChatDataTransferEndPacket() {}
public:
	static const int ID = WCHNET_CHATDATATRANSFEREND;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatDataTransferEndPacket &packet,int recvid);
};

class CRNETAPP_EXPORT crAddChatFriendPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAddChatFriendPacket> sptr;
	typedef GNE::WeakPtr<crAddChatFriendPacket> wptr;
public :
	crAddChatFriendPacket();
	crAddChatFriendPacket( const crAddChatFriendPacket &p );

	virtual ~crAddChatFriendPacket() {}
public:
	static const int ID = WCHNET_ADDCHATFRIEND;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crAddChatFriendPacket &packet,int friendPlayerid);
	static void buildRequestPacket2(crAddChatFriendPacket &packet,int myPlayerid, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crAddChatFriendPacket &packet,int recvid, unsigned char befriendvalid, crNetChatData::crChatFriend *chatFriend);
	static void buildReplyPacket2(crAddChatFriendPacket &packet, CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRemoveChatFriendPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRemoveChatFriendPacket> sptr;
	typedef GNE::WeakPtr<crRemoveChatFriendPacket> wptr;
public :
	crRemoveChatFriendPacket();
	crRemoveChatFriendPacket( const crRemoveChatFriendPacket &p );

	virtual ~crRemoveChatFriendPacket() {}
public:
	static const int ID = WCHNET_REMOVECHATFRIEND;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRemoveChatFriendPacket &packet,int friendPlayerid);
	static void buildRequestPacket2(crRemoveChatFriendPacket &packet,int myPlayerid, int friendPlayerid);
};

class CRNETAPP_EXPORT crChatPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crChatPacket> sptr;
	typedef GNE::WeakPtr<crChatPacket> wptr;
public :
	crChatPacket();
	crChatPacket( const crChatPacket &p );

	virtual ~crChatPacket() {}
public:
	static const int ID = WCHNET_CHAT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crChatPacket &packet,int recvid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crChatPacket &packet,int senderid,CRCore::crStreamBuf *stream);
    static void buildReplyPacket(crChatPacket &packet,int senderid, int recvid, /*const std::string &time,*/CRCore::crStreamBuf *stream);
	static void buildRequestPacket(crChatPacket &packet,int senderid, int recvid, unsigned short msg, CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crChatPacket &packet,int senderid, int recvid, unsigned short msg, /*const std::string &time,*/ CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crChatPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crGroupChatPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crGroupChatPacket> sptr;
	typedef GNE::WeakPtr<crGroupChatPacket> wptr;
public :
	crGroupChatPacket();
	crGroupChatPacket( const crGroupChatPacket &p );

	virtual ~crGroupChatPacket() {}
public:
	static const int ID = WCHNET_GROUPCHAT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crGroupChatPacket &packet,int playergroupid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crGroupChatPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crGroupChatPacket &packet,int senderid, int recvid,int groupid,/*const std::string &time,*/CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crGroupChatPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crApplyJoinGroupPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crApplyJoinGroupPacket> sptr;
	typedef GNE::WeakPtr<crApplyJoinGroupPacket> wptr;
public :
	crApplyJoinGroupPacket();
	crApplyJoinGroupPacket( const crApplyJoinGroupPacket &p );

	virtual ~crApplyJoinGroupPacket() {}
public:
	static const int ID = WCHNET_APPLYJOINGROUP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crApplyJoinGroupPacket &packet,int groupid);
	static void buildRequestPacket2(crApplyJoinGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crApplyJoinGroupPacket &packet,int senderid, int recvid,int groupid/*,const std::string &time*/);
	void sendToGroupManager(int senderid,int recvid,int groupid,/*const std::string &time,*/ CRDataBase::crDataBase *global_query,crChatServerPlayerData *senderPlayerData);
};

class CRNETAPP_EXPORT crAcceptJoinGroupPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAcceptJoinGroupPacket> sptr;
	typedef GNE::WeakPtr<crAcceptJoinGroupPacket> wptr;
public :
	crAcceptJoinGroupPacket();
	crAcceptJoinGroupPacket( const crAcceptJoinGroupPacket &p );

	virtual ~crAcceptJoinGroupPacket() {}
public:
	static const int ID = WCHNET_ACCEPTJOINGROUP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crAcceptJoinGroupPacket &packet,int recvid,int groupid);
	static void buildRequestPacket2(crAcceptJoinGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crAcceptJoinGroupPacket &packet,int senderid, int recvid,int groupid/*,const std::string &time*/);
};

class CRNETAPP_EXPORT crNewMemberComingPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crNewMemberComingPacket> sptr;
	typedef GNE::WeakPtr<crNewMemberComingPacket> wptr;
public :
	crNewMemberComingPacket();
	crNewMemberComingPacket( const crNewMemberComingPacket &p );

	virtual ~crNewMemberComingPacket() {}
public:
	static const int ID = WCHNET_NEWMEMBERCOMING;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crNewMemberComingPacket &packet,int groupid);
	static void buildRequestPacket2(crNewMemberComingPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crNewMemberComingPacket &packet,int senderid, int recvid,int groupid,const std::string &time);
};

class CRNETAPP_EXPORT crAddChatGroupPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAddChatGroupPacket> sptr;
	typedef GNE::WeakPtr<crAddChatGroupPacket> wptr;
public :
	crAddChatGroupPacket();
	crAddChatGroupPacket( const crAddChatGroupPacket &p );

	virtual ~crAddChatGroupPacket() {}
public:
	static const int ID = WCHNET_ADDCHATGROUP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crAddChatGroupPacket &packet,int recvid,crNetChatData::crChatGroup *chatGroup);
	static void buildRequestPacket2(crAddChatGroupPacket &packet,int count);
};

class CRNETAPP_EXPORT crAddGroupMemberPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crAddGroupMemberPacket> sptr;
	typedef GNE::WeakPtr<crAddGroupMemberPacket> wptr;
public :
	crAddGroupMemberPacket();
	crAddGroupMemberPacket( const crAddGroupMemberPacket &p );

	virtual ~crAddGroupMemberPacket() {}
public:
	static const int ID = WCHNET_ADDGROUPMEMBER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crAddGroupMemberPacket &packet,int senderid, int recvid, int groupid,int senderMemberID,char chatState,const std::string &nickName);
};

class CRNETAPP_EXPORT crExitGroupPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crExitGroupPacket> sptr;
	typedef GNE::WeakPtr<crExitGroupPacket> wptr;
public :
	crExitGroupPacket();
	crExitGroupPacket( const crExitGroupPacket &p );

	virtual ~crExitGroupPacket() {}
public:
	static const int ID = WCHNET_EXITGROUP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crExitGroupPacket &packet,int groupid);
	static void buildRequestPacket2(crExitGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRemoveGroupMemberPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRemoveGroupMemberPacket> sptr;
	typedef GNE::WeakPtr<crRemoveGroupMemberPacket> wptr;
public :
	crRemoveGroupMemberPacket();
	crRemoveGroupMemberPacket( const crRemoveGroupMemberPacket &p );

	virtual ~crRemoveGroupMemberPacket() {}
public:
	static const int ID = WCHNET_REMOVEGROUPMEMBER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRemoveGroupMemberPacket &packet,int senderid, int recvid, int groupid);
};

class CRNETAPP_EXPORT crQueryOnlinePlayerListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryOnlinePlayerListPacket> sptr;
	typedef GNE::WeakPtr<crQueryOnlinePlayerListPacket> wptr;
public :
	crQueryOnlinePlayerListPacket();
	crQueryOnlinePlayerListPacket( const crQueryOnlinePlayerListPacket &p );

	virtual ~crQueryOnlinePlayerListPacket() {}
public:
	static const int ID = WCHNET_QUERYONLINEPLAYERLIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryOnlinePlayerListPacket &packet,int rows,int offset);
	static void buildRequestPacket2(crQueryOnlinePlayerListPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQueryOnlinePlayerListPacket &packet,int senderid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQueryChatGroupListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryChatGroupListPacket> sptr;
	typedef GNE::WeakPtr<crQueryChatGroupListPacket> wptr;
public :
	crQueryChatGroupListPacket();
	crQueryChatGroupListPacket( const crQueryChatGroupListPacket &p );

	virtual ~crQueryChatGroupListPacket() {}
public:
	static const int ID = WCHNET_QUERYCHATGROUPLIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryChatGroupListPacket &packet,int rows,int offset);
	static void buildRequestPacket2(crQueryChatGroupListPacket &packet,int senderid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQueryChatGroupListPacket &packet,int senderid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRegisterAccountPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRegisterAccountPacket> sptr;
	typedef GNE::WeakPtr<crRegisterAccountPacket> wptr;
public :
	crRegisterAccountPacket();
	crRegisterAccountPacket( const crRegisterAccountPacket &p );

	virtual ~crRegisterAccountPacket() {}
public:
	static const int ID = WCHNET_REGACCOUNT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRegisterAccountPacket &packet, std::string &name, std::string &password, std::string &email, std::string &nickname, std::string &relname, char sex,std::string &code);
	static void buildRequestPacket2(crRegisterAccountPacket &packet,const std::string &address, CRCore::crStreamBuf *buf);
	static void buildReplyPacket(crRegisterAccountPacket &packet,const std::string &address,char ret,const std::string &validdate);
	static void buildReplyPacket2(crRegisterAccountPacket &packet,CRCore::crStreamBuf* buf);
	//static int s_success;
};

class CRNETAPP_EXPORT crDeleteAccountPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crDeleteAccountPacket> sptr;
	typedef GNE::WeakPtr<crDeleteAccountPacket> wptr;
public :
	crDeleteAccountPacket();
	crDeleteAccountPacket( const crDeleteAccountPacket &p );

	virtual ~crDeleteAccountPacket() {}
public:
	static const int ID = WCHNET_DELACCOUNT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crDeleteAccountPacket &packet, const std::string &name);
};

class CRNETAPP_EXPORT crUpdateAccountPasswordPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crUpdateAccountPasswordPacket> sptr;
	typedef GNE::WeakPtr<crUpdateAccountPasswordPacket> wptr;
public :
	crUpdateAccountPasswordPacket();
	crUpdateAccountPasswordPacket( const crUpdateAccountPasswordPacket &p );

	virtual ~crUpdateAccountPasswordPacket() {}
public:
	static const int ID = WCHNET_UPDPASSWORD;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crUpdateAccountPasswordPacket &packet, int playerid, const std::string &password);
};

class CRNETAPP_EXPORT crCreateChatGroupPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreateChatGroupPacket> sptr;
	typedef GNE::WeakPtr<crCreateChatGroupPacket> wptr;
public :
	crCreateChatGroupPacket();
	crCreateChatGroupPacket( const crCreateChatGroupPacket &p );

	virtual ~crCreateChatGroupPacket() {}
public:
	static const int ID = WCHNET_CREATECHATGROUP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateChatGroupPacket &packet,const std::string &groupName, const std::string &introduce);
	static void buildRequestPacket2(crCreateChatGroupPacket &packet,int playerid, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crCreateChatGroupPacket &packet,int recvid, int memberid, crNetChatData::crChatGroup *chatGroup);
	static void buildReplyPacket2(crCreateChatGroupPacket &packet, CRCore::crStreamBuf *stream);
};


}

#endif
