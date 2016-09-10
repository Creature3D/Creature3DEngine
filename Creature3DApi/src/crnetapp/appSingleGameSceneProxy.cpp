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
#include <CRNetApp/appSingleGameSceneProxy.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appDataParam.h>
#include <CRCore/crNotify.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appMsg.h>
#include <sys/stat.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CRIOManager;
///////////////////////////////////
//
//crSingleGameSceneProxy
//
/////////////////////////////////////
ref_ptr<crSingleGameSceneProxy> crSingleGameSceneProxy::m_instance = NULL;
crSingleGameSceneProxy::crSingleGameSceneProxy():
m_reliveInterval(2),
m_reliveTime(0)
{
}
crSingleGameSceneProxy* crSingleGameSceneProxy::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crSingleGameSceneProxy;
		crBrain::getInstance()->pushInstance(m_instance.get());
		CRUtil::crSceneView::registerExternUpdater(m_instance.get());
	}
	return m_instance.get();
}

void crSingleGameSceneProxy::clear()
{
	m_scene = NULL;
	m_instance = NULL;
}

void crSingleGameSceneProxy::initScene()
{
	const std::string &gameName = crMyPlayerData::getInstance()->getSelectedGame()->getGameName();
	crQuerySceneInfoData *sceneInfoData = crMyPlayerData::getInstance()->getSceneInfoData();
	const std::string &sceneName = sceneInfoData->getSceneName();
	std::string file = gameName + "/script/scene/" + sceneName + ".cfg";
	crMyPlayerData::getInstance()->createScene();
	m_scene = crMyPlayerData::getInstance()->getScene();
	//m_scene = new crScene;
	//m_scene->setGameID(0);
	//m_scene->setGameName(gameName);
	//m_scene->setSceneID(0);
	//m_scene->setSceneName(sceneName);
	//m_scene->setCollideFile(sceneInfoData->getCollideFile());
	//m_scene->buildSceneLayer();

	rcfg::ConfigScript cfg_script;
	if(!CRIOManager::crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crSingleGameSceneProxy()::initScene(): file open error,"<<file<<std::endl;
		return;
	}

	std::string str;
	int nItem = 1;
	int int1;
	float flt1;
	ref_ptr<crInstanceItem> item;
	crVector3 dir;
	int posx,posy;
	while(cfg_script.Push("instanceitem",nItem++))
	{
		item = new crInstanceItem;
		item->setAbstractItemID(0);
		int1 = nItem-1;
		cfg_script.Get("itemid", int1);
		item->setInstanceItemID(int1);
		if(cfg_script.Get("itemtype", int1))
		    item->setItemtype(int1);
		if(cfg_script.Get("iname", str))
			item->setIName(str);
		cfg_script.Get("posx", posx);
		cfg_script.Get("posy", posy);
		item->setPosxy(posx,posy);
		if(cfg_script.Get("zoffset", int1))
			item->setZoffset(int1);
		if(cfg_script.Get("dirx", flt1))
			dir[0] = flt1;
		if(cfg_script.Get("diry", flt1))
			dir[1] = flt1;
		if(cfg_script.Get("dirz", flt1))
			dir[2] = flt1;
		item->setDir(dir);
		if(cfg_script.Get("name", str))
			item->getAbstractItem()->setName(str);
		if(cfg_script.Get("script", str))
			item->getAbstractItem()->setScript(str);
		if(cfg_script.Get("filename", str))
			item->getAbstractItem()->setFileName(str);
		if(cfg_script.Get("resscript", str))
			item->getAbstractItem()->setResScript(str);
		float posz = m_scene->getPosZ(item->getLayerID(),item->getPosx() * crGlobalHandle::gData()->gUnitScale(),item->getPosy() * crGlobalHandle::gData()->gUnitScale(), item->getZoffset() *crGlobalHandle::gData()->gUnitScale());
		item->setPosz(posz / crGlobalHandle::gData()->gUnitScale());
        item->loadItemData(SaveStream);
        m_scene->insertSceneItem(item.get());
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crSingleGameSceneProxy::initScene(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

crScene *crSingleGameSceneProxy::getScene()
{
	return m_scene.get();
}

void crSingleGameSceneProxy::operator()(float dt)
{
	if(!m_scene.valid())
		return;
	crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
	if(!role)
		return;
	crScene::SceneItemMap &sceneItemMap = m_scene->getSceneItemMap();
	crNode *node = role->getRelNode();
	if(!node)
		return;
	role->doEvent(WCH_NodeCoordToItem,MAKEINT64(NULL,node));

	crInstanceItem *item;
	crData *itemData;
	void *param;
	unsigned char itemstate;
	float *rthp;
	unsigned char itemtype;
	int id,roleid;
	//role->doEvent(WCH_ServerUpdate,MAKEINT64(&dt,NULL));
	for( crScene::SceneItemMap::iterator sitr = sceneItemMap.begin();
		sitr != sceneItemMap.end();
		++sitr )
	{
		item = sitr->second.get();
		itemData = item->getDataClass();
		itemData->getParam(WCHDATA_RTHP,param);
		rthp = (float*)param;
		if(rthp && *rthp<=0)
		{//IS_Dead
			itemtype = item->getItemtype();
			id = item->getID();
			roleid = item->getRoleID();
			crNode *relNode;
			if(itemtype == crInstanceItem::Role)
			{
				crMyPlayerData::RoleNpcPair roleNpcPair;
				if(crMyPlayerData::getInstance()->findInRangePlayer(id,roleid,roleNpcPair))
				{
					//roleNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
					relNode = roleNpcPair.first->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(false);
						relNode->setEnableIntersect(false);
					}
					crMyPlayerData::getInstance()->removeInRangeRole(id,roleid);
				}
			}
			else if(itemtype == crInstanceItem::Npc)
			{
				crMyPlayerData::ItemNpcPair itemNpcPair;
				if(crMyPlayerData::getInstance()->findInRangeNpc(id,itemNpcPair))
				{
					//itemNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
					relNode = itemNpcPair.first->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(false);
						relNode->setEnableIntersect(false);
					}
					crMyPlayerData::getInstance()->removeInRangeNpc(id);
				}
			}
			else
			{
				crMyPlayerData::ItemNodePair itemNodePair;
				if(crMyPlayerData::getInstance()->findInRangeItem(id,itemNodePair))
				{
					//itemNodePair->second->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
					relNode = itemNodePair.first->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(false);
						relNode->setEnableIntersect(false);
					}
					crMyPlayerData::getInstance()->removeInRangeItem(id);
				}
			}
			CRCore::notify(CRCore::ALWAYS)<<"ItemOutRange "<<std::endl;
		}
		else
		    role->doEvent(WCH_ItemInRangeTest,MAKEINT64(role,sitr->second.get()));

		itemData->getParam(WCHDATA_ItemState,param);
		itemstate = *(unsigned char*)param;
		if(itemstate == IS_Dead)
		{
			item->doEvent(MAKEINT64(WCH_DeadEvent,IR_ServerRefash),MAKEINT64(&dt,NULL));
		}
		else if(itemstate == IS_Relive)
		{
			m_reliveTime += dt;
			if(m_reliveTime>m_reliveInterval)
			{
				m_reliveTime = 0;
				itemstate = IS_Stop;
				itemData->inputParam(WCHDATA_ItemState,&itemstate);
			}
		}
		//else
		//{
		//	item->doEvent(WCH_ServerUpdate,MAKEINT64(&dt,NULL));
		//}
	}
}
