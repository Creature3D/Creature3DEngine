/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNet/crNetContainer.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
#include <CREncapsulation/crTableIO.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRDownload/crDownload.h>
#include <CREncapsulation/crLoader.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crFileNameUtils.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CRNet;
using namespace CREncapsulation;
using namespace CRDataBase;
using namespace CRDownload;
using namespace CRIOManager;
/////////////////////////////////////////
//
//crGlobalData
//
/////////////////////////////////////////
crGlobalData::crGlobalData():
g_unitScale(0.01f),
g_maxLapover(99),
g_salerate(0.25f),
g_removeOutRangeTime(120.0f),//100s
g_recycleIDTime(130.0f),//110s
g_recycleStreamIDTime_in(60.0f),//服务器间(局域网内)回收时间
g_recycleStreamIDTime_out(180.0f),//外网回收时间
g_maxgroupmember(16),
g_maxSkillLevelLearn(20),//可学到的最高技能等级
g_minFollowDistance(1.0f),
g_minItemUseRange(3.0f),//武器的最小攻击距离
g_itemVisiableDelay(200),//ms
g_maxRecBufSize(524288),//0.5MB
g_gameChatRecFile("rec/gamechat.cfg"),
g_chatMsgRecFile("rec/chatmsg.cfg"),
g_numGameChatShow(50),
g_numGameChatClear(30),
g_maxInRangePlayer(100),
g_maxInRangeItem(100),
g_streamCacheMaxTime(600),
g_maxDisplayTaskTrace(5),
g_removeItemChildTime(20.0f),//20s
g_controllerTime(0.5f),
g_layerHeight(2.5f),
g_walkHeight(0.8f),
g_maxRoleCanJoin(1),
g_firstGameSceneID(0),
g_gameid(0),
g_mainRoleAItemID(0),
g_mainRoleAMetierID(0),
g_showDebugInfo(0),
g_enableRobotTest(0),
g_sightRange(1000),
g_eyeHeight(150),
g_itemAdjustAngular(20.0f),
g_followDelta(2.0f),
g_portScopeSqr(100.0f),
g_itemVisiableInterval(1.0f),
g_loginConnectCount(1),
g_dynamicDownloadTabName("dynamic.crb"),
g_dynamicTryWait(1.0f)
{
}
crGlobalData::crGlobalData(const crGlobalData& data):
crData(data),
g_startDate(data.g_startDate),
g_unitScale(data.g_unitScale),
g_maxLapover(data.g_maxLapover),
g_salerate(data.g_salerate),
g_removeOutRangeTime(data.g_removeOutRangeTime),
g_recycleIDTime(data.g_recycleIDTime),
g_recycleStreamIDTime_in(data.g_recycleStreamIDTime_in),
g_recycleStreamIDTime_out(data.g_recycleStreamIDTime_out),
g_maxgroupmember(data.g_maxgroupmember),
g_maxSkillLevelLearn(data.g_maxSkillLevelLearn),
g_minFollowDistance(data.g_minFollowDistance),
g_minItemUseRange(data.g_minItemUseRange),
g_itemVisiableDelay(data.g_itemVisiableDelay),
g_maxRecBufSize(data.g_maxRecBufSize),
g_gameChatRecFile(data.g_gameChatRecFile),
g_chatMsgRecFile(data.g_chatMsgRecFile),
g_numGameChatShow(data.g_numGameChatShow),
g_numGameChatClear(data.g_numGameChatClear),
g_maxInRangePlayer(data.g_maxInRangePlayer),
g_maxInRangeItem(data.g_maxInRangeItem),
g_streamCacheMaxTime(data.g_streamCacheMaxTime),
g_maxDisplayTaskTrace(data.g_maxDisplayTaskTrace),
g_removeItemChildTime(data.g_removeItemChildTime),
g_controllerTime(data.g_controllerTime),
g_layerHeight(data.g_layerHeight),
g_walkHeight(data.g_walkHeight),
g_maxRoleCanJoin(data.g_maxRoleCanJoin),
g_firstGameSceneID(data.g_firstGameSceneID),
g_gameid(data.g_gameid),
g_gameGlobalTable(data.g_gameGlobalTable),
g_globalTables(data.g_globalTables),
g_globalTableFileMap(data.g_globalTableFileMap),
g_mainRoleAItemID(data.g_mainRoleAItemID),
g_mainRoleAMetierID(data.g_mainRoleAMetierID),
g_showDebugInfo(data.g_showDebugInfo),
g_enableRobotTest(data.g_enableRobotTest),
g_sightRange(data.g_sightRange),
g_eyeHeight(data.g_eyeHeight),
g_itemAdjustAngular(data.g_itemAdjustAngular),
g_followDelta(data.g_followDelta),
g_portScopeSqr(data.g_portScopeSqr),
g_itemVisiableInterval(data.g_itemVisiableInterval),
g_loginConnectCount(data.g_loginConnectCount),
g_dynamicDownloadTabName(data.g_dynamicDownloadTabName),
g_newDynamicDownloadTable(data.g_newDynamicDownloadTable),
//g_gmTable(data.g_gmTable),
//g_dynamicTryCount(data.g_dynamicTryCount),
g_dynamicTryWait(data.g_dynamicTryWait)
{
}
void crGlobalData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_gStartDate:
		g_startDate = relStr;
		break;
	case WCHDATA_gUnitScale:
		g_unitScale = atof(relStr.c_str());
		break;
	case WCHDATA_gMaxLapover:
		g_maxLapover = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gSaleRate:
		g_salerate = atof(relStr.c_str());
		break;
	case WCHDATA_gRemoveOutRangeTime:
		g_removeOutRangeTime = atof(relStr.c_str());
		break;
	case WCHDATA_gRecycleIDTime:
		g_recycleIDTime = atof(relStr.c_str());
		break;
	case WCHDATA_gRecycleStreamIDTime_in:
		g_recycleStreamIDTime_in = atof(relStr.c_str());
		break;
	case WCHDATA_gRecycleStreamIDTime_out:
		g_recycleStreamIDTime_out = atof(relStr.c_str());
		break;
	case WCHDATA_gMaxgroupmember:
		g_maxgroupmember = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gMaxSkillLevelLearn:
		g_maxSkillLevelLearn = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gMinFollowDistance:
		g_minFollowDistance = atof(relStr.c_str());
		break;
	case WCHDATA_gMinItemUseRange:
		g_minItemUseRange = atof(relStr.c_str());
		break;
	case WCHDATA_gItemVisiableDelay:
		g_itemVisiableDelay = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gMaxRecBufSize:
		g_maxRecBufSize = atoi(relStr.c_str());
		break;
	case WCHDATA_gGameChatRecFile:
		g_gameChatRecFile = relStr;
		break;
	case WCHDATA_gChatMsgRecFile:
		g_chatMsgRecFile = relStr;
		break;
	case WCHDATA_gNumGameChatShow:
		g_numGameChatShow = atoi(relStr.c_str());
		break;
	case WCHDATA_gNumGameChatClear:
		g_numGameChatClear = atoi(relStr.c_str());
		break;
	case WCHDATA_gMaxInRangePlayer:
		g_maxInRangePlayer = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gMaxInRangeItem:
		g_maxInRangeItem = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gStreamCacheMaxTime:
		g_streamCacheMaxTime = atof(relStr.c_str());
		break;
	case WCHDATA_gMaxDisplayTaskTrace:
		g_maxDisplayTaskTrace = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gRemoveItemChildTime:
		g_removeItemChildTime = atof(relStr.c_str());
		break;
	case WCHDATA_gControllerTime:
		g_controllerTime = atof(relStr.c_str());
		break;
	case WCHDATA_gLayerHeight:
		g_layerHeight = atof(relStr.c_str());
		break;
	case WCHDATA_gWalkHeight:
		g_walkHeight = atof(relStr.c_str());
		break;
	case WCHDATA_gMaxRoleCanJoin:
		g_maxRoleCanJoin = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gFirstGameSceneID:
		g_firstGameSceneID = atoi(relStr.c_str());
		break;
	case WCHDATA_gGameID:
		g_gameid = atoi(relStr.c_str());
		break;
	case WCHDATA_gMainRoleAItemID:
		g_mainRoleAItemID = atoi(relStr.c_str());
		break;
	case WCHDATA_gMainRoleAMetierID:
		g_mainRoleAMetierID = atoi(relStr.c_str());
		break;
	case WCHDATA_gLoginConnectCount:
		g_loginConnectCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gGameGlobalTable:
		g_gameGlobalTable = crTableIO::openFile(relStr);
		//g_gameGlobalTable->openFile(relStr);
		break;
	//case WCHDATA_gGMTable:
	//	g_gmTable = crTableIO::openFile(relStr);
	//	break;
	case WCHDATA_gGlobalTables:
		{
			std::string::size_type comma2 = relStr.find_first_of(',');
			std::string type2 = std::string(relStr.begin(),relStr.begin()+comma2);
			relStr = std::string(relStr.begin()+comma2 + 1,relStr.end());
			int _type2 = atoi(type2.c_str());
			crTableIO *tableIO = crTableIO::openFile(relStr);
			if(!tableIO && _type2 == WCHDATA_DynamicDownloadTab)
			{
				tableIO = new crTableIO;
				//检测及修复版本信息表
				CREncapsulation::crTableIO::StrVec title;
				title.push_back("file");
				title.push_back("version");
				tableIO->setTitleVec(title);
			}
			g_globalTables[_type2] = tableIO;
			g_globalTableFileMap[_type2] = relStr;
		}
		break;
	case WCHDATA_gShowDebugInfo:
		g_showDebugInfo = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gEnableRobotTest:
		g_enableRobotTest = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_gSightRange:
		{
			float range = atof(relStr.c_str());
			range /= crGlobalHandle::gData()->gUnitScale();
			g_sightRange = (short)range;
		}
		break;
	case WCHDATA_gEyeHeight:
		{
			float height = atof(relStr.c_str());
			height /= crGlobalHandle::gData()->gUnitScale();
			g_eyeHeight = (short)height;
		}
		break;
	case WCHDATA_gItemAdjustAngular:
		g_itemAdjustAngular = atof(relStr.c_str());
		break;
	case WCHDATA_gFollowDelta:
		g_followDelta = atof(relStr.c_str());
		break;
	case WCHDATA_gPortScopeSqr:
		g_portScopeSqr = atof(relStr.c_str());
		break;
	case WCHDATA_gItemVisiableInterval:
		g_itemVisiableInterval = atof(relStr.c_str());
		break;
	case WCHDATA_gDynamicDownloadTabName:
		g_dynamicDownloadTabName = relStr.c_str();
		break;
	//case WCHDATA_gDynamicTryCount:
	//	g_dynamicTryCount = (char)atoi(relStr.c_str());
	//	break;
	case WCHDATA_gDynamicTryWait:
		g_dynamicTryWait = atof(relStr.c_str());
		break;
	}
}
void crGlobalData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_gNewDynamicDownloadTab:
		g_newDynamicDownloadTable = (crTableIO*)param;
		break;
	}
}
void crGlobalData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_gStartDate:
		param = &g_startDate;
		break;
	case WCHDATA_gUnitScale:
		param = &g_unitScale;
		break;
	case WCHDATA_gMaxLapover:
		param = &g_maxLapover;
		break;
	case WCHDATA_gSaleRate:
		param = &g_salerate;
		break;
	case WCHDATA_gRemoveOutRangeTime:
		param = &g_removeOutRangeTime;
		break;
	case WCHDATA_gRecycleIDTime:
		param = &g_recycleIDTime;
		break;
	case WCHDATA_gRecycleStreamIDTime_in:
		param = &g_recycleStreamIDTime_in;
		break;
	case WCHDATA_gRecycleStreamIDTime_out:
		param = &g_recycleStreamIDTime_out;
		break;
	case WCHDATA_gMaxgroupmember:
		param = &g_maxgroupmember;
		break;
	case WCHDATA_gMaxSkillLevelLearn:
		param = &g_maxSkillLevelLearn;
		break;
	case WCHDATA_gMinFollowDistance:
		param = &g_minFollowDistance;
		break;
	case WCHDATA_gMinItemUseRange:
		param = &g_minItemUseRange;
		break;
	case WCHDATA_gItemVisiableDelay:
		param = &g_itemVisiableDelay;
		break;
	case WCHDATA_gMaxRecBufSize:
		param = &g_maxRecBufSize;
		break;
	case WCHDATA_gGameChatRecFile:
		param = &g_gameChatRecFile;
		break;
	case WCHDATA_gChatMsgRecFile:
		param = &g_chatMsgRecFile;
		break;
	case WCHDATA_gNumGameChatShow:
		param = &g_numGameChatShow;
		break;
	case WCHDATA_gNumGameChatClear:
		param = &g_numGameChatClear;
		break;
	case WCHDATA_gMaxInRangePlayer:
		param = &g_maxInRangePlayer;
		break;
	case WCHDATA_gMaxInRangeItem:
		param = &g_maxInRangeItem;
		break;
	case WCHDATA_gStreamCacheMaxTime:
		param = &g_streamCacheMaxTime;
		break;
	case WCHDATA_gMaxDisplayTaskTrace:
		param = &g_maxDisplayTaskTrace;
		break;
	case WCHDATA_gRemoveItemChildTime:
		param = &g_removeItemChildTime;
		break;
	case WCHDATA_gControllerTime:
		param = &g_controllerTime;
		break;
	case WCHDATA_gLayerHeight:
		param = &g_layerHeight;
		break;
	case WCHDATA_gWalkHeight:
		param = &g_walkHeight;
		break;
	case WCHDATA_gMaxRoleCanJoin:
		param = &g_maxRoleCanJoin;
		break;
	case WCHDATA_gFirstGameSceneID:
		param = &g_firstGameSceneID;
		break;
	case WCHDATA_gGameID:
		param = &g_gameid;
		break;
	case WCHDATA_gMainRoleAItemID:
		param = &g_mainRoleAItemID;
		break;
	case WCHDATA_gMainRoleAMetierID:
		param = &g_mainRoleAMetierID;
		break;
	case WCHDATA_gLoginConnectCount:
		param = &g_loginConnectCount;
		break;
	case WCHDATA_gGameGlobalTable:
		param = g_gameGlobalTable.get();
		break;
	//case WCHDATA_gGMTable:
	//	param = g_gmTable.get();
	//	break;
	case WCHDATA_gDynamicDownloadTabName:
		param = &g_dynamicDownloadTabName;
		break;
	case WCHDATA_gNewDynamicDownloadTab:
		param = &g_newDynamicDownloadTable;
		break;
	default:
		param = NULL;
		break;
	}
}
//void crGlobalData::reloadGMTable(const std::string &filename)
//{
//	crScriptLoadManager::getInstance()->eraseLoadedRefeanced(filename);
//	g_gmTable = crTableIO::openFile(filename);
//}
void crGlobalData::reloadGameGlobalTable(const std::string &filename)
{
	crScriptLoadManager::getInstance()->eraseLoadedRefeanced(filename);
	g_gameGlobalTable = crTableIO::openFile(filename);
}
void crGlobalData::reloadGlobalTable(int id)
{
	TableFileMap::iterator itr = g_globalTableFileMap.find(id);
	if(itr != g_globalTableFileMap.end())
	{
		reloadGlobalTable(id,itr->second);
	}
}
void crGlobalData::reloadGlobalTable(int id,const std::string &filename)
{
	crScriptLoadManager::getInstance()->eraseLoadedRefeanced(filename);
	g_globalTables[id] = crTableIO::openFile(filename);
}
void crGlobalData::lockGMMap()
{
	m_gmMapMutex.acquire();
}
GMMap &crGlobalData::getGMMap()
{
	return m_gmMap;
}
short crGlobalData::getGMAuthority(int playerid)//-1表示非GM
{
	GNE::LockMutex lock( m_gmMapMutex );
	GMMap::iterator itr = m_gmMap.find(playerid);
	return itr != m_gmMap.end()?itr->second:-1;
}
void crGlobalData::unlockGMMap()
{
	m_gmMapMutex.release();
}
/////////////////////////////////////////
//
//crGameClientData
//
/////////////////////////////////////////
crGameClientData::crGameClientData()
{
}
crGameClientData::crGameClientData(const crGameClientData& data):
crGlobalData(data)
{
}
void crGameClientData::addParam(int i, const std::string& str)
{
	crGlobalData::addParam(i,str);
}

void crGameClientData::inputParam(int i, void *param)
{
	crGlobalData::inputParam(i,param);
}

void crGameClientData::getParam(int i, void*& param)
{
	crGlobalData::getParam(i,param);
}

void crGameClientData::insertFriendChatMap(CRGUI::crImageStage *stage, crNetChatData::crChatFriend *chatFriend)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendChatMapMutex);
	m_friendChatMap[stage] = chatFriend;
}

crNetChatData::crChatFriend *crGameClientData::getChatFriend(CRGUI::crImageStage *stage)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendChatMapMutex);
	FriendChatMap::iterator itr = m_friendChatMap.find(stage);
	return itr==m_friendChatMap.end()?NULL:itr->second.get();
}

CRGUI::crImageStage *crGameClientData::getChatFriendStage(crNetChatData::crChatFriend *chatFriend)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendChatMapMutex);
	for( FriendChatMap::iterator itr = m_friendChatMap.begin();
		 itr != m_friendChatMap.end();
		 ++itr )
	{
		if(itr->second == chatFriend)
			return const_cast<CRGUI::crImageStage *>(itr->first.get());
	}
	return NULL;
}

bool crGameClientData::insertDownloadFile(int msgparam, const std::string &file)
{//对于公用的文件处理有问题
	GNE::LockMutex lock( m_downloadFileMutex );
	DownloadFileTaskMap::iterator itr = m_downloadFileTaskMap.find(msgparam);
	if(itr != m_downloadFileTaskMap.end())
	{
		for ( DownloadFileDeque::iterator ditr = itr->second.begin();
			  ditr != itr->second.end();
			  ++ditr )
		{
			if((*ditr).compare(file) == 0)
			{
				return false;
			}
		}
		itr->second.push_back(file);
	}
	else
	{
		m_downloadFileTaskMap[msgparam].push_back(file);
	}
	return true;
}
void crGameClientData::startDownloadFile(int msgparam,short netType)
{
	GNE::LockMutex lock( m_downloadFileMutex );
	DownloadFileTaskMap::iterator itr = m_downloadFileTaskMap.find(msgparam);
	if(itr != m_downloadFileTaskMap.end())
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(netType);
		crDownloadFilePacket packet;
		crDownloadFilePacket::buildRequestPacket(packet,itr->second.front(),msgparam);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
void crGameClientData::recvedFileStream(crRecvFileStream *fileStream)
{
	GNE::LockMutex lock( m_downloadFileMutex );
	DownloadFileTaskMap::iterator itr = m_downloadFileTaskMap.find(fileStream->getDownloadFlg());
	if(itr != m_downloadFileTaskMap.end())
	{
		if(!itr->second.empty() && itr->second.front().compare(fileStream->getFileName()) == 0)
		{
			itr->second.pop_front();
		}
		if (itr->second.empty())
		{
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,fileStream->getDownloadFlg()),NULL);
			m_downloadFileTaskMap.erase(itr);
		}
		else
		{//开始下载下一个文件
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(fileStream->getNetType());
			crDownloadFilePacket packet;
			crDownloadFilePacket::buildRequestPacket(packet,itr->second.front(),fileStream->getDownloadFlg());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else
	{
		crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,fileStream->getDownloadFlg()),NULL);
	}
}
void crGameClientData::fileNotExist(unsigned short downloadFlg,const std::string &file,short netType)
{//文件不存在
	GNE::LockMutex lock( m_downloadFileMutex );
	DownloadFileTaskMap::iterator itr = m_downloadFileTaskMap.find(downloadFlg);
	if(itr != m_downloadFileTaskMap.end())
	{
		if(!itr->second.empty() && itr->second.front().compare(file) == 0)
		{
			itr->second.pop_front();
		}
		if (itr->second.empty())
		{
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,downloadFlg),NULL);
			m_downloadFileTaskMap.erase(itr);
		}
		else
		{//开始下载下一个文件
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(netType);
			crDownloadFilePacket packet;
			crDownloadFilePacket::buildRequestPacket(packet,itr->second.front(),downloadFlg);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else
	{
		crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,downloadFlg),NULL);
	}
}
//void crGameClientData::lockHttpDownload()
//{
//	m_httpDownloadMutex.acquire();
//}
//void crGameClientData::unlockHttpDownload()
//{
//	m_httpDownloadMutex.release();
//}
void crGameClientData::insertHttpDownloadFile(int msgparam, const std::string &file)
{
	GNE::LockMutex lock( m_perhttpDownloadMutex );
	DownloadFileTaskMap::iterator itr = m_perhttpDownloadTaskMap.find(msgparam);
	if(itr != m_perhttpDownloadTaskMap.end())
	{
		//for ( DownloadFileDeque::iterator ditr = itr->second.begin();
		//	ditr != itr->second.end();
		//	++ditr )
		//{
		//	if((*ditr).compare(file) == 0)
		//	{
		//		return false;
		//	}
		//}
		itr->second.push_back(file);
	}
	else
	{
		m_perhttpDownloadTaskMap[msgparam].push_back(file);
	}
}
void crGameClientData::startHttpDownloadTask(int msgparam)
{
	GNE::LockMutex lock1( m_perhttpDownloadMutex );
	GNE::LockMutex lock2( m_httpDownloadMutex );
	DownloadFileTaskMap::iterator itr = m_perhttpDownloadTaskMap.find(msgparam);
	if(itr != m_perhttpDownloadTaskMap.end())
	{
		DownloadFileTaskMap::iterator hitr = m_httpDownloadTaskMap.find(msgparam);
		if(hitr != m_httpDownloadTaskMap.end())
		{
			bool found = false;
			for( DownloadFileDeque::iterator fitr = itr->second.begin();
				fitr != itr->second.end();
				++fitr )
			{
				found = false;
				for( DownloadFileDeque::iterator dfitr = hitr->second.begin();
					 dfitr != hitr->second.end();
					 ++dfitr )
				{
					if((*dfitr).compare(*fitr) == 0)
					{
						found = true;
						break;
					}
				}
				if(!found)
					hitr->second.push_back(*fitr);
			}
		}
		else
		{
			m_httpDownloadTaskMap[msgparam] = itr->second;
		}
		m_perhttpDownloadTaskMap.erase(itr);
	}
}
//short crGameClientData::gainHttpDownloadID()
//{
//	static short s_id = 0;
//	return s_id++;
//}
void crGameClientData::downloadUpdate()
{
	std::vector<int>DoneVec;
	{
		GNE::LockMutex lock( m_httpDownloadMutex );
		std::string file;
		ref_ptr<crDownload> download;
		ref_ptr<CREncapsulation::crTableIO> curDynamictab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_DynamicDownloadTab);
		if(!curDynamictab.valid())
			return;
		ref_ptr<crTableIO> newDynamictab = crGlobalHandle::gData()->gNewDynamicDownloadTable();
		CREncapsulation::crTableIO::StrVec qrecord;
		std::string newver;
		int rowcount;
		bool found = false;
		int i;
		crTableIO::StrVec record;
		record.resize(2);
		bool needsave = false;
		for( DownloadFileTaskMap::iterator itr = m_httpDownloadTaskMap.begin();
			itr != m_httpDownloadTaskMap.end();
			)
		{
			if(itr->second.empty())
			{
				if(LOINT32(itr->first)!=HttpD_None)
				{
					DoneVec.push_back(itr->first);
				}
				else
				{
					crGlobalHandle::recycleHttpDownloadID(HIINT32(itr->first));
				}
				itr = m_httpDownloadTaskMap.erase(itr);
				continue;
			}
			//file = itr->second.front();
			for( DownloadFileDeque::iterator fitr = itr->second.begin();
				 fitr != itr->second.end(); )
			{
				file = *fitr;
				download = crDownloadManager::getInstance()->getDownload(file);
				if(!download.valid())
				{
					//itr->second.pop_front();
					fitr = itr->second.erase(fitr);
					//CRCore::notify(CRCore::ALWAYS)<<"crGameClientData::downloadUpdate() 移动下载文件出错："<<file<<std::endl;
				}
				else if(download->getDownloadState() == crDownload::Done)
				{
					download->moveFileToDirectory();
					newver.clear();
					if(newDynamictab.valid() && newDynamictab->queryOneRecord(0,file,qrecord)>=0)
					{
						newver = qrecord[1];
					}
					rowcount = curDynamictab->getRowCount();
					found = false;
					for( i = 0; i<rowcount; i++)
					{
						//if((*curDynamictab)(i,0) == file)
						if(CRIOManager::equalCaseInsensitive((*curDynamictab)(i,0),file))
						{
							(*curDynamictab)(i,1) = newver;
							found = true;
							break;
						}
					}
					if(!found)
					{
						record[0] = file;
						record[1] = newver;
						curDynamictab->addData(record);
					}
					needsave = true;
					crDownloadManager::getInstance()->removeDownload(file);
					//itr->second.pop_front();
					fitr = itr->second.erase(fitr);
				}
				else
				{
					++fitr;
				}
			}
			++itr;
		}
		if(needsave)
			curDynamictab->saveToFileStreamNoCook(crGlobalHandle::gData()->gDynamicDownloadTabName(),SM_Copy);
	}
	if(!DoneVec.empty())
	{
		_crInt16 loid = 0;
		_crInt16 hiid = 0;
		char gbuf[256];
		for( std::vector<int>::iterator itr = DoneVec.begin();
			itr != DoneVec.end();
			++itr )
		{
			loid = LOINT32(*itr);
			hiid = HIINT32(*itr);
			sprintf(gbuf,"DownloadTaskDone HttpDownload:%d,GainID:%d\n\0",loid,hiid);
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,loid),MAKEINT64(hiid,NULL));
			if(loid == HttpD_DownLoadItem || loid == HttpD_DownLoadItemChild)
			{
				crGlobalHandle::recycleHttpDownloadID(hiid);
			}
		}
	}
}
/////////////////////////////////////////
//
//crRoleData
//
/////////////////////////////////////////
GNE::Mutex crRoleData::m_saveMutex;
crRoleData::crRoleData():
	m_maxattack(1/*5*/),
	m_maxskill(0/*30*/),
	m_maxbackpack(0/*50*/),
	m_maxstorage(0/*50*/),
	m_maxequipon(0/*6*/),
	m_maxquick(0/*18*/),
	m_sightRange(2000),
	m_guardRange(1000),
	m_eyeHeight(150),
	m_hp(100),
	m_mp(50),
	m_attack(10),
	m_defend(0),
	m_magicDefend(0),
	m_speed(400),
	m_stealth(0),
	m_stealthspy(0),
	m_hitratio(0),
	m_attackSpeed(0),
	m_skillSpeed(0),
	m_armourType(0),
	m_camp(Nonaligned),
	m_targetType(0),
	m_targetID(0),
	m_targetRoleID(0),
	m_itemState(0),
	m_guiseState(GS_Normal),
	m_currentAttackID(0),
	m_aboutToUseItemID(0),
	m_followDistance(1.0f),
	m_fightmode(FM_Peace),
	m_patrolIndex(0),
	m_patrolLoopMode(PL_Swing),
	//m_baseEnmity(0),
	m_extrahp(0),
	m_extrahp_rd(0),
	m_extramp(0),
	m_extramp_rd(0),
	m_extraAttack(0),
	m_extraAttack_rd(0),
	m_extraDefend(0),
	m_extraDefend_rd(0),
	//m_extraSpeed(0),
	//m_extraSpeed_rd(0),
	m_extraStealth(0),
	m_extraStealth_rd(0),
	m_extraStealthSpy(0),
	m_extraStealthSpy_rd(0),
	m_extraHitratioPercent(0),
	m_extraHitratioPercent_rd(0),
	m_extraAttackSpeed(0),
	m_extraAttackSpeed_rd(0),
	m_extraSkillSpeed(0),
	m_extraSkillSpeed_rd(0),
	m_extraGuise(GS_Normal),
	m_extraGuise_rd(GS_Normal),
	m_extraRadius(100),
	m_extraRadius_rd(100),
	m_extraIgnoreDefend(0),
	m_extraIgnoreDefend_rd(0),
	m_extraIgnoreMagicDefend(0),
	m_extraIgnoreMagicDefend_rd(0),
	m_extraSightRange(0),
	m_extraSightRange_rd(0),
	//m_currentUseItemProtectTime(0),
	m_currentActState(0),
	m_currentActDuration(0.0f),
	m_crit(0),
	m_parry(0),
	m_dodge(0),
	m_extraCrit(0),
	m_extraCrit_rd(0),
	m_extraParry(0),
	m_extraParry_rd(0),
	m_extraDodge(0),
	m_extraDodge_rd(0),
	m_isCrit(false),
	m_isParry(false),
	m_isDodge(false),
	m_isMiss(false),
	m_extraAttackPercent(0),
	m_extraAttackPercent_rd(0),
	m_extraSpeedPercent(0),
	m_extraSpeedPercent_rd(0),
	m_extraDefendPercent(0),
	m_extraDefendPercent_rd(0),
	m_extraMagicDefendPercent(0),
	m_extraMagicDefendPercent_rd(0),
	m_extraBuildAttackPercent(0),
	m_extraBuildAttackPercent_rd(0),
	m_extraCritPercent(0),
	m_extraCritPercent_rd(0),
	m_extraParryPercent(0),
	m_extraParryPercent_rd(0),
	m_extraDodgePercent(0),
	m_extraDodgePercent_rd(0),
	m_extraAttackDist(0),
	m_extraAttackDist_rd(0),
	m_extraAttackAbsorbHP(0),
	m_extraAttackAbsorbHP_rd(0),
	m_extraSkillAbsorbHP(0),
	m_extraSkillAbsorbHP_rd(0),
	m_extraAttackDamageBack(0),
	m_extraAttackDamageBack_rd(0),
	m_extraSkillDamageBack(0),
	m_extraSkillDamageBack_rd(0),
	m_extraRTHP(0.0f),
	m_extraRTHP_rd(0.0f),
	m_inbornHPPercent(0),
	m_inbornMPPercent(0),
	m_inbornAttackPercent(0),
	m_inbornDefendPercent(0),
	m_inbornMagicDefendPercent(0),
	m_inbornBuildAttackPercent(0),
	m_inbornSpeed(0),
	m_inbornAttackSpeedPercent(0),
	m_inbornSkillSpeedPercent(0),
	m_inbornSightRange(0),
	m_inbornAttackDist(0),
	m_inbornStealth(0),
	m_inbornStealthSpy(0),
	m_inbornCrit(0),
	m_inbornParry(0),
	m_inbornDodge(0),
	m_inbornIgnoreDefend(0),
	m_inbornIgnoreMagicDefend(0),
	m_inbornIgnoreDodge(0),
	m_inbornAttackDamageBack(0),
	m_inbornSkillDamageBack(0),
	m_inbornAttackAbsorbHP(0),
	m_inbornSkillAbsorbHP(0),
	m_inbornCritPower(0),
	m_inbornSputtering(0),
	m_inbornAttackDamage(0),
	m_inbornSkillDamage(0),
	m_extraAttackDamage(0),
	m_extraAttackDamage_rd(0),
	m_extraSkillDamage(0),
	m_extraSkillDamage_rd(0),
	m_inbornHitratioPercent(0),
	m_extraIgnoreDodge(0),
	m_extraIgnoreDodge_rd(0),
	m_extraCritPower(0),
	m_extraCritPower_rd(0),
	m_extraSputtering(0),
	m_extraSputtering_rd(0)
{
	m_rthp = m_hp;
	m_rtmp = m_mp;
	m_halfPhysicsBox.set(0.7f, 0.7f, 1.0f);
}
crRoleData::crRoleData(const crRoleData& data):
	crData(data),
	m_maxattack(data.m_maxattack),
	m_maxskill(data.m_maxskill),
	m_maxbackpack(data.m_maxbackpack),
	m_maxstorage(data.m_maxstorage),
	m_maxequipon(data.m_maxequipon),
	m_maxquick(data.m_maxquick),
	m_attackMap(data.m_attackMap),
	m_skillMap(data.m_skillMap),
	//m_backPackMap(data.m_backPackMap),
	//m_storageMap(data.m_storageMap),
	//m_storageCountMap(data.m_storageCountMap),
	m_equipOnMap(data.m_equipOnMap),
	//m_quickList(data.m_quickList),
	//m_quickKeyVec(data.m_quickKeyVec),
	//m_itemCountMap(data.m_itemCountMap),
	//m_itemDropMap(data.m_itemDropMap),
	//m_saleItemMap(data.m_saleItemMap),
	m_sightRange(data.m_sightRange),
	m_guardRange(data.m_guardRange),
	m_eyeHeight(data.m_eyeHeight),
	m_hp(data.m_hp),
	m_mp(data.m_mp),
	m_attack(data.m_attack),
	m_defend(data.m_defend),
	m_magicDefend(data.m_magicDefend),
	m_speed(data.m_speed),
	m_stealth(data.m_stealth),
	m_stealthspy(data.m_stealthspy),
	m_hitratio(data.m_hitratio),
	m_attackSpeed(data.m_attackSpeed),
	m_skillSpeed(data.m_skillSpeed),
	m_armourType(data.m_armourType),
	m_patrolPointVec(data.m_patrolPointVec),
	m_camp(data.m_camp),
	m_currentAttackID(data.m_currentAttackID),
	m_aboutToUseItemID(0),
	m_followDistance(data.m_followDistance),
	m_fightmode(data.m_fightmode),
	m_targetType(0),
	m_targetID(0),
	m_targetRoleID(0),
	m_itemState(0),
	m_guiseState(data.m_guiseState),
	m_patrolIndex(0),
	m_patrolLoopMode(data.m_patrolLoopMode),
	//m_baseEnmity(data.m_baseEnmity),
	m_extrahp(0),
	m_extrahp_rd(0),
	m_extramp(0),
	m_extramp_rd(0),
	m_extraAttack(0),
	m_extraAttack_rd(0),
	m_extraDefend(0),
	m_extraDefend_rd(0),
	//m_extraSpeed(0),
	//m_extraSpeed_rd(0),
	m_extraStealth(0),
	m_extraStealth_rd(0),
	m_extraStealthSpy(0),
	m_extraStealthSpy_rd(0),
	m_extraHitratioPercent(0),
	m_extraHitratioPercent_rd(0),
	m_extraAttackSpeed(0),
	m_extraAttackSpeed_rd(0),
	m_extraSkillSpeed(0),
	m_extraSkillSpeed_rd(0),
	m_extraGuise(GS_Normal),
	m_extraGuise_rd(GS_Normal),
	m_extraRadius(100),
	m_extraRadius_rd(100),
	m_extraIgnoreDefend(0),
	m_extraIgnoreDefend_rd(0),
	m_extraIgnoreMagicDefend(0),
	m_extraIgnoreMagicDefend_rd(0),
	m_extraSightRange(0),
	m_extraSightRange_rd(0),
	//m_currentUseItemProtectTime(0),
	m_currentActState(0),
	m_currentActDuration(0.0f),
	m_actStateMap(data.m_actStateMap),
	m_halfPhysicsBox(data.m_halfPhysicsBox),
	m_crit(data.m_crit),
	m_parry(data.m_parry),
	m_dodge(data.m_dodge),
	m_extraCrit(0),
	m_extraCrit_rd(0),
	m_extraParry(0),
	m_extraParry_rd(0),
	m_extraDodge(0),
	m_extraDodge_rd(0),
	m_isCrit(false),
	m_isParry(false),
	m_isDodge(false),
	m_isMiss(false),
	m_extraAttackPercent(0),
	m_extraAttackPercent_rd(0),
	m_extraSpeedPercent(0),
	m_extraSpeedPercent_rd(0),
	m_extraDefendPercent(0),
	m_extraDefendPercent_rd(0),
	m_extraMagicDefendPercent(0),
	m_extraMagicDefendPercent_rd(0),
	m_extraBuildAttackPercent(0),
	m_extraBuildAttackPercent_rd(0),
	m_extraCritPercent(0),
	m_extraCritPercent_rd(0),
	m_extraParryPercent(0),
	m_extraParryPercent_rd(0),
	m_extraDodgePercent(0),
	m_extraDodgePercent_rd(0),
	m_extraAttackDist(0),
	m_extraAttackDist_rd(0),
	m_extraAttackAbsorbHP(0),
	m_extraAttackAbsorbHP_rd(0),
	m_extraSkillAbsorbHP(0),
	m_extraSkillAbsorbHP_rd(0),
	m_extraAttackDamageBack(0),
	m_extraAttackDamageBack_rd(0),
	m_extraSkillDamageBack(0),
	m_extraSkillDamageBack_rd(0),
	m_extraRTHP(0.0f),
	m_extraRTHP_rd(0.0f),
	m_inbornHPPercent(data.m_inbornHPPercent),
	m_inbornMPPercent(data.m_inbornMPPercent),
	m_inbornAttackPercent(data.m_inbornAttackPercent),
	m_inbornDefendPercent(data.m_inbornDefendPercent),
	m_inbornMagicDefendPercent(data.m_inbornMagicDefendPercent),
	m_inbornBuildAttackPercent(data.m_inbornBuildAttackPercent),
	m_inbornSpeed(data.m_inbornSpeed),
	m_inbornAttackSpeedPercent(data.m_inbornAttackSpeedPercent),
	m_inbornSkillSpeedPercent(data.m_inbornSkillSpeedPercent),
	m_inbornSightRange(data.m_inbornSightRange),
	m_inbornAttackDist(data.m_inbornAttackDist),
	m_inbornStealth(data.m_inbornStealth),
	m_inbornStealthSpy(data.m_inbornStealthSpy),
	m_inbornCrit(data.m_inbornCrit),
	m_inbornParry(data.m_inbornParry),
	m_inbornDodge(data.m_inbornDodge),
	m_inbornIgnoreDefend(data.m_inbornIgnoreDefend),
	m_inbornIgnoreMagicDefend(data.m_inbornIgnoreMagicDefend),
	m_inbornIgnoreDodge(data.m_inbornIgnoreDodge),
	m_inbornAttackDamageBack(data.m_inbornAttackDamageBack),
	m_inbornSkillDamageBack(data.m_inbornSkillDamageBack),
	m_inbornAttackAbsorbHP(data.m_inbornAttackAbsorbHP),
	m_inbornSkillAbsorbHP(data.m_inbornSkillAbsorbHP),
	m_inbornCritPower(data.m_inbornCritPower),
	m_inbornSputtering(data.m_inbornSputtering),
	m_inbornAttackDamage(data.m_inbornAttackDamage),
	m_inbornSkillDamage(data.m_inbornSkillDamage),
	m_extraAttackDamage(0),
	m_extraAttackDamage_rd(0),
	m_extraSkillDamage(0),
	m_extraSkillDamage_rd(0),
	m_extraIgnoreDodge(0),
	m_extraIgnoreDodge_rd(0),
	m_extraCritPower(0),
	m_extraCritPower_rd(0),
	m_extraSputtering(0),
	m_extraSputtering_rd(0),
	m_inbornHitratioPercent(data.m_inbornHitratioPercent)
{
	m_rthp = m_hp;
	m_rtmp = m_mp;
}
void crRoleData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_SightRange:
		{
			float range = atof(relStr.c_str());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_sightRange = (short)range;
		}
		break;
	case WCHDATA_GuardRange:
		{
			float range = atof(relStr.c_str());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_guardRange = (short)range;
		}
		break;
	case WCHDATA_EyeHeight:
		{
			float height = atof(relStr.c_str());
			height /= crGlobalHandle::gData()->gUnitScale();
			m_eyeHeight = (short)height;
		}
		break;
	case WCHDATA_HP:
		m_hp = atoi(relStr.c_str());
		break;
	case WCHDATA_MP:
		m_mp = atoi(relStr.c_str());
		break;
	case WCHDATA_Attack:
		m_attack = atoi(relStr.c_str());
		break;
	case WCHDATA_Defend:
		m_defend = atoi(relStr.c_str());
		break;
	case WCHDATA_MagicDefend:
		m_magicDefend = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Speed:
		{
			float speed = atof(relStr.c_str());
			speed /= crGlobalHandle::gData()->gUnitScale();
			m_speed = (short)speed;
		}
		break;
	case WCHDATA_Stealth:
		{
			float stealth = atof(relStr.c_str());
			stealth /= crGlobalHandle::gData()->gUnitScale();
			m_stealth = (short)stealth;
		}
		break;
	case WCHDATA_StealthSpy:
		{
			float stealthspy = atof(relStr.c_str());
			stealthspy /= crGlobalHandle::gData()->gUnitScale();
			m_stealthspy = (short)stealthspy;
		}
		break;
	case WCHDATA_Hitratio:
		m_hitratio = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_AttackSpeed:
		m_attackSpeed = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_SkillSpeed:
		m_skillSpeed = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ArmourType:
		m_armourType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxAttack:
		m_maxattack = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxSkill:
		m_maxskill = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxBackPack:
		m_maxbackpack = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxStorage:
		m_maxstorage = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxEquipon:
		m_maxequipon = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MaxQuick:
		m_maxquick = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_AttackMap:
		{
			crVector2i vec2;
			CRCore::crArgumentParser::appAtoVec(relStr,vec2);
			if(vec2[0]>0)
			{
				m_attackMap[vec2[0]] = vec2[1];
			}
		}
		break;
	case WCHDATA_SkillMap:
		{
			crVector2i vec2;
			CRCore::crArgumentParser::appAtoVec(relStr,vec2);
			if(vec2[0]>0)
			{
				m_skillMap[vec2[0]] = vec2[1];
			}
		}
		break;
	//case WCHDATA_BackPackMap:
	//	{
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{
	//			m_backPackMap.insert(std::make_pair(vec2[0],vec2[1]));
	//		}
	//	}
	//	break;
	//case WCHDATA_ItemDropMap:
	//	{
	//		crVector2 vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{
	//			m_itemDropMap[(int)(vec2[0])] = vec2[1];
	//		}
	//	}
	//	break;
	//case WCHDATA_SaleItemMap:
	//	{//abstractid count
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{//系统售价0表示价格从itemdata里取
	//			m_saleItemMap.insert(std::make_pair(vec2[0],crVector3i(0,vec2[1],0)));
	//		}
	//	}
	//	break;
	case WCHDATA_PatrolPointVec:
		{
			crVector2 vec2;
			CRCore::crArgumentParser::appAtoVec(relStr,vec2);
			vec2 *= crGlobalHandle::gData()->gUnitScale();
			m_patrolPointVec.push_back(vec2);
		}
		break;
	case WCHDATA_Camp:
		m_camp = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ActStateMap:
		{
			std::string id,act;
			std::string::size_type icomma = relStr.find_first_of(',');
			id = std::string(relStr.begin(),relStr.begin()+icomma);
			act = std::string(relStr.begin()+icomma + 1,relStr.end());
			unsigned short _id = (unsigned short)(atoi(id.c_str()));
			m_actStateMap[_id] = act;
		}
		break;
	case WCHDATA_HalfPhysicsBox:
		crArgumentParser::appAtoVec(relStr,m_halfPhysicsBox);
		break;
	case WCHDATA_GuiseState:
		m_guiseState = (unsigned int)(atoi(relStr.c_str()));
		break;
	case WCHDATA_PatrolLoopMode:
		m_patrolLoopMode = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_BaseEnmity:
	//	m_baseEnmity = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	case WCHDATA_Crit:
		m_crit = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Parry:
		m_parry = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Dodge:
		m_dodge = (short)(atoi(relStr.c_str()));
		break;
	}
}
void crRoleData::serverCheckData(crInstanceItem *item, _crInt64 completeMsg/*int& count*/)
{
	std::set<int> TemporaryAbstractIDSet;
	std::set<int> AbstractIDSet;
	//item->getCreateItemChildLock();
	int count = 0;
	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
	for( AttackMap::iterator itr = m_attackMap.begin();
		itr != m_attackMap.end();
		++itr )
	{
		//checkItemChild(netConductor,item,itr->first,itr->second,count);
		checkItemChildMultiMap(netConductor,item,itr->first,itr->second,TemporaryAbstractIDSet,count);
	}
	for( SkillMap::iterator itr = m_skillMap.begin();
		itr != m_skillMap.end();
		++itr )
	{
		checkItemChild(netConductor,item,itr->first,itr->second,AbstractIDSet,count);
	}
	//for( BackPackMap::iterator itr = m_backPackMap.begin();
	//	itr != m_backPackMap.end();
	//	++itr )
	//{
	//	checkItemChildMultiMap(netConductor,item,itr->first,itr->second,TemporaryAbstractIDSet,count);
	//}
	//for( StorageMap::iterator itr = m_storageMap.begin();
	//	itr != m_storageMap.end();
	//	++itr )
	//{
	//	checkItemChildMultiMap(netConductor,item,itr->first,itr->second,TemporaryAbstractIDSet,count);
	//}
	for( EquipOnMap::iterator itr = m_equipOnMap.begin();
		itr != m_equipOnMap.end();
		++itr )
	{
		checkItemChildMultiMap(netConductor,item,itr->first,itr->second,TemporaryAbstractIDSet,count);
	}
	//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
	//	itr != m_saleItemMap.end();
	//	++itr )
	//{
	//	checkItemChildMultiMap(netConductor,item,itr->first,itr->second[0],TemporaryAbstractIDSet,count);
	//}
	item->beginCreateItemChild(completeMsg,count);
	if(count == 0)
	{
		item->completeCreateItemChild();
	}
	else if(count>0)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
		for( std::set<int>::iterator itr = TemporaryAbstractIDSet.begin();
			itr != TemporaryAbstractIDSet.end();
			++itr )
		{
			crCreateTemporaryItemChildPacket packet;
			crCreateTemporaryItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),*itr);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		for( std::set<int>::iterator itr = AbstractIDSet.begin();
			itr != AbstractIDSet.end();
			++itr )
		{
			crCreateItemChildPacket packet;
			crCreateItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),*itr);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	//crItemChild *itemChild;
	//for( QuickList::iterator itr = m_quickList.begin();
	//	itr != m_quickList.end();
	//	++itr )
	//{//QuickList只是快捷索引
	//	if(itr->second <= 0)
	//	{
	//		itemChild = item->findChildItemByAbstractID(itr->first);
	//	}
	//	else
	//	{
	//		itemChild = item->findChildItem(itr->second);
	//	}
	//	if(itemChild)
	//	{
	//		itr->second = itemChild->getInstanceItemID();
	//	}
	//	else if(itr->second<0)
	//	{
	//		itr->second = 0;
	//	}
	//	//if(itr->second<=0)
	//	//{
	//	//	itr->second = 0/*INT_MIN*/;
	//	//}
	//	//else
	//	//{
	//	//	itemChild = item->findChildItem(itr->second);
	//	//	if(!itemChild)
	//	//	{
	//	//		itr->second = 0;
	//	//	}
	//	//}
	//}
}
void crRoleData::checkItemChild(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,std::set<int>&AbstractIDSet,int &count)
{
	crItemChild *itemChild;
	if(second == 0)
	{
		itemChild = item->findChildItemByAbstractID(first);
		if(itemChild)
		{
			second = itemChild->getInstanceItemID();
		}
	}
	else
	{
		itemChild = item->findChildItem(second);
	}
	if(!itemChild)
	{
		count++;
		second = 0;
		////AbstractIDSet.insert(first);
		//crCreateItemChildPacket packet;
		//crCreateItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),first);
		//netConductor->getNetManager()->sendPacket("all",packet);
		////if(!itemChild->isLoaded())
		////	crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,role));
	}
}
void crRoleData::checkItemChildMultiMap(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,std::set<int>&TemporaryAbstractIDSet,int &count)
{
	std::set<int>::iterator taitr;
	crItemChild *itemChild = NULL;
	if(second<=0)
	{
		second = 0/*INT_MIN*/;
		taitr = TemporaryAbstractIDSet.find(first);
		if(taitr == TemporaryAbstractIDSet.end())
		{
			count++;
			TemporaryAbstractIDSet.insert(first);

			//crCreateTemporaryItemChildPacket packet;
			//crCreateTemporaryItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),first);
			//netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else
	{
		itemChild = item->findChildItem(second);
		if(!itemChild)
		{
			second = 0/*INT_MIN*/;
			taitr = TemporaryAbstractIDSet.find(first);
			if(taitr == TemporaryAbstractIDSet.end())
			{
				count++;
				TemporaryAbstractIDSet.insert(first);

				//crCreateTemporaryItemChildPacket packet;
				//crCreateTemporaryItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),first);
				//netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}
void crRoleData::clientCheckData(crInstanceItem *item)
{
	std::set<int> TemporaryItemIDSet;
	for( AttackMap::iterator itr = m_attackMap.begin();
		itr != m_attackMap.end();
		++itr )
	{
		clientCheckItemChild(item,itr->first,itr->second);
	}
	for( SkillMap::iterator itr = m_skillMap.begin();
		itr != m_skillMap.end();
		++itr )
	{
		clientCheckItemChild(item,itr->first,itr->second);
	}
	//for( BackPackMap::iterator itr = m_backPackMap.begin();
	//	itr != m_backPackMap.end();
	//	++itr )
	//{
	//	clientCheckItemChildMultiMap(item,itr->first,itr->second,TemporaryItemIDSet);
	//}
	//for( StorageMap::iterator itr = m_storageMap.begin();
	//	itr != m_storageMap.end();
	//	++itr )
	//{
	//	clientCheckItemChildMultiMap(item,itr->first,itr->second,TemporaryItemIDSet);
	//}
	for( EquipOnMap::iterator itr = m_equipOnMap.begin();
		itr != m_equipOnMap.end();
		++itr )
	{
		clientCheckItemChildMultiMap(item,itr->first,itr->second,TemporaryItemIDSet);
	}
	//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
	//	itr != m_saleItemMap.end();
	//	++itr )
	//{
	//	clientCheckItemChildMultiMap(item,itr->first,itr->second[0],TemporaryItemIDSet);
	//}
	//for( QuickList::iterator itr = m_quickList.begin();
	//	itr != m_quickList.end();
	//	++itr )
	//{
	//	clientCheckItemChildMultiMap(item,itr->first,itr->second,TemporaryItemIDSet);
	//}
}
void crRoleData::clientCheckItemChild(crInstanceItem *item,int first,int &second)
{
	crItemChild *itemChild;
	if(second != 0)
	{
		itemChild = item->findChildItem(second);
		if(itemChild)
		{//check And download itemchild
			if(!itemChild->isLoaded())
			{
				crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
				//crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(NULL,NULL));//清理
			}
		}
		else
		{
			crRequestItemChildPacket packet;
			crRequestItemChildPacket::buildRequestPacket(packet,item,second);
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else
	{
		itemChild = item->findChildItemByAbstractID(first);
		if(itemChild)
		{
			second = itemChild->getInstanceItemID();
		}
		else
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRpgGameRoleData clientCheckItemChild ID没初始化 "<<first<<std::endl;
			//sprintf(gDebugInfo->buf(),"crRpgGameRoleData clientCheckItemChild ID没初始化 %d\n\0",first);
			//gDebugInfo->debugInfo(CRCore::ALWAYS);
		}
	}
}
void crRoleData::clientCheckItemChildMultiMap(crInstanceItem *item,int first,int &second,std::set<int>&TemporaryItemIDSet)
{
	crItemChild *itemChild = NULL;
	if(second!=0)
	{
		std::set<int>::iterator taitr;
		crItemChild *itemChild;
		taitr = TemporaryItemIDSet.find(second);
		if(taitr == TemporaryItemIDSet.end())
		{
			itemChild = item->findChildItem(second);
			if(itemChild)
			{//check And download itemchild
				if(!itemChild->isLoaded())
				{
					TemporaryItemIDSet.insert(second);
					crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
					//crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(NULL,NULL));//清理
				}
			}
			else
			{
				TemporaryItemIDSet.insert(second);
				crRequestItemChildPacket packet;
				crRequestItemChildPacket::buildRequestPacket(packet,item,second);
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else
	{
		itemChild = item->findChildItemByAbstractID(first);
		if(itemChild)
		{
			second = itemChild->getInstanceItemID();
		}
		else
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRoleData clientCheckItemChildMultiMap ID没初始化 "<<first<<std::endl;
			//sprintf(gDebugInfo->buf(),"crRoleData clientCheckItemChildMultiMap ID没初始化 %d\n\0",first);
			//gDebugInfo->debugInfo(CRCore::ALWAYS);
		}
	}
}
void crRoleData::gameServerCheckData(crInstanceItem *item)
{
	int gamedbid = 0;
	if(dynamic_cast<crRole *>(item))
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData((dynamic_cast<crRole *>(item))->getPlayerID()));
		if(playerData.valid()) gamedbid = playerData->getGameDBID();
	}
	for( AttackMap::iterator itr = m_attackMap.begin();
		itr != m_attackMap.end();
		++itr )
	{
		gameServerCheckItemChild(item,itr->first,itr->second,gamedbid);
	}
	for( SkillMap::iterator itr = m_skillMap.begin();
		itr != m_skillMap.end();
		++itr )
	{
		gameServerCheckItemChild(item,itr->first,itr->second,gamedbid);
	}

	//for( BackPackMap::iterator itr = m_backPackMap.begin();
	//	itr != m_backPackMap.end();
	//	++itr )
	//{
	//	gameServerCheckItemChildMultiMap(item,itr->first,itr->second);
	//}
	//for( StorageMap::iterator itr = m_storageMap.begin();
	//	itr != m_storageMap.end();
	//	++itr )
	//{
	//	gameServerCheckItemChildMultiMap(item,itr->first,itr->second);
	//}
	for( EquipOnMap::iterator itr = m_equipOnMap.begin();
		itr != m_equipOnMap.end();
		++itr )
	{
		gameServerCheckItemChildMultiMap(item,itr->first,itr->second);
	}
	//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
	//	itr != m_saleItemMap.end();
	//	++itr )
	//{
	//	gameServerCheckItemChildMultiMap(item,itr->first,itr->second[0]);
	//}
	//crItemChild *itemChild;
	//for( QuickList::iterator itr = m_quickList.begin();
	//	itr != m_quickList.end();
	//	++itr )
	//{//QuickList只是快捷索引
	//	if(itr->second <= 0)
	//	{
	//		itemChild = item->findChildItemByAbstractID(itr->first);
	//	}
	//	else
	//	{
	//		itemChild = item->findChildItem(itr->second);
	//	}
	//	if(itemChild)
	//	{
	//		itr->second = itemChild->getInstanceItemID();
	//	}
	//	else
	//	{
	//		CRCore::notify(CRCore::ALWAYS)<<"crRoleData::gameServerCheckData itemchild 没有找到 abstractid = "<<itr->first<<" itemid = "<<itr->second<<std::endl;
	//		//sprintf(gDebugInfo->buf(),"crRoleData::gameServerCheckData itemchild 没有找到 abstractid = %d,itemid =%d\n\0",itr->first,itr->second);
	//		//gDebugInfo->debugInfo(CRCore::ALWAYS);
	//	}
	//}
}
void crRoleData::gameServerCheckItemChild(crInstanceItem *item,int first,int &second,unsigned short gamedbid)
{
	crItemChild *itemChild;
	if(second == 0)
	{
		itemChild = item->findChildItemByAbstractID(first);
		if(itemChild)
		{
			second = itemChild->getInstanceItemID();
		}
	}
	else
	{
		itemChild = item->findChildItem(second);
	}
	if(!itemChild)
	{
		crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
		ref_ptr<crDataBase> gameSession = gamedb->beginSession();
		bool game_ended = false;
		ref_ptr<crCreateInstanceItem> createInstanceItem = new crCreateInstanceItem;
		createInstanceItem->buildUpdateSql(first,crInstanceItem::instanceitem);
		if(gameSession->executeUpdate(createInstanceItem.get()))
		{//插入成功
			int instanceitemid = gameSession->getInsertID();
			ref_ptr<crCreateItemChild> createItemChild = new crCreateItemChild;
			createItemChild->buildUpdateSql(instanceitemid,item->getInstanceItemID());
			if(gameSession->executeUpdate(createItemChild.get()))
			{
				int itemchildid = gameSession->getInsertID();
				gameSession->commit();

				gamedb->endSession(gameSession.get());
				game_ended = true;

				ref_ptr<crItemChild> itemChild = new crItemChild;
				itemChild->setItemChildID(itemchildid);
				itemChild->setInstanceItemID(instanceitemid);
				itemChild->setParentItem(item);
				crInstanceItem *childItem = new crInstanceItem;
				childItem->setInstanceItemID(instanceitemid);
				childItem->setAbstractItem(crGlobalHandle::getInstance()->gameServerQueryAbstractItem(first));
				itemChild->setInstanceItem(childItem);

				item->insertChildItem(itemChild.get());
				second = instanceitemid;
			}
			else
			{
				gameSession->rollback();
			}
		}
		if(!game_ended) gamedb->endSession(gameSession.get());
	}
}
void crRoleData::gameServerCheckItemChildMultiMap(crInstanceItem *item,int first,int &second)
{
	crItemChild *itemChild = NULL;
	if(second<=0)
	{
		itemChild = item->findChildItemByAbstractID(first);
		if(itemChild)
		{
			second = itemChild->getInstanceItemID();
		}
	}
	else
	{
		itemChild = item->findChildItem(second);
	}
	if(!itemChild)
	{
		crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(first);
		crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServerConductor->getNetDataManager()->getNetCallback());
		//crScene *mainScene = callback->getMainScene();
		int itemchildid = crGlobalHandle::gainTemporaryItemChildID();
		int childitemid = crGlobalHandle::gainTemporaryItemID();
		ref_ptr<crItemChild> itemChild = new crItemChild;
		itemChild->setItemChildID(itemchildid);
		itemChild->setInstanceItemID(childitemid);
		itemChild->setParentItem(item);
		crInstanceItem *childItem = new crInstanceItem;
		childItem->setInstanceItemID(childitemid);
		childItem->setAbstractItem(aitem);
		itemChild->setInstanceItem(childItem);

		item->insertChildItem(itemChild.get());
		second = childitemid;
	}
}
void crRoleData::excHandle(_crInt64 msg)
{
	crData::excHandle(getHandle(msg)); 
}
void crRoleData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_MaxAttack:
		m_maxattack = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_MaxSkill:
		m_maxskill = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_MaxBackPack:
		m_maxbackpack = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_MaxStorage:
		m_maxstorage = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_MaxEquipon:
		m_maxequipon = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_MaxQuick:
		m_maxquick = param==NULL?NULL:*((unsigned char*)param);
		break;
	//case WCHDATA_ExchangePlayerSet:
	//	if(param)
	//	{
	//		int playerid = *(int*)param;
	//		m_exchangePlayerSet.insert(playerid);
	//	}
	//	break;
	case WCHDATA_RTHP:
		m_rthp = param==NULL?NULL:*((float*)param);
		break;
	case WCHDATA_RTMP:
		m_rtmp = param==NULL?NULL:*((float*)param);
		break;
	case WCHDATA_SightRange:
		m_sightRange = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_GuardRange:
		m_guardRange = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_EyeHeight:
		m_eyeHeight = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_HP:
		m_hp = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_MP:
		m_mp = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_Attack:
		m_attack = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_Defend:
		m_defend = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_MagicDefend:
		m_magicDefend = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Speed:
		m_speed = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Stealth:
		m_stealth = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_StealthSpy:
		m_stealthspy = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Hitratio:
		m_hitratio = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_AttackSpeed:
		m_attackSpeed = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_SkillSpeed:
		m_skillSpeed = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_ArmourType:
		m_armourType = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_ItemState:
		{
			//m_itemState = param==NULL?NULL:*((unsigned char*)param);
			unsigned char newstate = param==NULL?NULL:*((unsigned char*)param);
			if(m_itemState == IS_Dead)
			{
				if(m_rthp>0.0f)
				{
					m_itemState = newstate;
				}
			}
			else
			{
				m_itemState = newstate;
			}
		}
		//m_itemState = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_GuiseState:
		{
			unsigned int newstate = param==NULL?NULL:*((unsigned int*)param);
			if(m_guiseState & GS_Static || m_guiseState & GS_StaticNoneBlock || m_guiseState & GS_StaticUnVisiable)
			{
				if(newstate & GS_Static || newstate & GS_StaticNoneBlock || newstate & GS_StaticUnVisiable)
					m_guiseState = newstate;
			}
			else
			{
				m_guiseState = newstate;
			}
		}
		break;
	case WCHDATA_TargetType:
		m_targetType = param==NULL?NULL:*((unsigned char*)param);
		break;
	//case WCHDATA_TargetNode:
	//	m_targetNode = param==NULL?NULL:(crNode*)param;
	//	break;
	case WCHDATA_TargetID:
		m_targetID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_TargetRoleID:
		m_targetRoleID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_TargetPos:
		m_targetPosition = param==NULL?m_targetPosition:*((crVector3*)param);
		break;
	case WCHDATA_MoveToPos:
		m_moveToPosition = param==NULL?m_moveToPosition:*((crVector3*)param);
		break;
	case WCHDATA_CurrentAttackID:
		m_currentAttackID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_AboutToUseItemID:
		m_aboutToUseItemID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_FollowDistance:
		m_followDistance = param==NULL?crGlobalHandle::gData()->gMinFollowDistance():*((float*)param);
		break;
	case WCHDATA_FightMode:
		m_fightmode = param==NULL?FM_Peace:*((unsigned char*)param);
		break;
	case WCHDATA_PatrolPointVec:
		if(param)
		{
			GNE::LockMutex lock( m_dataMutex );
			crVector2 vec2 = *((crVector2*)param);
			m_patrolPointVec.push_back(vec2);
		}
		break;
	case WCHDATA_PatrolIndex:
		m_patrolIndex = param==NULL?NULL:*((char*)param);
		break;
	case WCHDATA_PatrolLoopMode:
		m_patrolLoopMode = param==NULL?NULL:*((unsigned char*)param);
		break;
	//case WCHDATA_BaseEnmity:
	//	m_baseEnmity = param==NULL?NULL:*((int*)param);
	//	break;
	case WCHDATA_ExtraHP:
		m_extrahp = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraMP:
		m_extramp = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraAttack:
		m_extraAttack = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraDefend:
		m_extraDefend = param==NULL?0:*((short*)param);
		break;
	//case WCHDATA_ExtraSpeed:
	//	m_extraSpeed = param==NULL?0:*((short*)param);
	//	break;
	case WCHDATA_ExtraStealth:
		m_extraStealth = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraStealthSpy:
		m_extraStealthSpy = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraHitratioPercent:
		m_extraHitratioPercent = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraAttackSpeed:
		m_extraAttackSpeed = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraSkillSpeed:
		m_extraSkillSpeed = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraGuise:
		m_extraGuise = param==NULL?0:*((unsigned int*)param);
		break;
	case WCHDATA_ExtraRadius:
		m_extraRadius = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraIgnoreDefend:
		m_extraIgnoreDefend = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraIgnoreMagicDefend:
		m_extraIgnoreMagicDefend = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraSightRange:
		m_extraSightRange = param==NULL?0:*((short*)param);
		break;
	//case WCHDATA_CurUseItemProtectTime:
	//	m_currentUseItemProtectTime = param==NULL?NULL:*(float*)param;
	//	break;
	case WCHDATA_CurActState:
		m_currentActState = param==NULL?NULL:*(unsigned short*)param;
		break;
	case WCHDATA_CurActDuration:
		m_currentActDuration = param==NULL?0.0f:*(float*)param;
		break;
	case WCHDATA_ItemUseParam:
		m_itemUseParam = param?(ItemUseParam*)param:NULL;
		break;
	case WCHDATA_AttackUseParam:
		m_attackUseParam = param?(ItemUseParam*)param:NULL;
		break;
	case WCHDATA_HalfPhysicsBox:
		m_halfPhysicsBox = param?*(crVector3*)param:crVector3(0.7f,0.7f,1.0f);
		break;
	case WCHDATA_Camp:
		m_camp = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_Crit:
		m_crit = param?*((short*)param):NULL;
		break;
	case WCHDATA_Parry:
		m_parry = param?*((short*)param):NULL;
		break;
	case WCHDATA_Dodge:
		m_dodge = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraCrit:
		m_extraCrit = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraParry:
		m_extraParry = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraDodge:
		m_extraDodge = param?*((short*)param):NULL;
		break;
	case WCHDATA_IsCrit:
		m_isCrit = param?*((bool*)param):NULL;
		break;
	case WCHDATA_IsParry:
		m_isParry = param?*((bool*)param):NULL;
		break;
	case WCHDATA_IsDodge:
		m_isDodge = param?*((bool*)param):NULL;
		break;
	case WCHDATA_IsMiss:
		m_isMiss = param?*((bool*)param):NULL;
		break;
	case WCHDATA_ExtraAttackPercent:
		m_extraAttackPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraSpeedPercent:
		m_extraSpeedPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraDefendPercent:
		m_extraDefendPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraMagicDefendPercent:
		m_extraMagicDefendPercent = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraBuildAttackPercent:
		m_extraBuildAttackPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraCritPercent:
		m_extraCritPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraParryPercent:
		m_extraParryPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraDodgePercent:
		m_extraDodgePercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraAttackDist:
		m_extraAttackDist = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraAttackAbsorbHP:
		m_extraAttackAbsorbHP = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraSkillAbsorbHP:
		m_extraSkillAbsorbHP = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraAttackDamageBack:
		m_extraAttackDamageBack = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraSkillDamageBack:
		m_extraSkillDamageBack = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraRTHP:
		m_extraRTHP = param?*((float*)param):NULL;
		break;
	case WCHDATA_InbornHPPercent:
		m_inbornHPPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornMPPercent:
		m_inbornMPPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornAttackPercent:
		m_inbornAttackPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornDefendPercent:
		m_inbornDefendPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornMagicDefendPercent:
		m_inbornMagicDefendPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornBuildAttackPercent:
		m_inbornBuildAttackPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSpeed:
		m_inbornSpeed = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornAttackSpeedPercent:
		m_inbornAttackSpeedPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSkillSpeedPercent:
		m_inbornSkillSpeedPercent = param?*((short*)param):NULL;
		break;
	//case WCHDATA_InbornAttackRangePercent:
	//	m_inbornAttackRangePercent = param?*((short*)param):NULL;
	//	break;
	case WCHDATA_InbornSightRange:
		m_inbornSightRange = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornAttackDist:
		m_inbornAttackDist = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornStealth:
		m_inbornStealth = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornStealthSpy:
		m_inbornStealthSpy = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornCrit:
		m_inbornCrit = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornParry:
		m_inbornParry = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornDodge:
		m_inbornDodge = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornIgnoreDefend:
		m_inbornIgnoreDefend = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornIgnoreMagicDefend:
		m_inbornIgnoreMagicDefend = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornIgnoreDodge:
		m_inbornIgnoreDodge = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_InbornAttackDamageBack:
		m_inbornAttackDamageBack = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSkillDamageBack:
		m_inbornSkillDamageBack = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornAttackAbsorbHP:
		m_inbornAttackAbsorbHP = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSkillAbsorbHP:
		m_inbornSkillAbsorbHP = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornCritPower:
		m_inbornCritPower = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSputtering:
		m_inbornSputtering = param?*((_crInt32*)param):NULL;
		break;
	case WCHDATA_InbornAttackDamage:
		m_inbornAttackDamage = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornSkillDamage:
		m_inbornSkillDamage = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraAttackDamage:
		m_extraAttackDamage = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraSkillDamage:
		m_extraSkillDamage = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraIgnoreDodge:
		m_extraIgnoreDodge = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_ExtraCritPower:
		m_extraCritPower = param?*((short*)param):NULL;
		break;
	case WCHDATA_InbornHitratioPercent:
		m_inbornHitratioPercent = param?*((short*)param):NULL;
		break;
	case WCHDATA_ExtraSputtering:
		m_extraSputtering = param?*((_crInt32*)param):NULL;
		break;
	}
}
void crRoleData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_MaxAttack:
		param = &m_maxattack;
		break;
	case WCHDATA_MaxSkill:
		param = &m_maxskill;
		break;
	case WCHDATA_MaxBackPack:
		param = &m_maxbackpack;
		break;
	case WCHDATA_MaxStorage:
		param = &m_maxstorage;
		break;
	case WCHDATA_MaxEquipon:
		param = &m_maxequipon;
		break;
	case WCHDATA_MaxQuick:
		param = &m_maxquick;
		break;
	case WCHDATA_AttackMap:
		param = &m_attackMap;
		break;
	case WCHDATA_SkillMap:
		param = &m_skillMap;
		break;
	//case WCHDATA_BackPackMap:
	//	param = &m_backPackMap;
	//	break;
	//case WCHDATA_StorageMap:
	//	param = &m_storageMap;
	//	break;
	//case WCHDATA_StorageCountMap:
	//	param = &m_storageCountMap;
	//	break;
	case WCHDATA_EquipOnMap:
		param = &m_equipOnMap;
		break;
	////case WCHDATA_QuickList:
	////	param = &m_quickList;
	////	break;
	////case WCHDATA_QuickKeyVec:
	////	param = &m_quickKeyVec;
	////	break;
	////case WCHDATA_ItemCountMap:
	////	param = &m_itemCountMap;
	////	break;
	////case WCHDATA_ItemDropMap:
	////	param = &m_itemDropMap;
	////	break;
	////case WCHDATA_SaleItemMap:
	////	param = &m_saleItemMap;
	////	break;
	////case WCHDATA_ExchangePlayerSet:
	////	param = &m_exchangePlayerSet;
	////	break;
	case WCHDATA_ExtraData:
		param = m_extraDataClass.get();
		break;
	case WCHDATA_SightRange:
		param = &m_sightRange;
		break;
	case WCHDATA_GuardRange:
		param = &m_guardRange;
		break;
	case WCHDATA_EyeHeight:
		param = &m_eyeHeight;
		break;
	case WCHDATA_HP:
		param = &m_hp;
		break;
	case WCHDATA_MP:
		param = &m_mp;
		break;
	case WCHDATA_Attack:
		param = &m_attack;
		break;
	case WCHDATA_Defend:
		param = &m_defend;
		break;
	case WCHDATA_MagicDefend:
		param = &m_magicDefend;
		break;
	case WCHDATA_Speed:
		param = &m_speed;
		break;
	case WCHDATA_Stealth:
		param = &m_stealth;
		break;
	case WCHDATA_StealthSpy:
		param = &m_stealthspy;
		break;
	case WCHDATA_Hitratio:
		param = &m_hitratio;
		break;
	case WCHDATA_AttackSpeed:
		param = &m_attackSpeed;
		break;
	case WCHDATA_SkillSpeed:
		param = &m_skillSpeed;
		break;
	case WCHDATA_ArmourType:
		param = &m_armourType;
		break;
	case WCHDATA_Camp:
		param = &m_camp;
		break;
	case WCHDATA_PatrolPointVec:
		param = &m_patrolPointVec;
		break;
	case WCHDATA_TargetType:
		param = &m_targetType;
		break;
	//case WCHDATA_TargetNode:
	//	param = m_targetNode.get();
	//	break;
	case WCHDATA_TargetID:
		param = &m_targetID;
		break;
	case WCHDATA_TargetRoleID:
		param = &m_targetRoleID;
		break;
	case WCHDATA_TargetPos:
		param = &m_targetPosition;
		break;
	case WCHDATA_MoveToPos:
		param = &m_moveToPosition;
		break;
	case WCHDATA_ItemState:
		param = &m_itemState;
		break;
	case WCHDATA_GuiseState:
		param = &m_guiseState;
		break;
	case WCHDATA_FollowDistance:
		if(m_followDistance<crGlobalHandle::gData()->gMinFollowDistance())
			m_followDistance = crGlobalHandle::gData()->gMinFollowDistance();
		param = &m_followDistance;
		break;
	case WCHDATA_RTHP:
		param = &m_rthp;
		break;
	case WCHDATA_RTMP:
		param = &m_rtmp;
		break;
	case WCHDATA_CurrentAttackID:
		//if(m_currentAttackID == 0 && !m_attackMap.empty())
		//	m_currentAttackID = m_attackMap.begin()->second;
		param = &m_currentAttackID;
		break;
	case WCHDATA_AboutToUseItemID:
		param = &m_aboutToUseItemID;
		break;
	case WCHDATA_FightMode:
		param = &m_fightmode;
		break;
	case WCHDATA_PatrolIndex:
		param = &m_patrolIndex;
		break;
	case WCHDATA_PatrolLoopMode:
		param = &m_patrolLoopMode;
		break;
	//case WCHDATA_BaseEnmity:
	//	param = &m_baseEnmity;
	//	break;
	case WCHDATA_EnmityMap:
		param = &m_enmityMap;
		break;
	case WCHDATA_InPatrolEnemyMap:
		param = &m_inpatrolEnemyMap;
		break;
	case WCHDATA_ExtraHP:
		param = &m_extrahp;
		break;
	case WCHDATA_ExtraHP_RD:
		param = &m_extrahp_rd;
		break;
	case WCHDATA_ExtraMP:
		param = &m_extramp;
		break;
	case WCHDATA_ExtraMP_RD:
		param = &m_extramp_rd;
		break;
	case WCHDATA_ExtraAttack:
		param = &m_extraAttack;
		break;
	case WCHDATA_ExtraAttack_RD:
		param = &m_extraAttack_rd;
		break;
	case WCHDATA_ExtraDefend:
		param = &m_extraDefend;
		break;
	case WCHDATA_ExtraDefend_RD:
		param = &m_extraDefend_rd;
		break;
	//case WCHDATA_ExtraSpeed:
	//	param = &m_extraSpeed;
	//	break;
	//case WCHDATA_ExtraSpeed_RD:
	//	param = &m_extraSpeed_rd;
	//	break;
	case WCHDATA_ExtraStealth:
		param = &m_extraStealth;
		break;
	case WCHDATA_ExtraStealth_RD:
		param = &m_extraStealth_rd;
		break;
	case WCHDATA_ExtraStealthSpy:
		param = &m_extraStealthSpy;
		break;
	case WCHDATA_ExtraStealthSpy_RD:
		param = &m_extraStealthSpy_rd;
		break;
	case WCHDATA_ExtraHitratioPercent:
		param = &m_extraHitratioPercent;
		break;
	case WCHDATA_ExtraHitratioPercent_RD:
		param = &m_extraHitratioPercent_rd;
		break;
	case WCHDATA_ExtraAttackSpeed:
		param = &m_extraAttackSpeed;
		break;
	case WCHDATA_ExtraAttackSpeed_RD:
		param = &m_extraAttackSpeed_rd;
		break;
	case WCHDATA_ExtraSkillSpeed:
		param = &m_extraSkillSpeed;
		break;
	case WCHDATA_ExtraSkillSpeed_RD:
		param = &m_extraSkillSpeed_rd;
		break;
	case WCHDATA_ExtraGuise:
		param = &m_extraGuise;
		break;
	case WCHDATA_ExtraGuise_RD:
		param = &m_extraGuise_rd;
		break;
	case WCHDATA_ExtraRadius:
		param = &m_extraRadius;
		break;
	case WCHDATA_ExtraRadius_RD:
		param = &m_extraRadius_rd;
		break;
	case WCHDATA_ExtraIgnoreDefend:
		param = &m_extraIgnoreDefend;
		break;
	case WCHDATA_ExtraIgnoreDefend_RD:
		param = &m_extraIgnoreDefend_rd;
		break;
	case WCHDATA_ExtraIgnoreMagicDefend:
		param = &m_extraIgnoreMagicDefend;
		break;
	case WCHDATA_ExtraIgnoreMagicDefend_RD:
		param = &m_extraIgnoreMagicDefend_rd;
		break;
	case WCHDATA_ExtraSightRange:
		param = &m_extraSightRange;
		break;
	case WCHDATA_ExtraSightRange_RD:
		param = &m_extraSightRange_rd;
		break;
	//case WCHDATA_CurUseItemProtectTime:
	//	param = &m_currentUseItemProtectTime;
	//	break;
	case WCHDATA_ActStateMap:
		param = &m_actStateMap;
		break;
	case WCHDATA_CurActState:
		param = &m_currentActState;
		break;
	case WCHDATA_CurActDuration:
		param = &m_currentActDuration;
		break;
	case WCHDATA_HalfPhysicsBox:
		param = &m_halfPhysicsBox;
		break;
	case WCHDATA_ItemUseParam:
		param = m_itemUseParam.get();
		break;
	case WCHDATA_AttackUseParam:
		param = m_attackUseParam.get();
		break;
	case WCHDATA_Crit:
		param = &m_crit;
		break;
	case WCHDATA_Parry:
		param = &m_parry;
		break;
	case WCHDATA_Dodge:
		param = &m_dodge;
		break;
	case WCHDATA_ExtraCrit:
		param = &m_extraCrit;
		break;
	case WCHDATA_ExtraCrit_RD:
		param = &m_extraCrit_rd;
		break;
	case WCHDATA_ExtraParry:
		param = &m_extraParry;
		break;
	case WCHDATA_ExtraParry_RD:
		param = &m_extraParry_rd;
		break;
	case WCHDATA_ExtraDodge:
		param = &m_extraDodge;
		break;
	case WCHDATA_ExtraDodge_RD:
		param = &m_extraDodge_rd;
		break;
	case WCHDATA_IsCrit:
		param = &m_isCrit;
		break;
	case WCHDATA_IsParry:
		param = &m_isParry;
		break;
	case WCHDATA_IsDodge:
		param = &m_isDodge;
		break;
	case WCHDATA_IsMiss:
		param = &m_isMiss;
		break;
	case WCHDATA_ExtraAttackPercent:
		param = &m_extraAttackPercent;
		break;
	case WCHDATA_ExtraAttackPercent_RD:
		param = &m_extraAttackPercent_rd;
		break;
	case WCHDATA_ExtraSpeedPercent:
		param = &m_extraSpeedPercent;
		break;
	case WCHDATA_ExtraSpeedPercent_RD:
		param = &m_extraSpeedPercent_rd;
		break;
	case WCHDATA_ExtraDefendPercent:
		param = &m_extraDefendPercent;
		break;
	case WCHDATA_ExtraDefendPercent_RD:
		param = &m_extraDefendPercent_rd;
		break;
	case WCHDATA_ExtraMagicDefendPercent:
		param = &m_extraMagicDefendPercent;
		break;
	case WCHDATA_ExtraMagicDefendPercent_RD:
		param = &m_extraMagicDefendPercent_rd;
		break;
	case WCHDATA_ExtraBuildAttackPercent:
		param = &m_extraBuildAttackPercent;
		break;
	case WCHDATA_ExtraBuildAttackPercent_RD:
		param = &m_extraBuildAttackPercent_rd;
		break;
	case WCHDATA_ExtraCritPercent:
		param = &m_extraCritPercent;
		break;
	case WCHDATA_ExtraCritPercent_RD:
		param = &m_extraCritPercent_rd;
		break;
	case WCHDATA_ExtraParryPercent:
		param = &m_extraParryPercent;
		break;
	case WCHDATA_ExtraParryPercent_RD:
		param = &m_extraParryPercent_rd;
		break;
	case WCHDATA_ExtraDodgePercent:
		param = &m_extraDodgePercent;
		break;
	case WCHDATA_ExtraDodgePercent_RD:
		param = &m_extraDodgePercent_rd;
		break;
	case WCHDATA_ExtraAttackDist:
		param = &m_extraAttackDist;
		break;
	case WCHDATA_ExtraAttackDist_RD:
		param = &m_extraAttackDist_rd;
		break;
	case WCHDATA_ExtraAttackAbsorbHP:
		param = &m_extraAttackAbsorbHP;
		break;
	case WCHDATA_ExtraAttackAbsorbHP_RD:
		param = &m_extraAttackAbsorbHP_rd;
		break;
	case WCHDATA_ExtraSkillAbsorbHP:
		param = &m_extraSkillAbsorbHP;
		break;
	case WCHDATA_ExtraSkillAbsorbHP_RD:
		param = &m_extraSkillAbsorbHP_rd;
		break;
	case WCHDATA_ExtraAttackDamageBack:
		param = &m_extraAttackDamageBack;
		break;
	case WCHDATA_ExtraAttackDamageBack_RD:
		param = &m_extraAttackDamageBack_rd;
		break;
	case WCHDATA_ExtraSkillDamageBack:
		param = &m_extraSkillDamageBack;
		break;
	case WCHDATA_ExtraSkillDamageBack_RD:
		param = &m_extraSkillDamageBack_rd;
		break;
	case WCHDATA_ExtraRTHP:
		param = &m_extraRTHP;
		break;
	case WCHDATA_ExtraRTHP_RD:
		param = &m_extraRTHP_rd;
		break;
	case WCHDATA_InbornHPPercent:
		param = &m_inbornHPPercent;
		break;
	case WCHDATA_InbornMPPercent:
		param = &m_inbornMPPercent;
		break;
	case WCHDATA_InbornAttackPercent:
		param = &m_inbornAttackPercent;
		break;
	case WCHDATA_InbornDefendPercent:
		param = &m_inbornDefendPercent;
		break;
	case WCHDATA_InbornMagicDefendPercent:
		param = &m_inbornMagicDefendPercent;
		break;
	case WCHDATA_InbornBuildAttackPercent:
		param = &m_inbornBuildAttackPercent;
		break;
	case WCHDATA_InbornSpeed:
		param = &m_inbornSpeed;
		break;
	case WCHDATA_InbornAttackSpeedPercent:
		param = &m_inbornAttackSpeedPercent;
		break;
	case WCHDATA_InbornSkillSpeedPercent:
		param = &m_inbornSkillSpeedPercent;
		break;
	//case WCHDATA_InbornAttackRangePercent:
	//	param = &m_inbornAttackRangePercent;
	//	break;
	case WCHDATA_InbornSightRange:
		param = &m_inbornSightRange;
		break;
	case WCHDATA_InbornAttackDist:
		param = &m_inbornAttackDist;
		break;
	case WCHDATA_InbornStealth:
		param = &m_inbornStealth;
		break;
	case WCHDATA_InbornStealthSpy:
		param = &m_inbornStealthSpy;
		break;
	case WCHDATA_InbornCrit:
		param = &m_inbornCrit;
		break;
	case WCHDATA_InbornParry:
		param = &m_inbornParry;
		break;
	case WCHDATA_InbornDodge:
		param = &m_inbornDodge;
		break;
	case WCHDATA_InbornIgnoreDefend:
		param = &m_inbornIgnoreDefend;
		break;
	case WCHDATA_InbornIgnoreMagicDefend:
		param = &m_inbornIgnoreMagicDefend;
		break;
	case WCHDATA_InbornIgnoreDodge:
		param = &m_inbornIgnoreDodge;
		break;
	case WCHDATA_InbornAttackDamageBack:
		param = &m_inbornAttackDamageBack;
		break;
	case WCHDATA_InbornSkillDamageBack:
		param = &m_inbornSkillDamageBack;
		break;
	case WCHDATA_InbornAttackAbsorbHP:
		param = &m_inbornAttackAbsorbHP;
		break;
	case WCHDATA_InbornSkillAbsorbHP:
		param = &m_inbornSkillAbsorbHP;
		break;
	case WCHDATA_InbornCritPower:
		param = &m_inbornCritPower;
		break;
	case WCHDATA_InbornSputtering:
		param = &m_inbornSputtering;
		break;
	case WCHDATA_InbornAttackDamage:
		param = &m_inbornAttackDamage;
		break;
	case WCHDATA_InbornSkillDamage:
		param = &m_inbornSkillDamage;
		break;
	case WCHDATA_ExtraAttackDamage:
		param = &m_extraAttackDamage;
		break;
	case WCHDATA_ExtraAttackDamage_RD:
		param = &m_extraAttackDamage_rd;
		break;
	case WCHDATA_ExtraSkillDamage:
		param = &m_extraSkillDamage;
		break;
	case WCHDATA_ExtraSkillDamage_RD:
		param = &m_extraSkillDamage_rd;
		break;
	case WCHDATA_ExtraIgnoreDodge:
		param = &m_extraIgnoreDodge;
		break;
	case WCHDATA_ExtraIgnoreDodge_RD:
		param = &m_extraIgnoreDodge_rd;
		break;
	case WCHDATA_ExtraCritPower:
		param = &m_extraCritPower;
		break;
	case WCHDATA_ExtraCritPower_RD:
		param = &m_extraCritPower_rd;
		break;
	case WCHDATA_InbornHitratioPercent:
		param = &m_inbornHitratioPercent;
		break;
	case WCHDATA_ExtraSputtering:
		param = &m_extraSputtering;
		break;
	case WCHDATA_ExtraSputtering_RD:
		param = &m_extraSputtering_rd;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameRoleData
//
/////////////////////////////////////////
crRpgGameRoleData::crRpgGameRoleData():
m_currentSpeed(0),
m_acceleration(500),
m_damp(100),
m_currentAngular(0),
m_angularAccel(0),
m_angularDamp(0),
m_maxAngular(0),
m_level(0),
m_experience(0),
m_gold(0),
m_strength(0),
m_agility(0),
m_intellect(0),
//m_maxattack(5),
//m_maxskill(30),
//m_maxbackpack(50),
//m_maxstorage(50),
//m_maxequipon(6),
//m_maxquick(18),
m_expToKiller(0),
m_goldToKiller(0),
m_attrPoint(0),
m_skillPoint(0),
m_controllerFlg(0),
m_kill(0),
m_dead(0),
m_controllerTime(0.0f)
{
}
crRpgGameRoleData::crRpgGameRoleData(const crRpgGameRoleData& data):
crRoleData(data),
m_currentSpeed(data.m_currentSpeed),
m_acceleration(data.m_acceleration),
m_damp(data.m_damp),
m_currentAngular(data.m_currentAngular),
m_angularAccel(data.m_angularAccel),
m_angularDamp(data.m_angularDamp),
m_maxAngular(data.m_maxAngular),
m_level(data.m_level),
m_experience(data.m_experience),
m_gold(data.m_gold),
m_strength(data.m_strength),
m_agility(data.m_agility),
m_intellect(data.m_intellect),
//m_maxattack(data.m_maxattack),
//m_maxskill(data.m_maxskill),
//m_maxbackpack(data.m_maxbackpack),
//m_maxstorage(data.m_maxstorage),
//m_maxequipon(data.m_maxequipon),
//m_maxquick(data.m_maxquick),
//m_attackMap(data.m_attackMap),
//m_skillMap(data.m_skillMap),
//m_backPackMap(data.m_backPackMap),
//m_storageMap(data.m_storageMap),
//m_storageCountMap(data.m_storageCountMap),
//m_equipOnMap(data.m_equipOnMap),
//m_quickList(data.m_quickList),
//m_quickKeyVec(data.m_quickKeyVec),
//m_itemCountMap(data.m_itemCountMap),
m_itemIcon(data.m_itemIcon),
m_expToKiller(data.m_expToKiller),
m_goldToKiller(data.m_goldToKiller),
//m_itemDropMap(data.m_itemDropMap),
//m_saleItemMap(data.m_saleItemMap),
m_attrPoint(data.m_attrPoint),
m_skillPoint(data.m_skillPoint),
m_groupMemberSet(data.m_groupMemberSet),
m_controllerFlg(0),
m_kill(0),
m_dead(0),
m_controllerTime(0.0f)
{
	//m_extraDataClass = new crRpgGameExtraData;
}
void crRpgGameRoleData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_Acceleration:
		{
			float acc = atof(relStr.c_str());
			acc /= crGlobalHandle::gData()->gUnitScale();
			m_acceleration = (short)acc;
		}
		break;
	case WCHDATA_Damp:
		{
			float damp = atof(relStr.c_str());
			damp /= crGlobalHandle::gData()->gUnitScale();
			m_damp = (short)damp;
		}
		break;
	case WCHDATA_AngularAccel:
		m_angularAccel= atoi(relStr.c_str());
		break;
	case WCHDATA_AngularDamp:
		m_angularDamp= atoi(relStr.c_str());
		break;
	case WCHDATA_MaxAngular:
		m_maxAngular = atoi(relStr.c_str());
		break;
	case WCHDATA_Level:
		m_level = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Experience:
		m_experience = atoi(relStr.c_str());
		break;
	case WCHDATA_Gold:
		m_gold = atoi(relStr.c_str());
		break;
	case WCHDATA_Strength:
		m_strength = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Agility:
		m_agility = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Intellect:
		m_intellect = (short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_MaxAttack:
	//	m_maxattack = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_MaxSkill:
	//	m_maxskill = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_MaxBackPack:
	//	m_maxbackpack = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_MaxStorage:
	//	m_maxstorage = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_MaxEquipon:
	//	m_maxequipon = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_MaxQuick:
	//	m_maxquick = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	case WCHDATA_ItemIcon:
		m_itemIcon = relStr;
		break;
	//case WCHDATA_AttackMap:
	//	{
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{
	//			m_attackMap[vec2[0]] = vec2[1];
	//		}
	//	}
	//	break;
	//case WCHDATA_SkillMap:
	//	{
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//	    if(vec2[0]>0)
	//		{
	//			m_skillMap[vec2[0]] = vec2[1];
	//		}
	//	}
	//	break;
	//case WCHDATA_BackPackMap:
	//	{
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{
	//			m_backPackMap.insert(std::make_pair(vec2[0],vec2[1]));
	//		}
	//	}
	//	break;
	case WCHDATA_ExpToKiller:
		m_expToKiller = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_GoldToKiller:
		m_goldToKiller = (short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_ItemDropMap:
	//	{
	//		crVector2 vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{
	//			m_itemDropMap[(int)(vec2[0])] = vec2[1];
	//		}
	//	}
	//	break;
	//case WCHDATA_SaleItemMap:
	//	{//abstractid count
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{//系统售价0表示价格从itemdata里取
	//			m_saleItemMap.insert(std::make_pair(vec2[0],crVector3i(0,vec2[1],0)));
	//		}
	//	}
	//	break;
	case WCHDATA_AttrPoint:
		m_attrPoint = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_SkillPoint:
		m_skillPoint = (unsigned short)(atoi(relStr.c_str()));
		break;
	default:
		crRoleData::addParam(i,str);
	}
}
void crRpgGameRoleData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[128];
		//sprintf(buf,"%d,%s\0",WCHDATA_SightRange,crArgumentParser::appFtoa((float)m_sightRange * crGlobalHandle::gData()->gUnitScale()).c_str());
		//cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_HP,(int)m_hp);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_MP,(int)m_mp);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Attack,(int)m_attack);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Defend,(int)m_defend);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Speed,(int)((float)m_speed * crGlobalHandle::gData()->gUnitScale()));
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Stealth,(int)((float)m_stealth * crGlobalHandle::gData()->gUnitScale()));
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_StealthSpy,(int)((float)m_stealthspy * crGlobalHandle::gData()->gUnitScale()));
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_AttackSpeed,(int)m_attackSpeed);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_SkillSpeed,(int)m_skillSpeed);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_ArmourType,(int)m_armourType);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Level,(int)m_level);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Experience,(int)m_experience);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Gold,(int)m_gold);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Strength,(int)m_strength);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Agility,(int)m_agility);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Intellect,(int)m_intellect);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Camp,(int)m_camp);
		sprintf(buf,"%d,%d\0",WCHDATA_MaxAttack,(int)m_maxattack);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_MaxSkill,(int)m_maxskill);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_MaxBackPack,(int)m_maxbackpack);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_MaxStorage,(int)m_maxstorage);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_AttrPoint,(int)m_attrPoint);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_SkillPoint,(int)m_skillPoint);
		cfg_script.Add("Param",buf);
		//Tank
		sprintf(buf,"%d,%s\0",WCHDATA_Acceleration,crArgumentParser::appFtoa((float)m_acceleration * crGlobalHandle::gData()->gUnitScale()).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_Damp,crArgumentParser::appFtoa((float)m_damp * crGlobalHandle::gData()->gUnitScale()).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_AngularAccel,crArgumentParser::appItoa(m_angularAccel).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_AngularDamp,crArgumentParser::appItoa(m_angularDamp).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_MaxAngular,crArgumentParser::appItoa(m_maxAngular).c_str());
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_MaxEquipon,(int)m_maxequipon);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_MaxQuick,(int)m_maxquick);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_ExpToKiller,(int)m_expToKiller);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_GoldToKiller,(int)m_goldToKiller);
		//cfg_script.Add("Param",buf);
		//cfg_script.Add("Param",buf);
		///////
		std::vector<float> v_i;
		if(!m_attackMap.empty())
		{
			cfg_script.Add("AttackMap");
			cfg_script.Push("AttackMap");
			for( AttackMap::iterator itr = m_attackMap.begin();
				itr != m_attackMap.end();
				++itr )
			{
				v_i.clear();
				v_i.push_back(itr->first);
				if(itr->second>0)
					v_i.push_back(itr->second);
				else
					v_i.push_back(0);
				cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}
		if(!m_skillMap.empty())
		{
			cfg_script.Add("SkillMap");
			cfg_script.Push("SkillMap");
			for( SkillMap::iterator itr = m_skillMap.begin();
				itr != m_skillMap.end();
				++itr )
			{
				v_i.clear();
				v_i.push_back(itr->first);
				if(itr->second>0)
					v_i.push_back(itr->second);
				else
					v_i.push_back(0);
				cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}
		//if(!m_backPackMap.empty())
		//{
		//	cfg_script.Add("BackPackMap");
		//	cfg_script.Push("BackPackMap");
		//	for( BackPackMap::iterator itr = m_backPackMap.begin();
		//		itr != m_backPackMap.end();
		//		++itr )
		//	{
		//		v_i.clear();
		//		v_i.push_back(itr->first);
		//		if(itr->second>0)
		//			v_i.push_back(itr->second);
		//		else
		//			v_i.push_back(0);
		//		cfg_script.Add("Param",v_i);
		//	}
		//	cfg_script.Pop();
		//}
  //      if(!m_storageMap.empty())
		//{
		//	cfg_script.Add("StorageMap");
		//	cfg_script.Push("StorageMap");
		//	for( StorageMap::iterator itr = m_storageMap.begin();
		//		itr != m_storageMap.end();
		//		++itr )
		//	{
		//		v_i.clear();
		//		v_i.push_back(itr->first);
		//		if(itr->second>0)
		//			v_i.push_back(itr->second);
		//		else
		//			v_i.push_back(0);
		//		cfg_script.Add("Param",v_i);
		//	}
		//	cfg_script.Pop();
		//}
		//if(!m_storageCountMap.empty())
		//{
		//	cfg_script.Add("StorageCountMap");
		//	cfg_script.Push("StorageCountMap");
		//	for( ItemCountMap::iterator itr = m_storageCountMap.begin();
		//		itr != m_storageCountMap.end();
		//		++itr )
		//	{
		//		v_i.clear();
		//		v_i.push_back(itr->first);
		//		v_i.push_back((float)(itr->second));
		//		cfg_script.Add("Param",v_i);
		//	}
		//	cfg_script.Pop();
		//}
		if(!m_equipOnMap.empty())
		{
			cfg_script.Add("EquipOnMap");
			cfg_script.Push("EquipOnMap");
			for( EquipOnMap::iterator itr = m_equipOnMap.begin();
				itr != m_equipOnMap.end();
				++itr )
			{
				v_i.clear();
				v_i.push_back(itr->first);
				if(itr->second>0)
					v_i.push_back(itr->second);
				else
					v_i.push_back(0);
				cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}
		//if(!m_quickList.empty())
		//{
		//	cfg_script.Add("QuickList");
		//	cfg_script.Push("QuickList");
		//	for( QuickList::iterator itr = m_quickList.begin();
		//		itr != m_quickList.end();
		//		++itr )
		//	{
		//		v_i.clear();
		//		v_i.push_back(itr->first);
		//		if(itr->second>0)
		//			v_i.push_back(itr->second);
		//		else
		//			v_i.push_back(0);
		//		cfg_script.Add("Param",v_i);
		//	}
		//	cfg_script.Pop();
		//}
		/*if(!m_quickKeyVec.empty())
		{
			cfg_script.Add("QuickKeyVec");
			cfg_script.Push("QuickKeyVec");
			for( QuickKeyVec::iterator itr = m_quickKeyVec.begin();
				itr != m_quickKeyVec.end();
				++itr )
			{
				v_i.clear();
				v_i.push_back(itr->first);
				v_i.push_back(itr->second);
				cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}
        if(!m_itemCountMap.empty())
		{
			cfg_script.Add("ItemCountMap");
			cfg_script.Push("ItemCountMap");
			for( ItemCountMap::iterator itr = m_itemCountMap.begin();
				itr != m_itemCountMap.end();
				++itr )
			{
				v_i.clear();
				v_i.push_back(itr->first);
				v_i.push_back((float)(itr->second));
				cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}*/
		//cfg_script.Add("PatrolPointVec");
		//cfg_script.Push("PatrolPointVec");
		//for( PatrolPointVec::iterator itr = m_patrolPointVec.begin();
		//	itr != m_patrolPointVec.end();
		//	++itr )
		//{
		//	v_i.clear();
		//	v_i.push_back((*itr)[0]);
		//	v_i.push_back((*itr)[1]);
		//	cfg_script.Add("Param",v_i);
		//}
		//cfg_script.Pop();

		//cfg_script.Add("ItemDropMap");
		//cfg_script.Push("ItemDropMap");
		//for( ItemDropMap::iterator itr = m_itemDropMap.begin();
		//	itr != m_itemDropMap.end();
		//	++itr )
		//{
		//	v_i.clear();
		//	v_i.push_back(itr->first);
		//	v_i.push_back(itr->second);
		//	cfg_script.Add("Param",v_i);
		//}
		//cfg_script.Pop();
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(1024);

		stream->_writeInt(m_hp);
        stream->_writeInt(m_mp);
		stream->_writeInt(m_attack);
		stream->_writeInt(m_defend);
		stream->_writeShort(m_speed);
		stream->_writeShort(m_stealth);
		stream->_writeShort(m_stealthspy);
		stream->_writeShort(m_attackSpeed);
		stream->_writeShort(m_skillSpeed);
		stream->_writeUChar(m_armourType);
		stream->_writeUChar(m_level);
		stream->_writeInt(m_experience);
		stream->_writeInt(m_gold);
		stream->_writeShort(m_strength);
		stream->_writeShort(m_agility);
		stream->_writeShort(m_intellect);
		stream->_writeUChar(m_camp);
		stream->_writeUShort(m_attrPoint);
		stream->_writeUShort(m_skillPoint);

		//Tank
		stream->_writeShort(m_acceleration);
		stream->_writeShort(m_damp);
		stream->_writeShort(m_angularAccel);
		stream->_writeShort(m_angularDamp);
		stream->_writeShort(m_maxAngular);
		///////
		stream->_writeUChar(m_attackMap.size());
		for( AttackMap::iterator itr = m_attackMap.begin();
			itr != m_attackMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		stream->_writeUChar(m_skillMap.size());
		for( SkillMap::iterator itr = m_skillMap.begin();
			itr != m_skillMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_backPackMap.size());
		//for( BackPackMap::iterator itr = m_backPackMap.begin();
		//	itr != m_backPackMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		stream->_writeUChar(m_equipOnMap.size());
		for( EquipOnMap::iterator itr = m_equipOnMap.begin();
			itr != m_equipOnMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_quickList.size());
		//for( QuickList::iterator itr = m_quickList.begin();
		//	itr != m_quickList.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		//stream->_writeUChar(m_quickKeyVec.size());
		//for( QuickKeyVec::iterator itr = m_quickKeyVec.begin();
		//	itr != m_quickKeyVec.end();
		//	++itr )
		//{
		//	stream->_writeChar(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		//stream->_writeUChar(m_itemCountMap.size());
		//for( ItemCountMap::iterator itr = m_itemCountMap.begin();
		//	itr != m_itemCountMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeUChar(itr->second);
		//}
		//SaleItemMap
		//stream->_writeUChar(m_saleItemMap.size());
		//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
		//	itr != m_saleItemMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);//abstractid
		//	stream->_writeVec3i(itr->second);
		//}
		//stream->_writeUChar(m_storageMap.size());
		//for( StorageMap::iterator itr = m_storageMap.begin();
		//	itr != m_storageMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		//stream->_writeUChar(m_storageCountMap.size());
		//for( ItemCountMap::iterator itr = m_storageCountMap.begin();
		//	itr != m_storageCountMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeUChar(itr->second);
		//}
		stream->seekBegin();
	}
	if(LOINT64(msg) == WCH_BuildNetStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(1024);

		stream->_writeInt(m_hp);
		stream->_writeInt(m_mp);
		stream->_writeInt(m_attack);
		stream->_writeInt(m_defend);
		stream->_writeShort(m_speed);
		stream->_writeUChar(m_level);
		stream->_writeShort(m_strength);
		stream->_writeShort(m_agility);
		stream->_writeShort(m_intellect);
		stream->_writeUChar(m_camp);
		//Tank
		stream->_writeShort(m_acceleration);
		stream->_writeShort(m_damp);
		stream->_writeShort(m_angularAccel);
		stream->_writeShort(m_angularDamp);
		stream->_writeShort(m_maxAngular);
		///////
		stream->_writeUChar(m_attackMap.size());
		for( AttackMap::iterator itr = m_attackMap.begin();
			itr != m_attackMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		stream->_writeUChar(m_skillMap.size());
		for( SkillMap::iterator itr = m_skillMap.begin();
			itr != m_skillMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_backPackMap.size());
		//for( BackPackMap::iterator itr = m_backPackMap.begin();
		//	itr != m_backPackMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		stream->_writeUChar(m_equipOnMap.size());
		for( EquipOnMap::iterator itr = m_equipOnMap.begin();
			itr != m_equipOnMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_quickList.size());
		//for( QuickList::iterator itr = m_quickList.begin();
		//	itr != m_quickList.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		////SaleItemMap
		//stream->_writeUChar(m_saleItemMap.size());
		//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
		//	itr != m_saleItemMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);//abstractid
		//	stream->_writeVec3i(itr->second);
		//}
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_ServerCheckRoleData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
        serverCheckData(item,WCH_ServerCheckRoleData);
		//item->beginCreateItemChild(WCH_ServerCheckRoleData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_ServerCheckItemData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
		serverCheckData(item,WCH_ServerCheckItemData);
		//item->beginCreateItemChild(WCH_ServerCheckItemData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_CheckNetData/*WCH_CheckPlayerData*/)
	{
		//GNE::LockMutex lock( m_dataMutex );避免与crGlobalHandle的互斥信号产生互锁现象
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		clientCheckData(item);
	}
	//else if(LOINT64(msg) == WCH_CheckNetData)
	//{
	//	//GNE::LockMutex lock( m_dataMutex );
	//	crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
	//	crItemChild *itemChild;
	//	for( AttackMap::iterator itr = m_attackMap.begin();
	//		itr != m_attackMap.end();
	//		++itr )
	//	{
	//		if(itr->second != 0)
	//		{
	//			itemChild = item->findChildItem(itr->second);
	//			if(itemChild && !itemChild->getInstanceItem()->isLoaded())
	//			{//check And download itemchild
	//				crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
	//			}
	//		}
	//		else
	//		{
	//			CRCore::notify(CRCore::ALWAYS)<<"crRpgGameRoleData WCH_CheckPlayerData AttackMap ID没初始化 "<<itr->first<<std::endl;
	//		}
	//	}
	//	for( SkillMap::iterator itr = m_skillMap.begin();
	//		itr != m_skillMap.end();
	//		++itr )
	//	{
	//		itemChild = item->findChildItem(itr->second);
	//		if(itemChild && !itemChild->getInstanceItem()->isLoaded())
	//		{//check And download itemchild
	//			crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
	//		}
	//	}
	//	std::set<int> TemporaryItemIDSet;
	//	std::set<int>::iterator taitr;
	//	for( EquipOnMap::iterator itr = m_equipOnMap.begin();
	//		itr != m_equipOnMap.end();
	//		++itr )
	//	{
	//		itemChild = item->findChildItem(itr->second);
	//		if(itemChild && !itemChild->getInstanceItem()->isLoaded())
	//		{//check And download itemchild
	//			taitr = TemporaryItemIDSet.find(itr->second);
	//			if(taitr == TemporaryItemIDSet.end())
	//			{
	//				TemporaryItemIDSet.insert(itr->second);
	//				crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
	//			}
	//		}
	//	}
	//	for( SaleItemMap::iterator itr = m_saleItemMap.begin();
	//		itr != m_saleItemMap.end();
	//		++itr )
	//	{
	//		itemChild = item->findChildItem(itr->second[0]);
	//		if(itemChild && !itemChild->getInstanceItem()->isLoaded())
	//		{//check And download itemchild
	//			taitr = TemporaryItemIDSet.find(itr->second[0]);
	//			if(taitr == TemporaryItemIDSet.end())
	//			{
	//				TemporaryItemIDSet.insert(itr->second[0]);
	//				crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
	//			}
	//		}
	//	}
	//}
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_InitData)
	{
		if(m_guiseState & GS_StaticUnVisiable || m_guiseState & GS_Static || m_guiseState & GS_StaticNoneBlock)
			m_guiseState = GS_Static;
		else
			m_guiseState = GS_Normal;
		if(m_guiseState == GS_Normal)
		{//GS_Static没有buf
			if(!m_extraDataClass.valid())
				m_extraDataClass = new crRpgGameExtraData;
			else
				m_extraDataClass->clear();//ExtraData lock > datalock
		}

		//GNE::LockMutex lock( m_dataMutex );
		m_dataMutex.acquire();
		m_targetType = Target_None;
		//m_targetNode = NULL;
		m_targetID = 0;
		m_targetRoleID = 0;
		m_targetPosition.set(0,0,0);
		m_moveToPosition.set(0,0,0);
		m_itemState = IS_Relive;
		m_followDistance = crGlobalHandle::gData()->gMinFollowDistance();
		m_rthp = m_hp;
		m_rtmp = m_mp;
		m_kill = 0;
		m_dead = 0;
		
		m_currentAttackID = 0;
		m_aboutToUseItemID = 0;
		m_fightmode = FM_Peace;
		m_groupMemberSet.clear();
		m_patrolIndex = 0;
		m_enmityMap.clear();
		m_inpatrolEnemyMap.clear();

		m_extrahp = 0;
		m_extramp = 0;
		m_extraAttack = 0;
		m_extraDefend = 0;
		//m_extraSpeed = 0;
		m_extraStealth = 0;
		m_extraStealthSpy = 0;
		m_extraHitratioPercent = 0;
		m_extraAttackSpeed = 0;
		m_extraSkillSpeed = 0;
		m_extraGuise = GS_Normal;
		m_extraRadius = 100;
		m_extraIgnoreDefend = 0;
		m_extraSightRange = 0;

		m_extraLevel = 0;
		m_extraStrength = 0;
		m_extraAgility = 0;
		m_extraIntellect = 0;
		m_currentSpeed = 0;
		m_dataMutex.release();
	}
	else if(msg == WCH_ResetExtra)
	{
		GNE::LockMutex lock( m_dataMutex );
		m_extrahp_rd = m_extrahp;
		m_extrahp = 0;
		m_extramp_rd = m_extramp;
		m_extramp = 0;
		m_extraAttack = 0;
		m_extraDefend = 0;
		//m_extraSpeed = 0;
		m_extraStealth = 0;
		m_extraStealthSpy = 0;
		m_extraHitratioPercent = 0;
		m_extraAttackSpeed = 0;
		m_extraSkillSpeed = 0;
		m_extraGuise = GS_Normal;
		m_extraRadius = 100;
		m_extraIgnoreDefend = 0;
		m_extraSightRange = 0;

		m_extraLevel = 0;
		m_extraStrength = 0;
		m_extraAgility = 0;
		m_extraIntellect = 0;
	}
	else
	{
		crRoleData::excHandle(msg); 
	}
}
void crRpgGameRoleData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_CurrentSpeed:
		m_currentSpeed = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Acceleration:
		m_acceleration = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Damp:
		m_damp = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_CurrentAngular:
		m_currentAngular = param==NULL?NULL:*((float*)param);
		break;
	case WCHDATA_AngularAccel:
		m_angularAccel = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_AngularDamp:
		m_angularDamp = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_MaxAngular:
		m_maxAngular = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Level:
		m_level = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_Experience:
		m_experience = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_Gold:
		m_gold = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_Strength:
		m_strength = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Agility:
		m_agility = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Intellect:
		m_intellect = param==NULL?NULL:*((short*)param);
		break;
	//case WCHDATA_MaxAttack:
	//	m_maxattack = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_MaxSkill:
	//	m_maxskill = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_MaxBackPack:
	//	m_maxbackpack = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_MaxStorage:
	//	m_maxstorage = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_MaxEquipon:
	//	m_maxequipon = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_MaxQuick:
	//	m_maxquick = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	case WCHDATA_GroupMemberSet:
		if(param && m_groupMemberSet.size()<crGlobalHandle::gData()->gMaxgroupmember())
		{
            int playerid = *((int*)param);
			m_groupMemberSet.insert(playerid);
		}
		break;
	case WCHDATA_ExtraLevel:
		m_extraLevel = param==NULL?0:*((char*)param);
		break;
	case WCHDATA_ExtraStrength:
		m_extraStrength = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraAgility:
		m_extraAgility = param==NULL?0:*((short*)param);
		break;
	case WCHDATA_ExtraIntellect:
		m_extraIntellect = param==NULL?0:*((short*)param);
		break;
	//case WCHDATA_ExchangePlayerSet:
	//	if(param)
	//	{
	//		int playerid = *(int*)param;
	//		m_exchangePlayerSet.insert(playerid);
	//	}
	//	break;
	case WCHDATA_SkillPoint:
		m_skillPoint = param==NULL?NULL:*(unsigned short*)param;
		break;
	case WCHDATA_AttrPoint:
		m_attrPoint = param==NULL?NULL:*(unsigned short*)param;
		break;
	case WCHDATA_ControllerFlg:
		m_controllerFlg = param==NULL?0:*(unsigned char*)param;
		break;
	case WCHDATA_ControllerTime:
		m_controllerTime = param==NULL?0.0f:*(float*)param;
		break;
	case WCHDATA_Kill:
		m_kill = param==NULL?NULL:*(short*)param;
		break;
	case WCHDATA_Dead:
		m_dead = param==NULL?NULL:*(short*)param;
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crRpgGameRoleData creUnCompress error"<<std::endl;
			}

			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
            dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			std::vector<float> v_i;
			int nParam = 1;
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}
			if(cfg_script.Push("AttackMap"))
			{
				m_attackMap.clear();
				nParam = 1;
				while (cfg_script.Get("Param", v_i,nParam++))
				{
					m_attackMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
				}
				cfg_script.Pop();
			}
			if(cfg_script.Push("SkillMap"))
			{
				m_skillMap.clear();
				nParam = 1;
				while (cfg_script.Get("Param", v_i,nParam++))
				{
				    m_skillMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
				}
				cfg_script.Pop();
			}
			//if(cfg_script.Push("BackPackMap"))
			//{
			//	m_backPackMap.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		m_backPackMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			//if(cfg_script.Push("StorageMap"))
			//{
			//	m_storageMap.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		m_storageMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			//if(cfg_script.Push("StorageCountMap"))
			//{
			//	m_storageCountMap.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		if(v_i[1]>0)
			//			m_storageCountMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			if(cfg_script.Push("EquipOnMap"))
			{
				m_equipOnMap.clear();
				nParam = 1;
				while (cfg_script.Get("Param", v_i,nParam++))
				{
					m_equipOnMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
				}
				cfg_script.Pop();
			}
			//if(cfg_script.Push("QuickList"))
			//{
			//	m_quickList.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		m_quickList.push_back(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			//if(cfg_script.Push("QuickKeyVec"))
			//{
			//	m_quickKeyVec.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		m_quickKeyVec.push_back(std::make_pair((char)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			//if(cfg_script.Push("ItemCountMap"))
			//{
			//	m_itemCountMap.clear();
			//	nParam = 1;
			//	while (cfg_script.Get("Param", v_i,nParam++))
			//	{
			//		if(v_i[1]>0)
			//		    m_itemCountMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
			//	}
			//	cfg_script.Pop();
			//}
			cfg_script.Close();
			DeleteFileA(file);
			//dataStream = NULL;
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_hp = dataStream->_readInt();
			m_mp = dataStream->_readInt();
			m_attack = dataStream->_readInt();
			m_defend = dataStream->_readInt();
			m_speed = dataStream->_readShort();
			m_stealth = dataStream->_readShort();
			m_stealthspy = dataStream->_readShort();
			m_attackSpeed = dataStream->_readShort();
			m_skillSpeed = dataStream->_readShort();
			m_armourType = dataStream->_readUChar();
			m_level = dataStream->_readUChar();
			m_experience = dataStream->_readInt();
			m_gold = dataStream->_readInt();
			m_strength = dataStream->_readShort();
			m_agility = dataStream->_readShort();
			m_intellect = dataStream->_readShort();
            m_camp = dataStream->_readUChar();
			m_attrPoint = dataStream->_readUShort();
			m_skillPoint = dataStream->_readUShort();
			//Tank交互
			m_acceleration = dataStream->_readShort();
			m_damp = dataStream->_readShort();
			m_angularAccel = dataStream->_readShort();
			m_angularDamp = dataStream->_readShort();
			m_maxAngular = dataStream->_readShort();
			///////
			unsigned char size;
			unsigned char i;
			int int1,int2;
			//unsigned char uchar2;
			crVector3i vec3;
			size = dataStream->_readUChar();
			m_attackMap.clear();
			for(i = 0; i<size; i++)
			{
                int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				//m_attackMap[int1] = int2;
				m_attackMap.insert(std::make_pair(int1,int2));
			}
			size = dataStream->_readUChar();
			m_skillMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				//m_skillMap[int1] = int2;
				m_skillMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_backPackMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_backPackMap.insert(std::make_pair(int1,int2));
			//}
			size = dataStream->_readUChar();
			m_equipOnMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_equipOnMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_quickList.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_quickList.push_back(std::make_pair(int1,int2));
			//}
			//size = dataStream->_readUChar();
			//char key;
			//m_quickKeyVec.clear();
			//for(i = 0; i<size; i++)
			//{
			//	key = dataStream->_readChar();
			//	int1 = dataStream->_readInt();
			//	m_quickKeyVec.push_back(std::make_pair(key,int1));
			//}
			//size = dataStream->_readUChar();
			//m_itemCountMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	uchar2 = dataStream->_readUChar();
			//	m_itemCountMap.insert(std::make_pair(int1,uchar2));
			//}
			//size = dataStream->_readUChar();
			//m_saleItemMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	vec3 = dataStream->_readVec3i();
			//	m_saleItemMap.insert(std::make_pair(int1,vec3));
			//}
			//size = dataStream->_readUChar();
			//m_storageMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_storageMap.insert(std::make_pair(int1,int2));
			//}
			//size = dataStream->_readUChar();
			//m_storageCountMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	uchar2 = dataStream->_readUChar();
			//	m_storageCountMap.insert(std::make_pair(int1,uchar2));
			//}
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_NetStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_hp = dataStream->_readInt();
			m_mp = dataStream->_readInt();
			m_attack = dataStream->_readInt();
			m_defend = dataStream->_readInt();
			m_speed = dataStream->_readShort();
			m_level = dataStream->_readUChar();
			m_strength = dataStream->_readShort();
			m_agility = dataStream->_readShort();
			m_intellect = dataStream->_readShort();
			m_camp = dataStream->_readUChar();
			//Tank交互
			m_acceleration = dataStream->_readShort();
			m_damp = dataStream->_readShort();
			m_angularAccel = dataStream->_readShort();
			m_angularDamp = dataStream->_readShort();
			m_maxAngular = dataStream->_readShort();
			///////
			unsigned char size;
			unsigned char i;
			int int1,int2;
			crVector3i vec3;
			size = dataStream->_readUChar();
			m_attackMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				//m_attackMap[int1] = int2;
				m_attackMap.insert(std::make_pair(int1,int2));
			}
			size = dataStream->_readUChar();
			m_skillMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				//m_skillMap[int1] = int2;
				m_skillMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_backPackMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_backPackMap.insert(std::make_pair(int1,int2));
			//}
			size = dataStream->_readUChar();
			m_equipOnMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_equipOnMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_quickList.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_quickList.push_back(std::make_pair(int1,int2));
			//}
			//size = dataStream->_readUChar();
			//m_saleItemMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	vec3 = dataStream->_readVec3i();
			//	m_saleItemMap.insert(std::make_pair(int1,vec3));
			//}
			//dataStream = NULL;
		}
		break;
	default:
		crRoleData::inputParam(i,param);
		break;
	}
}
void crRpgGameRoleData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_CurrentSpeed:
		param = &m_currentSpeed;
		break;
	case WCHDATA_Acceleration:
		param = &m_acceleration;
		break;
	case WCHDATA_Damp:
		param = &m_damp;
		break;
	case WCHDATA_CurrentAngular:
		param = &m_currentAngular;
		break;
	case WCHDATA_AngularAccel:
		param = &m_angularAccel;
		break;
	case WCHDATA_AngularDamp:
		param = &m_angularDamp;
		break;
	case WCHDATA_MaxAngular:
		param = &m_maxAngular;
		break;
	case WCHDATA_Level:
		param = &m_level;
		break;
	case WCHDATA_Experience:
		param = &m_experience;
		break;
	case WCHDATA_Gold:
		param = &m_gold;
		break;
	case WCHDATA_Strength:
		param = &m_strength;
		break;
	case WCHDATA_Agility:
		param = &m_agility;
		break;
	case WCHDATA_Intellect:
		param = &m_intellect;
		break;
	//case WCHDATA_MaxAttack:
	//	param = &m_maxattack;
	//	break;
	//case WCHDATA_MaxSkill:
	//	param = &m_maxskill;
	//	break;
	//case WCHDATA_MaxBackPack:
	//	param = &m_maxbackpack;
	//	break;
	//case WCHDATA_MaxStorage:
	//	param = &m_maxstorage;
	//	break;
	//case WCHDATA_MaxEquipon:
	//	param = &m_maxequipon;
	//	break;
	//case WCHDATA_MaxQuick:
	//	param = &m_maxquick;
	//	break;
	//case WCHDATA_AttackMap:
	//	param = &m_attackMap;
	//	break;
	//case WCHDATA_SkillMap:
	//	param = &m_skillMap;
	//	break;
	//case WCHDATA_BackPackMap:
	//	param = &m_backPackMap;
	//	break;
	//case WCHDATA_StorageMap:
	//	param = &m_storageMap;
	//	break;
	//case WCHDATA_StorageCountMap:
	//	param = &m_storageCountMap;
	//	break;
	//case WCHDATA_EquipOnMap:
	//	param = &m_equipOnMap;
	//	break;
	//case WCHDATA_QuickList:
	//	param = &m_quickList;
	//	break;
	//case WCHDATA_QuickKeyVec:
	//	param = &m_quickKeyVec;
	//	break;
	//case WCHDATA_ItemCountMap:
	//	param = &m_itemCountMap;
	//	break;
	case WCHDATA_ItemIcon:
		param = &m_itemIcon;
		break;
	case WCHDATA_ExpToKiller:
		param = &m_expToKiller;
		break;
	case WCHDATA_GoldToKiller:
		param = &m_goldToKiller;
		break;
	//case WCHDATA_ItemDropMap:
	//	param = &m_itemDropMap;
	//	break;
	//case WCHDATA_SaleItemMap:
	//	param = &m_saleItemMap;
	//	break;
	case WCHDATA_AttrPoint:
		param = &m_attrPoint;
		break;
	case WCHDATA_SkillPoint:
		param = &m_skillPoint;
		break;
	case WCHDATA_GroupMemberSet:
		param = &m_groupMemberSet;
		break;
	//case WCHDATA_NpcEnmityMap:
	//	param = &m_playerEnmityMap;
	//	break;
	case WCHDATA_ExtraLevel:
		param = &m_extraLevel;
		break;
	case WCHDATA_ExtraStrength:
		param = &m_extraStrength;
		break;
	case WCHDATA_ExtraAgility:
		param = &m_extraAgility;
		break;
	case WCHDATA_ExtraIntellect:
		param = &m_extraIntellect;
		break;
	case WCHDATA_ControllerFlg:
		param = &m_controllerFlg;
		break;
	case WCHDATA_ControllerTime:
		param = &m_controllerTime;
		break;
	case WCHDATA_Kill:
		param = &m_kill;
		break;
	case WCHDATA_Dead:
		param = &m_dead;
		break;
	default:
		crRoleData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameMetierData
//
/////////////////////////////////////////
GNE::Mutex crRpgGameMetierData::m_saveMutex;
crRpgGameMetierData::crRpgGameMetierData():
m_currentTaskID(0)
{
}
crRpgGameMetierData::crRpgGameMetierData(const crRpgGameMetierData& data):
crData(data),
m_gameTaskMap(data.m_gameTaskMap),
m_metierSkillASet(data.m_metierSkillASet),
m_currentTaskID(0)
{
}
crRpgGameMetierData::~crRpgGameMetierData()
{
	GNE::LockMutex lock( m_dataMutex );
	for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
		itr != m_gameTaskMap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_gameTaskMap.clear();
}
void crRpgGameMetierData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_MetierSkillASet:
		m_metierSkillASet.insert(atoi(relStr.c_str()));
		break;
	}
}
void crRpgGameMetierData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[128];

		//cfg_script.Add("MetierSkillAVec");
		//cfg_script.Push("MetierSkillAVec");
		//for( MetierSkillAVec::iterator itr = m_metierSkillAVec.begin();
		//	itr != m_metierSkillAVec.end();
		//	++itr )
		//{
		//	sprintf(buf,"%d\0",*itr);
		//	cfg_script.Add("Param",buf);
		//}
		//cfg_script.Pop();

		ref_ptr<crGameTask> gameTask;
		std::vector<float> v_i;
		int nGameTask = 1;
		for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
			itr != m_gameTaskMap.end();
			++itr )
		{
			if(itr->second->isNeedSave())
			{
				cfg_script.Add("GameTaskMap");
				cfg_script.Push("GameTaskMap",nGameTask++);

				gameTask = itr->second.get();
				sprintf(buf,"%d\0",(int)(gameTask->getTaskID()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getActivationFlg()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getTaskProgress()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getCircles()));
				cfg_script.Add("Param",buf);

				cfg_script.Pop();
			}
		}

		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		///////
		std::vector< ref_ptr<crGameTask> > GameTaskVec;
		ref_ptr<crGameTask> gameTask;
		for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
			itr != m_gameTaskMap.end();
			++itr )
		{
			gameTask = itr->second.get();
			if(gameTask->isNeedSave())
			{
				GameTaskVec.push_back(gameTask.get());
			}
		}
		stream->createBuf(10*GameTaskVec.size()+1);
		stream->_writeUChar(GameTaskVec.size());
		for( std::vector< ref_ptr<crGameTask> >::iterator itr = GameTaskVec.begin();
			itr != GameTaskVec.end();
			++itr )
		{
			gameTask = itr->get();
			stream->_writeInt(gameTask->getTaskID());
			stream->_writeChar(gameTask->getActivationFlg());
			stream->_writeInt(gameTask->getTaskProgress());
			stream->_writeUChar(gameTask->getCircles());
		}
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crRpgGameMetierData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_GameTaskTab:
		{
			std::string *file = (std::string *)param;
			ref_ptr<crTableIO> tasktab = crTableIO::openFile(*file);
			if(tasktab.valid())
			{
				ref_ptr<crGameTask> gameTask;
				int rowcount = tasktab->getRowCount();
				//crVector2 awardItem;
				crVector2s lvRange;
				int col = 0;
				crVector4b taskStep;
				std::string datafile;
				int overdue = tasktab->getTitleIndex("过期");
				for(int i = 0; i<rowcount; ++i)
				{
					col = 0;
					if(overdue>=0 && atoi((*tasktab)(i,overdue).c_str())==1)
						continue;
					gameTask = new crGameTask;
					gameTask->setTaskID(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setTaskName((*tasktab)(i,col++));
					gameTask->setHyperText((*tasktab)(i,col++));
					gameTask->setCircles(atoi((*tasktab)(i,col++).c_str()));
					if(crArgumentParser::appAtoVec((*tasktab)(i,col++),lvRange))
						gameTask->setLvRange(lvRange);
					gameTask->setPreTask(atoi((*tasktab)(i,col++).c_str()));
					taskStep.set(0,0,0,0);
					crArgumentParser::appAtoVec((*tasktab)(i,col++),taskStep);
					gameTask->setTaskSteps(taskStep);
					gameTask->setTaskStepType((unsigned char)(atoi((*tasktab)(i,col++).c_str())));
					gameTask->setExp(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setExp2(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setGold(atoi((*tasktab)(i,col++).c_str()));
					if(crGlobalHandle::isClient())
					{
						datafile = (*tasktab)(i,col++);
						if(!datafile.empty())
							gameTask->setDataClass(loadData(crGlobalHandle::getInstance()->getScriptDir()+datafile.c_str()));
					}
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					m_gameTaskMap[gameTask->getTaskID()] = gameTask.get();
				}
			}
		}
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crRpgGameMetierData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			int nParam = 1;
			int nGameTask = 1;
			ref_ptr<crGameTask> gameTask;
			int gametaskid;
			GameTaskMap::iterator taskitr;
			while(cfg_script.Push("GameTaskMap",nGameTask++))
			{
				nParam = 1;
				cfg_script.Get("Param", str,nParam++);
				gametaskid = atoi(str.c_str());
				taskitr = m_gameTaskMap.find(gametaskid);
				if(taskitr != m_gameTaskMap.end())
				{
					gameTask = taskitr->second.get();
					cfg_script.Get("Param", str,nParam++);
					gameTask->setActivationFlg(atoi(str.c_str()));
					cfg_script.Get("Param", str,nParam++);
					gameTask->setTaskProgress(atoi(str.c_str()));
					cfg_script.Get("Param", str,nParam++);
					gameTask->setCircles(atoi(str.c_str()));
				}
				cfg_script.Pop();
			}
			cfg_script.Close();
			DeleteFileA(file);
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			unsigned char size = dataStream->_readUChar();
			unsigned char i;
			ref_ptr<crGameTask> gameTask;
			int gametaskid;
			GameTaskMap::iterator itr;
			for(i = 0; i<size; i++)
			{
				gametaskid = dataStream->_readInt();
				itr = m_gameTaskMap.find(gametaskid);
				if(itr != m_gameTaskMap.end())
				{
					gameTask = itr->second.get();
					if(gameTask.valid())
					{
						gameTask->setActivationFlg(dataStream->_readChar());
						gameTask->setTaskProgress(dataStream->_readInt());
						gameTask->setCircles(dataStream->_readUChar());
					}
				}
				else
				{
					dataStream->seek(6);
				}
			}
			//m_dataStream = NULL;
		}
	case WCHDATA_CurrentTaskID:
		m_currentTaskID = param==NULL?NULL:*((int*)param);
		break;
	}
}
void crRpgGameMetierData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_MetierSkillASet:
		param = &m_metierSkillASet;
		break;
	case WCHDATA_GameTaskMap:
		param = &m_gameTaskMap;
		break;
	case WCHDATA_CurrentTaskID:
		param = &m_currentTaskID;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameBotData
//
/////////////////////////////////////////
crRpgGameBotData::crRpgGameBotData():
m_item(NULL),
m_viewHeight(1.7),
m_walkableHeight(0.5),
m_turnToIdleTime(5.0)//,
//m_cameraBorrowTime(0.0f),
//m_cameraBorrowInterval(0.0f)
{
}
crRpgGameBotData::crRpgGameBotData(const crRpgGameBotData& data):
crNodeData(data),
m_item(data.m_item),
m_viewHeight(data.m_viewHeight),
m_walkableHeight(data.m_walkableHeight),
m_turnToIdleTime(data.m_turnToIdleTime),
m_botEffectStr(data.m_botEffectStr)//,
//m_cameraBorrowTime(0.0f),
//m_cameraBorrowInterval(0.0f)
{
}
void crRpgGameBotData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_ViewHeight:
		m_viewHeight = atof(relStr.c_str());
		break;
	case WCHDATA_WalkableHeight:
		m_walkableHeight = atof(relStr.c_str());
		break;
	case WCHDATA_TurnToIdleTime:
		m_turnToIdleTime = atof(relStr.c_str());
		break;
	case WCHDATA_BotEffectStr:
		m_botEffectStr = relStr;
		break;
	}
}

void crRpgGameBotData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_Item:
		m_item = (crInstanceItem *)param;
		break;
	//case WCHDATA_CameraBorrowTime:
	//	m_cameraBorrowTime = *(float *)param;
	//	break;
	//case WCHDATA_CameraBorrowInterval:
	//	m_cameraBorrowInterval = *(float *)param;
	//	break;
	}
}

void crRpgGameBotData::getParam(int i, void*& param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_Item:
		param = m_item.get();
		break;
	case WCHDATA_ViewHeight:
		param = &m_viewHeight;
		break;
	case WCHDATA_WalkableHeight:
		param = &m_walkableHeight;
		break;
	case WCHDATA_TurnToIdleTime:
		param = &m_turnToIdleTime;
		break;
	case WCHDATA_BotEffectStr:
		param = &m_botEffectStr;
		break;
	//case WCHDATA_CameraBorrowTime:
	//	param = &m_cameraBorrowTime;
	//	break;
	//case WCHDATA_CameraBorrowInterval:
	//	param = &m_cameraBorrowInterval;
	//	break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameItemData
//
/////////////////////////////////////////
crRpgGameItemData::crRpgGameItemData():
m_dataType(DT_MatrixNode),
m_itemState(0),
m_guiseState(0),
m_touchDistance(10),
m_zoffset(0),
m_belongPlayerID(0),
m_itemPrice(0),
m_rthp(999999),
//m_composeEquipID(0),
m_useItemProtectTime(0),
m_defaultkey(0)
{
}
crRpgGameItemData::crRpgGameItemData(const crRpgGameItemData& data):
crData(data),
m_dataType(data.m_dataType),
m_itemState(data.m_itemState),
m_guiseState(data.m_guiseState),
m_touchDistance(data.m_touchDistance),
m_zoffset(data.m_zoffset),
m_belongPlayerID(0),
m_itemPrice(data.m_itemPrice),
m_rthp(data.m_rthp),
m_itemIcon(data.m_itemIcon),
//m_equipComposeRequestVec(data.m_equipComposeRequestVec),
//m_composeEquipID(data.m_composeEquipID),
//m_saleItemMap(data.m_saleItemMap),
m_useItemProtectTime(data.m_useItemProtectTime),
m_meshFileName(data.m_meshFileName),
m_animFileName(data.m_animFileName),
m_itemTalk(data.m_itemTalk),
m_itemOption(data.m_itemOption),
m_itemInfoText(data.m_itemInfoText),
m_halfPhysicsBox(data.m_halfPhysicsBox),
m_defaultkey(data.m_defaultkey)
{
}
void crRpgGameItemData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_DataType:
		m_dataType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_TouchDistance:
		{
			float range = atof(relStr.c_str());
			range = CRCore::maximum(range,crGlobalHandle::gData()->gMinItemUseRange());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_touchDistance = (unsigned short)range;
		}
		break;
	case WCHDATA_Zoffset:
		{
			float zoffset = atof(relStr.c_str());
			zoffset /= crGlobalHandle::gData()->gUnitScale();
			m_zoffset = (short)zoffset;
		}
		break;
	case WCHDATA_ItemPrice:
		m_itemPrice = atoi(relStr.c_str());
		break;
	case WCHDATA_RTHP:
		m_rthp = atof(relStr.c_str());
		break;
	case WCHDATA_ItemIcon:
		m_itemIcon = relStr;
		break;
	//case WCHDATA_EquipComposeRequestVec:
	//	m_equipComposeRequestVec.push_back(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_ComposeEquipID:
	//	m_composeEquipID = atoi(relStr.c_str());
	//	break;
	//case WCHDATA_SaleItemMap:
	//	{//abstractid count
	//		crVector2i vec2;
	//		CRCore::crArgumentParser::appAtoVec(relStr,vec2);
	//		if(vec2[0]>0)
	//		{//系统售价0表示价格从itemdata里取
	//			m_saleItemMap.insert(std::make_pair(vec2[0],crVector3i(0,vec2[1],0)));
	//		}
	//	}
	//	break;
	case WCHDATA_UseItemProtectTime:
		m_useItemProtectTime = atof(relStr.c_str());
		break;
	case WCHDATA_MeshFileName:
		m_meshFileName = relStr;
		break;
	case WCHDATA_AnimFileName:
		m_animFileName = relStr;
		break;
	case WCHDATA_ItemInfoText:
		m_itemInfoText = relStr;
		break;
	case WCHDATA_HalfPhysicsBox:
		crArgumentParser::appAtoVec(relStr,m_halfPhysicsBox);
		break;
	case WCHDATA_DefaultKey:
		m_defaultkey = (char)atoi(relStr.c_str());
		break;
	case WCHDATA_GuiseState:
		m_guiseState = (unsigned int)atoi(relStr.c_str());
		break;
	}
}
void crRpgGameItemData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildNetStream||LOINT64(msg) == WCH_BuildPlayerStream)
	{
		//GNE::LockMutex lock( m_dataMutex );
		//crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		//stream->createBuf(512);
		////SaleItemMap
		//stream->_writeUChar(m_saleItemMap.size());
		//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
		//	itr != m_saleItemMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);//abstractid
		//	stream->_writeVec3i(itr->second);
		//}
		//stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_ServerCheckItemData)
	{//SceneServer npc
		////GNE::LockMutex lock( m_dataMutex );
		//crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
		////item->getCreateItemChildLock();
		//int count = 0;
		/////SaleItemMap
		//std::set<int> TemporaryAbstractIDSet;
		//std::set<int>::iterator taitr;
		//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
		//	itr != m_saleItemMap.end();
		//	++itr )
		//{
		//	itr->second[0] = 0/*INT_MIN*/;
		//	taitr = TemporaryAbstractIDSet.find(itr->first);
		//	if(taitr == TemporaryAbstractIDSet.end())
		//	{
		//		TemporaryAbstractIDSet.insert(itr->first);
		//		count++;
		//	}
		//}
		//item->beginCreateItemChild(WCH_ServerCheckItemData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
		//else
		//{
		//	crCreateTemporaryItemChildPacket packet;
		//	for( std::set<int>::iterator itr = TemporaryAbstractIDSet.begin();
		//		 itr != TemporaryAbstractIDSet.end();
		//		 ++itr )
		//	{
		//		crCreateTemporaryItemChildPacket::buildRequestPacket(packet,item,item->getInstanceItemID(),*itr);
		//		netConductor->getNetManager()->sendPacket("all",packet);
		//	}
		//}
	}
	else if(LOINT64(msg) == WCH_CheckNetData)
	{
		////GNE::LockMutex lock( m_dataMutex );
		//crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//crItemChild *itemChild;
		//std::set<int> TemporaryItemIDSet;
		//std::set<int>::iterator taitr;
		//for( SaleItemMap::iterator itr = m_saleItemMap.begin();
		//	itr != m_saleItemMap.end();
		//	++itr )
		//{
		//	if(itr->second[0]!=0)
		//	{
		//		std::set<int>::iterator taitr;
		//		taitr = TemporaryItemIDSet.find(itr->second[0]);
		//		if(taitr == TemporaryItemIDSet.end())
		//		{
		//			itemChild = item->findChildItem(itr->second[0]);
		//			if(itemChild)
		//			{//check And download itemchild
		//				if(!itemChild->isLoaded())
		//				{
		//					TemporaryItemIDSet.insert(itr->second[0]);
		//					crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
		//				}
		//			}
		//			else
		//			{
		//				TemporaryItemIDSet.insert(itr->second[0]);
		//				crRequestItemChildPacket packet;
		//				crRequestItemChildPacket::buildRequestPacket(packet,item,itr->second[0]);
		//				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		//				netConductor->getNetManager()->sendPacket("all",packet);
		//			}
		//		}
		//	}
		//	//itemChild = item->findChildItem(itr->second[0]);
		//	//if(itemChild && !itemChild->isLoaded())
		//	//{//check And download itemchild
		//	//	taitr = TemporaryItemIDSet.find(itr->second[0]);
		//	//	if(taitr == TemporaryItemIDSet.end())
		//	//	{
		//	//		TemporaryItemIDSet.insert(itr->second[0]);
		//	//		crGlobalHandle::getInstance()->doEvent(WCH_DownloadItemChild,MAKEINT64(itemChild,item));
		//	//	}
		//	//}
		//}
	}
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crRpgGameItemData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_ItemState:
		m_itemState = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_GuiseState:
		{
			unsigned int newstate = param==NULL?NULL:*((unsigned int*)param);
			if(m_guiseState & GS_Static || m_guiseState & GS_StaticNoneBlock || m_guiseState & GS_StaticUnVisiable)
			{
				if(newstate & GS_Static || newstate & GS_StaticNoneBlock || newstate & GS_StaticUnVisiable)
					m_guiseState = newstate;
			}
			else
			{
				m_guiseState = newstate;
			}
		}
		break;
	case WCHDATA_BelongPlayerID:
		m_belongPlayerID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_ItemPrice:
		m_itemPrice = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_RTHP:
		m_rthp = param==NULL?0.0f:*((float*)param);
		break;
	case WCHDATA_ItemTalk:
		m_itemTalk = param==NULL?"":*((std::string*)param);
		break;
	case WCHDATA_ItemOption:
		m_itemOption = param==NULL?"":*((std::string*)param);
		break;
	case WCHDATA_ItemInfoText:
		m_itemInfoText = param==NULL?"":*((std::string*)param);
		break;
	//case WCHDATA_ExchangePlayerSet:
	//	if(param)
	//	{
	//		int playerid = *((int*)param);
	//		m_exchangePlayerSet.insert(playerid);
	//	}
	//	break;
	//case WCHDATA_NetStream:
	//case WCHDATA_PlayerStream:
	//	if(param)
	//	{
	//		ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
	//		unsigned char size;
	//		unsigned char i;
	//		int int1;
	//		crVector3i vec3;
	//		size = dataStream->_readUChar();
	//		m_saleItemMap.clear();
	//		for(i = 0; i<size; i++)
	//		{
	//			int1 = dataStream->_readInt();
	//			vec3 = dataStream->_readVec3i();
	//			m_saleItemMap.insert(std::make_pair(int1,vec3));
	//		}
	//		//m_dataStream = NULL;
	//	}
	//	break;
	}
}

void crRpgGameItemData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_DataType:
		param = &m_dataType;
		break;
	case WCHDATA_ItemState:
		param = &m_itemState;
		break;
	case WCHDATA_GuiseState:
		param = &m_guiseState;
		break;
	case WCHDATA_TouchDistance:
		param = &m_touchDistance;
		break;
	case WCHDATA_Zoffset:
		param = &m_zoffset;
		break;
	case WCHDATA_BelongPlayerID:
		param = &m_belongPlayerID;
		break;
	case WCHDATA_ItemPrice:
		param = &m_itemPrice;
		break;
	case WCHDATA_RTHP:
		param = &m_rthp;
		break;
	case WCHDATA_ItemIcon:
		param = &m_itemIcon;
		break;
	//case WCHDATA_EquipComposeRequestVec:
	//	param = &m_equipComposeRequestVec;
	//	break;
	//case WCHDATA_ComposeEquipID:
	//	param = &m_composeEquipID;
	//	break;
	//case WCHDATA_SaleItemMap:
	//	param = &m_saleItemMap;
	//	break;
	//case WCHDATA_ExchangePlayerSet:
	//	param = &m_exchangePlayerSet;
	//	break;
	case WCHDATA_UseItemProtectTime:
		param = &m_useItemProtectTime;
		break;
	case WCHDATA_MeshFileName:
		param = &m_meshFileName;
		break;
	case WCHDATA_AnimFileName:
		param = &m_animFileName;
		break;
	case WCHDATA_ItemTalk:
		param = &m_itemTalk;
		break;
	case WCHDATA_ItemOption:
		param = &m_itemOption;
		break;
	case WCHDATA_ItemInfoText:
		param = &m_itemInfoText;
		break;
	case WCHDATA_HalfPhysicsBox:
		param = &m_halfPhysicsBox;
		break;
	case WCHDATA_DefaultKey:
		param = &m_defaultkey;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameNodeData
//
/////////////////////////////////////////
crRpgGameNodeData::crRpgGameNodeData():
m_item(0)
{
}
crRpgGameNodeData::crRpgGameNodeData(const crRpgGameNodeData& data):
crNodeData(data),
m_item(data.m_item)
{
}
void crRpgGameNodeData::addParam(int i, const std::string& str)
{
}
void crRpgGameNodeData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_Item:
		m_item = (crInstanceItem *)param;
		break;
	}
}

void crRpgGameNodeData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_Item:
		param = m_item.get();
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameWeaponData
//
/////////////////////////////////////////
crRpgGameWeaponData::crRpgGameWeaponData():
m_item(0),
m_weaponSpeedScale(1.0f)
{
}
crRpgGameWeaponData::crRpgGameWeaponData(const crRpgGameWeaponData& data):
crNodeData(data),
m_item(data.m_item),
m_weaponActStr(data.m_weaponActStr),
m_weaponSpeedScale(data.m_weaponSpeedScale)
{
}
void crRpgGameWeaponData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_WeaponActStr:
		m_weaponActStr = relStr;
		break;
	case WCHDATA_WeaponSpeedScale:
		m_weaponSpeedScale = atof(relStr.c_str());
		break;
	}
}
void crRpgGameWeaponData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_Item:
		m_item = (crInstanceItem *)param;
		break;
	}
}

void crRpgGameWeaponData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_Item:
		param = m_item.get();
		break;
	case WCHDATA_WeaponActStr:
		param = &m_weaponActStr;
		break;
	case WCHDATA_WeaponSpeedScale:
		param = &m_weaponSpeedScale;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameSkillData
//
/////////////////////////////////////////
GNE::Mutex crRpgGameSkillData::m_saveMutex;
crRpgGameSkillData::crRpgGameSkillData():
m_dataType(DT_Skill),
m_skilllevel(0),
m_cdtime(0),
m_skilltype(ST_Initiative),
m_itemUseRange(10),
m_itemUseScope(0),
m_needMP(0),
m_needHP(0),
m_targetExtraID(0),
m_userExtraID(0),
m_targetType(Target_Bot),
m_belongPlayerID(0),
m_itemPrice(0),
m_useItemProtectTime(0),
m_attackAim(1.0f),
m_aimid(1),
m_defaultkey(0),
m_lastUseTime(0L),
m_duration(1.0f)
{
}
crRpgGameSkillData::crRpgGameSkillData(const crRpgGameSkillData& data):
crData(data),
m_dataType(data.m_dataType),
m_skilllevel(data.m_skilllevel),
m_cdtime(data.m_cdtime),
m_skilltype(data.m_skilltype),
m_itemUseRange(data.m_itemUseRange),
m_itemUseScope(data.m_itemUseScope),
m_physicsDM(data.m_physicsDM),
m_magicDM(data.m_magicDM),
m_needMP(data.m_needMP),
m_needHP(data.m_needHP),
m_targetExtraID(data.m_targetExtraID),
m_userExtraID(data.m_userExtraID),
m_targetType(data.m_targetType),
m_belongPlayerID(0),
m_itemPrice(data.m_itemPrice),
m_itemIcon(data.m_itemIcon),
m_useItemProtectTime(data.m_useItemProtectTime),
m_meshFileName(data.m_meshFileName),
m_animFileName(data.m_animFileName),
m_itemInfoText(data.m_itemInfoText),
m_aimid(data.m_aimid),
m_uiAimRange(data.m_uiAimRange),
m_attackAim(data.m_attackAim),
m_defaultkey(data.m_defaultkey),
m_lastUseTime(0L),
m_duration(data.m_duration)
{
}
void crRpgGameSkillData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_DataType:
		m_dataType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_SkillLevel:
		m_skilllevel = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_CDTime:
		m_cdtime = atof(relStr.c_str());
		break;
	case WCHDATA_SkillType:
		m_skilltype = (char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ItemUseRange:
		{
			float range = atof(relStr.c_str());
			range = CRCore::maximum(range,crGlobalHandle::gData()->gMinItemUseRange());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_itemUseRange = (unsigned short)range;
		}
		//m_itemUseRange = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ItemUseScope:
		{
			float range = atof(relStr.c_str());
			//range = CRCore::maximum(range,crGlobalHandle::gData()->gMinItemUseRange());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_itemUseScope = (short)range;
		}
		break;
	case WCHDATA_PhysicsDM:
		crArgumentParser::appAtoVec(relStr,m_physicsDM);
		break;
	case WCHDATA_MagicDM:
		crArgumentParser::appAtoVec(relStr,m_magicDM);
		break;
	case WCHDATA_NeedMP:
		m_needMP = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_NeedHP:
		m_needHP = (unsigned short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_TargetExtraID:
	//	m_targetExtraID = atoi(relStr.c_str());
	//	break;
	//case WCHDATA_UserExtraID:
	//	m_userExtraID = atoi(relStr.c_str());
	//	break;
	case WCHDATA_TargetType:
		m_targetType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ItemPrice:
		m_itemPrice = atoi(relStr.c_str());
		break;
	case WCHDATA_ItemIcon:
		m_itemIcon = relStr;
		break;
	case WCHDATA_UseItemProtectTime:
		m_useItemProtectTime = atof(relStr.c_str());
		break;
	case WCHDATA_MeshFileName:
		m_meshFileName = relStr;
		break;
	case WCHDATA_AnimFileName:
		m_animFileName = relStr;
		break;
	case WCHDATA_ItemInfoText:
		m_itemInfoText = relStr;
		break;
	case WCHDATA_AimID:
		m_aimid = (short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_UIAimRange:
		crArgumentParser::appAtoVec(relStr,m_uiAimRange);
		break;
	case WCHDATA_AttackAim:
		m_attackAim = atof(relStr.c_str());
		break;
	case WCHDATA_DefaultKey:
		m_defaultkey = (char)atoi(relStr.c_str());
		break;
	case WCHDATA_Duration:
		m_duration = atof(relStr.c_str());
		break;
	}
}
void crRpgGameSkillData::excHandle(_crInt64 msg)
{
	//if(LOINT64(msg) == WCH_BuildSaveStream)
	//{
	//	GNE::LockMutex lock( m_dataMutex );
	//	ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
	//	ItemMap itemMap;
	//	rcfg::ConfigScript cfg_script(&itemMap);
	//	char buf[128];
	//	sprintf(buf,"%d,%d\0",WCHDATA_SkillLevel,(int)m_skilllevel);
	//	cfg_script.Add("Param",buf);

	//	{
	//		GNE::LockMutex lock( m_saveMutex );
	//		sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
	//		cfg_script.Write(buf,"");
	//		cfg_script.Close();

	//		//stream = new crStreamBuf;
	//		stream->loadFromFile2(buf);
	//		stream->seekBegin();
	//		DeleteFileA(buf);
	//	}
	//}
	//else 
	if(LOINT64(msg) == WCH_BuildPlayerStream||LOINT64(msg) == WCH_BuildNetStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		//m_dataStream = new crStreamBuf;
		stream->createBuf(5);
        stream->_writeUChar(m_skilllevel);
		stream->_writeFloat(m_attackAim);
		stream->seekBegin();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crRpgGameSkillData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_DataType:
		m_dataType = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_SkillLevel:
		m_skilllevel = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_CDTime:
		m_cdtime = param==NULL?0:*((float*)param);
		break;
	case WCHDATA_SkillType:
		m_skilltype = param==NULL?0:*((char*)param);
		break;
	case WCHDATA_ItemUseRange:
		m_itemUseRange = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_ItemUseScope:
		m_itemUseScope = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_PhysicsDM:
		if(param) m_physicsDM = *((crVector2s*)param);
		break;
	case WCHDATA_MagicDM:
		if(param) m_magicDM = *((crVector2s*)param);
		break;
	case WCHDATA_NeedMP:
		m_needMP = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_NeedHP:
		m_needHP = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_TargetType:
		m_targetType = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_ItemInfoText:
		m_itemInfoText = param==NULL?"":*((std::string*)param);
		break;
	case WCHDATA_AttackAim:
		m_attackAim = param==NULL?NULL:*((float*)param);
		break;
	case WCHDATA_BelongPlayerID:
		m_belongPlayerID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_ItemPrice:
		m_itemPrice = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_UseItemProtectTime:
		m_useItemProtectTime = param?*((float*)param):NULL;
		break;
	case WCHDATA_LastUseTime:
		m_lastUseTime = param?*(_crInt64 *)param:0L;
		break;
	case WCHDATA_Duration:
		m_duration = param?*(float *)param:0.0f;
		break;
	case WCHDATA_UserExtraID:
		m_userExtraID = param?*(_crInt32 *)param:0.0f;
		break;
	case WCHDATA_TargetExtraID:
		m_targetExtraID = param?*(_crInt32 *)param:0.0f;
		break;
	//case WCHDATA_DataStream:
	//	if(param)
	//	{
	//		GNE::LockMutex lock( m_saveMutex );
	//		ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
	//		char file[128];
	//		sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
	//		dataStream->saveToFile2(file);
	//		rcfg::ConfigScript cfg_script;
	//		cfg_script.Open(file,"");
	//		std::string str;
	//		int nParam = 1;
	//		while (cfg_script.Get("Param", str,nParam++))
	//		{
	//			addParam(nParam-2,str);
	//		}
	//		cfg_script.Close();
	//		DeleteFileA(file);
	//		//m_dataStream = NULL;
	//	}
	//	break;
	case WCHDATA_NetStream:
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
            m_skilllevel = dataStream->_readUChar();
			m_attackAim = dataStream->_readFloat();
			//m_dataStream = NULL;
		}
		break;
	}
}

void crRpgGameSkillData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_DataType:
		param = &m_dataType;
		break;
	case WCHDATA_SkillLevel:
		param = &m_skilllevel;
		break;
	case WCHDATA_CDTime:
		param = &m_cdtime;
		break;
	case WCHDATA_SkillType:
		param = &m_skilltype;
		break;
	case WCHDATA_ItemUseRange:
		param = &m_itemUseRange;
		break;
	case WCHDATA_ItemUseScope:
		param = &m_itemUseScope;
		break;
	case WCHDATA_PhysicsDM:
		param = &m_physicsDM;
		break;
	case WCHDATA_MagicDM:
		param = &m_magicDM;
		break;
	case WCHDATA_NeedMP:
		param = &m_needMP;
		break;
	case WCHDATA_NeedHP:
		param = &m_needHP;
		break;
	case WCHDATA_TargetExtraID:
		param = &m_targetExtraID;
		break;
	case WCHDATA_UserExtraID:
		param = &m_userExtraID;
		break;
	case WCHDATA_TargetType:
		param = &m_targetType;
		break;
	case WCHDATA_BelongPlayerID:
		param = &m_belongPlayerID;
		break;
	case WCHDATA_ItemPrice:
		param = &m_itemPrice;
		break;
	case WCHDATA_ItemIcon:
		param = &m_itemIcon;
		break;
	case WCHDATA_UseItemProtectTime:
		param = &m_useItemProtectTime;
		break;
	case WCHDATA_MeshFileName:
		param = &m_meshFileName;
		break;
	case WCHDATA_AnimFileName:
		param = &m_animFileName;
		break;
	case WCHDATA_ItemInfoText:
		param = &m_itemInfoText;
		break;
	case WCHDATA_AimID:
		param = &m_aimid;
		break;
	case WCHDATA_UIAimRange:
		param = &m_uiAimRange;
		break;
	case WCHDATA_AttackAim:
		param = &m_attackAim;
		break;
	case WCHDATA_DefaultKey:
		param = &m_defaultkey;
		break;
	case WCHDATA_LastUseTime:
		param = &m_lastUseTime;
		break;
	case WCHDATA_Duration:
		param = &m_duration;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRpgGameExtraData
//
/////////////////////////////////////////
crRpgGameExtraData::crRpgGameExtraData()
{
}
crRpgGameExtraData::crRpgGameExtraData(const crRpgGameExtraData& data):
crData(data)
{
}
void crRpgGameExtraData::addParam(int i, const std::string& str)
{
}
void crRpgGameExtraData::excHandle(_crInt64 msg)
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
		crData::excHandle(getHandle(msg)); 
	}
}
void crRpgGameExtraData::inputParam(int i, void *param)
{
}
void crRpgGameExtraData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_ExtraIDMap:
		param = &m_extraIDMap;
		break;
	default:
		param = NULL;
		break;
	}
}
void crRpgGameExtraData::clear()
{
	GNE::LockMutex lock( m_dataMutex );
	crData::clear();
	m_extraIDMap.clear();
}
/////////////////////////////////////////
//
//crTalkChooseUIData
//
/////////////////////////////////////////
crTalkChooseUIData::crTalkChooseUIData():
m_buttonid(0),
m_textid(0)
{
}
crTalkChooseUIData::crTalkChooseUIData(const crTalkChooseUIData& data):
crUIData(data),
m_buttonid(data.m_buttonid),
m_textid(data.m_textid)
{
}
void crTalkChooseUIData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_ButtonID:
		{
			m_buttonid = (unsigned short)(atoi(relStr.c_str()));
		}
		break;
	case WCHDATA_TextID:
		{
			m_textid = (unsigned short)(atoi(relStr.c_str()));
		}
		break;
	}
}
void crTalkChooseUIData::excHandle(_crInt64 msg)
{
	crData::excHandle(getHandle(msg)); 
}
void crTalkChooseUIData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_ButtonID:
		m_buttonid = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_TextID:
		m_textid = param==NULL?NULL:*((unsigned short*)param);
		break;
	}
}
void crTalkChooseUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_ButtonID:
		param = &m_buttonid;
		break;
	case WCHDATA_TextID:
		param = &m_textid;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crBrainData
//
/////////////////////////////////////////
crBrainData::crBrainData():
m_currentFocusNode(NULL),
m_currentRoleTabIndex(0),
m_roleAttachToNode(NULL),
m_attachRotAngle(0.0f),
m_loadedNode(NULL)
{
	m_attachScale.set(1.0f,1.0f,1.0f);
}
crBrainData::crBrainData(const crBrainData& data):
crData(data),
m_currentFocusNode(data.m_currentFocusNode),
m_currentRoleTabIndex(data.m_currentRoleTabIndex),
m_roleAttachToNode(data.m_roleAttachToNode),
m_attachBoneName(data.m_attachBoneName),
m_attachOffset(data.m_attachOffset),
m_attachScale(data.m_attachScale),
m_attachRotAngle(data.m_attachRotAngle),
m_loadedNode(data.m_loadedNode)
{
}
void crBrainData::addParam(int i, const std::string& str)
{
}
void crBrainData::excHandle(_crInt64 msg)
{
	crData::excHandle(getHandle(msg)); 
}
void crBrainData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_CurrentFocusNode:
		m_currentFocusNode = (crNode*)param;
		break;
	case WCHDATA_CreateRoleTab:
		m_createRoleTab = (crTableIO*)param;
		break;
	case WCHDATA_AttachToNode:
		m_roleAttachToNode = (crNode*)param;
		break;
	case WCHDATA_AttachBoneName:
		if(param)
			m_attachBoneName = *(std::string*)param;
		else
			m_attachBoneName.clear();
		break;
	case WCHDATA_AttachOffset:
		m_attachOffset = param?*(crVector3*)param:crVector3();
		break;
	case WCHDATA_AttachScale:
		m_attachScale = param?*(crVector3*)param:crVector3(1.0f,1.0f,1.0f);
		break;
	case WCHDATA_AttachRotAngle:
		m_attachRotAngle = param?*(float*)param:0.0f;
		break;
	case WCHDATA_LoadedNode:
		m_loadedNode = (crNode*)param;
		break;
	}
}
void crBrainData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_CurrentFocusNode:
		param = m_currentFocusNode;
		break;
	case WCHDATA_CreateRoleTab:
		param = m_createRoleTab.get();
		break;
	case WCHDATA_CurrentRoleTabIndex:
		param = &m_currentRoleTabIndex;
		break;
	case WCHDATA_AttachToNode:
		param = m_roleAttachToNode;
		break;
	case WCHDATA_AttachBoneName:
		param = &m_attachBoneName;
		break;
	case WCHDATA_AttachOffset:
		param = &m_attachOffset;
		break;
	case WCHDATA_AttachScale:
		param = &m_attachScale;
		break;
	case WCHDATA_AttachRotAngle:
		param = &m_attachRotAngle;
		break;
	case WCHDATA_LoadedNode:
		param = m_loadedNode;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crCameraData
//
/////////////////////////////////////////
crCameraData::crCameraData():
	m_cameraMove(None),
	m_cameraRot(None),
	m_targetType(0),
	m_cameraBorrowTime(0.0f),
	m_cameraBorrowInterval(0.0f)
{
}
crCameraData::~crCameraData()
{

}
crCameraData::crCameraData(const crCameraData& data):
	crNodeData(data),
	m_cameraMove(data.m_cameraMove),
	m_cameraRot(data.m_cameraRot),
	m_targetType(0),
	m_cameraBorrowTime(data.m_cameraBorrowTime),
	m_cameraBorrowInterval(data.m_cameraBorrowInterval)
{
}
void crCameraData::addParam(int i, const std::string& str)
{
}
void crCameraData::excHandle(_crInt64 msg)
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
		crNodeData::excHandle(getHandle(msg)); 
	}
}
void crCameraData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_CameraMove:
		m_cameraMove = param?*(unsigned char*)param:0.0f;
		break;
	case WCHDATA_CameraRot:
		m_cameraRot = param?*(unsigned char*)param:0.0f;
		break;
	case WCHDATA_TargetType:
		m_targetType = param==NULL?NULL:*((unsigned char*)param);
		break;
	//case WCHDATA_TargetNode:
	//	m_targetNode = param==NULL?NULL:(crNode*)param;
	//	break;
	case WCHDATA_TargetItem:
		m_targetItem = param==NULL?NULL:(crInstanceItem*)param;
		break;
	case WCHDATA_TargetPos:
		m_targetPosition = param==NULL?m_targetPosition:*((crVector3*)param);
		break;
	case WCHDATA_MousePosRec:
		m_mousePosRec = param==NULL?crVector3i(0,0,0):*((crVector3i*)param);
		break;
	case WCHDATA_SelectItem:
		m_selectItem = param==NULL?NULL:(crInstanceItem*)param;
		break;
	case WCHDATA_CameraBorrowTime:
		m_cameraBorrowTime = param==NULL?NULL:*(float *)param;
		break;
	case WCHDATA_CameraBorrowInterval:
		m_cameraBorrowInterval = param==NULL?NULL:*(float *)param;
		break;
	//case WCHDATA_WaypointRoot:
	//	m_waypointroot = param==NULL?NULL:(crWaypoint*)param;
	//	break;
	}
}
void crCameraData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_CameraMove:
		param = &m_cameraMove;
		break;
	case WCHDATA_CameraRot:
		param = &m_cameraRot;
		break;
	case WCHDATA_TargetType:
		param = &m_targetType;
		break;
	//case WCHDATA_TargetNode:
	//	param = m_targetNode.get();
	//	break;
	case WCHDATA_TargetItem:
		param = m_targetItem.get();
		break;
	case WCHDATA_TargetPos:
		param = &m_targetPosition;
		break;
	case WCHDATA_MousePosRec:
		param = &m_mousePosRec;
		break;
	case WCHDATA_SelectMyRoleVec:
		param = &m_selectMyRoles;
		break;
	case WCHDATA_SelectItem:
		param = m_selectItem.get();
		break;
	case WCHDATA_CameraBorrowTime:
		param = &m_cameraBorrowTime;
		break;
	case WCHDATA_CameraBorrowInterval:
		param = &m_cameraBorrowInterval;
		break;
	//case WCHDATA_WaypointRoot:
	//	param = m_waypointroot.get();
	//	break;
	case WCHDATA_WaypointNodeMap:
		param = &m_waypointNodeMap;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crCommandDlgData
//
/////////////////////////////////////////
crCommandDlgData::crCommandDlgData():
	m_parentType(CDP_Widget),
	m_parent(NULL)
{
}
crCommandDlgData::crCommandDlgData(const crCommandDlgData& data):
	crUIData(data),
	m_parentType(CDP_Widget),
	m_parent(NULL)
{
}
void crCommandDlgData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_CommandDlgParentType:
		m_parentType = param?*(int*)param:NULL;
		//m_commandDlgWidget = param?((crWidgetNode*)param):NULL;
		//if(m_commandDlgWidget.valid())
		//	m_commandDlgPlayerGameData = NULL;
		break;
	case WCHDATA_CommandDlgParent:
		m_parent = param?(void*)param:NULL;
		//m_commandDlgPlayerGameData = param?((crPlayerGameData*)param):NULL;
		//if(m_commandDlgPlayerGameData.valid())
		//	m_commandDlgWidget = NULL;
		break;
	}
}
void crCommandDlgData::excHandle(_crInt64 msg)
{
	//if(m_commandDlgWidget.valid())
	//	m_commandDlgWidget->doEvent(msg);
	//else if(m_commandDlgPlayerGameData.valid())
	//	m_commandDlgPlayerGameData->doEvent(msg);
	if(m_parent)
	{
		switch (m_parentType)
		{
		case CDP_Widget:
			((crWidgetNode*)m_parent)->doEvent(msg);
			break;
		case CDP_PlayerGameData:
			((crPlayerGameData*)m_parent)->doEvent(msg);
			break;
		case CDP_InstanceItem:
			((crInstanceItem*)m_parent)->doEvent(msg);
			break;
		case CDP_Base:
			((crBase*)m_parent)->doEvent(msg);
			break;
		case CDP_DataObject:
			((crDataObject*)m_parent)->doEvent(msg);
			break;
		}
	}
}
/////////////////////////////////////////
//
//crSceneServerData
//
/////////////////////////////////////////
crSceneServerData::crSceneServerData()
{
}
crSceneServerData::crSceneServerData(const crSceneServerData& data):
	crData(data)
{
}
void crSceneServerData::addParam(int i, const std::string& str)
{
}
void crSceneServerData::excHandle(_crInt64 msg)
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
		crData::excHandle(getHandle(msg)); 
	}
}
void crSceneServerData::inputParam(int i, void *param)
{
}
void crSceneServerData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_UseItemRecordMap:
		param = &m_useItemRecordMap;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crRoomData
//
/////////////////////////////////////////
crRoomData::crRoomData():
	m_progress(0)
{
}
crRoomData::crRoomData(const crRoomData& data):
	crData(data),
	m_progress(data.m_progress)
{
}
void crRoomData::addParam(int i, const std::string& str)
{
}
void crRoomData::excHandle(_crInt64 msg)
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
		crData::excHandle(getHandle(msg)); 
	}
}
void crRoomData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_RoomProgress:
		m_progress = param?*(short*)param:0;
		break;
	}
}
void crRoomData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_RoomProgress:
		param = &m_progress;
		break;
	default:
		param = NULL;
		break;
	}
}