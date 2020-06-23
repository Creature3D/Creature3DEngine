/**********************************************************************
*
*	文件:	   appData3.cpp
*
*	描述:	   
*
*	作者:	   陆宾礼
*					
*
**********************************************************************/
#include <JXJ/appData3.h>
#include <JXJ/appData.h>
#include <JXJ/appDataParam.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetGameData.h>
#include <rcfg/ConfigScriptOO.h>
#include <windows.h>

using namespace JXJ;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Formation Data Class
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
JXJ::crJXJUIFormationData::crJXJUIFormationData():
	m_selecting(-1),
	firstcardindex(0),
	secondcardindex(0)
	//: m_selecting(-1), m_is_move(false)
{
	//for(int i = 0; i < 8; i++)
	//{
	//	arms[i] = -1;
	//}

	//for(int i = 0; i < 3; i++)
	//{
	//	eqps[i] = -1;
	//}

	//for(int i = 0; i < 9; i++)
	//{
	//	heds[i] = -1;
	//}
}
crJXJUIFormationData::crJXJUIFormationData(const crJXJUIFormationData& data):
	m_formationInfoVec(data.m_formationInfoVec),
	m_selecting(data.m_selecting),
	firstcardindex(data.firstcardindex),
	secondcardindex(data.secondcardindex)
{
	m_formationInfoVec.resize(3);
}
void crJXJUIFormationData::addParam(int i, const std::string& str)
{
}
void crJXJUIFormationData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{	
	case WCHDATA_JXJCurFormation1stCardIndex:
		firstcardindex = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCurFormation2ndCardIndex:
		secondcardindex = param?*((int*)param):NULL;
		break;
	default:
		break;
	}
}
void crJXJUIFormationData::getParam(int i, void*& param)
{
	switch (i)
	{	
	case WCHDATA_JXJTempFormationVec:
		param = &m_formationInfoVec;
		break;
	case WCHDATA_JXJArmsCells:
		param = &m_arms[0];
		break;
	case WCHDATA_JXJCurFormation1stCardIndex:
		param = &firstcardindex;
		break;
	case WCHDATA_JXJCurFormation2ndCardIndex:
		param = &secondcardindex;
		break;
	case WCHDATA_JXJFormationTempRoleMap:
		param = &m_tempRoleMap;
		break;
	default:
		break;
	}
}
void crJXJUIFormationData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJUIFormationJKData
//
/////////////////////////////////////////
crJXJUIFormationJKData::crJXJUIFormationJKData()
	:m_show_type(0),
	m_curTainkaRow(0),
	m_curDikaRow(0),
	m_tiankaCount(0),
	m_dikaCount(0),
	m_curpage(1),
	m_totalpage(1),
	m_puechasecardid(0),
	m_recvupgradeflag(0),
	m_gongfangmaterialindex(0)
{}
crJXJUIFormationJKData::crJXJUIFormationJKData(const crJXJUIFormationJKData& data)
	:m_cards(data.m_cards), m_show_type(data.m_show_type),
	m_curTainkaRow(data.m_curTainkaRow), m_curDikaRow(data.m_curDikaRow),
	m_tiankaCount(data.m_tiankaCount), m_dikaCount(data.m_dikaCount),
	m_cardExtraActiveParam(data.m_cardExtraActiveParam),
	m_curpage(data.m_curpage),
	m_totalpage(data.m_totalpage),
	m_cardiddeq(data.m_cardiddeq),
	m_puechasecardid(data.m_puechasecardid),
	m_recvupgradeflag(data.m_recvupgradeflag),
	m_gongfangmaterialvec(data.m_gongfangmaterialvec),
	m_gongfangmaterialindex(data.m_gongfangmaterialindex)
{}
void crJXJUIFormationJKData::addParam(int i, const std::string& str)
{}
void crJXJUIFormationJKData::inputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_JXJForJKType:
		m_show_type = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJCurTiankaRow:
		m_curTainkaRow = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJCurDikaRow:
		m_curDikaRow = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJTiankaCount:
		m_tiankaCount = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJDikaCount:
		m_dikaCount = param ? *((int*)param) : NULL;
		break;
	case WCHDATA_JXJCardExtraActiveParam:
		m_cardExtraActiveParam = *((CRCore::crVector3i*)param);
		break;
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
	case WCHDATA_JXJRecvChengKaUpgradeFlag:
		m_recvupgradeflag = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGongFangMaterialIndex:
		m_gongfangmaterialindex = param?*((int*)param):NULL;
		break;
	}
}
void crJXJUIFormationJKData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJForJKVec:
		param = &m_cards;
		break;
	case WCHDATA_JXJForJKType:
		param = &m_show_type;
		break;
	case WCHDATA_JXJUpdateNeedCardVec:
		param = &m_updateneedcardvec;
		break;
	case WCHDATA_JXJCurTiankaRow:
		param = &m_curTainkaRow;
		break;
	case WCHDATA_JXJCurDikaRow:
		param = &m_curDikaRow;
		break;
	case WCHDATA_JXJTiankaCount:
		param = &m_tiankaCount;
		break;
	case WCHDATA_JXJDikaCount:
		param = &m_dikaCount;
		break;
	case WCHDATA_JXJCardExtraActiveParam:
		param = &m_cardExtraActiveParam;
		break;
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
	case WCHDATA_JXJRecvChengKaUpgradeFlag:
		param = &m_recvupgradeflag;
		break;
	case WCHDATA_JXJGongFangMaterialVec:
		param = &m_gongfangmaterialvec;
		break;
	case WCHDATA_JXJGongFangMaterialIndex:
		param = &m_gongfangmaterialindex;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUIFormationJKData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
// Battle Data Class
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
crJXJUIBattleData::crJXJUIBattleData():
	//m_selected(0),
	m_cityliststartindexA(0),
	m_cityliststartindexD(0),
	m_IsFirst(0),
	m_battlecity(0)
{}
crJXJUIBattleData::crJXJUIBattleData(const crJXJUIBattleData& data)
	://m_selected(m_selected),
	m_cityliststartindexA(data.m_cityliststartindexA),
	m_cityliststartindexD(data.m_cityliststartindexD),
	m_battlecities(data.m_battlecities),
	m_cityinbattle(data.m_cityinbattle),
	m_mycitiesset(data.m_mycitiesset),
	m_IsFirst(data.m_IsFirst),
	m_battlecity(data.m_battlecity),
	m_cityindeque(data.m_cityindeque)
{}
void crJXJUIBattleData::addParam(int i, const std::string& str)
{}
void crJXJUIBattleData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch (i)
	{
	//case WCHDATA_JXJBattleUIType:
	//	m_selected = *((int *)param);
	//	break;
	case WCHDATA_JXJCityListstartindexA:
		m_cityliststartindexA = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJCityListstartindexD:
		m_cityliststartindexD = param?*((int*)param):NULL;
		break;	
	case WCHDATA_JXJIsFirstIniUIBattleData:
		m_IsFirst = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJCityNowInBattle:
		m_battlecity = param?*((unsigned short *)param):NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUIBattleData::getParam(int i, void*& param)
{
	switch (i)
	{
	//case WCHDATA_JXJBattleUIType:
	//	param = &m_selected;
	//	break;
	case WCHDATA_JXJCityListstartindexA:
		param = &m_cityliststartindexA;
		break;
	case WCHDATA_JXJCityListstartindexD:
		param = &m_cityliststartindexD;
		break;
	case WCHDATA_JXJBattleCitiesSet:
		param = &m_battlecities;
		break;
	case WCHDATA_JXJCityInBattleSet:
		param = &m_cityinbattle;
		break;
	case WCHDATA_JXJMyBattleCitiesSet:
		param = &m_mycitiesset;
		break;
	case WCHDATA_JXJChengchiPlayFireMap:
		param = &m_playfiremap;
		break;
	case WCHDATA_JXJIsFirstIniUIBattleData:
		param = &m_IsFirst;
		break;
	case WCHDATA_JXJCityNowInBattle:
		param = &m_battlecity;
		break;
	case WCHDATA_JXJCityHaveDeque:
		param = &m_cityindeque;
		break;
	default:
		break;
	}
}
void crJXJUIBattleData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJUICityMapData
//
/////////////////////////////////////////
crJXJUICityMapData::crJXJUICityMapData():
	m_mousePos(0.0f,0.0f),
	m_centerOffset(0.0f,0.0f),
	m_scale(1.0f,1.0f,1.0f),
	m_scale2(1.0f,1.0f,1.0f),
	m_offset(0.0f,0.0f),
	m_isCanDrag(false),
	m_chooseRebelCity(0),
	m_turnTimer(0.0f),
	m_rebelCount(0),
	m_widgetNode(NULL)
{
}
crJXJUICityMapData::crJXJUICityMapData(const crJXJUICityMapData& data):
	crUIData(data),
	m_mousePos(data.m_mousePos),
	m_centerOffset(data.m_centerOffset),
	m_scale(data.m_scale),
	m_scale2(data.m_scale2),
	m_offset(data.m_offset),
	m_isCanDrag(data.m_isCanDrag),
	m_chooseRebelCity(data.m_chooseRebelCity),
	m_turnTimer(data.m_turnTimer),
	m_rebelCount(data.m_rebelCount),
	m_widgetNode(data.m_widgetNode)
{

}
void crJXJUICityMapData::addParam(int i, const std::string& str)
{

}
void crJXJUICityMapData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCityMapMouseCenterOffsetPos:
		if(param)
			m_mousePos = *(CRCore::crVector2*)param;
		break;
	case WCHDATA_JXJCityCenterOffset:
		if(param)
			m_centerOffset = *(CRCore::crVector2*)param;
		break;
	case WCHDATA_JXJCityMapFrameScale:
		if(param)
			m_scale = *(CRCore::crVector3*)param;
		break;
	case WCHDATA_JXJCityMapFrameScale2:
		if(param)
			m_scale2 = *(CRCore::crVector3*)param;
		break;
	case WCHDATA_JXJCityMapOffsetMax:
		if(param)
			m_offset = *(CRCore::crVector2*)param;
		break;
	case WCHDATA_JXJCityMapCanDrag:
		m_isCanDrag = param?*(bool *)param:false;
		break;
	case WCHDATA_JXJChooseRebelCityID:
		m_chooseRebelCity = param?*(unsigned short *)param:0;
		break;
	case WCHDATA_JXJPerCounterinsurgencyTurnTimer:
		m_turnTimer = param?*(float *)param:0;
		break;
	case WCHDATA_JXJBatchRebelCount:
		m_rebelCount = param?*(unsigned char *)param:0;
		break;
	case WCHDATA_JXJMiniMapFrameWidget:
		m_widgetNode = param?(CRCore::crWidgetNode *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUICityMapData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCityMapMouseCenterOffsetPos:
		param = &m_mousePos;
		break;
	case WCHDATA_JXJCityCenterOffset:
		param = &m_centerOffset;
		break;
	case WCHDATA_JXJCityMapFrameScale:
		param = &m_scale;
		break;
	case WCHDATA_JXJCityMapFrameScale2:
		param = &m_scale2;
		break;
	case WCHDATA_JXJCityMapOffsetMax:
		param = &m_offset;
		break;
	case WCHDATA_JXJCityMapCanDrag:
		param = &m_isCanDrag;
		break;
	case WCHDATA_JXJRandomRebelCityNodeVec:
		param = &m_rebelCanvasVec;
		break;
	case WCHDATA_JXJChooseRebelCityID:
		param = &m_chooseRebelCity;
		break;
	case WCHDATA_JXJPerCounterinsurgencyTurnTimer:
		param = &m_turnTimer;
		break;
	case WCHDATA_JXJBatchRebelCount:
		param = &m_rebelCount;
		break;
	case WCHDATA_JXJMiniMapFrameWidget:
		param = m_widgetNode.get();
		break;
	case WCHDATA_JXJChengChiInfoMap:
		param = &m_chengchiInfoMap;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJUIDamageData
//
/////////////////////////////////////////
crJXJUIDamageData::crJXJUIDamageData():
	m_role(NULL),
	m_pos(0.0f,0.0f,0.0f)/*,
	m_isUsed(false)*/
{
}
crJXJUIDamageData::crJXJUIDamageData(const crJXJUIDamageData& data):
	crUIData(data),
	m_role(data.m_role),
	m_pos(data.m_pos)/*,
					 m_isUsed(data.m_isUsed)*/
{

}
void crJXJUIDamageData::addParam(int i, const std::string& str)
{

}
void crJXJUIDamageData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRecvDamageItem:
		m_role = param?(CRNetApp::crInstanceItem *)param:NULL;
		break;
	case WCHDATA_JXJRecvTrackFireMissObject:
		m_object = param?(CRPhysics::crViewMatterObject *)param:NULL;
		break;
	case WCHDATA_JXJTrackFireMissPos:
		if(param)
			m_pos = *(CRCore::crVector3 *)param;
		else
		{
			m_pos = CRCore::crVector3(0.0f,0.0f,0.0f);
		}
		break;
	//case WCHDATA_JXJDemageNodeIsUsing:
	//	m_isUsed = param?*(bool *)param:false;
	//	break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUIDamageData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRecvDamageItem:
		param = m_role.get();
		break;
	case WCHDATA_JXJRecvTrackFireMissObject:
		param = m_object.get();
		break;
	case WCHDATA_JXJTrackFireMissPos:
		param = &m_pos;
		break;
	//case WCHDATA_JXJDemageNodeIsUsing:
	//	param = &m_isUsed;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUIDamageData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJUITongGaoData
//
/////////////////////////////////////////
crJXJUITongGaoData::crJXJUITongGaoData()
{
}
crJXJUITongGaoData::crJXJUITongGaoData(const crJXJUITongGaoData& data):
	crUIData(data),
	m_tonggaodeq(data.m_tonggaodeq)
{

}
void crJXJUITongGaoData::addParam(int i, const std::string& str)
{

}
void crJXJUITongGaoData::inputParam(int i, void *param)
{

}
void crJXJUITongGaoData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBattleBrocastqeque:
		param = &m_tonggaodeq;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUITongGaoData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJBattleBehaveData
//
/////////////////////////////////////////
crJXJBattleBehaveData::crJXJBattleBehaveData():
	m_time(0)
{
}
crJXJBattleBehaveData::crJXJBattleBehaveData(const crJXJBattleBehaveData& data):
	crUIData(data),
	m_time(data.m_time)
{

}
void crJXJBattleBehaveData::addParam(int i, const std::string& str)
{

}
void crJXJBattleBehaveData::inputParam(int i, void *param)
{

}
void crJXJBattleBehaveData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBattleBehaveTime:
		param = &m_time;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJBattleBehaveData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJWorldFuBenData
//
/////////////////////////////////////////
crJXJWorldFuBenData::crJXJWorldFuBenData():
	m_iscanshow(true),
	m_huodongid(0),
	m_temactiom(0),
	m_openday(0),
	m_tempcountrypoints(0)
{}
crJXJWorldFuBenData::crJXJWorldFuBenData(const crJXJWorldFuBenData& data)
	://m_selected(m_selected),
	m_countrypoints(data.m_countrypoints),
	m_pointsrank(data.m_pointsrank),
	m_deadrank(data.m_deadrank),
	m_killrank(data.m_killrank),
	m_huodongid(data.m_huodongid),
	m_praisedid(data.m_praisedid),
	m_iscanshow(data.m_iscanshow),
	m_temactiom(data.m_temactiom),
	m_openday(data.m_openday),
	m_tempcountrypoints(data.m_tempcountrypoints)
{
	for (int i=0; i<6; i++)
	{
		m_myinfomation[i] = data.m_myinfomation[i];
	}
}
void crJXJWorldFuBenData::addParam(int i, const std::string& str)
{}
void crJXJWorldFuBenData::inputParam(int i, void *param)
{
}
void crJXJWorldFuBenData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJCountryPoints:
		param = &m_countrypoints;
		break;
	case WCHDATA_JXJPointsRank:
		param = &m_pointsrank;
		break;
	case WCHDATA_JXJDeadRank:
		param = &m_deadrank;
		break;
	case WCHDATA_JXJKillRank:
		param = &m_killrank;
		break;
	case WCHDATA_JXJMyWorldInfo:
		param = &m_myinfomation;
		break;
	case WCHDATA_JXJTempHuoDongId:
		param = &m_huodongid;
		break;
	case WCHDATA_JXJWorldPraisedI:
		param = &m_praisedid;
		break;
	case WCHDATA_JXJWorldIsCanShow:
		param = &m_iscanshow;
		break;
	case WCHDATA_JXJWorldTemHuoDong:
		param = &m_temactiom;
		break;
	case WCHDATA_JXJOpenDiJiTian:
		param = &m_openday;
		break;
	case WCHDATA_JXJTempCountryPoints:
		param = &m_tempcountrypoints;
		break;
	default:
		break;
	}
}
void crJXJWorldFuBenData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJWorldStatusData
//
/////////////////////////////////////////
crJXJWorldStatusData::crJXJWorldStatusData():
	m_remaintime(0),
	m_time(0)
{}
crJXJWorldStatusData::crJXJWorldStatusData(const crJXJWorldStatusData& data):
   m_event(data.m_event),
   m_remaintime(data.m_remaintime),
    m_time(data.m_time)
{
}
void crJXJWorldStatusData::addParam(int i, const std::string& str)
{}
void crJXJWorldStatusData::inputParam(int i, void *param)
{
}
void crJXJWorldStatusData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJWorldEventRemainTime:
		param = &m_remaintime;
		break;
	case WCHDATA_JXJWorldEvent:
		param = &m_event;
		break;
	case WCHDATA_JXJWorldEventTime:
		param = &m_time;
		break;
	}
}
void crJXJWorldStatusData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJUIAddArmyCanvasData
//
/////////////////////////////////////////
crJXJUIAddArmyCanvasData::crJXJUIAddArmyCanvasData():m_case(-1),m_gold(0),m_copper(0),
	m_food(0),m_wood(0),m_iron(0),m_horse(0),m_touchCamp(NULL)
{
}
crJXJUIAddArmyCanvasData::crJXJUIAddArmyCanvasData(const crJXJUIAddArmyCanvasData& data):
	crUIData(data),
	m_case(data.m_case),
	m_gold(data.m_gold),
	m_armyMap(data.m_armyMap),
	m_copper(data.m_copper),
	m_food(data.m_food),
	m_wood(data.m_wood),
	m_iron(data.m_iron),
	m_horse(data.m_horse),
	m_touchCamp(data.m_touchCamp)
{

}
void crJXJUIAddArmyCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJUIAddArmyCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBattleAddArmyCase:
		m_case = param?*(char *)param:-1;
		break;
	case WCHDATA_JXJBattleAddArmyNeedGold:
		m_gold = param?*(int *)param:0;
		break;
	case WCHDATA_JXJBattleAddArmyInfoMap:
		if(param)
			m_armyMap = *(BattleAddArmyInfoVec *)param;
		break;
	case WCHDATA_JXJBattleAddArmyNeedCopper:
		m_copper = param?*(int *)param:0;
		break;
	case WCHDATA_JXJBattleAddArmyNeedWood:
		m_wood = param?*(int *)param:0;
		break;
	case WCHDATA_JXJBattleAddArmyNeedFood:	
		m_food = param?*(int *)param:0;
		break;
	case WCHDATA_JXJBattleAddArmyNeedIron:	
		m_iron = param?*(int *)param:0;
		break;
	case WCHDATA_JXJBattleAddArmyNeedHorse:	
		m_horse = param?*(int *)param:0;
		break;
	case WCHDATA_JXJAddArmyCampItem:
		m_touchCamp = param?(CRNetApp::crInstanceItem *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJUIAddArmyCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBattleAddArmyCase:
		param = &m_case;
		break;
	case WCHDATA_JXJBattleAddArmyNeedGold:
		param = &m_gold;
		break;
	case WCHDATA_JXJBattleAddArmyInfoMap:
		param = &m_armyMap;
		break;
	case WCHDATA_JXJBattleAddArmyNeedCopper:
		param = &m_copper;
		break;
	case WCHDATA_JXJBattleAddArmyNeedWood:
		param = &m_wood;
		break;
	case WCHDATA_JXJBattleAddArmyNeedFood:	
		param = &m_food;
		break;
	case WCHDATA_JXJBattleAddArmyNeedIron:
		param = &m_iron;
		break;
	case WCHDATA_JXJBattleAddArmyNeedHorse:	
		param = &m_horse;
		break;
	case WCHDATA_JXJAddArmyCampItem:
		param = m_touchCamp;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJUIAddArmyCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJFubenRewardCanvasData
//
/////////////////////////////////////////
crJXJFubenRewardCanvasData::crJXJFubenRewardCanvasData():m_exp(0),m_gold(0),m_copper(0),m_exploit(0),
	m_isWin(false),m_index(0),m_overTimes(1),m_timer(0),m_canReliveCount(0),
	m_fubenDropID(0),m_starCount(0)
{
}
crJXJFubenRewardCanvasData::crJXJFubenRewardCanvasData(const crJXJFubenRewardCanvasData& data):
	crUIData(data),
	m_exp(data.m_exp),
	m_gold(data.m_gold),
	m_copper(data.m_copper),
	m_exploit(data.m_exploit),
	m_isWin(data.m_isWin),
	m_fubenDropVec(data.m_fubenDropVec),
	m_index(data.m_index),
	m_overTimes(data.m_overTimes),
	m_timer(data.m_timer),
	m_canReliveCount(data.m_canReliveCount),
	m_fubenDropID(data.m_fubenDropID),
	m_starCount(data.m_starCount)
{

}
void crJXJFubenRewardCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJFubenRewardCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJFubenRewardExp:
		m_exp = (param)?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenRewardGold:
		m_gold = (param)?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenRewardCopper:
		m_copper = param?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenRewardExploit:
		m_exploit = param?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenIsWin:
		m_isWin = param?*(bool *)param:false;
		break;
	case WCHDATA_JXJFubenRewardDropVec:
		if(param)
			m_fubenDropVec = *(FubenDropRewardVec *)param;
		break;
	case WCHDATA_JXJFubenDropChooseIndex:
		m_index = param?*(unsigned char*)param:0;
		break;
	case WCHDATA_JXJFubenDropOverTimes:
		m_overTimes = param?*(unsigned char *)param:1;
		break;
	case WCHDATA_JXJFubenAutoCloseTimer:
		m_timer = param?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenCanReliveCount:
		m_canReliveCount = param?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenDropItemChooseId:
		m_fubenDropID = param?*(int *)param:0;
		break;
	case WCHDATA_JXJFubenStarCount:
		m_starCount = param?*(unsigned char *)param:0;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJFubenRewardCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJFubenRewardExp:
		param = &m_exp;
		break;
	case WCHDATA_JXJFubenRewardGold:
		param = &m_gold;
		break;
	case WCHDATA_JXJFubenRewardCopper:
		param = &m_copper;
		break;
	case WCHDATA_JXJFubenRewardExploit:
		param = &m_exploit;
		break;
	case WCHDATA_JXJFubenIsWin:
		param = &m_isWin;
		break;
	case WCHDATA_JXJFubenRewardDropVec:
		param = &m_fubenDropVec;
		break;
	case WCHDATA_JXJFubenDropChooseIndex:
		param = &m_index;
		break;
	case WCHDATA_JXJFubenDropOverTimes:
		param = &m_overTimes;
		break;
	case WCHDATA_JXJFubenAutoCloseTimer:
		param = &m_timer;
		break;
	case WCHDATA_JXJFubenCanReliveCount:
		param = &m_canReliveCount;
		break;
	case WCHDATA_JXJFubenDropItemChooseId:
		param = &m_fubenDropID;
		break;
	case WCHDATA_JXJFubenStarCount:
		param = &m_starCount;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJFubenRewardCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJSweepCanvasData
//
/////////////////////////////////////////
crJXJSweepCanvasData::crJXJSweepCanvasData():m_count(1)
{
}
crJXJSweepCanvasData::crJXJSweepCanvasData(const crJXJSweepCanvasData& data):
	crUIData(data),
	m_count(data.m_count)
{

}
void crJXJSweepCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJSweepCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJFubenRushCount:
		m_count = param?*(unsigned int *)param:1;
		break;
	case WCHDATA_JXJFubenRushRewardItemMap:
		if(param)
			m_fubenRushMap = *(FubenRushItemMap *)param;
		else
		{
			m_fubenRushMap.clear();
		}
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJSweepCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJFubenRushCount:
		param = &m_count;
		break;
	case WCHDATA_JXJFubenRushRewardItemMap:
		param = &m_fubenRushMap;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJSweepCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJSweepCanvasData
//
/////////////////////////////////////////
crJXJOnlineRewardCanvasData::crJXJOnlineRewardCanvasData():m_count(0),m_timer(0)
{
}
crJXJOnlineRewardCanvasData::crJXJOnlineRewardCanvasData(const crJXJOnlineRewardCanvasData& data):
	crUIData(data),
	m_count(data.m_count),
	m_timer(data.m_timer)
{

}
void crJXJOnlineRewardCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJOnlineRewardCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJOnlineRewardCanvasCount:
		m_count = param?*(unsigned char*)param:0;
		break;
	case WCHDATA_JXJOnlineRewardCanvasTimer:
		m_timer = param?*(int *)param:0;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJOnlineRewardCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJOnlineRewardCanvasCount:
		param = &m_count;
		break;
	case WCHDATA_JXJOnlineRewardCanvasTimer:
		param = &m_timer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJOnlineRewardCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJMoveCanvasData
//
/////////////////////////////////////////
crJXJMoveCanvasData::crJXJMoveCanvasData():m_beginPos(0.0f,0.0f,0.0f),m_targetPos(0.0f,0.0f,0.0f),m_speed(0.0f,0.0f,0.0f),m_distance(0.0f)
{
}
crJXJMoveCanvasData::crJXJMoveCanvasData(const crJXJMoveCanvasData& data):
	crUIData(data),
	m_beginPos(data.m_beginPos),
	m_targetPos(data.m_targetPos),
	m_speed(data.m_speed),
	m_distance(data.m_distance)
{

}
void crJXJMoveCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJMoveCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJMoveCanvasBeginPos:
		m_beginPos = param?*(CRCore::crVector3 *)param:CRCore::crVector3(0.0f,0.0f,0.0f);
		break;
	case WCHDATA_JXJMoveCanvasTargetPos:
		m_targetPos = param?*(CRCore::crVector3 *)param:CRCore::crVector3(0.0f,0.0f,0.0f);
		break;
	case WCHDATA_JXJMoveCanvasSpeed:
		m_speed = param?*(CRCore::crVector3 *)param:CRCore::crVector3(0.0f,0.0f,0.0f);
		break;
	case WCHDATA_JXJMoveDistance:
		m_distance = param?*(float *)param:0.0f;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJMoveCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJMoveCanvasBeginPos:
		param = &m_beginPos;
		break;
	case WCHDATA_JXJMoveCanvasTargetPos:
		param = &m_targetPos;
		break;
	case WCHDATA_JXJMoveCanvasSpeed:
		param = &m_speed;
		break;
	case WCHDATA_JXJMoveDistance:
		param = &m_distance;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJMoveCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJBattleShowCanvasData
//
/////////////////////////////////////////
crJXJBattleShowCanvasData::crJXJBattleShowCanvasData():m_curItem(NULL)
{
}
crJXJBattleShowCanvasData::crJXJBattleShowCanvasData(const crJXJBattleShowCanvasData& data):
	crUIData(data),
	m_curItem(data.m_curItem)
{

}
void crJXJBattleShowCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJBattleShowCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJRoomMemberInfoMap:
		if(param)
			m_roomInfoMap = *(RoomMemberInfoMap *)param;
		else
		{
			m_roomInfoMap.clear();
		}
		break;
	case WCHDATA_JXJRoomMemberCurSelect:
		m_curItem = (param)?(CRNetApp::crInstanceItem *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJBattleShowCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJRoomMemberInfoMap:
		param = &m_roomInfoMap;
		break;
	case WCHDATA_JXJRoomMemberCurSelect:
		param = m_curItem.get();
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJBattleShowCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJPersuadeCanvasData
//
/////////////////////////////////////////
crJXJPersuadeCanvasData::crJXJPersuadeCanvasData():m_fubenid(0),m_jkid(0),m_canvas(NULL)
{
	m_countrystrength[0] = 1;
	m_countrystrength[1] = 1;
	m_countrystrength[2] = 1;
}
crJXJPersuadeCanvasData::crJXJPersuadeCanvasData(const crJXJPersuadeCanvasData& data):
	crUIData(data),
	m_fubenid(data.m_fubenid),
	m_jkid(data.m_jkid),
	m_canvas(data.m_canvas),
	m_countrystrength(data.m_countrystrength)
{

}
void crJXJPersuadeCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJPersuadeCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJPersuadeFubenID:
		m_fubenid = param?*(unsigned short *)param:0;
		break;
	case WCHDATA_JXJPersuadeJKID:
		m_jkid = param?*(int *)param:0;
		break;
	case WCHDATA_JXJPersuadeNeedCanvas:
		m_canvas = param?(CRCore::crCanvasNode *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJPersuadeCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJPersuadeFubenID:
		param = &m_fubenid;
		break;
	case WCHDATA_JXJPersuadeJKID:
		param = &m_jkid;
		break;
	case WCHDATA_JXJPersuadeNeedCanvas:
		param = m_canvas;
		break;
	case WCHDATA_JXJCountryStrength:
		param = &m_countrystrength;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJPersuadeCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJLandRewardCanvasData
//
/////////////////////////////////////////
crJXJLandRewardCanvasData::crJXJLandRewardCanvasData():m_nMulti(1),m_bHas(false)
{
}
crJXJLandRewardCanvasData::crJXJLandRewardCanvasData(const crJXJLandRewardCanvasData& data):
	crUIData(data),
	m_nMulti(data.m_nMulti),
	m_bHas(data.m_bHas)
{

}
void crJXJLandRewardCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJLandRewardCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJLandRewardMap:
		if(param)
		{
			m_lanRewadMap = *(LandRewardMap *)param;
		}
		else
			m_lanRewadMap.clear();
		break;
	case WCHDATA_JXJLandRewardMulti:
		m_nMulti = param?*(unsigned char *)param:1;
		break;
	case WCHDATA_JXJClientHasLandReward:
		m_bHas = param?*(bool *)param:false;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJLandRewardCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJLandRewardMap:
		param = &m_lanRewadMap;
		break;
	case WCHDATA_JXJLandRewardMulti:
		param = &m_nMulti;
		break;
	case WCHDATA_JXJClientHasLandReward:
		param = &m_bHas;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJLandRewardCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJLunHuiCanvasData
//
/////////////////////////////////////////
crJXJLunHuiCanvasData::crJXJLunHuiCanvasData():m_type(-1),m_replacecount(0)
{
}
crJXJLunHuiCanvasData::crJXJLunHuiCanvasData(const crJXJLunHuiCanvasData& data):
	crUIData(data),m_type(-1),m_replacecount(data.m_replacecount)
{

}
void crJXJLunHuiCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJLunHuiCanvasData::inputParam(int i, void *param)
{
// 	GNE::LockMutex lock( m_dataMutex );
// 	switch(i) 
// 	{
// 	case WCHDATA_JXJLunHuiItemOrCardType:
// 		m_type = param?*(char *)param:-1;
// 		break;
// 	default:
// 		crUIData::inputParam(i,param);
// 		break;
// 	}
}
void crJXJLunHuiCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJLunHuiItemMap:
		param = &m_itemMap;
		break;
	case WCHDATA_JXJLunHuiCardMap:
		param = &m_cardMap;
		break;
	case WCHDATA_JXJTempItemSwitchMap:
		param = &m_itemSwitchMap;
		break;
	case WCHDATA_JXJTempCardSwitchMap:
		param = &m_cardSwitchMap;
		break;
	case WCHDATA_JXJPlayerLunHuiReplacecount:
		param = &m_replacecount;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJLunHuiCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crBattleJKCanvasData
//
/////////////////////////////////////////
crBattleJKCanvasData::crBattleJKCanvasData()
{
}
crBattleJKCanvasData::crBattleJKCanvasData(const crBattleJKCanvasData& data) :
crUIData(data),
m_timer(0)
{

}
void crBattleJKCanvasData::addParam(int i, const std::string& str)
{

}
void crBattleJKCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJPlayerBackCityCoolTimer:
		m_timer = param?*(int*)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crBattleJKCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBufferTimerMap:
		param = m_bufferInfoMap;
		break;
	case WCHDATA_JXJPlayerBackCityCoolTimer:
		param = &m_timer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crBattleJKCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crBufferWidgetData
//
/////////////////////////////////////////
crBufferWidgetData::crBufferWidgetData() :m_isFlash(false)
{
}
crBufferWidgetData::crBufferWidgetData(const crBufferWidgetData& data) :
crUIData(data), m_isFlash(data.m_isFlash)
{

}
void crBufferWidgetData::addParam(int i, const std::string& str)
{

}
void crBufferWidgetData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJBufferWidgetIsFlash:
		m_isFlash = param ? *(bool *)param : false;
		break;
	default:
		crUIData::inputParam(i, param);
		break;
	}
}
void crBufferWidgetData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJBufferWidgetIsFlash:
		param = &m_isFlash;
		break;
	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crBufferWidgetData::excHandle(CREPARAM msg)
{
	if (LOCREPARAM(msg) == WCH_LockData)
	{
		if (HICREPARAM(msg))
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
//crJXJChatChannelCanvasData
//
/////////////////////////////////////////
crJXJChatChannelCanvasData::crJXJChatChannelCanvasData():
	m_chatChannelSet(0),
	m_chatChannelTempSet(0)
{
}
crJXJChatChannelCanvasData::crJXJChatChannelCanvasData(const crJXJChatChannelCanvasData& data):
	crUIData(data),
	m_chatChannelSet(data.m_chatChannelSet),
	m_chatChannelTempSet(0)
{

}
void crJXJChatChannelCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJChatChannelCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJChatChannelDefaultSet:
		m_chatChannelSet = param?*(BitSet64 *)param:NULL;
		break;
	case WCHDATA_JXJChatChannelTempSet:
		m_chatChannelTempSet = param?*(BitSet64 *)param:NULL;
		break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJChatChannelCanvasData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJChatChannelDefaultSet:
		param = &m_chatChannelSet;
		break;
	case WCHDATA_JXJChatChannelTempSet:
		param = &m_chatChannelTempSet;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJChatChannelCanvasData::excHandle(CREPARAM msg)
{
	if(LOCREPARAM(msg) == WCH_LockData)
	{
		if(HICREPARAM(msg))
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
//crJXJJuQingCanvasData
//
/////////////////////////////////////////
crJXJJuQingCanvasData::crJXJJuQingCanvasData() :
m_time(0.0f)
{
}
crJXJJuQingCanvasData::crJXJJuQingCanvasData(const crJXJJuQingCanvasData& data) :
crUIData(data),
m_time(0.0f)
{

}
void crJXJJuQingCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJJuQingCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJJuQingCanvasShowTimer:
		m_time = param ? *(float *)param:0.0f;
		break;
	case WCHDATA_JXJJuQingImg:
		m_strImgName = param ? *(std::string *)param : "";
		break;
	case WCHDATA_JXJJuQingName:
		m_strJuQingName = param ? *(std::string *)param : "";
		break;
	case WCHDATA_JXJJuQingInfo:
		m_strInfo = param ? *(std::string *)param : "";
		break;
	default:
		crUIData::inputParam(i, param);
		break;
	}
}
void crJXJJuQingCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJJuQingCanvasShowTimer:
		param = &m_time;
		break;
	case WCHDATA_JXJJuQingImg:
		param = &m_strImgName;
		break;
	case WCHDATA_JXJJuQingName:
		param = &m_strJuQingName;
		break;
	case WCHDATA_JXJJuQingInfo:
		param = &m_strInfo;
		break;
	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crJXJJuQingCanvasData::excHandle(CREPARAM msg)
{
	if (LOCREPARAM(msg) == WCH_LockData)
	{
		if (HICREPARAM(msg))
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
//crBattleReportCanvasData
//
/////////////////////////////////////////
crBattleReportCanvasData::crBattleReportCanvasData()
{
}
crBattleReportCanvasData::crBattleReportCanvasData(const crBattleReportCanvasData& data) :
crUIData(data)
{

}
void crBattleReportCanvasData::addParam(int i, const std::string& str)
{

}
void crBattleReportCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJNeedQueryPlayerName:
		m_needQueryPlayerName = param ? *(std::string *)param : "";
		break;
	}
}
void crBattleReportCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJNeedQueryPlayerName:
		param = &m_needQueryPlayerName;
		break;
	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crBattleReportCanvasData::excHandle(CREPARAM msg)
{
	if (LOCREPARAM(msg) == WCH_LockData)
	{
		if (HICREPARAM(msg))
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
//crJXJStarCanvasData
//
/////////////////////////////////////////
crJXJStarCanvasData::crJXJStarCanvasData() :m_npcKillCount(0), m_bossKillCount(0)
{
}
crJXJStarCanvasData::crJXJStarCanvasData(const crJXJStarCanvasData& data) :
crUIData(data),
m_npcKillCount(0),
m_bossKillCount(0)
{

}
void crJXJStarCanvasData::addParam(int i, const std::string& str)
{

}
void crJXJStarCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJStarInfoNPCKillCount:
		m_npcKillCount = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJStarInfoBossKillCount:
		m_bossKillCount = param ? *(int *)param : NULL;
		break;
	}
}
void crJXJStarCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJStarInfoNPCKillCount:
		param = &m_npcKillCount;
		break;
	case WCHDATA_JXJStarInfoBossKillCount:
		param = &m_bossKillCount;
		break;
	case WCHDATA_JXJCanvasStarInfoVec:
		param = m_starInfoVec;
		break;
	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crJXJStarCanvasData::excHandle(CREPARAM msg)
{
	if (LOCREPARAM(msg) == WCH_LockData)
	{
		if (HICREPARAM(msg))
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
//crHeadCanvasData
//
/////////////////////////////////////////
crHeadCanvasData::crHeadCanvasData() :m_instance(NULL)
{
}
crHeadCanvasData::crHeadCanvasData(const crHeadCanvasData& data) :
crUIData(data),
m_instance(data.m_instance)
{

}
void crHeadCanvasData::addParam(int i, const std::string& str)
{

}
void crHeadCanvasData::inputParam(int i, void *param)
{
	GNE::LockMutex lock(m_dataMutex);
	switch (i)
	{
	case WCHDATA_JXJHeadCanvasBindInstance:
		m_instance = param ? (CRNetApp::crInstanceItem *)param : NULL;
		break;
	}
}
void crHeadCanvasData::getParam(int i, void*& param)
{
	switch (i)
	{
	case WCHDATA_JXJHeadCanvasBindInstance:
		param = m_instance;
		break;
	default:
		crUIData::getParam(i, param);
		break;
	}
}
void crHeadCanvasData::excHandle(CREPARAM msg)
{
	if (LOCREPARAM(msg) == WCH_LockData)
	{
		if (HICREPARAM(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(msg);
	}
}