/**********************************************************************
*
*	文件:	   appMethod8.cpp
*
*	描述:	   
*
*	作者:	   wxd
*					  
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod8.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <CRUI/crWidgetExtend.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNodeCallbacks.h>
#include <CREncapsulation/crGameDices.h>
#include <stdlib.h>
#include <JXJ/appDBQuery.h>

using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace rbody;
using namespace JXJ;
using namespace CRIOManager;
using namespace CRUI;

///////////////////////////////////////////
////
////crJXJBossMarkMethod
////
///////////////////////////////////////////
//crJXJBossMarkMethod::crJXJBossMarkMethod() :
//	m_this(NULL)
//{
//}
//crJXJBossMarkMethod::crJXJBossMarkMethod(const crJXJBossMarkMethod& handle) :
//	crMethod(handle),
//	m_this(NULL)
//{
//}
//void crJXJBossMarkMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crCanvasNode*)param;
//		break;
//	}
//}
//void crJXJBossMarkMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{ 
//	case 0:
//		m_strCanvas = str;
//		break;
//	case 1:
//		m_strMiniMap = str;
//		break;
//	case 2:
//		m_strRotMap = str;
//		break;
//	case 3:
//		m_strBossIcon = str;
//		break;
//	}
//}
//void crJXJBossMarkMethod::operator()(crHandle &handle)
//{
//	if(!crRunGameHandle::getInstance()->isInGame())
//		return;
//	// 1. 取小地图
//	crImageBoxWidgetNode* mapWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strMiniMap));
//	if (!mapWidget)
//		return;
//
//	const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
//	const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
//
//	crVector2 mapWidgetSize;
//	const crBoundingBox &bbox = mapWidget->getBoundBox();
//	mapWidgetSize.set(bbox.xLength(),bbox.yLength());
//	crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);
//	// 2.
//	// 3.
//	crWidgetNode *rotMapWidget = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_strRotMap));
//	if (!rotMapWidget)
//		return;
//	// 4. 取canvas
//	crCanvasNode *mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();	
//	if (!mainCanvas) 
//		return;
//	// 5. 取boss的图标
//	crWidgetNode *bossWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_strBossIcon));
//	if (!bossWidget)
//		return;
//	// 6. 小地图和boss图标
////	crLoadManager::getInstance()->requestAddNode(mapWidget, bossWidget, false);
//	// 7. 取boss的位置
//	float px, py;
//	{
//		crPlayerGameData* playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//		if (!playerData) 
//			return;
//
//		crData *data = playerData->getDataClass();
//		if (!data) 
//			return;
//
//		void *param;
//		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
//		unsigned char type = *(unsigned char *)param;
//		if(type == FT_Fuben)
//		{
//			data->getParam(WCHDATA_JXJFubenOrChengchiID, param);
//			unsigned short fubenid = *(unsigned short*)param;
//
//			crTableIO::StrVec record;
//			ref_ptr<crTableIO>  fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenMarkTab);
//			if (fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid), record)<0) 
//				return;
//
//			int xIndex = fubenTab->getTitleIndex("x");
//			int yIndex = fubenTab->getTitleIndex("y");
//			int mapIndex = fubenTab->getTitleIndex("icon");
//			if (xIndex < 0 || yIndex < 0 || mapIndex < 0) 
//				return;
//
//			px = atof(record[xIndex].c_str());
//			py = atof(record[yIndex].c_str());
//		}
//		else
//		{
//			return;
//		}
//	}
//	crLoadManager::getInstance()->requestAddNode(rotMapWidget, bossWidget, false);
//
//	px = px*crGlobalHandle::gData()->gUnitScale();// - myX;
//	py = py*crGlobalHandle::gData()->gUnitScale();// - myY;
//	px *= mapFactor[0];
//	py *= mapFactor[1];
//
//	crVector3 pos(px, py, 0.0f);
//
//	crQuat quat = rotMapWidget->getMatrix().getRotate();
//	crMatrixf mtrx = crMatrix::rotate(quat.inverse());
//
//	// 8. 显示boss图标
//	bossWidget->setMatrix(invScaleMat * mtrx * scaleMat * crMatrix::translate(mapWidget->getBound().center() + pos));
////	bossWidget->setMatrix(invScaleMat * scaleMat * crMatrix::translate(mapWidget->getBound().center() + pos));
//	bossWidget->setVisiable(true);
//}
/////////////////////////////////////////
//
//crJXJNotInSight2Logic
//
/////////////////////////////////////////
crJXJNotInSight2Logic::crJXJNotInSight2Logic() :
	m_targetPosition(crVector3(0.0f,0.0f,0.0f))
{
}
crJXJNotInSight2Logic::crJXJNotInSight2Logic(const crJXJNotInSight2Logic& handle) :
	crNodeLogic(handle),
	m_targetPosition(handle.m_targetPosition)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crJXJNotInSight2Logic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_param = NULL;
			m_targetNode = NULL;
			if(m_playSceneFx.valid())
				m_playSceneFx->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if(param)
		{
			m_param = *(_crInt64*)param;
			m_targetPosition = *(crVector3 *)(LOINT64(m_param));
		}
		else
		{
			m_targetPosition = crVector3(0.0f,0.0f,0.0f);
			m_param = NULL;
		}
		break;
	}
}
void crJXJNotInSight2Logic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_fileName = str;
		break;
	default:
		break;
	}
}
void crJXJNotInSight2Logic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crJXJNotInSight2Logic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_TargetNode:
		m_targetNode = param?((crViewMatterObject *)(param)):NULL;
		break;
	}
}
void crJXJNotInSight2Logic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJNotInSight2Logic::operator()(crHandle &handle)
{
	if(!m_playSceneFx.valid() && m_targetPosition == crVector3(0.0f,0.0f,0.0f)) return;

	m_targetPosition *= crGlobalHandle::gData()->gUnitScale();
	m_playSceneFx->inputParam(3,&m_targetPosition);

	// 			crVector3 camPos = m_this->getTrans();
	// 			crVector3 dir = camPos - m_targetPosition;
	// 			dir[2] = 0.0f;
	// 			dir.normalize();
	// 			m_playSceneFx->inputParam(4,&dir);
	m_playSceneFx->inputParam(5,&m_fileName);
	(*m_playSceneFx)(*this);
}
/////////////////////////////////////////
//
//crJXJNotInSightMethod
//
/////////////////////////////////////////
crJXJNotInSightMethod::crJXJNotInSightMethod():
	m_netType(GameClient_Game),
	m_invertal(2.0f),
	m_countOfWidgets(5),
	m_this(NULL)
{}
crJXJNotInSightMethod::crJXJNotInSightMethod(const crJXJNotInSightMethod& handle):
	crMethod(handle),
	m_this(handle.m_this),
	m_netType(GameClient_Game),
	m_invertal(handle.m_invertal),
	m_countOfWidgets(handle.m_countOfWidgets),
	m_strCanvas(handle.m_strCanvas),
	m_strMiniMap(handle.m_strMiniMap),
	m_strRotMap(handle.m_strRotMap),
	m_strIcon(handle.m_strIcon)
{
}
void crJXJNotInSightMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		//m_this = (crPlayerGameData*)param;
		m_this = (crCanvasNode*)param; 
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJNotInSightMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strMiniMap = str;
		break;
	case 2:
		m_strRotMap = str;
		break;
	case 3:
		m_strIcon = str;
		break;
	case 4:
		m_invertal = atof(str.c_str());
		break;
	case 5:
		m_countOfWidgets = atoi(str.c_str());
		break;
	}
}
void crJXJNotInSightMethod::displayIcon(const float posx, const float posy, crCanvasNode* canvas, crImageBoxWidgetNode* mapWidget, crWidgetNode* rotMapWidget, crWidgetNode* widget)
{
	const crBoundingBox &bbox = mapWidget->getBoundBox();
	crVector2 mapWidgetSize;
	mapWidgetSize.set(bbox.xLength(),bbox.yLength());
	crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);

	crVector3 pos(posx, posy, 0);

	pos.x() = pos.x() * crGlobalHandle::gData()->gUnitScale() * mapFactor[0];
	pos.y() = pos.y() * crGlobalHandle::gData()->gUnitScale() * mapFactor[1];

	//pos = pos * rotMapWidget->getMatrix();

	widget->setVisiable(true);
	const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
	const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
	widget->setMatrix(scaleMat * invScaleMat * crMatrix::translate(bbox.center() + pos));
}
void crJXJNotInSightMethod::operator()(crHandle &handle)
{
	//m_strIcon = "UI_CopyBattle_BossIcon_Big";
	if(m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game &&crRunGameHandle::getInstance()->isInBattle())
		{
			crVector3 pos = m_stream->_readVec3();
			int npcid = m_stream->_readInt();

			ref_ptr<crCamera>camera1 = crCameraManager::getInstance()->getMainCamera();
			if (!camera1.valid())
				return;
			camera1->getAttachedNode()->doEvent(WCH_JXJNotInSight2MsgData, MAKEINT64(&pos,NULL));

			crCanvasNode *mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();	
			crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if(!mainCanvas || !canvas) 
			{
				return;
			}

			crWidgetNode *bossWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_strIcon));
			crWidgetNode *rotMapWidget = canvas->getWidget(m_strRotMap);
			if(!bossWidget || !rotMapWidget)
			{
				return;
			}
			
			crImageBoxWidgetNode* mapWidget = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_strMiniMap));
			if (!mapWidget)
			{
				return;
			}
			CRCore::crNode *camera = camera1->getAttachedNode();
			if(!camera) 
			{
				return;
			}
			
			crData* cameraData = camera->getDataClass();
			CRCore::ScopedLock<crData> lock(*cameraData);
 
			do
			{
				bool ret = false;
				// 取 SceneNodeMap
				void* param;

				cameraData->getParam(WCHDATA_JXJAttackIconWidgetMap, param);
				SceneNodesVec* sceneNodes = (SceneNodesVec*)param;
				// 找出 npcid的node
				for (int i = 0; i < sceneNodes->size(); ++i)
				{
					if (sceneNodes->at(i).m_id == npcid && sceneNodes->at(i).m_isEnable)
					{
						if (sceneNodes->at(i).m_widget.valid())
						{
							sceneNodes->at(i).m_deltaTime = m_invertal;
							displayIcon(pos.x(), pos.y(), canvas, mapWidget, rotMapWidget, sceneNodes->at(i).m_widget.get());
						}
						ret = true;
						break;
					}
				}
				if (ret)
					break;

				// 找出一个false的node
				int i = 0;
				int shortestNodeIndex = 0;
				float myInterval = 1000.0f;
				for (; i < sceneNodes->size(); ++i)
				{
					if (sceneNodes->at(i).m_isEnable == false)
					{
						if(sceneNodes->at(i).m_widget.valid())
						{
							// 插入 icon widget 到 SceneNodeMap
							sceneNodes->at(i).m_deltaTime = m_invertal;
							sceneNodes->at(i).m_isEnable = true;
							sceneNodes->at(i).m_id = npcid;
							// 显示图标
							displayIcon(pos.x(), pos.y(), canvas, mapWidget, rotMapWidget, sceneNodes->at(i).m_widget.get());
						}
						ret = true;
						break;
					}
					else
					{
						if (sceneNodes->at(i).m_deltaTime <= myInterval)
						{
							shortestNodeIndex = i;
							myInterval = sceneNodes->at(i).m_deltaTime;
						}
					}
				}
				if (ret)
					break;

				// i >= countOfWidgets, 找出实际最短的node
				if (i >= m_countOfWidgets)
				{
					sceneNodes->at(shortestNodeIndex).m_deltaTime = m_invertal;
					sceneNodes->at(shortestNodeIndex).m_isEnable = true;
					sceneNodes->at(shortestNodeIndex).m_id = npcid;
					// 显示图标
					displayIcon(pos.x(), pos.y(), canvas, mapWidget, rotMapWidget, sceneNodes->at(shortestNodeIndex).m_widget.get());
					break;
				}
				
				// 增加一个node
				NotInSightNode node;
				node.m_deltaTime = m_invertal;
				node.m_id = npcid;
				node.m_isEnable = true;
				node.m_widget = dynamic_cast<crWidgetNode *>(bossWidget->clone(crCopyOp::DEEP_COPY_NODES));
				if (node.m_widget.valid())
				{
					// 显示图标
					crLoadManager::getInstance()->requestAddNode(canvas, node.m_widget.get(), false);
					displayIcon(pos.x(), pos.y(), canvas, mapWidget, rotMapWidget, node.m_widget.get());
					sceneNodes->push_back(node);
				}
			}
			while(0);
		}
	}
}
/////////////////////////////////////////
//
//crJXJNotInSightUpdateMethod
//
/////////////////////////////////////////
crJXJNotInSightUpdateMethod::crJXJNotInSightUpdateMethod():
	m_dt(NULL)
{}
crJXJNotInSightUpdateMethod::crJXJNotInSightUpdateMethod(const crJXJNotInSightUpdateMethod& handle):
	crMethod(handle),
	m_dt(NULL)
{
}
void crJXJNotInSightUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJNotInSightUpdateMethod::addParam(int i, const std::string& str)
{
}
void crJXJNotInSightUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *cameraData = m_this->getDataClass();
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJAttackIconWidgetMap, param);
			SceneNodesVec* iconWidgetNodeMap = (SceneNodesVec*)param;
			if(iconWidgetNodeMap)
			{
				for (int i = 0; i < iconWidgetNodeMap->size(); ++i)
				{
					if (iconWidgetNodeMap->at(i).m_isEnable)
					{
						if (iconWidgetNodeMap->at(i).m_deltaTime < 0.0f)
						{
							iconWidgetNodeMap->at(i).m_id = -1;
							iconWidgetNodeMap->at(i).m_isEnable = false;
							iconWidgetNodeMap->at(i).m_deltaTime = 0.0f;
							if (iconWidgetNodeMap->at(i).m_widget.valid())
							{
								iconWidgetNodeMap->at(i).m_widget->setVisiable(false);
							}
						}
						else
						{
							iconWidgetNodeMap->at(i).m_deltaTime -= *m_dt;
						}
					}
				}
			}
		}
	} 
}
/////////////////////////////////////////
//
//crJXJVIPRevenuesBonusMethod
//
/////////////////////////////////////////
crJXJVIPRevenuesBonusMethod::crJXJVIPRevenuesBonusMethod() :
	m_this(NULL),
	m_cashPtr(NULL),
	m_cash1Ptr(NULL)
{
}
crJXJVIPRevenuesBonusMethod::crJXJVIPRevenuesBonusMethod(const crJXJVIPRevenuesBonusMethod& handle) :
	crMethod(handle),
	m_cashPtr(handle.m_cashPtr),
	m_cash1Ptr(handle.m_cash1Ptr)
{
}
void crJXJVIPRevenuesBonusMethod::inputParam(int i, void* param)
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
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_cashPtr = (int*)(HIINT64(param64));
			m_cash1Ptr = (int*)(LOINT64(param64));
		}
		break;
	}
}
void crJXJVIPRevenuesBonusMethod::addParam(int i, const std::string& str)
{
}
void crJXJVIPRevenuesBonusMethod::operator()(crHandle& handle)
{
	crData* data = m_this->getDataClass();

	void* param;

	// 1. 取征收倍数
	data->getParam(WCHDATA_JXJVIPZhengShouXinDe, param);
	unsigned char increaseLev = *((unsigned char*)param);

	// 2. 取可能性
	data->getParam(WCHDATA_JXJVIPGuLiMaoYi, param);
	unsigned char randomLev = *((unsigned char*)param);

	// 3. 打开VIPAffairsLev
	crTableIO::DataVec recordVec;
	ref_ptr<crTableIO>  vipLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);
			
	// 4. 取征收倍数
	unsigned int increase;
	unsigned int random;
	int lvIndex = vipLvTab->getTitleIndex("lv");
	int addIndex = vipLvTab->getTitleIndex("add");

	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPZhengShouXinDe), recordVec);
	bool ret = false;
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == increaseLev)
		{
			increase = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	// 5. 取可能性 
	ret = false;
	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPGuLiMaoYi), recordVec);
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == randomLev)
		{
			random = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	CRCore::rangei r(0, 1000);
	int dice = r.get_random();
	if (dice < random)
	{
		if (m_cashPtr)
		{
			*m_cash1Ptr += (-(int)((float)(*m_cashPtr) * (float)increase * 0.001f));
		}
		else
		{
			*m_cash1Ptr = (int)((float)(*m_cash1Ptr) * (1.0f + (float)increase * 0.001f));
		}
	}
}
/////////////////////////////////////////
//
//crJXJVIPSacrificeBonusMethod
//
/////////////////////////////////////////
crJXJVIPSacrificeBonusMethod::crJXJVIPSacrificeBonusMethod() :
	m_this(NULL),
	m_cashPtr(NULL),
	m_cash1Ptr(NULL)
{
}
crJXJVIPSacrificeBonusMethod::crJXJVIPSacrificeBonusMethod(const crJXJVIPSacrificeBonusMethod& handle) :
	crMethod(handle),
	m_cashPtr(handle.m_cashPtr),
	m_cash1Ptr(handle.m_cash1Ptr)
{
}
void crJXJVIPSacrificeBonusMethod::inputParam(int i, void* param)
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
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_cashPtr = (int*)(HIINT64(param64));
			m_cash1Ptr = (int*)(LOINT64(param64));
		}
		break;
	}
}
void crJXJVIPSacrificeBonusMethod::addParam(int i, const std::string& str)
{
}
void crJXJVIPSacrificeBonusMethod::operator()(crHandle& handle)
{
	crData* data = m_this->getDataClass();

	void* param;

	// 1. 取征收倍数
	data->getParam(WCHDATA_JXJVIPJiSiXinDe, param);
	unsigned char increaseLev = *((unsigned char*)param);

	// 2. 取可能性
	data->getParam(WCHDATA_JXJVIPZongMiaoJiSi, param);
	unsigned char randomLev = *((unsigned char*)param);

	printf("Sacrifice increaseLev is %d, randomLev is %d\n", increaseLev, randomLev);
	// 3. 打开VIPAffairsLev
	crTableIO::DataVec recordVec;
	ref_ptr<crTableIO>  vipLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);

	// 4. 取征收倍数
	unsigned int increase;
	unsigned int random;
	int lvIndex = vipLvTab->getTitleIndex("lv");
	int addIndex = vipLvTab->getTitleIndex("add");

	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPJiSiXinDe), recordVec);
	bool ret = false;
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == increaseLev)
		{
			increase = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	// 5. 取可能性 
	ret = false;
	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPZongMiaoJiSi), recordVec);
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == randomLev)
		{
			random = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	CRCore::rangei r(0, 1000);
	int dice = r.get_random();
	if (dice < random)
	{
		if (m_cashPtr)
		{
			*m_cash1Ptr += (-(int)((float)(*m_cashPtr) * (float)increase * 0.001f));
		}
		else
		{
			*m_cash1Ptr = (int)((float)(*m_cash1Ptr) * (1.0f + (float)increase * 0.001f));
		}
	}
}
/////////////////////////////////////////
//
//crJXJVIPRecruitBonusMethod
//
/////////////////////////////////////////
crJXJVIPRecruitBonusMethod::crJXJVIPRecruitBonusMethod() :
	m_this(NULL),
	m_cashPtr(NULL),
	m_cash1Ptr(NULL)
{
}
crJXJVIPRecruitBonusMethod::crJXJVIPRecruitBonusMethod(const crJXJVIPRecruitBonusMethod& handle) :
	crMethod(handle),
	m_cashPtr(handle.m_cashPtr),
	m_cash1Ptr(handle.m_cash1Ptr)
{
}
void crJXJVIPRecruitBonusMethod::inputParam(int i, void* param)
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
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_cashPtr = (int*)(HIINT64(param64));
			m_cash1Ptr = (int*)(LOINT64(param64));
		}
		break;
	}
}
void crJXJVIPRecruitBonusMethod::addParam(int i, const std::string& str)
{
}
void crJXJVIPRecruitBonusMethod::operator()(crHandle& handle)
{
	crData* data = m_this->getDataClass();
	void* param;
	// 1. 取征收倍数
	data->getParam(WCHDATA_JXJVIPXunBingXinDe, param);
	unsigned char increaseLev = *((unsigned char*)param);

	// 2. 取可能性
	data->getParam(WCHDATA_JXJVIPYanBingYueWu, param);
	unsigned char randomLev = *((unsigned char*)param);

//	printf("Recruit increaseLev is %d, randomLev is %d\n", increaseLev, randomLev);

	// 3. 打开VIPAffairsLev
	crTableIO::DataVec recordVec;
	ref_ptr<crTableIO>  vipLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);

	// 4. 取征收倍数
	unsigned int increase;
	unsigned int random;
	int lvIndex = vipLvTab->getTitleIndex("lv");
	int addIndex = vipLvTab->getTitleIndex("add");

	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPXunBingXinDe), recordVec);
	bool ret = false;
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == increaseLev)
		{
			increase = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	// 5. 取可能性 
	ret = false;
	vipLvTab->queryRecords(0, crArgumentParser::appItoa(WCHDATA_JXJVIPYanBingYueWu), recordVec);
	for (int i = 0; i < recordVec.size(); ++i)
	{
		if (atoi(recordVec[i][lvIndex].c_str()) == randomLev)
		{
			random = atoi(recordVec[i][addIndex].c_str());
			ret = true;
			break;
		}
	}
	if (!ret)
		return;

	CRCore::rangei r(0, 1000);
	int dice = r.get_random();
	if (dice < random)
	{
		if (m_cashPtr)
		{
			*m_cash1Ptr += (-(int)((float)(*m_cashPtr) * (float)increase * 0.001f));
		}
		else
		{
			*m_cash1Ptr = (int)((float)(*m_cash1Ptr) * (1.0f + (float)increase * 0.001f));
		}
	}
}
/////////////////////////////////////////
//
//crJXJAdjustToushicheBulletSpeedMethod
//
/////////////////////////////////////////
crJXJAdjustToushicheBulletSpeedMethod::crJXJAdjustToushicheBulletSpeedMethod() :
	m_this(NULL),
	m_firer(NULL),
	m_zoffset(0.0f)
{
}
crJXJAdjustToushicheBulletSpeedMethod::crJXJAdjustToushicheBulletSpeedMethod(const crJXJAdjustToushicheBulletSpeedMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_firer(NULL),
	m_zoffset(handle.m_zoffset),
	m_speedRange(handle.m_speedRange)
{
}
void crJXJAdjustToushicheBulletSpeedMethod::inputParam(int i, void* param)
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
		m_this = (crWeaponMatterObject*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_firer = (crInstanceItem*)(LOINT64(param64));
		}
		break;
	}
}
void crJXJAdjustToushicheBulletSpeedMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_speedRange);
		break;
	}
}
void crJXJAdjustToushicheBulletSpeedMethod::operator()(crHandle& handle)
{
	crVector3 firepos = m_firer->getPosition();
	firepos[2] += m_zoffset;
	crVector3 targetPos;
	ref_ptr<crInstanceItem> targetItem;
	void *param;
	crData *firerData = m_firer->getDataClass();
	firerData->getParam(WCHDATA_ItemState,param);
	unsigned char itemstate = *(unsigned char *)param;
	if(itemstate == IS_MoveToPos || itemstate == IS_MoveToUseSkill/* || itemstate == IS_AttackToPos*/)
	{
		firerData->getParam(WCHDATA_MoveToPos,param);
		targetPos = *(crVector3 *)param;
	}
	else
	{
		firerData->getParam(WCHDATA_TargetType,param);
		unsigned char targettype = *(unsigned char *)param;
		if(targettype & Target_Instance || targettype & Target_StaticNpc/* || targettype & Target_StaticItem*/)
		{
			ref_ptr<crInstanceItem> targetItem;
			ref_ptr<crMatrixTransform> targetNode;
			m_firer->clientGetTarget(targetItem,targetNode);
			if(targetItem.valid())
			{
				targetPos = targetItem->getPosition();
			}
			else
			{
				return;
			}
		}
		else
		{
			firerData->getParam(WCHDATA_TargetPos,param);
			targetPos = *(crVector3 *)param;
		}
	}
	///
	float speed = 15.0f;
	//计算speed

	float horizDistance = targetPos[2] - firepos[2];
	float verticalDistanceSquare = pow(firepos[0] - targetPos[0], 2) + pow(firepos[1] - targetPos[1], 2);
	float verticalDistance = sqrt(verticalDistanceSquare);
	if (horizDistance - verticalDistance < -0.001f)
	{
		float g = crBrain::getInstance()->getCurrentActiveDB()->getGravity()[2];
		float xspeed = 0.5f * g * verticalDistanceSquare * (1/(horizDistance - verticalDistance));
		speed = sqrt(xspeed + xspeed);
		speed = clampTo(speed, m_speedRange[0], m_speedRange[1]);
	}
	const_cast<crBulletObject *>(m_this->getBulletObject())->setLinearVel(speed);
}
/////////////////////////////////////////
//
//crJXJConnectWithStationCheckMethod
//
/////////////////////////////////////////
crJXJConnectWithStationCheckMethod::crJXJConnectWithStationCheckMethod():
	m_chengchiid(0),
	m_output(NULL){}
crJXJConnectWithStationCheckMethod::crJXJConnectWithStationCheckMethod(const crJXJConnectWithStationCheckMethod& handle):
	crMethod(handle),
	m_chengchiid(0),
	m_output(NULL)
{
}
void crJXJConnectWithStationCheckMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_output = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_chengchiid = LOINT64(param64);
			m_output = (bool *)(HIINT64(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}
void crJXJConnectWithStationCheckMethod::addParam(int i, const std::string& str)
{
}
void crJXJConnectWithStationCheckMethod::operator()(crHandle &handle)
{
	if(m_output)
	{
		crData *data = m_this->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJChengchiID,param);
		unsigned short stationid = *(unsigned short *)param;
		if(stationid == m_chengchiid)
			*m_output = true;
		else
		{
			m_myCitySet.clear();
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;
			if(crGlobalHandle::isClient())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *map_city = (ChengchiMap *)param;
				for( ChengchiMap::iterator itr = map_city->begin(); itr != map_city->end(); ++itr)
				{
					if(itr->second->getChengzhuShili() == shiliid)
					{
						m_myCitySet.insert(itr->first);
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				crData *serverBrainData = crServerBrainHandle::getInstance()->getDataClass();
				serverBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				serverBrainData->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *map_city = (ChengchiMap *)param;
				for( ChengchiMap::iterator itr = map_city->begin(); itr != map_city->end(); ++itr)
				{
					if(itr->second->getChengzhuShili() == shiliid)
					{
						m_myCitySet.insert(itr->first);
					}
				}
				serverBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			///
			m_searchSet.clear();
			*m_output = search(stationid);
		}
	}
}
bool crJXJConnectWithStationCheckMethod::search(unsigned short searchid)
{
	if(searchid == m_chengchiid)
		return true;
	m_searchSet.insert(searchid);
	ref_ptr<crTableIO> tab_city = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	crTableIO::StrVec record;
	unsigned short ngbs[4];
	int ngb_ids[4];
	ngb_ids[0] = tab_city->getTitleIndex("接壤1");
	ngb_ids[1] = tab_city->getTitleIndex("接壤2");
	ngb_ids[2] = tab_city->getTitleIndex("接壤3");
	ngb_ids[3] = tab_city->getTitleIndex("接壤4");
	for(int i = 0; i < 4; i++)
	{
		if(tab_city->queryOneRecord(0, crArgumentParser::appItoa(searchid), record)>=0)
		{
			ngbs[i] = (unsigned short)(atoi(record[ngb_ids[i]].c_str()));
			if(ngbs[i]>0 && m_searchSet.find(ngbs[i]) == m_searchSet.end())
			{
				if(ngbs[i] == m_chengchiid)
					return true;
				if(m_myCitySet.find(ngbs[i]) != m_myCitySet.end() && search(ngbs[i]))
					return true;
			}
		}
	}
	return false;
}
crJXJShowCityInfoMethod::crJXJShowCityInfoMethod() :
	m_this(NULL),
	m_ea(NULL)
{
}
crJXJShowCityInfoMethod::crJXJShowCityInfoMethod(const crJXJShowCityInfoMethod& handle) :
	crMethod(handle),
	m_this(handle.m_this),
	m_ea(handle.m_ea),
	m_mainCanvas(handle.m_mainCanvas),
	m_tip1(handle.m_tip1),
	m_tip2(handle.m_tip2),
	m_chengchi_chanliang(handle.m_chengchi_chanliang),
	m_guishu_juntuan(handle.m_guishu_juntuan),
	m_chengchi_techan(handle.m_chengchi_techan),
	m_xiangdui_dengji(handle.m_xiangdui_dengji),
	m_ling_name(handle.m_ling_name),
	m_chengchi_type(handle.m_chengchi_type),
	m_zhangguan_shenfen(handle.m_zhangguan_shenfen),
	m_guojia(handle.m_guojia),
	m_zhoujun(handle.m_zhoujun),
	m_cityMap(handle.m_cityMap),
	m_terrainInfo(handle.m_terrainInfo)
{
}
void crJXJShowCityInfoMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		if (param)
		{
			m_this = (crWidgetNode*)param;
		}
		else
		{
			m_this = NULL;
		}
		break;
	case 2:
		if(param)
		{
			//m_this = (crWidgetNode*)param;
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));

		}
		else
		{
			m_ea = NULL;
		}
		break;
	}

}
void crJXJShowCityInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_mainCanvas = str;
		break;
	case 1:
		m_tip1 = str;
		break;
	case 2:
		m_tip2 = str;
		break;
	case 3:
		m_chengchi_chanliang = str;
		break;
	case 4:
		m_guishu_juntuan = str;
		break;
	case 5:	
		m_chengchi_techan = str;
		break;
	case 6:	
		m_xiangdui_dengji = str;
		break;
	case 7:
		m_ling_name = str;
		break;
	case 8:
		m_chengchi_type = str;
		break;
	case 9:
		m_zhangguan_shenfen = str;
		break;
	case 10:	
		m_guojia = str;
		break;
	case 11:	
		m_zhoujun = str;
		break;
	case 12:
		m_cityMap = str;
		break;
	case 13:
		m_terrainInfo = str;
		break;
	}
}
void crJXJShowCityInfoMethod::operator()(crHandle &handle)
{
	// 取主ui
	crCanvasNode *this_canvas = crFilterRenderManager::getInstance()->findCanvas(m_mainCanvas);
	crCanvasNode * cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_cityMap);
	if (!this_canvas || !cityMapCanvas)
		return;

	ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	if (false == chengchitab.valid())
		return;

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (!myPlayer)
		return;

	crData *data = myPlayer->getDataClass();
	if (!data)
		return;

	void* param;
	data->getParam(WCHDATA_JXJChengChiMap,param);
	ChengchiMap *map_city = (ChengchiMap *)param;
	if (!map_city)
		return;

	data->getParam(WCHDATA_JXJFoundCountryGetAward, param);
	unsigned char hadGottenAward = *(unsigned char *)param;

	crTableIO::StrVec record;
	int buttonnameindex = chengchitab->getTitleIndex("buttonname");
	int citynameindex = chengchitab->getTitleIndex("名字");
	int idIndex = chengchitab->getTitleIndex("id");
	int xingzhenIndex = chengchitab->getTitleIndex("行政长官");
	int xingzhenchengweiIndex = chengchitab->getTitleIndex("称谓");
	int shuomingIndex = chengchitab->getTitleIndex("说明");
	int dengjiIndex = chengchitab->getTitleIndex("建国后等级");
	if ((unsigned char)-1 != hadGottenAward)
	{//降低建国城池等级，占领建国城池后等级恢复正常
		dengjiIndex = chengchitab->getTitleIndex("城池等级");
	}
	int restypeIndex = chengchitab->getTitleIndex("收益类型");
	int resproduceIndex = chengchitab->getTitleIndex("收益数量");
	int specialtypeIndex = chengchitab->getTitleIndex("特产类型");
	int specialproduceIndex = chengchitab->getTitleIndex("特产数量");
	int cantonidIndex = chengchitab->getTitleIndex("州ID");
	int zhoujunnameindex = chengchitab->getTitleIndex("州名称");
	int chengchiTypeIndex = chengchitab->getTitleIndex("类型");
	int terrainInfoIndex = chengchitab->getTitleIndex("地形");

	if (buttonnameindex < 0 || citynameindex < 0 || idIndex < 0 || xingzhenIndex < 0 || xingzhenchengweiIndex < 0 || shuomingIndex < 0 || dengjiIndex < 0
		|| restypeIndex < 0 || resproduceIndex < 0 || specialtypeIndex < 0 || specialproduceIndex < 0 || cantonidIndex < 0 || zhoujunnameindex < 0 || chengchiTypeIndex < 0
		|| terrainInfoIndex < 0)
	{
		return;
	}
	std::string btName = m_this->getName();

	// 取哪个城池
	if(chengchitab->queryOneRecord(buttonnameindex, btName, record)>=0)
	{
		crStaticTextWidgetNode *tip1 = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_tip1));
		crStaticTextWidgetNode *tip2 = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_tip2));
		crStaticTextWidgetNode *chengchi_chanliang = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_chengchi_chanliang));
		crStaticTextWidgetNode *guishu_juntuan = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_guishu_juntuan));
		crStaticTextWidgetNode *chengchi_techan = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_chengchi_techan));
		crStaticTextWidgetNode *xiangdui_dengji	= dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_xiangdui_dengji));
		crStaticTextWidgetNode *ling_name = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_ling_name));
		crStaticTextWidgetNode *zhoujun = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_zhoujun));
		crStaticTextWidgetNode *chengchi_type = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_chengchi_type));
		crStaticTextWidgetNode *terrainInfo = dynamic_cast<crStaticTextWidgetNode *>(this_canvas->getWidget(m_terrainInfo));

		if(terrainInfo)
			terrainInfo->setString(record[terrainInfoIndex]);

		int id = atoi(record[idIndex].c_str());
		//int cantonID = atoi(record[cantonidIndex].c_str());
		unsigned char shili = 0;
		ChengchiMap::iterator ccItr = map_city->find(id);
		if(ccItr != map_city->end())
			shili = ccItr->second->getChengzhuShili();
		//for( ChengchiMap::iterator itr = map_city->begin(); itr != map_city->end(); ++itr)
		//{
		//	if (itr->first == id)
		//	{
		//		shili = itr->second.first;
		//		break;
		//	}
		//}	
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshili = *(unsigned char *)param;
		//显示关隘
		int city_type = atoi(record[chengchiTypeIndex].c_str());
		if (city_type == 0)
		{
			if (chengchi_type)
			{
				chengchi_type->setVisiable(true);
			}
		}
		else
		{
			if (chengchi_type)
			{
				chengchi_type->setVisiable(false);
			}
		}
		//州郡名称
		if (zhoujun)
		{
			zhoujun->setString(record[zhoujunnameindex]);
		}
		//城池名称
		if (tip1)
		{
			tip1->setString(record[citynameindex]);
		}
		//城市说明
		if (tip2)
		{
			tip2->setString(record[shuomingIndex]);
		}
		//城池产量
		if (chengchi_chanliang)
			chengchi_chanliang->setString(record[resproduceIndex]/* + "/时"*/);
		//归属军团
		crData * cityMapData = cityMapCanvas->getDataClass();
		if (guishu_juntuan)
		{
			std::string groupName = "无";
			std::string groupLeader = record[xingzhenIndex];
			if (cityMapData)
			{
				cityMapData->excHandle(MAKEINT64(WCH_LockData,1));
				cityMapData->getParam(WCHDATA_JXJChengChiInfoMap,param);
				CantonInfoMap * chengchiInfoMap = (CantonInfoMap *)param;
				CantonInfoMap::iterator cmItr = chengchiInfoMap->find(id);
				if (cmItr!=chengchiInfoMap->end() &&\
					cmItr->second->m_groupName.empty() == false)
				{
					groupName = cmItr->second->m_groupName;
				}
				if (cmItr!=chengchiInfoMap->end() && \
					cmItr->second->m_groupLeader.empty() == false)
				{
					groupLeader = cmItr->second->m_groupLeader;
				}
				cityMapData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			guishu_juntuan->setString(groupName);
			if (ling_name)
			{
				ling_name->setString(groupLeader);
			}
		}
		//城市特产
		if (chengchi_techan)
		{
			std::string specialname;
			int specialtype = atoi(record[specialtypeIndex].c_str());
			switch (specialtype)
			{
			case T_Food:
				specialname = "粮食";
				break;
			case T_Wood:
				specialname = "木材";
				break;
			case T_Iron:
				specialname = "铁矿";
				break;
			case T_Horse:
				specialname = "马匹";
				break;
			case T_Copper:
				specialname = "铜钱";
				break;
			case T_Exp:
				specialname = "经验";
				break;
			case T_Achievement:
				specialname = "战功";
				break;
			case T_Giftgold:
				specialname = "礼金";
				break;
			case T_TroopsEquip:
				specialname = "部队装备";
				break;
			case T_Item:
				specialname = "物品";
				break;
			case T_Troops:
				specialname = "部队";
				break;
			case T_Exploit:
				specialname = "功勋";
				break;
			case T_Contribute:
				specialname = "国家贡献";
				break;
			case T_Gold:
				specialname = "元宝";
				break;
			case T_JiangHun:
				specialname = "将魂";
				break;
			case T_CardImpression:
				specialname = "将卡好感度";
				break;
			default:
				specialname = "无";
				break;
			}
			chengchi_techan->setString(specialname);
		}
		//相对等级
		if (xiangdui_dengji)
		{
			crVector4i dvalue;
			crArgumentParser::appAtoVec(record[dengjiIndex],dvalue);
			std::string dengjiString;
			switch (myshili)
			{
			case 2:
				dengjiString = crArgumentParser::appItoa(dvalue[3]);
				break;
			case 10:
				dengjiString = crArgumentParser::appItoa(dvalue[0]);
				break;
			case 11:
				dengjiString = crArgumentParser::appItoa(dvalue[1]);
				break;
			case 12:
				dengjiString = crArgumentParser::appItoa(dvalue[2]);
				break;
			}
			xiangdui_dengji->setString(dengjiString);
		}
		
		// 取看守者的身份
		crMultiSwitch* select_sw = dynamic_cast<crMultiSwitch *>(this_canvas->getChildNode(m_zhangguan_shenfen));
		if (select_sw)
		{
			crVector2i kvalue;
			crArgumentParser::appAtoVec(record[xingzhenIndex], kvalue);
			select_sw->setActiveSwitchSet(kvalue[0]);
		}
		// 取国家
		crMultiSwitch* select_sw1 = dynamic_cast<crMultiSwitch *>(this_canvas->getChildNode(m_guojia));
		if (select_sw1)
		{
			switch (shili)
			{
			case 10:	// 魏
				select_sw1->setActiveSwitchSet(0);
				break;
			case 11:	// 蜀
				select_sw1->setActiveSwitchSet(1);
				break;
			case 12:	// 吴
				select_sw1->setActiveSwitchSet(2);
				break;
			case 2:
			default:
				select_sw1->setActiveSwitchSet(3);
				break;
			}
		}
		// 设置位置
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = this_canvas->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();

		float posx = 0.0f;
		float posy = 0.0f;

		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		CRCore::crVector3 trans = mat.getTrans();
		if (mouse.x() > 0.0f)
		{
			posx = mouse.x() -  trans.x() - tipsbox.xLength()/2 - 0.005f;
		}
		else
		{
			posx = mouse.x() -  trans.x() + tipsbox.xLength()/2 + 0.005f;;
		}

		if (mouse.y() > 0.0f)
		{
			posy = mouse.y() - trans.y() - tipsbox.yLength()/2 - 0.005f;;
		}
		else
		{
			posy = mouse.y() - trans.y() + tipsbox.yLength()/2 + 0.005f;; 
		}

		this_canvas->setMatrix(crMatrix::translate(posx, posy, 0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(this_canvas, true);
	}
}

crJXJStoveInitMethod::crJXJStoveInitMethod() :
	m_this(NULL)
{
}
crJXJStoveInitMethod::crJXJStoveInitMethod(const crJXJStoveInitMethod& handle) :
	crMethod(handle),
	m_this(handle.m_this)
{
}
void crJXJStoveInitMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJStoveInitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_locks[0] = str;
		break;
	case 1:
		m_locks[1] = str;
		break;
	case 2:
		m_locks[2] = str;
		break;
	}
}
void crJXJStoveInitMethod::operator()(crHandle &handle)
{
	if (!m_this)
		return;
	crCheckBoxWidgetNode* lockSwitch[3];
	for (int i = 0; i < 3; i++)
	{
		lockSwitch[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_locks[i]));
	}
	for (int i = 0; i < 3; i++)
	{
		if (lockSwitch[i])
		{
			lockSwitch[i]->select(false);
			lockSwitch[i]->setVisiable(false);
		}
	}
}
crJXJFriendHmdMethod::crJXJFriendHmdMethod() :
	m_this(NULL)
{

}
crJXJFriendHmdMethod::crJXJFriendHmdMethod(const crJXJFriendHmdMethod& handle) :
	crMethod(handle),
	m_this(handle.m_this),
	m_hmdTitleSwitch(handle.m_hmdTitleSwitch),
	m_contentSwitch(handle.m_contentSwitch)
{

}
void crJXJFriendHmdMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJFriendHmdMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_hmdTitleSwitch = str;
		break;
	case 1:
		m_contentSwitch = str;
		break;
	}
}
void crJXJFriendHmdMethod::operator()(crHandle &handle)
{
	//crCanvasNode *this_canvas = crFilterRenderManager::getInstance()->findCanvas(m_mainCanvas);
	crCanvasNode *this_canvas = m_this->getParentCanvas();
	if (!this_canvas)
		return;

	crMultiSwitch* hmdTitleSwitch = dynamic_cast<crMultiSwitch *>(this_canvas->getChildNode(m_hmdTitleSwitch));
	crMultiSwitch* contentSwitch = dynamic_cast<crMultiSwitch *>(this_canvas->getChildNode(m_contentSwitch));
	if (!hmdTitleSwitch || !contentSwitch)
		return;

	hmdTitleSwitch->setActiveSwitchSet(1);
	contentSwitch->setActiveSwitchSet(2);

	int firstpage = 1;
	crData *brainData = crBrain::getInstance()->getDataClass();
	brainData->inputParam(WCHDATA_JXJMyCurFriendPageNum,&firstpage);
	

}
crJXJFriendHmdAddMethod::crJXJFriendHmdAddMethod() :
	m_this(NULL)
{
}
crJXJFriendHmdAddMethod::crJXJFriendHmdAddMethod(const crJXJFriendHmdAddMethod& handle) :
	crMethod(handle),
	m_this(handle.m_this)
{
}
void crJXJFriendHmdAddMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJFriendHmdAddMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_thisCanvas = str;
		break;
	case 1:
		m_titleSwitch = str;
		break;
	}
}
void crJXJFriendHmdAddMethod::operator()(crHandle &handle)
{
	crCanvasNode *this_canvas = crFilterRenderManager::getInstance()->findCanvas(m_thisCanvas);
	if (!this_canvas)
		return;
	crMultiSwitch* titleSwitch = dynamic_cast<crMultiSwitch *>(this_canvas->getChildNode(m_titleSwitch));
	if (titleSwitch)
	{
		titleSwitch->setActiveSwitchSet(1);  
		titleSwitch->setVisiable(true);
	}
	unsigned char addblackname = JXJAddFriendType_BlackName;
	crData *braindata = crBrain::getInstance()->getDataClass();
	braindata->inputParam(WCHDATA_JXJAddFriendOrBlack,&addblackname);
	crFilterRenderManager::getInstance()->showCanvas(this_canvas, true);

}
