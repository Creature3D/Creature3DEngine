/**********************************************************************
*
*	文件:	   appData5.cpp
*
*	描述:	   
*
*	作者:	   李雪洁
*					
*
**********************************************************************/
#include <JXJ/appData5.h>
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
//crJXJAccomplishCanvasIconData
//
/////////////////////////////////////////
crJXJAccomplishCanvasIconData::crJXJAccomplishCanvasIconData():
	m_btnIndex(0)
{
}
crJXJAccomplishCanvasIconData::crJXJAccomplishCanvasIconData(const crJXJAccomplishCanvasIconData& data):
	crUIData(data),
	m_accomplishCanvasIconVec(data.m_accomplishCanvasIconVec),
	m_btnIndex(data.m_btnIndex)
{
}
void crJXJAccomplishCanvasIconData::addParam(int i, const std::string& str)
{

}
void crJXJAccomplishCanvasIconData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJAccomplishBtnIndex:
		m_btnIndex = param?*((int*)param):NULL;
		break;
	//case WCHDATA_JXJAccomplishCanvasIcon:
	//	if(param)
	//	m_accomplishCanvasIconVec = (AccomplishCanvasIconVec *)param;
	//	break;
	default:
		crUIData::inputParam(i,param);
		break;
	}
}
void crJXJAccomplishCanvasIconData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJAccomplishCanvasIcon:
		param = &m_accomplishCanvasIconVec;
		break;
	case WCHDATA_JXJAccomplishBtnIndex:
		param = &m_btnIndex;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJAccomplishCanvasIconData::excHandle(_crInt64 msg)
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