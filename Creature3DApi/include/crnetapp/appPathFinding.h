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
#ifndef CRNETAPP_PATHFINDING_H
#define CRNETAPP_PATHFINDING_H 1

#include <CRNetApp/appExport.h>
#include <CRCore/crVector2.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/thread/crThread.h>
#include <gnelib.h>
#include <map>
#include <vector>
#include <deque>
#include <hash_map>
namespace CRNetApp {
class crScene;
class crSceneLayer;
class crInstanceItem;
class crArea;
class crWaypoint;
typedef std::deque<CRCore::crVector2s> CoordDeque;
typedef std::deque<crArea *> AreaDeque;
typedef std::deque<CRCore::crVector2f> PortDeque;
//// 直线平滑
//bool NodeHasBarrier(const crSceneLayer *sceneLayer, const CRCore::crVector2s& node);
//bool NodeHasBarrier(const crSceneLayer *sceneLayer, const short x, const short y);
//void XGetNode(float x, float y, CRCore::crVector2s& outNode);
//void YGetNode(float x, float y, CRCore::crVector2s& outNode);
//bool LineHasBarrier(const crSceneLayer *sceneLayer, const CRCore::crVector2s& start, const CRCore::crVector2s& end);
////	bool LineHasBarrier2(const crSceneLayer *sceneLayer, const CRCore::crVector2s& start, const CRCore::crVector2s& end);
//int SmoothPath(const crSceneLayer *sceneLayer, const CoordDeque& path, CoordDeque& smoothPath);
class CRNETAPP_EXPORT crPathFinding : public CRCore::Referenced
{
public:
	crPathFinding();
	class PathNode : public CRCore::Referenced
	{
	public:
		PathNode(int x = 0, int y = 0):
		  G(0),
		  H(0)/*,
		  ParentG(0)*/
		{
			m_coord.set(x,y);
		}
		/// F值=G+H
		inline int F(){ return G+H; }
		/// G值，即从起始节点到当前节点的距离，直线规定为10，对角线规定为14
		int G;
		/// 从当前节点到目标节点纵向坐标距离和横向坐标距离之和
		int H;
		/// 该节点父节点的G值
		//int ParentG;
		//CRCore::crVector2s m_parentCoord;
		CRCore::crVector2s m_coord;
		CRCore::ref_ptr<PathNode> m_parentNode;
	};
	typedef std::map< CRCore::crVector2s,CRCore::ref_ptr<PathNode> >PathNodeCloseMap;
	typedef std::multimap< int,CRCore::ref_ptr<PathNode>,std::less<int> >PathNodeMap;

	unsigned char findBestPath(crInstanceItem *item, const CRCore::crVector2s &start, const CRCore::crVector2s &target, crSceneLayer *sceneLayer);
	void TryToFindPath(PathNode* node);
	/// 把当前节点四周的八个节点尝试全部添加到开启列表中
	void AddOpenNode(PathNode* node);
	/// 添加一个新节点到开启列表中
	bool AddOneOpenNode(short x, short y, PathNode* node,int g);
	/// 从关闭列表中，从目标点开始倒着查找所有父节点并将其添加到最佳路径中
	void InsertPathNode(PathNode* node);
	bool IsInClosePath(PathNode* node);
	PathNode* IsInOpenPath(PathNode* node);
	CoordDeque &getBestPath();
	inline int getClosePathCount(){ return m_closePath.size(); }
	void reset();
	static void setMaxSearchCount(int count){ s_maxSearchCount = count; }
protected:
	virtual ~crPathFinding();
	bool canpass(short x, short y);

	//void sortPathByF();
	CRCore::ref_ptr<crSceneLayer> m_sceneLayer;
	CRCore::ref_ptr<crScene> m_scene;
	CRCore::ref_ptr<crInstanceItem> m_item;
	PathNodeCloseMap m_closePath;
	PathNodeMap m_openPath;
	CRCore::ref_ptr<PathNode> m_startNode;
	CRCore::ref_ptr<PathNode> m_endNode;
	CRCore::ref_ptr<PathNode> m_currentNode;
	//CRCore::ref_ptr<PathNode> m_nearestNode;
	CoordDeque m_bestPath;
	bool m_findSuccess;
	//short m_step;
	static int s_maxSearchCount;
};

class CRNETAPP_EXPORT crAreaPathFinding : public CRCore::Referenced
{
public:
	crAreaPathFinding();
	class AreaPathNode : public CRCore::Referenced
	{
	public:
		AreaPathNode(crArea *area = NULL):
		  G(0),
		  m_area(area)
		{
		}
		inline float F(){ return G; }
		float G;
		crArea *m_area;
		CRCore::ref_ptr<AreaPathNode> m_parentNode;
		CRCore::crVector2f m_pos;
	};
	typedef std::map<float,CRCore::ref_ptr<AreaPathNode> > AreaPathNodeCloseMap;
	typedef std::multimap< float,CRCore::ref_ptr<AreaPathNode>,std::less<float> >AreaPathNodeMap;

	unsigned char findBestPath(crArea *start, crArea *target, const CRCore::crVector2f &from, const CRCore::crVector2f &to);
	void TryToFindPath(AreaPathNode* node);
	/// 把当前节点四周的八个节点尝试全部添加到开启列表中
	void AddOpenNode(AreaPathNode* node);
	/// 添加一个新节点到开启列表中
	bool AddOneOpenNode(crArea *area, AreaPathNode* node);
	/// 从关闭列表中，从目标点开始倒着查找所有父节点并将其添加到最佳路径中
	void InsertPathNode(AreaPathNode* node);
	bool IsInClosePath(AreaPathNode* node);
	AreaPathNode* IsInOpenPath(AreaPathNode* node);
	PortDeque &getBestPath();
	void reset();
protected:
	virtual ~crAreaPathFinding();
	AreaPathNodeCloseMap m_closePath;
	AreaPathNodeMap m_openPath;
	CRCore::ref_ptr<AreaPathNode> m_startNode;
	CRCore::ref_ptr<AreaPathNode> m_endNode;
	CRCore::ref_ptr<AreaPathNode> m_currentNode;
	PortDeque m_bestPath;
	bool m_findSuccess;
};

class CRNETAPP_EXPORT crWaypointPathFinding : public CRCore::Referenced
{
public:
	crWaypointPathFinding();
	class WaypointPathNode : public CRCore::Referenced
	{
	public:
		WaypointPathNode(crWaypoint *wp = NULL):
			G(0),
			H(0),
			m_waypoint(wp)
		{
		}
		inline int F(){ return G+H; }
		int G;
		int H;
		crWaypoint *m_waypoint;
		CRCore::ref_ptr<WaypointPathNode> m_parentNode;
	};
	typedef std::map<CRCore::crVector2s,CRCore::ref_ptr<WaypointPathNode> > WaypointPathNodeCloseMap;
	typedef std::multimap< int,CRCore::ref_ptr<WaypointPathNode>,std::less<int> >WaypointPathNodeMap;

	unsigned char findBestPath(crWaypoint *start, crWaypoint *target);
	void TryToFindPath(WaypointPathNode* node);
	/// 把当前节点四周的八个节点尝试全部添加到开启列表中
	void AddOpenNode(WaypointPathNode* node);
	/// 添加一个新节点到开启列表中
	bool AddOneOpenNode(crWaypoint *wp, WaypointPathNode* node);
	/// 从关闭列表中，从目标点开始倒着查找所有父节点并将其添加到最佳路径中
	void InsertPathNode(WaypointPathNode* node);
	bool IsInClosePath(WaypointPathNode* node);
	WaypointPathNode* IsInOpenPath(WaypointPathNode* node);
	CoordDeque &getBestPath();
	inline int getClosePathCount(){ return m_closePath.size(); }
	void reset();
protected:
	virtual ~crWaypointPathFinding();
	WaypointPathNodeCloseMap m_closePath;
	WaypointPathNodeMap m_openPath;
	CRCore::ref_ptr<WaypointPathNode> m_startNode;
	CRCore::ref_ptr<WaypointPathNode> m_endNode;
	CRCore::ref_ptr<WaypointPathNode> m_currentNode;
	CoordDeque m_bestPath;
	bool m_findSuccess;
};

class CRNETAPP_EXPORT crDynamicPathFinding : public CRCore::Referenced
{
public:
	crDynamicPathFinding();
	class PathNode : public CRCore::Referenced
	{
	public:
		PathNode(int x = 0, int y = 0):
		  G(0),
		  H(0)
		{
			m_coord.set(x,y);
		}
		inline int F(){ return G+H; }
		int G;
		/// 从当前节点到目标节点纵向坐标距离和横向坐标距离之和
		int H;
		CRCore::crVector2s m_coord;
		CRCore::ref_ptr<PathNode> m_parentNode;
	};
	typedef std::map< CRCore::crVector2s,CRCore::ref_ptr<PathNode> >PathNodeCloseMap;
	typedef std::multimap< int,CRCore::ref_ptr<PathNode>,std::less<int> >PathNodeMap;

	unsigned char findBestPath(crInstanceItem *item, CRCore::crVector2s& start, CRCore::crVector2s& target, crSceneLayer *sceneLayer);
	void TryToFindPath(PathNode* node);
	void AddOpenNode(PathNode* node);
	bool AddOneOpenNode(short x, short y, PathNode* node,int g);
	void InsertPathNode(PathNode* node);
	bool IsInClosePath(PathNode* node);
	PathNode* IsInOpenPath(PathNode* node);
	CoordDeque &getBestPath();
	inline int getClosePathCount(){ return m_closePath.size(); }
	void reset();
protected:
	virtual ~crDynamicPathFinding();
	CRCore::ref_ptr<crSceneLayer> m_sceneLayer;
	CRCore::ref_ptr<crScene> m_scene;
	CRCore::ref_ptr<crInstanceItem> m_item;
	PathNodeCloseMap m_closePath;
	PathNodeMap m_openPath;
	CRCore::ref_ptr<PathNode> m_startNode;
	CRCore::ref_ptr<PathNode> m_endNode;
	CRCore::ref_ptr<PathNode> m_currentNode;
	CoordDeque m_bestPath;
	bool m_findSuccess;
};

class CRNETAPP_EXPORT crPathFindingManager : public CRCore::Referenced
{
public:
	crPathFindingManager();
	//static void setPathFindingCount(unsigned char count);
	//typedef std::vector< CRCore::ref_ptr<crPathFinding> > PathFindingVec;
	typedef std::deque< CRCore::ref_ptr<crInstanceItem> > RequestDeque;
	void addPathFindRequest(crInstanceItem *item);
	void addWaypointFindRequest(crInstanceItem *item);
	void recvPathFindRequest(crInstanceItem *item);
	void update();
	void init(crScene *scene);
	void SmoothPath(const crSceneLayer *sceneLayer, CoordDeque& inpath, CoordDeque& outpath);
	bool NodeHasBarrier(const crSceneLayer *sceneLayer, const CRCore::crVector2s& node);
	bool NodeHasBarrier(const crSceneLayer *sceneLayer, const short x, const short y);
	void XGetNode(float x, float y, CRCore::crVector2s& outNode);
	void YGetNode(float x, float y, CRCore::crVector2s& outNode);
	bool LineHasBarrier(const crSceneLayer *sceneLayer, const CRCore::crVector2s& start, const CRCore::crVector2s& end);
protected:
	virtual ~crPathFindingManager();
	bool isInRequestDeque(crInstanceItem *item);
	CRCore::ref_ptr<crPathFinding> m_pathFinding;
	CRCore::ref_ptr<crAreaPathFinding> m_areaPathFinding;
	CRCore::ref_ptr<crWaypointPathFinding> m_waypointPathFinding;
	//PathFindingVec m_pathFindingVec;
	GNE::Mutex m_requestDequeMutex;
	RequestDeque m_requestDeque;
	RequestDeque m_requestDeque_buf;
	CRCore::ref_ptr<crScene> m_scene;
	//static unsigned char s_pathFindingCount;
};

class CRNETAPP_EXPORT crPathFindingUpdater: public CRCore::crThread, public CRCore::Referenced
{
public:
	crPathFindingUpdater();
	static crPathFindingUpdater *getInstance();
	virtual void clear();
	virtual void run();
	void done();
	void registerPathFindingManager(crPathFindingManager *manager);
protected:
	virtual ~crPathFindingUpdater();
	static CRCore::ref_ptr<crPathFindingUpdater> m_instance;
	typedef std::vector< CRCore::ref_ptr<crPathFindingManager> > PathFindingManagerVec;
	GNE::Mutex m_pathFindingManagerMutex;
	PathFindingManagerVec m_pathFindingManagerVec;
private:
	bool m_done;
};
}
#endif