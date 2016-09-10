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
#ifndef CRNETAPP_NETPACKET2_H
#define CRNETAPP_NETPACKET2_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNet\crStreamPacket.h>
#include <CRNet\crNetContainer.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appClientPlayerData.h>
namespace CRDataBase
{
	class crUpdateSql;
}
namespace CRNetApp {

class CRNETAPP_EXPORT crQueryGameListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryGameListPacket> sptr;
	typedef GNE::WeakPtr<crQueryGameListPacket> wptr;
public :
	crQueryGameListPacket();
	crQueryGameListPacket( const crQueryGameListPacket &p );

	virtual ~crQueryGameListPacket() {}
public:
	static const int ID = WCHNET_QUERYGAMELIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryGameListPacket &packet,int rows,int offset);
	static void buildRequestPacket2(crQueryGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
	//static void buildReplyPacket(crQueryGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
	//static void buildReplyPacket2(crQueryGameListPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRecvGameListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRecvGameListPacket> sptr;
	typedef GNE::WeakPtr<crRecvGameListPacket> wptr;
public :
	crRecvGameListPacket();
	crRecvGameListPacket( const crRecvGameListPacket &p );

	virtual ~crRecvGameListPacket() {}
public:
	static const int ID = WCHNET_RECVGAMELIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRecvGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRecvGameListPacket &packet,CRCore::crStreamBuf *stream);
};
//class CRNETAPP_EXPORT crLoginDownloadPacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crLoginDownloadPacket> sptr;
//	typedef GNE::WeakPtr<crLoginDownloadPacket> wptr;
//public :
//	crLoginDownloadPacket();
//	crLoginDownloadPacket( const crLoginDownloadPacket &p );
//
//	virtual ~crLoginDownloadPacket() {}
//public:
//	static const int ID = WCHNET_LOGINDOWNLOAD;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crLoginDownloadPacket &packet);
//	static void buildRequestPacket(crLoginDownloadPacket &packet,char flg);
//	static void buildRequestPacket2(crLoginDownloadPacket &packet,const std::string &sender,int playerid);
//	static void buildRequestPacket2(crLoginDownloadPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket(crLoginDownloadPacket &packet,int playerid);
//	static void buildReplyPacket(crLoginDownloadPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket2(crLoginDownloadPacket &packet,CRCore::crStreamBuf *stream);
//	//static int s_success;
//};

class CRNETAPP_EXPORT crDownloadFilePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crDownloadFilePacket> sptr;
	typedef GNE::WeakPtr<crDownloadFilePacket> wptr;
public :
	crDownloadFilePacket();
	crDownloadFilePacket( const crDownloadFilePacket &p );

	virtual ~crDownloadFilePacket() {}
public:
	static const int ID = WCHNET_DOWNLOADFILE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crDownloadFilePacket &packet,const std::string &filename,unsigned short downloadFlg);
	static void buildRequestPacket2(crDownloadFilePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crDownloadFilePacket &packet,int playerid,const std::string &filename, int cacheid, int count, CRCore::Timer_t mtime, CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crDownloadFilePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRequestFileStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestFileStreamPacket> sptr;
	typedef GNE::WeakPtr<crRequestFileStreamPacket> wptr;
public :
	crRequestFileStreamPacket();
	crRequestFileStreamPacket( const crRequestFileStreamPacket &p );

	virtual ~crRequestFileStreamPacket() {}
public:
	static const int ID = WCHNET_REQUESTFILESTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestFileStreamPacket &packet,int cacheid, int pos);
	static void buildRequestPacket2(crRequestFileStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crFileStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crFileStreamPacket> sptr;
	typedef GNE::WeakPtr<crFileStreamPacket> wptr;
public :
	crFileStreamPacket();
	crFileStreamPacket( const crFileStreamPacket &p );

	virtual ~crFileStreamPacket() {}
public:
	static const int ID = WCHNET_FILESTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crFileStreamPacket &packet,int playerid,int cacheid, char *ptr, int remainsize);
	static void buildReplyPacket2(crFileStreamPacket &packet,CRCore::crStreamBuf *stream);
};

//class CRNETAPP_EXPORT crDownloadFileEndPacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crDownloadFileEndPacket> sptr;
//	typedef GNE::WeakPtr<crDownloadFileEndPacket> wptr;
//public :
//	crDownloadFileEndPacket();
//	crDownloadFileEndPacket( const crDownloadFileEndPacket &p );
//
//	virtual ~crDownloadFileEndPacket() {}
//public:
//	static const int ID = WCHNET_DOWNLOADFILEEND;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crDownloadFileEndPacket &packet,unsigned short downloadFlg);
//	static void buildRequestPacket2(crDownloadFileEndPacket &packet,int playerid,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket(crDownloadFileEndPacket &packet,CRCore::crStreamBuf *stream);
//};

////////crLoginGamePacket
class CRNETAPP_EXPORT crLoginGamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginGamePacket> sptr;
	typedef GNE::WeakPtr<crLoginGamePacket> wptr;
public :
	crLoginGamePacket();
	crLoginGamePacket( const crLoginGamePacket &p );

	virtual ~crLoginGamePacket() {}
public:
	static const int ID = WCHNET_LOGINGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginGamePacket &packet);
	static void buildRequestPacket2(crLoginGamePacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crLoginGamePacket &packet,const std::string &address,char loginCode,crGameServerPlayerData *playerData);
	static void buildReplyPacket2(crLoginGamePacket &packet,char loginCode,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQueryGameRoleListPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryGameRoleListPacket> sptr;
	typedef GNE::WeakPtr<crQueryGameRoleListPacket> wptr;
public :
	crQueryGameRoleListPacket();
	crQueryGameRoleListPacket( const crQueryGameRoleListPacket &p );

	virtual ~crQueryGameRoleListPacket() {}
public:
	static const int ID = WCHNET_QUERYGAMEROLELIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryGameRoleListPacket &packet);
	static void buildRequestPacket2(crQueryGameRoleListPacket &packet,int playerid);
	static void buildReplyPacket(crQueryGameRoleListPacket &packet,int playerid,int streamid,int count);
	static void buildReplyPacket2(crQueryGameRoleListPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crCreateRolePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreateRolePacket> sptr;
	typedef GNE::WeakPtr<crCreateRolePacket> wptr;
public :
	crCreateRolePacket();
	crCreateRolePacket( const crCreateRolePacket &p );

	virtual ~crCreateRolePacket() {}
public:
	static const int ID = WCHNET_CREATEROLE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateRolePacket &packet,int abstractitemid, int abstractmetierid, std::string &rolename);
	static void buildRequestPacket2(crCreateRolePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crCreateRolePacket &packet,int playerid,int streamid,int count);
	static void buildReplyPacket2(crCreateRolePacket &packet,CRCore::crStreamBuf *stream);
};

//class CRNETAPP_EXPORT crQuerySceneInfoPacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crQuerySceneInfoPacket> sptr;
//	typedef GNE::WeakPtr<crQuerySceneInfoPacket> wptr;
//public :
//	crQuerySceneInfoPacket();
//	crQuerySceneInfoPacket( const crQuerySceneInfoPacket &p );
//
//	virtual ~crQuerySceneInfoPacket() {}
//public:
//	static const int ID = WCHNET_QUERYSCENEINFO;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crQuerySceneInfoPacket &packet,int sceneid);
//	static void buildRequestPacket2(crQuerySceneInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket(crQuerySceneInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket2(crQuerySceneInfoPacket &packet,CRCore::crStreamBuf *stream);
//};

class CRNETAPP_EXPORT crSceneServerInitPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSceneServerInitPacket> sptr;
	typedef GNE::WeakPtr<crSceneServerInitPacket> wptr;
public :
	crSceneServerInitPacket();
	crSceneServerInitPacket( const crSceneServerInitPacket &p );

	virtual ~crSceneServerInitPacket() {}
public:
	static const int ID = WCHNET_SCENESERVERINIT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSceneServerInitPacket &packet,const std::string &sceneName,const std::string &serverName);
	static void buildReplyPacket(crSceneServerInitPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRegisterSceneServerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRegisterSceneServerPacket> sptr;
	typedef GNE::WeakPtr<crRegisterSceneServerPacket> wptr;
public :
	crRegisterSceneServerPacket();
	crRegisterSceneServerPacket( const crRegisterSceneServerPacket &p );

	virtual ~crRegisterSceneServerPacket() {}
public:
	static const int ID = WCHNET_REGISTERSCENESERVER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRegisterSceneServerPacket &packet,int sceneid,const std::string &serverName);
};
class CRNETAPP_EXPORT crSceneServerInitEndPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSceneServerInitEndPacket> sptr;
	typedef GNE::WeakPtr<crSceneServerInitEndPacket> wptr;
public :
	crSceneServerInitEndPacket();
	crSceneServerInitEndPacket( const crSceneServerInitEndPacket &p );

	virtual ~crSceneServerInitEndPacket() {}
public:
	static const int ID = WCHNET_SCENESERVERINITEND;
public:
	virtual void parsePacket(const std::string &sender);
};
//extern CRNETAPP_EXPORT void itemQuery(CRCore::ref_ptr<crInstanceItem>&item,CRDataBase::crDataBase *session_instance,CRDataBase::crDataBase *session_res);
//extern CRNETAPP_EXPORT void childItemQuery(crInstanceItem *parentItem,CRDataBase::crDataBase *session_instance,CRDataBase::crDataBase *session_res);
extern CRNETAPP_EXPORT void writeItem(crInstanceItem *item,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count,char streamType);
extern CRNETAPP_EXPORT void writeChildItem(crInstanceItem *parentItem,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count,char streamType);
extern CRNETAPP_EXPORT void writeAbstractItem(crAbstractItem *item,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count);
extern CRNETAPP_EXPORT void readItem(CRCore::ref_ptr<crInstanceItem>&item,CRCore::crStreamBuf *stream,bool loadData = false);
extern CRNETAPP_EXPORT void readChildItem(crInstanceItem *parentItem,CRCore::crStreamBuf *stream,bool loadData);
extern CRNETAPP_EXPORT void readAbstractItem(CRCore::ref_ptr<crAbstractItem>&item,CRCore::crStreamBuf *stream/*,bool loadData = true*/);

class CRNETAPP_EXPORT crQuerySceneItemPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQuerySceneItemPacket> sptr;
	typedef GNE::WeakPtr<crQuerySceneItemPacket> wptr;
public :
	crQuerySceneItemPacket();
	crQuerySceneItemPacket( const crQuerySceneItemPacket &p );

	virtual ~crQuerySceneItemPacket() {}
public:
	static const int ID = WCHNET_QUERYSCENEITEM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQuerySceneItemPacket &packet,int sceneid);
	static void buildReplyPacket(crQuerySceneItemPacket &packet,int streamid,int count);
};

////////crLoginScenePacket
class CRNETAPP_EXPORT crLoginScenePacket : public CRNet::crStreamPacket
{//20130225 改成先根据sceneid从gameserver获得sceneservername，然后根据sceneservername登录到sceneserver
public:
	typedef GNE::SmartPtr<crLoginScenePacket> sptr;
	typedef GNE::WeakPtr<crLoginScenePacket> wptr;
public :
	crLoginScenePacket();
	crLoginScenePacket( const crLoginScenePacket &p );

	virtual ~crLoginScenePacket() {}
public:
	static const int ID = WCHNET_LOGINSCENE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginScenePacket &packet,int sceneid,int msgid);
	static void buildRequestPacket2(crLoginScenePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket3(crLoginScenePacket &packet,const std::string &playerAddress,int playerid,/*const std::string &playerName,*/CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crLoginScenePacket &packet,const std::string &serverName,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crLoginScenePacket &packet,char loginCode,int playerid,int msgid);
	static void buildReplyPacket2(crLoginScenePacket &packet,char loginCode,int msgid);
};

class CRNETAPP_EXPORT crLoginScene2Packet : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginScene2Packet> sptr;
	typedef GNE::WeakPtr<crLoginScene2Packet> wptr;
public :
	crLoginScene2Packet();
	crLoginScene2Packet( const crLoginScene2Packet &p );

	virtual ~crLoginScene2Packet() {}
public:
	static const int ID = WCHNET_LOGINSCENE2;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crLoginScene2Packet &packet,const std::string &connectServerAddress,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crLoginScene2Packet &packet,char loginCode,unsigned short gamedbid,CRCore::crStreamBuf *stream,crPlayerGameData *playerGameData);
	static void buildReplyPacket2(crLoginScene2Packet &packet,int playerid, int streamid, int count);
};
class CRNETAPP_EXPORT crLoginScene3Packet : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crLoginScene3Packet> sptr;
	typedef GNE::WeakPtr<crLoginScene3Packet> wptr;
public :
	crLoginScene3Packet();
	crLoginScene3Packet( const crLoginScene3Packet &p );

	virtual ~crLoginScene3Packet() {}
public:
	static const int ID = WCHNET_LOGINSCENE3;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crLoginScene3Packet &packet,int playerid, int streamid, int count);
};
class CRNETAPP_EXPORT crQueryRoleInfoPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryRoleInfoPacket> sptr;
	typedef GNE::WeakPtr<crQueryRoleInfoPacket> wptr;
public :
	crQueryRoleInfoPacket();
	crQueryRoleInfoPacket( const crQueryRoleInfoPacket &p );

	virtual ~crQueryRoleInfoPacket() {}
public:
	static const int ID = WCHNET_QUERYROLEINFO;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryRoleInfoPacket &packet,crRole *role);
	static void buildRequestPacket(crQueryRoleInfoPacket &packet,int roleid);
	static void buildRequestPacket2(crQueryRoleInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQueryRoleInfoPacket &packet,int playerid,int streamid,int count);
	static void buildReplyPacket2(crQueryRoleInfoPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQueryRoleInfo2Packet : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryRoleInfo2Packet> sptr;
	typedef GNE::WeakPtr<crQueryRoleInfo2Packet> wptr;
public :
	crQueryRoleInfo2Packet();
	crQueryRoleInfo2Packet( const crQueryRoleInfo2Packet &p );

	virtual ~crQueryRoleInfo2Packet() {}
public:
	static const int ID = WCHNET_QUERYROLEINFO2;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryRoleInfo2Packet &packet,CRCore::crStreamBuf *stream);
	static void buildRequestPacket(crQueryRoleInfo2Packet &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQueryRoleInfo2Packet &packet,int playerid,int streamid,int count);
};

class CRNETAPP_EXPORT crRequestPlayerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestPlayerPacket> sptr;
	typedef GNE::WeakPtr<crRequestPlayerPacket> wptr;
public :
	crRequestPlayerPacket();
	crRequestPlayerPacket( const crRequestPlayerPacket &p );

	virtual ~crRequestPlayerPacket() {}
public:
	static const int ID = WCHNET_REQUESTPLAYER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestPlayerPacket &packet,int playerid,int roleid);
	static void buildRequestPacket2(crRequestPlayerPacket &packet,int playerid, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crRequestPlayerPacket &packet,int recvid,int streamid,int count);
	static void buildReplyPacket2(crRequestPlayerPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crItemInRangePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crItemInRangePacket> sptr;
	typedef GNE::WeakPtr<crItemInRangePacket> wptr;
public :
	crItemInRangePacket();
	crItemInRangePacket( const crItemInRangePacket &p );

	virtual ~crItemInRangePacket() {}
public:
	static const int ID = WCHNET_ITEMINRANGE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crItemInRangePacket &packet,int recvid,crInstanceItem *item,CRCore::crStreamBuf *itemStateStream);
	static void buildRequestPacket2(crItemInRangePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crItemOutRangePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crItemOutRangePacket> sptr;
	typedef GNE::WeakPtr<crItemOutRangePacket> wptr;
public :
	crItemOutRangePacket();
	crItemOutRangePacket( const crItemOutRangePacket &p );

	virtual ~crItemOutRangePacket() {}
public:
	static const int ID = WCHNET_ITEMOUTRANGE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crItemOutRangePacket &packet,int recvid,crInstanceItem *item);
	static void buildRequestPacket2(crItemOutRangePacket &packet,CRCore::crStreamBuf *stream);
};

//class CRNETAPP_EXPORT crNpcInRangePacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crNpcInRangePacket> sptr;
//	typedef GNE::WeakPtr<crNpcInRangePacket> wptr;
//public :
//	crNpcInRangePacket();
//	crNpcInRangePacket( const crNpcInRangePacket &p );
//
//	virtual ~crNpcInRangePacket() {}
//public:
//	static const int ID = WCHNET_NPCINRANGE;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crNpcInRangePacket &packet,int recvid,int itemid,crInstanceItem *item);
//	static void buildRequestPacket2(crNpcInRangePacket &packet,CRCore::crStreamBuf *stream);
//};
//class CRNETAPP_EXPORT crNpcOutRangePacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crNpcOutRangePacket> sptr;
//	typedef GNE::WeakPtr<crNpcOutRangePacket> wptr;
//public :
//	crNpcOutRangePacket();
//	crNpcOutRangePacket( const crNpcOutRangePacket &p );
//
//	virtual ~crNpcOutRangePacket() {}
//public:
//	static const int ID = WCHNET_NPCOUTRANGE;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crNpcOutRangePacket &packet,int recvid,int itemid);
//	static void buildRequestPacket2(crNpcOutRangePacket &packet,CRCore::crStreamBuf *stream);
//};
class CRNETAPP_EXPORT crRequestItemPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestItemPacket> sptr;
	typedef GNE::WeakPtr<crRequestItemPacket> wptr;
public :
	crRequestItemPacket();
	crRequestItemPacket( const crRequestItemPacket &p );

	virtual ~crRequestItemPacket() {}
public:
	static const int ID = WCHNET_REQUESTITEM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestItemPacket &packet,int itemid);
	static void buildRequestPacket2(crRequestItemPacket &packet,int playerid, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crRequestItemPacket &packet,int recvid,int streamid,int count);
	static void buildReplyPacket2(crRequestItemPacket &packet,CRCore::crStreamBuf *stream);
};
//class CRNETAPP_EXPORT crRequestTaskAwardInfoPacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crRequestTaskAwardInfoPacket> sptr;
//	typedef GNE::WeakPtr<crRequestTaskAwardInfoPacket> wptr;
//public :
//	crRequestTaskAwardInfoPacket();
//	crRequestTaskAwardInfoPacket( const crRequestTaskAwardInfoPacket &p );
//
//	virtual ~crRequestTaskAwardInfoPacket() {}
//public:
//	static const int ID = WCHNET_REQUESTTASKAWARDINFO;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crRequestTaskAwardInfoPacket &packet,int roleid,int taskid);
//	static void buildRequestPacket2(crRequestTaskAwardInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream);
//	static void buildRequestPacket3(crRequestTaskAwardInfoPacket &packet,int abstractid,int playerid,int roleid,int taskid);
//	static void buildReplyPacket(crRequestTaskAwardInfoPacket &packet,int playerid,int roleid,int taskid,crAbstractItem *aitem);
//	static void buildReplyPacket2(crRequestTaskAwardInfoPacket &packet,CRCore::crStreamBuf *stream);
//};

class CRNETAPP_EXPORT crPlayerEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerEventPacket> wptr;
public :
	crPlayerEventPacket();
	crPlayerEventPacket( const crPlayerEventPacket &p );

	virtual ~crPlayerEventPacket() {}
public:
	static const int ID = WCHNET_PLAYEREVENT;
public:
	virtual void parsePacket(const std::string &sender);
	//static void buildRequestPacket(crPlayerEventPacket &packet, int roleid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crPlayerEventPacket &packet, crInstanceItem *role, _crInt64 msg, CRCore::crStreamBuf *param);
	//static void buildRequestPacket(crPlayerEventPacket &packet, int recvid, int playerid, int roleid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crPlayerEventPacket &packet, int recvid, crInstanceItem *role, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	//static void buildReplyPacket(crPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerEventPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crItemEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crItemEventPacket> sptr;
	typedef GNE::WeakPtr<crItemEventPacket> wptr;
public :
	crItemEventPacket();
	crItemEventPacket( const crItemEventPacket &p );

	virtual ~crItemEventPacket() {}
public:
	static const int ID = WCHNET_ITEMEVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crItemEventPacket &packet, int recvid,crInstanceItem *item, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildReplyPacket(crItemEventPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crSetClientRunningPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSetClientRunningPacket> sptr;
	typedef GNE::WeakPtr<crSetClientRunningPacket> wptr;
public :
	crSetClientRunningPacket();
	crSetClientRunningPacket( const crSetClientRunningPacket &p );

	virtual ~crSetClientRunningPacket() {}
public:
	static const int ID = WCHNET_SETCLIENTRUNNING;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSetClientRunningPacket &packet,bool runflg);
	static void buildRequestPacket2(crSetClientRunningPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRemovePlayerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRemovePlayerPacket> sptr;
	typedef GNE::WeakPtr<crRemovePlayerPacket> wptr;
public :
	crRemovePlayerPacket();
	crRemovePlayerPacket( const crRemovePlayerPacket &p );

	virtual ~crRemovePlayerPacket() {}
public:
	static const int ID = WCHNET_REMOVEPLAYER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRemovePlayerPacket &packet,int recvid,int playerid);
	static void buildRequestPacket2(crRemovePlayerPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crSaveItemDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSaveItemDataPacket> sptr;
	typedef GNE::WeakPtr<crSaveItemDataPacket> wptr;
public :
	crSaveItemDataPacket();
	crSaveItemDataPacket( const crSaveItemDataPacket &p );

	virtual ~crSaveItemDataPacket() {}
public:
	static const int ID = WCHNET_SAVEITEMDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSaveItemDataPacket &packet,crInstanceItem *item,CRCore::crStreamBuf *itemstream,unsigned short gamedbid);
};
class CRNETAPP_EXPORT crSaveItemStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSaveItemStreamPacket> sptr;
	typedef GNE::WeakPtr<crSaveItemStreamPacket> wptr;
public :
	crSaveItemStreamPacket();
	crSaveItemStreamPacket( const crSaveItemStreamPacket &p );

	virtual ~crSaveItemStreamPacket() {}
public:
	static const int ID = WCHNET_SAVEITEMSTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSaveItemStreamPacket &packet,int itemid,CRCore::crStreamBuf *itemstream);
	static void buildRequestPacket2(crSaveItemStreamPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	//static void buildReplyPacket(crSaveItemStreamPacket &packet, int playerid,char code);
	//static void buildReplyPacket2(crSaveItemStreamPacket &packet, CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crSaveRoleDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSaveRoleDataPacket> sptr;
	typedef GNE::WeakPtr<crSaveRoleDataPacket> wptr;
public :
	crSaveRoleDataPacket();
	crSaveRoleDataPacket( const crSaveRoleDataPacket &p );

	virtual ~crSaveRoleDataPacket() {}
public:
	static const int ID = WCHNET_SAVEROLEDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSaveRoleDataPacket &packet,crRole *role,CRCore::crStreamBuf *metierstream,unsigned short gamedbid);
};

class CRNETAPP_EXPORT crCreateItemChildPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreateItemChildPacket> sptr;
	typedef GNE::WeakPtr<crCreateItemChildPacket> wptr;
public :
	crCreateItemChildPacket();
	crCreateItemChildPacket( const crCreateItemChildPacket &p );

	virtual ~crCreateItemChildPacket() {}
public:
	static const int ID = WCHNET_CREATEITEMCHILD;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateItemChildPacket &packet,crInstanceItem *item, int parentitemtid, int abstractitemid);
	//static void buildReplyPacket(crCreateItemChildPacket &packet,int playerid, int streamid, int count);
	//static void buildReplyPacket2(crCreateItemChildPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crPlayerServerEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerServerEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerServerEventPacket> wptr;
public :
	crPlayerServerEventPacket();
	crPlayerServerEventPacket( const crPlayerServerEventPacket &p );

	virtual ~crPlayerServerEventPacket() {}
public:
	static const int ID = WCHNET_PLAYERSERVEREVENT;
public:
	virtual void parsePacket(const std::string &sender);
	//static void buildRequestPacket(crPlayerServerEventPacket &packet,_crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crPlayerServerEventPacket &packet,_crInt64 msg, crInstanceItem *item, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerServerEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRequestItemRTDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestItemRTDataPacket> sptr;
	typedef GNE::WeakPtr<crRequestItemRTDataPacket> wptr;
public :
	crRequestItemRTDataPacket();
	crRequestItemRTDataPacket( const crRequestItemRTDataPacket &p );

	virtual ~crRequestItemRTDataPacket() {}
public:
	static const int ID = WCHNET_REQUESTITEMRTDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestItemRTDataPacket &packet,crInstanceItem *item);
	static void buildRequestPacket2(crRequestItemRTDataPacket &packet,int playerid, CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRequestItemChildPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestItemChildPacket> sptr;
	typedef GNE::WeakPtr<crRequestItemChildPacket> wptr;
public :
	crRequestItemChildPacket();
	crRequestItemChildPacket( const crRequestItemChildPacket &p );

	virtual ~crRequestItemChildPacket() {}
public:
	static const int ID = WCHNET_REQUESTITEMCHILD;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestItemChildPacket &packet,crInstanceItem *rootitem, int itemid);
	static void buildRequestPacket2(crRequestItemChildPacket &packet,int playerid, CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerServerMetierEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerServerMetierEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerServerMetierEventPacket> wptr;
public :
	crPlayerServerMetierEventPacket();
	crPlayerServerMetierEventPacket( const crPlayerServerMetierEventPacket &p );

	virtual ~crPlayerServerMetierEventPacket() {}
public:
	static const int ID = WCHNET_PLAYERSERVERMETIEREVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerServerMetierEventPacket &packet,int roleid,_crInt64 msg,CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerServerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crPlayerMetierEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerMetierEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerMetierEventPacket> wptr;
public :
	crPlayerMetierEventPacket();
	crPlayerMetierEventPacket( const crPlayerMetierEventPacket &p );

	virtual ~crPlayerMetierEventPacket() {}
public:
	static const int ID = WCHNET_PLAYERMETIEREVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerMetierEventPacket &packet,int roleid,_crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crPlayerMetierEventPacket &packet, int recvid, int playerid, int roleid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerMetierEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPlayerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerMetierEventPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQueryAbstractItemPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryAbstractItemPacket> sptr;
	typedef GNE::WeakPtr<crQueryAbstractItemPacket> wptr;
public :
	crQueryAbstractItemPacket();
	crQueryAbstractItemPacket( const crQueryAbstractItemPacket &p );

	virtual ~crQueryAbstractItemPacket() {}
public:
	static const int ID = WCHNET_QUERYABSTRACTITEM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryAbstractItemPacket &packet,int abstractid,crInstanceItem *item,int belongid);
	static void buildReplyPacket(crQueryAbstractItemPacket &packet,bool sucess,int streamid,int count);
};
class CRNETAPP_EXPORT crCreateTemporaryItemChildPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreateTemporaryItemChildPacket> sptr;
	typedef GNE::WeakPtr<crCreateTemporaryItemChildPacket> wptr;
public :
	crCreateTemporaryItemChildPacket();
	crCreateTemporaryItemChildPacket( const crCreateTemporaryItemChildPacket &p );

	virtual ~crCreateTemporaryItemChildPacket() {}
public:
	static const int ID = WCHNET_CREATETEMPORARYITEMCHILD;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateTemporaryItemChildPacket &packet,crInstanceItem *item, int parentitemtid, int abstractitemid);
	//static void buildReplyPacket(crCreateTemporaryItemChildPacket &packet,int playerid, int streamid, int count);
	//static void buildReplyPacket2(crCreateTemporaryItemChildPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRemoveItemChildPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRemoveItemChildPacket> sptr;
	typedef GNE::WeakPtr<crRemoveItemChildPacket> wptr;
public :
	crRemoveItemChildPacket();
	crRemoveItemChildPacket( const crRemoveItemChildPacket &p );

	virtual ~crRemoveItemChildPacket() {}
public:
	static const int ID = WCHNET_REMOVEITEMCHILD;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRemoveItemChildPacket &packet,crItemChild *itemchild,unsigned short gamedbid);
};

class CRNETAPP_EXPORT crSceneChatPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSceneChatPacket> sptr;
	typedef GNE::WeakPtr<crSceneChatPacket> wptr;
public :
	crSceneChatPacket();
	crSceneChatPacket( const crSceneChatPacket &p );

	virtual ~crSceneChatPacket() {}
public:
	static const int ID = WCHNET_SCENECHAT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSceneChatPacket &packet, const std::string &chatStr);
	static void buildRequestPacket2(crSceneChatPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crSceneChatPacket &packet,int recvid,const std::string &senderName,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crSceneChatPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crGameChatPacket : public CRNet::crStreamPacket
{//向所有玩家发的消息
public:
	typedef GNE::SmartPtr<crGameChatPacket> sptr;
	typedef GNE::WeakPtr<crGameChatPacket> wptr;
public :
	crGameChatPacket();
	crGameChatPacket( const crGameChatPacket &p );

	virtual ~crGameChatPacket() {}
public:
	static const int ID = WCHNET_GAMECHAT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crGameChatPacket &packet, const std::string &chatStr);
	static void buildRequestPacket2(crGameChatPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crGameChatPacket &packet,int recvid,const std::string &senderName,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crGameChatPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQuerySceneListPacket : public CRNet::crStreamPacket
{//对于开房间游戏，登陆gameserver后选择角色后，查询场景列表
public:
	typedef GNE::SmartPtr<crQuerySceneListPacket> sptr;
	typedef GNE::WeakPtr<crQuerySceneListPacket> wptr;
public :
	crQuerySceneListPacket();
	crQuerySceneListPacket( const crQuerySceneListPacket &p );

	virtual ~crQuerySceneListPacket() {}
public:
	static const int ID = WCHNET_QUERYSCENELIST;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQuerySceneListPacket &packet,int rows, unsigned char offset);
	static void buildRequestPacket2(crQuerySceneListPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQuerySceneListPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crQuerySceneListPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crQuerySceneRoomsPacket : public CRNet::crStreamPacket
{//登陆游戏场景后返回房间列表
public:
	typedef GNE::SmartPtr<crQuerySceneRoomsPacket> sptr;
	typedef GNE::WeakPtr<crQuerySceneRoomsPacket> wptr;
public :
	crQuerySceneRoomsPacket();
	crQuerySceneRoomsPacket( const crQuerySceneRoomsPacket &p );

	virtual ~crQuerySceneRoomsPacket() {}
public:
	static const int ID = WCHNET_QUERYSCENEROOMS;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQuerySceneRoomsPacket &packet);
	static void buildRequestPacket2(crQuerySceneRoomsPacket &packet,int playerid);
	static void buildReplyPacket(crQuerySceneRoomsPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crQuerySceneRoomsPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crCreateSceneRoomPacket : public CRNet::crStreamPacket
{//创建房间
public:
	typedef GNE::SmartPtr<crCreateSceneRoomPacket> sptr;
	typedef GNE::WeakPtr<crCreateSceneRoomPacket> wptr;
public :
	crCreateSceneRoomPacket();
	crCreateSceneRoomPacket( const crCreateSceneRoomPacket &p );

	virtual ~crCreateSceneRoomPacket() {}
public:
	static const int ID = WCHNET_CREATESCENEROOM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreateSceneRoomPacket &packet,CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crCreateSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crCreateSceneRoomPacket &packet,int playerid, crRoom *room);
	static void buildReplyPacket2(crCreateSceneRoomPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crPreEnterSceneRoomPacket : public CRNet::crStreamPacket
{//进入房间
public:
	typedef GNE::SmartPtr<crPreEnterSceneRoomPacket> sptr;
	typedef GNE::WeakPtr<crPreEnterSceneRoomPacket> wptr;
public :
	crPreEnterSceneRoomPacket();
	crPreEnterSceneRoomPacket( const crPreEnterSceneRoomPacket &p );

	virtual ~crPreEnterSceneRoomPacket() {}
public:
	static const int ID = WCHNET_PREENTERSCENEROOM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPreEnterSceneRoomPacket &packet,int roomid,const std::string &characterName/*,const std::string &password = ""*/);
	static void buildRequestPacket2(crPreEnterSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPreEnterSceneRoomPacket &packet,int playerid,bool success);
	static void buildReplyPacket2(crPreEnterSceneRoomPacket &packet,CRCore::crStreamBuf *stream);
};

//class CRNETAPP_EXPORT crRoomPlayerDataPacket : public CRNet::crStreamPacket
//{//SceneServer发给客户端
//public:
//	typedef GNE::SmartPtr<crRoomPlayerDataPacket> sptr;
//	typedef GNE::WeakPtr<crRoomPlayerDataPacket> wptr;
//public :
//	crRoomPlayerDataPacket();
//	crRoomPlayerDataPacket( const crRoomPlayerDataPacket &p );
//
//	virtual ~crRoomPlayerDataPacket() {}
//public:
//	static const int ID = WCHNET_ROOMPLAYERDATA;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildReplyPacket(crRoomPlayerDataPacket &packet,int playerid,int streamid,int count);
//	static void buildReplyPacket2(crRoomPlayerDataPacket &packet,CRCore::crStreamBuf *stream);
//};

//class CRNETAPP_EXPORT crLeaveSceneRoomPacket : public CRNet::crStreamPacket
//{//退出我当前所在的房间
//public:
//	typedef GNE::SmartPtr<crLeaveSceneRoomPacket> sptr;
//	typedef GNE::WeakPtr<crLeaveSceneRoomPacket> wptr;
//public :
//	crLeaveSceneRoomPacket();
//	crLeaveSceneRoomPacket( const crLeaveSceneRoomPacket &p );
//
//	virtual ~crLeaveSceneRoomPacket() {}
//public:
//	static const int ID = WCHNET_LEAVESCENEROOM;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crLeaveSceneRoomPacket &packet);
//	static void buildRequestPacket2(crLeaveSceneRoomPacket &packet,int playerid);
//	static void buildReplyPacket(crLeaveSceneRoomPacket &packet,int playerid,int leaver);
//	static void buildReplyPacket2(crLeaveSceneRoomPacket &packet,CRCore::crStreamBuf *stream);
//};

class CRNETAPP_EXPORT crStartRoomGamePacket : public CRNet::crStreamPacket
{//开始房间游戏，房主
public:
	typedef GNE::SmartPtr<crStartRoomGamePacket> sptr;
	typedef GNE::WeakPtr<crStartRoomGamePacket> wptr;
public :
	crStartRoomGamePacket();
	crStartRoomGamePacket( const crStartRoomGamePacket &p );

	virtual ~crStartRoomGamePacket() {}
public:
	static const int ID = WCHNET_STARTROOMGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crStartRoomGamePacket &packet);
	static void buildRequestPacket2(crStartRoomGamePacket &packet,int playerid);
	static void buildReplyPacket(crStartRoomGamePacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crStartRoomGamePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crEndRoomGamePacket : public CRNet::crStreamPacket
{//SceneServer发给客户端
public:
	typedef GNE::SmartPtr<crEndRoomGamePacket> sptr;
	typedef GNE::WeakPtr<crEndRoomGamePacket> wptr;
public :
	crEndRoomGamePacket();
	crEndRoomGamePacket( const crEndRoomGamePacket &p );

	virtual ~crEndRoomGamePacket() {}
public:
	static const int ID = WCHNET_ENDROOMGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crEndRoomGamePacket &packet,int playerid);
	static void buildReplyPacket2(crEndRoomGamePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crPauseRoomGamePacket : public CRNet::crStreamPacket
{//SceneServer发给客户端
public:
	typedef GNE::SmartPtr<crPauseRoomGamePacket> sptr;
	typedef GNE::WeakPtr<crPauseRoomGamePacket> wptr;
public :
	crPauseRoomGamePacket();
	crPauseRoomGamePacket( const crPauseRoomGamePacket &p );

	virtual ~crPauseRoomGamePacket() {}
public:
	static const int ID = WCHNET_PAUSEROOMGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crPauseRoomGamePacket &packet,int playerid,float pausetime);
	static void buildReplyPacket2(crPauseRoomGamePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRemoveRoomGamePacket : public CRNet::crStreamPacket
{//SceneServer发给客户端
public:
	typedef GNE::SmartPtr<crRemoveRoomGamePacket> sptr;
	typedef GNE::WeakPtr<crRemoveRoomGamePacket> wptr;
public :
	crRemoveRoomGamePacket();
	crRemoveRoomGamePacket( const crRemoveRoomGamePacket &p );

	virtual ~crRemoveRoomGamePacket() {}
public:
	static const int ID = WCHNET_REMOVEROOMGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRemoveRoomGamePacket &packet,int playerid);
	static void buildReplyPacket2(crRemoveRoomGamePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRoomGameTimePacket : public CRNet::crStreamPacket
{//SceneServer发给客户端
public:
	typedef GNE::SmartPtr<crRoomGameTimePacket> sptr;
	typedef GNE::WeakPtr<crRoomGameTimePacket> wptr;
public :
	crRoomGameTimePacket();
	crRoomGameTimePacket( const crRoomGameTimePacket &p );

	virtual ~crRoomGameTimePacket() {}
public:
	static const int ID = WCHNET_ROOMGAMETIME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRoomGameTimePacket &packet,int playerid,float time);
	static void buildReplyPacket2(crRoomGameTimePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRequestEmporiumPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRequestEmporiumPacket> sptr;
	typedef GNE::WeakPtr<crRequestEmporiumPacket> wptr;
public :
	crRequestEmporiumPacket();
	crRequestEmporiumPacket( const crRequestEmporiumPacket &p );

	virtual ~crRequestEmporiumPacket() {}
public:
	static const int ID = WCHNET_REQUESTEMPORIUM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRequestEmporiumPacket &packet);
	static void buildRequestPacket2(crRequestEmporiumPacket &packet,int playerid);
};

class CRNETAPP_EXPORT crRemoveRolePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRemoveRolePacket> sptr;
	typedef GNE::WeakPtr<crRemoveRolePacket> wptr;
public :
	crRemoveRolePacket();
	crRemoveRolePacket( const crRemoveRolePacket &p );

	virtual ~crRemoveRolePacket() {}
public:
	static const int ID = WCHNET_REMOVEROLE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRemoveRolePacket &packet,int roleid,std::string &code);
	static void buildRequestPacket2(crRemoveRolePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crRemoveRolePacket &packet,int playerid,int roleid,bool success);
	static void buildReplyPacket2(crRemoveRolePacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crEnterSceneRoomPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crEnterSceneRoomPacket> sptr;
	typedef GNE::WeakPtr<crEnterSceneRoomPacket> wptr;
public :
	crEnterSceneRoomPacket();
	crEnterSceneRoomPacket( const crEnterSceneRoomPacket &p );

	virtual ~crEnterSceneRoomPacket() {}
public:
	static const int ID = WCHNET_ENTERSCENEROOM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crEnterSceneRoomPacket &packet,int roomid);
	static void buildRequestPacket2(crEnterSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRoomPlayerEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomPlayerEventPacket> sptr;
	typedef GNE::WeakPtr<crRoomPlayerEventPacket> wptr;
public :
	crRoomPlayerEventPacket();
	crRoomPlayerEventPacket( const crRoomPlayerEventPacket &p );

	virtual ~crRoomPlayerEventPacket() {}
public:
	static const int ID = WCHNET_ROOMPLAYEREVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRoomPlayerEventPacket &packet,_crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crRoomPlayerEventPacket &packet, int recvid, int playerid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crRoomPlayerEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crRoomPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRoomPlayerEventPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRoomAddMemberPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomAddMemberPacket> sptr;
	typedef GNE::WeakPtr<crRoomAddMemberPacket> wptr;
public :
	crRoomAddMemberPacket();
	crRoomAddMemberPacket( const crRoomAddMemberPacket &p );

	virtual ~crRoomAddMemberPacket() {}
public:
	static const int ID = WCHNET_ROOMADDMEMBER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRoomAddMemberPacket &packet,int recvid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRoomAddMemberPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRoomRemoveMemberPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomRemoveMemberPacket> sptr;
	typedef GNE::WeakPtr<crRoomRemoveMemberPacket> wptr;
public :
	crRoomRemoveMemberPacket();
	crRoomRemoveMemberPacket( const crRoomRemoveMemberPacket &p );

	virtual ~crRoomRemoveMemberPacket() {}
public:
	static const int ID = WCHNET_ROOMREMOVEMEMBER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRoomRemoveMemberPacket &packet,int recvid,int playerid);
	static void buildReplyPacket2(crRoomRemoveMemberPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRoomSetPlayerGameDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomSetPlayerGameDataPacket> sptr;
	typedef GNE::WeakPtr<crRoomSetPlayerGameDataPacket> wptr;
public :
	crRoomSetPlayerGameDataPacket();
	crRoomSetPlayerGameDataPacket( const crRoomSetPlayerGameDataPacket &p );

	virtual ~crRoomSetPlayerGameDataPacket() {}
public:
	static const int ID = WCHNET_ROOMSETPLAYERGAMEDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRoomSetPlayerGameDataPacket &packet,int recvid,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRoomSetPlayerGameDataPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRoomPlayerGameDataUpdatePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomPlayerGameDataUpdatePacket> sptr;
	typedef GNE::WeakPtr<crRoomPlayerGameDataUpdatePacket> wptr;
public :
	crRoomPlayerGameDataUpdatePacket();
	crRoomPlayerGameDataUpdatePacket( const crRoomPlayerGameDataUpdatePacket &p );

	virtual ~crRoomPlayerGameDataUpdatePacket() {}
public:
	static const int ID = WCHNET_ROOMPLAYERGAMEDATAUPDATE;
public:
	virtual void parsePacket(const std::string &sender);
	//static void buildRequestPacket(crRoomPlayerGameDataUpdatePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crRoomPlayerGameDataUpdatePacket &packet,int recvid,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRoomPlayerGameDataUpdatePacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crRoomPlayerReadyPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomPlayerReadyPacket> sptr;
	typedef GNE::WeakPtr<crRoomPlayerReadyPacket> wptr;
public :
	crRoomPlayerReadyPacket();
	crRoomPlayerReadyPacket( const crRoomPlayerReadyPacket &p );

	virtual ~crRoomPlayerReadyPacket() {}
public:
	static const int ID = WCHNET_ROOMPLAYERREADY;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crRoomPlayerReadyPacket &packet,int recvid,int playerid,unsigned char groupid,bool ready);
	static void buildReplyPacket2(crRoomPlayerReadyPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crSavePlayerGameDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSavePlayerGameDataPacket> sptr;
	typedef GNE::WeakPtr<crSavePlayerGameDataPacket> wptr;
public :
	crSavePlayerGameDataPacket();
	crSavePlayerGameDataPacket( const crSavePlayerGameDataPacket &p );

	virtual ~crSavePlayerGameDataPacket() {}
public:
	static const int ID = WCHNET_SAVEPLAYERGAMEDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSavePlayerGameDataPacket &packet,int playerid, CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crSavePlayerGameDataPacket &packet, CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crCheckPlayerGameNamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCheckPlayerGameNamePacket> sptr;
	typedef GNE::WeakPtr<crCheckPlayerGameNamePacket> wptr;
public :
	crCheckPlayerGameNamePacket();
	crCheckPlayerGameNamePacket( const crCheckPlayerGameNamePacket &p );

	virtual ~crCheckPlayerGameNamePacket() {}
public:
	static const int ID = WCHNET_CHECKPLAYERGAMENAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCheckPlayerGameNamePacket &packet,std::string &name);
	static void buildRequestPacket2(crCheckPlayerGameNamePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crCheckPlayerGameNamePacket &packet,int recvid,char code);
	static void buildReplyPacket2(crCheckPlayerGameNamePacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crCreatePlayerGameDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCreatePlayerGameDataPacket> sptr;
	typedef GNE::WeakPtr<crCreatePlayerGameDataPacket> wptr;
public :
	crCreatePlayerGameDataPacket();
	crCreatePlayerGameDataPacket( const crCreatePlayerGameDataPacket &p );

	virtual ~crCreatePlayerGameDataPacket() {}
public:
	static const int ID = WCHNET_CREATEPLAYERGAMEDATA;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crCreatePlayerGameDataPacket &packet);
	static void buildRequestPacket2(crCreatePlayerGameDataPacket &packet,int playerid);
	static void buildReplyPacket(crCreatePlayerGameDataPacket &packet,int recvid,char code,int mainroleid = 0, int streamid = 0, int count = 0);
	static void buildReplyPacket2(crCreatePlayerGameDataPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataCreateStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataCreateStreamPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataCreateStreamPacket> wptr;
public :
	crPlayerDataCreateStreamPacket();
	crPlayerDataCreateStreamPacket( const crPlayerDataCreateStreamPacket &p );

	virtual ~crPlayerDataCreateStreamPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATACREATESTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerDataCreateStreamPacket &packet,CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crPlayerDataCreateStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataUpdateStreamPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataUpdateStreamPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataUpdateStreamPacket> wptr;
public :
	crPlayerDataUpdateStreamPacket();
	crPlayerDataUpdateStreamPacket( const crPlayerDataUpdateStreamPacket &p );

	virtual ~crPlayerDataUpdateStreamPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATAUPDATESTREAM;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crPlayerDataUpdateStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerDataUpdateStreamPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataEventPacket> wptr;
public :
	crPlayerDataEventPacket();
	crPlayerDataEventPacket( const crPlayerDataEventPacket &p );

	virtual ~crPlayerDataEventPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATAEVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerDataEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param);
	//static void buildRequestPacket(crPlayerDataEventPacket &packet, int playerid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerDataEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket3(crPlayerDataEventPacket &packet, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPlayerDataEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket_p(crPlayerDataEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerDataEventPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket3(crPlayerDataEventPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataEventZipPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataEventZipPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataEventZipPacket> wptr;
public :
	crPlayerDataEventZipPacket();
	crPlayerDataEventZipPacket( const crPlayerDataEventZipPacket &p );

	virtual ~crPlayerDataEventZipPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATAEVENTZIP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerDataEventZipPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerDataEventZipPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket3(crPlayerDataEventZipPacket &packet, CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPlayerDataEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket_p(crPlayerDataEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerDataEventZipPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket3(crPlayerDataEventZipPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataSceneEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataSceneEventPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataSceneEventPacket> wptr;
public :
	crPlayerDataSceneEventPacket();
	crPlayerDataSceneEventPacket( const crPlayerDataSceneEventPacket &p );

	crPlayerDataSceneEventPacket(int id);
	crPlayerDataSceneEventPacket( int id, const CRNet::crStreamPacket &p );

	virtual ~crPlayerDataSceneEventPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATASCENEEVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerDataSceneEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerDataSceneEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPlayerDataSceneEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerDataSceneEventPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crPlayerDataSceneEventZipPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crPlayerDataSceneEventZipPacket> sptr;
	typedef GNE::WeakPtr<crPlayerDataSceneEventZipPacket> wptr;
public :
	crPlayerDataSceneEventZipPacket();
	crPlayerDataSceneEventZipPacket( const crPlayerDataSceneEventZipPacket &p );

	crPlayerDataSceneEventZipPacket(int id);
	crPlayerDataSceneEventZipPacket( int id, const CRNet::crStreamPacket &p );

	virtual ~crPlayerDataSceneEventZipPacket() {}
public:
	static const int ID = WCHNET_PLAYERDATASCENEEVENTZIP;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crPlayerDataSceneEventZipPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crPlayerDataSceneEventZipPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crPlayerDataSceneEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crPlayerDataSceneEventZipPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crEyePointsPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crEyePointsPacket> sptr;
	typedef GNE::WeakPtr<crEyePointsPacket> wptr;
public :
	crEyePointsPacket();
	crEyePointsPacket( const crEyePointsPacket &p );

	virtual ~crEyePointsPacket() {}
public:
	static const int ID = WCHNET_EYEPOINTS;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crEyePointsPacket &packet, int recvid, std::vector<CRCore::crVector2i>&eyepoints );
	static void buildReplyPacket2(crEyePointsPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crCheckPlayerDelayTimePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crCheckPlayerDelayTimePacket> sptr;
	typedef GNE::WeakPtr<crCheckPlayerDelayTimePacket> wptr;
public :
	crCheckPlayerDelayTimePacket();
	crCheckPlayerDelayTimePacket( const crCheckPlayerDelayTimePacket &p );

	virtual ~crCheckPlayerDelayTimePacket() {}
public:
	static const int ID = WCHNET_CHECKPLAYERDELAYTIME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crCheckPlayerDelayTimePacket &packet, int recvid);
	static void buildReplyPacket2(crCheckPlayerDelayTimePacket &packet, CRCore::crStreamBuf *stream);
	static void buildRequestPacket2(crCheckPlayerDelayTimePacket &packet, int playerid,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crStopMyRolesPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crStopMyRolesPacket> sptr;
	typedef GNE::WeakPtr<crStopMyRolesPacket> wptr;
public :
	crStopMyRolesPacket();
	crStopMyRolesPacket( const crStopMyRolesPacket &p );

	virtual ~crStopMyRolesPacket() {}
public:
	static const int ID = WCHNET_STOPMYROLES;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crStopMyRolesPacket &packet);
	static void buildRequestPacket2(crStopMyRolesPacket &packet,int playerid);
};
class CRNETAPP_EXPORT crDBSavePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crDBSavePacket> sptr;
	typedef GNE::WeakPtr<crDBSavePacket> wptr;
public :
	crDBSavePacket();
	crDBSavePacket( const crDBSavePacket &p );

	virtual ~crDBSavePacket() {}
public:
	static const int ID = WCHNET_DBSAVE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crDBSavePacket &packet,CRDataBase::crUpdateSql *sql,char flg = 0);//flg = -1表示account,0表示GlobalDB
	static void buildRequestPacket(crDBSavePacket &packet,unsigned short enumid, CRDataBase::crUpdateSql *sql);//DBVec
	static void buildRequestPacket(crDBSavePacket &packet,unsigned short enumid, unsigned short dbid, CRDataBase::crUpdateSql *sql);//DBMapVec
};
/////////////////////////
class CRNETAPP_EXPORT crServerListUpdatePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crServerListUpdatePacket> sptr;
	typedef GNE::WeakPtr<crServerListUpdatePacket> wptr;
public :
	crServerListUpdatePacket();
	crServerListUpdatePacket( const crServerListUpdatePacket &p );

	virtual ~crServerListUpdatePacket() {}
public:
	static const int ID = WCHNET_SERVERLISTUPDATE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crServerListUpdatePacket &packet,int serverid,short remaincount,short onlinecount);
};
class CRNETAPP_EXPORT crServerListQueryPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crServerListQueryPacket> sptr;
	typedef GNE::WeakPtr<crServerListQueryPacket> wptr;
public :
	crServerListQueryPacket();
	crServerListQueryPacket( const crServerListQueryPacket &p );

	virtual ~crServerListQueryPacket() {}
public:
	static const int ID = WCHNET_SERVERLISTQUERY;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crServerListQueryPacket &packet,int gameid,int playerid);
	static void buildRequestPacket2(crServerListQueryPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crServerListQueryPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crServerListQueryPacket &packet,CRCore::crStreamBuf *stream);
};
////
class CRNETAPP_EXPORT crConnectServerInitPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crConnectServerInitPacket> sptr;
	typedef GNE::WeakPtr<crConnectServerInitPacket> wptr;
public :
	crConnectServerInitPacket();
	crConnectServerInitPacket( const crConnectServerInitPacket &p );

	virtual ~crConnectServerInitPacket() {}
public:
	static const int ID = WCHNET_CONNECTSERVERINIT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crConnectServerInitPacket &packet,const std::string &ip,const std::string &ip2,const std::string &ip3,unsigned short port,short remaincount);
	static void buildRequestPacket2(crConnectServerInitPacket &packet,int serverid,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crConnectServerUpdatePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crConnectServerUpdatePacket> sptr;
	typedef GNE::WeakPtr<crConnectServerUpdatePacket> wptr;
public :
	crConnectServerUpdatePacket();
	crConnectServerUpdatePacket( const crConnectServerUpdatePacket &p );

	virtual ~crConnectServerUpdatePacket() {}
public:
	static const int ID = WCHNET_CONNECTSERVERUPDATE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crConnectServerUpdatePacket &packet,unsigned short port,short remaincount);
	static void buildRequestPacket2(crConnectServerUpdatePacket &packet,int serverid,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crConnectServerQueryPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crConnectServerQueryPacket> sptr;
	typedef GNE::WeakPtr<crConnectServerQueryPacket> wptr;
public :
	crConnectServerQueryPacket();
	crConnectServerQueryPacket( const crConnectServerQueryPacket &p );

	virtual ~crConnectServerQueryPacket() {}
public:
	static const int ID = WCHNET_CONNECTSERVERQUERY;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crConnectServerQueryPacket &packet,int gameid);
	static void buildRequestPacket2(crConnectServerQueryPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crConnectServerQueryPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crConnectServerQueryPacket &packet,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRoomEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crRoomEventPacket> sptr;
	typedef GNE::WeakPtr<crRoomEventPacket> wptr;
public :
	crRoomEventPacket();
	crRoomEventPacket( const crRoomEventPacket &p );

	virtual ~crRoomEventPacket() {}
public:
	static const int ID = WCHNET_ROOMEVENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRoomEventPacket &packet,_crInt64 msg, CRCore::crStreamBuf *param = NULL);
	static void buildRequestPacket2(crRoomEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildRequestPacket(crRoomEventPacket &packet,int playerid,_crInt64 msg, CRCore::crStreamBuf *param = NULL);
	static void buildReplyPacket(crRoomEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crRoomEventPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crKickoffPlayerPacket : public CRNet::crStreamPacket
{//
public:
	typedef GNE::SmartPtr<crKickoffPlayerPacket> sptr;
	typedef GNE::WeakPtr<crKickoffPlayerPacket> wptr;
public :
	crKickoffPlayerPacket();
	crKickoffPlayerPacket( const crKickoffPlayerPacket &p );

	virtual ~crKickoffPlayerPacket() {}
public:
	static const int ID = WCHNET_KICKOFFPLAYER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crKickoffPlayerPacket &packet,int playerid);
};
class CRNETAPP_EXPORT crSetGamelogPathPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crSetGamelogPathPacket> sptr;
	typedef GNE::WeakPtr<crSetGamelogPathPacket> wptr;
public :
	crSetGamelogPathPacket();
	crSetGamelogPathPacket( const crSetGamelogPathPacket &p );

	virtual ~crSetGamelogPathPacket() {}
public:
	static const int ID = WCHNET_SETGAMELOGPATH;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crSetGamelogPathPacket &packet,const std::string &logpath);
};
class CRNETAPP_EXPORT crThrowServerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crThrowServerPacket> sptr;
	typedef GNE::WeakPtr<crThrowServerPacket> wptr;
public :
	crThrowServerPacket();
	crThrowServerPacket( const crThrowServerPacket &p );

	virtual ~crThrowServerPacket() {}
public:
	static const int ID = WCHNET_THROWSERVER;
public:
	virtual void parsePacket(const std::string &sender);
};
class CRNETAPP_EXPORT crServerNeedRestartPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crServerNeedRestartPacket> sptr;
	typedef GNE::WeakPtr<crServerNeedRestartPacket> wptr;
public :
	crServerNeedRestartPacket();
	crServerNeedRestartPacket( const crServerNeedRestartPacket &p );

	virtual ~crServerNeedRestartPacket() {}
public:
	static const int ID = WCHNET_SERVERNEEDRESTART;
public:
	virtual void parsePacket(const std::string &sender);
};
class CRNETAPP_EXPORT crClientReconnectPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crClientReconnectPacket> sptr;
	typedef GNE::WeakPtr<crClientReconnectPacket> wptr;
public :
	crClientReconnectPacket();
	crClientReconnectPacket( const crClientReconnectPacket &p );

	virtual ~crClientReconnectPacket() {}
public:
	static const int ID = WCHNET_CLIENTRECONNECT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crClientReconnectPacket &packet,int playerid);
};
class CRNETAPP_EXPORT crQueryGameInfoPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryGameInfoPacket> sptr;
	typedef GNE::WeakPtr<crQueryGameInfoPacket> wptr;
public :
	crQueryGameInfoPacket();
	crQueryGameInfoPacket( const crQueryGameInfoPacket &p );

	virtual ~crQueryGameInfoPacket() {}
public:
	static const int ID = WCHNET_QUERYGAMEINFO;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crQueryGameInfoPacket &packet,int gameid);
	static void buildRequestPacket2(crQueryGameInfoPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crQueryGameInfoPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crQueryGameInfoPacket &packet,CRCore::crStreamBuf *stream);
};
class CRNETAPP_EXPORT crShutDownServerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crShutDownServerPacket> sptr;
	typedef GNE::WeakPtr<crShutDownServerPacket> wptr;
public :
	crShutDownServerPacket();
	crShutDownServerPacket( const crShutDownServerPacket &p );

	virtual ~crShutDownServerPacket() {}
public:
	static const int ID = WCHNET_SHUTDOWNSERVER;
public:
	virtual void parsePacket(const std::string &sender);
};
}

#endif
