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
#ifndef CRNETAPP_APPSIGHTINFO_H
#define CRNETAPP_APPSIGHTINFO_H 1

#include <CRNetApp/appExport.h>
#include <CRNet/crNetContainer.h>
#include <CRNet/crNetDataManager.h>
#include <CRNetApp/appNetChatData.h>
#include <map>
#include <vector>
#include <deque>

namespace CRNetApp {
class crSceneServerPlayerData;
class crInstanceItem;
class crRole;
class crScene;
typedef std::set< CRCore::crVector2i >EyePointSet;//x,y
//typedef std::pair< CRCore::ref_ptr<crRole>, CRCore::ref_ptr<crSceneServerPlayerData> > RolePlayerPair;
class CRNETAPP_EXPORT crSightInfo :  public CRCore::Referenced
{
public:
	crSightInfo();
	typedef std::map< int,CRCore::ref_ptr<crInstanceItem> >ItemEyeMap;
	void addEyeItem(crInstanceItem *item);//item出生或复活时候触发
	void removeEyeItem(int itemid);//item死亡时候触发
	bool isEyeItem(int itemid);
	void lockItemEyeMap();
	ItemEyeMap &getItemEyeMap();
	void unlockItemEyeMap();
	typedef std::map< int,CRCore::ref_ptr<crSceneServerPlayerData> > PlayerEyeMap;//playerid
	void addEyePlayer(crSceneServerPlayerData *player);
	void removeEyePlayer(int playerid);
	bool isEyeRole(int playerid);
	bool isEmpty();
	bool isEmptySight();
	void sendPacketToEyePlayer(CRNet::crStreamPacket &packet,int exceptPlayerid = 0);
	void lockPlayerEyeMap();
	PlayerEyeMap &getPlayerEyeMap();
	void unlockPlayerEyeMap();

	void insertEyePoint(const CRCore::crVector2i &eyepoint);
	void removeEyePoint(const CRCore::crVector2i &eyepoint);
	void update(crScene *scene, int roomid,int &roleeyecount, int &itemeyecount);

	typedef std::map<_crInt64,CRCore::ref_ptr<crRole> > RolePlayerMap;//playerid,roleid
	typedef std::set< CRCore::ref_ptr<crRole> >InSightRolePlayerSet;
	bool isRoleInSight(crRole *role);
	void lockInSightRolePlayerMap();
	RolePlayerMap &getInSightRolePlayerMap();
	void unlockInSightRolePlayerMap();
	void insertInSightRole(crRole *role);
	void roleInSightUpdate();
	void removeInSightPlayer(crSceneServerPlayerData *player);
	void removeInSightRole(crRole *role);

	//void sendPacketToNeighbor(CRNet::crStreamPacket &packet,crSceneServerPlayerData *fromPlayer = NULL);

	typedef std::map<int, CRCore::ref_ptr<crInstanceItem> > ItemMap;
	typedef std::set<CRCore::ref_ptr<crInstanceItem> > InSightItemSet;
	bool isItemInSight(crInstanceItem *item);
	void lockInSightItemMap();
	ItemMap &getInSightItemMap();
	void unlockInSightItemMap();
	bool isItemInSightByID(int itemid);
	void insertInSightItem(crInstanceItem *item);
	void itemInSightUpdate();
	void removeInSightItem(crInstanceItem *item);
	crInstanceItem *findInSightItem(int itemid);

	typedef std::set< CRCore::ref_ptr<crInstanceItem> > OutSightItemSet;
	void insertOutSightItem(crInstanceItem *item);
	void insertDropedItem(crInstanceItem *item);
	void outSightUpdate();;
	//void setRoomSight();
	//inline bool isRoomSight(){ return m_isRoomSight; }
	void setRoomID(int roomid);
	inline int getRoomID(){ return m_roomid; }
	crInstanceItem *dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &coordPos);//返回碰撞的item

	void clearAllInsights();
	void sendPacketToInSight(crInstanceItem *item, CRNet::crStreamPacket &packet);
protected:
	virtual ~crSightInfo();
	void removeInSightPlayer2(crSceneServerPlayerData *player);
	void roleInEyePointTest(const CRCore::crVector2i &eye,crRole *role);
	void itemInEyePointTest(const CRCore::crVector2i &eye,crInstanceItem *item);
	ItemEyeMap m_itemEyeMap;
	GNE::Mutex m_itemEyeMapMutex;
	PlayerEyeMap m_playerEyeMap;
	GNE::Mutex m_playerEyeMapMutex;
	EyePointSet m_eyePointSet;
	GNE::Mutex m_eyePointSetMutex;

	InSightRolePlayerSet m_inSightRolePlayerSet;
	GNE::Mutex m_inSightRolePlayerSetMutex;
	RolePlayerMap m_inSightRolePlayer;
	GNE::Mutex m_inSightRolePlayerMutex;
	InSightItemSet m_inSightItemSet;
	GNE::Mutex m_inSightItemSetMutex;
	ItemMap m_inSightItem;
	GNE::Mutex m_inSightItemMutex;
	OutSightItemSet m_outSightItemSet;
	GNE::Mutex m_outSightItemSetMutex;
	OutSightItemSet m_dropedItemSet;
	GNE::Mutex m_dropedItemSetMutex;
	//bool m_isRoomSight;
	int m_roomid;
};
}
#endif;