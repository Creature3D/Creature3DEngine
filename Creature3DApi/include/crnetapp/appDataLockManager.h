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
/**********************************************************************
*用在应用开发dll里，用于鉴定应用层dll是否使用了多重锁
*为避免潜在的交叉锁风险，应用层代码应当避免使用多重锁				
**********************************************************************/
#ifndef CRNETAPP_DATALOCKMANAGER_H
#define CRNETAPP_DATALOCKMANAGER_H 1

#include <CRNetApp/appExport.h>
#include <CRCore/crDataManager.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include  <CRCore/thread/crCriticalMutex.h>
#include <map>

namespace CRNetApp{
//调用方法示例：CRNetApp::crDataLock scopedLock(data);
class CRNETAPP_EXPORT crDataLock
{
public:
	crDataLock(CRCore::crData *data);
	~crDataLock();
	void unlock();//用于需要提前释放的地方
private:
	CRCore::ref_ptr<CRCore::crData> m_lockedData;
};
class CRNETAPP_EXPORT crDataLockManager : public CRCore::Referenced
{
public:
	crDataLockManager();
	static crDataLockManager *getInstance();
	void clear();

	void lockData(CRCore::crData *data);
	void unlockData(CRCore::crData *data);
	typedef std::map<int,CRCore::ref_ptr<CRCore::crData> > ThreadLockDataMap;
protected:
	virtual ~crDataLockManager(){}
	static CRCore::ref_ptr<crDataLockManager> m_instance;
	CRCore::crCriticalMutex m_mutex;
	ThreadLockDataMap m_threadLockDataMap;
};
}
#endif