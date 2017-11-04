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
#ifndef CRNETAPP_CLIENTPLAYERDATA_H
#define CRNETAPP_CLIENTPLAYERDATA_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetGameData.h>
//#include <creature3d.h>
#include <CRNet/crNetDataManager.h>
#include <CRCore/crWchMsg.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRNetApp/appAccountData.h>
#include <CRNetApp/appNetChatData.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRUtil/crSceneView.h>
#include <CRCore/crVector2.h>
#include <CRNetApp/appSightInfo.h>
#include <map>
#include <vector>
#include <deque>

namespace CRNetApp {

class CRNETAPP_EXPORT crQueryGameListData : public CRCore::Referenced
{//
public:
	crQueryGameListData();

	void setID(int id);
	void setGameName(const std::string &gamename);
	void setCName(const std::string &cname);
	void setPlatform(const std::string &platform);
	//void setUIFile(const std::string &uifile);
	//void setClientCfg(const std::string &clientcfg);
	//void setBaseFileList(const std::string &basefilelist);
	void setRoleCount(unsigned char rolecount);

	int getID();
	const std::string& getGameName() const;
	const std::string& getCName() const;
	const std::string& getPlatform() const;
	//const std::string& getUIFile() const;
	//const std::string& getClientCfg() const;
	//const std::string& getBaseFileList() const;
	unsigned char getRoleCount();
	std::string getGameDir() const;
	std::string getScriptDir() const;
	std::string getTextDir() const;
	std::string getTableDir() const;
protected:
	int m_id;
	std::string m_gamename;
	std::string m_cname;
	std::string m_platform;
	//std::string m_uifile;
	//std::string m_clientcfg;
	//std::string m_basefilelist;
	unsigned char m_rolecount;
};
typedef std::map< int, CRCore::ref_ptr<crQueryGameListData> > GameMap;

//class CRNETAPP_EXPORT crQueryGameRoleListData : public CRCore::Referenced
//{//
//public:
//	crQueryGameRoleListData();
//
//	void setID(int id);
//	void setInstanceItemID(int instanceitemid);
//	void setMetierID(int metierid);
//	void setRoleName(const std::string &rolename);
//	void setSceneID(int sceneid);
//	void setLayerID(unsigned short layerid);
//	void setAbstractItemID(int abstractid);
//	void setStream(CRCore::crStreamBuf *stream);
//
//	int getID();
//	int getInstanceItemID();
//	int getMetierID();
//	const std::string& getRoleName() const;
//	int getSceneID();
//	unsigned short getLayerID();
//	int getAbstractItemID();
//	CRCore::crStreamBuf *getStream();
//protected:
//	int m_id;
//	int m_instanceitemid;
//	int m_metierid;
//	std::string m_rolename;
//	int m_sceneid;
//	unsigned short m_layerid;
//	int m_abstractitemid;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
typedef std::map< int, CRCore::ref_ptr<crRole> > GameRoleMap;

class CRNETAPP_EXPORT crQuerySceneInfoData : public CRCore::Referenced
{//
public:
	crQuerySceneInfoData();
	void setSceneID(int sceneid);
	void setSceneName(const std::string &scenename);
	void setFileName(const std::string &filename);
	void setUIScene(const std::string &uiscene);
	void setCollideFile(const std::string &collidefile);
	void setBrainFile(const std::string &brainfile);
	void setExternSceneType(int scenetype);

    int getSceneID();
	const std::string& getSceneName() const;
	const std::string& getFileName() const;
	const std::string& getUIScene() const;
	const std::string &getCollideFile();
	const std::string& getBrainFile();
	int getExternSceneType();
protected:
	int m_sceneid;
	std::string m_scenename;
	std::string m_filename;
	std::string m_uiscene;
	std::string m_collidefile;
	std::string m_brainfile;

	int m_externSceneType;
};
typedef std::vector< CRCore::ref_ptr<crQuerySceneInfoData> >QuerySceneList;
class CRNETAPP_EXPORT crMyPlayerData : public CRCore::Referenced
{
public:
	crMyPlayerData();
	static crMyPlayerData *getInstance();
	void setExitCode(unsigned char exitCode);
	unsigned char getExitCode();
	void clear();

	void setPlayerID(int playerid);
	int getPlayerID();
	void setUserName(const std::string &username);
	const std::string &getUserName();
	void setLastServerID(int serverid);
	int getLastServerID();
	void setWebServerID(int webserverid);
	int getWebServerID();
	void setISP(int isp);
	int getISP();
	void setServerName(const std::string &serverName);
	const std::string& getServerName();
	void setSite(const std::string &site);
	const std::string& getSite();
	//void setPlayerName(const std::string &playerName);
	//const std::string& getPlayerName(); 
	std::string getCharacterName();

	void setKey(const std::string &key);
	const std::string &getKey() const;
	//void setAccountData(crAccountData *accountData) { m_accountData = accountData; }
	//crAccountData *getAccountData() { return m_accountData.get(); }

	void setMyChatData(crNetChatData *myChatData);
	crNetChatData *getMyChatData();

	///GameList
	void addGame(crQueryGameListData *game);
	crQueryGameListData *getGame(int gameid);
	void clearGameMap();
    void lockGameMap();
    GameMap &getGameMap();
	void unlockGameMap();
	void setSelectedGame(crQueryGameListData *game);
    crQueryGameListData *getSelectedGame();

	//enum GameState
	//{
	//	None,
	//	EnterGame,
	//	EnterScene,
	//	CreateRole,
	//	EnterSceneSucess,
	//	StartGame,
	//	EndGame
	//};
	//void setGameState(unsigned int gamestate);
	//unsigned int getGameState();

	void addGameRole(crRole *role);
	crRole *getGameRole(int roleid);
	void removeGameRole(int roleid);
	void clearGameRoleMap();
	void lockGameRoleMap();
	GameRoleMap &getGameRoleMap();
	int getGameRoleCount();
	void unlockGameRoleMap();
	bool addSelectedGameRole(crRole *role);
	typedef std::map<int,CRCore::ref_ptr<crRole> >RoleMap;
	RoleMap &getSelectedGameRoles();
	void clearSelectedGameRoles();
	void removeSelectedGameRole(int roleid);
	bool setSelectedGameRole(crRole *role);
	crRole *getSelectedGameRole();
	crRole *getSelectedGameRoleByIndex(unsigned int index);
	crRole *queryFirstRole();
	crRole *queryNextRole();
	crRole *getCurrentQueryRole();
	bool hasSelectedGameRole();
	void setSceneInfoData(crQuerySceneInfoData *sceneInfo);
	crQuerySceneInfoData *getSceneInfoData();
	void createScene();
	void createScene(int sceneid, const std::string &sceneName, const std::string &fileName, const std::string &collideFile, const std::string &brainFile);
	crScene *getScene();
	crSceneLayer *getSceneLayer();
	typedef std::pair< CRCore::ref_ptr<crRole>, CRCore::ref_ptr<CREncapsulation::crNPC> > RoleNpcPair;
	typedef std::map<int,RoleNpcPair>MyRoleNpcMap;//roleid,
	void addRole(crRole *role);
	crRole *getRole(int roleid);
	void lockMyRoleNpcMap();
	MyRoleNpcMap &getMyRoleNpcMap();
	void unlockMyRoleNpcMap();
	crRole *getCurrentRole();
	crRole *getLoadedRoleByIndex(unsigned int index);
	int getLoadedRoleIndex(int roleid);
	void setCurrentRoleID(int roleid);
	int getCurrentRoleID();
	bool ifItemIsMe(crInstanceItem *item);
	bool ifNodeIsMe(CRCore::crNode *relNode);
	//void setRole(crRole *role);
	//crRole *getRole();
	//void setNPC(CREncapsulation::crNPC *npc);
	//CREncapsulation::crNPC *getNPC();

	typedef std::map< _crInt64, RoleNpcPair > InRangePlayerMap;//playerid,roleid
	//PlayerMap &getPlayerMap();
	bool findInRangePlayer(int playerid,int roleid,RoleNpcPair& roleNpcPair);
	void insertInRangePlayer(crRole *player,CREncapsulation::crNPC *npc);
	void insertInRangePlayer(RoleNpcPair &roleNpcPair);
	void removeInRangeRole(int playerid,int roleid);
	void removeInRangePlayer(int playerid);
	void lockInRangePlayerMap();
	InRangePlayerMap &getInRangePlayerMap();
	void unlockInRangePlayerMap();

	typedef std::pair< RoleNpcPair, float > PlayerTimePair;
	typedef std::multimap< _crInt64, PlayerTimePair > OutRangePlayerMap;
	bool findOutRangePlayer(int playerid,int roleid,RoleNpcPair& roleNpcPair);
	void insertOutRangePlayer(crRole *player,CREncapsulation::crNPC *npc);
	void removeOutRangePlayer(int playerid,int roleid);
	void removeOutRangePlayerInNextFrame(int playerid,int roleid);

	//typedef std::deque< CRCore::ref_ptr<crInstanceItem> > DownloadItemDeque;
	typedef std::map< short, CRCore::ref_ptr<crInstanceItem> > DownloadItemMap;
	void insertDownloadItem(short id,crInstanceItem *item);
	crInstanceItem *findDownloadItem(short id);
	void removeDownloadItem(short id);

	typedef std::pair< CRCore::ref_ptr<crItemChild>, CRCore::ref_ptr<crInstanceItem> > ItemChildPair;
	//typedef std::deque< ItemChildPair > DownloadItemChildDeque;
	typedef std::map< short, ItemChildPair > DownloadItemChildMap;
	//void lockDownloadItemChildDeque();
	//DownloadItemChildDeque& getDownloadItemChildDeque();
	//void unlockDownloadItemChildDeque();
	void insertDownloadItemChild(short id, crItemChild *item, crInstanceItem *rootitem);
	bool findDownloadItemChild(short id, ItemChildPair &itemChildPair);
	void removeDownloadItemChild(short id);

	typedef std::pair< CRCore::ref_ptr<crInstanceItem>, CRCore::ref_ptr<CREncapsulation::crNPC> > ItemNpcPair;
	typedef std::map< int, ItemNpcPair > InRangeNpcMap;
	bool findInRangeNpc(int itemid,ItemNpcPair& itemNpcPair);
	void insertInRangeNpc(crInstanceItem *item,CREncapsulation::crNPC *npc);
	void insertInRangeNpc(ItemNpcPair &itemNpcPair);
	void removeInRangeNpc(int itemid);
	//ItemNpcPair getOneInRangeNpc(crInstanceItem *item,float selectTargetRange);
	crInstanceItem *getOneInRangeEnemy(crInstanceItem *iitem, float attackrange = 0.0f);
	crInstanceItem *getOneInRangeEnemyByMouse();
	crInstanceItem *selectTargetInPointRange(const CRCore::crVector3 &point, float range, unsigned char targetType, unsigned char &outTargetType);
	unsigned char getTargetItemType(crInstanceItem *targetItem);
	void updateInPatrolEnemyMap(crInstanceItem *iitem);
	void lockInRangeNpcMap();
	InRangeNpcMap &getInRangeNpcMap();
	void unlockInRangeNpcMap();

	typedef std::pair< ItemNpcPair, float > NpcTimePair;
	typedef std::map< int, NpcTimePair > OutRangeNpcMap;
	bool findOutRangeNpc(int itemid,ItemNpcPair& itemNpcPair);
	void insertOutRangeNpc(crInstanceItem *item,CREncapsulation::crNPC *npc);
	void removeOutRangeNpc(int itemid);

	typedef std::pair< CRCore::ref_ptr<crInstanceItem>, CRCore::ref_ptr<CRCore::crNode> > ItemNodePair;
	typedef std::map< int, ItemNodePair > InRangeItemMap;
	bool findInRangeItem(int itemid,ItemNodePair& itemNodePair);
	void insertInRangeItem(crInstanceItem *item,CRCore::crNode *node);
	void insertInRangeItem(ItemNodePair &itemNodePair);
	void removeInRangeItem(int itemid);
	void lockInRangeItemMap();
	InRangeItemMap &getInRangeItemMap();
	void unlockInRangeItemMap();

	typedef std::pair< ItemNodePair, float > ItemTimePair;
	typedef std::map< int, ItemTimePair > OutRangeItemMap;
	bool findOutRangeItem(int itemid,ItemNodePair& itemNpcPair);
	void insertOutRangeItem(crInstanceItem *item,CRCore::crNode *node);
	void removeOutRangeItem(int itemid);

	void removeAllInRanges();
	void update(float dt);

	void setCurrentGameTask(crGameTask *gameTask);
	crGameTask *getCurrentGameTask();
    
	void initGameChatRec();
	void recGameChat(const std::string &str);
	void initChatMsgRec();
	void saveGameChat();
	void recChatMsg(const std::string &str);
	void saveChatMsg();

	//////开房间式网游
    QuerySceneList &getSceneList();
	void setRoomID(int roomid);
	int getRoomID();
	void insertRoom(crRoom *room);
	void lockRoomMap();
	RoomMap &getRoomMap();
	void unlockRoomMap();
	crRoom *getRoom(int roomid);
	void setSelectedRoom(crRoom *room);
	crRoom *getSelectedRoom();

	//单机游戏
	bool isItemInRange(crInstanceItem *item);
	//typedef std::map<int, std::string> TextMap;
	//void loadTextMap(const std::string &fileName);
	//bool getText(int id, std::string &text);

	//void setMapPxpm(float pxpm);
	//float getMapPxpm();
	void setMapScale(float scale);
	float getMapScale();
	void setMapScaleRange(CRCore::crVector2 &range);
	CRCore::crVector2 getMapFactor(const CRCore::crVector2 &mapWidgetSize);

	//void setCurrentDialogData(void *data);
	//void *getCurrentDialogData();
	//void setSaleDialogEmporium(bool emporium);
	//bool getSaleDialogEmporium();
	//void setITransport(bool bln);
	//void iTransportCheck();

	crInstanceItem* dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideIgnoreMode ignoreflg = IG_None);//返回碰撞的item
	bool dynamicCollideTestAll(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideItemSet& collideItemSet);//返回碰撞的item
	//crInstanceItem *dynamicCollideTest(const CRCore::crVector2 &coordPos,crInstanceItem *item);//返回碰撞的item
	//void setLoginGameSceneMode(char mode);
	//char getLoginGameSceneMode();
	void setPlayerGameData(crPlayerGameData *data);
	crPlayerGameData *getPlayerGameData();
	void rectSelectItem(const CRCore::crVector4i &rect,std::vector<crInstanceItem *> &itemvec);
	float getPosZ(float x,float y,float zoffset);
	void getEyes(std::vector<CRCore::crVector4i>&eyeVec);
	typedef std::map< CRCore::crVector2i, int >EyePointMap;//x,y,eyeheight
	void insertEyePoint(const CRCore::crVector2i &eye);
	void removeEyePoint(const CRCore::crVector2i &eye);
	void clearEyePoint();
	void sightVisiableUpdate(float dt);
	typedef std::map< CRCore::ref_ptr<crInstanceItem>,float > ItemVisiableMap;
	void insertItemVisiable(crInstanceItem *item);
	bool findItemVisiable(crInstanceItem *item);
	void itemVisiableUpdate(float dt);
	void clearItemVisiableMap();
	bool isTeammateAndShareSight(crInstanceItem *item);
	//////////收到ConnectServer
	void setConnectServerData(crQueryConnectServerData *connectServerData);
	crQueryConnectServerData *getConnectServerData();
	void clearAllInsights();
	void setLoadMainRoleCase(int _case);
	int getLoadMainRoleCase();
	typedef std::list< CRCore::ref_ptr<crInstanceItem> >RemovedItemList;
protected:
	virtual ~crMyPlayerData();
	static CRCore::ref_ptr<crMyPlayerData> m_instance;
	void checkItemVisiable(std::vector<CRCore::crVector4i> &eyeVec,crInstanceItem *item);
	RemovedItemList m_removedItemList;
	unsigned char m_exitCode;
	std::string m_username;//联运平台格式是site.username
    int m_playerID;
	int m_lastServerID;
	int m_webserverid;//联运区服ID
	int m_isp;//0自动，1电信，2联通
	std::string m_site;//联运平台标识
	std::string m_serverName;
	//std::string m_playerName;

	std::string m_key;
	//CRCore::ref_ptr<crAccountData> m_accountData;

	CRCore::ref_ptr<crNetChatData> m_myChatData;

	GameMap m_gameMap;
	CRCore::crCriticalMutex m_gameMapMutex;
	CRCore::ref_ptr<crQueryGameListData> m_selectedGame;

	GameRoleMap m_gameRoleMap;
	CRCore::crCriticalMutex m_gameRoleMapMutex;
	RoleMap m_selectedGameRoles;
	unsigned int     m_currentQueryRoleIndex;
	CRCore::ref_ptr<crQuerySceneInfoData> m_sceneInfo;
	GNE::Mutex m_myRoleNpcMapMutex;
	MyRoleNpcMap m_myRoleNpcMap;
	int m_currentRoleID;//当前控制的roleid
	//CRCore::ref_ptr<crRole> m_role;
	//CRCore::ref_ptr<CREncapsulation::crNPC> m_npc;
	CRCore::ref_ptr<crScene> m_scene;//客户端当前所在的场景

	InRangePlayerMap m_inRangePlayerMap;
	CRCore::crCriticalMutex m_inRangePlayerMapMutex;
	OutRangePlayerMap m_outRangePlayerMap;
	CRCore::crCriticalMutex m_outRangePlayerMapMutex;
	
	DownloadItemMap m_downloadItemMap;
    CRCore::crCriticalMutex m_downloadItemMapMutex;
	
	DownloadItemChildMap m_downloadItemChildMap;
	CRCore::crCriticalMutex m_downloadItemChildMapMutex;

	InRangeNpcMap m_inRangeNpcMap;
	CRCore::crCriticalMutex m_inRangeNpcMapMutex;
	OutRangeNpcMap m_outRangeNpcMap;
	CRCore::crCriticalMutex m_outRangeNpcMapMutex;
	
	InRangeItemMap m_inRangeItemMap;
	CRCore::crCriticalMutex m_inRangeItemMapMutex;
	OutRangeItemMap m_outRangeItemMap;
	CRCore::crCriticalMutex m_outRangeItemMapMutex;

	EyePointMap m_eyePointMap;
	GNE::Mutex m_eyePointMapMutex;
	ItemVisiableMap m_itemVisiableMap;
	GNE::Mutex m_itemVisiableMapMutex;

	CRCore::ref_ptr<crGameTask> m_currentGameTask;

	CRCore::ref_ptr<CRCore::crStreamBuf> m_gameChatRec;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_chatMsgRec;

	//////开房间式网游
	QuerySceneList m_sceneList;
	int m_roomid;
	RoomMap m_roomMap;
	CRCore::ref_ptr<crRoom> m_selectedRoom;
	CRCore::crCriticalMutex m_roomMapMutex;

	//TextMap m_textMap;
	//小地图
	//float m_mapPxpm;
	float m_mapscale;
	CRCore::crVector2 m_mapScaleRange;
	//
	//void *m_currentDialogData;
	//bool m_saleDialogEmporium;
	//bool m_iTransport;

	//char m_loginGameSceneMode;//0,登录成功后自动执行WCH_QueryRoleInfo 1：登录成功后由外部处理
	CRCore::ref_ptr<crPlayerGameData> m_playerGameData;
	//////////收到ConnectServer
	CRCore::ref_ptr<crQueryConnectServerData> m_connectServerData;
	int m_loadmainrolecase;//1正常登录，3角色创建
	float m_removedt;
};

class CRNETAPP_EXPORT crGameClientUpdate : public CRUtil::crSceneView::ExternUpdater
{
public:
	crGameClientUpdate();
	static crGameClientUpdate *getInstance();
	void clear();

	virtual void operator()(float dt);
protected:
	virtual ~crGameClientUpdate(){}
	static CRCore::ref_ptr<crGameClientUpdate> m_instance;
	float m_time;
	float m_interval;
};
}

#endif
