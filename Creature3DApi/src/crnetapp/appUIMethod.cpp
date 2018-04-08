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
#include <CRNetApp/appNetMethod.h>
#include <CRNetApp/appUIMethod.h>
#include <CRNetApp/appNetStartLoader.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appNetLogic.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crWidgetNode.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRCore/thread/crThread.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crWriteFile.h>
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
#include <CREncapsulation/crGameDices.h>
#include <CRCore/crTexMat.h>

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
using namespace CRText;
using namespace CRUI;
////////////////////////////////
//
//crShowTalkTextMethod
//
///////////////////////////////////
crUIShowNpcNameMethod::crUIShowNpcNameMethod()
{
}
crUIShowNpcNameMethod::crUIShowNpcNameMethod(const crUIShowNpcNameMethod& handle):
crMethod(handle),
m_canvasName(handle.m_canvasName),
m_widgetName(handle.m_widgetName)
{
}

void crUIShowNpcNameMethod::inputParam(int i, void *param)
{
}

void crUIShowNpcNameMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_widgetName = str;
		break;
	}
}

void crUIShowNpcNameMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	if(canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode> widget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_widgetName));
		if(widget.valid())
		{
			crRole *player = crMyPlayerData::getInstance()->getCurrentRole();
			ref_ptr<crInstanceItem>targetItem;
			ref_ptr<crMatrixTransform>targetNode;
			player->clientGetTarget(targetItem,targetNode);
			if(targetItem.valid())
			{
				std::string name = targetItem->getIName();
				widget->setString(name);
			}
		}
	}
}
////////////////////////////////
//
//crUIShowHyperTextMethod
//
///////////////////////////////////
crUIShowHyperTextMethod::crUIShowHyperTextMethod()
{
}
crUIShowHyperTextMethod::crUIShowHyperTextMethod(const crUIShowHyperTextMethod& handle):
crMethod(handle),
m_canvasName(handle.m_canvasName),
m_widgetName(handle.m_widgetName),
m_hypertextFile(handle.m_hypertextFile)
{
}

void crUIShowHyperTextMethod::inputParam(int i, void *param)
{
}

void crUIShowHyperTextMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_widgetName = str;
		break;
	case 2:
		m_hypertextFile = str;
		break;
	}
}

void crUIShowHyperTextMethod::operator()(crHandle &handle)
{
	do 
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
		if(!canvas.valid())
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_widgetName));
		if(!widget.valid())
			break;
		if(m_hypertextFile.empty())
			widget->setHypertext(m_hypertextFile);
		else
			widget->setHypertext(crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + m_hypertextFile);
	} while (0);
}
/////////////////////////////////////////
//
//crCreateItemTalkMethod
//
/////////////////////////////////////////
crCreateItemTalkMethod::crCreateItemTalkMethod(){}
crCreateItemTalkMethod::crCreateItemTalkMethod(const crCreateItemTalkMethod& handle):
crMethod(handle),
m_tablefile(handle.m_tablefile)
{
}
void crCreateItemTalkMethod::inputParam(int i, void *param)
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

void crCreateItemTalkMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tablefile = str;
		break;
	}
}

void crCreateItemTalkMethod::operator()(crHandle &handle)
{
	if(m_this && !m_tablefile.empty())
	{
		if(!m_tasktable.valid())
		{
			m_tasktable = crTableIO::openFile(crMyPlayerData::getInstance()->getSelectedGame()->getTableDir() + m_tablefile);
		}
		if(m_tasktable.valid())
		{
			void *param;
			crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
			crData *meData = me->getDataClass();
			meData->getParam(WCHDATA_Level,param);
			unsigned char lv = *((unsigned char*)param);

			crData *metierData = me->getMetierDataClass();
			ref_ptr<crGameTask> gameTask;
			metierData->excHandle(MAKEINT64(WCH_LockData,1));
			metierData->getParam(WCHDATA_GameTaskMap,param);
			GameTaskMap* gameTaskMap = (GameTaskMap* )param;

			int rowcount = m_tasktable->getRowCount();
			int perTaskID = 0;
			std::string talkText,option;
			int taskid = 0;
			char progress = 0;
			GameTaskMap::iterator gameTaskItr;
			bool found = false;
			for(int i = 1; i<rowcount; i++)
			{
				taskid = atoi((*m_tasktable)(i,1).c_str());
				gameTaskItr = gameTaskMap->find(taskid);
				gameTask = gameTaskItr!=gameTaskMap->end()?gameTaskItr->second.get():NULL;
				if(!gameTask.valid() || gameTask->isComplete())
				{//任务不存在或者已经完成
					continue;
				}
				progress = atoi((*m_tasktable)(i,2).c_str());
				if(!gameTask->getActivation())
				{//任务未接
					if(progress == 0)
					{//这是接任务处，检查条件
						const crVector2ub &lvRange = gameTask->getLvRange();
						if(lvRange != crVector2ub(0,0))
						{
							if(lv<lvRange[0] || lv>lvRange[1])
							{//等级不满足
								continue;
							}
						}
						perTaskID = gameTask->getPreTask();
						if(perTaskID!=0)
						{
							gameTaskItr = gameTaskMap->find(perTaskID);
							gameTask = gameTaskItr!=gameTaskMap->end()?gameTaskItr->second.get():NULL;
							if(!gameTask.valid() || !gameTask->isComplete())
							{//前置任务没有完成
								continue;
							}
						}
						talkText = (*m_tasktable)(i,3);
						option = (*m_tasktable)(i,4);
					}
					else
					{//这里不是接任务处
						continue;
					}
				}
				else
				{//任务已接
					if(progress == -1)
					{
						if(gameTask->isTaskCanRefer())
						{
							talkText = (*m_tasktable)(i,3);
							option = (*m_tasktable)(i,4);
						}
						else
						{
							continue;
						}
					}
					else if(gameTask->getTaskStepType() == TS_Zero && gameTask->getTaskProgress() + 1 == progress)
					{//显示任务相关对话
						talkText = (*m_tasktable)(i,3);
						option = (*m_tasktable)(i,4);
					}
					else
					{//步骤不对
						continue;
					}
				}
				found = true;
				break;
			}
			if(!found)
			{//显示无任务时的对话内容
				talkText = (*m_tasktable)(0,3);
				option = (*m_tasktable)(0,4);
			}
			metierData->excHandle(MAKEINT64(WCH_LockData,0));
			crData *itemData = m_this->getDataClass();
			itemData->inputParam(WCHDATA_ItemTalk,&talkText);
			itemData->inputParam(WCHDATA_ItemOption,&option);
		}
	}
}
////////////////////////////////
//
//crShowItemTalkMethod
//
///////////////////////////////////
crShowItemTalkMethod::crShowItemTalkMethod():
m_talkoropt(true)
{
}
crShowItemTalkMethod::crShowItemTalkMethod(const crShowItemTalkMethod& handle):
crMethod(handle),
m_canvasName(handle.m_canvasName),
m_widgetName(handle.m_widgetName),
m_talkoropt(handle.m_talkoropt)
{
}

void crShowItemTalkMethod::inputParam(int i, void *param)
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

void crShowItemTalkMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_widgetName = str;
		break;
	case 2:
		m_talkoropt = (bool)(atoi(str.c_str()));
		break;
	}
}

void crShowItemTalkMethod::operator()(crHandle &handle)
{
	do 
	{
		if(!m_this)
			break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
		if(!canvas.valid())
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_widgetName));
		if(!widget.valid())
			break;
		void *param;
		crData *itemData = m_this->getDataClass();
		itemData->getParam(m_talkoropt?WCHDATA_ItemTalk:WCHDATA_ItemOption,param);
		std::string file = *(std::string *)param;
		if(!file.empty())
			file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
		widget->setHypertext(file);
	} while (0);
}
//////////////////////////////////
////
////crUIShowTaskMethod
////
/////////////////////////////////////
//crUIShowTaskMethod::crUIShowTaskMethod():
//m_taskid(0),
//m_canvas(NULL),
//m_waitTime(5000)
//{
//}
//crUIShowTaskMethod::crUIShowTaskMethod(const crUIShowTaskMethod& handle):
//crMethod(handle),
//m_taskid(0),
//m_canvas(NULL),
//m_frameStaticText(handle.m_frameStaticText),
//m_desTextHypertext(handle.m_desTextHypertext),
//m_progressHypertext(handle.m_progressHypertext),
//m_item1ImageBox(handle.m_item1ImageBox),
//m_item2ImageBox(handle.m_item2ImageBox),
//m_item3ImageBox(handle.m_item3ImageBox),
//m_item4ImageBox(handle.m_item4ImageBox),
//m_expStaticText(handle.m_expStaticText),
//m_exp2StaticText(handle.m_exp2StaticText),
//m_goldStaticText(handle.m_goldStaticText),
//m_waitTime(handle.m_waitTime)
//{
//}
//
//void crUIShowTaskMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_canvas = NULL;
//		}
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_taskid = LOINT64(param64);
//			m_canvas = (std::string *)(HIINT64(param64));
//		}
//		else
//		{
//			m_taskid = 0;
//		}
//		break;
//	}
//}
//
//void crUIShowTaskMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_frameStaticText = str;
//		break;
//	case 1:
//		m_desTextHypertext = str;
//		break;
//	case 2:
//		m_progressHypertext = str;
//		break;
//	case 3:
//		m_item1ImageBox = str;
//		break;
//	case 4:
//		m_item2ImageBox = str;
//		break;
//	case 5:
//		m_item3ImageBox = str;
//		break;
//	case 6:
//		m_item4ImageBox = str;
//		break;
//	case 7:
//		m_expStaticText = str;
//		break;
//	case 8:
//		m_exp2StaticText = str;
//		break;
//	case 9:
//		m_goldStaticText = str;
//		break;
//	case 10:
//		m_waitTime = atoi(str.c_str());
//		break;
//	}
//}
//
//void crUIShowTaskMethod::operator()(crHandle &handle)
//{
//	do 
//	{
//		if(m_taskid == 0)
//			break;
//
//		void *param;
//		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
//		crData *metierData = me->getMetierDataClass();
//		metierData->excHandle(MAKEINT64(WCH_LockData,1));
//		metierData->getParam(WCHDATA_GameTaskMap,param);
//		GameTaskMap* gameTaskMap = (GameTaskMap* )param;
//		GameTaskMap::iterator itr = gameTaskMap->find(m_taskid);
//		ref_ptr<crGameTask> gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
//		metierData->excHandle(MAKEINT64(WCH_LockData,0));
//		if(!gameTask.valid())
//			break;
//
//		crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(*m_canvas);
//		if(!canvas)
//			break;
//		crStaticTextWidgetNode *frameWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_frameStaticText));
//		if(frameWidget)
//		{
//			frameWidget->setString(gameTask->getTaskName());
//		}
//		crHypertextWidgetNode *desWidget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_desTextHypertext));
//		if(desWidget)
//		{
//			std::string file = gameTask->getHyperText();
//			if(!file.empty())
//				file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
//			desWidget->setHypertext(file);
//		}
//		crHypertextWidgetNode *progressWidget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_progressHypertext));
//		if(progressWidget)
//		{
//			std::string file = gameTask->getHyperText();
//			if(gameTask->isComplete() || file.empty())
//			{
//				progressWidget->clearText();
//			}
//			else if(!gameTask->getActivation())
//			{
//				file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
//				std::string ext = crArgumentParser::getFileExtension(file);
//				file = crArgumentParser::getFileNameEliminateExt(file)+"0."+ext;
//				progressWidget->setHypertext(file);
//			}
//			else
//			{
//				file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
//				std::string ext = crArgumentParser::getFileExtension(file);
//
//				unsigned char steptype = gameTask->getTaskStepType();
//				if(steptype == TS_Zero)
//				{
//					int progress = gameTask->getTaskProgress()+1;
//					file = crArgumentParser::getFileNameEliminateExt(file)+crArgumentParser::appItoa(progress)+"."+ext;
//					progressWidget->setHypertext(file);
//				}
//				else if(gameTask->isTaskCanRefer())
//				{
//					file = crArgumentParser::getFileNameEliminateExt(file)+"1."+ext;
//					progressWidget->setHypertext(file);
//				}
//				else
//				{
//					char tmpText[64];
//					std::string relfile;
//					int progress = gameTask->getTaskProgress();
//					_crInt16 A,B;
//					A = LOINT32(progress);
//					B = HIINT32(progress);
//					_crInt16 C,D;
//					int step = gameTask->getTaskSteps()-1;
//					C = LOINT32(step);
//					D = HIINT32(step);
//					_crInt8 a,c;
//					if(steptype >= TS_One)
//					{
//						relfile = crArgumentParser::getFileNameEliminateExt(file)+"One."+ext;
//						progressWidget->setHypertext(relfile);
//						
//						a = LOINT16(A);
//						c = LOINT16(C);
//						ItemMap itemMap;
//						rcfg::ConfigScript cfg_script(&itemMap);
//						cfg_script.Add("Hypertext");
//						cfg_script.Push("Hypertext");
//						cfg_script.Add("Content");
//						cfg_script.Push("Content");
//						std::vector<float>v_i;
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						cfg_script.Add("Color",v_i);
//						sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
//						cfg_script.Add("Text",tmpText);
//						cfg_script.Pop();
//						cfg_script.Pop();
//						progressWidget->setHypertext(cfg_script,true);
//					}
//					if(steptype >= TS_Two)
//					{
//						relfile = crArgumentParser::getFileNameEliminateExt(file)+"Two."+ext;
//						progressWidget->setHypertext(relfile,true);
//
//						a = HIINT16(A);
//						c = HIINT16(C);
//						ItemMap itemMap;
//						rcfg::ConfigScript cfg_script(&itemMap);
//						cfg_script.Add("Hypertext");
//						cfg_script.Push("Hypertext");
//						cfg_script.Add("Content");
//						cfg_script.Push("Content");
//						std::vector<float>v_i;
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						cfg_script.Add("Color",v_i);
//						sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
//						cfg_script.Add("Text",tmpText);
//						cfg_script.Pop();
//						cfg_script.Pop();
//						progressWidget->setHypertext(cfg_script,true);
//					}
//					if(steptype >= TS_Three)
//					{
//						relfile = crArgumentParser::getFileNameEliminateExt(file)+"Three."+ext;
//						progressWidget->setHypertext(relfile,true);
//
//						a = LOINT16(B);
//						c = LOINT16(D);
//						ItemMap itemMap;
//						rcfg::ConfigScript cfg_script(&itemMap);
//						cfg_script.Add("Hypertext");
//						cfg_script.Push("Hypertext");
//						cfg_script.Add("Content");
//						cfg_script.Push("Content");
//						std::vector<float>v_i;
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						cfg_script.Add("Color",v_i);
//						sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
//						cfg_script.Add("Text",tmpText);
//						cfg_script.Pop();
//						cfg_script.Pop();
//						progressWidget->setHypertext(cfg_script,true);
//					}
//					if(steptype >= TS_Four)
//					{
//						relfile = crArgumentParser::getFileNameEliminateExt(file)+"Four."+ext;
//						progressWidget->setHypertext(relfile,true);
//
//						a = HIINT16(B);
//						c = HIINT16(D);
//						ItemMap itemMap;
//						rcfg::ConfigScript cfg_script(&itemMap);
//						cfg_script.Add("Hypertext");
//						cfg_script.Push("Hypertext");
//						cfg_script.Add("Content");
//						cfg_script.Push("Content");
//						std::vector<float>v_i;
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						v_i.push_back(0.0f);
//						v_i.push_back(255.0f);
//						cfg_script.Add("Color",v_i);
//						sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
//						cfg_script.Add("Text",tmpText);
//						cfg_script.Pop();
//						cfg_script.Pop();
//						progressWidget->setHypertext(cfg_script,true);
//					}
//				}
//			}
//		}
//		crStaticTextWidgetNode *expWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_expStaticText));
//		if(expWidget)
//		{
//			expWidget->setString(crArgumentParser::appItoa(gameTask->getExp()));
//		}
//		crStaticTextWidgetNode *exp2Widget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_exp2StaticText));
//		if(exp2Widget)
//		{
//			exp2Widget->setString(crArgumentParser::appItoa(gameTask->getExp2()));
//		}
//		crStaticTextWidgetNode *goldWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_goldStaticText));
//		if(goldWidget)
//		{
//			goldWidget->setString(crArgumentParser::appItoa(gameTask->getGold()));
//		}
//		///m_item1ImageBox
//		bool request = gameTask->isRequestAwardItem();
//		if(request)
//		{
//			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//			if(netConductor)
//			{
//				crWaitNetReturnStreamLogic::getLock();
//				crRequestTaskAwardInfoPacket packet;
//				crRequestTaskAwardInfoPacket::buildRequestPacket(packet,me->getRoleID(),m_taskid);
//				netConductor->getNetManager()->sendPacket("all",packet);
//				crWaitNetReturnStreamLogic::simpleWait(m_waitTime);
//			}
//		}
//		int i = 0;
//		crImageBoxWidgetNode *imagebox[4];
//		imagebox[0] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_item1ImageBox));
//		imagebox[1] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_item2ImageBox));
//		imagebox[2] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_item3ImageBox));
//		imagebox[3] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_item4ImageBox));
//		if(/*gameTask->getActivation() && */!gameTask->isComplete())
//		{
//			crData *itemData;
//			AwardItemMap &awardItemMap = gameTask->getAwardItemMap();
//			for( AwardItemMap::iterator itr = awardItemMap.begin();
//				itr != awardItemMap.end()&&i<4;
//				++itr,i++ )
//			{
//				if(itr->second.valid())
//				{
//					itemData = itr->second->getDataClass();
//					itemData->getParam(WCHDATA_ItemIcon,param);
//					imagebox[i]->setImageName(*(std::string *)param);
//				}
//				else
//				{
//					imagebox[i]->setImageName(std::string());
//				}
//			}
//		}
//		for(int j = i; j<4; j++)
//		{
//			imagebox[j]->setImageName(std::string());
//		}
//		metierData->inputParam(WCHDATA_CurrentTaskID,&m_taskid);
//		crFilterRenderManager::getInstance()->showCanvas(canvas,true);
//	} while (0);
//}
//////////////////////////////////
////
////crUIAwardItemTipInfoMethod
////
/////////////////////////////////////
//crUIAwardItemTipInfoMethod::crUIAwardItemTipInfoMethod():
//m_id(1),
//m_ea(NULL)
//{
//}
//crUIAwardItemTipInfoMethod::crUIAwardItemTipInfoMethod(const crUIAwardItemTipInfoMethod& handle):
//crMethod(handle),
//m_id(handle.m_id),
//m_tipCanvasName(handle.m_tipCanvasName),
//m_tipWidgetName(handle.m_tipWidgetName),
//m_ea(NULL)
//{
//}
//
//void crUIAwardItemTipInfoMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_ea = NULL;
//		}
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
//		}
//		else
//		{
//			m_ea = NULL;
//		}
//		break;
//	}
//}
//
//void crUIAwardItemTipInfoMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_id = atoi(str.c_str());
//		break;
//	case 1:
//		m_tipCanvasName = str;
//		break;
//	case 2:
//		m_tipWidgetName = str;
//		break;
//	}
//}
//
//void crUIAwardItemTipInfoMethod::operator()(crHandle &handle)
//{
//	do 
//	{
//		crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
//		if(!canvas)
//			break;
//		crHypertextWidgetNode *widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
//		if(!widget)
//			break;
//		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
//		if(me)
//		{
//			void *param;
//			crData *metierData = me->getMetierDataClass();
//			metierData->getParam(WCHDATA_CurrentTaskID,param);
//			int taskid = *(int *)param;
//			ref_ptr<crGameTask> gameTask;
//			metierData->getParam(WCHDATA_GameTaskMap,param);
//			metierData->excHandle(MAKEINT64(WCH_LockData,1));
//			GameTaskMap* gameTaskMap = (GameTaskMap* )param;
//			GameTaskMap::iterator itr = gameTaskMap->find(taskid);
//			gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
//			metierData->excHandle(MAKEINT64(WCH_LockData,0));
//			if(gameTask.valid() && /*gameTask->getActivation() && */!gameTask->isComplete())
//			{
//				AwardItemMap &awardItemMap = gameTask->getAwardItemMap();
//				int i = 1;
//				for( AwardItemMap::iterator itr = awardItemMap.begin();
//					itr != awardItemMap.end()&&i<5;
//					++itr,i++ )
//				{
//					if(i == m_id)
//					{
//						if(itr->second.valid())
//						{
//							crInstanceItem *item = itr->second.get();
//							crData *itemData = item->getDataClass();
//							void *param;
//							itemData->getParam(WCHDATA_ItemInfoText,param);
//							std::string file = *(std::string *)param;
//							if(!file.empty())
//								file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
//							widget->setHypertext(file);
//							itemData->getParam(WCHDATA_ItemPrice,param);
//							int price = *(int*)param;
//							price *= crGlobalHandle::gData()->gSalerate();
//							char tmpText[64];
//							ItemMap itemMap;
//							rcfg::ConfigScript cfg_script(&itemMap);
//							cfg_script.Add("Hypertext");
//							cfg_script.Push("Hypertext");
//							cfg_script.Add("Content");
//							cfg_script.Push("Content");
//							std::vector<float>v_i;
//							v_i.push_back(255.0f);
//							v_i.push_back(255.0f);
//							v_i.push_back(0.0f);
//							v_i.push_back(255.0f);
//							cfg_script.Add("Color",v_i);
//							sprintf(tmpText,"回收价：%d\0",price);
//							cfg_script.Add("Text",tmpText);
//							cfg_script.Pop();
//							cfg_script.Pop();
//							widget->setHypertext(cfg_script,true);
//							crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
//							canvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
//							canvas->setCanFocus(false);
//							crFilterRenderManager::getInstance()->showCanvas(canvas,true);
//						}
//						break;
//					}
//				}
//			}
//		}
//	} while (0);
//}
////////////////////////////////
//
//crUIShowTask2Method
//
///////////////////////////////////
crUIShowTask2Method::crUIShowTask2Method():
m_taskid(0)
{
}
crUIShowTask2Method::crUIShowTask2Method(const crUIShowTask2Method& handle):
crMethod(handle),
m_taskid(handle.m_taskid)
{
}

void crUIShowTask2Method::inputParam(int i, void *param)
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
		m_this = (crListBoxWidgetNode*)param;
		break;
	}
}

void crUIShowTask2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = atoi(str.c_str());
		break;
	case 1:
		m_canvas = str;
		break;
	}
}

void crUIShowTask2Method::operator()(crHandle &handle)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	if(!me) return;
	if(m_taskid != 0)
	{
		if(m_taskid == -1)
		{
			void *param;
			crData *metierData = me->getMetierDataClass();
			metierData->getParam(WCHDATA_CurrentTaskID,param);
			int taskid = *(int *)param;
			me->doEvent(WCH_UIShowTask,MAKEINT64(taskid,&m_canvas));
		}
		else
			me->doEvent(WCH_UIShowTask,MAKEINT64(m_taskid,&m_canvas));
	}
	else if(m_this)
	{
		do 
		{
			std::wstring curData;
			if(!m_this->getData(m_this->getSelect(),curData) || curData.empty())
				break;
			int taskid = _wtoi(curData.c_str());
			if(taskid == 0)
				break;
			me->doEvent(WCH_UIShowTask,MAKEINT64(taskid,&m_canvas));
		} while (0);
	}
}
////////////////////////////////
//
//crSetCurrentTaskMethod
//
///////////////////////////////////
crSetCurrentTaskMethod::crSetCurrentTaskMethod():
m_taskid(0)
{
}
crSetCurrentTaskMethod::crSetCurrentTaskMethod(const crSetCurrentTaskMethod& handle):
crMethod(handle),
m_taskid(handle.m_taskid)
{
}

void crSetCurrentTaskMethod::inputParam(int i, void *param)
{
}

void crSetCurrentTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = atoi(str.c_str());
		break;
	}
}

void crSetCurrentTaskMethod::operator()(crHandle &handle)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	crData *metierData = me->getMetierDataClass();
	metierData->inputParam(WCHDATA_CurrentTaskID,&m_taskid);
}
////////////////////////////////
//
//cr3DUINotifyMethod
//
///////////////////////////////////
cr3DUINotifyMethod::cr3DUINotifyMethod()
{
}
cr3DUINotifyMethod::cr3DUINotifyMethod(const cr3DUINotifyMethod& handle):
crMethod(handle),
m_notice(handle.m_notice)
{
}

void cr3DUINotifyMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			std::string notice = *(std::string *)(LOINT64(param64));
			crTextAttrWidgetNode::parseText(notice);
			m_notice = CRIOManager::convertUTF8toUTF16(notice);
		}
		break;
	}
}

void cr3DUINotifyMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
	{
		std::string notice = str;
		crTextAttrWidgetNode::parseText(notice);
		m_notice = CRIOManager::convertUTF8toUTF16(notice);
	}
}

void cr3DUINotifyMethod::operator()(crHandle &handle)
{
	CRText::crText *noticeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("Notify"));
	if(noticeText)
	{
		noticeText->setText(m_notice.c_str());
		noticeText->setVisiable(true);
	}
}
////////////////////////////////
//
//cr3DUINotify2Method
//
///////////////////////////////////
cr3DUINotify2Method::cr3DUINotify2Method()
{
}
cr3DUINotify2Method::cr3DUINotify2Method(const cr3DUINotify2Method& handle):
crMethod(handle),
m_notice(handle.m_notice)
{
}

void cr3DUINotify2Method::inputParam(int i, void *param)
{
}

void cr3DUINotify2Method::addParam(int i, const std::string& str)
{
	if(i == 0)
	{
		std::string notice = str;
		crTextAttrWidgetNode::parseText(notice);
		m_notice = CRIOManager::convertUTF8toUTF16(notice);
	}
}

void cr3DUINotify2Method::operator()(crHandle &handle)
{
	CRText::crText *noticeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("Notify"));
	if(noticeText)
	{
		noticeText->setText(m_notice.c_str());
		noticeText->setVisiable(true);
	}
}
////////////////////////////////
//
//crUIListControlTipInfoMethod
//
///////////////////////////////////
crUIListControlTipInfoMethod::crUIListControlTipInfoMethod():
m_emporium(false)
{
}
crUIListControlTipInfoMethod::crUIListControlTipInfoMethod(const crUIListControlTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName),
m_emporium(handle.m_emporium)
{
}

void crUIListControlTipInfoMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			//m_selectIndex = LOINT64(param64);
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			//m_selectIndex = -1;
			m_ea = NULL;
		}
		break;
	}
}

void crUIListControlTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	case 2:
		m_emporium = atoi(str.c_str());
		break;
	}
}

void crUIListControlTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		if(!m_this)
			break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas.valid())
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget.valid())
			break;
		//if(m_selectIndex == -1)
		//{
		//	crFilterRenderManager::getInstance()->showCanvas(canvas,false);
		//	break;
		//}
		ref_ptr<crListControlNode> listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
			break;
		}
		ref_ptr<crInstanceItem> targetItem;
		if(m_emporium)
		{
			targetItem = crMyPlayerData::getInstance()->getScene()->getEmporium();
		}
		else
		{
			ref_ptr<crRole> me = crMyPlayerData::getInstance()->getCurrentRole();
			ref_ptr<crMatrixTransform> targetNode;
			me->clientGetTarget(targetItem,targetNode);
		}
		if(targetItem.valid())
		{
			crVector3i* saleItemData = (crVector3i *)(listNode->getData());
			if(!saleItemData)
				break;
			ref_ptr<crItemChild> itemchild = targetItem->findChildItem((*saleItemData)[0]);
			if(itemchild.valid() && itemchild->isLoaded())
			{
				crInstanceItem *item = itemchild->getInstanceItem();
				crData *itemData = item->getDataClass();
				void *param;
				itemData->getParam(WCHDATA_ItemInfoText,param);
				std::string file = *(std::string *)param;
				if(!file.empty())
					file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
				widget->setHypertext(file);
				char tmpText[64];
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				std::vector<float>v_i;
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				if((*saleItemData)[1] == -1)
					sprintf(tmpText,"剩余数量：无穷 单价：%d\0",(int)((*saleItemData)[2]));
				else
					sprintf(tmpText,"剩余数量：%d 单价：%d\0",(int)((*saleItemData)[1]),(int)((*saleItemData)[2]));
				cfg_script.Add("Text",tmpText);
				cfg_script.Pop();
				cfg_script.Pop();
				widget->setHypertext(cfg_script,true);
				crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()/*+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset()*/);
				canvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
				canvas->setCanFocus(false);
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
			}
		}
	} while (0);
}
////////////////////////////////
//
//crUIItemListTipInfoMethod
//
///////////////////////////////////
crUIItemListTipInfoMethod::crUIItemListTipInfoMethod()
{
}
crUIItemListTipInfoMethod::crUIItemListTipInfoMethod(const crUIItemListTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName)
{
}

void crUIItemListTipInfoMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			//m_selectIndex = LOINT64(param64);
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			//m_selectIndex = -1;
			m_ea = NULL;
		}
		break;
	}
}

void crUIItemListTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	}
}

void crUIItemListTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		if(!m_this)
			break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas)
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget)
			break;
		//if(m_selectIndex == -1)
		//{
		//	crFilterRenderManager::getInstance()->showCanvas(canvas,false);
		//	break;
		//}
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
			break;
		}
		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
		if(me)
		{
			crInstanceItem *item = (crInstanceItem *)(listNode->getData());
			if(!item)
				break;
			crData *itemData = item->getDataClass();
			void *param;
			itemData->getParam(WCHDATA_ItemInfoText,param);
			std::string file = *(std::string *)param;
			if(!file.empty())
				file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
			widget->setHypertext(file);
			itemData->getParam(WCHDATA_ItemPrice,param);
			int price = *(int*)param;
			price *= crGlobalHandle::gData()->gSalerate();
			char tmpText[64];
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			std::vector<float>v_i;
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			sprintf(tmpText,"回收价：%d\0",price);
			cfg_script.Add("Text",tmpText);
			cfg_script.Pop();
			cfg_script.Pop();
			widget->setHypertext(cfg_script,true);
			crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()/*+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset()*/);
			canvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			canvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		}
	} while (0);
}
/////////////////////////////////////////
//
//crUIUpdateRole2Method
//
/////////////////////////////////////////
crUIUpdateRole2Method::crUIUpdateRole2Method(){}
crUIUpdateRole2Method::crUIUpdateRole2Method(const crUIUpdateRole2Method& handle):
crMethod(handle),
m_name(handle.m_name),
m_lv(handle.m_lv),
m_metier(handle.m_metier),
m_exp(handle.m_exp),
m_gold(handle.m_gold),
m_hp(handle.m_hp),
m_mp(handle.m_mp),
m_attack(handle.m_attack),
m_defend(handle.m_defend),
m_strength(handle.m_strength),
m_agility(handle.m_agility),
m_intellect(handle.m_intellect),
m_attrpoint(handle.m_attrpoint),
m_skillpoint(handle.m_skillpoint),
m_strengthL(handle.m_strengthL),
m_strengthR(handle.m_strengthR),
m_agilityL(handle.m_agilityL),
m_agilityR(handle.m_agilityR),
m_intellectL(handle.m_intellectL),
m_intellectR(handle.m_intellectR),
m_attrok(handle.m_attrok)
{
}
void crUIUpdateRole2Method::inputParam(int i, void *param)
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
void crUIUpdateRole2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_name = str;
		break;
	case 1:
		m_lv = str;
		break;
	case 2:
		m_metier = str;
		break;
	case 3:
		m_exp = str;
		break;
	case 4:
		m_gold = str;
		break;
	case 5:
		m_hp = str;
		break;
	case 6:
		m_mp = str;
		break;
	case 7:
		m_attack = str;
		break;
	case 8:
		m_defend = str;
		break;
	case 9:
		m_strength = str;
		break;
	case 10:
		m_agility = str;
		break;
	case 11:
		m_intellect = str;
		break;
	case 12:
		m_attrpoint = str;
		break;
	case 13:
		m_skillpoint = str;
		break;
	case 14:
		m_strengthL = str;
		break;
	case 15:
		m_strengthR = str;
		break;
	case 16:
		m_agilityL = str;
		break;
	case 17:
		m_agilityR = str;
		break;
	case 18:
		m_intellectL = str;
		break;
	case 19:
		m_intellectR = str;
		break;
	case 20:
		m_attrok = str;
		break;
	}
}
void crUIUpdateRole2Method::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		ref_ptr<crRole> role = crMyPlayerData::getInstance()->getCurrentRole();
		if(!role.valid()) return;
		crStaticTextWidgetNode *nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_name));
		crHypertextWidgetNode *lvWidge = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_lv));
		crStaticTextWidgetNode *metierWidge = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_metier));
		crStaticTextWidgetNode *expWidge = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_exp));
		crStaticTextWidgetNode *goldWidge = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_gold));
		crStaticTextWidgetNode *hpWidge = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_hp));
		crStaticTextWidgetNode *mpWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mp));
		crHypertextWidgetNode *attackWidge = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_attack));
		crHypertextWidgetNode *defendWidge = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_defend));

		std::string str;
		void *param;
		crData *roleData = role->getDataClass();
		char tmpText[64];
		roleData->excHandle(MAKEINT64(WCH_LockData,1));
		roleData->getParam(WCHDATA_EquipOnMap,param);
		crItemChild *itemChild;
		float equip = 0;
		float equipStrengthSum = 0;
		float equipAgilitySum = 0;
		float equipIntellectSum = 0;
		float equipLvSum = 0;
		float equipAttackSum = 0;
		float equipDefendSum = 0;
		EquipOnMap* equipOnMap = (EquipOnMap*)param;
		crInstanceItem *item;
		for( EquipOnMap::iterator itr = equipOnMap->begin();
			itr != equipOnMap->end();
			++itr )
		{
			itemChild = role->findChildItem(itr->second);
			if(itemChild && itemChild->isLoaded())
			{
				item = itemChild->getInstanceItem();
				equip = 0;
				item->doEvent(WCH_GetEquipStrength,MAKEINT64(m_this,&equip));
				equipStrengthSum+=equip;
				equip = 0;
				item->doEvent(WCH_GetEquipAgility,MAKEINT64(m_this,&equip));
				equipAgilitySum+=equip;
				equip = 0;
				item->doEvent(WCH_GetEquipIntellect,MAKEINT64(m_this,&equip));
				equipIntellectSum+=equip;
				equip = 0;
				item->doEvent(WCH_GetEquipLv,MAKEINT64(m_this,&equip));
				equipLvSum+=equip;
			}
		}
		roleData->excHandle(MAKEINT64(WCH_LockData,0));

		if(nameWidget)
		{
			str = role->getIName();
			nameWidget->setString(str);
		}
		if(lvWidge)
		{
			roleData->getParam(WCHDATA_Level,param);
			unsigned char lv = *((unsigned char*)param);
			str = crArgumentParser::appItoa((int)lv);
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",str);
			cfg_script.Pop();
			if(equipLvSum!=0)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content",2);
				std::vector<float>v_i;
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				if(equipLvSum>0)
					sprintf(tmpText,"+%d\0",(int)equipLvSum);
				else
					sprintf(tmpText,"%d\0",(int)equipLvSum);
				cfg_script.Add("Text",tmpText);
				cfg_script.Pop();
			}
			cfg_script.Pop();
			lvWidge->setHypertext(cfg_script);
		}
		if(metierWidge && role->getAbstractMetier())
		{
			str = role->getAbstractMetier()->getMetierName();
			metierWidge->setString(str);
		}
		if(expWidge)
		{
			roleData->getParam(WCHDATA_Experience,param);
			int exp = *((int*)param);
			int nextexp = exp;
			role->doEvent(WCH_GetNextLvExp,MAKEINT64(&nextexp,NULL));
			str = crArgumentParser::appItoa(exp) + "/" + crArgumentParser::appItoa(nextexp);
			expWidge->setString(str);
			//str = crArgumentParser::appItoa(nextexp);
			//nextExpWidge->setString(str);
		}
		if(goldWidge)
		{
			roleData->getParam(WCHDATA_Gold,param);
			int curGold = *(int *)param;
			str = crArgumentParser::appItoa(curGold);
			goldWidge->setString(str);
		}
		if(hpWidge)
		{
			float maxhp = 0;
			role->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			str = crArgumentParser::appItoa(rthp) + "/" + crArgumentParser::appItoa(maxhp);
			hpWidge->setString(str);
		}
		if(mpWidget)
		{
			float maxmp = 0;
			role->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&maxmp,NULL));
			roleData->getParam(WCHDATA_RTMP,param);
			float rtmp = *(float*)param;
			str = crArgumentParser::appItoa(rtmp) + "/" + crArgumentParser::appItoa(maxmp);
			mpWidget->setString(str);
		}
		if(attackWidge)
		{
			float attack = 0;
			role->doEvent(MAKEINT64(WCH_GetDamage,NULL),MAKEINT64(&attack,NULL));
			str = crArgumentParser::appItoa(attack);
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",str);
			cfg_script.Pop();
			if(equipAttackSum!=0)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content",2);
				std::vector<float>v_i;
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				if(equipAttackSum>0)
					sprintf(tmpText,"+%d\0",(int)equipAttackSum);
				else
					sprintf(tmpText,"%d\0",(int)equipAttackSum);
				cfg_script.Add("Text",tmpText);
				cfg_script.Pop();
			}
			cfg_script.Pop();
			attackWidge->setHypertext(cfg_script);
		}
		if(defendWidge)
		{
			float defend = 0;
			role->doEvent(MAKEINT64(WCH_GetDefend,NULL),MAKEINT64(&defend,NULL));
			str = crArgumentParser::appItoa(defend);
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",str);
			cfg_script.Pop();
			if(equipDefendSum!=0)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content",2);
				std::vector<float>v_i;
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				if(equipDefendSum>0)
					sprintf(tmpText,"+%d\0",(int)equipDefendSum);
				else
					sprintf(tmpText,"%d\0",(int)equipDefendSum);
				cfg_script.Add("Text",tmpText);
				cfg_script.Pop();
			}
			cfg_script.Pop();
			defendWidge->setHypertext(cfg_script);
		}
		crStaticTextWidgetNode *skillpointWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_skillpoint));
		if(skillpointWidget)
		{
			roleData->getParam(WCHDATA_SkillPoint,param);
			unsigned short skillpoint = *((unsigned short*)param);
			str = crArgumentParser::appItoa((int)skillpoint);
			skillpointWidget->setString(str);
		}
		crButtonWidgetNode *attrokWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_attrok));
		crButtonWidgetNode *strengthLWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strengthL));
		crButtonWidgetNode *strengthRWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strengthR));
		crButtonWidgetNode *agilityLWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_agilityL));
		crButtonWidgetNode *agilityRWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_agilityR));
		crButtonWidgetNode *intellectLWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_intellectL));
		crButtonWidgetNode *intellectRWidget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_intellectR));
		crStaticTextWidgetNode *attrpointWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_attrpoint));
		if(!attrokWidget->getVisiable())
		{
			roleData->getParam(WCHDATA_AttrPoint,param);
			unsigned short attrpoint = *((unsigned short*)param);
			str = crArgumentParser::appItoa((int)attrpoint);
			attrpointWidget->setString(str);
			if(attrpoint > 0)
			{
				strengthLWidget->setVisiable(true);
				strengthRWidget->setVisiable(true);
				agilityLWidget->setVisiable(true);
				agilityRWidget->setVisiable(true);
				intellectLWidget->setVisiable(true);
				intellectRWidget->setVisiable(true);
			}
			else
			{
				strengthLWidget->setVisiable(false);
				strengthRWidget->setVisiable(false);
				agilityLWidget->setVisiable(false);
				agilityRWidget->setVisiable(false);
				intellectLWidget->setVisiable(false);
				intellectRWidget->setVisiable(false);
			}

			crHypertextWidgetNode *strengthWidget = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strength));
			crHypertextWidgetNode *agilityWidget = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_agility));
			crHypertextWidgetNode *intellectWidget = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_intellect));
			if(strengthWidget)
			{
				roleData->getParam(WCHDATA_Strength,param);
				short strength = *((short*)param);
				str = crArgumentParser::appItoa(strength);
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text",str);
				cfg_script.Pop();
				if(equipStrengthSum!=0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					std::vector<float>v_i;
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					if(equipStrengthSum>0)
						sprintf(tmpText,"+%d\0",(int)equipStrengthSum);
					else
						sprintf(tmpText,"%d\0",(int)equipStrengthSum);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();
				}
				cfg_script.Pop();
				strengthWidget->setHypertext(cfg_script);
			}
			if(agilityWidget)
			{
				roleData->getParam(WCHDATA_Agility,param);
				short agility = *((short*)param);
				str = crArgumentParser::appItoa(agility);
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text",str);
				cfg_script.Pop();
				if(equipAgilitySum!=0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					std::vector<float>v_i;
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					if(equipAgilitySum>0)
						sprintf(tmpText,"+%d\0",(int)equipAgilitySum);
					else
						sprintf(tmpText,"%d\0",(int)equipAgilitySum);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();
				}
				cfg_script.Pop();
				agilityWidget->setHypertext(cfg_script);
			}
			if(intellectWidget)
			{
				roleData->getParam(WCHDATA_Intellect,param);
				short intellect = *((short*)param);
				str = crArgumentParser::appItoa(intellect);
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text",str);
				cfg_script.Pop();
				if(equipIntellectSum!=0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					std::vector<float>v_i;
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					if(equipIntellectSum>0)
						sprintf(tmpText,"+%d\0",(int)equipIntellectSum);
					else
						sprintf(tmpText,"%d\0",(int)equipIntellectSum);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();
				}
				cfg_script.Pop();
				intellectWidget->setHypertext(cfg_script);
			}
		}
	}
}
/////////////////////////////////////////
//
//crUIUpdateExpProgressMethod
//
/////////////////////////////////////////
crUIUpdateExpProgressMethod::crUIUpdateExpProgressMethod()
{
}
crUIUpdateExpProgressMethod::crUIUpdateExpProgressMethod(const crUIUpdateExpProgressMethod& handle):
crMethod(handle)
{
}
void crUIUpdateExpProgressMethod::inputParam(int i, void *param)
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
void crUIUpdateExpProgressMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_expprogress = str;
		break;
	}
}
void crUIUpdateExpProgressMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
		if(!role) return;
		crProgressWidgetNode *progressWidget = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_expprogress));
		std::string str;
		void *param;
		crData *roleData = role->getDataClass();
		roleData->getParam(WCHDATA_Experience,param);
		int exp = *((int*)param);
		roleData->getParam(WCHDATA_Level,param);
		unsigned char* lv = (unsigned char *)param;
		(*lv)--;
		int lvexp;
		role->doEvent(WCH_GetNextLvExp,MAKEINT64(&lvexp,NULL));
		(*lv)++;
		int nextexp = exp;
		role->doEvent(WCH_GetNextLvExp,MAKEINT64(&nextexp,NULL));
		progressWidget->setProgress((float)(exp-lvexp)/(float)(nextexp-lvexp));
		//progressWidget->setProgress(0.1f);
	}
}
///////////////////////////////////////////
////
////crBeginDragItemMethod
////
///////////////////////////////////////////
//crBeginDragItemMethod::crBeginDragItemMethod()
//{
//}
//crBeginDragItemMethod::crBeginDragItemMethod(const crBeginDragItemMethod& handle):
//crMethod(handle)
//{
//}
//void crBeginDragItemMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crListControlWidgetNode*)param;
//		break;
//	}
//}
//void crBeginDragItemMethod::addParam(int i, const std::string& str)
//{
//}
//void crBeginDragItemMethod::operator()(crHandle &handle)
//{
//	if(m_this)
//	{
//		crListControlNode *listNode = m_this->getSelectNode();
//		if(listNode)
//		{
//			CRCore::crObject *imgObject = listNode->getImageObject();
//			crMatrixTransform *dragNode = new crMatrixTransform;
//			dragNode->setName("DragNode");
//			dragNode->addChild(imgObject);
//		}
//	}
//}
/////////////////////////////////////////
//
//crBuildTaskListMethod
//
/////////////////////////////////////////
crBuildTaskListMethod::crBuildTaskListMethod(){}
crBuildTaskListMethod::crBuildTaskListMethod(const crBuildTaskListMethod& handle):
crMethod(handle),
m_canvasName(handle.m_canvasName),
m_listName(handle.m_listName)
{
}
void crBuildTaskListMethod::inputParam(int i, void *param)
{
}
void crBuildTaskListMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_listName = str;
		break;
	}
}
void crBuildTaskListMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	if(canvasNode.valid())
	{
		ref_ptr<crListBoxWidgetNode> taskList = dynamic_cast<crListBoxWidgetNode *>(canvasNode->getWidget(m_listName));
		if(taskList.valid())
		{
			taskList->clearData();
			crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
			crData *myMetierData = me->getMetierDataClass();
			void *param;
			myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
			myMetierData->getParam(WCHDATA_GameTaskMap,param);
			GameTaskMap *gameTaskMap = (GameTaskMap *)param;
			if(!gameTaskMap||gameTaskMap->empty())
			{
				myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
				return;
			}
			std::string str;
			for( GameTaskMap::iterator itr = gameTaskMap->begin();
				 itr != gameTaskMap->end();
				 ++itr )
			{
				str = crArgumentParser::appItoa(itr->second->getTaskID())+" "+itr->second->getTaskName();
				if(!itr->second->getActivation())
					str += "(未接)";
				else if(itr->second->isComplete())
				{
					str += "(已完成)";
				}
				else
				{
					str += "(进行中)";
				}
				taskList->addData(str);
			}
			myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crUIUpdateMyInfoMethod
//
/////////////////////////////////////////
crUIUpdateMyInfoMethod::crUIUpdateMyInfoMethod(){}
crUIUpdateMyInfoMethod::crUIUpdateMyInfoMethod(const crUIUpdateMyInfoMethod& handle):
crMethod(handle),
m_headImg(handle.m_headImg),
m_lv(handle.m_lv),
m_stats(handle.m_stats),
m_name(handle.m_name),
m_hpProgress(handle.m_hpProgress),
m_mpProgress(handle.m_mpProgress),
m_stateBufList(handle.m_stateBufList)
{
}
void crUIUpdateMyInfoMethod::inputParam(int i, void *param)
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
void crUIUpdateMyInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_headImg = str;
		break;
	case 1:
		m_lv = str;
		break;
	case 2:
		m_stats = str;
		break;
	case 3:
		m_name = str;
		break;
	case 4:
		m_hpProgress = str;
		break;
	case 5:
		m_mpProgress = str;
		break;
	case 6:
		m_stateBufList = str;
		break;
	}
}
void crUIUpdateMyInfoMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
		if(!role) return;
		crImageBoxWidgetNode *headImageWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_headImg));
		crStaticTextWidgetNode *lvWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_lv));
		crStaticTextWidgetNode *statsWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_stats));
		crStaticTextWidgetNode *nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_name));
		crProgressWidgetNode *hpProgressWidget = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_hpProgress));
		crProgressWidgetNode *mpProgressWidget = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_mpProgress));
		crListControlWidgetNode *stateBufListWidget = dynamic_cast<crListControlWidgetNode *>(m_this->getWidget(m_stateBufList));

		std::string str;
		void *param;
		crData *roleData = role->getDataClass();
		roleData->excHandle(MAKEINT64(WCH_LockData,1));
		roleData->getParam(WCHDATA_EquipOnMap,param);
		crItemChild *itemChild;
		float equip = 0;
		float equipLvSum = 0;
		EquipOnMap* equipOnMap = (EquipOnMap*)param;
		crInstanceItem *item;
		for( EquipOnMap::iterator itr = equipOnMap->begin();
			itr != equipOnMap->end();
			++itr )
		{
			itemChild = role->findChildItem(itr->second);
			if(itemChild && itemChild->isLoaded())
			{
				item = itemChild->getInstanceItem();
				equip = 0;
				item->doEvent(WCH_GetEquipLv,MAKEINT64(m_this,&equip));
				equipLvSum+=equip;
			}
		}
		roleData->excHandle(MAKEINT64(WCH_LockData,0));
		
		if(lvWidget)
		{
			roleData->getParam(WCHDATA_Level,param);
			unsigned char lv = *((unsigned char*)param);
			str = crArgumentParser::appItoa((int)lv + (int)equipLvSum);
			lvWidget->setString(str);
		}
		if(headImageWidget)
		{//
			roleData->getParam(WCHDATA_ItemIcon,param);
			std::string iconFile = *(std::string *)param;
			headImageWidget->setImageName(iconFile);
		}
		if(statsWidget)
		{//FightMode
			roleData->getParam(WCHDATA_FightMode,param);
			unsigned char fightMode = *(unsigned char *)param;
			switch(fightMode)
			{
			case FM_Peace:
				str = "和";
				break;
			case FM_Group:
				str = "组";
				break;
			case FM_All:
				str = "全";
				break;
			case FM_Defence:
				str = "自";
				break;
			}
			statsWidget->setString(str);
		}
		if(nameWidget)
		{
			str = role->getIName();
			nameWidget->setString(str);
		}
		if(hpProgressWidget)
		{
			float maxhp = 0;
			role->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			hpProgressWidget->setProgress(rthp/maxhp);
			//str = crArgumentParser::appItoa(rthp) + "/" + crArgumentParser::appItoa(maxhp);
		}
		if(mpProgressWidget)
		{
			float maxmp = 0;
			role->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&maxmp,NULL));
			roleData->getParam(WCHDATA_RTMP,param);
			float rtmp = *(float*)param;
			mpProgressWidget->setProgress(rtmp/maxmp);
			//str = crArgumentParser::appItoa(rtmp) + "/" + crArgumentParser::appItoa(maxmp);
			//mpWidget->setString(str);
		}
		if(stateBufListWidget)
		{
		}
	}
}
/////////////////////////////////////////
//
//crUIUpdateTargetInfoMethod
//
/////////////////////////////////////////
crUIUpdateTargetInfoMethod::crUIUpdateTargetInfoMethod(){}
crUIUpdateTargetInfoMethod::crUIUpdateTargetInfoMethod(const crUIUpdateTargetInfoMethod& handle):
crMethod(handle),
m_headImg(handle.m_headImg),
m_lv(handle.m_lv),
m_name(handle.m_name),
m_metier(handle.m_metier),
m_hpProgress(handle.m_hpProgress)
{
}
void crUIUpdateTargetInfoMethod::inputParam(int i, void *param)
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
void crUIUpdateTargetInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_headImg = str;
		break;
	case 1:
		m_lv = str;
		break;
	case 2:
		m_name = str;
		break;
	case 3:
		m_metier = str;
		break;
	case 4:
		m_hpProgress = str;
		break;
	}
}
void crUIUpdateTargetInfoMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
		if(!role) return;
		crImageBoxWidgetNode *headImageWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_headImg));
		crStaticTextWidgetNode *lvWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_lv));
		crStaticTextWidgetNode *nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_name));
		crStaticTextWidgetNode *metierWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_metier));
		crProgressWidgetNode *hpProgressWidget = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_hpProgress));

		ref_ptr<crInstanceItem> targetItem;
		ref_ptr<crMatrixTransform> targetNode;
		role->clientGetTarget(targetItem,targetNode);
		if(targetItem.valid())
		{
			void *param;
			std::string str;
			crData *itemData = targetItem->getDataClass();
			if(headImageWidget)
			{
				itemData->getParam(WCHDATA_ItemIcon,param);
				std::string iconFile = *(std::string *)param;
				headImageWidget->setImageName(iconFile);
			}
			if(nameWidget)
			{
				str = targetItem->getIName();
				nameWidget->setString(str);
			}
			if(lvWidget)
			{
				itemData->getParam(WCHDATA_Level,param);
				unsigned char* lv = (unsigned char*)param;
				if(lv)
				{
					str = crArgumentParser::appItoa((int)(*lv));
					lvWidget->setString(str);
				}
				else
				{
					lvWidget->clearString();
				}
			}
			if(metierWidget)
			{
				switch (targetItem->getItemtype())
				{
				case crInstanceItem::instanceitem:
					{
						itemData->getParam(WCHDATA_DataType,param);
						unsigned char dataType = *(unsigned char *)param;
						if(dataType == DT_NPC)
							str = "NPC";
						else
							str = "道具";
					}
					break;
				case crInstanceItem::Npc:
					str = "NPC";
					break;
				case crInstanceItem::Role:
					{
						crAbstractMetier *ametier = dynamic_cast<crRole *>(targetItem.get())->getAbstractMetier();
						if(ametier)
							str = ametier->getMetierName();
					}
					break;
				}
				metierWidget->setString(str);
			}
			if(hpProgressWidget)
			{
				if(targetItem->getItemtype()>crInstanceItem::instanceitem)
				{
					itemData->getParam(WCHDATA_RTHP,param);
					float* rthp = (float*)param;
					if(rthp)
					{
						float maxhp = 0;
						targetItem->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
						str = crArgumentParser::appItoa(*rthp) + "/" + crArgumentParser::appItoa(maxhp);
						hpProgressWidget->setProgress((*rthp)/maxhp);
					}
					else
					{
						hpProgressWidget->setProgress(1.0f);
					}
				}
				else
				{
					hpProgressWidget->setProgress(1.0f);
				}
			}
		}
	}
}
////////////////////////////////
//
//crMyHPTipInfoMethod
//
///////////////////////////////////
crMyHPTipInfoMethod::crMyHPTipInfoMethod()
{
}
crMyHPTipInfoMethod::crMyHPTipInfoMethod(const crMyHPTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName),
m_coord(handle.m_coord)
{
}

void crMyHPTipInfoMethod::inputParam(int i, void *param)
{
}

void crMyHPTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_coord);
		break;
	}
}

void crMyHPTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas)
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget)
			break;
		ref_ptr<crRole> me = crMyPlayerData::getInstance()->getCurrentRole();
		if(me.valid())
		{
			void *param;
			float maxhp = 0;
			me->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
			crData *data = me->getDataClass();
			data->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			std::string str = crArgumentParser::appItoa(rthp) + "/" + crArgumentParser::appItoa(maxhp);

			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			std::vector<float>v_i;
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",str);
			cfg_script.Pop();
			cfg_script.Pop();
			widget->setHypertext(cfg_script);
			//crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset());
			canvas->setMatrix(crMatrix::translate(m_coord[0],m_coord[1],0.0f));
			canvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		}
	} while (0);
}
////////////////////////////////
//
//crMyMPTipInfoMethod
//
///////////////////////////////////
crMyMPTipInfoMethod::crMyMPTipInfoMethod()
{
}
crMyMPTipInfoMethod::crMyMPTipInfoMethod(const crMyMPTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName),
m_coord(handle.m_coord)
{
}

void crMyMPTipInfoMethod::inputParam(int i, void *param)
{
}

void crMyMPTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_coord);
		break;
	}
}

void crMyMPTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas)
			break;
		ref_ptr<crHypertextWidgetNode>widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget)
			break;
		ref_ptr<crRole> me = crMyPlayerData::getInstance()->getCurrentRole();
		if(me.valid())
		{
			void *param;
			float maxmp = 0;
			me->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&maxmp,NULL));
			crData *data = me->getDataClass();
			data->getParam(WCHDATA_RTMP,param);
			float rtmp = *(float*)param;
			std::string str = crArgumentParser::appItoa(rtmp) + "/" + crArgumentParser::appItoa(maxmp);

			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			std::vector<float>v_i;
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",str);
			cfg_script.Pop();
			cfg_script.Pop();
			widget->setHypertext(cfg_script);
			//crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset());
			canvas->setMatrix(crMatrix::translate(m_coord[0],m_coord[1],0.0f));
			canvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		}
	} while (0);
}
////////////////////////////////
//
//crTargetHPTipInfoMethod
//
///////////////////////////////////
crTargetHPTipInfoMethod::crTargetHPTipInfoMethod()
{
}
crTargetHPTipInfoMethod::crTargetHPTipInfoMethod(const crTargetHPTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName),
m_coord(handle.m_coord)
{
}

void crTargetHPTipInfoMethod::inputParam(int i, void *param)
{
}

void crTargetHPTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_coord);
		break;
	}
}

void crTargetHPTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas)
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget)
			break;
		ref_ptr<crRole> me = crMyPlayerData::getInstance()->getCurrentRole();
		if(!me)
			break;
		ref_ptr<crInstanceItem> targetItem;
		ref_ptr<crMatrixTransform> targetNode;
		me->clientGetTarget(targetItem,targetNode);
		if(targetItem.valid())
		{
			if(targetItem->getItemtype()>crInstanceItem::instanceitem)
			{
				void *param;
				crData *itemData = targetItem->getDataClass();
				itemData->getParam(WCHDATA_RTHP,param);
				float* rthp = (float*)param;
				if(rthp)
				{
					float maxhp = 0;
					targetItem->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
					std::string str = crArgumentParser::appItoa(*rthp) + "/" + crArgumentParser::appItoa(maxhp);

					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					cfg_script.Add("Content");
					cfg_script.Push("Content");
					std::vector<float>v_i;
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",str);
					cfg_script.Pop();
					cfg_script.Pop();
					widget->setHypertext(cfg_script);
					//crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset());
					canvas->setMatrix(crMatrix::translate(m_coord[0],m_coord[1],0.0f));
					canvas->setCanFocus(false);
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	} while (0);
}
////////////////////////////////
//
//crTargetMPTipInfoMethod
//
///////////////////////////////////
crTargetMPTipInfoMethod::crTargetMPTipInfoMethod()
{
}
crTargetMPTipInfoMethod::crTargetMPTipInfoMethod(const crTargetMPTipInfoMethod& handle):
crMethod(handle),
m_tipCanvasName(handle.m_tipCanvasName),
m_tipWidgetName(handle.m_tipWidgetName),
m_coord(handle.m_coord)
{
}

void crTargetMPTipInfoMethod::inputParam(int i, void *param)
{
}

void crTargetMPTipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipCanvasName = str;
		break;
	case 1:
		m_tipWidgetName = str;
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_coord);
		break;
	}
}

void crTargetMPTipInfoMethod::operator()(crHandle &handle)
{
	do 
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvasName);
		if(!canvas)
			break;
		ref_ptr<crHypertextWidgetNode> widget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_tipWidgetName));
		if(!widget)
			break;
		ref_ptr<crRole> me = crMyPlayerData::getInstance()->getCurrentRole();
		if(!me)
			break;
		ref_ptr<crInstanceItem> targetItem;
		ref_ptr<crMatrixTransform> targetNode;
		me->clientGetTarget(targetItem,targetNode);
		if(targetItem.valid())
		{
			if(targetItem->getItemtype()>crInstanceItem::instanceitem)
			{
				void *param;
				crData *itemData = targetItem->getDataClass();
				itemData->getParam(WCHDATA_RTMP,param);
				float* rtmp = (float*)param;
				if(rtmp)
				{
					float maxmp = 0;
					targetItem->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&maxmp,NULL));
					std::string str = crArgumentParser::appItoa(*rtmp) + "/" + crArgumentParser::appItoa(maxmp);

					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					cfg_script.Add("Content");
					cfg_script.Push("Content");
					std::vector<float>v_i;
					v_i.push_back(0.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",str);
					cfg_script.Pop();
					cfg_script.Pop();
					widget->setHypertext(cfg_script);
					//crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()+crKeyboardMouseHandle::getInstance()->getBindViewer()->getUIMouseOffset());
					canvas->setMatrix(crMatrix::translate(m_coord[0],m_coord[1],0.0f));
					canvas->setCanFocus(false);
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	} while (0);
}
////////////////////////////////
//
//crInitSceneMapMethod
//
///////////////////////////////////
crInitSceneMapMethod::crInitSceneMapMethod():
m_scale(0.5f),
m_imageSize(256),
m_staticmap(false)
{
	m_scaleRange.set(0.1,1.0);
}
crInitSceneMapMethod::crInitSceneMapMethod(const crInitSceneMapMethod& handle):
crMethod(handle),
m_canvasName(handle.m_canvasName),
m_mapWidgetName(handle.m_mapWidgetName),
m_imageFile(handle.m_imageFile),
m_scale(handle.m_scale),
m_scaleRange(handle.m_scaleRange),
//m_myIconWidget(handle.m_myIconWidget),
m_imageSize(handle.m_imageSize),
m_staticmap(handle.m_staticmap)
{
}

void crInitSceneMapMethod::inputParam(int i, void *param)
{
}

void crInitSceneMapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_mapWidgetName = str;
		break;
	case 2:
		m_imageFile = str;
		break;
	case 3:
		m_scale = atof(str.c_str());
		break;
	case 4:
		crArgumentParser::appAtoVec(str,m_scaleRange);
		break;
	//case 5:
	//	m_myIconWidget = str;
	//	break;
	case 5:
		m_imageSize = atof(str.c_str());
		break;
	case 6:
		m_staticmap = (bool)(atoi(str.c_str()));
		break;
	}
}

void crInitSceneMapMethod::operator()(crHandle &handle)
{
	do 
	{
		if(m_imageFile.empty())
			break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
		if(!canvas)
			break;
		ref_ptr<crImageBoxWidgetNode> widget = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_mapWidgetName));
		if(!widget)
			break;
		widget->setImageName(m_imageFile);
		crStateSet *stateset = widget->getImageStateSet();
		if(!m_staticmap)
		{
			crTexMat *tm = new crTexMat;
			tm->setMatrix(crMatrix::scale(m_scale,m_scale,1.0f));
			stateset->setTextureAttribute(0, tm);
		}
		//float pxpm;
		if(m_imageFile.compare("RTT") == 0)
		{
			crTexture *rtt = crShaderManager::getInstance()->getGiTexture();
			stateset->setTextureAttributeAndModes(0,rtt,crStateAttribute::ON);
			//pxpm = (float)rtt->getTextureWidth()/(float)worldSize[0];
			//crMyPlayerData::getInstance()->setMapPxpm(pxpm);
		}
		//else
		//{
			//pxpm = m_imageSize/(float)worldSize[0];
			//crMyPlayerData::getInstance()->setMapPxpm(pxpm);
		//}
		crMyPlayerData::getInstance()->setMapScaleRange(m_scaleRange);
		crMyPlayerData::getInstance()->setMapScale(m_scale);
		const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
		const crBoundingBox &bbox = widget->getBoundBox();
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
		if(worldSize[0] > worldSize[1])
		{
			widget->setEnableMatrix(true);
			float scale = float(worldSize[1])/float(worldSize[0]);
			widget->setMatrix(crMatrix::scale(1.0f,scale,1.0f)*crMatrix::translate(0,-bbox.yLength(),0));//
		}
		else if(worldSize[0] < worldSize[1])
		{
			widget->setEnableMatrix(true);
			float scale = float(worldSize[0])/float(worldSize[1]);
			widget->setMatrix(crMatrix::scale(scale,1.0f,1.0f)*crMatrix::translate(bbox.xLength(),0,0));//
		}
		//crCanvasNode *mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
		//crWidgetNode *myIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_myIconWidget));
		//if(myIconWidget)
		//{
		//	myIconWidget = dynamic_cast<crWidgetNode *>(myIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
		//	canvas->addChild(myIconWidget);
		//	myIconWidget->setVisiable(true);
		//}
	} while (0);
}
/////////////////////////////////////////
//
//crUIUpdateMapInfoMethod
//
/////////////////////////////////////////
crUIUpdateMapInfoMethod::crUIUpdateMapInfoMethod(){}
crUIUpdateMapInfoMethod::crUIUpdateMapInfoMethod(const crUIUpdateMapInfoMethod& handle):
crMethod(handle),
m_nameWidget(handle.m_nameWidget),
m_mapWidget(handle.m_mapWidget),
m_myIconWidget(handle.m_myIconWidget),
m_friendPlayerIconWidget(handle.m_friendPlayerIconWidget),
m_threadPlayerIconWidget(handle.m_threadPlayerIconWidget),
m_enemyPlayerIconWidget(handle.m_enemyPlayerIconWidget),
m_friendNpcIconWidget(handle.m_friendNpcIconWidget),
m_threadNpcIconWidget(handle.m_threadNpcIconWidget),
m_enemyNpcIconWidget(handle.m_enemyNpcIconWidget),
m_enemyBossIconWidget(handle.m_enemyBossIconWidget),
m_itemIconWidget(handle.m_itemIconWidget)
{
}
void crUIUpdateMapInfoMethod::inputParam(int i, void *param)
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
void crUIUpdateMapInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nameWidget = str;
		break;
	case 1:
		m_mapWidget = str;
		break;
	case 2:
		m_myIconWidget = str;
		break;
	case 3:
		m_friendPlayerIconWidget = str;
		break;
	case 4:
		m_threadPlayerIconWidget = str;
		break;
	case 5:
		m_enemyPlayerIconWidget = str;
		break;
	case 6:
		m_friendNpcIconWidget = str;
		break;
	case 7:
		m_threadNpcIconWidget = str;
		break;
	case 8:
		m_enemyNpcIconWidget = str;
		break;
	case 9:
		m_enemyBossIconWidget = str;
		break;
	case 10:
		m_itemIconWidget = str;
		break;
	}
}
void crUIUpdateMapInfoMethod::operator()(crHandle &handle)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	if(me && m_this && m_this->getVisiable())
	{
		//float width = crDisplaySettings::instance()->getViewWidth();
		//float height = crDisplaySettings::instance()->getViewHeight();
		//float aspectRatio = width / height;
		//crMatrix scaleMat = crMatrix::scale(1.0f,aspectRatio,1.0f);
		//crMatrix invScaleMat = crMatrix::inverse(scaleMat);
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();

		ref_ptr<crStaticTextWidgetNode> nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_nameWidget));
		ref_ptr<crImageBoxWidgetNode> mapWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mapWidget));
		if(nameWidget.valid())
		{
			CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
			std::string str = scene->getSceneName();
			unsigned short layerid = me->getLayerID();
			if(layerid > 0)
			{
				str += ":"+crArgumentParser::appItoa(layerid);
			}
			nameWidget->setString(str);
		}
		float myX = me->getPosx() * crGlobalHandle::gData()->gUnitScale();
		float myY = me->getPosy() * crGlobalHandle::gData()->gUnitScale();
		//float texWidth = 0.0f;
		//float texHeight = 0.0f;
		float mapscale = crMyPlayerData::getInstance()->getMapScale();
		crVector2 mapWidgetSize;
		//float pxpm = crMyPlayerData::getInstance()->getMapPxpm();
		const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
		if(mapWidget.valid())
		{
			crStateSet *stateset = mapWidget->getImageStateSet();
			//crTexture2D *tex = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(0,crStateAttribute::TEXTURE));
			//if(tex)
			//{
				//pxpm = (float)rtt->getTextureWidth()/(float)worldSize[0];
				float x = 0.0f;
				float y = 0.0f;
				//texWidth = tex->getTextureWidth();
				//if(texWidth>0.0f)
					x = myX / (float)worldSize[0];//pxpm/texWidth;
				//texHeight = tex->getTextureHeight();
				//if(texHeight>0.0f)
					y = myY / (float)worldSize[1]; //* pxpm/texHeight;
				crTexMat *tm = dynamic_cast<crTexMat *>(stateset->getTextureAttribute(0,crStateAttribute::TEXMAT));
				crMatrix mat = crMatrix::scale(mapscale,mapscale,1.0f) * crMatrix::translate((1.0f-mapscale)*0.5f+x,(1.0f-mapscale)*0.5f+y,0.0f);
				tm->setMatrix(mat);
				const crBoundingBox &bbox = mapWidget->getBoundBox();
				mapWidgetSize.set(bbox.xLength(),bbox.yLength());
			//}
		}
		ref_ptr<crCanvasNode> mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
		ref_ptr<crWidgetNode> myIconWidget = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_myIconWidget));
		if(myIconWidget.valid())
		{
			crVector3 dir(me->getDirx(),me->getDiry(),me->getDirz());
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
			myIconWidget->setMatrix(invScaleMat * newMat * scaleMat * crMatrix::translate(mapWidget->getBound().center()));
		}
		crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(/*crVector2(texWidth,texHeight),*/mapWidgetSize);
		//if(texWidth>0.0f)
		//	mapFactor[0] = pxpm*mapWidgetSize[0]/(texWidth*mapscale);
		//if(texHeight>0.0f)
		//	mapFactor[1] = pxpm*mapWidgetSize[1]/(texHeight*mapscale);

		crWidgetNode *iconWidget;
		char isEnemy = 0;
		crRole *player;
		crData *playerData;
		void *param;
		int i = 0;
		int j = 0;
		int k = 0;
		int friendCount = m_friendPlayerWidgetVec.size();
		int thirdCount = m_threadPlayerWidgetVec.size();
		int enemyCount = m_enemyPlayerWidgetVec.size();
		crWidgetNode *friendIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_friendPlayerIconWidget));
		crWidgetNode *thirdIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_threadPlayerIconWidget));
		crWidgetNode *enemyIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_enemyPlayerIconWidget));
		crMyPlayerData::getInstance()->lockInRangePlayerMap();
		crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
		for( crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
			 itr != playerMap.end();
			 ++itr )
		{
			iconWidget = NULL;
			player = itr->second.first.get();
			isEnemy = 0;
			me->doEvent(WCH_EnemyCheck,MAKEINT64(player,&isEnemy));
			if(isEnemy==1)
			{
				if(i<friendCount)
				{
					iconWidget = m_friendPlayerWidgetVec[i].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(friendIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_friendPlayerWidgetVec.push_back(iconWidget);
				}
				i++;
			}
			else if(isEnemy == 0)
			{
				if(j<thirdCount)
				{
					iconWidget = m_threadPlayerWidgetVec[j].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(thirdIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_threadPlayerWidgetVec.push_back(iconWidget);
				}
				j++;
			}
			else
			{
				playerData = player->getDataClass();
				//playerData->getParam(WCHDATA_GuiseState,param);
				//unsigned char guisestate = *(unsigned char *)param;
				//if(guisestate == GS_Hide || guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable)
				//{
				//	iconWidget = NULL;
				//}
				unsigned int guisestate = GS_Normal;
				player->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
				if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
				{
					iconWidget = NULL;
				}
				else
				{
					if(k<enemyCount)
					{
						iconWidget = m_enemyPlayerWidgetVec[k].get();
					}
					else
					{
						iconWidget = dynamic_cast<crWidgetNode *>(enemyIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
						m_this->addChild(iconWidget);
						m_enemyPlayerWidgetVec.push_back(iconWidget);
					}
					k++;
				}
			}
			if(iconWidget)
			{
				iconWidget->setVisiable(true);
				crVector3 dir(player->getDirx(),player->getDiry(),player->getDirz());
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
				float px = player->getPosx()*crGlobalHandle::gData()->gUnitScale() - myX;
				float py = player->getPosy()*crGlobalHandle::gData()->gUnitScale() - myY;
				//if(texWidth>0.0f)
					px *= mapFactor[0];
				//if(texHeight>0.0f)
					py *= mapFactor[1];
				iconWidget->setMatrix(invScaleMat * newMat * scaleMat * crMatrix::translate(mapWidget->getBound().center()+crVector3(px,py,0.0f)));
			}
		}
		if(i<friendCount)
		{
			for(int ii = i; ii<friendCount; ++ii)
			{
				m_friendPlayerWidgetVec[ii]->setVisiable(false);
			}
		}
		if(j<thirdCount)
		{
			for(int ii = j; ii<thirdCount; ++ii)
			{
				m_threadPlayerWidgetVec[ii]->setVisiable(false);
			}
		}
		if(k<enemyCount)
		{
			for(int ii = k; ii<enemyCount; ++ii)
			{
				m_enemyPlayerWidgetVec[ii]->setVisiable(false);
			}
		}
		crMyPlayerData::getInstance()->unlockInRangePlayerMap();

		i = 0;
		j = 0;
		k = 0;
		friendCount = m_friendNpcWidgetVec.size();
		thirdCount = m_threadNpcWidgetVec.size();
		enemyCount = m_enemyNpcWidgetVec.size();
		friendIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_friendNpcIconWidget));
		thirdIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_threadNpcIconWidget));
		enemyIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_enemyNpcIconWidget));
		crMyPlayerData::getInstance()->lockInRangeNpcMap();
		crInstanceItem *item;
		crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
		for( crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
			itr != npcMap.end();
			++itr )
		{
			iconWidget = NULL;
			item = itr->second.first.get();
			isEnemy = 0;
			me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
			if(isEnemy==1)
			{
				if(i<friendCount)
				{
					iconWidget = m_friendNpcWidgetVec[i].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(friendIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_friendNpcWidgetVec.push_back(iconWidget);
				}
				i++;
			}
			else if(isEnemy == 0)
			{
				if(j<thirdCount)
				{
					iconWidget = m_threadNpcWidgetVec[j].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(thirdIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_threadNpcWidgetVec.push_back(iconWidget);
				}
				j++;
			}
			else
			{
				if(k<enemyCount)
				{
					iconWidget = m_enemyNpcWidgetVec[k].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(enemyIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_enemyNpcWidgetVec.push_back(iconWidget);
				}
				k++;
			}
			if(iconWidget)
			{
				iconWidget->setVisiable(true);
				crVector3 dir(item->getDirx(),item->getDiry(),item->getDirz());
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
				float px = item->getPosx()*crGlobalHandle::gData()->gUnitScale() - myX;
				float py = item->getPosy()*crGlobalHandle::gData()->gUnitScale() - myY;
				//if(texWidth>0.0f)
					px *= mapFactor[0];
				//if(texHeight>0.0f)
					py *= mapFactor[1];
				iconWidget->setMatrix(invScaleMat * newMat * scaleMat * crMatrix::translate(mapWidget->getBound().center()+crVector3(px,py,0.0f)));
			}
		}
		if(i<friendCount)
		{
			for(int ii = i; ii<friendCount; ++ii)
			{
				m_friendNpcWidgetVec[ii]->setVisiable(false);
			}
		}
		if(j<thirdCount)
		{
			for(int ii = j; ii<thirdCount; ++ii)
			{
				m_threadNpcWidgetVec[ii]->setVisiable(false);
			}
		}
		if(k<enemyCount)
		{
			for(int ii = k; ii<enemyCount; ++ii)
			{
				m_enemyNpcWidgetVec[ii]->setVisiable(false);
			}
		}
		crMyPlayerData::getInstance()->unlockInRangeNpcMap();

		i = 0;
		int itemCount = m_itemWidgetVec.size();
		crWidgetNode *itemIconWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_itemIconWidget));
		crMyPlayerData::getInstance()->lockInRangeItemMap();
		unsigned char dataType;
		crMyPlayerData::InRangeItemMap &itemMap = crMyPlayerData::getInstance()->getInRangeItemMap();
		for( crMyPlayerData::InRangeItemMap::iterator itr = itemMap.begin();
			itr != itemMap.end();
			++itr )
		{
			iconWidget = NULL;
			item = itr->second.first.get();
			item->getDataClass()->getParam(WCHDATA_DataType,param);
			dataType = *(unsigned char *)param;
			if(dataType == DT_Bot || dataType == DT_NPC || dataType == DT_MatrixNode || dataType == DT_Box || dataType == DT_Sphere)
			{
				if(i<itemCount)
				{
					iconWidget = m_itemWidgetVec[i].get();
				}
				else
				{
					iconWidget = dynamic_cast<crWidgetNode *>(itemIconWidget->clone(crCopyOp::DEEP_COPY_NODES));
					m_this->addChild(iconWidget);
					m_itemWidgetVec.push_back(iconWidget);
				}
				i++;
			}
			if(iconWidget)
			{
				iconWidget->setVisiable(true);
				float px = item->getPosx()*crGlobalHandle::gData()->gUnitScale() - myX;
				float py = item->getPosy()*crGlobalHandle::gData()->gUnitScale() - myY;
				//if(texWidth>0.0f)
					px *= mapFactor[0];
				//if(texHeight>0.0f)
					py *= mapFactor[1];
				iconWidget->setMatrix(crMatrix::translate(mapWidget->getBound().center()+crVector3(px,py,0.0f)));
			}
		}
		if(i<itemCount)
		{
			for(int ii = i; ii<itemCount; ++ii)
			{
				m_itemWidgetVec[ii]->setVisiable(false);
			}
		}
		crMyPlayerData::getInstance()->unlockInRangeItemMap();
	}
}
/////////////////////////////////////////
//
//crMapZoomMethod
//
/////////////////////////////////////////
crMapZoomMethod::crMapZoomMethod():
m_step(1.0f){}
crMapZoomMethod::crMapZoomMethod(const crMapZoomMethod& handle):
crMethod(handle),
m_step(handle.m_step)
{
}
void crMapZoomMethod::inputParam(int i, void *param)
{
}
void crMapZoomMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_step = atof(str.c_str());
		break;
	}
}
void crMapZoomMethod::operator()(crHandle &handle)
{
	float scale = crMyPlayerData::getInstance()->getMapScale();
	scale += m_step;
	crMyPlayerData::getInstance()->setMapScale(scale);
}
/////////////////////////////////////////
//
//crMapHitMoveMethod
//
/////////////////////////////////////////
crMapHitMoveMethod::crMapHitMoveMethod(){}
crMapHitMoveMethod::crMapHitMoveMethod(const crMapHitMoveMethod& handle):
crMethod(handle)
{
}
void crMapHitMoveMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crImageBoxWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
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
void crMapHitMoveMethod::addParam(int i, const std::string& str)
{
}
void crMapHitMoveMethod::operator()(crHandle &handle)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	if(m_this && me)
	{
		//crViewer *bindViewer = crKeyboardMouseHandle::getInstance()->getBindViewer();
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized()/*+bindViewer->getUIMouseOffset()*/);
		crCanvasNode *canvas = m_this->getParentCanvas();
		canvas->mouseTrans(mouse[0],mouse[1]);
		crVector3 centerCoord = m_this->getBound().center();
		crVector2 dxdy(mouse[0]-centerCoord[0],mouse[1]-centerCoord[1]);
		float myX = me->getPosx() * crGlobalHandle::gData()->gUnitScale();
		float myY = me->getPosy() * crGlobalHandle::gData()->gUnitScale();
		//float texWidth = 0.0f;
		//float texHeight = 0.0f;
		//float mapscale = crMyPlayerData::getInstance()->getMapScale();
		//crStateSet *stateset = m_this->getImageStateSet();
		//crTexture2D *tex = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(0,crStateAttribute::TEXTURE));
		//float x = 0.0f;
		//float y = 0.0f;
		//float pxperm = crMyPlayerData::getInstance()->getMapPxpm();
		//const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
		//texWidth = tex->getTextureWidth();
		//if(texWidth>0.0f)
			//x = myX / (float)worldSize[0];//pxpm/texWidth;
			//x = myX * pxperm/texWidth;
		//texHeight = tex->getTextureHeight();
		//if(texHeight>0.0f)
			//y = myY / (float)worldSize[1]; //* pxpm/texHeight;
			//y = myY * pxperm/texHeight;
		const crBoundingBox &bbox = m_this->getBoundBox();
		crVector2 mapWidgetSize(bbox.xLength(),bbox.yLength());

		//crVector2 mapFactor(1.0f,1.0f);
		//if(texWidth>0.0f)
		//	mapFactor[0] = pxperm*mapWidgetSize[0]/(texWidth*mapscale);
		//if(texHeight>0.0f)
		//	mapFactor[1] = pxperm*mapWidgetSize[1]/(texHeight*mapscale);
		crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);
		crVector3 hitPos;
		hitPos[0] = (mouse[0] - centerCoord[0])/mapFactor[0]+myX;
		hitPos[1] = (mouse[1] - centerCoord[1])/mapFactor[1]+myY;
		me->doEvent(WCH_PlayerMoveTo,MAKEINT64(&hitPos,NULL));
	}
}
///////////////////////////////////////////
////
////crInitBuyCountDlgMethod
////
///////////////////////////////////////////
//crInitBuyCountDlgMethod::crInitBuyCountDlgMethod():
//m_emporium(false){}
//crInitBuyCountDlgMethod::crInitBuyCountDlgMethod(const crInitBuyCountDlgMethod& handle):
//crMethod(handle),
//m_canvasWidget(handle.m_canvasWidget),
//m_buyCountWidget(handle.m_buyCountWidget),
//m_priceWidget(handle.m_priceWidget),
//m_emporium(handle.m_emporium)
//{
//}
//void crInitBuyCountDlgMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crListControlWidgetNode*)param;
//		break;
//	//case 2:
//	//	if(param)
//	//	{
//	//		_crInt64 param64 = *(_crInt64*)param;
//	//		m_selectIndex = LOINT64(param64);
//	//	}
//	//	else
//	//	{
//	//		m_selectIndex = -1;
//	//	}
//	//	break;
//	}
//}
//void crInitBuyCountDlgMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_canvasWidget = str;
//		break;
//	case 1:
//		m_buyCountWidget = str;
//		break;
//	case 2:
//		m_priceWidget = str;
//		break;
//	case 3:
//		m_emporium = (bool)(atoi(str.c_str()));
//		break;
//	}
//}
//void crInitBuyCountDlgMethod::operator()(crHandle &handle)
//{
//	bool cancleTask = true;
//	do 
//	{
//		if(!m_this)
//			break;
//		crListControlNode *listNode = m_this->getSelectNode();
//		if(!listNode || !listNode->getVisiable())
//		{
//			break;
//		}
//		crVector3i *saleParam = (crVector3i *)(listNode->getData());
//		if(!saleParam || (*saleParam)[1]==0)
//		{//ui提示没有库存
//			//std::string str = "没有库存了，请下次再来！";
//			//crGlobalHandle::getInstance()->doEvent(WCH_UINotify,MAKEINT64(&str,NULL));
//			break;
//		}
//		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasWidget);
//		if(!canvas)
//			break;
//		ref_ptr<crEditWidgetNode> buyCountWidget = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_buyCountWidget));
//		if(!buyCountWidget)
//			break;
//		buyCountWidget->setString(std::string("1"));
//		ref_ptr<crStaticTextWidgetNode> priceWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_priceWidget));
//		if(!priceWidget)
//			break;
//		priceWidget->setString(crArgumentParser::appItoa((*saleParam)[2]));
//		crMyPlayerData::getInstance()->setCurrentDialogData(saleParam);
//		crMyPlayerData::getInstance()->setSaleDialogEmporium(m_emporium);
//		cancleTask = false;
//	} while (0);
//	handle.outputParam(0,&cancleTask);
//}
///////////////////////////////////////////
////
////crInitSaleCountDlgMethod
////
///////////////////////////////////////////
//crInitSaleCountDlgMethod::crInitSaleCountDlgMethod(){}
//crInitSaleCountDlgMethod::crInitSaleCountDlgMethod(const crInitSaleCountDlgMethod& handle):
//crMethod(handle),
//m_canvasWidget(handle.m_canvasWidget),
//m_saleCountWidget(handle.m_saleCountWidget),
//m_priceWidget(handle.m_priceWidget)
//{
//}
//void crInitSaleCountDlgMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crListControlWidgetNode*)param;
//		break;
//	//case 2:
//	//	if(param)
//	//	{
//	//		_crInt64 param64 = *(_crInt64*)param;
//	//		m_selectIndex = LOINT64(param64);
//	//	}
//	//	else
//	//	{
//	//		m_selectIndex = -1;
//	//	}
//	//	break;
//	}
//}
//void crInitSaleCountDlgMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_canvasWidget = str;
//		break;
//	case 1:
//		m_saleCountWidget = str;
//		break;
//	case 2:
//		m_priceWidget = str;
//		break;
//	}
//}
//void crInitSaleCountDlgMethod::operator()(crHandle &handle)
//{
//	bool cancleTask = true;
//	do 
//	{
//		if(!m_this)
//			break;
//		crListControlNode *listNode = m_this->getSelectNode();
//		if(!listNode || !listNode->getVisiable())
//		{
//			break;
//		}
//		crInstanceItem *item = (crInstanceItem*)(listNode->getData());
//		if(!item)
//		{
//			break;
//		}
//		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasWidget);
//		if(!canvas)
//			break;
//		ref_ptr<crEditWidgetNode> saleCountWidget = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_saleCountWidget));
//		if(!saleCountWidget)
//			break;
//		saleCountWidget->setString(std::string("1"));
//		ref_ptr<crStaticTextWidgetNode> priceWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_priceWidget));
//		if(!priceWidget)
//			break;
//		crData *itemData = item->getDataClass();
//		void *param;
//		itemData->getParam(WCHDATA_ItemPrice,param);
//		int price = *(int*)param;
//		price *= crGlobalHandle::gData()->gSalerate();
//		priceWidget->setString(crArgumentParser::appItoa(price));
//		crMyPlayerData::getInstance()->setCurrentDialogData(item);
//		cancleTask = false;
//	} while (0);
//	handle.outputParam(0,&cancleTask);
//}
///////////////////////////////////////////
////
////crBuyCountChangeMethod
////
///////////////////////////////////////////
//crBuyCountChangeMethod::crBuyCountChangeMethod(){}
//crBuyCountChangeMethod::crBuyCountChangeMethod(const crBuyCountChangeMethod& handle):
//crMethod(handle),
//m_priceWidget(handle.m_priceWidget)
//{
//}
//void crBuyCountChangeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crEditWidgetNode*)param;
//		break;
//	}
//}
//void crBuyCountChangeMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_priceWidget = str;
//		break;
//	}
//}
//void crBuyCountChangeMethod::operator()(crHandle &handle)
//{
//	crCanvasNode *parentCanvas = m_this->getParentCanvas();
//	do 
//	{
//		if(!m_this)
//			break;
//		crVector3i *saleParam = (crVector3i *)(crMyPlayerData::getInstance()->getCurrentDialogData());
//		if(!saleParam || (*saleParam)[1]==0)
//		{//ui提示没有库存
//			//std::string str = "没有库存了，请下次再来！";
//			//crGlobalHandle::getInstance()->doEvent(WCH_UINotify,MAKEINT64(&str,NULL));
//			break;
//		}
//		const std::wstring &data = m_this->getString();
//		int count = _wtoi(data.c_str());
//		if(count<1)
//			count = 1;
//		else if((*saleParam)[1]>0)
//		{
//			if(count > (*saleParam)[1])
//			{
//				count = (*saleParam)[1];
//			}
//			if(count>crGlobalHandle::gData()->gMaxLapover())
//			{
//				count = crGlobalHandle::gData()->gMaxLapover();
//			}
//		}
//		else if(count>crGlobalHandle::gData()->gMaxLapover())
//			count = crGlobalHandle::gData()->gMaxLapover();
//		m_this->setString(crArgumentParser::appItoa(count));
//		
//		crStaticTextWidgetNode *priceWidget = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_priceWidget));
//		if(!priceWidget)
//			break;
//		int price = count * (*saleParam)[2];
//		priceWidget->setString(crArgumentParser::appItoa(price));
//		return;
//	} while (0);
//	crFilterRenderManager::getInstance()->closeCanvas(parentCanvas);
//}
///////////////////////////////////////////
////
////crSaleCountChangeMethod
////
///////////////////////////////////////////
//crSaleCountChangeMethod::crSaleCountChangeMethod(){}
//crSaleCountChangeMethod::crSaleCountChangeMethod(const crSaleCountChangeMethod& handle):
//crMethod(handle),
//m_priceWidget(handle.m_priceWidget)
//{
//}
//void crSaleCountChangeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crEditWidgetNode*)param;
//		break;
//	}
//}
//void crSaleCountChangeMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_priceWidget = str;
//		break;
//	}
//}
//void crSaleCountChangeMethod::operator()(crHandle &handle)
//{
//	crCanvasNode *parentCanvas = m_this->getParentCanvas();
//	do 
//	{
//		if(!m_this)
//			break;
//		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
//		if(!me)
//			break;
//		crInstanceItem *item = (crInstanceItem *)(crMyPlayerData::getInstance()->getCurrentDialogData());
//		if(!item)
//		{
//			break;
//		}
//		short maxcount;
//		me->doEvent(WCH_GetItemCountInBackPack,MAKEINT64(item,&maxcount));
//		if(maxcount == 0)
//		{
//			break;
//		}
//		const std::wstring &data = m_this->getString();
//		int count = _wtoi(data.c_str());
//		if(item->getInstanceItemID()>0)
//		{
//			count = 1;
//		}
//		else if(count<1)
//		{
//			count = 1;
//		}
//		else
//		{
//			if(count>crGlobalHandle::gData()->gMaxLapover())
//				count = crGlobalHandle::gData()->gMaxLapover();
//			if(count>maxcount)
//				count = maxcount;
//		}
//		m_this->setString(crArgumentParser::appItoa(count));
//		
//		crStaticTextWidgetNode *priceWidget = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_priceWidget));
//		if(!priceWidget)
//			break;
//		crData *itemData = item->getDataClass();
//		void *param;
//		itemData->getParam(WCHDATA_ItemPrice,param);
//		int price = *(int*)param;
//		price = price * crGlobalHandle::gData()->gSalerate() * count;
//		priceWidget->setString(crArgumentParser::appItoa(price));
//		return;
//	} while (0);
//	crFilterRenderManager::getInstance()->closeCanvas(parentCanvas);
//}
/////////////////////////////////////////
//
//crUIUpdateTaskTraceMethod
//
/////////////////////////////////////////
crUIUpdateTaskTraceMethod::crUIUpdateTaskTraceMethod(){}
crUIUpdateTaskTraceMethod::crUIUpdateTaskTraceMethod(const crUIUpdateTaskTraceMethod& handle):
crMethod(handle),
m_traceWidget(handle.m_traceWidget)
{
}
void crUIUpdateTaskTraceMethod::inputParam(int i, void *param)
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
void crUIUpdateTaskTraceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_traceWidget = str;
		break;
	}
}
void crUIUpdateTaskTraceMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
		if(!me) return;
		crHypertextWidgetNode *traceWidget = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_traceWidget));
		traceWidget->clearText();
		crData *myMetierData = me->getMetierDataClass();
		void *param;
		myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
		myMetierData->getParam(WCHDATA_GameTaskMap,param);
		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
		if(!gameTaskMap||gameTaskMap->empty())
		{
			myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
			return;
		}
		std::string file,ext;
		unsigned char steptype;
		unsigned char i = 0;
		char tmpText[64];
		crGameTask* gameTask;
		for( GameTaskMap::iterator itr = gameTaskMap->begin();
			i<crGlobalHandle::gData()->gMaxDisplayTaskTrace() && itr != gameTaskMap->end();
			++itr )
		{
			if(itr->second->getActivation() && !itr->second->isComplete())
			{
				gameTask = itr->second.get();
				file = gameTask->getHyperText();
				if(!file.empty())
				{
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					std::vector<float>v_i;
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",itr->second->getTaskName()+"\n");
					cfg_script.Pop();
					cfg_script.Pop();
					traceWidget->setHypertext(cfg_script,true);

					file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
					ext = crArgumentParser::getFileExtension(file);
					steptype = itr->second->getTaskStepType();
					if(steptype == TS_Zero)
					{
						int progress = gameTask->getTaskProgress()+1;
						file = crArgumentParser::getFileNameEliminateExt(file)+crArgumentParser::appItoa(progress)+"."+ext;
						traceWidget->setHypertext(file,true);
					}
					else if(gameTask->isTaskCanRefer())
					{
						file = crArgumentParser::getFileNameEliminateExt(file)+"1."+ext;
						traceWidget->setHypertext(file,true);
					}
					else
					{
						std::string relfile;
						int progress = gameTask->getTaskProgress();
						_crInt16 A,B;
						A = LOINT32(progress);
						B = HIINT32(progress);
						_crInt16 C,D;
						int step = gameTask->getTaskSteps()-1;
						C = LOINT32(step);
						D = HIINT32(step);
						_crInt8 a,c;
						if(steptype >= TS_One)
						{
							relfile = crArgumentParser::getFileNameEliminateExt(file)+"One."+ext;
							traceWidget->setHypertext(relfile,true);

							a = LOINT16(A);
							c = LOINT16(C);
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							cfg_script.Add("Content");
							cfg_script.Push("Content");
							std::vector<float>v_i;
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color",v_i);
							sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
							cfg_script.Add("Text",tmpText);
							cfg_script.Pop();
							cfg_script.Pop();
							traceWidget->setHypertext(cfg_script,true);
						}
						if(steptype >= TS_Two)
						{
							relfile = crArgumentParser::getFileNameEliminateExt(file)+"Two."+ext;
							traceWidget->setHypertext(relfile,true);

							a = HIINT16(A);
							c = HIINT16(C);
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							cfg_script.Add("Content");
							cfg_script.Push("Content");
							std::vector<float>v_i;
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color",v_i);
							sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
							cfg_script.Add("Text",tmpText);
							cfg_script.Pop();
							cfg_script.Pop();
							traceWidget->setHypertext(cfg_script,true);
						}
						if(steptype >= TS_Three)
						{
							relfile = crArgumentParser::getFileNameEliminateExt(file)+"Three."+ext;
							traceWidget->setHypertext(relfile,true);

							a = LOINT16(B);
							c = LOINT16(D);
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							cfg_script.Add("Content");
							cfg_script.Push("Content");
							std::vector<float>v_i;
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color",v_i);
							sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
							cfg_script.Add("Text",tmpText);
							cfg_script.Pop();
							cfg_script.Pop();
							traceWidget->setHypertext(cfg_script,true);
						}
						if(steptype >= TS_Four)
						{
							relfile = crArgumentParser::getFileNameEliminateExt(file)+"Four."+ext;
							traceWidget->setHypertext(relfile,true);

							a = HIINT16(B);
							c = HIINT16(D);
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							cfg_script.Add("Content");
							cfg_script.Push("Content");
							std::vector<float>v_i;
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color",v_i);
							sprintf(tmpText,"(%d/%d)\0",(int)a,(int)c);
							cfg_script.Add("Text",tmpText);
							cfg_script.Pop();
							cfg_script.Pop();
							traceWidget->setHypertext(cfg_script,true);
						}
					}
					i++;
				}
			}
		}
		myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
////////////////////////////////
//
//crBackPackItemDragEventMethod
//
///////////////////////////////////
crBackPackItemDragEventMethod::crBackPackItemDragEventMethod():
m_ea(NULL)
{
}
crBackPackItemDragEventMethod::crBackPackItemDragEventMethod(const crBackPackItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL),
m_storeCanvasName(handle.m_storeCanvasName),
m_storageCanvasName(handle.m_storageCanvasName),
m_panelCanvasName(handle.m_panelCanvasName),
m_bodyCanvasName(handle.m_bodyCanvasName)
{
}

void crBackPackItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crBackPackItemDragEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_storeCanvasName = str;
		break;
	case 1:
		m_storageCanvasName = str;
		break;
	case 2:
		m_panelCanvasName = str;
		break;
	case 3:
		m_bodyCanvasName = str;
		break;
	}
}

void crBackPackItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{//Drop Item
			_case = 0;
			break;
		}
		else if(pickUI != listNode && dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			const std::string &name = pickUI->getParentCanvas()->getName();
			if(name.compare(m_storeCanvasName) == 0)
			{
				_case = 1;
			}
			else if(name.compare(m_storageCanvasName) == 0)
			{
				_case = 2;
			}
			else if(name.compare(m_panelCanvasName) == 0 || name.compare(m_bodyCanvasName) == 0)
			{
				_case = 3;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crSaleItemDragEventMethod
//
///////////////////////////////////
crSaleItemDragEventMethod::crSaleItemDragEventMethod():
m_ea(NULL)
{
}
crSaleItemDragEventMethod::crSaleItemDragEventMethod(const crSaleItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL),
m_backpackCanvasName(handle.m_backpackCanvasName)
{
}

void crSaleItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crSaleItemDragEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_backpackCanvasName = str;
		break;
	}
}

void crSaleItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{
			break;
		}
		else if(pickUI != listNode && dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			const std::string &name = pickUI->getParentCanvas()->getName();
			if(name.compare(m_backpackCanvasName) == 0)
			{
				_case = 0;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crSkillItemDragEventMethod
//
///////////////////////////////////
crSkillItemDragEventMethod::crSkillItemDragEventMethod():
m_ea(NULL)
{
}
crSkillItemDragEventMethod::crSkillItemDragEventMethod(const crSkillItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL),
m_panelCanvasName(handle.m_panelCanvasName)
{
}

void crSkillItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crSkillItemDragEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_panelCanvasName = str;
		break;
	}
}

void crSkillItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{
			break;
		}
		else if(pickUI != listNode && dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			const std::string &name = pickUI->getParentCanvas()->getName();
			if(name.compare(m_panelCanvasName) == 0)
			{
				_case = 0;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crQuickItemDragEventMethod
//
///////////////////////////////////
crQuickItemDragEventMethod::crQuickItemDragEventMethod():
m_ea(NULL)
{
}
crQuickItemDragEventMethod::crQuickItemDragEventMethod(const crQuickItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL)
{
}

void crQuickItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crQuickItemDragEventMethod::addParam(int i, const std::string& str)
{
}

void crQuickItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{
			_case = 0;
			break;
		}
		else if(dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			if(pickUI == m_this)
			{
				_case = 1;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crEquipOnItemDragEventMethod
//
///////////////////////////////////
crEquipOnItemDragEventMethod::crEquipOnItemDragEventMethod():
m_ea(NULL)
{
}
crEquipOnItemDragEventMethod::crEquipOnItemDragEventMethod(const crEquipOnItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL),
m_backpackCanvasName(handle.m_backpackCanvasName)
{
}

void crEquipOnItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crEquipOnItemDragEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_backpackCanvasName = str;
		break;
	}
}

void crEquipOnItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{
			_case = 0;
			break;
		}
		else if(pickUI != listNode && dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			const std::string &name = pickUI->getParentCanvas()->getName();
			if(name.compare(m_backpackCanvasName) == 0)
			{
				_case = 0;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crStorageItemDragEventMethod
//
///////////////////////////////////
crStorageItemDragEventMethod::crStorageItemDragEventMethod():
m_ea(NULL)
{
}
crStorageItemDragEventMethod::crStorageItemDragEventMethod(const crStorageItemDragEventMethod& handle):
crMethod(handle),
m_ea(NULL),
m_backpackCanvasName(handle.m_backpackCanvasName)
{
}

void crStorageItemDragEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crStorageItemDragEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_backpackCanvasName = str;
		break;
	}
}

void crStorageItemDragEventMethod::operator()(crHandle &handle)
{
	int _case = -1;
	do 
	{
		if(!m_this)
			break;
		crListControlNode *listNode = m_this->getSelectNode();
		if(!listNode || !listNode->getVisiable())
		{
			break;
		}
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickUI = NULL;
		crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
		if(!pickUI)
		{
			break;
		}
		else if(pickUI != listNode && dynamic_cast<crListControlWidgetNode *>(pickUI))
		{
			const std::string &name = pickUI->getParentCanvas()->getName();
			if(name.compare(m_backpackCanvasName) == 0)
			{
				_case = 0;
			}
		}
	} while (0);
	handle.outputParam(0,&_case);
}
/////////////////////////////////////////
//
//crUIUpdateFaceframeMethod
//
/////////////////////////////////////////
crUIUpdateFaceframeMethod::crUIUpdateFaceframeMethod(){}
crUIUpdateFaceframeMethod::crUIUpdateFaceframeMethod(const crUIUpdateFaceframeMethod& handle):
crMethod(handle),
m_headImg(handle.m_headImg),
m_lv(handle.m_lv),
m_name(handle.m_name)
{
}
void crUIUpdateFaceframeMethod::inputParam(int i, void *param)
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
void crUIUpdateFaceframeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_headImg = str;
		break;
	case 1:
		m_lv = str;
		break;
	case 2:
		m_name = str;
		break;
	}
}
void crUIUpdateFaceframeMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getSelectedGameRole();
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(role && role->isLoaded() && playerGameData)
	{
		void *param;
		crData *roleData = role->getDataClass();

		crImageBoxWidgetNode *headImageWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_headImg));
		crStaticTextWidgetNode *lvWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_lv));
		crStaticTextWidgetNode *nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_name));

		std::string str;
		if(lvWidget)
		{
			crData *data = playerGameData->getDataClass();
			data->getParam(WCHDATA_Level,param);
			unsigned char lv = *((unsigned char*)param);
			str = crArgumentParser::appItoa((int)lv);
			lvWidget->setString(str);
		}
		if(headImageWidget)
		{//
			roleData->getParam(WCHDATA_ItemIcon,param);
			std::string iconFile = *(std::string *)param;
			headImageWidget->setImageName(iconFile);
		}
		if(nameWidget)
		{
			str = role->getIName();
			nameWidget->setString(str);
		}
	}
}
/////////////////////////////////////////
//
//crUIUpdateHPProgressMethod
//
/////////////////////////////////////////
crUIUpdateHPProgressMethod::crUIUpdateHPProgressMethod()
{
}
crUIUpdateHPProgressMethod::crUIUpdateHPProgressMethod(const crUIUpdateHPProgressMethod& handle):
crMethod(handle),
m_hpProgress(handle.m_hpProgress),
m_hpValue(handle.m_hpValue)
{
}
void crUIUpdateHPProgressMethod::inputParam(int i, void *param)
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
void crUIUpdateHPProgressMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_hpProgress = str;
		break;
	case 1:
		m_hpValue = str;
		break;
	}
}
void crUIUpdateHPProgressMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		crRole *role = crMyPlayerData::getInstance()->getCurrentRole();
		if(!role) return;
		crProgressWidgetNode *hpProgressWidget = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_hpProgress));
		crStaticTextWidgetNode *hpValueWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_hpValue));

		std::string str;
		void *param;
		crData *roleData = role->getDataClass();
		if(hpProgressWidget)
		{
			float maxhp = 0;
			role->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			hpProgressWidget->setProgress(rthp/maxhp);
			if(hpValueWidget)
			{
				str = crArgumentParser::appItoa(rthp) + "/" + crArgumentParser::appItoa(maxhp);
				hpValueWidget->setString(str);
			}
		}
	}
}
/////////////////////////////////////////
//
//crUIShowBattleTimeLimitMethod
//
/////////////////////////////////////////
crUIShowBattleTimeLimitMethod::crUIShowBattleTimeLimitMethod()
{
}
crUIShowBattleTimeLimitMethod::crUIShowBattleTimeLimitMethod(const crUIShowBattleTimeLimitMethod& handle):
crMethod(handle),
m_timeLimit(handle.m_timeLimit)
{
}
void crUIShowBattleTimeLimitMethod::inputParam(int i, void *param)
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
void crUIShowBattleTimeLimitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_timeLimit = str;
		break;
	}
}
void crUIShowBattleTimeLimitMethod::operator()(crHandle &handle)
{
	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	if(room && m_this && m_this->getVisiable())
	{
		crStaticTextWidgetNode *timeWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_timeLimit));
		if(timeWidget)
		{
			int limits = (int)(room->timeRemain());
			int minute = limits/60;
			int s = limits%60;
			timeWidget->setString(crArgumentParser::appItoa(minute)+":"+crArgumentParser::appItoa(s));
		}
	}
}
/////////////////////////////////////////
//
//crUISwapEnableMethod
//
/////////////////////////////////////////
crUISwapEnableMethod::crUISwapEnableMethod():
m_interval(1.0f)
{
}
crUISwapEnableMethod::crUISwapEnableMethod(const crUISwapEnableMethod& handle):
	crMethod(handle),
	m_interval(handle.m_interval)
{
}
void crUISwapEnableMethod::inputParam(int i, void *param)
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
void crUISwapEnableMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}
void crUISwapEnableMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		m_this->swapEnable(m_interval);
	}
}
/////////////////////////////////////////
//
//crCommandEventMethod
//
/////////////////////////////////////////
crCommandEventMethod::crCommandEventMethod():
	m_this(NULL),
	m_msg(NULL)
{
}
crCommandEventMethod::crCommandEventMethod(const crCommandEventMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_msg(handle.m_msg)
{
}
void crCommandEventMethod::inputParam(int i, void *param)
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
void crCommandEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msg = CREncapsulation::parseEventMsg(str);
		break;
	}
}
void crCommandEventMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crCanvasNode *canvas = m_this->getParentCanvas();
		crData *data = canvas->getDataClass();
		if(data) data->excHandle(m_msg);
	}
}
////////////////////////////////
//
//crAddRadioMethod
//
///////////////////////////////////
crAddRadioMethod::crAddRadioMethod()
{}

crAddRadioMethod::crAddRadioMethod(const crAddRadioMethod& handle):
	crMethod(handle),
	m_strvec(handle.m_strvec)
{
}

void crAddRadioMethod::inputParam(int i, void *param)
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
		m_this = (crRadioGroupWidgetNode*)param;
		break;
	}
}

void crAddRadioMethod::addParam(int i, const std::string& str)
{
	m_strvec.push_back(str);
}

void crAddRadioMethod::operator()(crHandle &handle)
{
	for( StrVec::iterator itr = m_strvec.begin();
		itr != m_strvec.end();
		++itr )
	{
		m_this->addRadioName(*itr);
	}
}
////////////////////////////////
//
//crPageValueChangeMethod
//
///////////////////////////////////
crPageValueChangeMethod::crPageValueChangeMethod():
	m_this(NULL)
{}

crPageValueChangeMethod::crPageValueChangeMethod(const crPageValueChangeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_pageSW(handle.m_pageSW)
{
}

void crPageValueChangeMethod::inputParam(int i, void *param)
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
		m_this = (crRadioGroupWidgetNode*)param;
		break;
	}
}

void crPageValueChangeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_pageSW = str;
		break;
	}
}

void crPageValueChangeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crCanvasNode *canvas = m_this->getParentCanvas();
		crMultiSwitch *pageSW = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_pageSW));
		if(pageSW)
			pageSW->setActiveSwitchSet(m_this->getSelect());
		canvas->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crGetRadioGroupValueMethod
//
///////////////////////////////////
crGetRadioGroupValueMethod::crGetRadioGroupValueMethod():
	m_this(NULL)
{}

crGetRadioGroupValueMethod::crGetRadioGroupValueMethod(const crGetRadioGroupValueMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_radioGroup(handle.m_radioGroup)
{
}

void crGetRadioGroupValueMethod::inputParam(int i, void *param)
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

void crGetRadioGroupValueMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radioGroup = str;
		break;
	}
}

void crGetRadioGroupValueMethod::operator()(crHandle &handle)
{
	crRadioGroupWidgetNode *radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radioGroup));
	int value = radioGroup?radioGroup->getSelect():0;
	handle.outputParam(0,&value);
}
////////////////////////////////
//
//crEnableRTTMethod
//
///////////////////////////////////
crEnableRTTMethod::crEnableRTTMethod()
{}

crEnableRTTMethod::crEnableRTTMethod(const crEnableRTTMethod& handle):
	crMethod(handle),
	m_rttname(handle.m_rttname),
	m_enable(handle.m_enable)
{
}

void crEnableRTTMethod::inputParam(int i, void *param)
{
}

void crEnableRTTMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_rttname = str;
		break;
	case 1:
		m_enable = (bool)(atoi(str.c_str()));
		break;
	}
}

void crEnableRTTMethod::operator()(crHandle &handle)
{
	crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture(m_rttname);
	if(rtt)
	{
		rtt->setEnable(m_enable);
	}
}
////////////////////////////////
//
//crTopViewRTTMethod
//
///////////////////////////////////
crTopViewRTTMethod::crTopViewRTTMethod():
	m_updInterval(0.5f)
{
}

crTopViewRTTMethod::crTopViewRTTMethod(const crTopViewRTTMethod& handle):
	crMethod(handle),
	m_imageBox(handle.m_imageBox),
	m_updInterval(handle.m_updInterval)
{
}

void crTopViewRTTMethod::inputParam(int i, void *param)
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

void crTopViewRTTMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_imageBox = str;
		break;
	case 1:
		m_updInterval = atof(str.c_str());
		break;
	}
}

void crTopViewRTTMethod::operator()(crHandle &handle)
{
	crImageBoxWidgetNode *imageBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getParentCanvas()->getWidget(m_imageBox));
	if(imageBox)
	{
		//imageBox->setImageName("RTT");
		//crStateSet *stateset = imageBox->getImageStateSet();
		//crTexture *rtt = crShaderManager::getInstance()->getGiTexture();
		//stateset->setTextureAttributeAndModes(0, rtt, crStateAttribute::ON);
		//imageBox->dirtyImage();
		crDisplaySettings::instance()->setDisableLOD(true);
		crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("TopViewRTT");
		if(!rtt)
		{
			const crVector2i& mapSize = crBrain::getInstance()->getMapSize();
			const crVector3i& worldSize = crBrain::getInstance()->getWorldSize();
			crRenderToTexture *rtt = new crRenderToTexture;
			rtt->setName("TopViewRTT");
			rtt->setMode(crRenderToTexture::RenderToTextureMode(crRenderToTexture::RenderToTexture|crRenderToTexture::RenderColorMap|crRenderToTexture::RealTimeUpd|crRenderToTexture::ExternMVP));
			rtt->setMapSize(mapSize[0],mapSize[1]);
			rtt->setUpdateInterval(m_updInterval);

			ref_ptr<crGroup> scene;
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("Static");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			scene = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			rtt->addRenderNode(scene.get());
			rtt->addTexturedNode(imageBox->getImageObject());
			rtt->setProjectionMatrix(CRCore::crMatrix::ortho(-worldSize[0]*0.5f,worldSize[0]*0.5f,-worldSize[1]*0.5f,worldSize[1]*0.5f,-worldSize[2],worldSize[2]));
			rtt->setViewMatrix(crMatrix::identity());
			rtt->init(1.0f);
			//CRIOManager::crLoadManager::getInstance()->requestCompileStateSet(rtt->getCameraNode()->getOrCreateStateSet());
			crRenderToTextureManager::getInstance()->addRenderToTexture(rtt);
		}
		else
		{
			rtt->setEnable(true);
		}
	}
}
////////////////////////////////
//
//crCameraViewRTTMethod
//
///////////////////////////////////
crCameraViewRTTMethod::crCameraViewRTTMethod():
	m_updInterval(0.1f)
{
	m_texSize.set(512,512);
}

crCameraViewRTTMethod::crCameraViewRTTMethod(const crCameraViewRTTMethod& handle):
	crMethod(handle),
	m_imageBox(handle.m_imageBox),
	m_texSize(handle.m_texSize),
	m_updInterval(handle.m_updInterval)
{
}

void crCameraViewRTTMethod::inputParam(int i, void *param)
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

void crCameraViewRTTMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_imageBox = str;
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_texSize);
		break;
	case 2:
		m_updInterval = atof(str.c_str());
		break;
	}
}

void crCameraViewRTTMethod::operator()(crHandle &handle)
{
	crImageBoxWidgetNode *imageBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getParentCanvas()->getWidget(m_imageBox));
	if(imageBox)
	{
		crDisplaySettings::instance()->setDisableLOD(false);
		//crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("ls1shadow");
		//imageBox->getOrCreateImageObject()->getOrCreateStateSet()->setTextureAttributeAndModes(TEXTURE_BASEMAP,rtt->getTexture(),crStateAttribute::ON);
		crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("CameraViewRTT");
		if(!rtt)
		{
			crRenderToTexture *rtt = new crRenderToTexture;
			rtt->setName("CameraViewRTT");
			rtt->setMode(crRenderToTexture::RenderToTextureMode(crRenderToTexture::RenderToTexture|crRenderToTexture::RenderColorMap|crRenderToTexture::RealTimeUpd|crRenderToTexture::CameraMVP));
			rtt->setMapSize(m_texSize[0],m_texSize[1]);
			rtt->setUpdateInterval(m_updInterval);

			ref_ptr<crGroup> scene;
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("Static");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			scene = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			rtt->addRenderNode(scene.get());
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("Dynamic");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			scene = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			rtt->addRenderNode(scene.get());
			rtt->addTexturedNode(imageBox->getImageObject());
			rtt->init(1.0f);
			//CRIOManager::crLoadManager::getInstance()->requestCompileStateSet(rtt->getCameraNode()->getOrCreateStateSet());
			crRenderToTextureManager::getInstance()->addRenderToTexture(rtt);
		}
		else
		{
			rtt->setEnable(true);
		}
	}
}
////////////////////////////////
//
//crCollideMapRTTMethod
//
///////////////////////////////////
crCollideMapRTTMethod::crCollideMapRTTMethod():
	m_updInterval(0.5f)
{
}

crCollideMapRTTMethod::crCollideMapRTTMethod(const crCollideMapRTTMethod& handle):
	crMethod(handle),
	m_imageBox(handle.m_imageBox),
	m_updInterval(handle.m_updInterval)
{
}

void crCollideMapRTTMethod::inputParam(int i, void *param)
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

void crCollideMapRTTMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_imageBox = str;
		break;
	case 1:
		m_updInterval = atof(str.c_str());
		break;
	}
}

void crCollideMapRTTMethod::operator()(crHandle &handle)
{
	crImageBoxWidgetNode *imageBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getParentCanvas()->getWidget(m_imageBox));
	if(imageBox)
	{
		crDisplaySettings::instance()->setDisableLOD(true);
		//crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("ls1shadow2");
		//imageBox->getOrCreateImageObject()->getOrCreateStateSet()->setTextureAttributeAndModes(TEXTURE_BASEMAP,rtt->getTexture(),crStateAttribute::ON);
		crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("CollideMapRTT");
		if(!rtt)
		{
			const crVector2i& mapSize = crBrain::getInstance()->getMapSize();
			const crVector3i& worldSize = crBrain::getInstance()->getWorldSize();
			crRenderToTexture *rtt = new crRenderToTexture;
			rtt->setName("CollideMapRTT");
			rtt->setMode(crRenderToTexture::RenderToTextureMode(crRenderToTexture::RenderToTexture|crRenderToTexture::RenderCollideMap|crRenderToTexture::RealTimeUpd|crRenderToTexture::ExternMVP));
			rtt->setMapSize(mapSize[0],mapSize[1]);
			rtt->setUpdateInterval(m_updInterval);

			ref_ptr<crGroup> scene;
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("Static");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			scene = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			rtt->addRenderNode(scene.get());
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("CollideRoot");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			scene = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			rtt->addRenderNode(scene.get());
			rtt->addTexturedNode(imageBox->getImageObject());
			//float terHeight = heightField->getMaxHeight();
			rtt->setProjectionMatrix(CRCore::crMatrix::ortho(-(float)worldSize[0]*0.5f, (float)worldSize[0]*0.5f,-(float)worldSize[1]*0.5f, (float)worldSize[1]*0.5f,-(float)worldSize[2], (float)worldSize[2]));
			rtt->setViewMatrix(crMatrix::identity());
			rtt->init(1.0f);
			crStateSet *camerass = rtt->getCameraNode()->getOrCreateStateSet();
			//CRIOManager::crLoadManager::getInstance()->requestCompileStateSet(camerass);
			///////////
			crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("collidemap");
			crUniform *uniform = camerass->getOrCreateUniform("walkable",CRCore::crUniform::FLOAT);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(1.0f);
			uniform = camerass->getOrCreateUniform("maxheight",CRCore::crUniform::FLOAT);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set((float)worldSize[2]);
			camerass->merge(*shader);
			camerass->setStateSetMode(crStateSet::SS_CollideMapRender);
			crRenderToTextureManager::getInstance()->addRenderToTexture(rtt);
		}
		else
		{
			rtt->setEnable(true);
		}
	}
}
////////////////////////////////
//
//crExportRTT
//
///////////////////////////////////
void crExportRTT::operator () (crCameraNode& camera)
{
	if(camera.getExportImageBuf()==2)
	{
		crCameraNode::BufferAttachmentMap& bufferAttachmentMap = camera.getBufferAttachmentMap();
		crImage *image = bufferAttachmentMap[crCameraNode::COLOR_BUFFER].m_image.get();
		if(image)
		{
			std::string fileName = image->getFileName();
			CRIOManager::writeImageFile(*image,fileName);
			bufferAttachmentMap[crCameraNode::COLOR_BUFFER].m_image = NULL;
			camera.setPostDrawCallback(NULL);
			camera.exportImageBuf(3);
		}		
	}
}
////////////////////////////////
//
//crExportRTTMethod
//
///////////////////////////////////
crExportRTTMethod::crExportRTTMethod()
{}

crExportRTTMethod::crExportRTTMethod(const crExportRTTMethod& handle):
	crMethod(handle),
	m_rttmodeWidget(handle.m_rttmodeWidget),
	m_rttNameVec(handle.m_rttNameVec)
{
}

void crExportRTTMethod::inputParam(int i, void *param)
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

void crExportRTTMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_rttmodeWidget = str;
		break;
	default:
		m_rttNameVec.push_back(str);
	}
}

void crExportRTTMethod::operator()(crHandle &handle)
{
	crRadioGroupWidgetNode *rttmodeWidget = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getParentCanvas()->getWidget(m_rttmodeWidget));
	if(rttmodeWidget)
	{
		unsigned int select = rttmodeWidget->getSelect();
		if(select < m_rttNameVec.size())
		{
			crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture(CRCore::crArgumentParser::getFileNameEliminateExt(m_rttNameVec[select]));
			if(rtt)
			{
				crCameraNode *cameraNode = rtt->getCameraNode();
				if(cameraNode && cameraNode->getExportImage() == 0)
				{
					char filename[128];
					sprintf(filename,"output/photos/%s\0",m_rttNameVec[select].c_str());
					crImage *image = new crImage;
					image->setFileName(filename);
					const crVector2i &mapsize = rtt->getMapSize();
					crTexture2D *texture = rtt->getTexture();
					if(crArgumentParser::getFileExtension(m_rttNameVec[select]).compare("img")!=0)
						image->allocateImage(mapsize[0], mapsize[1], 1, texture->getSourceFormat(), GL_UNSIGNED_BYTE);
					else
						image->allocateImage(mapsize[0], mapsize[1], 1, texture->getSourceFormat(), texture->getSourceType());
					cameraNode->attach(crCameraNode::COLOR_BUFFER,image);
					cameraNode->setPostDrawCallback(new crExportRTT);
					cameraNode->exportImage(1);
				}
			}
		}
	}
}