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
#ifndef CRNETAPP_NETCALLBACKS_H
#define CRNETAPP_NETCALLBACKS_H 1

#include <CRNetApp/appExport.h>
#include <CRNet/crNetCallbackManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CREncapsulation/crTableIO.h>
#include <queue>
#include <vector>
#include <set>
#include <string>

namespace CRNetApp {

////////////crLoginServerCallback
class CRNETAPP_EXPORT crLoginServerCallback :  public CRNet::crNetCallback
{
public:
	crLoginServerCallback();
	crLoginServerCallback(const crLoginServerCallback& callback);
	META_NetCallback(CRNetApp, LoginServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
protected:
	float m_pingInterval;
	float m_pingTime;
	int m_reconnectCount;//重连次数
};

////////////crLoginConnectServerCallback
class CRNETAPP_EXPORT crLoginConnectServerCallback :  public CRNet::crNetCallback
{
public:
	crLoginConnectServerCallback();
	crLoginConnectServerCallback(const crLoginConnectServerCallback& callback);
	META_NetCallback(CRNetApp, LoginConnectServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crChatServerCallback
class CRNETAPP_EXPORT crChatServerCallback :  public CRNet::crNetCallback
{
public:
	crChatServerCallback();
	crChatServerCallback(const crChatServerCallback& callback);
	META_NetCallback(CRNetApp, ChatServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);

	virtual void playerDroped(CRNet::crPlayerData *playerData);
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crChatConnectServerCallback
class CRNETAPP_EXPORT crChatConnectServerCallback :  public CRNet::crNetCallback
{
public:
	crChatConnectServerCallback();
	crChatConnectServerCallback(const crChatConnectServerCallback& callback);
	META_NetCallback(CRNetApp, ChatConnectServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crParentServerCallback
class CRNETAPP_EXPORT crParentServerCallback :  public CRNet::crNetCallback
{
public:
	crParentServerCallback();
	crParentServerCallback(const crParentServerCallback& callback);
	META_NetCallback(CRNetApp, ParentServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);

	virtual void playerDroped(CRNet::crPlayerData *_playerData);
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crDownloadServerCallback
class CRNETAPP_EXPORT crDownloadServerCallback :  public CRNet::crNetCallback
{
public:
	crDownloadServerCallback();
	crDownloadServerCallback(const crDownloadServerCallback& callback);
	META_NetCallback(CRNetApp, DownloadServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);

	virtual void playerDroped(CRNet::crPlayerData *_playerData);
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crDownloadConnectServerCallback
class CRNETAPP_EXPORT crDownloadConnectServerCallback :  public CRNet::crNetCallback
{
public:
	crDownloadConnectServerCallback();
	crDownloadConnectServerCallback(const crDownloadConnectServerCallback& callback);
	META_NetCallback(CRNetApp, DownloadConnectServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
protected:
	float m_pingInterval;
	float m_pingTime;
};

////////////crGameServerCallback
class CRNETAPP_EXPORT crGameServerCallback :  public CRNet::crNetCallback
{
public:
	crGameServerCallback();
	crGameServerCallback(const crGameServerCallback& callback);
	META_NetCallback(CRNetApp, GameServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);

	virtual void playerDroped(CRNet::crPlayerData *_playerData);
	int getGameID();
	const std::string &getGameName() const;

	bool checkServer(int serverid);
	void savePlayerData(crGameServerPlayerData *playerData);
	inline float getSaveInterval(){ return m_saveInterval; }
	typedef std::map<int,std::string> CharacterNameMap;
	bool insertCharacterName(int playerid,const std::string &name);
	bool findCharacterName(int playerid, std::string &name);
	void removeCharacterName(int playerid);
	inline crScene *getMainScene(){ return m_mainScene.get(); }
	typedef std::map<int,std::pair<std::string,std::string> > SceneServerMap;//sceneid,sceneservername,sceneserveraddress
	typedef std::map<std::string,std::string> SceneServerNameMap;//sceneservername,sceneserveraddress
	void registerSceneServer(int sceneid,const std::string &name,const std::string &address);
	std::string findSceneServerName(int sceneid);
	std::string findSceneServerAddress(int sceneid);
	void sendPacketToSceneServer(const std::string &sceneservername,GNE::Packet &packet);//"all"表示发送给所有sceneserver
	std::string getSceneServerAddressBySceneServerName(const std::string &sceneservername);
	typedef std::map<CRCore::ref_ptr<crGameServerPlayerData>,float> DelayRemovePlayerMap;
	typedef std::map<CRCore::ref_ptr<crGameServerPlayerData>,float> DropedPlayerMap;
	void insertDelayRemovePlayer(crGameServerPlayerData *player);
	void removeDropedPlayer(crGameServerPlayerData *player);
	int remuseDropedPlayer(int playerid,const std::string &key,CRCore::ref_ptr<crGameServerPlayerData>&outplayer);//0:可以登录,1可以重连,-1存储保护时间内
	//bool foundDelayRemovePlayer(int playerid);
	//bool foundDropedPlayer(int playerid, CRCore::ref_ptr<crGameServerPlayerData>&outplayer);
	//////区服务器信息
	void setServerID(int serverid);
	int getServerID();
	int getWebServerID();
	const std::string &getServerName() const;
	const std::string &getSite() const;
	unsigned char getISP();
	short getMaxPlayerCount();
	void setMaxPlayerCount(short count);
	short getRemainPlayerCount();
	void shutDown();
protected:
	void insertDropedPlayer(crGameServerPlayerData *player);
	void updateDropedPlayerMap(float dt);
	float m_pingInterval;
	float m_pingTime;
	float m_dbpingTime;
	float m_saveInterval;
	float m_shutdownTime;
	int m_gameid;
	std::string m_gameName;
	//GNE::Mutex m_saveMutex;
	CharacterNameMap m_characterNameMap;
	GNE::Mutex m_characterNameMapMutex;
	typedef std::vector< CRCore::ref_ptr<crGameServerPlayerData> >PlayerVec;
	PlayerVec m_savePlayerVec;
	PlayerVec m_updPlayerVec;
	int m_savePlayerNumPerFrame;
	CRCore::ref_ptr<crScene> m_mainScene;//
	SceneServerMap m_sceneServerMap;
	SceneServerNameMap m_sceneServerNameMap;
	GNE::Mutex m_sceneServerMapMutex;
	DelayRemovePlayerMap m_delayRemovePlayerMap;//sceneserver存储延迟
	GNE::Mutex m_delayRemovePlayerMapMutex;
	DropedPlayerMap m_dropedPlayerMap;//掉线玩家缓存
	GNE::Mutex m_dropedPlayerMapMutex;
	//区服务器信息
	int m_serverid;
	int m_webserverid;//平台serverid
	std::string m_serverName;
	std::string m_site;//联运平台标识
	unsigned char m_isp;
	short m_maxplayercount;
	float m_updateInterval;
	float m_updateTime;
	_crInt32 m_lastcount;
	int m_dropedServerCount;//
	//float m_dropedTimer;//服务器重启倒计时
	bool m_dropedSave;//
	int m_reconnectCount;//重连次数
	int m_maxDropServerCount;//
};

////////////crGameConnectServerCallback
class CRNETAPP_EXPORT crGameConnectServerCallback :  public CRNet::crNetCallback
{
public:
	crGameConnectServerCallback();
	crGameConnectServerCallback(const crGameConnectServerCallback& callback);
	META_NetCallback(CRNetApp, GameConnectServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	typedef std::map<CRCore::ref_ptr<crGameConnectServerPlayerData>,float> DropedPlayerMap;
	virtual void removeConnection(CRNet::crPlayerData *playerData);
	void removeDropedPlayer(crGameConnectServerPlayerData *player);
	void remuseDropedPlayer(int playerid,CRCore::ref_ptr<crGameConnectServerPlayerData>&outplayer);
protected:
	void insertDropedPlayer(crGameConnectServerPlayerData *player);
	void updateDropedPlayerMap(float dt);
	DropedPlayerMap m_dropedPlayerMap;//掉线玩家缓存
	GNE::Mutex m_dropedPlayerMapMutex;
	float m_pingInterval;
	float m_pingTime;
	float m_updateInterval;
	float m_updateTime;
	short m_lastcount;
	std::string m_ip;
	std::string m_ip2;
	std::string m_ip3;
	unsigned short m_proxyport;
	bool m_firstUpdated;
	int m_inreconnect;
};
////////////crSceneServerCallback
class CRNETAPP_EXPORT crSceneServerCallback :  public CRNet::crNetCallback
{
public:
	crSceneServerCallback();
	crSceneServerCallback(const crSceneServerCallback& callback);
	META_NetCallback(CRNetApp, SceneServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);

	virtual void playerDroped(CRNet::crPlayerData *_playerData);
	enum SaveFlg
	{
		SaveItem = 0x1,
		SaveRole = 0x2,
		SaveAll = SaveItem|SaveRole
	};
	void savePlayerData(crSceneServerPlayerData *playerData,SaveFlg flg=SaveAll);

	typedef std::map< int, CRCore::ref_ptr<crScene> >SceneMap;
	void addScene(crScene *scene);
	void registerSceneServer(CRNet::crNetConductor *sceneclient_game);
	crScene *findScene(int sceneid);
	bool checkScene(int sceneid);
	void clearSceneMap();
	void initSuccess();
	void initend();
	//void sendPacketToItemNeighbor(crInstanceItem *item,CRNet::crStreamPacket &packet);
	//void sendPacketToItemNeighborByID(int itemid, int sceneid, int roomid, CRNet::crStreamPacket &packet);

	///开房间式
	int getMaxRoomCount();
	int getRoomCount();
	crRoom* createRoom(crSceneServerPlayerData *creator,CRCore::crStreamBuf* streamBuf);//return roomid
	crRoom* createRoom(int sceneid,CRCore::crStreamBuf* streamBuf);
	void lockRoomMap();
	RoomMap &getRoomMap();
	void unlockRoomMap();
	crRoom *findRoom(int roomid);
	void roomUpdate(float dt);
	void startRoomGame(crRoom *room);
	void closeAllRoom();
	void shutDown();
	//void removeRoom(int roomid);
	//typedef std::deque<int> RecycleIDDeque;
	//int gainTemporaryRoomID();
	//void recycleRoomID(int id);
	crInstanceItem *dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos);//返回碰撞的item
	inline float getSaveInterval(){ return m_saveInterval; }
	inline void setGameName(const std::string &gamename){ m_gameName = gamename; }
	inline void setServerName(const std::string &servername){ m_serverName = servername; }
	inline const std::string &getGameName() const{ return m_gameName; }
	inline const std::string &getServerName() const{ return m_serverName; }
	typedef std::map<CRCore::ref_ptr<crSceneServerPlayerData>,float> DropedPlayerMap;
	typedef std::map<CRCore::ref_ptr<crRoom>,float> RemovedRoomMap;
protected:
	void insertDropedPlayer(crSceneServerPlayerData *player);
	void updateDropedPlayerMap(float dt);
	void insertRemovedRoom(crRoom *room);
	void updateRemovedRoomMap(float dt);
	void endRoomGame(crRoom *room);
	float m_pingInterval;
	float m_pingTime;
	float m_saveInterval;
	float m_reliveInterval;
	float m_reliveTime;
	float m_shutdownTime;
	bool  m_saveRole;

	std::string m_gameName;
	std::string m_serverName;
	SceneMap m_sceneMap;
	//GNE::Mutex m_sceneMapMutex;
	int m_initscenecount;
	bool m_initend;
	//GNE::Mutex m_saveMutex;

	///开房间式
	int m_maxRoomCount;
	RoomMap m_roomMap;
	GNE::Mutex m_roomMapMutex;

	std::string m_emporiumScript;
	typedef std::vector< CRCore::ref_ptr<crSceneServerPlayerData> >PlayerVec;
	PlayerVec m_savePlayerVec;
	int m_dropedServerCount;//
	//float m_dropedTimer;//服务器重启倒计时
	int m_maxDropServerCount;//
	DropedPlayerMap m_dropedPlayerMap;//掉线玩家延迟清除
	GNE::Mutex m_dropedPlayerMapMutex;
	RemovedRoomMap m_removedRoomMap;//房间延迟清除
	GNE::Mutex m_removedRoomMapMutex;
	//int m_reconnectCount;//重连次数
	//static RecycleIDDeque s_recycleRoomIDDeque;
	//static GNE::Mutex s_recycleRoomIDDequeMutex;
	//static int s_temporaryRoomID;
};
////////////crDBServerCallback
class CRNETAPP_EXPORT crDBServerCallback :  public CRNet::crNetCallback
{
public:
	crDBServerCallback();
	crDBServerCallback(const crDBServerCallback& callback);
	META_NetCallback(CRNetApp, DBServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
	virtual bool clientReconnectToServer();
	virtual void serverRemoveConnectionImplementation(const std::string& address);
	void saveGameLog(const std::string& address,int playerid,unsigned short logtype,const std::string &logdata);
	void saveDBLog(char *logdata);
	typedef std::map< std::string,std::pair<std::string,CRCore::ref_ptr<CREncapsulation::crTableIO> > > GamelogPathMap;//address,logpath
	void insertGamelogPath(const std::string &address,const std::string &logpath);
	void eraseGamelogPath(const std::string &address);
	void getGamelogPath(const std::string &address,std::string &outpath,CRCore::ref_ptr<CREncapsulation::crTableIO>&outtabio );
	void shutDown();
protected:
	float m_pingInterval;
	float m_pingTime;
	float m_shutdownTime;
	//bool m_init;
	int m_columncount;
	int	m_cacheCount;
	//std::string m_filename;
	//CREncapsulation::crTableIO::DataVec m_logInfoData;
	GNE::Mutex m_gamelogPathMapMutex;
	GamelogPathMap m_gamelogPathMap;
	//GNE::Mutex m_saveMutex;
};

//////////////crSceneConnectServerCallback
//class CRNETAPP_EXPORT crSceneConnectServerCallback :  public CRNet::crNetCallback
//{
//public:
//	crSceneConnectServerCallback();
//	crSceneConnectServerCallback(const crSceneConnectServerCallback& callback);
//	META_NetCallback(CRNetApp, SceneConnectServer)
//
//	virtual void serverInitImplementation();
//	virtual void clientInitImplementation();
//	virtual void serverUpdateImplementation();
//	virtual void clientUpdateImplementation();
//	virtual void serverShutdown();
//	virtual void clientShutdown();
//protected:
//	CRCore::Timer_t m_time;
//	float m_pingInterval;
//	float m_pingTime;
//};

}

#endif
