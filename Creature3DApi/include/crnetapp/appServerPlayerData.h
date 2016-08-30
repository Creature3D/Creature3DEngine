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
#ifndef CRNETAPP_SERVERPLAYERDATA_H
#define CRNETAPP_SERVERPLAYERDATA_H 1

#include <CRNetApp/appExport.h>
#include <CRNet/crNetContainer.h>
#include <CRNet\crNetDataManager.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetChatData.h>
#include <CRNetApp/appSightInfo.h>
#include <map>
#include <vector>
#include <deque>

namespace CRNetApp {

class CRNETAPP_EXPORT crParentServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crParentServerPlayerData();
	void setParentServerLineConductor(CRNet::crNetConductor *netConductor) { m_parentServerLine = netConductor; }
	CRNet::crNetConductor *getParentServerLineConductor() { return m_parentServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_parentServerLine;
};

class CRNETAPP_EXPORT crChatServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crChatServerPlayerData();
	void setChatDBID(unsigned short chatdbid){ m_chatdbid = chatdbid; }
	unsigned short getChatDBID(){ return m_chatdbid; }
	void setNickName(const std::string &nickName);
	const std::string &getNickName() const;

	void setNetChatData(crNetChatData *netChatData){ m_netChatData = netChatData; }
	crNetChatData *getNetChatData(){ return m_netChatData.get(); }

	void setParentServerLineConductor(CRNet::crNetConductor *netConductor) { m_parentServerLine = netConductor; }
	CRNet::crNetConductor *getParentServerLineConductor() { return m_parentServerLine.get(); }
protected:
	std::string m_nickName;
	unsigned short m_chatdbid;
	CRCore::ref_ptr<crNetChatData> m_netChatData;
	CRCore::ref_ptr<CRNet::crNetConductor> m_parentServerLine;
};

class CRNETAPP_EXPORT crVoiceServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crVoiceServerPlayerData();
	void setParentServerLineConductor(CRNet::crNetConductor *netConductor) { m_parentServerLine = netConductor; }
	CRNet::crNetConductor *getParentServerLineConductor() { return m_parentServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_parentServerLine;
};

class CRNETAPP_EXPORT crCameraServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crCameraServerPlayerData();
	void setParentServerLineConductor(CRNet::crNetConductor *netConductor) { m_parentServerLine = netConductor; }
	CRNet::crNetConductor *getParentServerLineConductor() { return m_parentServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_parentServerLine;
};

class CRNETAPP_EXPORT crChatConnectServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crChatConnectServerPlayerData();
	void setChatServerLineConductor(CRNet::crNetConductor *netConductor) { m_chatServerLine = netConductor; }
	CRNet::crNetConductor *getChatServerLineConductor() { return m_chatServerLine.get(); }
	void setVoiceServerLineConductor(CRNet::crNetConductor *netConductor) { m_voicedServerLine = netConductor; }
	CRNet::crNetConductor *getVoiceServerLineConductor() { return m_voicedServerLine.get(); }
	void setCameraServerLineConductor(CRNet::crNetConductor *netConductor) { m_cameraServerLine = netConductor; }
	CRNet::crNetConductor *getCameraServerLineConductor() { return m_cameraServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_chatServerLine;
	CRCore::ref_ptr<CRNet::crNetConductor> m_voicedServerLine;
	CRCore::ref_ptr<CRNet::crNetConductor> m_cameraServerLine;
};

class CRNETAPP_EXPORT crLoginConnectServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crLoginConnectServerPlayerData(){};
	void setLoginServerLineConductor(CRNet::crNetConductor *netConductor) { m_loginServerLine = netConductor; }
	CRNet::crNetConductor *getLoginServerLineConductor() { return m_loginServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_loginServerLine;
};

class CRNETAPP_EXPORT crDownloadConnectServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crDownloadConnectServerPlayerData();
	void setDownloadServerLineConductor(CRNet::crNetConductor *netConductor) { m_downloadServerLine = netConductor; }
	CRNet::crNetConductor *getDownloadServerLineConductor() { return m_downloadServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_downloadServerLine;
};

class CRNETAPP_EXPORT crGameServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crGameServerPlayerData();
	inline void setGameDBID(unsigned short gamedbid){ m_gamedbid = gamedbid; }
	inline unsigned short getGameDBID(){ return m_gamedbid; }

	inline void setSceneServerAddress(const std::string &address) { m_sceneServerAddress = address; }
	inline const std::string &getSceneServerAddress() { return m_sceneServerAddress; }
	inline void setParentServerLineConductor(CRNet::crNetConductor *netConductor) { m_parentServerLine = netConductor; }
	inline CRNet::crNetConductor *getParentServerLineConductor() { return m_parentServerLine.get(); }

	inline void setSceneID(int sceneid) { m_sceneid = sceneid; }
	inline int getSceneID() { return m_sceneid; }
	inline void setPlayerGameData(crPlayerGameData *data) { m_playerGameData = data; }
	inline crPlayerGameData* getPlayerGameData() { return m_playerGameData.get(); }
	inline bool isPlayerGameDataValid(){ return m_playerGameData.valid() && m_playerGameData->getDataClass(); }
	std::string getCharacterName();
	float& saveTimer(){ return m_saveTimer; }
	void setKey(const std::string &key,const std::string &username);
	bool checkKey(const std::string &key);
	const std::string &getUserName();
	virtual void clearData();
protected:
	unsigned short m_gamedbid;
	std::string m_sceneServerAddress;
	CRCore::ref_ptr<CRNet::crNetConductor> m_parentServerLine;

	int m_sceneid;
	CRCore::ref_ptr<crPlayerGameData> m_playerGameData;
	float m_saveTimer;
	std::string m_username;
	std::string m_key;
};

class CRNETAPP_EXPORT crGameConnectServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crGameConnectServerPlayerData();
	void setGameServerLineConductor(CRNet::crNetConductor *netConductor) { m_gameServerLine = netConductor; }
	CRNet::crNetConductor *getGameServerLineConductor() { return m_gameServerLine.get(); }
	void setDownloadServerLineConductor(CRNet::crNetConductor *netConductor) { m_downServerLine = netConductor; }
	CRNet::crNetConductor *getDownloadServerLineConductor() { return m_downServerLine.get(); }
	void setSceneServerLineConductor(CRNet::crNetConductor *netConductor) { m_sceneServerLine = netConductor; }
	CRNet::crNetConductor *getSceneServerLineConductor() { return m_sceneServerLine.get(); }
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_gameServerLine;
	CRCore::ref_ptr<CRNet::crNetConductor> m_downServerLine;
	CRCore::ref_ptr<CRNet::crNetConductor> m_sceneServerLine;
};
class CRNETAPP_EXPORT crSceneServerPlayerData :  public CRNet::crServerPlayerData
{
public:
	crSceneServerPlayerData();
	void setGameDBID(unsigned short gamedbid){ m_gamedbid = gamedbid; }
	unsigned short getGameDBID(){ return m_gamedbid; }

	void setGameServerLineConductor(CRNet::crNetConductor *netConductor) { m_gameServerLine = netConductor; }
	CRNet::crNetConductor *getGameServerLineConductor() { return m_gameServerLine.get(); }

	typedef std::map<int,CRCore::ref_ptr<crRole> >RoleMap;//一个玩家控制多个角色
	void addRole(crRole *role);
	crRole *getRole(int roleid);
	//void lockRoleMap();
	RoleMap &getRoleMap();
	//void unlockRoleMap();
	//void setRole(crRole *role);
	//crRole *getRole();
	void setClientRunning(bool bln);
	inline bool isClientRunning(){ return m_clientRunning; }
	//typedef std::map<_crInt64,RolePlayerPair> RolePlayerMap;//playerid,roleid,
	//typedef std::multimap< float,RolePlayerPair,std::less<float> >InRangeRolePlayerMap;
	//bool isRoleInRange(int playerid,int roleid);
	//void insertInRangeRole(crRole *role,crSceneServerPlayerData *player,float dist);
	//void roleInRangeUpdate();
	//void removeInRangePlayer(crSceneServerPlayerData *player);
	//void removeInRangeRole(crRole *role);
	//void sendPacketToNeighbor(CRNet::crStreamPacket &packet);

	//typedef std::map<int, CRCore::ref_ptr<crInstanceItem> > ItemMap;
	//typedef std::multimap<float, CRCore::ref_ptr<crInstanceItem>, std::less<float> > InRangeItemMap;
	//bool isItemInRange(int itemid);
	//void insertInRangeItemMap(crInstanceItem *item,float dist);
	//void itemInRangeUpdate();
	//void removeInRangeItem(crInstanceItem *item);
	//crInstanceItem *findInRangeItem(int itemid);
    
	//typedef std::set< CRCore::ref_ptr<crInstanceItem> > OutRangeItemSet;
	//void insertOutRangeItem(crInstanceItem *item);
	//void outRangeUpdate();
	//void clearInRanges();

	float& saveTimer(){ return m_saveTimer; }
	void setSceneID(int sceneid){ m_sceneid = sceneid; }
	int getSceneID(){ return m_sceneid; }
	void setRoomID(int roomid);
	int getRoomID();
	void setPlayerGameData(crPlayerGameData *data);
	crPlayerGameData *getPlayerGameData();
	std::string getCharacterName();
	void setSightInfo(crSightInfo *sightinfo);
	crSightInfo *getSightInfo();
	int getLayerid();
	void setDelayTime(short delayTime);
	short getDelayTime();
	virtual void clearData();
protected:
	CRCore::ref_ptr<CRNet::crNetConductor> m_gameServerLine;
	unsigned short m_gamedbid;
	
	RoleMap m_roleMap;
	//GNE::Mutex m_roleMapMutex;
	CRCore::ref_ptr<crPlayerGameData> m_playerGameData;

	//InRangeRolePlayerMap m_inRangeRolePlayerMap;
	//CRCore::crMutex m_inRangeRolePlayerMapMutex;
	//RolePlayerMap m_inrangeRolePlayer;
 //   CRCore::crMutex m_inrangeRolePlayerMutex;
	//InRangeItemMap m_inRangeItemMap;
	//CRCore::crMutex m_inRangeItemMapMutex;
	//ItemMap m_inrangeItem;
	//CRCore::crMutex m_inrangeItemMutex;
	//OutRangeItemSet m_outRangeItemSet;
	//CRCore::crMutex m_outRangeItemSetMutex;
	bool m_clientRunning;
	float m_saveTimer;

	int m_sceneid;
	int m_roomid;
	CRCore::ref_ptr<crSightInfo> m_sightInfo;
	short m_delayTime;
};
//class CRNETAPP_EXPORT crSceneConnectServerPlayerData :  public CRNet::crServerPlayerData
//{
//public:
//	crSceneConnectServerPlayerData();
//	void setSceneServerLineConductor(CRNet::crNetConductor *netConductor) { m_sceneServerLine = netConductor; }
//	CRNet::crNetConductor *getSceneServerLineConductor() { return m_sceneServerLine.get(); }
//protected:
//	CRCore::ref_ptr<CRNet::crNetConductor> m_sceneServerLine;
//};

}

#endif
