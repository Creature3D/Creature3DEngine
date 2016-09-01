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
#include <CRNet/crNetWorkUpdater.h>
#include <CRNet/crNetContainer.h>
#include <CRCore/crBrain.h>
//#include <CRCore/crBlockDetectThread.h>
using namespace CRNet;
using namespace CRCore;

///////////////////////////////////
//
//crNetWorkUpdater
//
/////////////////////////////////////
ref_ptr<crNetWorkUpdater> crNetWorkUpdater::m_instance = NULL;

crNetWorkUpdater* crNetWorkUpdater::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crNetWorkUpdater;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crNetWorkUpdater::clear()
{
	m_instance = NULL;
}

crNetWorkUpdater::crNetWorkUpdater()
:m_done(true)
{
}

crNetWorkUpdater::~crNetWorkUpdater() 
{
	m_done = true;
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}
}

void crNetWorkUpdater::done()
{
	m_done = true;
}

void crNetWorkUpdater::run()
{
#if _DEBUG
	try
	{
#endif
		m_done = false;
		//crBlockDetectThread::getInstance()->start();
		while(!m_done)
		{
			CRNet::crNetContainer::getInstance()->update();
			//crBlockDetectThread::getInstance()->update();
		}
		//crBlockDetectThread::getInstance()->clear();
#if _DEBUG
		CRCore::notify(CRCore::ALWAYS)<<"crNetWorkUpdater end"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crNetWorkUpdater end\n\0");
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crNetWorkUpdater::run() error "<<std::endl;
		//sprintf(gDebugInfo->buf(),"crNetWorkUpdater error\n\0");
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
#endif
}
///////////////////////////////////
//
//crNetBridgeUpdater
//
/////////////////////////////////////
ref_ptr<crNetBridgeUpdater> crNetBridgeUpdater::m_instance = NULL;
bool crNetBridgeUpdater::s_done = true;
crNetBridgeUpdater* crNetBridgeUpdater::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crNetBridgeUpdater;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crNetBridgeUpdater::setNetConductor(crNetConductor *netConductor)
{
	m_netConductor = netConductor;
}
void crNetBridgeUpdater::clear()
{
	m_instance = NULL;
}
bool crNetBridgeUpdater::isvalid(crNetConductor *netConductor)
{
	if(!s_done)
	{
		return getInstance()->m_netConductor == netConductor;
	}
	return false;
}
crNetBridgeUpdater::crNetBridgeUpdater()
{
}

crNetBridgeUpdater::~crNetBridgeUpdater() 
{
	s_done = true;
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}
}

void crNetBridgeUpdater::done()
{
	s_done = true;
}

void crNetBridgeUpdater::run()
{
#if _DEBUG
	try
	{
#endif
		s_done = false;
		while(!s_done)
		{//1分钟触发一次
			m_netConductor->bridgeupdate();
			CRCore::crThread::sleep(60000);
		}
#if _DEBUG
		CRCore::notify(CRCore::ALWAYS)<<"crNetBridgeUpdater end"<<std::endl;
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crNetBridgeUpdater::run() error "<<std::endl;
	}
#endif
}
///////////////////////////////////
//
//crNetWorkReconnecter
//
/////////////////////////////////////
ref_ptr<crNetWorkReconnecter> crNetWorkReconnecter::m_instance = NULL;

crNetWorkReconnecter* crNetWorkReconnecter::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crNetWorkReconnecter;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crNetWorkReconnecter::clear()
{
	m_instance = NULL;
}

crNetWorkReconnecter::crNetWorkReconnecter()
:m_done(true)
{
	//setSchedulePriority(THREAD_PRIORITY_LOW);
}

crNetWorkReconnecter::~crNetWorkReconnecter() 
{
	m_done = true;
	wakeup();
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}
}

void crNetWorkReconnecter::done()
{
	m_done = true;
	wakeup();
}

void crNetWorkReconnecter::wakeup()
{
    m_condition.signal();
}

void crNetWorkReconnecter::run()
{
#if _DEBUG
	try
	{
#endif
	m_done = false;
	while(!m_done)
	{
		crNetCallbackManager::getInstance()->reconnectLock();
		int n = crNetCallbackManager::getInstance()->getNeedReconnectSize();
		crNetCallbackManager::getInstance()->reconnectUnlock();
		if(n==0) m_condition.wait(&m_mutex)	;

        crNetCallbackManager::getInstance()->clientReconnectToServer();
		CRCore::crThread::sleep(6000);
	}
#if _DEBUG
	CRCore::notify(CRCore::ALWAYS)<<"crNetWorkReconnecter end"<<std::endl;
	//sprintf(gDebugInfo->buf(),"crNetWorkReconnecter end\n\0");
	//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crNetWorkReconnecter::run() error "<<std::endl;
		//sprintf(gDebugInfo->buf(),"crNetWorkReconnecter::run() error\n\0");
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
#endif
}