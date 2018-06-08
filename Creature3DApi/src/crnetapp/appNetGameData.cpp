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
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStreamBuf.h>
#include <CRNet/crNetContainer.h>
#include <CRIOManager/crFileUtils.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crCookFile.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRIOManager/crReadFile.h>
#include <CREncapsulation/crTableIO.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appSightInfo.h>
#include <CRCore/crPoint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <CRIOManager/crLoadManager.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRPhysics;
using namespace CRIOManager;

//#undef SMOOTH_PATH
#define SMOOTH_PATH

///////////////////////////////////
//
//crRes
//
/////////////////////////////////////
crRes::crRes():
m_resid(0)//,
//m_fileModifyTime(0),
//m_updcfgModifyTime(0),
//m_resscriptModifyTime(0)
{
}
crRes::~crRes()
{

}
crRes::crRes(const crRes& res):
m_resid(res.m_resid),
m_filename(res.m_filename),
//m_fileModifyTime(res.m_fileModifyTime),
//m_updcfgModifyTime(res.m_updcfgModifyTime),
m_resscript(res.m_resscript)//,
//m_resscriptModifyTime(res.m_resscriptModifyTime)
{

}
void crRes::setResID(int id)
{
	m_resid = id;
}
int crRes::getResID()
{
	return m_resid;
}
void crRes::setFileName(const std::string &fileName)
{
	m_filename = fileName;
}
const std::string &crRes::getFileName() const
{
	return m_filename;
}
//void crRes::setFileModifyTime(CRCore::Timer_t mtime)
//{
//	m_fileModifyTime = mtime;
//}
//CRCore::Timer_t crRes::getFileModifyTime() const
//{
//	return m_fileModifyTime;
//}
//std::string crRes::getUpdatecfg() const
//{
//	if(m_filename.empty())
//		return m_filename;
//	std::string updcfg = crArgumentParser::getFileNameEliminateExt(m_filename)+".crb";
//	return updcfg;
//}
//void crRes::setUpdcfgModifyTime(CRCore::Timer_t mtime)
//{
//	m_updcfgModifyTime = mtime;
//}
//CRCore::Timer_t crRes::getUpdcfgModifyTime() const
//{
//	return m_updcfgModifyTime;
//}
void crRes::setResScript(const std::string &script)
{
	m_resscript = script;
}
const std::string &crRes::getResScript() const
{
	return m_resscript;
}
//void crRes::setResScriptModifyTime(CRCore::Timer_t mtime)
//{
//	m_resscriptModifyTime = mtime;
//}
//CRCore::Timer_t crRes::getResScriptModifyTime() const
//{
//	return m_resscriptModifyTime;
//}
///////////////////////////////////
//
//crAbstractItem
//
/////////////////////////////////////
crAbstractItem::crAbstractItem():
m_abstractitemid(0)/*,
m_scriptModifyTime(0)*/
{
}
crAbstractItem::~crAbstractItem()
{

}
crAbstractItem::crAbstractItem(const crAbstractItem& item):
crRes(item),
m_abstractitemid(item.m_abstractitemid),
m_name(item.m_name),
m_script(item.m_script)/*,
m_scriptModifyTime(item.m_scriptModifyTime)*/
{
}
void crAbstractItem::setAbstractItemID(int id)
{
	m_abstractitemid = id;
}
int crAbstractItem::getAbstractItemID()
{
	return m_abstractitemid;
}
void crAbstractItem::setName(const std::string &name)
{
	m_name = name;
}
const std::string &crAbstractItem::getName() const
{
	return m_name;
}
void crAbstractItem::setScript(const std::string &script)
{
	m_script = script;
}
const std::string &crAbstractItem::getScript() const
{
	return m_script;
}
//void crAbstractItem::setScriptModifyTime(CRCore::Timer_t mtime)
//{
//	m_scriptModifyTime = mtime;
//}
//CRCore::Timer_t crAbstractItem::getScriptModifyTime() const
//{
//	return m_scriptModifyTime;
//}
///////////////////////////////////
//
//crAbstractMetier
//
/////////////////////////////////////
crAbstractMetier::crAbstractMetier():
m_abstractmetierid(0),
m_gameid(0),
m_initsceneid(0),
m_initlayerid(0),
m_zoffset(0)/*,
m_metierScriptModifyTime(0L),
m_tasktabModifyTime(0L)*/
{
}
crAbstractMetier::~crAbstractMetier()
{

}
crAbstractMetier::crAbstractMetier(const crAbstractMetier& metier):
m_abstractmetierid(metier.m_abstractmetierid),
m_gameid(metier.m_gameid),
m_metiername(metier.m_metiername),
m_initsceneid(metier.m_initsceneid),
m_initlayerid(metier.m_initlayerid),
m_zoffset(metier.m_zoffset),
m_metierscript(metier.m_metierscript)/*,
m_metierScriptModifyTime(metier.m_metierScriptModifyTime)*/
{
}
void crAbstractMetier::setAbstractMetierID(int id)
{
	m_abstractmetierid = id;
}
int crAbstractMetier::getAbstractMetierID()
{
	return m_abstractmetierid;
}
void crAbstractMetier::setGameID(int gameid)
{
	m_gameid = gameid;
}
int crAbstractMetier::getGameID()
{
	return m_gameid;
}
void crAbstractMetier::setMetierName(const std::string &metiername)
{
	m_metiername = metiername;
}
const std::string &crAbstractMetier::getMetierName() const
{
	return m_metiername;
}
void crAbstractMetier::setInitSceneID(int sceneid)
{
	m_initsceneid = sceneid;
}
int crAbstractMetier::getInitSceneID()
{
	return m_initsceneid;
}
void crAbstractMetier::setInitLayerID(unsigned short layerid)
{
	m_initlayerid = layerid;
}
unsigned short crAbstractMetier::getInitLayerID()
{
	return m_initlayerid;
}
void crAbstractMetier::setZoffset(short zoffset)
{
	m_zoffset = zoffset;
}
short crAbstractMetier::getZoffset()
{
	return m_zoffset;
}
void crAbstractMetier::setMetierScript(const std::string &script)
{
	m_metierscript = script;
}
const std::string &crAbstractMetier::getMetierScript() const
{
	return m_metierscript;
}
//void crAbstractMetier::setMetierScriptModifyTime(CRCore::Timer_t mtime)
//{
//    m_metierScriptModifyTime = mtime;
//}
//CRCore::Timer_t crAbstractMetier::getMetierScriptModifyTime() const
//{
//    return m_metierScriptModifyTime;
//}
void crAbstractMetier::setTaskTable(const std::string &file)
{
	m_tasktab = file;
}
std::string &crAbstractMetier::getTaskTable()
{
	return m_tasktab;
}
//void crAbstractMetier::setTaskTableModifyTime(CRCore::Timer_t mtime)
//{
//	m_tasktabModifyTime = mtime;
//}
//CRCore::Timer_t crAbstractMetier::getTaskTableModifyTime() const
//{
//	return m_tasktabModifyTime;
//}
///////////////////////////////////
//
//crInstanceItem
//
/////////////////////////////////////
crInstanceItem::crInstanceItem():
m_instanceitemid(0),
m_itemtype(0),
m_sceneid(0),
m_layerid(0),
m_posx(0.0f),
m_posy(0.0f),
m_posz(0.0f),
m_zoffset(0.0f),
m_adjustDirTime(0.0f),
m_adjustRadian(0.0f),
m_currentMsg(0),
m_roomid(0),
m_itemstate_record(IS_None),
m_transportsceneid(0),
m_delayTime(0.0f),
m_dropItemTimer(300.0f),
m_sightInfo(NULL),
m_ownerid(0L),
m_reliveTimer(0.0f)
{
	m_dir.set(0,-1,0);
	m_targetDir.set(0,-1,0);
	m_adjustUp = CRCore::Z_AXIS;

	//char gbuf[256];
	//sprintf(gbuf,"new crInstanceItem %d\n\0",this);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}
crInstanceItem::~crInstanceItem()
{
	//char gbuf[256];
	//sprintf(gbuf,"delete crInstanceItem %d\n\0",this);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}
crInstanceItem::crInstanceItem(const crInstanceItem& item):
Referenced(item),
m_abstractItem(item.m_abstractItem),
m_instanceitemid(item.m_instanceitemid),
m_itemtype(item.m_itemtype),
m_iname(item.m_iname),
m_sceneid(item.m_sceneid),
m_layerid(item.m_layerid),
m_posx(item.m_posx),
m_posy(item.m_posy),
m_posz(item.m_posz),
m_zoffset(item.m_zoffset),
m_dir(item.m_dir),
m_targetDir(item.m_targetDir),
m_adjustDirTime(item.m_adjustDirTime),
m_adjustRadian(item.m_adjustRadian),
m_adjustUp(item.m_adjustUp),
m_currentMsg(0),
m_stream(NULL),
m_node(NULL),
m_roomid(item.m_roomid),
m_itemstate_record(IS_None),
m_transportsceneid(0),
m_delayTime(0.0f),
m_dropItemTimer(300.0f),
m_sightInfo(NULL),
m_ownerid(0L),
m_reliveTimer(0.0f)
{
	if(item.m_dataClass.valid())
		m_dataClass = item.m_dataClass->clone(CRCore::crCopyOp::DEEP_COPY_ALL);
	if(!item.m_childItemMap.empty())
	{
	    ref_ptr<crItemChild> itemChild;
		GNE::LockMutex lock( m_childItemMapMutex );
		for( ChildItemMap::const_iterator itr = item.m_childItemMap.begin();
			 itr != item.m_childItemMap.end();
			 ++itr )
		{
			itemChild = itr->second->clone();
			itemChild->setParentItem(this);
            m_childItemMap[itemChild->getItemChildID()] = itemChild.get();
		}
	}

	//char gbuf[256];
	//sprintf(gbuf,"clone crInstanceItem %d\n\0",this);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
}
void crInstanceItem::setAbstractItem(crAbstractItem *aitem)
{
	m_abstractItem = aitem;
}
crAbstractItem *crInstanceItem::getAbstractItem()
{ 
	return m_abstractItem.get(); 
}
int crInstanceItem::getAbstractItemID()
{
	if(m_abstractItem.valid())
		return m_abstractItem->getAbstractItemID();
	else
		return 0;
}
void crInstanceItem::setAbstractItemID(int abstractid)
{
	if(!m_abstractItem.valid())
	{
		m_abstractItem = new crAbstractItem;
	}
	m_abstractItem->setAbstractItemID(abstractid);
}
void crInstanceItem::setInstanceItemID(int id)
{
	m_instanceitemid = id;
}
int crInstanceItem::getInstanceItemID()
{
	return m_instanceitemid;
}
void crInstanceItem::setItemtype(unsigned char itemtype)
{
	m_itemtype = itemtype;
}
unsigned char crInstanceItem::getItemtype()
{
	return m_itemtype;
}
void crInstanceItem::setIName(const std::string &iname)
{
	m_iname = iname;
}
const std::string &crInstanceItem::getIName() const
{
	return m_iname.empty()?m_abstractItem->getName():m_iname;
}
const std::string &crInstanceItem::getIName2() const
{
	return m_iname;
}
const std::string &crInstanceItem::getAName() const
{
	return m_abstractItem->getName();
}
void crInstanceItem::setSceneID(int sceneid)
{
	m_sceneid = sceneid;
}
int crInstanceItem::getSceneID()
{
	return m_sceneid;
}
void crInstanceItem::setLayerID(unsigned short layerid)
{
	m_layerid = layerid;
}
unsigned short crInstanceItem::getLayerID()
{
	return m_layerid;
}
void crInstanceItem::setPosxy(float posx,float posy)
{
	m_posx = posx;
	m_posy = posy;
	//if(crGlobalHandle::isClient())
	//{
	//	crSceneLayer *layer = crMyPlayerData::getInstance()->getSceneLayer();
	//	if(layer && dynamic_cast<crRole *>(this))
	//	{
	//		CRCore::crVector2s coord = layer->getCoord(m_posx * crGlobalHandle::gData()->gUnitScale(),m_posy * crGlobalHandle::gData()->gUnitScale());
	//		if(!layer->walkability(coord[0],coord[1]))
	//		{
	//			CRCore::notify(CRCore::WARN)<<"crRole::setPosxy: 区域不能通过："<<m_posx<<","<<m_posy<<std::endl;
	//		}
	//	}
	//}
}
void crInstanceItem::setPosxy(CRCore::crVector2i& posxy)
{
	setPosxy(posxy[0],posxy[1]);
}
void crInstanceItem::setPosxy(CRCore::crVector2f& posxy)
{
	setPosxy(posxy[0],posxy[1]);
}
float crInstanceItem::getPosx()
{
	return m_posx;
}
float crInstanceItem::getPosy()
{
	return m_posy;
}
void crInstanceItem::setPosz(float posz)
{
	m_posz = posz;
}
void crInstanceItem::setZoffset(short zoffset)
{
	m_zoffset = zoffset;
}
short crInstanceItem::getZoffset()
{
	return m_zoffset;
}
float crInstanceItem::getPosz()
{
	return m_posz;
}
//void crInstanceItem::setDirx(float dirx)
//{
//	m_dirx = dirx;
//}
float crInstanceItem::getDirx()
{
	return m_dir[0];
}
//void crInstanceItem::setDiry(float diry)
//{
//	m_diry = diry;
//}
float crInstanceItem::getDiry()
{
	return m_dir[1];
}
//void crInstanceItem::setDirz(float dirz)
//{
//	m_dirz = dirz;
//}
float crInstanceItem::getDirz()
{
	return m_dir[2];
}
void crInstanceItem::setTargetDir(const CRCore::crVector3 &dir)
{
	unsigned int guisestate = GS_Normal;
	doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
	if(guisestate & GS_ExtraShifting)
		return;
	if(crGlobalHandle::isClient())
	{
		m_targetDir = dir;
		if( m_targetDir == m_dir )
		{
			m_adjustUp = CRCore::Z_AXIS;
			m_adjustDirTime = 0.0f;
			m_adjustRadian = 0.0f;
		}
		else
		{
			m_adjustUp = m_dir ^ m_targetDir;
			m_adjustUp.normalize();
			if(m_adjustUp == crVector3(0.0f,0.0f,0.0f))
			{
				m_adjustUp = CRCore::Z_AXIS;
				m_adjustDirTime = 0.0f;
				m_adjustRadian = 0.0f;
				m_dir = m_targetDir;
			}
			else
			{
				m_adjustRadian = acos(m_dir * m_targetDir);
				m_adjustDirTime = m_adjustRadian / crGlobalHandle::gData()->gItemAdjustAngular();
				//CRCore::notify(CRCore::ALWAYS)<<m_dir<<"^"<<m_targetDir<<"="<<m_adjustUp<<std::endl;
			}
		}
	}
	else
	{
		setDir(dir);
	}
}
void crInstanceItem::addSyncPos(const CRCore::crVector2 &pos)
{
	GNE::LockMutex lock( m_syncPosDequeMutex );
	if(isMainAI())
	{
		if(!m_syncPosDeque.empty())
		{
			m_syncPosDeque.back() = pos;
		}
		else
		{
			m_syncPosDeque.push_back(pos);
		}
	}
	else
	{
		if(!m_syncPosDeque.empty())
		{
			if(m_syncPosDeque.back() == pos)
				return;
		}
		m_syncPosDeque.push_back(pos);
	}
}
void crInstanceItem::clearSyncPos()
{
	GNE::LockMutex lock( m_syncPosDequeMutex );
	return m_syncPosDeque.clear();
}
void crInstanceItem::clearExtra()
{
	if(m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_ExtraData,param);
		if(param)
		{
			m_dataClass->excHandle(MAKEINT64(WCH_ResetExtra,NULL));
			crData *extraData = (crData *)param;
			if(crGlobalHandle::isClient())
			{
				extraData->releaseObjects(NULL);
			}
			extraData->clear();
			//crHandle *handle;
			//extraData->excHandle(MAKEINT64(WCH_LockData,1));
			//extraData->getParam(WCHDATA_ExtraIDMap,param);
			//ExtraIDMap *extraIDMap = (ExtraIDMap *)param;
			//for( ExtraIDMap::iterator itr = extraIDMap->begin();
			//	 itr != extraIDMap->end();
			//	 ++itr )
			//{
			//	if(crGlobalHandle::isClient())
			//	{
			//		handle = extraData->getHandle(MAKEINT64(WCH_DoExtra,itr->first));
			//		if(handle)
			//		{
			//			handle->releaseObjects(NULL);
			//		}
			//	}
			//	extraData->removeHandle(MAKEINT64(WCH_DoExtra,itr->first));
			//}
			//extraIDMap->clear();
			//extraData->excHandle(MAKEINT64(WCH_LockData,0));
			//extraData->clear();
		}
	}
}
void crInstanceItem::clearData()
{
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	{
		GNE::LockMutex lock( m_childItemMapMutex );
		for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
			itr != m_childItemMap.end();
			++itr )
		{
			itemChild = itr->second.get();
			instanceItem = itemChild->getInstanceItem();
			instanceItem->clearData();
			itemChild->clear();
		}
		m_childItemMap.clear();
	}
	m_abstractItem = NULL;
	m_stream = NULL;
	m_node = NULL;
	m_dataClass = NULL;
}
bool crInstanceItem::hasSyncPos()
{
	GNE::LockMutex lock( m_syncPosDequeMutex );
	return !m_syncPosDeque.empty();
}
void crInstanceItem::closeToSyncPos(crScene *scene,float& distance)
{
	GNE::LockMutex lock( m_syncPosDequeMutex );
	float scale = crGlobalHandle::gData()->gUnitScale();
	if(m_syncPosDeque.empty())
	{
		distance = 0.0f;
	}
	else
	{
		crVector2 curPos(m_posx*scale,m_posy*scale);
		crVector2 syncPos = m_syncPosDeque.front();
		crVector2 newPos;
		crVector2 dir = syncPos - curPos;
		float length = dir.length();
		if(length == 0.0f)
		{
			m_syncPosDeque.pop_front();
			return closeToSyncPos(scene,distance);
		}
		dir.normalize();
		setTargetDir(crVector3(dir[0],dir[1],0.0f));
		if(distance>=length)
		{
			newPos = syncPos;
			m_syncPosDeque.pop_front();
			distance -= length;
			newPos/=scale;
			setPosxy(newPos[0],newPos[1]);
			return closeToSyncPos(scene,distance);
		}
		else if(length>5.0f)
		{//位移超过5米
			newPos = syncPos;
			m_syncPosDeque.pop_front();
			distance = 0.0f;
		}
		else
		{
			//if(equivalent(distance,length,DELTA))
			//	m_syncPosDeque.pop_front();
			newPos = curPos + dir * distance;
			distance = 0.0f;
		}
		newPos/=scale;
		setPosxy(newPos[0],newPos[1]);
	}
	crSceneLayer *scenelayer = scene->getSceneLayer(getLayerID());
	if(scenelayer)
	{
		float posx = getPosx() * scale;
		float posy = getPosy() * scale;
		crVector2s pos = scenelayer->getCoord(posx,posy);
		float z = scenelayer->getHeight(pos[0],pos[1]);
		z /= scale;
		z += getZoffset();
		setPosz(z);
		if(crGlobalHandle::isClient())
		{
			crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(getRelNode());
			if(bot)
			{
				doEvent(WCH_ItemCoordToNode,MAKEINT64(bot,crMatterObject::MD_FullMatrix));
			}
		}
	}
}
bool crInstanceItem::getFrontSyncPos(CRCore::crVector2 &pos)
{
	GNE::LockMutex lock( m_syncPosDequeMutex );
	if(!m_syncPosDeque.empty())
	{
		pos = m_syncPosDeque.front();
		return true;
	}
	return false;
}
void crInstanceItem::setDir(const CRCore::crVector3 &dir)
{
	m_dir = dir;
	m_dir.normalize();
	m_adjustDirTime = 0.0f;
	m_adjustRadian = 0.0f;
}
CRCore::crVector3f crInstanceItem::getPosition()
{
	return crVector3f(m_posx,m_posy,m_posz) * crGlobalHandle::gData()->gUnitScale();
}
const CRCore::crVector3f& crInstanceItem::getDir()
{
	return m_dir;
}
//CRCore::crVector3f crInstanceItem::getDir()
//{
//	return crVector3f(m_dirx,m_diry,m_dirz);
//}
void crInstanceItem::insertChildItem(crItemChild *item)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	m_childItemMap[item->getItemChildID()] = item;
	item->setParentItem(this);
}
void crInstanceItem::removeChildItem(int itemchildid)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	ChildItemMap::iterator itr = m_childItemMap.find(itemchildid);
	if(itr != m_childItemMap.end())
	{
		insertRemovedItemChild(itr->second.get());
		m_childItemMap.erase(itr);
	}
}
void crInstanceItem::insertRemovedItemChild(crItemChild *item)
{
	GNE::LockMutex lock( m_removedItemChildDequeMutex );
	m_removedItemChildDeque.push_back(std::make_pair(item,crGlobalHandle::gData()->gRemoveItemChildTime()));
}
crItemChild* crInstanceItem::findRemovedItemChild(int itemid)
{
	GNE::LockMutex lock( m_removedItemChildDequeMutex );
	for( RemovedItemChildDeque::iterator itr = m_removedItemChildDeque.begin();
		 itr != m_removedItemChildDeque.end();
		 ++itr )
	{
		if(itr->first->getInstanceItemID() == itemid)
			return itr->first.get();
	}
	return NULL;
}
void crInstanceItem::removedItemChildUpdate(float dt)
{
	GNE::LockMutex lock( m_removedItemChildDequeMutex );
	for( RemovedItemChildDeque::iterator itr = m_removedItemChildDeque.begin();
		itr != m_removedItemChildDeque.end();
		++itr )
	{
		itr->second -= dt;
	}
	if(!m_removedItemChildDeque.empty() && m_removedItemChildDeque.front().second<=0.0f)
	{
		m_removedItemChildDeque.pop_front();
	}
}
void crInstanceItem::closeToTargetDir(float dt)
{
	if(m_adjustDirTime>0.0f)
	{
		float adjust;
		if(m_adjustDirTime>dt)
		{
			adjust = dt * crGlobalHandle::gData()->gItemAdjustAngular();
			m_adjustDirTime -= dt;
		}
		else
		{
			adjust = m_adjustDirTime * crGlobalHandle::gData()->gItemAdjustAngular();
			m_adjustDirTime = 0.0f;
		}
		m_dir = m_dir * crMatrix::rotate(adjust,m_adjustUp);
		m_dir.normalize();
	}
	else
	{
		m_adjustDirTime = 0.0f;
		m_adjustRadian = 0.0f;
	}
}
void crInstanceItem::clientUpdate(float dt)
{
	//void *param;
	//if (m_dataClass.valid())
	//{
	//	m_dataClass->getParam(WCHDATA_ItemState, param);
	//	unsigned char itemstate = *(unsigned char*)param;
	//	if (itemstate == IS_Relive)
	//	{
	//		if (m_reliveTimer == 0.0f)
	//		{
	//			doEvent(WCH_ItemRelive);
	//		}
	//		m_reliveTimer += dt;
	//		if (m_reliveTimer > 1.0f)
	//		{
	//			m_reliveTimer = 0.0f;
	//			itemstate = IS_Stop;
	//			m_dataClass->inputParam(WCHDATA_ItemState, &itemstate);
	//		}
	//		//clearExtra();
	//		//m_dataClass->excHandle(MAKEINT64(WCH_InitData,this));
	//	}
	//}
	closeToTargetDir(dt);
	removedItemChildUpdate(dt);
}
void crInstanceItem::setDropItemTimer(float interval)
{
	m_dropItemTimer = interval;
}
void crInstanceItem::setOwnerID(_crInt64 ownerid)
{
	m_ownerid = ownerid;
}
_crInt64 crInstanceItem::getOwnerID()
{
	return m_ownerid;
}
void crInstanceItem::serverUpdate(float dt)
{
	//closeToTargetDir(dt);
	void *param;
	if(m_dataClass.valid())
	{
		m_dataClass->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *(unsigned char*)param;
		if(itemstate == IS_Dead)
		{
			doEvent(MAKEINT64(WCH_DeadEvent,IR_ServerRefash),MAKEINT64(&dt,NULL));
		}
		else if(itemstate == IS_Relive)
		{
			if (m_reliveTimer == 0.0f)
			{
				doEvent(WCH_ItemRelive);
			}
			m_reliveTimer+=dt;
			if (m_reliveTimer > 1.0f)
			{
				m_reliveTimer = 0.0f;
				itemstate = IS_Stop;
				m_dataClass->inputParam(WCHDATA_ItemState, &itemstate);
			}
			//clearExtra();
			//m_dataClass->excHandle(MAKEINT64(WCH_InitData,this));
		}
		else
		{
			float updatedt = dt;
			doEvent(WCH_ServerUpdate,MAKEINT64(&updatedt,NULL));
		}
		if(getItemtype() == crInstanceItem::Npc)
		{
			m_dataClass->excHandle(MAKEINT64(WCH_LockData,1));
			m_dataClass->getParam(WCHDATA_InPatrolEnemyMap,param);
			InPatrolEnemyMap *inPatrolEnemyMap = (InPatrolEnemyMap *)param;
			if(inPatrolEnemyMap)
			{
				m_dataClass->getParam(WCHDATA_EnmityMap,param);
				EnmityMap *enmityMap = (EnmityMap *)param;
				if(itemstate == IS_Dead)
				{
					if(!inPatrolEnemyMap->empty()) inPatrolEnemyMap->clear();
					if(!enmityMap->empty()) enmityMap->clear();
				}
				else
				{
					EnmityMap newEnmityMap;
					EnmityMap::iterator eitr;
					//_crInt64 id;
					for( InPatrolEnemyMap::iterator itr = inPatrolEnemyMap->begin();
						itr != inPatrolEnemyMap->end();
						++itr )
					{
						//if(itr->second->getItemtype() == crInstanceItem::Role)
						//{
						//	id = MAKEINT64(itr->second->getRoleID(),itr->second->getID());//roleid,playerid
						//}
						//else if(itr->second->getItemtype() == crInstanceItem::Npc)
						//{
						//	id = MAKEINT64(itr->second->getInstanceItemID(),NULL);
						//}
						eitr = enmityMap->find(itr->second);
						if(eitr != enmityMap->end())
						{
							newEnmityMap.insert(*eitr);//[itr->second] = eitr->first;
						}
					}
					enmityMap->swap(newEnmityMap);
					inPatrolEnemyMap->clear();
				}
			}
			else
			{
				CRCore::notify(CRCore::FATAL)<<"(错误)crInstanceItem::serverUpdate NPC没有Patrol instanceid="<<getInstanceItemID()<<std::endl;
				//sprintf(gDebugInfo->buf(),"(错误)crInstanceItem::serverUpdate NPC没有Patrol instanceid=%d\n\0",getInstanceItemID());
				//gDebugInfo->debugInfo(CRCore::FATAL);
			}
			m_dataClass->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
	removedItemChildUpdate(dt);
	if(m_instanceitemid<0 && m_dropItemTimer>0.0f)
	{
		m_dropItemTimer -= dt;
		if(m_dropItemTimer<=0.0f)
		{
			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crNetManager *netManager = sceneServerConductor->getNetManager();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crScene *scene = netCallback->findScene(getSceneID());
			if(m_dataClass.valid())
			{
				void *param;
				m_dataClass->getParam(WCHDATA_GuiseState,param);
				unsigned int guiseState = *(unsigned int *)param;
				if(guiseState & GS_Static || guiseState & GS_StaticNoneBlock)
					guiseState = GS_StaticUnVisiable;
				else
					guiseState = GS_UnVisiable;
				//send to client
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeUInt(guiseState);
				
				if(getItemtype() == crInstanceItem::Role)
				{
					int playerid = getID();
					crPlayerEventPacket packet;
					crPlayerEventPacket::buildRequestPacket(packet,playerid,this,WCH_RecvGuiseState,stream.get());
					ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(getID()));
					if(playerData.valid())
					{
						netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					scene->sendPacketToItemNeighbor(this,packet);
				}
				else
				{
					crItemEventPacket packet;
					crItemEventPacket::buildRequestPacket(packet,0,this,WCH_RecvGuiseState,stream.get());
					scene->sendPacketToItemNeighbor(this,packet);
				}
				m_dataClass->inputParam(WCHDATA_GuiseState, &guiseState);
				m_dataClass->inputParam(WCHDATA_ControllerFlg,NULL);
			}
			scene->wantToRemoveItem(this);
			crGlobalHandle::recycleItemID(m_instanceitemid);
		}
	}
}
crItemChild *crInstanceItem::getChildItem(int itemchildid)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	ChildItemMap::iterator itr = m_childItemMap.find(itemchildid);
	return itr != m_childItemMap.end()?itr->second.get():NULL;
}
void crInstanceItem::lockChildItemMap()
{
    m_childItemMapMutex.acquire();
}
crInstanceItem::ChildItemMap &crInstanceItem::getChildItemMap()
{
	return m_childItemMap;
}
void crInstanceItem::unlockChildItemMap()
{
	m_childItemMapMutex.release();
}
crItemChild* crInstanceItem::findTemporaryItemChild(int abstractid)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
		itr != m_childItemMap.end();
		++itr )
	{
		itemChild = itr->second.get();
		instanceItem = itemChild->getInstanceItem();
		if(instanceItem->getAbstractItemID() == abstractid)
		{
			if(instanceItem->getInstanceItemID()<0)
				return itemChild;
		}
	}
	return NULL;
}
//void crInstanceItem::clientLoadItemChild()
//{
//	GNE::LockMutex lock( m_childItemMapMutex );
//	crItemChild* itemChild;
//	crInstanceItem *instanceItem;
//	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
//		itr != m_childItemMap.end();
//		++itr )
//	{
//		itemChild = itr->second.get();
//		instanceItem = itemChild->getInstanceItem();
//		if(!instanceItem->isLoaded())
//		{
//			crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,this));
//		}
//		instanceItem->clientLoadItemChild();
//	}
//}
void crInstanceItem::setStream(CRCore::crStreamBuf *stream)
{
    m_stream = stream;
}
CRCore::crStreamBuf *crInstanceItem::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
///////////////////////////////////////////////////////////
void crInstanceItem::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
	if(m_dataClass.valid())
	{
		if(m_itemtype == Npc || m_itemtype == Role)
		{
			if(!dynamic_cast<crRoleData *>(m_dataClass.get()))
			{
				std::string str = "crInstanceItem::loadItemData Error 该NPC载入的脚本不正确，itemid = "+crArgumentParser::appItoa(getInstanceItemID());
				CRCore::notify(CRCore::FATAL)<<str<<std::endl;
				//sprintf(gDebugInfo->buf(),"crInstanceItem::loadItemData Error 该NPC载入的脚本不正确，itemid = %d\n\0",getInstanceItemID());
				//gDebugInfo->debugInfo(CRCore::FATAL);
				//MessageBox(::GetDesktopWindow(),str.c_str(),"Creature3D",MB_OK);
				m_itemtype = instanceitem;
			}
		}
		//else
		//{
		//	if(dynamic_cast<crRoleData *>(m_dataClass.get()))
		//	{
		//		std::string str = "crInstanceItem::loadItemData Error 该Item载入的脚本不正确，itemid = "+crArgumentParser::appItoa(getInstanceItemID());
		//		CRCore::notify(CRCore::FATAL)<<str<<std::endl;
		//		//sprintf(gDebugInfo->buf(),"crInstanceItem::loadItemData Error 该Item载入的脚本不正确，itemid = %d\n\0",getInstanceItemID());
		//		//gDebugInfo->debugInfo(CRCore::FATAL);
		//		//MessageBox(::GetDesktopWindow(),str.c_str(),"Creature3D",MB_OK);
		//		m_itemtype = Npc;
		//	}
		//}
		if(m_abstractItem.valid()) doEvent(WCH_LoadDataAttrFromTab);
	}
}
CRCore::crData *crInstanceItem::getDataClass()
{
	return m_dataClass.get();
}
void crInstanceItem::doEvent(_crInt64 kbmsg, _crInt64 param)
{
	//bool isClient = crNetContainer::getInstance()->getMainNetType() == CRNetApp::Client;
	//if(isClient) m_eventMutex.acquire();
	m_currentMsg = kbmsg;
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				CRCore::notify(CRCore::ALWAYS)<<"crInstanceItem::doEvent error msg = "<<kbmsg<<" itemid = "<<getInstanceItemID()<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
				//sprintf(gDebugInfo->buf(),"crInstanceItem::doEvent error msg=%s itemid=%d\n\0",crArgumentParser::appI64toa(kbmsg),getInstanceItemID());
				//gDebugInfo->debugInfo(CRCore::FATAL);
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
	//if(isClient) m_eventMutex.release();
}
crHandle* crInstanceItem::getHandle(__int64 kbmsg, const std::string& name)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		if(handle)
		{
			handle = handle->getHandle(name);
		}
		if(handle) return handle;
	}
	return NULL;
}

bool crInstanceItem::hasHandle(__int64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crInstanceItem::setNode(CRCore::crNode *node)
{
	m_node = node;
}
CRCore::crNode *crInstanceItem::getNode()
{
	return m_node.get();
}
CRCore::crNode *crInstanceItem::getRelNode()
{
	if(m_itemtype>=crInstanceItem::Npc && m_itemtype<=crInstanceItem::Role)
	{
		crGroup *group = dynamic_cast<crGroup *>(getNode());
		if(group)
			return group->getChild(0);
	}
	else
	{
		return getNode();
	}
	return NULL;
}
//void crInstanceItem::getCreateItemChildLock()
//{
//	m_createItemChildMutex.lock();
//	////m_begin = false;
//}
void crInstanceItem::beginCreateItemChild(_crInt64 completeMsg,int count,_crInt32 completeParam)
{
    m_createCount = count;
	m_completeMsg = completeMsg;
	m_completeParam = completeParam;
	//m_createItemChildVec.clear();
	//m_begin = true;
}
void crInstanceItem::recvItemChild(crItemChild *itemChild)
{
 //   while(!m_begin)
	//{
	//	CRCore::crThread::yieldCurrentThread();
	//}
	m_createItemChildVec.push_back(itemChild);
	m_createCount--;
	if(m_createCount<=0)
	{
		completeCreateItemChild();
	}
}
void crInstanceItem::completeCreateItemChild()
{
	doEvent(m_completeMsg,MAKEINT64(&m_createItemChildVec,m_completeParam));
	m_createItemChildVec.clear();
	//m_begin = false;
	m_completeMsg = 0;
	m_completeParam = 0;
	//m_createItemChildMutex.unlock();
}
void crInstanceItem::loadItemData(char streamType,bool loadchild)
{
	if(!m_dataClass.valid())
		m_dataClass = CREncapsulation::loadData(getAbstractItem()->getScript());
	if(m_dataClass.valid())
	{
		if(m_abstractItem.valid())
		{
			doEvent(WCH_LoadDataAttrFromTab);
		}
		crStreamBuf *itemstream = getStream();
		if(itemstream)
		{
			switch (streamType)
			{
			case SaveStream:
				m_dataClass->inputParam(WCHDATA_DataStream,itemstream);
				break;
			case PlayerStream:
				m_dataClass->inputParam(WCHDATA_PlayerStream,itemstream);
				break;
			case NetStream:
				m_dataClass->inputParam(WCHDATA_NetStream,itemstream);
				break;
			}
		}
		m_dataClass->excHandle(MAKEINT64(WCH_InitData,this));
		setStream(NULL);
		if(m_itemtype == Npc || m_itemtype == Role)
		{
			if(!dynamic_cast<crRoleData *>(m_dataClass.get()))
			{
				std::string str = "crInstanceItem::loadItemData Error 该NPC载入的脚本不正确，itemid = "+crArgumentParser::appItoa(getInstanceItemID());
				CRCore::notify(CRCore::FATAL)<<str<<std::endl;
				//sprintf(gDebugInfo->buf(),"crInstanceItem::loadItemData Error 该NPC载入的脚本不正确，itemid = %d\n\0",getInstanceItemID());
				//gDebugInfo->debugInfo(CRCore::FATAL);
				//MessageBox(::GetDesktopWindow(),str.c_str(),"Creature3D",MB_OK);
				m_itemtype = instanceitem;
			}
		}
		//else
		//{
		//	if(dynamic_cast<crRoleData *>(m_dataClass.get()))
		//	{
		//		std::string str = "crInstanceItem::loadItemData Error 该Item载入的脚本不正确，itemid = "+crArgumentParser::appItoa(getInstanceItemID());
		//		CRCore::notify(CRCore::FATAL)<<str<<std::endl;
		//		//sprintf(gDebugInfo->buf(),"crInstanceItem::loadItemData Error 该Item载入的脚本不正确，itemid = %d\n\0",getInstanceItemID());
		//		//gDebugInfo->debugInfo(CRCore::FATAL);
		//		//MessageBox(::GetDesktopWindow(),str.c_str(),"Creature3D",MB_OK);
		//		m_itemtype = Npc;
		//	}
		//}
	}
	else
	{
		std::string str = "crInstanceItem::loadItemData Error abstractid = "+crArgumentParser::appItoa(getAbstractItemID())+" itemid = "+crArgumentParser::appItoa(getInstanceItemID());
		CRCore::notify(CRCore::FATAL)<<str<<std::endl;
		//sprintf(gDebugInfo->buf(),"crInstanceItem::loadItemData Error abstractid = %d itemid = %d\n\0",getAbstractItemID(),getInstanceItemID());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		//MessageBox(::GetDesktopWindow(),str.c_str(),"Creature3D",MB_OK);
	}
	if(loadchild)
		loadChildItemData(streamType,loadchild);
}
void crInstanceItem::loadChildItemData(char streamType,bool loadchild)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	crInstanceItem *item;
	for( ChildItemMap::iterator itr = m_childItemMap.begin();
		 itr != m_childItemMap.end();
		 ++itr )
	{
		item = itr->second->getInstanceItem();
		item->loadItemData(streamType,loadchild);
	}
}
void crInstanceItem::childItemQuery(crDataBase *instanceSession)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	ref_ptr<crInstanceItemQueryData> instanceItemQuery;
	CRCore::ref_ptr<crItemChild> itemChild;
	CRCore::ref_ptr<crInstanceItem> instanceItem;
	int abstractid = 0;
	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
		itr != m_childItemMap.end();
		++itr )
	{
		instanceItem = NULL;
		instanceItemQuery = new crInstanceItemQueryData;
		instanceItemQuery->buildQuerySql(itr->second->getInstanceItemID());
		instanceSession->executeQuery(instanceItemQuery.get());
		crDataBase::QueryResultVec &instanceItemQueryResultVec = instanceSession->getQueryResult();
		if(!instanceItemQueryResultVec.empty())
		{
			instanceItemQuery = dynamic_cast<crInstanceItemQueryData *>(instanceItemQueryResultVec[0].get());
			instanceItem = new crInstanceItem;
			instanceItem->setInstanceItemID(itr->second->getInstanceItemID());
			abstractid = instanceItemQuery->getAbstractItemID();
			//instanceItem->setAbstractItemID(instanceItemQuery->getAbstractItemID());
			instanceItem->setItemtype(instanceItemQuery->getItemtype());
			instanceItem->setIName(instanceItemQuery->getIName());
			instanceItem->setSceneID(instanceItemQuery->getSceneID());
			instanceItem->setLayerID(instanceItemQuery->getLayerid());
			instanceItem->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
			instanceItem->setZoffset(instanceItemQuery->getZoffset());
			instanceItem->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
			instanceItem->setStream(instanceItemQuery->getStream());
			itr->second->setInstanceItem(instanceItem.get());
		}
		instanceSession->releaseQuery();

		if(instanceItem.valid())
		{
			crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
			instanceItem->setAbstractItem(aitem);
			//itemchild查询
			ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
			itemChildQuery->buildQuerySql(instanceItem->getInstanceItemID());
			instanceSession->executeQuery(itemChildQuery.get());
			crDataBase::QueryResultVec &itemChildQueryResultVec = instanceSession->getQueryResult();
			if(!itemChildQueryResultVec.empty())
			{
				for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
					citr != itemChildQueryResultVec.end();
					++citr )
				{
					itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
					itemChild = new crItemChild;
					itemChild->setItemChildID(itemChildQuery->getID());
					itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
					itemChild->setParentItem(instanceItem.get());
					instanceItem->insertChildItem(itemChild.get());
				}
			}
			instanceSession->releaseQuery();
			//ChildItem查询
			instanceItem->childItemQuery(instanceSession);
		}
	}
}
crItemChild *crInstanceItem::findChildItem(int itemid)
{
	GNE::LockMutex lock( m_childItemMapMutex );
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
		itr != m_childItemMap.end();
		++itr )
	{
		itemChild = itr->second.get();
		instanceItem = itemChild->getInstanceItem();
		if(instanceItem->getInstanceItemID() == itemid)
		{
			return itemChild;
		}
		instanceItem->findChildItem(itemid);
	}

	return NULL;
}
crItemChild *crInstanceItem::findChildItemByAbstractID(int abstractid)
{
    GNE::LockMutex lock( m_childItemMapMutex );
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
		itr != m_childItemMap.end();
		++itr )
	{
		itemChild = itr->second.get();
		instanceItem = itemChild->getInstanceItem();
		if(instanceItem->getAbstractItemID() == abstractid)
		{
			return itemChild;
		}
		instanceItem->findChildItemByAbstractID(abstractid);
	}
	return NULL;
}
void crInstanceItem::insertTemporaryItem( crInstanceItem *item )
{
    GNE::LockMutex lock( m_temporaryItemMapMutex );
	m_temporaryItemMap[item->getInstanceItemID()] = item;
}
crInstanceItem *crInstanceItem::findTemporaryItem(int itemid)
{
    GNE::LockMutex lock( m_temporaryItemMapMutex );
	TemporaryItemMap::iterator itr = m_temporaryItemMap.find(itemid);
	return itr != m_temporaryItemMap.end()? itr->second.get() : NULL;
}
void crInstanceItem::removeTemporaryItem(int itemid)
{
    GNE::LockMutex lock( m_temporaryItemMapMutex );
    m_temporaryItemMap.erase(itemid);
}
crInstanceItem *crInstanceItem::serverGetTarget()
{
	void *param;
	crData *thisData = this->getDataClass();
	thisData->getParam(WCHDATA_TargetType,param);
	unsigned char targetType = *(unsigned char *)param;
	if(targetType & Target_Self)
	{
		return this;
	}
	if(targetType & Target_Instance || targetType & Target_StaticNpc/* || targetType & Target_StaticItem*/)
	{
		thisData->getParam(WCHDATA_TargetID,param);
		int targetid = *(int *)param;
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServer->getNetDataManager();
		if(targetType == Target_Role)
		{
			thisData->getParam(WCHDATA_TargetRoleID,param);
			int targetroleid = *(int *)param;
			if(targetid>0)
			{
				ref_ptr<crSceneServerPlayerData> targetPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(targetid));
				if(targetPlayerData.valid())
				{
					return targetPlayerData->getRole(targetroleid);
				}
			}
			else
			{
				crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = callback->findScene(m_sceneid);
				return scene?scene->findSceneItem(targetid,m_roomid):NULL;
			}
		}
		else if(targetType == Target_Npc || targetType == Target_Item || targetType == Target_StaticNpc/* || targetType == Target_StaticItem*/)
		{
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crScene *scene = callback->findScene(m_sceneid);
			return scene?scene->findSceneItem(targetid,m_roomid):NULL;
		}
	}
	return NULL;
}
void crInstanceItem::clientGetTarget(CRCore::ref_ptr<crInstanceItem> &outItem,CRCore::ref_ptr<crMatrixTransform>&outNode)
{
	//GNE::LockMutex lock( m_eventMutex );
	crGroup *group = NULL;
	void *param;
	crData *thisData = this->getDataClass();
	thisData->getParam(WCHDATA_TargetType,param);
	unsigned char targetType = *(unsigned char *)param;
	if(targetType & Target_Self)
	{
		outItem = this;
		outNode = dynamic_cast<crMatrixTransform *>(outItem->getRelNode());
	}
	else if(targetType & Target_Instance || targetType & Target_StaticNpc/* || targetType & Target_StaticItem*/)
	{
		thisData->getParam(WCHDATA_TargetID,param);
		int targetid = *(int *)param;
		if(targetType == Target_Role)
		{
			thisData->getParam(WCHDATA_TargetRoleID,param);
			int targetroleid = *(int *)param;
			if(targetid == crMyPlayerData::getInstance()->getPlayerID())
			{
				outItem = crMyPlayerData::getInstance()->getRole(targetroleid);
			}
			else
			{
				crMyPlayerData::RoleNpcPair roleNpcPair;
				if(crMyPlayerData::getInstance()->findInRangePlayer(targetid,targetroleid,roleNpcPair))
				{
					outItem = roleNpcPair.first.get();
				}
				//else if(crMyPlayerData::getInstance()->findOutRangePlayer(targetid,targetroleid,roleNpcPair))
				//{
				//	outItem = roleNpcPair.first.get();
				//}
			}
			if(outItem.valid())
			{
				outNode = dynamic_cast<crMatrixTransform *>(outItem->getRelNode());
			}
		}
		else if(targetType == Target_Npc || targetType == Target_StaticNpc)
		{
			crMyPlayerData::ItemNpcPair itemNpcPair;
			if(crMyPlayerData::getInstance()->findInRangeNpc(targetid,itemNpcPair))
			{
				outItem = itemNpcPair.first.get();
			}
			//else if(crMyPlayerData::getInstance()->findOutRangeNpc(targetid,itemNpcPair))
			//{
			//	outItem = itemNpcPair.first.get();
			//}
			if(outItem.valid())
			{
				outNode = dynamic_cast<crMatrixTransform *>(outItem->getRelNode());
			}
		}
		else if(targetType == Target_Item/* || targetType == Target_StaticItem*/)
		{
			crMyPlayerData::ItemNodePair itemNodePair;
			if(crMyPlayerData::getInstance()->findInRangeItem(targetid,itemNodePair))
			{
				outItem = itemNodePair.first.get();
			}
			//else if(crMyPlayerData::getInstance()->findOutRangeItem(targetid,itemNodePair))
			//{
			//	outItem = itemNodePair.first.get();
			//}
			if(outItem.valid())
			{
				outNode = dynamic_cast<crMatrixTransform *>(outItem->getRelNode());
			}
			//if(outItem.valid())
			//	outNode = dynamic_cast<crMatrixTransform *>(outItem->getNode());
		}
	}
}

void crInstanceItem::setRoomID(int roomid)
{
	m_roomid = roomid;
}
int crInstanceItem::getRoomID()
{
	return m_roomid;
}
void crInstanceItem::clientRelease()
{
	//GNE::LockMutex elock( m_eventMutex );
	crNode *relNode;
	crData *data;
	relNode = getRelNode();
	if(relNode)
	{
		data = relNode->getDataClass();
		if(data)
		{
			data->inputParam(WCHDATA_Item,NULL);
		}
	}
	m_node = NULL;

	GNE::LockMutex lock( m_childItemMapMutex );
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
		itr != m_childItemMap.end();
		++itr )
	{
		itemChild = itr->second.get();
		instanceItem = itemChild->getInstanceItem();
		relNode = instanceItem->getRelNode();
		if(relNode)
		{
			data = relNode->getDataClass();
			if(data)
			{
				data->inputParam(WCHDATA_Item,NULL);
			}
		}
		instanceItem->setNode(NULL);
		instanceItem->setDataClass(NULL);
		instanceItem->clientRelease();
	}

	{
		GNE::LockMutex lock( m_temporaryItemMapMutex );
		m_temporaryItemMap.clear();
	}

	{
		//CRCore::ScopedLock<CRCore::crMutex> lock(m_createItemChildMutex);
		m_createItemChildVec.clear();
	}
}
//void crInstanceItem::clientReload()
//{
//	GNE::LockMutex lock( m_childItemMapMutex );
//	crItemChild* itemChild;
//	crInstanceItem *instanceItem;
//	crInstanceItem *rootItem = this;
//	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
//		itr != m_childItemMap.end();
//		++itr )
//	{
//		itemChild = itr->second.get();
//		instanceItem = itemChild->getInstanceItem();
//		instanceItem->clientLoadItemChildNode(rootItem);
//		instanceItem->clientReloadItemChild(rootItem);
//	}
//}
//void crInstanceItem::clientReloadItemChild(crInstanceItem *rootItem)
//{
//	GNE::LockMutex lock( m_childItemMapMutex );
//	crItemChild* itemChild;
//	crInstanceItem *instanceItem;
//	for( crInstanceItem::ChildItemMap::iterator itr = m_childItemMap.begin();
//		itr != m_childItemMap.end();
//		++itr )
//	{
//		itemChild = itr->second.get();
//		instanceItem = itemChild->getInstanceItem();
//		instanceItem->clientLoadItemChildNode(rootItem);
//		instanceItem->clientReloadItemChild(rootItem);
//	}
//}
void crInstanceItem::clientLoadItemChildNode(crInstanceItem *rootItem)
{
	CRCore::ref_ptr<crGroup> group = new crGroup;
	//group->setCullingActive(false);
	group->setName(getIName());
	const std::string &filename = this->getAbstractItem()->getFileName();
	if(!filename.empty())
	{
		CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
		CRIOManager::crLoadManager::getInstance()->requestNode(filename,proxyNode.get(),false);
		if(proxyNode->getNumChildren()==1)
		{
			crNode *loadedNode = proxyNode->getChild(0);
			loadedNode = dynamic_cast<crNode *>(loadedNode->clone(crCopyOp::DEEP_COPY_NODES));
			group->addChild(loadedNode);
		}
		//CRIOManager::crLoadManager::getInstance()->requestNode(filename,group.get(),false);
	}
	const std::string &resScript = this->getAbstractItem()->getResScript();
	if(!resScript.empty())
	{//group->getChild(0) = weaponMatterObject; group->getChild(1) = bulletMatterObject
		CREncapsulation::loadSkillCfg(resScript,group.get());
		crData *data;
		for(int i = 0; i<group->getNumChildren(); ++i)
		{
			data = group->getChild(i)->getDataClass();
			if(data)
			{
				data->inputParam(WCHDATA_Item,this);
			}
		}
	}
	this->setNode(group.get());
	CRIOManager::crLoadManager::getInstance()->requestAddNode(dynamic_cast<crGroup *>(rootItem->getNode()),group.get(),true);
}
crInstanceItem::crPathFinder *crInstanceItem::getOrCreatePathFinder()
{
    if(!m_pathFinder.valid())
		m_pathFinder = new crPathFinder;
	return m_pathFinder.get();
}
void crInstanceItem::setPathFinder(crPathFinder *finder)
{
	m_pathFinder = finder;
}
crInstanceItem::crPathFinder *crInstanceItem::getPathFinder()
{
	return m_pathFinder.get();
}
void crInstanceItem::recordItemState()
{
    crData *data = getDataClass();
	void *param;
	data->getParam(WCHDATA_ItemState,param);
	m_itemstate_record = *(unsigned char *)param;
}
bool crInstanceItem::getBackItemState()
{
	crData *data = getDataClass();
	void *param;
	data->getParam(WCHDATA_ItemState,param);
	unsigned char itemstate = *(unsigned char *)param;
	if(m_itemstate_record != itemstate)
	{
		data->inputParam(WCHDATA_ItemState,&m_itemstate_record);
		m_itemstate_record = IS_None;
		return true;
	}
	m_itemstate_record = IS_None;
	return false;
}
void crInstanceItem::setTransportSceneID(int sceneid)
{
	m_transportsceneid = sceneid;
}
int crInstanceItem::getTransportSceneID()
{
	return m_transportsceneid;
}
//#define interpolateCoord(from,to,coords) \
//	while (from!=to)\
//	{\
//		if(from[0]<to[0]) from[0]+=1;\
//			else if(from[0]>to[0]) from[0]-=1;\
//			if(from[1]<to[1]) from[1]+=1;\
//			else if(from[1]>to[1]) from[1]-=1;\
//			coords.insert(from);\
//	}
bool crInstanceItem::testNewPos(crScene *scene,const CRCore::crVector2 &newPos,CRCore::crVector3 dir,float &height)
{
	//  D___C
	//	|   |
	//	A___B
	//crVector2 center = newPos;
	void *param;
	crData *data = getDataClass();
	data->getParam(WCHDATA_HalfPhysicsBox,param);
	crVector3 halfPhysicsBox = *((crVector3*)(param));
	crVector2 A = crVector2(-halfPhysicsBox[0],-halfPhysicsBox[1]);
	crVector2 B = crVector2( halfPhysicsBox[0],-halfPhysicsBox[1]);
	crVector2 C = crVector2( halfPhysicsBox[0], halfPhysicsBox[1]);
	crVector2 D = crVector2(-halfPhysicsBox[0], halfPhysicsBox[1]);

	CRCore::crMatrix rotMat;
	if(dir == crVector3(0.0f,0.0f,0.0f))
	{
		dir[1] = -1.0f;
	}

	if(dir[1] > 0.999)
	{
		rotMat.makeRotate(-CRCore::Y_AXIS,dir);
		CRCore::crVector3d ep,center,up;
		rotMat.getLookAt(ep,center,up);
		if(center[2]>0.0f)
		{
			center = -center;
		}
		rotMat.makeLookAt(ep,center,up);
	}
	else
		rotMat.makeRotate(-CRCore::Y_AXIS,dir);
	rotMat.setTrans(newPos[0],newPos[1],0.0f);
	crVector3 AA = crVector3(A[0],A[1],0.0f) * rotMat;
	crVector3 BB = crVector3(B[0],B[1],0.0f) * rotMat;
	crVector3 CC = crVector3(C[0],C[1],0.0f) * rotMat;
	crVector3 DD = crVector3(D[0],D[1],0.0f) * rotMat;

	crSceneLayer *sceneLayer = scene->getSceneLayer(m_layerid);
	crVector2s a = sceneLayer->getCoord(AA[0],AA[1]);
	crVector2s b = sceneLayer->getCoord(BB[0],BB[1]);
	crVector2s c = sceneLayer->getCoord(CC[0],CC[1]);
	crVector2s d = sceneLayer->getCoord(DD[0],DD[1]);
	std::set<CRCore::crVector2s>coordSet;
	coordSet.insert(a);
	coordSet.insert(b);
	coordSet.insert(c);
	coordSet.insert(d);

	CRCore::crVector2 mapInfo;
	bool walkAble = true;
	crInstanceItem *collideItem;
	float z = (m_posz - getZoffset()) * crGlobalHandle::gData()->gUnitScale();
	for( std::set<crVector2s>::iterator itr = coordSet.begin();
		itr != coordSet.end();
		++itr )
	{
		mapInfo = sceneLayer->getCollideInfoEx(*itr,z);
		walkAble = sceneLayer->walkability(mapInfo[0]);
		if(!walkAble)
		{
			break;
		}
	}
	if(walkAble)
	{
		void *param;
		crData *data = getDataClass();
		data->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *(unsigned char *)param;
		if(itemstate != IS_Relive && itemstate != IS_ItemLoad && itemstate != IS_Dead)
		{
			if(crGlobalHandle::isClient())
			{
				collideItem = crMyPlayerData::getInstance()->dynamicCollideTest(this,newPos);
			}
			else
			{
				collideItem = scene->dynamicCollideTest(this,newPos);
				if(!collideItem)
				{//test player
					crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
					crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
					crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
					collideItem = netCallback->dynamicCollideTest(this,newPos);
				}
			}
			if(collideItem)
			{
				return false;
			}
		}
		crVector2s o = sceneLayer->getCoord(newPos[0],newPos[1]);
		mapInfo = sceneLayer->getCollideInfoEx(o,z);
		height = mapInfo[1];
		//{
		//	GNE::LockMutex lock( m_itemCoordSetMutex );
		//	m_itemCoordSet = coordSet;
		//}
	}
	return walkAble;
}
bool crInstanceItem::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos)
{
	if(!item || !item->getDataClass() || this == item)
		return false;
	//GNE::LockMutex lock( m_itemCoordSetMutex );
	//return m_itemCoordSet.find(coord) != m_itemCoordSet.end();
	float scale = crGlobalHandle::gData()->gUnitScale();
	if(fabs(m_posz-item->getPosz())*scale>=crGlobalHandle::gData()->gLayerHeight())
		return false;
	//void *param;
	unsigned int guisestate1 = GS_Normal;
	doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate1,NULL));
	if(guisestate1 & GS_UnVisiable || guisestate1 & GS_StaticUnVisiable || guisestate1 & GS_StaticNoneBlock)
	{
		return false;
	}
	unsigned int guisestate2 = GS_Normal;
	item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate2,NULL));
	if(guisestate2 & GS_UnVisiable || guisestate2 & GS_StaticUnVisiable || guisestate2 & GS_StaticNoneBlock)
	{
		return false;
	}
	if(guisestate1 & GS_NoneBlock ||  guisestate2 & GS_NoneBlock)
	{
		//char isEnemy = 0;
		//doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
		//if(isEnemy == 1 )
		return false;
	}
	////友军不阻挡
	//char isEnemy = 0;
	//doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
	//if (isEnemy == 1)
	//	return false;
	void *param;
	ref_ptr<crData> data;
	if(!(guisestate1 & GS_Static))
	{//非静态建筑死亡可穿
		data = getDataClass();
		if(data.valid())
		{
			data->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				return false;
		}
	}
	if(!(guisestate2 & GS_Static))
	{//非静态建筑死亡可穿
		data = item->getDataClass();
		if(data.valid())
		{
			data->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				return false;
		}
	}
	//if(param)
	//{
	//	guisestate = *(unsigned char *)param;
	//	if(guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable || guisestate == GS_NoneBlock || guisestate == GS_StaticNoneBlock)
	//	{
	//		return false;
	//	}
	//}
	//crData *itemData = item->getDataClass();
	//itemData->getParam(WCHDATA_GuiseState,param);
	//if(param)
	//{
	//	guisestate = *(unsigned char *)param;
	//	if(guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable || guisestate == GS_NoneBlock || guisestate == GS_StaticNoneBlock)
	//	{
	//		return false;
	//	}
	//}
	float mindist = getMinDistance(item);
	crVector2 thisPos(m_posx*scale,m_posy*scale);
	float dist1 = (thisPos - newPos).length();
	float dist2 = 0.0f;
	crVector2 syncPos;
	if(getFrontSyncPos(syncPos))
	{
		dist2 = (syncPos - newPos).length();
	}
	return dist1<mindist && dist2<mindist;
}
//bool crInstanceItem::dynamicCollideTest(const crVector2 &coordPos,crInstanceItem *item)
//{
//	if(this == item)
//		return false;
//	void *param;
//	crData* data = getDataClass();
//	data->getParam(WCHDATA_GuiseState,param);
//	if(param)
//	{
//		unsigned char guisestate = *(unsigned char *)param;
//		if(guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable)
//		{
//			return false;
//		}
//	}
//
//	float scale = crGlobalHandle::gData()->gUnitScale();
//
//	float mindist = getMinDistance(item);
//	crVector2 thisPos(m_posx*scale,m_posy*scale);
//	float dist = (thisPos - coordPos).length();
//	return dist<mindist;
//	//crVector2s coord = layer->getCoord(pos);
//	//if(coord[0] == x && coord[1] == y)
//	//	return true;
//	//return false;
//}
float crInstanceItem::getMinDistance(crInstanceItem *item)
{
	float radius = 0.0f;
	doEvent(MAKEINT64(WCH_GetRadius,0),MAKEINT64(&radius,NULL));
	if(item)
	{
		float itemRadius = 0.0f;
		item->doEvent(MAKEINT64(WCH_GetRadius,0),MAKEINT64(&itemRadius,NULL));
		radius += itemRadius;
	}
	return radius;
}
float crInstanceItem::getAttackDistance(crInstanceItem *item)
{
	//void *param;
	float dist = getMinDistance(item);
	float extraDist = 0;
	doEvent(MAKEINT64(WCH_GetAttackDist,0),MAKEINT64(&extraDist,NULL));
	return maximum(dist + extraDist,1.0f);
}
bool crInstanceItem::isCoordInItem(const CRCore::crVector2 &coordPos)
{
	//float scale = crGlobalHandle::gData()->gUnitScale();
	//crVector2 pos(m_posx * scale,m_posy * scale);
	//crVector2s coord = layer->getCoord(pos);
	//if(coord[0] == x && coord[1] == y)
	//	return true;
	//return false;

	float scale = crGlobalHandle::gData()->gUnitScale();
	//void *param;
	//crData* data = getDataClass();
	//data->getParam(WCHDATA_HalfLengths,param);
	//crVector2 thisHalfLengths = *((crVector2*)(param));
	//float mindist = thisHalfLengths.length();
	float mindist = getMinDistance(NULL);
	crVector2 thisPos(m_posx*scale,m_posy*scale);
	float dist = (thisPos - coordPos).length();
	return dist<mindist;
}
void crInstanceItem::addSceneFxNode(CRCore::crNode *node)
{
	GNE::LockMutex lock(m_sceneFxNodeArrayMutex);
	m_sceneFxNodeArray.push_back(node);
}
void crInstanceItem::lockSceneFxArray()
{
	m_sceneFxNodeArrayMutex.acquire();
}
crInstanceItem::SceneFxNodeArray &crInstanceItem::getSceneFxNodeArray()
{
	return m_sceneFxNodeArray;
}
void crInstanceItem::unlockSceneFxArray()
{
	m_sceneFxNodeArrayMutex.release();
}
CRCore::crNode *crInstanceItem::findSceneFxNode(const std::string &name)
{
	GNE::LockMutex lock(m_sceneFxNodeArrayMutex);
	for( SceneFxNodeArray::iterator itr = m_sceneFxNodeArray.begin();
		 itr != m_sceneFxNodeArray.end();
		 ++itr )
	{
		if((*itr)->getName() == name)
			return itr->get();
	}
	return NULL;
}
void crInstanceItem::setSightInfo(crSightInfo *sight)
{
	m_sightInfo = sight;
}
bool crInstanceItem::isMainAI()
{
	bool mainai = false;
	if(crGlobalHandle::isClient())
	{
		if(crMyPlayerData::getInstance()->ifItemIsMe(this))
		{
			mainai = true;
		}
	}
	else if(getItemtype() != crInstanceItem::Role)
	{
		mainai = true;
	}
	return mainai;
}
CRNetApp::crScene *crInstanceItem::getScene()
{
	CRNetApp::crScene *scene = NULL;
	if(crGlobalHandle::isClient()) 
	{
		scene = crMyPlayerData::getInstance()->getScene();
	}
	else
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		scene = netCallback->findScene(getSceneID());
	}
	return scene;
}
CRNetApp::crSceneLayer *crInstanceItem::getSceneLayer()
{
	CRNetApp::crScene *scene = getScene();
	if(scene)
	{
		return scene->getSceneLayer(m_layerid);
	}
	return NULL;
}
float crInstanceItem::getPosZ(float posx,float posy)
{
	CRNetApp::crScene *scene = getScene();
	if(scene)
	{
		return scene->getPosZ(m_layerid,posx,posy,getZoffset()*crGlobalHandle::gData()->gUnitScale());
	}
	return 0.0f;
}
//bool crInstanceItem::intersect(const CRCore::crVector2s &coord)
//{//判断点在多边形内
//	GNE::LockMutex lock( m_itemCoordVecMutex );
//	if(m_itemCoordVec.empty())
//		return false;
//	for( ItemCoordVec::iterator itr = m_itemCoordVec.begin();
//		itr != m_itemCoordVec.end();
//		++itr )
//	{
//		if(*itr == coord)
//			return true;
//	}
//	int count = m_itemCoordVec.size();
//	if(count < 3)
//	{
//		return false;
//	}
//	else
//	{
//		int nCross = 0;
//		ItemCoordVec::iterator itr2;
//		crVector2s p1,p2;
//		for( ItemCoordVec::iterator itr1 = m_itemCoordVec.begin(); 
//			itr1 != m_itemCoordVec.end(); 
//			++itr1 )
//		{
//			itr2 = itr1++;
//			if(itr2 == m_itemCoordVec.end())
//				itr2 = m_itemCoordVec.begin();
//			p1 = *itr1;
//			p2 = *itr2;
//			// 求解 y=p.y 与 p1p2 的交点
//			if ( p1.y() == p2.y() ) // p1p2 与 y=p0.y平行 
//				continue;
//			if ( coord.y() < CRCore::minimum(p1.y(), p2.y()) ) // 交点在p1p2延长线上 
//				continue; 
//			if ( coord.y() >= CRCore::maximum(p1.y(), p2.y()) ) // 交点在p1p2延长线上 
//				continue;
//			// 求交点的 X 坐标 -------------------------------------------------------------- 
//			float x = (float)(coord.y() - p1.y()) * (float)(p2.x() - p1.x()) / (float)(p2.y() - p1.y()) + p1.x();
//			if ( x >= (float)(coord.x()) ) 
//				nCross++; // 只统计单边交点 
//		}
//		// 单边交点为偶数，点在多边形之外 --- 
//		return (nCross % 2 == 1); 
//	}
//	return false;
//}
//void crInstanceItem::readPathCollideUpdate(crScene *scene)
//{
//	void *param;
//	crData *data = getDataClass();
//	data->getParam(WCHDATA_HalfLengths,param);
//	crVector2 halfLengths = *((crVector2*)(param));
//	crVector2 A = crVector2(-halfLengths[0],-halfLengths[1]);
//	crVector2 B = crVector2( halfLengths[0],-halfLengths[1]);
//	crVector2 C = crVector2( halfLengths[0], halfLengths[1]);
//	crVector2 D = crVector2(-halfLengths[0], halfLengths[1]);
//
//	CRCore::crMatrix rotMat;
//	crVector3 dir(m_dirx,m_diry,m_dirz);
//	if(dir == crVector3(0.0f,0.0f,0.0f))
//	{
//		dir[1] = -1.0f;
//	}
//
//	if(dir[1] > 0.999)
//	{
//		rotMat.makeRotate(-CRCore::Y_AXIS,dir);
//		CRCore::crVector3d ep,center,up;
//		rotMat.getLookAt(ep,center,up);
//		if(center[2]>0.0f)
//		{
//			center = -center;
//		}
//		rotMat.makeLookAt(ep,center,up);
//	}
//	else
//		rotMat.makeRotate(-CRCore::Y_AXIS,dir);
//	float posx = m_posx * crGlobalHandle::gData()->gUnitScale();
//	float posy = m_posy * crGlobalHandle::gData()->gUnitScale();
//	rotMat.setTrans(posx,posy,0.0f);
//	crVector3 AA = crVector3(A[0],A[1],0.0f) * rotMat;
//	crVector3 BB = crVector3(B[0],B[1],0.0f) * rotMat;
//	crVector3 CC = crVector3(C[0],C[1],0.0f) * rotMat;
//	crVector3 DD = crVector3(D[0],D[1],0.0f) * rotMat;
//
//	crSceneLayer *sceneLayer = scene->getSceneLayer(m_layerid);
//	crVector2s a = sceneLayer->getCoord(AA[0],AA[1]);
//	crVector2s b = sceneLayer->getCoord(BB[0],BB[1]);
//	crVector2s c = sceneLayer->getCoord(CC[0],CC[1]);
//	crVector2s d = sceneLayer->getCoord(DD[0],DD[1]);
//	std::set<CRCore::crVector2s>coordSet;
//	coordSet.insert(a);
//	coordSet.insert(b);
//	coordSet.insert(c);
//	coordSet.insert(d);
//	crVector2s from,to;
//	from = a;
//	to = b;
//	interpolateCoord(from,to,coordSet);
//	from = b;
//	to = c;
//	interpolateCoord(from,to,coordSet);
//	from = c;
//	to = d;
//	interpolateCoord(from,to,coordSet);
//	from = d;
//	to = a;
//	interpolateCoord(from,to,coordSet);
//	crVector2s o = sceneLayer->getCoord(m_posx,m_posy);
//	coordSet.insert(o);
//	{
//		GNE::LockMutex lock( m_itemCoordSetMutex );
//		m_itemCoordSet = coordSet;
//	}
//}

//_crInt8 crInstanceItem::updateItemGeoInfo(crScene *scene)
//{
//	return 0;
//}
/////////////////////////////////////////////////////////////
///////////////////////////////////
//
//crItemChild
//
/////////////////////////////////////
crItemChild::crItemChild():
m_itemchildid(0),
m_instanceitemid(0),
m_parentItem(NULL)
{
}
crItemChild::~crItemChild()
{

}
crItemChild::crItemChild(const crItemChild& itemchild):
m_itemchildid(itemchild.m_itemchildid)
{
	m_instanceItem = dynamic_cast<crInstanceItem *>(itemchild.m_instanceItem->clone());
	m_instanceitemid = m_instanceItem->getInstanceItemID();
}
void crItemChild::setItemChildID(int id)
{
	m_itemchildid = id;
}
int crItemChild::getItemChildID()
{
	return m_itemchildid;
}
void crItemChild::setInstanceItemID(int instanceitemid)
{
    m_instanceitemid = instanceitemid;
}
int crItemChild::getInstanceItemID()
{
    return m_instanceitemid;
}
void crItemChild::setInstanceItem(crInstanceItem *item)
{
	m_instanceItem = item;
	if(item) setInstanceItemID(item->getInstanceItemID());
}
crInstanceItem *crItemChild::getInstanceItem()
{
	return m_instanceItem.get();
}
void crItemChild::setParentItem(crInstanceItem *item)
{
	m_parentItem = item;
}
crInstanceItem *crItemChild::getParentItem()
{
	return m_parentItem;
}
void crItemChild::clear()
{
	m_instanceItem = NULL;
}
///////////////////////////////////
//
//crInstanceItem::crPathFinder
//
/////////////////////////////////////
bool crInstanceItem::crPathFinder::findTargetInPath(CRCore::crVector2s target)
{
	GNE::LockMutex lock( m_pathMutex );
	while(!m_bestPath.empty())
	{
		if(m_bestPath.back() == target)
			return true;
		m_bestPath.pop_back();
	}
	return false;
}
void crInstanceItem::crPathFinder::setBestPath(CoordDeque &bestPath)
{
	GNE::LockMutex lock( m_pathMutex );
	m_bestPath.swap(bestPath);
	m_pathStatus = Found;
}
//CoordDeque &crInstanceItem::crPathFinder::getBestPath()
//{
//	return m_bestPath;
//}
bool crInstanceItem::crPathFinder::isBestPathEmpty()
{
	GNE::LockMutex lock( m_pathMutex );
	return m_bestPath.empty();
}
bool crInstanceItem::crPathFinder::dynamicChangePath(crInstanceItem *item,crScene *scene)
{
	GNE::LockMutex lock( m_pathMutex );
	bool success = false;
	if(m_pathStatus == Found && !m_bestPath.empty())
	{
		crSceneLayer *scenelayer = scene->getSceneLayer(item->getLayerID());
		crVector2s pathCoord,to;
		crVector2 path,startpath,steppath;
		crVector2 current(item->getPosx(),item->getPosy());
		current *= crGlobalHandle::gData()->gUnitScale();
		crVector2s from = scenelayer->getCoord(current);
		crVector2 dir;
		float length,delta;
		bool found = false;
		startpath = current;
		CoordDeque::iterator itr = m_bestPath.begin();
		for( ;
			itr != m_bestPath.end();
			++itr )
		{
			pathCoord = *itr;
			path = scenelayer->getPosXY(pathCoord);
			dir = path - startpath;
			length = dir.length();
			if(length>1.0f)
			{
				dir.normalize();
				for( delta = 1.0f; delta<length; delta+=1.0f )
				{
					steppath = startpath+dir*delta;
					if(!scene->dynamicCollideTest(steppath,item,scenelayer,IG_Me))
					{
						to = scenelayer->getCoord(steppath);
						found = true;
						break;
					}
				}
				if(found)
					break;
			}
			else
			{
				if(!scene->dynamicCollideTest(path,item,scenelayer,IG_Me))
				{
					to = pathCoord;
					found = true;
					break;
				}
			}
			startpath = path;
		}
		if(!found)
		{
			pathCoord = *(m_bestPath.rbegin());
			path = scenelayer->getPosXY(pathCoord);
			crVector3 newPos;
			found = scene->findNonblockPos(item,path,10,newPos,IG_Me);
			to = scenelayer->getCoord(crVector2(newPos[0],newPos[1]));
		}
		if(found)
		{//寻找从当前点到改点的带动态碰撞的新路径，如果找不到，则停止，并同步位置
			//crVector2s to = scenelayer->getCoord(crVector2(newPos[0],newPos[1]));
			unsigned char fingpath = m_dynamicPathFinding->findBestPath(item,from,to,scenelayer);
			if(fingpath == crInstanceItem::crPathFinder::Found)
			{
				CoordDeque &coordDeque = m_dynamicPathFinding->getBestPath();
				//if(!coordDeque.empty())coordDeque.pop_front();
				//if(!coordDeque.empty()) m_bestPath.swap(coordDeque);
				//if(itr != m_bestPath.end() && !coordDeque.empty() && coordDeque.back() != *itr)
				//{
				//	coordDeque.push_back(*itr);
				//}
				m_bestPath.erase(m_bestPath.begin(),itr);
				m_bestPath.insert(m_bestPath.begin(),coordDeque.begin(),coordDeque.end());
				success = true;
			}
			else
			{//
				//CRCore::notify(CRCore::ALWAYS)<<"FindDynamicPath 没有找到路径 From:"<<from<<"To:"<<pathCoord<<" 消耗值:"<<m_dynamicPathFinding->getClosePathCount()<<std::endl;
				m_dynamicwait = crGlobalHandle::gData()->gDynamicTryWait();
				//char gbuf[256];
				//sprintf(gbuf,"%s\n\0",argvstr.c_str());
				//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			}
			m_dynamicPathFinding->reset();
		}
		else
		{
			m_dynamicwait = crGlobalHandle::gData()->gDynamicTryWait();
			//CRCore::notify(CRCore::ALWAYS)<<"FindDynamicPath 目标不可通过,等待中"<<std::endl;
		}
		//if(!success)
		//{
		//	if(!isPortPathEmpty())
		//	{
		//		nextPortPath(item,scene);
		//	}
		//	else if(!isWaypointEmpty())
		//	{
		//		nextWaypoint(item,scene);
		//	}
		//	else
		//	{
		//		void *param;
		//		item->getDataClass()->getParam(WCHDATA_ItemState,param);
		//		unsigned char itemstate = *(unsigned char *)param;
		//		if(itemstate != IS_CloseTarget)
		//		{
		//			itemstate = IS_AttackToPos;
		//			item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
		//		}
		//		dirty();
		//	}
		//}

	}
	return success;
}
bool crInstanceItem::crPathFinder::readPath(crInstanceItem *item,crScene *scene,float& distance,float dt)
{
	GNE::LockMutex lock( m_pathMutex );
	if(m_pathStatus == Found || m_pathStatus == ReFound)
	{
		if(!m_bestPath.empty())
		{
			crSceneLayer *scenelayer = scene->getSceneLayer(item->getLayerID());
			//CRCore::notify(CRCore::ALWAYS)<<"readPath(): distance = "<<distance<<std::endl;
			crVector2 current(item->getPosx(),item->getPosy());
			float scale = crGlobalHandle::gData()->gUnitScale();
			current *= scale;
			//float halfdlength = scenelayer->getIntervalLength() * 0.5f;

			crVector2s pathCoord;
			pathCoord = m_bestPath.front();
			crVector2 path;
			path = scenelayer->getPosXY(pathCoord);
			crVector2 dir = path - current;
			float griddist = dir.length();
			dir.normalize();
			//CRCore::notify(CRCore::ALWAYS)<<"ReadPath(): "<<pathCoord;
			//
			crVector2 newPos = current;
			//crVector2 dir = path - newPos;
			//float griddist = dir.length();
			if(griddist == 0.0f)
			{
				m_bestPath.pop_front();
				return readPath(item,scene,distance,dt);
			}
			//dir.normalize();
			float z;
			crVector2 info;
			//bool test = false;
			if(distance >= griddist)
			{//移动到该grid
				newPos = path;
				m_bestPath.pop_front();
				info = scenelayer->getCollideInfo(pathCoord[0], pathCoord[1]);
				z = info[1];
				distance -= griddist;
				//test = true;
				//CRCore::notify(CRCore::ALWAYS)<<item->getRoleID()<<" Pop:"<<pathCoord<<" 剩余"<<m_bestPath.size()<<" Ports:"<<m_portPathDeque.size()<<std::endl;
			}
			else
			{//计算newPos
				//if(/*crGlobalHandle::isClient() || */item->isMainAI())
				if(crGlobalHandle::isClient())
				{
					//加直线平滑，动态寻路使用thisPath
					//crVector2 thisPath = current + dir * scenelayer->getXInterval();
					m_dynamicblocktime -= dt;
					m_dynamicwait -= dt;
					if(m_dynamicblocktime<= 0.0f)
					{
						m_dynamicblocktime = 0.0f;
						crVector2 thisPath = current + dir * scenelayer->getXInterval();
						CollideItemSet collideItemSet;
						bool block = scene->dynamicCollideTestAll(thisPath, item, scenelayer, collideItemSet);//IG_Me
						//bool block = scene->dynamicCollideTestAll(path, item, scenelayer, collideItemSet);
						if(block)
						{
							//前面被动态物体挡住，等待
							//向前寻找一个可以去的点
							ref_ptr<crInstanceItem> blockItem;
							//unsigned int guisestate = GS_Normal;
							for( CollideItemSet::iterator itr = collideItemSet.begin();
								itr != collideItemSet.end();
								++itr )
							{
								blockItem = *itr;
								blockItem->doEvent(WCH_DynamicCollide,MAKEINT64(item,NULL));
								//item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
								//if(guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable || guisestate & GS_NoneBlock )
								//{
								//	distance = 0.0f;
								//	m_dynamicblocktime = 0.5f;
								//	return false;
								//}
							}

							bool canpass = false;
							if(m_dynamicwait <= 0.0f)
							{
								m_dynamicwait = 0.0f;

								if(dynamicChangePath(item,scene))
									canpass = true;

								//if(!dynamicChangePath(item,scene))
								//{
								//	//item->doEvent(WCH_DynamicBlock,MAKEINT64(blockItem,NULL));
								//}

							}
							if(!canpass)
							{
								char isEnemy = 0;
								bool hasEnemy = false;
								for( CollideItemSet::iterator itr = collideItemSet.begin();
									itr != collideItemSet.end();
									++itr )
								{
									blockItem = *itr;
									item->doEvent(WCH_EnemyCheck,MAKEINT64(blockItem.get(),&isEnemy));
									if(isEnemy != 1)
									{
										hasEnemy = true;
										break;
									}
								}
								if(!hasEnemy)
									canpass = true;
								//else
								//{//忽略友军动态寻路

								//}
							}
							if(!canpass)
							{
								distance = 0.0f;
								m_dynamicblocktime = 0.5f;
								return false;
							}
						}
					}
					else
					{
						distance = 0.0f;
						return false;
					}
					//m_dynamicblocktime = 0.0f;
					//m_dynamicwait = 0.0f;
				}

				newPos = newPos + dir * distance;
				crVector2s pos = scenelayer->getCoord(newPos[0],newPos[1]);
				z = scenelayer->getHeight(pos[0],pos[1]);
				//info = scenelayer->getCollideInfo(pos[0], pos[1]);
				//if(scenelayer->walkability(info[0]))
				//	z = info[1];
				//else
				//{//路径错误
				////	//void *param;
				////	//item->getDataClass()->getParam(WCHDATA_ItemState,param);
				////	//unsigned char itemstate = *(unsigned char *)param;
				////	//if(itemstate != IS_CloseTarget)
				////	//{
				////	//	itemstate = IS_Stop;
				////	//	item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
				////	//}
				////	//m_pathStatus = NotStarted;
				////	//distance = 0.0f;
				////	dynamicChangePath(item,scene);
				//	CRCore::notify(CRCore::ALWAYS)<<" 路径错误: pos:"<<pos<<"distance:"<<distance<<"griddist"<<griddist<<std::endl;
				//	distance = 0.0f;
				//	return false;
				//}
				distance = 0.0f;
			}
			newPos/=scale;
			z /= scale;
			z += item->getZoffset();
			item->setPosxy(newPos[0],newPos[1]);
			//if(test)
			//{取整造成卡在地图边缘问题
			//	CRCore::crVector2s coord = scenelayer->getCoord(newPos[0] * crGlobalHandle::gData()->gUnitScale(),newPos[1] * crGlobalHandle::gData()->gUnitScale());
			//	if(pathCoord != coord)
			//	{
			//		int i = 0;
			//	}
			//}
			//item->setPosy(newPos[1]);
			item->setPosz(z);
			item->setTargetDir(crVector3(dir[0],dir[1],0.0f));
			//CRCore::notify(CRCore::FATAL)<<"crPathFinder::readPath item "<<item->getID()<<" distance = "<<distance<<" path = "<<path/crGlobalHandle::gData()->gUnitScale()<<" newPos = "<<newPos<<" dir = "<<dir<<std::endl;
			//update newpos to Node
			//item->readPathCollideUpdate(scene);//item碰撞信息更新
			if(crGlobalHandle::isClient())
			{
				crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(item->getRelNode());
				if(bot)
				{
					item->doEvent(WCH_ItemCoordToNode,MAKEINT64(bot,crMatterObject::MD_FullMatrix));
				}
			}
			if(m_bestPath.empty())
			{//waypoint find
				if(!isPortPathEmpty())
				{
					nextPortPath(item,scene);
				}
				else if(!isWaypointEmpty())
				{
					nextWaypoint(item,scene);
				}
				else if(m_pathStatus!=ReFound)//if(item->isMainAI())
				{
					void *param;
					item->getDataClass()->getParam(WCHDATA_ItemState,param);
					unsigned char itemstate = *(unsigned char *)param;
					if(itemstate != IS_CloseTarget)
					{
						itemstate = IS_Stop;
						item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
					}
				}
				distance = 0.0f;
				//m_dynamicTryCount = 0;
			}
			return true;
		}
		else if(m_pathStatus!=ReFound && isWaypointEmpty() && isPortPathEmpty())
		{
			void *param;
			item->getDataClass()->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate != IS_CloseTarget)
			{
				itemstate = IS_Stop;
				item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
			}
			m_pathStatus = NotStarted;
		}
	}
	else if(m_pathStatus >= Lapover)
	{
		if(!isPortPathEmpty())
		{
			nextPortPath(item,scene);
		}
		else if(!isWaypointEmpty())
		{
			nextWaypoint(item,scene);
		}
		else
		{
			void *param;
			item->getDataClass()->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate != IS_CloseTarget)
			{
				itemstate = IS_Stop;
				item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
			}
			m_pathStatus = NotStarted;
		}
	}
	distance = 0.0f;
	return false;
}
void crInstanceItem::crPathFinder::nextWaypoint(crInstanceItem *item,crScene *scene,bool refind)
{
	WayPoint waypoint;
	if(!refind)
	{
		WayPoint waypoint = getFrontWaypoint();
		if(waypoint.m_msg != NULL)
		{
			item->doEvent(waypoint.m_msg,MAKEINT64(LOINT64(waypoint.m_param),MAKEINT32(HIINT64(waypoint.m_param),crGlobalHandle::isClient()?1:0)));
			if(waypoint.m_msg == WCH_ITransport)
			{
				popFrontWaypoint();
				return;
			}
		}
		popFrontWaypoint();
	}
	if(!isWaypointEmpty())
	{
		waypoint = getFrontWaypoint();
		if(item->getSceneID() != waypoint.m_sceneid || item->getLayerID() != waypoint.m_layerid)
		{//寻路至传送点
			crSceneLayer *scenelayer = scene->getSceneLayer(item->getLayerID());
			crVector2 transport = scenelayer->getTransportCoord();
			if(transport!=crVector2(0.0f,0.0f))
			{
				waypoint.m_coord.set(transport[0],transport[1],0.0f);
				waypoint.m_msg = WCH_ITransport;
				waypoint.m_param = MAKEINT64(waypoint.m_sceneid,waypoint.m_layerid);
				waypoint.m_sceneid = item->getSceneID();
				waypoint.m_layerid = item->getLayerID();
				addFrontWaypoint(waypoint);
			}
			else
			{//
				if(crGlobalHandle::isClient())
				{
					//std::string str = "所在场景不支持跨图寻路";
					crGlobalHandle::getInstance()->doEvent(WCH_UINotify,MAKEINT64(26,NULL));
				}
				clearWaypointDeque();
				unsigned char itemstate = IS_Stop;
				item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);
				m_pathStatus = NotStarted;
				return;
			}
		}
		item->getDataClass()->inputParam(WCHDATA_TargetPos,&(waypoint.m_coord));
		//unsigned char targettype = Target_Coord;
		//item->getDataClass()->inputParam(WCHDATA_TargetType,&targettype);
		scene->getPathFindingManager()->addWaypointFindRequest(item);
		m_pathStatus = NotStarted;
	}
}
void crInstanceItem::crPathFinder::nextPortPath(crInstanceItem *item,crScene *scene,bool refind)
{
	if(!refind)
	{
		popFrontPortPath();
	}
	if(!isPortPathEmpty())
	{
		crVector2f port = getFrontPortPath();
		crVector3 target(port[0],port[1],0.0f);
		crVector3 from = item->getPosition();
		crVector2 dir(port[0] - from[0], port[1] - from[1]);
		if(getPortPathSize()>1 && dir.length2()<crGlobalHandle::gData()->gPortScopeSqr())
			nextPortPath(item,scene);
		else
		{
			item->getDataClass()->inputParam(WCHDATA_TargetPos,&target);
			scene->getPathFindingManager()->addWaypointFindRequest(item);
			m_pathStatus = NotStarted;
		}
	}
}
void crInstanceItem::crPathFinder::dirty()
{
	GNE::LockMutex lock( m_pathMutex );
	m_bestPath.clear();
	m_pathStatus = NotStarted;
	clearWaypointDeque();
	clearPortPathDeque();
	m_dynamicwait = 0.0f;
}
void crInstanceItem::crPathFinder::refind()
{
	if(m_pathStatus == Found)
		m_pathStatus = ReFound;
}
void crInstanceItem::crPathFinder::setWaypointDeque(WaypointDeque &waypointDeque)
{
	GNE::LockMutex lock( m_waypointMutex );
	m_waypointDeque = waypointDeque;
}
void crInstanceItem::crPathFinder::addWaypoint(crInstanceItem::WayPoint point)
{
	GNE::LockMutex lock( m_waypointMutex );
	m_waypointDeque.push_back(point);
}
void crInstanceItem::crPathFinder::addFrontWaypoint(crInstanceItem::WayPoint point)
{
	GNE::LockMutex lock( m_waypointMutex );
	m_waypointDeque.push_front(point);
}
void crInstanceItem::crPathFinder::clearWaypointDeque()
{
	GNE::LockMutex lock( m_waypointMutex );
	m_waypointDeque.clear();
}
crInstanceItem::WayPoint crInstanceItem::crPathFinder::getFrontWaypoint()
{
	GNE::LockMutex lock( m_waypointMutex );
	return m_waypointDeque.front();
}
void crInstanceItem::crPathFinder::popFrontWaypoint()
{
	GNE::LockMutex lock( m_waypointMutex );
    m_waypointDeque.pop_front();
}
bool crInstanceItem::crPathFinder::isWaypointEmpty()
{
	GNE::LockMutex lock( m_waypointMutex );
	return m_waypointDeque.empty();
}
crInstanceItem::crPathFinder::WaypointDeque &crInstanceItem::crPathFinder::getWaypointDeque()
{
	return m_waypointDeque;
}
void crInstanceItem::crPathFinder::setPortPathDeque(const PortDeque &portPath)
{
	GNE::LockMutex lock( m_portpathMutex );
	m_portPathDeque = portPath;
}
void crInstanceItem::crPathFinder::clearPortPathDeque()
{
	GNE::LockMutex lock( m_portpathMutex );
	m_portPathDeque.clear();
}
CRCore::crVector2f crInstanceItem::crPathFinder::getFrontPortPath()
{
	GNE::LockMutex lock( m_portpathMutex );
	return m_portPathDeque.front();
}
void crInstanceItem::crPathFinder::popFrontPortPath()
{
	GNE::LockMutex lock( m_portpathMutex );
	m_portPathDeque.pop_front();
}
bool crInstanceItem::crPathFinder::isPortPathEmpty()
{
	GNE::LockMutex lock( m_portpathMutex );
	return m_portPathDeque.empty();
}
int crInstanceItem::crPathFinder::getPortPathSize()
{
	GNE::LockMutex lock( m_portpathMutex );
	return m_portPathDeque.size();
}
///////////////////////////////////
//
//crScene
//
/////////////////////////////////////
crScene::ImageMap crScene::s_collideMap;
crScene::crScene():
m_sceneid(0),
m_gameid(0),
m_externSceneType(0),
m_mainroomid(0)
{
	m_pathFindingManager = new crPathFindingManager;
	m_pathFindingManager->init(this);
}
crScene::~crScene()
{

}
void crScene::setSceneID(int id)
{
	m_sceneid = id;
}
int crScene::getSceneID()
{
	return m_sceneid;
}
void crScene::setGameID(int id)
{
	m_gameid = id;
}
int crScene::getGameID()
{
	return m_gameid;
}
void crScene::setGameName(const std::string &gamename)
{
	m_gamename = gamename;
}
const std::string &crScene::getGameName() const
{
	return m_gamename;
}
void crScene::setSceneName(const std::string &scenename)
{
	m_scenename = scenename;
}
const std::string &crScene::getSceneName() const
{
	return m_scenename;
}
void crScene::setFileName(const std::string &filename)
{
	m_filename = filename;
}
const std::string &crScene::getFileName() const
{
	return m_filename;
}
//std::string crScene::getUpdatecfg() const
//{
//	if(m_filename.empty())
//		return m_filename;
//	std::string updcfg = crArgumentParser::getFileNameEliminateExt(m_filename)+".crb";
//	return updcfg;
//}
void crScene::setCollideFile(const std::string &filename)
{
	m_collidefile = filename;
}
const std::string& crScene::getCollideFile()
{
	if(m_collidefile.empty())
	{
		if(!m_filename.empty())
			m_collidefile = crArgumentParser::getFileNameEliminateExt(m_filename)+"collide.cfg";
	}
	return m_collidefile;
}
void crScene::setBrainFile(const std::string &brainfile)
{
	m_brainfile = brainfile;
}
const std::string& crScene::getBrainFile()
{
	if(m_brainfile.empty())
	{
		if(!m_filename.empty())
			m_brainfile = crArgumentParser::getFileNameEliminateExt(m_filename)+".cfg";
	}
	return m_brainfile;
}
///static
//std::string crScene::getUpdatecfg(const std::string &filename)
//{
//	if(filename.empty())
//		return filename;
//	std::string updcfg = crArgumentParser::getFileNameEliminateExt(filename)+".crb";
//	return updcfg;
//}
std::string crScene::getCollideFile(const std::string &filename)
{
	if(filename.empty())
		return filename;
	std::string collidefile = crArgumentParser::getFileNameEliminateExt(filename)+"collide.cfg";
	return collidefile;
}
std::string crScene::getBrainFile(const std::string &filename)
{
	if(filename.empty())
		return filename;
	std::string brainfile = crArgumentParser::getFileNameEliminateExt(filename)+".cfg";
	return brainfile;
}
/////////////////
CRCore::crImage *crScene::findCollideMap(const std::string &name)
{
	ImageMap::iterator itr = s_collideMap.find(name);
	return itr != s_collideMap.end()?itr->second.get():NULL;
}
void crScene::insertCollideMap(const std::string &name, CRCore::crImage *image)
{
	s_collideMap[name] = image;
}
CRCore::crGroup *crScene::getAreaDisplayNode()
{
	return m_areaDisplay.get();
}
void crScene::buildSceneLayer()
{
	std::string collidefile = getCollideFile();
	if(collidefile.empty())
		return;
	m_layers.clear();
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,collidefile))
	{
		CRCore::notify(CRCore::FATAL)<<"crScene::buildSceneLayer(): file open error,"<<collidefile<<std::endl;
		//sprintf(gDebugInfo->buf(),"crScene::buildSceneLayer(): file open error,%s\n\0",collidefile.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	std::string wpfile = crArgumentParser::getFileNameEliminateExt(m_filename) + ".crp";
	ref_ptr<crWaypoint> waypointRoot;
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	if(stream->loadFromFile2(wpfile))
	{
		waypointRoot = crWaypoint::beginread(stream.get());
		waypointRoot->read(stream.get());
		waypointRoot->endread();
	}

	int nLayer = 1;
	std::string collidemap;
	//float layerHeight = 1.0f;
	float maxHeight = 200.0f;
	float width = 256;
	float length = 256;
	std::string str;
	ref_ptr<crSceneLayer> layer;
	ref_ptr<crImage> collideimage;
	std::vector<float> v_i;
	int nBirthPoint = 1;
	int nMap = 1;
	ref_ptr<CREncapsulation::crTableIO> table;
	ref_ptr<crArea> area1,area2;
	crVector2f coord;
	crVector2s scoord;
	std::map<float,ref_ptr<crArea> >AreaMap;
	std::map<float,ref_ptr<crArea> >::iterator amitr;
	while (cfg_script.Push("Layer",nLayer++))
	{
		layer = new crSceneLayer(this);
		layer->setWaypointRoot(waypointRoot.get());
		nMap = 1;
		while (cfg_script.Get("CollideMap", collidemap,nMap++))
		{
			collideimage = findCollideMap(collidemap);
			if(!collideimage.valid())
				collideimage = CRIOManager::readImageFile(collidemap);
			if(collideimage.valid())
			{
				insertCollideMap(collidemap,collideimage.get());
				layer->addCollideMap(collideimage.get());
				//heightField = new crHeightField;
				//heightField->allocate(columns,rows);
				//heightField->setOrigin();
				//heightField->setXInterval(length/(float)(columns-1));
				//heightField->setYInterval(width/(float)(rows-1));
				//heightField->buildFromHeightMap(heightimage.get(),maxHeight);
				//layer->setHeightField(heightField.get());
			}
			else
			{
				CRCore::notify(CRCore::FATAL)<<"LoadCollideMap error "<<collidemap<<std::endl;
				//sprintf(gDebugInfo->buf(),"LoadCollideMap error %s\n\0",collidemap.c_str());
				//gDebugInfo->debugInfo(CRCore::FATAL);
			}
		}
		//cfg_script.Get("LayerHeight", layerHeight);
		cfg_script.Get("SceneWidth", width);
		cfg_script.Get("SceneLength", length);
		cfg_script.Get("MaxHeight", maxHeight);
		//layer->setLayerHeight(layerHeight);
		layer->setMaxHeight(maxHeight);
		layer->setOrigin(crVector2(-width*0.5f,-length*0.5f));
		layer->setMapSize(width,length);
		//CRCore::notify(CRCore::INFO)<<"BuildSceneLayer, SceneName:"<<m_scenename<<" CollideMap:"<<collidemap<<" MaxHeight:"<<maxHeight<<" SceneWidth:"<<width<<" SceneLength:"<<length<<std::endl;
		//sprintf(gDebugInfo->buf(),"BuildSceneLayer, SceneName:%s CollideMap:%s MaxHeight:%d SceneWidth:%d SceneLength:%d\n\0",m_scenename.c_str(),collidemap.c_str(),maxHeight,width,length);
		//gDebugInfo->debugInfo(CRCore::INFO);

		nBirthPoint = 1;
		while(cfg_script.Get("BirthPoint",v_i,nBirthPoint++))
		{
            layer->addBirthPoint(crVector3i(v_i[0],v_i[1],v_i[2]));
			//CRCore::notify(CRCore::INFO)<<"BirthPoint:"<<crVector3i(v_i[0],v_i[1],v_i[2])<<std::endl;
		}
		if(cfg_script.Get("TransportCoord",v_i))
		{
			layer->setTransportCoord(crVector2f(v_i[0]*crGlobalHandle::gData()->gUnitScale(),v_i[1]*crGlobalHandle::gData()->gUnitScale()));
		}
		//不再需要加载区域路点
		//if(cfg_script.Get("AreaTab", str))
		//{
		//	//CRCore::notify(CRCore::INFO)<<"LoadAreaTab: "<<str<<std::endl;
		//	bool areaportsuccess = true;
		//	char buf[256];
		//	table = CREncapsulation::crTableIO::openFile(str);
		//	if(table.valid())
		//	{
		//		int rowcount = table->getRowCount();
		//		int colcount = table->getColumnCount();
		//		int col = 0;
		//		float areaValue;
		//		Vec3Array* coords;
		//		Vec4Array* colours;
		//		if(crGlobalHandle::isClient() && crGlobalHandle::gData()->gShowDebugInfo())
		//		{
		//			m_areaDisplay = new crGroup;
		//			crObject *object = new crObject;
		//			crGeometry* geom = new crGeometry;
		//			crStateSet *stateset = geom->getOrCreateStateSet(); 
		//			stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
		//			crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
		//			stateset->setAttributeAndModes(depth,crStateAttribute::ON);
		//			crPoint *point = new crPoint;
		//			point->setSize(10.0f);
		//			stateset->setAttributeAndModes(point,crStateAttribute::ON);
		//			stateset->setRenderingHint(crStateSet::GRIDDRAW_BIN);
		//			coords = new Vec3Array(rowcount);
		//			colours = new Vec4Array(rowcount);
		//			geom->setVertexArray(coords);
		//			geom->setColorArray(colours);
		//			geom->setColorBinding(crGeometry::BIND_PER_VERTEX);
		//			geom->setNormalBinding(crGeometry::BIND_OFF);
		//			geom->addPrimitive(new DrawArrays(crPrimitive::PT_POINTS,0,rowcount));
		//			object->addDrawable(geom);
		//			object->setCollideMode(crNode::CollideNone);
		//			object->setName("AreaPorts");
		//			m_areaDisplay->addChild(object);
		//		}
		//		for(int i = 0; i<rowcount; ++i)
		//		{
		//			col = 1;
		//			crArgumentParser::appAtoVec((*table)(i,col),coord);
		//			scoord = layer->getCoord(coord);
		//			coord = layer->getPosXY(scoord);
		//			col+=2;
		//			areaValue = atof((*table)(i,col).c_str());
		//			amitr = AreaMap.find(areaValue);
		//			if(amitr == AreaMap.end())
		//			{
		//				area1 = new crArea(areaValue);
		//				AreaMap[areaValue] = area1;
		//			}
		//			else
		//			{
		//				area1 = amitr->second.get();
		//			}
		//			col+=2;
		//			areaValue = atof((*table)(i,col).c_str());
		//			amitr = AreaMap.find(areaValue);
		//			if(amitr == AreaMap.end())
		//			{
		//				area2 = new crArea(areaValue);
		//				AreaMap[areaValue] = area2;
		//			}
		//			else
		//			{
		//				area2 = amitr->second.get();
		//			}
		//			
		//			areaportsuccess = true;
		//			if(!area1->insertAreaPort(coord,area2.get()))
		//			{
		//				//sprintf(buf,"crArea::insertAreaPort警告，port已经存在,row=%d,area1=%f,area2=%f",i,area1->getAreaValue(),area2->getAreaValue());
		//				//MessageBox(::GetActiveWindow(),buf,"Creature3D",MB_OK);
		//				areaportsuccess = false;
		//			}
		//			if(!area2->insertAreaPort(coord,area1.get()))
		//			{
		//				//sprintf(buf,"crArea::insertAreaPort警告，port已经存在,row=%d,area1=%f,area2=%f",i,area1->getAreaValue(),area2->getAreaValue());
		//				//MessageBox(::GetActiveWindow(),buf,"Creature3D",MB_OK);
		//				areaportsuccess = false;
		//			}
		//			if(!areaportsuccess)
		//			{
		//				sprintf(buf,"crArea::insertAreaPort警告,SceneName:%s,port已经存在,row=%d,area1=%f,area2=%f\0",m_scenename.c_str(),i,area1->getAreaValue(),area2->getAreaValue());
		//				CRCore::notify(CRCore::NOTICE)<<buf<<std::endl;
		//			}
		//			if(crGlobalHandle::isClient() && crGlobalHandle::gData()->gShowDebugInfo())
		//			{
		//				crVector2 info = layer->getCollideInfo(layer->getCoord(coord));
		//				(*coords)[i] = crVector3(coord[0],coord[1],info[1]);
		//				if(areaportsuccess)
		//					(*colours)[i] = crVector4(area1->getAreaValue(),area1->getAreaValue(),0.0f,1.0f);
		//				else
		//					(*colours)[i] = crVector4(1.0f,0.0f,0.0f,1.0f);
		//			}
		//			if(i == 0)
		//				layer->setFirstArea(area1.get());
		//			layer->insertAreaPort(coord);
		//		}
		//	}
		//	else
		//	{
		//		CRCore::notify(CRCore::FATAL)<<"LoadAreaTab error "<<str<<std::endl;
		//		//sprintf(gDebugInfo->buf(),"LoadAreaTab error %s\n\0",str.c_str());
		//		//gDebugInfo->debugInfo(CRCore::FATAL);
		//	}
		//	//CRCore::notify(CRCore::INFO)<<"LoadAreaTab: "<<str<<" Success!"<<std::endl;
		//}
		m_layers.push_back(layer);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crScene::buildSceneLayer(): "<<cfg_script.GetLastError()<<std::endl;
	}
}
int crScene::getBirthPointCount(unsigned short layerid)
{
	if(layerid<m_layers.size())
	{
		return m_layers[layerid]->getBirthPointArray().size();
	}
	return 0;
}
bool crScene::findNonblockPos(crInstanceItem *item,const CRCore::crVector2 &targetpos,float searchRange,CRCore::crVector3 &validPos,CollideIgnoreMode ignoreFriends)
{
	unsigned short layerid = item->getLayerID();
	crVector2 newPos(targetpos[0],targetpos[1]);
	bool found = false;
	if(layerid<m_layers.size())
	{
		crSceneLayer *scenelayer = getSceneLayer(layerid);
		short range = (short)(searchRange / scenelayer->getXInterval()+0.5f);
		crVector2s pathCoord = scenelayer->getCoord(newPos);
		CRCore::crVector2 curmapinfo = scenelayer->getCollideInfo(pathCoord);
		if(!scenelayer->walkability(curmapinfo[0]) || dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
		{
			short search = 1;
			short x,y;
			CRCore::crVector2 testmapinfo;
			float walkHeight = crGlobalHandle::gData()->gWalkHeight();
			do
			{
				x = pathCoord[0];
				y = pathCoord[1] + search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0];
				y = pathCoord[1] - search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] + search;
				y = pathCoord[1];
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] - search;
				y = pathCoord[1];
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] - search;
				y = pathCoord[1] - search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] - search;
				y = pathCoord[1] + search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] + search;
				y = pathCoord[1] + search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}

				x = pathCoord[0] + search;
				y = pathCoord[1] - search;
				testmapinfo = scenelayer->getCollideInfo(x,y);
				if(fabs(testmapinfo[1] - curmapinfo[1])<walkHeight)
				{
					newPos = scenelayer->getPosXY(crVector2s(x,y));
					if(scenelayer->walkability(testmapinfo[0]) && !dynamicCollideTest(newPos,item,scenelayer,ignoreFriends))
					{
						found = true;
						break;
					}
				}
				search++;
			} while (search<=range);
		}
		else
		{
			found = true;
		}
		if(!found)
		{
			newPos =  targetpos;
		}
		float zoffset = item->getZoffset() * crGlobalHandle::gData()->gUnitScale();
		float z = getPosZ(layerid,newPos[0],newPos[1], zoffset);
		validPos = crVector3(newPos[0],newPos[1],z);
	}
	return found;
}
bool crScene::findWalkablePos(crInstanceItem *item,const CRCore::crVector2 &targetpos,float searchRange,CRCore::crVector3 &validPos)
{
	bool found = false;
	unsigned short layerid = item->getLayerID();
	crVector2 newPos =  targetpos;
	if(layerid<m_layers.size())
	{
		crSceneLayer *scenelayer = getSceneLayer(layerid);
		short range = (short)(searchRange / scenelayer->getXInterval()+0.5f);
		crVector2s target = scenelayer->getCoord(newPos);
		if(!scenelayer->walkability(target[0],target[1]))
		{//寻找target附近可走的点
			short search = 1;
			short x,y;
			do
			{
				x = target[0];
				y = target[1] + search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0];
				y = target[1] - search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0] + search;
				y = target[1];
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0] - search;
				y = target[1];
				if(scenelayer->walkability(x,y))
					break;
				x = target[0] - search;
				y = target[1] - search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0] - search;
				y = target[1] + search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0] + search;
				y = target[1] + search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				x = target[0] + search;
				y = target[1] - search;
				if(scenelayer->walkability(x,y))
				{
					found = true;
					break;
				}
				search++;
			} while (search<range);
			if(found)
			{
				newPos = scenelayer->getPosXY(crVector2s(x,y));
			}
		}
		else
		{
			found = true;
		}
		if(!found)
		{
			newPos =  targetpos;
		}
		float zoffset = item->getZoffset() * crGlobalHandle::gData()->gUnitScale();
		float z = getPosZ(layerid,newPos[0],newPos[1], zoffset);
		validPos = crVector3(newPos[0],newPos[1],z);
	}
	return found;
}
void crScene::gainBirthPoint(crInstanceItem *item,/*unsigned short layerid,float zoffset,*/int index)
{
	unsigned short layerid = item->getLayerID();
	if(layerid<m_layers.size())
	{
		crSceneLayer *scenelayer = getSceneLayer(layerid);
		const crSceneLayer::BirthPointArray &birthPointArray = scenelayer->getBirthPointArray();
		int count = birthPointArray.size();
		if(count>0)
		{
			float zoffset = item->getZoffset() * crGlobalHandle::gData()->gUnitScale();
			//float z;
			crVector3i birthPoint;
			crVector3 dir(item->getDirx(),item->getDiry(),item->getDirz());
			crVector2 newPos;
			if(index <= -1 || index>=count)
			{
				index = rangei(0,count).get_random();
			}
			birthPoint = birthPointArray[index];

			//birthPoint[0] += rangei(-birthPoint[2],birthPoint[2]).get_random();
			//birthPoint[1] += rangei(-birthPoint[2],birthPoint[2]).get_random();
			float scale = crGlobalHandle::gData()->gUnitScale();
			newPos[0] = (float)birthPoint[0] * scale;
			newPos[1] = (float)birthPoint[1] * scale;
			crVector3 pos;
			if(!findWalkablePos(item,newPos,20.0f,pos))
				CRCore::notify(CRCore::ALWAYS)<<"gainBirthPoint findWalkablePos失败 "<<newPos<<std::endl;
			pos/=scale;
			//float z = getPosZ(layerid,newPos[0],newPos[1], zoffset);
			item->setPosxy(pos[0],pos[1]);
			item->setPosz(pos[2]);
			//CRCore::notify(CRCore::ALWAYS)<<"gainBirthPoint "<<item->getRoleID()<<" "<<item->getPosition()<<std::endl;
			//for(int tryTimes = 0; tryTimes<10; ++tryTimes)
			//{
			//	if(index <= -1 || index>=count)
			//	{
			//		index = rangei(0,count).get_random();
			//	}
			//	birthPoint = m_layers[layerid]->getBirthPointArray()[index];

			//	birthPoint[0] += rangei(-birthPoint[2],birthPoint[2]).get_random();
			//	birthPoint[1] += rangei(-birthPoint[2],birthPoint[2]).get_random();
			//	
			//	newPos[0] = (float)birthPoint[0] * crGlobalHandle::gData()->gUnitScale();
			//	newPos[1] = (float)birthPoint[1] * crGlobalHandle::gData()->gUnitScale();
			//	z = getPosZ(layerid,newPos[0] * crGlobalHandle::gData()->gUnitScale(),newPos[1] * crGlobalHandle::gData()->gUnitScale(), zoffset);
			//	item->setPosz(z / crGlobalHandle::gData()->gUnitScale());
			//	if(item->testNewPos(isClient,this,newPos,dir,height))
			//	{
			//		z = height;
			//		newPos/=crGlobalHandle::gData()->gUnitScale();
			//		z /= crGlobalHandle::gData()->gUnitScale();
			//		z += item->getZoffset();
			//		item->setPosx(newPos[0]);
			//		item->setPosy(newPos[1]);
			//		item->setPosz(z);
			//		break;
			//	}
			//}
		}
	}
}
crSceneLayer *crScene::getSceneLayer(unsigned short layerid)
{
	if(layerid<m_layers.size())
	{
		return m_layers[layerid].get();
	}
	return NULL;
}
float crScene::getPosZ(unsigned short layerid, float posx,float posy, float zoffset)
{
    if(layerid<m_layers.size())
	{
		crSceneLayer* sceneLayer = m_layers[layerid].get();
        //crHeightField *layer = m_layers[layerid]->getHeightField();
		crVector2s pos = sceneLayer->getCoord(posx,posy);
		crVector2 info = sceneLayer->getCollideInfo(pos[0], pos[1]);
		float z = info[1] + zoffset;
	    return z;
	}
	return zoffset;
}
void crScene::insertSceneItem(crInstanceItem *item)
{
	if(item->getRoomID()!=0)
	{
		insertRoomItem(item);
	}
	else
	{
		GNE::LockMutex lock( m_sceneItemMutex );
		m_sceneItemMap.insert(std::make_pair(item->getInstanceItemID(),item));
	}
}
crInstanceItem *crScene::findSceneItem(int itemid,int roomid)
{
	if(roomid == 0)
	{
		GNE::LockMutex lock( m_sceneItemMutex );
		SceneItemMap::iterator sitr = m_sceneItemMap.find(itemid);
		return sitr == m_sceneItemMap.end()?NULL:sitr->second.get();
	}
	else
	{
		return findRoomItem(roomid,itemid);
	}
}
void crScene::insertRoomItem(crInstanceItem *item)
{
	GNE::LockMutex lock( m_roomItemMutex );
	m_roomItemMap[item->getRoomID()][item->getInstanceItemID()] = item;
}
void crScene::removeRoomItem(crInstanceItem *item)
{
	GNE::LockMutex lock( m_roomItemMutex );
	item->clearExtra();
	item->clearData();
	m_roomItemMap[item->getRoomID()].erase(item->getInstanceItemID());
}
crInstanceItem *crScene::findRoomItem(int roomid, int itemid)
{
	GNE::LockMutex lock( m_roomItemMutex );
	RoomItemMap::iterator itr = m_roomItemMap.find(roomid);
	if(itr != m_roomItemMap.end())
	{
		SceneItemMap::iterator sitr = itr->second.find(itemid);
		if(sitr != itr->second.end())
		{
			return sitr->second.get();
		}
	}
	return NULL;
}
void crScene::lockRoomItemMap()
{
	m_roomItemMutex.acquire();
}
crScene::RoomItemMap &crScene::getRoomItemMap()
{
	return m_roomItemMap;
}
void crScene::unlockRoomItemMap()
{
	return m_roomItemMutex.release();
}
void crScene::insertRoomRole(crRole *role)
{
	GNE::LockMutex lock( m_roomRoleMutex );
	m_roomRoleMap[role->getRoomID()][MAKEINT64(role->getPlayerID(),role->getRoleID())] = role;
}
void crScene::removeRoomRole(crRole *role)
{
	GNE::LockMutex lock( m_roomRoleMutex );
	m_roomRoleMap[role->getRoomID()].erase(MAKEINT64(role->getPlayerID(),role->getRoleID()));
	sightRemoveRole(role);
	role->clearExtra();
}
crRole* crScene::findRoomRole(int roomid,int playerid,int roleid)
{
	GNE::LockMutex lock( m_roomRoleMutex );
	RoomRoleMap::iterator itr = m_roomRoleMap.find(roomid);
	if(itr != m_roomRoleMap.end())
	{
		SceneRoleMap::iterator sitr = itr->second.find(MAKEINT64(playerid,roleid));
		if(sitr != itr->second.end())
		{
			return sitr->second.get();
		}
	}
	return NULL;
}
void crScene::lockRoomRoleMap()
{
	m_roomRoleMutex.acquire();
}
crScene::RoomRoleMap &crScene::getRoomRoleMap()
{
	return m_roomRoleMap;
}
void crScene::unlockRoomRoleMap()
{
	m_roomRoleMutex.release();
}
void crScene::insertSightInfo(crSightInfo *sightInfo)
{
	GNE::LockMutex lock( m_sightInfoSetMutex );
	m_sightInfoSet.insert(sightInfo);
}
void crScene::removeSightInfo(crSightInfo *sightinfo)
{
	GNE::LockMutex lock( m_sightInfoSetMutex );
	m_sightInfoSet.erase(sightinfo);
}
void crScene::sendPacketToItemNeighbor(crInstanceItem *item,CRNet::crStreamPacket &packet)
{
	GNE::LockMutex lock( m_sightInfoSetMutex );
	ref_ptr<crSightInfo> sightInfo;
	int roomid = item->getRoomID();
	crRole *role = dynamic_cast<crRole *>(item);
	int id = item->getID();
	//int ownerplayerid = LOINT64(item->getOwnerID());
	for( SightInfoSet::iterator sitr = m_sightInfoSet.begin();
		sitr != m_sightInfoSet.end();
		++sitr )
	{
		sightInfo = *sitr;
		if(sightInfo->getRoomID() == roomid)
		{
			if(role)
			{
				if ((id>0 && sightInfo->isEyeRole(id)) || (id<0 && sightInfo->isEyeItem(id)))
				{
					//if (id < 0)
					//	sightInfo->sendPacketToEyePlayer(packet, ownerplayerid);
					//else
					sightInfo->sendPacketToEyePlayer(packet, id);
				}
				else if (sightInfo->isRoleInSight(role)/* || sightInfo->isItemInSight(role)*/)
				{
					sightInfo->sendPacketToEyePlayer(packet);
				}
			}
			else
			{
				if(sightInfo->isItemInSight(item) || sightInfo->isEyeItem(id))
				{
					sightInfo->sendPacketToEyePlayer(packet);
				}
			}
		}
	}
}
void crScene::itemDead(crInstanceItem *item)
{
	GNE::LockMutex lock( m_sightInfoSetMutex );
	item->clearExtra();
	ref_ptr<crSightInfo> sightInfo;
	int roomid = item->getRoomID();
	crRole *role = dynamic_cast<crRole *>(item);
	int id = item->getID();
	crItemOutRangePacket packet;
	crItemOutRangePacket::buildRequestPacket(packet,0,item);
	bool found = false;
	for( SightInfoSet::iterator sitr = m_sightInfoSet.begin();
		sitr != m_sightInfoSet.end();
		++sitr )
	{
		sightInfo = *sitr;
		if(sightInfo->getRoomID() == roomid)
		{
			if(role)
			{
				if ((id>0 && sightInfo->isEyeRole(id)) || (id<0 && sightInfo->isEyeItem(id)))
				{
					sightInfo->sendPacketToEyePlayer(packet,id);
				}
				//else if(sightInfo->isRoleInSight(role))
				//{
				//	//sightInfo->removeInSightItem(role); 
				//	sightInfo->insertOutSightItem(role);
				//}
			}
			else
			{
				if(sightInfo->isEyeItem(id))
				{
					//sightInfo->sendPacketToEyePlayer(packet);
					sightInfo->removeEyeItem(id);
					found = true;
				}
				//else if(sightInfo->isItemInSight(item))
				//{
				//	//sightInfo->removeInSightItem(item);
				//	sightInfo->insertOutSightItem(item);
				//}
			}
		}
	}
	if (!found && item->getSightInfo() && (!role || id<0))
	{
		sightInfo = item->getSightInfo();
		sightInfo->removeEyeItem(id);
	}
}
void crScene::itemRelive(crInstanceItem *item)
{
	GNE::LockMutex lock( m_sightInfoSetMutex );
	crData *data = item->getDataClass();
	if(data)
	{
		unsigned char itemstate = IS_Relive;
		data->inputParam(WCHDATA_ItemState,&itemstate);
	}
	ref_ptr<crSightInfo> sightInfo;
	int roomid = item->getRoomID();
	crRole *role = dynamic_cast<crRole *>(item);
	int id = item->getID();
	ref_ptr<crStreamBuf> rtDataStream;
	item->doEvent(WCH_GetItemRTData,MAKEINT64(&rtDataStream,NULL));
	if(!rtDataStream.valid())
	{
		return;
	}
	crItemInRangePacket packet;
	crItemInRangePacket::buildRequestPacket(packet,0,item,rtDataStream.get());
	bool found = false;
	for( SightInfoSet::iterator sitr = m_sightInfoSet.begin();
		sitr != m_sightInfoSet.end();
		++sitr )
	{
		sightInfo = *sitr;
		if(sightInfo->getRoomID() == roomid)
		{
			if(role)
			{
				if ((id>0 && sightInfo->isEyeRole(id)) || (id<0 && sightInfo->isEyeItem(id)))
				{
					sightInfo->sendPacketToEyePlayer(packet,id);
				}
				else if(sightInfo->isRoleInSight(role))
				{
					sightInfo->insertDropedItem(role);
				}
			}
			else
			{
				if(sightInfo == item->getSightInfo())
				{
					//sightInfo->sendPacketToEyePlayer(packet);
					sightInfo->addEyeItem(item);
					found = true;
				}
				else if(sightInfo->isItemInSight(item))
				{
					sightInfo->insertDropedItem(item);
				}
			}
		}
	}
	if(role && id>0)
	{
		crPlayerEventPacket packet;
		crPlayerEventPacket::buildRequestPacket(packet,id,role,WCH_RecvItemRTData,rtDataStream.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(id));
		if(playerData.valid()) netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
	}
	if(!found && item->getSightInfo() && (!role || id<0))
	{
		sightInfo = item->getSightInfo();
		sightInfo->addEyeItem(item);
	}
	item->clearSyncPos();
	//item->doEvent(WCH_ItemRelive);
}
void crScene::sightRemoveRole(crRole *role)
{//playerData->setSightInfo(NULL);已经将信息发送给同伴了
	GNE::LockMutex lock( m_sightInfoSetMutex );
	ref_ptr<crSightInfo> sightInfo;
	int roomid = role->getRoomID();
	int playerid = role->getPlayerID();
	for( SightInfoSet::iterator sitr = m_sightInfoSet.begin();
		sitr != m_sightInfoSet.end();
		++sitr )
	{
		sightInfo = *sitr;
		if(sightInfo->getRoomID() == roomid)
		{
			if(sightInfo->isRoleInSight(role))
			{
				sightInfo->insertDropedItem(role);
			}
		}
	}
}
void crScene::removeSceneItem(crInstanceItem *item)
{
	GNE::LockMutex lock( m_sceneItemMutex );
	item->clearExtra();
	item->clearData();
	m_sceneItemMap.erase(MAKEINT64(item->getInstanceItemID(),item->getRoomID()));
	//if(itemid<0)
	//{
	//	recycleItemID(itemid);
	//}
}
void crScene::wantToRemoveItem(crInstanceItem *item)
{
	item->setLayerID(0xffff);//USHORT_MAX,使它们不再被任何视野看到
	itemDead(item);
	m_removedItemMapMutex.acquire();
	m_removedItemMap[item] = 30.0f;
	m_removedItemMapMutex.release();

	int roomid = item->getRoomID();
	CRCore::ref_ptr<crSightInfo> sightInfo = item->getSightInfo();
	if (sightInfo.valid())
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		crRoom *room = netCallback->findRoom(roomid);
		if (room)
		{
			room->removeSightInfo(sightInfo.get());
			item->setSightInfo(NULL);
		}
	}
	m_sightInfoSetMutex.acquire();
	SightInfoSet::iterator sitr;
	for( sitr = m_sightInfoSet.begin();
		sitr != m_sightInfoSet.end();
		++sitr )
	{
		sightInfo = *sitr;
		if(sightInfo->getRoomID() == roomid)
			sightInfo->removeInSightItem(item);
	}
	m_sightInfoSetMutex.release();
}
void crScene::updateRemovedItemMap(float dt)
{
	std::vector< ref_ptr<crInstanceItem> >ItemVec;
	ref_ptr<crInstanceItem> item;
	{
		GNE::LockMutex lock( m_removedItemMapMutex );
		for( RemovedItemMap::iterator itr = m_removedItemMap.begin();
			itr != m_removedItemMap.end();)
		{
			itr->second -= dt;
			if(itr->second>-1000.0f && itr->second<0.0f)
			{
				item = itr->first;
				ItemVec.push_back(item);
				itr->second = -1000.0f;
			}
			else if(itr->second<-1020.0f)
			{
				itr = m_removedItemMap.erase(itr);
				continue;
			}
			++itr;
		}
	}
	for( std::vector< ref_ptr<crInstanceItem> >::iterator itr = ItemVec.begin();
		 itr != ItemVec.end();
		 ++itr )
	{
		item = *itr;
		if(item->getRoomID() != 0)
		{
			removeRoomItem(item.get());
		}
		else
		{
			removeSceneItem(item.get());
		}
	}
}
void crScene::lockSceneItemMap()
{
	m_sceneItemMutex.acquire();
}
crScene::SceneItemMap &crScene::getSceneItemMap()
{
	return m_sceneItemMap;
}
void crScene::unlockSceneItemMap()
{
	m_sceneItemMutex.release();
}
//int crScene::gainTemporaryItemID()
//{
//	int id;
//	GNE::LockMutex lock( s_recycleItemIDDequeMutex );
//    if(!s_recycleItemIDDeque.empty())
//	{
//		RecycleIDDeque::iterator itr = s_recycleItemIDDeque.begin();
//		if(itr->second>crGlobalHandle::gData()->gRecycleIDTime())
//		{
//            id = itr->first;
//            s_recycleItemIDDeque.pop_front();
//			return id;
//		}
//	}
//	id = s_temporaryItemID;
//	s_temporaryItemID--;
//	return id;
//}
//void crScene::recycleItemID(int id,float delay)
//{
//	if(id<0)
//	{
//		GNE::LockMutex lock( s_recycleItemIDDequeMutex );
//		for( RecycleIDDeque::iterator itr = m_recycleItemIDDeque.begin();
//			itr != s_recycleItemIDDeque.end();
//			++itr )
//		{
//			if(itr->first == id)
//			{
//				itr->second = crGlobalHandle::gData()->gRecycleIDTime() - delay;
//				return;
//			}
//		}
//		s_recycleItemIDDeque.push_back(std::make_pair(id,crGlobalHandle::gData()->gRecycleIDTime() - delay));
//	}
//}
//int crScene::gainTemporaryItemChildID()
//{
//	int id;
//	GNE::LockMutex lock( s_recycleItemChildIDDequeMutex );
//	if(!s_recycleItemChildIDDeque.empty())
//	{
//		RecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
//		if(itr->second>crGlobalHandle::gData()->gRecycleIDTime())
//		{
//			id = itr->first;
//			s_recycleItemChildIDDeque.pop_front();
//			return id;
//		}
//	}
//	id = s_temporaryItemChildID;
//	s_temporaryItemChildID--;
//	return id;
//}
//void crScene::recycleItemChildID(int id,float delay)
//{
//	if(id<0)
//	{
//		GNE::LockMutex lock( s_recycleItemChildIDDequeMutex );
//		for( RecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
//			itr != s_recycleItemChildIDDeque.end();
//			++itr )
//		{
//			if(itr->first == id)
//			{
//				itr->second = crGlobalHandle::gData()->gRecycleIDTime() - delay;
//				return;
//			}
//		}
//		s_recycleItemChildIDDeque.push_back(std::make_pair(id,crGlobalHandle::gData()->gRecycleIDTime() - delay));
//	}
//}
void crScene::serverUpdate(float dt,crSceneServerCallback *sc)
{
	ref_ptr<crRoom> room;
	{
		crRole *role;
		GNE::LockMutex lock( m_roomRoleMutex );//m_roomRoleMutex->roomdatalock
		for( RoomRoleMap::iterator itr = m_roomRoleMap.begin();
			itr != m_roomRoleMap.end();
			++itr )
		{
			room = sc->findRoom(itr->first);
			if(room.valid() && !room->isPaused())
			{
				for( SceneRoleMap::iterator sitr = itr->second.begin();
					sitr != itr->second.end();
					++sitr )
				{
					role = sitr->second.get();
					role->serverUpdate(dt);
				}
			}
		}
	}
	//roomitem update
	{
		crInstanceItem *item;
		//GNE::LockMutex lock( m_roomItemMutex );//m_roomItemMutex->roomdatalock
		m_roomItemMutex.acquire();
		RoomItemMap _roomItemMap = m_roomItemMap;
		m_roomItemMutex.release();
		for (RoomItemMap::iterator itr = _roomItemMap.begin();
			itr != _roomItemMap.end();
			 ++itr )
		{
			room = sc->findRoom(itr->first);
			if(room.valid() && !room->isPaused())
			{
				for( SceneItemMap::iterator sitr = itr->second.begin();
					sitr != itr->second.end();
					++sitr )
				{
					item = sitr->second.get();
					item->serverUpdate(dt);
				}
			}
		}
	}
	//if(m_roomItemMap.empty())
	//{//对于非开房间式场景,也需要建立一个room
	//	GNE::LockMutex lock( m_sceneItemMutex );
	//	for( SceneItemMap::iterator siitr = m_sceneItemMap.begin();
	//		siitr != m_sceneItemMap.end();
	//		++siitr )
	//	{
	//		item = siitr->second.get();
	//		item->serverUpdate(dt);
	//	}
	//}
	//{
	//	GNE::LockMutex lock2( m_sightInfoSetMutex );
	//	GNE::LockMutex lock( m_sceneItemMutex );
	//	SightInfoSet::iterator sitr;
	//	CRCore::ref_ptr<crSightInfo> sightInfo;
	//	if(!m_removedItemVec.empty())
	//	{
	//		int roomid;
	//		crInstanceItem *item;
	//		for( RemovedItemVec::iterator ritr = m_removedItemVec.begin();
	//			ritr != m_removedItemVec.end();
	//			++ritr )
	//		{
	//			item = ritr->get();
	//			roomid = item->getRoomID();
	//			for( sitr = m_sightInfoSet.begin();
	//				 sitr != m_sightInfoSet.end();
	//				 ++sitr )
	//			{
	//				sightInfo = *sitr;
	//				if(sightInfo->getRoomID() == roomid)
	//					sightInfo->removeInSightItem(item);
	//			}
	//			item->clearData();
	//			removeSceneItem(item->getID(),roomid);
	//		}
	//		m_removedItemVec.clear();
	//	}
	//	//for( sitr = m_sightInfoSet.begin();
	//	//	 sitr != m_sightInfoSet.end(); )
	//	//{
	//	//	sightInfo = *sitr;
	//	//	if(sightInfo->isEmpty())
	//	//	{
	//	//		sitr = m_sightInfoSet.erase(sitr);
	//	//	}
	//	//	else
	//	//	{
	//	//		++sitr;
	//	//	}
	//	//}
	//}
	if(m_emporium.valid())
	{
		float updatedt = dt;
		m_emporium->doEvent(WCH_ServerUpdate,MAKEINT64(&updatedt,NULL));
	}
	updateRemovedItemMap(dt);
	//s_recycleItemIDDequeMutex.acquire();
	//for( RecycleIDDeque::iterator itr = s_recycleItemIDDeque.begin();
	//	 itr != s_recycleItemIDDeque.end();
	//	 ++itr )
	//{
	//	if(itr->second <= crGlobalHandle::gData()->gRecycleIDTime())
	//		itr->second+=dt;
	//}
	//s_recycleItemIDDequeMutex.release();

	//s_recycleItemChildIDDequeMutex.acquire();
	//for( RecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
	//	itr != s_recycleItemChildIDDeque.end();
	//	++itr )
	//{
	//	if(itr->second <= crGlobalHandle::gData()->gRecycleIDTime())
	//		itr->second+=dt;
	//}
	//s_recycleItemChildIDDequeMutex.release();
}
void crScene::clientUpdate(float dt)
{
}
crPathFindingManager *crScene::getPathFindingManager()
{
	return m_pathFindingManager;
}
void crScene::clearCollideMap()
{
	s_collideMap.clear();
}
void crScene::setEmporium(crInstanceItem *item)
{
	m_emporium = item;
}
crInstanceItem *crScene::getEmporium()
{
	return m_emporium.get();
}
crInstanceItem* crScene::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos)//返回碰撞的item
{
	GNE::LockMutex lock( m_sceneItemMutex );
	crInstanceItem *sceneItem;
	int roomid = item->getRoomID();
	int layerid = item->getLayerID();
	for( SceneItemMap::iterator itr = m_sceneItemMap.begin();
		itr != m_sceneItemMap.end();
		++itr )
	{
		sceneItem = itr->second.get();
		if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
		{
			if(sceneItem->dynamicCollideTest(item,newPos))
			{
				return sceneItem;
			}
		}
	}
	return NULL;
}
crInstanceItem* crScene::dynamicCollideTest(const CRCore::crVector2 &coordPos,crInstanceItem *item,crSceneLayer *layer,CollideIgnoreMode ignoreFriends)
{//返回碰撞的item
	//出生点不碰撞
	//if(layer->isInBirthPoint(coordPos))
	//	return NULL;
	//////路点不碰撞
	//if(layer->isInAreaPort(coordPos))
	//	return NULL;
	if(crGlobalHandle::isClient())
	{
		return crMyPlayerData::getInstance()->dynamicCollideTest(item,coordPos,ignoreFriends);
	}
	else
	{
		//crSightInfo *sightInfo = item->getSightInfo();
		//return sightInfo->dynamicCollideTest(coordPos,item);
		crVector2f pos;
		crVector2s coord;
		crInstanceItem *sceneItem;
		int roomid = item->getRoomID();
		int layerid = item->getLayerID();
		if(roomid == 0)
		{
			GNE::LockMutex lock( m_sceneItemMutex );
			for( SceneItemMap::iterator itr = m_sceneItemMap.begin();
				itr != m_sceneItemMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						return sceneItem;
					}
				}
			}
		}
		else
		{
			GNE::LockMutex lock( m_roomItemMutex );
			SceneItemMap &_sceneItemMap = m_roomItemMap[roomid];
			for( SceneItemMap::iterator itr = _sceneItemMap.begin();
				itr != _sceneItemMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid)
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						return sceneItem;
					}
				}
			}
		}
		{
			GNE::LockMutex lock( m_roomRoleMutex );
			SceneRoleMap &sceneRoleMap = m_roomRoleMap[roomid];
			for( SceneRoleMap::iterator itr = sceneRoleMap.begin();
				itr != sceneRoleMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid)
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						return sceneItem;
					}
				}
			}
		}
	}
	return NULL;
}
bool crScene::dynamicCollideTestAll(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideItemSet &collideItemSet)//返回碰撞的item
{
	GNE::LockMutex lock( m_sceneItemMutex );
	crInstanceItem *sceneItem;
	int roomid = item->getRoomID();
	int layerid = item->getLayerID();
	for( SceneItemMap::iterator itr = m_sceneItemMap.begin();
		 itr != m_sceneItemMap.end();
		 ++itr )
	{
		sceneItem = itr->second.get();
		if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
		{
			if(sceneItem->dynamicCollideTest(item,newPos))
			{
				collideItemSet.insert(sceneItem);
			}
		}
	}
	return !collideItemSet.empty();
}
bool crScene::dynamicCollideTestAll(const CRCore::crVector2 &coordPos,crInstanceItem *item,crSceneLayer *layer,CollideItemSet &collideItemSet)
{//返回碰撞的item
	//出生点不碰撞
	//if(layer->isInBirthPoint(coordPos))
	//	return NULL;
	//////路点不碰撞
	//if(layer->isInAreaPort(coordPos))
	//	return NULL;
	if(crGlobalHandle::isClient())
	{
		return crMyPlayerData::getInstance()->dynamicCollideTestAll(item,coordPos,collideItemSet);
	}
	else
	{
		//crSightInfo *sightInfo = item->getSightInfo();
		//return sightInfo->dynamicCollideTest(coordPos,item);
		crVector2f pos;
		crVector2s coord;
		crInstanceItem *sceneItem;
		int roomid = item->getRoomID();
		int layerid = item->getLayerID();
		if(roomid == 0)
		{
			GNE::LockMutex lock( m_sceneItemMutex );
			for( SceneItemMap::iterator itr = m_sceneItemMap.begin();
				itr != m_sceneItemMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						collideItemSet.insert(sceneItem);
					}
				}
			}
		}
		else
		{
			GNE::LockMutex lock( m_roomItemMutex );
			SceneItemMap &_sceneItemMap = m_roomItemMap[roomid];
			for( SceneItemMap::iterator itr = _sceneItemMap.begin();
				itr != _sceneItemMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid)
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						collideItemSet.insert(sceneItem);
					}
				}
			}
		}
		{
			GNE::LockMutex lock( m_roomRoleMutex );
			SceneRoleMap &sceneRoleMap = m_roomRoleMap[roomid];
			for( SceneRoleMap::iterator itr = sceneRoleMap.begin();
				itr != sceneRoleMap.end();
				++itr )
			{
				sceneItem = itr->second.get();
				if(sceneItem != item && sceneItem->getLayerID() == layerid)
				{
					if(sceneItem->dynamicCollideTest(item,coordPos))
					{
						collideItemSet.insert(sceneItem);
					}
				}
			}
		}
	}
	return !collideItemSet.empty();
}
///////////////////////////////////
//
//crArea
//
/////////////////////////////////////
std::set<crArea *> crArea::s_searchset;
void crArea::beginSearch()
{
	s_searchset.clear();
}
bool crArea::insertAreaPort(const CRCore::crVector2f &port, crArea *area)
{
	AreaPortMap::iterator itr = m_areaPortMap.find(port);
	if(itr != m_areaPortMap.end())
	{
		char buf[128];
		sprintf(buf,"crArea::insertAreaPort警告，已经存在的Port,area1=%f,area2=%f,Port:%f,%f\0",getAreaValue(),area->getAreaValue(),itr->first[0],itr->first[1]);
		CRCore::notify(CRCore::NOTICE)<<buf<<std::endl;
		return false;
		//std::string str = "crArea::insertAreaPort警告，port已经存在["+crArgumentParser::appVectoa(port)+"]";
		//MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
	}
	m_areaPortMap[port] = area;
	return true;
}
crArea* crArea::searchArea(float area)
{
	s_searchset.insert(this);
	if(CollideMapNear(area,m_areaValue))
	{
		return this;
	}
	else
	{
		crArea *node;
		for( AreaPortMap::iterator itr = m_areaPortMap.begin();
			itr != m_areaPortMap.end();
			++itr )
		{
			node = itr->second.get();
			if(s_searchset.find(node) == s_searchset.end())
			{
				node = node->searchArea(area);
				if(node) return node;
			}
		}
		return NULL;
	}
}
void crArea::distillPath(std::stack<crArea *>&path,PortDeque&portPath)
{
	crArea *area1 = this;
	crArea *area2;
	crVector2f port;
	while(!path.empty())
	{
		area2 = path.top();
		path.pop();
		if(area1->findAreaPort(area2,port))
		{
			portPath.push_front(port);
			//CRCore::notify(CRCore::ALWAYS)<<"crArea::distillPath area1:"<<area1->getAreaValue()<<"area2:"<<area2->getAreaValue()<<"port:"<<port<<std::endl;
			area1 = area2;
		}
	}
}
bool crArea::findAreaPort(crArea *connectArea,CRCore::crVector2f &port)
{
	for( AreaPortMap::iterator itr = m_areaPortMap.begin();
		itr != m_areaPortMap.end();
		++itr )
	{
		if(itr->second == connectArea)
		{
			port = itr->first;
			return true;
		}
	}
	return false;
}
bool crArea::getBestPort(crArea *area2,const crVector2f &pos,crVector2f &bestport,float& mindist)
{
	if(m_areaPortMap.empty()) return false;
	mindist = FLT_MAX;
	float dist;
	crVector2f searchpos;
	for( AreaPortMap::iterator itr = m_areaPortMap.begin();
		itr != m_areaPortMap.end();
		++itr )
	{
		if(itr->second == area2)
		{
			searchpos = itr->first;
			dist = (searchpos-pos).length();
			if(dist<mindist)
			{
				mindist = dist;
				bestport = searchpos;
			}
		}
	}
	return true;
}
///////////////////////////////////
//
//crWaypoint
//
/////////////////////////////////////
crWaypoint::WaypointSet crWaypoint::s_travelset;
std::map< CRCore::crVector2s,CRCore::ref_ptr<crWaypoint> > crWaypoint::s_readmap;
crWaypoint::WaypointSet crWaypoint::s_drawset;
CRCore::ref_ptr<crWaypoint> crWaypoint::m_neareastWaypoint;
float crWaypoint::m_nearestDist = INT_MAX;
void crWaypoint::connectWaypoint(crWaypoint *waypoint)
{ 
	if(waypoint && waypoint!=this) 
	{
		int g = 10*(abs(waypoint->m_point[0] - m_point[0]) + abs(waypoint->m_point[1] - m_point[1]));
		m_linkmap[waypoint] = g;
		waypoint->m_linkmap[this] = g;
	}
}
void crWaypoint::disconnectWaypoint(crWaypoint *waypoint)
{ 
	if(waypoint!=this)
	{ 
		m_linkmap.erase(waypoint);
		waypoint->m_linkmap.erase(this);
	}
}
void crWaypoint::disconnectAll()
{
	ref_ptr<crWaypoint> wp;
	for( LinkMap::iterator itr = m_linkmap.begin();
		itr != m_linkmap.end();
		++itr )
	{
		wp = itr->first;
		wp->m_linkmap.erase(this);
	}
	m_linkmap.clear();
}
crWaypoint *crWaypoint::findWaypoint(const CRCore::crVector2s &point)
{
	if(s_travelset.find(this)==s_travelset.end())
	{
		s_travelset.insert(this);
		if(m_point == point)
			return this;
		ref_ptr<crWaypoint> wp;
		crWaypoint *findwp;
		for( LinkMap::iterator itr = m_linkmap.begin();
			itr != m_linkmap.end();
			++itr )
		{
			wp = itr->first;
			findwp = wp->findWaypoint(point);
			if(findwp)
				return findwp;
		}
	}
	return NULL;
}
void crWaypoint::resetNearest()
{
	m_neareastWaypoint = NULL;
	m_nearestDist = INT_MAX;
}
crWaypoint *crWaypoint::searchNearest(const CRCore::crVector2s &point,float range)
{
	if(s_travelset.find(this)==s_travelset.end())
	{
		s_travelset.insert(this);
		float dist = (m_point-point).length();
		if(dist<range)
			return this;
		ref_ptr<crWaypoint> wp;
		crWaypoint *findwp;
		for( LinkMap::iterator itr = m_linkmap.begin();
			itr != m_linkmap.end();
			++itr )
		{
			wp = itr->first;
			findwp = wp->searchNearest(point,range);
			if(findwp)
				return findwp;
		}
		if(dist<m_nearestDist)
		{
			m_neareastWaypoint = this;
			m_nearestDist = dist;
		}
	}
	return NULL;
}
crWaypoint *crWaypoint::getNearest()
{
	return m_neareastWaypoint.get();
}
void crWaypoint::write(CRCore::crStreamBuf *stream)
{
	stream->_writeVec2s(m_point);
	if(s_travelset.find(this)==s_travelset.end())
	{
		s_travelset.insert(this);
		stream->_writeUChar(m_linkmap.size());
		CRCore::ref_ptr<crWaypoint> wp;
		for( crWaypoint::LinkMap::iterator itr = m_linkmap.begin();
			itr != m_linkmap.end();
			++itr )
		{
			wp = itr->first;
			wp->write(stream);
		}
	}
	else
	{
		stream->_writeUChar(0);
	}
}
void crWaypoint::endtravel()
{
	s_travelset.clear();
}
crWaypoint *crWaypoint::beginread(CRCore::crStreamBuf *stream)
{
	return readWaypoint(stream);
}
void crWaypoint::endread()
{
	s_readmap.clear();
}
crWaypoint *crWaypoint::readWaypoint(CRCore::crStreamBuf *stream)
{
	crWaypoint *wp;
	crVector2s point = stream->_readVec2s();
	std::map< CRCore::crVector2s,CRCore::ref_ptr<crWaypoint> >::iterator itr = s_readmap.find(point);
	if(itr  == s_readmap.end())
	{
		wp = new crWaypoint;
		wp->setPoint(point);
		s_readmap[point] = wp;
	}
	else
	{
		wp = itr->second.get();
	}
	return wp;
}
void crWaypoint::read(CRCore::crStreamBuf *stream)
{
	int count = stream->_readUChar();
	if(count>0)
	{
		ref_ptr<crWaypoint> wp;
		for( int i = 0; i<count; i++ )
		{
			wp = readWaypoint(stream);
			wp->read(stream);
			int g = 10*(abs(wp->m_point[0] - m_point[0]) + abs(wp->m_point[1] - m_point[1]));
			m_linkmap[wp] = g;
		}
	}
}
void crWaypoint::drawlink(CRCore::Vec3Array *coords,float zoffset)
{
	crSceneLayer *layer = crMyPlayerData::getInstance()->getScene()->getSceneLayer(0);
	if(layer)
	{
		if(s_drawset.find(this)==s_drawset.end())
		{
			s_drawset.insert(this);
			
			crVector2f pos = layer->getPosXY(m_point);
			crScene *scene = crMyPlayerData::getInstance()->getScene();
			float z = scene->getPosZ(0,pos[0],pos[1],zoffset);
			crVector3 thispos = crVector3(pos[0],pos[1],z);
			crVector3 linkpos;

			CRCore::ref_ptr<crWaypoint> wp;
			for( crWaypoint::LinkMap::iterator itr = m_linkmap.begin();
				itr != m_linkmap.end();
				++itr )
			{
				wp = itr->first;
				if(s_drawset.find(wp)==s_drawset.end())
				{
					coords->push_back(thispos);
					pos = layer->getPosXY(wp->m_point);
					z = scene->getPosZ(0,pos[0],pos[1],zoffset);
					linkpos.set(pos[0],pos[1],z);
					coords->push_back(linkpos);
				}
			}
			for( crWaypoint::LinkMap::iterator itr = m_linkmap.begin();
				itr != m_linkmap.end();
				++itr )
			{
				wp = itr->first;
				wp->drawlink(coords,zoffset);
			}
		}
	}
}
void crWaypoint::enddraw()
{
	s_drawset.clear();
}
void crWaypoint::removeRedundantWaypoint(WaypointSet &waypointset)
{
	if(s_travelset.find(this)==s_travelset.end())
	{
		s_travelset.insert(this);
		CRCore::ref_ptr<crWaypoint> wp;
		for( crWaypoint::LinkMap::iterator itr = m_linkmap.begin();
			itr != m_linkmap.end();)
		{
			wp = itr->first;
			if(waypointset.find(this)==waypointset.end())
			{
				itr = m_linkmap.erase(itr);
			}
			else
			{
				++itr;
			}
			wp->removeRedundantWaypoint(waypointset);
		}
		if(waypointset.find(this)==waypointset.end())
		{
			m_linkmap.clear();
		}
	}
}
int crWaypoint::getLinkDist(crWaypoint *waypoint)
{
	LinkMap::iterator itr = m_linkmap.find(waypoint);
	return itr != m_linkmap.end()?itr->second:INT_MAX;
}
///////////////////////////////////
//
//crSceneLayer
//
/////////////////////////////////////
crSceneLayer::crSceneLayer(crScene *parent)
{
	m_parentScene = parent;
}
bool crSceneLayer::findAreaPorts(const CRCore::crVector2s &from, const CRCore::crVector2s &to, PortDeque&portPath,crAreaPathFinding *areaPathFinding)
{
	//crVector2s froms = getCoord(from[0],from[1]);
	//crVector2s tos = getCoord(to[0],to[1]);
	//crVector2s _tos = tos;
	//if(!walkability(tos[0],tos[1]))
	//{//寻找target附近可走的点
	//	short search = 1;
	//	short x,y;
	//	do
	//	{
	//		x = tos[0];
	//		y = tos[1] + search;
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0];
	//		y = tos[1] - search;
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] + search;
	//		y = tos[1];
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] - search;
	//		y = tos[1];
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] - search;
	//		y = tos[1] - search;
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] - search;
	//		y = tos[1] + search;
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] + search;
	//		y = tos[1] + search;
	//		if(walkability(x,y))
	//			break;
	//		x = tos[0] + search;
	//		y = tos[1] - search;
	//		if(walkability(x,y))
	//			break;
	//		search++;
	//	} while (1);
	//	tos.set(x,y);
	//}
	//if(tos == froms)
	//	return false;
	crVector2f _from = getPosXY(from);
	crVector2f _to = getPosXY(to);
	//if(tos != _tos)
	//{
	//	_to = getPosXY(tos);
	//}
	//else
	//{
	//	_to.set(to[0],to[1]);
	//}
	float fromarea = getAreaValue(from);
	float toarea = getAreaValue(to);
	//portPath.push_front(_to);
	if(!CollideMapNear(fromarea,toarea))
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crSceneLayer::findAreaPorts from:"<<from<<"to:"<<to<<std::endl;
		crArea::beginSearch();
		crArea *startarea = m_firstArea->searchArea(fromarea);
		if(startarea)
		{
			crArea::beginSearch();
			crArea *endarea = startarea->searchArea(toarea);
			if(endarea && areaPathFinding->findBestPath(startarea,endarea,_from,_to) == crInstanceItem::crPathFinder::Found)
			{
				portPath = areaPathFinding->getBestPath();
				//if(!bestPath.empty())
				//{
				//	crVector2f currentCoord = _to;
				//	AreaDeque::iterator itr = bestPath.begin();
				//	crArea *area1 = *itr;
				//	crArea *area2;
				//	crVector2f port;
				//	//CRCore::notify(CRCore::ALWAYS)<<"FromArea:"<<startarea->getAreaValue()<<"ToArea:"<<area1->getAreaValue()<<std::endl;
				//	//CRCore::notify(CRCore::ALWAYS)<<"Start:"<<area1->getAreaValue();
				//	for( ++itr;
				//		itr != bestPath.end();
				//		++itr )
				//	{
				//		area2 = *itr;
				//		port = area1->getBestPort(area2,currentCoord);
				//		portPath.push_front(port);
				//		currentCoord = port;
				//		area1 = area2;
				//		//CRCore::notify(CRCore::ALWAYS)<<"["<<port<<"]"<<area1->getAreaValue();
				//	}
				//	//CRCore::notify(CRCore::ALWAYS)<<"PortCount:"<<portPath.size()<<std::endl;
				//}
				//if(portPath.empty())
				//{
				//	CRCore::notify(CRCore::ALWAYS)<<"区域NotFound!"<<std::endl;
				//}
				areaPathFinding->reset();
			}
			else
			{
				CRCore::notify(CRCore::ALWAYS)<<m_parentScene->getSceneName()<<"区域之间不通!FromArea:"<<startarea->getAreaValue()<<"ToArea:"<<toarea<<std::endl;
				return false;
			}
		}
	}
	else
	{
		portPath.push_front(_to);
	}
	return true;
}
float crSceneLayer::getAreaValue(const CRCore::crVector2s &coord)
{
	return getCollideInfo(coord)[0];
}
bool crSceneLayer::isInBirthPoint(const CRCore::crVector2 &pos)
{
	crVector3 birthPoint;
	float scale = crGlobalHandle::gData()->gUnitScale();
	for( BirthPointArray::iterator itr = m_birthPointArray.begin();
		 itr != m_birthPointArray.end();
		 ++itr )
	{
		birthPoint = (*itr);
		birthPoint *= scale;
		if((pos - crVector2(birthPoint[0],birthPoint[1])).length()<birthPoint[2])
			return true;
	}
	return false;
}
void crSceneLayer::insertAreaPort(const CRCore::crVector2 &coord)
{
	bool found = false;
	for( AreaPortVec::iterator itr = m_areaPortVec.begin();
		 itr != m_areaPortVec.end();
		 ++itr )
	{
		if((*itr) == coord)
		{
			found = true;
			break;
		}
	}
	if(!found)
		m_areaPortVec.push_back(coord);
}
bool crSceneLayer::isInAreaPort(const CRCore::crVector2 &coord)
{
	crVector2 port;
	for( AreaPortVec::iterator itr = m_areaPortVec.begin();
		 itr != m_areaPortVec.end();
		 ++itr )
	{
		port = (*itr);
		if((port - coord).length2()<10.0f)
			return true;
	}
	return false;
}
CRCore::crVector2s crSceneLayer::getCoord(float posx,float posy) const
{
	return getCoord(crVector2(posx,posy));
}
CRCore::crVector2s crSceneLayer::getCoord(CRCore::crVector2 pos) const
{
	pos -= m_origin;
	pos[0] /= getXInterval();
	pos[1] /= getYInterval();
	crVector2s coord(int(pos[0]+0.5f),int(pos[1]+0.5f));//4舍5入取整
	//if(coord[0]<0 || coord[0]>m_collideMapVec[0]->s() || coord[1]<0 && coord[1]>m_collideMapVec[0]->t())
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"crSceneLayer::getCoord "<<coord<<std::endl;
	//}
	return coord;
}
CRCore::crVector2 crSceneLayer::getPosXY(const crVector2s& coord) const
{
	crVector2 pos(coord[0],coord[1]);
	pos[0] *= getXInterval();
	pos[1] *= getYInterval();
	pos += m_origin;
	return pos;
}
float crSceneLayer::getHeight(short x, short y)
{
	if(x>=0 && x<m_collideMapVec[0]->s() && y>=0 && y<m_collideMapVec[0]->t())
	{
		crVector4 color = m_collideMapVec[0]->getColor(x,y);
		float height = color[1] * 2.0f - 1.0f;
		return height * m_maxHeight;
	}
	return 0.0f;
}
CRCore::crVector2 crSceneLayer::getCollideInfo(short x, short y) const
{
	if(x>=0 && x<m_collideMapVec[0]->s() && y>=0 && y<m_collideMapVec[0]->t())
	{
		crVector4 color = m_collideMapVec[0]->getColor(x,y);
		float height = color[1] * 2.0f - 1.0f;
		return crVector2(color[0],height * m_maxHeight);
	}
	return crVector2(0.0f,0.0f);
}
CRCore::crVector2 crSceneLayer::getCollideInfo(const CRCore::crVector2s& coord) const
{
	return getCollideInfo(coord[0],coord[1]);
}
//void crSceneLayer::setLayerHeight(float layerHeight)
//{
//	m_layerHeight = layerHeight;
//}
//float crSceneLayer::getLayerHeight() const
//{
//	return m_layerHeight;
//}
CRCore::crVector2 crSceneLayer::getCollideInfoEx(short x, short y, float z) const
{
	if(x>=0&&x<m_collideMapVec[0]->s() && y>=0&&y<m_collideMapVec[0]->t())
	{
		const crImage *collideMap;
		if(m_collideMapVec.size()==1)
		{
			collideMap = m_collideMapVec[0].get();
			crVector4 color = collideMap->getColor(x,y);
			float height = (color[1] * 2.0f - 1.0f) * m_maxHeight;
			return crVector2(color[0],height);
		}
		else
		{
			crVector4 color;
			float height;
			for( CollideMapVec::const_iterator itr = m_collideMapVec.begin();
				itr != m_collideMapVec.end();
				++itr )
			{
				collideMap = itr->get();

				color = collideMap->getColor(x,y);
				height = (color[1] * 2.0f - 1.0f) * m_maxHeight;
				if(height-z>=crGlobalHandle::gData()->gLayerHeight())
				{//高1米，尝试寻找下一张map
					continue;
				}
				return crVector2(color[0],height);
			}
			collideMap = m_collideMapVec[0].get();
			color = collideMap->getColor(x,y);
			height = (color[1] * 2.0f - 1.0f) * m_maxHeight;
			return crVector2(color[0],height);
		}
	}
	return crVector2(0,0.0f); 
}
CRCore::crVector2 crSceneLayer::getCollideInfoEx(const CRCore::crVector2s& coord,float z) const
{
	return getCollideInfoEx(coord[0],coord[1],z);
}
bool crSceneLayer::walkability(short x, short y) const
{
	if(x>=0 && x<m_collideMapVec[0]->s() && y>=0 && y<m_collideMapVec[0]->t())
	{
		return walkability(m_collideMapVec[0]->getColor(x,y)[0]);
	}
	return false;
}
bool crSceneLayer::walkability(float info0) const
{
	return getMapProtected(info0) != MP_NoPass;
}
unsigned char crSceneLayer::getMapProtected(short x, short y) const
{
	if(x>=0 && x<m_collideMapVec[0]->s() && y>=0 && y<m_collideMapVec[0]->t())
	{
		return getMapProtected(m_collideMapVec[0]->getColor(x,y)[0]);
	}
	return MP_NoPass;
}
unsigned char crSceneLayer::getMapProtected(float info0) const
{//<0.1为系统保留区域
	if(CollideMapNear(info0,0.0f))
	{
		return MP_NoPass;
	}
	else if(CollideMapNear(info0,0.01f))
	{
		return MP_NoAutoFight;
	}
	else if(CollideMapNear(info0,0.02f))
	{
		return MP_NoPK;
	}
	return MP_Normal;
}
void crSceneLayer::setTransportCoord(const CRCore::crVector2 &coord)
{
	m_transportCoord = coord;
}
const CRCore::crVector2 &crSceneLayer::getTransportCoord()const
{
	return m_transportCoord;
}
void crSceneLayer::setWaypointRoot(crWaypoint *waypoint)
{
	m_waypointRoot = waypoint;
}
crWaypoint *crSceneLayer::getWaypointRoot()
{
	return m_waypointRoot.get();
}
//void crSceneLayer::updateItemCollideMap(crInstanceItem *collideitem)
//{
//	GNE::LockMutex lock( m_itemCollideMapMutex );
//	crInstanceItem::ItemCollideInfo itemCollideInfo = collideitem->getItemCollideInfo();
//	crInstanceItem::ItemCollideInfo::iterator infoItr;
//	std::vector<CRCore::crVector2s> EraseVec;
//	for( ItemCollideMap::iterator itr = m_itemCollideMap.begin();
//		 itr != m_itemCollideMap.end();
//		 ++itr )
//	{
//		if(itr->second == collideitem)
//		{
//			infoItr = itemCollideInfo.find(itr->first);
//			if(infoItr != itemCollideInfo.end())
//			{
//				itemCollideInfo.erase(infoItr);
//			}
//			else
//			{
//				EraseVec.push_back(itr->first);
//			}
//		}
//	}
//	for( std::vector<CRCore::crVector2s>::iterator itr = EraseVec.begin();
//		 itr != EraseVec.end();
//		 ++itr )
//	{
//		m_itemCollideMap.erase(*itr);
//	}
//	for( crInstanceItem::ItemCollideInfo::iterator itr = itemCollideInfo.begin();
//		 itr != itemCollideInfo.end();
//		 ++itr )
//	{
//		m_itemCollideMap[*itr] = collideitem;
//	}
//}
//void crSceneLayer::removeCollideItem(crInstanceItem *collideitem)
//{
//	GNE::LockMutex lock( m_itemCollideMapMutex );
//	std::vector<CRCore::crVector2s> EraseVec;
//	for( ItemCollideMap::iterator itr = m_itemCollideMap.begin();
//		itr != m_itemCollideMap.end();
//		++itr )
//	{
//		if(itr->second == collideitem)
//		{
//			EraseVec.push_back(itr->first);
//		}
//	}
//	for( std::vector<CRCore::crVector2s>::iterator itr = EraseVec.begin();
//		itr != EraseVec.end();
//		++itr )
//	{
//		m_itemCollideMap.erase(*itr);
//	}
//}
//crInstanceItem* crSceneLayer::dynamicCollideTest(const CRCore::crVector2s &coord)
//{
//	GNE::LockMutex lock( m_itemCollideMapMutex );
//	ItemCollideMap::iterator itr = m_itemCollideMap.find(coord);
//	return itr==m_itemCollideMap.end()?NULL:itr->second.get();
//}
//void crSceneLayer::insertCollideItem(crInstanceItem *collideItem)
//{
//	GNE::LockMutex lock( m_collideItemSetMutex );
//	m_collideItemSet.insert(collideItem);
//}
//void crSceneLayer::removeCollideItem(crInstanceItem *collideItem)
//{
//	GNE::LockMutex lock( m_collideItemSetMutex );
//	m_collideItemSet.erase(collideItem);
//}
//crInstanceItem* crSceneLayer::dynamicCollideTest(const CRCore::crVector2s &coord)
//{
//	GNE::LockMutex lock( m_collideItemSetMutex );
//	CRCore::ref_ptr<crInstanceItem> setItr;
//	for( CollideItemSet::iterator itr = m_collideItemSet.begin();
//		 itr != m_collideItemSet.end();
//		 ++itr )
//	{
//		setItr = *itr;
//		if(setItr->dynamicCollideTest(coord))
//			return setItr.get();
//	}
//	return NULL;
//}
///////////////////////////////////
//
//crRole
//
/////////////////////////////////////
crRole::crRole():
m_roleid(0),
m_playerid(0),
m_currentMetierMsg(0)
{
	//m_abstractMetier = new crAbstractMetier;
}
crRole::~crRole()
{
	//CRCore::notify(CRCore::NOTICE)<<"~crRole() roleid = "<<getRoleID()<<std::endl;
}
crRole::crRole(const crRole& item):
crInstanceItem(item),
m_roleid(item.m_roleid),
m_playerid(item.m_playerid),
m_currentMetierMsg(NULL)
{
	if(item.m_abstractMetier.valid())
		m_abstractMetier = item.m_abstractMetier->clone();
	if(item.m_metierDataClass.valid())
		m_metierDataClass = item.m_metierDataClass->clone(CRCore::crCopyOp::DEEP_COPY_ALL);
}
void crRole::setRoleID(int id)
{
	m_roleid = id;
}
void crRole::setPlayerID(int playerid)
{
	m_playerid = playerid;
}
void crRole::setAbstractMetier(crAbstractMetier *metier)
{
	m_abstractMetier = metier;
}
crAbstractMetier *crRole::getAbstractMetier()
{
	return m_abstractMetier.get();
}
void crRole::setMetierStream(CRCore::crStreamBuf *stream)
{
	m_metierStream = stream;
}
CRCore::crStreamBuf *crRole::getMetierStream()
{
	if(m_metierStream.valid()) m_metierStream->seekBegin();
	return m_metierStream.get();
}
//////////////////////////////////////////////////////////////
void crRole::setMetierDataClass( CRCore::crData *data )
{
	m_metierDataClass = data;
}
CRCore::crData *crRole::getMetierDataClass()
{
	return m_metierDataClass.get();
}
void crRole::doMetierEvent(_crInt64 kbmsg, _crInt64 param)
{
	//bool isClient = crNetContainer::getInstance()->getMainNetType() == CRNetApp::Client;
	//if(isClient) m_eventMutex.acquire();
	m_currentMetierMsg = kbmsg;
	if(m_metierDataClass.valid())
	{
		ref_ptr<crData> data = m_metierDataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				CRCore::notify(CRCore::ALWAYS)<<"crRole::doMetierEvent error msg = "<<kbmsg<<" playerid = "<<getPlayerID()<<std::endl;
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
	//if(isClient) m_eventMutex.release();
}
crHandle* crRole::getMetierHandle(__int64 kbmsg, const std::string& name)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		if(handle)
		{
			handle = handle->getHandle(name);
		}
		if(handle) return handle;
	}
	return NULL;
}

bool crRole::hasMetierHandle(__int64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crRole::loadMetierData(char streamType)
{
	if(m_abstractMetier.valid())
	{
		if(!m_metierDataClass.valid() && !(m_abstractMetier->getMetierScript().empty()))
			m_metierDataClass = CREncapsulation::loadData(m_abstractMetier->getMetierScript());
		if(m_metierDataClass.valid())
		{
			m_metierDataClass->inputParam(WCHDATA_GameTaskTab,&(m_abstractMetier->getTaskTable()));
			crStreamBuf *metierstream = getMetierStream();
			if(metierstream)
			{
				switch (streamType)
				{
				case SaveStream:
					m_metierDataClass->inputParam(WCHDATA_DataStream,metierstream);
					break;
				case PlayerStream:
					m_metierDataClass->inputParam(WCHDATA_PlayerStream,metierstream);
					break;
					//case NetStream:
					//	m_metierDataClass->inputParam(WCHDATA_NetStream,metierstream);
					//	break;
				}
			}
			m_metierDataClass->excHandle(MAKEINT64(WCH_InitData,this));
			setMetierStream(NULL);
		}
	}
}
void crRole::setPlayerGameData(crPlayerGameData *data)
{
	m_playerGameData = data;
}
crPlayerGameData *crRole::getPlayerGameData()
{
	return m_playerGameData.get();
}
void crRole::clearData()
{
	m_abstractMetier = NULL;
	m_metierStream = NULL;
	m_playerGameData = NULL;
	m_metierDataClass = NULL;
	crInstanceItem::clearData();
}
//void crRole::serverUpdate(float dt)
//{
//	void *param;
//	if(m_dataClass.valid())
//	{
//		m_dataClass->getParam(WCHDATA_ItemState,param);
//		unsigned char itemstate = *(unsigned char*)param;
//		if(itemstate == IS_Dead)
//		{
//			doEvent(MAKEINT64(WCH_DeadEvent,IR_ServerRefash),MAKEINT64(&dt,NULL));
//		}
//		else
//		{
//			float updatedt = dt;
//			doEvent(WCH_ServerUpdate,MAKEINT64(&updatedt,NULL));
//		}
//	}
//	removedItemChildUpdate(dt);
//}
///////////////////////////////////
//
//crPlayerGameData
//
/////////////////////////////////////
crPlayerGameData::crPlayerGameData():
m_id(0),
m_playerid(0),
m_currentMsg(0),
m_dbvalid(false),
m_mainRoleID(0)
{
}
crPlayerGameData::~crPlayerGameData()
{
}
void crPlayerGameData::setID(int id)
{
	m_id = id;
}
int crPlayerGameData::getID()
{
	return m_id;
}
void crPlayerGameData::setPlayerID(int id)
{
	m_playerid = id;
}
int crPlayerGameData::getPlayerID()
{
	return m_playerid;
}
void crPlayerGameData::setStream(CRCore::crStreamBuf *stream)
{
	m_stream = stream;
}
CRCore::crStreamBuf *crPlayerGameData::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
//////////////////////////////////////////////////////////////
void crPlayerGameData::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crPlayerGameData::getDataClass()
{
	return m_dataClass.get();
}
void crPlayerGameData::doEvent(_crInt64 kbmsg, _crInt64 param)
{
	//bool isClient = crNetContainer::getInstance()->getMainNetType() == CRNetApp::Client;
	//if(isClient) m_eventMutex.acquire();
	m_currentMsg = kbmsg;
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch(...)
			{
				CRCore::notify(CRCore::ALWAYS)<<"crPlayerGameData::doEvent error msg = "<<kbmsg<<" playerid = "<<getPlayerID()<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
	//if(isClient) m_eventMutex.release();
}
crHandle* crPlayerGameData::getHandle(__int64 kbmsg, const std::string& name)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		if(handle)
		{
			handle = handle->getHandle(name);
		}
		if(handle) return handle;
	}
	return NULL;
}

bool crPlayerGameData::hasHandle(__int64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crPlayerGameData::setDBValid(bool dbvalid)
{
	m_dbvalid = dbvalid;
}
bool crPlayerGameData::getDBValid()
{
	return m_dbvalid;
}
void crPlayerGameData::setMainRoleID(int roleid)
{
	m_mainRoleID = roleid;
}
int crPlayerGameData::getMainRoleID()
{
	return m_mainRoleID;
}
void crPlayerGameData::setMainRole(crRole *role)
{
	m_mainRole = role;
}
crRole *crPlayerGameData::getMainRole()
{
	return m_mainRole.get();
}
std::string crPlayerGameData::getCharacterName()
{
	return m_mainRole.valid()?m_mainRole->getIName2():"";
}
void crPlayerGameData::clearData()
{
	if(m_mainRole.valid())
		m_mainRole->clearData();
	m_dataClass = NULL;
	m_mainRole = NULL;
	m_stream = NULL;
}
void crPlayerGameData::setAdvertid(const std::string &advertid)
{
	m_advert_id = advertid; 
}
const std::string &crPlayerGameData::getAdvertid()
{
	return m_advert_id;
}
///////////////////////////////////
//
//crRoomPlayer
//
/////////////////////////////////////
crRoomPlayer::crRoomPlayer():
	m_playerid(0),
	m_groupid(0),
	m_birthPointIndex(-1),
	m_ready(false)
{
}
void crRoomPlayer::setPlayerID(int playerid)
{
	m_playerid = playerid;
}
int crRoomPlayer::getPlayerID()
{
	return m_playerid;
}
void crRoomPlayer::setCharacterName(const std::string &characterName)
{
	m_characterName = characterName;
}
const std::string &crRoomPlayer::getCharacterName()
{
	return m_characterName;
}
void crRoomPlayer::setGroupID(unsigned char groupid)
{
	m_groupid = groupid;
}
unsigned char crRoomPlayer::getGroupID()
{
	return m_groupid;
}
void crRoomPlayer::setPlayerGameData(crPlayerGameData *data)
{
	m_playerGameData = data;
}
crPlayerGameData *crRoomPlayer::getPlayerGameData()
{
	return m_playerGameData.get();
}
void crRoomPlayer::getStream(CRCore::ref_ptr<CRCore::crStreamBuf> &stream)
{
	stream = new crStreamBuf;
	stream->createBuf(9 + m_characterName.length());
	stream->_writeInt(m_playerid);//4
	stream->_writeString(m_characterName);
	stream->_writeUChar(m_groupid);
}
void crRoomPlayer::setStream(CRCore::crStreamBuf *stream)
{
	m_playerid = stream->_readInt();
	m_characterName = stream->_readString();
	m_groupid = stream->_readUChar();
}
void crRoomPlayer::setBirthPointIndex(char index)
{
	m_birthPointIndex = index;
}
char crRoomPlayer::getBirthPointIndex()
{
	return m_birthPointIndex;
}
void crRoomPlayer::setReady(bool ready)
{
	m_ready = ready;
}
bool crRoomPlayer::getReady()
{
	return m_ready;
}
///////////////////////////////////
//
//crRoom
//
/////////////////////////////////////
crRoom::crRoom(crScene *scene):
m_scene(scene),
m_gameMode(Melee),
m_roomid(0),
m_maxPlayerCount(16),
m_minPlayerCount(1),
m_roomHost(0),
m_gameRunning(false),
m_gameTime(0),
m_lastAssigndGroupid(0),
m_friendlyFire(false),
m_autoStart(true),
m_autoStartTime(5.0f),
m_timeRemain(0.0f),
m_shareSight(true),
m_removeTime(60.0f),
m_canRemove(false),
m_pauseTimer(0.0f)
{
	if(m_scene.valid())
		m_sceneid = m_scene->getSceneID();
	else
	{
		m_sceneid = 0;
	}
}
crRoom::~crRoom()
{

}
crScene *crRoom::getScene()
{
	return m_scene.get();
}
int crRoom::getSceneID()
{
	return m_sceneid;
}
void crRoom::setGameMode(char gameMode)
{
	m_gameMode = (GameMode)gameMode;
}
unsigned char crRoom::getGameMode()
{
	return m_gameMode;
}
void crRoom::setRoomID(int id)
{
	m_roomid = id;
}
int crRoom::getRoomID()
{
	return m_roomid;
}
void crRoom::setName(const std::string &name)
{
	m_name = name;
}
const std::string &crRoom::getName() const
{
	return m_name;
}
void crRoom::setPassword(const std::string &password)
{
	m_password = password;
}
const std::string &crRoom::getPassword() const
{
	return m_password;
}
int crRoom::getGameTime()
{
	return m_gameTime;
}
bool crRoom::getFriendlyFire()
{
	return m_friendlyFire;
}
void crRoom::setMaxPlayerCount(short count)
{
	m_maxPlayerCount = count;
}
short crRoom::getMaxPlayerCount()
{
	return m_maxPlayerCount;
}
void crRoom::setMinPlayerCount(short count)
{
	m_minPlayerCount = count;
}
short crRoom::getMinPlayerCount()
{
	return m_minPlayerCount;
}
void crRoom::setShareSight(bool shareSight)
{
	m_shareSight = shareSight;
}
bool crRoom::getShareSight()
{
	return m_shareSight;
}
void crRoom::assignGroupID(crRoomPlayer *roomplayer)
{
	if(roomplayer->getGroupID() == 0)
	{
		switch (m_gameMode)
		{
		case Melee:
			roomplayer->setGroupID(1);
			break;
		case TwoGroup:
		case CTF:
		case Dota:
			if(getPlayerCount()%2 == 0)
			{
				roomplayer->setGroupID(2);
			}
			else
			{
				roomplayer->setGroupID(3);
			}
			break;
		}
	}
	//将我的groupid同步给房间内的所有玩家
	
}
int crRoom::isEnemy(crInstanceItem *thisItem,crInstanceItem *targetItem)
{//npc和npc间是队友关系
	if(hasHandle(WCH_EnemyCheck))
	{
		char enemy = 0;
		std::pair<crInstanceItem *,crInstanceItem *> ItemPair(thisItem,targetItem);
		doEvent(WCH_EnemyCheck,MAKEINT64(&ItemPair,&enemy));
		return enemy;
	}
	else
	{
		unsigned char targetGroupid = 0;
		unsigned char thisGroupid = 0;
		if(targetItem->getItemtype() == crInstanceItem::Role)
		{
			crRole *targetRole = dynamic_cast<crRole *>(targetItem);
			ref_ptr<crRoomPlayer> roomPlayer = getMember(targetRole->getPlayerID());
			if(roomPlayer.valid())
			{
				targetGroupid = roomPlayer->getGroupID();
				if(targetGroupid == 0)
				{
					return 0;
				}
				else if(targetGroupid == 1)
				{
					return -1;
				}
			}
		}
		if(thisItem->getItemtype() == crInstanceItem::Role)
		{
			crRole *thisRole = dynamic_cast<crRole *>(thisItem);
			ref_ptr<crRoomPlayer> roomPlayer = getMember(thisRole->getPlayerID());
			if(roomPlayer.valid())
			{
				thisGroupid = roomPlayer->getGroupID();
				if(thisGroupid == 0)
				{
					return 0;
				}
				else if(thisGroupid == 1)
				{
					return -1;
				}
			}
		}
		if(targetGroupid != thisGroupid)
			return -1;
		return 1;
	}
}
bool crRoom::addMember(crRoomPlayer *roomPlayer/*,bool send*/)
{
	if(roomPlayer && isCanJoin())
	{
		GNE::LockMutex lock( m_playerListMutex );
		bool found = false;
		crRoomPlayer *rp;
		for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
		{
			rp = itr->get();
			if(rp->getPlayerID() == roomPlayer->getPlayerID())
			{
				//*itr = (crRoomPlayer*)NULL;
				*itr = roomPlayer;
				found = true;
				break;
			}
		}
		if(!found) m_playerList.push_back(roomPlayer);
		//if(send)
		//{//通知房间里的其它成员
		//	ref_ptr<crStreamBuf> stream;
		//	roomPlayer->getStream(stream);
		//	crRoomAddMemberPacket packet;
		//	crRoomAddMemberPacket::buildReplyPacket(packet,0,stream.get());
		//	sendPacketToAll(packet,roomPlayer->getPlayerID());
		//}
		return true;
	}
	return false;
}
void crRoom::removeMember(int playerid,bool send)
{
	CRCore::ref_ptr<crPlayerGameData> playerGameData;
	{
		GNE::LockMutex lock( m_playerListMutex );
		crRoomPlayer *roomPlayer;
		bool found = false;
		for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
		{
			roomPlayer = itr->get();
			if(roomPlayer->getPlayerID() == playerid)
			{
				playerGameData = roomPlayer->getPlayerGameData();
				m_playerList.erase(itr);
				found = true;
				break;
			}
		}
		if(!found)
			return;
		if(playerid == m_roomHost)
		{
			if(!m_playerList.empty())
			{
				roomPlayer = m_playerList.begin()->get();
				setRoomHost(roomPlayer->getPlayerID());
			}
			else
				setRoomHost(0);
		}
		if(send && !m_playerList.empty())
		{//通知房间里的其它成员
			crRoomRemoveMemberPacket packet;
			crRoomRemoveMemberPacket::buildReplyPacket(packet,0,playerid);
			sendPacketToAll(packet,playerid);
		}
	}
	doEvent(WCH_RoomPlayerLeave,MAKEINT64(playerid,playerGameData.get()));
}
crRoomPlayer *crRoom::getMember(int playerid)
{
	GNE::LockMutex lock( m_playerListMutex );
	crRoomPlayer *roomPlayer;
	for( PlayerList::iterator itr = m_playerList.begin();
		itr != m_playerList.end();
		++itr )
	{
		roomPlayer = itr->get();
		if(roomPlayer->getPlayerID() == playerid)
			return roomPlayer;
	}
	return NULL;
}
int crRoom::getMemberIndex(int playerid)
{
	GNE::LockMutex lock( m_playerListMutex );
	crRoomPlayer *roomPlayer;
	int i = 0;
	for( PlayerList::iterator itr = m_playerList.begin();
		itr != m_playerList.end();
		++itr,++i )
	{
		roomPlayer = itr->get();
		if(roomPlayer->getPlayerID() == playerid)
			break;
	}
	return i;
}
void crRoom::lockPlayerList()
{
	m_playerListMutex.acquire();
}
crRoom::PlayerList &crRoom::getPlayerList()
{
	return m_playerList;
}
void crRoom::unlockPlayerList()
{
	m_playerListMutex.release();
}
int crRoom::getPlayerCount()
{
	GNE::LockMutex lock( m_playerListMutex );
	//int numPlayer = 0;
	//for( PlayerList::iterator itr = m_playerList.begin();
	//	itr != m_playerList.end();
	//	++itr )
	//{
	//	if(itr->valid())
	//		numPlayer++;
	//}
	//return numPlayer;
	return m_playerList.size();
}
void crRoom::setRoomHost(int playerid)
{
	m_roomHost = playerid;
}
int crRoom::getRoomHost()
{
	return m_roomHost;
}
void crRoom::setGameRunning(bool gameRunning)
{
	if(m_gameRunning != gameRunning)
	{
		m_gameRunning = gameRunning;
		if(m_gameRunning)
			m_timeRemain = m_gameTime;
		else
			m_timeRemain = 0.0f;
	}
}
bool crRoom::getGameRunning()
{
	return m_gameRunning;
}
bool crRoom::isCanJoin()
{
	return (m_autoStart/*||!m_gameRunning*/) && (m_maxPlayerCount==-1 || getPlayerCount()<m_maxPlayerCount);
}
void crRoom::clear()
{
	GNE::LockMutex lock( m_playerListMutex );
    m_playerList.clear();
}
void crRoom::pauseRoom(float time)
{
	m_pauseTimer = time;
	if(!crGlobalHandle::isClient())
	{
		crPauseRoomGamePacket packet;
		crPauseRoomGamePacket::buildReplyPacket(packet,0,m_pauseTimer);
		sendPacketToAll(packet);
	}
}
bool crRoom::isPaused()
{
	return m_pauseTimer>0.0f;
}
void crRoom::serverUpdate(float dt,crSceneServerCallback *sc)
{
	roomMessageDispose();
	if(m_pauseTimer>0.0f)
	{
		m_pauseTimer-=dt;
		if(m_pauseTimer<=0.0f)
		{
			pauseRoom(0.0f);
		}
		return;
	}
	if(m_gameRunning)
	{
		//float timeRemain = m_timeRemain;
		if(m_gameTime == -1)
			m_timeRemain = FLT_MAX;
		else
			m_timeRemain-=dt;
		if(m_timeRemain<=0.0f)
		{
			m_timeRemain = 0.0f;
			m_gameRunning = false;
			{
				GNE::LockMutex lock( m_sightInfoMapMutex );
				for( SightInfoMap::iterator itr = m_sightInfoMap.begin();
					itr != m_sightInfoMap.end();
					++itr )
				{
					itr->second->clearAllInsights();
				}
			}
			{
				GNE::LockMutex lock( m_sightInfoSetMutex );
				ref_ptr<crSightInfo> sightinfo;
				for( SightInfoSet::iterator itr = m_sightInfoSet.begin();
					itr != m_sightInfoSet.end();
					++itr )
				{
					sightinfo = *itr;
					sightinfo->clearAllInsights();
				}
			}
			doEvent(WCH_RoomEnd,MAKEINT64(NULL,sc));
			//sc->endRoomGame(this);
			crEndRoomGamePacket packet;
			crEndRoomGamePacket::buildReplyPacket(packet,0);
			sendPacketToAll(packet);
		}
		else
		{
			//if((int)timeRemain-(int)m_timeRemain != 0)
			//{
			//	crRoomGameTimePacket packet;
			//	crRoomGameTimePacket::buildReplyPacket(packet,0,m_timeRemain);
			//	sendPacketToAll(packet);
			//}
			//sightupdate
			if(m_scene.valid())
			{
				CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
				CRCore::Timer_t end_tick;
				int roleeyecount = 0;
				int itemeyecount = 0;
				{
					GNE::LockMutex lock( m_sightInfoMapMutex );
					for( SightInfoMap::iterator itr = m_sightInfoMap.begin();
						itr != m_sightInfoMap.end();
						++itr )
					{
						itr->second->update(m_scene.get(),m_roomid,roleeyecount,itemeyecount);
					}
				}
				{
					GNE::LockMutex lock( m_sightInfoSetMutex );
					ref_ptr<crSightInfo> sightinfo;
					for( SightInfoSet::iterator itr = m_sightInfoSet.begin();
						itr != m_sightInfoSet.end();
						++itr )
					{
						sightinfo = *itr;
						sightinfo->update(m_scene.get(), m_roomid,roleeyecount, itemeyecount);
					}
				}
				end_tick = CRCore::Timer::instance()->tick();
				float t = CRCore::Timer::instance()->delta_s(start_tick, end_tick);
				if (t > 0.08f)
				{
					time_t localt = time(0);
					char tmp[20];
					strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&localt));
					CRCore::notify(CRCore::WARN) << tmp << " SceneName=" << m_scene->getSceneName() << " Time to SightUpdate=" << t << " sightInfoMap=" << m_sightInfoMap.size() << " sightInfoSet=" << m_sightInfoSet.size() << " roleeyecount=" << roleeyecount << " itemeyecount=" << itemeyecount << std::endl;
				}
				float _dt = dt;
				doEvent(WCH_RoomUpdate,MAKEINT64(&_dt,sc));
				start_tick = end_tick;
				end_tick = CRCore::Timer::instance()->tick();
				t = CRCore::Timer::instance()->delta_s(start_tick, end_tick);
				if (t > 0.02f)
				{
					time_t localt = time(0);
					char tmp[20];
					strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&localt));
					CRCore::notify(CRCore::WARN) << tmp << " SceneName=" << m_scene->getSceneName() << " Time to RoomUpdate=" << t << std::endl;
				}
			}
			else
			{
				m_timeRemain = 0.0f;
			}
		}
	}
	else if(m_autoStart/* && allMemberIsReady()*/)
	{
		m_timeRemain -= dt;
		if(m_timeRemain<=-m_autoStartTime)
		{
			m_timeRemain = 0.0f;
			sc->startRoomGame(this);
			startGame();
			doEvent(WCH_RoomStart,MAKEINT64(NULL,sc));
		}
	}
	else
	{
		m_timeRemain -= dt;
		if(m_timeRemain<=-m_removeTime)
		{
			m_timeRemain = 0.0f;
			doEvent(WCH_RoomRemoved,MAKEINT64(NULL,sc));
			crRemoveRoomGamePacket packet;
			crRemoveRoomGamePacket::buildReplyPacket(packet,0);
			sendPacketToAll(packet);
			m_canRemove = true;
		}
	}
}
void crRoom::setRemoveTime(float removetime)
{
	m_removeTime = removetime;
}
bool crRoom::getCanRemove()
{
	return m_canRemove && getPlayerCount() == 0;
}
bool crRoom::allMemberIsReady()
{
	GNE::LockMutex lock( m_playerListMutex );
	if(m_playerList.size()<getMinPlayerCount())
		return false;
	crRoomPlayer *roomPlayer;
	for( PlayerList::iterator itr = m_playerList.begin();
		itr != m_playerList.end();
		++itr )
	{
		roomPlayer = itr->get();
		if(!roomPlayer->getReady())
		{
			return false;
		}
	}
	return true;
}
//void crRoom::setRemainTime(float time)
//{
//	m_timeRemain = time;
//}
//float crRoom::getRemainTime()
//{
//	return m_timeRemain;
//}
void crRoom::sendPacketToAll(CRNet::crStreamPacket &packet,int exceptPlayerid)
{
	GNE::LockMutex lock( m_playerListMutex );
	if(!m_playerList.empty())
	{
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetManager *netManager = sceneServer->getNetManager();
		crNetDataManager *netDataManager = sceneServer->getNetDataManager();
		ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
		int bufsize = packetStream->getBufSize();
		int playerid;
		ref_ptr<crSceneServerPlayerData> playerData;
		crRoomPlayer *roomPlayer;
		for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
		{
			roomPlayer = itr->get();
			playerid = roomPlayer->getPlayerID();
			if(playerid == exceptPlayerid)
				continue;
			playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid()/* && playerData->isClientRunning() && roomPlayer->getReady() && playerData->getRoomID() == m_roomid*/)
			{
				packetStream->seekBegin();
				packetStream->_writeInt(playerid);
				packetStream->setBufSize(bufsize);
				netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
}
void crRoom::sendPacketToGroup(unsigned char groupid, CRNet::crStreamPacket &packet,int exceptPlayerid)
{
	GNE::LockMutex lock( m_playerListMutex );
	if(!m_playerList.empty())
	{
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetManager *netManager = sceneServer->getNetManager();
		crNetDataManager *netDataManager = sceneServer->getNetDataManager();
		ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
		int bufsize = packetStream->getBufSize();
		int playerid;
		ref_ptr<crSceneServerPlayerData> playerData;
		crRoomPlayer *roomPlayer;
		for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
		{
			roomPlayer = itr->get();
			if(roomPlayer->getGroupID() == groupid)
			{
				playerid = roomPlayer->getPlayerID();
				if(playerid == exceptPlayerid)
					continue;
				playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
				if(playerData.valid()/* && playerData->isClientRunning() && roomPlayer->getReady() && playerData->getRoomID() == m_roomid*/)
				{
					packetStream->seekBegin();
					packetStream->_writeInt(playerid);
					packetStream->setBufSize(bufsize);
					netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
	}
}
void crRoom::sendRoomMessage(CRNet::crStreamPacket *packet, unsigned char groupid, int exceptPlayerid)
{
	ref_ptr<RoomMessage> roomMessage = new RoomMessage(packet, groupid, exceptPlayerid);
	m_roomMessageDequeMutex.acquire();
	m_roomMessageDeque.push_back(roomMessage);
	m_roomMessageDequeMutex.release();
}
void crRoom::roomMessageDispose()
{
	ref_ptr<RoomMessage> groupMessage;
	m_roomMessageDequeMutex.acquire();
	if (!m_roomMessageDeque.empty())
	{
		groupMessage = m_roomMessageDeque.front().get();
		m_roomMessageDeque.pop_front();
	}
	m_roomMessageDequeMutex.release();
	if (groupMessage.valid())
	{
		GNE::LockMutex lock(m_playerListMutex);
		if (!m_playerList.empty())
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetManager *netManager = sceneServer->getNetManager();
			crNetDataManager *netDataManager = sceneServer->getNetDataManager();
			ref_ptr<crStreamBuf> packetStream = groupMessage->m_packet->getStreamBuf();
			int bufsize = packetStream->getBufSize();
			int playerid;
			ref_ptr<crSceneServerPlayerData> playerData;
			crRoomPlayer *roomPlayer;
			for (PlayerList::iterator itr = m_playerList.begin();
				itr != m_playerList.end();
				++itr)
			{
				roomPlayer = itr->get();
				playerid = roomPlayer->getPlayerID();
				if (playerid == groupMessage->m_exceptPlayerid)
					continue;
				if (groupMessage->m_groupid==0 || roomPlayer->getGroupID() == groupMessage->m_groupid)
				{
					playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
					if (playerData.valid()/* && playerData->isClientRunning() && roomPlayer->getReady() && playerData->getRoomID() == m_roomid*/)
					{
						packetStream->seekBegin();
						packetStream->_writeInt(playerid);
						packetStream->setBufSize(bufsize);
						netManager->sendPacket(playerData->getPlayerConnectServerAddress(), *(groupMessage->m_packet));
					}
				}
			}
		}
	}
}
void crRoom::playerJoined(crRoomPlayer *roomplayer)
{
	if(roomplayer)
	{
		//assignGroupID(roomplayer);
		//roomplayer->setReady(true);
		//crRoomPlayerReadyPacket packet;
		//crRoomPlayerReadyPacket::buildReplyPacket(packet,0,roomplayer->getPlayerID(),roomplayer->getGroupID(),roomplayer->getReady());
		//sendPacketToAll(packet);
		//if(getGameRunning())
		//{
		//	startGame(roomplayer);
		//}

		//通知房间里的其它成员
		ref_ptr<crStreamBuf> stream;
		roomplayer->getStream(stream);
		crRoomAddMemberPacket packet;
		crRoomAddMemberPacket::buildReplyPacket(packet,0,stream.get());
		sendPacketToAll(packet,roomplayer->getPlayerID());
	}
}
void crRoom::setReady(crRoomPlayer *roomplayer)
{
	if(roomplayer)
	{
		GNE::LockMutex lock( m_playerListMutex );
		roomplayer->setReady(true);
		//crRoomPlayerReadyPacket packet;
		//crRoomPlayerReadyPacket::buildReplyPacket(packet,0,roomplayer->getPlayerID(),roomplayer->getGroupID(),roomplayer->getReady());
		//sendPacketToAll(packet);
		if(getGameRunning())
		{
			startGame(roomplayer);
		}
	}
}
//void crRoom::lockSightInfoMap()
//{
//	m_sightInfoMapMutex.acquire();
//}
crSightInfo* crRoom::getOrCreateSightInfo(unsigned char groupid/*, bool npcshare*/)
{
	//unsigned char groupid = member->getGroupID();
	if(groupid==0)
		return NULL;//中立没有视野
	if(m_shareSight /*|| npcshare*/ || groupid==2 )
	{//NPC队伍的采用共享视野
		GNE::LockMutex lock( m_sightInfoMapMutex );
		crSightInfo *sightInfo;
		SightInfoMap::iterator itr = m_sightInfoMap.find(groupid);
		if(itr != m_sightInfoMap.end())
		{
			sightInfo = itr->second.get();
		}
		else
		{
			sightInfo = new crSightInfo;
			sightInfo->setRoomID(m_roomid);
			//sightInfo->setRoomSight();
			m_sightInfoMap[groupid] = sightInfo;
			m_scene->insertSightInfo(sightInfo);
		}
		return sightInfo;
	}
	else
	{
		GNE::LockMutex lock( m_sightInfoSetMutex );
		crSightInfo *sightInfo = new crSightInfo;
		sightInfo->setRoomID(m_roomid);
		m_sightInfoSet.insert(sightInfo);
		m_scene->insertSightInfo(sightInfo);
		return sightInfo;
	}
	return NULL;
}
void crRoom::removeSightInfo(crSightInfo *sightinfo)
{
	if(!m_shareSight && sightinfo->isEmptySight())
	{
		GNE::LockMutex lock( m_sightInfoSetMutex );
		m_sightInfoSet.erase(sightinfo);
		m_scene->removeSightInfo(sightinfo);
	}
}
void crRoom::removeRoom()
{
	{
		GNE::LockMutex lock( m_sightInfoMapMutex );
		for( SightInfoMap::iterator itr = m_sightInfoMap.begin();
			itr != m_sightInfoMap.end();
			++itr )
		{
			m_scene->removeSightInfo(itr->second.get());
		}
	}
	{
		GNE::LockMutex lock( m_sightInfoSetMutex );
		ref_ptr<crSightInfo> sightinfo;
		for( SightInfoSet::iterator itr = m_sightInfoSet.begin();
			itr != m_sightInfoSet.end();
			++itr )
		{
			sightinfo = *itr;
			m_scene->removeSightInfo(sightinfo.get());
		}
	}
	clearData();
}
//void crRoom::unlockSightInfoMap()
//{
//	m_sightInfoMapMutex.release();
//}
void crRoom::sendStartGame(crNetManager *netManager,crNetDataManager *netDataManager,crRoomPlayer *roomPlayer)
{
	int playerid = roomPlayer->getPlayerID();
	ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
	if(playerData.valid() && playerData->isClientRunning() && roomPlayer->getReady() && playerData->getRoomID() == m_roomid)
	{
		crRole *role;
		//playerData->lockRoleMap();
		crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
		int count = roleMap.size();
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(count * 16 + 9);
		stream->_writeInt(playerid);
		stream->_writeChar(count);
		for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
			itr != roleMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			role = itr->second.get();
			if(role)
			{
				crData *roleData = role->getDataClass();
				roleData->excHandle(MAKEINT64(WCH_InitData,role));
				//unsigned char itemstate = IS_Stop;
				//roleData->inputParam(WCHDATA_ItemState, &itemstate);
				//m_scene->gainBirthPoint(role,getBirthPointIndex(roomPlayer));
				doEvent(WCH_GainBirthPoint,MAKEINT64(role,getBirthPointIndex(roomPlayer)));
				stream->_writeFloat(role->getPosx());
				stream->_writeFloat(role->getPosy());
				stream->_writeFloat(role->getPosz());
			}
			else
			{
				stream->_writeFloat(0);
				stream->_writeFloat(0);
				stream->_writeFloat(0);
			}
		}
		stream->_writeFloat(m_timeRemain);
		//playerData->unlockRoleMap();
		crStartRoomGamePacket packet;
		crStartRoomGamePacket::buildReplyPacket(packet,stream.get());
		netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

		doEvent(WCH_AddPlayer,MAKEINT64(playerData.get(),NULL));
	}
}
void crRoom::startGame(crRoomPlayer *roomPlayer)
{//需要为每个玩家初始化位置
	if(!m_scene.valid()) return;
	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetManager *netManager = sceneServer->getNetManager();
	crNetDataManager *netDataManager = sceneServer->getNetDataManager();
	GNE::LockMutex lock( m_playerListMutex );
	if(roomPlayer && roomPlayer->getReady())
	{
		sendStartGame(netManager,netDataManager,roomPlayer);
	}
	else
	{
		setGameRunning(true);
		for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
		{
			roomPlayer = itr->get();
			if(roomPlayer->getReady())
			{
				sendStartGame(netManager,netDataManager,roomPlayer);
			}
		}
	}
}
int crRoom::getBirthPointIndex(crRoomPlayer *roomplayer)
{
	if(roomplayer)
	{
		char index = roomplayer->getBirthPointIndex();
		if(index != -1)
		{
			return index;
		}
		switch (m_gameMode)
		{
		case Melee:
			//return getMemberIndex(roomplayer->getPlayerID());
			return -1;
		case TwoGroup:
		case CTF:
		case Dota:
			return roomplayer->getGroupID()==2?0:1;
		}
	}
	return -1;
}
void crRoom::setCreateRoomData(CRCore::crStreamBuf *stream)
{
	m_gameMode = (GameMode)(stream->_readChar());
	m_name = stream->_readString();
	if(m_name.size()>16)
		m_name = m_name.substr(0,16);
	m_gameTime = stream->_readInt();
	m_maxPlayerCount = stream->_readShort();
	m_friendlyFire = stream->_readBool();
	//m_autoStart = stream->_readBool();
	//m_autoStartTime = stream->_readFloat();
	if(!stream->eof())
	{
		m_password = stream->_readString();
		if(m_password.size()>8)
			m_password = m_password.substr(0,8);
	}
}
void crRoom::getRoomData(CRCore::ref_ptr<CRCore::crStreamBuf> &stream)
{
	GNE::LockMutex lock( m_playerListMutex );
	int count = m_playerList.size();
	stream = new crStreamBuf;
	stream->createBuf(59 + count * 25);
	stream->_writeInt(m_sceneid);//4
	stream->_writeInt(m_roomid);//4
	stream->_writeInt(getRoomHost());//4

	stream->_writeChar(m_gameMode);//1
	stream->_writeString(m_name);//16+4
	stream->_writeInt(m_gameTime);//4
	stream->_writeBool(m_friendlyFire);//1
	//stream->_writeBool(m_autoStart);//1
	//stream->_writeFloat(m_autoStartTime);//4
	stream->_writeShort(m_maxPlayerCount);//2
	stream->_writeString(m_password);//8+4

	stream->_writeUChar(count);
	crRoomPlayer *roomPlayer;
	for( PlayerList::iterator itr = m_playerList.begin();
			itr != m_playerList.end();
			++itr )
	{
		roomPlayer = itr->get();
		stream->_writeInt(roomPlayer->getPlayerID());
		stream->_writeString(roomPlayer->getCharacterName());//20
		stream->_writeUChar(roomPlayer->getGroupID());
	}
	stream->_writeBool(m_shareSight);
}
void crRoom::setRoomData(CRCore::crStreamBuf *stream)
{
	GNE::LockMutex lock( m_playerListMutex );
	m_sceneid = stream->_readInt();
	m_roomid = stream->_readInt();
	m_roomHost = stream->_readInt();

	m_gameMode = stream->_readChar();
	m_name = stream->_readString();
	m_gameTime = stream->_readInt();
	m_friendlyFire = stream->_readBool();
	//m_autoStart = stream->_readBool();//1
	//m_autoStartTime = stream->_readFloat();//4
	m_maxPlayerCount = stream->_readShort();
	m_password = stream->_readString();

	int count = stream->_readUChar();
	m_playerList.resize(count,(crRoomPlayer *)NULL);
	crRoomPlayer *roomPlayer;
	int playerid;
	for( PlayerList::iterator itr = m_playerList.begin();
		itr != m_playerList.end();
		++itr )
	{
		playerid = stream->_readInt();
		if(playerid!=0)
		{
			roomPlayer = new crRoomPlayer;
			roomPlayer->setPlayerID(playerid);
			roomPlayer->setCharacterName(stream->_readString());
			roomPlayer->setGroupID(stream->_readUChar());
			*itr = roomPlayer;
		}
	}
	m_shareSight = stream->_readBool();
}
void crRoom::setAutoStart(bool autostart,float autostarttime)
{
	m_autoStart = autostart;
	m_autoStartTime = autostarttime;
}
///////////////////////////////////////////////////////////
void crRoom::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crRoom::getDataClass()
{
	return m_dataClass.get();
}
void crRoom::doEvent(_crInt64 kbmsg, _crInt64 param)
{
	m_currentMsg = kbmsg;
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch(...)
			{
				CRCore::notify(CRCore::ALWAYS)<<"crRoom::doEvent error msg = "<<kbmsg<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}
bool crRoom::hasHandle(_crInt64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crRoom::clearData()
{
	m_dataClass = NULL;
}
///////////////////////////////////
//
//crGameTask
//
/////////////////////////////////////
crGameTask::crGameTask():
m_taskid(0),
m_taskSteps(0),
m_taskProgress(0),
m_activation(0),
m_exp(0),
m_exp2(0),
m_gold(0),
m_preTask(0),
m_circles(1),
m_taskStepType(TS_Zero)
{
}
crGameTask::~crGameTask()
{

}
crGameTask& crGameTask::operator = (const crGameTask& gametask)
{
	m_taskid          = gametask.m_taskid;
	m_taskName        = gametask.m_taskName;
	m_hypertext       = gametask.m_hypertext;
	m_taskSteps       = gametask.m_taskSteps;  
	m_taskProgress    = gametask.m_taskProgress;
	m_activation      = gametask.m_activation;
	m_exp             = gametask.m_exp;
	m_exp2            = gametask.m_exp2;
	m_gold            = gametask.m_gold;
	//m_awardItemMap    = gametask.m_awardItemMap;
	m_lvRange         = gametask.m_lvRange;
	m_preTask         = gametask.m_preTask;
	m_circles         = gametask.m_circles;
	m_taskStepType    = gametask.m_taskStepType;
	m_dataClass       = gametask.m_dataClass;
	return (*this);
}
void crGameTask::setTaskID(int id)
{
	m_taskid = id;
}
int crGameTask::getTaskID()
{
	return m_taskid;
}
void crGameTask::setTaskName(const std::string &name)
{
	m_taskName = name;
}
std::string &crGameTask::getTaskName() 
{
	return m_taskName; 
}
void crGameTask::setHyperText(const std::string &filename)
{
	m_hypertext = filename;
}
const std::string &crGameTask::getHyperText() const
{
	return m_hypertext; 
}
void crGameTask::setTaskSteps(const CRCore::crVector4b &steps)
{
	m_taskSteps = MAKEINT32(MAKEINT16(steps[0],steps[1]),MAKEINT16(steps[2],steps[3]));
}
void crGameTask::setTaskSteps(int steps)
{
	m_taskSteps = steps;
}
int crGameTask::getTaskSteps()
{
	return m_taskSteps; 
}
void crGameTask::setExp(int exp)
{
	m_exp = exp;
}
int crGameTask::getExp()
{
	return m_exp;
}
void crGameTask::setExp2(int exp)
{
	m_exp2 = exp;
}
int crGameTask::getExp2()
{
	return m_exp2;
}
void crGameTask::setGold(int gold)
{
	m_gold = gold;
}
int crGameTask::getGold()
{
	return m_gold;
}
//void crGameTask::addAwardItemMap(int abstractid/*,float probability*/)
//{
//	//m_awardItemMap[abstractid] = probability;
//	if(abstractid>0)
//	{
//		m_awardItemMap.insert(std::make_pair(abstractid,(crInstanceItem*)0));
//	}
//}
//bool crGameTask::setAwardItem(crInstanceItem *item)
//{
//	AwardItemMap::iterator itr = m_awardItemMap.find(item->getAbstractItemID());
//	if(itr != m_awardItemMap.end())
//	{
//		itr->second = item;
//		return true;
//	}
//	return false;
//}
//bool crGameTask::getAwardItem(int abstractid,CRCore::ref_ptr<crInstanceItem> &outitem)
//{
//	AwardItemMap::iterator itr = m_awardItemMap.find(abstractid);
//	if(itr != m_awardItemMap.end())
//	{
//		outitem = itr->second.get();
//		return true;
//	}
//	return false;
//}
//AwardItemMap &crGameTask::getAwardItemMap()
//{
//    return m_awardItemMap;
//}
void crGameTask::setActivation(bool activation)
{
	m_activation = activation?1:0;
}
bool crGameTask::getActivation()
{
	return m_activation==1; 
}
void crGameTask::setActivationFlg(char flg)
{
	m_activation = flg;
}
char crGameTask::getActivationFlg()
{
	return m_activation;
}
bool crGameTask::isNeedSave()
{
	return m_activation>0||m_circles>1;
}
void crGameTask::setTaskProgress(int progress)
{
    m_taskProgress = minimum(progress,m_taskSteps);
}
int crGameTask::getTaskProgress()
{
	if(m_taskProgress>m_taskSteps)
	{
		m_taskProgress = m_taskSteps;
	}
	return m_taskProgress; 
}
void crGameTask::resetTask()
{
	m_taskProgress = 0;
	m_activation = 0;
}
void crGameTask::refresh(int circles)
{
	resetTask();
	m_circles = circles;
}
bool crGameTask::isTaskCanRefer()
{//任务的最后一步就是提交任务
	return m_taskProgress == m_taskSteps-1;
}
bool crGameTask::isComplete()
{
	return m_activation == 2 || m_taskProgress == m_taskSteps;//m_taskProgress == m_taskSteps是为了兼容以前的
	//return m_taskProgress == m_taskSteps;
}
bool crGameTask::isTaskCanAdvance(unsigned char steptype)
{
	if(m_taskStepType == TS_Zero && steptype != TS_Zero)
		return false;
	if(m_taskStepType != TS_Zero && steptype == TS_Zero)
		return false;
	if(m_activation && m_taskProgress<m_taskSteps-1)
	{
		if(m_taskStepType == TS_Zero)
		{
			return true;
		}
		else
		{
			_crInt16 A,B;
			A = LOINT32(m_taskProgress);
			B = HIINT32(m_taskProgress);
			_crInt16 C,D;
			int step = m_taskSteps-1;
			C = LOINT32(step);
			D = HIINT32(step);
			if(steptype == TS_One)
			{
				_crInt8 a = LOINT16(A);
				_crInt8 c = LOINT16(C);
				if(a<c)
				{
					return true;
				}
			}
			else if(steptype == TS_Two)
			{
				_crInt8 a = HIINT16(A);
				_crInt8 c = HIINT16(C);
				if(a<c)
				{
					return true;
				}
			}
			else if(steptype == TS_Three)
			{
				_crInt8 b = LOINT16(B);
				_crInt8 d = LOINT16(D);
				if(b<d)
				{
					return true;
				}
			}
			else if(steptype == TS_Four)
			{
				_crInt8 b = HIINT16(B);
				_crInt8 d = HIINT16(D);
				if(b<d)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool crGameTask::advance(unsigned char steptype)
{
	if(!isTaskCanAdvance(steptype))
		return false;
	bool changed = false;
	if(steptype<=m_taskStepType && m_taskProgress<m_taskSteps-1)
	{
		if(steptype == TS_Zero)
		{
			m_taskProgress++;
			changed = true;
		}
		else
		{
			_crInt16 A,B;
			A = LOINT32(m_taskProgress);
			B = HIINT32(m_taskProgress);
			_crInt16 C,D;
			int step = m_taskSteps-1;
			C = LOINT32(step);
			D = HIINT32(step);
			if(steptype == TS_One)
			{
				_crInt8 a = LOINT16(A);
				_crInt8 c = LOINT16(C);
				if(a<c)
				{
					a++;
					m_taskProgress = MAKEINT32(MAKEINT16(a,HIINT16(A)),B);
					changed = true;
				}
			}
			else if(steptype == TS_Two)
			{
				_crInt8 a = HIINT16(A);
				_crInt8 c = HIINT16(C);
				if(a<c)
				{
					a++;
					m_taskProgress = MAKEINT32(MAKEINT16(LOINT16(A),a),B);
					changed = true;
				}
			}
			else if(steptype == TS_Three)
			{
				_crInt8 b = LOINT16(B);
				_crInt8 d = LOINT16(D);
				if(b<d)
				{
					b++;
					m_taskProgress = MAKEINT32(A,MAKEINT16(b,HIINT16(B)));
					changed = true;
				}
			}
			else if(steptype == TS_Four)
			{
				_crInt8 b = HIINT16(B);
				_crInt8 d = HIINT16(D);
				if(b<d)
				{
					b++;
					m_taskProgress = MAKEINT32(A,MAKEINT16(LOINT16(B),b));
					changed = true;
				}
			}
		}
	}
	return changed;
}
void crGameTask::refer()
{
	if(isTaskCanRefer())
	{
		m_taskProgress++;
		m_activation = 2;
		if(m_circles>1)
		{
			m_circles--;
			resetTask();
		}
		else if(m_circles == 0)
		{
			resetTask();
		}
		//else//m_circles == 1
		//{
		//}
	}
}
void crGameTask::setLvRange(const CRCore::crVector2s &range)
{
	m_lvRange.set((unsigned char)(range[0]),(unsigned char)(range[1]));
}
const CRCore::crVector2ub &crGameTask::getLvRange()
{
	return m_lvRange;
}
void crGameTask::setPreTask(int preTask)
{
	m_preTask = preTask;
}
int crGameTask::getPreTask()
{
	return m_preTask;
}
void crGameTask::setCircles(unsigned char circles)
{
	m_circles = circles;
}
unsigned char crGameTask::getCircles()
{
	return m_circles;
}
void crGameTask::setTaskStepType(unsigned char steptype)
{
	m_taskStepType = steptype;
}
unsigned char crGameTask::getTaskStepType()
{
	return m_taskStepType;
}
//bool crGameTask::isRequestAwardItem()
//{
//	if(/*getActivation() && */!isComplete())
//	{
//		for( AwardItemMap::iterator itr = m_awardItemMap.begin();
//			itr != m_awardItemMap.end();
//			++itr )
//		{
//			if(!itr->second.valid())
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
///////////////////////////////////
//
//crQueryServerListData
//
/////////////////////////////////////
crQueryServerListData::crQueryServerListData():
	m_serverid(0),
	m_isp(0),
	m_remaincount(0)
{
}
void crQueryServerListData::setServerID(int serverid)
{
	m_serverid = serverid;
}
int crQueryServerListData::getServerID()
{
	return m_serverid;
}
void crQueryServerListData::setServerName(const std::string &servername)
{
	m_servername = servername;
}
const std::string &crQueryServerListData::getServerName()
{
	return m_servername;
}
void crQueryServerListData::setISP(unsigned char isp)
{
	m_isp = isp;
}
unsigned char crQueryServerListData::getISP()
{
	return m_isp;
}
void crQueryServerListData::setRemainCount(short count)
{
	m_remaincount = count;
}
short crQueryServerListData::getRemainCount()
{
	return m_remaincount;
}
///////////////////////////////////
//
//crQueryConnectServerData
//
/////////////////////////////////////
crQueryConnectServerData::crQueryConnectServerData():
	m_port(0),
	m_remaincount(0)
{
}
void crQueryConnectServerData::setIP(const std::string &ip)
{
	m_ip = ip;
}
const std::string &crQueryConnectServerData::getIP()
{
	return m_ip;
}
void crQueryConnectServerData::setIP2(const std::string &ip2)
{
	m_ip2 = ip2;
}
const std::string &crQueryConnectServerData::getIP2()
{
	return m_ip2;
}
void crQueryConnectServerData::setIP3(const std::string &ip3)
{
	m_ip3 = ip3;
}
const std::string &crQueryConnectServerData::getIP3()
{
	return m_ip3;
}
void crQueryConnectServerData::setPort(unsigned short port)
{
	m_port = port;
}
unsigned short crQueryConnectServerData::getPort()
{
	return m_port;
}
void crQueryConnectServerData::setRemainCount(short remaincount)
{
	m_remaincount = remaincount;
}
short crQueryConnectServerData::getRemainCount()
{
	return m_remaincount;
}