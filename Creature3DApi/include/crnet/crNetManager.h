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
#ifndef CRNET_CRNETMANAGER_H
#define CRNET_CRNETMANAGER_H

#include <CRNet/crStreamPacket.h>
#include <CRNet\crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRNet/crNetBridge.h>
#include <string>
namespace CRNet
{
class crNetConductor;
class  CRNET_EXPORT crNetManager :  public CRCore::Referenced
{
friend class crNetConductor;
public:
    crNetManager();
	//static crNetManager *getInstance();
    
	void attachNetBridge(crNetBridge *netBridge);
	inline void setName( const std::string& name ) { m_netBridge->setName(name); }
	inline void setName( const char* name ) { m_netBridge->setName(name); }
	inline const std::string& getName() const { return m_netBridge->getName(); }
	inline crNetBridge *getNetBridge(){ return m_netBridge.get(); }

    void shutdown();

    void sendPacket( const std::string &address, GNE::Packet &packet, bool reliable = true );
	void removeConnection(const std::string &address);

    int getNumConnections() { return m_netBridge->getNumConnections(); }
	bool checkConnectCount() { return getNumConnections()<=m_maxConnectCount; }
	int getMaxConnectCount() { return m_maxConnectCount; }
	int getRemainConnectCount(){ return m_maxConnectCount - getNumConnections(); }

	int getNumTryConnections() { return m_netBridge->getNumTryConnections(); }

	bool getIsServer() const {return m_netBridge->getIsServer();}

	void broadcastExceptSender(const std::string &sender, GNE::Packet &packet);

	//virtual void clear();
	
	inline bool isRunning() { return m_netBridge.valid()?m_netBridge->isRunning():false; }
	//typedef std::vector< CRCore::ref_ptr<CRCore::crEventGroup> > SyncNodeVec;

	inline crNetConductor *getNetConductor(){ return m_netBridge.valid()?m_netBridge->getNetConductor():NULL; }

	//trytimes = 尝试次数
	int clientReconnectToServerCircle(unsigned int trytimes = INFINITE);
	int clientReconnectToServerCircle(const std::string &hostip, const std::string &callback, int playerid, const std::string &serverName, const std::string &serverPassword, int port, int bufScale = 1, unsigned int trytimes = INFINITE);
    void setServerRefuseCode(int code);

	void heartJump();//heartJump客户端，以保持连接

	void getHostInfo(std::string &hostip, std::string &callback, int &playerid, std::string &serverName, std::string &serverPassword, int &port);
    void disconnectAll();
protected:
	virtual ~crNetManager();

	bool createServer(const std::string &callback, const std::string &name, const std::string &password, int maxConnect, int port, int bufScale = 1, int timeoutms = 0);
	int createClient(unsigned int waitms);
	bool setupServer(int portNum,int bufScale,int timeoutms);
	bool setupClient(const std::string &host, int portNum, int bufScale, unsigned int waitms);

	void update(float dt);
	void bridgeupdate();
	//static CRCore::ref_ptr<crNetManager> m_instance;
	CRCore::ref_ptr<crNetBridge> m_netBridge; ///Reference to a crNetBridge
    //CRCore::ref_ptr<crBuildStreamVisitor> m_buildStreamVisitor;
	//CRCore::ref_ptr<CRCore::crEventGroup> m_me;
	//SyncNodeVec m_syncNodes;
	//crStreamPacket m_streamPacket;
    struct HostInfo //client
	{
		HostInfo():m_hasConnected(false),m_refuseCode(0),m_playerid(0){}
        std::string m_hostip;
		int m_port;
        std::string m_callback;
        int m_playerid;
        std::string m_serverName;
		std::string m_serverPassword;
		bool m_hasConnected;
		int m_refuseCode;
		int m_bufScale;
	};
	HostInfo m_hostInfo;

	int m_maxConnectCount;//server
};

}

#endif
