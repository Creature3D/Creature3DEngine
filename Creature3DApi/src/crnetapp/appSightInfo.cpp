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
#include <CRNetApp/appSightInfo.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appMsg.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
using namespace CRNetApp;
using namespace CRNet;
using namespace CRCore;
///////////////////////////////////
//
//crSightInfo
//
/////////////////////////////////////
crSightInfo::crSightInfo():
	//m_isRoomSight(false),
	m_roomid(0)
{
}
crSightInfo::~crSightInfo()
{
}
void crSightInfo::insertEyePoint(const CRCore::crVector2i &eyepoint)
{
	GNE::LockMutex lock( m_eyePointSetMutex );
	EyePointSet::iterator itr = m_eyePointSet.find(eyepoint);
	if( itr == m_eyePointSet.end() )
	{//将eyepoints发送给eyes
		std::vector<crVector2i> eyeVec;
		eyeVec.push_back(eyepoint);
		crEyePointsPacket packet;
		crEyePointsPacket::buildReplyPacket(packet,0,eyeVec);
		sendPacketToEyePlayer(packet);
		m_eyePointSet.insert(eyepoint);
	}
}
void crSightInfo::removeEyePoint(const CRCore::crVector2i &eyepoint)
{
	GNE::LockMutex lock( m_eyePointSetMutex );
	m_eyePointSet.erase(eyepoint);
}
void crSightInfo::update(crScene *scene, int roomid, int &roleeyecount, int &itemeyecount)
{
	bool haseye = false;
	if(roomid!=0)
	{
		scene->lockRoomRoleMap();
		scene->lockRoomItemMap();
		crScene::RoomRoleMap &roomRoleMap = scene->getRoomRoleMap();
		crScene::SceneRoleMap &sceneRoleMap = roomRoleMap[roomid];
		crScene::RoomItemMap &roomItemMap = scene->getRoomItemMap();
		crScene::SceneItemMap &_sceneItemMap = roomItemMap[roomid];
		crRole *role2;
		crInstanceItem *item;
		crData *eyeData;
		void *param;
		unsigned char itemState;
		unsigned int guiseState;
		{
			GNE::LockMutex lock1( m_itemEyeMapMutex );
			GNE::LockMutex lock2( m_playerEyeMapMutex );
			if(!m_playerEyeMap.empty())
			{
				crSceneServerPlayerData *eyePlayer;
				crRole *eye;
				int layerid;
				for( PlayerEyeMap::iterator itr = m_playerEyeMap.begin();
					itr != m_playerEyeMap.end();
					++itr )
				{
					eyePlayer = itr->second.get();
					crSceneServerPlayerData::RoleMap &roleMap = eyePlayer->getRoleMap();
					for( crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
						ritr != roleMap.end();
						++ritr )
					{
						eye = ritr->second.get();
						eyeData = eye->getDataClass();
						eyeData->getParam(WCHDATA_ItemState, param);
						itemState = *(unsigned char *)param;
						if (itemState == IS_Dead)
							continue;
						guiseState = GS_Normal;
						eye->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guiseState,NULL));
						if(guiseState & GS_StaticUnVisiable || guiseState & GS_UnVisiable || guiseState & GS_Blind)
						{//
							continue;
						}
						layerid = eye->getLayerID();
						if (layerid == 0xffff)
						{
							continue;
						}
						for( crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
							sritr != sceneRoleMap.end();
							++sritr )
						{
							role2 = sritr->second.get();
							if(role2->getPlayerID() != eye->getPlayerID() && role2->getLayerID() == layerid && !isEyeRole(role2->getPlayerID()))
							{
								eye->doEvent(WCH_PlayerInRangeTest,MAKECREPARAM(this,role2));
							}
						}
						for( crScene::SceneItemMap::iterator siitr = _sceneItemMap.begin();
							siitr != _sceneItemMap.end();
							++siitr )
						{
							item = siitr->second.get();
							if(item->getLayerID() == layerid && !isEyeItem(item->getInstanceItemID()))
							{
								//if (item->getItemtype() == crInstanceItem::Role)
								//	eye->doEvent(WCH_PlayerInRangeTest, MAKEINT64(this, item));
								//else
									eye->doEvent(WCH_ItemInRangeTest,MAKECREPARAM(this,item));
							}
						}
						haseye = true;
						roleeyecount++;
					}
				}
			}
		}
		{
			GNE::LockMutex lock( m_itemEyeMapMutex );
			if(!m_itemEyeMap.empty())
			{
				crInstanceItem *eye;
				int layerid;
				for( ItemEyeMap::iterator itr = m_itemEyeMap.begin();
					itr != m_itemEyeMap.end();
					++itr )
				{
					eye = itr->second.get();
					eyeData = eye->getDataClass();
					eyeData->getParam(WCHDATA_ItemState, param);
					itemState = *(unsigned char *)param;
					if (itemState == IS_Dead)
						continue;
					guiseState = GS_Normal;
					eye->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guiseState,NULL));
					if(guiseState & GS_StaticUnVisiable || guiseState & GS_UnVisiable || guiseState & GS_Blind)
					{//
						continue;
					}
					layerid = eye->getLayerID();
					if (layerid == 0xffff)
					{
						continue;
					}
					for( crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
						sritr != sceneRoleMap.end();
						++sritr )
					{
						role2 = sritr->second.get();
						if(role2->getLayerID() == layerid && !isEyeRole(role2->getPlayerID()))
						{
							eye->doEvent(WCH_PlayerInRangeTest,MAKECREPARAM(this,role2));
						}
					}
					for( crScene::SceneItemMap::iterator siitr = _sceneItemMap.begin();
						siitr != _sceneItemMap.end();
						++siitr )
					{
						item = siitr->second.get();
						if(item->getLayerID() == layerid && !isEyeItem(item->getInstanceItemID()))
						{
							//if (item->getItemtype() == crInstanceItem::Role)
							//	eye->doEvent(WCH_PlayerInRangeTest, MAKEINT64(this, item));
							//else
								eye->doEvent(WCH_ItemInRangeTest,MAKECREPARAM(this, item));
						}
					}
					haseye = true;
					itemeyecount++;
				}
			}
		}
		{
			GNE::LockMutex lock( m_eyePointSetMutex );
			for( EyePointSet::iterator itr = m_eyePointSet.begin();
				itr != m_eyePointSet.end();
				++itr )
			{
				for( crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
					sritr != sceneRoleMap.end();
					++sritr )
				{
					role2 = sritr->second.get();
					if (role2->getLayerID() != 0xffff && !isEyeRole(role2->getPlayerID()))
					{
						roleInEyePointTest(*itr,role2);
					}
				}
				for( crScene::SceneItemMap::iterator siitr = _sceneItemMap.begin();
					siitr != _sceneItemMap.end();
					++siitr )
				{
					item = siitr->second.get();
					if (item->getLayerID() != 0xffff && !isEyeItem(item->getInstanceItemID()))
					{
						//if (item->getItemtype() == crInstanceItem::Role)
						//	roleInEyePointTest(*itr, dynamic_cast<crRole*>(item));
						//else
							itemInEyePointTest(*itr, item);
					}
				}
				haseye = true;
			}
		}
		if(!haseye)
		{
			{
				GNE::LockMutex lock( m_inSightRolePlayerMutex );
				for( RolePlayerMap::iterator itr = m_inSightRolePlayer.begin();
					 itr != m_inSightRolePlayer.end();
					 ++itr )
				{
					insertOutSightItem(itr->second.get());
				}
			}
			{
				GNE::LockMutex lock( m_inSightItemMutex );
				for( ItemMap::iterator itr = m_inSightItem.begin();
					itr != m_inSightItem.end();
					++itr )
				{
					insertOutSightItem(itr->second.get());
				}
			}
		}
		outSightUpdate();
		roleInSightUpdate();
		itemInSightUpdate();
		scene->unlockRoomRoleMap();
		scene->unlockRoomItemMap();
	}
}
void crSightInfo::clearAllInsights()
{
	{
		GNE::LockMutex lock( m_inSightRolePlayerMutex );
		m_inSightRolePlayer.clear();
	}
	{
		GNE::LockMutex lock( m_inSightItemMutex );
		m_inSightItem.clear();
	}
}
void crSightInfo::roleInEyePointTest(const CRCore::crVector2i &eye,crRole *role)
{
	//void *param;
	//crData *data = role->getDataClass();
	//data->getParam(WCHDATA_GuiseState,param);
	//if(param)
	//{
	//	unsigned char guiseState = *(unsigned char *)param;
	//	if(guiseState == GS_UnVisiable || guiseState == GS_StaticUnVisiable)
	//	{//
	//		if(isRoleInSight(role))
	//		{
	//			insertOutSightItem(role);
	//		}
	//		return;
	//	}
	//}
	unsigned int guisestate = GS_Normal;
	role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
	if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable || (guisestate & GS_Hide && !(guisestate & GS_UnHide)))
	{
		if(isRoleInSight(role))
		{
			insertOutSightItem(role);
		}
		return;
	}

	bool inRange = true;
	float scale = crGlobalHandle::gData()->gUnitScale();
	float sightRange = crGlobalHandle::gData()->gEyePointRange()*scale;
	crVector2 itemPos(role->getPosx(), role->getPosy());
	crVector2 eyePos(eye[0], eye[1]);
	itemPos *= scale;
	eyePos *= scale;
	float dist = (eye - itemPos).length();
	if(dist>sightRange)
	{
		inRange = false;
	}
	if(inRange)
		insertInSightRole(role);
	else
	{
		if(isRoleInSight(role))
		{
			insertOutSightItem(role);
		}
	}
}
void crSightInfo::itemInEyePointTest(const CRCore::crVector2i &eye,crInstanceItem *item)
{
	//void *param;
	//crData *data = item->getDataClass();
	//data->getParam(WCHDATA_GuiseState,param);
	//if(param)
	//{
	//	unsigned char guiseState = *(unsigned char *)param;
	//	if(guiseState == GS_UnVisiable || guiseState == GS_StaticUnVisiable)
	//	{//
	//		if(isItemInSight(item))
	//		{
	//			insertOutSightItem(item);
	//		}
	//		return;
	//	}
	//}
	unsigned int guisestate = GS_Normal;
	item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
	if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable || (guisestate & GS_Hide && !(guisestate & GS_UnHide)))
	{
		if(isItemInSight(item))
		{
			insertOutSightItem(item);
		}
		return;
	}
	bool inRange = true;
	float scale = crGlobalHandle::gData()->gUnitScale();
	float sightRange = crGlobalHandle::gData()->gEyePointRange()*scale;
	crVector2 itemPos(item->getPosx(),item->getPosy());
	crVector2 eyePos(eye[0],eye[1]);
	itemPos *= scale;
	eyePos *= scale;
	float dist = (eyePos - itemPos).length();
	if(dist>sightRange)
	{
		inRange = false;
	}
	if(inRange)
		insertInSightItem(item);
	else
	{
		if(isItemInSight(item))
		{
			insertOutSightItem(item);
		}
	}
}
void crSightInfo::addEyeItem(crInstanceItem *item)
{
	GNE::LockMutex lock( m_itemEyeMapMutex );
	crItemInRangePacket inpacket;
	ref_ptr<crStreamBuf> rtDataStream;
	item->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
	if(rtDataStream.valid())
	{
		crItemInRangePacket::buildRequestPacket(inpacket,0,item,rtDataStream.get());
		sendPacketToEyePlayer(inpacket);
	}
	m_itemEyeMap[item->getInstanceItemID()] = item;
	item->setSightInfo(this);
}
void crSightInfo::removeEyeItem(int itemid)
{
	GNE::LockMutex lock( m_itemEyeMapMutex );
	ItemEyeMap::iterator itr = m_itemEyeMap.find(itemid);
	if(itr != m_itemEyeMap.end())
	{
		crItemOutRangePacket packet;
		crItemOutRangePacket::buildRequestPacket(packet,0,itr->second.get());
		sendPacketToEyePlayer(packet);
		//itr->second->setSightInfo(NULL);
		m_itemEyeMap.erase(itr);
	}
}
bool crSightInfo::isEyeItem(int itemid)
{
	GNE::LockMutex lock( m_itemEyeMapMutex );
	return m_itemEyeMap.find(itemid) != m_itemEyeMap.end();
}
void crSightInfo::lockItemEyeMap()
{
	m_itemEyeMapMutex.acquire();
}
crSightInfo::ItemEyeMap &crSightInfo::getItemEyeMap()
{
	return m_itemEyeMap;
}
void crSightInfo::unlockItemEyeMap()
{
	m_itemEyeMapMutex.release();
}
void crSightInfo::addEyePlayer(crSceneServerPlayerData *player)
{
	int playerid = player->getPlayerID();
	//if(!isEyeRole(playerid))
	{
		void *param;
		crData *data;
		unsigned char itemstate;
		crItemInRangePacket inpacket;
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetManager *netManager = sceneServer->getNetManager();
		crRole *role;
		crInstanceItem *item;
		//将视野信息同步给我
		{
			GNE::LockMutex lock( m_itemEyeMapMutex );
			for( ItemEyeMap::iterator itr = m_itemEyeMap.begin();
				 itr != m_itemEyeMap.end();
				 ++itr )
			{
				item = itr->second.get();
				ref_ptr<crStreamBuf> rtDataStream;
				item->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket::buildRequestPacket(inpacket,playerid,item,rtDataStream.get());
					netManager->sendPacket(player->getPlayerConnectServerAddress(),inpacket);
				}
			}
		}
		{
			GNE::LockMutex lock( m_inSightRolePlayerMutex );
			for( RolePlayerMap::iterator itr = m_inSightRolePlayer.begin();
				itr != m_inSightRolePlayer.end();
				++itr )
			{
				role = itr->second.get();
				ref_ptr<crStreamBuf> rtDataStream;
				role->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket::buildRequestPacket(inpacket,playerid,role,rtDataStream.get());
					netManager->sendPacket(player->getPlayerConnectServerAddress(),inpacket);
				}
			}
		}
		{
			GNE::LockMutex lock( m_inSightItemMutex );
			for( ItemMap::iterator itr = m_inSightItem.begin();
				itr != m_inSightItem.end();
				++itr )
			{
				item = itr->second.get();
				ref_ptr<crStreamBuf> rtDataStream;
				item->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket::buildRequestPacket(inpacket,playerid,item,rtDataStream.get());
					netManager->sendPacket(player->getPlayerConnectServerAddress(),inpacket);
				}
			}
		}
		{
			GNE::LockMutex lock( m_eyePointSetMutex );
			std::vector<crVector2i> eyeVec;
			for( EyePointSet::iterator itr = m_eyePointSet.begin();
				 itr != m_eyePointSet.end();
				 ++itr )
			{
				eyeVec.push_back(*itr);
			}
			crEyePointsPacket packet;
			crEyePointsPacket::buildReplyPacket(packet,playerid,eyeVec);
			netManager->sendPacket(player->getPlayerConnectServerAddress(),packet);
		}
		{
			GNE::LockMutex lock( m_playerEyeMapMutex );
			crSceneServerPlayerData *eyeplayer;
			for( PlayerEyeMap::iterator itr = m_playerEyeMap.begin();
				itr != m_playerEyeMap.end();
				++itr )
			{
				eyeplayer = itr->second.get();
				crSceneServerPlayerData::RoleMap &eyeRoleMap = eyeplayer->getRoleMap();
				for( crSceneServerPlayerData::RoleMap::iterator eitr = eyeRoleMap.begin();
					eitr != eyeRoleMap.end();
					++eitr )
				{
					role = eitr->second.get();
					data = role->getDataClass();
					data->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						ref_ptr<crStreamBuf> rtDataStream;
						role->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
						if(rtDataStream.valid())
						{
							crItemInRangePacket::buildRequestPacket(inpacket,playerid,role,rtDataStream.get());
							netManager->sendPacket(player->getPlayerConnectServerAddress(),inpacket);
						}
					}
				}
			}
			//player->lockRoleMap();
			//将我的信息同步给队友
			crSceneServerPlayerData::RoleMap &roleMap = player->getRoleMap();
			for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
				itr != roleMap.end();
				++itr )
			{
				ref_ptr<crStreamBuf> rtDataStream;
				role = itr->second.get();
				role->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket::buildRequestPacket(inpacket,0,role,rtDataStream.get());
					sendPacketToEyePlayer(inpacket);
				}
				role->setSightInfo(this);
			}
			//player->unlockRoleMap();
			//将我加入到playerEyeMap
			m_playerEyeMap[player->getPlayerID()] = player;
		}
	}
}
void crSightInfo::removeEyePlayer(int playerid)
{
	GNE::LockMutex lock(m_playerEyeMapMutex);
	PlayerEyeMap::iterator itr = m_playerEyeMap.find(playerid);
	ref_ptr<crSceneServerPlayerData> player;
	if (itr != m_playerEyeMap.end())
	{
		player = itr->second.get();
		m_playerEyeMap.erase(itr);
		if (!m_playerEyeMap.empty())
		{
			//player->lockRoleMap();
			crSceneServerPlayerData::RoleMap &roleMap = player->getRoleMap();
			crItemOutRangePacket packet;
			crRole *role;
			for (crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
				ritr != roleMap.end();
				++ritr)
			{
				role = ritr->second.get();
				crItemOutRangePacket::buildRequestPacket(packet, 0, role);
				sendPacketToEyePlayer(packet);
				role->setSightInfo(NULL);
			}
			//player->unlockRoleMap();
		}
	}
}
bool crSightInfo::isEyeRole(int playerid)
{
	GNE::LockMutex lock(m_playerEyeMapMutex);
	return m_playerEyeMap.find(playerid) != m_playerEyeMap.end();
}
bool crSightInfo::isEmpty()
{
	GNE::LockMutex lock(m_playerEyeMapMutex);
	return m_playerEyeMap.empty();
}
bool crSightInfo::isEmptySight()
{
	bool empty = true;
	{
		GNE::LockMutex lock(m_playerEyeMapMutex);
		empty &= m_playerEyeMap.empty();
	}
	{
		GNE::LockMutex lock(m_itemEyeMapMutex);
		empty &= m_itemEyeMap.empty();
	}
	return empty;
}
void crSightInfo::sendPacketToEyePlayer(CRNet::crStreamPacket &packet,int exceptPlayerid)
{
	GNE::LockMutex lock(m_playerEyeMapMutex);
	if(m_playerEyeMap.empty())
		return;
	crSceneServerPlayerData *player;
	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetManager *netManager = sceneServer->getNetManager();
	ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
	int bufsize = packetStream->getBufSize();
	//short delay;
	//_crInt64 msg;
	for( PlayerEyeMap::iterator itr = m_playerEyeMap.begin();
		itr != m_playerEyeMap.end();
		++itr )
	{
		player = itr->second.get();
		if(player && player->getPlayerID() != exceptPlayerid)
		{
			packetStream->seekBegin();
			packetStream->_writeInt(player->getPlayerID());
			//if(packet.getType() == WCHNET_PLAYEREVENT || packet.getType() == WCHNET_ITEMEVENT)
			//{
			//	msg = packetStream->_readInt64();
			//	if(msg == WCH_RecvPathFinding)
			//	{
			//		packetStream->seekEnd();
			//		packetStream->seek(-2);
			//		delay = packetStream->_readShort();
			//		delay += player->getDelayTime();
			//		packetStream->_writeShort(delay);
			//	}
			//}
			packetStream->setBufSize(bufsize);
			netManager->sendPacket(player->getPlayerConnectServerAddress(),packet);
		}
	}
}
void crSightInfo::lockPlayerEyeMap()
{
	m_playerEyeMapMutex.acquire();
}
crSightInfo::PlayerEyeMap &crSightInfo::getPlayerEyeMap()
{
	return m_playerEyeMap;
}
void crSightInfo::unlockPlayerEyeMap()
{
	m_playerEyeMapMutex.release();
}
bool crSightInfo::isRoleInSight(crRole *role)
{
	if (role->getID() < 0)
	{
		return isItemInSight(role);
	}
	else
	{
		GNE::LockMutex lock(m_inSightRolePlayerMutex);
		return m_inSightRolePlayer.find(MAKEINT64(role->getPlayerID(), role->getRoleID())) != m_inSightRolePlayer.end();
	}
}
void crSightInfo::lockInSightRolePlayerMap()
{
	m_inSightRolePlayerMutex.acquire();
}
crSightInfo::RolePlayerMap &crSightInfo::getInSightRolePlayerMap()
{
	return m_inSightRolePlayer;
}
void crSightInfo::unlockInSightRolePlayerMap()
{
	m_inSightRolePlayerMutex.release();
}
void crSightInfo::insertInSightRole(crRole *role)
{
	GNE::LockMutex lock(m_inSightRolePlayerSetMutex);
	m_inSightRolePlayerSet.insert(role);
}
void crSightInfo::roleInSightUpdate()
{
	GNE::LockMutex lock(m_inSightRolePlayerSetMutex);
	GNE::LockMutex lock1(m_inSightRolePlayerMutex);
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	int count = 0;
	bool playerInRange;
	//crSceneServerPlayerData *player;
	ref_ptr<crRole> role;
	_crInt64 id;
	crItemInRangePacket inpacket;
	crItemOutRangePacket outpacket;
	for( InSightRolePlayerSet::iterator itr = m_inSightRolePlayerSet.begin();
		itr != m_inSightRolePlayerSet.end();
		++itr,++count)
	{
		role = *itr;
		//player = itr->second.second.get();
		//role = player->getRole();
		id = MAKEINT64(role->getPlayerID(),role->getRoleID());
		playerInRange = m_inSightRolePlayer.find(id) != m_inSightRolePlayer.end();
		if(count<crGlobalHandle::gData()->gMaxInRangePlayer())
		{
			if(!playerInRange)
			{
				m_inSightRolePlayer[id] = role;
				ref_ptr<crStreamBuf> rtDataStream;
				role->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket::buildRequestPacket(inpacket,0,role.get(),rtDataStream.get());
					sendPacketToEyePlayer(inpacket);
				}
			}
		}
		else if(playerInRange)
		{//超出最大数量
			m_inSightRolePlayer.erase(id);
			crItemOutRangePacket::buildRequestPacket(outpacket,0,role.get());
			sendPacketToEyePlayer(outpacket);
			//sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),outpacket);
		}
	}
	m_inSightRolePlayerSet.clear();
}
void crSightInfo::removeInSightPlayer(crSceneServerPlayerData *player)
{
	GNE::LockMutex lock(m_inSightRolePlayerMutex);
	//player->lockRoleMap();
	crSceneServerPlayerData::RoleMap &roleMap = player->getRoleMap();
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	//crNetManager *netManager = sceneServerConductor->getNetManager();
	crItemOutRangePacket packet;
	RolePlayerMap::iterator ritr;
	crRole *role;
	for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
		itr != roleMap.end();
		++itr )
	{
		role = itr->second.get();
		ritr = m_inSightRolePlayer.find(MAKEINT64(role->getPlayerID(),role->getRoleID()));
		if(ritr!=m_inSightRolePlayer.end())
		{
			crItemOutRangePacket::buildRequestPacket(packet,0,role);
			sendPacketToEyePlayer(packet);
			//netManager->sendPacket(getPlayerConnectServerAddress(),packet);
			m_inSightRolePlayer.erase(ritr);
		}
	}
	//player->unlockRoleMap();
}
void crSightInfo::removeInSightPlayer2(crSceneServerPlayerData *player)
{
	GNE::LockMutex lock(m_inSightRolePlayerMutex);
	//player->lockRoleMap();
	crSceneServerPlayerData::RoleMap &roleMap = player->getRoleMap();
	RolePlayerMap::iterator ritr;
	crRole *role;
	for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
		itr != roleMap.end();
		++itr )
	{
		role = itr->second.get();
		ritr = m_inSightRolePlayer.find(MAKEINT64(role->getPlayerID(),role->getRoleID()));
		if(ritr!=m_inSightRolePlayer.end())
		{
			m_inSightRolePlayer.erase(ritr);
		}
	}
	//player->unlockRoleMap();
}
void crSightInfo::removeInSightRole(crRole *role)
{
	if(role)
	{
		GNE::LockMutex lock(m_inSightRolePlayerMutex);
		RolePlayerMap::iterator ritr = m_inSightRolePlayer.find(MAKEINT64(role->getPlayerID(),role->getRoleID()));
		if(ritr!=m_inSightRolePlayer.end())
		{
			m_inSightRolePlayer.erase(ritr);
			//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			//crNetManager *netManager = sceneServerConductor->getNetManager();
			crItemOutRangePacket packet;
			crItemOutRangePacket::buildRequestPacket(packet,0,role);
			sendPacketToEyePlayer(packet);
			//netManager->sendPacket(getPlayerConnectServerAddress(),packet);
		}
	}
}
//void crSightInfo::sendPacketToNeighbor(CRNet::crStreamPacket &packet,crSceneServerPlayerData *fromPlayer)
//{
//	GNE::LockMutex lock(m_inSightRolePlayerMutex);
//	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	crNetManager *netManager = sceneServer->getNetManager();
//	ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
//	int bufsize = packetStream->getBufSize();
//	crSceneServerPlayerData *player;
//	std::set<int> PlayeridSet;
//	int playerid;
//	std::set<crSightInfo *>SightSet;
//	for( RolePlayerMap::iterator itr = m_inSightRolePlayer.begin();
//		itr != m_inSightRolePlayer.end();
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
//			if(fromPlayer && packet.getType() == crRemovePlayerPacket::ID && itr->second.second->getSightInfo())
//			{
//				SightSet.insert(itr->second.second->getSightInfo());
//			}
//			PlayeridSet.insert(playerid);
//		}
//	}
//	for( std::set<crSightInfo *>::iterator itr = SightSet.begin();
//		 itr != SightSet.end();
//		 ++itr )
//	{
//		(*itr)->removeInSightPlayer2(fromPlayer);
//	}
//	//同时同步给共享视野的其它player
//	sendPacketToEyePlayer(packet,fromPlayer->getPlayerID());
//}
bool crSightInfo::isItemInSight(crInstanceItem *item)
{
	GNE::LockMutex lock(m_inSightItemMutex);
	return m_inSightItem.find(item->getInstanceItemID()) != m_inSightItem.end();
}
void crSightInfo::lockInSightItemMap()
{
	m_inSightItemMutex.acquire();
}
crSightInfo::ItemMap &crSightInfo::getInSightItemMap()
{
	return m_inSightItem;
}
void crSightInfo::unlockInSightItemMap()
{
	m_inSightItemMutex.release();
}
bool crSightInfo::isItemInSightByID(int itemid)
{
	GNE::LockMutex lock(m_inSightItemMutex);
	return m_inSightItem.find(itemid) != m_inSightItem.end();
}
void crSightInfo::insertInSightItem(crInstanceItem *item)
{
	GNE::LockMutex lock(m_inSightItemSetMutex);
	m_inSightItemSet.insert(item);
}
void crSightInfo::itemInSightUpdate()
{
	GNE::LockMutex lock(m_inSightItemSetMutex);
	GNE::LockMutex lock1(m_inSightItemMutex);
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	int count = 0;
	bool itemInRange;
	ref_ptr<crInstanceItem>item;
	for( InSightItemSet::iterator itr = m_inSightItemSet.begin();
		itr != m_inSightItemSet.end();
		++itr,++count)
	{
		item = *itr;
		itemInRange = m_inSightItem.find(item->getInstanceItemID()) != m_inSightItem.end();
		if(count<crGlobalHandle::gData()->gMaxInRangeItem())
		{
			if(!itemInRange)
			{
				m_inSightItem[item->getInstanceItemID()] = item;
				ref_ptr<crStreamBuf> rtDataStream;
				item->doEvent(WCH_GetItemRTData,MAKECREPARAM(&rtDataStream,NULL));
				if(rtDataStream.valid())
				{
					crItemInRangePacket packet;
					crItemInRangePacket::buildRequestPacket(packet,0,item.get(),rtDataStream.get());
					sendPacketToEyePlayer(packet);
				}
			}
		}
		else if(itemInRange)
		{
			m_inSightItem.erase(item->getInstanceItemID());
			crItemOutRangePacket packet;
			crItemOutRangePacket::buildRequestPacket(packet,0,item.get());
			sendPacketToEyePlayer(packet);
			//sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
		}
	}
	m_inSightItemSet.clear();
}
void crSightInfo::removeInSightItem(crInstanceItem *item)
{
	if(item)
	{
		GNE::LockMutex lock(m_inSightItemMutex);
		ItemMap::iterator itr = m_inSightItem.find(item->getInstanceItemID());
		if(itr!=m_inSightItem.end())
		{
			m_inSightItem.erase(itr);
			//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crItemOutRangePacket packet;
			crItemOutRangePacket::buildRequestPacket(packet,0,item);
			sendPacketToEyePlayer(packet);
			//sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
		}
	}
}
crInstanceItem *crSightInfo::findInSightItem(int itemid)
{
	GNE::LockMutex lock(m_inSightItemMutex);
	ItemMap::iterator itr = m_inSightItem.find(itemid);
	return itr!=m_inSightItem.end()?itr->second.get():NULL;
}
void crSightInfo::insertOutSightItem(crInstanceItem *item)
{
	GNE::LockMutex lock(m_outSightItemSetMutex);
	m_outSightItemSet.insert(item);
}
void crSightInfo::insertDropedItem(crInstanceItem *item)
{
	GNE::LockMutex lock(m_dropedItemSetMutex);
	m_dropedItemSet.insert(item);
}
void crSightInfo::outSightUpdate()
{
	GNE::LockMutex lock1(m_outSightItemSetMutex);
	GNE::LockMutex lock2(m_dropedItemSetMutex);
	{
		GNE::LockMutex lock3(m_inSightRolePlayerSetMutex);
		GNE::LockMutex lock4(m_inSightItemSetMutex);
		ref_ptr<crInstanceItem> item;
		for( OutSightItemSet::iterator itr = m_dropedItemSet.begin();
			itr != m_dropedItemSet.end();
			++itr )
		{
			item = *itr;
			if(dynamic_cast<crRole *>(item.get()))
				m_inSightRolePlayerSet.erase(dynamic_cast<crRole *>(item.get()));
			else
				m_inSightItemSet.erase(item.get());
		}
		ref_ptr<crRole> role;
		for( InSightRolePlayerSet::iterator itr = m_inSightRolePlayerSet.begin();
			itr != m_inSightRolePlayerSet.end();
			++itr )
		{
			role = *itr;
			m_outSightItemSet.erase(role.get());
		}
		for( InSightItemSet::iterator itr = m_inSightItemSet.begin();
			itr != m_inSightItemSet.end();
			++itr )
		{
			m_outSightItemSet.erase(*itr);
		}
	}
	{
		//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		//crNetManager *netManager = sceneServerConductor->getNetManager();
		crItemOutRangePacket packet;
		ref_ptr<crInstanceItem> item;
		RolePlayerMap::iterator ritr;
		ItemMap::iterator iitr;
		GNE::LockMutex lock5(m_inSightRolePlayerMutex);
		GNE::LockMutex lock6(m_inSightItemMutex);
		for( OutSightItemSet::iterator itr = m_outSightItemSet.begin();
			itr != m_outSightItemSet.end();
			++itr )
		{
			item = *itr;
			if(item->getItemtype() == crInstanceItem::Role)
			{
				ritr = m_inSightRolePlayer.find(MAKEINT64(item->getID(),item->getRoleID()));
				if(ritr!=m_inSightRolePlayer.end())
				{
					m_inSightRolePlayer.erase(ritr);
					crItemOutRangePacket::buildRequestPacket(packet,0,item.get());
					sendPacketToEyePlayer(packet);
					//netManager->sendPacket(getPlayerConnectServerAddress(),packet);
				}
			}
			else
			{
				iitr = m_inSightItem.find(item->getInstanceItemID());
				if(iitr!=m_inSightItem.end())
				{
					m_inSightItem.erase(iitr);
					crItemOutRangePacket::buildRequestPacket(packet,0,item.get());
					sendPacketToEyePlayer(packet);
					//sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
				}
			}
		}
		for( OutSightItemSet::iterator itr = m_dropedItemSet.begin();
			 itr != m_dropedItemSet.end();
			 ++itr )
		{
			item = *itr;
			if(item->getItemtype() == crInstanceItem::Role)
			{
				ritr = m_inSightRolePlayer.find(MAKEINT64(item->getID(),item->getRoleID()));
				if(ritr!=m_inSightRolePlayer.end())
				{
					m_inSightRolePlayer.erase(ritr);
					crItemOutRangePacket::buildRequestPacket(packet,0,item.get());
					sendPacketToEyePlayer(packet);
					//netManager->sendPacket(getPlayerConnectServerAddress(),packet);
				}
			}
			else
			{
				iitr = m_inSightItem.find(item->getInstanceItemID());
				if(iitr!=m_inSightItem.end())
				{
					m_inSightItem.erase(iitr);
					crItemOutRangePacket::buildRequestPacket(packet,0,item.get());
					sendPacketToEyePlayer(packet);
					//sceneServerConductor->getNetManager()->sendPacket(getPlayerConnectServerAddress(),packet);
				}
			}
		}
	}
	m_outSightItemSet.clear();
	m_dropedItemSet.clear();
}
//void crSightInfo::setRoomSight()
//{
//	m_isRoomSight = true;
//}
void crSightInfo::setRoomID(int roomid)
{
	m_roomid = roomid;
}
crInstanceItem *crSightInfo::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &coordPos)
{//返回碰撞的item
	int layerid = item->getLayerID();
	{
		GNE::LockMutex lock(m_inSightRolePlayerMutex);
		crRole *role;
		for( RolePlayerMap::iterator itr = m_inSightRolePlayer.begin();
			itr != m_inSightRolePlayer.end();
			++itr )
		{
			role = itr->second.get();
			if(role->getLayerID() == layerid && role->dynamicCollideTest(item,coordPos))
				return role;
		}
	}
	{
		GNE::LockMutex lock(m_inSightItemMutex);
		crInstanceItem *item;
		for( ItemMap::iterator itr = m_inSightItem.begin();
			itr != m_inSightItem.end();
			++itr )
		{
			item = itr->second.get();
			if(item->getLayerID() == layerid && item->dynamicCollideTest(item,coordPos))
				return item;
		}
	}
	return NULL;
}
void crSightInfo::sendPacketToInSight(crInstanceItem *item, CRNet::crStreamPacket &packet)
{
	crRole *role = dynamic_cast<crRole *>(item);
	int id = item->getID();
	if (role)
	{
		if ((id > 0 && isEyeRole(id)) || (id < 0 && isEyeItem(id)))
		{
			//if (id < 0)
			//	sightInfo->sendPacketToEyePlayer(packet, ownerplayerid);
			//else
			sendPacketToEyePlayer(packet, id);
		}
		else if (isRoleInSight(role)/* || sightInfo->isItemInSight(role)*/)
		{
			sendPacketToEyePlayer(packet);
		}
	}
	else
	{
		if (isItemInSight(item) || isEyeItem(id))
		{
			sendPacketToEyePlayer(packet);
		}
	}
}