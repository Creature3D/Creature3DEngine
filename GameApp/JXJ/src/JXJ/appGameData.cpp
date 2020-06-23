/**********************************************************************
*
*	文件:	   appGameData.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#include <JXJ/appGameData.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appData.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNet/crNetContainer.h>
#include <CREncapsulation/crLoader.h>
#include <JXJ/appData2.h>
#include <CRIOManager/crLoadManager.h>

using namespace CRCore;
using namespace CREncapsulation;
using namespace CRNetApp;
using namespace CRNet;
using namespace JXJ;
using namespace CRIOManager;
/////////////////////////////////////////
//
//crJXJConsortia
//
/////////////////////////////////////////
crJXJConsortia::crJXJConsortia():
	m_id(0),
	m_stream(NULL)
{
}
crJXJConsortia::~crJXJConsortia()
{

}
void crJXJConsortia::setID(int id)
{
	m_id = id;
}
int crJXJConsortia::getID()
{
	return m_id;
}
void crJXJConsortia::getName(std::string & name) 
{
	name = "";
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaName,param);
		name = *(std::string *)param;
	}
}

void crJXJConsortia::setCreateDate(const std::string &createdate)
{
	m_createdate = createdate; 
}
std::string &crJXJConsortia::getCreateDate()
{
	return m_createdate; 
}
void crJXJConsortia::setStream(CRCore::crStreamBuf *stream)
{
	m_stream = stream;
}
CRCore::crStreamBuf *crJXJConsortia::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}

bool crJXJConsortia::checkPermission(int id,unsigned int permission)
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap = (ConsortiaMemberMap *)param;
		ConsortiaMemberMap::iterator itr = consortiaMemberMap->find(id);
		if(itr != consortiaMemberMap->end())
		{
			PermissionCheck check;
			if (check(itr->second->getGroupPos(),permission))
			{
				return true;
			}
		}
	}
	return false;
}

void crJXJConsortia::updateLeaderVec()
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
		GroupLeaderVec & groupLeaderVec  = *(GroupLeaderVec *)param;
		groupLeaderVec.clear();
		groupLeaderVec.resize(1);
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap = (ConsortiaMemberMap *)param;
		ConsortiaMemberMap::iterator itr = consortiaMemberMap->begin();
		unsigned char i = 0,leaderIdx = 0;
		for (; itr != consortiaMemberMap->end(); ++ itr)
		{
			if (itr->second->getGroupPos() == JXJGroup_FirstLeader )
			{
				groupLeaderVec[0] = itr->first;
			}
			else if ( itr->second->getGroupPos() == JXJGroup_SecondLeader )
			{
				groupLeaderVec.push_back(itr->first);
			}
		}
	}
}

void crJXJConsortia::loadData()
{
	if(!m_dataClass.valid())
	{
		std::string str = crGlobalHandle::getInstance()->getScriptDir()+"consortia.cfg";
		m_dataClass = CREncapsulation::loadData(str);
	}
	if(m_dataClass.valid() && m_stream.valid())
	{
		m_stream->seekBegin();
		m_dataClass->inputParam(WCHDATA_DataStream,m_stream.get());
		m_stream = NULL;
	}
}
/////////////////////////////////////////
//
//crJXJAccomplish
//
/////////////////////////////////////////
crJXJAccomplish::crJXJAccomplish():
	m_complete(false),
	m_id(0),
	m_step(0),
	m_largestep(0),
	m_stepCount(0),
	m_accomplishPoint(0)
{
}
crJXJAccomplish::~crJXJAccomplish()
{

}
void crJXJAccomplish::setID(unsigned short id)
{
	m_id = id;
}
unsigned short crJXJAccomplish::getID()
{
	return m_id;
}
void crJXJAccomplish::setComplete(bool complete)
{
	m_complete = complete;
}
bool crJXJAccomplish::getComplete()
{
	return m_complete;
}
void crJXJAccomplish::setStep(int step)
{
	m_step = step>UCHAR_MAX?UCHAR_MAX:step;
	if(crGlobalHandle::isClient() && !m_complete && isComplete())
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor)
		{
			ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
			streamBuf->createBuf(2);
			streamBuf->_writeUShort(m_id);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAccomplishComplete,streamBuf.get());
			netConductor->getNetManager()->sendPacket("all",packet);
			//m_complete = true;
		}
	}
}
unsigned char crJXJAccomplish::getStep()
{
	return m_step;
}
void crJXJAccomplish::setLardgeStep(unsigned int step)
{
	m_largestep = step;
}
unsigned int crJXJAccomplish::getLardgeStep()
{
	return m_largestep;
}
void crJXJAccomplish::loadData()
{
	ref_ptr<crTableIO>accomplishTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
	if(accomplishTab.valid())
	{
		crTableIO::StrVec record;
		int scriptid = accomplishTab->getTitleIndex("脚本");
		int stepcountid = accomplishTab->getTitleIndex("Steps");
		int pointid = accomplishTab->getTitleIndex("成就");
		if(accomplishTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0)
		{
			std::string script = record[scriptid];
			if(!script.empty())
			{
				crData *data = CREncapsulation::loadData(crGlobalHandle::getInstance()->getScriptDir()+"accomplish/"+script);
				if(data)
				{
					setDataClass(data);
				}
			}
			m_stepCount = atoi(record[stepcountid].c_str());
			m_accomplishPoint = atoi(record[pointid].c_str());
		}
	}
}
unsigned char crJXJAccomplish::getStepCount()
{
	return m_stepCount;
}
unsigned short crJXJAccomplish::getAccomplishPoint()
{
	return m_accomplishPoint;
}
/////////////////////////////////////////
//
//crJXJShili
//
/////////////////////////////////////////
crJXJShili::crJXJShili():
	m_id(0),
	m_stream(NULL)
{
}
crJXJShili::~crJXJShili()
{

}
void crJXJShili::setID(int id)
{
	m_id = id;
}
int crJXJShili::getID()
{
	return m_id;
}
void crJXJShili::setStream(CRCore::crStreamBuf *stream)
{
	m_stream = stream;
}
CRCore::crStreamBuf *crJXJShili::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
void crJXJShili::loadData()
{
	if(!m_dataClass.valid())
	{
		std::string str = crGlobalHandle::getInstance()->getScriptDir()+"shili.cfg";
		m_dataClass = CREncapsulation::loadData(str);
	}
	if(m_dataClass.valid() && m_stream.valid())
	{
		m_stream->seekBegin();
		m_dataClass->inputParam(WCHDATA_DataStream,m_stream.get());
		m_stream = NULL;
	}
}
/////////////////////////////////////////
//
//crJXJPlayerStore
//
/////////////////////////////////////////
crJXJPlayerStore::crJXJPlayerStore():
	m_id(0)
{
}
crJXJPlayerStore::~crJXJPlayerStore()
{

}
void crJXJPlayerStore::setID(int id)
{
	m_id = id;
}
int crJXJPlayerStore::getID()
{
	return m_id;
}
void crJXJPlayerStore::setStream(CRCore::crStreamBuf *stream)
{
	m_stream = stream;
}
CRCore::crStreamBuf *crJXJPlayerStore::getStream()
{
	if(m_stream.valid()) m_stream->seekBegin();
	return m_stream.get();
}
void crJXJPlayerStore::loadData()
{
	if(!m_dataClass.valid())
	{
		std::string str = crGlobalHandle::getInstance()->getScriptDir()+"playerstore.cfg";
		m_dataClass = CREncapsulation::loadData(str);
	}
	if(m_dataClass.valid() && m_stream.valid())
	{
		m_stream->seekBegin();
		m_dataClass->inputParam(WCHDATA_DataStream,m_stream.get());
		m_stream = NULL;
	}
}
/////////////////////////////////////////
//
//crJXJConsortia
//
/////////////////////////////////////////
void JXJ::crJXJConsortia::dayZero()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			data->dayZero();
		}
	}
}

void JXJ::crJXJConsortia::weekZero()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			data->weekZero();
		}
	}
}

void JXJ::crJXJConsortia::twoSecZero()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			data->twoSecZero();
		}
	}
}

bool JXJ::crJXJConsortia::willDismiss()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			void * param;
			data->getParam(WCHDATA_JXJConsortiaDismissTime,param);
			return *(int *) param == 0;
		}
	}
	return false;
}

int JXJ::crJXJConsortia::getDismissTime()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			void * param;
			data->getParam(WCHDATA_JXJConsortiaDismissTime,param);
			return *(int *)param;
		}
	}
	return -1;
}

int JXJ::crJXJConsortia::getFunds()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			void * param;
			data->getParam(WCHDATA_JXJConsortiaFunds,param);
			return *(int *)param;
		}
	}
	return 0;
}

void JXJ::crJXJConsortia::setFunds( int funds )
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			data->inputParam(WCHDATA_JXJConsortiaFunds,&funds);
		}
	}
}

void JXJ::crJXJConsortia::updateToRank(crData * gsBrainData)
{
	if (m_dataClass.valid())
	{
		void * param;
		gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
		ConsortialvRank * consortialvRank = (ConsortialvRank *) param;
		CRCore::ref_ptr<ConsortiaLvRankItem > rankItem = new ConsortiaLvRankItem(m_id);
		getName(rankItem->m_groupName);
		getLeaderName(0,rankItem->m_leaderName);
		rankItem->m_lv = getLevel();
		rankItem->m_memberCount = getMemberCount();
		rankItem->m_exploit = getExploit();
		rankItem->m_troops = getTroops();
		rankItem->m_shili = getShiLi();
		consortialvRank->updateRank(rankItem->getKey(),rankItem);
	}
}


void JXJ::crJXJConsortia::delFromRank(crData * gsBrainData)
{
	if (m_dataClass.valid())
	{
		void * param;
		gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
		ConsortialvRank * consortialvRank = (ConsortialvRank *) param;
		consortialvRank->delRank(m_id);
	}
}

int JXJ::crJXJConsortia::getRankNum( CRCore::crData * gsBrainData )
{
	void * param;
	gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
	ConsortialvRank * consortialvRank = (ConsortialvRank *) param;
	return consortialvRank->getRankNum(m_id);
}

void JXJ::crJXJConsortia::getLeaderName( unsigned char idx ,std::string &name)
{
	name = "";
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
		GroupLeaderVec * groupLeaderVec  = (GroupLeaderVec *)param;
		if (idx < groupLeaderVec->size())
		{
			m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
			ConsortiaMemberMap *consortiaMemberMap = (ConsortiaMemberMap *)param;
			ConsortiaMemberMap::iterator itr = consortiaMemberMap->find((*groupLeaderVec)[idx]);
			if (itr != consortiaMemberMap->end())
			{
				if (itr->second.valid())
					name = itr->second->getName();
			}
		}
	}
}

unsigned char JXJ::crJXJConsortia::getLevel()
{
	if (m_dataClass.valid())
	{
		void * param;
		m_dataClass->getParam(WCHDATA_JXJConsortialv,param);
		return  *(unsigned char *)param;
	}
	return 0;
}

unsigned char JXJ::crJXJConsortia::getMemberCount()
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap = (ConsortiaMemberMap *)param;
		return consortiaMemberMap->size();
	}
	return 0;
}

int JXJ::crJXJConsortia::getExploit()
{
	if (m_dataClass.valid())
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		if (gsBrainData)
		{
			int totalexploit = 0;
			void *param;
			unsigned char shiliid = getShiLi();
			if (shiliid >= 10 && shiliid <=12)
			{
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJShiliWei + shiliid - c_startShiliID,param);
				crJXJShili *shili = (crJXJShili *)param;
				if (shili && shili->getDataClass())
				{
					crData *shilidata = shili->getDataClass();	
					shilidata->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap *peoplemap = (PeopleMap *)param;
					PeopleMap::iterator peoitr;

					m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
					ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->begin();
					for (;cmmIt != consortiaMemberMap->end(); ++ cmmIt)
					{
						peoitr = peoplemap->find(cmmIt->first);
						if (peoitr != peoplemap->end() && peoitr->second.valid())
						{
							totalexploit += peoitr->second->getExploit();
						}
					}
				}
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			return totalexploit;
		}
	}
	return 0;
}

int JXJ::crJXJConsortia::getTroops()
{
	if (m_dataClass.valid())
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		if (gsBrainData)
		{
			int totaltroops = 0;
			void *param;
			unsigned char shiliid = getShiLi();
			if (shiliid >= 10 && shiliid <= 12)
			{
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJShiliWei + shiliid - c_startShiliID,param);
				crJXJShili *shili = (crJXJShili *)param;
				if (shili && shili->getDataClass())
				{
					crData *shilidata = shili->getDataClass();	
					shilidata->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap *peoplemap = (PeopleMap *)param;
					PeopleMap::iterator peoitr;
					m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
					ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->begin();
					for (;cmmIt != consortiaMemberMap->end(); ++ cmmIt)
					{
						peoitr = peoplemap->find(cmmIt->first);
						if (peoitr != peoplemap->end() && peoitr->second.valid())
						{
							totaltroops += peoitr->second->getTroopsnum();
						}
					}
				}
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			return totaltroops;
		}
	}
	return 0;
}

unsigned char JXJ::crJXJConsortia::getShiLi()
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaShiliID,param);
		return *(unsigned char *)param;
	}
	return 0;
}

void JXJ::crJXJConsortia::setName( std::string & name )
{
	if (m_dataClass.valid())
	{
		m_dataClass->inputParam(WCHDATA_JXJConsortiaName,&name);
	}
}

int JXJ::crJXJConsortia::getTotalActiveVal()
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
		ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->begin();

		int totalActiveVal = 0;
		for (;cmmIt != consortiaMemberMap->end(); ++ cmmIt)
		{
			if (cmmIt->second.valid())
			{
				totalActiveVal += cmmIt->second->getActiveVaule();
			}
		}
		return totalActiveVal;
	}
	return 0;
}

void JXJ::crJXJConsortia::settleEarngins()
{
	if (m_dataClass.valid())
	{
		crJXJConsortiaData * data = dynamic_cast<crJXJConsortiaData *>(m_dataClass.get());
		if (data)
		{
			data->settleEarnings();
		}
	}
}

void JXJ::crJXJConsortia::setUpdateDate( const std::string &date )
{
	m_updateDate = date;
}

std::string & JXJ::crJXJConsortia::getUpateDate()
{
	return m_updateDate;
}

unsigned char JXJ::crJXJConsortia::getUserGroupPos( int id )
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
		ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->find(id);
		if (cmmIt != consortiaMemberMap->end())
		{
			return cmmIt->second->getGroupPos();
		}
	}
	return JXJGroup_Normal;
}

bool JXJ::crJXJConsortia::isUserNewer( int id )
{
	if (m_dataClass.valid())
	{
		void *param;
		m_dataClass->getParam(WCHDATA_JXJConsortiaMemberMap,param);
		ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
		ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->find(id);
		if (cmmIt != consortiaMemberMap->end())
		{
			return cmmIt->second->getNewer();
		}
	}
	return false;
}

///////////////////////////////////////////
////
////crJXJHuodong
////
///////////////////////////////////////////
//crJXJHuodong::crJXJHuodong():
//	m_complete(NotStart),
//	m_id(0),
//	m_step(-1),
//	m_stepCount(0),
//	m_goalStep(0),
//	m_usedTimes(0),
//	m_delFlag(0),
//	m_isOpen(false)
//{
//}
//crJXJHuodong::~crJXJHuodong()
//{
//
//}
//void crJXJHuodong::setID(unsigned short id)
//{
//	m_id = id;
//}
//unsigned short crJXJHuodong::getID()
//{
//	return m_id;
//}
//void crJXJHuodong::setComplete(crJXJHuodong::CompleteStatus complete)
//{
//	m_complete = complete;
//}
//crJXJHuodong::CompleteStatus crJXJHuodong::getComplete()
//{
//	return m_complete;
//}
//void crJXJHuodong::setStep(int step,int playerId ,bool syn)
//{
//	int tmpStepCount = step - m_step;
//	if (((unsigned int)tmpStepCount ^ (unsigned int )m_goalStep) & 0x80000000)		//  如果不同符号
//	{
//		tmpStepCount = 0;
//	}
//	m_step = step;
//	m_stepCount += tmpStepCount;
//	if (syn)
//	{
//		synStep(playerId);
//	}
//}
//int crJXJHuodong::getStep()
//{
//	return m_step;
//}
//void crJXJHuodong::loadData()
//{
//	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//	if(huodongTab.valid())
//	{
//		crTableIO::StrVec record;
//		int scriptid = huodongTab->getTitleIndex("脚本");
//		int stepcountid = huodongTab->getTitleIndex("条件");
//		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
//		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
//		int openIDx = huodongTab->getTitleIndex("开启");
//		int delFlagIDx = huodongTab->getTitleIndex("删除标记");
//		if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0)
//		{
//			int delFlag = atoi(record[delFlagIDx].c_str());
//			if(delFlag != m_delFlag)
//				clearData();
//
//			std::string script = record[scriptid];
//			if(!script.empty())
//			{
//				std::string scriptName = crGlobalHandle::getInstance()->getScriptDir()+"huodong/"+script;
//				
//				rcfg::ConfigScript cfg_script;
//				crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptName,false);
//				crData *data = CREncapsulation::loadData(scriptName);
//				if(data)
//				{
//					setDataClass(data);
//				}
//			}
//			m_goalStep = atoi(record[stepcountid].c_str());
//			m_startTime = record[startTimeIdx];
//			m_endTime = record[endTimeIdx];
//			m_isOpen = atoi(record[openIDx].c_str());
//			m_delFlag = delFlag;
//		}
//	}
//}
//
//int crJXJHuodong::getStepCount()
//{
//	return m_stepCount;
//}
//
//void JXJ::crJXJHuodong::changeComplete( CompleteStatus complete ,int playerId)
//{
//	if(crGlobalHandle::isClient() )
//	{
//		if (m_complete < complete)
//		{
//			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//			if(netConductor)
//			{
//				crPlayerDataEventPacket packet;
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				stream->createBuf(2);
//				stream->_writeShort(m_id);
//				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHuodongChangeStatus,stream.get());
//				netConductor->getNetManager()->sendPacket("all",packet);
//			}
//		}
//	}
//	else
//	{
//		if (complete != m_complete)
//		{
//			bool willSyn = false;
//			switch (complete)
//			{
//			case JXJ::crJXJHuodong::Started:
//				{
//					if (isOpen() && !isDoing())
//					{
//						m_complete = crJXJHuodong::Started;
//						willSyn = true;
//					}
//				}
//				break;
//			case JXJ::crJXJHuodong::CannotGet:
//				{
//					if (isOpen())
//					{
//						m_complete = crJXJHuodong::CannotGet;
//						willSyn = true;
//					}
//				}
//				break;
//			case JXJ::crJXJHuodong::CanGet:
//				{
//					if (isOpen() && isDoing() && canGet() && !hadGot())
//					{
//						m_complete = crJXJHuodong::CanGet;
//						willSyn = true;
//					}
//				}
//				break;
//			case JXJ::crJXJHuodong::HadGot:
//				if (isOpen() && m_complete == CanGet)
//				{
//					ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//					if(huodongTab.valid())
//					{
//						crTableIO::StrVec record;
//						int timesLimitIdx = huodongTab->getTitleIndex("次数");
//						int isOverlapIdx = huodongTab->getTitleIndex("是否累计次数");
//						if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0)
//						{
//							unsigned short timesLimit = atoi(record[timesLimitIdx].c_str());
//							//addCanUseTimes(-1);
//							addUsedTimes(1);
//							if (atoi(record[isOverlapIdx].c_str()))
//							{
//								m_stepCount -= m_goalStep;
//							}
//							else 
//							{
//								// 不累计次数，单次领取后目标值清空
//								// 状态设为初始开启状态，重新开始检测是否可领取。
//								m_stepCount = 0;
//								m_complete = crJXJHuodong::Started;
//							}
//							if (getUsedTimes() >= timesLimit)
//							{
//								m_complete = crJXJHuodong::HadGot;
//							}
//						}
//					}
//					willSyn = true;
//				}
//				break;
//			case JXJ::crJXJHuodong::Finished:
//				{
//					if (!isOpen())
//					{
//						switch (m_complete)
//						{
//						case JXJ::crJXJHuodong::NotStart:
//						case JXJ::crJXJHuodong::Started:
//						case JXJ::crJXJHuodong::CannotGet:
//							m_complete = Finished_cannotGot;
//							willSyn = true;
//							break;
//						case JXJ::crJXJHuodong::CanGet:
//							m_complete = Finished_notGot;
//							willSyn = true;
//							break;
//						case JXJ::crJXJHuodong::HadGot:
//							m_complete = Finished_hadGot;
//							willSyn = true;
//							break;
//						default:
//							break;
//						}
//					}
//				}
//				break;
//			default:
//				break;
//			}
//			if (willSyn)
//			{
//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//				ref_ptr<crGameServerPlayerData> playerData =
//					dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerId));
//				if (gameServer && playerData.valid())
//				{
//					ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
//					streamBuf->createBuf(11);
//					streamBuf->_writeUShort(m_id);
//					streamBuf->_writeUChar(m_complete);
//					streamBuf->_writeInt(m_step);
//					streamBuf->_writeInt(m_stepCount);
//					streamBuf->_writeUShort(m_usedTimes);
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerId,WCH_JXJRecvHuodongChangeStatus,streamBuf.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//				}
//			}
//		}
//	}
//}
//
//void JXJ::crJXJHuodong::setStepCount( int stepCount ,int playerId,bool syn)
//{
//	if(m_stepCount != stepCount)
//	{
//		m_stepCount = stepCount;
//		//if (m_goalStep)
//		//{
//		//	addCanUseTimes(abs(m_stepCount / m_goalStep));
//		//}
//		if (syn)
//		{
//			synStep(playerId);
//		}
//	}
//}
//
//void JXJ::crJXJHuodong::resetStep( int step ,int playerId,bool syn)
//{
//	m_step = step;
//	m_stepCount = 0;
//	if (syn)
//	{
//		synStep(playerId);
//	}
//}
//
//bool JXJ::crJXJHuodong::canGet()
//{
//	return abs(m_stepCount) >= abs(m_goalStep);
//}
//
//void JXJ::crJXJHuodong::synStep(int playerId)
//{
//	if(crGlobalHandle::isClient())
//	{
//		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//		if (netConductor)
//		{
//			ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
//			streamBuf->createBuf(2);
//			streamBuf->_writeUShort(m_id);
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHuodongChangeStatus,streamBuf.get());
//			netConductor->getNetManager()->sendPacket("all",packet);
//		}
//	}
//	if (canGet())
//	{
//		changeComplete(CanGet,playerId);
//	}
//}
//
//void JXJ::crJXJHuodong::checkStartOrFinished(int playerId)
//{
//	if ( isOpen() )
//	{
//		if (checkPreCompleted(playerId))
//		{
//			changeComplete(Started,playerId);
//		}
//		if (m_goalStep == -1)
//		{
//		//	changeComplete(CanGet,playerId);
//			setComplete(CanGet);
//		}
//	}
//	else
//	{
//		changeComplete(Finished,playerId);
//	}
//}
//
//bool JXJ::crJXJHuodong::isOpen()
//{
//	do 
//	{
//		if (!m_isOpen)
//		{
//			return false;
//		}
//
//		ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//		if (false == huodongTab.valid()) break;
//		crTableIO::StrVec huoRecord;
//		int isOpenIdx = huodongTab->getTitleIndex("开启");
//		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
//		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
//		int idIdx = huodongTab->getTitleIndex("id");
//
//		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(this->getID()),huoRecord) < 0) break;
//
//		typedef std::vector<int> Vec5i;
//		Vec5i startTimeVec5i;
//		Vec5i endTimeVec5i;
//		startTimeVec5i.reserve(5);
//		endTimeVec5i.reserve(5);
//		crArgumentParser::appAtoVec(huoRecord[startTimeIdx],startTimeVec5i);
//		crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endTimeVec5i);
//
//		startTimeVec5i.resize(5,0);
//		endTimeVec5i.resize(5,0);
//
//		time_t nowTime_t = time(0);
//		struct  tm nowTm = *localtime(&nowTime_t);
//		struct tm  startTm;
//		struct tm  endTm;
//		memset(&startTm,0,sizeof (startTm));
//		memset(&endTm,0,sizeof (endTm));
//
//		startTm.tm_year = startTimeVec5i[0] - 1900;
//		endTm.tm_year = endTimeVec5i[0] - 1900;
//		startTm.tm_mon = startTimeVec5i[1] - 1;
//		endTm.tm_mon = endTimeVec5i[1] - 1;
//		startTm.tm_mday = startTimeVec5i[2];
//		endTm.tm_mday = endTimeVec5i[2];
//		startTm.tm_hour = startTimeVec5i[3];
//		endTm.tm_hour = endTimeVec5i[3];
//		startTm.tm_min = startTimeVec5i[4];
//		endTm.tm_min = endTimeVec5i[4];
//		startTm.tm_isdst = nowTm.tm_isdst;
//		endTm.tm_isdst = nowTm.tm_isdst;
//
//		time_t startTime_t = mktime(&startTm);
//		time_t endTime_t = mktime(&endTm);
//		if (nowTime_t >= startTime_t && nowTime_t <= endTime_t)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	} while (0);
//	return false;
//}
//
//bool JXJ::crJXJHuodong::checkPreCompleted( int playerID )
//{
//	ref_ptr<crPlayerGameData > playerData = NULL;
//
//	bool preCompleted = false;
//
//	// 检查前置活动是否已经完成
//	if(crGlobalHandle::isClient())
//	{
//		playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	}
//	else
//	{
//		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//		ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
//		playerData = serverPlayerData->getPlayerGameData();
//	}
//	if (playerData.valid())
//	{
//		ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//		int idIdx = huodongTab->getTitleIndex("id");
//		int preIDIdx = huodongTab->getTitleIndex("前置id");
//
//		int preID = 0;
//		crTableIO::StrVec record;
//		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_id),record) >= 0)
//		{
//			preID = atoi(record[preIDIdx].c_str());
//		}
//		crRole *mainRole = playerData->getMainRole();
//		if(mainRole && mainRole->getDataClass())
//		{
//			void *param;
//			crData *roleData = mainRole->getDataClass();
//			roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
//			roleData->getParam(WCHDATA_JXJHuodongMap	,param);
//			HuodongMap *huodongMap= (HuodongMap *)param;
//			HuodongMap::iterator hmItr = huodongMap->find(preID);
//			if (huodongMap->end() != hmItr)
//			{
//				if (hmItr->second->getComplete() == Finished_hadGot 
//					|| hmItr->second->getComplete() == HadGot)
//				{
//					preCompleted = true;
//				}
//			}
//			else
//			{
//				preCompleted = true;
//			}
//			roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
//		}
//	}
//	return preCompleted;
//}
//
//void JXJ::crJXJHuodong::clearData()
//{
//	m_complete = NotStart;
//	//m_id = 0;
//	m_step = -1;
//	m_stepCount = 0;
//	m_goalStep = 0;
//	m_usedTimes = 0;
//	m_delFlag = 0;
//	m_isOpen = false;
//}
//
//bool JXJ::crJXJHuodong::needSave()
//{
//	// 数据有没有变化
//	return m_step != -1 || m_stepCount != 0 || m_usedTimes != 0;
//}

/////////////////////////////////////////
//
//crJXJNewGuide
//
/////////////////////////////////////////
crJXJNewGuide::crJXJNewGuide():
	m_complete(false),
	m_id(0),
	m_step(0),
	m_stepCount(0),
	m_accomplishPoint(0)
{
}
crJXJNewGuide::~crJXJNewGuide()
{

}
void crJXJNewGuide::setID(unsigned short id)
{
	m_id = id;
}
unsigned short crJXJNewGuide::getID()
{
	return m_id;
}
void crJXJNewGuide::setComplete(bool complete)
{
	m_complete = complete;
}
bool crJXJNewGuide::getComplete()
{
	return m_complete;
}
void crJXJNewGuide::setStep(unsigned char step)
{
	m_step = step;
	if(crGlobalHandle::isClient() && !m_complete && isComplete())
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor)
		{
			ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
			streamBuf->createBuf(2);
			streamBuf->_writeUShort(m_id);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvNewguideComplete,streamBuf.get());
			netConductor->getNetManager()->sendPacket("all",packet);
			//m_complete = true;
		}
	}
}
unsigned char crJXJNewGuide::getStep()
{
	return m_step;
}
void crJXJNewGuide::loadData()
{
	ref_ptr<crTableIO>newguideTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTab);
	if(newguideTab.valid())
	{
		crTableIO::StrVec record;
		//int scriptid = newguideTab->getTitleIndex("脚本");
		int stepcountid = newguideTab->getTitleIndex("step总数");
		if(stepcountid>0 && newguideTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0)
		{
			//std::string script = record[scriptid];
			//if(!script.empty())
			//{
			//	crData *data = CREncapsulation::loadData(crGlobalHandle::getInstance()->getScriptDir()+"accomplish/"+script);
			//	if(data)
			//	{
			//		setDataClass(data);
			//	}
			//}
			m_stepCount = atoi(record[stepcountid].c_str());//
		}
	}
}
unsigned char crJXJNewGuide::getStepCount()
{
	return m_stepCount;
}

/////////////////////////////////////////
//
//crJXJHuoyuedu
//活跃度（小管家功能）
/////////////////////////////////////////
crJXJHuoyuedu::crJXJHuoyuedu() :
m_complete(false),
m_id(0),
m_step(0),
m_stepCount(0),
m_Point(0)
{
}
crJXJHuoyuedu::~crJXJHuoyuedu()
{

}
void crJXJHuoyuedu::setID(unsigned short id)
{
	m_id = id;
}
unsigned short crJXJHuoyuedu::getID()
{
	return m_id;
}
void crJXJHuoyuedu::setComplete(bool complete)
{
	m_complete = complete;
}
bool crJXJHuoyuedu::getComplete()
{
	return m_complete;
}
void crJXJHuoyuedu::setStep(unsigned char step)
{
	//if (/*m_step != step && crGlobalHandle::isClient() && */!m_complete)
	{
		if (step > m_stepCount)
		{
			step = m_stepCount;
		}
		m_step = step;
		//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		//if (netConductor)
		//{
		//	ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
		//	streamBuf->createBuf(3);
		//	streamBuf->_writeUShort(m_id);
		//	streamBuf->_writeUChar(m_step);
		//	crPlayerDataEventPacket packet;
		//	crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvHuoyueduStepAdvance, streamBuf.get());
		//	netConductor->getNetManager()->sendPacket("all", packet);
		//	//m_complete = true;
		//}
	}
}
unsigned char crJXJHuoyuedu::getStep()
{
	return m_step;
}
void crJXJHuoyuedu::loadData()
{
	ref_ptr<crTableIO>huoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
	if (huoyueduTab.valid())
	{
		crTableIO::StrVec record;
		//int scriptid = huoyueduTab->getTitleIndex("脚本");
		int stepcountid = huoyueduTab->getTitleIndex("可完成次数");
		int pointid = huoyueduTab->getTitleIndex("活跃值");
		int needlvindex = huoyueduTab->getTitleIndex("等级");
		if (huoyueduTab->queryOneRecord(0, crArgumentParser::appItoa(m_id), record) >= 0)
		{
			//std::string script = record[scriptid];
			//if (!script.empty())
			//{
			//	crData *data = CREncapsulation::loadData(crGlobalHandle::getInstance()->getScriptDir() + "huoyuedu/" + script);
			//	if (data)
			//	{
			//		setDataClass(data);
			//	}
			//}
			m_stepCount = atoi(record[stepcountid].c_str());
			m_Point = atoi(record[pointid].c_str());
			m_needlv = atoi(record[needlvindex].c_str());
		}
	}
}
unsigned char crJXJHuoyuedu::getStepCount()
{
	return m_stepCount;
}
unsigned short crJXJHuoyuedu::getPoint()
{
	return m_Point;
}
unsigned char crJXJHuoyuedu::getNeedlv()
{
	return m_needlv;
}
/////////////////////////////////////////
//
//crJXJNewHuodong
//
/////////////////////////////////////////
crJXJNewHuodong::crJXJNewHuodong():
	m_activation(0),
	m_id(0),
	m_step(0),
	m_stepCount(0),
	m_delFlag(0),
	m_openFlag(0),
	m_circles(1)
{
}
crJXJNewHuodong::~crJXJNewHuodong()
{

}
void crJXJNewHuodong::setID(unsigned short id)
{
	m_id = id;
}
unsigned short crJXJNewHuodong::getID()
{
	return m_id;
}
void crJXJNewHuodong::setStep(int step,int playerid)
{
	if(crGlobalHandle::isClient())
	{
		m_step = step;
	}
	else if(m_activation >= 1)
	{
		if(m_step != step)
		{
			m_step = step;
			if(playerid>0)
			{
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(serverPlayerData.valid())
				{
					ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
					streamBuf->createBuf(6);
					streamBuf->_writeUShort(m_id);
					streamBuf->_writeInt(m_step);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvNewHuodongStep,streamBuf.get());
					gameServer->getNetManager()->sendPacket(serverPlayerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
	}
}
int crJXJNewHuodong::getStep()
{
	return m_step;
}
void crJXJNewHuodong::reset()
{
	m_activation = 0;
	m_step -= m_stepCount;
	if(m_step<0)
		m_step = 0;
}
void crJXJNewHuodong::refresh(int circles)
{
	/*reset();*/
	m_activation = 0;
	m_step = 0;
	m_circles = circles;
}
void crJXJNewHuodong::loadData()
{
	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	if(huodongTab.valid())
	{
		crTableIO::StrVec record;
		int scriptid = huodongTab->getTitleIndex("脚本");
		int stepcountid = huodongTab->getTitleIndex("条件");
		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
		int openIDx = huodongTab->getTitleIndex("开启");
		int delFlagIDx = huodongTab->getTitleIndex("删除标记");
		int circleidx = huodongTab->getTitleIndex("次数");
		int dispidx = huodongTab->getTitleIndex("仅显示");
		int autoAdvance = huodongTab->getTitleIndex("自动领取");
		if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0)
		{
			int delFlag = atoi(record[delFlagIDx].c_str());
			if(delFlag != m_delFlag)
				reset();

			m_stepCount = atoi(record[stepcountid].c_str());
			m_startTime = record[startTimeIdx];
			m_endTime = record[endTimeIdx];
			m_openFlag = atoi(record[openIDx].c_str());
			m_delFlag = delFlag;
			m_circles = atoi(record[circleidx].c_str());
			m_autoAdvance = atoi(record[autoAdvance].c_str());

			int awardIdx[5];
			awardIdx[0] = huodongTab->getTitleIndex("奖励1");
			awardIdx[1] = huodongTab->getTitleIndex("奖励2");
			awardIdx[2] = huodongTab->getTitleIndex("奖励3");
			awardIdx[3] = huodongTab->getTitleIndex("奖励4");
			awardIdx[4] = huodongTab->getTitleIndex("奖励5");
			CRCore::crVector4i awardVec;
			for (unsigned char i = 0; i < 5; ++i)
			{
				if (record[awardIdx[i]] == "") continue;
				crArgumentParser::appAtoVec(record[awardIdx[i]], awardVec);
				m_awards.push_back(awardVec);
			}

			if(!crGlobalHandle::isClient())
			{
				std::string script = record[scriptid];
				if(!script.empty())
				{
					std::string scriptName = crGlobalHandle::getInstance()->getScriptDir()+"huodong/"+script;

					rcfg::ConfigScript cfg_script;
					crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,scriptName,false);
					crData *data = CREncapsulation::loadData(scriptName);
					if(data)
					{
						setDataClass(data);
					}
				}
			}
			else
			{
				if(atoi(record[dispidx].c_str())==1)
				{
					m_activation = 1;
				}
			}
		}
	}
}
int crJXJNewHuodong::getStepCount()
{
	return m_stepCount;
}
char crJXJNewHuodong::getOpenFlg()
{
	return m_openFlag;
}
void crJXJNewHuodong::setCircles(unsigned char circles)
{
	m_circles = circles;
}
unsigned char crJXJNewHuodong::getCircles()
{
	return m_circles;
}
void crJXJNewHuodong::setDelFlag(unsigned char delflag)
{
	m_delFlag = delflag;
}
unsigned char crJXJNewHuodong::getDelFlag()
{
	return m_delFlag;
}
std::string crJXJNewHuodong::getStartTime()
{
	return m_startTime;
}
std::string crJXJNewHuodong::getEndTime()
{
	return m_endTime;
}
bool crJXJNewHuodong::isNeedSave()
{
	return m_openFlag && (m_activation>0 || m_circles>1);
}
void crJXJNewHuodong::setActivation(unsigned char activation, int playerid)
{
	if(m_activation != activation)
	{
		m_activation = activation;
		if(!crGlobalHandle::isClient() && playerid>0)
		{
			if(m_activation==2)
			{
				if(m_circles > 1)
				{
					m_circles--;
					reset();
					m_activation = 1;
				}
				else if(m_circles == 1)
				{
					m_circles--;
				}
				//else if(m_circles == 0)
				//{
				//	reset();
				//}
			}
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(serverPlayerData.valid())
			{
				ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
				streamBuf->createBuf(8);
				streamBuf->_writeUShort(m_id);
				streamBuf->_writeUChar(m_activation);
				streamBuf->_writeUChar(m_circles);
				streamBuf->_writeInt(m_step);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvNewHuodongActive,streamBuf.get());
				gameServer->getNetManager()->sendPacket(serverPlayerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
}
unsigned char crJXJNewHuodong::getActivation()
{
	return m_activation;
}
void crJXJNewHuodong::activationCheck(int playerid)
{
	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	crTableIO::StrVec huoRecord;
	if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),huoRecord) >= 0)
	{
		int activeCondition = huodongTab->getTitleIndex("条件激活");
		if(playerid>0 && m_activation<2 && (huoRecord[activeCondition].empty() || huoRecord[activeCondition] == "0"))
		{
			if(isPreCompleted(playerid)>0)
			{
				if(isIntimeCheck())
				{
					setActivation(1,playerid);
				}
				else
				{
					setActivation(0,playerid);
				}
			}
			//else
			//{
			//	setActivation(0,playerid);
			//}
		}
	}
}

int crJXJNewHuodong::isPreCompleted( int playerid )
{
	int preCompleted = -1;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (serverPlayerData.valid() && serverPlayerData->getPlayerGameData())
	{
		ref_ptr<crPlayerGameData > playerData = serverPlayerData->getPlayerGameData();
		ref_ptr<crRole> mainRole = playerData->getMainRole();
		if(mainRole.valid() && mainRole->getDataClass())
		{
			preCompleted = 1;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if (huodongTab.valid())
			{
				int preIDIdx = huodongTab->getTitleIndex("前置id");
				int preID = 0;
				crTableIO::StrVec record;
				if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record) >= 0)
				{
					preID = atoi(record[preIDIdx].c_str());
					if(preID>0)
					{
						void *param;
						crData *roleData = mainRole->getDataClass();
						roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *newHuodongMap= (NewHuodongMap *)param;
						NewHuodongMap::iterator hmItr = newHuodongMap->find(preID);
						if (newHuodongMap->end() != hmItr)
						{
							preCompleted = hmItr->second->getComplete()?1:0;
						}
						roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
		}
	}
	return preCompleted;
}
bool crJXJNewHuodong::isIntimeCheck()
{
	if (!m_openFlag)
	{
		return false;
	}
	do 
	{
		ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		crTableIO::StrVec huoRecord;
		if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(this->getID()),huoRecord) < 0) break;

		int isOpenIdx = huodongTab->getTitleIndex("开启");
		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
		int opendayIdx = huodongTab->getTitleIndex("开服天数");

		time_t t = time( 0 );
		char tmp[20];
		strftime( tmp, sizeof(tmp), "%Y|%m|%d\0", localtime(&t) );
		crVector3i opentm,curtm,endtm;
		crArgumentParser::appTimetoVec(huoRecord[startTimeIdx],opentm,'|');
		crArgumentParser::appTimetoVec(tmp,curtm,'|');
		crArgumentParser::appTimetoVec(huoRecord[endTimeIdx],endtm,'|');

		bool isserveropenday = false;
		CRCore::crVector2i opendayVec2i;
		crArgumentParser::appAtoVec(huoRecord[opendayIdx],opendayVec2i);
		if (opendayVec2i[0] != 0)
		{
			std::string dateStr = crGlobalHandle::gData()->gStartDate();
			typedef std::vector<int> Vec5i;
			Vec5i startTimeVec5i;
			startTimeVec5i.reserve(5);
			crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
			startTimeVec5i.resize(5,0);
			time_t nowTime_t = time(0);
			struct  tm  nowTm = *localtime(&nowTime_t);
			struct tm  startTm;
			memset(&startTm,0,sizeof (startTm));
			startTm.tm_year = startTimeVec5i[0] - 1900;
			startTm.tm_mon = startTimeVec5i[1] - 1;
			startTm.tm_mday = startTimeVec5i[2];
			startTm.tm_hour = startTimeVec5i[3];
			startTm.tm_min = startTimeVec5i[4];
			startTm.tm_isdst = nowTm.tm_isdst;
			time_t startTime_t = mktime(&startTm);
			if (opendayVec2i[1] == 0)
			{
				if ((nowTime_t - startTime_t) / c_24hours == opendayVec2i[0] - 1)
				{
					isserveropenday = true;
				}
			}
			else
			{
				if ( ((nowTime_t - startTime_t) / c_24hours >= opendayVec2i[0] - 1) && ((nowTime_t - startTime_t) / c_24hours <= opendayVec2i[1] - 1) )
				{
					isserveropenday = true;
				}
			}
		}
		else
		{
			isserveropenday = true;
		}

		if(opentm<=curtm && curtm<=endtm && isserveropenday)
		{
			return true;
		}

	} while (0);
	return false;
}