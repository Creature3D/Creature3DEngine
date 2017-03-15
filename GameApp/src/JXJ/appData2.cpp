/**********************************************************************
*
*	文件:	   appData2.cpp
*
*	描述:	   
*
*	作者:	   李婧
*					
*
**********************************************************************/
#include <JXJ/appData2.h>
#include <JXJ/appDataParam.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetGameData.h>
#include <rcfg/ConfigScriptOO.h>
#include <windows.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;

/////////////////////////////////////////
//
//crJXJRegisterMoveUIData
//
/////////////////////////////////////////
crJXJRegisterMoveUIData::crJXJRegisterMoveUIData():
	m_bMoveFlag(false),
	m_bHaveRandomed(true),
	playName(""),
	m_createTimer(25.0f)
{
}
crJXJRegisterMoveUIData::crJXJRegisterMoveUIData(const crJXJRegisterMoveUIData& data):
	crUIData(data),
	m_bMoveFlag(data.m_bMoveFlag),
	m_bHaveRandomed(data.m_bHaveRandomed),
	playName(data.playName),
	m_createTimer(data.m_createTimer)
{
}
void crJXJRegisterMoveUIData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJCreateTimer:
		m_createTimer = atoi(relStr.c_str());
		break;
	}
}
void crJXJRegisterMoveUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRegisterMoveFlag:
		m_bMoveFlag = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJHaveRandomed:
		m_bHaveRandomed = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJRandomPlayName:
		playName = param?*((std::string*)param):"";
		break;
	case WCHDATA_JXJCreateTimer:
		m_createTimer = param?*((float*)param):0.0f;
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJRegisterMoveUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRegisterMoveFlag:
		param = &m_bMoveFlag;
		break;
	case WCHDATA_JXJHaveRandomed:
		param = &m_bHaveRandomed;
		break;
	case WCHDATA_JXJRandomPlayName:
		param = &playName;
		break;
	case WCHDATA_JXJCreateTimer:
		param = &m_createTimer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}	
/////////////////////////////////////////
//
//crJXJGameChatUIData
//
/////////////////////////////////////////
crJXJGameChatUIData::crJXJGameChatUIData():
	m_timer_1(0.0f), m_timer_2(0.0f), m_timer_3(0.0f), m_timer_4(0.0f), m_timer_5(0.0f), m_timer_fade(0.0f),m_isToShow(false),m_isShowAchieve(false),m_timerHide(0.0f),m_oldHKL(NULL)
{
	//m_chatRecordMap.clear();
}

crJXJGameChatUIData::crJXJGameChatUIData(const crJXJGameChatUIData& data):
	crUIData(data),
	m_chatmode(data.m_chatmode),
	m_timer_1(data.m_timer_1), m_timer_2(data.m_timer_2), m_timer_3(data.m_timer_3), 
	m_timer_4(data.m_timer_4), m_timer_5(data.m_timer_5), m_timer_fade(data.m_timer_fade),
	m_chatInputDeque(data.m_chatInputDeque),
	m_isToShow(data.m_isToShow),m_isToShowItemIDVec(data.m_isToShowItemIDVec),
	m_isShowAchieve(data.m_isShowAchieve),m_AchieveIDVec(data.m_AchieveIDVec),
	m_timerHide(data.m_timerHide),
	m_oldHKL(data.m_oldHKL)
{}

void crJXJGameChatUIData::addParam(int i, const std::string& str)
{}

void crJXJGameChatUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJOldHKLValue:
		m_oldHKL = *(HKL *)param;
		break;
	}
}

void crJXJGameChatUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJChatTimer1:
		param = &m_timer_1;
		break;
	case WCHDATA_JXJChatTimer2:
		param = &m_timer_2;
		break;
	case WCHDATA_JXJChatTimer3:
		param = &m_timer_3;
		break;
	case WCHDATA_JXJChatTimer4:
		param = &m_timer_4;
		break;
	case WCHDATA_JXJChatTimer5:
		param = &m_timer_5;
		break;
	case WCHDATA_JXJChatTimerFade:
		param = &m_timer_fade;
		break;
	case WCHDATA_JXJChatInputRecordDeque:
		param = &m_chatInputDeque;
		break;
	case WCHDATA_JXJIsToShowItems:
		param = &m_isToShow;
		break;
	case WCHDATA_JXJAboutToShowItemIDVec:
		param = &m_isToShowItemIDVec; 
		break;
	case WCHDATA_JXJChatCanvasAutoHideTimer:
		param = &m_timerHide;
		break;
	case WCHDATA_JXJOldHKLValue:
		param = &m_oldHKL;
		break;
	case WCHDATA_JXJIsToShowAchieve:
		param = &m_isShowAchieve;
		break;
	case WCHDATA_JXJShowAchieveIDVec:
		param = &m_AchieveIDVec; 
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJGameChatUIData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJBuildingHeadCanvasData
//
/////////////////////////////////////////
crJXJBuildingHeadCanvasData::crJXJBuildingHeadCanvasData():
	m_building_item(NULL)
{
}
crJXJBuildingHeadCanvasData::crJXJBuildingHeadCanvasData(const crJXJBuildingHeadCanvasData& data):
	crUIData(data),
	m_buildingid(data.m_buildingid),
	m_building_item(NULL)
{
}
void crJXJBuildingHeadCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJBuildingHeadCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBuildingID:
		m_buildingid = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCanvasBuildingItem:
		m_building_item = (crInstanceItem *)param;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBuildingHeadCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBuildingID:
		param = &m_buildingid;
		break;
	case WCHDATA_JXJCanvasBuildingItem:
		param = m_building_item;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}	
/////////////////////////////////////////
//
//crJXJCheckBuildingCanvasData
//
/////////////////////////////////////////
crJXJCheckBuildingCanvasData::crJXJCheckBuildingCanvasData()
	:m_building_item(NULL),
	m_sellsourcetype(0),
	m_sellsourcecount(0),
	m_getsourcetype(0)
{
}
crJXJCheckBuildingCanvasData::crJXJCheckBuildingCanvasData(const crJXJCheckBuildingCanvasData& data):
	crUIData(data),
	m_buildingid(data.m_buildingid),
	m_goldbuySteps(data.m_goldbuySteps),
	m_coppersaleSteps(data.m_coppersaleSteps),
	m_building_item(data.m_building_item),
	m_tuZhiVec(data.m_tuZhiVec),
	m_ResourceCanExchangeVec(data.m_ResourceCanExchangeVec),
	m_sellsourcetype(data.m_sellsourcetype),
	m_sellsourcecount(data.m_sellsourcecount),
	m_getsourcetype(data.m_getsourcetype)
{
}
void crJXJCheckBuildingCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJCheckBuildingCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		m_buildingid = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCanvasBuildingItem:
		m_building_item = param ? (crInstanceItem *)param : NULL ;
		break;
	case WCHDATA_JXJSellResourceType:
		m_sellsourcetype = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJSellResourceCount:
		m_sellsourcecount = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJGetResourceType:
		m_getsourcetype = param ? *((unsigned char*)param) : NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJCheckBuildingCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		param = &m_buildingid;
		break;
	case WCHDATA_JXJGoldBuySteps:
		param = &m_goldbuySteps;
		break;
	case WCHDATA_JXJCopperSaleSteps:
		param = &m_coppersaleSteps;
		break;
	case WCHDATA_JXJCanvasBuildingItem:
		param = m_building_item;
		break;
	case WCHDATA_JXJGongFangTuZhiVec:
		param = &m_tuZhiVec;
		break;
	case WCHDATA_JXJResourceCanExchangeVec:
		param = &m_ResourceCanExchangeVec;
		break;
	case WCHDATA_JXJSellResourceType:
		param = &m_sellsourcetype;
		break;
	case WCHDATA_JXJSellResourceCount:
		param = &m_sellsourcecount;
		break;
	case WCHDATA_JXJGetResourceType:
		param = &m_getsourcetype;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

/////////////////////////////////////////
//
//crJXJCheckAddFriendRequestData
//
/////////////////////////////////////////
crJXJCheckAddFriendRequestData::crJXJCheckAddFriendRequestData()
{
}
crJXJCheckAddFriendRequestData::crJXJCheckAddFriendRequestData(const crJXJCheckAddFriendRequestData& data):
	crUIData(data),
	m_addFriendRequest(data.m_addFriendRequest)
{
}
void crJXJCheckAddFriendRequestData::addParam(int i, const std::string& str)
{

}
void crJXJCheckAddFriendRequestData::inputParam(int i, void *param)
{
}
void crJXJCheckAddFriendRequestData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJFriendRequestDeq:
		param = &m_addFriendRequest;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJCheckAddFriendRequestData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// mail data class
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
crJXJUIMailData::crJXJUIMailData()
	: /*send_to_id(-1), send_to_name(std::string("")), mail_canvas("UI_Beta2_Mail"),*/
	total_page(1), /*page_num(5),*/ cur_page(1), m_state_sel(MS_ALL)
{}
crJXJUIMailData::crJXJUIMailData(const crJXJUIMailData& data)
	: /*send_to_id(data.send_to_id), send_to_name(data.send_to_name), mail_canvas("UI_Beta2_Mail"),*/
	total_page(1), /*page_num(5),*/ cur_page(1), m_state_sel(MS_ALL)
{}
void crJXJUIMailData::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case WCHDATA_JXJMailTotalPage:
		total_page = atoi(str.c_str());
		break;
	//case WCHDATA_JXJMailPerPage:
	//	page_num = atoi(str.c_str());
	//	break;
	case WCHDATA_JXJMailCurPage:
		cur_page = atoi(str.c_str());
		break;
	case WCHDATA_JXJMailTabSel:
		m_state_sel = (MAIL_STATE)atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIMailData::inputParam(int i, void *param)
{
}
void crJXJUIMailData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJMailTotalPage:
		param = &total_page;
		break;
	//case WCHDATA_JXJMailPerPage:
	//	param = &page_num;
	//	break;
	case WCHDATA_JXJMailCurPage:
		param = &cur_page;
		break;
	default:
		break;
	}
}
void crJXJUIMailData::prepare_send_to(int id, std::string name)
{
	this->send_to_id = id;
	this->send_to_name = std::string(name);
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJBirthPointButtonData
//
//////////////////////////////////////////////////////////////////////////
crJXJBirthPointButtonData::crJXJBirthPointButtonData():m_birthPos(0,0,0)
{
}
crJXJBirthPointButtonData::crJXJBirthPointButtonData(const crJXJBirthPointButtonData& data):
	crUIData(data),
	m_birthPos(data.m_birthPos)
{
}
void crJXJBirthPointButtonData::addParam(int i, const std::string& str)
{

}
void crJXJBirthPointButtonData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBirthPointButtonData:
		m_birthPos = param?*((crVector3i*)param):crVector3i(0,0,0);
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBirthPointButtonData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBirthPointButtonData:
		param = &m_birthPos;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJBigMapCityData
//
//////////////////////////////////////////////////////////////////////////
crJXJBigMapCityData::crJXJBigMapCityData()
{
}
crJXJBigMapCityData::crJXJBigMapCityData(const crJXJBigMapCityData& data):
	crUIData(data),
	m_cityIndex(data.m_cityIndex)
{
}
void crJXJBigMapCityData::addParam(int i, const std::string& str)
{

}
void crJXJBigMapCityData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBigMapCity:
		m_cityIndex = param?*((char*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBigMapCityData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBigMapCity:
		param = &m_cityIndex;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}	
/////////////////////////////////////////
//
//crJXJSellJiangkaCanvasData
//
/////////////////////////////////////////
crJXJSellJiangkaCanvasData::crJXJSellJiangkaCanvasData()
{
}
crJXJSellJiangkaCanvasData::crJXJSellJiangkaCanvasData(const crJXJSellJiangkaCanvasData& data):
	crUIData(data),
	m_jiangkaVec(data.m_jiangkaVec)
{
}
void crJXJSellJiangkaCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJSellJiangkaCanvasData::inputParam(int i, void *param)
{
}
void crJXJSellJiangkaCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJSellJiangkaVec:
		param = &m_jiangkaVec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJUIHyperTextItemData
//
/////////////////////////////////////////
crJXJUIHyperTextItemData::crJXJUIHyperTextItemData()
{
}
crJXJUIHyperTextItemData::crJXJUIHyperTextItemData(const crJXJUIHyperTextItemData& data):
	crUIData(data),
	m_itemvec(data.m_itemvec)
{
}
void crJXJUIHyperTextItemData::addParam(int i, const std::string& str)
{
	if (i>=0)
	{
		crVector2 iteminfo;
		crArgumentParser::appAtoVec(str,iteminfo);
		ref_ptr<crBagItemData> bagitem = new crBagItemData;
		bagitem->setItemID((int)iteminfo[0]);
		bagitem->setItemCount((unsigned char)iteminfo[1]);
		m_itemvec.push_back(bagitem);
	}
}
void crJXJUIHyperTextItemData::inputParam(int i, void *param)
{
}
void crJXJUIHyperTextItemData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJHyperTextItemVec:
		param = &m_itemvec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJTavernBuildingCanvasData
//
/////////////////////////////////////////
crJXJTavernBuildingCanvasData::crJXJTavernBuildingCanvasData()
	:m_building_item(NULL),
	m_firstshowtimer(0),
	m_lastshowtimer(0),
	m_step(0),
	m_luckycardindex(0),
	m_ifknowcard(false)
{
	//for (int i = 0;i<2;i++)
	//{
	//	m_luckycard[i] = 0;
	//}
}
crJXJTavernBuildingCanvasData::crJXJTavernBuildingCanvasData(const crJXJTavernBuildingCanvasData& data):
	crUIData(data),
	m_buildingid(data.m_buildingid),
	m_building_item(data.m_building_item),
	m_firstshowtimer(data.m_firstshowtimer),
	m_lastshowtimer(data.m_lastshowtimer),
	m_step(data.m_step),
	m_ifknowcard(data.m_ifknowcard),
	m_luckycardindex(data.m_luckycardindex)
{
}
void crJXJTavernBuildingCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJTavernBuildingCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		m_buildingid = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCanvasBuildingItem:
		m_building_item = param ? (crInstanceItem *)param : NULL ;
		break;
	case WCHDATA_JXJLuckyCardFirstShowTimer:
		m_firstshowtimer = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardLastShowTimer:
		m_lastshowtimer = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardRefreshTimer:
		m_refreshtimer = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardPushTimer:
		m_pushtimer = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardStep:
		m_step = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardKnowCard:
		m_ifknowcard = param?*((bool*)param):NULL;
		break;	
	case WCHDATA_JXJLuckyCardIndex:
		m_luckycardindex = param?*((unsigned char*)param):NULL;
		break;	
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJTavernBuildingCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		param = &m_buildingid;
		break;
	case WCHDATA_JXJCanvasBuildingItem:
		param = m_building_item;
		break;
	case WCHDATA_JXJLuckyCardFirstShowTimer:
		param = &m_firstshowtimer;
		break;
	case WCHDATA_JXJLuckyCardLastShowTimer:
		param = &m_refreshtimer;
		break;
	case WCHDATA_JXJLuckyCardRefreshTimer:
		param = &m_firstshowtimer;
		break;
	case WCHDATA_JXJLuckyCardPushTimer:
		param = &m_pushtimer;
		break;
	case WCHDATA_JXJLuckyCardStep:
		param = &m_step;
		break;
	case WCHDATA_JXJLuckyCardKnowCard:
		param = &m_ifknowcard;
		break;
	case WCHDATA_JXJLuckyCardIndex:
		param = &m_luckycardindex;
		break;
	//case WCHDATA_JXJLuckyCardTemp:
	//	param = &m_luckycard[0];
	//	break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJBeautyDialogueCanvasData
//
/////////////////////////////////////////
crJXJBeautyDialogueCanvasData::crJXJBeautyDialogueCanvasData()
	:
	m_taskid(0),
	m_xumuid(0),
	m_dialoguenum(0),
	m_widgetnode(NULL),
	m_dataObject(NULL)
{
	//for (int i = 0;i<2;i++)
	//{
	//	m_luckycard[i] = 0;
	//}
}
crJXJBeautyDialogueCanvasData::crJXJBeautyDialogueCanvasData(const crJXJBeautyDialogueCanvasData& data):
	crUIData(data),
	m_taskid(data.m_taskid),
	m_xumuid(data.m_xumuid),
	m_dialoguenum(data.m_dialoguenum),
	m_widgetnode(data.m_widgetnode),
	m_dataObject(data.m_dataObject)
{

}
void crJXJBeautyDialogueCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJBeautyDialogueCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJNewHandTaskID:
		m_taskid = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJTalkID:
		m_xumuid = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJNewHandDialogNum:
		m_dialoguenum = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJClickWidget:
		m_widgetnode = param?((crWidgetNode*)param):NULL;
		if(m_widgetnode)
			m_dataObject = NULL;
		break;
	case WCHDATA_JXJClickWidget2:
		m_dataObject = param?((crDataObject*)param):NULL;
		if(m_dataObject)
			m_widgetnode = NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBeautyDialogueCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJNewHandTaskID:
		param = &m_taskid;
		break;
	case WCHDATA_JXJTalkID:
		param = &m_xumuid;
		break;
	case WCHDATA_JXJNewHandDialogNum:
		param = &m_dialoguenum;
		break;
	case WCHDATA_JXJClickWidget:
		param = m_widgetnode;
		break;
	case WCHDATA_JXJClickWidget2:
		param = m_dataObject;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJBeautyDialogueCanvasData::excHandle(_crInt64 msg)
{
	if(m_widgetnode)
		m_widgetnode->doEvent(msg);
	else if(m_dataObject)
		m_dataObject->doEvent(msg);
}
/////////////////////////////////////////
//
//crJXJRenWuCanvasData
//
/////////////////////////////////////////
crJXJRenWuCanvasData::crJXJRenWuCanvasData()
	:
	m_unfolden(true),
	m_xinshouyindaoFlag21(0),
	m_ArrowsUISelect(1),
	m_brainTargetCardCount(0),
	m_TargetBuildingID(0),
	m_newHandUpgradeCardid(0),
	m_newHandTroopPositionid(0)
{
}
crJXJRenWuCanvasData::crJXJRenWuCanvasData(const crJXJRenWuCanvasData& data):
	crUIData(data),
	m_unfolden(data.m_unfolden),
	m_xinshouyindaoFlag21(data.m_xinshouyindaoFlag21),
	m_ArrowsUISelect(data.m_ArrowsUISelect),
	m_brainTargetCardCount(data.m_brainTargetCardCount),
	m_TargetBuildingID(data.m_TargetBuildingID),
	m_newHandUpgradeCardid(data.m_newHandUpgradeCardid),
	m_newHandTroopPositionid(data.m_newHandTroopPositionid)
{

}
void crJXJRenWuCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJRenWuCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRenWuUnFolden:
		m_unfolden = param?*((bool*)param):NULL;
		break;	
	case WCHDATA_JXJNewHandTaskID:
		m_newHandTaskid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTargetCanvasName:
		m_targetCanvasName = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJTargetWidgetName:
		m_targetWidgetName = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJRectCanvasName:
		m_rectCanvasName = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJxinshouyindaoFlag21:
		m_xinshouyindaoFlag21 = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJArrowsUISelect:
		m_ArrowsUISelect = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJBrainTargetCardCount:
		m_brainTargetCardCount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTargetBuildingID:
		m_TargetBuildingID = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewHandUpgradeCardID:
		m_newHandUpgradeCardid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewHandTroopPositionID:
		m_newHandTroopPositionid = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJRenWuCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRenWuUnFolden:
		param = &m_unfolden;
		break;
	case WCHDATA_JXJNewHandTaskID:
		param = &m_newHandTaskid;
		break;
	case WCHDATA_JXJTargetCanvasName:
		param = &m_targetCanvasName;
		break;
	case WCHDATA_JXJTargetWidgetName:
		param = &m_targetWidgetName;
		break;
	case WCHDATA_JXJRectCanvasName:
		param = &m_rectCanvasName;
		break;
	case WCHDATA_JXJxinshouyindaoFlag21:
		param = &m_xinshouyindaoFlag21;
		break;
	case WCHDATA_JXJArrowsUISelect:
		param = &m_ArrowsUISelect;
		break;
	case WCHDATA_JXJBrainTargetCardCount:
		param = &m_brainTargetCardCount;
		break;
	case WCHDATA_JXJTargetBuildingID:
		param = &m_TargetBuildingID;
		break;
	case WCHDATA_JXJNewHandUpgradeCardID:
		param = &m_newHandUpgradeCardid;
		break;
	case WCHDATA_JXJNewHandTroopPositionID:
		param = &m_newHandTroopPositionid;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJCityMiniMenuData
//
/////////////////////////////////////////
crJXJCityMiniMenuData::crJXJCityMiniMenuData()
	:
	m_chengchiid(0)
{
}
crJXJCityMiniMenuData::crJXJCityMiniMenuData(const crJXJCityMiniMenuData& data):
	crUIData(data),
	m_chengchiid(data.m_chengchiid)
{

}
void crJXJCityMiniMenuData::addParam(int i, const std::string& str)
{

}
void crJXJCityMiniMenuData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCityMenuChengchiid:
		m_chengchiid = param?*((int*)param):NULL;
		break;	
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJCityMiniMenuData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCityMenuChengchiid:
		param = &m_chengchiid;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJBattleRoomInfoData
//
/////////////////////////////////////////
crJXJBattleRoomInfoData::crJXJBattleRoomInfoData():
	m_chengchiid(0)//,
	//m_shiliid(0),
	//m_chengfang(0),
	//m_shiliwin1(0),
	//m_shiliwin2(0),
	//m_shiliwin3(0),
	//m_roomnum(0)
{
}
crJXJBattleRoomInfoData::crJXJBattleRoomInfoData(const crJXJBattleRoomInfoData& data):
	crUIData(data),
	m_chengchiid(data.m_chengchiid),
	//m_shiliid(data.m_shiliid),
	//m_chengfang(data.m_chengfang),
	//m_shiliwin1(data.m_shiliwin1),
	//m_shiliwin2(data.m_shiliwin2),
	//m_shiliwin3(data.m_shiliwin3),
	//m_roomnum(data.m_roomnum),
	m_chengfanginfovec(data.m_chengfanginfovec),
	m_roomstatevec(data.m_roomstatevec)
{

}
void crJXJBattleRoomInfoData::addParam(int i, const std::string& str)
{

}
void crJXJBattleRoomInfoData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJChengfangChengchiid:
		m_chengchiid = param?*((unsigned short*)param):NULL;
		break;
	//case WCHDATA_JXJChengfangDefendShiliid:
	//	m_shiliid = param?*((unsigned char*)param):NULL;
	//	break;	
	//case WCHDATA_JXJChengfangNum:
	//	m_chengfang = param?*((unsigned short*)param):NULL;
	//	break;	
	//case WCHDATA_JXJChengfangWin1:
	//	m_shiliwin1 = param?*((unsigned short*)param):NULL;
	//	break;	
	//case WCHDATA_JXJChengfangWin2:
	//	m_shiliwin2 = param?*((unsigned short*)param):NULL;
	//	break;	
	//case WCHDATA_JXJChengfangWin3:
	//	m_shiliwin3 = param?*((unsigned short*)param):NULL;
	//	break;	
	//case WCHDATA_JXJChengfangOpenRoomNum:
	//	m_roomnum = param?*((unsigned char*)param):NULL;
	//	break;	
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBattleRoomInfoData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJChengfangChengchiid:
		param = &m_chengchiid;
		break;
	//case WCHDATA_JXJChengfangDefendShiliid:
	//	param = &m_shiliid;
	//	break;
	//case WCHDATA_JXJChengfangNum:
	//	param = &m_chengfang;
	//	break;
	//case WCHDATA_JXJChengfangWin1:
	//	param = &m_shiliwin1;
	//	break;
	//case WCHDATA_JXJChengfangWin2:
	//	param = &m_shiliwin2;
	//	break;
	//case WCHDATA_JXJChengfangWin3:
	//	param = &m_shiliwin3;
	//	break;
	//case WCHDATA_JXJChengfangOpenRoomNum:
	//	param = &m_roomnum;
	//	break;
	case WCHDATA_JXJChengfanginfovec:
		param = &m_chengfanginfovec;
		break;
	case WCHDATA_JXJRoomStateVec:
		param = &m_roomstatevec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJBattleRoomInfoData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJUIFxData
//
/////////////////////////////////////////
crJXJUIFxData::crJXJUIFxData():
	m_visiableTime(3.0f),
	m_curaccomplishid(0)
{
}
crJXJUIFxData::crJXJUIFxData(const crJXJUIFxData& data):
	crUIData(data),
	m_visiableTime(data.m_visiableTime),
	m_curaccomplishid(data.m_curaccomplishid)
{

}
void crJXJUIFxData::addParam(int i, const std::string& str)
{

}
void crJXJUIFxData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJVisiableTime:
		m_visiableTime = param?*((float*)param):NULL;
		break;	
	case WCHDATA_JXJCurAccomplishID:
		m_curaccomplishid = param?*((unsigned short*)param):NULL;
		break;	
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUIFxData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJVisiableTime:
		param = &m_visiableTime;
		break;
	case WCHDATA_JXJCurAccomplishID:
		param = &m_curaccomplishid;
		break;	
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUIFxData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	

/////////////////////////////////////////
//
//crJXJGetRenWuRewardData
//
/////////////////////////////////////////
crJXJGetRenWuRewardData::crJXJGetRenWuRewardData():
	m_taskid(0)
{
}
crJXJGetRenWuRewardData::crJXJGetRenWuRewardData(const crJXJGetRenWuRewardData& data):
	crUIData(data),
	m_taskid(data.m_taskid),
	m_renwurewardvec(data.m_renwurewardvec)
{

}
void crJXJGetRenWuRewardData::addParam(int i, const std::string& str)
{

}
void crJXJGetRenWuRewardData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJGetRenWuRewardID:
		m_taskid = param?*((int*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJGetRenWuRewardData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGetRenWuRewardID:
		param = &m_taskid;
		break;
	case WCHDATA_JXJRenwuRewardVec:
		param = &m_renwurewardvec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJGetRenWuRewardData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJUIBattlePunishData
//
/////////////////////////////////////////
crJXJUIBattlePunishData::crJXJUIBattlePunishData():
	m_punishtime(0),
	m_needgold(0)
{
}
crJXJUIBattlePunishData::crJXJUIBattlePunishData(const crJXJUIBattlePunishData& data):
	crUIData(data),
	m_punishtime(data.m_punishtime),
	m_needgold(data.m_needgold)
{

}
void crJXJUIBattlePunishData::addParam(int i, const std::string& str)
{

}
void crJXJUIBattlePunishData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBattlePunishTime:
		m_punishtime = param?*((short*)param):NULL;
		break;
	case WCHDATA_JXJClearPunishTimeNeedGold:
		m_needgold = param?*((int*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUIBattlePunishData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBattlePunishTime:
		param = &m_punishtime;
		break;
	case WCHDATA_JXJClearPunishTimeNeedGold:
		param = &m_needgold;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUIBattlePunishData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJFubenCanvasData
//
/////////////////////////////////////////
crJXJFubenCanvasData::crJXJFubenCanvasData():
	m_fubencurpage(1),
	m_fubentotalpage(1),
	m_selfubenid(0)
{
}
crJXJFubenCanvasData::crJXJFubenCanvasData(const crJXJFubenCanvasData& data):
	crUIData(data),
	m_fubencurpage(data.m_fubencurpage),
	m_fubentotalpage(data.m_fubentotalpage),
	m_selfubenid(data.m_selfubenid),
	m_fubenidvec(data.m_fubenidvec)
{

}
void crJXJFubenCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJFubenCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJFubenCurPage:
		m_fubencurpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJFubenTotalPage:
		m_fubentotalpage = param?*((int*)param):NULL;
		if (m_fubentotalpage < 1)
		{
			m_fubentotalpage = 1;
		}
		break;
	case WCHDATA_JXJSelFubenID:
		m_selfubenid = param?*((unsigned short*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJFubenCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJFubenCurPage:
		param = &m_fubencurpage;
		break;
	case WCHDATA_JXJFubenTotalPage:
		param = &m_fubentotalpage;
		break;
	case WCHDATA_JXJSelFubenID:
		param = &m_selfubenid;
		break;
	case WCHDATA_JXJCanvasFubenIDVec:
		param = &m_fubenidvec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJFubenCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	

/////////////////////////////////////////
//
//crJXJJueSeCanvasData
//
/////////////////////////////////////////
crJXJJueSeCanvasData::crJXJJueSeCanvasData():
	m_proFoodProduct(0),
	m_proWoodProduct(0),
	m_proIronProduct(0),
	m_proHorseProduct(0),
	m_proFoodProductadd(0),
	m_proWoodProductadd(0),
	m_proIronProductadd(0),
	m_proHorseProductadd(0)
{
}
crJXJJueSeCanvasData::crJXJJueSeCanvasData(const crJXJJueSeCanvasData& data):
	crUIData(data),
	m_proFoodProduct(data.m_proFoodProduct),
	m_proWoodProduct(data.m_proWoodProduct),
	m_proIronProduct(data.m_proIronProduct),
	m_proHorseProduct(data.m_proHorseProduct),
	m_proFoodProductadd(data.m_proFoodProductadd),
	m_proWoodProductadd(data.m_proWoodProductadd),
	m_proIronProductadd(data.m_proIronProductadd),
	m_proHorseProductadd(data.m_proHorseProductadd)
{

}
void crJXJJueSeCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJJueSeCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJProFoodProduct:
		m_proFoodProduct = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJProWoodProduct:
		m_proWoodProduct = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJProIronProduct:
		m_proIronProduct = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJProHorseProduct:
		m_proHorseProduct = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJProFoodProductAdd:
		m_proFoodProductadd = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJProWoodProductAdd:
		m_proWoodProductadd = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJProIronProductAdd:
		m_proIronProductadd = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJProHorseProductAdd:
		m_proHorseProductadd = param ? *((int*)param) : NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJJueSeCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJProFoodProduct:
		param = &m_proFoodProduct;
		break;
	case WCHDATA_JXJProWoodProduct:
		param = &m_proWoodProduct;
		break;
	case WCHDATA_JXJProIronProduct:
		param = &m_proIronProduct;
		break;
	case WCHDATA_JXJProHorseProduct:
		param = &m_proHorseProduct;
		break;
	case WCHDATA_JXJProFoodProductAdd:
		param = &m_proFoodProductadd;
		break;
	case WCHDATA_JXJProWoodProductAdd:
		param = &m_proWoodProductadd;
		break;
	case WCHDATA_JXJProIronProductAdd:
		param = &m_proIronProductadd;
		break;
	case WCHDATA_JXJProHorseProductAdd:
		param = &m_proHorseProductadd;
		break;
		/*case WCHDATA_JXJIsClickHongDongBtn:
		param = &m_ispushhuodong;
		break;*/
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJJueSeCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJGroupCanvasData
//
/////////////////////////////////////////
crJXJGroupCanvasData::crJXJGroupCanvasData():
	m_groupcurpage(1),
	m_grouptotalpage(1),
	m_curselectgroupid(0)
{
	//std::cout << "new ==============================";
}
crJXJGroupCanvasData::crJXJGroupCanvasData(const crJXJGroupCanvasData& data):
	crUIData(data),
	m_groupcurpage(data.m_groupcurpage),
	m_grouptotalpage(data.m_grouptotalpage),
	m_groupinfovec(data.m_groupinfovec),
	m_groupApplySet(data.m_groupApplySet),
	//m_myapplygroupinfoMap(data.m_myapplygroupinfoMap),
	//m_myapplygroupinfo(data.m_myapplygroupinfo),
	m_curselectgroupid(data.m_curselectgroupid)
{

}
void crJXJGroupCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJGroupCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJGroupCurPage:
		m_groupcurpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGroupTotalPage:
		m_grouptotalpage = param?*((int*)param):NULL;
		if (m_grouptotalpage < 1)
		{
			m_grouptotalpage = 1;
		}
		break;
	case WCHDATA_JXJCurSelectGroupID:
		m_curselectgroupid = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCheckGroupCurPage:
		m_groupMemberListCurPage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCheckGroupTotalPage:
		m_groupMemberListTotalPage = param?*((int*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJGroupCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGroupCurPage:
		param = &m_groupcurpage;
		break;
	case WCHDATA_JXJGroupTotalPage:
		param = &m_grouptotalpage;
		break;
	case WCHDATA_JXJGroupInfoVec:
		param = &m_groupinfovec;
		break;
	case WCHDATA_JXJMyApplyGroupInfoSet:
		param = &m_groupApplySet;
		break;
	case WCHDATA_JXJCurSelectGroupID:
		param = &m_curselectgroupid;
		break;
	case WCHDATA_JXJCheckGroupCurPage:
		param = &m_groupMemberListCurPage;
		break;
	case WCHDATA_JXJCheckGroupTotalPage:
		param = &m_groupMemberListTotalPage;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJGroupCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJMyGroupCanvasData
//
/////////////////////////////////////////
crJXJMyGroupCanvasData::crJXJMyGroupCanvasData():
	m_groupapplycurpage(1),
	m_groupapplytotalpage(1),
	m_groupmembercurpage(1),
	m_groupmembertotalpage(1),
	m_playerid(0),
	m_selmember(NULL),
	m_myselfInfo(NULL),
	m_fubenCurPage(1),
	m_fubenTotalPage(1),
	m_chengchiCurPage(1),
	m_chengchiTotalPage(1),
	m_myGroupPos(JXJGroup_Normal),
	m_hasNewApplyer(false),
	m_hadOpenShenHe(false),
	m_selectedCantonId (0),
	m_preSelectedIdx(-1),
	m_qubingBtnidx(0),
	m_fenFengCurPage(1)
{
}
crJXJMyGroupCanvasData::crJXJMyGroupCanvasData(const crJXJMyGroupCanvasData& data):
	crUIData(data),
	m_groupapplycurpage(data.m_groupapplycurpage),
	m_groupapplytotalpage(data.m_groupapplytotalpage),
	m_groupapplyvec(data.m_groupapplyvec),
	m_playerid(data.m_playerid),
	m_mygroupinfo(data.m_mygroupinfo),
	m_groupmembercurpage(data.m_groupmembercurpage),
	m_groupmembertotalpage(data.m_groupmembertotalpage),
	m_selmember(data.m_selmember),
	m_fubenCurPage(data.m_fubenCurPage),
	m_fubenTotalPage(data.m_fubenTotalPage),
	m_chengchiCurPage(data.m_chengchiCurPage),
	m_chengchiTotalPage(data.m_chengchiTotalPage),
	m_myGroupPos(data.m_myGroupPos),
	m_hasNewApplyer(data.m_hasNewApplyer),
	m_hadOpenShenHe(data.m_hadOpenShenHe),
	m_selectedCantonId (data.m_selectedCantonId),
	m_preSelectedIdx(data.m_preSelectedIdx),
	m_qubingBtnidx(data.m_qubingBtnidx),
	m_fenFengCurPage(data.m_fenFengCurPage)
{
	m_groupapplyvec.clear();
	m_activeMemberVec.clear();
	m_cantonGroupMap.clear();
	m_myApplyCantonVec.clear();
	/*m_startedFubenSet.clear();*/
	m_startedFubenMap.clear();
	m_allocateCantonVec.clear();
	m_cunruTroopsMap.clear();
	m_groupTroopsMap.clear();
}

void crJXJMyGroupCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJMyGroupCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJGroupApplyCurPage:
		m_groupapplycurpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGroupApplyTotalPage:
		m_groupapplytotalpage = param?*((int*)param):NULL;
		if (m_groupapplytotalpage < 1)
		{
			m_groupapplytotalpage = 1;
		}
		break;
	case WCHDATA_JXJGroupSelectPlayerid:
		m_playerid = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJMyGroupMemberCurPage:
		m_groupmembercurpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJMyGroupMemberTotalPage:
		m_groupmembertotalpage = param?*((int*)param):NULL;
		if (m_groupmembertotalpage < 1)
		{
			m_groupmembertotalpage = 1;
		}
		break;
	case WCHDATA_JXJMyGroupSelPlayData:
		m_selmember = param?(crJXJConsortiaMember *)param:NULL;
		break;
	case WCHDATA_JXJMySelfGroupMemberINfo:
		m_myselfInfo = param ? (crJXJConsortiaMember *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupPos:
		m_myGroupPos = param?*((unsigned char *)param):NULL;
		break;

	case WCHDATA_JXJMyGroupFuBenCurPage:		
		m_fubenCurPage = param? *(int *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupFuBenTotalPage:		
		m_fubenTotalPage = param? *(int *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupChengChiCurPage:
		m_chengchiCurPage = param ? *(int *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupChengChiTotalPage:			
		m_chengchiTotalPage = param ? *(int *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupFenFengCurPage:
		m_fenFengCurPage = param ? *(int *)param:NULL;
		break;
	case WCHDATA_JXJMyGroupFenFengTotalPage:
		m_fenFengTotalPage = param ? *(int *)param:NULL;
		break;
	case WCHDATA_JXJConsortiaHasNewApplyer:
		m_hasNewApplyer = param ? * (bool *)param:false;
		break;
	case WCHDATA_JXJConsortiaHadOpenShenHe		:
		m_hadOpenShenHe = param ? * (bool *)param:false;
		break;
	case WCHDATA_JXJGroupCanvasSelectedCantonId	:
		m_selectedCantonId = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJPreSelectedIdx:
		m_preSelectedIdx = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJQuBingBtnIdx:
		m_qubingBtnidx = param?*((unsigned char *)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}

void crJXJMyGroupCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGroupApplyCurPage:
		param = &m_groupapplycurpage;
		break;
	case WCHDATA_JXJGroupApplyTotalPage:
		param = &m_groupapplytotalpage;
		break;
	case WCHDATA_JXJConsortiaApplyVec:
		param = &m_groupapplyvec;
		break;
	case WCHDATA_JXJMyGroupInfo:
		param = &m_mygroupinfo;
		break;
	case WCHDATA_JXJGroupSelectPlayerid:
		param = &m_playerid;
		break;
	case WCHDATA_JXJMyGroupMemberCurPage:
		param = &m_groupmembercurpage;
		break;
	case WCHDATA_JXJMyGroupMemberTotalPage:
		param = &m_groupmembertotalpage;
		break;
	case WCHDATA_JXJMyGroupMemberVec:
		param = &m_memberVec;
		break;
	case WCHDATA_JXJMyGroupActiveMemberVec:
		param = &m_activeMemberVec;
		break;
	case WCHDATA_JXJMyGroupSelPlayData:
		param = m_selmember.get();
		break;
	case WCHDATA_JXJMySelfGroupMemberINfo:
		param = m_myselfInfo.get();
		break;
	case WCHDATA_JXJMyGroupPos:
		param = &m_myGroupPos;
		break;
	case WCHDATA_JXJMyGroupFuBenCurPage:		
		param = &m_fubenCurPage;
		break;
	case WCHDATA_JXJMyGroupFuBenTotalPage:
		param = &m_fubenTotalPage;
		break;
	//case WCHDATA_JXJMyGroupStartedFuBenSet:
	//	param = &m_startedFubenSet;
	//	break;
	case WCHDATA_JXJMyApplyCantonVec:
		param = &m_myApplyCantonVec;
		break;
	case WCHDATA_JXJMyGroupChengChiCurPage:
		param = &m_chengchiCurPage;
		break;
	case WCHDATA_JXJMyGroupChengChiTotalPage:			
		param = &m_chengchiTotalPage;
		break;
	case WCHDATA_JXJCantonGroupVec:
		param = & m_cantonGroupMap;
		break;
		/*case WCHDATA_JXJCantonFenFengChengChiSet:			
		param = & m_fenFengChengChiSet;
		break;*/
	case WCHDATA_JXJMyGroupFenFengCurPage:
		param = & m_fenFengCurPage;
		break;
	case WCHDATA_JXJMyGroupFenFengTotalPage:
		param = & m_fenFengTotalPage;
		break;
	case WCHDATA_JXJConsortiaHasNewApplyer:			
		param = & m_hasNewApplyer;
		break;
	case WCHDATA_JXJAllocateCantonVec:
		param = & m_allocateCantonVec;
		break;
	case WCHDATA_JXJConsortiaHadOpenShenHe:
		param = & m_hadOpenShenHe;
		break;
	case WCHDATA_JXJGroupCanvasSelectedCantonId	:
		param = & m_selectedCantonId;
		break;
	case WCHDATA_JXJGroupChengChiIdVec			:
		param = & m_chengChiIdVec;
		break;
	case WCHDATA_JXJHadWonGroupFuBenSet:
		param = & m_hadWonGroupFuBenSet;
		break;
	case WCHDATA_JXJPreSelectedIdx:
		param = & m_preSelectedIdx;
		break;
	case WCHDATA_JXJMyGroupStartedFuBenMap:
		param = & m_startedFubenMap;
		break;
	case WCHDATA_JXJCunruTroopsMap:
		param = & m_cunruTroopsMap;
		break;
	case WCHDATA_JXJGroupTroopsMap:
		param = & m_groupTroopsMap;
		break;
	case WCHDATA_JXJQuBingBtnIdx:
		param = & m_qubingBtnidx;
		break;
	case WCHDATA_JXJFenFengedChengChiMap:
		param = & m_fenFengChengChiMap;
		break;
	case WCHDATA_JXJGroupCanFengFenmemVec:
		param = & m_canFengfenmemberVec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJMyGroupCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJCheckGroupCanvasData
//
/////////////////////////////////////////
crJXJCheckGroupCanvasData::crJXJCheckGroupCanvasData():
	m_groupcurpage(1),
	m_grouptotalpage(1)
{
}
crJXJCheckGroupCanvasData::crJXJCheckGroupCanvasData(const crJXJCheckGroupCanvasData& data):
	crUIData(data),
	m_groupmemberinfovec(data.m_groupmemberinfovec),
	m_grouptotalpage(data.m_grouptotalpage),
	m_groupcurpage(data.m_groupcurpage)
{

}
void crJXJCheckGroupCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJCheckGroupCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCheckGroupCurPage:
		m_groupcurpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCheckGroupTotalPage:
		m_grouptotalpage = param?*((int*)param):NULL;
		if (m_grouptotalpage < 1)
		{
			m_grouptotalpage = 1;
		}
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJCheckGroupCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCheckGroupCurPage:
		param = &m_groupcurpage;
		break;
	case WCHDATA_JXJCheckGroupTotalPage:
		param = &m_grouptotalpage;
		break;
	case WCHDATA_JXJCheckGroupMemberVec:
		param = &m_groupmemberinfovec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJCheckGroupCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJMyCountryCanvasData
//
/////////////////////////////////////////
crJXJMyCountryCanvasData::crJXJMyCountryCanvasData():
	m_siJinTai(0),
	m_shenNongMiao(0),
	m_siMaJian(0),
	m_shangLinYuan(0),
	m_taiMiao(0),
	m_curFenFengPage(1),
	m_totalFenFengPage(1),
	m_curPeopleListPage(1),
	m_totalPeopleListPage(1),
	m_officalPos(0),
	m_groupPos(0),
	m_voted(false),
	m_selectedCantonId(0),
	m_countryContribute(0),
	m_statedFete(false),
	m_startedEnrich(false),
	m_feteTimes(0),
	m_enrichType(0),
	m_enrichTimes(0),
	m_lastMinLv(0),
	m_lastMaxLv(0),
	m_shopCurPage(1),
	m_shopTotalPage(1),
	m_benzhouzhanlingcity(0),
	m_isselectedCantonfenfenged(false),
	m_guojiazijin(0),
	m_destroycountryid1(-1),
	m_destroycountryid2(-1)
{
	m_historyVec.resize(JXJCountryHistoryType_max);
}
crJXJMyCountryCanvasData::crJXJMyCountryCanvasData(const crJXJMyCountryCanvasData& data):
	crUIData(data),
	m_notice(data.m_notice),
	m_windeq(data.m_windeq),
	m_lostdeq(data.m_lostdeq),
	m_siJinTai(data.m_siJinTai),
	m_shenNongMiao(data.m_shenNongMiao),
	m_siMaJian(data.m_siMaJian),
	m_shangLinYuan(data.m_shangLinYuan),
	m_taiMiao(data.m_taiMiao),
	m_curFenFengPage(data.m_curFenFengPage),
	m_totalFenFengPage(data.m_totalFenFengPage),
	m_curPeopleListPage(data.m_curPeopleListPage),
	m_totalPeopleListPage(data.m_totalPeopleListPage),
	m_officalPos(data.m_officalPos),
	m_groupPos(data.m_groupPos),
	m_voted(data.m_voted),
	m_selectedCantonId(data.m_selectedCantonId),
	m_countryContribute(data.m_countryContribute),
	m_statedFete(data.m_statedFete),
	m_startedEnrich(data.m_startedEnrich),
	m_feteTimes(data.m_feteTimes),
	m_enrichType(data.m_enrichType),
	m_enrichTimes(data.m_enrichTimes),
	m_lastMinLv(data.m_lastMinLv),
	m_lastMaxLv(data.m_lastMaxLv),
	m_shopCurPage(data.m_shopCurPage),
	m_shopTotalPage(data.m_shopTotalPage),
	m_benzhouzhanlingcity(data.m_benzhouzhanlingcity),
	m_isselectedCantonfenfenged(data.m_isselectedCantonfenfenged),
	m_guojiazijin(data.m_guojiazijin),
	m_destroycountryid1(data.m_destroycountryid1),
	m_destroycountryid2(data.m_destroycountryid2)
{
	m_historyVec.resize(JXJCountryHistoryType_max);
}
void crJXJMyCountryCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJMyCountryCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	////case WCHDATA_JXJCountryKingName:
	////	m_kingname = param?*(std::string*)param:NULL;
	////	break;
	case WCHDATA_JXJMyCountryNotice:
		m_notice = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJSijintai					:
		m_siJinTai = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJShenNongMiao				:
		m_shenNongMiao = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJSiMaJian:
		m_siMaJian = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJShangLinYuan:
		m_shangLinYuan = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJTaiMiao:
		m_taiMiao = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCountryCanvasZhenShouGroupLeader:
		m_zhenshouGroupLeader = *(std::string *)param;
		break;
	case WCHDATA_JXJCountryCanvasZhenShouGroup:
		m_zhenshouGroupName = *(std::string *)param;
		break;
	case WCHDATA_JXJCountryCanvasSelectedCantonId:
		m_selectedCantonId = *(int *) param;
		break;
	case WCHDATA_JXJCountryCanvasFenFengCurPage:
		m_curFenFengPage = *(int *)param;
		break;
	case WCHDATA_JXJCountryCanvasFenFengTotalPage:
		m_totalFenFengPage = *(int *)param;
		break;
	case WCHDATA_JXJCountryCanvasHaveVoted:
		m_voted = *(bool *) param;
		break;
	case WCHDATA_JXJCountryContribute:
		m_countryContribute = *(int *)param;
		break;
	case WCHDATA_JXJCountryCanvasOfficalPos	:
		m_officalPos = *(unsigned char *) param;
		break;
	case WCHDATA_JXJZhanLingCityCountCanvas	:
		m_benzhouzhanlingcity = *(unsigned char *) param;
		break;
	case WCHDATA_JXJCountryCanvasGroupPos:
		m_groupPos = *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryPeopleListCurPage:
		m_curPeopleListPage = *(int *)param;
		break;
	case WCHDATA_JXJCountryPeopleListTotalPage:		
		m_totalPeopleListPage = *(int *)param;
		break;
	case WCHDATA_JXJCountryStartedFete:
		m_statedFete = *(bool *)param;
		break;
	case WCHDATA_JXJCountryStartedEnrichCountry:
		m_startedEnrich = *(bool *)param;
		break;
	case WCHDATA_JXJCountryBenefitEnrichType	:
		m_enrichType = *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryBenefitFeteTimes	:
		m_feteTimes = *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryBenefitEnrichTimes:
		m_enrichTimes = *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryQueryPeopleListMinLv	:
		m_lastMinLv = *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryQueryPeopleListMaxLv	:
		m_lastMaxLv = *(unsigned char *)param;
		break;
	//case WCHDATA_JXJGovPostShopCurPage:
	//	m_shopCurPage = *(int *)param;
	//	break;
	//case WCHDATA_JXJGovPostShopTotalPage:
	//	m_shopTotalPage = *(int *)param;
	//	break;
	case WCHDATA_JXJGovPostShopCurPage:
		m_shopCurPage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGovPostShopTotalPage:
		m_shopTotalPage= param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJPurchaseGovPostShopItemID:
		m_purchaseitemid = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJIsSelectCabtonFenFenged:
		m_isselectedCantonfenfenged = param?*((bool *)param):NULL;
		break;
	case WCHDATA_JXJCanvasCapital:
		m_guojiazijin = *(int *)param;
		break;
	case WCHDATA_JXJDestroyCountryid1:
		m_destroycountryid1 = *(char *)param;
		break;
	case WCHDATA_JXJDestroyCountryid2:
		m_destroycountryid2 = *(char *)param;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJMyCountryCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJMyCountryNotice:
		param = &m_notice;
		break;
	case WCHDATA_JXJShiliWinCityDeq:
		param = &m_windeq;
		break;
	case WCHDATA_JXJShiliLostCityDeq:
		param = &m_lostdeq;
		break;
	case WCHDATA_JXJSijintai:
		param = & m_siJinTai;
		break;
	case WCHDATA_JXJShenNongMiao:
		param = & m_shenNongMiao;
		break;
	case WCHDATA_JXJSiMaJian:
		param = & m_siMaJian;
		break;
	case WCHDATA_JXJShangLinYuan:
		param = & m_shangLinYuan;
		break;
	case WCHDATA_JXJTaiMiao	:
		param = & m_taiMiao; 
		break;
	case WCHDATA_JXJCountryCanvasZhoujunMap:
		param = & m_zhoujunMap;
		break;
	case WCHDATA_JXJCountryCanvasOfficalVec:
		param = & m_officalVec;
		break;
	//case WCHDATA_JXJCountryCanvasVoteVec:
	//	param = & m_voteVec;
	//	break;
	case WCHDATA_JXJCountryCanvasChengchiInfoMap:
		param = & m_chengchiMap;
		break;
	case WCHDATA_JXJCountryCanvasZhenShouGroupLeader:
		param = & m_zhenshouGroupLeader;
		break;
	case WCHDATA_JXJCountryCanvasZhenShouGroup:
		param = & m_zhenshouGroupName;
		break;
	case WCHDATA_JXJCountryCanvasAllocateGroupVec:
		param = &m_groupVec;
		break;
	case WCHDATA_JXJCountryCanvasSelectedCantonId:
		param = & m_selectedCantonId;
		break;
	case WCHDATA_JXJCountryCanvasFenFengCurPage:
		param = & m_curFenFengPage;
		break;
	case WCHDATA_JXJCountryCanvasFenFengTotalPage:
		param = & m_totalFenFengPage;
		break;
	case WCHDATA_JXJCountryCandidateVecT:
		param = & m_candidateVec;
		break;
	case WCHDATA_JXJCountryCanvasHaveVoted:
		param = & m_voted;
		break;
	case WCHDATA_JXJCountryCanvasOfficalPos:
		param = & m_officalPos;
		break;
	case WCHDATA_JXJZhanLingCityCountCanvas:
		param = & m_benzhouzhanlingcity;
		break;
	case WCHDATA_JXJCountryCanvasGroupPos:
		param = & m_groupPos;
		break;
	case WCHDATA_JXJCountryPeopleListCurPage:
		param = & m_curPeopleListPage;
		break;
	case WCHDATA_JXJCountryPeopleListTotalPage:
		param = & m_totalPeopleListPage;
		break;
	case WCHDATA_JXJShiliDataTempPeopleVec:
		param = & m_peopleVec;
		break;
	case WCHDATA_JXJCountryStartedFete:
		param = & m_statedFete;
		break;
	case WCHDATA_JXJCountryStartedEnrichCountry:
		param = & m_startedEnrich;
		break;
	case WCHDATA_JXJCountryBenefitEnrichType	:
		param = & m_enrichType;
		break;
	case WCHDATA_JXJCountryBenefitFeteTimes	:
		param = & m_feteTimes;
		break;
	case WCHDATA_JXJCountryBenefitEnrichTimes:
		param = & m_enrichTimes;
		break;
	case WCHDATA_JXJCountryHistory:
		param = & m_historyVec;
		break;
	case WCHDATA_JXJCountryQueryPeopleListMinLv	:
		param = & m_lastMinLv;
		break;
	case WCHDATA_JXJCountryQueryPeopleListMaxLv	:
		param = & m_lastMaxLv;
		break;
	//case WCHDATA_JXJGovPostShopItemDeq:
	//	param = & m_govShop;
	//	break;
	case WCHDATA_JXJSelectLeaderSet	:
		param = & m_selectLeaderSet;
		break;
	case WCHDATA_JXJGovPostShopCurPage:
		param = &m_shopCurPage;
		break;
	case WCHDATA_JXJGovPostShopTotalPage:
		param = &m_shopTotalPage;
		break;
	//case WCHDATA_JXJGovPostShopItemDeq:
	//	param = &m_govpostitemdeq;
	//	break;
	case WCHDATA_JXJPurchaseGovPostShopItemID:
		param = &m_purchaseitemid;
		break;
	case WCHDATA_JXJCountryCandidateVecL:
		param = &m_candidateVecL;
		break;
	case WCHDATA_JXJIsSelectCabtonFenFenged:
		param = &m_isselectedCantonfenfenged;
		break;
	case WCHDATA_JXJCanvasCapital:
		param = &m_guojiazijin;
		break;
	case WCHDATA_JXJDestroyCountryid1:
		param = &m_destroycountryid1;
		break;
	case WCHDATA_JXJDestroyCountryid2:
		param = &m_destroycountryid2;
		break;
	case WCHDATA_JXJGovShopIDVec:
		param = &m_govShopIDVec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJMyCountryCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crCenterNoticeCanvasData
//
/////////////////////////////////////////
crCenterNoticeCanvasData::crCenterNoticeCanvasData():
	cfgstream(NULL)
{
}
crCenterNoticeCanvasData::crCenterNoticeCanvasData(const crCenterNoticeCanvasData& data):
	crUIData(data),
	cfgstream(data.cfgstream)
{
}
void crCenterNoticeCanvasData::addParam(int i, const std::string& str)
{
}
void crCenterNoticeCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	//rcfg::ConfigScript cfg_script;
	switch(i) 
	{
	case WCHDATA_JXJCenterNotice:
		cfgstream = param?(crStreamBuf*)param:NULL;
		break;
	}
}
void crCenterNoticeCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCenterNotice:
		param = cfgstream.get();
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crCenterNoticeCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJRankingCanvasData
//
/////////////////////////////////////////
crJXJRankingCanvasData::crJXJRankingCanvasData():
	m_curpagenum(1),
	m_totalpagenum(1),
	m_myranking(0)
{
}
crJXJRankingCanvasData::crJXJRankingCanvasData(const crJXJRankingCanvasData& data):
	crUIData(data),
	m_curpagenum(data.m_curpagenum),
	m_totalpagenum(data.m_totalpagenum),
	m_lvrankingmap(data.m_lvrankingmap),
	m_govpostRankingMap(data.m_govpostRankingMap),
	m_expRankingMap(data.m_expRankingMap),
	m_troopsRankingMap(data.m_troopsRankingMap),
	m_accomplishRankingMap(data.m_accomplishRankingMap),
	m_countrycontributeRankingMap(data.m_countrycontributeRankingMap),
	m_manorlvrankingMap(data.m_manorlvrankingMap),
	m_countryCityNumRankingMap(data.m_countryCityNumRankingMap),
	m_countryMoneyRankingMap(data.m_countryMoneyRankingMap),
	m_myranking(data.m_myranking)
{
}
void crJXJRankingCanvasData::addParam(int i, const std::string& str)
{
}
void crJXJRankingCanvasData::inputParam(int i, void *param)
{
	//GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRankingPageNum:
		m_curpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJRankingTotal:
		m_totalpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJMyRanking:
		m_myranking = param?*((unsigned short*)param):NULL;
		break;
	}
}
void crJXJRankingCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRankingPageNum:
		param = &m_curpagenum;
		break;
	case WCHDATA_JXJRankingTotal:
		param = &m_totalpagenum;
		break;
	case WCHDATA_JXJLevelRankingMap:
		param = &m_lvrankingmap;
		break;
	case WCHDATA_JXJGovpostRankingMap:
		param = &m_govpostRankingMap;
		break;
	case WCHDATA_JXJExpRankingMap:
		param = &m_expRankingMap;
		break;
	case WCHDATA_JXJTroopsRankingMap:
		param = &m_troopsRankingMap;
		break;
	case WCHDATA_JXJAccomplishRankingMap:
		param = &m_accomplishRankingMap;
		break;
	case WCHDATA_JXJCountrycontributeRankingMap:
		param = &m_countrycontributeRankingMap;
		break;
	case WCHDATA_JXJMyRanking:
		param = &m_myranking;
		break;
	case WCHDATA_JXJManorlvRankingMap:
		param = &m_manorlvrankingMap;
		break;
	case WCHDATA_JXJCountryCityNumRankingMap:
		param = &m_countryCityNumRankingMap;
		break;
	case WCHDATA_JXJCountryMoneyRankingMap:
		param = &m_countryMoneyRankingMap;
		break;
	case WCHDATA_JXJConsortialvRankingMap:
		param = &m_GrouplvRankingMap;
		break;
	case WCHDATA_JXJConsortiaMemberRankingMap:
		param = &m_GroupMemberRankingMap;
		break;
	case WCHDATA_JXJConsortiaExploitRankingMap:
		param = &m_GroupExploitRankingMap;
		break;
	case WCHDATA_JXJConsortiaTroopsRankingMap:
		param = &m_GroupTroopsRankingMap;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJRankingCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJCardShopCanvasData
//
/////////////////////////////////////////
crJXJCardShopCanvasData::crJXJCardShopCanvasData():
	m_curpage(1),
	m_totalpage(1),
	m_puechasecardid(0)
{
}
crJXJCardShopCanvasData::crJXJCardShopCanvasData(const crJXJCardShopCanvasData& data):
	crUIData(data),
	m_curpage(data.m_curpage),
	m_totalpage(data.m_totalpage),
	m_cardiddeq(data.m_cardiddeq),
	m_puechasecardid(data.m_puechasecardid)
{

}
void crJXJCardShopCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJCardShopCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCardShopCurPage:
		m_curpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCardShopTotalPage:
		m_totalpage = param?*((int*)param):NULL;
		if (m_totalpage < 1)
		{
			m_totalpage = 1;
		}
		break;
	case WCHDATA_JXJPurchaseCardID:
		m_puechasecardid = param?*((int*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJCardShopCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCardShopCurPage:
		param = &m_curpage;
		break;
	case WCHDATA_JXJCardShopTotalPage:
		param = &m_totalpage;
		break;
	case WCHDATA_JXJCardShopItemDeq:
		param = &m_cardiddeq;
		break;
	case WCHDATA_JXJPurchaseCardID:
		param = &m_puechasecardid;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJCardShopCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJProgressCanvasData
//
/////////////////////////////////////////
crJXJProgressCanvasData::crJXJProgressCanvasData():
	m_totalcount(1),
	m_progressAboutUseItemId(0)
{
}
crJXJProgressCanvasData::crJXJProgressCanvasData(const crJXJProgressCanvasData& data):
	crUIData(data),
	m_totalcount(data.m_totalcount),
	m_progressAboutUseItemId(data.m_progressAboutUseItemId)
{

}
void crJXJProgressCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJProgressCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	//case WCHDATA_JXJNextProgress:
	//	m_curmaxprogress = param?*((float*)param):NULL;
	//	break;
	case WCHDATA_JXJTotalCount:
		m_totalcount = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJProgressCanvasAboutUseItemId:
		m_progressAboutUseItemId = param?*((int*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJProgressCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_JXJNextProgress:
	//	param = &m_curmaxprogress;
	//	break;
	case WCHDATA_JXJTotalCount:
		param = &m_totalcount;
		break;
	case WCHDATA_JXJProgressCanvasAboutUseItemId:
		param = &m_progressAboutUseItemId;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJProgressCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crJXJTouchItemCanvasData
//
/////////////////////////////////////////
crJXJTouchItemCanvasData::crJXJTouchItemCanvasData():
	m_interval(2.0f)
{
}
crJXJTouchItemCanvasData::crJXJTouchItemCanvasData(const crJXJTouchItemCanvasData& data):
	crUIData(data),
	m_interval(data.m_interval)
{
}
void crJXJTouchItemCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJTouchItemCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJTouchItem:
		m_item = param ? (crInstanceItem *)param:NULL;
		break;
	case WCHDATA_JXJTouchInterval:
		m_interval = param?*((float*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJTouchItemCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJTouchItem:
		param = m_item.get();
		break;
	case WCHDATA_JXJTouchInterval:
		param = &m_interval;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJTouchItemCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crJXJGovPostShopCanvasData
//
/////////////////////////////////////////
crJXJGovPostShopCanvasData::crJXJGovPostShopCanvasData():
	m_curpage(1),
	m_totalpage(1),
	m_purchaseitemid(0)
{
}
crJXJGovPostShopCanvasData::crJXJGovPostShopCanvasData(const crJXJGovPostShopCanvasData& data):
	crUIData(data),
	m_curpage(data.m_curpage),
	m_totalpage(data.m_totalpage),
	//m_govpostitemdeq(data.m_govpostitemdeq),
	m_purchaseitemid(data.m_purchaseitemid)
{

}
void crJXJGovPostShopCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJGovPostShopCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJGovPostShopCurPage:
		m_curpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGovPostShopTotalPage:
		m_totalpage = param?*((int*)param):NULL;
		if (m_totalpage < 1)
		{
			m_totalpage = 1;
		}
		break;
	case WCHDATA_JXJPurchaseGovPostShopItemID:
		m_purchaseitemid = param?*((unsigned char*)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJGovPostShopCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGovPostShopCurPage:
		param = &m_curpage;
		break;
	case WCHDATA_JXJGovPostShopTotalPage:
		param = &m_totalpage;
		break;
	//case WCHDATA_JXJGovPostShopItemDeq:
	//	param = &m_govpostitemdeq;
	//	break;
	case WCHDATA_JXJPurchaseGovPostShopItemID:
		param = &m_purchaseitemid;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJGovPostShopCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJGongFangCanvasData
//
/////////////////////////////////////////
crJXJGongFangCanvasData::crJXJGongFangCanvasData():
	m_curpage(1),
	m_totalpage(1),
	m_maxmaterialcount(0),
	m_costtype(0),
	m_itemindex(0),
	m_equipCurpage(1),
	m_equipTotalpage(1),
	m_RefineEquipSelectIndex(-1),
	m_AttachEquipSelectIndex(-1),
	m_AttachMateSelectIndex(-1),
	m_refinetype(0),
	m_newequipmagic(0),
	m_curpage2(1),
	m_totalpage2(1),
	m_gongfangmaterialindex(0),
	m_CurSelForgeTroopsEquipID(0),
	m_CurSelForgeLordEquipID(0),
	m_RefineNeedGold(0),
	m_openway(0)
{
}
crJXJGongFangCanvasData::crJXJGongFangCanvasData(const crJXJGongFangCanvasData& data):
	crUIData(data),
	m_curpage(data.m_curpage),
	m_totalpage(data.m_totalpage),
	m_bagitemdata(data.m_bagitemdata),
	//m_tempbagvec(data.m_tempbagvec),
	m_costtype(data.m_costtype),
	m_itemindex(data.m_itemindex),
	m_equipCurpage(data.m_equipCurpage),
	m_equipTotalpage(data.m_equipTotalpage),
	m_RefineEquipSelectIndex(data.m_RefineEquipSelectIndex),
	m_AttachEquipSelectIndex(data.m_AttachEquipSelectIndex),
	m_AttachMateSelectIndex(data.m_AttachMateSelectIndex),
	m_RefineEquipBagitemdata(data.m_RefineEquipBagitemdata),
	m_AttachEquipBagitemdata(data.m_AttachEquipBagitemdata),
	m_AttachMateBagitemdata(data.m_AttachMateBagitemdata),
	m_refinetype(data.m_refinetype),
	m_newequipmagic(data.m_newequipmagic),
	m_curpage2(data.m_curpage2),
	m_totalpage2(data.m_totalpage2),
	m_gongfangmaterialvec(data.m_gongfangmaterialvec),
	m_gongfangmaterialindex(data.m_gongfangmaterialindex),
	m_CurSelForgeTroopsEquipID(data.m_CurSelForgeTroopsEquipID),
	m_CurSelForgeLordEquipID(data.m_CurSelForgeLordEquipID),
	m_RefineNeedGold(data.m_RefineNeedGold),
	m_openway(data.m_openway)
{

}
void crJXJGongFangCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJGongFangCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		m_buildingid = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCanvasBuildingItem:
		m_building_item = param ? (crInstanceItem *)param : NULL ;
		break;
	case WCHDATA_JXJSmelterCurPage:
		m_curpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJSmelterTotalPage:
		m_totalpage = param?*((int*)param):NULL;
		if (m_totalpage < 1)
		{
			m_totalpage = 1;
		}
		break;
	//case WCHDATA_JXJSmelterItemData:
	//	m_bagitemdata = param?*((crBagItemData*)param):NULL;
	//	break;
	case WCHDATA_JXJSmelterMateMaxCount:
		m_maxmaterialcount = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJSmelterCostType:
		m_costtype = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJSmeltItemSelectIndex:
		m_itemindex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJGongFangEquipCurPage:
		m_equipCurpage = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJGongFangEquipTotalPage:
		m_equipTotalpage = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJRefineEquipSelectIndex:
		m_RefineEquipSelectIndex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJRefineEquipSuitIndex:
		m_RefineEquipSuitIndex = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJAttachEquipSelectIndex:
		m_AttachEquipSelectIndex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJAttachMateSelectIndex:
		m_AttachMateSelectIndex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJRefineEquipType:
		m_refinetype = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJAfterRefineEquipMagic:
		m_newequipmagic = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJAttachMateCurPage:
		m_curpage2 = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJAttachMateTotalPage:
		m_totalpage2 = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJGongFangMaterialIndex:
		m_gongfangmaterialindex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCurSelForgeTroopsEquipID:
		m_CurSelForgeTroopsEquipID = param?*((unsigned short*)param):NULL;
		break;	
	case WCHDATA_JXJCurSelForgeLordEquipID:
		m_CurSelForgeLordEquipID = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJRefineNeedGold:
		m_RefineNeedGold = param?*((int*)param):NULL;
		break;	
	//case WCHDATA_JXJRefineEquipBagitemdata:
	//	m_RefineEquipBagitemdata = param?*(crBagItemData*)param:NULL;
	//	break;
	//case WCHDATA_JXJAttachEquipBagitemdata:
	//	m_AttachEquipBagitemdata = param?*(crBagItemData*)param:NULL;
	//	break;
	//case WCHDATA_JXJAttachMateBagitemdata:
	//	m_AttachMateBagitemdata = param?*(crBagItemData*)param:NULL;
	//	break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJGongFangCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCheckBuildingID:
		param = &m_buildingid;
		break;
	case WCHDATA_JXJCanvasBuildingItem:
		param = m_building_item;
		break;
	case WCHDATA_JXJGongFangTuZhiVec:
		param = &m_tuZhiVec;
		break;
	case WCHDATA_JXJSmelterCurPage:
		param = &m_curpage;
		break;
	case WCHDATA_JXJSmelterTotalPage:
		param = &m_totalpage;
		break;
	case WCHDATA_JXJSmelterItemData:
		param = &m_bagitemdata;
		break;
	case WCHDATA_JXJSmelterMateMaxCount:
		param = &m_maxmaterialcount;
		break;
	//case WCHDATA_JXJTempItemBagVec:
	//	param = &m_tempbagvec;
	//	break;
	case WCHDATA_JXJSmelterCostType:
		param = &m_costtype;
		break;
	case WCHDATA_JXJSmeltItemSelectIndex:
		param = &m_itemindex;
		break;
	case WCHDATA_JXJGongFangEquipCurPage:
		param = &m_equipCurpage;
		break;
	case WCHDATA_JXJGongFangEquipTotalPage:
		param = &m_equipTotalpage;
		break;
	case WCHDATA_JXJRefineEquipSelectIndex:
		param = &m_RefineEquipSelectIndex;
		break;
	case WCHDATA_JXJRefineEquipSuitIndex:
		param = &m_RefineEquipSuitIndex;
		break;
	case WCHDATA_JXJAttachEquipSelectIndex:
		param = &m_AttachEquipSelectIndex;
		break;
	case WCHDATA_JXJAttachMateSelectIndex:
		param = &m_AttachMateSelectIndex;
		break;
	case WCHDATA_JXJRefineEquipBagitemdata:
		param = &m_RefineEquipBagitemdata;
		break;
	case WCHDATA_JXJAttachEquipBagitemdata:
		param = &m_AttachEquipBagitemdata;
		break;
	case WCHDATA_JXJAttachMateBagitemdata:
		param = &m_AttachMateBagitemdata;
		break;
	case WCHDATA_JXJRefineEquipType:
		param = &m_refinetype;
		break;
	case WCHDATA_JXJAfterRefineEquipMagic:
		param = &m_newequipmagic;
		break;
	case WCHDATA_JXJAttachMateCurPage:
		param = &m_curpage2;
		break;
	case WCHDATA_JXJAttachMateTotalPage:
		param = &m_totalpage2;
		break;
	case WCHDATA_JXJGongFangMaterialVec:
		param = &m_gongfangmaterialvec;
		break;
	case WCHDATA_JXJGongFangMaterialIndex:
		param = &m_gongfangmaterialindex;
		break;
	case WCHDATA_JXJCurSelForgeTroopsEquipID:
		param = &m_CurSelForgeTroopsEquipID;
		break;
	case WCHDATA_JXJCurSelForgeLordEquipID:
		param = &m_CurSelForgeLordEquipID;
		break;
	case WCHDATA_JXJRefineNeedGold:
		param = &m_RefineNeedGold;
		break;
	case WCHDATA_JXJTheWayToEnterWorkshorp:
		param = &m_openway;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJGongFangCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJAppointCanvasData
//
/////////////////////////////////////////
crJXJAppointCanvasData::crJXJAppointCanvasData():
	m_curAppointJiangkaBagPageNum(1),
	m_totalAppointJiangkaBagPageNum(1),
	m_curselectappointcardid(0),
	m_curselectundoappointcardid(0)
{
}
crJXJAppointCanvasData::crJXJAppointCanvasData(const crJXJAppointCanvasData& data):
	crUIData(data),
	m_curAppointJiangkaBagPageNum(data.m_curAppointJiangkaBagPageNum),
	m_totalAppointJiangkaBagPageNum(data.m_totalAppointJiangkaBagPageNum),
	m_curselectappointcardid(data.m_curselectappointcardid),
	m_curselectundoappointcardid(data.m_curselectundoappointcardid)
{

}
void crJXJAppointCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJAppointCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCurSelAppointCardid:
		m_curselectappointcardid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJCurSelUndoAppointCardid:
		m_curselectundoappointcardid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJCurAppointJiangkaBagPageNum:
		m_curAppointJiangkaBagPageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTotalAppointJiangkaBagPageNum:
		m_totalAppointJiangkaBagPageNum = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJAppointCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCurSelAppointCardid:
		param = &m_curselectappointcardid;
		break;
	case WCHDATA_JXJCurSelUndoAppointCardid:
		param = &m_curselectundoappointcardid;
		break;
	case WCHDATA_JXJCurAppointJiangkaBagPageNum:
		param = &m_curAppointJiangkaBagPageNum;
		break;
	case WCHDATA_JXJTotalAppointJiangkaBagPageNum:
		param = &m_totalAppointJiangkaBagPageNum;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJAppointCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJAppointCanvasData
//
/////////////////////////////////////////
crJXJArmyAdviserCanvasData::crJXJArmyAdviserCanvasData()
{
}
crJXJArmyAdviserCanvasData::crJXJArmyAdviserCanvasData(const crJXJArmyAdviserCanvasData& data):
	crUIData(data),
	m_vipaffairsvec(data.m_vipaffairsvec),
	m_curpagenum(data.m_curpagenum),
	m_totalpagenum(data.m_totalpagenum)
{

}
void crJXJArmyAdviserCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJArmyAdviserCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJVipCanvasCurPageNum:
		m_curpagenum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJVipCanvasTotalPageNum:
		m_totalpagenum = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJArmyAdviserCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJVipCanvasCurPageNum:
		param = &m_curpagenum;
		break;
	case WCHDATA_JXJVipCanvasTotalPageNum:
		param = &m_totalpagenum;
		break;
	case WCHDATA_JXJVipAffairsIDVec:
		param = &m_vipaffairsvec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJArmyAdviserCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	
/////////////////////////////////////////
//
//crJXJOperationCanvasData
//
/////////////////////////////////////////
crJXJOperationCanvasData::crJXJOperationCanvasData():
	m_OnlineCount(0),
	m_interval(0),
	m_init(false),
	m_netType(0)
{
}
crJXJOperationCanvasData::crJXJOperationCanvasData(const crJXJOperationCanvasData& data):
	crUIData(data),
	m_OnlineCount(data.m_OnlineCount),
	m_SceneSeverOnlineMap(data.m_SceneSeverOnlineMap),
	m_remainplayervec(data.m_remainplayervec),
	m_remainplayermap(data.m_remainplayermap),
	m_totalplayervec(data.m_totalplayervec),
	m_totalplayermap(data.m_totalplayermap),
	m_createplayervec(data.m_createplayervec),
	m_createplayermap(data.m_createplayermap),
	m_playerlvmapWei(data.m_playerlvmapWei),
	m_playerlvmapShu(data.m_playerlvmapShu),
	m_playerlvmapWu(data.m_playerlvmapWu),
	m_interval(data.m_interval),
	m_init(false),
	m_netType(0)
{
	
}
void crJXJOperationCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJOperationCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJOnlineCount:
		m_OnlineCount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJGameServerThrowTimeInterval:
		m_interval = param?*(time_t *)param:NULL;
		break;
	//case WCHDATA_JXJGMToolsInit				:
	//	m_init = param ? *(bool *)param : false;
	//	break;
	case WCHDATA_JXJGMMonitorTimer			:
		m_monitorTimer = param?*(time_t *)param:NULL;
		break;
	case WCHDATA_JXJGMRestartServerType			:
		m_netType = param ? *(int *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJOperationCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJOnlineCount:
		param = &m_OnlineCount;
		break;
	case WCHDATA_JXJSceneSeverOnlineMap:
		param = &m_SceneSeverOnlineMap;
		break;
	case WCHDATA_JXJRemainPlayerVec:
		param = &m_remainplayervec;
		break;
	case WCHDATA_JXJRemainPlayerMap:
		param = &m_remainplayermap;
		break;
	case WCHDATA_JXJTotalPlayerVec:
		param = &m_totalplayervec;
		break;
	case WCHDATA_JXJTotalPlayerMap:
		param = &m_totalplayermap;
		break;
	case WCHDATA_JXJCreatePlayerVec:
		param = &m_createplayervec;
		break;
	case WCHDATA_JXJCreatePlayerMap:
		param = &m_createplayermap;
		break;
	case WCHDATA_JXJPlayerLvMapWei:
		param = &m_playerlvmapWei;
		break;
	case WCHDATA_JXJPlayerLvMapShu:
		param = &m_playerlvmapShu;
		break;
	case WCHDATA_JXJPlayerLvMapWu:
		param = &m_playerlvmapWu;
		break;
	case WCHDATA_JXJGameServerThrowTimeInterval	:
		param = &m_interval;
		break;
	//case WCHDATA_JXJGMToolsInit			:
	//	param = &m_init;
	//	break;
	case WCHDATA_JXJServerHeartMap			:
		param = & m_serverHeartMap;
		break;
	case WCHDATA_JXJGMMonitorTimer				:
		param = & m_monitorTimer;
		break;
	case WCHDATA_JXJGMRestartServerType			:
		param = & m_netType;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJOperationCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	

/************************************************************************/
/*    crJXJCheckAddGroupMemberRequestData                                                                  */
/************************************************************************/

JXJ::crJXJCheckAddGroupMemberRequestData::crJXJCheckAddGroupMemberRequestData()
{

}

JXJ::crJXJCheckAddGroupMemberRequestData::crJXJCheckAddGroupMemberRequestData( const crJXJCheckAddGroupMemberRequestData & data ):
	crUIData(data),
	m_addGroupMemberRequestDeq(data.m_addGroupMemberRequestDeq)
{

}

void JXJ::crJXJCheckAddGroupMemberRequestData::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCheckAddGroupMemberRequestData::inputParam( int i, void *param )
{

}

void JXJ::crJXJCheckAddGroupMemberRequestData::getParam( int i, void*& param )
{
	switch(i) 
	{
	case WCHDATA_JXJAddGroupMemberRequestDeq:
		param = &m_addGroupMemberRequestDeq;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

void JXJ::crJXJCheckAddGroupMemberRequestData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}


}

std::string JXJ::crJXJGroupInfo::getLeaderName( int idx )
{
	if (idx < m_groupLeaderVec.size())
	{
		return m_groupLeaderVec[idx].first;
	}
	return "";
}

int JXJ::crJXJGroupInfo::getLeaderLevel( int idx )
{
	if (idx < m_groupLeaderVec.size())
	{
		return m_groupLeaderVec[idx].second;
	}
	return 0;
}

/////////////////////////////////////////
//
//crJXJTechCanvasData
//
/////////////////////////////////////////
crJXJTechCanvasData::crJXJTechCanvasData():
	m_TechCanvasCurPage(1),
	m_TechCanvasTotalPage(1),
	m_ClearTechCDNeedGold(0),
	m_ClearTechCDID(0)
{
}
crJXJTechCanvasData::crJXJTechCanvasData(const crJXJTechCanvasData& data):
	crUIData(data),
	m_TechCanvasCurPage(data.m_TechCanvasCurPage),
	m_TechCanvasTotalPage(data.m_TechCanvasTotalPage),
	m_showtechvec(data.m_showtechvec),
	m_ClearTechCDNeedGold(data.m_ClearTechCDNeedGold),
	m_ClearTechCDID(data.m_ClearTechCDID)
{

}
void crJXJTechCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJTechCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJTechCanvasCurPage:
		m_TechCanvasCurPage = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTechCanvasTotalPage:
		m_TechCanvasTotalPage = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJClearTechDevelopTimeNeedGold:
		m_ClearTechCDNeedGold = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJClearTechDevelopID:
		m_ClearTechCDID = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJTechCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJTechCanvasCurPage:
		param = &m_TechCanvasCurPage;
		break;
	case WCHDATA_JXJTechCanvasTotalPage:
		param = &m_TechCanvasTotalPage;
		break;
	case WCHDATA_JXJShowTechVec:
		param = &m_showtechvec;
		break;
	case WCHDATA_JXJClearTechDevelopTimeNeedGold:
		param = &m_ClearTechCDNeedGold;
		break;
	case WCHDATA_JXJClearTechDevelopID:
		param = &m_ClearTechCDID;
		break;
		
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJTechCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}	

/************************************************************************/
/*    crJXJJunTuanFuBenSettleCanvasData                                                                  */
/************************************************************************/

JXJ::crJXJJunTuanFuBenSettleCanvasData::crJXJJunTuanFuBenSettleCanvasData():
	m_fubenId(0),
	m_times(0),
	m_loseTime (0),
	m_totalTimes(0),
	m_juntuanfubencircle(0.0f)
{

}

JXJ::crJXJJunTuanFuBenSettleCanvasData::crJXJJunTuanFuBenSettleCanvasData( const crJXJJunTuanFuBenSettleCanvasData & data ):
	crUIData(data),
	m_fubenId(data.m_fubenId),
	m_times(data.m_times),
	m_loseTime(data.m_times),
	m_totalTimes(data.m_totalTimes),
	m_juntuanfubencircle(data.m_juntuanfubencircle)
{
}

void JXJ::crJXJJunTuanFuBenSettleCanvasData::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJJunTuanFuBenSettleCanvasData::inputParam( int i, void *param )
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{
	case WCHDATA_JXJJunTuanFuBenId	:
		m_fubenId = param ? *(int *) param: 0;
		break;
	case WCHDATA_JXJJunTuanFuBenTimes:
		m_times = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenTotalTimes:
		m_totalTimes = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenActiveVal:
		m_activeVal = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenExp	:
		m_exp = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenAchivement	:
		m_achivement = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenExploit		:
		m_exploit = param ? *(int *)param : 0;
		break;

	case WCHDATA_JXJJunTuanFuBenGroupFunds	:
		m_groupExp = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenCountryContribute:
		m_countryContribute = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJGroupFuBenLoseTime:
		m_loseTime = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenGroupContribute:
		m_groupContribute = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJJunTuanFuBenCircle:
		m_juntuanfubencircle = param ? *(float *)param : 0;
		break;
	};
}

void JXJ::crJXJJunTuanFuBenSettleCanvasData::getParam( int i, void*& param )
{
	switch (i)
	{
	case WCHDATA_JXJJunTuanFuBenId	:
		param = & m_fubenId;
		break;
	case WCHDATA_JXJJunTuanFuBenTimes:
		param = & m_times;
		break;
	case WCHDATA_JXJJunTuanFuBenTotalTimes:
		param = & m_totalTimes;
		break;
	case WCHDATA_JXJJunTuanFuBenActiveVal:
		param = & m_activeVal;
		break;
	case WCHDATA_JXJJunTuanFuBenExp	:
		param = & m_exp;
		break;
	case WCHDATA_JXJJunTuanFuBenAchivement	:
		param = & m_achivement ;
		break;
	case WCHDATA_JXJJunTuanFuBenExploit		:
		param = & m_exploit;
		break;
	case WCHDATA_JXJJunTuanFuBenGroupFunds	:
		param = & m_groupExp ;
		break;
	case WCHDATA_JXJJunTuanFuBenCountryContribute:
		param = & m_countryContribute ;
		break;
	case WCHDATA_JXJGroupFuBenLoseTime:
		param = & m_loseTime;
		break;
	case WCHDATA_JXJJunTuanFuBenGroupContribute:
		param = & m_groupContribute;
		break;
	case WCHDATA_JXJJunTuanFuBenCircle:
		param = & m_juntuanfubencircle;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

void JXJ::crJXJJunTuanFuBenSettleCanvasData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crJXJOrangeCanvasData
//
/////////////////////////////////////////
crJXJOrangeCanvasData::crJXJOrangeCanvasData():
	m_recvupgradeflag(0)
{
}
crJXJOrangeCanvasData::crJXJOrangeCanvasData(const crJXJOrangeCanvasData& data):
	crUIData(data),
	m_recvupgradeflag(data.m_recvupgradeflag)
{

}
void crJXJOrangeCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJOrangeCanvasData::inputParam(int i, void *param)
{
	//GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRecvChengKaUpgradeFlag:
		m_recvupgradeflag = param?*((int*)param):NULL;
		break;	
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJOrangeCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRecvChengKaUpgradeFlag:
		param = &m_recvupgradeflag;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

JXJ::crJXJFPSAndPingData::crJXJFPSAndPingData():
	m_ping(0)
{
}

JXJ::crJXJFPSAndPingData::crJXJFPSAndPingData( const crJXJFPSAndPingData& data ):
	crUIData(data),
	m_ping(data.m_ping)
{

}

void JXJ::crJXJFPSAndPingData::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJFPSAndPingData::inputParam( int i, void *param )
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{
	case WCHDATA_JXJPingVal:							
		m_ping = *(float *)param;
		break;
	default:
		break;
	}
}

void JXJ::crJXJFPSAndPingData::getParam( int i, void*& param )
{
	switch (i)
	{
	case WCHDATA_JXJPingVal:							
		param = & m_ping;
		break;
	default:
		param = NULL;
		break;
	}
}

void JXJ::crJXJFPSAndPingData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/**************************************************************************

METHOD:crHuodongCanvasData  

**************************************************************************/
JXJ::crHuodongCanvasData::crHuodongCanvasData():
	m_selectId(0),
	m_version(0),
	m_newVersion(0),
	m_idx(0),
	m_selectleijihuodongid(0),
	m_isFestivalHuodongExist(0)
{

}

JXJ::crHuodongCanvasData::crHuodongCanvasData( const crHuodongCanvasData & data ):
	crUIData(data),
	m_selectId(data.m_selectId),
	m_version(data.m_version),
	m_newVersion(data.m_newVersion),
	m_idx(data.m_idx),
	m_selectleijihuodongid(data.m_selectleijihuodongid),
	m_isFestivalHuodongExist(data.m_isFestivalHuodongExist)
{

}

void JXJ::crHuodongCanvasData::addParam( int i, const std::string& str )
{

}

void JXJ::crHuodongCanvasData::inputParam( int i, void *param )
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{
	case WCHDATA_JXJHuodongSelectId		:
		m_selectId = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJHuodongVersion:
		m_version = param ? *(unsigned int *)param : 0;
		break;
	case WCHDATA_JXJHuodongNewVersion:
		m_newVersion = param ? *(unsigned int *)param : 0;
		break;
	case WCHDATA_JXJHuodongSelectIdx				:
		m_idx = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJSelectLeiJiHuoDongID:
		m_selectleijihuodongid =  param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJisFestivalHuodongExist:
		m_isFestivalHuodongExist = param ? *(unsigned char *)param : 0;
		break;
	default:
		break;
	}
}

void JXJ::crHuodongCanvasData::getParam( int i, void*& param )
{
	switch (i)
	{
	case WCHDATA_JXJHuodongVec:
		param = & m_huodongVec;
		break;
	case WCHDATA_JXJHuodongSelectId		:
		param = & m_selectId;
		break;
	case WCHDATA_JXJHuodongVersion:
		param = &m_version;
		break;
	case WCHDATA_JXJHuodongNewVersion:
		param = & m_newVersion;
		break;
	case WCHDATA_JXJHuodongSelectIdx				:
		param = & m_idx;
		break;
	case WCHDATA_JXJSelectLeiJiHuoDongID:
		param = & m_selectleijihuodongid;
		break;
	case WCHDATA_JXJisFestivalHuodongExist:
		param = & m_isFestivalHuodongExist;
		break;
	default:
		break;
	}
}

void JXJ::crHuodongCanvasData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crScenarioTalkCanvasData
//场景剧情
/////////////////////////////////////////
crScenarioTalkCanvasData::crScenarioTalkCanvasData():
	m_ScenarioTalkStep(0),
	m_ScenarioTalkEndmsg(0),
	m_ScenarioTalkCloseTime(0.0f),
	m_ScenarioTalkAutoClose(0.0f)
{
}
crScenarioTalkCanvasData::crScenarioTalkCanvasData(const crScenarioTalkCanvasData& data):
	crUIData(data),
	m_ScenarioTalkIDVec(data.m_ScenarioTalkIDVec),
	m_ScenarioTalkStep(data.m_ScenarioTalkStep),
	m_ScenarioTalkHyperText(data.m_ScenarioTalkHyperText),
	m_ScenarioTalkImage(data.m_ScenarioTalkImage),
	m_ScenarioTalkEndmsg(data.m_ScenarioTalkEndmsg),
	m_ScenarioTalkNametext(data.m_ScenarioTalkNametext),
	m_ScenarioTalkAutoClose(data.m_ScenarioTalkAutoClose),
	m_ScenarioTalkCloseTime(data.m_ScenarioTalkCloseTime)
{

}
void crScenarioTalkCanvasData::addParam(int i, const std::string& str)
{

}
void crScenarioTalkCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJScenarioTalkStep:
		m_ScenarioTalkStep = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJScenarioTalkHyperText:
		m_ScenarioTalkHyperText = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJScenarioTalkImage:
		m_ScenarioTalkImage = param ? *(std::string*)param : NULL;
		break;
	case WCHDATA_JXJScenarioTalkEndmsg:
		m_ScenarioTalkEndmsg = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJScenarioTalkNameText:
		m_ScenarioTalkNametext = param ? *(std::string*)param : NULL;
		break;
	case WCHDATA_JXJVisiableTime:
		m_ScenarioTalkCloseTime = param ? *(float*)param : NULL;
		break;
	case WCHDATA_JXJScenarioTalkAutoCloseTime:
		m_ScenarioTalkAutoClose = param ? *(float*)param : NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crScenarioTalkCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJScenarioTalkIDVec:
		param = & m_ScenarioTalkIDVec;
		break;
	case WCHDATA_JXJScenarioTalkStep:
		param = & m_ScenarioTalkStep;
		break;
	case WCHDATA_JXJScenarioTalkHyperText:
		param = & m_ScenarioTalkHyperText;
		break;
	case WCHDATA_JXJScenarioTalkImage:
		param = &m_ScenarioTalkImage;
		break;
	case WCHDATA_JXJScenarioTalkEndmsg:
		param = & m_ScenarioTalkEndmsg;
		break;
	case WCHDATA_JXJScenarioTalkNameText:
		param = &m_ScenarioTalkNametext;
		break;
	case WCHDATA_JXJVisiableTime:
		param = &m_ScenarioTalkCloseTime;
		break;
	case WCHDATA_JXJScenarioTalkAutoCloseTime:
		param = &m_ScenarioTalkAutoClose;
		break;

	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crScenarioTalkCanvasData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}

/////////////////////////////////////////
//
//crNewFunctionOpenCanvasData
//功能开启提示
/////////////////////////////////////////
crNewFunctionOpenCanvasData::crNewFunctionOpenCanvasData() :
m_NewFunctionID(0),
m_visiabletime(0.0f),
m_troopsid(0)
{
}
crNewFunctionOpenCanvasData::crNewFunctionOpenCanvasData(const crNewFunctionOpenCanvasData& data) :
crUIData(data),
m_NewFunctionID(data.m_NewFunctionID),
m_visiabletime(data.m_visiabletime),
m_troopsid(data.m_troopsid)
{

}
void crNewFunctionOpenCanvasData::addParam(int i, const std::string& str)
{

}
void crNewFunctionOpenCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJNewFunctionOpenID:
		m_NewFunctionID = param ? *(int*)param : NULL;
		break;
	case WCHDATA_JXJVisiableTime:
		m_visiabletime = param ? *(float*)param : NULL;
		break;
	case WCHDATA_JXJNewOpenTroopsID:
		m_troopsid = param ? *(float*)param : NULL;
		break;
	default:
		crUIData::inputParam(i, param);
		break;
	}
}
void crNewFunctionOpenCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJNewFunctionOpenID:
		param = &m_NewFunctionID;
		break;
	case WCHDATA_JXJVisiableTime:
		param = &m_visiabletime;
		break;
	case WCHDATA_JXJNewOpenTroopsID:
		param = &m_troopsid;
		break;

	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crNewFunctionOpenCanvasData::excHandle(_crInt64 msg)
{
	if (LOINT64(msg) == WCH_LockData)
	{
		if (HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg);
	}
}
/////////////////////////////////////////
//
//crXiaoguanjiaCanvasData
//小管家
/////////////////////////////////////////
crXiaoguanjiaCanvasData::crXiaoguanjiaCanvasData() :
m_HuoyueduRewardID(0)
{
}
crXiaoguanjiaCanvasData::crXiaoguanjiaCanvasData(const crXiaoguanjiaCanvasData& data) :
crUIData(data),
m_HuoyueduRewardID(data.m_HuoyueduRewardID),
m_huoyueduvec(data.m_huoyueduvec),
m_huoyueduitemvec(data.m_huoyueduitemvec),
m_huoyueduitemcountvec(data.m_huoyueduitemcountvec)
{

}
void crXiaoguanjiaCanvasData::addParam(int i, const std::string& str)
{

}
void crXiaoguanjiaCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJHuoyueduRewardID:
		m_HuoyueduRewardID = param ? *(unsigned short*)param : NULL;
		break;
	default:
		crUIData::inputParam(i, param);
		break;
	}
}
void crXiaoguanjiaCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJHuoyueduRewardID:
		param = &m_HuoyueduRewardID;
		break;
	case WCHDATA_JXJHuoyueduRewardVec:
		param = &m_huoyueduvec;
		break;
	case WCHDATA_JXJHuoyueduRewardItemVec:
		param = &m_huoyueduitemvec;
		break;
	case WCHDATA_JXJHuoyueduRewardItemCountVec:
		param = &m_huoyueduitemcountvec;
		break;

	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crXiaoguanjiaCanvasData::excHandle(_crInt64 msg)
{
	if (LOINT64(msg) == WCH_LockData)
	{
		if (HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg);
	}
}

/**************************************************************************

METHOD:crChongZhiHuiKuiCanvasData  

**************************************************************************/
JXJ::crChongZhiHuiKuiCanvasData::crChongZhiHuiKuiCanvasData():
	m_serveropenday(0),
	m_idx(0)
{

}

JXJ::crChongZhiHuiKuiCanvasData::crChongZhiHuiKuiCanvasData( const crChongZhiHuiKuiCanvasData & data ):
	crUIData(data),
	m_serveropenday(data.m_serveropenday),
	m_idx(data.m_idx)
{

}

void JXJ::crChongZhiHuiKuiCanvasData::addParam( int i, const std::string& str )
{

}

void JXJ::crChongZhiHuiKuiCanvasData::inputParam( int i, void *param )
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{
	case WCHDATA_JXJHuodongSelectId		:
		m_serveropenday = param ? *(unsigned char *)param : 0;
		break;
	case WCHDATA_JXJHuodongSelectIdx				:
		m_idx = param ? *(unsigned char *)param : 0;
		break;
	default:
		break;
	}
}

void JXJ::crChongZhiHuiKuiCanvasData::getParam( int i, void*& param )
{
	switch (i)
	{
	case WCHDATA_JXJHuiKuiHuoDongMap:
		param = & m_huikuihuodongmap;
		break;
	case WCHDATA_JXJServerOpenDay:
		param = & m_serveropenday;
		break;
	case WCHDATA_JXJHuodongSelectIdx				:
		param = & m_idx;
		break;
	default:
		break;
	}
}

void JXJ::crChongZhiHuiKuiCanvasData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}

/////////////////////////////////////////
//
//crJXJJuntuanHuoDongUIData
//
/////////////////////////////////////////
crJXJJuntuanHuoDongUIData::crJXJJuntuanHuoDongUIData():
	m_timer(1)
{
}
crJXJJuntuanHuoDongUIData::crJXJJuntuanHuoDongUIData(const crJXJJuntuanHuoDongUIData& data):
	crUIData(data),
	m_timer(data.m_timer)
{
}
void crJXJJuntuanHuoDongUIData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJConsortiaPromotionTimer:
		m_timer = atoi(relStr.c_str());
		break;
	}
}
void crJXJJuntuanHuoDongUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJConsortiaPromotionTimer:
		m_timer = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJJuntuanHuoDongUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJConsortiaPromotionTimer:
		param = &m_timer;
		break;
	case WCHDATA_JXJConsortiaPromotionNameVec:
		param = &m_ConsortiaNameVec;
		break;
	case WCHDATA_JXJConsortiaPromotionLeaderVec:
		param = &m_ConsortiaLeaderNameVec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJJuntuanHuoDongUIData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crJXJDelayHideCanvasUIData
//窗口延迟自动隐藏
/////////////////////////////////////////
crJXJDelayHideCanvasUIData::crJXJDelayHideCanvasUIData():
	m_timer(0)
{
}
crJXJDelayHideCanvasUIData::crJXJDelayHideCanvasUIData(const crJXJDelayHideCanvasUIData& data):
	crUIData(data),
	m_timer(data.m_timer)
{
}
void crJXJDelayHideCanvasUIData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJDelayHideCanvasTimer:
		m_timer = atoi(relStr.c_str());
		break;
	}
}
void crJXJDelayHideCanvasUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJDelayHideCanvasTimer:
		m_timer = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJDelayHideCanvasUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJDelayHideCanvasTimer:
		param = &m_timer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJDelayHideCanvasUIData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
/////////////////////////////////////////
//
//crJXJTimerCanvasData
//倒计时
/////////////////////////////////////////
crJXJTimerCanvasData::crJXJTimerCanvasData():
	m_timer(0)
{
}
crJXJTimerCanvasData::crJXJTimerCanvasData(const crJXJTimerCanvasData& data):
	crUIData(data),
	m_timer(data.m_timer)
{
}
void crJXJTimerCanvasData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJCanvasTimer:
		m_timer = atoi(relStr.c_str());
		break;
	}
}
void crJXJTimerCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCanvasTimer:
		m_timer = param?*(short*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJTimerCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCanvasTimer:
		param = &m_timer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJTimerCanvasData::excHandle( _crInt64 msg )
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg); 
	}
}
