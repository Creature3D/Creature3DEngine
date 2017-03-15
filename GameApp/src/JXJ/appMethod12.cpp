/**********************************************************************
*
*	文件:	   appMethod12.cpp
*
*	描述:	   
*
*	作者:	   李婧
*					  
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod12.h>
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
#include <stdlib.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crReadFile.h>
#include <shellapi.h>

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

/////////////////////////////////////////
//
//crJXJNewGuideGetRenWuRewardMethod
//
/////////////////////////////////////////
crJXJNewGuideGetRenWuRewardMethod::crJXJNewGuideGetRenWuRewardMethod()
{
}
crJXJNewGuideGetRenWuRewardMethod::crJXJNewGuideGetRenWuRewardMethod(const crJXJNewGuideGetRenWuRewardMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_expCount(handle.m_expCount),
	m_copCashCount(handle.m_copCashCount),
	m_oneCount(handle.m_oneCount),
	m_twoCount(handle.m_twoCount)
{
	for (int i = 0;i < 2; ++i)
	{
		m_rewardstr[i] = handle.m_rewardstr[i];
	}
}
void crJXJNewGuideGetRenWuRewardMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 0:
	//	if(param == 0)
	//	{//释放
	//		m_this = NULL;
	//	}
	//	break;
	//case 1:
	//	m_this = (crGameTask*)param;
	//	break;
	//}
}
void crJXJNewGuideGetRenWuRewardMethod::addParam(int i, const std::string& str)
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
	}
}
void crJXJNewGuideGetRenWuRewardMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(!canvas.valid())return;
	if (canvas->getVisiable())return;
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getMainRole())
	{
		crRole *mainRole = playerData->getMainRole();
		crData *metierData = mainRole->getMetierDataClass();
		if(metierData)
		{
			int taskid = 0;
			ref_ptr<crGameTask> gameTask;
			metierData->getParam(WCHDATA_GameTaskMap,param);
			metierData->excHandle(MAKEINT64(WCH_LockData,1));
			GameTaskMap* gameTaskMap = (GameTaskMap* )param;
			for (GameTaskMap::iterator itr = gameTaskMap->begin();
				itr != gameTaskMap->end();
				++itr)
			{
				taskid = 0;
				if (itr->second->isTaskCanRefer() && itr->second->getActivation())
				{
					taskid = itr->first;
					break;
				}
			}
			metierData->excHandle(MAKEINT64(WCH_LockData,0));
			//if (taskid == 1)
			//{
			//	playerData->doEvent(WCH_JXJReferTask,MAKEINT64(taskid,NULL));
			//}
			if (taskid > 0)
			{
				crData *data = canvas->getDataClass();
				data->inputParam(WCHDATA_JXJGetRenWuRewardID,&taskid);
				//data->getParam(WCHDATA_JXJRenwuRewardVec, param);
				//RenWuRewardVec* renwurewardvec = (RenWuRewardVec *)param;
				//renwurewardvec->clear();
				int cardcolorindex = 0;
				std::string itemiconname[2], cardiconname[2], itemswname[2], cardcolorname[2], rewardstr[2];
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
				ref_ptr<crStaticTextWidgetNode> expCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_expCount));
				ref_ptr<crStaticTextWidgetNode> copCashCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_copCashCount));
				ref_ptr<crStaticTextWidgetNode> nameinput[2] = { NULL };// = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_twoCount));
				ref_ptr<crImageBoxWidgetNode> itemicon[2] = { NULL };
				ref_ptr<crImageBoxWidgetNode> cardicon[2] = { NULL };
				ref_ptr<crMultiSwitch> rewardsw[2] = { NULL };
				ref_ptr<crMultiSwitch> cardcolor[2] = { NULL };
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
				//rewarditemindex[0] = tasktitleTab->getTitleIndex("类型1");
				//rewarditemindex[1] = tasktitleTab->getTitleIndex("奖励1");
				//rewarditemindex[2] = tasktitleTab->getTitleIndex("类型2");
				//rewarditemindex[3] = tasktitleTab->getTitleIndex("奖励2");
				int awardIdx[2] = {0};
				awardIdx[0] = tasktitleTab->getTitleIndex("奖励1");
				awardIdx[1] = tasktitleTab->getTitleIndex("奖励2");
				//int nameindex = itemTab->getTitleIndex("name");
				//int itemtypeindex = itemTab->getTitleIndex("类型");
				//int iconnameindex = itemTab->getTitleIndex("icon");
				//int itemcolorindex = itemTab->getTitleIndex("品质");
				crTableIO::StrVec record,titlerecord;//,itemrecord;
				if(taskTab->queryOneRecord(0,crArgumentParser::appItoa(taskid),record) >= 0 && expCountid >= 0 && copCashCountid >= 0)
				{
					if (expCount.valid())
					{
						expCount->setString(crArgumentParser::appItoa(atoi(record[expCountid].c_str())));
					}
					if (copCashCount.valid())
					{
						copCashCount->setString(crArgumentParser::appItoa(atoi(record[copCashCountid].c_str())));
					}
				}
				if (tasktitleTab->queryOneRecord(taskidindex,crArgumentParser::appItoa(taskid),titlerecord) >= 0)
				{
					crVector4i awardVec;
					//int iconboxindex = 0;
					for (int i = 0; i < 2; i++)
					{
						if (titlerecord[awardIdx[i]] == "") continue;
						crArgumentParser::appAtoVec(titlerecord[awardIdx[i]],awardVec);
						if(setRewardInfo(awardVec,itemicon[i].get(),nameinput[i].get(),cardcolor[i].get())==1)
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
						//	if (crArgumentParser::appAtoVec(titlerecord[i==0?rewarditemindex[1]:rewarditemindex[3]],itemvec[i]) && itemvec[i][0] > 0 && itemvec[i][2] > 0)
						//	{
						//		renwurewardvec->push_back(itemvec[i]);
						//		if (rewardsw[iconboxindex].valid())rewardsw[iconboxindex]->setVisiable(true);
						//		if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemvec[i][0]),itemrecord) >= 0)
						//		{
						//			if (nameinput[iconboxindex].valid())nameinput[iconboxindex]->setString(crArgumentParser::appItoa(itemvec[i][2]));
						//			if(atoi(itemrecord[itemtypeindex].c_str()) == IT_Jiangka)
						//			{
						//				if (cardicon[iconboxindex].valid())
						//				{
						//					cardicon[iconboxindex]->setVisiable(true);
						//					cardicon[iconboxindex]->setImageName(itemrecord[iconnameindex]);
						//				}
						//				if (rewardsw[iconboxindex].valid())rewardsw[iconboxindex]->setActiveSwitchSet(2);
						//				if (cardcolor[iconboxindex].valid())
						//				{
						//					cardcolorindex = atoi(itemrecord[itemcolorindex].c_str());
						//					//if (cardcolorindex > CardColor_Orange)
						//					//{
						//					//	cardcolorindex = CardColor_Orange;
						//					//}
						//					cardcolor[iconboxindex]->setActiveSwitchSet(cardcolorindex);
						//				}
						//			}
						//			else
						//			{
						//				if (itemicon[iconboxindex].valid())
						//				{
						//					itemicon[iconboxindex]->setVisiable(true);
						//					itemicon[iconboxindex]->setImageName(itemrecord[iconnameindex]);
						//				}

						//				if (rewardsw[iconboxindex].valid())rewardsw[iconboxindex]->setActiveSwitchSet(1);
						//			}
						//		}
						//		iconboxindex++;
						//	}
						//}
						//else if(atoi(titlerecord[i==0?rewarditemindex[0]:rewarditemindex[2]].c_str()) == T_Zhanquan)
						//{
						//	if (nameinput[iconboxindex].valid())nameinput[iconboxindex]->setString(titlerecord[i==0?rewarditemindex[1]:rewarditemindex[3]]);
						//	if (itemicon[iconboxindex].valid())
						//	{
						//		itemicon[iconboxindex]->setVisiable(true);
						//		itemicon[iconboxindex]->setImageName("T_Icon_Zhengzhanjuan.img");
						//	}

						//	if (rewardsw[iconboxindex].valid())rewardsw[iconboxindex]->setActiveSwitchSet(1);

						//	iconboxindex++;
						//}
					}
				}
				if(canvas.valid())crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				//if(canvas.valid())crFilterRenderManager::getInstance()->doModal(canvas.get()); 
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewGuideSearchBuildingLvMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchBuildingLvMethod::crJXJNewGuideSearchBuildingLvMethod():
	m_this(NULL),
	m_buildingid(0),
	m_targetlv(0)
{
}
crJXJNewGuideSearchBuildingLvMethod::crJXJNewGuideSearchBuildingLvMethod(const crJXJNewGuideSearchBuildingLvMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_buildingid(handle.m_buildingid),
	m_targetlv(handle.m_targetlv)
{
}
void crJXJNewGuideSearchBuildingLvMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchBuildingLvMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	case 1:
		m_targetlv = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchBuildingLvMethod::operator()(crHandle &handle)
{
	if (m_this && !m_this->isComplete() && !m_this->isTaskCanRefer())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(myPlayer && m_buildingid > 0)
		{
			void *param;
			int maxcount = 0;
			unsigned char buildinglv = 0;
			unsigned char maxlv = 0;
			crData *data = myPlayer->getDataClass();
			switch (m_buildingid)
			{
			case WCHDATA_JXJFuyalv:
			case WCHDATA_JXJBingyinglv:
			case WCHDATA_JXJJiusilv:
			case WCHDATA_JXJGongfanglv:
			case WCHDATA_JXJShijilv:
			case WCHDATA_JXJFukulv:
				maxcount = 1;
				break;
			case WCHDATA_JXJMingjulv1:
			case WCHDATA_JXJNongtianlv1:
				maxcount = 8;
				break;
			case WCHDATA_JXJMuchanglv1:
			case WCHDATA_JXJLingdilv1:
			case WCHDATA_JXJKuangshanlv1:
				maxcount = 4;
				break;
			default:
				maxcount = 0;
				break;
			}
			if (maxcount > 0)
			{
				for (int i = 0;i < maxcount; ++i)
				{
					data->getParam(m_buildingid + i,param);
					buildinglv = *(unsigned char*)param;
					if (buildinglv > maxlv)
					{
						maxlv = buildinglv;
						if (maxlv >= m_targetlv)
						{
							myPlayer->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
							break;
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchRecruitCountMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchRecruitCountMethod::crJXJNewGuideSearchRecruitCountMethod():
	m_armid(0),
	m_count(0)
{
}
crJXJNewGuideSearchRecruitCountMethod::crJXJNewGuideSearchRecruitCountMethod(const crJXJNewGuideSearchRecruitCountMethod& handle):
	crMethod(handle),
	m_armid(handle.m_armid),
	m_count(handle.m_count)
{
}
void crJXJNewGuideSearchRecruitCountMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchRecruitCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	}
}
void crJXJNewGuideSearchRecruitCountMethod::operator()(crHandle &handle)
{
	if (m_this && !m_this->isComplete() && !m_this->isTaskCanRefer())
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerData)
		{
			void *param;
			crData *data = playerData->getDataClass();
			if(!data)return;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJTroopsMap,param);
			TroopsMap *troopsMap = (TroopsMap *)param;
			if (troopsMap)
			{
				for( TroopsMap::iterator itr = troopsMap->begin();
					itr != troopsMap->end();
					++itr)
				{
					if(itr->first == m_armid)
					{
						if (itr->second >= m_count)
						{
							playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
						}
						break;
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewGuideSearchFormationInfoMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchFormationInfoMethod::crJXJNewGuideSearchFormationInfoMethod():
	m_armid(0),
	m_count(0)
{
}
crJXJNewGuideSearchFormationInfoMethod::crJXJNewGuideSearchFormationInfoMethod(const crJXJNewGuideSearchFormationInfoMethod& handle):
	crMethod(handle),
	m_armid(handle.m_armid),
	m_count(handle.m_count)
{
}
void crJXJNewGuideSearchFormationInfoMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchFormationInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid = atoi(str.c_str());
		break;
	case 1:
		m_count = (unsigned char)(atoi(str.c_str()));
		break;
	}
}
void crJXJNewGuideSearchFormationInfoMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		FormationInfoVec::iterator itr_info = formationInfoVec->begin();
		for (; itr_info != formationInfoVec->end(); itr_info++)
		{
			if (itr_info->get())
			{
				if (itr_info->get()->getAbstractID() == m_armid && itr_info->get()->getCount() >= m_count)
				{
					playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
					break;
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchFubenSuccessMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchFubenSuccessMethod::crJXJNewGuideSearchFubenSuccessMethod():
	m_fubenid(0)
{
}
crJXJNewGuideSearchFubenSuccessMethod::crJXJNewGuideSearchFubenSuccessMethod(const crJXJNewGuideSearchFubenSuccessMethod& handle):
	crMethod(handle),
	m_fubenid(handle.m_fubenid)
{
}
void crJXJNewGuideSearchFubenSuccessMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchFubenSuccessMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenid = (unsigned short)(atoi(str.c_str()));;
		break;
	}
}
void crJXJNewGuideSearchFubenSuccessMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	void *param;
	crData *data = playerData->getDataClass();
	data->excHandle(MAKEINT64(WCH_LockData,1));
	data->getParam(WCHDATA_JXJFubenProgressMap,param);
	FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
	FubenProgressMap::iterator itrcur = fubenProgress->find((unsigned short)m_fubenid);
	if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 0)
	{
		playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchFormationCardMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchFormationCardMethod::crJXJNewGuideSearchFormationCardMethod()
{
}
crJXJNewGuideSearchFormationCardMethod::crJXJNewGuideSearchFormationCardMethod(const crJXJNewGuideSearchFormationCardMethod& handle):
	crMethod(handle),
	m_armid(handle.m_armid),
	m_count(handle.m_count),
	m_cardid(handle.m_cardid)
{
}
void crJXJNewGuideSearchFormationCardMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchFormationCardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_armid[0] = atoi(str.c_str());
		break;
	case 1:
		m_count[0] = atoi(str.c_str());
		break;
	case 2:
		m_armid[1] = atoi(str.c_str());
		break;
	case 3:
		m_count[1] = atoi(str.c_str());
		break;
	case 4:
		m_cardid[0] = atoi(str.c_str());
		break;
	case 5:
		m_cardid[1] = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchFormationCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
		FormationInfoVec::iterator itr_info = formationInfoVec->begin();
		crVector2i equips;
		bool ifformation[2] = {false,false};
		bool ifhascard[2] = {false,false};
		for (; itr_info != formationInfoVec->end(); itr_info++)
		{
			if (itr_info->get())
			{
				for (int i = 0; i < 2; ++i)
				{
					if (m_armid[i] != 0)
					{
						if (itr_info->get()->getAbstractID() == m_armid[i] && itr_info->get()->getCount() >= m_count[i])
						{
							ifformation[i] = true;
						}
					}
					else
					{
						ifformation[i] = true;
					}
				}
				equips = itr_info->get()->getEquips();
				if (equips[0] > 0 || equips[1] > 0)
				{
					for (int i = 0; i < 2; i++)
					{
						if (m_cardid[i] != 0)
						{
							if (m_cardid[i] == equips[0] || m_cardid[i] == equips[1])
							{
								ifhascard[i] = true;
							}
						}
						else
						{
							ifhascard[i] = true;
						}
					}
				}
			}
		}
		if (ifformation[0] && ifformation[1] && ifhascard[0] && ifhascard[1])
		{
			playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}


/////////////////////////////////////////
//
//crJXJNewGuideSearchTroopsTechMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchTroopsTechMethod::crJXJNewGuideSearchTroopsTechMethod():
	m_troopstechid(0),
	m_troopstechlv(0)
{
}
crJXJNewGuideSearchTroopsTechMethod::crJXJNewGuideSearchTroopsTechMethod(const crJXJNewGuideSearchTroopsTechMethod& handle):
	crMethod(handle),
	m_troopstechid(handle.m_troopstechid),
	m_troopstechlv(handle.m_troopstechlv)
{
}
void crJXJNewGuideSearchTroopsTechMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchTroopsTechMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_troopstechid = atoi(str.c_str());
		break;
	case 1:
		m_troopstechlv = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchTroopsTechMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		if (m_troopstechid > 0)
		{
			TroopsTechMap::iterator itr = troopstechmap->find(m_troopstechid);
			if (itr != troopstechmap->end())
			{
				if (itr->second >= m_troopstechlv)
				{
					playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchRollCardMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchRollCardMethod::crJXJNewGuideSearchRollCardMethod():
	m_needrollcardcount(0)
{
}
crJXJNewGuideSearchRollCardMethod::crJXJNewGuideSearchRollCardMethod(const crJXJNewGuideSearchRollCardMethod& handle):
	crMethod(handle),
	m_needrollcardcount(handle.m_needrollcardcount)
{
}
void crJXJNewGuideSearchRollCardMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchRollCardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_needrollcardcount = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchRollCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJNewguideRollCardCount,param);
			int rollcardcount = *(int *)param;
			if (rollcardcount >= m_needrollcardcount)
			{
				playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchRollCardMethod
//
/////////////////////////////////////////
crJXJTaskSearchRollCardMethod::crJXJTaskSearchRollCardMethod() :
m_oldrollcount(0)
{
}
crJXJTaskSearchRollCardMethod::crJXJTaskSearchRollCardMethod(const crJXJTaskSearchRollCardMethod& handle) :
crMethod(handle),
m_oldrollcount(handle.m_oldrollcount)
{
}
void crJXJTaskSearchRollCardMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchRollCardMethod::addParam(int i, const std::string& str)
{
}
void crJXJTaskSearchRollCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJNewguideRollCardCount, param);
			int rollcardcount = *(int *)param;
			if (rollcardcount > m_oldrollcount)
			{
				m_oldrollcount = rollcardcount;
				playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 1));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchUpgradeCardMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchUpgradeCardMethod::crJXJNewGuideSearchUpgradeCardMethod():
	m_needupgradecardcount(0)
{
}
crJXJNewGuideSearchUpgradeCardMethod::crJXJNewGuideSearchUpgradeCardMethod(const crJXJNewGuideSearchUpgradeCardMethod& handle):
	crMethod(handle),
	m_needupgradecardcount(handle.m_needupgradecardcount)
{
}
void crJXJNewGuideSearchUpgradeCardMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchUpgradeCardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_needupgradecardcount = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchUpgradeCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJNewguideUpgradeCardCount,param);
			int needupgradecardcount = *(int *)param;
			if (needupgradecardcount >= m_needupgradecardcount)
			{
				playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchTechMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchTechMethod::crJXJNewGuideSearchTechMethod():
	m_techid(0),
	m_techlv(0)
{
}
crJXJNewGuideSearchTechMethod::crJXJNewGuideSearchTechMethod(const crJXJNewGuideSearchTechMethod& handle):
	crMethod(handle),
	m_techid(handle.m_techid),
	m_techlv(handle.m_techlv)
{
}
void crJXJNewGuideSearchTechMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchTechMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_techid = atoi(str.c_str());
		break;
	case 1:
		m_techlv = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchTechMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data = playerData->getDataClass();
		if(!data)return;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(m_techid,param);
		if (param)
		{
			unsigned char techlv = *(unsigned char *)param;
			if (techlv >= m_techlv)
			{
				playerData->doEvent(WCH_JXJAdvanceTask,MAKEINT64(m_this->getTaskID(),0));
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideSearchUpgradeCardMethod
//
/////////////////////////////////////////
crJXJNewGuideSearchMakeLordEquipMethod::crJXJNewGuideSearchMakeLordEquipMethod():
	m_needcount(0),
	m_needequipid(0)
{
}
crJXJNewGuideSearchMakeLordEquipMethod::crJXJNewGuideSearchMakeLordEquipMethod(const crJXJNewGuideSearchMakeLordEquipMethod& handle):
	crMethod(handle),
	m_needcount(handle.m_needcount),
	m_needequipid(handle.m_needequipid)
{
}
void crJXJNewGuideSearchMakeLordEquipMethod::inputParam(int i, void *param)
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
void crJXJNewGuideSearchMakeLordEquipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_needcount = atoi(str.c_str());
		break;
	case 1:
		m_needequipid = atoi(str.c_str());
		break;
	}
}
void crJXJNewGuideSearchMakeLordEquipMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJNewguideMakeLordEquipCount,param);
			int needcount = *(int *)param;
			if (needcount >= m_needcount)
			{
				if (m_needequipid > 0)
				{//指定打造装备
					crData *data = playerData->getDataClass();
					data->getParam(WCHDATA_JXJItemBagVec, param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					for (JXJItemBagVec::iterator itr = itembagvec->begin();
						itr != itembagvec->end();
						++itr)
					{
						if (itr->get())
						{
							if (itr->get()->getItemID() == m_needequipid)
							{
								playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
								break;
							}
						}
					}
				}
				else
				{
					playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchWearLordEquipMethod
//
/////////////////////////////////////////
crJXJTaskSearchWearLordEquipMethod::crJXJTaskSearchWearLordEquipMethod() :
m_equipid(0)
{
}
crJXJTaskSearchWearLordEquipMethod::crJXJTaskSearchWearLordEquipMethod(const crJXJTaskSearchWearLordEquipMethod& handle) :
crMethod(handle),
m_equipid(handle.m_equipid)
{
}
void crJXJTaskSearchWearLordEquipMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchWearLordEquipMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_equipid = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJTaskSearchWearLordEquipMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crRole *mainRole = playerData->getMainRole();
		if (mainRole)
		{
			crData *mainroledata = mainRole->getDataClass();
			mainroledata->getParam(WCHDATA_JXJLordEquipVec, param);
			JXJLordEquipVec lordequipvec = *(JXJLordEquipVec *)param;
			for (JXJLordEquipVec::iterator itr = lordequipvec.begin();
				itr != lordequipvec.end();
				++itr)
			{
				if (itr->first > 0)
				{
					if (itr->first == m_equipid)
					{
						playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchTroopsTechlvMethod
//
/////////////////////////////////////////
crJXJTaskSearchTroopsTechlvMethod::crJXJTaskSearchTroopsTechlvMethod() :
m_troopslv(0)
{
}
crJXJTaskSearchTroopsTechlvMethod::crJXJTaskSearchTroopsTechlvMethod(const crJXJTaskSearchTroopsTechlvMethod& handle) :
crMethod(handle),
m_troopslv(handle.m_troopslv)
{
}
void crJXJTaskSearchTroopsTechlvMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchTroopsTechlvMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_troopslv = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJTaskSearchTroopsTechlvMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJTroopsTechMap, param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		for (TroopsTechMap::iterator itr = troopstechmap->begin();
			itr != troopstechmap->end();
			++itr)
		{
			if (itr->first > 0)
			{
				if (itr->second >= m_troopslv)
				{
					playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchItemMethod
//
/////////////////////////////////////////
crJXJTaskSearchItemMethod::crJXJTaskSearchItemMethod() :
m_itemid(0),
m_itemcount(0)
{
}
crJXJTaskSearchItemMethod::crJXJTaskSearchItemMethod(const crJXJTaskSearchItemMethod& handle) :
crMethod(handle),
m_itemid(handle.m_itemid),
m_itemcount(handle.m_itemcount)
{
}
void crJXJTaskSearchItemMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_itemid = atoi(str.c_str());
		break;
	case 1:
		m_itemcount = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJTaskSearchItemMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		int totalitemcount = 0;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJItemBagVec, param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		for (JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr)
		{
			if (itr->get())
			{
				if (itr->get()->getItemID() == m_itemid)
				{
					totalitemcount += itr->get()->getItemCount();
					if (totalitemcount >= m_itemcount)
					{
						playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
						break;
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchAppointCardMethod
//
/////////////////////////////////////////
crJXJTaskSearchAppointCardMethod::crJXJTaskSearchAppointCardMethod() :
m_itemid(0),
m_itemcount(0)
{
}
crJXJTaskSearchAppointCardMethod::crJXJTaskSearchAppointCardMethod(const crJXJTaskSearchAppointCardMethod& handle) :
crMethod(handle),
m_itemid(handle.m_itemid),
m_itemcount(handle.m_itemcount)
{
}
void crJXJTaskSearchAppointCardMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchAppointCardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_itemid = atoi(str.c_str());
		break;
	case 1:
		m_itemcount = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJTaskSearchAppointCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		int totalitemcount = 0;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJAppointJiangkaBagVec, param);
		JXJAppointCardVec *appointjiangkabagvec = (JXJAppointCardVec *)param;
		for (JXJAppointCardVec::iterator itr = appointjiangkabagvec->begin();
			itr != appointjiangkabagvec->end();
			++itr)
		{
			if (itr->get())
			{
				if (m_itemid > 0)
				{
					if (itr->get()->getItemID() == m_itemid)
					{
						totalitemcount += 1;
						if (totalitemcount >= m_itemcount)
						{
							playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
							break;
						}
					}
				}
				else
				{
					if (itr->get()->getItemID() > 0)
					{
						playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
						break;
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchLevyCountMethod
//
/////////////////////////////////////////
crJXJTaskSearchLevyCountMethod::crJXJTaskSearchLevyCountMethod()
{
}
crJXJTaskSearchLevyCountMethod::crJXJTaskSearchLevyCountMethod(const crJXJTaskSearchLevyCountMethod& handle) :
crMethod(handle)
{
}
void crJXJTaskSearchLevyCountMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchLevyCountMethod::addParam(int i, const std::string& str)
{
}
void crJXJTaskSearchLevyCountMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJTotalLevyCount, param);
			int totalcount = *(int *)param;
			if (totalcount >= 1)
			{
				playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
			}
		}
		//int totalitemcount = 0;
		//crData *data = playerData->getDataClass();
		//data->getParam(WCHDATA_JXJVipLv, param);
		//unsigned char viplv = *(unsigned char *)param;
		//data->getParam(WCHDATA_JXJLevyCount, param);
		//char levyCount = *(char *)param;
		//crVector2i maxlevycount;
		//crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount, viplv).c_str(), maxlevycount);
		//char totallevycount = maxlevycount[LevyType_Gold] + maxlevycount[LevyType_Free];
		//if (levyCount < totallevycount)
		//{
		//	playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
		//}
	}
}
/////////////////////////////////////////
//
//crJXJTaskSearchMarketExchangeMethod
//
/////////////////////////////////////////
crJXJTaskSearchMarketExchangeMethod::crJXJTaskSearchMarketExchangeMethod()
{
}
crJXJTaskSearchMarketExchangeMethod::crJXJTaskSearchMarketExchangeMethod(const crJXJTaskSearchMarketExchangeMethod& handle) :
crMethod(handle)
{
}
void crJXJTaskSearchMarketExchangeMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJTaskSearchMarketExchangeMethod::addParam(int i, const std::string& str)
{
}
void crJXJTaskSearchMarketExchangeMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			braindata->getParam(WCHDATA_JXJTotalExchangeResCount, param);
			int totalcount = *(int *)param;
			if (totalcount >= 1)
			{
				playerData->doEvent(WCH_JXJAdvanceTask, MAKEINT64(m_this->getTaskID(), 0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUINewGuideCheckMethod
//新手引导检测
//
/////////////////////////////////////////
crJXJUINewGuideCheckMethod::crJXJUINewGuideCheckMethod()
{
}
crJXJUINewGuideCheckMethod::crJXJUINewGuideCheckMethod(const crJXJUINewGuideCheckMethod& handle):
	crMethod(handle)
{
}
void crJXJUINewGuideCheckMethod::inputParam(int i, void *param)
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
void crJXJUINewGuideCheckMethod::addParam(int i, const std::string& str)
{
}

void crJXJUINewGuideCheckMethod::operator()(crHandle &handle)
{
	int usenewguide = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJUseNewGuide,0).c_str());
	if (usenewguide == 0)
	{
		return;
	}
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getMainRole())
	{
		crRole *mainRole = playerData->getMainRole();
		crData *mainroledata = mainRole->getDataClass();
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJSkipNewHand,param);
		bool skipnewhand = *(bool *)param;
		if (skipnewhand)
		{
			return;
		}
		//#define WCHDATA_JXJNewguideStepTab			WCHDATA_JXJ+2317//新手引导执行表
		//#define WCHDATA_JXJNewguideTab				WCHDATA_JXJ+2318//新手引导触发条件表
		//#define WCHDATA_JXJNewguideTextTab			WCHDATA_JXJ+2319//新手引导文本表
		std::string triggercanvasname,triggerbtnname;
		int triggermessage = 0;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO>NewguideStepTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideStepTab);
		ref_ptr<crTableIO>NewguideTextTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTextTab);
		crTableIO::StrVec Step_record, Text_record, itemrecord;
		int itemtypeindex = itemtab->getTitleIndex("类型");
		int cardtypeindex = itemtab->getTitleIndex("将卡类型");
		int Step_stepindex = NewguideStepTab->getTitleIndex("step");
		int Step_canvasindex = NewguideStepTab->getTitleIndex("触发控件父窗口名称");
		int Step_btnindex = NewguideStepTab->getTitleIndex("触发控件名称");
		int Step_coverbtnindex = NewguideStepTab->getTitleIndex("遮罩需聚焦的控件");
		int Step_eventindex = NewguideStepTab->getTitleIndex("点击事件");
		int Step_paramindex = NewguideStepTab->getTitleIndex("参数");
		int Step_coverindex = NewguideStepTab->getTitleIndex("遮罩名称");
		int Step_hyperinputindex = NewguideStepTab->getTitleIndex("超文本输入框名");
		int Step_hypertextindex = NewguideStepTab->getTitleIndex("超文本文件名ID");
		int Step_staticinputindex = NewguideStepTab->getTitleIndex("静态文本输入框名");
		int Step_statictextindex = NewguideStepTab->getTitleIndex("静态文本ID");
		int Step_imgwidgetindex = NewguideStepTab->getTitleIndex("贴图控件名称");
		int Step_imgnameindex = NewguideStepTab->getTitleIndex("贴图文件名");
		int Text_textindex = NewguideTextTab->getTitleIndex("text");
		bool ifguiding = false;
		//检测是否已有引导正在进行
		crData *braindata = crBrain::getInstance()->getDataClass();
		//CRCore::ScopedLock<crData> lock(*braindata);
		braindata->getParam(WCHDATA_JXJNewGuideID,param);
		int curguideid = *(int *)param;
		braindata->getParam(WCHDATA_JXJNewGuideStep,param);
		int curstep = *(int *)param;

		mainroledata->getParam(WCHDATA_JXJNewGuideMap,param);
		JXJNewGuideMap *newguidemap = (JXJNewGuideMap *)param;
		JXJNewGuideMap::iterator newguideitr;
		ref_ptr<crTableIO>NewguideTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTab);
		crTableIO::StrVec Guide_record;
		int Guide_stepcountindex = NewguideTab->getTitleIndex("step总数");
		if (curguideid > 0)
		{//已有引导
			if(NewguideTab->queryOneRecord(0,crArgumentParser::appItoa(curguideid),Guide_record) >= 0)
			{
				newguideitr = newguidemap->find(curguideid);
				if (newguideitr != newguidemap->end())
				{
					newguideitr->second->setStep((curstep));
				}
				int maxstep = atoi(Guide_record[Guide_stepcountindex].c_str());
				if (curstep <= maxstep)
				{//引导仍在进行中
					ifguiding = true;
				}
				else
				{//引导结束
					if(NewguideStepTab->queryOneRecord(0,crArgumentParser::appItoa(curguideid),Step_stepindex,crArgumentParser::appItoa(maxstep),Step_record) >= 0)
					{
						ref_ptr<crCanvasNode> PreCoverCanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_coverindex]);
						if (PreCoverCanvas.valid())
						{
							crFilterRenderManager::getInstance()->closeCanvas(PreCoverCanvas.get());
						}
						triggercanvasname = Step_record[Step_canvasindex];
						ref_ptr<crCanvasNode> PreTriggerCanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_canvasindex]);
						if (triggercanvasname.compare("UI_ManorStair_RenWu") == 0 && PreTriggerCanvas.valid())
						{
							ref_ptr<crMultiSwitch> arrowsw = dynamic_cast<crMultiSwitch *>(PreTriggerCanvas->getChildNode("sw66"));
							if(arrowsw.valid())arrowsw->setActiveSwitchSet(0);
							crFilterRenderManager::getInstance()->showCanvas(PreTriggerCanvas.get(),1);
						}

					}
					curguideid = 0;
					curstep = 0;
				}
			}
		}
		if(!ifguiding)
		{
			//检测是否触发引导
			int Guide_taskidindex = NewguideTab->getTitleIndex("0任务ID");
			int Guide_lordlvindex = NewguideTab->getTitleIndex("1领主等级");
			int Guide_fubenidindex = NewguideTab->getTitleIndex("2进行副本ID");
			int Guide_cfubenidindex = NewguideTab->getTitleIndex("3完成副本ID");
			int Guide_itemidindex = NewguideTab->getTitleIndex("4获得物品ID");
			int Guide_itemcountindex = NewguideTab->getTitleIndex("4获得物品数量");
			int Guide_canvasnameindex = NewguideTab->getTitleIndex("5打开窗口名");
			int Guide_triggertypeindex = NewguideTab->getTitleIndex("触发类型");
			if (mainroledata && mainRole->getMetierDataClass())
			{
				crData *mainData = mainRole->getMetierDataClass();
				CRCore::ScopedLock<crData> lock3(*data);
				CRCore::ScopedLock<crData> lock2(*mainData);
				unsigned short guideid = 0;
				int item_i = 0;
				bool ifbegintrigger;
				std::vector<int> triggertypevec;
				std::vector<int> getitemidvec;
				std::vector<int> getitemcountvec;
				std::vector<int>::iterator itr;
				std::vector<int>::iterator getitemitr;
				FubenProgressMap::iterator fubenitr;
				GameTaskMap::iterator gitr;
				JXJItemBagVec::iterator bagitr;
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				unsigned char fubenorchengchitype = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short curselectfubenid = *(unsigned short *)param;
				mainroledata->getParam(WCHDATA_Level,param);
				unsigned char lordlv = *(unsigned char *)param;
				mainData->getParam(WCHDATA_GameTaskMap,param);
				GameTaskMap *gameTaskMap = (GameTaskMap *)param;
				data->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgressmap = (FubenProgressMap *)param;
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				int activetaskid = 0;
				//int needlordlv = 0;
				crVector2i needlordlv;
				int curfubenid = 0;
				int completedfubenid = 0;
				std::string opencanvasname;
				ref_ptr<crCanvasNode> openCanvas = NULL;//crFilterRenderManager::getInstance()->findCanvas(m_zhezhaoCanvas);
				int rowcount = NewguideTab->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					ifbegintrigger = true;
					triggertypevec.clear();
					getitemidvec.clear();
					crArgumentParser::appAtoVec((*NewguideTab)(i,Guide_triggertypeindex),triggertypevec);
					activetaskid = atoi((*NewguideTab)(i,Guide_taskidindex).c_str());
					//needlordlv = atoi((*NewguideTab)(i,Guide_lordlvindex).c_str());
					crArgumentParser::appAtoVec((*NewguideTab)(i,Guide_lordlvindex),needlordlv);
					curfubenid = atoi((*NewguideTab)(i,Guide_fubenidindex).c_str());
					completedfubenid = atoi((*NewguideTab)(i,Guide_cfubenidindex).c_str());
					crArgumentParser::appAtoVec((*NewguideTab)(i,Guide_itemidindex),getitemidvec);
					crArgumentParser::appAtoVec((*NewguideTab)(i,Guide_itemcountindex),getitemcountvec);
					getitemcountvec.resize(getitemidvec.size());
					opencanvasname = (*NewguideTab)(i,Guide_canvasnameindex);
					for (itr = triggertypevec.begin(); itr != triggertypevec.end(); ++itr)
					{

						switch (*itr)
						{
						case 0://0任务ID
							if (activetaskid > 0)
							{
								for(gitr = gameTaskMap->begin();
									gitr != gameTaskMap->end();
									++gitr )
								{
									if (gitr->first == activetaskid)
									{
										if(!gitr->second->getActivation() || gitr->second->isComplete())
										{
											ifbegintrigger = false;
										}
										break;
									}
								}
							}
							break;
						case 1://1领主等级
							if(lordlv<needlordlv[0] || lordlv>needlordlv[1])
							{
								ifbegintrigger = false;
							}
							break;
						case 2://2进行副本ID
							if (fubenorchengchitype == FT_Fuben)
							{
								if (curfubenid != curselectfubenid)
								{
									ifbegintrigger = false;
								}
							}
							else
							{
								ifbegintrigger = false;
							}
							break;
						case 3://3完成副本ID
							fubenitr = fubenProgressmap->find(completedfubenid);
							if (fubenitr != fubenProgressmap->end())
							{
								if(fubenitr->second->getComplete() <= 0)
								{
									ifbegintrigger = false;
								}
							}
							else
							{
								ifbegintrigger = false;
							}
							break;
						case 4://4获得物品ID
							for (bagitr = itembagvec->begin();
								bagitr != itembagvec->end();
								++bagitr)
							{
								if ((*bagitr).get())
								{
									for (item_i = 0;item_i < getitemidvec.size();++item_i)
									{
										if (getitemcountvec[item_i] > 0)
										{
											if((*bagitr)->getItemID() == getitemidvec[item_i])
											{
												getitemcountvec[item_i] -= (*bagitr)->getItemCount();
												if (getitemcountvec[item_i] < 0)
												{
													getitemcountvec[item_i] = 0;
												}
											}
										}
									}
								}
							}
							for (item_i = 0;item_i < getitemcountvec.size();++item_i)
							{
								if (getitemcountvec[item_i] > 0)
								{
									ifbegintrigger = false;
									break;
								}
							}
							break;
						case 5://5打开窗口名
							openCanvas = crFilterRenderManager::getInstance()->findCanvas(opencanvasname);
							if (!openCanvas.valid() || !openCanvas->getVisiable())
							{
								ifbegintrigger = false;
							}
							break;
						default:
							break;
						}
						if (!ifbegintrigger)
						{
							break;
						}
					}
					if (ifbegintrigger)
					{
						guideid = atoi((*NewguideTab)(i, 0).c_str());
						//检测是否已经引导过
						newguideitr = newguidemap->find((unsigned short)guideid);
						if (newguideitr != newguidemap->end() && newguideitr->second->isComplete())
						{
							curguideid = 0;
							curstep = 0;
						}
						else
						{
							curguideid = guideid;
							curstep = 1;
							break;
						}
					}
				}
			}
		}
		if((curguideid) > 0)
		{
			if ((curstep) > 1)
			{
				Step_record.clear();
				if(NewguideStepTab->queryOneRecord(0,crArgumentParser::appItoa(curguideid),Step_stepindex,crArgumentParser::appItoa((curstep) - 1),Step_record) >= 0)
				{
					ref_ptr<crCanvasNode> PreCoverCanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_coverindex]);
					if (PreCoverCanvas.valid())
					{
						crFilterRenderManager::getInstance()->closeCanvas(PreCoverCanvas.get());
					}
					triggercanvasname = Step_record[Step_canvasindex];
					ref_ptr<crCanvasNode> PreTriggerCanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_canvasindex]);
					if (triggercanvasname.compare("UI_ManorStair_RenWu") == 0 && PreTriggerCanvas.valid())
					{
						ref_ptr<crMultiSwitch> arrowsw = dynamic_cast<crMultiSwitch *>(PreTriggerCanvas->getChildNode("sw66"));
						if(arrowsw.valid())arrowsw->setActiveSwitchSet(0);
						crFilterRenderManager::getInstance()->showCanvas(PreTriggerCanvas.get(),1);
					}
					CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
					if(scene)
					{
						crInstanceItem *item  = scene->findSceneItem(atoi(Step_record[Step_coverbtnindex].c_str()));
						if(item)
						{
							item->doEvent(WCH_JXJBuildingTaskEffect,MAKEINT64(0,NULL));
						}
					}
				}
			}
			//显示引导
			Step_record.clear();
			if(NewguideStepTab->queryOneRecord(0,crArgumentParser::appItoa(curguideid),Step_stepindex,crArgumentParser::appItoa(curstep),Step_record) >= 0)
			{
				triggercanvasname = Step_record[Step_canvasindex];
				if (triggercanvasname.compare("closeallcanvas") == 0)
				{//关闭所有窗口
					crFilterRenderManager::getInstance()->closeAllCanvas();
					(curstep) += 1;
				}
				else if(triggercanvasname.compare("closecanvas") == 0)
				{//关闭指定窗口
					std::string closename = Step_record[Step_paramindex];
					ref_ptr<crCanvasNode> CloseCanvas = crFilterRenderManager::getInstance()->findCanvas(closename);
					if (CloseCanvas.valid() && CloseCanvas->getVisiable())
					{
						crFilterRenderManager::getInstance()->showCanvas(CloseCanvas.get(),0);
					}
					(curstep) += 1;
				}
				else
				{
					triggerbtnname = Step_record[Step_btnindex];
					triggermessage = atoi(Step_record[Step_eventindex].c_str());
					braindata->inputParam(WCHDATA_JXJNewGuideCanvas,&triggercanvasname);
					braindata->inputParam(WCHDATA_JXJNewGuideBtn,&triggerbtnname);
					braindata->inputParam(WCHDATA_JXJNewGuideBtnMessage,&triggermessage);
					ref_ptr<crCanvasNode> TriggerCanvas = crFilterRenderManager::getInstance()->findCanvas(triggercanvasname);
					if (triggercanvasname.compare("UI_ManorStair_RenWu") == 0 && TriggerCanvas.valid())
					{
						//ref_ptr<crMultiSwitch> arrowsw = dynamic_cast<crMultiSwitch *>(TriggerCanvas->getChildNode("sw66"));
						//if(arrowsw.valid())arrowsw->setActiveSwitchSet(1);
						crFilterRenderManager::getInstance()->doModal(triggercanvasname);
					}
					ref_ptr<crCanvasNode> CoverCanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_coverindex]);
					if (CoverCanvas.valid() && !CoverCanvas->getVisiable())
					{
						ref_ptr<crHypertextWidgetNode> hypertext = dynamic_cast<crHypertextWidgetNode *>(CoverCanvas->getWidget(Step_record[Step_hyperinputindex]));
						if (hypertext.valid())
						{//超文本框文字
							if(NewguideTextTab->queryOneRecord(0, Step_record[Step_hypertextindex],Text_record) >= 0)
							{
								std::string file = Text_record[Text_textindex];
								file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
								hypertext->setHypertext(file);
							}
						}
						ref_ptr<crStaticTextWidgetNode> statictext = dynamic_cast<crStaticTextWidgetNode *>(CoverCanvas->getWidget(Step_record[Step_staticinputindex]));
						if (statictext.valid())
						{//静态文本框文字
							if(NewguideTextTab->queryOneRecord(0, Step_record[Step_statictextindex],Text_record) >= 0)
							{
								std::string text = Text_record[Text_textindex];
								statictext->setString(text);
							}
						}
						if (TriggerCanvas.valid())
						{//遮罩聚焦到相应的控件上
							ref_ptr<crListControlWidgetNode> Triggerlist = dynamic_cast<crListControlWidgetNode *>(TriggerCanvas->getWidget(Step_record[Step_coverbtnindex]));

							if (Triggerlist.valid())
							{//是否是list，是的话需要找出需要聚焦的那个ID.
								int selectindex = 0;
								int itemselectindex = 0;
								crListControlWidgetNode::ListNodeVec& Triggerlistvec = Triggerlist->getListNodeVec();
								std::vector<int> paramvec;
								crArgumentParser::appAtoVec(Step_record[Step_paramindex],paramvec);
								if (paramvec[0] == T_Troops)
								{
									if (paramvec.size() > 1)
									{
										for (int i = 0; i < Triggerlistvec.size(); ++i)
										{
											if (Triggerlistvec[i]->getData() == paramvec[1])
											{
												selectindex = i;
												itemselectindex = selectindex;
												break;
											}
										}
									}
								}
								else if (paramvec[0] == T_Item)
								{
									int cardselectindex = 0;
									int bagselectindex = 0;
									ref_ptr<crCanvasNode> bagcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Knapsack");
									ref_ptr<crCanvasNode> cardcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Jk");
									if (cardcanvas.valid())
									{
										ref_ptr<crRadioGroupWidgetNode> TcjkRaido2 = dynamic_cast<crRadioGroupWidgetNode *>(cardcanvas->getWidget("TcjkRaido2"));
										if (TcjkRaido2.valid())
										{
											cardselectindex = TcjkRaido2->getSelect();
										}
									}
									if (bagcanvas.valid())
									{
										ref_ptr<crRadioGroupWidgetNode> KnapsackRaido = dynamic_cast<crRadioGroupWidgetNode *>(bagcanvas->getWidget("UI_KnapsackRaido"));
										if (KnapsackRaido.valid())
										{
											bagselectindex = KnapsackRaido->getSelect();
										}
									}
									int itemindex = 0;
									int onepagecount = 0;
									if (paramvec.size() > 1)
									{
										if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(paramvec[1]), itemrecord) >= 0)
										{
											int itemid = 0;
											int cardtype = atoi(itemrecord[cardtypeindex].c_str());
											int itemtype = atoi(itemrecord[itemtypeindex].c_str());
											JXJItemBagVec *bagvec;
											if (itemtype == IT_Jiangka)
											{
												//ref_ptr<crCanvasNode>formationcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Beta2_Formation");
												if (cardcanvas.valid())
												{
													crData * formationdata = cardcanvas->getDataClass();
													onepagecount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJiangkaBagOnePageNum, 0).c_str()));
													formationdata->getParam(WCHDATA_JXJForJKVec, param);
													bagvec = (JXJItemBagVec *)param;
												}
											}
											else
											{
												onepagecount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagOnePage, 0).c_str()));
												data->getParam(WCHDATA_JXJItemBagVec, param);
												bagvec = (JXJItemBagVec *)param;
											}
											for (JXJItemBagVec::iterator itemitr = bagvec->begin(); itemitr != bagvec->end(); ++itemitr)
											{
												if (itemitr->get())
												{
													itemid = (*itemitr)->getItemID();
													if (itemid == paramvec[1] && (*itemitr)->getItemCount() > 0)
													{
														selectindex = itemindex;
														break;
													}
													if (itemtype == IT_Jiangka)
													{
														itemrecord.clear();
														if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), itemrecord) >= 0)
														{
															if (cardselectindex != 0 && cardtype != atoi(itemrecord[cardtypeindex].c_str()))
															{
																//itemindex--;
																continue;
															}
														}
													}
													else
													{
														itemrecord.clear();
														if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), itemrecord) >= 0)
														{
															if (bagselectindex != 0 && itemtype != atoi(itemrecord[itemtypeindex].c_str()))
															{
																//itemindex--;
																continue;
															}
														}
													}
													++itemindex;
												}
											}
											//检测该物品在背包第几页
											crData *brainData = crBrain::getInstance()->getDataClass();
											int pagenumindex = 0;
											if (selectindex >= 0)
											{
												pagenumindex = (selectindex + 1) / onepagecount;
												if (itemtype == IT_Jiangka)
												{
													pagenumindex += 1;
													if (brainData)
													{
														brainData->inputParam(WCHDATA_JXJCurJiangkaBagPageNum, &pagenumindex);
														if (cardcanvas.valid())cardcanvas->doEvent(WCH_UI_UpdateData);
													}
												}
												else
												{
													if (bagcanvas.valid())
													{
														ref_ptr<crRadioGroupWidgetNode> pagenumradio = dynamic_cast<crRadioGroupWidgetNode *>(bagcanvas->getWidget("UI_KnapsackRaido2"));
														if (pagenumradio.valid())
														{
															pagenumradio->select(pagenumindex);
															bagcanvas->doEvent(WCH_UI_UpdateData);
														}
													}
												}
												itemselectindex = selectindex;
												selectindex = selectindex % onepagecount;
											}
										}
									}
								}
								braindata->getParam(WCHDATA_JXJNewGuideBtnParamVec, param);
								std::vector<int> *newguideparamvec = (std::vector<int> *)param;
								newguideparamvec->clear();
								newguideparamvec->push_back(itemselectindex);
								Triggerlist->select(selectindex);
								crListControlNode *selectNode = Triggerlist->getSelectNode();
								if (selectNode)
								{
									crVector4f rect = selectNode->getRect();
									CRCore::crMatrixTransform *m_mvpwNode = Triggerlist->getMvpwNode();
									const crMatrix &mat = m_mvpwNode->getMatrix() * TriggerCanvas->getMatrix();
									//crVector3 pos = Triggerlistvec[i]->getBound().center() ;
									crVector3 pos(0.5f*rect[2] + rect[0], rect[1] - 0.5f*rect[3], 0.0f);
									pos = pos * mat;
									CoverCanvas->setPosition(pos);
								}
							}
							else
							{
								ref_ptr<crWidgetNode> TriggerWidget = dynamic_cast<crWidgetNode *>(TriggerCanvas->getWidget(Step_record[Step_coverbtnindex]));
								if (TriggerWidget.valid())
								{
									crMatrix mat = TriggerCanvas->getMatrix();
									crVector3 pos = TriggerWidget->getBound().center();
									pos = pos * mat;
									CoverCanvas->setPosition(pos);
								}
							}
						}
						else
						{
							CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
							if(scene)
							{
								crInstanceItem *item  = scene->findSceneItem(atoi(Step_record[Step_coverbtnindex].c_str()));
								if(item)
								{
									CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
									item->doEvent(WCH_JXJBuildingTaskEffect,MAKEINT64(1,NULL));
									crVector3 itempos = item->getPosition();
									itempos[2]+=item->getRelNode()->getBoundBox().zLength();
									if(crBrain::getInstance()->getCameraFrustum().contains(itempos))
									{
										crVector3 window;
										if (bindview && bindview->isInited())
										{
											bindview->getDefaultSceneHandler()->getSceneView()->projectObjectIntoWindow(itempos,window);
											itempos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
											itempos[2] = 0.0f;
											CoverCanvas->setPosition(itempos);
										}
									}
								}
							}
						}
						//ref_ptr<crCanvasNode> zhezhaocanvas = crFilterRenderManager::getInstance()->findCanvas(Step_record[Step_coverindex]);
						CoverCanvas->setCanvasPos(CANVAS_TOPMOST);
						crFilterRenderManager::getInstance()->doModal(Step_record[Step_coverindex]);
					}
				}
			}
		}
		braindata->inputParam(WCHDATA_JXJNewGuideID,&curguideid);
		braindata->inputParam(WCHDATA_JXJNewGuideStep,&curstep);
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideBtnTouchMethod
//
/////////////////////////////////////////
crJXJNewGuideBtnTouchMethod::crJXJNewGuideBtnTouchMethod():
	m_this(NULL),
	m_time(0L)
{
}
crJXJNewGuideBtnTouchMethod::crJXJNewGuideBtnTouchMethod(const crJXJNewGuideBtnTouchMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_time(0L)
{
}
void crJXJNewGuideBtnTouchMethod::inputParam(int i, void *param)
{
}
void crJXJNewGuideBtnTouchMethod::addParam(int i, const std::string& str)
{
}
void crJXJNewGuideBtnTouchMethod::operator()(crHandle &handle)
{
	CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	if(m_time == 0)
	{
		m_time = t1;
	}
	else
	{
		float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
		if(dt<0.5f)
		{
			return;
		}
		m_time = t1;
	}
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *braindata = crBrain::getInstance()->getDataClass();
		//CRCore::ScopedLock<crData> lock(*braindata);
		braindata->getParam(WCHDATA_JXJNewGuideCanvas,param);
		std::string newguidecanvas = *(std::string *)param;
		braindata->getParam(WCHDATA_JXJNewGuideBtn,param);
		std::string newguidebtn = *(std::string *)param;
		braindata->getParam(WCHDATA_JXJNewGuideBtnMessage,param);
		int newguidemessage = *(int *)param;
		ref_ptr<crCanvasNode> TriggerCanvas = crFilterRenderManager::getInstance()->findCanvas(newguidecanvas);
		if (TriggerCanvas.valid())
		{
			if(!TriggerCanvas->getVisiable())
				return;
			ref_ptr<crListControlWidgetNode> Triggerlist = dynamic_cast<crListControlWidgetNode *>(TriggerCanvas->getWidget(newguidebtn));
			ref_ptr<crHypertextWidgetNode> Triggerhypertext = dynamic_cast<crHypertextWidgetNode *>(TriggerCanvas->getWidget(newguidebtn));
			if (Triggerlist.valid())
			{//是否是list，是的话需要定位点击事件
				braindata->excHandle(MAKEINT64(WCH_LockData,1));
				braindata->getParam(WCHDATA_JXJNewGuideBtnParamVec,param);
				std::vector<int> *newguideparamvec = (std::vector<int> *)param;
				if (newguideparamvec->size() > 0)
				{
					Triggerlist->select((*newguideparamvec)[0]);
					switch (newguidemessage)
					{
					case 0:
						Triggerlist->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						break;
					case 1:
						Triggerlist->doEvent(MAKEINT32(crGUIEventAdapter::RIGHT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						break;
					case 2:
						Triggerlist->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::DOUBLECLICK));
						break;
					default:
						break;
					}
				}
				braindata->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else if (Triggerhypertext.valid())
			{
				crFilterRenderManager::getInstance()->showCanvas(TriggerCanvas.get(),true);
				if (newguidecanvas.compare("UI_ManorStair_RenWu") == 0)
				{
					ref_ptr<crMultiSwitch> arrowsw = dynamic_cast<crMultiSwitch *>(TriggerCanvas->getChildNode("sw66"));
					if(arrowsw.valid())arrowsw->setActiveSwitchSet(0);
				}
			}
			else
			{
				ref_ptr<crWidgetNode> TriggerWidget = dynamic_cast<crWidgetNode *>(TriggerCanvas->getWidget(newguidebtn));
				if (TriggerWidget.valid())
				{
					switch (newguidemessage)
					{
					case 0:
						TriggerWidget->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						if (dynamic_cast<crRadioWidgetNode *>(TriggerWidget.get()))
						{
							(dynamic_cast<crRadioWidgetNode *>(TriggerWidget.get()))->select(true);
						}
						break;
					case 1:
						TriggerWidget->doEvent(MAKEINT32(crGUIEventAdapter::RIGHT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						break;
					case 2:
						TriggerWidget->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::DOUBLECLICK));
						break;
					default:
						break;
					}
				}
				else
				{
					TriggerCanvas->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
			}
		}
		else
		{
			CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
			if(scene)
			{
				crInstanceItem *item = scene->findSceneItem(atoi(newguidecanvas.c_str()));
				if(item)
				{
					item->doEvent(WCH_Touch);
				}
			}
		}
		braindata->excHandle(MAKEINT64(WCH_LockData,1));
		braindata->getParam(WCHDATA_JXJNewGuideStep,param);
		int *curstep = (int *)param;
		*curstep += 1;
		braindata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideHyperTextTouchMethod
//
/////////////////////////////////////////
crJXJNewGuideHyperTextTouchMethod::crJXJNewGuideHyperTextTouchMethod():
	m_this(NULL)
{
}
crJXJNewGuideHyperTextTouchMethod::crJXJNewGuideHyperTextTouchMethod(const crJXJNewGuideHyperTextTouchMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJNewGuideHyperTextTouchMethod::inputParam(int i, void *param)
{
}
void crJXJNewGuideHyperTextTouchMethod::addParam(int i, const std::string& str)
{
}
void crJXJNewGuideHyperTextTouchMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *braindata = crBrain::getInstance()->getDataClass();
		CRCore::ScopedLock<crData> lock(*braindata);
		braindata->getParam(WCHDATA_JXJNewGuideID,param);
		int curid = *(int *)param;
		if (curid > 0)
		{
			braindata->getParam(WCHDATA_JXJNewGuideStep,param);
			int *curstep = (int *)param;
			*curstep += 1;
			braindata->inputParam(WCHDATA_JXJNewGuideEmptyHyperText,0);
		}
	}
}
/////////////////////////////////////////
//
//crJXJNewGuideEmptyHyperTextTouchMethod
//
/////////////////////////////////////////
crJXJNewGuideEmptyHyperTextTouchMethod::crJXJNewGuideEmptyHyperTextTouchMethod():
	m_this(NULL)
{
}
crJXJNewGuideEmptyHyperTextTouchMethod::crJXJNewGuideEmptyHyperTextTouchMethod(const crJXJNewGuideEmptyHyperTextTouchMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJNewGuideEmptyHyperTextTouchMethod::inputParam(int i, void *param)
{
}
void crJXJNewGuideEmptyHyperTextTouchMethod::addParam(int i, const std::string& str)
{
}
void crJXJNewGuideEmptyHyperTextTouchMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *braindata = crBrain::getInstance()->getDataClass();
		CRCore::ScopedLock<crData> lock(*braindata);
		braindata->getParam(WCHDATA_JXJNewGuideEmptyHyperText,param);
		int *emptyhypertext = (int *)param;
		if ((*emptyhypertext) > 0)
		{
			braindata->getParam(WCHDATA_JXJNewGuideID,param);
			int curid = *(int *)param;
			if (curid > 0)
			{
				braindata->getParam(WCHDATA_JXJNewGuideStep,param);
				int *curstep = (int *)param;
				*curstep += 1;
			}
		}
		else
		{
			(*emptyhypertext) = 1;
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvNewguideCompleteMethod
//
/////////////////////////////////////////
crJXJRecvNewguideCompleteMethod::crJXJRecvNewguideCompleteMethod():
	m_netType(GameClient_Game){}
crJXJRecvNewguideCompleteMethod::crJXJRecvNewguideCompleteMethod(const crJXJRecvNewguideCompleteMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvNewguideCompleteMethod::inputParam(int i, void *param)
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

void crJXJRecvNewguideCompleteMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvNewguideCompleteMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				unsigned short accid = m_stream->_readShort();

				bool suc = false;
				void *param;
				crData *data = mainRole->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJNewGuideMap,param);
				JXJNewGuideMap *newguidemap = (JXJNewGuideMap *)param;
				JXJNewGuideMap::iterator itr = newguidemap->find(accid);
				if(itr != newguidemap->end())
				{
					//if(!(itr->second->isComplete()))
					//{//
					suc = true;
					itr->second->setComplete(true);
					//返回信息
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(2);
					stream->_writeUShort(accid);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvNewguideComplete,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					//}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));

				// 将新手引导id放入国家列表中
				crData *playerGameData = m_this->getDataClass();

				playerGameData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));
				if (shiliid >= c_startShiliID)
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData * shiliData = shili->getDataClass();
					CRCore::ScopedLock<crData> lock(*shiliData);

					bool isModify = false;
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap * peopleMap = (PeopleMap *)param;
					PeopleMap::iterator pmItr = peopleMap->find(playerid);
					if (pmItr != peopleMap->end())
					{
						pmItr->second->setNewrStep(accid);
						//pmItr->second->m_newerStep = accid;
					}
					else
					{
						CRCore::ref_ptr<PeopleInfo> item = new PeopleInfo(m_this->getPlayerID(),m_this->getCharacterName(),shiliid);							
						item->needInsert();
						item->setNewrStep(accid);
						//item->m_newerStep = accid;
						// 其余信息会在下线时同步
						peopleMap->insert(PeopleMap::value_type(item->getID(),item));
					}
					if(isModify) shiliData->inputParam(WCHDATA_JXJShiliModify,&isModify);
				}
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crRole *mainRole = m_this->getMainRole();
			if(mainRole && mainRole->getDataClass())
			{
				unsigned short accid = m_stream->_readUShort();
				void *param;
				crData *data = mainRole->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJNewGuideMap,param);
				JXJNewGuideMap *newguidemap = (JXJNewGuideMap *)param;
				JXJNewGuideMap::iterator itr = newguidemap->find(accid);
				if(itr != newguidemap->end())
				{
					itr->second->setComplete(true);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJChangeDataDisposeMethod
//
/////////////////////////////////////////
crJXJChangeDataDisposeMethod::crJXJChangeDataDisposeMethod():
	m_this(NULL),
	m_count(50){}
crJXJChangeDataDisposeMethod::crJXJChangeDataDisposeMethod(const crJXJChangeDataDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_count(handle.m_count)
{
}
void crJXJChangeDataDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}
void crJXJChangeDataDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	}
}
void crJXJChangeDataDisposeMethod::operator()(crHandle &handle)
{//提取需要修改的玩家数据并进行修改
	int changetype = 0;
	int itemid = 0;
	int count = 0;
	bool success = false;
	void *param;
	int playerid = 0;
	int id = 0;
	int newcount = 0;
	int gmid = 0;
	std::string information;
	ref_ptr<crGameServerPlayerData> playerData = NULL;
	ref_ptr<crGameServerPlayerData> playerData2 = NULL;
	crData *data,*mainroledata;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	if (!gameServer)
	{
		return;
	}
	crNetManager *netManager = gameServer->getNetManager();
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	int serverid = callback->getServerID();
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
	ref_ptr<crDataBase> accountSession;
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crPlayerdataChangeListQueryData> playerdatachangelistQuery = new crPlayerdataChangeListQueryData;
	ref_ptr<crJXJLockPlayerdata> lockPlayerData = new crJXJLockPlayerdata;
	ref_ptr<crJXJUnLockPlayerdata> unlockPlayerData = new crJXJUnLockPlayerdata;
	playerdatachangelistQuery->buildQuerySql(serverid,m_count);
	globalSession->executeQuery(playerdatachangelistQuery.get());
	crDataBase::QueryResultVec playerdatachangelistQueryResultVec = globalSession->getQueryResult();
	globalSession->releaseQuery();
	db->endSession(globalSession.get());
	if(!playerdatachangelistQueryResultVec.empty())
	{
		crPlayerDataEventPacket packet;
		ref_ptr<crStreamBuf>stream;
		MoneyChangeData  moneydata;//
		//ref_ptr<crUpdateRechargerecord> updateRechargerecord = new crUpdateRechargerecord;
		ref_ptr<crJXJRemovePlayerdataChangeList> removDatachangelist = new crJXJRemovePlayerdataChangeList;
		ref_ptr<crJXJInsertPlayerdataChangeRecord> insertDatachangerecord = new crJXJInsertPlayerdataChangeRecord;
		for( crDataBase::QueryResultVec::iterator itr = playerdatachangelistQueryResultVec.begin();
			itr != playerdatachangelistQueryResultVec.end();
			++itr )
		{
			success = false;
			playerdatachangelistQuery = dynamic_cast<crPlayerdataChangeListQueryData *>(itr->get());
			changetype = playerdatachangelistQuery->getType();
			itemid = playerdatachangelistQuery->getItemID();
			count = playerdatachangelistQuery->getCount();
			playerid = playerdatachangelistQuery->getPlayerID();
			id = playerdatachangelistQuery->getID();
			gmid = playerdatachangelistQuery->getGmid();
			information = playerdatachangelistQuery->getInformation();
			//
			accountSession = accountdb->beginSession();
			lockPlayerData->buildUpdateSql(playerid);
			if(accountSession->executeUpdate(lockPlayerData.get()))
			{
				accountSession->commit();
			}
			else
			{
				accountSession->rollback();
			}
			accountdb->endSession(accountSession.get());
			newcount = count;

			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (!playerData.valid())
			{
				playerData = getPlayerData(playerid);
			}
			//
			if (playerData.valid() && playerData->getPlayerGameData()->getMainRole())
			{
				data = playerData->getPlayerGameData()->getDataClass();
				CRCore::ScopedLock<crData> lock(*data);
				mainroledata = playerData->getPlayerGameData()->getMainRole()->getDataClass();
				//data->excHandle(MAKEINT64(WCH_LockData,1));
				if (changetype == T_Giftgold)
				{
					moneydata.first = count;
					moneydata.second = "GM修改礼金";
					playerData->getPlayerGameData()->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
				}
				else if (changetype == T_GroupID)
				{
					m_this->doEvent(WCH_JXJKictOutGroupMemberByGM, MAKEINT64(playerid,NULL));
				}
				else if (changetype == T_Item)
				{
					if (count > 0)
					{
						RewardItemVec rewardItems;
						ref_ptr<crBagItemData> newitem =new crBagItemData;
						newitem->setItemID(itemid);
						newitem->setItemCount(count);
						rewardItems.push_back(newitem);
						playerData->getPlayerGameData()->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
					}
					else if (count < 0)
					{
						DeductItemMap deductitemmap;
						ref_ptr<crBagItemData> deductitem;
						deductitem =new crBagItemData;
						deductitem->setItemID(itemid);
						deductitem->setItemDeductCount(-count);
						deductitemmap.insert(std::make_pair(itemid,deductitem));
						playerData->getPlayerGameData()->doEvent(WCH_JXJRecvDeductItems,MAKEINT64(&deductitemmap,0));
					}
				}
				else if(changetype == T_Troops)
				{
					data->getParam(WCHDATA_JXJTroopsMap,param);
					TroopsMap *troopsMap = (TroopsMap *)param;
					TroopsMap::iterator titr = troopsMap->find(itemid);
					if (titr != troopsMap->end())
					{
						titr->second += count;
						if (titr->second < 0)
						{
							titr->second = 0;
						}
					}
					else
					{
						if (count > 0)
						{
							troopsMap->insert(std::make_pair(itemid,count));
						}
					}
				}
				else
				{
					int datatype = 0;
					param = NULL;
					switch (changetype)
					{
					case T_Copper:
						data->getParam(WCHDATA_JXJCoppercash,param);
						datatype = 0;
						break;
					case T_Food:
						data->getParam(WCHDATA_JXJFood,param);
						datatype = 0;
						break;
					case T_Wood:
						data->getParam(WCHDATA_JXJWood,param);
						datatype = 0;
						break;
					case T_Iron:
						data->getParam(WCHDATA_JXJIron,param);
						datatype = 0;
						break;
					case T_Horse:
						data->getParam(WCHDATA_JXJHorse,param);
						datatype = 0;
						break;
					case T_Exp:
						mainroledata->getParam(WCHDATA_Experience,param);
						datatype = 0;
						break;
					case T_Achievement:
						mainroledata->getParam(WCHDATA_JXJAchievement,param);
						datatype = 0;
						break;
					//case T_Giftgold:
					//	data->getParam(WCHDATA_JXJGiftGold,param);
					//	datatype = 0;
					//	break;
					case T_Exploit:
						mainroledata->getParam(WCHDATA_JXJExploit,param);
						datatype = 0;
						break;
					case T_Contribute:
						data->getParam(WCHDATA_JXJCountryContribute,param);
						datatype = 0;
						break;
					case T_Gold:
						data->getParam(WCHDATA_Money,param);
						datatype = 0;
						break;
					case T_JiangHun:
						data->getParam(WCHDATA_JXJCardPieces,param);
						datatype = 0;
						break;
					case T_CardImpression:
						data->getParam(WCHDATA_JXJCardImpression,param);
						datatype = 1;
						break;
					case T_JunLing:
						count *= 100;
						data->getParam(WCHDATA_JXJJunling,param);
						datatype = 1;
						break;
					case T_Level:
						mainroledata->getParam(WCHDATA_Level,param);
						datatype = 2;
						break;
					case T_Panguo:
						data->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
						datatype = 2;
						break;
					case T_Govpost:
						mainroledata->getParam(WCHDATA_JXJGovPost, param);
						datatype = 2;
						break;
					case T_PlayerTotalPayGold:
						data->getParam(WCHDATA_JXJPlayerTotalPayGold, param);
						datatype = 0;
						break;
					default:
						break;
					}
					if (param)
					{
						if (datatype == 0)
						{
							int *curmoney = (int *)param;
							INTLIMITADD(*curmoney,count,INT_MAX);
							newcount = (*curmoney);
						}
						else if (datatype == 1)
						{
							unsigned short *curmoney2 = (unsigned short *)param;
							INTLIMITADD(*curmoney2,count,USHRT_MAX);
							newcount = (*curmoney2);
						}
						else if (datatype == 2)
						{
							unsigned char *curmoney3 = (unsigned char *)param;
							INTLIMITADD(*curmoney3,count,UCHAR_MAX);
							newcount = (*curmoney3);
						}
					}
				}
				//data->excHandle(MAKEINT64(WCH_LockData,0));
				success = true;
				//if(/*globalSession->executeUpdate(insertDatachangerecord.get()) && */globalSession->executeUpdate(removDatachangelist.get()))
				//{
				//	globalSession->commit();
				//}
				//else
				//{
				//	globalSession->rollback();
				//}
				if (success && changetype != T_Giftgold)
				{//成功
					playerData2 = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
					if(playerData2.valid() && playerData2->getPlayerGameData() && playerData2->getPlayerGameData()->getDataClass())
					{//玩家在线
						stream = new crStreamBuf;
						stream->createBuf(12);
						stream->_writeInt(changetype);
						stream->_writeInt(itemid);
						stream->_writeInt(newcount);
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeData,stream.get());
						netManager->sendPacket(playerData2->getPlayerConnectServerAddress(),packet);
					}
				}
			}
			globalSession = db->beginSession();
			insertDatachangerecord->buildUpdateSql(playerid, serverid, gmid, changetype, itemid, count, information);
			removDatachangelist->buildUpdateSql(id);
			if(globalSession->executeUpdate(insertDatachangerecord.get()) && globalSession->executeUpdate(removDatachangelist.get()))
			{
				globalSession->commit();
			}
			else
			{
				globalSession->rollback();
			}
			db->endSession(globalSession.get());
			if (playerid > 0 && changetype == T_Gold && playerData.valid() && playerData->getPlayerGameData() && success && data && count > 0)
			{
				changeMoney(playerid,count,playerData->getPlayerGameData()->getCharacterName(),playerData->getPlayerGameData()->getAdvertid());
				if (itemid > 0)
				{
					data -> getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					int lvToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRMBToVipExpFactor,viplv).c_str());
					int getvipexp = count * lvToVipExpFactor;
					if (getvipexp < 0)getvipexp = 0;
					playerData->getPlayerGameData()->doEvent(WCH_JXJAddVipExp,MAKEINT64(getvipexp,NULL));
				}
			}
			if (playerData.valid())
			{
				callback->savePlayerData(playerData.get());
				//解锁玩家
				crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
				unlockPlayerData->buildUpdateSql(playerid);
				crDBSavePacket packet;
				crDBSavePacket::buildRequestPacket(packet,unlockPlayerData.get(),-1);
				dbConductor->getNetManager()->sendPacket("all",packet);
			} 
		}
	}	
}
bool crJXJChangeDataDisposeMethod::changeMoney(int playerid, int changecount,const std::string &name,const std::string &advertid)
{
	bool success = false;
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if(dbConductor)
	{
		int dbmoney = 0;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetManager *netManager = gameServer->getNetManager();
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int serverid = callback->getServerID();
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
		ref_ptr<crUpdatePlayerMoney> updatePlayerMoney = new crUpdatePlayerMoney;
		ref_ptr<crInsertPlayerMoney> insertPlayerMoney = new crInsertPlayerMoney;
		//ref_ptr<crJXJInsertMoneyrecord> insertMoneyrecord = new crJXJInsertMoneyrecord;
		playerMoneyQuery->buildQuerySql(playerid,serverid);
		globalSession->executeQuery(playerMoneyQuery.get());
		crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
		if(!playerMoneyQueryResultVec.empty())
		{
			playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
			dbmoney = playerMoneyQuery->getMoney();
			globalSession->releaseQuery();
			updatePlayerMoney->buildUpdateSql(playerid,serverid,changecount);
			//insertMoneyrecord->buildUpdateSql(playerid,serverid,changecount,dbmoney,"GM充值","","");
			if(globalSession->executeUpdate(updatePlayerMoney.get())/* && globalSession->executeUpdate(insertMoneyrecord.get())*/)
			{//插入成功
				globalSession->commit();
				success = true;
			}
			else
			{
				globalSession->rollback();
			}
		}
		else
		{
			globalSession->releaseQuery();
			insertPlayerMoney->buildUpdateSql(playerid,serverid,changecount);
			if(globalSession->executeUpdate(insertPlayerMoney.get()))
			{//插入成功
				globalSession->commit();
				success = true;
			}
			else
			{
				globalSession->rollback();
			}
		}
		db->endSession(globalSession.get());
		if(success)
		{
			std::string consortianame;//军团名称
			crServerBrainHandle *gsBrain = crServerBrainHandle::getInstance();
			gsBrain->doEvent(WCH_JXJGetConsortiaName,MAKEINT64(playerid,&consortianame));
			crDBSavePacket dbpacket;
			ref_ptr<crJXJInsertMoneyrecord> insertMoneyrecord = new crJXJInsertMoneyrecord;
			insertMoneyrecord->buildUpdateSql(playerid,serverid,changecount,dbmoney,"GM充值",consortianame,name,advertid,"");
			crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertMoneyrecord.get());
			dbConductor->getNetManager()->sendPacket("all",dbpacket);
		}
	}
	return success;
}
CRCore::ref_ptr<crGameServerPlayerData> crJXJChangeDataDisposeMethod::getPlayerData(int playerid)
{
	ref_ptr<crGameServerPlayerData> playerData = NULL;
	playerData = new crGameServerPlayerData;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	playerData->setPlayerID(playerid);
	unsigned short gamedbid = 1;
	bool found = false;
	int money = 0;
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crPlayerGameDBQueryData> playergamedbQuery = new crPlayerGameDBQueryData;
	playergamedbQuery->buildQuerySql(playerid,netCallback->getServerID());

	globalSession->executeQuery(playergamedbQuery.get());
	crDataBase::QueryResultVec &playergamedbQueryResultVec = globalSession->getQueryResult();
	if(!playergamedbQueryResultVec.empty())
	{
		playergamedbQuery = dynamic_cast<crPlayerGameDBQueryData *>(playergamedbQueryResultVec[0].get());
		gamedbid = playergamedbQuery->getGameDBID();
		found = true;
	}
	globalSession->releaseQuery();

	if(found)
	{
		ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
		playerMoneyQuery->buildQuerySql(playerid,netCallback->getServerID());

		globalSession->executeQuery(playerMoneyQuery.get());
		crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
		if(!playerMoneyQueryResultVec.empty())
		{
			playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
			money = playerMoneyQuery->getMoney();
		}
		globalSession->releaseQuery();
	}
	db->endSession(globalSession.get());
	//加载playergamedata
	if(found)
	{
		playerData->setGameDBID(gamedbid);
		////////playerGameData查询
		ref_ptr<crPlayerGameData> playerGameData;
		std::string script = netCallback->getGameName() + "/playergamedata.cfg";
		ref_ptr<crData> data = CREncapsulation::loadData(script);
		if(data.valid())
		{
			playerGameData = new crPlayerGameData;
			playerGameData->setPlayerID(playerid);
			playerGameData->setDataClass(data.get());

			data->inputParam(WCHDATA_Money,&money);
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crPlayerGameDataQueryData> playerGameDataQuery = new crPlayerGameDataQueryData;
				playerGameDataQuery->buildQuerySql(playerid);

				gameSession->executeQuery(playerGameDataQuery.get());
				crDataBase::QueryResultVec &playerGameDataQueryResultVec = gameSession->getQueryResult();
				if(!playerGameDataQueryResultVec.empty())
				{
					playerGameDataQuery = dynamic_cast<crPlayerGameDataQueryData *>(playerGameDataQueryResultVec[0].get());
					playerGameData->setID(playerGameDataQuery->getID());
					playerGameData->setStream(playerGameDataQuery->getStream());
					int roleid = playerGameDataQuery->getMainRoleID();
					if(roleid > 0)
					{//查询并设置mainrole
						playerGameData->setMainRoleID(roleid);
					}
					playerGameData->setDBValid(true);
				}
				if(playerGameData->getStream())
				{
					data->inputParam(WCHDATA_DataStream,playerGameData->getStream());
					playerGameData->setStream(NULL);
				}
				playerData->setPlayerGameData(playerGameData.get());
				gameSession->releaseQuery();
				gamedb->endSession(gameSession.get());
			}
			else
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = "<<playerData->getGameDBID()<<std::endl;
				//sprintf(gDebugInfo->buf(),"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = %d\n\0",playerData->getGameDBID());
				//gDebugInfo->debugInfo(CRCore::ALWAYS);
			}
		}
	}
	////加载mainrole
	if(playerData.valid())
	{
		unsigned short gamedbid = playerData->getGameDBID();
		bool isMainRole = true;
		crPlayerGameData *playerGameData = playerData->getPlayerGameData();
		if(playerGameData)
		{
			int roleid = playerGameData->getMainRoleID();
			ref_ptr<crRole> role;
			CRCore::ref_ptr<crItemChild> itemChild;
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				bool game_ended = false;
				ref_ptr<crRoleQueryData> roleQuery = new crRoleQueryData;
				roleQuery->buildQuerySql(roleid);
				gameSession->executeQuery(roleQuery.get());
				crDataBase::QueryResultVec &roleQueryResultVec = gameSession->getQueryResult();
				int ametierid = 0;
				if(!roleQueryResultVec.empty())
				{
					role = new crRole;
					roleQuery = dynamic_cast<crRoleQueryData*>(roleQueryResultVec[0].get());
					role->setRoleID(roleid);
					role->setPlayerID(playerid);
					role->setInstanceItemID(roleQuery->getInstanceItemID());
					ametierid = roleQuery->getAbstractMetierID();
					role->setMetierStream(roleQuery->getMetierStream());
				}
				gameSession->releaseQuery();
				//gamedb->endSession(gameSession.get());

				if(role.valid())
				{
					crAbstractMetier *aMetier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(ametierid);
					role->setAbstractMetier(aMetier);

					if(role->getInstanceItemID()>0)
					{
						ref_ptr<crInstanceItemQueryData> instanceItemQuery = new crInstanceItemQueryData;
						instanceItemQuery->buildQuerySql(role->getInstanceItemID());
						gameSession->executeQuery(instanceItemQuery.get());
						crDataBase::QueryResultVec &instanceItemQueryResultVec = gameSession->getQueryResult();
						int abstractid = 0;
						if(!instanceItemQueryResultVec.empty())
						{
							instanceItemQuery = dynamic_cast<crInstanceItemQueryData*>(instanceItemQueryResultVec[0].get());
							abstractid = instanceItemQuery->getAbstractItemID();
							//role->setAbstractItemID();
							role->setItemtype(instanceItemQuery->getItemtype());
							role->setIName(instanceItemQuery->getIName());
							role->setSceneID(instanceItemQuery->getSceneID());
							role->setLayerID(instanceItemQuery->getLayerid());
							role->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
							role->setZoffset(instanceItemQuery->getZoffset());
							role->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
							role->setStream(instanceItemQuery->getStream());
						}
						gameSession->releaseQuery();

						//if(cAbstractID!=0)
						//	abstractid = cAbstractID;
						crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
						role->setAbstractItem(aitem);

						////ItemChild查询
						//ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
						//itemChildQuery->buildQuerySql(role->getInstanceItemID());
						//gameSession->executeQuery(itemChildQuery.get());
						//crDataBase::QueryResultVec &itemChildQueryResultVec = gameSession->getQueryResult();
						//if(!itemChildQueryResultVec.empty())
						//{
						//	for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
						//		citr != itemChildQueryResultVec.end();
						//		++citr )
						//	{
						//		itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
						//		itemChild = new crItemChild;
						//		itemChild->setItemChildID(itemChildQuery->getID());
						//		itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
						//		itemChild->setParentItem(role.get());
						//		role->insertChildItem(itemChild.get());
						//	}
						//}
						//gameSession->releaseQuery();

						////ChildItem查询
						//role->childItemQuery(gameSession.get());
					}
					gamedb->endSession(gameSession.get());
					game_ended = true;
					if(isMainRole)
					{
						playerGameData->setMainRole(role.get());
						role->setSceneID(0);
						role->setRoomID(0);
						role->loadMetierData(SaveStream);
						role->loadItemData(SaveStream,true);
						//role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
						//role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
					}
				}
				if(!game_ended) gamedb->endSession(gameSession.get());
			}
		}
	}
	if (!found)
	{
		return NULL;
	}
	return playerData.get();
}
/////////////////////////////////////////
//
//crJXJRecvChangeDataMethod
//
/////////////////////////////////////////
crJXJRecvChangeDataMethod::crJXJRecvChangeDataMethod():
	m_netType(GameClient_Game){}
crJXJRecvChangeDataMethod::crJXJRecvChangeDataMethod(const crJXJRecvChangeDataMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvChangeDataMethod::inputParam(int i, void *param)
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

void crJXJRecvChangeDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvChangeDataMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned short uscount = 0;
			unsigned char uccount = 0;
			int changetype = m_stream->_readInt();
			int itemid = m_stream->_readInt();
			int count = m_stream->_readInt();
			crData *data = m_this->getDataClass();
			if (m_this->getMainRole())
			{
				crData *mainroledata = m_this->getMainRole()->getDataClass();
				if (data && mainroledata)
				{
					if(changetype == T_Troops)
					{
						data->getParam(WCHDATA_JXJTroopsMap,param);
						TroopsMap *troopsMap = (TroopsMap *)param;
						TroopsMap::iterator titr = troopsMap->find(itemid);
						if (titr != troopsMap->end())
						{
							titr->second += count;
							if (titr->second < 0)
							{
								titr->second = 0;
							}
						}
						else
						{
							if (count > 0)
							{
								troopsMap->insert(std::make_pair(itemid,count));
							}
						}
					}
					else
					{
						int datatype = 0;
						switch (changetype)
						{
						case T_Copper:
							data->inputParam(WCHDATA_JXJCoppercash,&count);
							break;
						case T_Food:
							data->inputParam(WCHDATA_JXJFood,&count);
							break;
						case T_Wood:
							data->inputParam(WCHDATA_JXJWood,&count);
							break;
						case T_Iron:
							data->inputParam(WCHDATA_JXJIron,&count);
							break;
						case T_Horse:
							data->inputParam(WCHDATA_JXJHorse,&count);
							break;
						case T_Exp:
							mainroledata->inputParam(WCHDATA_Experience,&count);
							break;
						case T_Achievement:
							mainroledata->inputParam(WCHDATA_JXJAchievement,&count);
							break;
						//case T_Giftgold:
						//	data->inputParam(WCHDATA_JXJGiftGold,&count);
						//	break;
						case T_Exploit:
							mainroledata->inputParam(WCHDATA_JXJExploit,&count);
							break;
						case T_Contribute:
							data->inputParam(WCHDATA_JXJCountryContribute,&count);
							break;
						case T_Gold:
							data->inputParam(WCHDATA_Money,&count);
							break;
						case T_JiangHun:
							data->inputParam(WCHDATA_JXJCardPieces,&count);
							break;
						case T_CardImpression:
							uscount = (unsigned short)count;
							data->inputParam(WCHDATA_JXJCardImpression,&uscount);
							break;
						case T_JunLing:
							uscount = (unsigned short)count;
							data->inputParam(WCHDATA_JXJJunling,&uscount);
							break;
						case T_Level:
							uccount = (unsigned char)count;
							mainroledata->inputParam(WCHDATA_Level,&uccount);
							break;
						case T_Panguo:
							uccount = (unsigned char)count;
							mainroledata->inputParam(WCHDATA_JXJPlayerChangeShiliTimes,&uccount);
							break;
						case T_Govpost:
							uccount = (unsigned char)count;
							mainroledata->inputParam(WCHDATA_JXJGovPost, &uccount);
							break;
						case T_PlayerTotalPayGold:
							data->inputParam(WCHDATA_JXJPlayerTotalPayGold, &count);
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckBattlePunishTimeMethod
//
/////////////////////////////////////////
crJXJCheckBattlePunishTimeMethod::crJXJCheckBattlePunishTimeMethod()
{
}
crJXJCheckBattlePunishTimeMethod::crJXJCheckBattlePunishTimeMethod(const crJXJCheckBattlePunishTimeMethod& handle):
	crMethod(handle),
	m_punitshtimecanvas(handle.m_punitshtimecanvas)
{
}
void crJXJCheckBattlePunishTimeMethod::inputParam(int i, void *param)
{
}
void crJXJCheckBattlePunishTimeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_punitshtimecanvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}
void crJXJCheckBattlePunishTimeMethod::operator()(crHandle &handle)
{
	//UI_Battleground_Deserter
	bool ifcancletask = true;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_punitshtimecanvas);
		if (canvas.get())
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJBattlePunishTime,param);
			short time = *(short *)param;
			if (time <= 0)
			{
				ifcancletask = false;
				crData *roledata = myPlayer->getMainRole()->getDataClass();
				crData *data = myPlayer->getDataClass();
				roledata->getParam(WCHDATA_Level, param);
				unsigned char characterlevel = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				int battlelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevel, viplv).c_str());
				int maxBattlelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenMaxLimitLevel,viplv).c_str());
				int IncreaseDate = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevelIncreaseDate,viplv).c_str());
				int tianshu = 0;
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
					if (characterlevel >= battlelv)
					{
						ifcancletask = false;
					}
					else
					{
						ifcancletask = true;
						int texid = 7001;//"等级不足，进入战场失败!"
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 1;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					}
				}
				else if (tianshu >= IncreaseDate && tianshu < maxBattlelv-IncreaseDate)
				{
					int battlelv_1 = battlelv + tianshu - IncreaseDate + 1;
					if (characterlevel >= battlelv_1)
					{
						ifcancletask = false;
					}
					else
					{
						ifcancletask = true;
						int texid = 7001;//"等级不足，进入战场失败!"
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 1;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					}
				}
				else
				{
					if (characterlevel >= maxBattlelv)
					{
						ifcancletask = false;
					}
					else
					{
						ifcancletask = true;
						int texid = 7001;//"等级不足，进入战场失败!"
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 1;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					}
				}
			}
			else
			{
				int texid = 1083;//"您在逃兵惩罚时间内,不能进入战场!"
				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
				int mode = 1;
				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			}
		}
	}
	handle.outputParam(0,&ifcancletask);
}
/////////////////////////////////////////
//
//crJXJScenarioTalkMethod
//
/////////////////////////////////////////
crJXJScenarioTalkMethod::crJXJScenarioTalkMethod():
	m_talkid(0),
	m_endmsg(0),
	m_second(0)
{
}
crJXJScenarioTalkMethod::crJXJScenarioTalkMethod(const crJXJScenarioTalkMethod& handle):
	crMethod(handle),
	m_talkid(handle.m_talkid),
	m_endmsg(handle.m_endmsg),
	m_canvasname(handle.m_canvasname),
	m_hypertextname(handle.m_hypertextname),
	m_imageboxname(handle.m_imageboxname),
	m_second(handle.m_second),
	m_scenarioId(handle.m_scenarioId)
{
}
void crJXJScenarioTalkMethod::inputParam(int i, void *param)
{
}
void crJXJScenarioTalkMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_talkid = atoi(str.c_str());
		break;
	case 1:
		m_endmsg = atoi(str.c_str());
		break;
	case 2:
		m_canvasname = str;
		break;
	case 3:
		m_hypertextname = str;
		break;
	case 4:
		m_imageboxname = str;
		break;
	case 5:
		m_statictextname = str;
		break;
	case 6:
		m_second = atoi(str.c_str());
		break;
	case 7:
		m_scenarioId = str;
		break;
	default:
		break;
	}
}
void crJXJScenarioTalkMethod::operator()(crHandle &handle)
{
	void *param;
	int curstep = 0;
	int m_scenarioValue = 0;
	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	if (room && room->getDataClass())
	{
		crData *data = room->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJScenarioVariableMap,param);
		ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
		ScenarioVariableMap::iterator newItr = variableMap->find(m_scenarioId);
		if(newItr != variableMap->end())
			m_scenarioValue = newItr->second;
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	crTableIO::DataVec records;
	std::string canvasname = m_canvasname;
	std::string hypertextname = m_hypertextname;
	std::string imageboxname = m_imageboxname;
	std::string statictextname = m_statictextname;
	int endmsgid = m_endmsg;
	float autotime = (float)m_second;
	ref_ptr<crTableIO>ScenarioTalkTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJScenarioTalkTab);
	int talkidindex = ScenarioTalkTab->getTitleIndex("talkid");
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(canvasname);
	if (ScenarioTalkTab.valid() && talkidindex >= 0 && canvas.valid())
	{
		crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);

		ref_ptr<crData> canvasdata = canvas->getDataClass();
		ScenarioTalkTab->queryRecords(talkidindex,crArgumentParser::appItoa(m_talkid),records);
		if (canvasdata.valid() && !records.empty())
		{
			CRCore::ScopedLock<crData> lock(*canvasdata);
			canvasdata->inputParam(WCHDATA_JXJVisiableTime, &autotime);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkAutoCloseTime, &autotime);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkHyperText,&hypertextname);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkImage, &imageboxname);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkEndmsg,&endmsgid);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkStep, 0);
			canvasdata->inputParam(WCHDATA_JXJScenarioTalkNameText, &statictextname);
			canvasdata->getParam(WCHDATA_JXJScenarioTalkIDVec,param);
			std::vector<int>* talkidvec = (std::vector<int> *)param;
			talkidvec->clear();
			for (crTableIO::DataVec::iterator itr = records.begin();
				itr != records.end();
				++itr)
			{
				talkidvec->push_back(atoi((*itr)[0].c_str()));
			}
			canvas->doEvent(WCH_JXJJXJShowScenarioTalk,MAKEINT64(&canvasname,m_scenarioValue));
		}
	}
}
/////////////////////////////////////////
//
//crJXJScenarioTalkTouchMethod
//
/////////////////////////////////////////
crJXJScenarioTalkTouchMethod::crJXJScenarioTalkTouchMethod():
	m_this(NULL)
{
}
crJXJScenarioTalkTouchMethod::crJXJScenarioTalkTouchMethod(const crJXJScenarioTalkTouchMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJScenarioTalkTouchMethod::inputParam(int i, void *param)
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
void crJXJScenarioTalkTouchMethod::addParam(int i, const std::string& str)
{
}
void crJXJScenarioTalkTouchMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		ref_ptr<crData>canvasdata = m_this->getDataClass();
		if (canvasdata.valid())
		{
			CRCore::ScopedLock<crData> lock(*canvasdata);
			canvasdata->getParam(WCHDATA_JXJScenarioTalkAutoCloseTime, param);
			float autoclose = *(float *)param;
			if (autoclose <= 0.0f)
			{
				canvasdata->getParam(WCHDATA_JXJScenarioTalkStep, param);
				int *curstep = (int *)param;
				*curstep += 1;
				std::string str = m_this->getName();
				m_this->doEvent(WCH_JXJJXJShowScenarioTalk, MAKEINT64(&str, NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJShowScenarioTalkMethod
//
/////////////////////////////////////////
crJXJShowScenarioTalkMethod::crJXJShowScenarioTalkMethod():
	m_this(NULL),
	m_scenarioValue(0)
{
}
crJXJShowScenarioTalkMethod::crJXJShowScenarioTalkMethod(const crJXJShowScenarioTalkMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_scenarioValue(0)
{
}
void crJXJShowScenarioTalkMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_canvasname = *(std::string *)LOINT64(param64);
			m_scenarioValue = HIINT64(param64);
		}
		else
		{
			m_canvasname.clear();
			m_scenarioValue = 0;
		}
		break;
	}
}
void crJXJShowScenarioTalkMethod::addParam(int i, const std::string& str)
{
	//msgid
}
void crJXJShowScenarioTalkMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO>ScenarioTalkTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJScenarioTalkTab);
	if (ScenarioTalkTab.valid())
	{
		int statictextindex = ScenarioTalkTab->getTitleIndex("内容对白");
		int hypertextindex = ScenarioTalkTab->getTitleIndex("脚本对白");
		int nameindex = ScenarioTalkTab->getTitleIndex("人物名称");
		int imageindex = ScenarioTalkTab->getTitleIndex("图片名称");
		crTableIO::StrVec record;
		int curid = 0;
		void *param;
		ref_ptr<crCanvasNode>JuqingShow2 = crFilterRenderManager::getInstance()->findCanvas("UI_CopyBattle_JuqingShow2");
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
		if (canvas.valid())
		{
			ref_ptr<crData>canvasdata = canvas->getDataClass();
			if (canvasdata.valid())
			{
				canvasdata->getParam(WCHDATA_JXJScenarioTalkStep,param);
				int curstep = *(int *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkEndmsg,param);
				int endmsgid = *(int *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkIDVec,param);
				std::vector<int>* talkidvec = (std::vector<int> *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkHyperText,param);
				std::string hypertextname = *(std::string *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkImage,param);
				std::string imageboxname = *(std::string *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkNameText, param);
				std::string talknametext = *(std::string *)param;
				canvasdata->getParam(WCHDATA_JXJScenarioTalkAutoCloseTime, param);
				float autoclosetime = *(float *)param;
				if (curstep < talkidvec->size())
				{
					if (JuqingShow2.valid() && m_canvasname.compare("UI_CopyBattle_JuqingShow") == 0)
					{
						crFilterRenderManager::getInstance()->showCanvas(JuqingShow2.get(), true);
						ref_ptr<crSequence>  seq = dynamic_cast<crSequence*>(JuqingShow2->getChildNode("JuqingShow_Play"));
						if (seq.valid())
						{
							seq->setVanishWhenStop(true);
							seq->setMode(crSequence::START);
						}
					}
					if (autoclosetime > 0.0f || m_canvasname.compare("UI_CopyBattle_JuqingShow") == 0/*!canvas->getVisiable()*/)
					{
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
					}
					else
					{
						crFilterRenderManager::getInstance()->doModal(canvas.get());
					}
					curid = (*talkidvec)[curstep];
					if(ScenarioTalkTab->queryOneRecord(0,crArgumentParser::appItoa(curid),record) >= 0)
					{
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(hypertextname));
						if (information.valid())
						{
							if (!record[statictextindex].empty())
							{
								if (0 == m_scenarioValue)
								{
									ItemMap itemMap;
									int index = 1;
									rcfg::ConfigScript cfg_script(&itemMap);
									cfg_script.Add("Hypertext");
									cfg_script.Push("Hypertext");

									cfg_script.Add("Content");
									cfg_script.Push("Content",index);index++;
									std::vector<float>v_i;
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									cfg_script.Add("Color",v_i);
									cfg_script.Add("Text",record[statictextindex]);
									cfg_script.Pop();


									cfg_script.Pop();
									information->setHypertext(cfg_script,false);
								}
								else
								{
									std::string str;
									str = record[statictextindex];
									int i = str.find("%s");
									if(i != std::string::npos)
									{
										str = str.substr(0,i) + crArgumentParser::appItoa(m_scenarioValue) + str.substr(i+2,str.length());
									}
									ItemMap itemMap;
									int index = 1;
									rcfg::ConfigScript cfg_script(&itemMap);
									cfg_script.Add("Hypertext");
									cfg_script.Push("Hypertext");

									cfg_script.Add("Content");
									cfg_script.Push("Content",index);index++;
									std::vector<float>v_i;
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									v_i.push_back(255.0f);
									cfg_script.Add("Color",v_i);
									cfg_script.Add("Text",str);
									cfg_script.Pop();


									cfg_script.Pop();
									information->setHypertext(cfg_script,false);
								}
							}
							else
							{
								information->setHypertext(crGlobalHandle::getInstance()->getTextDir() + record[hypertextindex],false);
							}
						}
						else
						{
							ref_ptr<crStaticTextWidgetNode> textinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(hypertextname));
							if (textinput.valid())
							{
								textinput->setString(record[statictextindex]);
							}
						}
						if (record[nameindex] == "Player")
						{
							crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
							if (myPlayer && myPlayer->getDataClass())
							{
								void * param;
								crData *playerData = myPlayer->getDataClass();
								playerData->getParam(WCHDATA_PlayerIconID,param);
								unsigned char iconID = *(unsigned char *) param;

								crTableIO::StrVec record;
								ref_ptr<crTableIO>playerIconTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
								if(playerIconTable.valid() && playerIconTable->queryOneRecord(0,crArgumentParser::appItoa(iconID),record)>=0)
								{
									//头像
									int iconindex = playerIconTable->getTitleIndex("icon剧情");
									ref_ptr<crImageBoxWidgetNode> imagebox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(imageboxname));
									if (imagebox.valid() && iconindex >= 0)
									{
										imagebox->setImageName(record[iconindex].c_str());
									}
								}
								ref_ptr<crStaticTextWidgetNode> namebox = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(talknametext));
								if (namebox.valid())
								{
									namebox->setString(myPlayer->getCharacterName());
								}
							}
						}
						else
						{

							ref_ptr<crStaticTextWidgetNode> namebox = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(talknametext));
							if (namebox.valid())
							{
								namebox->setString(record[nameindex]);
							}
							ref_ptr<crImageBoxWidgetNode> imagebox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(imageboxname));
							if (imagebox.valid())
							{
								imagebox->setImageName(record[imageindex].c_str());
							}
						}
					}
				}
				else
				{
					if (JuqingShow2.valid())
					{
						crFilterRenderManager::getInstance()->showCanvas(JuqingShow2.get(), false);
					}
					if (canvas->getVisiable())
					{
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);
					}
					//msgid
					if(endmsgid>0)
					{
						crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
						if(room)
						{
							room->doEvent(MAKEINT64(WCH_JXJEndTalk,endmsgid));
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSelGateUpdateMethod
//
/////////////////////////////////////////
crJXJSelGateUpdateMethod::crJXJSelGateUpdateMethod():
	m_this(NULL)
{
}
crJXJSelGateUpdateMethod::crJXJSelGateUpdateMethod(const crJXJSelGateUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
	m_pagenum = handle.m_pagenum;
	for (int i = 0; i < 6; ++i)
	{
		m_serverNo[i] = handle.m_serverNo[i];
		m_serverName[i] = handle.m_serverName[i];
		m_stats[i] = handle.m_stats[i];
		m_telecom[i] = handle.m_telecom[i];
		m_cnc[i] = handle.m_cnc[i];
		m_backboard[i] = handle.m_backboard[i];
	}
}
void crJXJSelGateUpdateMethod::inputParam(int i, void *param)
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
void crJXJSelGateUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i) 
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_serverNo[i] = str;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		m_serverName[i - 6] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		m_stats[i - 12] = str;
		break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		m_telecom[i - 18] = str;
		break;
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		m_cnc[i - 24] = str;
		break;
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
		m_backboard[i - 30] = str;
		break;
	case 36:
		m_pagenum = str;
		break;
	default:
		break;
	}
}
void crJXJSelGateUpdateMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		int curpageservercount = 6;
		crData *canvasData = m_this->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJServerListMap,param);
		ServerListMap *serverListMap = (ServerListMap *)param;
		canvasData->getParam(WCHDATA_JXJSelectServerUICurPage,param);
		int curpagenum = *(int *)param;
		if(!serverListMap->empty())
		{
			///////////显示服务器列表
			ref_ptr<crQueryServerListData> serverListData = NULL;
			//serverListData = serverListMap->begin()->second.get();
			ref_ptr<crStaticTextWidgetNode> serverNo[6] = {NULL};// = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_serverNo));
			ref_ptr<crStaticTextWidgetNode> serverName[6] = {NULL};// = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_serverName));
			ref_ptr<crStaticTextWidgetNode> stats[6] = {NULL};// = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_stats));
			ref_ptr<crButtonWidgetNode> telecom[6] = {NULL};// = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_telecom));
			ref_ptr<crButtonWidgetNode> cnc[6] = {NULL};// = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_cnc
			ref_ptr<crNode> backboard[6] = {NULL};// = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_cnc));
			for (int i = 0; i < 6; ++i)
			{
				serverNo[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_serverNo[i]));
				serverName[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_serverName[i]));
				stats[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_stats[i]));
				telecom[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_telecom[i]));
				cnc[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getChildNode(m_cnc[i]));
				backboard[i] = dynamic_cast<crNode *>(m_this->getChildNode(m_backboard[i]));
				if(serverNo[i].valid())serverNo[i]->clearString();
				if(serverName[i].valid())serverName[i]->clearString();
				if(stats[i].valid())stats[i]->clearString();
				if(telecom[i].valid())telecom[i]->setVisiable(false);
				if(cnc[i].valid())cnc[i]->setVisiable(false);
				if(backboard[i].valid())backboard[i]->setVisiable(false);
			}
			int serverid = 0;
			int index = -curpageservercount * (curpagenum - 1);
			int count  = 0;
			std::string name;
			std::string statstr;
			for (ServerListMap::iterator itr = serverListMap->begin();
				itr != serverListMap->end() && index < 6;
				++itr,++index,++serverid)
			{
				if (index >= 0)
				{
					if(backboard[index].valid())backboard[index]->setVisiable(true);
					serverListData = itr->second;
					if(serverNo[index].valid())
					{
						serverNo[index]->setString(std::string(crArgumentParser::appItoa(serverid + 1)));
					}
					if(serverName[index].valid())
					{
						name = serverListData->getServerName();
						serverName[index]->setString(name);
					}
					count = serverListData->getRemainCount();
					if(stats[index].valid())
					{
						if(count == -1000)
						{
							statstr = "维护";
							stats[index]->setColor(crVector4(100,100,100,255));
						}
						else if(count == -1001)
						{
							statstr = "已轮回";
							stats[index]->setColor(crVector4(100,100,100,255));
						}
						else if(count == -1002)
						{
							statstr = "即将开服";
							stats[index]->setColor(crVector4(100,100,100,255));
						}
						else if(count>500)
						{
							statstr = "空闲";
							stats[index]->setColor(crVector4(0,255,0,255));
						}
						else if(count>100)
						{
							statstr = "繁忙";
							stats[index]->setColor(crVector4(255,255,0,255));
						}
						else
						{
							statstr = "爆满";
							stats[index]->setColor(crVector4(255,0,0,255));
						}
						stats[index]->setString(statstr);
					}
					if (telecom[index].valid() && cnc[index].valid())
					{
						telecom[index]->setVisiable(true);
						cnc[index]->setVisiable(true);
						bool enable = count>0||count<=-1000;
						if(serverListData->getISP() == 0)
						{
							telecom[index]->setEnable(enable);
							cnc[index]->setEnable(enable);
						}
						else if(serverListData->getISP() == 1)
						{
							telecom[index]->setEnable(enable);
							cnc[index]->setEnable(false);
						}
						else
						{
							telecom[index]->setEnable(false);
							cnc[index]->setEnable(enable);
						}
					}
				}
			}
			int totalpagenum = ceil((float)serverListMap->size()/(float)curpageservercount);
			canvasData->inputParam(WCHDATA_JXJSelectServerUITotalPage,&totalpagenum);
			std::string pagestr = crArgumentParser::appItoa(curpagenum) + "/" + crArgumentParser::appItoa(totalpagenum);
			ref_ptr<crStaticTextWidgetNode> pagenum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pagenum));
			if (pagenum.valid())
			{
				pagenum->setString(pagestr);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRoomStateTipsMethod
//
/////////////////////////////////////////
crJXJRoomStateTipsMethod::crJXJRoomStateTipsMethod():
	m_this(NULL)
{
}
crJXJRoomStateTipsMethod::crJXJRoomStateTipsMethod(const crJXJRoomStateTipsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_tipscanvas(handle.m_tipscanvas),
	m_tipinput(handle.m_tipinput)
{
}
void crJXJRoomStateTipsMethod::inputParam(int i, void *param)
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
void crJXJRoomStateTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tipscanvas = str;
		break;
	case 1:
		m_tipinput = str;
		break;
	default:
		break;
	}
}
void crJXJRoomStateTipsMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		int index = -1;
		std::string inputname = m_this->getName();
		std::string names[12] = 
		{
			"LineUp_Input3",
			"LineUp_Input6",
			"LineUp_Input9",
			"LineUp_Input12",
			"LineUp_Input15",
			"LineUp_Input18",
			"LineUp_Input21",
			"LineUp_Input24",
			"LineUp_Input27",
			"LineUp_Input30",
			"LineUp_Input33",
			"LineUp_Input36"
		};
		for (int i = 0; i < 12; ++i)
		{
			if (inputname.compare(names[i]) == 0)
			{
				index = i;
			}
		}

		void *param;
		std::string tipstr;
		if (m_this->getParentCanvas() && m_this->getParentCanvas()->getDataClass())
		{
			ref_ptr<crCanvasNode> tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipscanvas);
			if (tipscanvas.valid())
			{
				ref_ptr<crStaticTextWidgetNode> tipinput = dynamic_cast<crStaticTextWidgetNode *>(tipscanvas->getWidget(m_tipinput));
				if (tipinput.valid())
				{
					crData *canvasdata = m_this->getParentCanvas()->getDataClass();
					CRCore::ScopedLock<crData> lock(*canvasdata);
					canvasdata->getParam(WCHDATA_JXJRoomStateVec,param);
					RoomStateVec *roomstatevec = (RoomStateVec *)param;

					if (index < roomstatevec->size())
					{
						if ((*roomstatevec)[index] != RoomStateType_Accessible)
						{
							if((*roomstatevec)[index] == RoomStateType_NotOur)
							{
								tipstr = "不是我方的战场，不可进入";
							}
							else if((*roomstatevec)[index] == RoomStateType_NotEnoughTime)
							{
								tipstr = "战场剩余时间小于5分钟，不可进入";
							}
							tipinput->setString(tipstr);
							const crBoundingBox &btnbox = m_this->getBoundBox();
							const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
							float posx = btnbox.m_min[0];
							float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
							crVector2 mouse(crVector2(posx,posy));
							tipscanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
							crFilterRenderManager::getInstance()->showCanvas(tipscanvas.get(),true);
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJGetConsortiaNameMethod
//
/////////////////////////////////////////
crJXJGetConsortiaNameMethod::crJXJGetConsortiaNameMethod(){}
crJXJGetConsortiaNameMethod::crJXJGetConsortiaNameMethod(const crJXJGetConsortiaNameMethod& handle):
	crMethod(handle)
{
}
void crJXJGetConsortiaNameMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_playerid = 0;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerid = LOINT64(param64);
			m_consortianame = (std::string*)(HIINT64(param64));
		}
		else
		{
			m_playerid = 0;
		}
		break;
	}
}

void crJXJGetConsortiaNameMethod::addParam(int i, const std::string& str)
{
}

void crJXJGetConsortiaNameMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crData *gsBrainData = m_this->getDataClass();
		if (gsBrainData)
		{
			void *param;
			ref_ptr<crGameServerPlayerData> playerData = NULL;
			crData *playerGameData = NULL;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = ( ConsortiaMap *)param;
			ConsortiaMap::iterator consoriaitr;
			ConsortiaMemberMap::iterator memberitr;
			crData *condata = NULL;
			ConsortiaMemberMap *membermap = NULL;
			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_playerid));
			if (playerData.valid() && playerData->getPlayerGameData())
			{//玩家在线
				playerGameData = playerData->getPlayerGameData()->getDataClass();
				playerGameData-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *(int *)param;
				if(mygroupid > 0)
				{
					consoriaitr = consortiamap->find(mygroupid);
					if (consoriaitr != consortiamap->end())
					{
						consoriaitr->second->getName(*m_consortianame);
					}
				}
			}
			else
			{//玩家不在线,遍历军团map
				for (consoriaitr = consortiamap->begin();
					consoriaitr != consortiamap->end();
					++consoriaitr)
				{
					condata = consoriaitr->second->getDataClass();
					condata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					membermap = (ConsortiaMemberMap *)param;
					for (memberitr = membermap->begin();
						memberitr != membermap->end();
						++memberitr)
					{
						if (memberitr->second != NULL)
						{
							if(memberitr->second->getPlayerID() == m_playerid)
							{
								consoriaitr->second->getName(*m_consortianame);
							}
						}
					}
				}
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJUINewFunctionOpenCheckMethod
//新功能开启检测
//
/////////////////////////////////////////
crJXJUINewFunctionOpenCheckMethod::crJXJUINewFunctionOpenCheckMethod()
{
}
crJXJUINewFunctionOpenCheckMethod::crJXJUINewFunctionOpenCheckMethod(const crJXJUINewFunctionOpenCheckMethod& handle) :
crMethod(handle)
{
}
void crJXJUINewFunctionOpenCheckMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJUINewFunctionOpenCheckMethod::addParam(int i, const std::string& str)
{
}

void crJXJUINewFunctionOpenCheckMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getDataClass())
	{
		crData *data = playerData->getDataClass();
		crData *roledata = playerData->getMainRole()->getDataClass();
		ref_ptr<crTableIO>NewFuctionOpenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewFunctionOpenTab);
		ref_ptr<crTableIO>TroopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		crTableIO::StrVec armyrecord;
		int kejiidindex = NewFuctionOpenTab->getTitleIndex("0科技ID");
		int lingzhulvindex = NewFuctionOpenTab->getTitleIndex("1领主等级");
		int fubenidindex = NewFuctionOpenTab->getTitleIndex("2完成副本ID");
		int bingzhongidindex = NewFuctionOpenTab->getTitleIndex("3兵种ID");
		int jianguozhiluindex = NewFuctionOpenTab->getTitleIndex("4完成建国之路");
		int canvasnameindex = NewFuctionOpenTab->getTitleIndex("打开窗口名");
		int iconboxnameindex = NewFuctionOpenTab->getTitleIndex("图片控件名");
		int iconnameindex = NewFuctionOpenTab->getTitleIndex("图片名字");
		int inputnameindex = NewFuctionOpenTab->getTitleIndex("文本控件名");
		int infotextindex = NewFuctionOpenTab->getTitleIndex("文字说明");
		int tiaozhuanindex = NewFuctionOpenTab->getTitleIndex("是否跳转");
		int tiaozhuannameindex = NewFuctionOpenTab->getTitleIndex("跳转窗口名");
		int radiogroupnameindex = NewFuctionOpenTab->getTitleIndex("切页控件名");
		int pagenumindex = NewFuctionOpenTab->getTitleIndex("页码");
		if (NewFuctionOpenTab.valid() && kejiidindex > 0 && lingzhulvindex > 0 && fubenidindex > 0
			&& bingzhongidindex > 0 && canvasnameindex > 0 && inputnameindex > 0 && infotextindex > 0 && tiaozhuanindex > 0
			&& tiaozhuannameindex > 0 && tiaozhuannameindex > 0 && radiogroupnameindex > 0 && pagenumindex > 0
			&& iconboxnameindex > 0 && inputnameindex > 0 && jianguozhiluindex > 0)
		{
			int curshowid = 0;
			CRCore::ScopedLock<crData> lock(*data);
			data->getParam(WCHDATA_JXJNewFunctionOpenSet, param);
			NewFunctionOpenSet *newfunctionopenset = (NewFunctionOpenSet *)param;
			data->getParam(WCHDATA_JXJTroopsTechMap, param);
			TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
			NewFunctionOpenSet::iterator newitr;
			TroopsTechMap::iterator troopsitr;
			const char linestr = '|';
			int lineint = 0;
			int newtroopsid = 0;
			int newfunctionopenid = 0;
			int needtechid = 0;
			int needlordlv = 0;
			int needfubenid = 0;
			int troopsid = 0;
			int needjianguo;
			unsigned char techlv = 0;
			unsigned char lordlv = 0;
			bool ifcanrecuit = true;
			std::vector < int > needvec;
			FubenProgressMap *fubenProgressMap = NULL;
			FubenProgressMap::iterator fubenitr;
			std::vector<std::string> iconnamevec;
			std::string tempiconname;
			std::string canvasname, inputname, inputinfo,iconboxname,iconname;
			int tneedIndex = TroopsTab->getTitleIndex("需求军备ID");
			int ticonIndex = TroopsTab->getTitleIndex("icon");
			for (int i = 0; i < NewFuctionOpenTab->getRowCount(); ++i)
			{
				iconnamevec.clear();
				newfunctionopenid = atoi((*NewFuctionOpenTab)(i, 0).c_str());
				newitr = newfunctionopenset->find(newfunctionopenid);
				canvasname = (*NewFuctionOpenTab)(i, canvasnameindex);
				inputname = (*NewFuctionOpenTab)(i, inputnameindex);
				inputinfo = (*NewFuctionOpenTab)(i, infotextindex);
				iconboxname = (*NewFuctionOpenTab)(i, iconboxnameindex);
				iconname = (*NewFuctionOpenTab)(i, iconnameindex);
				lineint = iconname.find(linestr);
				if (lineint > 0)
				{
					tempiconname = iconname;
					tempiconname.erase(lineint, iconname.size());
					iconnamevec.push_back(tempiconname);
					iconname.erase(0, lineint + 1);
					tempiconname = iconname;
					lineint = iconname.find(linestr);
					if (lineint > 0)
					{
						tempiconname.erase(lineint, iconname.size());
						iconnamevec.push_back(tempiconname);
						iconname.erase(0, lineint);
					}
					iconnamevec.push_back(iconname);
				}
				if (newitr != newfunctionopenset->end())
				{
					continue;
				}
				needtechid = atoi((*NewFuctionOpenTab)(i, kejiidindex).c_str());
				needlordlv = atoi((*NewFuctionOpenTab)(i, lingzhulvindex).c_str());
				needfubenid = atoi((*NewFuctionOpenTab)(i, fubenidindex).c_str());
				troopsid = atoi((*NewFuctionOpenTab)(i, bingzhongidindex).c_str());
				needjianguo = atoi((*NewFuctionOpenTab)(i, jianguozhiluindex).c_str());
				if (needtechid > 0)
				{
					data->getParam(needtechid, param);
					techlv = *(unsigned char *)param;
					if (techlv > 0)
					{
						curshowid = newfunctionopenid;
						break;
					}
				}
				else if (needlordlv > 0)
				{
					roledata->getParam(WCHDATA_Level, param);
					lordlv = *(unsigned char *)param;
					if (lordlv >= needlordlv)
					{
						curshowid = newfunctionopenid;
						break;
					}
				}
				else if (needfubenid > 0)
				{
					data->getParam(WCHDATA_JXJFubenProgressMap, param);
					fubenProgressMap = (FubenProgressMap *)param;
					fubenitr = fubenProgressMap->find(needfubenid);
					if (fubenitr != fubenProgressMap->end() && fubenitr->second->getComplete())
					{
						curshowid = newfunctionopenid;
						break;
					}
				}
				else if (troopsid > 0)
				{
					ifcanrecuit = true;
					needvec.clear();
					if (TroopsTab->queryOneRecord(0, crArgumentParser::appItoa(troopsid), armyrecord) >= 0)
					{
						iconname = armyrecord[ticonIndex];
						crArgumentParser::appAtoVec(armyrecord[tneedIndex], needvec);
						for (int i = 0; i < needvec.size(); ++i)
						{
							if (needvec[i] > 0)
							{
								troopsitr = troopstechmap->find(needvec[i]);
								if (troopsitr == troopstechmap->end() || troopsitr->second <= 0)
								{
									ifcanrecuit = false;
									break;
								}
							}
						}
					}
					if (ifcanrecuit)
					{
						newtroopsid = troopsid;
						curshowid = newfunctionopenid;
						break;
					}
				}
				else if (needjianguo > 0)
				{
					data->getParam(WCHDATA_JXJFoundCountryGetAward, param);
					unsigned char getAward = *(unsigned char *)param;
					if (getAward == (unsigned char)-1)
					{
						curshowid = newfunctionopenid;
						break;
					}
				}
			}
			if (curshowid > 0)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				unsigned short newfuncid = curshowid;
				newfunctionopenset->insert(newfuncid);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUShort(newfuncid);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvUpdateNewFunction, stream.get());
				netConductor->getNetManager()->sendPacket("all", packet);
				//
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(canvasname);
				if (canvas.valid() && canvas->getDataClass())
				{
					float visiabletime = 8.0f;
					crData *canvasdata = canvas->getDataClass();
					canvasdata->inputParam(WCHDATA_JXJNewFunctionOpenID, &curshowid);
					canvasdata->inputParam(WCHDATA_JXJNewOpenTroopsID, &newtroopsid);
					canvasdata->inputParam(WCHDATA_JXJVisiableTime, &visiabletime);
					ref_ptr<crStaticTextWidgetNode> textinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(inputname));
					ref_ptr<crImageBoxWidgetNode> iconbox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(iconboxname));
					if (textinput.valid())
					{
						textinput->setString(inputinfo);
					}
					if (iconbox.valid())
					{
						iconbox->setImageName(iconname);
					}
					else
					{
						crStateSet *state_set;
						crTexture2D *texture_2d;
						ref_ptr<crButtonWidgetNode> buttonbox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(iconboxname));
						if (buttonbox.valid())
						{
							for (int i = 0; i < 3; ++i)
							{
								if (i < iconnamevec.size())
								{
									state_set = dynamic_cast<crObject *>((dynamic_cast<crGroup *>(buttonbox->getChild(0)))->getChild(i))->getDrawable(0)->getStateSet();
									if (state_set)texture_2d = dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0, crStateAttribute::TEXTURE));
									if (texture_2d)
										texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(iconnamevec[i], texture_2d));
								}
							}
						}
					}
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUpdateNewFunctionMethod
//新功能提示
/////////////////////////////////////////
crJXJRecvUpdateNewFunctionMethod::crJXJRecvUpdateNewFunctionMethod() :
m_netType(GameClient_Game){}
crJXJRecvUpdateNewFunctionMethod::crJXJRecvUpdateNewFunctionMethod(const crJXJRecvUpdateNewFunctionMethod& handle) :
crMethod(handle)
{
}
void crJXJRecvUpdateNewFunctionMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvUpdateNewFunctionMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUpdateNewFunctionMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			void *param;
			unsigned short curid = m_stream->_readUShort();
			crData *data = m_this->getDataClass();
			CRCore::ScopedLock<crData> lock(*data);
			data->getParam(WCHDATA_JXJNewFunctionOpenSet, param);
			NewFunctionOpenSet *newfunctionopenset = (NewFunctionOpenSet *)param;
			newfunctionopenset->insert(curid);
		}
	}
}
/////////////////////////////////////////
//
//crJXJUINewFunctionBtnTouchMethod
//新功能开启跳转
//
/////////////////////////////////////////
crJXJUINewFunctionBtnTouchMethod::crJXJUINewFunctionBtnTouchMethod()
{
}
crJXJUINewFunctionBtnTouchMethod::crJXJUINewFunctionBtnTouchMethod(const crJXJUINewFunctionBtnTouchMethod& handle) :
crMethod(handle)
{
}
void crJXJUINewFunctionBtnTouchMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUINewFunctionBtnTouchMethod::addParam(int i, const std::string& str)
{
}

void crJXJUINewFunctionBtnTouchMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && m_this && m_this->getParentCanvas())
	{
		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
		ref_ptr<crTableIO>NewFuctionOpenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewFunctionOpenTab);
		crTableIO::StrVec newfucrecord;
		int jianguozhiluindex = NewFuctionOpenTab->getTitleIndex("4完成建国之路");
		int tiaozhuanindex = NewFuctionOpenTab->getTitleIndex("是否跳转");
		int tiaozhuannameindex = NewFuctionOpenTab->getTitleIndex("跳转窗口名");
		int radiogroupnameindex = NewFuctionOpenTab->getTitleIndex("切页控件名");
		int pagenumindex = NewFuctionOpenTab->getTitleIndex("页码");
		int jianzhuIDindex = NewFuctionOpenTab->getTitleIndex("建筑ID");
		if (NewFuctionOpenTab.valid() && tiaozhuanindex > 0 && tiaozhuannameindex > 0 && 
			radiogroupnameindex > 0 && pagenumindex > 0)
		{
			canvasdata->getParam(WCHDATA_JXJNewOpenTroopsID, param);
			int newtroopsid = *(int *)param;
			canvasdata->getParam(WCHDATA_JXJNewFunctionOpenID, param);
			int curid = *(int *)param;
			if (curid > 0)
			{
				if (NewFuctionOpenTab->queryOneRecord(0, crArgumentParser::appItoa(curid), newfucrecord) >= 0)
				{
					if (atoi(newfucrecord[tiaozhuanindex].c_str()) > 0)
					{
						if (atoi(newfucrecord[jianguozhiluindex].c_str()) > 0)
						{
							ref_ptr<crCanvasNode> jianguocanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Jianguo");
							if (jianguocanvas.valid())
							{
								ref_ptr<crButtonWidgetNode> qianwangguozhan = dynamic_cast<crButtonWidgetNode *>(jianguocanvas->getWidget("UI_Jianguo_Qwgz"));
								if (qianwangguozhan.valid())qianwangguozhan->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
							}
						}
						else
						{
							ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(newfucrecord[tiaozhuannameindex]);
							if (canvas.valid())
							{
								int itemID = atoi(newfucrecord[jianzhuIDindex].c_str());
								if(itemID > 0)
								{
									CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
									if(scene)
									{
										crInstanceItem *item = scene->findSceneItem(itemID);
										if(item)
										{
											item->doEvent(WCH_Touch);
										}
									}
								}
								else
								{
									crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
								}
								ref_ptr<crRadioGroupWidgetNode> radiogroupwidget = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(newfucrecord[radiogroupnameindex]));
								if (radiogroupwidget.valid())
								{
									radiogroupwidget->select(atoi(newfucrecord[pagenumindex].c_str()));
									radiogroupwidget->doEvent(WCH_UI_SelectChange);
								}
							}
						}
					}
				}
			}
			if (newtroopsid > 0)
			{//定位到兵营该兵种
				ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Barracks");
				if (canvas.valid())
				{
					CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
					if (scene)
					{
						crInstanceItem *item = scene->findSceneItem(2);
						if (item)
						{
							item->doEvent(WCH_Touch);
						}
					}
					ref_ptr<crRadioGroupWidgetNode> ArmyRaido = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget("ArmyRaido"));
					ref_ptr<crRadioGroupWidgetNode> ArmyRaido_1 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget("ArmyRaido_1"));
					ref_ptr<crTableIO>troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
					crTableIO::StrVec armyrecord;
					crTableIO::DataVec recordVec;
					RecruitCheckResult recruitCheck;
					std::vector<int> EnableRowVec;
					std::vector<int> DisableRowVec;
					int armType = 0;
					int canindex = troopsTab->getTitleIndex("可招募");
					int armytypeindex = troopsTab->getTitleIndex("兵种");
					if (troopsTab->queryOneRecord(0, crArgumentParser::appItoa(newtroopsid), armyrecord) >= 0)
					{
						armType = atoi(armyrecord[armytypeindex].c_str());
						troopsTab->queryRecords(armytypeindex, crArgumentParser::appItoa(armType), canindex, "1", recordVec);
						int rowCount = recordVec.size();
						int abastactid = 0;
						int curtroopindex = 0;
						int curpagefirstid = 0;
						for (int row = 0; row < rowCount; ++row)
						{
							abastactid = atoi(recordVec[row][0].c_str());
							playerData->doEvent(WCH_JXJRecruitCheck, MAKEINT64(abastactid, &recruitCheck));
							if (recruitCheck.m_code == -3 || recruitCheck.m_code == -4)
							{
								DisableRowVec.push_back(row);
							}
							else
							{
								EnableRowVec.push_back(row);
								if (EnableRowVec.size() > 3 && EnableRowVec.size() % 3 == 1)
								{
									curpagefirstid += 3;// = abastactid;
								}
								if (abastactid == newtroopsid)
								{
									curtroopindex = EnableRowVec.size();
									break;
								}
							}
						}
						crData *braindata = crBrain::getInstance()->getDataClass();
						if (ArmyRaido.valid())
						{ 
							ArmyRaido->select(armType); 
							ArmyRaido->doEvent(WCH_UI_SelectChange);
						}
						if (braindata)
						{
							braindata->inputParam(WCHDATA_JXJCurRecruitArmID, &curpagefirstid);
						}
						if (ArmyRaido_1.valid())
						{ 
							ArmyRaido_1->select(curtroopindex % 3);
							ArmyRaido_1->doEvent(WCH_UI_SelectChange);
						}
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJScenarioTalkFx1Method
//
//////////////////////////////////////////////////////////////////////////
crJXJScenarioTalkFx1Method::crJXJScenarioTalkFx1Method() :
	m_dt(NULL),
m_rotate(0.0f),
m_totaltime(0.0f),
m_alpha(1.0f),
m_maxscale(1.1f),
m_nowscale(-PI)
{}
crJXJScenarioTalkFx1Method::crJXJScenarioTalkFx1Method(const crJXJScenarioTalkFx1Method& handle) :
crMethod(handle),
m_dt(NULL),
m_vector3(handle.m_vector3),
m_rotate(handle.m_rotate),
m_totaltime(handle.m_totaltime),
m_alpha(handle.m_alpha),
m_maxscale(handle.m_maxscale)
{
}
void crJXJScenarioTalkFx1Method::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJScenarioTalkFx1Method::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str, m_vector3);
		break;
	case 1:
		m_maxscale = atof(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJScenarioTalkFx1Method::operator()(crHandle &handle)
{
	//UI_CopyBattle_JuqingShow2
	float scalemax = m_maxscale;
	crVector3 speeddir = m_vector3; //crVector3(10, 5, 0.1);
	float nowscale = 0.0f;
	if (m_totaltime <=0.0f)
	{
		nowscale = -0.6f;
	}
	else if (m_totaltime < 0.1f)
	{
		nowscale = 0.0f;// -0.6 + 0.6f / 0.1f * m_totaltime;// -0.6f;
	}
	else if (m_totaltime < 0.2f)
	{
		nowscale = 0.2f;
	}
	else
	{
		nowscale = 0.0f;
	}
	//else if (m_totaltime < 0.3f)
	//{
	//	nowscale = 1.0f - 1.0f / 0.2f * (m_totaltime - 0.1f);// (m_totaltime - 0.3));
	//}
	ref_ptr<crCanvasNode >JuqingShow2 = crFilterRenderManager::getInstance()->findCanvas("UI_CopyBattle_JuqingShow2");
	if (JuqingShow2.valid())
	{
		JuqingShow2->setMatrix(crMatrix::translate(0.0f, 0.65f, 0.0f));
	}
	ref_ptr<crHypertextWidgetNode> JuqingShow_Input = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget("JuqingShow_Input"));
	if (JuqingShow_Input.valid())
	{
		JuqingShow_Input->setEnableMatrix(true);
		JuqingShow_Input->setMatrix(crMatrix::scale(1.0f + nowscale, 1.0f + nowscale, 1.0f));// * crMatrix::translate(0.0f, 0.65f, 0.0f));
	}
	m_this->setMatrix(/*crMatrix::scale(1.0f + nowscale, 1.0f + nowscale, 1.0f) * */crMatrix::translate(0.0f, 0.65f, 0.0f));
	//m_this->setEnableMatrix(true);
	//const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
	//const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
	////m_lastangle
	//crMatrix scaleMatrix = crMatrix::scale(1.0f + nowscale, 1.0f + nowscale, 1.0f);
	//crMatrix mat = m_this->getMatrix();
	//crVector3 trans = m_this->getBound().center();//matrixTransform->getBound().center();
	//crMatrix put_mat = crMatrix::translate(trans);
	//crMatrix inv_put = crMatrix::inverse(put_mat);
	//mat.postMult(inv_put);
	//mat.postMult(scaleMatrix);
	//mat.postMult(put_mat);
	//m_this->setMatrix(mat);
	//if (nowscale != 0.0f)
	//{
	//}
	//else
	//{
		//m_this->setMatrix();
	//}
				//m_rotate += CRCore::PI * 0.5f;
	if(m_totaltime<=0.2f)
	{
		m_alpha += 1.0f / 0.2f * (*m_dt);
	}
	else if (m_totaltime > 1.0f)
	{
		m_alpha -= 1.0f / 2.0f * (*m_dt);
		if (m_alpha < 0.0f)
		{
			void *param;
			crData *canvasdata = m_this->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJScenarioTalkStep, param);
			int *curstep = (int *)param;
			*curstep += 1;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			std::string str = m_this->getName();
			m_this->doEvent(WCH_JXJJXJShowScenarioTalk, MAKEINT64(&str, NULL));

			m_alpha = 0.0f;
			m_totaltime = 0.0f;
		}
	}
	CRCore::crCollectNodeBYClassNameVisitor searchVisitor;
	searchVisitor.insertClassNameId("crObject");
	m_this->accept(searchVisitor);
	NodeArray &objectArray = searchVisitor.getResult();
	for (NodeArray::iterator itr = objectArray.begin();
		itr != objectArray.end();
		++itr)
	{
		crObject::DrawableList &drawableList = dynamic_cast<crObject *>(itr->get())->getDrawableList();
		for (crObject::DrawableList::iterator ditr = drawableList.begin();
			ditr != drawableList.end();
			++ditr)
		{
			(*ditr)->getColor().a() = m_alpha;
		}
	}
	m_totaltime += *m_dt;
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJScenarioTalkOnShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJScenarioTalkOnShowMethod::crJXJScenarioTalkOnShowMethod() 
{}
crJXJScenarioTalkOnShowMethod::crJXJScenarioTalkOnShowMethod(const crJXJScenarioTalkOnShowMethod& handle) :
crMethod(handle)
{
}
void crJXJScenarioTalkOnShowMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJScenarioTalkOnShowMethod::addParam(int i, const std::string& str)
{
}

void crJXJScenarioTalkOnShowMethod::operator()(crHandle &handle)
{
	m_this->setMatrix(crMatrix::scale(0.4f, 0.4f, 1.0f));
	float alpha = 0.0f;
	CRCore::crCollectNodeBYClassNameVisitor searchVisitor;
	searchVisitor.insertClassNameId("crObject");
	m_this->accept(searchVisitor);
	NodeArray &objectArray = searchVisitor.getResult();
	for (NodeArray::iterator itr = objectArray.begin();
		itr != objectArray.end();
		++itr)
	{
		crObject::DrawableList &drawableList = dynamic_cast<crObject *>(itr->get())->getDrawableList();
		for (crObject::DrawableList::iterator ditr = drawableList.begin();
			ditr != drawableList.end();
			++ditr)
		{
			(*ditr)->getColor().a() = alpha;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJExchangeResCanvasInitMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJExchangeResCanvasInitMethod::crJXJExchangeResCanvasInitMethod()
{}
crJXJExchangeResCanvasInitMethod::crJXJExchangeResCanvasInitMethod(const crJXJExchangeResCanvasInitMethod& handle)
{
	for (int i = 0; i < 4; ++i)
	{
		m_resourcemenu[i] = handle.m_resourcemenu[i];
		m_resourcelist[i] = handle.m_resourcelist[i];
	}
}
void crJXJExchangeResCanvasInitMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	default:
		break;
	}
}
void crJXJExchangeResCanvasInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_resourcemenu[i] = str;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_resourcelist[i - 4] = str;
		break;
	}
}
void crJXJExchangeResCanvasInitMethod::operator()(crHandle &handle)
{
	std::string resname[4] = { "粮食", "木材", "铁矿", "马匹" };
	ref_ptr<crComboBoxWidgetNode> resourcemenu[4] = { NULL };
	ref_ptr<crTableWidgetNode> gamelist[4] = { NULL };
	int index = 0;
	for (int i = 0; i < 4; ++i)
	{
		index = 0;
		resourcemenu[i] = dynamic_cast<crComboBoxWidgetNode *>(m_this->getWidget(m_resourcemenu[i]));
		gamelist[i] = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_resourcelist[i]));
		if (gamelist[i].valid())
		{
			gamelist[i]->clearData();
			for (int ii = 0; ii < 4; ++ii)
			{
				if (ii != i)
				{
					gamelist[i]->addData(0, index, resname[ii]);
					gamelist[i]->addData(index, ii);
					index++;
				}
			}
		}
		if (resourcemenu[i].valid())
		{
			resourcemenu[i]->select(0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJMarketExchangeResourceMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJMarketExchangeResourceMethod::crJXJMarketExchangeResourceMethod()
{}
crJXJMarketExchangeResourceMethod::crJXJMarketExchangeResourceMethod(const crJXJMarketExchangeResourceMethod& handle) :
crMethod(handle)
{
	m_remainCount = handle.m_remainCount;
	m_nextlvCount = handle.m_nextlvCount;
	for (int i = 0; i < 4; ++i)
	{
		m_resourcecurcount[i] = handle.m_resourcecurcount[i];
		m_resourcesellcount[i] = handle.m_resourcesellcount[i];
		m_resourcemenu[i] = handle.m_resourcemenu[i];
		m_resourceget[i] = handle.m_resourceget[i];
		m_exchangebtn[i] = handle.m_exchangebtn[i];
		m_countscrollbar[i] = handle.m_countscrollbar[i];
	}
}
void crJXJMarketExchangeResourceMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJMarketExchangeResourceMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_resourcecurcount[i] = str;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_resourcesellcount[i - 4] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		m_resourcemenu[i - 8] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		m_resourceget[i - 12] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		m_exchangebtn[i - 16] = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
		m_countscrollbar[i - 20] = str;
		break;
	case 24:
		m_remainCount = str;
		break;
	case 25:
		m_nextlvCount = str;
		break;
	}

}

void crJXJMarketExchangeResourceMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getDataClass())
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		crData *canvasdata = m_this->getDataClass();
		CRCore::ScopedLock<crData> lock(*canvasdata);
		std::string str;
		int rescount = 0;
		canvasdata->getParam(WCHDATA_JXJResourceCanExchangeVec, param);
		std::vector<int> *canexchangevec = (std::vector<int> *)param;
		canexchangevec->clear();
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char viplv = *(unsigned char *)param;
		crVector4i sellcountvec;
		crVector4f pricevec4f;//所有资源的价格
		float myprice = 0.0f;//自身的价格
		crVector3f pricevec3f[4];//除去自身的价格
		crVector3i resourcetype[4];
		crVector4i getresourcevec;
		int rescount2;
		//获取资源价值
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
		float priceparam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourceExchangeParam, viplv).c_str());
		crStaticTextWidgetNode *remainCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_remainCount));
		crStaticTextWidgetNode *nextlvCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_nextlvCount));
		ref_ptr<crStaticTextWidgetNode> resourcecurcount[4] = { NULL };
		ref_ptr<crStaticTextWidgetNode> resourcesellcount[4] = { NULL };
		ref_ptr<crComboBoxWidgetNode> resourcemenu[4] = { NULL };
		ref_ptr<crStaticTextWidgetNode> resourceget[4] = { NULL };
		ref_ptr<crScrollBarWidgetNode> countscrollbar[4] = { NULL };
		ref_ptr<crButtonWidgetNode> exchangebtn[4] = { NULL };
		data->getParam(WCHDATA_JXJFukulv, param);
		unsigned char fukulv = *(unsigned char *)param;
		ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);
		crTableIO::StrVec record, recordn, recordf;
		fukutab->queryOneRecord(0, crArgumentParser::appItoa((int)fukulv), record);
		int resourcelimitindexp[4];
		resourcelimitindexp[0] = fukutab->getTitleIndex("粮食");
		resourcelimitindexp[1] = fukutab->getTitleIndex("木材");
		resourcelimitindexp[2] = fukutab->getTitleIndex("铁矿");
		resourcelimitindexp[3] = fukutab->getTitleIndex("马匹");
		float fukulimitaddpercent = 0.0f;
		myPlayer->doEvent(WCH_JXJGetFukuLimitAdd, MAKEINT64(&fukulimitaddpercent, NULL));
		int resourcelimit[4];//府库上限
		//crVector3f(pricevec4f[1], pricevec4f[2], pricevec4f[3]);
		//pricevec3f[1] = crVector3f(pricevec4f[0], pricevec4f[2], pricevec4f[3]);
		//pricevec3f[2] = crVector3f(pricevec4f[0], pricevec4f[1], pricevec4f[3]);
		//pricevec3f[3] = crVector3f(pricevec4f[0], pricevec4f[1], pricevec4f[2]);
		int pindex = 0;
		int j = 0;
		for (int i = 0; i < 4; ++i)
		{
			pindex = 0;
			for (j = 0; j < 4; ++j)
			{
				if (j != i && pindex < 3)
				{
					pricevec3f[i][pindex] = pricevec4f[j];
					resourcetype[i][pindex] = j;
					pindex++;
				}
			}
			resourcelimit[i] = atoi(record[resourcelimitindexp[i]].c_str()) * (1.0f + fukulimitaddpercent);
		}
		int getresourcetype = 0;
		for (int i = 0; i < 4; ++i)
		{
			myprice = pricevec4f[i];
			resourcecurcount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_resourcecurcount[i]));
			resourcesellcount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_resourcesellcount[i]));
			resourcemenu[i] = dynamic_cast<crComboBoxWidgetNode *>(m_this->getWidget(m_resourcemenu[i]));
			resourceget[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_resourceget[i]));
			countscrollbar[i] = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_countscrollbar[i]));
			exchangebtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_exchangebtn[i]));
			//显示现有的资源
			data->getParam(WCHDATA_JXJFood + i, param);
			rescount = *(int *)param;
			if (resourcecurcount[i].valid())resourcecurcount[i]->setString(crArgumentParser::appItoa(rescount));
			//获取需要出售的资源
			if (resourcesellcount[i].valid())
			{
				sellcountvec[i] = atoi(resourcesellcount[i]->getUTF8String().c_str());
				sellcountvec[i] = CRCore::minimum(sellcountvec[i], rescount);
				resourcesellcount[i]->setString(crArgumentParser::appItoa(sellcountvec[i]));
			}
			if (resourcemenu[i].valid() && resourceget[i].valid())
			{
				getresourcetype = resourcetype[i][resourcemenu[i]->getSelectIndex()];
				data->getParam(WCHDATA_JXJFood + getresourcetype, param);//获得当前资源总量
				rescount2 = *(int *)param;
				getresourcevec[i] = sellcountvec[i] * myprice / pricevec3f[i][resourcemenu[i]->getSelectIndex()] * priceparam;
				if (getresourcevec[i] + rescount2 > resourcelimit[getresourcetype])
				{
					resourceget[i]->setColor(COLOR_RED / 255.0f);
					canexchangevec->push_back(0);
				}
				else
				{
					resourceget[i]->setColor(COLOR_WHITE / 255.0f);
					canexchangevec->push_back(1);
				}
				resourceget[i]->setString(crArgumentParser::appItoa(getresourcevec[i]));
			}
			if (countscrollbar[i].valid())
			{
				countscrollbar[i]->setRange(0.0f, (float)(rescount/* / 1000 * 1000*/));
				countscrollbar[i]->setLineValue(1.0f);
				countscrollbar[i]->outerSetValue(sellcountvec[i]);
			}
		}

	}
}
////////////////////////////////////////////
//
//crJXJMarketSetCountScrolPosMethod
//
////////////////////////////////////////////
crJXJMarketSetCountScrolPosMethod::crJXJMarketSetCountScrolPosMethod() :
m_this(NULL),
m_min(0.0), m_max(1.0)
{
}
crJXJMarketSetCountScrolPosMethod::crJXJMarketSetCountScrolPosMethod(const crJXJMarketSetCountScrolPosMethod& handle) :
crMethod(handle),
m_this(NULL),
m_min(handle.m_min), m_max(handle.m_max), m_sourceCount(NULL)
{
}

void crJXJMarketSetCountScrolPosMethod::inputParam(int i, void *param)
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
		m_this = (crScrollBarWidgetNode*)param;
		break;
	}
}
void crJXJMarketSetCountScrolPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_min = atof(str.c_str());
		break;
	case 1:
		m_max = atof(str.c_str());
		break;
	case 2:
		m_type = atoi(str.c_str());
		break;
	case 3:
		m_scrollbar = str;
		break;
	case 4:
		m_input_f = str;
		break;
	case 5:
		m_input_w = str;
		break;
	case 6:
		m_input_i = str;
		break;
	case 7:
		m_input_h = str;
		break;
	}
}
void crJXJMarketSetCountScrolPosMethod::operator ()(crHandle &handle)
{
	std::string sellcountinputname[4] = { "UI_Market_ConsignmentResources_Input9", "UI_Market_ConsignmentResources_Input10", 
		"UI_Market_ConsignmentResources_Input11", "UI_Market_ConsignmentResources_Input12" };
	std::string countinputname[4] = { "UI_Market_ConsignmentResources_Input17", "UI_Market_ConsignmentResources_Input18",
		"UI_Market_ConsignmentResources_Input19", "UI_Market_ConsignmentResources_Input20" };
	std::string comboboxname[4] = { "UI_ComboBox1", "UI_ComboBox2", "UI_ComboBox3","UI_ComboBox4" };
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	ref_ptr<crEditWidgetNode>sellsourceCount[4] = { NULL };
	ref_ptr<crStaticTextWidgetNode>getsourceCount[4] = { NULL };
	ref_ptr<crComboBoxWidgetNode>resourcemenu[4] = { NULL };
	ref_ptr<crTableIO>shijiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShijiTab);
	if (myPlayer)
	{
		float myprice = 0.0f;//自身的价格
		crVector3f pricevec3f[4];//除去自身的价格
		crVector4f pricevec4f;//所有资源的价格
		int _value;
		int rescount2;
		int rescount;
		void *param;
		int getrescount;
		crData *data = myPlayer->getDataClass();
		crData *canvasdata = canvas->getDataClass();
		CRCore::ScopedLock<crData> lock(*canvasdata);
		canvasdata->getParam(WCHDATA_JXJResourceCanExchangeVec, param);
		std::vector<int> *canexchangevec = (std::vector<int> *)param;
		crVector3i resourcetype[4];
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char viplv = *(unsigned char *)param;
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
		float priceparam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourceExchangeParam, viplv).c_str());
		for (int i = 0; i < 4; ++i)
		{
			resourcemenu[i] = dynamic_cast<crComboBoxWidgetNode *>(canvas->getWidget(comboboxname[i]));
			sellsourceCount[i] = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(sellcountinputname[i]));
			getsourceCount[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(countinputname[i]));
		}
		myprice = pricevec4f[m_type];
		_value = m_this->getValue();//获得当前出售资源
		data->getParam(WCHDATA_JXJFood + m_type, param);//获得当前资源总量
		rescount = *(int *)param;
		//获取府库上限
		data->getParam(WCHDATA_JXJFukulv, param);
		unsigned char fukulv = *(unsigned char *)param;
		ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);
		crTableIO::StrVec record, recordn, recordf;
		fukutab->queryOneRecord(0, crArgumentParser::appItoa((int)fukulv), record);
		int resourcelimitindexp[4];
		resourcelimitindexp[0] = fukutab->getTitleIndex("粮食");
		resourcelimitindexp[1] = fukutab->getTitleIndex("木材");
		resourcelimitindexp[2] = fukutab->getTitleIndex("铁矿");
		resourcelimitindexp[3] = fukutab->getTitleIndex("马匹");
		float fukulimitaddpercent = 0.0f;
		myPlayer->doEvent(WCH_JXJGetFukuLimitAdd, MAKEINT64(&fukulimitaddpercent, NULL));
		int resourcelimit[4];//府库上限
		int pindex = 0;
		int j = 0;
		for (int i = 0; i < 4; ++i)
		{
			pindex = 0;
			for (j = 0; j < 4; ++j)
			{
				if (j != i)
				{
					pricevec3f[i][pindex] = pricevec4f[j];
					resourcetype[i][pindex] = j;
					pindex++;
				}
			}
			resourcelimit[i] = atoi(record[resourcelimitindexp[i]].c_str()) * (1.0f + fukulimitaddpercent);
		}
		int getresourcetype = resourcetype[m_type][resourcemenu[m_type]->getSelectIndex()];
		if (getsourceCount[m_type].valid())
		{
			data->getParam(WCHDATA_JXJFood + getresourcetype, param);//获得当前资源总量
			rescount2 = *(int *)param;
			getrescount = _value * myprice / pricevec3f[m_type][resourcemenu[m_type]->getSelectIndex()] * priceparam;
			if (getrescount + rescount2 > resourcelimit[getresourcetype])
			{
				getsourceCount[m_type]->setColor(COLOR_RED / 255.0f);
				if (m_type<canexchangevec->size())(*canexchangevec)[m_type] = 0;
			}
			else
			{
				getsourceCount[m_type]->setColor(COLOR_WHITE / 255.0f);
				if (m_type<canexchangevec->size())(*canexchangevec)[m_type] = 1;
			}
			getsourceCount[m_type]->setString(crArgumentParser::appItoa(getrescount));
		}
		if (sellsourceCount[m_type].valid())sellsourceCount[m_type]->setString(crArgumentParser::appItoa(_value));

	}
}
/////////////////////////////////////////
//
//crJXJUIMarketExchangeResMethod
//
/////////////////////////////////////////
crJXJUIMarketExchangeResMethod::crJXJUIMarketExchangeResMethod() :
m_index(0),
m_font("simhei.ttf"),
m_characterSize(16),
m_goldColor(0.0f, 1.0f, 1.0f, 1.0f),
m_textColor(1.0f, 1.0f, 0.0f, 1.0f)
{
}

crJXJUIMarketExchangeResMethod::crJXJUIMarketExchangeResMethod(const crJXJUIMarketExchangeResMethod& handle) :
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

void crJXJUIMarketExchangeResMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIMarketExchangeResMethod::addParam(int i, const std::string& str)
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
		crArgumentParser::appAtoVec(str, m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str, m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}

void crJXJUIMarketExchangeResMethod::operator()(crHandle &handle)
{
	void *param;
	std::string resource1name, resource2name;
	std::string sellcountinputname[4] = { "UI_Market_ConsignmentResources_Input9", "UI_Market_ConsignmentResources_Input10",
		"UI_Market_ConsignmentResources_Input11", "UI_Market_ConsignmentResources_Input12" };
	std::string countinputname[4] = { "UI_Market_ConsignmentResources_Input17", "UI_Market_ConsignmentResources_Input18",
		"UI_Market_ConsignmentResources_Input19", "UI_Market_ConsignmentResources_Input20" };
	std::string comboboxname[4] = { "UI_ComboBox1", "UI_ComboBox2", "UI_ComboBox3", "UI_ComboBox4" };
	std::string exchangebtnname[4] = { "UI_Market_ConsignmentResources_Sell1", "UI_Market_ConsignmentResources_Sell2",
		"UI_Market_ConsignmentResources_Sell3", "UI_Market_ConsignmentResources_Sell4" };
	std::string gamelistname[4] = { "Gamelist1", "Gamelist2",
		"Gamelist3", "Gamelist4" };

	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && playerGameData)
	{
		int btnindex = 0;
		std::string btnname = m_this->getName();
		for (int i = 0; i < 4; ++i)
		{
			if (btnname.compare(exchangebtnname[i]) == 0)
			{
				btnindex = i;
				break;
			}
		}
		ref_ptr<crCanvasNode>marketcanvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid() && marketcanvas.valid())
		{
			unsigned char sellsourcetype = 0;
			int sellsourcecount = 0;
			unsigned char getsourcetype = 0;
			crData *mcanvasdata = marketcanvas->getDataClass();
			mcanvasdata->getParam(WCHDATA_JXJResourceCanExchangeVec, param);
			std::vector<int> *canexchangevec = (std::vector<int> *)param;
			if (btnindex < canexchangevec->size() && (*canexchangevec)[btnindex]!=0)
			{
				ref_ptr<crTableWidgetNode> gamelist = dynamic_cast<crTableWidgetNode *>(marketcanvas->getWidget(gamelistname[btnindex]));
				ref_ptr<crEditWidgetNode>sellsourceCount = dynamic_cast<crEditWidgetNode *>(marketcanvas->getWidget(sellcountinputname[btnindex]));
				ref_ptr<crStaticTextWidgetNode>getsourceCount = dynamic_cast<crStaticTextWidgetNode *>(marketcanvas->getWidget(countinputname[btnindex]));
				ref_ptr<crComboBoxWidgetNode>resourcemenu = dynamic_cast<crComboBoxWidgetNode *>(marketcanvas->getWidget(comboboxname[btnindex]));
				ref_ptr<crButtonWidgetNode>exchangebtn = dynamic_cast<crButtonWidgetNode *>(marketcanvas->getWidget(exchangebtnname[btnindex]));
				if (sellsourceCount.valid())
				{
					resource1name = sellsourceCount->getUTF8String();
					sellsourcecount = atoi(resource1name.c_str());
				}
				if (getsourceCount.valid())
				{
					resource2name = getsourceCount->getUTF8String();
				}
				switch (btnindex)
				{
				case 0:
					resource1name += "粮食";
					sellsourcetype = T_Food;
					break;
				case 1:
					resource1name += "木材";
					sellsourcetype = T_Wood;
					break;
				case 2:
					resource1name += "铁矿";
					sellsourcetype = T_Iron;
					break;
				case 3:
					resource1name += "马匹";
					sellsourcetype = T_Horse;
					break;
				default:
					break;
				}
				if (resourcemenu.valid() && gamelist.valid())
				{
					switch (gamelist->getData(resourcemenu->getSelectIndex()))
					{
					case 0:
						resource2name += "粮食";
						getsourcetype = T_Food;
						break;
					case 1:
						resource2name += "木材";
						getsourcetype = T_Wood;
						break;
					case 2:
						resource2name += "铁矿";
						getsourcetype = T_Iron;
						break;
					case 3:
						resource2name += "马匹";
						getsourcetype = T_Horse;
						break;
					default:
						break;
					}
				}

				mcanvasdata->inputParam(WCHDATA_JXJSellResourceType, &sellsourcetype);
				mcanvasdata->inputParam(WCHDATA_JXJSellResourceCount, &sellsourcecount);
				mcanvasdata->inputParam(WCHDATA_JXJGetResourceType, &getsourcetype);
				std::string buildingname;
				crData *data = playerGameData->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				int price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourceExchangePrice, viplv).c_str());
				int tempprice = price;
				playerGameData->doEvent(WCH_JXJCheckLijinGold, MAKEINT64(&tempprice, NULL));
				//data->getParam(WCHDATA_JXJGoldCostHintInt, param);
				//unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_MarketExchangeRes;
				if (sellsourcecount > 0)
				{
					if (tempprice == 0)
					{
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
						//if (ifhint)
						{
							int hinttype = -1;// JXJGoldCostType_MarketExchangeRes;
							crData *braindata = crBrain::getInstance()->getDataClass();
							braindata->inputParam(WCHDATA_JXJCurGoldHintType, &hinttype);
							std::vector<float>v_i;
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							//cfg_script.Add("Font",m_font);
							//cfg_script.Add("CharacterSize",m_characterSize);

							cfg_script.Add("Content");
							cfg_script.Push("Content", 1);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "您确认要消耗");
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
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "和");
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 4);
							v_i.clear();
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", resource1name.c_str());
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 5);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "交换");
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 6);
							v_i.clear();
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", resource2name.c_str());
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 7);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "吗？");
							cfg_script.Pop();

							cfg_script.Pop();

							if (information.valid())information->setHypertext(cfg_script);
						}
						if (okbtn.valid())okbtn->setEnable(true);
						crData *dlgData = canvas->getDataClass();
						int commandtype = CDP_Widget;
						dlgData->inputParam(WCHDATA_CommandDlgParentType, &commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent, m_this);
						//if (ifhint)
						//{
						crFilterRenderManager::getInstance()->doModal(canvas.get());
						//}
						//else
						//{
						//	if (okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						//}
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2036, NULL));//元宝不足
					}
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2104, NULL));//资源购买后超过府库上线
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJMarketExchangeResMethod
//
/////////////////////////////////////////
crJXJMarketExchangeResMethod::crJXJMarketExchangeResMethod() :
m_this(NULL)
{
}
crJXJMarketExchangeResMethod::crJXJMarketExchangeResMethod(const crJXJMarketExchangeResMethod& handle) :
crMethod(handle)
{
}
void crJXJMarketExchangeResMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJMarketExchangeResMethod::addParam(int i, const std::string& str)
{
}
void crJXJMarketExchangeResMethod::operator()(crHandle &handle)
{
	void *param;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJSellResourceType, param);
		unsigned char sellrestype = *(unsigned char *)param;
		mcanvasdata->getParam(WCHDATA_JXJSellResourceCount, param);
		int sellrescount = *(int *)param;
		mcanvasdata->getParam(WCHDATA_JXJGetResourceType, param);
		unsigned char exchangerestype = *(unsigned char *)param;
		if (sellrestype != exchangerestype)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(6);
			stream->_writeUChar(sellrestype);
			stream->_writeInt(sellrescount);
			stream->_writeUChar(exchangerestype);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvMarketResexchange, stream.get());
			netConductor->getNetManager()->sendPacket("all", packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvMarketExchangeResMethod
//
/////////////////////////////////////////
crJXJRecvMarketExchangeResMethod::crJXJRecvMarketExchangeResMethod() :
m_netType(GameClient_Game){}
crJXJRecvMarketExchangeResMethod::crJXJRecvMarketExchangeResMethod(const crJXJRecvMarketExchangeResMethod& handle)
{
}
void crJXJRecvMarketExchangeResMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvMarketExchangeResMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvMarketExchangeResMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				unsigned char sellrestype = m_stream->_readUChar();
				int sellrescount = m_stream->_readInt();
				unsigned char exchangerestype = m_stream->_readUChar();
				char success = 0;
				void *param;
				bool needdeductgold = false;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				crVector4f pricevec4f;
				int price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourceExchangePrice, viplv).c_str());
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
				float priceparam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourceExchangeParam, viplv).c_str());
				//获取府库上限
				data->getParam(WCHDATA_JXJFukulv, param);
				unsigned char fukulv = *(unsigned char *)param;
				ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);
				crTableIO::StrVec record, recordn, recordf;
				fukutab->queryOneRecord(0, crArgumentParser::appItoa((int)fukulv), record);
				int resourcelimitindexp[4];
				resourcelimitindexp[0] = fukutab->getTitleIndex("粮食");
				resourcelimitindexp[1] = fukutab->getTitleIndex("木材");
				resourcelimitindexp[2] = fukutab->getTitleIndex("铁矿");
				resourcelimitindexp[3] = fukutab->getTitleIndex("马匹");
				float fukulimitaddpercent = 0.0f;
				m_this->doEvent(WCH_JXJGetFukuLimitAdd, MAKEINT64(&fukulimitaddpercent, NULL));
				int resourcelimit;//府库上限
				int getrescount = 0;
				do 
				{
					if (sellrestype <= T_Horse && exchangerestype <= T_Horse)
					{
						resourcelimit = atoi(record[resourcelimitindexp[exchangerestype - T_Food]].c_str()) * (1.0f + fukulimitaddpercent);
						if (pricevec4f[sellrestype - T_Food] != 0)
						{
							//获得当前资源总量
							getrescount = sellrescount * pricevec4f[sellrestype - T_Food] / pricevec4f[exchangerestype - T_Food] * priceparam;
							if (getrescount == 0)
							{
								success = -3;//输入的数量有误
								break;
							}
							data->getParam(WCHDATA_JXJFood + exchangerestype - T_Food, param);
							int currescount = *(int *)param;
							if (currescount + getrescount > resourcelimit)
							{
								success = -1;//超过府库上限
								break;
							}
						}
						else
						{
							break;
						}
					}
					MoneyChangeData moneydata(price, "市场交换资源");
					m_this->doEvent(WCH_JXJDeductLijinGold, MAKEINT64(&moneydata, &needdeductgold));
					if (moneydata.first == 0)
					{
						success = 1;
					}
					else
					{
						success = -2;//元宝不足
					}
				} while (0);	
				std::string resname1;
				std::string resname2;//
				if (success == 1)
				{
					switch (sellrestype)
					{
					case T_Food:
						m_this->doEvent(WCH_JXJRecvFood, MAKEINT64(-sellrescount, NULL));
						resname1 = "粮食";
						break;
					case T_Wood:
						m_this->doEvent(WCH_JXJRecvWood, MAKEINT64(-sellrescount, NULL));
						resname1 = "木材";
						break;
					case T_Iron:
						m_this->doEvent(WCH_JXJRecvIron, MAKEINT64(-sellrescount, NULL));
						resname1 = "铁矿";
						break;
					case T_Horse:
						m_this->doEvent(WCH_JXJRecvHorse, MAKEINT64(-sellrescount, NULL));
						resname1 = "马匹";
						break;
					default:
						break;
					}
					switch (exchangerestype)
					{
					case T_Food:
						m_this->doEvent(WCH_JXJRecvFood, MAKEINT64(getrescount, NULL));
						resname2 = "粮食";
						break;
					case T_Wood:
						m_this->doEvent(WCH_JXJRecvWood, MAKEINT64(getrescount, NULL));
						resname2 = "木材";
						break;
					case T_Iron:
						m_this->doEvent(WCH_JXJRecvIron, MAKEINT64(getrescount, NULL));
						resname2 = "铁矿";
						break;
					case T_Horse:
						m_this->doEvent(WCH_JXJRecvHorse, MAKEINT64(getrescount, NULL));
						resname2 = "马匹";
						break;
					default:
						break;
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(success);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvMarketResexchange, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				if (needdeductgold)
				{//扣除了元宝，立即存储
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				if (success > 0)
				{///游戏日志
					std::string logdata = "使用" + crArgumentParser::appItoa(sellrescount) + resname1 + "交换了" + crArgumentParser::appItoa(getrescount) + resname2;
					GameLogData gamelog(Log_MarketResExchange, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			int tipsindex = 0;

			if (success == 1)
			{
				crData *braindata = crBrain::getInstance()->getDataClass();
				CRCore::ScopedLock<crData> lock(*braindata);
				braindata->getParam(WCHDATA_JXJTotalExchangeResCount, param);
				int *totalexcount = (int *)param;
				(*totalexcount) += 1;
			}
			else
			{
				switch (success)
				{
				case -1://超过府库上限
					tipsindex = 2104;
					break;
				case -2://元宝不足
					tipsindex = 2036;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}
///////////////////////////////////////////
////
////crJXJHuoyueduUpdateMethod
////
///////////////////////////////////////////
//crJXJHuoyueduUpdateMethod::crJXJHuoyueduUpdateMethod() :
//	m_dt(NULL)
//{
//}
//crJXJHuoyueduUpdateMethod::crJXJHuoyueduUpdateMethod(const crJXJHuoyueduUpdateMethod& handle) :
//	crMethod(handle),
//	m_dt(NULL)
//{
//}
//void crJXJHuoyueduUpdateMethod::inputParam(int i, void *param)
//{
//	switch(i)
//	{
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			int lparam = LOINT64(param64);
//			m_dt = (float*)lparam;
//		}
//		else
//		{
//			m_dt = NULL;
//		}
//		break;
//	}
//}
//void crJXJHuoyueduUpdateMethod::addParam(int i, const std::string& str)
//{
//}
//void crJXJHuoyueduUpdateMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(myPlayer && myPlayer->getMainRole())
//	{
//		crData *data = myPlayer->getDataClass();
//		crData *roleData = myPlayer->getMainRole()->getDataClass();
//		void *param;
//		data->excHandle(MAKEINT64(WCH_LockData,1));
//		roleData->excHandle(MAKEINT64(WCH_LockData,1));
//		roleData->getParam(WCHDATA_JXJHuoyueduMap,param);
//		HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
//		for (HuoyueduMap::iterator itr = huoyuedumap->begin();
//			itr != huoyuedumap->end();
//			 ++itr )
//		{
//			if(itr->second!=NULL && !itr->second->isComplete())
//				itr->second->doEvent(WCH_UPDATE,MAKEINT64((int)(m_dt),NULL));
//		}
//		roleData->excHandle(MAKEINT64(WCH_LockData,0));
//		data->excHandle(MAKEINT64(WCH_LockData,0));
//	}
//}
/////////////////////////////////////////
//
//crJXJRecvHuoyueduMethod
//
/////////////////////////////////////////
crJXJRecvHuoyueduMethod::crJXJRecvHuoyueduMethod() :
m_this(NULL),
m_id(0),
m_addstep(0)
{
}

crJXJRecvHuoyueduMethod::crJXJRecvHuoyueduMethod(const crJXJRecvHuoyueduMethod& handle) :
crMethod(handle),
m_this(NULL),
m_id(0),
m_addstep(0)
{
}

void crJXJRecvHuoyueduMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_id = NULL;
			m_addstep = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_id = LOINT64(param64);
			m_addstep = HIINT64(param64);
		}
		else
		{
			m_id = NULL;
			m_addstep = NULL;
		}
		break;
	}
}

void crJXJRecvHuoyueduMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvHuoyueduMethod::operator()(crHandle &handle)
{
	if (m_this && m_id != 0 && m_addstep != 0)
	{
		crData *data = m_this->getDataClass();
		if (data)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if (playerData.valid() && mainRole && mainRole->getDataClass())
			{
				ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
				crTableIO::StrVec record;
				int needlv = HuoyueduTab->getTitleIndex("等级");
				if (HuoyueduTab->queryOneRecord(0, crArgumentParser::appItoa(m_id), record) >= 0 && needlv > 0)
				{
					void *param;
					crData *mainroledata = mainRole->getDataClass();
					//mainroledata->getParam(WCHDATA_Level, param);
					//unsigned char mylv = *(unsigned char *)param;
					//if (mylv >= atoi(record[needlv].c_str()))
					{
						unsigned char step = 0;
						bool iscomplete = false;
						mainroledata->excHandle(MAKEINT64(WCH_LockData, 1));
						mainroledata->getParam(WCHDATA_JXJHuoyueduMap, param);
						HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
						HuoyueduMap::iterator itr = huoyuedumap->find(m_id);
						if (itr != huoyuedumap->end())
						{
							iscomplete = itr->second->isComplete();
							if (!iscomplete)
							{//
								step = itr->second->getStep() + m_addstep;
								itr->second->setStep(step);
								if (iscomplete)
								{
									itr->second->setComplete(iscomplete);
								}
								//返回信息
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(4);
								stream->_writeUShort(m_id);
								stream->_writeUChar(step);
								stream->_writeBool(iscomplete);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvHuoyueduUpdate, stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
							}
						}
						mainroledata->excHandle(MAKEINT64(WCH_LockData, 0));
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvAccomplishCompleteMethod
//
/////////////////////////////////////////
crJXJRecvHuoyueduUpdateMethod::crJXJRecvHuoyueduUpdateMethod() :
m_time(5.0f),
m_netType(GameClient_Game){}
crJXJRecvHuoyueduUpdateMethod::crJXJRecvHuoyueduUpdateMethod(const crJXJRecvHuoyueduUpdateMethod& handle) :
crMethod(handle),
m_time(handle.m_time),
m_canvas(handle.m_canvas)
{
}
void crJXJRecvHuoyueduUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvHuoyueduUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvHuoyueduUpdateMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			crRole *mainRole = m_this->getMainRole();
			if (mainRole && mainRole->getDataClass())
			{
				unsigned short accid = m_stream->_readUShort();
				unsigned char step = m_stream->_readUChar();
				bool complete = m_stream->_readBool();
				void *param;
				crData *data = mainRole->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData, 1));

				data->getParam(WCHDATA_JXJHuoyueduMap, param);
				HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
				HuoyueduMap::iterator itr = huoyuedumap->find(accid);
				if (itr != huoyuedumap->end())
				{
					itr->second->setStep(step);
					itr->second->setComplete(complete);
				}
				data->excHandle(MAKEINT64(WCH_LockData, 0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJGetHuoyueduRewardIDMethod
//
/////////////////////////////////////////
crJXJGetHuoyueduRewardIDMethod::crJXJGetHuoyueduRewardIDMethod() :
m_this(NULL)
{
}
crJXJGetHuoyueduRewardIDMethod::crJXJGetHuoyueduRewardIDMethod(const crJXJGetHuoyueduRewardIDMethod& handle) :
crMethod(handle)
{
}
void crJXJGetHuoyueduRewardIDMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJGetHuoyueduRewardIDMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetHuoyueduRewardIDMethod::operator()(crHandle &handle)
{
	if (m_this && m_this->getParentCanvas())
	{
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		if (mcanvasdata)
		{
			std::string getrewardbtnname[6] = { "XGJ_Linqujiangli1", "XGJ_Linqujiangli2", "XGJ_Linqujiangli3",
				"XGJ_Linqujiangli4", "XGJ_Linqujiangli5", "XGJ_Linqujiangli6" };
			void *param;
			int index = 0;
			unsigned short getrewardid = 0;
			std::string thisname = m_this->getName();
			for (int i = 0; i < 6; ++i)
			{
				if (getrewardbtnname[i].compare(thisname) == 0)
				{
					index = i;
					break;
				}
			}
			mcanvasdata->getParam(WCHDATA_JXJHuoyueduRewardVec, param);
			HuoyueduRewardVec *rewardidvec = (HuoyueduRewardVec *)param;
			if (rewardidvec && index < rewardidvec->size())
			{
				getrewardid = (*rewardidvec)[index];
			}
			mcanvasdata->inputParam(WCHDATA_JXJHuoyueduRewardID, &getrewardid);
		}
	}
}
/////////////////////////////////////////
//
//crJXJGetHuoyueduRewardMethod
//
/////////////////////////////////////////
crJXJGetHuoyueduRewardMethod::crJXJGetHuoyueduRewardMethod() :
m_this(NULL)
{
}
crJXJGetHuoyueduRewardMethod::crJXJGetHuoyueduRewardMethod(const crJXJGetHuoyueduRewardMethod& handle) :
crMethod(handle)
{
}
void crJXJGetHuoyueduRewardMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJGetHuoyueduRewardMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetHuoyueduRewardMethod::operator()(crHandle &handle)
{
	void *param;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		unsigned short rewardid = 0;
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJHuoyueduRewardID, param);
		rewardid = *(unsigned short *)param;
		if (rewardid > 0)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			stream->_writeUShort(rewardid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvHuoyueduReward, stream.get());
			netConductor->getNetManager()->sendPacket("all", packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvHuoyueduRewardMethod
//
/////////////////////////////////////////
crJXJRecvHuoyueduRewardMethod::crJXJRecvHuoyueduRewardMethod() :
m_netType(GameClient_Game){}
crJXJRecvHuoyueduRewardMethod::crJXJRecvHuoyueduRewardMethod(const crJXJRecvHuoyueduRewardMethod& handle) :
crMethod(handle)
{
}
void crJXJRecvHuoyueduRewardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvHuoyueduRewardMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvHuoyueduRewardMethod::operator()(crHandle &handle)
{
	if (m_this && m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			char success = 0;
			RewardItemVec rewardItems;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				unsigned short rewardid = m_stream->_readUShort();
				crData *data = m_this->getDataClass();
				crData *mainroledata = m_this->getMainRole()->getDataClass();
				if (rewardid > 0)
				{
					int itemidindex[3],itemtypeindex[3];
					crTableIO::StrVec record;
					ref_ptr<crTableIO>HuoyueduRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduRewardTab);
					int huoyuezhiindex = HuoyueduRewardTab->getTitleIndex("活跃值");
					int dengjiindex = HuoyueduRewardTab->getTitleIndex("等级");
					itemtypeindex[0] = HuoyueduRewardTab->getTitleIndex("类型1");
					itemtypeindex[1] = HuoyueduRewardTab->getTitleIndex("类型2");
					itemtypeindex[2] = HuoyueduRewardTab->getTitleIndex("类型3");
					itemidindex[0] = HuoyueduRewardTab->getTitleIndex("奖励1");
					itemidindex[1] = HuoyueduRewardTab->getTitleIndex("奖励2");
					itemidindex[2] = HuoyueduRewardTab->getTitleIndex("奖励3");
					if (huoyuezhiindex > 0 && itemidindex[0] > 0 && itemidindex[1] > 0 && itemidindex[2] > 0
						&& HuoyueduRewardTab->queryOneRecord(0, crArgumentParser::appItoa(rewardid), record) >= 0
						&& itemtypeindex[0] > 0 && itemtypeindex[1] > 0 && itemtypeindex[2] > 0)
					{
						data->excHandle(MAKEINT64(WCH_LockData,1));
						mainroledata->excHandle(MAKEINT64(WCH_LockData,1));
						mainroledata->getParam(WCHDATA_JXJHuoyueduMap, param);
						HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
						mainroledata->getParam(WCHDATA_JXJHuoyueduRewardSet, param);
						HuoyueduRewardSet *rewardset = (HuoyueduRewardSet *)param;
						data->getParam(WCHDATA_JXJVipLv, param);
						unsigned char viplv = *(unsigned char *)param;
						mainroledata->getParam(WCHDATA_Level, param);
						unsigned char mylv = *(unsigned char *)param;
						unsigned char huoyueduopenlv = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJHuoyueduOpenlv, viplv).c_str());
						crVector3i itemid[3];
						int itemtype[3];
						for (int i = 0; i < 3; ++i)
						{
							crArgumentParser::appAtoVec(record[itemidindex[i]], itemid[i]);
							itemtype[i] = atoi(record[itemtypeindex[i]].c_str());
						}
						int myhuoyuezhi = 0;
						unsigned char steps = 0;
						unsigned short point = 0;
						int huoyuezhi = atoi(record[huoyuezhiindex].c_str());
						for (HuoyueduMap::iterator hyditr = huoyuedumap->begin();
							hyditr != huoyuedumap->end();++hyditr)
						{
							steps = hyditr->second->getStep();
							point = hyditr->second->getPoint();
							if (steps != 0 && point != 0)
							{
								myhuoyuezhi += steps * point;
							}
							
						}
						data->getParam(WCHDATA_JXJPlayerCycleCount, param);
						short cycleCount = *(short *)param;
						do
						{
							if (mylv < huoyueduopenlv || mylv < atoi(record[dengjiindex].c_str()))
							{
								success = -1;//等级不足
								break;
							}
							if (myhuoyuezhi < huoyuezhi)
							{
								success = -2;//活跃值不足
								break;
							}
							HuoyueduRewardSet::iterator hritr = rewardset->find(rewardid);
							if (hritr != rewardset->end())
							{
								success = -3;//已经领取过
								break;
							}
							if (success == 0)
							{
								MoneyChangeData  moneydata;//
								success = 1;
								rewardset->insert(rewardid);
								ref_ptr<crBagItemData>bagitemdata = NULL;
								for (int i = 0; i < 3; ++i)
								{
									switch (itemtype[i])
									{
									case T_Exp:
										m_this->getMainRole()->doEvent(WCH_JXJRecvExperience, MAKEINT64(itemid[i][0], NULL));
										break;
									case T_VIPExp:
										m_this->doEvent(WCH_JXJAddVipExp, MAKEINT64(itemid[i][0] * (cycleCount + 1), NULL));
										break;
									case T_Copper:
										m_this->doEvent(WCH_JXJRecvCoppercash, MAKEINT64(itemid[i][0], NULL));
										break;
									case T_Exploit:
										m_this->getMainRole()->doEvent(WCH_JXJRecvExploit, MAKEINT64(itemid[i][0], NULL));
										break;
									case T_Giftgold:
										moneydata.first = itemid[i][0];
										moneydata.second = "小管家奖励";
										m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
										break;
									case T_Item:
										if (itemid[i][0] > 0)
										{
											bagitemdata = new crBagItemData;
											bagitemdata->setItemID(itemid[i][0]);
											bagitemdata->setEquipMagic(itemid[i][1]);
											bagitemdata->setItemCount(itemid[i][2]);
											rewardItems.push_back(bagitemdata);
										}
										break;
									}
								}
							}
						} while (0);
						mainroledata->excHandle(MAKEINT64(WCH_LockData,0));
						data->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(3);
					stream->_writeChar(success);
					stream->_writeUShort(rewardid);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				//data->excHandle(MAKEINT64(WCH_LockData, 0));
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvHuoyueduReward, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				///游戏日志
				if (success == 1)
				{
					data->excHandle(MAKEINT64(WCH_LockData, 1));
					m_this->doEvent(WCH_JXJRecvRewardItems, MAKEINT64(&rewardItems, 0));
					data->excHandle(MAKEINT64(WCH_LockData, 0));
				}
				std::string logdata = "获取活跃度奖励(success, rewardid)："+ crArgumentParser::appVectoa(crVector2i(success, rewardid));
				GameLogData gamelog(Log_HuoyueduReward, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				unsigned short rewardid = m_stream->_readUShort();
				void *param;
				crData *mainroledata = m_this->getMainRole()->getDataClass();
				mainroledata->getParam(WCHDATA_JXJHuoyueduRewardSet, param);
				HuoyueduRewardSet *rewardset = (HuoyueduRewardSet *)param;
				rewardset->insert(rewardid);
			}
			else
			{
				int tipsindex = 0;
				switch (success)
				{
				case -1:
					tipsindex = 2105;
					break;
				case -2:
					tipsindex = 2106;
					break;
				case -3:
					tipsindex = 2107;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}
////////////////////////////////
//
//crPageValueChangeNoUpdateMethod
//切页但不刷新窗口
///////////////////////////////////
crPageValueChangeNoUpdateMethod::crPageValueChangeNoUpdateMethod() :
m_this(NULL)
{}

crPageValueChangeNoUpdateMethod::crPageValueChangeNoUpdateMethod(const crPageValueChangeNoUpdateMethod& handle) :
crMethod(handle),
m_this(NULL),
m_pageSW(handle.m_pageSW)
{
}

void crPageValueChangeNoUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crRadioGroupWidgetNode*)param;
		break;
	}
}

void crPageValueChangeNoUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pageSW = str;
		break;
	}
}

void crPageValueChangeNoUpdateMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crCanvasNode *canvas = m_this->getParentCanvas();
		crMultiSwitch *pageSW = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_pageSW));
		if (pageSW)
			pageSW->setActiveSwitchSet(m_this->getSelect());
		//canvas->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crHuoyueduPageUpdateMethod
//
///////////////////////////////////
crHuoyueduPageUpdateMethod::crHuoyueduPageUpdateMethod() :
m_this(NULL)
{}

crHuoyueduPageUpdateMethod::crHuoyueduPageUpdateMethod(const crHuoyueduPageUpdateMethod& handle) :
crMethod(handle),
m_this(NULL)
{
	m_gamelist = handle.m_gamelist;
	m_progress = handle.m_progress;
	m_progressinput = handle.m_progressinput;
	m_lvcontrolsw = handle.m_lvcontrolsw;
	for (int i = 0; i < 18; ++i)
	{
		m_rewardicon[i] = handle.m_rewardicon[i];
		m_rewardcountinput[i] = handle.m_rewardcountinput[i];
		if (i < 6)
		{
			m_rewardbtnsw[i] = handle.m_rewardbtnsw[i];
			m_rewardbtn[i] = handle.m_rewardbtn[i];
			m_rewardinput[i] = handle.m_rewardinput[i];
		}
		
	}
}

void crHuoyueduPageUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crHuoyueduPageUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gamelist = str;
		break;
	case 1:
		m_progress = str;
		break;
	case 2:
		m_progressinput = str;
		break;
	default:
		break;
	}
	if (i > 2 && i <= 20)
	{
		m_rewardicon[i - 3] = str;
	}
	else if (i > 20 && i <= 38)
	{
		m_rewardcountinput[i - 21] = str;
	}
	else if (i > 38 && i <= 45)
	{
		if(i!=45)
			m_rewardbtnsw[i - 39] = str;
		else
			m_lvcontrolsw=str;
	}
	else if (i > 45 && i <= 51)
	{
		m_rewardbtn[i - 46] = str;
	}
	else if (i > 51 && i <= 57)
	{
		m_rewardinput[i - 52] = str;
	}
}

void crHuoyueduPageUpdateMethod::operator()(crHandle &handle)
{
	void* param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && myPlayer && myPlayer->getMainRole() && myPlayer->getDataClass())
	{
		//ref_ptr<crTableWidgetNode> gamelist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_gamelist));
		//if (!gamelist.valid())
		//{
		//	return;
		//}
		//gamelist->clearData();
		ref_ptr<crProgressWidgetNode> progress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_progress));
		ref_ptr<crStaticTextWidgetNode> progressinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_progressinput));
		ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
		ref_ptr<crTableIO>HuoyueduRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduRewardTab);
		ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int informationindex = HuoyueduTab->getTitleIndex("说明");
		int iconnameindex = itemTab->getTitleIndex("icon");
		crTableIO::StrVec hydrecord, rewardrecord, itemrecord;
		ref_ptr<crImageBoxWidgetNode> rewardicon[18] = { NULL };
		ref_ptr<crNode> rewardiconbk[18] = { NULL };
		ref_ptr<crStaticTextWidgetNode> rewardcountinput[18] = { NULL };
		ref_ptr<crMultiSwitch> rewardbtnsw[6] = { NULL };
		ref_ptr<crButtonWidgetNode> rewardbtn[6] = { NULL };
		ref_ptr<crStaticTextWidgetNode> rewardinput[6] = { NULL };
		ref_ptr<crStaticTextWidgetNode> huoyuedulistinput[4][14] = { NULL };
		std::string listinputname[4];
		listinputname[0] = "XGJ_Input28_";
		listinputname[1] = "XGJ_Input29_";
		listinputname[2] = "XGJ_Input30_";
		listinputname[3] = "XGJ_Input31_";
		std::string rewardiconbkname[18];
		ref_ptr<crMultiSwitch> lvcontrolsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lvcontrolsw));
		lvcontrolsw->setActiveSwitchSet(0);

		for (int i = 0; i < 18; ++i)
		{
			rewardiconbkname[i] = "IconBackboard" + crArgumentParser::appItoa(i + 1);
		}
		for (int i = 0; i < 18; ++i)
		{
			rewardiconbk[i] = m_this->getChildNode(rewardiconbkname[i]);
			rewardicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_rewardicon[i]));
			rewardcountinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_rewardcountinput[i]));
			if (rewardicon[i].valid())
			{
				rewardicon[i]->setVisiable(false);
			}
			if (rewardiconbk[i].valid())
			{
				rewardiconbk[i]->setVisiable(false);
			}
			if (rewardcountinput[i].valid())
			{
				rewardcountinput[i]->clearString();
			}
			if (i < 4)
			{
				for (int ii = 0; ii < 14; ++ii)
				{
					huoyuedulistinput[i][ii] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(listinputname[i] + crArgumentParser::appItoa(ii)));
					if (huoyuedulistinput[i][ii].valid())
					{
						huoyuedulistinput[i][ii]->setColor(COLOR_BLACK / 255.0f);
						huoyuedulistinput[i][ii]->clearString();
					}
				}
			}
			if (i < 6)
			{
				rewardbtnsw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_rewardbtnsw[i]));
				rewardbtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rewardbtn[i]));
				rewardinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_rewardinput[i]));
				if (rewardbtnsw[i].valid())
				{
					rewardbtnsw[i]->setActiveSwitchSet(0);

				}
				if (rewardbtn[i].valid())
				{
					rewardbtn[i]->setEnable(false);
				}
				if (rewardinput[i].valid())
				{
					rewardinput[i]->clearString();
				}
			}	
		}
		crData *canvasdata = m_this->getDataClass();
		crData *data = myPlayer->getDataClass();
		crData *mainroledata = myPlayer->getMainRole()->getDataClass();
		CRCore::ScopedLock<crData> lock(*mainroledata);
		mainroledata->getParam(WCHDATA_Level, param);
		unsigned char mylv = *(unsigned char *)param;
		mainroledata->getParam(WCHDATA_JXJHuoyueduMap, param);
		HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
		mainroledata->getParam(WCHDATA_JXJHuoyueduRewardSet, param);
		HuoyueduRewardSet *rewardset = (HuoyueduRewardSet *)param;
		int mytotalhuoyuedu = 0;
		int totalhuoyuedu = 0;
		unsigned char curstep = 0;
		unsigned char totalstep = 0;
		unsigned short perhuoyuedu = 0;
		int index = 0;
		//左边的活跃度统计
		typedef std::vector<std::pair<unsigned short, CRCore::ref_ptr<crJXJHuoyuedu>>> HuoyueduVec;
		HuoyueduVec completevec;
		HuoyueduVec::iterator completeitr;
		HuoyueduVec uncompletevec;
		HuoyueduVec::iterator uncompleteitr;
		for (HuoyueduMap::iterator hyditr = huoyuedumap->begin();
			hyditr != huoyuedumap->end();
			++hyditr)
		{
			if (hyditr->second->isComplete())
			{
				completevec.push_back(std::make_pair(hyditr->first, hyditr->second));
			}
			else
			{
				uncompletevec.push_back(std::make_pair(hyditr->first, hyditr->second));
			}
		}
		for (uncompleteitr = uncompletevec.begin();
			uncompleteitr != uncompletevec.end() && index < 14;
			++uncompleteitr)
		{
			if (HuoyueduTab->queryOneRecord(0, crArgumentParser::appItoa(uncompleteitr->first), hydrecord) >= 0)
			{
				if (uncompleteitr->second->getNeedlv() <= mylv)
				{
					curstep = uncompleteitr->second->getStep();
					totalstep = uncompleteitr->second->getStepCount();
					perhuoyuedu = uncompleteitr->second->getPoint();
					mytotalhuoyuedu += curstep * perhuoyuedu;
					totalhuoyuedu += totalstep * perhuoyuedu;
					//gamelist->addData(0, index, hydrecord[informationindex]);
					//gamelist->addData(1, index, crArgumentParser::appItoa(curstep) + "/" + crArgumentParser::appItoa(totalstep));
					//gamelist->addData(2, index, crArgumentParser::appItoa(curstep * perhuoyuedu) + "/" + crArgumentParser::appItoa(totalstep * perhuoyuedu));
					//gamelist->addData(3, index, "进行中");
					if (huoyuedulistinput[0][index].valid())huoyuedulistinput[0][index]->setString(hydrecord[informationindex]);
					if (huoyuedulistinput[1][index].valid())huoyuedulistinput[1][index]->setString(crArgumentParser::appItoa(curstep) + "/" + crArgumentParser::appItoa(totalstep));
					if (huoyuedulistinput[2][index].valid())huoyuedulistinput[2][index]->setString(crArgumentParser::appItoa(curstep * perhuoyuedu) + "/" + crArgumentParser::appItoa(totalstep * perhuoyuedu));
					if (huoyuedulistinput[3][index].valid())huoyuedulistinput[3][index]->setString(std::string("进行中"));
					index++;
				}
			}
		}
		for (completeitr = completevec.begin();
			completeitr != completevec.end() && index < 14;
			++completeitr)
		{
			if (HuoyueduTab->queryOneRecord(0, crArgumentParser::appItoa(completeitr->first), hydrecord) >= 0)
			{
				if (completeitr->second->getNeedlv() <= mylv)
				{
					curstep = completeitr->second->getStep();
					totalstep = completeitr->second->getStepCount();
					perhuoyuedu = completeitr->second->getPoint();
					mytotalhuoyuedu += curstep * perhuoyuedu;
					totalhuoyuedu += totalstep * perhuoyuedu;
					//gamelist->addData(0, index, hydrecord[informationindex]);
					//gamelist->addData(1, index, crArgumentParser::appItoa(curstep) + "/" + crArgumentParser::appItoa(totalstep));
					//gamelist->addData(2, index, crArgumentParser::appItoa(curstep * perhuoyuedu) + "/" + crArgumentParser::appItoa(totalstep * perhuoyuedu));
					//gamelist->addData(3, index, "已完成");
					for (int ii = 0; ii < 4; ++ii)
					{
						if (huoyuedulistinput[ii][index].valid())huoyuedulistinput[ii][index]->setColor(crVector4f(70.0f,140.0f,50.0f,255.0f) / 255.0f);
					}
					if (huoyuedulistinput[0][index].valid())huoyuedulistinput[0][index]->setString(hydrecord[informationindex]);
					if (huoyuedulistinput[1][index].valid())huoyuedulistinput[1][index]->setString(crArgumentParser::appItoa(totalstep) + "/" + crArgumentParser::appItoa(totalstep));
					if (huoyuedulistinput[2][index].valid())huoyuedulistinput[2][index]->setString(crArgumentParser::appItoa(totalstep * perhuoyuedu) + "/" + crArgumentParser::appItoa(totalstep * perhuoyuedu));
					if (huoyuedulistinput[3][index].valid())huoyuedulistinput[3][index]->setString(std::string("已完成"));
					index++;
				}
			}
		}
		//中间的总活跃度进度
		if (progress.valid())
		{
			progress->setProgress((float)mytotalhuoyuedu / (float)totalhuoyuedu);
		}
		if (progressinput.valid())
		{
			progressinput->setString(crArgumentParser::appItoa(mytotalhuoyuedu) + "/" + crArgumentParser::appItoa(totalhuoyuedu));
		}
		//右边的活跃度奖励领取
		CRCore::ScopedLock<crData> lock1(*canvasdata);
		canvasdata->getParam(WCHDATA_JXJHuoyueduRewardVec, param);
		HuoyueduRewardVec *huoyuedurewardvec = (HuoyueduRewardVec *)param;
		canvasdata->getParam(WCHDATA_JXJHuoyueduRewardItemVec, param);
		HuoyueduRewardItemVec *huoyuedurewarditemvec = (HuoyueduRewardItemVec *)param;
		canvasdata->getParam(WCHDATA_JXJHuoyueduRewardItemCountVec, param);
		HuoyueduRewardItemVec *huoyueduitemcountvec = (HuoyueduRewardItemVec *)param;
		huoyuedurewarditemvec->clear();
		huoyuedurewardvec->clear();
		huoyueduitemcountvec->clear();
		crVector3i itemid[3]; 
		int rewarditemindex[3], itemtypeindex[3],itemtype[3];
		HuoyueduRewardSet::iterator rsitr;// *rewardset = (HuoyueduRewardSet *)param;
		itemtypeindex[0] = HuoyueduRewardTab->getTitleIndex("类型1");
		itemtypeindex[1] = HuoyueduRewardTab->getTitleIndex("类型2");
		itemtypeindex[2] = HuoyueduRewardTab->getTitleIndex("类型3");
		rewarditemindex[0] = HuoyueduRewardTab->getTitleIndex("奖励1");
		rewarditemindex[1] = HuoyueduRewardTab->getTitleIndex("奖励2");
		rewarditemindex[2] = HuoyueduRewardTab->getTitleIndex("奖励3");
		int needlvindex = HuoyueduRewardTab->getTitleIndex("等级");
		int needhuoyueduindex = HuoyueduRewardTab->getTitleIndex("活跃值");
		unsigned short rewardid = 0;
		int rewardtype = 0;
		std::string itemiconname;
		std::string itemcountstr;
		int tipsid = 0;
		int tipscount = 0;

		data->getParam(WCHDATA_JXJPlayerCycleCount, param);
		short cycleCount = *(short *)param;
		if(mylv >= 50)
			lvcontrolsw->setActiveSwitchSet(1);
		for (int i = 0; i < HuoyueduRewardTab->getRowCount() && i < 6; ++i)
		{
			if (rewardinput[i].valid())
			{
				rewardinput[i]->setString("活跃度：" + (*HuoyueduRewardTab)(i, needhuoyueduindex));
			}
			rewardid = (unsigned short)atoi((*HuoyueduRewardTab)(i, 0).c_str());
			rsitr = rewardset->find(rewardid);
			huoyuedurewardvec->push_back(rewardid);
			if (rsitr != rewardset->end())
			{
				if (rewardbtnsw[i].valid())rewardbtnsw[i]->setActiveSwitchSet(1);
			}
			else
			{
				if (rewardbtn[i].valid() && mytotalhuoyuedu >= atoi((*HuoyueduRewardTab)(i, needhuoyueduindex).c_str()))
				{
					rewardbtn[i]->setEnable(true);
				}
			}
			for (int ii = 0; ii < 3; ++ii)
			{
				itemiconname.clear(); itemcountstr.clear(); tipsid = 0;
				itemtype[ii] = atoi((*HuoyueduRewardTab)(i, itemtypeindex[ii]).c_str());
				if (itemtype[ii] > 0)
				{
					crArgumentParser::appAtoVec((*HuoyueduRewardTab)(i, rewarditemindex[ii]), itemid[ii]);
					switch (itemtype[ii])
					{
					case T_Exp:
						itemiconname = "T_Icon_Task_EXP.img";
						itemcountstr.clear();// = crArgumentParser::appItoa(itemid[ii][0]);
						tipsid = T_Exp;
						tipscount = itemid[ii][0];
						break;
					case T_VIPExp:
						itemiconname = "T_Icon_Task_VIP.img";
						itemcountstr.clear();// = crArgumentParser::appItoa(itemid[ii][0] * (cycleCount + 1));
						tipsid = T_VIPExp;
						tipscount = itemid[ii][0]/* * (cycleCount + 1)*/;
						break;
					case T_Copper:
						itemiconname = "T_Icon_Task_Coins.img";
						itemcountstr.clear();// = crArgumentParser::appItoa(itemid[ii][0]);
						tipsid = T_Copper;
						tipscount = itemid[ii][0];
						break;
					case T_Exploit:
						itemiconname = "T_Icon_Task_Gongxun.img";
						itemcountstr.clear();// = crArgumentParser::appItoa(itemid[ii][0]);
						tipsid = T_Exploit;
						tipscount = itemid[ii][0];
						break;
					case T_Giftgold:
						itemiconname = "T_Icon_Task_Gifts.img";
						itemcountstr.clear();// = crArgumentParser::appItoa(itemid[ii][0]);
						tipsid = T_Giftgold;
						tipscount = itemid[ii][0];
						break;
					case T_Item:
						tipsid = itemid[ii][0];
						tipscount = itemid[ii][0];
						if (itemid[ii][0] > 0)
						{
							if (rewardicon[i * 3 + ii].valid())
							{
								if (itemTab->queryOneRecord(0, crArgumentParser::appItoa(itemid[ii][0]), itemrecord) >= 0)
								{
									itemiconname = itemrecord[iconnameindex];
								}
							}
							itemcountstr = crArgumentParser::appItoa(itemid[ii][2]);
						}
						break;
					}
					if (rewardicon[i * 3 + ii].valid() && !itemiconname.empty())
					{
						rewardicon[i * 3 + ii]->setImageName(itemiconname);
						rewardicon[i * 3 + ii]->setVisiable(true);
					}
					if (rewardiconbk[i * 3 + ii].valid())
					{
						rewardiconbk[i * 3 + ii]->setVisiable(true);
					}
					if (rewardcountinput[i * 3 + ii].valid())
					{
						rewardcountinput[i * 3 + ii]->setString(itemcountstr);
					}
				}
				huoyuedurewarditemvec->push_back(tipsid);
				huoyueduitemcountvec->push_back(tipscount);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJLingquFengluMethod
//
/////////////////////////////////////////
crJXJLingquFengluMethod::crJXJLingquFengluMethod() :
m_this(NULL)
{
}
crJXJLingquFengluMethod::crJXJLingquFengluMethod(const crJXJLingquFengluMethod& handle) :
crMethod(handle)
{
}
void crJXJLingquFengluMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJLingquFengluMethod::addParam(int i, const std::string& str)
{
}
void crJXJLingquFengluMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (myPlayer && netConductor)
	{
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJLingqufengluFlag, param);
		unsigned char flag = *(unsigned char *)param;
		if (flag == 0)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvLingquFenglu, NULL);
			netConductor->getNetManager()->sendPacket("all", packet);
		}
		else
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2108, NULL));//
	}
}
/////////////////////////////////////////
//
//crJXJRecvLingquFengluMethod
//
/////////////////////////////////////////
crJXJRecvLingquFengluMethod::crJXJRecvLingquFengluMethod() :
m_netType(GameClient_Game){}
crJXJRecvLingquFengluMethod::crJXJRecvLingquFengluMethod(const crJXJRecvLingquFengluMethod& handle) :
crMethod(handle)
{
}
void crJXJRecvLingquFengluMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvLingquFengluMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvLingquFengluMethod::operator()(crHandle &handle)
{
	if (m_this && m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			char success = 0;
			RewardItemVec rewardItems;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				crData *data = m_this->getDataClass();
				crData *mainroledata = m_this->getMainRole()->getDataClass();

				mainroledata->getParam(WCHDATA_JXJGovPost, param);
				unsigned char govpost = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJLingqufengluFlag, param);
				unsigned char flag = *(unsigned char *)param;
				if (flag == 0)
				{
					if (govpost > 0)
					{
						int fenglutype = 0;
						int itemid = 0;
						int count = 0;
						crTableIO::DataVec records;
						ref_ptr<crTableIO>GovpostRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovpostRewardTab);
						int fenglutypeindex = GovpostRewardTab->getTitleIndex("俸禄类型");
						int fengluidindex = GovpostRewardTab->getTitleIndex("俸禄id");
						int fenglucountindex = GovpostRewardTab->getTitleIndex("俸禄数量");
						data->getParam(WCHDATA_JXJTechFengluLv, param);
						unsigned char techfenglulv = *(unsigned char *)param;
						data->getParam(WCHDATA_JXJVipLv, param);
						unsigned char viplv = *(unsigned char *)param;
						int TechFengluPercent = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTechFengluPercent, viplv).c_str());
						if (fenglutypeindex > 0 && fengluidindex > 0 && fenglucountindex > 0)
						{
							GovpostRewardTab->queryRecords(0, crArgumentParser::appItoa(govpost), records);
							for (crTableIO::DataVec::iterator itr = records.begin(); itr != records.end(); ++itr)
							{
								fenglutype = atoi((*itr)[fenglutypeindex].c_str());
								itemid = atoi((*itr)[fengluidindex].c_str());
								count = atoi((*itr)[fenglucountindex].c_str());
								if (count > 0)
								{
									if (techfenglulv > 0)
									{
										count = count * (100 + TechFengluPercent) / 100;
									}
									success = 1;
									if (fenglutype == T_Item)
									{
										RewardItemVec rewardItems;
										ref_ptr<crBagItemData> newitem = new crBagItemData;
										newitem->setItemID(itemid);
										newitem->setItemCount(count);
										rewardItems.push_back(newitem);
										playerData->getPlayerGameData()->doEvent(WCH_JXJRecvRewardItems, MAKEINT64(&rewardItems, 0));
									}
									else
									{
										std::string str = "领取俸禄";
										MoneyChangeData  moneydata;//
										int datatype = 0;
										param = NULL;
										switch (fenglutype)
										{
										case T_Copper:
											m_this->doEvent(WCH_JXJRecvCoppercash, MAKEINT64(count, NULL));
											break;
										case T_Food:
											m_this->doEvent(WCH_JXJRecvFood, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Wood:
											m_this->doEvent(WCH_JXJRecvWood, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Iron:
											m_this->doEvent(WCH_JXJRecvIron, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Horse:
											m_this->doEvent(WCH_JXJRecvHorse, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Exp:
											m_this->getMainRole()->doEvent(WCH_JXJRecvExperience, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Achievement:
											m_this->getMainRole()->doEvent(WCH_JXJRecvAchievement, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Giftgold:
											moneydata.first = count;
											moneydata.second = str;
											m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
											datatype = 0;
											break;
										case T_Exploit:
											m_this->getMainRole()->doEvent(WCH_JXJRecvExploit, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										case T_Contribute:
											m_this->doEvent(WCH_JXJRecvCountryContribute, MAKEINT64(count, NULL));
											datatype = 0;
											break;
										default:
											break;
										}
									}
								}
							}
						}
					}
				}
				else
				{
					success = -1;//今天的俸禄已经领取过了
				}
				if (success == 1)
				{
					m_this->doEvent(WCH_JXJRecvGetChengChiEarnings,MAKEINT64(NULL,GameServer));
					unsigned char fengluflag = 1;
					data->inputParam(WCHDATA_JXJLingqufengluFlag, &fengluflag);
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(success);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvLingquFenglu, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				///游戏日志
				if (success == 1)
				{
					data->excHandle(MAKEINT64(WCH_LockData, 1));
					m_this->doEvent(WCH_JXJRecvRewardItems, MAKEINT64(&rewardItems, 0));
					m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_GovpostMoney, 1));
					data->excHandle(MAKEINT64(WCH_LockData, 0));
				}
				std::string logdata = "领取官职俸禄(官职，success)：" + crArgumentParser::appVectoa(crVector2i(govpost, success));
				GameLogData gamelog(Log_HuoyueduReward, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				unsigned char fengluflag = 1;
				data->inputParam(WCHDATA_JXJLingqufengluFlag, &fengluflag);
			}
			else
			{
				int tipsindex = 0;
				switch (success)
				{
				case -1:
					tipsindex = 2108;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}
////////////////////////////////
//
//crXiaoguanjiaBaodianUpdateMethod
//小管家宝典切页
///////////////////////////////////
crXiaoguanjiaBaodianUpdateMethod::crXiaoguanjiaBaodianUpdateMethod() :
m_this(NULL)
{}

crXiaoguanjiaBaodianUpdateMethod::crXiaoguanjiaBaodianUpdateMethod(const crXiaoguanjiaBaodianUpdateMethod& handle) :
crMethod(handle),
m_this(NULL),
m_gongnengkaifanginput(handle.m_gongnengkaifanginput),
m_banbenxinxiinput(handle.m_banbenxinxiinput)
{
}

void crXiaoguanjiaBaodianUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crXiaoguanjiaBaodianUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongnengkaifanginput = str;
		break;
	case 1:
		m_banbenxinxiinput = str;
		break;
	}
}

void crXiaoguanjiaBaodianUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && myPlayer)
	{
		ref_ptr<crStaticTextWidgetNode> gongnengkaifanginput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_gongnengkaifanginput));
		ref_ptr<crHypertextWidgetNode> banbenxinxiinput = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_banbenxinxiinput));
		if (gongnengkaifanginput.valid())
		{
			crData *mainroledata = myPlayer->getMainRole()->getDataClass();
			mainroledata->getParam(WCHDATA_Level, param);
			unsigned char lv = *(unsigned char *)param;
			unsigned char needlv = 0;
			ref_ptr<crTableIO>FuntionOpenInfoTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFuntionOpenInfoTab);
			int infoindex = FuntionOpenInfoTab->getTitleIndex("说明");
			std::string infostr = "                      敬 请 期 待";//文字居中
			for (int i = 0; i < FuntionOpenInfoTab->getRowCount(); ++i)
			{
				needlv = atoi((*FuntionOpenInfoTab)(i, 0).c_str());
				if (needlv <= lv)
				{
					continue;
				}
				else
				{
					infostr = (*FuntionOpenInfoTab)(i, infoindex);
					break;
				}
			}
			gongnengkaifanginput->setString(infostr);
		}
		if (banbenxinxiinput.valid())
		{
			std::string filename = crGlobalHandle::getInstance()->getGameDir() + "versioninfo.cfg";
			banbenxinxiinput->setHypertext(filename);
		}
		//canvas->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crCheckHuoyueduCompleteMethod
//小管家活跃度提示
///////////////////////////////////
crCheckHuoyueduCompleteMethod::crCheckHuoyueduCompleteMethod() :
m_this(NULL)
{}

crCheckHuoyueduCompleteMethod::crCheckHuoyueduCompleteMethod(const crCheckHuoyueduCompleteMethod& handle) :
crMethod(handle),
m_this(NULL),
m_huodongcanvas(handle.m_huodongcanvas),
m_countbackboard(handle.m_countbackboard),
m_countinput(handle.m_countinput),
m_xiaoguanjiabtn(handle.m_xiaoguanjiabtn)
{
}

void crCheckHuoyueduCompleteMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crCheckHuoyueduCompleteMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_huodongcanvas = str;
		break;
	case 1:
		m_countbackboard = str;
		break;
	case 2:
		m_countinput = str;
		break;
	case 3:
		m_xiaoguanjiabtn = str;
		break;
	}
}

void crCheckHuoyueduCompleteMethod::operator()(crHandle &handle)
{
	if (crGlobalHandle::isClient())
	{
		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode>huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
		if (huodongcanvas.valid())
		{
			ref_ptr<crButtonWidgetNode> xiaoguanjiabtn = dynamic_cast<crButtonWidgetNode *>(huodongcanvas->getWidget(m_xiaoguanjiabtn));
			ref_ptr<crStaticTextWidgetNode> countinput = dynamic_cast<crStaticTextWidgetNode *>(huodongcanvas->getWidget(m_countinput));
			ref_ptr<crNode> countbackboard = dynamic_cast<crNode *>(huodongcanvas->getChildNode(m_countbackboard));
			crData *data = myPlayer->getDataClass();
			crData *mainroledata = myPlayer->getMainRole()->getDataClass();
			if (countbackboard.valid())
			{
				countbackboard->setVisiable(false);
			}
			if (countinput.valid())
			{
				//countinput->setVisiable(false);
				countinput->clearString();
			}
			if (xiaoguanjiabtn.valid() && xiaoguanjiabtn->getVisiable())
			{
				int targetcount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJHuoYueDuTargetCount).c_str());
				
				mainroledata->getParam(WCHDATA_Level, param);
				unsigned char mylv = *(unsigned char *)param;
				ref_ptr<crTableIO>HuoyueduRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduRewardTab);
				int needlvindex = HuoyueduRewardTab->getTitleIndex("等级");
				int needhuoyueduindex = HuoyueduRewardTab->getTitleIndex("活跃值");
				int totalhuoyuedu = 0;
				int curstep = 0;
				int curhuoyuedu = 0;
				int cangetcount = 0;
				mainroledata->excHandle(MAKEINT64(WCH_LockData,1));
				mainroledata->getParam(WCHDATA_JXJHuoyueduMap, param);
				HuoyueduMap *huoyuedumap = (HuoyueduMap *)param;
				mainroledata->getParam(WCHDATA_JXJHuoyueduRewardSet, param);
				HuoyueduRewardSet *rewardset = (HuoyueduRewardSet *)param;
				for (HuoyueduMap::iterator itr = huoyuedumap->begin();
					itr != huoyuedumap->end();
					++itr)
				{
					curstep = itr->second->getStep();
					curhuoyuedu = itr->second->getPoint();
					totalhuoyuedu += curstep*curhuoyuedu;
				}
				if (totalhuoyuedu > 0)
				{
					for (int i = 0; i < HuoyueduRewardTab->getRowCount(); ++i)
					{
						if (atoi((*HuoyueduRewardTab)(i, needlvindex).c_str()))
						{
							if (totalhuoyuedu >= atoi((*HuoyueduRewardTab)(i, needhuoyueduindex).c_str())
								&& rewardset->find(atoi((*HuoyueduRewardTab)(i, 0).c_str())) == rewardset->end())
							{
								if (mylv >= 50 || i < 5)
								{
									cangetcount +=1;
								}
							}
						}
					}
				}
				mainroledata->excHandle(MAKEINT64(WCH_LockData,0));

				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char vipLv = *(unsigned char *)param;
				int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJHuoyueduOpenlv,vipLv).c_str());
				if (cangetcount > 0)
				{
					if (countbackboard.valid())
					{
						countbackboard->setVisiable(true);
					}
					if (countinput.valid())
					{
						//countinput->setVisiable(true);
						countinput->setString(crArgumentParser::appItoa(cangetcount));
					}
					if (mylv >= openlv)
					{//等级达到小管家开放要求时才启用奖励领取提示
						data->excHandle(MAKEINT64(WCH_LockData, 1));
						data->getParam(WCHDATA_JXJCouldGetAwardMap, param);
						CouldGetAwardMap * couldGetAwardMap = (CouldGetAwardMap *)param;
						unsigned short AwardType = CouldGetAward_SmallManager;
						bool couldGet = true;
						CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
						if (awItr != couldGetAwardMap->end())
						{
							if (!(awItr->second))
							{
								awItr->second = couldGet;
							}
						}
						else
						{
							couldGetAwardMap->insert(std::make_pair(AwardType, couldGet));
						}
						data->excHandle(MAKEINT64(WCH_LockData, 0));
					}
				}
				else if (mylv >= openlv)
				{
					data->excHandle(MAKEINT64(WCH_LockData, 1));
					data->getParam(WCHDATA_JXJCouldGetAwardMap, param);
					CouldGetAwardMap * couldGetAwardMap = (CouldGetAwardMap *)param;
					unsigned short AwardType = CouldGetAward_SmallManager;
					bool couldGet = true;
					CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
					if (awItr != couldGetAwardMap->end())
					{
						couldGetAwardMap->erase(awItr);
					}
					data->excHandle(MAKEINT64(WCH_LockData, 0));
				}
				if (data)
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJIsCheckHuoYueDuSign,param);
					bool *ischeck = (bool *)param;
					if (*ischeck && totalhuoyuedu >= targetcount)
					{
						ref_ptr<crTableIO> huoyuedufinish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJContinueHuoYueDuTab);
						//int tianshu = huoyuedufinish_table->getTitleIndex("days");
						int points = huoyuedufinish_table->getTitleIndex("points");
						std::vector<std::string> record;

						*ischeck = false;
						data->getParam(WCHDATA_JXJFinishHuoYueDuDayCount,param);
						unsigned char *continueday = (unsigned char *)param;
						*continueday += 1;

						int days = 0;
						if(*continueday > 7)
							days = 7;
						else
							days = *continueday;

						data->getParam(WCHDATA_JXJAchievePointsCount,param);
					    int *praisecount = (int *)param;

						if (huoyuedufinish_table->queryOneRecord(0,crArgumentParser::appItoa(days),record) >=0)
							*praisecount += atoi(record[points].c_str());

						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if(netConductor)
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(5);
							stream->_writeUChar(*continueday);
							stream->_writeInt(*praisecount);

							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJContinueFinishHuYueDu,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		//canvas->doEvent(WCH_UI_UpdateData);
	}
}
/////////////////////////////////////////
//
//crJXJGMMailDisposeMethod
//GM邮件
/////////////////////////////////////////
crJXJGMMailDisposeMethod::crJXJGMMailDisposeMethod() :
m_this(NULL),
m_count(50){}
crJXJGMMailDisposeMethod::crJXJGMMailDisposeMethod(const crJXJGMMailDisposeMethod& handle) :
crMethod(handle),
m_this(NULL),
m_count(handle.m_count)
{
}
void crJXJGMMailDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}
void crJXJGMMailDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	}
}
void crJXJGMMailDisposeMethod::operator()(crHandle &handle)
{//发送GM邮件
	//try
	//{
	bool success = false;
	void *param;
	int id = 0;
	int gmid = 0;
	unsigned char mailtype = 0;
	std::string playername;
	std::string title;
	std::string content;
	//std::vector<int> itemidvec;
	//std::vector<int> itemcountvec;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
	if (!gameServer)
	{
		return;
	}
	int playerid = 0;
	crData *gsBrainData = m_this->getDataClass();
	crVector4i items;
	crNetManager *netManager = gameServer->getNetManager();
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	//int serverid = callback->getServerID();
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crJXJGMMailQueryData> gmmail = new crJXJGMMailQueryData;
	int serverid = callback->getServerID();
	gmmail->buildQuerySql(m_count,serverid);
	globalSession->executeQuery(gmmail.get());
	crDataBase::QueryResultVec gmmailQueryResultVec = globalSession->getQueryResult();
	globalSession->releaseQuery();
	db->endSession(globalSession.get());
	ref_ptr<crJXJSystermMailInfo> systermMailInfo = NULL;
	int itemindex = 0;//
	if (!gmmailQueryResultVec.empty())
	{
		crVector3i item1,item2,item3,item4;
		gsBrainData->getParam(WCHDATA_JXJSystermMailInfoDeq, param);
		SystermMailInfoDeq *sysMailInfoDeq = (SystermMailInfoDeq *)param;
		crPlayerDataEventPacket packet;
		ref_ptr<crStreamBuf>stream;
		ref_ptr<crJXJRemoveGMMail> RemoveGMMail = new crJXJRemoveGMMail;
		ref_ptr<crJXJInsertGMMailRecord> InsertGMMailRecord = new crJXJInsertGMMailRecord;
		for (crDataBase::QueryResultVec::iterator itr = gmmailQueryResultVec.begin();
			itr != gmmailQueryResultVec.end();
			++itr)
		{
			items = crVector4i(0, 0, 0, 0);
			gmmail = dynamic_cast<crJXJGMMailQueryData *>(itr->get());
			id = gmmail->getID();
			mailtype = gmmail->getType();
			gmid = gmmail->getGMID();
			playername = gmmail->getPlayerName();
			title = gmmail->getTitle();
			content = gmmail->getContent();
			//itemidvec = gmmail->getItemIDVec();
			//itemcountvec = gmmail->getItemCountVec();
			crArgumentParser::appAtoVec(gmmail->getItem1(),item1);
			crArgumentParser::appAtoVec(gmmail->getItem2(),item2);
			crArgumentParser::appAtoVec(gmmail->getItem3(),item3);
			crArgumentParser::appAtoVec(gmmail->getItem4(),item4);
			//

			if (callback)
			{
				playerid = CRNetApp::crGlobalHandle::getInstance()->gameServerQueryPlayerID(playername,callback->getServerID());
			}
			//
			RemoveGMMail->buildUpdateSql(id);
			InsertGMMailRecord->buildUpdateSql(gmid,serverid,mailtype, playerid, playername, title, content, gmmail->getItem1(), gmmail->getItem2(),gmmail->getItem3(),gmmail->getItem4());
			globalSession = db->beginSession();
			if (globalSession->executeUpdate(InsertGMMailRecord.get()) && globalSession->executeUpdate(RemoveGMMail.get()))
			{
				globalSession->commit();
			}
			else
			{
				globalSession->rollback();
			}
			db->endSession(globalSession.get());
			//
			//for (itemindex = 0; itemindex < itemidvec.size() && itemindex < 4; ++itemindex)
			//{
			//	if (itemindex < itemcountvec.size() && itemcountvec[itemindex] > 0)
			//	{
			//		items[itemindex] = itemidvec[itemindex];
			//	}
			//}
			systermMailInfo = new crJXJSystermMailInfo;
			systermMailInfo->setSysMailTitle(title);
			systermMailInfo->setSysMailContent(content);
			systermMailInfo->setSysMailItems(item1,item2,item3,item4);
			systermMailInfo->setSysMailType(mailtype);
			systermMailInfo->setPlayerCharacterName(playername);

			if (mailtype == SYS_MAIL_TO_PLAYER)
			{
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermSendMailToPlayer, MAKEINT64(systermMailInfo.get(), NULL));
			}
			else
			{
				gsBrainData->excHandle(MAKEINT64(WCH_LockData, 1));
				sysMailInfoDeq->push_back(systermMailInfo.get());
				gsBrainData->excHandle(MAKEINT64(WCH_LockData, 0));

				m_this->doEvent(WCH_JXJSystermSendMailToPlayer, NULL);
			}
		}
	}

	//}
	//catch (...)
	//{
	//	int iii = 0;
	//}
}
/////////////////////////////////////////
//
//crJXJGMNoticeDisposeMethod
//GM公告
/////////////////////////////////////////
crJXJGMNoticeDisposeMethod::crJXJGMNoticeDisposeMethod() :
m_this(NULL){}
crJXJGMNoticeDisposeMethod::crJXJGMNoticeDisposeMethod(const crJXJGMNoticeDisposeMethod& handle) :
crMethod(handle),
m_this(NULL)
{
}
void crJXJGMNoticeDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}
void crJXJGMNoticeDisposeMethod::addParam(int i, const std::string& str)
{
}
void crJXJGMNoticeDisposeMethod::operator()(crHandle &handle)
{//发送GM公告
	bool success = false;
	void *param;
	int id = 0;
	int gmid = 0;
	std::string content;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
	if (!gameServer)
	{
		return;
	}
	//time_t t = time(0);
	//char tmp[20];
	//std::string opentime;
	//std::string endtime;
	//strftime(tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t));
	//crVector3i opentm, endtm, curtm;
	//crArgumentParser::appTimetoVec(tmp, curtm);
	crData *gsBrainData = m_this->getDataClass();
	crNetManager *netManager = gameServer->getNetManager();
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	int serverid = callback->getServerID();
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crJXJGMNoticeQueryData> gmnotice = new crJXJGMNoticeQueryData;
	gmnotice->buildQuerySql(serverid);
	globalSession->executeQuery(gmnotice.get());
	crDataBase::QueryResultVec gmnoticeQueryResultVec = globalSession->getQueryResult();
	globalSession->releaseQuery();
	db->endSession(globalSession.get());
	//ref_ptr<crJXJSystermMailInfo> systermMailInfo = NULL;
	//int opendt = 0;
	//int enddt = 0;
	if (!gmnoticeQueryResultVec.empty())
	{
		ref_ptr<crGameServerPlayerData> playerData;
		gsBrainData->getParam(WCHDATA_JXJSystemNoticeID, param);
		int systemnoticeid = *(int *)param;
		gmnotice = dynamic_cast<crJXJGMNoticeQueryData *>(gmnoticeQueryResultVec[0].get());
		id = gmnotice->getID();
		content = gmnotice->getContent();
		if (id != systemnoticeid)
		{
			crPlayerDataEventPacket replypacket;
			ref_ptr<crStreamBuf>stream;
			ref_ptr<crStreamBuf> noticestream = new crStreamBuf;
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content", 1);
			v_i.clear();
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", content);
			cfg_script.Pop();

			cfg_script.Pop();
			crGlobalHandle::getInstance()->getScriptStream(cfg_script, noticestream);
			gsBrainData->inputParam(WCHDATA_JXJCenterNotice, noticestream.get());
			gsBrainData->inputParam(WCHDATA_JXJSystemNoticeID, &id);
			//CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", content, 0, 0, 0);
			//gsBrainData->excHandle(MAKEINT64(WCH_LockData, 1));
			//crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate, MAKEINT64(msg.get(), NULL));
			//gsBrainData->excHandle(MAKEINT64(WCH_LockData, 0));
			//推送给全部玩家
			stream = new crStreamBuf;
			stream->createBuf(noticestream->getRemainSize());
			stream->write(noticestream->getPtr(), noticestream->getRemainSize());
			crPlayerDataEventPacket::buildReplyPacket(replypacket, 0, WCH_JXJRecvClientNotice, stream.get());
			stream->seekBegin();
			crStreamBuf *pkgstream = replypacket.getStreamBuf();
			int size = pkgstream->getBufSize();
			netDataManager->lockPlayerDataMap();//
			crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			for (crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end(); ++itr)
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (!playerData.valid())
				{
					continue;
				}
				pkgstream->seekBegin();
				pkgstream->_writeInt(playerData->getPlayerID());
				pkgstream->setBufSize(size);
				netManager->sendPacket(playerData->getPlayerConnectServerAddress(), replypacket);
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
}
/////////////////////////////////////////
//
//crJXJCanvasTableWidgetInitMethod
//
/////////////////////////////////////////
crJXJCanvasTableWidgetInitMethod::crJXJCanvasTableWidgetInitMethod() :
m_this(NULL)
{
}
crJXJCanvasTableWidgetInitMethod::crJXJCanvasTableWidgetInitMethod(const crJXJCanvasTableWidgetInitMethod& handle) :
crMethod(handle),
m_this(NULL),
m_tablename(handle.m_tablename),
m_listnamevec(handle.m_listnamevec)
{
}
void crJXJCanvasTableWidgetInitMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJCanvasTableWidgetInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tablename = str;
		break;
	default:
		m_listnamevec.push_back(str);
		break;
	}
}
void crJXJCanvasTableWidgetInitMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO>chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	if (m_this && chengchitab.valid())
	{
		ref_ptr<crTableWidgetNode> tablewidget = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_tablename));
		if (tablewidget.valid())
		{
			tablewidget->clearColumn();
			for (std::vector<std::string>::iterator itr = m_listnamevec.begin();
				itr != m_listnamevec.end();++itr)
			{
				tablewidget->addColumn(*itr);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIUpgradeResAddBuildingMethod
//
/////////////////////////////////////////
crJXJUIUpgradeResAddBuildingMethod::crJXJUIUpgradeResAddBuildingMethod() :
m_index(0),
m_font("simhei.ttf"),
m_characterSize(16),
m_goldColor(0.0f, 1.0f, 1.0f, 1.0f),
m_textColor(1.0f, 1.0f, 0.0f, 1.0f)
{
}

crJXJUIUpgradeResAddBuildingMethod::crJXJUIUpgradeResAddBuildingMethod(const crJXJUIUpgradeResAddBuildingMethod& handle) :
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

void crJXJUIUpgradeResAddBuildingMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIUpgradeResAddBuildingMethod::addParam(int i, const std::string& str)
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
		crArgumentParser::appAtoVec(str, m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str, m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}

void crJXJUIUpgradeResAddBuildingMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>resaddbuildingcanvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		crTableIO::StrVec ResAddBuildingrecord;
		ref_ptr<crTableIO> ResAddBuildingTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResAddBuildingTab);
		int needviplvindex = ResAddBuildingTab->getTitleIndex("Viplv");
		int buildinglvindex = ResAddBuildingTab->getTitleIndex("lv");
		int priceindex = ResAddBuildingTab->getTitleIndex("元宝");
		if (canvas.valid() && resaddbuildingcanvas.valid() && needviplvindex >= 0 && buildinglvindex >= 0 && priceindex >= 0)
		{
			crData *data = playerGameData->getDataClass();
			crData *mcanvasdata = resaddbuildingcanvas->getDataClass();
			mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
			int buildingid = *(int *)param;
			data->getParam(buildingid, param);
			unsigned char buildinglv = *(unsigned char *)param;
			if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv + 1), ResAddBuildingrecord) >= 0)
			{
				unsigned char needviplv = atoi(ResAddBuildingrecord[needviplvindex].c_str());
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				if (viplv >= needviplv)
				{
					int price = atoi(ResAddBuildingrecord[priceindex].c_str());
					int tempprice = price;
					playerGameData->doEvent(WCH_JXJCheckLijinGold, MAKEINT64(&tempprice, NULL));
					if (tempprice == 0)
					{
						//data->getParam(WCHDATA_JXJGoldCostHintInt, param);
						//unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_UpgradeResAddBuilding;
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
						//if (ifhint)
						{
							//int hinttype = JXJGoldCostType_UpgradeResAddBuilding;// -1;// JXJGoldCostType_MarketExchangeRes;
							//crData *braindata = crBrain::getInstance()->getDataClass();
							//braindata->inputParam(WCHDATA_JXJCurGoldHintType, &hinttype);
							std::vector<float>v_i;
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							//cfg_script.Add("Font",m_font);
							//cfg_script.Add("CharacterSize",m_characterSize);

							cfg_script.Add("Content");
							cfg_script.Push("Content", 1);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "您确认要消耗");
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
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", "升级该资源加成建筑吗？");
							cfg_script.Pop();

							cfg_script.Pop();

							if (information.valid())information->setHypertext(cfg_script);
						}
						if (okbtn.valid())okbtn->setEnable(true);
						crData *dlgData = canvas->getDataClass();
						int commandtype = CDP_Widget;
						dlgData->inputParam(WCHDATA_CommandDlgParentType, &commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent, m_this);
						//if (ifhint)
						{
							crFilterRenderManager::getInstance()->doModal(canvas.get());
						}
						//else
						//{
						//	if (okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
						//}
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2036, NULL));//元宝不足
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2111, NULL));//VIP不足
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2110, NULL));//建筑已升至最高级
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUpgradeResAddBuildingMethod
//
/////////////////////////////////////////
crJXJUpgradeResAddBuildingMethod::crJXJUpgradeResAddBuildingMethod() :
m_this(NULL)
{
}
crJXJUpgradeResAddBuildingMethod::crJXJUpgradeResAddBuildingMethod(const crJXJUpgradeResAddBuildingMethod& handle) :
crMethod(handle)
{
}
void crJXJUpgradeResAddBuildingMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUpgradeResAddBuildingMethod::addParam(int i, const std::string& str)
{
}
void crJXJUpgradeResAddBuildingMethod::operator()(crHandle &handle)
{
	void *param;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
		int buildingid = *(int *)param;
		if (buildingid > 0)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(buildingid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvUpgradeResAddBuilding, stream.get());
			netConductor->getNetManager()->sendPacket("all", packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUpgradeResAddBuildingMethod
//
/////////////////////////////////////////
crJXJRecvUpgradeResAddBuildingMethod::crJXJRecvUpgradeResAddBuildingMethod() :
m_netType(GameClient_Game){}
crJXJRecvUpgradeResAddBuildingMethod::crJXJRecvUpgradeResAddBuildingMethod(const crJXJRecvUpgradeResAddBuildingMethod& handle)
{
}
void crJXJRecvUpgradeResAddBuildingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvUpgradeResAddBuildingMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUpgradeResAddBuildingMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				int buildingid = m_stream->_readInt();
				char success = 0;
				void *param;
				bool needdeductgold = false;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				data->getParam(buildingid, param);
				unsigned char buildinglv = *(unsigned char *)param;
				unsigned char nowbuildinglv = buildinglv + 1;
				crTableIO::StrVec ResAddBuildingrecord;
				ref_ptr<crTableIO> ResAddBuildingTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResAddBuildingTab);
				int needviplvindex = ResAddBuildingTab->getTitleIndex("Viplv");
				int buildinglvindex = ResAddBuildingTab->getTitleIndex("lv");
				int priceindex = ResAddBuildingTab->getTitleIndex("元宝");
				if (needviplvindex >= 0 && buildinglvindex >= 0 && priceindex >= 0)
				{
					do
					{
						if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(nowbuildinglv), ResAddBuildingrecord) < 0)
						{
							success = -1;//建筑已到最高级
							break;
						}
						unsigned char needviplv = atoi(ResAddBuildingrecord[needviplvindex].c_str());
						int price = atoi(ResAddBuildingrecord[priceindex].c_str());
						if (viplv < needviplv)
						{
							success = -2;//Vip等级不足
							break;
						}
						MoneyChangeData moneydata(price, "升级资源建筑");
						m_this->doEvent(WCH_JXJDeductLijinGold, MAKEINT64(&moneydata, &needdeductgold));
						if (moneydata.first == 0)
						{
							success = 1;
						}
						else
						{
							success = -3;//元宝不足
							break;
						}
					} while (0);
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					data->inputParam(buildingid, &nowbuildinglv);
					stream->createBuf(6);
					stream->_writeChar(success);
					stream->_writeInt(buildingid);
					stream->_writeUChar(nowbuildinglv);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
					nowbuildinglv = buildinglv;
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvUpgradeResAddBuilding, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				if (needdeductgold)
				{//扣除了元宝，立即存储
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				if (success > 0)
				{///游戏日志
					std::string logdata = "升级资源加成建筑。建筑ID：" + crArgumentParser::appItoa(buildingid) + "当前等级" + crArgumentParser::appItoa(nowbuildinglv);
					GameLogData gamelog(Log_UpgradeResAddBuilding, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			//void *param;
			char success = m_stream->_readChar();
			int tipsindex = 0;

			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				int buildingid = m_stream->_readInt();
				unsigned char buildinglv = m_stream->_readUChar();
				data->inputParam(buildingid, &buildinglv);
			}
			else
			{
				switch (success)
				{
				case -1://建筑已到最高级
					tipsindex = 2110;
					break;
				case -2://Vip等级不足
					tipsindex = 2111;
					break;
				case -3://元宝不足
					tipsindex = 2036;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJResAddBuildingCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJResAddBuildingCanvasUpdateMethod::crJXJResAddBuildingCanvasUpdateMethod() :
m_this(NULL)
{
}
crJXJResAddBuildingCanvasUpdateMethod::crJXJResAddBuildingCanvasUpdateMethod(const crJXJResAddBuildingCanvasUpdateMethod& handle) :
crMethod(handle)
{
	m_canvasname = handle.m_canvasname;
	for (int i = 0; i < 9; ++i)
	{
		if (i < 3)
		{
			m_btnname[i] = handle.m_btnname[i];
		}
		m_inputname[i] = handle.m_inputname[i];
	}
}
void crJXJResAddBuildingCanvasUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	}
}
void crJXJResAddBuildingCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasname = str;
		break;
	case 1:
		m_levelname = str;
		break;
	case 2:
		m_buildingnamesw = str;
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
		m_inputname[i - 3] = str;
		break;
	case 12:
	case 13:
	case 14:
		m_btnname[i - 12] = str;
		break;
	default:
		break;
	}
}
void crJXJResAddBuildingCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
	if (playerGameData && canvas.valid() && canvas->getDataClass())
	{
		int buildingid = 0;
		crData *canvasdata = canvas->getDataClass();
		if (m_this && m_this->getDataClass())
		{
			crData *thisData = m_this->getDataClass();
			thisData->getParam(WCHDATA_JXJBuildingID, param);
			buildingid = *(int *)param;
			canvasdata->inputParam(WCHDATA_JXJCheckBuildingID, &buildingid);
			canvasdata->inputParam(WCHDATA_JXJCanvasBuildingItem, m_this);
		}
		else
		{
			canvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
			buildingid = *(int *)param;
		}
		if (buildingid > 0)
		{
			crData *data = playerGameData->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(buildingid, param);
			if (param)
			{
				unsigned char buildinglv = *(unsigned char *)param;
				ref_ptr<crStaticTextWidgetNode> levelinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_levelname));
				ref_ptr<crMultiSwitch> buildingnamesw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_buildingnamesw));
				ref_ptr<crStaticTextWidgetNode> input[9];
				ref_ptr<crButtonWidgetNode> btn[3];
				for (int i = 0; i < 9; ++i)
				{
					input[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_inputname[i]));
					if (i < 3)
					{
						btn[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_btnname[i]));
					}
				}
				std::string chanliangstr;
				if (buildingnamesw.valid())
				{
					switch (buildingid)
					{
					case WCHDATA_JXJShuichelv:
						buildingnamesw->setActiveSwitchSet(0);
						chanliangstr = "%农田基础产量";
						break;
					case WCHDATA_JXJRonglulv:
						buildingnamesw->setActiveSwitchSet(1);
						chanliangstr = "%铁矿基础产量";
						break;
					case WCHDATA_JXJGongshelv:
						buildingnamesw->setActiveSwitchSet(2);
						chanliangstr = "%木材基础产量";
						break;
					case WCHDATA_JXJMochuanlv:
						buildingnamesw->setActiveSwitchSet(3);
						chanliangstr = "%马匹基础产量";
						break;
					default:
						break;
					}
				}
				std::string nowaddstr,nextaddstr;
				crTableIO::StrVec nowrecord, nextrecord;
				ref_ptr<crTableIO> ResAddBuildingTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResAddBuildingTab);
				int needviplvindex = ResAddBuildingTab->getTitleIndex("Viplv");
				int buildinglvindex = ResAddBuildingTab->getTitleIndex("lv");
				int priceindex = ResAddBuildingTab->getTitleIndex("元宝");
				int addpercentindex = ResAddBuildingTab->getTitleIndex("加成");

				crTableIO::StrVec ResAddBuildingrecord;
				if(ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv + 1), ResAddBuildingrecord)>=0)
				{
					unsigned char needviplv = atoi(ResAddBuildingrecord[needviplvindex].c_str());
					if(needviplv > viplv) 
					{
						btn[0]->setEnable(false);
					}
					else
					{
						btn[0]->setEnable(true);
					}
				}

				if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv), nowrecord) >= 0)
				{
					nowaddstr = "提升" + nowrecord[addpercentindex] + chanliangstr;
				}
				else
				{
					nowaddstr = "无";
				}
				if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv + 1), nextrecord) >= 0)
				{
					nextaddstr = "提升" + nextrecord[addpercentindex] + chanliangstr;
				}
				else
				{
					nextaddstr = "已升至最高级";
				}
				if (levelinput.valid())
				{
					levelinput->setString(crArgumentParser::appItoa(buildinglv));
				}
				if (input[0].valid())
				{
					input[0]->setString(std::string("加成效果"));
				}
				if (input[1].valid())
				{
					input[1]->setString(std::string("下一等级"));
				}
				if (input[2].valid())
				{
					input[2]->setString(nowaddstr);
				}
				if (input[3].valid())
				{
					input[3]->setString(nextaddstr);
				}
				if (input[4].valid())
				{
					ref_ptr<crCanvasNode> jueSeCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_JueSe");
					if (jueSeCanvas.valid())
					{
						crData *Canvasdata = jueSeCanvas->getDataClass();
						int resourcepro = 0, resourceadd = 0;
						switch (buildingid)
						{
						case WCHDATA_JXJShuichelv:
							Canvasdata->getParam(WCHDATA_JXJProFoodProduct, param);
							resourcepro = *(int *)param;
							Canvasdata->getParam(WCHDATA_JXJProFoodProductAdd, param);
							resourceadd = *(int *)param;
							break;
						case WCHDATA_JXJRonglulv:
							Canvasdata->getParam(WCHDATA_JXJProIronProduct, param);
							resourcepro = *(int *)param;
							Canvasdata->getParam(WCHDATA_JXJProIronProductAdd, param);
							resourceadd = *(int *)param;
							break;
						case WCHDATA_JXJGongshelv:
							Canvasdata->getParam(WCHDATA_JXJProWoodProduct, param);
							resourcepro = *(int *)param;
							Canvasdata->getParam(WCHDATA_JXJProWoodProductAdd, param);
							resourceadd = *(int *)param;
							break;
						case WCHDATA_JXJMochuanlv:
							Canvasdata->getParam(WCHDATA_JXJProHorseProduct, param);
							resourcepro = *(int *)param;
							Canvasdata->getParam(WCHDATA_JXJProHorseProductAdd, param);
							resourceadd = *(int *)param;
							break;
						default:
							break;
						}
						if (resourceadd >= 0)
						{
							input[4]->setString(crArgumentParser::appItoa(resourcepro) + "(+" + crArgumentParser::appItoa(resourceadd) + ")");
						}
						else
						{
							input[4]->setString(crArgumentParser::appItoa(resourcepro) + "(" + crArgumentParser::appItoa(resourceadd) + ")");
						}
						input[4]->setColor(crVector4f(0.0f, 128.0f, 32.0f, 255.0f));
					}
				}
				if (input[5].valid())
				{
					int time = 0;
					int thingtype = 0;
					switch (buildingid)
					{
					case WCHDATA_JXJShuichelv:
						thingtype = T_Food;
						break;
					case WCHDATA_JXJRonglulv:
						thingtype = T_Iron;
						break;
					case WCHDATA_JXJGongshelv:
						thingtype = T_Wood;
						break;
					case WCHDATA_JXJMochuanlv:
						thingtype = T_Horse;
						break;
					default:
						break;
					}
					data->getParam(WCHDATA_JXJExtraProduceMap, param);
					ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
					ExtraProduceMap::iterator exitr = extraProduceMap->find(thingtype);
					if (exitr != extraProduceMap->end())
					{
						//percent += (float)itr->second->getPercent();
						time += exitr->second->timer();
					}
					if (time > 0)
					{
						int hour = time / 3600;
						int t = time % 3600;
						int minute = t / 60;
						int s = t % 60;
						if (hour > 0)
						{
							input[5]->setString(crArgumentParser::appItoa(hour) + "时" + crArgumentParser::appItoa(minute) + "分");
						}
						else if (minute > 0)
						{
							input[5]->setString(crArgumentParser::appItoa(minute) + "分");
						}
						else
						{
							input[5]->setString(std::string("小于1分"));
						}
					}
					else
					{
						input[5]->setString(std::string("无"));
					}
				}
				if (input[6].valid())
				{
					input[6]->setString(std::string("使用背包已有加成卡\n由低品质到高品质"));
				}
				if (input[7].valid())
				{
					int buyhours = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOnceHours, viplv).c_str()));
					input[7]->setString(crArgumentParser::appItoa(buyhours) + "小时");
				}
				if (input[8].valid())
				{
					int hoursprice = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOncePrice, viplv).c_str()));
					input[8]->setString("消耗" + crArgumentParser::appItoa(hoursprice) + "礼金/元宝");
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuyResAddBuildingTimeMethod
//
/////////////////////////////////////////
crJXJBuyResAddBuildingTimeMethod::crJXJBuyResAddBuildingTimeMethod() :
m_this(NULL)
{
}
crJXJBuyResAddBuildingTimeMethod::crJXJBuyResAddBuildingTimeMethod(const crJXJBuyResAddBuildingTimeMethod& handle) :
crMethod(handle)
{
}
void crJXJBuyResAddBuildingTimeMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyResAddBuildingTimeMethod::addParam(int i, const std::string& str)
{
}
void crJXJBuyResAddBuildingTimeMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		char success = 0;
		int tipsid = 0;
		crData *data = playerGameData->getDataClass();
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
		int buildingid = *(int *)param;
		unsigned char thingtype = 0;
		if (buildingid > 0)
		{
			switch (buildingid)
			{
			case WCHDATA_JXJShuichelv:
				thingtype = T_Food;
				break;
			case WCHDATA_JXJRonglulv:
				thingtype = T_Iron;
				break;
			case WCHDATA_JXJGongshelv:
				thingtype = T_Wood;
				break;
			case WCHDATA_JXJMochuanlv:
				thingtype = T_Horse;
				break;
			default:
				break;
			}
			data->getParam(WCHDATA_JXJVipLv, param);
			unsigned char viplv = *(unsigned char *)param;
			int buytime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOnceHours, viplv).c_str())) * 3600;
			int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOncePrice, viplv).c_str()));
			int maxtime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddTimeMax, viplv).c_str()));
			//MoneyChangeData moneydata(price, "资源建筑购买加成时间");
			playerGameData->doEvent(WCH_JXJCheckLijinGold, MAKEINT64(&price, NULL));
			if (price == 0)
			{
				data->getParam(WCHDATA_JXJExtraProduceMap, param);
				ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
				ExtraProduceMap::iterator itr = extraProduceMap->find(thingtype);
				if (itr != extraProduceMap->end())
				{
					if ((itr->second->timer() + buytime) <= maxtime)
					{
						//itr->second->timer() += buytime;
						success = 1;
					}
					else
					{
						tipsid = 2112;//时间超过上限
					}
				}
				else
				{
					success = 1;
				}
			}
			else
			{
				tipsid = 2036;//元宝不足
			}
			if (success == 1)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(thingtype);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvBuyResAddBuildingTime, stream.get());
				netConductor->getNetManager()->sendPacket("all", packet);
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsid, NULL));//
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuyResAddBuildingTimeMethod
//
/////////////////////////////////////////
crJXJRecvBuyResAddBuildingTimeMethod::crJXJRecvBuyResAddBuildingTimeMethod() :
m_netType(GameClient_Game){}
crJXJRecvBuyResAddBuildingTimeMethod::crJXJRecvBuyResAddBuildingTimeMethod(const crJXJRecvBuyResAddBuildingTimeMethod& handle)
{
}
void crJXJRecvBuyResAddBuildingTimeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvBuyResAddBuildingTimeMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuyResAddBuildingTimeMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				unsigned char thingtype = m_stream->_readUChar();
				char success = 0;
				void *param;
				bool needdeductgold = false;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				int buytime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOnceHours, viplv).c_str())) * 3600;
				int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOncePrice, viplv).c_str()));
				int maxtime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddTimeMax, viplv).c_str()));
				do
				{
					MoneyChangeData moneydata(price, "资源建筑购买加成时间");
					m_this->doEvent(WCH_JXJDeductLijinGold, MAKEINT64(&moneydata, &needdeductgold));
					if (moneydata.first == 0)
					{
						data->getParam(WCHDATA_JXJExtraProduceMap, param);
						ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
						ExtraProduceMap::iterator itr = extraProduceMap->find(thingtype);
						if (itr != extraProduceMap->end())
						{
							if ((itr->second->timer() + buytime) <= maxtime)
							{
								itr->second->timer() += buytime;
								success = 1;
								break;
							}
							else
							{
								success = -1;//时间超出上限
								break;
							}
						}
						else
						{
							ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
							extraProduce->timer() = buytime;
							extraProduce->setPercent(0);
							extraProduceMap->insert(std::make_pair(thingtype, extraProduce));
							success = 1;
							break;
						}
					}
					else
					{
						success = -2;//元宝不足
						break;
					}
				} while (0);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(6);
					stream->_writeChar(success);
					stream->_writeUChar(thingtype);
					stream->_writeInt(buytime);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvBuyResAddBuildingTime, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				if (needdeductgold)
				{//扣除了元宝，立即存储
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				if (success > 0)
				{///游戏日志
					std::string logdata = "资源建筑购买加成时间。" ;
					GameLogData gamelog(Log_BuyResAddTime, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			int tipsindex = 0;

			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				unsigned char thingtype = m_stream->_readUChar();
				int buytime = m_stream->_readInt();
				data->getParam(WCHDATA_JXJExtraProduceMap, param);
				ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
				ExtraProduceMap::iterator itr = extraProduceMap->find(thingtype);
				if (itr != extraProduceMap->end())
				{
					itr->second->timer() += buytime;
				}
				else
				{
					ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
					extraProduce->timer() = buytime;
					extraProduce->setPercent(0);
					extraProduceMap->insert(std::make_pair(thingtype, extraProduce));
				}
			}
			else
			{
				switch (success)
				{
				case -1://加成时间超过上线
					tipsindex = 2112;
					break;
				case -2://元宝不足
					tipsindex = 2036;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJOneKeyResAddCardMethod
//
/////////////////////////////////////////
crJXJOneKeyResAddCardMethod::crJXJOneKeyResAddCardMethod() :
m_this(NULL)
{
}
crJXJOneKeyResAddCardMethod::crJXJOneKeyResAddCardMethod(const crJXJOneKeyResAddCardMethod& handle) :
crMethod(handle)
{
}
void crJXJOneKeyResAddCardMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJOneKeyResAddCardMethod::addParam(int i, const std::string& str)
{
}
void crJXJOneKeyResAddCardMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		char success = 0;
		int tipsid = 0;
		crData *data = playerGameData->getDataClass();
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
		int buildingid = *(int *)param;
		unsigned char thingtype = 0;
		if (buildingid > 0)
		{
			switch (buildingid)
			{
			case WCHDATA_JXJShuichelv:
				thingtype = T_Food;
				break;
			case WCHDATA_JXJRonglulv:
				thingtype = T_Iron;
				break;
			case WCHDATA_JXJGongshelv:
				thingtype = T_Wood;
				break;
			case WCHDATA_JXJMochuanlv:
				thingtype = T_Horse;
				break;
			default:
				break;
			}
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(thingtype);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvOneKeyResAddCard, stream.get());
			netConductor->getNetManager()->sendPacket("all", packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvOneKeyResAddCardMethod
//一键使用加成卡
/////////////////////////////////////////
crJXJRecvOneKeyResAddCardMethod::crJXJRecvOneKeyResAddCardMethod() :
m_netType(GameClient_Game){}
crJXJRecvOneKeyResAddCardMethod::crJXJRecvOneKeyResAddCardMethod(const crJXJRecvOneKeyResAddCardMethod& handle)
{
}
void crJXJRecvOneKeyResAddCardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvOneKeyResAddCardMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvOneKeyResAddCardMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				unsigned char thingtype = m_stream->_readUChar();
				char success = 0;
				void *param;
				bool needdeductgold = false;
				int totaltime = 0;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				ref_ptr<crTableIO>ProduceCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJProduceCardTab);
				int timeid = ProduceCardTab->getTitleIndex("时间");
				int typeindex = ProduceCardTab->getTitleIndex("类型");
				crTableIO::StrVec record,precord,addrecord;
				crTableIO::DataVec precords;
				int colorindex = itemtab->getTitleIndex("品质");
				std::vector <std::pair<int, int>> pcarditemvec;//背包里总共有的加成卡
				int needdeductcount = 0;//使用掉的加成卡（需扣除的加成卡）
				ProduceCardTab->queryRecords(typeindex, crArgumentParser::appItoa(thingtype), precords);
				for (crTableIO::DataVec::iterator pitr = precords.begin();
					pitr != precords.end();
					++pitr)
				{
					pcarditemvec.push_back(std::make_pair(atoi((*pitr)[0].c_str()), 0));
				}
				//检测背包里现有的加成卡，按品质从低到高排序
				data->getParam(WCHDATA_JXJItemBagVec, param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				int itemid = 0;
				int itemcount = 0;
				for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
					itemitr != itembagvec->end();
					++itemitr)
				{
					if (itemitr->get())
					{
						itemid = itemitr->get()->getItemID();
						itemcount = (int)itemitr->get()->getItemCount();
						if (ProduceCardTab->queryOneRecord(0, crArgumentParser::appItoa(itemid), typeindex, crArgumentParser::appItoa(thingtype), precord) >= 0)
						{
							if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid),record) >= 0)
							{
								pcarditemvec[atoi(record[colorindex].c_str())].second += itemcount;
							}
						}
					}
				}
				//使用这些加成卡直到超过加成时间上限
				DeductItemMap deductitemmap;
				ref_ptr<crBagItemData> deductcard;
				int maxtime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddTimeMax, viplv).c_str()));
				data->getParam(WCHDATA_JXJExtraProduceMap, param);
				ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
				ExtraProduceMap::iterator itr;
				itr = extraProduceMap->find(thingtype);
				if (itr == extraProduceMap->end())
				{//未使用过该类型加成卡，添加该类型加成卡
					ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
					extraProduce->timer() = 0;
					extraProduce->setPercent(0);
					extraProduceMap->insert(std::make_pair(thingtype, extraProduce));
				}
				bool ifend = false;
				int addtime = 0;
				int j = 0;
				itr = extraProduceMap->find(thingtype);
				if (itr != extraProduceMap->end())
				{//已使用过该类型加成卡
					for (int i = 0; i < pcarditemvec.size(); ++i)
					{
						needdeductcount = 0;
						if (ProduceCardTab->queryOneRecord(0, crArgumentParser::appItoa(pcarditemvec[i].first), addrecord) >= 0)
						{
							for (j = 0; j < pcarditemvec[i].second; ++j)
							{
								if ((itr->second->timer() + atoi(addrecord[timeid].c_str()) <= maxtime))
								{
									itr->second->timer() += atoi(addrecord[timeid].c_str());
									needdeductcount += 1;
									totaltime = itr->second->timer();
									success = 1;
								}
								else
								{
									ifend = true;
									break;
								}
							}
						}
						deductcard = new crBagItemData;
						deductcard->setItemID(pcarditemvec[i].first);
						deductcard->setItemDeductCount(needdeductcount);
						deductitemmap.insert(std::make_pair(pcarditemvec[i].first, deductcard));
						if (ifend)
						{
							break;
						}
					}
				}
				//从背包中扣除这些加成卡
				m_this->doEvent(WCH_JXJRecvDeductItems, MAKEINT64(&deductitemmap, 0));

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(6);
					stream->_writeChar(success);
					stream->_writeUChar(thingtype);
					stream->_writeInt(totaltime);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvOneKeyResAddCard, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				if (success > 0)
				{///游戏日志
					std::string logdata = "一键使用加成卡";
					GameLogData gamelog(Log_OneKeyUseProduceCard, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			int tipsindex = 0;

			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				unsigned char thingtype = m_stream->_readUChar();
				int totaltime = m_stream->_readInt();
				data->getParam(WCHDATA_JXJExtraProduceMap, param);
				ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
				ExtraProduceMap::iterator itr = extraProduceMap->find(thingtype);
				if (itr != extraProduceMap->end())
				{
					itr->second->timer() = totaltime;
				}
				else
				{
					ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
					extraProduce->timer() = totaltime;
					extraProduce->setPercent(0);
					extraProduceMap->insert(std::make_pair(thingtype, extraProduce));
				}
			}
			else
			{
				//switch (success)
				//{
				//case -1://建筑已到最高级
				//	tipsindex = 2110;
				//	break;
				//case -2://Vip等级不足
				//	tipsindex = 2111;
				//	break;
				//case -3://元宝不足
				//	tipsindex = 2036;
				//	break;
				//default:
				//	break;
				//}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJUIBuyResAddBuildingTimeMethod
 
**************************************************************************/

JXJ::crJXJBuyResAddBuildingTimeShowDialogMethod::crJXJBuyResAddBuildingTimeShowDialogMethod():
	m_this(NULL)
{

}

JXJ::crJXJBuyResAddBuildingTimeShowDialogMethod::crJXJBuyResAddBuildingTimeShowDialogMethod( const crJXJBuyResAddBuildingTimeShowDialogMethod& handle ):
	crMethod(handle),
	m_msgbox(handle.m_msgbox),
	m_msg(handle.m_msg),
	m_ok(handle.m_ok)
{

}

void JXJ::crJXJBuyResAddBuildingTimeShowDialogMethod::operator()( crHandle &handle )
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		char success = 0;
		int tipsid = 0;
		crData *data = playerGameData->getDataClass();
		crData *mcanvasdata = m_this->getParentCanvas()->getDataClass();
		mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
		int buildingid = *(int *)param;
		unsigned char thingtype = 0;
		if (buildingid > 0)
		{
			switch (buildingid)
			{
			case WCHDATA_JXJShuichelv:
				thingtype = T_Food;
				break;
			case WCHDATA_JXJRonglulv:
				thingtype = T_Iron;
				break;
			case WCHDATA_JXJGongshelv:
				thingtype = T_Wood;
				break;
			case WCHDATA_JXJMochuanlv:
				thingtype = T_Horse;
				break;
			default:
				break;
			}
			data->getParam(WCHDATA_JXJVipLv, param);
			unsigned char viplv = *(unsigned char *)param;
			int buytime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOnceHours, viplv).c_str())) * 3600;
			int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddOncePrice, viplv).c_str()));
			int maxtime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResAddTimeMax, viplv).c_str()));
			//MoneyChangeData moneydata(price, "资源建筑购买加成时间");
			int needPrice = price;
			playerGameData->doEvent(WCH_JXJCheckLijinGold, MAKEINT64(&price, NULL));
			if (price == 0)
			{
				data->getParam(WCHDATA_JXJExtraProduceMap, param);
				ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
				ExtraProduceMap::iterator itr = extraProduceMap->find(thingtype);
				if (itr != extraProduceMap->end())
				{
					if ((itr->second->timer() + buytime) <= maxtime)
					{
						//itr->second->timer() += buytime;
						success = 1;
					}
					else
					{
						tipsid = 2112;//时间超过上限
					}
				}
				else
				{
					success = 1;
				}
			}
			else
			{
				tipsid = 2036;//元宝不足
			}
			if (success == 1)
			{
				ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
				if (msgCanvas.valid())
				{
					int percent =0;
					data->getParam(buildingid, param);
					unsigned char buildinglv = *(unsigned char *)param;
					crTableIO::StrVec ResAddBuildingrecord;
					ref_ptr<crTableIO> ResAddBuildingTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResAddBuildingTab);
					int buildinglvindex = ResAddBuildingTab->getTitleIndex("lv");
					int addindex = ResAddBuildingTab->getTitleIndex("加成");
					if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv), ResAddBuildingrecord) >= 0)
						 percent = atoi(ResAddBuildingrecord[addindex].c_str());

					ref_ptr<crHypertextWidgetNode> msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msg));
					ref_ptr<crButtonWidgetNode> okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_ok));
					data->getParam(WCHDATA_JXJGoldCostHintInt,param);
					unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_BuyResAddTimeHint;
					if(okBtn.valid())okBtn->setEnable(true);
					if (ifhint)
					{
						int hinttype = JXJGoldCostType_BuyResAddTimeHint;
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
						v_i.push_back(m_copperColor[0]);
						v_i.push_back(m_copperColor[1]);
						v_i.push_back(m_copperColor[2]);
						v_i.push_back(m_copperColor[3]);
						cfg_script.Add("Color",v_i);
						char tmpText[160],tmpText2[80];
						sprintf(tmpText,"%d礼金/元宝\0",needPrice);
						cfg_script.Add("Text",tmpText);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						sprintf(tmpText2,"来购买【%d小时资源加成%d%%】吗？\0",buytime / 3600,percent);
						cfg_script.Add("Text",tmpText2);
						cfg_script.Pop();
						cfg_script.Pop();
						if(msg.valid())msg->setHypertext(cfg_script);
					}

					if (msgCanvas.valid())
					{
						crData *dlgData = msgCanvas->getDataClass();
						int commandtype = CDP_Widget;
						dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
						if (ifhint)
						{
							crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
						}
						else
						{
							if(okBtn.valid())okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
						}
					}
				}

				//ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//stream->createBuf(1);
				//stream->_writeUChar(thingtype);
				//crPlayerDataEventPacket packet;
				//crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvBuyResAddBuildingTime, stream.get());
				//netConductor->getNetManager()->sendPacket("all", packet);
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsid, NULL));//
			}
		}
	}
	

}

void JXJ::crJXJBuyResAddBuildingTimeShowDialogMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}

}

void JXJ::crJXJBuyResAddBuildingTimeShowDialogMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_msgbox = str;
		break;
	case 1:
		m_msg = str;
		break;
	case 2:
		m_ok = str;
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_copperColor);
		break;
	}
}
