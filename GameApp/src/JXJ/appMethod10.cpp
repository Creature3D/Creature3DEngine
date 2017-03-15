/**********************************************************************
*
*	文件:	   appMethod10.cpp
*
*	描述:	   
*
*	作者:	   刘超
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod10.h>
#include <JXJ/appMethod2.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMethod16.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <CRUI/crWidgetExtend.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRIOManager/crConvertUTF.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crReadFile.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <sys/stat.h>
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
//crJXJUIChooseCheckBoxMethod
//
/////////////////////////////////////////
crJXJUIChooseCheckBoxMethod::crJXJUIChooseCheckBoxMethod():
	m_this(NULL)
{
}
crJXJUIChooseCheckBoxMethod::crJXJUIChooseCheckBoxMethod(const crJXJUIChooseCheckBoxMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_Recommend(handle.m_Recommend),
	m_Recommend_sw(handle.m_Recommend_sw)
{
	for (int i = 0;i<15 ;i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
		m_checkbox_sw[i] = handle.m_checkbox_sw[i];
	}
}
void crJXJUIChooseCheckBoxMethod::inputParam(int i, void *param)
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
void crJXJUIChooseCheckBoxMethod::addParam(int i, const std::string& str)
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
		m_checkbox[i] = str;
		break;
	case 11:
		m_checkbox[i] = str;
		break;
	case 12:
		m_checkbox[i] = str;
		break;
	case 13:
		m_checkbox[i] = str;
		break;
	case 14:
		m_checkbox[i] = str;
		break;
	case 15:
		m_Recommend = str;
		break;
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
	case 27:
	case 28:
	case 29:
	case 30:
		m_checkbox_sw[i - 16] = str;
		break;
	case 31:
		m_Recommend_sw = str;
		break;
	default:
		break;
	}
}
void crJXJUIChooseCheckBoxMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			int iCheckedNumber = 0;
			ref_ptr<crButtonWidgetNode>  recommend = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Recommend));
			ref_ptr<crCheckBoxWidgetNode> foldbtn[15] = {NULL};
			crMultiSwitch *foldbtn_sw[15] = {NULL};
			crMultiSwitch *recommend_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_Recommend_sw));
			for (int i = 0; i<15;i++)
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
			for (int i = 0; i < 15; i++)
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
		}
	}
}

/////////////////////////////////////////
//
//crJXJCountryRadioAddZhouJunNameMethod
//
/////////////////////////////////////////
crJXJCountryRadioAddZhouJunNameMethod::crJXJCountryRadioAddZhouJunNameMethod():
	m_this(NULL)
{
}
crJXJCountryRadioAddZhouJunNameMethod::crJXJCountryRadioAddZhouJunNameMethod(const crJXJCountryRadioAddZhouJunNameMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_radio(handle.m_radio)
{
	for (int i = 0; i < 20; i++)
	{
		m_zhoujunname[i] = handle.m_zhoujunname[i];
	}
}
void crJXJCountryRadioAddZhouJunNameMethod::inputParam(int i, void *param)
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
void crJXJCountryRadioAddZhouJunNameMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radio = str;
		break;
	case 1:
		m_zhoujunname[i-1] = str;
		break;
	case 2:
		m_zhoujunname[i-1] = str;
		break;
	case 3:
		m_zhoujunname[i-1] = str;
		break;
	case 4:
		m_zhoujunname[i-1] = str;
		break;
	case 5:
		m_zhoujunname[i-1] = str;
		break;
	case 6:
		m_zhoujunname[i-1] = str;
		break;
	case 7:
		m_zhoujunname[i-1] = str;
		break;
	case 8:
		m_zhoujunname[i-1] = str;
		break;
	case 9:
		m_zhoujunname[i-1] = str;
		break;
	case 10:
		m_zhoujunname[i-1] = str;
		break;
	case 11:
		m_zhoujunname[i-1] = str;
		break;
	case 12:
		m_zhoujunname[i-1] = str;
		break;
	case 13:
		m_zhoujunname[i-1] = str;
		break;
	case 14:
		m_zhoujunname[i-1] = str;
		break;
	case 15:
		m_zhoujunname[i-1] = str;
		break;
	case 16:
		m_zhoujunname[i-1] = str;
		break;
	case 17:
		m_zhoujunname[i-1] = str;
		break;
	case 18:
		m_zhoujunname[i-1] = str;
		break;
	case 19:
		m_zhoujunname[i-1] = str;
		break;
	case 20:
		m_zhoujunname[i-1] = str;
		break;
	default:
		break;
	}
}
void crJXJCountryRadioAddZhouJunNameMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crRadioGroupWidgetNode> radiogroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radio));
		if (radiogroup.valid())
		{
			radiogroup->clear();
			for (int i = 0; i < 20; i++)
			{
				radiogroup->addRadioName(m_zhoujunname[i]);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCountryZhouJunUpdateMethod
//
/////////////////////////////////////////
crJXJCountryCantonFenFengUpdateMethod::crJXJCountryCantonFenFengUpdateMethod():
	m_this(NULL)
{
}

crJXJCountryCantonFenFengUpdateMethod::crJXJCountryCantonFenFengUpdateMethod(const crJXJCountryCantonFenFengUpdateMethod& handle):
	crMethod(handle),
	m_tableWidget(handle.m_tableWidget),
	m_countryCanvas(handle.m_countryCanvas),
	m_pageNum(handle.m_pageNum),
	m_notice(handle.m_notice ),
	m_title(handle.m_title)
{
	for (unsigned char i = 0; i < 9; ++ i)
	{
		m_info[0][i] = handle.m_info[0][i];
		m_info[1][i] = handle.m_info[1][i];
		m_fenfeng[i] = handle.m_fenfeng[i];
		m_jiechu[i] = handle.m_jiechu[i];
		m_fenfeng_sw[i] = handle.m_fenfeng_sw[i];
		m_jiechu_sw[i] = handle.m_jiechu_sw[i];
	}
}

void crJXJCountryCantonFenFengUpdateMethod::inputParam(int i, void *param)
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

void crJXJCountryCantonFenFengUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tableWidget = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_info[0][i - 1] = str;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		m_info[1][i - 10] = str;
		break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
		m_fenfeng[i - 19] = str;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		m_jiechu[i - 28] = str;
		break;
	case 37:
		m_countryCanvas = str;
		break;
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
		m_fenfeng_sw[i - 38] = str;
		break;
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
		m_jiechu_sw[i - 47] = str;
		break;
	case 56:
		m_pageNum = str;
		break;
	case 57:
		m_notice = str;
		break;
	case 58:
		m_title = str;
		break;
	default:
		break;
	}
}

void crJXJCountryCantonFenFengUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  countryCanvas = crFilterRenderManager::getInstance()->findCanvas(m_countryCanvas);
	do 
	{
		if ( !m_this || !countryCanvas || countryCanvas->getDataClass() == NULL)
			break;
		crTableWidgetNode	* groupList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_tableWidget)); 
		if (!groupList) break;

		ref_ptr<crStaticTextWidgetNode>  info[2][9] = {NULL};
		ref_ptr<crButtonWidgetNode>  fenfeng[9] = {NULL};
		ref_ptr<crButtonWidgetNode>  jiechu[9] = {NULL};
		crMultiSwitch * fenfeng_sw[9] = {NULL};
		crMultiSwitch * jiechu_sw[9] = {NULL};

		for (unsigned char i = 0; i < 9; ++ i)
		{
			info[0][i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_info[0][i]));
			info[1][i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_info[1][i]));
			info[0][i]->clearString();
			info[1][i]->clearString();
			fenfeng[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_fenfeng[i]));
			jiechu[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_jiechu[i]));
			fenfeng_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_fenfeng_sw[i]));
			jiechu_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_jiechu_sw[i]));
		}

		ref_ptr<crStaticTextWidgetNode>  pageinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageNum));
		ref_ptr<crStaticTextWidgetNode>  notice = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_notice));
		ref_ptr<crStaticTextWidgetNode>  title = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title));

		ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
		crTableIO::StrVec record;
		int nameIdx = 0;
		nameIdx = cantontab->getTitleIndex("名称");
		
		void *param;
		crData * myCountryCanvasData = countryCanvas->getDataClass();
		myCountryCanvasData->getParam(WCHDATA_JXJCountryCanvasAllocateGroupVec,param);
		CountryCanvasAllocateGroupVec * groupVec = (CountryCanvasAllocateGroupVec *)param;

		myCountryCanvasData->getParam(WCHDATA_JXJCountryCanvasSelectedCantonId,param);
		int cantonId = *(int *)param;

		myCountryCanvasData->getParam(WCHDATA_JXJIsSelectCabtonFenFenged,param);
		bool isfenfenged = *(bool *)param;

		if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(cantonId),record)>=0)
		{
			if (title.valid()) title->setString("待分封的州郡："+record[nameIdx]);
		}

		myCountryCanvasData->getParam(WCHDATA_JXJCountryCanvasFenFengCurPage	,param);
		int curPage = *(int *)param;
		myCountryCanvasData->getParam(WCHDATA_JXJCountryCanvasFenFengTotalPage,param);
		int totalPage = *(int *)param;
		if (pageinput.valid())
		{
			pageinput->setString(crArgumentParser::appItoa(curPage) + "/" + crArgumentParser::appItoa(totalPage));
		}

		unsigned char showJiechuIdx = 10;
		unsigned char willGray[9] ;			// -1 表示都不显示，0 表示按钮不用变灰色，1变成灰色
		memset(willGray,-1,sizeof(willGray));

		groupList->clearData();
		CountryCanvasAllocateGroupVec::iterator gmItr = groupVec->begin();
		for (unsigned char i = 0;i < 9 && gmItr != groupVec->end(); ++ gmItr)
		{
			do 
			{
				if (gmItr->valid() == false) break;
				groupList->addData(0,i,crArgumentParser::appItoa( (curPage - 1) * 9 + i));
				groupList->addData(1,i,(*gmItr)->m_groupName);
				groupList->addData(2,i,(*gmItr)->m_leaderName);
				groupList->addData(3,i,crArgumentParser::appItoa((*gmItr)->m_lv));
				groupList->addData(4,i,crArgumentParser::appItoa((*gmItr)->m_memberCount));
				groupList->addData(5,i,crArgumentParser::appItoa((*gmItr)->m_exploit));
				groupList->addData(i,(*gmItr)->m_id);

				FenFengedCantonSet::iterator fsItr = (*gmItr)->m_fenFengedCantonSet.begin();
				unsigned char j = 0;
				for (; j < 2 && fsItr != (*gmItr)->m_fenFengedCantonSet.end(); ++ fsItr)
				{
					if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(*fsItr),record)>=0)
					{
						if (info[j][i].valid()) info[j][i]->setString(record[nameIdx]);
						if (cantonId == *fsItr) showJiechuIdx = i;
						++ j;
					}
				}
				willGray[i] = j == 2 ? 1 : 0;

				++ i;
			} while (0);
		}
		for (unsigned char i = 0; i < 9;  ++ i)
		{
			if (willGray[i] != (unsigned char )-1)
			{
				if (showJiechuIdx == 10)
				{
					if (isfenfenged)
					{
						if (jiechu[i].valid()) 
						{
							jiechu[i]->setEnable(false);
							jiechu[i]->setVisiable(false);
						}
						if (fenfeng[i].valid()) 
						{
							fenfeng[i]->setVisiable(true);
							fenfeng[i]->setEnable(true);
						}
						//if (willGray[i])
						{
							if (fenfeng_sw[i]) fenfeng_sw[i]->setActiveSwitchSet(3);
							if (fenfeng[i].valid()) fenfeng[i]->setEnable(false);
						}
					}
					else
					{
						if (jiechu[i].valid()) 
						{
							jiechu[i]->setEnable(false);
							jiechu[i]->setVisiable(false);
						}
						if (fenfeng[i].valid()) 
						{
							fenfeng[i]->setVisiable(true);
							fenfeng[i]->setEnable(true);
						}
						if (willGray[i])
						{
							if (fenfeng_sw[i]) fenfeng_sw[i]->setActiveSwitchSet(3);
							if (fenfeng[i].valid()) fenfeng[i]->setEnable(false);
						}
					}
				}
				else
				{
					if (showJiechuIdx == i)
					{
						if (jiechu[i].valid()) 
						{
							jiechu[i]->setVisiable(true);
							jiechu[i]->setEnable(true);
						}
						if (fenfeng[i].valid())
						{
							fenfeng[i]->setVisiable(false);
							fenfeng[i]->setEnable(false);
						}
					}
					else
					{
						if (jiechu[i].valid()) 
						{
							jiechu[i]->setEnable(false);
							jiechu[i]->setVisiable(false);
						}
						if (fenfeng[i].valid()) 
						{
							fenfeng[i]->setVisiable(true);
							fenfeng[i]->setEnable(true);
						}
						//if (willGray[i])
						{
							if (fenfeng_sw[i]) fenfeng_sw[i]->setActiveSwitchSet(3);
							if (fenfeng[i].valid()) fenfeng[i]->setEnable(false);
						}
					}
				}
				info[0][i]->setVisiable(true);
				info[1][i]->setVisiable(true);
			}
			else
			{
				fenfeng[i]->setVisiable(false);
				jiechu[i]->setVisiable(false);
				info[0][i]->setVisiable(false);
				info[1][i]->setVisiable(false);
			}
		}

		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		int mode = 2;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		std::string noticeStr = "";
		int texid = 4007;
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&noticeStr);
		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		if (notice.valid()) notice->setString(noticeStr);

	} while (0);
}
/////////////////////////////////////////
//
//crJXJHistoryInfoChangeCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJHistoryInfoChangeCanvasUpdateMethod::crJXJHistoryInfoChangeCanvasUpdateMethod()
{
}

crJXJHistoryInfoChangeCanvasUpdateMethod::crJXJHistoryInfoChangeCanvasUpdateMethod(const crJXJHistoryInfoChangeCanvasUpdateMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_radio(handle.m_radio),
	m_history(handle.m_history)
{
}

void crJXJHistoryInfoChangeCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJHistoryInfoChangeCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radio = str;
		break;
	case 2:
		m_history = str;
		break;
	default:
		break;
	}
}

void crJXJHistoryInfoChangeCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if (canvas.valid()&&playerGameData&&netConductor)
	{
		ref_ptr<crRadioGroupWidgetNode> radio =  dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio));
		ref_ptr<crHypertextWidgetNode> history = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_history));
		crData *canvasdata = m_this->getDataClass();
		if (radio.valid()&&canvasdata&&history.valid())
		{
			int index = 0;
			if (radio.valid())
			{
				index = radio->getSelect();
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJBuildInfoChangeCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJBuildInfoChangeCanvasUpdateMethod::crJXJBuildInfoChangeCanvasUpdateMethod()
{
}

crJXJBuildInfoChangeCanvasUpdateMethod::crJXJBuildInfoChangeCanvasUpdateMethod(const crJXJBuildInfoChangeCanvasUpdateMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_radio(handle.m_radio),
	m_explain(handle.m_explain),
	m_percent(handle.m_percent),
	m_buttonBuiding(handle.m_buttonBuiding),
	m_image_sw(handle.m_image_sw)
{
}

void crJXJBuildInfoChangeCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJBuildInfoChangeCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radio = str;
		break;
	case 2:
		m_explain = str;
		break;
	case 3:
		m_percent = str;
		break;
	case 4:
		m_buttonBuiding = str;
		break;
	case 5:
		m_image_sw = str;
		break;
	default:
		break;
	}
}

void crJXJBuildInfoChangeCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if (canvas.valid()&&playerGameData&&netConductor)
	{
		ref_ptr<crRadioGroupWidgetNode> radio =  dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio));
		ref_ptr<crMultiSwitch > image_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_image_sw));

		crData *canvasdata = canvas->getDataClass();
		crData *data = playerGameData->getDataClass();
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (radio.valid()&&canvasdata&&data&&braindata)
		{
			int select = radio->getSelect();
			image_sw->setActiveSwitchSet(select);
			ref_ptr<crStaticTextWidgetNode> explain = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_explain));
			ref_ptr<crStaticTextWidgetNode> percent = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_percent));
			ref_ptr<crButtonWidgetNode> buildbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_buttonBuiding));

			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			std::string buildinfo[5];
			int mode = 2;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			for (int i = 4056,j = 0; i <= 4060; ++ i,++ j)
			{
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&i);
				noticeHandle->inputParam(WCHDATA_NoticeOutPut,&buildinfo[j]);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			}
		
			//std::string buildinfo[5] = {"金是矿石的意思，乱世之中，军备格外重要，矿石更是一等一的重要物资。国家为此专设司金台，管理铁矿开采提炼等事，通过严格的规章制度与统一调度，可以提高铁矿产量。",
			//	"神农尝百草，选出五谷以供民食，制作类似，方便耕耘。因此建神农庙祭祀神农，为国家大事。仿佛冥冥之中神农也保佑着华夏子民，凡是为建立神农庙提供帮助的人，都可以提高粮食产量。",
			//	"有感于国家马匹数量过少，国家专门设立用于培养优良战马的机构，雇佣经验丰富的马夫，采取最先进的养马技术，因而可以一定程度上提升马匹的产量。",
			//	"上林苑最开始虽然是皇家园林，但身处乱世，国家风雨飘摇，一切都是为了战争服务。故而上林苑中种类繁多的树木优先供给军中，极大的提高了木材的产量。",
			//	"太庙是祭拜祖先的地方，军队出战前来此祭拜，以求祖先庇护。太庙的规模越大，越显得王权昭彰，百姓踊跃参军，故而可以减少征兵消耗的资源与财帛。"};
			std::string percentinfo[5] ;

			void *param;
			int gongxian[5];
			unsigned char j = 0;
			for ( int i = WCHDATA_JXJShenNongMiao; j < 5 && i <= WCHDATA_JXJTaiMiao; ++ i,++ j)
			{
				canvasdata->getParam(i,param);
				gongxian[j] = *(int *)param;
			}
			ref_ptr<crTableIO> countrybuildtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBuildTab);
			crTableIO::StrVec record;
			crTableIO::StrVec record2;
			crTableIO::DataVec records;
			crTableIO::DataVec::iterator itr,peritr;
			//std::string filename[5];
			int moneyneedindex = countrybuildtab->getTitleIndex("铜钱");
			int typeindex = countrybuildtab->getTitleIndex("type");
			int addindex = countrybuildtab->getTitleIndex("lv");
			float progress = 0.0f;
			//unsigned char lv = 0;
			//unsigned char addp = 0;
			float nowcopper;
			float needcopper;
			//data->getParam(WCHDATA_JXJVipLv,param);
			//unsigned char viplv = *(unsigned char *)param;
			//int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJContriCountryOnceCopper,viplv).c_str()));
			//data ->getParam(WCHDATA_JXJCoppercash,param);
			//int mycopper = *(int *)param;
			for (int i = 0; i < 5; i ++)
			{
				progress = 0.0f;
				records.clear();
				countrybuildtab->queryRecords(typeindex,crArgumentParser::appItoa(i),records);
				if(!records.empty())
				{
					unsigned char lv = 0;
					peritr = records.begin();
					itr = peritr;
					++ itr;
					for (; itr != records.end();
						++itr,++peritr)
					{
						if (gongxian[i] <  atoi((*itr)[moneyneedindex].c_str()))
						{
							break;
						}
					}
					lv = atoi((*peritr)[0].c_str());
					if(countrybuildtab->queryOneRecord(0,crArgumentParser::appItoa(lv),record) >= 0 && itr != records.end())
					{
						nowcopper = float(gongxian[i]) - atof(record[moneyneedindex].c_str());
						needcopper = atof((*itr)[moneyneedindex].c_str()) - atof(record[moneyneedindex].c_str());
						if ((int)needcopper <= 0)
						{
							progress = 1.0f;
						}
						else
						{
							progress = nowcopper/needcopper;
						}
					}
					percentinfo[i] = (*peritr)[addindex];
				}
			}
			explain->setString(buildinfo[select]);
			percent->setString(percentinfo[select]);
		}
	}
}

/////////////////////////////////////////
//
//crJXJSelectCountryLeaderMethod
//
/////////////////////////////////////////
crJXJSelectCountryLeaderMethod::crJXJSelectCountryLeaderMethod():
	m_this(NULL)
{
}

crJXJSelectCountryLeaderMethod::crJXJSelectCountryLeaderMethod(const crJXJSelectCountryLeaderMethod& handle):
	crMethod(handle),
	m_Recommend(handle.m_Recommend),
	m_tableWidget(handle.m_tableWidget)
{
	for (int i = 0; i < 15; i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}

void crJXJSelectCountryLeaderMethod::inputParam(int i, void *param)
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

void crJXJSelectCountryLeaderMethod::addParam(int i, const std::string& str)
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
		m_checkbox[i] = str;
		break;
	case 11:
		m_checkbox[i] = str;
		break;
	case 12:
		m_checkbox[i] = str;
		break;
	case 13:
		m_checkbox[i] = str;
		break;
	case 14:
		m_checkbox[i] = str;
		break;
	case 15:
		m_Recommend = str;
		break;
	case 16:
		m_tableWidget = str;
		break;
	default:
		break;
	}
}

void crJXJSelectCountryLeaderMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			typedef std::vector<int> SelectedIdVec;
			SelectedIdVec idVec;
			ref_ptr<crCheckBoxWidgetNode> foldbtn[15] = {NULL};
			ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_tableWidget));
			for (int i = 0; i<15;i++)
			{
				foldbtn[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
				if (foldbtn[i].valid() && foldbtn[i]->getSelect())
				{
					idVec.push_back(tableWidget->getData(i));
				}
			}

			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor && playerGameData)
			{
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(16);
				stream->_writeUChar(idVec.size());
				SelectedIdVec::iterator sivItr = idVec.begin();
				for (; sivItr != idVec.end(); ++ sivItr)
				{
					stream->_writeInt(*sivItr);
				}
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSelectCountryLeader,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJMyCounrtyInfoCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyCounrtyInfoCanvasUpdateMethod::crJXJMyCounrtyInfoCanvasUpdateMethod():
	m_this(NULL)
{
}

crJXJMyCounrtyInfoCanvasUpdateMethod::crJXJMyCounrtyInfoCanvasUpdateMethod(const crJXJMyCounrtyInfoCanvasUpdateMethod& handle):
	crMethod(handle),
	m_countryiconsw(handle.m_countryiconsw),
	m_EditNotice(handle.m_EditNotice),
	m_PublishNotice(handle.m_PublishNotice),
	m_leadername1(handle.m_leadername1),
	m_leadername2(handle.m_leadername2),
	m_leadername3(handle.m_leadername3),
	m_nextopentime(handle.m_nextopentime),
	m_notice(handle.m_notice),
	m_getCityControl(handle.m_getCityControl),
	m_lostCityControl(handle.m_lostCityControl)
{
	for(int i = 0; i < 12; i++)
	{
		m_getcity[i] = handle.m_getcity[i];
		m_getcity_sw[i] = handle.m_getcity_sw[i];
		m_losecity[i] = handle.m_losecity[i];
		m_donghua[i] = handle.m_donghua[i];
	}
}

void crJXJMyCounrtyInfoCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyCounrtyInfoCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_countryiconsw = str;
		break;
	case 1:
		m_leadername1 = str;
		break;
	case 2:
		m_leadername2 = str;
		break;
	case 3:
		m_leadername3 = str;
		break;
	case 4:
		m_nextopentime = str;
		break;
	case 5:
		m_getcity[0] = str;
		break;
	case 6:
		m_getcity[1] = str;
		break;
	case 7:
		m_getcity[2] = str;
		break;
	case 8:
		m_getcity[3] = str;
		break;
	case 9:
		m_getcity[4] = str;
		break;
	case 10:
		m_getcity[5] = str;
		break;
	case 11:
		m_getcity[6] = str;
		break;
	case 12:
		m_getcity[7] = str;
		break;
	case 13:
		m_getcity[8] = str;
		break;
	case 14:
		m_getcity[9] = str;
		break;
	case 15:
		m_getcity[10] = str;
		break;
	case 16:
		m_getcity[11] = str;
		break;
	case 17:
		m_losecity[0] = str;
		break;
	case 18:
		m_losecity[1] = str;
		break;
	case 19:
		m_losecity[2] = str;
		break;
	case 20:
		m_losecity[3] = str;
		break;
	case 21:
		m_losecity[4] = str;
		break;
	case 22:
		m_losecity[5] = str;
		break;
	case 23:
		m_losecity[6] = str;
		break;
	case 24:
		m_losecity[7] = str;
		break;
	case 25:
		m_losecity[8] = str;
		break;
	case 26:
		m_losecity[9] = str;
		break;
	case 27:
		m_losecity[10] = str;
		break;
	case 28:
		m_losecity[11] = str;
		break;
	case 29:
		m_notice = str;
		break;
	case 30:
		m_donghua[0] = str;
		break;
	case 31:
		m_donghua[1] = str;
		break;
	case 32:
		m_donghua[2] = str;
		break;
	case 33:
		m_donghua[3] = str;
		break;
	case 34:
		m_donghua[4] = str;
		break;
	case 35:
		m_donghua[5] = str;
		break;
	case 36:
		m_donghua[6] = str;
		break;
	case 37:
		m_donghua[7] = str;
		break;
	case 38:
		m_donghua[8] = str;
		break;
	case 39:
		m_donghua[9] = str;
		break;
	case 40:
		m_donghua[10] = str;
		break;
	case 41:
		m_donghua[11] = str;
		break;
	case 42:
		m_getCityControl = str;
		break;
	case 43:
		m_lostCityControl = str;
		break;
	case 44:
		m_EditNotice = str;
		break;
	case 45:
		m_PublishNotice = str;
		break;
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
		m_getcity_sw[i - 46] = str;
		break;
	default:
		break;
	}
}

void crJXJMyCounrtyInfoCanvasUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		ref_ptr<crTableIO> citytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		int citynameindex = citytab->getTitleIndex("名字");

		void *param;
		crData *canvasdata = m_this->getDataClass();
		crMultiSwitch *countryiconsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_countryiconsw));
		ref_ptr<crButtonWidgetNode> noticeeditbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_EditNotice));
		ref_ptr<crButtonWidgetNode> noticepublishbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_PublishNotice));
		ref_ptr<crStaticTextWidgetNode> leadername1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_leadername1));
		ref_ptr<crStaticTextWidgetNode> leadername2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_leadername2));
		ref_ptr<crStaticTextWidgetNode> leadername3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_leadername3));
		ref_ptr<crStaticTextWidgetNode> nextopentime = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_nextopentime));
		ref_ptr<crEditWidgetNode> notice = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_notice));
		ref_ptr<crButtonWidgetNode> getcity[12] = {NULL};
		ref_ptr<crStaticTextWidgetNode> losecity[12] = {NULL};
		crMultiSwitch *getcity_sw[12] = {NULL};
		crNode* donghua[12] = {NULL};
		for (int i = 0; i < 12; i ++)
		{
			getcity_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_getcity_sw[i]));
			getcity[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_getcity[i]));
			losecity[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_losecity[i]));
			if (getcity[i].valid())
			{
				getcity[i]->setEnable(false);
				getcity[i]->setVisiable(false);
			}
			if (losecity[i].valid())
			{
				losecity[i]->setVisiable(false);
				losecity[i]->clearString();
			}
			donghua[i] = m_this->getChildNode(m_donghua[i]);
			if (donghua[i])
			{
				donghua[i]->setVisiable(false);
			}
		}
		ref_ptr<crScrollBarWidgetNode> getCityControl = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_getCityControl));
		ref_ptr<crScrollBarWidgetNode> lostCityControl = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_lostCityControl));

		getCityControl->setEnable(false);
		lostCityControl->setEnable(false);

		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		if (countryiconsw)
		{
			countryiconsw->setActiveSwitchSet(shiliid-c_startShiliID);
		}
		
		if (leadername1.valid())
		{
			leadername1->clearString();
		}
		if (leadername2.valid())
		{
			leadername2->clearString();
		}
		if (leadername3.valid())
		{
			leadername3->clearString();
		}
		if (nextopentime.valid())
		{
			nextopentime->clearString();
		}
		if (notice.valid())
		{
			notice->clearString();
			notice->setEnable(false);
		}
		if (nextopentime.valid())
		{
			std::string str;
			data->getParam(WCHDATA_JXJOpenBattle, param);
			bool isbattleopened = *((bool *)param);
			if (isbattleopened)
			{
				str = "城战开启中...";
			}
			else
			{
				//data->getParam(WCHDATA_JXJVipLv,param);
				//unsigned char viplv = *(unsigned char *)param;

				std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_1,0);
				std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_1,0);
				std::string opentime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_2,0);
				std::string endtime_2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_2,0);
				std::string opentime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOpenBattleTime_3,0);
				std::string endtime_3 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEndBattleTime_3,0);
				char tmp[20];
				time_t t = time(0);
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
					str = opentime_2+"-"+endtime_2;
				}
				else if(curtm > endtm_2 && curtm <= endtm_3)
				{
					str = opentime_3+"-"+endtime_3;
				}
				else if(opentm<endtm)
					str = opentime+"-"+endtime;
				else
				{
					str = opentime_2+"-"+endtime_2;
				}
			}
			nextopentime->setString(str);
		}
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalVec,param);
			OfficalVec & officalVec = *(OfficalVec *)param;
			if (!officalVec.empty())
			{
				if (leadername1.valid() && officalVec.size() >= 1)
					leadername1->setString( officalVec[0].second);
				if (leadername2.valid() && officalVec.size() >= 2)
					leadername2->setString( officalVec[1].second);
				if (leadername3.valid() && officalVec.size() >= 3)
					leadername3->setString( officalVec[2].second);
			}

			canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalPos,param);
			unsigned char officalPos = *(unsigned char *) param;

			canvasdata->getParam(WCHDATA_JXJCountryCanvasGroupPos,param);
			unsigned char groupPos = *(unsigned char *)param;

			PermissionCheck permissionCheck;
			if (noticeeditbtn.valid())
			{
				if (permissionCheck(officalPos,JXJCountry_EditNotice)
					|| permissionCheck(groupPos,JXJCountry_PublicNotice))
					noticeeditbtn->setVisiable(true);
				else
					noticeeditbtn->setVisiable(false);
			}
			if (noticepublishbtn.valid())
			{

				if (permissionCheck(officalPos,JXJCountry_EditNotice)
					|| permissionCheck(groupPos,JXJCountry_PublicNotice))
				{
					noticepublishbtn->setVisiable(true);
				}
				else 
					noticepublishbtn->setVisiable(false);
			}
			if (notice.valid())
			{
				canvasdata->getParam(WCHDATA_JXJMyCountryNotice,param);
				std::string strNotice = *(std::string *)param;
				notice->setString(strNotice);
			}
			//canvasdata->getParam(WCHDATA_JXJShiliWinCityDeq,param);
			//ShiliCityChangeDeq & windeq = *(ShiliCityChangeDeq *)param;
			canvasdata ->getParam(WCHDATA_JXJShiliLostCityDeq,param);
			ShiliCityChangeDeq *lostdeq = (ShiliCityChangeDeq *)param;
			ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
			crTableIO::StrVec record;
			int iconid[4] = {0};
			iconid[0] = cantontab->getTitleIndex("icon0");
			iconid[1] = cantontab->getTitleIndex("icon1");
			iconid[2] = cantontab->getTitleIndex("icon2");
			iconid[3] = cantontab->getTitleIndex("icon3");

			canvasdata->getParam(WCHDATA_JXJCountryCanvasZhoujunMap,param);
			ZhoujunMap *zhoujunMap = (ZhoujunMap *)param;

			//float getcityControlValue = getCityControl->getValue();
			getCityControl->setRange(0, (zhoujunMap->size() + 2 ) / 3 - 4);
			getCityControl->setLineValue(1.0f);
			getCityControl->setPageValue(1.0f);
			int start = 0;
			if (zhoujunMap->size() > 12)
			{
				getCityControl->setEnable(true);
				start = ( getCityControl->getValue() )* 3;
			}
			else
				getCityControl->setEnable(false);
			int end = start + 12;
		
			int i = 0,k = 0;
			for( ZhoujunMap::iterator itr= zhoujunMap->begin(); i < end && itr != zhoujunMap->end(); ++itr,i++)
			{
				if (i >= start )
				{
					if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
					{
						for (int j = 0; j < 4; j++)
						{
							std::string iconfile1 = record[iconid[j]];
							crStateSet* ss1 = dynamic_cast<crObject *>(getcity[k]->getButton()->getChild(j))->getDrawable(0)->getStateSet();
							crTexture2D* tex2d = dynamic_cast<crTexture2D *>(ss1->getTextureAttribute(0,crStateAttribute::TEXTURE));
							if(tex2d)
							{
								if(tex2d->getImageNameID().compare(iconfile1)!=0)
									tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
							}
							else
							{
								tex2d = new crTexture2D;
								tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
								tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
								tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
								tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
								tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
								tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
								ss1->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
							}
						}
					}

					if (getcity[k].valid()) getcity[k]->setVisiable(true);
					if (itr->second)
					{
						if (donghua[k]) donghua[k]->setVisiable(true);
						if (getcity_sw[k]) getcity_sw[k]->setActiveSwitchSet(0);
					}
					else
					{
						if (getcity_sw[k]) getcity_sw[k]->setActiveSwitchSet(3);
					}
					++ k;
				}
			}
			
			ref_ptr<crTableIO>  chengchiTab;
			chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			//int chengchiNameIdx = chengchiTab->getTitleIndex("名字");

			lostCityControl->setRange(0, (lostdeq->size() + 2) / 3 - 4);
			lostCityControl->setLineValue(1.0f);
			lostCityControl->setPageValue(1);
			lostCityControl->setEnable(false);

			start = 0;
			if (lostdeq->size() > 12)
			{
				lostCityControl->setEnable(true);
				start = lostCityControl->getValue() * 3;
			}
			end = start + 12;

			i = 0;
			int j = 0;
			for( ShiliCityChangeDeq::iterator itr=lostdeq->begin(); i < end && itr != lostdeq->end(); ++itr,i++)
			{
				if (i >= start)
				{
					if(citytab->queryOneRecord(0, crArgumentParser::appItoa(*itr), record)>=0)
					{
						losecity[j]->setVisiable(true);
						losecity[j]->setString(record[citynameindex]);
						++ j;
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJBuildMyCountryPageUpdateMethod
//
/////////////////////////////////////////
crJXJBuildMyCountryPageUpdateMethod::crJXJBuildMyCountryPageUpdateMethod()
{
}

crJXJBuildMyCountryPageUpdateMethod::crJXJBuildMyCountryPageUpdateMethod(const crJXJBuildMyCountryPageUpdateMethod& handle):
	crMethod(handle),
	m_selectRadio(handle.m_selectRadio),
	m_mycontributeinput(handle.m_mycontributeinput),
	m_store(handle.m_store),
	m_sijintai(handle.m_sijintai),
	m_shenlongtai(handle.m_shenlongtai),
	m_simajian(handle.m_simajian),
	m_lubanci(handle.m_lubanci),
	m_dianjiangtai(handle.m_dianjiangtai),
	m_explain(handle.m_explain),
	m_percent(handle.m_percent),
	m_building(handle.m_building)
{
	for (unsigned char i = 0; i < 5; ++ i)
	{
		m_add[i] = handle.m_add[i];
		m_add_text[i] = handle.m_add_text[i];
	}
}

void crJXJBuildMyCountryPageUpdateMethod::inputParam(int i, void *param)
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

void crJXJBuildMyCountryPageUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_add[0] = str;
		break;
	case 1:
		m_add[1] = str;
		break;
	case 2:
		m_add[2] = str;
		break;
	case 3:
		m_add[3] = str;
		break;
	case 4:
		m_add[4] = str;
		break;
	case 5:
		m_selectRadio = str;
		break;
	case 6:
		m_store = str;
		break;
	case 7:
		m_mycontributeinput = str;
		break;
	case 8:
		m_sijintai = str;
		break;
	case 9:
		m_shenlongtai = str;
		break;
	case 10:
		m_simajian = str;
		break;
	case 11:
		m_lubanci = str;
		break;
	case 12:
		m_dianjiangtai = str;
		break;
	case 13:
		m_explain = str;
		break;
	case 14:
		m_percent = str;
		break;
	case 15:
		m_building = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		m_add_text[i - 16] = str;
		break;
	case 21:
		m_guojiazijin = str;
		break;
	default:
		break;
	}
}

void crJXJBuildMyCountryPageUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		crData *canvasdata = m_this->getDataClass();
		crData *data = playerGameData->getDataClass();
		ref_ptr<crStaticTextWidgetNode> mycontributeinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mycontributeinput));
		ref_ptr<crStaticTextWidgetNode> guojiazijininput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_guojiazijin));
		ref_ptr<crProgressWidgetNode> add[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> add_text[5] = {NULL};

		ref_ptr<crStaticTextWidgetNode> explain = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_explain));
		ref_ptr<crStaticTextWidgetNode> percent = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_percent));
		crButtonWidgetNode* buildbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_building));
		//crButtonWidgetNode* store = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_store));
		if (mycontributeinput.valid())
		{
			mycontributeinput->clearString();
		}
		if (guojiazijininput.valid())
		{
			guojiazijininput->clearString();
		}
		for (int i = 0; i < 5; i++)
		{
			add[i] = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_add[i]));
			add_text[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_add_text[i]));
		}
		if (explain.valid())
		{
			explain->clearString();
		}
		if (percent.valid())
		{
			percent->clearString();
		}
		if (buildbtn)
		{
			void *param;
			crData *mainroledata = playerGameData->getMainRole()->getDataClass();
			if (mainroledata && data)
			{
				mainroledata->getParam(WCHDATA_Level,param);
				unsigned char mylv = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				int countrybuildlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryBuildLv, viplv).c_str());
				if (mylv >=countrybuildlv)
				{
					buildbtn->setEnable(true);
				}
				else
				{
					buildbtn->setEnable(false);
				}
			}	
		}
		if (canvasdata && data)
		{
			void *param;
			int gongxian[5];
			//crData *braindata = crBrain::getInstance()->getDataClass();
			data -> getParam(WCHDATA_JXJCountryContribute,param);
			int mycountrycontribute = *(int *)param;
			if (mycontributeinput.valid())
			{
				mycontributeinput->setString(crArgumentParser::appItoa(mycountrycontribute));
			}
			canvasdata->getParam(WCHDATA_JXJCanvasCapital,param);
			int countrycapital = *(int *)param;
			if (guojiazijininput.valid())
			{
				guojiazijininput->setString(crArgumentParser::appItoa(countrycapital));
			}
			unsigned char j = 0;
			for (int i = WCHDATA_JXJShenNongMiao; j < 5 && i <= WCHDATA_JXJTaiMiao; ++ i,++ j)
			{
				canvasdata->getParam(i,param);
				gongxian[j] = *(int *)param;
			}
			ref_ptr<crTableIO> countrybuildtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBuildTab);
			crTableIO::StrVec record;
			crTableIO::StrVec record2;
			crTableIO::DataVec records;
			crTableIO::DataVec::iterator itr,peritr;
			//std::string filename[5];
			int moneyneedindex = countrybuildtab->getTitleIndex("铜钱");
			int typeindex = countrybuildtab->getTitleIndex("type");
			int addindex = countrybuildtab->getTitleIndex("加成");
			float progress = 0.0f;
			//unsigned char lv = 0;
			//unsigned char addp = 0;
			float nowcopper;
			float needcopper;
			//data->getParam(WCHDATA_JXJVipLv,param);
			//unsigned char viplv = *(unsigned char *)param;
			//int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJContriCountryOnceCopper,viplv).c_str()));
			//data ->getParam(WCHDATA_JXJCoppercash,param);
			//int mycopper = *(int *)param;
			for (int i = 0; i < 5; i ++)
			{
				progress = 0.0f;
				records.clear();
				countrybuildtab->queryRecords(typeindex,crArgumentParser::appItoa(i),records);
				if(!records.empty())
				{
					peritr = records.begin();
					itr = peritr;
					++itr;
					for (; itr != records.end(); ++itr,++peritr)
					{
						if (gongxian[i] <  atoi((*itr)[moneyneedindex].c_str()))
						{
							break;
						}
					}
					unsigned char lv = atoi((*peritr)[0].c_str());
					if(itr != records.end() && countrybuildtab->queryOneRecord(0,crArgumentParser::appItoa(lv),record) >= 0)
					{
						nowcopper = float(gongxian[i]) - atof(record[moneyneedindex].c_str());
						needcopper = atof((*itr)[moneyneedindex].c_str()) - atof(record[moneyneedindex].c_str());
						if ((int)needcopper <= 0)
						{
							progress = 1.0f;
						}
						else
						{
							progress = nowcopper/needcopper;
						}
					}
					if (add[i].valid())
					{
						add[i]->setVisiable(true);
						add[i]->setProgress(progress);
						if (add_text[i].valid())
						{
							char tmpVal[16];
							sprintf_s(tmpVal,"%.1f %%\0",progress * 100);
							//int tmpVal = progress * 1000;
							//add_text[i]->setString(crArgumentParser::appFtoa() + "%");
							add_text[i]->setString((*peritr)[addindex] + "%");
							//add_text[i]->setString(std::string(tmpVal));
						}
					}
				}
			}
			//std::string expStr = "金是矿石的意思，乱世之中，军备格外重要，矿石更是一等一的重要物资。国家为此专设司金台，管理铁矿开采提炼等事，通过严格的规章制度与统一调度，可以提高铁矿产量。";
			//explain->setString(expStr);
			//percent->setString(add[0]->getString());
		}
	}
}

/////////////////////////////////////////
//
//crJXJMyLeaderManagerCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyLeaderManagerCanvasUpdateMethod::crJXJMyLeaderManagerCanvasUpdateMethod():
	m_this(NULL)
{
}

crJXJMyLeaderManagerCanvasUpdateMethod::crJXJMyLeaderManagerCanvasUpdateMethod(const crJXJMyLeaderManagerCanvasUpdateMethod& handle):
	crMethod(handle),
	m_PrimeMinister (handle.m_PrimeMinister ),
	m_GreatMinister (handle.m_GreatMinister),
	m_Daisyougun	(handle.m_Daisyougun),
	m_Obligation	(handle.m_Obligation)	,
	m_Rule			(handle.m_Rule		)	,
	m_Crops			(handle.m_Crops		)	,
	m_Recommend		(handle.m_Recommend	)		
{
	for (unsigned char i = 0; i < 15; ++ i)
	{
		m_checkbox_sw[i] = handle.m_checkbox_sw[i];
		m_Select[i] = handle.m_Select[i];
	}
}

void crJXJMyLeaderManagerCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyLeaderManagerCanvasUpdateMethod::addParam(int i, const std::string& str)
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
	case 3:
		m_Obligation = str;
		break;
	case 4:
		m_Rule = str;
		break;
	case 5:
		m_Crops = str;
		break;
	case 6:
		m_Select[0] = str;
		break;
	case 7:
		m_Select[1] = str;
		break;
	case 8:
		m_Select[2] = str;
		break;
	case 9:
		m_Select[3] = str;
		break;
	case 10:
		m_Select[4] = str;
		break;
	case 11:
		m_Select[5] = str;
		break;
	case 12:
		m_Select[6] = str;
		break;
	case 13:
		m_Select[7] = str;
		break;
	case 14:
		m_Select[8] = str;
		break;
	case 15:
		m_Select[9] = str;
		break;
	case 16:
		m_Select[10] = str;
		break;
	case 17:
		m_Select[11] = str;
		break;
	case 18:
		m_Select[12] = str;
		break;
	case 19:
		m_Select[13] = str;
		break;
	case 20:
		m_Select[14] = str;
		break;
	case 21:
		m_Recommend = str;
		break;
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
		m_checkbox_sw[i - 22] = str;
		break;
	default:
		break;
	}
}

void crJXJMyLeaderManagerCanvasUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		crData *canvasdata = m_this->getDataClass();
		int playerid = playerGameData->getPlayerID();
		//crData *data = playerGameData->getDataClass();

		ref_ptr<crStaticTextWidgetNode> chengXiang = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_PrimeMinister));
		ref_ptr<crStaticTextWidgetNode> daSiMa = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_GreatMinister));
		ref_ptr<crStaticTextWidgetNode> jiangJun = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Daisyougun));

		void *param;
		bool elector = false;
		crData *playerdata = playerGameData->getDataClass();
		playerdata->getParam(WCHDATA_JXJElector,param);
		elector = *(bool *)param;
		canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalVec,param);
		OfficalVec & officalVec = *(OfficalVec *)param;
		if (chengXiang.valid())
		{
			chengXiang->setString(officalVec[0].second);
		}
		if (daSiMa.valid())
		{
			daSiMa->setString(officalVec[1].second);
		}
		if (jiangJun.valid())
		{
			jiangJun->setString(officalVec[2].second);
		}

		crMultiSwitch *foldbtn_sw[15] = {NULL};
		ref_ptr<crCheckBoxWidgetNode >itemCheckBox[15] = {NULL}; 
		int iCheckNumber = 0;
		for (int i = 0; i < 15; i++)
		{
			itemCheckBox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_Select[i]));
			foldbtn_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_checkbox_sw[i]));
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

		ref_ptr<crButtonWidgetNode> buildbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Recommend));
		//if (buildbtn)
		//{
		//	if (iCheckNumber == 2 && selectLeaderSet->empty())
		//	{
		//		buildbtn->setEnable(true);
		//		for (int i = 0; i < 15; i++)
		//		{
		//			itemCheckBox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_Select[i]));
		//			if (itemCheckBox[i].valid())
		//			{
		//				if(!itemCheckBox[i]->getSelect())
		//				{
		//					itemCheckBox[i]->setEnable(false);
		//				}
		//			}
		//		}
		//	}
		//	else
		//		buildbtn->setEnable(false);
		//}

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


		//canvasdata->getParam(WCHDATA_JXJCountryCanvasGroupPos,param);
		//unsigned char groupPos = *(unsigned char *) param;

		//if (selectLeaderSet->empty())
		//{
		//	// 之前没有投过票
		//	if (permissionCheck(groupPos,JXJCountry_Vote))
		//	{
		//		// 有投票权限
		//		checkBoxsVisiable = true;
		//		checkBoxsEnable  = true;
		//		voteBtnVisiable = true;
		//		voteBtnEanble = true;
		//	}
		//	//else
		//	//{
		//	//	// 没有投票权限
		//	//	checkBoxsVisiable = false;
		//	//	voteBtnVisiable = false;
		//	//	voteBtnEanble = false;
		//	//}
		//}
		//else
		//{
		//	// 之前投过票
		//	//if (permissionCheck(groupPos,JXJCountry_Vote))
		//	{
		//		// 有投票权限
		//		checkBoxsVisiable = true;
		//		checkBoxsEnable = false;
		//		voteBtnVisiable = false;
		//		voteBtnEanble = false;
		//	}
		//	//else
		//	//{
		//	//	// 没有投票权限
		//	//	checkBoxsVisiable = false;
		//	//	voteBtnVisiable = false;
		//	//	voteBtnEanble = false;
		//	//}
		//}

		if (buildbtn.valid())
		{
			buildbtn->setVisiable(voteBtnVisiable);
			buildbtn->setEnable(voteBtnEanble);
		}
		
		ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_Crops));
		if(tableWidget.valid())
		{
			tableWidget->clearData();

			SelectLeaderIdSet::iterator slsItr;
			canvasdata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			CandidateVec *candidateVec = (CandidateVec *)param;
			CandidateVec::iterator cvItr = candidateVec->begin();

			int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,0).c_str());
			int i = 0;
			for (;cvItr != candidateVec->end() && i<15; ++ cvItr)
			{
				if (cvItr->valid())
				{
					if ((*cvItr)->m_rankVal > maxRankingNum)
					{
						tableWidget->addData(0,i,crArgumentParser::appItoa(maxRankingNum) + "+");
					}
					else
					{
						tableWidget->addData(0,i,crArgumentParser::appItoa((*cvItr)->m_rankVal));
					}
					tableWidget->addData(1,i,(*cvItr)->playerName);
					tableWidget->addData(2,i,(*cvItr)->juntuanName);
					tableWidget->addData(i,(*cvItr)->nPlayerID);

					if (itemCheckBox[i].valid())
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
		}
		canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJMyZhoujunManagerCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyZhoujunManagerCanvasUpdateMethod::crJXJMyZhoujunManagerCanvasUpdateMethod():
	m_this(NULL)
{
}

crJXJMyZhoujunManagerCanvasUpdateMethod::crJXJMyZhoujunManagerCanvasUpdateMethod(const crJXJMyZhoujunManagerCanvasUpdateMethod& handle):
	crMethod(handle),
	m_Country_Affiliation(handle.m_Country_Affiliation),
	m_Country_Apply(handle.m_Country_Apply),
	m_Country_Bar(handle.m_Country_Bar),
	m_Country_Cash(handle.m_Country_Cash),
	m_Country_CityName(handle.m_Country_CityName),
	m_Country_Corps(handle.m_Country_Corps),
	m_Country_Enfeoff(handle.m_Country_Enfeoff),
	m_Country_GarrisonCorps(handle.m_Country_GarrisonCorps),
	m_Country_Horse(handle.m_Country_Horse),
	m_Country_ListScroll(handle.m_Country_ListScroll),
	m_Country_Material(handle.m_Country_Material),
	m_Country_Satrap(handle.m_Country_Satrap),
	m_Country_zhoujun(handle.m_Country_zhoujun),
	m_Country_ZhouMu(handle.m_Country_ZhouMu),
	m_CantonRadio(handle.m_CantonRadio),
	m_chengChiList(handle.m_chengChiList),
	m_Country_Enfeoff_sw(handle.m_Country_Enfeoff_sw),
	m_Country_Apply_sw(handle.m_Country_Apply_sw),
	m_Battleground_map(handle.m_Battleground_map),
	m_Country_Cancle(handle.m_Country_Cancle)
{
	for (unsigned char i = 0; i < 20; ++ i)
	{
		m_Country_State[i] = handle.m_Country_State[i];
		m_Country_State_sw[i] = handle.m_Country_State_sw[i];
		m_Country_newState_sw[i] = handle.m_Country_newState_sw[i];
	}
}

void crJXJMyZhoujunManagerCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyZhoujunManagerCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Country_State[0] = str;
		break;
	case 1:
		m_Country_State[1] = str;
		break;
	case 2:
		m_Country_State[2] = str;
		break;
	case 3:
		m_Country_State[3] = str;
		break;
	case 4:
		m_Country_State[4] = str;
		break;
	case 5:
		m_Country_State[5] = str;
		break;
	case 6:
		m_Country_State[6] = str;
		break;
	case 7:
		m_Country_State[7] = str;
		break;
	case 8:
		m_Country_State[8] = str;
		break;
	case 9:
		m_Country_State[9] = str;
		break;
	case 10:
		m_Country_State[10] = str;
		break;
	case 11:
		m_Country_State[11] = str;
		break;
	case 12:
		m_Country_State[12] = str;
		break;
	case 13:
		m_Country_State[13] = str;
		break;
	case 14:
		m_Country_State[14] = str;
		break;
	case 15:
		m_Country_State[15] = str;
		break;
	case 16:
		m_Country_State[16] = str;
		break;
	case 17:
		m_Country_State[17] = str;
		break;
	case 18:
		m_Country_State[18] = str;
		break;
	case 19:
		m_Country_State[19] = str;
		break;
	case 20:
		m_Country_zhoujun = str;
		break;
	case 21:
		m_Country_ZhouMu = str;
		break;
	case 22:
		m_Country_Corps = str;
		break;
	case 23:
		m_Country_Enfeoff = str;
		break;
	case 24:
		m_Country_Apply = str;
		break;
	case 25:
		m_Country_Affiliation = str;
		break;
	case 26:
		m_Country_CityName = str;
		break;
	case 27:
		m_Country_Cash = str;
		break;
	case 28:
		m_Country_Horse = str;
		break;
	case 29:
		m_Country_Material = str;
		break;
	case 30:
		m_Country_GarrisonCorps = str;
		break;
	case 31:
		m_Country_Satrap = str;
		break;
	case 32:
		m_Country_ListScroll = str;
	case 33:
		m_Country_Bar = str;
		break;
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
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
	case 52:
	case 53:
		m_Country_State_sw[i - 34] = str;
		break;
	case 54:
		m_CantonRadio = str;
		break;
	case 55:
		m_chengChiList = str;
		break;
	case 56:
		m_Country_Enfeoff_sw = str;
		break;
	case 57:
		m_Country_Apply_sw = str;
		break;
	case 58:
		m_Battleground_map = str;
		break;
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
		m_Country_newState_sw[i - 59] = str;
		break;
	case 79:
		m_Country_Cancle = str;
		break;
	default:
		break;
	}
}

void crJXJMyZhoujunManagerCanvasUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crCanvasNode * cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_Battleground_map);
	crCanvasNode* mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Crops1");
	crCanvasNode* myCountryCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && myCountryCanvas &&cityMapCanvas && mygroupcanvas)
	{
		crData *canvasdata = myCountryCanvas->getDataClass();
		crData *data = playerGameData->getDataClass();
		void *param;
		if (!canvasdata) return;
		crData * cityMapData = cityMapCanvas->getDataClass();
		if (!cityMapData) return;
		crData *groupCanvasdata = mygroupcanvas->getDataClass();
		if (!groupCanvasdata) return;
		canvasdata->getParam(WCHDATA_JXJCountryCanvasZhoujunMap,param);
		ZhoujunMap *zhoujunMap = (ZhoujunMap *)param;

		ref_ptr<crRadioWidgetNode> countryState[20] = {NULL};
		crMultiSwitch * countryState_sw[20]= {NULL};
		crMultiSwitch * newCountryState_sw[20]= {NULL};
		for (int i = 0; i < 20; i++)
		{
			countryState[i]= dynamic_cast<crRadioWidgetNode *>(myCountryCanvas->getWidget(m_Country_State[i]));
			if (countryState[i].valid()) countryState[i]->setVisiable(false);
			countryState_sw[i] = dynamic_cast<crMultiSwitch*>(myCountryCanvas->getChildNode(m_Country_State_sw[i]));
			newCountryState_sw[i] = dynamic_cast<crMultiSwitch*>(myCountryCanvas->getChildNode(m_Country_newState_sw[i]));
		}
		ref_ptr<crStaticTextWidgetNode>  zhoujun = dynamic_cast<crStaticTextWidgetNode *>(myCountryCanvas->getWidget(m_Country_zhoujun));
		ref_ptr<crStaticTextWidgetNode>  zhoumu = dynamic_cast<crStaticTextWidgetNode *>(myCountryCanvas->getWidget(m_Country_ZhouMu));
		crTableWidgetNode	* chengchiList = dynamic_cast<crTableWidgetNode *>(myCountryCanvas->getWidget(m_chengChiList)); 
		std::string tmpString = "暂无";
		if (zhoumu.valid()) zhoumu->setString(tmpString);
		ref_ptr<crStaticTextWidgetNode>  zhenShouGroup = dynamic_cast<crStaticTextWidgetNode *>(myCountryCanvas->getWidget(m_Country_Corps));
		if (zhenShouGroup.valid()) zhenShouGroup->setString(tmpString);

		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(myCountryCanvas->getWidget(m_CantonRadio));

		crButtonWidgetNode* allocateCanton = dynamic_cast<crButtonWidgetNode *>(myCountryCanvas->getWidget(m_Country_Enfeoff));
		crButtonWidgetNode* applyCanton = dynamic_cast<crButtonWidgetNode *>(myCountryCanvas->getWidget(m_Country_Apply));
		crButtonWidgetNode* cancleCanton = dynamic_cast<crButtonWidgetNode *>(myCountryCanvas->getWidget(m_Country_Cancle));

		ref_ptr<crMultiSwitch > allocateCanton_sw = dynamic_cast<crMultiSwitch *>(myCountryCanvas->getChildNode(m_Country_Enfeoff_sw));
		ref_ptr<crMultiSwitch > applyCanton_sw = dynamic_cast<crMultiSwitch *>(myCountryCanvas->getChildNode(m_Country_Apply_sw));

		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(myCountryCanvas->getWidget(m_Country_ListScroll));

		PermissionCheck permissionCheck;
		unsigned char officalPos = 0;
		canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalPos,param);
		if (param)
		{
			 officalPos = *(unsigned char*) param;
		}
		if (!permissionCheck(officalPos,JXJCountry_AllocateCanton))
		{
			if (allocateCanton) allocateCanton->setVisiable(false);
		}
		else
		{
			if (allocateCanton) allocateCanton->setVisiable(true);
		}

		ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
		crTableIO::StrVec record;
		int iconid[4] = {0},nameIdx = 0;
		nameIdx = cantontab->getTitleIndex("名称");
		iconid[0] = cantontab->getTitleIndex("icon0");
		iconid[1] = cantontab->getTitleIndex("icon1");
		iconid[2] = cantontab->getTitleIndex("icon2");
		iconid[3] = cantontab->getTitleIndex("icon3");

		groupCanvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
		MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
		int myGroupCantonId = -1;
		if (!myAllocatedCantonVec->empty())
		{
			myGroupCantonId = (*myAllocatedCantonVec)[0];
		}
		groupCanvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
		MyApplyCantonVec * myApplyCantonVec = (MyApplyCantonVec *)param;
		int myApplyCantonId_1 = -1;
		int myApplyCantonId_2 = -1;
		if (!myApplyCantonVec->empty())
		{
			myApplyCantonId_1 = (*myApplyCantonVec)[0];
			if (myApplyCantonVec->size() > 1)
			{
				myApplyCantonId_2 = (*myApplyCantonVec)[1];
			}
		}
		// 首次打开州郡管理界面时，默认选择第一个州，这时需要将取消驻扎按钮状态初始化
		canvasdata->getParam(WCHDATA_JXJCountryCanvasSelectedCantonId, param);
		int selectedCantonId = *(int *)param;
		if (myApplyCantonId_1 != selectedCantonId && myApplyCantonId_2 != selectedCantonId)
		{
			cancleCanton->setVisiable(false);
		}
		// 申请驻扎按钮状态初始化 应放在 取消驻扎按钮状态初始化 之后
		canvasdata->getParam(WCHDATA_JXJCountryCanvasGroupPos,param);
		unsigned char groupPos = *(unsigned char *) param;
		if (!permissionCheck(groupPos,JXJGroup_ApplyStationCanton))
		{
			if (applyCanton) applyCanton->setVisiable(false);
		}
		else
		{
			if (!cancleCanton->getVisiable())
			{
				if (applyCanton) applyCanton->setVisiable(true);
			}
			else
			{
				if (applyCanton) applyCanton->setVisiable(false);
			}
		}
		std::string cantonName;
		ZhoujunMap::iterator zItr = zhoujunMap->begin();
		int cantonId = 0;
		for(int i = 0; i < 20 && zItr != zhoujunMap->end(); i++, ++ zItr)
		{
			if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(zItr->first),record)>=0)
			{
				countryState[i]->setVisiable(true);
				countryState_sw[i]->setActiveSwitchSet(zItr->second ? 0 : 3);
				countryState[i]->setEnable(zItr->second);
				newCountryState_sw[i]->setVisiable(false);
				int newCantonId = zItr->first;
				if (groupradio.valid() && groupradio->getSelect() == i) 
				{
					cantonId = zItr->first;
					cantonName = record[nameIdx];
					if (zhoujun.valid()) zhoujun->setString(cantonName);
					if (zItr->second)
					{
						allocateCanton->setEnable(true);
						allocateCanton_sw->setActiveSwitchSet(0);
						applyCanton->setEnable(true);
						applyCanton_sw->setActiveSwitchSet(0);
					}
					else
					{
						applyCanton->setEnable(false);
						applyCanton_sw->setActiveSwitchSet(3);
					}
				}
				if (newCantonId == myGroupCantonId)
				{
					newCountryState_sw[i]->setVisiable(true);
					newCountryState_sw[i]->setActiveSwitchSet(1);
				}
				if (newCantonId == myApplyCantonId_1 || newCantonId == myApplyCantonId_2)
				{
					newCountryState_sw[i]->setVisiable(true);
					newCountryState_sw[i]->setActiveSwitchSet(0);
				}
				for (int j = 0; j < 4; j++)
				{
					std::string iconfile1 = record[iconid[j]];
					crStateSet* ss1 = dynamic_cast<crObject *>(countryState[i]->getButton()->getChild(j))->getDrawable(0)->getStateSet();
					crTexture2D* tex2d = dynamic_cast<crTexture2D *>(ss1->getTextureAttribute(0,crStateAttribute::TEXTURE));
					if(tex2d)
					{
						if(tex2d->getImageNameID().compare(iconfile1)!=0)
							tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
					}
					else
					{
						tex2d = new crTexture2D;
						tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
						tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
						tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
						tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
						tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
						ss1->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
					}
				}
			}
		}
		canvasdata->getParam(WCHDATA_JXJCountryCanvasZhenShouGroupLeader,param);
		std::string *groupLeader = (std::string *)param;
		canvasdata->getParam(WCHDATA_JXJCountryCanvasZhenShouGroup,param);
		std::string *groupName = (std::string *)param;
		if (zhoumu.valid() && !(groupLeader->empty()) ) zhoumu->setString(*groupLeader);
		if (zhenShouGroup.valid() && !(groupName->empty()) ) zhenShouGroup->setString(*groupName);

		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		/*canvasdata->getParam(WCHDATA_JXJCountryCanvasChengchiInfoMap,param);
		ChengchiInfoMap *chengchiInfoMap = (ChengchiInfoMap *)param;*/
		cityMapData->getParam(WCHDATA_JXJChengChiInfoMap,param);
		CantonInfoMap * chengchiInfoMap = (CantonInfoMap *)param;
		CantonInfoMap::iterator cimItr;
		do 
		{
			ref_ptr<crTableIO>  chengchiTab;
			crTableIO::StrVec record;
			int cantonIdx ,junNameIdx,chengchiNameIdx;
			int tmpCantonId;
			chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			if (false == chengchiTab.valid()) break;
			junNameIdx = chengchiTab->getTitleIndex("郡名");
			chengchiNameIdx = chengchiTab->getTitleIndex("名字");
			//int earningsTypeIdx = chengchiTab->getTitleIndex("收益类型");
			int earningsNumIdx = chengchiTab->getTitleIndex("收益数量");
			int techanTypeIdx = chengchiTab->getTitleIndex("特产类型");
			int techanNumIdx = chengchiTab->getTitleIndex("特产数量");
			cantonIdx = chengchiTab->getTitleIndex("州ID");
			//int m_totalChengChiNum = 0;

			std::string teChanName[] = 
			{
				"",
				"粮草","木材","铁矿","马匹","铜钱",
				"经验","战功","礼金","兵力材料","道具",
				"兵力","功勋","国家贡献","元宝","将魂","将卡好感度"
			};

			data->getParam(WCHDATA_JXJChengChiMap,param);
			ChengchiMap *chengchiMap = (ChengchiMap *)param;
			ChengchiMap::iterator ccItr = chengchiMap->begin();
			chengchiList->clearData();
			unsigned char totalNum = 0;
			unsigned char firstIdx = 0;
			if (scroll.valid())
			{
				if (scroll->getEnable() == true)
				{
					firstIdx = scroll->getValue();
				}
			}
			for (unsigned char i = 0; ccItr != chengchiMap->end(); ++ ccItr)
			{
				if (shiliid != ccItr->second->getChengzhuShili()) continue;
				tmpCantonId = 0;
				if (chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(ccItr->first),record)>=0)
				{
					tmpCantonId = atoi(record[cantonIdx].c_str());
				}
				else continue;
				if (tmpCantonId != cantonId) continue;
				++ totalNum ;
				if (totalNum <= firstIdx || totalNum > firstIdx + 7)
				{
					continue;
				}
				chengchiList->addData(0,i,record[junNameIdx]);
				chengchiList->addData(1,i,record[chengchiNameIdx]);
				chengchiList->addData(2,i,record[earningsNumIdx]);
				chengchiList->addData(3,i,teChanName[atoi(record[techanTypeIdx].c_str())]);
				chengchiList->addData(4,i,record[techanNumIdx]);
				
				cimItr = chengchiInfoMap->find(ccItr->first);
				if (cimItr != chengchiInfoMap->end() && cimItr->second.valid())
				{
					chengchiList->addData(5,i,cimItr->second->m_groupName);
					chengchiList->addData(6,i,cimItr->second->m_groupLeader);
				}
				++ i;
			}


			if (scroll.valid())
			{
				int scrollPerpageNum = 7;
				scroll->setLineValue(1.0);
				scroll->setPageValue(1.0);
				if (totalNum > scrollPerpageNum)
				{
					scroll->setRange(0,totalNum - scrollPerpageNum);
					scroll->setEnable(true);
				}
				else 
				{
					scroll->setEnable(false);
				}
			}
		} while (0);
	}
}
///////////////////////////////////////////
////
////crJXJMyCounrtyShopCanvasUpdateMethod
////
///////////////////////////////////////////
//crJXJMyCounrtyShopCanvasUpdateMethod::crJXJMyCounrtyShopCanvasUpdateMethod():
//	m_this(NULL)
//{
//}
//
//crJXJMyCounrtyShopCanvasUpdateMethod::crJXJMyCounrtyShopCanvasUpdateMethod(const crJXJMyCounrtyShopCanvasUpdateMethod& handle):
//	crMethod(handle),
//	m_Country_LeftPage (handle.m_Country_LeftPage),
//	m_Country_PageNumber(handle.m_Country_PageNumber),
//	m_Country_RightPage(handle.m_Country_RightPage),
//	m_Country_Select(handle.m_Country_Select),
//	m_Country_Contribute(handle.m_Country_Contribute),
//	m_Country_shopRadio(handle.m_Country_shopRadio),
//	m_ShopRadio_shenMi(handle.m_ShopRadio_shenMi),
//	m_ShopRaido_guanZhi(handle.m_ShopRaido_guanZhi)
//{
//	for (unsigned char i = 0; i < 5; ++ i)
//	{
//		m_Country_Image[i] = handle.m_Country_Image[i];
//		m_Country_Name[i] = handle.m_Country_Name[i];
//		m_Country_Explain[i] = handle.m_Country_Explain[i];
//		m_Country_Position[i] = handle.m_Country_Position[i];
//		m_Country_Number[i] = handle.m_Country_Number[i];
//		m_Country_Category[i] = handle.m_Country_Category[i];
//		m_Country_Purchase[i] = handle.m_Country_Purchase[i];
//	}
//}
//
//void crJXJMyCounrtyShopCanvasUpdateMethod::inputParam(int i, void *param)
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
//
//void crJXJMyCounrtyShopCanvasUpdateMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_Country_Image[0] = str;
//		break;
//	case 1:
//		m_Country_Image[1] = str;
//		break;
//	case 2:
//		m_Country_Image[2] = str;
//		break;
//	case 3:
//		m_Country_Image[3] = str;
//		break;
//	case 4:
//		m_Country_Image[4] = str;
//		break;
//	case 5:
//		m_Country_Name[0] = str;
//		break;
//	case 6:
//		m_Country_Name[1] = str;
//		break;
//	case 7:
//		m_Country_Name[2] = str;
//		break;
//	case 8:
//		m_Country_Name[3] = str;
//		break;
//	case 9:
//		m_Country_Name[4] = str;
//		break;
//	case 10:
//		m_Country_Explain[0] = str;
//		break;
//	case 11:
//		m_Country_Explain[1] = str;
//		break;
//	case 12:
//		m_Country_Explain[2] = str;
//		break;
//	case 13:
//		m_Country_Explain[3] = str;
//		break;
//	case 14:
//		m_Country_Explain[4] = str;
//		break;
//	case 15:
//		m_Country_Position[0] = str;
//		break;
//	case 16:
//		m_Country_Position[1] = str;
//		break;
//	case 17:
//		m_Country_Position[2] = str;
//		break;
//	case 18:
//		m_Country_Position[3] = str;
//		break;
//	case 19:
//		m_Country_Position[4] = str;
//		break;
//	case 20:
//		m_Country_Number[0] = str;
//		break;
//	case 21:
//		m_Country_Number[1] = str;
//		break;
//	case 22:
//		m_Country_Number[2] = str;
//		break;
//	case 23:
//		m_Country_Number[3] = str;
//		break;
//	case 24:
//		m_Country_Number[4] = str;
//		break;
//	case 25:
//		m_Country_Category[0] = str;
//		break;
//	case 26:
//		m_Country_Category[1] = str;
//		break;
//	case 27:
//		m_Country_Category[2] = str;
//		break;
//	case 28:
//		m_Country_Category[3] = str;
//		break;
//	case 29:
//		m_Country_Category[4] = str;
//		break;
//	case 30:
//		m_Country_Purchase[0] = str;
//		break;
//	case 31:
//		m_Country_Purchase[1] = str;
//		break;
//	case 32:
//		m_Country_Purchase[2] = str;
//		break;
//	case 33:
//		m_Country_Purchase[3] = str;
//		break;
//	case 34:
//		m_Country_Purchase[4] = str;
//		break;
//	case 35:
//		m_Country_LeftPage = str;
//		break;
//	case 36:
//		m_Country_PageNumber = str;
//		break;
//	case 37:
//		m_Country_RightPage = str;
//		break;
//	case 38:
//		m_Country_Select = str;
//		break;
//	case 39:
//		m_Country_Contribute = str;
//		break;
//	case 40:
//		m_Country_shopRadio = str;
//		break;
//	case 41:
//		m_ShopRaido_guanZhi = str;
//		break;
//	case 42:
//		m_ShopRadio_shenMi = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJMyCounrtyShopCanvasUpdateMethod::operator()(crHandle &handle)
//{
//	ref_ptr<crCanvasNode> canvas = m_this;
//	if (canvas.valid() && canvas->getDataClass())
//	{
//		crData * canvasdata = canvas->getDataClass();
//		void *param;
//		canvasdata->getParam(WCHDATA_JXJGovPostShopCurPage, param);
//		int curpage = *(int *)param;
//
//		ref_ptr<crStaticTextWidgetNode> pageNumber = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_PageNumber));
//		ref_ptr<crStaticTextWidgetNode> contribute = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Contribute));
//		ref_ptr<crRadioGroupWidgetNode> shopGroupRadio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_Country_shopRadio));
//		ref_ptr<crRadioWidgetNode> shopRadioGuanZhi = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_ShopRaido_guanZhi));
//		ref_ptr<crRadioWidgetNode> shopRadioShenMi = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_ShopRadio_shenMi));
//		if (shopRadioShenMi.valid())
//		{
//			shopRadioShenMi->setVisiable(false);
//		}
//
//		ref_ptr<crImageBoxWidgetNode> Image[5] = {NULL};
//		ref_ptr<crStaticTextWidgetNode> Name[5] = {NULL};
//		ref_ptr<crStaticTextWidgetNode> Explain[5] = {NULL};
//		ref_ptr<crStaticTextWidgetNode> Position[5] = {NULL};
//		ref_ptr<crStaticTextWidgetNode> Number[5] = {NULL};
//		ref_ptr<crStaticTextWidgetNode> Category[5] = {NULL};
//		ref_ptr<crButtonWidgetNode> Purchase[5] = {NULL};
//		ref_ptr<crButtonWidgetNode> left = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_LeftPage));
//		ref_ptr<crButtonWidgetNode> right = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_RightPage));
//		ref_ptr<crCheckBoxWidgetNode> showAll = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_Country_Select));
//		for (int i = 0; i < 5; i++)
//		{
//			Image[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_Country_Image[i]));
//			Name[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Name[i]));
//			Explain[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Explain[i]));
//			Position[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Position[i]));
//			Number[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Number[i]));
//			Category[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Category[i]));
//			Purchase[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_Purchase[i]));
//			if (Image[i].valid())Image[i]->setVisiable(false);
//			if (Name[i].valid())Name[i]->clearString();
//			if (Explain[i].valid())Explain[i]->clearString();
//			if (Position[i].valid())Position[i]->clearString();
//			if (Number[i].valid())Number[i]->clearString();
//			if (Category[i].valid())Category[i]->clearString();
//			if (Purchase[i].valid())Purchase[i]->setVisiable(false);
//		}
//		bool ifShowAll = true;
//		if (showAll.valid())
//		{
//			if (showAll->getSelect())
//			{
//				ifShowAll = false;
//			}
//		}
//
//
//		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//		ref_ptr<crTableIO> govpostshoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostShopTab);
//		ref_ptr<crTableIO> govposttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
//		if(m_this && playerGameData && itemtab.valid() && govpostshoptab.valid() && govposttab.valid()
//			&& playerGameData->getMainRole())
//		{
//			crData *data = playerGameData->getDataClass();
//			crData *mainroledata = playerGameData->getMainRole()->getDataClass();
//			if (data && mainroledata)
//			{
//				mainroledata -> getParam(WCHDATA_JXJGovPost,param);
//				unsigned char govPost = *(unsigned char *)param;
//				data -> getParam(WCHDATA_JXJCountryContribute,param);
//				int mycountrycontribute = *(int *)param;
//				if (contribute.valid())
//				{
//					contribute->setString(crArgumentParser::appItoa(mycountrycontribute));
//				}
//				crTableIO::StrVec record;
//				crTableIO::StrVec recordg;
//				//int scrollvalue = 0;
//				//int row = 0;
//				//int showcurpos = 0;
//				int rowcount = govpostshoptab->getRowCount();
//				int govnameindex = govposttab->getTitleIndex("govpost");
//				int infoindex = itemtab->getTitleIndex("物品描述");
//				int colorindex = itemtab->getTitleIndex("品质");
//				int iconindex = itemtab->getTitleIndex("icon");
//				int nameindex = itemtab->getTitleIndex("name");
//				int priceindex = govpostshoptab->getTitleIndex("消耗贡献");
//				int govpostindex = govpostshoptab->getTitleIndex("官职需求");
//				if (infoindex >= 0 && colorindex >= 0 && iconindex >= 0 && priceindex >= 0 && govpostindex >= 0)
//				{
//
//					canvasdata -> getParam(WCHDATA_JXJGovPostShopItemDeq,param);
//					GovPostShopItemIDDeq *govpostshopitemdeq = (GovPostShopItemIDDeq *)param;
//					govpostshopitemdeq->clear();
//					std::string pricestr;
//					std::string itemidstr;
//					int saleitemcount = 0;
//					int price = 0;
//					int itemid = 0;
//					unsigned char needgovpost = 0;
//					bool ifshow = false;
//					int onepagecount = 5;
//					int startVal = (curpage - 1) * onepagecount;
//					int endVal = startVal + onepagecount;
//					int j = 0;
//					for (int i = 0; i < rowcount; ++ i)
//					{
//						ifshow = false;
//						needgovpost = atoi((*govpostshoptab)(i,govpostindex).c_str());
//						if (!ifShowAll)
//						{
//							if (needgovpost <= govPost)
//							{
//								ifshow = true;
//								++ saleitemcount;
//							}
//						}
//						else
//						{
//							ifshow = true;
//							++ saleitemcount;
//						}
//						if (ifshow && saleitemcount - 1 >= startVal && saleitemcount  <= endVal)
//						{
//							//j = i - startVal;
//							pricestr = (*govpostshoptab)(i,priceindex);
//							price =  atoi(pricestr.c_str());
//							itemidstr = (*govpostshoptab)(i,0);
//							itemid =  atoi(itemidstr.c_str());
//							if (itemtab->queryOneRecord(0,itemidstr,record)>=0)
//							{
//								govpostshopitemdeq->push_back(itemid);
//								if(Image[j].valid())
//								{
//									Image[j]->setVisiable(true);
//									Image[j]->setImageName(record[iconindex]);
//								}
//								if(Name[j].valid())
//								{
//									Name[j]->setString(record[nameindex]);
//								}
//								if(Number[j].valid())
//								{
//									if (mycountrycontribute>=price)
//									{
//										Number[j]->setColor(crVector4f(1.0f,1.0f,1.0f,1.0f));
//									}
//									else
//									{
//										Number[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
//									}
//									Number[j]->setString(pricestr);
//								}
//								if(Explain[j].valid())
//								{
//									Explain[j]->setString(record[infoindex]);
//								}
//								if(Position[j].valid())
//								{
//									if (govPost >= needgovpost)
//									{
//										Position[j]->setColor(crVector4f(32.0f,32.0f,32.0f,255.0f)/255.0f);
//									}
//									else
//									{
//										Position[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
//									}
//
//									if (govposttab->queryOneRecord(0,crArgumentParser::appItoa(needgovpost),recordg)>=0)
//									{
//										Position[j]->setString(recordg[govnameindex]);
//									}
//								}
//								if(Purchase[j].valid())
//								{
//									Purchase[j]->setVisiable(true);
//								}
//								if(Category[j].valid())
//								{
//									Category[j]->setString(std::string("国家贡献"));
//								}
//							}
//							++ j;
//						}
//					}
//					int totalpage = ceil((float)saleitemcount/(float)onepagecount);
//					canvasdata -> inputParam(WCHDATA_JXJGovPostShopTotalPage,&totalpage);
//					if (totalpage == 0)
//					{
//						totalpage = 1;
//					}
//					if (pageNumber.valid())
//					{
//						pageNumber->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
//					}
//				}
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJMyCounrtyFuliCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyCounrtyFuliCanvasUpdateMethod::crJXJMyCounrtyFuliCanvasUpdateMethod()
{
}

crJXJMyCounrtyFuliCanvasUpdateMethod::crJXJMyCounrtyFuliCanvasUpdateMethod(const crJXJMyCounrtyFuliCanvasUpdateMethod& handle):
	crMethod(handle)
{
}

void crJXJMyCounrtyFuliCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyCounrtyFuliCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_sw111_12 = str;
		break;
	case 1:
		m_sw111_13 = str;
		break;
	case 2:
		m_Country7_Choose1 = str;
		break;
	case 3:
		m_Country7_Fete = str;
		break;
	case 4:
		m_Country7_Worship = str;
		break;
	case 5:
		m_Country7_Choose2 = str;
		break;
	case 6:
		m_Country7_Economy = str;
		break;
	case 7:
		m_Country7_Interface2 = str;
		break;
	case 8:
		m_Country7_Function1 = str;
		break;
	case 9:
		m_Country7_Function2 = str;
		break;
	case 10:
		m_Country7_Function3 = str;
		break;
	default:
		break;
	}
}

void crJXJMyCounrtyFuliCanvasUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		//crData *canvasdata = m_this->getDataClass();
		//crData *data = playerGameData->getDataClass();
	}
}
/////////////////////////////////////////
//
//crJXJMyCounrtyHistoryCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyCounrtyHistoryCanvasUpdateMethod::crJXJMyCounrtyHistoryCanvasUpdateMethod():
	m_this(NULL)
{
}

crJXJMyCounrtyHistoryCanvasUpdateMethod::crJXJMyCounrtyHistoryCanvasUpdateMethod(const crJXJMyCounrtyHistoryCanvasUpdateMethod& handle):
	crMethod(handle),
	m_CountryRadio		 (handle.m_CountryRadio)	   ,//国家历史切换
	m_CountryRadioButton1(handle.m_CountryRadioButton1),//大事件
	m_CountryRadioButton2(handle.m_CountryRadioButton2),//官员变迁
	m_CountryRadioButton3(handle.m_CountryRadioButton3),//国家福利
	m_CountryRadioButton4(handle.m_CountryRadioButton4),//城池
	m_CountryRadioButton5(handle.m_CountryRadioButton5),//州牧任免
	m_CountryRadioButton6(handle.m_CountryRadioButton6),//国家建设
	m_CountryHistory	 (handle.m_CountryHistory	 ) ,
	m_scroll			 (handle.m_scroll			 )
{
}

void crJXJMyCounrtyHistoryCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyCounrtyHistoryCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_CountryRadio = str;
		break;
	case 1:
		m_CountryRadioButton1 = str;
		break;
	case 2:
		m_CountryRadioButton2 = str;
		break;
	case 3:
		m_CountryRadioButton3 = str;
		break;
	case 4:
		m_CountryRadioButton4 = str;
		break;
	case 5:
		m_CountryRadioButton5 = str;
		break;
	case 6:
		m_CountryRadioButton6 = str;
		break;
	case 7:
		m_CountryHistory = str;
		break;
	//case 8:
	//	m_scroll = str;
	//	break;
	default:
		break;
	}
}

void crJXJMyCounrtyHistoryCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crData *canvasdata = m_this->getDataClass();
		ref_ptr<crRadioGroupWidgetNode> radio  = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_CountryRadio));
		ref_ptr<crHypertextWidgetNode> history = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_CountryHistory));
		int index = 0;
		if (radio.valid())
		{
			index = radio->getSelect();
		}
		void *param;
		canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJCountryHistory,param);
		UIHistoryVec & historyVec = *(UIHistoryVec *)param;

		std::string text;
		
		history->clearText();
		if (!historyVec[index].empty())
		{
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			CountryHistory::reverse_iterator chItr = historyVec[index].rbegin();
			for (short i = 1; chItr != historyVec[index].rend(); ++ chItr, ++ i)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content",i);
				(*chItr)->getCompleteString(text);
				cfg_script.Add("Text",text + " /n");
				cfg_script.Pop();
			}
			cfg_script.Pop();
			history->setHypertext(cfg_script);
		}
		canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJCounrtyShopChangePageCanvasUpdateMethod
//
/////////////////////////////////////////
JXJ::crJXJCounrtyShopLeftPageCanvasUpdateMethod::crJXJCounrtyShopLeftPageCanvasUpdateMethod()
{
}

JXJ::crJXJCounrtyShopLeftPageCanvasUpdateMethod::crJXJCounrtyShopLeftPageCanvasUpdateMethod(const crJXJCounrtyShopLeftPageCanvasUpdateMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_Country_LeftPage(handle.m_Country_LeftPage),
	m_Country_PageNumber(handle.m_Country_PageNumber),
	m_Country_RightPage(m_Country_RightPage),
	m_Country_Select(m_Country_Select)
{

	for (unsigned char i = 0; i < 5; i++)
	{
		m_Country_Image[i] = handle.m_Country_Image[i];
		m_Country_Name[i] = handle.m_Country_Name[i];
		m_Country_Explain[i] = handle.m_Country_Explain[i];
		m_Country_Position[i] = handle.m_Country_Position[i];
		m_Country_Number[i] = handle.m_Country_Number[i];
		m_Country_Category[i] = handle.m_Country_Category[i];
		m_Country_Purchase[i] = handle.m_Country_Purchase[i];
	}
}

void JXJ::crJXJCounrtyShopLeftPageCanvasUpdateMethod::inputParam(int i, void *param)
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

void JXJ::crJXJCounrtyShopLeftPageCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Country_Image[0] = str;
		break;
	case 1:
		m_Country_Image[1] = str;
		break;
	case 2:
		m_Country_Image[2] = str;
		break;
	case 3:
		m_Country_Image[3] = str;
		break;
	case 4:
		m_Country_Image[4] = str;
		break;
	case 5:
		m_Country_Name[0] = str;
		break;
	case 6:
		m_Country_Name[1] = str;
		break;
	case 7:
		m_Country_Name[2] = str;
		break;
	case 8:
		m_Country_Name[3] = str;
		break;
	case 9:
		m_Country_Name[4] = str;
		break;
	case 10:
		m_Country_Explain[0] = str;
		break;
	case 11:
		m_Country_Explain[1] = str;
		break;
	case 12:
		m_Country_Explain[2] = str;
		break;
	case 13:
		m_Country_Explain[3] = str;
		break;
	case 14:
		m_Country_Explain[4] = str;
		break;
	case 15:
		m_Country_Position[0] = str;
		break;
	case 16:
		m_Country_Position[1] = str;
		break;
	case 17:
		m_Country_Position[2] = str;
		break;
	case 18:
		m_Country_Position[3] = str;
		break;
	case 19:
		m_Country_Position[4] = str;
		break;
	case 20:
		m_Country_Number[0] = str;
		break;
	case 21:
		m_Country_Number[1] = str;
		break;
	case 22:
		m_Country_Number[2] = str;
		break;
	case 23:
		m_Country_Number[3] = str;
		break;
	case 24:
		m_Country_Number[4] = str;
		break;
	case 25:
		m_Country_Category[0] = str;
		break;
	case 26:
		m_Country_Category[1] = str;
		break;
	case 27:
		m_Country_Category[2] = str;
		break;
	case 28:
		m_Country_Category[3] = str;
		break;
	case 29:
		m_Country_Category[4] = str;
		break;
	case 30:
		m_Country_Purchase[0] = str;
		break;
	case 31:
		m_Country_Purchase[1] = str;
		break;
	case 32:
		m_Country_Purchase[2] = str;
		break;
	case 33:
		m_Country_Purchase[3] = str;
		break;
	case 34:
		m_Country_Purchase[4] = str;
		break;
	case 35:
		m_Country_LeftPage = str;
		break;
	case 36:
		m_Country_PageNumber = str;
		break;
	case 37:
		m_Country_RightPage = str;
		break;
	case 38:
		m_Country_Select = str;
		break;
	case 39:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCounrtyShopLeftPageCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	if (canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode> pageNumber = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_PageNumber));
		int pageIndex = 0;
		if (pageNumber.valid())
		{
			pageIndex = atoi(pageNumber->getUTF8String().c_str());
			if (pageIndex == 1)
			{
				return;
			}
		}
		ref_ptr<crImageBoxWidgetNode> Image[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Name[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Explain[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Position[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Number[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Category[5] = {NULL};
		ref_ptr<crButtonWidgetNode> Purchase[5] = {NULL};
		ref_ptr<crButtonWidgetNode> left = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_LeftPage));
		ref_ptr<crButtonWidgetNode> right = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_RightPage));
		ref_ptr<crCheckBoxWidgetNode> showAll = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_Country_Select));
		for (int i = 0; i < 5; i++)
		{
			Image[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_Country_Image[i]));
			Name[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Name[i]));
			Explain[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Explain[i]));
			Position[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Position[i]));
			Number[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Number[i]));
			Category[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Category[i]));
			Purchase[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_Purchase[i]));
			if (Image[i].valid())Image[i]->setVisiable(false);
			if (Name[i].valid())Name[i]->clearString();
			if (Explain[i].valid())Explain[i]->clearString();
			if (Position[i].valid())Position[i]->clearString();
			if (Number[i].valid())Number[i]->clearString();
			if (Category[i].valid())Category[i]->clearString();
			if (Purchase[i].valid())Purchase[i]->setVisiable(false);
		}
		//bool ifShowAll = true;
		//if (showAll.valid())
		//{
		//	if (showAll->getSelect())
		//	{
		//		ifShowAll = false;
		//	}
		//}


		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> govpostshoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostShopTab);
		ref_ptr<crTableIO> govposttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		if(m_this && playerGameData && itemtab.valid() && govpostshoptab.valid() && govposttab.valid()
			&& playerGameData->getMainRole())
		{
			crData *data = playerGameData->getDataClass();
			crData *mainroledata = playerGameData->getMainRole()->getDataClass();
			if (data && mainroledata)
			{
				void *param;
				mainroledata -> getParam(WCHDATA_JXJGovPost,param);
				unsigned char govPost = *(unsigned char *)param;
				data -> getParam(WCHDATA_JXJCountryContribute,param);
				int mycountrycontribute = *(int *)param;
				crTableIO::StrVec record;
				crTableIO::StrVec recordg;
				//int scrollvalue = 0;
				//int row = 0;
				int showcurpos = 0;
				int rowcount = govpostshoptab->getRowCount();
				int govnameindex = govposttab->getTitleIndex("govpost");
				int infoindex = itemtab->getTitleIndex("物品描述");
				int colorindex = itemtab->getTitleIndex("品质");
				int iconindex = itemtab->getTitleIndex("icon");
				int nameindex = itemtab->getTitleIndex("name");
				int priceindex = govpostshoptab->getTitleIndex("消耗贡献");
				int govpostindex = govpostshoptab->getTitleIndex("官职需求");
				if (infoindex >= 0 && colorindex >= 0 && iconindex >= 0 && priceindex >= 0 && govpostindex >= 0)
				{
					std::string pricestr;
					std::string itemidstr;
					int saleitemcount = 0;
					int price = 0;
					//int itemid = 0;
					unsigned char needgovpost = 0;
					bool ifshow = false;
					if (!showcurpos)
					{
						saleitemcount = rowcount;
					}
					int onepagecount = 5;
					for (int i = 0 + (pageIndex-1)*onepagecount, j = 0; i < rowcount; i++)
					{
						ifshow = false;
						pricestr = (*govpostshoptab)(i,priceindex);
						price =  atoi(pricestr.c_str());
						itemidstr = (*govpostshoptab)(i,0);
						//itemid =  atoi(itemidstr.c_str());
						needgovpost = atoi((*govpostshoptab)(i,govpostindex).c_str());
						if (showcurpos)
						{
							if (needgovpost == govPost)
							{
								ifshow = true;
								saleitemcount ++;
							}
						}
						else
						{
							ifshow = true;
						}
						if (ifshow)
						{
							if (j >= 0 && j < onepagecount)
							{
								if (itemtab->queryOneRecord(0,itemidstr,record)>=0)
								{
									if(Image[j].valid())
									{
										Image[j]->setVisiable(true);
										Image[j]->setImageName(record[iconindex]);
									}
									if(Name[j].valid())
									{
										Name[j]->setString(record[nameindex]);
									}
									if(Number[j].valid())
									{
										if (mycountrycontribute>=price)
										{
											Number[j]->setColor(crVector4f(1.0f,1.0f,1.0f,1.0f));
										}
										else
										{
											Number[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
										}
										Number[j]->setString(pricestr);
									}
									if(Explain[j].valid())
									{
										Explain[j]->setString(record[infoindex]);
									}
									if(Position[j].valid())
									{
										if (govPost >= needgovpost)
										{
											Position[j]->setColor(crVector4f(210.0f,220.0f,200.0f,255.0f)/255.0f);
										}
										else
										{
											Position[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
										}

										if (govposttab->queryOneRecord(0,crArgumentParser::appItoa(needgovpost),recordg)>=0)
										{
											Position[j]->setString(recordg[govnameindex]);
										}
									}
									if(Purchase[j].valid())
									{
										Purchase[j]->setVisiable(true);
									}
									if(Category[j].valid())
									{
										Category[j]->setString(std::string("国家贡献"));
									}
								}
							}
							j++;
						}
					}
					int totalpage = ceil((float)saleitemcount/(float)onepagecount);
					if (totalpage == 0)
					{
						totalpage = 1;
					}
					if (pageNumber.valid())
					{
						pageNumber->setString(crArgumentParser::appItoa(pageIndex-1) + "/" + crArgumentParser::appItoa(totalpage));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCounrtyShopRightPageCanvasUpdateMethod
//
/////////////////////////////////////////
JXJ::crJXJCounrtyShopRightPageCanvasUpdateMethod::crJXJCounrtyShopRightPageCanvasUpdateMethod()
{
}

JXJ::crJXJCounrtyShopRightPageCanvasUpdateMethod::crJXJCounrtyShopRightPageCanvasUpdateMethod(const crJXJCounrtyShopRightPageCanvasUpdateMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_Country_LeftPage(handle.m_Country_LeftPage),
	m_Country_PageNumber(handle.m_Country_PageNumber),
	m_Country_RightPage(m_Country_RightPage),
	m_Country_Select(m_Country_Select)
{

	for (unsigned char i = 0; i < 5; i++)
	{
		m_Country_Image[i] = handle.m_Country_Image[i];
		m_Country_Name[i] = handle.m_Country_Name[i];
		m_Country_Explain[i] = handle.m_Country_Explain[i];
		m_Country_Position[i] = handle.m_Country_Position[i];
		m_Country_Number[i] = handle.m_Country_Number[i];
		m_Country_Category[i] = handle.m_Country_Category[i];
		m_Country_Purchase[i] = handle.m_Country_Purchase[i];
	}
}

void JXJ::crJXJCounrtyShopRightPageCanvasUpdateMethod::inputParam(int i, void *param)
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

void JXJ::crJXJCounrtyShopRightPageCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Country_Image[0] = str;
		break;
	case 1:
		m_Country_Image[1] = str;
		break;
	case 2:
		m_Country_Image[2] = str;
		break;
	case 3:
		m_Country_Image[3] = str;
		break;
	case 4:
		m_Country_Image[4] = str;
		break;
	case 5:
		m_Country_Name[0] = str;
		break;
	case 6:
		m_Country_Name[1] = str;
		break;
	case 7:
		m_Country_Name[2] = str;
		break;
	case 8:
		m_Country_Name[3] = str;
		break;
	case 9:
		m_Country_Name[4] = str;
		break;
	case 10:
		m_Country_Explain[0] = str;
		break;
	case 11:
		m_Country_Explain[1] = str;
		break;
	case 12:
		m_Country_Explain[2] = str;
		break;
	case 13:
		m_Country_Explain[3] = str;
		break;
	case 14:
		m_Country_Explain[4] = str;
		break;
	case 15:
		m_Country_Position[0] = str;
		break;
	case 16:
		m_Country_Position[1] = str;
		break;
	case 17:
		m_Country_Position[2] = str;
		break;
	case 18:
		m_Country_Position[3] = str;
		break;
	case 19:
		m_Country_Position[4] = str;
		break;
	case 20:
		m_Country_Number[0] = str;
		break;
	case 21:
		m_Country_Number[1] = str;
		break;
	case 22:
		m_Country_Number[2] = str;
		break;
	case 23:
		m_Country_Number[3] = str;
		break;
	case 24:
		m_Country_Number[4] = str;
		break;
	case 25:
		m_Country_Category[0] = str;
		break;
	case 26:
		m_Country_Category[1] = str;
		break;
	case 27:
		m_Country_Category[2] = str;
		break;
	case 28:
		m_Country_Category[3] = str;
		break;
	case 29:
		m_Country_Category[4] = str;
		break;
	case 30:
		m_Country_Purchase[0] = str;
		break;
	case 31:
		m_Country_Purchase[1] = str;
		break;
	case 32:
		m_Country_Purchase[2] = str;
		break;
	case 33:
		m_Country_Purchase[3] = str;
		break;
	case 34:
		m_Country_Purchase[4] = str;
		break;
	case 35:
		m_Country_LeftPage = str;
		break;
	case 36:
		m_Country_PageNumber = str;
		break;
	case 37:
		m_Country_RightPage = str;
		break;
	case 38:
		m_Country_Select = str;
		break;
	case 39:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCounrtyShopRightPageCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	if (canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode> pageNumber = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_PageNumber));
		int pageIndex = 0;
		if (pageNumber.valid())
		{
			pageIndex = atoi(pageNumber->getUTF8String().c_str());
		}
		ref_ptr<crImageBoxWidgetNode> Image[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Name[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Explain[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Position[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Number[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode> Category[5] = {NULL};
		ref_ptr<crButtonWidgetNode> Purchase[5] = {NULL};
		ref_ptr<crButtonWidgetNode> left = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_LeftPage));
		ref_ptr<crButtonWidgetNode> right = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_RightPage));
		ref_ptr<crCheckBoxWidgetNode> showAll = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_Country_Select));
		for (int i = 0; i < 5; i++)
		{
			Image[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_Country_Image[i]));
			Name[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Name[i]));
			Explain[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Explain[i]));
			Position[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Position[i]));
			Number[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Number[i]));
			Category[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_Country_Category[i]));
			Purchase[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Country_Purchase[i]));
			if (Image[i].valid())Image[i]->setVisiable(false);
			if (Name[i].valid())Name[i]->clearString();
			if (Explain[i].valid())Explain[i]->clearString();
			if (Position[i].valid())Position[i]->clearString();
			if (Number[i].valid())Number[i]->clearString();
			if (Category[i].valid())Category[i]->clearString();
			if (Purchase[i].valid())Purchase[i]->setVisiable(false);
		}
		//bool ifShowAll = true;
		//if (showAll.valid())
		//{
		//	if (showAll->getSelect())
		//	{
		//		ifShowAll = false;
		//	}
		//}


		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> govpostshoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostShopTab);
		ref_ptr<crTableIO> govposttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		if(m_this && playerGameData && itemtab.valid() && govpostshoptab.valid() && govposttab.valid()
			&& playerGameData->getMainRole())
		{
			crData *data = playerGameData->getDataClass();
			crData *mainroledata = playerGameData->getMainRole()->getDataClass();
			if (data && mainroledata)
			{
				void *param;
				mainroledata -> getParam(WCHDATA_JXJGovPost,param);
				unsigned char govPost = *(unsigned char *)param;
				data -> getParam(WCHDATA_JXJCountryContribute,param);
				int mycountrycontribute = *(int *)param;
				crTableIO::StrVec record;
				crTableIO::StrVec recordg;
				//int scrollvalue = 0;
				//int row = 0;
				int showcurpos = 0;
				int rowcount = govpostshoptab->getRowCount();
				int govnameindex = govposttab->getTitleIndex("govpost");
				int infoindex = itemtab->getTitleIndex("物品描述");
				int colorindex = itemtab->getTitleIndex("品质");
				int iconindex = itemtab->getTitleIndex("icon");
				int nameindex = itemtab->getTitleIndex("name");
				int priceindex = govpostshoptab->getTitleIndex("消耗贡献");
				int govpostindex = govpostshoptab->getTitleIndex("官职需求");
				if (infoindex >= 0 && colorindex >= 0 && iconindex >= 0 && priceindex >= 0 && govpostindex >= 0)
				{
					std::string pricestr;
					std::string itemidstr;
					int saleitemcount = 0;
					int price = 0;
					//int itemid = 0;
					unsigned char needgovpost = 0;
					bool ifshow = false;
					if (!showcurpos)
					{
						saleitemcount = rowcount;
					}
					int onepagecount = 5;
					for (int i = 0 + (pageIndex-1)*onepagecount, j = 0; i < rowcount; i++)
					{
						ifshow = false;
						pricestr = (*govpostshoptab)(i,priceindex);
						price =  atoi(pricestr.c_str());
						itemidstr = (*govpostshoptab)(i,0);
						//itemid =  atoi(itemidstr.c_str());
						needgovpost = atoi((*govpostshoptab)(i,govpostindex).c_str());
						if (showcurpos)
						{
							if (needgovpost == govPost)
							{
								ifshow = true;
								saleitemcount ++;
							}
						}
						else
						{
							ifshow = true;
						}

						int totalpage = ceil((float)saleitemcount/(float)onepagecount);
						if (totalpage == 0)
						{
							totalpage = 1;
						}
						if (pageIndex == totalpage)return;
						if (ifshow)
						{
							if (pageNumber.valid())
							{
								pageNumber->setString(crArgumentParser::appItoa(pageIndex+1) + "/" + crArgumentParser::appItoa(totalpage));
							}
							if (j >= 0 && j < onepagecount)
							{
								if (itemtab->queryOneRecord(0,itemidstr,record)>=0)
								{
									if(Image[j].valid())
									{
										Image[j]->setVisiable(true);
										Image[j]->setImageName(record[iconindex]);
									}
									if(Name[j].valid())
									{
										Name[j]->setString(record[nameindex]);
									}
									if(Number[j].valid())
									{
										if (mycountrycontribute>=price)
										{
											Number[j]->setColor(crVector4f(1.0f,1.0f,1.0f,1.0f));
										}
										else
										{
											Number[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
										}
										Number[j]->setString(pricestr);
									}
									if(Explain[j].valid())
									{
										Explain[j]->setString(record[infoindex]);
									}
									if(Position[j].valid())
									{
										if (govPost >= needgovpost)
										{
											Position[j]->setColor(crVector4f(210.0f,220.0f,200.0f,255.0f)/255.0f);
										}
										else
										{
											Position[j]->setColor(crVector4f(0.8f,0.2f,0.1f,1.0f));
										}

										if (govposttab->queryOneRecord(0,crArgumentParser::appItoa(needgovpost),recordg)>=0)
										{
											Position[j]->setString(recordg[govnameindex]);
										}
									}
									if(Purchase[j].valid())
									{
										Purchase[j]->setVisiable(true);
									}
									if(Category[j].valid())
									{
										Category[j]->setString(std::string("国家贡献"));
									}
								}
							}
							j++;
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJOnCountryBuildingPageMethod
//
/////////////////////////////////////////
crJXJOnCountryBuildingPageMethod::crJXJOnCountryBuildingPageMethod()
{
}

crJXJOnCountryBuildingPageMethod::crJXJOnCountryBuildingPageMethod(const crJXJOnCountryBuildingPageMethod& handle):
	crMethod(handle)
{
}

void crJXJOnCountryBuildingPageMethod::inputParam(int i, void *param)
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

void crJXJOnCountryBuildingPageMethod::addParam(int i, const std::string& str)
{
}

void crJXJOnCountryBuildingPageMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor && playerGameData)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvOnCountryBuildingPage,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJBuildMyCountryMethod
//
/////////////////////////////////////////
crJXJBuildMyCountryMethod::crJXJBuildMyCountryMethod():
	m_canvas(""),
	m_radio("")
{
}

crJXJBuildMyCountryMethod::crJXJBuildMyCountryMethod(const crJXJBuildMyCountryMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_radio(handle.m_radio)
{
}

void crJXJBuildMyCountryMethod::inputParam(int i, void *param)
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

void crJXJBuildMyCountryMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radio = str;
		break;
	default:
		break;
	}
}

void crJXJBuildMyCountryMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if (canvas.valid()&&playerGameData&&netConductor)
	{
		ref_ptr<crRadioGroupWidgetNode> radio =  dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio));
		unsigned char select;
		if (radio.valid())
		{
			select = radio->getSelect();
		}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(2);
		stream->_writeUChar(select);
		stream->_writeUChar(0);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuildMyCountry,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJCountryEditNoticeMethod
//
/////////////////////////////////////////
crJXJCountryEditNoticeMethod::crJXJCountryEditNoticeMethod():
	m_canvas(""),
	m_notice(""),
	m_noticeEdit(""),
	m_noticePublish("")
{
}

crJXJCountryEditNoticeMethod::crJXJCountryEditNoticeMethod(const crJXJCountryEditNoticeMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_notice(handle.m_notice),
	m_noticeEdit(handle.m_noticeEdit),
	m_noticePublish(handle.m_noticePublish)
{
}

void crJXJCountryEditNoticeMethod::inputParam(int i, void *param)
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

void crJXJCountryEditNoticeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_notice = str;
		break;
	case 2:
		m_noticeEdit = str;
		break;
	case 3:
		m_noticePublish = str;
		break;
	default:
		break;
	}
}
void crJXJCountryEditNoticeMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if (canvas.valid()&&playerGameData&&netConductor)
	{
		ref_ptr<crButtonWidgetNode> edit = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_noticeEdit));		
		ref_ptr<crButtonWidgetNode> publish = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_noticePublish));
		ref_ptr<crEditWidgetNode> text = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_notice));
		if (edit.valid()&&publish.valid()&&text.valid())
		{
			text->setEnable(true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCountryPublishNoticeMethod
//
/////////////////////////////////////////
crJXJCountryPublishNoticeMethod::crJXJCountryPublishNoticeMethod():
	m_canvas(""),
	m_notice(""),
	m_noticeEdit(""),
	m_noticePublish("")
{
}

crJXJCountryPublishNoticeMethod::crJXJCountryPublishNoticeMethod(const crJXJCountryPublishNoticeMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_notice(handle.m_notice),
	m_noticeEdit(handle.m_noticeEdit),
	m_noticePublish(handle.m_noticePublish)
{
}

void crJXJCountryPublishNoticeMethod::inputParam(int i, void *param)
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

void crJXJCountryPublishNoticeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_notice = str;
		break;
	case 2:
		m_noticeEdit = str;
		break;
	case 3:
		m_noticePublish = str;
		break;
	default:
		break;
	}
}
void crJXJCountryPublishNoticeMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if (canvas.valid()&&netConductor)
	{
		ref_ptr<crEditWidgetNode> text = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_notice));
		if (text.valid())
		{
			text->setEnable(false);
			std::string noticeContents  = text->getUTF8String();
			CRUI::crTextAttrWidgetNode::transformText(noticeContents);
			if (noticeContents.length() <= 400)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(noticeContents.length()+4);
				stream->_writeString(noticeContents);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeCountryNotice,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			else
			{
				crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
				if (playerGameData)
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(5018,NULL));
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvModifyCountryNoticeMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvModifyCountryNoticeMethod::crJXJRecvModifyCountryNoticeMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvModifyCountryNoticeMethod::crJXJRecvModifyCountryNoticeMethod( const crJXJRecvModifyCountryNoticeMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvModifyCountryNoticeMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvModifyCountryNoticeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}
void JXJ::crJXJRecvModifyCountryNoticeMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			std::string notice = m_stream->_readString();
			if (notice.length() > 400)
			{
				notice = notice.substr(0,400);
			}
			void* param;
			crData *data = m_this->getDataClass();
			if (data)
			{
				int playerid = m_this->getPlayerID();
				unsigned char retVal = JXJEditNoticeRetVal_failed_unkwon;
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *( unsigned char *)param;

				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					do 
					{
						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if (!shili || shili->getDataClass() == NULL) break;
						crJXJShiliData *shilidata = dynamic_cast<crJXJShiliData *>(shili->getDataClass());
						if (!shilidata) break;
						if (shilidata->getOfficialDes(playerid) == JXJCountryOfficial_normal)
						{
							retVal = JXJEditNoticeRetVal_failed_noPermission;
							break;
						}

						shilidata->inputParam(WCHDATA_JXJCountryNotice,&notice);
						bool modify = true;
						shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
						retVal = JXJEditNoticeRetVal_suc;
					} while (0);
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(playerData.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					if (retVal == JXJEditNoticeRetVal_suc)
					{
						stream->createBuf(notice.length()+4);
						stream->_writeUChar(retVal);
						stream->_writeString(notice);
					}
					else
					{
						stream->createBuf(1);
						stream->_writeUChar(retVal);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeCountryNotice,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char retVal = m_stream->_readUChar();
			int msgid = 0;
			switch (retVal)
			{
			case JXJEditNoticeRetVal_failed_noPermission:
				msgid = 3051;
				break;
			case JXJEditNoticeRetVal_suc:
				{
					msgid = 3050;
					std::string notice = m_stream->_readString();
					do 
					{
						ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
						if (!canvas) break;
						crData * canvasData = canvas->getDataClass();
						if (canvasData) break;
						canvasData->inputParam(WCHDATA_JXJMyCountryNotice,&notice);
					} while (0);
				}
				break;
			default:
				break;
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}
/////////////////////////////////////////
//
//crJXJQueryCountryHistoryMethod
//
/////////////////////////////////////////
crJXJQueryCountryHistoryMethod::crJXJQueryCountryHistoryMethod()
{
}

crJXJQueryCountryHistoryMethod::crJXJQueryCountryHistoryMethod(const crJXJQueryCountryHistoryMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryCountryHistoryMethod::inputParam(int i, void *param)
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

void crJXJQueryCountryHistoryMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryCountryHistoryMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryHistory,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/************************************************************************/
/*    crJXJRecvCountryHistoryMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvCountryHistoryMethod::crJXJRecvCountryHistoryMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvCountryHistoryMethod::crJXJRecvCountryHistoryMethod( const crJXJRecvCountryHistoryMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvCountryHistoryMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvCountryHistoryMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}
void JXJ::crJXJRecvCountryHistoryMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void* param;
			crData *data = m_this->getDataClass();
			if (data)
			{
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *( unsigned char *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					if (shili && shili->getDataClass())
					{
						crData *shilidata = shili->getDataClass();	
						if (shilidata)
						{
							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
							shilidata->getParam(WCHDATA_JXJCountryHistory,param);
							CountryHistory* countryHistory = (CountryHistory*)param;

							int maxSize = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryHistoryMaxNum,viplv).c_str()));
							while (countryHistory->size() > maxSize)
							{
								countryHistory->pop_front();
							}
							int iSize = countryHistory->size();
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(2048);
							stream->_writeInt(iSize);
							for (CountryHistory::iterator itr = countryHistory->begin(); itr != countryHistory->end(); ++itr)
							{
								(*itr)->writeContent(stream);
							}

							shilidata->excHandle(MAKEINT64(WCH_LockData,0));
							int playerid = m_this->getPlayerID();
							crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
							ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
							if(playerData.valid())
							{
								crPlayerDataEventZipPacket packet;
								crPlayerDataEventZipPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCountryHistory,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				crData * canvasData = canvas->getDataClass();
				void * param;
				
				canvasData->excHandle(MAKEINT64(WCH_LockData,1));
				canvasData->getParam(WCHDATA_JXJCountryHistory,param);
				UIHistoryVec & histroyVec = *(UIHistoryVec *)param;
				histroyVec.clear();
				histroyVec.resize(JXJCountryHistoryType_max);
				int nHistoryNumber = m_stream->_readInt();
				CRCore::ref_ptr<CountryHistoryInfoBase> ptr;
				CountryHistoryManager creator;
				for (int i = 0; i < nHistoryNumber; i++)
				{
					int nType  = m_stream->_readUChar();
					creator(nType,ptr);
					if (ptr.valid())
					{
						ptr->readContent(m_stream);
						histroyVec[JXJCountryHistoryType_all].push_back(ptr);
						histroyVec[nType].push_back(ptr);
					}
				}
				canvasData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJQueryCountryToupiaoInfoMethod
//
/////////////////////////////////////////
crJXJQueryCountryToupiaoInfoMethod::crJXJQueryCountryToupiaoInfoMethod()
{
}

crJXJQueryCountryToupiaoInfoMethod::crJXJQueryCountryToupiaoInfoMethod(const crJXJQueryCountryToupiaoInfoMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryCountryToupiaoInfoMethod::inputParam(int i, void *param)
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

void crJXJQueryCountryToupiaoInfoMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryCountryToupiaoInfoMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryToupiao,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJQueryCountryWillBeLeaderMethod
//
/////////////////////////////////////////
crJXJQueryCountryWillBeLeaderMethod::crJXJQueryCountryWillBeLeaderMethod()
{
}

crJXJQueryCountryWillBeLeaderMethod::crJXJQueryCountryWillBeLeaderMethod(const crJXJQueryCountryWillBeLeaderMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryCountryWillBeLeaderMethod::inputParam(int i, void *param)
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

void crJXJQueryCountryWillBeLeaderMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryCountryWillBeLeaderMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryWillBeLeader,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/************************************************************************/
/*    crJXJRecvToupiaoMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvToupiaoMethod::crJXJRecvToupiaoMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvToupiaoMethod::crJXJRecvToupiaoMethod( const crJXJRecvToupiaoMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvToupiaoMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvToupiaoMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}
void JXJ::crJXJRecvToupiaoMethod::operator()( crHandle & handle )
{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			std::string notice = m_stream->_readString();
//			void* param;
//			crData *data = m_this->getDataClass();
//			if (data)
//			{
//				data->getParam(WCHDATA_JXJShiliID,param);
//				unsigned char shiliid = *( unsigned char *)param;
//				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
//				if (gsBrainData)
//				{
//					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
//					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
//					crJXJShili *shili = (crJXJShili *)param;
//					if (shili && shili->getDataClass())
//					{
//						crData *shilidata = shili->getDataClass();	
//						if (shilidata)
//						{
//							ref_ptr<crStreamBuf> stream = new crStreamBuf;
//							stream->createBuf(8196);
//							//写入已投票玩家ID
//							shilidata->getParam(WCHDATA_JXJCountryToupiaoMap,param);
//							PeopleMap *peopleMap = (PeopleMap*)param;
//							int nSize = peopleMap->size();
//							stream->_writeInt(nSize);
//							for (PeopleMap::iterator it = peopleMap->begin(); it != peopleMap->end(); it++)
//							{
//								stream->_writeInt(it->first);
//							}
//							//写入本周投票数据
//							shilidata->getParam(WCHDATA_JXJCountryToupiaoMap,param);
//							ToupiaoData *toupiaoData = (ToupiaoData*)param;
//							nSize = toupiaoData->size();
//							stream->_writeInt(nSize);
//							for (ToupiaoData::iterator it = toupiaoData->begin(); it != toupiaoData->end(); it++)
//							{
//								stream->_writeInt(it->second->nPlayerID);
//								stream->_writeString(it->second->playerName);
//								stream->_writeInt(it->second->nJuntuanID);
//								stream->_writeString(it->second->juntuanName);
//								stream->_writeInt(it->second->nRankNumber);
//								stream->_writeInt(it->second->nNumber);
//							}
//							//写入上周投票结果
//							shilidata->getParam(WCHDATA_JXJCountryToupiaoLast,param);
//							ToupiaoData *toupiaoDataLast = (ToupiaoData*)param;
//							nSize = toupiaoDataLast->size();
//							for (ToupiaoData::iterator it = toupiaoDataLast->begin(); it != toupiaoDataLast->end(); it++)
//							{
//								stream->_writeInt(it->second->nPlayerID);
//								stream->_writeString(it->second->playerName);
//								stream->_writeInt(it->second->nJuntuanID);
//								stream->_writeString(it->second->juntuanName);
//								stream->_writeInt(it->second->nRankNumber);
//								stream->_writeInt(it->second->nNumber);
//							}
//							//bool modify = true;
//							//shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
//							gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
//
//							int playerid = m_this->getPlayerID();
//							crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//							ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//							
//							stream->_writeString(notice);
//							crPlayerDataEventPacket packet;
//							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCountryToupiao,stream.get());
//							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//						}
//					}
//				}
//			}
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			void* param;
//			crData *braindata = crBrain::getInstance()->getDataClass();
//			braindata->getParam(WCHDATA_JXJMyCountryData,param);
//			crJXJShili *shili = (crJXJShili *)param;
//			if (shili && shili->getDataClass())
//			{
//				crData *shilidata = shili->getDataClass();
//				if (shilidata)
//				{
//					//已投票玩家ID
//					int nSize = m_stream->_readInt();
//					PeopleMap people;
//					for(int i = 0; i < nSize; i++)
//					{	
//						nSize = m_stream->_readInt();
//						people.insert(std::make_pair(nSize, 1));
//					}
//					shilidata->inputParam(WCHDATA_JXJCountryVotersSet, &people);
//
//					//本周投票数据
//					nSize = m_stream->_readInt();
//					PeopleMap peopleData;
//					for (int i = 0; i < nSize; i++)
//					{
//						int nFrist = m_stream->_readInt();
//						int nSecond = m_stream->_readInt();
//						peopleData.insert(std::make_pair(nFrist, nSecond));
//					}
//					shilidata->inputParam(WCHDATA_JXJCountryToupiaoMap, &peopleData);
//
//					//上周投票数据
//					nSize = m_stream->_readInt();
//					PeopleMap peopleDataLast;
//					for (int i = 0; i < nSize; i++)
//					{
//						int nFrist = m_stream->_readInt();
//						int nSecond = m_stream->_readInt();
//						peopleDataLast.insert(std::make_pair(nFrist, nSecond));
//					}
//					shilidata->inputParam(WCHDATA_JXJCountryToupiaoMap, &peopleDataLast);
//
//					bool modify = true;
//					shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
//				}
//			}
//		}
//	}
}
/************************************************************************/
/*    crJXJRecvCountryWillBeLeaderMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvCountryWillBeLeaderMethod::crJXJRecvCountryWillBeLeaderMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvCountryWillBeLeaderMethod::crJXJRecvCountryWillBeLeaderMethod( const crJXJRecvCountryWillBeLeaderMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvCountryWillBeLeaderMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvCountryWillBeLeaderMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}
void JXJ::crJXJRecvCountryWillBeLeaderMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			//unsigned char success = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && data)
			{
				void *param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *( unsigned char *)param;
				if(shiliid >= c_startShiliID)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					if(shili && shili->getDataClass())
					{
						crData *shilidata = shili->getDataClass();
						shilidata->excHandle(MAKEINT64(WCH_LockData,1));
						shilidata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
						CandidateVec *candidateVec  = (CandidateVec *) param;

						shilidata->getParam(WCHDATA_JXJCountryCandidateVecL,param);
						CandidateVec *candidateVecL  = (CandidateVec *) param;


						shilidata->getParam(WCHDATA_JXJCountryVotersSet,param);
						VotersSet * votersSet = (VotersSet *)param;

						//gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
						//ConsortialvRank *consortialvRank = (ConsortialvRank *)param;

						gsBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
						LevelRankingMap *levelRankingMap = (LevelRankingMap *)param;

						//gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						//ConsortiaMap *consortiamap = (ConsortiaMap *)param;

						//int startDay = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryVoteStartDay).c_str()));
						//int minNum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryVotePeopleMinNum).c_str()));
						//int maxum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryVotePeopleMaxNum).c_str()));

						//if (candidateVec->empty())
						//{
						//	ConsortialvRank::RankSet & rankSet = consortialvRank->getRankSet();
						//	ConsortialvRank::RankSet::iterator rsItr = rankSet.begin();
						//	ConsortiaMap::iterator cmItr;
						//	for (; candidateVec->size() < maxum && rsItr != rankSet.end(); ++ rsItr)
						//	{
						//		if (rsItr->valid())
						//		{
						//			cmItr = consortiamap->find((*rsItr)->m_id);
						//			if (cmItr == consortiamap->end() || cmItr->second.valid() == false) continue;
						//			if (cmItr->second->getShiLi() != shiliid) continue;
						//			crData * groupData = cmItr->second->getDataClass();
						//			if (groupData)
						//			{
						//				groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						//				ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
						//				ConsortiaMemberMap::iterator cmmItr;
						//				groupData->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
						//				GroupLeaderVec *groupLeaderVec = (GroupLeaderVec *)param;
						//				GroupLeaderVec::iterator glvItr = groupLeaderVec->begin();
						//				for (; candidateVec->size() < maxum && glvItr != groupLeaderVec->end(); ++ glvItr)
						//				{
						//					cmmItr = consortiaMemberMap->find(*glvItr);
						//					if (cmmItr != consortiaMemberMap->end())
						//					{
						//						CRCore::ref_ptr<strToupiaoInfo> strinfo = new strToupiaoInfo;
						//						strinfo->nPlayerID = *glvItr;
						//						strinfo->playerName = cmmItr->second->getName();
						//						strinfo->nJuntuanID = cmItr->second->getID();
						//						cmItr->second->getName(strinfo->juntuanName);
						//						candidateVec->push_back(strinfo);
						//					}
						//				}
						//			}
						//		}
						//	}
						//	//if (candidateVec->size() < minNum)
						//	//{
						//	//	candidateVec->clear();
						//	//}
						//	//else
						//	//{
						//	//	bool modify = true;
						//	//	shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
						//	//}
						//}
						//本周候选人
						if(!candidateVec->empty())
						{							
							for( CandidateVec::iterator wbItr = candidateVec->begin();
								wbItr != candidateVec->end(); 
								++ wbItr )
							{
								int rankVal = 0;
								LevelRankingMap::iterator lrmItr = levelRankingMap->begin(); 
								for (;lrmItr != levelRankingMap->end(); ++ rankVal,++ lrmItr)
								{
									if (lrmItr->second.valid())
									{
										if (lrmItr->second->getPlayerID() == (*wbItr)->nPlayerID)
										{
											(*wbItr)->m_level = lrmItr->second->getLevel();
											break;
										}
									}
								}
								(*wbItr)->m_rankVal = rankVal + 1;
							}
							std::sort(candidateVec->begin(),candidateVec->end(),strToupiaoInfoCmp());
							//bool modify = true;
							//shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
						}
						//上周候选人
						if(!candidateVecL->empty())
						{							
							for( CandidateVec::iterator wbItr = candidateVecL->begin();
								wbItr != candidateVecL->end(); 
								++ wbItr )
							{
								int rankVal = 0;
								LevelRankingMap::iterator lrmItr = levelRankingMap->begin(); 
								for (;lrmItr != levelRankingMap->end(); ++ rankVal,++ lrmItr)
								{
									if (lrmItr->second.valid())
									{
										if (lrmItr->second->getPlayerID() == (*wbItr)->nPlayerID)
										{
											break;
										}
									}
								}
								(*wbItr)->m_rankVal = rankVal + 1;
							}
							std::sort(candidateVecL->begin(),candidateVecL->end(),strToupiaoInfoCmp());
						}

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(866);
						VotersSet::iterator vsItr = votersSet->find(playerid);
						stream->_writeBool(vsItr != votersSet->end());
						//本周候选人
						unsigned char size = candidateVec->size();
						stream->_writeUChar(size);
						for( CandidateVec::iterator wbItr = candidateVec->begin();
							 wbItr != candidateVec->end(); 
							 ++ wbItr )
						{
							stream->_writeInt((*wbItr)->nPlayerID);//4
							//int rankVal = 0;
							//LevelRankingMap::iterator lrmItr = levelRankingMap->begin(); 
							//for (;lrmItr != levelRankingMap->end(); ++ rankVal,++ lrmItr)
							//{
							//	if (lrmItr->second.valid())
							//	{
							//		if (lrmItr->second->getPlayerID() == (*wbItr)->nPlayerID)
							//		{
							//			break;
							//		}
							//	}
							//}
							stream->_writeInt((*wbItr)->m_rankVal);//4
							stream->_writeString((*wbItr)->playerName);//20
							stream->_writeInt((*wbItr)->nJuntuanID);//4
							stream->_writeString((*wbItr)->juntuanName);//20
							stream->_writeInt((*wbItr)->m_level);
							stream->_writeInt((*wbItr)->nNumber);
						}
						//上周候选人
						unsigned char size_L = candidateVecL->size();
						stream->_writeUChar(size_L);
						for( CandidateVec::iterator wbItr = candidateVecL->begin();
							wbItr != candidateVecL->end(); 
							++ wbItr )
						{
							stream->_writeInt((*wbItr)->nPlayerID);//4
							stream->_writeString((*wbItr)->playerName);//20
							stream->_writeInt((*wbItr)->nNumber);
						}

						data->getParam(WCHDATA_JXJSelectLeaderSet,param);
						SelectLeaderIdSet * selectLeaderIdSet = (SelectLeaderIdSet *)param;
						stream->_writeUChar(selectLeaderIdSet->size());
						SelectLeaderIdSet::iterator slisItr = selectLeaderIdSet->begin();
						for (; slisItr != selectLeaderIdSet->end(); ++ slisItr)
						{
							stream->_writeInt(*slisItr);
						}
						shilidata->excHandle(MAKEINT64(WCH_LockData,0));
						if(playerData.valid())
						{
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCountryWillBeLeader,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Piaoshu");
			ref_ptr<crCanvasNode> canvas_2 = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Guanyuanxuanju");
			if (canvas.valid()&& canvas->getDataClass() && canvas_1.valid() && canvas_1->getDataClass() && canvas_2.valid() && canvas_2->getDataClass())
			{
				crData * myCountryData = canvas->getDataClass();
				void *param;
				bool haveVoted = m_stream->_readBool();
				myCountryData->inputParam(WCHDATA_JXJCountryCanvasHaveVoted,&haveVoted);

				myCountryData->excHandle(MAKEINT64(WCH_LockData,1));
				myCountryData->getParam(WCHDATA_JXJCountryCandidateVecT,param);
				CandidateVec *candidateVec = (CandidateVec *)param;
				candidateVec->clear();
				unsigned char size = m_stream->_readUChar();
				for (unsigned char i = 0; i < size; ++ i)
				{
					CRCore::ref_ptr<strToupiaoInfo> strinfo = new strToupiaoInfo;
					strinfo->nPlayerID = m_stream->_readInt();
					strinfo->m_rankVal = m_stream->_readInt();
					strinfo->playerName = m_stream->_readString();
					strinfo->nJuntuanID = m_stream->_readInt();
					strinfo->juntuanName= m_stream->_readString();
					strinfo->m_level = m_stream->_readInt();
					strinfo->nNumber = m_stream->_readInt();
					candidateVec->push_back(strinfo);
				}


				myCountryData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
				CandidateVec *candidateVecL = (CandidateVec *)param;
				candidateVecL->clear();
				unsigned char size_L = m_stream->_readUChar();
				for (unsigned char i = 0; i < size_L; ++ i)
				{
					CRCore::ref_ptr<strToupiaoInfo> strinfo = new strToupiaoInfo;
					strinfo->nPlayerID = m_stream->_readInt();
					strinfo->playerName = m_stream->_readString();
					strinfo->nNumber = m_stream->_readInt();

					candidateVecL->push_back(strinfo);
				}

				myCountryData->getParam(WCHDATA_JXJSelectLeaderSet,param);
				SelectLeaderIdSet * selectLeaderSet = (SelectLeaderIdSet *)param;
				selectLeaderSet->clear();
				size = m_stream->_readUChar();
				for (unsigned char i = 0; i < size; ++ i)
				{
					selectLeaderSet->insert(m_stream->_readInt());
				}
				myCountryData->excHandle(MAKEINT64(WCH_LockData,0));
				canvas->doEvent(WCH_UI_UpdateData);
				canvas_1->doEvent(WCH_UI_UpdateData);
				canvas_2->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJQueryMyCountryInfoMethod
//
/////////////////////////////////////////
crJXJQueryMyCountryInfoMethod::crJXJQueryMyCountryInfoMethod()
{
}

crJXJQueryMyCountryInfoMethod::crJXJQueryMyCountryInfoMethod(const crJXJQueryMyCountryInfoMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryMyCountryInfoMethod::inputParam(int i, void *param)
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

void crJXJQueryMyCountryInfoMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryMyCountryInfoMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeInt(0);//1
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCountryInfo,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvQueryMyCountryInfoMethod
//
/////////////////////////////////////////
crJXJRecvQueryMyCountryInfoMethod::crJXJRecvQueryMyCountryInfoMethod():
	m_netType(GameClient_Game){}
crJXJRecvQueryMyCountryInfoMethod::crJXJRecvQueryMyCountryInfoMethod(const crJXJRecvQueryMyCountryInfoMethod& handle):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}
void crJXJRecvQueryMyCountryInfoMethod::inputParam(int i, void *param)
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

void crJXJRecvQueryMyCountryInfoMethod::addParam(int i, const std::string& str)
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	}
}

void crJXJRecvQueryMyCountryInfoMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int CandidateId = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_JXJShiliID, param);
			unsigned char shiliid = *(unsigned char *)param;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{

				bool m_elector = false;
				bool m_candidate = false;
				unsigned char m_vote = 0;
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				crJXJShili *shili;
				crData *shiliData;
				PeopleMap *peopleMap = NULL;
				PeopleMap::iterator peoItr;
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				serverData->getParam(WCHDATA_JXJShiliWei+shiliid-10,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				peopleMap = (PeopleMap *)param;
				peoItr = peopleMap->begin();

				serverData->getParam(WCHDATA_JXJLevelRankingMap,param);
				LevelRankingMap * lvRankingmap = (LevelRankingMap *)param;
				if (!(lvRankingmap->empty()))
				{
					LevelRankingMap::iterator pmItr_1 = lvRankingmap->begin();
					int count = 0;
					for (; pmItr_1 != lvRankingmap->end() && count < 100; pmItr_1++)
					{
						peoItr = peopleMap->find(pmItr_1->second->getPlayerID());
						if(peoItr != peopleMap->end())
						{
							if (playerid == pmItr_1->second->getPlayerID())
							{
								m_elector = true;
								m_vote += 1;
								break;
							}
							count++;
						}
					}
				}

				serverData->getParam(WCHDATA_JXJExpRankingMap,param);
				ExpRankingMap * expRankingmap = (ExpRankingMap *)param;
				if (!(expRankingmap->empty()))
				{
					ExpRankingMap::iterator pmItr_2 = expRankingmap->begin();
					int count_2 = 0;
					for (; pmItr_2 != expRankingmap->end() && count_2 < 100; pmItr_2++)
					{
						peoItr = peopleMap->find(pmItr_2->second->getPlayerID());
						if(peoItr != peopleMap->end())
						{
							if (playerid == pmItr_2->second->getPlayerID())
							{
								m_elector = true;
								m_vote += 1;
								break;
							}
							count_2++;
						}
					}
				}

				serverData->getParam(WCHDATA_JXJAccomplishRankingMap,param);
				AccomplishRankingMap * accomplishRankingmap = (AccomplishRankingMap *)param;
				if (!(accomplishRankingmap->empty()))
				{
					AccomplishRankingMap::iterator pmItr_3 = accomplishRankingmap->begin();
					int count_3 = 0;
					for (; pmItr_3 != accomplishRankingmap->end() && count_3 < 100; pmItr_3++)
					{
						peoItr = peopleMap->find(pmItr_3->second->getPlayerID());
						if(peoItr != peopleMap->end())
						{
							if (playerid == pmItr_3->second->getPlayerID())
							{
								m_elector = true;
								m_vote += 1;
								break;
							}
							count_3++;
						}
					}
				}
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				data->inputParam(WCHDATA_JXJVoteTimes, &m_vote);
				data->getParam(WCHDATA_JXJCountryContribute,param);
				int contribute = *( int *)param;
				data->getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if(shiliid >= c_startShiliID)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					unsigned char groupPos = JXJGroup_Normal;
					ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
					if (cmItr != consortiamap->end())
					{
						if (cmItr->second.valid())
						{
							crJXJConsortiaData * groupData = dynamic_cast<crJXJConsortiaData *>(cmItr->second->getDataClass());
							if (groupData)
							{
								groupPos = groupData->getGroupPos(playerid);
							}
						}
					}
					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					if(shili && shili->getDataClass())
					{
						crJXJShiliData * shilidata = dynamic_cast<crJXJShiliData *>(shili->getDataClass());
						shilidata->excHandle(MAKEINT64(WCH_LockData,1));
						shilidata->getParam(WCHDATA_JXJChengxiang,param);
						int kingplayerid1 = *(int*)param;
						shilidata->getParam(WCHDATA_JXJChengxiangName,param);
						std::string kingname1 = *(std::string *)param;
						shilidata->getParam(WCHDATA_JXJDasima,param);
						int kingplayerid2 = *(int*)param;
						shilidata->getParam(WCHDATA_JXJDasimaName,param);
						std::string kingname2 = *(std::string *)param;
						shilidata->getParam(WCHDATA_JXJDajiangjun,param);
						int kingplayerid3 = *(int*)param;
						shilidata->getParam(WCHDATA_JXJDajiangjunName,param);
						std::string kingname3 = *(std::string *)param;
						shilidata->getParam(WCHDATA_JXJCountryNotice,param);
						std::string notice = *(std::string *)param;
						shilidata->getParam(WCHDATA_JXJShiliWinCityDeq,param);
						ShiliCityChangeDeq * windeq = (ShiliCityChangeDeq *)param;
						shilidata->getParam(WCHDATA_JXJShiliLostCityDeq,param);
						ShiliCityChangeDeq *lostdeq = (ShiliCityChangeDeq *)param;
						CandidateVec *candidateVecT;
						CandidateVec::iterator cvItr;
						shilidata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
						candidateVecT = (CandidateVec*)param;
						if (CandidateId > 0 && candidateVecT)
						{//在候选人自荐窗口选择放弃 会将玩家自己从候选人列表删除
							for (cvItr = candidateVecT->begin(); cvItr != candidateVecT->end(); ++ cvItr)
							{
								if (cvItr->valid() && (*cvItr)->nPlayerID == CandidateId)
								{
									candidateVecT->erase(cvItr);
									break;
								}
							}
						}

						if (candidateVecT)
						{//判断玩家是否有候选人资格
							for (cvItr = candidateVecT->begin(); cvItr != candidateVecT->end(); ++ cvItr)
							{
								if (cvItr->valid() && (*cvItr)->nPlayerID == playerid)
								{
									m_candidate = true;
									break;
								}
							}
						}

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

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(4096);
						//由此处进入客户端，第一个写入值为1；
						stream->_writeUChar(1);
						//写入名字
						stream->_writeInt(kingplayerid1);
						stream->_writeString(kingname1);
						stream->_writeInt(kingplayerid2);
						stream->_writeString(kingname2);
						stream->_writeInt(kingplayerid3);
						stream->_writeString(kingname3);
						stream->_writeUChar(shilidata->getOfficialDes(playerid));
						stream->_writeUChar(groupPos);
						//写入公告
						stream->_writeString(notice);
						ShiliCityChangeDeq::iterator sitr;

						// 写入所得城池
						stream->_writeUChar(windeq->size());
						for (sitr = windeq->begin(); sitr != windeq->end(); ++sitr)
						{
							stream->_writeUShort(*sitr);
						}
						//写入丢失城池
						stream->_writeUChar(lostdeq->size());
						for (sitr = lostdeq->begin(); sitr != lostdeq->end(); ++sitr)
						{
							stream->_writeUShort(*sitr);
						}					
						//写入建筑
						int gongxian = 0;
						for (int ii = WCHDATA_JXJShenNongMiao; ii <= WCHDATA_JXJTaiMiao; ii++)
						{
							shilidata->getParam(ii,param);
							gongxian = *(int *)param;
							stream->_writeInt(gongxian);
						}
						shilidata->getParam(WCHDATA_JXJZhanLingCityCount,param);
						unsigned char citycount = *(unsigned char *)param;
						shilidata->getParam(WCHDATA_JXJCountryCapital,param);
						int capital = *(int *)param;
						shilidata->getParam(WCHDATA_JXJDestroyCountryid1,param);
						char decountryid1 = *(char *)param;
						shilidata->getParam(WCHDATA_JXJDestroyCountryid2,param);
						char decountryid2 = *(char *)param;
						shilidata->excHandle(MAKEINT64(WCH_LockData,0));
						//data->getParam(WCHDATA_JXJCountryContribute,param);
						stream->_writeInt(contribute);
						stream->_writeUChar(citycount);
						stream->_writeBool(m_elector);
						stream->_writeBool(m_candidate);
						stream->_writeUChar(electIcon);
						stream->_writeInt(capital);
						stream->_writeChar(decountryid1);
						stream->_writeChar(decountryid2);

						//ref_ptr<crStreamBuf> shiliStream = new crStreamBuf;
						//shilidata->excHandle(MAKEINT64(WCH_BuildPlayerStream,shiliStream.get()));
						//stream->_writeInt(shiliStream->getBufSize());
						//stream->write(shiliStream->getBuf(),shiliStream->getBufSize());

						if(playerData.valid())
						{
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyCountryInfo,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			std::string notice;
			unsigned char size = 0;
			int cityid = 0;
			int tempContribute ;
			crData *data = m_this->getDataClass();
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			ref_ptr<crCanvasNode> ElectResultCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Toupiaojieguo");
			if (mygroupcanvas.valid() && ElectResultCanvas.valid() && data)
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					unsigned char type = 0;
					type = m_stream->_readUChar();
					if (type == 1)
					{
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalVec,param);
						OfficalVec * officalVec = (OfficalVec *)param;
						officalVec->clear();
						int id;
						std::string name;
						for (unsigned char i = 0; i < 3; ++ i)
						{
							id = m_stream->_readInt();
							name = m_stream->_readString();
							officalVec->push_back(std::make_pair(id,name));
						}
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));

						unsigned char officalPos = m_stream->_readUChar();
						unsigned char groupPos = m_stream->_readUChar();

						canvasdata->inputParam(WCHDATA_JXJCountryCanvasOfficalPos,&officalPos);
						canvasdata->inputParam(WCHDATA_JXJCountryCanvasGroupPos,&groupPos);

						notice = m_stream->_readString();
						canvasdata->inputParam(WCHDATA_JXJMyCountryNotice,&notice);

						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJShiliWinCityDeq,param);
						ShiliCityChangeDeq * windeq = (ShiliCityChangeDeq *)param;
						windeq->clear();
						size = m_stream->_readUChar();
						for (int i = 0; i < size; i++)
						{
							cityid = m_stream->_readUShort();
							windeq->push_back(cityid);
						}
						canvasdata->getParam(WCHDATA_JXJShiliLostCityDeq,param);
						ShiliCityChangeDeq  *lostdeq = (ShiliCityChangeDeq *)param;

						lostdeq->clear();
						//丢失城池
						size = m_stream->_readUChar();
						for (int i = 0; i < size; i++)
						{
							cityid = m_stream->_readUShort();
							lostdeq->push_back(cityid);
						}
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
						//建筑
						int j = 0;
						for (int i = WCHDATA_JXJShenNongMiao; i <= WCHDATA_JXJTaiMiao && j<5; i++,j++)
						{
							tempContribute = m_stream->_readInt();
							canvasdata->inputParam(i,&tempContribute);
						}
						tempContribute = m_stream->_readInt();
						canvasdata->inputParam(WCHDATA_JXJCountryContribute,&tempContribute);
						unsigned char citycount = m_stream->_readUChar();
						canvasdata->inputParam(WCHDATA_JXJZhanLingCityCountCanvas,&citycount);

						bool elector = m_stream->_readBool();
						data->inputParam(WCHDATA_JXJElector, &elector);
						bool candidate = m_stream->_readBool();
						data->inputParam(WCHDATA_JXJCandidate, &candidate);
						unsigned char electIcon = m_stream->_readUChar();
						data->inputParam(WCHDATA_JXJElectIconTimeControl, &electIcon);

						int capital = m_stream->_readInt();
						canvasdata->inputParam(WCHDATA_JXJCanvasCapital,&capital);

						char decountryid1 = m_stream->_readChar();
						char decountryid2 = m_stream->_readChar();
						canvasdata->inputParam(WCHDATA_JXJDestroyCountryid1,&decountryid1);
						canvasdata->inputParam(WCHDATA_JXJDestroyCountryid2,&decountryid2);

						mygroupcanvas->doEvent(WCH_UI_UpdateData);
						ElectResultCanvas->doEvent(WCH_UI_UpdateData);
						ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
						if (renwucanvas.valid())
							renwucanvas->doEvent(WCH_UI_UpdateData);

						//int shiliSize = m_stream->_readInt();
						//ref_ptr<crStreamBuf> shiliStream = new crStreamBuf;
						//shiliStream->createBuf(shiliSize);
						//shiliStream->write(m_stream->getPtr(),shiliSize);
						//shiliStream->seekBegin();
						////crData *braindata = crBrain::getInstance()->getDataClass();
						////braindata->excHandle(MAKEINT64(WCH_LockData,1));

						//data->excHandle(MAKEINT64(WCH_LockData,1));
						//data->getParam(WCHDATA_JXJMyCountryData,param);
						//crJXJShili *shiliPtr = (crJXJShili *)param;
						//if (!shiliPtr)
						//{
						//	shiliPtr = new crJXJShili;
						//	shiliPtr->loadData();
						//}
						//crData *shiliData = shiliPtr->getDataClass();
						//shiliData->inputParam(WCHDATA_PlayerStream,shiliStream.get());
						//data->inputParam(WCHDATA_JXJMyCountryData,shiliPtr);
						//data->excHandle(MAKEINT64(WCH_LockData,0));
					}
					else
					{
						unsigned char electIcon = m_stream->_readUChar();
						data->inputParam(WCHDATA_JXJElectIconTimeControl, &electIcon);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBuildMyCountryMethod
//
/////////////////////////////////////////
crJXJUIBuildMyCountryMethod::crJXJUIBuildMyCountryMethod():
	m_index(0),
	m_font("simhei.ttf"),
	m_characterSize(16),
	m_goldColor(0.0f,1.0f,1.0f,1.0f),
	m_textColor(1.0f,1.0f,0.0f,1.0f)
{
}

crJXJUIBuildMyCountryMethod::crJXJUIBuildMyCountryMethod(const crJXJUIBuildMyCountryMethod& handle):
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

void crJXJUIBuildMyCountryMethod::inputParam(int i, void *param)
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

void crJXJUIBuildMyCountryMethod::addParam(int i, const std::string& str)
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

void crJXJUIBuildMyCountryMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			//std::string buildingname;
			//switch (m_index)
			//{
			//case BuildCountryType_TongQueTai:
			//	buildingname = "铜雀台";
			//	break;
			//case BuildCountryType_CaoYun:
			//	buildingname = "漕运";
			//	break;
			//case BuildCountryType_SiMaJian:
			//	buildingname = "司马监";
			//	break;
			//case BuildCountryType_ShangLinYuan:
			//	buildingname = "上林苑";
			//	break;
			//case BuildCountryType_TaiMiao:
			//	buildingname = "太庙";
			//	break;
			//default:
			//	break;
			//}
			crData *data = playerGameData->getDataClass();
			void *param;
			data ->getParam(WCHDATA_JXJCoppercash,param);
			int mycopper = *(int *)param;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int price = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJContriCountryOnceCopper,viplv).c_str()));
			if (mycopper >= price)
			{
				ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
				ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
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
				sprintf(tmpText,"%d铜钱\0",price);
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
				cfg_script.Add("Text","来进行建设吗？");
				cfg_script.Pop();

				cfg_script.Pop();

				if(information.valid())information->setHypertext(cfg_script);
				if(okbtn.valid())okbtn->setEnable(true);
				crData *dlgData = canvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				crFilterRenderManager::getInstance()->doModal(canvas.get());
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3032,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuildMyCountryMethod
//
/////////////////////////////////////////
crJXJRecvBuildMyCountryMethod::crJXJRecvBuildMyCountryMethod():
	m_netType(GameClient_Game){}
crJXJRecvBuildMyCountryMethod::crJXJRecvBuildMyCountryMethod(const crJXJRecvBuildMyCountryMethod& handle):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}
void crJXJRecvBuildMyCountryMethod::inputParam(int i, void *param)
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

void crJXJRecvBuildMyCountryMethod::addParam(int i, const std::string& str)
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	}
}

void crJXJRecvBuildMyCountryMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			ref_ptr<crTableIO> countrybuildtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBuildTab);
			crTableIO::StrVec record;
			crTableIO::DataVec records;
			int moneyneedindex = countrybuildtab->getTitleIndex("铜钱");
			int typeindex = countrybuildtab->getTitleIndex("type");
			//int addindex = countrybuildtab->getTitleIndex("加成");
			void *param;
			unsigned char success = 0;
			bool enoughmoney = false;
			//float factor = 0.0f;
			//int curgold = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crData *mainroledata = m_this->getMainRole()->getDataClass();
			unsigned char index = m_stream ->_readUChar();
			unsigned char iscustom = m_stream ->_readUChar();
			int coppercost = 0;
			
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && data && mainroledata)
			{
				mainroledata->getParam(WCHDATA_Level,param);
				unsigned char mylv = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				int countrybuildlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryBuildLv, viplv).c_str());
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if(mylv>=countrybuildlv)
				{
					/*data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;*/
					if (iscustom==1)
					{
						coppercost = m_stream->_readInt();
						coppercost = coppercost * 10000;
					}
					else
						coppercost = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJContriCountryOnceCopper,viplv).c_str()));

					float factor = (float)(atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJContriCountryCopperFactor,viplv).c_str()));
					data->excHandle(MAKEINT64(WCH_LockData,1));
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					data -> getParam(WCHDATA_JXJShiliID,param);
					unsigned char shiliid = *( unsigned char *)param;
					data -> getParam(WCHDATA_JXJCoppercash,param);
					int *mycopper = ( int *)param;
					data -> getParam(WCHDATA_JXJCountryContribute,param);
					int *mycountrycontribute = ( int *)param;
					if (*mycopper >= coppercost && coppercost>0)
					{
						enoughmoney = true;
					}
					else
					{
						success = 2;//铜钱不足
					}
					if(shiliid >= c_startShiliID && enoughmoney)
					{
						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if(shili && shili->getDataClass())
						{
							crData *shilidata = shili->getDataClass();
							int ii = 0;
							switch (index)
							{
							case BuildCountryType_TongQueTai:
								ii = WCHDATA_JXJShenNongMiao;
								break;
							case BuildCountryType_CaoYun:
								ii = WCHDATA_JXJSijintai;
								break;
							case BuildCountryType_SiMaJian:
								ii = WCHDATA_JXJSiMaJian;
								break;
							case BuildCountryType_ShangLinYuan:
								ii = WCHDATA_JXJShangLinYuan;
								break;
							case BuildCountryType_TaiMiao:
								ii = WCHDATA_JXJTaiMiao;
								break;
							default:
								break;
							}
							if (ii > 0)
							{
								shilidata->excHandle(MAKEINT64(WCH_LockData,1));
								shilidata->getParam(ii,param);
								int *gongxian = (int *)param;
								shilidata->getParam(WCHDATA_JXJCountryCapital,param);
								int *guojiazijin = (int *)param;
								float gain = 0.0f;
								countrybuildtab->queryRecords(typeindex,crArgumentParser::appItoa(index),records);
								if(records.size())
								{
									if (atoi((*records.rbegin())[moneyneedindex].c_str()) > *gongxian)
									{
										data->getParam(WCHDATA_JXJBuildCountryTimes,param);
										char* buildtimes = (char *)param;
										if((*buildtimes)==0)
										{
											success = 5;//国家建设次数上限
										}
										else
										{
											if((*buildtimes) > 0)
											{
												(*buildtimes)--;
											}
											int cost = coppercost;
											m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&cost,NULL));
											//*mycopper -= coppercost;
											gain = (float)coppercost * factor;
											INTLIMITADD(*gongxian,coppercost,INT_MAX);
											INTLIMITADD(*mycountrycontribute,gain,INT_MAX);
											INTLIMITADD(*guojiazijin,coppercost,INT_MAX);
											//*gongxian += (float)coppercost * factor;
											success = 1;
											stream->createBuf(19);	
											stream->_writeUChar(success);	
											stream->_writeUChar(index);
											stream->_writeInt(*gongxian);
											stream->_writeInt(*mycopper);
											stream->_writeInt(*mycountrycontribute);
											stream->_writeInt(*guojiazijin);
											stream->_writeChar(*buildtimes);
										}
									}
									else
									{//已经升级到顶级了
										success = 3;
									}
								}
								bool modify = true;
								shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
								shilidata->excHandle(MAKEINT64(WCH_LockData,0));
							}
						}

					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}
				else
				{
					success = 4;
				}
				if(success != 1)
				{
					stream->createBuf(2);
					stream->_writeUChar(success);
					stream->_writeUChar(index);
				}
				if(playerData.valid())
				{
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuildMyCountry,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				if (success == 1)
				{
					m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_BuildCountry, 1));
				}
				///游戏日志
				std::string logdata = "国家建设"+crArgumentParser::appVectoa(crVector2i(success,index),'|');
				GameLogData gamelog(Log_BuildMyCountry,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			unsigned char success = m_stream->_readUChar();
			unsigned char index = m_stream->_readUChar();
			int ii = 0;
			int tipindex = 0;
			switch (index)
			{
			case BuildCountryType_TongQueTai:
				ii = WCHDATA_JXJShenNongMiao;
				break;
			case BuildCountryType_CaoYun:
				ii = WCHDATA_JXJSijintai;
				break;
			case BuildCountryType_SiMaJian:
				ii = WCHDATA_JXJSiMaJian;
				break;
			case BuildCountryType_ShangLinYuan:
				ii = WCHDATA_JXJShangLinYuan;
				break;
			case BuildCountryType_TaiMiao:
				ii = WCHDATA_JXJTaiMiao;
				break;
			default:
				break;
			}
			if (success == 1)
			{
				tipindex = 2031;
				int gongxian = m_stream->_readInt();
				int copper = m_stream->_readInt();
				int countrycontri = m_stream->_readInt();
				int guojiazijin = m_stream->_readInt();
				char buildtimes = m_stream->_readChar();
				if (mygroupcanvas.valid() && ii > 0)
				{
					void *param;
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata)
					{
						canvasdata->inputParam(ii,&gongxian);
						canvasdata->getParam(WCHDATA_JXJCanvasCapital,param);
						int extra = guojiazijin-(*(int *)param);
						if(extra!=0)
						{
							canvasdata->inputParam(WCHDATA_JXJCanvasCapital,&guojiazijin);
						}
					}
					crData *data = m_this->getDataClass();
					data->getParam(WCHDATA_JXJCountryContribute,param);
					int extra = countrycontri-(*(int *)param);
					if(extra!=0)
					{
						data->inputParam(WCHDATA_JXJCountryContribute,&countrycontri);
						crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Contribute,extra));
						m_this->doEvent(WCH_JXJGetAwardsPlaySound);
					}
					data->inputParam(WCHDATA_JXJBuildCountryTimes,&buildtimes);
				}
				//crData *braindata = crBrain::getInstance()->getDataClass();
				//braindata->getParam(WCHDATA_JXJMyCountryData,param);
				//crJXJShili *shili = (crJXJShili *)param;
				//if (shili)
				//{
				//	crData *shilidata = shili->getDataClass();
				//	if (shilidata)
				//	{
				//		shilidata->inputParam(ii,&gongxian);
				//	}
				//}
				//crData *data = m_this->getDataClass();
				//data->inputParam(WCHDATA_JXJCoppercash,&copper);
				//data->inputParam(WCHDATA_JXJCountryContribute,&countrycontri);
			}
			else if(success == 2)
			{
				tipindex = 2032;
			}
			else if (success == 3)
			{
				tipindex = 2033;
				ref_ptr<crTableIO> countrybuildtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBuildTab);
				crTableIO::StrVec record;
				crTableIO::DataVec records;
				int moneyneedindex = countrybuildtab->getTitleIndex("铜钱");
				int typeindex = countrybuildtab->getTitleIndex("type");
				int gongxian = 0;
				countrybuildtab->queryRecords(typeindex,crArgumentParser::appItoa(index),records);
				if(records.size())
				{
					if (atoi(records.back()[moneyneedindex].c_str()) > 0)
					{
						gongxian = atoi(records.back()[moneyneedindex].c_str());
					}
				}
				if (mygroupcanvas.valid() && ii > 0 && gongxian > 0)
				{
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata)
					{
						canvasdata->inputParam(ii,&gongxian);
					}
				}
			}
			else if (success == 4)
			{
				tipindex = 5001;
			}
			else if (success == 5)
			{
				tipindex = 1121;
			}
			if (success != 1)
			{
				if (tipindex !=5001)
				{
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipindex,NULL));
				}
				else
				{
					crData *data = m_this->getDataClass();
					if (data)
					{
						void * param;
						data->getParam(WCHDATA_JXJVipLv, param);
						unsigned char viplv = *(unsigned char *)param;
						std::string param1 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryBuildLv,viplv);
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&tipindex);
						int mode = 1;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					}
					
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJMyCounrtyCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMyCounrtyCanvasUpdateMethod::crJXJMyCounrtyCanvasUpdateMethod()
{
}

crJXJMyCounrtyCanvasUpdateMethod::crJXJMyCounrtyCanvasUpdateMethod(const crJXJMyCounrtyCanvasUpdateMethod& handle):
	crMethod(handle),
	m_pageradio(handle.m_pageradio),
	m_pagesw(handle.m_pagesw)
{
}

void crJXJMyCounrtyCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyCounrtyCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pageradio = str;
		break;
	case 1:
		m_pagesw = str;
		break;
	default:
		break;
	}
}

void crJXJMyCounrtyCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		ref_ptr<crRadioGroupWidgetNode> pageradio = dynamic_cast<crRadioGroupWidgetNode *>(parentCanvas->getWidget(m_pageradio));
		crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_pagesw));
		int select = 0;
		if (pageradio.valid())
		{
			select = pageradio->getSelect();
		}
		if (pagesw)
		{
			pagesw->setActiveSwitchSet(select);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvOnCountryBuildingPageMethod
//
/////////////////////////////////////////
crJXJRecvOnCountryBuildingPageMethod::crJXJRecvOnCountryBuildingPageMethod():
	m_netType(GameClient_Game){}
crJXJRecvOnCountryBuildingPageMethod::crJXJRecvOnCountryBuildingPageMethod(const crJXJRecvOnCountryBuildingPageMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvOnCountryBuildingPageMethod::inputParam(int i, void *param)
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

void crJXJRecvOnCountryBuildingPageMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvOnCountryBuildingPageMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			//unsigned char success = 0;
			std::string noticestr = m_stream->_readString();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJOnCountryQueryMap,param);
				OnCountryQueryMap *oncountryquerymap = (OnCountryQueryMap *)param;
				OnCountryQueryMap::iterator itr;
				OnCountryQueryDeq oncountryquerydeq;
				OnCountryQueryDeq::iterator itr2;
				itr = oncountryquerymap->find(shiliid);
				if (itr != oncountryquerymap->end())
				{
					oncountryquerydeq = itr->second;
					for (itr2 = oncountryquerydeq.begin();
						itr2 != oncountryquerydeq.end();
						++itr2)
					{
						if ((*itr2) == playerid)
						{
							break;
						}
					}
					if (itr2 != oncountryquerydeq.end())
					{
						oncountryquerydeq.push_back(playerid);
					}
				}
				else
				{
					oncountryquerydeq.push_back(playerid);
					oncountryquerymap->insert(std::make_pair(shiliid,oncountryquerydeq));
				}
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJQueryCountryBuildingDisposeMethod
//
/////////////////////////////////////////
crJXJQueryCountryBuildingDisposeMethod::crJXJQueryCountryBuildingDisposeMethod():
	m_this(NULL),
	m_dt(NULL),
	m_validterm(60){}
crJXJQueryCountryBuildingDisposeMethod::crJXJQueryCountryBuildingDisposeMethod(const crJXJQueryCountryBuildingDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_dt(NULL),
	m_validterm(handle.m_validterm)
{
}
void crJXJQueryCountryBuildingDisposeMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			if(lparam)
				m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJQueryCountryBuildingDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_validterm = atof(str.c_str());
		break;
	}
}

void crJXJQueryCountryBuildingDisposeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJOnCountryQueryMap,param);
		OnCountryQueryMap *oncountryquerymap = (OnCountryQueryMap *)param;
		//ref_ptr<crJXJShili> shili[3] = {NULL};
		//crData *shilidata[3] = {NULL};
		//int j = 0;
		//for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu,j<3; i++,j++)
		//{
		//	data->getParam(i,param);
		//	shili[j] = (crJXJShili *)param;
		//	if (shili[j].valid())
		//	{
		//		shilidata[j] = shili[j]->getDataClass();	
		//	}
		//}
		if(!oncountryquerymap->empty())
		{
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData;
			OnCountryQueryDeq oncountryquerydeq;
			int playerid = 0;
			ref_ptr<crStreamBuf> stream;
			crPlayerDataEventPacket packet;
			int gongxian = 0;
			crJXJShili *shili;
			crData *shilidata;
			for (OnCountryQueryMap::iterator mitr = oncountryquerymap->begin();
				mitr!=oncountryquerymap->end();
				++mitr)
			{
				if(mitr->first>=c_startShiliID)
				{
					data->getParam(WCHDATA_JXJShiliWei+mitr->first-c_startShiliID,param);
					shili = (crJXJShili *)param;
					if(shili && shili->getDataClass())
					{
						shilidata = shili->getDataClass();
						stream = new crStreamBuf;
						stream->createBuf(20);
						for (int ii = WCHDATA_JXJShenNongMiao; ii <= WCHDATA_JXJTaiMiao; ii++)
						{
							shilidata->getParam(ii,param);
							gongxian = *(int *)param;
							stream->_writeInt(gongxian);
						}
						oncountryquerydeq = mitr->second;
						for( OnCountryQueryDeq::iterator pitr = mitr->second.begin();
							pitr != mitr->second.end();
							++pitr )
						{
							playerid = *pitr;
							playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
							if(playerData.valid())
							{
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvCountryBuilding,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}


/************************************************************************/
/*    crJXJCountryCantonFenFengMethod                                                                  */
/************************************************************************/

JXJ::crJXJCountryCantonFenFengMethod::crJXJCountryCantonFenFengMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryCantonFenFengMethod::crJXJCountryCantonFenFengMethod( const crJXJCountryCantonFenFengMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_tabelWidget(handle.m_tabelWidget),
	m_idx(handle.m_idx),
	m_op(handle.m_op)
{
}

void JXJ::crJXJCountryCantonFenFengMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCountryCantonFenFengMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_tabelWidget = str;
		break;
	case 2:
		m_idx = (unsigned char )atoi(str.c_str());
		break;
	case 3:
		m_op = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJCountryCantonFenFengMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	do 
	{
		if (!m_this || !playerGameData || !netConductor) break;
		ref_ptr<crCanvasNode> myCountryCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!myCountryCanvas || myCountryCanvas->getDataClass() == NULL) break;
		crData * myCountryData = myCountryCanvas->getDataClass();
		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		if (!parentCanvas) break;
		ref_ptr<crTableWidgetNode>  groupList  = dynamic_cast<crTableWidgetNode *>(parentCanvas->getWidget(m_tabelWidget));
		if (!groupList) break;
		void *param;
		myCountryData->getParam(WCHDATA_JXJCountryCanvasSelectedCantonId,param);
		int cantonId = *(int *)param;
		groupList->selectRow(m_idx);
		int groupId = groupList->getSelectData();
		
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeInt(cantonId);
		stream->_writeInt(groupId);
		stream->_writeUChar(m_op);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryCantonAllocate,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	} while (0);
}

/************************************************************************/
/*    crJXJCountrySetSelectedCantonIdMethod                                                                  */
/************************************************************************/

JXJ::crJXJCountrySetSelectedCantonIdMethod::crJXJCountrySetSelectedCantonIdMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountrySetSelectedCantonIdMethod::crJXJCountrySetSelectedCantonIdMethod( const crJXJCountrySetSelectedCantonIdMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_groupRadio(handle.m_groupRadio)
{
}

void JXJ::crJXJCountrySetSelectedCantonIdMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCountrySetSelectedCantonIdMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCountrySetSelectedCantonIdMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		ref_ptr<crCanvasNode> myCountryCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!myCountryCanvas) break;
		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(myCountryCanvas->getWidget(m_groupRadio));
		if (!groupradio) break;
		int selectedId = groupradio->getSelect();
		crData * myCountryData = myCountryCanvas->getDataClass();
		if (!myCountryData) break;

		myCountryData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		myCountryData->getParam(WCHDATA_JXJCountryCanvasZhoujunMap,param);
		ZhoujunMap * zhoujunMap = (ZhoujunMap *) param;
		ZhoujunMap::iterator zmItr = zhoujunMap->begin();
		unsigned char i;
		for (i = 0; i != selectedId && zmItr != zhoujunMap->end(); ++ zmItr, ++ i);
		if (i == selectedId) selectedId = zmItr->first;
		myCountryData->excHandle(MAKEINT64(WCH_LockData,0));
		myCountryData->inputParam(WCHDATA_JXJCountryCanvasSelectedCantonId,&selectedId);
	} while (0);
}

/************************************************************************/
/*    crJXJQueryMyCounrtyCantonFenFengGroupListMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryMyCounrtyCantonFenFengGroupListMethod::crJXJQueryMyCounrtyCantonFenFengGroupListMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryMyCounrtyCantonFenFengGroupListMethod::crJXJQueryMyCounrtyCantonFenFengGroupListMethod( const crJXJQueryMyCounrtyCantonFenFengGroupListMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJQueryMyCounrtyCantonFenFengGroupListMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryMyCounrtyCantonFenFengGroupListMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJQueryMyCounrtyCantonFenFengGroupListMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crCanvasNode> myCountryCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		do 
		{
			if (!netConductor || !myCountryCanvas ) break;
			crData * canvasData = myCountryCanvas->getDataClass();
			if (!canvasData) break;
			void *param;
			canvasData->getParam(WCHDATA_JXJCountryCanvasFenFengCurPage	,param);
			int curPage = *(int *)param;

			crPlayerDataEventPacket packet;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(3);
			stream->_writeInt((curPage - 1 ) * 9);
			stream->_writeUChar(9);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCounrtyCantonFenFengGroupList,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		} while (0);
	}
}

/************************************************************************/
/*    crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod( const crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas),
	m_fenFengCanvas(handle.m_fenFengCanvas)
{
}

void JXJ::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	case 1:
	 	m_fenFengCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				int start = m_stream->_readInt();
				unsigned char querycount = m_stream->_readUChar();
				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				//data->getParam(WCHDATA_JXJConsortiaID,param);
				//int mygroupid = *(int *)param;
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				serverData->getParam(WCHDATA_JXJConsortiaLvRank,param);
				ConsortialvRank * consortiaLvRank = (ConsortialvRank *)param;
				ConsortialvRank ::RankSet & lvRankSet = consortiaLvRank->getRankSet();

				serverData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator cMitr;

				int count = CRCore::minimum((int)querycount,(int)(lvRankSet.size()-start));
				if(count>0)
				{
					//short myranking = -1;
					int recordcount = 0;
					int i = 0;
					unsigned short totalrecord = 0;
					//unsigned char groupshiliid = 0;
					ConsortiaLvRankItem *rankItem;
					ConsortialvRank::RankSet::iterator itr = lvRankSet.begin(); 

					for(;itr != lvRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem *)( (*itr).get() );

						if (rankItem->m_shili == shiliid)
						{
							cMitr = consortiamap->find(rankItem->m_id);
							if (cMitr != consortiamap->end())
							{
								if(i >= start && i < start + count) recordcount ++;
								++i;
								++totalrecord;
							}
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					crPlayerDataEventPacket packet;
					stream->createBuf(128);
					stream->_writeUChar((unsigned char)recordcount);
					crData *groupData = NULL;
					itr = lvRankSet.begin(); 
					for ( i = 0;i < start + count && itr != lvRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem * )(itr->get());
						if (rankItem == NULL) continue;
						if (rankItem->m_shili == shiliid)
						{
							cMitr = consortiamap->find(rankItem->m_id);
							if (cMitr != consortiamap->end())
							{
								if(i>=start)
								{
									stream->_writeInt(rankItem->m_id);
									stream->_writeString(rankItem->m_groupName);//20
									stream->_writeString(rankItem->m_leaderName);//20
									stream->_writeUChar(rankItem->m_lv);//1
									stream->_writeUShort(rankItem->m_memberCount);//2
									//stream->_writeInt(rankItem->m_troops);//4
									stream->_writeInt(rankItem->m_exploit);//4

									groupData = cMitr->second->getDataClass();
									if (groupData)
									{
										groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
										FenFengedCantonMap * cantonMap = (FenFengedCantonMap *) param;
										stream->_writeUChar(cantonMap->size());
										FenFengedCantonMap::iterator ffcmItr = cantonMap->begin();
										for (; ffcmItr != cantonMap->end(); ++ ffcmItr)
										{
											stream->_writeInt(ffcmItr->first);
										}
									}
									else 
									{
										stream->_writeUChar(0);
									}
								}
								++i;
							}
						}
					}
					stream->_writeUShort(totalrecord);

					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyCounrtyCantonFenFengGroupList,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas );
			ref_ptr<crCanvasNode> fenFengcanvas = crFilterRenderManager::getInstance()->findCanvas(m_fenFengCanvas);
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJCountryCanvasAllocateGroupVec,param);
			CountryCanvasAllocateGroupVec * groupVec = (CountryCanvasAllocateGroupVec *)param;
			groupVec->clear();
			int count = (int)(m_stream->_readUChar());
			CRCore::ref_ptr<CountryCanvasFenFengListItem > item;
			for (int i = 0; i<count; i++)
			{
				item = new CountryCanvasFenFengListItem (m_stream->_readInt());
				item->m_groupName = m_stream->_readString();
				item->m_leaderName = m_stream->_readString();
				item->m_lv = m_stream->_readUChar();
				item->m_memberCount = m_stream->_readUShort();
				item->m_exploit = m_stream->_readInt();
				unsigned char size = m_stream->_readUChar();
				for (unsigned j = 0; j < size; ++ j)
					item->m_fenFengedCantonSet.insert(m_stream->_readInt());

				groupVec->push_back(item);
			}
			unsigned short mapsize = m_stream->_readUShort();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			int totalPage = ceil((float)mapsize/(float)9);
			if(totalPage == 0) totalPage = 1;
			canvasdata->inputParam(WCHDATA_JXJCountryCanvasFenFengTotalPage	,&totalPage);
			if (fenFengcanvas.valid())
			{
				fenFengcanvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvCountryCantonFenFengMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvCountryCantonAllocateMethod::crJXJRecvCountryCantonAllocateMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvCountryCantonAllocateMethod::crJXJRecvCountryCantonAllocateMethod( const crJXJRecvCountryCantonAllocateMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas),
	m_cantonCanvas(handle.m_cantonCanvas)
{
}

void JXJ::crJXJRecvCountryCantonAllocateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvCountryCantonAllocateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	case 1:
		m_cantonCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvCountryCantonAllocateMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
//				cantionid,groupid,op
				int cantonId = m_stream->_readInt();
				int groupId = m_stream->_readInt();
				unsigned char op = m_stream->_readUChar();

				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				unsigned char maxNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCantonChengChiAllocateMaxNum,viplv).c_str());

				unsigned char retVal = JXJCountryAllocateCantonRetVal_failed_unkwon;
				
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(16);

				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					if (!cantonId) break;
					serverData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator cMitr = consortiamap->find(groupId);
					if (cMitr == consortiamap->end() || !(cMitr->second.valid()))
					{
						break;
					}
					ref_ptr<crData> groupData = cMitr->second->getDataClass();
					groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
					FenFengedCantonMap  * fenFengedCantonMap = (FenFengedCantonMap *)param;

					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					switch (op)
					{
					case JXJCountryCanvasAllocateCantonOpType_cancel:
						{
							FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->find(cantonId);
							if (ffcmItr != fenFengedCantonMap->end())
							{
								//去除分封出去的的城池
								groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
								FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
								fenFengedChengChiMap->clear();

								fenFengedCantonMap->erase(ffcmItr);
								retVal = JXJCountryAllocateCantonRetVal_suc;
								if(shili && shili->getDataClass())
								{
									crData * shiliData = shili->getDataClass();
									shiliData->excHandle(MAKEINT64(WCH_LockData,1));
									shiliData->getParam(WCHDATA_JXJShiliCantonMap,param);
									CantonMap * cantonMap = (CantonMap *)param;
									CantonMap::iterator cmItr = cantonMap->find(cantonId);
									if (cmItr != cantonMap->end())
									{
										if (cmItr->second.valid() && cmItr->second->getConsortiaId() == groupId)
										{
											cmItr->second->setConsortiaId(0);
										}
									}
									bool modify = true;
									groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
									shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
									shiliData->excHandle(MAKEINT64(WCH_LockData,0));
								}
							}
						}
						break;
					case JXJCountryCanvasAllocateCantonOpType_doFenFeng:
						{
							FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->find(cantonId);
							if (ffcmItr != fenFengedCantonMap->end())
							{
								retVal = JXJCountryAllocateCantonRetVal_failed_hasOwn;
							}
							else
							{
								if (fenFengedCantonMap->size() >= maxNum)
								{
									retVal = JXJCountryAllocateCantonRetVal_failed_full;
								}
								else
								{
									fenFengedCantonMap->insert(FenFengedCantonMap::value_type(cantonId,1000));
									retVal = JXJCountryAllocateCantonRetVal_suc;

									if(shili && shili->getDataClass())
									{
										crData * shiliData = shili->getDataClass();
										shiliData->excHandle(MAKEINT64(WCH_LockData,1));
										shiliData->getParam(WCHDATA_JXJShiliCantonMap,param);
										CantonMap * cantonMap = (CantonMap *)param;
										CantonMap::iterator cmItr = cantonMap->find(cantonId);
										if (cmItr != cantonMap->end())
										{
											if (cmItr->second.valid() && cmItr->second->getConsortiaId() != groupId)
											{
												cmItr->second->setConsortiaId(groupId);
											}
										}
										bool modify = true;
										groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
										shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
										shiliData->excHandle(MAKEINT64(WCH_LockData,0));

										std::string country[] = {"魏国","蜀国","吴国"};

										ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
										ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
										crTableIO::StrVec textrecord;
										crTableIO::StrVec record;
										if(textTab.valid() && cantontab.valid() 
											&& cantontab->queryOneRecord(0,crArgumentParser::appItoa(cantonId),record)>=0)
										{
											char szNotice[256];
											int textid = 4092;
											std::string groupName;
											cMitr->second->getName(groupName);
											if (textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
											{
												sprintf_s(szNotice,textrecord[1].c_str(),
													country[shiliid - c_startShiliID].c_str(),
													m_this->getCharacterName().c_str(),
													record[1].c_str(),
													groupName.c_str());
												CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
													szNotice,shiliid,0);
												crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
											}

											textid = 4093;
											if (textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
											{
												std::string leaderName;
												cMitr->second->getLeaderName(0,leaderName);
												sprintf_s(szNotice,textrecord[1].c_str(),
													groupName.c_str(),
													record[1].c_str(),
													leaderName.c_str()
													);
												CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
													szNotice,shiliid,shiliid);
												crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
											}
										}
									}
								}
							}
						}
						break;
					default:
						break;
					}
					stream->_writeUChar(retVal);
					if (retVal == JXJCountryAllocateCantonRetVal_suc)
					{
						stream->_writeInt(groupId);
						stream->_writeUChar(fenFengedCantonMap->size());
						FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->begin();
						for (;ffcmItr != fenFengedCantonMap->end(); ++ ffcmItr)
						{
							stream->_writeInt(ffcmItr->first);
						}
					}
				} while (0);
				
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCountryCantonAllocate,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			if (m_stream->eof()) return;
			
			unsigned char retVal = m_stream->_readUChar();
			void *param;
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas );
			ref_ptr<crCanvasNode> cantonCanvas = crFilterRenderManager::getInstance()->findCanvas(m_cantonCanvas);
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

			if (retVal == JXJCountryAllocateCantonRetVal_suc)
			{
				int groupId = m_stream->_readInt();
				canvasdata->getParam(WCHDATA_JXJCountryCanvasAllocateGroupVec,param);
				CountryCanvasAllocateGroupVec * groupVec = (CountryCanvasAllocateGroupVec *)param;
				CountryCanvasAllocateGroupVec::iterator ccagItr = groupVec->begin();
				for (; ccagItr != groupVec->end(); ++ ccagItr)
				{
					if ((*ccagItr).valid() && (*ccagItr)->m_id == groupId)
					{
						(*ccagItr)->m_fenFengedCantonSet.clear();
						unsigned char size = m_stream->_readUChar();
						for (unsigned char i = 0; i < size; ++ i)
						{
							(*ccagItr)->m_fenFengedCantonSet.insert(m_stream->_readInt());
						}
					}
				}
				cantonCanvas->doEvent(WCH_UI_UpdateData);
				m_this->doEvent(WCH_JXJQueryMyCounrtyCantonChengChiINfo);	
			}
			else 
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4008,NULL));
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/************************************************************************/
/*    crJXJRecvSelectCountryLeaderMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvSelectCountryLeaderMethod::crJXJRecvSelectCountryLeaderMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvSelectCountryLeaderMethod::crJXJRecvSelectCountryLeaderMethod( const crJXJRecvSelectCountryLeaderMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJRecvSelectCountryLeaderMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvSelectCountryLeaderMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvSelectCountryLeaderMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			int CandidateId = 0;
			typedef std::set<int> SelectedIdSet;
			SelectedIdSet idSet;
			unsigned char size = m_stream->_readUChar();
			for (unsigned char i = 0; i < size; ++ i)
			{
				idSet.insert(m_stream->_readInt());
			}
			if (size == 0)
			{
				CandidateId = m_stream->_readInt();
			}
			void* param;
			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (data && playerData.valid() && gameServer)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));

				bool recommendSet = false;
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char vipLevel = *( unsigned char*)param;
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *( unsigned char *)param;
				data -> getParam(WCHDATA_JXJCountryContribute,param);
				int *mycountrycontribute = ( int *)param;
				unsigned char myvotetimes = 0;
				data->getParam(WCHDATA_JXJVoteTimes, param);
				myvotetimes = *(unsigned char *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					unsigned char retVal = JXJCountryVoteRet_failed_unkwon;

					//gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					//ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					//ConsortiaMap::iterator cMitr = consortiamap->find(mygroupid);
					do 
					{
						//if (cMitr == consortiamap->end()
						//	|| (cMitr->second.valid() && !cMitr->second->checkPermission(playerid,JXJCountry_Vote)))
						//{
						//	retVal = JXJCountryVoteRet_failed_noPermission;
						//	break;
						//}
						//if (!shili || shili->getDataClass() == NULL) break;
						crData *shilidata = shili->getDataClass();
						CRCore::ScopedLock<crData> lock(*shilidata);
						data->getParam(WCHDATA_JXJCountryRecommendSet,param);
						recommendSet = *(bool *)param;
						shilidata->getParam(WCHDATA_JXJCountryVotersSet,param);
						VotersSet * votersSet = (VotersSet *)param;
						VotersSet::iterator vsItr = votersSet->find(playerid);
						//if (vsItr != votersSet->end())
						//{	
						//	retVal = JXJCountryVoteRet_failed_hadVoted;
						//	break;
						//}
						data->getParam(WCHDATA_JXJSelectLeaderSet,param);
						SelectLeaderIdSet *selectLeaderIdSet = (SelectLeaderIdSet *)param;
						shilidata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
						CandidateVec*candidateVec = (CandidateVec*)param;
						CandidateVec::iterator tdItr;
						SelectedIdSet::iterator sivItr = idSet.begin();
						for (;sivItr != idSet.end(); ++ sivItr)
						{
							for (tdItr = candidateVec->begin(); tdItr != candidateVec->end(); ++ tdItr)
							{
								if (tdItr->valid())
								{
									if (*sivItr == (*tdItr)->nPlayerID)
									{
										(*tdItr)->nNumber += myvotetimes;
										retVal = JXJCountryVoteRet_suc;
										selectLeaderIdSet->insert(*sivItr);
										break;
									}
								}
							}
						}
						if (CandidateId > 0)
						{
							for (tdItr = candidateVec->begin(); tdItr != candidateVec->end(); ++ tdItr)
							{
								if (tdItr->valid())
								{
									if (CandidateId == (*tdItr)->nPlayerID)
									{
										recommendSet = true;
										data->inputParam(WCHDATA_JXJCountryRecommendSet, &recommendSet);
										(*tdItr)->nNumber += 1;
										break;
									}
								}
							}
						}
						if (retVal == JXJCountryVoteRet_suc)
						{
							votersSet->insert(playerid);
						}
						bool modify = true;
						shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
					} while (0);
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

					std::string logdata = "玩家"+crArgumentParser::appItoa(playerid)+"国家选举投票：";
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeBool(recommendSet);
					stream->_writeUChar(retVal);
					if (retVal == JXJCountryVoteRet_suc)
					{
						stream->_writeUChar(idSet.size());
						SelectedIdSet::iterator sivItr = idSet.begin();
						for (;sivItr != idSet.end(); ++ sivItr)
						{
							stream->_writeInt(*sivItr);
							logdata+=crArgumentParser::appItoa(*sivItr);
						}
						int gain = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJVoteBonus,vipLevel).c_str());
						INTLIMITADD(*mycountrycontribute,gain,INT_MAX);
						stream->_writeInt(*mycountrycontribute);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSelectCountryLeader,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

					///游戏日志
					GameLogData gamelog(Log_SelectCountryLeader, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			bool recommendSet = false;
			crData *data = m_this->getDataClass();
			recommendSet = m_stream->_readBool();
			if (recommendSet)
			{
				data->inputParam(WCHDATA_JXJCountryRecommendSet, &recommendSet);
			}
			unsigned char retVal = m_stream->_readUChar();
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas );
			ref_ptr<crCanvasNode> canvas_1 = crFilterRenderManager::getInstance()->findCanvas("UI_Country3_Guanyuanxuanju" );
			if (canvas.valid() && canvas->getDataClass() && canvas_1.valid())
			{
				int msgid = 0;
				switch (retVal)
				{
				case JXJCountryVoteRet_suc:
					{
						bool hadVoted = true;
						crData *canvasdata = canvas->getDataClass();
						canvasdata->inputParam(WCHDATA_JXJCountryCanvasHaveVoted,&hadVoted);
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJSelectLeaderSet,param);
						SelectLeaderIdSet *selectLeaderSet = (SelectLeaderIdSet *)param;
						selectLeaderSet->clear();
						unsigned char size = m_stream->_readUChar();
						for (unsigned char i = 0; i < size; ++ i)
						{
							selectLeaderSet->insert(m_stream->_readInt());
						}
						int countrycontri = m_stream->_readInt();
						data->getParam(WCHDATA_JXJCountryContribute,param);
						int extra = countrycontri-(*(int *)param);
						if(extra!=0)
						{
							data->inputParam(WCHDATA_JXJCountryContribute,&countrycontri);
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Contribute,extra));
							m_this->doEvent(WCH_JXJGetAwardsPlaySound);
						}
						msgid = 3076;
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
						canvas->doEvent(WCH_UI_UpdateData);
						canvas_1->doEvent(WCH_UI_UpdateData);
					}
					break;
				case JXJCountryVoteRet_failed_hadVoted:
					msgid = 4066;
					break;

				case JXJCountryVoteRet_failed_noPermission:
					msgid = 4067;
					break;
				default:
					break;
				};
				if (msgid)
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
		}
	}
}

/************************************************************************/
/*    crJXJUICheckBoxUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUICheckBoxUpdateMethod::crJXJUICheckBoxUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUICheckBoxUpdateMethod::crJXJUICheckBoxUpdateMethod( const crJXJUICheckBoxUpdateMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_Recommend(handle.m_Recommend),
	m_Recommend_sw(handle.m_Recommend_sw)
{
	for (int i = 0;i<15 ;i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
		m_checkbox_sw[i] = handle.m_checkbox_sw[i];
	}
}

void JXJ::crJXJUICheckBoxUpdateMethod::operator()( crHandle &handle )
{
	if(m_this)
	{
		//ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode> canvas = m_this;
		//int iCheckedNumber = 0;
		if (canvas.valid() && canvas->getDataClass())
		{
			crData * canvasData = canvas->getDataClass();
			void *param;
			canvasData->getParam(WCHDATA_JXJCountryCanvasHaveVoted,param);
			bool voted = *(bool *)param;
			ref_ptr<crButtonWidgetNode>  recommend = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_Recommend));
			ref_ptr<crCheckBoxWidgetNode> foldbtn[15] = {NULL};
			crMultiSwitch *foldbtn_sw[15] = {NULL};
			crMultiSwitch *recommend_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_Recommend_sw));
			PermissionCheck permissionCheck;
			canvasData->getParam(WCHDATA_JXJCountryCanvasGroupPos,param);
			unsigned char groupPos = *(unsigned char *)param;
			if (voted || !permissionCheck(groupPos,JXJCountry_Vote))
			{
				if (recommend_sw) recommend_sw->setActiveSwitchSet(3);
				if (recommend.valid()) recommend->setEnable(false);
				for (int i = 0; i<15;i++)
				{
					foldbtn[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
					foldbtn_sw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_checkbox_sw[i]));
					if (foldbtn[i].valid()) foldbtn[i]->setEnable(false);
					if (foldbtn_sw[i]) foldbtn_sw[i]->setActiveSwitchSet(2);
				}
			}
		}
	}
}

void JXJ::crJXJUICheckBoxUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUICheckBoxUpdateMethod::addParam( int i, const std::string& str )
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
		m_checkbox[i] = str;
		break;
	case 11:
		m_checkbox[i] = str;
		break;
	case 12:
		m_checkbox[i] = str;
		break;
	case 13:
		m_checkbox[i] = str;
		break;
	case 14:
		m_checkbox[i] = str;
		break;
	case 15:
		m_Recommend = str;
		break;
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
	case 27:
	case 28:
	case 29:
	case 30:
		m_checkbox_sw[i - 16] = str;
		break;
	case 31:
		m_Recommend_sw = str;
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJGenerateCountryOfficialMethod                                                                  */
/************************************************************************/

JXJ::crJXJGenerateCountryOfficialMethod::crJXJGenerateCountryOfficialMethod():
	m_this(NULL)
{
}

JXJ::crJXJGenerateCountryOfficialMethod::crJXJGenerateCountryOfficialMethod( const crJXJGenerateCountryOfficialMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJGenerateCountryOfficialMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGenerateCountryOfficialMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJGenerateCountryOfficialMethod::operator()( crHandle &handle )
{
	//int shiliId[3] = {WCHDATA_JXJShiliWei,WCHDATA_JXJShiliShu,WCHDATA_JXJShiliWu};
	void *param;
	crJXJShili * shili;
	crData * shiliData;
	crData *gsBrainData = m_this->getDataClass();
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	for (int i = 0; i < 3; ++ i)
	{
		gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
		shili = (crJXJShili *)param;
		shiliData = shili->getDataClass();
		shiliData->excHandle(MAKEINT64(WCH_LockData,1));
		shiliData->getParam(WCHDATA_JXJShiliFoundCountryWinCity,param);
		unsigned char winCity = *(unsigned char *)param;
		if (winCity == (unsigned char)-1)
		{//建国完成
			shiliData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
			CandidateVec*candidateVecL = (CandidateVec*)param;
			candidateVecL->clear();
			shiliData->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			CandidateVec*candidateVecT = (CandidateVec*)param;
			if(!candidateVecT->empty())
			{
				gsBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
				LevelRankingMap *levelRankingMap = (LevelRankingMap *)param;
				for( CandidateVec::iterator wbItr = candidateVecT->begin();
					wbItr != candidateVecT->end(); 
					++ wbItr )
				{
					int rankVal = 0;
					LevelRankingMap::iterator lrmItr = levelRankingMap->begin(); 
					for (;lrmItr != levelRankingMap->end(); ++ rankVal,++ lrmItr)
					{
						if (lrmItr->second.valid())
						{
							if (lrmItr->second->getPlayerID() == (*wbItr)->nPlayerID)
							{
								break;
							}
						}
					}
					(*wbItr)->m_rankVal = rankVal + 1;
				}
				std::sort(candidateVecT->begin(),candidateVecT->end(),strToupiaoInfoCmp());

				for (CandidateVec::iterator cvItr = candidateVecT->begin();
					cvItr != candidateVecT->end(); ++ cvItr)
				{
					candidateVecL->push_back(*cvItr);
				}
				candidateVecT->clear();
			}
			unsigned char size = candidateVecL->size();
			shiliData->getParam(WCHDATA_JXJCountryHistory,param);
			CountryHistory *history = (CountryHistory *)param;
			time_t nowTime = time(0);

			for (CandidateVec::iterator cvLItr = candidateVecL->begin();
				cvLItr != candidateVecL->end(); ++ cvLItr)
			{
				(*cvLItr)->m_lastLogin = nowTime;
			}
			int id;
			std::string name;
			if (size >= 1)
			{
				id = (*candidateVecL)[0]->nPlayerID;
				name = (*candidateVecL)[0]->playerName;
				shiliData->inputParam(WCHDATA_JXJChengxiang,&id);
				shiliData->inputParam(WCHDATA_JXJChengxiangName,&name);
				//shiliData->inputParam(WCHDATA_JXJChengxiangLastLogin,&nowTime);
				//candidateVecL->erase(candidateVecL->begin());

				CRCore::ref_ptr<CountryHistoryOfficalChange > chocPtr = new CountryHistoryOfficalChange;
				chocPtr->m_playerName = name;
				chocPtr->m_officalName = "丞相";
				history->push_back(chocPtr.get());

				char log[128];
				memset(log,0,sizeof(log));
				sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());

				std::string logdata(log);
				GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));

				if (size >= 2)
				{
					id = (*candidateVecL)[1]->nPlayerID;
					name = (*candidateVecL)[1]->playerName;
					shiliData->inputParam(WCHDATA_JXJDasima,&id);
					shiliData->inputParam(WCHDATA_JXJDasimaName,&name);
					//shiliData->inputParam(WCHDATA_JXJDasimaLastLogin,&nowTime);
					//candidateVecL->erase(candidateVecL->begin());

					chocPtr = new CountryHistoryOfficalChange;
					chocPtr->m_playerName = name;
					chocPtr->m_officalName = "大司马";
					history->push_back(chocPtr.get());

					sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());
					logdata = log;
					GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));

					if (size >= 3)
					{
						id = (*candidateVecL)[2]->nPlayerID;
						name = (*candidateVecL)[2]->playerName;
						shiliData->inputParam(WCHDATA_JXJDajiangjun,&id);
						shiliData->inputParam(WCHDATA_JXJDajiangjunName,&name);
						//shiliData->inputParam(WCHDATA_JXJDajiangjunLastLogin,&nowTime);
						//candidateVecL->erase(candidateVecL->begin());

						chocPtr = new CountryHistoryOfficalChange;
						chocPtr->m_playerName = name;
						chocPtr->m_officalName = "大将军";
						history->push_back(chocPtr.get());

						sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());
						logdata = log;
						GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));
					}
					else
					{
						shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
						shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
					}
				}
				else
				{
					shiliData->inputParam(WCHDATA_JXJDasima,NULL);
					shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
					shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
					shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
				}
			}
			else
			{
				shiliData->inputParam(WCHDATA_JXJChengxiang,NULL);
				shiliData->inputParam(WCHDATA_JXJChengxiangName,NULL);
				shiliData->inputParam(WCHDATA_JXJDasima,NULL);
				shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
			}
			bool modify = true;
			shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
		}
		shiliData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
}

///************************************************************************/
///*    crJXJCountryCanvasApplyStationMethod                                                                  */
///************************************************************************/
//
JXJ::crJXJCountryCanvasApplyStationMethod::crJXJCountryCanvasApplyStationMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryCanvasApplyStationMethod::crJXJCountryCanvasApplyStationMethod( const crJXJCountryCanvasApplyStationMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJCountryCanvasApplyStationMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCountryCanvasApplyStationMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJCountryCanvasApplyStationMethod::operator()( crHandle &handle )
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
			int cantonId = *(int *)param;
			crPlayerDataEventPacket packet;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(cantonId);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCountryCanvasApplyStation,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		} while (0);
	}
}

/************************************************************************/
/*	    crJXJRecvJCountryCanvasApplyStationMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvJCountryCanvasApplyStationMethod::crJXJRecvJCountryCanvasApplyStationMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvJCountryCanvasApplyStationMethod::crJXJRecvJCountryCanvasApplyStationMethod( const crJXJRecvJCountryCanvasApplyStationMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvJCountryCanvasApplyStationMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvJCountryCanvasApplyStationMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJRecvJCountryCanvasApplyStationMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				int cantonId = m_stream->_readInt();
				void *param;
				crData *data = m_this->getDataClass();

				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
			/*	unsigned char maxNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCantonChengChiAllocateMaxNum,viplv).c_str());*/
				unsigned char maxNum = 2;
				unsigned char retVal = JXJCountryApplyStationRetVal_failed_unkwon;
				
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));
				if (cantonId >= 0)
				{//申请驻扎
					do 
					{
						serverData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						ConsortiaMap::iterator cMitr = consortiamap->find(mygroupid);
						if (cMitr == consortiamap->end() || !(cMitr->second.valid()))
						{
							break;
						}
						if (cMitr->second->checkPermission(playerid,JXJGroup_ApplyStationCanton) == false)
						{
							retVal = JXJCountryApplyStationRetVal_failed_noPermission;
							break;
						}
						ref_ptr<crData>groupData = cMitr->second->getDataClass();

						serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;

						groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
						FenFengedCantonMap  * fenFengedCantonMap = (FenFengedCantonMap *)param;

						FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->find(cantonId);
						if (ffcmItr != fenFengedCantonMap->end())
						{
							retVal = JXJCountryApplyStationRetVal_failed_hasGruadCanton;
							break;
						}
						crData * shiliData = shili->getDataClass();

						CRCore::ScopedLock<crData> lock(*shiliData);
						shiliData->getParam(WCHDATA_JXJShiliCantonMap,param);

						CantonMap * cantonMap = (CantonMap *)param;
						CantonMap::iterator cmItr = cantonMap->find(cantonId);
						if (cmItr == cantonMap->end() || cmItr->second->getConsortiaId() == 0)
						{
							retVal = JXJCountryApplyStationRetVal_failed_noGruadCanton;
							break;
						}
						ChengChiEarningsMap	& chengChiEarningsMap = cmItr->second->getChengChiEarningsMap();
						ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(mygroupid);
						if (ccemItr != chengChiEarningsMap.end())
						{
							retVal = JXJCountryApplyStationRetVal_failed_hasApplied;
							break;
						}

						groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
						ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
						if (applyCantonMap->size() >= maxNum)
						{
							retVal = JXJCountryApplyStationRetVal_failed_full;
							break;
						}
						applyCantonMap->insert(ApplyCantonMap::value_type(cantonId,0));

						CRCore::ref_ptr<ChengChiEarnings> chengChiEarnings = new ChengChiEarnings(mygroupid);
						chengChiEarningsMap.insert(ChengChiEarningsMap::value_type(mygroupid,chengChiEarnings));

						bool modify = true;
						groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
						shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
						time_t nowTime_t = time(0);
						struct  tm  nowTm = *localtime(&nowTime_t); //服务器当前时间
						int cancelCantonTimer = nowTime_t;
						data->inputParam(WCHDATA_JXJCancelCantonTimer, &cancelCantonTimer);
						retVal = JXJCountryApplyStationRetVal_suc;
					} while (0);
				}
				else
				{//取消驻扎
					int newCantonId = m_stream->_readInt();
					do 
					{
						time_t nowTime_t = time(0);
						struct  tm  nowTm = *localtime(&nowTime_t); //服务器当前时间
						data->getParam(WCHDATA_JXJCancelCantonTimer, param);
						int cancelCantonTimer = *(int *)param;
						if (nowTime_t - cancelCantonTimer < 24*3600)
						{
							retVal = JXJCountryCancleStationRetVal_time_limit;
							break;
						}
						serverData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						ConsortiaMap::iterator cMitr = consortiamap->find(mygroupid);
						if (cMitr == consortiamap->end() || !(cMitr->second.valid()))
						{
							break;
						}
						if (cMitr->second->checkPermission(playerid,JXJGroup_ApplyStationCanton) == false)
						{
							retVal = JXJCountryApplyStationRetVal_failed_noPermission;
							break;
						}
						ref_ptr<crData> groupData = cMitr->second->getDataClass();

						serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;

						crData * shiliData = shili->getDataClass();
						CRCore::ScopedLock<crData> lock(*shiliData);

						//if (cMitr != consortiamap->end() && cMitr->second.valid())
						{
							shiliData->getParam(WCHDATA_JXJShiliCantonMap,param);
							CantonMap * cantonMap = (CantonMap *)param;
							CantonMap::iterator cmItr = cantonMap->find(newCantonId);
							if (cmItr != cantonMap->end())
							{
								ChengChiEarningsMap	& chengChiEarningsMap = cmItr->second->getChengChiEarningsMap();
								ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(mygroupid);
								if (ccemItr != chengChiEarningsMap.end())
								{
									chengChiEarningsMap.erase(mygroupid);
								}
							}
							groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
							ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
							ApplyCantonMap::iterator acmItr = applyCantonMap->find(newCantonId);
							if (acmItr != applyCantonMap->end())
							{

								applyCantonMap->erase(acmItr);
								retVal = JXJCountryCancleStationRetVal_suc;
								break;
							}
						}
						bool modify = true;
						groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
						shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
					} while (0);
				}
	
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(retVal);

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCountryCanvasApplyStation,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> CountryCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
			ref_ptr<crButtonWidgetNode> applyCanton = dynamic_cast<crButtonWidgetNode *>(CountryCanvas->getWidget("UI_Country5_Apply"));
			ref_ptr<crButtonWidgetNode> cancleCanton = dynamic_cast<crButtonWidgetNode *>(CountryCanvas->getWidget("UI_Country5_Cancel"));
			ref_ptr<crMultiSwitch> applyCanton_sw = dynamic_cast<crMultiSwitch *>(CountryCanvas->getChildNode("sw84_2"));
			ref_ptr<crMultiSwitch> cancleCanton_sw = dynamic_cast<crMultiSwitch *>(CountryCanvas->getChildNode("sw986"));

			unsigned char retVal = m_stream->_readUChar();
			int msgid = 0;
			switch (retVal)
			{
			case JXJCountryApplyStationRetVal_failed_unkwon:
				msgid = 3044;
				break;
			case JXJCountryApplyStationRetVal_failed_hasApplied:
				msgid = 3045;
				break;
			case JXJCountryApplyStationRetVal_failed_noPermission:
				msgid = 3046;
				break;
			case JXJCountryApplyStationRetVal_failed_hasGruadCanton:
				msgid = 3047;
				break;
			case JXJCountryApplyStationRetVal_failed_noGruadCanton:
				msgid = 3048;
				break;
			case JXJCountryApplyStationRetVal_suc:
				if (applyCanton.valid() && cancleCanton.valid() && cancleCanton_sw.valid())
				{
					applyCanton->setVisiable(false);
					cancleCanton->setVisiable(true);
					cancleCanton->setEnable(true);
					cancleCanton_sw->setActiveSwitchSet(0);
				}
				msgid = 3049;
				break;
			case JXJCountryCancleStationRetVal_suc:
				if (applyCanton.valid() && cancleCanton.valid() && applyCanton_sw.valid())
				{
					cancleCanton->setVisiable(false);
					applyCanton->setVisiable(true);
					applyCanton->setEnable(true);
					applyCanton_sw->setActiveSwitchSet(0);
				}
				msgid = 7007;
				break;
			case JXJCountryCancleStationRetVal_time_limit:
				msgid = 7008;
				break;
			default:
				break;
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}

/************************************************************************/
/*    crJXJResetGroupRadioMethod                                                                  */
/************************************************************************/

JXJ::crJXJResetGroupRadioMethod::crJXJResetGroupRadioMethod():
	m_this(NULL)
{
}

JXJ::crJXJResetGroupRadioMethod::crJXJResetGroupRadioMethod( const crJXJResetGroupRadioMethod & handle ):
	crMethod(handle),
	m_groupRadio(handle.m_groupRadio),
	m_canvas(handle.m_canvas),
	m_val(handle.m_val)
{
}

void JXJ::crJXJResetGroupRadioMethod::inputParam( int i, void *param )
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

void JXJ::crJXJResetGroupRadioMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
		break;
	case 2:
		m_val = (unsigned char )atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJResetGroupRadioMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!canvas) break;
		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
		if (!groupradio) break;
		groupradio->select(m_val);
	} while (0);
}

/************************************************************************/
/*    crJXJInitGroupRadioMethod                                                                  */
/************************************************************************/

JXJ::crJXJInitGroupRadioMethod::crJXJInitGroupRadioMethod():
	m_this(NULL)
{
}

JXJ::crJXJInitGroupRadioMethod::crJXJInitGroupRadioMethod( const crJXJInitGroupRadioMethod & handle ):
	m_canvas(handle.m_canvas),
	m_groupRadio(handle.m_groupRadio)
{
	for (unsigned char i = 0; i < 20; ++ i)
	{
		m_radio[i] = handle.m_radio[i];
	}
}

void JXJ::crJXJInitGroupRadioMethod::inputParam( int i, void *param )
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

void JXJ::crJXJInitGroupRadioMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
		break;
	case 2:
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
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_radio[i - 2] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJInitGroupRadioMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();/*crFilterRenderManager::getInstance()->findCanvas(m_canvas);*/
		if (!canvas) break;
		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
		if (!groupradio) break;
		groupradio->clear();

		for (unsigned char i = 0; i < 20; ++ i)
			groupradio->addRadioName(m_radio[i]);
		
	} while (0);
}

/************************************************************************/
/*    crJXJQueryPeopleInfoListMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryPeopleInfoListMethod::crJXJQueryPeopleInfoListMethod():
	m_this(NULL),
	m_lastMinlv(0),
	m_lastMaxLv(0)
{
}

JXJ::crJXJQueryPeopleInfoListMethod::crJXJQueryPeopleInfoListMethod( const crJXJQueryPeopleInfoListMethod & handle ):
	crMethod(handle),
	m_maxLv(handle.m_maxLv),
	m_minLv(handle.m_minLv),
	m_page(handle.m_page)
{
}

void JXJ::crJXJQueryPeopleInfoListMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryPeopleInfoListMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_minLv = str;
		break;
	case 1:
		m_maxLv = str;
		break;
	default:
		break;
	}
}

class CheckValidString
{
public:
	bool operator () (std::string & str)
	{
		const char * strPtr = str.c_str();
		for (unsigned char i = 0; i < str.length(); ++ i)
		{
			if (strPtr[i] < '0' || strPtr[i] > '9') return false;
		}
		return true;
	}
};

void JXJ::crJXJQueryPeopleInfoListMethod::operator()( crHandle &handle )
{
	int msgid = 0;
	do 
	{
		unsigned char minLevel = 0;
		unsigned char maxLevel = 0;
		if (!m_this) break;
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (!canvas) break;
		ref_ptr<crEditWidgetNode>  minLvEdit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_minLv));
		ref_ptr<crEditWidgetNode>  maxLvEdit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_maxLv));
		std::string validString1 = minLvEdit->getUTF8String();
		std::string validString2 = maxLvEdit->getUTF8String();
		CheckValidString check;
		if (!check(validString1) || !check(validString2))
		{
			msgid = 3052;
			break;
		}
		minLevel = atoi(validString1.c_str());
		maxLevel = atoi(validString2.c_str());
		if (minLevel > maxLevel)
			std::swap(minLevel,maxLevel);
		crData * canvasData = canvas->getDataClass();
		if (!canvasData) break;

		void *param;
		canvasData->getParam(WCHDATA_JXJCountryQueryPeopleListMinLv	,param);
		m_lastMinlv = *(unsigned char *)param;
		canvasData->getParam(WCHDATA_JXJCountryQueryPeopleListMaxLv	,param);
		m_lastMaxLv = *(unsigned char *)param;
		if (minLevel != m_lastMinlv || maxLevel != m_lastMaxLv)
		{
			int curPage = 1;
			canvasData->inputParam(WCHDATA_JXJCountryPeopleListCurPage,&curPage);
			m_lastMinlv = minLevel;
			m_lastMaxLv = maxLevel;

			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMinLv	,&m_lastMinlv);
			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMaxLv	,&m_lastMaxLv);

			//canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			//canvasData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
			//PeopleVec * peopleVec = (PeopleVec *)param;
			//peopleVec->clear();
			//canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		canvasData->getParam(WCHDATA_JXJCountryPeopleListCurPage,param);
		int curPage = *(int *)param;
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		
		if (!netConductor) break;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeUChar(minLevel);
		stream->_writeUChar(maxLevel);
		stream->_writeInt(curPage);
		
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryPeopleInfoList,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);

	} while (0);
	if (msgid)
	{
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if (playerGameData)
		{
			playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}

/************************************************************************/
/*    crJXJRecvQueryPeopleInfoListMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryPeopleInfoListMethod::crJXJRecvQueryPeopleInfoListMethod():
	m_this(NULL),
	m_netType(GameClient_Game),
	m_stream(NULL)
{
}

JXJ::crJXJRecvQueryPeopleInfoListMethod::crJXJRecvQueryPeopleInfoListMethod( const crJXJRecvQueryPeopleInfoListMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJRecvQueryPeopleInfoListMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryPeopleInfoListMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJRecvQueryPeopleInfoListMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				unsigned char minLevel = m_stream->_readUChar();
				unsigned char maxLevel = m_stream->_readUChar();
				int curPage = m_stream->_readInt();

				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData * shiliData = shili->getDataClass();
					if (!shiliData) break;
					shiliData->excHandle(MAKEINT64(WCH_LockData,1));

					//shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					//PeopleMap * peopleMap = (PeopleMap *)param;
					//if (peopleMap->empty())
					//{
					//	shiliData->excHandle(MAKEINT64(WCH_LockData,0));
					//	break;
					//}

					//shiliData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
					//PeopleVec * tempPeopleVec = (PeopleVec *)param;
					//ref_ptr<PeopleInfo> peopleInfo = NULL;
					//if (tempPeopleVec->empty())
					//{
					//	PeopleMap::iterator pmItr = peopleMap->begin();
					//	for (; pmItr != peopleMap->end(); ++ pmItr)
					//	{
					//		if (pmItr->second.valid())
					//		{
					//			// 防止外部修改,导致排序紊乱
					//			peopleInfo = new PeopleInfo(*(pmItr->second.get()));
					//			tempPeopleVec->push_back(peopleInfo);
					//		}
					//	}
					//	if (tempPeopleVec->empty() == false)
					//		std::sort(tempPeopleVec->begin(),tempPeopleVec->end(),PeopleInfoCmp());
					//}
					//int minLvIdx = 0;
					//int maxLvIdx = 0;
					//shiliData->getParam(WCHDATA_JXJQueryFindByMinLvMap,param);
					//UIQueryFindByMinLvMap * findByMinLvMap = (UIQueryFindByMinLvMap *)param;
					//if (findByMinLvMap->empty())
					//{
					//	PeopleVec::iterator pvItr = tempPeopleVec->begin();
					//	for (minLvIdx = 0; pvItr != tempPeopleVec->end(); ++ minLvIdx,++ pvItr)
					//	{
					//		if (pvItr->valid())
					//		{
					//			if ( (*pvItr)->m_lv >= minLevel )
					//			{
					//				break;
					//			}
					//		}
					//	}
					//	if (pvItr == tempPeopleVec->end()) -- minLvIdx;
					//	findByMinLvMap->insert(UIQueryFindByMinLvMap::value_type(minLevel,minLvIdx));
					//}
					//UIQueryFindByMinLvMap::iterator fmmItr = findByMinLvMap->lower_bound(minLevel);

					//if (fmmItr != findByMinLvMap->end())
					//{
					//	for (int i = fmmItr->second; i >= 0; -- i)
					//	{
					//		if ((*tempPeopleVec)[i].valid())
					//		{
					//			if ( (*tempPeopleVec)[i]->m_lv <= minLevel)
					//			{
					//				break;
					//			}
					//			else
					//			{
					//				minLvIdx = i;
					//			}
					//		}
					//	}
					//}
					//else
					//{
					//	PeopleVec::iterator pvItr = tempPeopleVec->begin();
					//	for (minLvIdx = 0; pvItr != tempPeopleVec->end(); ++ minLvIdx,++ pvItr)
					//	{
					//		if (pvItr->valid())
					//		{
					//			if ( (*pvItr)->m_lv >= minLevel )
					//			{
					//				break;
					//			}
					//		}
					//	}
					//	if (pvItr == tempPeopleVec->end()) -- minLvIdx;
					//}
					//findByMinLvMap->insert(UIQueryFindByMinLvMap::value_type(minLevel,minLvIdx));

					//shiliData->getParam(WCHDATA_JXJQueryFindByMaxLvMap,param);
					//UIQueryFindByMaxLvMap * findByMaxLvMap = (UIQueryFindByMaxLvMap *)param;
					//if (findByMaxLvMap->empty())
					//{
					//	PeopleVec::iterator pvItr = tempPeopleVec->begin();
					//	
					//	for (maxLvIdx = 0; pvItr != tempPeopleVec->end(); ++ maxLvIdx,++ pvItr)
					//	{
					//		if (pvItr->valid())
					//		{
					//			if ( (*pvItr)->m_lv > maxLevel )
					//			{
					//				break;
					//			}
					//		}
					//	}
					//	if (maxLvIdx) -- maxLvIdx; 
					//	findByMaxLvMap->insert(UIQueryFindByMaxLvMap::value_type(maxLevel,maxLvIdx));
					//}
					//UIQueryFindByMaxLvMap::iterator qfmmItr = findByMaxLvMap->lower_bound(maxLevel);
					//if (qfmmItr != findByMaxLvMap->end())
					//{
					//	int i = qfmmItr->second;
					//	for (; i >= 0; -- i)
					//	{
					//		if ((*tempPeopleVec)[i].valid())
					//		{
					//			if ( (*tempPeopleVec)[i]->m_lv <= maxLevel)
					//			{
					//				break;
					//			}
					//		}
					//	}
					//	maxLvIdx = i;
					//}
					//else
					//{
					//	PeopleVec::iterator pvItr = tempPeopleVec->begin();
					//	for (maxLvIdx = 0; pvItr != tempPeopleVec->end(); ++ maxLvIdx,++ pvItr)
					//	{
					//		if (pvItr->valid())
					//		{
					//			if ( (*pvItr)->m_lv >= maxLevel )
					//			{
					//				break;
					//			}
					//		}
					//	}
					//	if (maxLvIdx) -- maxLvIdx; 
					//}

					//findByMaxLvMap->insert(UIQueryFindByMaxLvMap::value_type(maxLevel,maxLvIdx));

					shiliData->excHandle(MAKEINT64(WCH_LockData,0));
					serverData->getParam(WCHDATA_JXJCountryUIQueryPeopleInfoDeque,param);
					UIQueryPeopleInfoDeq * queryPeopleInfoDeq = (UIQueryPeopleInfoDeq *)param;
					//UIQueryPeopleInfoDeq::iterator pimItr = queryPeopleInfoDeq->find(playerid);
					
					CRCore::ref_ptr<UIQueryPeopleInfoByLevel> item = new UIQueryPeopleInfoByLevel;
					item->m_playerid = playerid;
					item->m_shili = shiliid;
					item->m_curPage = curPage;
					item->m_minLv = minLevel;
					item->m_maxLv = maxLevel;
					item->m_queryTime = time(NULL);
					queryPeopleInfoDeq->push_back(item);
					//if (pimItr != queryPeopleInfoDeq->end())
					//{
					//	pimItr->second = item;
					//}
					//else
					//{
					//	queryPeopleInfoDeq->insert(UIQueryPeopleInfoDeq::value_type(playerid,item));
					//}
				} while (0);
				
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
		}
	}
}

/************************************************************************/
/*    crJXJQueryPeopleInfoDisposeMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryPeopleInfoDisposeMethod::crJXJQueryPeopleInfoDisposeMethod():
	m_this(NULL),
	m_count(0)
{
}

JXJ::crJXJQueryPeopleInfoDisposeMethod::crJXJQueryPeopleInfoDisposeMethod( const crJXJQueryPeopleInfoDisposeMethod& handle ):
	crMethod(handle),
	m_count(handle.m_count)
{
}

void JXJ::crJXJQueryPeopleInfoDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryPeopleInfoDisposeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJQueryPeopleInfoDisposeMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		crData * gsBrainData = m_this->getDataClass();
		if (!gsBrainData) break;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 

		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

		void *param;

		gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = (ConsortiaMap *)param;
		ConsortiaMap::iterator cMitr;
		gsBrainData->getParam(WCHDATA_JXJCountryUIQueryPeopleInfoDeque,param);
		UIQueryPeopleInfoDeq *queryPeopleDeq = (UIQueryPeopleInfoDeq *)param;
		crPlayerDataEventPacket packet;
		CRCore::ref_ptr<CRCore::crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(64);
		int playerid = 0;
		int tmpCount = 0;
		while (!queryPeopleDeq->empty() && tmpCount < m_count)
		{
			CRCore::ref_ptr<UIQueryPeopleInfoByLevel> item = queryPeopleDeq->front();
			queryPeopleDeq->pop_front();
			++ tmpCount;

			playerid = item->m_playerid;
			ref_ptr<crGameServerPlayerData> playerData = 
				dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			
			do 
			{
				if (! (playerData.valid()) ) break;
				if ( (playerData->getPlayerGameData() == NULL)
					) break;

				gsBrainData->getParam(WCHDATA_JXJShiliWei+item->m_shili-c_startShiliID,param);
				crJXJShili *shili = (crJXJShili *)param;
				crData * shiliData = shili->getDataClass();
				if (!shiliData) break;

				shiliData->excHandle(MAKEINT64(WCH_LockData,1));

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap * peopleMap = (PeopleMap *)param;
				shiliData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
				PeopleVec * peopleVec = (PeopleVec *)param;
				peopleVec->clear();

				std::string groupName;
				peopleVec->reserve(peopleMap->size());
				for (PeopleMap::iterator pmItr = peopleMap->begin(); pmItr != peopleMap->end(); ++ pmItr)
				{
					if (pmItr->second->getLv() >= item->m_minLv 
						&& pmItr->second->getLv() <= item->m_maxLv)
					{
						peopleVec->push_back(pmItr->second);
					}
				}

				stream->seekBegin();
				stream->_writeUChar(0);
				stream->_writeShort(peopleVec->size());

				if (peopleVec->empty() == false)
					std::sort(peopleVec->begin(),peopleVec->end(),PeopleInfoCmp());
				int i = (item->m_curPage - 1) * 9;
				int beginIdx = i;
				int endIdx = i + 9;
				if (i < peopleVec->size())
				{
					for (; i < endIdx && i < peopleVec->size(); ++ i)
					{
						stream->_writeBool((*peopleVec)[i]->getSmallAcc());
						stream->_writeUChar((*peopleVec)[i]->getLv());
						stream->_writeUChar((*peopleVec)[i]->getTroopsLv());
						stream->_writeInt((*peopleVec)[i]->getID());
						stream->_writeString((*peopleVec)[i]->getName());
						cMitr = consortiamap->find((*peopleVec)[i]->getConsortiaId());
						groupName = "";
						if (cMitr != consortiamap->end() && cMitr->second.valid())
							cMitr->second->getName(groupName);
						stream->_writeString(groupName);
					}
				}
				int bufSize = stream->getBufSize();
				stream->seekBegin();
				stream->_writeUChar(i - beginIdx);
				stream->setBufSize(bufSize);
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));

				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvPeopleInfoList	,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			} while (0);
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	} while (0);

}

/************************************************************************/
/*    crJXJRecvPeopleInfoListMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvPeopleInfoListMethod::crJXJRecvPeopleInfoListMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvPeopleInfoListMethod::crJXJRecvPeopleInfoListMethod( const crJXJRecvPeopleInfoListMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJRecvPeopleInfoListMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvPeopleInfoListMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvPeopleInfoListMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this || !m_stream || m_stream.valid() == false) break;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
		if (!canvas || canvas->getDataClass() == NULL) break;
		crData * countryData = canvas->getDataClass();
		void *param;
		countryData->excHandle(MAKEINT64(WCH_LockData,1));
		countryData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
		PeopleVec * peopleVec = (PeopleVec *)param;
		peopleVec->clear();
		unsigned char size = m_stream->_readUChar();
		peopleVec->reserve(size);
		int totalPage = (m_stream->_readShort() + 8 ) / 9;
		CRCore::ref_ptr<PeopleInfo> recvItem ;
		//std::string name ;
		for (unsigned char i = 0; i < size ; ++ i)
		{
			recvItem = new PeopleInfo;
			recvItem->setSamllAcc(m_stream->_readBool());
			recvItem->setLv( m_stream->_readUChar());
			recvItem->setTroopsLv( m_stream->_readUChar());
			recvItem->setID( m_stream->_readInt());
			recvItem->setName( m_stream->_readString());
			recvItem->setConsortiaName( m_stream->_readString() );
			peopleVec->push_back(recvItem);
		}
		countryData->excHandle(MAKEINT64(WCH_LockData,0));
		countryData->inputParam(WCHDATA_JXJCountryPeopleListTotalPage,&totalPage);
		//canvas->doEvent(WCH_UI_UpdateData);
	} while (0);
}

/************************************************************************/
/*    crJXJUISmallAccUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUISmallAccUpdateMethod::crJXJUISmallAccUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUISmallAccUpdateMethod::crJXJUISmallAccUpdateMethod( const crJXJUISmallAccUpdateMethod & handle ):
	crMethod(handle),
	m_confirm(handle.m_confirm),
	m_filter(handle.m_filter),
	m_query(handle.m_query),
	m_shili(handle.m_shili),
	m_table(handle.m_table),
	m_pageInput (handle.m_pageInput)

{
	for (unsigned char i = 0; i < 9; ++ i)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}

void JXJ::crJXJUISmallAccUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUISmallAccUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_shili = str;
		break;
	case 1:
		m_filter = str;
		break;
	case 2:
		m_confirm = str;
		break;
	case 3:
		m_table = str;
		break;
	case 4:
		m_pageInput = str;
		break;
	case 5:
		m_query = str;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		m_checkbox[i - 6] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUISmallAccUpdateMethod::operator()( crHandle &handle )
{
	do 
	{
		std::string shiliName[] = {"魏 国","蜀 国","吴 国"};
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if (!m_this || !playerGameData || playerGameData->getDataClass() == NULL) break;
		if (m_this->getDataClass() == NULL) break;
		void *param;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;
		ref_ptr<crStaticTextWidgetNode>  shili = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_shili));
		if (shili.valid())
			shili->setString(shiliName[shiliid - c_startShiliID]);
		ref_ptr<crTableWidgetNode>  tableWidget = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_table));

		ref_ptr<crButtonWidgetNode>  confirmBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_confirm));
		ref_ptr<crCheckBoxWidgetNode>  checkbox[9] = {NULL};
		for (unsigned char i = 0; i < 9; ++ i)
		{
			checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_checkbox[i]));
			if (checkbox[i].valid()) checkbox[i]->setVisiable(false);
		}
		crData * canvasData = m_this->getDataClass();
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		canvasData->getParam(WCHDATA_JXJCountryCanvasOfficalPos,param);
		unsigned char officalPos = *(unsigned char *)param;
		PermissionCheck  permissionCheck;
		if (permissionCheck(officalPos,JXJCountry_Protect))
		{
			if (confirmBtn.valid())
				confirmBtn->setVisiable(true);
		}
		else
		{
			if (confirmBtn.valid())
				confirmBtn->setVisiable(false);
		}

		canvasData->getParam(WCHDATA_JXJCountryPeopleListCurPage,param);
		int curPage = *(int *) param;
		canvasData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
		PeopleVec  & peopleVec = *(PeopleVec *)param;
		int vecSize = peopleVec.size();
		tableWidget->clearData();
		for (int i = 0; i < 9 && i < vecSize; ++ i)
		{
			tableWidget->addData(0,i, crArgumentParser::appItoa(peopleVec[i]->getLv()));
			tableWidget->addData(1,i, peopleVec[i]->getName());
			tableWidget->addData(2,i, crArgumentParser::appItoa(peopleVec[i]->getTroopsLv()));
			tableWidget->addData(3,i, peopleVec[i]->getConsortiaName());
			tableWidget->addData(i,peopleVec[i]->getID());
			if (checkbox[i].valid())
			{
				if (permissionCheck(officalPos,JXJCountry_Protect))
					checkbox[i]->setVisiable(true);
				else
					checkbox[i]->setVisiable(false);

				if (peopleVec[i]->getSmallAcc())
				{
					checkbox[i]->setVisiable(true);
					checkbox[i]->select(true);
				}
				else
				{
					checkbox[i]->select(false);
				}
			}
		}

		ref_ptr<crStaticTextWidgetNode>  pageinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageInput));
		canvasData->getParam(WCHDATA_JXJCountryPeopleListTotalPage,param);
		int totalPage = *(int *)param;
		pageinput->setString(crArgumentParser::appItoa(curPage) + "/" + crArgumentParser::appItoa(totalPage));

		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	} while (0);

}

/************************************************************************/
/*    crJXJUIConfirmSmallAccMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIConfirmSmallAccMethod::crJXJUIConfirmSmallAccMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIConfirmSmallAccMethod::crJXJUIConfirmSmallAccMethod( const crJXJUIConfirmSmallAccMethod & handle ):
	m_canvas(handle.m_canvas),
	crMethod(handle)
{
	for (unsigned char i = 0; i < 9; ++ i)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}


void JXJ::crJXJUIConfirmSmallAccMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIConfirmSmallAccMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		m_checkbox[i] = str;
		break;
	case 9:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIConfirmSmallAccMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!parentCanvas || parentCanvas->getDataClass() == NULL) break;
		crData * canvasData = parentCanvas->getDataClass();
		ref_ptr<crCheckBoxWidgetNode>  checkbox[9] = {NULL};
		for (unsigned char i = 0; i < 9; ++ i)
		{
			checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_checkbox[i]));
		}
		PeopleIdList intoBlacklist;
		intoBlacklist.reserve(9);
		PeopleIdList outOfBlacklist;
		outOfBlacklist.reserve(9);
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;

		//canvasData->getParam(WCHDATA_JXJCountryPeopleListCurPage,param);
		//int curPage = *(int *) param;
		canvasData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
		PeopleVec * peopleVec = (PeopleVec *) param;
		int vecSize = peopleVec->size();
		for (int i = 0; i < vecSize  && i < 9; ++ i)
		{
			if (checkbox[i]->getSelect() != (*peopleVec)[i]->getSmallAcc())
			{
				if (checkbox[i]->getSelect() == false)
					outOfBlacklist.push_back((*peopleVec)[i]->getID());
				else
					intoBlacklist.push_back((*peopleVec)[i]->getID());
				(*peopleVec)[i]->setSamllAcc( checkbox[i]->getSelect());
			}
		}
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));

		if (outOfBlacklist.empty() && intoBlacklist.empty())
		{
			break;
		}

		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (!netConductor) break;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(80);
		stream->_writeUChar(outOfBlacklist.size());
		PeopleIdList::iterator pilItr = outOfBlacklist.begin();
		for (; pilItr != outOfBlacklist.end(); ++ pilItr)
		{
			stream->_writeInt(*pilItr);
		}
		stream->_writeUChar(intoBlacklist.size());
		pilItr = intoBlacklist.begin();
		for (; pilItr != intoBlacklist.end(); ++ pilItr)
		{
			stream->_writeInt(*pilItr);
		}
		
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUIConfirmSmallAcc,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	} while (0);
}
/************************************************************************/
/*    crJXJRecvUIConfirmSmallAccMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvUIConfirmSmallAccMethod::crJXJRecvUIConfirmSmallAccMethod():
	m_this(NULL),
	m_stream(NULL)
{
}

JXJ::crJXJRecvUIConfirmSmallAccMethod::crJXJRecvUIConfirmSmallAccMethod( const crJXJRecvUIConfirmSmallAccMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvUIConfirmSmallAccMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvUIConfirmSmallAccMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJRecvUIConfirmSmallAccMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				void *param;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				
				unsigned char retVal = JXJCountrySetSamllAccRet_failed_unkwon;
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData * shiliData = shili->getDataClass();
					if (!shiliData) break;
					CRCore::ScopedLock<crData> lock(*shiliData);
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap * peopleMap = (PeopleMap *)param;
					if (peopleMap->empty())
					{
						break;
					}

					shiliData->getParam(WCHDATA_JXJCountrySmallAccNum,param);
					short smallAccLimit = *(short *)param;

					unsigned char size = m_stream->_readUChar();
					int id;
					PeopleMap::iterator pmItr;
					for (unsigned char i = 0; i < size; ++ i)
					{
						id = m_stream->_readInt();
						pmItr = peopleMap->find(id);
						if (pmItr != peopleMap->end() && pmItr->second.valid())
						{
							if (pmItr->second->getSmallAcc())
							{
								pmItr->second->setSamllAcc( false);
								-- smallAccLimit;

								ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
								crTableIO::StrVec textrecord;
								int textid = 7000;
								if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
								{
									CRCore::ref_ptr<crStreamBuf> pMailStream = new crStreamBuf;
									pMailStream->createBuf(64);
									pMailStream->_writeString(pmItr->second->getName());
									pMailStream->_writeString("解除小号标记");

									ItemMap itemMap;
									rcfg::ConfigScript cfg_script(&itemMap);
									cfg_script.Add("Hypertext");
									cfg_script.Push("Hypertext");
									cfg_script.Add("Content");
									cfg_script.Push("Content");
									cfg_script.Add("Text",textrecord[1]);
									cfg_script.Pop();
									cfg_script.Pop();

									CRCore::ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
									crGlobalHandle::getInstance()->getScriptStream(cfg_script, mailBuffer);
									pMailStream->write(mailBuffer->getPtr(),mailBuffer->getBufSize());

									int bufSize = pMailStream->getBufSize();
									pMailStream->seekBegin();
									pMailStream->setBufSize(bufSize);

									crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvSystemMail,MAKEINT64(pMailStream.get(),NULL));
								}
							}
						}
					}
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					unsigned char maxNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountrySmallAccMaxNum,viplv).c_str());
					size = m_stream->_readUChar();
					//crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					//int titleTextId = 4079;
					if (size + smallAccLimit >= maxNum)
					{
						retVal = JXJCountrySetSamllAccRet_failed_limit;
						break;
					}
					unsigned char govPost = 0;
					serverData->getParam(WCHDATA_JXJGovpostRankingMap,param);
					GovpostRankingMap * govpostRankingmap = (GovpostRankingMap *)param;
					if (govpostRankingmap->empty())
					{
						break;
					}
					GovpostRankingMap::iterator pmItr_1 = govpostRankingmap->begin();
					id = m_stream->_readInt();
					for (; pmItr_1 != govpostRankingmap->end(); pmItr_1++)
					{
						if (pmItr_1->second->getPlayerID() == id)
						{
							govPost = pmItr_1->second->getGovpost();
						}
					}
					unsigned char pos_limit = 0;
					int tianshu = 0;
					if (crGlobalHandle::isClient())
					{
						crGlobalHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
					}
					else
					{
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
					}
					if (0 <= tianshu && tianshu < 28)
					{
						pos_limit = 4 + (tianshu / 3);
					}
					else
					{
						pos_limit = 13;
					}
					if (govPost > pos_limit )
					{
						retVal = JXJCountrySetSmallAccRet_failed_poslimit;
						break;
					}
					for (unsigned char i = 0; i < size; ++ i)
					{
						id = m_stream->_readInt();
						pmItr = peopleMap->find(id);
						if (pmItr != peopleMap->end() && pmItr->second.valid())
						{
							if (!(pmItr->second->getSmallAcc()))
							{
								pmItr->second->setSamllAcc( true);
								++ smallAccLimit;

								ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
								crTableIO::StrVec textrecord;
								int textid = 4079;
								if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
								{
									CRCore::ref_ptr<crStreamBuf> pMailStream = new crStreamBuf;
									pMailStream->createBuf(64);
									pMailStream->_writeString(pmItr->second->getName());
									pMailStream->_writeString("小号标记");

									ItemMap itemMap;
									rcfg::ConfigScript cfg_script(&itemMap);
									cfg_script.Add("Hypertext");
									cfg_script.Push("Hypertext");
									cfg_script.Add("Content");
									cfg_script.Push("Content");
									cfg_script.Add("Text",textrecord[1]);
									cfg_script.Pop();
									cfg_script.Pop();

									CRCore::ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
									crGlobalHandle::getInstance()->getScriptStream(cfg_script, mailBuffer);
									pMailStream->write(mailBuffer->getPtr(),mailBuffer->getBufSize());

									int bufSize = pMailStream->getBufSize();
									pMailStream->seekBegin();
									pMailStream->setBufSize(bufSize);

									crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvSystemMail,MAKEINT64(pMailStream.get(),NULL));
								}
							}
						}
					}

					shiliData->inputParam(WCHDATA_JXJCountrySmallAccNum,&smallAccLimit );
					retVal = JXJCountrySetSamllAccRet_suc;
				} while (0);
				
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				CRCore::ref_ptr<crStreamBuf > stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(retVal);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUIConfirmSmallAcc,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int msgid = 0;
			switch (m_stream->_readUChar())
			{
			case JXJCountrySetSamllAccRet_failed_limit:
				msgid = 4055;
				break;
			case JXJCountrySetSamllAccRet_suc:
				msgid = 3053;
				break;
			case JXJCountrySetSmallAccRet_failed_poslimit:
				msgid = 7002;
				break;
			default:
				break;
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}

/************************************************************************/
/*    crJXJQueryCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryCountryBenefitMethod::crJXJQueryCountryBenefitMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryCountryBenefitMethod::crJXJQueryCountryBenefitMethod( const crJXJQueryCountryBenefitMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJQueryCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryCountryBenefitMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJQueryCountryBenefitMethod::operator()( crHandle &handle )
{
	do 
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (!m_this || !netConductor)  break;
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryCountryBenefit,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	} while (0);
}

/************************************************************************/
/*    crJXJRecvQueryCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryCountryBenefitMethod::crJXJRecvQueryCountryBenefitMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryCountryBenefitMethod::crJXJRecvQueryCountryBenefitMethod( const crJXJRecvQueryCountryBenefitMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{

}

void JXJ::crJXJRecvQueryCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryCountryBenefitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvQueryCountryBenefitMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				void *param;
				crData *data = m_this->getDataClass();

				CRCore::ref_ptr<crStreamBuf > stream = new crStreamBuf;
				stream->createBuf(7);
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				data->getParam(WCHDATA_JXJCountryBenefitFeteTimes,param);
				unsigned char feteTimes = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJCountryBenefitEnrichType,param);
				unsigned char enrichType = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJCountryBenefitEnrichTimes	,param);
				unsigned char enrichTimes = *(unsigned char *)param;
				
				stream->_writeUChar(feteTimes);
				stream->_writeUChar(enrichType);
				stream->_writeUChar(enrichTimes);
				bool retVal = false;
				
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData * shiliData = shili->getDataClass();
					if (!shiliData) break;

					shiliData->getParam(WCHDATA_JXJCountryStartedFete,param);
					bool startedFete = *(bool *)param;
					shiliData->getParam(WCHDATA_JXJCountryStartedEnrichCountry,param);
					bool startedEnrich = *(bool *)param;
					stream->_writeBool(startedFete);
					stream->_writeBool(startedEnrich);
					retVal = true;
				} while (0);
				
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				if (retVal)
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryCountryBenefit,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				crData * canvasData = canvas->getDataClass();

				unsigned char feteTimes = m_stream->_readUChar();
				unsigned char enrichType = m_stream->_readUChar();
				unsigned char enrichTimes = m_stream->_readUChar();
				bool startedFete = m_stream->_readBool();
				bool startedEnrich = m_stream->_readBool();
				canvasData->inputParam(WCHDATA_JXJCountryStartedFete,&startedFete);
				canvasData->inputParam(WCHDATA_JXJCountryStartedEnrichCountry,&startedEnrich);
				canvasData->inputParam(WCHDATA_JXJCountryBenefitFeteTimes,&feteTimes);
				canvasData->inputParam(WCHDATA_JXJCountryBenefitEnrichType,&enrichType);
				canvasData->inputParam(WCHDATA_JXJCountryBenefitEnrichTimes	,&enrichTimes);
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/************************************************************************/
/*    crJXJUICountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJUICountryBenefitUpdateMethod::crJXJUICountryBenefitUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJUICountryBenefitUpdateMethod::crJXJUICountryBenefitUpdateMethod( const crJXJUICountryBenefitUpdateMethod & handle ):
	crMethod(handle),
	m_fete(handle.m_fete),
	m_fete_sw(handle.m_fete_sw),
	m_enrich(handle.m_enrich),
	m_enrich_sw(handle.m_enrich_sw),
	m_doFete(handle.m_doFete),
	m_doFete_sw(handle.m_doFete_sw)
	
{
	for (unsigned char i = 0; i < 3; ++ i)
	{
		m_doEnrich[i] = handle.m_doEnrich[i];
		m_doEnrich_sw[i] = handle.m_doEnrich_sw[i];
		if (i < 2)
		{
			m_describe_title[i] = handle.m_describe_title[i];
			m_describe_text[i] = handle.m_describe_text[i];
		}
	}
}

void JXJ::crJXJUICountryBenefitUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUICountryBenefitUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_fete = str;
		break;
	case 1:
		m_fete_sw = str;
		break;
	case 2:
		m_enrich = str;
		break;
	case 3:
		m_enrich_sw = str;
		break;
	case 4:
		m_doFete = str;
		break;
	case 5:
	case 6:
	case 7:
		m_doEnrich[i - 5] = str;
		break;
	case 8:
	case 9:
	case 10:
		m_doEnrich_sw[i - 8] = str;
		break;
	case 11:
		m_doFete_sw = str;
		break;
	case 12:
	case 13:
		m_describe_title[i - 12] = str;
		break;
	case 14:
	case 15:
		m_describe_text[i - 14] = str;
		break;
	}
}

void JXJ::crJXJUICountryBenefitUpdateMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this || m_this->getDataClass() == NULL) break;
		crMultiSwitch *fete_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_fete_sw));
		crMultiSwitch *enrich_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_enrich_sw));
		crMultiSwitch *doFete_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_doFete_sw));

		crMultiSwitch *doEnrich_sw[3] = {NULL};
		ref_ptr<crButtonWidgetNode>  doEnrich[3] = {NULL};
		ref_ptr<crStaticTextWidgetNode>  title[2] = {NULL};
		ref_ptr<crStaticTextWidgetNode>  text[2] = {NULL};
		for ( unsigned char i = 0; i < 3; ++ i)
		{
			doEnrich_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_doEnrich_sw[i]));
			doEnrich[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_doEnrich[i]));
			if (i < 2)
			{
				title[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_describe_title[i]));
				text[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_describe_text[i]));
			}
		}

		ref_ptr<crButtonWidgetNode>  fete = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_fete));
		ref_ptr<crButtonWidgetNode>  doFete = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_doFete));
		ref_ptr<crButtonWidgetNode>  enrich = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_enrich));


		crData * canvasData = m_this->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJCountryStartedFete,param);
		bool startedFete = *(bool *)param;
		canvasData->getParam(WCHDATA_JXJCountryStartedEnrichCountry,param);
		bool startedEnrich = *(bool *)param;

		canvasData->getParam(WCHDATA_JXJCountryCanvasOfficalPos,param);
		unsigned char officalPos = *(unsigned char *)param;

		canvasData->getParam(WCHDATA_JXJCountryBenefitFeteTimes	,param);
		unsigned char feteTimes = *(unsigned char *)param;
		canvasData->getParam(WCHDATA_JXJCountryBenefitEnrichType	,param);
		unsigned char enrichType = *(unsigned char *)param;
		canvasData->getParam(WCHDATA_JXJCountryBenefitEnrichTimes,param);
		unsigned char enrichTimes = *(unsigned char *)param;
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if (!playerGameData || playerGameData->getDataClass() == NULL) break;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;

		ref_ptr<crTableIO>  countryBenefitTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBenefitTable);
		if (false == countryBenefitTab.valid()) break;
		int timesLimitIdx = countryBenefitTab->getTitleIndex("每日次数");

		crTableIO::StrVec record;

		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		std::string notice = "";
		int mode = 2;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		int titleTextId = 0;
		int contentTextId = 0;
		
		if (startedFete)
		{
			if (fete_sw)
				fete_sw->setActiveSwitchSet(1);
			if (countryBenefitTab->queryOneRecord(
				0,crArgumentParser::appItoa(JXJCountryBenefitType_fete),
				1,crArgumentParser::appItoa(viplv),
				record)>=0)
			{
				int timesLimit = atoi(record[timesLimitIdx].c_str());
				if (feteTimes >= timesLimit)
				{
					doFete_sw->setActiveSwitchSet(3);
					doFete->setEnable(false);
				}
				else
				{
					doFete_sw->setActiveSwitchSet(0);
					doFete->setEnable(true);
				}
			}
			titleTextId = 3058;
			contentTextId = 3059;
		}
		else
		{
			if (fete_sw)
				fete_sw->setActiveSwitchSet(0);
			PermissionCheck  permissionCheck;
			if (permissionCheck(officalPos,JXJCountry_Fete_Business	))
			{
				fete->setVisiable(true);
			}
			else
			{
				fete->setVisiable(false);
			}
			titleTextId = 3054;
			contentTextId = 3055;
		}
		if(title[0].valid())
		{
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&titleTextId);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			title[0]->setString(notice);
		}
		if(text[0].valid())
		{
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&contentTextId);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			text[0]->setString(notice);
		}
		if (startedEnrich)
		{
			titleTextId = 3060;
			contentTextId = 3061;
			if (enrich_sw) enrich_sw->setActiveSwitchSet(1);
			if (enrichType && enrichTimes)
			{
				unsigned char j = 0;
				for (unsigned char i = JXJCountryBenefitType_smallInvest; i <= JXJCountryBenefitType_largeInvest; ++ i,++ j)
				{
					if (i != enrichType)
					{
						doEnrich[j]->setEnable(false);
						doEnrich_sw[j]->setActiveSwitchSet(3);
					}
					else
					{
						if (countryBenefitTab->queryOneRecord(
							0,crArgumentParser::appItoa(enrichType),
							1,crArgumentParser::appItoa(viplv),
							record)>=0)
						{
							int timesLimit = atoi(record[timesLimitIdx].c_str());
							if (enrichTimes >= timesLimit)
							{
								doEnrich_sw[j]->setActiveSwitchSet(3);
								doEnrich[j]->setEnable(false);
							}
							else
							{
								doEnrich_sw[j]->setActiveSwitchSet(0);
								doEnrich[j]->setEnable(true);
							}
						}
					}
				}
			}
			else // 没有投资过
			{
				unsigned char j = 0;
				for (unsigned char i = JXJCountryBenefitType_smallInvest; i <= JXJCountryBenefitType_largeInvest; ++ i,++ j)
				{
					doEnrich_sw[j]->setActiveSwitchSet(0);
					doEnrich[j]->setEnable(true);
				}
			}
		}
		else
		{
			titleTextId = 3056;
			contentTextId = 3057;
			if (enrich_sw)
				enrich_sw->setActiveSwitchSet(0);
			PermissionCheck  permissionCheck;
			if (permissionCheck(officalPos,JXJCountry_Fete_Business	))
			{
				enrich->setVisiable(true);
			}
			else
			{
				enrich->setVisiable(false);
			}
		}
		if(title[1].valid())
		{
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&titleTextId);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			title[1]->setString(notice);
		}
		if(text[1].valid())
		{
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&contentTextId);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			text[1]->setString(notice);
		}
	} while (0);
}

/************************************************************************/
/*    crJXJStartCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJStartCountryBenefitMethod::crJXJStartCountryBenefitMethod():
	m_this(NULL)
{
}

JXJ::crJXJStartCountryBenefitMethod::crJXJStartCountryBenefitMethod( const crJXJStartCountryBenefitMethod & handle ):
	crMethod(handle),
	m_type(handle.m_type)
{

}

void JXJ::crJXJStartCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJStartCountryBenefitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char )atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJStartCountryBenefitMethod::operator()( crHandle &handle )
{
	do 
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (!m_this || !netConductor)  break;
		CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(m_type);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvStartCountryBenefit,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	} while (0);
}

/************************************************************************/
/*    crJXJRecvStartCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvStartCountryBenefitMethod::crJXJRecvStartCountryBenefitMethod():
	m_this(NULL),
	m_netType(GameClient_Game),
	m_stream(NULL)
{
}

JXJ::crJXJRecvStartCountryBenefitMethod::crJXJRecvStartCountryBenefitMethod( const crJXJRecvStartCountryBenefitMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJRecvStartCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvStartCountryBenefitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvStartCountryBenefitMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			std::string name = m_this->getCharacterName();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				unsigned char benefitType = m_stream->_readUChar();
				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;

				unsigned char retVal = JXJStartCountryBenefit_failed_unkwon;

				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crJXJShiliData * shiliData = dynamic_cast<crJXJShiliData * > (shili->getDataClass());
					if (!shiliData) break;

					shiliData->excHandle(MAKEINT64(WCH_LockData,1));
					PermissionCheck check;
					if ( ! check(shiliData->getOfficialDes(playerid),JXJCountry_Fete_Business) )
					{
						retVal = JXJStartCountryBenefit_failed_noPermission;
						shiliData->excHandle(MAKEINT64(WCH_LockData,0));
						break;
					}
					shiliData->excHandle(MAKEINT64(WCH_LockData,0));

					ref_ptr<crTableIO>  countryBenefitTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBenefitTable);
					if (false == countryBenefitTab.valid()) break;
					crTableIO::StrVec record;

					int startCostIdx = countryBenefitTab->getTitleIndex("开启消耗元宝");
					switch (benefitType)
					{
					case JXJCountryBenefitType_fete			:
						{
							shiliData->getParam(WCHDATA_JXJCountryStartedFete,param);
							bool startedFete = *(bool *)param;
							if (startedFete)
							{
								retVal = JXJStartCountryBenefit_failed_hadStarted;
							}
							else
							{
								if (countryBenefitTab->queryOneRecord(
									0,crArgumentParser::appItoa(benefitType),
									1,crArgumentParser::appItoa(viplv),
									record)>=0)
								{
									int yuanBao = atoi(record[startCostIdx].c_str());
									bool needdeductgold = false;
									MoneyChangeData moneydata(yuanBao,"开启国家福利1");
									m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
									if (moneydata.first == 0)
									{
										startedFete = true;
										shiliData->inputParam(WCHDATA_JXJCountryStartedFete,&startedFete);
										bool modify = true;
										shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
										retVal = JXJStartCountryBenefit_suc;

										//CRCore::ScopedLock<crData> lock(*shiliData);
										shiliData->excHandle(MAKEINT64(WCH_LockData,1));
										shiliData->getParam(WCHDATA_JXJCountryHistory,param);
										CountryHistory *history = (CountryHistory *)param;

										CRCore::ref_ptr<CountryHistoryBenefitChange > chocPtr = new CountryHistoryBenefitChange ;
										chocPtr->m_infoId = 3069;
										chocPtr->m_playerName = name;
										chocPtr->m_officalName = shiliData->getOfficialStr(playerid);
										history->push_back(chocPtr.get());
										shiliData->excHandle(MAKEINT64(WCH_LockData,0));

										ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
										crTableIO::StrVec textrecord;
										char szNotice[256];
										int textid = 4077;
										if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
										{
											sprintf_s(szNotice,textrecord[1].c_str(),
												shiliData->getOfficialStr(playerid).c_str(),
												name.c_str());
											CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
												szNotice,shiliid,shiliid);
											crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
										}
									}
									else if (moneydata.first == -1)
										retVal = JXJStartCountryBenefit_failed_noYuanBao;
								}
							}
						}
						break;
					case JXJCountryBenefitType_invest:
						{
							shiliData->getParam(WCHDATA_JXJCountryStartedEnrichCountry,param);
							bool startedEnrich = *(bool *)param;
							if (startedEnrich)
							{
								retVal = JXJStartCountryBenefit_failed_hadStarted;
							}
							else
							{
								if (countryBenefitTab->queryOneRecord(
									0,crArgumentParser::appItoa(benefitType),
									1,crArgumentParser::appItoa(viplv),
									record)>=0)
								{
									int yuanBao = atoi(record[startCostIdx].c_str());
									bool needdeductgold = false;
									MoneyChangeData moneydata(yuanBao,"开启国家福利2");
									m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
									if (moneydata.first == 0)
									{
										startedEnrich = true;
										shiliData->inputParam(WCHDATA_JXJCountryStartedEnrichCountry,&startedEnrich);
										retVal = JXJStartCountryBenefit_suc;
										bool modify = true;
										shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);

										//CRCore::ScopedLock<crData> lock(*shiliData);
										shiliData->excHandle(MAKEINT64(WCH_LockData,1));
										shiliData->getParam(WCHDATA_JXJCountryHistory,param);
										CountryHistory *history = (CountryHistory *)param;

										CRCore::ref_ptr<CountryHistoryBenefitChange > chocPtr = new CountryHistoryBenefitChange ;
										chocPtr->m_infoId = 3070;
										chocPtr->m_playerName = name;
										chocPtr->m_officalName = shiliData->getOfficialStr(playerid);
										history->push_back(chocPtr.get());
										shiliData->excHandle(MAKEINT64(WCH_LockData,0));

										ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
										crTableIO::StrVec textrecord;
										char szNotice[256];
										int textid = 4078;
										if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
										{
											sprintf_s(szNotice,textrecord[1].c_str(),
												shiliData->getOfficialStr(playerid).c_str(),
												name.c_str());
											CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
												szNotice,shiliid,shiliid);
											crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
										}
									}
									else if (moneydata.first == -1)
										retVal = JXJStartCountryBenefit_failed_noYuanBao;
								}
							}
						}
						break;
					default:
						break;
					}
				} while (0);

				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				CRCore::ref_ptr<crStreamBuf > stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUChar(benefitType);
				stream->_writeUChar(retVal);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvStartCountryBenefit,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				crData * canvasData = canvas->getDataClass();

				unsigned char benefitType = m_stream->_readUChar();
				unsigned char retVal = m_stream->_readUChar();
				switch (retVal)
				{
				case JXJStartCountryBenefit_failed_noPermission:
					{
						int msgid = 3062;
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
					}
					break;
				case JXJStartCountryBenefit_failed_noYuanBao:
					{
						int msgid = 3065;
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
					}
				case JXJStartCountryBenefit_failed_hadStarted:
				case JXJStartCountryBenefit_suc:
					{
						bool tmpVal = true;
						switch (benefitType)
						{
						case JXJCountryBenefitType_fete	:
							canvasData->inputParam(WCHDATA_JXJCountryStartedFete,&tmpVal);
							break;
						case JXJCountryBenefitType_invest:
							canvasData->inputParam(WCHDATA_JXJCountryStartedEnrichCountry,&tmpVal);
							break;
						default:
							break;
						}
						canvas->doEvent(WCH_UI_UpdateData);
					}
				default:
					break;
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJEnjoyCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJEnjoyCountryBenefitMethod::crJXJEnjoyCountryBenefitMethod():
	m_this(NULL)
{
}

JXJ::crJXJEnjoyCountryBenefitMethod::crJXJEnjoyCountryBenefitMethod( const crJXJEnjoyCountryBenefitMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJEnjoyCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJEnjoyCountryBenefitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char) atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJEnjoyCountryBenefitMethod::operator()( crHandle &handle )
{
	do 
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (!m_this || !netConductor)  break;
		CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(m_type);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvEnjoyCountryBenefit,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	} while (0);
}

/************************************************************************/
/*    crJXJRecvEnjoyCountryBenefitMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvEnjoyCountryBenefitMethod::crJXJRecvEnjoyCountryBenefitMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvEnjoyCountryBenefitMethod::crJXJRecvEnjoyCountryBenefitMethod( const crJXJRecvEnjoyCountryBenefitMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJRecvEnjoyCountryBenefitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvEnjoyCountryBenefitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}



void JXJ::crJXJRecvEnjoyCountryBenefitMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				unsigned char benefitType = m_stream->_readUChar();
				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;

				data->getParam(WCHDATA_JXJCountryBenefitFeteTimes	,param);
				unsigned char feteTimes = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJCountryBenefitEnrichType	,param);
				unsigned char enrichType = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJCountryBenefitEnrichTimes,param);
				unsigned char enrichTimes = *(unsigned char *)param;

				unsigned char retVal = JXJEnjoyCountryBenefit_failed_unkwon;

				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crJXJShiliData * shiliData = dynamic_cast<crJXJShiliData * > (shili->getDataClass());
					if (!shiliData) break;

					ref_ptr<crTableIO>  countryBenefitTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCountryBenefitTable);
					if (false == countryBenefitTab.valid()) break;
					crTableIO::StrVec record;

					int timesLimitIdx = countryBenefitTab->getTitleIndex("每日次数");
					int enjoyCostIdx = countryBenefitTab->getTitleIndex("收益消耗元宝");
					int woodIdx = countryBenefitTab->getTitleIndex("木材");
					int feIdx = countryBenefitTab->getTitleIndex("铁矿");
					int foodIdx = countryBenefitTab->getTitleIndex("粮食");
					int horseIdx = countryBenefitTab->getTitleIndex("马匹");
					int probabilityIdx[3];
					int yuanBaoIdx[3];
					probabilityIdx[0] = countryBenefitTab->getTitleIndex("概率1");
					probabilityIdx[1] = countryBenefitTab->getTitleIndex("概率2");
					probabilityIdx[2] = countryBenefitTab->getTitleIndex("概率3");
					yuanBaoIdx[0] = countryBenefitTab->getTitleIndex("礼金1");
					yuanBaoIdx[1] = countryBenefitTab->getTitleIndex("礼金2");
					yuanBaoIdx[2] = countryBenefitTab->getTitleIndex("礼金3");
					
					switch (benefitType)
					{
					case JXJCountryBenefitType_fete			:
						{
							shiliData->getParam(WCHDATA_JXJCountryStartedFete,param);
							bool startedFete = *(bool *)param;
							if (!startedFete)
							{
								retVal = JXJEnjoyCountryBenefit_failed_noStarted;
							}
							else
							{
								if (countryBenefitTab->queryOneRecord(
									0,crArgumentParser::appItoa(benefitType),
									1,crArgumentParser::appItoa(viplv),
									record)>=0)
								{
									do 
									{
										if (feteTimes > atoi(record[timesLimitIdx].c_str()))
										{
											retVal = JXJEnjoyCountryBenefit_failed_timesLimit;
											break;
										}
										int yuanBao = atoi(record[enjoyCostIdx].c_str());
										bool needdeductgold = false;
										MoneyChangeData moneydata(yuanBao,"享受国家福利1");
										m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
										if (moneydata.first == 0)
										{
											retVal = JXJEnjoyCountryBenefit_suc;
											++ feteTimes;
											data->inputParam(WCHDATA_JXJCountryBenefitFeteTimes	,&feteTimes);

											ResourceChangeData foodData(T_Food, -atoi(record[foodIdx].c_str()));
											ResourceChangeData woodData(T_Wood,-atoi(record[woodIdx].c_str()));
											ResourceChangeData ironData(T_Iron,-atoi(record[feIdx].c_str()));
											ResourceChangeData horseData(T_Horse,-atoi(record[horseIdx].c_str()));
											m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&foodData,NULL));
											m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&woodData,NULL));
											m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&ironData,NULL));
											m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&horseData,NULL));

										}
										else if (moneydata.first == -1)
											retVal = JXJEnjoyCountryBenefit_failed_noYuanBao;
									} while (0);
								}
							}
						}
						break;
					case JXJCountryBenefitType_smallInvest	:
					case JXJCountryBenefitType_middleInvest	:
					case JXJCountryBenefitType_largeInvest	:
						{
							shiliData->getParam(WCHDATA_JXJCountryStartedEnrichCountry,param);
							bool startedEnrich = *(bool *)param;
							if (!startedEnrich)
							{
								retVal = JXJEnjoyCountryBenefit_failed_noStarted;
							}
							else
							{
								if (countryBenefitTab->queryOneRecord(
									0,crArgumentParser::appItoa(benefitType),
									1,crArgumentParser::appItoa(viplv),
									record)>=0)
								{
									do 
									{
										if( enrichType && enrichType != benefitType)
										{
											retVal = JXJEnjoyCountryBenefit_failed_diffType;
											break;
										}
										if (enrichTimes > atoi(record[timesLimitIdx].c_str()))
										{
											retVal = JXJEnjoyCountryBenefit_failed_timesLimit;
											break;
										}

										int yuanBao = atoi(record[enjoyCostIdx].c_str());
										bool needdeductgold = false;
										MoneyChangeData moneydata(yuanBao,"享受国家福利2");
										m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
										if (moneydata.first == 0)
										{
											CRCore::rangei r(0,100);
											char selectid = r.get_random();
											unsigned char idx = 0;
											std::vector<int> rangeVec;
											for (; idx < 3; ++ idx)
											{
												crArgumentParser::appAtoVec(record[probabilityIdx[idx]],rangeVec);
												if (selectid <= rangeVec[0] && selectid > rangeVec[1]) //当selectid取值为0时，取不到对应的礼金类型
												{
													break;
												}
												if (selectid == 0)
												{
													idx = 2;
													break;
												}
											}
											if (idx < 3)
											{
												ResourceChangeData giftGoldData(T_Giftgold, -atoi(record[yuanBaoIdx[idx]].c_str()));
												m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&giftGoldData,NULL));

												//int yuanBao = -atoi(record[yuanBaoIdx[idx]].c_str());
												//bool needdeductgold = false;
												//MoneyChangeData moneydata(yuanBao,"享受国家福利2");
												//m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
												retVal = JXJEnjoyCountryBenefit_suc;
												++ enrichTimes;
												data->inputParam(WCHDATA_JXJCountryBenefitEnrichTimes	,&enrichTimes);
												data->inputParam(WCHDATA_JXJCountryBenefitEnrichType,&benefitType);
											}
										}
										else if (moneydata.first == -1)
											retVal = JXJEnjoyCountryBenefit_failed_noYuanBao;
									} while (0);
								}
							}
						}
						break;
					default:
						break;
					}
				} while (0);

				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				CRCore::ref_ptr<crStreamBuf > stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUChar(benefitType);
				stream->_writeUChar(retVal);
				stream->_writeUChar(feteTimes);
				stream->_writeUChar(enrichTimes);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvEnjoyCountryBenefit,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				crData * canvasData = canvas->getDataClass();

				unsigned char benefitType = m_stream->_readUChar();
				unsigned char retVal = m_stream->_readUChar();
				unsigned char feteTimes = m_stream->_readUChar();
				unsigned char enrichTimes = m_stream->_readUChar();

				int msgid = 0;
				switch (retVal)
				{
				case JXJEnjoyCountryBenefit_failed_timesLimit:
					{
						msgid = 3063;
						canvasData->inputParam(WCHDATA_JXJCountryBenefitFeteTimes,&feteTimes);
						canvasData->inputParam(WCHDATA_JXJCountryBenefitEnrichTimes,&enrichTimes);
						canvas->doEvent(WCH_UI_UpdateData);
					}
					break;
				case JXJEnjoyCountryBenefit_failed_noStarted:
					msgid = 3064;
					break;
				case JXJEnjoyCountryBenefit_failed_noYuanBao:
					msgid = 3065;
					break;
				case JXJEnjoyCountryBenefit_failed_diffType:
					msgid = 3066;
					break;
				case JXJEnjoyCountryBenefit_suc:
					{
						msgid = 3067;
						if (benefitType == JXJCountryBenefitType_smallInvest
							|| benefitType == JXJCountryBenefitType_middleInvest
							|| benefitType == JXJCountryBenefitType_largeInvest	)
						{
							canvasData->inputParam(WCHDATA_JXJCountryBenefitEnrichType,&benefitType);
							canvasData->inputParam(WCHDATA_JXJCountryBenefitFeteTimes,&feteTimes);
							canvasData->inputParam(WCHDATA_JXJCountryBenefitEnrichTimes,&enrichTimes);
							canvas->doEvent(WCH_UI_UpdateData);
						}
					}
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
		}
	}
}

/************************************************************************/
/*    crJXJCountryCanvasDataInitMethod                                                                  */
/************************************************************************/

JXJ::crJXJCountryCanvasDataInitMethod::crJXJCountryCanvasDataInitMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryCanvasDataInitMethod::crJXJCountryCanvasDataInitMethod( const crJXJCountryCanvasDataInitMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJCountryCanvasDataInitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCountryCanvasDataInitMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCountryCanvasDataInitMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		crData * canvasData = m_this->getDataClass();
		if (canvasData)
		{
			int tmpVal = 0;
			canvasData->inputParam(WCHDATA_JXJCountryCanvasSelectedCantonId,&tmpVal);
			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMinLv,&tmpVal);
			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMaxLv,&tmpVal);

			tmpVal = 1;
			canvasData->inputParam(WCHDATA_JXJCountryCanvasFenFengCurPage,&tmpVal);
			canvasData->inputParam(WCHDATA_JXJCountryCanvasFenFengTotalPage,&tmpVal);
			canvasData->inputParam(WCHDATA_JXJCountryPeopleListCurPage,&tmpVal);
			canvasData->inputParam(WCHDATA_JXJCountryPeopleListTotalPage,&tmpVal);
		}
	}
}

/************************************************************************/
/*    crJXJQueryCountryPeopleMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryCountryPeopleMethod::crJXJQueryCountryPeopleMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryCountryPeopleMethod::crJXJQueryCountryPeopleMethod( const crJXJQueryCountryPeopleMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_edit(handle.m_edit),
	m_table(handle.m_table)
{
	for (unsigned i = 0; i < 9; ++ i)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}

void JXJ::crJXJQueryCountryPeopleMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryCountryPeopleMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_edit = str;
		break;
	case 2:
		m_table = str;
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
		m_checkbox[i - 3] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJQueryCountryPeopleMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		ref_ptr<crEditWidgetNode> nameEdit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_edit));
		ref_ptr<crTableWidgetNode>  tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_table));

		//ref_ptr<crCheckBoxWidgetNode>  checkbox[9] = {NULL};
		//for (unsigned char i = 0; i < 9; ++ i)
		//{
		//	checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
		//}
		if (canvas.valid() && canvas->getDataClass() && nameEdit.valid())
		{
			crData * canvasData = canvas->getDataClass();
			void *param;
			std::string name = nameEdit->getUTF8String();
			
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
			PeopleVec & peopleVec = *(PeopleVec *)param;
			int vecSize = peopleVec.size();
			int i;
			for (i = 0;i < vecSize; ++ i)
			{
				if (peopleVec[i]->getName() == name)
				{
					peopleVec[0] = peopleVec[i];
					peopleVec.resize(1);
					break;
				}
			}

			unsigned char lv = 0;
			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMinLv	,&lv);
			canvasData->inputParam(WCHDATA_JXJCountryQueryPeopleListMaxLv	,&lv);

			canvasData->excHandle(MAKEINT64(WCH_LockData,0));

			if (i == vecSize)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if (netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(16);
					stream->_writeString(name);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryCountryPeople,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
			else
			{
				int curPage = 1;
				canvasData->inputParam(WCHDATA_JXJCountryPeopleListCurPage,&curPage);
				canvasData->inputParam(WCHDATA_JXJCountryPeopleListTotalPage,&curPage);
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvQueryCountryPeopleMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryCountryPeopleMethod::crJXJRecvQueryCountryPeopleMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryCountryPeopleMethod::crJXJRecvQueryCountryPeopleMethod( const crJXJRecvQueryCountryPeopleMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_table(handle.m_table)
{
	for (unsigned i = 0; i < 9; ++ i)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}

void JXJ::crJXJRecvQueryCountryPeopleMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryCountryPeopleMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_table = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		m_checkbox[i - 2] = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJRecvQueryCountryPeopleMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() )
			{
				std::string name = m_stream->_readString();

				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				bool retVal = false;
				
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));
				ref_ptr<crStreamBuf> stream = new  crStreamBuf;
				do 
				{
					serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData * shiliData = shili->getDataClass();
					if (!shiliData) break;
					shiliData->excHandle(MAKEINT64(WCH_LockData,1));

					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap * peopleMap = (PeopleMap *)param;
					if (peopleMap->empty())
					{
						shiliData->excHandle(MAKEINT64(WCH_LockData,0));
						break;
					}
					PeopleMap::iterator pmItr = peopleMap->begin();
					for (; pmItr != peopleMap->end(); ++ pmItr)
					{
						if (pmItr->second.valid())
						{
							if (pmItr->second->getName() == name)
							{
								retVal = true;
								stream->createBuf(36);
								stream->_writeBool(retVal);
								stream->_writeBool(pmItr->second->getSmallAcc());
								stream->_writeUChar(pmItr->second->getLv());
								stream->_writeUChar(pmItr->second->getTroopsLv());
								stream->_writeInt(pmItr->second->getID());
								stream->_writeString(pmItr->second->getName());
								stream->_writeString(pmItr->second->getConsortiaName());
								break;
							}
						}
					}
					shiliData->excHandle(MAKEINT64(WCH_LockData,0));
				} while (0);
				if (!retVal)
				{
					stream->createBuf(1);
					stream->_writeBool(retVal);
				}
				serverData->excHandle(MAKEINT64(WCH_LockData,0));

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryCountryPeople,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			bool retVal = m_stream->_readBool();
			if (retVal)
			{
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				ref_ptr<crTableWidgetNode>  tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_table));

				//ref_ptr<crCheckBoxWidgetNode>  checkbox[9] = {NULL};
				//for (unsigned char i = 0; i < 9; ++ i)
				//{
				//	checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkbox[i]));
				//}
				if (canvas.valid() && canvas->getDataClass())
				{
					crData * canvasData = canvas->getDataClass();
					void *param;
					canvasData->excHandle(MAKEINT64(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJShiliDataTempPeopleVec,param);
					PeopleVec & peopleVec = *(PeopleVec *)param;
					peopleVec.resize(1);

					peopleVec[0] = new PeopleInfo;

					peopleVec[0]->setSamllAcc( m_stream->_readBool());
					peopleVec[0]->setLv( m_stream->_readUChar());
					peopleVec[0]->setTroopsLv( m_stream->_readUChar());
					peopleVec[0]->setID( m_stream->_readInt());
					peopleVec[0]->setName( m_stream->_readString());
					peopleVec[0]->setConsortiaName( m_stream->_readString());

					canvasData->excHandle(MAKEINT64(WCH_LockData,0));

					int curPage = 1;
					canvasData->inputParam(WCHDATA_JXJCountryPeopleListCurPage,&curPage);
					canvasData->inputParam(WCHDATA_JXJCountryPeopleListTotalPage,&curPage);

					canvas->doEvent(WCH_UI_UpdateData);
				}
			}
			else
			{
				int msgid = 3075;
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
		}
	}
}

/************************************************************************/
/*    crJXJGMOrderCountryVoteMethod                                                                  */
/************************************************************************/

JXJ::crJXJGMOrderCountryVoteMethod::crJXJGMOrderCountryVoteMethod():
	m_this(NULL),
	m_gmData(NULL)
{
}

JXJ::crJXJGMOrderCountryVoteMethod::crJXJGMOrderCountryVoteMethod( const crJXJGMOrderCountryVoteMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJGMOrderCountryVoteMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_gmData = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		{
			if(param)
			{
				_crInt64 param64 = *(_crInt64*)param;
				m_gmData = (crJXJCountryVoteGMData *)(LOINT64(param64));
			}
			else
			{
				m_gmData = NULL;
			}
		}
		break;
	}
}

void JXJ::crJXJGMOrderCountryVoteMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJGMOrderCountryVoteMethod::operator()( crHandle &handle )
{
	if (m_gmData)
	{
		int playerid = m_gmData->m_this->getPlayerID();
		void* param;
		crData *data = m_gmData->m_this->getDataClass();

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (data && playerData.valid() && gameServer)
		{
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *( unsigned char *)param;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			if (gsBrainData)
			{
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
				crJXJShili *shili = (crJXJShili *)param;
				bool retVal = false;
				do 
				{
					if (!shili || shili->getDataClass() == NULL) break;
					crData *shilidata = shili->getDataClass();
					shilidata->excHandle(MAKEINT64(WCH_LockData,1));
					shilidata->getParam(WCHDATA_JXJCountryVotersSet,param);
					VotersSet * votersSet = (VotersSet *)param;
					VotersSet::iterator vsItr = votersSet->find(playerid);
					if (vsItr == votersSet->end())
					{	
						shilidata->excHandle(MAKEINT64(WCH_LockData,0));
						break;
					}
					shilidata->getParam(WCHDATA_JXJCountryCandidateVecT,param);
					CandidateVec*candidateVec = (CandidateVec*)param;
					CandidateVec::iterator tdItr;

					for (tdItr = candidateVec->begin(); tdItr != candidateVec->end(); ++ tdItr)
					{
						if (tdItr->valid())
						{
							if (m_gmData->m_name  == (*tdItr)->playerName)
							{
								(*tdItr)->nNumber += m_gmData->m_voteNum;
								retVal = true;
								break;
							}
						}
					}
					shilidata->excHandle(MAKEINT64(WCH_LockData,0));
				} while (0);
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(retVal);

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSelectCountryLeader,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
}

/************************************************************************/
/*    crJXJCountryBenefitRefreshMethod                                                                  */
/************************************************************************/
JXJ::crJXJCountryBenefitRefreshMethod::crJXJCountryBenefitRefreshMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryBenefitRefreshMethod::crJXJCountryBenefitRefreshMethod( const crJXJCountryBenefitRefreshMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJCountryBenefitRefreshMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCountryBenefitRefreshMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCountryBenefitRefreshMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		crData *gsBrainData = m_this->getDataClass();
		if (!gsBrainData) break;

		time_t nowTime = time(NULL);
		struct tm sTm = *localtime(&nowTime);

		bool willZero = false;
		bool started = false;
		bool modify = true;
		crJXJShili * shili;
		crData * shiliData;
		void *param;
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		for (unsigned char i = 0; i < 3; ++ i)
		{
			gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
			shili = (crJXJShili *)param;
			shiliData = shili->getDataClass();
			willZero = false;

			shiliData->getParam(WCHDATA_JXJShiliDayLastUpdate,param);
			time_t shiliDayUpdate = *(time_t *)param;

			if (nowTime - shiliDayUpdate >= 86400)
			{
				willZero = true;
			}
			else if (sTm.tm_hour == 0 && sTm.tm_min == 0 && nowTime - shiliDayUpdate > 60)
			{
				willZero = true;
			}

			if (willZero)
			{
				shiliData->inputParam(WCHDATA_JXJShiliDayLastUpdate,&nowTime);

				shiliData->inputParam(WCHDATA_JXJCountryStartedFete,&started);
				shiliData->inputParam(WCHDATA_JXJCountryStartedEnrichCountry,&started);
				shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
			}
		}
		gsBrainData->inputParam(WCHDATA_JXJShiliDayLastUpdate,&nowTime);
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	} while (0);
}

/************************************************************************/
/*    crJXJCountryCantonMapRefreshMethod                                                                  */
/************************************************************************/
JXJ::crJXJCountryCantonMapRefreshMethod::crJXJCountryCantonMapRefreshMethod():
	m_this(NULL)
{
}

JXJ::crJXJCountryCantonMapRefreshMethod::crJXJCountryCantonMapRefreshMethod( const crJXJCountryCantonMapRefreshMethod& handle ):
	crMethod(handle),
	m_shiliId(handle.m_shiliId),
	m_chengchiId(handle.m_chengchiId)
{
}

void JXJ::crJXJCountryCantonMapRefreshMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_chengchiId = LOINT64(param64);
			m_shiliId = (unsigned char )HIINT64(param64);
		}
		else
		{
			m_chengchiId = 0;
			m_shiliId = 0;
		}
		break;
	};
}

void JXJ::crJXJCountryCantonMapRefreshMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCountryCantonMapRefreshMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass() && m_chengchiId && m_shiliId)
	{
		crData * serverData = m_this->getDataClass();
		void *param;
		serverData->excHandle(MAKEINT64(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchiMap = (ChengchiMap *)param;

		serverData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = (ConsortiaMap *)param;
		
		crJXJShili * shili[3] = {NULL};
		crData *shiliData[3] = {NULL};

		char idx = 0;
		for (int shiliId = WCHDATA_JXJShiliWei; shiliId <= WCHDATA_JXJShiliWu	; ++ shiliId, ++ idx)
		{
			serverData->getParam(shiliId,param);
			shili[idx] = (crJXJShili *)param;
			shiliData[idx] = shili[idx]->getDataClass();
		}

		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crTableIO::StrVec record;
		int cantonIdx = 0;
		int cantonId = 0;
		if (chengchiTab.valid()) cantonIdx = chengchiTab->getTitleIndex("州ID");
		// 1.得到该城池的州ID
		if (chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(m_chengchiId),record) >= 0)
		{
			cantonId = atoi(record[cantonIdx].c_str());
		}

		CantonMap * cantonMap = NULL;
		CantonMap::iterator cmItr;
		ConsortiaMap::iterator cMItr;

		idx = m_shiliId - c_startShiliID;
		unsigned char beginId = 0,endId = 2;
		if (idx >= 0)
		{
			beginId = 1;
			//1.1 得到该城池，即有了州的归属权

			shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,1));
			shiliData[idx]->getParam(WCHDATA_JXJShiliCantonMap,param);
			cantonMap = (CantonMap *)param;
			cmItr = cantonMap->find(cantonId);
			// 该势力拥有该州
			if (cmItr == cantonMap->end())
			{
				ref_ptr<Canton> cantonPtr = new Canton;
				cantonPtr->setCantonId(cantonId);
				cantonMap->insert(CantonMap::value_type(cantonId,cantonPtr));
			}
			shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else idx = 0;

		// 2.如果该城池分给了其他势力的军团，则撤销它的分封
		for (unsigned char i = beginId; i <= endId; ++ i)
		{
			idx = (idx + 1) % 3;
			shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,1));
			shiliData[idx]->getParam(WCHDATA_JXJShiliCantonMap,param);
			cantonMap = (CantonMap *)param;
			cmItr = cantonMap->find(cantonId);
			// 该势力拥有该州
			if (cmItr != cantonMap->end())
			{
				ChengChiEarningsMap & chengChiEarningsMap = cmItr->second->getChengChiEarningsMap();
				ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.begin();

				for (; ccemItr != chengChiEarningsMap.end(); ++ ccemItr)
				{
					if (ccemItr->second.valid() 
						&& ccemItr->second->getChengChiId() == m_chengchiId) 
					{
						ccemItr->second->setChengChiId(0);
						cMItr = consortiamap->find(ccemItr->first);
						if (cMItr != consortiamap->end() && cMItr->second.valid())
						{
							crData * groupData = cMItr->second->getDataClass();
							groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
							FenFengedChengChiMap * fenFengedChengChiMap = (FenFengedChengChiMap *)param;
							FenFengedChengChiMap::iterator ffccmItr = fenFengedChengChiMap->find(m_chengchiId);
							if (ffccmItr != fenFengedChengChiMap->end())
							{
								fenFengedChengChiMap->erase(ffccmItr);
							}
						}
					}
				}
			}
			shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,0));
		}
		// 3.城池归属变化，可能会导致州归属变化，检查州归属变化
		bool ownChengchi[3] = {false,false,false};
		// 3.1 得到该州的所有城池

		crTableIO::DataVec dataVec;
		int chengchiIdx = chengchiTab->getTitleIndex("id");
		chengchiTab->queryRecords(cantonIdx,crArgumentParser::appItoa(cantonId),dataVec);

		ChengchiMap::iterator ccmItr ;
		// 3.2 获得所有城池的归属
		crTableIO::DataVec::iterator dvItr = dataVec.begin();
		for (; dvItr != dataVec.end(); ++ dvItr)
		{
			ccmItr = chengchiMap->find(atoi((*dvItr)[chengchiIdx].c_str()));
			if (ccmItr != chengchiMap->end())
			{
				if (ccmItr->second->getChengzhuShili() >= c_startShiliID)
				{
					ownChengchi[ccmItr->second->getChengzhuShili() - c_startShiliID] = true;
				}
			}
		}
		for (idx = 0; idx < 3; ++ idx)
		{
			if (!ownChengchi[idx])
			{
				// 该势力可能丢失了州
				shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,1));

				shiliData[idx]->getParam(WCHDATA_JXJShiliCantonMap,param);
				cantonMap = (CantonMap *)param;
				cmItr = cantonMap->find(cantonId);
				// 该势力曾经拥有该州,取消分封
				if (cmItr != cantonMap->end())
				{
					// 维护容器里军团相关数据
					ChengChiEarningsMap & chengChiEarningsMap = cmItr->second->getChengChiEarningsMap();
					ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.begin();
					bool first_run = true;
					do
					{
						if (first_run)
						{
							cMItr = consortiamap->find(cmItr->second->getConsortiaId());
							first_run = false;
						}
						else
						{
							cMItr = consortiamap->find(ccemItr->first);
							++ ccemItr;
						}
						if (cMItr != consortiamap->end() && cMItr->second.valid())
						{
							crData * groupData = cMItr->second->getDataClass();
							groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
							FenFengedCantonMap * fenFengedCantonMap = (FenFengedCantonMap *)param;
							FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->find(cantonId);
							if (ffcmItr != fenFengedCantonMap->end())
							{
								fenFengedCantonMap->erase(ffcmItr);
							}
							groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
							ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
							ApplyCantonMap::iterator acmItr = applyCantonMap->find(cantonId);
							if (acmItr != applyCantonMap->end())
							{
								applyCantonMap->erase(acmItr);
							}
							groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
							FenFengedChengChiMap * fenFengedChengChiMap = (FenFengedChengChiMap *)param;
							FenFengedChengChiMap::iterator ffccmItr = fenFengedChengChiMap->find(m_chengchiId);
							if (ffccmItr != fenFengedChengChiMap->end())
							{
								fenFengedChengChiMap->erase(ffccmItr);
							}
						}
					} while (ccemItr != chengChiEarningsMap.end());
					cantonMap->erase(cmItr);
				}
				shiliData[idx]->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		serverData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
JXJ::crJXJSYNChengChiToCantonMethod::crJXJSYNChengChiToCantonMethod():
	m_this(NULL)
{
}

JXJ::crJXJSYNChengChiToCantonMethod::crJXJSYNChengChiToCantonMethod( const crJXJSYNChengChiToCantonMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJSYNChengChiToCantonMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSYNChengChiToCantonMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJSYNChengChiToCantonMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		crData *gsBrainData = m_this->getDataClass();
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;

		for (ChengchiMap::iterator itr = chengchimap->begin();
			itr != chengchimap->end();
			++itr)
		{
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCountryCantonMapRefresh,MAKEINT64(itr->first,itr->second->getChengzhuShili()));
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/************************************************************************/
/*    crJXJSmallAccCheckBoxUpdateMethod                                                                  */
/************************************************************************/
JXJ::crJXJSmallAccCheckBoxUpdateMethod::crJXJSmallAccCheckBoxUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJSmallAccCheckBoxUpdateMethod::crJXJSmallAccCheckBoxUpdateMethod( const crJXJSmallAccCheckBoxUpdateMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_checkBox(handle.m_checkBox)
{

}

void JXJ::crJXJSmallAccCheckBoxUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSmallAccCheckBoxUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_checkBox = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJSmallAccCheckBoxUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && canvas->getDataClass())
		{
			ref_ptr<crCheckBoxWidgetNode>  checkbox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkBox));
			crData * canvasData = canvas->getDataClass();
			void *param;
			canvasData->getParam(WCHDATA_JXJCountryCanvasOfficalPos,param);
			unsigned char officalPos = *(unsigned char *)param;
			PermissionCheck  permissionCheck;
			if (!permissionCheck(officalPos,JXJCountry_Protect))
			{
				if (checkbox.valid())
				{
					checkbox->select(!checkbox->getSelect());
				}
			}
		}
	}
}

JXJ::crJXJStaticTextIsNumberCheckMethod::crJXJStaticTextIsNumberCheckMethod():
	m_this(NULL)
{
}

JXJ::crJXJStaticTextIsNumberCheckMethod::crJXJStaticTextIsNumberCheckMethod( const crJXJStaticTextIsNumberCheckMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_staticText(handle.m_staticText)
{

}

void JXJ::crJXJStaticTextIsNumberCheckMethod::inputParam( int i, void *param )
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

void JXJ::crJXJStaticTextIsNumberCheckMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_staticText = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJStaticTextIsNumberCheckMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> text = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_staticText));
			std::string numStr = text->getUTF8String();

			bool illegal = false;
			unsigned char j = 0;
			for (std::string::iterator i = numStr.begin(); i != numStr.end(); ++ j)
			{
				if (*i - '0' > 9 || *i - '0' < 0 || j >= 2)
				{
					i = numStr.erase(i);
					if (!illegal) illegal = true;
				}
				else ++ i;
			}
			if (illegal)
			{
				text->setString(numStr);
			}
		}
	}
}

///************************************************************************/
///*    crJXJUICountryShopItemTipsMethod                                                                  */
///************************************************************************/
//JXJ::crJXJUICountryShopItemTipsMethod::crJXJUICountryShopItemTipsMethod():
//	m_this(NULL)
//{
//}
//
//JXJ::crJXJUICountryShopItemTipsMethod::crJXJUICountryShopItemTipsMethod( const crJXJUICountryShopItemTipsMethod & handle ):
//	crMethod(handle),
//	m_tips (handle.m_tips),
//	m_name (handle.m_name),
//	m_info2 (handle.m_info2),
//	m_itemlv(handle.m_itemlv),
//	m_itemicon(handle.m_itemicon),
//	m_itemequipon(handle.m_itemequipon),
//	m_infocanvas(handle.m_infocanvas),
//	m_infobk(handle.m_infobk),
//	m_infodi(handle.m_infodi),
//	m_offsets (handle.m_offsets),
//	m_colorred (handle.m_colorred),
//	m_index (handle.m_index)
//{
//
//}
//
//void JXJ::crJXJUICountryShopItemTipsMethod::addParam( int i, const std::string& str )
//{
//	switch(i) 
//	{
//	case 0:
//		m_tips = str;
//		break;
//	case 1:
//		m_name = str;
//		break;
//	case 2:
//		m_info2 = str;
//		break;
//	case 3:
//		m_itemlv = str;
//		break;
//	case 4:
//		m_itemicon = str;
//		break;
//	case 5:
//		m_itemequipon = str;
//		break;
//	case 6:
//		m_infocanvas = str;
//		break;
//	case 7:
//		m_infobk = str;
//		break;
//	case 8:
//		m_infodi = str;
//		break;
//	case 9:
//		crArgumentParser::appAtoVec(str,m_offsets);
//		break;
//	case 10:
//		crArgumentParser::appAtoVec(str,m_colorred);//red
//		m_colorred /= 255.0f;
//		break;
//	case 11:
//		m_index = atoi(str.c_str());
//		break;
//	}
//}
//
//void JXJ::crJXJUICountryShopItemTipsMethod::inputParam( int i, void *param )
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
//
//void JXJ::crJXJUICountryShopItemTipsMethod::operator()( crHandle &handle )
//{
//	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
//	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(tipsCanvas.valid() && playerData && infocanvas.valid())
//	{
//		void *param;
//		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
//		crTableIO::StrVec record,lordrecord;
//		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
//		crStaticTextWidgetNode *itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
//		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
//		crStaticTextWidgetNode *itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
//		//
//		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
//		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
//		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
//		if (name)
//		{
//			name->clearString();
//		}
//		if (info2)
//		{
//			info2->clearText();
//		}
//		if (itemlv)
//		{
//			itemlv->clearString();
//		}
//		if (itemicon)
//		{
//			itemicon->clearImage();
//		}
//		if (itemequipon)
//		{
//			itemequipon->clearString();
//		}
//		crRole *mainRole = playerData->getMainRole();
//		crData *roleData = mainRole->getDataClass();
//		roleData->getParam(WCHDATA_Level,param);
//		unsigned char rolelv = *(unsigned char *)param;
//		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
//		//crData *data = crBrain::getInstance()->getDataClass();
//		crTableIO::DataVec recordVec;
//		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
//		int itemid = 0;
//		if (canvasdata)
//		{
//			canvasdata->getParam(WCHDATA_JXJGovPostShopItemDeq,param);
//			GovPostShopItemIDDeq *itemiddeq = (GovPostShopItemIDDeq *)param;
//			if (m_index < itemiddeq->size())
//			{
//				itemid  = (*itemiddeq)[m_index];
//			}
//		}
//		if (itemid > 0)
//		{
//			if (itemid >= 3001 && itemid < 3131)
//			{
//				crTableIO::StrVec record,lordrecord,recorde;
//				ref_ptr<crTableIO>lordequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
//				int equipidindex = lordequiptab->getTitleIndex("equipid");
//				int iconindex = itemtab->getTitleIndex("icon");
//				//int info2index = itemtab->getTitleIndex("物品描述");
//				int nameindex = itemtab->getTitleIndex("name");
//				int typeindex = itemtab->getTitleIndex("类型");
//				int colorindex = itemtab->getTitleIndex("品质");
//				int needlvindex = itemtab->getTitleIndex("人物等级");
//				if (lordequiptab->queryOneRecord(2,crArgumentParser::appItoa(itemid),recorde) >= 0)
//				{
//					int itemidindex = itemtab->getTitleIndex("itemid");
//					int equipid = atoi(recorde[equipidindex].c_str());
//					itemtab->queryOneRecord(itemidindex,crArgumentParser::appItoa(equipid),record);
//					int colormodeid = atoi(record[colorindex].c_str());
//					if(name)
//					{
//						name->setColor(crVector4(1,1,0,1));
//						name->setString(record[nameindex]);
//					}
//					std::string file;
//					if (itemlv)
//					{
//						if (rolelv < atoi(record[needlvindex].c_str()))
//						{
//							itemlv->setColor(m_colorred);
//							if (itemequipon)
//							{
//								itemequipon->setColor(m_colorred);
//							}
//						}
//						else
//						{
//							itemlv->setColor(crVector4(1,1,0,1));
//							if (itemequipon)
//							{
//								itemequipon->setColor(crVector4(1,1,1,1));
//							}
//						}
//						itemlv->setString(record[needlvindex]);
//					}
//					if (itemicon)
//					{
//						itemicon->setImageName(record[iconindex]);
//					}
//					//if (itemequipon)
//					//{
//					//	itemequipon->setString(std::string("未装备"));
//					//}
//					//if (itemequipon)
//					//{
//					//	std::string str = "未装备"; // = "未使用";
//					//	if (atoi(record[typeindex].c_str()) == IT_Equip)
//					//	{
//					//		if (m_index == 1)
//					//		{
//					//			str = "已装备";
//					//		}
//					//	}
//					//	itemequipon->setString(str);
//					//}
//					if(info2)
//					{
//						ItemMap itemMap;
//						rcfg::ConfigScript outscript(&itemMap);
//						crVector2i item(equipid,0);
//						playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
//						info2->setHypertext(outscript);
//					}
//				}
//			}
//			else
//			{
//				int iconindex = itemtab->getTitleIndex("icon");
//				int nameindex = itemtab->getTitleIndex("name");
//				//int typeindex = itemtab->getTitleIndex("类型");
//				//int colorindex = itemtab->getTitleIndex("品质");
//				int needlvindex = itemtab->getTitleIndex("人物等级");
//				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
//				{
//					//int colormodeid = atoi(record[colorindex].c_str());
//					if(name)
//					{
//						name->setColor(crVector4(1,1,0,1));
//						name->setString(record[nameindex]);
//					}
//					if (itemlv)
//					{
//						if (rolelv < atoi(record[needlvindex].c_str()))
//						{
//							itemlv->setColor(m_colorred);
//						}
//						else
//						{
//							itemlv->setColor(crVector4(1,1,1,1));
//						}
//						itemlv->setString(record[needlvindex]);
//					}
//					if (itemicon)
//					{
//						itemicon->setImageName(record[iconindex]);
//					}
//					//if (itemequipon)
//					//{
//					//	std::string str;// = "未使用";
//					//	if (atoi(record[typeindex].c_str()) == IT_Equip)
//					//	{
//					//		if (m_index == 1)
//					//		{
//					//			str = "已装备";
//					//		}
//					//		else
//					//		{
//					//			str = "未装备";
//					//		}
//					//	}
//					//	itemequipon->setString(str);
//					//}
//					if(info2)
//					{
//						ItemMap itemMap;
//						rcfg::ConfigScript outscript(&itemMap);
//						crVector2i item(itemid,0);
//						playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
//						info2->setHypertext(outscript);
//					}
//				}
//			}
//			/////////////////////////////////////////
//			float posx2,posy2;
//			float boxXlength = infobk->getChild(0)->getBoundBox().xLength();
//			///////////计算缩放
//			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
//			info2->updateData();
//			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
//			const crBoundingBox &bbox2 = info2->getBoundBox();
//			float texheight = 0.0f;
//			float needmove = 0.0f;
//			if(bbox.valid())
//			{
//				texheight = bbox.yLength();
//				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
//				if(texheight<boxYlength) texheight = boxYlength;
//				float scale = texheight/boxYlength;
//				float needmove ;
//				//const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
//				crVector3 centerTansTar = info2->getBound().center();
//
//				float tmpLength = bbox2.yLength();
//				if(infodi)
//				{
//					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
//					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
//					//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
//				}
//				else
//					centerTansTar[1] += 0.5f * (tmpLength - texheight);
//				// 源中心点
//				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
//				// 将源中心点平移到原点
//				crMatrix put_mat = crMatrix::translate(centerTansSrc);
//				crMatrix inv_put = crMatrix::inverse(put_mat);
//				crMatrix rotmat;
//				rotmat.postMult(inv_put);
//				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
//				rotmat.postMult(crMatrix::translate(centerTansTar));
//				if(infobk)infobk->setMatrix(rotmat);
//
//				if(infodi)
//				{
//					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
//					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
//					needmove = boxYlength - texheight + diYlength * 0.25;
//					//needmove = boxYlength - texheight;
//					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
//				}
//			}
//			else
//			{
//				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
//				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
//			}
//			/////////
//
//			/////////////////////////////////////////
//			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
//			const crBoundingBox &iconbox = m_this->getBoundBox();
//			const crBoundingBox &tip = tipsCanvas->getBoundBox();
//			float posx = (tip.m_max[0]-tip.m_min[0])*0.5;
//			float posy = (tip.m_max[1]-tip.m_min[1])*0.5;
//			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
//			pos = pos * mat;
//			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
//			pos2 = pos2 * mat;
//			/////////////////////////////////////////
//			if (pos2[0] >= 0)
//			{
//				boxXlength = -boxXlength;
//				posx2 = pos2[0] - posx;
//			}
//			else
//			{
//				posx2 = pos[0] + posx;
//			}
//			if (pos2[1] >= 0)
//			{
//				posy2 = pos[1] - posy;
//			}
//			else
//			{
//				posy2 = pos2[1] + posy - needmove;// + posinfoy;
//			}
//			crVector2 mouse(crVector2(posx2,posy2));
//
//			//float needmovex = 0;
//			//if (m_index == 1)
//			//{
//			//	needmovex += boxXlength;
//			//}
//			//tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f)/* * mat*/);
//			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
//			tipsCanvas->setCanFocus(false);
//			infocanvas->setCanFocus(false);
//			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
//			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
//			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
//		}
//		else
//		{
//			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
//			crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
//		}
//	}
//}


/**************************************************************************

METHOD: crJXJCheckCountryOfficialMethod
 
**************************************************************************/
JXJ::crJXJCheckCountryOfficialMethod::crJXJCheckCountryOfficialMethod():
	m_this(NULL)
{

}

JXJ::crJXJCheckCountryOfficialMethod::crJXJCheckCountryOfficialMethod( const crJXJCheckCountryOfficialMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJCheckCountryOfficialMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCheckCountryOfficialMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCheckCountryOfficialMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		crData *gsBrainData = m_this->getDataClass();
		if (!gsBrainData) break;

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		time_t nowTime = time(0);
		//bool modify = false;
		int shiliId[3] = {WCHDATA_JXJShiliWei,WCHDATA_JXJShiliShu,WCHDATA_JXJShiliWu};
		for (unsigned char i = 0; i < 3; ++ i)
		{
			//modify = false;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			void *param;
			gsBrainData->getParam(shiliId[i],param);
			crJXJShili * shili = (crJXJShili *)param;
			crData * shiliData = shili->getDataClass();
			shiliData->excHandle(MAKEINT64(WCH_LockData,1));

			//shiliData->getParam(WCHDATA_JXJChengxiang,param);
			//int chengxiangID = *(int *)param;
			//shiliData->getParam(WCHDATA_JXJDasima,param);
			//int dasimaID = *(int *)param;
			//shiliData->getParam(WCHDATA_JXJDajiangjun,param);
			//int dajiangjunID = *(int *)param;

			shiliData->getParam(WCHDATA_JXJCountryHistory,param);
			CountryHistory *history = (CountryHistory *)param;

			shiliData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
			CandidateVec*candidateVecL = (CandidateVec*)param;

			bool modify = false;
			for (CandidateVec::iterator cvLItr = candidateVecL->begin();
				cvLItr != candidateVecL->end();)
			{
				ref_ptr<crGameServerPlayerData> playerData 
					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData((*cvLItr)->nPlayerID));
				if (playerData.valid())
				{
					(*cvLItr)->m_lastLogin = nowTime;
					modify = true;
				}
				if ( (nowTime - (*cvLItr)->m_lastLogin) / 86400 >= 3 )
				{
					char log[128];
					memset(log,0,sizeof(log));
					sprintf(log,"%d %s 三天未上线 从国家官员候补列表中删除\0",(*cvLItr)->nPlayerID,(*cvLItr)->playerName.c_str());

					std::string logdata(log);
					GameLogData gamelog(Log_RemoveFromCandidateL,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64((*cvLItr)->nPlayerID,&gamelog));

					cvLItr = candidateVecL->erase(cvLItr);
					modify = true;
				}
				else
				{
					++ cvLItr;
				}
			}
			size_t candidateSize = candidateVecL->size();

			int id;
			std::string name;
			if (candidateSize >= 1)
			{
				int oldID;
				//std::string oldName;
				shiliData->getParam(WCHDATA_JXJChengxiang,param);
				oldID = *(int *)param;
				//shiliData->getParam(WCHDATA_JXJChengxiangName,param);
				//oldName = *(std::string *)param;

				id = (*candidateVecL)[0]->nPlayerID;
				name = (*candidateVecL)[0]->playerName;
				shiliData->inputParam(WCHDATA_JXJChengxiang,&id);
				shiliData->inputParam(WCHDATA_JXJChengxiangName,&name);
				//shiliData->inputParam(WCHDATA_JXJChengxiangLastLogin,&nowTime);
				//candidateVecL->erase(candidateVecL->begin());
				CRCore::ref_ptr<CountryHistoryOfficalChange > chocPtr;
				char log[128];
				std::string logdata;

				if (oldID != id)
				{
					chocPtr = new CountryHistoryOfficalChange;
					chocPtr->m_playerName = name;
					chocPtr->m_officalName = "丞相";
					history->push_back(chocPtr.get());
					memset(log,0,sizeof(log));
					sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());

					logdata = log;
					GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));
					modify = true;
				}

				if (candidateSize >= 2)
				{
					int oldID;
					//std::string oldName;
					shiliData->getParam(WCHDATA_JXJDasima,param);
					oldID = *(int *)param;
					//shiliData->getParam(WCHDATA_JXJDasimaName,param);
					//oldName = *(std::string *)param;

					id = (*candidateVecL)[1]->nPlayerID;
					name = (*candidateVecL)[1]->playerName;
					shiliData->inputParam(WCHDATA_JXJDasima,&id);
					shiliData->inputParam(WCHDATA_JXJDasimaName,&name);
					//shiliData->inputParam(WCHDATA_JXJDasimaLastLogin,&nowTime);
					//candidateVecL->erase(candidateVecL->begin());

					if (oldID != id)
					{
						chocPtr = new CountryHistoryOfficalChange;
						chocPtr->m_playerName = name;
						chocPtr->m_officalName = "大司马";
						history->push_back(chocPtr.get());
						memset(log,0,sizeof(log));
						sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());

						logdata = log;
						GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));
						modify = true;
					}

					if (candidateSize >= 3)
					{
						int oldID;
						//std::string oldName;
						shiliData->getParam(WCHDATA_JXJDajiangjun,param);
						oldID = *(int *)param;
						//shiliData->getParam(WCHDATA_JXJDajiangjunName,param);
						//oldName = *(std::string *)param;

						id = (*candidateVecL)[2]->nPlayerID;
						name = (*candidateVecL)[2]->playerName;
						shiliData->inputParam(WCHDATA_JXJDajiangjun,&id);
						shiliData->inputParam(WCHDATA_JXJDajiangjunName,&name);
						//shiliData->inputParam(WCHDATA_JXJDajiangjunLastLogin,&nowTime);
						//candidateVecL->erase(candidateVecL->begin());

						if (oldID != id)
						{
							chocPtr = new CountryHistoryOfficalChange;
							chocPtr->m_playerName = name;
							chocPtr->m_officalName = "大将军";
							history->push_back(chocPtr.get());
							memset(log,0,sizeof(log));
							sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());

							logdata = log;
							GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));
							modify = true;
						}
					}
					else
					{
						shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
						shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
					}
				}
				else
				{
					shiliData->inputParam(WCHDATA_JXJDasima,NULL);
					shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
					shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
					shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
				}
			}
			else
			{
				shiliData->inputParam(WCHDATA_JXJChengxiang,NULL);
				shiliData->inputParam(WCHDATA_JXJChengxiangName,NULL);
				shiliData->inputParam(WCHDATA_JXJDasima,NULL);
				shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
			}
			int maxSize = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryHistoryMaxNum).c_str()));
			if (history->size() > maxSize)
			{
				modify = true;
			}
			while (history->size() > maxSize)
			{
				history->pop_front();
			}
			shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
			shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}while (0);
}

/**************************************************************************

METHOD: crJXJOnlineSynToRankMethod
 
**************************************************************************/

JXJ::crJXJOnlineSynToRankMethod::crJXJOnlineSynToRankMethod():
	m_this(NULL)
{

}

JXJ::crJXJOnlineSynToRankMethod::crJXJOnlineSynToRankMethod( const crJXJOnlineSynToRankMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJOnlineSynToRankMethod::inputParam( int i, void *param )
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

void JXJ::crJXJOnlineSynToRankMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJOnlineSynToRankMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{

		crData * gsBrainData = m_this->getDataClass();

		typedef std::vector<int> PlayerIDVec;
		PlayerIDVec playerIDVec;
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;

		for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i)
		{
			gsBrainData->getParam(i,param);
			crJXJShili *shili = (crJXJShili *)param;
			if (shili && shili->getDataClass())
			{
				crData *shiliData = shili->getDataClass();	
				CRCore::ScopedLock<crData> lock(*shiliData);

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				playerIDVec.reserve(peopleMap->size() + playerIDVec.size());
				for (PeopleMap::iterator pmItr = peopleMap->begin(); pmItr != peopleMap->end(); ++ pmItr)
				{
					playerIDVec.push_back(pmItr->first);
				}
			}
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData;
		crPlayerGameData *playerGameData;
		for (PlayerIDVec::iterator pvItr = playerIDVec.begin(); pvItr != playerIDVec.end(); ++ pvItr)
		{
			playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(*pvItr));
			if (playerData.valid() && playerData->getPlayerGameData())
			{
				playerGameData = playerData->getPlayerGameData();
				playerGameData->doEvent(WCH_JXJUpdateRankingData/*	,MAKEINT64(playerData.get(),NULL)*/);
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJQueryCityMapChengChiInfoMethod
 
**************************************************************************/

JXJ::crJXJQueryCityMapChengChiInfoMethod::crJXJQueryCityMapChengChiInfoMethod()
{

}

JXJ::crJXJQueryCityMapChengChiInfoMethod::crJXJQueryCityMapChengChiInfoMethod( const crJXJQueryCityMapChengChiInfoMethod& handle )
{

}

void JXJ::crJXJQueryCityMapChengChiInfoMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJQueryCityMapChengChiInfoMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJQueryCityMapChengChiInfoMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryCityMapChengChiInfo,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/**************************************************************************

METHOD: crJXJRecvQueryCityMapChengChiInfoMethod
 
**************************************************************************/

JXJ::crJXJRecvQueryCityMapChengChiInfoMethod::crJXJRecvQueryCityMapChengChiInfoMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvQueryCityMapChengChiInfoMethod::crJXJRecvQueryCityMapChengChiInfoMethod( const crJXJRecvQueryCityMapChengChiInfoMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_netType(handle.m_netType),
	m_this(NULL),
	m_stream(NULL)
{

}

void JXJ::crJXJRecvQueryCityMapChengChiInfoMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryCityMapChengChiInfoMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvQueryCityMapChengChiInfoMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				void *param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					CantonInfoMap chengchiInfoMap;
					CantonInfoMap::iterator cifitr;
					CantonInfo *ptr = NULL;
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
					ChengchiMap *chengchiMap = (ChengchiMap *)param;
					ConsortiaMap::iterator groupItr ;
					ChengchiMap::iterator ccmitr;
					ConsortiaMemberMap::iterator cmmItr;
					std::string groupName;
					std::string groupLeader;
					FenFengedChengChiMap * fenfengedchengchimap = NULL;
					ConsortiaMemberMap *consortiaMemberMap = NULL;
					CantonMap * cantonMap = NULL;
					crJXJShili *shili = NULL;
					crData *shilidata = NULL;
					int bufsize = 0;
					int groupid = 0;
					int cantonId = 0;
					int chengchiid = 0 ;
					int shiliid = 0;
					ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
					if(!chengchitab.valid())
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
						return;
					}
					crTableIO::DataVec recordVec;
					int chengchiidIdx = chengchitab->getTitleIndex("id");
					int cantonIdx = chengchitab->getTitleIndex("州ID");
					int chengchinameIdx = chengchitab->getTitleIndex("名字");
					int junnameIdx = chengchitab->getTitleIndex("郡名");
					crData *groupdata = NULL;
					for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i)
					{
						shiliid = i - 20273;
						gsBrainData->getParam(i,param);
						shili = (crJXJShili *)param;
						if (!shili || shili->getDataClass() == NULL) break;
						shilidata = shili->getDataClass();	
						CRCore::ScopedLock<crData> lock(*shilidata);
						shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
						cantonMap = (CantonMap *)param;
						for (CantonMap::iterator cmItr = cantonMap->begin(); cmItr != cantonMap->end(); ++ cmItr)
						{
							if (cmItr->second.valid())
							{
								groupid = cmItr->second->getConsortiaId();
								if (groupid > 0)
								{
									groupItr = consortiamap->find(groupid);
									if (groupItr != consortiamap->end())
									{
										groupItr->second->getName(groupName);
										groupItr->second->getLeaderName(0,groupLeader);
										cantonId = cmItr->second->getCantonId();
										chengchitab->queryRecords(cantonIdx,crArgumentParser::appItoa(cantonId),recordVec);
										if(!recordVec.empty())
										{
											for (crTableIO::DataVec::iterator ritr = recordVec.begin();ritr != recordVec.end();)
											{
												chengchiid = atoi((*ritr)[chengchiidIdx].c_str());
												ccmitr = chengchiMap->find(chengchiid);
												if (ccmitr != chengchiMap->end())
												{
													if (shiliid == ccmitr->second->getChengzhuShili())
													{
														ptr = new CantonInfo(groupid,groupName,groupLeader);
														chengchiInfoMap.insert(CantonInfoMap::value_type(chengchiid,ptr));
													}
												}
												++ritr;
											}
										}
										
									}
								}
							}
						}		
					}

					for (groupItr = consortiamap->begin();groupItr != consortiamap->end(); ++groupItr)
					{
						if (groupItr->second.valid())
						{
							groupdata = groupItr->second->getDataClass();
							groupItr->second->getName(groupName);
							groupid = groupItr->first;
							if (groupdata)
							{
								groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
								consortiaMemberMap = (ConsortiaMemberMap *)param;
								groupdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
								fenfengedchengchimap = (FenFengedChengChiMap *)param;
								for (FenFengedChengChiMap::iterator ffccmitr = fenfengedchengchimap->begin(); ffccmitr != fenfengedchengchimap->end(); ++ffccmitr)
								{
									chengchiid = ffccmitr->first;
									if (chengchiid > 0)
									{
										cmmItr = consortiaMemberMap->find(ffccmitr->second);
										if (cmmItr != consortiaMemberMap->end() && cmmItr->second.valid())
										{
											groupLeader = cmmItr->second->getName();
											ptr = new CantonInfo(groupid,groupName,groupLeader);
											cifitr = chengchiInfoMap.find(chengchiid);
											if (cifitr != chengchiInfoMap.end())
											{
												chengchiInfoMap[chengchiid] = ptr;
											}
											else
											{
												chengchiInfoMap.insert(CantonInfoMap::value_type(chengchiid,ptr));
											}
										}
									}
								}
							}
						}		
					}

					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(512);
					unsigned short count = chengchiInfoMap.size();
					stream->_writeUShort(count);
					for (cifitr = chengchiInfoMap.begin();cifitr != chengchiInfoMap.end();++cifitr)
					{
						stream->_writeInt(cifitr->first);
						/*stream->_writeInt(cifitr->second->m_groupID);*/
						stream->_writeString(cifitr->second->m_groupName);
						stream->_writeString(cifitr->second->m_groupLeader);
					}
					crPlayerDataEventZipPacket packet;
					crPlayerDataEventZipPacket::buildReplyPacket(packet,playerData->getPlayerID(),WCH_JXJRecvQueryCityMapChengChiInfo,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				crData * canvasData = canvas->getDataClass();
				void *param;
				canvasData->excHandle(MAKEINT64(WCH_LockData,1));
				canvasData->getParam(WCHDATA_JXJChengChiInfoMap,param);
				CantonInfoMap * cantonInfoMap = (CantonInfoMap *)param;
				cantonInfoMap->clear();
				CantonInfo *ptr = NULL;
				int groupID = 0;
				int chengchiID;
				std::string groupName;
				std::string groupLeader;
				unsigned short count = m_stream->_readUShort();
				for (unsigned short i = 0; i < count; ++ i)
				{
					chengchiID = m_stream->_readInt();
					/*groupID = m_stream->_readInt();*/
					groupName = m_stream->_readString();
					groupLeader = m_stream->_readString();
					ptr = new CantonInfo(groupID,groupName,groupLeader);
					cantonInfoMap->insert(CantonInfoMap::value_type(chengchiID,ptr));	
				}
				canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

JXJ::crJXJGameServerThrowMethod::crJXJGameServerThrowMethod()
{

}

JXJ::crJXJGameServerThrowMethod::crJXJGameServerThrowMethod( const crJXJGameServerThrowMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{

}

void JXJ::crJXJGameServerThrowMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJGameServerThrowMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJGameServerThrowMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		// 点击确定后
		crData * canvasData = canvas->getDataClass();
		// 记录当前时间
		time_t nowTime = time(NULL);
		canvasData->inputParam(WCHDATA_JXJGameServerThrowTimeInterval,&nowTime );

		// openlogin 0

		std::string chat_text = "/opengamelogin 0";
		crNetConductor *net_conductor;
		net_conductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (net_conductor)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4 + chat_text.size());
			stream->_writeString(chat_text);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGMOrder,stream.get());
			net_conductor->getNetManager()->sendPacket("all",packet);

			// 踢所有玩家下线

			stream->seekBegin();
			stream->_writeString("");
			stream->_writeUChar(JXJGMOperation_kickAllPlayerOff);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJAddNoChatPlayer,stream.get());
			net_conductor->getNetManager()->sendPacket("all",packet);
		}

		// 脚本秒后重启服务器
	}
}

/**************************************************************************

METHOD: crJXJGameServerTimeCountMethod
 
**************************************************************************/

JXJ::crJXJGameServerTimeCountMethod::crJXJGameServerTimeCountMethod():
	m_interval(0)
{

}

JXJ::crJXJGameServerTimeCountMethod::crJXJGameServerTimeCountMethod( const crJXJGameServerTimeCountMethod& handle ):
	crMethod(handle),
	m_interval(handle.m_interval),
	m_canvas(handle.m_canvas)
{

}

void JXJ::crJXJGameServerTimeCountMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJGameServerTimeCountMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas =str;
		break;
	case 1:
		m_interval = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJGameServerTimeCountMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		crData * canvasData = canvas->getDataClass();
		time_t nowTime = time(NULL);
		canvasData->getParam(WCHDATA_JXJGameServerThrowTimeInterval,param);
		time_t lastTime = *(time_t *)param;
		if (lastTime && nowTime - lastTime > m_interval)
		{
			// 脚本秒后重启服务器

			std::string chat_text = "/jxjgameserverthrow";
			crNetConductor *net_conductor;
			net_conductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if (net_conductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4 + chat_text.size());
				stream->_writeString(chat_text);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGMOrder,stream.get());
				net_conductor->getNetManager()->sendPacket("all",packet);
			}
			canvasData->inputParam(WCHDATA_JXJGameServerThrowTimeInterval,NULL);
		}
	}
}

JXJ::crJXJGameServerButtonClickMethod::crJXJGameServerButtonClickMethod()
{

}

JXJ::crJXJGameServerButtonClickMethod::crJXJGameServerButtonClickMethod( const crJXJGameServerButtonClickMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_confirm(handle.m_confirm)
{

}

void JXJ::crJXJGameServerButtonClickMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGameServerButtonClickMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_confirm =str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJGameServerButtonClickMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_input));
		ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_confirm));
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content",1);
		v_i.clear();
		v_i.push_back(255.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("Text","您确认要重启服务器吗？");
		cfg_script.Pop();

		cfg_script.Pop();

		if(information.valid())information->setHypertext(cfg_script);
		if(okbtn.valid())okbtn->setEnable(true);
		crData *dlgData = canvas->getDataClass();
		int commandtype = CDP_Widget;
		dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
		dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
		crFilterRenderManager::getInstance()->doModal(canvas.get());
	}
}

////////////监控服务器组
//{
//	m_init=true
//	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
//	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
//	netConductor = netContainer->getNetConductor(GameClient_Game);
//	std::string hostip;
//	//int port = 9100-9109
//	//netConductor->getNetManager()->getHostInfo()
//	for(int i = 0; i<10; i++)
//	{
//		netConductor = new CRNet::crNetConductor;
//		netConductor->init(port);
//		netConductor->setNetType(JXJMonitor1+i);
//		netConductor->setName(serverName);
//		netConductor->setKeyStr("GameServer");
//		//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
//		int code = netConductor->getNetManager()->clientReconnectToServerCircle(hostip,callback,JXJMonitor1+i,"GameConnectServer","GameConnectServer",9108,_bufSizeScale);
//
//		if(code<=0)
//		{
//			return;
//		}
//
//		netContainer->pushNetConductor(netConductor.get());
//	}
//
//	crJXJMonitorPingPacket pkg;
//	for (int i = 0; i<10; i++)
//	{
//		netConductor = netContainer->getNetConductor(JXJMonitor1+i);
//		netConductor->getNetManager()->sendPacket("all",pkg);
//	}
//}

/**************************************************************************

METHOD: crJXJInitClientConnectGatewayMethod
 
**************************************************************************/

JXJ::crJXJInitClientConnectGatewayMethod::crJXJInitClientConnectGatewayMethod():
	m_serverGroupNum(10),
	m_gatewayNum(0),
	m_port(0)
{

}

JXJ::crJXJInitClientConnectGatewayMethod::crJXJInitClientConnectGatewayMethod( const crJXJInitClientConnectGatewayMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_serverGroupNum(10),
	m_gatewayNum(0),
	m_port(0)
{

}

void JXJ::crJXJInitClientConnectGatewayMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJInitClientConnectGatewayMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_serverGroupNum = atoi(str.c_str());
		break;
	case 2:
		m_port = atoi(str.c_str());
		break;
	case 3:
		m_gatewayNum = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJInitClientConnectGatewayMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		//void *param;
		crData * canvasData = canvas->getDataClass();
	/*	canvasData->getParam(WCHDATA_JXJGMToolsInit,param);
		bool init = *(bool *)param;
		if (init)
		{
			return;
		}*/
		//canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		//canvasData->getParam(WCHDATA_JXJServerHeartMap	,param);
		//crJXJOperationCanvasData:: ServerGroupVec * serverGroupVec = (crJXJOperationCanvasData::ServerGroupVec *)param;
	//	serverGroupVec->resize(m_serverGroupNum);

		//canvasData->excHandle(MAKEINT64(WCH_LockData,0));


		CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
		CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
		netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		// netConductor = netContainer->getNetConductor(GameClient_Game); 获取指针为空

		int port ;
		std::string hostip; 
		std::string callback;
		int playerid;
		std::string serverName = "GameConnectServer";
		std::string serverPassword = "GameConnectServer";
		netConductor->getNetManager()->getHostInfo(hostip,callback,playerid,serverName,serverPassword,port);

		for(char i = 0; i < m_gatewayNum; i++)
		{
			netConductor = new CRNet::crNetConductor;
			netConductor->init(m_port + i);
			netConductor->setNetType(JXJMonitor1+i);
			//netConductor->setName(serverName);
			netConductor->setName(crArgumentParser::appItoa(JXJMonitor1+i));
			netConductor->setEnableCook(true);

			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(hostip,"MonitorConnectServer",JXJMonitor1+i,"GameConnectServer","GameConnectServer",m_port + i,1,1);

			if(code<=0)
			{
				continue;
			}
			netContainer->pushNetConductor(netConductor.get());
		}
		//init = true;
		//canvasData->inputParam(WCHDATA_JXJGMToolsInit,&init);
	}

}

/**************************************************************************

METHOD: crJXJCountMonitorPacketMethod
 
**************************************************************************/

JXJ::crJXJCountMonitorPacketMethod::crJXJCountMonitorPacketMethod():
	m_netType(Client )
{

}

JXJ::crJXJCountMonitorPacketMethod::crJXJCountMonitorPacketMethod( const crJXJCountMonitorPacketMethod& handle ):
	crMethod(handle),
	m_netType(Client )
{

}

void JXJ::crJXJCountMonitorPacketMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
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
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	default:
		break;
	}
}

void JXJ::crJXJCountMonitorPacketMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJCountMonitorPacketMethod::operator()( crHandle &handle )
{
	if (m_netType != Client && m_stream.valid())
	{
		double curtime;
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && canvas->getDataClass())
		{
			void *param;
			crData * canvasData = canvas->getDataClass();
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJServerHeartMap	,param);

			crJXJOperationCanvasData::ServerHeartMap *tmpmap = (crJXJOperationCanvasData::ServerHeartMap *)param;
			std::string sername = m_stream->_readString();
			curtime = crFrameStamp::getInstance()->getReferenceTime();
			//tmpmap->insert(std::make_pair(sername,curtime));
			(*tmpmap)[sername]=curtime;
			//canvasData->inputParam(WCHDATA_JXJOnlineCount,&tmpmap);

			//crJXJOperationCanvasData:: ServerGroupVec * serverGroupVec = (crJXJOperationCanvasData::ServerGroupVec *)param;
			//if (serverGroupVec && m_netType < serverGroupVec->size())
			//{
			//	std::string sername =m_stream->_readString();
			//	ServerHeartMap serverHertMap;
			//	serverHertMap[sername]=crFrameStamp::getInstance()->getReferenceTime();
			//	crJXJOperationCanvasData::ServerHeartMap serverHertMap;
			//	serverHertMap.insert(std::make_pair(sername,crFrameStamp::getInstance()->getReferenceTime()));

			//	(*serverGroupVec)[m_netType].push_back(std::make_pair( m_stream->_readInt(),m_stream->_readInt()));

			//}


			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/**************************************************************************

METHOD: crJXJMonitorDisplayAndRefreshMethod
 
**************************************************************************/

JXJ::crJXJMonitorDisplayAndRefreshMethod::crJXJMonitorDisplayAndRefreshMethod():
	m_serverNum(0)
{

}

JXJ::crJXJMonitorDisplayAndRefreshMethod::crJXJMonitorDisplayAndRefreshMethod( const crJXJMonitorDisplayAndRefreshMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_tabList(handle.m_tabList),
	m_serverNum(0)
{

}

void JXJ::crJXJMonitorDisplayAndRefreshMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJMonitorDisplayAndRefreshMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_tabList = str;
		break;
	case 2:
		m_serverNum = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJMonitorDisplayAndRefreshMethod::operator()( crHandle &handle )
{

	//ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	//if (canvas.valid() && canvas->getDataClass())
	//{
	//	ref_ptr<crTableWidgetNode>  tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_tabList));
	//	crData * canvasData = canvas->getDataClass();
	//	canvasData->excHandle(MAKEINT64(WCH_LockData,1));

	//	void *param;
	//	canvasData->getParam(WCHDATA_JXJGMServerGroupVec,param);
	//	crJXJOperationCanvasData::ServerGroupVec * serverGroupVec= (crJXJOperationCanvasData::ServerGroupVec *)param;

	//	// 统计并显示
	//	typedef crJXJOperationCanvasData:: ServerGroupVec ServerGroupVec; 
	//	typedef crJXJOperationCanvasData:: ServerVec ServerVec; 
	//	
	//	if (tableWidget.valid())
	//	{
	//		unsigned char i = 0;
	//		std::string gatewayName = "网关";
	//		std::string netStatus = "正常";
	//		int peopleNum = 0;
	//		unsigned char gameServer = 0;
	//		unsigned char sceneServer = 0;
	//		for (ServerGroupVec::iterator sgvItr = serverGroupVec->begin(); sgvItr != serverGroupVec->end(); ++i,++ sgvItr)
	//		{

	//			tableWidget->addData(i,0,gatewayName + crArgumentParser::appItoa(i +1));

	//			peopleNum = 0;
	//			for (ServerVec::iterator svItr = sgvItr->begin(); svItr != sgvItr->end(); ++ svItr)
	//			{
	//				peopleNum += svItr->first;
	//			}
	//			if (!sgvItr->empty())
	//			{
	//				peopleNum /= sgvItr->size();
	//			}
	//			tableWidget->addData(i,1,crArgumentParser::appItoa(peopleNum));

	//			if (sgvItr->size() == m_serverNum)
	//			{
	//				tableWidget->addData(i,2,netStatus);
	//			}
	//			else
	//			{
	//				gameServer = 0;
	//				sceneServer = 0;
	//				for (ServerVec::iterator svItr = sgvItr->begin(); svItr != sgvItr->end(); ++ svItr)
	//				{
	//					switch (svItr->second)
	//					{
	//					case GameServer:
	//						++ gameServer;
	//						break;
	//					case SceneServer:
	//						++ sceneServer;
	//						break;
	//					default:
	//						break;
	//					}
	//				}
	//				std::ostringstream text;
	//				text << " gs:"<< gameServer << " ss:" << sceneServer;
	//				tableWidget->addData(i,2,text.str());
	//			}
	//		}
	//	}



		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && canvas->getDataClass())
		{
			ref_ptr<crTableWidgetNode>  tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_tabList));
			crData * canvasData = canvas->getDataClass();
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));

			// 重新发包
	/*		void *param;
			canvasData->getParam(WCHDATA_JXJGMToolsInit,param);
			bool init = *(bool *)param;
			if (init)
			{*/
				crJXJMonitorPingPacket pkg;
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if (netConductor)
				{
					netConductor->getNetManager()->sendPacket("all",pkg);
				}
			
				//for (char i = 0; i< m_serverNum ; i++)
				//{
				//	netConductor = netContainer->getNetConductor(JXJMonitor1+i);
				//	if (netConductor.valid())
				//	{
				//		netConductor->getNetManager()->sendPacket("all",pkg);
				//	}
				//}
			/*}*/
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}	
}

/**************************************************************************

METHOD: crJXJRestartServerCheckMethod
 
**************************************************************************/

JXJ::crJXJRestartServerCheckMethod::crJXJRestartServerCheckMethod():
	m_this(NULL)
{

}

JXJ::crJXJRestartServerCheckMethod::crJXJRestartServerCheckMethod( const crJXJRestartServerCheckMethod & handle ):
	crMethod(handle),
	m_userPasswordWidget(handle.m_userPasswordWidget),
	m_this(NULL)
{

}

void JXJ::crJXJRestartServerCheckMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRestartServerCheckMethod::addParam( int i, const std::string& str )
{
		
	switch (i)
	{
	case 0:
		m_userPasswordWidget = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJRestartServerCheckMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		std::string userPassword;
		crCanvasNode *canvas = m_this->getParentCanvas();

		ref_ptr<crEditWidgetNode> passwordWidget = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_userPasswordWidget));
		if(passwordWidget.valid())
		{
			userPassword = crArgumentParser::trim(const_cast<char *>(passwordWidget->getUTF8String().c_str()));
			passwordWidget->clearString();
		}

		if(userPassword.empty())
		{
			//CRCore::notify(CRCore::FATAL)<<"Name或者Password不能为空"<<std::endl;
			return;
		}

		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor)
		{
			crPlayerDataEventPacket packet;
			CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
			m_stream->createBuf(userPassword.length() + 4);
			m_stream->_writeString(userPassword);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvRestartServerCheck,m_stream.get() );
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/**************************************************************************

METHOD: crJXJRecvRestartServerCheckMethod
 
**************************************************************************/
JXJ::crJXJRecvRestartServerCheckMethod::crJXJRecvRestartServerCheckMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvRestartServerCheckMethod::crJXJRecvRestartServerCheckMethod( const crJXJRecvRestartServerCheckMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

void JXJ::crJXJRecvRestartServerCheckMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_netType = Client;
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
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvRestartServerCheckMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvRestartServerCheckMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			unsigned char suc = 0;

			std::string userPassword = m_stream->_readString();
			crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();

			ref_ptr<crDataBase> accountSession = accountdb->beginSession();

			ref_ptr<crJXJCheckPasswordQueryData > checkPasswordData = new crJXJCheckPasswordQueryData ;
			checkPasswordData->buildQuerySql(m_this->getPlayerID(),userPassword);

			accountSession->executeQuery(checkPasswordData.get());
			crDataBase::QueryResultVec &loginQueryResultVec = accountSession->getQueryResult();
			if(loginQueryResultVec.empty())
			{
				suc = 0;
				// 密码错误
			}
			else suc = 1;
			accountSession->releaseQuery();
			accountdb->endSession(accountSession.get());

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData 
				= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if (playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(suc);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerData->getPlayerID(),WCH_JXJRecvRestartServerCheck,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if (m_netType == GameClient_Game)
		{
			if (m_stream->_readUChar())
			{
				// 密码正确

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (canvas.valid() && canvas->getDataClass())
				{
					void *param;
					crData * canvasData = canvas->getDataClass();
					canvasData->getParam(WCHDATA_JXJGMRestartServerType	,param);
					int netType = *(int *)param;

					// 马上重置
					canvasData->inputParam(WCHDATA_JXJGMRestartServerType	,NULL);

					CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
					CRCore::ref_ptr<CRNet::crNetConductor> netConductor;

					crJXJRestartServerPacket restartServerPacket;
					netConductor = netContainer->getNetConductor(JXJMonitor1 + netType);
					if (netConductor.valid())
					{
						netConductor->getNetManager()->sendPacket("all",restartServerPacket);
					}
				}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJStatisticsMethod
 
**************************************************************************/

JXJ::crJXJRemainStatisticsMethod::crJXJRemainStatisticsMethod():
	m_this(NULL)
{

}

JXJ::crJXJRemainStatisticsMethod::crJXJRemainStatisticsMethod( const crJXJRemainStatisticsMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJRemainStatisticsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRemainStatisticsMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRemainStatisticsMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		int curServerID = callback->getServerID();

		typedef std::vector<int> RemainplayerIDvec;
		RemainPlayerIDVec remainplayerIDvec;
		remainplayerIDvec.reserve(4);
		crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();//getAccountDB();;
		//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crDataBase> db_session = global_db->beginSession();
		ref_ptr<crGetTodayLoginPlayerData> playerdataquery = NULL;
		int overdue = 0;
		for (int i = 0; i < 4; ++i)
		{
			switch (i)
			{//n天留存率
			case 0:
				overdue = 2;
				break;
			case 1:
				overdue = 3;
				break;
			case 2:
				overdue = 7;
				break;
			case 3:
				overdue = 1;
				break;
			default:
				break;
			}
			playerdataquery = new crGetTodayLoginPlayerData;
			playerdataquery->buildQuerySql(curServerID,overdue,JXJGMPermission_newer);
			db_session->executeQuery(playerdataquery.get());
			crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				playerdataquery = dynamic_cast<crGetTodayLoginPlayerData*>(itr->get());
				remainplayerIDvec.push_back(playerdataquery->getCount());
			}
			db_session->releaseQuery();
		}
		remainplayerIDvec.resize(4);
		//// 查询charactername
		ref_ptr<crSelectCountCharacternameExceptGMData> selectCountQuery = NULL;
		selectCountQuery = new crSelectCountCharacternameExceptGMData;
		//std::string tabname = "charactername";
		selectCountQuery->buildQuerySql(curServerID,JXJGMPermission_newer);

		int characternameCount = 0;
		db_session->executeQuery(selectCountQuery.get());
		crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			selectCountQuery = dynamic_cast<crSelectCountCharacternameExceptGMData *>(playerdataQueryResultVec.begin()->get());
			characternameCount = selectCountQuery->getCount();
		}
		db_session->releaseQuery();
	
		
		global_db->endSession(db_session.get());
		
		//留存人数
		ref_ptr<crTableIO> table = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowVec;
		titleVec.resize(6);
		rowVec.resize(6);
		titleVec[0] = "title";
		titleVec[1] = "创建角色数";
		titleVec[2] = "二日留存";
		titleVec[3] = "三日留存";
		titleVec[4] = "七日留存";
		titleVec[5] = "当前留存";
		table->setTitleVec(titleVec);
		rowVec[0] = "数量";
		rowVec[1] = crArgumentParser::appItoa(characternameCount);
		rowVec[2] = crArgumentParser::appItoa(remainplayerIDvec[0]);
		rowVec[3] = crArgumentParser::appItoa(remainplayerIDvec[1]);
		rowVec[4] = crArgumentParser::appItoa(remainplayerIDvec[2]);
		rowVec[5] = crArgumentParser::appItoa(remainplayerIDvec[3]);
		table->addData(rowVec);

		rowVec[0] = "比例";
		if (characternameCount )
		{
			rowVec[1] = crArgumentParser::appFtoa(characternameCount * 100.0f / characternameCount ) + "%";
			rowVec[2] = crArgumentParser::appFtoa(remainplayerIDvec[0] * 100.0f / characternameCount ) + "%";
			rowVec[3] = crArgumentParser::appFtoa(remainplayerIDvec[1] * 100.0f / characternameCount ) + "%";
			rowVec[4] = crArgumentParser::appFtoa(remainplayerIDvec[2] * 100.0f / characternameCount ) + "%";
			rowVec[5] = crArgumentParser::appFtoa(remainplayerIDvec[3] * 100.0f / characternameCount ) + "%";
			table->addData(rowVec);
		}
		
		
		const time_t nowTime = time(NULL);
		char dateStr[128];
		strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));


		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());

		std::string tabName = std::string("../JXJDB/") + netCallback->getServerName() + "/" 
			+ std::string("玩家留存统计_") + std::string(dateStr) + std::string(".tab");
		table->saveToFileNoCook(tabName);
	}
}

/**************************************************************************

METHOD: crJXJLevelStatisticsMethod
 
**************************************************************************/

JXJ::crJXJLevelStatisticsMethod::crJXJLevelStatisticsMethod():
	m_this(NULL)
{

}

JXJ::crJXJLevelStatisticsMethod::crJXJLevelStatisticsMethod( const crJXJLevelStatisticsMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJLevelStatisticsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJLevelStatisticsMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJLevelStatisticsMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		int curServerID = callback->getServerID();
		// account_db begin
		crDataBase *account_db = crDataBaseManager::getInstance()->getAccountDB();;
		ref_ptr<crDataBase> db_session = account_db ->beginSession();

		//// 查询 account
		ref_ptr<crSelectCountTableData > selectAccountCountQuery = NULL;
		selectAccountCountQuery = new crSelectCountTableData ;
		std::string tabname = "account";
		selectAccountCountQuery->buildQuerySql(tabname);

		int accountCount = 0;
		db_session->executeQuery(selectAccountCountQuery.get());
		crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			selectAccountCountQuery = dynamic_cast<crSelectCountTableData *>(playerdataQueryResultVec.begin()->get());
			accountCount = selectAccountCountQuery->getCount();
		}
		db_session->releaseQuery();
		
		account_db->endSession(db_session.get());
		// account_db end

		// global_db begin
		crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();//getAccountDB();;
		db_session = global_db->beginSession();

		/// 查询 gm

		ref_ptr<crSelectCountTableData > selectGMCountQuery = NULL;
		selectGMCountQuery = new crSelectCountTableData ;
		tabname = "gm";
		selectGMCountQuery->buildQuerySql(tabname);

		int gmCount = 0;
		db_session->executeQuery(selectGMCountQuery.get());
		playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			selectGMCountQuery= dynamic_cast<crSelectCountTableData *>(playerdataQueryResultVec.begin()->get());
			gmCount = selectGMCountQuery->getCount();
		}
		db_session->releaseQuery();

		accountCount -= gmCount;

		// 查询gmid
		typedef std::set<int> GMIDSet;
		GMIDSet gmIDSet;


		ref_ptr<crJXJSelectGMIDData> selectGMIDQuery = NULL;
		selectGMIDQuery = new crJXJSelectGMIDData;
		selectGMIDQuery->buildQuerySql(curServerID,JXJGMPermission_newer);

		db_session->executeQuery(selectGMIDQuery.get());
		playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			for (crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end(); ++ itr)
			{
				selectGMIDQuery = dynamic_cast<crJXJSelectGMIDData *>(itr->get());
				gmIDSet.insert(selectGMIDQuery->getPlayerID());
			}
		}
		db_session->releaseQuery();

		//// 查询charactername
		ref_ptr<crSelectCountCharacternameExceptGMData> selectCharacternameCountQuery = NULL;
		selectCharacternameCountQuery = new crSelectCountCharacternameExceptGMData;
		//tabname = "charactername";
		selectCharacternameCountQuery->buildQuerySql(curServerID,JXJGMPermission_newer);

		int characternameCount = 0;
		db_session->executeQuery(selectCharacternameCountQuery .get());
		playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			selectCharacternameCountQuery = dynamic_cast<crSelectCountCharacternameExceptGMData *>(playerdataQueryResultVec.begin()->get());
			characternameCount = selectCharacternameCountQuery ->getCount();
		}
		db_session->releaseQuery();
		
		global_db->endSession(db_session.get());
		// global_db end


		int peopleNum[3] = {0,0,0};
		crData * gsBrainData = m_this->getDataClass();

		typedef std::vector<int> PlayerLevelVec;
		PlayerLevelVec playerLevelVec;
		playerLevelVec.resize(81,0);
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		int j = 0;
		for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i, ++ j)
		{
			gsBrainData->getParam(i,param);
			crJXJShili *shili = (crJXJShili *)param;
			if (shili && shili->getDataClass())
			{
				crData *shiliData = shili->getDataClass();	
				CRCore::ScopedLock<crData> lock(*shiliData);

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				//peopleNum[i - (WCHDATA_JXJShiliWei)] = peopleMap->size();
				for (PeopleMap::iterator pmItr = peopleMap->begin(); pmItr != peopleMap->end(); ++ pmItr)
				{
					if (pmItr->second->getLv() >= 0 
						&& pmItr->second->getLv() <= 80
						&& gmIDSet.find(pmItr->second->getID()) == gmIDSet.end())
					{
						++ playerLevelVec[pmItr->second->getLv()];
						++ peopleNum[j];
					}
				}
			}
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		ref_ptr<crTableIO> levelTable = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowRec;
		titleVec.resize(2);
		rowRec.resize(2);
		titleVec[0] = "title";
		titleVec[1] = "全部";
		levelTable ->setTitleVec(titleVec);

		rowRec[0]= "总注册";
		rowRec[1] = crArgumentParser::appItoa(accountCount);
		levelTable->addData(rowRec);

		rowRec[0] = "总创建";
		rowRec[1] = crArgumentParser::appItoa(characternameCount);
		levelTable->addData(rowRec);

		std::string countryName[3] = {"魏国","蜀国","吴国"};
		for (unsigned char i = 0; i < 3; ++ i)
		{
			rowRec[0] = countryName[i];
			rowRec[1] = crArgumentParser::appItoa(peopleNum[i]);
			levelTable->addData(rowRec);
		}

		for (unsigned char i = 1; i <= 80; ++ i)
		{
			rowRec[0] = crArgumentParser::appItoa(i) + "级";
			rowRec[1] = crArgumentParser::appItoa(playerLevelVec[i]) + " ("
				+ crArgumentParser::appFtoa(playerLevelVec[i] * 100.0f / characternameCount ) + ")%";
			levelTable->addData(rowRec);
		}
		
		const time_t nowTime = time(NULL);
		char dateStr[128];
		strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));

		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());

		std::string tabName = std::string("../JXJDB/") + netCallback->getServerName() + "/" 
			+ std::string("注册统计_") + std::string(dateStr) + std::string(".tab");
		levelTable->saveToFileNoCook(tabName);
	}
}

/**************************************************************************

METHOD: crJXJNewerStepStatisticsMethod
 
**************************************************************************/

JXJ::crJXJNewerStepStatisticsMethod::crJXJNewerStepStatisticsMethod():
	m_this(NULL)
{

}

JXJ::crJXJNewerStepStatisticsMethod::crJXJNewerStepStatisticsMethod( const crJXJNewerStepStatisticsMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJNewerStepStatisticsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJNewerStepStatisticsMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJNewerStepStatisticsMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		int curServerID = callback->getServerID();

		// global_db begin
		crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();//getAccountDB();;
		ref_ptr<crDataBase> db_session = global_db->beginSession();

		//// 查询charactername
		ref_ptr<crSelectCountCharacternameExceptGMData> selectCountQuery = new crSelectCountCharacternameExceptGMData;
		//std::string tabname = "charactername";
		selectCountQuery->buildQuerySql(curServerID,JXJGMPermission_newer );

		int characternameCount = 0;
		db_session->executeQuery(selectCountQuery.get());
		crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			selectCountQuery = dynamic_cast<crSelectCountCharacternameExceptGMData *>(playerdataQueryResultVec.begin()->get());
			characternameCount = selectCountQuery->getCount();
		}
		db_session->releaseQuery();

		// 查询gmid
		typedef std::set<int> GMIDSet;
		GMIDSet gmIDSet;

		ref_ptr<crJXJSelectGMIDData> selectGMIDQuery = NULL;
		selectGMIDQuery = new crJXJSelectGMIDData;
		selectGMIDQuery->buildQuerySql(curServerID,JXJGMPermission_newer);

		db_session->executeQuery(selectGMIDQuery.get());
		playerdataQueryResultVec = db_session->getQueryResult();
		if (!playerdataQueryResultVec.empty())
		{
			for (crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end(); ++ itr)
			{
				selectGMIDQuery = dynamic_cast<crJXJSelectGMIDData *>(itr->get());
				gmIDSet.insert(selectGMIDQuery->getPlayerID());
			}
		}
		db_session->releaseQuery();
		
		global_db->endSession(db_session.get());
		// global_db end

		crData * gsBrainData = m_this->getDataClass();
		typedef std::map<int,int> PlayerNewerStepMap;
		PlayerNewerStepMap playerNewerStepMap;
		PlayerNewerStepMap::iterator pnsItr ;
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;

		for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i)
		{
			gsBrainData->getParam(i,param);
			crJXJShili *shili = (crJXJShili *)param;
			if (shili && shili->getDataClass())
			{
				crData *shiliData = shili->getDataClass();	
				CRCore::ScopedLock<crData> lock(*shiliData);

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				for (PeopleMap::iterator pmItr = peopleMap->begin(); pmItr != peopleMap->end(); ++ pmItr)
				{
					if (gmIDSet.find(pmItr->second->getID()) == gmIDSet.end())
					{
						pnsItr = playerNewerStepMap.find(pmItr->second->getNewerStep());
						if (pnsItr != playerNewerStepMap.end())
						{
							pnsItr->second  ++;
						}
						else playerNewerStepMap[pmItr->second->getNewerStep()] = 1;
					}
				}
			}
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		ref_ptr<crTableIO> levelTable = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowVec;
		titleVec.resize(4);
		rowVec.resize(4);
		titleVec[0] = "新手步骤id";
		titleVec[1] = "人数";
		titleVec[2] = "比例";
		titleVec[3] = "流失率";
		levelTable ->setTitleVec(titleVec);

		PlayerNewerStepMap::iterator preItr = playerNewerStepMap.begin();

		for (pnsItr = playerNewerStepMap.begin(); preItr != playerNewerStepMap.end(); preItr = pnsItr ++)
		{
			if (preItr == pnsItr)
			{
				rowVec[0] = "总创角";
				rowVec[1] = crArgumentParser::appItoa(characternameCount);
				rowVec[2] = "100%";
			}
			else
			{
				rowVec[0] = crArgumentParser::appItoa(preItr->first) ;
				rowVec[1] = crArgumentParser::appItoa(preItr->second);
				rowVec[2] = crArgumentParser::appFtoa(preItr->second * 100.0f / characternameCount );
				if (pnsItr != playerNewerStepMap.end())
				{
					rowVec[3] = crArgumentParser::appFtoa((preItr->second - pnsItr->second) * 100.0f / characternameCount );
				}
			}
			levelTable->addData(rowVec);
			if (pnsItr == playerNewerStepMap.end())
			{
				break;
			}
		}
		
		const time_t nowTime = time(NULL);
		char dateStr[128];
		strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));

		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());

		std::string tabName = std::string("../JXJDB/") + netCallback->getServerName() + "/" 
			+ std::string("新手流失_") + std::string(dateStr) + std::string(".tab");
		levelTable->saveToFileNoCook(tabName);
	}
}

 crJXJUIButtonNewTipsMethod::crJXJUIButtonNewTipsMethod():
	m_noticeid(0)
{
}
 crJXJUIButtonNewTipsMethod::crJXJUIButtonNewTipsMethod(const crJXJUIButtonNewTipsMethod& handle):
	crMethod(handle),
	m_noticeid(handle.m_noticeid),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIButtonNewTipsMethod::inputParam(int i, void *param)
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
void crJXJUIButtonNewTipsMethod::addParam(int i, const std::string& str)
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
	}
}
void crJXJUIButtonNewTipsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{
		ref_ptr<crCanvasNode>tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
		if (tips.valid())
		{
			ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tips->getWidget(m_tipsTextWidget ));

			std::string str;
			ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_noticeid),record) >= 0 )
			{
				str = record[1];
			}

			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content",1);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",str);
			cfg_script.Pop();

			cfg_script.Pop();

			if(information.valid())information->setHypertext(cfg_script);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}

/**************************************************************************

METHOD: crJXJCheckServerVersionMethod
 
**************************************************************************/

///JXJ::crJXJCheckServerVersionMethod::crJXJCheckServerVersionMethod()
///{
///
///}
///
///JXJ::crJXJCheckServerVersionMethod::crJXJCheckServerVersionMethod( const crJXJCheckServerVersionMethod & handle ):
///	crMethod(handle)
///{
///
///}
///
///void JXJ::crJXJCheckServerVersionMethod::operator()( crHandle &handle )
///{
///	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
///	if (netConductor)
///	{
///		crPlayerDataEventPacket packet;
///		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckServerVersion,NULL);
///		netConductor->getNetManager()->sendPacket("all",packet);
///	}
///}
///
///void JXJ::crJXJCheckServerVersionMethod::inputParam( int i, void *param )
///{
///
///}
///
///void JXJ::crJXJCheckServerVersionMethod::addParam( int i, const std::string& str )
///{
///	
///}
/**************************************************************************

METHOD: crJXJRecvCheckServerVersionMethod
 
**************************************************************************/

//JXJ::crJXJRecvCheckServerVersionMethod::crJXJRecvCheckServerVersionMethod():
//	m_this(NULL),
//	m_netType(GameClient_Game),
//	m_stream(NULL)
//{
//
//}
//
//JXJ::crJXJRecvCheckServerVersionMethod::crJXJRecvCheckServerVersionMethod( const crJXJRecvCheckServerVersionMethod& handle ):
//	crMethod(handle),
//	m_this(NULL),
//	m_netType(GameClient_Game),
//	m_stream(NULL)
//{
//
//}
//
//void JXJ::crJXJRecvCheckServerVersionMethod::inputParam( int i, void *param )
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//			m_netType = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void JXJ::crJXJRecvCheckServerVersionMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	default:
//		break;
//	}
//
//}
//
//void JXJ::crJXJRecvCheckServerVersionMethod::operator()( crHandle &handle )
//{
//	if (m_this)
//	{
//		if (m_netType == GameServer)
//		{
//			void *param;
//			crData *data = m_this->getDataClass();
//			data->getParam(WCHDATA_JXJServerVersion,param);
//			int serverVersion = *(int *)param;
//			if (serverVersion != crGlobalHandle::getVersion())
//			{
//				serverVersion = crGlobalHandle::getVersion();
//				data->inputParam(WCHDATA_JXJServerVersion,&serverVersion);
//
//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//				ref_ptr<crGameServerPlayerData> playerData 
//					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
//				if (playerData.valid() && gameServer)
//				{
//					ref_ptr<crStreamBuf> stream = new crStreamBuf;
//					stream->createBuf(1);
//					stream->_writeBool(true);
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvCheckServerVersion,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//				}
//			}
//		}
//		else if (m_netType == GameClient_Game)
//		{
//			bool willShow = false;
//			if (m_stream.valid())
//			{
//				willShow = m_stream->_readBool();
//
//				crData *data = crBrain::getInstance()->getDataClass();
//				if (data)
//				{
//					data->inputParam(WCHDATA_JXJShowVersion,&willShow);
//				}
//				
//			}
//		}
//	}
//}

/**************************************************************************

METHOD: crJXJAddMpMethod
 
**************************************************************************/

JXJ::crJXJReduceRTMpMethod::crJXJReduceRTMpMethod():
	m_this(NULL),
	m_reduce(0.0f)
{

}

JXJ::crJXJReduceRTMpMethod::crJXJReduceRTMpMethod( const crJXJReduceRTMpMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_reduce(0.0f)
{

}

void JXJ::crJXJReduceRTMpMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
			m_reduce = 0.0f;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			//m_damagePair = *(DamagePair *)(LOINT64(param64));
			m_reduce = *(float *)LOINT64(param64);
			//m_fireItem = (crInstanceItem*)(HIINT64(param64));
		}
		else
		{
			m_reduce = 0.0f;
			//m_fireItem = NULL;
		}
		break;
	}
}

void JXJ::crJXJReduceRTMpMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJReduceRTMpMethod::operator()( crHandle &handle )
{
	if (m_this /*&& m_fireItem.get()*/)
	{
		//float mp = 0.0f;
		//m_this->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&mp,NULL));
		crData *thisData = m_this->getDataClass();
		void *param;
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_RTMP,param);
		float *rtmp = (float*)param;
		if(!rtmp)
		{
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
			return;
		}
		//thisData->getParam(WCHDATA_ExtraRTMP,param);
		//float* extramp = (float*)param;
		//if(*extramp>0.0f)
		//{
		//	*extramp -= m_damagePair.first;
		//	if(*extramp<0.0f)
		//	{
		//		*rtmp += *extramp;
		//		*extramp = 0.0f;
		//	}
		//}
		//else if(*extramp<0.0f)
		//{
		//	*rtmp += *extramp;
		//	*extramp = 0.0f;
		//	*rtmp -= m_damagePair.first;
		//}
		//else
		{
			//*rtmp -= m_damagePair.first;
			*rtmp -= m_reduce;
		}
		if (*rtmp < 0.0f)
		{
			*rtmp = 0.0f;
		}
		float _rtmp = *rtmp;

		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		//将信息传给m_this以及其附近玩家
		unsigned char hittype = m_this->getItemtype();
		int hitid = m_this->getID();

		//unsigned char firetype = 0;
		//int fireid = 0;
		//if(m_fireItem.valid())
		//{
		//	firetype = m_fireItem->getItemtype();
		//	fireid = m_fireItem->getID();
		//}

		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crNetManager *netManager = sceneServerConductor->getNetManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			ref_ptr<crStreamBuf> damageStream = new crStreamBuf;
			damageStream->createBuf(18);
			//damageStream->_writeUChar(m_damagePair.second);
			//damageStream->_writeUChar(firetype);
			//damageStream->_writeInt(fireid);
			damageStream->_writeFloat(m_reduce);
			damageStream->_writeFloat(_rtmp);

			if(hittype == crInstanceItem::Role)
			{
				crPlayerEventPacket damagePacket;
				crPlayerEventPacket::buildRequestPacket(damagePacket,hitid,m_this,WCH_JXJRecvReduceRTMp	,damageStream.get());
				ref_ptr<crSceneServerPlayerData> hitPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(hitid));
				if (hitPlayerData.valid())
				{
					netManager->sendPacket(hitPlayerData->getPlayerConnectServerAddress(),damagePacket);
				}
				scene->sendPacketToItemNeighbor(m_this,damagePacket);
			}
			//else
			//{
			//	crItemEventPacket damagePacket;
			//	crItemEventPacket::buildRequestPacket(damagePacket,0,m_this,WCH_RecvDamage,damageStream.get());
			//	scene->sendPacketToItemNeighbor(m_this,damagePacket);
			//}
		}
	}
}

/**************************************************************************

METHOD: crJXJCureRTMpMethod
 
**************************************************************************/

JXJ::crJXJCureRTMpMethod::crJXJCureRTMpMethod():
	m_this(NULL),
	m_cureMP(0.0f)
{

}

JXJ::crJXJCureRTMpMethod::crJXJCureRTMpMethod( const crJXJCureRTMpMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_cureMP(0.0f)
{

}

void JXJ::crJXJCureRTMpMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_cureMP = 0.0f;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		{
			_crInt64 param64 = *(_crInt64*)param;
			if (LOINT64(param64))
			{
				m_cureMP = *(float *)(LOINT64(param64));
			}
			else 
				m_cureMP = 0;
		}
		break;
	}
}

void JXJ::crJXJCureRTMpMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCureRTMpMethod::operator()( crHandle &handle )
{
	crData *thisData = m_this->getDataClass();

	void *param;
	thisData->getParam(WCHDATA_JXJExtraRevertType_RD,param);
	unsigned char revertType = *(unsigned char *)param;
	if (revertType & ART_NotRevertMP)
	{
		return;
	}


	thisData->excHandle(MAKEINT64(WCH_LockData,1));
	thisData->getParam(WCHDATA_RTMP,param);
	float *rtmp = (float *)param;
	//if(!rtmp || *rtmp<=0)
	//{
	//	thisData->excHandle(MAKEINT64(WCH_LockData,0));
	//	return;
	//}
	*rtmp += m_cureMP;
	float maxMp = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMAXRTMPVal).c_str());
	if (*rtmp > maxMp)
	{
		*rtmp = maxMp;
	}
	thisData->excHandle(MAKEINT64(WCH_LockData,0));

	//将信息传给m_this以及其附近玩家
	unsigned char hittype = m_this->getItemtype();
	int hitid = m_this->getID();

	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	crNetManager *netManager = sceneServerConductor->getNetManager();
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(6);
	stream->_writeFloat(m_cureMP);
	stream->_writeFloat(*rtmp);
	crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
	if(hittype == crInstanceItem::Role)
	{
		crPlayerEventPacket packet;
		crPlayerEventPacket::buildRequestPacket(packet,hitid,m_this,WCH_JXJRecvCureMP	,stream.get());
		ref_ptr<crSceneServerPlayerData> hitPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(hitid));
		if(hitPlayerData.valid())
		{
			netManager->sendPacket(hitPlayerData->getPlayerConnectServerAddress(),packet);
		}
		scene->sendPacketToItemNeighbor(m_this,packet);
	}
	else
	{
		crItemEventPacket packet;
		crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_JXJRecvCureMP,stream.get());
		scene->sendPacketToItemNeighbor(m_this,packet);
	}
}

/**************************************************************************

METHOD: crJXJRecvRedeceRTMpMethod
 
**************************************************************************/

JXJ::crJXJRecvRedeceRTMpMethod::crJXJRecvRedeceRTMpMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvRedeceRTMpMethod::crJXJRecvRedeceRTMpMethod( const crJXJRecvRedeceRTMpMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{

}

void JXJ::crJXJRecvRedeceRTMpMethod::inputParam( int i, void *param )
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvRedeceRTMpMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvRedeceRTMpMethod::operator()( crHandle &handle )
{
	if(m_stream.valid() && crRunGameHandle::getInstance()->getRunningCode() == crRunGameHandle::NetGame)
	{
		float reduce = m_stream->_readFloat();
		float rtmp = m_stream->_readFloat();
		crData *data = m_this->getDataClass();
		data->inputParam(WCHDATA_RTMP,&rtmp);

		//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		//if (canvas.valid() && canvas->getDataClass())
		//{
		//	crData * canvasData = canvas->getDataClass();

		//}
		////客户端显示
		//if(reduce>0)
		//{
		//	//m_this->doEvent(WCH_JXJClientRecvDamage,MAKEINT64(-reduce,NULL));
		//}
	}
}

/**************************************************************************

METHOD: crJXJRecvCureMPMethod
 
**************************************************************************/

JXJ::crJXJRecvCureMPMethod::crJXJRecvCureMPMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvCureMPMethod::crJXJRecvCureMPMethod( const crJXJRecvCureMPMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}


void JXJ::crJXJRecvCureMPMethod::inputParam( int i, void *param )
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvCureMPMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRecvCureMPMethod::operator()( crHandle &handle )
{
	if(m_stream.valid() && crRunGameHandle::getInstance()->getRunningCode() == crRunGameHandle::NetGame)
	{
		float cureMP= m_stream->_readFloat();
		float rtmp = m_stream->_readFloat();
		crData *data = m_this->getDataClass();
		data->inputParam(WCHDATA_RTMP,&rtmp);
		//客户端显示
		//if(cureMP>0)
		//{
		//	//m_this->doEvent(WCH_JXJClientRecvDamage,MAKEINT64(-reduce,NULL));
		//}
	}
}

/**************************************************************************

METHOD: crJXJRevertUpdateMethod
 
**************************************************************************/

JXJ::crJXJRevertUpdateMethod::crJXJRevertUpdateMethod():
	m_this(NULL),
	m_dt(0.0f),
	m_interval(2.0f),
	m_time(0.0f)
{

}

JXJ::crJXJRevertUpdateMethod::crJXJRevertUpdateMethod( const crJXJRevertUpdateMethod & handle ):
	m_this(NULL),
	m_dt(0.0f),
	m_interval(2.0f),
	m_time(0.0f),
	crMethod(handle)
{

}

void JXJ::crJXJRevertUpdateMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = *((float*)(LOINT64(param64)));
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}

void JXJ::crJXJRevertUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJRevertUpdateMethod::operator()( crHandle &handle )
{
	m_time += m_dt;
	if(m_time>m_interval)
	{
		m_time = 0.0f;

		// 每多少回复士气
		float addMP = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAttackedAddMP).c_str());
		m_this->doEvent(MAKEINT64(WCH_JXJCureRTMp,0),MAKEINT64(&addMP,NULL));
	}
}

JXJ::crJXJRestartServerShowCanvasMethod::crJXJRestartServerShowCanvasMethod():
	m_netType(-1)
{

}

JXJ::crJXJRestartServerShowCanvasMethod::crJXJRestartServerShowCanvasMethod( const crJXJRestartServerShowCanvasMethod & handle ):
	crMethod(handle),
	m_netType(-1)
{

}

void JXJ::crJXJRestartServerShowCanvasMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJRestartServerShowCanvasMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_passwordCanvas = str;
		break;
	case 2:
		m_netType = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJRestartServerShowCanvasMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode> passwordCanvas = crFilterRenderManager::getInstance()->findCanvas(m_passwordCanvas);
	if (canvas.valid() && canvas->getDataClass() && passwordCanvas.valid())
	{
		crData * canvasData = canvas->getDataClass();
		canvasData->inputParam(WCHDATA_JXJGMRestartServerType	,&m_netType);
		crFilterRenderManager::getInstance()->doModal(passwordCanvas.get());
	}
}

/**************************************************************************

METHOD: crJXJLoadGameServerRankFilesMethod
 
**************************************************************************/
JXJ::crJXJLoadGameServerRankFilesMethod::crJXJLoadGameServerRankFilesMethod():
	m_this(NULL)
{

}

JXJ::crJXJLoadGameServerRankFilesMethod::crJXJLoadGameServerRankFilesMethod( const crJXJLoadGameServerRankFilesMethod & handle ):
	crMethod(handle),
	m_serverName (handle.m_serverName),
	m_exploitRank(handle.m_exploitRank),
	m_levelRank (handle.m_levelRank),
	m_govpostRank(handle.m_govpostRank),
	m_accomplishRank(handle.m_accomplishRank),
	m_troopsRank(handle.m_troopsRank),
	m_countrycontributeRank(handle.m_countrycontributeRank),
	m_manorlvRank(handle.m_manorlvRank),
	m_consortiaLvRank(handle.m_consortiaLvRank),
	m_onlinePlayeridTab(handle.m_onlinePlayeridTab)
{

}

void JXJ::crJXJLoadGameServerRankFilesMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_serverName = *((std::string *)(LOINT64(param64)));
		}
		else
		{
			m_serverName.clear();
		}
		break;
	};
}

void JXJ::crJXJLoadGameServerRankFilesMethod::addParam( int i, const std::string& str )
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());

	switch(_type) 
	{
	case WCHDATA_JXJExpRankingMap:
			m_exploitRank = relStr;
		break;
	case WCHDATA_JXJLevelRankingMap:
			m_levelRank = relStr;
		break;
	case WCHDATA_JXJGovpostRankingMap:
		m_govpostRank = relStr;
		break;
	case WCHDATA_JXJAccomplishRankingMap:
		m_accomplishRank = relStr;
		break;
	case WCHDATA_JXJTroopsRankingMap:
		m_troopsRank = relStr;
		break;
	case WCHDATA_JXJCountrycontributeRankingMap:
		m_countrycontributeRank = relStr;
		break;
	case WCHDATA_JXJManorlvRankingMap:
		m_manorlvRank = relStr;
		break;
	case WCHDATA_JXJConsortiaLvRank:
		m_consortiaLvRank = relStr;
		break;
	case WCHDATA_JXJServerCycleIni:
		m_strServerCycleRank = relStr;
		break;
	case WCHDATA_JXJOnlineEveryDayPlayeridSet	:
		m_onlinePlayeridTab = relStr;
		break;
	}
}

void JXJ::crJXJLoadGameServerRankFilesMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		void *param;
		crData * serverBrainData = m_this->getDataClass();
		serverBrainData->excHandle(MAKEINT64(WCH_LockData,1));

		{
			serverBrainData->getParam(WCHDATA_JXJExpRankingMap,param);
			ExpRankingMap & shiliExpRanking = *(ExpRankingMap *)param;
			std::string filepath = crArgumentParser::getFilePath(m_exploitRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_exploitRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					shiliExpRanking.insert(std::make_pair(rank->getExploit(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}

		{
			serverBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
			LevelRankingMap & lvRanking = *(LevelRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_levelRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_levelRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					lvRanking.insert(std::make_pair(rank->getLevel(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJGovpostRankingMap,param);
			GovpostRankingMap &govpostRanking = *(GovpostRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_govpostRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_govpostRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					govpostRanking.insert(std::make_pair(MAKEINT64(rank->getExploit(),rank->getGovpost()),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJAccomplishRankingMap,param);
			AccomplishRankingMap &accomplishRanking = *(AccomplishRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_accomplishRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_accomplishRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					accomplishRanking.insert(std::make_pair(rank->getAccomplishPoint(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJTroopsRankingMap,param);
			TroopsRankingMap & troopsRanking = *(TroopsRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_troopsRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_troopsRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					troopsRanking.insert(std::make_pair(rank->getTroops(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJCountrycontributeRankingMap,param);
			CountrycontributeRankingMap & countrycontributeRankingMap = *(CountrycontributeRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_countrycontributeRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_countrycontributeRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					countrycontributeRankingMap.insert(std::make_pair(rank->getCountrycontribute(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJManorlvRankingMap,param);
			ManorlvRankingMap &	manorlvRankingMap = *(ManorlvRankingMap *)param;

			std::string filepath = crArgumentParser::getFilePath(m_manorlvRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_manorlvRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<crPlayerRanking> rank;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					rank = new crPlayerRanking;
					col = 0;
					rank->setPlayerID(atoi((*table)(i,col++).c_str()));
					rank->setExploit(atoi((*table)(i,col++).c_str()));
					rank->setName((*table)(i,col++));
					rank->setGovpost(atoi((*table)(i,col++).c_str()));
					rank->setShiliID(atoi((*table)(i,col++).c_str()));
					rank->setLevel(atoi((*table)(i,col++).c_str()));
					rank->setNobility(atoi((*table)(i,col++).c_str()));
					rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
					rank->setTroops(atoi((*table)(i,col++).c_str()));
					rank->setManorlv(atoi((*table)(i,col++).c_str()));
					rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
					//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
					manorlvRankingMap.insert(std::make_pair(rank->getManorlv(),rank));
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}
		{
			serverBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
			ConsortialvRank & consortiaLvRank = *(ConsortialvRank *)param;

			std::string filepath = crArgumentParser::getFilePath(m_consortiaLvRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_consortiaLvRank);
			std::string relFileName = filepath + "/" + m_serverName + "/"+ filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if(table.valid())
			{
				ref_ptr<ConsortiaLvRankItem> rankItem;
				int rowcount = table->getRowCount();
				int col = 0;
				for(int i = 0; i<rowcount; ++i)
				{
					col = 0;
					rankItem = new ConsortiaLvRankItem(atoi((*table)(i,col++).c_str()));
					rankItem->m_groupName = (*table)(i,col++);
					rankItem->m_leaderName = (*table)(i,col++);
					rankItem->m_lv = atoi((*table)(i,col++).c_str());
					rankItem->m_memberCount = atoi((*table)(i,col++).c_str());
					rankItem->m_troops = atoi((*table)(i,col++).c_str());
					rankItem->m_exploit = atoi((*table)(i,col++).c_str());
					rankItem->m_shili = atoi((*table)(i,col++).c_str());
					consortiaLvRank.updateRank(rankItem->getKey(),rankItem);
				}
			}
			else
			{
				makeDirectoryForFile(relFileName);
			}
		}

		{
			std::string filepath = crArgumentParser::getFilePath(m_strServerCycleRank);
			std::string filename = crArgumentParser::getSimpleFileName(m_strServerCycleRank);
			std::string relFileName = filepath + "/" + m_serverName + "/" + filename;
			CRCore::ref_ptr<CRCore::crStreamBuf> streamBuf = new crStreamBuf;
			if(streamBuf->loadFromFile2(relFileName))
			{
				bool isCycle = streamBuf->_readBool();
				float cycleTime = streamBuf->_readFloat();

				serverBrainData->inputParam(WCHDATA_JXJServerCycleTimer, &cycleTime);
				serverBrainData->inputParam(WCHDATA_JXJIsServerCycling, &isCycle);
			}
		}
		{
			// 每日登陆的玩家表
			const time_t nowTime = time(NULL);
			char dateStr[128];
			strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));

			std::string filepath = crArgumentParser::getFilePath(m_onlinePlayeridTab);
			std::string filename = crArgumentParser::getSimpleFileName(m_onlinePlayeridTab);
			std::string filenameNoExtend = crArgumentParser::getFileNameEliminateExt(filename);

			filename = filenameNoExtend + "_" + std::string(dateStr) + ".tab";
			std::string relFileName = filepath + "/" + m_serverName + "/" + filename;
			ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
			if (table.valid())
			{
				void *param;
				serverBrainData->getParam(WCHDATA_JXJOnlineEveryDayPlayeridSet,param);
				OnlinePlayerSet * playerSet = (OnlinePlayerSet *)param;
				if (playerSet)
				{
					playerSet->clear();
					int rowcount = table->getRowCount();
					//int col = 0;
					for (int i = 0; i < rowcount; ++ i)
					{
						playerSet->insert( atoi((*table)(i,0).c_str()) );
					}
				}
			}
		}

		serverBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJQueryMyShiliDataMethod
 
**************************************************************************/
JXJ::crJXJQueryMyShiliDataMethod::crJXJQueryMyShiliDataMethod()
{

}

JXJ::crJXJQueryMyShiliDataMethod::crJXJQueryMyShiliDataMethod( const crJXJQueryMyShiliDataMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJQueryMyShiliDataMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJQueryMyShiliDataMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJQueryMyShiliDataMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJQueryMyShiliData,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/**************************************************************************

METHOD: crJXJRecvQueryMyShiliDataMethod
 
**************************************************************************/
JXJ::crJXJRecvQueryMyShiliDataMethod::crJXJRecvQueryMyShiliDataMethod():
	m_this(NULL)
{

}

JXJ::crJXJRecvQueryMyShiliDataMethod::crJXJRecvQueryMyShiliDataMethod( const crJXJRecvQueryMyShiliDataMethod& handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJRecvQueryMyShiliDataMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyShiliDataMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRecvQueryMyShiliDataMethod::operator()( crHandle &handle )
{
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(playerData.valid())
	{
		void *param;
		crData *data = m_this->getDataClass();

		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
		serverData->excHandle(MAKEINT64(WCH_LockData,1));

		do 
		{
			if (shiliid < c_startShiliID)
			{
				break;
			}

			serverData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
			crJXJShili *shili = (crJXJShili *)param;
			crJXJShiliData * shiliData = dynamic_cast<crJXJShiliData * > (shili->getDataClass());
			if (!shiliData) break;

			shiliData->excHandle(MAKEINT64(WCH_LockData,1)); 
			// 同步给客户端
			ref_ptr<crStreamBuf> shiliStream = new crStreamBuf;
			shiliData->excHandle(MAKEINT64(WCH_BuildPlayerStream,shiliStream.get()));

			shiliData->excHandle(MAKEINT64(WCH_LockData,0)); 

			ref_ptr<crGameServerPlayerData> serverPlayerData ; 
			serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (serverPlayerData.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJSYNShiliData,shiliStream.get());
				gameServer->getNetManager()->sendPacket(serverPlayerData ->getPlayerConnectServerAddress(),packet);
			}

		}while (0);
		serverData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJShowVersionCanvasMethod
 
**************************************************************************/
JXJ::crJXJShowVersionCanvasMethod::crJXJShowVersionCanvasMethod():
	m_this(NULL)
{

}

JXJ::crJXJShowVersionCanvasMethod::crJXJShowVersionCanvasMethod( const crJXJShowVersionCanvasMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_version(handle.m_version )
{

}

void JXJ::crJXJShowVersionCanvasMethod::inputParam( int i, void *param )
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
		m_this = (crBrain *)param;
		break;
	}
}

void JXJ::crJXJShowVersionCanvasMethod::addParam( int i, const std::string& str )
{
	switch (i )
	{
	case 0:
		m_version = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJShowVersionCanvasMethod::operator()( crHandle &handle )
{
	//if (m_this && m_this->getDataClass())
	//{
	//	crData * brainData = m_this->getDataClass();
	//	void *param;
	//	brainData->getParam(WCHDATA_JXJShowVersion,param);
	//	bool  willShow = *(bool *)param;
	//	if (willShow)
	//	{
	//		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_version);
	//		if (canvas.valid())
	//		{
	//			crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
	//			willShow = false;
	//			brainData->inputParam(WCHDATA_JXJShowVersion,&willShow);
	//		}
	//	}
	//}
}

/**************************************************************************

METHOD: crJXJCheckWetherChangeGroupLeaderMethod
 
**************************************************************************/

JXJ::crJXJCheckWetherChangeGroupLeaderMethod::crJXJCheckWetherChangeGroupLeaderMethod():
	m_this(NULL)
{

}

JXJ::crJXJCheckWetherChangeGroupLeaderMethod::crJXJCheckWetherChangeGroupLeaderMethod( const crJXJCheckWetherChangeGroupLeaderMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJCheckWetherChangeGroupLeaderMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCheckWetherChangeGroupLeaderMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCheckWetherChangeGroupLeaderMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		if (gsBrainData)
		{
			time_t nowTime = time(0);
			bool changed = false;
			void *param;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			crJXJConsortiaData *consortiaData = NULL;
			ConsortiaMemberMap * consortiaMemberMap = NULL;
			GroupLeaderVec *groupLeaderVec = NULL;
			ConsortiaMemberMap::iterator cmmItr;
			ConsortiaMemberMap::iterator cmmItr1;
			int id = 0;
			int newID = 0;
			int maxRolelvlID = 0;
			int maxRolelv = 0;
			for (ConsortiaMap::iterator cmItr = consortiamap->begin();
				cmItr != consortiamap->end(); ++ cmItr)
			{
				consortiaData = dynamic_cast<crJXJConsortiaData *>(cmItr->second->getDataClass());
				if (consortiaData)
				{
					consortiaData->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
					groupLeaderVec = (GroupLeaderVec *)param;
					id = 0;
					if (groupLeaderVec->empty() == false)
						id = (*groupLeaderVec)[0];

					consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					consortiaMemberMap = (ConsortiaMemberMap *) param;
					cmmItr = consortiaMemberMap->find(id);
					if ( cmmItr != consortiaMemberMap->end()
						&&(nowTime - cmmItr->second->getLoginTime()) / 86400 >= 3)
					{
						newID = 0;
						maxRolelvlID = 0;
						maxRolelv = 0;
						for (ConsortiaMemberMap::iterator cmm2Itr = consortiaMemberMap->begin();
							cmm2Itr != consortiaMemberMap->end(); ++ cmm2Itr)
						{
							if (cmm2Itr->second->getGroupPos() == JXJGroup_SecondLeader)
							{
								if ((nowTime - cmm2Itr->second->getLoginTime()) / 86400 < 3)
								{
									newID = cmm2Itr->first;
									changed = true;
									break;
								}
							}
						}
						for (ConsortiaMemberMap::iterator cmm2Itr = consortiaMemberMap->begin();
							cmm2Itr != consortiaMemberMap->end(); ++ cmm2Itr)
						{
							if ((nowTime - cmm2Itr->second->getLoginTime()) / 86400 < 1)
							{
								if (cmm2Itr->second->getLevel() > maxRolelv)
								{
									maxRolelv = cmm2Itr->second->getLevel();
									maxRolelvlID = cmm2Itr->first;
									changed = true;
								}
							}
						}
						newID = !newID ? maxRolelvlID : newID;
						cmmItr = consortiaMemberMap->find(id);
						cmmItr1 = consortiaMemberMap->find(newID);
						if (changed && id != newID && cmmItr != consortiaMemberMap->end() 
							&& cmmItr1 != consortiaMemberMap->end() && cmmItr->second.valid() && cmmItr1->second.valid())
						{
							cmmItr->second->setGroupPos(JXJGroup_Normal);

							char log[128];
							memset(log,0,sizeof(log));
							sprintf(log,"%d %s 由于三天未上线失去团长一职\0",id,cmmItr->second->getName().c_str());
							std::string logdata(log);
							GameLogData gamelog(Log_ChangeGroupLeader,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));

							cmmItr1->second->setGroupPos(JXJGroup_FirstLeader);
							memset(log,0,sizeof(log));
							sprintf(log,"%d %s 成为 新的军团长\0",newID,cmmItr1->second->getName().c_str());
							std::string logdata1(log);
							GameLogData gamelog1(Log_ChangeGroupLeader,logdata1);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(newID,&gamelog1));
						}
						cmItr->second->updateLeaderVec();
						cmItr->second->updateToRank(gsBrainData);

						bool modify = true;
						consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
					}
					int dismissTime = 0;
					int tmpdismisstime = cmItr->second->getDismissTime();
					if (consortiaMemberMap->size() < 10)
					{
						if (tmpdismisstime > 0)
						{
							dismissTime = tmpdismisstime;
						}
						else
						{
							dismissTime = 48 * 60 * 60;
						}
					}
					else
					{
						dismissTime = -1;
					}
					if (tmpdismisstime != dismissTime)
					{
						consortiaData->inputParam(WCHDATA_JXJConsortiaDismissTime, &dismissTime);
						bool modify = true;
						consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
					}
				}
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/**************************************************************************

METHOD: crJXJBuildPlayerDataSaveStreamTabMethod
 
**************************************************************************/

JXJ::crJXJBuildPlayerDataSaveStreamTabMethod::crJXJBuildPlayerDataSaveStreamTabMethod():
	m_this(NULL),
	m_player(NULL),
	m_gmID(0)
{

}

JXJ::crJXJBuildPlayerDataSaveStreamTabMethod::crJXJBuildPlayerDataSaveStreamTabMethod( const crJXJBuildPlayerDataSaveStreamTabMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_gmID(0)
{

}

void JXJ::crJXJBuildPlayerDataSaveStreamTabMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
			m_gmID = *(int *)(HIINT64(param64));
		}
		else
		{
			m_player = NULL;
			m_gmID = 0;
		}
		break;
	};
}

void JXJ::crJXJBuildPlayerDataSaveStreamTabMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJBuildPlayerDataSaveStreamTabMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass()
		&& m_player && m_player->getDataClass())
	{
		crData * gsBrainData = m_this->getDataClass();
		crData * playerData = m_player->getDataClass();
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		ref_ptr<crTableIO> playerDataTab = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowVec;
		{
			std::string tabPath = std::string("../JXJDB/GMQuery/")  +  crArgumentParser::appItoa(m_gmID) + "/";
			makeDirectory(tabPath);

			titleVec.resize(2);
			rowVec.resize(2);
			titleVec[0] = "信息";
			titleVec[1] = "值";
			playerDataTab->setTitleVec(titleVec);

			void *param;
			playerData->getParam(WCHDATA_PlayerSex,param);
			bool playerSex = *(bool *)param;
			rowVec[0] = "性别";
			rowVec[1] = crArgumentParser::appItoa(playerSex);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_PlayerIconID,param);
			unsigned char iconID = *(unsigned char *)param;
			rowVec[0] = "头像id";
			rowVec[1] = crArgumentParser::appItoa(iconID);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliID = *(unsigned char *)param;
			rowVec[0] = "国家";
			std::string country[] = {"魏国","蜀国","吴国"};

			if (shiliID >= c_startShiliID)
			{
				rowVec[1] = country[shiliID - c_startShiliID];
			}
			else
			{
				rowVec[1] = crArgumentParser::appItoa(shiliID);
			}

			playerDataTab->addData(rowVec);

			//playerData->getParam(WCHDATA_JXJHomeID,param);
			//playerData->getParam(WCHDATA_JXJChengchiID,param);
			
			playerData->getParam(WCHDATA_JXJGoldingot,param);
			int goldingot =  *(int *)param;
			rowVec[0] = "元宝";
			rowVec[1] = crArgumentParser::appItoa(goldingot);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold =  *(int *)param;
			rowVec[0] = "礼金";
			rowVec[1] = crArgumentParser::appItoa(giftgold);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCoppercash,param);
			int coppercash =  *(int *)param;
			rowVec[0] = "铜钱";
			rowVec[1] = crArgumentParser::appItoa(coppercash);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFood,param);
			int food =  *(int *)param;
			rowVec[0] = "粮食";
			rowVec[1] = crArgumentParser::appItoa(food);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJWood,param);
			int wood =  *(int *)param;
			rowVec[0] = "木材";
			rowVec[1] = crArgumentParser::appItoa(wood);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJIron,param);
			int iron =  *(int *)param;
			rowVec[0] = "铁";
			rowVec[1] = crArgumentParser::appItoa(iron);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJHorse,param);
			int horse =  *(int *)param;
			rowVec[0] = "马匹";
			rowVec[1] = crArgumentParser::appItoa(horse);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFuyalv,param);
			unsigned char fuyalv =  *(unsigned char *)param;
			rowVec[0] = "府衙等级";
			rowVec[1] = crArgumentParser::appItoa(fuyalv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJBingyinglv,param);
			unsigned char bingyinglv =  *(unsigned char *)param;
			rowVec[0] = "兵营等级";
			rowVec[1] = crArgumentParser::appItoa(bingyinglv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJJiusilv,param);
			unsigned char jiusilv =  *(unsigned char *)param;
			rowVec[0] = "酒肆等级";
			rowVec[1] = crArgumentParser::appItoa(jiusilv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJGongfanglv,param);
			unsigned char gongfanglv =  *(unsigned char *)param;
			rowVec[0] = "工坊等级";
			rowVec[1] = crArgumentParser::appItoa(gongfanglv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJShijilv,param);
			unsigned char shijilv =  *(unsigned char *)param;
			rowVec[0] = "市集等级";
			rowVec[1] = crArgumentParser::appItoa(shijilv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFukulv,param);
			unsigned char fukulv =  *(unsigned char *)param;
			rowVec[0] = "府库等级";
			rowVec[1] = crArgumentParser::appItoa(fukulv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJMingjulv,param);
			_crInt64 mingjulv =  *(_crInt64 *)param;
			std::vector<int > mingju;
			mingju.resize(8);
			{
				_crInt32 lo32 = LOINT64(mingjulv);
				_crInt32 hi32 = HIINT64(mingjulv);
				_crInt16 lo16 = LOINT32(lo32);
				_crInt16 hi16 = HIINT32(lo32);

				mingju[0] = (unsigned char)LOINT16(lo16);
				mingju[1] = (unsigned char)HIINT16(lo16);
				mingju[2] = (unsigned char)LOINT16(hi16);
				mingju[3] = (unsigned char)HIINT16(hi16);
				lo16 = LOINT32(hi32);
				hi16 = HIINT32(hi32);
				mingju[4] = (unsigned char)LOINT16(lo16);
				mingju[5] = (unsigned char)HIINT16(lo16);
				mingju[6] = (unsigned char)LOINT16(hi16);
				mingju[7] = (unsigned char)HIINT16(hi16);
			}
			rowVec[0] = "民居等级";
			rowVec[1] = crArgumentParser::appVectoa(mingju,'|');
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJNongtianlv,param);
			_crInt64 nongtianlv =  *(_crInt64 *)param;
			std::vector<int > nongtian;
			nongtian.resize(8);
			{
				_crInt32 lo32 = LOINT64(nongtianlv);
				_crInt32 hi32 = HIINT64(nongtianlv);
				_crInt16 lo16 = LOINT32(lo32);
				_crInt16 hi16 = HIINT32(lo32);

				nongtian[0] = (unsigned char)LOINT16(lo16);
				nongtian[1] = (unsigned char)HIINT16(lo16);
				nongtian[2] = (unsigned char)LOINT16(hi16);
				nongtian[3] = (unsigned char)HIINT16(hi16);
				lo16 = LOINT32(hi32);
				hi16 = HIINT32(hi32);
				nongtian[4] = (unsigned char)LOINT16(lo16);
				nongtian[5] = (unsigned char)HIINT16(lo16);
				nongtian[6] = (unsigned char)LOINT16(hi16);
				nongtian[7] = (unsigned char)HIINT16(hi16);
			}
			rowVec[0] = "农田等级";
			rowVec[1] = crArgumentParser::appVectoa(nongtian,'|');
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJMuchanglv,param);
			_crInt32 muchanglv  =  *(_crInt32 *)param;
			std::vector<int > muchang;
			muchang.resize(4);
			{
				_crInt16 lo16 = LOINT32(muchanglv);
				_crInt16 hi16 = HIINT32(muchanglv);

				muchang[0] = (unsigned char)LOINT16(lo16);
				muchang[1] = (unsigned char)HIINT16(lo16);
				muchang[2] = (unsigned char)LOINT16(hi16);
				muchang[3] = (unsigned char)HIINT16(hi16);
			}
			rowVec[0] = "牧场等级";
			rowVec[1] = crArgumentParser::appVectoa(muchang,'|');
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJLingdilv,param);
			_crInt32 lingdilv = *(_crInt32 *)param;
			std::vector<int > lingdi;
			lingdi.resize(4);
			{
				_crInt16 lo16 = LOINT32(lingdilv);
				_crInt16 hi16 = HIINT32(lingdilv);

				lingdi[0] = (unsigned char)LOINT16(lo16);
				lingdi[1] = (unsigned char)HIINT16(lo16);
				lingdi[2] = (unsigned char)LOINT16(hi16);
				lingdi[3] = (unsigned char)HIINT16(hi16);
			}
			rowVec[0] = "领地等级";
			rowVec[1] = crArgumentParser::appVectoa(lingdi,'|');
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJKuangshanlv,param);
			_crInt32 kuangshanlv = *(_crInt32 *)param;
			std::vector<int > kuangshan;
			kuangshan.resize(4);
			{
				_crInt16 lo16 = LOINT32(kuangshanlv);
				_crInt16 hi16 = HIINT32(kuangshanlv);

				kuangshan[0] = (unsigned char)LOINT16(lo16);
				kuangshan[1] = (unsigned char)HIINT16(lo16);
				kuangshan[2] = (unsigned char)LOINT16(hi16);
				kuangshan[3] = (unsigned char)HIINT16(hi16);
			}
			rowVec[0] = "矿山等级";
			rowVec[1] = crArgumentParser::appVectoa(kuangshan,'|');
			playerDataTab->addData(rowVec);
			
			//playerData->getParam(WCHDATA_UpdateTimer,param);
			
			playerData->getParam(WCHDATA_JXJBuildingTimes,param);
			crVector4i buildingTimes = *(crVector4i *)param;
			rowVec[0] = "建筑时间";
			rowVec[1] = crArgumentParser::appVectoa(buildingTimes);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJRecruitTimes,param);
			crVector4i recruitTimes = *(crVector4i *)param;
			rowVec[0] = "招募时间";
			rowVec[1] = crArgumentParser::appVectoa(recruitTimes);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJJunling,param);
			unsigned short junling = *(unsigned short *)param;
			rowVec[0] = "军令";
			rowVec[1] = crArgumentParser::appItoa(junling/100);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCountryBenefitFeteTimes,param);
			unsigned char feteTimes = *(unsigned char *)param;
			rowVec[0] = "祭祀次数";
			rowVec[1] = crArgumentParser::appItoa(feteTimes);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCountryBenefitEnrichType,param);
			unsigned char enrichTypes = *(unsigned char *)param;
			rowVec[0] = "富国类型";
			std::string enrich[] = {"小额投资","中等投资","大额投资"};
			if (enrichTypes >= JXJCountryBenefitType_smallInvest 
				&& enrichTypes <= JXJCountryBenefitType_largeInvest )
			{
				rowVec[1] = enrich[enrichTypes - JXJCountryBenefitType_smallInvest ];
			}
			else
			{
				rowVec[1] = crArgumentParser::appItoa(enrichTypes );
			}
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCountryBenefitEnrichTimes,param);
			unsigned char enrichTimes = *(unsigned char *)param;
			rowVec[0] = "富国次数";
			rowVec[1] = crArgumentParser::appItoa(enrichTimes);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *( int *)param;
			std::string groupName;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator cMitr = consortiamap->find(mygroupid);
			if (cMitr != consortiamap->end())
			{
				cMitr->second->getName(groupName);
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			rowVec[0] = "军团名称";
			rowVec[1] = groupName;
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			unsigned int goldcosthint = *(unsigned int *)param;
			rowVec[0] = "消费提示";
			rowVec[1] = crArgumentParser::appItoa(goldcosthint);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJOpenBagNum,param);
			unsigned char openbagnum = *(unsigned char *)param;
			rowVec[0] = "开启背包格子数";
			rowVec[1] = crArgumentParser::appItoa(openbagnum );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJLevyCount,param);
			unsigned char levyCount = *(unsigned char *)param;
			rowVec[0] = "征税次数";
			rowVec[1] = crArgumentParser::appItoa(levyCount);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJOnlineRewardCount,param);
			unsigned char onlineRewardCount = *(unsigned char *)param;
			rowVec[0] = "剩余在线奖励次数";
			rowVec[1] = crArgumentParser::appItoa(onlineRewardCount );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJOnlineRewardTimer,param);
			_crInt64 onlineRewardTimer = *(_crInt64 *)param;
			rowVec[0] = "上次领取时间";
			rowVec[1] = crArgumentParser::appI64toa(onlineRewardTimer );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJBuyJunlingCount,param);
			char buyJunlingCount = *(char *)param;
			rowVec[0] = "购买军令次数";
			rowVec[1] = crArgumentParser::appItoa(buyJunlingCount);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJBuyJunlingTimer,param);
			unsigned short buyJunlingTimer = *(unsigned short *)param;
			rowVec[0] = "每天第几次购买军令";
			rowVec[1] = crArgumentParser::appItoa(buyJunlingTimer );
			playerDataTab->addData(rowVec);

			//playerData->getParam(WCHDATA_JXJLuckyCardCopperCost,param);
			//unsigned char luckycardcoppercost = *(unsigned char *)param;
			//rowVec[0] = "抽取将卡铜钱增加次数";
			//rowVec[1] = crArgumentParser::appItoa(luckycardcoppercost );
			//playerDataTab->addData(rowVec);

			//playerData->getParam(WCHDATA_JXJLuckyCardGoldCost,param);
			//unsigned char luckycardgoldcost = *(unsigned char *)param;
			//rowVec[0] = "抽取将卡元宝增加次数";
			//rowVec[1] = crArgumentParser::appItoa(luckycardgoldcost );
			//playerDataTab->addData(rowVec);
			//
			//playerData->getParam(WCHDATA_JXJKnowLuckyCard,param);
			//unsigned char knowluckycard = *(unsigned char *)param;
			//rowVec[0] = "知天命元宝增加次数";
			//rowVec[1] = crArgumentParser::appItoa(knowluckycard );
			//playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJVipExp,param);
			int vipExp = *(int *)param;
			rowVec[0] = "军师等级成长值";
			rowVec[1] = crArgumentParser::appItoa(vipExp);
			playerDataTab->addData(rowVec);
			//
			playerData->getParam(WCHDATA_JXJCountryContribute,param);
			int countrycontribute = *(int *)param;
			rowVec[0] = "国家贡献";
			rowVec[1] = crArgumentParser::appItoa(countrycontribute );
			playerDataTab->addData(rowVec);
			
			////////////////////////////////////
			
			
			/////////////////////////////////////
			playerData->getParam(WCHDATA_JXJCardImpression,param);
			unsigned short cardimpression	= *(unsigned short *)param;
			rowVec[0] = "抽将卡好感度";
			rowVec[1] = crArgumentParser::appItoa(cardimpression);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJCardPieces,param);
			int cardpieces = *(int *)param;
			rowVec[0] = "将魂";
			rowVec[1] = crArgumentParser::appItoa(cardpieces );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
			unsigned char freeCounterinsurgency = *(unsigned char *)param;
			rowVec[0] = "免费平叛次数";
			rowVec[1] = crArgumentParser::appItoa(freeCounterinsurgency );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
			unsigned short refineEquipFreeCount = *(unsigned short *)param;
			rowVec[0] = "免费精炼次数";
			rowVec[1] = crArgumentParser::appItoa(refineEquipFreeCount/100);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJVIPNongTianShuiLi,param);
			unsigned char nongTianShuiLilv  = *(unsigned char *)param;
			rowVec[0] = "农田水利";
			rowVec[1] = crArgumentParser::appItoa(nongTianShuiLilv  );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPZhiShuZaoLin,param);
			unsigned char zhiShuZaoLinlv = *(unsigned char *)param;
			rowVec[0] = "植树造林";
			rowVec[1] = crArgumentParser::appItoa(zhiShuZaoLinlv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPYouZhiCaoChang,param);
			unsigned char youZhiCaoChanglv = *(unsigned char *)param;
			rowVec[0] = "优质草场";
			rowVec[1] = crArgumentParser::appItoa(youZhiCaoChanglv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPJingKuangShenWa,param);
			unsigned char jingKuangShenWalv = *(unsigned char *)param;
			rowVec[0] = "精矿深挖";
			rowVec[1] = crArgumentParser::appItoa(jingKuangShenWalv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJVIPGuLiMaoYi,param);
			unsigned char guLiMaoYilv = *(unsigned char *)param;
			rowVec[0] = "鼓励贸易";
			rowVec[1] = crArgumentParser::appItoa(guLiMaoYilv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJVIPZhengShouXinDe,param);
			unsigned char zhengShouXinDelv = *(unsigned char *)param;
			rowVec[0] = "征收心得";
			rowVec[1] = crArgumentParser::appItoa(zhengShouXinDelv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJVIPYanBingYueWu,param);
			unsigned char yanBingYueWulv = *(unsigned char *)param;
			rowVec[0] = "演兵阅武";
			rowVec[1] = crArgumentParser::appItoa(yanBingYueWulv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPXunBingXinDe,param);
			unsigned char xunBingXinDelv = *(unsigned char *)param;
			rowVec[0] = "训兵心得";
			rowVec[1] = crArgumentParser::appItoa(xunBingXinDelv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPZongMiaoJiSi,param);
			unsigned char zongMiaoJiSilv = *(unsigned char *)param;
			rowVec[0] = "宗庙祭祀";
			rowVec[1] = crArgumentParser::appItoa(zongMiaoJiSilv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJVIPJiSiXinDe,param);
			unsigned char jiSiXinDelv = *(unsigned char *)param;
			rowVec[0] = "祭祀心得";
			rowVec[1] = crArgumentParser::appItoa(jiSiXinDelv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_MarketBuyResourceCount,param);
			int marketBuyResourceMax = *(int *)param;
			rowVec[0] = "市集每日已购买量";
			rowVec[1] = crArgumentParser::appItoa(marketBuyResourceMax );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJRollCardFreeCount,param);
			unsigned short freeRollCardCount		= *(unsigned short *)param;
			rowVec[0] = "免费抽将次数";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardCount/100);
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJAutoSellCard,param);
			unsigned short autosellcard		= *(unsigned short *)param;
			rowVec[0] = "自动出售将卡";
			rowVec[1] = crArgumentParser::appItoa(autosellcard	);
			playerDataTab->addData(rowVec);
			
			///////新科技
			playerData->getParam(WCHDATA_JXJCardImpression2CritTechLv,param);
			unsigned char cardImpression2CritTechLv = *(unsigned char *)param;
			rowVec[0] = "好感度暴击科技2";
			rowVec[1] = crArgumentParser::appItoa(cardImpression2CritTechLv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJCardImpression4CritTechLv,param);
			unsigned char cardImpression4CritTechLv = *(unsigned char *)param;
			rowVec[0] = "好感度暴击科技4";
			rowVec[1] = crArgumentParser::appItoa(cardImpression4CritTechLv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCardImpression6CritTechLv,param);
			unsigned char cardImpression6CritTechLv = *(unsigned char *)param;
			rowVec[0] = "好感度暴击科技6";
			rowVec[1] = crArgumentParser::appItoa(cardImpression6CritTechLv );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJCardImpression10CritTechLv,param);
			unsigned char cardImpression10CritTechLv = *(unsigned char *)param;
			rowVec[0] = "好感度暴击科技10";
			rowVec[1] = crArgumentParser::appItoa(cardImpression10CritTechLv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCardTechLv,param);
			unsigned char freeRollCardTechL= *(unsigned char *)param;
			rowVec[0] = "免费刷将科技16";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardTechL);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJOrangeCardUpgradeTechLv,param);
			unsigned char orangeCardUpgradeTechLv = *(unsigned char *)param;
			rowVec[0] = "橙卡升星科技";
			rowVec[1] = crArgumentParser::appItoa(orangeCardUpgradeTechLv );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJCardShopOpenTechLv,param);
			unsigned char cardShopOpenTechLv= *(unsigned char *)param;
			rowVec[0] = "将卡商店开启科技";
			rowVec[1] = crArgumentParser::appItoa(cardShopOpenTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJMakeEquipTechLv,param);
			unsigned char makeEquipTechLv= *(unsigned char *)param;
			rowVec[0] = "装备打造科技";
			rowVec[1] = crArgumentParser::appItoa(makeEquipTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJEquipRefineTechLv,param);
			unsigned char equipRefineTechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技";
			rowVec[1] = crArgumentParser::appItoa(equipRefineTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJEquipAttachTechLv,param);
			unsigned char equipAttachTechLv= *(unsigned char *)param;
			rowVec[0] = "附炼科技";
			rowVec[1] = crArgumentParser::appItoa(equipAttachTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJSmeltEquipTechLv,param);
			unsigned char smeltEquipTechLv= *(unsigned char *)param;
			rowVec[0] = "开启装备熔炼";
			rowVec[1] = crArgumentParser::appItoa(smeltEquipTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJSmeltMaterialTechLv,param);
			unsigned char smeltMaterialTechLv= *(unsigned char *)param;
			rowVec[0] = "开启材料熔炼";
			rowVec[1] = crArgumentParser::appItoa(smeltMaterialTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJSmeltStoneTechLv,param);
			unsigned char smeltStoneTechLv= *(unsigned char *)param;
			rowVec[0] = "开启八卦石熔炼";
			rowVec[1] = crArgumentParser::appItoa(smeltStoneTechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeCount1TechLv,param);
			unsigned char equipRefineMaxCount1TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技免费次数12";
			rowVec[1] = crArgumentParser::appItoa(equipRefineMaxCount1TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeCount2TechLv,param);
			unsigned char equipRefineMaxCount2TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技免费次数16";
			rowVec[1] = crArgumentParser::appItoa(equipRefineMaxCount2TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeCount3TechLv,param);
			unsigned char equipRefineMaxCount3TechLv = *(unsigned char *)param;
			rowVec[0] = "装备精炼科技免费次数20";
			rowVec[1] = crArgumentParser::appItoa(equipRefineMaxCount3TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeCount4TechLv,param);
			unsigned char equipRefineMaxCount4TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技免费次数24";
			rowVec[1] = crArgumentParser::appItoa(equipRefineMaxCount4TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeProduce1TechLv,param);
			unsigned char equipRefineProduce1TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技回复50";
			rowVec[1] = crArgumentParser::appItoa(equipRefineProduce1TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeProduce2TechLv,param);
			unsigned char equipRefineProduce2TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技回复100";
			rowVec[1] = crArgumentParser::appItoa(equipRefineProduce2TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeProduce3TechLv,param);
			unsigned char equipRefineProduce3TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技回复150";
			rowVec[1] = crArgumentParser::appItoa(equipRefineProduce3TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJAddRefineFreeProduce4TechLv,param);
			unsigned char equipRefineProduce4TechLv= *(unsigned char *)param;
			rowVec[0] = "装备精炼科技回复200";
			rowVec[1] = crArgumentParser::appItoa(equipRefineProduce4TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCard2TechLv,param);
			unsigned char freeRollCard2TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将科技20";
			rowVec[1] = crArgumentParser::appItoa(freeRollCard2TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCard3TechLv,param);
			unsigned char freeRollCard3TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将科技24";
			rowVec[1] = crArgumentParser::appItoa(freeRollCard3TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCardProduce1TechLv,param);
			unsigned char freeRollCardProduce1TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将回复科技1";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardProduce1TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCardProduce2TechLv,param);
			unsigned char freeRollCardProduce2TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将回复科技2";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardProduce2TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCardProduce3TechLv,param);
			unsigned char freeRollCardProduce3TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将回复科技3";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardProduce3TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFreeRollCardProduce4TechLv,param);
			unsigned char freeRollCardProduce4TechLv= *(unsigned char *)param;
			rowVec[0] = "免费刷将回复科技4";
			rowVec[1] = crArgumentParser::appItoa(freeRollCardProduce4TechLv);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFuKuMaxTechLv,param);
			unsigned char fuKuMaxTechLv= *(unsigned char *)param;
			rowVec[0] = "府库最大值科技20";
			rowVec[1] = crArgumentParser::appItoa(fuKuMaxTechLv);
			playerDataTab->addData(rowVec);
			

			//////////////////////////////////////////////////////////////////////////////////
			playerData->getParam(WCHDATA_JXJOpenSaveFormationInfo,param);
			unsigned char opensavedformationifo= *(unsigned char *)param;
			rowVec[0] = "编队保存切页开启";
			rowVec[1] = crArgumentParser::appItoa(opensavedformationifo);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
			unsigned char changeShiliTimes= *(unsigned char *)param;
			rowVec[0] = "主动叛国次数";
			rowVec[1] = crArgumentParser::appItoa(changeShiliTimes);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJSeriesLandDay,param);
			unsigned short seriesLandDays= *(unsigned short *)param;
			rowVec[0] = "连续登录时间";
			rowVec[1] = crArgumentParser::appItoa(seriesLandDays);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJLandRewardGetCount,param);
			unsigned char landRewardGetCount= *(unsigned char *)param;
			rowVec[0] = "登陆抽奖次数";
			rowVec[1] = crArgumentParser::appItoa(landRewardGetCount);
			playerDataTab->addData(rowVec);
			

			playerData->getParam(WCHDATA_JXJGroupContributeiType,param);
			unsigned char groupContributeiType= *(unsigned char *)param;
			rowVec[0] = "军团捐献类型";
			rowVec[1] = crArgumentParser::appItoa(groupContributeiType);
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJGroupContributeiTimes,param);
			unsigned char groupContributeiTimes = *(unsigned char *)param;
			rowVec[0] = "军团捐献次数";
			rowVec[1] = crArgumentParser::appItoa(groupContributeiTimes );
			playerDataTab->addData(rowVec);

			playerData->getParam(WCHDATA_JXJChatChannelSetInfo,param);
			BitSet64 *chatChannelSetInfo = (BitSet64 *)param;
			rowVec[0] = "玩家聊天信息设置信息";
			rowVec[1] = crArgumentParser::appI64toa(chatChannelSetInfo->to_ullong() );
			playerDataTab->addData(rowVec);
			

			////////////////////////////////////////////////////
			//跳过新手引导
			playerData->getParam(WCHDATA_JXJSkipNewHand,param);
			bool skipnewhand = *(bool *)param;
			rowVec[0] = "是否在轮回";
			rowVec[1] = crArgumentParser::appItoa(skipnewhand);
			playerDataTab->addData(rowVec);
			
			//保存编队索引
			playerData->getParam(WCHDATA_JXJSavedFormationIndex,param);
			unsigned char savedformationindex = *(unsigned char *)param;
			rowVec[0] = "保存编队索引";
			rowVec[1] = crArgumentParser::appItoa(savedformationindex );
			playerDataTab->addData(rowVec);
			
			//科技图标是否闪烁
			playerData->getParam(WCHDATA_JXJIfTechBtnNew,param);
			unsigned char iftechbtnnew= *(unsigned char *)param;
			rowVec[0] = "科技图标是否闪烁";
			rowVec[1] = crArgumentParser::appItoa(iftechbtnnew);
			playerDataTab->addData(rowVec);
			
			//府库最大值科技
			playerData->getParam(WCHDATA_JXJFuKuMaxTech2Lv,param);
			unsigned char fuKuMaxTechLv2 = *(unsigned char *)param;
			rowVec[0] = "府库最大值科技5";
			rowVec[1] = crArgumentParser::appItoa(fuKuMaxTechLv2 );
			playerDataTab->addData(rowVec);
			
			playerData->getParam(WCHDATA_JXJFuKuMaxTech3Lv,param);
			unsigned char fuKuMaxTechLv3 = *(unsigned char *)param;
			rowVec[0] = "府库最大值科技10";
			rowVec[1] = crArgumentParser::appItoa(fuKuMaxTechLv3 );
			playerDataTab->addData(rowVec);

			playerDataTab->saveToFileNoCook(tabPath + "玩家基本数据.tab");
			
			/*
					
			////预备部队
			std::vector<std::string> v_str;
			//sprintf(buf,"%d,%d\0",WCHDATA_JXJMaxTroops,(int)m_maxtroops);
			//cfg_script.Add("Param",buf);
			if(!m_troopsMap.empty())
			{
				cfg_script.Add("TroopsMap");
				cfg_script.Push("TroopsMap");
				v_str.resize(2);
				for( TroopsMap::iterator itr = m_troopsMap.begin();
					itr != m_troopsMap.end();
					++itr )
				{
					if(itr->second>0)
					{
						v_str[0] = crArgumentParser::appItoa(itr->first);
						v_str[1] = crArgumentParser::appItoa(itr->second);
						cfg_script.Add("Param",v_str);
					}
				}
				cfg_script.Pop();
			}
			if(!m_troopsEquipMap.empty())
			{
				cfg_script.Add("TroopsEquipMap");
				cfg_script.Push("TroopsEquipMap");
				v_str.resize(2);
				for( TroopsEquipMap::iterator itr = m_troopsEquipMap.begin();
					itr != m_troopsEquipMap.end();
					++itr )
				{
					if(itr->second>0)
					{
						v_str[0] = crArgumentParser::appItoa(itr->first);
						v_str[1] = crArgumentParser::appItoa(itr->second);
						cfg_script.Add("Param",v_str);
					}
				}
				cfg_script.Pop();
			}
			*/

			ref_ptr<crTableIO> bagDataTab = new crTableIO;
			/////////////////背包已开格子数
			/////////背包列表
			titleVec.resize(4);
			rowVec.resize(4);
			titleVec[0] = "ID";
			titleVec[1] = "名称";
			titleVec[2] = "数量";
			titleVec[3] = "magic";


			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int itemIDIdx = itemtab->getTitleIndex("itemid");
			int nameIdx = itemtab->getTitleIndex("name");

			bagDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec &itemBagVec = * (JXJItemBagVec *)param;
			if(!itemBagVec.empty())
			{
				bool haveItem = false;
				int openbagnum = itemBagVec.size();
				for( int i = 0; i < openbagnum; i++ )
				{
					haveItem = false;
					if((itemBagVec)[i].valid() && (itemBagVec)[i]->getItemCount()>0)
					{
						rowVec[0] = crArgumentParser::appItoa((itemBagVec)[i]->getItemID());
						if (itemIDIdx >= 0 && itemtab.valid() && itemtab->queryOneRecord(itemIDIdx,rowVec[0],record) >= 0)
						{
							rowVec[1] = record[nameIdx];
							rowVec[2] = crArgumentParser::appItoa((itemBagVec)[i]->getItemCount());
							rowVec[3] = crArgumentParser::appItoa((itemBagVec)[i]->getEquipMagic());
							haveItem = true;
						}
					}
					if (!haveItem)
					{
						rowVec[0] = "";
						rowVec[1] = "";
						rowVec[2] = "";
						rowVec[3] = "";
					}
					bagDataTab->addData(rowVec);
				}
			}

			bagDataTab->saveToFileNoCook(tabPath + "背包列表.tab");
			// bag end

			ref_ptr<crTableIO> friendsDataTab = new crTableIO;

			titleVec.resize(4);
			rowVec.resize(4);
			titleVec[0] = "playerID";
			titleVec[1] = "name";
			titleVec[2] = "level";
			titleVec[3] = "iconID";
			friendsDataTab->setTitleVec(titleVec);

			playerData->getParam(WCHDATA_JXJFriendList,param);
			FriendVec &friendList = *(FriendVec *)param;
			//////好友列表
			if(!friendList.empty())
			{
				for( FriendVec::iterator itr = friendList.begin();
					itr != friendList.end();
					++itr )
				{
					rowVec[0] = crArgumentParser::appItoa((*itr)->getPlayerID());
					rowVec[1] = (*itr)->getName();
					rowVec[2] = crArgumentParser::appItoa((*itr)->getLevel());
					rowVec[3] = crArgumentParser::appItoa((*itr)->getIconID());
					friendsDataTab->addData(rowVec);
				}
			}
			friendsDataTab->saveToFileNoCook(tabPath + "好友列表.tab");
			// friend end

			ref_ptr<crTableIO> saleListDataTab = new crTableIO;

			titleVec.resize(10);
			rowVec.resize(10);
			titleVec[0] = "SaleListID";
			titleVec[1] = "GoodsType";
			titleVec[2] = "GoodsID";
			titleVec[3] = "Price";
			titleVec[4] = "Count";
			titleVec[5] = "prepareTimer";
			titleVec[6] = "SaleID";
			titleVec[7] = "saleTimer";
			titleVec[8] = "SaleState";
			titleVec[9] = "GainCash";
			saleListDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJSaleList,param);
			SaleList & saleList = *(SaleList *)param;
			////军令购买次数
			//寄售         
			if(!saleList.empty())
			{
				for( SaleList::iterator itr = saleList.begin();
					itr != saleList.end();
					++itr )
				{
					if((*itr)->getSaleState() != Sale_Canceled)
					{
						rowVec[0] = crArgumentParser::appItoa((*itr)->getSaleListID());
						rowVec[1] = crArgumentParser::appItoa((*itr)->getGoodsType());
						rowVec[2] = crArgumentParser::appItoa((*itr)->getGoodsID());
						rowVec[3] = crArgumentParser::appItoa((*itr)->getPrice());
						rowVec[4] = crArgumentParser::appItoa((*itr)->getCount());
						rowVec[5] = crArgumentParser::appItoa((*itr)->prepareTimer());
						rowVec[6] = crArgumentParser::appItoa((*itr)->getSaleID());
						rowVec[7] = crArgumentParser::appItoa((*itr)->saleTimer());
						rowVec[8] = crArgumentParser::appItoa((*itr)->getSaleState());
						rowVec[9] = crArgumentParser::appItoa((*itr)->getGaincash());
						saleListDataTab->addData(rowVec);
					}
				}
			}
			saleListDataTab->saveToFileNoCook(tabPath + "寄售.tab");

			// salelist end
			//////最近出售列表
			ref_ptr<crTableIO> saleItemListDataTab = new crTableIO;

			titleVec.resize(4);
			rowVec.resize(4);
			titleVec[0] = "ItemID";
			titleVec[1] = "ItemCount";
			titleVec[2] = "Time";
			titleVec[3] = "EquipMagic";
			saleItemListDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJSaleItemList,param);
			JXJSaleItemList &saleitemlist = *(JXJSaleItemList *)param;
			if(!saleitemlist.empty())
			{
				for( JXJSaleItemList::iterator itr = saleitemlist.begin();
					itr != saleitemlist.end();
					++itr )
				{
					rowVec[0] = crArgumentParser::appItoa((*itr)->getItemID());
					rowVec[1] = crArgumentParser::appItoa((int)(*itr)->getItemCount());
					rowVec[2] = crArgumentParser::appItoa((int)(*itr)->getTime());
					rowVec[3] = crArgumentParser::appItoa((*itr)->getEquipMagic());
					saleItemListDataTab->addData(rowVec);
				}
			}

			saleItemListDataTab->saveToFileNoCook(tabPath + "最近出售列表.tab");
			// saleItemList end
			/////////将卡背包列表

			ref_ptr<crTableIO> JKBagDataTab = new crTableIO;

			titleVec.resize(2);
			rowVec.resize(2);
			titleVec[0] = "ItemID";
			titleVec[1] = "ItemCount";
			JKBagDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec & jiangkabagvec = *(JXJItemBagVec *)param;
			
			if(!jiangkabagvec.empty())
			{
				int bagcount = jiangkabagvec.size();
				for( int i = 0;i<bagcount ; i++)
				{
					if(jiangkabagvec[i].valid() && jiangkabagvec[i]->getItemCount()>0)
					{
						rowVec[0] = crArgumentParser::appItoa(jiangkabagvec[i]->getItemID());
						rowVec[1] = crArgumentParser::appItoa(jiangkabagvec[i]->getItemCount());
						//v_str[2] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getEquipMagic());
					}
					else
					{
						rowVec[0] = "";
						rowVec[1] = "";
						//v_str[2] = "";
					}
					JKBagDataTab->addData(rowVec);
				}
			}

			JKBagDataTab->saveToFileNoCook(tabPath + "将卡背包列表.tab");
			// jkbag end
			/////////将卡派遣列表
			ref_ptr<crTableIO> appointJKBagDataTab = new crTableIO;

			titleVec.resize(2);
			rowVec.resize(2);
			titleVec[0] = "ItemID";
			titleVec[1] = "ValidTime";
			appointJKBagDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
			JXJAppointCardVec & appointjiangkabagvec = *(JXJAppointCardVec *)param;

			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLV = *(unsigned char *)param;
			
			unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,vipLV).c_str()));
			if(!appointjiangkabagvec.empty())
			{
				for( int i = 0;i<bagnummax2 ; i++)
				{
					if(appointjiangkabagvec[i].valid())
					{
						rowVec[0] = crArgumentParser::appItoa(appointjiangkabagvec[i]->getItemID());
						rowVec[1] = crArgumentParser::appItoa(appointjiangkabagvec[i]->getValidTime());
					}
					else
					{
						rowVec[0] = "";
						rowVec[1] = "";
					}
					appointJKBagDataTab->addData(rowVec);
				}
			}

			appointJKBagDataTab->saveToFileNoCook(tabPath + "将卡派遣列表.tab");
			/////////FubenProgressMap
			ref_ptr<crTableIO> fuBenProgressDataTab = new crTableIO;

			titleVec.resize(4);
			rowVec.resize(4);
			titleVec[0] = "fuBenID";
			titleVec[1] = "fuBenProgress";
			titleVec[2] = "complete";
			titleVec[3] = "starCount";
			fuBenProgressDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap & fubenProgressMap = *(FubenProgressMap *)param;

			if(!fubenProgressMap.empty())
			{
				for( FubenProgressMap::iterator itr = fubenProgressMap.begin();
					itr != fubenProgressMap.end();
					++itr )
				{
					rowVec[0] = crArgumentParser::appItoa(itr->first);
					rowVec[1] = crArgumentParser::appItoa(itr->second->getFubenProgress());
					rowVec[2] = crArgumentParser::appItoa(itr->second->getComplete());
					rowVec[3] = crArgumentParser::appItoa(itr->second->getStarCount());
					fuBenProgressDataTab->addData(rowVec);
				}
			}
			fuBenProgressDataTab->saveToFileNoCook(tabPath + "副本进度.tab");

			// fuben end
			/////////ExtraProduceMap
			ref_ptr<crTableIO> extraProduceDataTab = new crTableIO;

			titleVec.resize(3);
			rowVec.resize(3);
			titleVec[0] = "ID";
			titleVec[1] = "timer";
			titleVec[2] = "percent";
			extraProduceDataTab ->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJExtraProduceMap,param);
			ExtraProduceMap &extraProduceMap = *(ExtraProduceMap *)param;
			if(!extraProduceMap.empty())
			{
				//unsigned char extramaxcount = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJExtraProduceMaxCount).c_str()));
				for( ExtraProduceMap::iterator itr = extraProduceMap.begin();
					itr != extraProduceMap.end();
					++itr)
				{
					rowVec[0] = crArgumentParser::appItoa(itr->first);
					rowVec[1] = crArgumentParser::appItoa(itr->second->timer());
					rowVec[2] = crArgumentParser::appItoa(itr->second->getPercent());
					extraProduceDataTab->addData(rowVec);
				}
			}
			extraProduceDataTab->saveToFileNoCook(tabPath + "额外产出.tab");
			// extraproduce end

			/////////军备map（兵种科技）
			ref_ptr<crTableIO> troopstechDataTab = new crTableIO;

			titleVec.resize(2);
			rowVec.resize(2);
			titleVec[0] = "军备ID";
			titleVec[1] = "军备等级";
			troopstechDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJTroopsTechMap,param);
			TroopsTechMap & troopstechmap = *(TroopsTechMap *)param;

			if(!troopstechmap.empty())
			{
				for(TroopsTechMap::iterator itr = troopstechmap.begin();
					itr != troopstechmap.end();
					++itr)
				{
					rowVec[0] = crArgumentParser::appItoa(itr->first);
					rowVec[1] = crArgumentParser::appItoa(itr->second);
					troopstechDataTab->addData(rowVec);
				}
			}
			troopstechDataTab->saveToFileNoCook(tabPath + "兵种科技.tab");

			// troopstech end

			//////黑名单

			ref_ptr<crTableIO> blackNameDataTab = new crTableIO;

			titleVec.resize(1);
			rowVec.resize(1);
			titleVec[0] = "name";
			blackNameDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJBlackNameSet,param);
			BlackNameSet& blacknameset= *(BlackNameSet*)param;
			if(!blacknameset.empty())
			{
				for(BlackNameSet::iterator itr = blacknameset.begin();
					itr != blacknameset.end();
					++itr)
				{
					rowVec[0] = (*itr);
					blackNameDataTab->addData(rowVec);
				}
			}
			blackNameDataTab->saveToFileNoCook(tabPath + "黑名单.tab");
			// black name end

			////////研发中的科技map
			ref_ptr<crTableIO> techdevelopDataTab = new crTableIO;

			titleVec.resize(2);
			rowVec.resize(2);
			titleVec[0] = "科技ID";
			titleVec[1] = "科技倒计时";
			techdevelopDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJTechDevelopMap,param);
			TechDevelopMap & techdevelopmap = *(TechDevelopMap *)param;

			if(!techdevelopmap.empty())
			{
				for(TechDevelopMap::iterator itr = techdevelopmap.begin();
					itr != techdevelopmap.end();
					++itr)
				{
					rowVec[0] = crArgumentParser::appItoa(itr->first);
					rowVec[1] = crArgumentParser::appItoa(itr->second);//???Int??
					techdevelopDataTab->addData(rowVec);
				}
			}
			techdevelopDataTab->saveToFileNoCook(tabPath + "研发中的科技.tab");
			// tech end
			//说服队列
			//ref_ptr<crTableIO> persuadeInfoDeqDataTab = new crTableIO;

			//titleVec.resize(4);
			//rowVec.resize(4);
			//titleVec[0] = "副本ID";
			//titleVec[1] = "将卡ID";
			//titleVec[2] = "time";
			//titleVec[3] = "level";
			//persuadeInfoDeqDataTab ->setTitleVec(titleVec);
			//playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
			//PersuadeInfoDeq &persuadeInfoDeq = *(PersuadeInfoDeq *)param;
			//if(!persuadeInfoDeq.empty())
			//{
			//	for(PersuadeInfoDeq::iterator itr = persuadeInfoDeq.begin();
			//		itr != persuadeInfoDeq.end();
			//		++itr)
			//	{
			//		rowVec[0] = crArgumentParser::appItoa(itr->get()->getFubenId());
			//		rowVec[1] = crArgumentParser::appItoa(itr->get()->getJKId());
			//		rowVec[2] = crArgumentParser::appI64toa(itr->get()->getTime());
			//		rowVec[3] = crArgumentParser::appItoa(itr->get()->getLevel());
			//		persuadeInfoDeqDataTab->addData(rowVec);
			//	}
			//}
			//persuadeInfoDeqDataTab->saveToFileNoCook(tabPath + "说服队列.tab");
			//////SavedFormationInfo
			ref_ptr<crTableIO> savedformationinfovecDataTab = new crTableIO;

			titleVec.resize(4);
			rowVec.resize(4);
			titleVec[0] = "AbstractID";
			titleVec[1] = "Count";
			titleVec[2] = "Equips0";
			titleVec[3] = "Equip1";
			savedformationinfovecDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
			SavedFormationInfoVec & savedformationinfovec = *(SavedFormationInfoVec *)param;
			if(!savedformationinfovec.empty())
			{
				for( SavedFormationInfoVec::iterator itr = savedformationinfovec.begin();
					itr != savedformationinfovec.end();
					++itr )
				{
					for( FormationInfoVec::iterator fitr = itr->second.begin();
						fitr != itr->second.end();
						++fitr )
					{
						rowVec[0] = crArgumentParser::appItoa((*fitr)->getAbstractID());
						rowVec[1] = crArgumentParser::appItoa((*fitr)->getCount());
						rowVec[2] = crArgumentParser::appItoa((*fitr)->getEquips()[0]);
						rowVec[3] = crArgumentParser::appItoa((*fitr)->getEquips()[1]);
						savedformationinfovecDataTab->addData(rowVec);
					}
				}
			}
			savedformationinfovecDataTab->saveToFileNoCook(tabPath + "编队.tab");

			// savedformationinfo end

			////////轮回福利map

			ref_ptr<crTableIO> lunhuiRewardMapDataTab = new crTableIO;

			titleVec.resize(3);
			rowVec.resize(3);
			titleVec[0] = "level";
			titleVec[1] = "是否领取";
			titleVec[2] = "奖励列表";
			lunhuiRewardMapDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJLunHuiRewardMap,param);
			LunHuiRewardMap & lunhuiRewardMap = *(LunHuiRewardMap *)param;

			if(!lunhuiRewardMap.empty())
			{
				for(LunHuiRewardMap::iterator itr = lunhuiRewardMap.begin();
					itr != lunhuiRewardMap.end();
					++itr)
				{
					rowVec[1] = crArgumentParser::appItoa(itr->first);
					rowVec[2] = crArgumentParser::appItoa(itr->second.first);
					rowVec[3] = crArgumentParser::appVectoa(itr->second.second);				
					lunhuiRewardMapDataTab->addData(rowVec);
				}
			}
			lunhuiRewardMapDataTab->saveToFileNoCook(tabPath + "轮回福利.tab");
			////////////////////////////////////////////////
			////////////////////////////////// 玩家购买头像列表 ////////////////////////////////////////

			//ref_ptr<crTableIO> playerBuyIconVecDataTab = new crTableIO;

			//titleVec.resize(1);
			//rowVec.resize(1);
			//titleVec[0] = "level";
			//playerBuyIconVecDataTab->setTitleVec(titleVec);
			//playerData->getParam(WCHDATA_JXJLunHuiRewardMap,param);
			//
			//if(!m_playerBuyIconVec.empty())
			//{
			//	int openbagnum = m_playerBuyIconVec.size();
			//	cfg_script.Add("JXJPlayerBuyIconVec");
			//	cfg_script.Push("JXJPlayerBuyIconVec");
			//	for( int i = 0; i < openbagnum; i++ )
			//	{
			//		cfg_script.Add("Param", m_playerBuyIconVec[i]);
			//	}
			//	cfg_script.Pop();
			//}
			//////////////投票记录

			ref_ptr<crTableIO> selectLeaderIdSetDataTab = new crTableIO;

			titleVec.resize(1);
			rowVec.resize(1);
			titleVec[0] = "playerID";
			selectLeaderIdSetDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJSelectLeaderSet,param);
			SelectLeaderIdSet & selectLeaderIdSet = *(SelectLeaderIdSet *)param;
			if (!selectLeaderIdSet.empty())
			{
				SelectLeaderIdSet::iterator slsItr = selectLeaderIdSet.begin();
				for (;slsItr != selectLeaderIdSet.end(); ++ slsItr)
				{
					rowVec[0] = crArgumentParser::appItoa(*slsItr);
					selectLeaderIdSetDataTab->addData(rowVec);
				}
			}
			selectLeaderIdSetDataTab->saveToFileNoCook(tabPath + "投票记录.tab");

			// selectLeaderIdSetDataTab end

			//MyGroupApplySet
			ref_ptr<crTableIO> myGroupApplySetDataTab = new crTableIO;

			titleVec.resize(1);
			rowVec.resize(1);
			titleVec[0] = "groupID";
			myGroupApplySetDataTab ->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJMyApplyGroupIDSet,param);
			MyGroupApplySet & myGroupApplySet = *(MyGroupApplySet *)param;
			if(!myGroupApplySet.empty())
			{
				for( MyGroupApplySet::iterator itr = myGroupApplySet.begin();
					itr != myGroupApplySet.end(); ++itr )
				{
					rowVec[0] = crArgumentParser::appItoa(*itr);
					myGroupApplySetDataTab->addData(rowVec);
				}
			}
			myGroupApplySetDataTab->saveToFileNoCook(tabPath + "申请的军团.tab");

			// 已经通关的军团副本id

			ref_ptr<crTableIO> hadWonGroupFuBenSetDataTab = new crTableIO;

			titleVec.resize(1);
			rowVec.resize(1);
			titleVec[0] = "fubenID";
			hadWonGroupFuBenSetDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJHadWonGroupFuBenSet	,param);
			HadWonGroupFuBenSet & hadWonGroupFuBenSet = *(HadWonGroupFuBenSet *)param;
			if (!hadWonGroupFuBenSet.empty())
			{
				HadWonGroupFuBenSet::iterator hwgfItr = hadWonGroupFuBenSet.begin();
				for (;hwgfItr != hadWonGroupFuBenSet.end(); ++ hwgfItr )
				{
					rowVec[0] = crArgumentParser::appItoa(*hwgfItr);
					hadWonGroupFuBenSetDataTab->addData(rowVec);
				}
			}
			hadWonGroupFuBenSetDataTab->saveToFileNoCook(tabPath + "已通关的军团副本.tab");
			//hadWonGroupFuBenSetDataTab end

			// 新功能开启提示set，已提示过的在set中做记录

			ref_ptr<crTableIO> newfunctionopensetDataTab = new crTableIO;

			titleVec.resize(1);
			rowVec.resize(1);
			titleVec[0] = "functionID";
			newfunctionopensetDataTab->setTitleVec(titleVec);
			playerData->getParam(WCHDATA_JXJNewFunctionOpenSet,param);
			NewFunctionOpenSet * newfunctionopenset = (NewFunctionOpenSet *)param;
			if (newfunctionopenset && !newfunctionopenset->empty())
			{
				NewFunctionOpenSet::iterator newfuncItr = newfunctionopenset->begin();
				for (; newfuncItr != newfunctionopenset->end(); ++newfuncItr)
				{
					rowVec[0] = crArgumentParser::appItoa(*newfuncItr);
					newfunctionopensetDataTab->addData(rowVec);
				}
			}
			newfunctionopensetDataTab->saveToFileNoCook(tabPath + "新功能开启提示.tab");
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJWebQueryPlayerDataDisposeMethod
 
**************************************************************************/

JXJ::crJXJWebQueryPlayerDataDisposeMethod::crJXJWebQueryPlayerDataDisposeMethod():
	m_this(NULL)
{

}

JXJ::crJXJWebQueryPlayerDataDisposeMethod::crJXJWebQueryPlayerDataDisposeMethod( const crJXJWebQueryPlayerDataDisposeMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJWebQueryPlayerDataDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJWebQueryPlayerDataDisposeMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJWebQueryPlayerDataDisposeMethod::operator()( crHandle &handle )
{
	int id;
	int playerid;
	int serverid;
	int gmID;
	//int queryid;
	int querytype;

	ref_ptr<crGameServerPlayerData> playerData = NULL;
	//crData *data,*mainroledata;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	if (!gameServer)
	{
		return;
	}
	crNetManager *netManager = gameServer->getNetManager();
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	serverid = callback->getServerID();
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
	ref_ptr<crDataBase> accountSession;
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crDataQueryTaskData> dataQueryTask = new crDataQueryTaskData;
	ref_ptr<crJXJLockPlayerdata> lockPlayerData = new crJXJLockPlayerdata;
	ref_ptr<crJXJUnLockPlayerdata> unlockPlayerData = new crJXJUnLockPlayerdata;
	dataQueryTask->buildQuerySql("dataquerytask",1,serverid);
	globalSession->executeQuery(dataQueryTask.get());
	crDataBase::QueryResultVec dataQueryTaskVec = globalSession->getQueryResult();
	globalSession->releaseQuery();
	db->endSession(globalSession.get());
			
	if(!dataQueryTaskVec.empty())
	{
		bool suc = false;
		crPlayerDataEventPacket packet;
		ref_ptr<crStreamBuf>stream;
		ref_ptr<crJXJRemoveDataQueryTaskRecord> removeDataQueryTaskRecord = new crJXJRemoveDataQueryTaskRecord;
		ref_ptr<crJXJInsertDataQueryRecord> insertDataQueryRecord = new crJXJInsertDataQueryRecord;
		for( crDataBase::QueryResultVec::iterator itr = dataQueryTaskVec.begin();
			itr != dataQueryTaskVec.end(); ++itr )
		{
			dataQueryTask = dynamic_cast<crDataQueryTaskData*>(itr->get());

			id = dataQueryTask->getID();
			playerid = dataQueryTask->getPlayerID();
			//serverid = dataQueryTask->getServerID();
			gmID = dataQueryTask->getGMID();
			//queryid = dataQueryTask->getQueryID();
			querytype = dataQueryTask->getQueryType();

			std::string retStr; 
			suc = false;
			switch (querytype)
			{
			case QT_GM_RefreshNoChatTab:
				{
					globalSession = db->beginSession();
					ref_ptr<crJXJNoChatQueryData> noChatQueryQuery = new crJXJNoChatQueryData;
					noChatQueryQuery->buildQuerySql(serverid);
					globalSession->executeQuery(noChatQueryQuery.get());
					crDataBase::QueryResultVec noChatQueryQueryResultVec = globalSession->getQueryResult();
					globalSession->releaseQuery();
					db->endSession(globalSession.get());

					void *param;
					crData * gameData = m_this->getDataClass();
					gameData->excHandle(MAKEINT64(WCH_LockData,1));

					gameData->getParam(WCHDATA_JXJNoChatSet,param);
					NoChatSet *noChatSet = (NoChatSet *)param;
					noChatSet->clear();
					for( crDataBase::QueryResultVec::iterator itr = noChatQueryQueryResultVec.begin();
						itr != noChatQueryQueryResultVec.end();
						++itr )
					{
						noChatQueryQuery = dynamic_cast<crJXJNoChatQueryData *>(itr->get());
						noChatSet->insert(noChatQueryQuery->getPlayerID());
					}
					gameData->excHandle(MAKEINT64(WCH_LockData,0));
					suc = true;
				}
				break;
			case QT_GM_RefreshGMTab:
				{
					globalSession = db->beginSession();
					ref_ptr<crJXJGmQueryData> gmQueryQuery = new crJXJGmQueryData;
					gmQueryQuery->buildQuerySql(serverid);
					globalSession->executeQuery(gmQueryQuery.get());
					crDataBase::QueryResultVec gmQueryQueryResultVec = globalSession->getQueryResult();
					globalSession->releaseQuery();
					db->endSession(globalSession.get());

					crGlobalHandle::gData()->lockGMMap();
					GMMap &gmMap = crGlobalHandle::gData()->getGMMap();
					gmMap.clear();
					for( crDataBase::QueryResultVec::iterator itr = gmQueryQueryResultVec.begin();
						itr != gmQueryQueryResultVec.end();
						++itr )
					{
						gmQueryQuery = dynamic_cast<crJXJGmQueryData *>(itr->get());
						gmMap[gmQueryQuery->getPlayerID()] = gmQueryQuery->getAuthorityid();
					}
					crGlobalHandle::gData()->unlockGMMap();
					suc = true;
				}
				break;
			case QT_PlayerData	:
				{
					accountSession = accountdb->beginSession();
					//锁住玩家
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

					playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
					if (!playerData.valid())
					{
						playerData = getPlayerData(playerid,serverid);
					}
					if (playerData.valid() && playerData->getPlayerGameData()->getMainRole())
					{
						m_this->doEvent(WCH_JXJBuildPlayerDataSaveStreamTab	,MAKEINT64(playerData->getPlayerGameData(),&gmID));

						suc = true;
					}
							
					accountSession = accountdb->beginSession();
					unlockPlayerData->buildUpdateSql(playerid);
					if(accountSession->executeUpdate(unlockPlayerData.get()))
					{
						accountSession->commit();
					}
					else
					{
						accountSession->rollback();
					}
					accountdb->endSession(accountSession.get());
				}
				break;
			case QT_GM_QueryOnlinePlayerNum :
				{
					crData * serverBrainData = m_this->getDataClass();
					serverBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					void *param;
					serverBrainData->getParam(WCHDATA_JXJOnlineEveryDayPlayeridSet,param);
					OnlinePlayerSet * playerSet = (OnlinePlayerSet *)param;
					int curCount = playerSet->size();
					serverBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					retStr = crArgumentParser::appItoa(curCount);
					suc = true;
				}
				break;
			default:
				break;
			}

			globalSession = db->beginSession();
			insertDataQueryRecord ->buildUpdateSql(playerid,serverid,id,gmID,querytype,suc,retStr);
			removeDataQueryTaskRecord->buildUpdateSql(id) ;
			if(globalSession->executeUpdate(insertDataQueryRecord .get()) && globalSession->executeUpdate(removeDataQueryTaskRecord.get()))
			{
				globalSession->commit();
			}
			else
			{
				globalSession->rollback();
			}
			db->endSession(globalSession.get());
		}
	}
}

CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> JXJ::crJXJWebQueryPlayerDataDisposeMethod::getPlayerData( int playerid,int serverid )
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
	playergamedbQuery->buildQuerySql(playerid,serverid);

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
		playerMoneyQuery->buildQuerySql(playerid,serverid);

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

/**************************************************************************

METHOD: crJXJSavePeopleMapDisposeMethod
 
**************************************************************************/
JXJ::crJXJSavePeopleMapDisposeMethod::crJXJSavePeopleMapDisposeMethod():
	m_this(NULL)
{

}

JXJ::crJXJSavePeopleMapDisposeMethod::crJXJSavePeopleMapDisposeMethod( const crJXJSavePeopleMapDisposeMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJSavePeopleMapDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSavePeopleMapDisposeMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJSavePeopleMapDisposeMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		crData *gsBrainData = m_this->getDataClass();
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

		crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crDataBase> db_session;// = global_db->beginSession();

		CRCore::ref_ptr<crJXJUpdatePeopleMapRecord > updateSql = new crJXJUpdatePeopleMapRecord; 
		CRCore::ref_ptr<crJXJInsertPeopleMapRecord> insertSql = new crJXJInsertPeopleMapRecord; 
		void *param;
		for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i)
		{
			gsBrainData->getParam(i,param);
			crJXJShili *shili = (crJXJShili *)param;
			if (shili && shili->getDataClass())
			{
				crData *shiliData = shili->getDataClass();	
				CRCore::ScopedLock<crData> lock(*shiliData);

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				PeopleMap::iterator itr;

				shiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
				ModifyPlayerSet * modifyPlayerSet = (ModifyPlayerSet *)param;
				shiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
				InsertPlayerSet * insertPlayerSet = (InsertPlayerSet *)param;

				if (!insertPlayerSet->empty())
				{
					db_session = global_db->beginSession();
					for (InsertPlayerSet::iterator ipsItr = insertPlayerSet->begin();
						ipsItr != insertPlayerSet->end(); ++ ipsItr)
					{
						itr = peopleMap->find(*ipsItr); 
						if ( itr != peopleMap->end())
						{
							insertSql->buildUpdateSql(itr->second->getID(),itr->second->getShili(),
								itr->second->getName(),itr->second->getLv(),itr->second->getTroopsLv(),
								itr->second->getConsortiaId(),itr->second->getSmallAcc(),
								itr->second->getKillNum(),itr->second->getNewerStep(),itr->second->getDeadTroopsNum()
								,itr->second->getGiftGold(),itr->second->getTotalRecharge(),itr->second->getTroopsnum(),itr->second->getExploit());
							if(db_session->executeUpdate(insertSql.get()))
							{
								db_session->commit();
							}
							else
							{
								db_session->rollback();
							}
						}
					}
					global_db->endSession(db_session.get());
					insertPlayerSet->clear();
				}
				if (!modifyPlayerSet->empty())
				{
					db_session = global_db->beginSession();
					for (ModifyPlayerSet::iterator mpsItr = modifyPlayerSet->begin();
						mpsItr != modifyPlayerSet->end(); ++ mpsItr )
					{
						itr = peopleMap->find(*mpsItr); 
						if ( itr != peopleMap->end())
						{
							updateSql->buildUpdateSql(itr->second->getID(),itr->second->getShili(),
								itr->second->getName(),itr->second->getLv(),itr->second->getTroopsLv(),
								itr->second->getConsortiaId(),itr->second->getSmallAcc(),
								itr->second->getKillNum(),itr->second->getNewerStep(),itr->second->getDeadTroopsNum()
								,itr->second->getGiftGold(),itr->second->getTotalRecharge(),itr->second->getTroopsnum(),itr->second->getExploit());
							if(db_session->executeUpdate(updateSql.get()))
							{
								db_session->commit();
							}
							else
							{
								db_session->rollback();
							}
						}
					}
					global_db->endSession(db_session.get());
					modifyPlayerSet->clear();
				}
			}
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJSaveLatestMousePushPosMethod
 
**************************************************************************/
JXJ::crJXJSaveLatestMousePushPosMethod::crJXJSaveLatestMousePushPosMethod():
	m_this(NULL),
	m_param(0),
	m_ea(NULL)
{

}

JXJ::crJXJSaveLatestMousePushPosMethod::crJXJSaveLatestMousePushPosMethod( const crJXJSaveLatestMousePushPosMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_param(0),
	m_ea(NULL)
{

}

void JXJ::crJXJSaveLatestMousePushPosMethod::inputParam( int i, void *param )
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
		m_this = (crMatrixTransform*)param;
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

void JXJ::crJXJSaveLatestMousePushPosMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJSaveLatestMousePushPosMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crVector3 mousepos(m_ea->getXnormalized(),m_ea->getYnormalized(),0.0f);
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->inputParam(WCHDATA_JXJMouseLastPushPos,&mousepos);
	}
}
/**************************************************************************

METHOD: crJXJShowFloadAnimationTipsMethod
 
**************************************************************************/
JXJ::crJXJShowFloadAnimationTipsMethod::crJXJShowFloadAnimationTipsMethod():
	m_this(NULL)
{

}

JXJ::crJXJShowFloadAnimationTipsMethod::crJXJShowFloadAnimationTipsMethod( const crJXJShowFloadAnimationTipsMethod & handle ):
	crMethod(handle),
	 m_canvasInput(handle.m_canvasInput),
	 m_input(handle.m_input),
	 m_pos(handle.m_pos),
	 m_speed(handle.m_speed),
	 m_inputPos(handle.m_inputPos),
	 m_inputTargetPos(handle.m_inputTargetPos),
	 m_inputSpeed(handle.m_inputSpeed)
{

}

void JXJ::crJXJShowFloadAnimationTipsMethod::inputParam( int i, void *param )
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
		m_this = (crBrain*)param;
		break;
	}

}

void JXJ::crJXJShowFloadAnimationTipsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvasInput = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		crArgumentParser::appAtoVec(str.c_str(),m_pos);
		break;
	case 3:
		m_speed = atof(str.c_str());
		break;
	case 4:
		crArgumentParser::appAtoVec(str.c_str(),m_inputPos);
		break;
	case 5:
		crArgumentParser::appAtoVec(str.c_str(),m_inputTargetPos);
		break;
	case 6:
		m_inputSpeed = atof(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJShowFloadAnimationTipsMethod::operator()( crHandle &handle )
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  inPutCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasInput);
		CRCore::ref_ptr<CRCore::crCanvasNode> cpInputNode;
		ref_ptr<crStaticTextWidgetNode>  inputBox = NULL;

		void *param;
		crTableIO::StrVec itemrecord;
		crData *brainData = m_this->getDataClass();
		if(brainData && inPutCanvas.valid())
		{
			brainData->excHandle(MAKEINT64(WCH_LockData,1));
			brainData->getParam(WCHDATA_JXJFloatAnimationTextDeq	,param);
			if(param)
			{
				RecvFloatAnimationTextDeq *contentDeq = (RecvFloatAnimationTextDeq *)param;
				if (!contentDeq->empty())
				{
					std::string text = contentDeq->front();
					brainData->getParam(WCHDATA_JXJFloatAnimationCanvasVec,param);
					FloatAnimationCanvasVec * canvasVec = (FloatAnimationCanvasVec *)param;
					for ( FloatAnimationCanvasVec ::iterator itr = canvasVec->begin(); itr != canvasVec->end(); ++itr )
					{
						if(!itr->valid()) continue;
						if((*itr)->getVisiable()==false)
						{
							cpInputNode = *itr;
							break;
						}
					}
					if(!cpInputNode.valid())
					{
						cpInputNode = dynamic_cast<crCanvasNode *>(inPutCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
						cpInputNode->setEnable(false);
						cpInputNode->setCanFocus(false);
						cpInputNode->setCanCaptureMouse(false);
						crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpInputNode.get(),false);

						canvasVec->push_back(cpInputNode);
					}
					if(cpInputNode.valid() && cpInputNode->getDataClass())
					{
						crData *inputCanvasData = cpInputNode->getDataClass();
						inputCanvasData->excHandle(MAKEINT64(WCH_LockData,1));
						inputBox = dynamic_cast<crStaticTextWidgetNode *>(cpInputNode->getWidget(m_input));
						if(inputBox.valid())
						{
							crVector4f colormode = COLOR_YELLOW;
							inputBox->setColor(colormode);
							inputBox->setString(text);
							inputBox->setAlignment(CRText::crText::CENTER_BASE_LINE);
							inputCanvasData->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
							crVector3 * beginPos = (crVector3 *)param;
							inputCanvasData->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
							crVector3 * targetPos = (crVector3 *)param;
							inputCanvasData->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
							crVector3 * moveSpeed = (crVector3 *)param;
							inputCanvasData->getParam(WCHDATA_JXJMoveDistance,param);
							float *moveDistance = (float *)param;

							inputCanvasData->getParam(WCHDATA_JXJDongHuaTranslate,param);
							crVector3 * translatePos = (crVector3 *)param;
							brainData->getParam(WCHDATA_JXJMouseLastPushPos,param);
							crVector3 * mouse = (crVector3 *)param;

							*beginPos = m_inputPos;
							(*beginPos)[2] =  0;

							*translatePos = *mouse - *beginPos;

							(*targetPos)[0] = CRCore::rangei(-25,25).get_random() * 0.01f;
							(*targetPos)[1] = m_inputTargetPos.y();
							(*targetPos)[2] = 0.0f;

							crVector3 dirVector = *targetPos - *beginPos;
							float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
							*moveDistance = length;

							(*moveSpeed)[0] = m_inputSpeed* dirVector[0]/length;
							(*moveSpeed)[1] = m_inputSpeed* dirVector[1]/length+0.1f;
							(*moveSpeed)[2] = 0.0f;
							cpInputNode->setMatrix(crMatrix::translate(m_inputPos));
							cpInputNode->setCanvasPos(CANVAS_TOPMOST);
							crFilterRenderManager::getInstance()->showCanvas(cpInputNode.get(),true);
						}
						inputCanvasData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
				contentDeq->pop_front();
			}
		}
		brainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJPutFloatContentIntoDeqMethod
 
**************************************************************************/
JXJ::crJXJPutFloatContentIntoDeqMethod::crJXJPutFloatContentIntoDeqMethod():
	m_this(NULL),
	m_textID(0)
{

}

JXJ::crJXJPutFloatContentIntoDeqMethod::crJXJPutFloatContentIntoDeqMethod( const crJXJPutFloatContentIntoDeqMethod & handle ):
	crMethod(handle),
	m_textID(handle.m_textID)
{

}

void JXJ::crJXJPutFloatContentIntoDeqMethod::inputParam( int i, void *param )
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
		m_this = (crBrain*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_textID = LOINT64(param64);
		}
		else
		{
			m_textID = 0;
		}
		break;
	}

}

void JXJ::crJXJPutFloatContentIntoDeqMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJPutFloatContentIntoDeqMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass() && m_textID)
	{
		void *param;
		crData * brainData = m_this->getDataClass();
		brainData->excHandle(MAKEINT64(WCH_LockData,1));
		brainData->getParam(WCHDATA_JXJFloatAnimationTextDeq	,param);
		RecvFloatAnimationTextDeq *contentDeq = (RecvFloatAnimationTextDeq *)param;

		ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		crTableIO::StrVec textrecord;
		if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(m_textID),textrecord) >= 0)
		{
			contentDeq->push_back(textrecord[1]);
		}
		brainData->excHandle(MAKEINT64(WCH_LockData,0));
	}

}

/**************************************************************************

METHOD: crJXJFoundCountryGetAwardMethod
 
**************************************************************************/

JXJ::crJXJFoundCountryGetAwardMethod::crJXJFoundCountryGetAwardMethod():
	m_awardType(0),
	m_idx(0)
{

}

JXJ::crJXJFoundCountryGetAwardMethod::crJXJFoundCountryGetAwardMethod( const crJXJFoundCountryGetAwardMethod & handle ):
	crMethod(handle),
	m_awardType(handle.m_awardType),
	m_idx(handle.m_idx)
{

}

void JXJ::crJXJFoundCountryGetAwardMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJFoundCountryGetAwardMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_awardType = atoi(str.c_str());
		break;
	case 1:
		m_idx = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJFoundCountryGetAwardMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(6);
		stream->_writeUChar(m_awardType);
		if (!m_awardType)
		{
			stream->_writeUChar(m_idx);
		}
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFoundCountryGetAward,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/**************************************************************************

METHOD: crJXJRecvFoundCountryGetAwardMethod
 
**************************************************************************/

JXJ::crJXJRecvFoundCountryGetAwardMethod::crJXJRecvFoundCountryGetAwardMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvFoundCountryGetAwardMethod::crJXJRecvFoundCountryGetAwardMethod( const crJXJRecvFoundCountryGetAwardMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvFoundCountryGetAwardMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_netType = Client;
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
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void JXJ::crJXJRecvFoundCountryGetAwardMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvFoundCountryGetAwardMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		if (m_netType == GameServer && m_stream.valid())
		{
			unsigned char awardType = m_stream->_readUChar();
			unsigned char cityIdx = 8;	// > 7 
			if (!awardType)
			{
				cityIdx = m_stream->_readUChar();
			}

			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();

			//bool newerCompleted = false;
			//m_this->doEvent(WCH_JXJNewerTaskHadAllCompleted	,MAKEINT64(&newerCompleted,NULL));

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				crData *roledata = m_this->getMainRole()->getDataClass();
				roledata->getParam(WCHDATA_Level, param);
				unsigned char characterlevel = *(unsigned char *)param;

				unsigned char ret = JXJFoundCountryGetAwardRet_unkwon;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

					data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
					unsigned char hadGotAward = *(unsigned char *)param;
					unsigned char oldGotAwardVal = hadGotAward;

					data->getParam(WCHDATA_JXJShiliID,param);
					unsigned char shiliid = *(unsigned char *)param;
					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;

					crData *shilidata = shili->getDataClass();	

					shilidata->getParam(WCHDATA_JXJShiliFoundCountryWinCity,param);
					unsigned char winCity = *(unsigned char *)param;

					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char viplv = *(unsigned char *)param;
					unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFoundCountryOpenLv,viplv).c_str()));
					if (characterlevel >= openlv)
					{
						if (awardType)
						{
							if (winCity != (unsigned char) -1)
							{
								// 没有全部达成
								ret = JXJFoundCountryGetAwardRet_noAllOwnCity;
							}
							else
							{
								if (hadGotAward != (unsigned char)-1)
								{
									ret = JXJFoundCountryGetAwardRet_suc;
									hadGotAward = (unsigned char)-1;
								}
								else
								{
									// 已经全部领取
									ret = JXJFoundCountryGetAwardRet_hadGotAward;
								}
							}
						}
						else
						{
							if (cityIdx < 8)
							{
								if (winCity & (1 << cityIdx ))
								{
									if (hadGotAward & (1 << cityIdx))
									{
										// 已经领取过
										ret = JXJFoundCountryGetAwardRet_hadGotAward;
									}
									else
									{
										ret = JXJFoundCountryGetAwardRet_suc;
										hadGotAward |= 1 << cityIdx;
									}
								}
								else
								{
									ret = JXJFoundCountryGetAwardRet_noOwnCity;
								}
							}
						}
					}
					else
					{
						ret = JXJFoundCountryGetAwardRet_lessOpenLevel;
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					if (ret == JXJFoundCountryGetAwardRet_suc)
					{
						data->inputParam(WCHDATA_JXJFoundCountryGetAward,&hadGotAward);

						ref_ptr<crTableIO> foundCountryAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);
						if (foundCountryAwardTab.valid())
						{
							crTableIO::StrVec record;
							crTableIO::DataVec dataVec;
							int awardIdx1 = foundCountryAwardTab->getTitleIndex("奖励1");
							int awardIdx2 = foundCountryAwardTab->getTitleIndex("奖励2");
							int orderIdx = foundCountryAwardTab->getTitleIndex("顺序");
							int shiliIdx = foundCountryAwardTab->getTitleIndex("shili");
							//int vipExpIdx = foundCountryAwardTab->getTitleIndex("vipExp");
							std::vector<CRCore::crVector4i> awards;
							crVector4i vec4;
							//RewardItemVec rewardItems;
							//CRCore::ref_ptr<crBagItemData> itemPtr;
							std::string logdata;
							//int addVipExp = 0;
							if (!awardType)
							{
								logdata="领取建国奖励:"+crArgumentParser::appItoa(cityIdx + 1)+",";
								// 领取单个奖励
								if (foundCountryAwardTab->queryOneRecord(orderIdx,crArgumentParser::appItoa(cityIdx + 1),
									shiliIdx,crArgumentParser::appItoa(shiliid),record) >= 0)
								{
									//typedef std::vector<int> AwardVec;
									//AwardVec awardVec;
									if(!record[awardIdx1].empty())
									{
										logdata += record[awardIdx1];
										crArgumentParser::appAtoVec(record[awardIdx1],vec4);
										awards.push_back(vec4);
										//itemPtr = new crBagItemData;
										//itemPtr->setItemID(vec3[0]);
										//itemPtr->setEquipMagic(vec3[1]);
										//itemPtr->setItemCount(vec3[2]);
										//rewardItems.push_back(itemPtr);
										//data->excHandle(MAKEINT64(WCH_LockData,1));
										//m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
										//data->excHandle(MAKEINT64(WCH_LockData,0));
									}
									if(!record[awardIdx2].empty())
									{
										logdata += record[awardIdx2];
										crArgumentParser::appAtoVec(record[awardIdx2],vec4);
										awards.push_back(vec4);
									}
									if(!awards.empty())
									{
										data->excHandle(MAKEINT64(WCH_LockData,1));
										sendReward(awards,m_this,"建国奖励");
										data->excHandle(MAKEINT64(WCH_LockData,0));
									}
									//addVipExp = atoi(record[vipExpIdx].c_str());
									//if (addVipExp > 0)
									//{
									//	logdata += " VipExp:"+record[vipExpIdx];
									//	m_this->doEvent(WCH_JXJAddVipExp,MAKEINT64(addVipExp,NULL));
									//}
								}
							}
							else
							{
								logdata="领取全部建国奖励:";
								unsigned char order = 8;
								// 领取所有

								//typedef std::vector<int> AwardVec;
								//AwardVec awardVec;
								//RewardItemVec rewardItems;
								foundCountryAwardTab->queryRecords( shiliIdx,crArgumentParser::appItoa(shiliid),dataVec);
								for (crTableIO::DataVec::iterator dvItr = dataVec.begin();
									dvItr != dataVec.end(); ++ dvItr)
								{
									order = atoi((*dvItr)[orderIdx].c_str()) - 1;
									if (order > 8
										|| (oldGotAwardVal & (1 << order)))
									{
										// 表中参数不对，或者是已经领过的
										continue;
									}

									//CRCore::ref_ptr<crBagItemData> itemPtr = NULL;
									if(!(*dvItr)[awardIdx1].empty())
									{
										logdata += (*dvItr)[awardIdx1];
										crArgumentParser::appAtoVec((*dvItr)[awardIdx1],vec4);
										awards.push_back(vec4);
										//itemPtr = new crBagItemData;
										//itemPtr->setItemID(vec3[0]);
										//itemPtr->setEquipMagic(vec3[1]);
										//itemPtr->setItemCount(vec3[2]);
										//rewardItems.push_back(itemPtr);
									}
									if(!(*dvItr)[awardIdx2].empty())
									{
										logdata += (*dvItr)[awardIdx2];
										crArgumentParser::appAtoVec((*dvItr)[awardIdx2],vec4);
										awards.push_back(vec4);
									}
									//addVipExp += atoi((*dvItr)[vipExpIdx].c_str());
								}
								if(!awards.empty())
								{
									data->excHandle(MAKEINT64(WCH_LockData,1));
									sendReward(awards,m_this,"建国奖励");
									data->excHandle(MAKEINT64(WCH_LockData,0));
								}
								//if(addVipExp>0)
								//{
								//	logdata += " VipExp:"+crArgumentParser::appItoa(addVipExp);
								//	m_this->doEvent(WCH_JXJAddVipExp,MAKEINT64(addVipExp,NULL));
								//}
								//if(!rewardItems.empty())
								//{
								//	data->excHandle(MAKEINT64(WCH_LockData,1));
								//	m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
								//	data->excHandle(MAKEINT64(WCH_LockData,0));
								//}
							}
							GameLogData gamelog(Log_GetPlayerFoundAward,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
						}
					}

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(6);
					stream->_writeUChar(ret);
					stream->_writeUChar(winCity);
					stream->_writeUChar(hadGotAward);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerData->getPlayerID(),WCH_JXJRecvFoundCountryGetAward,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game && m_stream.valid())
		{
			unsigned char ret = m_stream->_readUChar();
			unsigned char winCity = m_stream->_readUChar();
			unsigned char hadGotAward = m_stream->_readUChar();

			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				//crData * canvasData = canvas->getDataClass();
				void *param;
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				if (myPlayer && myPlayer->getDataClass())
				{
					crData * playerData = myPlayer->getDataClass();
					playerData->inputParam(WCHDATA_JXJFoundCountryGetAward,&hadGotAward);

					playerData->excHandle(MAKEINT64(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJMyCountryData,param);
					crJXJShili *shiliPtr = (crJXJShili *)param;
					if (shiliPtr && shiliPtr->getDataClass())
					{
						crData *shiliData = shiliPtr->getDataClass();
						shiliData->inputParam(WCHDATA_JXJShiliFoundCountryWinCity,&winCity);
					}
					playerData->excHandle(MAKEINT64(WCH_LockData,0));
					canvas->doEvent(WCH_UI_UpdateData);
				}
				switch (ret)
				{
				case JXJFoundCountryGetAwardRet_suc:
					break;
				case JXJFoundCountryGetAwardRet_newerStepUncompleted:
					break;
				case JXJFoundCountryGetAwardRet_noOwnCity:
					break;
				case JXJFoundCountryGetAwardRet_noAllOwnCity:
					break;
				case JXJFoundCountryGetAwardRet_hadGotAward:
					break;
				case JXJFoundCountryGetAwardRet_lessOpenLevel:
					{
						int msgid = 4104;
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJNewerTaskHadAllCompletedMethod
 
**************************************************************************/
JXJ::crJXJNewerTaskHadAllCompletedMethod::crJXJNewerTaskHadAllCompletedMethod():
	m_this(NULL),
	m_lastTaskID(0),
	m_isCompleted(NULL)
{

}

JXJ::crJXJNewerTaskHadAllCompletedMethod::crJXJNewerTaskHadAllCompletedMethod( const crJXJNewerTaskHadAllCompletedMethod & handle ):
	crMethod(handle),
	m_lastTaskID(handle.m_lastTaskID)
{

}

void JXJ::crJXJNewerTaskHadAllCompletedMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_isCompleted = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_isCompleted = (bool *)(LOINT64(param64));
		}
		else
		{
			m_isCompleted = NULL;
		}
		break;
	}
}

void JXJ::crJXJNewerTaskHadAllCompletedMethod::addParam( int i, const std::string& str )
{

	switch (i)
	{
	case 0:
		m_lastTaskID = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJNewerTaskHadAllCompletedMethod::operator()( crHandle &handle )
{
	if (m_this && m_isCompleted)
	{
		*m_isCompleted = false;

		void *param;

		crRole *mainRole = m_this->getMainRole();
		if (mainRole)
		{
			crData *metierData = mainRole->getMetierDataClass();
			if(metierData)
			{
				metierData->excHandle(MAKEINT64(WCH_LockData,1));

				metierData->getParam(WCHDATA_GameTaskMap,param);
				GameTaskMap* gameTaskMap = (GameTaskMap* )param;
				if(gameTaskMap)
				{
					GameTaskMap::iterator gmItr = gameTaskMap->find(m_lastTaskID);
					if (gmItr != gameTaskMap->end())
					{
						if (gmItr->second->isComplete())
						{
							*m_isCompleted = true;
						}
					}
				}
				metierData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

///**************************************************************************
//
//METHOD: crJXJFoundCountryCanvasUpdateMethod
// 
//**************************************************************************/
//JXJ::crJXJFoundCountryCanvasUpdateMethod::crJXJFoundCountryCanvasUpdateMethod():
//	m_this(NULL)
//{
//
//}
//
//JXJ::crJXJFoundCountryCanvasUpdateMethod::crJXJFoundCountryCanvasUpdateMethod( const crJXJFoundCountryCanvasUpdateMethod & handle ):
//	crMethod(handle),
//	m_bckSw(handle.m_bckSw),
//	m_getAllAwardBtn(handle.m_getAllAwardBtn),
//	m_pageSw(handle.m_pageSw),
//	m_topInput(handle.m_topInput),
//	m_getAllAwardBtnSw(handle.m_getAllAwardBtnSw),
//	m_goToGuozhanBtn(handle.m_goToGuozhanBtn)
//	
//{
//	for (unsigned char i = 0; i < 3; ++ i)
//	{
//		for (unsigned char j = 0; j < 9; ++ j)
//		{
//			if (j < 8)
//			{
//				m_citySw[i][j] = handle.m_citySw[i][j];
//				m_cityBtnSw[i][j] = handle.m_cityBtnSw[i][j];
//				m_cityBtn[i][j] = handle.m_cityBtn[i][j];
//			}
//			m_cityInput[i][j] = handle.m_cityInput[i][j];
//		}
//	}
//}
//
//void JXJ::crJXJFoundCountryCanvasUpdateMethod::inputParam( int i, void *param )
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
//		m_this = (crCanvasNode *)param;
//		break;
//	}
//}
//
//void JXJ::crJXJFoundCountryCanvasUpdateMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_bckSw = str;
//		break;
//	case 1:
//		m_getAllAwardBtn = str;
//		break;
//	case 2:
//		m_pageSw = str;
//		break;
//	case 3:
//		m_topInput = str;
//		break;
//	case 4:
//	case 5:
//	case 6:
//	case 7:
//	case 8:
//	case 9:
//	case 10:
//	case 11:
//	case 12:
//	case 13:
//	case 14:
//	case 15:
//	case 16:
//	case 17:
//	case 18:
//	case 19:
//	case 20:
//	case 21:
//	case 22:
//	case 23:
//	case 24:
//	case 25:
//	case 26:
//	case 27:
//		m_citySw[(i - 4) / 8][(i - 4) % 8] = str;
//		break;
//	case 28:
//	case 29:
//	case 30:
//	case 31:
//	case 32:
//	case 33:
//	case 34:
//	case 35:
//	case 36:
//	case 37:
//	case 38:
//	case 39:
//	case 40:
//	case 41:
//	case 42:
//	case 43:
//	case 44:
//	case 45:
//	case 46:
//	case 47:
//	case 48:
//	case 49:
//	case 50:
//	case 51:
//		m_cityBtnSw[(i - 28) / 8][(i - 28) % 8] = str;
//	break;
//	case 52:
//	case 53:
//	case 54:
//	case 55:
//	case 56:
//	case 57:
//	case 58:
//	case 59:
//	case 60:
//	case 61:
//	case 62:
//	case 63:
//	case 64:
//	case 65:
//	case 66:
//	case 67:
//	case 68:
//	case 69:
//	case 70:
//	case 71:
//	case 72:
//	case 73:
//	case 74:
//	case 75:
//	case 76:
//	case 77:
//	case 78:
//		m_cityInput[(i - 52) / 9][(i - 52) % 9] = str;
//		break;
//	case 79:
//		m_goToGuozhanBtn = str;
//		break;
//	case 80:
//	case 81:
//	case 82:
//	case 83:
//	case 84:
//	case 85:
//	case 86:
//	case 87:
//	case 88:
//	case 89:
//	case 90:
//	case 91:
//	case 92:
//	case 93:
//	case 94:
//	case 95:
//	case 96:
//	case 97:
//	case 98:
//	case 99:
//	case 100:
//	case 101:
//	case 102:
//	case 103:
//		m_cityBtn[(i - 80) / 8][(i - 80) % 8] = str;
//		break;
//	case 104:
//	case 105:
//	case 106:
//		m_arrow[i-104] = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJFoundCountryCanvasUpdateMethod::operator()( crHandle &handle )
//{
//	if (m_this)
//	{
//		ref_ptr<crMultiSwitch> bakSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_bckSw));
//		ref_ptr<crMultiSwitch> pageSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_pageSw));
//		ref_ptr<crStaticTextWidgetNode> topInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_topInput));
//		ref_ptr<crButtonWidgetNode> getAllAwardBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_getAllAwardBtn));
//		ref_ptr<crButtonWidgetNode> goToBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goToGuozhanBtn));
//		ref_ptr<crMultiSwitch> getAllAwardBtnSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_getAllAwardBtnSw));
//
//		ref_ptr<crMultiSwitch> citySw[3][8] = {NULL};
//		ref_ptr<crMultiSwitch> cityBtnSw[3][8] = {NULL};
//		ref_ptr<crMultiSwitch> arrowSw[3] = {NULL};
//		
//		//ref_ptr<crStaticTextWidgetNode> cityInput[3][9] = {NULL};
//		ref_ptr<crButtonWidgetNode> cityBtn[3][8] = {NULL};
//		for (unsigned char i = 0; i < 3; ++ i)
//		{
//			for (unsigned char j  = 0; j < 9; ++ j)
//			{
//				if (j < 8)
//				{
//					citySw[i][j]  = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_citySw[i][j]));
//					cityBtnSw[i][j]  = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_cityBtnSw[i][j]));
//					cityBtn[i][j] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_cityBtn[i][j]));
//					if (cityBtn[i][j].valid())
//					{
//						cityBtn[i][j]->setEnable(false);
//					}
//				}
//				//cityInput[i][j]  = dynamic_cast<crStaticTextWidgetNode *>(m_this->getChildNode(m_cityInput[i][j]));
//			}
//			arrowSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_arrow[i]));
//		}
//		unsigned char hadGotAward = 0;
//		unsigned char shiliId = 0;
//		unsigned char winCity = 0;
//
//		void *param;
//		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//		if (myPlayer && myPlayer->getDataClass())
//		{
//			crData * playerData = myPlayer->getDataClass();
//			playerData->getParam(WCHDATA_JXJFoundCountryGetAward,param);
//			hadGotAward = *(unsigned char *)param;
//
//			playerData->getParam(WCHDATA_JXJShiliID,param);
//			shiliId = *(unsigned char *)param;
//
//			playerData->excHandle(MAKEINT64(WCH_LockData,1));
//			playerData->getParam(WCHDATA_JXJMyCountryData,param);
//			crJXJShili *shiliPtr = (crJXJShili *)param;
//			if (shiliPtr && shiliPtr->getDataClass())
//			{
//				crData *shiliData = shiliPtr->getDataClass();
//				shiliData->getParam(WCHDATA_JXJShiliFoundCountryWinCity,param);
//				winCity = *(unsigned char *)param;
//			}
//			playerData->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//		if (shiliId >= c_startShiliID && shiliId < c_startShiliID + 3)
//		{
//			unsigned char shiliOffset = shiliId - c_startShiliID;
//			if (pageSw.valid())
//				pageSw->setActiveSwitchSet(shiliOffset);
//			if (bakSw.valid())
//				bakSw->setActiveSwitchSet(shiliOffset);
//
//			ref_ptr<crTableIO> foundCountryAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);
//			crTableIO::StrVec record;
//			crTableIO::DataVec dataVec;
//
//			//int awardIdx = -1;
//			//int orderIdx = -1;
//			//int shiliIdx = -1;
//			//int nameIdx = -1;
//			//if (foundCountryAwardTab.valid())
//			//{
//			//	//awardIdx = foundCountryAwardTab->getTitleIndex("奖励");
//			//	//orderIdx = foundCountryAwardTab->getTitleIndex("顺序");
//			//	//shiliIdx = foundCountryAwardTab->getTitleIndex("shili");
//			//	//nameIdx = foundCountryAwardTab->getTitleIndex("名字");
//			//}	
//
//			int textID = 0;
//			if (winCity == (unsigned char)-1)
//			{
//				// 全部占领
//				switch (shiliId)
//				{
//				case 10:
//					textID = 4100;
//					break;
//				case 11:
//					textID = 4101;
//					break;
//				case 12:
//					textID = 4102;
//					break;
//				default:
//					break;
//				}
//			}
//			else
//			{
//				// 建国途中
//				switch (shiliId)
//				{
//				case 10:
//					textID = 4097;
//					break;
//				case 11:
//					textID = 4098;
//					break;
//				case 12:
//					textID = 4099;
//					break;
//				default:
//					break;
//				}
//			}
//			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
//			crTableIO::StrVec textrecord;
//			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textID),textrecord) >= 0)
//			{
//				topInput->setString(textrecord[1]);
//			}
//			int arrowIndex = 0;
//			for (unsigned char i = 0; i < 8; ++ i)
//			{
//				if (cityBtnSw[shiliOffset][i].valid() && citySw[shiliOffset][i].valid())
//				{
//					if (winCity & (1 << i)) 
//					{
//						// 如果被占领
//						if (!(hadGotAward & (1 << i))) // 没有领取过
//						{
//							cityBtn[shiliOffset][i]->setEnable(true);
//							cityBtnSw[shiliOffset][i]->setActiveSwitchSet(0);
//						}
//						else 
//						{
//							cityBtn[shiliOffset][i]->setEnable(false);
//							//cityBtnSw[shiliOffset][i]->setActiveSwitchSet(0);
//						}
//						citySw[shiliOffset][i]->setActiveSwitchSet(1);
//						arrowIndex = i+1;
//					}
//					else
//					{
//						citySw[shiliOffset][i]->setActiveSwitchSet(0);
//						//cityBtnSw[shiliOffset][i]->setActiveSwitchSet(0);
//						cityBtn[shiliOffset][i]->setEnable(false);
//					}
//				}
//
//				//if (foundCountryAwardTab->queryOneRecord(orderIdx,crArgumentParser::appItoa(i + 1),
//				//	shiliIdx,crArgumentParser::appItoa(shiliId),record) >= 0)
//				//{
//				//	cityInput[shiliOffset][i]->setString(record[nameIdx]);
//				//}
//			}
//			if(arrowSw[shiliOffset].valid()) arrowSw[shiliOffset]->setActiveSwitchSet(arrowIndex>7?7:arrowIndex);
//			if (getAllAwardBtn.valid())
//			{
//				if (winCity != (unsigned char)-1)
//				{
//					getAllAwardBtn->setVisiable(false);
//					//getAllAwardBtn->setEnable(false);
//					//if (getAllAwardBtnSw.valid())
//					//	getAllAwardBtnSw->setActiveSwitchSet(3);
//				}
//				else if (winCity == (unsigned char) -1)
//				{
//					getAllAwardBtn->setVisiable(true);
//					//getAllAwardBtn->setEnable(true);
//					//if (getAllAwardBtnSw.valid())
//					//	getAllAwardBtnSw->setActiveSwitchSet(0);
//				}
//			}
//			if (goToBtn.valid())
//			{
//				if (winCity != (unsigned char ) -1)
//				{
//					goToBtn->setVisiable(true);
//					//goToBtn->setEnable(true);
//				}
//				else goToBtn->setVisiable(false);//goToBtn->setEnable(false);
//			}
//		}
//	}
//}

/**************************************************************************

METHOD: crJXJFoundCountryCanvasSwitchBoxAndBtnMethod
 
**************************************************************************/
JXJ::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod():
	m_this(NULL),
	m_focus(false)
{

}

JXJ::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod( const crJXJFoundCountryCanvasSwitchBoxAndBtnMethod & handle ):
	crMethod(handle),
	m_focus(handle.m_focus),
	m_sw(handle.m_sw),
	m_btnSw(handle.m_btnSw),
	m_btn(handle.m_btn)
{

}

void JXJ::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod::inputParam( int i, void *param )
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

void JXJ::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_focus = atoi(str.c_str());
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_sw = str;
		break;
	case 3:
		m_btnSw = str;
		break;
	case 4:
		m_btn = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJFoundCountryCanvasSwitchBoxAndBtnMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		ref_ptr<crMultiSwitch  > sw = dynamic_cast<crMultiSwitch  *>(canvas->getChildNode(m_sw));
		ref_ptr<crMultiSwitch  > btnSw = dynamic_cast<crMultiSwitch  *>(canvas->getChildNode(m_btnSw));
		ref_ptr<crMultiSwitch  > btn = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_btn));
		if (sw.valid())
		{
			if (sw->getActiveSwitchSet() == 1)
			{
				if (m_focus)
					sw->setActiveSwitchSet(2);
				else
					sw->setActiveSwitchSet(1);
			}
			else if (sw->getActiveSwitchSet() == 2)
			{
				if (!m_focus)
					sw->setActiveSwitchSet(1);
			}
		}
	}
}


/**************************************************************************

METHOD: crJXJFirstWeekGenerateCountryOfficialMethod
 
**************************************************************************/
JXJ::crJXJFirstWeekGenerateCountryOfficialMethod::crJXJFirstWeekGenerateCountryOfficialMethod():
	m_this(NULL),
	m_isFirstWeek(false),
	m_shiliID(0)
{
}

JXJ::crJXJFirstWeekGenerateCountryOfficialMethod::crJXJFirstWeekGenerateCountryOfficialMethod( const crJXJFirstWeekGenerateCountryOfficialMethod & handle ):
	crMethod(handle),
	m_isFirstWeek(handle.m_isFirstWeek),
	m_shiliID(handle.m_shiliID)
{
}

void JXJ::crJXJFirstWeekGenerateCountryOfficialMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_isFirstWeek = (bool )(LOINT64(param64));
			m_shiliID = (unsigned char )(HIINT64(param64));
		}
		else
		{
			m_isFirstWeek = false;
			m_shiliID = 0;
		}
		break;
	}
}

void JXJ::crJXJFirstWeekGenerateCountryOfficialMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJFirstWeekGenerateCountryOfficialMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this || !m_isFirstWeek || !m_shiliID) break;
		crData *gsBrainData = m_this->getDataClass();
		if (!gsBrainData) break;
		m_this->doEvent(WCH_JXJOnlineSynToRank);

		std::string country[] = {"魏国","蜀国","吴国"};
		std::string shoudu[] = {"邺城","成都","建业"};
		std::string leader[] = {"曹操","刘备","孙坚"};

		unsigned char shiliOffset = m_shiliID - c_startShiliID;
		time_t nowTime = time(0);
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		gsBrainData->getParam(WCHDATA_JXJShiliWei +shiliOffset,param);
		crJXJShili * shili = (crJXJShili *)param;
		crData * shiliData = shili->getDataClass();
		shiliData->excHandle(MAKEINT64(WCH_LockData,1));
		shiliData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
		CandidateVec*candidateVecL = (CandidateVec*)param;
		candidateVecL->clear();
		if (m_isFirstWeek)
		{
			// 开服第一周 从功勋榜获取
			gsBrainData->getParam(WCHDATA_JXJExpRankingMap,param);
			ExpRankingMap & shiliExpRanking = *(ExpRankingMap *)param;
			for (ExpRankingMap::iterator emPtr = shiliExpRanking.begin();
				emPtr != shiliExpRanking.end(); ++ emPtr)
			{
				if (emPtr->second->getShiliID() == m_shiliID)
				{
					ref_ptr<strToupiaoInfo> ptr = new strToupiaoInfo;
					ptr->m_lastLogin = nowTime;
					ptr->playerName = emPtr->second->getName();
					ptr->nPlayerID = emPtr->second->getPlayerID();
					candidateVecL->push_back(ptr);
				}
				if (candidateVecL->size() >= 3)
				{
					break;
				}
			}
		}
		else
		{
			shiliData->getParam(WCHDATA_JXJCountryCandidateVecT,param);
			CandidateVec*candidateVecT = (CandidateVec*)param;
			if(!candidateVecT->empty())
			{
				gsBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
				LevelRankingMap *levelRankingMap = (LevelRankingMap *)param;
				for( CandidateVec::iterator wbItr = candidateVecT->begin();
					wbItr != candidateVecT->end(); 
					++ wbItr )
				{
					int rankVal = 0;
					LevelRankingMap::iterator lrmItr = levelRankingMap->begin(); 
					for (;lrmItr != levelRankingMap->end(); ++ rankVal,++ lrmItr)
					{
						if (lrmItr->second.valid())
						{
							if (lrmItr->second->getPlayerID() == (*wbItr)->nPlayerID)
							{
								break;
							}
						}
					}
					(*wbItr)->m_rankVal = rankVal + 1;
				}
				std::sort(candidateVecT->begin(),candidateVecT->end(),strToupiaoInfoCmp());
				for (CandidateVec::iterator cvItr = candidateVecT->begin();
					cvItr != candidateVecT->end(); ++ cvItr)
				{
					candidateVecL->push_back(*cvItr);
				}
				candidateVecT->clear();
			}
		}
		unsigned char size = candidateVecL->size();
		shiliData->getParam(WCHDATA_JXJCountryHistory,param);
		CountryHistory *history = (CountryHistory *)param;

		for (CandidateVec::iterator cvLItr = candidateVecL->begin();
			cvLItr != candidateVecL->end(); ++ cvLItr)
		{
			(*cvLItr)->m_lastLogin = nowTime;
		}
		int id;
		std::string name;
		typedef std::vector<std::string> OfficalVec;
		OfficalVec officalVec;
		officalVec.resize(3);
		if (size >= 1)
		{
			id = (*candidateVecL)[0]->nPlayerID;
			name = (*candidateVecL)[0]->playerName;
			shiliData->inputParam(WCHDATA_JXJChengxiang,&id);
			shiliData->inputParam(WCHDATA_JXJChengxiangName,&name);
			//shiliData->inputParam(WCHDATA_JXJChengxiangLastLogin,&nowTime);
			//candidateVecL->erase(candidateVecL->begin());

			CRCore::ref_ptr<CountryHistoryOfficalChange > chocPtr = new CountryHistoryOfficalChange;
			chocPtr->m_playerName = name;
			chocPtr->m_officalName = "丞相";
			history->push_back(chocPtr.get());
			officalVec[0] = name;

			char log[128];
			memset(log,0,sizeof(log));
			sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());

			std::string logdata(log);
			GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));

			if (size >= 2)
			{
				id = (*candidateVecL)[1]->nPlayerID;
				name = (*candidateVecL)[1]->playerName;
				shiliData->inputParam(WCHDATA_JXJDasima,&id);
				shiliData->inputParam(WCHDATA_JXJDasimaName,&name);
				//shiliData->inputParam(WCHDATA_JXJDasimaLastLogin,&nowTime);
				//candidateVecL->erase(candidateVecL->begin());

				chocPtr = new CountryHistoryOfficalChange;
				chocPtr->m_playerName = name;
				chocPtr->m_officalName = "大司马";
				history->push_back(chocPtr.get());
				officalVec[1] = name;

				sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());
				logdata = log;
				GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));

				if (size >= 3)
				{
					id = (*candidateVecL)[2]->nPlayerID;
					name = (*candidateVecL)[2]->playerName;
					shiliData->inputParam(WCHDATA_JXJDajiangjun,&id);
					shiliData->inputParam(WCHDATA_JXJDajiangjunName,&name);
					//shiliData->inputParam(WCHDATA_JXJDajiangjunLastLogin,&nowTime);
					//candidateVecL->erase(candidateVecL->begin());

					chocPtr = new CountryHistoryOfficalChange;
					chocPtr->m_playerName = name;
					chocPtr->m_officalName = "大将军";
					history->push_back(chocPtr.get());
					officalVec[2] = name;

					sprintf(log,"%d %s 成为 %s\0",id,name.c_str(),chocPtr->m_officalName.c_str());
					logdata = log;
					GameLogData gamelog(Log_GenerateCountryOfficial,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(id,&gamelog));
				}
				else
				{
					shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
					shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
				}
			}
			else
			{
				shiliData->inputParam(WCHDATA_JXJDasima,NULL);
				shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
			}
		}
		else
		{
			shiliData->inputParam(WCHDATA_JXJChengxiang,NULL);
			shiliData->inputParam(WCHDATA_JXJChengxiangName,NULL);
			shiliData->inputParam(WCHDATA_JXJDasima,NULL);
			shiliData->inputParam(WCHDATA_JXJDasimaName,NULL);
			shiliData->inputParam(WCHDATA_JXJDajiangjun,NULL);
			shiliData->inputParam(WCHDATA_JXJDajiangjunName,NULL);
		}
		bool modify = true;
		shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
		shiliData->excHandle(MAKEINT64(WCH_LockData,0));
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		if (m_isFirstWeek)
		{
			// 发建国公告
			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec textrecord;
			crTableIO::StrVec record;
			if(textTab.valid())
			{
				char szNotice[256];
				int textid = 4103;
				if (textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
				{
					sprintf_s(szNotice,textrecord[1].c_str(),
						country[shiliOffset].c_str(),
						shoudu[shiliOffset].c_str(),
						country[shiliOffset].c_str(),
						leader[shiliOffset].c_str(),
						officalVec[0].c_str(),
						officalVec[1].c_str(),
						officalVec[2].c_str());
					CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
						szNotice,m_shiliID,0);
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	} while (0);
}

/**************************************************************************

METHOD: crJXJKictOutGroupMemberByGMMethod
 
**************************************************************************/
JXJ::crJXJKictOutGroupMemberByGMMethod::crJXJKictOutGroupMemberByGMMethod():
	m_this(NULL),
	m_playerid(0)
{

}

JXJ::crJXJKictOutGroupMemberByGMMethod::crJXJKictOutGroupMemberByGMMethod( const crJXJKictOutGroupMemberByGMMethod & handle ):
	crMethod(handle),
	m_playerid(handle.m_playerid)
{

}

void JXJ::crJXJKictOutGroupMemberByGMMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerid = (int)(LOINT64(param64));
		}
		else
		{
			m_playerid = 0;
		}
		break;
	}

}

void JXJ::crJXJKictOutGroupMemberByGMMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJKictOutGroupMemberByGMMethod::operator()( crHandle &handle )
{
	if (m_this && m_playerid)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		void *param;
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = (ConsortiaMap *)param;
		crData *groupdata = NULL;

		bool suc = false;

		for (ConsortiaMap::iterator cmItr = consortiamap->begin();
			cmItr != consortiamap->end(); ++ cmItr)
		{
			if (cmItr->second.valid())
			{
				groupdata = cmItr->second->getDataClass();
				groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
				ConsortiaMemberMap::iterator itr ;
				itr = membermap->find(m_playerid);
				if (itr != membermap->end())
				{
					if (itr->second->getGroupPos() == JXJGroup_FirstLeader)
					{
						// 不允许弹劾军团长 否则会出现问题
						break;
					}
					crPlayerDataEventPacket packet;
					ref_ptr<crGameServerPlayerData> playerData 
						= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_playerid));
					if(playerData.valid() && playerData->getPlayerGameData())
					{
						crData *data = playerData->getPlayerGameData()->getDataClass();
						if(data )
						{
							data -> inputParam(WCHDATA_JXJConsortiaID,0);
							crPlayerDataEventPacket::buildReplyPacket(packet,m_playerid,WCH_JXJClientLeaveGroup,NULL);
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}

					membermap->erase(itr);
					bool modify = true;
					groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);

					cmItr->second->updateLeaderVec();
					cmItr->second->updateToRank(gsBrainData);
					suc = true;

					break;
				}
			}
		}

		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		///游戏日志
		std::string logdata = "【GM 操作】离开军团 ，玩家ID: " + crArgumentParser::appItoa(m_playerid) + "是否操作成功 ： " + crArgumentParser::appItoa(suc);
		GameLogData gamelog(Log_LeaveGroup,logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(m_playerid,&gamelog));
	}
}

/**************************************************************************

METHOD: crJXJSaveOnlinePlayerEveryDayMethod
 
**************************************************************************/
JXJ::crJXJSaveOnlinePlayerEveryDayMethod::crJXJSaveOnlinePlayerEveryDayMethod():
	m_this(NULL)
{

}

JXJ::crJXJSaveOnlinePlayerEveryDayMethod::crJXJSaveOnlinePlayerEveryDayMethod( const crJXJSaveOnlinePlayerEveryDayMethod & handle ):
	crMethod(handle),
	m_onlinePlayerTab(handle.m_onlinePlayerTab)
{

}

void JXJ::crJXJSaveOnlinePlayerEveryDayMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSaveOnlinePlayerEveryDayMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_onlinePlayerTab = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJSaveOnlinePlayerEveryDayMethod::operator()( crHandle &handle )
{
	crData * serverBrainData = m_this->getDataClass();
	serverBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	void *param;
	serverBrainData->getParam(WCHDATA_JXJOnlineEveryDayPlayeridSet,param);
	OnlinePlayerSet * playerSet = (OnlinePlayerSet *)param;

	const time_t nowTime = time(NULL);
	struct tm nowTm = *localtime(&nowTime);
	if (nowTm.tm_hour == 0)
	{// 凌晨 清空数据，为下一天准备
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if (gameServer)
		{
			for (OnlinePlayerSet::iterator opsItr = playerSet->begin();
				opsItr != playerSet->end(); )
			{
				ref_ptr<crGameServerPlayerData> playerData 
					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(*opsItr));
				if(playerData.valid() == false)
				{
					// 不在线
					opsItr = playerSet->erase(opsItr);
				}
				else
				{
					++ opsItr;
				}
			}
		}
		else
		{
			playerSet->clear();
		}
	}
	else
	{
		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());

		char dateStr[128];
		strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));

		std::string filepath = crArgumentParser::getFilePath(m_onlinePlayerTab);
		std::string filename = crArgumentParser::getSimpleFileName(m_onlinePlayerTab);
		std::string filenameNoExtend = crArgumentParser::getFileNameEliminateExt(filename);

		filename = filenameNoExtend + "_" + std::string(dateStr) + ".tab";
		std::string relFileName = filepath + "/" + netCallback->getServerName() + "/" + filename;
		//ref_ptr<crTableIO> table = crTableIO::openFile(relFileName);
		//if (table.valid())
		//{
		//	// 删除之前的备份
		//	::DeleteFile(relFileName.c_str());
		//}

		ref_ptr<crTableIO> table = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowVec;
		titleVec.resize(1);
		rowVec.resize(1);
		titleVec[0] = "playerid";
		table->setTitleVec(titleVec);

		for (OnlinePlayerSet::iterator opsItr = playerSet->begin();
			opsItr != playerSet->end(); ++ opsItr)
		{
			rowVec[0] = crArgumentParser::appItoa(*opsItr);
			table->addData(rowVec);
		}
		table->saveToFileNoCook(relFileName);
	}

	serverBrainData->excHandle(MAKEINT64(WCH_LockData,0));
}

/**************************************************************************

METHOD: crJXJ7DaysGetAwardMethod
 
**************************************************************************/
JXJ::crJXJ7DaysGetAwardMethod::crJXJ7DaysGetAwardMethod():
	m_idx(0)
{

}

JXJ::crJXJ7DaysGetAwardMethod::crJXJ7DaysGetAwardMethod( const crJXJ7DaysGetAwardMethod & handle ):
	crMethod(handle),
	m_idx(0)
{

}

void JXJ::crJXJ7DaysGetAwardMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJ7DaysGetAwardMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_idx = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJ7DaysGetAwardMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(6);
		stream->_writeUChar(m_idx);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecv7DaysGetAward,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/**************************************************************************

METHOD: crJXJRecv7DaysGetAwardMethod
 
**************************************************************************/
JXJ::crJXJRecv7DaysGetAwardMethod::crJXJRecv7DaysGetAwardMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(Client)
{

}

JXJ::crJXJRecv7DaysGetAwardMethod::crJXJRecv7DaysGetAwardMethod( const crJXJRecv7DaysGetAwardMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(Client)
{

}

void JXJ::crJXJRecv7DaysGetAwardMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_netType = Client;
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
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void JXJ::crJXJRecv7DaysGetAwardMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecv7DaysGetAwardMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crData * data = m_this->getDataClass();
			unsigned char idx = m_stream->_readUChar();
			void *param;
			crData *roledata = m_this->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;

			unsigned char ret = JXJ7DaysGetAwardRet_unkwon;
			data->getParam(WCHDATA_JXJPlayerLoginDays,param);
			unsigned short loginDays = *(unsigned short*)param;

			data->getParam(WCHDATA_JXJ7DaysGetAward,param);
			unsigned char hadGotAward = *(unsigned char *)param;
			//unsigned char oldGotAwardVal = hadGotAward;

			data->getParam(WCHDATA_JXJVipLv, param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJ7DaysGetAwardOpenLv	,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				if (loginDays >= idx && idx <= 7)
				{
					if (hadGotAward & 1 << idx)
					{
						ret = JXJ7DaysGetAwardRet_hadGotAward;
					}
					else
					{
						std::string logdata = "领取7日奖励:";
						ret = JXJ7DaysGetAwardRet_suc;
						hadGotAward |= 1 << idx;
						data->inputParam(WCHDATA_JXJ7DaysGetAward,&hadGotAward);

						ref_ptr<crTableIO> _7DaysAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DaysAwardTab	);
						crTableIO::StrVec record;
						crTableIO::DataVec dataVec;

						if (_7DaysAwardTab.valid())
						{
							int awardIdx = _7DaysAwardTab->getTitleIndex("奖励");
							int orderIdx = _7DaysAwardTab->getTitleIndex("顺序");

							if (_7DaysAwardTab->queryOneRecord(orderIdx,crArgumentParser::appItoa(idx) ,record) >= 0)
							{
								crVector3i vec3;
								if(crArgumentParser::appAtoVec(record[awardIdx],vec3))
								{
									logdata += record[awardIdx];
									RewardItemVec rewardItems;
									CRCore::ref_ptr<crBagItemData> itemPtr = new crBagItemData;
									itemPtr->setItemID(vec3[0]);
									itemPtr->setEquipMagic(vec3[1]);
									itemPtr->setItemCount(vec3[2]);
									rewardItems.push_back(itemPtr);
									data->excHandle(MAKEINT64(WCH_LockData,1));
									m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
									data->excHandle(MAKEINT64(WCH_LockData,0));
								}
							}
						}
						GameLogData gamelog(Log_Get7DaysAward,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
					}
				}
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(playerData.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(6);
					stream->_writeUChar(ret);
					stream->_writeUChar(hadGotAward);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecv7DaysGetAward,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char ret = m_stream->_readUChar();
			unsigned char hadGotAward = m_stream->_readUChar();

			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				//crData * canvasData = canvas->getDataClass();
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				if (myPlayer && myPlayer->getDataClass())
				{
					crData * playerData = myPlayer->getDataClass();
					playerData->inputParam(WCHDATA_JXJ7DaysGetAward,&hadGotAward);

					canvas->doEvent(WCH_UI_UpdateData);
				}
				switch (ret)
				{
				case JXJFoundCountryGetAwardRet_suc:
					break;
				case JXJFoundCountryGetAwardRet_newerStepUncompleted:
					break;
				case JXJFoundCountryGetAwardRet_noOwnCity:
					break;
				case JXJFoundCountryGetAwardRet_noAllOwnCity:
					break;
				case JXJFoundCountryGetAwardRet_hadGotAward:
					break;
				case JXJFoundCountryGetAwardRet_lessOpenLevel:
					{
						int msgid = 4104;
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJRegisterStatisticsMethod
 
**************************************************************************/
JXJ::crJXJRegisterStatisticsMethod::crJXJRegisterStatisticsMethod():
	m_this(NULL)
{

}

JXJ::crJXJRegisterStatisticsMethod::crJXJRegisterStatisticsMethod( const crJXJRegisterStatisticsMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJRegisterStatisticsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRegisterStatisticsMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRegisterStatisticsMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		int curServerID = callback->getServerID();

		crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
		crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> accountSession;
		ref_ptr<crDataBase> db_session;

		ref_ptr<crJXJSelectAllAccountByCreateDateData> allAccountQuery = NULL;
		ref_ptr<crJXJSelectGMAccountByCreateDateData > gmAccountQuery = NULL;
		ref_ptr<crJXJSelectAllCharacterByCreateDateData > allCharacterQuery = NULL;
		ref_ptr<crJXJSelectGMCharacterByCreateDateData > gmCharacterQuery = NULL;
		int overdue = 0;

		int allAccount = 0;
		int allCharacter = 0;
		int gmAccount = 0;
		int gmCharacter = 0;


		int allAccountArray[3] = {0,0,0};
		int allCharacterArray[3] = {0,0,0};
		for (int i = 0; i < 3; ++i)
		{
			switch (i)
			{
			case 0:
				overdue = 0;
				break;
			case 1:
				overdue = 3;
				break;
			case 2:
				overdue = 7;
				break;
			default:
				break;
			}
			// 总注册
			accountSession = accountdb->beginSession();
			allAccountQuery = new crJXJSelectAllAccountByCreateDateData;
			allAccountQuery->buildQuerySql(overdue);
			accountSession->executeQuery(allAccountQuery.get());
			crDataBase::QueryResultVec &playerdataQueryResultVec = accountSession->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				allAccountQuery = dynamic_cast<crJXJSelectAllAccountByCreateDateData*>(itr->get());
				allAccount = allAccountQuery->getCount();
			}
			// gm 注册
			gmAccountQuery = new crJXJSelectGMAccountByCreateDateData ;
			gmAccountQuery ->buildQuerySql(curServerID,overdue,JXJGMPermission_newer);
			accountSession->executeQuery(gmAccountQuery .get());
			playerdataQueryResultVec = accountSession->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				gmAccountQuery = dynamic_cast<crJXJSelectGMAccountByCreateDateData *>(itr->get());
				gmAccount = gmAccountQuery ->getCount();
			}
			accountSession->releaseQuery();
			accountdb->endSession(accountSession.get());

			// 总创角
			db_session = global_db->beginSession();
			allCharacterQuery = new crJXJSelectAllCharacterByCreateDateData ;
			allCharacterQuery ->buildQuerySql(curServerID,overdue);
			db_session->executeQuery(allCharacterQuery .get());
			playerdataQueryResultVec = db_session->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				allCharacterQuery = dynamic_cast<crJXJSelectAllCharacterByCreateDateData *>(itr->get());
				allCharacter = allCharacterQuery ->getCount();
			}
			// gm 创角
			gmCharacterQuery = new crJXJSelectGMCharacterByCreateDateData ;
			gmCharacterQuery ->buildQuerySql(curServerID,JXJGMPermission_newer, overdue);
			db_session->executeQuery(gmCharacterQuery .get());
			playerdataQueryResultVec = db_session->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				gmCharacterQuery = dynamic_cast<crJXJSelectGMCharacterByCreateDateData *>(itr->get());
				gmCharacter = gmCharacterQuery->getCount();
			}
			db_session->releaseQuery();
			global_db->endSession(db_session.get());

			allAccountArray[i] = allAccount - gmAccount;
			allCharacterArray[i] = allCharacter - gmCharacter;
		}
		//// 查询charactername
	
		
		//留存人数
		ref_ptr<crTableIO> table = new crTableIO;
		crTableIO::StrVec titleVec;
		crTableIO::StrVec rowVec;
		titleVec.resize(4);
		rowVec.resize(4);
		titleVec[0] = "title";
		titleVec[1] = "今日";
		titleVec[2] = "三日";
		titleVec[3] = "七日";
		table->setTitleVec(titleVec);

		rowVec[0] = "总注册";
		rowVec[1] = crArgumentParser::appItoa(allAccountArray[0]);
		rowVec[2] = crArgumentParser::appItoa(allAccountArray[1]);
		rowVec[3] = crArgumentParser::appItoa(allAccountArray[2]);
		table->addData(rowVec);

		rowVec[0] = "总创建";
		rowVec[1] = crArgumentParser::appItoa(allCharacterArray[0]);
		rowVec[2] = crArgumentParser::appItoa(allCharacterArray[1]);
		rowVec[3] = crArgumentParser::appItoa(allCharacterArray[2]);
		table->addData(rowVec);
		
		const time_t nowTime = time(NULL);
		char dateStr[128];
		strftime(dateStr,sizeof(dateStr) - 1,"%Y%m%d",localtime(&nowTime));


		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());

		std::string tabName = std::string("../JXJDB/") + netCallback->getServerName() + "/" 
			+ std::string("总注册统计_") + std::string(dateStr) + std::string(".tab");
		table->saveToFileNoCook(tabName);
	}
}

/**************************************************************************

METHOD: crJXJ7DaysCanvasUpdateMethod
 
**************************************************************************/
JXJ::crJXJ7DaysCanvasUpdateMethod::crJXJ7DaysCanvasUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJ7DaysCanvasUpdateMethod::crJXJ7DaysCanvasUpdateMethod( const crJXJ7DaysCanvasUpdateMethod & handle ):
	crMethod(handle)
{
	for (unsigned char i = 0; i < 7; ++ i)
	{
		m_7Button[i] = handle.m_7Button[i];
		m_animation[i] = handle.m_animation[i];
		m_sw[i] = handle.m_sw[i];
	}
}

void JXJ::crJXJ7DaysCanvasUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJ7DaysCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_7Button[i] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		m_animation[i - 7] = str;
		break;
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		m_sw[i - 14] = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJ7DaysCanvasUpdateMethod::operator()( crHandle &handle )
{
	ref_ptr<crButtonWidgetNode> btn[7] = {NULL};
	ref_ptr<crSequence> dongHua[7] = {NULL};
	ref_ptr<crMultiSwitch> sw[7] = {NULL};


	ref_ptr<crTableIO> _7DaysAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DaysAwardTab	);
	crTableIO::StrVec awardRecord,itemRecord;
	crTableIO::DataVec dataVec;

	unsigned char _7DaysAward = 0;
	unsigned short loginDays = 0; 
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getDataClass())
	{
		crData * playerData = myPlayer->getDataClass();
		void *param;
		playerData->getParam(WCHDATA_JXJ7DaysGetAward,param);
		_7DaysAward = *(unsigned char *)param;
		playerData->getParam(WCHDATA_JXJPlayerLoginDays,param);
		loginDays = *(unsigned short *)param;
	}

	if (_7DaysAwardTab.valid())
	{
		int awardIdx = _7DaysAwardTab->getTitleIndex("奖励");
		int orderIdx = _7DaysAwardTab->getTitleIndex("顺序");
		crVector3i awardVec;
									
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int iconGrayIdx = itemtab->getTitleIndex("icon灰");
		int itemIDIdx = itemtab->getTitleIndex("itemid");



		bool disableState = false;
		for (unsigned char i = 0; i < 7; ++ i)
		{
			btn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_7Button[i]));
			dongHua[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_animation[i]));
			sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw[i]));

			disableState = false;
			if (loginDays >= i + 1)
			{
				if(_7DaysAward & 1 << (i + 1))
				{
					btn[i]->setVisiable(false);
					disableState = true;
					if (dongHua[i].valid())
						dongHua[i]->setVisiable(false);
				}
				else
				{
					// 该日未领取，显示动画
					if (dongHua[i].valid())
						dongHua[i]->setVisiable(true);
				}
			}
			else
			{
				if (dongHua[i].valid())
					dongHua[i]->setVisiable(false);
			}
			if (disableState)
			{
				continue;
			}

			// 只要没领取，就显示btn
			btn[i]->setVisiable(true);
			btn[i]->setEnable(true);
			if (sw[i].valid())
				sw[i]->setActiveSwitchSet(1);

			// 设置button 的图片
			if ( _7DaysAwardTab->queryOneRecord(orderIdx,crArgumentParser::appItoa(i + 1) ,awardRecord) >= 0 &&
				 crArgumentParser::appAtoVec(awardRecord[awardIdx],awardVec))
			{
				if (itemtab.valid() && itemtab->queryOneRecord(itemIDIdx,crArgumentParser::appItoa(awardVec[0]),itemRecord) >= 0)
				{
					std::string iconfile1 = itemRecord[iconindex];
					std::string iconfile2 = itemRecord[iconGrayIdx];
					if (disableState && iconfile2 != "")
					{
						// 禁用状态 使用灰色图片
						iconfile1 = iconfile2;
					}

					for (int j = 0; j < 2; j++)
					{
						crStateSet* ss1 = dynamic_cast<crObject *> (dynamic_cast<crGroup *>((btn[i]->getChild(0)))->getChild(j))->getDrawable(0)->getStateSet();
						crTexture2D* tex2d = dynamic_cast<crTexture2D *>(ss1->getTextureAttribute(0,crStateAttribute::TEXTURE));
						if(tex2d)
						{
							if(tex2d->getImageNameID().compare(iconfile1)!=0)
								tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
						}
						else
						{
							tex2d = new crTexture2D;
							tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
							tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
							tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
							tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
							tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
							tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
							ss1->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
						}
					}
				}
			}
			
		}
	}
}


/************************************************************************/
/*    crJXJUI7DaysAwardTipsMethod                                                                  */
/************************************************************************/
JXJ::crJXJUI7DaysAwardTipsMethod::crJXJUI7DaysAwardTipsMethod():
	m_this(NULL)
{
}

JXJ::crJXJUI7DaysAwardTipsMethod::crJXJUI7DaysAwardTipsMethod( const crJXJUI7DaysAwardTipsMethod & handle ):
	crMethod(handle),
	m_tips (handle.m_tips),
	m_name (handle.m_name),
	m_info2 (handle.m_info2),
	m_itemlv(handle.m_itemlv),
	m_itemicon(handle.m_itemicon),
	m_itemequipon(handle.m_itemequipon),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets (handle.m_offsets),
	m_colorred (handle.m_colorred),
	m_index (handle.m_index)
{

}

void JXJ::crJXJUI7DaysAwardTipsMethod::addParam( int i, const std::string& str )
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
		m_index = atoi(str.c_str());
		break;
	}
}

void JXJ::crJXJUI7DaysAwardTipsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUI7DaysAwardTipsMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid())
	{
		void *param;
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
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
		//crData *data = crBrain::getInstance()->getDataClass();
		crTableIO::DataVec recordVec;
		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
		crVector3i awardVec;
		if (canvasdata)
		{
			ref_ptr<crTableIO> _7DaysAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DaysAwardTab	);
			int awardIdx = _7DaysAwardTab->getTitleIndex("奖励");
			int orderIdx = _7DaysAwardTab->getTitleIndex("顺序");
			crTableIO::StrVec awardRecord,itemRecord;

			if (_7DaysAwardTab->queryOneRecord(orderIdx,crArgumentParser::appItoa(m_index) ,awardRecord) >= 0)
			{
				crArgumentParser::appAtoVec(awardRecord[awardIdx],awardVec);
			}
		}
		if (awardVec[0] > 0)
		{
			int iconindex = itemtab->getTitleIndex("icon");
			int nameindex = itemtab->getTitleIndex("name");
			int typeindex = itemtab->getTitleIndex("类型");
			//int colorindex = itemtab->getTitleIndex("品质");
			int needlvindex = itemtab->getTitleIndex("人物等级");
			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(awardVec[0]),record) >= 0)
			{
				//int colormodeid = atoi(record[colorindex].c_str());
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
					crVector2i item(awardVec[0],awardVec[1]);
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
				//const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
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
					needmove = boxYlength - texheight + diYlength * 0.25;
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
				posy2 = pos2[1] + posy - needmove;// + posinfoy;
			}
			crVector2 mouse(crVector2(posx2,posy2));

			//float needmovex = 0;
			//if (m_index == 1)
			//{
			//	needmovex += boxXlength;
			//}
			//tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f)/* * mat*/);
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
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

/**************************************************************************

METHOD: crJXJSetBuildingFlagVisiableMethod
 
**************************************************************************/
JXJ::crJXJSetBuildingFlagVisiableMethod::crJXJSetBuildingFlagVisiableMethod():
	m_init(false)
{

}

JXJ::crJXJSetBuildingFlagVisiableMethod::crJXJSetBuildingFlagVisiableMethod( const crJXJSetBuildingFlagVisiableMethod & handle ):
	m_init(false)
{
	for (unsigned i = 0; i < 4; ++ i)
	{
		m_buildingID[i] = handle.m_buildingID[i];
	}
}

void JXJ::crJXJSetBuildingFlagVisiableMethod::operator()( crHandle &handle )
{
	CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
	if (!m_init)
	{
		crCollectNodeBYNameVisitor collector(MULTISWITCH);
		collector.insertNodeNameId("Work");
		crNode *constructtionNode;
		for (int i = 0; i < 4; i++)
		{
			CRNetApp::crInstanceItem *item  = scene->findSceneItem(m_buildingID[i]);
			if (item)
			{
				m_buildings[i] = item;
				collector.clearResult();
				constructtionNode = item->getRelNode();
				constructtionNode->accept(collector);
				m_nodeSwArray[i] = collector.getResult();
			}
			else
			{
				m_buildings[i] = NULL;
			}
		}
		m_init = true;
	}

	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if (playerData && scene)
	{
		crVector4i timer;
		void *param;
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJExtraProduceMap,param);
		ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;

		for (unsigned i = 0; i < 4; ++ i)
		{
			if (m_buildings[i].valid())
			{
				unsigned char showFlag = 0;
				crMultiSwitch * workSw = NULL;
				ExtraProduceMap::iterator findItr = extraProduceMap->find(i + T_Food);
				if (findItr != extraProduceMap->end())
					showFlag = 1;

				for( NodeArray::iterator itr = m_nodeSwArray[i].begin();
					itr != m_nodeSwArray[i].end();
					++itr )
				{
					workSw = dynamic_cast<crMultiSwitch *>(itr->get());
					workSw->setActiveSwitchSet(showFlag);
				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

void JXJ::crJXJSetBuildingFlagVisiableMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJSetBuildingFlagVisiableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_buildingID[i] = atoi(str.c_str());
		break;
	default:
		break;
	}
}
