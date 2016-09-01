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
#include <CRNet/crNetManager.h>
#include <CRNet/crNetBridge.h>
#include <CRNet/crConnectionListener.h>
#include <CRNet/crConnectionServer.h>
#include <CRNet/crStreamPacket.h>
#include <CRNet/crNetDataManager.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crNetContainer.h>
#include <CRCore/crNotify.h>
#include <CRNet/crConnectServerPacket.h>
#include <CRCore/crBlockDetectThread.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crFileUtils.h>
//#include <CRCore/stringutils.h>
#include <time.h>
#include <gnelib.h>

#include <Windows.h>
#include <WinInet.h>
#include <Dbghelp.h>
#pragma comment( lib, "DbgHelp" )
#pragma comment( lib, "wininet.lib" )
using namespace CRNet;
using namespace CRCore;

crNetBridge::crNetBridge():
  m_isServer(true),
  m_running(false),
  m_netConductor(NULL)
{
	setName("crNetBridge");
}

crNetBridge::~crNetBridge()
{
    //shutdown();
}

/** Initialize the network and setup the game parameters.  This method must be
*  called before any other crNetBridge methods.
*  The supplied  game name and game version are used to during the connection process to 
*  verify if the client/server match.
*
* @param gameName : the name of the network game
* @param gameVersion : the version number of the game
* @param logFile : a filename to log networking debug information
*/

void crNetBridge::initializeGame(const std::string &gameName, int gameVersion, const std::string &logFile )
{
	if (GNE::initGNE(NL_IP, atexit) )
	{
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Can't initialize network"<<std::endl;
		//sprintf(gDebugInfo->buf(),"Can't initialize network\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}


	GNE::setGameInformation(gameName, gameVersion );

	GNE::GNEProtocolVersionNumber num = GNE::getGNEProtocolVersion();

	//Log::GetInstance().LogMessage(Log::LOG_DEBUG, __FUNCTION__,     
	//		"Using GNE protocol: %d.%d.%d", num.version, num.subVersion, num.build );


	#ifdef _DEBUG
	GNE::initDebug(GNE::DLEVEL1 | GNE::DLEVEL2 | GNE::DLEVEL3 | GNE::DLEVEL4 | GNE::DLEVEL5, logFile.c_str());
	#endif
}

/** Create a client and try to connect to the supplied host name.
* @param host : the name of the host to connect to
* @param portNum : the socket port number to use
* @return true if successful, false otherwise
*/
bool crNetBridge::setupClient( const std::string &host, int portNum, int bufScale, unsigned int waitms)
{
	bool ret = true;
	m_isServer = false;

	GNE::Address address( host );
	address.setPort( portNum );

	if (!address.isValid())
	{
		//CRCore::notify(CRCore::FATAL)<<"Address invalid"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"Address invalid\n\0");
		gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
		ret = false;
		return false;
	}

	//CRCore::notify(CRCore::NOTICE)<<"Connecting to server at: "<<address.toString()<<std::endl;
	char gbuf[256];
	sprintf(gbuf,"Connecting to server at:%s\n\0",address.toString().c_str());
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

	GNE::ConnectionParams params( crConnectionListener::create(this) );
	params.setUnrel(false);
	params.setInRate(0);
	params.setOutRate(0);
	params.setBufSizeScale(bufScale);
	params.setTimeout(180000);
	GNE::ClientConnection::sptr client = GNE::ClientConnection::create();
	if (client->open( address, params))
	{
		//CRCore::notify(CRCore::FATAL)<<"Can not open socket"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"Can not open socket\n\0");
		gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
		ret = false;
		return false;
	}

	client->connect();
	client->waitForConnect(waitms);
	if (client->isConnected())
	{
		//CRCore::notify(CRCore::NOTICE)<<"Client is connected"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"Client is connected\n\0");
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
	else
	{
		//CRCore::notify(CRCore::FATAL)<<"Client connection failed."<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"Client connection failed\n\0");
		gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
		ret = false;
	}

	return ret;
}

/** Create and start the server network.
* @param portNum : the socket port number to listen to
* @return true if successful, false otherwise
*/
bool crNetBridge::setupServer(int portNum,int bufScale,int timeoutms)
{
	GNE::LockMutex lock( m_mutex );

	bool ret = true;
	m_isServer = true;
	crConnectionServer::sptr connectionServer = crConnectionServer::create(0, 0, bufScale, timeoutms, this);
	if (connectionServer->open(portNum))
	{
		CRCore::notify(CRCore::FATAL)<<"Can not open server on port "<<portNum<<std::endl;
		//sprintf(gDebugInfo->buf(),"Can not open server on port %d\n\0",portNum);
		//gDebugInfo->debugInfo(CRCore::FATAL);
		ret = false;
	}
	if (connectionServer->listen() )
	{
		CRCore::notify(CRCore::FATAL)<<"Can not listed on server socket"<<std::endl;
		//sprintf(gDebugInfo->buf(),"Can not listed on server socket\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
		ret = false;
	}
	else
	{
		CRCore::notify(CRCore::NOTICE)<<"Listening for connections"<<std::endl;
		//sprintf(gDebugInfo->buf(),"Listening for connections\n\0");
		//gDebugInfo->debugInfo(CRCore::NOTICE);
	}

	return ret;
}

/** Perform a graceful shutdown of the network.  This will attempt to disconnect
*  all currently active connections.
*/
void crNetBridge::shutdown()
{
	GNE::LockMutex lock( m_mutex );
	disconnectAll();

	m_running = false;
	m_netConductor = NULL;
}

void crNetBridge::shutdownGame()
{
    GNE::shutdownGNE();
}

int crNetBridge::getNumConnections()
{
	GNE::LockMutex lock( m_mutex );
	return m_connections.size(); 
}

int crNetBridge::getNumTryConnections()
{
	//GNE::LockMutex lock( m_tryAddMutex );
	//return m_newConnections.size(); 
	return 0;
}

bool crNetBridge::tryAddNewConnection(NLsocket socket)
{
	if(m_netConductor)
	{
		int maxConnectCount = m_netConductor->getNetManager()->getMaxConnectCount();
		if(maxConnectCount > getNumConnections() + getNumTryConnections())
		{
			//GNE::LockMutex lock( m_tryAddMutex );
   //         m_newConnections[socket] = CRCore::Timer::instance()->tick();
			////m_newConnections.insert(socket);
			return true;
		}
	}
	return false;
}
void crNetBridge::removeNewConnectionTry(NLsocket socket)
{
	//GNE::LockMutex lock( m_tryAddMutex );
	//m_newConnections.erase(socket);
}

void crNetBridge::newConnectionTimeoutCheck()
{
	//std::vector<NLsocket> SocketVec;
	//{
	//	GNE::LockMutex lock( m_tryAddMutex );
	//	if(!m_newConnections.empty())
	//	{
	//		CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	//		float dt;
	//		for( NewConnectionMap::iterator itr = m_newConnections.begin();
	//			 itr != m_newConnections.end(); )
	//		{
	//			dt = CRCore::Timer::instance()->delta_s( itr->second, t1 );
	//			if(dt>120.0f)
	//			{
	//				SocketVec.push_back(itr->first);
	//				itr = m_newConnections.erase(itr);
	//			}
	//			else
	//			{
	//				++itr;
	//			}
	//		}
	//	}
	//}
	//if(!SocketVec.empty())
	//{
	//	for( std::vector<NLsocket>::iterator sitr = SocketVec.begin();
	//		sitr != SocketVec.end();
	//		++sitr )
	//	{
	//		GNE::SocketPair sock(*sitr);
	//		sock.disconnect();
	//	}
	//}

}
/** Internal method used to store the connection in a map.  Typically gets called
* from onConnect() and onNewConn() to save the connection for later use.
* @param connection : the connection to add to the list
*/
void crNetBridge::addConnection(GNE::Connection *connection)
{
	if(connection)
	{
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Storing connection to: "<<connection->getRemoteAddress(true).toString()<<std::endl;
		//sprintf(gDebugInfo->buf(),"Storing connection to:%s\n\0",connection->getRemoteAddress(true).toString());
		//gDebugInfo->debugInfo(CRCore::DEBUG_INFO);
		GNE::LockMutex lock( m_mutex );
		m_connections[connection->getRemoteAddress(true).toString()] = connection;
	}
}

/** Internal method used to remove an existing connection from the list.  If 
* the supplied connection is not in the list, it won't be removed.
* @param connection : the connection to remove from the list
*/
void crNetBridge::removeConnection(GNE::Connection *connection)
{
	GNE::LockMutex lock( m_mutex );
	//crNetConductor* netConductor = m_netConductor;
	//CRCore::notify(CRCore::NOTICE)<<"Removing connection from: "<<connection->getRemoteAddress(true).toString()<<std::endl;
	bool found = false;
	ConnectionIterator itr;// = m_connections.find(connection->getRemoteAddress(true).toString());
	for( itr = m_connections.begin();
		 itr != m_connections.end();
		 ++itr )
	{
        if(connection == itr->second)
		{
			found = true;
			break;
		}
	}
    if(found)
	{
		if(getIsServer())
		{
			m_removeConnectionAddressListMutex.acquire();
			m_removeConnectionAddressList.push_back(itr->first);
			m_removeConnectionAddressListMutex.release();
		}
		connection->disconnect();
		m_connections.erase(itr);
	}
}

void crNetBridge::disconnectAll()
{
	GNE::LockMutex lock( m_mutex );
	crNetDataManager* netDataManager = m_netConductor->getNetDataManager();
	ref_ptr<crPlayerData> playdata;
	std::string address;
	for( ConnectionIterator itr = m_connections.begin();
		itr != m_connections.end();
		++itr )
	{
		if(getIsServer())
		{
			address = itr->first;
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Removing connection from: "<<address.c_str()<<std::endl;
			//sprintf(gDebugInfo->buf(),"Removing connection from:%s\n\0",address.c_str());
			//gDebugInfo->debugInfo(CRCore::DEBUG_INFO);
			playdata = netDataManager->getPlayerDataByAddress(address);
			if(playdata.valid()) 
			{
				int playerid = playdata->getPlayerID();
				m_netConductor->removeConnection(playerid);
				netDataManager->removePlayerData(playerid);
			}
			else
			{//不是player，可能是server
				netDataManager->serverRemoveConnection(address);
			}
			itr->second->disconnect();
		}
		else
		{
			itr->second->disconnect();
		}
	}
	m_connections.clear();
}
void crNetBridge::bridgeupdate()
{
	GNE::LockMutex lock( m_mutex );
	for( ConnectionIterator itr = m_connections.begin();
		itr != m_connections.end();
		++itr )
	{
		itr->second->update();
	}
}
void crNetBridge::serverUpdate(float dt)
{
	AddressList removeConnectionAddressList;
	m_removeConnectionAddressListMutex.acquire();
	removeConnectionAddressList.swap(m_removeConnectionAddressList);
	m_removeConnectionAddressListMutex.release();
	if(m_netConductor && m_netConductor->getNetDataManager())
	{
		ref_ptr<crPlayerData> playdata;
		crNetDataManager* netDataManager = m_netConductor->getNetDataManager();
		for( AddressList::iterator itr = removeConnectionAddressList.begin();
			 itr != removeConnectionAddressList.end();
			 ++itr )
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Removing connection from: "<<address.c_str()<<std::endl;
			//sprintf(gDebugInfo->buf(),"Removing connection from:%s\n\0",address.c_str());
			//gDebugInfo->debugInfo(CRCore::DEBUG_INFO);
			playdata = netDataManager->getPlayerDataByAddress(*itr);
			if(playdata.valid())
			{
				m_netConductor->removeConnection(playdata->getPlayerID());
				netDataManager->removePlayerData(playdata->getPlayerID());
			}
			else
			{//不是player，可能是server
				netDataManager->serverRemoveConnection(*itr);
			}
		}
	}
}
void crNetBridge::removeConnection(const std::string &address)
{
	GNE::LockMutex lock( m_mutex );
	ConnectionIterator itr = m_connections.find(address);
	if(itr != m_connections.end())
	{
		removeConnection(itr->second);
	}
}
/** Sends the supplied packet to all connections in the list.  If this is
* a server, it will send the packet to all existing connections.  If this is
* a client, typically there will be only one connection: to the server.
*
* @param address : the string representation of the address to send to or "all"
* @param packet : the GNE::Packet to send to the world
* @see addConnection()
*/
void crNetBridge::sendPacket( const std::string &address, GNE::Packet &packet, bool reliable )
{
	if(!m_netConductor) return;
	//crNetConductor* netConductor = m_netConductor;
	crNetDataManager* netDataManager = m_netConductor->getNetDataManager();
	try
	{
	if(packet.getType() >= crStreamPacket::ID)
	{
		if(m_netConductor->getEnableCook())
		{
			if(!((crStreamPacket &)packet).cook())
				return;
#ifdef _DEBUG
			if(packet.getSize()>100)
			{
				CRCore::notify(CRCore::NOTICE)<<"crNetBridge::sendPacket size = "<<packet.getSize()<<" id = "<<packet.getType()<<std::endl;
				//sprintf(gDebugInfo->buf(),"crNetBridge::sendPacket size = %s id = %d\n\0",address.c_str(),packet.getType());
				//gDebugInfo->debugInfo(CRCore::NOTICE);
			}
#endif
		}
	}
	if(packet.getSize()>NL_MAX_PACKET_LENGTH-4)
	{
		char gbuf[256];
		sprintf(gbuf,"crNetBridge::sendPacket 数据包越界 size=%d,id=%d \0",packet.getSize(),packet.getType());
		CRCore::notify(CRCore::WARN)<<gbuf<<std::endl;
		std::string str = gbuf;
		if(packet.getType() >= crStreamPacket::ID && ((crStreamPacket &)packet).getStreamBuf())
		{
			std::string filename = "数据包越界.bug";
			if( CRIOManager::fileExists(filename))
			{
				DeleteFile(filename.c_str());
				_asm   int   3   //只是为了让程序崩溃
			}
			//ref_ptr<crStreamBuf> stream = ((crStreamPacket &)packet).getStreamBuf();
			//int playerid = stream->_readInt();
			//_crInt64 msg = stream->_readInt64();
			//str+=address+ " playerid:" + crArgumentParser::appItoa(playerid) + " msg:" + crArgumentParser::appI64toa(msg);

			//SYSTEMTIME sys; 
			//GetLocalTime( &sys ); 
			//char temp[128];
			//char tempfilename[128];
			////char tempgamefileName[128];
			////char tempbmpfileName[128];

			//sprintf_s(temp,"PkgErr%4d%02d%02d%02d%02d%02d%03d\0",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wMinute,sys.wMilliseconds);
			//sprintf_s(tempfilename,"%s.dmp\0",temp);
			//HANDLE dumpFile = CreateFile(tempfilename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

			//char filePath[MAX_PATH];
			//GetCurrentDirectory(MAX_PATH,filePath);
			//char exeFilePath[2*MAX_PATH];
			////const char *exePath = "reportbugs\\reportbugs.exe";
			//sprintf_s(exeFilePath,"%s\0",temp);

			//MINIDUMP_EXCEPTION_INFORMATION dumpExceptioninfo;
			////dumpExceptioninfo.ExceptionPointers = NULL;
			//dumpExceptioninfo.ThreadId = GetCurrentThreadId();
			//dumpExceptioninfo.ClientPointers = TRUE;

			//BOOL isWriteSucess = MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),dumpFile,MiniDumpNormal,&dumpExceptioninfo,NULL,NULL);
			//CloseHandle(dumpFile);
			
		}
		netErrorLog(0,str);
		return;
	}
	//crPlayerData* playdata;
	if (address.compare("all") != 0)
	{
		//m_mutex.acquire();
		//m_connections_buf =	m_connections;
		//m_mutex.release();
		GNE::LockMutex lock( m_mutex );
		ConnectionIterator itr = m_connections.find(address);
		if(itr!=m_connections.end() && itr->second)
		{
			itr->second->stream().writePacket(packet, reliable);
			//else
			//{
			//	CRCore::notify(CRCore::NOTICE)<<"crNetBridge::sendPacket Removing Broken Connect from: "<<address<<std::endl;
			//	if(getIsServer())
			//	{
			//		playdata = netDataManager->getPlayerDataByAddress(address);
			//		if(playdata) 
			//		{
			//			int playerid = playdata->getPlayerID();
			//			m_netConductor->removeConnection(playerid);
			//			netDataManager->removePlayerData(playerid);
			//		}
			//		else
			//		{//不是player，可能是server
			//			netDataManager->serverRemoveConnection(address);
			//		}
			//	}
			//	m_connections.erase(itr);
			//}
		}
	}
	else
	{
		GNE::LockMutex lock( m_mutex );
		ConnectionIterator conns = m_connections.begin();
		while (conns != m_connections.end())
		{
			if((*conns).second)
			{
				(*conns).second->stream().writePacket(packet, reliable);
				++conns;
			}
			//else
			//{
			//	//BrokenConnectVec.push_back(conns);
			//	CRCore::notify(CRCore::NOTICE)<<"crNetBridge::sendPacket Removing Broken Connect from: "<<conns->first.c_str()<<std::endl;
			//	if(getIsServer())
			//	{
			//		playdata = netDataManager->getPlayerDataByAddress(conns->first);
			//		if(playdata) 
			//		{
			//			int playerid = playdata->getPlayerID();
			//			m_netConductor->removeConnection(playerid);
			//			netDataManager->removePlayerData(playerid);
			//		}
			//		else
			//		{//不是player，可能是server
			//			netDataManager->serverRemoveConnection(conns->first);
			//		}
			//	}
			//	conns = m_connections.erase(conns);
			//}
		}
	}
	}
	catch (...)
	{
		CRCore::notify(CRCore::FATAL)<<"crNetBridge::sendPacket error"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crNetBridge::sendPacket error\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
	}
}


void crNetBridge::onListenSuccess()
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"On Listen success"<<std::endl;
}

/**
* @param error : The GNE:Error describing the failure
* @param from : The GNE::Address of the problem
* @param listener The GNE::ConnectionListen who triggered this failure
*/
void crNetBridge::onListenFailure(const GNE::Error& error, const GNE::Address& from, const GNE::ConnectionListener::sptr& listener)
{
	//CRCore::notify(CRCore::FATAL)<<"onListenFailure:"<<error.toString()<<" from:"<<from.toString()<<std::endl;
	//sprintf(gDebugInfo->buf(),"onListenFailure:%s from:%s\n\0",error.toString(),from.toString());
	//gDebugInfo->debugInfo(CRCore::FATAL);
}

/** 
* @param conn : the GNE::Connection that was just disconnected
*/
void crNetBridge::onDisconnect( GNE::Connection &conn)
{
	removeConnection(&conn);
	if(!m_isServer) m_running = false;
	//CRCore::notify(CRCore::DEBUG_INFO)<<"onDisconnect"<<std::endl;
}

/** 
* @param conn : the GNE::Connetion that just exited
*/
void crNetBridge::onExit( GNE::Connection &conn)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"onExit"<<std::endl;
	removeConnection(&conn);
}

/**
* Typically, this new connection gets stored in a list for future reference.
* @param conn : the new connection
* @see addConnection()
*/
void crNetBridge::onNewConn( GNE::SyncConnection &conn)
{
	GNE::Connection &connection = *conn.getConnection();

	addConnection( &connection );

	//CRCore::notify(CRCore::DEBUG_INFO)<<"A new connection was received"<<std::endl;
}

/**
* Typically, this connection gets stored in a list for future reference.
* @param conn : the new connection
* @see addConnection()
*/
void crNetBridge::onConnect( GNE::SyncConnection &conn)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"Connection to server was successfull"<<std::endl;

	GNE::Connection &connection = *conn.getConnection();

	addConnection( &connection );
}
//void crNetBridge::getIllegalAddressList(AddressList &illegallist)
//{
//	GNE::LockMutex lock( m_illegalAddressMutex );
//	illegallist.swap(m_illegalAddressList);
//}
//void crNetBridge::addIllegalAddress(const std::string &address)
//{
//	m_illegalAddressMutex.acquire();
//	m_illegalAddressList.push_back(address);
//	m_illegalAddressMutex.release();
//}
void crNetBridge::netErrorLog(int playerid,const std::string &logdata)
{
	std::string filename = "neterror.log";
	std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
	if(fout)
	{
		time_t t = time( 0 ); 
		char tmp[20];
		strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
		fout<<tmp<<" AccountID "<<playerid<<" "<<logdata<<std::endl;
		fout.close();
	}
}
/**
* @param : conn : the GNE::Connection which contains the GNE::Packets to be read
*/
void crNetBridge::onReceive( GNE::Connection &conn)
{
	//if(crNetContainer::getInstance()->getMainNetType() == 36)//DBServer
	//	crBlockDetectThread::getInstance()->update();
	//GNE::LockMutex lock( m_mutex );
	//try
	//{
	//ref_ptr<crNetConductor> netConductor = m_netConductor;
	//crNetConductor* netConductor = m_netConductor;
	//if(!m_netConductor) return;
	GNE::Packet *next = conn.stream().getNextPacket();
	bool needDelete = true;
	crNetDataManager* netDataManager = m_netConductor->getNetDataManager();
	while (m_netConductor && next != NULL)
	{
		needDelete = true;
		int type = 0;
		try
		{
		type = next->getType();
		//if(type == crConnectServerPacket::ID)
		//	CRCore::notify(CRCore::ALWAYS)<<"Received ConnectServerPacket Server = "<<(int)getIsServer()<<" From:"<<conn.getRemoteAddress(true).toString()<<std::endl;
		//if(/*type != WCHNET_HEARTJUMP && */type != WCHNETID+119 && type != WCHNETID+120 && type != WCHNETID+203 && type != WCHNETID+148)
		//    CRCore::notify(CRCore::NOTICE)<<"Received packet type = "<<type<<std::endl;

		//if(type == GNE::PingPacket::ID) 
		//{
		//	GNE::PingPacket &ping = *((GNE::PingPacket*)next);
		//	if (ping.isRequest())
		//	{
		//		ping.makeReply();
		//		conn.stream().writePacket(ping, true);
		//	}
		//	else
		//	{
		//		//CRCore::notify(CRCore::DEBUG_INFO)<<"Ping: "<<ping.getPingInformation().pingTime.toString()<<std::endl;
		//	}

		//}
		//else 
		if(type >= crStreamPacket::ID)
		{
			//crNetManager::getInstance()->receivePacket(conn.getRemoteAddress(true).toString(), *next);
			crStreamPacket *streamPacket = static_cast<crStreamPacket*>(next);
			streamPacket->attachNetConductor(m_netConductor);
			if(m_netConductor->getEnableCook())
			{
#ifdef _DEBUG
				if(streamPacket->getSize()>100)
				{
					CRCore::notify(CRCore::NOTICE)<<"crNetBridge::onReceive size = "<<streamPacket->getSize()<<" id = "<<streamPacket->getType()<<std::endl;
					//sprintf(gDebugInfo->buf(),"crNetBridge::onReceive size=%d id=%d\n\0",streamPacket->getSize(),streamPacket->getType());
					//gDebugInfo->debugInfo(CRCore::NOTICE);
				}
#endif
				if(!streamPacket->unCook(m_netConductor,conn))
				{
					CRCore::notify(CRCore::ALWAYS)<<type<<" 收到非法数据包:"<<conn.getRemoteAddress(true).toString()<<std::endl;
					//sprintf(gDebugInfo->buf(),"非法数据包:%d\n\0",type);
					//gDebugInfo->debugInfo(CRCore::NOTICE);
					delete next;
					next = conn.stream().getNextPacket();
					//
					//addIllegalAddress(conn.getRemoteAddress(true).toString());
					ref_ptr<crPlayerData> player = netDataManager->getPlayerDataByAddress(conn.getRemoteAddress(true).toString());
					if(player.valid())
					{
						netErrorLog(player->getPlayerID(),"收到非法数据包:"+conn.getRemoteAddress(true).toString());
					}
					else
					{
						netErrorLog(0,"收到非法数据包:"+conn.getRemoteAddress(true).toString());
					}
					removeConnection(&conn);
					continue;
				}
			}
			//bool asynParse = stream == NULL?false:stream->peekBool();
			//if(type == crConnectServerPacket::ID) asynParse = false;
			if(0/*streamPacket->isAsynParsePacket()*/)//采用同步做法，可以防止线程锁引起的各种阻塞BUG
			{//对于一些可能要长时间处理的包，比如加载数据等，应当采用异步方式解析，这样可以保证不阻塞心跳包
                needDelete = false;
				netDataManager->receiveStream(streamPacket,conn.getRemoteAddress(true).toString());
			}
			else
			{//(服务器收到的包或者) 需要立即处理的包
			    streamPacket->parsePacket(conn.getRemoteAddress(true).toString());
			}
		}
		}
		catch(...)
		{
			needDelete = true;
			CRCore::notify(CRCore::FATAL)<<"crNetBridge::onReceive error PacketType = "<<type<<std::endl;
			//sprintf(gDebugInfo->buf(),"crNetBridge::onReceive error PacketType = %d\n\0",type);
			//gDebugInfo->debugInfo(CRCore::FATAL);
		}
		if(needDelete) delete next;
		next = conn.stream().getNextPacket();
	}
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::FATAL)<<"crNetBridge::onReceive error2"<<std::endl;
	//}
}

/** 
* @param conn: The GNE::Connection that caused the failure
* @param error : The error describing the failure
*/
void crNetBridge::onFailure( GNE::Connection &conn, const GNE::Error &error )
{
	//if(!m_isServer) m_running = false;
	//CRCore::notify(CRCore::DEBUG_INFO)<<"onFailure"<<std::endl;
}

/** 
* @param conn: The GNE::Connection that caused the failure
* @param error : The error describing the failure
*/
void crNetBridge::onError( GNE::Connection &conn, const GNE::Error &error)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"onError"<<std::endl;
}

/** 
* @param conn: The GNE::Connection that caused the failure
* @param error : The error describing the failure
*/
void crNetBridge::onConnectFailure( GNE::Connection &conn, const GNE::Error &error)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<error.toString()<<" from "<<conn.getRemoteAddress(true).toString()<<std::endl;
}

void crNetBridge::onTimeout( GNE::Connection& conn )
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"onTimeout"<<std::endl;
	if(m_isServer)
	{
        removeConnection(&conn);
	}
}

void crNetBridge::broadcastExceptSender(const std::string &sender, GNE::Packet &packet)
{
	//rebroadcast the packet to all connections except for the 
	//one who sent the packet in the first place
	GNE::LockMutex lock( m_mutex );
	ConnectionIterator conns = m_connections.begin();
	while(conns != m_connections.end())
	{
		if (conns->first != sender)
		{
			sendPacket(conns->first, packet);
		}
		++conns;
	}            
}