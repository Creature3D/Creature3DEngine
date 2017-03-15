/**********************************************************************
*
*	文件:	   appData6.cpp
*
*	描述:	   
*
*	作者:	   于毅欣
*					
*
**********************************************************************/
#include <JXJ/appData6.h>
#include <JXJ/appDataParam.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetGameData.h>
#include <rcfg/ConfigScriptOO.h>
#include <windows.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;

///////////////////////////////////////////
////
////crJXJPlayerIconShopData
////
///////////////////////////////////////////
//crJXJPlayerIconShopData::crJXJPlayerIconShopData():
//	m_nSex(0),
//	m_nCurSelType(0)
//{
//}
//crJXJPlayerIconShopData::crJXJPlayerIconShopData(const crJXJPlayerIconShopData& data):
//	crUIData(data)
//{
//}
//void crJXJPlayerIconShopData::addParam(int i, const std::string& str)
//{
//}
//void crJXJPlayerIconShopData::inputParam(int i, void *param)
//{
//// 	switch(i) 
//// 	{
//// 	case WCHDATA_JXJPlayerBtnIconShopMap:
//// 		m_curpage = param?*((int*)param):NULL;
//// 		break;
//// 	default:
//		crUIData::inputParam(i,param);
//// 		break;
//// 	}
//}
//void crJXJPlayerIconShopData::getParam(int i, void*& param)
//{
//	switch(i) 
//	{
//	case WCHDATA_JXJPlayerBtnIconShopMap:
//		param = &m_btnIconShopMap;
//		break;
//	case WCHDATA_JXJPlayerIconShopMap:
//		param = &m_iconShopMap;
//		break;
//	default:
//		crUIData::getParam(i,param);
//		break;
//	}
//}
//void crJXJPlayerIconShopData::excHandle(_crInt64 msg)
//{
//	if(LOINT64(msg) == WCH_LockData)
//	{
//		if(HIINT64(msg))
//			m_dataMutex.acquire();
//		else
//			m_dataMutex.release();
//	}
//	else
//	{
//		crUIData::excHandle(msg); 
//	}
//}	


