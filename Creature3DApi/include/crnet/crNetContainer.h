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
#ifndef CRNET_CRNETCONTAINER_H
#define CRNET_CRNETCONTAINER_H

#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRNet/crExport.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRCore/thread/crMutex.h>
#include <gnelib.h>
#include <string>
#include <set>
#define DEFAULTPORT 35813
namespace CRNet
{

class  CRNET_EXPORT crNetConductor :  public CRCore::Referenced
{
public:
	crNetConductor();
	void init(int port);

	inline void setName( const std::string& name ) { m_netManager->setName(name); }
	inline void setName( const char* name ) { m_netManager->setName(name); }
	inline const std::string& getName() const { return m_netManager->getName(); }

	inline int getPort() const { return m_port; }
	inline crNetManager *getNetManager() { return m_netManager.get(); }
	inline crNetDataManager *getNetDataManager() { return m_netDataManager.get(); }

	//timeout = ms
	bool createServer(const std::string &callback, const std::string &name, const std::string &password, int maxConnect, int bufScale = 1, int timeoutms = 0);
	void update(float dt);
	void bridgeupdate();
	inline void setNetType(int type){ m_myNetType = type; }
	inline int getNetType() { return m_myNetType; }

	virtual void removeConnection(int playerid);

	void setCfgFile(const std::string &cfgfile);
	const std::string &getCfgFile() const;
	void setKeyStr(const std::string &keystr);
    const std::string &getKeyStr() const;
	void setEnableCook(bool enableCook);
	bool getEnableCook();
protected:
	virtual ~crNetConductor();
	int m_myNetType;
	int m_port;
	std::string m_cfgfile;
	std::string m_keystr;
	CRCore::ref_ptr<crNetManager> m_netManager;
	CRCore::ref_ptr<crNetDataManager> m_netDataManager;
	bool m_enableCook;
};

class  CRNET_EXPORT crConnectServerConductor :  public crNetConductor
{
public:
	crConnectServerConductor();
	//typedef std::set< CRCore::ref_ptr<crNetConductor> > ConnectedServerSet;
	typedef std::set< CRCore::ref_ptr<crNetConductor> > ConnectedServerSet;//nettype
    void insertConnectedServer(crNetConductor *netconductor);
	void removeConnectedServer(crNetConductor *netconductor);
	virtual void removeConnection(int playerid);
	void playerLogout(int playerid);
	void setLogoutPacket(crStreamPacket &packet);
protected:
	virtual ~crConnectServerConductor();
    ConnectedServerSet m_connectedServerSet;
	crStreamPacket *m_logoutPacket;
	CRCore::crCriticalMutex m_mutex;
};

class  CRNET_EXPORT crNetContainer :  public CRCore::Referenced
{
public:
	crNetContainer();
	static crNetContainer *getInstance();
	void clear();
	void setMainNetType(int netType);
	int getMainNetType();

	typedef std::multimap< int, CRCore::ref_ptr<crNetConductor> > NetConductorMap;
	void pushNetConductor(crNetConductor *netConductor);
	crNetConductor *getNetConductor(int netType/*, int line = 0*/);//line = -1表示随机选取
	crNetConductor *findNetConductor(int netType, const std::string &name);
	void getNetConductorVec(int netType, std::vector< crNetConductor *>& netConductorVec);

	///Dynamic
	void pushDynamicNetConductor(crNetConductor *netConductor);
	void removeDynamicNetConductor(int netType, int line = -1);//line = -1表示所有
	crNetConductor *getDynamicNetConductor(int netType/*, int line = 0*/);//line = -1表示随机选取
	crNetConductor *findDynamicNetConductor(int netType, const std::string &name);
	void getDynamicNetConductorVec(int netType, std::vector< crNetConductor *>& netConductorVec);
	void setFpsControl(float fps);
	inline float getFpsControl() { return m_fpsControl; }//返回ms fps = 1000.0f/ms
	void update();
	void shutdown();
	bool isShutDown();
protected:
	virtual ~crNetContainer();
	void pushRemovedConductor(crNetConductor *netConductor);
	static CRCore::ref_ptr<crNetContainer> m_instance;
	NetConductorMap m_netConductorMap;//服务器大部分都是静态链接
	NetConductorMap m_dynamicNetConductorMap;//客户端所有都是动态链接
	typedef std::deque< std::pair<CRCore::ref_ptr<crNetConductor>,float> > RemovedConductorDeque;
	RemovedConductorDeque m_removedConductorDeque;
	GNE::Mutex m_mutex;
	int m_mainNetType;
	CRCore::Timer_t m_time;
	float m_fpsControl;
	bool m_shutdown;
};

//class CRNET_EXPORT crNetWorkUpdateCallback : public CRUtil::crSceneView::NetWorkUpdateCallback
//{
//	inline virtual void operator()(void){ CRNet::crNetContainer::getInstance()->update(); }
//};

}

#endif
