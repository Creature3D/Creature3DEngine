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
#include <CRNet/crNetContainer.h>
//#include <CRNet/crDisConnectPacket.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crRange.h>
#include <CRIOManager/crLoadManager.h>
#include <gnelib.h>
using namespace CRNet;
using namespace CRCore;
using namespace CRIOManager;
using namespace GNE;
//////////////////////////
//
//crNetConductor
//
///////////////////////////
crNetConductor::crNetConductor():m_port(DEFAULTPORT),m_enableCook(false)
{
}

crNetConductor::~crNetConductor()
{
	m_netManager = NULL;
	m_netDataManager = NULL;
	//CRCore::notify(CRCore::NOTICE)<<"~crNetConductor"<<std::endl;
}

void crNetConductor::init(int port)
{
    m_port = port;
	m_netManager = new crNetManager;
	crNetBridge *netBridge = new crNetBridge;
	netBridge->attachNetConductor(this);
	m_netManager->attachNetBridge(netBridge);
	m_netDataManager = new crNetDataManager;
	m_netDataManager->attachNetManager(m_netManager.get());
}

bool crNetConductor::createServer(const std::string &callback, const std::string &name, const std::string &password, int maxConnect, int bufScale, int timeoutms)
{
    if(!m_netManager.valid())
		return false;
	return m_netManager->createServer(callback,name,password,maxConnect,m_port,bufScale,timeoutms);
}

void crNetConductor::update(float dt)
{
    if(m_netManager.valid()) m_netManager->update(dt);
}
void crNetConductor::bridgeupdate()
{
	if(m_netManager.valid()) m_netManager->bridgeupdate();
}
void crNetConductor::removeConnection(int playerid)
{
}

void crNetConductor::setCfgFile(const std::string &cfgfile)
{
	m_cfgfile = cfgfile;
}
const std::string &crNetConductor::getCfgFile() const
{
	return m_cfgfile;
}
void crNetConductor::setKeyStr(const std::string &keystr)
{
	m_keystr = keystr;
}
const std::string &crNetConductor::getKeyStr() const
{
	return m_keystr;
}
void crNetConductor::setEnableCook(bool enableCook)
{
	m_enableCook = enableCook;
}
bool crNetConductor::getEnableCook()
{
	return m_enableCook;
}
//////////////////////////
//
//crConnectServerConductor
//
///////////////////////////
crConnectServerConductor::crConnectServerConductor():m_logoutPacket(NULL)
{
}
crConnectServerConductor::~crConnectServerConductor()
{
	delete m_logoutPacket;
}
void crConnectServerConductor::setLogoutPacket(crStreamPacket &packet)
{
    m_logoutPacket = dynamic_cast<crStreamPacket *>(packet.makeClone());
}
void crConnectServerConductor::insertConnectedServer(crNetConductor *netconductor)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
    m_connectedServerSet.insert(netconductor);
}

void crConnectServerConductor::removeConnectedServer(crNetConductor *netconductor)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
    m_connectedServerSet.erase(netconductor);
}

void crConnectServerConductor::removeConnection(int playerid)
{
	crPlayerData *playerdata = m_netDataManager->getPlayerData(playerid);
	if(playerdata)
		m_netDataManager->getNetCallback()->removeConnection(playerdata);
	else
	{
		playerLogout(playerid);
	}
}
void crConnectServerConductor::playerLogout(int playerid)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_mutex);
	if(m_logoutPacket)
	{
		crStreamBuf *streamBuf = new crStreamBuf;
		streamBuf->createBuf(4);//108
		m_logoutPacket->setStreamBuf(streamBuf);
		streamBuf->_writeInt(playerid);//name---8+4

		ref_ptr<crNetConductor> netConductor;
		for( ConnectedServerSet::iterator itr = m_connectedServerSet.begin();
			itr != m_connectedServerSet.end();
			++itr )
		{
			netConductor = *itr;
			if(netConductor.valid())
				netConductor->getNetManager()->sendPacket("all",*m_logoutPacket);//通知各个服务器playerid掉线，通知其他客户端，playerid掉线
		}
	}
}
//////////////////////////
//
//crNetContainer
//
///////////////////////////
crNetContainer::crNetContainer():
m_mainNetType(-1),
m_time(0L),
m_shutdown(false)
{
	setFpsControl(10);
}

crNetContainer::~crNetContainer()
{
	//CRCore::notify(CRCore::NOTICE)<<"~crNetContainer"<<std::endl;
}

ref_ptr<crNetContainer> crNetContainer::m_instance = NULL;

crNetContainer* crNetContainer::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crNetContainer;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crNetContainer::clear()
{
	//GNE::LockMutex lock( m_mutex );
	m_mutex.acquire();
	m_mutex.release();
	m_instance = NULL;
}
void crNetContainer::setMainNetType(int netType)
{
	m_mainNetType = netType;
}
int crNetContainer::getMainNetType()
{
	return m_mainNetType;
}
void crNetContainer::pushNetConductor(crNetConductor *netConductor)
{
	GNE::LockMutex lock( m_mutex );
	m_netConductorMap.insert(std::make_pair(netConductor->getNetType(),netConductor));
}

crNetConductor *crNetContainer::getNetConductor(int netType/*, int line*/)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_netConductorMap.find(netType);
	return itr==m_netConductorMap.end()?NULL:itr->second.get();
	//if(itr==m_netConductorMap.end()) return NULL;
	//if(line == 0)
	//{
	//	return itr->second.get();
	//}
	//else if(line>0)
	//{
	//	NetConductorMap::iterator nextItr = itr;
	//	do 
	//	{
	//		itr = nextItr;
	//		nextItr++;
	//		line--;
	//	} while (line>=0 && nextItr != m_netConductorMap.end() && nextItr->first == netType);

	//	return itr->second.get();
	//}
 //   else if(line == -1)
	//{//随机选择
 //       int cout = 0;
	//	NetConductorMap::iterator pItr = itr;
	//	for(pItr++; pItr != m_netConductorMap.end() && pItr->first == netType; pItr++)
	//		cout++;
	//	int r = CRCore::rangei(0,cout-1).get_random();
 //       while(r>0)
 //       {
	//		itr++;
	//		r--;
 //       }
	//	return itr->second.get();
	//}
	//else if(line == -2)
	//{//重第1个开始随机选择
	//	int cout = 0;
	//	NetConductorMap::iterator pItr = itr;
	//	for(pItr++; pItr != m_netConductorMap.end() && pItr->first == netType; pItr++)
	//		cout++;
	//	int r = CRCore::rangei(1,cout-1).get_random();
	//	while(r>0)
	//	{
	//		itr++;
	//		r--;
	//	}
	//	return itr->second.get();
	//}
	//return itr->second.get();
}

crNetConductor *crNetContainer::findNetConductor(int netType, const std::string &name)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_netConductorMap.find(netType);
	for(; itr != m_netConductorMap.end() && itr->first == netType; ++itr)
	{
        if(name.compare(itr->second->getName()) == 0)
			return itr->second.get();
	}
	return NULL;
}

void crNetContainer::getNetConductorVec(int netType, std::vector< crNetConductor *>& netConductorVec)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_netConductorMap.find(netType);
	for(; itr != m_netConductorMap.end() && itr->first == netType; ++itr)
	{
		netConductorVec.push_back(itr->second.get());
	}
}
//////Dynaimc
void crNetContainer::pushDynamicNetConductor(crNetConductor *netConductor)
{
	GNE::LockMutex lock( m_mutex );
	m_dynamicNetConductorMap.insert(std::make_pair(netConductor->getNetType(),netConductor));
}
void crNetContainer::pushRemovedConductor(crNetConductor *netConductor)
{
	GNE::LockMutex lock( m_mutex );
	netConductor->getNetManager()->shutdown();
	m_removedConductorDeque.push_back(std::make_pair(netConductor,10.0f));
}
void crNetContainer::removeDynamicNetConductor(int netType, int line)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_dynamicNetConductorMap.find(netType);
	if(itr==m_dynamicNetConductorMap.end()) return;

	if(line == 0)
	{
		pushRemovedConductor(itr->second.get());
		m_dynamicNetConductorMap.erase(itr);
	}
	else if(line>0)
	{
		NetConductorMap::iterator nextItr = itr;
		do 
		{
			itr = nextItr;
			nextItr++;
			line--;
		} while (line>=0 && nextItr != m_dynamicNetConductorMap.end() && nextItr->first == netType);

		pushRemovedConductor(itr->second.get());
		m_dynamicNetConductorMap.erase(itr);
	}
	else if(line == -1)
	{//移除所有
		std::vector< NetConductorMap::iterator > eraseList;
		eraseList.push_back(itr);
		for(itr++; itr != m_dynamicNetConductorMap.end() && itr->first == netType; itr++)
			eraseList.push_back(itr);
		for( std::vector< NetConductorMap::iterator >::iterator eitr = eraseList.begin();
			eitr != eraseList.end();
			++eitr )
		{
			pushRemovedConductor((*eitr)->second.get());
			m_dynamicNetConductorMap.erase(*eitr);
		}
	}
}

crNetConductor *crNetContainer::getDynamicNetConductor(int netType/*, int line*/)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_dynamicNetConductorMap.find(netType);
	return itr==m_dynamicNetConductorMap.end()?NULL:itr->second.get();

	//if(itr==m_dynamicNetConductorMap.end()) return NULL;
	//if(line == 0)
	//{
	//	return itr->second.get();
	//}
	//else if(line>0)
	//{
	//	NetConductorMap::iterator nextItr = itr;
	//	do 
	//	{
	//		itr = nextItr;
	//		nextItr++;
	//		line--;
	//	} while (line>=0 && nextItr != m_dynamicNetConductorMap.end() && nextItr->first == netType);

	//	return itr->second.get();
	//}
	//else if(line == -1)
	//{//随机选择
	//	int cout = 0;
	//	NetConductorMap::iterator pItr = itr;
	//	for(pItr++; pItr != m_dynamicNetConductorMap.end() && pItr->first == netType; pItr++)
	//		cout++;
	//	int r = CRCore::rangei(0,cout-1).get_random();
	//	while(r>0)
	//	{
	//		itr++;
	//		r--;
	//	}
	//	return itr->second.get();
	//}
	//else if(line == -2)
	//{//重第1个开始随机选择
	//	int cout = 0;
	//	NetConductorMap::iterator pItr = itr;
	//	for(pItr++; pItr != m_dynamicNetConductorMap.end() && pItr->first == netType; pItr++)
	//		cout++;
	//	int r = CRCore::rangei(1,cout-1).get_random();
	//	while(r>0)
	//	{
	//		itr++;
	//		r--;
	//	}
	//	return itr->second.get();
	//}
	//return itr->second.get();
}

crNetConductor *crNetContainer::findDynamicNetConductor(int netType, const std::string &name)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_dynamicNetConductorMap.find(netType);
	for(; itr != m_dynamicNetConductorMap.end() && itr->first == netType; ++itr)
	{
		if(name.compare(itr->second->getName()) == 0)
			return itr->second.get();
	}
	return NULL;
}

void crNetContainer::getDynamicNetConductorVec(int netType, std::vector< crNetConductor *>& netConductorVec)
{
	GNE::LockMutex lock( m_mutex );
	NetConductorMap::iterator itr = m_dynamicNetConductorMap.find(netType);
	for(; itr != m_dynamicNetConductorMap.end() && itr->first == netType; ++itr)
	{
		netConductorVec.push_back(itr->second.get());
	}
}
void crNetContainer::setFpsControl(float fps)
{
	if(fps<=0.0f)
		m_fpsControl = 0.0f;
	else
		m_fpsControl = CRCore::clampTo(1000.0f/fps,0.0f,100.0f);
}
void crNetContainer::update()
{//限定在10帧
	CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	if(m_time == 0)
	{
		m_time = t1;
		CRCore::crThread::sleep(m_fpsControl);
		return;
	}
	float dt = CRCore::Timer::instance()->delta_m( m_time, t1 );
	if(dt<m_fpsControl)
	{
		CRCore::crThread::sleep(m_fpsControl - dt);
	}
	t1 = CRCore::Timer::instance()->tick();
	dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
	m_time = t1;
	for( NetConductorMap::iterator itr = m_netConductorMap.begin(); 
		itr != m_netConductorMap.end(); 
		++itr)
	{
		itr->second->update(dt);
	}
	if(!m_dynamicNetConductorMap.empty() || !m_removedConductorDeque.empty())
	{//client
		GNE::LockMutex lock( m_mutex );
		for( NetConductorMap::iterator itr = m_dynamicNetConductorMap.begin(); 
			itr != m_dynamicNetConductorMap.end(); 
			++itr)
		{
			itr->second->update(dt);
		}

		for( RemovedConductorDeque::iterator itr = m_removedConductorDeque.begin();
			itr != m_removedConductorDeque.end();
			++itr )
		{
			itr->second -= dt;
		}
		if(!m_removedConductorDeque.empty() && m_removedConductorDeque.front().second<=0.0f)
		{
			m_removedConductorDeque.pop_front();
		}
	}
	crScriptLoadManager::getInstance()->updateRemovedReferencedMap(dt);
}
void crNetContainer::shutdown()
{
	m_shutdown = true;
}
bool crNetContainer::isShutDown()
{
	return m_shutdown;
}