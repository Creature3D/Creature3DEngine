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

#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp//appNetDefine.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crFileUtils.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRNetApp/appDBQuery.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appSightInfo.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CRDataBase;
///////////////////////////////////
//
//crParentServerPlayerData
//
/////////////////////////////////////
crParentServerPlayerData::crParentServerPlayerData()
{
}

///////////////////////////////////
//
//crChatServerPlayerData
//
/////////////////////////////////////
crChatServerPlayerData::crChatServerPlayerData()
{
}

void crChatServerPlayerData::setNickName(const std::string &nickName)
{
    m_nickName = nickName;
}
const std::string &crChatServerPlayerData::getNickName() const
{
    return m_nickName;
}
///////////////////////////////////
//
//crVoiceServerPlayerData
//
/////////////////////////////////////
crVoiceServerPlayerData::crVoiceServerPlayerData()
{
}
///////////////////////////////////
//
//crCameraServerPlayerData
//
/////////////////////////////////////
crCameraServerPlayerData::crCameraServerPlayerData()
{
}
///////////////////////////////////
//
//crChatConnectServerPlayerData
//
/////////////////////////////////////
crChatConnectServerPlayerData::crChatConnectServerPlayerData()
{
}
///////////////////////////////////
//
//crDownloadConnectServerPlayerData
//
/////////////////////////////////////
crDownloadConnectServerPlayerData::crDownloadConnectServerPlayerData()
{
}
///////////////////////////////////
//
//crGameServerPlayerData
//
/////////////////////////////////////
crGameServerPlayerData::crGameServerPlayerData():
m_gamedbid(0),
m_sceneid(0),
m_saveTimer(10.0f)
{
}
std::string crGameServerPlayerData::getCharacterName()
{
	//crRole *role = m_playerGameData->getMainRole();
	return m_playerGameData.valid()?m_playerGameData->getCharacterName():"";
}
void crGameServerPlayerData::setKey(const std::string &key,const std::string &username)
{
	m_key = key;
	m_username = username;
}
bool crGameServerPlayerData::checkKey(const std::string &key)
{
	return m_key.compare(key)==0;
}
const std::string &crGameServerPlayerData::getUserName()
{
	return m_username;
}
void crGameServerPlayerData::clearData()
{
	if(m_playerGameData.valid())
		m_playerGameData->clearData();
}
///////////////////////////////////
//
//crGameConnectServerPlayerData
//
/////////////////////////////////////
crGameConnectServerPlayerData::crGameConnectServerPlayerData()
{
}
///////////////////////////////////
//
//crSceneServerPlayerData
//
/////////////////////////////////////
crSceneServerPlayerData::crSceneServerPlayerData():
m_gamedbid(0),
m_clientRunning(false),
m_saveTimer(600.0f),
m_sceneid(0),
m_roomid(0),
m_delayTime(0)
{
}
void crSceneServerPlayerData::addRole(crRole *role)
{
	//GNE::LockMutex lock(m_roleMapMutex);
	if(role) m_roleMap[role->getRoleID()] = role;
}
crRole *crSceneServerPlayerData::getRole(int roleid)
{
	//GNE::LockMutex lock(m_roleMapMutex);
	RoleMap::iterator itr = m_roleMap.find(roleid);
	return itr != m_roleMap.end()?itr->second.get():NULL;
}
//void crSceneServerPlayerData::lockRoleMap()
//{
//	m_roleMapMutex.acquire();
//}
crSceneServerPlayerData::RoleMap &crSceneServerPlayerData::getRoleMap()
{
	return m_roleMap;
}
//void crSceneServerPlayerData::unlockRoleMap()
//{
//	m_roleMapMutex.release();
//}
//void crSceneServerPlayerData::setRole(crRole *role)
//{
//	//m_role = role;
//}
//crRole *crSceneServerPlayerData::getRole()
//{
//	return m_role.get();
//}
void crSceneServerPlayerData::setClientRunning(bool bln)
{
	m_clientRunning = bln;
}
//bool crSceneServerPlayerData::isRoleInRange(int playerid,int roleid)
//{
//    CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeRolePlayerMutex);
//	return m_inrangeRolePlayer.find(MAKEINT64(playerid,roleid)) != m_inrangeRolePlayer.end();
//}
//void crSceneServerPlayerData::insertInRangeRole(crRole *role,crSceneServerPlayerData *player,float dist)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inRangeRolePlayerMapMutex);
//	m_inRangeRolePlayerMap.insert(std::make_pair(dist,std::make_pair(role,player)));
//}
//void crSceneServerPlayerData::roleInRangeUpdate()
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inRangeRolePlayerMapMutex);
//	CRCore::ScopedLock<CRCore::crMutex> lock1(m_inrangeRolePlayerMutex);
//	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	int count = 0;
//	bool playerInRange;
//	crRole* role;
//	_crInt64 id;
//	crItemInRangePacket inpacket;
//	crItemOutRangePacket outpacket;
//	for( InRangeRolePlayerMap::iterator itr = m_inRangeRolePlayerMap.begin();
//		 itr != m_inRangeRolePlayerMap.end();
//		 ++itr,++count)
//	{
//		role = itr->second.first.get();
//		id = MAKEINT64(role->getPlayerID(),role->getRoleID());
//		playerInRange = m_inrangeRolePlayer.find(id) != m_inrangeRolePlayer.end();
//		if(count<crGlobalHandle::gData()->gMaxInRangePlayer())
//		{
//			if(!playerInRange)
//			{
//				m_inrangeRolePlayer[id] = itr->second;
//				ref_ptr<crStreamBuf> rtDataStream;
//				role->doEvent(WCH_GetItemRTData,MAKEINT64(&rtDataStream,NULL));
//				crItemInRangePacket::buildRequestPacket(inpacket,getPlayerID(),role,rtDataStream.get());
//				sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),inpacket);
//			}
//		}
//		else if(playerInRange)
//		{//超出最大数量
//			m_inrangeRolePlayer.erase(id);
//			crItemOutRangePacket::buildRequestPacket(outpacket,getPlayerID(),role);
//			sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),outpacket);
//		}
//	}
//	m_inRangeRolePlayerMap.clear();
//}
//void crSceneServerPlayerData::removeInRangePlayer(crSceneServerPlayerData *player)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeRolePlayerMutex);
//	RoleMap &roleMap = player->getRoleMap();
//	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	crNetManager *netManager = sceneServerConductor->getNetManager();
//	crItemOutRangePacket packet;
//	RolePlayerMap::iterator ritr;
//	crRole *role;
//	for( RoleMap::iterator itr = roleMap.begin();
//		 itr != roleMap.end();
//		 ++itr )
//	{
//		role = itr->second.get();
//		ritr = m_inrangeRolePlayer.find(MAKEINT64(role->getPlayerID(),role->getRoleID())/*player->getPlayerID()*/);
//		if(ritr!=m_inrangeRolePlayer.end())
//		{
//			m_inrangeRolePlayer.erase(ritr);
//			crItemOutRangePacket::buildRequestPacket(packet,getPlayerID(),role);
//			netManager->sendPacket(getPlayerConnectServerAddress(),packet);
//		}
//	}
//}
//void crSceneServerPlayerData::removeInRangeRole(crRole *role)
//{
//	if(role)
//	{
//		CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeRolePlayerMutex);
//		RolePlayerMap::iterator ritr = m_inrangeRolePlayer.find(MAKEINT64(role->getPlayerID(),role->getRoleID())/*player->getPlayerID()*/);
//		if(ritr!=m_inrangeRolePlayer.end())
//		{
//			m_inrangeRolePlayer.erase(ritr);
//			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//			crNetManager *netManager = sceneServerConductor->getNetManager();
//			crItemOutRangePacket packet;
//			crItemOutRangePacket::buildRequestPacket(packet,getPlayerID(),role);
//			netManager->sendPacket(getPlayerConnectServerAddress(),packet);
//		}
//	}
//}
//void crSceneServerPlayerData::sendPacketToNeighbor(CRNet::crStreamPacket &packet)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeRolePlayerMutex);
//	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
//    crNetManager *netManager = sceneServer->getNetManager();
//	ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
//	int bufsize = packetStream->getBufSize();
//	crSceneServerPlayerData *player;
//	std::set<int> PlayeridSet;
//	int playerid;
//	for( RolePlayerMap::iterator itr = m_inrangeRolePlayer.begin();
//		itr != m_inrangeRolePlayer.end();
//		++itr )
//	{
//		player = itr->second.second.get();
//		playerid = player->getPlayerID();
//		if(PlayeridSet.find(playerid)== PlayeridSet.end())
//		{
//			packetStream->seekBegin();
//			packetStream->_writeInt(playerid);
//			packetStream->setBufSize(bufsize);
//			netManager->sendPacket(player->getPlayerConnectServerAddress(),packet);
//
//			if(packet.getType() == crRemovePlayerPacket::ID)
//			{
//				player->removeInRangePlayer(this);
//			}
//			PlayeridSet.insert(playerid);
//		}
//	}
//}
//bool crSceneServerPlayerData::isItemInRange(int itemid)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeItemMutex);
//	return m_inrangeItem.find(itemid) != m_inrangeItem.end();
//}
//void crSceneServerPlayerData::insertInRangeItemMap(crInstanceItem *item,float dist)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inRangeItemMapMutex);
//	m_inRangeItemMap.insert(std::make_pair(dist,item));
//}
//void crSceneServerPlayerData::itemInRangeUpdate()
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inRangeItemMapMutex);
//	CRCore::ScopedLock<CRCore::crMutex> lock1(m_inrangeItemMutex);
//	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	int count = 0;
//	bool itemInRange;
//	crInstanceItem *item;
//	for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
//		itr != m_inRangeItemMap.end();
//		++itr,++count)
//	{
//		item = itr->second.get();
//		itemInRange = m_inrangeItem.find(item->getInstanceItemID()) != m_inrangeItem.end();
//		if(count<crGlobalHandle::gData()->gMaxInRangeItem())
//		{
//			if(!itemInRange)
//			{
//				m_inrangeItem[item->getInstanceItemID()] = item;
//				ref_ptr<crStreamBuf> rtDataStream;
//				item->doEvent(WCH_GetItemRTData,MAKEINT64(&rtDataStream,NULL));
//				crItemInRangePacket packet;
//				crItemInRangePacket::buildRequestPacket(packet,getPlayerID(),item,rtDataStream.get());
//				sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
//			}
//		}
//		else if(itemInRange)
//		{
//			m_inrangeItem.erase(item->getInstanceItemID());
//			crItemOutRangePacket packet;
//			crItemOutRangePacket::buildRequestPacket(packet,getPlayerID(),item);
//			sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
//		}
//	}
//	m_inRangeItemMap.clear();
//}
//void crSceneServerPlayerData::removeInRangeItem(crInstanceItem *item)
//{
//	if(item)
//	{
//		CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeItemMutex);
//		ItemMap::iterator itr = m_inrangeItem.find(item->getInstanceItemID());
//		if(itr!=m_inrangeItem.end())
//		{
//			m_inrangeItem.erase(itr);
//			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//			crItemOutRangePacket packet;
//			crItemOutRangePacket::buildRequestPacket(packet,getPlayerID(),item);
//			sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
//		}
//	}
//}
//crInstanceItem *crSceneServerPlayerData::findInRangeItem(int itemid)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_inrangeItemMutex);
//	ItemMap::iterator itr = m_inrangeItem.find(itemid);
//	return itr!=m_inrangeItem.end()?itr->second.get():NULL;
//}
//void crSceneServerPlayerData::insertOutRangeItem(crInstanceItem *item)
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_outRangeItemSetMutex);
//	m_outRangeItemSet.insert(item);
//}
//void crSceneServerPlayerData::outRangeUpdate()
//{
//	CRCore::ScopedLock<CRCore::crMutex> lock(m_outRangeItemSetMutex);
//	{
//		CRCore::ScopedLock<CRCore::crMutex> lock1(m_inRangeRolePlayerMapMutex);
//		for( InRangeRolePlayerMap::iterator itr = m_inRangeRolePlayerMap.begin();
//			 itr != m_inRangeRolePlayerMap.end();
//			 ++itr )
//		{
//			m_outRangeItemSet.erase(itr->second.first.get());
//		}
//	}
//	{
//		CRCore::ScopedLock<CRCore::crMutex> lock1(m_inRangeItemMapMutex);
//		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
//			itr != m_inRangeItemMap.end();
//			++itr )
//		{
//			m_outRangeItemSet.erase(itr->second.get());
//		}
//	}
//	{
//		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//		crNetManager *netManager = sceneServerConductor->getNetManager();
//		crItemOutRangePacket packet;
//		ref_ptr<crInstanceItem> item;
//		RolePlayerMap::iterator ritr;
//		ItemMap::iterator iitr;
//		CRCore::ScopedLock<CRCore::crMutex> lock1(m_inrangeRolePlayerMutex);
//		CRCore::ScopedLock<CRCore::crMutex> lock2(m_inrangeItemMutex);
//		for( OutRangeItemSet::iterator itr = m_outRangeItemSet.begin();
//			itr != m_outRangeItemSet.end();
//			++itr )
//		{
//			item = *itr;
//			if(item->getItemtype() == crInstanceItem::Role)
//			{
//				ritr = m_inrangeRolePlayer.find(MAKEINT64(item->getID(),item->getRoleID()));
//				if(ritr!=m_inrangeRolePlayer.end())
//				{
//					m_inrangeRolePlayer.erase(ritr);
//					crItemOutRangePacket::buildRequestPacket(packet,m_playerID,item.get());
//					netManager->sendPacket(getPlayerConnectServerAddress(),packet);
//				}
//			}
//			else
//			{
//				iitr = m_inrangeItem.find(item->getInstanceItemID());
//				if(iitr!=m_inrangeItem.end())
//				{
//					m_inrangeItem.erase(iitr);
//					crItemOutRangePacket::buildRequestPacket(packet,m_playerID,item.get());
//					sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
//				}
//			}
//		}
//	}
//	m_outRangeItemSet.clear();
//}
//void crSceneServerPlayerData::clearInRanges()
//{
//    CRCore::ScopedLock<CRCore::crMutex> lock1(m_inrangeRolePlayerMutex);
//	CRCore::ScopedLock<CRCore::crMutex> lock2(m_inrangeItemMutex);
//	m_inrangeRolePlayer.clear();
//	m_inrangeItem.clear();
//}
void crSceneServerPlayerData::setRoomID(int roomid)
{
	//GNE::LockMutex lock(m_roleMapMutex);
	m_roomid = roomid;
	//clearInRanges();
	for( RoleMap::iterator itr = m_roleMap.begin();
		 itr != m_roleMap.end();
		 ++itr )
	{
		itr->second->setRoomID(m_roomid);
	}
	if(m_playerGameData.valid() && m_playerGameData->getMainRole())
	{
		m_playerGameData->getMainRole()->setRoomID(roomid);
	}
}
int crSceneServerPlayerData::getRoomID()
{
	return m_roomid;
}
void crSceneServerPlayerData::setPlayerGameData(crPlayerGameData *data)
{
	m_playerGameData = data;
}
crPlayerGameData *crSceneServerPlayerData::getPlayerGameData()
{
	return m_playerGameData.get();
}
std::string crSceneServerPlayerData::getCharacterName()
{
	//GNE::LockMutex lock(m_roleMapMutex);
	crRole *role = m_playerGameData->getMainRole();
	if(!role && !m_roleMap.empty())
		role = m_roleMap.begin()->second.get();
	return role?role->getIName2():"";
}
void crSceneServerPlayerData::setSightInfo(crSightInfo *sightinfo)
{
	if(m_sightInfo == sightinfo)
		return;
	m_sightInfo = sightinfo;
	if(m_sightInfo.valid())
	{
		m_sightInfo->addEyePlayer(this);
	}
}
crSightInfo *crSceneServerPlayerData::getSightInfo()
{
	return m_sightInfo.get();
}
int crSceneServerPlayerData::getLayerid()
{
	//GNE::LockMutex lock(m_roleMapMutex);
	if(!m_roleMap.empty())
	{
		return m_roleMap.begin()->second->getLayerID();
	}
	return 0;
}
void crSceneServerPlayerData::setDelayTime(short delayTime)
{
	m_delayTime = delayTime;
}
short crSceneServerPlayerData::getDelayTime()
{
	return m_delayTime;
}
void crSceneServerPlayerData::clearData()
{
	for( RoleMap::iterator itr = m_roleMap.begin();
		itr != m_roleMap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_roleMap.clear();
	if(m_playerGameData.valid())
	{
		m_playerGameData->clearData();
	}
	m_sightInfo = NULL;
}
/////////////////////////////////////
////
////crSceneConnectServerPlayerData
////
///////////////////////////////////////
//crSceneConnectServerPlayerData::crSceneConnectServerPlayerData()
//{
//}