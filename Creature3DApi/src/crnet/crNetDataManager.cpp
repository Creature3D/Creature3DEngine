/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRNet/crNetDataManager.h>
#include <CRNet/crNetManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/crRange.h>
using namespace CRNet;
using namespace CRCore;
crNetDataManager::crNetDataManager():
m_netManager(NULL)
{
	//m_sqr_maxDistance = 2500000000;
	//m_maxConnectCount = 1000;
	m_serverName = "Creature3DServer";
	m_maxPlayerCount = 5000;
}

crNetDataManager::~crNetDataManager()
{
	//CRCore::notify(CRCore::NOTICE)<<"~crNetDataManager"<<std::endl;
	m_netManager = NULL;
}

//void crNetDataManager::setMaxDistance(float maxDistance)
//{
//    m_sqr_maxDistance = maxDistance * maxDistance;
//}

void crNetDataManager::serverUpdate(float dt)
{//发送需要同步的电脑控制的AI数据
	m_streamMutex.acquire();
	m_receivedStreamList_buf.swap(m_receivedStreamList);
	m_streamMutex.release();
	if(!m_receivedStreamList_buf.empty())
	{
		//const CRCore::Timer& timer = *CRCore::Timer::instance();
		//CRCore::Timer_t start_tick = timer.tick();
		//double elapsedTime = 0.0;
		//double availableTime = 0.05f;//50ms
		//ReceivedStreamDeque::iterator itr = m_receivedStreamDeque.begin();
		//for( ;
		//	 elapsedTime<availableTime && itr != m_receivedStreamDeque.end();
		//	 ++itr )
		for( ReceivedStreamList::iterator itr = m_receivedStreamList_buf.begin();
			itr != m_receivedStreamList_buf.end();
			++itr )
		{
			try
			{
				itr->first->parsePacket(itr->second);
				//elapsedTime = timer.delta_s(start_tick,timer.tick());
			}
			catch (...)
			{
				//CRCore::notify(CRCore::FATAL)<<"crNetDataManager::serverUpdate() parse a packet error PacketID = "<<itr->first->getType()<<std::endl;
				CRCore::notify(CRCore::FATAL)<<"error Packet Sender = "<<itr->second<<std::endl;
				//sprintf(gDebugInfo->buf(),"crNetDataManager::serverUpdate() parse a packet error PacketID = %d error Packet Sender = %s\n\0",itr->first->getType(),itr->second.c_str());
				//gDebugInfo->debugInfo(CRCore::FATAL);
				//throw 1;
			}
			delete itr->first;
		}
		m_receivedStreamList_buf.clear();
		//m_receivedStreamDeque.erase(m_receivedStreamDeque.begin(),itr);
	}
	if(m_netCallback.valid())
		m_netCallback->serverUpdateImplementation(dt);
}

void crNetDataManager::clientUpdate(float dt)
{
	m_streamMutex.acquire();
	m_receivedStreamList_buf.swap(m_receivedStreamList);
	m_streamMutex.release();
	if(!m_receivedStreamList_buf.empty())
	{
		//const CRCore::Timer& timer = *CRCore::Timer::instance();
		//CRCore::Timer_t start_tick = timer.tick();
		//double elapsedTime = 0.0;
		//double availableTime = 0.05f;//50ms

		//ReceivedStreamDeque::iterator itr = m_receivedStreamDeque_buf.begin();
		//for( ;
		//	elapsedTime<availableTime && itr != m_receivedStreamDeque.end();
		//	++itr )
		for( ReceivedStreamList::iterator itr = m_receivedStreamList_buf.begin();
			 itr != m_receivedStreamList_buf.end();
			 ++itr )
		{
			try
			{
				itr->first->parsePacket(itr->second);
				//elapsedTime = timer.delta_s(start_tick,timer.tick());
			}
			catch (...)
			{
				CRCore::notify(CRCore::FATAL)<<"crNetDataManager::clientUpdate() parse a packet error PacketID = "<<itr->first->getType()<<std::endl;
				CRCore::notify(CRCore::FATAL)<<"error Packet Sender = "<<itr->second<<std::endl;
				//throw 1;
				//sprintf(gDebugInfo->buf(),"crNetDataManager::clientUpdate() parse a packet error PacketID = %d error Packet Sender = %s\n\0",itr->first->getType(),itr->second.c_str());
				//gDebugInfo->debugInfo(CRCore::FATAL);
			}
			delete itr->first;
		}
		m_receivedStreamList_buf.clear();
		//m_receivedStreamDeque.erase(m_receivedStreamDeque.begin(),itr);
	}
	if(m_netCallback.valid())
		m_netCallback->clientUpdateImplementation(dt);
}

void crNetDataManager::receiveStream(crStreamPacket *packet,const std::string &sender)
{
	m_streamMutex.acquire();
	m_receivedStreamList.push_back(StreamPacketSender(packet,sender));
	m_streamMutex.release();
}

void crNetDataManager::serverShutdown()
{
	if(m_netCallback.valid())
		m_netCallback->serverShutdown();
}

void crNetDataManager::clientShutdown()
{
	if(m_netCallback.valid())
		m_netCallback->clientShutdown();
}

//void crNetDataManager::broadcastExceptSenderAndCulled(const std::string &sender, GNE::Packet &packet,const crClientData &data, unsigned int cullmask)
//{
//    if(!m_netManager) return;
//	for( ClientDataMap::iterator itr = m_clientDataMap.begin();
//		 itr != m_clientDataMap.end();
//		 ++itr )
//	{
//		if(sender.compare(itr->first)!=0 && !itr->second->culled(&data,cullmask,getSqrMaxDistance()))
//		{
//	        m_netManager->sendPacket(itr->first,packet);
//		}
//	}
//}

//void crNetDataManager::broadcastExceptCulled(GNE::Packet &packet,const crClientData &data,unsigned int cullmask)
//{
//	if(!m_netManager) return;
//	for( ClientDataMap::iterator itr = m_clientDataMap.begin();
//		itr != m_clientDataMap.end();
//		++itr )
//	{
//		if(!itr->second->culled(&data,cullmask,getSqrMaxDistance()))
//		{
//			m_netManager->sendPacket(itr->first,packet);
//		}
//	}
//}

void crNetDataManager::setServerName(const std::string &name)
{
    m_serverName = name;
}

std::string &crNetDataManager::getServerName()
{
    return m_serverName;
}

void crNetDataManager::setServerPassword(const std::string &password)
{
    m_serverPassword = password;
}

std::string &crNetDataManager::getServerPassword()
{
	return m_serverPassword;
}

void crNetDataManager::setNetCallbackString(const std::string &callbackStr)
{
	crNetCallback *callback = crNetCallbackManager::getInstance()->getNetCallback(callbackStr);
	if(!callback) 
	{
		CRCore::notify(CRCore::NOTICE)<<"crNetDataManager::setNetCallbackString(): 未找到callback: "<<callback<<std::endl;
		//sprintf(gDebugInfo->buf(),"crNetDataManager::setNetCallbackString(): 未找到callback:%s\n\0",callbackStr.c_str());
		//gDebugInfo->debugInfo(CRCore::NOTICE);
		return;
	}
	setNetCallback(callback);
}

//void crNetDataManager::setMaxConnectCount(int maxConnectCount)
//{
//    m_maxConnectCount = maxConnectCount;
//}
//
//int crNetDataManager::getMaxConnectCount()
//{
//	return m_maxConnectCount;
//}

//bool crNetDataManager::checkConnectCount()
//{
//    return m_nameClientMap.size()<m_maxConnectCount;
//}

//void crNetDataManager::setClientStamp(float stamp)
//{
//    m_clientStamp = stamp;
//}

//void crNetDataManager::clientUpdateAndReconnect(float interval, float delay)
//{
//    m_clientStamp += interval;
//	if(m_clientStamp>delay)
//	{//已经与服务器断开，需要进入重联循环。
//        m_netManager->clientReconnectToServerCircle();
//	}
//}

//void crNetDataManager::serverUpdateAndDrop(float interval, float delay)
//{
//    //CRCore::notify(CRCore::FATAL)<<"crNetDataManager::serverUpdateAndDrop "<<m_serverSyncDataMap.size()<<std::endl;
//	//CRCore::notify(CRCore::FATAL)<<"crNetDataManager::"<<std::endl;
//	if(!m_netManager||m_clientDataMap.empty()) return;
//
//	std::vector<std::string> addressArray;
//	for( ClientDataMap::iterator itr = m_clientDataMap.begin();
//		itr != m_clientDataMap.end();
//		++itr )
//	{
//		itr->second->m_stamp+=interval;
//		if(itr->second->m_stamp>delay)
//		{
//			addressArray.push_back(itr->first);
//		}
//	}
//
//	for( std::vector<std::string>::iterator itr = addressArray.begin();
//		 itr != addressArray.end();
//		 ++itr )
//	{
//		m_netManager->removeConnection(*itr);
//	}
//}

//void crNetDataManager::removeConnection(const std::string &name/*,const std::string &address*/)
//{
//	//removeNameClient(name);
//	//removeClientData(address);
//	removePlayerData(name);
//}

//void crNetDataManager::removePlayerData(const std::string &name)
//{
//	GNE::LockMutex lock( m_playerMapMutex );
//	for( PlayerDataMap::iterator itr = m_playerDataMap.begin();
//		 itr != m_playerDataMap.end();
//		 ++itr )
//	{
//		if(itr->second->getPlayerName().compare(name) == 0)
//		{
//			m_playerDataMap.erase(itr);
//			break;
//		}
//	}
//}

crPlayerData *crNetDataManager::getPlayerDataByAddress(const std::string &address)
{
	GNE::LockMutex lock( m_playerMapMutex );
	crServerPlayerData *serverPlayerData;
	for( PlayerDataMap::iterator itr = m_playerDataMap.begin();
		itr != m_playerDataMap.end();
		++itr )
	{
		serverPlayerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		if(serverPlayerData)
		{
			if(serverPlayerData->getPlayerAdress().compare(address) == 0)
			{
				return itr->second.get();
			}
		}
	}
	return NULL;
}

//crPlayerData *crNetDataManager::getPlayerDataByName(const std::string &name)
//{
//	GNE::LockMutex lock( m_playerMapMutex );
//	for( PlayerDataMap::iterator itr = m_playerDataMap.begin();
//		itr != m_playerDataMap.end();
//		++itr )
//	{
//		if(itr->second->getPlayerName().compare(name) == 0)
//		{
//			return itr->second.get();
//		}
//	}
//	return NULL;
//}

/*
void crNetDataManager::regServerClient(const std::string& name, const std::string& address)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_serverClientMapMutex);
	m_serverClientMap.insert(std::make_pair(name,address));
}

bool crNetDataManager::findServerClientAddress(const std::string& name, std::string& address, int line)
{//line = -1表示随机选取
	CRCore::ScopedLock<CRCore::crMutex> lock(m_serverClientMapMutex);
	ServerClientMap::iterator itr = m_serverClientMap.find(name);
	if(itr==m_serverClientMap.end()) return false;

	if(line == 0)
	{
        address = itr->second;
		return true;
	}
	else if(line>0)
	{
		ServerClientMap::iterator nextItr = itr;
		do 
		{
			itr = nextItr;
			nextItr++;
			line--;
		} while (line>=0 && nextItr != m_serverClientMap.end() && name.compare(nextItr->first) == 0);

		address = itr->second;
		return true;
	}
	else if(line == -1)
	{//随机选择
		int cout = 1;
		ServerClientMap::iterator pItr = itr;
		for(pItr++; pItr != m_serverClientMap.end() && name.compare(pItr->first) == 0; pItr++)
			cout++;
		int r = CRCore::rangei(0,cout).get_random();
		while(r>0)
		{
			itr++;
			r--;
		}
		address = itr->second;
		return true;
	}
	address = itr->second;
	return true;
}

bool crNetDataManager::findServerClientName(const std::string& address, std::string& name)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_serverClientMapMutex);

	for( ServerClientMap::iterator itr = m_serverClientMap.begin();
		itr != m_serverClientMap.end();
		itr++ )
	{
		if(address.compare(itr->second) == 0)
		{
			name = itr->first;
            return true;
		}
	}
	return false;
}

void crNetDataManager::unregServerClientByName(const std::string& name)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_serverClientMapMutex);

	for( ServerClientMap::iterator itr = m_serverClientMap.find(name);
         itr != m_serverClientMap.end() && name.compare(itr->first) == 0;
		 itr++ )
	{
		m_serverClientMap.erase(itr);
		break;
	}

	serverRemoveConnection(name);
}

void crNetDataManager::unregServerClientByAddress(const std::string& address)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_serverClientMapMutex);
	std::string name;
	bool found = false;
	for( ServerClientMap::iterator itr = m_serverClientMap.begin();
		itr != m_serverClientMap.end();
		itr++ )
	{
		if(address.compare(itr->second) == 0)
		{
			name = itr->first;
			m_serverClientMap.erase(itr);
			found = true;
			break;
		}
	}
	if(found)
	{
        ServerClientMap::iterator itr = m_serverClientMap.find(name);
		if(itr == m_serverClientMap.end())
		{
            serverRemoveConnection(name);
		}
	}
}
*/
void crNetDataManager::serverRemoveConnection(const std::string &address)
{
	if(m_netCallback.valid())
		m_netCallback->serverRemoveConnectionImplementation(address);
}
void crNetDataManager::sendPacketToAllPlayer(crStreamPacket &packet)
{
	GNE::LockMutex lock( m_playerMapMutex );
	crServerPlayerData *playerData;
	ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
	int bufsize = packetStream->getBufSize();
	for( PlayerDataMap::iterator itr = m_playerDataMap.begin();
		itr != m_playerDataMap.end();
		++itr )
	{
		playerData = dynamic_cast<crServerPlayerData *>(itr->second.get());
		packetStream->seekBegin();
		packetStream->_writeInt(playerData->getPlayerID());
		packetStream->setBufSize(bufsize);
		m_netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
	}
}