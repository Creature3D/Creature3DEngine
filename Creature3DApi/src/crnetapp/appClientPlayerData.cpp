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

#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStreamBuf.h>
#include <CRNet/crNetContainer.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crCookFile.h>
#include <CRCore/crArgumentParser.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRIOManager/crLoadManager.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crSceneView.h>
#include <CREncapsulation/crLoader.h>
#include <stdio.h>
#include <sys/stat.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace CRProducer;
using namespace CRUtil;
///////////////////////////////////
//
//crQueryGameListData
//
/////////////////////////////////////
crQueryGameListData::crQueryGameListData():
m_id(0),
m_rolecount(0)
{
}
void crQueryGameListData::setID(int id)
{
	m_id = id;
}
void crQueryGameListData::setGameName(const std::string &gamename)
{
    m_gamename = gamename;
}
void crQueryGameListData::setCName(const std::string &cname)
{
	m_cname = cname;
}
void crQueryGameListData::setPlatform(const std::string &platform)
{
	m_platform = platform;
}
//void crQueryGameListData::setUIFile(const std::string &uifile)
//{
//	m_uifile = uifile;
//}
//void crQueryGameListData::setClientCfg(const std::string &clientcfg)
//{
//    m_clientcfg = clientcfg;
//}
//void crQueryGameListData::setBaseFileList(const std::string &basefilelist)
//{
//    m_basefilelist = basefilelist;
//}
void crQueryGameListData::setRoleCount(unsigned char rolecount)
{
	m_rolecount = rolecount;
}
int crQueryGameListData::getID()
{
	return m_id;
}
const std::string& crQueryGameListData::getGameName() const
{
	return m_gamename;
}
const std::string& crQueryGameListData::getCName() const
{
	return m_cname;
}
const std::string& crQueryGameListData::getPlatform() const
{
	return m_platform;
}
//const std::string& crQueryGameListData::getUIFile() const
//{
//	return m_uifile;
//}
//const std::string& crQueryGameListData::getClientCfg() const
//{
//	return m_clientcfg;
//}
//const std::string& crQueryGameListData::getBaseFileList() const
//{
//	return m_basefilelist;
//}
unsigned char crQueryGameListData::getRoleCount()
{
	return m_rolecount;
}
std::string crQueryGameListData::getGameDir() const
{
	return m_gamename + "/";
}
std::string crQueryGameListData::getScriptDir() const
{
	return m_gamename + "/script/";
}
std::string crQueryGameListData::getTextDir() const
{
	return m_gamename + "/script/text/";
}
std::string crQueryGameListData::getTableDir() const
{
	return m_gamename + "/script/table/";
}
/////////////////////////////////////
////
////crQueryGameRoleListData
////
///////////////////////////////////////
//crQueryGameRoleListData::crQueryGameRoleListData():
//m_id(0),
//m_instanceitemid(0),
//m_metierid(0),
//m_sceneid(0),
//m_layerid(0),
//m_abstractitemid(0)
//{
//}
//void crQueryGameRoleListData::setID(int id)
//{
//	m_id = id;
//}
//void crQueryGameRoleListData::setInstanceItemID(int instanceitemid)
//{
//	m_instanceitemid = instanceitemid;
//}
//void crQueryGameRoleListData::setMetierID(int metierid)
//{
//    m_metierid = metierid;
//}
//void crQueryGameRoleListData::setRoleName(const std::string &rolename)
//{
//	m_rolename = rolename;
//}
//void crQueryGameRoleListData::setSceneID(int sceneid)
//{
//	m_sceneid = sceneid;
//}
//void crQueryGameRoleListData::setLayerID(unsigned short layerid)
//{
//	m_layerid = layerid;
//}
//void crQueryGameRoleListData::setSceneID(int abstractid)
//{
//	m_abstractitemid = abstractid;
//}
//void crQueryGameRoleListData::setStream(CRCore::crStreamBuf *stream)
//{
//	m_stream = stream;
//}
//int crQueryGameRoleListData::getID()
//{
//	return m_id;
//}
//int crQueryGameRoleListData::getInstanceItemID()
//{
//	return m_instanceitemid;
//}
//int crQueryGameRoleListData::getMetierID()
//{
//	return m_metierid;
//}
//const std::string& crQueryGameRoleListData::getRoleName() const
//{
//	return m_rolename;
//}
//int crQueryGameRoleListData::getSceneID()
//{
//	return m_sceneid;
//}
//unsigned short crQueryGameRoleListData::getLayerID()
//{
//    return m_layerid;
//}
//int crQueryGameRoleListData::getAbstractItemID()
//{
//	return m_abstractitemid;
//}
//CRCore::crStreamBuf* crQueryGameRoleListData::getStream()
//{
//	return m_stream.get();
//}
///////////////////////////////////
//
//crQuerySceneInfoData
//
/////////////////////////////////////
crQuerySceneInfoData::crQuerySceneInfoData():
m_sceneid(0),
m_externSceneType(0)
{
}
void crQuerySceneInfoData::setSceneID(int sceneid)
{
    m_sceneid = sceneid;
}
void crQuerySceneInfoData::setSceneName(const std::string &scenename)
{
	m_scenename = scenename;
}
void crQuerySceneInfoData::setFileName(const std::string &filename)
{
	m_filename = filename;
}
void crQuerySceneInfoData::setUIScene(const std::string &uiscene)
{
	m_uiscene = uiscene;
}
void crQuerySceneInfoData::setCollideFile(const std::string &collidefile)
{
	m_collidefile = collidefile;
}
void crQuerySceneInfoData::setBrainFile(const std::string &brainfile)
{
	m_brainfile = brainfile;
}
void crQuerySceneInfoData::setExternSceneType(int scenetype)
{
	m_externSceneType = scenetype;
}
int crQuerySceneInfoData::getSceneID()
{
	return m_sceneid;
}
const std::string& crQuerySceneInfoData::getSceneName() const
{
    return m_scenename;
}
const std::string& crQuerySceneInfoData::getFileName() const
{
	return m_filename;
}
const std::string& crQuerySceneInfoData::getUIScene() const
{
	return m_uiscene;
}
const std::string& crQuerySceneInfoData::getCollideFile()
{
	if(m_collidefile.empty())
	{
		if(!m_filename.empty())
			m_collidefile = crArgumentParser::getFileNameEliminateExt(m_filename)+"collide.cfg";
	}
	return m_collidefile;
}
const std::string& crQuerySceneInfoData::getBrainFile()
{
	if(m_brainfile.empty())
	{
		if(!m_filename.empty())
			m_brainfile = "brain/"+crArgumentParser::getFileNameEliminateExt(m_filename)+".cfg";
	}
	return m_brainfile;
}
int crQuerySceneInfoData::getExternSceneType()
{
	return m_externSceneType;
}
///////////////////////////////////
//
//crMyPlayerData
//
/////////////////////////////////////
ref_ptr<crMyPlayerData> crMyPlayerData::m_instance = NULL;
crMyPlayerData::crMyPlayerData():
m_exitCode(0),
m_playerID(0),
m_roomid(0),
m_mapscale(1.0f),
//m_currentDialogData(NULL),
//m_saleDialogEmporium(false),
//m_iTransport(false),
//m_loginGameSceneMode(0),
m_currentQueryRoleIndex(0),
m_currentRoleID(0),
m_loadmainrolecase(1),
m_removedt(0.0f),
m_lastServerID(0),
m_webserverid(0),
m_isp(1)
{
	crGlobalData *gdata = crGlobalHandle::gData();
	if(gdata)
	{
		CRIOManager::makeDirectoryForFile(gdata->gGameChatRecFile());
		CRIOManager::makeDirectoryForFile(gdata->gChatMsgRecFile());
		initGameChatRec();
		initChatMsgRec();
	}
}
crMyPlayerData::~crMyPlayerData()
{
}
crMyPlayerData* crMyPlayerData::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crMyPlayerData;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crMyPlayerData::initGameChatRec()
{
	m_gameChatRec = new crStreamBuf;
	m_gameChatRec->createBuf(1024);
	std::string cookfilename = crGlobalHandle::gData()->gGameChatRecFile();
	CRIOManager::GetCookedFileName(cookfilename);
	DeleteFileA(cookfilename.c_str());
}
void crMyPlayerData::recGameChat(const std::string &str)
{
	int size = str.length() + 4;
	m_gameChatRec->_writeString(str);
	if(m_gameChatRec->getBufSize()>crGlobalHandle::gData()->gMaxRecBufSize())
	{
		saveGameChat();
	}
}
void crMyPlayerData::saveGameChat()
{
	if(m_gameChatRec.valid() && m_gameChatRec->getBufSize()>0)
	{
		m_gameChatRec->seekBegin();
		CRIOManager::crLoadCookFile scopedLoad(crGlobalHandle::gData()->gGameChatRecFile());
		m_gameChatRec->saveToFile2(crGlobalHandle::gData()->gGameChatRecFile(),true);
		CRIOManager::crWriteCookFile scopedWrite(crGlobalHandle::gData()->gGameChatRecFile());

		m_gameChatRec = new crStreamBuf;
		m_gameChatRec->createBuf(1024);
	}
}
void crMyPlayerData::initChatMsgRec()
{
	m_chatMsgRec = new crStreamBuf;
	m_chatMsgRec->createBuf(1024);
	std::string cookfilename = crGlobalHandle::gData()->gChatMsgRecFile();
	CRIOManager::GetCookedFileName(cookfilename);
	DeleteFileA(cookfilename.c_str());
}
void crMyPlayerData::recChatMsg(const std::string &str)
{
	int size = str.length() + 4;
	m_chatMsgRec->_writeString(str);
	if(m_chatMsgRec->getBufSize()>crGlobalHandle::gData()->gMaxRecBufSize())
	{
		saveChatMsg();
	}
}
void crMyPlayerData::saveChatMsg()
{
	if(m_chatMsgRec.valid() && m_chatMsgRec->getBufSize()>0)
	{
		m_chatMsgRec->seekBegin();
		CRIOManager::crLoadCookFile scopedLoad(crGlobalHandle::gData()->gChatMsgRecFile());
		m_chatMsgRec->saveToFile2(crGlobalHandle::gData()->gChatMsgRecFile(),true);
		CRIOManager::crWriteCookFile scopedWrite(crGlobalHandle::gData()->gChatMsgRecFile());

		m_chatMsgRec = new crStreamBuf;
		m_chatMsgRec->createBuf(1024);
	}
}
void crMyPlayerData::setExitCode(unsigned char exitCode)
{
	m_exitCode = exitCode;
}
unsigned char crMyPlayerData::getExitCode()
{
	return m_exitCode;
}
void crMyPlayerData::clear()
{
	m_gameRoleMapMutex.lock();
	for( GameRoleMap::iterator itr = m_gameRoleMap.begin();
		itr != m_gameRoleMap.end();
		++itr )
	{
		itr->second->clearExtra();
	}
	m_gameRoleMapMutex.unlock();
	saveGameChat();
	saveChatMsg();
	//if(m_exitCode == EC_ExitGame)
	//{//完全退出
	//	m_myChatData = NULL;//保存聊天数据
	//	m_instance = NULL;
	//}
	//else
	{//退出到角色选择界面
		//m_npc = NULL;
		m_myRoleNpcMapMutex.acquire();
		for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
			itr != m_myRoleNpcMap.end();
			++itr )
		{
			itr->second.first->clearExtra();
			itr->second.first->clearData();
			//	itr->second.second = (CREncapsulation::crNPC *)NULL;
		}
		m_myRoleNpcMap.clear();
		m_myRoleNpcMapMutex.release();
		clearSelectedGameRoles();

        m_inRangePlayerMapMutex.lock();
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			itr->second.first->clearExtra();
			itr->second.first->clearData();
		}
		m_inRangePlayerMap.clear();
		m_inRangePlayerMapMutex.unlock();
		
		m_inRangeNpcMapMutex.lock();
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			itr->second.first->clearExtra();
			itr->second.first->clearData();
		}
		m_inRangeNpcMap.clear();
		m_inRangeNpcMapMutex.unlock();

		m_inRangeItemMapMutex.lock();
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			itr != m_inRangeItemMap.end();
			++itr )
		{
			itr->second.first->clearExtra();
			itr->second.first->clearData();
		}

		m_outRangePlayerMapMutex.lock();
		for( OutRangePlayerMap::iterator itr = m_outRangePlayerMap.begin();
			itr != m_outRangePlayerMap.end();
			++itr )
		{
			itr->second.first.first->clearData();
		}
		m_outRangePlayerMap.clear();
		m_outRangePlayerMapMutex.unlock();

		m_downloadItemMapMutex.lock();
		//for( DownloadItemDeque::iterator itr = m_downloadItemDeque.begin();
		//	 itr != m_downloadItemDeque.end();
		//	 ++itr )
		//{
		//	(*itr)->setDataClass(NULL);
		//}
		m_downloadItemMap.clear();
		m_downloadItemMapMutex.unlock();

		m_downloadItemChildMapMutex.lock();
		m_downloadItemChildMap.clear();
		m_downloadItemChildMapMutex.unlock();

		m_outRangeNpcMapMutex.lock();
		for( OutRangeNpcMap::iterator itr = m_outRangeNpcMap.begin();
			itr != m_outRangeNpcMap.end();
			++itr )
		{
			itr->second.first.first->clearData();
		}
		m_outRangeNpcMap.clear();
		m_outRangeNpcMapMutex.unlock();

		m_inRangeItemMap.clear();
		m_inRangeItemMapMutex.unlock();

		m_outRangeItemMapMutex.lock();
		for( OutRangeItemMap::iterator itr = m_outRangeItemMap.begin();
			itr != m_outRangeItemMap.end();
			++itr )
		{
			itr->second.first.first->clearData();
		}
		m_outRangeItemMap.clear();
		m_outRangeItemMapMutex.unlock();

		if(m_exitCode != EC_EnterRoom)
		{
			m_roomMapMutex.lock();
			m_selectedRoom = NULL;
			m_roomMap.clear();
			m_roomMapMutex.unlock();
		}
		//if(m_role.valid()) m_role->clientRelease();
	}
	m_scene = NULL;
	//m_sceneInfo = NULL;
	//m_loginGameSceneMode = 0;
	////
	m_currentQueryRoleIndex = 0;
	m_currentRoleID = 0;
	clearEyePoint();
	clearItemVisiableMap();

	///切换场景前清理所有item
	for( RemovedItemList::iterator itr = m_removedItemList.begin(); itr != m_removedItemList.end(); ++itr)
	{
		(*itr)->clearData();
	}
	m_removedItemList.clear();

	//m_playerGameData = NULL;
	//if(m_playerGameData.valid())
	//{
	//	void *param;
	//	ref_ptr<crStreamBuf> playerStream;
	//	crData *olddata = m_playerGameData->getDataClass();
	//	olddata->excHandle(MAKEINT64(WCH_LockData,1));
	//	olddata->excHandle(MAKEINT64(WCH_BuildDataStream,PlayerStream));
	//	olddata->getParam(WCHDATA_DataStream,param);
	//	if(param)
	//	{
	//		playerStream = *((ref_ptr<crStreamBuf>*)param);
	//		playerStream->seekBegin();
	//	}
	//	olddata->excHandle(MAKEINT64(WCH_LockData,0));

	//	std::string script = crMyPlayerData::getInstance()->getSelectedGame()->getGameName() + "/playergamedata.cfg";
	//	ref_ptr<crData> data = CREncapsulation::loadData(script);
	//	if(data.valid())
	//	{
	//		data->inputParam(WCHDATA_PlayerStream,playerStream.get());
	//	}

	//	m_playerGameData->setDataClass(data.get());
	//	crRole *mainrole = m_playerGameData->getMainRole();
	//	if(mainrole)
	//	{
	//		olddata = mainrole->getDataClass();
	//		olddata->excHandle(MAKEINT64(WCH_LockData,1));
	//		olddata->excHandle(MAKEINT64(WCH_BuildDataStream,PlayerStream));
	//		olddata->getParam(WCHDATA_DataStream,param);
	//		if(param)
	//		{
	//			playerStream = *((ref_ptr<crStreamBuf>*)param);
	//			playerStream->seekBegin();
	//		}
	//		olddata->excHandle(MAKEINT64(WCH_LockData,0));

	//		script = mainrole->getAbstractItem()->getScript();
	//		data = CREncapsulation::loadData(script);
	//		if(data.valid())
	//		{
	//			data->inputParam(WCHDATA_PlayerStream,playerStream.get());
	//		}
	//		mainrole->setDataClass(data.get());

	//		//olddata = mainrole->getMetierDataClass();
	//		//olddata->excHandle(MAKEINT64(WCH_LockData,1));
	//		//olddata->excHandle(MAKEINT64(WCH_BuildDataStream,PlayerStream));
	//		//olddata->getParam(WCHDATA_DataStream,param);
	//		//if(param)
	//		//{
	//		//	playerStream = *((ref_ptr<crStreamBuf>*)param);
	//		//	playerStream->seekBegin();
	//		//}
	//		//olddata->excHandle(MAKEINT64(WCH_LockData,0));

	//		//script = mainrole->getAbstractMetier()->getMetierScript();
	//		//data = CREncapsulation::loadData(script);
	//		//if(data.valid())
	//		//{
	//		//	data->inputParam(WCHDATA_PlayerStream,playerStream.get());
	//		//}
	//		//mainrole->setMetierDataClass(data.get());
	//	}
	//}
}
void crMyPlayerData::setPlayerID(int playerid)
{
	m_playerID = playerid;
}
int crMyPlayerData::getPlayerID()
{
	return m_playerID;
}
void crMyPlayerData::setUserName(const std::string &username)
{
	m_username = username;
}
const std::string &crMyPlayerData::getUserName()
{
	return m_username;
}
void crMyPlayerData::setLastServerID(int serverid)
{
	m_lastServerID = serverid;
}
int crMyPlayerData::getLastServerID()
{
	return m_lastServerID;
}
void crMyPlayerData::setWebServerID(int webserverid)
{
	m_webserverid = webserverid;
}
int crMyPlayerData::getWebServerID()
{
	return m_webserverid;
}
void crMyPlayerData::setISP(int isp)
{
	m_isp = isp;
}
int crMyPlayerData::getISP()
{
	return m_isp;
}
void crMyPlayerData::setServerName(const std::string &serverName)
{
	m_serverName = serverName;
}
const std::string& crMyPlayerData::getServerName()
{
	return m_serverName;
}
void crMyPlayerData::setSite(const std::string &site)
{
	m_site = site;
}
const std::string& crMyPlayerData::getSite()
{
	return m_site;
}
//void crMyPlayerData::setPlayerName(const std::string &playerName)
//{
//	m_playerName = playerName;
//}
//const std::string& crMyPlayerData::getPlayerName()
//{
//	return m_playerName;
//}
std::string crMyPlayerData::getCharacterName()
{
	crRole *role = m_playerGameData->getMainRole();
	if(!role)
		role = getCurrentRole();
	return role?role->getIName2():"";
}
void crMyPlayerData::setKey(const std::string &key)
{
	m_key = key;
}
const std::string &crMyPlayerData::getKey() const
{
	return m_key;
}

void crMyPlayerData::setMyChatData(crNetChatData *myChatData)
{
	m_myChatData = myChatData;
	char file[32];
	sprintf(file,"rec/group/%d.cfg\0",getPlayerID());
	CRIOManager::makeDirectoryForFile(file);
    sprintf(file,"rec/%d/%d.cfg\0",getPlayerID(),getPlayerID());
	CRIOManager::makeDirectoryForFile(file);
}

crNetChatData *crMyPlayerData::getMyChatData()
{
	return m_myChatData.get();
}

///GameList
void crMyPlayerData::addGame(crQueryGameListData *game)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameMapMutex);
	m_gameMap[game->getID()] = game;
}
crQueryGameListData *crMyPlayerData::getGame(int gameid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameMapMutex);
	GameMap::iterator itr = m_gameMap.find(gameid);
	return itr!=m_gameMap.end()?itr->second.get():NULL;
}
void crMyPlayerData::clearGameMap()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameMapMutex);
	m_gameMap.clear();
}
void crMyPlayerData::lockGameMap()
{
	m_gameMapMutex.lock();
}
GameMap &crMyPlayerData::getGameMap()
{
	return m_gameMap;
}
void crMyPlayerData::unlockGameMap()
{
	m_gameMapMutex.unlock();
}
void crMyPlayerData::setSelectedGame(crQueryGameListData *game)
{
	m_selectedGame = game;
	if(m_selectedGame.valid())
		crGlobalHandle::getInstance()->setGameName(m_selectedGame->getGameName());
	else
		crGlobalHandle::getInstance()->setGameName("");
}
crQueryGameListData *crMyPlayerData::getSelectedGame()
{
	return m_selectedGame.get();
}
//void crMyPlayerData::setGameState(unsigned int gamestate)
//{
//	m_gameState = (GameState)gamestate;
//}
//unsigned int crMyPlayerData::getGameState()
//{
//	return m_gameState;
//}
void crMyPlayerData::addGameRole(crRole *role)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameRoleMapMutex);
	m_gameRoleMap[role->getRoleID()] = role;
}
crRole *crMyPlayerData::getGameRole(int roleid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameRoleMapMutex);
	GameRoleMap::iterator itr = m_gameRoleMap.find(roleid);
	return itr!=m_gameRoleMap.end()?itr->second.get():NULL;
}
void crMyPlayerData::removeGameRole(int roleid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameRoleMapMutex);
	GameRoleMap::iterator itr = m_gameRoleMap.find(roleid);
	if(itr != m_gameRoleMap.end())
	{
		crRole *role = itr->second.get();
		crNode *node = role->getNode();
		crGroup *parent = node->getParent(0);
		if(parent)
			CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,node);
		m_gameRoleMap.erase(itr);
		RoleMap::iterator ritr = m_selectedGameRoles.find(roleid);
		if(ritr != m_selectedGameRoles.end())
		{
			m_selectedGameRoles.erase(roleid);
			if(m_selectedGameRoles.empty() && !m_gameRoleMap.empty())
			{
				addSelectedGameRole(m_gameRoleMap.begin()->second.get());
			}
		}
		//if(role== m_selectedGameRole.get())
		//{
		//	m_selectedGameRole = NULL;
		//	if(!m_gameRoleMap.empty())
		//	{
		//		setSelectedGameRole(m_gameRoleMap.begin()->second.get());
		//	}
		//}
	}
}
void crMyPlayerData::clearGameRoleMap()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameRoleMapMutex);
	m_gameRoleMap.clear();
}
void crMyPlayerData::lockGameRoleMap()
{
	m_gameRoleMapMutex.lock();
}
GameRoleMap &crMyPlayerData::getGameRoleMap()
{
	return m_gameRoleMap;
}
int crMyPlayerData::getGameRoleCount()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_gameRoleMapMutex);
	return m_gameRoleMap.size();
}
void crMyPlayerData::unlockGameRoleMap()
{
	m_gameRoleMapMutex.unlock();
}
void crMyPlayerData::clearSelectedGameRoles()
{
	m_selectedGameRoles.clear();
}
bool crMyPlayerData::setSelectedGameRole(crRole *role)
{
	//m_selectedGameRole = role;
	m_selectedGameRoles.clear();
	return addSelectedGameRole(role);
}
crRole *crMyPlayerData::getSelectedGameRole()
{
	//return m_selectedGameRole.get();
	return m_selectedGameRoles.empty()?NULL:m_selectedGameRoles.begin()->second.get();
}
bool crMyPlayerData::addSelectedGameRole(crRole *role)
{
	if(m_playerGameData.valid() && m_playerGameData->getMainRoleID() == role->getRoleID())
	{//
		//CRCore::notify(CRCore::WARN)<<"crMyPlayerData::addSelectedGameRole 不能选择MainRole"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"crMyPlayerData::addSelectedGameRole 不能选择MainRole\n\0");
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return false;
	}
	if(role && m_selectedGameRoles.size()<crGlobalHandle::gData()->gMaxRoleCanJoin())
	{
		m_selectedGameRoles[role->getRoleID()] = role;
		return true;
	}
	return false;
}
crMyPlayerData::RoleMap &crMyPlayerData::getSelectedGameRoles()
{
	return m_selectedGameRoles;
}
void crMyPlayerData::removeSelectedGameRole(int roleid)
{
	m_selectedGameRoles.erase(roleid);
}
crRole *crMyPlayerData::getSelectedGameRoleByIndex(unsigned int index)
{
	if(index < m_selectedGameRoles.size())
	{
		unsigned int i = 0;
		RoleMap::iterator itr;
		for( itr = m_selectedGameRoles.begin();
			itr != m_selectedGameRoles.end() && i<index;
			++itr,++i);
		return itr->second.get();
	}
	return NULL;
}
crRole *crMyPlayerData::queryFirstRole()
{
	m_currentQueryRoleIndex = 0;
	return m_selectedGameRoles.empty()?NULL:m_selectedGameRoles.begin()->second.get();
}
crRole *crMyPlayerData::queryNextRole()
{
	if(m_currentQueryRoleIndex+1<m_selectedGameRoles.size())
	{
		m_currentQueryRoleIndex++;
		return getSelectedGameRoleByIndex(m_currentQueryRoleIndex);
	}
	return NULL;
}
crRole *crMyPlayerData::getCurrentQueryRole()
{
	return getSelectedGameRoleByIndex(m_currentQueryRoleIndex);
}
bool crMyPlayerData::hasSelectedGameRole()
{
	return !m_selectedGameRoles.empty();
}
void crMyPlayerData::setSceneInfoData(crQuerySceneInfoData *sceneInfo)
{
	m_sceneInfo = sceneInfo;
}
crQuerySceneInfoData *crMyPlayerData::getSceneInfoData()
{
	return m_sceneInfo.get();
}
void crMyPlayerData::createScene()
{
	if(!m_scene.valid())
		m_scene = new crScene;
	m_scene->setSceneID(m_sceneInfo->getSceneID());
	m_scene->setSceneName(m_sceneInfo->getSceneName());
	m_scene->setFileName(m_sceneInfo->getFileName());
	m_scene->setCollideFile(m_sceneInfo->getCollideFile());
	m_scene->setBrainFile(m_sceneInfo->getBrainFile());
	m_scene->setExternSceneType(m_sceneInfo->getExternSceneType());
	int runmode = crDisplaySettings::instance()->getRunMode();
	if(runmode == crDisplaySettings::RobotNoTexture)
		crDisplaySettings::instance()->setRunMode(crDisplaySettings::Robot);
	m_scene->buildSceneLayer();
	crDisplaySettings::instance()->setRunMode(runmode);
}
void crMyPlayerData::createScene(int sceneid, const std::string &sceneName, const std::string &fileName, const std::string &collideFile, const std::string &brainFile)
{
	if(!m_scene.valid())
		m_scene = new crScene;
	m_scene->setSceneID(sceneid);
	m_scene->setSceneName(sceneName);
	m_scene->setFileName(fileName);
	m_scene->setCollideFile(collideFile);
	m_scene->setBrainFile(brainFile);
	int runmode = crDisplaySettings::instance()->getRunMode();
	if(runmode == crDisplaySettings::RobotNoTexture)
		crDisplaySettings::instance()->setRunMode(crDisplaySettings::Robot);
	m_scene->buildSceneLayer();
	crDisplaySettings::instance()->setRunMode(runmode);
}
CRNetApp::crScene *crMyPlayerData::getScene()
{
	return m_scene.get();
}
crSceneLayer *crMyPlayerData::getSceneLayer()
{
	if(m_scene.valid() && getCurrentRole())
	{
		return m_scene->getSceneLayer(getCurrentRole()->getLayerID());
	}
	return NULL;
}
void crMyPlayerData::addRole(crRole *role)
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	//if(m_iTransport && !m_roomMap.empty()/* && m_role.valid() && m_role->getPlayerID() == role->getPlayerID() && m_role->getRoleID() == role->getRoleID()*/)
	//{
	//	crRole *mRole = getRole(role->getRoleID());
	//	if(mRole)
	//		role->setPathFinder(mRole->getPathFinder());
	//}
	m_myRoleNpcMap[role->getRoleID()] = std::make_pair(role,(CREncapsulation::crNPC *)NULL);
}
crRole *crMyPlayerData::getRole(int roleid)
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	MyRoleNpcMap::iterator itr = m_myRoleNpcMap.find(roleid);
	return itr != m_myRoleNpcMap.end()?itr->second.first.get():NULL;
}
crRole *crMyPlayerData::getCurrentRole()
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	if(m_myRoleNpcMap.empty())
		return NULL;
	if(m_currentRoleID==0)
	{
		MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
		m_currentRoleID = itr->first;
		return itr->second.first.get();
	}
	return getRole(m_currentRoleID);
}
crRole *crMyPlayerData::getLoadedRoleByIndex(unsigned int index)
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	if(index < m_myRoleNpcMap.size())
	{
		unsigned int i = 0;
		MyRoleNpcMap::iterator itr;
		for( itr = m_myRoleNpcMap.begin();
			itr != m_myRoleNpcMap.end() && i<index;
			++itr,++i);
		return itr->second.first.get();
	}
	return NULL;
}
int crMyPlayerData::getLoadedRoleIndex(int roleid)
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	int i = 0;
	MyRoleNpcMap::iterator itr;
	for( itr = m_myRoleNpcMap.begin();
		 itr != m_myRoleNpcMap.end();
		 ++itr,++i)
	{
		if(roleid == itr->first)
			return i;
	}
	return 0;
}
void crMyPlayerData::setCurrentRoleID(int roleid)
{
	m_currentRoleID = roleid;
}
int crMyPlayerData::getCurrentRoleID()
{
	GNE::LockMutex lock(m_myRoleNpcMapMutex);
	if(m_currentRoleID == 0 && !m_myRoleNpcMap.empty())
	{
		MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
		m_currentRoleID = itr->first;
	}
	return m_currentRoleID;
}
bool crMyPlayerData::ifItemIsMe(crInstanceItem *item)
{
	return item && item->getItemtype()==crInstanceItem::Role && (item->getID() == m_playerID || (item->getID()<0&&item->getIName()==getCharacterName()));
}
bool crMyPlayerData::ifNodeIsMe(CRCore::crNode *relNode)
{
	if(relNode)
	{
		crData *data = relNode->getDataClass();
		if(data)
		{
			void *param;
			data->getParam(WCHDATA_Item,param);
			if(param)
			{
				crInstanceItem *item = (crInstanceItem *)param;
				return ifItemIsMe(item);
			}
		}
	}
	return false;
}
void crMyPlayerData::lockMyRoleNpcMap()
{
	m_myRoleNpcMapMutex.acquire();
}
crMyPlayerData::MyRoleNpcMap &crMyPlayerData::getMyRoleNpcMap()
{
	return m_myRoleNpcMap;
}
void crMyPlayerData::unlockMyRoleNpcMap()
{
	m_myRoleNpcMapMutex.release();
}
//crRole *crMyPlayerData::getRole()
//{
//	//if(!m_role.valid())
//	//	CRCore::notify(CRCore::WARN)<<"crMyPlayerData::getRole() No role"<< std::endl;
//	return m_role.get();
//}
//void crMyPlayerData::setRole(crRole *role)
//{
//	if(m_iTransport && m_role.valid() && m_role->getPlayerID() == role->getPlayerID() && m_role->getRoleID() == role->getRoleID())
//	{
//		role->setPathFinder(m_role->getPathFinder());
//	}
//	m_role = role;
//}
//crRole *crMyPlayerData::getRole()
//{
//	//if(!m_role.valid())
//	//	CRCore::notify(CRCore::WARN)<<"crMyPlayerData::getRole() No role"<< std::endl;
//	return m_role.get();
//}
//void crMyPlayerData::setNPC(CREncapsulation::crNPC *npc)
//{
//	m_npc = npc;
//}
//CREncapsulation::crNPC *crMyPlayerData::getNPC()
//{
//	return m_npc.get();
//}
//crMyPlayerData::PlayerMap &crMyPlayerData::getPlayerMap()
//{
//	return m_playerMap;
//}
bool crMyPlayerData::findInRangePlayer(int playerid,int roleid, RoleNpcPair &roleNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
	InRangePlayerMap::iterator itr = m_inRangePlayerMap.find(MAKEINT64(playerid,roleid));
	if(itr != m_inRangePlayerMap.end())
	{
		roleNpcPair = itr->second;
		return true;
	}
	return false;
}
void crMyPlayerData::insertInRangePlayer(crRole *player,crNPC *npc)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
	crMyPlayerData::RoleNpcPair roleNpcPair;
	if(findInRangePlayer(player->getPlayerID(),player->getRoleID(),roleNpcPair))
	{
		if(roleNpcPair.first != player || roleNpcPair.second != npc)
		{
			crNode *relNode = roleNpcPair.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
			roleNpcPair.first->clearSyncPos();
			roleNpcPair.first->clearExtra();
			insertOutRangePlayer(roleNpcPair.first.get(),roleNpcPair.second.get());
			m_inRangePlayerMap[MAKEINT64(player->getPlayerID(),player->getRoleID())] = RoleNpcPair(player,npc);
		}
	}
	else
	{
		m_inRangePlayerMap[MAKEINT64(player->getPlayerID(),player->getRoleID())] = RoleNpcPair(player,npc);
	}
	player->clearSyncPos();
}
void crMyPlayerData::insertInRangePlayer(RoleNpcPair &roleNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
	m_inRangePlayerMap[MAKEINT64(roleNpcPair.first->getPlayerID(),roleNpcPair.first->getRoleID())] = roleNpcPair;
}
void crMyPlayerData::removeInRangeRole(int playerid,int roleid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
	InRangePlayerMap::iterator itr = m_inRangePlayerMap.find(MAKEINT64(playerid,roleid));
	if(itr!=m_inRangePlayerMap.end())
	{
        insertOutRangePlayer(itr->second.first.get(),itr->second.second.get());
		crNode *relNode = itr->second.first->getRelNode();
		if(relNode)
		{
			relNode->setVisiable(false);
			relNode->setEnableIntersect(false);
		}
		itr->second.first->clearSyncPos();
		itr->second.first->clearExtra();
		m_inRangePlayerMap.erase(itr);
	}
}
void crMyPlayerData::removeInRangePlayer(int playerid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
	crNode *relNode;
	for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
	    itr != m_inRangePlayerMap.end();)
	{
		if(itr->second.first->getPlayerID() == playerid)
		{
			//itr->second.second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));//隐藏
			relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
			itr->second.first->clearSyncPos();
			itr->second.first->clearExtra();
			insertOutRangePlayer(itr->second.first.get(),itr->second.second.get());
			itr = m_inRangePlayerMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
void crMyPlayerData::lockInRangePlayerMap()
{
	m_inRangePlayerMapMutex.lock();
}
crMyPlayerData::InRangePlayerMap &crMyPlayerData::getInRangePlayerMap()
{
	return m_inRangePlayerMap;
}
void crMyPlayerData::unlockInRangePlayerMap()
{
	m_inRangePlayerMapMutex.unlock();
}
bool crMyPlayerData::findOutRangePlayer(int playerid,int roleid,RoleNpcPair& roleNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangePlayerMapMutex);
	OutRangePlayerMap::iterator itr = m_outRangePlayerMap.find(MAKEINT64(playerid,roleid));
	if(itr != m_outRangePlayerMap.end())
	{
		roleNpcPair = itr->second.first;
		return true;
	}
	return false;
}
void crMyPlayerData::insertOutRangePlayer(crRole *player,crNPC *npc)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangePlayerMapMutex);
	m_outRangePlayerMap.insert(std::make_pair(MAKEINT64(player->getPlayerID(),player->getRoleID()),PlayerTimePair(RoleNpcPair(player,npc),0.0f)));
}
void crMyPlayerData::removeOutRangePlayer(int playerid,int roleid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangePlayerMapMutex);
	m_outRangePlayerMap.erase(MAKEINT64(playerid,roleid));
}
void crMyPlayerData::removeOutRangePlayerInNextFrame(int playerid,int roleid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangePlayerMapMutex);
	OutRangePlayerMap::iterator itr = m_outRangePlayerMap.find(MAKEINT64(playerid,roleid));
	if(itr != m_outRangePlayerMap.end())
	{
		itr->second.second = crGlobalHandle::gData()->gRemoveOutRangeTime();
	}
}
void crMyPlayerData::insertDownloadItem(short id,crInstanceItem *item)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemMapMutex);
	m_downloadItemMap[id] = item;
}
crInstanceItem *crMyPlayerData::findDownloadItem(short id)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemMapMutex);
	DownloadItemMap::iterator itr = m_downloadItemMap.find(id);
	return itr != m_downloadItemMap.end()?itr->second.get():NULL;
}
void crMyPlayerData::removeDownloadItem(short id)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemMapMutex);
	m_downloadItemMap.erase(id);
}
//void crMyPlayerData::lockDownloadItemDeque()
//{
//	m_downloadItemDequeMutex.lock();
//}
//crMyPlayerData::DownloadItemDeque& crMyPlayerData::getDownloadItemDeque()
//{
//	return m_downloadItemDeque;
//}
//void crMyPlayerData::unlockDownloadItemDeque()
//{
//	m_downloadItemDequeMutex.unlock();
//}
//void crMyPlayerData::pushDownloadItem(crInstanceItem *item)
//{
//    CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemDequeMutex);
//	m_downloadItemDeque.push_back(item);
//	//crDownloadFileEndPacket packet;
//	//crDownloadFileEndPacket::buildRequestPacket(packet,flg);
//	//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Download);
//	//netConductor->getNetManager()->sendPacket("all",packet);
//}
//crInstanceItem *crMyPlayerData::getFrontDownloadItem()
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemDequeMutex);
//	return m_downloadItemDeque.empty()?NULL:m_downloadItemDeque.front().get();
//}
//void crMyPlayerData::popFrontDownloadItem()
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemDequeMutex);
//    m_downloadItemDeque.pop_front();
//}
void crMyPlayerData::insertDownloadItemChild(short id, crItemChild *item, crInstanceItem *rootitem)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildMapMutex);
	m_downloadItemChildMap[id] = ItemChildPair(item,rootitem);
}
bool crMyPlayerData::findDownloadItemChild(short id, ItemChildPair &itemChildPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildMapMutex);
	DownloadItemChildMap::iterator itr = m_downloadItemChildMap.find(id);
	if(itr != m_downloadItemChildMap.end())
	{
		itemChildPair = itr->second;
		return true;
	}
	return false;
}
void crMyPlayerData::removeDownloadItemChild(short id)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildMapMutex);
	m_downloadItemChildMap.erase(id);
}
//void crMyPlayerData::lockDownloadItemChildDeque()
//{
//	m_downloadItemChildDequeMutex.lock();
//}
//crMyPlayerData::DownloadItemChildDeque& crMyPlayerData::getDownloadItemChildDeque()
//{
//	return m_downloadItemChildDeque;
//}
//void crMyPlayerData::unlockDownloadItemChildDeque()
//{
//	m_downloadItemChildDequeMutex.unlock();
//}
//void crMyPlayerData::pushDownloadItemChild(crItemChild *item, crInstanceItem *rootitem)
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildDequeMutex);
//	m_downloadItemChildDeque.push_back(ItemChildPair(item,rootitem));
//	//crDownloadFileEndPacket packet;
//	//crDownloadFileEndPacket::buildRequestPacket(packet,flg);
//	//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Download);
//	//netConductor->getNetManager()->sendPacket("all",packet);
//}
//bool crMyPlayerData::getFrontDownloadItemChild(ItemChildPair &itemChildPair)
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildDequeMutex);
//    if(!m_downloadItemChildDeque.empty())
//	{
//        itemChildPair = m_downloadItemChildDeque.front();
//		return true;
//	}
//	return false;
//}
//void crMyPlayerData::popFrontDownloadItemChild()
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_downloadItemChildDequeMutex);
//	m_downloadItemChildDeque.pop_front();
//}
bool crMyPlayerData::findInRangeNpc(int itemid, ItemNpcPair &itemNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
	InRangeNpcMap::iterator itr = m_inRangeNpcMap.find(itemid);
	if(itr != m_inRangeNpcMap.end())
	{
		itemNpcPair = itr->second;
		return true;
	}
	return false;
}
void crMyPlayerData::insertInRangeNpc(crInstanceItem *item,crNPC *npc)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
	m_inRangeNpcMap[item->getInstanceItemID()] = ItemNpcPair(item,npc);
	item->clearSyncPos();
}
void crMyPlayerData::insertInRangeNpc(ItemNpcPair &itemNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
	m_inRangeNpcMap[itemNpcPair.first->getInstanceItemID()] = itemNpcPair;
}
void crMyPlayerData::removeInRangeNpc(int itemid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
	InRangeNpcMap::iterator itr = m_inRangeNpcMap.find(itemid);
	if(itr!=m_inRangeNpcMap.end())
	{
		insertOutRangeNpc(itr->second.first.get(),itr->second.second.get());
		crNode *relNode = itr->second.first->getRelNode();
		if(relNode)
		{
			relNode->setVisiable(false);
			relNode->setEnableIntersect(false);
		}
		itr->second.first->clearSyncPos();
		itr->second.first->clearExtra();
		m_inRangeNpcMap.erase(itr);
	}
}
//crMyPlayerData::ItemNpcPair crMyPlayerData::getOneInRangeNpc(crInstanceItem *item,float selectTargetRange)
//{
//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
//	if(!m_inRangeNpcMap.empty())
//	{
//		crVector3 myPos(item->getPosx(),item->getPosy(),item->getPosz());
//		crVector3 targetPos;
//		std::vector<ItemNpcPair> InRangeVec;
//		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
//			 itr != m_inRangeNpcMap.end();
//			 ++itr )
//		{
//			targetPos.set(itr->second.first->getPosx(),itr->second.first->getPosy(),itr->second.first->getPosz());
//			if((targetPos-myPos).length() < selectTargetRange/crGlobalHandle::gData()->gUnitScale())
//			{
//				InRangeVec.push_back(itr->second);
//			}
//		}
//		int count = InRangeVec.size();
//		if(count>0)
//		{
//			int rnd = CRCore::rangei(0,count).get_random();
//			std::vector<ItemNpcPair>::iterator vitr = InRangeVec.begin();
//			while(rnd>0)
//			{
//				rnd--;
//				vitr++;
//			}
//			return *vitr;
//		}
//	}
//	return ItemNpcPair((crInstanceItem*)NULL,(CREncapsulation::crNPC*)NULL);
//}
crInstanceItem *crMyPlayerData::getOneInRangeEnemy(crInstanceItem *iitem, float attackrange)
{
	void *param;
	float sightRange = attackrange;
	crVector3 ipos;
	if (!iitem)
	{
		if (attackrange == 0.0f)
			return NULL;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		CRProducer::crKeyboardMouseCallback* callback = bindview->getKeyboardMouseCallback();
		CRCore::crIntersectVisitor::HitList hits;
		bindview->computeIntersections(callback->mx(), callback->my(), hits);
		if (!hits.empty())
		{
			ipos = hits[0].getWorldIntersectPoint();
			//CRCore::notify(CRCore::ALWAYS) << "mx=" << callback->mx() << "my=" << callback->my() << "ipos = " << ipos << std::endl;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		crData *data = iitem->getDataClass();
		if (attackrange == 0.0f)
		{
			data->getParam(WCHDATA_CurrentAttackID, param);
			int itemid = *(int *)param;
			ref_ptr<crItemChild>aitemChild = iitem->findChildItem(itemid);
			if (aitemChild.valid() && aitemChild->isLoaded())
			{
				crInstanceItem *aitem = aitemChild->getInstanceItem();
				crData *aitemData = aitem->getDataClass();
				aitemData->getParam(WCHDATA_ItemUseRange, param);
				unsigned short itemUseRange = *((unsigned short*)param);
				attackrange = itemUseRange * crGlobalHandle::gData()->gUnitScale();
				attackrange += iitem->getAttackDistance(NULL);
			}
		}
		iitem->doEvent(MAKEINT64(WCH_GetSightRange, NULL), MAKEINT64(&sightRange, NULL));
		sightRange *= crGlobalHandle::gData()->gUnitScale();
		if (sightRange < attackrange)
			sightRange = attackrange;
		ipos = iitem->getPosition();
	}
	crVector3 epos;
	crInstanceItem *enemyItem;
	char isEnemy = 0;
	unsigned int guisestate;
	unsigned char itemstate;
	float dist,rthp;
	crNode *relNode;
	crData *itemData;
	std::multimap< float,crInstanceItem *,std::less<float> > EnemyMap;
	crRole *me = getCurrentRole();
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			enemyItem = itr->second.first.get();
			relNode = enemyItem->getRelNode();
			if(!relNode || !relNode->getVisiable())
				continue;
			itemData = enemyItem->getDataClass();
			itemData->getParam(WCHDATA_ItemState,param);
			if(!param)
				continue;
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				continue;
			//guisestate = GS_Normal;
			//if(param)
			//{
			//	guisestate = *(unsigned char *)param;
			//	if(guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable)
			//	{
			//		continue;
			//	}
			//}
			guisestate = GS_Normal;
			enemyItem->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
			{
				continue;
			}
			isEnemy = 0;
			me->doEvent(WCH_EnemyCheck, MAKEINT64(enemyItem, &isEnemy));
			if(isEnemy == -1)
			{
				epos = enemyItem->getPosition();
				dist = (epos-ipos).length();
				if(dist<=sightRange)
				{
					//itemData->getParam(WCHDATA_RTHP, param);
					//rthp = *(float*)param;
					EnemyMap.insert(std::make_pair(dist, enemyItem));
				}
			}
		}
		if(!EnemyMap.empty())
			return EnemyMap.begin()->second;
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			enemyItem = itr->second.first.get();
			relNode = enemyItem->getRelNode();
			if(!relNode || !relNode->getVisiable())
				continue;
			itemData = enemyItem->getDataClass();
			itemData->getParam(WCHDATA_ItemState,param);
			if(!param)
				continue;
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				continue;
			guisestate = GS_Normal;
			enemyItem->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
			{
				continue;
			}
			isEnemy = 0;
			me->doEvent(WCH_EnemyCheck, MAKEINT64(enemyItem, &isEnemy));
			if(isEnemy == -1)
			{
				epos = enemyItem->getPosition();
				dist = (epos-ipos).length();
				if(dist<=sightRange)
				{
					//itemData->getParam(WCHDATA_RTHP, param);
					//rthp = *(float*)param;
					EnemyMap.insert(std::make_pair(dist, enemyItem));
				}
			}
		}
		if(!EnemyMap.empty())
			return EnemyMap.begin()->second;
	}
	return NULL;
}
crInstanceItem *crMyPlayerData::getOneInRangeEnemyByMouse()
{
	return getOneInRangeEnemy(NULL, 3.5f);
}
crInstanceItem *crMyPlayerData::selectTargetInPointRange(const CRCore::crVector3 &point, float range, unsigned char targetType, unsigned char &outTargetType)
{
	crInstanceItem *targetItem = NULL;
	do
	{
		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
		void *param;
		crData *itemData;
		crInstanceItem *item;
		char isEnemy = 0;
		unsigned int guisestate;
		unsigned char itemstate;
		crNode *relNode;
		crVector3 itempos;
		float dist;
		std::multimap< float, crInstanceItem *, std::less<float> > TargetMap;
		if (targetType & Target_Self)
		{
			GNE::LockMutex lock(m_myRoleNpcMapMutex);
			for (MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
				itr != m_myRoleNpcMap.end();
				++itr)
			{
				item = itr->second.first.get();
				relNode = item->getRelNode();
				if (!relNode || !relNode->getVisiable())
					continue;
				itemData = item->getDataClass();
				itemData->getParam(WCHDATA_ItemState, param);
				if (!param)
					continue;
				itemstate = *(unsigned char *)param;
				if (itemstate == IS_Dead)
					continue;
				guisestate = GS_Normal;
				item->doEvent(MAKEINT64(WCH_GetGuiseState, 0), MAKEINT64(&guisestate, NULL));
				if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
				{
					continue;
				}
				itempos = item->getPosition();
				dist = (itempos - point).length();
				if (dist <= range)
				{
					//itemData->getParam(WCHDATA_RTHP, param);
					//rthp = *(float*)param;
					TargetMap.insert(std::make_pair(dist, item));
				}
			}
			if (!TargetMap.empty())
			{
				targetItem = TargetMap.begin()->second;
				break;
			}
		}
		if (targetType & Target_Role)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
			for (InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
				itr != m_inRangePlayerMap.end();
				++itr)
			{
				item = itr->second.first.get();
				relNode = item->getRelNode();
				if (!relNode || !relNode->getVisiable())
					continue;
				itemData = item->getDataClass();
				itemData->getParam(WCHDATA_ItemState, param);
				if (!param)
					continue;
				itemstate = *(unsigned char *)param;
				if (itemstate == IS_Dead)
					continue;
				guisestate = GS_Normal;
				item->doEvent(MAKEINT64(WCH_GetGuiseState, 0), MAKEINT64(&guisestate, NULL));
				if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
				{
					continue;
				}
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck, MAKEINT64(item, &isEnemy));
				if (isEnemy == -1 && (targetType & Target_Friend || targetType & Target_Self))
				{
					continue;
				}
				else if ((isEnemy == 1 && targetType & Target_Enemy))
				{
					continue;
				}
				itempos = item->getPosition();
				dist = (itempos - point).length();
				if (dist <= range)
				{
					//itemData->getParam(WCHDATA_RTHP, param);
					//rthp = *(float*)param;
					TargetMap.insert(std::make_pair(dist, item));
				}
			}
			if (!TargetMap.empty())
			{
				targetItem = TargetMap.begin()->second;
				break;
			}
		}
		if (targetType & Target_Npc || targetType & Target_StaticNpc)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
			for (InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
				itr != m_inRangeNpcMap.end();
				++itr)
			{
				item = itr->second.first.get();
				relNode = item->getRelNode();
				if (!relNode || !relNode->getVisiable())
					continue;
				itemData = item->getDataClass();
				itemData->getParam(WCHDATA_ItemState, param);
				if (!param)
					continue;
				itemstate = *(unsigned char *)param;
				if (itemstate == IS_Dead)
					continue;
				guisestate = GS_Normal;
				item->doEvent(MAKEINT64(WCH_GetGuiseState, 0), MAKEINT64(&guisestate, NULL));
				if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
				{
					continue;
				}
				if (guisestate & GS_Static)
				{//建筑
					if (!(targetType & Target_StaticNpc))
						continue;
				}
				else
				{
					if (!(targetType & Target_Npc))
						continue;
				}
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck, MAKEINT64(item, &isEnemy));
				if (isEnemy == -1 && (targetType & Target_Friend || targetType & Target_Self))
				{
					continue;
				}
				else if ((isEnemy == 1 && targetType & Target_Enemy))
				{
					continue;
				}
				itempos = item->getPosition();
				dist = (itempos - point).length();
				if (dist <= range)
				{
					//itemData->getParam(WCHDATA_RTHP, param);
					//rthp = *(float*)param;
					TargetMap.insert(std::make_pair(dist, item));
				}
			}
			if (!TargetMap.empty())
			{
				targetItem = TargetMap.begin()->second;
				break;
			}
		}
		//if (targetType & Target_Item)
		//{//不能对道具施法
		//}
	}
	while (0);
	outTargetType = getTargetItemType(targetItem);
	return targetItem;
}
unsigned char crMyPlayerData::getTargetItemType(crInstanceItem *targetItem)
{
	unsigned char targetType = Target_Coord;
	if (targetItem)
	{
		switch (targetItem->getItemtype())
		{
		case crInstanceItem::Role:
			if (targetItem->getID() == crMyPlayerData::getInstance()->getPlayerID())
				targetType = Target_Role | Target_Self;
			else
				targetType = Target_Role;
			break;
		case crInstanceItem::Npc:
			{
				unsigned int guisestate = GS_Normal;
				guisestate = GS_Normal;
				targetItem->doEvent(MAKEINT64(WCH_GetGuiseState, 0), MAKEINT64(&guisestate, NULL));
				if (guisestate & GS_Static)
				{
					targetType = Target_StaticNpc;
				}
				else
				{
					targetType = Target_Npc;
				}
			}
			break;
		case crInstanceItem::instanceitem:
		case crInstanceItem::LocalItem:
			targetType = Target_Item;
			break;
		}
	}
	return targetType;
}
void crMyPlayerData::updateInPatrolEnemyMap(crInstanceItem *iitem)
{
	void *param;
	crData *data = iitem->getDataClass();
	//data->getParam(WCHDATA_GuardRange,param);
	//short guardRange = *((short*)param);
	//float fguardRange = (float)guardRange * crGlobalHandle::gData()->gUnitScale();
	float scale = crGlobalHandle::gData()->gUnitScale();
	float sightRange = 0;
	iitem->doEvent(MAKEINT64(WCH_GetSightRange,NULL),MAKEINT64(&sightRange,NULL));
	sightRange *= scale;
	crVector2 ipos(iitem->getPosx(),iitem->getPosy());
	ipos *= scale;
	crVector2 epos;
	crInstanceItem *enemyItem;
	char isEnemy = 0;
	unsigned int guisestate;
	unsigned char itemstate;
	float dist;
	crNode *relNode;
	crData *itemData;
	//std::map<float,crInstanceItem *,std::less<float> > EnemyMap;
	CRCore::ScopedLock<crData> lock(*data);
	data->getParam(WCHDATA_InPatrolEnemyMap,param);
	InPatrolEnemyMap *inPatrolEnemyMap = (InPatrolEnemyMap *)param;
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			enemyItem = itr->second.first.get();
			relNode = enemyItem->getRelNode();
			if(!relNode || !relNode->getVisiable())
				continue;
			itemData = enemyItem->getDataClass();
			itemData->getParam(WCHDATA_ItemState,param);
			if(!param)
				continue;
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				continue;
			//guisestate = GS_Normal;
			//if(param)
			//{
			//	guisestate = *(unsigned char *)param;
			//	if(guisestate == GS_UnVisiable || guisestate == GS_StaticUnVisiable)
			//	{
			//		continue;
			//	}
			//}
			guisestate = GS_Normal;
			enemyItem->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
			{
				continue;
			}
			isEnemy = 0;
			iitem->doEvent(WCH_EnemyCheck,MAKEINT64(enemyItem,&isEnemy));
			if(isEnemy == -1)
			{
				epos.set(enemyItem->getPosx(),enemyItem->getPosy());
				epos *= scale;
				dist = (epos-ipos).length();
				if (dist<sightRange)
				{
					inPatrolEnemyMap->insert(std::make_pair(dist,enemyItem));
				}
			}
		}
		//if(!EnemyMap.empty())
		//	return EnemyMap.begin()->second;
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			enemyItem = itr->second.first.get();
			relNode = enemyItem->getRelNode();
			if(!relNode || !relNode->getVisiable())
				continue;
			itemData = enemyItem->getDataClass();
			itemData->getParam(WCHDATA_ItemState,param);
			if(!param)
				continue;
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				continue;
			guisestate = GS_Normal;
			enemyItem->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
			{
				continue;
			}
			isEnemy = 0;
			iitem->doEvent(WCH_EnemyCheck,MAKEINT64(enemyItem,&isEnemy));
			if(isEnemy == -1)
			{
				epos.set(enemyItem->getPosx(),enemyItem->getPosy());
				epos *= scale;
				dist = (epos-ipos).length();
				if(dist<sightRange)
				{
					inPatrolEnemyMap->insert(std::make_pair(dist,enemyItem));
				}
			}
		}
		//if(!EnemyMap.empty())
		//	return EnemyMap.begin()->second;
	}
	//return NULL;
}
void crMyPlayerData::lockInRangeNpcMap()
{
	m_inRangeNpcMapMutex.lock();
}
crMyPlayerData::InRangeNpcMap &crMyPlayerData::getInRangeNpcMap()
{
	return m_inRangeNpcMap;
}
void crMyPlayerData::unlockInRangeNpcMap()
{
	m_inRangeNpcMapMutex.unlock();
}
bool crMyPlayerData::findOutRangeNpc(int itemid,ItemNpcPair &itemNpcPair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeNpcMapMutex);
	OutRangeNpcMap::iterator itr = m_outRangeNpcMap.find(itemid);
	if(itr != m_outRangeNpcMap.end())
	{
		itemNpcPair = itr->second.first;
		return true;
	}
	return false;
}
void crMyPlayerData::insertOutRangeNpc(crInstanceItem *item,crNPC *npc)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeNpcMapMutex);
	m_outRangeNpcMap[item->getInstanceItemID()] = NpcTimePair(ItemNpcPair(item,npc),0.0f);
}
void crMyPlayerData::removeOutRangeNpc(int itemid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeNpcMapMutex);
	m_outRangeNpcMap.erase(itemid);
}
bool crMyPlayerData::findInRangeItem(int itemid, ItemNodePair& itemNodePair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
	InRangeItemMap::iterator itr = m_inRangeItemMap.find(itemid);
	if(itr != m_inRangeItemMap.end())
	{
		itemNodePair = itr->second;
		return true;
	}
	return false;
}
void crMyPlayerData::insertInRangeItem(crInstanceItem *item,CRCore::crNode *node)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
	m_inRangeItemMap[item->getInstanceItemID()] = ItemNodePair(item,node);
	item->clearSyncPos();
}
void crMyPlayerData::insertInRangeItem(ItemNodePair &itemNodePair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
	m_inRangeItemMap[itemNodePair.first->getInstanceItemID()] = itemNodePair;
	itemNodePair.first->clearSyncPos();
}
void crMyPlayerData::removeInRangeItem(int itemid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
	InRangeItemMap::iterator itr = m_inRangeItemMap.find(itemid);
	if(itr!=m_inRangeItemMap.end())
	{
		insertOutRangeItem(itr->second.first.get(),itr->second.second.get());
		crNode *relNode = itr->second.first->getRelNode();
		if(relNode)
		{
			relNode->setVisiable(false);
			relNode->setEnableIntersect(false);
		}
		itr->second.first->clearSyncPos();
		itr->second.first->clearExtra();
		m_inRangeItemMap.erase(itr);
	}
}
void crMyPlayerData::lockInRangeItemMap()
{
	m_inRangeItemMapMutex.lock();
}
crMyPlayerData::InRangeItemMap &crMyPlayerData::getInRangeItemMap()
{
	return m_inRangeItemMap;
}
void crMyPlayerData::unlockInRangeItemMap()
{
	m_inRangeItemMapMutex.unlock();
}
bool crMyPlayerData::findOutRangeItem(int itemid,ItemNodePair& itemNodePair)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeItemMapMutex);
	OutRangeItemMap::iterator itr = m_outRangeItemMap.find(itemid);
	if(itr != m_outRangeItemMap.end())
	{
		itemNodePair = itr->second.first;
		return true;
	}
	return false;
}
void crMyPlayerData::insertOutRangeItem(crInstanceItem *item,CRCore::crNode *node)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeItemMapMutex);
	m_outRangeItemMap[item->getInstanceItemID()] = ItemTimePair(ItemNodePair(item,node),0.0f);
}
void crMyPlayerData::removeOutRangeItem(int itemid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeItemMapMutex);
	m_outRangeItemMap.erase(itemid);
}
void crMyPlayerData::removeAllInRanges()
{
	crNode *relNode;
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			 itr != m_inRangePlayerMap.end();
			 ++itr )
		{
			insertOutRangePlayer(itr->second.first.get(),itr->second.second.get());
			//itr->second.second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
			relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
		}
		m_inRangePlayerMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			 itr != m_inRangeNpcMap.end();
			 ++itr )
		{
			insertOutRangeNpc(itr->second.first.get(),itr->second.second.get());
			//itr->second.second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
			relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
		}
		m_inRangeNpcMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			 itr != m_inRangeItemMap.end();
			 ++itr )
		{
			insertOutRangeItem(itr->second.first.get(),itr->second.second.get());
			//itr->second.second->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
			relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
		}
		m_inRangeItemMap.clear();
	}
}
void crMyPlayerData::update(float dt)
{//update m_outRangePlayerMap等，超时则将数据卸载。
	//if(m_playerGameData.valid())
	//{
	//	m_playerGameData->doEvent(WCH_ClientTick,MAKEINT64(dt,NULL));
	//}
	itemVisiableUpdate(dt);
	sightVisiableUpdate(dt);
	if(m_scene.valid())
	{
		m_scene->clientUpdate(dt);
	}
	m_removedt+=dt;
	if(m_removedt>=5.0f)
	{
		{
			//std::vector<_crInt64> EraseList;
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangePlayerMapMutex);
			if(!m_outRangePlayerMap.empty())
			{
				for( OutRangePlayerMap::iterator itr = m_outRangePlayerMap.begin();
					itr != m_outRangePlayerMap.end();)
				{
					itr->second.second += m_removedt;
					if(itr->second.second>crGlobalHandle::gData()->gRemoveOutRangeTime())
					{
						//EraseList.push_back(itr->first);
						itr->second.first.first->clearExtra();
						//itr->second.first.first->clearData();
						m_removedItemList.push_back(itr->second.first.first.get());
						itr->second.first.second->removeFromScene();
						//CRCore::notify(CRCore::ALWAYS)<<"delete player "<<itr->first<<std::endl;
						//char gbuf[256];
						//sprintf(gbuf,"delete player %d\n\0",itr->first);
						//gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
						itr = m_outRangePlayerMap.erase(itr);
					}
					else
					{
						++itr;
					}
				}
				//for( std::vector<_crInt64>::iterator itr = EraseList.begin();
				//	itr != EraseList.end();
				//	++itr )
				//{
				//	m_outRangePlayerMap.erase(*itr);
				//	CRCore::notify(CRCore::ALWAYS)<<"delete player "<<*itr<<std::endl;
				//}
			}
		}
		{
			//std::vector<int> EraseList;
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeNpcMapMutex);
			if(!m_outRangeNpcMap.empty())
			{
				//EraseList.clear();
				for( OutRangeNpcMap::iterator itr = m_outRangeNpcMap.begin();
					itr != m_outRangeNpcMap.end(); )
				{
					itr->second.second += m_removedt;
					if(itr->second.second>crGlobalHandle::gData()->gRemoveOutRangeTime())
					{
						//EraseList.push_back(itr->first);
						//itr->second.first.first->setDataClass(NULL);
						itr->second.first.first->clearExtra();
						//itr->second.first.first->clearData();
						m_removedItemList.push_back(itr->second.first.first);
						itr->second.first.second->removeFromScene();
						//CRCore::notify(CRCore::ALWAYS)<<"delete npc "<<itr->first<<std::endl;
						//char gbuf[256];
						//sprintf(gbuf,"delete npc %d\n\0",itr->first);
						//gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
						itr = m_outRangeNpcMap.erase(itr);
					}
					else
					{
						++itr;
					}
				}
				//for( std::vector<int>::iterator itr = EraseList.begin();
				//	itr != EraseList.end();
				//	++itr )
				//{
				//	m_outRangeNpcMap.erase(*itr);
				//	CRCore::notify(CRCore::ALWAYS)<<"delete npc "<<*itr<<std::endl;
				//}
			}
		}
		{
			//std::vector<int> EraseList;
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_outRangeItemMapMutex);
			if(!m_outRangeItemMap.empty())
			{
				//EraseList.clear();
				ref_ptr<crNode> node;
				crGroup *parent = NULL;
				for( OutRangeItemMap::iterator itr = m_outRangeItemMap.begin();
					itr != m_outRangeItemMap.end(); )
				{
					itr->second.second += m_removedt;
					if(itr->second.second>crGlobalHandle::gData()->gRemoveOutRangeTime())
					{
						//EraseList.push_back(itr->first);
						//itr->second.first.first->setDataClass(NULL);
						itr->second.first.first->clearExtra();
						//itr->second.first.first->clearData();
						m_removedItemList.push_back(itr->second.first.first);
						node = itr->second.first.second.get();

						parent = node->getParent(0);
						if(parent)
							CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,node.get());
						//CRCore::notify(CRCore::ALWAYS)<<"delete item "<<itr->first<<std::endl;
						//char gbuf[256];
						//sprintf(gbuf,"delete item %d\n\0",itr->first);
						//gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
						itr = m_outRangeItemMap.erase(itr);
					}
					else
					{
						++itr;
					}
				}
				//for( std::vector<int>::iterator itr = EraseList.begin();
				//	itr != EraseList.end();
				//	++itr )
				//{
				//	m_outRangeItemMap.erase(*itr);
				//	CRCore::notify(CRCore::ALWAYS)<<"delete node "<<*itr<<std::endl;
				//}
			}
		}
		m_removedt = 0.0f;

		for( RemovedItemList::iterator itr = m_removedItemList.begin(); itr != m_removedItemList.end();)
		{
			if((*itr)->referenceCount()==1)
			{
				(*itr)->clearData();
				itr = m_removedItemList.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
void crMyPlayerData::getEyes(std::vector<CRCore::crVector4i>&eyeVec)
{
	if(!getSelectedRoom()->getGameRunning())
	{
		return;
	}
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	void *param;
	crData *data;
	crInstanceItem *item;
	float sightRange;
	short eyeHeight;
	crVector4i eye;
	char isEnemy = 0;
	unsigned int guisestate;
	unsigned char itemstate;
	crNode *relNode;
	{
		GNE::LockMutex lock(m_myRoleNpcMapMutex);
		for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
			itr != m_myRoleNpcMap.end();
			++itr )
		{
			item = itr->second.first.get();
			data = item->getDataClass();
			guisestate = GS_Normal;
			item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Blind)
			{
				continue;
			}
			if(guisestate & GS_Static)
			{
				data->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				if(itemstate == IS_Dead)
					continue;
			}
			relNode = item->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(true);
				relNode->setEnableIntersect(true);
			}
			//data->getParam(WCHDATA_SightRange,param);
			//sightRange = *(short *)param;
			sightRange = 0;
			item->doEvent(MAKEINT64(WCH_GetSightRange,NULL),MAKEINT64(&sightRange,NULL));

			data->getParam(WCHDATA_EyeHeight,param);
			eyeHeight = *(short *)param;
			eye[0] = item->getPosx();
			eye[1] = item->getPosy();
			eye[2] = item->getPosz() + eyeHeight;
			eye[3] = sightRange;
			eyeVec.push_back(eye);
		}
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
			for (InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
				itr != m_inRangePlayerMap.end();
				++itr)
			{
				item = itr->second.first.get();
				if (ifItemIsMe(item))
				{//幻象
					data = item->getDataClass();
					guisestate = GS_Normal;
					item->doEvent(MAKEINT64(WCH_GetGuiseState, 0), MAKEINT64(&guisestate, NULL));
					if (guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Blind)
					{
						continue;
					}
					if (guisestate & GS_Static)
					{
						data->getParam(WCHDATA_ItemState, param);
						itemstate = *(unsigned char *)param;
						if (itemstate == IS_Dead)
							continue;
					}
					relNode = item->getRelNode();
					if (relNode)
					{
						relNode->setVisiable(true);
						relNode->setEnableIntersect(true);
					}
					//data->getParam(WCHDATA_SightRange,param);
					//sightRange = *(short *)param;
					sightRange = 0;
					item->doEvent(MAKEINT64(WCH_GetSightRange, NULL), MAKEINT64(&sightRange, NULL));
					data->getParam(WCHDATA_EyeHeight, param);
					eyeHeight = *(short *)param;
					eye[0] = item->getPosx();
					eye[1] = item->getPosy();
					eye[2] = item->getPosz() + eyeHeight;
					eye[3] = /*sqrtlen ? sightRange*sightRange : */sightRange;
					eyeVec.push_back(eye);
				}
			}
		}
	}
	crRoom *room = getSelectedRoom();
	if(room && room->getShareSight())
	{
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
			for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
				itr != m_inRangePlayerMap.end();
				++itr )
			{
				item = itr->second.first.get();
				if (ifItemIsMe(item))
					continue;
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy==1)
				{//队友
					data = item->getDataClass();
					guisestate = GS_Normal;
					item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
					if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Blind)
					{
						continue;
					}
					if(guisestate & GS_Static)
					{
						data->getParam(WCHDATA_ItemState,param);
						itemstate = *(unsigned char *)param;
						if(itemstate == IS_Dead)
							continue;
					}
					relNode = item->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(true);
						relNode->setEnableIntersect(true);
					}
					//data->getParam(WCHDATA_SightRange,param);
					//sightRange = *(short *)param;
					sightRange = 0;
					item->doEvent(MAKEINT64(WCH_GetSightRange,NULL),MAKEINT64(&sightRange,NULL));
					data->getParam(WCHDATA_EyeHeight,param);
					eyeHeight = *(short *)param;
					eye[0] = item->getPosx();
					eye[1] = item->getPosy();
					eye[2] = item->getPosz() + eyeHeight;
					eye[3] = /*sqrtlen ? sightRange*sightRange : */sightRange;
					eyeVec.push_back(eye);
				}
			}
		}
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
			for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
				itr != m_inRangeNpcMap.end();
				++itr )
			{
				item = itr->second.first.get();
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy==1)
				{//队友
					data = item->getDataClass();
					guisestate = GS_Normal;
					item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
					if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Blind)
					{
						continue;
					}
					if(guisestate & GS_Static)
					{
						data->getParam(WCHDATA_ItemState,param);
						itemstate = *(unsigned char *)param;
						if(itemstate == IS_Dead)
							continue;
					}
					relNode = item->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(true);
						relNode->setEnableIntersect(true);
					}
					//data->getParam(WCHDATA_SightRange,param);
					//sightRange = *(short *)param;
					sightRange = 0;
					item->doEvent(MAKEINT64(WCH_GetSightRange,NULL),MAKEINT64(&sightRange,NULL));
					data->getParam(WCHDATA_EyeHeight,param);
					eyeHeight = *(short *)param;
					eye[0] = item->getPosx();
					eye[1] = item->getPosy();
					eye[2] = item->getPosz()+eyeHeight;
					eye[3] = /*sqrtlen ? sightRange*sightRange : */sightRange;
					eyeVec.push_back(eye);
				}
			}
		}
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
			for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
				itr != m_inRangeItemMap.end();
				++itr )
			{
				item = itr->second.first.get();
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy==1)
				{//队友
					data = item->getDataClass();
					//guisestate = GS_Normal;
					//item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
					//if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Blind)
					//{
					//	continue;
					//}
					relNode = item->getRelNode();
					if(relNode)
					{
						relNode->setVisiable(true);
						relNode->setEnableIntersect(true);
					}
					//data->getParam(WCHDATA_SightRange,param);
					//sightRange = *(short *)param;
					sightRange = 0;
					item->doEvent(MAKEINT64(WCH_GetSightRange,NULL),MAKEINT64(&sightRange,NULL));
					data->getParam(WCHDATA_EyeHeight,param);
					eyeHeight = *(short *)param;
					eye[0] = item->getPosx();
					eye[1] = item->getPosy();
					eye[2] = item->getPosz()+eyeHeight;
					eye[3] =/* sqrtlen ? sightRange*sightRange : */sightRange;
					eyeVec.push_back(eye);
				}
			}
		}
		{
			int sight = crGlobalHandle::gData()->gSightRange();
			GNE::LockMutex lock(m_eyePointMapMutex);
			for( EyePointMap::iterator itr = m_eyePointMap.begin();
				 itr != m_eyePointMap.end();
				 ++itr )
			{
				eye[0] = itr->first[0];
				eye[1] = itr->first[1];
				eye[2] = itr->second;
				eye[3] = /*sqrtlen ? sight*sight : */sight;
				eyeVec.push_back(eye);
			}
		}
	}
}
bool crMyPlayerData::isTeammateAndShareSight(crInstanceItem *item)
{
	if(ifItemIsMe(item))
		return true;
	crRoom *room = getSelectedRoom();
	bool isShareSight = room && room->getShareSight();
	if(isShareSight)
	{
		crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
		char isEnemy = 0;
		me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
		return isEnemy==1;
	}
	return false;
}
void crMyPlayerData::sightVisiableUpdate(float dt)
{
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	if(!me)
		return;
	std::vector<CRCore::crVector4i> eyeVec;
	getEyes(eyeVec);
	crRoom *room = getSelectedRoom();
	bool isShareSight = room && room->getShareSight();
	char isEnemy = 0;
	crInstanceItem *item;
	void *param;
	crData *data;
	unsigned char itemstate;
	unsigned int guisestate;
	crNode *relNode;
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			item = itr->second.first.get();
			relNode = item->getRelNode();
			if(!relNode)
				continue;
			guisestate = GS_Normal;
			item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable)
			{//死亡消失
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
				continue;
			}
			data = item->getDataClass();
			data->getParam(WCHDATA_ItemState,param);
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_ItemLoad)
				continue;
			if (ifItemIsMe(item))
				continue;
			if(findItemVisiable(item))
				continue;
			if(isShareSight)
			{
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy!=1)
				{//不是队友
					checkItemVisiable(eyeVec,item);
				}
			}
			else
			{
				checkItemVisiable(eyeVec,item);
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			item = itr->second.first.get();
			relNode = item->getRelNode();
			if(!relNode)
				continue;
			guisestate = GS_Normal;
			item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable)
			{//死亡消失
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
				continue;
			}
			data = item->getDataClass();
			data->getParam(WCHDATA_ItemState,param);
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_ItemLoad)
				continue;
			if(findItemVisiable(item))
				continue;
			if(isShareSight)
			{
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy!=1)
				{//
					checkItemVisiable(eyeVec,item);
				}
			}
			else
			{
				checkItemVisiable(eyeVec,item);
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			itr != m_inRangeItemMap.end();
			++itr )
		{
			item = itr->second.first.get();
			relNode = item->getRelNode();
			if(!relNode)
				continue;
			data = item->getDataClass();
			data->getParam(WCHDATA_GuiseState,param);
			guisestate = *(unsigned int *)param;
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable)
			{//消失
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
				continue;
			}
			data->getParam(WCHDATA_ItemState,param);
			itemstate = *(unsigned char *)param;
			if(itemstate == IS_ItemLoad)
				continue;
			checkItemVisiable(eyeVec,item);
		}
	}
}
void crMyPlayerData::checkItemVisiable(std::vector<CRCore::crVector4i> &eyeVec,crInstanceItem *item)
{
	crNode *relNode = item->getRelNode();
	if(relNode)
	{
		int eyeheight;
		bool inSight = false;
		crVector2 eyepos,itempos;
		float scale = crGlobalHandle::gData()->gUnitScale();
		for( std::vector<CRCore::crVector4i>::iterator itr = eyeVec.begin();
			itr != eyeVec.end();
			++itr )
		{
			eyeheight = (*itr)[2];
			if (item->getPosz() <= eyeheight)
			{
				eyepos.set((*itr)[0], (*itr)[1]);
				itempos.set(item->getPosx(), item->getPosy());
				eyepos *= scale;
				itempos *= scale;
				if ((itempos - eyepos).length() <= (float)((*itr)[3]) * scale)
				{
					inSight = true;
					break;
				}
			}
		}
		relNode->setVisiable(inSight);
		if(inSight) relNode->setEnableIntersect(true);
	}
}
void crMyPlayerData::insertItemVisiable(crInstanceItem *item)
{
	GNE::LockMutex lock( m_itemVisiableMapMutex );
	m_itemVisiableMap[item] = crGlobalHandle::gData()->gItemVisiableInterval();
	crNode *relNode = item->getRelNode();
	if(relNode)
	{
		relNode->setVisiable(true);
		relNode->setEnableIntersect(true);
	}
}
bool crMyPlayerData::findItemVisiable(crInstanceItem *item)
{
	GNE::LockMutex lock( m_itemVisiableMapMutex );
	return m_itemVisiableMap.find(item)!=m_itemVisiableMap.end();
}
void crMyPlayerData::itemVisiableUpdate(float dt)
{
	GNE::LockMutex lock( m_itemVisiableMapMutex );
	//CRCore::ref_ptr<crInstanceItem> item;
	//crNode *relNode;
	for( ItemVisiableMap::iterator itr = m_itemVisiableMap.begin();
		 itr != m_itemVisiableMap.end();)
	{
		itr->second -= dt;
		if(itr->second<0.0f)
		{
			//item = itr->first;
			//relNode = item->getRelNode();
			//if(relNode)
			//{
			//	relNode->setVisiable(false);
			//	relNode->setEnableIntersect(false);
			//}
			itr = m_itemVisiableMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void crMyPlayerData::setCurrentGameTask(crGameTask *gameTask)
{
	m_currentGameTask = gameTask;
}
crGameTask *crMyPlayerData::getCurrentGameTask()
{
	return m_currentGameTask.get();
}
//////开房间式网游
QuerySceneList &crMyPlayerData::getSceneList()
{
    return m_sceneList;
}
void crMyPlayerData::setRoomID(int roomid)
{
	m_roomid = roomid;
}
int crMyPlayerData::getRoomID()
{
	return m_roomid;
}
void crMyPlayerData::insertRoom(crRoom *room)
{
    CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_roomMapMutex);
	m_roomMap[room->getRoomID()] = room;
}
void crMyPlayerData::lockRoomMap()
{
	m_roomMapMutex.lock();
}
RoomMap &crMyPlayerData::getRoomMap()
{
    return m_roomMap;
}
void crMyPlayerData::unlockRoomMap()
{
	m_roomMapMutex.unlock();
}
crRoom *crMyPlayerData::getRoom(int roomid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_roomMapMutex);
	RoomMap::iterator itr = m_roomMap.find(roomid);
	return itr != m_roomMap.end()?itr->second.get():NULL;
}
void crMyPlayerData::setSelectedRoom(crRoom *room)
{
	m_selectedRoom = room;
}
crRoom *crMyPlayerData::getSelectedRoom()
{
	return m_selectedRoom.get();
}

bool crMyPlayerData::isItemInRange(crInstanceItem *item)
{
	if(item->getItemtype() == crInstanceItem::instanceitem)
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		return m_inRangeItemMap.find(item->getInstanceItemID()) != m_inRangeItemMap.end();
	}
	else if(item->getItemtype() == crInstanceItem::Npc)
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		return m_inRangeNpcMap.find(item->getInstanceItemID()) != m_inRangeNpcMap.end();
	}
	else if(item->getItemtype() == crInstanceItem::Role)
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		return m_inRangePlayerMap.find(MAKEINT64(item->getID(),item->getRoleID())) != m_inRangePlayerMap.end();
	}
	return false;
}
//void crMyPlayerData::loadTextMap(const std::string &fileName)
//{
//	rcfg::ConfigScript cfg_script;
//	CRIOManager::crLoadCookFile scopedLoad(fileName);
//
//	struct stat buf;
//	if (stat( fileName.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"crMyPlayerData::loadTextMap(): file open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(fileName))
//	{
//		CRCore::notify(CRCore::FATAL)<<"crMyPlayerData::loadTextMap(): "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	int int1;
//	std::string str;
//	int nText = 1;
//	while(cfg_script.Push("Text",nText++))
//	{
//		cfg_script.Get("ID", int1);
//		cfg_script.Get("Value", str);
//        m_textMap[int1] = str;
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crMyPlayerData::loadTextMap(): "<<cfg_script.GetLastError()<<std::endl;
//	}
//}
//bool crMyPlayerData::getText(int id, std::string &text)
//{
//	TextMap::iterator itr = m_textMap.find(id);
//	if(itr != m_textMap.end())
//	{
//		text = itr->second;
//		return true;
//	}
//	return false;
//}
//void crMyPlayerData::setMapPxpm(float pxpm)
//{
//	m_mapPxpm = pxpm;
//}
//float crMyPlayerData::getMapPxpm()
//{
//	return m_mapPxpm;
//}
void crMyPlayerData::setMapScale(float scale)
{
	m_mapscale = CRCore::clampTo(scale,m_mapScaleRange[0],m_mapScaleRange[1]);
}
float crMyPlayerData::getMapScale()
{
	return m_mapscale;
}
void crMyPlayerData::setMapScaleRange(CRCore::crVector2 &range)
{
	m_mapScaleRange = range;
}
CRCore::crVector2 crMyPlayerData::getMapFactor(/*const crVector2& mapSize, */const crVector2 &mapWidgetSize)
{
	crVector2 factor;
	//if(mapSize[0]>0.0f)
	//	factor[0] = m_mapPxpm*mapWidgetSize[0]/(mapSize[0]*m_mapscale);
	//if(mapSize[1]>0.0f)
	//	factor[1] = m_mapPxpm*mapWidgetSize[1]/(mapSize[1]*m_mapscale);
	const crVector3i &worldSize = crBrain::getInstance()->getWorldSize();
	factor[0] = mapWidgetSize[0]/((float)(worldSize[0])*m_mapscale);
	factor[1] = mapWidgetSize[1]/((float)(worldSize[1])*m_mapscale);
	return factor;
}
//void crMyPlayerData::setCurrentDialogData(void *data)
//{
//	m_currentDialogData = data;
//}
//void *crMyPlayerData::getCurrentDialogData()
//{
//	return m_currentDialogData;
//}
//void crMyPlayerData::setSaleDialogEmporium(bool emporium)
//{
//	m_saleDialogEmporium = emporium;
//}
//bool crMyPlayerData::getSaleDialogEmporium()
//{
//	return m_saleDialogEmporium;
//}
//void crMyPlayerData::setITransport(bool bln)
//{
//	m_iTransport = bln;
//}
//void crMyPlayerData::iTransportCheck()
//{
//	if(m_iTransport)
//	{
//		GNE::LockMutex lock(m_myRoleNpcMapMutex);
//		m_iTransport = false;
//		crRole *role;
//		for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
//			 itr != m_myRoleNpcMap.end();
//			 ++itr )
//		{
//			role = itr->second.first.get();
//			if(!role->getOrCreatePathFinder()->isWaypointEmpty())
//			{
//				crInstanceItem::crPathFinder::WaypointDeque &waypointDeque = role->getOrCreatePathFinder()->getWaypointDeque();
//				crInstanceItem::WayPoint waypoint = waypointDeque.front();
//				crData *data = role->getDataClass();
//				unsigned char itemstate = IS_Move;
//				data->inputParam(WCHDATA_ItemState,&itemstate);
//				data->inputParam(WCHDATA_TargetPos,&(waypoint.m_coord));
//				unsigned char targettype = Target_Coord;
//				data->inputParam(WCHDATA_TargetType,&targettype);
//				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addWaypointFindRequest(role);
//				//send to net
//				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//				if(netConductor)
//				{
//					ref_ptr<crStreamBuf> stream = new crStreamBuf;
//					stream->createBuf(13+waypointDeque.size() * 34);
//					stream->_writeInt(role->getPosx());//4
//					stream->_writeInt(role->getPosy());//4
//					stream->_writeInt(role->getPosz());//4
//					stream->_writeUChar(waypointDeque.size());//1
//					for(int i = 0; i<waypointDeque.size(); i++)
//					{
//						stream->_writeInt(waypointDeque[i].m_sceneid);//4
//						stream->_writeUShort(waypointDeque[i].m_layerid);//2
//						stream->_writeVec3(waypointDeque[i].m_coord);//12
//						stream->_writeInt64(waypointDeque[i].m_msg);//8
//						stream->_writeInt64(waypointDeque[i].m_param);//8
//					}
//					crPlayerEventPacket packet;
//					crPlayerEventPacket::buildRequestPacket(packet,role->getRoleID(),WCH_RecvFindPathTo,stream.get());
//					netConductor->getNetManager()->sendPacket("all",packet);
//				}
//			}
//		}
//	}
//}
crInstanceItem* crMyPlayerData::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideIgnoreMode ignoreflg)//返回碰撞的item
{
	crInstanceItem *sceneItem;
	int roomid = item->getRoomID();
	int layerid = item->getLayerID();
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	char isEnemy = 0;
	if(ignoreflg==IG_None)
	{
		GNE::LockMutex lock(m_myRoleNpcMapMutex);
		for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
			itr != m_myRoleNpcMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					return sceneItem;
				}
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if(ignoreflg==IG_Friends)
			{
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy==1)
					continue;
			}
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					return sceneItem;
				}
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if(ignoreflg==IG_Friends)
			{
				isEnemy = 0;
				me->doEvent(WCH_EnemyCheck,MAKEINT64(item,&isEnemy));
				if(isEnemy==1)
					continue;
			}
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					return sceneItem;
				}
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			itr != m_inRangeItemMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					return sceneItem;
				}
			}
		}
	}
	return NULL;
}
bool crMyPlayerData::dynamicCollideTestAll(crInstanceItem *item,const CRCore::crVector2 &newPos,CollideItemSet& collideItemSet)//返回碰撞的item
{
	crInstanceItem *sceneItem;
	int roomid = item->getRoomID();
	int layerid = item->getLayerID();
	//{
		//GNE::LockMutex lock(m_myRoleNpcMapMutex);
		//for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
		//	itr != m_myRoleNpcMap.end();
		//	++itr )
		//{
		//	sceneItem = itr->second.first.get();
		//	if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
		//	{
		//		if(sceneItem->dynamicCollideTest(item,newPos))
		//		{
		//			collideItemSet.insert(sceneItem);
		//		}
		//	}
		//}
	//}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			itr != m_inRangePlayerMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					collideItemSet.insert(sceneItem);
				}
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			itr != m_inRangeNpcMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					collideItemSet.insert(sceneItem);
				}
			}
		}
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			itr != m_inRangeItemMap.end();
			++itr )
		{
			sceneItem = itr->second.first.get();
			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
			{
				if(sceneItem->dynamicCollideTest(item,newPos))
				{
					collideItemSet.insert(sceneItem);
				}
			}
		}
	}
	return !collideItemSet.empty();
}
//crInstanceItem *crMyPlayerData::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &coordPos)
//{
//	crInstanceItem *sceneItem;
//	int roomid = item->getRoomID();
//	int layerid = item->getLayerID();
//	{
//		GNE::LockMutex lock(m_myRoleNpcMapMutex);
//		for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
//			itr != m_myRoleNpcMap.end();
//			++itr )
//		{
//			sceneItem = itr->second.first.get();
//			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
//			{
//				if(sceneItem->dynamicCollideTest(item,coordPos))
//					return sceneItem;
//			}
//		}
//	}
//	{
//		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
//		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
//			itr != m_inRangePlayerMap.end();
//			++itr )
//		{
//			sceneItem = itr->second.first.get();
//			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
//			{
//				if(sceneItem->dynamicCollideTest(item,coordPos))
//					return sceneItem;
//			}
//		}
//	}
//	{
//		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
//		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
//			itr != m_inRangeNpcMap.end();
//			++itr )
//		{
//			sceneItem = itr->second.first.get();
//			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
//			{
//				if(sceneItem->dynamicCollideTest(item,coordPos))
//					return sceneItem;
//			}
//		}
//	}
//	{
//		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
//		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
//			itr != m_inRangeItemMap.end();
//			++itr )
//		{
//			sceneItem = itr->second.first.get();
//			if( sceneItem != item && sceneItem->getLayerID() == layerid && sceneItem->getRoomID() == roomid )
//			{
//				if(sceneItem->dynamicCollideTest(item,coordPos))
//					return sceneItem;
//			}
//		}
//	}
//	return NULL;
//}
//void crMyPlayerData::setLoginGameSceneMode(char mode)
//{
//	m_loginGameSceneMode = mode;
//}
//char crMyPlayerData::getLoginGameSceneMode()
//{
//	return m_loginGameSceneMode;
//}
void crMyPlayerData::setPlayerGameData(crPlayerGameData *data)
{
	m_playerGameData = data;
}
crPlayerGameData *crMyPlayerData::getPlayerGameData()
{
	return m_playerGameData.get();
}
void crMyPlayerData::rectSelectItem(const CRCore::crVector4i &rect,std::vector<crInstanceItem *> &itemvec)
{
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(bindview && bindview->isInited())
	{
		crSceneView *sceneView = bindview->getDefaultSceneHandler()->getSceneView();
		crBoundingBox rectBB(crVector3(rect[0],rect[1],-1),crVector3(rect[2],rect[3],1));
		rectBB.correct();
		//crBoundingBox bbox;
		crMatrixTransform *node;
		crVector3 window;
		crVector3 pos;
		crInstanceItem *item;
		crData *data;
		void *param;
		unsigned int guisestate;
		unsigned char itemstate;
		{
			GNE::LockMutex lock(m_myRoleNpcMapMutex);
			for( MyRoleNpcMap::iterator itr = m_myRoleNpcMap.begin();
				itr != m_myRoleNpcMap.end();
				++itr )
			{
				item = itr->second.first.get();
				data = item->getDataClass();
				data->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				guisestate = GS_Normal;
				item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
				if(itemstate == IS_Dead || guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
				{//不可被自己选中
					continue;
				}
				node = dynamic_cast<crMatrixTransform *>(itr->second.first->getRelNode());
				if(node && node->getVisiable())
				{
					pos = node->getTrans();
					if(crBrain::getInstance()->getCameraFrustum().contains(pos))
					{
						//bbox = node->getBoundBox();
						//sceneView->projectObjectIntoWindow(bbox.m_min,window);
						//bbox.m_min = window;
						//sceneView->projectObjectIntoWindow(bbox.m_max,window);
						//bbox.m_max = window;
						//bbox.correct();
						sceneView->projectObjectIntoWindow(pos,window);
						window[2] = 0;
						if(rectBB.contains(window))
						{
							itemvec.push_back(itr->second.first.get());
						}
					}
				}
			}
		}
		//if(itemvec.empty())
		//{//选player
		//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		//	for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
		//		itr != m_inRangePlayerMap.end();
		//		++itr )
		//	{
		//		item = itr->second.first.get();
		//		data = item->getDataClass();
		//		data->getParam(WCHDATA_ItemState,param);
		//		itemstate = *(unsigned char *)param;
		//		guisestate = GS_Normal;
		//		item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
		//		if(itemstate == IS_Dead || guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
		//		{//不可被别人选中
		//			continue;
		//		}
		//		node = dynamic_cast<crMatrixTransform *>(itr->second.first->getRelNode());
		//		if(node && node->getVisiable())
		//		{
		//			pos = node->getTrans();
		//			if(crBrain::getInstance()->getCameraFrustum().contains(pos))
		//			{
		//				//bbox = node->getBoundBox();
		//				//sceneView->projectObjectIntoWindow(bbox.m_min,window);
		//				//bbox.m_min = window;
		//				//sceneView->projectObjectIntoWindow(bbox.m_max,window);
		//				//bbox.m_max = window;
		//				//bbox.correct();
		//				//if(rectBB.intersect2D(bbox))
		//				//{
		//				//	itemvec.push_back(itr->second.first.get());
		//				//}
		//				sceneView->projectObjectIntoWindow(pos,window);
		//				window[2] = 0;
		//				if(rectBB.contains(window))
		//				{
		//					itemvec.push_back(itr->second.first.get());
		//				}
		//			}
		//		}
		//	}
		//}
		//if(itemvec.empty())
		//{//选npc
		//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		//	for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
		//		itr != m_inRangeNpcMap.end();
		//		++itr )
		//	{
		//		item = itr->second.first.get();
		//		data = item->getDataClass();
		//		data->getParam(WCHDATA_ItemState,param);
		//		itemstate = *(unsigned char *)param;
		//		guisestate = GS_Normal;
		//		item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
		//		if(itemstate == IS_Dead || guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
		//		{//不可被别人选中
		//			continue;
		//		}
		//		node = dynamic_cast<crMatrixTransform *>(itr->second.first->getRelNode());
		//		if(node && node->getVisiable())
		//		{
		//			pos = node->getTrans();
		//			if(crBrain::getInstance()->getCameraFrustum().contains(pos))
		//			{
		//				//bbox = node->getBoundBox();
		//				//sceneView->projectObjectIntoWindow(bbox.m_min,window);
		//				//bbox.m_min = window;
		//				//sceneView->projectObjectIntoWindow(bbox.m_max,window);
		//				//bbox.m_max = window;
		//				//bbox.correct();
		//				//if(rectBB.intersect2D(bbox))
		//				//{
		//				//	itemvec.push_back(itr->second.first.get());
		//				//}
		//				sceneView->projectObjectIntoWindow(pos,window);
		//				window[2] = 0;
		//				if(rectBB.contains(window))
		//				{
		//					itemvec.push_back(itr->second.first.get());
		//				}
		//			}
		//		}
		//	}
		//}
		//if(itemvec.empty())
		//{//选item
		//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		//	for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
		//		itr != m_inRangeItemMap.end();
		//		++itr )
		//	{
		//		item = itr->second.first.get();
		//		data = item->getDataClass();
		//		data->getParam(WCHDATA_ItemState,param);
		//		itemstate = *(unsigned char *)param;
		//		guisestate = GS_Normal;
		//		item->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));
		//		if(itemstate == IS_Dead || guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate || guisestate & GS_NoAttack)
		//		{//不可被别人选中
		//			continue;
		//		}
		//		node = dynamic_cast<crMatrixTransform *>(itr->second.first->getRelNode());
		//		if(node && node->getVisiable())
		//		{
		//			pos = node->getTrans();
		//			if(crBrain::getInstance()->getCameraFrustum().contains(pos))
		//			{
		//				//bbox = node->getBoundBox();
		//				//sceneView->projectObjectIntoWindow(bbox.m_min,window);
		//				//bbox.m_min = window;
		//				//sceneView->projectObjectIntoWindow(bbox.m_max,window);
		//				//bbox.m_max = window;
		//				//bbox.correct();
		//				//if(rectBB.intersect2D(bbox))
		//				//{
		//				//	itemvec.push_back(itr->second.first.get());
		//				//}
		//				sceneView->projectObjectIntoWindow(pos,window);
		//				window[2] = 0;
		//				if(rectBB.contains(window))
		//				{
		//					itemvec.push_back(itr->second.first.get());
		//				}
		//			}
		//		}
		//	}
		//}
	}
}
float crMyPlayerData::getPosZ(float x,float y,float zoffset)
{
	if(m_scene.valid())
	{
		crRole* role = getCurrentRole();
		return m_scene->getPosZ(role?role->getLayerID():0,x,y,zoffset);
	}
	return 0.0f;
}
void crMyPlayerData::insertEyePoint(const CRCore::crVector2i &eye)
{
	crSceneLayer *scenelayer = crMyPlayerData::getInstance()->getSceneLayer();
	if(scenelayer)
	{
		GNE::LockMutex lock( m_eyePointMapMutex );
		float scale = crGlobalHandle::gData()->gUnitScale();
		crVector2s coord = scenelayer->getCoord(eye * scale);
		int height = scenelayer->getHeight(coord[0],coord[1])/scale + crGlobalHandle::gData()->gEyeHeight();
		m_eyePointMap[eye] = height;
	}
}
void crMyPlayerData::removeEyePoint(const CRCore::crVector2i &eye)
{
	GNE::LockMutex lock( m_eyePointMapMutex );
	m_eyePointMap.erase(eye);
}
void crMyPlayerData::clearEyePoint()
{
	GNE::LockMutex lock( m_eyePointMapMutex );
	m_eyePointMap.clear();
}
void crMyPlayerData::clearItemVisiableMap()
{
	GNE::LockMutex lock( m_itemVisiableMapMutex );
	m_itemVisiableMap.clear();
}
//////////收到ConnectServer
void crMyPlayerData::setConnectServerData(crQueryConnectServerData *connectServerData)
{
	m_connectServerData = connectServerData;
}
crQueryConnectServerData *crMyPlayerData::getConnectServerData()
{
	return m_connectServerData.get();
}
void crMyPlayerData::clearAllInsights()
{
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangePlayerMapMutex);
		for( InRangePlayerMap::iterator itr = m_inRangePlayerMap.begin();
			 itr != m_inRangePlayerMap.end();
			 ++itr )
		{
			insertOutRangePlayer(itr->second.first.get(),itr->second.second.get());
			crNode *relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
			itr->second.first->clearSyncPos();
			itr->second.first->clearExtra();
		}
		m_inRangePlayerMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeNpcMapMutex);
		for( InRangeNpcMap::iterator itr = m_inRangeNpcMap.begin();
			 itr != m_inRangeNpcMap.end();
			 ++itr )
		{
			insertOutRangeNpc(itr->second.first.get(),itr->second.second.get());
			crNode *relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
			itr->second.first->clearSyncPos();
			itr->second.first->clearExtra();
		}
		m_inRangeNpcMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_inRangeItemMapMutex);
		for( InRangeItemMap::iterator itr = m_inRangeItemMap.begin();
			 itr != m_inRangeItemMap.end();
			 ++itr )
		{
			insertOutRangeItem(itr->second.first.get(),itr->second.second.get());
			crNode *relNode = itr->second.first->getRelNode();
			if(relNode)
			{
				relNode->setVisiable(false);
				relNode->setEnableIntersect(false);
			}
			itr->second.first->clearSyncPos();
			itr->second.first->clearExtra();
		}
		m_inRangeItemMap.clear();
	}
	clearItemVisiableMap();
}
void crMyPlayerData::setLoadMainRoleCase(int _case)
{
	m_loadmainrolecase = _case;
}
int crMyPlayerData::getLoadMainRoleCase()
{
	return m_loadmainrolecase;
}
///////////////////////////////////
//
//crGameClientUpdate
//
/////////////////////////////////////
ref_ptr<crGameClientUpdate> crGameClientUpdate::m_instance = NULL;
crGameClientUpdate::crGameClientUpdate():
	m_time(0.0f),
	m_interval(0.1f)
{
}
crGameClientUpdate* crGameClientUpdate::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crGameClientUpdate;
		crBrain::getInstance()->pushInstance(m_instance.get());
		CRUtil::crSceneView::registerExternUpdater(m_instance.get());
	}
	return m_instance.get();
}

void crGameClientUpdate::clear()
{
	m_instance = NULL;
}

void crGameClientUpdate::operator()(float dt)
{
	m_time += dt;
	if(m_time > m_interval)
	{
		crMyPlayerData::getInstance()->update(m_time);
		m_time = 0.0f;
	}
}
