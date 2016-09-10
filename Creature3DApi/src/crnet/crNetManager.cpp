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
#include <CRNet/crNetManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRNet/crNetContainer.h>
#include <CRNet/crConnectionListener.h>
#include <CRNet/crConnectionServer.h>
#include <CRNet/crConnectServerPacket.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTimer.h>
#include <CRCore/thread/crThread.h>
#include <gnelib.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
using namespace CRNet;
using namespace CRCore;

crNetManager::crNetManager():m_maxConnectCount(2048)
{
}

crNetManager::~crNetManager()
{
	//CRCore::notify(CRCore::NOTICE)<<"~crNetManager"<<std::endl;
	//CRCore::notify(CRCore::NOTICE)<<"~crNetManager shutdown"<<std::endl;
}

void crNetManager::attachNetBridge(crNetBridge *netBridge)
{
	m_netBridge = netBridge;
}

//ref_ptr<crNetManager> crNetManager::m_instance = NULL;
//
//crNetManager* crNetManager::getInstance()
//{
//	if (m_instance==NULL) 
//	{
//		m_instance = new crNetManager;
//		crBrain::getInstance()->pushInstance(m_instance.get());
//	}
//	if(!m_instance->m_netBridge.valid())
//	{
//		m_instance->m_netBridge = new crNetBridge;
//	}
//	return m_instance.get();
//}
//void crNetManager::clear()
//{
//	m_instance = NULL;
//}

bool crNetManager::createServer(const std::string &callback, const std::string &name, const std::string &password, int maxConnect, int port, int bufScale, int timeoutms)
{
	if(!m_netBridge.valid()) return false;
	//initializeGame("Creature3DServer",1,"server.log");
	std::string filename = name + ".log";
	//DeleteFileA(filename.c_str());
	std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
	if(fout)
	{
		time_t t = time( 0 ); 
		char tmp[20];
		strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
		fout<<tmp<<" 服务器["<<name<<"]启动"<<std::endl;
		fout.close();
	}

	crNetDataManager *netDataManager = getNetConductor()->getNetDataManager();
	netDataManager->setServerName(name);
    netDataManager->setServerPassword(password);
	netDataManager->setNetCallbackString(callback);
    m_maxConnectCount = maxConnect;
	m_netBridge->m_running = setupServer(port, bufScale, timeoutms);
	if (!isRunning())
	{
        m_netBridge = NULL;
		return false;
	}
	return true;
}
void crNetManager::setServerRefuseCode(int code)
{
    m_hostInfo.m_refuseCode = code;
}
int crNetManager::clientReconnectToServerCircle(unsigned int trytimes)
{
	if(m_hostInfo.m_hasConnected && m_hostInfo.m_refuseCode == 0)
	{
		int code = 0;
		unsigned int connectTimes = 0;
		while(code == 0 && connectTimes<trytimes)
		{
			if(code==0)
			{
				CRCore::crThread::sleep(10);
				connectTimes += 1;
			}
			code = createClient(5000);
		}
		return code;
	}
	return m_hostInfo.m_refuseCode;
}

int crNetManager::clientReconnectToServerCircle(const std::string &hostip, const std::string &callback, int playerid, const std::string &serverName, const std::string &serverPassword, int port, int bufScale, unsigned int trytimes)
{
	if(!m_netBridge.valid()) return 0;
	m_hostInfo.m_hostip = hostip;
	m_hostInfo.m_port = port;
	m_hostInfo.m_callback = callback;
	m_hostInfo.m_playerid = playerid;
	m_hostInfo.m_serverName = serverName;
	m_hostInfo.m_serverPassword = serverPassword;
	m_hostInfo.m_refuseCode = 0;
	m_hostInfo.m_bufScale = bufScale;
    int code = 0;
	unsigned int connectTimes = 0;
	while(code ==0 && connectTimes<trytimes)
	{
		if(code==0)
		{
			CRCore::crThread::sleep(10);
			connectTimes += 1;
		}
        code = createClient(5000);
	}
	return code;
}

int crNetManager::createClient(unsigned int waitms)
{//name最大长度20，password最大长度6
	if(!m_netBridge.valid()) return 0;
	
	//if(m_hostInfo.m_hasConnected)
	//    m_netBridge->removeConnection(server.toString());
	
	//initializeGame("Creature3DServer",1,"client.log");
	if(setupClient(m_hostInfo.m_hostip,m_hostInfo.m_port,m_hostInfo.m_bufScale,waitms))
	{
		m_hostInfo.m_hasConnected = true;

		crNetDataManager *netDataManager = getNetConductor()->getNetDataManager();
	    netDataManager->setServerName(m_hostInfo.m_serverName);
		if(!netDataManager->getNetCallback())
		{
			netDataManager->setNetCallbackString(m_hostInfo.m_callback);
		}
		m_netBridge->m_running = true;
		//CRCore::notify(CRCore::NOTICE)<<"Connected"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"Connected\n\0");
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return 1;
        //send username and password
  //      crConnectServerPacket packet;
		//crConnectServerPacket::s_success = 0;
		//crConnectServerPacket::buildRequestPacket(packet,m_hostInfo.m_playerid,m_hostInfo.m_serverName,m_hostInfo.m_serverPassword);
		//sendPacket("all",packet);
		//int waittime = 0;
		//while(crConnectServerPacket::s_success<=0 && waittime<waitms)
		//{
		//	CRCore::crThread::sleep(10);
		//	waittime += 10;
		//}
		//m_netBridge->m_running = crConnectServerPacket::s_success>0;

		//if(crConnectServerPacket::s_success>0)
		//{
		//    CRCore::notify(CRCore::NOTICE)<<"Connected"<<std::endl;
		//}
		//else
		//{
		//	m_netBridge->disconnectAll();
		//	CRCore::notify(CRCore::NOTICE)<<"Not Connected"<<std::endl;
		//}
		//return crConnectServerPacket::s_success;
	}
	else
	{
        m_netBridge->disconnectAll();
	}
	return 0;
}

void crNetManager::getHostInfo(std::string &hostip, std::string &callback, int &playerid, std::string &serverName, std::string &serverPassword, int &port)
{
    hostip = m_hostInfo.m_hostip;
	callback = m_hostInfo.m_callback;
	playerid = m_hostInfo.m_playerid;
	serverName = m_hostInfo.m_serverName;
	serverPassword = m_hostInfo.m_serverPassword;
	port = m_hostInfo.m_port;
}

void crNetManager::disconnectAll()
{
	if(m_netBridge.valid()) m_netBridge->disconnectAll();
}

bool crNetManager::setupClient( const std::string &host, int portNum, int bufScale, unsigned int timeout )
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return false;
	}

	return m_netBridge->setupClient(host,portNum,bufScale,timeout);
}

bool crNetManager::setupServer(int portNum,int bufScale,int timeoutms)
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return false;
	}

	return m_netBridge->setupServer(portNum,bufScale,timeoutms);
}

void crNetManager::shutdown()
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	//send
	crNetDataManager *netDataManager = getNetConductor()->getNetDataManager();
	if(getIsServer())
		netDataManager->serverShutdown();
	else
		netDataManager->clientShutdown();

	m_netBridge->shutdown();
}

void crNetManager::sendPacket( const std::string &address, GNE::Packet &packet, bool reliable )
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}
	m_netBridge->sendPacket(address,packet,reliable);
}

void crNetManager::removeConnection(const std::string &address)
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	m_netBridge->removeConnection(address);
}

void crNetManager::broadcastExceptSender(const std::string &sender, GNE::Packet &packet)
{
	if(!m_netBridge)
	{
		CRCore::notify(CRCore::FATAL)<<"not attached netBridge"<<std::endl;
		//sprintf(gDebugInfo->buf(),"not attached netBridge\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	m_netBridge->broadcastExceptSender(sender,packet);
}

void crNetManager::update(float dt)
{
	//if(!isRunning()) return;
//send
	crNetDataManager *netDataManager = getNetConductor()->getNetDataManager();
	if(getIsServer())
	{
		//if(m_netBridge.valid()) m_netBridge->newConnectionTimeoutCheck();
		m_netBridge->serverUpdate(dt);
	    netDataManager->serverUpdate(dt);
	}
	else
	    netDataManager->clientUpdate(dt);
}
void crNetManager::bridgeupdate()
{
	if(m_netBridge.valid())
		m_netBridge->bridgeupdate();
}
void crNetManager::heartJump()
{
	//if(getIsServer())
	//{
		crHeartJumpPacket packet;
		crHeartJumpPacket::buildRequestPacket(packet);
		sendPacket("all",packet);
	//}
}