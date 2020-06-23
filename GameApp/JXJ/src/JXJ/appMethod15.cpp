/**********************************************************************
*
*	文件:	   appMethod15.cpp
*
*	描述:	   
*
*	作者:	   邹俊
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod15.h>
#include <JXJ/appDataParam.h>
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
#include <CREncapsulation/crGameDices.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crLoadManager.h>
#include <CRCore/crBlockDetectThread.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRDownload/crDownload.h>
#include <sstream>
#include <JXJ/appDBQuery.h>
#include <shellapi.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace CRIOManager;
using namespace rbody;
using namespace JXJ;
using namespace CRUI;
using namespace CRDownload;
using namespace std;

/////////////////////////////////////////
//
//crJXJGetStartedDaysMethod
//获得开服天数
/////////////////////////////////////////
crJXJGetStartedDaysMethod::crJXJGetStartedDaysMethod():
	m_output(NULL){}
crJXJGetStartedDaysMethod::crJXJGetStartedDaysMethod(const crJXJGetStartedDaysMethod& handle):
	crMethod(handle),
	m_output(NULL)
{
}
void crJXJGetStartedDaysMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_output = (int *)(LOCREPARAM(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}
void crJXJGetStartedDaysMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetStartedDaysMethod::operator()(crHandle &handle)
{
	if(m_output)
	{
		std::string dateStr = crGlobalHandle::gData()->gStartDate();
		typedef std::vector<int> Vec5i;
		Vec5i startTimeVec5i;
		startTimeVec5i.reserve(5);
		crArgumentParser::appAtoVec(dateStr,startTimeVec5i);

		startTimeVec5i.resize(5,0);

		time_t nowTime_t = time(0);
		struct  tm  nowTm = *localtime(&nowTime_t); //服务器当前时间
		struct tm  startTm;
		memset(&startTm,0,sizeof (startTm));

		startTm.tm_year = startTimeVec5i[0] - 1900;
		startTm.tm_mon = startTimeVec5i[1] - 1;
		startTm.tm_mday = startTimeVec5i[2];
		startTm.tm_hour = startTimeVec5i[3];
		startTm.tm_min = startTimeVec5i[4];
		startTm.tm_isdst = nowTm.tm_isdst;

		time_t startTime_t = mktime(&startTm); //服务器开服时间
		*m_output = ceil((float)(nowTime_t - startTime_t) / 86400.0f); //开服天数
	}
}

////////////////////////////////
//
//crJXJCountryOfficialUpdateMethod
//国家官员选举结果
///////////////////////////////////
crJXJCountryOfficialUpdateMethod::crJXJCountryOfficialUpdateMethod() :
	m_this(NULL)
{}

crJXJCountryOfficialUpdateMethod::crJXJCountryOfficialUpdateMethod(const crJXJCountryOfficialUpdateMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_PrimeMinister(handle.m_PrimeMinister),
	m_GreatMinister(handle.m_GreatMinister),
	m_Daisyougun(handle.m_Daisyougun)
{
}

void crJXJCountryOfficialUpdateMethod::inputParam(int i, void *param)
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

void crJXJCountryOfficialUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_PrimeMinister = str;
		break;
	case 1:
		m_GreatMinister = str;
		break;
	case 2:
		m_Daisyougun = str;
		break;
	default:
		break;
	}
}

void crJXJCountryOfficialUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Toupiaojieguo");
		ref_ptr<crHypertextWidgetNode> chengXiang = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_PrimeMinister));
		ref_ptr<crHypertextWidgetNode> daSiMa = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_GreatMinister));
		ref_ptr<crHypertextWidgetNode> jiangJun = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_Daisyougun));
		ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
		crData *canvasdata = canvas_1->getDataClass();
		canvasdata->excHandle(MAKECREPARAM(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalVec,param);
		OfficalVec & officialVec = *(OfficalVec *)param;
		if (!officialVec.empty())
		{
			std::vector<float>v_i;
			if (!officialVec[0].second.empty())
			{
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				//丞相
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content", 1);
				v_i.clear();
				v_i.push_back(140.0f);
				v_i.push_back(40.0f);
				v_i.push_back(40.0f);
				v_i.push_back(255.0f);

				cfg_script.Add("Color", v_i);
				cfg_script.Add("Text", "   恭喜玩家" + officialVec[0].second + "成为本国丞相");
				cfg_script.Pop();

				cfg_script.Pop();
				if(chengXiang.valid()) chengXiang->setHypertext(cfg_script);
			}
			if (!officialVec[1].second.empty())
			{
				//大司马
				ItemMap itemMap_2;
				rcfg::ConfigScript cfg_script_2(&itemMap_2);
				cfg_script_2.Add("Hypertext");
				cfg_script_2.Push("Hypertext");

				cfg_script_2.Add("Content");
				cfg_script_2.Push("Content", 1);
				v_i.clear();
				v_i.push_back(140.0f);
				v_i.push_back(40.0f);
				v_i.push_back(40.0f);
				v_i.push_back(255.0f);

				cfg_script_2.Add("Color", v_i);
				cfg_script_2.Add("Text", "   恭喜玩家" + officialVec[1].second + "成为本国大司马");
				cfg_script_2.Pop();

				cfg_script_2.Pop();
				if(daSiMa.valid()) daSiMa->setHypertext(cfg_script_2);
			}
			if (!officialVec[2].second.empty())
			{
				//大将军
				ItemMap itemMap_3;
				rcfg::ConfigScript cfg_script_3(&itemMap_3);
				cfg_script_3.Add("Hypertext");
				cfg_script_3.Push("Hypertext");

				cfg_script_3.Add("Content");
				cfg_script_3.Push("Content", 1);
				v_i.clear();
				v_i.push_back(140.0f);
				v_i.push_back(40.0f);
				v_i.push_back(40.0f);
				v_i.push_back(255.0f);

				cfg_script_3.Add("Color", v_i);
				cfg_script_3.Add("Text", "   恭喜玩家" + officialVec[2].second + "成为本国大将军");
				cfg_script_3.Pop();

				cfg_script_3.Pop();
				if(jiangJun.valid()) jiangJun->setHypertext(cfg_script_3);
			}			
		}
		canvasdata->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
////////////////////////////////
//
//crJXJXuanJuTiShiUpdateMethod
//国家官员选举提示
///////////////////////////////////
crJXJXuanJuTiShiUpdateMethod::crJXJXuanJuTiShiUpdateMethod() :
	m_this(NULL)
{}

crJXJXuanJuTiShiUpdateMethod::crJXJXuanJuTiShiUpdateMethod(const crJXJXuanJuTiShiUpdateMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_Title(handle.m_Title),
	m_Rule(handle.m_Rule)
{
}

void crJXJXuanJuTiShiUpdateMethod::inputParam(int i, void *param)
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

void crJXJXuanJuTiShiUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Title = str;
		break;
	case 1:
		m_Rule = str;
		break;
	default:
		break;
	}
}

void crJXJXuanJuTiShiUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Guanyuanxuanju_Tips");
		ref_ptr<crStaticTextWidgetNode> Title = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Title));
		ref_ptr<crStaticTextWidgetNode> Rule = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Rule));
		if (Title.valid())
		{
			Title->setString(std::string("选举规则"));
		}
		if (Rule.valid())
		{
			int textid = 7003;
			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			std::string str;
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),record)>=0)
			{
				str = record[1];
			}
			Rule->setString(str);
		}
	}
}
/////////////////////////////////////////
//
//crJXJCountryOfficialPollsUpdateMethod
//国家官员选举票数公布
/////////////////////////////////////////
crJXJCountryOfficialPollsUpdateMethod::crJXJCountryOfficialPollsUpdateMethod():
	m_this(NULL)
{
}

crJXJCountryOfficialPollsUpdateMethod::crJXJCountryOfficialPollsUpdateMethod(const crJXJCountryOfficialPollsUpdateMethod& handle):
	crMethod(handle),
	m_Crops(handle.m_Crops),
	m_CandidateControl(handle.m_CandidateControl)
{
}

void crJXJCountryOfficialPollsUpdateMethod::inputParam(int i, void *param)
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

void crJXJCountryOfficialPollsUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Crops = str;
		break;
	case 1:
		m_CandidateControl = str;
		break;
	default:
		break;
	}
}

void crJXJCountryOfficialPollsUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Piaoshu");
	ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
	if(playerGameData && netConductor && canvas.valid())
	{
		void *param;
		crData *canvasdata = canvas_1->getDataClass();
		int playerid = playerGameData->getPlayerID();
		canvasdata->excHandle(MAKECREPARAM(WCH_LockData,1));
		ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_Crops));
		//显示上周国家官员候选人票数情况
		do 
		{
			canvasdata->getParam(WCHDATA_JXJCountryCandidateVecL,param);
			CandidateVec *candidateVecL = (CandidateVec *)param;
			if (!candidateVecL)
			{
				break;
			}
			CandidateVec::iterator cvItr = candidateVecL->begin();
			ref_ptr<crScrollBarWidgetNode> CandidateControl = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_CandidateControl));
			CandidateControl->setEnable(false);
			//CandidateControl->setRange(0, candidateVecL->size() - 10);
			//CandidateControl->setLineValue(1.0f);
			CandidateControl->setPageValue(50.0f);
			//int start = 0;
			if (candidateVecL->size() > 10)
			{
				CandidateControl->setEnable(true);
				//start = CandidateControl->getValue();
			}
			int end = candidateVecL->size();

			if(tableWidget.valid())
			{
				tableWidget->clearData();
				cvItr = candidateVecL->begin();
				//int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,0).c_str());

				int totalPolls = 0;
				int i = 0;
				for (cvItr = candidateVecL->begin(); cvItr != candidateVecL->end(); ++cvItr)
				{
					totalPolls += (*cvItr)->nNumber;
				}
				for (cvItr = candidateVecL->begin();cvItr != candidateVecL->end() && i < end; ++cvItr)
				{
					if (cvItr->valid() && (*cvItr)->nNumber > 0)
					{
						float percent = 0.0f;
						if (totalPolls != 0)
						{
							percent = (float)((*cvItr)->nNumber)/totalPolls;
						}
						char Tmp[40];
						sprintf(Tmp,"%.0f%%", percent * 100);
						tableWidget->addData(0,i,(*cvItr)->playerName);
						tableWidget->addData(1,i,Tmp);
						tableWidget->addData(i,(*cvItr)->nPlayerID);
						++ i;
					}
				}
			}
		} while (0);
		canvasdata->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

////////////////////////////////////////
//
//crJXJCountryOfficialElectUpdateMethod
//国家官员选举
/////////////////////////////////////////
crJXJCountryOfficialElectUpdateMethod::crJXJCountryOfficialElectUpdateMethod():
	m_this(NULL)
{
}

crJXJCountryOfficialElectUpdateMethod::crJXJCountryOfficialElectUpdateMethod(const crJXJCountryOfficialElectUpdateMethod& handle):
	crMethod(handle),
	m_Crops(handle.m_Crops),
	m_Recommend(handle.m_Recommend),
	m_CandidateControl(handle.m_CandidateControl),
	m_Recommend_sw(handle.m_Recommend_sw)
{
	for (unsigned char i = 0; i < 10; ++ i)
	{
		m_checkbox_sw[i] = handle.m_checkbox_sw[i];
		m_Select[i] = handle.m_Select[i];
	}
}

void crJXJCountryOfficialElectUpdateMethod::inputParam(int i, void *param)
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

void crJXJCountryOfficialElectUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Crops = str;
		break;
	case 1:
		m_Select[0] = str;
		break;
	case 2:
		m_Select[1] = str;
		break;
	case 3:
		m_Select[2] = str;
		break;
	case 4:
		m_Select[3] = str;
		break;
	case 5:
		m_Select[4] = str;
		break;
	case 6:
		m_Select[5] = str;
		break;
	case 7:
		m_Select[6] = str;
		break;
	case 8:
		m_Select[7] = str;
		break;
	case 9:
		m_Select[8] = str;
		break;
	case 10:
		m_Select[9] = str;
		break;
	case 11:
		m_Recommend = str;
		break;
	case 12:
		m_CandidateControl = str;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
		m_checkbox_sw[i - 13] = str;
		break;
	case 23:
		m_Recommend_sw = str;
		break;
	default:
		break;
	}
}

void crJXJCountryOfficialElectUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Guanyuanxuanju");
	ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
	if(playerGameData && netConductor && canvas.valid() && canvas_1.valid() && canvas_1->getDataClass())
	{
		crData *playerdata = playerGameData->getDataClass();
		if (playerdata)
		{
			int playerid = playerGameData->getPlayerID();
			void *param;
			bool elector = false;
			int start = 0;
			int valueChange = 0;
			int iCheckNumber = 0;
			playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
			playerdata->getParam(WCHDATA_JXJElector,param);
			elector = *(bool *)param;

			crMultiSwitch *foldbtn_sw[10] = {NULL};
			ref_ptr<crCheckBoxWidgetNode >itemCheckBox[10] = {NULL}; 
			ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_Crops));
			ref_ptr<crButtonWidgetNode> buildbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Recommend));
			ref_ptr<crScrollBarWidgetNode> CandidateControl = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_CandidateControl));

			crData *canvasdata = canvas_1->getDataClass();
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			CandidateVec *candidateVec = (CandidateVec *)param;
			CandidateVec::iterator cvItr = candidateVec->begin();
			if (CandidateControl.valid())
			{
				CandidateControl->setValue(0);
				CandidateControl->setEnable(false);
				if (candidateVec->size() > 10)
				{
					CandidateControl->setRange(0, candidateVec->size() - 10);
					CandidateControl->setLineValue(1.0f);
					CandidateControl->setPageValue(1.0f);
					CandidateControl->setVisiable(true);
					CandidateControl->setEnable(true);
					valueChange = CandidateControl->getValue();
				}
			}
			int end = valueChange + 10;

			for (int i = 0; i < 10; i++)
			{
				itemCheckBox[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_Select[i]));
				foldbtn_sw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_checkbox_sw[i]));
				if (itemCheckBox[i].valid())
				{
					if(itemCheckBox[i]->getSelect())
					{
						iCheckNumber++;
					}
					itemCheckBox[i]->setVisiable(false);
					itemCheckBox[i]->setEnable(false);
					//itemCheckBox[i]->select(true);
					//foldbtn_sw[i]->setActiveSwitchSet(1);
				}
			}

			canvasdata->getParam(WCHDATA_JXJSelectLeaderSet,param);
			SelectLeaderIdSet * selectLeaderSet = (SelectLeaderIdSet *)param;
			bool checkBoxsVisiable = false;
			bool checkBoxsEnable = false;
			bool voteBtnVisiable = false;
			bool voteBtnEanble = false;

			if(elector)
			{
				checkBoxsVisiable = true;
				if (selectLeaderSet->empty())
				{
					// 之前没有投过票
					checkBoxsEnable  = true;
					voteBtnVisiable = true;
					voteBtnEanble = true;
				}
			}

			if (buildbtn.valid())
			{
				buildbtn->setVisiable(voteBtnVisiable);
				buildbtn->setEnable(voteBtnEanble);
			}
			if (valueChange != start)
			{
				//滚动条值发生改变时，重置控件勾选状态
				playerdata->getParam(WCHDATA_JXJSelectedIdVec, param);
				SelectedIdVec *idVec = (SelectedIdVec *)param;
				unsigned char size = idVec->size();
				for (int i = 0; i < 10;)
				{
					ref_ptr<crCheckBoxWidgetNode> foldbtn[10] = {NULL};
					SelectedIdVec::iterator sivItr = idVec->begin();
					for (; sivItr != idVec->end(); ++ sivItr)
					{
						if (itemCheckBox[i].valid())
						{
							itemCheckBox[i]->setVisiable(checkBoxsVisiable);
							if (!idVec->empty())
							{
								if (*sivItr == tableWidget->getData(i + valueChange))
								{
									itemCheckBox[i]->setEnable(true);
									itemCheckBox[i]->select(true);
								}
								else if (size < 2)
								{
									itemCheckBox[i]->setEnable(true);
									itemCheckBox[i]->select(false);
								}
								else
								{
									itemCheckBox[i]->setEnable(false);
									itemCheckBox[i]->select(false);
								}
							}
							else
							{
								itemCheckBox[i]->setEnable(true);
								itemCheckBox[i]->select(false);
							}
						}
					}
					++ i;
				}
			}
			if(tableWidget.valid())
			{
				tableWidget->clearData();
				SelectLeaderIdSet::iterator slsItr;
				//int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,0).c_str());
				int i = 0;
				for (;cvItr != candidateVec->end() && i < candidateVec->size(); ++ cvItr)
				{
					if (cvItr->valid() && (*cvItr)->nNumber > 0)
					{//自荐过的玩家票数为1，筛选出候选人
						//if ((*cvItr)->m_rankVal > maxRankingNum)
						//{
						//	tableWidget->addData(0,i,crArgumentParser::appItoa(maxRankingNum) + "+");
						//}
						//else
						//{
						//	tableWidget->addData(0,i,crArgumentParser::appItoa((*cvItr)->m_rankVal));
						//}
						tableWidget->addData(0,i,(*cvItr)->playerName);
						tableWidget->addData(1,i,crArgumentParser::appItoa((*cvItr)->m_level));
						tableWidget->addData(2,i,(*cvItr)->juntuanName);
						tableWidget->addData(i,(*cvItr)->nPlayerID);

						if (itemCheckBox[i].valid() && i < 10)
						{
							itemCheckBox[i]->setVisiable(checkBoxsVisiable);
							if (!selectLeaderSet->empty())
							{
								slsItr = selectLeaderSet->find((*cvItr)->nPlayerID);
								if (slsItr != selectLeaderSet->end())
								{
									itemCheckBox[i]->setEnable(true);
									itemCheckBox[i]->select(true);
								}
							}
							else
							{
								itemCheckBox[i]->setEnable(checkBoxsEnable);
							}
						}
						++ i;
					}
				}
				if (i <= 10 && CandidateControl.valid())
				{
					CandidateControl->setEnable(false);
				}
			}
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData,0));
			playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewSelectCountryLeaderMethod
//
/////////////////////////////////////////
crJXJNewSelectCountryLeaderMethod::crJXJNewSelectCountryLeaderMethod():
	m_this(NULL)
{
}

crJXJNewSelectCountryLeaderMethod::crJXJNewSelectCountryLeaderMethod(const crJXJNewSelectCountryLeaderMethod& handle):
	crMethod(handle),
	m_Recommend(handle.m_Recommend),
	m_tableWidget(handle.m_tableWidget),
	m_CandidateControl(handle.m_CandidateControl)
{
	for (int i = 0; i < 10; i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}

void crJXJNewSelectCountryLeaderMethod::inputParam(int i, void *param)
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

void crJXJNewSelectCountryLeaderMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_checkbox[i] = str;
		break;
	case 1:
		m_checkbox[i] = str;
		break;
	case 2:
		m_checkbox[i] = str;
		break;
	case 3:
		m_checkbox[i] = str;
		break;
	case 4:
		m_checkbox[i] = str;
		break;
	case 5:
		m_checkbox[i] = str;
		break;
	case 6:
		m_checkbox[i] = str;
		break;
	case 7:
		m_checkbox[i] = str;
		break;
	case 8:
		m_checkbox[i] = str;
		break;
	case 9:
		m_checkbox[i] = str;
		break;
	case 10:
		m_Recommend = str;
		break;
	case 11:
		m_CandidateControl = str;
		break;
	case 12:
		m_tableWidget = str;
		break;
	default:
		break;
	}
}

void crJXJNewSelectCountryLeaderMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Guanyuanxuanju");
		ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
		if (playerGameData && canvas.valid() && canvas_1.valid() && canvas_1->getDataClass())
		{
			void *param;
			crData *canvasdata = canvas_1->getDataClass();
			crData *playerdata = playerGameData->getDataClass();
			//typedef std::vector<int> SelectedIdVec;
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
			canvasdata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			CandidateVec *candidateVec = (CandidateVec *)param;
			//ref_ptr<crScrollBarWidgetNode> CandidateControl = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_CandidateControl));
			//CandidateControl->setEnable(false);
			//CandidateControl->setRange(0, candidateVec->size() - 10);
			//CandidateControl->setLineValue(1.0f);
			//CandidateControl->setPageValue(1.0f);
			//int start = 0;
			//if (candidateVec->size() > 10)
			//{
			//	CandidateControl->setEnable(true);
			//	start = CandidateControl->getValue();
			//}
			//else
			//{
			//	CandidateControl->setEnable(false);
			//}
			//int end = start + 10;
			playerdata->getParam(WCHDATA_JXJSelectedIdVec, param);
			SelectedIdVec *idVec = (SelectedIdVec *)param;
			ref_ptr<crCheckBoxWidgetNode> foldbtn[10] = {NULL};
			ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_tableWidget));
			//for (int i = 0; i < 10;i++)
			//{
			//	foldbtn[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
			//	if (foldbtn[i].valid() && foldbtn[i]->getSelect())
			//	{
			//		idVec.push_back(tableWidget->getData(i));
			//	}
			//}
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor && playerGameData)
			{
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(16);
				stream->_writeUChar(idVec->size());
				SelectedIdVec::iterator sivItr = idVec->begin();
				for (; sivItr != idVec->end(); ++ sivItr)
				{
					stream->_writeInt(*sivItr);
				}
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSelectCountryLeader,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJUINewChooseCheckBoxMethod
//
/////////////////////////////////////////
crJXJUINewChooseCheckBoxMethod::crJXJUINewChooseCheckBoxMethod():
	m_this(NULL)
{
}
crJXJUINewChooseCheckBoxMethod::crJXJUINewChooseCheckBoxMethod(const crJXJUINewChooseCheckBoxMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_Recommend(handle.m_Recommend),
	m_Recommend_sw(handle.m_Recommend_sw)
{
	for (int i = 0;i < 10 ;i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
		m_checkbox_sw[i] = handle.m_checkbox_sw[i];
	}
}
void crJXJUINewChooseCheckBoxMethod::inputParam(int i, void *param)
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
void crJXJUINewChooseCheckBoxMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_checkbox[0] = str;
		break;
	case 1:
		m_checkbox[1] = str;
		break;
	case 2:
		m_checkbox[2] = str;
		break;
	case 3:
		m_checkbox[3] = str;
		break;
	case 4:
		m_checkbox[4] = str;
		break;
	case 5:
		m_checkbox[5] = str;
		break;
	case 6:
		m_checkbox[6] = str;
		break;
	case 7:
		m_checkbox[7] = str;
		break;
	case 8:
		m_checkbox[8] = str;
		break;
	case 9:
		m_checkbox[9] = str;
		break;
	case 10:
		m_Recommend = str;
		break;
	case 11:
		m_CandidateControl = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_checkbox_sw[i - 12] = str;
		break;
	case 22:
		m_Recommend_sw = str;
		break;
	case 23:
		m_tableWidget = str;
		break;
	default:
		break;
	}
}
void crJXJUINewChooseCheckBoxMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
		if (playerGameData && canvas.valid() && canvas_1.valid() && canvas_1->getDataClass())
		{
			void *param;
			int iCheckedNumber = 0;
			crData *canvasdata = canvas_1->getDataClass();
			crData *playerdata = playerGameData->getDataClass();
			ref_ptr<crButtonWidgetNode>  recommend = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Recommend));
			ref_ptr<crCheckBoxWidgetNode> foldbtn[10] = {NULL};
			crMultiSwitch *foldbtn_sw[10] = {NULL};
			crMultiSwitch *recommend_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_Recommend_sw));

			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
			//canvasdata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			//CandidateVec *candidateVec = (CandidateVec *)param;
			//ref_ptr<crScrollBarWidgetNode> CandidateControl = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_CandidateControl));
			//CandidateControl->setEnable(false);
			//CandidateControl->setRange(0, candidateVec->size() - 10);
			//CandidateControl->setLineValue(1.0f);
			//CandidateControl->setPageValue(1.0f);
			//int start = 0;
			//if (candidateVec->size() > 10)
			//{
			//	CandidateControl->setEnable(true);
			//	start = CandidateControl->getValue();
			//}
			//else
			//{
			//	CandidateControl->setEnable(false);
			//}
			//int end = start + 10;

			playerdata->getParam(WCHDATA_JXJSelectedIdVec, param);
			SelectedIdVec *idVec = (SelectedIdVec *)param;
			ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_tableWidget));
			for (int i = 0; i < 10;i++)
			{
				foldbtn[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
				if (foldbtn[i].valid())
				{
					if (foldbtn[i]->getSelect())
					{
						if (std::find(idVec->begin(),idVec->end(),tableWidget->getData(i)) == idVec->end())
						{
							idVec->push_back(tableWidget->getData(i));
						}
					}
					else
					{
						if (std::find(idVec->begin(),idVec->end(),tableWidget->getData(i)) != idVec->end())
						{
							idVec->erase(std::find(idVec->begin(),idVec->end(),tableWidget->getData(i)));
						}
					}
				}
			}

			for (int i = 0; i < 10;i++)
			{
				foldbtn[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
				foldbtn_sw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_checkbox_sw[i]));
				if (foldbtn[i].valid())
				{
					if (foldbtn[i]->getSelect())
					{
						iCheckedNumber++;
					}
				}
			}
			for (int i = 0; i < 10; i++)
			{
				if (foldbtn[i].valid())
				{
					if (iCheckedNumber >= 2 && !foldbtn[i]->getSelect())
					{
						foldbtn[i]->setEnable(false);
						foldbtn_sw[i]->setActiveSwitchSet(2);
					}
					else
					{
						foldbtn[i]->setEnable(true);
						foldbtn_sw[i]->setActiveSwitchSet(0);
					}
				}
			}
			if (recommend.valid() && recommend->getVisiable())
			{
				if (iCheckedNumber > 2) 
				{
					recommend->setEnable(false);
					recommend_sw->setActiveSwitchSet(3);
				}
				else
				{
					recommend->setEnable(true);
					recommend_sw->setActiveSwitchSet(0);
				}
			}
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJStartXuanJuTimerMethod
//选举开始时间
/////////////////////////////////////////
crJXJStartXuanJuTimerMethod::crJXJStartXuanJuTimerMethod():
	m_this(NULL)
{
}
crJXJStartXuanJuTimerMethod::crJXJStartXuanJuTimerMethod(const crJXJStartXuanJuTimerMethod& handle):
	crMethod(handle),
	m_CandidateZiJiantimer(handle.m_CandidateZiJiantimer)
	//m_XuanJutimer(handle.m_XuanJutimer)
{
}
void crJXJStartXuanJuTimerMethod::inputParam(int i, void *param)
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
void crJXJStartXuanJuTimerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_CandidateZiJiantimer = str;
		break;
	//case 1:
	//	m_XuanJutimer = str;
	//	break;
	default:
		break;
	}
}
void crJXJStartXuanJuTimerMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{
		time_t curtime = time(0);
		crData *data = myPlayer->getDataClass();
		ref_ptr<crStaticTextWidgetNode> CandidateZiJianTimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_CandidateZiJiantimer));
		//ref_ptr<crStaticTextWidgetNode> XuanJuTimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_XuanJutimer));
		struct  tm  sTm = *localtime(&curtime);
		int _hour = 23 - sTm.tm_hour;
		int _min = 59 - sTm.tm_min;
		int _sec = 59 - sTm.tm_sec;
		std::string remaining;
		if (_sec >= 0)
		{
			remaining =  crArgumentParser::appItoa(_hour) 
				+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
		}
		else
		{
			remaining = "0:00:00";
		}
		if (CandidateZiJianTimer.valid())
		{
			CandidateZiJianTimer->clearString();
			CandidateZiJianTimer->setString(remaining);
		}
	}
}
////////////////////////////////
//
//crJXJHouXuanRenTipsUpdateMethod
//国家官员候选人自荐提示
///////////////////////////////////
crJXJHouXuanRenTipsUpdateMethod::crJXJHouXuanRenTipsUpdateMethod() :
	m_this(NULL)
{}

crJXJHouXuanRenTipsUpdateMethod::crJXJHouXuanRenTipsUpdateMethod(const crJXJHouXuanRenTipsUpdateMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_Title(handle.m_Title)
{
}

void crJXJHouXuanRenTipsUpdateMethod::inputParam(int i, void *param)
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

void crJXJHouXuanRenTipsUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Title = str;
		break;
	default:
		break;
	}
}

void crJXJHouXuanRenTipsUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Houxuanren");
		ref_ptr<crStaticTextWidgetNode> Tips = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Title));
		if (Tips.valid())
		{
			Tips->setString(std::string(" 因将军名列前茅,特邀将军参加国家官员竞选\n 希望将军成为国家官员后能带领国民获得最\n 后的胜利"));
		}
	}
}
/////////////////////////////////////////
//
//crJXJGiveUpCountryCandidateMethod
//放弃成为国家官员候选人
/////////////////////////////////////////
crJXJGiveUpCountryCandidateMethod::crJXJGiveUpCountryCandidateMethod()
{
}

crJXJGiveUpCountryCandidateMethod::crJXJGiveUpCountryCandidateMethod(const crJXJGiveUpCountryCandidateMethod& handle):
	crMethod(handle)
{
}

void crJXJGiveUpCountryCandidateMethod::inputParam(int i, void *param)
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

void crJXJGiveUpCountryCandidateMethod::addParam(int i, const std::string& str)
{
}

void crJXJGiveUpCountryCandidateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData && playerGameData->getDataClass())
	{
		int playerid = playerGameData->getPlayerID();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeInt(playerid);//1
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCountryInfo,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJConfirmCountryCandidateMethod
//确认成为国家官员候选人
/////////////////////////////////////////
crJXJConfirmCountryCandidateMethod::crJXJConfirmCountryCandidateMethod()
{
}

crJXJConfirmCountryCandidateMethod::crJXJConfirmCountryCandidateMethod(const crJXJConfirmCountryCandidateMethod& handle):
	crMethod(handle)
{
}

void crJXJConfirmCountryCandidateMethod::inputParam(int i, void *param)
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

void crJXJConfirmCountryCandidateMethod::addParam(int i, const std::string& str)
{
}

void crJXJConfirmCountryCandidateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData && playerGameData->getDataClass())
	{
		int playerid = playerGameData->getPlayerID();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeUChar(0);
			stream->_writeInt(playerid);//1
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSelectCountryLeader,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/**************************************************************************

 crJXJElectIconMethod
 //选举图标显示
**************************************************************************/

JXJ::crJXJElectIconMethod::crJXJElectIconMethod():
	m_this(NULL)
{

}

JXJ::crJXJElectIconMethod::crJXJElectIconMethod( const crJXJElectIconMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJElectIconMethod::inputParam( int i, void *param )
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
	};
}

void JXJ::crJXJElectIconMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJElectIconMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(gameServer)
		{
			time_t curtime = time(0);
			struct  tm  sTm = *localtime(&curtime);
			unsigned char electIcon = 0;
			switch (sTm.tm_wday)
			{
			case 6:
				electIcon = 6;
				break;
			case 0:
				electIcon = 7;
				break;
			case 1:
				electIcon = 1;
				break;
			default:
				break;
			}
			gameServer->getNetDataManager()->getPlayerDataMap();
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(2);
			stream->_writeUChar(electIcon);
			crPlayerDataEventPacket packetpk;
			crPlayerDataEventPacket::buildReplyPacket(packetpk, 0, WCH_JXJRecvQueryMyCountryInfo, stream.get());

			stream->seekBegin();
			//gsbraindata->inputParam(WCHDATA_JXJCenterNotice,stream.get());
			crStreamBuf *pkgstream = packetpk.getStreamBuf();
			int size = pkgstream->getBufSize();
			ref_ptr<crGameServerPlayerData> recvPlayerData;
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crNetManager *netManager = gameServer->getNetManager();
			if(netDataManager && netManager)
			{
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();
					++itr )
				{
					recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if (recvPlayerData.valid())
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(recvPlayerData->getPlayerID());
						pkgstream->setBufSize(size);
						gameServer->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packetpk);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIOfficialTipMethod
//官员领导力加成与职能提示
/////////////////////////////////////////
crJXJUIOfficialTipMethod::crJXJUIOfficialTipMethod():
	m_noticeid(0)
{
}
crJXJUIOfficialTipMethod::crJXJUIOfficialTipMethod(const crJXJUIOfficialTipMethod& handle):
	crMethod(handle),
	m_noticeid(handle.m_noticeid),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIOfficialTipMethod::inputParam(int i, void *param)
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
void crJXJUIOfficialTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_noticeid = atoi(str.c_str());
		break;
	case 1:
		m_tipsCanvas = str;
		break;
	case 2:
		m_tipsTextWidget = str;
		break;
	default:
		break;
	}
}
void crJXJUIOfficialTipMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{
		ref_ptr<crCanvasNode>tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
		if (tips.valid())
		{
			ref_ptr<crStaticTextWidgetNode> tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
			if (tipsText.valid())
			{
				crData *data = myPlayer->getDataClass();
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				std::string str;
				tipsText->setColor(crVector4(255.0,255.0,0.0,255.0));
				CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				if (noticeHandle)
				{
					int mode = 2;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&m_noticeid);
					noticeHandle->inputParam(WCHDATA_NoticeOutPut,&str);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					tipsText->setString(str);
				}
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				const crBoundingBox &iconbox = m_this->getBoundBox();
				const crBoundingBox &tipsbox = tips->getBoundBox();
				float posx = (tipsbox.m_max[0]-tipsbox.m_min[0])*0.5;
				float posy = (tipsbox.m_max[1]-tipsbox.m_min[1])*0.5;
				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
				/////////////////////////////////////////
				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
				float posx2,posy2;
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
				crVector2 mouse(crVector2(posx2,posy2));
				/////////////////////////////////////////
				tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
				crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCountryCanvasCancleMethod
//州郡管理取消驻扎
/////////////////////////////////////////
JXJ::crJXJCountryCanvasCancleMethod::crJXJCountryCanvasCancleMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryCanvasCancleMethod::crJXJCountryCanvasCancleMethod( const crJXJCountryCanvasCancleMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJCountryCanvasCancleMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJCountryCanvasCancleMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCountryCanvasCancleMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crCanvasNode> myCountryCanvas = m_this->getParentCanvas();
		do 
		{
			if (!netConductor || !myCountryCanvas ) break;
			crData * canvasData = myCountryCanvas->getDataClass();
			if (!canvasData) break;
			void *param;
			canvasData->getParam(WCHDATA_JXJCountryCanvasSelectedCantonId,param);
			int cancle_Type = -1;//取消驻扎，收包时用来区分申请驻扎
			int cantonId = *(int *)param;
			crPlayerDataEventPacket packet;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(8);
			stream->_writeInt(cancle_Type);
			stream->_writeInt(cantonId);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryCanvasApplyStation,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		} while (0);
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJUIBattlePreventAccMethod
//国战踢小号（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
crJXJUIBattlePreventAccMethod::crJXJUIBattlePreventAccMethod():m_this(NULL){}
crJXJUIBattlePreventAccMethod::crJXJUIBattlePreventAccMethod(const crJXJUIBattlePreventAccMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}

void crJXJUIBattlePreventAccMethod::inputParam(int i, void *param)
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
void crJXJUIBattlePreventAccMethod::addParam(int i,const std::string& str)
{
	switch(i)
	{
	case 0:
		m_input[0] = str;
		break;
	case 1:
		m_input[1] = str;
		break;
	case 2:
		m_input[2] = str;
		break;
	default:
		break;
	}
}
void crJXJUIBattlePreventAccMethod::operator()(crHandle &handle)
{
	void *param;
	crTableIO::StrVec record;
	ref_ptr<crStaticTextWidgetNode>  input[3];
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crTableIO> validTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJValidateTab);
	int questionIndex = validTab->getTitleIndex("验证码");

	input[0] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[0]));
	input[1] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[1]));
	input[2] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[2]));

	crData *data = playerGameData->getDataClass();
	if(data)
	{
		data->getParam(WCHDATA_JXJBattleValidateID, param);
		int validId = *(int *)param;
		if (input[0].valid()) 
		{
			input[0]->setString(std::string(TEXT("提问")));
		}
		if (input[1].valid())
		{
			input[1]->setString(std::string(TEXT("回答")));
		}
		if (validTab->queryOneRecord(0, crArgumentParser::appItoa(validId), record) > 0)
		{
			if (input[2].valid())
			{
				input[2]->setString(record[questionIndex]);
			}
		}
	}			
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleCheckIdentifyingCodeMethod
//国战踢小号验证确认（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
JXJ::crJXJBattleCheckIdentifyingCodeMethod::crJXJBattleCheckIdentifyingCodeMethod():
	m_this(NULL)
{
}

JXJ::crJXJBattleCheckIdentifyingCodeMethod::crJXJBattleCheckIdentifyingCodeMethod( const crJXJBattleCheckIdentifyingCodeMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_edit(handle.m_edit)
{
}
void JXJ::crJXJBattleCheckIdentifyingCodeMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJBattleCheckIdentifyingCodeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_edit = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJBattleCheckIdentifyingCodeMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		bool pass = false;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		ref_ptr<crEditWidgetNode> resultEdit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_edit));
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crTableIO> validTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJValidateTab);
		int resultIndex = validTab->getTitleIndex("结果");
		if (canvas.valid() && canvas->getDataClass() && resultEdit.valid() && playerGameData)
		{
			void *param;
			crTableIO::StrVec record;
			crData *data = playerGameData->getDataClass();
			int result = crArgumentParser::appAtoI64(resultEdit->getUTF8String());
			data->getParam(WCHDATA_JXJBattleValidateID, param);
			if (!param) return;
			int validId = *(int *)param;
			if (validTab->queryOneRecord(0, crArgumentParser::appItoa(validId), record) > 0)
			{
				int rightResult = crArgumentParser::appAtoI64(record[resultIndex]);
				if (rightResult == result)
				{
					pass = true;
				}
			}
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if (netConductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeBool(pass);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBattleCheckIdentifyingCode,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleCheckIdentifyingCodeMethod
//国战踢小号验证确认（防止小号挂机）
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleCheckIdentifyingCodeMethod::crJXJRecvBattleCheckIdentifyingCodeMethod():
	m_this(NULL),
	m_stream(NULL)
{
}
crJXJRecvBattleCheckIdentifyingCodeMethod::crJXJRecvBattleCheckIdentifyingCodeMethod(const crJXJRecvBattleCheckIdentifyingCodeMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvBattleCheckIdentifyingCodeMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_player = (crSceneServerPlayerData*)(HICREPARAM(param64));
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
		}
		else
		{
			m_player = NULL;
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvBattleCheckIdentifyingCodeMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvBattleCheckIdentifyingCodeMethod::operator()(crHandle &handle)
{
	crData *data = m_this->getDataClass();
	bool pass = m_stream->_readBool();
	void *param;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
	unsigned char fubentype = *(unsigned char *)param;
	if(fubentype == 1)
	{//1：城站
		if(!pass)
		{//验证失败，踢出玩家
			///游戏日志
			std::string logdata = "踢除战场无操作玩家";
			GameLogData gamelog(Log_NotActive,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(m_this->getPlayerID(),&gamelog));
			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			//聊天框消息
			crHandle *noticeHandle = crServerBrainHandle::getInstance()->getDataClass()->getHandle(WCH_JXJServerNotice);
			std::string outString ;
			int texid = 1097;
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJServerNotice);

			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(outString.length()+9);
			streamBuf->_writeChar(ChatAll);
			streamBuf->_writeString("[系统]");
			streamBuf->_writeString(outString);
			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvRoomChatMessage,streamBuf.get());
			sceneServerConductor->getNetManager()->sendPacket(m_player->getPlayerConnectServerAddress(),packet);
			//sceneServerConductor->getNetDataManager()->removePlayerData(m_this->getPlayerID());
		}
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）消费提示
//////////////////////////////////////////////////////////////////////////
crJXJUIBuyAppointColunmsMethod::crJXJUIBuyAppointColunmsMethod():
	m_index(0),
	m_this(NULL)
{

}

crJXJUIBuyAppointColunmsMethod::crJXJUIBuyAppointColunmsMethod(const crJXJUIBuyAppointColunmsMethod& handle):
	crMethod(handle),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_index(handle.m_index)
{

}


void crJXJUIBuyAppointColunmsMethod::inputParam(int i, void *param)
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

void crJXJUIBuyAppointColunmsMethod::addParam(int i, const std::string& str)
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
	case 3:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJUIBuyAppointColunmsMethod::operator()(crHandle &handle)
{
	void *param = NULL;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			//crVector2i price;
			int ifhint = 1;
			crRole *mainRole = playerGameData->getMainRole();
			crData *mainroledata = mainRole?mainRole->getDataClass():NULL;
			crData *data = playerGameData->getDataClass();
			if (!mainroledata || !data)
			{
				return;
			}
			std::string buildingname;

			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			int price = 0;
			unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,viplv).c_str()));
			//mainroledata->excHandle(MAKECREPARAM(WCH_LockData,1));
			mainroledata->getParam(WCHDATA_JXJAppointColumnsVec,param);
			//mainroledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			unsigned char appointColumns = *(unsigned char *)param;
			switch (appointColumns)
			{
			//当玩家不按顺序购买时，保证第一、二、三次购买扣除金额依次为200、300、500
			case 3:
				m_index = 0;
				break;
			case 4:
				m_index = 1;
				break;
			case 5:
				m_index = 2;
				break;
			default:
				break;
			}
			switch (m_index)
			{
			case 0:
				price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_1,viplv).c_str());
				break;
			case 1:
				price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_2,viplv).c_str());
				break;
			case 2:
				price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_3,viplv).c_str());
				break;
			default:
				break;
			}
			if(appointColumns < bagnummax2)
			{
				int needgold = price;
				playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKECREPARAM(&needgold,NULL));
				if (needgold == 0)
				{
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));

					if (ifhint)
					{
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
						v_i.push_back(140.0f);
						v_i.push_back(40.0f);
						v_i.push_back(40.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						char tmpText[32];
						sprintf(tmpText,"%d礼金/元宝\0",price);
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
						cfg_script.Add("Text","购买委任栏吗？");
						cfg_script.Pop();

						cfg_script.Pop();
						if(information.valid())information->setHypertext(cfg_script);
					}

					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = canvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					if (ifhint)
					{
						crFilterRenderManager::getInstance()->doModal(canvas.get());
					}
					else
					{
						if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(2036,NULL));
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(2098,NULL));
			}

		}
	}

}
//////////////////////////////////////////////////////////////////////////
//
//crJXJBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）
//////////////////////////////////////////////////////////////////////////
crJXJBuyAppointColunmsMethod::crJXJBuyAppointColunmsMethod():
	m_index(0),
	m_this(NULL)
{

}

crJXJBuyAppointColunmsMethod::crJXJBuyAppointColunmsMethod(const crJXJBuyAppointColunmsMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{

}

void crJXJBuyAppointColunmsMethod::inputParam(int i, void *param)
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

void crJXJBuyAppointColunmsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJBuyAppointColunmsMethod::operator ()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
		streamBuf->createBuf(1);
		streamBuf->_writeUChar(m_index);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuyAppointColunms,streamBuf.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBuyAppointColunmsMethod
//购买委任栏（第二页的三个委任栏为可购买对象）
//////////////////////////////////////////////////////////////////////////
crJXJRecvBuyAppointColunmsMethod::crJXJRecvBuyAppointColunmsMethod():
	m_netType(GameClient_Game)
{

}

crJXJRecvBuyAppointColunmsMethod::crJXJRecvBuyAppointColunmsMethod(const crJXJRecvBuyAppointColunmsMethod& handle)
{

}

void crJXJRecvBuyAppointColunmsMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvBuyAppointColunmsMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvBuyAppointColunmsMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			unsigned char m_index = m_stream->_readUChar();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (serverPlayerData.valid() && serverPlayerData->getPlayerGameData() && serverPlayerData->getPlayerGameData()->getMainRole())
			{
				crPlayerGameData* playerData = serverPlayerData->getPlayerGameData();
				crRole *MainRole = playerData->getMainRole();

				CRCore::crData* data = m_this->getDataClass();
				CRCore::crData* roleData = MainRole->getDataClass();
				if(data && roleData)
				{
					void *param;
					char success = 0;
					bool needdeductgold = false;
					int needgold = 0;

					data->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,viplv).c_str()));
					data->excHandle(MAKECREPARAM(WCH_LockData,0));
					//roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJAppointColumnsVec,param);
					unsigned char appointColumns = *(unsigned char *)param;
					switch (appointColumns)
					{
					//当玩家不按顺序购买时，保证第一、二、三次购买扣除金额依次为200、300、500
					case 3:
						m_index = 0;
						break;
					case 4:
						m_index = 1;
						break;
					case 5:
						m_index = 2;
						break;
					default:
						break;
					}
					switch (m_index)
					{
					case 0:
						needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_1,viplv).c_str());
						break;
					case 1:
						needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_2,viplv).c_str());
						break;
					case 2:
						needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPricePerAppointColumn_3,viplv).c_str());
						break;
					default:
						break;
					}
					do 
					{
						if (appointColumns >= bagnummax2)
						{
							//委任栏已满
							success = -1;
							break;
						}
						MoneyChangeData moneydata(needgold,"购买委任栏");
						m_this->doEvent(WCH_JXJDeductLijinGold,MAKECREPARAM(&moneydata,&needdeductgold));
						if (moneydata.first == 0)
						{
							success = 1;//购买委任栏成功
						}
						else
						{
							//元宝不够
							success = -2;
							break;
						}
						if (success == 1)
						{
							appointColumns += 1;
						}
					} while (0);
					//roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
					if (success == 1)
					{
						roleData->inputParam(WCHDATA_JXJAppointColumnsVec,&appointColumns);
						//unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,viplv).c_str()));
						//data->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
						//std::vector<JXJAppointCardVec> *appointjiangkabagvec = (std::vector<JXJAppointCardVec> *)param;
						//appointjiangkabagvec->resize(*appointColumns);
						//JXJAppointCardVec appointCardVec;
						//appointCardVec.resize(bagnummax2);
						//(*appointjiangkabagvec)[(*appointColumns)-1] = appointCardVec;
						if (needdeductgold)
						{
							crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
							callback->savePlayerData(serverPlayerData.get());
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					if (success == 1)
					{
						stream->createBuf(2);
						stream->_writeChar(success);
						stream->_writeUChar(appointColumns);
					}
					else
					{
						stream->createBuf(1);
						stream->_writeChar(success);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyAppointColunms,stream.get());
					gameServer->getNetManager()->sendPacket(serverPlayerData->getPlayerConnectServerAddress(),packet);
					///游戏日志
					std::string logdata = crArgumentParser::appItoa(success);
					GameLogData gamelog(Log_BuyAppointColumns,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream -> _readChar();
			int tipsindex = 0;
			if (success == 1)
			{
				tipsindex = 7009;
				unsigned char appointColumns = m_stream->_readUChar();
				crRole* MainRole = m_this->getMainRole();
				if (MainRole)
				{
					crData *roledata = MainRole->getDataClass();
					crData *data = m_this->getDataClass();
					if (roledata && data)
					{
						data->getParam(WCHDATA_JXJVipLv,param);
						unsigned char viplv = *(unsigned char *)param;
						//roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
						roledata->inputParam(WCHDATA_JXJAppointColumnsVec,&appointColumns);
						//roledata->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
						//std::vector<JXJAppointCardVec> *appointjiangkabagvec = (std::vector<JXJAppointCardVec> *)param;
						//roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
						//unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,viplv).c_str()));
						//appointjiangkabagvec->resize(appointColumns);
						//JXJAppointCardVec appointCardVec;
						//appointCardVec.resize(bagnummax2);
						//(*appointjiangkabagvec)[appointColumns-1] = appointCardVec;
					}
				}
			}
			else
			{
				switch (success)
				{
				case -1://切页已最大
					tipsindex = 2098;
					break;
				case -2://元宝不足
					tipsindex = 2036;
					break;
				default:
					break;
				}
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(tipsindex,NULL));//
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJDelayCanvasCloseMethod
//窗口延时自动关闭
//////////////////////////////////////////////////////////////////////////
crJXJDelayCanvasCloseMethod::crJXJDelayCanvasCloseMethod():
	m_this(NULL)
{

}
crJXJDelayCanvasCloseMethod::crJXJDelayCanvasCloseMethod(const crJXJDelayCanvasCloseMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJDelayCanvasCloseMethod::inputParam(int i, void *param)
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
void crJXJDelayCanvasCloseMethod::addParam(int i, const std::string& str)
{
}
void crJXJDelayCanvasCloseMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid() && m_this)
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJDelayCanvasCloseTime,param);
		short timer = *(short*)param;
		if(timer<=0)
			crFilterRenderManager::getInstance()->showCanvas(m_this,false);
		if(timer>0)
			timer--;
		cameraData->inputParam(WCHDATA_JXJDelayCanvasCloseTime,&timer);
	}
}
/////////////////////////////////////////
//
//crJXJCameraOperatorDefaultMethod
//鼠标切换为默认状态
/////////////////////////////////////////
crJXJCameraOperatorDefaultMethod::crJXJCameraOperatorDefaultMethod():
	m_this(NULL){}
crJXJCameraOperatorDefaultMethod::crJXJCameraOperatorDefaultMethod(const crJXJCameraOperatorDefaultMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCameraOperatorDefaultMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJCameraOperatorDefaultMethod::addParam(int i, const std::string& str)
{
}

void crJXJCameraOperatorDefaultMethod::operator()(crHandle &handle)
{
	void *param;
	//bool isatttack = false;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid() && camera->getAttachedNode() && camera->getAttachedNode()->getDataClass())
	{
		crData* cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		unsigned short attackToPos = *(unsigned short *)param;
		attackToPos = Cam_Default;
		//isatttack = true;
		cameraData->inputParam(WCHDATA_JXJCameraOperator,&attackToPos);
		//handle.outputParam(0,&attackToPos);
	}
}
/////////////////////////////////////////
//
//crJXJCheckIsMyselfMethod
//检查是否刷新为玩家自己的装备信息
/////////////////////////////////////////
crJXJCheckIsMyselfMethod::crJXJCheckIsMyselfMethod():
	m_this(NULL){}
crJXJCheckIsMyselfMethod::crJXJCheckIsMyselfMethod(const crJXJCheckIsMyselfMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckIsMyselfMethod::inputParam(int i, void *param)
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
	}
}

void crJXJCheckIsMyselfMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckIsMyselfMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData)
	{
		bool myself = true;
		crData *playerdata = playerGameData->getDataClass();
		playerdata->inputParam(WCHDATA_JXJCheckIsMyself,&myself);
	}
}
/////////////////////////////////////////////////////////////////////////
//crJXJShowZhengzhaoBtnCanvasMethod
//征召按钮窗口显示
/////////////////////////////////////////////////////////////////////////
crJXJShowZhengzhaoBtnCanvasMethod::crJXJShowZhengzhaoBtnCanvasMethod()
{

}
crJXJShowZhengzhaoBtnCanvasMethod::crJXJShowZhengzhaoBtnCanvasMethod(const crJXJShowZhengzhaoBtnCanvasMethod &handle)
{

}
void crJXJShowZhengzhaoBtnCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJShowZhengzhaoBtnCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_Canvas = str;
		break;
	case 1:
		m_minimapCanvas = str;
		break;
	}
}
void crJXJShowZhengzhaoBtnCanvasMethod::operator ()(crHandle &handle)
{
	CRCore::ref_ptr<crCanvasNode>  zhengzhaoBtnCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_Canvas);
	CRCore::ref_ptr<crCanvasNode>  miniMapCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_minimapCanvas);
	if(miniMapCanvas.valid() && zhengzhaoBtnCanvas.valid() && (zhengzhaoBtnCanvas->getVisiable()==false))
	{
		const crBoundingBox& canvas_box = zhengzhaoBtnCanvas->getBoundBox();
		const crBoundingBox& bbox = miniMapCanvas->getBoundBox();
		zhengzhaoBtnCanvas->setMatrix(crMatrix::translate(bbox.xMax() + canvas_box.xLength()*0.5f + 0.02, bbox.yMin() + canvas_box.yLength()*0.5f, 0.0f));
		crFilterRenderManager::getInstance()->showCanvas(zhengzhaoBtnCanvas.get(), true);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckZhengZhaoBtnMethod
//征召按钮控制
//////////////////////////////////////////////////////////////////////////
crJXJCheckZhengZhaoBtnMethod::crJXJCheckZhengZhaoBtnMethod():
	m_this(NULL)
{
}
crJXJCheckZhengZhaoBtnMethod::crJXJCheckZhengZhaoBtnMethod(const crJXJCheckZhengZhaoBtnMethod& handle):
	crMethod(handle),
	m_guojiazhengzhao(handle.m_guojiazhengzhao),
	m_juntuanzhengzhao(handle.m_juntuanzhengzhao),
	m_juntuan_sw(handle.m_juntuan_sw)
{

}
void crJXJCheckZhengZhaoBtnMethod::inputParam(int i, void *param)
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
void crJXJCheckZhengZhaoBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_guojiazhengzhao = str;
		break;
	case 1:
		m_juntuanzhengzhao = str;
		break;
	case 2:
		m_juntuan_sw = str;
		break;
	default:
		break;
	}
}
void crJXJCheckZhengZhaoBtnMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{
		void *param;
		bool ZhengZhaoKaiQi_GuoJia = false;
		bool ZhengZhaoKaiQi_JunTuan = false;
		unsigned char myGuanZhi = 4;//默认为平民
		unsigned char MaxTimes = 0;
		ref_ptr<crButtonWidgetNode> GuoJiaZhengzhao = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_guojiazhengzhao));
		ref_ptr<crButtonWidgetNode> JunTuanzhengzhao = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_juntuanzhengzhao));
		ref_ptr<crMultiSwitch> JunTuan_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_juntuan_sw));
		crData *data = myPlayer->getDataClass();
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char fubentype = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJOpenBattle, param);
		bool isbattleopened = *(bool *)param;
		int groupid = 0;
		data-> getParam(WCHDATA_JXJConsortiaID,param);
		if (param)
		{
			groupid = *(int *)param;
		}
		if (fubentype == FT_Fuben)
		{
			if (GuoJiaZhengzhao.valid() && GuoJiaZhengzhao->getVisiable())
			{
				GuoJiaZhengzhao->setEnable(false);
				GuoJiaZhengzhao->setVisiable(false);
			}
			if (JunTuanzhengzhao.valid() && JunTuanzhengzhao->getVisiable())
			{
				JunTuanzhengzhao->setEnable(false);
				JunTuanzhengzhao->setVisiable(false);
			}
		}
		if (fubentype == FT_WorldFuben)
		{
			if (GuoJiaZhengzhao.valid() && GuoJiaZhengzhao->getVisiable())
			{
				GuoJiaZhengzhao->setEnable(false);
				GuoJiaZhengzhao->setVisiable(false);
			}
			if (JunTuanzhengzhao.valid())
			{
				if (groupid > 0)
				{
					JunTuanzhengzhao->setEnable(true);
					JunTuanzhengzhao->setVisiable(true);
					JunTuan_sw->setActiveSwitchSet(1);
					time_t nowTime_t = time(0);
					struct  tm  nowTm = *localtime(&nowTime_t);
					data->getParam(WCHDATA_JXJJunTuanReleaseTimer, param);
					int lastTimer = *(int *)param;
					if (nowTime_t - lastTimer < 180)
					{
						JunTuanzhengzhao->setEnable(false);
						GuoJiaZhengzhao->setEnable(false);
					}
				}
				else
				{
					JunTuanzhengzhao->setEnable(false);
					JunTuanzhengzhao->setVisiable(false);
				}
			}
		}
		if (fubentype == FT_Chengchi/* && isbattleopened*/)
		{
			int remain_timer = 0;
			int Invalid_timer = 0;
			unsigned char myviplv = 0;
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
			{
				remain_timer = room->timeRemain();
			}
			data->getParam(WCHDATA_JXJVipLv,param);
			if (param) myviplv = *(unsigned char *)param;
			Invalid_timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenInvalidTimer,myviplv).c_str());
			if (remain_timer < Invalid_timer)
			{//该战场即将结束，无法再发布征召
				if (GuoJiaZhengzhao.valid() && GuoJiaZhengzhao->getVisiable())
				{
					GuoJiaZhengzhao->setVisiable(false);
				}
				if (JunTuanzhengzhao.valid() && JunTuanzhengzhao->getVisiable())
				{
					JunTuanzhengzhao->setVisiable(false);
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				return;
			}
			if (isbattleopened)
			{//城战开启
				data->getParam(WCHDATA_JXJPlayerGuanZhi, param);
				if (param)
				{
					myGuanZhi = *(unsigned char *)param;
				}
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = 0;
				if (param)
				{
					viplv = *(unsigned char *)param;
				}
				switch (myGuanZhi)
				{
				case 1:
					MaxTimes = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGuoJiaZhengZhaoTimes_1,viplv).c_str()));
					break;
				case 2:
				case 3:
					MaxTimes = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGuoJiaZhengZhaoTimes_2,viplv).c_str()));
					break;
				default:
					break;
				}
				data->getParam(WCHDATA_JXJUsedZhengZhaoTimes, param);
				unsigned char usedZhengZhaoTimes = 0;
				if (param)
				{
					usedZhengZhaoTimes = *(unsigned char *)param;
				}
				if (usedZhengZhaoTimes < MaxTimes)
				{
					ZhengZhaoKaiQi_GuoJia = true;
				}
				if (GuoJiaZhengzhao.valid())
				{
					if (ZhengZhaoKaiQi_GuoJia)
					{
						GuoJiaZhengzhao->setVisiable(true);
						GuoJiaZhengzhao->setEnable(true);
						time_t nowTime_t = time(0);
						struct  tm  nowTm = *localtime(&nowTime_t);
						data->getParam(WCHDATA_JXJGuoJiaReleaseTimer, param);
						int lastTimer = *(int *)param;
						if (nowTime_t - lastTimer < 180)
						{
							JunTuanzhengzhao->setEnable(false);
							GuoJiaZhengzhao->setEnable(false);
						}
					}
					else
					{
						GuoJiaZhengzhao->setEnable(false);
						GuoJiaZhengzhao->setVisiable(false);
					}
				}
			}
			if (JunTuanzhengzhao.valid())
			{
				if (groupid > 0)
				{
					JunTuanzhengzhao->setEnable(true);
					JunTuanzhengzhao->setVisiable(true);
					JunTuan_sw->setActiveSwitchSet(1);
					time_t nowTime_t = time(0);
					struct  tm  nowTm = *localtime(&nowTime_t);
					data->getParam(WCHDATA_JXJJunTuanReleaseTimer, param);
					int lastTimer = *(int *)param;
					data->getParam(WCHDATA_JXJGuoJiaReleaseTimer, param);
					int lastTimer_1 = *(int *)param;
					if (nowTime_t - lastTimer < 180 || nowTime_t - lastTimer_1 < 180)
					{
						JunTuanzhengzhao->setEnable(false);
						GuoJiaZhengzhao->setEnable(false);
					}
				}
				else
				{
					JunTuanzhengzhao->setEnable(false);
					JunTuanzhengzhao->setVisiable(false);
				}
			}
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJZhengZhaoFaBuCanvasMethod
//征召发布窗口
//////////////////////////////////////////////////////////////////////////
crJXJZhengZhaoFaBuCanvasMethod::crJXJZhengZhaoFaBuCanvasMethod():
	m_index(0),
	m_this(NULL)
{
}
crJXJZhengZhaoFaBuCanvasMethod::crJXJZhengZhaoFaBuCanvasMethod(const crJXJZhengZhaoFaBuCanvasMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_index(handle.m_index),
	m_mark(handle.m_mark),
	m_newcanvas(handle.m_newcanvas),
	m_inputAttack(handle.m_inputAttack),
	m_tips(handle.m_tips),
	m_inputDefend(handle.m_inputDefend)
{

}
void crJXJZhengZhaoFaBuCanvasMethod::inputParam(int i, void *param)
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
void crJXJZhengZhaoFaBuCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	case 2:
		m_mark = str;
		break;
	case 3:
		m_newcanvas = str;
		break;
	case 4:
		m_inputAttack = str;
		break;
	case 5:
		m_tips = str;
		break;
	case 6:
		m_inputDefend = str;
		break;
	default:
		break;
	}
}
void crJXJZhengZhaoFaBuCanvasMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode>canvas_1 = crFilterRenderManager::getInstance()->findCanvas(m_newcanvas);
	if(m_this && myPlayer && canvas.valid() && canvas_1.valid())
	{
		int tipsindex = 0;
		char success = 0;
		std::wstring text;
		ref_ptr<crStaticTextWidgetNode> tips = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_tips));
		ref_ptr<crMultiSwitch > mark = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mark));
		if (mark.valid())
		{
			switch(m_index)
			{
			case 0:
				mark->setActiveSwitchSet(0); //0 国家征召
				break;
			case 1:
				mark->setActiveSwitchSet(1); //1 军团征召
				break;
			default:
				break;
			}
		}
		if (m_index == 0)
		{
			void* param;
			crData* playerdata = myPlayer->getDataClass();
			if (playerdata)
			{
				playerdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
				playerdata->getParam(WCHDATA_JXJVipLv, param);
				unsigned char vipLv = *(unsigned char *)param;
				int cost = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGuoJiaZhengZhaoCost,vipLv).c_str()));
				playerdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
				if (tips.valid())
				{
					std::string strMessage;
					strMessage = "本次征召需消耗" + crArgumentParser::appItoa(cost) + "国家资金";
					tips->setString(strMessage);
				}
				unsigned char rolesCount = 0;
				playerdata->getParam(WCHDATA_JXJShiliID, param);
				unsigned char myShiliId = *(unsigned char *)param;
				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				if(room && room->getDataClass())
				{
					ref_ptr<crStaticTextWidgetNode> MyTotalRoles = dynamic_cast<crStaticTextWidgetNode *>(canvas_1->getWidget(m_inputAttack));
					crData* roomData = room->getDataClass();
					roomData->getParam(WCHDATA_JXJBattleID, param);
					int battleId = *(int *)param;
					unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleId)));
					if (myShiliId != attackShili)
					{
						MyTotalRoles = dynamic_cast<crStaticTextWidgetNode *>(canvas_1->getWidget(m_inputDefend));
					}
					if (MyTotalRoles.valid())
					{
						text = MyTotalRoles->getString();
					}
					std::string totalRoles = CRIOManager::convertUTF16toUTF8(text.c_str());
					rolesCount = atoi(totalRoles.c_str());
					if (rolesCount < 10)
					{
						success = 1;
					}
					else
					{
						success = 2;
						tipsindex = 7011; //友方人数超过10人，不能开启征召
					}
					switch (success)
					{
					case 1:
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
						break;
					case 2:
						myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(tipsindex,NULL));
						break;
					default:
						break;
					}
				}
			}
		}
		if (m_index == 1)
		{
			void* param;
			crData* playerdata = myPlayer->getDataClass();
			if (playerdata)
			{
				if (tips.valid())
				{
					std::string strMessage;
					strMessage = "本次征召需消耗1个军团征召令";
					tips->setString(strMessage);
				}

				//检测背包里现有的军团征召令数量
				playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerdata->getParam(WCHDATA_JXJItemBagVec, param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				int itemid = 7523;
				int itemcount = 0;
				for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
					itemitr != itembagvec->end();
					++itemitr)
				{
					if (itemitr->get())
					{
						if (itemid == itemitr->get()->getItemID())
						{
							itemcount = (int)itemitr->get()->getItemCount();
						}
					}
				}
				if (itemcount > 0)
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
				else
				{
					tipsindex = 7012;
					myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(tipsindex,NULL));
				}
				playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJReleaseGuoJiaZhengZhaoMethod
//发布国家征召
//////////////////////////////////////////////////////////////////////////
crJXJReleaseGuoJiaZhengZhaoMethod::crJXJReleaseGuoJiaZhengZhaoMethod():
	m_this(NULL)
{

}

crJXJReleaseGuoJiaZhengZhaoMethod::crJXJReleaseGuoJiaZhengZhaoMethod(const crJXJReleaseGuoJiaZhengZhaoMethod& handle):
	crMethod(handle),
	m_fuyan(handle.m_fuyan),
	m_mark(handle.m_mark),
	m_canvasname(handle.m_canvasname),
	m_chatmodesw(handle.m_chatmodesw),
	m_chatinput(handle.m_chatinput),
	m_sendbtn(handle.m_sendbtn)
{

}

void crJXJReleaseGuoJiaZhengZhaoMethod::inputParam(int i, void *param)
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

void crJXJReleaseGuoJiaZhengZhaoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_fuyan = str;
		break;
	case 1:
		m_mark = str;
		break;
	case 2:
		m_canvasname = str;
		break;
	case 3:
		m_chatmodesw = str;
		break;
	case 4:
		m_chatinput = str;
		break;
	case 5:
		m_sendbtn = str;
		break;
	case 6:
		m_switch = str;
		break;
	default:
		break;
	}
}

void crJXJReleaseGuoJiaZhengZhaoMethod::operator ()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(netConductor && m_this && playerGameData)
	{
		ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
		ref_ptr<crMultiSwitch > mark = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_mark));
		unsigned char type = 0;
		if (mark->getActiveSwitchSet() == 0)
		{
			type = 1; //国家
		}
		else
		{
			type = 2; //军团
		}
		if (1 == type)
		{
			void *param;
			int sceneid = 0;
			unsigned char myGuanZhi = 4; //初始化为平民
			string official;
			string myplayername = playerGameData->getCharacterName();
			string battlename;
			string fuyan;
			unsigned char m_battletype = 0;
			crTableIO::StrVec record;
			crData *data = playerGameData->getDataClass();
			if (data)
			{
				int playerid = playerGameData->getPlayerID();
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJPlayerGuanZhi, param);
				if (param)
				{
					myGuanZhi = *(unsigned char *)param;
				}
				switch (myGuanZhi)
				{
				case 1:
					official = "丞相" + myplayername;
					break;
				case 2:
					official = "大司马" + myplayername;
					break;
				case 3:
					official = "大将军" + myplayername;
					break;
				default:
					break;
				}
				ref_ptr<crEditWidgetNode> Fuyan_input = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_fuyan));
				ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				if (Fuyan_input.valid())
				{
					fuyan = Fuyan_input->getUTF8String();
					string strMessage = "非必填-不高于20字";
					if (fuyan == strMessage)
					{
						fuyan = "";
					}
				}
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myShili = *(unsigned char *)param;

				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				crData *roomData = room->getDataClass();
				roomData->getParam(WCHDATA_JXJBattleID,param);
				int battleid = *(int *)param;
				int roomid = room->getRoomID();
				_crInt64 battleroomid = MAKEINT64(battleid,roomid);
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				m_battletype = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short fubenid = *(unsigned short *)param;
				if (FT_WorldFuben != m_battletype)
				{
					if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record) >= 0 )
					{
						int nameIndex = chengchiTab->getTitleIndex("名字");
						battlename = record[nameIndex].c_str();
						unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
						bool npcchengchi = defenceShili<c_startShiliID;
						crVector2i sceneids;
						crArgumentParser::appAtoVec(record[chengchiTab->getTitleIndex("sceneid")], sceneids);
						sceneid = npcchengchi? sceneids[1]:sceneids[0];
					}
				}
				else
				{
					ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
					if(worldfubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
					{
						int sceneindex = worldfubenTab->getTitleIndex("sceneid");
						sceneid = atoi(record[sceneindex].c_str());
						battlename = record[worldfubenTab->getTitleIndex("name")].c_str();
					}
				}
				if (sceneid <= 0) 
				{
					data->excHandle(MAKECREPARAM(WCH_LockData, 0));
					return;
				}
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(150);
				streamBuf->_writeInt(playerid);
				streamBuf->_writeUChar(myShili);
				streamBuf->_writeString(official); 
				streamBuf->_writeString(battlename);
				streamBuf->_writeInt64(battleroomid);
				streamBuf->_writeInt(sceneid);
				streamBuf->_writeString(fuyan);
				streamBuf->_writeUChar(m_battletype);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvReleaseGuoJiaZhengZhao,streamBuf.get());
				netConductor->getNetManager()->sendPacket("all",packet);
				data->getParam(WCHDATA_JXJUsedZhengZhaoTimes, param);
				if (param)
				{
					unsigned char *usedZhengZhaoTimes = (unsigned char *)param;
					(*usedZhengZhaoTimes) += 1;
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		if (2 == type)
		{
			void *param;
			int sceneid = 0;
			string myplayername = playerGameData->getCharacterName();
			string battlename;
			string fuyan;
			unsigned char m_battletype = 0;
			crTableIO::StrVec record;
			crData *data = playerGameData->getDataClass();
			if (data)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				int playerid = playerGameData->getPlayerID();
				ref_ptr<crEditWidgetNode> Fuyan_input = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_fuyan));
				ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				if (Fuyan_input.valid())
				{
					fuyan = Fuyan_input->getUTF8String();
					string strMessage = "非必填-不高于20字";
					if (fuyan == strMessage)
					{
						fuyan = "";
					}
				}
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myShili = *(unsigned char *)param;

				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				crData *roomData = room->getDataClass();
				roomData->getParam(WCHDATA_JXJBattleID,param);
				int battleid = *(int *)param;
				int roomid = room->getRoomID();
				_crInt64 battleroomid = MAKEINT64(battleid,roomid);
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				m_battletype = *(unsigned char *)param;				

				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short fubenid = *(unsigned short *)param;
				if (FT_WorldFuben != m_battletype)
				{
					if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record) >= 0 )
					{
						int nameIndex = chengchiTab->getTitleIndex("名字");
						battlename = record[nameIndex].c_str();
						unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
						bool npcchengchi = defenceShili<c_startShiliID;
						crVector2i sceneids;
						crArgumentParser::appAtoVec(record[chengchiTab->getTitleIndex("sceneid")], sceneids);
						sceneid = npcchengchi ? sceneids[1] : sceneids[0];
					}
				}
				else
				{
					ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
					if(worldfubenTab->queryOneRecord(0,crArgumentParser::appItoa(10000),record)>=0)
					{
						int sceneindex = worldfubenTab->getTitleIndex("sceneid");
						sceneid = atoi(record[sceneindex].c_str());
						battlename = record[worldfubenTab->getTitleIndex("name")].c_str();
					}
				}
				if (sceneid <= 0) 
				{
					data->excHandle(MAKECREPARAM(WCH_LockData, 0));
					return;
				}
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(150);
				streamBuf->_writeInt(playerid);
				streamBuf->_writeUChar(myShili);
				streamBuf->_writeString(myplayername); 
				streamBuf->_writeString(battlename);
				streamBuf->_writeInt64(battleroomid);
				streamBuf->_writeInt(sceneid);
				streamBuf->_writeString(fuyan);
				streamBuf->_writeUChar(m_battletype);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvReleaseJunTuanZhengZhao,streamBuf.get());
				netConductor->getNetManager()->sendPacket("all",packet);
				//检测背包里现有的军团征召令数量
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				unsigned char itemcount = 0;
				unsigned char needitemcount = 1;
				int needitemid = 7523;
				for (JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr)
				{
					if (itr->get())
					{
						if (itr->get()->getItemID() == needitemid)
						{
							itemcount = itr->get()->getItemCount();
							if(itemcount >= needitemcount)
							{
								itr->get()->setItemCount(itemcount - needitemcount);
								if (itemcount - needitemcount == 0)
								{
									(*itr) = NULL;
								}
								break;
							}
							else
							{
								(*itr) = NULL;
								needitemcount -= itemcount;
							}
						}
					}
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				time_t nowTime_t = time(0);
				struct  tm  nowTm = *localtime(&nowTime_t);
				data->inputParam(WCHDATA_JXJJunTuanReleaseTimer, &nowTime_t);

				ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
				if(!canvas)return;
				ref_ptr<crRadioGroupWidgetNode> chatmodesw = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_chatmodesw));
				ref_ptr<crStaticTextWidgetNode> chatinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_chatinput));
				ref_ptr<crButtonWidgetNode> sendbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_sendbtn));
				crMultiSwitch *mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
				if(!chatmodesw || !chatinput ||!sendbtn || !mulitSwitch)return;
				chatmodesw->select(ChatInGroup);
				chatmodesw->doEvent(WCH_UI_SelectChange);
				mulitSwitch->setActiveSwitchSet(ChatInGroup-1);

				crTableIO::StrVec record;
				std::string name;
				std::string strMessage;
				int textid = 0;
				if (FT_WorldFuben == m_battletype)
				{
					strMessage = "军团玩家" + myplayername + "发布世界战场征召令，请前往支援";
				}
				else
				{
					strMessage = "军团玩家" + myplayername + "发布" + battlename + "征召令，请前往支援";
				}
				if(!strMessage.empty())
				{
					chatinput->setString(strMessage);
					sendbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
				//std::string text_1;
				//text_1 = "消耗1个军团令";
				//ref_ptr<crJXJChatRecord> chatrecord_1 = new crJXJChatRecord();
				//chatrecord_1->setChatText(text_1);
				////record->setTextColor(COLOR_GREEN);
				//chatrecord->setChatChannelName("[系统]");
				//crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(chatrecord_1.get(),NULL));
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvReleaseGuoJiaZhengZhaoMethod
//发布国家征召
//////////////////////////////////////////////////////////////////////////
crJXJRecvReleaseGuoJiaZhengZhaoMethod::crJXJRecvReleaseGuoJiaZhengZhaoMethod(){}
crJXJRecvReleaseGuoJiaZhengZhaoMethod::crJXJRecvReleaseGuoJiaZhengZhaoMethod(const crJXJRecvReleaseGuoJiaZhengZhaoMethod& handle):
	crMethod(handle),
	m_canvasname(handle.m_canvasname),
	m_chatmodesw(handle.m_chatmodesw),
	m_chatinput(handle.m_chatinput),
	m_sendbtn(handle.m_sendbtn)
{
}
void crJXJRecvReleaseGuoJiaZhengZhaoMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvReleaseGuoJiaZhengZhaoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasname = str;
		break;
	case 1:
		m_chatmodesw = str;
		break;
	case 2:
		m_chatinput = str;
		break;
	case 3:
		m_sendbtn = str;
		break;
	case 4:
		m_switch = str;
		break;
	default:
		break;
	}
}
void crJXJRecvReleaseGuoJiaZhengZhaoMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_stream->_readInt();
			unsigned char publisherShili = m_stream->_readUChar();
			string official = m_stream->_readString();
			string battleName = m_stream->_readString();
			_crInt64 battleroomid = m_stream->_readInt64();
			int sceneid = m_stream->_readInt();
			string fuyan = m_stream->_readString();
			unsigned char battletype = m_stream->_readUChar();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			if(!gameServer) return;
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				bool success = true;
				void *param;
				crPlayerGameData* pdata = playerData->getPlayerGameData();
				crData* data = pdata->getDataClass(); 
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				crJXJShili *shili;
				crData *shiliData;
				PeopleMap *peopleMap = NULL;
				PeopleMap::iterator peoItr;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJTroopsRankingMap,param);
				TroopsRankingMap *rankingMap = (TroopsRankingMap *)param;
				gsBrainData->getParam(WCHDATA_JXJShiliWei+publisherShili-10,param);
				shili = (crJXJShili *)param;
				if (shili && shili->getDataClass())
				{
					shiliData = shili->getDataClass();
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char vipLv = *(unsigned char *)param;
					int cost = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGuoJiaZhengZhaoCost,vipLv).c_str()));
					shiliData->getParam(WCHDATA_JXJCountryCapital,param);
					int *guojiazijin = (int *)param;
					if (*guojiazijin >= cost)
					{
						*guojiazijin -= cost;
					}
					else
					{
						success = false;
					}
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					peopleMap = (PeopleMap *)param;
					if (success)
					{
						peoItr = peopleMap->begin();
						if (!(rankingMap->empty()))
						{
							TroopsRankingMap::iterator pmItr = rankingMap->begin();
							int count = 0;
							int olplayerid = 0;
							ref_ptr<crGameServerPlayerData> OlplayerData ;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(150);
							stream->_writeUChar(1); //1 表示国家征召、 3 表示军团征召
							stream->_writeString(official);  
							stream->_writeString(battleName);
							stream->_writeInt64(battleroomid);
							stream->_writeInt(sceneid);
							stream->_writeString(fuyan);
							stream->_writeUChar(battletype);
							crPlayerDataEventPacket packet;
							for (; pmItr != rankingMap->end() && pmItr->second.valid() && count < 30; pmItr++)
							{
								peoItr = peopleMap->find(pmItr->second->getPlayerID());
								if(peoItr != peopleMap->end() && peoItr->second.valid() && !peoItr->second->getSmallAcc())
								{
									olplayerid = pmItr->second->getPlayerID();
									OlplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(olplayerid));
									if(OlplayerData.valid() && OlplayerData->getPlayerGameData() && OlplayerData->getPlayerGameData()->getMainRole() && OlplayerData->getSceneID()==0)
									{
										crPlayerDataEventPacket::buildReplyPacket(packet,olplayerid,WCH_JXJShowYingZhengCanvas,stream.get());
										gameServer->getNetManager()->sendPacket(OlplayerData->getPlayerConnectServerAddress(),packet);
										count++;
									}
								}
							}
						}
						data->getParam(WCHDATA_JXJUsedZhengZhaoTimes, param);
						if (param)
						{
							unsigned char *usedZhengZhaoTimes = (unsigned char *)param;
							if (usedZhengZhaoTimes > 0)
							{
								(*usedZhengZhaoTimes) += 1;
							}
						}
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(32);
						stream->_writeBool(success);
						stream->_writeString(official);  
						stream->_writeString(battleName);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvReleaseGuoJiaZhengZhao,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					else
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1);
						stream->_writeBool(success);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvReleaseGuoJiaZhengZhao,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			bool succuss = m_stream->_readBool();
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if (myPlayer)
			{
				if (!succuss)
				{
					int tipsindex = 7013;
					myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(tipsindex,NULL));
				}
				else
				{
					string official = m_stream->_readString();
					string battleName = m_stream->_readString();	
					crData* data = myPlayer->getDataClass();
					if (data)
					{
						time_t nowTime_t = time(0);
						struct  tm  nowTm = *localtime(&nowTime_t);
						data->inputParam(WCHDATA_JXJGuoJiaReleaseTimer, &nowTime_t);

						ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
						if(!canvas)return;
						ref_ptr<crRadioGroupWidgetNode> chatmodesw = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_chatmodesw));
						ref_ptr<crStaticTextWidgetNode> chatinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_chatinput));
						ref_ptr<crButtonWidgetNode> sendbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_sendbtn));
						crMultiSwitch *mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
						if(!chatmodesw || !chatinput ||!sendbtn || !mulitSwitch)return;
						chatmodesw->select(ChatInCountry);
						chatmodesw->doEvent(WCH_UI_SelectChange);
						mulitSwitch->setActiveSwitchSet(ChatInCountry-1);

						crTableIO::StrVec record;
						std::string name;
						std::string strMessage;
						int textid = 0;
						strMessage = official + "发布国战出击令，全军出击" + battleName + "，请前往支援";
						if(!strMessage.empty())
						{
							chatinput->setString(strMessage);
							sendbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
						}
						//std::string text_1;
						//text_1 = "消耗50W国家资金";
						//ref_ptr<crJXJChatRecord> chatrecord_1 = new crJXJChatRecord();
						//chatrecord_1->setChatText(text_1);
						////record->setTextColor(COLOR_GREEN);
						//chatrecord->setChatChannelName("[系统]");
						//crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(chatrecord_1.get(),NULL));
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvReleaseJuanTuanZhengZhaoMethod
//发布军团征召
//////////////////////////////////////////////////////////////////////////
crJXJRecvReleaseJuanTuanZhengZhaoMethod::crJXJRecvReleaseJuanTuanZhengZhaoMethod(){}
crJXJRecvReleaseJuanTuanZhengZhaoMethod::crJXJRecvReleaseJuanTuanZhengZhaoMethod(const crJXJRecvReleaseJuanTuanZhengZhaoMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvReleaseJuanTuanZhengZhaoMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvReleaseJuanTuanZhengZhaoMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvReleaseJuanTuanZhengZhaoMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		int playerid = m_stream->_readInt();
		unsigned char publisherShili = m_stream->_readUChar();
		string official = m_stream->_readString();
		string battleName = m_stream->_readString();
		_crInt64 battleroomid = m_stream->_readInt64();
		int sceneid = m_stream->_readInt();
		string fuyan = m_stream->_readString();
		unsigned char battletype = m_stream->_readUChar();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(!gameServer) return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
		{
			void *param;
			crPlayerGameData* pdata = playerData->getPlayerGameData();
			crData* data = pdata->getDataClass(); 
			data-> getParam(WCHDATA_JXJConsortiaID,param);
			int groupid = *(int *)param;
			data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			unsigned char fubentype = *(unsigned char *)param;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator itr = consortiamap->find(groupid);
			ref_ptr<crData> groupData;
			if (itr != consortiamap->end())
			{
				groupData = itr->second->getDataClass();
			}
			if (groupData.valid())
			{
				pdata->doEvent(WCH_JXJRecvQueryMyGroupInfo,MAKECREPARAM(NULL,GameServer));
				groupData->excHandle(MAKECREPARAM(WCH_LockData,1));
				groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
				groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				ConsortiaMemberMap *consortiamembermap = (ConsortiaMemberMap *)param;
				int olplayerid = 0;
				ref_ptr<crGameServerPlayerData> OlplayerData ;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(150);
				stream->_writeUChar(3); //1 表示国家征召、 3 表示军团征召
				stream->_writeString(official);  
				stream->_writeString(battleName);
				stream->_writeInt64(battleroomid);
				stream->_writeInt(sceneid);
				stream->_writeString(fuyan);
				stream->_writeUChar(battletype);
				crPlayerDataEventPacket packet;
				for (ConsortiaMemberMap::iterator itr = consortiamembermap->begin();itr != consortiamembermap->end();++itr)
				{
					olplayerid = itr->second->getPlayerID();
					OlplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(olplayerid));
					if(OlplayerData.valid() && OlplayerData->getPlayerGameData() && OlplayerData->getPlayerGameData()->getMainRole())
					{
						crPlayerDataEventPacket::buildReplyPacket(packet,olplayerid,WCH_JXJShowYingZhengCanvas,stream.get());
						gameServer->getNetManager()->sendPacket(OlplayerData->getPlayerConnectServerAddress(),packet);
					}
				}
				groupData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
			//检测背包里现有的军团征召令数量
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			unsigned char itemcount = 0;
			unsigned char needitemcount = 1;
			int needitemid = 7523;
			for (JXJItemBagVec::iterator itr = itembagvec->begin();
				itr != itembagvec->end();
				++itr)
			{
				if (itr->get())
				{
					if (itr->get()->getItemID() == needitemid)
					{
						itemcount = itr->get()->getItemCount();
						if(itemcount >= needitemcount)
						{
							itr->get()->setItemCount(itemcount - needitemcount);
							if (itemcount - needitemcount == 0)
							{
								(*itr) = NULL;
							}
							break;
						}
						else
						{
							(*itr) = NULL;
							needitemcount -= itemcount;
						}
					}
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJGainZhengZhaoLingVecMethod
//生成征召令信息
//////////////////////////////////////////////////////////////////////////
crJXJGainZhengZhaoLingVecMethod::crJXJGainZhengZhaoLingVecMethod():
	m_this(NULL)
{

}
crJXJGainZhengZhaoLingVecMethod::crJXJGainZhengZhaoLingVecMethod(const crJXJGainZhengZhaoLingVecMethod &handle):
	crMethod(handle)
{
}
void crJXJGainZhengZhaoLingVecMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJGainZhengZhaoLingVecMethod::addParam(int i, const std::string& str)
{
}
void crJXJGainZhengZhaoLingVecMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		void *param;
		crData* data = playerGameData->getDataClass();
		unsigned char type = m_stream->_readUChar();
		std::string releasePlayer = m_stream->_readString();
		std::string newbattleName = m_stream->_readString();
		_crInt64 battleroomId = m_stream->_readInt64();
		int sceneId = m_stream->_readInt();
		unsigned short timer = 180;
		std::string LiuYan = m_stream->_readString();
		unsigned char battletype = m_stream->_readUChar();
		if (data)
		{
			time_t nowTime_t = time(0);
			struct  tm  nowTm = *localtime(&nowTime_t);
			data->inputParam(WCHDATA_JXJZhengzhaolingUpdateTimer, &nowTime_t);

			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			// 当前页面显示的征召令
			ZhengzhaolingVec *CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = (ZhengzhaolingVec*)param;
			CurZhengzhaolingVec->clear(); //只存一组，存入之前清空
			CRCore::ref_ptr<strZhengzhaoInfo> strinfo = new strZhengzhaoInfo;
			strinfo->m_type = type;
			strinfo->m_publisher = releasePlayer;
			strinfo->m_chengchiName = newbattleName;
			strinfo->m_battleroomid = battleroomId;
			strinfo->m_sceneid = sceneId;
			strinfo->m_timer = timer;
			strinfo->m_liuyan = LiuYan;
			strinfo->m_battletype = battletype;
			CurZhengzhaolingVec->push_back(strinfo);

			// 国家征召令缓存列表
			if (1 == type)
			{
				ZhengzhaolingVec *guanyuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
				guanyuanlingVec = (ZhengzhaolingVec*)param;
				CRCore::ref_ptr<strZhengzhaoInfo> strinfo = new strZhengzhaoInfo;
				strinfo->m_type = type;
				strinfo->m_publisher = releasePlayer;
				strinfo->m_chengchiName = newbattleName;
				strinfo->m_battleroomid = battleroomId;
				strinfo->m_sceneid = sceneId;
				strinfo->m_timer = timer; 
				strinfo->m_liuyan = LiuYan;
				strinfo->m_battletype = battletype;
				guanyuanlingVec->push_back(strinfo);
			}

			// 军团征召令缓存列表
			if (3 == type)
			{
				ZhengzhaolingVec *juntuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
				juntuanlingVec = (ZhengzhaolingVec*)param;
				CRCore::ref_ptr<strZhengzhaoInfo> strinfo = new strZhengzhaoInfo;
				strinfo->m_type = type;
				strinfo->m_publisher = releasePlayer;
				strinfo->m_chengchiName = newbattleName;
				strinfo->m_battleroomid = battleroomId;
				strinfo->m_sceneid = sceneId;
				strinfo->m_timer = timer;
				strinfo->m_liuyan = LiuYan;
				strinfo->m_battletype = battletype;
				juntuanlingVec->push_back(strinfo);
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJShowYingZhengCanvasMethod
//显示国家应征窗口
//////////////////////////////////////////////////////////////////////////
crJXJShowYingZhengCanvasMethod::crJXJShowYingZhengCanvasMethod():
	m_this(NULL)
{

}

crJXJShowYingZhengCanvasMethod::crJXJShowYingZhengCanvasMethod(const crJXJShowYingZhengCanvasMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_type(handle.m_type),
	m_publisher(handle.m_publisher),
	m_battleName(handle.m_battleName),
	m_mainIconQ(handle.m_mainIconQ),
	m_mainIconW(handle.m_mainIconW),
	m_mainIconE(handle.m_mainIconE),
	m_iconQ(handle.m_iconQ),
	m_iconW(handle.m_iconW),
	m_iconE(handle.m_iconE),
	m_mainIconQSwitch(handle.m_mainIconQSwitch),
	m_mainIconWSwitch(handle.m_mainIconWSwitch),
	m_mainIconESwitch(handle.m_mainIconESwitch),
	m_iconQSwitch(handle.m_iconQSwitch),
	m_iconWSwitch(handle.m_iconWSwitch),
	m_iconESwitch(handle.m_iconESwitch),
	m_rewardInstruction(handle.m_rewardInstruction),
	m_timer(handle.m_timer),
	m_message(handle.m_message),
	m_newcanvas(handle.m_newcanvas),
	m_leftPageBtn(handle.m_leftPageBtn),
	m_rightPageBtn(handle.m_rightPageBtn)
{
	for (unsigned char i = 0; i < 3; i++)
	{
		m_army[i] = handle.m_army[i];
		m_army_sw[i] = handle.m_army[i];
		m_armyCount[i] = handle.m_armyCount[i];
		m_formationIcon[i] = handle.m_formationIcon[i];
	}
}

void crJXJShowYingZhengCanvasMethod::inputParam(int i, void *param)
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
	}
}

void crJXJShowYingZhengCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_type = str;
		break;
	case 2:
		m_publisher = str;
		break;
	case 3:
		m_battleName = str;
		break;
	case 4:
		m_army[0] = str;
		break;
	case 5:
		m_army[1] = str;
		break;
	case 6:
		m_army[2] = str;
		break;
	case 7:
		m_army_sw[0] = str;
		break;
	case 8:
		m_army_sw[1] = str;
		break;
	case 9:
		m_army_sw[2] = str;
		break;
	case 10:
		m_mainIconQ = str;
		break;
	case 11:
		m_mainIconW = str;
		break;
	case 12:
		m_mainIconE = str;
		break;
	case 13:
		m_iconQ = str;
		break;
	case 14:
		m_iconW = str;
		break;
	case 15:
		m_iconE = str;
		break;
	case 16:
		m_mainIconQSwitch = str;
		break;
	case 17:
		m_mainIconWSwitch = str;
		break;
	case 18:
		m_mainIconESwitch = str;
		break;
	case 19:
		m_iconQSwitch = str;
		break;
	case 20:
		m_iconWSwitch = str;
		break;
	case 21:
		m_iconESwitch = str;
		break;
	case 22:
		m_armyCount[0] = str;
		break;
	case 23:
		m_armyCount[1] = str;
		break;
	case 24:
		m_armyCount[2] = str;
		break;
	case 25:
		m_rewardInstruction = str;
		break;
	case 26:
		m_timer = str;
		break;
	case 27:
		m_message = str;
		break;
	case 28:
		m_newcanvas = str;
		break;
	case 29:
		m_formationIcon[0] = str;
		break;
	case 30:
		m_formationIcon[1] = str;
		break;
	case 31:
		m_formationIcon[2] = str;
		break;
	case 32:
		m_newArmyCount[0] = str;
		break;
	case 33:
		m_newArmyCount[1] = str;
		break;
	case 34:
		m_newArmyCount[2] = str;
		break;
	case 35:
		m_leftPageBtn = str;
		break;
	case 36:
		m_rightPageBtn = str;
		break;
	default:
		break;
	}
}

void crJXJShowYingZhengCanvasMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		void *param;
		crData *playerData = playerGameData->getDataClass();
		if(playerData)
		{
			unsigned char type = 0;
			std::string releasePlayer;
			std::string newbattleName;
			_crInt64 battleroomId;
			int sceneId;
			unsigned short timer;
			std::string LiuYan;
			unsigned char battletype = 0;
			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			ZhengzhaolingVec *CurZhengzhaolingVec = (ZhengzhaolingVec *)param;
			ZhengzhaolingVec::iterator cvItr = CurZhengzhaolingVec->begin();
			if (cvItr != CurZhengzhaolingVec->end() && cvItr->valid())
			{
				type = (*cvItr)->m_type;
				releasePlayer = (*cvItr)->m_publisher;
				newbattleName = (*cvItr)->m_chengchiName;
				battleroomId = (*cvItr)->m_battleroomid;
				sceneId = (*cvItr)->m_sceneid;
				timer = (*cvItr)->m_timer;
				LiuYan = (*cvItr)->m_liuyan;
				battletype = (*cvItr)->m_battletype;
			}
			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
			std::string imageName[3];
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crCanvasNode> newcanvas = crFilterRenderManager::getInstance()->findCanvas(m_newcanvas);
			if (!(canvas.valid() && newcanvas.valid()))
			{
				return;
			}
			ref_ptr<crImageBoxWidgetNode> mainIconQ = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_mainIconQ));
			ref_ptr<crImageBoxWidgetNode> mainIconW = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_mainIconW));
			ref_ptr<crImageBoxWidgetNode> mainIconE = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_mainIconE));
			ref_ptr<crImageBoxWidgetNode> iconQ = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_iconQ));
			ref_ptr<crImageBoxWidgetNode> iconW = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_iconW));
			ref_ptr<crImageBoxWidgetNode> iconE = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_iconE));
			ref_ptr<crImageBoxWidgetNode> armyIcon[3];
			ref_ptr<crImageBoxWidgetNode> newformationIcon[3];

			ref_ptr<crMultiSwitch> typeSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_type));
			ref_ptr<crMultiSwitch> mainIconQSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mainIconQSwitch));
			ref_ptr<crMultiSwitch> mainIconWSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mainIconWSwitch));
			ref_ptr<crMultiSwitch> mainIconESwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mainIconESwitch));
			ref_ptr<crMultiSwitch> iconQSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_iconQSwitch));
			ref_ptr<crMultiSwitch> iconWSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_iconWSwitch));
			ref_ptr<crMultiSwitch> iconESwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_iconESwitch));

			ref_ptr<crStaticTextWidgetNode> publisher = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_publisher));
			ref_ptr<crStaticTextWidgetNode> battleName = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_battleName));
			ref_ptr<crStaticTextWidgetNode> rewardInstruction = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_rewardInstruction));
			ref_ptr<crStaticTextWidgetNode> message = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_message));
			ref_ptr<crStaticTextWidgetNode> armyCount[3];
			ref_ptr<crStaticTextWidgetNode> newArmyCount[3];
			ref_ptr<crButtonWidgetNode> leftPageBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_leftPageBtn));
			ref_ptr<crButtonWidgetNode> rightPageBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_rightPageBtn));

			unsigned char curPageNum = 1;
			playerData->getParam(WCHDATA_JXJCurZhengzhaolingPage, param);
			if (param)
			{
				curPageNum = *(unsigned char *)param;
				if (0 == curPageNum)
				{
					curPageNum = 1;
				}
			}
			if (type == 1)
			{
				ZhengzhaolingVec guanyuanlingVec;
				playerData->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
				guanyuanlingVec = *(ZhengzhaolingVec*)param;
				if (leftPageBtn.valid())
				{
					if (curPageNum < 2)
					{
						leftPageBtn->setEnable(false);
					}
					else
					{
						leftPageBtn->setVisiable(true);
						leftPageBtn->setEnable(true);
					}
				}
				if (rightPageBtn.valid())
				{
					if (curPageNum >= guanyuanlingVec.size())
					{
						rightPageBtn->setEnable(false);
					}
					else
					{
						rightPageBtn->setVisiable(true);
						rightPageBtn->setEnable(true);
					}
				}
				if (typeSwitch.valid())
				{
					typeSwitch->setActiveSwitchSet(1);
				}
				if (rewardInstruction.valid())
				{
					rewardInstruction->setString(std::string("出兵成功可获得5000国家贡献"));
				}
			}
			if (type == 3)
			{
				ZhengzhaolingVec juntuanlingVec;
				playerData->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
				juntuanlingVec = *(ZhengzhaolingVec*)param;
				if (leftPageBtn.valid())
				{
					if (curPageNum < 2)
					{
						leftPageBtn->setEnable(false);
					}
					else
					{
						leftPageBtn->setVisiable(true);
						leftPageBtn->setEnable(true);
					}
				}
				if (rightPageBtn.valid())
				{
					if (curPageNum >= juntuanlingVec.size())
					{
						rightPageBtn->setEnable(false);
					}
					else
					{
						rightPageBtn->setVisiable(true);
						rightPageBtn->setEnable(true);
					}
				}
				if (typeSwitch.valid())
				{
					typeSwitch->setActiveSwitchSet(0);
				}
				if (rewardInstruction.valid())
				{
					rewardInstruction->setString(std::string("出兵成功可获得1000国家贡献"));
				}
			}
			if (publisher.valid())
			{
				publisher->setString(releasePlayer);
			}
			if (battleName.valid())
			{
				battleName->setString(newbattleName);
			}
			if (message.valid())
			{
				message->setString(LiuYan);
			}
			for (int i = 0; i < 3;i++)
			{
				armyIcon[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_army[i]));
				newformationIcon[i] = dynamic_cast<crImageBoxWidgetNode *>(newcanvas->getWidget(m_formationIcon[i]));
				armyCount[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_armyCount[i]));
				newArmyCount[i] = dynamic_cast<crStaticTextWidgetNode *>(newcanvas->getWidget(m_newArmyCount[i]));
			}

			ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);

			mainIconQ->setVisiable(false);
			mainIconW->setVisiable(false);
			mainIconE->setVisiable(false);
			iconQ->setVisiable(false);
			iconW->setVisiable(false);
			iconE->setVisiable(false);

			int iconid = itemtab->getTitleIndex("icon");
			int nameid = itemtab->getTitleIndex("name");
			int levelid = itemtab->getTitleIndex("品质");
			crVector2i equip = crVector2i(-1,-1);
			crTableIO::StrVec record,itemrecord1;
			int index = 0;
			int cardcolor = 0;
			int abstractid = 0;
			unsigned short count = 0;
			playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
			FormationInfoVec *formationVec = (FormationInfoVec *)param;
			//crMyPlayerData::MyRoleNpcMap roleMap =  crMyPlayerData::getInstance()->getMyRoleNpcMap();
			//for(crMyPlayerData::MyRoleNpcMap::iterator itr_role = roleMap.begin();itr_role!=roleMap.end();itr_role++,index++)
			for(FormationInfoVec::iterator itr = formationVec->begin(); itr != formationVec->end() && index< 3;)
			{
				if (itr->get())
				{
					equip = itr->get()->getEquips();
					count = itr->get()->getCount();
					abstractid = itr->get()->getAbstractID();
					itr++;
				}
				if(index == 0)
				{
					if(equip[0]>0 || equip[1]>0)
					{
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							mainIconQ->setVisiable(true);
							mainIconQ->setImageName(itemrecord1[iconid]);
							mainIconQSwitch->setActiveSwitchSet(cardcolor+1);
						}
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							iconQ->setVisiable(true);
							iconQ->setImageName(itemrecord1[iconid]);
							iconQSwitch->setActiveSwitchSet(cardcolor+1);
						}
					}
					if(troopsTab->queryOneRecord(0, crArgumentParser::appItoa(abstractid), record) >= 0)
					{
						int iconindex = troopsTab->getTitleIndex("icon");
						if (armyIcon[index].valid())
						{
							armyIcon[index]->setImageName(record[iconindex]);
							armyIcon[index]->setVisiable(true);
						}
					}
					if (armyCount[index].valid())
					{
						armyCount[index]->setString(crArgumentParser::appItoa(count));
					}
					index++;
				}
				else if(index == 1)
				{
					if(equip[0]>0 || equip[1]>0)
					{
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							mainIconW->setVisiable(true);
							mainIconW->setImageName(itemrecord1[iconid]);
							mainIconWSwitch->setActiveSwitchSet(cardcolor+1);
						}
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							iconW->setVisiable(true);
							iconW->setImageName(itemrecord1[iconid]);
							iconWSwitch->setActiveSwitchSet(cardcolor+1);
						}
					}
					if(troopsTab->queryOneRecord(0, crArgumentParser::appItoa(abstractid), record) >= 0)
					{
						int iconindex = troopsTab->getTitleIndex("icon");
						if (armyIcon[index].valid())
						{
							armyIcon[index]->setImageName(record[iconindex]);
							armyIcon[index]->setVisiable(true);
						}
					}
					if (armyCount[index].valid())
					{
						armyCount[index]->setString(crArgumentParser::appItoa(count));
					}
					index++;
				}
				else if(index == 2)
				{
					if(equip[0]>0 || equip[1]>0)
					{
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							mainIconE->setVisiable(true);
							mainIconE->setImageName(itemrecord1[iconid]);
							mainIconESwitch->setActiveSwitchSet(cardcolor+1);
						}
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
						{
							cardcolor = atoi(itemrecord1[levelid].c_str());
							iconE->setVisiable(true);
							iconE->setImageName(itemrecord1[iconid]);
							iconESwitch->setActiveSwitchSet(cardcolor+1);
						}
					}
					if(troopsTab->queryOneRecord(0, crArgumentParser::appItoa(abstractid), record) >= 0)
					{
						int iconindex = troopsTab->getTitleIndex("icon");
						if (armyIcon[index].valid())
						{
							armyIcon[index]->setImageName(record[iconindex]);
							armyIcon[index]->setVisiable(true);
						}
					}
					if (armyCount[index].valid())
					{
						armyCount[index]->setString(crArgumentParser::appItoa(count));
					}
					index++;
				}
			}
			//crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJGuanYuanLingTimerUpdateMethod
//征召令信息刷新
//////////////////////////////////////////////////////////////////////////
crJXJGuanYuanLingTimerUpdateMethod::crJXJGuanYuanLingTimerUpdateMethod():
	m_this(NULL)
{

}
crJXJGuanYuanLingTimerUpdateMethod::crJXJGuanYuanLingTimerUpdateMethod(const crJXJGuanYuanLingTimerUpdateMethod &handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_guanyuanling(handle.m_guanyuanling),
	m_juntuanling(handle.m_juntuanling),
	m_count_1(handle.m_count_1),
	m_count_2(handle.m_count_2)
{
}
void crJXJGuanYuanLingTimerUpdateMethod::inputParam(int i, void *param)
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
void crJXJGuanYuanLingTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_guanyuanling = str;
		break;
	case 2:
		m_juntuanling = str;
		break;
	case 3:
		m_count_1 = str;
		break;
	case 4:
		m_count_2 = str;
		break;
	default:
		break;
	}
}
void crJXJGuanYuanLingTimerUpdateMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(m_this && playerGameData && canvas.valid())
	{
		ref_ptr<crMultiSwitch> zhengzhaoBtn_sw_1 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_guanyuanling));
		ref_ptr<crMultiSwitch> zhengzhaoBtn_sw_2 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_juntuanling));
		ref_ptr<crStaticTextWidgetNode> count_1 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_count_1));
		ref_ptr<crStaticTextWidgetNode> count_2 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_count_2));
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			// 当前页面显示的征召令
			ZhengzhaolingVec *CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));

			time_t nowTime_t = time(0);
			struct  tm  nowTm = *localtime(&nowTime_t);
			data->getParam(WCHDATA_JXJZhengzhaolingUpdateTimer, param);
			int lastTimer = *(int *)param;
			int dt = nowTime_t - lastTimer;
			lastTimer = nowTime_t;
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = (ZhengzhaolingVec*)param;
			cvItr = CurZhengzhaolingVec->begin();
			if (cvItr != CurZhengzhaolingVec->end() && cvItr->valid())
			{
				if ((*cvItr)->m_timer > 0)
				{
					if ((*cvItr)->m_timer - dt >= 0)
					{
						(*cvItr)->m_timer -= dt;
					}
					else
					{
						(*cvItr)->m_timer = 0;
					}
				}
				else
				{
					cvItr = CurZhengzhaolingVec->erase(cvItr);						
				}
			}

			// 国家征召令缓存列表
			ZhengzhaolingVec *guanyuanlingVec;
			data->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
			guanyuanlingVec = (ZhengzhaolingVec*)param;
			for (cvItr = guanyuanlingVec->begin(); cvItr != guanyuanlingVec->end();)
			{
				if (cvItr->valid())
				{
					if ((*cvItr)->m_timer > 0)
					{
						if ((*cvItr)->m_timer - dt >= 0)
						{
							(*cvItr)->m_timer -= dt;
						}
						else
						{
							(*cvItr)->m_timer = 0;
						}
						++ cvItr;
					}
					else
					{
						cvItr = guanyuanlingVec->erase(cvItr);						
					}
				}
				else
				{
					cvItr = guanyuanlingVec->erase(cvItr);						
				}
			}
			unsigned char size_1 = guanyuanlingVec->size();

			// 军团征召令缓存列表
			ZhengzhaolingVec *juntuanlingVec;
			data->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
			juntuanlingVec = (ZhengzhaolingVec*)param;
			for (cvItr = juntuanlingVec->begin(); cvItr != juntuanlingVec->end();)
			{
				if (cvItr->valid())
				{
					if ((*cvItr)->m_timer > 0)
					{
						if ((*cvItr)->m_timer - dt >= 0)
						{
							(*cvItr)->m_timer -= dt;
						}
						else
						{
							(*cvItr)->m_timer = 0;
						}
						++ cvItr;
					}
					else
					{
						cvItr = juntuanlingVec->erase(cvItr);
					}
				}
				else
				{
					cvItr = juntuanlingVec->erase(cvItr);						
				}
			}
			unsigned char size_2 = juntuanlingVec->size();

			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			data->inputParam(WCHDATA_JXJZhengzhaolingUpdateTimer, &lastTimer);
			if (zhengzhaoBtn_sw_1.valid() && zhengzhaoBtn_sw_2.valid())
			{
				if (0 == size_1 && 0 == size_2)
				{
					zhengzhaoBtn_sw_1->setVisiable(false);
					zhengzhaoBtn_sw_2->setVisiable(false);
				}
				else if (0 == size_1)
				{
					zhengzhaoBtn_sw_1->setVisiable(true);
					zhengzhaoBtn_sw_1->setActiveSwitchSet(1);
					zhengzhaoBtn_sw_2->setVisiable(false);
					if (count_1.valid())
					{
						if (!count_1->getVisiable())
						{
							count_1->setVisiable(true);
						}
						count_1->setString(crArgumentParser::appItoa(size_2));
					}
				}
				else if (0 == size_2)
				{
					zhengzhaoBtn_sw_1->setVisiable(true);
					zhengzhaoBtn_sw_1->setActiveSwitchSet(0);
					zhengzhaoBtn_sw_2->setVisiable(false);
					if (count_1.valid())
					{
						if (!count_1->getVisiable())
						{
							count_1->setVisiable(true);
						}
						count_1->setString(crArgumentParser::appItoa(size_1));
					}
				}
				else
				{
					zhengzhaoBtn_sw_1->setVisiable(true);
					zhengzhaoBtn_sw_1->setActiveSwitchSet(0);
					zhengzhaoBtn_sw_2->setVisiable(true);
					zhengzhaoBtn_sw_2->setActiveSwitchSet(1);
					if (count_1.valid())
					{
						if (!count_1->getVisiable())
						{
							count_1->setVisiable(true);
						}
						count_1->setString(crArgumentParser::appItoa(size_1));
					}
					if (count_2.valid())
					{
						if (!count_2->getVisiable())
						{
							count_2->setVisiable(true);
						}
						count_2->setString(crArgumentParser::appItoa(size_2));
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJClickZhengZhaoLingBtnMethod
//单击征召令弹窗
//////////////////////////////////////////////////////////////////////////
crJXJClickZhengZhaoLingBtnMethod::crJXJClickZhengZhaoLingBtnMethod():
	m_this(NULL)
{

}
crJXJClickZhengZhaoLingBtnMethod::crJXJClickZhengZhaoLingBtnMethod(const crJXJClickZhengZhaoLingBtnMethod &handle):
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJClickZhengZhaoLingBtnMethod::inputParam(int i, void *param)
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
void crJXJClickZhengZhaoLingBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJClickZhengZhaoLingBtnMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Zhengzhao_Yingzheng");
	if(m_this && playerGameData && canvas.valid() && !canvas->getVisiable())
	{
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			// 当前页面显示的征召令
			ZhengzhaolingVec *CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			unsigned char pageNum = 1;
			data->inputParam(WCHDATA_JXJCurZhengzhaolingPage, &pageNum);
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = (ZhengzhaolingVec*)param;
			CurZhengzhaolingVec->clear(); //只存一组，存入之前清空
			CRCore::ref_ptr<strZhengzhaoInfo> strinfo = new strZhengzhaoInfo;
			if (m_index == 1) //国家
			{
				// 国家征召令缓存列表
				ZhengzhaolingVec guanyuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
				guanyuanlingVec = *(ZhengzhaolingVec*)param;
				for (cvItr = guanyuanlingVec.begin(); cvItr != guanyuanlingVec.end(); ++ cvItr)
				{
					if (cvItr->valid() && (*cvItr)->m_timer > 0)
					{
						strinfo->m_type = (*cvItr)->m_type;
						strinfo->m_publisher = (*cvItr)->m_publisher;
						strinfo->m_chengchiName =(*cvItr)->m_chengchiName;
						strinfo->m_battleroomid = (*cvItr)->m_battleroomid;
						strinfo->m_sceneid = (*cvItr)->m_sceneid;
						strinfo->m_timer = (*cvItr)->m_timer;
						strinfo->m_liuyan = (*cvItr)->m_liuyan;
						strinfo->m_battletype = (*cvItr)->m_battletype;
						CurZhengzhaolingVec->push_back(strinfo);
						break;
					}
				}
			}
			if (m_index == 2)
			{
				// 军团征召令缓存列表
				ZhengzhaolingVec juntuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
				juntuanlingVec = *(ZhengzhaolingVec*)param;
				for (cvItr = juntuanlingVec.begin(); cvItr != juntuanlingVec.end(); ++ cvItr)
				{
					if (cvItr->valid() && (*cvItr)->m_timer > 0)
					{
						strinfo->m_type = (*cvItr)->m_type;
						strinfo->m_publisher = (*cvItr)->m_publisher;
						strinfo->m_chengchiName =(*cvItr)->m_chengchiName;
						strinfo->m_battleroomid = (*cvItr)->m_battleroomid;
						strinfo->m_sceneid = (*cvItr)->m_sceneid;
						strinfo->m_timer = (*cvItr)->m_timer;
						strinfo->m_liuyan = (*cvItr)->m_liuyan;
						strinfo->m_battletype = (*cvItr)->m_battletype;
						CurZhengzhaolingVec->push_back(strinfo);
						break;
					}
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJYingZhengCanvasTurnPageMethod
//应征窗口翻页
//////////////////////////////////////////////////////////////////////////
crJXJYingZhengCanvasTurnPageMethod::crJXJYingZhengCanvasTurnPageMethod():
	m_this(NULL)
{

}
crJXJYingZhengCanvasTurnPageMethod::crJXJYingZhengCanvasTurnPageMethod(const crJXJYingZhengCanvasTurnPageMethod &handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_mark(handle.m_mark),
	m_index(handle.m_index)
{
}
void crJXJYingZhengCanvasTurnPageMethod::inputParam(int i, void *param)
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
void crJXJYingZhengCanvasTurnPageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_mark = str;
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJYingZhengCanvasTurnPageMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(m_this && playerGameData && canvas.valid())
	{
		ref_ptr<crMultiSwitch > mark = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mark));
		unsigned char type = 0;
		if (mark.valid())
		{
			if (mark->getActiveSwitchSet() == 1)
			{
				type = 1; //国家
			}
			else
			{
				type = 2; //军团
			}
		}
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			// 当前页面显示的征召令
			ZhengzhaolingVec *CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			unsigned char *curPageNum;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJCurZhengzhaolingPage, param);
			if (param)
			{
				curPageNum = (unsigned char *)param;
				switch (m_index)
				{
				case 1: //左翻页
					if ((*curPageNum > 1))
					{
						(*curPageNum) -= 1;
					}
					break;
				case 2: //右翻页
					(*curPageNum) += 1;
				default:
					break;
				}

			}
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = (ZhengzhaolingVec*)param;
			CurZhengzhaolingVec->clear(); //只存一组，存入之前清空
			CRCore::ref_ptr<strZhengzhaoInfo> strinfo = new strZhengzhaoInfo;
			if (type == 1) //国家
			{
				// 国家征召令缓存列表
				ZhengzhaolingVec *guanyuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
				guanyuanlingVec = (ZhengzhaolingVec*)param;
				int i = 0;
				for (cvItr = guanyuanlingVec->begin(); cvItr != guanyuanlingVec->end(); ++ cvItr)
				{
					if (i >= *curPageNum)
					{
						if (cvItr->valid() && (*cvItr)->m_timer > 0)
						{
							strinfo->m_type = (*cvItr)->m_type;
							strinfo->m_publisher = (*cvItr)->m_publisher;
							strinfo->m_chengchiName =(*cvItr)->m_chengchiName;
							strinfo->m_battleroomid = (*cvItr)->m_battleroomid;
							strinfo->m_sceneid = (*cvItr)->m_sceneid;
							strinfo->m_timer = (*cvItr)->m_timer;
							strinfo->m_liuyan = (*cvItr)->m_liuyan;
							strinfo->m_battletype = (*cvItr)->m_battletype;
							CurZhengzhaolingVec->push_back(strinfo);
							break;
						}
					}
					++i;
				}
			}
			if (type == 2)
			{
				// 军团征召令缓存列表
				ZhengzhaolingVec *juntuanlingVec;
				ZhengzhaolingVec::iterator cvItr;
				data->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
				juntuanlingVec = (ZhengzhaolingVec*)param;
				int i = 1; //初始化为第一页
				for (cvItr = juntuanlingVec->begin(); cvItr != juntuanlingVec->end(); ++ cvItr)
				{
					if (i >= *curPageNum)
					{
						if (cvItr->valid() && (*cvItr)->m_timer > 0)
						{
							strinfo->m_type = (*cvItr)->m_type;
							strinfo->m_publisher = (*cvItr)->m_publisher;
							strinfo->m_chengchiName =(*cvItr)->m_chengchiName;
							strinfo->m_battleroomid = (*cvItr)->m_battleroomid;
							strinfo->m_sceneid = (*cvItr)->m_sceneid;
							strinfo->m_timer = (*cvItr)->m_timer;
							strinfo->m_liuyan = (*cvItr)->m_liuyan;
							strinfo->m_battletype = (*cvItr)->m_battletype;
							CurZhengzhaolingVec->push_back(strinfo);
							break;
						}
					}
					++i;
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJClearSameBattleInfoMethod
//清除征召令缓存列表同一战场的邀请信息(点出征时执行)
//////////////////////////////////////////////////////////////////////////
crJXJClearSameBattleInfoMethod::crJXJClearSameBattleInfoMethod():
	m_this(NULL)
{

}
crJXJClearSameBattleInfoMethod::crJXJClearSameBattleInfoMethod(const crJXJClearSameBattleInfoMethod &handle):
	crMethod(handle)
{
}
void crJXJClearSameBattleInfoMethod::inputParam(int i, void *param)
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
void crJXJClearSameBattleInfoMethod::addParam(int i, const std::string& str)
{

}
void crJXJClearSameBattleInfoMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			// 当前页面显示的征召令
			_crInt64 battleroomid;
			ZhengzhaolingVec *CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));

			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = (ZhengzhaolingVec*)param;
			cvItr = CurZhengzhaolingVec->begin();
			if (cvItr != CurZhengzhaolingVec->end() && cvItr->valid())
			{
				battleroomid = (*cvItr)->m_battleroomid;
			}

			// 清除国家征召令缓存列表同一战场的邀请信息
			ZhengzhaolingVec *guanyuanlingVec;
			data->getParam(WCHDATA_JXJGuoZhanZhengzhaolingVec,param);
			guanyuanlingVec = (ZhengzhaolingVec*)param;
			for (cvItr = guanyuanlingVec->begin(); cvItr != guanyuanlingVec->end();)
			{
				if (cvItr->valid())
				{
					if (battleroomid == (*cvItr)->m_battleroomid)
					{
						cvItr = guanyuanlingVec->erase(cvItr);
						
					}
					else
					{
						++ cvItr;						
					}
				}
				else
				{
					cvItr = guanyuanlingVec->erase(cvItr);
				}
			}

			// 清除军团征召令缓存列表同一战场的邀请信息
			ZhengzhaolingVec *juntuanlingVec;
			data->getParam(WCHDATA_JXJJunTuanZhengzhaolingVec,param);
			juntuanlingVec = (ZhengzhaolingVec*)param;
			for (cvItr = juntuanlingVec->begin(); cvItr != juntuanlingVec->end();)
			{
				if (cvItr->valid())
				{
					if (battleroomid == (*cvItr)->m_battleroomid)
					{
						cvItr = juntuanlingVec->erase(cvItr);

					}
					else
					{
						++ cvItr;						
					}
				}
				else
				{
					cvItr = juntuanlingVec->erase(cvItr);
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJInputZhengZhaoBattleInfoMethod
//设置应征战场信息
//////////////////////////////////////////////////////////////////////////
crJXJInputZhengZhaoBattleInfoMethod::crJXJInputZhengZhaoBattleInfoMethod():
	m_this(NULL)
{

}
crJXJInputZhengZhaoBattleInfoMethod::crJXJInputZhengZhaoBattleInfoMethod(const crJXJInputZhengZhaoBattleInfoMethod &handle):
	crMethod(handle)
{
}
void crJXJInputZhengZhaoBattleInfoMethod::inputParam(int i, void *param)
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
void crJXJInputZhengZhaoBattleInfoMethod::addParam(int i, const std::string& str)
{

}
void crJXJInputZhengZhaoBattleInfoMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		bool bExit = true;
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			bool yingzhengreward = true;
			unsigned char zhengzhaolingType = 0;
			std::string releaseName;
			_crInt64 battleroomid = 0;
			int sceneid = 0;
			unsigned char battletype = 0;
			// 当前页面显示的征召令
			ZhengzhaolingVec CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = *(ZhengzhaolingVec*)param;
			cvItr = CurZhengzhaolingVec.begin();
			if (cvItr != CurZhengzhaolingVec.end() && cvItr->valid() && (*cvItr)->m_timer > 0)
			{
				zhengzhaolingType = (*cvItr)->m_type;
				battleroomid = (*cvItr)->m_battleroomid;
				sceneid = (*cvItr)->m_sceneid;
				battletype = (*cvItr)->m_battletype;
				releaseName = (*cvItr)->m_publisher;
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			_crInt32 battleid = LOINT64(battleroomid);
			int roomid = HIINT64(battleroomid);
			unsigned short fubenid = HIINT32(battleid);
			if (FT_WorldFuben != battletype)
			{
				bExit = false;
			}
			data->inputParam(WCHDATA_JXJFubenOrChengchiType,&battletype);
			data->inputParam(WCHDATA_JXJFubenOrChengchiID,&fubenid);
			data->inputParam(WCHDATA_JXJBattleRoomID, &battleroomid);
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr4");
			if(canvas.valid())
			{	
				crData *canvasData = canvas->getDataClass();
				canvasData->inputParam(WCHDATA_JXJEnterSceneID,&sceneid);
			}
			if (battletype == FT_WorldFuben)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1 + releaseName.size());
					stream->_writeUChar(zhengzhaolingType);
					stream->_writeString(releaseName);
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJYingzhengReward,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
				yingzhengreward = false;
			}
			data->inputParam(WCHDATA_JXJCheckYingzhengReward, &yingzhengreward);
		}
		handle.outputParam(0,&bExit);
	}
}
/////////////////////////////////////////
//
//crJXJYingZhengEnterBattleDequeMethod
//
/////////////////////////////////////////
crJXJYingZhengEnterBattleDequeMethod::crJXJYingZhengEnterBattleDequeMethod():
	m_this(NULL)
{
}
crJXJYingZhengEnterBattleDequeMethod::crJXJYingZhengEnterBattleDequeMethod(const crJXJYingZhengEnterBattleDequeMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJYingZhengEnterBattleDequeMethod::inputParam(int i, void *param)
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
	}
}
void crJXJYingZhengEnterBattleDequeMethod::addParam(int i, const std::string& str)
{
}
void crJXJYingZhengEnterBattleDequeMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		unsigned short chengchiid = *(unsigned short *)param;
		if(type == FT_Chengchi && chengchiid>0)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(3);
			stream->_writeUShort(chengchiid);
			stream->_writeUChar(0);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvEnterBattleDeque,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJCheckYingzhengRewardMethod
//检查是否发放应征奖励
//////////////////////////////////////////////////////////////////////////
crJXJCheckYingzhengRewardMethod::crJXJCheckYingzhengRewardMethod():
	m_this(NULL)
{

}
crJXJCheckYingzhengRewardMethod::crJXJCheckYingzhengRewardMethod(const crJXJCheckYingzhengRewardMethod &handle):
	crMethod(handle)
{
}
void crJXJCheckYingzhengRewardMethod::inputParam(int i, void *param)
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
void crJXJCheckYingzhengRewardMethod::addParam(int i, const std::string& str)
{

}
void crJXJCheckYingzhengRewardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			void *param;
			bool yingzhengreward = false;
			bool execute = false;
			unsigned char zhengzhaolingType = 0;
			std::string releaseName;
			_crInt64 battleroomid = 0;
			// 当前页面显示的征召令
			ZhengzhaolingVec CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			if (param)
			{
				CurZhengzhaolingVec = *(ZhengzhaolingVec*)param;
			}
			if (CurZhengzhaolingVec.size() > 0)
			{
				cvItr = CurZhengzhaolingVec.begin();
				if (cvItr != CurZhengzhaolingVec.end() && cvItr->valid() && (*cvItr)->m_timer > 0)
				{
					zhengzhaolingType = (*cvItr)->m_type;
					battleroomid = (*cvItr)->m_battleroomid;
					releaseName = (*cvItr)->m_publisher;
				}
				_crInt32 battleid = LOINT64(battleroomid);
				int roomid = HIINT64(battleroomid);
				unsigned short fubenid = HIINT32(battleid);

				data->getParam(WCHDATA_JXJCheckYingzhengReward, param);
				yingzhengreward = *(bool *)param;
				if (yingzhengreward)
				{
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					if(netConductor)
					{
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1 + releaseName.size());
						stream->_writeUChar(zhengzhaolingType);
						stream->_writeString(releaseName);
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJYingzhengReward,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
						execute = true;
					}
				}
				yingzhengreward = false;
			}
			if (!execute)
			{//清除应征广播信息
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					releaseName = "";
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1 + releaseName.size());
					stream->_writeUChar(0);
					stream->_writeString(releaseName);
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJYingzhengReward,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
					execute = true;
				}
			}
			data->inputParam(WCHDATA_JXJCheckYingzhengReward, &yingzhengreward);
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJYingzhengRewardMethod
//
//////////////////////////////////////////////////////////////////////////
JXJ::crJXJYingzhengRewardMethod::crJXJYingzhengRewardMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJYingzhengRewardMethod::crJXJYingzhengRewardMethod( const crJXJYingzhengRewardMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJYingzhengRewardMethod::inputParam( int i, void *param )
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJYingzhengRewardMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJYingzhengRewardMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			unsigned char type = m_stream->_readUChar();
			std::string m_releaseName = m_stream->_readString();

			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (data && playerData.valid())
			{
				void* param;
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJAcceptZhengZhaoEnterBroadcast,param);
				std::string *releaseName = (std::string *)param;
				*releaseName = m_releaseName;
				if (type != 0)
				{//当type为0时，只需要把广播应征信息清空，不用发放奖励
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char vipLevel = *( unsigned char*)param;
					data -> getParam(WCHDATA_JXJCountryContribute,param);
					int *mycountrycontribute = ( int *)param;
					int gain = 0;
					if (type == 1)
					{
						gain = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGuojiachubingReward,vipLevel).c_str());
					}
					if (type == 3)
					{
						gain = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJuntuanchubingReward,vipLevel).c_str());
					}
					INTLIMITADD(*mycountrycontribute,gain,INT_MAX);
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(*mycountrycontribute);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJYingzhengReward,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			int countrycontri = m_stream->_readInt();
			data->getParam(WCHDATA_JXJCountryContribute,param);
			int extra = countrycontri-(*(int *)param);
			if(extra!=0)
			{
				data->inputParam(WCHDATA_JXJCountryContribute,&countrycontri);
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Contribute,extra));
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJShowYingZhengTimerMethod
//////////////////////////////////////////////////////////////////////////
crJXJShowYingZhengTimerMethod::crJXJShowYingZhengTimerMethod():
	m_this(NULL)
{

}
crJXJShowYingZhengTimerMethod::crJXJShowYingZhengTimerMethod(const crJXJShowYingZhengTimerMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJShowYingZhengTimerMethod::inputParam(int i, void *param)
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
		m_this = (crStaticTextWidgetNode*)param;
		break;
	}
}
void crJXJShowYingZhengTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJShowYingZhengTimerMethod::operator ()(crHandle &handle)
{
	//bool bExit = true;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Zhengzhao_Yingzheng");
	if(m_this && playerGameData)
	{
		crData* data = playerGameData->getDataClass();
		if (data)
		{
			m_this->setAlignment(CRText::crText::CENTER_BASE_LINE);
			void *param;
			unsigned short timer = 0;
			// 当前页面显示的征召令
			ZhengzhaolingVec CurZhengzhaolingVec;
			ZhengzhaolingVec::iterator cvItr;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJCurZhengzhaolingVec,param);
			CurZhengzhaolingVec = *(ZhengzhaolingVec*)param;
			cvItr = CurZhengzhaolingVec.begin();
			if (cvItr != CurZhengzhaolingVec.end() && cvItr->valid() && (*cvItr)->m_timer > 0)
			{
				timer = (*cvItr)->m_timer;
			}
			char str[4];
			sprintf(str,"%02d\0",timer);
			m_this->setString(string(str));
			if(timer<=0)
			{
				if (canvas.valid() && canvas->getVisiable())
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(), false);
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
	//handle.outputParam(0,&bExit);
}
/////////////////////////////////////////
//
//crJXJCheckOrangeSuitEquipMethod
//橙色套装成就（传奇神兵）
/////////////////////////////////////////
crJXJCheckOrangeSuitEquipMethod::crJXJCheckOrangeSuitEquipMethod():
	m_this(NULL),
	m_player(NULL),
	m_equipcolor(0)
{
}
crJXJCheckOrangeSuitEquipMethod::crJXJCheckOrangeSuitEquipMethod(const crJXJCheckOrangeSuitEquipMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_equipcolor(handle.m_equipcolor)
{
}
void crJXJCheckOrangeSuitEquipMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_player = NULL;
		}
		break;
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_player = (crPlayerGameData *)(LOCREPARAM(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJCheckOrangeSuitEquipMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_itemtype = atoi(str.c_str());
		break;
	case 1:
		m_equipcolor = atoi(str.c_str());
		break;
	}
}
void crJXJCheckOrangeSuitEquipMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crRole *mainrole = m_player->getMainRole();
		crData *roleData = mainrole->getDataClass();
		if (roleData)
		{
			void *param;
			int itemid = 0;
			int passcount = 0;
			int equipcount = 0;
			roleData->getParam(WCHDATA_JXJLordEquipVec,param);
			JXJLordEquipVec lordequipvec = *(JXJLordEquipVec *)param;
			if (m_itemtype == IT_Equip)
			{
				ref_ptr<crTableIO>suitequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipSuitTab);
				crTableIO::StrVec suitequiprecord,srecord2;
				int equipindex = suitequiptab->getTitleIndex("头盔");

				if(suitequiptab->queryOneRecord(equipindex,crArgumentParser::appItoa((lordequipvec)[0].first),suitequiprecord)>=0)
				{//是套装
					int equipid = 0;
					for (int i = 0; i<ET_Count; i++)
					{
						equipid = atoi(suitequiprecord[equipindex + i].c_str());
						if (equipid>0)
						{
							if ((lordequipvec)[i].first == equipid)
							{
								if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((lordequipvec)[i].first),record1) >= 0)
								{
									if (atoi(record1[typeIndex].c_str()) == m_itemtype && atoi(record1[colorIndex].c_str()) == m_equipcolor)
									{
										equipcount++;
										if (equipcount == 4)
										{
											passcount++;
											m_this->setStep(passcount);
											if(m_this->isComplete())
											{
												break;
											}
										}
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchOrangeSuitEquipMethod
//检查橙色套装
/////////////////////////////////////////
crJXJSearchOrangeSuitEquipMethod::crJXJSearchOrangeSuitEquipMethod():
	m_this(NULL),
	m_equipcolor(0)
{
}
crJXJSearchOrangeSuitEquipMethod::crJXJSearchOrangeSuitEquipMethod(const crJXJSearchOrangeSuitEquipMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_equipcolor(handle.m_equipcolor)
{
}
void crJXJSearchOrangeSuitEquipMethod::inputParam(int i, void *param)
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
		m_this = (crJXJAccomplish*)param;
		break;
	}
}
void crJXJSearchOrangeSuitEquipMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_itemtype = atoi(str.c_str());
		break;
	case 1:
		m_equipcolor = atoi(str.c_str());
		break;
	}
}
void crJXJSearchOrangeSuitEquipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *m_player = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_player && !m_this->getComplete())
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crRole *mainrole = m_player->getMainRole();
		crData *roleData = mainrole->getDataClass();
		if (roleData)
		{
			void *param;
			int itemid = 0;
			int passcount = 0;
			int equipcount = 0;
			roleData->getParam(WCHDATA_JXJLordEquipVec,param);
			JXJLordEquipVec lordequipvec = *(JXJLordEquipVec *)param;
			if (m_itemtype == IT_Equip)
			{
				ref_ptr<crTableIO>suitequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipSuitTab);
				crTableIO::StrVec suitequiprecord,srecord2;
				int equipindex = suitequiptab->getTitleIndex("头盔");

				if(suitequiptab->queryOneRecord(equipindex,crArgumentParser::appItoa((lordequipvec)[0].first),suitequiprecord)>=0)
				{//是套装
					int equipid = 0;
					for (int i = 0; i<ET_Count; i++)
					{
						equipid = atoi(suitequiprecord[equipindex + i].c_str());
						if (equipid>0)
						{
							if ((lordequipvec)[i].first == equipid)
							{
								if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((lordequipvec)[i].first),record1) >= 0)
								{
									if (atoi(record1[typeIndex].c_str()) == m_itemtype && atoi(record1[colorIndex].c_str()) == m_equipcolor)
									{
										equipcount++;
										if (equipcount == 4)
										{
											passcount++;
											m_this->setStep(passcount);
											if(m_this->isComplete())
											{
												break;
											}
										}
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJSetGuoZhanVictoryTimesMethod
//记录国战胜利次数（分为攻城胜利和守城胜利）
//////////////////////////////////////////////////////////////////////////
JXJ::crJXJSetGuoZhanVictoryTimesMethod::crJXJSetGuoZhanVictoryTimesMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJSetGuoZhanVictoryTimesMethod::crJXJSetGuoZhanVictoryTimesMethod( const crJXJSetGuoZhanVictoryTimesMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJSetGuoZhanVictoryTimesMethod::inputParam( int i, void *param )
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJSetGuoZhanVictoryTimesMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJSetGuoZhanVictoryTimesMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		//if(m_netType == GameServer)
		//{
		//	int playerid = m_this->getPlayerID();
		//	unsigned char type = m_stream->_readUChar();

		//	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		//	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		//	crData *data = playerData->getPlayerGameData()->getDataClass();
		//	if (data && playerData.valid() && gameServer)
		//	{
		//		void* param;
		//		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		//		if (type == 1)
		//		{
		//			data->getParam(WCHDATA_JXJGuoZhanAttackVictoryTimes,param);
		//			unsigned char *attackVictoryTimes = (unsigned char *)param;
		//			if (*attackVictoryTimes < 101)
		//			{
		//				*attackVictoryTimes += 1;
		//			}
		//		}
		//		if (type == 2)
		//		{
		//			data->getParam(WCHDATA_JXJGuoZhanDefendVictoryTimes,param);
		//			unsigned char *defendVictoryTimes = (unsigned char *)param;
		//			if (*defendVictoryTimes < 101)
		//			{
		//				*defendVictoryTimes += 1;
		//			}
		//		}
		//		data->excHandle(MAKECREPARAM(WCH_LockData,0));
		//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//		stream->createBuf(1);
		//		stream->_writeUChar(type);
		//		crPlayerDataEventPacket packet;
		//		crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJSetGuoZhanVictoryTimes,stream.get());
		//		gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		//	}
		//}
		if(m_netType == GameClient_Game)
		{
			crData *data = m_this->getDataClass();
			unsigned char type = m_stream->_readUChar();
			void* param;
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			if (type == 1)
			{
				data->getParam(WCHDATA_JXJGuoZhanAttackVictoryTimes,param);
				unsigned char *attackVictoryTimes = (unsigned char *)param;
				if (*attackVictoryTimes < UCHAR_MAX)
				{
					*attackVictoryTimes += 1;
				}
			}
			if (type == 2)
			{
				data->getParam(WCHDATA_JXJGuoZhanDefendVictoryTimes,param);
				unsigned char *defendVictoryTimes = (unsigned char *)param;
				if (*defendVictoryTimes < UCHAR_MAX)
				{
					*defendVictoryTimes += 1;
				}
			}
			data->getParam(WCHDATA_JXJCountryBattleTimes,param);
			unsigned short *countryBattleTimes = (unsigned short *)param;
			if (*countryBattleTimes < USHRT_MAX)
			{
				*countryBattleTimes += 1;
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
////////////////////////////////////////////////
//
//crJXJCheckGuoZhanAttackVictoryTimesMethod
//国战攻城胜利次数
///////////////////////////////////////////////
crJXJCheckGuoZhanAttackVictoryTimesMethod::crJXJCheckGuoZhanAttackVictoryTimesMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJCheckGuoZhanAttackVictoryTimesMethod::crJXJCheckGuoZhanAttackVictoryTimesMethod(const crJXJCheckGuoZhanAttackVictoryTimesMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJCheckGuoZhanAttackVictoryTimesMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_player = NULL;
		}
		break;
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_player = (crPlayerGameData *)(LOCREPARAM(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJCheckGuoZhanAttackVictoryTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckGuoZhanAttackVictoryTimesMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		crData *data = m_player->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJGuoZhanAttackVictoryTimes,param);
		unsigned char attackVictoryTimes = *(unsigned char *)param;
		if (/*attackVictoryTimes < 101 &&*/!m_this->isComplete())
		{
			m_this->setStep(attackVictoryTimes);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchGuoZhanAttackVictoryTimesMethod
//
/////////////////////////////////////////
crJXJSearchGuoZhanAttackVictoryTimesMethod::crJXJSearchGuoZhanAttackVictoryTimesMethod():
	m_this(NULL)
{
}
crJXJSearchGuoZhanAttackVictoryTimesMethod::crJXJSearchGuoZhanAttackVictoryTimesMethod(const crJXJSearchGuoZhanAttackVictoryTimesMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchGuoZhanAttackVictoryTimesMethod::inputParam(int i, void *param)
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
		m_this = (crJXJAccomplish*)param;
		break;
	}
}
void crJXJSearchGuoZhanAttackVictoryTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchGuoZhanAttackVictoryTimesMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		crData *data = myPlayer->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJGuoZhanAttackVictoryTimes,param);
		unsigned char attackVictoryTimes = *(unsigned char *)param;
		if (/*attackVictoryTimes < 101 && */!m_this->isComplete())
		{
			m_this->setStep(attackVictoryTimes);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
////////////////////////////////////////////////
//
//crJXJCheckGuoZhanDefendVictoryTimesMethod
//国战守城胜利次数
///////////////////////////////////////////////
crJXJCheckGuoZhanDefendVictoryTimesMethod::crJXJCheckGuoZhanDefendVictoryTimesMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJCheckGuoZhanDefendVictoryTimesMethod::crJXJCheckGuoZhanDefendVictoryTimesMethod(const crJXJCheckGuoZhanDefendVictoryTimesMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJCheckGuoZhanDefendVictoryTimesMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_player = NULL;
		}
		break;
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_player = (crPlayerGameData *)(LOCREPARAM(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJCheckGuoZhanDefendVictoryTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckGuoZhanDefendVictoryTimesMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		crData *data = m_player->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJGuoZhanDefendVictoryTimes,param);
		unsigned char defendVictoryTimes = *(unsigned char *)param;
		if (/*defendVictoryTimes < 101 && */!m_this->isComplete())
		{
			m_this->setStep(defendVictoryTimes);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchGuoZhanDefendVictoryTimesMethod
//
/////////////////////////////////////////
crJXJSearchGuoZhanDefendVictoryTimesMethod::crJXJSearchGuoZhanDefendVictoryTimesMethod():
	m_this(NULL)
{
}
crJXJSearchGuoZhanDefendVictoryTimesMethod::crJXJSearchGuoZhanDefendVictoryTimesMethod(const crJXJSearchGuoZhanDefendVictoryTimesMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchGuoZhanDefendVictoryTimesMethod::inputParam(int i, void *param)
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
		m_this = (crJXJAccomplish*)param;
		break;
	}
}
void crJXJSearchGuoZhanDefendVictoryTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchGuoZhanDefendVictoryTimesMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		crData *data = myPlayer->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJGuoZhanDefendVictoryTimes,param);
		unsigned char defendVictoryTimes = *(unsigned char *)param;
		if (/*defendVictoryTimes < 101 && */!m_this->isComplete())
		{
			m_this->setStep(defendVictoryTimes);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJRecvTouchRandomExtraMethod
//触摸获取随机BUFF(不会与上一个随机buf叠加，拾取之前清除上一个随机buf)
/////////////////////////////////////////
crJXJRecvTouchRandomExtraMethod::crJXJRecvTouchRandomExtraMethod():
	m_this(NULL),
	m_range(10)
{}
crJXJRecvTouchRandomExtraMethod::crJXJRecvTouchRandomExtraMethod(const crJXJRecvTouchRandomExtraMethod& handle):
	crMethod(handle),
	m_range(handle.m_range),
	m_idvec(handle.m_idvec)
{
}
void crJXJRecvTouchRandomExtraMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	default:
		break;
	}
}

void crJXJRecvTouchRandomExtraMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_range = atoi(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_idvec);
		break;
	default:
		break;
	}
}

void crJXJRecvTouchRandomExtraMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this && m_this->getDataClass())
	{
		void *param;
		crData *itemData = m_this->getDataClass();
		if(itemData) 
		{
			itemData->excHandle(MAKECREPARAM(WCH_LockData,1));
			itemData->getParam(WCHDATA_ItemState,param);
			unsigned char *itemState = (unsigned char *)param;
			if(*itemState==IS_Dead)
			{
				itemData->excHandle(MAKECREPARAM(WCH_LockData,0));
				return;
			}
			*itemState = IS_Dead;
			itemData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
		int playerid = m_stream->_readInt();
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crNetManager *netManager = sceneServerConductor->getNetManager();
		//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		//CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());

		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		//ref_ptr<crRole> mainRole = playerData->getPlayerGameData()->getMainRole();
		if(playerData.valid() && playerData->getSightInfo() /*&& mainRole.valid() && mainRole->getDataClass()*/)
		{
			ref_ptr<crSightInfo> hitSight = playerData->getSightInfo();
			std::vector< ref_ptr<crInstanceItem> > ItemVec;
			crVector3 randomExtraPos = m_this->getPosition();
			crVector3 itempos;
			ref_ptr<crInstanceItem> item;
			unsigned int guisestate = GS_Normal;
			unsigned char itemstate;
			crData *itemdata;
			//hitSight->lockItemEyeMap();
			//crSightInfo::ItemEyeMap &itemEyeMap = hitSight->getItemEyeMap();
			//for( crSightInfo::ItemEyeMap::iterator itr = itemEyeMap.begin();
			//	itr != itemEyeMap.end();
			//	++itr )
			//{
			//	item = itr->second;
			//	itemdata = item->getDataClass();
			//	if (!itemdata) continue;
			//	itemdata->getParam(WCHDATA_ItemState,param);
			//	itemstate = *(unsigned char *)param;
			//	if(itemstate == IS_Dead)
			//		continue;
			//	guisestate = GS_Normal;
			//	item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			//	if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable
			//		||guisestate & GS_Stagnate || guisestate & GS_immunoSkill )
			//	{
			//		continue;
			//	}
			//	itempos = item->getPosition();
			//	if((itempos - randomExtraPos).length() < m_range)
			//	{
			//		ItemVec.push_back(item);
			//	}
			//}
			//hitSight->unlockItemEyeMap();

			ref_ptr<crSceneServerPlayerData> playerdata;
			hitSight->lockPlayerEyeMap();
			crSightInfo::PlayerEyeMap &playerEyeMap = hitSight->getPlayerEyeMap();
			for( crSightInfo::PlayerEyeMap::iterator itr = playerEyeMap.begin();
				itr != playerEyeMap.end();
				++itr )
			{
				playerdata = itr->second;
				crSceneServerPlayerData::RoleMap &roleMap = playerdata->getRoleMap();
				for( crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
					ritr != roleMap.end();
					++ritr )
				{
					item = dynamic_cast<crInstanceItem *>(ritr->second.get());
					itemdata = item->getDataClass();
					if (!itemdata) continue;
					itemdata->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate == IS_Dead)
						continue;
					guisestate = GS_Normal;
					item->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
					if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable ||
						guisestate & GS_Stagnate || guisestate & GS_immunoSkill )
					{
						continue;
					}
					itempos = item->getPosition();
					if((itempos - randomExtraPos).length() < m_range)
					{
						ItemVec.push_back(item);
					}
				}
			}
			hitSight->unlockPlayerEyeMap();

			//随机BUFFid
			int count = 0;
			int itemId = m_this->getInstanceItemID();
			ref_ptr<crTableIO> RandomBuffTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenRandomBuffTab);;				
			if(RandomBuffTab.valid())
			{
				int chanceValueTotal = 0;
				crTableIO::DataVec recordVec;
				std::vector< int >chanceValueVec;
				RandomBuffTab->queryRecords(0,crArgumentParser::appItoa(itemId),recordVec);
				if(!recordVec.empty())
				{
					int chanceIndex = RandomBuffTab->getTitleIndex("触发概率");
					int bufidIndex = RandomBuffTab->getTitleIndex("BuffId");
					int i,j;
					for (i = 0; i < recordVec.size(); i++)
					{
						chanceValueTotal += atoi(recordVec[i][chanceIndex].c_str());
						chanceValueVec.push_back(chanceValueTotal);
					}
					rangei rndi(0,chanceValueTotal);
					int r = rndi.get_random();
					short extraId = 0;
					for(j = 0; j<recordVec.size(); j++)
					{
						if(r<=chanceValueVec[j])
						{//该物品掉出
							extraId = atoi(recordVec[j][bufidIndex].c_str());
							break;
						}
					}
					if (extraId != 0)
					{
						//清除 之前拾取的随机BUFF
						short old_extraId = 0;
						crData *extraData;
						ExtraIDMap *extraIDMap;
						crPlayerEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeShort(old_extraId);
						for( std::vector< ref_ptr<crInstanceItem> >::iterator itr = ItemVec.begin();
							itr != ItemVec.end();
							++itr )
						{
							itemdata = (*itr)->getDataClass();
							if (!itemdata) continue;
							itemdata->getParam(WCHDATA_JXJOldRandomExtraId,param);
							if (param) old_extraId = *(short *)param;
							itemdata->getParam(WCHDATA_ExtraData,param);
							if(param)
							{
								extraData = (crData *)param;
								extraData->excHandle(MAKECREPARAM(WCH_LockData,1));
								extraData->getParam(WCHDATA_ExtraIDMap,param);
								extraIDMap = (ExtraIDMap *)param;
								if(old_extraId != 0 && extraIDMap->find(old_extraId)!=extraIDMap->end())
								{
									extraData->removeHandle(MAKEINT64(WCH_DoExtra,old_extraId));
									extraIDMap->erase(old_extraId);
									playerid = (*itr)->getID();
									stream->seekBegin();
									stream->_writeShort(old_extraId);
									crPlayerEventPacket::buildRequestPacket(packet,playerid,(*itr).get(),WCH_JXJRecvRemoveExtra,stream.get());
									if (playerData.valid())
									{
										netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
									}
									(*itr)->sendPacketToItemNeighbor(packet);
								}
								extraData->excHandle(MAKECREPARAM(WCH_LockData,0));

								(*itr)->doEvent(WCH_ExtraHandle,MAKECREPARAM(MAKEINT32(extraId,-1),NULL));
								itemdata->inputParam(WCHDATA_JXJOldRandomExtraId, &extraId);
							}
						}
					}
				}
			}
		}
		//客户端
		//if(scene)
		//{
			crItemEventPacket packet;
			crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_ClientRecvTouch,NULL);
			m_this->sendPacketToItemNeighbor(packet);
		//}
	}
}
/////////////////////////////////////////
//
//crJXJCollideItemMethod
//碰撞物体
/////////////////////////////////////////
crJXJCollideItemMethod::crJXJCollideItemMethod():
	m_this(NULL)
{}
crJXJCollideItemMethod::crJXJCollideItemMethod(const crJXJCollideItemMethod& handle):
	crMethod(handle)
{
}
void crJXJCollideItemMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_item = (crInstanceItem *)(LOCREPARAM(param64));
		}
		break;
	}
}
void crJXJCollideItemMethod::addParam(int i, const std::string& str)
{
}
void crJXJCollideItemMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getDataClass() && m_item && m_item->getDataClass())
	{
		void *param;
		crData *Data = m_this->getDataClass();
		if(Data)
		{
			Data->excHandle(MAKECREPARAM(WCH_LockData,1));
			Data->getParam(WCHDATA_ItemState,param);
			if (param)
			{
				unsigned char *itemState = (unsigned char *)param;
				if(*itemState!=IS_Dead)
				{
					crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
					if(myplayer)
					{
						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if(netConductor)
						{
							int id = m_item->getID();
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(9);
							stream->_writeUChar(m_item->getItemtype());
							stream->_writeInt(id);
							if(m_item->getItemtype() == crInstanceItem::Role)
								stream->_writeInt(m_item->getRoleID());
							crPlayerServerEventPacket packet;
							crPlayerServerEventPacket::buildRequestPacket(packet,WCH_JXJRecvRandomExtraCollide,m_this,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
						}
					}
					*itemState = IS_Dead;
				}
			}
			Data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvCollideRandomExtraMethod
//碰撞获取随机BUFF(不会与上一个随机buf叠加，拾取之前清除上一个随机buf)
/////////////////////////////////////////
crJXJRecvCollideRandomExtraMethod::crJXJRecvCollideRandomExtraMethod():
	m_this(NULL)
{}
crJXJRecvCollideRandomExtraMethod::crJXJRecvCollideRandomExtraMethod(const crJXJRecvCollideRandomExtraMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCollideRandomExtraMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	default:
		break;
	}
}

void crJXJRecvCollideRandomExtraMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvCollideRandomExtraMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this->getDataClass())
	{
		void *param;
		crData *itemData = m_this->getDataClass();
		if(itemData) 
		{
			itemData->excHandle(MAKECREPARAM(WCH_LockData,1));
			itemData->getParam(WCHDATA_ItemState,param);
			unsigned char *itemState = (unsigned char *)param;
			if(*itemState==IS_Dead)
			{
				itemData->excHandle(MAKECREPARAM(WCH_LockData,0));
				return;
			}
			*itemState = IS_Dead;
			itemData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}

		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crNetManager *netManager = sceneServerConductor->getNetManager();
		//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		//CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());

		unsigned char fireType = m_stream->_readUChar();
		int id = m_stream->_readInt();
		int roleid = 0;
		if(!m_stream->eof() && fireType == crInstanceItem::Role)
			roleid = m_stream->_readInt();

		ref_ptr<crSceneServerPlayerData> netPlayerData;
		if(fireType == crInstanceItem::Role)
		{
			ref_ptr<crRole> role;
			if(id>0)
			{
				netPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
				if(netPlayerData.valid() && netPlayerData->getRoomID() == m_this->getRoomID())
				{
					role = netPlayerData->getRole(roleid);
				}
			}
			else
			{
				//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				CRNetApp::crScene *scene = m_this->getScene();
				role = dynamic_cast<crRole *>(scene->findSceneItem(id,m_this->getRoomID()));
			}

			if(role.valid() && role->getDataClass())
			{
				int playerid = role->getPlayerID();
				ref_ptr<crRoom> roomofplayer = m_this->getRoom();// netCallback->findRoom(m_this->getRoomID());
				if(roomofplayer.valid() && roomofplayer->getDataClass())
				{
					ref_ptr<crData> roomdata =  roomofplayer->getDataClass();

					roomdata->excHandle(MAKECREPARAM(WCH_LockData,1));
					roomdata->getParam(WCHDATA_JXJBattleStatsMap,param);
					BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;

					unsigned short m_points = 10;
					unsigned char shiliid = 0;
					std::string name;
					BattleStatsMap::iterator itr = battleStatsMap->find(playerid);
					if(itr!=battleStatsMap->end())
					{
						name  = itr->second->getPlayerName();
						itr->second->addPoints(m_points);
						shiliid = itr->second->getShiliID();
					}
					else
					{
						ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServerConductor->getNetDataManager()->getPlayerData(playerid));
						if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
						{
							crPlayerGameData *playerGameData = playerData->getPlayerGameData();
							crData *gameData = playerGameData->getDataClass();
							gameData->getParam(WCHDATA_JXJShiliID,param);
							shiliid = *(unsigned char *)param;

							name = playerData->getCharacterName();
							ref_ptr<crJXJBattleStats> jxjBattleStats = new crJXJBattleStats(playerid,shiliid);
							jxjBattleStats->setCharacterName(playerData->getCharacterName());
							jxjBattleStats->addPoints(m_points);
							(*battleStatsMap)[playerid] = jxjBattleStats.get();

							//itr = battleStatsMap->find(playerid);
						}
					}
					roomdata->excHandle(MAKECREPARAM(WCH_LockData,0));
					
					if(shiliid>=c_startShiliID)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(11+name.size());
						stream->_writeUChar(5);
						stream->_writeShort(m_points);
						stream->_writeInt(playerid);
						stream->_writeString(name);

						ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
						crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvWorldFuBenData,stream.get());
						roomofplayer->sendRoomMessage(packet.get(),shiliid);
					}
					//else
					//{
					//	return;
					//}
				}
				crData *roleData;
				roleData = role->getDataClass();
				//bool hitValid = false;
				//crSightInfo *sightInfo = role->getSightInfo();
				//if(sightInfo)
				//{
					unsigned int guisestate = GS_Normal;
					unsigned char itemstate;
					roleData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate == IS_Dead)
						return;
					guisestate = GS_Normal;
					role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
					if( guisestate & GS_Static || guisestate & GS_StaticUnVisiable || guisestate & GS_StaticNoneBlock || guisestate & GS_UnVisiable ||
						guisestate & GS_Stagnate || guisestate & GS_immunoSkill )
					{
						return;
					}

					//crRole *hitRole = dynamic_cast<crRole *>(m_this);
					//if(hitRole)
					//{
					//	if(hitRole->getPlayerID() == playerid)
					//	{
					//		hitValid = true;
					//	}
					//	else
					//	{
					//		hitValid = sightInfo->isRoleInSight(hitRole)||sightInfo->isEyeRole(hitRole->getPlayerID());
					//	}
					//}
					//else
					//{
					//	hitValid = sightInfo->isItemInSight(m_this)||sightInfo->isEyeItem(m_this->getInstanceItemID());
					//}
				//}
				//if(hitValid)
				{
					//随机BUFFid
					int index = 0;
					int itemId = m_this->getAbstractItemID();
					ref_ptr<crTableIO> RandomBuffTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenRandomBuffTab);;				
					if(RandomBuffTab.valid())
					{
						int chanceValueTotal = 0;
						crTableIO::DataVec recordVec;
						std::vector< int >chanceValueVec;
						RandomBuffTab->queryRecords(0,crArgumentParser::appItoa(itemId),recordVec);
						if(!recordVec.empty())
						{
							int chanceIndex = RandomBuffTab->getTitleIndex("触发概率");
							int bufidIndex = RandomBuffTab->getTitleIndex("BuffId");
							int i,j;
							for (i = 0; i < recordVec.size(); i++)
							{
								chanceValueTotal += atoi(recordVec[i][chanceIndex].c_str());
								chanceValueVec.push_back(chanceValueTotal);
							}
							rangei rndi(0,chanceValueTotal);
							int r = rndi.get_random();
							short extraId = 0;
							for(j = 0; j<recordVec.size(); j++)
							{
								if(r<=chanceValueVec[j])
								{//该物品掉出
									extraId = atoi(recordVec[j][bufidIndex].c_str());
									break;
								}
							}
							if (extraId != 0)
							{
								//清除 之前拾取的随机BUFF
								short old_extraId = 0;
								crData *extraData;
								ExtraIDMap *extraIDMap;
								roleData->getParam(WCHDATA_JXJOldRandomExtraId,param);
								if (param) old_extraId = *(short *)param;
								roleData->getParam(WCHDATA_ExtraData,param);
								if(param)
								{
									extraData = (crData *)param;
									extraData->excHandle(MAKECREPARAM(WCH_LockData,1));
									extraData->getParam(WCHDATA_ExtraIDMap,param);
									extraIDMap = (ExtraIDMap *)param;
									if(old_extraId !=0 && extraIDMap->find(old_extraId)!=extraIDMap->end())
									{
										extraData->removeHandle(MAKEINT64(WCH_DoExtra,old_extraId));
										extraIDMap->erase(old_extraId);
										//stream->seekBegin();
										crPlayerEventPacket packet;
										ref_ptr<crStreamBuf> stream = new crStreamBuf;
										stream->createBuf(2);
										stream->_writeShort(old_extraId);
										crPlayerEventPacket::buildRequestPacket(packet,id,role.get(),WCH_JXJRecvRemoveExtra,stream.get());
										if (netPlayerData.valid())
										{
											netManager->sendPacket(netPlayerData->getPlayerConnectServerAddress(),packet);
										}
										role->sendPacketToItemNeighbor(packet);
									}
									extraData->excHandle(MAKECREPARAM(WCH_LockData,0));

									role->doEvent(WCH_ExtraHandle,MAKECREPARAM(MAKEINT32(extraId,-1),NULL));
									roleData->inputParam(WCHDATA_JXJOldRandomExtraId, &extraId);
								}
							}
						}
					}
				}
				//客户端
				//if(scene)
				//{
					crItemEventPacket packet;
					crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_ClientRecvTouch,NULL); //改变item状态
					m_this->sendPacketToItemNeighbor(packet);
				//}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAcceptZhengZhaoEnterBattleMethod
//玩家通过应征进入战场时发出通告
/////////////////////////////////////////
crJXJAcceptZhengZhaoEnterBattleMethod::crJXJAcceptZhengZhaoEnterBattleMethod()
{}
crJXJAcceptZhengZhaoEnterBattleMethod::crJXJAcceptZhengZhaoEnterBattleMethod(const crJXJAcceptZhengZhaoEnterBattleMethod& handle):
	crMethod(handle),
	m_playerData(handle.m_playerData)
{
}
void crJXJAcceptZhengZhaoEnterBattleMethod::addParam(int i, const std::string& str)
{
}
void crJXJAcceptZhengZhaoEnterBattleMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_playerData = (crSceneServerPlayerData *)LOCREPARAM(param64);
		}
		break;
	}
}
void crJXJAcceptZhengZhaoEnterBattleMethod::operator()(crHandle &handle)
{
	if(m_this && m_playerData.valid())
	{
		void *param;
		if(m_playerData->getPlayerGameData() && m_playerData->getPlayerGameData()->getDataClass())
		{
			unsigned char roomType = 0;
			crData *roomData = m_this->getDataClass();
			if(roomData)
			{
				roomData->getParam(WCHDATA_JXJRoomType,param);
				roomType = *(unsigned char *)param;
			}
			if (roomType == FT_Chengchi || roomType == FT_WorldFuben)
			{
				crPlayerGameData *playerGameData = m_playerData->getPlayerGameData();
				crData *playerData = playerGameData->getDataClass();
				if (playerData)
				{
					playerData->getParam(WCHDATA_JXJAcceptZhengZhaoEnterBroadcast,param);
					std::string releaseName = *(std::string *)param;
					std::string playerName = playerGameData->getCharacterName();
					if (!releaseName.empty() && !playerName.empty())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(releaseName.size() + playerName.size());
						stream->_writeUChar(5); //表示通告类型
						stream->_writeString(releaseName);
						stream->_writeString(playerName);

						//playerData->inputParam(WCHDATA_JXJAcceptZhengZhaoEnterBroadcast, NULL);

						ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
						crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvBattleBrocast,stream.get());
						m_this->sendRoomMessage(packet.get());
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJReliveItemsFromTabExMethod
//根据当前波数加载对应的NPC
/////////////////////////////////////////
crJXJReliveItemsFromTabExMethod::crJXJReliveItemsFromTabExMethod(){}
crJXJReliveItemsFromTabExMethod::crJXJReliveItemsFromTabExMethod(const crJXJReliveItemsFromTabExMethod& handle):
	crMethod(handle),
	m_nameid(handle.m_nameid)
{
}
void crJXJReliveItemsFromTabExMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom*)param;
		break;
	}
}

void crJXJReliveItemsFromTabExMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nameid = str;
		break;
	}
}

void crJXJReliveItemsFromTabExMethod::operator()(crHandle &handle)
{
	int _case = 0;
	int boshu = 0;
	crData *data = m_this->getDataClass();
	void *param;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJScenarioVariableMap,param);
	ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
	ScenarioVariableMap::iterator itr = variableMap->find(m_nameid);
	if(itr != variableMap->end())
	{
		boshu = itr->second;
		_case = boshu % 5;
		if (_case == 0)
		{//NPC类型为BOSS
			_case = 5;
		}
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
	m_this->doEvent(WCH_JXJReliveItemsFromTab,MAKECREPARAM(_case, boshu));
	//if (boshu>10 && boshu % 10 == 1)
	//{
	//	m_this->doEvent(WCH_JXJReliveItemsFromTab, MAKECREPARAM(boshu, 0));
	//}
}
/////////////////////////////////////////////////////
//
//crJXJCheckItemsDeadFromTabExMethod
//判断传入的m_item是否死亡（可同时接收两个值，第二个表示波数）
/////////////////////////////////////////////////////
crJXJCheckItemsDeadFromTabExMethod::crJXJCheckItemsDeadFromTabExMethod()
{}
crJXJCheckItemsDeadFromTabExMethod::crJXJCheckItemsDeadFromTabExMethod(const crJXJCheckItemsDeadFromTabExMethod& handle):
	crMethod(handle),
	m_item(handle.m_item)
{
}
void crJXJCheckItemsDeadFromTabExMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_item = str;
		break;
	default:
		break;
	}
}
void crJXJCheckItemsDeadFromTabExMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJCheckItemsDeadFromTabExMethod::operator()(crHandle &handle)
{
	bool condition = true;
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	CRNetApp::crScene *scene = m_this->getScene();// netCallback->findScene(m_this->getSceneID());
	if(scene)
	{
		crInstanceItem *item;
		crData *itemData;
		int itemValue = 0;
		unsigned char itemstate;
		void *param;
		crData *data = m_this->getDataClass();
		scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJScenarioVariableMap,param);
		ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
		ScenarioVariableMap::iterator newItr = variableMap->find(m_item);
		if(newItr != variableMap->end())
		{
			itemValue = newItr->second;
			int remainder = itemValue % 5;
			if (remainder != 0)
			{
				itemValue = remainder;
			}
			else
			{
				itemValue = 5;
			}
		}
		data->getParam(WCHDATA_JXJReliveItemMap,param);
		ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
		ReliveItemMap::iterator itr = reliveItemMap->find(itemValue);
		for( ;
			itr != reliveItemMap->end() && itr->first == itemValue;
			++itr )
		{
			item = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
			if(item)
			{	
				itemData = item->getDataClass();
				itemData->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				if(IS_Dead != itemstate)
				{
					condition = false;
					break;
				}
			}
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
		scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
	}
	handle.outputParam(0,&condition);
}
/////////////////////////////////////////
//
//crJXJSetWidgetEnableExOpenLvMethod
//根据功能开放等级控制图标显示状态
/////////////////////////////////////////
crJXJSetWidgetEnableExOpenLvMethod::crJXJSetWidgetEnableExOpenLvMethod():
	m_enable(false),
	m_index(0)
{
}
crJXJSetWidgetEnableExOpenLvMethod::crJXJSetWidgetEnableExOpenLvMethod(const crJXJSetWidgetEnableExOpenLvMethod& handle):
	m_canvas(handle.m_canvas),
	m_button(handle.m_button),
	m_enable(handle.m_enable),
	m_index(handle.m_index)
{
}
void crJXJSetWidgetEnableExOpenLvMethod::inputParam(int i, void *param)
{
}
void crJXJSetWidgetEnableExOpenLvMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_button = str;
		break;
	case 2:
		m_enable = (bool)atoi(str.c_str());
		break;
	case 3:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJSetWidgetEnableExOpenLvMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	if(player && player->getDataClass() && player->getMainRole() && canvas.valid())
	{
		if (m_enable != 0)
		{
			bool condition = false;
			void *param;
			crRole *mainrole = player->getMainRole();
			crData *mainroledata = mainrole->getDataClass();
			if(!mainroledata)return;
			mainroledata->getParam(WCHDATA_Level,param);
			unsigned char mylv = *(unsigned char *)param;
			crData *data = player->getDataClass();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(m_index,viplv).c_str());
			if (mylv >= openlv)
			{
				condition = true;
			}
			if (condition)
			{
				ref_ptr<crWidgetNode> targetWidget = canvas->getWidget(m_button);
				if (targetWidget.valid())
				{
					targetWidget->setEnable(m_enable);
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
		else
		{
			ref_ptr<crWidgetNode> targetWidget = canvas->getWidget(m_button);
			if (targetWidget.valid())
			{
				targetWidget->setEnable(m_enable);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSyncScenarioVariableMethod
//sceneServer变量值同步到客户端
/////////////////////////////////////////
crJXJSyncScenarioVariableMethod::crJXJSyncScenarioVariableMethod()
{}
crJXJSyncScenarioVariableMethod::crJXJSyncScenarioVariableMethod(const crJXJSyncScenarioVariableMethod& handle):
	crMethod(handle)
{
}
void crJXJSyncScenarioVariableMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJSyncScenarioVariableMethod::addParam(int i, const std::string& str)
{

}

void crJXJSyncScenarioVariableMethod::operator()(crHandle &handle)
{
	if (m_netType && m_stream.valid())
	{
		std::string m_nameId = m_stream->_readString();
		float m_value = m_stream->_readFloat();
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		crData *roomData = room->getDataClass();
		if (roomData)
		{
			void *param;
			roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
			roomData->getParam(WCHDATA_JXJScenarioVariableMap,param);
			ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
			ScenarioVariableMap::iterator itr = variableMap->find(m_nameId);
			if(itr == variableMap->end())
			{
				(*variableMap)[m_nameId] = m_value;
			}
			else
			{
				itr->second = m_value;
			}
			roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerAutoBattleMethod
//玩家自动战斗
/////////////////////////////////////////
crJXJPlayerAutoBattleMethod::crJXJPlayerAutoBattleMethod():
	m_cancleCameraLock(false),
	m_dt(NULL)
{
	for (int i = 0; i < 3; i++)
	{
		m_movetimer[i] = 5.0f;
	}
}
crJXJPlayerAutoBattleMethod::crJXJPlayerAutoBattleMethod(const crJXJPlayerAutoBattleMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_newCanvas(handle.m_newCanvas),
	m_autoBattleBtn(handle.m_autoBattleBtn),
	m_cameraLockBtn(handle.m_cameraLockBtn),
	m_cancleCameraLock(false),
	m_dt(NULL)
{
	for (int i = 0; i < 3; i++)
	{
		m_movetimer[i] = 5.0f;
	}
}
void crJXJPlayerAutoBattleMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)LOCREPARAM(param64);
		}
		break;
	}
}

void crJXJPlayerAutoBattleMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_newCanvas = str;
		break;
	case 2:
		m_autoBattleBtn = str;
		break;
	case 3:
		m_cameraLockBtn = str;
		break;
	default:
		break;
	}
}

void crJXJPlayerAutoBattleMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && playerGameData->getDataClass())
	{
		crData *playerdata = playerGameData->getDataClass();
		ref_ptr<crCanvasNode> canvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			bool cancleCameraLock = false;
			ref_ptr<crCheckBoxWidgetNode> autoBattleBtn = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getChildNode(m_autoBattleBtn));
			if (autoBattleBtn.valid() && autoBattleBtn->getSelect())
			{
				for (int i=0; i<3; i++)
				{
					m_movetimer[i] += *m_dt;
				}
				bool moved = false;
				void *param;
				bool autoBattle = true;
				unsigned char myVip = 0;
				playerdata->getParam(WCHDATA_JXJVipLv, param);
				myVip = *(unsigned char *)param;
				unsigned char needViplv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAutoBattleUseSkillNeedViplv,myVip).c_str());
				playerdata->inputParam(WCHDATA_JXJChooseAutoBattleMode, &autoBattle);
				crGlobalHandle::gData()->UpdateWorldValue(WCHDATA_Word_AutoSelectTargetMode, 0);
				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				if (room && room->getDataClass())
				{
					crData *roomData = room->getDataClass();
					ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
					if(camera.valid())
					{//选择自动战斗时切换为锁屏状态，（考虑是否在解除自动战斗时取消锁屏?）
						crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(camera->getAttachedNode());
						crVector2i equips = crVector2i(-1,-1);
						crTableIO::StrVec itemrecord;
						ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
						if(!itemtab.valid()) return;
						playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
						FormationInfoVec *myteam = (FormationInfoVec *)param;
						if (!m_cancleCameraLock)
						{
							ref_ptr<crCanvasNode> miniMapCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_newCanvas);
							if (miniMapCanvas.valid())
							{
								ref_ptr<crCheckBoxWidgetNode> cameraLock = dynamic_cast<crCheckBoxWidgetNode *>(miniMapCanvas->getChildNode(m_cameraLockBtn));
								if (cameraLock.valid() && !cameraLock->getSelect())
								{
									cameraLock->select(true);
									m_cancleCameraLock = true;
								}
							}
						}
						playerdata->getParam(WCHDATA_JXJFubenOrChengchiType,param);
						unsigned char type = *(unsigned char *)param;
						float px = 0;
						float py = 0;
						if(type == FT_Chengchi)
						{//城战自动战斗：自动释放技能有VIP等级限制
							roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
							roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
							JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
							roomData->getParam(WCHDATA_JXJBattleID, param);
							int battleid = *(int *)param;
							playerdata->getParam(WCHDATA_JXJShiliID, param);
							unsigned char myShili = *(unsigned char *)param;
							unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
							if (myShili == dshili)
							{
								unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));
								JXJCampBirthpoint::iterator itr_point;
								if(campMap && !(*campMap)[ashili].empty())
								{
									JXJCampBirthpointMap::iterator itr_map = campMap->find(ashili);
									//int judianIndex = 1;
									if (itr_map!=campMap->end())
									{
										itr_point = itr_map->second.begin();
										for (;itr_point!=itr_map->second.end();itr_point++)
										{
											px = itr_point->first.x()*crGlobalHandle::gData()->gUnitScale();
											py = itr_point->first.y()*crGlobalHandle::gData()->gUnitScale();
											break;
										}
									}
								}
							}
							else
							{
								playerdata->getParam(WCHDATA_JXJFubenOrChengchiID,param);
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
							roomData->excHandle(MAKECREPARAM(WCH_LockData, 0));
						}
						if(type == FT_Fuben)
						{//副本自动战斗：附加自动释放技能
							playerdata->getParam(WCHDATA_JXJFubenOrChengchiID,param);
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
						int i = 0;
						for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
							itr != myRoles.end();
							++itr,++i )
						{
							role = itr->second.first.get();
							if (role && role->getDataClass())
							{
								unsigned short useResult = UR_None;
								crData *roleData = role->getDataClass();
								int roleid = role->getRoleID();
								roleData->getParam(WCHDATA_ItemState, param);
								unsigned char itemstate = *((unsigned char*)param);
								unsigned int guisestate = GS_Normal;
								role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
								if(itemstate == IS_Dead || guisestate & GS_UnVisiable)
									continue;
								roleData->excHandle(MAKECREPARAM(WCH_LockData, 1));
								//自动战斗状态不能驻守
								roleData->getParam(WCHDATA_GuiseState, param);
								unsigned int *gs = (unsigned int *)param;
								*gs &= ~GS_Garrison;

								roleData->getParam(WCHDATA_RTHP,param);
								float rthp = *(float*)param;
								if (type == FT_Fuben || myVip >= needViplv)
								{//自动施法限制条件
									if( itemstate != IS_Dead && rthp>0.0f && 
										!(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
										!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
										/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
										/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
										!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
										!(guisestate & GS_ExtraShifting)/* && !(guisestate & GS_NoAttack)*/ )
									{//检查无法使用技能的状态
										int abstractID = 0;
										int aboutUseID = 0;
										for (FormationInfoVec::iterator itr = myteam->begin();
											itr != myteam->end();
											++itr)
										{
											if (roleid == (*itr)->getRoleID())
											{
												equips = (*itr)->getEquips();
												break;
											}
										}
										if (equips[0] > 0)
										{
											if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(equips[0]), itemrecord) >= 0)
											{
												abstractID = atoi(itemrecord[itemtab->getTitleIndex("abstractid")].c_str());
											}
										}
										if (abstractID != 0)
										{
											crInstanceItem *enemy = crMyPlayerData::getInstance()->getOneInRangeEnemy(role);
											roleData->getParam(WCHDATA_EquipOnMap, param);
											EquipOnMap *equipOnMap = (EquipOnMap *)param;
											crItemChild *itemChild = role->findChildItem(equipOnMap->find(abstractID)->second);
											if (enemy && itemChild && itemChild->isLoaded())
											{
												unsigned short range = 0;
												crInstanceItem *item = itemChild->getInstanceItem();
												crData *itemData = item->getDataClass();
												aboutUseID = item->getInstanceItemID();
												UseItemParam useItemParam;
												useItemParam.m_user = role;
												useItemParam.m_target = NULL;
												item->doEvent(WCH_UseItemCheck, MAKECREPARAM(&useItemParam, &useResult));
												if (useResult == UR_Succeed)
												{
													useResult = UR_None;
													crVector3 rolepos = role->getPosition();
													ref_ptr<crTableIO> skillDataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
													int effectIndex = skillDataAttrTab->getTitleIndex("作用类型");
													crTableIO::StrVec record;
													if (skillDataAttrTab->queryOneRecord(0, crArgumentParser::appItoa(abstractID), record) >= 0)
													{
														int effectTarget = atoi(record[effectIndex].c_str());
														itemData->getParam(WCHDATA_ItemUseRange, param);
														if (param)
														{
															unsigned char targetType = effectTarget;
															unsigned char outTargetType;
															range = *(unsigned short *)param;
															if (range == 0)
															{
																itemData->getParam(WCHDATA_ItemUseScope, param);
																short scope = *(short *)param;
																crInstanceItem *targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(rolepos, (float)scope * crGlobalHandle::gData()->gUnitScale(), targetType, outTargetType);
																if (targetItem)
																{
																	//直接释放技能
																	crVector3 pos;
																	crMatrixTransform *bot;
																	bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
																	if (bot && bot->getVisiable())
																	{
																		pos = role->getPosition() + role->getDir() * 0.5f;
																		unsigned char itemstate = IS_UseItem;
																		roleData->inputParam(WCHDATA_ItemState, &itemstate);
																		unsigned char targettype = Target_Coord;
																		roleData->inputParam(WCHDATA_TargetType, &targettype);
																		roleData->inputParam(WCHDATA_TargetPos, &pos);
																		roleData->inputParam(WCHDATA_AboutToUseItemID, &aboutUseID);
																		useResult = UR_Succeed;
																	}
																}
															}
															else
															{//指定释放目标
															 //ref_ptr<crInstanceItem> targetItem;
															 //ref_ptr<crMatrixTransform> targetNode;
															 //role->clientGetTarget(targetItem,targetNode);
																crInstanceItem *targetItem = crMyPlayerData::getInstance()->selectTargetInPointRange(rolepos, (float)range * crGlobalHandle::gData()->gUnitScale(), targetType, outTargetType);
																if (targetItem)
																{
																	unsigned char targettype = targetItem->getItemtype() == crInstanceItem::Role ? Target_Role : Target_Npc;
																	roleData->getParam(WCHDATA_TargetID, param);
																	int targetid = *(int *)param;
																	roleData->getParam(WCHDATA_TargetRoleID, param);
																	int targetroleid = *(int *)param;
																	if (targetid != 0)
																	{
																		unsigned char targettype = targetroleid == 0 ? Target_Npc : Target_Role;
																		roleData->inputParam(WCHDATA_TargetType, &targettype);
																	}
																	else
																	{
																		roleData->inputParam(WCHDATA_TargetType, &targettype);
																		targetid = targetItem->getID();
																		roleData->inputParam(WCHDATA_TargetID, &targetid);
																	}
																	roleData->inputParam(WCHDATA_TargetNode, targetItem->getRelNode());
																	roleData->getParam(WCHDATA_TargetRoleID, param);
																	if (targetroleid == 0)
																	{
																		targetroleid = targetItem->getRoleID();
																		roleData->inputParam(WCHDATA_TargetRoleID, &targetroleid);
																	}
																	crVector3 targetPos = targetItem->getPosition();
																	roleData->inputParam(WCHDATA_TargetPos, &targetPos);
																	//crData *targetData = targetItem->getDataClass();
																	//if(targetData)
																	//{
																	//	targetData->getParam(WCHDATA_ItemState,param);
																	//	unsigned char targetItemState = *(unsigned char *)param;
																	//	if(targetItemState == IS_Dead)
																	//	{//目标死亡
																	//		roleData->excHandle(MAKECREPARAM(WCH_LockData, 0));
																	//		continue;
																	//	}
																	//}
																	unsigned char itemstate = IS_UseItem;
																	roleData->inputParam(WCHDATA_ItemState, &itemstate);
																	roleData->inputParam(WCHDATA_AboutToUseItemID, &aboutUseID);
																	useResult = UR_Succeed;
																}
															}
														}
													}
												}
											}
										}
									}
								}
								roleData->excHandle(MAKECREPARAM(WCH_LockData, 0));
								if (useResult != UR_Succeed)
								{//未施法状态
									bool templateCancleMove = false; 
									roleData->getParam(WCHDATA_TargetType,param);
									unsigned char newTargettype = *(unsigned char *)param;
									if (newTargettype & Target_EnemyAll)
									{//消灭当前敌对目标，临时取消前往政厅或BOSS处
										templateCancleMove = true;
									}
									if (!templateCancleMove && i < 3 && m_movetimer[i]>5.0f)
									{//前往政厅或BOSS处
										m_movetimer[i] = 0.0f;
										unsigned char itemstate = IS_AttackToPos;
										unsigned char targettype = Target_Coord;
										roleData->inputParam(WCHDATA_ItemState,&itemstate);
										roleData->inputParam(WCHDATA_TargetType,&targettype);
										roleData->inputParam(WCHDATA_TargetPos,&m_targetPosition);
										roleData->inputParam(WCHDATA_MoveToPos,&m_targetPosition);
										crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(role);
									}
								}
							}
						}
						crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
					}
				}
			}
			else if (!autoBattleBtn->getSelect())
			{
				bool autoBattle = false;
				playerdata->inputParam(WCHDATA_JXJChooseAutoBattleMode, &autoBattle);
				if (m_cancleCameraLock)
				{
					ref_ptr<crCanvasNode> miniMapCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_newCanvas);
					if (miniMapCanvas.valid())
					{//取消自动战斗的同时，取消锁屏
						ref_ptr<crCheckBoxWidgetNode> cameraLock = dynamic_cast<crCheckBoxWidgetNode *>(miniMapCanvas->getChildNode(m_cameraLockBtn));
						if (cameraLock.valid())
						{
							cameraLock->select(false);
							m_cancleCameraLock = false;
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJStartAutoBattleMethod
//通过快捷方式（ctrl+A）启用自动战斗
/////////////////////////////////////////
crJXJStartAutoBattleMethod::crJXJStartAutoBattleMethod():
	m_this(NULL),
	m_ea(NULL),
	m_key(82)
{}
crJXJStartAutoBattleMethod::crJXJStartAutoBattleMethod(const crJXJStartAutoBattleMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_autoBattleBtn(handle.m_autoBattleBtn),
	m_ea(NULL),
	m_key(handle.m_key)
{
}
void crJXJStartAutoBattleMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOCREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crJXJStartAutoBattleMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_autoBattleBtn = str;
		break;
	case 2:
		m_key = char(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJStartAutoBattleMethod::operator()(crHandle &handle)
{
	bool cancle = false;
	if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode> autoBattleBtn = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getChildNode(m_autoBattleBtn));
			if (autoBattleBtn.valid() && autoBattleBtn->getEnable())
			{
				bool result = !autoBattleBtn->getSelect();
				autoBattleBtn->select(result);
				cancle = true;
			}
		}
	}
	handle.outputParam(0,&cancle);
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJShowAutoBattleCanvasMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJShowAutoBattleCanvasMethod::crJXJShowAutoBattleCanvasMethod()
{

}
crJXJShowAutoBattleCanvasMethod::crJXJShowAutoBattleCanvasMethod(const crJXJShowAutoBattleCanvasMethod &handle):
	crMethod(handle),
	m_noticeCanvas(handle.m_noticeCanvas),
	m_showCanvas(handle.m_showCanvas),
	m_autoBattleCanvas(handle.m_autoBattleCanvas),
	m_autoBattleBtn(handle.m_autoBattleBtn),
	m_zhenglvCanvas(handle.m_zhenglvCanvas),
	m_fubenCanvas(handle.m_fubenCanvas)
{

}
void crJXJShowAutoBattleCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJShowAutoBattleCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_showCanvas = str;
		break;
	case 2:
		m_autoBattleCanvas = str;
		break;
	case 3:
		m_autoBattleBtn = str;
		break;
	case 4:
		m_zhenglvCanvas = str;
		break;
	case 5:
		m_fubenCanvas = str;
		break;
	default:
		break;
	}
}
void crJXJShowAutoBattleCanvasMethod::operator ()(crHandle &handle)
{
	CRCore::ref_ptr<crCanvasNode>  noticeCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	CRCore::ref_ptr<crCanvasNode>  showCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_showCanvas);
	CRCore::ref_ptr<crCanvasNode>  autoBattleCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_autoBattleCanvas);
	CRCore::ref_ptr<crCanvasNode>  zhenglvCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_zhenglvCanvas);
	if(showCanvas.valid() && noticeCanvas.valid() && autoBattleCanvas.valid())
	{
		ref_ptr<crCheckBoxWidgetNode> autoBattleBtn = dynamic_cast<crCheckBoxWidgetNode *>(autoBattleCanvas->getChildNode(m_autoBattleBtn));
		unsigned char type = FT_Fuben;
		if (autoBattleBtn.valid())
		{
			void *param;
			int noticeId = 0;
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if (myPlayer && myPlayer->getDataClass())
			{
				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				if (room && room->getDataClass())
				{
					crData *roomData = room->getDataClass();
					crData *data = myPlayer->getDataClass();
					unsigned char myVip = 0;
					data->getParam(WCHDATA_JXJVipLv, param);
					myVip = *(unsigned char *)param;
					unsigned char fubenLvLimit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenAutoBattleLimitLv,myVip).c_str());
					data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
					type = *(unsigned char *)param;
					if (type == FT_Fuben)
					{
						//暂时屏蔽副本自动战斗功能
						autoBattleBtn->setEnable(false);
// 						ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
// 						data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
// 						unsigned short fubenid = *(unsigned short *)param;
// 						crTableIO::StrVec record;
// 						if(fubenid>0 && fubentab.valid() && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
// 						{
// 							int needLvIndex = fubentab->getTitleIndex("需求等级");
// 							if (atoi(record[needLvIndex].c_str()) > fubenLvLimit || record[needLvIndex].empty())
// 							{//当副本等级大于15时，关闭自动战斗功能
// 								autoBattleBtn->setEnable(false);
// 							}
// 							else
// 							{
// 								autoBattleBtn->setEnable(true);
// 							}
// 						}
					}
					else if (type == FT_Chengchi)
					{
						autoBattleBtn->setEnable(true);
					}
					else if(type == FT_WorldFuben)
					{
						autoBattleBtn->setEnable(false);
					}
				}
			}
		}
		const crBoundingBox& canvas_box = autoBattleCanvas->getBoundBox();
 		const crBoundingBox& bbox = showCanvas->getBoundBox();
// 		const crBoundingBox& bbox_1 = noticeCanvas->getBoundBox();
// 		autoBattleCanvas->setMatrix(crMatrix::translate(bbox.xMax() - canvas_box.xLength()*0.5f, bbox_1.yMax() - canvas_box.yLength()*0.6f, 0.0f));
		if(type == FT_Chengchi)
		{
			crFilterRenderManager::getInstance()->showCanvas(autoBattleCanvas.get(), true);
		}
		if (zhenglvCanvas.valid() && zhenglvCanvas->getVisiable())
		{
			const crBoundingBox& zhenglvCanvas_box = zhenglvCanvas->getBoundBox();
			if (type == FT_Chengchi)
			{
				zhenglvCanvas->setMatrix(crMatrix::translate(bbox.xMax() - zhenglvCanvas_box.xLength()*0.15f, canvas_box.yMin() - zhenglvCanvas_box.yLength()*0.6f, 0.0f));
			}
			else
			{
				CRCore::ref_ptr<crCanvasNode>  fubenCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_fubenCanvas);
				if (fubenCanvas.valid())
				{
					const crBoundingBox& fubenCanvas_box = fubenCanvas->getBoundBox(); 
					zhenglvCanvas->setMatrix(crMatrix::translate(bbox.xMax() - zhenglvCanvas_box.xLength()*0.15f, fubenCanvas_box.yMin() - zhenglvCanvas_box.yLength()*0.2f, 0.0f));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAutoBattleButtonTipsMethod
//自动战斗提示
/////////////////////////////////////////
crJXJAutoBattleButtonTipsMethod::crJXJAutoBattleButtonTipsMethod()
{
}
crJXJAutoBattleButtonTipsMethod::crJXJAutoBattleButtonTipsMethod(const crJXJAutoBattleButtonTipsMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_headCanvas(handle.m_headCanvas),
	m_title(handle.m_title),
	m_text(handle.m_text),
	m_textbk(handle.m_textbk),
	m_textdi(handle.m_textdi)
{
}
void crJXJAutoBattleButtonTipsMethod::inputParam(int i, void *param)
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
void crJXJAutoBattleButtonTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_headCanvas = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_text = str;
		break;
	case 4:
		m_textbk = str;
		break;
	case 5:
		m_textdi = str;
		break;
	default:
		break;
	}
}
void crJXJAutoBattleButtonTipsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{

		ref_ptr<crCanvasNode> trunkCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
		ref_ptr<crCanvasNode> headerCanvas = crFilterRenderManager::getInstance()->findCanvas(m_headCanvas);
		if (headerCanvas.valid() && trunkCanvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode > title = dynamic_cast<crStaticTextWidgetNode *>(headerCanvas->getWidget(m_title));
			ref_ptr<crHypertextWidgetNode> text = dynamic_cast<crHypertextWidgetNode *>(trunkCanvas->getWidget(m_text));
			if(!text.valid()) return;
			crMatrixTransform *textbk = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textbk));
			if(!textbk) return;
			crMatrixTransform *textdi = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textdi));
			if(!textdi) return;

			void *param;
			int noticeId = 0;
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if (room && room->getDataClass())
			{
				crData *roomData = room->getDataClass();
				crData *data = myPlayer->getDataClass();
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				unsigned char type = *(unsigned char *)param;
				if (type == FT_Fuben)
				{
					ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
					data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
					unsigned short fubenid = *(unsigned short *)param;
					crTableIO::StrVec record;
					if(fubenid>0 && fubentab.valid() && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
					{
						int needLvIndex = fubentab->getTitleIndex("需求等级");
						if (atoi(record[needLvIndex].c_str()) > 15)
						{
							noticeId = 7018;
						}
					}
				}
				if (type == FT_Chengchi)
				{
					roomData->getParam(WCHDATA_JXJBattleID, param);
					int battleid = *(int *)param;
					data->getParam(WCHDATA_JXJShiliID, param);
					unsigned char myShili = *(unsigned char *)param;
					unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
					if (myShili == dshili)
					{
						noticeId = 7020;
					}
					else
					{
						noticeId = 7019;
					}
				}
			}
			std::string titlestr = "自动战斗说明";
			std::string notice;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			if (noticeHandle)
			{
				ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
				crTableIO::StrVec record;
				if(textTab->queryOneRecord(0,crArgumentParser::appItoa(noticeId),record) >= 0)
				{
					notice = record[1];
				}
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content",1);
				cfg_script.Add("Text",notice + " /n");
				cfg_script.Pop();
				cfg_script.Pop();
				text->setHypertext(cfg_script);
			}
			if (title.valid())
			{
				title->setString(titlestr);
			}		
			///////////计算缩放
			float boxYlength = textbk->getChild(0)->getBoundBox().yLength();
			text->updateData();
			const crBoundingBox &bbox = text->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = text->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = trunkCanvas->getBoundBox();
				crVector3 centerTansTar = text->getBound().center();

				float tmpLength = bbox2.yLength();
				if(textdi)
				{
					float diYlength = textdi->getChild(0)->getBoundBox().yLength();
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = textbk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(textbk)textbk->setMatrix(rotmat);

				if(textdi)
				{
					float diYlength = textdi->getChild(0)->getBoundBox().yLength();
					needmove = boxYlength - texheight + diYlength * 0.25f;
					textdi->setMatrix(crMatrix::translate(0.0f,needmove,0.0f));
				}
			}
			else
			{
				textbk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(textdi) textdi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}
			///////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = trunkCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2,posy2;
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
			headerCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			trunkCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			trunkCanvas->setCanFocus(false);
			headerCanvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(headerCanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(trunkCanvas.get(),true);
		}
	}
}
/////////////////////////////////////////
//
//crJXJGameIntroduceCanvasUpdateMethod
//游戏宝典窗口刷新
/////////////////////////////////////////
crJXJGameIntroduceCanvasUpdateMethod::crJXJGameIntroduceCanvasUpdateMethod()
{
}

crJXJGameIntroduceCanvasUpdateMethod::crJXJGameIntroduceCanvasUpdateMethod(const crJXJGameIntroduceCanvasUpdateMethod& handle):
	crMethod(handle),
	m_pageradio(handle.m_pageradio),
	m_pagesw(handle.m_pagesw),
	m_pageName_1(handle.m_pageName_1),
	m_pageName_2(handle.m_pageName_2),
	m_pageName_3(handle.m_pageName_3),
	m_pageName_4(handle.m_pageName_4),
	m_canvas(handle.m_canvas),
	m_checkBoxNoTips(handle.m_checkBoxNoTips)
{
}

void crJXJGameIntroduceCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJGameIntroduceCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pageradio = str;
		break;
	case 1:
		m_pagesw = str;
		break;
	case 2:
		m_pageName_1 = str;
		break;
	case 3:
		m_pageName_2 = str;
		break;
	case 4:
		m_pageName_3 = str;
		break;
	case 5:
		m_pageName_4 = str;
		break;
	case 6:
		m_canvas = str;
		break;
	case 7:
		m_checkBoxNoTips = str;
		break;
	default:
		break;
	}
}

void crJXJGameIntroduceCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_pagesw));
			ref_ptr<crCheckBoxWidgetNode> noTipsWidget = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_checkBoxNoTips));
			ref_ptr<crRadioGroupWidgetNode> pageradio = dynamic_cast<crRadioGroupWidgetNode *>(parentCanvas->getWidget(m_pageradio));
			ref_ptr<crStaticTextWidgetNode> pageName_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_pageName_1));
			ref_ptr<crStaticTextWidgetNode> pageName_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_pageName_2));
			ref_ptr<crStaticTextWidgetNode> pageName_3 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_pageName_3));
			ref_ptr<crStaticTextWidgetNode> pageName_4 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_pageName_4));
			int select = 0;
			crData *pdata = myPlayer->getDataClass();
			if (pdata)
			{
				void *param;
				unsigned char curPageNum = 0;
				pdata->inputParam(WCHDATA_JXJGameIntroductionCurPageNum, &curPageNum);
				pdata->getParam(WCHDATA_JXJGameIntroductionNoTipsCheck, param);
				bool noTips = *(bool *)param;
				if (noTipsWidget.valid())
				{
					noTipsWidget->select(noTips);
				}
			}
			if (pageradio.valid())
			{
				select = pageradio->getSelect();
			}
			if (pagesw)
			{
				pagesw->setActiveSwitchSet(select);
			}
			if (pageName_1.valid())
			{
				pageName_1->setString(L"升级指南");

			}
			if (pageName_2.valid())
			{
				pageName_2->setString(L"提升战斗力");
			}
			if (pageName_3.valid())
			{
				pageName_3->setString(L"国战攻略");
			}
			if (pageName_4.valid())
			{
				pageName_4->setString(L"高手进阶");
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJLevelUpIntroduceCanvasUpdateMethod
//升级指南页面刷新
/////////////////////////////////////////
crJXJLevelUpIntroduceCanvasUpdateMethod::crJXJLevelUpIntroduceCanvasUpdateMethod()
{
}

crJXJLevelUpIntroduceCanvasUpdateMethod::crJXJLevelUpIntroduceCanvasUpdateMethod(const crJXJLevelUpIntroduceCanvasUpdateMethod& handle):
	crMethod(handle),
	m_title(handle.m_title), 
	m_introduction(handle.m_introduction),
	m_image1_sw(handle.m_image1_sw),
	m_image2_sw(handle.m_image2_sw),
	m_image3_sw(handle.m_image3_sw),
	m_image4_sw(handle.m_image4_sw),
	m_pageBtn_R(handle.m_pageBtn_R),
	m_pageBtn_L(handle.m_pageBtn_L),
	m_canvas(handle.m_canvas),
	m_imagePage_R(handle.m_imagePage_R),
	m_imagePage_L(handle.m_imagePage_L)
{
}

void crJXJLevelUpIntroduceCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJLevelUpIntroduceCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_title = str;
		break;
	case 1:
		m_introduction = str;
		break;
	case 2:
		m_image1_sw = str;
		break;
	case 3:
		m_image2_sw = str;
		break;
	case 4:
		m_image3_sw = str;
		break;
	case 5:
		m_image4_sw = str;
		break;
	case 6:
		m_pageBtn_R = str;
		break;
	case 7:
		m_pageBtn_L = str;
		break;
	case 8:
		m_canvas = str;
		break;
	case 9:
		m_imagePage_R = str;
		break;
	case 10:
		m_imagePage_L = str;
		break;
	default:
		break;
	}
}

void crJXJLevelUpIntroduceCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			ref_ptr<crStaticTextWidgetNode> title = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title));
			ref_ptr<crStaticTextWidgetNode> introduction = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_introduction));
			ref_ptr<crMultiSwitch> image1_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image1_sw));
			ref_ptr<crMultiSwitch> image2_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image2_sw));
			ref_ptr<crMultiSwitch> image3_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image3_sw));
			ref_ptr<crMultiSwitch> image4_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image4_sw));
			ref_ptr<crButtonWidgetNode> pageBtn_R = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_pageBtn_R));
			ref_ptr<crButtonWidgetNode> pageBtn_L = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_pageBtn_L));
			ref_ptr<crButtonWidgetNode> imagePageBtn_R = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_imagePage_R));
			ref_ptr<crButtonWidgetNode> imagePageBtn_L = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_imagePage_L));
			unsigned char curPageNum = 0;
			crData *pdata = myPlayer->getDataClass();
			if (pdata)
			{
				void *param;
				pdata->getParam(WCHDATA_JXJGameIntroductionCurPageNum, param);
				if (param)
				{
					curPageNum = *(unsigned char *)param;
				}
			}
			if (title.valid())
			{
				title->setString(L"升级主要途径");
			}
			if (image1_sw.valid())
			{
				image1_sw->setActiveSwitchSet(curPageNum);
			}
			if (image2_sw.valid())
			{
				image2_sw->setActiveSwitchSet(curPageNum);
			}
			if (image3_sw.valid())
			{
				image3_sw->setActiveSwitchSet(curPageNum);
			}
			if (image4_sw.valid())
			{
				image4_sw->setActiveSwitchSet(curPageNum);
			}
			if (pageBtn_L.valid())
			{
				if (0 == curPageNum)
				{
					pageBtn_L->setEnable(false);
				}
				else
				{
					pageBtn_L->setEnable(true);
				}
			}
			if (imagePageBtn_L.valid())
			{
				if (0 == curPageNum)
				{
					imagePageBtn_L->setEnable(false);
				}
				else
				{
					imagePageBtn_L->setEnable(true);
				}
			}
			if (pageBtn_R.valid())
			{
				if (3 == curPageNum)
				{
					pageBtn_R->setEnable(false);
				}
				else
				{
					pageBtn_R->swapEnable(true);
				}
			}
			if (imagePageBtn_R.valid())
			{
				if (3 == curPageNum)
				{
					imagePageBtn_R->setEnable(false);
				}
				else
				{
					imagePageBtn_R->setEnable(true);
				}
			}
			if (introduction.valid())
			{
				std::string str;
				int noticeId = 7021;
				switch (curPageNum)
				{
				case 0:
					noticeId = 7021;
					break;
				case 1:
					noticeId = 7022;
					break;
				case 2:
					noticeId = 7023;
					break;
				case 3:
					noticeId = 7024;
					break;
				default:
					break;
				}
				CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				if (noticeHandle)
				{
					int mode = 2;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&noticeId);
					noticeHandle->inputParam(WCHDATA_NoticeOutPut,&str);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
				introduction->setString(str);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCombatEffectivenessIntroduceCanvasUpdateMethod
//战斗力提升来源页面刷新
/////////////////////////////////////////
crJXJCombatEffectivenessIntroduceCanvasUpdateMethod::crJXJCombatEffectivenessIntroduceCanvasUpdateMethod()
{
}

crJXJCombatEffectivenessIntroduceCanvasUpdateMethod::crJXJCombatEffectivenessIntroduceCanvasUpdateMethod(const crJXJCombatEffectivenessIntroduceCanvasUpdateMethod& handle):
	crMethod(handle),
	m_title(handle.m_title),
	m_childCanvas_sw(handle.m_childCanvas_sw),
	m_canvas(handle.m_canvas)
{
}

void crJXJCombatEffectivenessIntroduceCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJCombatEffectivenessIntroduceCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_title = str;
		break;
	case 1:
		m_childCanvas_sw = str;
		break;
	case 2:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void crJXJCombatEffectivenessIntroduceCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			crMultiSwitch *childCanvas_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_childCanvas_sw));
			ref_ptr<crStaticTextWidgetNode> title = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title));
			int select = 0;
			crData *pdata = myPlayer->getDataClass();
			if (pdata)
			{
				unsigned char curPageNum = 0;
				pdata->inputParam(WCHDATA_JXJGameIntroductionCurPageNum, &curPageNum);
			}
			if (childCanvas_sw)
			{
				childCanvas_sw->setActiveSwitchSet(select);
			}
			if (title.valid())
			{
				title->setString(L"战斗力提升来源");
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCountryBattleIntroduceCanvasUpdateMethod
//国战指南页面刷新
/////////////////////////////////////////
crJXJCountryBattleIntroduceCanvasUpdateMethod::crJXJCountryBattleIntroduceCanvasUpdateMethod()
{
}

crJXJCountryBattleIntroduceCanvasUpdateMethod::crJXJCountryBattleIntroduceCanvasUpdateMethod(const crJXJCountryBattleIntroduceCanvasUpdateMethod& handle):
	crMethod(handle),
	m_title(handle.m_title), 
	m_image_sw(handle.m_image_sw),
	m_pageBtn_R(handle.m_pageBtn_R),
	m_pageBtn_L(handle.m_pageBtn_L),
	m_canvas(handle.m_canvas)
{
}

void crJXJCountryBattleIntroduceCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJCountryBattleIntroduceCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_title = str;
		break;
	case 1:
		m_image_sw = str;
		break;
	case 2:
		m_pageBtn_R = str;
		break;
	case 3:
		m_pageBtn_L = str;
		break;
	case 4:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void crJXJCountryBattleIntroduceCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			ref_ptr<crStaticTextWidgetNode> title = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title));
			ref_ptr<crMultiSwitch> image_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image_sw));
			ref_ptr<crButtonWidgetNode> pageBtn_R = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_pageBtn_R));
			ref_ptr<crButtonWidgetNode> pageBtn_L = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_pageBtn_L));
			unsigned char curPageNum = 0;
			crData *pdata = myPlayer->getDataClass();
			if (pdata)
			{
				void *param;
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
				pdata->getParam(WCHDATA_JXJGameIntroductionCurPageNum, param);
				if (param)
				{
					curPageNum = *(unsigned char *)param;
				}
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
			}
			if (title.valid())
			{
				title->setString(L"如何参加国战");
			}
			if (image_sw.valid())
			{
				image_sw->setActiveSwitchSet(curPageNum);
			}
			if (pageBtn_L.valid())
			{
				if (0 == curPageNum)
				{
					pageBtn_L->setEnable(false);
				}
				else
				{
					pageBtn_L->setEnable(true);
				}
			}
			if (pageBtn_R.valid())
			{
				if (image_sw->getSwitchSetList().size()-1 == curPageNum)
				{
					pageBtn_R->setEnable(false);
				}
				else
				{
					pageBtn_R->setEnable(true);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAdvancedMasterCanvasUpdateMethod
//高手进阶页面刷新
/////////////////////////////////////////
crJXJAdvancedMasterCanvasUpdateMethod::crJXJAdvancedMasterCanvasUpdateMethod()
{
}

crJXJAdvancedMasterCanvasUpdateMethod::crJXJAdvancedMasterCanvasUpdateMethod(const crJXJAdvancedMasterCanvasUpdateMethod& handle):
	crMethod(handle),
	m_title_1(handle.m_title_1),
	m_title_2(handle.m_title_2),
	m_title_3(handle.m_title_3),
	m_title_4(handle.m_title_4),
	m_content1_1(handle.m_content1_1),
	m_content1_2(handle.m_content1_2),
	m_content1_3(handle.m_content1_3),
	m_content1_4(handle.m_content1_4),
	m_content2_1(handle.m_content2_1),
	m_content2_2(handle.m_content2_2),
	m_content2_3(handle.m_content2_3),
	m_content2_4(handle.m_content2_4),
	m_content3_1(handle.m_content3_1),
	m_content3_2(handle.m_content3_2),
	m_content3_3(handle.m_content3_3),
	m_content3_4(handle.m_content3_4),
	m_content4_1(handle.m_content4_1),
	m_content4_2(handle.m_content4_2),
	m_canvas(handle.m_canvas),
	m_mainTitle(handle.m_mainTitle)
{
}

void crJXJAdvancedMasterCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJAdvancedMasterCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_title_1 = str;
		break;
	case 1:
		m_title_2 = str;
		break;
	case 2:
		m_title_3 = str;
		break;
	case 3:
		m_title_4 = str;
		break;
	case 4:
		m_content1_1 = str;
		break;
	case 5:
		m_content1_2 = str;
		break;
	case 6:
		m_content1_3 = str;
		break;
	case 7:
		m_content1_4 = str;
		break;
	case 8:
		m_content2_1 = str;
		break;
	case 9:
		m_content2_2 = str;
		break;
	case 10:
		m_content2_3 = str;
		break;
	case 11:
		m_content2_4 = str;
		break;
	case 12:
		m_content3_1 = str;
		break;
	case 13:
		m_content3_2 = str;
		break;
	case 14:
		m_content3_3 = str;
		break;
	case 15:
		m_content3_4 = str;
		break;
	case 16:
		m_content4_1 = str;
		break;
	case 17:
		m_content4_2 = str;
		break;
	case 18:
		m_canvas =  str;
		break;
	case 19:
		m_mainTitle = str;
		break;
	default:
		break;
	}
}

void crJXJAdvancedMasterCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			ref_ptr<crStaticTextWidgetNode> mainTitle = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_mainTitle));
			ref_ptr<crStaticTextWidgetNode> title_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title_1));
			ref_ptr<crStaticTextWidgetNode> title_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title_2));
			ref_ptr<crStaticTextWidgetNode> title_3 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title_3));
			ref_ptr<crStaticTextWidgetNode> title_4 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_title_4));
			ref_ptr<crStaticTextWidgetNode> content1_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content1_1));
			ref_ptr<crStaticTextWidgetNode> content1_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content1_2));
			ref_ptr<crStaticTextWidgetNode> content1_3 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content1_3));
			ref_ptr<crStaticTextWidgetNode> content1_4 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content1_4));
			ref_ptr<crStaticTextWidgetNode> content2_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content2_1));
			ref_ptr<crStaticTextWidgetNode> content2_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content2_2));
			ref_ptr<crStaticTextWidgetNode> content2_3 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content2_3));
			ref_ptr<crStaticTextWidgetNode> content2_4 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content2_4));
			ref_ptr<crStaticTextWidgetNode> content3_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content3_1));
			ref_ptr<crStaticTextWidgetNode> content3_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content3_2));
			ref_ptr<crStaticTextWidgetNode> content3_3 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content3_3));
			ref_ptr<crStaticTextWidgetNode> content3_4 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content3_4));
			ref_ptr<crStaticTextWidgetNode> content4_1 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content4_1));
			ref_ptr<crStaticTextWidgetNode> content4_2 = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_content4_2));
			if (mainTitle.valid())
			{
				mainTitle->setString(L"高手进阶");
			}
			if (title_1.valid())
			{
				title_1->setString(L"兵种篇");
			}
			if (title_2.valid())
			{
				title_2->setString(L"将卡篇");
			}
			if (title_3.valid())
			{
				title_3->setString(L"装备篇");
			}
			if (title_4.valid())
			{
				title_4->setString(L"灭国篇");
			}
			if (content1_1.valid())
			{
				content1_1->setString(L"弓兵系进阶攻略");
			}
			if (content1_2.valid())
			{
				content1_2->setString(L"步兵系进阶攻略");
			}
			if (content1_3.valid())
			{
				content1_3->setString(L"骑兵系进阶攻略");
			}
			if (content1_4.valid())
			{
				content1_4->setString(L"器械系进阶攻略");
			}
			if (content2_1.valid())
			{
				content2_1->setString(L"魏国将卡搭配攻略");
			}
			if (content2_2.valid())
			{
				content2_2->setString(L"蜀国将卡搭配攻略");
			}
			if (content2_3.valid())
			{
				content2_3->setString(L"吴国将卡搭配攻略");
			}
			if (content2_4.valid())
			{
				content2_4->setString(L"群雄将卡搭配攻略");
			}
			if (content3_1.valid())
			{
				content3_1->setString(L"弓兵系装备精炼攻略");
			}
			if (content3_2.valid())
			{
				content3_2->setString(L"步兵系装备精炼攻略");
			}
			if (content3_3.valid())
			{
				content3_3->setString(L"骑兵系装备精炼攻略");
			}
			if (content3_4.valid())
			{
				content3_4->setString(L"器械系装备精炼攻略");
			}
			if (content4_1.valid())
			{
				content4_1->setString(L"灭国战应该怎么打");
			}
			if (content4_2.valid())
			{
				content4_2->setString(L"轮回策略");
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJGameIntroduceCanvasTurnPageMethod
//新手攻略翻页
/////////////////////////////////////////
crJXJGameIntroduceCanvasTurnPageMethod::crJXJGameIntroduceCanvasTurnPageMethod():
	m_this(NULL),
	m_index(0)
{
}

crJXJGameIntroduceCanvasTurnPageMethod::crJXJGameIntroduceCanvasTurnPageMethod(const crJXJGameIntroduceCanvasTurnPageMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_swnode(handle.m_swnode)
{
}

void crJXJGameIntroduceCanvasTurnPageMethod::inputParam(int i, void *param)
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

void crJXJGameIntroduceCanvasTurnPageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_swnode = str;
		break;
	}
}

void crJXJGameIntroduceCanvasTurnPageMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (parentCanvas.valid() && myPlayer && myPlayer->getDataClass())
		{
			int select = 0;
			crData *pdata = myPlayer->getDataClass();
			if (pdata)
			{
				void *param;
				unsigned char *curPageNum = 0;
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
				pdata->getParam(WCHDATA_JXJGameIntroductionCurPageNum, param);
				curPageNum = (unsigned char *)param;
				switch (m_index)
				{
				case 0:
					if (*curPageNum > 0)
					{
						*curPageNum -= 1;
					}
					break;
				case 1:
					{
						ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_swnode));
						if(sw.valid())
						{
							int count = sw->getSwitchSetList().size();
							if (*curPageNum+1 < count)
							{
								*curPageNum += 1;
							}
						}
					}
					break;
				default:
					break;
				}
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJIconDetailIntroduceMethod
//战斗力提升页面点击Icon弹窗
/////////////////////////////////////////
crJXJIconDetailIntroduceMethod::crJXJIconDetailIntroduceMethod():
	m_this(NULL),
	m_index(0)
{
}

crJXJIconDetailIntroduceMethod::crJXJIconDetailIntroduceMethod(const crJXJIconDetailIntroduceMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_main_sw(handle.m_main_sw),
	m_image_sw(handle.m_image_sw),
	m_introduction(handle.m_introduction)
{
}

void crJXJIconDetailIntroduceMethod::inputParam(int i, void *param)
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

void crJXJIconDetailIntroduceMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_main_sw = str;
		break;
	case 2:
		m_image_sw = str;
		break;
	case 3:
		m_introduction = str;
		break;
	default:
		break;
	}
}

void crJXJIconDetailIntroduceMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		if (parentCanvas.valid())
		{
			ref_ptr<crMultiSwitch> main_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_main_sw));
			ref_ptr<crMultiSwitch> image_sw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_image_sw));
			ref_ptr<crStaticTextWidgetNode> introduction = dynamic_cast<crStaticTextWidgetNode *>(parentCanvas->getWidget(m_introduction));
			if (main_sw.valid())
			{
				if (m_index > 0)
				{
					std::string str;
					int noticeId = 0;
					main_sw->setActiveSwitchSet(1);
					if (image_sw.valid())
					{
						image_sw->setActiveSwitchSet(m_index - 1); //m_index的值从1到6，image_sw中的值从0到5
					}
					switch (m_index)
					{
					case 1:
						noticeId = 7027;
						break;
					case 2:
						noticeId = 7028;
						break;
					case 3:
						noticeId = 7029;
						break;
					case 4:
						noticeId = 7025;
						break;
					case 5:
						noticeId = 7030;
						break;
					case 6:
						noticeId = 7026;
						break;
					default:
						break;
					}
					CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					if (noticeHandle && noticeId > 0)
					{

						int mode = 2;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&noticeId);
						noticeHandle->inputParam(WCHDATA_NoticeOutPut,&str);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);						
					}
					introduction->setString(str);
				}
				else
				{
					main_sw->setActiveSwitchSet(0);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAdvancedMasterForumNavigatorMethod
//高手进阶论坛导航
/////////////////////////////////////////
crJXJAdvancedMasterForumNavigatorMethod::crJXJAdvancedMasterForumNavigatorMethod()
{

}

crJXJAdvancedMasterForumNavigatorMethod::crJXJAdvancedMasterForumNavigatorMethod(const crJXJAdvancedMasterForumNavigatorMethod& handle):
	m_url(handle.m_url),
	m_txUrl(handle.m_txUrl)
{

}

void crJXJAdvancedMasterForumNavigatorMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJAdvancedMasterForumNavigatorMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_url = str;
		break;
	case 1:
		m_txUrl = str;
		break;
	default:
		break;
	}
}

void crJXJAdvancedMasterForumNavigatorMethod::operator ()(crHandle &handle)
{
	std::string url;
	//if (CRIOManager::fileExists("web.ini"))
	if(crMyPlayerData::getInstance()->getSelectedGame()->getPlatform().find("QQ") != std::string::npos)
	{//tx
		if(CRCore::crDisplaySettings::instance()->getRunMode()==3)
		{//WebGame
			TCHAR* str = new TCHAR[256] ;
			wsprintf(str,(LPCTSTR)m_txUrl.c_str());

			HWND  hwnd = CRNetApp::crRunGameHandle::getInstance()->getParentWnd();
			COPYDATASTRUCT data;
			data.cbData = _msize(str);
			data.lpData = str;
			if (hwnd > 0 )
			{
				::SendMessage(hwnd,WM_USER+603,NULL,(LPARAM)&data);
			}
			delete [] str;
		}
		else
		{
			url = m_txUrl;
			ShellExecute(NULL,"open",url.c_str(),NULL,NULL,SW_SHOW);
		}
	}
	else
	{
		url = m_url;
		ShellExecute(NULL,"open",url.c_str(),NULL,NULL,SW_SHOW);
	}
}
/////////////////////////////////////////
//
//crJXJHideGameIntroductionIconMethod
//隐藏领地界面新手宝典图标(GameClient)
/////////////////////////////////////////
JXJ::crJXJHideGameIntroductionIconMethod::crJXJHideGameIntroductionIconMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJHideGameIntroductionIconMethod::crJXJHideGameIntroductionIconMethod( const crJXJHideGameIntroductionIconMethod& handle ):
	crMethod(handle),
	m_netType(handle.m_netType),
	m_noTipsCheckBox(handle.m_noTipsCheckBox)
{

}

void JXJ::crJXJHideGameIntroductionIconMethod::inputParam( int i, void *param )
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

void JXJ::crJXJHideGameIntroductionIconMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_noTipsCheckBox = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJHideGameIntroductionIconMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData && playerGameData->getDataClass())
	{
		crData *pdata = playerGameData->getDataClass();
		ref_ptr<crCanvasNode> parentCanvas = dynamic_cast<crCanvasNode *>(m_this->getParentCanvas());
		if(pdata && parentCanvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode> noTipsWidget = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_noTipsCheckBox));
			if (noTipsWidget.valid())
			{
				bool noTips = noTipsWidget->getSelect();
				pdata->inputParam(WCHDATA_JXJGameIntroductionNoTipsCheck, &noTips);

				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if (netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(1);//1表示同步新手引导图标显示状态，2表示同步是否已经自动弹出过节日活动窗口
					stream->_writeBool(noTips);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHideGameIntroductionIcon,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}
///////////////////////////////////////////////////
//
//crJXJRecvHideGameIntroductionIconMethod
//隐藏领地界面新手宝典图标(GameServer)
//////////////////////////////////////////////////
crJXJRecvHideGameIntroductionIconMethod::crJXJRecvHideGameIntroductionIconMethod():
	m_netType(GameClient_Game){}
crJXJRecvHideGameIntroductionIconMethod::crJXJRecvHideGameIntroductionIconMethod(const crJXJRecvHideGameIntroductionIconMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvHideGameIntroductionIconMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvHideGameIntroductionIconMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvHideGameIntroductionIconMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getDataClass() && m_stream.valid() && m_netType == GameServer)
	{
		unsigned char type = m_stream->_readUChar();
		bool noTips = m_stream->_readBool();
		crData *pData = m_this->getDataClass();
		if (pData)
		{
			switch (type)
			{
			case 1:
				pData->inputParam(WCHDATA_JXJGameIntroductionNoTipsCheck, &noTips);
				break;
			case 2:
				pData->inputParam(WCHDATA_JXJHadShowFestivalHuodongCanvas, &noTips);
				break;
			default:
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckNotificationCanvasIndexMethod
//检查进入战场提示框索引值
/////////////////////////////////////////
// JXJ::crJXJCheckNotificationCanvasIndexMethod::crJXJCheckNotificationCanvasIndexMethod():
// 	m_this(NULL),
// 	m_netType(GameClient_Game)
// {
// 
// }
// 
// JXJ::crJXJCheckNotificationCanvasIndexMethod::crJXJCheckNotificationCanvasIndexMethod( const crJXJCheckNotificationCanvasIndexMethod& handle ):
// 	crMethod(handle),
// 	m_noticeCanvas(handle.m_noticeCanvas),
// 	m_netType(handle.m_netType),
// 	m_this(NULL)
// {
// 
// }
// 
// void JXJ::crJXJCheckNotificationCanvasIndexMethod::inputParam( int i, void *param )
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		if(param == 0)
// 		{//释放
// 			m_this = NULL;
// 		}
// 		break;
// 	case 1:
// 		m_this = (crPlayerGameData*)param;
// 		break;
// 	}
// }
// 
// void JXJ::crJXJCheckNotificationCanvasIndexMethod::addParam( int i, const std::string& str )
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		m_noticeCanvas = str;
// 		break;
// 	}
// }
// 
// void JXJ::crJXJCheckNotificationCanvasIndexMethod::operator()( crHandle &handle )
// {
// 	unsigned char output = 0;
// 	if(m_this && m_netType == GameClient_Game)
// 	{
// 		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
// 		if(canvas.valid() && canvas->getDataClass())
// 		{
// 			void *param;
// 			crData *canvasData = canvas->getDataClass();
// 			canvasData->getParam(WCHDATA_JXJCanvasIndex, param);
// 			output = *(unsigned char *)param;
// 		}
// 	}
// 	handle.outputParam(0,&output);
// }
/////////////////////////////////////////
//
//crJXJGainConsortiaStartPromotionVecMethod
//生成军团开服活动满足资格成员列表
/////////////////////////////////////////
crJXJGainConsortiaStartPromotionVecMethod::crJXJGainConsortiaStartPromotionVecMethod():
	m_this(NULL){}
crJXJGainConsortiaStartPromotionVecMethod::crJXJGainConsortiaStartPromotionVecMethod(const crJXJGainConsortiaStartPromotionVecMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJGainConsortiaStartPromotionVecMethod::inputParam(int i, void *param)
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

void crJXJGainConsortiaStartPromotionVecMethod::addParam(int i, const std::string& str)
{
}

void crJXJGainConsortiaStartPromotionVecMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		if (gsBrainData)
		{
			void *param;
			crJXJShili *shili;
			crData *shiliData;
			PeopleMap *peopleMap = NULL;
			PeopleMap::iterator peoItr;
			int maxNum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaStartPromotionNum).c_str()));
			gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
			ConsortialvRank *consortialvRank = (ConsortialvRank *)param;
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator cmItr;
			for( int i = 0; i < 3; ++ i)
			{
				gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				if (shili && shili->getDataClass())
				{
					shiliData = shili->getDataClass();
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJConsortiaPromotionPlayerVecT,param);
					ConsortiaStartPromotionVec * consortiaStartPromotionVecT = (ConsortiaStartPromotionVec *)param;
					consortiaStartPromotionVecT->clear();

					shiliData->getParam(WCHDATA_JXJConsortiaPromotionRankVecT,param);
					ConsortiaStartPromotionRankVec * consortiaPromotionRankVecT = (ConsortiaStartPromotionRankVec *)param;
					consortiaPromotionRankVecT->clear();

					ConsortialvRank::RankSet & rankSet = consortialvRank->getRankSet();
					ConsortialvRank::RankSet::iterator rsItr = rankSet.begin();
					for (int j = 0; j < maxNum && rsItr != rankSet.end(); ++ rsItr)
					{//本国军团排行前maxNum名
						if (rsItr->valid())
						{
							ref_ptr<crData>groupData;
							cmItr = consortiamap->find((*rsItr)->m_id);
							if (cmItr != consortiamap->end() && cmItr->second.valid() && cmItr->second->getShiLi() == c_startShiliID+i)
							{
								groupData = cmItr->second->getDataClass();
							}
							if (groupData.valid())
							{
								//军团开福活动入围军团列表
								CRCore::ref_ptr<strConsortiaHuoDongInfo> newStrinfo = new strConsortiaHuoDongInfo;
								newStrinfo->rankNum = j+1;
								newStrinfo->groupId = ((*rsItr)->m_id);
								consortiaPromotionRankVecT->push_back(newStrinfo);

								groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
								ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
								for (ConsortiaMemberMap::iterator itr = consortiaMemberMap->begin();itr != consortiaMemberMap->end();++itr)
								{//军团开服活动入围玩家列表
									int mygrouppos = cmItr->second->getUserGroupPos(itr->second->getPlayerID());
									CRCore::ref_ptr<strConsortiaHuoDongInfo> strinfo = new strConsortiaHuoDongInfo;
									strinfo->nPlayerId = itr->second->getPlayerID();
									strinfo->rankNum = j+1;
									strinfo->isConsortiaLeader = (mygrouppos==JXJGroup_FirstLeader);
									strinfo->groupId = ((*rsItr)->m_id);
									consortiaStartPromotionVecT->push_back(strinfo);
								}
								++j;
							}
						}
					}
					bool modify = true;
					shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
			gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
////////////////////////////////////////////////////
//crJXJQueryConsortiaPromotionDataMethod
//查询军团开服活动数据 (Client)
////////////////////////////////////////////////////
crJXJQueryConsortiaPromotionDataMethod::crJXJQueryConsortiaPromotionDataMethod()
{

}

crJXJQueryConsortiaPromotionDataMethod::crJXJQueryConsortiaPromotionDataMethod(const crJXJQueryConsortiaPromotionDataMethod& handle):
	crMethod(handle)
{

}

void crJXJQueryConsortiaPromotionDataMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}

void crJXJQueryConsortiaPromotionDataMethod::addParam(int i, const std::string& str)
{

}

void crJXJQueryConsortiaPromotionDataMethod::operator ()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(netConductor && playerGameData && playerGameData->getDataClass())
	{
		void *param;
		int playerId = playerGameData->getPlayerID();
		ref_ptr<crData> pdata = playerGameData->getDataClass();
		if (pdata.valid())
		{
			pdata->getParam(WCHDATA_JXJShiliID, param);
			unsigned char myshiliId = *(unsigned char *)param;
			pdata->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char querycount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRankingCount,viplv).c_str());
			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(4);
			streamBuf->_writeInt(playerId);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryConsortiaPromotionData,streamBuf.get());
			netConductor->getNetManager()->sendPacket("all",packet);

			ref_ptr<crStreamBuf> newStream = new crStreamBuf;
			newStream->createBuf(4);
			newStream->_writeUShort(0);//start
			newStream->_writeUChar(querycount);//count
			newStream->_writeUChar(myshiliId);//shiliid
			crPlayerDataEventPacket newPacket;
			crPlayerDataEventPacket::buildRequestPacket(newPacket,WCH_JXJRecvConsortialvRanking,newStream.get());
			netConductor->getNetManager()->sendPacket("all",newPacket);
		}
	}
}
////////////////////////////////////////////////////
//crJXJRecvQueryConsortiaPromotionDataMethod
//查询军团开服活动数据 (Server)
////////////////////////////////////////////////////
JXJ::crJXJRecvQueryConsortiaPromotionDataMethod::crJXJRecvQueryConsortiaPromotionDataMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryConsortiaPromotionDataMethod::crJXJRecvQueryConsortiaPromotionDataMethod( const crJXJRecvQueryConsortiaPromotionDataMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvQueryConsortiaPromotionDataMethod::inputParam( int i, void *param )
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
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvQueryConsortiaPromotionDataMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvQueryConsortiaPromotionDataMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_stream->_readInt();
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && gsBrainData && gameServer)
			{
				ref_ptr<crData> pdata = playerData->getPlayerGameData()->getDataClass();
				if (pdata.valid())
				{
					void* param;
					int restTimer = 1;//剩余时间
					int lastTimer = 0;//持续时间
					std::string dateStr = crGlobalHandle::gData()->gStartDate();
					typedef std::vector<int> Vec5i;
					Vec5i startTimeVec5i;
					startTimeVec5i.reserve(5);
					crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
					startTimeVec5i.resize(5,0);
					time_t nowTime_t = time(0);
					struct  tm  nowTm = *localtime(&nowTime_t); //服务器当前时间
					struct tm  startTm;
					memset(&startTm,0,sizeof (startTm));

					startTm.tm_year = startTimeVec5i[0] - 1900;
					startTm.tm_mon = startTimeVec5i[1] - 1;
					startTm.tm_mday = startTimeVec5i[2];
					startTm.tm_hour = startTimeVec5i[3];
					startTm.tm_min = startTimeVec5i[4];
					startTm.tm_isdst = nowTm.tm_isdst;

					time_t startTime_t = mktime(&startTm); //服务器开服时间
					int startedTimer = (nowTime_t - startTime_t); //已开服时间
					//unsigned short startDays = ceil((float)(nowTime_t - startTime_t) / 86400.0f);//开服天数
					if (startedTimer <= 0)
					{
						startedTimer = 0;
					}
					int Deadline = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaPromotionDeadline, 0).c_str())); //活动截止到开服第几天
					//lastTimer = (23 - startTm.tm_hour) * 3600 + (59 - startTm.tm_min) * 60 + (59 - startTm.tm_isdst) + 24 * 3600 * (Deadline - 2) + 5 * 3600;
					lastTimer =/* (23 - startTm.tm_hour) * 3600 + (59 - startTm.tm_min) * 60 + (59 - startTm.tm_isdst) + */24 * 3600 * Deadline + 5 * 3600;
					restTimer = lastTimer - startedTimer;
					if (restTimer < 0)
					{
						restTimer = 0;
					}
					unsigned char rankVal = 0;
					bool hadGotten = false;
					std::string consortiaName[5];
					std::string consortiaLeaderName[5];
					crJXJShili *shili;
					crData *shiliData;
					CRCore::ScopedLock<crData> lock(*pdata);
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));

					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator cmItr;

					//pdata->inputParam(WCHDATA_JXJGameStartedDays, &startDays);
					pdata->getParam(WCHDATA_JXJShiliID, param);
					unsigned char myshili = *(unsigned char *)param;
					pdata->getParam(WCHDATA_JXJHadGottenConsortiaPromotionReward, param);
					hadGotten = *(bool *)param;
					gsBrainData->getParam(WCHDATA_JXJShiliWei + myshili - c_startShiliID,param);
					shili = (crJXJShili *)param;
					if (shili && shili->getDataClass())
					{
						shiliData = shili->getDataClass();
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJConsortiaPromotionPlayerVecT,param);
						ConsortiaStartPromotionVec * consortiaPromotionPlayerVecT = (ConsortiaStartPromotionVec *)param;
						ConsortiaStartPromotionVec::iterator tdItr;
						for (tdItr = consortiaPromotionPlayerVecT->begin(); tdItr != consortiaPromotionPlayerVecT->end(); ++ tdItr)
						{
							if (tdItr->valid())
							{
								if (playerid == (*tdItr)->nPlayerId)
								{
									rankVal = (*tdItr)->rankNum;
									break;
								}
							}
						}

						shiliData->getParam(WCHDATA_JXJConsortiaPromotionRankVecT,param);
						ConsortiaStartPromotionRankVec * consortiaPromotionRankVecT = (ConsortiaStartPromotionRankVec *)param;
						ConsortiaStartPromotionRankVec::iterator jtItr;
						jtItr = consortiaPromotionRankVecT->begin();
						for (int i = 0; jtItr != consortiaPromotionRankVecT->end() && i < 5; ++ jtItr)
						{
							if (jtItr->valid())
							{
								cmItr = consortiamap->find((*jtItr)->groupId);
								if (cmItr != consortiamap->end() && cmItr->second.valid())
								{
									cmItr->second->getName(consortiaName[i]);
									cmItr->second->getLeaderName(0,consortiaLeaderName[i]);
									++i;
								}
							}
						}
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
						pdata->inputParam(WCHDATA_JXJConsortiaPromotionMyRankVal, &rankVal);
					}
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(128);
					//stream->_writeUShort(startDays);
					stream->_writeInt(restTimer);
					stream->_writeUChar(rankVal);
					stream->_writeBool(hadGotten);
					for (int i = 0; i < 5; ++i)
					{
						stream->_writeString(consortiaName[i]);
						stream->_writeString(consortiaLeaderName[i]);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryConsortiaPromotionData,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		if(m_netType == GameClient_Game)
		{
			//unsigned short startDays = 0;
			int restTimer = 0;
			unsigned char myrankVal = 0;
			bool hadGotten = false;
			std::string consortiaName[5];
			std::string consortiaLeaderName[5];

			//startDays = m_stream->_readUShort();
			restTimer = m_stream->_readInt();
			myrankVal = m_stream->_readUChar();
			hadGotten = m_stream->_readBool();
			for (int i = 0; i < 5; ++i)
			{
				consortiaName[i] = m_stream->_readString();
				consortiaLeaderName[i] = m_stream->_readString();
			}

			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			crData *data = myPlayer->getDataClass();
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				void* param;
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				//data->inputParam(WCHDATA_JXJGameStartedDays, &startDays);
				data->inputParam(WCHDATA_JXJConsortiaPromotionMyRankVal, &myrankVal);
				ref_ptr<crData> canvasData = canvas->getDataClass();
				if (canvasData.valid())
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					std::vector<std::string>* ConsortiaNameVec;
					canvasData->getParam(WCHDATA_JXJConsortiaPromotionNameVec, param);
					ConsortiaNameVec = (std::vector<std::string> *)param;
					ConsortiaNameVec->clear();
					for (int i = 0; i < 5 && i < consortiaName->size(); ++i)
					{
						(*ConsortiaNameVec).push_back(consortiaName[i]);
					}

					std::vector<std::string>* ConsortiaLeaderNameVec;
					canvasData->getParam(WCHDATA_JXJConsortiaPromotionLeaderVec, param);
					ConsortiaLeaderNameVec = (std::vector<std::string> *)param;
					ConsortiaLeaderNameVec->clear();
					for (int i = 0; i < 5 && i < consortiaLeaderName->size(); ++i)
					{
						(*ConsortiaLeaderNameVec).push_back(consortiaLeaderName[i]);
					}
					canvasData->inputParam(WCHDATA_JXJConsortiaPromotionTimer, &restTimer);
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
///////////////////////////////////////////////////
//
//crJXJConsortiaStartedPromotionCanvasUpdateMethod
//军团开服活动窗口刷新
///////////////////////////////////////////////////
crJXJConsortiaStartedPromotionCanvasUpdateMethod::crJXJConsortiaStartedPromotionCanvasUpdateMethod():
	m_this(NULL)
{
}
crJXJConsortiaStartedPromotionCanvasUpdateMethod::crJXJConsortiaStartedPromotionCanvasUpdateMethod(const crJXJConsortiaStartedPromotionCanvasUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas)
{
}
void crJXJConsortiaStartedPromotionCanvasUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJConsortiaStartedPromotionCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_consortiaName_rank[i-1] = str;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		m_consortiaLeader_rank[i-6] = str;
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		m_leaderExtraReward[i-11] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		m_getRewardBtn[i-16] = str;
		break;
	case 21:
	case 22:
	case 23:
	case 24:

	case 25:
	case 26:
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
	case 39:
	case 40:
		m_icon[(i-21)/4][(i-21)%4] = str;
		break;
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
	case 52:

	case 53:
	case 54:
	case 55:
	case 56:

	case 57:
	case 58:
	case 59:
	case 60:
		m_itemCount[(i-41)/4][(i-41)%4] = str;
		break;
	case 61:
		m_introductionText = str;
		break;
	case 62:
		m_timeText = str;
		break;
	case 63:
		m_newCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJConsortiaStartedPromotionCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = m_this;//crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (canvas.valid() && canvas->getDataClass() && playerGameData)
	{
		ref_ptr<crData> data = playerGameData->getDataClass();
		if (data.valid())
		{
			void *param;
			crTableIO::StrVec record;
			std::vector<int> awardVec;
			//std::vector<int> extraGiftGoldVec;
			std::vector<std::string> consortiaNameVec;
			std::vector<std::string> consortiaLeaderVec;
			//crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaPromotionLeaderExtraReward, 0), extraGiftGoldVec);
			ref_ptr<crTableIO> JuntuanHuoDongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJConsortiaPromotionRewardTab);
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int nameIndex = itemtab->getTitleIndex("name");
			int itemidIndex = itemtab->getTitleIndex("itemid");
			int qualityIndex = itemtab->getTitleIndex("品质");
			int extraRewardIndex = JuntuanHuoDongTab->getTitleIndex("额外奖励");
			int rowcount = JuntuanHuoDongTab->getRowCount();
			for (int i = 0; i < rowcount; ++i)
			{
				if (JuntuanHuoDongTab.valid() && JuntuanHuoDongTab->queryOneRecord(0,crArgumentParser::appItoa(i+1),record) >=0)
				{
					std::string imageName;
					std::string strCount;
					int restTimer = 1;//用这个时间去检查是否开放奖励领取功能，从服务器查询过来。
					char rankVal = 0;
					bool hadGotten = false;
					int itemId;
					int count;
					data->getParam(WCHDATA_JXJShiliID, param);
					unsigned char myshili = *(unsigned char *)param;
					data->getParam(WCHDATA_JXJConsortiaPromotionMyRankVal, param);
					rankVal = *(unsigned char *)param;
					data->getParam(WCHDATA_JXJHadGottenConsortiaPromotionReward,param);
					hadGotten = *(bool *)param;
					ref_ptr<crButtonWidgetNode> getRewardBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_getRewardBtn[i]));
					ref_ptr<crStaticTextWidgetNode> consortiaName_rank = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_consortiaName_rank[i]));
					ref_ptr<crStaticTextWidgetNode> consortiaLeader_rank = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_consortiaLeader_rank[i]));
					ref_ptr<crHypertextWidgetNode> leaderExtraRewardText = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_leaderExtraReward[i]));
					ref_ptr<crStaticTextWidgetNode> introductionText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_introductionText));
					ref_ptr<crStaticTextWidgetNode> timeText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_timeText));
					if (introductionText.valid())
					{
						introductionText->setString(L"奖励以活动结算时的本国军团排名和军团成员列表为准");
						introductionText->setVisiable(true);
					}
					if (timeText.valid())
					{
						timeText->setString(L" 活动倒计时：");
						timeText->setVisiable(true);
					}
					if (leaderExtraRewardText.valid())
					{
						leaderExtraRewardText->clear();
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
						v_i.push_back(0.0f);
						v_i.push_back(0.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","     额外");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(140.0f);
						v_i.push_back(40.0f);
						v_i.push_back(40.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text", "+" + record[extraRewardIndex]);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						v_i.clear();
						v_i.push_back(0.0f);
						v_i.push_back(0.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","礼金");
						cfg_script.Pop();

						cfg_script.Pop();
						leaderExtraRewardText->setHypertext(cfg_script);
					}
					ref_ptr<crData> canvasData = canvas->getDataClass();
					canvasData->getParam(WCHDATA_JXJConsortiaPromotionTimer, param);
					restTimer = *(int *)param;
					if (restTimer == 0)
					{//时间结束才公布入选名单
						canvasData->getParam(WCHDATA_JXJConsortiaPromotionNameVec, param);
						consortiaNameVec = *(std::vector<std::string> *)param;
						canvasData->getParam(WCHDATA_JXJConsortiaPromotionLeaderVec, param);
						consortiaLeaderVec = *(std::vector<std::string> *)param;
						if (consortiaName_rank.valid())
						{
							if (i < consortiaNameVec.size())
							{
								consortiaName_rank->setString(consortiaNameVec[i]);
							}
						}
						if (consortiaLeader_rank.valid())
						{
							if (i < consortiaLeaderVec.size())
							{
								consortiaLeader_rank->setString(consortiaLeaderVec[i]);
							}
						}
					}
					else 
					{
						ref_ptr<crCanvasNode> newCanvas = crFilterRenderManager::getInstance()->findCanvas(m_newCanvas);
						if (newCanvas.valid() && newCanvas->getDataClass())
						{
							ref_ptr<crData> newCanvasData = newCanvas->getDataClass();
							if (newCanvasData.valid())
							{
								newCanvasData->getParam(WCHDATA_JXJConsortialvRankingMap,param);
								GrouplvRankingMap *lvRankingMap = (GrouplvRankingMap *)param;
								GrouplvRankingMap::iterator itr = lvRankingMap->begin();
								for( int j = 0;itr != lvRankingMap->end() && itr->second.valid() && j <= i;
									++itr)
								{
									if (itr->second->getShiliID() == myshili)
									{
										if (j == i)
										{
											if (consortiaName_rank.valid())
											{
												consortiaName_rank->setString(itr->second->getName());
											}
											if (consortiaLeader_rank.valid())
											{
												consortiaLeader_rank->setString(itr->second->getCreatorName());
											}
										}
										++j;
									}
								}
							}
						}
						else
						{
							if (consortiaName_rank.valid())
							{
								consortiaName_rank->setString(L"");
							}
							if (consortiaLeader_rank.valid())
							{
								consortiaLeader_rank->setString(L"");
							}
						}
					}
					if (getRewardBtn.valid())
					{
						if (rankVal > 0 && rankVal == i+1 && restTimer == 0)
						{
							if (hadGotten)
							{
								getRewardBtn->setEnable(false);
							}
							else
							{
								getRewardBtn->setVisiable(true);
								getRewardBtn->setEnable(true);
							}
						}
						else
						{
							getRewardBtn->setVisiable(false);
						}
					}
					int awardIndex = JuntuanHuoDongTab->getTitleIndex("奖励");
					crArgumentParser::appAtoVec(record[awardIndex],awardVec);
					for (int j = 0; j < 4; ++j)
					{
						ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_icon[i][j]));
						ref_ptr<crStaticTextWidgetNode> itemCount = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_itemCount[i][j]));

						imageName = "";
						strCount = "";
						if (j < awardVec.size()/3)
						{
							crTableIO::StrVec itemRecord;
							int iconIdx = itemtab->getTitleIndex("icon");
							int itemIdIdx = itemtab->getTitleIndex("itemid");
							itemId = awardVec[0+j*3];
							count = awardVec[2+j*3];
							if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(itemId),itemRecord) >= 0)
							{
								imageName = itemRecord[iconIdx];
								strCount = crArgumentParser::appItoa(count);
							}
							crData* uiData = icon->getDataClass();
							if (uiData)
							{
								uiData->inputParam(0,&itemId);
								uiData->inputParam(1,&count);
							}
							icon->setVisiable(true);
							itemCount->setVisiable(true);
							icon->setImageName(imageName);
							itemCount->setString(strCount);
						}
						else
						{
							icon->setVisiable(false);
							itemCount->setVisiable(false);
						}
					}
				}
			}
		}
	}
}
///////////////////////////////////////////////////
//
//crJXJUIGetConsortiaPromotionAwardMethod
//Client领取军团开服活动奖励
//////////////////////////////////////////////////
crJXJUIGetConsortiaPromotionAwardMethod::crJXJUIGetConsortiaPromotionAwardMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJUIGetConsortiaPromotionAwardMethod::crJXJUIGetConsortiaPromotionAwardMethod(const crJXJUIGetConsortiaPromotionAwardMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{
}
void crJXJUIGetConsortiaPromotionAwardMethod::inputParam(int i, void *param)
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
void crJXJUIGetConsortiaPromotionAwardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIGetConsortiaPromotionAwardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeChar(m_index);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGetConsortiaPromotionReward,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

///////////////////////////////////////////////////
//
//crJXJRecvGetConsortiaPromotionAwardMethod
//发放军团开服活动奖励
//////////////////////////////////////////////////
crJXJRecvGetConsortiaPromotionAwardMethod::crJXJRecvGetConsortiaPromotionAwardMethod():
	m_netType(GameClient_Game){}
crJXJRecvGetConsortiaPromotionAwardMethod::crJXJRecvGetConsortiaPromotionAwardMethod(const crJXJRecvGetConsortiaPromotionAwardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGetConsortiaPromotionAwardMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGetConsortiaPromotionAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGetConsortiaPromotionAwardMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		int playerid = m_this->getPlayerID();
		crData *pPlayerData = m_this->getDataClass();
		if (pPlayerData)
		{
			void* param = NULL;
			if(m_netType == GameServer)
			{
				char index = m_stream->_readChar();
				bool success = false; //领取成功
				bool condition = false; //领取资格检查
				bool hadGotten = false; //是否已领取奖励
				bool isGroupLeader = false;//是否是军团长
				unsigned char rankVal = 0;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					//gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
					crJXJShili *shili;
					crData *shiliData;
					//gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					//ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					//ConsortiaMap::iterator cmItr;

					pPlayerData->getParam(WCHDATA_JXJShiliID, param);
					unsigned char myshili = *(unsigned char *)param;
					gsBrainData->getParam(WCHDATA_JXJShiliWei + myshili - c_startShiliID,param);
					shili = (crJXJShili *)param;
					if (shili && shili->getDataClass())
					{
						shiliData = shili->getDataClass();
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJConsortiaPromotionPlayerVecT,param);
						ConsortiaStartPromotionVec * consortiaPromotionPlayerVecT = (ConsortiaStartPromotionVec *)param;
						ConsortiaStartPromotionVec::iterator tdItr;
						for (tdItr = consortiaPromotionPlayerVecT->begin(); tdItr != consortiaPromotionPlayerVecT->end(); ++ tdItr)
						{
							if (tdItr->valid())
							{
								if (playerid == (*tdItr)->nPlayerId)
								{
									rankVal = (*tdItr)->rankNum;
									isGroupLeader = (*tdItr)->isConsortiaLeader;
									condition = true;
									break;
								}
							}
						}
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
					//gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				pPlayerData->getParam(WCHDATA_JXJHadGottenConsortiaPromotionReward, param);
				hadGotten = *(bool *)param;
				if (condition && !hadGotten)
				{
					success = true;
					crTableIO::StrVec record;
					std::vector<std::string> awardVec;
					JXJItemBagVec items;
					ref_ptr<crTableIO> JuntuanHuoDongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJConsortiaPromotionRewardTab);
					if(JuntuanHuoDongTab.valid() && JuntuanHuoDongTab->queryOneRecord(0,crArgumentParser::appItoa(rankVal),record) >= 0)
					{
						int awardIndex = JuntuanHuoDongTab->getTitleIndex("奖励");
						crArgumentParser::appAtoVec(record[awardIndex],awardVec);
						for (int i = 0; i < awardVec.size()/3; ++i)
						{
							crBagItemData* pItem = new crBagItemData();
							pItem->setItemID(atoi(awardVec[0+i*3].c_str()));
							pItem->setEquipMagic(atoi(awardVec[1+i*3].c_str()));
							pItem->setItemCount(atoi(awardVec[2+i*3].c_str()));
							items.push_back(pItem);
						}
						m_this->doEvent(WCH_JXJRecvRewardItems,MAKECREPARAM(&items,0));

						if (isGroupLeader)
						{
							int extraRewardIndex = JuntuanHuoDongTab->getTitleIndex("额外奖励");
							if (extraRewardIndex > 0)
							{
								std::string logdata = "军团开服活动团长额外奖励： "; 
								int giftGold = atoi(record[extraRewardIndex].c_str());
								logdata += crArgumentParser::appItoa(giftGold);

								MoneyChangeData moneydata(giftGold,"军团活动额外礼金奖励");
								m_this->doEvent(WCH_JXJRecvGiftgold, MAKECREPARAM(&moneydata, NULL));
								GameLogData gamelog(Log_GetJunTuanHuoDongExtraAward,logdata);
								crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog)); 
							}
						}
					}
				}
				pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));
				if (success)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeBool(success);
					crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
					ref_ptr<crGameServerPlayerData> player = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGetConsortiaPromotionReward,stream.get());
					gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(),packet);
					pPlayerData->inputParam(WCHDATA_JXJHadGottenConsortiaPromotionReward, &success);
				}
				///游戏日志
				std::string logdata = "领取军团开服活动奖励:第" + crArgumentParser::appItoa(rankVal) + "名奖励";
				GameLogData gamelog(Log_GetJunTuanHuoDongAward, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
			}
			else if(m_netType == GameClient_Game)
			{
				bool success = m_stream->_readBool();
				pPlayerData->inputParam(WCHDATA_JXJHadGottenConsortiaPromotionReward, &success);
			}
		}
	}
}
//////////////////////////////////////////////////////
//
//crJXJConsortiaPromotionCanvasShowTimerMethod
//军团活动界面倒计时
//////////////////////////////////////////////////////
crJXJConsortiaPromotionCanvasShowTimerMethod::crJXJConsortiaPromotionCanvasShowTimerMethod():
	m_this(NULL)
{

}
crJXJConsortiaPromotionCanvasShowTimerMethod::crJXJConsortiaPromotionCanvasShowTimerMethod(const crJXJConsortiaPromotionCanvasShowTimerMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJConsortiaPromotionCanvasShowTimerMethod::inputParam(int i, void *param)
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
		m_this = (crHypertextWidgetNode*)param;
		break;
	}
}
void crJXJConsortiaPromotionCanvasShowTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJConsortiaPromotionCanvasShowTimerMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if(canvas.valid() && canvas->getDataClass())
		{
			m_this->setAlignment(CRText::crText::CENTER_BASE_LINE);
			void *param;
			ref_ptr<crData> canvasData = canvas->getDataClass();
			canvasData->getParam(WCHDATA_JXJConsortiaPromotionTimer,param);
			int timer = *(int *)param;
			if(timer>0)
			{
				timer--;
			}
			else
			{
				timer = 0;
			}
			int _hour = timer / 3600;
			int _min = (timer % 3600) / 60;
			int _sec = timer % 60;
			std::string remaining;
			if (_sec >= 0)
			{
				remaining =  crArgumentParser::appItoa(_hour) 
					+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
			}
			else
			{
				remaining = "0:00:00";
			}

			m_this->clear();
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content",1);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",remaining);
			cfg_script.Pop();

			cfg_script.Pop();
			m_this->setHypertext(cfg_script);
			canvasData->inputParam(WCHDATA_JXJConsortiaPromotionTimer,&timer);
		}
	}
}
/////////////////////////////////////////////////////
//
//crJXJCheckManorSceneCouldGetAwardMethod
//检查是否有可领取的俸禄，赋税，免费刷将等奖励
////////////////////////////////////////////////////
crJXJCheckManorSceneCouldGetAwardMethod::crJXJCheckManorSceneCouldGetAwardMethod():
	m_this(NULL)
{
}
crJXJCheckManorSceneCouldGetAwardMethod::crJXJCheckManorSceneCouldGetAwardMethod(const crJXJCheckManorSceneCouldGetAwardMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}

void crJXJCheckManorSceneCouldGetAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode *)param;
		break;
	default:
		break;
	}
}
void crJXJCheckManorSceneCouldGetAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
	case 2:
	case 3:
		m_textInfo[i-1] = str;
		break;
	case 4:
		m_canvasChildNode = str;
		break;
	default:
		break;
	}
}
void crJXJCheckManorSceneCouldGetAwardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && myPlayer)
	{
		ref_ptr<crData> pdata = myPlayer->getDataClass();
		ref_ptr<crRole> mainRole = myPlayer->getMainRole();
		ref_ptr<crData> roleData = mainRole->getDataClass();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (pdata.valid() && roleData.valid() && canvas.valid())
		{
			void *param;
			roleData->getParam(WCHDATA_Level, param);
			unsigned char roleLevel = *(unsigned char *)param;
			unsigned char functionOpenLevel = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAwardTipsOpenLevel,0).c_str()));
			ref_ptr<crNode> canvasChildNode = dynamic_cast<crNode *>(canvas->getChildNode(m_canvasChildNode));
			if (roleLevel < functionOpenLevel && canvasChildNode.valid())
			{//设置功能开放等级限制
				canvasChildNode->setVisiable(false);
				return;
			}
			bool couldGet = true;
			unsigned char textLineNum = 0;
			ref_ptr<crHypertextWidgetNode> textInfo[3];
			for (int i = 0; i < 3; ++i)
			{
				textInfo[i] = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_textInfo[i]));
			}
			ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			std::vector<std::string> textInfoVec;
			int noticeId = 7031;
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(noticeId),record) >= 0)
			{
				crArgumentParser::appAtoVec(record[1], textInfoVec);
			}
			if (textInfoVec.size() >= 4 && textInfo[0].valid() && textInfo[1].valid() && textInfo[2].valid())
			{
				for (int i = 0; i < 3; i++)
				{
					textInfo[i]->clearText();
					//std::vector<float>v_i;
					//ItemMap itemMap;
					//rcfg::ConfigScript cfg_script(&itemMap);
					//cfg_script.Add("Hypertext");
					//cfg_script.Push("Hypertext");

					//cfg_script.Add("Content");
					//cfg_script.Push("Content",1);
					//v_i.clear();
					//v_i.push_back(255.0f);
					//v_i.push_back(255.0f);
					//v_i.push_back(255.0f);
					//v_i.push_back(255.0f);
					//cfg_script.Add("Color",v_i);
					//cfg_script.Add("Text","");
					//cfg_script.Pop();

					//cfg_script.Pop();
					//textInfo[i]->setHypertext(cfg_script);
				}
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
				pdata->getParam(WCHDATA_JXJCouldGetAwardMap, param);
				CouldGetAwardMap * couldGetAwardMap = (CouldGetAwardMap *)param;

				pdata->getParam(WCHDATA_JXJLingqufengluFlag, param);
				unsigned char flag = *(unsigned char *)param;
				if (flag == 0)
				{//检查俸禄领取状态
					unsigned short AwardType = CouldGetAward_Salary;
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
					//设置俸禄领取信息 优先级为1
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
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",textInfoVec[0]);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(0.0f);
					v_i.push_back(153.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text", "领取");
					cfg_script.Pop();

					cfg_script.Pop();
					textInfo[0]->setHypertext(cfg_script);
					++textLineNum;
				}
				else
				{
					unsigned short AwardType = CouldGetAward_Salary;
					CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
					if (awItr != couldGetAwardMap->end())
					{
						couldGetAwardMap->erase(awItr);
					}
				}
				pdata->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				pdata->getParam(WCHDATA_JXJLevyCount,param);
				unsigned char levyCount = *(unsigned char *)param;
				crVector2i maxlevycount;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,viplv).c_str(),maxlevycount);
				if(levyCount > maxlevycount[LevyType_Gold])
				{//检查赋税征收状态
					unsigned short AwardType = CouldGetAward_Taxes;
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
					//设置赋税领取信息 优先级为2
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
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",textInfoVec[1]);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(0.0f);
					v_i.push_back(153.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text", "领取");
					cfg_script.Pop();

					cfg_script.Pop();
					textInfo[textLineNum]->setHypertext(cfg_script);
					++textLineNum;
				}
				else
				{
					unsigned short AwardType = CouldGetAward_Taxes;
					CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
					if (awItr != couldGetAwardMap->end())
					{
						couldGetAwardMap->erase(awItr);
					}
				}
				if (couldGetAwardMap->find(CouldGetAward_SmallManager) != couldGetAwardMap->end())
				{//设置小管家奖励领取信息 优先级为3
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
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",textInfoVec[2]);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(0.0f);
					v_i.push_back(153.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text", "领取");
					cfg_script.Pop();

					cfg_script.Pop();
					textInfo[textLineNum]->setHypertext(cfg_script);
					++textLineNum;
				}
				pdata->getParam(WCHDATA_JXJRollCardFreeCount,param);
				unsigned short freecount = *(unsigned short *)param;
				if(freecount >= 100)
				{//检查免费抽奖次数
					unsigned short AwardType = CouldGetAward_RollCard;
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
					if (textLineNum < 3)
					{//设置免费刷将提示信息 优先级为4
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
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text",textInfoVec[3]);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(0.0f);
						v_i.push_back(153.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text", "刷将");
						cfg_script.Pop();

						cfg_script.Pop();
						textInfo[textLineNum]->setHypertext(cfg_script);
						++textLineNum;
					}
				}
				else
				{
					unsigned short AwardType = CouldGetAward_RollCard;
					CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
					if (awItr != couldGetAwardMap->end())
					{
						couldGetAwardMap->erase(awItr);
					}
				}
				pdata->getParam(WCHDATA_JXJFreeBuyInfo,param);
				unsigned char freeInfo = *(unsigned char*)param;
				if (freeInfo == 0)
				{//检查免费市集够买次数
					unsigned short AwardType = CouldGetAward_BuyResource;
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
					if (textLineNum < 3)
					{//设置免费购买提示信息 优先级为5
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
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text",textInfoVec[4]);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(0.0f);
						v_i.push_back(153.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text", "购买");
						cfg_script.Pop();

						cfg_script.Pop();
						textInfo[textLineNum]->setHypertext(cfg_script);
					}
				} 
				else
				{
					unsigned short AwardType = CouldGetAward_BuyResource;
					CouldGetAwardMap::iterator awItr = couldGetAwardMap->find(AwardType);
					if (awItr != couldGetAwardMap->end())
					{
						couldGetAwardMap->erase(awItr);
					}
				}
				pdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
				if (canvasChildNode.valid())
				{
					if (couldGetAwardMap->empty())
					{
						canvasChildNode->setVisiable(false);
					}
					else
					{
						canvasChildNode->setVisiable(true);
					}
				}
			}
		}
	}
}
////////////////////////////////////////////////////
//
//crJXJShowRelationAwardCanvasMethod
//打开俸禄，赋税，活跃度，免费刷将窗口
////////////////////////////////////////////////////
crJXJShowRelationAwardCanvasMethod::crJXJShowRelationAwardCanvasMethod():
	m_this(NULL),
	m_itemId(0),
	m_itemIdshiji(0)
{
}
crJXJShowRelationAwardCanvasMethod::crJXJShowRelationAwardCanvasMethod(const crJXJShowRelationAwardCanvasMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}

void crJXJShowRelationAwardCanvasMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode *)param;
		break;
	default:
		break;
	}
}

void crJXJShowRelationAwardCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		m_canvas[i-1] = str;
		break;
	case 5:
		m_itemId = atoi(str.c_str());
		break;
	case 6:
		m_itemIdshiji = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJShowRelationAwardCanvasMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && myPlayer)
	{
		ref_ptr<crData> pdata = myPlayer->getDataClass();
		ref_ptr<crCanvasNode> canvas[4];
		for (int i = 0; i < 4; i++)
		{
			canvas[i] = crFilterRenderManager::getInstance()->findCanvas(m_canvas[i]);
		}
		if (pdata.valid())
		{
			void *param;
			bool queryResult[5];
			unsigned char mapSize = 0;
			pdata->getParam(WCHDATA_JXJCouldGetAwardMap, param);
			CouldGetAwardMap &couldGetAwardMap = *(CouldGetAwardMap *)param;
			if (!couldGetAwardMap.empty())
			{
				mapSize = couldGetAwardMap.size();
				if (m_index < mapSize)
				{
					int j = 0;
					CouldGetAwardMap::iterator iter=couldGetAwardMap.begin();
					for(; j < m_index; j++)
						iter++;

					if (iter->first == CouldGetAward_BuyResource)
					{
						//市集窗口不能直接用showCanvas打开，应调用JXJTouchItem
						CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
						if(scene)
						{
							crInstanceItem *item = scene->findSceneItem(m_itemIdshiji);
							if(item)
							{
								item->doEvent(WCH_Touch);
							}
						}
						return;
					}
				}

				for (int i = CouldGetAward_Salary; i < 6; i++)
				{//i对应可领取奖励的四个宏名
					queryResult[i-1] = couldGetAwardMap.find(i) != couldGetAwardMap.end();
				}
				switch (m_index)
				{
				case 0:  //根据窗口优先级显示
					if (queryResult[0])
					{
						if (canvas[0].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[0].get(), true);
						}
					}
					else if (queryResult[1])
					{
						if (canvas[1].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[1].get(), true);
						}
					}
					else if (queryResult[2])
					{
						if (canvas[2].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[2].get(), true);
						}
					}
					else if (queryResult[3])
					{//酒肆窗口不能直接用showCanvas打开，应调用JXJTouchItem
						CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
						if(scene)
						{
							crInstanceItem *item = scene->findSceneItem(m_itemId);
							if(item)
							{
								item->doEvent(WCH_Touch);
							}
						}
					}
					break;
				case 1:  //根据窗口优先级显示
					if (queryResult[1] && queryResult[0])
					{
						if (canvas[1].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[1].get(), true);
						}
					}
					else if (queryResult[2] && (queryResult[1] || queryResult[0]))
					{
						if (canvas[2].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[2].get(), true);
						}
					}
					else if (queryResult[3] && mapSize >= 2)
					{//酒肆窗口不能直接用showCanvas打开，应调用JXJTouchItem
						CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
						if(scene)
						{
							crInstanceItem *item = scene->findSceneItem(m_itemId);
							if(item)
							{
								item->doEvent(WCH_Touch);
							}
						}
					}
					break;
				case 2:  //根据窗口优先级显示
					if (queryResult[2] && queryResult[1] && queryResult[0])
					{//小管家窗口
						if (canvas[2].valid())
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas[2].get(), true);
						}
					}
					else if (queryResult[3] && mapSize >= 3)
					{//酒肆窗口不能直接用showCanvas打开，应调用JXJTouchItem
						CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
						if(scene)
						{
							crInstanceItem *item = scene->findSceneItem(m_itemId);
							if(item)
							{
								item->doEvent(WCH_Touch);
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
}
///////////////////////////////////////////////
//
//crJXJBackCityButtonCoolTimerUpdateMethod
//回城按钮冷却时间检测
//////////////////////////////////////////////
crJXJBackCityButtonCoolTimerUpdateMethod::crJXJBackCityButtonCoolTimerUpdateMethod():
	m_this(NULL){}
crJXJBackCityButtonCoolTimerUpdateMethod::crJXJBackCityButtonCoolTimerUpdateMethod(const crJXJBackCityButtonCoolTimerUpdateMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_backCityButton(handle.m_backCityButton),
	m_buttonSequence(handle.m_buttonSequence),
	m_timeText(handle.m_timeText)
{
}
void crJXJBackCityButtonCoolTimerUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode *)param;
		break;
	default:
		break;
	}	
}
void crJXJBackCityButtonCoolTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_backCityButton = str;
		break;
	case 2:
		m_buttonSequence = str;
		break;
	case 3:
		m_timeText = str;
		break;
	default:
		break;
	}
}
void crJXJBackCityButtonCoolTimerUpdateMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (m_this && myPlayer && canvas.valid() && canvas->getDataClass())
	{
		ref_ptr<crButtonWidgetNode> backCityButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_backCityButton));
		ref_ptr<crSequence>  buttonSequence = dynamic_cast<crSequence*>(canvas->getChildNode(m_buttonSequence));
		ref_ptr<crStaticTextWidgetNode> timeText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_timeText));
		if (backCityButton.valid())
		{
			void *param;
			ref_ptr<crData> canvasData = canvas->getDataClass();
			if (canvasData.valid())
			{
				time_t nowTime_t = time(0);
				struct  tm  nowTm = *localtime(&nowTime_t);
				canvasData->getParam(WCHDATA_JXJPlayerBackCityCoolTimer, param);
				int lastTimer = *(int *)param;
				int cdTimer = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBackCityCoolTimeInGlobalTab,0).c_str()));
				float dt = nowTime_t - lastTimer;
				if (dt < cdTimer)
				{
					backCityButton->setEnable(false);
					if (buttonSequence.valid())
					{
						crSequence::LoopMode loopmode;
						int begin;
						int end;
						int frameCount;
						if(dt<cdTimer-1.0f && buttonSequence->getMode() == crSequence::STOP)
						{
							buttonSequence->getInterval(loopmode,begin,end,frameCount);
							buttonSequence->setMode(crSequence::START);
							buttonSequence->setDuration((cdTimer-dt)/(float)frameCount,1);
						}
						int timer =(int)(cdTimer-dt);
						if(timer>=0 && buttonSequence->getMode() == crSequence::START)
						{
							buttonSequence->setVisiable(true);
						}
						else
						{
							buttonSequence->setVisiable(false);
						}
						if (timeText.valid())
						{
							timeText->setString(crArgumentParser::appItoa(timer));
							timeText->setAlignment(CRText::crText::CENTER_BASE_LINE);
						}
					}
				}
				else
				{
					backCityButton->setEnable(true);
					if(buttonSequence.valid() && buttonSequence->getMode() == crSequence::START)
					{
						buttonSequence->stopEffect();
					}
					if (timeText.valid())
					{
						timeText->setString(L"");
					}
				}
			}
			//当前部队全部死亡时 禁用回程按钮
			bool enable = false;
			crMyPlayerData::getInstance()->lockMyRoleNpcMap();
			crRole *role;
			crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
			for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
				itr != myRoles.end();
				++itr)
			{
				role = itr->second.first.get();
				if (role && role->getDataClass())
				{
					crData *roleData = role->getDataClass();
					roleData->getParam(WCHDATA_ItemState, param);
					unsigned char itemState = *(unsigned char *)param;
					if (itemState != IS_Dead)
					{//有一支部队存活 回程按钮就可用
						enable = true;
						break;
					}
				}
			}
			backCityButton->setEnable(enable);
			crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerTransportationMethod
//回城技能使用
//////////////////////////////////////////////////////////////////////////
crJXJPlayerTransportationMethod::crJXJPlayerTransportationMethod(){}
crJXJPlayerTransportationMethod::crJXJPlayerTransportationMethod(const crJXJPlayerTransportationMethod& handle):
	crMethod(handle),
	m_abstractId(0),
	m_canvas(handle.m_canvas)
{
}
void crJXJPlayerTransportationMethod::inputParam(int i, void *param)
{
}

void crJXJPlayerTransportationMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_abstractId = atoi(str.c_str());
		break;
	case 1:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void crJXJPlayerTransportationMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(camera.valid() && canvas.valid() && canvas->getDataClass())
	{
		void *param;
		bool isSetCDTimer = true;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		unsigned short attackToPos = *(unsigned short *)param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(attackToPos == Cam_Default)
		{
// 			cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
// 			AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
// 			crRole *role = useItemPair->first.get();
			crMyPlayerData::getInstance()->lockMyRoleNpcMap();
			crRole *role;
			crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
			for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
				itr != myRoles.end();
				++itr)
			{
				role = itr->second.first.get();
				if (role && role->getDataClass())
				{
					crData *roleData = role->getDataClass();
					roleData->getParam(WCHDATA_EquipOnMap, param);
					EquipOnMap *equipOnMap = (EquipOnMap *)param;
					crItemChild *itemChild = role->findChildItem(equipOnMap->find(m_abstractId)->second);
					if (itemChild && itemChild->isLoaded())
					{
						crInstanceItem *item = itemChild->getInstanceItem();
						int aboutUseID = item->getInstanceItemID();
						UseItemParam useItemParam;
						useItemParam.m_user = role;
						useItemParam.m_target = NULL;
						unsigned short useResult = UR_None;
						item->doEvent(WCH_UseItemCheck, MAKECREPARAM(&useItemParam, &useResult));
						if (useResult == UR_Succeed)
						{
							unsigned short range = 0;
							crData *itemData = item->getDataClass();
							crVector3 pos;
							crMatrixTransform *bot;
							//直接释放技能
							bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
							if(bot && bot->getVisiable())
							{
								pos = role->getPosition()+role->getDir() * 0.5f;
								unsigned char itemstate = IS_UseItem;
								roleData->inputParam(WCHDATA_ItemState,&itemstate);
								unsigned char targettype = Target_Coord;
								roleData->inputParam(WCHDATA_TargetType,&targettype);							
								roleData->inputParam(WCHDATA_TargetPos,&pos);
								roleData->inputParam(WCHDATA_AboutToUseItemID,&aboutUseID);
							}
							if (isSetCDTimer)
							{//三只部队只需设置一次
								crData *canvasData = canvas->getDataClass();
								if (canvasData)
								{
									time_t nowTime_t = time(0);
									struct  tm  nowTm = *localtime(&nowTime_t);
									canvasData->inputParam(WCHDATA_JXJPlayerBackCityCoolTimer, &nowTime_t);
									isSetCDTimer = false;
								}
							}
						}
					}
				}
			}
			crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBattleTransportationTipMethod
//战场回城tips
/////////////////////////////////////////
crJXJUIBattleTransportationTipMethod::crJXJUIBattleTransportationTipMethod():
	m_str(0)
{
}

crJXJUIBattleTransportationTipMethod::crJXJUIBattleTransportationTipMethod(const crJXJUIBattleTransportationTipMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_tipstext(handle.m_tipstext),
	m_str(handle.m_str)
{
}
void crJXJUIBattleTransportationTipMethod::inputParam(int i, void *param)
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
void crJXJUIBattleTransportationTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_strBt = str;
		break;
	case 1:
		m_tips = str;
		break;
	case 2:
		m_tipstext = str;
		break;
	case 3:
		m_str = atoi(str.c_str());
		break;
	}
}

void crJXJUIBattleTransportationTipMethod::operator()(crHandle &handle)
{
	std::string str;
	crCanvasNode *tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	crCanvasNode *parentCanvas = m_this->getParentCanvas();
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *playerData = myPlayer?myPlayer->getDataClass():NULL;
	if(tips && parentCanvas && playerData)
	{
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		crButtonWidgetNode * bt = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_strBt));
		ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipstext));
		if(tipsText.valid() && bt)
		{
// 			playerData->getParam(WCHDATA_JXJVipLv,param);
// 			unsigned char vipLevel = *(unsigned char *)param;
			int cdTimer = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBackCityCoolTimeInGlobalTab,0).c_str()));
			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			std::string str1,str2;
			if (textTab->queryOneRecord(0, crArgumentParser::appItoa(1130), record) >= 0)
			{
				str1 = record[1];
			}
			if (textTab->queryOneRecord(0, crArgumentParser::appItoa(1131), record) >= 0)
			{
				str2 = record[1];
			}
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
			cfg_script.Add("Text", str1.c_str()/*"自动传送至附近最近据点,冷却"*/);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", crArgumentParser::appItoa(cdTimer));
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 3);
			v_i.clear();
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", str2.c_str()/*"秒"*/);
			cfg_script.Pop();


			cfg_script.Pop();
			tipsText->setHypertext(cfg_script);
			const crBoundingBox &btnbox = bt->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			float posx = btnbox.center().x()-btnbox.xLength()*0.5f+tipsbox.xLength()*0.5 - 0.3;
			float posy = btnbox.center().y()-btnbox.yLength()*0.5 + 0.15;
			tips->setMatrix(crMatrix::translate(posx,posy,0.0f));
			crFilterRenderManager::getInstance()->showCanvas(tips,true);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUICreateGroupCostTipsMethod
//元宝创建军团消费 二次提示Client
//////////////////////////////////////////////////////////////////////////
crJXJUICreateGroupCostTipsMethod::crJXJUICreateGroupCostTipsMethod()
{}
crJXJUICreateGroupCostTipsMethod::crJXJUICreateGroupCostTipsMethod(const crJXJUICreateGroupCostTipsMethod& handle)
{

}
void crJXJUICreateGroupCostTipsMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUICreateGroupCostTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}
void crJXJUICreateGroupCostTipsMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	if(!msgCanvas)return;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int needGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCreateGroupNeedGold,vipLevel).c_str());

			std::string strInfo ;
			ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
			if(msg.valid())
			{
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
				v_i.push_back(140.0);
				v_i.push_back(40.0);
				v_i.push_back(40.0);
				v_i.push_back(255.0);
				cfg_script.Add("Color",v_i);
				strInfo=crArgumentParser::appItoa(needGold) + string("礼金/元宝") ;
				cfg_script.Add("Text",strInfo);
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content",3);
				cfg_script.Add("Text","创建军团吗？");
				cfg_script.Pop();

				cfg_script.Pop();
				msg->setHypertext(cfg_script);

				ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
				crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUISaveEquipRefineTipsMethod
//工坊保存装备精炼属性 二次提示Client
//////////////////////////////////////////////////////////////////////////
crJXJUISaveEquipRefineTipsMethod::crJXJUISaveEquipRefineTipsMethod()
{}
crJXJUISaveEquipRefineTipsMethod::crJXJUISaveEquipRefineTipsMethod(const crJXJUISaveEquipRefineTipsMethod& handle)
{

}
void crJXJUISaveEquipRefineTipsMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUISaveEquipRefineTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}
void crJXJUISaveEquipRefineTipsMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	if(!msgCanvas)return;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int needGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCreateGroupNeedGold,vipLevel).c_str());

			std::string strInfo ;
			ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
			if(msg.valid())
			{
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text","您确认替换当前精炼属性？");
				cfg_script.Pop();

				cfg_script.Pop();
				msg->setHypertext(cfg_script);

				ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
				crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJKickMemberOutofGroupTipsMethod
//移除军团成员 二次提示Client
//////////////////////////////////////////////////////////////////////////
crJXJKickMemberOutofGroupTipsMethod::crJXJKickMemberOutofGroupTipsMethod()
{}
crJXJKickMemberOutofGroupTipsMethod::crJXJKickMemberOutofGroupTipsMethod(const crJXJKickMemberOutofGroupTipsMethod& handle)
{

}
void crJXJKickMemberOutofGroupTipsMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJKickMemberOutofGroupTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}
void crJXJKickMemberOutofGroupTipsMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	if(!msgCanvas)return;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int needGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCreateGroupNeedGold,vipLevel).c_str());

			std::string strInfo ;
			ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
			if(msg.valid())
			{
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text","您确认移除该军团成员？");
				cfg_script.Pop();

				cfg_script.Pop();
				msg->setHypertext(cfg_script);

				ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
				crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJDelayHideWidgetMethod
//控件延时隐藏
//////////////////////////////////////////////////////////////////////////
crJXJDelayHideStaticTextWidgetMethod::crJXJDelayHideStaticTextWidgetMethod():
	m_this(NULL)
{

}
crJXJDelayHideStaticTextWidgetMethod::crJXJDelayHideStaticTextWidgetMethod(const crJXJDelayHideStaticTextWidgetMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_Widget(handle.m_Widget)
{
}
void crJXJDelayHideStaticTextWidgetMethod::inputParam(int i, void *param)
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
void crJXJDelayHideStaticTextWidgetMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_Widget = str;
		break;
	default:
		break;
	}
}
void crJXJDelayHideStaticTextWidgetMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode> textWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Widget));
		if (textWidget.valid())
		{
			void *param;
			crData *braindata = crBrain::getInstance()->getDataClass();
			CRCore::ScopedLock<crData> lock(*braindata);

			braindata->getParam(WCHDATA_JXJDelayHideWidgetTime,param);
			int timer = *(int*)param;
			if(timer<=0)
				textWidget->setVisiable(false);
			else
				timer--;
			braindata->inputParam(WCHDATA_JXJDelayHideWidgetTime,&timer);
		}
	}
}
///////////////////////////////////////////////
//
//crJXJConsortiaCopyBattleProgressCanvasUpdateMethod
//军团副本boss存活时间刷新
//////////////////////////////////////////////
crJXJConsortiaCopyBattleProgressCanvasUpdateMethod::crJXJConsortiaCopyBattleProgressCanvasUpdateMethod():
	m_this(NULL),
	m_limitTimer_1(0),
	m_limitTimer_2(0),
	m_limitBossCircles(0),
	m_totalBossCircles(0),
	m_addCircles(0)
{}
crJXJConsortiaCopyBattleProgressCanvasUpdateMethod::crJXJConsortiaCopyBattleProgressCanvasUpdateMethod(const crJXJConsortiaCopyBattleProgressCanvasUpdateMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_remainTimer(handle.m_remainTimer),
	m_limitTimer_1(handle.m_limitTimer_1),
	m_limitTimer_2(handle.m_limitTimer_1),
	m_limitBossCircles(handle.m_limitBossCircles),
	m_totalBossCircles(handle.m_totalBossCircles),
	m_variableName(handle.m_variableName),
	m_variableName_2(handle.m_variableName_2),
	m_addCircles(handle.m_addCircles)
{
}
void crJXJConsortiaCopyBattleProgressCanvasUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	default:
		break;
	}	
}
void crJXJConsortiaCopyBattleProgressCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_remainTimer = str;
		break;
	case 2:
		m_limitTimer_1 = atoi(str.c_str());
		break;
	case 3:
		m_limitTimer_2 = atoi(str.c_str());
		break;
	case 4:
		m_limitBossCircles = atoi(str.c_str());
		break;
	case 5:
		m_totalBossCircles = atoi(str.c_str());
		break;
	case 6:
		m_variableName = str;
		break;
	case 7:
		m_variableName_2 = str;
		break;
	case 8:
		m_addCircles = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJConsortiaCopyBattleProgressCanvasUpdateMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (m_this && myPlayer && canvas.valid() && canvas->getDataClass())
	{
		ref_ptr<crHypertextWidgetNode> bossRemainTimer = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_remainTimer));
		if (bossRemainTimer.valid())
		{
			unsigned short boshu = 1;
			unsigned short remainTimer = 1;
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
			{
				void *param;
				ref_ptr<crData> data = room->getDataClass();
				if (data.valid())
				{
					data->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->getParam(WCHDATA_JXJScenarioVariableMap,param);
					ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
					ScenarioVariableMap::iterator svmitr = variableMap->find(m_variableName);
					if (svmitr != variableMap->end())
					{
						boshu = (unsigned short)svmitr->second;
					}
					svmitr = variableMap->find(m_variableName_2);
					if (svmitr != variableMap->end())
					{
						remainTimer = (unsigned short)svmitr->second;
					}
					data->excHandle(MAKECREPARAM(WCH_LockData,0));
					if (boshu >= m_totalBossCircles)
					{//最后一波
						std::vector<float>v_i;
						ItemMap itemMap;
						rcfg::ConfigScript cfg_script(&itemMap);
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");

						cfg_script.Add("Content");
						cfg_script.Push("Content");
						cfg_script.Add("Text","可获得胜利");
						cfg_script.Pop();

						cfg_script.Pop();
						bossRemainTimer->setHypertext(cfg_script);
					}
					else
					{
						int nextCircles = 1;//跳至第nextCircles波
						int countdown = 0;//倒计时
						if (boshu > m_limitBossCircles)
						{
							nextCircles = boshu + 1;
							countdown = m_limitTimer_2 - remainTimer;
						}
						else
						{
							if (m_limitTimer_1 - remainTimer > 0)
							{
								nextCircles = boshu + m_addCircles;
								countdown = m_limitTimer_1 - remainTimer;
							}
							else
							{
								nextCircles = boshu + 1;
								countdown = m_limitTimer_2 - remainTimer;
							}
						}
						std::string strInfo ;
						std::vector<float>v_i;
						ItemMap itemMap;
						rcfg::ConfigScript cfg_script(&itemMap);
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");
						cfg_script.Add("Content");
						cfg_script.Push("Content");
						cfg_script.Add("Text","可跳至");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(140.0);
						v_i.push_back(40.0);
						v_i.push_back(40.0);
						v_i.push_back(255.0);
						cfg_script.Add("Color",v_i);
						strInfo=string("第") + crArgumentParser::appItoa(nextCircles) + string("波:") ;
						cfg_script.Add("Text",strInfo);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						v_i.clear();
						v_i.push_back(140.0);
						v_i.push_back(40.0);
						v_i.push_back(40.0);
						v_i.push_back(255.0);
						cfg_script.Add("Color",v_i);
						strInfo=crArgumentParser::appItoa(countdown) + string("s") ;
						cfg_script.Add("Text",strInfo);
						cfg_script.Pop();

						cfg_script.Pop();
						bossRemainTimer->setHypertext(cfg_script);
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJDelayHideSwitchCanvasTipsMethod
//战场界面 Alt+Tab切换提示窗口 显示十秒后自动隐藏(Client)
//////////////////////////////////////////////////////////////////////////
crJXJDelayHideSwitchCanvasTipsMethod::crJXJDelayHideSwitchCanvasTipsMethod():
	m_this(NULL),
	m_delayTimer(0)
{

}
crJXJDelayHideSwitchCanvasTipsMethod::crJXJDelayHideSwitchCanvasTipsMethod(const crJXJDelayHideSwitchCanvasTipsMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_Widget(handle.m_Widget),
	m_delayTimer(handle.m_delayTimer)
{
}
void crJXJDelayHideSwitchCanvasTipsMethod::inputParam(int i, void *param)
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
void crJXJDelayHideSwitchCanvasTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_Widget = str;
		break;
	case 2:
		m_delayTimer = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJDelayHideSwitchCanvasTipsMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crHypertextWidgetNode> textWidget = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_Widget));
		if (textWidget.valid())
		{
			void *param;
			crData *canvasData = canvas->getDataClass();
			if (canvasData)
			{
				canvasData->getParam(WCHDATA_JXJDelayHideCanvasTimer,param);
				int timer = *(int*)param;
				if (timer == 0)
				{
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content");
					cfg_script.Add("Text","按Alt+Tab可以进行窗口切换哟!");
					cfg_script.Pop();

					cfg_script.Pop();
					textWidget->setHypertext(cfg_script);
				}
				if(timer >= m_delayTimer)
				{
					canvasData->inputParam(WCHDATA_JXJDelayHideCanvasTimer, 0); //隐藏窗口之后，重置时间
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(), false);
				}
				else
				{
					canvasData->inputParam(WCHDATA_JXJDelayHideCanvasTimer,&(++timer));
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJAutoShowFestivalHuodongCanvasMethod
//玩家每日首次登录时检测是否有处于激活状态的节日活动，有就自动弹出活动窗口
//////////////////////////////////////////////////////////////////////////
crJXJAutoShowFestivalHuodongCanvasMethod::crJXJAutoShowFestivalHuodongCanvasMethod()
{
}
crJXJAutoShowFestivalHuodongCanvasMethod::crJXJAutoShowFestivalHuodongCanvasMethod(const crJXJAutoShowFestivalHuodongCanvasMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_Widget(handle.m_Widget)
{
}
void crJXJAutoShowFestivalHuodongCanvasMethod::inputParam(int i, void *param)
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
void crJXJAutoShowFestivalHuodongCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_Widget = str;
		break;
	default:
		break;
	}
}
void crJXJAutoShowFestivalHuodongCanvasMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid() && canvas->getDataClass() && myPlayer)
	{
		void* param;
		ref_ptr<crData> roleData = myPlayer->getMainRole()->getDataClass();
		if (roleData.valid())
		{
			roleData->getParam(WCHDATA_Level, param);
			unsigned char roleLevel = *(unsigned char *)param;
			unsigned char needlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAutoShowFestivalCanvasNeedLv,0).c_str());
			if (roleLevel < needlv)
			{
				return;
			}
			bool hadShow = true;
			ref_ptr<crData> playerData = myPlayer->getDataClass();
			if (playerData.valid())
			{
				playerData->getParam(WCHDATA_JXJHadShowFestivalHuodongCanvas, param);
				hadShow = *(bool *)param;
			}
			if (!hadShow)
			{
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
				ref_ptr<crData> canvasData = canvas->getDataClass();
				if (canvasData.valid())
				{
					canvasData->getParam(WCHDATA_JXJisFestivalHuodongExist, param);
					unsigned short festivalHuodongExist = *(unsigned short *)param;
					if (festivalHuodongExist > 0)
					{
						festivalHuodongExist--;
						canvasData->inputParam(WCHDATA_JXJHuodongSelectId, &festivalHuodongExist);
						hadShow = true;
						playerData->inputParam(WCHDATA_JXJHadShowFestivalHuodongCanvas, &hadShow);
						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if (netConductor)
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeUChar(2);//1表示同步新手引导图标显示状态，2表示同步是否已经自动弹出过节日活动窗口
							stream->_writeBool(hadShow);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHideGameIntroductionIcon,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
						}
						canvas->doEvent(WCH_UI_UpdateData);
					}
					else
					{
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(), false);
					}
				}
				else
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(), false);
				}
			}
		}		
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvCheckFestivalActivityCompleteMethod
//检查一组节日活动下的三个活动是否都完成
//////////////////////////////////////////////////////////////////////////
JXJ::crJXJRecvCheckFestivalActivityCompleteMethod::crJXJRecvCheckFestivalActivityCompleteMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvCheckFestivalActivityCompleteMethod::crJXJRecvCheckFestivalActivityCompleteMethod( const crJXJRecvCheckFestivalActivityCompleteMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_canvas_sw(handle.m_canvas_sw)
{

}

void JXJ::crJXJRecvCheckFestivalActivityCompleteMethod::inputParam( int i, void *param )
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvCheckFestivalActivityCompleteMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_canvas_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvCheckFestivalActivityCompleteMethod::operator()( crHandle &handle )
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
				bool isAllComplete = true;
				unsigned short huodongID = m_stream->_readShort();

				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;
				int displayIndex = huodongTab->getTitleIndex("活动显示");
				int count = 4;
				if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
				{
					if (atoi(record[displayIndex].c_str()) == Huodong_Festival)
					{
						count = 4;
					}
					else if (atoi(record[displayIndex].c_str()) == Huodong_Qiangguo)
					{
						count = 5;
					}
				}
				void *param;
				crData *data = mainRole->getDataClass();
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;

				for (int i = 1; i < count; ++i)
				{
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID + i);
					if(itr != huodongMap->end())
					{
						if(!itr->second->getComplete())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID+i),record) >= 0)
							{
								isAllComplete = false;
								break;
							}
						}
					}
				}
				if (isAllComplete)
				{
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
					if(itr != huodongMap->end())
					{
						if(itr->second->isCanAdvance() && !itr->second->getComplete())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
							{
								itr->second->setActivation(2,playerid);
							}
						}
					}
					//crPlayerDataEventPacket packet;
					//crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvCheckFestivalActivityComplete,NULL);
					//gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

					char gbuf[256];
					sprintf(gbuf,"%s 该组节日活动奖励 活动ID %d 是否全部领取 %d\0",playerData->getCharacterName().c_str(),huodongID,isAllComplete);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
				}
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		//else if(m_netType == GameClient_Game)
		//{
		//	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		//	if(canvas.valid())
		//	{
		//		ref_ptr<crMultiSwitch> canvas_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_canvas_sw));
		//		if (canvas_sw.valid())
		//		{
		//			canvas_sw->setActiveSwitchSet(1);
		//		}
		//	}
		//}
	}
}