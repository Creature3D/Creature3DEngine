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
#include <CRNetApp/appDataLockManager.h>
#include <CRCore/thread/crThread.h>
#include <windows.h>
using namespace CRNetApp;
using namespace CRCore;
/////////////////////////////////////////
//
//crDataLock
//
/////////////////////////////////////////
crDataLock::crDataLock(CRCore::crData *data)
{
	m_lockedData = data;
	crDataLockManager::getInstance()->lockData(data);
}
void crDataLock::unlock()
{
	if(m_lockedData.valid())
	{
		crDataLockManager::getInstance()->unlockData(m_lockedData.get());
		m_lockedData = NULL;
	}
}
crDataLock::~crDataLock()
{
	if(m_lockedData.valid())
	{
		crDataLockManager::getInstance()->unlockData(m_lockedData.get());
	}
}
/////////////////////////////////////////
//
//crDataLockManager
//
/////////////////////////////////////////
ref_ptr<crDataLockManager> crDataLockManager::m_instance = NULL;
crDataLockManager::crDataLockManager()
{
}
crDataLockManager* crDataLockManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDataLockManager;
	}
	return m_instance.get();
}

void crDataLockManager::clear()
{
	m_instance = NULL;
}
void crDataLockManager::lockData(CRCore::crData *data)
{
	int id = crThread::getCurrentThreadId();
	m_mutex.acquire();
	if(m_threadLockDataMap.find(id) == m_threadLockDataMap.end())
	{
		m_threadLockDataMap[id] = data;
	}
	else
	{
		MessageBox(NULL,"WARRN：为完全考虑APP层请勿使用多重锁！","Creature3D",MB_OK);
	}
	m_mutex.release();
	data->lock();
}
void crDataLockManager::unlockData(CRCore::crData *data)
{
	data->unlock();
	int id = crThread::getCurrentThreadId();
	m_mutex.acquire();
	m_threadLockDataMap.erase(id);
	m_mutex.release();
}