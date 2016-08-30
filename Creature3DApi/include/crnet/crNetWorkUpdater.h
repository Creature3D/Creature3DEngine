/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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

#ifndef CRNET_CRNETWORKUPDATER
#define CRNET_CRNETWORKUPDATER 1

#include <CRNet/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crCondition.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRNet/crNetContainer.h>
namespace CRNet
{
class CRNET_EXPORT crNetWorkUpdater: public CRCore::crThread, public CRCore::Referenced
{
public:
	crNetWorkUpdater();
	static crNetWorkUpdater *getInstance();

	virtual void clear();

	virtual void run();

	void done();
protected:
	virtual ~crNetWorkUpdater();
	static CRCore::ref_ptr<crNetWorkUpdater> m_instance;
private:
	bool m_done;
};

class CRNET_EXPORT crNetBridgeUpdater: public CRCore::crThread, public CRCore::Referenced
{
public:
	crNetBridgeUpdater();
	static crNetBridgeUpdater *getInstance();

	void setNetConductor(crNetConductor *netConductor);
	virtual void clear();

	virtual void run();

	void done();
	static bool isvalid(crNetConductor *netConductor);
protected:
	virtual ~crNetBridgeUpdater();
	static CRCore::ref_ptr<crNetBridgeUpdater> m_instance;
private:
	CRCore::ref_ptr<crNetConductor> m_netConductor;
	static bool s_done;
};

class CRNET_EXPORT crNetWorkReconnecter: public CRCore::crThread, public CRCore::Referenced
{
public:
	crNetWorkReconnecter();
	static crNetWorkReconnecter *getInstance();

	virtual void clear();

	virtual void run();

	void done();

	void wakeup();
protected:
	virtual ~crNetWorkReconnecter();
	static CRCore::ref_ptr<crNetWorkReconnecter> m_instance;
private:
	bool m_done;
	CRCore::crCondition m_condition;
	CRCore::crMutex m_mutex;
};
}

#endif
