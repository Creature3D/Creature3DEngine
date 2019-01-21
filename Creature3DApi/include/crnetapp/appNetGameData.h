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
#ifndef CRNETAPP_NETGAMEDATA_H
#define CRNETAPP_NETGAMEDATA_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appGlobalHandle.h>
//#include <creature3d.h>
#include <CRNet/crNetDataManager.h>
#include <CRCore/crWchMsg.h>
#include <CRCore/crTimer.h>
#include <CRNetApp/appPathFinding.h>
#include <CRNetApp/appSightInfo.h>
#include <gnelib.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crShape.h>
#include "rcfg/ConfigScriptOO.h"
#include <map>
#include <vector>
#include <deque>
#include <string>
namespace CRDataBase {
	class crDataBase;
}
namespace CRNetApp {
#define ResClass(library,name) \
	virtual crRes* clone() const { return new name(*this); }
#define MetierClass(library,name) \
	virtual crAbstractMetier* clone() const { return new name(*this); }
#define ItemClass(library,name) \
	virtual crInstanceItem* clone() const { return new name(*this); }
#define ItemChildClass(library,name) \
	virtual crItemChild* clone() const { return new name(*this); }
class crSceneServerCallback;
///ResDB
class CRNETAPP_EXPORT crRes : public CRCore::Referenced
{
public:
	crRes();
	crRes(const crRes& res);
	ResClass(CRNetApp,crRes)
	void setResID(int id);
	int getResID();
	void setFileName(const std::string &fileName);
	const std::string &getFileName() const;
	//void setFileModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getFileModifyTime() const;
	//std::string getUpdatecfg() const;
	//void setUpdcfgModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getUpdcfgModifyTime() const;
	void setResScript(const std::string &script);
	const std::string &getResScript() const;
	//void setResScriptModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getResScriptModifyTime() const;
protected:
	virtual ~crRes();
	int m_resid;
	std::string m_filename;
	//CRCore::Timer_t m_fileModifyTime;
	//CRCore::Timer_t m_updcfgModifyTime;
	std::string m_resscript;//bot/role.cfg, Data: RpgGameBot
	//CRCore::Timer_t m_resscriptModifyTime;
};

class CRNETAPP_EXPORT crAbstractItem : public crRes
{
public:
	crAbstractItem();
	crAbstractItem(const crAbstractItem& item);
	ResClass(CRNetApp,crAbstractItem)
	void setAbstractItemID(int id);
	int getAbstractItemID();
	void setName(const std::string &name);
	const std::string &getName() const;
	void setScript(const std::string &script);
	const std::string &getScript() const;
	//void setScriptModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getScriptModifyTime() const;
protected:
	virtual ~crAbstractItem();
	int m_abstractitemid;
	std::string m_name;
	std::string m_script;//item/playerscript.cfg, Data: RpgGameRole
	//CRCore::Timer_t m_scriptModifyTime;
};

class CRNETAPP_EXPORT crAbstractMetier : public CRCore::Referenced
{
public:
	crAbstractMetier();
	crAbstractMetier(const crAbstractMetier& metier);
	MetierClass(CRNetApp,crAbstractMetier)
	void setAbstractMetierID(int id);
	int getAbstractMetierID();
	void setGameID(int gameid);
	int getGameID();
	void setMetierName(const std::string &metiername);
	const std::string &getMetierName() const;
	void setInitSceneID(int sceneid);
	int getInitSceneID();
	void setInitLayerID(unsigned short layerid);
	unsigned short getInitLayerID();
	void setZoffset(short zoffset);
	short getZoffset();
	void setMetierScript(const std::string &script);
	const std::string &getMetierScript() const;
	//void setMetierScriptModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getMetierScriptModifyTime() const;
	void setTaskTable(const std::string &file);
	std::string &getTaskTable();
	//void setTaskTableModifyTime(CRCore::Timer_t mtime);
	//CRCore::Timer_t getTaskTableModifyTime() const;
protected:
	virtual ~crAbstractMetier();
	int m_abstractmetierid;
	int m_gameid;
	std::string m_metiername;
	int m_initsceneid;
	unsigned short m_initlayerid;
	short m_zoffset;
	std::string m_metierscript;
	//CRCore::Timer_t m_metierScriptModifyTime;
	std::string m_tasktab;
	//CRCore::Timer_t m_tasktabModifyTime;
};
////InstanceDB
class crItemChild;
class crRoom;
class CRNETAPP_EXPORT crInstanceItem : public CRCore::Referenced/*: public crAbstractItem*/
{
public:
	crInstanceItem();
	crInstanceItem(const crInstanceItem& item);
	ItemClass(CRNetApp, crInstanceItem)
		void setAbstractItem(crAbstractItem *item);
	crAbstractItem *getAbstractItem();
	int getAbstractItemID();
	void setAbstractItemID(int abstractid);
	inline virtual int getID() { return getInstanceItemID(); }//itemid
	inline virtual int getRoleID() { return 0; }
	inline virtual int getPlayerID() { return 0; }
	void setInstanceItemID(int id);
	int getInstanceItemID();
	void setItemtype(unsigned char itemtype);
	unsigned char getItemtype();
	void setIName(const std::string &iname);
	const std::string &getIName() const;//获得ItemName
	const std::string &getIName2() const;//获得ItemName
	const std::string &getAName() const;//获得AbstractItemName
	void setSceneID(int sceneid);
	int getSceneID();
	void setLayerID(unsigned short layerid);
	unsigned short getLayerID();
	void setPosxy(float posx, float posy);//单位:厘米
	void setPosxy(CRCore::crVector2i& posxy);//单位:厘米
	void setPosxy(CRCore::crVector2f& posxy);//单位:厘米
	float getPosx();//单位:厘米
	//void setPosy(float posy);
	float getPosy();//单位:厘米
	void setPosz(float posz);//单位:厘米
	float getPosz();//单位:厘米
	void setZoffset(short zoffset);//单位:厘米
	short getZoffset();//单位:厘米
	void setTargetDir(const CRCore::crVector3 &dir);
	typedef std::deque<CRCore::crVector2> SyncPosDeque;
	void addSyncPos(const CRCore::crVector2 &pos);
	void clearSyncPos();
	void clearExtra();
	virtual void clearData();
	bool hasSyncPos();
	void closeToSyncPos(crScene *scene, float& distance);
	bool getFrontSyncPos(CRCore::crVector2 &pos);
	void setDir(const CRCore::crVector3 &dir);
	//void setDirx(float dirx);
	float getDirx();
	//void setDiry(float diry);
	float getDiry();
	//void setDirz(float dirz);
	float getDirz();
	CRCore::crVector3f getPosition();//单位:米
	const CRCore::crVector3f& getDir();

	typedef std::map<int, CRCore::ref_ptr<crItemChild> >ChildItemMap;
	void insertChildItem(crItemChild *item);
	void removeChildItem(int itemchildid);
	crItemChild *getChildItem(int itemchildid);
	void lockChildItemMap();
	ChildItemMap &getChildItemMap();
	void unlockChildItemMap();
	crItemChild* findTemporaryItemChild(int abstractid);

	typedef std::deque< std::pair<CRCore::ref_ptr<crItemChild>, float> > RemovedItemChildDeque;
	void insertRemovedItemChild(crItemChild *item);
	crItemChild* findRemovedItemChild(int itemid);
	virtual void clientUpdate(float dt);
	virtual void serverUpdate(float dt);
	//void clientLoadItemChild();

	void setStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();

	/////////////////////
	void setDataClass(CRCore::crData *data);
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMsg() const { return m_currentMsg; }
	CRCore::crHandle* getHandle(__int64 kbmsg, const std::string& name);
	bool hasHandle(__int64 kbmsg);
	//////////////////////////

	enum ItemType
	{
		instanceitem,
		Npc,
		Role,
		Emporium,
		LocalItem,
		//LocalNpc,
	};
	void setNode(CRCore::crNode *node);
	CRCore::crNode *getNode();
	CRCore::crNode *getRelNode();

	inline bool isLoaded() { return m_dataClass.valid(); }

	/////SceneServer
	typedef std::vector< CRCore::ref_ptr<crItemChild> > CreateItemChildVec;
	//void getCreateItemChildLock();
	void beginCreateItemChild(_crInt64 completeMsg, int count, _crInt32 completeParam = NULL);
	void recvItemChild(crItemChild *itemChild);
	void completeCreateItemChild();
	//
	void loadItemData(char streamType, bool loadchild = false);
	void childItemQuery(CRDataBase::crDataBase *instanceSession);
	crItemChild *findChildItem(int itemid);
	crItemChild *findChildItemByAbstractID(int abstractid);

	typedef std::map<int, CRCore::ref_ptr<crInstanceItem> > TemporaryItemMap;
	void insertTemporaryItem(crInstanceItem *item);
	crInstanceItem *findTemporaryItem(int itemid);
	void removeTemporaryItem(int itemid);

	crInstanceItem *serverGetTarget();
	void clientGetTarget(CRCore::ref_ptr<crInstanceItem> &outItem, CRCore::ref_ptr<CRCore::crMatrixTransform>&outNode);

	//开房间式网游
	void setRoomID(int roomid);
	int getRoomID();
	crRoom *getRoom();
	CRNetApp::crScene *getScene();
	void sendPacketToItemNeighbor(CRNet::crStreamPacket &packet);
	void sendPacketToAll(CRNet::crStreamPacket &packet);
	void sendPacketToInSight(CRNet::crStreamPacket &packet);

	void clientRelease();
	//void clientReload();
	void clientLoadItemChildNode(crInstanceItem *rootItem);
	struct WayPoint
	{
		WayPoint():m_msg(NULL),m_param(NULL){}
		int m_sceneid;
		unsigned short m_layerid;
		CRCore::crVector3 m_coord;
		_crInt64 m_msg;
		_crInt64 m_param;
	};
	class CRNETAPP_EXPORT crPathFinder: public CRCore::Referenced
	{
	public:
		crPathFinder():m_pathStatus(NotStarted),/*m_waittime(0.0f),m_lastwaittime(0.0f),*/m_dynamicwait(0.0f),m_dynamicblocktime(0.0f)/*,m_dynamicTryCount(0)*/
		{ m_dynamicPathFinding = new crDynamicPathFinding; }
		enum PathStatus
		{
			NotStarted,
			Found,
			ReFound,
			Lapover,
			NonExistent,
			OutMap,
		};
		inline void setPathStatus(unsigned char status){ m_pathStatus = status; }
		inline unsigned char getPathStatus(){ return m_pathStatus; }
		bool findTargetInPath(CRCore::crVector2s target);
		void setBestPath(CoordDeque &bestPath);
		//CoordDeque &getBestPath();
		bool isBestPathEmpty();
		bool readPath(crInstanceItem *item,crScene *scene,float& distance,float dt);
		bool dynamicChangePath(crInstanceItem *item,crScene *scene);
		void dirty();//切换场景或者切换层时触发
		void refind();
		//事件路径点
		typedef std::deque<WayPoint> WaypointDeque;
		void setWaypointDeque(WaypointDeque &waypointDeque);
		void addWaypoint(WayPoint point);
		void addFrontWaypoint(WayPoint point);
		void clearWaypointDeque();
		WayPoint getFrontWaypoint();
		void popFrontWaypoint();
		bool isWaypointEmpty();
		WaypointDeque &getWaypointDeque();
		//区域寻路
		void setPortPathDeque(const PortDeque &portPath);
		void clearPortPathDeque();
		CRCore::crVector2f getFrontPortPath();
		void popFrontPortPath();
		bool isPortPathEmpty();
		int getPortPathSize();
	protected:
		void nextWaypoint(crInstanceItem *item,crScene *scene,bool refind = false);
		void nextPortPath(crInstanceItem *item,crScene *scene,bool refind = false);
		unsigned char m_pathStatus;
		GNE::Mutex m_pathMutex;
		CoordDeque m_bestPath;
		GNE::Mutex m_waypointMutex;
		WaypointDeque m_waypointDeque;
		GNE::Mutex m_portpathMutex;
		PortDeque m_portPathDeque;
		CRCore::ref_ptr<crDynamicPathFinding> m_dynamicPathFinding;
		//float m_waittime;
		//float m_lastwaittime;
		float m_dynamicwait;
		float m_dynamicblocktime;
		//char m_dynamicTryCount;
	};
	crPathFinder *getOrCreatePathFinder();
	void setPathFinder(crPathFinder *pathFinder);
	crPathFinder *getPathFinder();
	void recordItemState();
	bool getBackItemState();
	void setTransportSceneID(int sceneid);
	int getTransportSceneID();

	//typedef std::set<CRCore::crVector2s> ItemCoordSet;//coord
	bool testNewPos(crScene *scene,const CRCore::crVector2 &newPos,CRCore::crVector3 dir,float &height);
	//void readPathCollideUpdate(crScene *scene);
	bool dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos);//返回true表示碰撞，返回false表示没有碰撞
	//bool dynamicCollideTest(const CRCore::crVector2 &coordPos,crInstanceItem *item);//返回true表示碰撞，返回false表示没有碰撞
	float getMinDistance(crInstanceItem *item = NULL);
	float getAttackDistance(crInstanceItem *item);
	bool isCoordInItem(const CRCore::crVector2 &coordPos);
	typedef std::vector< CRCore::ref_ptr<CRCore::crNode> > SceneFxNodeArray;
	void addSceneFxNode(CRCore::crNode *node);
	void lockSceneFxArray();
	SceneFxNodeArray &getSceneFxNodeArray();
	void unlockSceneFxArray();
	CRCore::crNode *findSceneFxNode(const std::string &name);
	inline void setDelayTime(float delay){ m_delayTime = delay; }
	inline float getDelayTime(){ return m_delayTime; }//s
	void setSightInfo(crSightInfo *sight);
	inline crSightInfo *getSightInfo(){return m_sightInfo.get();}
	bool isMainAI();
	//CRNetApp::crScene *getScene();
	CRNetApp::crSceneLayer *getSceneLayer();
	float getPosZ(float posx,float posy);//posx,posy,返回值均为米
	void setDropItemTimer(float interval);
	void setOwnerID(_crInt64 ownerid);
	_crInt64 getOwnerID();
	int getOwnerItemID() { return LOINT64(m_ownerid); }
	int getOwnerRoleID() { return HIINT64(m_ownerid); }
protected:
	virtual ~crInstanceItem();
	void closeToTargetDir(float dt);
	void removedItemChildUpdate(float dt);
	void loadChildItemData(char streamType,bool loadchild = false);
	CRCore::ref_ptr<crAbstractItem> m_abstractItem;
	//void clientReloadItemChild(crInstanceItem *rootItem);
	int m_instanceitemid;
	unsigned char m_itemtype;//0:instanceitem 1:crNpc 2:crRole
	std::string m_iname;
	int m_sceneid;
	unsigned short m_layerid;
	float m_posx;//realpos = m_pos * crGlobalHandle::gData()->gUnitScale()//单位:厘米
	float m_posy;//单位:厘米
	float m_posz;//单位:厘米
	short m_zoffset;//(身高*0.5+0.15)/crGlobalHandle::gData()->gUnitScale()  realZoffset = m_zoffset*crGlobalHandle::gData()->gUnitScale()//单位:厘米
	//初始值（0,-1,0）
	CRCore::crVector3 m_dir;
	CRCore::crVector3 m_targetDir;
	GNE::Mutex m_syncPosDequeMutex;
	SyncPosDeque m_syncPosDeque;
	float m_adjustDirTime;
	float m_adjustRadian;
	CRCore::crVector3 m_adjustUp;
	ChildItemMap m_childItemMap;
	GNE::Mutex m_childItemMapMutex;
	RemovedItemChildDeque m_removedItemChildDeque;
	GNE::Mutex m_removedItemChildDequeMutex;
	//存数据库role data param bot data param
	//格式：
	//stream->_writeInt(id);
	//stream->_writeXXX(param);
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	///////////////////
	CRCore::ref_ptr<CRCore::crData> m_dataClass;
	_crInt64 m_currentMsg;
	//////////////////////
	CRCore::ref_ptr<CRCore::crNode> m_node;
	///////SceneServer CreateItemChild
	//CRCore::crMutex m_createItemChildMutex;
	int m_createCount;
	_crInt64 m_completeMsg;
	_crInt32 m_completeParam;
	bool m_begin;
    CreateItemChildVec m_createItemChildVec;

	TemporaryItemMap m_temporaryItemMap;
    GNE::Mutex m_temporaryItemMapMutex;
	//GNE::Mutex m_eventMutex;

	//开房间式网游的房间ID
	int m_roomid;
	crRoom *m_room;
	crScene *m_scene;

	CRCore::ref_ptr< crPathFinder > m_pathFinder;

	unsigned char m_itemstate_record;
	int m_transportsceneid;
	GNE::Mutex m_sceneFxNodeArrayMutex;
	SceneFxNodeArray m_sceneFxNodeArray;
	float m_delayTime;
	CRCore::ref_ptr<crSightInfo> m_sightInfo;
	float m_dropItemTimer;//掉落物品计时器
	_crInt64 m_ownerid;//宿主ID，(id,roleid)
	float m_reliveTimer;
	//ItemCoordSet m_itemCoordSet;
	//GNE::Mutex m_itemCoordSetMutex;
};

class CRNETAPP_EXPORT crItemChild : public CRCore::Referenced
{
public:
	crItemChild();
	crItemChild(const crItemChild& itemchild);
	ItemChildClass(CRNetApp,crItemChild)
	void setItemChildID(int id);
	int getItemChildID();
	void setInstanceItemID(int instanceitemid);
	int getInstanceItemID();
	void setInstanceItem(crInstanceItem *item);
	crInstanceItem *getInstanceItem();
	void setParentItem(crInstanceItem *item);
	crInstanceItem *getParentItem();
	inline bool isLoaded(){ return m_instanceItem.valid() && m_instanceItem->isLoaded(); }
	virtual void clear();
protected:
	virtual ~crItemChild();
	int m_itemchildid;
	int m_instanceitemid;
	CRCore::ref_ptr<crInstanceItem> m_instanceItem;
	crInstanceItem *m_parentItem;
};
class CRNETAPP_EXPORT crArea : public CRCore::Referenced
{
friend class crAreaPathFinding;
public:
	crArea(float area):m_areaValue(area){}
	float getAreaValue(){ return m_areaValue; }
	typedef std::map< CRCore::crVector2f,CRCore::ref_ptr<crArea> > AreaPortMap;
	bool insertAreaPort(const CRCore::crVector2f &port, crArea *area);
	static void beginSearch();
	crArea* searchArea(float area);
	void distillPath(std::stack<crArea *>&path,PortDeque&portPath);
	bool findAreaPort(crArea *connectArea,CRCore::crVector2f &port);
	bool getBestPort(crArea *area2,const CRCore::crVector2f &pos,CRCore::crVector2f &bestport,float& mindist);
protected:
	float m_areaValue;
	AreaPortMap m_areaPortMap;
	static std::set<crArea *> s_searchset;
};
class CRNETAPP_EXPORT crWaypoint : public CRCore::Referenced
{
	friend class crWaypointPathFinding;
public:
	crWaypoint(){}
	void setPoint(const CRCore::crVector2s& point){ m_point = point; }
	const CRCore::crVector2s& getPoint(){ return m_point; }
	void connectWaypoint(crWaypoint *waypoint);
	void disconnectWaypoint(crWaypoint *waypoint);
	void disconnectAll();
	crWaypoint *findWaypoint(const CRCore::crVector2s &point);
	static void resetNearest();
	crWaypoint *searchNearest(const CRCore::crVector2s &point,float range = 2.0f);
	static crWaypoint *getNearest();
	typedef std::map< CRCore::ref_ptr<crWaypoint>,int >LinkMap;
	void write(CRCore::crStreamBuf *stream);
	void read(CRCore::crStreamBuf *stream);
	static void endtravel();
	static crWaypoint *beginread(CRCore::crStreamBuf *stream);
	static void endread();
	void drawlink(CRCore::Vec3Array *coords,float zoffset);
	static void enddraw();
	typedef std::set< CRCore::ref_ptr<crWaypoint> > WaypointSet;
	void removeRedundantWaypoint(WaypointSet &waypointset);
	int getLinkDist(crWaypoint *waypoint);
protected:
	static crWaypoint *readWaypoint(CRCore::crStreamBuf *stream);
	CRCore::crVector2s m_point;
	LinkMap m_linkmap;
	static WaypointSet s_travelset;
	static WaypointSet s_drawset;
	static std::map< CRCore::crVector2s,CRCore::ref_ptr<crWaypoint> > s_readmap;
	static CRCore::ref_ptr<crWaypoint> m_neareastWaypoint;
	static float m_nearestDist;
};
class crScene;
class CRNETAPP_EXPORT crSceneLayer : public CRCore::Referenced
{
friend class crPathFindingManager;
public:
	crSceneLayer(crScene *parent);
	crScene *getParentScene(){ return m_parentScene; }
	typedef std::vector<CRCore::crVector3i> BirthPointArray;//x,y,r
	//inline void setHeightField(CRCore::crHeightField *heightField){ m_heightField = heightField; }
	//inline CRCore::crHeightField *getHeightField(){ return m_heightField.get(); }
	typedef std::vector< CRCore::ref_ptr<CRCore::crImage> > CollideMapVec;
	inline void addCollideMap(CRCore::crImage *collidemap){ m_collideMapVec.push_back(collidemap); }
	inline CRCore::crImage *getCollideMap(int id = 0){ return m_collideMapVec[id].get(); }
	void setFirstArea(crArea *area){ m_firstArea = area; }
	float getAreaValue(const CRCore::crVector2s &coord);
	//inline const CRCore::crImage *getCollideMap() const { return m_collideMap.get(); }
	inline void setOrigin(CRCore::crVector2 &origin) { m_origin = origin; }
	inline const CRCore::crVector2& getOrigin() const { return m_origin; }
	inline void setMaxHeight(float maxHeight){ m_maxHeight = maxHeight; }
	inline float getMaxHeight() const { return m_maxHeight; }
	inline void setMapSize(float width, float length){ m_dx = width / (float)(m_collideMapVec[0]->s()-1); m_dy = length / (float)(m_collideMapVec[0]->t()-1); m_dlength = CRCore::crVector2(m_dx,m_dy).length(); }
	inline float getXInterval() const { return m_dx; }
	inline float getYInterval() const { return m_dy; }
	inline float getIntervalLength() const { return m_dlength; }
	inline short getNumColumns() const { return m_collideMapVec[0]->s(); }//mapWidth
	inline short getNumRows() const { return m_collideMapVec[0]->t(); }//mapHeight
	float getHeight(short x, short y);
	CRCore::crVector2 getCollideInfo(short x, short y) const;
	CRCore::crVector2 getCollideInfo(const CRCore::crVector2s& coord) const;
	CRCore::crVector2 getCollideInfoEx(short x, short y, float z) const;
	CRCore::crVector2 getCollideInfoEx(const CRCore::crVector2s& coord, float z) const;
	inline void addBirthPoint(CRCore::crVector3i& point){ m_birthPointArray.push_back(point); }
	inline const BirthPointArray &getBirthPointArray() const{ return m_birthPointArray; }
	bool isInBirthPoint(const CRCore::crVector2 &pos);
	CRCore::crVector2s getCoord(float posx,float posy) const;
	CRCore::crVector2s getCoord(CRCore::crVector2 pos) const;
	CRCore::crVector2 getPosXY(const CRCore::crVector2s& coord) const;
	//typedef std::vector<CRCore::crVector2s> PointVec;
	//typedef std::vector< CRCore::ref_ptr<crNaviPath> >NaviPathList;//任务点间路径列表
	//void addPoint(const CRCore::crVector2s &point);
	//void buildNaviPathList();
	//const CRCore::crVector2s& findNearstPoint(const CRCore::crVector2s &mypoint);//寻找离开我最近的任务点，寻路到该任务点，然后查找该任务点到目标点的路径
	//														//并非直线距离最近，需要通过寻路器验证
	//crNaviPath *findNaviPath(const CRCore::crVector2i& from, const CRCore::crVector2i &to);
	bool walkability(short x,short y) const;
	bool walkability(float info0)const;
	unsigned char getMapProtected(short x, short y) const;
	unsigned char getMapProtected(float info0)const;
	void setTransportCoord(const CRCore::crVector2 &coord);
	const CRCore::crVector2 &getTransportCoord()const;

	//typedef std::map< CRCore::crVector2s,CRCore::ref_ptr<crInstanceItem> > ItemCollideMap;//coord,item
	//void updateItemCollideMap(crInstanceItem *collideitem);//
	//void removeCollideItem(crInstanceItem *collideitem);
	//crInstanceItem* dynamicCollideTest(const CRCore::crVector2s &coord);//返回碰撞的item
	//void setLayerHeight(float layerHeight);
	//float getLayerHeight() const;
	//typedef std::set< CRCore::ref_ptr<crInstanceItem> > CollideItemSet;
	//void insertCollideItem(crInstanceItem *collideItem);
	//void removeCollideItem(crInstanceItem *collideItem);
	//crInstanceItem *dynamicCollideTest(const CRCore::crVector2s &coord);//返回碰撞的item
	void insertAreaPort(const CRCore::crVector2 &coord);
	bool isInAreaPort(const CRCore::crVector2 &coord);
	void setWaypointRoot(crWaypoint *waypoint);
	crWaypoint *getWaypointRoot();
protected:
	virtual ~crSceneLayer(){}
	crScene *m_parentScene;
	bool findAreaPorts(const CRCore::crVector2s &from, const CRCore::crVector2s &to, PortDeque&portPath,crAreaPathFinding *areaPathFinding);
	//CRCore::ref_ptr<CRCore::crHeightField> m_heightField;
	//CRCore::ref_ptr<CRCore::crImage> m_collideMap;
	CollideMapVec m_collideMapVec;
	CRCore::ref_ptr<crArea> m_firstArea;
	CRCore::ref_ptr<crWaypoint> m_waypointRoot;
	typedef std::vector<CRCore::crVector2> AreaPortVec;
	AreaPortVec m_areaPortVec;
	CRCore::crVector2       m_origin;
	//float			m_layerHeight;
	float			m_maxHeight;
	float	        m_dx;
	float	        m_dy;
	float           m_dlength;
	BirthPointArray m_birthPointArray;
	//PointVec        m_taskPointVec;//任务点
	//NaviPathList m_naviPathList;
	CRCore::crVector2 m_transportCoord;
	//ItemCollideMap m_itemCollideMap;
	//GNE::Mutex m_itemCollideMapMutex;
	//CollideItemSet m_collideItemSet;
	//GNE::Mutex m_collideItemSetMutex;
};
typedef std::set< CRCore::ref_ptr<crInstanceItem> >CollideItemSet;
class CRNETAPP_EXPORT crScene : public CRCore::Referenced
{
public:
	crScene();
	void setSceneID(int id);
	int getSceneID();
	void setGameID(int id);
	int getGameID();
	void setGameName(const std::string &gamename);
	const std::string &getGameName() const;
	void setSceneName(const std::string &scenename);
	const std::string &getSceneName() const;
	void setFileName(const std::string &filename);
	const std::string &getFileName() const;
	//std::string getUpdatecfg() const;
	void setCollideFile(const std::string &collidefile);
	const std::string& getCollideFile();
	void setBrainFile(const std::string &brainfile);
	const std::string& getBrainFile();

	//static std::string getUpdatecfg(const std::string &filename);
	static std::string getCollideFile(const std::string &filename);
	static std::string getBrainFile(const std::string &filename);

	void buildSceneLayer();
	float getPosZ(unsigned short layerid, float posx,float posy, float zoffset);//posx,posy,zoffset,返回值均以米为单位

	typedef std::map<int, CRCore::ref_ptr<crInstanceItem> > SceneItemMap;//itemid, crInstanceItem
	void insertSceneItem(crInstanceItem *item);
	crInstanceItem *findSceneItem(int itemid,int roomid = 0);
	void lockSceneItemMap();
	SceneItemMap &getSceneItemMap();
	void unlockSceneItemMap();

	typedef std::map< _crInt64,CRCore::ref_ptr<crRole> > SceneRoleMap;//playerid,roleid（对于来自不同数据库的roleid有可能冲突所以这里要加上playerid）
	typedef std::map< int,SceneItemMap > RoomItemMap;//roomid
	typedef std::map< int,SceneRoleMap > RoomRoleMap;//roomid
	typedef std::set< CRCore::ref_ptr<crSightInfo> > SightInfoSet;
	void insertRoomItem(crInstanceItem *item);
	void removeRoomItem(crInstanceItem *item);
	crInstanceItem *findRoomItem(int roomid, int itemid);
	void lockRoomItemMap();
	RoomItemMap &getRoomItemMap();
	void unlockRoomItemMap();

	void insertRoomRole(crRole *role);
	void removeRoomRole(crRole *role);
	crRole *findRoomRole(int roomid,int playerid,int roleid);
	void lockRoomRoleMap();
	RoomRoleMap &getRoomRoleMap();
	void unlockRoomRoleMap();

	void insertSightInfo(crSightInfo *sightInfo);
	void removeSightInfo(crSightInfo *sightinfo);
	//void sendPacketToItemNeighbor(crInstanceItem *item,CRNet::crStreamPacket &packet);//item可以是role
	void itemDead(crInstanceItem *item);//可以是role
	void itemRelive(crInstanceItem *item);//可以是role

	typedef std::vector< CRCore::ref_ptr<crSceneLayer> >LayerVec;
	crSceneLayer *getSceneLayer(unsigned short layerid);
	int getBirthPointCount(unsigned short layerid);
	void gainBirthPoint(crInstanceItem *item,/*unsigned short layerid,float zoffset,*/int index = -1);//index = -1表示随机

	//typedef std::pair<int,float>RecycleIDPair;
	//typedef std::deque<RecycleIDPair> RecycleIDDeque;
 //   int gainTemporaryItemID();
 //   void recycleItemID(int id,float delay = crGlobalHandle::gData()->gRecycleIDTime());
	//int gainTemporaryItemChildID();
	//void recycleItemChildID(int id,float delay = crGlobalHandle::gData()->gRecycleIDTime());
	typedef std::map< CRCore::ref_ptr<crInstanceItem>,float > RemovedItemMap;
	void wantToRemoveItem(crInstanceItem *item);
	void serverUpdate(float dt,crSceneServerCallback *sc);//server update
	void clientUpdate(float dt);
	crPathFindingManager *getPathFindingManager();
	static void clearCollideMap();
	void setEmporium(crInstanceItem *item);
	crInstanceItem *getEmporium();
	crInstanceItem *dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos);//返回碰撞的item
	crInstanceItem *dynamicCollideTest(const CRCore::crVector2 &coordPos,crInstanceItem *item,crSceneLayer *layer,CollideIgnoreMode ignoreFriends = IG_None);//返回碰撞的item
	bool dynamicCollideTestAll(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideItemSet &collideItemSet);//返回碰撞的item
	bool dynamicCollideTestAll(const CRCore::crVector2 &coordPos,crInstanceItem *item,crSceneLayer *layer,CollideItemSet &collideItemSet);//返回碰撞的item
	CRCore::crGroup *getAreaDisplayNode();
	bool findNonblockPos(crInstanceItem *item,const CRCore::crVector2 &targetpos,float searchRange,CRCore::crVector3 &validPos,CollideIgnoreMode ignoreFriends = IG_None);
	bool findWalkablePos(crInstanceItem *item,const CRCore::crVector2 &targetpos,float searchRange,CRCore::crVector3 &validPos);
	inline void setExternSceneType(int scenetype){ m_externSceneType = scenetype; }
	inline int getExternSceneType(){ return m_externSceneType; }
	inline void setMainRoomID(int roomid){ m_mainroomid = roomid; }
	inline int getMainRoomID() { return m_mainroomid; }
protected:
	virtual ~crScene();
	void sightRemoveRole(crRole *role);
	void removeSceneItem(crInstanceItem *item);//只限定serverUpdate调用
	void updateRemovedItemMap(float dt);
	int m_sceneid;
	int m_gameid;
	int m_mainroomid;
	std::string m_gamename;
	std::string m_scenename;
	std::string m_filename;
	std::string m_collidefile;
	std::string m_brainfile;

	SceneItemMap m_sceneItemMap;
	GNE::Mutex m_sceneItemMutex;
	RemovedItemMap m_removedItemMap;
	GNE::Mutex m_removedItemMapMutex;

	RoomItemMap m_roomItemMap;
	GNE::Mutex m_roomItemMutex;//m_roomItemMutex->roomdatalock
	RoomRoleMap m_roomRoleMap;
	GNE::Mutex m_roomRoleMutex;//m_roomRoleMutex->roomdatalock
	SightInfoSet m_sightInfoSet;
	GNE::Mutex m_sightInfoSetMutex;

	//static RecycleIDDeque s_recycleItemIDDeque;
	//static GNE::Mutex s_recycleItemIDDequeMutex;
	//static int s_temporaryItemID;

	//static RecycleIDDeque s_recycleItemChildIDDeque;
	//static GNE::Mutex s_recycleItemChildIDDequeMutex;
	//static int s_temporaryItemChildID;

	LayerVec m_layers;
	typedef std::map< std::string,CRCore::ref_ptr<CRCore::crImage> > ImageMap;
	static ImageMap s_collideMap;
	static CRCore::crImage *findCollideMap(const std::string &name);
	static void insertCollideMap(const std::string &name, CRCore::crImage *image);
	
	crPathFindingManager* m_pathFindingManager;
	CRCore::ref_ptr<crInstanceItem> m_emporium;
	CRCore::ref_ptr<CRCore::crGroup> m_areaDisplay;

	int m_externSceneType;//0:默认，1:不保存FormationInfoVec
};
///GameDB
///crPlayerGameData
class crRole;
class crInstanceItem;
class CRNETAPP_EXPORT crPlayerGameData : public CRCore::Referenced
{
public:
	crPlayerGameData();
	void setID(int id);
	int getID();
	void setPlayerID(int id);
	int getPlayerID();
	void setStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();

	/////////////////////
	void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMsg() const { return m_currentMsg; }
	CRCore::crHandle* getHandle(__int64 kbmsg, const std::string& name);
	bool hasHandle(__int64 kbmsg);
	//////////////////////////
	//gameserver
	void setDBValid(bool dbvalid);
	bool getDBValid();
	void setMainRoleID(int roleid);
	int getMainRoleID();
	void setMainRole(crRole *role);
	crRole *getMainRole();
	std::string getCharacterName();
	virtual void clearData();
	void setAdvertid(const std::string &advertid);
	const std::string &getAdvertid();
	typedef std::set< CRCore::ref_ptr<crInstanceItem> > InCameraItemSet;
	void clearInCameraItemSet() 
	{ 
		GNE::LockMutex lock(m_inCameraItemSetMutex);
		m_inCameraItemSet.clear(); 
	}
	void insertInCameraItem(crInstanceItem *item)
	{
		GNE::LockMutex lock(m_inCameraItemSetMutex);
		m_inCameraItemSet.insert(item); 
	}
	bool isInCamera(crInstanceItem *item) 
	{ 
		GNE::LockMutex lock(m_inCameraItemSetMutex);
		return m_inCameraItemSet.find(item) != m_inCameraItemSet.end(); 
	}
protected:
	virtual ~crPlayerGameData();
	int m_id;
	bool m_dbvalid;
	int m_playerid;
	std::string m_advert_id;//渠道ID,只存在于gameserver
	//std::string m_script;"gamename/playergamedata.cfg"
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	///////////////////
	CRCore::ref_ptr<CRCore::crData> m_dataClass;
	_crInt64 m_currentMsg;
	//////////////////////
	int m_mainRoleID;
	CRCore::ref_ptr<crRole> m_mainRole;
	GNE::Mutex m_inCameraItemSetMutex;
	InCameraItemSet m_inCameraItemSet;
};
class CRNETAPP_EXPORT crRole : public crInstanceItem
{
public:
	crRole();
	crRole(const crRole& item);
	ItemClass(CRNetApp,crRole)
	inline virtual int getID(){ return m_playerid; }//itemid
	inline virtual int getRoleID() { return m_roleid; }
	void setRoleID(int id);
	void setPlayerID(int playerid);
	inline virtual int getPlayerID(){ return m_playerid; } 

	void setAbstractMetier(crAbstractMetier *metier);
	crAbstractMetier *getAbstractMetier();
	void setMetierStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getMetierStream();

	/////////////////////
	void setMetierDataClass( CRCore::crData *data );
	CRCore::crData *getMetierDataClass();
	virtual void doMetierEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMetierMsg() const { return m_currentMetierMsg; }
	CRCore::crHandle* getMetierHandle(__int64 kbmsg, const std::string& name);
	bool hasMetierHandle(__int64 kbmsg);
	//////////////////////////
	virtual void loadMetierData(char streamType);

	void setPlayerGameData(crPlayerGameData *data);//注意：mainrole不设
	crPlayerGameData *getPlayerGameData();//注意：mainrole这里取到的是NULL
	virtual void clearData();
	//virtual void serverUpdate(float dt);
protected:
	virtual ~crRole();
	int m_roleid;
	int m_playerid;

	CRCore::ref_ptr<crAbstractMetier> m_abstractMetier;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_metierStream;
	///////////////////
	CRCore::ref_ptr<CRCore::crData> m_metierDataClass;
	_crInt64 m_currentMetierMsg;
	//////////////////////
	CRCore::ref_ptr<crPlayerGameData> m_playerGameData;
};
/////////////开房间式网游
class CRNETAPP_EXPORT crRoomPlayer : public CRCore::Referenced
{
public:
	crRoomPlayer();
	void setPlayerID(int playerid);
	int getPlayerID();
	void setCharacterName(const std::string &characterName);
	const std::string &getCharacterName();
	void setGroupID(unsigned char groupid);
	unsigned char getGroupID();
	void setPlayerGameData(crPlayerGameData *data);
	crPlayerGameData *getPlayerGameData();
	void getStream(CRCore::ref_ptr<CRCore::crStreamBuf> &stream);
	void setStream(CRCore::crStreamBuf *stream);
	void setBirthPointIndex(char index);
	char getBirthPointIndex();
	void setReady(bool ready);
	bool getReady();
	bool isInCamera(crInstanceItem *item) { return m_playerGameData->isInCamera(item); }
protected:
	virtual ~crRoomPlayer(){}
	int m_playerid;
	std::string m_characterName;
	unsigned char m_groupid;//0中立（不可攻击,观察者） 1混战（所有人都可以对其攻击） 2组队ID（从2开始后面都是组队ID，2为NPC队伍，不同组队间可以攻击）
	unsigned char m_level;
	char m_birthPointIndex;
	bool m_ready;
	CRCore::ref_ptr<crPlayerGameData> m_playerGameData;
};
class CRNETAPP_EXPORT crRoom : public CRCore::Referenced
{
public:
	friend class crInstanceItem;
	crRoom(crScene *scene);
	crScene *getScene();
	int getSceneID();
	enum GameMode
	{
		Melee,
		TwoGroup,
		CTF,
		Dota,
		ThreeGroup,
		FourGroup,
		Extern,
		GameModeCount
	};
	void setGameMode(char gameMode);
	unsigned char getGameMode();
	void setRoomID(int id);
	int getRoomID();
	void setName(const std::string &name);
	const std::string &getName() const;
	void setPassword(const std::string &password);
	const std::string &getPassword() const;
	int getGameTime();
	bool getFriendlyFire();
	void setMaxPlayerCount(short count);
	short getMaxPlayerCount();
	void setMinPlayerCount(short count);
	short getMinPlayerCount();
	void setShareSight(bool shareSight);
	bool getShareSight();

	void assignGroupID(crRoomPlayer *roomplayer);
	int isEnemy(crInstanceItem *thisItem,crInstanceItem *targetItem);//0中立不可攻击，1队友，-1敌人
	typedef std::list< CRCore::ref_ptr<crRoomPlayer> > PlayerList;
	bool addMember(crRoomPlayer *roomPlayer/*,bool send*/);
	void removeMember(int playerid,bool send);
	crRoomPlayer *getMember(int playerid);
	int getMemberIndex(int playerid);
	void lockPlayerList();
	PlayerList &getPlayerList();
	void unlockPlayerList();
	int getPlayerCount();
	void setRoomHost(int playerid);
	int getRoomHost();
	void setGameRunning(bool gameRunning);
	bool getGameRunning();
	bool isCanJoin();
	void clear();

	void serverUpdate(float dt,crSceneServerCallback *sc);
	//void setRemainTime(float time);
	float& timeRemain(){ return m_timeRemain; }
	void sendPacketToAll(CRNet::crStreamPacket &packet,int exceptPlayerid = 0);
	void sendPacketToGroup(unsigned char groupid, CRNet::crStreamPacket &packet,int exceptPlayerid = 0);
    ////SceneServer
	void startGame(crRoomPlayer *roomplayer = NULL);
	int getBirthPointIndex(crRoomPlayer *roomplayer);

	void setCreateRoomData(CRCore::crStreamBuf *stream);
	void getRoomData(CRCore::ref_ptr<CRCore::crStreamBuf> &stream);
	void setRoomData(CRCore::crStreamBuf *stream);
	bool allMemberIsReady();
	void playerJoined(crRoomPlayer *roomPlayer);
	void setReady(crRoomPlayer *roomPlayer);
	typedef std::map< unsigned char,CRCore::ref_ptr<crSightInfo> > SightInfoMap;
	typedef std::set< CRCore::ref_ptr<crSightInfo> > SightInfoSet;
	//void lockSightInfoMap();
	crSightInfo* getOrCreateSightInfo(unsigned char groupid/*,bool npcshare = false*/);
	void removeSightInfo(crSightInfo *sightinfo);
	void removeRoom();
	//void unlockSightInfoMap();
	void setAutoStart(bool autostart,float autostarttime);

	/////////////////////
	virtual void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMsg() const { return m_currentMsg; }
	bool hasHandle(_crInt64 kbmsg);
	void clearData();
	//////////////////////////
	void setRemoveTime(float removetime);
	bool getCanRemove();
	void pauseRoom(float time);//暂停多少s,0.0f表示取消暂停
	bool isPaused();
	struct RoomMessage : public CRCore::Referenced
	{
		RoomMessage(CRNet::crStreamPacket *packet, unsigned char groupid, int exceptPlayerid)
		{
			m_packet = packet;
			m_groupid = groupid;
			m_exceptPlayerid = exceptPlayerid;
		}
		CRCore::ref_ptr<CRNet::crStreamPacket> m_packet;
		unsigned char m_groupid;//0表示All
		int m_exceptPlayerid;
	};
	typedef std::deque< CRCore::ref_ptr<RoomMessage> >RoomMessageDeque;
	void sendRoomMessage(CRNet::crStreamPacket *packet, unsigned char groupid = 0, int exceptPlayerid = 0);
protected:
	virtual ~crRoom();
	void sendStartGame(CRNet::crNetManager *netManager,CRNet::crNetDataManager *netDataManager,crRoomPlayer *roomPlayer);
	void roomMessageDispose();
	void sendPacketToItemNeighbor(crInstanceItem *item, CRNet::crStreamPacket &packet);
	void sendPacketToInSight(crInstanceItem *item, CRNet::crStreamPacket &packet);
	int m_sceneid;
	CRCore::ref_ptr<crScene> m_scene;
	char m_gameMode;
	int m_roomid;
	std::string m_name;
	std::string m_password;
	short m_maxPlayerCount;//人数上限,-1表示无上限
	short m_minPlayerCount;//最小开启人数
	PlayerList m_playerList;
	GNE::Mutex m_playerListMutex;
	int m_roomHost;
	bool m_gameRunning;//游戏已经开始，不能再加入玩家
	int m_gameTime;//-1表示无穷
	float m_timeRemain;
	bool m_friendlyFire;//同伴伤害
	bool m_shareSight;//共享视野
	unsigned char m_lastAssigndGroupid;
	bool m_autoStart;
	float m_autoStartTime;
	GNE::Mutex m_sightInfoMapMutex;
	SightInfoMap m_sightInfoMap;
	GNE::Mutex m_sightInfoSetMutex;//不共享视野
	SightInfoSet m_sightInfoSet;
	///////////////////
	CRCore::ref_ptr<CRCore::crData> m_dataClass;
	_crInt64 m_currentMsg;
	//////////////////////
	float m_removeTime;//当room结束后触发计时
	bool m_canRemove;
	float m_pauseTimer;
	GNE::Mutex m_roomMessageDequeMutex;
	RoomMessageDeque m_roomMessageDeque;
};
typedef std::map< int, CRCore::ref_ptr<crRoom>, std::greater<int> > RoomMap;//roomid, crRoom
//////////////////////////////////////////////////////////////////////////
//typedef std::pair<crInstanceItem*,crInstanceItem*> UseItemPair;
//typedef std::pair< CRCore::ref_ptr<CRCore::crGroup>,std::pair<CRCore::ref_ptr<CRCore::crNode>,CRCore::crVector3> > ItemUsePair;
struct UseItemParam : public CRCore::Referenced
{
	CRCore::ref_ptr<crInstanceItem> m_user;
	CRCore::ref_ptr<crInstanceItem> m_target;
	CRCore::crVector3 m_dirOffset;
	CRCore::crVector3 m_targetPos;
};
struct ItemUseParam : public CRCore::Referenced
{
	ItemUseParam():m_itemid(0),m_chanttime(0.0f){}
	bool isValid(){ return m_weapon.valid(); }
	void clear(){m_weapon = NULL; m_targetNode = NULL; m_itemid = 0; m_chanttime = 0.0f; }
	int m_itemid;//
	float m_chanttime;//0表示只吟唱一次，当m_chanttime<m_useItemProtectTime时，表示该法术是持续施法，将会在m_useItemProtectTime期间内每隔m_chanttime触发一次WCH_RecvItemUse
	CRCore::ref_ptr<CRCore::crGroup> m_weapon;
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::crVector3 m_targetPos;
	CRCore::crVector3 m_dirOffset;
};
//typedef std::map<int,float> AwardItemMap;//item abstractid,可能性 任务完成后的物品奖励
//typedef std::map< int,CRCore::ref_ptr<crInstanceItem> > AwardItemMap;//item abstractid,可能性 任务完成后的物品奖励
class CRNETAPP_EXPORT crGameTask : public CRCore::crDataObject
{
public:
	crGameTask();
	crGameTask& operator = (const crGameTask& gametask);
	void setTaskID(int id);
	int getTaskID();
	void setTaskName(const std::string &name);
	std::string &getTaskName();
	void setHyperText(const std::string &filename);
	const std::string &getHyperText() const;
	void setTaskSteps(const CRCore::crVector4b &steps);
	void setTaskSteps(int steps);
	int getTaskSteps();
	void setExp(int exp);
	int getExp();
	void setExp2(int exp);
	int getExp2();
	void setGold(int gold);
	int getGold();
	//void addAwardItemMap(int abstractid/*,float probability*/);
	//bool setAwardItem(crInstanceItem *item);
	//bool getAwardItem(int abstractid,CRCore::ref_ptr<crInstanceItem> &outitem);
	//AwardItemMap &getAwardItemMap();

	void setActivation(bool activation);
	bool getActivation();
	void setActivationFlg(char flg);
	char getActivationFlg();
	bool isNeedSave();
	void setTaskProgress(int progress);
	int getTaskProgress();
	void resetTask();
	void refresh(int circles);
	bool isTaskCanRefer();
	bool isComplete();
	bool isTaskCanAdvance(unsigned char steptype);
	bool advance(unsigned char steptype);
	void refer();//

	void setLvRange(const CRCore::crVector2s &range);
	const CRCore::crVector2ub &getLvRange();
	void setPreTask(int preTask);
	int getPreTask();
	void setCircles(unsigned char circles);
	unsigned char getCircles();
	void setTaskStepType(unsigned char steptype);
	unsigned char getTaskStepType();
	//bool isRequestAwardItem();//client端
protected:
	virtual ~crGameTask();
	int m_taskid;
	std::string m_taskName;
	std::string m_hypertext;
	int m_taskSteps;//表示任务步骤数量
	//完成任务的奖励或者扣除，关于物品的扣除可以在设定区域使用任务道具
	int m_exp;
	int m_exp2;//灵兽经验
	int m_gold;
	//关于物品的奖励
	//AwardItemMap m_awardItemMap;

	unsigned char m_activation;//任务激活标识 0:未激活,1激活,2已完成
	int m_taskProgress;//m_taskProgress==m_taskSteps表示任务已经完成

	//等级要求
	CRCore::crVector2ub m_lvRange;
	int m_preTask;
	unsigned char m_circles;//循环次数,0表示无限次，1默认
	unsigned char m_taskStepType;
};
struct CRNETAPP_EXPORT HitParam : public CRCore::Referenced
{
	CRCore::ref_ptr<crInstanceItem> m_hitItem;
	CRCore::ref_ptr<crInstanceItem> m_fireItem;
	float m_rate;
	CRCore::crVector3 m_targetPos;
};
class CRNETAPP_EXPORT crQueryServerListData : public CRCore::Referenced
{//
public:
	crQueryServerListData();
	void setServerID(int serverid);
	int getServerID();
	void setServerName(const std::string &servername);
	const std::string &getServerName();
	void setISP(unsigned char isp);
	unsigned char getISP();
	void setRemainCount(short count);
	short getRemainCount();
protected:
	int m_serverid;
	int m_webserverid;
	std::string m_servername;
	unsigned char m_isp;//0双线，1电信，2联通
	unsigned short m_remaincount;
};
typedef std::multimap< unsigned char,CRCore::ref_ptr<crQueryServerListData> >ServerListMap;//isp,
class CRNETAPP_EXPORT crQueryConnectServerData : public CRCore::Referenced
{//
public:
	crQueryConnectServerData();
	void setIP(const std::string &ip);
	const std::string &getIP();
	void setIP2(const std::string &ip2);
	const std::string &getIP2();
	void setIP3(const std::string &ip3);
	const std::string &getIP3();
	void setPort(unsigned short port);
	unsigned short getPort();
	void setRemainCount(short remaincount);
	short getRemainCount();
protected:
	std::string m_ip;//电信
	std::string m_ip2;//联通
	std::string m_ip3;//移动
	unsigned short m_port;
	short m_remaincount;
};
typedef std::multimap< short,CRCore::ref_ptr<crQueryConnectServerData>, std::greater<short> >ConnectServerMap;//remaincount,
}

#endif
