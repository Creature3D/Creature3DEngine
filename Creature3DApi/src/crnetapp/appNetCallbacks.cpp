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

#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/crNotify.h>
#include <CRNet/crNetDataManager.h>
#include <CRNetApp/appGlobalHandle.h>
#include "rcfg/ConfigScriptOO.h"
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRNetApp/appDBQuery.h>
#include <sys/stat.h>
#include <CREncapsulation/crLoader.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crFileUtils.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crTableIO.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRIOManager;
///////////////////////////////////
//
//crLoginServerCallback
//
/////////////////////////////////////
crLoginServerCallback::crLoginServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_reconnectCount(0)
{
}

crLoginServerCallback::crLoginServerCallback(const crLoginServerCallback& callback):
crNetCallback(callback),
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_reconnectCount(0)
{
}

void crLoginServerCallback::serverInitImplementation()
{
	const std::string& file = m_netConductor->getCfgFile();
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crLoginServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		//sprintf(gDebugInfo->buf(),"crLoginServerCallback::serverInitImplementation(): file open error,%s\n\0",file.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
		{
			CRCore::notify(CRCore::FATAL)<<"crLoginServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
			//sprintf(gDebugInfo->buf(),"crLoginServerCallback::init,检查Server.cfg %s\n\0",cfg_script.GetLastError().c_str());
			//gDebugInfo->debugInfo(CRCore::FATAL);
		}
	}

	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	//查询并返回GameList
	ref_ptr<crGmQueryData> gmQueryQuery = new crGmQueryData;
	gmQueryQuery->buildQuerySql();
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
		gmQueryQuery = dynamic_cast<crGmQueryData *>(itr->get());
		gmMap[gmQueryQuery->getPlayerID()] = gmQueryQuery->getAuthorityid();
	}
	crGlobalHandle::gData()->unlockGMMap();
}

void crLoginServerCallback::clientInitImplementation()
{
}

void crLoginServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crLoginServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			if(m_reconnectCount>=3)
			{
				std::string logdata = "与LoginServer重连次数超过3次"+m_netConductor->getName()+"自动重启！";
				CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
				_asm   int   3   //只是为了让程序崩溃
			}
			m_reconnectCount++;
			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
	}
}

void crLoginServerCallback::serverShutdown()
{

}

void crLoginServerCallback::clientShutdown()
{
}

bool crLoginServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crLoginServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crLoginServerCallback::clientUpdateImplementation 重新连接成功\n\0");
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
		return true;
	}
	return false;
}

///////////////////////////////////
//
//crLoginConnectServerCallback
//
/////////////////////////////////////
crLoginConnectServerCallback::crLoginConnectServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crLoginConnectServerCallback::crLoginConnectServerCallback(const crLoginConnectServerCallback& callback):
crNetCallback(callback)
{
}

void crLoginConnectServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crLoginConnectServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		//sprintf(gDebugInfo->buf(),"crLoginConnectServerCallback::serverInitImplementation(): file open error,%s\n\0",file.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
		{
			CRCore::notify(CRCore::FATAL)<<"crLoginServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
			//sprintf(gDebugInfo->buf(),"crLoginServerCallback::init,检查Server.cfg %s\n\0",cfg_script.GetLastError().c_str());
			//gDebugInfo->debugInfo(CRCore::FATAL);
		}
	}
}

void crLoginConnectServerCallback::clientInitImplementation()
{
}

void crLoginConnectServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crLoginConnectServerCallback::clientUpdateImplementation(float dt)
{
	//if(!m_netConductor->getNetManager()->isRunning())
	//{//客户端断线不进行重连接
	//	//crGlobalHandle::getInstance()->doEvent(WCH_LoginConnectServerAbnormal,MAKEINT64(NULL,NULL));
	//	CRCore::notify(CRCore::ALWAYS)<<"crLoginConnectServerCallback::clientUpdateImplementation"<<std::endl;
	//}
}

void crLoginConnectServerCallback::serverShutdown()
{

}

void crLoginConnectServerCallback::clientShutdown()
{
}

///////////////////////////////////
//
//crChatServerCallback
//
/////////////////////////////////////
crChatServerCallback::crChatServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crChatServerCallback::crChatServerCallback(const crChatServerCallback& callback):
crNetCallback(callback)
{
}

void crChatServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crChatServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crChatServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
	ref_ptr<crData> data = CREncapsulation::loadData("server/chatserverevent.cfg");
	crGlobalHandle::getInstance()->setDataClass(data.get());
}

void crChatServerCallback::clientInitImplementation()
{
}

void crChatServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
		crDataStreamTransfer::getInstance()->update(dt);
	}
}

void crChatServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			crNetConductor *chatConnectServer = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
			if(chatConnectServer)
			{//ChatServer异常，断开与玩家的连接
				chatConnectServer->getNetManager()->disconnectAll();
			}

			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crChatServerCallback::serverShutdown()
{

}

void crChatServerCallback::clientShutdown()
{
}

bool crChatServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crChatServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
		return true;
	}
	return false;
}
void crChatServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	CRCore::notify(CRCore::ALWAYS)<<"crChatServerCallback::serverRemoveConnectionImplementation  "<<address<<std::endl;

	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crServerPlayerData *playerData;
	std::vector< ref_ptr<crServerPlayerData> > playerEraseList;
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		if(playerData)
		{
			if(address.compare(playerData->getPlayerConnectServerAddress()) == 0)
			{
				playerEraseList.push_back(playerData);
			}
		}
	}
	std::vector< ref_ptr<crServerPlayerData> >::iterator eitr;
	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerMap.erase((*eitr)->getPlayerID());
		//netDataManager->removePlayerData((*eitr)->getPlayerID());
	}
	netDataManager->unLockPlayerDataMap();


	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerDroped(eitr->get());
	}
}
void crChatServerCallback::playerDroped(crPlayerData *_playerData)
{
	crChatServerPlayerData *playerData = dynamic_cast<crChatServerPlayerData *>(_playerData);
    //数据库设成离线状态。通知我的好友及群成员，我离线了
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crUpdateChatState> updateChatState = new crUpdateChatState;
	updateChatState->buildUpdateSql(playerData->getPlayerID(),crNetChatData::OffLine);
	if(globalSession->executeUpdate(updateChatState.get()))
	{//插入成功
		globalSession->commit();
	}
	db->endSession(globalSession.get());

	std::set<int> InformedPlayerSet;
	bool informed = false;
	playerData->getNetChatData()->lockFriendMap();
	crNetChatData::crChatFriend *chatFriend,*chatFriend2;
	ref_ptr<crChatServerPlayerData> loginedChatPlayer;
	crNetChatData::FriendMap &friendMap = playerData->getNetChatData()->getFriendMap();
	crChatStateChangePacket chatStateChangePacket;
	crNetConductor *parentServer = playerData->getParentServerLineConductor();
	for( crNetChatData::FriendMap::iterator itr = friendMap.begin();
		 itr != friendMap.end();
		 ++itr )
	{
        chatFriend = itr->second.get();
		if(chatFriend->getPlayerID() != playerData->getPlayerID())
		{
			InformedPlayerSet.insert(chatFriend->getPlayerID());
			if(chatFriend->getFriendChatState()>0)
			{
				crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),chatFriend->getPlayerID(),crNetChatData::OffLine,playerData->getNickName());
				loginedChatPlayer = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(chatFriend->getPlayerID()));
				if(loginedChatPlayer.valid())
				{
					chatFriend2 = loginedChatPlayer->getNetChatData()->getFriend(playerData->getPlayerID());
					if(chatFriend2)
					{
						chatFriend2->setFriendChatState(crNetChatData::OffLine);
						m_netConductor->getNetManager()->sendPacket(loginedChatPlayer->getPlayerConnectServerAddress(),chatStateChangePacket);
					}
				}
				else if(parentServer)
				{
					parentServer->getNetManager()->sendPacket("all",chatStateChangePacket);
				}
			}
		}
	}
	playerData->getNetChatData()->unlockFriendMap();

	playerData->getNetChatData()->lockGroupMap();
	crNetChatData::crChatGroupMember *chatMember, *chatMember2;
	crNetChatData::ChatGroupMap &chatGroup = playerData->getNetChatData()->getChatGroupMap();
	for( crNetChatData::ChatGroupMap::iterator itr = chatGroup.begin();
		 itr != chatGroup.end();
		 ++itr )
	{
		itr->second->lockMemberMap();
		crNetChatData::crChatGroup::ChatMemberMap &chatMemberMap = itr->second->getMemberMap();
		for( crNetChatData::crChatGroup::ChatMemberMap::iterator mitr = chatMemberMap.begin();
			 mitr != chatMemberMap.end();
			 ++mitr )
		{
            chatMember = mitr->second.get();
			if(chatMember->getPlayerID() != playerData->getPlayerID())
			{
				informed = false;
				if(InformedPlayerSet.find(chatMember->getPlayerID())!=InformedPlayerSet.end())
				{
					informed = true;
				}
				else
				{
					InformedPlayerSet.insert(chatMember->getPlayerID());
				}
				if(!informed && chatMember->getMemberChatState()>0)
				{
					crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),chatMember->getPlayerID(),crNetChatData::OffLine,playerData->getNickName());
					loginedChatPlayer = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(chatMember->getPlayerID()));
					if(loginedChatPlayer.valid())
					{
						chatMember2 = loginedChatPlayer->getNetChatData()->findChatGroup(itr->second->getGroupID())->getMember(chatMember->getPlayerID());
						if(chatMember2)
						{
							chatMember2->setMemberChatState(crNetChatData::OffLine);
							m_netConductor->getNetManager()->sendPacket(loginedChatPlayer->getPlayerConnectServerAddress(),chatStateChangePacket);
						}
					}
					else if(parentServer)
					{
						parentServer->getNetManager()->sendPacket("all",chatStateChangePacket);
					}
				}
			}
		}
		itr->second->unlockMemberMap();
	}
	playerData->getNetChatData()->unlockGroupMap();
}
///////////////////////////////////
//
//crChatConnectServerCallback
//
/////////////////////////////////////
crChatConnectServerCallback::crChatConnectServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crChatConnectServerCallback::crChatConnectServerCallback(const crChatConnectServerCallback& callback):
crNetCallback(callback)
{
}

void crChatConnectServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crChatConnectServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crLoginServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crChatConnectServerCallback::clientInitImplementation()
{
}

void crChatConnectServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crChatConnectServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//客户端断线不进行重连接
		//crGlobalHandle::getInstance()->doEvent(WCH_ChatConnectServerAbnormal,MAKEINT64(NULL,NULL));
		CRCore::notify(CRCore::ALWAYS)<<"crChatConnectServerCallback::clientUpdateImplementation Net已经断开"<<std::endl;
		CRCore::crThread::sleep(1000);
	}
	else
	{
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crChatConnectServerCallback::serverShutdown()
{

}

void crChatConnectServerCallback::clientShutdown()
{
}

///////////////////////////////////
//
//crParentServerCallback
//
/////////////////////////////////////
crParentServerCallback::crParentServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crParentServerCallback::crParentServerCallback(const crParentServerCallback& callback):
crNetCallback(callback)
{
}

void crParentServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crParentServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crChatServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crParentServerCallback::clientInitImplementation()
{
}

void crParentServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crParentServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crParentServerCallback::serverShutdown()
{

}

void crParentServerCallback::clientShutdown()
{
}

bool crParentServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crChatServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
        //relogin ParentServer
		crNetConductor *serverConductor = NULL;
		int netType = m_netConductor->getNetType();
		if(netType == ParentServerClient_Parent)
		{
			serverConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
			if(serverConductor)
			{
				crNetDataManager *netDataManager = serverConductor->getNetDataManager();
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
				crParentServerPlayerData *playerData;
				crLoginParentPacket loginParentPacket;
				for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
					itr != playerMap.end();
					++itr )
				{
					playerData = dynamic_cast<crParentServerPlayerData *>(itr->second.get());
					if(playerData->getParentServerLineConductor() == m_netConductor)
					{
						crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerData->getPlayerAdress());
						m_netConductor->getNetManager()->sendPacket("all",loginParentPacket);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
		else if(netType == ChatServerClient_Parent)
		{
			serverConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
			if(serverConductor)
			{
				crNetDataManager *netDataManager = serverConductor->getNetDataManager();
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
				crChatServerPlayerData *playerData;
				crLoginParentPacket loginParentPacket;
				for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
					itr != playerMap.end();
					++itr )
				{
					playerData = dynamic_cast<crChatServerPlayerData *>(itr->second.get());
					if(playerData->getParentServerLineConductor() == m_netConductor)
					{
						crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerData->getPlayerAdress());
						m_netConductor->getNetManager()->sendPacket("all",loginParentPacket);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
		else if(netType == GameServerClient_Parent)
		{
			serverConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
			if(serverConductor)
			{
				crNetDataManager *netDataManager = serverConductor->getNetDataManager();
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
				crGameServerPlayerData *playerData;
				crLoginParentPacket loginParentPacket;
				for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
					itr != playerMap.end();
					++itr )
				{
					playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if(playerData->getParentServerLineConductor() == m_netConductor)
					{
						crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerData->getPlayerAdress());
						m_netConductor->getNetManager()->sendPacket("all",loginParentPacket);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
		return true;
	}
	return false;
}
void crParentServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	CRCore::notify(CRCore::ALWAYS)<<"crParentServerCallback::serverRemoveConnectionImplementation  "<<address<<std::endl;

	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crServerPlayerData *playerData;
	std::vector< ref_ptr<crServerPlayerData> > playerEraseList;
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		if(playerData)
		{
			if(address.compare(playerData->getPlayerConnectServerAddress()) == 0)
			{
				playerEraseList.push_back(playerData);
			}
		}
	}
	std::vector< ref_ptr<crServerPlayerData> >::iterator eitr;
	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerMap.erase((*eitr)->getPlayerID());
		//netDataManager->removePlayerData((*eitr)->getPlayerID());
	}
	netDataManager->unLockPlayerDataMap();


	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerDroped(eitr->get());
	}
}
void crParentServerCallback::playerDroped(crPlayerData *_playerData)
{

}

///////////////////////////////////
//
//crDownloadServerCallback
//
/////////////////////////////////////
crDownloadServerCallback::crDownloadServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crDownloadServerCallback::crDownloadServerCallback(const crDownloadServerCallback& callback):
crNetCallback(callback)
{
}

void crDownloadServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crDownloadServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crChatServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crDownloadServerCallback::clientInitImplementation()
{
}

void crDownloadServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		crFileStreamCacheManager::getInstance()->update(dt);
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crDownloadServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
			if(connectServer)
			{//GameServer异常，断开与玩家的连接
				connectServer->getNetManager()->disconnectAll();
			}

			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crDownloadServerCallback::serverShutdown()
{

}

void crDownloadServerCallback::clientShutdown()
{
}

bool crDownloadServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crGameServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
		return true;
	}
	return false;
}
void crDownloadServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	CRCore::notify(CRCore::ALWAYS)<<"crDownloadServerCallback::serverRemoveConnectionImplementation  "<<address<<std::endl;

	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crServerPlayerData *playerData;
	std::vector< ref_ptr<crServerPlayerData> > playerEraseList;
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		if(playerData)
		{
			if(address.compare(playerData->getPlayerConnectServerAddress()) == 0)
			{
				playerEraseList.push_back(playerData);
			}
		}
	}
	std::vector< ref_ptr<crServerPlayerData> >::iterator eitr;
	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerMap.erase((*eitr)->getPlayerID());
		//netDataManager->removePlayerData((*eitr)->getPlayerID());
	}
	netDataManager->unLockPlayerDataMap();


	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerDroped(eitr->get());
	}
}
void crDownloadServerCallback::playerDroped(crPlayerData *_playerData)
{
}
///////////////////////////////////
//
//crDownloadConnectServerCallback
//
/////////////////////////////////////
crDownloadConnectServerCallback::crDownloadConnectServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f)
{
}

crDownloadConnectServerCallback::crDownloadConnectServerCallback(const crDownloadConnectServerCallback& callback):
crNetCallback(callback)
{
}

void crDownloadConnectServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crDownloadConnectServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crDownloadConnectServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crDownloadConnectServerCallback::clientInitImplementation()
{
}

void crDownloadConnectServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
}

void crDownloadConnectServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//客户端断线不进行重连接
		//crGlobalHandle::getInstance()->doEvent(WCH_DownloadConnectServerAbnormal,MAKEINT64(NULL,NULL));
		CRCore::notify(CRCore::ALWAYS)<<"crDownloadConnectServerCallback::clientUpdateImplementation Net已经断开"<<std::endl;
		CRCore::crThread::sleep(1000);
	}
	else
	{
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crDownloadConnectServerCallback::serverShutdown()
{

}

void crDownloadConnectServerCallback::clientShutdown()
{
}
///////////////////////////////////
//
//crGameServerCallback
//
/////////////////////////////////////
crGameServerCallback::crGameServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_dbpingTime(0.0f),
m_gameid(0),
m_saveInterval(600.0f),
m_shutdownTime(0.0f),
m_savePlayerNumPerFrame(50),
m_serverid(0),
m_webserverid(0),
m_isp(0),
m_maxplayercount(0),
m_updateInterval(300.0f),
m_updateTime(0.0f),
m_lastcount(0),
m_dropedServerCount(0),
//m_dropedTimer(0.0f),
m_dropedSave(false),
m_reconnectCount(0),
m_maxDropServerCount(9)
{
}
crGameServerCallback::crGameServerCallback(const crGameServerCallback& callback):
crNetCallback(callback),
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_dbpingTime(0.0f),
m_gameid(0),
m_saveInterval(600.0f),
m_shutdownTime(0.0f),
m_savePlayerNumPerFrame(50),
m_serverid(0),
m_webserverid(0),
m_isp(0),
m_maxplayercount(0),
m_updateInterval(300.0f),
m_updateTime(0.0f),
m_lastcount(0),
m_dropedServerCount(0),
//m_dropedTimer(0.0f),
m_dropedSave(false),
m_reconnectCount(0),
m_maxDropServerCount(9)
{
}
void crGameServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crGameServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		//sprintf(gDebugInfo->buf(),"crGameServerCallback::serverInitImplementation(): file open error,%s\n\0",file.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}
	CRCore::ref_ptr<CREncapsulation::crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	CREncapsulation::crTableIO::StrVec record;
	int gameindex = cfgtable->getTitleIndex("GameID");
	int serverindex = cfgtable->getTitleIndex("ServerID");
	cfgtable->queryOneRecord(0,"GameServer",record);
	m_gameName = record[gameindex];
	std::string serverid = record[serverindex];

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		cfg_script.Get("SaveInterval", m_saveInterval);
		//cfg_script.Get("GameName", m_gameName);
		cfg_script.Get("SavePlayerNumPerFrame",m_savePlayerNumPerFrame);
		cfg_script.Get("UpdateInterval",m_updateInterval);
		cfg_script.Get("MaxDropServerCount",m_maxDropServerCount);
		crGlobalHandle::getInstance()->setGameName(m_gameName);
		std::string globalData = m_gameName + "/GlobalData.cfg";
		crData* data = crGlobalHandle::getInstance()->getDataClass();
		CREncapsulation::loadIncludeData(globalData,data);
		std::string brainData = m_gameName+"/GameServerBrain.cfg";
		data = CREncapsulation::loadData(brainData);
		crServerBrainHandle::getInstance()->setDataClass(data);
		if (!cfg_script.Pop())
		{
			CRCore::notify(CRCore::FATAL)<<"crGameServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
			//sprintf(gDebugInfo->buf(),"crGameServerCallback::init,检查Server.cfg %s\n\0",cfg_script.GetLastError().c_str());
			//gDebugInfo->debugInfo(CRCore::FATAL);
		}
	}
	m_savePlayerVec.reserve(m_savePlayerNumPerFrame);
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crGameIDQueryData> gameIDQueryData = new crGameIDQueryData;
	gameIDQueryData->buildQuerySql(m_gameName);
	globalSession->executeQuery(gameIDQueryData.get());
	crDataBase::QueryResultVec &gameIDQueryResultVec = globalSession->getQueryResult();
	if(!gameIDQueryResultVec.empty())
	{
		gameIDQueryData = dynamic_cast<crGameIDQueryData *>(gameIDQueryResultVec[0].get());
		m_gameid = gameIDQueryData->getID();
	}
	globalSession->releaseQuery();

	m_mainScene = new crScene;
	m_mainScene->setGameID(m_gameid);
	m_mainScene->setSceneID(0);
	///////区服务器信息
	ref_ptr<CREncapsulation::crTableIO> gameTab = CREncapsulation::crTableIO::openFile("server/"+m_gameName+"/GameServerTab.tab1");
	if(gameTab.valid())
	{
		CREncapsulation::crTableIO::StrVec record;
		gameTab->queryOneRecord(0,serverid/*m_netConductor->getNetDataManager()->getServerName()*/,record);
		int servernameid = gameTab->getTitleIndex("servername");
		int ispid = gameTab->getTitleIndex("isp");
		int maxplayercountid = gameTab->getTitleIndex("maxplayercount");
		int siteid = gameTab->getTitleIndex("site");
		int webserverid = gameTab->getTitleIndex("webserverid");
		m_serverName = record[servernameid];
		m_isp = (unsigned char)(atoi(record[ispid].c_str()));
		m_maxplayercount = (short)(atoi(record[maxplayercountid].c_str()));
		if(siteid>=0) m_site = record[siteid];
		if(webserverid>=0) m_webserverid = atoi(record[webserverid].c_str());
		//查询并返回GameList
		ref_ptr<crServerListIDQueryData> serverListIDQuery = new crServerListIDQueryData;
		serverListIDQuery->buildQuerySql(m_gameid,serverid);
		globalSession->executeQuery(serverListIDQuery.get());
		crDataBase::QueryResultVec serverListIDQueryResultVec = globalSession->getQueryResult();
		globalSession->releaseQuery();
		if(serverListIDQueryResultVec.empty())
		{
			ref_ptr<crInsertServerList> insertServerList = new crInsertServerList;
			insertServerList->buildUpdateSql(m_gameid,m_webserverid,m_site,serverid,m_serverName,m_isp,m_maxplayercount);
			if(globalSession->executeUpdate(insertServerList.get()))
			{//插入成功
				m_serverid = globalSession->getInsertID();
				globalSession->commit();
			}
		}
		else
		{
			serverListIDQuery = dynamic_cast<crServerListIDQueryData *>(serverListIDQueryResultVec[0].get());
			m_serverid = serverListIDQuery->getID();
			m_webserverid = serverListIDQuery->getWebServerID();
			m_site = serverListIDQuery->getSite();
		}
	}
	db->endSession(globalSession.get());

	crServerBrainHandle::getInstance()->doEvent(WCH_ServerInit,MAKEINT64(&m_serverName,m_serverid));
	CRCore::notify(CRCore::ALWAYS)<<"crGameServerCallback::serverInitImplementation() "<<m_serverName<<" 初始化完成,ServerID:"<<m_serverid<<std::endl;
	//sprintf(gDebugInfo->buf(),"crGameServerCallback::serverInitImplementation() %s 初始化完成\n\0",m_serverName.c_str());
	//gDebugInfo->debugInfo(CRCore::ALWAYS);
}
int crGameServerCallback::getGameID()
{
	return m_gameid;
}

const std::string &crGameServerCallback::getGameName() const
{
	return m_gameName;
}
bool crGameServerCallback::checkServer(int serverid)
{
	return m_serverid == serverid;
}
void crGameServerCallback::clientInitImplementation()
{
}
void crGameServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	crGlobalHandle::getInstance()->timer()+=dt;
	if(m_netConductor->getNetManager()->isRunning())
	{
		//if(m_dropedTimer>0.0f)
		//{
		//	m_dropedTimer -= dt;
		//	if(m_dropedTimer<=0.0f)
		//	{
		//		m_dropedTimer = 0.0f;
		//		CRCore::notify(CRCore::FATAL)<<"GameServer服务器自动重启！"<<std::endl;
		//		_asm   int   3   //只是为了让程序崩溃
		//	}
		//}
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
		m_dbpingTime += dt;
		if(m_dbpingTime>DBPingInterval)
		{
			m_dbpingTime = 0.0f;
			crNetConductor *dbserver = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
			if(dbserver)
			{
				crDBSavePacket packet;
				dbserver->getNetManager()->sendPacket("all",packet);
			}
		}
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData;
		crPlayerGameData *playerGameData;
		netDataManager->lockPlayerDataMap();
		crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
		int mapsize = playerMap.size();
		m_updPlayerVec.resize(0);
		m_savePlayerVec.resize(0);
		if(m_dropedSave)
		{
			m_dropedSave = false;
			for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
				itr != playerMap.end(); 
				++itr )
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				playerGameData = playerData->getPlayerGameData();
				if(playerGameData && playerGameData->getDBValid())
				{
					m_updPlayerVec.push_back(playerData);
					m_savePlayerVec.push_back(playerData);
				}
			}
		}
		else
		{
			for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
				itr != playerMap.end(); 
				++itr )
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				playerGameData = playerData->getPlayerGameData();
				if(playerGameData && playerGameData->getDBValid())
				{
					m_updPlayerVec.push_back(playerData);

					playerData->saveTimer() -= dt;
					if(playerData->saveTimer()<=0 && m_savePlayerVec.size()<m_savePlayerNumPerFrame)
					{
						m_savePlayerVec.push_back(playerData);
						//savePlayerData(playerData);
						//elapsedTime = timer.delta_s(start_tick,timer.tick());
					}
				}
			}
		}
		netDataManager->unLockPlayerDataMap();
		for( PlayerVec::iterator itr = m_updPlayerVec.begin();
			 itr != m_updPlayerVec.end();
			 ++itr )
		{
			playerData = dynamic_cast<crGameServerPlayerData *>(itr->get());
			float updatedt = dt;
			playerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_ServerUpdate,GameServer),MAKEINT64(&updatedt,playerData.get()));
		}
		for( PlayerVec::iterator itr = m_savePlayerVec.begin();
			itr != m_savePlayerVec.end(); 
			++itr )
		{
			playerData = dynamic_cast<crGameServerPlayerData *>(itr->get());
			savePlayerData(playerData.get());
		}

		crDataStreamTransfer::getInstance()->update(dt);
		crFileStreamCacheManager::getInstance()->update(dt);
		float updatedt = dt;
		crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_ServerUpdate,GameServer),MAKEINT64(&updatedt,NULL));
		updateDropedPlayerMap(dt);
		/////////////////区服务器信息
		//if(m_serverid == 0)
		//{
		//	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		//	ref_ptr<crDataBase> globalSession = db->beginSession();
		//	//查询并返回GameList
		//	ref_ptr<crServerListIDQueryData> serverListIDQuery = new crServerListIDQueryData;
		//	serverListIDQuery->buildQuerySql(m_gameid,m_serverName);
		//	globalSession->executeQuery(serverListIDQuery.get());
		//	crDataBase::QueryResultVec serverListIDQueryResultVec = globalSession->getQueryResult();
		//	globalSession->releaseQuery();
		//	if(serverListIDQueryResultVec.empty())
		//	{
		//		ref_ptr<crInsertServerList> insertServerList = new crInsertServerList;
		//		insertServerList->buildUpdateSql(m_gameid,m_serverName,m_isp,m_maxplayercount);
		//		if(globalSession->executeUpdate(insertServerList.get()))
		//		{//插入成功
		//			m_serverid = globalSession->getInsertID();
		//			globalSession->commit();
		//		}
		//	}
		//	else
		//	{
		//		serverListIDQuery = dynamic_cast<crServerListIDQueryData *>(serverListIDQueryResultVec[0].get());
		//		m_serverid = serverListIDQuery->getID();
		//		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		//		if(gameServer)
		//		{
		//			//unsigned short playcount = m_netConductor->getNetDataManager()->getPlayerCount();
		//			unsigned short remaincount = m_maxplayercount - m_netConductor->getNetDataManager()->getPlayerCount();
		//			m_lastcount = remaincount;
		//			crServerListUpdatePacket packet;
		//			crServerListUpdatePacket::buildRequestPacket(packet,m_serverid,remaincount);
		//			gameServer->getNetManager()->sendPacket("all",packet);
		//		}
		//	}
		//	db->endSession(globalSession.get());
		//}
		if(m_serverid>0)
		{
			m_updateTime += dt;
			if(m_updateTime>m_updateInterval)
			{
				m_updateTime = 0;
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
				if(gameServer)
				{
					//unsigned short playcount = m_netConductor->getNetDataManager()->getPlayerCount();
					short onlinecount = m_netConductor->getNetDataManager()->getPlayerCount();
					short remaincount = crGlobalHandle::gOpenGameLogin()?m_maxplayercount - onlinecount:-1000;
					_crInt32 count = MAKEINT32(onlinecount,remaincount);
					if(m_lastcount != count)
					{
						m_lastcount = count;
						crServerListUpdatePacket packet;
						crServerListUpdatePacket::buildRequestPacket(packet,m_serverid,remaincount,onlinecount);
						gameServer->getNetManager()->sendPacket("all",packet);
					}
				}
			}
		}
	}
	if(m_shutdownTime>0.0f)
	{
		m_shutdownTime -= dt;
		if(m_shutdownTime<0.0f)
		{
			crNetConductor *dbserver = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
			if(dbserver)
			{
				crShutDownServerPacket packet;
				dbserver->getNetManager()->sendPacket("all",packet);
			}
			crThread::yieldCurrentThread();
			crNetContainer::getInstance()->shutdown();
		}
	}
}
void crGameServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
			if(connectServer)
			{//GameServer异常，断开与玩家的连接
				if(m_reconnectCount>=3)
				{
					std::string logdata = "与GameServer重连次数超过3次"+m_netConductor->getName()+" 自动重启！";
					CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
					_asm   int   3   //只是为了让程序崩溃
				}
				m_reconnectCount++;
				connectServer->getNetManager()->disconnectAll();
				crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			}
			else
			{//SceneServer
				std::string logdata = "与GameServer断开连接"+m_netConductor->getName()+" 自动重启！";
				CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
				_asm   int   3   //只是为了让程序崩溃
			}
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}
void crGameServerCallback::serverShutdown()
{

}
void crGameServerCallback::clientShutdown()
{
}
bool crGameServerCallback::clientReconnectToServer()
{
	if(m_netConductor->getNetType() == SceneServerClient_Game)
	{
		CRCore::notify(CRCore::FATAL)<<"SceneServer服务器与GameServer断开，自动重启！"<<std::endl;
		_asm   int   3   
	}
	else
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		int code = netManager->clientReconnectToServerCircle(1);
		if(code>0)
		{//重新连接成功
			CRCore::notify(CRCore::ALWAYS)<<"crGameServerCallback::clientReconnectToServer 重新连接成功"<<std::endl;
			////sprintf(gDebugInfo->buf(),"crGameServerCallback::clientUpdateImplementation 重新连接成功\n\0");
			////gDebugInfo->debugInfo(CRCore::ALWAYS);
			//if(m_netConductor->getNetType() == SceneServerClient_Game)
			//{
			//	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			//	if(sceneServer)
			//	{//需要重新注册
			//		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			//		callback->registerSceneServer(m_netConductor);
			//	}
			//}
			return true;
		}
	}
	return false;
}
void crGameServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	CRCore::notify(CRCore::ALWAYS)<<"crGameServerCallback::serverRemoveConnectionImplementation  "<<address<<std::endl;
	//sprintf(gDebugInfo->buf(),"crGameServerCallback::serverRemoveConnectionImplementation %s\n\0",address.c_str());
	//gDebugInfo->debugInfo(CRCore::ALWAYS);

	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crGameServerPlayerData *playerData;
	std::vector< ref_ptr<crGameServerPlayerData> > playerEraseList;
	std::vector< ref_ptr<crGameServerPlayerData> > playerEraseList2;
	std::vector< ref_ptr<crGameServerPlayerData> >::iterator eitr;
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
		if(playerData)
		{
			if(address.compare(playerData->getPlayerConnectServerAddress()) == 0)
			{
				playerEraseList.push_back(playerData);
			}
			else if(address.compare(playerData->getSceneServerAddress()) == 0)
			{//sceneServer掉线
				playerEraseList2.push_back(playerData);
			}
		}
	}
	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerMap.erase((*eitr)->getPlayerID());
		//netDataManager->removePlayerData((*eitr)->getPlayerID());
	}
	netDataManager->unLockPlayerDataMap();

	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerDroped(eitr->get());
	}
	if(!playerEraseList2.empty())
	{//通知玩家退回到领地
		crNetManager *netManager = m_netConductor->getNetManager();
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_SceneServerAbnormal,NULL);
		ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
		int bufsize = packetStream->getBufSize();
		std::vector< ref_ptr<crGameServerPlayerData> >::iterator eitr2;
		for( eitr2=playerEraseList2.begin();
			eitr2!=playerEraseList2.end();
			++eitr2 )
		{
			//crKickoffPlayerPacket::buildReplyPacket(kpacket,(*eitr2)->getPlayerID());
			(*eitr2)->getPlayerGameData()->doEvent(MAKEINT64(WCH_SceneServerAbnormal,GameServer));
			packetStream->seekBegin();
			packetStream->_writeInt((*eitr2)->getPlayerID());
			packetStream->setBufSize(bufsize);
			netManager->sendPacket((*eitr2)->getPlayerConnectServerAddress(),packet);
			(*eitr2)->setSceneServerAddress("");
			(*eitr2)->setSceneID(0);
			//netDataManager->removePlayerData((*eitr2)->getPlayerID());
		}
	}
	{
		GNE::LockMutex lock( m_sceneServerMapMutex );
		for( SceneServerMap::iterator itr = m_sceneServerMap.begin();
			 itr != m_sceneServerMap.end();
			 ++itr )
		{
			if(itr->second.second.compare(address) == 0)
			{
				itr->second.second = "";
				crServerBrainHandle::getInstance()->doEvent(WCH_SceneServerAbnormal,MAKEINT64(itr->first,NULL));
			}
		}
	}
	m_dropedServerCount++;
	if(m_dropedServerCount>=m_maxDropServerCount/* && m_dropedTimer == 0.0f*/)
	{
		//m_dropedTimer = 60.0f;
		m_dropedSave = true;
		//crServerNeedRestartPacket packet;
		//m_netConductor->getNetManager()->sendPacket("all",packet);
		std::string logdata = "掉线服务器个数超过"+crArgumentParser::appItoa(m_maxDropServerCount)+"个"+m_netConductor->getName();//+" 将在60s后自动重启！";
		CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
		GameLogData gamelog(0,logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(0,&gamelog));
		m_dropedServerCount = 0;
	}
}
void crGameServerCallback::playerDroped(crPlayerData *_playerData)
{
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(_playerData);
	//数据库设成离线状态。通知我的好友及群成员，我离线了
	//ref_ptr<crDataBase> globalSession;
	//crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	//globalSession = db->beginSession();
	//if(globalSession.valid())
	//{
	//	ref_ptr<crUpdateGameState> updateGameState = new crUpdateGameState;
	//	updateGameState->buildUpdateSql(playerData->getPlayerID(),0);
	//	if(globalSession->executeUpdate(updateGameState.get()))
	//	{//插入成功
	//		globalSession->commit();
	//	}
	//}
	//db->endSession(globalSession.get());
	//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	//ref_ptr<crUpdateOnlineTime> updateOnlineTime = new crUpdateOnlineTime;
	//updateOnlineTime->buildUpdateSql(playerData->getPlayerID());
	//crDBSavePacket packet;
	//crDBSavePacket::buildRequestPacket(packet,updateOnlineTime.get());
	//dbConductor->getNetManager()->sendPacket("all",packet);
	ref_ptr<crPlayerGameData> playerGameData = playerData->getPlayerGameData();
	if(playerGameData.valid())
	{
		playerGameData->doEvent(MAKEINT64(WCH_ServerDroped,GameServer),MAKEINT64(_playerData,NULL));
	}
	savePlayerData(playerData.get());
	removeCharacterName(playerData->getPlayerID());
	insertDropedPlayer(playerData.get());
}
void crGameServerCallback::savePlayerData(crGameServerPlayerData *playerData)
{
	crPlayerGameData *playerGameData = playerData->getPlayerGameData();
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if(dbConductor && playerGameData && playerGameData->getDBValid() && playerGameData->getDataClass())
	{
		//crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
		//if(gamedb)
		//{
			//GNE::LockMutex lock( m_saveMutex );
			playerData->saveTimer() = m_saveInterval;
			int playerid = playerData->getPlayerID();
			crDBSavePacket packet;
			//ref_ptr<crDataBase> gameSession = gamedb->beginSession();
			//bool success = true;
			do 
			{
				//void *param;
				crData *data = playerGameData->getDataClass();
				if(data)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
					//data->getParam(WCHDATA_DataStream,param);
					if(stream->getRemainSize()>0)
					{
						ref_ptr<crUpdatePlayerGameData> updatePlayerGameData = new crUpdatePlayerGameData;
						//ref_ptr<crStreamBuf> stream = *((ref_ptr<crStreamBuf>*)param);
						updatePlayerGameData->buildUpdateSql(playerGameData->getID(),stream.get());
						crDBSavePacket::buildRequestPacket(packet,GameDB,playerData->getGameDBID(),updatePlayerGameData.get());
						dbConductor->getNetManager()->sendPacket("all",packet);
						//if(gameSession->executeUpdate(updatePlayerGameData.get()))
						//{
						//	//gameSession->commit();
						//	CRCore::notify(CRCore::ALWAYS)<<"SavePlayerGameData playerid = "<<playerid<<std::endl;
						//}
						//else
						//{
						//	success = false;
						//	break;
						//}
						////gamedb->endSession(gameSession.get());
					}
					else
					{
						std::string logdata = "Save playergamedata Faild,stream is NULL,playerid=" + crArgumentParser::appItoa(playerid);
						GameLogData gamelog(0,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
						CRCore::notify(CRCore::ALWAYS)<<"Save playergamedata Faild,stream is NULL,playerid="<<playerid<<std::endl;
					}
				}

				crRole *mainRole = playerGameData->getMainRole();
				if(mainRole)
				{//save mainrole
					int instanceitemid = mainRole->getInstanceItemID();
					int sceneid = mainRole->getSceneID();
					unsigned short layerid = mainRole->getLayerID();
					int posx = mainRole->getPosx();
					int posy = mainRole->getPosy();
					float dirx = mainRole->getDirx();
					float diry = mainRole->getDiry();
					float dirz = mainRole->getDirz();
					ref_ptr<crUpdateInstanceItem> updateItem = new crUpdateInstanceItem;
					//ref_ptr<crStreamBuf> itemStream;
					data = mainRole->getDataClass();
					if(data)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
						//data->getParam(WCHDATA_DataStream,param);
						if(stream->getRemainSize()>0)
						{
							//itemStream = *((ref_ptr<crStreamBuf>*)param);

							updateItem->buildUpdateSql(instanceitemid,sceneid,layerid,posx,posy,dirx,diry,dirz,stream.get());
							crDBSavePacket::buildRequestPacket(packet,GameDB,playerData->getGameDBID(),updateItem.get());
							dbConductor->getNetManager()->sendPacket("all",packet);
							//if(gameSession->executeUpdate(updateItem.get()))
							//{
							//	//gameSession->commit();
							//	CRCore::notify(CRCore::ALWAYS)<<"Save MainRole instanceitemid = "<<instanceitemid<<std::endl;
							//}
							//else
							//{
							//	success = false;
							//	break;
							//}
						}
						else
						{
							std::string logdata = "Save MainRole Faild,stream is NULL,playerid=" + crArgumentParser::appItoa(playerid);
							GameLogData gamelog(0,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
							CRCore::notify(CRCore::ALWAYS)<<"Save MainRole Faild,stream is NULL,playerid="<<playerid<<std::endl;
						}
					}
					//SaveItemChild
					mainRole->lockChildItemMap();
					crInstanceItem *item;
					crInstanceItem::ChildItemMap &childItemMap = mainRole->getChildItemMap();
					for( crInstanceItem::ChildItemMap::iterator itr = childItemMap.begin();
						itr != childItemMap.end();
						++itr )
					{
						item = itr->second->getInstanceItem();
						instanceitemid = item->getInstanceItemID();
						if(instanceitemid>0)
						{//需要存储
							data = item->getDataClass();
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
							//data->getParam(WCHDATA_DataStream,param);
							if(stream->getRemainSize()>0)
							{
								//itemStream = *((ref_ptr<crStreamBuf>*)param);
								sceneid = item->getSceneID();
								layerid = item->getLayerID();
								posx = item->getPosx();
								posy = item->getPosy();
								dirx = item->getDirx();
								diry = item->getDiry();
								dirz = item->getDirz();

								updateItem->buildUpdateSql(instanceitemid,sceneid,layerid,posx,posy,dirx,diry,dirz,stream.get());
								crDBSavePacket::buildRequestPacket(packet,GameDB,playerData->getGameDBID(),updateItem.get());
								dbConductor->getNetManager()->sendPacket("all",packet);
								//if(gameSession->executeUpdate(updateItem.get()))
								//{
								//	//gameSession->commit();
								//	CRCore::notify(CRCore::ALWAYS)<<"Save MainRole instanceitemid = "<<instanceitemid<<std::endl;
								//}
								//else
								//{
								//	success = false;
								//	break;
								//}
							}
							else
							{
								CRCore::notify(CRCore::ALWAYS)<<"Save MainRoleChildItem Faild,stream is NULL,playerid="<<playerid<<" instanceitemid="<<instanceitemid<<std::endl;
							}
						}
					}
					mainRole->unlockChildItemMap();
					//if(!success) break;
					data = mainRole->getMetierDataClass();
					if(data)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
						//data->getParam(WCHDATA_DataStream,param);
						if(stream->getRemainSize()>0)
						{
							//itemStream = *((ref_ptr<crStreamBuf>*)param);

							ref_ptr<crUpdateRole> updateRole = new crUpdateRole;
							updateRole->buildUpdateSql(mainRole->getRoleID(),stream.get());
							crDBSavePacket::buildRequestPacket(packet,GameDB,playerData->getGameDBID(),updateRole.get());
							dbConductor->getNetManager()->sendPacket("all",packet);
							//if(gameSession->executeUpdate(updateRole.get()))
							//{
							//	CRCore::notify(CRCore::ALWAYS)<<"Save MainRole roleid = "<<mainRole->getRoleID()<<std::endl;
							//}
							//else
							//{
							//	success = false;
							//	break;
							//}
						}
						else
						{
							std::string logdata = "Save MainRole Faild,stream is NULL,playerid=" + crArgumentParser::appItoa(playerid);
							GameLogData gamelog(0,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
							CRCore::notify(CRCore::ALWAYS)<<"Save MainRole Faild,stream is NULL,playerid="<<playerid<<std::endl;
						}
					}
				}
			} while (0);
			//if(success)
			//{
			//	CRCore::notify(CRCore::ALWAYS)<<"SavePlayerGameData Success playerid = "<<playerid<<std::endl;
			//	gameSession->commit();
			//}
			//else
			//{
			//	CRCore::notify(CRCore::ALWAYS)<<"SavePlayerGameData Failed playerid = "<<playerid<<std::endl;
			//	gameSession->rollback();
			//}
			//gamedb->endSession(gameSession.get());
		//}
	}
}
bool crGameServerCallback::insertCharacterName(int playerid,const std::string &name)
{
	GNE::LockMutex lock( m_characterNameMapMutex );
	for( CharacterNameMap::iterator itr = m_characterNameMap.begin();
		 itr != m_characterNameMap.end();
		 ++itr )
	{
		if(itr->second.compare(name) == 0)
		{
			return itr->first == playerid;
		}
	}
	crDataBase *globalDB = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = globalDB->beginSession();
	ref_ptr<crCharacterNameValidQueryData> characterNameValidQuery = new crCharacterNameValidQueryData;
	characterNameValidQuery->buildQuerySql(name,m_serverid);

	globalSession->executeQuery(characterNameValidQuery.get());
	crDataBase::QueryResultVec &characterNameValidQueryResultVec = globalSession->getQueryResult();
	int existplayerid = 0;
	if(!characterNameValidQueryResultVec.empty())
	{
		characterNameValidQuery = dynamic_cast<crCharacterNameValidQueryData *>(characterNameValidQueryResultVec[0].get());
		existplayerid = characterNameValidQuery->getPlayeID();
	}
	globalSession->releaseQuery();
	globalDB->endSession(globalSession.get());

	bool valid = false;
	if(existplayerid == 0 || existplayerid == playerid)
		valid = true;
	if(valid)
	{
		m_characterNameMap[playerid] = name;
	}
	return valid;
}
bool crGameServerCallback::findCharacterName(int playerid, std::string &name)
{
	GNE::LockMutex lock( m_characterNameMapMutex );
	CharacterNameMap::iterator itr = m_characterNameMap.find(playerid);
	if(itr != m_characterNameMap.end())
	{
		name = itr->second;
		return true;
	}
	return false;
}
void crGameServerCallback::removeCharacterName(int playerid)
{
	GNE::LockMutex lock( m_characterNameMapMutex );
	m_characterNameMap.erase(playerid);
}
void crGameServerCallback::registerSceneServer(int sceneid,const std::string &name,const std::string &address)
{
	GNE::LockMutex lock( m_sceneServerMapMutex );
	m_sceneServerMap[sceneid] = std::make_pair(name,address);
	m_sceneServerNameMap[name]=address;
	//CRCore::notify(CRCore::ALWAYS)<<"registerSceneServer Server:"<<name<<" sceneid:"<<sceneid<<" SceneServerMapCount:"<<m_sceneServerMap.size()<<std::endl;
}
std::string crGameServerCallback::findSceneServerName(int sceneid)
{
	GNE::LockMutex lock( m_sceneServerMapMutex );
	SceneServerMap::iterator itr = m_sceneServerMap.find(sceneid);
	return itr != m_sceneServerMap.end()?itr->second.first:"";
}
std::string crGameServerCallback::findSceneServerAddress(int sceneid)
{
	GNE::LockMutex lock( m_sceneServerMapMutex );
	SceneServerMap::iterator itr = m_sceneServerMap.find(sceneid);
	return itr != m_sceneServerMap.end()?itr->second.second:"";
}
void crGameServerCallback::sendPacketToSceneServer(const std::string &sceneservername,GNE::Packet &packet)//"all"表示发送给所有sceneserver
{
	GNE::LockMutex lock( m_sceneServerMapMutex );
	crNetManager *netManager = m_netConductor->getNetManager();
	if (sceneservername.compare("all") != 0)
	{
		SceneServerNameMap::iterator itr = m_sceneServerNameMap.find(sceneservername);
		if(itr!=m_sceneServerNameMap.end())
			netManager->sendPacket(itr->second,packet);
	}
	else
	{
		for( SceneServerNameMap::iterator itr = m_sceneServerNameMap.begin();
			 itr != m_sceneServerNameMap.end();
			 ++itr )
		{
			netManager->sendPacket(itr->second,packet);
		}
	}
}
std::string crGameServerCallback::getSceneServerAddressBySceneServerName(const std::string &sceneservername)
{
	GNE::LockMutex lock( m_sceneServerMapMutex );
	SceneServerNameMap::iterator itr = m_sceneServerNameMap.find(sceneservername);
	return itr != m_sceneServerNameMap.end()?itr->second:"";
}
void crGameServerCallback::insertDelayRemovePlayer(crGameServerPlayerData *player)
{
	GNE::LockMutex lock( m_delayRemovePlayerMapMutex );
	m_delayRemovePlayerMap[player] = 10.0f;
}
//bool crGameServerCallback::foundDelayRemovePlayer(int playerid)
//{
//	GNE::LockMutex lock( m_delayRemovePlayerMapMutex );
//	return m_delayRemovePlayerMap.find(playerid)!=m_delayRemovePlayerMap.end();
//}
void crGameServerCallback::insertDropedPlayer(crGameServerPlayerData* player)
{
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	m_dropedPlayerMap[player] = 600.0f;
}
void crGameServerCallback::removeDropedPlayer(crGameServerPlayerData *player)
{
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	m_dropedPlayerMap.erase(player);
}
//bool crGameServerCallback::foundDropedPlayer(int playerid, CRCore::ref_ptr<crGameServerPlayerData>&outplayer)
//{
//	GNE::LockMutex lock( m_dropedPlayerMapMutex );
//	for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
//		 itr != m_dropedPlayerMap.end();
//		 ++itr )
//	{
//		if(itr->first->getPlayerID() == playerid)
//		{
//			outplayer = itr->first.get();
//			return true;
//		}
//	}
//	return false;
//}
int crGameServerCallback::remuseDropedPlayer(int playerid,const std::string &key,CRCore::ref_ptr<crGameServerPlayerData>&outplayer)
{//0:未找到,1重连,-1存储保护时间内
	ref_ptr<crGameServerPlayerData> playerdata;
	{
		GNE::LockMutex lock( m_delayRemovePlayerMapMutex );
		for( DelayRemovePlayerMap::iterator itr = m_delayRemovePlayerMap.begin();
			 itr != m_delayRemovePlayerMap.end();
			 ++itr )
		{
			playerdata = itr->first;
			if(playerdata->getPlayerID() == playerid)
			{
				outplayer = playerdata;
				m_delayRemovePlayerMap.erase(itr);
				if(playerdata->checkKey(key))
				{
					//m_delayRemovePlayerMap.erase(itr);
					return 1;
				}
				return 0;
			}
		}
	}
	{
		GNE::LockMutex lock( m_dropedPlayerMapMutex );
		for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
			 itr != m_dropedPlayerMap.end();
			 ++itr )
		{
			playerdata = itr->first;
			if(playerdata->getPlayerID() == playerid)
			{
				outplayer = playerdata;
				m_dropedPlayerMap.erase(itr);
				if(playerdata->checkKey(key))
				{
					//m_dropedPlayerMap.erase(itr);
					return 1;
				}
				return 0;
			}
		}
	}
	return 0;
}
void crGameServerCallback::updateDropedPlayerMap(float dt)
{
	{
		ref_ptr<crGameServerPlayerData> playerData;
		std::vector<int> EraseVec;
		m_delayRemovePlayerMapMutex.acquire();
		for( DelayRemovePlayerMap::iterator itr = m_delayRemovePlayerMap.begin();
			itr != m_delayRemovePlayerMap.end();)
		{
			itr->second -= dt;
			if(itr->second<0.0f)
			{
				playerData = itr->first;
				//playerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(itr->first));
				//if(playerData.valid())
				//{
					//playerDroped(playerData.get());
				EraseVec.push_back(playerData->getPlayerID());
				//}
				itr = m_delayRemovePlayerMap.erase(itr);
			}
			else
			{
				++itr;
			}
		}
		m_delayRemovePlayerMapMutex.release();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		for( std::vector<int>::iterator itr = EraseVec.begin();
			 itr != EraseVec.end();
			 ++itr )
		{
			netDataManager->removePlayerData(*itr);
		}
	}
	{
		m_dropedPlayerMapMutex.acquire();
		ref_ptr<crGameServerPlayerData> playerdata;
		for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
			itr != m_dropedPlayerMap.end();)
		{
			itr->second -= dt;
			if(itr->second<0.0f)
			{
				playerdata = itr->first;
				playerdata->clearData();
				itr = m_dropedPlayerMap.erase(itr);
			}
			else
			{
				++itr;
			}
		}
		m_dropedPlayerMapMutex.release();
	}
}
//////区服务器信息
void crGameServerCallback::setServerID(int serverid)
{
	m_serverid = serverid;
}
int crGameServerCallback::getServerID()
{
	return m_serverid;
}
int crGameServerCallback::getWebServerID()
{
	return m_webserverid;
}
const std::string &crGameServerCallback::getServerName() const
{
	return m_serverName;
}
const std::string &crGameServerCallback::getSite() const
{
	return m_site;
}
unsigned char crGameServerCallback::getISP()
{
	return m_isp;
}
short crGameServerCallback::getMaxPlayerCount()
{
	return m_maxplayercount;
}
void crGameServerCallback::setMaxPlayerCount(short count)
{
	m_maxplayercount = count;
	m_updateTime = m_updateInterval;
}
short crGameServerCallback::getRemainPlayerCount()
{
	return m_maxplayercount - m_netConductor->getNetDataManager()->getPlayerCount();
}
void crGameServerCallback::shutDown()
{
	crShutDownServerPacket packet;
	m_netConductor->getNetManager()->sendPacket("all",packet);
	m_shutdownTime = 10.0f;
}
///////////////////////////////////
//
//crGameConnectServerCallback
//
/////////////////////////////////////
crGameConnectServerCallback::crGameConnectServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_firstUpdated(false),
m_updateInterval(5.0f),
m_updateTime(0.0f),
m_lastcount(0),
m_proxyport(0),
m_inreconnect(0)
{
}

crGameConnectServerCallback::crGameConnectServerCallback(const crGameConnectServerCallback& callback):
crNetCallback(callback),
m_firstUpdated(callback.m_firstUpdated),
m_updateInterval(callback.m_updateInterval),
m_updateTime(0),
m_lastcount(0),
m_inreconnect(0)
{
}

void crGameConnectServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crGameConnectServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		//sprintf(gDebugInfo->buf(),"crGameConnectServerCallback::serverInitImplementation: file open error,%s\n\0",file.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		cfg_script.Get("UpdateInterval", m_updateInterval);
		m_updateInterval = CRCore::minimum(m_updateInterval,50.0f);
		//cfg_script.Get("IP", m_ip);
		//cfg_script.Get("IP2", m_ip2);
		//cfg_script.Get("IP3", m_ip3);
		//int int1;
		//if(cfg_script.Get("ProxyPort", int1))
		//	m_proxyport = int1;
		//else
		//{
		//	cfg_script.Get("Port", int1);
		//	m_proxyport = int1;
		//}
		CRCore::ref_ptr<CREncapsulation::crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
		CREncapsulation::crTableIO::StrVec record;
		int ipindex = cfgtable->getTitleIndex("IP");
		int portindex = cfgtable->getTitleIndex("Port");
		int ip2index = cfgtable->getTitleIndex("IP2");
		int ip3index = cfgtable->getTitleIndex("IP3");
		int proxyportindex = cfgtable->getTitleIndex("ProxyPort");
		if(cfgtable->queryOneRecord(0,m_netConductor->getKeyStr(),record)>=0)
		{
			m_ip = record[ipindex];
			m_ip2 = record[ip2index];
			m_ip3 = record[ip3index];
			if(record[proxyportindex].empty())
			{
				m_proxyport = atoi(record[portindex].c_str());
			}
			else
			{
				m_proxyport = atoi(record[proxyportindex].c_str());
			}
		}
		CRCore::notify(CRCore::ALWAYS)<<"crGameConnectServerCallback ip:"<<m_ip<<"ip2:"<<m_ip2<<"ip3:"<<m_ip3<<":"<<m_proxyport<<std::endl;
		//sprintf(gDebugInfo->buf(),"crGameConnectServerCallback ip:%s ip2:%s :%d\n\0",m_ip.c_str(),m_ip2.c_str(),m_port);
		//gDebugInfo->debugInfo(CRCore::FATAL);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crGameConnectServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crGameConnectServerCallback::clientInitImplementation()
{
}

void crGameConnectServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
		m_updateTime += dt;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Game);
		if(gameServer)
		{
			short remaincount = m_netConductor->getNetManager()->getRemainConnectCount();
			if(!m_firstUpdated)
			{
				crConnectServerInitPacket packet;
				crConnectServerInitPacket::buildRequestPacket(packet,m_ip,m_ip2,m_ip3,m_proxyport,remaincount);
				gameServer->getNetManager()->sendPacket("all",packet);
				m_firstUpdated = true;
			}
			else if(m_updateTime>m_updateInterval)
			{
				if(m_lastcount != remaincount || m_updateTime>50.0f)
				{
					m_lastcount = remaincount;
					crConnectServerUpdatePacket packet;
					crConnectServerUpdatePacket::buildRequestPacket(packet,m_proxyport,remaincount);
					gameServer->getNetManager()->sendPacket("all",packet);
					m_updateTime = 0.0f;
				}
			}
		}
		updateDropedPlayerMap(dt);
	}
}

void crGameConnectServerCallback::clientUpdateImplementation(float dt)
{
	crGlobalHandle::getInstance()->timer()+=dt;
	if(!m_netConductor->getNetManager()->isRunning())
	{//客户端断线不进行重连接
		//crGlobalHandle::getInstance()->doEvent(WCH_GameConnectServerAbnormal,MAKEINT64(NULL,NULL));
		if(m_inreconnect<30)
		{
			crThread::sleep(100);
			crNetManager *netManager = m_netConductor->getNetManager();
			int code = netManager->clientReconnectToServerCircle(1);
			if(code>0)
			{//重新连接成功
				CRCore::notify(CRCore::ALWAYS)<<"crGameConnectServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
				//发送重连接包
				crClientReconnectPacket packet;
				crClientReconnectPacket::buildRequestPacket(packet,crMyPlayerData::getInstance()->getPlayerID());
				m_netConductor->getNetManager()->sendPacket("all",packet);
			}
			else
			{
				//crGlobalHandle::getInstance()->doEvent(WCH_GameConnectServerAbnormal);
				m_inreconnect++;
			}
		}
		else
		{
			crGlobalHandle::getInstance()->doEvent(WCH_GameConnectServerAbnormal);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crGameConnectServerCallback::clientUpdateImplementation Net已经断开"<<std::endl;
		//crNetWorkUpdater::getInstance()->done();
	}
	else
	{
		m_inreconnect = 0;
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
	//else
	//{
	//	CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	//	if(m_time == 0)
	//	{
	//		m_time = t1;
	//		return;
	//	}

	//	float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
	//	m_time = t1;
	//	crMyPlayerData::getInstance()->update(dt);
	//}
}

void crGameConnectServerCallback::serverShutdown()
{

}

void crGameConnectServerCallback::clientShutdown()
{
}
void crGameConnectServerCallback::removeConnection(CRNet::crPlayerData *_playerData)
{
	ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(_playerData);
	insertDropedPlayer(playerData.get());
}
void crGameConnectServerCallback::insertDropedPlayer(crGameConnectServerPlayerData* player)
{
	if(player)
	{
		ref_ptr<crGameConnectServerPlayerData> playerdata;
		GNE::LockMutex lock( m_dropedPlayerMapMutex );
		for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
			itr != m_dropedPlayerMap.end();
			++itr )
		{
			playerdata = itr->first;
			if(playerdata->getPlayerID() == player->getPlayerID())
			{
				m_dropedPlayerMap.erase(itr);
				break;
			}
		}
		m_dropedPlayerMap[player] = 20.0f;
	}
}
void crGameConnectServerCallback::removeDropedPlayer(crGameConnectServerPlayerData *player)
{
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	m_dropedPlayerMap.erase(player);
}
void crGameConnectServerCallback::remuseDropedPlayer(int playerid,CRCore::ref_ptr<crGameConnectServerPlayerData>&outplayer)
{
	ref_ptr<crGameConnectServerPlayerData> playerdata;
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
		itr != m_dropedPlayerMap.end();
		++itr )
	{
		playerdata = itr->first;
		if(playerdata->getPlayerID() == playerid)
		{
			outplayer = playerdata;
			m_dropedPlayerMap.erase(itr);
			break;
		}
	}
}
void crGameConnectServerCallback::updateDropedPlayerMap(float dt)
{
	crConnectServerConductor *connectConductor = dynamic_cast<crConnectServerConductor *>(m_netConductor);
	ref_ptr<crGameConnectServerPlayerData> playerdata;
	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	int playerid;
	m_dropedPlayerMapMutex.acquire();
	for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
		itr != m_dropedPlayerMap.end();)
	{
		itr->second -= dt;
		if(itr->second<0.0f)
		{
			//send logoutpkg
			if(connectConductor)
			{
				playerdata = itr->first;
				playerid = playerdata->getPlayerID();
				if(!netDataManager->getPlayerData(playerid))
					connectConductor->playerLogout(playerid);
			}
			itr = m_dropedPlayerMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	m_dropedPlayerMapMutex.release();
}
///////////////////////////////////
//
//crSceneServerCallback
//
/////////////////////////////////////
crSceneServerCallback::crSceneServerCallback():
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_saveInterval(600.0f),//10分钟自动存储一次
m_reliveInterval(2.0f),
m_reliveTime(0.0f),
m_shutdownTime(0.0f),
m_initscenecount(0),
m_initend(false),
m_maxRoomCount(0),
m_saveRole(true),
m_dropedServerCount(0),
//m_dropedTimer(0.0f),
m_maxDropServerCount(9)/*,
m_reconnectCount(0)*/
{
}

crSceneServerCallback::crSceneServerCallback(const crSceneServerCallback& callback):
crNetCallback(callback),
m_pingInterval(10.0f),
m_pingTime(0.0f),
m_saveInterval(600.0f),//10分钟自动存储一次
m_reliveInterval(2.0f),
m_reliveTime(0.0f),
m_shutdownTime(0.0f),
m_initscenecount(0),
m_initend(false),
m_maxRoomCount(0),
m_saveRole(true),
m_dropedServerCount(0),
//m_dropedTimer(0.0f),
m_maxDropServerCount(9)/*,
m_reconnectCount(0)*/
{
}

void crSceneServerCallback::serverInitImplementation()
{
	m_initscenecount = 0;
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crSceneServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	//ref_ptr<crData> data = CREncapsulation::loadData("server/sceneserverevent.cfg");
	//crGlobalHandle::getInstance()->setDataClass(data.get());
	crData *data = crGlobalHandle::getInstance()->getDataClass();
	CREncapsulation::loadIncludeData("server/sceneserverevent.cfg",data);

	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
	std::string sceneName;
	int pathFindingCount = 10;
	cfg_script.Get("Emporium", m_emporiumScript);
	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		CRCore::ref_ptr<CREncapsulation::crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
		CREncapsulation::crTableIO::StrVec record;
		int gameindex = cfgtable->getTitleIndex("GameID");
		cfgtable->queryOneRecord(0,"GameServer",record);
		m_gameName = record[gameindex];

		//cfg_script.Get("GameName", m_gameName);
		crGlobalHandle::getInstance()->setGameName(m_gameName);
		std::string globalData = m_gameName + "/GlobalData.cfg";
		CREncapsulation::loadIncludeData(globalData,data);
		std::string brainData = m_gameName+"/SceneServerBrain.cfg";
		data = CREncapsulation::loadData(brainData);
		crServerBrainHandle::getInstance()->setDataClass(data);
		crServerBrainHandle::getInstance()->doEvent(WCH_ServerInit);
		cfg_script.Get("PingInterval", m_pingInterval);
		cfg_script.Get("SaveInterval", m_saveInterval);
		cfg_script.Get("MaxRoomCount",m_maxRoomCount);
		cfg_script.Get("MaxDropServerCount",m_maxDropServerCount);
		int int1;
		cfg_script.Get("SaveRole",int1);
		m_saveRole = int1;
		//if(cfg_script.Get("PathFindingCount",pathFindingCount))
		//	crPathFindingManager::setPathFindingCount(pathFindingCount);
		std::string serverName = m_netConductor->getName();
		ref_ptr<CRNetApp::crScene> scene;
		if(netConductor)
		{
			int nScene = 1;
			clearSceneMap();
			std::string file = "server/"+m_gameName+"/SceneServerTab.crb";
			ref_ptr<CREncapsulation::crTableIO> sceneTab = CREncapsulation::crTableIO::openFile(file);
			if(sceneTab.valid())
			{
				CREncapsulation::crTableIO::DataVec recordVec;
				sceneTab->queryRecords(0,m_netConductor->getNetDataManager()->getServerName(),recordVec);
				crSceneServerInitPacket packet;
				int waittime = 0;
				m_initscenecount = recordVec.size();
				for( CREncapsulation::crTableIO::DataVec::iterator itr = recordVec.begin();
					itr != recordVec.end();
					++itr )
				{
					sceneName = (*itr)[1];
					if(!sceneName.empty())
					{
						crSceneServerInitPacket::buildRequestPacket(packet,sceneName,serverName);
						netConductor->getNetManager()->sendPacket("all",packet);
					}
					else
					{
						m_initscenecount--;
					}
				}
			}
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crSceneServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		crPathFindingUpdater::getInstance()->startThread();
	}
	//sprintf(gDebugInfo->buf(),"crSceneServerCallback::serverInitImplementation() 初始化完成\n\0");
	//gDebugInfo->debugInfo(CRCore::ALWAYS);
}
void crSceneServerCallback::addScene(crScene *scene)
{
	//GNE::LockMutex lock( m_sceneMapMutex );
	m_sceneMap[scene->getSceneID()] = scene;
	if(!m_emporiumScript.empty())
	{
		ref_ptr<crInstanceItem> emporium = new crInstanceItem;
		emporium->setInstanceItemID(crGlobalHandle::gainTemporaryItemID());
		emporium->setSceneID(scene->getSceneID());
		scene->setEmporium(emporium.get());
		emporium->setItemtype(crInstanceItem::Emporium);
		emporium->setAbstractItemID(0);
		emporium->getAbstractItem()->setScript(m_emporiumScript);
		//CRCore::Timer_t mtime = 0L;
		//std::string cookfilename = m_emporiumScript;
		//struct stat buf;
		//if(!cookfilename.empty())
		//{
		//	CRIOManager::GetCookedFileName(cookfilename);
		//	if (stat( cookfilename.c_str(), &buf )) 
		//	{
		//		CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
		//	}
		//	else
		//	{
		//		mtime = buf.st_mtime;
		//	}
		//}
		//emporium->getAbstractItem()->setScriptModifyTime(mtime);
		ref_ptr<crData> data = CREncapsulation::loadData(m_emporiumScript);
		emporium->setDataClass(data.get());
		data->excHandle(MAKEINT64(WCH_ServerCheckItemData,emporium.get()));
	}
}
void crSceneServerCallback::registerSceneServer(crNetConductor *sceneclient_game)
{
	if(sceneclient_game)
	{
		crRegisterSceneServerPacket packet;
		std::string serverName = m_netConductor->getName();
		for( SceneMap::iterator itr = m_sceneMap.begin();
			itr != m_sceneMap.end();
			++itr )
		{
			crRegisterSceneServerPacket::buildRequestPacket(packet,itr->first,serverName);
			sceneclient_game->getNetManager()->sendPacket("all",packet);
		}
	}

}
CRNetApp::crScene *crSceneServerCallback::findScene(int sceneid)
{
	//GNE::LockMutex lock( m_sceneMapMutex );
	SceneMap::iterator itr = m_sceneMap.find(sceneid);
	return itr != m_sceneMap.end()?itr->second.get():NULL;
}
void crSceneServerCallback::initSuccess()
{
    m_initscenecount--;
	CRCore::notify(CRCore::ALWAYS)<<"crSceneServerCallback 初始化剩余个数："<<m_initscenecount<<std::endl;
	if(m_initscenecount==0)
	{
		crSceneServerInitEndPacket packet;
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
void crSceneServerCallback::initend()
{
	CRCore::notify(CRCore::ALWAYS)<<"crSceneServerCallback 初始化完成"<<std::endl;
	m_initend = true;
	crServerBrainHandle::getInstance()->doEvent(WCH_ServerInitSuccess);
}
bool crSceneServerCallback::checkScene(int sceneid)
{
	if(sceneid == -1)
		return true;
    return findScene(sceneid) != NULL;
}
void crSceneServerCallback::clearSceneMap()
{
	//GNE::LockMutex lock( m_sceneMapMutex );
	m_sceneMap.clear();
}
void crSceneServerCallback::clientInitImplementation()
{
}

void crSceneServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		//if(m_dropedTimer>0.0f)
		//{
		//	m_dropedTimer -= dt;
		//	if(m_dropedTimer<=0.0f)
		//	{
		//		m_dropedTimer = 0.0f;
		//		CRCore::notify(CRCore::FATAL)<<"SceneServer服务器自动重启！"<<std::endl;
		//		_asm   int   3   //只是为了让程序崩溃
		//	}
		//}
		if(m_pingInterval>0.0f)
		{
		    m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
		//crRole *role,*role2;
		//void *param;
		//crData *data;
		//unsigned int guiseState;
		//crInstanceItem *item;
		//crScene *scene;
		//int sceneid,roomid,playerid,layerid;
		//crSightInfo *roleSight;
		//crRoom *room;
		crSceneServerPlayerData *playerData;
		crPlayerGameData *playerGameData;
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		netDataManager->lockPlayerDataMap();
		crNetDataManager::PlayerDataMap& playerMap = netDataManager->getPlayerDataMap();//避免线程互锁问题
		m_savePlayerVec.resize(0);
		for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
			itr != playerMap.end();
			++itr )
		{
			playerData = dynamic_cast<crSceneServerPlayerData *>(itr->second.get());
			playerGameData = playerData->getPlayerGameData();
			if(playerGameData)
			{
				float updatedt = dt;
				playerGameData->doEvent(MAKEINT64(WCH_ServerUpdate,SceneServer),MAKEINT64(&updatedt,playerData));
			}
			if(playerData->isClientRunning())
			{
				playerData->saveTimer() -= dt;
				if(playerData->saveTimer()<=0)
				{
					m_savePlayerVec.push_back(playerData);
				}
			}
		}
		netDataManager->unLockPlayerDataMap();
		for( PlayerVec::iterator itr = m_savePlayerVec.begin();
			itr != m_savePlayerVec.end(); 
			++itr )
		{
			playerData = dynamic_cast<crSceneServerPlayerData *>(itr->get());
			savePlayerData(playerData);
		}
		//////////SceneItem Update
		if(m_initend)
		{
			//GNE::LockMutex lock( m_sceneMapMutex );
			//int sceneid;
			crScene *scene;
			for (SceneMap::iterator itr = m_sceneMap.begin();
				itr != m_sceneMap.end();
				++itr)
			{
				scene = itr->second.get();
				//sceneid = scene->getSceneID();
				scene->serverUpdate(dt,this);
			}
			roomUpdate(dt);
		}
		crDataStreamTransfer::getInstance()->update(dt);
		float updatedt = dt;
		crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_ServerUpdate,SceneServer),MAKEINT64(&updatedt,NULL));
		crGlobalHandle::updateTimeRecycleID(dt);
		updateDropedPlayerMap(dt);
		updateRemovedRoomMap(dt);
	}
	if(m_shutdownTime>0.0f)
	{
		m_shutdownTime -= dt;
		if(m_shutdownTime<0.0f)
		{
			crNetContainer::getInstance()->shutdown();
		}
	}
}

void crSceneServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			//if(m_reconnectCount>=5)
			//{
			//	std::string logdata = "与SceneServer重连次数超过5次"+m_netConductor->getName()+" 自动重启！";
			//	CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
			//	_asm   int   3   //只是为了让程序崩溃
			//}
			//m_reconnectCount++;
			//crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
			//if(connectServer)
			//{//GameServer异常，断开与玩家的连接
			//	connectServer->getNetManager()->disconnectAll();
			//}
			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
	}
}

void crSceneServerCallback::serverShutdown()
{

}

void crSceneServerCallback::clientShutdown()
{
}

bool crSceneServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crSceneServerCallback::clientUpdateImplementation 重新连接成功"<<std::endl;
		return true;
	}
	return false;
}
void crSceneServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	CRCore::notify(CRCore::ALWAYS)<<"crSceneServerCallback::serverRemoveConnectionImplementation  "<<address<<std::endl;
	//sprintf(gDebugInfo->buf(),"crSceneServerCallback::serverRemoveConnectionImplementation %s\n\0",address.c_str());
	//gDebugInfo->debugInfo(CRCore::ALWAYS);

	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crServerPlayerData *playerData;
	std::vector< ref_ptr<crServerPlayerData> > playerEraseList;
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		if(playerData)
		{
			if(address.compare(playerData->getPlayerConnectServerAddress()) == 0)
			{
				playerEraseList.push_back(playerData);
			}
		}
	}
	std::vector< ref_ptr<crServerPlayerData> >::iterator eitr;
	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerMap.erase((*eitr)->getPlayerID());
		//netDataManager->removePlayerData((*eitr)->getPlayerID());
	}
	netDataManager->unLockPlayerDataMap();


	for( eitr=playerEraseList.begin();
		eitr!=playerEraseList.end();
		++eitr )
	{
		playerDroped(eitr->get());
	}
	m_dropedServerCount++;
	if(m_dropedServerCount>=m_maxDropServerCount/* && m_dropedTimer==0.0f*/)
	{
		//m_dropedTimer = 10.0f;
		//crServerNeedRestartPacket packet;
		//m_netConductor->getNetManager()->sendPacket("all",packet);
		std::string logdata = "掉线服务器个数超过"+crArgumentParser::appItoa(m_maxDropServerCount)+"个"+m_netConductor->getName();//+" 将在10s后自动重启！";
		CRCore::notify(CRCore::ALWAYS)<<logdata<<std::endl;
		GameLogData gamelog(0,logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(0,&gamelog));
		m_dropedServerCount = 0;
		//_asm   int   3   //只是为了让程序崩溃
	}
}
void crSceneServerCallback::playerDroped(crPlayerData *_playerData/*, bool kickoff*/)
{
	crSceneServerPlayerData *playerData = dynamic_cast<crSceneServerPlayerData *>(_playerData);
	crRole *role;// = playerData->getRole();
	//playerData->lockRoleMap();
	crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
	for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
		 itr != roleMap.end();
		 ++itr )
	{
		role = itr->second.get();
		if(role)
		{
			int transportsceneid = role->getTransportSceneID();
			if(transportsceneid>0)
			{
				role->setSceneID(transportsceneid);
				role->setTransportSceneID(0);
			}
		}
	}
	int playerid = playerData->getPlayerID();
	int sceneid = playerData->getSceneID();
	crScene *scene = findScene(sceneid);
	crData *roledata;
	unsigned char itemstate = IS_Dead;
	if(scene)
	{
		//crSceneLayer *layer;
		//scene->lockRoomRoleMap();
		for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
			itr != roleMap.end();
			++itr )
		{
			role = itr->second.get();
			if(role)
			{
				//role->clearExtra();
				//layer = scene->getSceneLayer(role->getLayerID());
				role->lockChildItemMap();
				crItemChild *itemchild;
				crInstanceItem::ChildItemMap &childItemMap = role->getChildItemMap();
				for( crInstanceItem::ChildItemMap::iterator itr = childItemMap.begin();
					itr != childItemMap.end();
					++itr )
				{
					itemchild = itr->second.get();
					crGlobalHandle::recycleItemChildID(itemchild->getItemChildID());
					crGlobalHandle::recycleItemID(itemchild->getInstanceItemID());
				}
				role->unlockChildItemMap();
				roledata = role->getDataClass();
				if(roledata)
				{
					roledata->inputParam(WCHDATA_ItemState,&itemstate);
				}
				scene->removeRoomRole(role);
			}
		}
		//scene->unlockRoomRoleMap();
		crSightInfo *sightinfo = playerData->getSightInfo();
		if(sightinfo)
		{
			sightinfo->removeEyePlayer(playerid);
		}
		int roomid = playerData->getRoomID();
		crRoom *room = NULL;
		if(roomid!=0)
		{
			GNE::LockMutex lock( m_roomMapMutex );
			room = findRoom(roomid);
			if(room)
			{
				room->removeMember(playerid,true);
				if(sightinfo)
				{
					room->removeSightInfo(sightinfo);
				}
			}
		}
		if(sightinfo && !room)
		{
			scene->removeSightInfo(sightinfo);
		}
		playerData->setSightInfo(NULL);
	}
	savePlayerData(playerData,SaveAll);
	crPlayerGameData *playerGameData = playerData->getPlayerGameData();
	if (/*kickoff && */playerGameData)
	{
		playerGameData->doEvent(MAKEINT64(WCH_ServerDroped,SceneServer),MAKEINT64(_playerData,NULL));
		//crPlayerDataEventPacket packet;
		//crPlayerDataEventPacket::buildReplyPacket(packet,playerData->getPlayerID(),WCH_RecvPlayerLeaveScene,NULL);
		//crNetConductor *gameServerLine = playerData->getGameServerLineConductor();
		//if(gameServerLine) gameServerLine->getNetManager()->sendPacket("all",packet);
		//if(!playerData->getPlayerConnectServerAddress().empty())m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
	}
	insertDropedPlayer(playerData);
	//playerData->clear();
	//crRemovePlayerPacket packet;
	//crRemovePlayerPacket::buildRequestPacket(packet,0,playerid);
	//playerData->getSightInfo()->sendPacketToNeighbor(packet);	
	//playerData->unlockRoleMap();
}
void crSceneServerCallback::insertDropedPlayer(crSceneServerPlayerData* player)
{
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	m_dropedPlayerMap[player] = 5.0f;
}
void crSceneServerCallback::updateDropedPlayerMap(float dt)
{
	GNE::LockMutex lock( m_dropedPlayerMapMutex );
	ref_ptr<crSceneServerPlayerData> playerdata;
	for( DropedPlayerMap::iterator itr = m_dropedPlayerMap.begin();
		itr != m_dropedPlayerMap.end();)
	{
		itr->second -= dt;
		if(itr->second<0.0f)
		{
			playerdata = itr->first;
			playerdata->clearData();
			itr = m_dropedPlayerMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
void crSceneServerCallback::savePlayerData(crSceneServerPlayerData *playerData,SaveFlg flg)
{
	//GNE::LockMutex lock( m_saveMutex );
	playerData->saveTimer() = m_saveInterval;
	int playerid = playerData->getPlayerID();
	//CRCore::notify(CRCore::ALWAYS)<<"crSceneServerCallback::savePlayerData:"<<playerid<<std::endl;
	crNetConductor *netConductor = playerData->getGameServerLineConductor();
	if(netConductor)
	{
		//void *param;
		crData *data;
		crPlayerGameData *playerGameData = playerData->getPlayerGameData();
		if(playerGameData)
		{
			playerGameData->doEvent(WCH_SceneSavePlayerData);
			data = playerGameData->getDataClass();
			if(data)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				data->excHandle(MAKEINT64(WCH_BuildSceneSaveStream,stream.get()));
				//data->getParam(WCHDATA_DataStream,param);
				//if(param)
				//{
					//itemStream = *((ref_ptr<crStreamBuf>*)param);
					//向GameServer提交playergamedata数据以及MainRole数据
					crSavePlayerGameDataPacket ppacket;
					crSavePlayerGameDataPacket::buildRequestPacket(ppacket,playerid,stream.get());
					netConductor->getNetManager()->sendPacket("all",ppacket);
				//}
			}
		}
		if(m_saveRole)
		{
			crRole *role;
			unsigned short gamedbid;
			crSaveItemDataPacket ipacket;
			crSaveRoleDataPacket rpacket;
			crInstanceItem *item;
			//playerData->lockRoleMap();
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
				 itr != roleMap.end();
				 ++itr )
			{
				role = itr->second.get();
				if(role)
				{
					gamedbid = playerData->getGameDBID();
					if(flg & SaveItem)
					{
						data = role->getDataClass();
						if(data)
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
							//data->getParam(WCHDATA_DataStream,param);
							//if(param)
							//{
								//itemStream = *((ref_ptr<crStreamBuf>*)param);
								crSaveItemDataPacket::buildRequestPacket(ipacket,role,stream.get(),gamedbid);
								netConductor->getNetManager()->sendPacket("all",ipacket);
							//}
						}
						//SaveItemChild
						role->lockChildItemMap();
						crInstanceItem::ChildItemMap &childItemMap = role->getChildItemMap();
						for( crInstanceItem::ChildItemMap::iterator itr = childItemMap.begin();
							itr != childItemMap.end();
							++itr )
						{
							item = itr->second->getInstanceItem();
							if(item->getInstanceItemID()>0)
							{//需要存储
								data = item->getDataClass();
								if(data)
								{
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
									//data->getParam(WCHDATA_DataStream,param);
									//if(param)
									//{
										//itemStream = *((ref_ptr<crStreamBuf>*)param);
										crSaveItemDataPacket::buildRequestPacket(ipacket,item,stream.get(),gamedbid);
										netConductor->getNetManager()->sendPacket("all",ipacket);
									//}
								}
							}
						}
						role->unlockChildItemMap();
					}
					if(flg & SaveRole)
					{
						data = role->getMetierDataClass();
						if(data)
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							data->excHandle(MAKEINT64(WCH_BuildSaveStream,stream.get()));
							//data->getParam(WCHDATA_DataStream,param);
							//if(param)
							//{
								//itemStream = *((ref_ptr<crStreamBuf>*)param);
								crSaveRoleDataPacket::buildRequestPacket(rpacket,role,stream.get(),gamedbid);
								netConductor->getNetManager()->sendPacket("all",rpacket);
							//}
						}
					}
				}
			}
			//playerData->unlockRoleMap();
		}
	}
}
//void crSceneServerCallback::sendPacketToItemNeighbor(crInstanceItem *item,CRNet::crStreamPacket &packet)
//{
//	//if(item/* && item->getItemtype() != crInstanceItem::Role*/)
//	//{
//	//	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//	//	crNetManager *netManager = m_netConductor->getNetManager();
//	//	netDataManager->lockPlayerDataMap();
//	//	crSceneServerPlayerData *playerData;
//	//	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
//	//	ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
//	//	int bufsize = packetStream->getBufSize();
//	//	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
//	//		itr != playerMap.end();
//	//		++itr )
//	//	{
//	//		playerData = dynamic_cast<crSceneServerPlayerData *>(itr->second.get());
//	//		if(playerData->findInRangeItem(item->getInstanceItemID()))
//	//		{
//	//			packetStream->seekBegin();
//	//			packetStream->_writeInt(playerData->getPlayerID());
//	//			packetStream->setBufSize(bufsize);
//	//			netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//	//		}
//	//	}
//	//	netDataManager->unLockPlayerDataMap();
//	//}
//	sendPacketToItemNeighborByID(item->getInstanceItemID(),item->getSceneID(),item->getRoomID(),packet);
//}
//void crSceneServerCallback::sendPacketToItemNeighborByID(int itemid, int sceneid, int roomid, CRNet::crStreamPacket &packet)
//{
//	//crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//	//crNetManager *netManager = m_netConductor->getNetManager();
//	//netDataManager->lockPlayerDataMap();
//	//crSceneServerPlayerData *playerData;
//	//crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
//	//ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
//	//int bufsize = packetStream->getBufSize();
//	//for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
//	//	itr != playerMap.end();
//	//	++itr )
//	//{
//	//	playerData = dynamic_cast<crSceneServerPlayerData *>(itr->second.get());
//	//	if(playerData->isClientRunning() && playerData->getSceneID() == sceneid && playerData->getRoomID() == roomid
//	//		&& playerData->getSightInfo() && playerData->getSightInfo()->isItemInSightByID(itemid))
//	//	{
//	//		packetStream->seekBegin();
//	//		packetStream->_writeInt(playerData->getPlayerID());
//	//		packetStream->setBufSize(bufsize);
//	//		netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//	//	}
//	//}
//	//netDataManager->unLockPlayerDataMap();
//	std::set<crSightInfo *>SightSet;
//	std::set<crSightInfo *>::iterator sitr;
//	crSceneServerPlayerData *playerData;
//	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//	netDataManager->lockPlayerDataMap();
//	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
//	for( crNetDataManager::PlayerDataMap::iterator pitr = playerMap.begin();
//		pitr != playerMap.end();
//		++pitr )
//	{
//		playerData = dynamic_cast<crSceneServerPlayerData *>(pitr->second.get());
//		if(playerData->isClientRunning() && playerData->getSceneID() == sceneid
//			&& playerData->getSightInfo())
//		{
//			SightSet.insert(playerData->getSightInfo());
//		}
//	}
//	netDataManager->unLockPlayerDataMap();
//	for( sitr = SightSet.begin();
//		sitr != SightSet.end();
//		++sitr )
//	{
//		if((*sitr)->getRoomID() == roomid && (*sitr)->isItemInSightByID(itemid))
//		{
//			(*sitr)->sendPacketToEyePlayer(packet);
//		}
//	}
//}
int crSceneServerCallback::getMaxRoomCount()
{
    return m_maxRoomCount;
}
int crSceneServerCallback::getRoomCount()
{
	GNE::LockMutex lock( m_roomMapMutex );
	return m_roomMap.size();
}
//int crSceneServerCallback::s_temporaryRoomID = -1;
//crSceneServerCallback::RecycleIDDeque crSceneServerCallback::s_recycleRoomIDDeque;
//GNE::Mutex crSceneServerCallback::s_recycleRoomIDDeque;
//int crSceneServerCallback::gainTemporaryRoomID()
//{
//	int id;
//	GNE::LockMutex lock( s_recycleRoomIDDequeMutex );
//	if(!s_recycleRoomIDDeque.empty())
//	{
//		RecycleIDDeque::iterator itr = s_recycleRoomIDDeque.begin();
//		id = *itr;
//		s_recycleRoomIDDeque.pop_front();
//		return id;
//	}
//	id = s_temporaryRoomID;
//	s_temporaryRoomID--;
//	return id;
//}
//void crSceneServerCallback::recycleRoomID(int id)
//{
//	if(id<0)
//	{
//		GNE::LockMutex lock( s_recycleRoomIDDequeMutex );
//		for( RecycleIDDeque::iterator itr = s_recycleRoomIDDeque.begin();
//			itr != s_recycleRoomIDDeque.end();
//			++itr )
//		{
//			if(*itr == id)
//			{
//				return;
//			}
//		}
//		s_recycleRoomIDDeque.push_back(id);
//	}
//}
crRoom* crSceneServerCallback::createRoom(crSceneServerPlayerData *creator,crStreamBuf* streamBuf)
{
	//crRole *role = creator->getRole();
	//if(!role) return NULL;
	GNE::LockMutex lock( m_roomMapMutex );
	if(m_roomMap.size()>=m_maxRoomCount)
		return NULL;
	crScene *scene = findScene(creator->getSceneID());
	if(!scene)
		return NULL;
	ref_ptr<crRoom> room = new crRoom(scene);
	std::string roomdata = crGlobalHandle::getInstance()->getScenarioDir()+scene->getSceneName()+".cfg";
	ref_ptr<crData> data = CREncapsulation::loadData(roomdata);
	room->setDataClass(data.get());

	std::string creatorName = streamBuf->_readString();
	room->setCreateRoomData(streamBuf);
	//if(gameMode>=0&&gameMode<crRoom::GameModeCount)
	//{
	//	room->setGameMode(gameMode);
	//}
	//else
	//{
	//	room->setGameMode(0);
	//}
	//room->setName(roomname);
	//room->setPassword(password);
	//int id = 1;
	//if(!m_roomMap.empty())
	//{
	//	RoomMap::iterator itr = m_roomMap.begin();
	//	id = itr->first;
	//	id++;
	//}
	room->setRoomID(crGlobalHandle::gainTemporaryRoomID());
	//if(maxPlayerCount <= 0 || maxPlayerCount>m_maxRoomPlayerCount)
	//	maxPlayerCount = m_maxRoomPlayerCount;
	//room->setMaxPlayerCount(maxPlayerCount);
	ref_ptr<crRoomPlayer>roomPlayer = new crRoomPlayer;
	roomPlayer->setPlayerID(creator->getPlayerID());
	roomPlayer->setCharacterName(creatorName);
	roomPlayer->setPlayerGameData(creator->getPlayerGameData());
	//roomPlayer->setGroupID(room->assignGroupID());
	room->addMember(roomPlayer.get()/*,false*/);
	room->setRoomHost(creator->getPlayerID());
	room->setMinPlayerCount(1);
	//crServerBrainHandle::getInstance()->doEvent(WCH_GetRoomMinPlayer,);
    m_roomMap[room->getRoomID()] = room.get();
	room->doEvent(WCH_RoomCreate,MAKEINT64(creator,NULL));
	//CRCore::notify(CRCore::ALWAYS)<<"CreateRoom "<<room->getRoomID()<<std::endl;
	return room.get();
}
crRoom* crSceneServerCallback::createRoom(int sceneid,crStreamBuf* streamBuf)
{
	GNE::LockMutex lock( m_roomMapMutex );
	if(m_roomMap.size()>=m_maxRoomCount)
		return NULL;
	crScene *scene = findScene(sceneid);
	if(!scene)
		return NULL;
	ref_ptr<crRoom> room = new crRoom(scene);
	std::string roomdata = crGlobalHandle::getInstance()->getScenarioDir()+scene->getSceneName()+".cfg";
	ref_ptr<crData> data = CREncapsulation::loadData(roomdata);
	room->setDataClass(data.get());
	room->setCreateRoomData(streamBuf);
	room->setRoomID(crGlobalHandle::gainTemporaryRoomID());
	m_roomMap[room->getRoomID()] = room.get();
	room->doEvent(WCH_RoomCreate);
	//CRCore::notify(CRCore::ALWAYS)<<"CreateRoom "<<room->getRoomID()<<std::endl;
	return room.get();
}
//void crSceneServerCallback::removeRoom(int roomid)
//{
//	GNE::LockMutex lock( m_roomMapMutex );
//	m_roomMap.erase(roomid);
//	recycleRoomID(roomid);
//	CRCore::notify(CRCore::ALWAYS)<<"RemoveRoom "<<roomid<<std::endl;
//}
void crSceneServerCallback::lockRoomMap()
{
	m_roomMapMutex.acquire();
}
RoomMap &crSceneServerCallback::getRoomMap()
{
	return m_roomMap;
}
void crSceneServerCallback::unlockRoomMap()
{
	m_roomMapMutex.release();
}
crRoom *crSceneServerCallback::findRoom(int roomid)
{
	GNE::LockMutex lock( m_roomMapMutex );
	RoomMap::iterator itr = m_roomMap.find(roomid);
	return itr!=m_roomMap.end()?itr->second.get():NULL;
}
void crSceneServerCallback::roomUpdate(float dt)
{
	GNE::LockMutex lock( m_roomMapMutex );
	crRoom *room;
	for( RoomMap::iterator itr = m_roomMap.begin();
		 itr != m_roomMap.end(); )
	{
		room = itr->second.get();
		if(room->getCanRemove())
		{
			insertRemovedRoom(room);
			itr = m_roomMap.erase(itr);
		}
		else
		{
			room->serverUpdate(dt,this);
			++itr;
		}
	}
}
void crSceneServerCallback::closeAllRoom()
{
	GNE::LockMutex lock( m_roomMapMutex );
	crRoom *room;
	for( RoomMap::iterator itr = m_roomMap.begin();
		 itr != m_roomMap.end(); 
		 ++itr )
	{
		room = itr->second.get();
		room->timeRemain() = 0.0f;
	}
}
void crSceneServerCallback::shutDown()
{
	closeAllRoom();
	m_shutdownTime = 5.0f;
}
void crSceneServerCallback::insertRemovedRoom(crRoom *room)
{
	GNE::LockMutex lock( m_removedRoomMapMutex );
	m_removedRoomMap[room] = 5.0f;
}
void crSceneServerCallback::updateRemovedRoomMap(float dt)
{
	GNE::LockMutex lock( m_removedRoomMapMutex );
	ref_ptr<crRoom> room;
	int roomid;
	for( RemovedRoomMap::iterator itr = m_removedRoomMap.begin();
		itr != m_removedRoomMap.end();)
	{
		itr->second -= dt;
		if(itr->second<0.0f)
		{
			room = itr->first;
			endRoomGame(room.get());
			room->removeRoom();
			roomid = room->getRoomID();
			crGlobalHandle::recycleRoomID(roomid);
			//CRCore::notify(CRCore::ALWAYS)<<"RemoveRoom "<<roomid<<std::endl;
			itr = m_removedRoomMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
void crSceneServerCallback::startRoomGame(crRoom *room)
{
	int roomid = room->getRoomID();
	if(roomid == 0) return;
	crScene *scene = room->getScene();
	if(!scene) return;
	scene->lockSceneItemMap();
    crScene::SceneItemMap &sceneItemMap = scene->getSceneItemMap();
	ref_ptr<crInstanceItem> roomitem;
	crData *itemData;
	void *param;
	std::vector< ref_ptr<crInstanceItem> > InstanceItemVec;
	unsigned char itemstate = IS_Dead;
	unsigned int guiseState;
	for( crScene::SceneItemMap::iterator itr = sceneItemMap.begin();
		itr != sceneItemMap.end();
		++itr )
	{
		if(HIINT64(itr->first) == 0)
		{
			roomitem = dynamic_cast<crInstanceItem *>(itr->second->clone());
			itemData = roomitem->getDataClass();
//			itemData->excHandle(MAKEINT64(WCH_InitData,NULL));
			itemData->inputParam(WCHDATA_ItemState, &itemstate);
			itemData->getParam(WCHDATA_GuiseState,param);
			guiseState = *(unsigned int *)param;
			if(guiseState & GS_Static || guiseState & GS_StaticNoneBlock)
				guiseState = GS_StaticUnVisiable;
			else
				guiseState = GS_UnVisiable;
			itemData->inputParam(WCHDATA_GuiseState, &guiseState);
			roomitem->setRoomID(roomid);
			InstanceItemVec.push_back(roomitem.get());
		}
	}
	scene->unlockSceneItemMap();
	for( std::vector< ref_ptr<crInstanceItem> >::iterator itr = InstanceItemVec.begin();
		 itr != InstanceItemVec.end();
		 ++itr )
	{
		scene->insertRoomItem(itr->get());
		//给item分配groupid和检查视野共享
	}
}
void crSceneServerCallback::endRoomGame(crRoom *room)
{
	int roomid = room->getRoomID();
	if(roomid == 0) return;
	crScene *scene = room->getScene();
	if(!scene) return;
	scene->lockRoomItemMap();
	crScene::RoomItemMap &roomItemMap = scene->getRoomItemMap();
	std::vector< ref_ptr<crInstanceItem> > InstanceItemVec;
	crScene::SceneItemMap &sceneItemMap = roomItemMap[roomid];
	for( crScene::SceneItemMap::iterator itr = sceneItemMap.begin();
		itr != sceneItemMap.end();
		++itr )
	{
		InstanceItemVec.push_back(itr->second.get());
	}
	scene->unlockRoomItemMap();
	int id = 0;
	for( std::vector< ref_ptr<crInstanceItem> >::iterator itr = InstanceItemVec.begin();
		itr != InstanceItemVec.end();
		++itr )
	{
		id = (*itr)->getInstanceItemID();
		if(id<0) crGlobalHandle::recycleItemID(id);
		scene->removeRoomItem(itr->get());
	}
	//removeRoom(room->getRoomID());
}
crInstanceItem *crSceneServerCallback::dynamicCollideTest(crInstanceItem *item,const CRCore::crVector2 &newPos)//返回碰撞的item
{
	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	netDataManager->lockPlayerDataMap();
	crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
	crSceneServerPlayerData *playerData;
	crRole *role;
	int sceneid = item->getSceneID();
	int roomid = item->getRoomID();
	int layerid = item->getLayerID();
	for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr )
	{
		playerData = dynamic_cast<crSceneServerPlayerData *>(itr->second.get());
		if( playerData->isClientRunning() &&
			playerData->getSceneID() == sceneid &&
			playerData->getRoomID() == roomid )
		{
			//playerData->lockRoleMap();
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			for( crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
				 ritr != roleMap.end();
				 ++ritr )
			{
				role = ritr->second.get();
				if(role != item && role->getLayerID() == layerid)
				{
					if(role->dynamicCollideTest(item,newPos))
					{
						netDataManager->unLockPlayerDataMap();
						return role;
					}
				}
			}
			//playerData->unlockRoleMap();
		}
	}
	netDataManager->unLockPlayerDataMap();
	return NULL;
}
///////////////////////////////////
//
//crDBServerCallback
//
/////////////////////////////////////
crDBServerCallback::crDBServerCallback():
	m_pingInterval(10.0f),
	m_pingTime(0.0f),
	m_shutdownTime(0.0f),
	//m_init(false),
	m_columncount(4),
	m_cacheCount(100)/*,
	m_filename("gamelog.tab")*/
{
}

crDBServerCallback::crDBServerCallback(const crDBServerCallback& callback):
	crNetCallback(callback),
	m_shutdownTime(0.0f),
	//m_init(false),
	m_columncount(4),
	m_cacheCount(100)/*,
	m_filename("gamelog.tab")*/
{
}

void crDBServerCallback::serverInitImplementation()
{
	rcfg::ConfigScript cfg_script;
	const std::string& file = m_netConductor->getCfgFile();
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"crDBServerCallback::serverInitImplementation(): file open error,"<<file<<std::endl;
		return;
	}

	if(cfg_script.Push(m_netConductor->getKeyStr()))
	{
		cfg_script.Get("PingInterval", m_pingInterval);
		cfg_script.Get("LogCacheCount", m_cacheCount);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crChatServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crDBServerCallback::clientInitImplementation()
{
}

void crDBServerCallback::serverUpdateImplementation(float dt)
{//当客户机连接超时，将其踢出
	if(m_netConductor->getNetManager()->isRunning())
	{
		//crFileStreamCacheManager::getInstance()->update(dt);
		if(m_pingInterval>0.0f)
		{
			m_pingTime += dt;
			if(m_pingTime>m_pingInterval)
			{
				m_pingTime = 0.0f;
				m_netConductor->getNetManager()->heartJump();
			}
		}
	}
	if(m_shutdownTime>0.0f)
	{
		m_shutdownTime -= dt;
		if(m_shutdownTime<0.0f)
		{
			crNetContainer::getInstance()->shutdown();
		}
	}
}

void crDBServerCallback::clientUpdateImplementation(float dt)
{
	if(!m_netConductor->getNetManager()->isRunning())
	{//
		if(!m_needReconnect)
		{
			//crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			//if(connectServer)
			//{//GameServer异常，断开与玩家的连接
			//	connectServer->getNetManager()->disconnectAll();
			//}
			crNetCallbackManager::getInstance()->insertNeedReconnectCallback(this);
			m_needReconnect = true;
		}
	}
	else
	{
		if(m_needReconnect)
		{
			m_needReconnect = false;
		}
		//if(m_pingInterval>0)
		//{
		//	m_pingTime += dt;
		//	if(m_pingTime>m_pingInterval)
		//	{
		//		m_pingTime = 0;
		//		m_netConductor->getNetManager()->heartJump();
		//	}
		//}
	}
}

void crDBServerCallback::serverShutdown()
{
	GNE::LockMutex lock( m_gamelogPathMapMutex );
	for( GamelogPathMap::iterator itr = m_gamelogPathMap.begin(); 
		 itr != m_gamelogPathMap.end();
		 ++itr )
	{
		saveGameLog(itr->first,0,0,"DBServer关闭");
	}
}

void crDBServerCallback::clientShutdown()
{
}

bool crDBServerCallback::clientReconnectToServer()
{
	crNetManager *netManager = m_netConductor->getNetManager();
	int code = netManager->clientReconnectToServerCircle(1);
	if(code>0)
	{//重新连接成功
		CRCore::notify(CRCore::ALWAYS)<<"crDBServerCallback::clientReconnectToServer 重新连接成功"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crGameServerCallback::clientUpdateImplementation 重新连接成功\n\0");
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
		if(m_netConductor->getNetType() == GameServerClient_DB)
		{
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			if(gameServer)
			{//需要重新注册
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
				std::string servername = callback->getServerName();
				crServerBrainHandle::getInstance()->doEvent(WCH_SetGamelogPath,MAKEINT64(&servername,NULL));
			}
		}
		return true;
	}
	return false;
}
void crDBServerCallback::serverRemoveConnectionImplementation(const std::string& address)
{
	saveGameLog(address,0,0,"服务器断开连接");
	eraseGamelogPath(address);
}
void crDBServerCallback::saveGameLog(const std::string& address,int playerid,unsigned short logtype,const std::string &logdata)
{
	//GNE::LockMutex lock( m_saveMutex );
	std::string filename;
	CRCore::ref_ptr<CREncapsulation::crTableIO> outtabio;
	getGamelogPath(address,filename,outtabio);
	if(!CRIOManager::fileExists(filename))
	{
		//m_filename = "gamelog.tab";
		makeDirectoryForFile(filename);
		CREncapsulation::crTableIO::StrVec titleVec;
		titleVec.push_back("时间");
		titleVec.push_back("玩家ID");
		titleVec.push_back("logtype");
		titleVec.push_back("logdata");
		m_columncount = titleVec.size();
		std::ofstream fout(filename.c_str(), std::ios::out);
		char tab = 9;
		char n = '\n';
		int i;
		for(i = 0; i<m_columncount-1; ++i)
		{
			fout.write(titleVec[i].c_str(),titleVec[i].length());
			fout.write(&tab,1);
		}
		fout.write(titleVec[i].c_str(),titleVec[i].length());
		fout.write(&n,1);
		fout.close();
		outtabio->getDataVec().reserve(m_cacheCount);
		//m_init = true;
	}
	time_t t = time( 0 ); 
	char tmp[20];
	strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
	CREncapsulation::crTableIO::StrVec record;
	record.resize(m_columncount);
	record[0] = tmp;
	record[1] = crArgumentParser::appItoa(playerid);
	record[2] = crArgumentParser::appItoa(logtype);
	record[3] = logdata;
	outtabio->getDataVec().push_back(record);
	int logsize = outtabio->getDataVec().size();
	if(logsize>=m_cacheCount || logtype == 0)
	{
		std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
		if(fout)
		{
			char tab = 9;
			char n = '\n';
			int i,j;
			for(i = 0; i<logsize;++i)
			{
				for(j=0; j<m_columncount-1; ++j)
				{
					fout.write(outtabio->getDataVec()[i][j].c_str(),outtabio->getDataVec()[i][j].length());
					fout.write(&tab,1); 
				}
				fout.write(outtabio->getDataVec()[i][j].c_str(),outtabio->getDataVec()[i][j].length());
				fout.write(&n,1);
			}
			fout.close();
		}
		outtabio->getDataVec().resize(0);
		///当大小超过10M是重命名文件
		struct stat f_stat;
		if(!stat( filename.c_str(), &f_stat ) && f_stat.st_size>10485760)//10M
		{
			std::string path = crArgumentParser::getFilePath(filename);
			std::string name = crArgumentParser::getSimpleFileName(filename);
			char programDir[256];
			GetCurrentDirectory(256,programDir);
			std::string dir = programDir;
			dir = dir + "/" + path;
			SetCurrentDirectory(dir.c_str());

			time_t t = time( 0 ); 
			char tmp[20];
			strftime( tmp, sizeof(tmp), "%Y%m%d\0", localtime(&t) );
			char buf[256];
			sprintf(buf,"ren %s %s%s.%s\0",name.c_str(),crArgumentParser::getFileNameEliminateExt(name).c_str(),tmp,crArgumentParser::getFileExtension(name).c_str());
			system(buf);

			SetCurrentDirectory(programDir);
		}
	}
}
void crDBServerCallback::saveDBLog(char *logdata)
{
	std::string filename = "DBSavePack.log";
	std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
	if(fout)
	{
		time_t t = time( 0 ); 
		char tmp[20];
		strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
		fout<<tmp<<logdata<<std::endl;
		fout.close();
	}
}
void crDBServerCallback::insertGamelogPath(const std::string &address,const std::string &logpath)
{
	GNE::LockMutex lock( m_gamelogPathMapMutex );
	m_gamelogPathMap[address] = std::make_pair(logpath+"/gamelog.tab",new CREncapsulation::crTableIO);
}
void crDBServerCallback::eraseGamelogPath(const std::string &address)
{
	GNE::LockMutex lock( m_gamelogPathMapMutex );
	m_gamelogPathMap.erase(address);
}
void crDBServerCallback::getGamelogPath(const std::string &address,std::string &outpath,CRCore::ref_ptr<CREncapsulation::crTableIO>&outtabio )
{
	GNE::LockMutex lock( m_gamelogPathMapMutex );
	GamelogPathMap::iterator itr = m_gamelogPathMap.find(address);
	if(itr != m_gamelogPathMap.end())
	{
		outpath = itr->second.first;
		outtabio = itr->second.second;
	}
	else
	{
		outpath = "gamelog.tab";
		outtabio = new CREncapsulation::crTableIO;
	}
}
void crDBServerCallback::shutDown()
{
	m_shutdownTime = 10.0f;
}
/////////////////////////////////////
////
////crSceneConnectServerCallback
////
///////////////////////////////////////
//crSceneConnectServerCallback::crSceneConnectServerCallback():
//m_time(0L),
//m_pingInterval(10),
//m_pingTime(0)
//{
//}
//
//crSceneConnectServerCallback::crSceneConnectServerCallback(const crSceneConnectServerCallback& callback):
//crNetCallback(callback)
//{
//}
//
//void crSceneConnectServerCallback::serverInitImplementation()
//{
//	rcfg::ConfigScript cfg_script;
//	struct stat buf;
//	const std::string& file = m_netConductor->getCfgFile();
//	CRIOManager::crLoadCookFile scopedLoad(file);
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg read error "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	if(cfg_script.Push(m_netConductor->getKeyStr()))
//	{
//		cfg_script.Get("PingInterval", m_pingInterval);
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"crGameConnectServerCallback::init,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//	}
//}
//
//void crSceneConnectServerCallback::clientInitImplementation()
//{
//}
//
//void crSceneConnectServerCallback::serverUpdateImplementation()
//{//当客户机连接超时，将其踢出
//	CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
//	if(m_time == 0)
//	{
//		m_time = t1;
//		return;
//	}
//
//	float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
//	m_time = t1;
//	if(m_netConductor->getNetManager()->isRunning())
//	{
//		if(m_pingInterval>0)
//		{
//			m_pingTime += dt;
//			if(m_pingTime>m_pingInterval)
//			{
//				m_pingTime = 0;
//				m_netConductor->getNetManager()->heartJump();
//			}
//		}
//	}
//}
//
//void crSceneConnectServerCallback::clientUpdateImplementation()
//{
//	if(!m_netConductor->getNetManager()->isRunning())
//	{//客户端断线不进行重连接
//		//crGlobalHandle::getInstance()->doEvent(WCH_SceneConnectServerAbnormal,MAKEINT64(NULL,NULL));
//		CRCore::notify(CRCore::ALWAYS)<<"crSceneConnectServerCallback::clientUpdateImplementation Net已经断开"<<std::endl;
//		CRCore::crThread::sleep(10);
//	}
//	else
//	{
//		CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
//		if(m_time == 0)
//		{
//			m_time = t1;
//			return;
//		}
//
//		float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
//		m_time = t1;
//		crMyPlayerData::getInstance()->update(dt);
//	}
//}
//
//void crSceneConnectServerCallback::serverShutdown()
//{
//
//}
//
//void crSceneConnectServerCallback::clientShutdown()
//{
//}