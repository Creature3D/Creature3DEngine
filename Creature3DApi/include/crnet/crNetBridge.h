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
#ifndef CRNET_CRNETBRIDGE_H
#define CRNET_CRNETBRIDGE_H

#include <CRNet/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crTimer.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <gnelib.h>
#include <string.h>
#include <set>
namespace CRNet
{

   /** This class is used as the base of all networking applications.  It 
    *  handles creating a server or a client and provides a convenient place to
    *  implement application-specific functionality.
    *  
    *  It can be used as-is, but it's anticipated that the end-user will derive
    *  from this class and override the virtual methods as needed.
    *
    *  To use this class, create an instance and call initializeGame(); this will 
    *  setup the internals of the network.  Then call either setupServer() or 
    *  setupClient() to start the networking.  To end the networking, just call
    *  the shutdown() method.
    *
    *  To pass data packets through the network, supply a GNE::Packet to SendPacketToAll().
    *  A custom GNE::Packet must be registered with GNE after InitilizeGame() using:
    *  \code   GNE::PacketParser::defaultRegisterPacket<MyCustomPacket>(); \endcode
    *
    */
class crNetManager;
class crNetConductor;
class  CRNET_EXPORT crNetBridge :  public CRCore::Referenced
{
friend class crNetManager;
public:
    crNetBridge();
	/** Set the name of object using C++ style string.*/
	inline void setName( const std::string& name ) { m_name = name; }

	/** Set the name of object using a C style string.*/
	inline void setName( const char* name ) { m_name = name; }

	/** Get the name of object.*/
	inline const std::string& getName() const { return m_name; }

    ///Initialize the networking and game environment
    static void initializeGame(const std::string &gameName, int gameVersion, const std::string &logFile );
	static void shutdownGame();

    ///Setup and create a server
	//timeout = ms
    bool setupServer(int portNum,int bufScale,int timeoutms);

    ///Setup and create a client to connect to the server
    bool setupClient(const std::string &host, int portNum, int bufScale, unsigned int waitms);

    ///shutdown the networking
    void shutdown();

    ///Send a packet to the given address
    void sendPacket( const std::string &address, GNE::Packet &packet, bool reliable = true );

    ///Get the number of connections to the network
    int getNumConnections();
    int getNumTryConnections();

    ///Is this instance setup as a server?
    bool getIsServer() const {return m_isServer;}

    //////////////////////////////////////////////////////////////////////////
    /// virtual methods
    //////////////////////////////////////////////////////////////////////////
    
    ///callback to signal a connection is successful
    virtual void onListenSuccess();

    ///callback to signal the connection to the socket failed
    virtual void onListenFailure(const GNE::Error& error, const GNE::Address& from, const GNE::ConnectionListener::sptr &listener);

    ///The GNE::Connection has been disconnected
    virtual void onDisconnect( GNE::Connection &conn);

    ///called when the remote has gracefully closed the connection
    virtual void onExit( GNE::Connection &conn);

    ///called when the server receives a new connection
    virtual void onNewConn( GNE::SyncConnection &conn);

    ///called when the client is connected to the server
    virtual void onConnect( GNE::SyncConnection &conn);

    ///one or more GNE::Packets have been received
    virtual void onReceive( GNE::Connection &conn);

    ///A fatal error has occurred in the connection
    virtual void onFailure( GNE::Connection &conn, const GNE::Error &error );

    ///A non-fatal error has occurred in the connection
    virtual void onError( GNE::Connection &conn, const GNE::Error &error);

    ///A connection failed before or during the onConnect event
    virtual void onConnectFailure( GNE::Connection &conn, const GNE::Error &error);

    virtual void onTimeout( GNE::Connection& conn );

	void broadcastExceptSender(const std::string &sender, GNE::Packet &packet);
	void removeConnection(const std::string &address);

	inline bool isRunning() { return m_running; }

	void attachNetConductor(crNetConductor *netConductor){ m_netConductor = netConductor; }
	inline crNetConductor *getNetConductor(){ return m_netConductor; }

	virtual bool tryAddNewConnection(NLsocket socket);
	virtual void removeNewConnectionTry(NLsocket socket);
	typedef std::map<NLsocket,CRCore::Timer_t> NewConnectionMap;
	void newConnectionTimeoutCheck();

	void disconnectAll();
	void bridgeupdate();
	void serverUpdate(float dt);
	typedef std::list<std::string> AddressList;
	//void getIllegalAddressList(AddressList &illegallist);
	//void addIllegalAddress(const std::string &address);
	void netErrorLog(int playerid,const std::string &logdata);
protected:
	virtual ~crNetBridge();
    ///Add a new GNE::Connection to the list of existing connections
    void addConnection( GNE::Connection *connection);

    ///Remove an existing GNE::Connection from the list of connections
    void removeConnection(GNE::Connection *connection);

	std::string m_name;
    bool m_isServer; ///<are we a server?
	bool m_running;

    typedef std::map<std::string, GNE::Connection*>::iterator ConnectionIterator;

    /** A map of network address strings to Connections*/
    std::map<std::string, GNE::Connection*> m_connections;
	//std::map<std::string, GNE::Connection*> m_connections_buf;
    //CRCore::crMutex m_mutex;
	GNE::Mutex m_mutex;

	crNetConductor* m_netConductor;
	//NewConnectionMap m_newConnections;
	//GNE::Mutex m_tryAddMutex;
	AddressList m_removeConnectionAddressList;
	GNE::Mutex m_removeConnectionAddressListMutex;
	//AddressList m_illegalAddressList;//使用非法工具IP
	//GNE::Mutex m_illegalAddressMutex;
};  
}

#endif
