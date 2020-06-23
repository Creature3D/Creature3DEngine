/**********************************************************************
*
*	文件:	   appMethod17.h
*
*	描述:	   
*
*	作者:	   熊辉
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod17.h>
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
#include <JXJ/appData5.h>
#include <stdlib.h>
#include <time.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crConvertUTF.h>

using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRUI;
using namespace CRIOManager;
using namespace CRProducer;

/////////////////////////////////////////
//
//crJXJUIManorStairJueSeJunlingTipMethod
//
/////////////////////////////////////////
crJXJUIManorStairJueSeJunlingTipMethod::crJXJUIManorStairJueSeJunlingTipMethod():
	m_this(NULL)
{
}

crJXJUIManorStairJueSeJunlingTipMethod::crJXJUIManorStairJueSeJunlingTipMethod(const crJXJUIManorStairJueSeJunlingTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_this(NULL),
	m_str(0)
{
}
void crJXJUIManorStairJueSeJunlingTipMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIManorStairJueSeJunlingTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	}
}
void crJXJUIManorStairJueSeJunlingTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	crStaticTextWidgetNode *tipsText = NULL;
	tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
	std::string str;
	str = "每30分钟获得一个军令";
	if (tipsText)
	{
		tipsText->setString(str);
		tipsText->setColor(FCOLOR_BLACK);
	}
	const crBoundingBox &btnbox = m_this->getBoundBox();
	const crBoundingBox &tipsbox = tips->getBoundBox();
	const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
	float posx = btnbox.m_min[0];
	float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
	////////////
	crVector2 mouse(crVector2(posx,posy));
	tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
	tips->setCanFocus(false);
	crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
}

/////////////////////////////////////////
//
//crJXJUIActiveCardExtraTipMethod
//
/////////////////////////////////////////
crJXJUIActiveCardExtraTipMethod::crJXJUIActiveCardExtraTipMethod():
	m_this(NULL)
{
}

crJXJUIActiveCardExtraTipMethod::crJXJUIActiveCardExtraTipMethod(const crJXJUIActiveCardExtraTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_this(NULL),
	m_str(0)
{
}
void crJXJUIActiveCardExtraTipMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIActiveCardExtraTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	}
}
void crJXJUIActiveCardExtraTipMethod::operator()(crHandle &handle)
{
	crMultiSwitch *switchnode;
	ref_ptr<crCanvasNode> jiangkacanvas = m_this->getParentCanvas();
	std::string butonname = m_this->getName();
	if ("Jkzh_Jihuo" == butonname)
	{
		switchnode = dynamic_cast<crMultiSwitch *>(jiangkacanvas->getChildNode("Jkzh_Jnxx1"));
		int status = switchnode->getActiveSwitchSet();
		if(status == 0)
		{
			ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
			crStaticTextWidgetNode *tipsText = NULL;
			tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
			std::string str;
			str = "天命符数量不足或未激活";
			if (tipsText)
			{
				tipsText->setString(str);
				tipsText->setColor(FCOLOR_BLACK);
			}
			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
			////////////
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			tips->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
	else if("Jkzh_Jihuo_1" == butonname)
	{
		switchnode = dynamic_cast<crMultiSwitch *>(jiangkacanvas->getChildNode("Jkzh_Jnxx2"));
		int status = switchnode->getActiveSwitchSet();
		if(status == 0)
		{
			ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
			crStaticTextWidgetNode *tipsText = NULL;
			tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
			std::string str;
			str = "天命符数量不足或未激活";
			if (tipsText)
			{
				tipsText->setString(str);
				tipsText->setColor(FCOLOR_BLACK);
			}
			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
			////////////
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			tips->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
	else if("Jkzh_Jihuo_2" == butonname)
	{
		switchnode = dynamic_cast<crMultiSwitch *>(jiangkacanvas->getChildNode("Jkzh_Jnxx3"));
		int status = switchnode->getActiveSwitchSet();
		if(status == 0)
		{
			ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
			crStaticTextWidgetNode *tipsText = NULL;
			tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
			std::string str;
			str = "天命符数量不足或未激活";
			if (tipsText)
			{
				tipsText->setString(str);
				tipsText->setColor(FCOLOR_BLACK);
			}
			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
			////////////
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			tips->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJAttachStoreFulianMethod
//
/////////////////////////////////////////
crJXJAttachStoreFulianMethod::crJXJAttachStoreFulianMethod():
	m_this(NULL)
{
}
crJXJAttachStoreFulianMethod::crJXJAttachStoreFulianMethod(const crJXJAttachStoreFulianMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJAttachStoreFulianMethod::inputParam(int i, void *param)
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
void crJXJAttachStoreFulianMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_promptCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn = str;
		break;
	default:
		break;
	}
}
void crJXJAttachStoreFulianMethod::operator()(crHandle &handle)
{
	void *param;
	crBagItemData *gongfangitemdata = NULL;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	crData *canvasdata = canvas->getDataClass();
	canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
	gongfangitemdata = (crBagItemData *)param;
	//int status = gongfangitemdata->getInlayID();
	
	if (canvas.valid())
	{
		crData *canvasdata = canvas->getDataClass();
		if (canvasdata)
		{
			ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int nameid = itemTab->getTitleIndex("name");//8
			ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);

			if (tipscanvas.valid())
			{
				ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
				ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
				{
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text","附炼会覆盖装备当前的附炼属性，你确认要附炼吗？");
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
				}
				if(okbtn.valid())okbtn->setEnable(true);
				crData *dlgData = tipscanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				if(gongfangitemdata->getInlayID() != 0)
				{
					crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
				}
				else
				{
					if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
			}
		}
	}	
}

/////////////////////////////////////////
//
//crJXJButtonExpendHintMethod
//
/////////////////////////////////////////
crJXJButtonExpendHintMethod::crJXJButtonExpendHintMethod():
	m_this(NULL)
{
}
crJXJButtonExpendHintMethod::crJXJButtonExpendHintMethod(const crJXJButtonExpendHintMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJButtonExpendHintMethod::inputParam(int i, void *param)
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
void crJXJButtonExpendHintMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_id = atoi(str.c_str());
		break;
	case 1:
		m_promptCanvas = str;
		break;
	case 2:
		m_information = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	default:
		break;
	}
}
void crJXJButtonExpendHintMethod::operator()(crHandle &handle)
{
	void *param;
	int huodongtype = 0;
	int goldneed = 0;
	if (m_id==0 || m_id == 1 || m_id == 2)
	{
		huodongtype = m_id;
	}
	ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crData> data = playergamedata->getDataClass();
	data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
	unsigned char gettimes = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char myviplv = *(unsigned char *)param;
	int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());

	if(gettimes < alltimes)
	{
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldJiFenAwardJiSuanTab);
		std::vector<std::string> record;
		int yuanbaoindex = itemtab->getTitleIndex("元宝数"); 
		if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(gettimes+1),record) >= 0)
			goldneed = atoi(record[yuanbaoindex].c_str());
	}
	
// 	ref_ptr<crMultiSwitch> switchnode1,switchnode2,switchnode3;
 	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
// 	switch (m_id)
// 	{
// 	case 0:
// 		switchnode1 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw1"));
// 		if(switchnode1.valid())
// 		{
// 			if(gettimes>=1)
// 			{
// 				switchnode1->setActiveSwitchSet(1);
// 			}
// 			else
// 			{
// 				switchnode1->setActiveSwitchSet(0);
// 			}
// 		}
// 		break;
// 	case 1:
// 		switchnode2 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw2"));
// 		if(switchnode2.valid())
// 		{
// 			if(gettimes>=1)
// 			{
// 				switchnode2->setActiveSwitchSet(1);
// 			}
// 			else
// 			{
// 				switchnode2->setActiveSwitchSet(0);
// 			}
// 		}
// 		break;
// 	case 2:
// 		switchnode3 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw3"));
// 		if(switchnode3.valid())
// 		{
// 			if(gettimes>=1)
// 			{
// 				switchnode3->setActiveSwitchSet(1);
// 			}
// 			else
// 			{
// 				switchnode3->setActiveSwitchSet(0);
// 			}
// 		}
// 		break;
// 	default:
// 		break;
// 	}

	data->getParam(WCHDATA_JXJGoldCostHintInt,param);
	unsigned int hint = *(unsigned int *)param;
	int ifhint = hint & JXJGoldCostType_ShiJieFuBenLingQuJiangLiHint;

	if (canvas.valid())
	{
		crData *canvasdata = canvas->getDataClass();
		if (canvasdata)
		{
			ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int nameid = itemTab->getTitleIndex("name");//8
			ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);

			if (tipscanvas.valid())
			{
				ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
				ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
				if(gettimes >= 1 && ifhint)
				{
					int hinttype = JXJGoldCostType_ShiJieFuBenLingQuJiangLiHint;
					crData *braindata = crBrain::getInstance()->getDataClass();
					braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					cfg_script.Add("Content");
					cfg_script.Push("Content");
					cfg_script.Add("Text","您确认要花费");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					char tmpText[160];
					sprintf(tmpText,"%d礼金/元宝\0",goldneed);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",3);
					cfg_script.Add("Text","来领取奖励吗？");
					cfg_script.Pop();
					cfg_script.Pop();
					if(information.valid())information->setHypertext(cfg_script);
				}
				if(okbtn.valid())okbtn->setEnable(true);
				crData *dlgData = tipscanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				if(gettimes >= 1 && ifhint && gettimes < alltimes)
				{
					crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
				}
				else
				{
					if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
			}
		}
	}	
}

///////////////////////////////////////////
////
////crJXJCheckGamePlanTaskMethod
////
///////////////////////////////////////////
//crJXJCheckGamePlanTaskMethod::crJXJCheckGamePlanTaskMethod():
//	m_case(),
//	m_trueExecCount()
//{
//}
//crJXJCheckGamePlanTaskMethod::crJXJCheckGamePlanTaskMethod(const crJXJCheckGamePlanTaskMethod& handle):
//	crMethod(handle),
//	m_case(handle.m_case),
//	m_trueExecCount(handle.m_trueExecCount)
//{
//}
//void crJXJCheckGamePlanTaskMethod::inputParam(int i, void *param)
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
//		m_this = (crCanvasNode*)param;
//		break;
//	}
//}
//void crJXJCheckGamePlanTaskMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_case = (int)(atoi(str.c_str()));
//		break;
//	case 1:
//		m_trueExecCount = (char)(atoi(str.c_str()));
//		break;
//	}
//}
//void crJXJCheckGamePlanTaskMethod::operator()(crHandle &handle)
//{
//	std::string test1 = "主公勤勉执政，大得人心。";
//	std::string test2 = "完成以下任务将激活";
//	std::string test2_1 = "“国富民强”";
//	std::string test3 = "";
//	std::string test4 = "";
//	int b;
//	crStaticTextWidgetNode *staticText1 = NULL;
//	crHypertextWidgetNode *staticText2 = NULL;
//	crStaticTextWidgetNode *staticText3 = NULL;
//	crStaticTextWidgetNode *staticText4 = NULL;
//	//ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas("UI_Qiyu");
//	//ref_ptr<crCanvasNode> tips = m_this->getParentCanvas();
//	void *param;
//	//crData *myMetierData = m_this->getMainRole()->getMetierDataClass();
//	ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
//	crData *myMetierData = playergamedata->getMainRole()->getMetierDataClass();
//	if (myMetierData)
//	{
//		crGameTask *task;
//		myMetierData->excHandle(MAKECREPARAM(WCH_LockData,1));
//		myMetierData->getParam(WCHDATA_GameTaskMap,param);
//		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
//		if(!gameTaskMap||gameTaskMap->empty())
//		{
//			myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));
//			return;
//		}
//		GameTaskMap::iterator preItr;
//		for( GameTaskMap::iterator itr = gameTaskMap->begin();
//			itr != gameTaskMap->end();
//			++itr )
//		{
//			task = itr->second.get();
//			if (task->getTaskID() >= 23000 && task->getTaskID() <= 23004)
//			{
//				if (task->getActivation() == 1)
//				{
//					b = task->getTaskID();
//					break;
//				}
//			}
//		}
//		myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));
//	}
//	switch (b)
//	{
//	case 23000:
//		test2_1 = "国富民强";
//		test3 = "任务：战场击杀玩家士兵数量达到20000";
//		test4 = "任务奖励：税收增加20%";
//		break;
//	case 23001:
//		test2_1 = "国富民强";
//		test3 = "任务：战场击杀士兵数量达到30000";
//		test4 = "任务奖励：农田产量增加20%";
//		break;
//	case 23002:
//		test2_1 = "国富民强";
//		test3 = "任务：战场击杀弓兵数量达到10000";
//		test4 = "任务奖励：林地产量增加20%";
//		break;
//	case 23003:
//		test2_1 = "国富民强";
//		test3 = "任务：战场击杀步兵数量达到10000";
//		test4 = "任务奖励：铁矿产量增加20%";
//		break;
//	case 23004:
//		test2_1 = "国富民强";
//		test3 = "任务：战场击杀骑兵数量达到10000";
//		test4 = "任务奖励：牧场产量增加20%";
//		break;
//	default:
//		break;
//	}
//	staticText1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget("UI_Qiyu_Wen"));
//	if (staticText1)
//	{
//		staticText1->setString(test1);
//		staticText1->setColor(FCOLOR_BLACK);
//	}
//	staticText2 = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget("UI_Qiyu_Wen2"));
//	if (staticText2)
//	{
//			std::vector<float>v_i;
//			ItemMap itemMap;
//			rcfg::ConfigScript cfg_script(&itemMap);
//
//			cfg_script.Add("Hypertext");
//			cfg_script.Push("Hypertext");
//			cfg_script.Add("Content");
//			cfg_script.Push("Content",1);
//			cfg_script.Add("Text",test2);
//			cfg_script.Pop();
//
//			cfg_script.Add("Content");
//			cfg_script.Push("Content",2);
//			v_i.clear();
//			v_i.push_back(255.0f);
//			v_i.push_back(220.0f);
//			v_i.push_back(50.0f);
//			v_i.push_back(255.0f);
//			cfg_script.Add("Color",v_i);
//			cfg_script.Add("Text",test2_1);
//			cfg_script.Pop();
//
//			cfg_script.Pop();
//
//			if(staticText2) staticText2->setHypertext(cfg_script);
//	}
//	staticText3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget("UI_Qiyu_Renwu"));
//	if (staticText3)
//	{
//		staticText3->setString(test3);
//		staticText3->setColor(FCOLOR_RED);
//	}
//	staticText4 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget("UI_Qiyu_Jianli"));
//	if (staticText4)
//	{
//		staticText4->setString(test4);
//		staticText4->setColor(FCOLOR_RED);
//	}
//// 	bool check = false;
//// 	void *param;
//// 	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//// 	crData *data = playerData->getDataClass();
//// 	if(playerData && data)
//// 	{
//// 		switch (m_case)
//// 		{
//// 		case 0:
//// 			if (m_trueExecCount != 0)
//// 			{
//// 				data->getParam(WCHDATA_JXJKillPlayerCount,param);
//// 				int KillPlayerNum = *(int *)param;
//// 				if(KillPlayerNum >= 20000)
//// 				{
//// 					check = true;
//// 					if(m_trueExecCount>0)
//// 						m_trueExecCount--;
//// 				}
//// 			}
//// 			break;
//// 		case 1:
//// 			if (m_trueExecCount != 0)
//// 			{
//// 				data->getParam(WCHDATA_JXJKillPlayerCount,param);
//// 				int KillPlayerCount = *(int *)param;
//// 				data->getParam(WCHDATA_JXJKillNpcCount,param);
//// 				int KillnpcCount = *(int *)param;
//// 				int KillPawnNum = KillPlayerCount + KillPlayerCount;
//// 				if(KillPawnNum >= 30000)
//// 				{
//// 					check = true;
//// 					if(m_trueExecCount>0)
//// 						m_trueExecCount--;
//// 				}
//// 			}
//// 			break;
//// 		case 2:
//// 			if (m_trueExecCount != 0)
//// 			{
//// 				data->getParam(WCHDATA_JXJKillEachTroopsCount,param);
//// 				crVector4i * ckilleachtroops = (crVector4i *)param;
//// 				int KillGongbingNum = (*ckilleachtroops)[1];
//// 				if(KillGongbingNum >= 10000)
//// 				{
//// 					check = true;
//// 					if(m_trueExecCount>0)
//// 						m_trueExecCount--;
//// 				}
//// 			}
//// 			break;
//// 		case 3:
//// 			if (m_trueExecCount != 0)
//// 			{
//// 				data->getParam(WCHDATA_JXJKillEachTroopsCount,param);
//// 				crVector4i * ckilleachtroops = (crVector4i *)param;
//// 				int KillGongbingNum = (*ckilleachtroops)[0];
//// 				if(KillGongbingNum >= 10000)
//// 				{
//// 					check = true;
//// 					if(m_trueExecCount>0)
//// 						m_trueExecCount--;
//// 				}
//// 			}
//// 			break;
//// 		case 4:
//// 			if (m_trueExecCount != 0)
//// 			{
//// 				data->getParam(WCHDATA_JXJKillEachTroopsCount,param);
//// 				crVector4i * ckilleachtroops = (crVector4i *)param;
//// 				int KillGongbingNum = (*ckilleachtroops)[2];
//// 				if(KillGongbingNum >= 10000)
//// 				{
//// 					check = true;
//// 					if(m_trueExecCount>0)
//// 						m_trueExecCount--;
//// 				}
//// 			}
//// 			break;
//// 		default:
//// 			break;
//// 		}
//// 		//handle.outputParam(0,&check);
//// 	}
//	
//// 	bool check = false;
//// 	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//// 	if(playerData && playerData->getDataClass())
//// 	{
//// 		void *param;
//// 		if (m_trueExecCount != 0)
//// 		{
//// 			crData *data = playerData->getDataClass();
//// 			if (data)
//// 			{
//// 				std::vector<std::string> record;
//// 				ref_ptr<crTableIO> taskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTab);
//// 				int paramid = taskTab->getTitleIndex("参数1");
//// 				if(taskTab->queryOneRecord(0,crArgumentParser::appItoa(m_this->getTaskID()),record)>=0)
//// 				{
//// 					int totaljifen = atoi(record[paramid].c_str());
//// 					data->getParam(WCHDATA_JXJGuoZhanJiFen,param);
//// 					int jifen = *(int*)param;
//// 					if (jifen >= totaljifen)
//// 					{
//// 						check = true;
//// 						if(m_trueExecCount>0)
//// 							m_trueExecCount--;
//// 					}	
//// 				}
//// 			}
//// 		}
//// 	}
//}

/////////////////////////////////////////
//
//crJXJAttachStoreFulianMethod
//
/////////////////////////////////////////
crJXJQiyuTaskUpdateMethod::crJXJQiyuTaskUpdateMethod():
	m_dt(NULL),
	m_time(1800)
{
}
crJXJQiyuTaskUpdateMethod::crJXJQiyuTaskUpdateMethod(const crJXJQiyuTaskUpdateMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_time(1800)
{
}
void crJXJQiyuTaskUpdateMethod::inputParam(int i, void *param)
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
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float *)(LOCREPARAM(param64));
		}
		break;
	}
}
void crJXJQiyuTaskUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_time = atoi(str.c_str());
		break;
	}
}
void crJXJQiyuTaskUpdateMethod::operator()(crHandle &handle)
{
	if(m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_this->getMainRole()->getMetierDataClass())
	{
		crData *roledata = m_this->getMainRole()->getDataClass();
		void *param;
		roledata->getParam(WCHDATA_Level,param);
		unsigned char mylv = *(unsigned char *)param;
		int newbieLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJNewbieEndLevel,0).c_str());
		roledata->getParam(WCHDATA_JXJQiyuTaskID,param);
		int qiyutaskid = *(int *)param;
		if(mylv>=newbieLevel && qiyutaskid==0)
		{
			std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQiYuStartTime,0);
			std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQiYuEndTime,0);
			time_t t = time(0);
			char tmp[20];
			void *param;
			strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
			crVector3i opentm,endtm,curtm;
			crArgumentParser::appTimetoVec(opentime,opentm);
			crArgumentParser::appTimetoVec(endtime,endtm);
			crArgumentParser::appTimetoVec(tmp,curtm);
			crData *myMetierData = m_this->getMainRole()->getMetierDataClass();
			if (curtm > opentm && curtm <= endtm)
			{
				/*激活奇遇任务*/
				//crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
				crData *data = m_this->getDataClass();
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJOnlineTimer,param);
				int tt = *(int *)param;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				if (tt >= m_time)
				{
					ref_ptr<crTableIO> taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
					crTableIO::DataVec datavec;
					taskTitleTab->queryRecords(0,"8",datavec);
					if(!datavec.empty())
					{
						int taskidindex = taskTitleTab->getTitleIndex("taskid");
						CRCore::rangei dices(0,datavec.size());
						int id = dices.get_random();
						qiyutaskid = atoi(datavec[id][taskidindex].c_str());
						roledata->inputParam(WCHDATA_JXJQiyuTaskID,&qiyutaskid);
						ref_ptr<crGameTask>gameTask;
						myMetierData->excHandle(MAKECREPARAM(WCH_LockData,1));
						myMetierData->getParam(WCHDATA_GameTaskMap,param);
						GameTaskMap *gameTaskMap = (GameTaskMap *)param;
						GameTaskMap::iterator itr = gameTaskMap->find(qiyutaskid);
						if(itr != gameTaskMap->end())
						{
							gameTask = itr->second;
							gameTask->refresh(0);
							gameTask->setActivation(true);
						}
						myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(4);
						stream->_writeInt(qiyutaskid);
						int playerid = m_this->getPlayerID();
						crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
						if(netConductor)
						{//server to client
							crJXJPlayerMetierEventPacket packet;
							crJXJPlayerMetierEventPacket::buildReplyPacket(packet,playerid,WCH_RecvGetTask,stream.get());
							ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(playerid));
							if(playerData.valid())
								netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

							crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
							if(dbConductor)
							{//任务日志
								crNetDataManager *netDataManager = netConductor->getNetDataManager();
								crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
								int serverid = callback->getServerID();
								crDBSavePacket dbpacket;
								ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;
								insertTaskrecord->buildUpdateSql(playerid,serverid,m_this->getCharacterName(),qiyutaskid,TS_Activation,gameTask->getTaskProgress());
								crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
								dbConductor->getNetManager()->sendPacket("all",dbpacket);
							}
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////
////
////crJXJCheckQiYuTaskUpdateMethod
////
///////////////////////////////////////////
///*检索是否有奇遇任务激活*/
//crJXJCheckQiYuTaskUpdateMethod::crJXJCheckQiYuTaskUpdateMethod():
//	m_dt(NULL)
//{
//}
//crJXJCheckQiYuTaskUpdateMethod::crJXJCheckQiYuTaskUpdateMethod(const crJXJCheckQiYuTaskUpdateMethod& handle):
//	crMethod(handle),
//	m_dt(NULL)
//{
//}
//void crJXJCheckQiYuTaskUpdateMethod::inputParam(int i, void *param)
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
//		m_this = (crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_dt = (float *)(LOCREPARAM(param64));
//		}
//		break;
//	}
//}
//void crJXJCheckQiYuTaskUpdateMethod::addParam(int i, const std::string& str)
//{
//}
//void crJXJCheckQiYuTaskUpdateMethod::operator()(crHandle &handle)
//{
//	static bool done = false;	
//	void *param;
//	bool success = false;
//	//crData *myMetierData = m_this->getMainRole()->getMetierDataClass();
//	ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
//	crData *myMetierData = playergamedata->getMainRole()->getMetierDataClass();
//	if (myMetierData && !done)
//	{
//		crGameTask *task;
//		myMetierData->excHandle(MAKECREPARAM(WCH_LockData,1));
//		myMetierData->getParam(WCHDATA_GameTaskMap,param);
//		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
//		if(!gameTaskMap||gameTaskMap->empty())
//		{
//			myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));
//			return;
//		}
//		GameTaskMap::iterator preItr;
//		for( GameTaskMap::iterator itr = gameTaskMap->begin();
//			itr != gameTaskMap->end();
//			++itr )
//		{
//			task = itr->second.get();
//			if (task->getTaskID() >= 23000 && task->getTaskID() <= 23004)
//			{
//				if (task->getActivation() == 1)
//				{
//					success = true;
//					done = true;
//
//				}
//			}
//		}
//		myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));
//	}
//	handle.outputParam(0,&success);
//}

/////////////////////////////////////////
//
//crJXJUIDixingAdditionTipMethod
//
/////////////////////////////////////////
crJXJUIDixingAdditionTipMethod::crJXJUIDixingAdditionTipMethod(){}

crJXJUIDixingAdditionTipMethod::crJXJUIDixingAdditionTipMethod(const crJXJUIDixingAdditionTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_str(0)
{
}
void crJXJUIDixingAdditionTipMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIDixingAdditionTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	}
}
void crJXJUIDixingAdditionTipMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crData> data = playergamedata->getDataClass();
	data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
	unsigned char fubenOrChengchiType = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
	unsigned short fubenOrChengchiID = *(unsigned short *)param;
	ref_ptr<crTableIO> TroopsChengchiTab = fubenOrChengchiType==1?crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsChengchiTab):crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsFubenTab);
	crTableIO::StrVec record;
	if(TroopsChengchiTab->queryOneRecord(0,crArgumentParser::appItoa(fubenOrChengchiID),record)>=0)
	{
		int desindex = TroopsChengchiTab->getTitleIndex("描述");
		ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
		crStaticTextWidgetNode *tipsText = NULL;
		tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
		if (tipsText)
		{
			tipsText->setString(record[desindex]);
			tipsText->setColor(FCOLOR_BLACK);
		}
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		tips->setCanFocus(false);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJGetViplvRewardTipMethod
//
/////////////////////////////////////////
crJXJGetViplvRewardTipMethod::crJXJGetViplvRewardTipMethod():
	m_this(NULL)
{
}

crJXJGetViplvRewardTipMethod::crJXJGetViplvRewardTipMethod(const crJXJGetViplvRewardTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_this(NULL),
	m_str(0)
{
}
void crJXJGetViplvRewardTipMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJGetViplvRewardTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	}
}
void crJXJGetViplvRewardTipMethod::operator()(crHandle &handle)
{
	void *param;
// 	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
// 	crData *data = myPlayer->getDataClass();
	//data->getParam(WCHDATA_VipLv,param);
	//unsigned char viplv = *(unsigned char *)param;

	ref_ptr<crCanvasNode> btnNode = crFilterRenderManager::getInstance()->findCanvas("UI_ArmyAdviser");

	crData *canvasdata = btnNode->getDataClass();
	canvasdata->getParam(WCHDATA_JXJVipCanvasCurPageNum,param);
	int viplv = *(int *)param;

	ref_ptr<crButtonWidgetNode> lingqubtn = dynamic_cast<crButtonWidgetNode *>(btnNode->getWidget("VIP_Linqu"));
	bool lingqu = lingqubtn->getEnable();

	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	crStaticTextWidgetNode *tipsText = NULL;
	tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
	std::string str;
	if (viplv == 0)
	{
		viplv = viplv + 1;
	}
	str = "VIP " + crArgumentParser::appItoa(viplv) + "及以上可领取";
	if (!lingqu)
	{
		if (tipsText)
		{
			tipsText->setString(str);
			tipsText->setColor(FCOLOR_BLACK);
		}
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		tips->setCanFocus(false);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}


}
