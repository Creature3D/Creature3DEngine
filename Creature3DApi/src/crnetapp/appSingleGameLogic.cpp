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
#include <CREncapsulation/crNodeCallbacks.h>
#include <CRNetApp/appSingleGameLogic.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/crArgumentParser.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRCore/thread/crThread.h>
#include "rcfg/ConfigScriptOO.h"
#include <sys/stat.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRPhysics/crBodyCaseMatterObject.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <CRCore/crSequence.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crSoundManager.h>
#include <CRCore/crLightSourceManager.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRIOManager/crCookFile.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRGUI;
using namespace CRNetApp;
using namespace CRPhysics;
using namespace CRProducer;
using namespace CRParticle;
//using namespace CRAL;
using namespace CREncapsulation;
using namespace CRIOManager;
/////////////////////////////////////////
//
//crVR_ICBCMousePickLogic
//
/////////////////////////////////////////
crVR_ICBCMousePickLogic::crVR_ICBCMousePickLogic():
m_targetType(0)
{
}

crVR_ICBCMousePickLogic::crVR_ICBCMousePickLogic(const crVR_ICBCMousePickLogic& handle):
crLogic(handle),
m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crVR_ICBCMousePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if(param)
		{
			m_param = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(m_param));
		}
		else
		{
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	}
}

void crVR_ICBCMousePickLogic::addParam(int i, const std::string& str)
{
}

void crVR_ICBCMousePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crVR_ICBCMousePickLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_TargetType:
		m_targetType = param?*((unsigned char*)param):0;
		break;
	case WCHDATA_TargetNode:
		m_targetNode = param?((crViewMatterObject *)(param)):NULL;
		break;
	case WCHDATA_TargetPos:
		m_targetPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	}
}

void crVR_ICBCMousePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_this->getRelNode());
	if(!bot) return;
	void *param;
	crData *thisData = m_this->getDataClass();

	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	(*m_pickTarget)(*this);
	thisData->inputParam(WCHDATA_TargetType,&m_targetType);
	//thisData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
	thisData->inputParam(WCHDATA_TargetPos,&m_targetPosition);

	crVector3 targetPosition = m_targetPosition;
	unsigned char itemstate = IS_None;
	crInstanceItem *targetItem = NULL;
	int targetid = 0;
	int targetroleid = 0;
	if(m_targetType & Target_Instance || m_targetType & Target_Self || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem)
		{
			targetid = targetItem->getID();
			targetroleid = targetItem->getRoleID();
		}

		if(dynamic_cast<crMatrixTransform *>(m_targetNode.get()))
		{
			targetPosition = (dynamic_cast<crMatrixTransform *>(m_targetNode.get()))->getTrans();
		}
	}
	thisData->inputParam(WCHDATA_TargetID,&targetid);
	thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);

	//m_this->doEvent(WCH_ShowTargetInfo,MAKEINT64(targetItem,NULL));

	crVector3 myPos = bot->getTrans();
	crVector3 targetDir = targetPosition - myPos;
	targetDir[2] = 0.0f;
	do 
	{
		if(m_targetType & Target_Self)
		{//pick myself,停止当前动作，不改变转向
			itemstate = IS_Stop;
			break;
		}
		//
		if(m_targetType == Target_Coord)
		{
			if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
			{
				itemstate = IS_Move;

				//float speed = 0;
				//m_this->doEvent(MAKEINT64(WCH_GetSpeed,NULL),MAKEINT64(&speed,NULL));
				//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
				//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&m_targetPosition,&relspeed));
				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
				break;
			}
		}
		if(!targetItem) break;
		//检查是否有事件
		if(targetItem->hasHandle(WCH_Touch))
		{
			float dist = targetDir.length();
			crData *targetItemData = targetItem->getDataClass();
			targetItemData->getParam(WCHDATA_TouchDistance,param);
			unsigned short touchDistance = *(unsigned short *)param;
			float relDist = (float)touchDistance * crGlobalHandle::gData()->gUnitScale();
			float followDistance = relDist - 0.2f;
			if(dist<=relDist)
			{//在使用范围内
				targetItem->doEvent(WCH_Touch,MAKEINT64(NULL,NULL));
				itemstate = IS_Stop;
			}
			else
			{
				itemstate = IS_MoveToPos;
				//crVector3 dir = targetDir.normalize();
				//crVector3 moveToPos = m_targetPosition - (dir * followDistance);
				//float speed = 0;
				//m_this->doEvent(MAKEINT64(WCH_GetSpeed,NULL),MAKEINT64(&speed,NULL));
				//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
				//bot->doEvent(MAKEINT64(WCH_NetMoveToTarget,NULL),MAKEINT64(&moveToPos,&relspeed));//移动到使用范围
				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
			}
			break;
		}
		itemstate = IS_Stop;
	} while (0);
	if(itemstate == IS_None) return;
	if(itemstate == IS_Stop)
	{
		//float speed = 0.0f;
		//crVector3 nullpos;
		//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
	}
	else if(itemstate != IS_Move && itemstate != IS_MoveToPos && itemstate != IS_MoveToUseSkill)
	{
		//turn to target
		targetDir[2] = 0.0f;
		crVector3 dir = targetDir.normalize();
		m_this->setTargetDir(dir);

		//float speed = 0.0f;
		//crVector3 nullpos;
		//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
		m_this->doEvent(WCH_ItemCoordToNode,MAKEINT64(bot,crMatterObject::MD_RotMatrix));//转向拾取点
	}
	thisData->inputParam(WCHDATA_ItemState,&itemstate);
}
/////////////////////////////////////////
//
//crShowShelfUILogic
//
/////////////////////////////////////////
crShowShelfUILogic::crShowShelfUILogic(){}
crShowShelfUILogic::crShowShelfUILogic(const crShowShelfUILogic& handle):
crLogic(handle),
m_nameid(handle.m_nameid)
{
	if(handle.m_filterSwitch.valid())
		m_filterSwitch = handle.m_filterSwitch->clone();
}
void crShowShelfUILogic::inputParam(int i, void *param)
{
}
void crShowShelfUILogic::addParam(int i, const std::string& str)
{
	std::string name,id;
	std::string::size_type comma = str.find_first_of(',');
	name = std::string(str.begin(),str.begin()+comma);
	id = std::string(str.begin()+comma + 1,str.end());
	m_nameid[name] = atoi(id.c_str());
}
void crShowShelfUILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_filterSwitch = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}
void crShowShelfUILogic::outputParam(int i, void *param)
{
}
void crShowShelfUILogic::operator()(crHandle &handle)
{
	crData *brainData = crBrain::getInstance()->getDataClass();
	void *param;
	brainData->getParam(WCHDATA_CurrentFocusNode,param);
	crNode *currentFocusNode = (crNode *)param;
	if(currentFocusNode)
	{
		std::string name = currentFocusNode->getName();
		std::map<std::string,int>::iterator itr = m_nameid.find(name);
		if(itr != m_nameid.end())
		{
			m_filterSwitch->inputParam(3,&(itr->second));
			(*m_filterSwitch)(*this);
		}
		else
		{
			int index = 0;
			m_filterSwitch->inputParam(3,&index);
			(*m_filterSwitch)(*this);
		}
	}
}