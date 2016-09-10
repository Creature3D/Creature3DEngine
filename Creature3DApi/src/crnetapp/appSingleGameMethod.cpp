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
#include <rbody/Creature3D/CreBody.h>
#include <rbody/Mesh.h>
#include <shellapi.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRNetApp/appSingleGameMethod.h>
#include <CRNetApp/appNetStartLoader.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appNetLogic.h>
#include <CRCore/crArgumentParser.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRCore/thread/crThread.h>
#include <CRIOManager/crFileUtils.h>
//#include "rcfg/ConfigScriptOO.h"
//#include <sys/stat.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRProducer/crViewer.h>
//#include <CRAL/crSoundState.h>
#include <CRPhysics/crBodyCaseMatterObject.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <CRCore/crSequence.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crLightSourceManager.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appSingleGameSceneProxy.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRGUI;
using namespace CRNetApp;
using namespace CRPhysics;
using namespace CRProducer;
//using namespace CRAL;
using namespace CREncapsulation;
using namespace CRProducer;
using namespace CRParticle;
using namespace rbody;

/////////////////////////////////////////
//
//crCreateSingleGameDataMethod
//
/////////////////////////////////////////
crCreateSingleGameDataMethod::crCreateSingleGameDataMethod():
m_rolecount(1),
m_zoffset(0){}
crCreateSingleGameDataMethod::crCreateSingleGameDataMethod(const crCreateSingleGameDataMethod& handle):
crMethod(handle),
m_gameName(handle.m_gameName),
m_gameCName(handle.m_gameCName),
m_rolecount(handle.m_rolecount),
m_roleName(handle.m_roleName),
m_sceneName(handle.m_sceneName),
m_sceneFileName(handle.m_sceneFileName),
m_collidefile(handle.m_collidefile),
m_metierName(handle.m_metierName),
m_metierScript(handle.m_metierScript),
m_tasktable(handle.m_tasktable),
m_resFileName(handle.m_resFileName),
m_resScript(handle.m_resScript),
m_abstractName(handle.m_abstractName),
m_abstractScript(handle.m_abstractScript),
m_zoffset(handle.m_zoffset)
{
}
void crCreateSingleGameDataMethod::inputParam(int i, void *param)
{
}

void crCreateSingleGameDataMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gameName = str;
		break;
	case 1:
		m_gameCName = str;
		break;
	case 2:
		m_rolecount = (unsigned char)(str.c_str());
		break;
	case 3:
		m_roleName = str;
		break;
	case 4:
		m_sceneName = str;
		break;
	case 5:
		m_sceneFileName = str;
		break;
	case 6:
		m_collidefile = str;
		break;
	case 7:
		m_metierName = str;
		break;
	case 8:
		m_metierScript = str;
		break;
	case 9:
		m_resFileName = str;
		break;
	case 10:
		m_resScript = str;
		break;
	case 11:
		m_abstractName = str;
		break;
	case 12:
		m_abstractScript = str;
		break;
	case 13:
		m_zoffset = (short)(atoi(str.c_str()));
		break;
	case 14:
		m_tasktable = str;
		break;
	}
}

void crCreateSingleGameDataMethod::operator()(crHandle &handle)
{
	ref_ptr<crData> data = CREncapsulation::loadData("script/clientevent.cfg");
	crGlobalHandle::getInstance()->setDataClass(data.get());
	data->excHandle(MAKEINT64(WCH_InitData,NULL));

	crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
	CRCore::ref_ptr<crQueryGameListData> queryGame = new crQueryGameListData;
	queryGame->setID(0);
	queryGame->setGameName(m_gameName);//"VR_ICBC"
	queryGame->setCName(m_gameCName);//"工行培训软件"
	queryGame->setRoleCount(m_rolecount);
	myPlayerData->addGame(queryGame.get());
	myPlayerData->setSelectedGame(queryGame.get());

	CRCore::ref_ptr<crRole> queryGameRole = new crRole;
	queryGameRole->setRoleID(0);
	queryGameRole->setInstanceItemID(0);
	crAbstractMetier *ametier = new crAbstractMetier;
	queryGameRole->setAbstractMetier(ametier);
	ametier->setAbstractMetierID(0);
	queryGameRole->setIName(m_roleName);
	queryGameRole->setSceneID(0);
	queryGameRole->setLayerID(0);
	myPlayerData->addGameRole(queryGameRole.get());
	myPlayerData->setSelectedGameRole(queryGameRole.get());

	ref_ptr<crQuerySceneInfoData> querySceneInfoData = new crQuerySceneInfoData;
	querySceneInfoData->setSceneID(0);
	querySceneInfoData->setSceneName(m_sceneName);
	querySceneInfoData->setFileName(m_sceneFileName);
	querySceneInfoData->setCollideFile(m_collidefile);
	//querySceneInfoData->setBrainFile(m_brainfile);
	myPlayerData->setSceneInfoData(querySceneInfoData.get());
	//创建角色
	ref_ptr<crRole> role = new crRole;
	role->setAbstractItemID(0);
	role->setRoleID(0);
	role->setPlayerID(0);
	//read abstractmetier
	role->setAbstractMetier(ametier);
	ametier->setAbstractMetierID(0);
	ametier->setGameID(0);
	ametier->setMetierName(m_metierName);
	ametier->setInitSceneID(0);
	ametier->setMetierScript(m_metierScript);
	ametier->setTaskTable(m_tasktable);
	if(!(ametier->getMetierScript().empty()))
	{
		data = CREncapsulation::loadData(ametier->getMetierScript());
		if(data.valid())
		{
			data->inputParam(WCHDATA_GameTaskTab,&(ametier->getTaskTable()));
			role->setMetierDataClass(data.get());
			//save数据
			//crStreamBuf *metierstream = role->getMetierStream();
			//if(metierstream)
			//{
			//	data->inputParam(WCHDATA_DataStream,metierstream);
			//}
			data->excHandle(MAKEINT64(WCH_InitData,role.get()));
			role->setMetierStream(NULL);
		}
	}

	role->setItemtype(crInstanceItem::Role);
	role->setInstanceItemID(0);
	//read res
	role->getAbstractItem()->setResID(0);
	role->getAbstractItem()->setFileName(m_resFileName);
	role->getAbstractItem()->setResScript(m_resScript);

	//read abstractitem
	role->getAbstractItem()->setName(m_abstractName);
	role->getAbstractItem()->setScript(m_abstractScript);

	//read instanceitem
	role->setRoomID(0);
	role->setIName(m_roleName);
	role->setSceneID(0);
	role->setLayerID(0);
	role->setZoffset(m_zoffset);
	role->setDir(crVector3(0,-1,0));
	data = CREncapsulation::loadData(role->getAbstractItem()->getScript());
	if(data.valid())
	{
		role->setDataClass(data.get());
		//save数据
		//crStreamBuf *itemstream = role->getStream();
		//if(itemstream)
		//{
		//	data->inputParam(WCHDATA_DataStream,itemstream);
		//}
		data->excHandle(MAKEINT64(WCH_InitData,role.get()));
		role->setStream(NULL);
	}
	myPlayerData->addRole(role.get());

	crSingleGameSceneProxy::getInstance()->initScene();

	crScene *scene = crSingleGameSceneProxy::getInstance()->getScene();
	//crVector3i pos = scene->gainBirthPoint(role->getLayerID(),role->getZoffset() * crGlobalHandle::gData()->gUnitScale(),-1);
	//role->setPosx(pos[0]);
	//role->setPosy(pos[1]);
	//role->setPosz(pos[2]/* + 50*/);
	scene->gainBirthPoint(role.get(),-1);
}
/////////////////////////////////////////
//
//crSingleGameItemInRangeTestMethod
//
/////////////////////////////////////////
crSingleGameItemInRangeTestMethod::crSingleGameItemInRangeTestMethod(){}
crSingleGameItemInRangeTestMethod::crSingleGameItemInRangeTestMethod(const crSingleGameItemInRangeTestMethod& handle):
crMethod(handle)
{
}
void crSingleGameItemInRangeTestMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_player = NULL;
			m_item = NULL;
		}
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crRole *)(LOINT64(param64));
			m_item = (crInstanceItem *)(HIINT64(param64));
		}
		else
		{
			m_player = NULL;
			m_item = NULL;
		}
		break;
	}
}

void crSingleGameItemInRangeTestMethod::addParam(int i, const std::string& str)
{
}

void crSingleGameItemInRangeTestMethod::operator()(crHandle &handle)
{
	if(m_player.valid() && m_item.valid())
	{
		void *param;
		if(m_item->getRoomID() == m_player->getRoomID())
		{
			bool inRange = false;
			float dist = -1;
			if(m_player->getLayerID() == m_item->getLayerID())
			{
				crData *data = m_player->getDataClass();

				data->getParam(WCHDATA_SightRange,param);
				short sightRange = *((short*)param);

				crVector3 rolePos(m_player->getPosx(),m_player->getPosy(),0);
				crVector3 itmePos(m_item->getPosx(),m_item->getPosy(),0);
				dist = (rolePos - itmePos).length();
				if(dist<sightRange)
				{
					inRange = true;
				}
			}
			if(m_item->getItemtype() != crInstanceItem::Role)
			{
				m_item->doEvent(WCH_ItemInRangeTest,MAKEINT64(m_player.get(),&dist));
			}
			bool playerInRange = crMyPlayerData::getInstance()->isItemInRange(m_item.get());
			if(playerInRange!=inRange)
			{
				unsigned char itemtype = m_item->getItemtype();
				int id = m_item->getID();
				if(inRange)
				{//新进入视野范围
					if(itemtype == crInstanceItem::Npc)
					{
						crMyPlayerData::ItemNpcPair itemNpcPair;
						if(!crMyPlayerData::getInstance()->findInRangeNpc(id,itemNpcPair))
						{
							if(crMyPlayerData::getInstance()->findOutRangeNpc(id,itemNpcPair))
							{
								itemNpcPair.first->doEvent(WCH_RecvItemRTData);
								crMyPlayerData::getInstance()->insertInRangeNpc(itemNpcPair);
								crMyPlayerData::getInstance()->removeOutRangeNpc(id);
							}
							else
							{
								crGlobalHandle::getInstance()->doEvent(WCH_LoadItem,MAKEINT64(m_item.get(),NULL));
							}
						}
						else
						{
							itemNpcPair.first->doEvent(WCH_RecvItemRTData);
						}
					}
					else if(itemtype == crInstanceItem::instanceitem)
					{
						crMyPlayerData::ItemNodePair itemNodePair;
						if(!crMyPlayerData::getInstance()->findInRangeItem(id,itemNodePair))
						{
							if(crMyPlayerData::getInstance()->findOutRangeItem(id,itemNodePair))
							{
								itemNodePair.first->doEvent(WCH_RecvItemRTData);
								crMyPlayerData::getInstance()->insertInRangeItem(itemNodePair);
								crMyPlayerData::getInstance()->removeOutRangeItem(id);
							}
							else
							{
								crGlobalHandle::getInstance()->doEvent(WCH_LoadItem,MAKEINT64(m_item.get(),NULL));
							}
						}
						else
						{
							itemNodePair.first->doEvent(WCH_RecvItemRTData);
						}
					}
					CRCore::notify(CRCore::ALWAYS)<<"ItemInRange id = "<<id<<std::endl;
				}
				else
				{//
					crNode *relNode;
					if(itemtype == crInstanceItem::Role)
					{
						int roleid = m_item->getRoleID();
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
			}	
		}
	}
}
/////////////////////////////////////////
//
//crSingleGameRecvItemRTDataMethod
//
/////////////////////////////////////////
crSingleGameRecvItemRTDataMethod::crSingleGameRecvItemRTDataMethod(){}
crSingleGameRecvItemRTDataMethod::crSingleGameRecvItemRTDataMethod(const crSingleGameRecvItemRTDataMethod& handle):
crMethod(handle)
{
}
void crSingleGameRecvItemRTDataMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	}
}

void crSingleGameRecvItemRTDataMethod::addParam(int i, const std::string& str)
{
}

void crSingleGameRecvItemRTDataMethod::operator()(crHandle &handle)
{
	void *param;
	crData *data = m_this->getDataClass();
	if(m_this->getItemtype() > crInstanceItem::instanceitem)
	{
		m_this->setPosz(m_this->getPosz()/* + 40*/);
	}
		
	crNode *node = m_this->getRelNode();
	if(m_this->getItemtype()>crInstanceItem::instanceitem)
	{
		if(node)
		{
			m_this->doEvent(WCH_ItemCoordToNode,MAKEINT64(node,crMatterObject::MD_FullMatrix));
			//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeInRange,crGlobalHandle::gData()->gItemVisiableDelay()));
			node->setVisiable(true);
			node->setEnableIntersect(true);
		}

		data->getParam(WCHDATA_TargetType, param);
		unsigned char targetType = *(unsigned char *)param;
		crVector3 targetPos;
		if(targetType & Target_Instance)
		{
			//int targetid = m_stream->_readInt();
			//data->inputParam(WCHDATA_TargetID,&targetid);
		}
		else
		{
			data->inputParam(WCHDATA_TargetPos,param);
			targetPos = *(crVector3 *)param;
		}
		data->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *(unsigned char *)param;
		if(node)
		{
			if(!(targetType & Target_Instance) && itemstate >= IS_Move && itemstate <= IS_Patrol)
			{
				//float speed = 0;
				//m_this->doEvent(MAKEINT64(WCH_GetSpeed,NULL),MAKEINT64(&speed,NULL));
				//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
				//if(m_this == crMyPlayerData::getInstance()->getRole())
				//	node->doEvent(WCH_NetMoveToTarget,MAKEINT64(&targetPos,&relspeed));
				//else
				//	node->doEvent(WCH_NetMoveToTarget2,MAKEINT64(&targetPos,&relspeed));
				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
			}
			//else
			//{
			//	crVector3 nullpos;
			//	float speed = 0.0f;
			//	if(m_this == crMyPlayerData::getInstance()->getRole())
			//		node->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));
			//	else
			//		node->doEvent(WCH_NetMoveToTarget2,MAKEINT64(&nullpos,&speed));
			//}
			if(m_this->getItemtype() == crInstanceItem::Role)
			{//如果是玩家，还应检查其身上装备的同步问题， 玩家的NetStream变化问题

			}
		}
	}
	else
	{//只在loaditem的时候才会触发接收一次
		if(node)
		{
			m_this->doEvent(WCH_ItemCoordToNode,MAKEINT64(node,crMatterObject::MD_FullMatrix));
			void *param;
			data->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_ItemLoad)
			{
				itemstate = IS_None;
				data->inputParam(WCHDATA_ItemState,&itemstate);
				const std::string &resScript = m_this->getAbstractItem()->getResScript();
				if(!resScript.empty())
				{
					CREncapsulation::loadItemCfg(resScript,dynamic_cast<crGroup *>(node));
					crData *nodeData = node->getDataClass();
					if(nodeData)
					{
						nodeData->inputParam(WCHDATA_Item,m_this);
					}
				}
				else
				{
					CRCore::notify(CRCore::FATAL)<<"crLoadItemMethod(): resScript是空的 "<<std::endl;
				}
			}
			//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeInRange,crGlobalHandle::gData()->gItemVisiableDelay()));
			node->setVisiable(true);
			node->setEnableIntersect(true);
		}
	}
	//CRCore::notify(CRCore::ALWAYS)<<"crRecvItemRTDataMethod pos = "<<pos<<std::endl;
}
/////////////////////////////////////////
//
//crTransportCommandMethod
//
/////////////////////////////////////////
crTransportCommandMethod::crTransportCommandMethod():
m_id(0){}
crTransportCommandMethod::crTransportCommandMethod(const crTransportCommandMethod& handle):
crMethod(handle),
m_command(handle.m_command),
m_id(handle.m_id)
{
}
void crTransportCommandMethod::inputParam(int i, void *param)
{
}
void crTransportCommandMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_command = str;
		break;
	case 1:
		m_id = atoi(str.c_str());
		break;
	}
}
void crTransportCommandMethod::operator()(crHandle &handle)
{
	if(m_id == 0)
	{
		crRole *player = crMyPlayerData::getInstance()->getCurrentRole();
		if(player)
		{
			crNode *node = player->getRelNode();
			if(node)
			{
				node->setVisiable(false);
				node->setEnableIntersect(false);
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,0));
				node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_TransportCommand,&m_command));

				//crNode *lenui = crFilterRenderManager::getInstance()->getFilterNode("LenUI",crSearchNodeBYNameVisitor::OBJECT);
				//if(lenui && !lenui->getVisiable())
				//{
				//	lenui->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFadeInit,MAKERGBA(0,0,0,255)));
				//	short speed = 200;
				//	short timeDelay = 500;
				//	lenui->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFade,MAKEINT32(speed,timeDelay)));
				//}
			}
		}
	}
	else
	{
		crScene *scene = crSingleGameSceneProxy::getInstance()->getScene();
		crInstanceItem *item = scene->findSceneItem(m_id);
		if(item)
		{
			crNode *node = item->getRelNode();
			if(node)
			{
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,0));
				node->setVisiable(false);
				node->setEnableIntersect(false);
				node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_TransportCommand,&m_command));
			}
		}
	}
}
/////////////////////////////////////////
//
//crDoTransportCommandMethod
//
/////////////////////////////////////////
crDoTransportCommandMethod::crDoTransportCommandMethod(){}
crDoTransportCommandMethod::crDoTransportCommandMethod(const crDoTransportCommandMethod& handle):
crMethod(handle)
{
}
void crDoTransportCommandMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_command = *(std::string *)(LOINT64(param64));
		}
		break;
	}
}
void crDoTransportCommandMethod::addParam(int i, const std::string& str)
{
}
void crDoTransportCommandMethod::operator()(crHandle &handle)
{
	crData *data = m_this->getDataClass();
	void *param;
	data->getParam(WCHDATA_Item,param);
	crInstanceItem *item = (crInstanceItem*)param;
	if(item && item->getDataClass())
	{
		//crVector3 nullpos;
		//float speed = 0.0f;
		//m_this->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
		unsigned char itemstate = IS_Stop;
		item->getDataClass()->inputParam(WCHDATA_ItemState,&itemstate);

		std::vector<float> value;
		crArgumentParser::appAtoVec(m_command,value);
		if(value.size()<6)
			return;

		item->setPosxy(value[0],value[1]);
		item->setPosz(value[2]);
		item->setDir(crVector3(value[3],value[4],value[5]));
		item->doEvent(WCH_ItemCoordToNode,MAKEINT64(m_this,crMatterObject::MD_FullMatrix));
		//m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeInRange,crGlobalHandle::gData()->gItemVisiableDelay()));
		m_this->setVisiable(true);
		m_this->setEnableIntersect(true);
	}
	//crRole *player = crMyPlayerData::getInstance()->getRole();
	//if(player)
	//{
	//	crGroup *group = dynamic_cast<crGroup *>(player->getNode());
	//	crNode *node = NULL;
	//	if(group)
	//		node = group->getChild(0);
	//	if(node)
	//	{
	//		crVector3 nullpos;
	//		float speed = 0.0f;
	//		node->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
	//	}
 //       
	//	std::vector<float> value;
	//	crArgumentParser::appAtoVec(m_command,value);
	//	if(value.size()<6)
	//		return;
	//	player->setPosx(value[0]);
	//	player->setPosy(value[1]);
	//	player->setPosz(value[2]);
	//	player->setDirx(value[3]);
	//	player->setDiry(value[4]);
	//	player->setDirz(value[5]);
	//	if(node)
	//	{
	//		player->doEvent(WCH_ItemCoordToNode,MAKEINT64(node,crMatterObject::MD_FullMatrix));
	//		node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeInRange,crGlobalHandle::gData()->gItemVisiableDelay()));
	//	}
	//}
}
////////////////////////////////
//
//crShowTalkChooseMethod
//
///////////////////////////////////
crShowTalkChooseMethod::crShowTalkChooseMethod():
m_visiable(0),
m_buttonid(0),
m_textid(0)
{}

crShowTalkChooseMethod::crShowTalkChooseMethod(const crShowTalkChooseMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_visiable(handle.m_visiable),
m_buttonid(handle.m_buttonid),
m_textid(handle.m_textid)
{
}

void crShowTalkChooseMethod::inputParam(int i, void *param)
{
}

void crShowTalkChooseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_visiable = bool(atoi(str.c_str()));
		break;
	case 2:
		m_buttonid = (unsigned short)(atoi(str.c_str()));
		break;
	case 3:
		m_textid = (unsigned short)(atoi(str.c_str()));
		break;
	}
}

void crShowTalkChooseMethod::operator()(crHandle &handle)
{
	if(!m_filterNode.valid())
	{
		//m_filterNode = crFilterRenderManager::getInstance()->getFilterNode(m_nodeName + crArgumentParser::appItoa(m_buttonid));
	}
	if(m_filterNode.valid())
	{
		m_filterNode->setVisiable(m_visiable);
		crData *data = m_filterNode->getDataClass();
		if(data)
		{
			data->inputParam(WCHDATA_ButtonID,&m_buttonid);
			data->inputParam(WCHDATA_TextID,&m_textid);
		}
	}
}
////////////////////////////////
//
//crShowTalkPreviewMethod
//
///////////////////////////////////
std::string crShowTalkPreviewMethod::m_front = "simsun.ttc";
CRCore::ref_ptr<CRText::crText> crShowTalkPreviewMethod::m_text = NULL;
crShowTalkPreviewMethod::crShowTalkPreviewMethod():
m_visiable(0),
m_characterSize(16.0f),
m_alignmentType(CRText::crText::LEFT_TOP),
m_offset(0.1f),
m_characterSize2(12.0f)
{}
crShowTalkPreviewMethod::crShowTalkPreviewMethod(const crShowTalkPreviewMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_visiable(handle.m_visiable),
m_characterSize(handle.m_characterSize),
m_alignmentType(handle.m_alignmentType),
m_color(handle.m_color),
m_textRect(handle.m_textRect),
m_offset(handle.m_offset),
m_characterSize2(handle.m_characterSize2)
{
	m_text = NULL;
}

void crShowTalkPreviewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	}
}

void crShowTalkPreviewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_visiable = bool(atoi(str.c_str()));
		break;
	case 2:
		m_characterSize = atof(str.c_str());
		break;
	case 3:
		m_characterSize2 = atof(str.c_str());
		break;
	case 4:
		if(str.compare("LEFT_TOP")==0)
			m_alignmentType = CRText::crText::LEFT_TOP;
		else if(str.compare("LEFT_CENTER")==0)
			m_alignmentType = CRText::crText::LEFT_CENTER;
		else if(str.compare("LEFT_BOTTOM")==0)
			m_alignmentType = CRText::crText::LEFT_BOTTOM;
		else if(str.compare("CENTER_TOP")==0)
			m_alignmentType = CRText::crText::CENTER_TOP;
		else if(str.compare("CENTER_CENTER")==0)
			m_alignmentType = CRText::crText::CENTER_CENTER;
		else if(str.compare("CENTER_BOTTOM")==0)
			m_alignmentType = CRText::crText::CENTER_BOTTOM;
		else if(str.compare("RIGHT_TOP")==0)
			m_alignmentType = CRText::crText::RIGHT_TOP;
		else if(str.compare("RIGHT_CENTER")==0)
			m_alignmentType = CRText::crText::RIGHT_CENTER;
		else if(str.compare("RIGHT_BOTTOM")==0)
			m_alignmentType = CRText::crText::RIGHT_BOTTOM;
		else if(str.compare("LEFT_BASE_LINE")==0)
			m_alignmentType = CRText::crText::LEFT_BASE_LINE;
		else if(str.compare("CENTER_BASE_LINE")==0)
			m_alignmentType = CRText::crText::CENTER_BASE_LINE;
		else if(str.compare("RIGHT_BASE_LINE")==0)
			m_alignmentType = CRText::crText::RIGHT_BASE_LINE;
		else if(str.compare("BASE_LINE")==0)
			m_alignmentType = CRText::crText::BASE_LINE;
		break;
	case 5:
		crArgumentParser::appAtoVec(str,m_color);
		m_color /= 255.0f;
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textRect);
		break;
	case 7:
		m_offset = atof(str.c_str());
		break;
	case 8:
		m_front = str;
		break;
	}
}

void crShowTalkPreviewMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			data->getParam(WCHDATA_ButtonID,param);
			unsigned short buttonid = *(unsigned short *)(param);
			crNode *filterNode = NULL;//crFilterRenderManager::getInstance()->getFilterNode(m_nodeName + crArgumentParser::appItoa(buttonid));
			if(filterNode)
			{
				filterNode->setVisiable(m_visiable);
				if(m_visiable)
				{
					data->getParam(WCHDATA_TextID,param);
					unsigned short textid = *(unsigned short*)(param);
					std::string text;
					//if(crMyPlayerData::getInstance()->getText(textid,text))
					{//show text
						if(!m_text.valid())
						{
							unsigned int characterSize = m_characterSize;
							if(CRCore::crDisplaySettings::instance()->getScreenWidth() == 800)
								characterSize = m_characterSize2;
							m_text = new CRText::crText;
							m_text->setName("TalkPreview");
							m_text->setFont(m_front);
							m_text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
							m_text->setCharacterSize(characterSize);
							//m_text->setCharacterSizeMode(CRText::crText::SCREEN_COORDS);
							m_text->setAlignment((CRText::crText::AlignmentType)m_alignmentType);
							m_text->setColor(m_color);
							m_text->setText(CRIOManager::convertUTF8toUTF16(text).c_str());
							//CRIOManager::crLoadManager::getInstance()->requestAddNode()
							crFilterRenderManager::getInstance()->addDrawable(m_text.get(),true);
						}
						else
						{
							m_text->setText(CRIOManager::convertUTF8toUTF16(text).c_str());
						}
						crVector4 rect = m_textRect;
						rect[0] += (float)buttonid * m_offset;
						rect[2] += (float)buttonid * m_offset;
						crVector3 pos(rect[0],rect[1],0.0f);
						pos = pos * crFilterRenderManager::getInstance()->getMVPW();
						pos[2] = 0.0f;
						m_text->setPosition(pos);
						crVector3 pos2(rect[2],rect[3],0.0f);
						pos2 = pos2 * crFilterRenderManager::getInstance()->getMVPW();
						m_text->setMaximumWidth(fabs(pos2[0] - pos[0]));
						m_text->setMaximumHeight(fabs(pos2[1] - pos[1]));
						m_text->setVisiable(true);
					}
				}
				else if(m_text.valid())
				{
					m_text->setVisiable(false);
				}
			}
		}
	}
}
////////////////////////////////
//
//crShowTalkTextMethod
//
///////////////////////////////////
std::string crShowTalkTextMethod::m_front = "simsun.ttc";
CRCore::ref_ptr<CRText::crText> crShowTalkTextMethod::m_text = NULL;
CRCore::ref_ptr<CRText::crText> crShowTalkTextMethod::m_titleText = NULL;
crShowTalkTextMethod::crShowTalkTextMethod():
m_visiable(0),
m_characterSize(16.0f),
m_alignmentType(CRText::crText::LEFT_TOP),
m_textid(0),
m_characterSize2(12.0f)
{
}
crShowTalkTextMethod::crShowTalkTextMethod(const crShowTalkTextMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_visiable(handle.m_visiable),
m_characterSize(handle.m_characterSize),
m_alignmentType(handle.m_alignmentType),
m_color(handle.m_color),
m_textRect(handle.m_textRect),
m_textid(handle.m_textid),
m_titleRect(handle.m_titleRect),
m_titleStr(handle.m_titleStr),
m_characterSize2(handle.m_characterSize2)
{
	m_text = NULL;
    m_titleText = NULL;
}

void crShowTalkTextMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	}
}

void crShowTalkTextMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_visiable = bool(atoi(str.c_str()));
		break;
	case 2:
		m_characterSize = atof(str.c_str());
		break;
	case 3:
		m_characterSize2 = atof(str.c_str());
		break;
	case 4:
		if(str.compare("LEFT_TOP")==0)
			m_alignmentType = CRText::crText::LEFT_TOP;
		else if(str.compare("LEFT_CENTER")==0)
			m_alignmentType = CRText::crText::LEFT_CENTER;
		else if(str.compare("LEFT_BOTTOM")==0)
			m_alignmentType = CRText::crText::LEFT_BOTTOM;
		else if(str.compare("CENTER_TOP")==0)
			m_alignmentType = CRText::crText::CENTER_TOP;
		else if(str.compare("CENTER_CENTER")==0)
			m_alignmentType = CRText::crText::CENTER_CENTER;
		else if(str.compare("CENTER_BOTTOM")==0)
			m_alignmentType = CRText::crText::CENTER_BOTTOM;
		else if(str.compare("RIGHT_TOP")==0)
			m_alignmentType = CRText::crText::RIGHT_TOP;
		else if(str.compare("RIGHT_CENTER")==0)
			m_alignmentType = CRText::crText::RIGHT_CENTER;
		else if(str.compare("RIGHT_BOTTOM")==0)
			m_alignmentType = CRText::crText::RIGHT_BOTTOM;
		else if(str.compare("LEFT_BASE_LINE")==0)
			m_alignmentType = CRText::crText::LEFT_BASE_LINE;
		else if(str.compare("CENTER_BASE_LINE")==0)
			m_alignmentType = CRText::crText::CENTER_BASE_LINE;
		else if(str.compare("RIGHT_BASE_LINE")==0)
			m_alignmentType = CRText::crText::RIGHT_BASE_LINE;
		else if(str.compare("BASE_LINE")==0)
			m_alignmentType = CRText::crText::BASE_LINE;
		break;
	case 5:
		crArgumentParser::appAtoVec(str,m_color);
		m_color /= 255.0f;
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textRect);
		break;
	case 7:
		m_textid = (unsigned short)(atoi(str.c_str()));
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_titleRect);
		break;
	case 9:
		m_titleStr = str;
		break;
	case 10:
		m_front = str;
		break;
	}
}

void crShowTalkTextMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crNode *filterNode = NULL;//crFilterRenderManager::getInstance()->getFilterNode(m_nodeName);
		if(filterNode)
		{
			filterNode->setVisiable(m_visiable);
			if(m_visiable)
			{
				std::string text;
				//if(crMyPlayerData::getInstance()->getText(m_textid,text))
				{//show text
					if(!m_text.valid())
					{
						unsigned int characterSize = m_characterSize;
						if(CRCore::crDisplaySettings::instance()->getScreenWidth() == 800)
							characterSize = m_characterSize2;
						m_text = new CRText::crText;
						m_text->setName("TalkText");
						m_text->setFont(m_front);
						m_text->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
						m_text->setCharacterSize(characterSize);
						//m_text->setCharacterSizeMode(CRText::crText::SCREEN_COORDS);
						m_text->setAlignment((CRText::crText::AlignmentType)m_alignmentType);
						m_text->setColor(m_color);
						m_text->setText(CRIOManager::convertUTF8toUTF16(text).c_str());
						crFilterRenderManager::getInstance()->addDrawable(m_text.get(),true);
					}
					else
					{
						m_text->setText(CRIOManager::convertUTF8toUTF16(text).c_str());
					}
					crVector4 rect = m_textRect;
					crVector3 pos(rect[0],rect[1],0.0f);
					pos = pos * crFilterRenderManager::getInstance()->getMVPW();
					pos[2]=0.0f;
					m_text->setPosition(pos);
					crVector3 pos2(rect[2],rect[3],0.0f);
					pos2 = pos2 * crFilterRenderManager::getInstance()->getMVPW();
					m_text->setMaximumWidth(fabs(pos2[0] - pos[0]));
					m_text->setMaximumHeight(fabs(pos2[1] - pos[1]));
					m_text->setVisiable(true);
				}
				if(!m_titleText.valid())
				{
					unsigned int characterSize = m_characterSize;
					if(CRCore::crDisplaySettings::instance()->getScreenWidth() == 800)
						characterSize = m_characterSize2;
					m_titleText = new CRText::crText;
					m_titleText->setName("TalkTitle");
					m_titleText->setFont(m_front);
					m_titleText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
					m_titleText->setCharacterSize(characterSize);
					//m_titleText->setCharacterSizeMode(CRText::crText::SCREEN_COORDS);
					m_titleText->setAlignment((CRText::crText::AlignmentType)m_alignmentType);
					m_titleText->setColor(m_color);
					m_titleText->setText(CRIOManager::convertUTF8toUTF16(m_titleStr).c_str());
					crFilterRenderManager::getInstance()->addDrawable(m_titleText.get(),true);
				}
				else
				{
					m_titleText->setText(CRIOManager::convertUTF8toUTF16(m_titleStr).c_str());
				}
				crVector4 rect = m_titleRect;
				crVector3 pos(rect[0],rect[1],0.0f);
				pos = pos * crFilterRenderManager::getInstance()->getMVPW();
				pos[2] = 0.0f;
				m_titleText->setPosition(pos);
				crVector3 pos2(rect[2],rect[3],0.0f);
				pos2 = pos2 * crFilterRenderManager::getInstance()->getMVPW();
				m_titleText->setMaximumWidth(fabs(pos2[0] - pos[0]));
				m_titleText->setMaximumHeight(fabs(pos2[1] - pos[1]));
				m_titleText->setVisiable(true);
			}
			else
			{
				if(m_text.valid())
				    m_text->setVisiable(false);
				if(m_titleText.valid())
					m_titleText->setVisiable(false);
			}
		}
	}
}
////////////////////////////////
//
//crPlayerChooseTalkMethod
//
///////////////////////////////////
crPlayerChooseTalkMethod::crPlayerChooseTalkMethod()
{}

crPlayerChooseTalkMethod::crPlayerChooseTalkMethod(const crPlayerChooseTalkMethod& handle):
crMethod(handle)
{
}

void crPlayerChooseTalkMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	}
}

void crPlayerChooseTalkMethod::addParam(int i, const std::string& str)
{
}

void crPlayerChooseTalkMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role && m_this)
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			data->getParam(WCHDATA_TextID,param);
			unsigned short textid = *(unsigned short *)(param);
			role->getRelNode()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(MAKEINT32(WCH_CutTalk,textid),MAKEINT32(0,0)));
		}
	}
}
////////////////////////////////
//
//crCutTalkMethod
//
///////////////////////////////////
crCutTalkMethod::crCutTalkMethod():
m_talkid(0)
{}

crCutTalkMethod::crCutTalkMethod(const crCutTalkMethod& handle):
crMethod(handle),
m_talkid(handle.m_talkid)
{
}

void crCutTalkMethod::inputParam(int i, void *param)
{
}

void crCutTalkMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_talkid = (unsigned short)(atoi(str.c_str()));
		break;
	}
}

void crCutTalkMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role)
	{
		role->getRelNode()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(MAKEINT32(WCH_CutTalk,m_talkid),MAKEINT32(0,0)));
	}
}
////////////////////////////////
//
//crCutSceneMethod
//
///////////////////////////////////
crCutSceneMethod::crCutSceneMethod():
m_id(0)
{}

crCutSceneMethod::crCutSceneMethod(const crCutSceneMethod& handle):
crMethod(handle),
m_id(handle.m_id)
{
}

void crCutSceneMethod::inputParam(int i, void *param)
{
}

void crCutSceneMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_id = (unsigned short)(atoi(str.c_str()));
		break;
	}
}

void crCutSceneMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role)
		role->getRelNode()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(MAKEINT32(WCH_CutScene,m_id),MAKEINT32(0,0)));
}
/////////////////////////////////////////
//
//crSetSpecialActStateMethod
//
/////////////////////////////////////////
crSetSpecialActStateMethod::crSetSpecialActStateMethod():
m_specActState(0),
m_itemtype(crInstanceItem::Role),
m_id(0),
m_roleid(0){}
crSetSpecialActStateMethod::crSetSpecialActStateMethod(const crSetSpecialActStateMethod& handle):
crMethod(handle),
m_specActState(handle.m_specActState),
m_itemtype(handle.m_itemtype),
m_id(handle.m_id),
m_roleid(handle.m_roleid)
{
}
void crSetSpecialActStateMethod::inputParam(int i, void *param)
{
}

void crSetSpecialActStateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_specActState = (unsigned short)(atoi(str.c_str()));
		break;
	case 1:
		m_itemtype = (unsigned char)(str.c_str());
		break;
	case 2:
		m_id = atoi(str.c_str());
		break;
	case 3:
		m_roleid = atoi(str.c_str());
		break;
	}
}

void crSetSpecialActStateMethod::operator()(crHandle &handle)
{
	crData *data;
	if(m_id == 0)
	{
		crRole *player = crMyPlayerData::getInstance()->getCurrentRole();
		data = player->getDataClass();
		data->inputParam(WCHDATA_CurActState,&m_specActState);
	}
	else if(m_itemtype == crInstanceItem::Role)
	{
		crMyPlayerData::RoleNpcPair roleNpcPair;
		if(crMyPlayerData::getInstance()->findInRangePlayer(m_id,m_roleid,roleNpcPair))
		{
			crRole *netplayer = roleNpcPair.first.get();
			data = netplayer->getDataClass();
			data->inputParam(WCHDATA_CurActState,&m_specActState);
		}
	}
	else if(m_itemtype == crInstanceItem::Npc)
	{
		crMyPlayerData::ItemNpcPair itemNpcPair;
		if(crMyPlayerData::getInstance()->findInRangeNpc(m_id,itemNpcPair))
		{
			crInstanceItem *item = itemNpcPair.first.get();
			data = item->getDataClass();
			data->inputParam(WCHDATA_CurActState,&m_specActState);
		}
	}
}
////////////////////////////////
//
//crShowTextMethod
//
///////////////////////////////////
crShowTextMethod::crShowTextMethod():
m_id(0)
{}

crShowTextMethod::crShowTextMethod(const crShowTextMethod& handle):
crMethod(handle),
m_id(handle.m_id)
{
}

void crShowTextMethod::inputParam(int i, void *param)
{
}

void crShowTextMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_id = (unsigned short)(atoi(str.c_str()));
		break;
	}
}

void crShowTextMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role)
		role->getRelNode()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(MAKEINT32(WCH_ShowText,m_id),MAKEINT32(0,-2)));
}
////////////////////////////////
//
//crGlobalValue_ICBCMethod
//
///////////////////////////////////
bool crGlobalValue_ICBCMethod::s_intervene = false;
crGlobalValue_ICBCMethod::crGlobalValue_ICBCMethod():
m_type(0)
{
    s_intervene = false;
}

crGlobalValue_ICBCMethod::crGlobalValue_ICBCMethod(const crGlobalValue_ICBCMethod& handle):
crMethod(handle),
m_type(handle.m_type),
m_value(handle.m_value)
{
	s_intervene = false;
}

void crGlobalValue_ICBCMethod::inputParam(int i, void *param)
{
}

void crGlobalValue_ICBCMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_type = (unsigned short)(atoi(str.c_str()));
		break;
	case 1:
		{
			switch(m_type) 
			{
			case Intervene:
				m_value.m_bln = (bool)(atoi(str.c_str()));
				break;
			}
		}
		break;
	}
}

void crGlobalValue_ICBCMethod::operator()(crHandle &handle)
{
	switch(m_type) 
	{
	case Intervene:
		s_intervene = m_value.m_bln;
		break;
	}
}
bool crGlobalValue_ICBCMethod::GetGlobalValue(unsigned short type, bool &value)
{
	switch(type) 
	{
	case Intervene:
		value = s_intervene;
		return true;
	}
	return false;
}
////////////////////////////////
//
//crIsInterveneMethod
//
///////////////////////////////////
crIsInterveneMethod::crIsInterveneMethod()
{}

crIsInterveneMethod::crIsInterveneMethod(const crIsInterveneMethod& handle):
crMethod(handle)
{
}

void crIsInterveneMethod::inputParam(int i, void *param)
{
}

void crIsInterveneMethod::addParam(int i, const std::string& str)
{
}

void crIsInterveneMethod::operator()(crHandle &handle)
{
	bool intervene = false;
	crGlobalValue_ICBCMethod::GetGlobalValue(crGlobalValue_ICBCMethod::Intervene,intervene);
	handle.outputParam(0,&intervene);
}
/////////////////////////////////////////
//
//crVR_ICBCChangeRoleMethod
//
/////////////////////////////////////////
int crVR_ICBCChangeRoleMethod::s_currentid = 0;
crVR_ICBCChangeRoleMethod::crVR_ICBCChangeRoleMethod():
m_addid(1)
{
	s_currentid = 0;
}
crVR_ICBCChangeRoleMethod::crVR_ICBCChangeRoleMethod(const crVR_ICBCChangeRoleMethod& handle):
crMethod(handle),
m_addid(handle.m_addid),
m_roleTypeElement(handle.m_roleTypeElement),
m_roleImageElement(handle.m_roleImageElement),
m_createElement(handle.m_createElement),
m_defaultImage(handle.m_defaultImage),
m_roleTypeVec(handle.m_roleTypeVec),
m_roleImageVec(handle.m_roleImageVec)
{
	s_currentid = 0;
}
void crVR_ICBCChangeRoleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crVR_ICBCChangeRoleMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_addid = atoi(str.c_str());
		break;
	case 1:
		m_roleTypeElement = str;
		break;
	case 2:
		m_roleImageElement = str;
		break;
	case 3:
		m_createElement = str;
		break;
	case 4:
		m_defaultImage = str;
		break;
	default:
		if(i%2==1)
		{
			m_roleTypeVec.push_back(str);
		}
		else
		{
			m_roleImageVec.push_back(str);
		}
	}
}

void crVR_ICBCChangeRoleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crImageBoxElement *roleTypeElement = dynamic_cast<crImageBoxElement *>(stage->getElement(m_roleTypeElement));
		crImageBoxElement *roleImageElement = dynamic_cast<crImageBoxElement *>(stage->getElement(m_roleImageElement));
		crElement *createElement = stage->getElement(m_createElement);
		if(roleTypeElement && roleImageElement)
		{
			s_currentid += m_addid;
			int maxid = m_roleTypeVec.size() - 1;
			s_currentid = CRCore::clampTo(s_currentid,0,maxid);
			const CRCore::crVector4i &rect = roleTypeElement->getRect();
			roleTypeElement->setElementImage(m_roleTypeVec[s_currentid],crVector2i(rect[2],rect[3]),crVector2i(0,0));

			if(s_currentid<m_roleImageVec.size())
			{
				const CRCore::crVector4i &rect2 = roleImageElement->getRect();
				roleImageElement->setElementImage(m_roleImageVec[s_currentid],crVector2i(rect2[2],rect2[3]),crVector2i(0,0));
				roleImageElement->setShow(true);
                if(createElement)
					createElement->setEnable(true);
			}
			else
			{
				const CRCore::crVector4i &rect2 = roleImageElement->getRect();
				roleImageElement->setElementImage(m_defaultImage,crVector2i(rect2[2],rect2[3]),crVector2i(0,0));
				roleImageElement->setShow(true);
				if(createElement)
					createElement->setEnable(false);
			}
		}
	}
}