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
#ifndef JXJ_APPDATA5_H
#define JXJ_APPDATA5_H
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crWchMsg.h>
#include <gnelib.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRNetApp/appDataClass.h>
#include <CREncapsulation/crTableIO.h>
#include <JXJ/appData.h>

namespace CRNetApp
{
	class crRole;
}

namespace JXJ{
typedef std::vector<std::pair<std::string, std::string>> CanvasIconVec;//id,partid
class crJXJAccomplishCanvasIconData : public CRCore::crUIData
{//client
public:
	crJXJAccomplishCanvasIconData();
	crJXJAccomplishCanvasIconData(const crJXJAccomplishCanvasIconData& data);
	DataClass(JXJ, JXJAccomplishCanvasIcon)
		virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	GNE::Mutex m_dataMutex;
	CanvasIconVec m_accomplishCanvasIconVec;
	int m_btnIndex;
};
class crJXJAchievementPassData : public CRCore::Referenced
{
public:
	crJXJAchievementPassData(){m_name = ""; m_totalFubenInt = 0; m_passedFubenInt = 0;}
	void setName(std::string m_name){this->m_name = m_name;}
	std::string getName(){return m_name;}
protected:
	std::string m_name;
	int m_totalFubenInt;
	int m_passedFubenInt;
};


}
#endif