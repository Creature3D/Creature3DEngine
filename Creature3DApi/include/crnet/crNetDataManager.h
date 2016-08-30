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
#ifndef CRNET_CRNETDATAMANAGER_H
#define CRNET_CRNETDATAMANAGER_H

#include <CRNet/crStreamPacket.h>
#include <CRNet\crExport.h>
#include <CRNet/crNetCallbackManager.h>
#include <CRNet/crNetManager.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <gnelib.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <string>
#include <map>

namespace CRNet
{
//class CRNET_EXPORT crClientData : public CRCore::Referenced
//{
//friend class crNetDataManager;
//public:
//    crClientData():m_stamp(0.0f){}
//	inline virtual bool culled(const crClientData *cd, unsigned int cullmask, float sqrMaxDistance){ return false; }
//	inline void setStamp(float stamp) { m_stamp = stamp; }
//	inline float getStamp() { return m_stamp; }
//protected:
//	float m_stamp;
//};
//
//class CRNET_EXPORT crConnectServerClientData : public crClientData
//{
//public:
//	crConnectServerClientData():m_sceneid(0),m_roomid(0){}
//	inline void setSceneID(unsigned short sceneid) { m_sceneid = sceneid; }
//	inline unsigned short getSceneID() { return m_sceneid; }
//	inline void setRoomID(unsigned short roomid) { m_roomid = roomid; }
//	inline unsigned short getRoomID() { return m_roomid; }
//
//	enum CullMask
//	{
//		NO_CULLING = 0x0,
//		SCENE_CULLING = 0x1,
//		DISTANCE_CULLING = 0x2,
//		ROOM_CULLING = 0x4,
//		DEFAULT_CULLING = SCENE_CULLING |
//		DISTANCE_CULLING
//	};
//	inline virtual bool culled(const crClientData *cd, unsigned int cullmask, float sqrMaxDistance)
//	{
//		const crConnectServerClientData *cscd = dynamic_cast<const crConnectServerClientData *>(cd);
//		if(cscd)
//		{
//			if(cullmask&SCENE_CULLING)
//			{
//				if(m_sceneid != cscd->m_sceneid)
//					return true;
//			}
//			if(cullmask&ROOM_CULLING)
//			{
//				if(m_roomid != cscd->m_roomid)
//					return true;
//			}
//			if(cullmask&DISTANCE_CULLING)
//			{
//				if((cscd->m_position - m_position).length2()>sqrMaxDistance)
//					return true;
//			}
//		}
//		return false;
//	}
//protected:
//	unsigned short m_sceneid;
//	unsigned short m_roomid;
//	CRCore::crVector3f m_position;
//	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
class crNetConductor;
class CRNET_EXPORT crPlayerData : public CRCore::Referenced
{
	friend class crNetDataManager;
public:
	crPlayerData():m_playerID(0){}
	inline void setPlayerID(int id) { m_playerID = id; }
	inline int getPlayerID() { return m_playerID; }

	//inline void setPlayerName(const std::string &playerName) { m_playerName = playerName; }
	//inline const std::string &getPlayerName() const { return m_playerName; }
protected:
	int m_playerID;//玩家ID，用于玩家索引，玩家成功登陆服务器后将被设置
	//std::string m_playerName;
};

class CRNET_EXPORT crServerPlayerData : public crPlayerData
{
public:
	crServerPlayerData()/*:m_gamedbid(0),m_chatdbid(0)*/{}
	//void setGameDBID(unsigned short gamedbid){ m_gamedbid = gamedbid; }
	//unsigned short getGameDBID(){ return m_gamedbid; }
	//void setChatDBID(unsigned short chatdbid){ m_chatdbid = chatdbid; }
	//unsigned short getChatDBID(){ return m_chatdbid; }

	inline void setPlayerAdress(const std::string &adress) { m_playerAddress = adress; }
	inline const std::string &getPlayerAdress() const { return m_playerAddress; }
	
	inline void setPlayerConnectServerAddress(const std::string &adress) { m_connectServerAddress = adress; }
	inline const std::string &getPlayerConnectServerAddress() const { return m_connectServerAddress; }
protected:
	//unsigned short m_gamedbid;//玩家成功登陆服务器后将被设置
	//unsigned short m_chatdbid;//玩家成功登陆服务器后将被设置
	std::string m_playerAddress;//玩家IP
	std::string m_connectServerAddress;//玩家连接服务器IP
};

class CRNET_EXPORT crNetDataManager :  public CRCore::Referenced
{
public:
    crNetDataManager();

	void serverUpdate(float dt);
	void clientUpdate(float dt);

	void serverShutdown();
	void clientShutdown();

	 //各服务器上保存的与其连接的客户端名与客户端地址的映射表
	//typedef std::map< std::string, std::string > NameClientMap;//GNE::Address::toString()
	//inline bool findNameClient(const std::string &name){ return m_nameClientMap.find(name) != m_nameClientMap.end(); }
	//inline void getNameClientAddress(const std::string &name,std::string &address) { NameClientMap::iterator itr = m_nameClientMap.find(name); if(itr != m_nameClientMap.end()) address = itr->second; }
	//inline void insertNameClient(const std::string &name, const std::string& address){ m_nameClientMap[name] = address; }
	//inline void removeNameClient(const std::string &name){ m_nameClientMap.erase(name); }

	//inline bool findClientName(const std::string &address)
	//{
	//	for( NameClientMap::iterator itr = m_nameClientMap.begin();
	//		 itr != m_nameClientMap.end();
	//		 ++itr )
	//	{
	//		if(itr->second.compare(address) == 0)
	//			return true;
	//	}
	//	return false; 
	//}
	//inline void getClientName(const std::string &address, std::string &name) 
	//{ 	
	//	for( NameClientMap::iterator itr = m_nameClientMap.begin();
 //   	     itr != m_nameClientMap.end();
	//         ++itr )
	//    {
	//	    if(itr->second.compare(address) == 0)
	//		{
	//			name = itr->first;
	//			break;
	//		}
 //   	} 
	//}
	//inline void removeClientName(const std::string &address)
	//{ 
	//	for( NameClientMap::iterator itr = m_nameClientMap.begin();
	//		itr != m_nameClientMap.end();
	//		++itr )
	//	{
	//		if(itr->second.compare(address) == 0)
	//		{
	//			m_nameClientMap.erase(itr);
	//			break;
	//		}
	//	}
	//}

	//void setMaxDistance(float maxDistance);
	//inline float getSqrMaxDistance(){return m_sqr_maxDistance;}

    //各服务器上保存的与其连接的客户端地址与crClientData的映射表
	//typedef std::map< std::string, CRCore::ref_ptr<crClientData> > ClientDataMap;
	//inline void insertClientData(const std::string &address, crClientData *data){ m_clientDataMap[address] = data; }
	//inline void removeClientData(const std::string &address){ m_clientDataMap.erase(address); }
	//inline crClientData *getClientData(const std::string &address){ ClientDataMap::iterator itr = m_clientDataMap.find(address); return itr != m_clientDataMap.end()? itr->second.get():NULL;}
	//inline ClientDataMap &getClientDataMap(){ return m_clientDataMap; }

	//void broadcastExceptSenderAndCulled(const std::string &sender, GNE::Packet &packet,const crClientData &data, unsigned int cullmask);
	//void broadcastExceptCulled(GNE::Packet &packet,const crClientData &data, unsigned int cullmask);

	void setNetCallback(crNetCallback* dc) { m_netCallback=dc; if(m_netCallback.valid()) m_netCallback->init(m_netManager->getNetConductor()); }
	crNetCallback* getNetCallback() { return m_netCallback.get(); }

	void setServerName(const std::string &name);
	std::string &getServerName();
	void setServerPassword(const std::string &password);
	std::string &getServerPassword();
	void setNetCallbackString(const std::string &callback);
	//void setMaxConnectCount(int maxConnectCount);
	//int getMaxConnectCount();
	//bool checkConnectCount();
	typedef std::pair<crStreamPacket *,std::string> StreamPacketSender;
	typedef std::list<StreamPacketSender> ReceivedStreamList;
	void receiveStream(crStreamPacket *packet,const std::string &sender);

	void clientUpdateAndReconnect(float interval, float delay);
	//void serverUpdateAndDrop(float interval, float delay);
	//void setClientStamp(float stamp);

	//inline int getConnectCount() const{ return m_nameClientMap.size(); }
	//void removeConnection(const std::string &name,const std::string &address);

	//LoginServer/ChatServer/MainServer/SceneServerX上保存clientName-ConnectServerClientAddres映射表
	//以便于各服务器可以快速定位到玩家所在的接入服务器
	//typedef std::map<std::string, std::string> ClientNameConnectServerAddressMap;
	//inline void insertClientNameConnectServerAddress(const std::string &clientName, const std::string &connectServerAddress)
	//{ m_clientNameConnectServerAddressMap.insert(std::make_pair(clientName,connectServerAddress)); }
	//inline void removeClientNameConnectServerAddress(const std::string &clientName)
	//{ m_clientNameConnectServerAddressMap.erase(clientName); }
	//inline void getClientNameConnectServerAddress(const std::string &clientName, std::string &connectServerAddress)
	//{ ClientNameConnectServerAddressMap::iterator itr = m_clientNameConnectServerAddressMap.find(clientName); if(itr != m_clientNameConnectServerAddressMap.end()) connectServerAddress = itr->second;}
	//inline ClientNameConnectServerAddressMap &getClientNameConnectServerAddressMap(){ return m_clientNameConnectServerAddressMap; }
    typedef std::map< int, CRCore::ref_ptr<crPlayerData> > PlayerDataMap;//playerid,crPlayerData
	inline void lockPlayerDataMap()
	{
		m_playerMapMutex.acquire();
	}
	inline void unLockPlayerDataMap()
	{
		m_playerMapMutex.release();
	}
	inline void insertPlayerData(crPlayerData *data)
	{
		GNE::LockMutex lock( m_playerMapMutex );
		m_playerDataMap[data->getPlayerID()] = data;
	}
	inline void removePlayerData(int playerid)
	{ 
		CRCore::ref_ptr<crPlayerData> playerdata;
		m_playerMapMutex.acquire();
		PlayerDataMap::iterator itr = m_playerDataMap.find(playerid); 
		if(itr != m_playerDataMap.end())
		{
			playerdata = itr->second;
			m_playerDataMap.erase(itr);
		}
		m_playerMapMutex.release();
		if(playerdata.valid() && m_netCallback.valid())
			m_netCallback->playerDroped(playerdata.get());
	}
	//void removePlayerData(const std::string &name);
	inline crPlayerData *getPlayerData(int playerid)//使用的时候必需用ref_ptr保证安全性
	{
		GNE::LockMutex lock( m_playerMapMutex );
		PlayerDataMap::iterator itr = m_playerDataMap.find(playerid); return itr != m_playerDataMap.end()? itr->second.get():NULL;
	}
	inline PlayerDataMap &getPlayerDataMap(){ return m_playerDataMap; }
	crPlayerData *getPlayerDataByAddress(const std::string &address);//使用的时候必需用ref_ptr保证安全性
	//crPlayerData *getPlayerDataByName(const std::string &name);
	inline int getPlayerCount(){ return m_playerDataMap.size(); }
	inline void setMaxPlayerCount(int maxPlayerCount) { m_maxPlayerCount = maxPlayerCount; }
	int getMaxPlayerCount() { return m_maxPlayerCount; }
	bool checkPlayerCount() { return getPlayerCount() < m_maxPlayerCount; }

	//typedef std::multimap<std::string, std::string> ServerClientMap;//name,address
	//void regServerClient(const std::string& name, const std::string& address);
	//bool findServerClientAddress(const std::string& name, std::string& address, int line = 0);//line = -1表示随机选取
	//bool findServerClientName(const std::string& address, std::string& name);
	//void unregServerClientByName(const std::string& name);
	//void unregServerClientByAddress(const std::string& address);
	void serverRemoveConnection(const std::string &address);
	void sendPacketToAllPlayer(crStreamPacket &packet);
public:

	void attachNetManager(crNetManager *netManager){ m_netManager = netManager; }
	inline crNetManager *getNetManager(){ return m_netManager; }
protected:
	virtual ~crNetDataManager();

	//NameClientMap m_nameClientMap;
	//ClientDataMap m_clientDataMap;
	//float m_sqr_maxDistance;

	std::string m_serverName;//server
	std::string m_serverPassword;//server

	CRCore::ref_ptr<crNetCallback>   m_netCallback;

	ReceivedStreamList m_receivedStreamList;
	ReceivedStreamList m_receivedStreamList_buf;
	GNE::Mutex m_streamMutex;

	crNetManager *m_netManager;
    
	//float m_clientStamp;
	PlayerDataMap m_playerDataMap;

	int m_maxPlayerCount;

	GNE::Mutex m_playerMapMutex;

	//ServerClientMap m_serverClientMap;
	//CRCore::crMutex m_serverClientMapMutex;
};
}
#endif
