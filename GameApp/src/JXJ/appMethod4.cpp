/**********************************************************************
*
*	文件:	   appMethod4.cpp
*
*	描述:	   
*
*	作者:		杨勇
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod4.h>
#include <JXJ/appMethod16.h>
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
#include <CREncapsulation/crStartHandler.h>
#include <JXJ/appData2.h>
#include <JXJ/appData3.h>
#include <stdlib.h>
#include <JXJ/appDBQuery.h>

using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRUI;
using namespace CRPhysics;
using namespace CRIOManager;
using namespace CRProducer;
using namespace CRUtil;

/////////////////////////////////////////
//
//crJXJUIRecruitTipMethod
//
/////////////////////////////////////////
crJXJUIRecruitTipMethod::crJXJUIRecruitTipMethod()
{
}
crJXJUIRecruitTipMethod::crJXJUIRecruitTipMethod(const crJXJUIRecruitTipMethod& handle):
	crMethod(handle),
	m_armInput(handle.m_armInput),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIRecruitTipMethod::inputParam(int i, void *param)
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
void crJXJUIRecruitTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armInput = str;
		break;
	case 1:
		m_tipsCanvas = str;
		break;
	case 2:
		m_tipsTextWidget = str;
		break;
	}
}
void crJXJUIRecruitTipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(tips.valid() && myPlayer)
	{
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
		std::string str;
		crCanvasNode *canvas = m_this->getParentCanvas();
		crEditWidgetNode *armyInput = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_armInput));
		int armys = _wtoi(armyInput->getString().c_str());
		do 
		{
			if(armys <= 0)
			{
				str = "输入的数量有误";
				break;

			}
			void *param;
			crData *data = crBrain::getInstance()->getDataClass();
			data->getParam(WCHDATA_JXJCurSelRecruitArmAID,param);
			int selAbastactid = *(int *)param;
			if(selAbastactid<=0)
			{
				str = "选中的部队有误";
				break;
			}

			RecruitCheckResult recruitCheck;
			myPlayer->doEvent(WCH_JXJRecruitCheck,MAKEINT64(selAbastactid,&recruitCheck));
			//0不能招募，1正常，-1部队人数上限, -2队伍数量上限，-3兵营等级不够， -4驻地条件, -5没有招募时间，-6装备不足，-7铜钱不足，-8粮食不足
			switch (recruitCheck.m_code)
			{
			case 0:
				str = "不能招募";
				break;
			case -1:
				str = "部队人数上限";
				break;
			case -2:
				str = "队伍数量上限";
				break;
			case -3:
				str = "兵营等级不够";
				break;
			case -4:
				str = "驻地条件"; 
				break;
			case -5:
				str = "没有征兵时间";
				break;
			//case -6:
			//	str = "装备不足";
			//	break;
			//T_Food = 1,
			//	T_Wood,
			//	T_Iron,
			//	T_Horse,
			//	T_Copper,
			case -7:
				str = "粮食不足";
				break;
			case -8:
				str = "木材不足"; 
				break;
			case -9:
				str = "铁矿不足"; 
				break;
			case -10:
				str = "马匹不足"; 
				break;
			case -11:
				str = "铜钱不足"; 
				break;
			case -12:
				str = "不可招募"; 
				break;
			}
		} while (0);
		tipsText->setString(str);
		tipsText->setColor(crVector4(255.0,255.0,0.0,255.0));
		if (!str.empty())
		{
			const crBoundingBox &btnbox = m_this->getBoundBox();
			//
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
			//
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBuildingLevelUpTipMethod
//
/////////////////////////////////////////
crJXJUIBuildingLevelUpTipMethod::crJXJUIBuildingLevelUpTipMethod()
{
}
crJXJUIBuildingLevelUpTipMethod::crJXJUIBuildingLevelUpTipMethod(const crJXJUIBuildingLevelUpTipMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIBuildingLevelUpTipMethod::inputParam(int i, void *param)
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
void crJXJUIBuildingLevelUpTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	}
}

void crJXJUIBuildingLevelUpTipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(myPlayer && tips.valid())
	{
		tips->setCanFocus(false);
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
		std::string str;
		///////////
		
		void *param;
		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
		canvasdata -> getParam(WCHDATA_JXJCheckBuildingID,param);
		int buildingid = *(int *)param;

		BuildingLevelUpCheckResult result;
		result.m_code = 0;
		myPlayer->doEvent(WCH_JXJBuildingLevelUpCheck,MAKEINT64(buildingid,&result));
		switch (result.m_code)
		{
		case 0:
			str = "不能升级";
			break;
		case -1:
			if (buildingid == WCHDATA_JXJFuyalv)
			{
				str = "领主等级不足";
			}
			else
			{
				str = "府衙等级不足";
			}
			break;
		case -2:
			str = "时间不足";
			break;
		case -3:
			str = "铜钱不足";
			break;
		case -4:
			str = "木材不足";
			break;
		case -5:
			str = "铁矿不足";
			break;
		case -6:
			str = "粮食不足";
			break;
		case -7:
			str = "";
			break;
		case -8:
			str = "超过等级上限";
			break;
		}
		tipsText->setString(str);
		tipsText->setColor(FCOLOR_RED);

		if(!str.empty())
		{
			const crBoundingBox &btnbox = m_this->getBoundBox();
			//
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
			//
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIGongFangBDDZTipMethod
//
/////////////////////////////////////////
crJXJUIGongFangBDDZTipMethod::crJXJUIGongFangBDDZTipMethod()
{

}
crJXJUIGongFangBDDZTipMethod::crJXJUIGongFangBDDZTipMethod(const crJXJUIGongFangBDDZTipMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIGongFangBDDZTipMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangBDDZTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_inputCount = str;
		break;
	case 1:
		m_tipsCanvas = str;
		break;
	case 2:
		m_tipsTextWidget = str;
		break;
	}
}
void crJXJUIGongFangBDDZTipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(tips.valid() && m_this->getParentCanvas() && myPlayer)
	{
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
		std::string str;
		crCanvasNode *canvas = m_this->getParentCanvas();
		crEditWidgetNode *inputCount = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_inputCount));
		void *param;
		unsigned short equipid = 0;
		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJCurSelForgeTroopsEquipID,param);
			equipid = *(unsigned short *)param;
		}
		if(equipid > 0)
		{
			int makecount = 0;
			if (inputCount)
			{
				if(!inputCount->getString().empty())
				{
					makecount = _wtoi(inputCount->getString().c_str());
					if(makecount == 0)
						str = "输入的数量不正确";
				}
				else
				{
					str = "数量不能为空";
				}				
			}
			ref_ptr<crTableIO>troopsEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
			int startIndex = troopsEquipTab->getTitleIndex("铜钱");
			int indexCount = troopsEquipTab->getColumnCount();
			std::string name;
			crTableIO::StrVec record;
			if (troopsEquipTab->queryOneRecord(0,crArgumentParser::appItoa(equipid),record)<0)
			{
				return;
			}
			//int count = atoi(needCount[0]->getUTF8String().c_str());
			int i = 0;
			int mycount,total,maxmakecount,count,resid;
			std::vector< std::pair<int,int> >ResCountVec;
			crData *data = myPlayer->getDataClass();
			ref_ptr<crTableIO>resnameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResNameTab);
			for(int i = startIndex; i<indexCount; i++)
			{
				count = atoi(record[i].c_str());
				if(count>0)
				{
					ResCountVec.push_back(std::make_pair(i,count));
				}
			}
			for( std::vector< std::pair<int,int> >::iterator itr = ResCountVec.begin();
					itr != ResCountVec.end() && i<4;
					++itr,++i )
			{
				name = troopsEquipTab->getTitle(itr->first);
				int need = itr->second;
				if (resnameTab->queryOneRecord(1,name,record) >= 0)
				{
					resid = atoi(record[0].c_str());
					data->getParam(resid,param);
					maxmakecount = INT_MAX/need; //整型最大值
					if (makecount<=maxmakecount)
					{
						total = need * makecount;
					}
					else
					{
						total = INT_MAX;
					}
					mycount = *(int *)param;
					if(mycount<total)
					{
						if(i == 0)	str = "铜钱不足";
						else if(i == 1) str = "木材不足";
						else if(i == 2) str = "铁矿不足";
						else if(i == 3) str = "马匹不足";
						break;
					}
				}
			}
		}
		else
		{
			str = "选中的部队不正确";
		}
		if (!str.empty())
		{
			tipsText->setString(str);
			tipsText->setColor(FCOLOR_RED);
			const crBoundingBox &btnbox = m_this->getBoundBox();
			//
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
			//
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGongFangLZDZTipMethod
//
/////////////////////////////////////////
crJXJUIGongFangLZDZTipMethod::crJXJUIGongFangLZDZTipMethod():
	m_this(NULL),
	m_lordAbstractID(0)
{
}

crJXJUIGongFangLZDZTipMethod::crJXJUIGongFangLZDZTipMethod(const crJXJUIGongFangLZDZTipMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget),
	m_this(NULL),
	m_lordAbstractID(handle.m_lordAbstractID)
{
}
void crJXJUIGongFangLZDZTipMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangLZDZTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	}
}

void crJXJUIGongFangLZDZTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if (tips.valid())
	{
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
		if(tipsText)
		{
			std::string str = tipsText->getUTF8String();
			if (!str.empty())
			{
				const crBoundingBox &btnbox = m_this->getBoundBox();
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				float posx = btnbox.m_min[0];
				float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
				//		//
				crVector2 mouse(crVector2(posx,posy));
				tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
				crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGongfangEquipPageResetMethod
//
/////////////////////////////////////////
crJXJUIGongfangEquipPageResetMethod::crJXJUIGongfangEquipPageResetMethod()
{
}
crJXJUIGongfangEquipPageResetMethod::crJXJUIGongfangEquipPageResetMethod(const crJXJUIGongfangEquipPageResetMethod& handle):
	crMethod(handle),
	m_pageBudui(handle.m_pageBudui),
	m_pageLingzhu(handle.m_pageLingzhu)
{
}
void crJXJUIGongfangEquipPageResetMethod::inputParam(int i, void *param)
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
void crJXJUIGongfangEquipPageResetMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_pageBudui = str;
		break;
	case 1:
		m_pageLingzhu = str;
		break;
	}
}
void crJXJUIGongfangEquipPageResetMethod::operator()(crHandle &handle)
{
	//void *param;
	crData *canvasdata = m_this->getParentCanvas()->getDataClass();
	//canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
	//canvasdata->getParam(WCHDATA_JXJCurForgeEquipID,param);
	//int* curArmID = (int *)param;
	//*curArmID = 0;
	if (canvasdata)
	{
		canvasdata->inputParam(WCHDATA_JXJCurSelForgeTroopsEquipID,0);
		canvasdata->inputParam(WCHDATA_JXJCurSelForgeLordEquipID,0);
	}
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	crMultiSwitch *pageBudui = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_pageBudui));
	if(pageBudui)
		pageBudui->setActiveSwitchSet(m_this->getSelect());
	crMultiSwitch *pageLingzhu = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_pageLingzhu));
	if(pageLingzhu)
		pageLingzhu->setActiveSwitchSet(m_this->getSelect());
	canvas->doEvent(WCH_UI_UpdateData);
	//canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
}

/////////////////////////////////////////
//
//crJXJUISystemIconTipMethod
//
/////////////////////////////////////////
crJXJUISystemIconTipMethod::crJXJUISystemIconTipMethod():
	m_this(NULL),
	m_timeid(0),
	m_cdhintid(0)
{
}

crJXJUISystemIconTipMethod::crJXJUISystemIconTipMethod(const crJXJUISystemIconTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tiptext(handle.m_tiptext),
	m_this(NULL),
	m_str(0),
	m_timeid(handle.m_timeid),
	m_cdhintid(handle.m_cdhintid)
{
}
void crJXJUISystemIconTipMethod::inputParam(int i, void *param)
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
void crJXJUISystemIconTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tiptext = str;
		break;
	case 2:
		m_str = atoi(str.c_str());
		break;
	case 3:
		m_cdhintid = atoi(str.c_str());
		break;
	case 4:
		m_timeid = atoi(str.c_str());
		break;
	}
}

void crJXJUISystemIconTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	if(!tips.valid())
		return;

	crStaticTextWidgetNode  *tipsText = NULL;
	ref_ptr<crHypertextWidgetNode>  tipsTexth = NULL;

	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);

	 if (m_str<3 || m_str>10)
	 {
		 tipsText = dynamic_cast< crStaticTextWidgetNode  *>(tips->getWidget(m_tiptext));
	 }
	 else
	 {
		 //计算所需元宝数量
		 int price = 0;
		 ref_ptr<crTableIO>hintTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHintTab);
		 crTableIO::StrVec record;
		 crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		 crData *playerData = myPlayer->getDataClass();
		 void *param;
		 playerData->getParam(m_timeid,param);
		 if(param)
		 {
			 int time = *(int *)param;
			 if(time>0)
			 {
				 if (hintTable->queryOneRecord(0,crArgumentParser::appItoa(m_cdhintid),record)>=0)
				 {
					 price = ceil((float)time * atof(record[2].c_str()));
					 if(price<1) price = 1;
				 }
			 }
		 }

		 tipsTexth = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tiptext));
	
		 cfg_script.Add("Hypertext");
		 cfg_script.Push("Hypertext");

		 cfg_script.Add("Content");
		 cfg_script.Push("Content", 1);
		 v_i.clear();
		 v_i.push_back(0.0f);
		 v_i.push_back(0.0f);
		 v_i.push_back(0.0f);
		 v_i.push_back(255.0f);
		 cfg_script.Add("Color", v_i);
		 cfg_script.Add("Text", "     花费");
		 cfg_script.Pop();

		 cfg_script.Add("Content");
		 cfg_script.Push("Content", 2);
		 v_i.clear();
		 v_i.push_back(140.0f);
		 v_i.push_back(40.0f);
		 v_i.push_back(40.0f);
		 v_i.push_back(255.0f);
		 cfg_script.Add("Color", v_i);
		 char tmpText[32];
		 sprintf(tmpText, "%d礼金/元宝\0", price);
		 cfg_script.Add("Text", tmpText);
		 cfg_script.Pop();

		 cfg_script.Add("Content");
		 cfg_script.Push("Content", 3);
		 v_i.clear();
		 v_i.push_back(0.0f);
		 v_i.push_back(0.0f);
		 v_i.push_back(0.0f);
		 v_i.push_back(255.0f);
		 cfg_script.Add("Color", v_i);
	 }
	std::string str;
	switch (m_str)
	{
	case 1:
		str = "邮箱";
		break;
	case 2:
		str = "系统设置";
		break;
	case 3:
		cfg_script.Add("Text", "清除建筑时间");
		cfg_script.Pop();
		//str = "清除建筑时间";
		break;
	case 4:
		cfg_script.Add("Text", "清除建筑时间");
		cfg_script.Pop();
		//str = "清除建筑时间";
		break;
	case 5:
		cfg_script.Add("Text", "清除建筑时间");
		cfg_script.Pop();
		//str = "清除建筑时间";
		break;
	case 6:
		cfg_script.Add("Text", "清除建筑时间");
		cfg_script.Pop();
		//str = "清除建筑时间";
		break;
	case 7:
		cfg_script.Add("Text", "清除征兵时间");
		cfg_script.Pop();
		//str = "清除征兵时间";
		break;
	case 8:
		cfg_script.Add("Text", "清除征兵时间");
		cfg_script.Pop();
		//str = "清除征兵时间";
		break;
	case 9:
		cfg_script.Add("Text", "清除征兵时间");
		cfg_script.Pop();
		//str = "清除征兵时间";
		break;
	case 10:
		cfg_script.Add("Text", "清除征兵时间");
		cfg_script.Pop();
		//str = "清除征兵时间";
		break;
	case 11:
		str = "购买建筑时间条";
		break;
	case 12:
		str = "购买征兵时间条";
		break;
	case 13:
		{
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			crData *data = myPlayer->getDataClass();

			void *param;
			data->getParam(WCHDATA_JXJChengchiID,param);
			unsigned short stationid = *(unsigned short *)param;

			crTableIO::StrVec record;
			ref_ptr<crTableIO>chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			int idIndex = chengchitab->getTitleIndex("id");
			int citynameindex = chengchitab->getTitleIndex("名字");
			if(chengchitab->queryOneRecord(idIndex, crArgumentParser::appItoa(stationid), record) >= 0)
			{
				str = "当前领地驻扎在【" + record[citynameindex] + "】";
			}
			
		}
		break;
	case 14:
		{
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			crData *data = myPlayer->getDataClass();

			void *param;
			data->getParam(WCHDATA_JXJAutoBuilding,param);
			bool autobuilding = *(bool *)param;
			if(autobuilding)
			{
				str = "建筑自动升级中";
			}
			else
			{
				str = "开启建筑自动升级";
			}
		}
		break;
	case 15:
		if (!m_this->getEnable())
		{
			str = "本次国战尚未结束或者您未参与";
		}
		break;
	case 100:
		if (!m_this->getEnable())
		{
			str = "等级不足或前置副本未通关";
		}
		break;
	case 101:
		if (!m_this->getEnable())
		{
			str = "请先研发相关科技";
		}
		break;
	}
	if(m_str<3 || m_str>10)
	{
		if (!str.empty())
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
	else
	{
		 cfg_script.Pop();

		 if(tipsTexth.valid()) tipsTexth->setHypertext(cfg_script);

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

///////////////////////////////////////////
////
////crJXJUITavernCardTicketsTipMethod
////
///////////////////////////////////////////
//crJXJUITavernCardTicketsTipMethod::crJXJUITavernCardTicketsTipMethod():
//	m_this(NULL)
//{
//}
//
//crJXJUITavernCardTicketsTipMethod::crJXJUITavernCardTicketsTipMethod(const crJXJUITavernCardTicketsTipMethod& handle):
//	crMethod(handle),
//	m_tips(handle.m_tips),
//	m_this(NULL)
//{
//}
//void crJXJUITavernCardTicketsTipMethod::inputParam(int i, void *param)
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
//		m_this = (crWidgetNode*)param;
//		break;
//	}
//}
//void crJXJUITavernCardTicketsTipMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_tips = str;
//		break;
//	case 1:
//		m_type = atoi(str.c_str());
//		break;
//	case 2:
//		m_raido = str;
//		break;
//	case 3:
//		m_jinengmiaoshu = str;
//		break;
//	case 4:
//		m_card = str;
//		break;
//	case 5:
//		m_nameimage = str;
//		break;
//	case 6:
//		m_backboard = str;
//		break;
//	}
//}
//
//void crJXJUITavernCardTicketsTipMethod::operator()(crHandle &handle)
//{
//	crCanvasNode *canvas = m_this->getParentCanvas();
//	crCanvasNode *tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(tipsCanvas && playerData)
//	{
//		crHypertextWidgetNode *jinengmiaoshu = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_jinengmiaoshu));
//		crImageBoxWidgetNode *card = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_card));
//		crImageBoxWidgetNode *nameimage = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_nameimage));
//		crMultiSwitch *backboard = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_backboard));
//		void *param;
//		int cardid;
//		crData *data = playerData->getDataClass();
//		crRadioGroupWidgetNode *raido = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_raido));
//		int select = raido->getSelect();
//		if(select == 0)
//		{
//			data->getParam(WCHDATA_JXJHallCard,param);
//			int *hallCard = (int *)param;
//			cardid = *(hallCard + m_type);
//		}
//		else
//		{
//			data->getParam(WCHDATA_JXJPrivateCard,param);
//			int *privateCard = (int *)param;
//			cardid = *(privateCard + m_type - 1);
//		}
//		if(cardid>0)
//		{
//			ref_ptr<crTableIO>jiangkaEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//
//			int jinengmiaoshuid = jiangkaEquipTab->getTitleIndex("技能描述");
//			int jiangcardid = jiangkaEquipTab->getTitleIndex("picture");
//			int nameimageid = jiangkaEquipTab->getTitleIndex("姓名");
//			int backboardid = jiangkaEquipTab->getTitleIndex("品质");
//			crTableIO::StrVec record;
//			jiangkaEquipTab->queryOneRecord(0,crArgumentParser::appItoa(cardid),record);
//			if (record.size())
//			{
//				std::string file;
//				if(jinengmiaoshu)
//				{
//					file = record[jinengmiaoshuid];
//					if(!file.empty())
//					{
//						jinengmiaoshu->clearText();
//						file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
//						jinengmiaoshu->setHypertext(file);
//					}
//					else
//					{
//						jinengmiaoshu->clearText();
//					}
//				}
//				if(card)
//				{
//					card->setImageName(record[jiangcardid].c_str());
//				}
//				if(nameimage)
//				{
//					nameimage->setImageName(record[nameimageid].c_str());
//				}
//				if(backboard)
//				{
//					backboard->setActiveSwitchSet(atoi(record[backboardid].c_str()));
//				}
//				ref_ptr<crTableIO>ItemTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//				crTableIO::StrVec record;
//				ItemTable->queryOneRecord(0,crArgumentParser::appItoa(cardid),record);
//				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
//				const crBoundingBox &iconbox = m_this->getBoundBox();
//				const crBoundingBox &tips = tipsCanvas->getBoundBox();
//				float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
//				float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
//				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
//				/////////////////////////////////////////
//				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
//				float posx2,posy2;
//				/////////////////////////////////////////
//				if (pos2[0] >= 0)
//				{
//					posx2 = pos2[0] - posx;
//				}
//				else
//				{
//					posx2 = pos[0] + posx;
//				}
//				if (pos2[1] >= 0)
//				{
//					posy2 = pos[1] - posy;
//				}
//				else
//				{
//					posy2 = pos2[1] + posy;
//				}
//				crVector2 mouse(crVector2(posx2,posy2));
//				/////////////////////////////////////////
//				tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
//				tipsCanvas->setCanFocus(false);
//				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas,true);
//			}
//		}
//	}
//}


/////////////////////////////////////////
//
//crJXJUIPostRankingChangePageMethod
//
/////////////////////////////////////////
crJXJUIPostRankingChangePageMethod::crJXJUIPostRankingChangePageMethod():
	m_this(NULL)
{
}

crJXJUIPostRankingChangePageMethod::crJXJUIPostRankingChangePageMethod(const crJXJUIPostRankingChangePageMethod& handle):
	crMethod(handle),
	m_step(handle.m_step),
	m_this(NULL)
{
}
void crJXJUIPostRankingChangePageMethod::inputParam(int i, void *param)
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
void crJXJUIPostRankingChangePageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_step = str;
		break;
	}
}

void crJXJUIPostRankingChangePageMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this &&playerGameData)
	{
		void *param;
		int curPageNum,totalPage;
		crData *data = crBrain::getInstance()->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJRankingPageNum,param);
		curPageNum = *(int *)param;
		data->getParam(WCHDATA_JXJRankingTotal,param);
		totalPage = *(int *)param;
		curPageNum += atoi(m_step.c_str());
		if (curPageNum < 1)
		{
			curPageNum = 1;
		}
		else if(curPageNum > totalPage)
		{
			curPageNum = totalPage;
		}
		data->inputParam(WCHDATA_JXJRankingPageNum,&curPageNum);
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJUIPostRankingPageShowMethod
//
/////////////////////////////////////////
crJXJUIPostRankingPageShowMethod::crJXJUIPostRankingPageShowMethod():
	m_this(NULL)
{
}

crJXJUIPostRankingPageShowMethod::crJXJUIPostRankingPageShowMethod(const crJXJUIPostRankingPageShowMethod& handle):
	crMethod(handle),
	m_pageNum(handle.m_pageNum),
	m_rankingList(handle.m_rankingList),
	m_rankingbtn(handle.m_rankingbtn),
	m_tips(handle.m_tips),
	m_tiptext(handle.m_tiptext),
	m_this(NULL)
{
}
void crJXJUIPostRankingPageShowMethod::inputParam(int i, void *param)
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
void crJXJUIPostRankingPageShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_pageNum = str;
		break;
	case 1:
		m_rankingList = str;
		break;
	case 2:
		m_rankingbtn = str;
		break;
	case 3:
		m_tips = str;
		break;
	case 4:
		m_tiptext = str;
		break;
	case 5:
		m_officeShopbtn = str;
		break;
	case 6:
		m_officeListbtn = str;
		break;
	case 7:
		m_mygroupcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJUIPostRankingPageShowMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
		crStaticTextWidgetNode *pageNum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageNum));
		crTableWidgetNode *rankingList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_rankingList));
		crButtonWidgetNode *btnRanking = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rankingbtn));
		crButtonWidgetNode *officeShopbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_officeShopbtn));
		crButtonWidgetNode *officeListbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_officeListbtn));

		crStaticTextWidgetNode *tipsText = NULL;
		ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
		if(tips.valid())
			tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tiptext));
		int pageCount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRankingCount,viplv).c_str()));
		int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,viplv).c_str());
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->getParam(WCHDATA_JXJRankingPageNum,param);
		int curpageNum = *(int * )param;
		int count = 0;
		std::string str;
		int row = 0;
		unsigned int myrankingid;
		myrankingid = myPlayer->getPlayerID();
		if(btnRanking)
		{
			btnRanking->setVisiable(false);
			btnRanking->setEnable(false);
		}
		if(officeShopbtn)
		{
			data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
			unsigned char hadGotAward = *(unsigned char *)param;
			if (hadGotAward == (unsigned char)-1)
			{
				officeShopbtn->setEnable(true);
			}
			else
			{
				officeShopbtn->setEnable(false);
			}
		}
		if(officeListbtn)officeListbtn->setEnable(false);
		if (mygroupcanvas.valid() && mygroupcanvas->getDataClass())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			int myGroupPos = JXJGroup_Normal;
			if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
			if (myGroupPos == JXJGroup_FirstLeader)
			{
				canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
				MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
				if (!groupCantonVec->empty())
				{
					int cantonId = (*groupCantonVec)[0];
					if (cantonId > 0)
					{
						if(officeListbtn)officeListbtn->setEnable(true);
					}
				}
			}
			else
			{
				canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
				ClientFenFengedChengChiMap *fenFengMap = (ClientFenFengedChengChiMap *)param;
				std::string playername = myPlayer->getCharacterName();
				ClientFenFengedChengChiMap::iterator cffccm = fenFengMap->begin();
				for (;cffccm != fenFengMap->end();++cffccm)
				{
					if (playername == cffccm->second)
					{
						if(officeListbtn)officeListbtn->setEnable(true);
					}
				}

			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
		crData *mydata = m_this->getDataClass();
		mydata->getParam(WCHDATA_JXJGovpostRankingMap,param);
		GovpostRankingMap *govpostRankingMap = (GovpostRankingMap *)param;
		if(rankingList)
		{
			mydata->excHandle(MAKEINT64(WCH_LockData,1));
			rankingList->clearData();
			//crRole *mainRole = myPlayer->getMainRole();
			//crData *mdata = mainRole->getDataClass();
			crTableIO::StrVec record;
			ref_ptr<crTableIO>govPostTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
			crData *rankingUI = m_this->getDataClass();
			crPlayerRanking *expRank;
			unsigned char postid;
			for( GovpostRankingMap::iterator itr = govpostRankingMap->begin();
				itr != govpostRankingMap->end();
				++itr,row++)
			{
				count++;
				if(myrankingid == itr->second->getPlayerID())
				{
					//if(btnRanking)
					//{
					//	btnRanking->setVisiable(true);
					//	btnRanking->setEnable(true);
					//}
					if (count >= maxRankingNum)
					{
						str = "没有排名";
						if(tipsText)
							tipsText->setString(str);
					}
				}
				if (row >= 0)
				{
					expRank = itr->second.get();
					rankingList->addData(0,row,crArgumentParser::appItoa(row+1+pageCount*(curpageNum - 1)));
					rankingList->addData(3,row,crArgumentParser::appItoa(expRank->getExploit()));
					rankingList->addData(2,row,expRank->getName());
					postid = expRank->getGovpost();
					if (govPostTab->queryOneRecord(0,crArgumentParser::appItoa(postid),record) >= 0)
					{
						rankingList->addData(1,row,record[1]);
						rankingList->addData(4,row,record[2]);
					}
				}
			}
			mydata->excHandle(MAKEINT64(WCH_LockData,0));
		}
		//int totalPage = ceil((float)count/(float)pageCount);
		//if(totalPage == 0) totalPage = 1;
		//data->excHandle(MAKEINT64(WCH_LockData,1));
		braindata->getParam(WCHDATA_JXJRankingTotal,param);
		int totalPage = *(int *)param;
		//data->excHandle(MAKEINT64(WCH_LockData,0));
		if(pageNum)pageNum->setString(crArgumentParser::appItoa(curpageNum)+"/"+crArgumentParser::appItoa(totalPage));

	}
}

/////////////////////////////////////////
//
//crJXJUIMyExplotRankingMethod
//
/////////////////////////////////////////
crJXJUIMyExplotRankingMethod::crJXJUIMyExplotRankingMethod():
	m_this(NULL)
{
}

crJXJUIMyExplotRankingMethod::crJXJUIMyExplotRankingMethod(const crJXJUIMyExplotRankingMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tiptext(handle.m_tiptext),
	m_pageNum(handle.m_pageNum),
	m_this(NULL)
{
}
void crJXJUIMyExplotRankingMethod::inputParam(int i, void *param)
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
void crJXJUIMyExplotRankingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tiptext = str;
		break;
	case 2:
		m_pageNum = str;
		break;
	}
}

void crJXJUIMyExplotRankingMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	if(tips.valid() && playerData)
	{
		crData *data = playerData->getDataClass();
		data ->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		int pageCount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRankingCount,viplv).c_str()));
		int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,viplv).c_str());
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		crStaticTextWidgetNode *pageNum = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_pageNum));
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tiptext));
		std::string str;
		unsigned int myrankingid;
		myrankingid = playerData->getPlayerID();
		crData *braindata = crBrain::getInstance()->getDataClass();
		//braindata->excHandle(MAKEINT64(WCH_LockData,1));
		braindata->getParam(WCHDATA_JXJRankingPageNum,param);
		int *curpageNum = (int *)param;
		int count = 1;
		crData *mydata = canvas->getDataClass();
		mydata->getParam(WCHDATA_JXJExpRankingMap,param);
		ExpRankingMap *expRankingMap = (ExpRankingMap *)param;
		mydata->excHandle(MAKEINT64(WCH_LockData,1));
		for( ExpRankingMap::iterator itr = expRankingMap->begin();
			itr != expRankingMap->end();
			++itr,count++)
		{
			if(myrankingid == itr->second->getPlayerID())
			{
				if (count % pageCount == 0)
				{
					*curpageNum = count / pageCount;
				}
				else
				{
					*curpageNum = count / pageCount + 1;
				}
			}
		}
		mydata->excHandle(MAKEINT64(WCH_LockData,0));
		//braindata->excHandle(MAKEINT64(WCH_LockData,0));
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1]);
		//
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJUIFuYaKeJiTipMethod
//
/////////////////////////////////////////
//crJXJUIFuYaKeJiTipMethod::crJXJUIFuYaKeJiTipMethod():
//	m_this(NULL)
//{
//}
//
//crJXJUIFuYaKeJiTipMethod::crJXJUIFuYaKeJiTipMethod(const crJXJUIFuYaKeJiTipMethod& handle):
//	crMethod(handle),
//	m_tips(handle.m_tips),
//	m_attributename(handle.m_attributename),
//	m_attributeinfo(handle.m_attributeinfo),
//	m_lordlv(handle.m_lordlv),
//	m_achievementcount(handle.m_achievementcount),
//	m_this(NULL)
//{
//}
//void crJXJUIFuYaKeJiTipMethod::inputParam(int i, void *param)
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
//		m_this = (crWidgetNode*)param;
//		break;
//	}
//}
//void crJXJUIFuYaKeJiTipMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_tips = str;
//		break;
//	case 1:
//		m_type = atoi(str.c_str());
//		break;
//	case 2:
//		m_attributename = str;
//		break;
//	case 3:
//		m_attributeinfo = str;
//		break;
//	case 4:
//		m_lordlv = str;
//		break;
//	case 5:
//		m_achievementcount = str;
//		break;
//	}
//}
//
//void crJXJUIFuYaKeJiTipMethod::operator()(crHandle &handle)
//{
//	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(tipsCanvas.valid() && playerData)
//	{
//		crStaticTextWidgetNode *attributeName = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_attributename));
//		crStaticTextWidgetNode *attributeInfo = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_attributeinfo));
//		crStaticTextWidgetNode *lordLv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_lordlv));
//		crStaticTextWidgetNode *achievementCount = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_achievementcount));
//		void *param;
//		int techid;
//		crData *data = playerData->getDataClass();
//		data->excHandle(MAKEINT64(WCH_LockData,1));
//		switch (m_type)
//		{
//		case Tech_Jingbing:
//			techid = WCHDATA_JXJTechJingBinglv;
//			data->getParam(WCHDATA_JXJTechJingBinglv, param);
//			break;
//		case Tech_JianJia:
//			techid = WCHDATA_JXJTechJianJialv;
//			data->getParam(WCHDATA_JXJTechJianJialv, param);
//			break;
//		case Tech_QiangRen:
//			techid = WCHDATA_JXJTechQiangRenlv;
//			data->getParam(WCHDATA_JXJTechQiangRenlv, param);
//			break;
//		case Tech_GuiMou:
//			techid = WCHDATA_JXJTechGuiMoulv;
//			data->getParam(WCHDATA_JXJTechGuiMoulv, param);
//			break;
//		case Tech_GongCheng:
//			techid = WCHDATA_JXJTechGongChenglv;
//			data->getParam(WCHDATA_JXJTechGongChenglv, param);
//			break;
//		case Tech_YuanGong:
//			techid = WCHDATA_JXJTechYuanGonglv;
//			data->getParam(WCHDATA_JXJTechYuanGonglv, param);
//			break;
//		case Tech_SuGong:
//			techid = WCHDATA_JXJTechSuGonglv;
//			data->getParam(WCHDATA_JXJTechSuGonglv, param);
//			break;
//		case Tech_QiangXing:
//			techid = WCHDATA_JXJTechQiangXinglv;
//			data->getParam(WCHDATA_JXJTechQiangXinglv, param);
//			break;
//		case Tech_TunTian:
//			techid = WCHDATA_JXJTechFoodlv;
//			data->getParam(WCHDATA_JXJTechFoodlv, param);
//			break;
//		case Tech_FanZhi:
//			techid = WCHDATA_JXJTechHorselv;
//			data->getParam(WCHDATA_JXJTechHorselv, param);
//			break;
//		case Tech_YeLian:
//			techid = WCHDATA_JXJTechIronlv;
//			data->getParam(WCHDATA_JXJTechIronlv, param);
//			break;
//		case Tech_FanMao:
//			techid = WCHDATA_JXJTechWoodlv;
//			data->getParam(WCHDATA_JXJTechWoodlv, param);
//			break;
//		case Tech_ZhaoMu:
//			techid = WCHDATA_JXJTechZhaoMulv;
//			data->getParam(WCHDATA_JXJTechZhaoMulv, param);
//			break;
//		case Tech_QiaoJiang:
//			techid = WCHDATA_JXJTechBuildingCDlv;
//			data->getParam(WCHDATA_JXJTechBuildingCDlv, param);
//			break;
//		case Tech_RenDe:
//			techid = WCHDATA_JXJTechRenDelv;
//			data->getParam(WCHDATA_JXJTechRenDelv, param);
//			break;
//		case Tech_ZhengShui:
//			techid = WCHDATA_JXJTechZhengShuilv;
//			data->getParam(WCHDATA_JXJTechZhengShuilv, param);
//			break;
//		default:
//			break;
//		}
//		unsigned char nowlv = *(unsigned char *)param;
//		if(techid>0)
//		{
//			ref_ptr<crTableIO>techTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
//			int attributeNameid = techTab->getTitleIndex("属性名称");
//			int attributeInfoid = techTab->getTitleIndex("属性加成");
//			int lordLvid = techTab->getTitleIndex("人物等级");
//			int achievementCountid = techTab->getTitleIndex("战功");
//			int lvindex = techTab->getTitleIndex("lv");
//			unsigned char nextlv = nowlv+1;
//			crTableIO::StrVec record,recordn;
//			crTableIO::DataVec recordv;
//			techTab->queryRecords(0,crArgumentParser::appItoa(techid),recordv);
//			if(techTab->queryOneRecord(0,crArgumentParser::appItoa(techid),lvindex,crArgumentParser::appItoa((int)nowlv),recordn) >= 0)
//			{
//				if (atoi(recordn[lvindex].c_str()) == recordv.size() - 1)
//				{
//					if(attributeName)
//					{
//						attributeName->setString(recordn[attributeNameid]);
//					}
//					if(attributeInfo)
//					{
//						if (techid == 20460)
//						{
//							attributeInfo->setString("效果提升"+recordn[attributeInfoid]);
//						}
//						else if(techid >= 20461 && techid <= 20468)
//						{
//							attributeInfo->setString("效果提升"+recordn[attributeInfoid]+"%");
//						}
//						else
//						{
//							std::string str;
//							char buf[64];
//							float f = (float)atoi(recordn[attributeInfoid].c_str()) * 0.1f;
//							sprintf(buf,"效果提升%.1f%%\0",f);
//							str = buf;
//							attributeInfo->setString(str);
//						}
//					}
//					if(lordLv)
//					{
//						lordLv->setString(L"已满级");
//					}
//					if(achievementCount)
//					{
//						achievementCount->setString(L"已满级");
//					}
//				}
//				else if (techTab->queryOneRecord(0,crArgumentParser::appItoa(techid),lvindex,crArgumentParser::appItoa((int)nextlv),record) >= 0)
//				{
//					if(attributeName)
//					{
//						attributeName->setString(record[attributeNameid]);
//					}
//					if(attributeInfo)
//					{
//						if (techid == 20460)
//						{
//							attributeInfo->setString("下一级效果提升"+record[attributeInfoid]);
//						}
//						else if(techid >= 20461 && techid <= 20468)
//						{
//							attributeInfo->setString("下一级效果提升"+record[attributeInfoid]+"%");
//						}
//						else
//						{
//							std::string str;
//							char buf[64];
//							float f = (float)atoi(record[attributeInfoid].c_str()) * 0.1f;
//							sprintf(buf,"下一级效果提升%.1f%%\0",f);
//							str = buf;
//							attributeInfo->setString(str);
//						}
//					}
//					if(lordLv)
//					{
//						lordLv->setString(record[lordLvid]);
//					}
//					if(achievementCount)
//					{
//						achievementCount->setString(record[achievementCountid]);
//					}
//				}
//				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
//				const crBoundingBox &iconbox = m_this->getBoundBox();
//				const crBoundingBox &tips = tipsCanvas->getBoundBox();
//				float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
//				float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
//				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
//				/////////////////////////////////////////
//				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
//				float posx2,posy2;
//				/////////////////////////////////////////
//				if (pos2[0] >= 0)
//				{
//					posx2 = pos2[0] - posx;
//				}
//				else
//				{
//					posx2 = pos[0] + posx;
//				}
//				if (pos2[1] >= 0)
//				{
//					posy2 = pos[1] - posy;
//				}
//				else
//				{
//					posy2 = pos2[1] + posy;
//				}
//				crVector2 mouse(crVector2(posx2,posy2));
//				/////////////////////////////////////////
//				tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
//				tipsCanvas->setCanFocus(false);
//				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
//			}
//		}
//		data->excHandle(MAKEINT64(WCH_LockData,0));
//	}
//}

/////////////////////////////////////////
//
//crJXJTestFontSizeMethod
//
/////////////////////////////////////////
crJXJTestFontSizeMethod::crJXJTestFontSizeMethod():
	m_charactersize(12),
	m_sizex(16),
	m_sizey(16),
	m_linespacing(5),
	m_underlinespacing(3),
	m_uplinespacing(1)
{
}

crJXJTestFontSizeMethod::crJXJTestFontSizeMethod(const crJXJTestFontSizeMethod& handle):
	crMethod(handle),
	m_charactersize(handle.m_charactersize),
	m_sizex(handle.m_sizex),
	m_sizey(handle.m_sizey),
	m_linespacing(handle.m_linespacing),
	m_underlinespacing(handle.m_underlinespacing),
	m_scrollbar(handle.m_scrollbar),
	m_table(handle.m_table),
	m_uplinespacing(handle.m_uplinespacing)
{
	for (int i = 0; i<9;i++)
	{
		m_equipList[i] = handle.m_equipList[i];
	}
}

void crJXJTestFontSizeMethod::inputParam(int i, void *param)
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

void crJXJTestFontSizeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_charactersize = atof(str.c_str());
		break;
	case 1:
		m_sizex = atof(str.c_str());
		break;
	case 2:
		m_sizey = atof(str.c_str());
		break;
	case 3:
		m_linespacing = atof(str.c_str());
		break;
	case 4:
		m_underlinespacing = atof(str.c_str());
		break;
	case 5:
		m_uplinespacing = atof(str.c_str());
		break;
	case 6:
		m_scrollbar = str;
		break;
	case 7:
		m_table = str;
		break;
	case 8:
		m_equipList[0] = str;
		break;
	case 9:
		m_equipList[1] = str;
		break;
	case 10:
		m_equipList[2] = str;
		break;
	case 11:
		m_equipList[3] = str;
		break;
	case 12:
		m_equipList[4] = str;
		break;
	case 13:
		m_equipList[5] = str;
		break;
	case 14:
		m_equipList[6] = str;
		break;
	case 15:
		m_equipList[7] = str;
		break;
	case 16:
		m_equipList[8] = str;
		break;
	}
}

void crJXJTestFontSizeMethod::operator()(crHandle &handle)
{
	//crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this/* && playerData*/)
	{
		crListBoxWidgetNode *equipList[9] ={NULL};
		crTableWidgetNode *tablelist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_table));
		crScrollBarWidgetNode *Vscrollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scrollbar));
		for(int i = 0 ;i<9 ;i++)
		{
			equipList[i] = dynamic_cast<crListBoxWidgetNode *>(m_this->getWidget(m_equipList[i]));
			if (equipList[i])
			{
				equipList[i]->setCharacterSize(m_charactersize);
				equipList[i]->setFontSize(crVector2f(m_sizex,m_sizey));
				equipList[i]->setLineSpacing(m_linespacing);
				equipList[i]->setUnderLineSpacing(m_underlinespacing);
				equipList[i]->setUpLineSpacing(m_uplinespacing);//
			}
		}
		//if (tablelist)
		//{
		//	int count = tablelist->getRowCount();
		//	if (Vscrollbar)
		//	{
		//		Vscrollbar->setLineValue(m_linespacing+m_charactersize);
		//		Vscrollbar->setPageValue(Vscrollbar->getLineValue()*count);
		//		//Vscrollbar->setMinMoveValue(Vscrollbar->getLineValue());
		//	}
		//}
	}
}

/////////////////////////////////////////
//
//crJXJUINoOpenIconTipMethod
//
/////////////////////////////////////////
crJXJUINoOpenIconTipMethod::crJXJUINoOpenIconTipMethod():
	m_this(NULL)
{
}

crJXJUINoOpenIconTipMethod::crJXJUINoOpenIconTipMethod(const crJXJUINoOpenIconTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tiptext(handle.m_tiptext),
	m_this(NULL),
	m_str(0)
{
}
void crJXJUINoOpenIconTipMethod::inputParam(int i, void *param)
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
void crJXJUINoOpenIconTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tiptext = str;
		break;
	case 2:
		m_str = atoi(str.c_str());
		break;
	}
}

void crJXJUINoOpenIconTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tips.valid() && player)
	{
		void *param;
		int tianshu = 0;
		crRole *mainrole = player->getMainRole();
		if(!mainrole)return;
		crData *mainroledata = mainrole->getDataClass();
		if(!mainroledata)return;
		crData *data = player->getDataClass();
		if(!data)return;
		data ->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevel,viplv).c_str());
		int maxBattlelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenMaxLimitLevel,viplv).c_str());
		int IncreaseDate = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevelIncreaseDate,viplv).c_str());
		mainroledata->getParam(WCHDATA_Level,param);
		unsigned char mylv = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJOpenBattle, param);
		bool isbattleopened = *((bool *)param);
		int rushquickendneedgold = 0;
		int worldFubenOpenlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldFubenOpenLevel,viplv).c_str());

		ref_ptr<crHypertextWidgetNode>  tipsTexth;
		if (m_str == 100)
		{
			tipsTexth = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tiptext));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int needgoldpersecond = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushPerGold,viplv).c_str());
			crData *brainData = crBrain::getInstance()->getDataClass();
			brainData->getParam(WCHDATA_JXJRemainFubenRushCount,param);
			unsigned char remaincount = *(unsigned char *)param;
			rushquickendneedgold = needgoldpersecond * remaincount;
			//playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgoldtemp,NULL));
		}
		//data->getParam(WCHDATA_JXJOpenPass, param);
		//bool ispassopened = *((bool *)param);
		crStaticTextWidgetNode *tipsText = NULL;
		if (m_str != 100)
		{
			tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tiptext));
		}
		std::string str;
		switch (m_str)
		{
		case 0:
			str = "暂未开放"; //在线礼包
			break;
		case 1:
			str = "暂未开放"; //精彩活动
			break;
		case 2:
			str = "暂未开放"; //官职商店
			break;
		case 3:
			str = "暂未开放"; //官职列表
			break;
		case 4:
			str = "暂未开放"; //势力
			break;
		case 9:
			str = "C";
			break;
		case 10:
			if (crGlobalHandle::isClient())
			{
				crGlobalHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			}
			else
			{
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			}
			if (tianshu >= 0 && tianshu < IncreaseDate)
			{
				if (mylv >= openlv)
				{
					if (isbattleopened)
					{
						str = "【城战】开启中";
					}
					else
					{
						// 					std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime,viplv);
						// 					std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime,viplv);
						// 					str = opentime+"-"+endtime;
						std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_1,0);
						std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_1,0);
						std::string opentime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_2,0);
						std::string endtime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_2,0);
						std::string opentime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_3,0);
						std::string endtime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_3,0);
						time_t t = time(0);
						char tmp[20];
						strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
						crVector3i opentm,endtm,curtm;
						crVector3i opentm_2,endtm_2,opentm_3,endtm_3;
						crArgumentParser::appTimetoVec(opentime,opentm);
						crArgumentParser::appTimetoVec(endtime,endtm);
						crArgumentParser::appTimetoVec(opentime_2,opentm_2);
						crArgumentParser::appTimetoVec(endtime_2,endtm_2);
						crArgumentParser::appTimetoVec(opentime_3,opentm_3);
						crArgumentParser::appTimetoVec(endtime_3,endtm_3);
						crArgumentParser::appTimetoVec(tmp,curtm);

						if(curtm > endtm && curtm <= endtm_2)
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}
						else if(curtm > endtm_2 && curtm <= endtm_3)
						{
							str = "国战时间:" + opentime_3+"-"+endtime_3;
						}
						else if(opentm<endtm)
							str = "国战时间:" + opentime+"-"+endtime;
						else
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}
					}
				}
				else
				{
					str = "lv." + crArgumentParser::appItoa(openlv) + " 开放";
				}
			}
			else if (tianshu >= IncreaseDate && tianshu < maxBattlelv-IncreaseDate)
			{
				int battlelv_1 = openlv + tianshu - IncreaseDate + 1;
				if (mylv >= battlelv_1)
				{
					if (isbattleopened)
					{
						str = "【城战】开启中";
					}
					else
					{
						// 					std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime,viplv);
						// 					std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime,viplv);
						// 					str = opentime+"-"+endtime;
						std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_1,0);
						std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_1,0);
						std::string opentime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_2,0);
						std::string endtime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_2,0);
						std::string opentime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_3,0);
						std::string endtime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_3,0);
						time_t t = time(0);
						char tmp[20];
						strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
						crVector3i opentm,endtm,curtm;
						crVector3i opentm_2,endtm_2,opentm_3,endtm_3;
						crArgumentParser::appTimetoVec(opentime,opentm);
						crArgumentParser::appTimetoVec(endtime,endtm);
						crArgumentParser::appTimetoVec(opentime_2,opentm_2);
						crArgumentParser::appTimetoVec(endtime_2,endtm_2);
						crArgumentParser::appTimetoVec(opentime_3,opentm_3);
						crArgumentParser::appTimetoVec(endtime_3,endtm_3);
						crArgumentParser::appTimetoVec(tmp,curtm);

						if(curtm > endtm && curtm <= endtm_2)
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}
						else if(curtm > endtm_2 && curtm <= endtm_3)
						{
							str = "国战时间:" + opentime_3+"-"+endtime_3;
						}
						else if(opentm<endtm)
							str = "国战时间:" + opentime+"-"+endtime;
						else
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}

					}
				}
				else
				{
					str = "lv." + crArgumentParser::appItoa(battlelv_1) + " 开放";
				}
			}
			else
			{
				if (mylv >= maxBattlelv)
				{
					if (isbattleopened)
					{
						str = "【城战】开启中";
					}
					else
					{
						// 					std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime,viplv);
						// 					std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime,viplv);
						// 					str = opentime+"-"+endtime;
						std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_1,0);
						std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_1,0);
						std::string opentime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_2,0);
						std::string endtime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_2,0);
						std::string opentime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_3,0);
						std::string endtime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_3,0);
						time_t t = time(0);
						char tmp[20];
						strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
						crVector3i opentm,endtm,curtm;
						crVector3i opentm_2,endtm_2,opentm_3,endtm_3;
						crArgumentParser::appTimetoVec(opentime,opentm);
						crArgumentParser::appTimetoVec(endtime,endtm);
						crArgumentParser::appTimetoVec(opentime_2,opentm_2);
						crArgumentParser::appTimetoVec(endtime_2,endtm_2);
						crArgumentParser::appTimetoVec(opentime_3,opentm_3);
						crArgumentParser::appTimetoVec(endtime_3,endtm_3);
						crArgumentParser::appTimetoVec(tmp,curtm);

						if(curtm > endtm && curtm <= endtm_2)
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}
						else if(curtm > endtm_2 && curtm <= endtm_3)
						{
							str = "国战时间:" + opentime_3+"-"+endtime_3;
						}
						else if(opentm<endtm)
							str = "国战时间:" + opentime+"-"+endtime;
						else
						{
							str = "国战时间:" + opentime_2+"-"+endtime_2;
						}
					}
				}
				else
				{
					str = "lv." + crArgumentParser::appItoa(maxBattlelv) + " 开放";
				}
			}
			break;
		case 11:
			str = "C";
			break;
		case 12:
			str = "B";
			break;
		case 13:
			str = "O";
			break;
		case 14:
			str = "P";
			break;
		case 15:
			str = "L";
			break;
		case 16:
			{
				int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWeiRenBtnOpenLv).c_str());
				if(mylv >= openlv)
					str = "W";
				else
					str = crArgumentParser::appItoa(openlv)+"级开启";
			}
			break;
		case 17:
			str = "G";
			break;
		case 18:
			str = "副本";
			break;
		case 19:
			{
				int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupBtnOpenLv).c_str());
				if(mylv >= openlv)
					str = "K";
				else
					str = crArgumentParser::appItoa(openlv)+"级开启";
			}
			break;
		case 20:
			str = "T";
			break;
		case 21:
			str = "U";
			break;
		case 22:
			if(mylv >= worldFubenOpenlv)
				str = "世界战场";
			else
				str = crArgumentParser::appItoa(worldFubenOpenlv) + "级开启";//"25级开放";
			break;
		case 23:
			{
				int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJForeignCardOpenLevel, 0).c_str());
				if (mylv >= openlv)
					str = "异国将卡兑换";
				else
					str = crArgumentParser::appItoa(openlv) + "级开启";//"75级开放";
			}
			break;
		case 100:
			{
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);

				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content", 1);
				v_i.clear();
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				cfg_script.Add("Text", "         花费");
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content", 2);
				v_i.clear();
				v_i.push_back(140.0f);
				v_i.push_back(40.0f);
				v_i.push_back(40.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				char tmpText[32];
				sprintf(tmpText, "%d礼金/元宝\0",rushquickendneedgold);
				cfg_script.Add("Text", tmpText);
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content", 3);
				v_i.clear();
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				cfg_script.Add("Text", "立即完成扫荡");
				cfg_script.Pop();

				cfg_script.Pop();
				if (tipsTexth.valid()) tipsTexth->setHypertext(cfg_script);
			}
			//str = "需要" + crArgumentParser::appItoa(rushquickendneedgold) + "元宝";
			break;

		}
		if (tipsText)
		{
			tipsText->setString(str);
			tipsText->setColor(FCOLOR_BLACK);
		}
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = 0.0;
		if(m_str == 10)
			posx = btnbox.m_min[0] - 0.2;
		else if(m_str == 22)
			posx = btnbox.m_min[0] - 0.15;
		else
			posx = btnbox.m_min[0];
		float posy = 0.0;
		if (m_str < 11 || m_str >= 100)
		{
			if(m_str == 10)
				posy = btnbox.m_min[1] - tipsbox.yLength()/2 + 0.15;
			else
				posy = btnbox.m_min[1] - tipsbox.yLength()/2;

		}
		else
		{
			if(m_str == 10)
				posy = btnbox.m_max[1] + tipsbox.yLength()/2 + 0.15;
			else if(m_str == 22)
				posy = btnbox.m_max[1] + tipsbox.yLength()/2 - 0.1;
			else
				posy = btnbox.m_max[1] + tipsbox.yLength()/2;
		}
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJUIUpgradePromptMethod
//
/////////////////////////////////////////
crJXJUIUpgradePromptMethod::crJXJUIUpgradePromptMethod():
	m_this(NULL)
{
}

crJXJUIUpgradePromptMethod::crJXJUIUpgradePromptMethod(const crJXJUIUpgradePromptMethod& handle):
	crMethod(handle),
	m_LevelType(handle.m_LevelType),
	m_LevelType_One(handle.m_LevelType_One),
	m_LevelType_Two(handle.m_LevelType_Two),
	m_this(NULL)
{
}
void crJXJUIUpgradePromptMethod::inputParam(int i, void *param)
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
void crJXJUIUpgradePromptMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_LevelType = str;
		break;
	case 1:
		m_LevelType_One = str;
		break;
	case 2:
		m_LevelType_Two = str;
		break;
	}
}

void crJXJUIUpgradePromptMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crMultiSwitch *LevelType = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_LevelType));
		crMultiSwitch *LevelType_One = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_LevelType_One));
		crMultiSwitch *LevelType_Two = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_LevelType_Two));
		if(!LevelType || !LevelType_One || !LevelType_Two) return;
		LevelType->setVisiable(false);
		LevelType_One->setVisiable(false);
		LevelType_Two->setVisiable(false);

		crRole *mainrole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		crData *data = mainrole->getDataClass();
		if (data)
		{
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_Level,param);
			unsigned char *nowlv = (unsigned char *)param;

			unsigned char nextlv = *nowlv;
			if ((int)nextlv < 10)
			{
				LevelType->setVisiable(true);
				LevelType->setActiveSwitchSet((int)nextlv - 2);
			}
			else if((int)nextlv > 9)
			{
				LevelType_One->setVisiable(true);
				LevelType_Two->setVisiable(true);
				LevelType_One->setActiveSwitchSet((int)nextlv / 10 - 1);
				LevelType_Two->setActiveSwitchSet((int)nextlv % 10);
			}
			m_this->setPosition(crVector3f(0.0f,1.0f,0.0f));
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckRecruitCountMethod
//
/////////////////////////////////////////
crJXJCheckRecruitCountMethod::crJXJCheckRecruitCountMethod():
	m_armid(0),
	m_count(0),
	m_trueExecCount(1)
{
}
crJXJCheckRecruitCountMethod::crJXJCheckRecruitCountMethod(const crJXJCheckRecruitCountMethod& handle):
	crMethod(handle),
	m_armid(handle.m_armid),
	m_count(handle.m_count),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckRecruitCountMethod::inputParam(int i, void *param)
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
void crJXJCheckRecruitCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	case 2:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckRecruitCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		if(m_trueExecCount != 0)
		{
			void *param;
			crData *data = playerData->getDataClass();
			if(!data)return;
			data->getParam(WCHDATA_JXJTroopsMap,param);
			TroopsMap *troopsMap = (TroopsMap *)param;
			if (troopsMap)
			{
				for( TroopsMap::iterator itr = troopsMap->begin();
					itr != troopsMap->end();
					++itr)
				{
					if(itr->first == m_armid && itr->second >= m_count)
					{
						check = true;
						if(m_trueExecCount > 0)	
							m_trueExecCount--;
						break;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckTempFormationInfoMethod
//
/////////////////////////////////////////
crJXJCheckTempFormationInfoMethod::crJXJCheckTempFormationInfoMethod():
	m_flag(false),
	m_armid(0),
	m_count(0),
	m_newCount(0),
	m_trueExecCount(1)
{
}
crJXJCheckTempFormationInfoMethod::crJXJCheckTempFormationInfoMethod(const crJXJCheckTempFormationInfoMethod& handle):
	crMethod(handle),
	m_flag(handle.m_flag),
	m_armid(handle.m_armid),
	m_count(handle.m_count),
	m_formationCanvas(handle.m_formationCanvas),
	m_formationInput(handle.m_formationInput),
	m_newCount(handle.m_newCount),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckTempFormationInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckTempFormationInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	case 2:
		m_formationCanvas = str;
		break;
	case 3:
		m_formationInput = str;
		break;
	case 4:
		m_newCount = (unsigned char)(atoi(str.c_str()));;
		break;
	case 5:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckTempFormationInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(!formationCanvas)
		return;
	crData *formationgcanvasdata = formationCanvas->getDataClass();
	if(!formationgcanvasdata)
		return;
	crEditWidgetNode *formationgInput = dynamic_cast<crEditWidgetNode *>(formationCanvas->getWidget(m_formationInput));
	if(!formationgInput)
		return;
	formationgcanvasdata->getParam(WCHDATA_JXJTempFormationVec,param);
	FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
	if(m_trueExecCount != 0)
	{
		if (formationInfoVec->size() > 0)
		{
			if (1 == m_newCount)
			{
				if ((*formationInfoVec)[0]!=NULL)
				{
					if((*formationInfoVec)[0]->getAbstractID() == m_armid && !m_flag)
					{
						formationgInput->setString(std::string("500"));
						formationgInput->doEvent(WCH_UI_ValueChange);
						m_flag = true;
					}
					if ((*formationInfoVec)[0]->getAbstractID() == m_armid && (*formationInfoVec)[0]->getCount() >= m_count)
					{
						m_flag = false;
						check = true;
						if(m_trueExecCount > 0)
							m_trueExecCount--;
					}
				}
			}
			if (2 == m_newCount)
			{
				for (int i = 0; i < (*formationInfoVec).size() - 1; i++)
				{
					if ((*formationInfoVec)[i]!=NULL)
					{
						if((*formationInfoVec)[i]->getAbstractID() == m_armid && !m_flag)
						{
							if((*formationInfoVec)[i]->getAbstractID() == m_armid && (*formationInfoVec)[i]->getCount()>0)
							{
								check = true;
								if(m_trueExecCount > 0)
									m_trueExecCount--;
								break;
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFormationInfoMethod
//
/////////////////////////////////////////
crJXJCheckFormationInfoMethod::crJXJCheckFormationInfoMethod():
	m_flag(false),
	m_armid(0),
	m_count(0),
	m_newCount(),
	m_trueExecCount()
{
}
crJXJCheckFormationInfoMethod::crJXJCheckFormationInfoMethod(const crJXJCheckFormationInfoMethod& handle):
	crMethod(handle),
	m_flag(handle.m_flag),
	m_armid(handle.m_armid),
	m_count(handle.m_count),
	m_newCount(handle.m_newCount),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckFormationInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFormationInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	case 2:
		m_newCount = (unsigned char)(atoi(str.c_str()));;
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFormationInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		FormationInfoVec::iterator itr_info = formationInfoVec->begin();
		if(m_trueExecCount != 0)
		{
			if (1 == m_newCount)
			{
				for (; itr_info != formationInfoVec->end(); itr_info++)
				{
					if (itr_info->get()->getAbstractID() == m_armid && itr_info->get()->getCount() >= m_count)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
			else if (2 == m_newCount)
			{
				for (; itr_info != formationInfoVec->end(); itr_info++)
				{
					if(itr_info->get()->getAbstractID() == m_armid && itr_info->get()->getCount() > 0)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
						break;
					}
				}
			}
			else if (3 == m_newCount)
			{
				for (; itr_info != formationInfoVec->end(); itr_info++)
				{
					if(itr_info->get()->getAbstractID() == m_armid && itr_info->get()->getCount() > 0)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
						break;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJGetTaskProgressMethod
//
/////////////////////////////////////////
crJXJGetTaskProgressMethod::crJXJGetTaskProgressMethod()
{
}
crJXJGetTaskProgressMethod::crJXJGetTaskProgressMethod(const crJXJGetTaskProgressMethod& handle):
	crMethod(handle)
{
}
void crJXJGetTaskProgressMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJGetTaskProgressMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetTaskProgressMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		int _case = m_this->getTaskProgress();
		handle.outputParam(0,&_case);
	}
}

/////////////////////////////////////////
//
//crJXJCheckBuildingLvTwoMethod
//
/////////////////////////////////////////
crJXJCheckBuildingLvTwoMethod::crJXJCheckBuildingLvTwoMethod():
	m_buildingid(0),
	m_lv(0)
{
}
crJXJCheckBuildingLvTwoMethod::crJXJCheckBuildingLvTwoMethod(const crJXJCheckBuildingLvTwoMethod& handle):
	crMethod(handle),
	m_buildingid(handle.m_buildingid),
	m_lv(handle.m_lv)
{
}
void crJXJCheckBuildingLvTwoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckBuildingLvTwoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	case 1:
		m_lv = (unsigned char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckBuildingLvTwoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->getParam(m_buildingid,param);
		unsigned char lv = *(unsigned char *)param;
		if(lv>=m_lv)
		{
			check = true;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckZhengShuiCountMethod
//
/////////////////////////////////////////
crJXJCheckZhengShuiCountMethod::crJXJCheckZhengShuiCountMethod():
	m_trueExecCount()
{
}
crJXJCheckZhengShuiCountMethod::crJXJCheckZhengShuiCountMethod(const crJXJCheckZhengShuiCountMethod& handle):
	crMethod(handle),
	m_zhengShuiCanvas(handle.m_zhengShuiCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckZhengShuiCountMethod::inputParam(int i, void *param)
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
void crJXJCheckZhengShuiCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_zhengShuiCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckZhengShuiCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> zhengShuiCanvas = crFilterRenderManager::getInstance()->findCanvas(m_zhengShuiCanvas);
		if(!zhengShuiCanvas)
			return;
		void *param;
		crData *data = playerData->getDataClass();
		if(!data)return;
		data ->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		crVector2i MaxLevyCount;
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,viplv).c_str(),MaxLevyCount);
		if(m_trueExecCount != 0)
		{
			data->getParam(WCHDATA_JXJLevyCount,param);
			{
				unsigned char levyCount = *(unsigned char *)param;
				if(levyCount < MaxLevyCount[LevyType_Free] + MaxLevyCount[LevyType_Gold])
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
					crFilterRenderManager::getInstance()->closeCanvas(zhengShuiCanvas.get());
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFormationJKInfoMethod
//
/////////////////////////////////////////
crJXJCheckFormationJKInfoMethod::crJXJCheckFormationJKInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckFormationJKInfoMethod::crJXJCheckFormationJKInfoMethod(const crJXJCheckFormationJKInfoMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_index(handle.m_index)
{
}
void crJXJCheckFormationJKInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFormationJKInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJCheckFormationJKInfoMethod::operator()(crHandle &handle)
{
 	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		if(m_trueExecCount != 0)
		{
			ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
			if(formationCanvas.valid() && playerData->getDataClass())
			{
				crData *Data = playerData->getDataClass();
				void *param;
				Data->getParam(WCHDATA_JXJFormationInfoVec,param);
				FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
				if(formationInfoVec)
				{
					FormationInfoVec::iterator itr_info = formationInfoVec->begin();
					for (; itr_info != formationInfoVec->end(); itr_info++)
					{
						if (m_index == 1)
						{
							if((itr_info->get()->getEquips())[0] > 0)
							{
								check = true;
								if(m_trueExecCount>0)
									m_trueExecCount--;
								break;
							}
						}
						if (m_index == 2)
						{
							if((itr_info->get()->getEquips())[1] > 0)
							{
								check = true;
								if(m_trueExecCount>0)
									m_trueExecCount--;
								break;
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckWeiRenJKInfoMethod
//
/////////////////////////////////////////
crJXJCheckWeiRenJKInfoMethod::crJXJCheckWeiRenJKInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckWeiRenJKInfoMethod::crJXJCheckWeiRenJKInfoMethod(const crJXJCheckWeiRenJKInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckWeiRenJKInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckWeiRenJKInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckWeiRenJKInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		void *param;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
		JXJAppointCardVec *appointcardvec = (JXJAppointCardVec *)param;
		{
			if(m_trueExecCount != 0)
			{
				for(int i = 0;i < appointcardvec->size();i++)
				{
					if ((*appointcardvec)[i] != NULL)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
						break;
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);

}

/////////////////////////////////////////
//
//crJXJUIGetRenWuRewardMethod
//
/////////////////////////////////////////
crJXJUIGetRenWuRewardMethod::crJXJUIGetRenWuRewardMethod():
	m_taskid(0)
{
}
crJXJUIGetRenWuRewardMethod::crJXJUIGetRenWuRewardMethod(const crJXJUIGetRenWuRewardMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_expCount(handle.m_expCount),
	m_copCashCount(handle.m_copCashCount),
	m_oneCount(handle.m_oneCount),
	m_twoCount(handle.m_twoCount),
	m_taskid(handle.m_taskid)
{
	for (int i = 0;i < 2; ++i)
	{
		m_rewardstr[i] = handle.m_rewardstr[i];
	}
}
void crJXJUIGetRenWuRewardMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJUIGetRenWuRewardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_expCount = str;
		break;
	case 2:
		m_copCashCount = str;
		break;
	case 3:
		m_oneCount = str;
		break;
	case 4:
		m_twoCount = str;
		break;
	case 5:
		m_rewardstr[0] = str;
		break;
	case 6:
		m_rewardstr[1] = str;
		break;
	case 7:
		m_taskid = atoi(str.c_str());
		break;
	}
}
void crJXJUIGetRenWuRewardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		int cardcolorindex = 0;
		std::string itemiconname[2],cardiconname[2],itemswname[2],cardcolorname[2],rewardstr[2];
		itemiconname[0] = "Icon3";
		itemiconname[1] = "Icon4";
		cardiconname[0] = "Card1";
		cardiconname[1] = "Card2";
		itemswname[0] = "sw13";
		itemswname[1] = "sw14";
		cardcolorname[0] = "sw11";
		cardcolorname[1] = "sw12";
		rewardstr[0] = "Input3";
		rewardstr[1] = "Input4";
		//int rewarditemindex[4] = {0};
		//crVector3i itemvec[4];
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(!canvas)return;
		ref_ptr<crStaticTextWidgetNode> expCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_expCount));
		ref_ptr<crStaticTextWidgetNode> copCashCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_copCashCount));
		ref_ptr<crStaticTextWidgetNode> nameinput[2] = {NULL};// = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_twoCount));
		ref_ptr<crImageBoxWidgetNode> itemicon[2] = {NULL};
		ref_ptr<crImageBoxWidgetNode> cardicon[2] = {NULL};
		ref_ptr<crMultiSwitch> rewardsw[2] = {NULL};
		ref_ptr<crMultiSwitch> cardcolor[2] = {NULL};
		for (int i = 0; i < 2; ++i)
		{
			itemicon[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(itemiconname[i]));
			cardicon[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(cardiconname[i]));
			rewardsw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(itemswname[i]));
			cardcolor[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(cardcolorname[i]));
			nameinput[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(rewardstr[i]));
			if (itemicon[i].valid())itemicon[i]->setVisiable(false);
			if (cardicon[i].valid())cardicon[i]->setVisiable(false);
			if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(0);
			if (cardcolor[i].valid())cardcolor[i]->setActiveSwitchSet(0);
			if (nameinput[i].valid())nameinput[i]->clearString();
		}
		//if (oneCount.valid())oneCount->setVisiable(false);
		//if (twoCount.valid())twoCount->setVisiable(false);
		ref_ptr<crTableIO>taskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTab);
		ref_ptr<crTableIO>tasktitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
		ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int expCountid = taskTab->getTitleIndex("Exp");
		int copCashCountid = taskTab->getTitleIndex("Gold");
		int taskidindex = tasktitleTab->getTitleIndex("taskid");
		int awardIdx[2] = {0};
		awardIdx[0] = tasktitleTab->getTitleIndex("奖励1");
		awardIdx[1] = tasktitleTab->getTitleIndex("奖励2");
		//int nameindex = itemTab->getTitleIndex("name");
		//int itemtypeindex = itemTab->getTitleIndex("类型");
		//int iconnameindex = itemTab->getTitleIndex("icon");
		//int itemcolorindex = itemTab->getTitleIndex("品质");
		int taskid = m_this->getTaskID();
		crData *data = canvas->getDataClass();
		data->inputParam(WCHDATA_JXJGetRenWuRewardID,&taskid);
		crTableIO::StrVec record,titlerecord;//,itemrecord;
		if(taskTab->queryOneRecord(0,crArgumentParser::appItoa(taskid),record) >= 0 && expCountid >= 0 && copCashCountid >= 0)
		{
			if (expCount.valid())
			{
				expCount->setString(record[expCountid]);
			}
			if (copCashCount.valid())
			{
				copCashCount->setString(record[copCashCountid]);
			}
		}
		if (tasktitleTab->queryOneRecord(taskidindex,crArgumentParser::appItoa(taskid),titlerecord) >= 0)
		{
			crVector4i awardVec;
			for (int i = 0; i < 2; i++)
			{
				if (titlerecord[awardIdx[i]] == "") continue;
				crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
				if(setRewardInfo(awardVec,itemicon[i].get(),nameinput[i].get(),cardcolor[i].get()) == 1)
				{//将卡
					if (cardicon[i].valid())
					{
						cardicon[i]->setImageName(itemicon[i]->getImageName());
						cardicon[i]->setVisiable(true);
					}
					if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(2);
				}
				else
				{
					if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(1);
				}
				//if (atoi(titlerecord[i==0?rewarditemindex[0]:rewarditemindex[2]].c_str()) == T_Item)
				//{
				//	crArgumentParser::appAtoVec(titlerecord[i==0?rewarditemindex[1]:rewarditemindex[3]],itemvec[i]);
				//	if (itemvec[i][0] > 0 && itemvec[i][2] > 0)
				//	{
				//		if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemvec[i][0]),itemrecord) >= 0)
				//		{
				//			if (nameinput[i].valid())nameinput[i]->setString(itemrecord[nameindex]);
				//			if(atoi(itemrecord[itemtypeindex].c_str()) == IT_Jiangka)
				//			{
				//				if (cardicon[i].valid())
				//				{
				//					cardicon[i]->setVisiable(true);
				//					cardicon[i]->setImageName(itemrecord[iconnameindex]);
				//				}
				//				if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(2);
				//				if (cardcolor[i].valid())
				//				{
				//					cardcolorindex = atoi(itemrecord[itemcolorindex].c_str());
				//					//if (cardcolorindex > CardColor_Orange)
				//					//{
				//					//	cardcolorindex = CardColor_Orange;
				//					//}
				//					cardcolor[i]->setActiveSwitchSet(cardcolorindex);
				//				}
				//			}
				//			else
				//			{
				//				if (itemicon[i].valid())
				//				{
				//					itemicon[i]->setVisiable(true);
				//					itemicon[i]->setImageName(itemrecord[iconnameindex]);
				//				}

				//				if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(1);
				//			}
				//		}
				//	}
				//}
				//else if(atoi(titlerecord[i==0?rewarditemindex[0]:rewarditemindex[2]].c_str()) == T_Zhanquan)
				//{
				//	if (nameinput[i].valid())nameinput[i]->setString(titlerecord[i==0?rewarditemindex[1]:rewarditemindex[3]]);
				//	if (itemicon[i].valid())
				//	{
				//		itemicon[i]->setVisiable(true);
				//		itemicon[i]->setImageName("T_Icon_Zhengzhanjuan.img");
				//	}

				//	if (rewardsw[i].valid())rewardsw[i]->setActiveSwitchSet(1);
				//}
			}
		}
		if(canvas.valid())crFilterRenderManager::getInstance()->doModal(canvas.get());

	}
}

/////////////////////////////////////////
//
//crJXJUIPlayerFriendListSelectMethod
//
/////////////////////////////////////////
crJXJUIPlayerFriendListSelectMethod::crJXJUIPlayerFriendListSelectMethod():
	m_iconid(0)
{
}
crJXJUIPlayerFriendListSelectMethod::crJXJUIPlayerFriendListSelectMethod(const crJXJUIPlayerFriendListSelectMethod& handle):
	crMethod(handle),
	m_friendlist(handle.m_friendlist),
	m_iconid(handle.m_iconid)
{
}
void crJXJUIPlayerFriendListSelectMethod::inputParam(int i, void *param)
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
void crJXJUIPlayerFriendListSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_friendlist = str;
		break;
	case 1:
		m_iconid = atoi(str.c_str());
		break;
	}
}
void crJXJUIPlayerFriendListSelectMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && m_this)
	{
		ref_ptr<crCanvasNode> Canvas = m_this->getParentCanvas();
		crTableWidgetNode *friendList = dynamic_cast<crTableWidgetNode *>(Canvas->getWidget(m_friendlist));
		if(friendList)
		{
			friendList->selectRow(m_iconid);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIXinShouYingDaoMoveMethod
//
/////////////////////////////////////////
crJXJUIXinShouYingDaoMoveMethod::crJXJUIXinShouYingDaoMoveMethod()
{
}
crJXJUIXinShouYingDaoMoveMethod::crJXJUIXinShouYingDaoMoveMethod(const crJXJUIXinShouYingDaoMoveMethod& handle):
	crMethod(handle),
	m_moveArrows(handle.m_moveArrows)
{
}
void crJXJUIXinShouYingDaoMoveMethod::inputParam(int i, void *param)
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
void crJXJUIXinShouYingDaoMoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_moveArrows = str;
		break;
	}
}
void crJXJUIXinShouYingDaoMoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && m_this)
	{
		void *param;
		ref_ptr<crCanvasNode> moveArrows = dynamic_cast<crCanvasNode *>(m_this->getWidget(m_moveArrows));
		if(!moveArrows)return;
		ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
		if(!renWuCanvas.valid())
			return;
		crData *data = renWuCanvas->getDataClass();
		if (data)
		{
			data->getParam(WCHDATA_JXJTargetCanvasName,param);
			std::string targetCanvasName = *(std::string *)param;
			ref_ptr<crCanvasNode> moveCanvas = crFilterRenderManager::getInstance()->findCanvas(targetCanvasName);
			if(!moveCanvas)return;
			data->getParam(WCHDATA_JXJTargetWidgetName,param);
			std::string targetWidgetName = *(std::string *)param;
			crWidgetNode *moveWidget = moveCanvas->getWidget(targetWidgetName);
			if(!moveWidget)return;
			crMatrix mat = moveCanvas->getMatrix();
			crVector3 pos = moveWidget->getBound().center();
			pos = pos * mat;
			moveArrows->setPosition(pos);
		}
	}
}

/////////////////////////////////////////
//
//crJXJAdvanceTask2Method
//
/////////////////////////////////////////
crJXJAdvanceTask2Method::crJXJAdvanceTask2Method():
	m_taskid(0),
	m_stepType(0)
{
}
crJXJAdvanceTask2Method::crJXJAdvanceTask2Method(const crJXJAdvanceTask2Method& handle):
	crMethod(handle),
	m_taskid(handle.m_taskid),
	m_stepType(handle.m_stepType)
{
}
void crJXJAdvanceTask2Method::inputParam(int i, void *param)
{
}
void crJXJAdvanceTask2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = atoi(str.c_str());
		break;
	case 1:
		m_stepType = atoi(str.c_str());
		break;
	}
}
void crJXJAdvanceTask2Method::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_taskid,m_stepType));
	}
}

///////////////////////////////////////////
////
////crJXJBuildingUpgradePromptEffectLogic
////
///////////////////////////////////////////
//crJXJBuildingUpgradePromptEffectLogic::crJXJBuildingUpgradePromptEffectLogic():
//	m_decalNode(NULL),
//	m_zoffset(1.0f),
//	m_buildingid(0)
//{
//}
//
//crJXJBuildingUpgradePromptEffectLogic::crJXJBuildingUpgradePromptEffectLogic(const crJXJBuildingUpgradePromptEffectLogic& handle):
//	crLogic(handle),
//	m_decalNode(NULL),
//	m_zoffset(handle.m_zoffset),
//	m_filename(handle.m_filename),
//	m_buildingid(handle.m_buildingid)
//{
//	if(handle.m_playSceneFx.valid())
//		m_playSceneFx = handle.m_playSceneFx->clone();
//}
//
//void crJXJBuildingUpgradePromptEffectLogic::inputParam(int i, void *param)
//{
//}
//
//void crJXJBuildingUpgradePromptEffectLogic::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_zoffset = atof(str.c_str());
//		break;
//	case 1:
//		m_filename = str;
//		break;
//	case 2:
//		m_buildingid = atoi(str.c_str());
//		break;
//	}
//}
//
//void crJXJBuildingUpgradePromptEffectLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_playSceneFx = param?(CRCore::crHandle *)param:NULL;
//		break;
//	}
//}
//
//void crJXJBuildingUpgradePromptEffectLogic::outputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case WCHDATA_LoadedNode:
//		m_decalNode = (crNode *)param;
//		break;
//	}
//}
//
//void crJXJBuildingUpgradePromptEffectLogic::operator()(crHandle &handle)
//{
//	if(!m_playSceneFx.valid()) return;
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	crData *data = playerData->getDataClass();
//	crData *brainData = crBrain::getInstance()->getDataClass();
//	crInstanceItem *item  = crMyPlayerData::getInstance()->getScene()->findSceneItem(m_buildingid);
//	{
//		m_playSceneFx->inputParam(5,&m_filename);
//		(*m_playSceneFx)(*this);
//		brainData->inputParam(WCHDATA_JXJBuildingUpgradePrompt,m_decalNode.get());
//	}
//	if(m_decalNode.valid())
//	{
//		do 
//		{
//			if(item)
//			{
//				crVector3 pos;
//				crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode.get());
//				if(skillRangeMatrix)
//				{
//					crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(item->getRelNode());
//					if(!bot || !bot->getVisiable())
//					{
//						skillRangeMatrix->setVisiable(false);
//					}
//					else
//					{
//						skillRangeMatrix->setVisiable(true);
//						pos = bot->getTrans();
//						skillRangeMatrix->setMatrix(crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
//						break;
//					}
//				}
//			}
//			m_decalNode->setVisiable(false);
//		}
//		while(0);
//	}
//}
//
///////////////////////////////////////////
////
////crJXJBuildingEffectCloseMethod
////
///////////////////////////////////////////
//crJXJBuildingEffectCloseMethod::crJXJBuildingEffectCloseMethod()
//{
//
//}
//crJXJBuildingEffectCloseMethod::crJXJBuildingEffectCloseMethod(const crJXJBuildingEffectCloseMethod& handle)
//{
//
//}
//void crJXJBuildingEffectCloseMethod::inputParam(int i, void *param)
//{
//
//}
//void crJXJBuildingEffectCloseMethod::addParam(int i, const std::string& str)
//{
//
//}
//void crJXJBuildingEffectCloseMethod::operator()(crHandle &handle)
//{
//	void *param;
//	crData *brainData = crBrain::getInstance()->getDataClass();
//	brainData->getParam(WCHDATA_JXJBuildingUpgradePrompt,param);
//	crNode *decalNode = (crNode *)param;
//	if(decalNode)
//	{
//		decalNode->setVisiable(false);
//	}
//}

/////////////////////////////////////////
//
//crJXJBuildingTaskEffectMethod
//
/////////////////////////////////////////
crJXJBuildingTaskEffectMethod::crJXJBuildingTaskEffectMethod():
	m_buildingid(0),
	m_visiable(true)
{

}
crJXJBuildingTaskEffectMethod::crJXJBuildingTaskEffectMethod(const crJXJBuildingTaskEffectMethod& handle):
crMethod(handle),
m_buildingid(handle.m_buildingid),
m_visiable(handle.m_visiable)
{
}
void crJXJBuildingTaskEffectMethod::inputParam(int i, void *param)
{

}
void crJXJBuildingTaskEffectMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	case 1:
		m_visiable = atoi(str.c_str());
		break;
	}
}
void crJXJBuildingTaskEffectMethod::operator()(crHandle &handle)
{
	CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
	if(scene)
	{
		crInstanceItem *item  = scene->findSceneItem(m_buildingid);
		if(item)
		{
			item->doEvent(WCH_JXJBuildingTaskEffect,MAKEINT64(m_visiable?1:0,NULL));
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckPutOnEquipInfoMethod
//
/////////////////////////////////////////
crJXJCheckPutOnEquipInfoMethod::crJXJCheckPutOnEquipInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckPutOnEquipInfoMethod::crJXJCheckPutOnEquipInfoMethod(const crJXJCheckPutOnEquipInfoMethod& handle):
	crMethod(handle),
	m_LordCanvas(handle.m_LordCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckPutOnEquipInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckPutOnEquipInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_LordCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckPutOnEquipInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCanvasNode> Lordcanvas = crFilterRenderManager::getInstance()->findCanvas(m_LordCanvas);
	if(!Lordcanvas)return;
	crRole *mainrole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
	if(!mainrole)return;
	crData *mainroledata = mainrole->getDataClass();
	if(!mainroledata)return;
	mainroledata->getParam(WCHDATA_JXJLordEquipVec,param);
	JXJLordEquipVec *equipvec = (JXJLordEquipVec *)param;
	if(equipvec)
	{
		if(m_trueExecCount != 0)
		{
			if ((*equipvec)[1].first>0)///????????只确认第二件装备吗
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJArrowsUITypeSelectMethod
//
/////////////////////////////////////////
crJXJArrowsUITypeSelectMethod::crJXJArrowsUITypeSelectMethod():
	m_Arrows(0)
{
}
crJXJArrowsUITypeSelectMethod::crJXJArrowsUITypeSelectMethod(const crJXJArrowsUITypeSelectMethod& handle):
	crMethod(handle),
	m_RenWuCanvas(handle.m_RenWuCanvas),
	m_Arrows(handle.m_Arrows)
{
}
void crJXJArrowsUITypeSelectMethod::inputParam(int i, void *param)
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
void crJXJArrowsUITypeSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_RenWuCanvas = str;
		break;
	case 1:
		m_Arrows = (int)(atoi(str.c_str()));
		break;
	}
}
void crJXJArrowsUITypeSelectMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> RenWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_RenWuCanvas);
	if(!RenWuCanvas)return;
	crData *data = RenWuCanvas->getDataClass();
	if(!data)return;
	data->inputParam(WCHDATA_JXJArrowsUISelect,&m_Arrows);
}

/////////////////////////////////////////
//
//crJXJCheckFormationCeKaInfoMethod
//
/////////////////////////////////////////
crJXJCheckFormationCeKaInfoMethod::crJXJCheckFormationCeKaInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckFormationCeKaInfoMethod::crJXJCheckFormationCeKaInfoMethod(const crJXJCheckFormationCeKaInfoMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckFormationCeKaInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFormationCeKaInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFormationCeKaInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
		if(!formationCanvas)
			return;
		crData *Data = playerData->getDataClass();
		if(!Data)return;
		void *param;
		Data->excHandle(MAKEINT64(WCH_LockData,1));
		Data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if(formationInfoVec->size() > 0)
		{
			for (int i = 0; i < (*formationInfoVec).size(); i++)
			{
				if ((*formationInfoVec)[i]!=NULL)
				{
					if((*formationInfoVec)[i]->getEquips()[1] > 0)
					{
						if(m_trueExecCount != 0)
						{
							check = true;
							if(m_trueExecCount>0)
								m_trueExecCount--;
							break;
						}
					}
				}
			}
		}
		Data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckRecruitTimesMethod
//
/////////////////////////////////////////
crJXJCheckRecruitTimesMethod::crJXJCheckRecruitTimesMethod():
	m_trueExecCount()
{
}
crJXJCheckRecruitTimesMethod::crJXJCheckRecruitTimesMethod(const crJXJCheckRecruitTimesMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckRecruitTimesMethod::inputParam(int i, void *param)
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
void crJXJCheckRecruitTimesMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckRecruitTimesMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *Data = playerData->getDataClass();
		if(!Data)return;
		void *param;
		Data->getParam(WCHDATA_JXJRecruitTimes,param);
		crVector4i *recruitTimes = (crVector4i *)param;
		if((*recruitTimes)[1] >= 0)
		{
			if(m_trueExecCount != 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckMasterFormationInfoMethod
//
/////////////////////////////////////////
crJXJCheckMasterFormationInfoMethod::crJXJCheckMasterFormationInfoMethod():
	m_armid(0),
	m_count(0),
	m_newArmid(0),
	m_newCount(),
	m_trueExecCount()
{
}
crJXJCheckMasterFormationInfoMethod::crJXJCheckMasterFormationInfoMethod(const crJXJCheckMasterFormationInfoMethod& handle):
	crMethod(handle),
	m_armid(handle.m_armid),
	m_count(handle.m_count),
	m_newArmid(handle.m_newArmid),
	m_newCount(handle.m_newCount),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckMasterFormationInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckMasterFormationInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	case 2:
		m_newArmid = atoi(str.c_str());
		break;
	case 3:
		m_newCount = (unsigned char)(atoi(str.c_str()));;
		break;
	case 4:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckMasterFormationInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if(m_trueExecCount != 0)
		{
			if (formationInfoVec->size() > 1)
			{
				if ((*formationInfoVec)[0]!=NULL && (*formationInfoVec)[1]!=NULL)
				{
					if ((*formationInfoVec)[0]->getAbstractID() == m_armid && (*formationInfoVec)[0]->getCount() >= m_count && (*formationInfoVec)[1]->getAbstractID() == m_newArmid && (*formationInfoVec)[1]->getCount() >= m_newCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
					if ((*formationInfoVec)[1]->getAbstractID() == m_armid && (*formationInfoVec)[1]->getCount() >= m_count && (*formationInfoVec)[0]->getAbstractID() == m_newArmid && (*formationInfoVec)[0]->getCount() >= m_newCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentRecruitCountInfoMethod
//
/////////////////////////////////////////
crJXJCheckCurrentRecruitCountInfoMethod::crJXJCheckCurrentRecruitCountInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckCurrentRecruitCountInfoMethod::crJXJCheckCurrentRecruitCountInfoMethod(const crJXJCheckCurrentRecruitCountInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckCurrentRecruitCountInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentRecruitCountInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentRecruitCountInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerData->getDataClass(); 
	data->excHandle(MAKEINT64(WCH_LockData,1));
	if(m_trueExecCount != 0)
	{
		data->getParam(WCHDATA_JXJTroopsMap,param);
		TroopsMap *troopsMap = (TroopsMap *)param;
		int Count = 0;
		for( TroopsMap::iterator itr = troopsMap->begin();
			itr != troopsMap->end();
			++itr)
		{
			Count += itr->second;
		}
		crData *brainData = crBrain::getInstance()->getDataClass();
		brainData->inputParam(WCHDATA_JXJBrainRecruitCount,&Count);
		check = true;
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckSupplementRecruitCountMethod
//
/////////////////////////////////////////
crJXJCheckSupplementRecruitCountMethod::crJXJCheckSupplementRecruitCountMethod():
	m_trueExecCount()
{
}
crJXJCheckSupplementRecruitCountMethod::crJXJCheckSupplementRecruitCountMethod(const crJXJCheckSupplementRecruitCountMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckSupplementRecruitCountMethod::inputParam(int i, void *param)
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
void crJXJCheckSupplementRecruitCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckSupplementRecruitCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerData->getDataClass(); 
	crData *brainData = crBrain::getInstance()->getDataClass();
	if(brainData)
	{
		if(m_trueExecCount != 0)
		{
			void *param;
			data->getParam(WCHDATA_JXJTroopsMap,param);
			TroopsMap *troopsMap = (TroopsMap *)param;
			int Count = 0;
			for( TroopsMap::iterator itr = troopsMap->begin();
				itr != troopsMap->end();
				++itr)
			{
				Count += itr->second;
			}
			brainData->getParam(WCHDATA_JXJBrainRecruitCount,param);
			int brainCount = *(int *)param;
			if(Count - brainCount >= 200)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentCardCountMethod
//
/////////////////////////////////////////
crJXJCheckCurrentCardCountMethod::crJXJCheckCurrentCardCountMethod()
{
}
crJXJCheckCurrentCardCountMethod::crJXJCheckCurrentCardCountMethod(const crJXJCheckCurrentCardCountMethod& handle):
	crMethod(handle),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJCheckCurrentCardCountMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentCardCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_renWuCanvas =str;
		break;
	}
}
void crJXJCheckCurrentCardCountMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
	if (playerData)
	{
		crData *data = playerData->getDataClass(); 
		data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
		int Count = 0;
		int totalCount = 0;
		int i = 0;
		for( JXJItemBagVec::iterator itr = cardbagvec->begin();
			itr != cardbagvec->end();
			++itr,i++)
		{
			if(i >= 0 && (*cardbagvec)[i] != NULL)
			{
				Count = (*itr)->getItemCount();
				totalCount += Count;
			}
		}
		if (renWuCanvas.valid())
		{
			crData *renwudata = renWuCanvas->getDataClass();
			renwudata->inputParam(WCHDATA_JXJBrainTargetCardCount,&totalCount);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckExtractJiangKaInfoMethod
//
/////////////////////////////////////////
crJXJCheckExtractJiangKaInfoMethod::crJXJCheckExtractJiangKaInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckExtractJiangKaInfoMethod::crJXJCheckExtractJiangKaInfoMethod(const crJXJCheckExtractJiangKaInfoMethod& handle):
	crMethod(handle),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckExtractJiangKaInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckExtractJiangKaInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_renWuCanvas =str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckExtractJiangKaInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
	crData *data = playerData->getDataClass(); 
	data->getParam(WCHDATA_JXJJiangkaBagVec,param);
	JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
	int Count = 0;
	int i = 0;
	int totalCount = 0;
	for( JXJItemBagVec::iterator itr = cardbagvec->begin();
		itr != cardbagvec->end();
		++itr,i++)
	{
		if(i >= 0 && (*cardbagvec)[i] != NULL)
		{
			Count = (*itr)->getItemCount();
			totalCount += Count;
		}
	}
	if (renWuCanvas.valid())
	{
		crData *renwudata = renWuCanvas->getDataClass();
		renwudata->getParam(WCHDATA_JXJBrainTargetCardCount,param);
		int brainCount = *(int *)param;
		if(m_trueExecCount != 0)
		{
			if (totalCount - brainCount >= 1)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckDirectlySubmitTaskMethod
//
/////////////////////////////////////////
crJXJCheckDirectlySubmitTaskMethod::crJXJCheckDirectlySubmitTaskMethod():
	m_taskid(0)
{
}
crJXJCheckDirectlySubmitTaskMethod::crJXJCheckDirectlySubmitTaskMethod(const crJXJCheckDirectlySubmitTaskMethod& handle):
	crMethod(handle),
	m_taskid(handle.m_taskid)
{
}
void crJXJCheckDirectlySubmitTaskMethod::inputParam(int i, void *param)
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
void crJXJCheckDirectlySubmitTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckDirectlySubmitTaskMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		playerData->doEvent(WCH_JXJReferTask,MAKEINT64(m_taskid,NULL));
	}
}

/////////////////////////////////////////
//
//crJXJCheckOpenCanvasInfoMethod
//
/////////////////////////////////////////
crJXJCheckOpenCanvasInfoMethod::crJXJCheckOpenCanvasInfoMethod():
	m_trueExecCount(1),
	m_buildingID(0)
{
}
crJXJCheckOpenCanvasInfoMethod::crJXJCheckOpenCanvasInfoMethod(const crJXJCheckOpenCanvasInfoMethod& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_buildingID(handle.m_buildingID)
{
}
void crJXJCheckOpenCanvasInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckOpenCanvasInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 2:
		m_buildingID = atoi(str.c_str());
		break;
	}
}
void crJXJCheckOpenCanvasInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
		if(playerData && targetCanvas.valid())
		{
			//crFilterRenderManager::getInstance()->showCanvas(targetCanvas.get(),true);
			if(targetCanvas->getVisiable())
			{
				if(m_buildingID>0)
				{
					crData *data = targetCanvas->getDataClass();
					if(data)
					{
						void *param;
						data->getParam(WCHDATA_JXJCheckBuildingID,param);
						int buildingid = *(int *)param;
						if(buildingid == m_buildingID)
						{
							check = true;
							if(m_trueExecCount>0)
								m_trueExecCount--;
						}
					}
				}
				else
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckMasterFormationMethod
//
/////////////////////////////////////////
crJXJCheckMasterFormationMethod::crJXJCheckMasterFormationMethod():
	m_trueExecCount(1)
{
}
crJXJCheckMasterFormationMethod::crJXJCheckMasterFormationMethod(const crJXJCheckMasterFormationMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckMasterFormationMethod::inputParam(int i, void *param)
{
}
void crJXJCheckMasterFormationMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckMasterFormationMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(!formationCanvas)
		return;
	crData *formationgdata = formationCanvas->getDataClass();
	if(formationgdata)
	{
		formationgdata->getParam(WCHDATA_JXJTempFormationVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if(m_trueExecCount != 0)
		{
			if (formationInfoVec->size() > 0)
			{
				if ((*formationInfoVec)[0] != NULL && (*formationInfoVec)[1] != NULL)
				{
					if ((*formationInfoVec)[0]->getAbstractID() == 20001 && (*formationInfoVec)[1]->getAbstractID() == 20007)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
					else if((*formationInfoVec)[0]->getAbstractID() == 20007 && (*formationInfoVec)[1]->getAbstractID() == 20001)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckPutOnJiangKaTaskMethod
//
/////////////////////////////////////////
crJXJCheckPutOnJiangKaTaskMethod::crJXJCheckPutOnJiangKaTaskMethod()
{
}
crJXJCheckPutOnJiangKaTaskMethod::crJXJCheckPutOnJiangKaTaskMethod(const crJXJCheckPutOnJiangKaTaskMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas)
{
}
void crJXJCheckPutOnJiangKaTaskMethod::inputParam(int i, void *param)
{
}
void crJXJCheckPutOnJiangKaTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	}
}
void crJXJCheckPutOnJiangKaTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(!formationCanvas)
		return;
	crData *formationgdata = formationCanvas->getDataClass();
	if(formationgdata)
	{
		formationgdata->excHandle(MAKEINT64(WCH_LockData,1));
		formationgdata->getParam(WCHDATA_JXJTempFormationVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if (formationInfoVec->size() > 0)
		{
			if ((*formationInfoVec)[0] != NULL)
			{
				check = true;
			}
		}
		formationgdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckTaskFuBenSuccessInfoMethod
//
/////////////////////////////////////////
crJXJCheckTaskFuBenSuccessInfoMethod::crJXJCheckTaskFuBenSuccessInfoMethod():
	m_fubenid(0),
	m_trueExecCount(1)
{
}
crJXJCheckTaskFuBenSuccessInfoMethod::crJXJCheckTaskFuBenSuccessInfoMethod(const crJXJCheckTaskFuBenSuccessInfoMethod& handle):
	crMethod(handle),
	m_fubenid(handle.m_fubenid),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckTaskFuBenSuccessInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckTaskFuBenSuccessInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenid = (unsigned short)(atoi(str.c_str()));;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckTaskFuBenSuccessInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	void *param;
	crData *data = playerData->getDataClass();
	data->getParam(WCHDATA_JXJFubenProgressMap,param);
	FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
	FubenProgressMap::iterator itrcur = fubenProgress->find((unsigned short)m_fubenid);
	if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 0)
	{
		if (m_trueExecCount != 0)
		{
			check = true;
			if(m_trueExecCount>0)
				m_trueExecCount--;
			crJXJCheckOpenFubenTipCanvasMethod::m_trueExecCount = 1;
			crJXJCheckOpenFubenTipCanvasTwoMethod::m_trueExecCount = 1;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCloseCanvasInfoMethod
//
/////////////////////////////////////////
crJXJCheckCloseCanvasInfoMethod::crJXJCheckCloseCanvasInfoMethod():
	m_trueExecCount(1)
{
}
crJXJCheckCloseCanvasInfoMethod::crJXJCheckCloseCanvasInfoMethod(const crJXJCheckCloseCanvasInfoMethod& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckCloseCanvasInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckCloseCanvasInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCloseCanvasInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
		if(playerData && targetCanvas.valid())
		{
			if(!targetCanvas->getVisiable())
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCloseCanvasInfo2Method
//
/////////////////////////////////////////
crJXJCheckCloseCanvasInfo2Method::crJXJCheckCloseCanvasInfo2Method()
{
}
crJXJCheckCloseCanvasInfo2Method::crJXJCheckCloseCanvasInfo2Method(const crJXJCheckCloseCanvasInfo2Method& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas)
{
}
void crJXJCheckCloseCanvasInfo2Method::inputParam(int i, void *param)
{
}
void crJXJCheckCloseCanvasInfo2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	}
}
void crJXJCheckCloseCanvasInfo2Method::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
	if(playerData && targetCanvas.valid())
	{
		if(!targetCanvas->getVisiable())
		{
			check = true;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckOpenCanvasInfo2Method
//
/////////////////////////////////////////
crJXJCheckOpenCanvasInfo2Method::crJXJCheckOpenCanvasInfo2Method():
	m_buildingID(0)
{
}
crJXJCheckOpenCanvasInfo2Method::crJXJCheckOpenCanvasInfo2Method(const crJXJCheckOpenCanvasInfo2Method& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas),
	m_buildingID(handle.m_buildingID)
{
}
void crJXJCheckOpenCanvasInfo2Method::inputParam(int i, void *param)
{
}
void crJXJCheckOpenCanvasInfo2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	case 1:
		m_buildingID = atoi(str.c_str());
		break;
	}
}
void crJXJCheckOpenCanvasInfo2Method::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
	if(playerData && targetCanvas.valid())
	{
		if(targetCanvas->getVisiable())
		{
			if(m_buildingID>0)
			{
				crData *data = targetCanvas->getDataClass();
				if(data)
				{
					void *param;
					data->getParam(WCHDATA_JXJCheckBuildingID,param);
					int buildingid = *(int *)param;
					if(buildingid == m_buildingID)
					{
						check = true;
					}
				}
			}
			else
			{
				check = true;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckMasterFormation2Method
//
/////////////////////////////////////////
crJXJCheckMasterFormation2Method::crJXJCheckMasterFormation2Method():
	m_trueExecCount(1),
	m_index(0)
{
}
crJXJCheckMasterFormation2Method::crJXJCheckMasterFormation2Method(const crJXJCheckMasterFormation2Method& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_index(handle.m_index)
{
}
void crJXJCheckMasterFormation2Method::inputParam(int i, void *param)
{
}
void crJXJCheckMasterFormation2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 2:
		m_index = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckMasterFormation2Method::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(!formationCanvas)
		return;
	crData *formationgdata = formationCanvas->getDataClass();
	if(formationgdata)
	{
		formationgdata->getParam(WCHDATA_JXJTempFormationVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if(m_trueExecCount != 0)
		{
			if(m_index == 1)
			{
				if (formationInfoVec->size() > 0)
				{
					if ((*formationInfoVec)[0] != NULL)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
			if(m_index == 2)
			{
				if (formationInfoVec->size() >= 0)
				{
					if ((*formationInfoVec)[0] != NULL && (*formationInfoVec)[0]->getAbstractID() < 0)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckAlreadyInBattleInfoMethod
//
/////////////////////////////////////////
crJXJCheckAlreadyInBattleInfoMethod::crJXJCheckAlreadyInBattleInfoMethod():
	m_trueExecCount(1),
	m_selectCount(0)
{
}
crJXJCheckAlreadyInBattleInfoMethod::crJXJCheckAlreadyInBattleInfoMethod(const crJXJCheckAlreadyInBattleInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_selectCount(handle.m_selectCount)
{
}
void crJXJCheckAlreadyInBattleInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckAlreadyInBattleInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_selectCount = (int)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckAlreadyInBattleInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	void *param;
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		//unsigned short battleID = *(unsigned short*)param;
		if (m_selectCount == 1)
		{
			//if(battleID > 0)
			if(type == FT_Chengchi)
			{
				check = true;
			}
		}
		if (m_selectCount == 2)
		{
			if(m_trueExecCount != 0)
			{
				//if(battleID <= 0)
				if(type != FT_Chengchi)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentResourcesCardCountMethod
//
/////////////////////////////////////////
crJXJCheckCurrentResourcesCardCountMethod::crJXJCheckCurrentResourcesCardCountMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_itemid(0)
{
}
crJXJCheckCurrentResourcesCardCountMethod::crJXJCheckCurrentResourcesCardCountMethod(const crJXJCheckCurrentResourcesCardCountMethod& handle):
	crMethod(handle),
	m_KnapsackCanvas(handle.m_KnapsackCanvas),
	m_myBagList(handle.m_myBagList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_itemid(handle.m_itemid)
{
}
void crJXJCheckCurrentResourcesCardCountMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentResourcesCardCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_KnapsackCanvas = str;
		break;
	case 1:
		m_myBagList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 4:
		m_taskid = (atoi(str.c_str()));
		break;
	case 5:
		m_itemid = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentResourcesCardCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> KnapsackCanvas = crFilterRenderManager::getInstance()->findCanvas(m_KnapsackCanvas);
	if(!KnapsackCanvas.valid())
		return;
	crListControlWidgetNode *KnapsackList = dynamic_cast<crListControlWidgetNode *>(KnapsackCanvas->getWidget(m_myBagList));
	crData *data = playerData->getDataClass(); 
	CRCore::ScopedLock<crData> lock(*data);
	data->getParam(WCHDATA_JXJItemBagVec,param);
	JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
	data->getParam(WCHDATA_JXJOpenBagNum,param);
	unsigned char openbagnum = *(unsigned char *)param;
	int Count = 0;
	int totalCount = 0;
	int i = 0;
	int itemid;
	for( JXJItemBagVec::iterator itr = itembagvec->begin();
		itr != itembagvec->end();
		++itr,i++)
	{
		if(i >= 0 && i < openbagnum && (*itembagvec)[i] != NULL)
		{
			itemid = (*itembagvec)[i]->getItemID();
			if(itemid == m_itemid)
			{
				KnapsackList->select(i);
				crListControlNode *selectNode = KnapsackList->getSelectNode();
				if(!selectNode)return;
				int selectid = selectNode->getData();
				if ((*itembagvec)[selectid]!=NULL)
				{
					Count = (*itembagvec)[selectid]->getItemCount();
					totalCount += Count;
					crData *brainData = crBrain::getInstance()->getDataClass();
					brainData->inputParam(WCHDATA_JXJBrainResourcesCardCount,&totalCount);
					ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
					ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
					if (totalCount >= 1)
					{
						if(m_trueExecCount != 0)
						{
							if(rectCanvas.valid() && renwucanvas.valid())
							{
								crData *renwudata = renwucanvas->getDataClass();
								if(!renwudata)return;
								renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
								//renwudata->inputParam(WCHDATA_JXJTargetCanvasName,&m_KnapsackCanvas);
								//renwudata->inputParam(WCHDATA_JXJTargetWidgetName,&KnapsackList);
								//renwudata->inputParam(WCHDATA_JXJRectCanvasName,&m_rectCanvasName);
							
								crVector4f rect = selectNode->getRect();
								CRCore::crMatrixTransform *m_mvpwNode = KnapsackList->getMvpwNode();
								const crMatrix &mat = m_mvpwNode->getMatrix() * KnapsackList->getParentCanvas()->getMatrix();
								//crMatrix mat = KnapsackCanvas->getMatrix();
								crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
								crVector3 ipos = pos * mat;
								rectCanvas->setPosition(ipos);
								if(m_trueExecCount>0)
									m_trueExecCount--;
								check = true;
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckResourcesCardCountInfoMethod
//
/////////////////////////////////////////
crJXJCheckResourcesCardCountInfoMethod::crJXJCheckResourcesCardCountInfoMethod():
	m_trueExecCount(),
	m_itemid(0),
	m_count(0)
{
}
crJXJCheckResourcesCardCountInfoMethod::crJXJCheckResourcesCardCountInfoMethod(const crJXJCheckResourcesCardCountInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_itemid(handle.m_itemid),
	m_count(handle.m_count)
{
}
void crJXJCheckResourcesCardCountInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckResourcesCardCountInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_itemid = (atoi(str.c_str()));
		break;
	case 2:
		m_count = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckResourcesCardCountInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerData->getDataClass(); 
	crData *brainData = crBrain::getInstance()->getDataClass();
	if(brainData)
	{
		if(m_trueExecCount != 0)
		{
			void *param;
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			int Count = 0;
			int totalCount = 0;
			for( JXJItemBagVec::iterator itr = itembagvec->begin();
				itr != itembagvec->end();
				++itr)
			{
				if((*itr)!=NULL && (*itr)->getItemID()!=0)
				{
					if ((*itr)->getItemID() == m_itemid)
					{
						Count = (*itr)->getItemCount();
					}
					totalCount += Count;
				}
			}
			brainData->getParam(WCHDATA_JXJBrainResourcesCardCount,param);
			int brainCount = *(int *)param;
			if(brainCount - totalCount >= m_count)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
			else if(totalCount == 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckUpdateTechLevelTaskMethod
//
/////////////////////////////////////////
crJXJCheckUpdateTechLevelTaskMethod::crJXJCheckUpdateTechLevelTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckUpdateTechLevelTaskMethod::crJXJCheckUpdateTechLevelTaskMethod(const crJXJCheckUpdateTechLevelTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_techid(handle.m_techid)
{
}
void crJXJCheckUpdateTechLevelTaskMethod::inputParam(int i, void *param)
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
void crJXJCheckUpdateTechLevelTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_techid = atoi(str.c_str());
		break;
	}
}
void crJXJCheckUpdateTechLevelTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		if(!data)return;
		if(m_trueExecCount != 0)
		{
			data->getParam(WCHDATA_JXJMakeEquipTechLv,param);
			unsigned char techlv = *(unsigned char *)param;
			if (techlv > 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckUpdateGovernmentPostTaskMethod
//
/////////////////////////////////////////
crJXJCheckUpdateGovernmentPostTaskMethod::crJXJCheckUpdateGovernmentPostTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckUpdateGovernmentPostTaskMethod::crJXJCheckUpdateGovernmentPostTaskMethod(const crJXJCheckUpdateGovernmentPostTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckUpdateGovernmentPostTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckUpdateGovernmentPostTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckUpdateGovernmentPostTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crRole *mainRole = playerData->getMainRole();
		crData *data = mainRole->getDataClass();
		unsigned short guanzhiid;
		if(m_trueExecCount != 0)
		{
			int taskid = m_this->getTaskID();
			ref_ptr<crTableIO>taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
			if (taskTitleTab.valid())
			{
				int guanzhiidindex = taskTitleTab->getTitleIndex("fubenid");
				int taskidindex = taskTitleTab->getTitleIndex("taskid");
				crTableIO::StrVec taskrecord;
				if(taskTitleTab->queryOneRecord(taskidindex,crArgumentParser::appItoa(taskid),taskrecord) >= 0)
				{
					guanzhiid = (unsigned short)atoi(taskrecord[guanzhiidindex].c_str());
					data->getParam(WCHDATA_JXJGovPost,param);
					unsigned char post = *(unsigned char*)param;
					if(post >= guanzhiid)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckUpdateCardConditionInfoMethod
//
/////////////////////////////////////////
crJXJCheckUpdateCardConditionInfoMethod::crJXJCheckUpdateCardConditionInfoMethod():
	m_trueExecCount(),
	m_cardid(0)
{
}
crJXJCheckUpdateCardConditionInfoMethod::crJXJCheckUpdateCardConditionInfoMethod(const crJXJCheckUpdateCardConditionInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_cardid(handle.m_cardid)
{
}
void crJXJCheckUpdateCardConditionInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckUpdateCardConditionInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_cardid = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckUpdateCardConditionInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->excHandle(MAKEINT64(WCH_LockData,1));
		braindata->getParam(WCHDATA_JXJUpgradeJiangkaIndex,param);
		short upgradejiangkaindex = *(short *)param;
		//braindata->getParam(WCHDATA_JXJUpgradeNewJiangkaId,param);
		//int *newjiangkaindex = (int *)param;
		int itemid = 0;
		int needcount = 0;
		int myCount = 0;
		if (upgradejiangkaindex >= 0 && jiangkabagvec && upgradejiangkaindex<jiangkabagvec->size())
		{
			if ((*jiangkabagvec)[upgradejiangkaindex]!=NULL)
			{
				itemid = (*jiangkabagvec)[upgradejiangkaindex]->getItemID();
			}
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int needindex = itemtab->getTitleIndex("升级材料");
			if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
			{
				needcount = atoi(record[needindex].c_str());
			}
			for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
				itr!= jiangkabagvec->end();
				++itr)
			{
				if ((*itr)!=NULL)
				{
					if((*itr)->getItemID() == itemid && (*itr)->getItemCount()>0)
					{
						myCount++;
					}
				}
			}
			if(m_trueExecCount != 0)
			{
				if (myCount >= needcount)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
		braindata->excHandle(MAKEINT64(WCH_LockData,0));
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFormationCardTaskMethod
//
/////////////////////////////////////////
crJXJCheckFormationCardTaskMethod::crJXJCheckFormationCardTaskMethod():
	m_trueExecCount(),
	m_cardid(0)
{
}
crJXJCheckFormationCardTaskMethod::crJXJCheckFormationCardTaskMethod(const crJXJCheckFormationCardTaskMethod& handle):
	crMethod(handle),
	m_cardid(handle.m_cardid),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckFormationCardTaskMethod::inputParam(int i, void *param)
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
void crJXJCheckFormationCardTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_cardid = (int)(atoi(str.c_str()));
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFormationCardTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		void *param;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		if(!formationInfoVec)return;
		if(formationInfoVec->size() > 0)
		{
			for (int i = 0; i < (*formationInfoVec).size(); i++)
			{
				if ((*formationInfoVec)[i]!=NULL)
				{
					if((*formationInfoVec)[i]->getEquips()[0] == m_cardid)
					{
						if(m_trueExecCount != 0)
						{
							check = true;
							if(m_trueExecCount>0)
								m_trueExecCount--;
							break;
						}
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentTargetCardCountMethod
//
/////////////////////////////////////////
crJXJCheckCurrentTargetCardCountMethod::crJXJCheckCurrentTargetCardCountMethod():
	m_cardid(0)
{
}
crJXJCheckCurrentTargetCardCountMethod::crJXJCheckCurrentTargetCardCountMethod(const crJXJCheckCurrentTargetCardCountMethod& handle):
	crMethod(handle),
	m_cardid(handle.m_cardid),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJCheckCurrentTargetCardCountMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentTargetCardCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_cardid = (int)(atoi(str.c_str()));
		break;
	case 1:
		m_renWuCanvas = str;
		break;
	}
}
void crJXJCheckCurrentTargetCardCountMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
	crData *data = playerData->getDataClass(); 
	data->excHandle(MAKEINT64(WCH_LockData,1));
	data->getParam(WCHDATA_JXJJiangkaBagVec,param);
	JXJItemBagVec *Jiangkabagvec = (JXJItemBagVec *)param;
	int Count = 0;
	int i = 0;
	for( JXJItemBagVec::iterator itr = Jiangkabagvec->begin();
		itr != Jiangkabagvec->end();
		++itr,i++)
	{
		if(i >= 0 && (*Jiangkabagvec)[i] != NULL)
		{
			if(m_cardid == (*Jiangkabagvec)[i]->getItemID())
			{
				Count++;
			}
		}
	}
	if (renWuCanvas.valid())
	{
		crData *renwudata = renWuCanvas->getDataClass();
		renwudata->inputParam(WCHDATA_JXJBrainTargetCardCount,&Count);
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));
}

/////////////////////////////////////////
//
//crJXJCheckTargetCardCountMethod
//
/////////////////////////////////////////
crJXJCheckTargetCardCountMethod::crJXJCheckTargetCardCountMethod():
	m_trueExecCount(),
	m_cardid(0)
{
}
crJXJCheckTargetCardCountMethod::crJXJCheckTargetCardCountMethod(const crJXJCheckTargetCardCountMethod& handle):
	crMethod(handle),
	m_cardid(handle.m_cardid),
	m_trueExecCount(handle.m_trueExecCount),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJCheckTargetCardCountMethod::inputParam(int i, void *param)
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
void crJXJCheckTargetCardCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_cardid = (int)(atoi(str.c_str()));
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 2:
		m_renWuCanvas = str;
		break;
	}
}
void crJXJCheckTargetCardCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
	crData *data = playerData->getDataClass(); 
	data->excHandle(MAKEINT64(WCH_LockData,1));
	data->getParam(WCHDATA_JXJJiangkaBagVec,param);
	JXJItemBagVec *Jiangkabagvec = (JXJItemBagVec *)param;
	int Count = 0;
	int i = 0;
	for( JXJItemBagVec::iterator itr = Jiangkabagvec->begin();
		itr != Jiangkabagvec->end();
		++itr,i++)
	{
		if(i >= 0 && (*Jiangkabagvec)[i] != NULL)
		{
			if(m_cardid == (*Jiangkabagvec)[i]->getItemID())
			{
				Count++;
			}
		}
	}
	if (renWuCanvas.valid())
	{
		crData *renwudata = renWuCanvas->getDataClass();
		renwudata->getParam(WCHDATA_JXJBrainTargetCardCount,param);
		int brainCount = *(int *)param;
		if(m_trueExecCount != 0)
		{
			if (Count - brainCount >= 1)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurSelRecruitArmAIDMethod
//
/////////////////////////////////////////
crJXJCheckCurSelRecruitArmAIDMethod::crJXJCheckCurSelRecruitArmAIDMethod():
	m_recruitArmID(0),
	m_trueExecCount()
	
{
}
crJXJCheckCurSelRecruitArmAIDMethod::crJXJCheckCurSelRecruitArmAIDMethod(const crJXJCheckCurSelRecruitArmAIDMethod& handle):
	crMethod(handle),
	m_recruitArmID(handle.m_recruitArmID),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckCurSelRecruitArmAIDMethod::inputParam(int i, void *param)
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
void crJXJCheckCurSelRecruitArmAIDMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_recruitArmID = (atoi((str.c_str())));
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurSelRecruitArmAIDMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crData *data = crBrain::getInstance()->getDataClass();
		if(m_trueExecCount != 0)
		{
			data->getParam(WCHDATA_JXJCurSelRecruitArmAID,param);
			int abstractid = *(int *)param;
			if(abstractid == m_recruitArmID)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckForgeEquipConditionInfoMethod
//
/////////////////////////////////////////
crJXJCheckForgeEquipConditionInfoMethod::crJXJCheckForgeEquipConditionInfoMethod():
	m_trueExecCount(),
	m_Equipid(0)
{
}
crJXJCheckForgeEquipConditionInfoMethod::crJXJCheckForgeEquipConditionInfoMethod(const crJXJCheckForgeEquipConditionInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_Equipid(handle.m_Equipid),
	m_gongfang(handle.m_gongfang)
{
}
void crJXJCheckForgeEquipConditionInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckForgeEquipConditionInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_Equipid = (atoi(str.c_str()));
		break;
	case 2:
		m_gongfang = str;
		break;
	}
}
void crJXJCheckForgeEquipConditionInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		ref_ptr<crTableIO>lairdEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
		int moneyid = lairdEquipTab->getTitleIndex("铜钱");
		int paperid = lairdEquipTab->getTitleIndex("图纸");
		int mate1id = lairdEquipTab->getTitleIndex("材料1");
		int mate2id = lairdEquipTab->getTitleIndex("材料2");
		int mate3id = lairdEquipTab->getTitleIndex("材料3");
		void *param;
		int equipid = 0;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfang);
		if (canvas.valid() && canvas->getDataClass())
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJCurSelForgeLordEquipID,param);
			equipid = *(int *)param;
		}
		if (equipid == m_Equipid)
		{
			if (m_trueExecCount != 0)
			{
				crTableIO::StrVec record;
				if(lairdEquipTab->queryOneRecord(paperid,crArgumentParser::appItoa(equipid),record) >= 0)
				{
					crVector2i mate1,mate2,mate3;
					crArgumentParser::appAtoVec(record[mate1id],mate1);
					crArgumentParser::appAtoVec(record[mate2id],mate2);
					crArgumentParser::appAtoVec(record[mate3id],mate3);
					data->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					int count1 = 0,count2 = 0,count3 = 0;
					for (JXJItemBagVec::iterator itr = itembagvec->begin();
						itr!= itembagvec->end();
						++itr)
					{
						if((*itr)!=NULL)
						{
							if((*itr)->getItemID() == mate1[0])
							{
								count1+=(*itr)->getItemCount();
							}
							if((*itr)->getItemID() == mate2[0])
							{
								count2+=(*itr)->getItemCount();
							}
							if((*itr)->getItemID() == mate3[0])
							{
								count3+=(*itr)->getItemCount();
							}
						}
					}
					data->getParam(WCHDATA_JXJCoppercash,param);
					int mymoney = *(int *)param;
					int needmoney = atoi(record[moneyid].c_str());
					if (mymoney >= needmoney && count1 >= mate1[1] && count2 >= mate2[1] && count3 >= mate3[1])
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentItemKindCountMethod
//
/////////////////////////////////////////
crJXJCheckCurrentItemKindCountMethod::crJXJCheckCurrentItemKindCountMethod():
	m_itemid(0)
{
}
crJXJCheckCurrentItemKindCountMethod::crJXJCheckCurrentItemKindCountMethod(const crJXJCheckCurrentItemKindCountMethod& handle):
	crMethod(handle),
	m_itemid(handle.m_itemid)
{
}
void crJXJCheckCurrentItemKindCountMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentItemKindCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_itemid = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentItemKindCountMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *data = playerData->getDataClass(); 
		data->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		data->getParam(WCHDATA_JXJOpenBagNum,param);
		unsigned char openbagnum = *(unsigned char *)param;
		int Count = 0;
		int totalCount = 0;
		int i = 0;
		int itemid;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr,i++)
		{
			if(i >= 0 && i < openbagnum && (*itembagvec)[i] != NULL)
			{
				itemid = (*itr)->getItemID();
				//itemid = (*itembagvec)[i]->getItemID;
				if(itemid == m_itemid)
				{
					Count = (*itr)->getItemCount();
					//Count = (*itembagvec)[i]->getItemCount;
					totalCount += Count;
				}
			}
		}
		crData *brainData = crBrain::getInstance()->getDataClass();
		brainData->inputParam(WCHDATA_JXJBrainResourcesCardCount,&totalCount);
	}
}

/////////////////////////////////////////
//
//crJXJCheckCurrentSelectRadioMethod
//
/////////////////////////////////////////
crJXJCheckCurrentSelectRadioMethod::crJXJCheckCurrentSelectRadioMethod():
	m_select(0),
	m_trueExecCount(0)
{
}
crJXJCheckCurrentSelectRadioMethod::crJXJCheckCurrentSelectRadioMethod(const crJXJCheckCurrentSelectRadioMethod& handle):
	crMethod(handle),
	m_GongFangCanvas(handle.m_GongFangCanvas),
	m_GongFangRaido(handle.m_GongFangRaido),
	m_select(handle.m_select),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckCurrentSelectRadioMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentSelectRadioMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_GongFangCanvas = str;
		break;
	case 1:
		m_GongFangRaido = str;
		break;
	case 2:
		m_select = (atoi(str.c_str()));
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentSelectRadioMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> gongFangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_GongFangCanvas);
		if (gongFangCanvas.valid())
		{
			crRadioGroupWidgetNode *gongFangRaido = dynamic_cast<crRadioGroupWidgetNode *>(gongFangCanvas->getWidget(m_GongFangRaido));
			if (gongFangRaido)
			{
				if (gongFangRaido->getSelect() == m_select)
				{
					if (m_trueExecCount != 0)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentItemsCountInfoMethod
//
/////////////////////////////////////////
crJXJCheckCurrentItemsCountInfoMethod::crJXJCheckCurrentItemsCountInfoMethod():
	m_trueExecCount(),
	m_itemid(0),
	m_count(0)
{
}
crJXJCheckCurrentItemsCountInfoMethod::crJXJCheckCurrentItemsCountInfoMethod(const crJXJCheckCurrentItemsCountInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_itemid(handle.m_itemid),
	m_count(handle.m_count)
{
}
void crJXJCheckCurrentItemsCountInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckCurrentItemsCountInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 1:
		m_itemid = (atoi(str.c_str()));
		break;
	case 2:
		m_count = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentItemsCountInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerData->getDataClass(); 
	crData *brainData = crBrain::getInstance()->getDataClass();
	if(brainData)
	{
		if(m_trueExecCount != 0)
		{
			void *param;
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			int Count = 0;
			int totalCount = 0;
			for( JXJItemBagVec::iterator itr = itembagvec->begin();
				itr != itembagvec->end();
				++itr)
			{
				if((*itr)!=NULL && (*itr)->getItemID()!=0)
				{
					if ((*itr)->getItemID() == m_itemid)
					{
						Count = (*itr)->getItemCount();
					}
					totalCount += Count;
				}
			}
			brainData->getParam(WCHDATA_JXJBrainResourcesCardCount,param);
			int brainCount = *(int *)param;
			if(totalCount >= m_count)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFormationRaidoSelectMethod
//			编队兵种选择
/////////////////////////////////////////
crJXJCheckFormationRaidoSelectMethod::crJXJCheckFormationRaidoSelectMethod():
	m_trueExecCount(1)
{
}
crJXJCheckFormationRaidoSelectMethod::crJXJCheckFormationRaidoSelectMethod(const crJXJCheckFormationRaidoSelectMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_formationRaido(handle.m_formationRaido),
	m_count(handle.m_count),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckFormationRaidoSelectMethod::inputParam(int i, void *param)
{
}
void crJXJCheckFormationRaidoSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_formationRaido = str;
		break;
	case 2:
		m_count = (atoi(str.c_str()));
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFormationRaidoSelectMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(formationCanvas.valid())
	{
		crRadioGroupWidgetNode *formationRaido = dynamic_cast<crRadioGroupWidgetNode *>(formationCanvas->getWidget(m_formationRaido));
		if (formationRaido)
		{
			if (formationRaido->getSelect() == m_count)
			{
				if (m_trueExecCount != 0)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckTaskTempFormationInfoMethod
//			第一行编入长戟兵
/////////////////////////////////////////
crJXJCheckTaskTempFormationInfoMethod::crJXJCheckTaskTempFormationInfoMethod():
	m_armid(0),
	m_trueExecCount(0)
{
}
crJXJCheckTaskTempFormationInfoMethod::crJXJCheckTaskTempFormationInfoMethod(const crJXJCheckTaskTempFormationInfoMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_armid(handle.m_armid),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckTaskTempFormationInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckTaskTempFormationInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_armid = (atoi(str.c_str()));
		break;
	case 2:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckTaskTempFormationInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	if(m_trueExecCount != 0)
	{
		ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
		if(formationCanvas.valid())
		{
			crData *formationgdata = formationCanvas->getDataClass();
			if(formationgdata)
			{
				formationgdata->getParam(WCHDATA_JXJTempFormationVec,param);
				FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
				FormationInfoVec::iterator itr_info = formationInfoVec->begin();
				for (;itr_info != formationInfoVec->end();itr_info++)
				{
					if((*itr_info) != NULL && itr_info->get()->getAbstractID() == m_armid)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
						break;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckCurrentSelectRadio2Method
//
/////////////////////////////////////////
crJXJCheckCurrentSelectRadio2Method::crJXJCheckCurrentSelectRadio2Method():
	m_select(0)
{
}
crJXJCheckCurrentSelectRadio2Method::crJXJCheckCurrentSelectRadio2Method(const crJXJCheckCurrentSelectRadio2Method& handle):
	crMethod(handle),
	m_GongFangCanvas(handle.m_GongFangCanvas),
	m_GongFangRaido(handle.m_GongFangRaido),
	m_select(handle.m_select)
{
}
void crJXJCheckCurrentSelectRadio2Method::inputParam(int i, void *param)
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
void crJXJCheckCurrentSelectRadio2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_GongFangCanvas = str;
		break;
	case 1:
		m_GongFangRaido = str;
		break;
	case 2:
		m_select = (atoi(str.c_str()));
		break;
	}
}
void crJXJCheckCurrentSelectRadio2Method::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> gongFangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_GongFangCanvas);
		if (gongFangCanvas.valid())
		{
			crRadioGroupWidgetNode *gongFangRaido = dynamic_cast<crRadioGroupWidgetNode *>(gongFangCanvas->getWidget(m_GongFangRaido));
			if (gongFangRaido)
			{
				if (gongFangRaido->getSelect() == m_select)
				{
					check = true;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUILordPropertyTipsMethod
//
/////////////////////////////////////////
crJXJUILordPropertyTipsMethod::crJXJUILordPropertyTipsMethod():
	m_this(NULL),
	m_str(0),
	m_index(0)
{
}

crJXJUILordPropertyTipsMethod::crJXJUILordPropertyTipsMethod(const crJXJUILordPropertyTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_propertyName(handle.m_propertyName),
	m_propertySW(handle.m_propertySW),
	m_textInputOne(handle.m_textInputOne),
	m_textInputTwo(handle.m_textInputTwo),
	m_textInputThree(handle.m_textInputThree),
	m_this(NULL),
	m_str(handle.m_str),
	m_index(handle.m_index)
{
}
void crJXJUILordPropertyTipsMethod::inputParam(int i, void *param)
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
void crJXJUILordPropertyTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_propertyName = str;
		break;
	case 2:
		m_propertySW = str;
		break;
	case 3:
		m_textInputOne = str;
		break;
	case 4:
		m_textInputTwo = str;
		break;
	case 5:
		m_textInputThree = str;
		break;
	case 6:
		m_str = atoi(str.c_str());
		break;
	case 7:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJUILordPropertyTipsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	if (playerGameData && tips.valid())
	{
		crStaticTextWidgetNode *propertyName = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_propertyName));
		crStaticTextWidgetNode *textInputOne = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_textInputOne));
		crStaticTextWidgetNode *textInputTwo = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_textInputTwo));
		crHypertextWidgetNode *textInputThree = dynamic_cast<crHypertextWidgetNode *>(tips->getWidget(m_textInputThree));
		crMultiSwitch *propertySW = dynamic_cast<crMultiSwitch *>(tips->getChildNode(m_propertySW));
		std::string str,str1,str2;
		if(propertyName)propertyName->clearString();
		if(textInputOne)textInputOne->clearString();
		if(textInputTwo)textInputTwo->clearString();
		if(textInputThree)textInputThree->clearText();

		void *param;
		crRole *mainRole = playerGameData->getMainRole();
		crData *data = playerGameData->getDataClass();	
		if(mainRole && data)
		{		
			crData *mainroledata = mainRole->getDataClass();	
			if(mainroledata)
			{
				data ->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				//兵法--》攻击力
				mainroledata->getParam(WCHDATA_JXJBingFa,param);
				short bingfa = *(short *)param;
				float totalbingfa = 0.0f;
				mainRole->doEvent(WCH_JXJGetBingFa,MAKEINT64(&totalbingfa,NULL));
				short bingfaInt = totalbingfa-bingfa;
				crVector3f bingfaFactor;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBingFaFactor,viplv),bingfaFactor);
				short bingfaPercent = (totalbingfa * bingfaFactor[0]/((totalbingfa * bingfaFactor[1]) + bingfaFactor[2])) * 1000.0f;
				char buf[16];
				sprintf(buf,"%.1f\0",(float)bingfaPercent*0.1f);
				std::string bingfaStr = crArgumentParser::trim(buf);

				//阵法--》防御力
				float totalzhenfa = 0.0f;
				mainroledata->getParam(WCHDATA_JXJZhenFa,param);
				short zhenfa = *(short *)param;
				mainRole->doEvent(WCH_JXJGetZhenFa,MAKEINT64(&totalzhenfa,NULL));
				int zhenfaInt = totalzhenfa-zhenfa;
				crVector3f zhenfaFactor;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJZhenFaFactor,viplv),zhenfaFactor);
				short zhenfaPercent = (totalzhenfa * zhenfaFactor[0]/((totalzhenfa * zhenfaFactor[1]) + zhenfaFactor[2])) * 1000.0f;
				sprintf(buf,"%.1f\0",(float)zhenfaPercent*0.1f);
				std::string zhenfaStr = crArgumentParser::trim(buf);

				//统率 影响领导力
				float totletongshuai = 0.0f;
				mainroledata->getParam(WCHDATA_JXJTongShuai,param);
				short tongshuai = *(short *)param;
				crVector3f tongshuaifactor;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),tongshuaifactor);
				mainRole->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
				//int tongshuaiInt = totletongshuai * tongshuaifactor[0] + tongshuaifactor[1];
				//int tongshuaiInt = (totletongshuai-tongshuai);
				int tongshuaiInt = totletongshuai;
				int permaxlead = tongshuaiInt * tongshuaifactor[0] + tongshuaifactor[1];
				std::string tongshuaiStr = crArgumentParser::appItoa(permaxlead);
				//洞察--》魔抗
				float totledongcha = 0.0f;
				mainroledata->getParam(WCHDATA_JXJDongCha,param);
				short dongcha = *(short *)param;
				mainRole->doEvent(WCH_JXJGetDongCha,MAKEINT64(&totledongcha,NULL));
				crVector3f dongchaFactor;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJDongChaFactor,viplv),dongchaFactor);
				int dongchaInt = totledongcha-dongcha;
				short dongchaPercent = (totledongcha * dongchaFactor[0]/((totledongcha * dongchaFactor[1]) + dongchaFactor[2])) * 1000.0f;
				sprintf(buf,"%.1f\0",(float)dongchaPercent*0.1f);
				std::string dongchaStr = crArgumentParser::trim(buf);

				std::string tmpPlusStr = "+";

				switch (m_str)
				{
				case 1:
					str = "官职可以提升编队的\n总领导力";
					str1 = "官职";
					break;
				case 2:
					str = "官职提升的必须条件";
					str1 = "功勋";
					break;
				case 3:
					//str = "影响单个兵的攻击力效果，随属性增加而提升\n"+tmpPlusStr+crArgumentParser::appItoa(bingfaInt)+"（提升所有兵种的攻击力"+bingfaStr+"%）";
					str = "提升所有兵种的攻击力:" + bingfaStr + "%";
					str1 = "兵法";
					break;
				case 4:
					//str = "影响单个兵的护甲效果随，属性增加而提升\n" + tmpPlusStr + crArgumentParser::appItoa(zhenfaInt) + "（提升所有兵种的防御力" + zhenfaStr + "%）";
					str = "提升所有兵种的防御力:" + zhenfaStr + "%";
					str1 = "阵法";
					break;
				case 5:
					//str = "影响编队的最大领导力效果，随属性增加而提升\n" + tmpPlusStr + crArgumentParser::appItoa(tongshuaiInt) + "（提升编队领导力" + tongshuaiStr + "）";
					str = "提升编队领导力:" + tongshuaiStr;
					str1 = "统率";
					break;
				case 6:
					//str = "影响单个兵对将卡技能的免伤效果，随属性增加而提升\n" + tmpPlusStr + crArgumentParser::appItoa(dongchaInt) + "（提升所有兵种的魔法抗性" + dongchaStr + "%）";
					str = "提升所有兵种的魔法抗性:" + dongchaStr + "%";
					str1 = "洞察";
					break;
				case 7:
					str = "经验：";
					str1 = "经验条";
					str2 = "已满级";
					break;
				}
				if (propertyName && m_str <= 7)
				{
					propertyName->setVisiable(true);
					propertyName->setString(str1);
					propertyName->setColor(FCOLOR_YELLOW);
				}
				mainroledata->getParam(WCHDATA_Experience,param);
				int curexperience = *(int *)param;
				crTableIO::StrVec record,recordd,recordc;
				ref_ptr<crTableIO>LordlvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordlvTab);
				int lvid = LordlvTab->getTitleIndex("lv");
				int experience = LordlvTab->getTitleIndex("经验");
				mainroledata->getParam(WCHDATA_Level,param);
				unsigned char *nowlv = (unsigned char *)param;
				unsigned char nextlv = *nowlv+1;
				int maxlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,viplv).c_str());
				if (LordlvTab->queryOneRecord(lvid,crArgumentParser::appItoa((int)(*nowlv)),recordd) >= 0)
				{
					if(LordlvTab->queryOneRecord(lvid,crArgumentParser::appItoa((int)nextlv),record) >= 0)
					{
						if (m_str == 7)
						{
							if(propertySW)
							{
								propertySW->setActiveSwitchSet(1);
							}
							if (textInputTwo)
							{
								if ((int)nextlv > maxlv)
								{
									textInputTwo->setColor(FCOLOR_BLACK);
									textInputTwo->setVisiable(true);
									textInputTwo->setString(str2);
								}
								else
								{
									textInputTwo->setVisiable(true);
									if ((int)(*nowlv) == 1)
									{
										if(textInputTwo)
											textInputTwo->setString(str + crArgumentParser::appItoa(curexperience) + "/" + crArgumentParser::appItoa(atoi(record[experience].c_str()) - atoi(recordd[experience].c_str())));
									}
									else if(textInputTwo)
										textInputTwo->setString(str + crArgumentParser::appItoa(curexperience - atoi(recordd[experience].c_str()))+"/"+crArgumentParser::appItoa(atoi(record[experience].c_str()) - atoi(recordd[experience].c_str())));
									textInputTwo->setColor(FCOLOR_BLACK);
								}
							}
						}
					}
					else
					{
						if (m_str == 7)
						{
							if(propertySW)
							{
								propertySW->setActiveSwitchSet(1);
							}
							if ((int)nextlv > maxlv)
							{
								if (textInputTwo)
								{
									textInputTwo->setColor(FCOLOR_BLACK);
									textInputTwo->setVisiable(true);
									textInputTwo->setString(str2);
								}
							}
						}
					}
				}
				if (m_str < 7)
				{
					if(propertySW)
					{
						propertySW->setActiveSwitchSet(0);
					}
					if (textInputOne)
					{
						textInputOne->setVisiable(true);
						textInputOne->setString(str);
						textInputOne->setColor(FCOLOR_BLACK);
					}
				}
				int abstractid = -1;
				ref_ptr<crTableIO>troopsTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				if (m_str == 8)//获取副本左上军队id
				{
					crMyPlayerData *me = crMyPlayerData::getInstance();
					if(me)
					{
						crRole *role = me->getLoadedRoleByIndex(m_index);
						if(role) abstractid = role->getAbstractItemID();
					}
				}
				if (m_str == 9)//获取副本选中军队的id
				{
					ref_ptr<CRPhysics::crCamera> camera = CRPhysics::crCameraManager::getInstance()->getMainCamera();
					if(camera.valid())
					{
						crData *cameraData = camera->getAttachedNode()->getDataClass();
						if(cameraData)
						{
							CRCore::ScopedLock<crData> lock(*cameraData);
							cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
							SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
							if(!selectMyRoles->empty())
							{
								crRole *role = (*selectMyRoles)[0].get();
								if(role) abstractid = role->getAbstractItemID();
							}
						}
					}
				}
				if(m_str >= 8 && abstractid > 0 && troopsTable->queryOneRecord(0, crArgumentParser::appItoa(abstractid), record) >= 0)
				{
					int infoid = troopsTable->getTitleIndex("说明");
					int nameindex = troopsTable->getTitleIndex("name");
					std::string name = record[nameindex];
					if(propertySW)
					{
						propertySW->setActiveSwitchSet(2);
					}
					if (propertyName)
					{
						propertyName->setVisiable(true);
						propertyName->setString(name);
						propertyName->setColor(FCOLOR_YELLOW);
					}
					if(textInputThree)
					{
						textInputThree->setVisiable(true);
						std::string file = record[infoid];
						if(!file.empty())
						{
							file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
							textInputThree->setHypertext(file);
						}
						else
						{
							textInputThree->clearText();
						}
					}
				}
				const crBoundingBox &btnbox = m_this->getBoundBox();
				const crBoundingBox &tipsbox = tips->getBoundBox();
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				float posx,posy;
				posx = btnbox.m_min[0];
				posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
				//float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
				if (m_str == 8 || m_str == 9)
				{
					float posx1 = (tipsbox.m_max[0]-tipsbox.m_min[0])*0.5;
					float posy1 = (tipsbox.m_max[1]-tipsbox.m_min[1])*0.5;
					crVector3 pos(btnbox.m_max[0],btnbox.m_max[1],0.0f);
					/////////////////////////////////////////
					crVector3 pos2(btnbox.m_min[0] ,btnbox.m_min[1],0.0f);
					/////////////////////////////////////////
					if (pos2[0] >= 0)
					{
						posx = pos2[0] - posx1;
					}
					else
					{
						posx = pos[0] + posx1;
					}
					if (pos2[1] >= 0)
					{
						posy = pos[1] - posy1;
					}
					else
					{
						posy = pos2[1] + posy1;
					}
				}
				////////////
				crVector2 mouse(crVector2(posx,posy));
				tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
				//tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
				tips->setCanFocus(false);
				crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckGameMainLineTaskMethod
//
/////////////////////////////////////////
crJXJCheckGameMainLineTaskMethod::crJXJCheckGameMainLineTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckGameMainLineTaskMethod::crJXJCheckGameMainLineTaskMethod(const crJXJCheckGameMainLineTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckGameMainLineTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckGameMainLineTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckGameMainLineTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		unsigned short fubenid;
		if (m_trueExecCount != 0)
		{
			int taskid = m_this->getTaskID();
			ref_ptr<crTableIO>taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
			if (taskTitleTab.valid())
			{
				int fubenidindex = taskTitleTab->getTitleIndex("fubenid");
				int taskidindex = taskTitleTab->getTitleIndex("taskid");
				crTableIO::StrVec taskrecord;
				if(taskTitleTab->queryOneRecord(taskidindex,crArgumentParser::appItoa(taskid),taskrecord) >= 0)
				{
					fubenid = (unsigned short)atoi(taskrecord[fubenidindex].c_str());
					crData *data = playerData->getDataClass();
					data->getParam(WCHDATA_JXJFubenProgressMap,param);
					FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
					if (fubenProgress)
					{
						FubenProgressMap::iterator itr = fubenProgress->find((unsigned short)fubenid);
						if (itr != fubenProgress->end() && itr->second->getComplete() > 0)
						{
							check = true;
							if(m_trueExecCount>0)
								m_trueExecCount--;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJAutoMainLineTaskMethod
//
/////////////////////////////////////////
crJXJAutoMainLineTaskMethod::crJXJAutoMainLineTaskMethod()
{
}
crJXJAutoMainLineTaskMethod::crJXJAutoMainLineTaskMethod(const crJXJAutoMainLineTaskMethod& handle):
	crMethod(handle)
{
}
void crJXJAutoMainLineTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJAutoMainLineTaskMethod::addParam(int i, const std::string& str)
{

}
void crJXJAutoMainLineTaskMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		playerData->doEvent(WCH_JXJReferTask,MAKEINT64(m_this->getTaskID(),NULL));
	}
}

/////////////////////////////////////////
//
//crJXJAutoCompensationTaskMethod
//
/////////////////////////////////////////
crJXJAutoCompensationTaskMethod::crJXJAutoCompensationTaskMethod()
{
}
crJXJAutoCompensationTaskMethod::crJXJAutoCompensationTaskMethod(const crJXJAutoCompensationTaskMethod& handle):
	crMethod(handle)
{
}
void crJXJAutoCompensationTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJAutoCompensationTaskMethod::addParam(int i, const std::string& str)
{

}
void crJXJAutoCompensationTaskMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		playerData->doEvent(WCH_JXJReferTask,MAKEINT64(m_this->getTaskID(),NULL));
	}
}

/////////////////////////////////////////
//
//crJXJZhengZhanTianXiaTaskRaidoSelectMethod
//
/////////////////////////////////////////
crJXJZhengZhanTianXiaTaskRaidoSelectMethod::crJXJZhengZhanTianXiaTaskRaidoSelectMethod():
	m_taskid(NULL)
{
}
crJXJZhengZhanTianXiaTaskRaidoSelectMethod::crJXJZhengZhanTianXiaTaskRaidoSelectMethod(const crJXJZhengZhanTianXiaTaskRaidoSelectMethod& handle):
	crMethod(handle),
	m_fubenCanvas(handle.m_fubenCanvas),
	m_zhengzhanfubenRaido(handle.m_zhengzhanfubenRaido),
	m_chapterRaido(handle.m_chapterRaido),
	m_fubenRaido(handle.m_fubenRaido),
	m_chapterscroll(handle.m_chapterscroll),
	m_taskid(handle.m_taskid)
{
}
void crJXJZhengZhanTianXiaTaskRaidoSelectMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJZhengZhanTianXiaTaskRaidoSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenCanvas = str;
		break;
	case 1:
		m_zhengzhanfubenRaido = str;
		break;
	case 2:
		m_chapterRaido = str;
		break;
	case 3:
		m_fubenRaido = str;
		break;
	case 4:
		m_chapterscroll = str;
		break;
	case 5:
		m_taskid = (atoi(str.c_str()));
		break;
	}
}
void crJXJZhengZhanTianXiaTaskRaidoSelectMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	int taskid;
	if(playerData)
	{
		void *param;
		crRole *mainRole = playerData->getMainRole();
		crData *mainData = mainRole->getMetierDataClass();
		if(!mainData) return;
		mainData->excHandle(MAKEINT64(WCH_LockData,1));
		mainData->getParam(WCHDATA_GameTaskMap,param);
		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
		for( GameTaskMap::iterator itr = gameTaskMap->begin();
			 itr != gameTaskMap->end();
			 ++itr )
		{
			if(itr->second->getActivation() && !itr->second->isComplete())
			{
				taskid = itr->second->getTaskID();
				if(taskid == m_taskid)
				{
					crTableIO::StrVec record,recordf;
					ref_ptr<crTableIO>taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
					int taskfubenidindex = taskTitleTab->getTitleIndex("fubenid");
					if(taskTitleTab->queryOneRecord(2,crArgumentParser::appItoa(taskid),record) >= 0)
					{
						int curfubenid = atoi(record[taskfubenidindex].c_str());
						ref_ptr<crTableIO>fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
						int chapteridindex = fubenTab->getTitleIndex("chapterid");
						int fubenidindex = fubenTab->getTitleIndex("id");

						crTableIO::DataVec recordcVec;
						ref_ptr<crTableIO>chapterTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChapterTab);
						int modeid = chapterTab->getTitleIndex("mode");
						chapterTab->queryRecords(modeid,crArgumentParser::appItoa(0),recordcVec);
						int startchapterid = atoi(recordcVec[0][0].c_str());

						if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(curfubenid),recordf) >= 0)
						{
							int curchapterid = atoi(recordf[chapteridindex].c_str());
							ref_ptr<crCanvasNode> fubenCanvas = crFilterRenderManager::getInstance()->findCanvas(m_fubenCanvas);
							if (fubenCanvas.valid())
							{
								crRadioGroupWidgetNode *zhengzhanfubenRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_zhengzhanfubenRaido));
								crRadioGroupWidgetNode *chapterRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_chapterRaido));
								crRadioGroupWidgetNode *fubenRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_fubenRaido));
								crScrollBarWidgetNode *chapterscroll = dynamic_cast<crScrollBarWidgetNode *>(fubenCanvas->getWidget(m_chapterscroll));
								crRadioGroupWidgetNode::RadioGroup&radioGroup = chapterRaido->getRadioGroup();
								int maxsize = radioGroup.size();
								if (zhengzhanfubenRaido)
								{
									zhengzhanfubenRaido->select(0);
									zhengzhanfubenRaido->doEvent(WCH_UI_SelectChange);
									if (zhengzhanfubenRaido->getSelect() == 0)
									{
										if (chapterRaido)
										{
											if (curchapterid - startchapterid < maxsize)
											{
												chapterRaido->select(curchapterid - startchapterid);
												chapterRaido->doEvent(WCH_UI_SelectChange);
												if (chapterRaido->getSelect() == curchapterid - startchapterid)
												{
													crTableIO::DataVec recordVec;
													fubenTab->queryRecords(chapteridindex,crArgumentParser::appItoa(curchapterid),recordVec);
													int beginfubenid = atoi(recordVec[0][0].c_str());
													int select = 0;
													if (fubenRaido)
													{
														for (int i = 0;i < recordVec.size();i++)
														{
															if (atoi(recordVec[i][0].c_str()) == curfubenid)
															{
																select = i;
																break;
															}
														}
														//int fubenCount = recordVec.size();
														//int select = curfubenid - beginfubenid;
														//int select = fubenCount - 1;
														crRadioGroupWidgetNode::RadioGroup &fubenRadioGroup = fubenRaido->getRadioGroup();
														int raidoCount = fubenRadioGroup.size();
														void *param;
														crData *Canvasdata = fubenCanvas->getDataClass();
														Canvasdata->getParam(WCHDATA_JXJFubenCurPage,param);
														int *curpage = (int *)param;
														if (select < raidoCount)
														{
															fubenRaido->select(select);
														}
														else if (select < 2 * raidoCount)
														{
															*curpage = 2;
															fubenRaido->select(select - raidoCount);
														}
														else if (select < 3 * raidoCount)
														{
															*curpage = 3;
															fubenRaido->select(select - 2*raidoCount);
														}
														fubenRaido->doEvent(WCH_UI_SelectChange);
													}
												}
											}
											else if(curchapterid - startchapterid >= maxsize)
											{
												chapterscroll->setValue(curchapterid - startchapterid - maxsize + 1);
												chapterRaido->select(maxsize - 1);
												chapterRaido->doEvent(WCH_UI_SelectChange);
												if (chapterRaido->getSelect() == maxsize - 1)
												{
													crTableIO::DataVec recordVec;
													fubenTab->queryRecords(chapteridindex,crArgumentParser::appItoa(curchapterid),recordVec);
													int beginfubenid = atoi(recordVec[0][0].c_str());
													int select = 0;
													if (fubenRaido)
													{
														for (int i = 0;i < recordVec.size();i++)
														{
															if (atoi(recordVec[i][0].c_str()) == curfubenid)
															{
																select = i;
																break;
															}
														}
														//int fubenCount = recordVec.size();
														//int select = curfubenid - beginfubenid;
														//int select = fubenCount - 1;
														crRadioGroupWidgetNode::RadioGroup &fubenRadioGroup = fubenRaido->getRadioGroup();
														int raidoCount = fubenRadioGroup.size();
														void *param;
														crData *Canvasdata = fubenCanvas->getDataClass();
														Canvasdata->getParam(WCHDATA_JXJFubenCurPage,param);
														int *curpage = (int *)param;
														if (select < raidoCount)
														{
															fubenRaido->select(select);
														}
														else if (select < 2 * raidoCount)
														{
															*curpage = 2;
															fubenRaido->select(select - raidoCount);
														}
														else if (select < 3 * raidoCount)
														{
															*curpage = 3;
															fubenRaido->select(select - 2*raidoCount);
														}
														fubenRaido->doEvent(WCH_UI_SelectChange);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		mainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJNewHandFuBenInitMethod
//
/////////////////////////////////////////
crJXJNewHandFuBenInitMethod::crJXJNewHandFuBenInitMethod()
{

}
crJXJNewHandFuBenInitMethod::crJXJNewHandFuBenInitMethod(const crJXJNewHandFuBenInitMethod& handle):
	crMethod(handle),
	m_rectCanvas(handle.m_rectCanvas),
	m_targetCanvasName(handle.m_targetCanvasName),
	m_targetWidgetName(handle.m_targetWidgetName)
{
}
void crJXJNewHandFuBenInitMethod::inputParam(int i, void *param)
{
}
void crJXJNewHandFuBenInitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_rectCanvas = str;
		break;
	case 1:
		m_targetCanvasName = str;
		break;
	case 2:
		m_targetWidgetName = str;
		break;
	}
}
void crJXJNewHandFuBenInitMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvas);
	if(rectCanvas.valid())
	{
		ref_ptr<crCanvasNode> targetCanvasName = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvasName);
		if(!targetCanvasName)return;
		crWidgetNode *targetWidget = targetCanvasName->getWidget(m_targetWidgetName);
		if(!targetWidget)return;
		if (!targetWidget->getVisiable())
		{
			rectCanvas->setVisiable(false);
		}
		crMatrix mat = targetCanvasName->getMatrix();
		crVector3 pos = targetWidget->getBound().center();
		pos = pos * mat;
		rectCanvas->setPosition(pos);
	}
}

/////////////////////////////////////////
//
//crJXJUIManorStairJueSeTipMethod
//
/////////////////////////////////////////
crJXJUIManorStairJueSeTipMethod::crJXJUIManorStairJueSeTipMethod():
	m_str(0)
{
}

crJXJUIManorStairJueSeTipMethod::crJXJUIManorStairJueSeTipMethod(const crJXJUIManorStairJueSeTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_jueSeCanvas(handle.m_jueSeCanvas),
	m_str(handle.m_str)
{
}
void crJXJUIManorStairJueSeTipMethod::inputParam(int i, void *param)
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
void crJXJUIManorStairJueSeTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	case 2:
		m_jueSeCanvas = str;
		break;
	case 3:
		m_str = atoi(str.c_str());
		break;
	}
}

void crJXJUIManorStairJueSeTipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
		if(!tips.valid())
			return;
		crStaticTextWidgetNode *tipstext = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipstext));
		ref_ptr<crCanvasNode> jueSeCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jueSeCanvas);
		if(!jueSeCanvas.valid())
			return;
		crData *Canvasdata = jueSeCanvas->getDataClass();
		Canvasdata->getParam(WCHDATA_JXJProFoodProduct,param);
		int foodpro = *(int *)param;
		Canvasdata->getParam(WCHDATA_JXJProWoodProduct,param);
		int woodpro = *(int *)param;
		Canvasdata->getParam(WCHDATA_JXJProIronProduct,param);
		int ironpro = *(int *)param;
		Canvasdata->getParam(WCHDATA_JXJProHorseProduct,param);
		int horsepro = *(int *)param;
		int money = -1;
		int minjulv = 0;
		int nongtianlv = 0;
		int muchanglv = 0;
		int kuangshanlv = 0;
		int lindilv = 0;
		if (data)
		{
			for (int i = 0; i < 8; ++i)
			{
				if (i < 4)
				{
					data->getParam(WCHDATA_JXJMuchanglv1 + i,param);
					muchanglv += *(unsigned char *)param;
					data->getParam(WCHDATA_JXJLingdilv1 + i,param);
					lindilv += *(unsigned char *)param;
					data->getParam(WCHDATA_JXJKuangshanlv1 + i,param);
					kuangshanlv += *(unsigned char *)param;
				}
				data->getParam(WCHDATA_JXJMingjulv1 + i,param);
				minjulv += *(unsigned char *)param;
				data->getParam(WCHDATA_JXJNongtianlv1 + i,param);
				nongtianlv += *(unsigned char *)param;
			}
		}
		CRCore::crData *playerData;
		CRNetApp::crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(myPlayer)
		{
			playerData = myPlayer->getDataClass();
		}
		
		std::string str = "";
		switch (m_str)
		{
		case 1:
			str = "粮食产量：" + crArgumentParser::appItoa(foodpro) + " 农田等级：" +  crArgumentParser::appItoa(nongtianlv);
			break;
		case 2:
			str = "木材产量：" + crArgumentParser::appItoa(woodpro) + " 林地等级：" +  crArgumentParser::appItoa(lindilv);
			break;
		case 3:
			str = "铁矿产量：" + crArgumentParser::appItoa(ironpro) + " 矿山等级：" +  crArgumentParser::appItoa(kuangshanlv);
			break;
		case 4:
			str = "马匹产量：" + crArgumentParser::appItoa(horsepro) + " 牧场等级：" +  crArgumentParser::appItoa(muchanglv);
			break;
		case 5:
			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJCoppercash,param);
				int money = *(int *)param;
				str = "铜钱：" + crArgumentParser::appItoa(money) + " 民居等级：" +  crArgumentParser::appItoa(minjulv);
			}
			break;
		case 6:
			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJGiftGold,param);
				int giftgold = *(int *)param;
				str = "礼金：" + crArgumentParser::appItoa(giftgold);
			}
			break;
		case 7:
			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJGoldingot,param);
				int gold = *(int *)param;
				str = "元宝：" + crArgumentParser::appItoa(gold);
			}
			break;
		case 8:
			if(playerData)
			{
				crRole *mainRole = playerGameData->getMainRole();
				crData *mainRoleData = mainRole->getDataClass();
				mainRoleData->getParam(WCHDATA_Experience,param);
				int curexperience = *(int *)param;
				
				mainRoleData->getParam(WCHDATA_Level,param);
				unsigned char *nowlv = (unsigned char *)param;

				crData *data = myPlayer->getDataClass();
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int maxlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,viplv).c_str());

				if((int)(*nowlv) < maxlv)
				{
					crTableIO::StrVec record1, record2;
					ref_ptr<crTableIO>LordlvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordlvTab);
					int lvIndex = LordlvTab->getTitleIndex("lv");
					int experienceIndex = LordlvTab->getTitleIndex("经验");
					if (LordlvTab->queryOneRecord(lvIndex,crArgumentParser::appItoa((int)(*nowlv)),record1) >= 0)
					{
						int experience1 = atoi(record1[experienceIndex].c_str());
						if (LordlvTab->queryOneRecord(lvIndex,crArgumentParser::appItoa((int)(*(nowlv)+1)),record2) >= 0)
						{
							int experience2 = atoi(record2[experienceIndex].c_str());
							str = "经验：" + crArgumentParser::appItoa(curexperience - experience1) + " / "  + crArgumentParser::appItoa(experience2 - experience1);
						}
					}
				}
				else
				{
					str = "已满级";
				}
			}
			break;
		}
		playerData->getParam(WCHDATA_JXJFukulv,param);
		unsigned char fukulv = *(unsigned char *)param;
		playerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		int maxlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,viplv).c_str());
		int nextlv = CRCore::minimum((int)fukulv + 1,maxlv);
		ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);
		ref_ptr<crTableIO>resnameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResNameTab);
		crTableIO::StrVec record,recordn,recordf;
		fukutab->queryOneRecord(0,crArgumentParser::appItoa((int)fukulv),record);
		int foodlimitid = fukutab->getTitleIndex("粮食");
		int woodlimitid = fukutab->getTitleIndex("木材");
		int ironlimitid = fukutab->getTitleIndex("铁矿");
		int horselimitid = fukutab->getTitleIndex("马匹");

		fukutab->queryOneRecord(0,crArgumentParser::appItoa(nextlv),recordf);
		float fukulimitaddpercent = 0.0f;
		myPlayer->doEvent(WCH_JXJGetFukuLimitAdd,MAKEINT64(&fukulimitaddpercent,NULL));

		CRCore::crVector4 overColor;
		//std::string str = "255,0,0,255";
		crArgumentParser::appAtoVec("255,0,0,255",overColor);
		overColor /= 255.0f;
		if(tipstext)
		{
			switch(m_str)
			{
			case 1:
				{
					playerData->getParam(WCHDATA_JXJFood,param);
					int curfood = *(int *)param;
					int foodlimit = atoi(record[foodlimitid].c_str()) * (1.0f + fukulimitaddpercent);
					tipstext->setColor(curfood>=foodlimit?overColor:crVector4(0.0f,0.0f,0.0f,1.0f));
					tipstext->setString(str);
					break;
				}
			case 2:
				{
					playerData->getParam(WCHDATA_JXJWood,param);
					int curwood = *(int *)param;
					int woodlimit = atoi(record[woodlimitid].c_str()) * (1.0f + fukulimitaddpercent);
					tipstext->setColor(curwood>=woodlimit?overColor:crVector4(0.0f,0.0f,0.0f,1.0f));
					tipstext->setString(str);
					break;
				}
			case 3:
				{
					playerData->getParam(WCHDATA_JXJIron,param);
					int curiron = *(int *)param;
					int ironlimit = atoi(record[ironlimitid].c_str()) * (1.0f + fukulimitaddpercent);
					tipstext->setColor(curiron>=ironlimit?overColor:crVector4(0.0f,0.0f,0.0f,1.0f));
					tipstext->setString(str);
					break;
				}
			case 4:
				{
					playerData->getParam(WCHDATA_JXJHorse,param);
					int curhorse = *(int *)param;
					int horselimit = atoi(record[horselimitid].c_str()) * (1.0f + fukulimitaddpercent);
					tipstext->setColor(curhorse>=horselimit?overColor:crVector4(0.0f,0.0f,0.0f,1.0f));
					tipstext->setString(str);
					break;
				}
			case 5:
			case 6:
			case 7:
			case 8:
				tipstext->setColor(crVector4(0.0f,0.0f,0.0f,1.0f));
				tipstext->setString(str);
				break;
			}
		}
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		//float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJUIFuKuProResourceCountMethod
//
/////////////////////////////////////////
crJXJUIFuKuProResourceCountMethod::crJXJUIFuKuProResourceCountMethod()
{
}
crJXJUIFuKuProResourceCountMethod::crJXJUIFuKuProResourceCountMethod(const crJXJUIFuKuProResourceCountMethod& handle):
	crMethod(handle),
	m_jueSeCanvas(handle.m_jueSeCanvas)
{
}
void crJXJUIFuKuProResourceCountMethod::inputParam(int i, void *param)
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
void crJXJUIFuKuProResourceCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jueSeCanvas = str;
		break;
	}
}
void crJXJUIFuKuProResourceCountMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> jueSeCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jueSeCanvas);
	if(!jueSeCanvas.valid()) return;
	ref_ptr<crTableIO>resbuildingtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResBuildingTab);
	int foodproid = resbuildingtab->getTitleIndex("粮食");
	int woodproid = resbuildingtab->getTitleIndex("木材");
	int ironproid = resbuildingtab->getTitleIndex("铁矿");
	int horseproid = resbuildingtab->getTitleIndex("马匹");
	crData *data = playerData->getDataClass();
	crRole *mainRole = playerData->getMainRole();
	crTableIO::StrVec record;
	void *param;
	//crData *mainRole
	///////////////////////////////////////
	float percent[4] = {0.0f};
	float percent1[4] = {0.0f};
	if(mainRole && data)
	{
		//data->getParam(WCHDATA_JXJTreasonPunishTimer,param);
		//int punishtime = *(int *)param; 
		for (int i = 0; i < 4; i++)
		{
			mainRole->doEvent(WCH_JXJGetResProductPercent,MAKEINT64(&percent[i],i+1));//1.food,2.wood,3.iron,4.horse
			playerData->doEvent(WCH_JXJGetTechResProductPercent,MAKEINT64(&percent1[i],i+1));
			percent[i] += percent1[i];
			//if(punishtime > 0)
				//percent[i] = percent[i] - 0.5f;
		}
	}

	data->getParam(WCHDATA_JXJTreasonPunishTimer,param);
	int punishtime = *(int *)param; 
	bool punish = false;
	if(punishtime > 0)
		punish = true;
	
	////////////////////////////
	crData *Canvasdata = jueSeCanvas->getDataClass();
	float foodpro = 0;
	float woodpro = 0;
	float ironpro = 0;
	float horsepro = 0;
	float foodproadd = 0;
	float woodproadd = 0;
	float ironproadd = 0;
	float horseproadd = 0;
	int startid = WCHDATA_JXJNongtianlv1;
	for (int i = 0;i < 8;i++,startid++)
	{
		data->getParam(startid,param);
		unsigned char nongtianlv = *(unsigned char *)param;
		if (nongtianlv>0)
		{
			if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)nongtianlv),record) >= 0)
			{
				foodpro += atof(record[foodproid].c_str()) * (1.0f + percent[0]);
				foodproadd += atof(record[foodproid].c_str()) * percent[0];
			}
		}
	}
	int ifoodpro = foodpro;
	int ifoodproadd = foodproadd;
	if (punish)
	{
		ifoodpro = foodpro*0.5f;
		ifoodproadd = foodproadd*0.5f;
	}
	Canvasdata->inputParam(WCHDATA_JXJProFoodProduct, &ifoodpro);
	Canvasdata->inputParam(WCHDATA_JXJProFoodProductAdd, &ifoodproadd);
	startid = WCHDATA_JXJLingdilv1;
	for (int i = 0;i < 4;i++,startid++)
	{
		data->getParam(startid,param);
		unsigned char lindilv = *(unsigned char *)param;
		if (lindilv>0)
		{
			if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lindilv),record) >= 0)
			{
				woodpro += atof(record[woodproid].c_str()) * (1.0f + percent[1]);
				woodproadd += atof(record[woodproid].c_str()) * percent[1];
			}
		}
	}
	int iwoodpro = woodpro;
	int iwoodproadd = woodproadd;
	if (punish)
	{
		iwoodpro = woodpro*0.5f;
		iwoodproadd = woodproadd*0.5f;
	}
	Canvasdata->inputParam(WCHDATA_JXJProWoodProduct, &iwoodpro);
	Canvasdata->inputParam(WCHDATA_JXJProWoodProductAdd, &iwoodproadd);
	startid = WCHDATA_JXJKuangshanlv1;
	for (int i = 0;i < 4;i++,startid++)
	{
		data->getParam(startid,param);
		unsigned char kuangshanlv = *(unsigned char *)param;
		if (kuangshanlv>0)
		{
			if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)kuangshanlv),record) >= 0)
			{
				ironpro += atof(record[ironproid].c_str()) * (1.0f + percent[2]);
				ironproadd += atof(record[ironproid].c_str()) * percent[2];
			}
		}
	}
	int iironpro = ironpro;
	int iironproadd = ironproadd;
	if (punish)
	{
		iironpro = ironpro*0.5f;
		iironproadd = ironproadd*0.5f;
	}
	Canvasdata->inputParam(WCHDATA_JXJProIronProduct, &iironpro);
	Canvasdata->inputParam(WCHDATA_JXJProIronProductAdd, &iironproadd);
	startid = WCHDATA_JXJMuchanglv1;
	for (int i = 0;i < 4;i++,startid++)
	{
		data->getParam(startid,param);
		unsigned char muchanglv = *(unsigned char *)param;
		if (muchanglv>0)
		{
			if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)muchanglv),record) >= 0)
			{
				horsepro += atof(record[horseproid].c_str()) * (1.0f + percent[3]);
				horseproadd += atof(record[horseproid].c_str()) * percent[3];
			}
		}
	}
	int ihorsepro = horsepro;
	int ihorseproadd = horseproadd;
	if (punish)
	{
		ihorsepro = horsepro*0.5f;
		ihorseproadd = horseproadd*0.5f;
	}
	Canvasdata->inputParam(WCHDATA_JXJProHorseProduct, &ihorsepro);
	Canvasdata->inputParam(WCHDATA_JXJProHorseProductAdd, &ihorseproadd);
}

/////////////////////////////////////////
//
//crJXJCheckSelFuBenCompleteInfoMethod
//
/////////////////////////////////////////
crJXJCheckSelFuBenCompleteInfoMethod::crJXJCheckSelFuBenCompleteInfoMethod()
{

}
crJXJCheckSelFuBenCompleteInfoMethod::crJXJCheckSelFuBenCompleteInfoMethod(const crJXJCheckSelFuBenCompleteInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJCheckSelFuBenCompleteInfoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckSelFuBenCompleteInfoMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckSelFuBenCompleteInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJFubenOrChengchiID, param);
		unsigned short selfubenid = *(unsigned short *)param;
		if(type == FT_Fuben && selfubenid>0)
		{
			data->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
			if(selfubenid > 0)
			{
				if (fubenProgress)
				{
					FubenProgressMap::iterator itrcur;
					itrcur = fubenProgress->find((unsigned short)selfubenid);
					if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 1)
					{
						check = true;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJSelFuBenCompleteTipsMethod
//
/////////////////////////////////////////
crJXJSelFuBenCompleteTipsMethod::crJXJSelFuBenCompleteTipsMethod()
{
}
crJXJSelFuBenCompleteTipsMethod::crJXJSelFuBenCompleteTipsMethod(const crJXJSelFuBenCompleteTipsMethod& handle):
	crMethod(handle),
	m_fubenTipCanvas(handle.m_fubenTipCanvas),
	m_tipInput(handle.m_tipInput)
{
}
void crJXJSelFuBenCompleteTipsMethod::inputParam(int i, void *param)
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
void crJXJSelFuBenCompleteTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenTipCanvas = str;
		break;
	case 1:
		m_tipInput = str;
		break;
	}
}
void crJXJSelFuBenCompleteTipsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> fubenTipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_fubenTipCanvas);
		if(fubenTipCanvas.valid())
		{
			crStaticTextWidgetNode *tipInput = dynamic_cast<crStaticTextWidgetNode *>(fubenTipCanvas->getWidget(m_tipInput));
			if (tipInput)
			{
				std::string str = "此副本已通关，需重置之后才能\n再次进入！";
				tipInput->setString(str);
			}
			crFilterRenderManager::getInstance()->doModal(fubenTipCanvas.get());
		}
	}
}

/////////////////////////////////////////
//
//crJXJCardShopOnShowMethod
//
/////////////////////////////////////////
crJXJCardShopOnShowMethod::crJXJCardShopOnShowMethod()
{
}

crJXJCardShopOnShowMethod::crJXJCardShopOnShowMethod(const crJXJCardShopOnShowMethod& handle):
	crMethod(handle)
{
}

void crJXJCardShopOnShowMethod::inputParam(int i, void *param)
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

void crJXJCardShopOnShowMethod::addParam(int i, const std::string& str)
{
}

void crJXJCardShopOnShowMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		//void *param;
		//crTableIO::StrVec record;
		//CRUI::crCheckBoxWidgetNode *checkBox = NULL;
		//int junlingindex = fubentab -> getTitleIndex("军令");
		//int nameindex = fubentab -> getTitleIndex("name");
		//crCanvasNode *junlingtips = crFilterRenderManager::getInstance()->findCanvas(m_junlingtips);
		//crStaticTextWidgetNode *fubeninput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_fubeninput));
		//crRadioGroupWidgetNode *rushradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_rushradio));
		//crEditWidgetNode *rushcountedit = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_rushcountedit));
		//crStaticTextWidgetNode *junlingcountinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_junlingcountinput));
		//crButtonWidgetNode *startbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_startrushbtn));

		//for(int i = 0;i<7;i++)
		//{
		//	checkBox = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_checkkbox[i]));
		//	if(checkBox)
		//	{
		//		checkBox->setVisiable(false);
		//	}
		//}

		//unsigned char junling = 0;
		//crData *data = playerGameData->getDataClass();
		//data->getParam(WCHDATA_JXJFubenID,param);
		//unsigned short fubenid = *(unsigned short *)param;
		//data->getParam(WCHDATA_JXJJunling,param);
		//junling = *(unsigned char *)param;
		//if (junlingcountinput)
		//{
		//	junlingcountinput->setString(std::string("0"));
		//}
		//if (rushcountedit)
		//{
		//	rushcountedit->setString(std::string("0"));
		//}
		//if (rushradio)
		//{
		//	rushradio->select(FubenRushCountType_All);
		//}
		//if (fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record) && nameindex > 0)
		//{
		//	if (fubeninput)
		//	{
		//		fubeninput->setString(record[nameindex]);
		//	}
		//	if (junlingtips)
		//	{
		//		if (junling < atoi(record[junlingindex].c_str()))
		//		{
		//			if (startbtn)
		//			{
		//				startbtn->setEnable(false);
		//			}
		//			crFilterRenderManager::getInstance()->doModal(junlingtips);
		//			//crFilterRenderManager::getInstance()->showCanvas(junlingtips,true);
		//		}
		//		else
		//		{
		//			if (startbtn)
		//			{
		//				startbtn->setEnable(true);
		//			}
		//		}
		//	}
		//}
	}
}
/////////////////////////////////////////
//
//crJXJCardShopUpdateMethod
//
/////////////////////////////////////////
crJXJCardShopUpdateMethod::crJXJCardShopUpdateMethod()
{
}

crJXJCardShopUpdateMethod::crJXJCardShopUpdateMethod(const crJXJCardShopUpdateMethod& handle):
	crMethod(handle),
	m_typeradio(handle.m_typeradio),
	m_colorradio(handle.m_colorradio),
	m_cardpiecesinput(handle.m_cardpiecesinput),
	m_pagenuminput(handle.m_pagenuminput),
	m_foreignCardSwitch(handle.m_foreignCardSwitch),
	m_foreignCardRadio(handle.m_foreignCardRadio)
{
	for (int i = 0; i < 8; i++)
	{
		m_priceinput[i] = handle.m_priceinput[i];
		m_purchasebtn[i] = handle.m_purchasebtn[i];
		m_cardicon[i] = handle.m_cardicon[i];
		m_colorswitch[i] = handle.m_colorswitch[i];
	}
	for (int i = 0; i < 3; i++)
	{
		m_foreignCardImg[i] = handle.m_foreignCardImg[i];
		m_foreignCardInfo[i] = handle.m_foreignCardInfo[i];
		m_foreignCardPrice[i] = handle.m_foreignCardPrice[i];
	}
}

void crJXJCardShopUpdateMethod::inputParam(int i, void *param)
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

void crJXJCardShopUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_typeradio = str;
		break;
	case 1:
		m_colorradio = str;
		break;
	case 2:
		m_cardpiecesinput = str;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		m_priceinput[i-3] = str;
		break;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
		m_purchasebtn[i - 15] = str;
		break;
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
		m_cardicon[i - 27] = str;
		break;
	case 39:
		m_pagenuminput = str;
		break;
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		m_colorswitch[i - 40] = str;
		break;
	case 52:
		m_foreignCardSwitch = str;
		break;
	case 53:
	case 54:
	case 55:
		m_foreignCardImg[i - 53] = str;
		break;
	case 56:
	case 57:
	case 58:
		m_foreignCardInfo[i - 56] = str;
		break;
	case 59:
	case 60:
	case 61:
		m_foreignCardPrice[i - 59] = str;
		break;
	case 62:
		m_foreignCardRadio = str;
		break;
	default:
		break;
	}
}

void crJXJCardShopUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(m_this && playerGameData && itemtab.valid())
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		data ->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		data ->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;
		int typeindex = itemtab->getTitleIndex("将卡类型");
		int colorindex = itemtab->getTitleIndex("品质");
		int iconindex = itemtab->getTitleIndex("icon");
		int priceindex = itemtab->getTitleIndex("商店价格");
		int shiliindex = itemtab->getTitleIndex("国家");
		int onepagecount = 12;//(int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCardShopPerPageNum,viplv).c_str()));
		crData *canvasdata = m_this->getDataClass();
		crRadioGroupWidgetNode *typeradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_typeradio));
		crRadioGroupWidgetNode *colorradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_colorradio));
		crStaticTextWidgetNode *cardpiecesinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cardpiecesinput));
		crStaticTextWidgetNode *pagenuminput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pagenuminput));
		crImageBoxWidgetNode *cardicon[12] = {NULL};
		crStaticTextWidgetNode *priceinput[12] = {NULL};
		crButtonWidgetNode *perchagebtn[12] = {NULL};
		crMultiSwitch *colorswitch[12];
		for (int i = 0; i < 12; i++)
		{
			cardicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_cardicon[i]));
			priceinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_priceinput[i]));
			perchagebtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_purchasebtn[i]));
			colorswitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_colorswitch[i]));
			if (cardicon[i])cardicon[i]->setVisiable(false);
			if (priceinput[i])priceinput[i]->setVisiable(false);
			if (perchagebtn[i])perchagebtn[i]->setVisiable(false);
			if (colorswitch[i])
			{
				colorswitch[i]->setVisiable(false);
				colorswitch[i]->setActiveSwitchSet(0);
			}
		}
		ref_ptr<crMultiSwitch> foreignCardSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_foreignCardSwitch));
		ref_ptr<crRadioWidgetNode> foreignCardRadio = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_foreignCardRadio));
		int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJForeignCardOpenLevel, 0).c_str());
		crData *roledata = playerGameData->getMainRole()->getDataClass();
		roledata->getParam(WCHDATA_Level, param);
		unsigned char lv = *(unsigned char *)param;
		if (foreignCardRadio.valid())
			foreignCardRadio->setEnable(lv >= openlv);
		if (canvasdata && typeradio && colorradio && typeindex >= 0 && colorindex >= 0 && iconindex >= 0 && priceindex >= 0)
		{
			data->getParam(WCHDATA_JXJCardPieces, param);
			int cardpieces = *(int *)param;
			if (cardpiecesinput)
			{
				cardpiecesinput->setString("拥有将魂：" + crArgumentParser::appItoa(cardpieces));
			}

			crTableIO::DataVec cardrecords;
			int jiangkatype = typeradio->getSelect();
			if (jiangkatype < 3)
			{
				if (foreignCardSwitch.valid())
					foreignCardSwitch->setActiveSwitchSet(0);
				int jiangkacolor = colorradio->getSelect();
				crRadioGroupWidgetNode::RadioGroup&radioGroup = colorradio->getRadioGroup();
				int colorsize = radioGroup.size() - jiangkacolor - 1;
				canvasdata->getParam(WCHDATA_JXJCardShopCurPage, param);
				int curpage = *(int *)param;
				canvasdata->getParam(WCHDATA_JXJCardShopItemDeq, param);
				ShopCardIDDeq *cardiddeq = (ShopCardIDDeq *)param;
				cardiddeq->clear();
				int row = -(curpage - 1) * onepagecount;
				unsigned char cardshili = 0;
				int cardCount = 0;
				int totalpage = 0;
				itemtab->queryRecords(typeindex, crArgumentParser::appItoa(jiangkatype), colorindex, crArgumentParser::appItoa(colorsize), cardrecords);
				if (!cardrecords.empty())
				{
					for (crTableIO::DataVec::iterator itr = cardrecords.begin();
						itr != cardrecords.end();
						++itr)
					{
						if (atoi((*itr)[priceindex].c_str()) > 0)
						{
							cardshili = atoi((*itr)[shiliindex].c_str());
							if (cardshili < c_startShiliID || shiliid == cardshili)
							{
								cardCount++;
								if (row >= 0 && row < onepagecount)
								{
									cardiddeq->push_back(atoi((*itr)[0].c_str()));
									if (cardicon[row])
									{
										cardicon[row]->setVisiable(true);
										cardicon[row]->setImageName((*itr)[iconindex]);
									}
									if (priceinput[row])
									{
										priceinput[row]->setVisiable(true);
										priceinput[row]->setString((*itr)[priceindex]);
									}
									if (perchagebtn[row])
									{
										perchagebtn[row]->setVisiable(true);
									}
									if (colorswitch[row])
									{
										colorswitch[row]->setVisiable(true);
										colorswitch[row]->setActiveSwitchSet(atoi((*itr)[colorindex].c_str()));
									}
								}
								row++;
							}
						}
					}
					totalpage = ceil((float)cardCount / (float)onepagecount);
					if (totalpage == 0)
					{
						totalpage = 1;
					}
					canvasdata->inputParam(WCHDATA_JXJCardShopTotalPage, &totalpage);
				}
				if (pagenuminput)
				{
					pagenuminput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}
			}
			else if(lv >= openlv)
			{
				if (foreignCardSwitch.valid())
					foreignCardSwitch->setActiveSwitchSet(1);
				ref_ptr<crImageBoxWidgetNode> foreignCardImg[3];
				ref_ptr<crImageBoxWidgetNode> foreignCardInfo[3];
				ref_ptr<crStaticTextWidgetNode> foreignCardPrice[3];
				data->getParam(WCHDATA_JXJForeignCard, param);
				crVector3i foreignCard = *(crVector3i *)param;
				ref_ptr<crTableIO>foreignCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJForeignCardTab);
				crTableIO::StrVec itemRecord, priceRecord;
				int imgindex = itemtab->getTitleIndex("picture");
				int infoindex = itemtab->getTitleIndex("姓名");
				for (int i = 0; i < 3; i++)
				{
					foreignCardImg[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_foreignCardImg[i]));
					foreignCardInfo[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_foreignCardInfo[i]));
					foreignCardPrice[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_foreignCardPrice[i]));
					if (foreignCard[i] > 0 &&
						itemtab->queryOneRecord(0, crArgumentParser::appItoa(foreignCard[i]), itemRecord) >= 0 &&
						foreignCardTab->queryOneRecord(0, crArgumentParser::appItoa(foreignCard[i]), priceRecord) >= 0)
					{
						if (foreignCardImg[i].valid())
						{
							foreignCardImg[i]->setImageName(itemRecord[imgindex]);
							foreignCardImg[i]->setVisiable(true);
						}
						if (foreignCardInfo[i].valid())
						{
							foreignCardInfo[i]->setImageName(itemRecord[infoindex]);
							foreignCardInfo[i]->setVisiable(true);
						}
						if (foreignCardPrice[i].valid())
						{
							foreignCardPrice[i]->setString(priceRecord[2]);
							foreignCardPrice[i]->setVisiable(true);
						}
					}
					else
					{
						if (foreignCardImg[i].valid())
						{
							foreignCardImg[i]->setVisiable(false);
						}
						if (foreignCardInfo[i].valid())
						{
							foreignCardInfo[i]->setVisiable(false);
						}
						if (foreignCardPrice[i].valid())
						{
							foreignCardPrice[i]->setVisiable(false);
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIPurChaseCardMethod
//
/////////////////////////////////////////
crJXJUIPurChaseCardMethod::crJXJUIPurChaseCardMethod():
	m_index(0),
	m_font("simhei.ttf"),
	m_characterSize(16),
	m_goldColor(0.0f,1.0f,1.0f,1.0f),
	m_textColor(1.0f,1.0f,0.0f,1.0f)
{
}

crJXJUIPurChaseCardMethod::crJXJUIPurChaseCardMethod(const crJXJUIPurChaseCardMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_textColor(handle.m_textColor),
	m_goldColor(handle.m_goldColor),
	m_font(handle.m_font),
	m_characterSize(handle.m_characterSize),
	m_okbtn(handle.m_okbtn)
{
}

void crJXJUIPurChaseCardMethod::inputParam(int i, void *param)
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

void crJXJUIPurChaseCardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
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
	case 4:
		m_font = str;
		break;
	case 5:
		m_characterSize = atoi(str.c_str());
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}

void crJXJUIPurChaseCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			std::string buildingname;
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crData *data = playerGameData->getDataClass();
			int cardid = 0;
			int price = 0;
			crData *canvasdata = m_this->getParentCanvas()->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJCardShopItemDeq,param);
				ShopCardIDDeq *cardiddeq = (ShopCardIDDeq *)param;
				if (m_index < cardiddeq->size())
				{
					cardid  = (*cardiddeq)[m_index];
				}
			}
			if (cardid > 0)
			{
				int needbagcount = 1;
				playerGameData->doEvent(WCH_JXJCheckJiangkaBagEmpty,MAKEINT64(&needbagcount,NULL));
				if (needbagcount == 0)
				{
					crTableIO::StrVec record;
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(cardid),record) >= 0)
					{
						int priceindex = itemtab->getTitleIndex("商店价格");
						if (priceindex > 0)
						{
							price = atoi(record[priceindex].c_str());
							data->getParam(WCHDATA_JXJCardPieces,param);
							int cardimpression = *(int *)param;
							if (cardimpression >= price)
							{
								canvasdata->inputParam(WCHDATA_JXJPurchaseCardID,&cardid);
								crHypertextWidgetNode *information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
								crButtonWidgetNode *okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
								std::vector<float>v_i;
								ItemMap itemMap;
								rcfg::ConfigScript cfg_script(&itemMap);
								cfg_script.Add("Hypertext");
								cfg_script.Push("Hypertext");
								//cfg_script.Add("Font",m_font);
								//cfg_script.Add("CharacterSize",m_characterSize);

								cfg_script.Add("Content");
								cfg_script.Push("Content",1);
								v_i.clear();
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
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
								char tmpText[32];
								sprintf(tmpText,"%d将魂\0",price);
								cfg_script.Add("Text",tmpText);
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",3);
								v_i.clear();
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","购买吗？");
								cfg_script.Pop();

								cfg_script.Pop();

								if(information)information->setHypertext(cfg_script);
								if(okbtn)okbtn->setEnable(true);
								crData *dlgData = canvas->getDataClass();
								int commandtype = CDP_Widget;
								dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
								dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
								crFilterRenderManager::getInstance()->doModal(canvas.get());
							}
							else
							{
								playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2039,NULL));
							}
						}
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2034,NULL));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJPurChaseCardMethod
//
/////////////////////////////////////////
crJXJPurChaseCardMethod::crJXJPurChaseCardMethod()
{}
crJXJPurChaseCardMethod::crJXJPurChaseCardMethod(const crJXJPurChaseCardMethod& handle):
	crMethod(handle)
{
}
void crJXJPurChaseCardMethod::inputParam(int i, void *param)
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
void crJXJPurChaseCardMethod::addParam(int i, const std::string& str)
{
}

void crJXJPurChaseCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		crData *data = playerGameData->getDataClass();
		data ->getParam(WCHDATA_JXJCardShopOpenTechLv,param);
		unsigned char CardShopOpenTechLv = *(unsigned char *)param;
		if (CardShopOpenTechLv > 0)
		{
			crData *canvasdata = m_this->getParentCanvas()->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJPurchaseCardID,param);
				int cardid = *(int *)param;

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(cardid);
				CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPurChaseCard,stream.get());
				net_conductor->getNetManager()->sendPacket("all",packet);
			}
		}
		else
		{
			playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2082,NULL));//请先研发将卡商店科技！
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvPurChaseCardMethod
//
/////////////////////////////////////////
crJXJRecvPurChaseCardMethod::crJXJRecvPurChaseCardMethod():
	m_netType(GameClient_Game){}
crJXJRecvPurChaseCardMethod::crJXJRecvPurChaseCardMethod(const crJXJRecvPurChaseCardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPurChaseCardMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
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

void crJXJRecvPurChaseCardMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvPurChaseCardMethod::operator()(crHandle &handle)
{
	//////////////////////////////////
	//返回
	//0：失败
	//1：成功
	//2：将魂不足
	//3：背包已满
	//////////////////////////////////
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			unsigned char success = 0;
			int cardid = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				void *param;
				data ->getParam(WCHDATA_JXJCardShopOpenTechLv,param);
				unsigned char CardShopOpenTechLv = *(unsigned char *)param;
				if (CardShopOpenTechLv > 0)
				{
					crVector2i giftbag;
					int needbagcount = 1;
					int price = 0;
					//data->getParam(WCHDATA_JXJCardPieces,param);
					//int *cardpieces = (int *)param;
					ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
					m_this->doEvent(WCH_JXJCheckJiangkaBagEmpty,MAKEINT64(&needbagcount,NULL));
					if (needbagcount == 0)
					{
						crTableIO::StrVec record;
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(cardid),record) >= 0)
						{
							int priceindex = itemtab->getTitleIndex("商店价格");
							if (priceindex > 0)
							{
								price = -atoi(record[priceindex].c_str());
								data->excHandle(MAKEINT64(WCH_LockData,1));
								m_this->doEvent(WCH_JXJChangePlayerData,MAKEINT64(&price,T_JiangHun));
								if(0 == price)
								{
									success = 1;
									//*cardpieces -= price;
									ref_ptr<crBagItemData> bagitemdata = new crBagItemData;
									bagitemdata->setItemID(cardid);
									bagitemdata->setItemCount(1);
									RewardItemVec rewardItems;
									rewardItems.push_back(bagitemdata);
									m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
								}
								else
								{
									success = 2;
								}
								data->excHandle(MAKEINT64(WCH_LockData,0));
							}

						}
					}
					else
					{
						success = 3;
					}
				}
				else
				{
					success = 4;
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(success);
				//stream->_writeInt(*cardpieces);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvPurChaseCard,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,cardid));
				GameLogData gamelog(Log_CardShopBuyCard,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			//void *param;
			unsigned char success = m_stream->_readUChar();
			//int cardpiece = m_stream->_readInt();
			//crData *data = m_this->getDataClass();
			//data->getParam(WCHDATA_JXJCardPieces,param);
			//int totalpieces = *(int *)param;
			//int extra = cardpiece - totalpieces;
			////data->inputParam(WCHDATA_JXJCardPieces,&cardpiece);
			//if(extra!=0)
			//{
			//	crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_JiangHun,extra));
			//}
			if (success == 2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2035,NULL));//将魂不足
			}
			else if (success == 3)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2034,NULL));//将卡背包已满
			}
			else if (success == 4)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2082,NULL));//请先研发将卡商店科技！
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJFuBenResetInfoMethod
//
/////////////////////////////////////////
crJXJFuBenResetInfoMethod::crJXJFuBenResetInfoMethod()
{}
crJXJFuBenResetInfoMethod::crJXJFuBenResetInfoMethod(const crJXJFuBenResetInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJFuBenResetInfoMethod::inputParam(int i, void *param)
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
void crJXJFuBenResetInfoMethod::addParam(int i, const std::string& str)
{
}

void crJXJFuBenResetInfoMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	if (canvas.valid() && crMyPlayerData::getInstance()->getPlayerGameData())
	{
		crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		if(data)
		{
			void *param;
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJSelFubenID,param);
			unsigned short selfubenid = *(unsigned short *)param;
			unsigned short progresscount = 0;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
			if(selfubenid > 0)
			{
				if (fubenProgress)
				{
					FubenProgressMap::iterator itrcur;
					itrcur = fubenProgress->find((unsigned short)selfubenid);
					if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 1)
					{
						progresscount = itrcur->second->getComplete();
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			if (progresscount > 1)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUShort(selfubenid);
				CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFuBenResetInfo,stream.get());
				net_conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIFuBenResetInfoMethod
//
/////////////////////////////////////////
crJXJUIFuBenResetInfoMethod::crJXJUIFuBenResetInfoMethod():
	m_this(NULL),
	m_textColor(crVector4f(100.0f,190.0f,12.0f,255.0f))
{}
crJXJUIFuBenResetInfoMethod::crJXJUIFuBenResetInfoMethod(const crJXJUIFuBenResetInfoMethod& handle):
	crMethod(handle),
	m_promptCanvas(handle.m_promptCanvas),
	m_input(handle.m_input),
	m_okbtn(handle.m_okbtn),
	m_textColor(handle.m_textColor),
	m_this(NULL)
{
}
void crJXJUIFuBenResetInfoMethod::inputParam(int i, void *param)
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
void crJXJUIFuBenResetInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_promptCanvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_okbtn = str;
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_textColor);
		break;
	case 4:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJUIFuBenResetInfoMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> promptCanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
	if (promptCanvas.valid() && m_this && playerData)
	{
		crHypertextWidgetNode *input = dynamic_cast<crHypertextWidgetNode *>(promptCanvas->getWidget(m_input));
		ref_ptr<crButtonWidgetNode > okbtn = dynamic_cast<crButtonWidgetNode *>(promptCanvas->getWidget(m_okbtn));
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			if (data)
			{
				void *param;
				crData *canvasdata = canvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJSelFubenID,param);
				unsigned short selfubenid = *(unsigned short *)param;
				unsigned short progresscount = 0;
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
				if(selfubenid > 0)
				{
					if (fubenProgress)
					{
						FubenProgressMap::iterator itrcur;
						itrcur = fubenProgress->find((unsigned short)selfubenid);
						if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 1)
						{
							progresscount = itrcur->second->getComplete();
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if (progresscount > 1)
				{
					ref_ptr<crTableIO>FuBenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
					crTableIO::StrVec record;
					int goldindex = FuBenTab->getTitleIndex("重置价格");
					int needgold = 0;
					int checkneedgold = 0;
					if(FuBenTab->queryOneRecord(0,crArgumentParser::appItoa(selfubenid),record) >= 0)
					{
						needgold = atoi(record[goldindex].c_str());
						checkneedgold = needgold;
						playerData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&checkneedgold,NULL));
						if(checkneedgold == 0)
						{
							if(okbtn.valid())okbtn->setEnable(true);
							int hinttype = JXJGoldCostType_ResetMingJiaRoad;
							crData *braindata = crBrain::getInstance()->getDataClass();
							braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
						
							data->getParam(WCHDATA_JXJGoldCostHintInt,param);
							unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_ResetMingJiaRoad;
							if (ifhint)
							{
								std::vector<float>v_i;
								ItemMap itemMap;
								rcfg::ConfigScript cfg_script(&itemMap);
								cfg_script.Add("Hypertext");
								cfg_script.Push("Hypertext");
								cfg_script.Add("Content");
								cfg_script.Push("Content");
								cfg_script.Add("Text","您确定要花费");
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",2);
								v_i.clear();
								v_i.push_back(m_textColor[0]);
								v_i.push_back(m_textColor[1]);
								v_i.push_back(m_textColor[2]);
								v_i.push_back(m_textColor[3]);
								cfg_script.Add("Color",v_i);
								char tmpText[160]/*,tmpText2[80]*/;
								sprintf(tmpText,"【%d礼金/元宝】\0",needgold);
								cfg_script.Add("Text",tmpText);
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",3);
								cfg_script.Add("Text","重置副本进度吗？");
								cfg_script.Pop();

								cfg_script.Pop();
								if(input)input->setHypertext(cfg_script);
							}

							if (promptCanvas.valid())
							{
								crData *dlgData = promptCanvas->getDataClass();
								int commandtype = CDP_Widget;
								dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
								dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
								if (ifhint)
								{
									crFilterRenderManager::getInstance()->doModal(promptCanvas.get());
								}
								else
								{
									if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
								}
								//crFilterRenderManager::getInstance()->doModal(promptCanvas.get());
							}
						}
						else
						{
							playerData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvFuBenResetInfoMethod
//
/////////////////////////////////////////
crJXJRecvFuBenResetInfoMethod::crJXJRecvFuBenResetInfoMethod():
	m_netType(GameClient_Game){}
crJXJRecvFuBenResetInfoMethod::crJXJRecvFuBenResetInfoMethod(const crJXJRecvFuBenResetInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvFuBenResetInfoMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
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

void crJXJRecvFuBenResetInfoMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvFuBenResetInfoMethod::operator()(crHandle &handle)
{
	//////////////////////////////////
	//返回
	//0：失败
	//1：成功
	//2：元宝不足
	//////////////////////////////////
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			unsigned char success = 0;
			unsigned short selfubenid = m_stream->_readUShort();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			//unsigned char *goldcost = 0;
			crTableIO::StrVec record;
			if(playerData.valid())
			{
				void *param;
				int progresscount = 0;
				bool needdeductgold = false;
				data->excHandle(MAKEINT64(WCH_LockData,1));
				ref_ptr<crTableIO>FuBenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				int goldindex = FuBenTab->getTitleIndex("重置价格");
				data->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
				if(fubenProgress && goldindex > 0 && selfubenid > 0  && FuBenTab->queryOneRecord(0,crArgumentParser::appItoa(selfubenid),record) >= 0)
				{
					FubenProgressMap::iterator itrcur = fubenProgress->find(selfubenid);
					if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 1)
					{
						progresscount = itrcur->second->getComplete();
						int needgold = 0;
						if (progresscount > 1)
						{
							needgold = atoi(record[goldindex].c_str());
							MoneyChangeData moneydata(needgold,"副本重置ID");
							m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
							if (moneydata.first == 0)
							{
								success = 1;
								itrcur->second->setFubenProgress(0);
								itrcur->second->setComplete(1);
							}
							else
							{
								success = 2;
							}
						}
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(3);
					stream->_writeUChar(success);
					//stream->_writeInt(*goldcost);
					stream->_writeUShort(selfubenid);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeUChar(success);
					//stream->_writeInt(*goldcost);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvFuBenResetInfo,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if (needdeductgold)
				{
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,selfubenid));
				GameLogData gamelog(Log_FubenReset,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success == 1)
			{
				unsigned short selfubenid = m_stream->_readUShort();
				void *param;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
				if(selfubenid > 0)
				{
					if (fubenProgress)
					{
						FubenProgressMap::iterator itrcur;
						itrcur = fubenProgress->find(selfubenid);
						if (itrcur != fubenProgress->end())
						{
							itrcur->second->setFubenProgress(0);
							itrcur->second->setComplete(1);
						}
					}
				}
			}
			if (success == 2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGongFangTipsMethod
//
/////////////////////////////////////////
crJXJUIGongFangTipsMethod::crJXJUIGongFangTipsMethod()
{}
crJXJUIGongFangTipsMethod::crJXJUIGongFangTipsMethod(const crJXJUIGongFangTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemlv(handle.m_itemlv),
	m_itemicon(handle.m_itemicon),
	m_itemequipon(handle.m_itemequipon),
	m_colorred(handle.m_colorred),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	m_gongFangTypeRadio(handle.m_gongFangTypeRadio),
	m_tuZhiLevelRadio(handle.m_tuZhiLevelRadio),
	m_this(NULL)
{
}
void crJXJUIGongFangTipsMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemlv = str;
		break;
	case 4:
		m_itemicon = str;
		break;
	case 5:
		m_itemequipon = str;
		break;
	case 6:
		m_infocanvas = str;
		break;
	case 7:
		m_infobk = str;
		break;
	case 8:
		m_infodi = str;
		break;
	case 9:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 10:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 11:
		m_gongFangTypeRadio = str;
		break;
	case 12:
		m_tuZhiLevelRadio = str;
		break;
	}
}
void crJXJUIGongFangTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid())
	{
		void *param;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec record,lordrecord;
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		crStaticTextWidgetNode *itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crStaticTextWidgetNode *itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
		//
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		if (info2)
		{
			info2->clearText();
		}
		if (itemlv)
		{
			itemlv->clearString();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}
		if (itemequipon)
		{
			itemequipon->clearString();
		}
		crRole *mainRole = playerData->getMainRole();
		crData *roleData = mainRole->getDataClass();
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		crRadioGroupWidgetNode *radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getParentWidget());
		int index = radioGroup->getRadioIndex(dynamic_cast<crRadioWidgetNode *>(m_this));
		crData *data = crBrain::getInstance()->getDataClass();
		crTableIO::DataVec recordVec;
		int equiptypeid = itemtab->getTitleIndex("类型");
		itemtab->queryRecords(equiptypeid,crArgumentParser::appItoa(IT_Paper),recordVec);
		data->getParam(WCHDATA_JXJCurForgeEquipID,param);
		int row = *(int *)param + index;
		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
		canvasdata->getParam(WCHDATA_JXJGongFangTuZhiVec,param);
		GongFangTuZhiVec *tuZhiVec = (GongFangTuZhiVec *)param;
		int itemid = 0;
		crRadioGroupWidgetNode *gongFangTypeRadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_gongFangTypeRadio));
		if(gongFangTypeRadio)
		{
			int gongFangRadio = gongFangTypeRadio->getSelect();
			if (gongFangRadio == 1)
			{
				if ((*tuZhiVec).size() > index)
				{
					itemid = (*tuZhiVec)[index];
				}
			}
		}
		if (itemid > 0)
		{
			int iconindex = itemtab->getTitleIndex("icon");
			int nameindex = itemtab->getTitleIndex("name");
			int typeindex = itemtab->getTitleIndex("类型");
			int colorindex = itemtab->getTitleIndex("品质");
			int needlvindex = itemtab->getTitleIndex("人物等级");
			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
			{
				int colormodeid = atoi(record[colorindex].c_str());
				if(name)
				{
					name->setColor(crVector4(1,1,0,1));
					name->setString(record[nameindex]);
				}
				if (itemlv)
				{
					if (rolelv < atoi(record[needlvindex].c_str()))
					{
						itemlv->setColor(m_colorred);
					}
					else
					{
						itemlv->setColor(crVector4(1,1,1,1));
					}
					itemlv->setString(record[needlvindex]);
				}
				if (itemicon)
				{
					itemicon->setImageName(record[iconindex]);
				}
				if (itemequipon)
				{
					std::string str;// = "未使用";
					if (atoi(record[typeindex].c_str()) == IT_Equip)
					{
						if (m_index == 1)
						{
							str = "已装备";
						}
						else
						{
							str = "未装备";
						}
					}
					itemequipon->setString(str);
				}
				if(info2)
				{
					ItemMap itemMap;
					rcfg::ConfigScript outscript(&itemMap);
					crVector2i item(itemid,0);
					playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
					info2->setHypertext(outscript);
				}
			}
			/////////////////////////////////////////
			float posx2,posy2;
			float boxXlength = infobk->getChild(0)->getBoundBox().xLength();
			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;
				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
					//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(infobk)infobk->setMatrix(rotmat);

				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					//needmove = boxYlength - texheight;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}
			/////////

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tip = tipsCanvas->getBoundBox();
			float posx = (tip.m_max[0]-tip.m_min[0])*0.5;
			float posy = (tip.m_max[1]-tip.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			pos = pos * mat;
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos2 = pos2 * mat;
			/////////////////////////////////////////
			if (pos2[0] >= 0)
			{
				boxXlength = -boxXlength;
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + 2.0f * posy - needmove;// + posinfoy;
			}
			crVector2 mouse(crVector2(posx2,posy2));

			float needmovex = 0;
			if (m_index == 1)
			{
				needmovex += boxXlength;
			}
			//tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f)/* * mat*/);
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0] + needmovex,mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0] + needmovex,mouse[1],0.0f));
			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
		else
		{
			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
			crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIForgeLordEquipTipsMethod
//
/////////////////////////////////////////
crJXJUIForgeLordEquipTipsMethod::crJXJUIForgeLordEquipTipsMethod():
	m_ea(NULL),
	m_this(NULL),
	//m_ifexpand(false),
	//m_expandnum(1.0f),
	m_imageindex(0)
{
}
crJXJUIForgeLordEquipTipsMethod::crJXJUIForgeLordEquipTipsMethod(const crJXJUIForgeLordEquipTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info1(handle.m_info1),
	m_info2(handle.m_info2),
	m_itemlv(handle.m_itemlv),
	m_itemicon(handle.m_itemicon),
	m_itemequipon(handle.m_itemequipon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJUIForgeLordEquipTipsMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
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
void crJXJUIForgeLordEquipTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info1 = str;
		break;
	case 3:
		m_info2 = str;
		break;
	case 4:
		m_itemlv = str;
		break;
	case 5:
		m_itemicon = str;
		break;
	case 6:
		m_itemequipon = str;
		break;
	case 7:
		m_canvas = str;
		break;
	case 8:
		m_infocanvas = str;
		break;
	case 9:
		m_infobk = str;
		break;
	case 10:
		m_infodi = str;
		break;
	case 11:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 12:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 13:
		m_imageindex = atoi(str.c_str());
		break;
	}
}
void crJXJUIForgeLordEquipTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid())
	{
		bool ifshowcanvas = false;
		void *param;
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		//crStaticTextWidgetNode *info1 = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_info1));
		crStaticTextWidgetNode *itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crStaticTextWidgetNode *itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
		//
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		//if (info1)
		//{
		//	info1->clearString();
		//}
		if (info2)
		{
			info2->clearText();
		}
		if (itemlv)
		{
			itemlv->clearString();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}
		if (itemequipon)
		{
			itemequipon->clearString();
		}
		crRole *mainRole = playerData->getMainRole();
		crData *roleData = mainRole->getDataClass();
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;
		crData *data = playerData->getDataClass();
		int itemid = 0;
		int equipmagic = 0;
		crVector4f colormode[5];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i =0; i<5;i++)
		{
			if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
			{
				if (qualityindex>=0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}
		if(canvas.valid())
		{
			if (!canvas->getVisiable())
			{
				void *param;
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				crData *canvasdata = m_this->getParentCanvas()->getDataClass();
				canvasdata->getParam(WCHDATA_JXJCurSelForgeLordEquipID,param);
				int equipid = *(int *)param;
				if(equipid > 0)
				{
					//equipmagic = (*equipid)[m_imageindex];
					crTableIO::StrVec record,lordrecord,recorde;
					ref_ptr<crTableIO>lordequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
					if (lordequiptab->queryOneRecord(2,crArgumentParser::appItoa(equipid),recorde) >= 0)
					{
						itemid = atoi(recorde[0].c_str());
					}
					ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
					int iconindex = itemtab->getTitleIndex("icon");
					//int info2index = itemtab->getTitleIndex("物品描述");
					int nameindex = itemtab->getTitleIndex("name");
					int typeindex = itemtab->getTitleIndex("类型");
					int colorindex = itemtab->getTitleIndex("品质");
					int needlvindex = itemtab->getTitleIndex("人物等级");
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
					{
						int colormodeid = atoi(record[colorindex].c_str());
						if(name)
						{
							name->setColor(colormode[colormodeid]);
							name->setString(record[nameindex]);
						}
						std::string file;
						if (itemlv)
						{
							if (rolelv < atoi(record[needlvindex].c_str()))
							{
								itemlv->setColor(m_colorred);
								if (itemequipon)
								{
									itemequipon->setColor(m_colorred);
								}
							}
							else
							{
								itemlv->setColor(colormode[0]);
								if (itemequipon)
								{
									itemequipon->setColor(colormode[1]);
								}
							}
							itemlv->setString(record[needlvindex]);
						}
						if (itemicon)
						{
							itemicon->setImageName(record[iconindex]);
						}
						if (itemequipon)
						{
							itemequipon->setString(std::string("未装备"));
						}
						if(info2)
						{
							ItemMap itemMap;
							rcfg::ConfigScript outscript(&itemMap);
							crVector2i item(itemid,0);
							playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
							info2->setHypertext(outscript);
						}


						///////////计算缩放
						float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
						info2->updateData();
						const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
						const crBoundingBox &bbox2 = info2->getBoundBox();
						float texheight = 0.0f;
						float needmove = 0.0f;
						if(bbox.valid())
						{
							texheight = bbox.yLength();
							texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
							if(texheight<boxYlength) texheight = boxYlength;
							float scale = texheight/boxYlength;

							float needmove ;
							const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
							crVector3 centerTansTar = info2->getBound().center();

							float tmpLength = bbox2.yLength();
							if(infodi)
							{
								float diYlength = infodi->getChild(0)->getBoundBox().yLength();
								//centerTansTar[1] += 0.5f * (tmpLength - texheight);
								centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
							}
							else
								centerTansTar[1] += 0.5f * (tmpLength - texheight);
							// 源中心点
							crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
							// 将源中心点平移到原点
							crMatrix put_mat = crMatrix::translate(centerTansSrc);
							crMatrix inv_put = crMatrix::inverse(put_mat);
							crMatrix rotmat;
							rotmat.postMult(inv_put);
							rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
							rotmat.postMult(crMatrix::translate(centerTansTar));
							if(infobk)infobk->setMatrix(rotmat);

							if(infodi)
							{
								float diYlength = infodi->getChild(0)->getBoundBox().yLength();
								//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
								//needmove = boxYlength - texheight;
								needmove = boxYlength - texheight + diYlength * 0.25f;
								infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
							}
						}
						else
						{
							infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
							if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
						}

						/////////////////////////////////////////
						const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
						const crBoundingBox &iconbox = m_this->getBoundBox();
						const crBoundingBox &tips = tipsCanvas->getBoundBox();
						float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
						float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
						crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
						/////////////////////////////////////////
						crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
						pos = pos * mat;
						pos2 = pos2 * mat;
						float posx2,posy2;
						const crBoundingBox &infos = infobk->getBoundBox();
						float posinfox = (infos.m_max[0]-infos.m_min[0]);
						float posinfoy = (infos.m_max[1]-infos.m_min[1]);
						if (pos2[0] >= 0)
						{
							posx2 = pos2[0] - posx;
						}
						else
						{
							posx2 = pos[0] + posx;
						}
						if (pos2[1] >= 0)
						{
							posy2 = pos[1] - posy;
						}
						else
						{
							posy2 = pos2[1] + 2.0f * posy - needmove;
						}
						if((abs(posy2)+posy)>1.0f)
						{
							if (posy2 >= 0)
							{
								posy2 -= posy;
							}
							else
							{
								posy2 += posy;
							}
						}
						crVector2 mouse(crVector2(posx2,posy2));
						/////////////////////////////////////////
						tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
						tipsCanvas->setCanFocus(false);
						infocanvas->setCanFocus(false);
						infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

						crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
						crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
						ifshowcanvas = true;
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		if (!ifshowcanvas)
		{
			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
			crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
		}
	}
}	

/////////////////////////////////////////
//
//crJXJCheckTaskResourceCountMethod
//
/////////////////////////////////////////
crJXJCheckTaskResourceCountMethod::crJXJCheckTaskResourceCountMethod():
	m_trueExecCount(),
	m_resourceCount(0),
	m_index()
{
}
crJXJCheckTaskResourceCountMethod::crJXJCheckTaskResourceCountMethod(const crJXJCheckTaskResourceCountMethod& handle):
	crMethod(handle),
	m_jueSeCanvas(handle.m_jueSeCanvas),
	m_resourceCount(handle.m_resourceCount),
	m_index(handle.m_index),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckTaskResourceCountMethod::inputParam(int i, void *param)
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
void crJXJCheckTaskResourceCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jueSeCanvas = str;
		break;
	case 1:
		m_resourceCount = (atoi(str.c_str()));
		break;
	case 2:
		m_index = (atoi(str.c_str())); //1：粮食，2：木材，3：铁矿，4：马匹
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckTaskResourceCountMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> jueSeCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jueSeCanvas);
	if(playerData && jueSeCanvas.valid())
	{
		void *param;
		crData *data = jueSeCanvas->getDataClass();
		if(data)
		{
			if(m_trueExecCount != 0)
			{
				if (m_index == 1)
				{
					data->getParam(WCHDATA_JXJProFoodProduct,param);
					int foodCount = *(int *)param;
					if(foodCount >= m_resourceCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
				if (m_index == 2)
				{
					data->getParam(WCHDATA_JXJProWoodProduct,param);
					int woodCount = *(int *)param;
					if(woodCount >= m_resourceCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
				if (m_index == 3)
				{
					data->getParam(WCHDATA_JXJProIronProduct,param);
					int ironCount = *(int *)param;
					if(ironCount >= m_resourceCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
				if (m_index == 4)
				{
					data->getParam(WCHDATA_JXJProHorseProduct,param);
					int horseCount = *(int *)param;
					if(horseCount >= m_resourceCount)
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUIGongFangRefineEquipUpdateMethod
//
/////////////////////////////////////////
crJXJUIGongFangRefineEquipUpdateMethod::crJXJUIGongFangRefineEquipUpdateMethod()
{
}

crJXJUIGongFangRefineEquipUpdateMethod::crJXJUIGongFangRefineEquipUpdateMethod(const crJXJUIGongFangRefineEquipUpdateMethod& handle):
	crMethod(handle),
	m_equipRaido(handle.m_equipRaido),
	m_equipListctrl(handle.m_equipListctrl),
	m_equipList_Left(handle.m_equipList_Left),
	m_equipList_Right(handle.m_equipList_Right),
	m_equipPageInput(handle.m_equipPageInput),
	m_pagesw(handle.m_pagesw),
	m_gongfangradio(handle.m_gongfangradio),
	m_defaulticon(handle.m_defaulticon),
	m_lordequipradio(handle.m_lordequipradio)
{	
}

void crJXJUIGongFangRefineEquipUpdateMethod::inputParam(int i, void *param)
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

void crJXJUIGongFangRefineEquipUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_equipRaido = str;
		break;
	case 1:
		m_equip[0] = str;
		break;
	case 2:
		m_equip[1] = str;
		break;
	case 3:
		m_equip[2] = str;
		break;
	case 4:
		m_equip[3] = str;
		break;
	case 5:
		m_equip[4] = str;
		break;
	case 6:
		m_defaultname[0] = str;
		break;
	case 7:
		m_defaultname[1] = str;
		break;
	case 8:
		m_defaultname[2] = str;
		break;
	case 9:
		m_defaultname[3] = str;
		break;
	case 10:
		m_defaultname[4] = str;
		break;
	case 11:
		m_equipListctrl = str;
		break;
	case 12:
		m_equipList_Left = str;
		break;
	case 13:
		m_equipList_Right = str;
		break;
	case 14:
		m_equipPageInput = str;
		break;
	case 15:
		m_pagesw = str;
		break;
	case 16:
		m_gongfangradio = str;
		break;
	case 17:
		m_defaulticon = str;
		break;
	case 18:
		m_lordequipradio = str;
		break;
	//case 19:
	//case 20:
	//case 21:
	//case 22:
	//	m_switchRadio[i-18] = str;
	//	break;
	}
}

void crJXJUIGongFangRefineEquipUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		bool ifshow = false;
		crRadioGroupWidgetNode *gongfangradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_gongfangradio));
		crListControlWidgetNode *equipListctrl = dynamic_cast<crListControlWidgetNode *>(m_this->getWidget(m_equipListctrl));
		crButtonWidgetNode *equipList_Left = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_equipList_Left));
		crButtonWidgetNode *equipList_Right = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_equipList_Right));
		crStaticTextWidgetNode *equipPageInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipPageInput));
		crRadioGroupWidgetNode *lordequipradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_lordequipradio));
		crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_pagesw));
		if(equipList_Left)equipList_Left->setVisiable(false);
		if(equipList_Right)equipList_Right->setVisiable(false);
		if(equipPageInput)equipPageInput->setVisiable(false);
		crRadioGroupWidgetNode *equipRaido = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_equipRaido));
		crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		crRole *mainrole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		crData *mainroledata = mainrole?mainrole->getDataClass():NULL;
		if (data &&equipRaido && equipListctrl && gongfangradio && lordequipradio && mainroledata)
		{
			void *param;
			int select = equipRaido->getSelect();
			mainroledata->getParam(WCHDATA_JXJSavedEquipVecs,param);
			unsigned char savedPage = *(unsigned char *)param;
			mainroledata->getParam(WCHDATA_JXJLordEquipVecs, param);
			std::vector<JXJLordEquipVec> openedEquipVec = *(std::vector<JXJLordEquipVec> *)param;
			JXJLordEquipVec equipInfo;
			int loadequipselect = lordequipradio->getSelect();
			equipInfo = openedEquipVec[loadequipselect];
			//crRadioWidgetNode* switchRadio[5];
			//for (int i = 0; i < 5; i++)
			//{
			//	switchRadio[i] =dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_switchRadio[i]));
			//	if (!switchRadio[i])
			//		continue;
			//	if (select == 0)
			//	{
			//		if (i < savedPage)
			//		{
			//			switchRadio[i]->setVisiable(true);
			//			if (switchRadio[i]->getSelect())
			//			{
			//				equipInfo = openedEquipVec[i];
			//			}
			//		}else
			//		{
			//			switchRadio[i]->setVisiable(false);
			//		}
			//	}else
			//	{
			//		switchRadio[i]->setVisiable(false);
			//	}
			//}
			if (select == 0)
			{
				if(pagesw)pagesw->setActiveSwitchSet(1);
				
				crImageBoxWidgetNode *equip[5];
				
				crTableIO::StrVec record;
				crRole *mainrole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();

				for (int i = 0; i < 5; i++)
				{
					equip[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_equip[i]));
				}
				

				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				int imageid = itemtab->getTitleIndex("icon");
				for (int i = 0; i<equipInfo.size();i++)
				{

					if(equip[i])
					{
						equip[i]->setVisiable(true);
						equip[i]->setImageName(m_defaultname[i]);
					}
					if(equipInfo[i].first>0)
					{
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equipInfo[i].first),record) >= 0)
						{
							if(equip[i] && record.size()>0)
								equip[i]->setImageName(record[imageid]);
						}
					}
				}

			}
			if (select == 1)
			{
				if(pagesw)pagesw->setActiveSwitchSet(0);
				void *param;
				int count = 0;
				int selectindex = 0;
				int itemid = 0;
				crBagItemData *bagitemdata = NULL;
				if(equipList_Left)equipList_Left->setVisiable(true);
				if(equipList_Right)equipList_Right->setVisiable(true);
				if(equipPageInput)equipPageInput->setVisiable(true);
				crData *Canvasdata = m_this->getDataClass();
				Canvasdata->getParam(WCHDATA_JXJGongFangEquipCurPage,param);
				int curpage = *(int *)param;
				Canvasdata->getParam(WCHDATA_JXJGongFangEquipTotalPage,param);
				int totalpage = *(int *)param;
				if (gongfangradio -> getSelect() == GongfangType_JingLian)
				{
					Canvasdata->getParam(WCHDATA_JXJRefineEquipSelectIndex,param);
					selectindex = *(int *)param;
					Canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
					bagitemdata = (crBagItemData *)param;
				}
				if (gongfangradio -> getSelect() == GongfangType_FuLian)
				{
					Canvasdata->getParam(WCHDATA_JXJAttachEquipSelectIndex,param);
					selectindex = *(int *)param;
					Canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
					bagitemdata = (crBagItemData *)param;
				}
				data ->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int totalcount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax,viplv).c_str()));
				int onepagecount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGongFangEquipPerPageCount,viplv).c_str()));
				crTableIO::StrVec itemrecord,recordl ,recordm;
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				int nameid = itemtab->getTitleIndex("name");
				int iconid = itemtab->getTitleIndex("icon");
				int equiptypeid = itemtab->getTitleIndex("类型");
				int row = -(curpage - 1) * onepagecount;
				crData *playerdata =  crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
				playerdata->excHandle(MAKEINT64(WCH_LockData,1));
				playerdata->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				playerdata->getParam(WCHDATA_JXJOpenBagNum,param);
				unsigned char openbagnum = *(unsigned char *)param;
				int i = 0;
				std::string title,title2,iconfile;
				equipListctrl->setListNodeCount(totalcount);
				equipListctrl->setStartYPos(curpage - 1);
				int newdata = 0;
				crListControlWidgetNode::ListNodeVec& listNodeVec = equipListctrl->getListNodeVec();
				for( JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr,++i,++newdata )
				{
					ifshow = true;
					if(i>=0 && i<openbagnum)
					{
						if((*itr)!=NULL && (*itr)->getItemID()!=0)
						{
							itemid = (*itr)->getItemID();
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord) >= 0)
							{
								int nowcount = (*itr)->getItemCount();
								if (bagitemdata && newdata == selectindex)
								{
									nowcount = nowcount - bagitemdata->getItemCount();
								}
								if (atoi(itemrecord[equiptypeid].c_str())!=IT_Equip)
								{
									ifshow = false;
								}
								if (nowcount == 0)
								{
									ifshow = false;
								}
								if (!ifshow)
								{
									i--;
									continue;
								}
								title = itemrecord[nameid];
								iconfile = itemrecord[iconid];
								//	listNodeVec[i]->setTitle(title);
								listNodeVec[i]->setFont("simhei.ttf");
								if (nowcount > 1)
								{
									title2 = crArgumentParser::appItoa(nowcount) + std::string(" ");;
								}
								else
								{
									title2.clear();
								}
								listNodeVec[i]->setTitle2(title2);
								listNodeVec[i]->setData(/*curpage*onepagecount+*/newdata);
								listNodeVec[i]->setImageName(iconfile);
								listNodeVec[i]->setVisiable(true);
								listNodeVec[i]->setEnable(true);
								count++;
							}
						}
						else
						{
							i--;
						}
					}
				}
				for(i;i<openbagnum;i++)
				{
					listNodeVec[i]->setTitle("");
					listNodeVec[i]->setTitle2("");
					listNodeVec[i]->setData(-1);
					listNodeVec[i]->setImageName(m_defaulticon);
					listNodeVec[i]->setVisiable(true);
					listNodeVec[i]->setEnable(false);
				}
				for(int j = openbagnum; j < totalcount; j++)
				{
					listNodeVec[j]->setData(j);
					listNodeVec[j]->setImageName("");
					//title = "暂未购买";
					listNodeVec[j]->setTitle("");
					//title = "点击购买";
					listNodeVec[j]->setTitle2("");
					listNodeVec[j]->setEnable(true);
					listNodeVec[j]->setVisiable(false);
				}
				playerdata->excHandle(MAKEINT64(WCH_LockData,0));
				totalpage = ceil((float)count/(float)onepagecount);
				if (totalpage == 0)
				{
					totalpage = 1;
				}
				Canvasdata->inputParam(WCHDATA_JXJGongFangEquipTotalPage,&totalpage);
				if(equipPageInput)equipPageInput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckJoinLegionTaskMethod
//
/////////////////////////////////////////
crJXJCheckJoinLegionTaskMethod::crJXJCheckJoinLegionTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckJoinLegionTaskMethod::crJXJCheckJoinLegionTaskMethod(const crJXJCheckJoinLegionTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckJoinLegionTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckJoinLegionTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckJoinLegionTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		if (m_trueExecCount != 0)
		{
			void *param;
			//crData *mainroledata = playerData->getMainRole()->getDataClass();
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJConsortiaID,param);
			int mylegionid = *( int *)param;
			if (mylegionid > 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJMouseOnRoleMethod
//
/////////////////////////////////////////
crJXJTerritoryMouseOnRoleMethod::crJXJTerritoryMouseOnRoleMethod()
{
}

crJXJTerritoryMouseOnRoleMethod::crJXJTerritoryMouseOnRoleMethod(const crJXJTerritoryMouseOnRoleMethod& handle):
	crMethod(handle)
{
}

void crJXJTerritoryMouseOnRoleMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			m_param = *(_crInt64*)param;
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crJXJTerritoryMouseOnRoleMethod::addParam(int i, const std::string& str)
{
}

void crJXJTerritoryMouseOnRoleMethod::operator()(crHandle &handle)
{
	void *param;
	crNode *fxNode;
	crDecalUpdateCallback *callback;
	crData *brainData = crBrain::getInstance()->getDataClass();
	if(m_this)
	{
		brainData->excHandle(MAKEINT64(WCH_LockData,1));
		brainData->getParam(WCHDATA_JXJTerritoryMouseOnRoleVec,param);
		ItemVec *mouseOnItemVec = (ItemVec *)param;
		ItemVec::iterator itr_item = mouseOnItemVec->begin();
		while (itr_item!=mouseOnItemVec->end())
		{
			if(itr_item->get() == m_this)
				break;
			itr_item++;
		}
		if(itr_item == mouseOnItemVec->end())
		{
			if(!mouseOnItemVec->empty())
			{
				itr_item = mouseOnItemVec->begin();
				while (itr_item!=mouseOnItemVec->end())
				{
					fxNode = itr_item->get()->findSceneFxNode("$MouseOnDecal");
					if(fxNode)
					{
						callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
						if(callback)
						{
							callback->setVisiable(false);
						}
					}
					itr_item++;
				}
				mouseOnItemVec->resize(0);
			}

			fxNode = m_this->findSceneFxNode("$MouseOnDecal");
			if(fxNode)
			{
				crData *itemData = m_this->getDataClass();
				callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
				if(callback && itemData)
				{
					callback->setVisiable(true);
					crVector3 pos = m_this->getPosition();
					callback->setPosition(crVector2(pos[0],pos[1]));
					
					itemData->getParam(WCHDATA_HalfPhysicsBox,param);
					crVector3 halfPhysicsBox = *(crVector3 *)param;
					crVector2 lengths(halfPhysicsBox[0],halfPhysicsBox[1]);
					lengths *= 2.0f;
					lengths[0] = lengths.length()+3.0f;
					lengths[1] = lengths[0];
					callback->setSize(lengths);
				}
			}
			mouseOnItemVec->push_back(m_this);
		}
		brainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJCheckRefineEquipTaskMethod
//
/////////////////////////////////////////
crJXJCheckRefineEquipTaskMethod::crJXJCheckRefineEquipTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckRefineEquipTaskMethod::crJXJCheckRefineEquipTaskMethod(const crJXJCheckRefineEquipTaskMethod& handle):
	crMethod(handle),
	m_gongFangCanvas(handle.m_gongFangCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckRefineEquipTaskMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJCheckRefineEquipTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongFangCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckRefineEquipTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode> gongFangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongFangCanvas);
	if (gongFangCanvas.valid())
	{
		if (m_trueExecCount != 0)
		{
			crData *canvasdata = gongFangCanvas->getDataClass();
			if (canvasdata)
			{
				void *param;
				canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
				int aftermagic = *(int *)param;
				if (aftermagic != 0)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFuLianEquipTaskMethod
//
/////////////////////////////////////////
crJXJCheckFuLianEquipTaskMethod::crJXJCheckFuLianEquipTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckFuLianEquipTaskMethod::crJXJCheckFuLianEquipTaskMethod(const crJXJCheckFuLianEquipTaskMethod& handle):
	crMethod(handle),
	m_gongFangCanvas(handle.m_gongFangCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckFuLianEquipTaskMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJCheckFuLianEquipTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongFangCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFuLianEquipTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode> gongFangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongFangCanvas);
	if (gongFangCanvas.valid())
	{
		if (m_trueExecCount != 0)
		{
			crData *canvasdata = gongFangCanvas->getDataClass();
			if (canvasdata)
			{
				void *param;
				canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
				crBagItemData *equipdata = (crBagItemData*)param;
				if (equipdata->getInlayID() != 0)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJGiveBuildingTime1Method
//
/////////////////////////////////////////
crJXJGiveBuildingTime1Method::crJXJGiveBuildingTime1Method()
{}
crJXJGiveBuildingTime1Method::crJXJGiveBuildingTime1Method(const crJXJGiveBuildingTime1Method& handle):
	crMethod(handle)
{
}
void crJXJGiveBuildingTime1Method::inputParam(int i, void *param)
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
void crJXJGiveBuildingTime1Method::addParam(int i, const std::string& str)
{
}

void crJXJGiveBuildingTime1Method::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		if (data)
		{
			data->getParam(WCHDATA_JXJBuildingTimes,param);
			crVector4i *buildingtime = (crVector4i *)param;
			if((*buildingtime)[1] < 0)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUShort((*buildingtime)[1]);
				CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGiveBuildingTime1,stream.get());
				net_conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvGiveBuildingTime1Method
//
/////////////////////////////////////////
crJXJRecvGiveBuildingTime1Method::crJXJRecvGiveBuildingTime1Method():
	m_netType(GameClient_Game)
{}
crJXJRecvGiveBuildingTime1Method::crJXJRecvGiveBuildingTime1Method(const crJXJRecvGiveBuildingTime1Method& handle):
	crMethod(handle)
{
}
void crJXJRecvGiveBuildingTime1Method::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
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

void crJXJRecvGiveBuildingTime1Method::addParam(int i, const std::string& str)
{
}

void crJXJRecvGiveBuildingTime1Method::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			void *param;
			int playerid = m_this -> getPlayerID();
			crData *data = m_this->getDataClass();
			ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (senderPlayerData.valid())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJBuildingTimes,param);
				crVector4i *buildingtime = (crVector4i *)param;
				(*buildingtime)[1] = 0;
				data->excHandle(MAKEINT64(WCH_LockData,0));
				crPlayerDataEventPacket packet;
				//ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//stream->createBuf(2);
				//stream->_writeUChar((*buildingtime)[1]);
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGiveBuildingTime1,NULL);
				netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet);
			}
		}
		if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this ->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJBuildingTimes,param);
			crVector4i *buildingtime = (crVector4i *)param;
			(*buildingtime)[1] = 0;
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewHandBattleTroopSelectCanvasMethod
//
/////////////////////////////////////////
crJXJNewHandBattleTroopSelectCanvasMethod::crJXJNewHandBattleTroopSelectCanvasMethod():
	m_instanceitemid(0)
{
}
crJXJNewHandBattleTroopSelectCanvasMethod::crJXJNewHandBattleTroopSelectCanvasMethod(const crJXJNewHandBattleTroopSelectCanvasMethod& handle):
	crMethod(handle),
	m_selectTroopCanvas(handle.m_selectTroopCanvas),
	m_instanceitemid(handle.m_instanceitemid)
{
}
void crJXJNewHandBattleTroopSelectCanvasMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJNewHandBattleTroopSelectCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_selectTroopCanvas = str;
		break;
	case 1:
		m_instanceitemid = atoi(str.c_str());;
		break;
	}
}
void crJXJNewHandBattleTroopSelectCanvasMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> selectTroopCanvas = crFilterRenderManager::getInstance()->findCanvas(m_selectTroopCanvas);
	if(!selectTroopCanvas.valid())
		return;
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	if (me && bindview && bindview->isInited())
	{
		ref_ptr<crSceneView> sceneView = bindview->getDefaultSceneHandler()->getSceneView();
		ref_ptr<crTableIO>dataRoleAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_RoleDataAttrTab);
		ref_ptr<crTableIO>dataNPCAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_NpcDataAttrTab);
		crInstanceItem *item;
		crMatrixTransform *bot;
		crVector3 window,pos;
		crData *itemData;
		void *param;
		crTableIO::StrVec record;
		crMyPlayerData::getInstance()->lockInRangeNpcMap();
		crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
		for( crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
			itr != npcMap.end();
			++itr )
		{
			item = itr->second.first.get();
			bot = dynamic_cast<crMatrixTransform *>(item->getRelNode());
			if(!bot || !bot->getVisiable())
				continue;
			pos = bot->getTrans();
			//if(dataNPCAttrTab->queryOneRecord(0,crArgumentParser::appItoa(item->getAbstractItemID()),record) >= 0)
			//	pos[2] += atof(record[ dataRoleAttrTab->getTitleIndex("血条高度")].c_str());
			//else
				pos[2]+=bot->getBoundBox().zLength();
			if(!crBrain::getInstance()->getCameraFrustum().contains(pos))
				continue;
			itemData = item->getDataClass();
			int instanceitemid = itr->first;
			if (instanceitemid == m_instanceitemid)
			{
				if (selectTroopCanvas.valid())
				{
					sceneView->projectObjectIntoWindow(pos,window);
					pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
					pos[2] = 0.0f;
					selectTroopCanvas->setPosition(pos);
					selectTroopCanvas->setVisiable(true);
				}
				if(itemData)
				{
					itemData->getParam(WCHDATA_ItemState,param);
					unsigned char itemstate = *(unsigned char *)param;
					if(itemstate == IS_Dead)
					{
						selectTroopCanvas->setVisiable(false);
						break;
					}
				}
			}
		}
		crMyPlayerData::getInstance()->unlockInRangeNpcMap();
	}

	//crRole *player;
	//crMatrixTransform *bot;
	//crCanvasNode *selectTroopCanvas;
	//CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	//crVector3 window,pos;
	//crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	//crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
	//if(itr->second.first != NULL)
	//{
	//	player = itr->second.first.get();
	//	bot = dynamic_cast<crMatrixTransform *>(player->getRelNode());
	//	selectTroopCanvas = NULL;
	//	selectTroopCanvas = crFilterRenderManager::getInstance()->findCanvas(m_selectTroopCanvas);
	//	if(!bot || !bot->getVisiable())
	//	pos = bot->getTrans();	
	//	if(selectTroopCanvas)
	//	{
	//		bindview->getDefaultSceneHandler()->getSceneView()->projectObjectIntoWindow(pos,window);
	//		pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
	//		selectTroopCanvas->setPosition(pos);
	//		selectTroopCanvas->setVisiable(true);
	//	}
	//}
}

/////////////////////////////////////////
//
//crJXJCheckNewHandBattleTroopSelectCanvasMethod
//
/////////////////////////////////////////
crJXJCheckNewHandBattleTroopSelectCanvasMethod::crJXJCheckNewHandBattleTroopSelectCanvasMethod()
{
}
crJXJCheckNewHandBattleTroopSelectCanvasMethod::crJXJCheckNewHandBattleTroopSelectCanvasMethod(const crJXJCheckNewHandBattleTroopSelectCanvasMethod& handle):
	crMethod(handle),
	m_selectTroopCanvas(handle.m_selectTroopCanvas)
{
}
void crJXJCheckNewHandBattleTroopSelectCanvasMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJCheckNewHandBattleTroopSelectCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_selectTroopCanvas = str;
		break;
	}
}
void crJXJCheckNewHandBattleTroopSelectCanvasMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		if (selectMyRoles->size() == 3)
		{
			check = true;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJXuMuDialogueProgressMethod
//
/////////////////////////////////////////
crJXJXuMuDialogueProgressMethod::crJXJXuMuDialogueProgressMethod():
	m_curtalkid(0),
	m_curtextid(0)
{}
crJXJXuMuDialogueProgressMethod::crJXJXuMuDialogueProgressMethod(const crJXJXuMuDialogueProgressMethod& handle):
	crMethod(handle),
	m_curtalkid(handle.m_curtalkid),
	m_curtextid(handle.m_curtextid),
	m_input(handle.m_input),
	m_duihuaswitch(handle.m_duihuaswitch)
{
}
void crJXJXuMuDialogueProgressMethod::inputParam(int i, void *param)
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
void crJXJXuMuDialogueProgressMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_duihuaswitch = str;
		break;
	}
}
void crJXJXuMuDialogueProgressMethod::operator()(crHandle &handle) 
{
	ref_ptr<crTableIO>talktab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTalkTab);
	crHypertextWidgetNode *input = dynamic_cast<crHypertextWidgetNode*>(m_this->getWidget(m_input));
	crData *data = m_this->getDataClass();
	if(data && talktab.valid())
	{
		void *param;
		data->getParam(WCHDATA_JXJTalkID,param);
		int talkid = *(int *)param;
		if(m_curtalkid != talkid)
		{
			m_curtalkid = talkid;
			m_curtextid = 0;
		}
		if(m_curtalkid>0)
		{
			crTableIO::StrVec record;
			std::vector<int> textidVec;
			if(talktab->queryOneRecord(0,crArgumentParser::appItoa(m_curtalkid),record) >= 0)
			{
				crArgumentParser::appAtoVec(record[1],textidVec);
				if(m_curtextid < textidVec.size())
				{
					int textid = textidVec[m_curtextid];
					////
					ref_ptr<crTableIO>dialoguetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJXuMuDialogueTab);
					crTableIO::StrVec dialoguevec;
					if (dialoguetab.valid())
					{
						int diaindex = dialoguetab->getTitleIndex("对白");
						int swindex = dialoguetab->getTitleIndex("sw");
						if(dialoguetab->queryOneRecord(0,crArgumentParser::appItoa(textid),dialoguevec) >= 0)
						{
							int dialoguevecsize = dialoguevec.size();
							int switchNum = atoi(dialoguevec[swindex].c_str());
							crMultiSwitch *duihuaswitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_duihuaswitch));
							if (duihuaswitch)
							{
								duihuaswitch->setActiveSwitchSet(switchNum);
							}
							if(dialoguevecsize>0)
							{
								std::string filename = dialoguevec[diaindex];
								if(input)
								{
									if(!filename.empty())
									{
										input->clearText();
										filename = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + filename;
										input->setHypertext(filename);
									}
									else
									{
										input->clearText();
									}
								}
							}
						}
					}
					m_curtextid++;
				}
				else
				{
					int endmsg = atoi(record[2].c_str());
					if(endmsg > 0)
						m_this->doEvent(MAKEINT64(WCH_JXJEndTalk,endmsg));
					crFilterRenderManager::getInstance()->closeCanvas(m_this);
					//crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
					//playerGameData->doEvent(WCH_JXJUIXuMuDialogue,MAKEINT64(talkid,NULL));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJXuMuDialogueInitMethod
//
/////////////////////////////////////////
crJXJXuMuDialogueInitMethod::crJXJXuMuDialogueInitMethod():
	m_talkid(0)
{}
crJXJXuMuDialogueInitMethod::crJXJXuMuDialogueInitMethod(const crJXJXuMuDialogueInitMethod& handle):
	crMethod(handle),
	m_xumucanvas(handle.m_xumucanvas),
	m_talkid(handle.m_talkid)
{
}
void crJXJXuMuDialogueInitMethod::inputParam(int i, void *param)
{

}

void crJXJXuMuDialogueInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_xumucanvas = str;
		break;
	case 1:
		m_talkid = atoi(str.c_str());
		break;

	}
}

void crJXJXuMuDialogueInitMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_xumucanvas);
	if(canvas.valid())
	{
		crData *data = canvas -> getDataClass();
		if (data)
		{
			data->inputParam(WCHDATA_JXJTalkID,&m_talkid);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIXuMuCanvasMethod
//
/////////////////////////////////////////
crJXJUIXuMuCanvasMethod::crJXJUIXuMuCanvasMethod():
	m_this(NULL),
	m_noticeindex(0)
{
}
crJXJUIXuMuCanvasMethod::crJXJUIXuMuCanvasMethod(const crJXJUIXuMuCanvasMethod& handle):
	crMethod(handle),
	m_noticeindex(handle.m_noticeindex),
	m_mousecanvas(handle.m_mousecanvas),
	m_jiangkacanvas(handle.m_jiangkacanvas)
{
}
void crJXJUIXuMuCanvasMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_noticeindex = LOINT64(param64);
		}
		else
		{
			m_noticeindex = 0;
		}
		break;
	}
}
void crJXJUIXuMuCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipcanvas = str;
		break;
	case 1:
		m_mousecanvas = str;
		break;
	case 2:
		m_jiangkacanvas = str;
		break;
	}
}
void crJXJUIXuMuCanvasMethod::operator()(crHandle &handle)
{	
	if (m_noticeindex > 0)
	{
		//ref_ptr<crCanvasNode> tipcanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipcanvas);
		//ref_ptr<crCanvasNode> mousecanvas = crFilterRenderManager::getInstance()->findCanvas(m_mousecanvas);
		//ref_ptr<crCanvasNode> jiangkacanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangkacanvas);
		switch(m_noticeindex)
		{
		case 1:
			crFilterRenderManager::getInstance()->showCanvas(m_tipcanvas,true);
			break;
		case 2:
			crFilterRenderManager::getInstance()->showCanvas(m_mousecanvas,true);
			break;
		case 3:
			crFilterRenderManager::getInstance()->showCanvas(m_jiangkacanvas,true);
			break;
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckNewHandBattleTroopMoveCanvasMethod
//
/////////////////////////////////////////
crJXJCheckNewHandBattleTroopMoveCanvasMethod::crJXJCheckNewHandBattleTroopMoveCanvasMethod()
{
}
crJXJCheckNewHandBattleTroopMoveCanvasMethod::crJXJCheckNewHandBattleTroopMoveCanvasMethod(const crJXJCheckNewHandBattleTroopMoveCanvasMethod& handle):
	crMethod(handle),
	m_moveTroopCanvas(handle.m_moveTroopCanvas)
{
}
void crJXJCheckNewHandBattleTroopMoveCanvasMethod::inputParam(int i, void *param)
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
		m_this = (crRole *)param;
		break;
	}
}
void crJXJCheckNewHandBattleTroopMoveCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_moveTroopCanvas = str;
		break;
	}
}
void crJXJCheckNewHandBattleTroopMoveCanvasMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crRole *myRore;
	crMyPlayerData *myPlayer = crMyPlayerData::getInstance();
	crMyPlayerData::MyRoleNpcMap myRoleMap = myPlayer->getMyRoleNpcMap();
	for (crMyPlayerData::MyRoleNpcMap::iterator itr_role = myRoleMap.begin();itr_role != myRoleMap.end();itr_role++)
	{
		myRore = itr_role->second.first.get();
		crData *roleData = myRore->getDataClass();
		roleData->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *(unsigned char *)param;
		if (itemstate == IS_Move)
		{
			check = true;
			break;
		}
	}
	handle.outputParam(0,&check);
} 

/////////////////////////////////////////
//
//crJXJCheckNewHandBattleUseJiangKaMethod
//
/////////////////////////////////////////
crJXJCheckNewHandBattleUseJiangKaMethod::crJXJCheckNewHandBattleUseJiangKaMethod():
	m_trueExecCount(1)
{
}
crJXJCheckNewHandBattleUseJiangKaMethod::crJXJCheckNewHandBattleUseJiangKaMethod(const crJXJCheckNewHandBattleUseJiangKaMethod& handle):
	crMethod(handle),
	m_jiangKaCanvas(handle.m_jiangKaCanvas),
	m_jiangKaPlay1(handle.m_jiangKaPlay1),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckNewHandBattleUseJiangKaMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJCheckNewHandBattleUseJiangKaMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaCanvas = str;
		break;
	case 1:
		m_jiangKaPlay1 = str;
		break;
	case 2:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckNewHandBattleUseJiangKaMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode> jiangKaCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaCanvas);
	if (jiangKaCanvas.valid())
	{
		crSequence *jiangKaPlay1 = dynamic_cast<crSequence *>(jiangKaCanvas->getChildNode(m_jiangKaPlay1));
		if (jiangKaPlay1->getMode() != crSequence::STOP)
		{
			if (m_trueExecCount != 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckGameIntroduceCanvasInfoMethod
//
/////////////////////////////////////////
crJXJCheckGameIntroduceCanvasInfoMethod::crJXJCheckGameIntroduceCanvasInfoMethod():
	m_delayTime(5),
	m_dt(0.0f)
{
}
crJXJCheckGameIntroduceCanvasInfoMethod::crJXJCheckGameIntroduceCanvasInfoMethod(const crJXJCheckGameIntroduceCanvasInfoMethod& handle):
	crMethod(handle),
	m_playdraw(handle.m_playdraw),
	m_delayTime(handle.m_delayTime),
	m_dt(0.0f),
	m_scale(handle.m_scale)

{
}
void crJXJCheckGameIntroduceCanvasInfoMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = *(float*)lparam;
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}
void crJXJCheckGameIntroduceCanvasInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_playdraw = str;
		break;
	case 1:
		m_delayTime = atof(str.c_str());
		break;
	case 2:
		m_scale = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJCheckGameIntroduceCanvasInfoMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		unsigned char scale = m_scale;//当scale值为0时，不对canvas进行缩放
		crMatrix rotmat = m_this->getMatrix();
		//计算缩放
		float height = crDisplaySettings::instance()->getViewHeight();
		float width = crDisplaySettings::instance()->getViewWidth();
		//像素转化成长度单位：米
		float factor = crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
		height *= factor;
		width *= factor;
		float boxXlength = m_this->getBoundBox().xLength();
		float boxYlength = m_this->getBoundBox().yLength();

		const crBoundingBox &bbox2 = m_this->getBoundBox();
		if(bbox2.valid())
		{
			if(boxXlength<height || scale == 0) height = boxXlength;
			float scale_x = height/boxXlength;

			if(boxYlength<width || scale == 0) width = boxYlength;
			float scale_y = width/boxYlength;

			// 源中心点
			crVector3 centerTansSrc = m_this->getBoundBox().center();
			// 将源中心点平移到原点
			crMatrix put_mat = crMatrix::translate(centerTansSrc);
			crMatrix inv_put = crMatrix::inverse(put_mat);
			rotmat.postMult(inv_put);
			rotmat.postMult(crMatrix::scale(scale_x,scale_y,1.0f));
			//rotmat.postMult(crMatrix::translate(centerTansSrc));
			if(m_this)m_this->setMatrix(rotmat);
		}
		else
		{
			m_this->setMatrix(crMatrix::scale(1.0f,1.0f,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
		}

		bool playend = false;
		crSequence *seq = dynamic_cast<crSequence *>(m_this->getChildNode(m_playdraw));
		if(m_delayTime>0.0f)
		{
			m_delayTime -= m_dt;
			if(m_delayTime<0.0f)
			{
				seq->setVisiable(true);
				seq->setVanishWhenStop(false);
				seq->startEffect();
			}
		}
		else
		{
			if(seq->getMode()==crSequence::STOP)
			{
				playend = true;
			}
		}
		handle.outputParam(0,&playend);
	}
}

/////////////////////////////////////////
//
//crJXJNewHandFocusMethod
//
/////////////////////////////////////////
crJXJNewHandFocusMethod::crJXJNewHandFocusMethod()
{}
crJXJNewHandFocusMethod::crJXJNewHandFocusMethod(const crJXJNewHandFocusMethod& handle):
	crMethod(handle)
{
}
void crJXJNewHandFocusMethod::inputParam(int i, void *param)
{
}

void crJXJNewHandFocusMethod::addParam(int i, const std::string& str)
{

}

void crJXJNewHandFocusMethod::operator()(crHandle &handle)
{
	crRole *role = crMyPlayerData::getInstance()->getLoadedRoleByIndex(0);
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(role && camera.valid())
	{
		crVector3 pos = role->getPosition();
		crNode *cameraNode = camera->getAttachedNode();
		cameraNode->doEvent(WCH_CameraFocus,MAKEINT64(&pos,NULL));
	}
}

/////////////////////////////////////////
//
//crJXJCheckChuZhengBtnMethod
//
/////////////////////////////////////////
crJXJCheckChuZhengBtnMethod::crJXJCheckChuZhengBtnMethod():
	m_this(NULL)
{
}
crJXJCheckChuZhengBtnMethod::crJXJCheckChuZhengBtnMethod(const crJXJCheckChuZhengBtnMethod& handle):
	crMethod(handle),
	m_chuzhengbtn(handle.m_chuzhengbtn),
	m_bianduibtn(handle.m_bianduibtn)
{
}
void crJXJCheckChuZhengBtnMethod::inputParam(int i, void *param)
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
void crJXJCheckChuZhengBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_chuzhengbtn = str;
		break;
	case 1:
		m_bianduibtn = str;
		break;
	default:
		break;
	}
}
void crJXJCheckChuZhengBtnMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		crButtonWidgetNode *chuzhengbtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_chuzhengbtn));
		crButtonWidgetNode *bianduibtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_bianduibtn));
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		roledata->getParam(WCHDATA_Level, param);
		unsigned char characterlevel = *(unsigned char *)param;
		if (chuzhengbtn)
		{
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFuBenOpenLv,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				chuzhengbtn->setEnable(true);
			}
			else
			{
				chuzhengbtn->setEnable(false);
			}
		}
		//if (bianduibtn)
		//{
		//	unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormationOpenLv,viplv).c_str()));
		//	if (characterlevel >= openlv)
		//	{
		//		bianduibtn->setEnable(true);
		//	}
		//	else
		//	{
		//		bianduibtn->setEnable(false);
		//	}
		//}
	}
}

/////////////////////////////////////////
//
//crJXJUIForcedOffLineExitMethod
//
/////////////////////////////////////////
crJXJUIForcedOffLineExitMethod::crJXJUIForcedOffLineExitMethod(){}
crJXJUIForcedOffLineExitMethod::crJXJUIForcedOffLineExitMethod(const crJXJUIForcedOffLineExitMethod& handle):
	crMethod(handle),
	m_msgCanvas(handle.m_msgCanvas),
	m_message(handle.m_message),
	m_messageIndex(handle.m_messageIndex)
{
}
void crJXJUIForcedOffLineExitMethod::inputParam(int i, void *param)
{
}

void crJXJUIForcedOffLineExitMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_msgCanvas = str;
		break;
	case 1:
		m_message = str;
		break;
	case 2:
		m_messageIndex = atoi(str.c_str());
		break;
	}
}

void crJXJUIForcedOffLineExitMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	crTableIO::StrVec record;
	std::string str;
	if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_messageIndex),record) >= 0)
	{
		std::string str = record[1];
		ref_ptr<crCanvasNode> msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgCanvas);
		if(msgCanvas.valid())
		{
			crStaticTextWidgetNode *messageInfo = dynamic_cast<crStaticTextWidgetNode *>(msgCanvas->getWidget(m_message));
			if(!messageInfo) return;
			messageInfo->setAlignment(CRText::crText::LEFT_BASE_LINE);
			messageInfo->setString(str);
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{
			if(IDOK == MessageBox(crStartHandler::getInstance()->getWindowHandle(),str.c_str(),"提示",MB_OK))
				exit(0);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIBattleSkillBuffTipMethod
//
/////////////////////////////////////////
crJXJUIBattleSkillBuffTipMethod::crJXJUIBattleSkillBuffTipMethod():
	m_index_1(0),
	m_index_2(0)
{
}
crJXJUIBattleSkillBuffTipMethod::crJXJUIBattleSkillBuffTipMethod(const crJXJUIBattleSkillBuffTipMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsName(handle.m_tipsName),
	m_tipsSkillBuffInfo(handle.m_tipsSkillBuffInfo),
	m_index_1(handle.m_index_1),
	m_index_2(handle.m_index_2)
{
}
void crJXJUIBattleSkillBuffTipMethod::inputParam(int i, void *param)
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
void crJXJUIBattleSkillBuffTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsName = str;
		break;
	case 2:
		m_tipsSkillBuffInfo = str;
		break;
	case 3:
		m_index_1 = atoi(str.c_str());
		break;
	case 4:
		m_index_2 = atoi(str.c_str());
		break;
	}
}

void crJXJUIBattleSkillBuffTipMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
	if (tipsCanvas.valid() && parentCanvas.valid())
	{
		crData *canvasData = parentCanvas->getDataClass();
		if (canvasData)
		{

			//crStaticTextWidgetNode *tipsName = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_tipsName));
			crHypertextWidgetNode *tipsSkillBuffInfo = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_tipsSkillBuffInfo));
			//if (tipsName)tipsName->clearString();
			if (tipsSkillBuffInfo)tipsSkillBuffInfo->clearText();

			crTableIO::StrVec record;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);

			ref_ptr<crTableIO>bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);
			crMyPlayerData *me = crMyPlayerData::getInstance();
			if (tipsSkillBuffInfo && me && bufferAttrTab.valid())
			{
				int infoid = bufferAttrTab->getTitleIndex("Buffer描述");
				std::string file;

				canvasData->excHandle(MAKEINT64(WCH_LockData, 1));
				canvasData->getParam(WCHDATA_JXJBufferTimerMap, param);
				BufferInfoMap * bufferInfoMap = (BufferInfoMap *)param;

				BufferInfoMap::iterator itr_buffer = bufferInfoMap[m_index_1].begin();

				int index = 0;
				for (; itr_buffer != bufferInfoMap[m_index_1].end(); itr_buffer++, index++)
				{
					if (index == m_index_2)
					{
						std::string extraValue = crArgumentParser::appItoa(itr_buffer->first) + std::string("|") \
							+ crArgumentParser::appItoa(itr_buffer->second.first);
						if (bufferAttrTab->queryOneRecord(0, extraValue, record) >= 0)
						{
							file = record[infoid];
							if (!file.empty())
							{
								cfg_script.Add("Hypertext");
								cfg_script.Push("Hypertext");

								cfg_script.Add("Content");
								cfg_script.Push("Content", 1);
								cfg_script.Add("Text", file);
								cfg_script.Pop();

								cfg_script.Pop();
								if (tipsSkillBuffInfo)tipsSkillBuffInfo->setHypertext(cfg_script);
							}
						}

						break;
					}
				}
				if (!file.empty())
				{
					const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
					const crBoundingBox &iconbox = m_this->getBoundBox();
					const crBoundingBox &tip = tipsCanvas->getBoundBox();
					float posx = (tip.m_max[0] - tip.m_min[0])*0.5;
					float posy = (tip.m_max[1] - tip.m_min[1])*0.5;
					crVector3 pos(iconbox.m_max[0], iconbox.m_max[1], 0.0f);
					pos = pos * mat;
					/////////////////////////////////////////
					crVector3 pos2(iconbox.m_min[0], iconbox.m_min[1], 0.0f);
					pos2 = pos2 * mat;
					float posx2, posy2;
					/////////////////////////////////////////
					if (pos2[0] >= 0)
					{
						posx2 = pos2[0] - posx;
					}
					else
					{
						posx2 = pos[0] + posx;
					}
					if (pos2[1] >= 0)
					{
						posy2 = pos[1] - posy;
					}
					else
					{
						posy2 = pos2[1] + posy;
					}
					crVector2 mouse(crVector2(posx2, posy2));
					/////////////////////////////////////////
					tipsCanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f)/* * mat*/);
					tipsCanvas->setCanFocus(false);
					
					crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(), true);
					
				}
				canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJAutoFuBenRaidoSelectMethod
//
/////////////////////////////////////////
crJXJAutoFuBenRaidoSelectMethod::crJXJAutoFuBenRaidoSelectMethod()
{
}
crJXJAutoFuBenRaidoSelectMethod::crJXJAutoFuBenRaidoSelectMethod(const crJXJAutoFuBenRaidoSelectMethod& handle):
	crMethod(handle),
	m_fubenCanvas(handle.m_fubenCanvas),
	m_zhengzhanfubenRaido(handle.m_zhengzhanfubenRaido),
	m_chapterRaido(handle.m_chapterRaido),
	m_fubenRaido(handle.m_fubenRaido),
	m_chapterscroll(handle.m_chapterscroll)
{
}
void crJXJAutoFuBenRaidoSelectMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJAutoFuBenRaidoSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenCanvas = str;
		break;
	case 1:
		m_zhengzhanfubenRaido = str;
		break;
	case 2:
		m_chapterRaido = str;
		break;
	case 3:
		m_fubenRaido = str;
		break;
	case 4:
		m_chapterscroll = str;
		break;
	}
}
void crJXJAutoFuBenRaidoSelectMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		void *param;
		crData *data = playerData ->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));

		crTableIO::DataVec recordcVec,recordfVec,recordffVec;
		ref_ptr<crTableIO>chapterTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChapterTab);
		int modeid = chapterTab->getTitleIndex("mode");
		int qianzhiid = chapterTab->getTitleIndex("前置id");
		chapterTab->queryRecords(modeid,crArgumentParser::appItoa(0),recordcVec);
		int chapterCountIndex = recordcVec.size() - 1;
		int startchapterid = atoi(recordcVec[0][0].c_str());

		ref_ptr<crTableIO>fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		int chapteridindex = fubenTab->getTitleIndex("chapterid");
		int fubenidindex = fubenTab->getTitleIndex("id");
		fubenTab->queryRecords(chapteridindex,recordcVec[chapterCountIndex][0],recordfVec);
		int fubenCount = recordfVec.size();
		int maxFubenid = fubenCount + atoi(recordcVec[chapterCountIndex][qianzhiid].c_str());

		fubenTab->queryRecords(chapteridindex,recordcVec[0][0],recordffVec);
		int startfubenid = atoi(recordffVec[0][0].c_str());

		int Count = 0;
		data->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
		for (FubenProgressMap::iterator itr = fubenProgress->begin();
			itr != fubenProgress->end();
			++itr)
		{
			if(itr->first >= startfubenid && itr->first <= maxFubenid)
			{
				Count++;
			}
		}
		int selfubenid = Count + startfubenid;
		if (selfubenid > maxFubenid)
		{
			selfubenid = maxFubenid;
		}
		crTableIO::StrVec record;
		if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(selfubenid),record) >= 0)
		{
			int curchapterid = atoi(record[chapteridindex].c_str());
			ref_ptr<crCanvasNode> fubenCanvas = crFilterRenderManager::getInstance()->findCanvas(m_fubenCanvas);
			if (fubenCanvas.valid())
			{
				crRadioGroupWidgetNode *zhengzhanfubenRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_zhengzhanfubenRaido));
				crRadioGroupWidgetNode *chapterRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_chapterRaido));
				crRadioGroupWidgetNode *fubenRaido = dynamic_cast<crRadioGroupWidgetNode *>(fubenCanvas->getWidget(m_fubenRaido));
				crScrollBarWidgetNode *chapterscroll = dynamic_cast<crScrollBarWidgetNode *>(fubenCanvas->getWidget(m_chapterscroll));
				crRadioGroupWidgetNode::RadioGroup&radioGroup = chapterRaido->getRadioGroup();
				int maxsize = radioGroup.size();
				if (zhengzhanfubenRaido)
				{
					zhengzhanfubenRaido->select(0);
					//zhengzhanfubenRaido->doEvent(WCH_UI_SelectChange);
					if (zhengzhanfubenRaido->getSelect() == 0)
					{
						if (chapterRaido)
						{
							if (curchapterid - startchapterid < maxsize)
							{
								chapterRaido->select(curchapterid - startchapterid);
								//chapterRaido->doEvent(WCH_UI_SelectChange);
								if (chapterRaido->getSelect() == curchapterid - startchapterid)
								{
									crTableIO::DataVec recordVec;
									fubenTab->queryRecords(chapteridindex,crArgumentParser::appItoa(curchapterid),recordVec);
									int beginfubenid = atoi(recordVec[0][0].c_str());
									int select = 0;
									if (fubenRaido)
									{
										for (int i = 0;i < recordVec.size();i++)
										{
											if (atoi(recordVec[i][0].c_str()) == selfubenid)
											{
												select = i;
												break;
											}
										}
										//int fubenCount = recordVec.size();
										//int select = selfubenid - beginfubenid;
										crRadioGroupWidgetNode::RadioGroup &fubenRadioGroup = fubenRaido->getRadioGroup();
										int raidoCount = fubenRadioGroup.size();
										void *param;
										crData *Canvasdata = fubenCanvas->getDataClass();
										Canvasdata->getParam(WCHDATA_JXJFubenCurPage,param);
										int *curpage = (int *)param;
										if (select < raidoCount)
										{
											*curpage = 1;
											fubenRaido->select(select);
										}
										else if (select < 2 * raidoCount)
										{
											*curpage = 2;
											fubenRaido->select(select - raidoCount);
										}
										else if (select < 3 * raidoCount)
										{
											*curpage = 3;
											fubenRaido->select(select - 2*raidoCount);
										}
										//fubenRaido->doEvent(WCH_UI_SelectChange);
									}
								}
							}
							else if(curchapterid - startchapterid >= maxsize)
							{
								chapterscroll->outerSetValue(curchapterid - startchapterid - maxsize + 1);
								chapterRaido->select(maxsize - 1);
								//chapterRaido->doEvent(WCH_UI_SelectChange);
								if (chapterRaido->getSelect() == maxsize - 1)
								{
									crTableIO::DataVec recordVec;
									fubenTab->queryRecords(chapteridindex,crArgumentParser::appItoa(curchapterid),recordVec);
									int beginfubenid = atoi(recordVec[0][0].c_str());
									int select = 0;
									if (fubenRaido)
									{
										for (int i = 0;i < recordVec.size();i++)
										{
											if (atoi(recordVec[i][0].c_str()) == selfubenid)
											{
												select = i;
												break;
											}
										}
										//int fubenCount = recordVec.size();
										//int select = selfubenid - beginfubenid;
										crRadioGroupWidgetNode::RadioGroup &fubenRadioGroup = fubenRaido->getRadioGroup();
										int raidoCount = fubenRadioGroup.size();
										void *param;
										crData *Canvasdata = fubenCanvas->getDataClass();
										Canvasdata->getParam(WCHDATA_JXJFubenCurPage,param);
										int *curpage = (int *)param;
										if (select < raidoCount)
										{
											fubenRaido->select(select);
										}
										else if (select < 2 * raidoCount)
										{
											*curpage = 2;
											fubenRaido->select(select - raidoCount);
										}
										else if (select < 3 * raidoCount)
										{
											*curpage = 3;
											fubenRaido->select(select - 2*raidoCount);
										}
										//fubenRaido->doEvent(WCH_UI_SelectChange);
									}
								}
							}
						}
					}
				}
				fubenCanvas->doEvent(WCH_UI_UpdateData);
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJGameDefaultChatModeSelectMethod
//
/////////////////////////////////////////
crJXJGameDefaultChatModeSelectMethod::crJXJGameDefaultChatModeSelectMethod()
{
}
crJXJGameDefaultChatModeSelectMethod::crJXJGameDefaultChatModeSelectMethod(const crJXJGameDefaultChatModeSelectMethod& handle):
	crMethod(handle),
	m_defaultmodeSW(handle.m_defaultmodeSW)
{
}
void crJXJGameDefaultChatModeSelectMethod::inputParam(int i, void *param)
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
void crJXJGameDefaultChatModeSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_defaultmodeSW = str;
		break;
	}
}

void crJXJGameDefaultChatModeSelectMethod::operator()(crHandle &handle)
{
	crMultiSwitch *defaultmodeSW = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_defaultmodeSW));
	if(defaultmodeSW)defaultmodeSW->setActiveSwitchSet(ChatInCountry - 1);
}

/////////////////////////////////////////
//
//crJXJAllSelectCanvasOpenInfoMethod
//
/////////////////////////////////////////
crJXJAllSelectCanvasOpenInfoMethod::crJXJAllSelectCanvasOpenInfoMethod()
{
}
crJXJAllSelectCanvasOpenInfoMethod::crJXJAllSelectCanvasOpenInfoMethod(const crJXJAllSelectCanvasOpenInfoMethod& handle):
	crMethod(handle)
{
	m_canvasVec = handle.m_canvasVec;
}
void crJXJAllSelectCanvasOpenInfoMethod::inputParam(int i, void *param)
{
}
void crJXJAllSelectCanvasOpenInfoMethod::addParam(int i, const std::string& str)
{
	m_canvasVec.push_back(str);
}
void crJXJAllSelectCanvasOpenInfoMethod::operator()(crHandle &handle)
{
	bool isopen = false;
	for (int i = 0; i < m_canvasVec.size();i++)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasVec[i]);
		if (canvas.valid() && canvas->getVisiable())
		{
			isopen = true;
			break;
		}
	}
	handle.outputParam(0,&isopen);
}

/////////////////////////////////////////
//
//crJXJUITheSmallMapPointInitMethod
//
/////////////////////////////////////////
crJXJUITheSmallMapPointInitMethod::crJXJUITheSmallMapPointInitMethod():
	m_this(NULL)
{
}
crJXJUITheSmallMapPointInitMethod::crJXJUITheSmallMapPointInitMethod(const crJXJUITheSmallMapPointInitMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_cityHallOrBossIcon(handle.m_cityHallOrBossIcon),
	m_battleMap(handle.m_battleMap),
	m_strRotMap(handle.m_strRotMap)
{
}
void crJXJUITheSmallMapPointInitMethod::inputParam(int i, void *param)
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
void crJXJUITheSmallMapPointInitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_cityHallOrBossIcon = str;
		break;
	case 2:
		m_battleMap = str;
		break;
	case 3:
		m_strRotMap = str;
		break;
	}
}
void crJXJUITheSmallMapPointInitMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		//crData *camadata = CRPhysics::crCameraManager::getInstance()->getMainCamera()->getAttachedNode()->getDataClass();
		if(!crRunGameHandle::getInstance()->isInGame())
			return;
		crImageBoxWidgetNode *battleMap = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_battleMap));
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();

		crWidgetNode *rotMapWidget = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_strRotMap));

		crVector2 mapWidgetSize;
		const crBoundingBox &bbox = battleMap->getBoundBox();
		mapWidgetSize.set(bbox.xLength(),bbox.yLength());
		crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);

		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;

		ref_ptr<crCanvasNode> Canvas = crFilterRenderManager::getInstance()->getMainCanvas();
		crWidgetNode *cityHallOrBossIcon = dynamic_cast<crWidgetNode *>(Canvas->getWidget(m_cityHallOrBossIcon));
		if(!cityHallOrBossIcon) return;
		crStateSet *state_set = dynamic_cast<crObject *>(( dynamic_cast<CRCore::crGroup *>(cityHallOrBossIcon))->getChild(0))->getDrawable(0)->getStateSet();
		if(!state_set) return;
		crTexture2D *texture_2d = dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,crStateAttribute::TEXTURE));
		crLoadManager::getInstance()->requestAddNode(rotMapWidget,cityHallOrBossIcon,false);
		int tableid = type == FT_Chengchi?WCHDATA_JXJChengchiMarkTab:WCHDATA_JXJFubenMarkTab;
		ref_ptr<crTableIO>chengchifubenMarkTab = crGlobalHandle::gData()->gGlobalTable(tableid);
		float px,py;
		if(chengchifubenMarkTab.valid())
		{
			data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short chengchifubenid = *(unsigned short *)param;
			if(chengchifubenid > 0)
			{
				crTableIO::StrVec recordid;
				int idxIndex = chengchifubenMarkTab->getTitleIndex("x");
				int idyIndex = chengchifubenMarkTab->getTitleIndex("y");
				int iconIndex = chengchifubenMarkTab->getTitleIndex("icon");
				if(chengchifubenMarkTab->queryOneRecord(0,crArgumentParser::appItoa(chengchifubenid),recordid) >= 0)
				{
					texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(recordid[iconIndex],texture_2d));

					if (atof(recordid[idxIndex].c_str()) != 0 || atof(recordid[idyIndex].c_str()) != 0)
					{
						px =atof(recordid[idxIndex].c_str())*crGlobalHandle::gData()->gUnitScale();
						py =atof(recordid[idyIndex].c_str())*crGlobalHandle::gData()->gUnitScale();

						px *= mapFactor[0];
						py *= mapFactor[1];

						ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
						if(camera.valid())
						{
							crMatrix viewerMat = camera->getCameraMatrix();
							viewerMat.setTrans(0.0f,0.0f,0.0f);
							crVector3 camDir = (-Y_AXIS * viewerMat).normalize();
							camDir[2] = 0.0f;
							camDir.normalize();
							CRCore::crMatrix newMat;
							if(camDir == crVector3(0.0f,0.0f,0.0f))
							{
								camDir[1] = -1.0f;
							}
							if(camDir[1] > 0.999)
							{
								newMat.makeRotate(-CRCore::Y_AXIS,camDir);
								CRCore::crVector3d ep,center,up;
								newMat.getLookAt(ep,center,up);
								if(center[2]>0.0f)
								{
									center = -center;
								}
								newMat.makeLookAt(ep,center,up);
							}
							else
								newMat.makeRotate(-CRCore::Y_AXIS,camDir);

							//crQuat quat = rotMapWidget->getMatrix().getRotate();
							//crMatrixf mtrx = crMatrix::rotate(quat.inverse());
							crMatrixf invertRot = crMatrix::inverse(newMat);
							cityHallOrBossIcon->setMatrix(invScaleMat * invertRot * scaleMat * crMatrix::translate(battleMap->getBound().center()+crVector3(px,py,0.0f)));
							cityHallOrBossIcon->setVisiable(true);
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandTaskBuildingLocationMethod
//
/////////////////////////////////////////
crJXJUINewHandTaskBuildingLocationMethod::crJXJUINewHandTaskBuildingLocationMethod()
{}
crJXJUINewHandTaskBuildingLocationMethod::crJXJUINewHandTaskBuildingLocationMethod(const crJXJUINewHandTaskBuildingLocationMethod& handle):
	crMethod(handle),
	m_itemid(handle.m_itemid),
	m_canvas(handle.m_canvas),
	m_renwuCanvas(handle.m_renwuCanvas)
{
}
void crJXJUINewHandTaskBuildingLocationMethod::inputParam(int i, void *param)
{
}
void crJXJUINewHandTaskBuildingLocationMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_itemid = atoi(str.c_str());
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_renwuCanvas = str;
		break;
	}
}

void crJXJUINewHandTaskBuildingLocationMethod::operator()(crHandle &handle)
{
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	if (playerData && bindview && bindview->isInited())
	{
		crInstanceItem *item  = crMyPlayerData::getInstance()->getScene()->findSceneItem(m_itemid);//找到府衙模型位置

		ref_ptr<crCanvasNode> renwuCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
		//ref_ptr<crCanvasNode> renwuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renwuCanvas);
		if(renwuCanvas.valid())
		{
			crData *data = renwuCanvas->getDataClass();
			if(data)
			{
				data->inputParam(WCHDATA_JXJTargetBuildingID,&m_itemid);
			}
		}
		
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();

		if(canvas.valid() && item)
		{
			//crStaticTextWidgetNode *input = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
			//if (input)
			//{
			//	input->setVisiable(false);
			//}
			crVector3 pos = item->getPosition();
			pos[2]+=item->getRelNode()->getBoundBox().zLength();
			//if (m_buildingnameposz == 0)
			//{
			//	pos[2]+=m_this->getRelNode()->getBoundBox().zLength();
			//}
			//else
			//{
			//	pos[2] = m_buildingnameposz;
			//}
			if(crBrain::getInstance()->getCameraFrustum().contains(pos))
			{
				crVector3 window;
				if (bindview && bindview->isInited())
				{
					bindview->getDefaultSceneHandler()->getSceneView()->projectObjectIntoWindow(pos,window);
					pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
					pos[2] = 0.0f;
					canvas->setPosition(pos);
					//headCanvas->setVisiable(true);
					if(!canvas->getVisiable()) 
					{
						//canvas->setShowTop(false);
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
					}
				}
			}
			else
			{
				if(canvas->getVisiable()) 
				{
					crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckTroopsTechTaskInfoMethod
//
/////////////////////////////////////////
crJXJCheckTroopsTechTaskInfoMethod::crJXJCheckTroopsTechTaskInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckTroopsTechTaskInfoMethod::crJXJCheckTroopsTechTaskInfoMethod(const crJXJCheckTroopsTechTaskInfoMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas),
	m_armytyperadio(handle.m_armytyperadio),
	m_troopstechid(handle.m_troopstechid),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckTroopsTechTaskInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckTroopsTechTaskInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_armytyperadio = str;
		break;
	case 2:
		m_troopstechid = atoi(str.c_str());
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}
void crJXJCheckTroopsTechTaskInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		unsigned short troopstechid = 0;
		unsigned char troopstechlv = 0;
		int armytype = 0;
		ref_ptr<crCanvasNode> gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		if(gongfangcanvas.valid())
		{
			crRadioGroupWidgetNode *armytyperadio = dynamic_cast<crRadioGroupWidgetNode*>(gongfangcanvas->getWidget(m_armytyperadio));
			armytype = armytyperadio->getSelect();
			ref_ptr<crTableIO>TroopsTechTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechTab);
			crTableIO::DataVec record;
			int armytypeIndex = TroopsTechTab->getTitleIndex("兵种");
			crData *data = myPlayer->getDataClass();

			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJTroopsTechMap,param);
			TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
			TroopsTechMap::iterator troopsitr;
			TroopsTechTab->queryRecords(armytypeIndex,crArgumentParser::appItoa(armytype),record);
			if (m_trueExecCount != 0)
			{
				for (crTableIO::DataVec::iterator itr = record.begin();
					itr != record.end();
					++itr)
				{
					troopstechid = atoi((*itr)[0].c_str());
					if (troopstechid == m_troopstechid)
					{
						troopsitr = troopstechmap -> find(troopstechid);
						if (troopsitr != troopstechmap->end())
						{
							troopstechlv = troopsitr->second;
							if (troopstechlv >= 1)
							{
								check = true;
								if(m_trueExecCount>0)
									m_trueExecCount--;
								break;
							}
						}
					}				
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJFormationTaskPositionMethod
//
/////////////////////////////////////////
crJXJFormationTaskPositionMethod::crJXJFormationTaskPositionMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_abstractid(0)
{
}
crJXJFormationTaskPositionMethod::crJXJFormationTaskPositionMethod(const crJXJFormationTaskPositionMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_formationList(handle.m_formationList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_abstractid(handle.m_abstractid)
{
}
void crJXJFormationTaskPositionMethod::inputParam(int i, void *param)
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
void crJXJFormationTaskPositionMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_formationList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 4:
		m_taskid = (atoi(str.c_str()));
		break;
	case 5:
		m_abstractid = (atoi(str.c_str()));
		break;
	}
}
void crJXJFormationTaskPositionMethod::operator()(crHandle &handle)
{
	bool check = false;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
	if(!formationCanvas.valid())
		return;
	crListControlWidgetNode *formationList = dynamic_cast<crListControlWidgetNode *>(formationCanvas->getWidget(m_formationList));
	crData *data = playerData->getDataClass(); 
	CRCore::ScopedLock<crData> lock(*data);
	crListControlWidgetNode::ListNodeVec& listNodeVec = formationList->getListNodeVec();
	int i = 0;
	data->getParam(WCHDATA_JXJTroopsMap, param);
	TroopsMap *troops =	(TroopsMap *)param;
	for( TroopsMap::iterator itr = troops->begin();
		itr != troops->end();
		++itr,i++)
	{
		if(itr->first == m_abstractid)
		{
			formationList->select(i);
			crListControlNode *selectNode = formationList->getSelectNode();
			if(!selectNode)return;
			int selectid = selectNode->getData();
			if (listNodeVec[i]!=NULL)
			{
				ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
				ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
				if(m_trueExecCount != 0)
				{
					if(rectCanvas.valid() && renwucanvas.valid())
					{
						crData *renwudata = renwucanvas->getDataClass();
						if(!renwudata)return;
						renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
						renwudata->inputParam(WCHDATA_JXJNewHandTroopPositionID,&i);
						crVector4f rect = selectNode->getRect();
						CRCore::crMatrixTransform *m_mvpwNode = formationList->getMvpwNode();
						const crMatrix &mat = m_mvpwNode->getMatrix() * formationList->getParentCanvas()->getMatrix();
						crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
						crVector3 ipos = pos * mat;
						rectCanvas->setPosition(ipos);
						if(m_trueExecCount>0)
							m_trueExecCount--;
						check = true;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckLingZhuLvInfoMethod
//
/////////////////////////////////////////
crJXJCheckLingZhuLvInfoMethod::crJXJCheckLingZhuLvInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckLingZhuLvInfoMethod::crJXJCheckLingZhuLvInfoMethod(const crJXJCheckLingZhuLvInfoMethod& handle):
	crMethod(handle),
	m_mylevel(handle.m_mylevel),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckLingZhuLvInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckLingZhuLvInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_mylevel = (unsigned char)atoi(str.c_str());;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckLingZhuLvInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getMainRole())
	{
		void *param;
		crRole *mainRole = playerData->getMainRole();
		crData *mainRoleData = mainRole->getDataClass();
		mainRoleData->getParam(WCHDATA_Level,param);
		unsigned char level = *(unsigned char *)param;
		if(m_trueExecCount != 0)
		{
			if(level >= m_mylevel)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckTriggerGuideZhuangBeiDZInfoMethod
//
/////////////////////////////////////////
crJXJCheckTriggerGuideZhuangBeiDZInfoMethod::crJXJCheckTriggerGuideZhuangBeiDZInfoMethod():
	m_trueExecCount(),
	m_index()
{
}
crJXJCheckTriggerGuideZhuangBeiDZInfoMethod::crJXJCheckTriggerGuideZhuangBeiDZInfoMethod(const crJXJCheckTriggerGuideZhuangBeiDZInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_gongFangCanvas(handle.m_gongFangCanvas),
	m_radioGroup(handle.m_radioGroup),
	m_index(handle.m_index)
{
}
void crJXJCheckTriggerGuideZhuangBeiDZInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckTriggerGuideZhuangBeiDZInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongFangCanvas = str;
		break;
	case 1:
		m_radioGroup = str;
		break;
	case 2:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 3:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJCheckTriggerGuideZhuangBeiDZInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getMainRole())
	{
		ref_ptr<crCanvasNode> gongFangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongFangCanvas);
		if (gongFangCanvas.valid())
		{
			crRadioGroupWidgetNode *radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(gongFangCanvas->getWidget(m_radioGroup));
			if(!radioGroup) return;
			int select = radioGroup->getSelect();
			if(m_trueExecCount != 0)
			{
				if(select == m_index)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFirstGetChengKaInfoMethod
//
/////////////////////////////////////////
crJXJCheckFirstGetChengKaInfoMethod::crJXJCheckFirstGetChengKaInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckFirstGetChengKaInfoMethod::crJXJCheckFirstGetChengKaInfoMethod(const crJXJCheckFirstGetChengKaInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas)
{
}
void crJXJCheckFirstGetChengKaInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFirstGetChengKaInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFirstGetChengKaInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getMainRole())
	{
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if (jiangKaBagCanvas.valid() && jiangKaBagCanvas->getDataClass())
		{
			void *param;
			crData *canvasdata = jiangKaBagCanvas->getDataClass();
			ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int colorindex = itemTab->getTitleIndex("品质");
			canvasdata->getParam(WCHDATA_JXJForJKVec, param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			if (m_trueExecCount != 0)
			{
				for (JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr)
				{
					if((*itr)!= NULL)
					{
						int jiangkaid = (*itr)->getItemID();
						if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(jiangkaid),record) >= 0)
						{
							int colorid = atoi(record[colorindex].c_str());
							if(colorid == 4)
							{
								check = true;
								if(m_trueExecCount>0)
									m_trueExecCount--;
								break;
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFirstFuBenWinInfoMethod
//
/////////////////////////////////////////
crJXJCheckFirstFuBenWinInfoMethod::crJXJCheckFirstFuBenWinInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckFirstFuBenWinInfoMethod::crJXJCheckFirstFuBenWinInfoMethod(const crJXJCheckFirstFuBenWinInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_fubenid(handle.m_fubenid)
{
}
void crJXJCheckFirstFuBenWinInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFirstFuBenWinInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenid = (unsigned short)(atoi(str.c_str()));;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFirstFuBenWinInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
		if (m_trueExecCount != 0)
		{
			if (fubenProgress)
			{
				FubenProgressMap::iterator itr = fubenProgress->find((unsigned short)m_fubenid);
				if (itr != fubenProgress->end() && itr->second->getComplete() > 0)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}
			}
		}	
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFirstFuBenOpenBoxInfoMethod
//
/////////////////////////////////////////
crJXJCheckFirstFuBenOpenBoxInfoMethod::crJXJCheckFirstFuBenOpenBoxInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckFirstFuBenOpenBoxInfoMethod::crJXJCheckFirstFuBenOpenBoxInfoMethod(const crJXJCheckFirstFuBenOpenBoxInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount),
	m_canvas(handle.m_canvas),
	m_fubenid(handle.m_fubenid)
{
}
void crJXJCheckFirstFuBenOpenBoxInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFirstFuBenOpenBoxInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenid = (unsigned short)(atoi(str.c_str()));;
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckFirstFuBenOpenBoxInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(playerData && playerData->getDataClass())
	{
		void *param;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		if (m_trueExecCount != 0)
		{
			if(type == FT_Fuben)
			{
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short fubenid = *(unsigned short *)param;
				if(fubenid == m_fubenid && canvas.valid())
				{
					if (canvas->getVisiable())
					{
						check = true;
						if(m_trueExecCount>0)
							m_trueExecCount--;
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJButtonRoleToZhenTingMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJButtonRoleToZhenTingMethod::crJXJButtonRoleToZhenTingMethod()
{

}
crJXJButtonRoleToZhenTingMethod::crJXJButtonRoleToZhenTingMethod(const crJXJButtonRoleToZhenTingMethod &handle)
{

}
void crJXJButtonRoleToZhenTingMethod::inputParam(int i, void *param)
{

}
void crJXJButtonRoleToZhenTingMethod::addParam(int i, const std::string& str)
{

}
void crJXJButtonRoleToZhenTingMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		float px = 0;
		float py = 0;
		if(type == FT_Chengchi)
		{
			data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short chengchiid = *(unsigned short *)param;
			if(chengchiid > 0)
			{
				ref_ptr<crTableIO>chengchiMarkTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengchiMarkTab);
				if (chengchiMarkTab.valid())
				{
					crTableIO::StrVec recordc;
					int idxIndex = chengchiMarkTab->getTitleIndex("x");
					int idyIndex = chengchiMarkTab->getTitleIndex("y");
					if(chengchiMarkTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),recordc) >= 0)
					{
						px =atof(recordc[idxIndex].c_str())*crGlobalHandle::gData()->gUnitScale();
						py =atof(recordc[idyIndex].c_str())*crGlobalHandle::gData()->gUnitScale();
					}
				}
			}
		}
		if(type == FT_Fuben)
		{
			data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short fubenid = *(unsigned short *)param;
			if(fubenid > 0)
			{
				ref_ptr<crTableIO>fubenMarkTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenMarkTab);
				if (fubenMarkTab.valid())
				{
					crTableIO::StrVec recordf;
					int idxIndex = fubenMarkTab->getTitleIndex("x");
					int idyIndex = fubenMarkTab->getTitleIndex("y");
					if(fubenMarkTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),recordf) >= 0)
					{
						px =atof(recordf[idxIndex].c_str())*crGlobalHandle::gData()->gUnitScale();
						py =atof(recordf[idyIndex].c_str())*crGlobalHandle::gData()->gUnitScale();
					}
				}
			}
		}
		crVector3 m_targetPosition(px,py,0.0f);
		crMyPlayerData::getInstance()->lockMyRoleNpcMap();
		crRole *role;
		crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
			itr != myRoles.end();
			++itr )
		{
			role = itr->second.first.get();
			crData *roledata = role->getDataClass();
			unsigned char itemstate = IS_AttackToPos;
			unsigned char targettype = Target_Coord;
			roledata->inputParam(WCHDATA_ItemState,&itemstate);
			roledata->inputParam(WCHDATA_TargetType,&targettype);
			roledata->inputParam(WCHDATA_TargetPos,&m_targetPosition);
			roledata->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
			crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(role);
		}
		crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandBuildingClickMethod
//
/////////////////////////////////////////
crJXJUINewHandBuildingClickMethod::crJXJUINewHandBuildingClickMethod()
{
}
crJXJUINewHandBuildingClickMethod::crJXJUINewHandBuildingClickMethod(const crJXJUINewHandBuildingClickMethod& handle):
	crMethod(handle),
	m_renwuCanvas(handle.m_renwuCanvas)
{
}
void crJXJUINewHandBuildingClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandBuildingClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_renwuCanvas = str;
		break;
	}
}
void crJXJUINewHandBuildingClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && m_this)
	{
		void *param;
		ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renwuCanvas);
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		if(renWuCanvas.valid())
		{
			crData *data = renWuCanvas->getDataClass();
			if (data)
			{
				data->getParam(WCHDATA_JXJTargetBuildingID,param);
				int targetbuildingid = *(int *)param;
				CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
				if(scene)
				{
					crInstanceItem *item = scene->findSceneItem(targetbuildingid);
					if(item)
					{
						item->doEvent(WCH_Touch);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandBuildingClickMethod	
//
/////////////////////////////////////////
crJXJUINewHandFormationTroopsClickMethod::crJXJUINewHandFormationTroopsClickMethod()
{
}
crJXJUINewHandFormationTroopsClickMethod::crJXJUINewHandFormationTroopsClickMethod(const crJXJUINewHandFormationTroopsClickMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_formationList(handle.m_formationList),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJUINewHandFormationTroopsClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandFormationTroopsClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_formationList = str;
		break;
	case 2:
		m_renWuCanvas = str;
		break;
	}
}
void crJXJUINewHandFormationTroopsClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		int i = 0;
		ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
		if (renWuCanvas.valid())
		{
			void *param;
			crData *renwudata = renWuCanvas->getDataClass();
			if (renwudata)
			{
				renwudata->getParam(WCHDATA_JXJNewHandTroopPositionID,param);
				i = *(int *)param;
			}
		}
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
		if(!formationCanvas.valid())
			return;
		crListControlWidgetNode *formationList = dynamic_cast<crListControlWidgetNode *>(formationCanvas->getWidget(m_formationList));
		if(formationList)
		{
			formationList->select(i);
			formationList->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
			//crFilterRenderManager::getInstance()->showCanvas(formationCanvas.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandKnapsackItemClickMethod	
//
/////////////////////////////////////////
crJXJUINewHandKnapsackItemClickMethod::crJXJUINewHandKnapsackItemClickMethod()
{
}
crJXJUINewHandKnapsackItemClickMethod::crJXJUINewHandKnapsackItemClickMethod(const crJXJUINewHandKnapsackItemClickMethod& handle):
	crMethod(handle),
	m_knapsackCanvas(handle.m_knapsackCanvas),
	m_knapsackList(handle.m_knapsackList)
{
}
void crJXJUINewHandKnapsackItemClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandKnapsackItemClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_knapsackCanvas = str;
		break;
	case 1:
		m_knapsackList = str;
		break;
	}
}
void crJXJUINewHandKnapsackItemClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> knapsackCanvas = crFilterRenderManager::getInstance()->findCanvas(m_knapsackCanvas);
		if(!knapsackCanvas.valid())
			return;
		crListControlWidgetNode *knapsackList = dynamic_cast<crListControlWidgetNode *>(knapsackCanvas->getWidget(m_knapsackList));
		if(knapsackList)
		{
			//crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
			knapsackList->select(0);
			knapsackList->doEvent(MAKEINT32(crGUIEventAdapter::RIGHT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
			//crFilterRenderManager::getInstance()->showCanvas(formationCanvas.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandFormationJiangKaClickMethod
//
/////////////////////////////////////////
crJXJUINewHandFormationJiangKaClickMethod::crJXJUINewHandFormationJiangKaClickMethod()
{
}
crJXJUINewHandFormationJiangKaClickMethod::crJXJUINewHandFormationJiangKaClickMethod(const crJXJUINewHandFormationJiangKaClickMethod& handle):
	crMethod(handle),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas),
	m_jiangKaBagList(handle.m_jiangKaBagList),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJUINewHandFormationJiangKaClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandFormationJiangKaClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_jiangKaBagList = str;
		break;
	case 2:
		m_renWuCanvas = str;
		break;
	}
}
void crJXJUINewHandFormationJiangKaClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		int i = 0;
		//ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
		//if (renWuCanvas.valid())
		//{
		//	void *param;
		//	crData *renwudata = renWuCanvas->getDataClass();
		//	if (renwudata)
		//	{
		//		renwudata->getParam(WCHDATA_JXJNewHandUpgradeCardID,param);
		//		i = *(int *)param;
		//	}
		//}
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if(!jiangKaBagCanvas.valid())
			return;
		crListControlWidgetNode *jiangKaBagList = dynamic_cast<crListControlWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaBagList));
		if(jiangKaBagList)
		{
			jiangKaBagList->select(i);
			jiangKaBagList->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::DOUBLECLICK));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandWeiRenJiangKaClickMethod
//
/////////////////////////////////////////
crJXJUINewHandWeiRenJiangKaClickMethod::crJXJUINewHandWeiRenJiangKaClickMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_jiangKaid(0)
{
}
crJXJUINewHandWeiRenJiangKaClickMethod::crJXJUINewHandWeiRenJiangKaClickMethod(const crJXJUINewHandWeiRenJiangKaClickMethod& handle):
	crMethod(handle),
	m_weiRenBagCanvas(handle.m_weiRenBagCanvas),
	m_weiRenBagList(handle.m_weiRenBagList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_jiangKaid(handle.m_jiangKaid)
{
}
void crJXJUINewHandWeiRenJiangKaClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandWeiRenJiangKaClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_weiRenBagCanvas = str;
		break;
	case 1:
		m_weiRenBagList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_renWuCanvas = str;
		break;
	case 4:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 5:
		m_taskid = (atoi(str.c_str()));
		break;
	case 6:
		m_jiangKaid = (atoi(str.c_str()));
		break;
	}
}
void crJXJUINewHandWeiRenJiangKaClickMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> weiRenBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_weiRenBagCanvas);
		if(!weiRenBagCanvas.valid())
			return;
		crListControlWidgetNode *weiRenBagList = dynamic_cast<crListControlWidgetNode *>(weiRenBagCanvas->getWidget(m_weiRenBagList));
		crData *data = playerData->getDataClass(); 
		CRCore::ScopedLock<crData> lock(*data);
		crListControlWidgetNode::ListNodeVec& listNodeVec = weiRenBagList->getListNodeVec();
		int i = 0;
		void *param;
		data->getParam(WCHDATA_JXJJiangkaBagVec, param);
		JXJItemBagVec *itembagvec =	(JXJItemBagVec *)param;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr,i++)
		{
			if((*itr) != NULL && (*itr)->getItemID() == m_jiangKaid)
			{
				weiRenBagList->select(i);
				crListControlNode *selectNode = weiRenBagList->getSelectNode();
				if(!selectNode)return;
				int selectid = selectNode->getData();
				if (listNodeVec[i]!=NULL)
				{
					ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
					ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
					if(m_trueExecCount != 0)
					{
						if(rectCanvas.valid() && renwucanvas.valid())
						{
							crData *renwudata = renwucanvas->getDataClass();
							if(!renwudata)return;
							renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
							crVector4f rect = selectNode->getRect();
							CRCore::crMatrixTransform *m_mvpwNode = weiRenBagList->getMvpwNode();
							const crMatrix &mat = m_mvpwNode->getMatrix() * weiRenBagList->getParentCanvas()->getMatrix();
							crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
							crVector3 ipos = pos * mat;
							rectCanvas->setPosition(ipos);
							if(m_trueExecCount>0)
								m_trueExecCount--;
							check = true;
							break;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUINewHandUpgradeJiangKaClickMethod
//
/////////////////////////////////////////
crJXJUINewHandUpgradeJiangKaClickMethod::crJXJUINewHandUpgradeJiangKaClickMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_jiangKaid(0)
{
}
crJXJUINewHandUpgradeJiangKaClickMethod::crJXJUINewHandUpgradeJiangKaClickMethod(const crJXJUINewHandUpgradeJiangKaClickMethod& handle):
	crMethod(handle),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas),
	m_jiangKaBagList(handle.m_jiangKaBagList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_jiangKaid(handle.m_jiangKaid)
{
}
void crJXJUINewHandUpgradeJiangKaClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandUpgradeJiangKaClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_jiangKaBagList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_renWuCanvas = str;
		break;
	case 4:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 5:
		m_taskid = (atoi(str.c_str()));
		break;
	case 6:
		m_jiangKaid = (atoi(str.c_str()));
		break;
	}
}
void crJXJUINewHandUpgradeJiangKaClickMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if(!jiangKaBagCanvas.valid())
			return;
		crListControlWidgetNode *jiangKaBagList = dynamic_cast<crListControlWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaBagList));
		crData *data = jiangKaBagCanvas->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);
		crListControlWidgetNode::ListNodeVec& listNodeVec = jiangKaBagList->getListNodeVec();
		int i = 0;
		void *param;
		data->getParam(WCHDATA_JXJForJKVec, param);
		JXJItemBagVec *itembagvec =	(JXJItemBagVec *)param;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr,i++)
		{
			if((*itr) != NULL && (*itr)->getItemID() == m_jiangKaid)
			{
				jiangKaBagList->select(i);
				crListControlNode *selectNode = jiangKaBagList->getSelectNode();
				if(!selectNode)return;
				int selectid = selectNode->getData();
				if (listNodeVec[i]!=NULL)
				{
					ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
					ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
					if(m_trueExecCount != 0)
					{
						if(rectCanvas.valid() && renwucanvas.valid())
						{
							crData *renwudata = renwucanvas->getDataClass();
							if(!renwudata)return;
							renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
							renwudata->inputParam(WCHDATA_JXJNewHandUpgradeCardID,&i);
							crVector4f rect = selectNode->getRect();
							CRCore::crMatrixTransform *m_mvpwNode = jiangKaBagList->getMvpwNode();
							const crMatrix &mat = m_mvpwNode->getMatrix() * jiangKaBagList->getParentCanvas()->getMatrix();
							crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
							crVector3 ipos = pos * mat;
							rectCanvas->setPosition(ipos);
							if(m_trueExecCount>0)
								m_trueExecCount--;
							check = true;
							break;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFormationTemporaryJKInfoMethod  //编队临时将卡信息
//
/////////////////////////////////////////
crJXJCheckFormationTemporaryJKInfoMethod::crJXJCheckFormationTemporaryJKInfoMethod():
	m_trueExecCount(1),
	m_index(0)
{
}
crJXJCheckFormationTemporaryJKInfoMethod::crJXJCheckFormationTemporaryJKInfoMethod(const crJXJCheckFormationTemporaryJKInfoMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_index(handle.m_index)
{
}
void crJXJCheckFormationTemporaryJKInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckFormationTemporaryJKInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJCheckFormationTemporaryJKInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		if(m_trueExecCount != 0)
		{
			ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
			if(formationCanvas.valid() && playerData->getDataClass())
			{
				crData *Data = formationCanvas->getDataClass();
				void *param;
				Data->getParam(WCHDATA_JXJTempFormationVec,param);
				FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
				if(formationInfoVec)
				{
					FormationInfoVec::iterator itr_info = formationInfoVec->begin();
					for (; itr_info != formationInfoVec->end(); itr_info++)
					{
						if (m_index == 1)
						{
							if((itr_info->get()->getEquips())[0] > 0)
							{
								check = true;
								if(m_trueExecCount>0)
									m_trueExecCount--;
								break;
							}
						}
						if (m_index == 2)
						{
							if ((*itr_info)->getAbstractID() == 20007)
							{
								if((itr_info->get()->getEquips())[0] > 0)
								{
									check = true;
									if(m_trueExecCount>0)
										m_trueExecCount--;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUINewHandKnapsackEquipClickMethod
//
/////////////////////////////////////////
crJXJUINewHandKnapsackEquipClickMethod::crJXJUINewHandKnapsackEquipClickMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_itemid(0)
{
}
crJXJUINewHandKnapsackEquipClickMethod::crJXJUINewHandKnapsackEquipClickMethod(const crJXJUINewHandKnapsackEquipClickMethod& handle):
	crMethod(handle),
	m_knapsackCanvas(handle.m_knapsackCanvas),
	m_knapsackList(handle.m_knapsackList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_itemid(handle.m_itemid)
{
}
void crJXJUINewHandKnapsackEquipClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandKnapsackEquipClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_knapsackCanvas = str;
		break;
	case 1:
		m_knapsackList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_renWuCanvas = str;
		break;
	case 4:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 5:
		m_taskid = (atoi(str.c_str()));
		break;
	case 6:
		m_itemid = (atoi(str.c_str()));
		break;
	}
}
void crJXJUINewHandKnapsackEquipClickMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> knapsackCanvas = crFilterRenderManager::getInstance()->findCanvas(m_knapsackCanvas);
		if(!knapsackCanvas.valid())
			return;
		crListControlWidgetNode *knapsackList = dynamic_cast<crListControlWidgetNode *>(knapsackCanvas->getWidget(m_knapsackList));
		crData *data = playerData->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);
		crListControlWidgetNode::ListNodeVec& listNodeVec = knapsackList->getListNodeVec();
		int i = 0;
		void *param;
		data->getParam(WCHDATA_JXJItemBagVec, param);
		JXJItemBagVec *itembagvec =	(JXJItemBagVec *)param;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr,i++)
		{
			if((*itr) != NULL && (*itr)->getItemID() == m_itemid)
			{
				knapsackList->select(i);
				crListControlNode *selectNode = knapsackList->getSelectNode();
				if(!selectNode)return;
				int selectid = selectNode->getData();
				if (listNodeVec[i]!=NULL)
				{
					ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
					ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
					if(m_trueExecCount != 0)
					{
						if(rectCanvas.valid() && renwucanvas.valid())
						{
							crData *renwudata = renwucanvas->getDataClass();
							if(!renwudata)return;
							renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
							crVector4f rect = selectNode->getRect();
							CRCore::crMatrixTransform *m_mvpwNode = knapsackList->getMvpwNode();
							const crMatrix &mat = m_mvpwNode->getMatrix() * knapsackList->getParentCanvas()->getMatrix();
							crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
							crVector3 ipos = pos * mat;
							rectCanvas->setPosition(ipos);
							if(m_trueExecCount>0)
								m_trueExecCount--;
							check = true;
							break;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUINewHandUpgradeCardClickMethod
//
/////////////////////////////////////////
crJXJUINewHandUpgradeCardClickMethod::crJXJUINewHandUpgradeCardClickMethod()
{
}
crJXJUINewHandUpgradeCardClickMethod::crJXJUINewHandUpgradeCardClickMethod(const crJXJUINewHandUpgradeCardClickMethod& handle):
	crMethod(handle),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas),
	m_jiangKaBagList(handle.m_jiangKaBagList),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_jiangKaColorRadio(handle.m_jiangKaColorRadio),
	m_jiangKaSystemRadio(handle.m_jiangKaSystemRadio)
{
}
void crJXJUINewHandUpgradeCardClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandUpgradeCardClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_jiangKaBagList = str;
		break;
	case 2:
		m_renWuCanvas = str;
		break;
	case 3:
		m_jiangKaColorRadio = str;
		break;
	case 4:
		m_jiangKaSystemRadio = str;
		break;
	}
}
void crJXJUINewHandUpgradeCardClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		int i = 0;
		ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
		if (renWuCanvas.valid())
		{
			void *param;
			crData *renwudata = renWuCanvas->getDataClass();
			if (renwudata)
			{
				renwudata->getParam(WCHDATA_JXJNewHandUpgradeCardID,param);
				i = *(int *)param;
			}
		}
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if(!jiangKaBagCanvas.valid())
			return;
		crListControlWidgetNode *jiangKaBagList = dynamic_cast<crListControlWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaBagList));
		if(jiangKaBagList)
		{
			//crRadioGroupWidgetNode *jiangKaColorRadio = dynamic_cast<crRadioGroupWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaColorRadio));
			//crRadioGroupWidgetNode *jiangKaSystemRadio = dynamic_cast<crRadioGroupWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaSystemRadio));
			//if (jiangKaColorRadio)
			//{
			//	jiangKaColorRadio->select(0);
			//	jiangKaColorRadio->doEvent(WCH_UI_SelectChange);
			//}
			//if (jiangKaSystemRadio)
			//{
			//	if (jiangKaSystemRadio->getSelect() != 1)
			//	{
			//		jiangKaSystemRadio->select(1);
			//		jiangKaSystemRadio->doEvent(WCH_UI_SelectChange);
			//	}
			//}
			jiangKaBagList->select(i);
			jiangKaBagList->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
		}
	}
}

/////////////////////////////////////////
//
//crJXJSkipNewHandInfoMethod
//
/////////////////////////////////////////
crJXJSkipNewHandInfoMethod::crJXJSkipNewHandInfoMethod():
	m_this(NULL)
{
}
crJXJSkipNewHandInfoMethod::crJXJSkipNewHandInfoMethod(const crJXJSkipNewHandInfoMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSkipNewHandInfoMethod::inputParam(int i, void *param)
{
}
void crJXJSkipNewHandInfoMethod::addParam(int i, const std::string& str)
{
}
void crJXJSkipNewHandInfoMethod::operator()(crHandle &handle)
{
	bool check = true;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJSkipNewHand,param);
		bool *skipnewhand = (bool *)param;
		if (*skipnewhand)
		{
			check = false;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUINewHandTroopUndoClickMethod	
//
/////////////////////////////////////////
crJXJUINewHandTroopUndoClickMethod::crJXJUINewHandTroopUndoClickMethod()
{
}
crJXJUINewHandTroopUndoClickMethod::crJXJUINewHandTroopUndoClickMethod(const crJXJUINewHandTroopUndoClickMethod& handle):
	crMethod(handle),
	m_formationCanvas(handle.m_formationCanvas),
	m_imageBoxWidget(handle.m_imageBoxWidget)
{
}
void crJXJUINewHandTroopUndoClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandTroopUndoClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_formationCanvas = str;
		break;
	case 1:
		m_imageBoxWidget = str;
		break;
	}
}
void crJXJUINewHandTroopUndoClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> formationCanvas = crFilterRenderManager::getInstance()->findCanvas(m_formationCanvas);
		if(!formationCanvas.valid())
			return;
		crImageBoxWidgetNode *imageBoxWidget = dynamic_cast<crImageBoxWidgetNode *>(formationCanvas->getWidget(m_imageBoxWidget));
		if(imageBoxWidget)
		{
			imageBoxWidget->doEvent(MAKEINT32(crGUIEventAdapter::RIGHT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUINewHandFormationJiangKaMethod
//
/////////////////////////////////////////
crJXJUINewHandFormationJiangKaMethod::crJXJUINewHandFormationJiangKaMethod():
	m_taskid(NULL),
	m_trueExecCount(),
	m_jiangKaid(0)
{
}
crJXJUINewHandFormationJiangKaMethod::crJXJUINewHandFormationJiangKaMethod(const crJXJUINewHandFormationJiangKaMethod& handle):
	crMethod(handle),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas),
	m_jiangKaBagList(handle.m_jiangKaBagList),
	m_rectCanvasName(handle.m_rectCanvasName),
	m_renWuCanvas(handle.m_renWuCanvas),
	m_trueExecCount(handle.m_trueExecCount),
	m_taskid(handle.m_taskid),
	m_jiangKaid(handle.m_jiangKaid)
{
}
void crJXJUINewHandFormationJiangKaMethod::inputParam(int i, void *param)
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
void crJXJUINewHandFormationJiangKaMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_jiangKaBagList = str;
		break;
	case 2:
		m_rectCanvasName = str;
		break;
	case 3:
		m_renWuCanvas = str;
		break;
	case 4:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	case 5:
		m_taskid = (atoi(str.c_str()));
		break;
	case 6:
		m_jiangKaid = (atoi(str.c_str()));
		break;
	}
}
void crJXJUINewHandFormationJiangKaMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if(!jiangKaBagCanvas.valid())
			return;
		crListControlWidgetNode *jiangKaBagList = dynamic_cast<crListControlWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaBagList));
		crData *data = jiangKaBagCanvas->getDataClass();
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if (itemtab.valid())
		{
			CRCore::ScopedLock<crData> lock(*data);
			crListControlWidgetNode::ListNodeVec& listNodeVec = jiangKaBagList->getListNodeVec();
			int i = 0;
			void *param;
			int itemid = 0;
			int cardtypeidindex = 0;
			int cardtypeid = 0;
			crTableIO::StrVec record;
			data->getParam(WCHDATA_JXJForJKVec, param);
			JXJItemBagVec *itembagvec =	(JXJItemBagVec *)param;
			for( JXJItemBagVec::iterator itr = itembagvec->begin();
				itr != itembagvec->end();
				++itr)
			{
				if ((*itr) != NULL)
				{
					itemid = (*itr)->getItemID();
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
					{
						cardtypeidindex = itemtab->getTitleIndex("将卡类型");
						cardtypeid = atoi(record[cardtypeidindex].c_str());
						if(cardtypeid == 0)
						{
							if (itemid == m_jiangKaid)
							{
								jiangKaBagList->select(i);
								crListControlNode *selectNode = jiangKaBagList->getSelectNode();
								if(!selectNode)return;
								int selectid = selectNode->getData();
								if (listNodeVec[i]!=NULL)
								{
									ref_ptr<crCanvasNode> rectCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rectCanvasName);
									ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
									if(m_trueExecCount != 0)
									{
										if(rectCanvas.valid() && renwucanvas.valid())
										{
											crData *renwudata = renwucanvas->getDataClass();
											if(!renwudata)return;
											renwudata->inputParam(WCHDATA_JXJNewHandTaskID,&m_taskid);
											renwudata->inputParam(WCHDATA_JXJNewHandUpgradeCardID,&i);
											crVector4f rect = selectNode->getRect();
											CRCore::crMatrixTransform *m_mvpwNode = jiangKaBagList->getMvpwNode();
											const crMatrix &mat = m_mvpwNode->getMatrix() * jiangKaBagList->getParentCanvas()->getMatrix();
											crVector3 pos(0.5f*rect[2] + rect[0],rect[1] - 0.5f*rect[3],0.0f);
											crVector3 ipos = pos * mat;
											rectCanvas->setPosition(ipos);
											if(m_trueExecCount>0)
												m_trueExecCount--;
											check = true;
											break;
										}
									}
								}
							}
							i++;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUINewHandFormationCardClickMethod
//
/////////////////////////////////////////
crJXJUINewHandFormationCardClickMethod::crJXJUINewHandFormationCardClickMethod()
{
}
crJXJUINewHandFormationCardClickMethod::crJXJUINewHandFormationCardClickMethod(const crJXJUINewHandFormationCardClickMethod& handle):
	crMethod(handle),
	m_jiangKaBagCanvas(handle.m_jiangKaBagCanvas),
	m_jiangKaBagList(handle.m_jiangKaBagList),
	m_renWuCanvas(handle.m_renWuCanvas)
{
}
void crJXJUINewHandFormationCardClickMethod::inputParam(int i, void *param)
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
void crJXJUINewHandFormationCardClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jiangKaBagCanvas = str;
		break;
	case 1:
		m_jiangKaBagList = str;
		break;
	case 2:
		m_renWuCanvas = str;
		break;
	}
}
void crJXJUINewHandFormationCardClickMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		int i = 0;
		ref_ptr<crCanvasNode> renWuCanvas = crFilterRenderManager::getInstance()->findCanvas(m_renWuCanvas);
		if (renWuCanvas.valid())
		{
			void *param;
			crData *renwudata = renWuCanvas->getDataClass();
			if (renwudata)
			{
				renwudata->getParam(WCHDATA_JXJNewHandUpgradeCardID,param);
				i = *(int *)param;
			}
		}
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		ref_ptr<crCanvasNode> jiangKaBagCanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangKaBagCanvas);
		if(!jiangKaBagCanvas.valid())
			return;
		crListControlWidgetNode *jiangKaBagList = dynamic_cast<crListControlWidgetNode *>(jiangKaBagCanvas->getWidget(m_jiangKaBagList));
		if(jiangKaBagList)
		{
			jiangKaBagList->select(i);
			jiangKaBagList->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::DOUBLECLICK));
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckGuanZhiOpenInfoMethod
//
/////////////////////////////////////////
crJXJCheckGuanZhiOpenInfoMethod::crJXJCheckGuanZhiOpenInfoMethod():
	m_trueExecCount()
{
}
crJXJCheckGuanZhiOpenInfoMethod::crJXJCheckGuanZhiOpenInfoMethod(const crJXJCheckGuanZhiOpenInfoMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckGuanZhiOpenInfoMethod::inputParam(int i, void *param)
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
void crJXJCheckGuanZhiOpenInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckGuanZhiOpenInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		if (m_trueExecCount != 0)
		{
			void *param;
			crData *data = myPlayer->getDataClass();
			crData *roledata = myPlayer->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char curlevel = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGovBtnOpenLv,viplv).c_str()));
			if (curlevel >= openlv)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}	
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUIGongFangOpenLvInfoMethod
//
/////////////////////////////////////////
crJXJUIGongFangOpenLvInfoMethod::crJXJUIGongFangOpenLvInfoMethod():
	m_trueExecCount()
{
}
crJXJUIGongFangOpenLvInfoMethod::crJXJUIGongFangOpenLvInfoMethod(const crJXJUIGongFangOpenLvInfoMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJUIGongFangOpenLvInfoMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangOpenLvInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJUIGongFangOpenLvInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		if (m_trueExecCount != 0)
		{
			void *param;
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crVector3i openlv;
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGongfangOpenlvVec3i,viplv).c_str(),openlv);
			data->getParam(WCHDATA_JXJGongfanglv,param);
			unsigned char gongfanglv = *(unsigned char *)param;
			if (gongfanglv >= openlv[m_index])
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUIGongFangJingLianInfoMethod
//
/////////////////////////////////////////
crJXJUIGongFangJingLianInfoMethod::crJXJUIGongFangJingLianInfoMethod():
	m_trueExecCount()
{
}
crJXJUIGongFangJingLianInfoMethod::crJXJUIGongFangJingLianInfoMethod(const crJXJUIGongFangJingLianInfoMethod& handle):
	crMethod(handle),
	m_gongFangCanvas(handle.m_gongFangCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJUIGongFangJingLianInfoMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangJingLianInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongFangCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJUIGongFangJingLianInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_gongFangCanvas);
	if(canvas.valid())
	{
		if (m_trueExecCount != 0)
		{
			void *param;
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData *)param;
			int equipid = equipdata->getItemID();
			int equipcount = equipdata->getItemCount();
			if (equipid > 0 && equipcount > 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJUIGongFangSaveJingLianInfoMethod
//
/////////////////////////////////////////
crJXJUIGongFangSaveJingLianInfoMethod::crJXJUIGongFangSaveJingLianInfoMethod():
	m_trueExecCount()
{
}
crJXJUIGongFangSaveJingLianInfoMethod::crJXJUIGongFangSaveJingLianInfoMethod(const crJXJUIGongFangSaveJingLianInfoMethod& handle):
	crMethod(handle),
	m_gongFangCanvas(handle.m_gongFangCanvas),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJUIGongFangSaveJingLianInfoMethod::inputParam(int i, void *param)
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
void crJXJUIGongFangSaveJingLianInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongFangCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJUIGongFangSaveJingLianInfoMethod::operator()(crHandle &handle)
{
	bool check = false;
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_gongFangCanvas);
	if(canvas.valid())
	{
		if (m_trueExecCount != 0)
		{
			void *param;
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
			int aftermagic = *(int *)param;
			if (aftermagic != 0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckGameSaoDangTaskMethod
//
/////////////////////////////////////////
crJXJCheckGameSaoDangTaskMethod::crJXJCheckGameSaoDangTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckGameSaoDangTaskMethod::crJXJCheckGameSaoDangTaskMethod(const crJXJCheckGameSaoDangTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckGameSaoDangTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckGameSaoDangTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckGameSaoDangTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		unsigned short fubenid;
		int taskid = m_this->getTaskID();
		if (m_trueExecCount != 0)
		{
			ref_ptr<crTableIO>taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
			if (taskTitleTab.valid())
			{
				int fubenidindex = taskTitleTab->getTitleIndex("fubenid");
				int taskidindex = taskTitleTab->getTitleIndex("taskid");
				crTableIO::StrVec taskrecord;
				if(taskTitleTab->queryOneRecord(taskidindex,crArgumentParser::appItoa(taskid),taskrecord) >= 0)
				{
					fubenid = (unsigned short)atoi(taskrecord[fubenidindex].c_str());
					crData *data = playerData->getDataClass();
					data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
					unsigned short curfubenid = *(unsigned short *)param;
					crData *brainData = crBrain::getInstance()->getDataClass();
					brainData->getParam(WCHDATA_JXJBrainRushTaskCount,param);
					int *rushTaskCount = (int *)param;
					if (curfubenid == fubenid)
					{
						data->getParam(WCHDATA_JXJFubenProgressMap,param);
						FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
						FubenProgressMap::iterator itrcur = fubenProgress->find((unsigned short)curfubenid);
						if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 0  && (*rushTaskCount) > 0)
						{
							for (int i = 0; i < (*rushTaskCount); i++)
							{
								if (m_this->getTaskProgress() == m_this->getTaskSteps() - 2)
								{
									playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(taskid,TS_One));
									check = true;
								}
								else
								{
									playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(taskid,TS_One));
									//m_trueExecCount++;
								}
								(*rushTaskCount)--;
								//brainData->inputParam(WCHDATA_JXJBrainRushTaskCount,NULL);
								//m_trueExecCount--;
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJAutoCheckSaoDangTaskMethod
//
/////////////////////////////////////////
crJXJAutoCheckSaoDangTaskMethod::crJXJAutoCheckSaoDangTaskMethod()
{
}
crJXJAutoCheckSaoDangTaskMethod::crJXJAutoCheckSaoDangTaskMethod(const crJXJAutoCheckSaoDangTaskMethod& handle):
	crMethod(handle)
{
}
void crJXJAutoCheckSaoDangTaskMethod::inputParam(int i, void *param)
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
void crJXJAutoCheckSaoDangTaskMethod::addParam(int i, const std::string& str)
{

}
void crJXJAutoCheckSaoDangTaskMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = m_this->getParentCanvas()->getDataClass();
		if(!data)return;
		void *param;
		data->getParam(WCHDATA_JXJGetRenWuRewardID,param);
		int taskid = *(int *)param;

		ref_ptr<crTableIO>taskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTab);
		int stepTypeindex = taskTab->getTitleIndex("StepType");
		crTableIO::StrVec record;
		if(taskTab->queryOneRecord(0,crArgumentParser::appItoa(taskid),record) >= 0)
		{
			int stepTypeid = atoi(record[stepTypeindex].c_str());
			switch (stepTypeid)
			{
			case 0:
				playerData->doEvent(WCH_JXJReferTask,MAKEINT64(taskid,NULL));
				break;
			case 1:
				playerData->doEvent(WCH_JXJReferTask,MAKEINT64(taskid,TS_One));
				break;
			default:
				break;
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUISaoDangRenWuRewardMethod
//
/////////////////////////////////////////
crJXJUISaoDangRenWuRewardMethod::crJXJUISaoDangRenWuRewardMethod():
	m_taskid(0)
{
}
crJXJUISaoDangRenWuRewardMethod::crJXJUISaoDangRenWuRewardMethod(const crJXJUISaoDangRenWuRewardMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_expCount(handle.m_expCount),
	m_copCashCount(handle.m_copCashCount),
	m_oneCount(handle.m_oneCount),
	m_twoCount(handle.m_twoCount),
	m_rewardOne(handle.m_rewardOne),
	m_rewardTwo(handle.m_rewardTwo),
	m_taskid(handle.m_taskid)
{
}
void crJXJUISaoDangRenWuRewardMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJUISaoDangRenWuRewardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_expCount = str;
		break;
	case 2:
		m_copCashCount = str;
		break;
	case 3:
		m_oneCount = str;
		break;
	case 4:
		m_twoCount = str;
		break;
	case 5:
		m_rewardOne = str;
		break;
	case 6:
		m_rewardTwo = str;
		break;
	case 7:
		m_taskid = atoi(str.c_str());
		break;
	}
}
void crJXJUISaoDangRenWuRewardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(!canvas.valid())
			return;
		crStaticTextWidgetNode *expCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_expCount));
		crStaticTextWidgetNode *copCashCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_copCashCount));
		crStaticTextWidgetNode *oneCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_oneCount));
		crStaticTextWidgetNode *twoCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_twoCount));
		crMultiSwitch *rewardOne = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_rewardOne));
		crMultiSwitch *rewardTwo = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_rewardTwo));
		if (oneCount)oneCount->setVisiable(false);
		if (twoCount)twoCount->setVisiable(false);
		if (rewardOne)rewardOne->setVisiable(false);
		if (rewardTwo)rewardTwo->setVisiable(false);
		ref_ptr<crTableIO>taskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTab);
		int expCountid = taskTab->getTitleIndex("Exp");
		int copCashCountid = taskTab->getTitleIndex("Gold");
		int oneCountid = taskTab->getTitleIndex("");
		int twoCountid = taskTab->getTitleIndex("");

		crData *data = canvas->getDataClass();
		data->inputParam(WCHDATA_JXJGetRenWuRewardID,&m_taskid);
		crTableIO::StrVec record;
		if(taskTab->queryOneRecord(0,crArgumentParser::appItoa(m_taskid),record) >= 0)
		{
			if (expCount)
			{
				expCount->setString(record[expCountid]);
			}
			if (copCashCount)
			{
				copCashCount->setString(record[copCashCountid]);
			}
			//if (oneCount)
			//{
			//	oneCount->setVisiable(true);
			//	if (rewardOne)rewardOne->setVisiable(true);
			//	oneCount->setString(record[oneCountid]);
			//}
			//if (twoCount)
			//{
			//	twoCount->setVisiable(true);
			//	if (rewardTwo)rewardTwo->setVisiable(true);
			//	twoCount->setString(record[twoCountid]);
			//}
		}
		if(canvas.valid())
			crFilterRenderManager::getInstance()->doModal(canvas.get());
	}
}

/////////////////////////////////////////
//
//crJXJUISelectSaoDangFubenMethod
//
/////////////////////////////////////////
crJXJUISelectSaoDangFubenMethod::crJXJUISelectSaoDangFubenMethod():
	m_taskid(),
	m_this(NULL)
{}
crJXJUISelectSaoDangFubenMethod::crJXJUISelectSaoDangFubenMethod(const crJXJUISelectSaoDangFubenMethod& handle):
	crMethod(handle),
	m_rushCanvas(handle.m_rushCanvas),
	m_rushCountInput(handle.m_rushCountInput),
	m_taskid(handle.m_taskid),
	m_this(NULL)
{
}
void crJXJUISelectSaoDangFubenMethod::inputParam(int i, void *param)
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

void crJXJUISelectSaoDangFubenMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_rushCanvas = str;
		break;
	case 1:
		m_rushCountInput = str;
		break;
	case 2:
		m_taskid = atoi(str.c_str());
		break;
	}
}
void crJXJUISelectSaoDangFubenMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		void *param;
		crRole *mainRole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		if(!mainRole || !mainRole->getMetierDataClass()) return;
		crData *myMetierData = mainRole->getMetierDataClass();
		myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
		myMetierData->getParam(WCHDATA_GameTaskMap,param);
		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
		if(!gameTaskMap||gameTaskMap->empty())
		{
			myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
			return;
		}
		unsigned char i = 0;
		int progress = 0;
		int step = 0;
		crGameTask* gameTask;
		for( GameTaskMap::iterator itr = gameTaskMap->begin();
			i < crGlobalHandle::gData()->gMaxDisplayTaskTrace() && itr != gameTaskMap->end();
			++itr )
		{
			if(itr->second->getActivation() && !itr->second->isComplete())
			{
				gameTask = itr->second.get();
				if (m_taskid == gameTask->getTaskID())
				{
					progress = gameTask->getTaskProgress();
					step = gameTask->getTaskSteps()-1;
					break;
				}
			}
		}
		int rushNum = step - progress;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJJunling,param);
		unsigned short junling = *(unsigned short *)param/100;
		ref_ptr<crCanvasNode> rushCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rushCanvas);
		if (rushCanvas.valid())
		{
			ref_ptr<crEditWidgetNode> rushCountInput = dynamic_cast<crEditWidgetNode *>(rushCanvas->getWidget(m_rushCountInput));
			if (rushCountInput.valid() && junling >= rushNum)
			{
				rushCountInput->setString(crArgumentParser::appItoa(rushNum));
				rushCountInput->doEvent(WCH_UI_ValueChange);
			}
			else if(rushCountInput.valid() && junling >= 0)
			{
				rushCountInput->setString(crArgumentParser::appItoa(junling));
				rushCountInput->doEvent(WCH_UI_ValueChange);
			}
		}
		myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJUISetSaoDangFubenInfoMethod
//
/////////////////////////////////////////
crJXJUISetSaoDangFubenInfoMethod::crJXJUISetSaoDangFubenInfoMethod():
	m_taskid(),
	m_this(NULL)
{}
crJXJUISetSaoDangFubenInfoMethod::crJXJUISetSaoDangFubenInfoMethod(const crJXJUISetSaoDangFubenInfoMethod& handle):
	crMethod(handle),
	m_taskid(handle.m_taskid),
	m_this(NULL)
{
}
void crJXJUISetSaoDangFubenInfoMethod::inputParam(int i, void *param)
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

void crJXJUISetSaoDangFubenInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = atoi(str.c_str());
		break;
	}
}
void crJXJUISetSaoDangFubenInfoMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		ref_ptr<crTableIO>taskTitleTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
		int fubenindex = taskTitleTab->getTitleIndex("fubenid");
		int fubenid = 0;
		crTableIO::StrVec record;
		if (taskTitleTab->queryOneRecord(2,crArgumentParser::appItoa(m_taskid),record) >= 0)
		{
			fubenid = atoi(record[fubenindex].c_str());
			if (fubenid > 0)
			{
				unsigned char type = FT_Fuben;
				crData *playerData = playerGameData->getDataClass();
				playerData->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
				playerData->inputParam(WCHDATA_JXJFubenOrChengchiID,&fubenid);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckOpenFubenTipCanvasMethod
//
/////////////////////////////////////////
char crJXJCheckOpenFubenTipCanvasMethod::m_trueExecCount = 1;
crJXJCheckOpenFubenTipCanvasMethod::crJXJCheckOpenFubenTipCanvasMethod()
{
}
crJXJCheckOpenFubenTipCanvasMethod::crJXJCheckOpenFubenTipCanvasMethod(const crJXJCheckOpenFubenTipCanvasMethod& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas)
{

}
void crJXJCheckOpenFubenTipCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJCheckOpenFubenTipCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckOpenFubenTipCanvasMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
		if(playerData && targetCanvas.valid())
		{
			if(targetCanvas->getVisiable())
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckOpenFubenTipCanvasTwoMethod
//
/////////////////////////////////////////
char crJXJCheckOpenFubenTipCanvasTwoMethod::m_trueExecCount = 1;
crJXJCheckOpenFubenTipCanvasTwoMethod::crJXJCheckOpenFubenTipCanvasTwoMethod()
{
}
crJXJCheckOpenFubenTipCanvasTwoMethod::crJXJCheckOpenFubenTipCanvasTwoMethod(const crJXJCheckOpenFubenTipCanvasTwoMethod& handle):
	crMethod(handle),
	m_targetCanvas(handle.m_targetCanvas)
{

}
void crJXJCheckOpenFubenTipCanvasTwoMethod::inputParam(int i, void *param)
{
}
void crJXJCheckOpenFubenTipCanvasTwoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetCanvas = str;
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckOpenFubenTipCanvasTwoMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> targetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_targetCanvas);
		if(playerData && targetCanvas.valid())
		{
			if(targetCanvas->getVisiable())
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJCheckFirstBuildingTaskMethod
//
/////////////////////////////////////////
crJXJCheckFirstBuildingTaskMethod::crJXJCheckFirstBuildingTaskMethod():
	m_buildingid(0)
{

}
crJXJCheckFirstBuildingTaskMethod::crJXJCheckFirstBuildingTaskMethod(const crJXJCheckFirstBuildingTaskMethod& handle):
	crMethod(handle),
	m_buildingid(handle.m_buildingid)
{
}
void crJXJCheckFirstBuildingTaskMethod::inputParam(int i, void *param)
{

}
void crJXJCheckFirstBuildingTaskMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	}
}
void crJXJCheckFirstBuildingTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
	if(scene)
	{
		crInstanceItem *item  = scene->findSceneItem(m_buildingid);
		if(item)
		{
			check = true;
		}
	}
	handle.outputParam(0,&check);
}

///////////////////////////////////////////
////
////crJXJResetRenWuScrollBarMethod
////
///////////////////////////////////////////
//crJXJResetRenWuScrollBarMethod::crJXJResetRenWuScrollBarMethod()
//{
//}
//
//crJXJResetRenWuScrollBarMethod::crJXJResetRenWuScrollBarMethod(const crJXJResetRenWuScrollBarMethod& handle):
//	m_scrollbar(handle.m_scrollbar)
//{
//}
//
//void crJXJResetRenWuScrollBarMethod::inputParam(int i, void *param)
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
//void crJXJResetRenWuScrollBarMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_scrollbar = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJResetRenWuScrollBarMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(m_this && playerData)
//	{
//		ref_ptr<crScrollBarWidgetNode> Vscrollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scrollbar));
//		crData *braindata = crBrain::getInstance()->getDataClass();
//		if (braindata && Vscrollbar.valid())
//		{
//			float value =  Vscrollbar->getValue();
//			braindata->inputParam(WCHDATA_JXJNewHandTaskScrollBar,&value);
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJXuMuInitSelectRoleMethod
//
/////////////////////////////////////////
crJXJXuMuInitSelectRoleMethod::crJXJXuMuInitSelectRoleMethod(){}
crJXJXuMuInitSelectRoleMethod::crJXJXuMuInitSelectRoleMethod(const crJXJXuMuInitSelectRoleMethod& handle):
	crMethod(handle)
{
}
void crJXJXuMuInitSelectRoleMethod::inputParam(int i, void *param)
{
}

void crJXJXuMuInitSelectRoleMethod::addParam(int i, const std::string& str)
{
}

void crJXJXuMuInitSelectRoleMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera>cam = crCameraManager::getInstance()->getMainCamera();
	if(cam.valid())
	{
		crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(cam->getAttachedNode());
		crMyPlayerData *me = crMyPlayerData::getInstance();
		crRole *role = me->getLoadedRoleByIndex(0);
		crRole *role_1 = me->getLoadedRoleByIndex(1);
		crRole *role_2 = me->getLoadedRoleByIndex(2);
		if(role && cameraNode)
		{
			crVector3 rolePos = role->getPosition();
			cameraNode->doEvent(WCH_CameraFocus,MAKEINT64(&rolePos,NULL));
			//相机选择
			void *param;
			crData *cameraData = cameraNode->getDataClass();
			if(cameraData)
			{
				CRCore::ScopedLock<crData> lock(*cameraData);
				cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
				SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
				selectMyRoles->clear();
				selectMyRoles->push_back(role);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckManorStairTaskbarBtnMethod
//
/////////////////////////////////////////
crJXJCheckManorStairTaskbarBtnMethod::crJXJCheckManorStairTaskbarBtnMethod():
	m_this(NULL)
{
}
crJXJCheckManorStairTaskbarBtnMethod::crJXJCheckManorStairTaskbarBtnMethod(const crJXJCheckManorStairTaskbarBtnMethod& handle):
	crMethod(handle),
	m_CountryBtn(handle.m_CountryBtn),
	m_GroupBtn(handle.m_GroupBtn),
	m_WeiRenBtn(handle.m_WeiRenBtn),
	m_canvas(handle.m_canvas)
{

}
void crJXJCheckManorStairTaskbarBtnMethod::inputParam(int i, void *param)
{
}
void crJXJCheckManorStairTaskbarBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_CountryBtn = str;
		break;
	case 2:
		m_GroupBtn = str;
		break;
	case 3:
		m_WeiRenBtn = str;
		break;
	default:
		break;
	}
}
void crJXJCheckManorStairTaskbarBtnMethod::operator()(crHandle &handle)
{
	if(crRunGameHandle::getInstance()->isInManor())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(myPlayer && canvas.valid())
		{
			void *param;
			crData *data = myPlayer->getDataClass();
			ref_ptr<crButtonWidgetNode> countryBtn = dynamic_cast<crButtonWidgetNode *>(canvas -> getWidget(m_CountryBtn));
			ref_ptr<crButtonWidgetNode> groupBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_GroupBtn));
			ref_ptr<crButtonWidgetNode> weiRenBtn = dynamic_cast<crButtonWidgetNode *>(canvas -> getWidget(m_WeiRenBtn));
			//ref_ptr<crButtonWidgetNode> formationBtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_FormationBtn));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
			unsigned char getAward = *(unsigned char *)param;
			//// to do test 要删掉
			//getAward = (unsigned char)-1;
			////
			crData *roledata = myPlayer->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;
			if (countryBtn.valid())
			{
				unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryBtnOpenLv,viplv).c_str()));
				if (characterlevel >= openlv && getAward == (unsigned char)-1)
				{
					countryBtn->setEnable(true);
				}
				else
				{
					countryBtn->setEnable(false);
				}
			}
			if (groupBtn.valid())
			{
				unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupBtnOpenLv,viplv).c_str()));
				if (characterlevel >= openlv)
				{
					groupBtn->setEnable(true);
				}
				else
				{
					groupBtn->setEnable(false);
				}
			}
			if (weiRenBtn.valid())
			{
				unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWeiRenBtnOpenLv,viplv).c_str()));
				if (characterlevel >= openlv)
				{
					weiRenBtn->setEnable(true);
				}
				else
				{
					weiRenBtn->setEnable(false);
				}
			}
			///绝对不能这样实时将编队按钮打开
			//if (formationBtn.valid())
			//{
			//	unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormationOpenLv,viplv).c_str()));
			//	if (characterlevel >= openlv)
			//	{
			//		formationBtn->setEnable(true);
			//	}
			//	else
			//	{
			//		formationBtn->setEnable(false);
			//	}
			//}
		}
	}
}