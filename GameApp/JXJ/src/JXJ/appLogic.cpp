/**********************************************************************
*
*	文件:	   appLogic.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#include <JXJ/appLogic.h>
#include <JXJ/appData.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <CRUI/crWidgetExtend.h>
#include <CREncapsulation/crGameDices.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNodeCallbacks.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRProducer/crSceneHandler.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crLoadManager.h>
#include <CRDownload/crDownload.h>
#include <JXJ/appNodeCallbacks.h>
#include <CRUtil/crSetObjectColorVisitor.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace JXJ;
using namespace CRUI;
using namespace CRIOManager;
using namespace CRDownload;
/////////////////////////////////////////
//
//crJXJMousePickLogic
//
/////////////////////////////////////////
crJXJMousePickLogic::crJXJMousePickLogic():
m_targetType(0)
{
}

crJXJMousePickLogic::crJXJMousePickLogic(const crJXJMousePickLogic& handle):
crLogic(handle),
m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crJXJMousePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJMousePickLogic::addParam(int i, const std::string& str)
{
}

void crJXJMousePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJMousePickLogic::outputParam(int i, void *param)
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

void crJXJMousePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	crData *thisData = m_this->getDataClass();
	thisData->getParam(WCHDATA_TargetItem,param);
	crInstanceItem *lastTargetItem = (crInstanceItem*)param;
	if(lastTargetItem)
		lastTargetItem->doEvent(WCH_UnTouch,MAKECREPARAM(m_ea,m_this));

	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	m_pickTarget->inputParam(3,NULL);

	thisData->inputParam(WCHDATA_TargetType,&m_targetType);
	//thisData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
	thisData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
	crInstanceItem *targetItem = NULL;
	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*//* || m_targetType & Target_Self*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem)
		{
			targetItem->doEvent(WCH_Touch,MAKECREPARAM(m_ea,NULL));
		}
	}
	thisData->inputParam(WCHDATA_TargetItem,targetItem);
}
/////////////////////////////////////////
//
//crJXJBattleLMousePickLogic
//
/////////////////////////////////////////
crJXJBattleLMousePickLogic::crJXJBattleLMousePickLogic():
	m_targetType(0)
{
}

crJXJBattleLMousePickLogic::crJXJBattleLMousePickLogic(const crJXJBattleLMousePickLogic& handle):
	crNodeLogic(handle),
	m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
	if(handle.m_useItemAndSendHandle.valid())
		m_useItemAndSendHandle = handle.m_useItemAndSendHandle->clone();
	if(handle.m_playSceneFx_Signal.valid())
		m_playSceneFx_Signal = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx_Signal->clone());;
}

void crJXJBattleLMousePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
			if(m_playSceneFx.valid())
				m_playSceneFx->inputParam(0,NULL);
			if(m_useItemAndSendHandle.valid())
				m_useItemAndSendHandle->inputParam(0,NULL);
			if(m_playSceneFx_Signal.valid())
				m_playSceneFx_Signal->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJBattleLMousePickLogic::addParam(int i, const std::string& str)
{
}

void crJXJBattleLMousePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	case 1:
		m_useItemAndSendHandle = (CRCore::crHandle *)param;
		break;
	case 2:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	case 3:
		m_playSceneFx_Signal = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJBattleLMousePickLogic::outputParam(int i, void *param)
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
		m_hitPosition = m_targetPosition;
		break;
	}
}
void crJXJBattleLMousePickLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
	if(m_playSceneFx_Signal.valid())
		m_playSceneFx_Signal->releaseObjects(state);
}
void crJXJBattleLMousePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	bool isattack = false;
	crData *cameraData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_JXJCameraOperator,param);
	unsigned short cameraOperator = *(unsigned short *)param;
	cameraData->getParam(WCHDATA_TargetItem,param);
	crInstanceItem *lastTargetItem = (crInstanceItem*)param;

	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	if(m_targetType == Target_None)
	{
		root = crSceneManager::getInstance()->getStaticRoot();
		m_pickTarget->inputParam(3,root);
		(*m_pickTarget)(*this);
	}
	m_pickTarget->inputParam(3,NULL);

	//crVector3 pickmsg(m_ea->getXnormalized(), m_ea->getYnormalized(), m_targetType);
	//MessageBoxA(NULL, crArgumentParser::appVectoa(pickmsg).c_str(), "PickTarget", MB_OK);

	m_targetType &= ~Target_Self;

	if(m_targetType & Target_Coord)
	{
		CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
		if(!scene->findWalkablePos(crMyPlayerData::getInstance()->getCurrentRole(),crVector2(m_targetPosition[0],m_targetPosition[1]),c_walkableSearchRange,m_targetPosition))
			return;
	}

	crVector3 camPos = m_this->getTrans();
	crVector3 camdir = camPos - m_targetPosition;
	camdir[2] = 0.0f;
	camdir.normalize();
	if ((m_targetType & Target_Coord) && ((cameraOperator == Cam_MoveToPos) || (cameraOperator == Cam_Patrol) || (cameraOperator == Cam_AttackToPos)))
	{
		m_playSceneFx->inputParam(3,&m_targetPosition);
		m_playSceneFx->inputParam(4,&camdir);
		(*m_playSceneFx)(*this);
	}
	unsigned int guisestate = GS_Normal;
	cameraData->getParam(WCHDATA_SelectMyRoleVec, param);
	SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
//////////////////////////////////////////////////////////////////////////信号
	if (cameraOperator == Cam_Signal)
	{
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
		crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas("UI_CopyBattle_MinMap");
		crImageBoxWidgetNode *mapWidget = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget("UI_CopyBattle_Minimap"));
		crWidgetNode *rotMapWidget = canvas->getWidget("RotMap");
		crVector2 mapWidgetSize;
		crVector3 pos;
		const crBoundingBox &bbox = mapWidget->getBoundBox();
		mapWidgetSize.set(bbox.xLength(),bbox.yLength());
		crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);

		CRCore::ref_ptr<CRCore::crWidgetNode> icon ;
		cameraData->getParam(WCHDATA_JXJSignalsMap,param);
		if(param) 
		{
			SignalNodeMap * signaldeque = (SignalNodeMap *)param;
			crCanvasNode *mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();	
			ref_ptr<crWidgetNode>iconWidget;
	
			crWidgetNode *signalWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget("UI_CopyBattle_remind"));
			
			SignalNodeMap::iterator itr_w = signaldeque->begin();
			while (itr_w != signaldeque->end())
			{
				if (itr_w->first->getVisiable() == false && \
					itr_w->first->getName().compare("UI_CopyBattle_remind") == 0)
				{
					iconWidget = itr_w->first;
					itr_w->second = 10.0f;
					break;
				}
				itr_w++;
			}

			if (itr_w == signaldeque->end() && signaldeque->size() <= 20)
			{
				iconWidget = dynamic_cast<crWidgetNode *>(signalWidget->clone(crCopyOp::DEEP_COPY_NODES));
				signaldeque->insert(std::make_pair(iconWidget, 10.0f));
				crLoadManager::getInstance()->requestAddNode(canvas, iconWidget.get(), false);
			}

			if (iconWidget.valid())
			{
				iconWidget->setVisiable(true);
				float px = m_targetPosition.x() * mapFactor[0];
				float py = m_targetPosition.y() * mapFactor[1];
				crVector3 dir = mapWidget->getBound().center();
				pos = dir+crVector3(px,py,0.0f);
				pos = pos*rotMapWidget->getMatrix();
				iconWidget->setMatrix(scaleMat*invScaleMat*crMatrix::translate(pos));
			}
		}

		m_this->doEvent(WCH_JXJPlaySignalFx,MAKECREPARAM(&m_targetPosition,NULL));
// 		cameraData->getParam(WCHDATA_JXJSignalTarget,param);
// 		m_targetNode = (crNode *)param;
// 		if(!m_targetNode.valid())
// 		{
// 			m_playSceneFx_Signal->inputParam(3,&m_targetPosition);
// 			crVector3 camPos = m_this->getTrans();
// 			crVector3 dir = camPos - m_targetPosition;
// 			dir[2] = 0.0f;
// 			dir.normalize();
// 			m_playSceneFx_Signal->inputParam(4,&dir);
// 			(*m_playSceneFx_Signal)(*this);
// 
// 			cameraData->inputParam(WCHDATA_JXJSignalTarget,m_targetNode.get());
// 		}
// 		else
// 		{
// 			crMatrixTransform * signalMatrix = dynamic_cast<crMatrixTransform *>(m_targetNode.get());
// 			if(signalMatrix)
// 			{
// 				signalMatrix->setVisiable(true);
// 				signalMatrix->setMatrix(crMatrix::translate(m_targetPosition));
// 			}
// 		}
		
		//cameraData->inputParam(WCHDATA_JXJSignalsMap,&signaldeque);
		//cameraData->inputParam(WCHDATA_JXJCameraOperator,NULL);

		//////////////////////////////////////////////////////////////////////////send packet
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		crNetManager *netManager = netConductor->getNetManager();
		ref_ptr<crStreamBuf> packetStream = new crStreamBuf;
		packetStream->createBuf(12);
		//packetStream->_writeBool(false);
		packetStream->_writeVec3(m_targetPosition);
		crPlayerDataSceneEventPacket packet;
		crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvSignalsPos,packetStream.get());
		netManager->sendPacket("all",packet);
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////巡逻
	else if (cameraOperator == Cam_Patrol)
	{
		//crVector2 mypos;
		PatrolPointVec* patrolPositionVec;
		float scale = crGlobalHandle::gData()->gUnitScale();
		for (SelectMyRoleVec::iterator itr = selectMyRoles->begin();
			itr != selectMyRoles->end();
			++itr)
		{	
			crRole *role = itr->get();
			guisestate = GS_Normal;
			role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
			if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate/* || guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
				continue;
			crData *data = role->getDataClass();
			//data->getParam(WCHDATA_CurActState, param);
			//unsigned short* act = (unsigned short *)param;
			//if (*act == ACT_Skill)
			//{//中断施法
			//	//*act = ACT_Idle;
			//	continue;
			//}
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_GuiseState,param);
			unsigned int *gs = (unsigned int *)param;
			*gs &= ~GS_Garrison;
			data->getParam(WCHDATA_PatrolPointVec,param);
			patrolPositionVec = (PatrolPointVec*)param;
			patrolPositionVec->clear();
			float scale = crGlobalHandle::gData()->gUnitScale();
			//pos.set(m_targetPosition.x(),m_targetPosition.y());
			patrolPositionVec->push_back(crVector2(m_targetPosition.x(), m_targetPosition.y()));
			patrolPositionVec->push_back(crVector2(role->getPosx()*scale,role->getPosy()*scale));
			//unsigned char itemstate = IS_Move;
// 			data->inputParam(WCHDATA_ItemState,&itemstate);
			data->getParam(WCHDATA_PatrolIndex,param);
 			char* idx = (char*)param;
 			*idx = 0;
			unsigned char itemstate = IS_AttackToPos;
			unsigned char targettype = Target_Coord;
			data->inputParam(WCHDATA_ItemState,&itemstate);
			data->inputParam(WCHDATA_TargetType,&targettype);
			data->inputParam(WCHDATA_TargetPos,&m_targetPosition);
			data->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
			crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(itr->get());
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			//itr++;
			m_targetPosition += camdir*c_targetOffset;
		}
		//cameraData->inputParam(WCHDATA_JXJCameraOperator,NULL);
	}
	//////////////////////////////////////////////////////////////////////////
	bool touched = false;
	crInstanceItem *targetItem = NULL;
	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
	}
	if(lastTargetItem && lastTargetItem != targetItem)
		lastTargetItem->doEvent(WCH_UnTouch,MAKECREPARAM(m_ea,m_this));
	do 
	{
		cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
		AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
		crRole *role = useItemPair->first.get();
		if(role || !selectMyRoles->empty())
		{
			//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(!role)
				role = (*selectMyRoles)[0].get();
			crData *roleData = role->getDataClass();
			char isEnemy = 0;
			if(targetItem)
				role->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem,&isEnemy));
			crVector3 myPos = role->getPosition();
			crVector3 targetDir = m_targetPosition - myPos;
			targetDir[2] = 0.0f;
			roleData->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			//roleData->getParam(WCHDATA_AboutToUseItemID,param);
			int itemid = useItemPair->second;//*((int*)param);
			if(itemstate != IS_Dead && itemid != 0 && rthp>0.0f)
			{//放技能
				 //roleData->getParam(WCHDATA_CurActState, param);
				 //unsigned short* act = (unsigned short *)param;
				 //if (*act == ACT_Skill)
				 //{//已经在施法状态，不能中断之前的施法
				 //	//*act = ACT_Idle;
				 //	break;
				 //}
				 guisestate = GS_Normal;
				 role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
				 if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate/* || guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
					 break;
				crItemChild *itemChild = role->findChildItem(itemid);
				if(!itemChild || !itemChild->isLoaded())
				{
					break;
				}
				crInstanceItem *item = itemChild->getInstanceItem();
				crData *itemData = item->getDataClass();
				itemData->getParam(WCHDATA_TargetType,param);
				unsigned char targetType = *(unsigned char *)param;
				if (!(targetType & Target_Coord || targetType & m_targetType))
				{
					targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(m_hitPosition, 4.0f, targetType, m_targetType);
					if (!targetItem)
						break;
				}
				if(targetType & Target_Coord || targetType & m_targetType)
				{//对象符合
					if(targetType != Target_Bot && targetType != Target_Instance && targetType != Target_All && targetType != Target_StaticNpc/* && targetType != Target_StaticItem*/)
					{
						if(isEnemy == -1)
						{
							if(targetType & Target_Friend || targetType & Target_Self)
							{//不能敌方使用
								targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(m_hitPosition, 4.0f, targetType, m_targetType);
								if (!targetItem)
									break;
							}
						}
						else if(isEnemy == 1)
						{
							if(targetType & Target_Enemy)
							{//不能对友方使用
								targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(m_hitPosition, 4.0f, targetType, m_targetType);
								if (!targetItem)
									break;
							}
							if(targetType == Target_Self)
							{//只能对自己使用
								if( targetItem->getItemtype()!=crInstanceItem::Role || 
									targetItem->getID() != role->getID())
								{
									targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(m_hitPosition, 4.0f, targetType, m_targetType);
									if (!targetItem)
										break;
								}
							}
						}
					}
					int targetid = 0;
					int  targetroleid = 0;
					unsigned char targettype = Target_Coord;
					//unsigned char itemstate = IS_MoveToUseSkill;
					if(targetType!=Target_Coord)
					{
						if((m_targetType & Target_Instance || m_targetType & Target_StaticNpc /*|| m_targetType & Target_StaticItem*/) && targetItem)
						{
							targetid = targetItem->getID();
							targetroleid = targetItem->getRoleID();
							//m_targetPosition = targetItem->getPosition();
							if(targetItem == role)
								m_targetType |= Target_Self;
							targettype = targetItem->getItemtype()==crInstanceItem::Role?Target_Role:Target_Npc;
							//itemstate = IS_CloseTargetUseSkill;
						}
					}
					else
					{
						targetItem=NULL;
					}
					roleData->inputParam(WCHDATA_TargetType,&targettype);
					//roleData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
					roleData->inputParam(WCHDATA_TargetID,&targetid);
					roleData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
					roleData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
					roleData->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
					roleData->inputParam(WCHDATA_TargetItem,targetItem);
					//itemData->getParam(WCHDATA_ItemUseRange,param);
					//unsigned short itemUseRange = *((unsigned short*)param);
					//float fitemUseRange = (float)itemUseRange * crGlobalHandle::gData()->gUnitScale();
					//float targetDist = (m_targetPosition - myPos).length();
					//if(targetDist <= fitemUseRange)
					itemstate = IS_UseItem;
					roleData->inputParam(WCHDATA_ItemState,&itemstate);
					roleData->inputParam(WCHDATA_AboutToUseItemID,&itemid);
					//float followDistance = fitemUseRange - crGlobalHandle::gData()->gFollowDelta();
					//roleData->inputParam(WCHDATA_FollowDistance,&followDistance);
					roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
					roleData->getParam(WCHDATA_GuiseState,param);
					unsigned int *gs = (unsigned int *)param;
					*gs &= ~GS_Garrison;

					roleData->getParam(WCHDATA_PatrolPointVec,param);
					PatrolPointVec *patrolPositionVec = (PatrolPointVec*)param;
					patrolPositionVec->clear();
					roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
					//if(itemstate == IS_MoveToUseSkill)
					//{
					//	crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(role);
					//}
					//else
					//{
					//	role->getOrCreatePathFinder()->dirty();
					//}
					//bool isAboutToUseItem = true;
					//m_useItemAndSendHandle->inputParam(1,role);
					//m_useItemAndSendHandle->inputParam(2,&itemid);
					//m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
					//(*m_useItemAndSendHandle)(*this);
					//m_useItemAndSendHandle->inputParam(1,NULL);
					cameraData->inputParam(WCHDATA_JXJRoleAboutToUseItemID,NULL);
					//send to net
					//if(netConductor)
					//{
					//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
					//	stream->createBuf(26);
					//	stream->_writeUChar(targettype);
					//	if(targettype & Target_Instance)
					//		stream->_writeInt(targetid);
					//	if(targettype & Target_Role)
					//		stream->_writeInt(targetroleid);
					//	stream->_writeUChar(itemstate);
					//	if(itemstate == IS_MoveToUseSkill)
					//	{
					//		stream->_writeVec3(m_targetPosition);
					//		crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(role);
					//	}
					//	//stream->_writeFloat(followDistance);
					//	crPlayerEventPacket packet;
					//	crPlayerEventPacket::buildRequestPacket(packet,role->getRoleID(),WCH_NetControl2,stream.get());
					//	netConductor->getNetManager()->sendPacket("all",packet);
					//}
					////向服务器发送消息
				}
				else
				{//UI提示使用对象不符
				}
				//使用技能状态，不执行下面的操作
				break;
			}
			else if ((cameraOperator == Cam_AttackToPos) || (cameraOperator == Cam_MoveToPos))
			{
				crRole *myRole;
				isattack = true;
				int targetid = 0;
				int  targetroleid = 0;
				unsigned char targettype = Target_Coord;
				unsigned char itemstate = cameraOperator == Cam_AttackToPos ? IS_AttackToPos : IS_MoveToPos;
				unsigned char olditemstate;
				//ref_ptr<crStreamBuf> stream;
				crData *myRoleData;
				//crPlayerEventPacket packet;
				for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
					itr != selectMyRoles->end();
					++itr )
				{
					myRole = itr->get();
					myRoleData = myRole->getDataClass();
					myRoleData->getParam(WCHDATA_ItemState,param);
					olditemstate = *(unsigned char *)param;
					if(olditemstate == IS_Dead)
						continue;
					//myRoleData->getParam(WCHDATA_CurActState, param);
					//unsigned short* act = (unsigned short *)param;
					//if (*act == ACT_Skill)
					//{//中断施法
					//	//*act = ACT_Idle;
					//	continue;
					//}
					guisestate = GS_Normal;
					myRole->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
					if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate/* || guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
						continue;
					//if(isEnemy == -1)
					//{
					//}
					//else if(isEnemy == 1)
					//{//攻击自己人
					//}
					if (itemstate == IS_AttackToPos)
					{
						if (!targetItem)
						{
							targetItem = crMyPlayerData::getInstance()->getOneInRangeEnemy(myRole);
						}
						if (targetItem)
						{
							targetid = targetItem->getID();
							targetroleid = targetItem->getRoleID();
							targettype = targetItem->getItemtype() == crInstanceItem::Role ? Target_Role : Target_Npc;
							m_targetType = targettype;
						}
					}

					myRoleData->inputParam(WCHDATA_ItemState,&itemstate);
					//myRoleData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
					myRoleData->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
					myRoleData->inputParam(WCHDATA_TargetPos,&m_targetPosition);

					myRoleData->inputParam(WCHDATA_TargetType,&targettype);
					myRoleData->inputParam(WCHDATA_TargetID,&targetid);
					myRoleData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
					myRoleData->inputParam(WCHDATA_TargetItem,targetItem);
					myRoleData->inputParam(WCHDATA_AboutToUseItemID,NULL);
					//////////////////////////////////////////////////////////////////////////
					myRoleData->excHandle(MAKECREPARAM(WCH_LockData,1));
					myRoleData->getParam(WCHDATA_GuiseState,param);
					unsigned int *gs = (unsigned int *)param;
					*gs &= ~GS_Garrison;

					myRoleData->getParam(WCHDATA_PatrolPointVec,param);
					PatrolPointVec *patrolPositionVec = (PatrolPointVec*)param;
					patrolPositionVec->clear();
					myRoleData->excHandle(MAKECREPARAM(WCH_LockData,0));
					if(itemstate == IS_AttackToPos)
					{
						//if(myRole->getAbstractItemID() == 20019)
						//{//投石车直接攻击
						//	myRoleData->excHandle(MAKECREPARAM(WCH_LockData,1));
						//	myRoleData->getParam(WCHDATA_AttackMap,param);
						//	AttackMap *attackMap = (AttackMap *)param;
						//	if(attackMap)
						//	{
						//		int attackid = attackMap->begin()->second;
						//		myRoleData->inputParam(WCHDATA_CurrentAttackID,&attackid);
						//	}
						//	myRoleData->excHandle(MAKECREPARAM(WCH_LockData,0));
						//}
						//else
						//{
						if (!targetItem)
							crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(myRole);
						else
							myRole->getOrCreatePathFinder()->dirty();
						//}
					}
					else
					{
						crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(myRole);
					}
					//if(m_targetType & Target_Coord)
					//{
						m_targetPosition += camdir*c_targetOffset;
					//}
					//}
					//else
					//{
					//	role->getOrCreatePathFinder()->dirty();
					//}
					////send to net
					//if(netConductor)
					//{
					//	stream = new crStreamBuf;
					//	stream->createBuf(22);
					//	stream->_writeUChar(targettype);
					//	if(targettype & Target_Instance)
					//		stream->_writeInt(targetid);
					//	if(targettype & Target_Role)
					//		stream->_writeInt(targetroleid);
					//	stream->_writeUChar(itemstate);
					//	if(itemstate == IS_AttackToPos)
					//	{
					//		stream->_writeVec3(m_targetPosition);
					//		crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(myRole);
					//	}
					//	crPlayerEventPacket::buildRequestPacket(packet,myRole->getRoleID(),WCH_NetControl2,stream.get());
					//	netConductor->getNetManager()->sendPacket("all",packet);
					//}
				}
				break;
			}
		}
		if(targetItem)
		{//执行点选
			//unsigned char targetitemtype = targetItem->getItemtype();
			if(crMyPlayerData::getInstance()->ifItemIsMe(targetItem))
			{
				int key = m_ea->getModKeyMask();
				if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_SHIFT)&&!isattack)
				{//shift多选
					//清除之前选中的自己人
					crNode *fxNode;
					crDecalUpdateCallback *callback;
					//crSelectNodeVisitor selectNode(false);
					for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
						itr != selectMyRoles->end();
						++itr )
					{
						//bot = (*itr)->getRelNode();
						//bot->accept(selectNode);
						fxNode = (*itr)->findSceneFxNode("$SelectDecal");
						if(fxNode)
						{
							callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
							if(callback)
							{
								callback->setVisiable(false);
							}
						}
					}
					selectMyRoles->resize(0);
				}
			}
			targetItem->doEvent(WCH_Touch,MAKECREPARAM(m_ea,NULL));
			touched = true;
		}
	} while (0);
	cameraData->inputParam(WCHDATA_TargetType,&m_targetType);
	//cameraData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
	cameraData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
	cameraData->inputParam(WCHDATA_TargetItem,targetItem);
	if (!touched)
	{
		if (!selectMyRoles->empty() && selectMyRoles->begin()->valid())
		{
			selectMyRoles->begin()->get()->doEvent(WCH_JXJTouchRoleShow);

			crNode *fxNode;
			crDecalUpdateCallback *callback;
			cameraData->getParam(WCHDATA_SelectItem, param);
			crInstanceItem *selectItem = (crInstanceItem*)param;
			if (selectItem)
			{
				fxNode = selectItem->findSceneFxNode("$SelectDecal");
				if (fxNode)
				{
					callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if (callback)
					{
						callback->setVisiable(false);
					}
				}
			}
			cameraData->inputParam(WCHDATA_SelectItem, NULL);

		}
	}
}
/////////////////////////////////////////
//
//crJXJBattleRMousePickLogic
//
/////////////////////////////////////////
crJXJBattleRMousePickLogic::crJXJBattleRMousePickLogic():
	m_targetType(0)
{
}

crJXJBattleRMousePickLogic::crJXJBattleRMousePickLogic(const crJXJBattleRMousePickLogic& handle):
	crNodeLogic(handle),
	m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJBattleRMousePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
			if(m_playSceneFx.valid())
				m_playSceneFx->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJBattleRMousePickLogic::addParam(int i, const std::string& str)
{
}

void crJXJBattleRMousePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	case 1:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJBattleRMousePickLogic::outputParam(int i, void *param)
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
void crJXJBattleRMousePickLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJBattleRMousePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	crData *cameraData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	if(m_targetType == Target_None)
	{
		root = crSceneManager::getInstance()->getStaticRoot();
		m_pickTarget->inputParam(3,root);
		(*m_pickTarget)(*this);
	}
	m_pickTarget->inputParam(3,NULL);
	if (m_targetType & Target_Self)
	{
		m_targetNode = NULL;
		m_targetType = Target_Coord;
	}
	//m_targetType &= ~Target_Self;
	
	crData *targetData;
	crData *itemData;
	unsigned char itemstate,targetitemstate;
	crInstanceItem *targetItem;
	int targetid = 0;
	int targetroleid = 0;
	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		targetData = m_targetNode->getDataClass();
		targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem)
		{
			itemData = targetItem->getDataClass();
			itemData->getParam(WCHDATA_ItemState,param);
			if(param)
			{
				targetitemstate = *(unsigned char *)param;
				if(targetitemstate == IS_Dead)
				{
					m_targetType = Target_Coord;
				}
			}
			else
			{//targetitem有可能拾取到skillitem,及场景item等
				m_targetType = Target_Coord;
			}
			if(m_targetType != Target_Coord)
			{
				targetid = targetItem->getID();
				targetroleid = targetItem->getRoleID();
				//m_targetPosition = targetItem->getPosition();
			}
		}
	}
	if(m_targetType & Target_Coord)
	{
		CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
		if(!scene->findWalkablePos(crMyPlayerData::getInstance()->getCurrentRole(),crVector2(m_targetPosition[0],m_targetPosition[1]),c_walkableSearchRange,m_targetPosition))
			return;
	}
	crVector3 camPos = m_this->getTrans();
	crVector3 camdir = camPos - m_targetPosition;
	camdir[2] = 0.0f;
	camdir.normalize();
	if(m_targetType & Target_Coord)
	{
		m_playSceneFx->inputParam(3,&m_targetPosition);
		m_playSceneFx->inputParam(4,&camdir);
		(*m_playSceneFx)(*this);
	}

	cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
	SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
	crRole *myRole;
	crData *roleData;
	//crInstanceItem *item;
	//crItemChild *itemChild;
	crVector3 rolePos,targetDir;
	//ref_ptr<crStreamBuf> stream = new crStreamBuf;
	//stream->createBuf(64);
	//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	unsigned int *gs;
	crVector3 lastTargetPos;
	int mytargetid = 0;
	int mytargetroleid = 0;
	unsigned int guisestate = GS_Normal;
	//int i = 0;
	for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
		 itr != selectMyRoles->end();
		 ++itr/*,i++*/ )
	{
		myRole = itr->get();
		guisestate = GS_Normal;
		myRole->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
		if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate/* || guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
			continue;
		roleData = myRole->getDataClass();
		roleData->getParam(WCHDATA_ItemState,param);
		itemstate = *(unsigned char *)param;
		if(itemstate != IS_Dead)
		{
			//roleData->getParam(WCHDATA_CurActState, param);
			//unsigned short* act = (unsigned short *)param;
			//if (*act == ACT_Skill)
			//{//中断施法
			//	//*act = ACT_Idle;
			//	continue;
			//}
			roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			roleData->getParam(WCHDATA_GuiseState,param);
			gs = (unsigned int *)param;
			*gs &= ~GS_Garrison;

			roleData->excHandle(MAKECREPARAM(WCH_LockData,0));

			//itemstate = IS_None;
			//targetItem = NULL;
			mytargetid = targetid;
			mytargetroleid = targetroleid;
			//if(targetItem == myRole)
			//{
			//	//m_targetType |= Target_Self;
			//	m_targetType = Target_Coord;
			//	mytargetid = 0;
			//	mytargetroleid = 0;
			//	targetItem = NULL;
			//}
			//if(m_targetType & Target_Instance/* || m_targetType & Target_Self*/)
			//{
			//	targetData = m_targetNode->getDataClass();
			//	targetData->getParam(WCHDATA_Item,param);
			//	targetItem = ((crInstanceItem*)param);
			//	if(targetItem)
			//	{
			//		itemData = targetItem->getDataClass();
			//		itemData->getParam(WCHDATA_ItemState,param);
			//		if(param)
			//		{//targetitem有可能拾取到skillitem,及场景item等
			//			targetitemstate = *(unsigned char *)param;
			//			if(targetitemstate == IS_Dead)
			//			{
			//				m_targetType = Target_Coord;
			//			}
			//		}
			//		if(m_targetType != Target_Coord)
			//		{
			//			targetid = targetItem->getID();
			//			targetroleid = targetItem->getRoleID();
			//			//m_targetPosition = targetItem->getPosition();
			//		}
			//	}
			//	//if(dynamic_cast<crMatrixTransform *>(m_targetNode.get()))
			//	//{
			//	//	targetPosition = (dynamic_cast<crMatrixTransform *>(m_targetNode.get()))->getTrans();
			//	//}
			//}
			roleData->inputParam(WCHDATA_TargetType,&m_targetType);
			//roleData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
			roleData->inputParam(WCHDATA_TargetID,&mytargetid);
			roleData->getParam(WCHDATA_TargetPos,param);
			lastTargetPos = *(crVector3 *)param;
			roleData->inputParam(WCHDATA_TargetRoleID,&mytargetroleid);
			roleData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
			roleData->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
			roleData->inputParam(WCHDATA_TargetItem,targetItem);
			roleData->inputParam(WCHDATA_AboutToUseItemID, NULL);
			//m_this->doEvent(WCH_ShowTargetInfo,MAKECREPARAM(targetItem,NULL));

			//rolePos = myRole->getPosition();
			//targetDir = m_targetPosition - rolePos;
			//targetDir[2] = 0.0f;
			//targetDir.normalize();
			//stream->seekBegin();
			//stream->_writeUChar(m_targetType);
			//if(m_targetType & Target_Instance || m_targetType & Target_Self)
			//{
			//	stream->_writeInt(targetid);//4
			//	if(m_targetType & Target_Role)
			//		stream->_writeInt(targetroleid);
			//}
			//else
			//	stream->_writeVec3(m_targetPosition);

			//if(targetItem == myRole)
			//{//stop move
			//	itemstate = IS_Stop;
			//	roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			//	roleData->getParam(WCHDATA_PatrolPointVec,param);
			//	PatrolPointVec *patrolPositionVec = (PatrolPointVec*)param;
			//	patrolPositionVec->clear();
			//	roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
			//	//stream->_writeUChar(itemstate);
			//}
			//else 
			//if(m_targetType & Target_Instance)
			//{//跟随
			//	itemstate = IS_CloseTarget;

			//	roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			//	roleData->getParam(WCHDATA_PatrolPointVec,param);
			//	PatrolPointVec *patrolPositionVec = (PatrolPointVec*)param;
			//	patrolPositionVec->clear();
			//	roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
			//	float followDistance = crGlobalHandle::gData()->gMinFollowDistance();
			//	roleData->inputParam(WCHDATA_FollowDistance,&followDistance);

			//	//stream->_writeUChar(itemstate);
			//	//stream->_writeFloat(followDistance);
			//	////crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(myRole);
			//	myRole->getOrCreatePathFinder()->dirty();
			//}
			//else if(m_targetType & Target_Coord)
			roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			roleData->getParam(WCHDATA_PatrolPointVec,param);
			PatrolPointVec *patrolPositionVec = (PatrolPointVec*)param;
			patrolPositionVec->clear();
			roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
			if (!(m_targetType&Target_Enemy) && (itemstate != IS_MoveToPos || (crVector2(lastTargetPos[0], lastTargetPos[1]) - crVector2(m_targetPosition[0], m_targetPosition[1])).length()>0.5f))
			{
				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(myRole);
			}
			itemstate = IS_MoveToPos;
			roleData->inputParam(WCHDATA_ItemState, &itemstate);
			//else
			//{
			//	stream->_writeUChar(itemstate);
			//}
			//if(netConductor)
			//{
			//	crPlayerEventPacket packet;
			//	crPlayerEventPacket::buildRequestPacket(packet,myRole->getRoleID(),WCH_JXJRecvBattleRMousePick,stream.get());
			//	netConductor->getNetManager()->sendPacket("all",packet);
			//	//CRCore::notify(CRCore::ALWAYS)<<"crNetMousePickLogic Send WCH_NetMousePick"<<std::endl;
			//}
		}
		//if(m_targetType & Target_Coord)
		//{
			m_targetPosition += camdir * c_targetOffset;
		//}
	}
	if (!selectMyRoles->empty() && selectMyRoles->begin()->valid())
	{
		selectMyRoles->begin()->get()->doEvent(WCH_JXJTouchRoleShow);

		crNode *fxNode;
		crDecalUpdateCallback *callback;
		cameraData->getParam(WCHDATA_SelectItem, param);
		crInstanceItem *selectItem = (crInstanceItem*)param;
		if (selectItem)
		{
			fxNode = selectItem->findSceneFxNode("$SelectDecal");
			if (fxNode)
			{
				callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
				if (callback)
				{
					callback->setVisiable(false);
				}
			}
		}
		cameraData->inputParam(WCHDATA_SelectItem, NULL);

	}
}
/////////////////////////////////////////
//
//crJXJUseSkillDecalUpdateLogic
//
/////////////////////////////////////////
crJXJUseSkillDecalUpdateLogic::crJXJUseSkillDecalUpdateLogic():
	m_decalNode(NULL)/*,
					 m_lastupdtime(0.0f),
					 m_delta(0.0f)*/
{
}

crJXJUseSkillDecalUpdateLogic::crJXJUseSkillDecalUpdateLogic(const crJXJUseSkillDecalUpdateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL)/*,
					 m_delta(handle.m_delta),
					 m_lastupdtime(0.0f)*/
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJUseSkillDecalUpdateLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			if(m_playSceneFx.valid())
				m_playSceneFx->inputParam(0,NULL);
		}
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (crGUIEventAdapter *)(LOCREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crJXJUseSkillDecalUpdateLogic::addParam(int i, const std::string& str)
{
	//switch (i)
	//{
	//case 0:
	//	m_delta = atof(str.c_str());
	//	break;
	//}
}

void crJXJUseSkillDecalUpdateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJUseSkillDecalUpdateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}
void crJXJUseSkillDecalUpdateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJUseSkillDecalUpdateLogic::operator()(crHandle &handle)
{
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!m_playSceneFx.valid()||!bindview||!bindview->isInited()) return;
	//double t = m_ea->time();
	//if(t-m_lastupdtime<m_delta)
	//{
	//	return;
	//}
	//m_lastupdtime = t;
	ref_ptr<crTableIO> skillDataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
	int effectIndex = skillDataAttrTab->getTitleIndex("作用类型");
	void *param;
	crVector3 pos;
	crMatrixTransform *bot;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!camera.valid()) return;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	if(!cameraData) return;
	CRCore::crIntersectVisitor::HitList hits;
	bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),crSceneManager::getInstance()->getStaticRoot(),hits,~AutoTransMask);
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_JXJUseSkillDecal,param);
	m_decalNode = (crNode *)param;
	cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
	AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
	if(!m_decalNode)
	{
		if(!hits.empty())
		{
			(*m_playSceneFx)(*this);
			cameraData->inputParam(WCHDATA_JXJUseSkillDecal,m_decalNode);
		}
	}
	if(m_decalNode)
	{
		do 
		{
			if(useItemPair && useItemPair->first.valid())
			{
				crRole *role = useItemPair->first.get();
				//crRole *role = (*selectMyRoles)[0].get();
				crData *roleData = role->getDataClass();
				if(roleData)
				{
					roleData->getParam(WCHDATA_RTHP,param);
					float rthp = *(float*)param;
					if(rthp>0.0f)
					{
						int itemid = useItemPair->second;//*((int*)param);
						if(itemid != 0)
						{
							crItemChild *itemChild = role->findChildItem(itemid);
							if(itemChild && itemChild->isLoaded())
							{
								crInstanceItem *item = itemChild->getInstanceItem();
								crData *itemData = item->getDataClass();
								itemData->getParam(WCHDATA_ItemUseScope,param);
								if(param)
								{
									short scope = *(short *)param;
									if(scope>0)
									{
										crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode);
										if(skillRangeMatrix)
										{
											////float t = m_ea->time();
											////if(t-m_lastupdtime>m_delta)
											////{
											////	m_lastupdtime = t;
											//	callback->setVisiable(true);
											//	callback->setColor(crVector4(0,1,0,1));
											//	//crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
											//	if(!hits.empty())
											//	{
											//		CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
											//		callback->setPosition(crVector2(pickPoint[0],pickPoint[1]));
											//	}
											//	float fscope = (float)scope * crGlobalHandle::gData()->gUnitScale();
											//	fscope *= 2.0f;
											//	callback->setSize(crVector2(fscope,fscope));
											////}
											//break;
											if(!hits.empty())
											{
												m_decalNode->setVisiable(true);
												float fscope = (float)scope * crGlobalHandle::gData()->gUnitScale();
												float radius = fscope;
												fscope *= 2.0f;
												CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
												skillRangeMatrix->setMatrix(crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(pickPoint[0],pickPoint[1],pickPoint[2]));
												//crVector3f centerPos(pos[0], pos[1], pos[2] + m_zoffset);
												//itemData->getParam(WCHDATA_TargetType, param);
												//unsigned char targettype = *(unsigned char *)param;
												crTableIO::StrVec bufRec;
												if (skillDataAttrTab->queryOneRecord(0, crArgumentParser::appItoa(item->getAbstractItemID()), bufRec) >= 0)
												{//buf描述
													if (!bufRec[effectIndex].empty())
													{
														crVector2f vec;
														vec[0] = radius;
														vec[1] = atoi(bufRec[effectIndex].c_str());
														camera->getAttachedNode()->doEvent(WCH_JXJSkillEffectRange, MAKECREPARAM(&pickPoint, &vec));
													}
												}
												
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			m_decalNode->setVisiable(false);
		}
		while(0);
	}
	cameraData->getParam(WCHDATA_JXJUseSkillRangeNodeMap,param);
	SkillRangeNodeMap *skillNodeMap = (SkillRangeNodeMap *)param;
	CRCore::crVector3 dir;
	CRCore::crNode *decalNode = NULL;
	ref_ptr<crTableIO> skilltab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
	int effectRangeIndex = skilltab->getTitleIndex("影响范围");
	int effectTargetIndex = skilltab->getTitleIndex("作用类型");
	SkillRangeNodeMap::iterator itr_node = skillNodeMap->begin();
	for (;itr_node!=skillNodeMap->end();itr_node++)
	{
		if(itr_node->second.valid() && itr_node->second->getVisiable())
		{
			decalNode = itr_node->second.get();
		}
	}
	if(decalNode)
	{
		do 
		{
			if(useItemPair && useItemPair->first.valid())
			{
				crRole *role = useItemPair->first.get();
				crData *roleData = role->getDataClass();
				if(roleData)
				{
					roleData->getParam(WCHDATA_RTHP,param);
					float rthp = *(float*)param;
					if(rthp>0.0f)
					{
						int itemid = useItemPair->second;
						if(itemid != 0)
						{
							crItemChild *itemChild = role->findChildItem(itemid);
							if(itemChild && itemChild->isLoaded())
							{
								crInstanceItem *item = itemChild->getInstanceItem();
								crData *itemData = item->getDataClass();
								itemData->getParam(WCHDATA_ItemUseRange,param);
								if(param)
								{
									unsigned short range = *(unsigned short *)param;
									if(range>0)
									{
										crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(decalNode);
										if(skillRangeMatrix)
										{
											bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
											if(!bot || !bot->getVisiable())
											{
												skillRangeMatrix->setVisiable(false);
											}
											else
											{
												float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
												if(!hits.empty())
												{
													CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
													pos = bot->getTrans();
													CRCore::crVector3 offset= pickPoint-pos;
													offset[2]=0.0f;
													dir = offset.normalize();

												}
												skillRangeMatrix->setVisiable(true);
												CRCore::crMatrix newMat;
												if(dir == crVector3(0.0f,0.0f,0.0f))
												{
													dir[1] = -1.0f;
												}
												if(dir[1] > 0.999)
												{
													newMat.makeRotate(-CRCore::Y_AXIS,dir);
													CRCore::crVector3d ep,center,up;
													newMat.getLookAt(ep,center,up);
													if(center[2]>0.0f)
													{
														center = -center;
													}
													newMat.makeLookAt(ep,center,up);
												}
												else
													newMat.makeRotate(-CRCore::Y_AXIS,dir);
												if(fscope<300.0f)
													skillRangeMatrix->setMatrix(newMat*crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+1.5)));
												else
												{
													skillRangeMatrix->setMatrix(newMat*crMatrix::translate(crVector3f(pos[0], pos[1], pos[2] + 1.5)));

													crTableIO::StrVec skillrecord;
													if (skilltab->queryOneRecord(0, crArgumentParser::appItoa(item->getAbstractItemID()), skillrecord) >= 0)
													{
														CRCore::crVector3 volumeLength;
														crArgumentParser::appAtoVec(skillrecord[effectRangeIndex], volumeLength);
														newMat.setTrans(pos + dir*(volumeLength[1] - 2.0f));
														ref_ptr<crMatrixVolumeNode> volumeNode = new crMatrixVolumeNode;
														dynamic_cast<crMatrixVolumeNode *>(volumeNode.get())->setEffectMatrix(newMat);
														crVector3 centerpos = crVector3(0.0f, 0.0f/*2.0f-volumeLength[1]*/, 0.0f);
														volumeNode->setBoundBox(centerpos, volumeLength);

														unsigned char targettype = atoi(skillrecord[effectTargetIndex].c_str());
														camera->getAttachedNode()->doEvent(WCH_JXJSkillEffectRectangle, MAKECREPARAM(volumeNode.get(), &targettype));
													}
													
												}
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			decalNode->setVisiable(false);
		}while(0);
		
	}
}
/////////////////////////////////////////
//
//crJXJUseSkillRangeDecalCreateLogic
//
/////////////////////////////////////////
crJXJUseSkillRangeDecalCreateLogic::crJXJUseSkillRangeDecalCreateLogic():
	m_decalNode(NULL),
	m_zoffset(1.0f)
{
}

crJXJUseSkillRangeDecalCreateLogic::crJXJUseSkillRangeDecalCreateLogic(const crJXJUseSkillRangeDecalCreateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL),
	m_zoffset(handle.m_zoffset)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJUseSkillRangeDecalCreateLogic::inputParam(int i, void *param)
{
}

void crJXJUseSkillRangeDecalCreateLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	}
}

void crJXJUseSkillRangeDecalCreateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJUseSkillRangeDecalCreateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}
void crJXJUseSkillRangeDecalCreateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJUseSkillRangeDecalCreateLogic::operator()(crHandle &handle)
{
	if(!m_playSceneFx.valid()) return;
	void *param;
	std::string matrix;
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!camera.valid()) return;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	if(!cameraData) return;
	crVector3 pos;
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
	AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
	unsigned short range = 0;
	crRole *role = NULL;
	cameraData->getParam(WCHDATA_JXJUseSkillRangeNodeMap,param);

	CRCore::crIntersectVisitor::HitList hits;
	crCanvasNode * mouseCanvas = crFilterRenderManager::getInstance()->getMouseCanvas();
	if(mouseCanvas)
	{
		const crBoundingBox & bbox = mouseCanvas->getBoundBox();
		bindview->computeIntersections(bbox.xMin(),bbox.yMin(),crSceneManager::getInstance()->getStaticRoot(),hits,~AutoTransMask);
	}
	//m_decalNode = (crNode *)param;
	SkillRangeNodeMap * skillNodeMap = (SkillRangeNodeMap *)param;
	
	if(useItemPair && useItemPair->first.valid())
	{
		role = useItemPair->first.get();
		//role = (*selectMyRoles)[0].get();
		crData *roleData = role->getDataClass();
		//roleData->getParam(WCHDATA_AboutToUseItemID,param);
		int itemid = useItemPair->second;//*((int*)param);
		if(itemid != 0)
		{
			crItemChild *itemChild = role->findChildItem(itemid);
			if(itemChild && itemChild->isLoaded())
			{
				crInstanceItem *item = itemChild->getInstanceItem();
				crData *itemData = item->getDataClass();	
				int itemAbstractID = item->getAbstractItemID();
				ref_ptr<crTableIO>dataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
				itemData->getParam(WCHDATA_ItemUseRange,param);
				if(param && dataAttrTab.valid())
				{
					range = *(unsigned short *)param;
					int useRangeMatrix = dataAttrTab->getTitleIndex("范围模型");
					crTableIO::StrVec record;
					if(dataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(itemAbstractID),record)>=0 && useRangeMatrix>=0)
					{
						matrix = record[useRangeMatrix];
						SkillRangeNodeMap::iterator itr_skillNode = skillNodeMap->find(matrix);
						if(itr_skillNode == skillNodeMap->end() || !itr_skillNode->second.valid())
						{
							m_playSceneFx->inputParam(5,&matrix);
							(*m_playSceneFx)(*this);
							(*skillNodeMap)[matrix] = m_decalNode;
						}
						else
						{
							m_decalNode = itr_skillNode->second.get();
						}
					}
				}
			}
		}
	}
	if(m_decalNode)
	{
		do 
		{
			if(range>0 && role)
			{
				crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode);
				if(skillRangeMatrix)
				{
					crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
					if(!bot || !bot->getVisiable())
					{
						skillRangeMatrix->setVisiable(false);
					}
					else
					{
						skillRangeMatrix->setVisiable(true);
						pos = bot->getTrans();

						crVector3f dir;
						if(!hits.empty())
						{
							CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
							pos = bot->getTrans();
							CRCore::crVector3 offset= pickPoint-pos;
							offset[2]=0.0f;
							dir = offset.normalize();
						}
						else
						{
							dir = role->getDir();
						}

						
						CRCore::crMatrix newMat;
						if(dir == crVector3(0.0f,0.0f,0.0f))
						{
							dir[1] = -1.0f;
						}
						if(dir[1] > 0.999)
						{
							newMat.makeRotate(-CRCore::Y_AXIS,dir);
							CRCore::crVector3d ep,center,up;
							newMat.getLookAt(ep,center,up);
							if(center[2]>0.0f)
							{
								center = -center;
							}
							newMat.makeLookAt(ep,center,up);
						}
						else
							newMat.makeRotate(-CRCore::Y_AXIS,dir);
						float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
						if(fscope<300.0f)
							skillRangeMatrix->setMatrix(newMat*crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
						else
							skillRangeMatrix->setMatrix(newMat*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
						break;
					}
				}
			}
			m_decalNode->setVisiable(false);
		}
		while(0);
	}
}
/////////////////////////////////////////
//
//crJXJBattleMouseMoveLogic
//
/////////////////////////////////////////
crJXJBattleMouseMoveLogic::crJXJBattleMouseMoveLogic():
	m_targetType(0)
{
}

crJXJBattleMouseMoveLogic::crJXJBattleMouseMoveLogic(const crJXJBattleMouseMoveLogic& handle):
	crLogic(handle),
	m_targetType(0),
	m_enemyCursor(handle.m_enemyCursor),
	m_friendCursor(handle.m_friendCursor),
	m_ismeCursor(handle.m_ismeCursor),
	m_canbepickCursor(handle.m_canbepickCursor),
	m_controlableCursor(handle.m_controlableCursor),
	m_defaultCursor(handle.m_defaultCursor)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crJXJBattleMouseMoveLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJBattleMouseMoveLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_enemyCursor = atoi(str.c_str());
		break;
	case 1:
		m_friendCursor = atoi(str.c_str());
		break;
	case 2:
		m_ismeCursor = atoi(str.c_str());
		break;
	case 3:
		m_canbepickCursor = atoi(str.c_str());
		break;
	case 4:
		m_controlableCursor = atoi(str.c_str());
		break;
	case 5:
		m_defaultCursor = atoi(str.c_str());
		break;
	case 6:
		m_canWalkCursor = atoi(str.c_str());
		break;
	}
}

void crJXJBattleMouseMoveLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJBattleMouseMoveLogic::outputParam(int i, void *param)
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

void crJXJBattleMouseMoveLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	m_pickTarget->inputParam(3,NULL);
	//crData *cameraData = crCameraManager::getInstance()->getMainCamera()->getAttachedNode()->getDataClass();
	crCanvasNode *mouseCanvas = crFilterRenderManager::getInstance()->getMouseCanvas();
	char isEnemy = 0;
	crInstanceItem *targetItem = NULL;
	crInstanceItem *myItem = NULL;

	//鼠标判断
	if(mouseCanvas)
	{
		crMultiSwitch *mouseSW = dynamic_cast<crMultiSwitch *>(mouseCanvas->getChild(0));
		if(mouseSW)
		{
			if(m_targetType & Target_Bot)
			{
				crData *targetData = m_targetNode->getDataClass();
				targetData->getParam(WCHDATA_Item,param);
				targetItem = ((crInstanceItem*)param);
				if(targetItem)
				{
					crRole *me = crMyPlayerData::getInstance()->getCurrentRole();		
					me->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem,&isEnemy));
				}
				if(isEnemy == -1)
				{
					crData *data = targetItem->getDataClass();
					if(data)
					{
						data->getParam(WCHDATA_ItemState,param);
						unsigned char itemstate = *(unsigned char *)param;
						data->getParam(WCHDATA_RTHP,param);
						float rthp = *(float*)param;
						if(rthp<=0.0f || itemstate==IS_Dead)
						{
							mouseSW->setActiveSwitchSet(m_defaultCursor);
						}
						else
						{
							mouseSW->setActiveSwitchSet(m_enemyCursor);
						}
					}
					
				}
				else if(isEnemy == 1)
				{
					crRole *me = crMyPlayerData::getInstance()->getCurrentRole();	
					int myRoleID = me->getRoleID();
					if(crMyPlayerData::getInstance()->ifItemIsMe(targetItem))
						mouseSW->setActiveSwitchSet(m_ismeCursor);
					else
						mouseSW->setActiveSwitchSet(m_friendCursor);				
				}
			}
			else if(m_targetType & Target_Item)
			{
				crData *targetData = m_targetNode->getDataClass();
				targetData->getParam(WCHDATA_Item,param);
				targetItem = ((crInstanceItem*)param);
				if(targetItem && targetItem->getDataClass())
				{
					crData *data = targetItem->getDataClass();
					data->getParam(WCHDATA_ItemState,param);
					if(param)
					{
						unsigned char itemstate = *(unsigned char *)param;
						if(itemstate == IS_Dead)
						{
							mouseSW->setActiveSwitchSet(m_defaultCursor);
						}
						else
						{
							mouseSW->setActiveSwitchSet(m_canbepickCursor);
						}
					}
					else
					{
						mouseSW->setActiveSwitchSet(m_defaultCursor);
					}
				}
			}
			else
			{
				mouseSW->setActiveSwitchSet(m_defaultCursor);
			}

			if (mouseSW->getActiveSwitchSet() == m_defaultCursor)
			{
				crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
				crSceneLayer *scenelayer = crMyPlayerData::getInstance()->getSceneLayer();
				if (scenelayer && bindview && bindview->isInited() && crFilterRenderManager::getInstance()->getFocusNode() == NULL)
				{
					CRCore::crIntersectVisitor::HitList hits;
					bindview->computeIntersections(m_ea->getXnormalized(), m_ea->getYnormalized(), crSceneManager::getInstance()->getStaticRoot(), hits, ~AutoTransMask);
					if (!hits.empty())
					{
						CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
						crVector2s coord = scenelayer->getCoord(crVector2(pickPoint[0], pickPoint[1]));
						if (!scenelayer->walkability(coord[0], coord[1]))
						{
							mouseSW->setActiveSwitchSet(m_canWalkCursor);
						}
						else
						{
							mouseSW->setActiveSwitchSet(m_defaultCursor);
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid() && camera->getAttachedNode() && camera->getAttachedNode()->getDataClass())
	{
		crData* cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		crNode *fxNode;
		crDecalUpdateCallback *callback;
		if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
		{
			crData *targetData = m_targetNode->getDataClass();
			targetData->getParam(WCHDATA_Item,param);
			targetItem = ((crInstanceItem*)param);
			if(targetItem && targetItem->getDataClass())
			{
				crData *itemData = targetItem->getDataClass();
				itemData->getParam(WCHDATA_RTHP,param);
				if(param)
				{
					float rthp = *(float*)param;
					if(rthp>0.0f)
						targetItem->doEvent(WCH_JXJMouseOnRole);
				}
				else
				{
					targetItem->doEvent(WCH_JXJMouseOnRole);
				}
			}
		}
		else
		{
			cameraData->getParam(WCHDATA_JXJBattleMouseOnRoleVec,param);
			ItemVec *mouseonItem = (ItemVec *)param;
			ItemVec::iterator itr_item = mouseonItem->begin();
			if(!mouseonItem->empty())
			{
				while (itr_item!=mouseonItem->end())
				{
					fxNode = itr_item->get()->findSceneFxNode("$MouseOnDecal");
					if(fxNode)
					{
						callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
						if(callback)
						{
							callback->setVisiable(false);
							crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
							itr_item->get()->getRelNode()->accept(selectNodeVisitor);
						}
					}
					itr_item++;
				}
				mouseonItem->resize(0);
			}

			cameraData->getParam(WCHDATA_JXJMouseOnShuiJingNode,param);
			crNode *decalNode = (CRCore::crNode *)param;
			if(decalNode)
				decalNode->setVisiable(false);
			cameraData->getParam(WCHDATA_JXJMouseOnJianTaNode,param);
			crNode *decalNode2 = (CRCore::crNode *)param;
			if(decalNode2)
				decalNode2->setVisiable(false);
		}
	}
}
/////////////////////////////////////////
//
//crJXJMouseMoveLogic
//
/////////////////////////////////////////
crJXJMouseMoveLogic::crJXJMouseMoveLogic():
	m_targetType(0)
{
}

crJXJMouseMoveLogic::crJXJMouseMoveLogic(const crJXJMouseMoveLogic& handle):
	crLogic(handle),
	m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crJXJMouseMoveLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJMouseMoveLogic::addParam(int i, const std::string& str)
{
}

void crJXJMouseMoveLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJMouseMoveLogic::outputParam(int i, void *param)
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

void crJXJMouseMoveLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	crData *thisData = m_this->getDataClass();
	thisData->getParam(WCHDATA_TargetItem,param);
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	m_pickTarget->inputParam(3,NULL);

	thisData->inputParam(WCHDATA_TargetType,&m_targetType);
	//thisData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
	thisData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
	crCanvasNode *mouseCanvas = crFilterRenderManager::getInstance()->getMouseCanvas();
	crInstanceItem *targetItem = NULL;

	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem)
		{
			CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
			if(scene)
			{
				crInstanceItem *item  = scene->findSceneItem(targetItem->getInstanceItemID());
				if(item)
				{
					item->doEvent(WCH_JXJMouseOnBuildingItemEffect/*,MAKEINT64(0,NULL)*/);
				}
			}
		}
	}
	else
	{
		crData *brainData = crBrain::getInstance()->getDataClass();
		if(brainData)
		{
			brainData->getParam(WCHDATA_JXJMouseOneBulidingNode,param);
			crNode *node = (crNode *)param;
			if(node)
			{
				node->setVisiable(false);
			}
		}
	}
	thisData->inputParam(WCHDATA_TargetItem,targetItem);
}
/////////////////////////////////////////
//
//crJXJNpcAILogic
//
/////////////////////////////////////////
crJXJNpcAILogic::crJXJNpcAILogic():
	m_dt(0.0f),m_enmityid(0),m_isAboutToUseItem(false),
	m_aiInterval(0.5f),
	m_aitime(0.0f),
m_attackTargetCount(1),
m_aiTargetInterval(1.0f),
m_aiTargetTimer(0.0f),
m_attackIndex(0){}
crJXJNpcAILogic::crJXJNpcAILogic(const crJXJNpcAILogic& handle):
	crLogic(handle),
	m_dt(0.0f),
	m_enmityid(0),
	m_isAboutToUseItem(false),
	m_aiInterval(handle.m_aiInterval),
	m_aitime(0.0f),
	m_attackTargetCount(handle.m_attackTargetCount),
	m_aiTargetInterval(handle.m_aiTargetInterval),
	m_aiTargetTimer(0.0f),
	m_attackIndex(0)
{
	if(handle.m_useItemAndSendHandle.valid())
		m_useItemAndSendHandle = handle.m_useItemAndSendHandle->clone();
	if(handle.m_selectSkillHandle.valid())
		m_selectSkillHandle = handle.m_selectSkillHandle->clone();
	if(handle.m_patrolHandle.valid())
		m_patrolHandle = handle.m_patrolHandle->clone();
	if(handle.m_closeTargetHandle.valid())
		m_closeTargetHandle = handle.m_closeTargetHandle->clone();
	if(handle.m_npcRevert.valid())
		m_npcRevert = handle.m_npcRevert->clone();
}
void crJXJNpcAILogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			if(m_useItemAndSendHandle.valid())
				m_useItemAndSendHandle->inputParam(0,NULL);
			if(m_selectSkillHandle.valid())
				m_selectSkillHandle->inputHandle(0,NULL);
			if(m_patrolHandle.valid())
				m_patrolHandle->inputParam(0,NULL);
			if(m_closeTargetHandle.valid())
				m_closeTargetHandle->inputParam(0,NULL);
			if(m_npcRevert.valid())
				m_npcRevert->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float *)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}
void crJXJNpcAILogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_aiInterval = atof(str.c_str());
		break;
	case 1:
		m_attackTargetCount = atoi(str.c_str());
		break;
	case 2:
		m_aiTargetInterval = atof(str.c_str());
		break;
	}
}
void crJXJNpcAILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_useItemAndSendHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_selectSkillHandle = (CRCore::crHandle *)param;
		break;
	case 2:
		m_patrolHandle = (CRCore::crHandle *)param;
		break;
	case 3:
		m_closeTargetHandle = (CRCore::crHandle *)param;
		break;
	case 4:
		m_npcRevert = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJNpcAILogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_enmityid = *(_crInt64 *)param;
		break;
	case 2:
		m_isAboutToUseItem = *(bool *)param;
		break;
	case 3:
		m_useResult = *(unsigned short *)param;
		break;
	}
}
void crJXJNpcAILogic::operator()(crHandle &handle) 
{
	void *param;
	crData *thisData = m_this->getDataClass();
	//thisData->getParam(WCHDATA_Camp,param);
	//unsigned char camp = *(unsigned char *)param;
	//if(camp>2)
	//{//友军
	//	int test = 0;
	//}
	thisData->getParam(WCHDATA_ItemState,param);
	unsigned char itemstate = *((unsigned char*)param);
	if(itemstate == IS_Dead)
		return;
	unsigned int guisestate = GS_Normal;
	m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
	if(guisestate & GS_Garrison || guisestate & GS_Fetter || guisestate & GS_Swoon || guisestate & GS_Stagnate || guisestate & GS_ExtraShifting || guisestate & GS_Confusion)
	{//不可移动
		if(itemstate>=IS_Move && itemstate<=IS_AttackToPos)
		{
			itemstate = IS_Stop;
			thisData->inputParam(WCHDATA_ItemState,&itemstate);
		}
		m_this->getOrCreatePathFinder()->dirty();
	}
	if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable || guisestate & GS_Swoon || guisestate & GS_Stagnate || guisestate & GS_ExtraShifting || guisestate & GS_Confusion)
	{
		return;
	}
	ref_ptr<crInstanceItem> targetItem = m_this->serverGetTarget();
	if(targetItem.valid())
	{
		//if (targetItem != m_aiTargetItem) m_aiTargetItem = NULL;
		crData *targetItemData = targetItem->getDataClass();
		targetItemData->getParam(WCHDATA_ItemState,param);
		unsigned char targetItemState = *(unsigned char *)param;
		if(targetItemState == IS_Dead)
		{
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType,&targetType);
			itemstate = IS_Stop;
			thisData->inputParam(WCHDATA_ItemState,&itemstate);
			m_this->getOrCreatePathFinder()->dirty();
			targetItem = NULL;
			//m_aiTargetTimer = 0.0f;
			m_attackTargetVec.resize(0);
		}
		else
		{
			unsigned int targetGuisestate = GS_Normal;
			targetItem->doEvent(WCH_GetGuiseState, MAKECREPARAM(&targetGuisestate,NULL));
			if((targetGuisestate & GS_Hide && !(targetGuisestate & GS_UnHide)) || targetGuisestate & GS_Stagnate || targetGuisestate & GS_NoAttack  || targetGuisestate & GS_UnVisiable  || targetGuisestate & GS_StaticUnVisiable)
			{
				unsigned char targetType = Target_Coord;
				thisData->inputParam(WCHDATA_TargetType,&targetType);
				itemstate = IS_Stop;
				thisData->inputParam(WCHDATA_ItemState,&itemstate);
				m_this->getOrCreatePathFinder()->dirty();
				//m_aiTargetTimer = 0.0f;
				m_attackTargetVec.resize(0);
			}
		}
	}
	else
	{
		thisData->getParam(WCHDATA_TargetType, param);
		unsigned char targettype = *(unsigned char *)param;
		if (targettype & (Target_Instance | Target_StaticNpc))
		{
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType, &targetType);
			itemstate = IS_Stop;
			thisData->inputParam(WCHDATA_ItemState, &itemstate);
			m_this->getOrCreatePathFinder()->dirty();
			m_attackTargetVec.resize(0);
		}
	}

	m_aitime += m_dt;
	if(m_aitime > m_aiInterval)
	{
		if(guisestate & GS_Confusion)
		{//不可攻击,混乱移动
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType,&targetType);
			targetItem = NULL;
			m_aiTargetTimer = 0.0f;
			m_attackTargetVec.resize(0);
			return;
		}
		if(guisestate & GS_Sheep || guisestate & GS_OnHorse || guisestate & GS_Disarm || guisestate & GS_Scare || guisestate & GS_Nihility/* || guisestate & GS_NoAttack*/)
		{//这几种状态不可攻击
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType,&targetType);
			targetItem = NULL;
			m_aiTargetTimer = 0.0f;
			m_attackTargetVec.resize(0);
			if(!(guisestate & GS_Garrison) && !(guisestate & GS_Fetter) && !(guisestate & GS_Swoon) && !(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && !(guisestate & GS_ExtraShifting) && !(guisestate & GS_Confusion))
			{
				//进入巡逻
				if(!(guisestate & GS_Static) && !(guisestate & GS_StaticNoneBlock)/* && (itemstate == IS_Stop || itemstate == IS_AttackToPos) */)
				{
					if(m_patrolHandle.valid())
					{
						//CRCore::notify(CRCore::FATAL)<<"crJXJNpcAILogic:Patrol"<<std::endl;
						m_patrolHandle->inputParam(1,m_this);
						(*m_patrolHandle)(*this);
						m_patrolHandle->inputParam(1,NULL);
					}
				}
			}
			return;
		}
		m_attackIndex = 0;
		m_aiTargetTimer -= m_aitime;
		if(!(guisestate & GS_Static) && !(guisestate & GS_StaticNoneBlock) && itemstate >= IS_CloseTarget && itemstate <= IS_CloseTargetTouch)
		{
			if( m_closeTargetHandle.valid() )
			{
				m_closeTargetHandle->inputParam(1,m_this);
				(*m_closeTargetHandle)(*this);
				m_closeTargetHandle->inputParam(1,NULL);
			}
		}
		m_aitime = 0.0f;
		if(guisestate & GS_Taunt)
		{//普通攻击嘲讽目标
			thisData->getParam(WCHDATA_CurrentAttackID,param);
			int itemid = *((int*)param);
			if(itemid == 0)
			{
				thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
				thisData->getParam(WCHDATA_AttackMap,param);
				AttackMap *attackMap = (AttackMap *)param;
				if(attackMap)
				{
					itemid = attackMap->begin()->second;
					thisData->inputParam(WCHDATA_CurrentAttackID,&itemid);
				}
				thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			if( m_useItemAndSendHandle.valid() )
			{
				bool isAboutToUseItem = false;
				m_useItemAndSendHandle->inputParam(1,m_this);
				m_useItemAndSendHandle->inputParam(2,&itemid);
				m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
				(*m_useItemAndSendHandle)(*this);
				m_useItemAndSendHandle->inputParam(1,NULL);
			}
			return;
		}
		//if (m_this->getPathFinder() && m_this->getPathFinder()->getPathStatus() == crInstanceItem::crPathFinder::NonExistent)
		//{//找不到路径，修正目标点
		//	thisData->getParam(WCHDATA_TargetPos, param);
		//	crVector3* targetpos = (crVector3 *)param;
		//	crVector3 mypos = m_this->getPosition();
		//	crVector3 dir = mypos - *targetpos;
		//	dir.normalize();
		//	*targetpos += dir*3.0f;
		//}
		//检查是否施法
		thisData->getParam(WCHDATA_AboutToUseItemID,param);
		int itemid = *((int*)param);
		if(itemid!=0 && !(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
			!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
			/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
			/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
			!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
			!(guisestate & GS_ExtraShifting) /*&& !(guisestate & GS_NoAttack)*//* && (itemstate == IS_MoveToUseSkill || itemstate == IS_CloseTargetUseSkill || itemstate == IS_UseItem)*/)
		{
			bool isAboutToUseItem = true;
			if( m_useItemAndSendHandle.valid() )
			{
				m_useItemAndSendHandle->inputParam(1,m_this);
				m_useItemAndSendHandle->inputParam(2,&itemid);
				m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
				(*m_useItemAndSendHandle)(*this);
				m_useItemAndSendHandle->inputParam(1,NULL);
			}
			thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);
		}
		else
		{
			if(m_aiTargetTimer<=0.0f || m_attackIndex>=m_attackTargetVec.size())
			{
				//thisData->getParam(WCHDATA_Camp,param);
				//unsigned char camp = *(unsigned char *)param;
				//判断在我的警戒区域和视野范围内是否有敌人
				//CRCore::ref_ptr<CRNetApp::crInstanceItem> lastTarget;
				if(!m_attackTargetVec.empty())
				{
				//	if(m_attackTargetCount==1) lastTarget = m_attackTargetVec[0];
					m_attackTargetVec.resize(0);
				}
				//int enemycount = 0;
				//ref_ptr<crInstanceItem> nearestEnemy;
				thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
				thisData->getParam(WCHDATA_InPatrolEnemyMap,param);
				InPatrolEnemyMap *inPatrolMap = (InPatrolEnemyMap *)param;
				if(m_attackTargetCount>1)
				{//箭塔
					//距离最近的人优先考虑
					if(!inPatrolMap->empty())
					{
						int i = 0;
						for( InPatrolEnemyMap::iterator itr = inPatrolMap->begin();
							itr != inPatrolMap->end() && i<m_attackTargetCount;
							++itr,++i )
						{
							m_attackTargetVec.push_back(itr->second);
						}
					}
				}
				else
				{
					do 
					{
						if(!inPatrolMap->empty())
						{
							float dist = inPatrolMap->begin()->first;
							//判断是否在我攻击范围内
							thisData->getParam(WCHDATA_CurrentAttackID,param);
							itemid = *((int*)param);
							if(itemid == 0)
							{
								thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
								thisData->getParam(WCHDATA_AttackMap,param);
								AttackMap *attackMap = (AttackMap *)param;
								if(attackMap)
								{
									itemid = attackMap->begin()->second;
									thisData->inputParam(WCHDATA_CurrentAttackID,&itemid);
								}
								thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
							}
							if(itemid!=0)
							{
								crItemChild *itemChild = m_this->findChildItem(itemid);
								if (itemChild && itemChild->getInstanceItem())
								{
									crInstanceItem *item = itemChild->getInstanceItem();
									crData *itemData = item->getDataClass();
									if (itemData)
									{
										itemData->getParam(WCHDATA_ItemUseRange, param);
										float itemUseRange = *((unsigned short*)param);
										float relRange = itemUseRange * crGlobalHandle::gData()->gUnitScale();
										float attackDistance = m_this->getAttackDistance(inPatrolMap->begin()->second.get());
										relRange += attackDistance;
										if (dist <= relRange)
										{//在攻击范围内
											m_attackTargetVec.push_back(inPatrolMap->begin()->second);
											break;
										}
									}
								}
							}
						}
						//寻找攻击我的人
						thisData->getParam(WCHDATA_EnmityMap,param);
						EnmityMap *enmityMap = (EnmityMap *)param;
						if(!enmityMap->empty())
						{
							EnmityMap::iterator eitr;
							if(!inPatrolMap->empty())
							{//攻击我，离我最近的人
								for( InPatrolEnemyMap::iterator itr = inPatrolMap->begin();
									itr != inPatrolMap->end();
									++itr )
								{
									eitr = enmityMap->find(itr->second);
									if(eitr != enmityMap->end())
									{
										m_attackTargetVec.push_back(itr->second);
										break;
									}
								}
							}
							else
							{
								eitr = enmityMap->begin();
								m_attackTargetVec.push_back(eitr->first);
								break;
							}
						}
						////距离我最近的人
						if (!inPatrolMap->empty())
						{
							m_attackTargetVec.push_back(inPatrolMap->begin()->second);
							break;
						}
					} while (0);
				}
				//_crInt64 distEnmity = 0;
				//_crInt32 lastEnmity = 0;
				//if(inPatrolMap && !inPatrolMap->empty())
				//{
				//	//enemycount = enemyMap->size();
				//	if(lastTarget.valid())
				//	{
				//		for( InPatrolEnemyMap::iterator itr = inPatrolMap->begin();
				//			 itr != inPatrolMap->end();
				//			 ++itr )
				//		{
				//			if(itr->second.get() == lastTarget.get())
				//			{
				//				distEnmity = itr->first;
				//				lastEnmity = HIINT64(distEnmity);
				//			}
				//		}
				//		bool changeEnemy = false;
				//		if(lastEnmity>=0)
				//		{
				//			distEnmity = enemyMap->begin()->first;
				//			_crInt32 enmity = HIINT64(distEnmity);
				//			if((float)enmity>(float)lastEnmity * 1.2f)
				//			{//仇恨值>120%的时候转移目标
				//				changeEnemy = true;
				//			}
				//		}
				//		if(changeEnemy)
				//		{
				//			m_attackTargetVec.push_back(enemyMap->begin()->second);
				//		}
				//		else
				//		{
				//			m_attackTargetVec.push_back(lastTarget);
				//		}
				//	}
				//	else
				//	{
				//		int i = 0;
				//		for( InPatrolEnemyMap::iterator itr = enemyMap->begin();
				//			itr != enemyMap->end() && i<m_attackTargetCount;
				//			++itr,++i )
				//		{
				//			m_attackTargetVec.push_back(itr->second);
				//		}
				//	}
				//}
				//else
				//{
				//	CRCore::notify(CRCore::FATAL)<<"(错误)crJXJNpcAILogic NPC没有Patrol instanceid="<<m_this->getInstanceItemID()<<std::endl;
				//}
				thisData->excHandle(MAKECREPARAM(WCH_LockData,0));

				if(!m_attackTargetVec.empty())
					m_aiTargetTimer = m_aiTargetInterval;
				else
					m_aiTargetTimer = 0.0f;
			}
			if(!m_attackTargetVec.empty())
			{//回血速度参数变小
				//进入战斗状态
				if( m_selectSkillHandle.valid() )
				{
					m_selectSkillHandle->inputParam(1,m_this);
					//m_selectSkillHandle->inputParam(2,&enemycount);
					(*m_selectSkillHandle)(*this);
					m_selectSkillHandle->inputParam(1,NULL);
				}

				thisData->getParam(WCHDATA_AboutToUseItemID,param);
				int itemid = *((int*)param);
				if(itemid == 0)
				{//普通攻击
					unsigned char targettype;
					int targetid,targetroleid;
					bool isAboutToUseItem = false;
					targetItem = m_attackTargetVec.front();
					//m_attackTargetDeq.pop_front();
					targettype = targetItem->getItemtype()==crInstanceItem::Role?Target_Role:Target_Npc;
					thisData->inputParam(WCHDATA_TargetType,&targettype);
					targetid = targetItem->getID();
					targetroleid = targetItem->getRoleID();
					thisData->inputParam(WCHDATA_TargetID,&targetid);
					thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
					//m_this->getOrCreatePathFinder()->dirty();
					//crVector3 targetPos = nearestEnemy->getPosition();
					//thisData->inputParam(WCHDATA_TargetPos,&targetPos);
					thisData->getParam(WCHDATA_CurrentAttackID,param);
					itemid = *((int*)param);
					if(itemid == 0)
					{
						thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
						thisData->getParam(WCHDATA_AttackMap,param);
						AttackMap *attackMap = (AttackMap *)param;
						if(attackMap)
						{
							itemid = attackMap->begin()->second;
							thisData->inputParam(WCHDATA_CurrentAttackID,&itemid);
						}
						thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
					if( m_useItemAndSendHandle.valid() )
					{
						m_useItemAndSendHandle->inputParam(1,m_this);
						m_useItemAndSendHandle->inputParam(2,&itemid);
						m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
						(*m_useItemAndSendHandle)(*this);
						m_useItemAndSendHandle->inputParam(1,NULL);
					}
				}
				else
				{
					bool isAboutToUseItem = true;
					if( m_useItemAndSendHandle.valid() )
					{
						m_useItemAndSendHandle->inputParam(1,m_this);
						m_useItemAndSendHandle->inputParam(2,&itemid);
						m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
						(*m_useItemAndSendHandle)(*this);
						m_useItemAndSendHandle->inputParam(1,NULL);
					}
					thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);
				}
			}
			else if(!(guisestate & GS_Garrison) && !(guisestate & GS_Fetter) && !(guisestate & GS_Swoon) && !(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && !(guisestate & GS_ExtraShifting) && !(guisestate & GS_Confusion))
			{
				unsigned char targetType = Target_Coord;
				thisData->inputParam(WCHDATA_TargetType,&targetType);
				//回血
				if( m_npcRevert.valid() )
				{
					m_npcRevert->inputParam(1,m_this);
					(*m_npcRevert)(*this);
					m_npcRevert->inputParam(1,NULL);
				}
				//进入巡逻
				if(!(guisestate & GS_Static) && !(guisestate & GS_StaticNoneBlock)/* && (itemstate == IS_Stop || itemstate == IS_AttackToPos) */)
				{
					if(m_patrolHandle.valid())
					{
						//CRCore::notify(CRCore::FATAL)<<"crJXJNpcAILogic:Patrol"<<std::endl;
						m_patrolHandle->inputParam(1,m_this);
						(*m_patrolHandle)(*this);
						m_patrolHandle->inputParam(1,NULL);
					}
				}
			}
		}
	}
	else if(m_aitime>=0.2f && m_attackTargetCount>1 && m_attackIndex+1<m_attackTargetVec.size())
	{//多发攻击的CD要<0.2
		if(guisestate & GS_Sheep || guisestate & GS_OnHorse || guisestate & GS_Disarm || guisestate & GS_Confusion || guisestate & GS_Scare || guisestate & GS_Nihility)
		{//这几种状态不可攻击
			return;
		}
		//if(m_attackTargetCount>1)
		//{
		//	CRCore::notify(CRCore::FATAL)<<"crJXJNpcAILogic多发:"<<m_attackTargetDeq.size()<<std::endl;
		//}
		m_aitime = 0.0f;

		unsigned char targettype;
		int targetid,targetroleid;
		bool isAboutToUseItem = false;
		targetItem = m_attackTargetVec[++m_attackIndex].get();
		targettype = targetItem->getItemtype()==crInstanceItem::Role?Target_Role:Target_Npc;
		thisData->inputParam(WCHDATA_TargetType,&targettype);
		targetid = targetItem->getID();
		targetroleid = targetItem->getRoleID();
		thisData->inputParam(WCHDATA_TargetID,&targetid);
		thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
		//m_this->getOrCreatePathFinder()->dirty();
		//crVector3 targetPos = nearestEnemy->getPosition();
		//thisData->inputParam(WCHDATA_TargetPos,&targetPos);
		thisData->getParam(WCHDATA_CurrentAttackID,param);
		int itemid = *((int*)param);
		if( itemid!=0 && m_useItemAndSendHandle.valid() )
		{
			m_useItemAndSendHandle->inputParam(1,m_this);
			m_useItemAndSendHandle->inputParam(2,&itemid);
			m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
			(*m_useItemAndSendHandle)(*this);
			m_useItemAndSendHandle->inputParam(1,NULL);
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerAILogic
//
/////////////////////////////////////////
crJXJPlayerAILogic::crJXJPlayerAILogic():
	m_dt(0.0f),
	m_interval(0.1f),
	m_time(0.0f),
	//m_autoFightTime(0.0f),
	//m_autoFightInterval(0.0f),
	m_aiTargetInterval(2.0f),
	m_aiTargetTimer(0.0f)
	//m_attackFailed(0),
	//m_attackFailedRange(5)
{
	//m_autoFightIntervalVec.set(0.5f,2.0f);
}
crJXJPlayerAILogic::crJXJPlayerAILogic(const crJXJPlayerAILogic& handle):
	crLogic(handle),
	m_dt(0.0f),
	m_interval(handle.m_interval),
	m_time(0.0f),
	//m_autoFightTime(0.0f),
	//m_autoFightInterval(0.0f),
	//m_autoFightIntervalVec(handle.m_autoFightIntervalVec),
	m_aiTargetInterval(handle.m_aiTargetInterval),
	m_aiTargetTimer(0.0f)
	//m_attackFailed(0),
	//m_attackFailedRange(handle.m_attackFailedRange)
{
	if(handle.m_useItemAndSendHandle.valid())
		m_useItemAndSendHandle = handle.m_useItemAndSendHandle->clone();
	if(handle.m_patrolHandle.valid())
		m_patrolHandle = handle.m_patrolHandle->clone();
	if(handle.m_closeTargetHandle.valid())
		m_closeTargetHandle = handle.m_closeTargetHandle->clone();
}
void crJXJPlayerAILogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			if(m_useItemAndSendHandle.valid())
				m_useItemAndSendHandle->inputParam(0,NULL);
			if(m_patrolHandle.valid())
				m_patrolHandle->inputParam(0,NULL);
			if(m_closeTargetHandle.valid())
				m_closeTargetHandle->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float *)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}
void crJXJPlayerAILogic::addParam(int i, const std::string& str)

{
	switch (i)
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	case 1:
		//crArgumentParser::appAtoVec(str,m_autoFightIntervalVec);
		break;
	case 2:
		m_aiTargetInterval = atof(str.c_str());
		break;
	case 3:
	case 4:
	case 5:
		m_progressCanvas[i-3] = str;
		break;
	default:
		break;
	//case 2:
	//	m_selectTargetRange = atof(str.c_str());
	//	break;
	//case 3:
	//	m_attackFailedRange = atoi(str.c_str());
	//	break;
	}
}
void crJXJPlayerAILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_useItemAndSendHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_patrolHandle = (CRCore::crHandle *)param;
		break;
	case 2:
		m_closeTargetHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJPlayerAILogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 3:
		m_useItemResult = *(unsigned short *)param;
		break;
	}
}
void crJXJPlayerAILogic::operator()(crHandle &handle) 
{
	if(!crMyPlayerData::getInstance()->ifItemIsMe(m_this))
	{
		return;
	}
	void *param;
	crData *thisData = m_this->getDataClass();
	thisData->getParam(WCHDATA_ItemState,param);
	unsigned char itemstate = *((unsigned char*)param);
	if(itemstate == IS_Dead || itemstate == IS_Relive)
	{
		//m_itemstate_rec = IS_Stop;
		return;
	}
	unsigned int guisestate = GS_Normal;
	m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
	if (guisestate & GS_Garrison || guisestate & GS_Fetter || guisestate & GS_Swoon || guisestate & GS_Stagnate || guisestate & GS_ExtraShifting || guisestate & GS_Confusion)
	{//不可移动
		if (itemstate >= IS_Move && itemstate <= IS_AttackToPos)
		{
			itemstate = IS_Stop;
			thisData->inputParam(WCHDATA_ItemState, &itemstate);
		}
		m_this->getOrCreatePathFinder()->dirty();
	}
	if (guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable || guisestate & GS_Swoon || guisestate & GS_Stagnate || guisestate & GS_ExtraShifting || guisestate & GS_Confusion)
	{
		return;
	}
	//if (guisestate & GS_Stop)
	//{
	//	m_itemstate_rec = IS_Stop;
	//}
	//if(itemstate == IS_Relive)
	//{
	//	//m_itemstate_rec = IS_Stop;
	//	itemstate = IS_Stop;
	//	thisData->inputParam(WCHDATA_ItemState,&itemstate);
	//}
	//else 
	if(itemstate == IS_Defend || itemstate == IS_Move || itemstate == IS_MoveToPos)
	{
		//m_itemstate_rec = IS_Stop;
		thisData->inputParam(WCHDATA_ItemState_rec, NULL);
	}
	thisData->getParam(WCHDATA_ItemState_rec, param);
	unsigned char itemstate_rec = *((unsigned char*)param);
	if (itemstate_rec == IS_AttackToPos)
	{
		if(itemstate == IS_AttackToPos)
		{
			thisData->getParam(WCHDATA_MoveToPos,param);
			m_attackToPos = *(crVector3*)param;
		}
		else
		{
			//thisData->getParam(WCHDATA_MoveToPos,param);
			//crVector3 attackToPos = *(crVector3*)param;
			crVector3 pos = m_this->getPosition();
			pos[2] = m_attackToPos[2];
			float length = (m_attackToPos - pos).length();
			if(length<6.0f)
			{
				itemstate_rec = IS_Stop;
				thisData->inputParam(WCHDATA_ItemState_rec, &itemstate_rec);
			}
		}
	}
	else if(itemstate==IS_AttackToPos)
	{
		thisData->getParam(WCHDATA_MoveToPos,param);
		m_attackToPos = *(crVector3*)param;
		//m_attackToPos = thisData->targettos
		itemstate_rec = IS_AttackToPos;
		thisData->inputParam(WCHDATA_ItemState_rec, &itemstate_rec);
	}
	//thisData->getParam(WCHDATA_GuiseState,param);
	//unsigned char guiseState = *(unsigned char *)param;
	//if(guiseState == GS_UnVisiable || guiseState == GS_StaticUnVisiable)
	//	return;
	m_time += m_dt;
	if(m_time>m_interval)
	{
		if(guisestate & GS_Confusion)
		{//不可攻击,混乱移动
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType,&targetType);
			m_aiTargetTimer = 0.0f;
			return;
		}
		if(guisestate & GS_Sheep || guisestate & GS_OnHorse || guisestate & GS_Scare /*|| guisestate & GS_Nihility || guisestate & GS_Disarm*//* || guisestate & GS_NoAttack*/)
		{//这几种状态不可攻击
			unsigned char targetType = Target_Coord;
			thisData->inputParam(WCHDATA_TargetType,&targetType);
			m_aiTargetTimer = 0.0f;
			if (m_patrolHandle.valid() && !(guisestate & GS_Stop) && !(guisestate & GS_Garrison) && !(guisestate & GS_Fetter) && !(guisestate & GS_Swoon) && !(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && !(guisestate & GS_ExtraShifting) && !(guisestate & GS_Confusion))
			{
				if(itemstate_rec == IS_AttackToPos && itemstate == IS_Stop)
				{
					itemstate = IS_AttackToPos;
					//thisData->getParam(WCHDATA_MoveToPos,param);
					//crVector3 attackToPos = *(crVector3*)param;
					unsigned char targettype = Target_Coord;
					thisData->inputParam(WCHDATA_ItemState,&itemstate);
					thisData->inputParam(WCHDATA_TargetType,&targettype);
					thisData->inputParam(WCHDATA_TargetPos,&m_attackToPos);
					crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
				}
				else
				{
					thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
					thisData->getParam(WCHDATA_PatrolPointVec,param);
					PatrolPointVec *vec = (PatrolPointVec*)param;
					if(!vec->empty())
					{
						//CRCore::notify(CRCore::FATAL)<<"crJXJNpcAILogic:Patrol"<<std::endl;
						m_patrolHandle->inputParam(1,m_this);
						(*m_patrolHandle)(*this);
						m_patrolHandle->inputParam(1,NULL);
					}
					thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
			return;
		}
		ref_ptr<crInstanceItem> targetItem;
		ref_ptr<crMatrixTransform> targetNode;
		m_this->clientGetTarget(targetItem,targetNode);
		if(targetItem.valid())
		{
			//if(targetItem != m_aiTargetItem) m_aiTargetItem = NULL;
			unsigned char targetType = Target_Coord;
			crData *targetItemData = targetItem->getDataClass();
			targetItemData->getParam(WCHDATA_ItemState,param);
			unsigned char targetItemState = *(unsigned char *)param;
			if(targetItemState == IS_Dead)
			{
				thisData->inputParam(WCHDATA_TargetType,&targetType);
				itemstate = IS_Stop;
				thisData->inputParam(WCHDATA_ItemState,&itemstate);
				m_this->getOrCreatePathFinder()->dirty();
				targetItem = NULL;
				targetNode = NULL;
			}
			else
			{
				unsigned int targetGuisestate = GS_Normal;
				targetItem->doEvent(WCH_GetGuiseState, MAKECREPARAM(&targetGuisestate,NULL));
				if( (targetGuisestate & GS_Hide && !(targetGuisestate & GS_UnHide)) || targetGuisestate & GS_Stagnate || targetGuisestate & GS_NoAttack ||
					targetGuisestate & GS_UnVisiable || targetGuisestate & GS_StaticUnVisiable || 
					!targetNode->getVisiable() )
				{
					thisData->inputParam(WCHDATA_TargetType,&targetType);
					itemstate = IS_Stop;
					thisData->inputParam(WCHDATA_ItemState,&itemstate);
					m_this->getOrCreatePathFinder()->dirty();

					targetItem = NULL;
					targetNode = NULL;
				}
			}
		}
		else
		{
			thisData->getParam(WCHDATA_TargetType, param);
			unsigned char targettype = *(unsigned char *)param;
			if (targettype & (Target_Instance| Target_StaticNpc))
			{
				unsigned char targetType = Target_Coord;
				thisData->inputParam(WCHDATA_TargetType, &targetType);
				itemstate = IS_Stop;
				thisData->inputParam(WCHDATA_ItemState, &itemstate);
				m_this->getOrCreatePathFinder()->dirty();
			}
		}

		//m_autoFightTime += m_time;
		if(guisestate & GS_Taunt)
			m_aiTargetItem = NULL;
		if(m_aiTargetItem.valid())
		{
			m_aiTargetTimer -= m_dt;
			if(m_aiTargetTimer<=0.0f)
			{
				if(targetItem == m_aiTargetItem)
				{
				//	unsigned char targetType = Target_Coord;
				//	thisData->inputParam(WCHDATA_TargetType,&targetType);
				//	itemstate = IS_Stop;
				//	thisData->inputParam(WCHDATA_ItemState,&itemstate);
				//	m_this->getOrCreatePathFinder()->dirty();
					targetItem = NULL;
					targetNode = NULL;
				}
				m_aiTargetItem = NULL;
				m_aiTargetTimer = 0.0f;
			}
			else if(crMyPlayerData::getInstance()->isItemInRange(m_aiTargetItem.get()))
			{
				crData *targetItemData = m_aiTargetItem->getDataClass();
				targetItemData->getParam(WCHDATA_ItemState,param);
				unsigned char targetItemState = *(unsigned char *)param;
				if(targetItemState == IS_Dead)
				{
					m_aiTargetItem = NULL;
					m_aiTargetTimer = 0.0f;
				}
				else
				{
					unsigned int targetGuisestate = GS_Normal;
					m_aiTargetItem->doEvent(WCH_GetGuiseState, MAKECREPARAM(&targetGuisestate,NULL));
					if( (targetGuisestate & GS_Hide && !(targetGuisestate & GS_UnHide)) || targetGuisestate & GS_Stagnate || targetGuisestate & GS_NoAttack ||
						targetGuisestate & GS_UnVisiable || targetGuisestate & GS_StaticUnVisiable ||
						!m_aiTargetItem->getRelNode()->getVisiable() )
					{
						m_aiTargetItem = NULL;
						m_aiTargetTimer = 0.0f;
					}
				}
			}
			else
			{
				m_aiTargetItem = NULL;
				m_aiTargetTimer = 0.0f;
			}
		}
		if(itemstate >= IS_CloseTarget && itemstate <= IS_CloseTargetTouch)
		{
			if( m_closeTargetHandle.valid() )
			{
				m_closeTargetHandle->inputParam(1,m_this);
				(*m_closeTargetHandle)(*this);
				m_closeTargetHandle->inputParam(1,NULL);
			}
		}
		//if (m_this->getPathFinder() && m_this->getPathFinder()->getPathStatus() == crInstanceItem::crPathFinder::NonExistent)
		//{//找不到路径，修正目标点
		//	thisData->getParam(WCHDATA_TargetPos, param);
		//	crVector3* targetpos = (crVector3 *)param;
		//	crVector3 mypos = m_this->getPosition();
		//	crVector3 dir = mypos - *targetpos;
		//	dir.normalize();
		//	*targetpos += dir*3.0f;
		//}
		m_time = 0.0f;
		do
		{
			crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_this->getRelNode());
			if(!bot) break;
			if(!(guisestate & GS_Hide && !(guisestate & GS_UnHide)) && (guisestate & GS_Taunt) && !(guisestate & GS_Nihility) && !(guisestate & GS_Disarm))
			{//普通攻击嘲讽目标
				thisData->getParam(WCHDATA_CurrentAttackID,param);
				int itemid = *((int*)param);
				if(itemid == 0)
				{
					thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
					thisData->getParam(WCHDATA_AttackMap,param);
					AttackMap *attackMap = (AttackMap *)param;
					if(attackMap)
					{
						itemid = attackMap->begin()->second;
						thisData->inputParam(WCHDATA_CurrentAttackID,&itemid);
					}
					thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				if( m_useItemAndSendHandle.valid() )
				{
					bool isAboutToUseItem = false;
					m_useItemAndSendHandle->inputParam(1,m_this);
					m_useItemAndSendHandle->inputParam(2,&itemid);
					m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
					(*m_useItemAndSendHandle)(*this);
					m_useItemAndSendHandle->inputParam(1,NULL);
				}
				break;
			}
			//检查是否施法
			thisData->getParam(WCHDATA_AboutToUseItemID,param);
			int itemid = *((int*)param);
			if(itemid!=0 && !(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
				!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
				/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
				/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
				!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
				!(guisestate & GS_ExtraShifting) /*&& !(guisestate & GS_NoAttack)*/ && (itemstate == IS_MoveToUseSkill || itemstate == IS_CloseTargetUseSkill || itemstate == IS_UseItem))
			{
				bool isAboutToUseItem = true;
				m_useItemAndSendHandle->inputParam(1,m_this);
				m_useItemAndSendHandle->inputParam(2,&itemid);
				m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
				(*m_useItemAndSendHandle)(*this);
				m_useItemAndSendHandle->inputParam(1,NULL);
				if(m_useItemResult==UR_Succeed)
				{
					unsigned int index = crMyPlayerData::getInstance()->getLoadedRoleIndex(m_this->getRoleID());
					if(index<3)
					{
						ref_ptr<crCanvasNode> progressCanvas = crFilterRenderManager::getInstance()->findCanvas(m_progressCanvas[index]);
						if(progressCanvas.valid() && progressCanvas->getDataClass())
						{
							crData *progressCanvasData = progressCanvas->getDataClass();
							if (progressCanvasData)
							{
								progressCanvasData->inputParam(WCHDATA_JXJProgressCanvasAboutUseItemId, &itemid);
							}
							crFilterRenderManager::getInstance()->showCanvas(progressCanvas.get(),true);
						}
					}
					////施放完技能后，重新攻击上一次的目标
					if (targetItem.valid())
					{
						char isEnemy = 0;
						m_this->doEvent(WCH_EnemyCheck, MAKECREPARAM(targetItem.get(), &isEnemy));
						if (isEnemy == -1)
						{
							m_aiTargetItem = targetItem;
							return;
						}
					}
					if (m_aiTargetItem.valid())
					{
						crVector3 myPos = m_this->getPosition();
						float /*sightRange, */attackrange,dist;
						thisData->getParam(WCHDATA_CurrentAttackID, param);
						int itemid = *(int *)param;
						ref_ptr<crItemChild>aitemChild = m_this->findChildItem(itemid);
						if (aitemChild.valid() && aitemChild->isLoaded())
						{
							crInstanceItem *aitem = aitemChild->getInstanceItem();
							crData *aitemData = aitem->getDataClass();
							aitemData->getParam(WCHDATA_ItemUseRange, param);
							unsigned short itemUseRange = *((unsigned short*)param);
							attackrange = itemUseRange * crGlobalHandle::gData()->gUnitScale();
							attackrange += m_this->getAttackDistance(NULL);
						}
						//if (targetItem.valid())
						//{
						//	char isEnemy = 0;
						//	m_this->doEvent(WCH_EnemyCheck, MAKECREPARAM(targetItem.get(), &isEnemy));
						//	if (isEnemy == -1)
						//	{
						//		crVector3 skillTargetPos = targetItem->getPosition();
						//		dist = (skillTargetPos - myPos).length();
						//		if (dist <= attackrange)
						//		{
						//			m_aiTargetItem = targetItem;
						//			return;
						//		}
						//	}
						//}
						crVector3 targetPos = m_aiTargetItem->getPosition();
						dist = (targetPos - myPos).length();
						//m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&sightRange, NULL));
						//sightRange *= crGlobalHandle::gData()->gUnitScale();
						//if (sightRange < attackrange)
						//	sightRange = attackrange;
						if (dist <= attackrange+3.0f/*sightRange*/)
						{
							unsigned char targettype = m_aiTargetItem->getItemtype() == crInstanceItem::Role ? Target_Role : Target_Npc;
							thisData->inputParam(WCHDATA_TargetType, &targettype);
							thisData->inputParam(WCHDATA_TargetNode, m_aiTargetItem->getRelNode());
							int targetid = m_aiTargetItem->getID();
							int targetroleid = m_aiTargetItem->getRoleID();
							thisData->inputParam(WCHDATA_TargetID, &targetid);
							thisData->inputParam(WCHDATA_TargetRoleID, &targetroleid);
							thisData->inputParam(WCHDATA_TargetPos, &targetPos);
						}
					}
				}
				//m_autoFightTime = 0.0f;
				//m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
				return;
			}
			//if(m_this->getAbstractItemID() == 20019)
			//{//投石车
			//	if(itemstate != IS_AttackToPos && itemstate != IS_UseItem && itemstate != IS_Stop)
			//	{
			//		thisData->inputParam(WCHDATA_CurrentAttackID,NULL);
			//	}
			//	if(!(guisestate & GS_Hide))
			//	{
			//		thisData->getParam(WCHDATA_CurrentAttackID,param);
			//		itemid = *((int*)param);
			//		if(itemid!=0)
			//		{
			//			//crItemChild *itemChild = m_this->findChildItem(itemid);
			//			//if(itemChild && itemChild->isLoaded())
			//			//{
			//			//	crInstanceItem *item = itemChild->getInstanceItem();
			//			//	crGroup *weaponGroup = dynamic_cast<crGroup *>(item->getNode());
			//			//	if(weaponGroup)
			//			//	{
			//			//		crWeaponMatterObject *weapon = dynamic_cast<crWeaponMatterObject *>(weaponGroup->getChild(0));
			//			//		if(weapon)
			//			//			weapon->doEvent(WCH_AdjustBulletSpeed,MAKECREPARAM(m_this,NULL));
			//			//	}
			//			//}
			//			bool isAboutToUseItem = false;
			//			m_useItemAndSendHandle->inputParam(1,m_this);
			//			m_useItemAndSendHandle->inputParam(2,&itemid);
			//			m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
			//			(*m_useItemAndSendHandle)(*this);
			//			m_useItemAndSendHandle->inputParam(1,NULL);
			//		}
			//	}
			//	return;
			//}
			else if((!(guisestate & GS_Nihility) && !(guisestate & GS_Disarm))&&(!(guisestate & GS_Hide && !(guisestate & GS_UnHide)) || itemstate==IS_AttackToPos))
			{
				//bool autoFight = false;//是否切换自动战斗目标
				unsigned char targettype;
				do 
				{
					thisData->getParam(WCHDATA_TargetType,param);
					targettype = *(unsigned char *)param;
					if(targettype&Target_Instance || targettype & Target_StaticNpc/* || targettype & Target_StaticItem*/)
					{//追踪并攻击目标
						//ref_ptr<crInstanceItem> targetItem;
						//ref_ptr<crMatrixTransform> targetNode;
						//m_this->clientGetTarget(targetItem,targetNode);
						if(targetItem.valid() && m_this != targetItem.get() && targetItem->getRelNode() && targetItem->getRelNode()->getVisiable())
						{
							//crData *targetItemData = targetItem->getDataClass();
							//targetItemData->getParam(WCHDATA_ItemState,param);
							//unsigned char targetItemState = *(unsigned char *)param;
							//if(targetItemState == IS_Dead)
							//{
							//	unsigned char targetType = Target_Coord;
							//	thisData->inputParam(WCHDATA_TargetType,&targetType);
							//	itemstate = IS_Stop;
							//	thisData->inputParam(WCHDATA_ItemState,&itemstate);
							//	m_this->getOrCreatePathFinder()->dirty();
							//	targetItem = NULL;
							//	targetNode = NULL;
							//	break;
							//}
							//unsigned int targetGuiseState = GS_Normal;
							//targetItem->doEvent(WCH_GetGuiseState, MAKECREPARAM(&targetGuiseState,NULL));
							//if(targetGuiseState & GS_UnVisiable || targetGuiseState & GS_StaticUnVisiable || !targetNode->getVisiable() || targetGuiseState & GS_Stagnate)
							//{
							//	unsigned char targetType = Target_Coord;
							//	thisData->inputParam(WCHDATA_TargetType,&targetType);
							//	itemstate = IS_Stop;
							//	thisData->inputParam(WCHDATA_ItemState,&itemstate);
							//	m_this->getOrCreatePathFinder()->dirty();
							//	targetItem = NULL;
							//	targetNode = NULL;
							//	break;
							//}
							crVector3 targetPos = targetItem->getPosition();
							if(itemstate == IS_CloseTargetTouch)
							{
								//targetItemData->getParam(WCHDATA_TouchDistance,param);
								//unsigned short touchDistance = *(unsigned short *)param;
								float relDist = 5.0f;//(float)touchDistance * crGlobalHandle::gData()->gUnitScale();
								crVector3 myPos(m_this->getPosx(),m_this->getPosy(),m_this->getPosz());
								myPos *= crGlobalHandle::gData()->gUnitScale();
								crVector3 targetDir = targetPos - myPos;
								targetDir[2] = 0;
								float dist = targetDir.length();
								if(dist<=relDist)
								{
									targetItem->doEvent(WCH_Touch);
									itemstate = IS_Stop;
									thisData->inputParam(WCHDATA_ItemState,&itemstate);
								}
								return;
							}
							char isEnemy = 0;
							m_this->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem.get(),&isEnemy));
							if(isEnemy == -1)
							{
								//if(!autoFight)
								//{
									thisData->getParam(WCHDATA_CurrentAttackID,param);
									itemid = *((int*)param);
									if(itemid == 0)
									{
										thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
										thisData->getParam(WCHDATA_AttackMap,param);
										AttackMap *attackMap = (AttackMap *)param;
										if(attackMap)
										{
											itemid = attackMap->begin()->second;
											thisData->inputParam(WCHDATA_CurrentAttackID,&itemid);
										}
										thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
									}
									bool isAboutToUseItem = false;
									m_useItemAndSendHandle->inputParam(1,m_this);
									m_useItemAndSendHandle->inputParam(2,&itemid);
									m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
									(*m_useItemAndSendHandle)(*this);
									m_useItemAndSendHandle->inputParam(1,NULL);
									if(m_useItemResult==UR_Succeed)
									{
										m_aiTargetTimer = m_aiTargetInterval;
										m_aiTargetItem = targetItem;
									}
									//m_autoFightTime = 0.0f;
									//m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
									return;
								//}
							}
							else if(isEnemy==0)
							{//中立IS_CloseTargetTouch
								//targetItemData->getParam(WCHDATA_TouchDistance,param);
								//unsigned short touchDistance = *(unsigned short *)param;
								float relDist = 5.0f;//(float)touchDistance * crGlobalHandle::gData()->gUnitScale();
								crVector3 myPos(m_this->getPosx(),m_this->getPosy(),m_this->getPosz());
								myPos *= crGlobalHandle::gData()->gUnitScale();
								crVector3 targetDir = targetPos - myPos;
								targetDir[2] = 0;
								float dist = targetDir.length();
								if(dist<=relDist)
								{
									targetItem->doEvent(WCH_Touch);
									itemstate = IS_Stop;
									thisData->inputParam(WCHDATA_ItemState,&itemstate);
								}
								return;
							}
						}
					}
					//autoFight = true;
				} while (0);
				if(itemstate == IS_Stop || itemstate == IS_Defend || itemstate == IS_UseItem || itemstate == IS_AttackToPos || itemstate == IS_Patrol/* || autoFight*/)
				{//自动选择附近的可攻击目标
					//if(m_autoFightTime > m_autoFightInterval)
					CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
					crSceneLayer *scenelayer = scene->getSceneLayer(m_this->getLayerID());
					crVector2s coord = scenelayer->getCoord(m_this->getPosx()*crGlobalHandle::gData()->gUnitScale(),m_this->getPosy()*crGlobalHandle::gData()->gUnitScale());
					unsigned char mapProtected = scenelayer->getMapProtected(coord[0],coord[1]);
					if(mapProtected == MP_Normal)
					{
						//判断是否需要回血回蓝
						//随机技能
						//选择敌人
						//crMyPlayerData::ItemNpcPair itemnpc = crMyPlayerData::getInstance()->getOneInRangeNpc(m_this,m_selectTargetRange);
						//crInstanceItem *enemy = crMyPlayerData::getInstance()->getOneInRangeEnemy(m_this);
						//避免玩家因为施放技能而切换目标
						//crInstanceItem *enemy = m_aiTargetItem.valid()?m_aiTargetItem.get():crMyPlayerData::getInstance()->getOneInRangeEnemy(m_this);
						crInstanceItem *enemy = crMyPlayerData::getInstance()->getOneInRangeEnemy(m_this);
						if(enemy)
						{
							m_aiTargetItem = enemy;
							m_aiTargetTimer = m_aiTargetInterval;

							targettype = enemy->getItemtype()==crInstanceItem::Role?Target_Role:Target_Npc;
							thisData->inputParam(WCHDATA_TargetType,&targettype);
							thisData->inputParam(WCHDATA_TargetNode,enemy->getRelNode());
							int targetid = enemy->getID();
							int targetroleid = enemy->getRoleID();
							thisData->inputParam(WCHDATA_TargetID,&targetid);
							thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
							crVector3 targetPos = enemy->getPosition();
							thisData->inputParam(WCHDATA_TargetPos,&targetPos);
							//m_this->getOrCreatePathFinder()->dirty();
							return;
						}
						//m_autoFightTime = 0.0f;
						//m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
					}
				}
				if (!(guisestate & GS_Stop) && !(guisestate & GS_Garrison) && !(guisestate & GS_Fetter) && !(guisestate & GS_Swoon) && !(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && !(guisestate & GS_ExtraShifting) && !(guisestate & GS_Confusion))
				{
					if(itemstate_rec == IS_AttackToPos && itemstate == IS_Stop)
					{
						itemstate = IS_AttackToPos;
						//thisData->getParam(WCHDATA_MoveToPos,param);
						//crVector3 attackToPos = *(crVector3*)param;
						unsigned char targettype = Target_Coord;
						thisData->inputParam(WCHDATA_ItemState,&itemstate);
						thisData->inputParam(WCHDATA_TargetType,&targettype);
						thisData->inputParam(WCHDATA_TargetPos,&m_attackToPos);
						crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
					}
					else if(m_patrolHandle.valid())
					{
						thisData->excHandle(MAKECREPARAM(WCH_LockData,1));
						thisData->getParam(WCHDATA_PatrolPointVec,param);
						PatrolPointVec *vec = (PatrolPointVec*)param;
						if(!vec->empty())
						{
							//CRCore::notify(CRCore::FATAL)<<"crJXJNpcAILogic:Patrol"<<std::endl;
							m_patrolHandle->inputParam(1,m_this);
							(*m_patrolHandle)(*this);
							m_patrolHandle->inputParam(1,NULL);
						}
						thisData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
		}while(0);
		//m_autoFightTime = 0.0f;
		//m_autoFightInterval = m_autoFightIntervalVec[1];
	}
}
/////////////////////////////////////////
//
//crJXJDownloadFirstSceneFileLogic
//
/////////////////////////////////////////
crJXJDownloadFirstSceneFileLogic::crJXJDownloadFirstSceneFileLogic():
	m_needDownload(false){}
crJXJDownloadFirstSceneFileLogic::crJXJDownloadFirstSceneFileLogic(const crJXJDownloadFirstSceneFileLogic& handle):
	crLogic(handle),
	m_needDownload(false)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crJXJDownloadFirstSceneFileLogic::inputParam(int i, void *param)
{
}

void crJXJDownloadFirstSceneFileLogic::addParam(int i, const std::string& str)
{
}
void crJXJDownloadFirstSceneFileLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJDownloadFirstSceneFileLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crJXJDownloadFirstSceneFileLogic::operator()(crHandle &handle)
{
	bool needDownload = false; 
	crQueryGameListData *selectGame = crMyPlayerData::getInstance()->getSelectedGame();
	if(m_checkAndDownloadFileMathod.valid() && selectGame)
	{//检查并下载Scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		//if(gamedata) gamedata->lockHttpDownload();

		m_needDownload = false;
		std::string filename;
		int downloadFlg = HttpD_FirstSceneFile;

		ref_ptr<crTableIO>table = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SceneTab);
		crData *data = crRunGameHandle::getInstance()->getDataClass();
		void *param;
		data->getParam(WCHDATA_SceneID,param);
		short sceneid = *(short *)param;
		crTableIO::StrVec record;
		int taskcount = 0;
		crDownloadManager::getInstance()->setCompleteCount(0);
		if(table.valid() && table->queryOneRecord(0,crArgumentParser::appItoa(sceneid),record)>=0)
		{
			std::string sceneFile = record[2];
			std::string uifile = crGlobalHandle::getInstance()->getUISceneDir()+record[3];

			std::string str = crArgumentParser::getFileNameEliminateExt(sceneFile) + ".crb";
			ref_ptr<crTableIO> table = crTableIO::openFile(str);
			int rowcount;
			if(table.valid())
			{
				rowcount = table->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					filename = (*table)(i,0);
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
					if(m_needDownload)
						taskcount++;
				}
			}
			filename = sceneFile;
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
			if(m_needDownload)
				taskcount++;

			str = crArgumentParser::getFileNameEliminateExt(uifile) + ".crb";
			table = crTableIO::openFile(str);
			if(table.valid())
			{
				rowcount = table->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					filename = (*table)(i,0);
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
					if(m_needDownload)
						taskcount++;
				}
			}
			filename = uifile;
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
			if(m_needDownload)
				taskcount++;
		}
		//CRCore::notify(CRCore::ALWAYS)<<"JXJDownloadFirstSceneFile "<<std::endl;
		if(gamedata && needDownload)
		{
			char gbuf[256];
			sprintf(gbuf,"JXJDownloadFirstSceneFile\n\0");
			gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			gamedata->startHttpDownloadTask(downloadFlg);
			crDownloadManager::getInstance()->setTaskCount(taskcount);
		}
		//if(gamedata) gamedata->unlockHttpDownload();
	}
	handle.outputParam(0,&needDownload);
}
///////////////////////////////////////////
////
////crJXJScriptDownloadTaskLogic
////
///////////////////////////////////////////
//crJXJScriptDownloadTaskLogic::crJXJScriptDownloadTaskLogic():
//	m_needDownload(false){}
//crJXJScriptDownloadTaskLogic::crJXJScriptDownloadTaskLogic(const crJXJScriptDownloadTaskLogic& handle):
//	crLogic(handle),
//	m_needDownload(false)
//{
//	if(handle.m_checkAndDownloadFileMathod.valid())
//		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
//}
//void crJXJScriptDownloadTaskLogic::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_fileList = *(std::vector<std::string> *)(LOINT64(param64));
//		}
//		else
//		{
//			m_fileList.clear();
//		}
//		break;
//	}
//}
//
//void crJXJScriptDownloadTaskLogic::addParam(int i, const std::string& str)
//{
//}
//void crJXJScriptDownloadTaskLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
//		break;
//	}
//}
//void crJXJScriptDownloadTaskLogic::outputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_needDownload = *(bool *)param;
//		break;
//	}
//}
//void crJXJScriptDownloadTaskLogic::operator()(crHandle &handle)
//{
//	if(!m_fileList.empty() && m_checkAndDownloadFileMathod.valid())
//	{//检查并下载Scene数据
//		std::string filename,str;
//		ref_ptr<crTableIO> table;
//		int rowcount;
//		m_needDownload = false;
//		bool needDownload = false;
//		int downloadFlg = HttpD_Script;
//		for( StrVec::iterator itr = m_fileList.begin();
//			 itr != m_fileList.end();
//			 ++itr )
//		{
//			filename = *itr;
//			if(isModelFile(crArgumentParser::getFileExtension(filename)))
//			{
//				str = crArgumentParser::getFileNameEliminateExt(filename) + ".crb";
//				table = new crTableIO;
//				if(table->openFile(str))
//				{
//					rowcount = table->getRowCount();
//					for (int i = 0; i < rowcount; i++)
//					{
//						filename = (*table)(i,0);
//						m_checkAndDownloadFileMathod->inputParam(1,&filename);
//						m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
//						(*m_checkAndDownloadFileMathod)(*this);
//						needDownload |= m_needDownload;
//					}
//				}
//				m_checkAndDownloadFileMathod->inputParam(1,&filename);
//				m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
//				(*m_checkAndDownloadFileMathod)(*this);
//				needDownload |= m_needDownload;
//			}
//			else
//			{
//				m_checkAndDownloadFileMathod->inputParam(1,&filename);
//				m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
//				(*m_checkAndDownloadFileMathod)(*this);
//				needDownload |= m_needDownload;
//			}
//		}
//		if(!needDownload)
//		{//不需要下载
//			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,downloadFlg));
//		}
//		CRCore::notify(CRCore::ALWAYS)<<"JXJScriptDownloadTask "<<std::endl;
//	}
//}
//bool crJXJScriptDownloadTaskLogic::isModelFile(const std::string& extension) const
//{
//	return CRIOManager::equalCaseInsensitive(extension,"cre") ||
//		CRIOManager::equalCaseInsensitive(extension,"cru") ||
//		CRIOManager::equalCaseInsensitive(extension,"crs");
//}
/////////////////////////////////////////
//
//crJXJPreLoadDownloadTaskLogic
//
/////////////////////////////////////////
crJXJPreLoadDownloadTaskLogic::crJXJPreLoadDownloadTaskLogic():
	m_needDownload(false){}
crJXJPreLoadDownloadTaskLogic::crJXJPreLoadDownloadTaskLogic(const crJXJPreLoadDownloadTaskLogic& handle):
	crLogic(handle),
	m_needDownload(false)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crJXJPreLoadDownloadTaskLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_file = *(std::string *)(LOCREPARAM(param64));
		}
		break;
	}
}

void crJXJPreLoadDownloadTaskLogic::addParam(int i, const std::string& str)
{
}
void crJXJPreLoadDownloadTaskLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJPreLoadDownloadTaskLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crJXJPreLoadDownloadTaskLogic::operator()(crHandle &handle)
{
	if(!m_file.empty() && m_checkAndDownloadFileMathod.valid())
	{//检查并下载Scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		//if(gamedata) gamedata->lockHttpDownload();
		std::string filename,str;
		ref_ptr<crTableIO> table;
		int rowcount;
		m_needDownload = false;
		bool needDownload = false;
		unsigned short id = crGlobalHandle::gainHttpDownloadID();
		int downloadFlg = MAKEINT32(HttpD_None,id);
		filename = m_file;
		if(isModelFile(crArgumentParser::getFileExtension(filename)))
		{
			str = crArgumentParser::getFileNameEliminateExt(filename) + ".crb";
			table = crTableIO::openFile(str);
			if(table.valid())
			{
				rowcount = table->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					filename = (*table)(i,0);
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
				}
			}
			filename = m_file;
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
		}
		else
		{
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
		}
		if(!needDownload)
		{//不需要下载
			//crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,downloadFlg));
			crGlobalHandle::recycleHttpDownloadID(id);
		}
		else if(gamedata)
		{
			char gbuf[256];
			sprintf(gbuf,"JXJPreLoadDownloadTask\n\0");
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			gamedata->startHttpDownloadTask(downloadFlg);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"JXJPreLoadDownloadTask "<<std::endl;
		//if(gamedata) gamedata->unlockHttpDownload();
	}
}
bool crJXJPreLoadDownloadTaskLogic::isModelFile(const std::string& extension) const
{
	return CRIOManager::equalCaseInsensitive(extension,"cre") ||
		CRIOManager::equalCaseInsensitive(extension,"cru") ||
		CRIOManager::equalCaseInsensitive(extension,"crs");
}
/////////////////////////////////////////
//
//crJXJBattleMouseMoveNewLogic
//
/////////////////////////////////////////
crJXJBattleMouseMoveNewLogic::crJXJBattleMouseMoveNewLogic():
	m_targetType(0)
{
}

crJXJBattleMouseMoveNewLogic::crJXJBattleMouseMoveNewLogic(const crJXJBattleMouseMoveNewLogic& handle):
	crLogic(handle),
	m_targetType(0),
	m_mouseAttack(handle.m_mouseAttack),
	m_mouseAttackSelf(handle.m_mouseAttackSelf),
	m_mouseAttackEnemy(handle.m_mouseAttackEnemy)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crJXJBattleMouseMoveNewLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJBattleMouseMoveNewLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mouseAttack = atoi(str.c_str());
		break;
	case 1:
		m_mouseAttackSelf = atoi(str.c_str());
		break;
	case 2:
		m_mouseAttackEnemy = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJBattleMouseMoveNewLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJBattleMouseMoveNewLogic::outputParam(int i, void *param)
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

void crJXJBattleMouseMoveNewLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	m_pickTarget->inputParam(3,NULL);
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!camera.valid()) return;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	crInstanceItem *targetItem = NULL;
	crNode *fxNode;
	crDecalUpdateCallback *callback;


	cameraData->getParam(WCHDATA_JXJCameraOperator, param);
	unsigned short attackToPos = *(unsigned short *)param;
	if (attackToPos == Cam_AttackToPos || attackToPos == Cam_MoveToPos || attackToPos == Cam_Patrol || attackToPos == Cam_Signal)
	{
		crCanvasNode *mouseCanvas = crFilterRenderManager::getInstance()->getMouseCanvas();
		char isEnemy = 0;
		crInstanceItem *myItem = NULL;

		//鼠标判断
		if (mouseCanvas)
		{
			crMultiSwitch *mouseSW = dynamic_cast<crMultiSwitch *>(mouseCanvas->getChild(0));
			if (mouseSW)
			{
				if (m_targetType & Target_Bot)
				{
					crData *targetData = m_targetNode->getDataClass();
					targetData->getParam(WCHDATA_Item, param);
					targetItem = ((crInstanceItem*)param);
					if (targetItem)
					{
						crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
						me->doEvent(WCH_EnemyCheck, MAKECREPARAM(targetItem, &isEnemy));
					}
					if (isEnemy == -1)
					{
						mouseSW->setActiveSwitchSet(m_mouseAttackEnemy);
					}
					else if (isEnemy == 1)
					{
						crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
						int myRoleID = me->getRoleID();
						mouseSW->setActiveSwitchSet(m_mouseAttackSelf);
					}
				}
				else
				{
					mouseSW->setActiveSwitchSet(m_mouseAttack);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem && targetItem->getDataClass())
		{
			//targetItem->doEvent(WCH_JXJMouseOnRole);
			crData *itemData = targetItem->getDataClass();
			itemData->getParam(WCHDATA_RTHP,param);
			if(param)
			{
				float rthp = *(float*)param;
				if(rthp>0.0f)
					targetItem->doEvent(WCH_JXJMouseOnRole);
			}
		}
	}
	else
	{
		cameraData->getParam(WCHDATA_JXJBattleMouseOnRoleVec,param);
		ItemVec *mouseonItem = (ItemVec *)param;
		ItemVec::iterator itr_item = mouseonItem->begin();
		if(!mouseonItem->empty())
		{
			while (itr_item!=mouseonItem->end())
			{
				fxNode = itr_item->get()->findSceneFxNode("$MouseOnDecal");
				if(fxNode)
				{
					callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						callback->setVisiable(false);
						crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
						itr_item->get()->getRelNode()->accept(selectNodeVisitor);
					}
				}
				itr_item++;
			}
			mouseonItem->resize(0);
		}
	}
}
/////////////////////////////////////////
//
//crJXJBattleMouseMoveTargetSkillLogic
//
/////////////////////////////////////////
crJXJBattleMouseMoveTargetSkillLogic::crJXJBattleMouseMoveTargetSkillLogic():
	m_targetType(0)
{
}

crJXJBattleMouseMoveTargetSkillLogic::crJXJBattleMouseMoveTargetSkillLogic(const crJXJBattleMouseMoveTargetSkillLogic& handle):
	crLogic(handle),
	m_targetType(0),
	m_cannotmouse(handle.m_cannotmouse)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crJXJBattleMouseMoveTargetSkillLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
			if(m_pickTarget.valid())
				m_pickTarget->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJBattleMouseMoveTargetSkillLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_cannotmouse = atoi(str.c_str());
		break;
	}
}

void crJXJBattleMouseMoveTargetSkillLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJBattleMouseMoveTargetSkillLogic::outputParam(int i, void *param)
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

void crJXJBattleMouseMoveTargetSkillLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	crGroup *root = crSceneManager::getInstance()->getBotRoot();
	m_pickTarget->inputParam(3,root);
	(*m_pickTarget)(*this);
	m_pickTarget->inputParam(3,NULL);
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!camera.valid()) return;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	crInstanceItem *targetItem = NULL;
	crNode *fxNode;
	crDecalUpdateCallback *callback;

	cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
	AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
	if(useItemPair)
	{
		crRole *role = useItemPair->first.get();
		crVector3 pos;
		//crMatrixTransform *bot;
		if(role)
		{
			crData *roleData = role->getDataClass();
			int itemid = useItemPair->second;
			unsigned int guisestate = GS_Normal;
			role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			roleData->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;

			if(itemstate != IS_Dead && rthp>0.0f && itemid != 0 && !(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
				!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
				/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
				/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
				!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
				!(guisestate & GS_ExtraShifting)/* && !(guisestate & GS_NoAttack)*/)
			{
				crItemChild *itemChild = role->findChildItem(itemid);
				if(itemChild && itemChild->isLoaded())
				{
					unsigned short range = 0;
					crInstanceItem *item = itemChild->getInstanceItem();
					crData *itemData = item->getDataClass();
					itemData->getParam(WCHDATA_TargetType,param);
					unsigned char targetType = *(unsigned char *)param;
					bool can = false;
					if(targetType & Target_Coord || targetType & m_targetType)
					{//对象符合
						can = true;
						if(targetType != Target_Bot && targetType != Target_Instance && targetType != Target_All && targetType != Target_StaticNpc/* && targetType != Target_StaticItem*/)
						{
							char isEnemy = 0;
							if (m_targetType & Target_Bot)
							{
								crData *targetData = m_targetNode->getDataClass();
								targetData->getParam(WCHDATA_Item, param);
								targetItem = ((crInstanceItem*)param);
								if(targetItem)
									role->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem,&isEnemy));
							}
							do 
							{
								if(isEnemy == -1)
								{
									if(targetType & Target_Friend || targetType & Target_Self)
									{//不能敌方使用
										can=false;
										break;
									}
								}
								else if(isEnemy == 1)
								{
									if(targetType & Target_Enemy)
									{//不能对友方使用
										can=false;
										break;
									}
									if(targetType == Target_Self)
									{//只能对自己使用
										if( targetItem->getItemtype()!=crInstanceItem::Role || 
											targetItem->getID() != role->getID())
										{
											can=false;
											break;
										}
									}
								}
							} while (0);
						}
					}
					if(!can)
					{
						crCanvasNode *canvas = crFilterRenderManager::getInstance()->getMouseCanvas();
						if(canvas)
						{
							crMultiSwitch *mouseSW = dynamic_cast<crMultiSwitch *>(canvas->getChild(0));
							if(mouseSW)
							{
								mouseSW->setActiveSwitchSet(m_cannotmouse);
							}
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if(m_targetType & Target_Instance || m_targetType & Target_StaticNpc /*|| m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem && targetItem->getDataClass())
		{
			//targetItem->doEvent(WCH_JXJMouseOnRole);
			crData *itemData = targetItem->getDataClass();
			itemData->getParam(WCHDATA_RTHP,param);
			if(param)
			{
				float rthp = *(float*)param;
				if(rthp>0.0f)
					targetItem->doEvent(WCH_JXJMouseOnRole);
			}
		}
	}
	else
	{
		cameraData->getParam(WCHDATA_JXJBattleMouseOnRoleVec,param);
		ItemVec *mouseonItem = (ItemVec *)param;
		ItemVec::iterator itr_item = mouseonItem->begin();
		if(!mouseonItem->empty())
		{
			while (itr_item!=mouseonItem->end())
			{
				fxNode = itr_item->get()->findSceneFxNode("$MouseOnDecal");
				if(fxNode)
				{
					callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						callback->setVisiable(false);
						crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
						itr_item->get()->getRelNode()->accept(selectNodeVisitor);
					}
				}
				itr_item++;
			}
			mouseonItem->resize(0);
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuildingItemEffectLogic
//
/////////////////////////////////////////
crJXJBuildingItemEffectLogic::crJXJBuildingItemEffectLogic():
	m_effectNode(NULL),
	m_visiable(true){}
crJXJBuildingItemEffectLogic::crJXJBuildingItemEffectLogic(const crJXJBuildingItemEffectLogic& handle):
	crNodeLogic(handle),
	m_effectNode(NULL),
	m_positionOffset(handle.m_positionOffset),
	m_visiable(handle.m_visiable)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crJXJBuildingItemEffectLogic::inputParam(int i, void *param)
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
		m_this = (crInstanceItem *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_visiable = LOCREPARAM(param64);
		}
		break;
	}
}
void crJXJBuildingItemEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	}
}
void crJXJBuildingItemEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crJXJBuildingItemEffectLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_effectNode = (CRCore::crNode *)param;
		break;
	}
}
void crJXJBuildingItemEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJBuildingItemEffectLogic::operator()(crHandle &handle)
{
	if(m_this->getRelNode())
	{
		if(!m_effectNode)
		{
			crVector3 position = m_this->getPosition();	
			position += m_positionOffset;
			m_playSceneFx->inputParam(3,&position);
			crVector3 dir = m_this->getDir();
			m_playSceneFx->inputParam(4,&dir);
			(*m_playSceneFx)(*this);
		}
		if(m_effectNode)
		{
			m_effectNode->setVisiable(m_visiable);
		}
	}
}

/////////////////////////////////////////
//
//crJXJMouseOnBuildingItemEffectLogic
//
/////////////////////////////////////////
crJXJMouseOnBuildingItemEffectLogic::crJXJMouseOnBuildingItemEffectLogic():
	m_effectNode(NULL),
	m_visiable(true){}
crJXJMouseOnBuildingItemEffectLogic::crJXJMouseOnBuildingItemEffectLogic(const crJXJMouseOnBuildingItemEffectLogic& handle):
	crNodeLogic(handle),
	m_effectNode(NULL),
	m_positionOffset(handle.m_positionOffset),
	m_visiable(handle.m_visiable)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crJXJMouseOnBuildingItemEffectLogic::inputParam(int i, void *param)
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
		m_this = (crInstanceItem *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_visiable = LOCREPARAM(param64);
		}
		break;
	}
}
void crJXJMouseOnBuildingItemEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	}
}
void crJXJMouseOnBuildingItemEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crJXJMouseOnBuildingItemEffectLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_effectNode = (CRCore::crNode *)param;
		break;
	}
}
void crJXJMouseOnBuildingItemEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJMouseOnBuildingItemEffectLogic::operator()(crHandle &handle)
{
	if(m_this->getRelNode())
	{
		void *param;
		crData * brainData = crBrain::getInstance()->getDataClass();
		if(brainData)
		{
			brainData->getParam(WCHDATA_JXJMouseOneBulidingNode,param);
			m_effectNode = (CRCore::crNode *)param;
		}
		if(!m_effectNode)
		{
			crVector3 position = m_this->getPosition();	
			position += m_positionOffset;
			m_playSceneFx->inputParam(3,&position);
			(*m_playSceneFx)(*this);

			brainData->inputParam(WCHDATA_JXJMouseOneBulidingNode,m_effectNode);
		}
		if(m_effectNode)
		{
			crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_effectNode);
			if(skillRangeMatrix)
			{
				m_effectNode->setVisiable(true);

				crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(m_this->getRelNode());
				if(!bot || !bot->getVisiable())
				{
					m_effectNode->setVisiable(false);
				}
				else
				{
					m_effectNode->setVisiable(true);
					crVector3 pos = bot->getTrans();
					const crBoundingBox & bbox = bot->getBoundBox();
					float fscopex =bbox.xLength();
					float fscopey =bbox.yLength()/* * crGlobalHandle::gData()->gUnitScale()*/;
					skillRangeMatrix->setMatrix(crMatrix::scale(fscopex/5.0f,fscopey/5.0f,1.0f) * crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+1.0)));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJShuiJingRangeDecalCreateLogic
//
/////////////////////////////////////////
crJXJShuiJingRangeDecalCreateLogic::crJXJShuiJingRangeDecalCreateLogic():
	m_decalNode(NULL),
	m_zoffset(1.0f)
{
}

crJXJShuiJingRangeDecalCreateLogic::crJXJShuiJingRangeDecalCreateLogic(const crJXJShuiJingRangeDecalCreateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL),
	m_zoffset(handle.m_zoffset)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJShuiJingRangeDecalCreateLogic::inputParam(int i, void *param)
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
		m_this = (crInstanceItem *)param;
		break;
	}
}

void crJXJShuiJingRangeDecalCreateLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	}
}

void crJXJShuiJingRangeDecalCreateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJShuiJingRangeDecalCreateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}
void crJXJShuiJingRangeDecalCreateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJShuiJingRangeDecalCreateLogic::operator()(crHandle &handle)
{
	if(m_this->getRelNode())
	{
		void *param;
		ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
		if(camera.valid())
		{
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			if(cameraData)
			{
				cameraData->getParam(WCHDATA_JXJMouseOnShuiJingNode,param);
				m_decalNode = (CRCore::crNode *)param;
			}
			if(!m_decalNode)
			{
				crVector3 position = m_this->getPosition();	
				position += crVector3(0.0f,0.0f,m_zoffset);
				m_playSceneFx->inputParam(3,&position);
				(*m_playSceneFx)(*this);

				cameraData->inputParam(WCHDATA_JXJMouseOnShuiJingNode,m_decalNode);
			}
			if(m_decalNode)
			{
				crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode);
				if(skillRangeMatrix)
				{
					m_decalNode->setVisiable(true);

					crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(m_this->getRelNode());
					if(!bot || !bot->getVisiable())
					{
						m_decalNode->setVisiable(false);
					}
					else
					{
						m_decalNode->setVisiable(true);
						crVector3 pos = bot->getTrans();
						const crBoundingBox & bbox = bot->getBoundBox();
						float fscopex =bbox.xLength();
						float fscopey =bbox.yLength();
						skillRangeMatrix->setMatrix(crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJJianTaRangeDecalCreateLogic
//
/////////////////////////////////////////
crJXJJianTaRangeDecalCreateLogic::crJXJJianTaRangeDecalCreateLogic():
	m_decalNode(NULL),
	m_zoffset(1.0f)
{
}

crJXJJianTaRangeDecalCreateLogic::crJXJJianTaRangeDecalCreateLogic(const crJXJJianTaRangeDecalCreateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL),
	m_zoffset(handle.m_zoffset),
	m_friendColor(handle.m_friendColor),
	m_enemyColor(handle.m_enemyColor)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJJianTaRangeDecalCreateLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem *)param;
		break;
	}
}

void crJXJJianTaRangeDecalCreateLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str, m_friendColor);
		//m_friendColor /= 255.0f;
		break;
	case 2:
		crArgumentParser::appAtoVec(str, m_enemyColor);
		//m_enemyColor /= 255.0f;
		break;
	}
}

void crJXJJianTaRangeDecalCreateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJJianTaRangeDecalCreateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}
void crJXJJianTaRangeDecalCreateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJJianTaRangeDecalCreateLogic::operator()(crHandle &handle)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(me && m_this->getRelNode() && camera.valid())
	{
		char isEnemy = 0;
		me->doEvent(WCH_EnemyCheck,MAKECREPARAM(m_this,&isEnemy));
		if(isEnemy != 0)
		{
			crVector3 offset(0.0f, 0.0f, m_zoffset);
			crVector3 position = m_this->getPosition();
			position += offset;
			int abstractid = m_this->getAbstractItemID();
			ref_ptr<crTableIO>dataAttrTab = m_this->getItemtype() == crInstanceItem::Role ? crGlobalHandle::gData()->gGlobalTable(WCHDATA_RoleDataAttrTab) : crGlobalHandle::gData()->gGlobalTable(WCHDATA_NpcDataAttrTab);
			ref_ptr<crTableIO>skilldataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
			if (dataAttrTab.valid() && skilldataAttrTab.valid())
			{
				crTableIO::StrVec record,records;
				float range = 0.0f;
				int attackid = dataAttrTab->getTitleIndex("attackid");
				if(dataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(abstractid),record)>=0)
				{
					int skillabstractid = atoi(record[attackid].c_str());
					int radiusid = skilldataAttrTab->getTitleIndex("使用半径");
					if(skilldataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(skillabstractid),records)>=0)
					{
						range = atof(records[radiusid].c_str());
						//range /= crGlobalHandle::gData()->gUnitScale();
					}
				}
				float outParam = 0.0f;
				m_this->doEvent(MAKEINT64(WCH_GetAttackDist, NULL), MAKECREPARAM(&outParam, NULL));
				range += outParam;
				crVector3 scale(range / 10.0f, range / 10.0f, 1.0f);

				void *param;
				crData *cameraData = camera->getAttachedNode()->getDataClass();
				cameraData->getParam(WCHDATA_JXJMouseOnJianTaNode, param);
				m_decalNode = (CRCore::crNode *)param;
				if (!m_decalNode)
				{
					//m_playSceneFx->inputParam(WCHDATA_AttachToNode, m_this->getRelNode());
					m_playSceneFx->inputParam(WCHDATA_AttachScale, &scale);
					m_playSceneFx->inputParam(WCHDATA_AttachOffset, &offset);
					(*m_playSceneFx)(*this);
					cameraData->inputParam(WCHDATA_JXJMouseOnJianTaNode, m_decalNode);
				}
				//else
				//{
				//	crData *decalData = m_decalNode->getDataClass();
				//	ref_ptr<crHandle> driver = decalData->getHandle(WCH_UPDATEVISITOR);
				//	ref_ptr<crHandle> method = driver->getHandle("AttachNodeUpdate");
				//	method->inputParam(5, &scale);
				//}
				if (m_decalNode)
				{
					crSetObjectColorVisitor setcolor;
					setcolor.setColor(isEnemy == 1 ? m_friendColor : m_enemyColor);
					m_decalNode->accept(setcolor);
					m_decalNode->setVisiable(m_this->getRelNode()->getVisiable());

					crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode);
					if (skillRangeMatrix)
					{
						//	m_decalNode->setVisiable(true);

						//	crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(m_this->getRelNode());
						//	if(!bot || !bot->getVisiable())
						//	{
						//		m_decalNode->setVisiable(false);
						//	}
						//	else
						//	{
						//		m_decalNode->setVisiable(true);
						//		//crVector3 pos = bot->getTrans();
						//		//const crBoundingBox & bbox = bot->getBoundBox();
						//		//float fscopex =bbox.xLength();
						//		//float fscopey =bbox.yLength();
						//		float fscope = range;//(float)range * crGlobalHandle::gData()->gUnitScale();
						skillRangeMatrix->setMatrix(crMatrix::scale(scale)*crMatrix::translate(position));
						//	}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJDownloadUISceneFileLogic
//
/////////////////////////////////////////
crJXJDownloadUISceneFileLogic::crJXJDownloadUISceneFileLogic():
	m_needDownload(false){}
crJXJDownloadUISceneFileLogic::crJXJDownloadUISceneFileLogic(const crJXJDownloadUISceneFileLogic& handle):
	crLogic(handle),
	m_needDownload(false),
	m_downloadFlg(handle.m_downloadFlg),
	m_file(handle.m_file)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crJXJDownloadUISceneFileLogic::inputParam(int i, void *param)
{
}

void crJXJDownloadUISceneFileLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_downloadFlg = atoi(str.c_str());
		break;
	case 1:
		m_file = str;
		break;
	}
}
void crJXJDownloadUISceneFileLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJDownloadUISceneFileLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crJXJDownloadUISceneFileLogic::operator()(crHandle &handle)
{
	if(!m_file.empty() && m_checkAndDownloadFileMathod.valid())
	{//检查并下载Scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		m_needDownload = false;
		bool needDownload = false;
		int rowcount;
		std::string filename;
		std::string str = crArgumentParser::getFileNameEliminateExt(m_file) + ".crb";
		ref_ptr<crTableIO> table = crTableIO::openFile(str);
		if(table.valid())
		{
			rowcount = table->getRowCount();
			for (int i = 0; i < rowcount; i++)
			{
				filename = (*table)(i,0);
				m_checkAndDownloadFileMathod->inputParam(1,&filename);
				m_checkAndDownloadFileMathod->inputParam(2,&m_downloadFlg);
				(*m_checkAndDownloadFileMathod)(*this);
				needDownload |= m_needDownload;
			}
		}
		filename = m_file;
		m_checkAndDownloadFileMathod->inputParam(1,&filename);
		m_checkAndDownloadFileMathod->inputParam(2,&m_downloadFlg);
		(*m_checkAndDownloadFileMathod)(*this);
		needDownload |= m_needDownload;

		if(!needDownload)
		{//不需要下载
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,m_downloadFlg));
		}
		else if(gamedata)
		{
			char gbuf[256];
			sprintf(gbuf,"JXJDownloadUISceneFile HttpDownload:%d\n\0",m_downloadFlg);
			gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			gamedata->startHttpDownloadTask(m_downloadFlg);
		}
	}
}
/////////////////////////////////////////
//
//crJXJNextSceneDownloadLogic
//
/////////////////////////////////////////
crJXJNextSceneDownloadLogic::crJXJNextSceneDownloadLogic():
m_needDownloadUI(false),
m_needDownload(false){}
crJXJNextSceneDownloadLogic::crJXJNextSceneDownloadLogic(const crJXJNextSceneDownloadLogic& handle):
	crLogic(handle),
	m_needDownloadUI(handle.m_needDownloadUI),
	m_extraDownloadFiles(handle.m_extraDownloadFiles),
	m_needDownload(false)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crJXJNextSceneDownloadLogic::inputParam(int i, void *param)
{
}

void crJXJNextSceneDownloadLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_needDownloadUI = atoi(str.c_str());
		break;
	default:
		m_extraDownloadFiles.push_back(str);
		break;
	}
}
void crJXJNextSceneDownloadLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crJXJNextSceneDownloadLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crJXJNextSceneDownloadLogic::operator()(crHandle &handle)
{
	crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
	ref_ptr<crTableIO>scenetable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SceneTab);
	int sceneid = 0;
	if(crRunGameHandle::getInstance()->getRunCode()>=crRunGameHandle::NetGame)
	{
		crQuerySceneInfoData *sceneInfoData = crMyPlayerData::getInstance()->getSceneInfoData();
		sceneid = sceneInfoData->getSceneID();
	}
	else
	{
		crData *data = crRunGameHandle::getInstance()->getDataClass();
		void *param;
		data->getParam(WCHDATA_SceneID,param);
		sceneid = *(short *)param;
	}
	char gbuf[256];
	sprintf(gbuf,"预下载检测,当前SceneID:%d\n\0",sceneid);
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);

	crTableIO::StrVec record,nextrec;
	bool needDownload = false; 
	if(gamedata && scenetable.valid() && scenetable->queryOneRecord(0,crArgumentParser::appItoa(sceneid),record)>=0)
	{
		int nextindex = scenetable->getTitleIndex("预下载");
		if(!record[nextindex].empty())
		{
			std::string sceneFile,uifile,bksound,filename,str;
			std::vector<int> SceneidVec;
			crArgumentParser::appAtoVec(record[nextindex],SceneidVec);
			ref_ptr<crTableIO> table;
			int rowcount;
			int downloadFlg = HttpD_None;
			for( std::vector<int>::iterator itr = SceneidVec.begin();
				itr != SceneidVec.end();
				++itr )
			{
				if(scenetable->queryOneRecord(0,crArgumentParser::appItoa(*itr),nextrec)>=0)
				{
					sceneFile = nextrec[2];
					uifile = crGlobalHandle::getInstance()->getUISceneDir()+nextrec[3];
					bksound = nextrec[7];
					str = crArgumentParser::getFileNameEliminateExt(sceneFile) + ".crb";

					table = crTableIO::openFile(str);
					if(table.valid())
					{
						rowcount = table->getRowCount();
						for (int i = 0; i < rowcount; i++)
						{
							filename = (*table)(i,0);
							m_checkAndDownloadFileMathod->inputParam(1,&filename);
							m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
							(*m_checkAndDownloadFileMathod)(*this);
							needDownload |= m_needDownload;
						}
					}
					filename = sceneFile;
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;

					if(m_needDownloadUI)
					{
						str = crArgumentParser::getFileNameEliminateExt(uifile) + ".crb";
						table = crTableIO::openFile(str);
						if(table.valid())
						{
							rowcount = table->getRowCount();
							for (int i = 0; i < rowcount; i++)
							{
								filename = (*table)(i,0);
								m_checkAndDownloadFileMathod->inputParam(1,&filename);
								m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
								(*m_checkAndDownloadFileMathod)(*this);
								needDownload |= m_needDownload;
							}
						}
						filename = uifile;
						m_checkAndDownloadFileMathod->inputParam(1,&filename);
						m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
						(*m_checkAndDownloadFileMathod)(*this);
						needDownload |= m_needDownload;
					}
					filename = bksound;
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
				}
			}
			if(needDownload)
			{
				sprintf(gbuf,"当前SceneID:%d 预下载:%s\n\0",sceneid,record[nextindex].c_str());
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
				gamedata->startHttpDownloadTask(downloadFlg);
			}
		}
	}
	std::string file;
	for( StrVec::iterator itr = m_extraDownloadFiles.begin();
		 itr != m_extraDownloadFiles.end();
		 ++itr )
	{
		file = *itr;
		crBrain::getInstance()->doEvent(WCH_DOWNLOAD,MAKECREPARAM(&file,NULL));
	}
}
/////////////////////////////////////////
//
//crJXJExtraShootEffectLogic
//
/////////////////////////////////////////
crJXJExtraShootEffectLogic::crJXJExtraShootEffectLogic()
{
}

crJXJExtraShootEffectLogic::crJXJExtraShootEffectLogic(const crJXJExtraShootEffectLogic& handle):
	crLogic(handle),
	m_closeToTargetCallback(handle.m_closeToTargetCallback),
	m_param(handle.m_param)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJExtraShootEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case WCHDATA_FireItem:
		m_fireItem = (crInstanceItem*)param;
		break;
	}
}

void crJXJExtraShootEffectLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_closeToTargetCallback = str;
		break;
	case 1:
		m_param = str;
		break;
	default:
		break;
	}
}

void crJXJExtraShootEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crHandle *)param;
		break;
	}
}

void crJXJExtraShootEffectLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_fxNode = param?((crNode *)(param)):NULL;
		break;
	}
}
void crJXJExtraShootEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
	if(m_callback.valid())
		m_callback->releaseObjects(state);

	m_fxNode = NULL;
}
void crJXJExtraShootEffectLogic::operator()(crHandle &handle)
{
	if(!m_playSceneFx.valid()||!m_fireItem.valid()) return;
	if(!m_fxNode.valid())
	{
		crVector3 pos = m_fireItem->getRelNode()->getBound().center();
		m_playSceneFx->inputParam(3,&pos);
		(*m_playSceneFx)(*this);

		if(m_fxNode.valid())
		{//从fireitem位置向我的位置飞行
			m_callback = m_fxNode->getUpdateCallback(m_closeToTargetCallback);
			if(!m_callback.valid())
			{
				m_callback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(m_closeToTargetCallback));
				m_fxNode->setUpdateCallback(m_callback.get());
			}
			crJXJCloseToTargetNodeCallback *callback = dynamic_cast<crJXJCloseToTargetNodeCallback *>(m_callback.get());
			callback->setTargetNode(dynamic_cast<crMatrixTransform *>(m_this->getRelNode()));
			callback->addEventParam(0,m_param);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCampAddArmyEffectLogic
//
/////////////////////////////////////////
crJXJCampAddArmyEffectLogic::crJXJCampAddArmyEffectLogic() :
m_effectNode(NULL),
m_visiable(true){}
crJXJCampAddArmyEffectLogic::crJXJCampAddArmyEffectLogic(const crJXJCampAddArmyEffectLogic& handle) :
crNodeLogic(handle),
m_effectNode(NULL),
m_positionOffset(handle.m_positionOffset),
m_visiable(handle.m_visiable)
{
	if (handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crJXJCampAddArmyEffectLogic::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem *)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_visiable = LOCREPARAM(param64);
		}
		break;
	}
}
void crJXJCampAddArmyEffectLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str, m_positionOffset);
		break;
	}
}
void crJXJCampAddArmyEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crJXJCampAddArmyEffectLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_effectNode = (CRCore::crNode *)param;
		break;
	}
}
void crJXJCampAddArmyEffectLogic::releaseObjects(CRCore::crState* state)
{
	if (m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJCampAddArmyEffectLogic::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if (m_this->getRelNode() && camera.valid())
	{
		void *param;
		char isEnemy = 0;
		crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck, MAKECREPARAM(m_this, &isEnemy));

		crData * cameraData = camera->getAttachedNode()->getDataClass();

		if (cameraData)
		{
			cameraData->getParam(WCHDATA_JXJCampAddArmyTimer, param);
			float addArmyTimer = *(float *)param;

			if (!m_effectNode)
			{
				crVector3 position = m_this->getPosition();
				position += m_positionOffset;
				m_playSceneFx->inputParam(3, &position);
				(*m_playSceneFx)(*this);

				//if (m_effectNode)
				{
					crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_effectNode);
					if (skillRangeMatrix)
					{
						crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(m_this->getRelNode());
						if (!bot || !bot->getVisiable())
						{
							m_effectNode->setVisiable(false);
						}
						else
						{
							crMatrix mat = bot->getMatrix();
							crVector3 pos = bot->getTrans();
							const crBoundingBox & bbox = bot->getBoundBox();
							float fscopex = bbox.xLength();
							float fscopey = bbox.yLength()/* * crGlobalHandle::gData()->gUnitScale()*/;

							crMatrix scaleMatrix = crMatrix::scale(fscopex / 5.0f, fscopey / 5.0f, 1.0f);

							mat.setTrans(0.0f, 0.0f, 0.0f);
							crVector3 dir = (-Y_AXIS * mat).normalize();
							crVector3 trans = bot->getTrans();
							crMatrix put_mat = crMatrix::translate(trans);
							//crMatrix inv_put = crMatrix::inverse(put_mat);
							crMatrix rot_mat = crMatrix::rotate(-Y_AXIS, dir);
							crMatrix inv_rot = crMatrix::inverse(rot_mat);
							//mat.postMult(inv_put);
							mat.postMult(inv_rot);
							mat.postMult(scaleMatrix);
							mat.postMult(rot_mat);
							mat.postMult(put_mat);


							m_effectNode->setVisiable(true);
							
							skillRangeMatrix->setMatrix(mat);
						}
					}
				}
			}
			if (m_effectNode)
			{
				if (isEnemy!=1 || addArmyTimer > 0.0f)
				{
					m_effectNode->setVisiable(false);
				}
				else
				{
					m_effectNode->setVisiable(true);
				}
			}
		}
// 		crData * brainData = crBrain::getInstance()->getDataClass();
// 		if (brainData)
// 		{
// 			brainData->getParam(WCHDATA_JXJMouseOneBulidingNode, param);
// 			m_effectNode = (CRCore::crNode *)param;
// 		}
		
	}
}
/////////////////////////////////////////
//
//crJXJBulletCollideFxLogic
//
/////////////////////////////////////////
crJXJBulletCollideFxLogic::crJXJBulletCollideFxLogic(){}
crJXJBulletCollideFxLogic::crJXJBulletCollideFxLogic(const crJXJBulletCollideFxLogic& handle):
	crNodeLogic(handle)
{
	CRCore::ref_ptr<CRCore::crNodeLogic> fx;
	for( LogicArray::const_iterator itr = handle.m_playSceneFxArray.begin();
		 itr != handle.m_playSceneFxArray.end();
		 ++itr )
	{
		fx = const_cast<crNodeLogic *>(itr->get());
		if(fx.valid())
		{
			m_playSceneFxArray.push_back(dynamic_cast<crNodeLogic *>(fx->clone()));
		}
	}
}
void crJXJBulletCollideFxLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 3:
		m_collidePos = param?*(crVector3*)param:crVector3();
		break;
	case WCHDATA_FXItem:
		m_hititem = (crInstanceItem *)param;
		break;
	}
}
void crJXJBulletCollideFxLogic::addParam(int i, const std::string& str)
{
}
void crJXJBulletCollideFxLogic::inputHandle(int i, void *param)
{
	if(param)
		m_playSceneFxArray.push_back((CRCore::crNodeLogic *)param);
}
void crJXJBulletCollideFxLogic::outputParam(int i, void *param)
{
}
void crJXJBulletCollideFxLogic::operator()(crHandle &handle)
{
	int armytype = -1;
	if(m_hititem.valid() && m_hititem->getDataClass())
	{
		crData *hitdata = m_hititem->getDataClass();
		if(hitdata)
		{
			void *param;
			hitdata->getParam(WCHDATA_JXJTroopType,param);
			if(param)
			{
				armytype = *(unsigned char *)param;
			}
		}
	}
	if(armytype>=0 && armytype<m_playSceneFxArray.size())
	{
		CRCore::ref_ptr<CRCore::crNodeLogic> fx = m_playSceneFxArray[armytype];
		if(fx.valid())
		{
			fx->inputParam(3,&m_collidePos);
			(*fx)(*this);
		}
	}
}
void crJXJBulletCollideFxLogic::releaseObjects(CRCore::crState* state)
{
	for( LogicArray::iterator itr = m_playSceneFxArray.begin();
		itr != m_playSceneFxArray.end();
		++itr )
	{
		if(itr->valid())
		{
			(*itr)->releaseObjects(state);
		}
	}
}
///////////////////////////////////////////
////
////crJXJExtraCorpseCountLogic
////
///////////////////////////////////////////
//crJXJExtraCorpseCountLogic::crJXJExtraCorpseCountLogic():
//	m_dt(NULL),
//	m_duration(0.0f),
//	m_range(0.0f),
//	m_haloInterval(5.0f),
//	m_corpseCount(0)
//{
//}
//
//crJXJExtraCorpseCountLogic::crJXJExtraCorpseCountLogic(const crJXJExtraCorpseCountLogic& handle):
//	crLogic(handle),
//	m_dt(NULL),
//	m_duration(handle.m_duration),
//	m_range(handle.m_range),
//	m_haloInterval(handle.m_haloInterval),
//	m_corpseCount(0)
//{
//	for( HandleVec::const_iterator itr = handle.m_handles.begin(); 
//		itr != handle.m_handles.end();
//		++itr )
//	{
//		m_handles.push_back((*itr)->clone());
//	}
//}
//
//void crJXJExtraCorpseCountLogic::inputParam(int i, void *param)
//{
//	for( HandleVec::iterator itr = m_handles.begin(); 
//		itr != m_handles.end();
//		++itr )
//	{
//		(*itr)->inputParam(i,param);
//	}
//	switch(i) 
//	{
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_dt = (float*)(LOCREPARAM(param64));
//		}
//		else
//		{
//			m_dt = NULL;
//		}
//		break;
//	case WCHDATA_FireItem:
//		m_fireItem = (crInstanceItem*)param;
//		break;
//	}
//}
//
//void crJXJExtraCorpseCountLogic::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_duration = atof(str.c_str());
//		break;
//	case 1:
//		m_range = atof(str.c_str());
//		break;
//	case 2:
//		m_haloInterval = atof(str.c_str());
//		break;
//	}
//}
//
//void crJXJExtraCorpseCountLogic::inputHandle(int i, void *param)
//{
//	if(param)
//	{
//		m_handles.push_back((CRCore::crHandle *)param);
//	}
//}
//
//void crJXJExtraCorpseCountLogic::outputParam(int i, void *param)
//{
//}
//
//void crJXJExtraCorpseCountLogic::operator()(crHandle &handle)
//{
//	if(!m_start)
//	{
//		m_start = true;
//		m_timer = m_duration;
//		m_dtrec = m_haloInterval;
//	}
//	if(m_timer>=0.0f)
//	{
//		if(m_duration>0.0f) m_timer -= *m_dt;
//		m_dtrec += *m_dt;
//		if(m_dtrec>=m_haloInterval)
//		{//光环每隔m_haloIntervals对外作用一次
//			m_dtrec = 0.0f;
//			short corpseCount = 0;
//			crVector3 hitpos = m_this->getPosition();
//			crVector3 itempos;
//			ref_ptr<crInstanceItem> item;
//			unsigned int guisestate = GS_Normal;
//			unsigned char itemstate;
//			ref_ptr<crData>itemdata;
//			void *param;
//			if(crGlobalHandle::isClient())
//			{
//				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//				crMyPlayerData::getInstance()->lockMyRoleNpcMap();
//				crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
//				for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
//					itr != myRoles.end();
//					++itr )
//				{
//					item = itr->second.first.get();
//					itemdata = item->getDataClass();
//					if(m_this != item.get() && itemdata.valid())
//					{
//						guisestate = GS_Normal;
//						item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//						if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//						{
//							continue;
//						}
//						itemdata->getParam(WCHDATA_ItemState,param);
//						itemstate = *(unsigned char *)param;
//						if(itemstate == IS_Dead)
//						{
//							itempos = item->getPosition();
//							if((itempos - hitpos).length() < m_range)
//							{
//								corpseCount++;
//							}
//						}
//					}
//				}
//				crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
//
//				crMyPlayerData::getInstance()->lockInRangePlayerMap();
//				crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
//				for( crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
//					itr != playerMap.end();
//					++itr )
//				{
//					item = itr->second.first.get();
//					itemdata = item->getDataClass();
//					if(itemdata.valid())
//					{
//						guisestate = GS_Normal;
//						item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//						if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//						{
//							continue;
//						}
//						itemdata->getParam(WCHDATA_ItemState,param);
//						itemstate = *(unsigned char *)param;
//						if(itemstate == IS_Dead)
//						{
//							itempos = item->getPosition();
//							if((itempos - hitpos).length() < m_range)
//							{
//								corpseCount++;
//							}
//						}
//					}
//				}
//				crMyPlayerData::getInstance()->unlockInRangePlayerMap();
//
//				crMyPlayerData::getInstance()->lockInRangeNpcMap();
//				crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
//				for( crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
//					itr != npcMap.end();
//					++itr )
//				{
//					item = itr->second.first.get();
//					itemdata = item->getDataClass();
//					if(itemdata.valid())
//					{
//						guisestate = GS_Normal;
//						item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//						if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//						{
//							continue;
//						}
//						itemdata->getParam(WCHDATA_ItemState,param);
//						itemstate = *(unsigned char *)param;
//						if(itemstate == IS_Dead)
//						{
//							itempos = item->getPosition();
//							if((itempos - hitpos).length() < m_range)
//							{
//								corpseCount++;
//							}
//						}
//					}
//				}
//				crMyPlayerData::getInstance()->unlockInRangeNpcMap();
//
//			}
//			else
//			{
//				ref_ptr<crSightInfo> hitSight = m_this->getSightInfo();
//				if(hitSight.valid() && m_range>0.0f)
//				{
//					hitSight->lockItemEyeMap();
//					crSightInfo::ItemEyeMap &itemEyeMap = hitSight->getItemEyeMap();
//					for( crSightInfo::ItemEyeMap::iterator itr = itemEyeMap.begin();
//						itr != itemEyeMap.end();
//						++itr )
//					{
//						item = itr->second;
//						itemdata = item->getDataClass();
//						if(m_this != item.get() && itemdata.valid())
//						{
//							guisestate = GS_Normal;
//							item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//							if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//							{
//								continue;
//							}
//							itemdata->getParam(WCHDATA_ItemState,param);
//							itemstate = *(unsigned char *)param;
//							if(itemstate == IS_Dead)
//							{
//								itempos = item->getPosition();
//								if((itempos - hitpos).length() < m_range)
//								{
//									corpseCount++;
//								}
//							}
//						}
//					}
//					hitSight->unlockItemEyeMap();
//
//					ref_ptr<crSceneServerPlayerData> playerdata;
//					hitSight->lockPlayerEyeMap();
//					crSightInfo::PlayerEyeMap &playerEyeMap = hitSight->getPlayerEyeMap();
//					for( crSightInfo::PlayerEyeMap::iterator itr = playerEyeMap.begin();
//						itr != playerEyeMap.end();
//						++itr )
//					{
//						playerdata = itr->second;
//						crSceneServerPlayerData::RoleMap &roleMap = playerdata->getRoleMap();
//						for( crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
//							ritr != roleMap.end();
//							++ritr )
//						{
//							item = dynamic_cast<crInstanceItem *>(ritr->second.get());
//							itemdata = item->getDataClass();
//							if(m_this != item.get() && itemdata.valid())
//							{
//								guisestate = GS_Normal;
//								item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//								if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//								{
//									continue;
//								}
//								itemdata->getParam(WCHDATA_ItemState,param);
//								itemstate = *(unsigned char *)param;
//								if(itemstate == IS_Dead)
//								{
//									itempos = item->getPosition();
//									if((itempos - hitpos).length() < m_range)
//									{
//										corpseCount++;
//									}
//								}
//							}
//						}
//					}
//					hitSight->unlockPlayerEyeMap();
//
//					hitSight->lockInSightItemMap();
//					crSightInfo::ItemMap &itemMap = hitSight->getInSightItemMap();
//					for( crSightInfo::ItemMap::iterator itr = itemMap.begin();
//						itr != itemMap.end();
//						++itr )
//					{
//						item = itr->second;
//						guisestate = GS_Normal;
//						item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//						if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//						{
//							continue;
//						}
//
//						itemdata = item->getDataClass();
//						if(itemdata.valid())
//						{
//							itemdata->getParam(WCHDATA_ItemState,param);
//							itemstate = *(unsigned char *)param;
//							if(itemstate == IS_Dead)
//							{
//								itempos = item->getPosition();
//								if((itempos - hitpos).length() < m_range)
//								{
//									corpseCount++;
//								}
//							}
//						}
//					}
//					hitSight->unlockInSightItemMap();
//
//					hitSight->lockInSightRolePlayerMap();
//					crSightInfo::RolePlayerMap &rolePlayerMap = hitSight->getInSightRolePlayerMap();
//					for( crSightInfo::RolePlayerMap::iterator itr = rolePlayerMap.begin();
//						itr != rolePlayerMap.end();
//						++itr )
//					{
//						item = dynamic_cast<crInstanceItem*>(itr->second.get());
//						guisestate = GS_Normal;
//						item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
//						if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable )
//						{
//							continue;
//						}
//
//						itemdata = item->getDataClass();
//						if(itemdata.valid())
//						{
//							itemdata->getParam(WCHDATA_ItemState,param);
//							itemstate = *(unsigned char *)param;
//							if(itemstate == IS_Dead)
//							{
//								itempos = item->getPosition();
//								if((itempos - hitpos).length() < m_range)
//								{
//									corpseCount++;
//								}
//							}
//						}
//					}
//					hitSight->unlockInSightRolePlayerMap();
//				}
//			}
//			m_corpseCount = corpseCount;
//		}
//		if(m_corpseCount>0)
//		{
//			for( HandleVec::iterator itr = m_handles.begin(); 
//				itr != m_handles.end();
//				++itr )
//			{
//				(*itr)->inputParam(WCHDATA_JXJOverlapParam,&m_corpseCount);
//				(*(itr->get()))(handle);
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJExtraOverlapHaloLogic
//
/////////////////////////////////////////
crJXJExtraOverlapHaloLogic::crJXJExtraOverlapHaloLogic():
	m_dt(NULL),
	m_duration(0.0f),
	m_haloInterval(5.0f),
	m_overlapCount(0),
	m_maxOverlapCount(-1),
	m_start(false),
	m_timer(0.0f),m_dtrec(0.0f),
	m_extraid(0)
{
}

crJXJExtraOverlapHaloLogic::crJXJExtraOverlapHaloLogic(const crJXJExtraOverlapHaloLogic& handle):
	crLogic(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_haloInterval(handle.m_haloInterval),
	m_maxOverlapCount(handle.m_maxOverlapCount),
	m_overlapCount(0),
	m_start(false),
	m_timer(0.0f), m_dtrec(0.0f), m_extraid(0)
{
	if(handle.m_overlapCountHandle.valid())
		m_overlapCountHandle = handle.m_overlapCountHandle->clone();
	for( HandleVec::const_iterator itr = handle.m_handles.begin(); 
		itr != handle.m_handles.end();
		++itr )
	{
		m_handles.push_back((*itr)->clone());
	}
}

void crJXJExtraOverlapHaloLogic::inputParam(int i, void *param)
{
	if(m_overlapCountHandle.valid())
		m_overlapCountHandle->inputParam(i,param);
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		(*itr)->inputParam(i,param);
	}
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	case 4:
		if(param)
			m_extraid = *(short *)param;
		break;
	}
}

void crJXJExtraOverlapHaloLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_haloInterval = atof(str.c_str());
		break;
	case 2:
		m_maxOverlapCount = atoi(str.c_str());
		break;
	}
}

void crJXJExtraOverlapHaloLogic::inputHandle(int i, void *param)
{
	if(i == 0)
	{
		m_overlapCountHandle = (CRCore::crHandle *)param;
	}
	else if(param)
	{
		m_handles.push_back((CRCore::crHandle *)param);
	}
}

void crJXJExtraOverlapHaloLogic::outputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_overlapCount = *(unsigned char*)param;
		break;
	}
}

void crJXJExtraOverlapHaloLogic::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		//m_overlapCount = -1;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_dtrec>=m_haloInterval)
		{//光环每隔m_haloIntervals对外作用一次
			m_dtrec -= m_haloInterval;
			if (m_overlapCountHandle.valid() && strcmp(m_overlapCountHandle->_name(),"Null")!=0)
			{
				(*(m_overlapCountHandle))(*this);
			}
			else
			{
				if(m_maxOverlapCount<0 || m_overlapCount<m_maxOverlapCount)
					m_overlapCount++;
			}
			void *param;
			ref_ptr<crData> data = m_this->getDataClass();
			data->getParam(WCHDATA_ExtraData, param);
			if (param)
			{
				ref_ptr<crData> extraData = (crData *)param;
				extraData->excHandle(MAKECREPARAM(WCH_LockData, 1));
				extraData->getParam(WCHDATA_ExtraIDMap, param);
				ExtraIDMap *extraIDMap = (ExtraIDMap *)param;
				ExtraIDMap::iterator itr = extraIDMap->find(m_extraid);
				if (itr != extraIDMap->end())
				{
					itr->second = MAKEINT16(m_overlapCount, HIINT16(itr->second));
				}
				extraData->excHandle(MAKECREPARAM(WCH_LockData, 0));
			}
		}
		if (m_overlapCount > 0)
		{
			for (HandleVec::iterator itr = m_handles.begin();
				itr != m_handles.end();
				++itr)
			{
				(*itr)->inputParam(WCHDATA_JXJOverlapParam, &m_overlapCount);
				(*(itr->get()))(*this);
			}
		}
	}
}