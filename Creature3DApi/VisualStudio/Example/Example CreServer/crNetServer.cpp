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
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crBrain.h>
#include <CRNet/crNetContainer.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appNetDefine.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/crBlockDetectThread.h>
#include "rcfg/ConfigScriptOO.h"
#include <sys/stat.h>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crRegistry.h>
#include <CRNet/crNetContainer.h>
#include <CREncapsulation/crTableIO.h>
#include <CRNetApp/appDataClass.h>
#include <CREncapsulation/crLoader.h>
#include <CRIOManager/crLoadManager.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace CRIOManager;

void startLoginServer()
{
	std::string callback = "LoginServer";
	std::string keyStr = "LoginServer";
	std::string myName = "LoginServer";
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startLoginServer(): file open error,"<<file<<std::endl;
		return;
	}

	std::string DBUserName,DBPassword;
	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", callback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	cfg_script.Get("DBUserName", DBUserName);
	cfg_script.Get("DBPassword", DBPassword);
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	int dbindex = cfgtable->getTitleIndex("DB");
	std::string dpip,dbname;
	int dpport;
	std::string sql = "set names 'gbk'";
	if(cfgtable->queryOneRecord(0,"GlobalDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> globalDB = new CRDataBase::crDataBase;
		globalDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!globalDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		globalDB->executeUpdate(sql);
		globalDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setGlobalDB(globalDB.get());
	}
	if(cfgtable->queryOneRecord(0,"AccountDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> accountDB = new CRDataBase::crDataBase;
		accountDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!accountDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		sql = "set names 'gbk'";
		accountDB->executeUpdate(sql);
		accountDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setAccountDB(accountDB.get());
	}
	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}

	CRCore::ref_ptr<CRNet::crNetConductor> netConductor = new CRNet::crNetConductor;
	netConductor->init(port);
	netConductor->setNetType(LoginServer);
	netConductor->setName(myName);
	netConductor->setCfgFile(file);
	netConductor->setKeyStr(keyStr);
	if(!netConductor->createServer(callback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
	netContainer->pushNetConductor(netConductor.get());

	CRCore::notify(CRCore::ALWAYS)<<"Start "<<myName<<" Success"<<std::endl;
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		CRDataBase::crDataBaseManager::getInstance()->dbping();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
}

void startLoginConnectServer(int serverid)
{
	//load server.cfg
	std::string keyStr = "LoginConnectServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "LoginConnectServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startLoginConnectServer(): file open error,"<<file<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}

	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(LoginConnectServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(file);
	connectServerConductor->setKeyStr(keyStr);
	connectServerConductor->setEnableCook(true);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
    CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至LoginServer
	std::string serverip;
	int serverport;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("LoginServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		//cfg_script.Get("IP", ip);
		//cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		if(cfgtable->queryOneRecord(0,"LoginServer",record)>=0)
		{
			serverip = record[ipindex];
			serverport = atoi(record[portindex].c_str());
			netConductor = new CRNet::crNetConductor;
			netConductor->init(serverport);
			netConductor->setNetType(LoginConnectServerClient_Login);
			netConductor->setName(serverName);
			netConductor->setKeyStr("LoginServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(serverip,callback,LoginConnectServerClient_Login,serverName,serverPassword,serverport,_bufSizeScale);

			if(code<=0)
			{
				return;
			}
            
			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		else
		{
			return;
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<myName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	////////ConnectServer Update Loop
	CRCore::notify(CRCore::ALWAYS)<<"Start "<<myName<<" Success"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//CRNet::crNetBridgeUpdater::getInstance()->setNetConductor(connectServerConductor.get());
	//CRNet::crNetBridgeUpdater::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	//CRNet::crNetBridgeUpdater::getInstance()->clear();
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startParentServer(int serverid)
{
	//load server.cfg
	std::string keyStr = "ParentServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "ParentServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int parentServerLineCount = 1;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startParentServer(): file open error,"<<file<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	//cfg_script.Get("ParentServerLineCount", parentServerLineCount);

	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(ParentServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(file);
	connectServerConductor->setKeyStr(keyStr);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至CenterServer
	std::string ip;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("ParentServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		cfg_script.Get("IP", ip);
		cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		for(int i = 0; i<parentServerLineCount; i++)
		{
			netConductor = new CRNet::crNetConductor;
			netConductor->init(port);
			netConductor->setNetType(ParentServerClient_Parent);
			netConductor->setName(serverName);
			netConductor->setKeyStr("ParentServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,ParentServerClient_Parent,serverName,serverPassword,port,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startChatServer(int serverid)
{
	//load server.cfg
	std::string keyStr = "ChatServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "ChatServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int parentServerLineCount = 1;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startChatServer(): file open error,"<<file<<std::endl;
		return;
	}

	//globaldb
	std::string QueryDB,UpdateDB,DBName;
	int DBPort = 3306;
	cfg_script.Push("GlobalDataBase");
	cfg_script.Get("QueryDB", QueryDB);
	cfg_script.Get("UpdateDB", UpdateDB);
	cfg_script.Get("DBName", DBName);
	cfg_script.Get("DBPort", DBPort);
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	//////////////AccountDataBase
	std::string AccountQueryDB,AccountUpdateDB,AccountDBName;
	int AccountDBPort = 3306;
	cfg_script.Push("AccountDataBase");
	cfg_script.Get("QueryDB", AccountQueryDB);
	cfg_script.Get("UpdateDB", AccountUpdateDB);
	cfg_script.Get("DBName", AccountDBName);
	cfg_script.Get("DBPort", AccountDBPort);
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	std::string DBUserName,DBPassword;
	
	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	//cfg_script.Get("ParentServerLineCount", parentServerLineCount);

	/////globaldb
	cfg_script.Get("DBUserName", DBUserName);
	cfg_script.Get("DBPassword", DBPassword);
	CRCore::ref_ptr<CRDataBase::crDataBase> globalDB = new CRDataBase::crDataBase;
	globalDB->init(/*UpdateDB,*/QueryDB,DBPort,DBUserName,DBPassword,DBName);
	while(!globalDB->openDataBase())
	{
		CRCore::crThread::sleep(10);
	}
	//while(!globalDB->openQueryDataBase())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	std::string sql = "set names 'gbk'";
	globalDB->executeUpdate(sql);
	globalDB->executeQuery(sql);
	CRDataBase::crDataBaseManager::getInstance()->setGlobalDB(globalDB.get());

	CRCore::ref_ptr<CRDataBase::crDataBase> accountDB = new CRDataBase::crDataBase;
	accountDB->init(/*AccountUpdateDB,*/AccountQueryDB,AccountDBPort,DBUserName,DBPassword,AccountDBName);
	while(!accountDB->openDataBase())
	{
		CRCore::crThread::sleep(10);
	}
	//while(!accountDB->openQueryDataBase())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	sql = "set names 'gbk'";
	accountDB->executeUpdate(sql);
	accountDB->executeQuery(sql);
	CRDataBase::crDataBaseManager::getInstance()->setAccountDB(accountDB.get());

    //////DBMapVec
	int int1;
	if(cfg_script.Get("DBMapVec", int1))
	{
		CRDataBase::crDataBaseManager::getInstance()->initDBMapVec(int1);
	}
	CRCore::ref_ptr<CRDataBase::crDataBase> db;
	int dbid = 0;
	int nDB = 1;
	bool sucess = true;
	//std::string sql = "SET NAMES 'gbk'";
	while(cfg_script.Push("ChatDB",nDB++))
	{
		sucess &= cfg_script.Get("DBID", dbid);
		sucess &= cfg_script.Get("QueryDB", QueryDB);
		sucess &= cfg_script.Get("UpdateDB", UpdateDB);
		sucess &= cfg_script.Get("DBName", DBName);
		cfg_script.Get("DBPort", DBPort);

		if(sucess)
		{
			db = new CRDataBase::crDataBase;
			db->init(/*UpdateDB,*/QueryDB,DBPort,DBUserName,DBPassword,DBName);
			while(!db->openDataBase())
			{
				CRCore::crThread::sleep(10);
			}
			//while(!db->openQueryDataBase())
			//{
			//	CRCore::crThread::sleep(10);
			//}
			db->executeUpdate(sql);
			db->executeQuery(sql);
			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(CRNetApp::ChatDB,dbid,db.get());
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
	}

	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(ChatServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(file);
	connectServerConductor->setKeyStr(keyStr);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至CenterServer
	std::string ip;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("ParentServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		cfg_script.Get("IP", ip);
		cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		for(int i = 0; i<parentServerLineCount; i++)
		{
			netConductor = new CRNet::crNetConductor;
			netConductor->init(port);
			netConductor->setNetType(ChatServerClient_Parent);
			netConductor->setName(serverName);
			netConductor->setKeyStr("ParentServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,ChatServerClient_Parent,serverName,serverPassword,port,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		CRDataBase::crDataBaseManager::getInstance()->dbping();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startChatConnectServer(int serverid)
{
	//load server.cfg
	std::string keyStr = "ChatConnectServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "ChatConnectServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int chatServerLineCount = 1;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startChatConnectServer(): file open error,"<<file<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	//cfg_script.Get("ChatServerLineCount", chatServerLineCount);

	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(ChatConnectServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(file);
	connectServerConductor->setKeyStr(keyStr);
	connectServerConductor->setEnableCook(true);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至CenterServer
	std::string ip;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("ChatServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		cfg_script.Get("IP", ip);
		cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		for(int i = 0; i<chatServerLineCount; i++)
		{
			netConductor = new CRNet::crNetConductor;
			netConductor->init(port);
			netConductor->setNetType(ChatConnectServerClient_Chat);
			netConductor->setName(serverName);
			netConductor->setKeyStr("ChatServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,ChatConnectServerClient_Chat,serverName,serverPassword,port,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startDownloadServer(int serverid,char *cfgfile)
{
	//load server.cfg
	std::string keyStr = "DownloadServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "DownloadServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startDownloadServer(): file open error,"<<file<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	CRCore::ref_ptr<CRNet::crNetConductor> serverConductor = new CRNet::crNetConductor;
	serverConductor->init(port);
	serverConductor->setNetType(DownloadServer);
	serverConductor->setName(myName);
	serverConductor->setCfgFile(file);
	serverConductor->setKeyStr(keyStr);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
	/////Start Server
	if(!serverConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(serverConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
}

void startDownloadConnectServer(int serverid)
{
	//load server.cfg
	std::string keyStr = "DownloadConnectServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "DownloadConnectServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int downloadServerLineCount = 1;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startDownloadConnectServer(): file open error,"<<file<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	//cfg_script.Get("DownloadServerLineCount", downloadServerLineCount);

	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(DownloadConnectServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(file);
	connectServerConductor->setKeyStr(keyStr);
	connectServerConductor->setEnableCook(true);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至DownloadServer
	std::string ip;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("DownloadServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		cfg_script.Get("IP", ip);
		cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		for(int i = 0; i<downloadServerLineCount; i++)
		{
			netConductor = new CRNet::crNetConductor;
			netConductor->init(port);
			netConductor->setNetType(DownloadConnectServerClient_Download);
			netConductor->setName(serverName);
			netConductor->setKeyStr("DownloadServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,DownloadConnectServerClient_Download,serverName,serverPassword,port,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startGameServer(std::string cfgfile/*,int serverid*/)
{
	//load server.cfg
	std::string keyStr = "GameServer"/* + crArgumentParser::appItoa(serverid)*/;
	std::string callback = "GameServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	//int serverLineCount = 1;
	int bufSizeScale = 1;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,cfgfile))
	{
		CRCore::notify(CRCore::FATAL)<<"startGameServfer(): file open error,"<<cfgfile<<std::endl;
		return;
	}
	////globaldb
	//std::string QueryDB,UpdateDB,DBName;
	//int DBPort = 3306;
	//cfg_script.Push("GlobalDataBase");
	//cfg_script.Get("QueryDB", QueryDB);
	//cfg_script.Get("UpdateDB", UpdateDB);
	//cfg_script.Get("DBPort", DBPort);
	//cfg_script.Get("DBName", DBName);
	//if (!cfg_script.Pop())
	//	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	////////////////AccountDataBase
	//std::string AccountQueryDB,AccountUpdateDB,AccountDBName;
	//int AccountDBPort = 3306;
	//cfg_script.Push("AccountDataBase");
	//cfg_script.Get("QueryDB", AccountQueryDB);
	//cfg_script.Get("UpdateDB", AccountUpdateDB);
	//cfg_script.Get("DBName", AccountDBName);
	//cfg_script.Get("DBPort", AccountDBPort);
	//if (!cfg_script.Pop())
	//	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	std::string DBUserName,DBPassword;

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	/////globaldb
	cfg_script.Get("DBUserName", DBUserName);
	cfg_script.Get("DBPassword", DBPassword);

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	int dbindex = cfgtable->getTitleIndex("DB");
	std::string dpip,dbname;
	int dpport;
	std::string sql = "set names 'gbk'";
	if(cfgtable->queryOneRecord(0,"GlobalDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> globalDB = new CRDataBase::crDataBase;
		globalDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!globalDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		globalDB->executeUpdate(sql);
		globalDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setGlobalDB(globalDB.get());
	}
	if(cfgtable->queryOneRecord(0,"AccountDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> accountDB = new CRDataBase::crDataBase;
		accountDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!accountDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		sql = "set names 'gbk'";
		accountDB->executeUpdate(sql);
		accountDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setAccountDB(accountDB.get());
	}

	//CRCore::ref_ptr<CRDataBase::crDataBase> globalDB = new CRDataBase::crDataBase;
	//globalDB->init(/*UpdateDB,*/QueryDB,DBPort,DBUserName,DBPassword,DBName);
	//while(!globalDB->openDataBase())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	////while(!globalDB->openQueryDataBase())
	////{
	////	CRCore::crThread::sleep(10);
	////}
	//std::string sql = "set names 'gbk'";
	//globalDB->executeUpdate(sql);
	//globalDB->executeQuery(sql);
	//CRDataBase::crDataBaseManager::getInstance()->setGlobalDB(globalDB.get());

	//CRCore::ref_ptr<CRDataBase::crDataBase> accountDB = new CRDataBase::crDataBase;
	//accountDB->init(/*AccountUpdateDB,*/AccountQueryDB,AccountDBPort,DBUserName,DBPassword,AccountDBName);
	//while(!accountDB->openDataBase())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	////while(!accountDB->openQueryDataBase())
	////{
	////	CRCore::crThread::sleep(10);
	////}
	//sql = "set names 'gbk'";
	//accountDB->executeUpdate(sql);
	//accountDB->executeQuery(sql);
	//CRDataBase::crDataBaseManager::getInstance()->setAccountDB(accountDB.get());

	//////DBMapVec
	int int1;
	if(cfg_script.Get("DBMapVec", int1))
	{
		CRDataBase::crDataBaseManager::getInstance()->initDBMapVec(int1);
	}
	CRCore::ref_ptr<CRDataBase::crDataBase> db;
	int dbid = 0;
	do 
	{
		if(cfgtable->queryOneRecord(0,"GameDB"+crArgumentParser::appItoa(dbid),record)>=0)
		{
			dpip = record[ipindex];
			dpport = atoi(record[portindex].c_str());
			dbname = record[dbindex];
			db = new CRDataBase::crDataBase;
			db->init(dpip,dpport,DBUserName,DBPassword,dbname);
			while(!db->openDataBase())
			{
				CRCore::crThread::sleep(10);
			}
			db->executeUpdate(sql);
			db->executeQuery(sql);
			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(CRNetApp::GameDB,dbid,db.get());
		}
		else
		{
			break;
		}
	} while (++dbid);

	////DBVec
	if(cfg_script.Get("DBVec", int1))
	{
		CRDataBase::crDataBaseManager::getInstance()->initDBVec(int1);
	}
	dbid = 0;
	do 
	{
		if(cfgtable->queryOneRecord(0,"VecDB"+crArgumentParser::appItoa(dbid),record)>=0)
		{
			dpip = record[ipindex];
			dpport = atoi(record[portindex].c_str());
			dbname = record[dbindex];
			db = new CRDataBase::crDataBase;
			db->init(dpip,dpport,DBUserName,DBPassword,dbname);
			while(!db->openDataBase())
			{
				CRCore::crThread::sleep(10);
			}
			db->executeUpdate(sql);
			db->executeQuery(sql);
			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(dbid,db.get());
		}
		else
		{
			break;
		}
	} while (++dbid);

	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}
	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(GameServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(cfgfile);
	connectServerConductor->setKeyStr(keyStr);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至DBServer
	std::string serverip;
	int serverport;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("DBServer"))
	{
		int _bufSizeScale = 1;
		//serverLineCount = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		//cfg_script.Get("IP", ip);
		//cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);
		//cfg_script.Get("ServerLineCount", serverLineCount);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		if(cfgtable->queryOneRecord(0,"DBServer",record)>=0)
		{
			serverip = record[ipindex];
			serverport = atoi(record[portindex].c_str());

			netConductor = new CRNet::crNetConductor;
			netConductor->init(serverport);
			netConductor->setNetType(GameServerClient_DB);
			netConductor->setName(serverName);
			netConductor->setKeyStr("DBServer");
			netConductor->setEnableCook(true);
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(serverip,callback,GameServerClient_DB,serverName,serverPassword,serverport,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());

			CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
		}
		else
		{
			return;
		}
	}
	//////////////连接至ParentServer
	//if(cfg_script.Push("ParentServer"))
	//{
	//	int _bufSizeScale = 1;
	//	serverLineCount = 1;
	//	cfg_script.Get("NetCallbackString", callback);
	//	cfg_script.Get("ServerName", serverName);
	//	cfg_script.Get("ServerPassword", serverPassword);
	//	cfg_script.Get("IP", ip);
	//	cfg_script.Get("Port", port);
	//	cfg_script.Get("BufSizeScale", _bufSizeScale);
	//	//cfg_script.Get("ServerLineCount", serverLineCount);

	//	if (!cfg_script.Pop())
	//		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	//	for(int i = 0; i<serverLineCount; i++)
	//	{
	//		netConductor = new CRNet::crNetConductor;
	//		netConductor->init(port);
	//		netConductor->setNetType(GameServerClient_Parent);
	//		netConductor->setName(serverName);
	//		netConductor->setKeyStr("ParentServer");
	//		//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
	//		int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,GameServerClient_Parent,serverName,serverPassword,port,_bufSizeScale);

	//		if(code<=0)
	//		{
	//			return;
	//		}

	//		netContainer->pushNetConductor(netConductor.get());
	//		connectServerConductor->insertConnectedServer(netConductor.get());
	//	}
	//	CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	//}
	///
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		CRDataBase::crDataBaseManager::getInstance()->dbping();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startGameConnectServer(std::string cfgfile,int serverid)
{
	//load server.cfg
	std::string keyStr = "GameConnectServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "GameConnectServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,cfgfile))
	{
		CRCore::notify(CRCore::FATAL)<<"startGameConnectServer(): file open error,"<<cfgfile<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}
	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(GameConnectServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(cfgfile);
	connectServerConductor->setKeyStr(keyStr);
	connectServerConductor->setEnableCook(true);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();

	////////////连接至GameServer
	std::string serverip;
	int serverport;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("GameServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		//cfg_script.Get("IP", ip);
		//cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		if(cfgtable->queryOneRecord(0,"GameServer",record)>=0)
		{
			serverip = record[ipindex];
			serverport = atoi(record[portindex].c_str());

			netConductor = new CRNet::crNetConductor;
			netConductor->init(serverport);
			netConductor->setNetType(GameConnectServerClient_Game);
			netConductor->setName(serverName);
			netConductor->setKeyStr("GameServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(serverip,callback,GameConnectServerClient_Game,serverName,serverPassword,serverport,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		else
		{
			return;
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}

	int nServer = 1;
	while(cfg_script.Push("SceneServer",nServer++))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		//cfg_script.Get("IP", ip);
		//cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
		{
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		if(cfgtable->queryOneRecord(0,"SceneServer"+crArgumentParser::appItoa(nServer-1),record)>=0)
		{
			serverip = record[ipindex];
			serverport = atoi(record[portindex].c_str());

			netConductor = new CRNet::crNetConductor;
			netConductor->init(serverport);
			netConductor->setNetType(GameConnectServerClient_Scene);
			netConductor->setName(serverName);
			netConductor->setKeyStr("SceneServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(serverip,callback,GameConnectServerClient_Scene,serverName,serverPassword,serverport,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		else
		{
			return;
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}

	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(connectServerConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//CRNet::crNetBridgeUpdater::getInstance()->setNetConductor(connectServerConductor.get());
	//CRNet::crNetBridgeUpdater::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	//CRNet::crNetBridgeUpdater::getInstance()->clear();
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}

void startSceneServer(std::string cfgfile,int serverid)
{//
	//load server.cfg
	std::string keyStr = "SceneServer" + crArgumentParser::appItoa(serverid);
	std::string callback = "SceneServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,cfgfile))
	{
		CRCore::notify(CRCore::FATAL)<<"startSceneServer(): file open error,"<<cfgfile<<std::endl;
		return;
	}

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}
	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
	connectServerConductor->init(port);
	connectServerConductor->setNetType(SceneServer);
	connectServerConductor->setName(myName);
	connectServerConductor->setCfgFile(cfgfile);
	connectServerConductor->setKeyStr(keyStr);
	crAppLogoutPacket logoutpacket;
	connectServerConductor->setLogoutPacket(logoutpacket);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
	netContainer->pushNetConductor(connectServerConductor.get());

	////////////连接至GameServer
	std::string serverip;
	int serverport;
	std::string serverName;
	std::string serverPassword;
	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
	if(cfg_script.Push("GameServer"))
	{
		int _bufSizeScale = 1;
		cfg_script.Get("NetCallbackString", callback);
		cfg_script.Get("ServerName", serverName);
		cfg_script.Get("ServerPassword", serverPassword);
		//cfg_script.Get("IP", ip);
		//cfg_script.Get("Port", port);
		cfg_script.Get("BufSizeScale", _bufSizeScale);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

		if(cfgtable->queryOneRecord(0,"GameServer",record)>=0)
		{
			serverip = record[ipindex];
			serverport = atoi(record[portindex].c_str());

			netConductor = new CRNet::crNetConductor;
			netConductor->init(serverport);
			netConductor->setNetType(SceneServerClient_Game);
			netConductor->setName(serverName);
			netConductor->setKeyStr("GameServer");
			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
			int code = netConductor->getNetManager()->clientReconnectToServerCircle(serverip,callback,SceneServerClient_Game,serverName,serverPassword,serverport,_bufSizeScale);

			if(code<=0)
			{
				return;
			}

			netContainer->pushNetConductor(netConductor.get());
			connectServerConductor->insertConnectedServer(netConductor.get());
		}
		else
		{
			return;
		}
		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
	}
	if (!cfg_script.Pop())
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;

	/////Start Server
	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	CRNet::crNetWorkReconnecter::getInstance()->start();
	crBlockDetectThread::getInstance()->setTimeout(180);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
	CRNet::crNetWorkReconnecter::getInstance()->clear();
}
void startDBServer(/*int serverid*/)
{
	//load server.cfg
	std::string keyStr = "DBServer"/* + crArgumentParser::appItoa(serverid)*/;
	std::string callback = "DBServer";
	std::string connectCallback;
	std::string myName = keyStr;
	std::string myPassword;
	int maxConnect = 1000;
	int port = DEFAULTPORT;
	int dropDelay = 1000;
	int bufSizeScale = 1;

	std::string file = "server/Server.cfg";
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,file))
	{
		CRCore::notify(CRCore::FATAL)<<"startDBServer(): file open error,"<<file<<std::endl;
		return;
	}

	std::string DBUserName,DBPassword;

	cfg_script.Push(keyStr);
	cfg_script.Get("NetCallbackString", connectCallback);
	cfg_script.Get("ServerName", myName);
	cfg_script.Get("ServerPassword", myPassword);
	cfg_script.Get("MaxConnect", maxConnect);
	cfg_script.Get("DropDelay", dropDelay);
	//cfg_script.Get("Port", port);
	cfg_script.Get("BufSizeScale", bufSizeScale);

	/////globaldb
	cfg_script.Get("DBUserName", DBUserName);
	cfg_script.Get("DBPassword", DBPassword);

	CRCore::ref_ptr<crTableIO> cfgtable = CREncapsulation::crTableIO::openFile("server/ServerConfig.tab1");
	crTableIO::StrVec record;
	int ipindex = cfgtable->getTitleIndex("IP");
	int portindex = cfgtable->getTitleIndex("Port");
	int dbindex = cfgtable->getTitleIndex("DB");
	std::string dpip,dbname;
	int dpport;
	std::string sql = "set names 'gbk'";
	if(cfgtable->queryOneRecord(0,"GlobalDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> globalDB = new CRDataBase::crDataBase;
		globalDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!globalDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		globalDB->executeUpdate(sql);
		globalDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setGlobalDB(globalDB.get());
	}
	if(cfgtable->queryOneRecord(0,"AccountDataBase",record)>=0)
	{
		dpip = record[ipindex];
		dpport = atoi(record[portindex].c_str());
		dbname = record[dbindex];
		CRCore::ref_ptr<CRDataBase::crDataBase> accountDB = new CRDataBase::crDataBase;
		accountDB->init(dpip,dpport,DBUserName,DBPassword,dbname);
		while(!accountDB->openDataBase())
		{
			CRCore::crThread::sleep(10);
		}
		sql = "set names 'gbk'";
		accountDB->executeUpdate(sql);
		accountDB->executeQuery(sql);
		CRDataBase::crDataBaseManager::getInstance()->setAccountDB(accountDB.get());
	}

	//////DBMapVec
	int int1;
	if(cfg_script.Get("DBMapVec", int1))
	{
		CRDataBase::crDataBaseManager::getInstance()->initDBMapVec(int1);
	}
	CRCore::ref_ptr<CRDataBase::crDataBase> db;
	int dbid = 0;
	do 
	{
		if(cfgtable->queryOneRecord(0,"GameDB"+crArgumentParser::appItoa(dbid),record)>=0)
		{
			dpip = record[ipindex];
			dpport = atoi(record[portindex].c_str());
			dbname = record[dbindex];
			db = new CRDataBase::crDataBase;
			db->init(dpip,dpport,DBUserName,DBPassword,dbname);
			while(!db->openDataBase())
			{
				CRCore::crThread::sleep(10);
			}
			db->executeUpdate(sql);
			db->executeQuery(sql);
			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(CRNetApp::GameDB,dbid,db.get());
		}
		else
		{
			break;
		}
	} while (++dbid);
	////DBVec
	if(cfg_script.Get("DBVec", int1))
	{
		CRDataBase::crDataBaseManager::getInstance()->initDBVec(int1);
	}
	dbid = 0;
	do 
	{
		if(cfgtable->queryOneRecord(0,"VecDB"+crArgumentParser::appItoa(dbid),record)>=0)
		{
			dpip = record[ipindex];
			dpport = atoi(record[portindex].c_str());
			dbname = record[dbindex];
			db = new CRDataBase::crDataBase;
			db->init(dpip,dpport,DBUserName,DBPassword,dbname);
			while(!db->openDataBase())
			{
				CRCore::crThread::sleep(10);
			}
			db->executeUpdate(sql);
			db->executeQuery(sql);
			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(dbid,db.get());
		}
		else
		{
			break;
		}
	} while (++dbid);
	if(cfgtable->queryOneRecord(0,keyStr,record)>=0)
	{
		port = atoi(record[portindex].c_str());
	}

	CRCore::ref_ptr<CRNet::crNetConductor> serverConductor = new CRNet::crNetConductor;
	serverConductor->init(port);
	serverConductor->setNetType(DBServer);
	serverConductor->setName(myName);
	serverConductor->setCfgFile(file);
	serverConductor->setKeyStr(keyStr);
	serverConductor->setEnableCook(true);
	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
	netContainer->setFpsControl(100);
	/////Start Server
	if(!serverConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
	{
		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
		return;
	}
	netContainer->pushNetConductor(serverConductor.get());

	/////// Update Loop
	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
	crBlockDetectThread::getInstance()->setTimeout(DBTimeOut);
	crBlockDetectThread::getInstance()->start();
	//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
	//crDebugInfo::getInstance()->init(mainConductor->getKeyStr() + ".txt");
	while(!netContainer->isShutDown())
	{
		netContainer->update();
		CRDataBase::crDataBaseManager::getInstance()->dbping();
		//crBlockDetectThread::getInstance()->update();
	}
	crBlockDetectThread::getInstance()->clear();
}
//void startSceneConnectServer(std::string cfgfile,int serverid)
//{
//	//load server.cfg
//	std::string keyStr = "SceneConnectServer" + crArgumentParser::appItoa(serverid);
//	std::string callback = "SceneConnectServer";
//	std::string connectCallback;
//	std::string myName = keyStr;
//	std::string myPassword;
//	int maxConnect = 1000;
//	int port = DEFAULTPORT;
//	int interval = 10;
//	int dropDelay = 1000;
//	int sceneServerLineCount = 1;
//	float bufSizeScale = 1.0f;
//
//	rcfg::ConfigScript cfg_script;
//	struct stat buf;
//	std::string file = cfgfile;//"FirstGame/GameServer.cfg";
//	CRIOManager::crLoadCookFile scopedLoad(file);
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg read error "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push(keyStr);
//	cfg_script.Get("NetCallbackString", connectCallback);
//	cfg_script.Get("ServerName", myName);
//	cfg_script.Get("ServerPassword", myPassword);
//	cfg_script.Get("MaxConnect", maxConnect);
//	cfg_script.Get("Interval", interval);
//	cfg_script.Get("DropDelay", dropDelay);
//	cfg_script.Get("Port", port);
//	cfg_script.Get("BufSizeScale", bufSizeScale);
//
//	cfg_script.Get("SceneServerLineCount", sceneServerLineCount);
//
//	CRCore::ref_ptr<CRNet::crConnectServerConductor> connectServerConductor = new CRNet::crConnectServerConductor;
//	connectServerConductor->init(port);
//	connectServerConductor->setNetType(SceneConnectServer);
//	connectServerConductor->setName(myName);
//	connectServerConductor->setCfgFile(file);
//	connectServerConductor->setKeyStr(keyStr);
//	crAppLogoutPacket logoutpacket;
//	connectServerConductor->setLogoutPacket(logoutpacket);
//	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
//
//	////////////连接至CenterServer
//	std::string ip;
//	std::string serverName;
//	std::string serverPassword;
//	CRCore::ref_ptr<CRNet::crNetConductor> netConductor;
//	if(cfg_script.Push("SceneServer"))
//	{
//		float _bufSizeScale = 1.0f;
//		cfg_script.Get("NetCallbackString", callback);
//		cfg_script.Get("ServerName", serverName);
//		cfg_script.Get("ServerPassword", serverPassword);
//		cfg_script.Get("IP", ip);
//		cfg_script.Get("Port", port);
//		cfg_script.Get("BufSizeScale", _bufSizeScale);
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//
//		for(int i = 0; i<sceneServerLineCount; i++)
//		{
//			netConductor = new CRNet::crNetConductor;
//			netConductor->init(port);
//			netConductor->setNetType(SceneConnectServerClient_Scene);
//			netConductor->setName(serverName);
//			netConductor->setKeyStr("SceneServer");
//			//int code = netConductor->createClient(ip,callback,myName,serverName,serverPassword);
//			int code = netConductor->getNetManager()->clientReconnectToServerCircle(ip,callback,myName,serverName,serverPassword,port,_bufSizeScale);
//
//			if(code<=0)
//			{
//				return;
//			}
//
//			netContainer->pushNetConductor(netConductor.get());
//		}
//		connectServerConductor->insertConnectedServer(SceneConnectServerClient_Scene);
//		CRCore::notify(CRCore::ALWAYS)<<"已连接至："<<serverName<<std::endl;
//	}
//	if (!cfg_script.Pop())
//		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//
//	/////Start Server
//	if(!connectServerConductor->createServer(connectCallback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" error"<<std::endl;
//		return;
//	}
//	netContainer->pushNetConductor(connectServerConductor.get());
//
//	/////// Update Loop
//	CRCore::notify(CRCore::FATAL)<<"Start "<<myName<<" sucess"<<std::endl;
//	CRNet::crNetWorkReconnecter::getInstance()->start();
//	while(1)
//	{
//		CRCore::crThread::sleep(interval);
//		netContainer->update();
//	}
//	CRNet::crNetWorkReconnecter::getInstance()->clear();
//}

//void startRecordServer()
//{
//	std::string callback = "RecordServer";
//	std::string myName = "RecordServer";
//	std::string myPassword;
//	int maxConnect = 1000;
//	int port = DEFAULTPORT;
//	int interval = 10;
//	int dropDelay = 1000;
//	float bufSizeScale = 1.0f;
//
//	rcfg::ConfigScript cfg_script;
//	struct stat buf;
//	std::string file = "server/Server.cfg";
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg read error "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	std::string QueryDB,UpdateDB,DBName;
//	std::string DBUserName,DBPassword;
//	cfg_script.Push("RecordServer");
//	cfg_script.Get("NetCallbackString", callback);
//	cfg_script.Get("ServerName", myName);
//	cfg_script.Get("ServerPassword", myPassword);
//	cfg_script.Get("MaxConnect", maxConnect);
//	cfg_script.Get("Interval", interval);
//	cfg_script.Get("DropDelay", dropDelay);
//	cfg_script.Get("Port", port);
//	cfg_script.Get("BufSizeScale", bufSizeScale);
//
//	cfg_script.Get("DBUserName", DBUserName);
//	cfg_script.Get("DBPassword", DBPassword);
//
//	int int1;
//	if(cfg_script.Get("DBMapVec", int1))
//	{
//		CRDataBase::crDataBaseManager::getInstance()->initDBMapVec(int1);
//	}
//	CRCore::ref_ptr<CRDataBase::crDataBase> db;
//	int dbid = 0;
//	int nDB = 1;
//	bool sucess = true;
//	std::string sql = "SET NAMES 'gbk'";
//	while(cfg_script.Push("CSMRecDB",nDB++))
//	{
//		sucess &= cfg_script.Get("DBID", dbid);
//		sucess &= cfg_script.Get("QueryDB", QueryDB);
//		sucess &= cfg_script.Get("UpdateDB", UpdateDB);
//		sucess &= cfg_script.Get("DBName", DBName);
//
//		if(sucess)
//		{
//			db = new CRDataBase::crDataBase;
//			db->init(UpdateDB,QueryDB,DBUserName,DBPassword,DBName);
//			while(!db->openUpdateDataBase())
//			{
//				CRCore::crThread::sleep(10);
//			}
//			while(!db->openQueryDataBase())
//			{
//				CRCore::crThread::sleep(10);
//			}
//			db->executeUpdate(sql);
//			db->executeQuery(sql);
//			CRDataBase::crDataBaseManager::getInstance()->insertDataBase(CSM::RecordServerDB::CSMRecDB,dbid,db.get());
//		}
//
//		if (!cfg_script.Pop())
//			CRCore::notify(CRCore::FATAL)<<"Start RecordServer error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//	}
//	if (!cfg_script.Pop())
//		CRCore::notify(CRCore::FATAL)<<"Start RecordServer error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//
//	CRCore::ref_ptr<CRNet::crNetConductor> netConductor = new CRNet::crNetConductor;
//	netConductor->init(port);
//	netConductor->setNetType(RecordServer);
//	netConductor->setName(myName);
//	if(!netConductor->createServer(callback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Start RecordServer error"<<std::endl;
//		return;
//	}
//	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
//	netContainer->pushNetConductor(netConductor.get());
//
//	CRCore::notify(CRCore::ALWAYS)<<"Start RecordServer Success"<<std::endl;
//	while(1)
//	{
//		CRCore::crThread::sleep(interval);
//		netContainer->update();
//	}
//}
//
//void startMonitorServer()
//{
//	std::string callback = "MonitorServer";
//	std::string myName = "MonitorServer";
//	std::string myPassword;
//	int maxConnect = 1000;
//	int port = DEFAULTPORT;
//	int interval = 10;
//	int dropDelay = 1000;
//	float bufSizeScale = 1.0f;
//
//	rcfg::ConfigScript cfg_script;
//	struct stat buf;
//	std::string file = "server/Server.cfg";
//	if (stat( file.c_str(), &buf )) 
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg open error"<<std::endl;
//		return;
//	}
//
//	if (!cfg_script.Open(file.c_str()))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Server.cfg read error "<<cfg_script.GetLastError()<<std::endl;
//		return;
//	}
//	cfg_script.Push("MonitorServer");
//	cfg_script.Get("NetCallbackString", callback);
//	cfg_script.Get("ServerName", myName);
//	cfg_script.Get("ServerPassword", myPassword);
//	cfg_script.Get("MaxConnect", maxConnect);
//	cfg_script.Get("Interval", interval);
//	cfg_script.Get("DropDelay", dropDelay);
//	cfg_script.Get("Port", port);
//	cfg_script.Get("BufSizeScale", bufSizeScale);
//
//	if (!cfg_script.Pop())
//		CRCore::notify(CRCore::FATAL)<<"Start MonitorServer error,检查Server.cfg "<<cfg_script.GetLastError()<<std::endl;
//
//	CRCore::ref_ptr<CRNet::crNetConductor> netConductor = new CRNet::crNetConductor;
//	netConductor->init(port);
//	netConductor->setNetType(MonitorServer);
//	netConductor->setName(myName);
//	if(!netConductor->createServer(callback,myName,myPassword,maxConnect,bufSizeScale,dropDelay))
//	{
//		CRCore::notify(CRCore::FATAL)<<"Start MonitorServer error"<<std::endl;
//		return;
//	}
//	CRNet::crNetContainer *netContainer = CRNet::crNetContainer::getInstance();
//	netContainer->pushNetConductor(netConductor.get());
//
//	CRCore::notify(CRCore::ALWAYS)<<"Start MonitorServer Success"<<std::endl;
//	CRNet::crNetWorkReconnecter::getInstance()->start();
//	while(1)
//	{
//		CRCore::crThread::sleep(interval);
//		netContainer->update();
//	}
//}

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define NetCookFile 1 
int main( int argc, char **argv )
{
	std::string argvstr;
	for( int i = 0; i<argc; i++)
	{
		argvstr += argv[i];
		argvstr += " ";
	}
	crGlobalHandle::argvstr()=argvstr;
	CRCore::notify(CRCore::ALWAYS)<<argvstr<<std::endl;
//#ifndef _DEBUG
#ifdef CookFile
	CRIOManager::SetCooked(true);
#endif
#ifdef _RetainSourceFile
	CRIOManager::SetRetainSourceFile(true);
#endif
#if defined(CookWarn) && defined(CookFile)
	if(MessageBox(::GetActiveWindow(),"警告：程序将以COOK方式运行，请确认!","Creature3D Cook" ,MB_OKCANCEL)==IDCANCEL) return 0;
#endif
//#endif
#ifdef NetCookFile
	CRIOManager::SetNetCooked(true);
#endif
	srand(time(0));

	CRIOManager::FilePathList& filePathList = CRIOManager::crRegistry::instance()->getDataFilePathList();
	filePathList.push_back("media/textures");
	filePathList.push_back("scene");
	CRIOManager::FilePathList& libPathList = CRIOManager::crRegistry::instance()->getLibraryFilePathList();
	libPathList.push_back("bin");

	std::string log = argv[1];
	log += "d.log";
	crNetBridge::initializeGame("CreServer", 1, log );
	CRNetApp::registerNetPacket();
	CRNetApp::registerNetCallbacks();

	_putenv("CRE_SERVER=1");

	CRNetApp::registerServerDataClass();
	CRNetApp::registerServerHandleClass();
	ref_ptr<crTableIO> table = crTableIO::openFile("gamedll.crb");
	if(table.valid())
	{
		int rowcount = table->getRowCount();
		int col = 1;
		std::string scriptFile;
		for(int i = 0; i<rowcount; ++i)
		{
			CRIOManager::loadScript((*table)(i,col));
		}
	}
	crData *data = CREncapsulation::loadData("script/GlobalData.cfg");
	CRNetApp::crGlobalHandle::getInstance()->setDataClass(data);
	//_asm   int   3 自动重启测试
	if(strcmp(argv[1],"LoginServer") == 0)
	{
		//DeleteFileA("assin.log");
		crNetContainer::getInstance()->setMainNetType(CRNetApp::LoginServer);
        startLoginServer();
	}
	else if(strcmp(argv[1],"LoginConnectServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::LoginConnectServer);
		startLoginConnectServer(atoi(argv[2]));
	}
	else if(strcmp(argv[1],"ParentServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::ParentServer);
		startParentServer(atoi(argv[2]));
	}
	else if(strcmp(argv[1],"ChatServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::ChatServer);
		startChatServer(atoi(argv[2]));
	}
	else if(strcmp(argv[1],"ChatConnectServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::ChatConnectServer);
		startChatConnectServer(atoi(argv[2]));
	}
	else if(strcmp(argv[1],"DownloadServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::DownloadServer);
		startDownloadServer(atoi(argv[2]),argv[3]);
	}
	else if(strcmp(argv[1],"DownloadConnectServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::DownloadConnectServer);
		startDownloadConnectServer(atoi(argv[2]));
	}
	else if(strcmp(argv[1],"GameServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::GameServer);
		startGameServer(argv[2]/*,atoi(argv[3])*/);
	}
	else if(strcmp(argv[1],"GameConnectServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::GameConnectServer);
		startGameConnectServer(argv[2],atoi(argv[3]));
	}
	else if(strcmp(argv[1],"SceneServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::SceneServer);
		startSceneServer(argv[2],atoi(argv[3]));
	}
	else if(strcmp(argv[1],"DBServer") == 0)
	{
		crNetContainer::getInstance()->setMainNetType(CRNetApp::DBServer);
		startDBServer(/*atoi(argv[2])*/);
	}
	//else if(strcmp(argv[1],"SceneConnectServer") == 0)
	//{
	//	crNetContainer::getInstance()->setMainNetType(CRNetApp::SceneConnectServer);
	//	startSceneConnectServer(argv[2],atoi(argv[3]));
	//}
	crNetBridge::shutdownGame();
	CRNet::crNetContainer::getInstance()->clear();
	CRCore::crBrain::getInstance()->clear();
	CRIOManager::crLoadManager::getInstance()->clear();
	CRCore::crEventCallbackManager::getInstance()->clear();
	CRCore::crDataManager::getInstance()->clear();
	CRCore::crHandleManager::getInstance()->clear();
	CRIOManager::crScriptLoadManager::getInstance()->clear();
	CRCore::notify(CRCore::ALWAYS)<<"ShutDown"<<std::endl;
	return 0;
}