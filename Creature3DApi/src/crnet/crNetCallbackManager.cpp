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

#include <CRNet/crNetCallbackManager.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crErrorPacket.h>
#include <CRCore/crBrain.h>
#include <CRNet/crNetContainer.h>
#include <CRNet/crNetWorkUpdater.h>
using namespace CRCore;
using namespace CRNet;

void crNetCallback::init(crNetConductor *netConductor) 
{ 
	m_netConductor = netConductor;  
	if(m_netConductor->getNetManager()->getIsServer()) 
		serverInitImplementation();
	else
		clientInitImplementation();
}

ref_ptr<crNetCallbackManager> crNetCallbackManager::m_instance = NULL;
crNetCallbackManager::~crNetCallbackManager()
{

}
crNetCallbackManager* crNetCallbackManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crNetCallbackManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crNetCallbackManager::clear()
{
	//CRCore::ScopedLock<CRCore::crMutex> lock(m_reconnectMutex);
	m_reconnectMutex.lock();
	m_reconnectMutex.unlock();
    m_instance = NULL;
}

crNetCallback *crNetCallbackManager::getNetCallback(const std::string &name)
{
	NetCallbackMap::iterator itr = m_netCallbackMap.find(name);
	if(itr != m_netCallbackMap.end())
		return itr->second.get()->cloneType();
	return 0;
}

void crNetCallbackManager::registerNetCallback(const std::string &name, crNetCallback *callback)
{
	if(m_netCallbackMap.find(name)==m_netCallbackMap.end())
		m_netCallbackMap[name] = callback;
	else
	{
		CRCore::notify(CRCore::ALWAYS)<<"NetCallback["<<name<<"]已经存在了"<<std::endl;
		//sprintf(gDebugInfo->buf(),"NetCallback[%s]已经存在了\n\0",name.c_str());
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
	}
}

void crNetCallbackManager::insertNeedReconnectCallback(crNetCallback *callback)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_reconnectMutex);
	m_needReconnetCallbackSet.insert(callback);
	crNetWorkReconnecter::getInstance()->wakeup();
}
void crNetCallbackManager::removeNeedReconnectCallback(crNetCallback *callback)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_reconnectMutex);
	m_needReconnetCallbackSet.erase(callback);
}

void crNetCallbackManager::reconnectLock()
{
    m_reconnectMutex.lock();
}

void crNetCallbackManager::reconnectUnlock()
{
    m_reconnectMutex.unlock();
}

int crNetCallbackManager::getNeedReconnectSize()
{
    return m_needReconnetCallbackSet.size();
}
void crNetCallbackManager::clientReconnectToServer()
{
    CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_reconnectMutex);
	//std::vector< crNetCallback* > eraseList;
	CRCore::ref_ptr<crNetCallback> setItr;
	for( CallbackSet::iterator itr = m_needReconnetCallbackSet.begin();
		 itr != m_needReconnetCallbackSet.end();
		 )
	{
		setItr = *itr;
		if(setItr->clientReconnectToServer())
		    itr = m_needReconnetCallbackSet.erase(itr);
		else
		{
			++itr;
		}
	}
	//std::vector< crNetCallback* >::iterator eitr;
	//for( eitr=eraseList.begin();
	//	eitr!=eraseList.end();
	//	++eitr )
	//{
	//	m_needReconnetCallbackSet.erase(*eitr);
	//}
}