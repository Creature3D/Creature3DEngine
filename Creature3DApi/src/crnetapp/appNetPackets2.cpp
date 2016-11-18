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
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/crNotify.h>
#include <CRNetApp/appDBQuery.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crCookFile.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appNetLogic.h>
#include <CREncapsulation/crTableIO.h>
#include <CRCore/crBlockDetectThread.h>
#include <sys/stat.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CREncapsulation;
/////////////////////////////////////////
//
//crQueryGameListPacket
//
/////////////////////////////////////////
crQueryGameListPacket::crQueryGameListPacket():
crStreamPacket(crQueryGameListPacket::ID)
{
}

crQueryGameListPacket::crQueryGameListPacket( const crQueryGameListPacket &p ):
crStreamPacket(crQueryGameListPacket::ID,p)
{
}

void crQueryGameListPacket::buildRequestPacket(crQueryGameListPacket &packet,int rows,int offset)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(rows);
	streamBuf->_writeInt(offset);
}

void crQueryGameListPacket::buildRequestPacket2(crQueryGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

//void crQueryGameListPacket::buildReplyPacket(crQueryGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(address.length()+4+stream->getBufSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeString(address);
//	streamBuf->write(stream->getBuf(),stream->getBufSize());
//}
//
//void crQueryGameListPacket::buildReplyPacket2(crQueryGameListPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}

void crQueryGameListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//
		crQueryGameListPacket packet;
		crQueryGameListPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{
		std::string address = m_streamBuf->_readString();
		int rows = m_streamBuf->_readInt();
		int offset = m_streamBuf->_readInt();

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		//查询并返回GameList
		ref_ptr<crGameListLimitQueryData> gameListLimitQuery = new crGameListLimitQueryData;
		gameListLimitQuery->buildQuerySql(rows,offset);
		globalSession->executeQuery(gameListLimitQuery.get());
		crDataBase::QueryResultVec gameListLimitQueryResultVec = globalSession->getQueryResult();
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		if(!gameListLimitQueryResultVec.empty())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			int maxsize = 2+53*gameListLimitQueryResultVec.size();
			stream->createBuf(maxsize);
			stream->_writeShort(gameListLimitQueryResultVec.size());
			for( crDataBase::QueryResultVec::iterator itr = gameListLimitQueryResultVec.begin();
				itr != gameListLimitQueryResultVec.end();
				++itr )
			{
				gameListLimitQuery = dynamic_cast<crGameListLimitQueryData *>(itr->get());
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket CName="<<gameListLimitQuery->getCName()<<std::endl;
				stream->_writeInt(gameListLimitQuery->getID());
				stream->_writeString(gameListLimitQuery->getGameName());//20+4
				stream->_writeString(gameListLimitQuery->getCName());//20+4
				//stream->_writeString(gameListLimitQuery->getUIFile());//50+4
				stream->_writeUChar(gameListLimitQuery->getRoleCount());//1
			}

			crRecvGameListPacket packet; 
			crRecvGameListPacket::buildReplyPacket(packet,address,stream.get());
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
	}
	//else if(netType == LoginConnectServerClient_Login)
	//{
	//	crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
	//	if(connectServer)
	//	{
	//		std::string address = m_streamBuf->_readString();
	//		crQueryGameListPacket packet;
	//		crQueryGameListPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//		connectServer->getNetManager()->sendPacket(address,packet);
	//		//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket SendTo GameClient"<<std::endl;
	//	}
	//}
	//else if(netType == GameClient_Login)
	//{//id name
 //       crMyPlayerData::getInstance()->clearGameMap();

	//	short count = m_streamBuf->_readShort();
	//	crQueryGameListData *data;
	//	for(short i = 0; i<count; i++)
	//	{
	//		data = new crQueryGameListData;
	//		data->setID(m_streamBuf->_readInt());
	//		data->setGameName(m_streamBuf->_readString());
	//		data->setCName(m_streamBuf->_readString());
	//		data->setUIFile(m_streamBuf->_readString());
	//		data->setClientCfg(m_streamBuf->_readString());
	//		data->setBaseFileList(m_streamBuf->_readString());
	//		data->setRoleCount(m_streamBuf->_readUChar());
 //           crMyPlayerData::getInstance()->addGame(data);
	//		CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket CName="<<data->getCName()<<std::endl;
	//	}
	//	crGlobalHandle::getInstance()->doEvent(WCH_RecvGameList,MAKEINT64(NULL,NULL));
	//}
}
/////////////////////////////////////////
//
//crRecvGameListPacket
//
/////////////////////////////////////////
crRecvGameListPacket::crRecvGameListPacket():
crStreamPacket(crRecvGameListPacket::ID)
{
}

crRecvGameListPacket::crRecvGameListPacket( const crRecvGameListPacket &p ):
crStreamPacket(crRecvGameListPacket::ID,p)
{
}
void crRecvGameListPacket::buildReplyPacket(crRecvGameListPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crRecvGameListPacket::buildReplyPacket2(crRecvGameListPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRecvGameListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crRecvGameListPacket packet;
			crRecvGameListPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//id name
		crMyPlayerData::getInstance()->clearGameMap();

		short count = m_streamBuf->_readShort();
		crQueryGameListData *data;
		for(short i = 0; i<count; i++)
		{
			data = new crQueryGameListData;
			data->setID(m_streamBuf->_readInt());
			data->setGameName(m_streamBuf->_readString());
			data->setCName(m_streamBuf->_readString());
			//data->setUIFile(m_streamBuf->_readString());
			data->setRoleCount(m_streamBuf->_readUChar());
			crMyPlayerData::getInstance()->addGame(data);
			//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameListPacket CName="<<data->getCName()<<std::endl;
		}
		crGlobalHandle::getInstance()->doEvent(WCH_RecvGameList,MAKEINT64(NULL,NULL));
	}
}
///////////////////////////////////////////
////
////crLoginDownloadPacket
////
///////////////////////////////////////////
////int crLoginDownloadPacket::s_success = 0;
//crLoginDownloadPacket::crLoginDownloadPacket():
//crStreamPacket(crLoginDownloadPacket::ID)
//{
//}
//
//crLoginDownloadPacket::crLoginDownloadPacket( const crLoginDownloadPacket &p ):
//crStreamPacket(crLoginDownloadPacket::ID,p)
//{
//}
//
//void crLoginDownloadPacket::buildRequestPacket(crLoginDownloadPacket &packet)
//{
//}
//void crLoginDownloadPacket::buildRequestPacket(crLoginDownloadPacket &packet,char flg)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(1);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeChar(flg);
//}
//void crLoginDownloadPacket::buildRequestPacket2(crLoginDownloadPacket &packet,const std::string &sender,int playerid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(sender.length()+8);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeString(sender);
//	streamBuf->_writeInt(playerid);
//}
//void crLoginDownloadPacket::buildRequestPacket2(crLoginDownloadPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(sender.length()+8+stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeString(sender);
//	streamBuf->_writeInt(0);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crLoginDownloadPacket::buildReplyPacket(crLoginDownloadPacket &packet,int playerid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//}
//void crLoginDownloadPacket::buildReplyPacket(crLoginDownloadPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(address.length()+4+stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeString(address);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crLoginDownloadPacket::buildReplyPacket2(crLoginDownloadPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crLoginDownloadPacket::parsePacket(const std::string &sender)
//{//id name
//	if(!m_netConductor) return;
//
//	int netType = m_netConductor->getNetType();
//	if(netType == DownloadConnectServer)
//	{//转发到ChatServer
//		crLoginDownloadPacket packet;
//		crLoginDownloadPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
//		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(DownloadConnectServerClient_Download);
//		if(netConductor)
//		{
//			netConductor->getNetManager()->sendPacket("all",packet);
//		}
//		//CRCore::notify(CRCore::ALWAYS)<<"crLoginDownloadPacket SendTo DownloadServer"<<std::endl;
//	}
//	else if(netType == GameConnectServer)
//	{//转发到ChatServer
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Download);
//			if(netConductor)
//			{
//				crLoginDownloadPacket packet;
//				crLoginDownloadPacket::buildRequestPacket2(packet,sender,playerData->getPlayerID());
//				netConductor->getNetManager()->sendPacket("all",packet);
//			}
//		}
//		//CRCore::notify(CRCore::ALWAYS)<<"crLoginDownloadPacket SendTo DownloadServer"<<std::endl;
//	}
//	else if(netType == DownloadServer)
//	{
//		//从数据库读入我的聊天信息数据：如好友列表、群列表
//		std::string playerAddress = m_streamBuf->_readString();
//		int playerid = m_streamBuf->_readInt();
//		crNetManager *netManager = m_netConductor->getNetManager();
//		if(playerid>0)
//		{
//			crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//
//			ref_ptr<crServerPlayerData> connectedPlayerData  = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(playerid));
//			if(connectedPlayerData.valid())
//			{//对于从DownloadConnectServer发送过来的登录消息
//				crAppServerRefusePacket refusePacket;
//				crAppServerRefusePacket::buildRequestPacket(refusePacket,playerid/*,connectedPlayerData->getPlayerAdress()*/);
//				netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),refusePacket);
//			}
//
//			ref_ptr<crServerPlayerData> playerData = new crServerPlayerData;
//			playerData->setPlayerID(playerid);
//			playerData->setPlayerAdress(playerAddress);
//			playerData->setPlayerConnectServerAddress(sender);
//			netDataManager->insertPlayerData(playerData.get());
//
//			crLoginDownloadPacket packet;
//			crLoginDownloadPacket::buildReplyPacket(packet,playerid);
//			netManager->sendPacket(sender,packet);
//			//CRCore::notify(CRCore::ALWAYS)<<"crLoginDownloadPacket "<<playerid<<std::endl;
//		}
//		else
//		{
//			crLoginDownloadPacket packet;
//			crLoginDownloadPacket::buildReplyPacket(packet,playerAddress,m_streamBuf.get());
//			netManager->sendPacket(sender,packet);
//		}
//	}
//	else if(netType == DownloadConnectServerClient_Download)
//	{//转发到Client
//		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(DownloadConnectServer);
//		if(connectServer)
//		{
//			std::string address = m_streamBuf->_readString();
//			crLoginDownloadPacket packet;
//			crLoginDownloadPacket::buildReplyPacket2(packet,m_streamBuf.get());
//			connectServer->getNetManager()->sendPacket(address,packet);
//			//CRCore::notify(CRCore::ALWAYS)<<"crLoginDownloadPacket SendTo GameClient"<<std::endl;
//		}
//	}
//	else if(netType == GameConnectServerClient_Download)
//	{//转发到Client
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData.valid())
//			{
//				playerData->setDownloadServerLineConductor(m_netConductor);
//				crLoginDownloadPacket packet;
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//			}
//		}
//	}
//	else if(netType == GameClient_Download)
//	{//id name
//		char flg = m_streamBuf->_readChar();
//		crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_LoginDownloadReturn,GameClient_Download),MAKEINT64(flg,NULL));
//		//CRCore::notify(CRCore::ALWAYS)<<"crLoginDownloadPacket "<<s_success<<std::endl;
//	}
//	else if(netType == GameClient_Game)
//	{//id name
//		crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_LoginDownloadReturn,GameClient_Game),MAKEINT64(1,NULL));
//	}
//}
/////////////////////////////////////////
//
//crDownloadFilePacket
//
/////////////////////////////////////////
crDownloadFilePacket::crDownloadFilePacket():
crStreamPacket(crDownloadFilePacket::ID)
{
}

crDownloadFilePacket::crDownloadFilePacket( const crDownloadFilePacket &p ):
crStreamPacket(crDownloadFilePacket::ID,p)
{
}

void crDownloadFilePacket::buildRequestPacket(crDownloadFilePacket &packet,const std::string &filename,unsigned short downloadFlg)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(filename.size()+6);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(filename);
	streamBuf->_writeUShort(downloadFlg);
}

void crDownloadFilePacket::buildRequestPacket2(crDownloadFilePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crDownloadFilePacket::buildReplyPacket(crDownloadFilePacket &packet,int playerid,const std::string &filename, int cachid, int count, CRCore::Timer_t mtime, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(24+filename.size()+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeString(filename);
	streamBuf->_writeInt(cachid);
	streamBuf->_writeInt(count);
	streamBuf->_writeInt64(mtime);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crDownloadFilePacket::buildReplyPacket2(crDownloadFilePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crDownloadFilePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crDownloadFilePacket packet;
				crDownloadFilePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket SendTo DownloadServer "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		if(playerid>0)
		{
			std::string filename = m_streamBuf->_readString();
			std::string cookfilename = filename;
			CRIOManager::GetCookedFileName(cookfilename);
			struct stat buf;
			if (stat( cookfilename.c_str(), &buf )) 
			{
				CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket: "<<cookfilename<<" 文件不存在"<<std::endl;
				crDownloadFilePacket packet;
				crDownloadFilePacket::buildReplyPacket(packet,playerid,filename,0,0,0L,m_streamBuf.get());
				m_netConductor->getNetManager()->sendPacket(sender,packet);
				return;
			}
			ref_ptr<crFileStreamCache> streamCache = crFileStreamCacheManager::getInstance()->findFileStreamCache(filename);
			if(!streamCache.valid() || streamCache->getMTime() != buf.st_mtime)
			{
				streamCache = new crFileStreamCache;
				streamCache->setFileName(filename);
				streamCache->setMTime(buf.st_mtime);
				crFileStreamCacheManager::getInstance()->addStreamCache(streamCache.get());
			}
			int cacheid = streamCache->getCacheid();
			int count = streamCache->getStreamSize();
			CRCore::Timer_t mtime = streamCache->getMTime();
			crDownloadFilePacket packet;
			crDownloadFilePacket::buildReplyPacket(packet,playerid,filename,cacheid,count,mtime,m_streamBuf.get());
			m_netConductor->getNetManager()->sendPacket(sender,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket: "<<filename<<" id="<<cacheid<<" count="<<count<<std::endl;
		}
		//else
		//{
		//	std::string playerAddress = m_streamBuf->_readString();
		//	std::string filename = m_streamBuf->_readString();
		//	crFileStreamCache *streamCache = crFileStreamCacheManager::getInstance()->findFileStreamCache(filename);
		//	if(!streamCache)
		//	{
		//		std::string cookfilename = filename;
		//		CRIOManager::GetCookedFileName(cookfilename);
		//		struct stat buf;
		//		if (stat( cookfilename.c_str(), &buf )) 
		//		{
		//			CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket: "<<cookfilename<<" 文件不存在"<<std::endl;
		//			crDownloadFilePacket packet;
		//			crDownloadFilePacket::buildReplyPacket(packet,playerid,filename,0,0,0L,m_streamBuf.get());
		//			m_netConductor->getNetManager()->sendPacket(sender,packet);
		//			return;
		//		}
		//		streamCache = new crFileStreamCache;
		//		streamCache->setFileName(filename);
		//		streamCache->setMTime(buf.st_mtime);
		//		crFileStreamCacheManager::getInstance()->addStreamCache(streamCache);
		//	}
		//	int cacheid = streamCache->getCacheid();
		//	int count = streamCache->getStreamSize();
		//	CRCore::Timer_t mtime = streamCache->getMTime();
		//	crDownloadFilePacket packet;
		//	crDownloadFilePacket::buildReplyPacket(packet,playerAddress,filename,cacheid,count,mtime,m_streamBuf.get());
		//	m_netConductor->getNetManager()->sendPacket(sender,packet);
		//	CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket: "<<filename<<" id="<<cacheid<<" count="<<count<<std::endl;
		//}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crDownloadFilePacket packet;
				crDownloadFilePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFilePacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		std::string filename = m_streamBuf->_readString();
		int cacheid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		int mtime = m_streamBuf->_readInt64();
		if(count>0)
		{
			crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
			crDataStreamTransfer::getInstance()->createFileStream(filename,netType,cacheid,count,mtime,m_streamBuf.get());
			//CRCore::notify(CRCore::ALWAYS)<<"start DownloadFile filename = "<<filename<<std::endl;
		}
		else
		{
			crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
			unsigned short downloadFlg = m_streamBuf->_readUShort();
			data->fileNotExist(downloadFlg,filename,netType);
		}
	}
}
/////////////////////////////////////////
//
//crRequestFileStreamPacket
//
/////////////////////////////////////////
crRequestFileStreamPacket::crRequestFileStreamPacket():
crStreamPacket(crRequestFileStreamPacket::ID)
{
}

crRequestFileStreamPacket::crRequestFileStreamPacket( const crRequestFileStreamPacket &p ):
crStreamPacket(crRequestFileStreamPacket::ID,p)
{
}

void crRequestFileStreamPacket::buildRequestPacket(crRequestFileStreamPacket &packet,int cacheid, int pos)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(cacheid);
	streamBuf->_writeInt(pos);
}

void crRequestFileStreamPacket::buildRequestPacket2(crRequestFileStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestFileStreamPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crRequestFileStreamPacket packet;
				crRequestFileStreamPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestFileStreamPacket SendTo DownloadServer "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		if(playerid>0)
		{
			int cacheid = m_streamBuf->_readInt();
			int pos = m_streamBuf->_readInt();
			ref_ptr<crFileStreamCache> streamCache = crFileStreamCacheManager::getInstance()->getStreamCache(cacheid);
			if(streamCache.valid())
			{
				char *ptr = streamCache->getStream(pos);
				crFileStreamPacket packet;
				crFileStreamPacket::buildReplyPacket(packet,playerid,cacheid,ptr,streamCache->getStreamSize()-pos);
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
			else
			{
				crFileStreamPacket packet;
				crFileStreamPacket::buildReplyPacket(packet,playerid,cacheid,0,0);
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crRequestFileStreamPacket"<<std::endl;
		}
		//else
		//{
		//	std::string playerAddress = m_streamBuf->_readString();
		//	int cacheid = m_streamBuf->_readInt();
		//	int pos = m_streamBuf->_readInt();
		//	crFileStreamCache *streamCache = crFileStreamCacheManager::getInstance()->getStreamCache(cacheid);
		//	if(streamCache)
		//	{
		//		char *ptr = streamCache->getStream(pos);
		//		crFileStreamPacket packet;
		//		crFileStreamPacket::buildReplyPacket(packet,playerAddress,cacheid,ptr,streamCache->getStreamSize()-pos);
		//		m_netConductor->getNetManager()->sendPacket(sender,packet);
		//	}
		//	else
		//	{
		//		crFileStreamPacket packet;
		//		crFileStreamPacket::buildReplyPacket(packet,playerAddress,cacheid,0,0);
		//		m_netConductor->getNetManager()->sendPacket(sender,packet);
		//	}
		//	//CRCore::notify(CRCore::ALWAYS)<<"crRequestFileStreamPacket"<<std::endl;
		//}
	}
}
/////////////////////////////////////////
//
//crFileStreamPacket
//
/////////////////////////////////////////
crFileStreamPacket::crFileStreamPacket():
crStreamPacket(crFileStreamPacket::ID)
{
}

crFileStreamPacket::crFileStreamPacket( const crFileStreamPacket &p ):
crStreamPacket(crFileStreamPacket::ID,p)
{
}

void crFileStreamPacket::buildReplyPacket(crFileStreamPacket &packet,int playerid,int cacheid,char *ptr,int remainsize)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(!ptr || remainsize == 0)
	{
		streamBuf->createBuf(9);
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeInt(playerid);
		streamBuf->_writeInt(cacheid);
		streamBuf->_writeBool(false);
	}
	else
	{
		streamBuf->createBuf(MAXBUFFERSIZE);
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeInt(playerid);
		streamBuf->_writeInt(cacheid);
		streamBuf->_writeBool(true);
		if(remainsize>MAXBUFFERSIZE - 9)
			streamBuf->write(ptr,MAXBUFFERSIZE - 9);
		else
			streamBuf->write(ptr,remainsize);
	}
}
void crFileStreamPacket::buildReplyPacket2(crFileStreamPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crFileStreamPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crFileStreamPacket packet;
				crFileStreamPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestFileStreamPacket Return"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		crDataStreamTransfer::getInstance()->recvFileStream(m_streamBuf.get());
	}
}
///////////////////////////////////////////
////
////crDownloadFileEndPacket
////
///////////////////////////////////////////
//crDownloadFileEndPacket::crDownloadFileEndPacket():
//crStreamPacket(crDownloadFileEndPacket::ID)
//{
//}
//
//crDownloadFileEndPacket::crDownloadFileEndPacket( const crDownloadFileEndPacket &p ):
//crStreamPacket(crDownloadFileEndPacket::ID,p)
//{
//}
//
//void crDownloadFileEndPacket::buildRequestPacket(crDownloadFileEndPacket &packet,unsigned short downloadFlg)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(2);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeUShort(downloadFlg);
//}
//
//void crDownloadFileEndPacket::buildRequestPacket2(crDownloadFileEndPacket &packet,int playerid,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crDownloadFileEndPacket::buildReplyPacket(crDownloadFileEndPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crDownloadFileEndPacket::parsePacket(const std::string &sender)
//{//
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//	if(netType == DownloadConnectServer)
//	{//
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crDownloadConnectServerPlayerData *playerData = dynamic_cast<crDownloadConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *conductor = playerData->getDownloadServerLineConductor();
//			if(conductor)
//			{
//				crDownloadFileEndPacket packet;
//				crDownloadFileEndPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
//				conductor->getNetManager()->sendPacket("all",packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFileEndPacket SendTo DownloadServer "<<std::endl;
//			}
//		}
//	}
//	if(netType == GameConnectServer)
//	{//
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *conductor = playerData->getDownloadServerLineConductor();
//			if(conductor)
//			{
//				crDownloadFileEndPacket packet;
//				crDownloadFileEndPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
//				conductor->getNetManager()->sendPacket("all",packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFileEndPacket SendTo DownloadServer "<<std::endl;
//			}
//		}
//	}
//	else if(netType == DownloadServer)
//	{
//		m_netConductor->getNetManager()->sendPacket(sender,*this);
//	}
//	else if(netType == DownloadConnectServerClient_Download)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(DownloadConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			crDownloadConnectServerPlayerData *playerData = dynamic_cast<crDownloadConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData)
//			{
//				crDownloadFileEndPacket packet;
//				crDownloadFileEndPacket::buildReplyPacket(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFileEndPacket"<<std::endl;
//			}
//		}
//	}
//	else if(netType == GameConnectServerClient_Download)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData)
//			{
//				crDownloadFileEndPacket packet;
//				crDownloadFileEndPacket::buildReplyPacket(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crDownloadFileEndPacket"<<std::endl;
//			}
//		}
//	}
//	else if(netType == GameClient_Download || netType == GameClient_Game)
//	{
//		crGlobalHandle::getInstance()->doEvent(WCH_DownloadFileEnd,MAKEINT64(m_streamBuf.get(),NULL));
//	}
//}
/////////////////////////////////////////
//
//crLoginGamePacket
//
/////////////////////////////////////////
crLoginGamePacket::crLoginGamePacket():
crStreamPacket(crLoginGamePacket::ID)
{
}

crLoginGamePacket::crLoginGamePacket( const crLoginGamePacket &p ):
crStreamPacket(crLoginGamePacket::ID,p)
{
}

void crLoginGamePacket::buildRequestPacket(crLoginGamePacket &packet)
{
	//crQueryGameListData *gameData = crMyPlayerData::getInstance()->getSelectedGame();
 //   if(gameData)
	//{
		crStreamBuf *streamBuf = new crStreamBuf;
		streamBuf->createBuf(68);
		packet.setStreamBuf(streamBuf);
		crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
		streamBuf->_writeInt(myPlayerData->getPlayerID());
		//streamBuf->_writeString(myPlayerData->getPlayerName());
		streamBuf->_writeString(myPlayerData->getKey());
		streamBuf->_writeInt(myPlayerData->getLastServerID());
		streamBuf->_writeInt(crGlobalHandle::getVersion());
	//}
}

void crLoginGamePacket::buildRequestPacket2(crLoginGamePacket &packet,const std::string &sender,CRCore::crStreamBuf *stream)
{
	if(stream)
	{
		crStreamBuf *streamBuf = new crStreamBuf;
		streamBuf->createBuf(sender.length()+4+stream->getRemainSize());
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(sender);
		streamBuf->write(stream->getPtr(),stream->getRemainSize());
	}
}

void crLoginGamePacket::buildReplyPacket(crLoginGamePacket &packet,const std::string &address,char loginCode,crGameServerPlayerData *playerData)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	ref_ptr<crStreamBuf> stream;
	//std::string playerGameName;
	crPlayerGameData *gameData = NULL;
	if(playerData)
	{
		gameData = playerData->getPlayerGameData();
		if(gameData && gameData->getDataClass())
		{
			//playerGameName = gameData->getName();
			crData *data = gameData->getDataClass();
			stream = new crStreamBuf;
			data->excHandle(MAKEINT64(WCH_BuildPlayerStream,stream.get()));
			//void *param;
			//data->getParam(WCHDATA_DataStream,param);
			//if(param)
			//{
			//	stream = *((ref_ptr<crStreamBuf>*)param);
			//}
		}
	}
	if(loginCode==1)
	{
		streamBuf->createBuf(36 + (stream.valid()?stream->getBufSize():0)/* + playerGameName.length()*/);
	}
	else
	{
		streamBuf->createBuf(26);
	}
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);//25
	streamBuf->_writeChar(loginCode);//1
	if(loginCode==1)
	{
		streamBuf->_writeInt(playerData->getPlayerID());//4
		//streamBuf->_writeString(playerData->getPlayerName());//24
		streamBuf->_writeBool(gameData!=NULL);
		if(gameData)
		{
			//streamBuf->_writeString(playerGameName);
			streamBuf->_writeBool(gameData->getDBValid());
			streamBuf->_writeInt(gameData->getMainRoleID());
			if(stream.valid())
			{
				streamBuf->write(stream->getBuf(),stream->getBufSize());
			}
		}
	}
}

void crLoginGamePacket::buildReplyPacket2(crLoginGamePacket &packet,char loginCode,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crLoginGamePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		if(m_streamBuf.valid())
		{
			crLoginGamePacket packet;
			crLoginGamePacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Game);
			if(netConductor)
			{
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crLoginGamePacket SendTo GameServer"<<std::endl;
		}
	}
	else if(netType == GameServer)
	{
		std::string playerAddress = m_streamBuf->_readString();
		int playerid = m_streamBuf->_readInt();
		//std::string name = m_streamBuf->_readString();
		std::string key = m_streamBuf->_readString();
		int serverid = m_streamBuf->_readInt();
		int version = 0;
		if(!m_streamBuf->eof())
			version = m_streamBuf->_readInt();
		//check gameid
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		char loginCode = 0;//0游戏登陆失败，1游戏登陆成功，2帐号已经在线，3有人在尝试登陆你的帐号，4客户端版本信息不正确,5重新创建角色，6区服务器信息错误,7您刚与该区服务器断开,请稍后在登录,或者选择其它区登录
		if(!netCallback->checkServer(serverid))
		{
			loginCode = 6;
			crLoginGamePacket packet;
			crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
			netManager->sendPacket(sender,packet);
			return;
		}
		/////断线重连check
		ref_ptr<crGameServerPlayerData> dropedPlayerData;
		int remuse = netCallback->remuseDropedPlayer(playerid,key,dropedPlayerData);
		if(remuse == -1)
		{//您刚与该区服务器断开,请稍后在登录,或者选择其它区登录
			loginCode = 7;
			crLoginGamePacket packet;
			crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
			netManager->sendPacket(sender,packet);
			return;
		}
		////维护期间只能GM账号登陆
		bool canlogin = true;
		if(!crGlobalHandle::gOpenGameLogin())
		{
			if(crGlobalHandle::gData()->getGMAuthority(playerid)<0)
				canlogin = false;//非GM帐号
			//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
			//if(gmTab.valid())
			//{
			//	crTableIO::StrVec record;
			//	if(gmTab->queryOneRecord(0,crArgumentParser::appItoa(playerid),record)<0)
			//		canlogin = false;//非GM帐号
			//}
		}
		else
		{//已经开放，判断版本号
			if(version!=crGlobalHandle::getVersion())
			{//玩家版本号与服务器不符
				loginCode = 4;
				crLoginGamePacket packet;
				crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
				netManager->sendPacket(sender,packet);
				return;
			}
		}
		if(canlogin)
		{
			if(remuse==1 && dropedPlayerData.valid())
			{
				dropedPlayerData->setPlayerAdress(playerAddress);
				dropedPlayerData->setPlayerConnectServerAddress(sender);
				dropedPlayerData->setSceneID(0);
				dropedPlayerData->setSceneServerAddress("");

				//游戏登陆成功
				loginCode = 1;
				crLoginGamePacket packet;
				crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,dropedPlayerData.get());
				netManager->sendPacket(sender,packet);
				netDataManager->insertPlayerData(dropedPlayerData.get());

				crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(dropedPlayerData.get(),NULL));
				dropedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,dropedPlayerData.get()));
			}
			else
			{
				ref_ptr<crGameServerPlayerData> connectedPlayerData  = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
				if(connectedPlayerData.valid())
				{//
					loginCode = 2;//通知客户端，帐号已经处于登陆状态
					////同时通知已经登陆的玩家，有人在尝试登陆你的帐号
					crPlayerGameData *playerGameData = connectedPlayerData->getPlayerGameData();
					if( playerGameData&&playerGameData->getDataClass() )
					{
						crPlayerDataEventPacket ppacket;
						crPlayerDataEventPacket::buildReplyPacket(ppacket,playerid,WCH_SomeOneTestLogin,NULL);
						netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),ppacket);
					}
					
					if(connectedPlayerData->getSceneServerAddress().empty())
					{////同时踢下线
						crKickoffPlayerPacket kpacket;
						crKickoffPlayerPacket::buildReplyPacket(kpacket,playerid);
						netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
					}
					//netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
					netDataManager->removePlayerData(playerid);

					crLoginGamePacket packet;
					crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
					netManager->sendPacket(sender,packet);
				}
				else
				{
					bool keyCheck = false;
					crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
					ref_ptr<crDataBase> globalSession = db->beginSession();
					if(crGlobalHandle::gData()->gEnableRobotTest())
					{
						if(key.compare("Creature3DRobot$@#") == 0)
						{//只有机器人帐号可以这样登录
							if(crGlobalHandle::gData()->getGMAuthority(playerid)==2)
								keyCheck = true;
							//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
							//if(gmTab.valid())
							//{
							//	crTableIO::StrVec record;
							//	if(gmTab->queryOneRecord(0,crArgumentParser::appItoa(playerid),record)>=0 && atoi(record[1].c_str()) == 1)
							//		keyCheck = true;//
							//}
						}
					}
					//if(!keyCheck)
					//{
					std::string username;
						ref_ptr<crLoginKeyQueryData> loginGameQuery = new crLoginKeyQueryData;
						loginGameQuery->buildQuerySql(playerid);

						globalSession->executeQuery(loginGameQuery.get());
						crDataBase::QueryResultVec loginGameQueryResultVec = globalSession->getQueryResult();
						globalSession->releaseQuery();
						if(!loginGameQueryResultVec.empty())
						{
							loginGameQuery = dynamic_cast<crLoginKeyQueryData *>(loginGameQueryResultVec[0].get());
							const std::string& dbkey = loginGameQuery->getKey();
							//std::string key1;
							//std::string::size_type i = dbkey.find_first_of(':');
							//ip = std::string(dbkey.begin(),dbkey.begin()+i);
							//key1 = std::string(dbkey.begin()+i+1,dbkey.end());
							//i = playerAddress.find_last_of(':');
							//std::string playerip = std::string(playerAddress.begin(),playerAddress.begin()+i);
							if(/*playerip.compare(ip) == 0 && */key.compare(dbkey) == 0)
							{//已经做了有效时间校验，这里不再做IP校验了，有些IP是易变的
								keyCheck = true;
								username = loginGameQuery->getUserName();
							}
						}
					//}
					if(!keyCheck)
					{
						db->endSession(globalSession.get());
					}
					else
					{
						if(dropedPlayerData.valid())
						{
							db->endSession(globalSession.get());
							dropedPlayerData->setPlayerAdress(playerAddress);
							dropedPlayerData->setPlayerConnectServerAddress(sender);
							dropedPlayerData->setSceneID(0);
							dropedPlayerData->setSceneServerAddress("");

							//游戏登陆成功
							loginCode = 1;
							crLoginGamePacket packet;
							crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,dropedPlayerData.get());
							netManager->sendPacket(sender,packet);
							netDataManager->insertPlayerData(dropedPlayerData.get());

							crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(dropedPlayerData.get(),NULL));
							dropedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,dropedPlayerData.get()));
						}
						else
						{
							ref_ptr<crGameServerPlayerData> playerData;
							playerData = new crGameServerPlayerData;
							playerData->setKey(key,username);
							playerData->saveTimer() = netCallback->getSaveInterval();
							playerData->setPlayerID(playerid);
							//playerData->setPlayerName(name);
							playerData->setPlayerAdress(playerAddress);
							playerData->setPlayerConnectServerAddress(sender);
							//playerData->setNickName(loginGameQuery->getNickName());

							//ref_ptr<crUpdateGameState> updateGameState = new crUpdateGameState;
							//updateGameState->buildUpdateSql(playerid,gameid);
							//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
							//crDBSavePacket dbpacket;
							//crDBSavePacket::buildRequestPacket(dbpacket,updateGameState.get());
							//dbConductor->getNetManager()->sendPacket("all",dbpacket);
							//if(globalSession->executeUpdate(updateGameState.get()))
							//{//插入成功
							//	globalSession->commit();
							//}

							unsigned short gamedbid = 1;
							bool found = false;
							int money = 0;
							unsigned char viplv = 0;
							int vipexp = 0;
							ref_ptr<crPlayerGameDBQueryData> playergamedbQuery = new crPlayerGameDBQueryData;
							playergamedbQuery->buildQuerySql(playerid,serverid);

							globalSession->executeQuery(playergamedbQuery.get());
							crDataBase::QueryResultVec &playergamedbQueryResultVec = globalSession->getQueryResult();
							if(!playergamedbQueryResultVec.empty())
							{
								playergamedbQuery = dynamic_cast<crPlayerGameDBQueryData *>(playergamedbQueryResultVec[0].get());
								gamedbid = playergamedbQuery->getGameDBID();
								found = true;
							}
							globalSession->releaseQuery();

							if(!found)
							{
								unsigned short dbcount = 0;

								ref_ptr<crGameDBCountQueryData> gamedbcountQuery = new crGameDBCountQueryData;
								gamedbcountQuery->buildQuerySql(netCallback->getGameID());

								globalSession->executeQuery(gamedbcountQuery.get());
								crDataBase::QueryResultVec &gamedbcountQueryResultVec = globalSession->getQueryResult();
								if(!gamedbcountQueryResultVec.empty())
								{//0是系统所有，玩家从1开始
									gamedbcountQuery = dynamic_cast<crGameDBCountQueryData *>(gamedbcountQueryResultVec[0].get());
									dbcount = gamedbcountQuery->getGameDBCount();
									if(dbcount>0) gamedbid = playerid%dbcount+1;//CRCore::rangei(1,dbcount).get_random();
								}
								globalSession->releaseQuery();

								ref_ptr<crInsertPlayerGameDB> insertPlayerGameDB = new crInsertPlayerGameDB;
								insertPlayerGameDB->buildUpdateSql(playerid,serverid,gamedbid);
								//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
								//crDBSavePacket packet;
								//crDBSavePacket::buildRequestPacket(packet,insertPlayerGameDB.get());
								//dbConductor->getNetManager()->sendPacket("all",packet);
								if(globalSession->executeUpdate(insertPlayerGameDB.get()))
								{//插入成功
									globalSession->commit();
									found = true;
								}
							}
							if(found)
							{
								ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
								playerMoneyQuery->buildQuerySql(playerid,serverid);

								globalSession->executeQuery(playerMoneyQuery.get());
								crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
								if(!playerMoneyQueryResultVec.empty())
								{
									playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
									money = playerMoneyQuery->getMoney();
								}
								globalSession->releaseQuery();

								ref_ptr<crPlayerVipQueryData> playerVipQuery = new crPlayerVipQueryData;
								playerVipQuery->buildQuerySql(playerid,netCallback->getServerID());

								globalSession->executeQuery(playerVipQuery.get());
								crDataBase::QueryResultVec &playerVipQueryResultVec = globalSession->getQueryResult();
								if(!playerVipQueryResultVec.empty())
								{
									playerVipQuery = dynamic_cast<crPlayerVipQueryData *>(playerVipQueryResultVec[0].get());
									viplv = playerVipQuery->getViplv();
									vipexp = playerVipQuery->getVipexp();
								}
								globalSession->releaseQuery();
								db->endSession(globalSession.get());

								playerData->setGameDBID(gamedbid);
								////////playerGameData查询
								ref_ptr<crPlayerGameData> playerGameData;
								std::string script = netCallback->getGameName() + "/playergamedata.cfg";
								ref_ptr<crData> data = CREncapsulation::loadData(script);
								if(data.valid())
								{
									playerGameData = new crPlayerGameData;
									playerGameData->setPlayerID(playerid);
									playerGameData->setDataClass(data.get());
									data->inputParam(WCHDATA_Money,&money);
									data->inputParam(WCHDATA_VipLv,&viplv);
									data->inputParam(WCHDATA_VipExp,&vipexp);
									crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
									if(gamedb)
									{
										ref_ptr<crDataBase> gameSession = gamedb->beginSession();
										ref_ptr<crPlayerGameDataQueryData> playerGameDataQuery = new crPlayerGameDataQueryData;
										playerGameDataQuery->buildQuerySql(playerid);

										gameSession->executeQuery(playerGameDataQuery.get());
										crDataBase::QueryResultVec &playerGameDataQueryResultVec = gameSession->getQueryResult();
										if(!playerGameDataQueryResultVec.empty())
										{
											playerGameDataQuery = dynamic_cast<crPlayerGameDataQueryData *>(playerGameDataQueryResultVec[0].get());
											playerGameData->setID(playerGameDataQuery->getID());
											playerGameData->setStream(playerGameDataQuery->getStream());
											int roleid = playerGameDataQuery->getMainRoleID();
											if(roleid > 0)
											{//查询并设置mainrole
												playerGameData->setMainRoleID(roleid);
											}
											playerGameData->setDBValid(true);
										}
										//else
										//{//insert
										//	ref_ptr<crInsertPlayerGameData> insertPlayerGameData = new crInsertPlayerGameData;
										//	insertPlayerGameData->buildUpdateSql(playerid);
										//	if(gameSession->executeUpdate(insertPlayerGameData.get()))
										//	{
										//		gameSession->commit();
										//	}
										//}
										gameSession->releaseQuery();
										gamedb->endSession(gameSession.get());
										if(playerGameData->getStream())
										{
											data->inputParam(WCHDATA_DataStream,playerGameData->getStream());
											playerGameData->setStream(NULL);
										}
										playerData->setPlayerGameData(playerGameData.get());
										data->excHandle(MAKEINT64(WCH_InitData,ID_LoginGameServer));

										//crNetConductor *parentServer = crNetContainer::getInstance()->getNetConductor(GameServerClient_Parent);
										//if(parentServer)
										//{
										//	crLoginParentPacket loginParentPacket;
										//	crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerAddress);
										//	parentServer->getNetManager()->sendPacket("all",loginParentPacket);
										//	playerData->setParentServerLineConductor(parentServer);
										//}

										//游戏登陆成功
										loginCode = 1;
										crLoginGamePacket packet;
										crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,playerData.get());
										netManager->sendPacket(sender,packet);
										netDataManager->insertPlayerData(playerData.get());

										crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(playerData.get(),NULL));
										playerGameData->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,playerData.get()));
									}
									else
									{
										CRCore::notify(CRCore::ALWAYS)<<"crLoginGamePacket Error 没有找到GameDB dbid = "<<playerData->getGameDBID()<<std::endl;
									}
								}
							}
							else
							{
								db->endSession(globalSession.get());
							}
						}
					}
				}
			}
		}
		if(loginCode == 0)
		{//游戏登陆失败
			crLoginGamePacket packet;
			crLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
			netManager->sendPacket(sender,packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginGamePacket "<<loginCode<<std::endl;
	}
	else if(netType == GameConnectServerClient_Game)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			char loginCode = m_streamBuf->_readChar();
			if(loginCode==1)
			{
				int playerid = m_streamBuf->_readInt();
				crNetDataManager *netDataManager = connectServer->getNetDataManager();
				netDataManager->lockPlayerDataMap();
				//ref_ptr<crGameConnectServerPlayerData> oldPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(playerid));
				//if(oldPlayerData.valid())
				//{
				//	crNetConductor *downloadServer = oldPlayerData->getDownloadServerLineConductor();
				//	if(downloadServer)
				//	{
				//		crAppServerRefusePacket refusePacket;
				//		crAppServerRefusePacket::buildRequestPacket(refusePacket,playerid/*,oldPlayerData->getPlayerAdress()*/);
				//		downloadServer->getNetManager()->sendPacket("all",refusePacket);
				//	}
				//	crAppServerRefusePacket packet;
				//	crAppServerRefusePacket::buildRequestPacket2(packet,-1);
				//	connectServer->getNetManager()->sendPacket(oldPlayerData->getPlayerAdress(),packet);
				//}

				crGameConnectServerPlayerData *playerData = new crGameConnectServerPlayerData;
				playerData->setPlayerID(playerid);
				//playerData->setPlayerName(m_streamBuf->_readString());
				playerData->setPlayerAdress(address);
				playerData->setGameServerLineConductor(m_netConductor);

				netDataManager->insertPlayerData(playerData);
				netDataManager->unLockPlayerDataMap();
			}
			crLoginGamePacket packet;
			crLoginGamePacket::buildReplyPacket2(packet,loginCode,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crLoginGamePacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		char loginCode = m_streamBuf->_readChar();
		if(loginCode==1)
		{
			if(crDisplaySettings::instance()->getRunMode()<crDisplaySettings::Robot)
			{
				crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
				char buf[128];
				memset(buf,0,128);
				sprintf_s(buf,"bin\\Jiangxingjue.exe -WebLogin %s|%d|%d|%d",myPlayerData->getKey().c_str(),
					myPlayerData->getPlayerID(),myPlayerData->getLastServerID(),myPlayerData->getISP());//key|playerid|serverid|isp
				CRNetApp::crGlobalHandle::argvstr() = buf;
			}

			crGlobalHandle::getInstance()->setStartupLoadingProgress(0.89f);
			const std::string &gameName = crMyPlayerData::getInstance()->getSelectedGame()->getGameName();
			crData *globalData = crGlobalHandle::getInstance()->getDataClass();
			std::string globalDataFile = gameName + "/GlobalData.cfg";
			CREncapsulation::loadIncludeData(globalDataFile,globalData);
			short sceneid = CRNetApp::crGlobalHandle::getInstance()->gData()->gFirstGameSceneID();
			crData *data = crRunGameHandle::getInstance()->getDataClass();
			data->inputParam(WCHDATA_SceneID,&sceneid);
			crGlobalHandle::getInstance()->setStartupLoadingProgress(0.99f);
			if(m_streamBuf->_readBool())
			{
				crPlayerGameData *playerGameData = new crPlayerGameData;
				//playerGameData->setName(m_streamBuf->_readString());
				playerGameData->setDBValid(m_streamBuf->_readBool());
				playerGameData->setMainRoleID(m_streamBuf->_readInt());
				crMyPlayerData::getInstance()->setPlayerGameData(playerGameData);
				std::string script = crMyPlayerData::getInstance()->getSelectedGame()->getGameName() + "/playergamedata.cfg";
				ref_ptr<crData> data = CREncapsulation::loadData(script);
				if(data.valid())
				{
					playerGameData->setPlayerID(crMyPlayerData::getInstance()->getPlayerID());
					playerGameData->setDataClass(data.get());
					if(m_streamBuf->getRemainSize()>0)
					{
						data->inputParam(WCHDATA_PlayerStream,m_streamBuf.get());
					}
					data->excHandle(MAKEINT64(WCH_InitData,playerGameData));
					playerGameData->doEvent(WCH_PlayerLogin,NULL);
					//CRCore::notify(CRCore::ALWAYS)<<"crLoginGamePacket doEvent WCH_PlayerLogin"<<std::endl;
					//void *param;
					//data->getParam(WCHDATA_HomeID,param);
					//if(param)
					//{
					//	int sceneID = *(int *)param;
					//	if(sceneID>0)
					//	{//如果有homescene,玩家登陆有直接进入homescene
					//		crData *runGameData = crRunGameHandle::getInstance()->getDataClass();
					//		runGameData->inputParam(WCHDATA_SceneID,&sceneID);
					//	}
					//}
				}
				int mainRoleID = playerGameData->getMainRoleID();
				if(mainRoleID>0)
				{
					crMyPlayerData::getInstance()->setLoadMainRoleCase(1);
					crQueryRoleInfoPacket packet;
					crQueryRoleInfoPacket::buildRequestPacket(packet,mainRoleID);
					m_netConductor->getNetManager()->sendPacket("all",packet);
					return;
				}
			}
			crGlobalHandle::getInstance()->setStartupLoadingProgress(1.0f);
		}
		else
		{
			crNetContainer::getInstance()->removeDynamicNetConductor(GameClient_Game);
		}
        crGlobalHandle::getInstance()->doEvent(WCH_LoginGameReturn,MAKEINT64(loginCode,NULL));
	}
}
/////////////////////////////////////////
//
//crQueryGameRoleListPacket
//
/////////////////////////////////////////
crQueryGameRoleListPacket::crQueryGameRoleListPacket():
crStreamPacket(crQueryGameRoleListPacket::ID)
{
}

crQueryGameRoleListPacket::crQueryGameRoleListPacket( const crQueryGameRoleListPacket &p ):
crStreamPacket(crQueryGameRoleListPacket::ID,p)
{
}

void crQueryGameRoleListPacket::buildRequestPacket(crQueryGameRoleListPacket &packet)
{
}

void crQueryGameRoleListPacket::buildRequestPacket2(crQueryGameRoleListPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crQueryGameRoleListPacket::buildReplyPacket(crQueryGameRoleListPacket &packet,int playerid,int streamid,int count)
{
	//crStreamBuf *streamBuf = new crStreamBuf;
	//streamBuf->createBuf(4+stream->getBufSize());
	//packet.setStreamBuf(streamBuf);
	//streamBuf->_writeInt(playerid);
	//streamBuf->write(stream->getBuf(),stream->getBufSize());
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crQueryGameRoleListPacket::buildReplyPacket2(crQueryGameRoleListPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQueryGameRoleListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crQueryGameRoleListPacket packet;
				crQueryGameRoleListPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			int gamedbid = playerData->getGameDBID();
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
			if(gamedb)
			{
                ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crGameRoleQueryData> gameRoleQuery = new crGameRoleQueryData;
				gameRoleQuery->buildQuerySql(playerid);
				gameSession->executeQuery(gameRoleQuery.get());
				crDataBase::QueryResultVec gameRoleQueryResultVec = gameSession->getQueryResult();
				gameSession->releaseQuery();
				ref_ptr<crRole> role;
				CRCore::ref_ptr<crItemChild> itemChild;
				std::vector< CRCore::ref_ptr<crRole> > RoleList;
				for( crDataBase::QueryResultVec::iterator itr = gameRoleQueryResultVec.begin();
					 itr != gameRoleQueryResultVec.end();
					 ++itr )
				{
                    gameRoleQuery = dynamic_cast<crGameRoleQueryData *>(itr->get());
                    
					ref_ptr<crInstanceItem2QueryData> instanceItem2Query = new crInstanceItem2QueryData;
					instanceItem2Query->buildQuerySql(gameRoleQuery->getInstanceItemID());
					gameSession->executeQuery(instanceItem2Query.get());
					crDataBase::QueryResultVec& instanceItem2QueryResultVec = gameSession->getQueryResult();
					if(!instanceItem2QueryResultVec.empty())
					{
                        instanceItem2Query = dynamic_cast<crInstanceItem2QueryData *>(instanceItem2QueryResultVec[0].get());
						if(playerData->getPlayerGameData()->getMainRoleID() == gameRoleQuery->getID())
							continue;
                        role = new crRole;
						role->setRoleID(gameRoleQuery->getID());
						role->setPlayerID(playerid);
						role->setInstanceItemID(gameRoleQuery->getInstanceItemID());
						crAbstractMetier *ametier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(gameRoleQuery->getMetierID());
						role->setAbstractMetier(ametier);

						if(role->getInstanceItemID()>0)
						{
							int abstractid = 0;
							ref_ptr<crInstanceItemQueryData> instanceItemQuery = new crInstanceItemQueryData;
							instanceItemQuery->buildQuerySql(role->getInstanceItemID());
							gameSession->executeQuery(instanceItemQuery.get());
							crDataBase::QueryResultVec &instanceItemQueryResultVec = gameSession->getQueryResult();
							if(!instanceItemQueryResultVec.empty())
							{
								instanceItemQuery = dynamic_cast<crInstanceItemQueryData*>(instanceItemQueryResultVec[0].get());
								abstractid = instanceItemQuery->getAbstractItemID();
								role->setItemtype(instanceItemQuery->getItemtype());
								role->setIName(instanceItemQuery->getIName());
								role->setSceneID(instanceItemQuery->getSceneID());
								role->setLayerID(instanceItemQuery->getLayerid());
								role->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
								role->setZoffset(instanceItemQuery->getZoffset());
								role->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
								role->setStream(instanceItemQuery->getStream());
							}
							gameSession->releaseQuery();

							if(abstractid>0)
							{
								crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
								role->setAbstractItem(aitem);
							}

							//itemchild查询
							ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
							itemChildQuery->buildQuerySql(role->getInstanceItemID());
							gameSession->executeQuery(itemChildQuery.get());
							crDataBase::QueryResultVec &itemChildQueryResultVec = gameSession->getQueryResult();
							if(!itemChildQueryResultVec.empty())
							{
								for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
									citr != itemChildQueryResultVec.end();
									++citr )
								{
									itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
									itemChild = new crItemChild;
									itemChild->setItemChildID(itemChildQuery->getID());
									itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
									itemChild->setParentItem(role.get());
									role->insertChildItem(itemChild.get());
								}
							}
							gameSession->releaseQuery();

							//ChildItem查询
							role->childItemQuery(gameSession.get());
						}
						RoleList.push_back(role.get());
					}
					gameSession->releaseQuery();
				}
				gamedb->endSession(gameSession.get());

				/////write and send
				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
				stream->_writeUChar(RoleList.size());
				for( std::vector< CRCore::ref_ptr<crRole> >::iterator itr = RoleList.begin();
					itr != RoleList.end();
					++itr )
				{
					writeItem(itr->get(),stream,BufVec,count,NetStream);
				}
				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crQueryGameRoleListPacket packet;
					crQueryGameRoleListPacket::buildReplyPacket(packet,playerid,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,crQueryGameRoleListPacket::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crQueryGameRoleListPacket packet;
				crQueryGameRoleListPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		if(count>0)
			crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crQueryGameRoleListPacket::ID,count);
	}
}
/////////////////////////////////////////
//
//crCreateRolePacket
//
/////////////////////////////////////////
crCreateRolePacket::crCreateRolePacket():
crStreamPacket(crCreateRolePacket::ID)
{
}

crCreateRolePacket::crCreateRolePacket( const crCreateRolePacket &p ):
crStreamPacket(crCreateRolePacket::ID,p)
{
}

void crCreateRolePacket::buildRequestPacket(crCreateRolePacket &packet,int abstractitemid, int abstractmetierid, std::string &rolename)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(rolename.size()>20)
		rolename = rolename.substr(0,20);
	streamBuf->createBuf(12+rolename.size());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(abstractitemid);
    streamBuf->_writeInt(abstractmetierid);
	streamBuf->_writeString(rolename);
}

void crCreateRolePacket::buildRequestPacket2(crCreateRolePacket &packet,int playerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateRolePacket::buildReplyPacket(crCreateRolePacket &packet,int playerid,int streamid,int count)
{
	//crStreamBuf *streamBuf = new crStreamBuf;
	//if(stream)
	//{
	//    streamBuf->createBuf(5+stream->getBufSize());
	//	streamBuf->_writeInt(playerid);
	//	streamBuf->_writeBool(true);
	//	streamBuf->write(stream->getBuf(),stream->getBufSize());
	//}
	//else
	//{
 //       streamBuf->createBuf(5);
	//	streamBuf->_writeInt(playerid);
	//    streamBuf->_writeBool(false);
	//}
	//packet.setStreamBuf(streamBuf);
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crCreateRolePacket::buildReplyPacket2(crCreateRolePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateRolePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crCreateRolePacket packet;
				crCreateRolePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateRolePacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{//需要加入rolecount限制
		int playerid = m_streamBuf->_readInt();
        int abstractitemid = m_streamBuf->_readInt();
		int abstractmetierid = m_streamBuf->_readInt();
		std::string rolename = m_streamBuf->_readString();
		if(rolename.size()>20)
			rolename = rolename.substr(0,20);
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		bool sucess = false;
		if(callback && playerData.valid())
		{
			int gameid = callback->getGameID();
			unsigned char rolecount = 0;
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();
			ref_ptr<crGameRoleCountQueryData> gameRoleCountQuery = new crGameRoleCountQueryData;
			gameRoleCountQuery->buildQuerySql(gameid);
			globalSession->executeQuery(gameRoleCountQuery.get());
			crDataBase::QueryResultVec &gameRoleCountQueryResultVec = globalSession->getQueryResult();
			if(!gameRoleCountQueryResultVec.empty())
			{
				gameRoleCountQuery = dynamic_cast<crGameRoleCountQueryData *>(gameRoleCountQueryResultVec[0].get());
				rolecount= gameRoleCountQuery->getRoleCount();
			}
			globalSession->releaseQuery();
			db->endSession(globalSession.get());
            
			if(rolecount>0)
			{//crRoleCountQueryData
				crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
				if(gamedb)
				{
					bool game_ended = false;
					int myRoleCount = 0;
					ref_ptr<crDataBase> gameSession = gamedb->beginSession();
					ref_ptr<crRoleCountQueryData> roleCountQuery = new crRoleCountQueryData;
					roleCountQuery->buildQuerySql(playerid);
					gameSession->executeQuery(roleCountQuery.get());
					crDataBase::QueryResultVec &roleCountQueryResultVec = gameSession->getQueryResult();
					if(!roleCountQueryResultVec.empty())
					{
						roleCountQuery = dynamic_cast<crRoleCountQueryData *>(roleCountQueryResultVec[0].get());
						myRoleCount= roleCountQuery->getCount();
					}
					gameSession->releaseQuery();
					if(myRoleCount<(int)rolecount)
					{
						crAbstractMetier *ametier = NULL;
						if(abstractmetierid>0) ametier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(abstractmetierid);
						ref_ptr<crCreateInstanceItem> createInstanceItem = new crCreateInstanceItem;
						if(ametier)
							createInstanceItem->buildUpdateSql(abstractitemid,crInstanceItem::Role,rolename,ametier->getInitSceneID(),ametier->getInitLayerID(),0,0,ametier->getZoffset());
						else if(rolename.empty())
							createInstanceItem->buildUpdateSql(abstractitemid,crInstanceItem::Role);
						else
							createInstanceItem->buildUpdateSql(abstractitemid,crInstanceItem::Role,rolename);

						if(gameSession->executeUpdate(createInstanceItem.get()))
						{//插入成功
							int instanceitemid = gameSession->getInsertID();
							ref_ptr<crCreateRole> createRole = new crCreateRole;
							createRole->buildUpdateSql(playerid,instanceitemid,abstractmetierid/*,metierStream.get()*/);
							if(gameSession->executeUpdate(createRole.get()))
							{
								int roleid = gameSession->getInsertID();
								gameSession->commit();

								gamedb->endSession(gameSession.get());
								game_ended = true;

								ref_ptr<crRole> role = new crRole;
								role->setItemtype(crInstanceItem::Role);
								role->setRoleID(roleid);
								role->setPlayerID(playerid);
								role->setInstanceItemID(instanceitemid);
								//role->setAbstractItemID(abstractitemid);
								role->setAbstractMetier(ametier);
								role->setIName(rolename);
								if(ametier)
								{
									role->setSceneID(ametier->getInitSceneID());
									role->setLayerID(ametier->getInitLayerID());
									role->setZoffset(ametier->getZoffset());
								}

								crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractitemid);
								role->setAbstractItem(aitem);

								/////write and send
								int count = 0;
								std::vector< ref_ptr<crStreamBuf> > BufVec;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

								writeItem(role.get(),stream,BufVec,count,NetStream);
								if(stream->getBufSize()>0)
								{
									count += stream->getBufSize();
									BufVec.push_back(stream.get());
								}
								if(count>0)
								{
									sucess = true;
									int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
									crCreateRolePacket packet;
									crCreateRolePacket::buildReplyPacket(packet,playerid,streamid,count);
									m_netConductor->getNetManager()->sendPacket(sender,packet);
									crDataStreamPacket streamPacket;
									for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
											itr != BufVec.end();
											++itr )
									{
										crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,crCreateRolePacket::ID,itr->get());
										m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
									}
									crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
								}
							}
							else
							{
								gameSession->rollback();

								gamedb->endSession(gameSession.get());
								game_ended = true;
							}
						}
					}
					if(!game_ended) gamedb->endSession(gameSession.get());
                }
			}
		}
		if(!sucess)
		{
			crCreateRolePacket packet;
			crCreateRolePacket::buildReplyPacket(packet,playerid,0,0);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crCreateRolePacket packet;
				crCreateRolePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateRolePacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		if(count>0)
			crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crCreateRolePacket::ID,count);
		else
		{
			CRCore::notify(CRCore::ALWAYS)<<"crCreateRolePacket faild"<<std::endl;
			crGlobalHandle::getInstance()->doEvent(WCH_RecvCreateRole,MAKEINT64(0,NULL));
		}
	}
}
///////////////////////////////////////////
////
////crQuerySceneInfoPacket
////
///////////////////////////////////////////
//crQuerySceneInfoPacket::crQuerySceneInfoPacket():
//crStreamPacket(crQuerySceneInfoPacket::ID)
//{
//}
//
//crQuerySceneInfoPacket::crQuerySceneInfoPacket( const crQuerySceneInfoPacket &p ):
//crStreamPacket(crQuerySceneInfoPacket::ID,p)
//{
//}
//
//void crQuerySceneInfoPacket::buildRequestPacket(crQuerySceneInfoPacket &packet,int sceneid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(sceneid);
//}
//
//void crQuerySceneInfoPacket::buildRequestPacket2(crQuerySceneInfoPacket &packet,int playerid, CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crQuerySceneInfoPacket::buildReplyPacket(crQuerySceneInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+stream->getBufSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->write(stream->getBuf(),stream->getBufSize());
//}
//
//void crQuerySceneInfoPacket::buildReplyPacket2(crQuerySceneInfoPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crQuerySceneInfoPacket::parsePacket(const std::string &sender)
//{//
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//	if(netType == GameConnectServer)
//	{//
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *conductor = playerData->getGameServerLineConductor();
//			if(conductor)
//			{
//				crQuerySceneInfoPacket packet;
//				crQuerySceneInfoPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
//				conductor->getNetManager()->sendPacket("all",packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket "<<std::endl;
//			}
//		}
//	}
//	else if(netType == GameServer)
//	{
//		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket "<<std::endl;
//
//		int playerid = m_streamBuf->_readInt();
//		int sceneid = m_streamBuf->_readInt();
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
//		if(playerData.valid())
//		{
//			crDataBase *resdb = crDataBaseManager::getInstance()->getDataBase(ResDB);
//			if(resdb)
//			{
//				ref_ptr<crDataBase> sceneSession = resdb->beginSession();
//				ref_ptr<crSceneInfoQueryData> sceneInfoQuery = new crSceneInfoQueryData;
//				sceneInfoQuery->buildQuerySql(sceneid);
//				sceneSession->executeQuery(sceneInfoQuery.get());
//				crDataBase::QueryResultVec sceneInfoQueryResultVec = sceneSession->getQueryResult();
//				sceneSession->releaseQuery();
//				resdb->endSession(sceneSession.get());
//				if(!sceneInfoQueryResultVec.empty())
//				{
//					ref_ptr<crStreamBuf> stream = new crStreamBuf;
//					stream->createBuf(136);
//                    sceneInfoQuery = dynamic_cast<crSceneInfoQueryData *>(sceneInfoQueryResultVec[0].get());
//					stream->_writeInt(sceneid);//4
//					stream->_writeString(sceneInfoQuery->getSceneName());//20+4
//					//stream->_writeString(sceneInfoQuery->getFileName());//50+4
//					////stream->_writeString(sceneInfoQuery->getUpdatecfg());//50+4
//					stream->_writeString(sceneInfoQuery->getCollideFile());//50+4
//     //               //file modify time
//					////char mtime[20];
//					//CRCore::Timer_t mtime = 0L;
//					//struct stat buf;
//					//std::string cookfilename = sceneInfoQuery->getFileName();
//					//if(!cookfilename.empty())
//					//{
//					//	CRIOManager::GetCookedFileName(cookfilename);
//					//	if (stat( cookfilename.c_str(), &buf )) 
//					//	{
//					//		CRCore::notify(CRCore::FATAL)<<cookfilename<<" 文件不存在"<<std::endl;
//					//	}
//					//	else
//					//	{
//					//		mtime = buf.st_mtime;
//					//	}
//					//}
//					////stream->_writeString(mtime);//20+4
//					//stream->_writeInt64(mtime);
//     //               //updatecfg modify time
//					//mtime = 0L;
//					//cookfilename = crScene::getUpdatecfg(sceneInfoQuery->getFileName());//sceneInfoQuery->getUpdatecfg();
//					//if(!cookfilename.empty())
//					//{
//					//	CRIOManager::GetCookedFileName(cookfilename);
//					//	if (stat( cookfilename.c_str(), &buf )) 
//					//	{
//					//		CRCore::notify(CRCore::FATAL)<<cookfilename<<" 文件不存在"<<std::endl;
//					//	}
//					//	else
//					//	{
//					//		mtime = buf.st_mtime;
//					//	}
//					//}
//					////stream->_writeString(mtime);//20+4
//					//stream->_writeInt64(mtime);
//					////collidefile modify time
//					//mtime = 0L;
//					//cookfilename = crScene::getCollideFile(sceneInfoQuery->getFileName());//sceneInfoQuery->getCollideFile();
//					//if(!cookfilename.empty())
//					//{
//					//	CRIOManager::GetCookedFileName(cookfilename);
//					//	if (stat( cookfilename.c_str(), &buf )) 
//					//	{
//					//		CRCore::notify(CRCore::FATAL)<<cookfilename<<" 文件不存在"<<std::endl;
//					//	}
//					//	else
//					//	{
//					//		mtime = buf.st_mtime;
//					//	}
//					//}
//					////stream->_writeString(mtime);//20+4
//					//stream->_writeInt64(mtime);
//
//					crQuerySceneInfoPacket packet;
//					crQuerySceneInfoPacket::buildReplyPacket(packet,playerid,stream.get());
//					m_netConductor->getNetManager()->sendPacket(sender,packet);
//				}
//			}
//		}
//	}
//	else if(netType == GameConnectServerClient_Game)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData.valid())
//			{
//				crQuerySceneInfoPacket packet;
//				crQuerySceneInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket"<<std::endl;
//			}
//		}
//	}
//	else if(netType == GameClient_Game)
//	{//
//	    ref_ptr<crQuerySceneInfoData> querySceneInfoData = new crQuerySceneInfoData;
//		querySceneInfoData->setSceneID(m_streamBuf->_readInt());
//		querySceneInfoData->setSceneName(m_streamBuf->_readString());
//		querySceneInfoData->setFileName(m_streamBuf->_readString());
//		//querySceneInfoData->setUpdatecfg(m_streamBuf->_readString());
//		querySceneInfoData->setCollideFile(m_streamBuf->_readString());
//		//querySceneInfoData->setFileModifyTime(m_streamBuf->_readInt64());
//		//querySceneInfoData->setUpdcfgModifyTime(m_streamBuf->_readInt64());
//		//querySceneInfoData->setCollideFileModifyTime(m_streamBuf->_readInt64());
//        crMyPlayerData::getInstance()->setSceneInfoData(querySceneInfoData.get());
//		crGlobalHandle::getInstance()->doEvent(WCH_RecvSceneInfo,MAKEINT64(NULL,NULL));
//	}
//}
/////////////////////////////////////////
//
//crSceneServerInitPacket
//
/////////////////////////////////////////
crSceneServerInitPacket::crSceneServerInitPacket():
crStreamPacket(crSceneServerInitPacket::ID)
{
}

crSceneServerInitPacket::crSceneServerInitPacket( const crSceneServerInitPacket &p ):
crStreamPacket(crSceneServerInitPacket::ID,p)
{
}

void crSceneServerInitPacket::buildRequestPacket(crSceneServerInitPacket &packet,const std::string &sceneName,const std::string &serverName)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(sceneName.length()+serverName.length()+8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(sceneName);
	streamBuf->_writeString(serverName);
}

void crSceneServerInitPacket::buildReplyPacket(crSceneServerInitPacket &packet,CRCore::crStreamBuf *stream)
{
	stream->seekBegin();
	packet.setStreamBuf(stream);
}

void crSceneServerInitPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
    if(netType == GameServer)
	{
		std::string sceneName = m_streamBuf->_readString();
		std::string serverName = m_streamBuf->_readString();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = netCallback->getGameID();
		const std::string& gameName = netCallback->getGameName();
		const std::string& gameServerName = netCallback->getServerName();
		crDataBase *scenedb = crDataBaseManager::getInstance()->getDataBase(ResDB);
		if(scenedb)
		{
			ref_ptr<crDataBase> sceneSession = scenedb->beginSession();
			ref_ptr<crSceneIDQueryData> sceneIDQuery = new crSceneIDQueryData;
			sceneIDQuery->buildQuerySql(gameid,sceneName);
			sceneSession->executeQuery(sceneIDQuery.get());
			crDataBase::QueryResultVec sceneIDQueryResultVec = sceneSession->getQueryResult();
			sceneSession->releaseQuery();
			scenedb->endSession(sceneSession.get());
			if(!sceneIDQueryResultVec.empty())
			{
				int sceneid = sceneIDQuery->getID();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(68);
				sceneIDQuery = dynamic_cast<crSceneIDQueryData *>(sceneIDQueryResultVec[0].get());
				stream->_writeInt(gameid);//4
				stream->_writeString(gameName);//16+4
				stream->_writeString(gameServerName);//16+4
				stream->_writeInt(sceneid);//4
				//stream->_writeString(sceneCollideFileQuery->getFileName());//50+4
				stream->_writeString(sceneName);//16+4
				//stream->_writeString(sceneCollideFileQuery->getCollideFile());//50+4

				netCallback->registerSceneServer(sceneid,serverName,sender);
				crSceneServerInitPacket packet;
				crSceneServerInitPacket::buildReplyPacket(packet,stream.get());
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
			else
			{
				CRCore::notify(CRCore::FATAL)<<"crSceneServerInitPacket "<<sceneName<<" 没有在场景数据库里找到，将导致该场景服务器无法完成启动!"<<std::endl;
			}
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(sceneServerConductor)
		{
			CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			//crScene *scene = netCallback->getScene();
			crScene *scene = new crScene;
			scene->setGameID(m_streamBuf->_readInt());
			std::string gameName = m_streamBuf->_readString();
			std::string gameServerName = m_streamBuf->_readString();
			netCallback->setGameName(gameName);
			netCallback->setServerName(gameServerName);
			scene->setGameName(gameName);
			scene->setSceneID(m_streamBuf->_readInt());
			//scene->setFileName(m_streamBuf->_readString());
			scene->setSceneName(m_streamBuf->_readString());
			//scene->setCollideFile(m_streamBuf->_readString());

			ref_ptr<crTableIO>sceneTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SceneTab);
			if(sceneTab.valid())
			{
				crTableIO::StrVec record;
				if(sceneTab->queryOneRecord(0,crArgumentParser::appItoa(scene->getSceneID()),record)>=0)
				{
					int filenameindex = sceneTab->getTitleIndex("filename");
					int collidefileindex = sceneTab->getTitleIndex("collidefile");
					int brainfileindex = sceneTab->getTitleIndex("brainfile");
					int extindex = sceneTab->getTitleIndex("exttype");

					scene->setFileName(record[filenameindex]);
					scene->setCollideFile(record[collidefileindex]);
					scene->setBrainFile(record[brainfileindex]);
					if(extindex>0) scene->setExternSceneType(atoi(record[extindex].c_str()));

					scene->buildSceneLayer();
					netCallback->addScene(scene);

					crQuerySceneItemPacket packet;
					crQuerySceneItemPacket::buildRequestPacket(packet,scene->getSceneID());
					m_netConductor->getNetManager()->sendPacket("all",packet);
					CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
					CRCore::notify(CRCore::ALWAYS)<<"crSceneServerInitPacket sceneid:"<<scene->getSceneID()<<" scenename:"<<scene->getSceneName()<<" 耗时"<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<"s"<<std::endl;
				}
				else
				{
					CRCore::notify(CRCore::ALWAYS)<<"crSceneServerInitPacket sceneid:"<<scene->getSceneID()<<" scenename:"<<scene->getSceneName()<<" 在SceneTab上没有找到，该场景无法加载"<<std::endl;
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRegisterSceneServerPacket
//
/////////////////////////////////////////
crRegisterSceneServerPacket::crRegisterSceneServerPacket():
	crStreamPacket(crRegisterSceneServerPacket::ID)
{
}

crRegisterSceneServerPacket::crRegisterSceneServerPacket( const crRegisterSceneServerPacket &p ):
	crStreamPacket(crRegisterSceneServerPacket::ID,p)
{
}

void crRegisterSceneServerPacket::buildRequestPacket(crRegisterSceneServerPacket &packet,int sceneid,const std::string &serverName)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(serverName.length()+8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(sceneid);
	streamBuf->_writeString(serverName);
}

void crRegisterSceneServerPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int sceneid = m_streamBuf->_readInt();
		std::string serverName = m_streamBuf->_readString();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		netCallback->registerSceneServer(sceneid,serverName,sender);
	}
}
/////////////////////////////////////////
//
//crSceneServerInitEndPacket
//
/////////////////////////////////////////
crSceneServerInitEndPacket::crSceneServerInitEndPacket():
	crStreamPacket(crSceneServerInitEndPacket::ID)
{
}

crSceneServerInitEndPacket::crSceneServerInitEndPacket( const crSceneServerInitEndPacket &p ):
	crStreamPacket(crSceneServerInitEndPacket::ID,p)
{
}

void crSceneServerInitEndPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		m_netConductor->getNetManager()->sendPacket(sender,*this);
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		netCallback->initend();
	}
}
///////////////////////////////////////////
////
////itemQuery
////
///////////////////////////////////////////
//void CRNetApp::itemQuery(CRCore::ref_ptr<crInstanceItem>&item,crDataBase *session_instance,crDataBase *session_res)
//{
//
//}
///////////////////////////////////////////
////
////childItemQuery
////
///////////////////////////////////////////
//void CRNetApp::childItemQuery(crInstanceItem *parentItem,crDataBase *instanceSession,crDataBase *resSession)
//{
//	parentItem->lockChildItemMap();
//	crInstanceItem::ChildItemMap &childItemMap = parentItem->getChildItemMap();
//	ref_ptr<crInstanceItemQueryData> instanceItemQuery;
//	CRCore::ref_ptr<crItemChild> itemChild;
//	CRCore::ref_ptr<crInstanceItem> instanceItem;
//	for( crInstanceItem::ChildItemMap::iterator itr = childItemMap.begin();
//		itr != childItemMap.end();
//		++itr )
//	{
//		instanceItem = NULL;
//		instanceItemQuery = new crInstanceItemQueryData;
//		instanceItemQuery->buildQuerySql(itr->second->getInstanceItemID());
//		instanceSession->executeQuery(instanceItemQuery.get());
//		crDataBase::QueryResultVec &instanceItemQueryResultVec = instanceSession->getQueryResult();
//		if(!instanceItemQueryResultVec.empty())
//		{
//			instanceItemQuery = dynamic_cast<crInstanceItemQueryData *>(instanceItemQueryResultVec[0].get());
//			instanceItem = new crInstanceItem;
//			instanceItem->setInstanceItemID(itr->second->getInstanceItemID());
//			instanceItem->setAbstractItemID(instanceItemQuery->getAbstractItemID());
//			instanceItem->setItemtype(instanceItemQuery->getItemtype());
//			instanceItem->setIName(instanceItemQuery->getIName());
//			instanceItem->setSceneID(instanceItemQuery->getSceneID());
//			instanceItem->setLayerID(instanceItemQuery->getLayerid());
//			instanceItem->setPosx(instanceItemQuery->getPosx());
//			instanceItem->setPosy(instanceItemQuery->getPosy());
//			instanceItem->setZoffset(instanceItemQuery->getZoffset());
//			instanceItem->setDirx(instanceItemQuery->getDirx());
//			instanceItem->setDiry(instanceItemQuery->getDiry());
//			instanceItem->setDirz(instanceItemQuery->getDirz());
//			instanceItem->setStream(instanceItemQuery->getStream());
//			itr->second->setInstanceItem(instanceItem.get());
//		}
//		instanceSession->releaseQuery();
//
//		if(instanceItem.valid())
//		{
//			//crAbstractItemQueryData
//			ref_ptr<crAbstractItemQueryData> abstractItemQuery = new crAbstractItemQueryData;
//			abstractItemQuery->buildQuerySql(instanceItem->getAbstractItemID());
//			resSession->executeQuery(abstractItemQuery.get());
//			crDataBase::QueryResultVec &abstractItemQueryResultVec = resSession->getQueryResult();
//			if(!abstractItemQueryResultVec.empty())
//			{
//				abstractItemQuery = dynamic_cast<crAbstractItemQueryData*>(abstractItemQueryResultVec[0].get());
//				instanceItem->setResID(abstractItemQuery->getResID());
//				instanceItem->setName(abstractItemQuery->getName());
//				instanceItem->setScript(abstractItemQuery->getScript());
//			}
//			resSession->releaseQuery();
//
//			//crResQueryData
//			ref_ptr<crResQueryData> resQuery = new crResQueryData;
//			resQuery->buildQuerySql(instanceItem->getResID());
//			resSession->executeQuery(resQuery.get());
//			crDataBase::QueryResultVec &resQueryResultVec = resSession->getQueryResult();
//			if(!resQueryResultVec.empty())
//			{
//				resQuery = dynamic_cast<crResQueryData*>(resQueryResultVec[0].get());
//				instanceItem->setFileName(resQuery->getFileName());
//				instanceItem->setUpdatecfg(resQuery->getUpdateCfg());
//				instanceItem->setResScript(resQuery->getScript());
//			}
//			resSession->releaseQuery();
//
//			//itemchild查询
//			ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
//			itemChildQuery->buildQuerySql(instanceItem->getInstanceItemID());
//			instanceSession->executeQuery(itemChildQuery.get());
//			crDataBase::QueryResultVec &itemChildQueryResultVec = instanceSession->getQueryResult();
//			if(!itemChildQueryResultVec.empty())
//			{
//				for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
//					citr != itemChildQueryResultVec.end();
//					++citr )
//				{
//					itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
//					itemChild = new crItemChild;
//					itemChild->setItemChildID(itemChildQuery->getID());
//					itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
//					itemChild->setParentItem(instanceItem.get());
//					instanceItem->insertChildItem(itemChild.get());
//				}
//			}
//			instanceSession->releaseQuery();
//			//ChildItem查询
//			childItemQuery(instanceItem.get(),instanceSession,resSession);
//		}
//	}
//	parentItem->unlockChildItemMap();
//}
/////////////////////////////////////////
//
//writeItem
//
/////////////////////////////////////////
void CRNetApp::writeItem(crInstanceItem *item,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count,char streamType)
{
	unsigned char itemtype;
	crStreamBuf* itemStream;
	bool streamwrited;
	//char mtime[20];
	//CRCore::Timer_t mtime = 0L;
	//struct stat buf;
	//std::string cookfilename;
	itemtype = item->getItemtype();
	writeStreamVec(stream,1,BufVec,count);
	stream->_writeUChar(itemtype);
    if(itemtype == crInstanceItem::Role)
	{//write role
		crRole *role = dynamic_cast<crRole *>(item);
		writeStreamVec(stream,12,BufVec,count);
        stream->_writeInt(role->getRoleID());//4
		stream->_writeInt(role->getPlayerID());//4

		itemStream = NULL;
		streamwrited = false;
		if(streamType != SaveStream)
		{
			crData *data = role->getMetierDataClass();
			if(data)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				ref_ptr<crStreamBuf> dataStream = new crStreamBuf;
				data->excHandle(MAKEINT64(WCH_BuildSaveStream+streamType-SaveStream,dataStream.get()));
				//void *param;
				//data->getParam(WCHDATA_DataStream,param);
				//if(param)
				//{
					//ref_ptr<crStreamBuf> metierStream = *((ref_ptr<crStreamBuf>*)param);
					if(dataStream->getBuf()>0)
					{
						//itemStream = metierStream.get();
						stream->_writeInt(dataStream->getBufSize());
						writeLongStream(stream,dataStream.get(),BufVec,count);
						streamwrited = true;
					}
				//}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else
		{
			itemStream = role->getMetierStream();
			if(itemStream && itemStream->getBufSize() > 0)
			{
				stream->_writeInt(itemStream->getBufSize());
				writeLongStream(stream,itemStream,BufVec,count);
				streamwrited = true;
			}
		}
		if(!streamwrited)
		{
			stream->_writeInt(0);//4
		}
		//else
		//{
		//	stream->_writeInt(itemStream->getBufSize());
		//	writeLongStream(stream,itemStream.get(),BufVec,count);
		//	//stream->write(itemStream->getBuf(),itemStream->getBufSize());
		//}

		//write abstractmetier
		crAbstractMetier *ametier = role->getAbstractMetier();
		if(ametier)
		{
			writeStreamVec(stream,9,BufVec,count);
			stream->_writeBool(true);
			stream->_writeInt(ametier->getAbstractMetierID());//4
			stream->_writeInt(ametier->getGameID());//4
			writeStreamVec(stream,ametier->getMetierName().size()+4,BufVec,count);
			stream->_writeString(ametier->getMetierName());
			//writeStreamVec(stream,role->getAbstractMetier()->getIntroduce().size()+4,BufVec,count);
			//stream->_writeString(role->getAbstractMetier()->getIntroduce());
			writeStreamVec(stream,4,BufVec,count);
			stream->_writeInt(ametier->getInitSceneID());//4
			writeStreamVec(stream,ametier->getMetierScript().size()+4,BufVec,count);
			stream->_writeString(ametier->getMetierScript());
			//MetierScript modify time
			//mtime = role->getAbstractMetier()->getMetierScriptModifyTime();
			//if(mtime == 0L)
			//{
			//	cookfilename = role->getAbstractMetier()->getMetierScript();
			//	if(!cookfilename.empty())
			//	{
			//		CRIOManager::GetCookedFileName(cookfilename);
			//		if (stat( cookfilename.c_str(), &buf )) 
			//		{
			//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
			//		}
			//		else
			//		{
			//			mtime = buf.st_mtime;
			//			role->getAbstractMetier()->setMetierScriptModifyTime(mtime);
			//		}
			//	}
			//}
			////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
			////stream->_writeString(mtime);//20+4
			//writeStreamVec(stream,8,BufVec,count);
			//stream->_writeInt64(mtime);//8

			writeStreamVec(stream,ametier->getTaskTable().size()+4,BufVec,count);
			stream->_writeString(ametier->getTaskTable());
			//TaskTable modify time
			//mtime = role->getAbstractMetier()->getTaskTableModifyTime();
			//if(mtime == 0L)
			//{
			//	cookfilename = role->getAbstractMetier()->getTaskTable();
			//	if(!cookfilename.empty())
			//	{
			//		CRIOManager::GetCookedFileName(cookfilename);
			//		if (stat( cookfilename.c_str(), &buf )) 
			//		{
			//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
			//		}
			//		else
			//		{
			//			mtime = buf.st_mtime;
			//			role->getAbstractMetier()->setTaskTableModifyTime(mtime);
			//		}
			//	}
			//}
			////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
			////stream->_writeString(mtime);//20+4
			//writeStreamVec(stream,8,BufVec,count);
			//stream->_writeInt64(mtime);//8
		}
		else
		{
			writeStreamVec(stream,1,BufVec,count);
			stream->_writeBool(false);
		}
	}
	writeStreamVec(stream,4,BufVec,count);
	stream->_writeInt(item->getInstanceItemID());//4
    if(item->getInstanceItemID()!=0)
	{
		if(itemtype == crInstanceItem::Emporium)
		{
			writeStreamVec(stream,item->getAbstractItem()->getScript().size()+4,BufVec,count);
			stream->_writeString(item->getAbstractItem()->getScript());
			//writeStreamVec(stream,8,BufVec,count);
			//stream->_writeInt64(item->getAbstractItem()->getScriptModifyTime());//8
			writeStreamVec(stream,4,BufVec,count);
			stream->_writeInt(item->getSceneID());//4
		}
		else
		{	//write abstractitem
			crAbstractItem *aitem = item->getAbstractItem();
			if(aitem)
			{
				writeStreamVec(stream,5,BufVec,count);
				stream->_writeBool(true);
				stream->_writeInt(aitem->getAbstractItemID());//4
				writeStreamVec(stream,aitem->getName().size()+4,BufVec,count);
				stream->_writeString(aitem->getName());//4
				writeStreamVec(stream,aitem->getScript().size()+4,BufVec,count);
				stream->_writeString(aitem->getScript());//4
				//script modify time
				//mtime = aitem->getScriptModifyTime();
				//if(mtime == 0L)
				//{
				//	cookfilename = aitem->getScript();
				//	if(!cookfilename.empty())
				//	{
				//		CRIOManager::GetCookedFileName(cookfilename);
				//		if (stat( cookfilename.c_str(), &buf )) 
				//		{
				//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
				//		}
				//		else
				//		{
				//			mtime = buf.st_mtime;
				//			aitem->setScriptModifyTime(mtime);
				//		}
				//	}
				//}
				////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
				////stream->_writeString(mtime);//20+4
				//writeStreamVec(stream,8,BufVec,count);
				//stream->_writeInt64(mtime);//8

				//write res
				writeStreamVec(stream,4,BufVec,count);
				stream->_writeInt(aitem->getResID());//4
				writeStreamVec(stream,aitem->getFileName().size()+4,BufVec,count);
				stream->_writeString(aitem->getFileName());
				//writeStreamVec(stream,item->getAbstractItem()->getUpdatecfg().size()+4,BufVec,count);
				//stream->_writeString(item->getAbstractItem()->getUpdatecfg());
				//writeStreamVec(stream,item->getPicture().size()+4,BufVec,count);
				//stream->_writeString(item->getPicture());
				writeStreamVec(stream,aitem->getResScript().size()+4,BufVec,count);
				stream->_writeString(aitem->getResScript());
				//file modify time
				//mtime = aitem->getFileModifyTime();
				//if(mtime == 0L)
				//{
				//	cookfilename = aitem->getFileName();
				//	if(!cookfilename.empty())
				//	{
				//		CRIOManager::GetCookedFileName(cookfilename);
				//		if (stat( cookfilename.c_str(), &buf )) 
				//		{
				//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
				//		}
				//		else
				//		{
				//			mtime = buf.st_mtime;
				//			aitem->setFileModifyTime(mtime);
				//		}
				//	}
				//}
				////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
				////stream->_writeString(mtime);//20+4
				//writeStreamVec(stream,8,BufVec,count);
				//stream->_writeInt64(mtime);//8
				//updatecfg modify time
				//mtime = aitem->getUpdcfgModifyTime();
				//if(mtime == 0L)
				//{
				//	cookfilename = aitem->getUpdatecfg();
				//	if(!cookfilename.empty())
				//	{
				//		CRIOManager::GetCookedFileName(cookfilename);
				//		if (stat( cookfilename.c_str(), &buf )) 
				//		{
				//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
				//		}
				//		else
				//		{
				//			mtime = buf.st_mtime;
				//			aitem->setUpdcfgModifyTime(mtime);
				//		}
				//	}
				//}
				////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
				////stream->_writeString(mtime);//20+4
				//writeStreamVec(stream,8,BufVec,count);
				//stream->_writeInt64(mtime);//8

				//script modify time
				//mtime = aitem->getResScriptModifyTime();
				//if(mtime == 0L)
				//{
				//	cookfilename = aitem->getResScript();
				//	if(!cookfilename.empty())
				//	{
				//		CRIOManager::GetCookedFileName(cookfilename);
				//		if (stat( cookfilename.c_str(), &buf )) 
				//		{
				//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
				//		}
				//		else
				//		{
				//			mtime = buf.st_mtime;
				//			aitem->setResScriptModifyTime(mtime);
				//		}
				//	}
				//}
				////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
				////stream->_writeString(mtime);//20+4
				//writeStreamVec(stream,8,BufVec,count);
				//stream->_writeInt64(mtime);//8
			}
			else
			{
				writeStreamVec(stream,1,BufVec,count);
				stream->_writeBool(false);
			}
			//write instanceitem
			writeStreamVec(stream,4,BufVec,count);
			stream->_writeInt(item->getRoomID());//4
			writeStreamVec(stream,item->getIName2().size()+4,BufVec,count);
			stream->_writeString(item->getIName2());
			writeStreamVec(stream,32,BufVec,count);
			stream->_writeInt(item->getSceneID());//4
			stream->_writeUShort(item->getLayerID());//2
			stream->_writeFloat(item->getPosx());//4
			stream->_writeFloat(item->getPosy());//4
			stream->_writeFloat(item->getPosz());//4
			stream->_writeShort(item->getZoffset());//2
			stream->_writeFloat(item->getDirx());
			stream->_writeFloat(item->getDiry());
			stream->_writeFloat(item->getDirz());
		}
		writeStreamVec(stream,4,BufVec,count);
		itemStream = NULL;
		streamwrited = false;
		if(streamType != SaveStream)
		{
			crData *data = item->getDataClass();
			if(data)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				ref_ptr<crStreamBuf> dataStream = new crStreamBuf;
				data->excHandle(MAKEINT64(WCH_BuildSaveStream+streamType-SaveStream,dataStream.get()));
				//void *param;
				//data->getParam(WCHDATA_DataStream,param);
				//if(param)
				//{
				//	ref_ptr<crStreamBuf> datastream = *((ref_ptr<crStreamBuf>*)param);
					if(/*datastream.valid() && */dataStream->getBuf()>0)
					{
						//itemStream = stream.get();
						stream->_writeInt(dataStream->getBufSize());
						writeLongStream(stream,dataStream.get(),BufVec,count);
						streamwrited = true;
					}
				//}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else
		{
			itemStream = item->getStream();
			if(itemStream && itemStream->getBufSize() > 0)
			{
				stream->_writeInt(itemStream->getBufSize());
				writeLongStream(stream,itemStream,BufVec,count);
				streamwrited = true;
			}
		}
		if(!streamwrited)
		{
			stream->_writeInt(0);//4
		}
		//else
		//{
		//	stream->_writeInt(itemStream->getBufSize());
		//	writeLongStream(stream,itemStream,BufVec,count);
		//	//stream->write(itemStream->getBuf(),itemStream->getBufSize());
		//}
		writeChildItem(item,stream,BufVec,count,streamType);
	}
	//if(stream->getBufSize()>0)
	//{
	//	count += stream->getBufSize();
	//	BufVec.push_back(stream.get());
	//	int size = stream->getMaxBufSize();
	//	stream = new crStreamBuf;
	//	stream->createBuf(size);
	//}
}
/////////////////////////////////////////
//
//writeChildItem
//
/////////////////////////////////////////
void CRNetApp::writeChildItem(crInstanceItem *parentItem,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< ref_ptr<crStreamBuf> > &BufVec,int &count,char streamType)
{
	parentItem->lockChildItemMap();
	crInstanceItem::ChildItemMap &childItemMap = parentItem->getChildItemMap();
	crItemChild* itemChild;
	crInstanceItem *instanceItem;
	unsigned char itemtype;
	CRCore::crStreamBuf *itemStream;
	bool streamwrited = false;
	writeStreamVec(stream,4,BufVec,count);
	stream->_writeInt(childItemMap.size());//4
	//char mtime[20];
	//CRCore::Timer_t mtime = 0L;
	//struct stat buf;
 //   std::string cookfilename;
	for( crInstanceItem::ChildItemMap::iterator itr = childItemMap.begin();
		itr != childItemMap.end();
		++itr )
	{
		itemChild = itr->second.get();
		writeStreamVec(stream,5,BufVec,count);
		stream->_writeInt(itemChild->getItemChildID());//4
		//stream->_writeInt(itemChild->getInstanceItemID());//4

		itemtype = itemChild->getInstanceItem()->getItemtype();
		stream->_writeUChar(itemtype);//1
		
		instanceItem = itemChild->getInstanceItem();
		//write abstractitem
		writeStreamVec(stream,4,BufVec,count);
		stream->_writeInt(instanceItem->getAbstractItemID());//4
		writeStreamVec(stream,instanceItem->getAbstractItem()->getName().size()+4,BufVec,count);
		stream->_writeString(instanceItem->getAbstractItem()->getName());//4
		writeStreamVec(stream,instanceItem->getAbstractItem()->getScript().size()+4,BufVec,count);
		stream->_writeString(instanceItem->getAbstractItem()->getScript());//4
		//script modify time
		//mtime = instanceItem->getAbstractItem()->getScriptModifyTime();
		//if(mtime == 0L)
		//{
		//	cookfilename = instanceItem->getAbstractItem()->getScript();
		//	if(!cookfilename.empty())
		//	{
		//		CRIOManager::GetCookedFileName(cookfilename);
		//		if (stat( cookfilename.c_str(), &buf )) 
		//		{
		//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
		//		}
		//		else
		//		{
		//			mtime = buf.st_mtime;
		//			instanceItem->getAbstractItem()->setScriptModifyTime(mtime);
		//		}
		//	}
		//}
		////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
		////stream->_writeString(mtime);//20+4
		//writeStreamVec(stream,8,BufVec,count);
		//stream->_writeInt64(mtime);//8

		//write res
		writeStreamVec(stream,4,BufVec,count);
		stream->_writeInt(instanceItem->getAbstractItem()->getResID());//4
		writeStreamVec(stream,instanceItem->getAbstractItem()->getFileName().size()+4,BufVec,count);
		stream->_writeString(instanceItem->getAbstractItem()->getFileName());
		//writeStreamVec(stream,instanceItem->getAbstractItem()->getUpdatecfg().size()+4,BufVec,count);
		//stream->_writeString(instanceItem->getAbstractItem()->getUpdatecfg());
		//writeStreamVec(stream,instanceItem->getPicture().size()+4,BufVec,count);
		//stream->_writeString(instanceItem->getPicture());
		writeStreamVec(stream,instanceItem->getAbstractItem()->getResScript().size()+4,BufVec,count);
		stream->_writeString(instanceItem->getAbstractItem()->getResScript());
		//file modify time
		//mtime = instanceItem->getAbstractItem()->getFileModifyTime();
		//if(mtime == 0L)
		//{
		//	cookfilename = instanceItem->getAbstractItem()->getFileName();
		//	if(!cookfilename.empty())
		//	{
		//		CRIOManager::GetCookedFileName(cookfilename);
		//		if (stat( cookfilename.c_str(), &buf )) 
		//		{
		//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
		//		}
		//		else
		//		{
		//			mtime = buf.st_mtime;
		//			instanceItem->getAbstractItem()->setFileModifyTime(mtime);
		//		}
		//	}
		//}
		////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
		////stream->_writeString(mtime);//20+4
		//writeStreamVec(stream,8,BufVec,count);
		//stream->_writeInt64(mtime);//8

		//updatecfg modify time
		//mtime = instanceItem->getAbstractItem()->getUpdcfgModifyTime();
		//if(mtime == 0L)
		//{
		//	cookfilename = instanceItem->getAbstractItem()->getUpdatecfg();
		//	if(!cookfilename.empty())
		//	{
		//		CRIOManager::GetCookedFileName(cookfilename);
		//		if (stat( cookfilename.c_str(), &buf )) 
		//		{
		//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
		//		}
		//		else
		//		{
		//			mtime = buf.st_mtime;
		//			instanceItem->getAbstractItem()->setUpdcfgModifyTime(mtime);
		//		}
		//	}
		//}
		////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
		////stream->_writeString(mtime);//20+4
		//writeStreamVec(stream,8,BufVec,count);
		//stream->_writeInt64(mtime);//8

		//script modify time
		//mtime = instanceItem->getAbstractItem()->getResScriptModifyTime();
		//if(mtime == 0L)
		//{
		//	cookfilename = instanceItem->getAbstractItem()->getResScript();
		//	if(!cookfilename.empty())
		//	{
		//		CRIOManager::GetCookedFileName(cookfilename);
		//		if (stat( cookfilename.c_str(), &buf )) 
		//		{
		//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
		//		}
		//		else
		//		{
		//			mtime = buf.st_mtime;
		//			instanceItem->getAbstractItem()->setResScriptModifyTime(mtime);
		//		}
		//	}
		//}
		////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
		////stream->_writeString(mtime);//20+4
		//writeStreamVec(stream,8,BufVec,count);
		//stream->_writeInt64(mtime);//8

		//write InstranceItem
		writeStreamVec(stream,4,BufVec,count);
		stream->_writeInt(instanceItem->getInstanceItemID());//4
		writeStreamVec(stream,instanceItem->getIName2().size()+4,BufVec,count);
		stream->_writeString(instanceItem->getIName2());
		//writeStreamVec(stream,34,BufVec,count);
		//stream->_writeInt(instanceItem->getSceneID());//4
		//stream->_writeUShort(instanceItem->getLayerID());//2
		//stream->_writeInt(instanceItem->getPosx());//4
		//stream->_writeInt(instanceItem->getPosy());//4
		//stream->_writeInt(instanceItem->getPosz());//4
		////stream->_writeShort(instanceItem->getZoffset());//2
		//stream->_writeFloat(instanceItem->getDirx());
		//stream->_writeFloat(instanceItem->getDiry());
		//stream->_writeFloat(instanceItem->getDirz());

		itemStream = NULL;
		streamwrited = false;
		if(streamType != SaveStream)
		{
			crData *data = instanceItem->getDataClass();
			if(data)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				ref_ptr<crStreamBuf> dataStream = new crStreamBuf;
				data->excHandle(MAKEINT64(WCH_BuildSaveStream+streamType-SaveStream,dataStream.get()));
				//void *param;
				//data->getParam(WCHDATA_DataStream,param);
				//if(param)
				//{
				//	ref_ptr<crStreamBuf> datastream = *((ref_ptr<crStreamBuf>*)param);
					if(/*datastream.valid() && */dataStream->getBuf()>0)
					{
						stream->_writeInt(dataStream->getBufSize());
						writeLongStream(stream,dataStream.get(),BufVec,count);
						streamwrited = true;
					}
				//}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else
		{
			itemStream = instanceItem->getStream();
			if(itemStream && itemStream->getBufSize() > 0)
			{
				stream->_writeInt(itemStream->getBufSize());
				writeLongStream(stream,itemStream,BufVec,count);
				streamwrited = true;
			}
		}
		if(!streamwrited)
		{
			stream->_writeInt(0);//4
		}
		else
		{
			stream->_writeInt(itemStream->getBufSize());
			writeLongStream(stream,itemStream,BufVec,count);
			//stream->write(itemStream->getBuf(),itemStream->getBufSize());
		}
		writeChildItem(instanceItem,stream,BufVec,count,streamType);
	}
	parentItem->unlockChildItemMap();
}
/////////////////////////////////////////
//
//writeAbstractItem
//
/////////////////////////////////////////
void CRNetApp::writeAbstractItem(crAbstractItem *item,CRCore::ref_ptr<CRCore::crStreamBuf>&stream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count)
{
	//char mtime[20];
	//CRCore::Timer_t mtime = 0L;
	//struct stat buf;
	//std::string cookfilename;
	//write res
	writeStreamVec(stream,4,BufVec,count);
	stream->_writeInt(item->getResID());//4
	writeStreamVec(stream,item->getFileName().size()+4,BufVec,count);
	stream->_writeString(item->getFileName());
	//writeStreamVec(stream,item->getUpdatecfg().size()+4,BufVec,count);
	//stream->_writeString(item->getUpdatecfg());
	//writeStreamVec(stream,item->getPicture().size()+4,BufVec,count);
	//stream->_writeString(item->getPicture());
	writeStreamVec(stream,item->getResScript().size()+4,BufVec,count);
	stream->_writeString(item->getResScript());
	//file modify time
	//mtime = item->getFileModifyTime();
	//if(mtime == 0L)
	//{
	//	cookfilename = item->getFileName();
	//	if(!cookfilename.empty())
	//	{
	//		CRIOManager::GetCookedFileName(cookfilename);
	//		if (stat( cookfilename.c_str(), &buf )) 
	//		{
	//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
	//		}
	//		else
	//		{
	//			mtime = buf.st_mtime;
	//			item->setFileModifyTime(mtime);
	//		}
	//	}
	//}
	////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
	////stream->_writeString(mtime);//20+4
	//writeStreamVec(stream,8,BufVec,count);
	//stream->_writeInt64(mtime);//8

	//updatecfg modify time
	//mtime = item->getUpdcfgModifyTime();
	//if(mtime == 0L)
	//{
	//	cookfilename = item->getUpdatecfg();
	//	if(!cookfilename.empty())
	//	{
	//		CRIOManager::GetCookedFileName(cookfilename);
	//		if (stat( cookfilename.c_str(), &buf )) 
	//		{
	//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
	//		}
	//		else
	//		{
	//			mtime = buf.st_mtime;
	//			item->setUpdcfgModifyTime(mtime);
	//		}
	//	}
	//}
	////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
	////stream->_writeString(mtime);//20+4
	//writeStreamVec(stream,8,BufVec,count);
	//stream->_writeInt64(mtime);//8

	//script modify time
	//mtime = item->getResScriptModifyTime();
	//if(mtime == 0L)
	//{
	//	cookfilename = item->getResScript();
	//	if(!cookfilename.empty())
	//	{
	//		CRIOManager::GetCookedFileName(cookfilename);
	//		if (stat( cookfilename.c_str(), &buf )) 
	//		{
	//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
	//		}
	//		else
	//		{
	//			mtime = buf.st_mtime;
	//			item->setResScriptModifyTime(mtime);
	//		}
	//	}
	//}
	////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
	////stream->_writeString(mtime);//20+4
	//writeStreamVec(stream,8,BufVec,count);
	//stream->_writeInt64(mtime);//8

	//write abstractitem
	writeStreamVec(stream,4,BufVec,count);
	stream->_writeInt(item->getAbstractItemID());//4
	writeStreamVec(stream,item->getName().size()+4,BufVec,count);
	stream->_writeString(item->getName());//4
	writeStreamVec(stream,item->getScript().size()+4,BufVec,count);
	stream->_writeString(item->getScript());//4
	//script modify time
	//mtime = item->getScriptModifyTime();
	//if(mtime == 0L)
	//{
	//	cookfilename = item->getScript();
	//	if(!cookfilename.empty())
	//	{
	//		CRIOManager::GetCookedFileName(cookfilename);
	//		if (stat( cookfilename.c_str(), &buf )) 
	//		{
	//			CRCore::notify(CRCore::ALWAYS)<<cookfilename<<" 文件不存在"<<std::endl;
	//		}
	//		else
	//		{
	//			mtime = buf.st_mtime;
	//			item->setScriptModifyTime(mtime);
	//		}
	//	}
	//}
	////writeStreamVec(stream,strlen(mtime)+4,BufVec,count);
	////stream->_writeString(mtime);//20+4
	//writeStreamVec(stream,8,BufVec,count);
	//stream->_writeInt64(mtime);//8
}
/////////////////////////////////////////
//
//readItem
//
/////////////////////////////////////////
void CRNetApp::readItem(CRCore::ref_ptr<crInstanceItem>&item,CRCore::crStreamBuf *stream,bool loadData)//服务器端loadData = true，客户端loadData = false
{
	unsigned char itemtype = stream->_readUChar();
	int streamCount;
	CRCore::crStreamBuf *itemStream;
	crData *data;
    if(itemtype == crInstanceItem::Role)
	{//read role
		crRole *role = new crRole;
		item = role;
		role->setRoleID(stream->_readInt());
		role->setPlayerID(stream->_readInt());
		streamCount = stream->_readInt();
		if(streamCount>0)
		{
			itemStream = new CRCore::crStreamBuf;
			itemStream->createBuf(streamCount);
			itemStream->write(stream->getPtr(),streamCount);
			stream->seek(streamCount);
			role->setMetierStream(itemStream);
		}
        
		bool hasMetier = stream->_readBool();
		if(hasMetier)
		{
			ref_ptr<crAbstractMetier> ametier = new crAbstractMetier;
			role->setAbstractMetier(ametier.get());
			//read abstractmetier
			ametier->setAbstractMetierID(stream->_readInt());
			ametier->setGameID(stream->_readInt());
			ametier->setMetierName(stream->_readString());
			ametier->setInitSceneID(stream->_readInt());
			ametier->setMetierScript(stream->_readString());
			//ametier->setMetierScriptModifyTime(stream->_readInt64());
			ametier->setTaskTable(stream->_readString());
			//ametier->setTaskTableModifyTime(stream->_readInt64());
			if(loadData && !(ametier->getMetierScript().empty()))
			{
				data = CREncapsulation::loadData(ametier->getMetierScript());
				if(data)
				{
					data->inputParam(WCHDATA_GameTaskTab,&(ametier->getTaskTable()));
					role->setMetierDataClass(data);
					crStreamBuf *metierstream = role->getMetierStream();
					if(metierstream)
					{
						data->inputParam(WCHDATA_DataStream,metierstream);
					}
					data->excHandle(MAKEINT64(WCH_InitData,role));
					role->setMetierStream(NULL);
				}
			}
		}
	}
	else
	{
		item = new crInstanceItem;
	}
	item->setItemtype(itemtype);
	item->setInstanceItemID(stream->_readInt());
	if(item->getInstanceItemID()!=0)
	{
		if(itemtype == crInstanceItem::Emporium)
		{
			item->setAbstractItemID(0);
			item->getAbstractItem()->setScript(stream->_readString());
			//item->getAbstractItem()->setScriptModifyTime(stream->_readInt64());
			item->setSceneID(stream->_readInt());
		}
		else
		{
			bool hasAItem = stream->_readBool();
			if(hasAItem)
			{
				//read abstractitem
				item->setAbstractItemID(stream->_readInt());
				item->getAbstractItem()->setName(stream->_readString());
				item->getAbstractItem()->setScript(stream->_readString());
				//item->getAbstractItem()->setScriptModifyTime(stream->_readInt64());

				//read res
				item->getAbstractItem()->setResID(stream->_readInt());
				item->getAbstractItem()->setFileName(stream->_readString());
				//item->getAbstractItem()->setUpdatecfg(stream->_readString());
				//item->setPicture(stream->_readString());
				item->getAbstractItem()->setResScript(stream->_readString());
				//file modify time
				//item->getAbstractItem()->setFileModifyTime(stream->_readInt64());
				//updatecfg modify time
				//item->getAbstractItem()->setUpdcfgModifyTime(stream->_readInt64());
				//picture modify time
				//item->setPicModifyTime(stream->_readString());
				//item->getAbstractItem()->setResScriptModifyTime(stream->_readInt64());
			}
			//read instanceitem
			item->setRoomID(stream->_readInt());
			item->setIName(stream->_readString());
			item->setSceneID(stream->_readInt());
			item->setLayerID(stream->_readUShort());
			item->setPosxy(stream->_readVec2());
			item->setPosz(stream->_readFloat());
			item->setZoffset(stream->_readShort());
			crVector3 dir;
			dir[0] = stream->_readFloat();
			dir[1] = stream->_readFloat();
			dir[2] = stream->_readFloat();
			item->setDir(dir);
		}
		streamCount = stream->_readInt();
		if(streamCount>0)
		{
			itemStream = new CRCore::crStreamBuf;
			itemStream->createBuf(streamCount);
			itemStream->write(stream->getPtr(),streamCount);
			stream->seek(streamCount);
			item->setStream(itemStream);
		}
		if(loadData)
		{
			if(!item->getAbstractItem())
			{
				CRCore::notify(CRCore::FATAL)<<"错误：Item没有抽象ID，ItemID = "<<item->getInstanceItemID()<<"检查InstanceItemTab"<<std::endl;
			}
			else
			{
				data = CREncapsulation::loadData(item->getAbstractItem()->getScript());
				if(data)
				{
					item->setDataClass(data);
					crStreamBuf *itemstream = item->getStream();
					if(itemstream)
					{
						data->inputParam(WCHDATA_DataStream,itemstream);
					}
					data->excHandle(MAKEINT64(WCH_InitData,item.get()));
					item->setStream(NULL);
				}
			}
		}
		readChildItem(item.get(),stream,loadData);
	}
}
/////////////////////////////////////////
//
//readChildItem
//
/////////////////////////////////////////
void CRNetApp::readChildItem(crInstanceItem *parentItem,CRCore::crStreamBuf *stream,bool loadData)
{
	ref_ptr<crItemChild> itemChild;
	unsigned char itemtype;
	ref_ptr<crInstanceItem> item;
	CRCore::crStreamBuf *itemStream;
	int itemStreamCount;
	int childItemCount = stream->_readInt();
	crData* data;
	for(int i = 0; i<childItemCount; i++)
	{
		itemChild = new crItemChild;

		itemChild->setItemChildID(stream->_readInt());
		//itemChild->setInstanceItemID(stream->_readInt());

		itemtype = stream->_readUChar();
        item = new crInstanceItem;
		//item->setInstanceItemID(itemChild->getInstanceItemID());
		//read abstractitem
		item->setAbstractItemID(stream->_readInt());
		item->getAbstractItem()->setName(stream->_readString());
		item->getAbstractItem()->setScript(stream->_readString());
		//item->getAbstractItem()->setScriptModifyTime(stream->_readInt64());

		//read res
		item->getAbstractItem()->setResID(stream->_readInt());
		item->getAbstractItem()->setFileName(stream->_readString());
		//item->getAbstractItem()->setUpdatecfg(stream->_readString());
		//item->setPicture(stream->_readString());
		item->getAbstractItem()->setResScript(stream->_readString());
		//file modify time
		//item->getAbstractItem()->setFileModifyTime(stream->_readInt64());
		//updatecfg modify time
		//item->getAbstractItem()->setUpdcfgModifyTime(stream->_readInt64());
		//picture modify time
		//item->setPicModifyTime(stream->_readString());
		//item->getAbstractItem()->setResScriptModifyTime(stream->_readInt64());

		//read instanceitem
		item->setInstanceItemID(stream->_readInt());
		item->setIName(stream->_readString());
		//item->setSceneID(stream->_readInt());
		//item->setLayerID(stream->_readUShort());
		//item->setPosx(stream->_readInt());
		//item->setPosy(stream->_readInt());
	 //   item->setPosz(stream->_readInt());
		////item->setZoffset(stream->_readShort());
		//item->setDirx(stream->_readFloat());
		//item->setDiry(stream->_readFloat());
		//item->setDirz(stream->_readFloat());
		itemStreamCount = stream->_readInt();
		if(itemStreamCount>0)
		{
			itemStream = new CRCore::crStreamBuf;
			itemStream->createBuf(itemStreamCount);
			itemStream->write(stream->getPtr(),itemStreamCount);
			stream->seek(itemStreamCount);
			item->setStream(itemStream);
		}
		if(loadData)
		{
			if(!item->getAbstractItem())
			{
				CRCore::notify(CRCore::FATAL)<<"错误：Item没有抽象ID，ItemID = "<<item->getInstanceItemID()<<"检查InstanceItemTab"<<std::endl;
			}
			else
			{
				data = CREncapsulation::loadData(item->getAbstractItem()->getScript());
				if(data)
				{
					item->setDataClass(data);
					crStreamBuf *itemstream = item->getStream();
					if(itemstream)
					{
						data->inputParam(WCHDATA_DataStream,itemstream);
					}
					data->excHandle(MAKEINT64(WCH_InitData,item.get()));
					item->setStream(NULL);
				}
			}
		}
		itemChild->setInstanceItem(item.get());
		itemChild->setParentItem(parentItem);
		readChildItem(item.get(),stream,loadData);

		parentItem->insertChildItem(itemChild.get());
	}
}
/////////////////////////////////////////
//
//readAbstractItem
//
/////////////////////////////////////////
void CRNetApp::readAbstractItem(CRCore::ref_ptr<crAbstractItem>&item,CRCore::crStreamBuf *stream/*,bool loadData*/)//服务器端loadData = true
{
	//crData *data;
	item = new crAbstractItem;
	//item->setItemtype(crInstanceItem::instanceitem);
	//read res
	item->setResID(stream->_readInt());
	item->setFileName(stream->_readString());
	//item->setUpdatecfg(stream->_readString());
	//item->setPicture(stream->_readString());
	item->setResScript(stream->_readString());
	//file modify time
	//item->setFileModifyTime(stream->_readInt64());
	//updatecfg modify time
	//item->setUpdcfgModifyTime(stream->_readInt64());
	//picture modify time
	//item->setPicModifyTime(stream->_readString());
	//item->setResScriptModifyTime(stream->_readInt64());

	//read abstractitem
	item->setAbstractItemID(stream->_readInt());
	item->setName(stream->_readString());
	item->setScript(stream->_readString());
	//item->setScriptModifyTime(stream->_readInt64());

	////read instanceitem
	//item->setInstanceItemID(stream->_readInt());
	//item->setIName(stream->_readString());
	//item->setSceneID(stream->_readInt());
	//item->setLayerID(stream->_readUShort());
	//item->setPosx(stream->_readInt());
	//item->setPosy(stream->_readInt());
	//item->setZoffset(stream->_readShort());
	//item->setDirx(stream->_readFloat());
	//item->setDiry(stream->_readFloat());
	//item->setDirz(stream->_readFloat());
	//if(loadData)
	//{
	//	data = CREncapsulation::loadData(item->getScript());
	//	if(data)
	//	{
	//		item->setDataClass(data);
	//		data->excHandle(MAKEINT64(WCH_InitData,NULL));
	//	}
	//}
	//crGlobalHandle::getInstance()->insertAbstractItem(item.get());
}
/////////////////////////////////////////
//
//crQuerySceneItemPacket
//
/////////////////////////////////////////
crQuerySceneItemPacket::crQuerySceneItemPacket():
crStreamPacket(crQuerySceneItemPacket::ID)
{
}

crQuerySceneItemPacket::crQuerySceneItemPacket( const crQuerySceneItemPacket &p ):
crStreamPacket(crQuerySceneItemPacket::ID,p)
{
}

void crQuerySceneItemPacket::buildRequestPacket(crQuerySceneItemPacket &packet,int sceneid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(sceneid);
}

void crQuerySceneItemPacket::buildReplyPacket(crQuerySceneItemPacket &packet,int streamid, int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crQuerySceneItemPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		//CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
		int sceneid = m_streamBuf->_readInt();
		crDataBase *instancedb = crDataBaseManager::getInstance()->getDataBase(GameDB,0);
		if(instancedb)
		{
			ref_ptr<crDataBase> instanceSession = instancedb->beginSession();
			ref_ptr<crSceneItemQueryData> sceneItemQuery = new crSceneItemQueryData;
			sceneItemQuery->buildQuerySql(sceneid);
			instanceSession->executeQuery(sceneItemQuery.get());
			crDataBase::QueryResultVec& sceneItemQueryResultVec = instanceSession->getQueryResult();
			if(!sceneItemQueryResultVec.empty())
			{
				CRCore::ref_ptr<crInstanceItem> instanceItem;
				CRCore::ref_ptr<crItemChild> itemChild;
				std::vector< CRCore::ref_ptr<crInstanceItem> > itemVec;
				std::vector< CRCore::ref_ptr<crInstanceItem> >::iterator itemItr;
				crAbstractItem *aitem;
				for( crDataBase::QueryResultVec::iterator itr = sceneItemQueryResultVec.begin();
					 itr != sceneItemQueryResultVec.end();
					 ++itr )
				{
					sceneItemQuery = dynamic_cast<crSceneItemQueryData *>(itr->get());
					instanceItem = new crInstanceItem;
					instanceItem->setInstanceItemID(sceneItemQuery->getID());
					//instanceItem->setAbstractItemID(sceneItemQuery->getAbstractItemID());
					aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(sceneItemQuery->getAbstractItemID());
					instanceItem->setAbstractItem(aitem);
					instanceItem->setItemtype(sceneItemQuery->getItemtype());
					instanceItem->setIName(sceneItemQuery->getIName());
					instanceItem->setSceneID(sceneid);
					instanceItem->setLayerID(sceneItemQuery->getLayerid());
					instanceItem->setPosxy(sceneItemQuery->getPosx(),sceneItemQuery->getPosy());
					instanceItem->setZoffset(sceneItemQuery->getZoffset());
					instanceItem->setDir(CRCore::crVector3(sceneItemQuery->getDirx(),sceneItemQuery->getDiry(),sceneItemQuery->getDirz()));
					instanceItem->setStream(sceneItemQuery->getStream());
					itemVec.push_back(instanceItem.get());
				}
				instanceSession->releaseQuery();

				for( itemItr = itemVec.begin();
						itemItr != itemVec.end();
						++itemItr )
				{
					//itemchild查询
					ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
					itemChildQuery->buildQuerySql((*itemItr)->getInstanceItemID());
					instanceSession->executeQuery(itemChildQuery.get());
					crDataBase::QueryResultVec &itemChildQueryResultVec = instanceSession->getQueryResult();
					if(!itemChildQueryResultVec.empty())
					{
						for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
							citr != itemChildQueryResultVec.end();
							++citr )
						{
							itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
							itemChild = new crItemChild;
							itemChild->setItemChildID(itemChildQuery->getID());
							itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
							itemChild->setParentItem(itemItr->get());
							(*itemItr)->insertChildItem(itemChild.get());
						}
					}
					instanceSession->releaseQuery();
					//ChildItem查询
                    (*itemItr)->childItemQuery(instanceSession.get());
				}
			    
				instancedb->endSession(instanceSession.get());

				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
				stream->_writeInt(sceneid);
				stream->_writeInt(itemVec.size());
	    
				for( itemItr = itemVec.begin();
					itemItr != itemVec.end();
					++itemItr )
				{
					writeItem(itemItr->get(),stream,BufVec,count,SaveStream);
				}
				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crQuerySceneItemPacket packet;
					crQuerySceneItemPacket::buildReplyPacket(packet,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,0,streamid,crQuerySceneItemPacket::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
				}
				else
				{
					crQuerySceneItemPacket packet;
					crQuerySceneItemPacket::buildReplyPacket(packet,0,0);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
				}
			}
			else
			{
				instanceSession->releaseQuery();
				instancedb->endSession(instanceSession.get());

				crQuerySceneItemPacket packet;
				crQuerySceneItemPacket::buildReplyPacket(packet,0,0);
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
		}
		//CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneItemPacket sceneid:"<<sceneid<<" 耗时"<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<"s"<<std::endl;
	}
	else if(netType == SceneServerClient_Game)
	{
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		if(count>0)
		    crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crQuerySceneItemPacket::ID,count);
		else
		{
            crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			netCallback->initSuccess();
		}
	}
}
/////////////////////////////////////////
//
//crLoginScenePacket
//
/////////////////////////////////////////
crLoginScenePacket::crLoginScenePacket():
crStreamPacket(crLoginScenePacket::ID)
{
}

crLoginScenePacket::crLoginScenePacket( const crLoginScenePacket &p ):
crStreamPacket(crLoginScenePacket::ID,p)
{
}

void crLoginScenePacket::buildRequestPacket(crLoginScenePacket &packet,int sceneid,int msgid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(sceneid);
	streamBuf->_writeInt(msgid);
}

void crLoginScenePacket::buildRequestPacket2(crLoginScenePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crLoginScenePacket::buildRequestPacket3(crLoginScenePacket &packet,const std::string &playerAddress,int playerid/*,const std::string &playerName*/,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+/*playerName.length()+*/playerAddress.length()+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->_writeInt(playerid);
	//streamBuf->_writeString(playerName);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crLoginScenePacket::buildReplyPacket(crLoginScenePacket &packet,const std::string &serverName,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(serverName.length()+4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(serverName);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crLoginScenePacket::buildReplyPacket(crLoginScenePacket &packet,char loginCode,int playerid,int msgid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);//1
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(msgid);
}
void crLoginScenePacket::buildReplyPacket2(crLoginScenePacket &packet,char loginCode,int msgid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);
	streamBuf->_writeInt(msgid);
}

void crLoginScenePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		//crLoginScenePacket packet;
		//crLoginScenePacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		//crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Scene,0);
		//if(netConductor)
		//{
		//	netConductor->getNetManager()->sendPacket("all",packet);
		//}
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crLoginScenePacket packet;
				crLoginScenePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crLoginScenePacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		int sceneid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		const std::string& serverName = netCallback->findSceneServerName(sceneid);
		crLoginScenePacket packet;
		crLoginScenePacket::buildReplyPacket(packet,serverName,m_streamBuf.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == GameConnectServerClient_Game)
	{
		std::string serverName = m_streamBuf->_readString();
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			if(!serverName.empty())
			{
				crNetConductor *sceneConductor = crNetContainer::getInstance()->findNetConductor(GameConnectServerClient_Scene,serverName);
				crLoginScenePacket packet;
				crLoginScenePacket::buildRequestPacket3(packet,playerData->getPlayerAdress(),playerid,/*playerData->getPlayerName(),*/m_streamBuf.get());
				sceneConductor->getNetManager()->sendPacket("all",packet);
			}
			else
			{
				int msgid = m_streamBuf->_readInt();
				crLoginScenePacket packet;
				crLoginScenePacket::buildReplyPacket2(packet,0,msgid);
				connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		std::string playerAddress = m_streamBuf->_readString();
		int playerid = m_streamBuf->_readInt();
		//std::string playerName = m_streamBuf->_readString();
		int sceneid = m_streamBuf->_readInt();

		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		char loginCode = 0;
		if(netCallback->checkScene(sceneid))
		{
			//向GameServer验证
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
			if(netConductor)
			{
				crLoginScene2Packet packet;
				crLoginScene2Packet::buildRequestPacket(packet,sender,m_streamBuf.get());
				netConductor->getNetManager()->sendPacket("all",packet);

			    loginCode = 1;
			}
		}
		if(loginCode == 0)
		{
			int msgid = m_streamBuf->_readInt();
			crLoginScenePacket packet;
			crLoginScenePacket::buildReplyPacket(packet,loginCode,playerid,msgid);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginScenePacket "<<loginCode<<std::endl;
	}
	else if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
		{
			crNetDataManager *netDataManager = connectServer->getNetDataManager();
			char loginCode = m_streamBuf->_readChar();
			int playerid = m_streamBuf->_readInt();
			int msgid = m_streamBuf->_readInt();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid())
			{
				if(loginCode>0)
				{
					playerData->setSceneServerLineConductor(m_netConductor);
				}
				crLoginScenePacket packet;
				crLoginScenePacket::buildReplyPacket2(packet,loginCode,msgid);
				connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crLoginScenePacket SendTo GameClient"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		char loginCode = m_streamBuf->_readChar();
		int msgid = m_streamBuf->_readInt();
		m_streamBuf->seekBegin();
		crBrain::getInstance()->doEvent(MAKEINT64(WCH_LoginSceneReturn,msgid),MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crLoginScene2Packet
//
/////////////////////////////////////////
crLoginScene2Packet::crLoginScene2Packet():
crStreamPacket(crLoginScene2Packet::ID)
{
}

crLoginScene2Packet::crLoginScene2Packet( const crLoginScene2Packet &p ):
crStreamPacket(crLoginScene2Packet::ID,p)
{
}

void crLoginScene2Packet::buildRequestPacket(crLoginScene2Packet &packet,const std::string &connectServerAddress,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(connectServerAddress.length()+4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(connectServerAddress);
    streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crLoginScene2Packet::buildReplyPacket(crLoginScene2Packet &packet,char loginCode,unsigned short gamedbid,CRCore::crStreamBuf *stream,crPlayerGameData *playerGameData)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	int playersize = 0;
	ref_ptr<crStreamBuf> dataStream;
	//std::string name;
	if(playerGameData)
	{
		crData *data = playerGameData->getDataClass();
		if(data)
		{
			dataStream = new crStreamBuf;
			data->excHandle(MAKEINT64(WCH_BuildPlayerStream,dataStream.get()/*NetStream*/));
			//void *param;
			//data->getParam(WCHDATA_DataStream,param);
			//if(param)
			//{
			//	dataStream = *((ref_ptr<crStreamBuf>*)param);
			//}
		}
		//name = playerGameData->getName();
		playersize = 4 + (dataStream.valid()?dataStream->getBufSize():0);
	}
	streamBuf->createBuf(3+stream->getBufSize()+playersize);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);
	streamBuf->_writeUShort(gamedbid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
	if(playersize>0)
	{
		//streamBuf->_writeString(name);
		int mainroleid = playerGameData->getMainRoleID();
		streamBuf->_writeInt(mainroleid);
		if(dataStream.valid())
			streamBuf->write(dataStream->getBuf(),dataStream->getBufSize());
	}
}

void crLoginScene2Packet::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		char loginCode = 0;
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		std::string playerConnectServerAddress = m_streamBuf->_readString();
		std::string playerAddress = m_streamBuf->_readString();
		int playerid = m_streamBuf->_readInt();
		//std::string playerName = m_streamBuf->_readString();
		int sceneid = m_streamBuf->_readInt();

		unsigned short gamedbid = 0;
		crPlayerGameData *playerGameData = NULL;
        ref_ptr<crGameServerPlayerData> gameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(gameServerPlayerData.valid())
		{
			//const std::string &sceneServerAddress = gameServerPlayerData->getSceneServerAddress();
			//if(!sceneServerAddress.empty())
			//{
			//	crLogoutScenePacket logoutScenePacket;
			//	crLogoutScenePacket::buildRequestPacket(logoutScenePacket,playerid);
			//	m_netConductor->getNetManager()->sendPacket(sceneServerAddress,logoutScenePacket);
			//}

			loginCode = 1;
			gamedbid = gameServerPlayerData->getGameDBID();
			gameServerPlayerData->setSceneServerAddress(sender);
			gameServerPlayerData->setSceneID(sceneid);
			playerGameData = gameServerPlayerData->getPlayerGameData();
		}
		crLoginScene2Packet packet;
		crLoginScene2Packet::buildReplyPacket(packet,loginCode,gamedbid,m_streamBuf.get(),playerGameData);
		m_netConductor->getNetManager()->sendPacket(sender,packet);
		if(playerGameData)
		{
			crRole *mainRole = playerGameData->getMainRole();
			if(mainRole)
			{//
				/////write and send
				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

				writeItem(mainRole,stream,BufVec,count,PlayerStream);

				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crLoginScene3Packet packet;
					crLoginScene3Packet::buildReplyPacket(packet,playerid,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,crLoginScene3Packet::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
				}
			}
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(sceneServer)
		{
			char loginCode = m_streamBuf->_readChar();
			unsigned short gamedbid = m_streamBuf->_readUShort();
			std::string playerConnectServerAddress = m_streamBuf->_readString();
			std::string playerAddress = m_streamBuf->_readString();
			int playerid = m_streamBuf->_readInt();
			int sceneid = m_streamBuf->_readInt();
			int msgid = m_streamBuf->_readInt();
			ref_ptr<crSceneServerPlayerData> playerData;
			if(loginCode>0)
			{
				crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
				ref_ptr<crSceneServerPlayerData> connectedPlayerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
				if (connectedPlayerData.valid())
				{
					//connectedPlayerData->setGameServerLineConductor(NULL);
					//connectedPlayerData->setPlayerConnectServerAddress("");
					//sceneServer->getNetDataManager()->removePlayerData(playerid);
					callback->playerDroped(connectedPlayerData.get(),true);
				}

                playerData = new crSceneServerPlayerData;
				playerData->saveTimer() = callback->getSaveInterval();
				playerData->setSceneID(sceneid);
				playerData->setGameDBID(gamedbid);
				playerData->setPlayerID(playerid);
				//playerData->setPlayerName(playerName);
				playerData->setPlayerAdress(playerAddress);
				playerData->setPlayerConnectServerAddress(playerConnectServerAddress);
			    playerData->setGameServerLineConductor(m_netConductor);

				crScene *scene = callback->findScene(sceneid);
				if(scene)
				{
					ref_ptr<crPlayerGameData> playerGameData;
					std::string script = scene->getGameName() + "/playergamedata.cfg";
					ref_ptr<crData> data = CREncapsulation::loadData(script);
					if(data.valid())
					{
						playerGameData = new crPlayerGameData;
						playerGameData->setPlayerID(playerid);
						playerGameData->setDataClass(data.get());
						//playerGameData->setName(m_streamBuf->_readString());
						playerGameData->setMainRoleID(m_streamBuf->_readInt());
						playerGameData->setDBValid(true);
						if(!m_streamBuf->eof())
						{
							//crStreamBuf *dataStream = new CRCore::crStreamBuf;
							//dataStream->createBuf(streamCount);
							//dataStream->write(stream->getPtr(),streamCount);
							//dataStream->seekBegin();
							//stream->seek(streamCount);
							data->inputParam(WCHDATA_PlayerStream,m_streamBuf.get());
							data->excHandle(MAKEINT64(WCH_InitData,playerGameData.get()));
						}
						playerData->setPlayerGameData(playerGameData.get());
					}
				}
				sceneServer->getNetDataManager()->insertPlayerData(playerData.get());
				crLoginScenePacket packet;
				crLoginScenePacket::buildReplyPacket(packet,loginCode,playerid,msgid);
				sceneServer->getNetManager()->sendPacket(playerConnectServerAddress,packet);
				crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,SceneServer),MAKEINT64(playerData.get(),NULL));
			}
			else
			{
				crLoginScenePacket packet;
				crLoginScenePacket::buildReplyPacket(packet,loginCode,playerid,msgid);
				sceneServer->getNetManager()->sendPacket(playerConnectServerAddress,packet);
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crLoginScene2Packet SendTo SceneConnectServer"<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crLoginScene3Packet
//
/////////////////////////////////////////
crLoginScene3Packet::crLoginScene3Packet():
	crStreamPacket(crLoginScene3Packet::ID)
{
}
crLoginScene3Packet::crLoginScene3Packet( const crLoginScene3Packet &p ):
	crStreamPacket(crLoginScene3Packet::ID,p)
{
}
void crLoginScene3Packet::buildReplyPacket(crLoginScene3Packet &packet,int playerid,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crLoginScene3Packet::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crLoginScene3Packet::ID,count);
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginScene3Packet count="<<count<<std::endl;
	}
}
/////////////////////////////////////////
//
//crQueryRoleInfoPacket
//
/////////////////////////////////////////
crQueryRoleInfoPacket::crQueryRoleInfoPacket():
crStreamPacket(crQueryRoleInfoPacket::ID)
{
}

crQueryRoleInfoPacket::crQueryRoleInfoPacket( const crQueryRoleInfoPacket &p ):
crStreamPacket(crQueryRoleInfoPacket::ID,p)
{
}

void crQueryRoleInfoPacket::buildRequestPacket(crQueryRoleInfoPacket &packet,crRole *role)
{
	crStreamBuf *streamBuf = new crStreamBuf;	
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(role->getRoleID());
	crAbstractItem *aitem = role->getAbstractItem();
	if(aitem)
		streamBuf->_writeInt(aitem->getAbstractItemID());
	else
		streamBuf->_writeInt(0);
}
void crQueryRoleInfoPacket::buildRequestPacket(crQueryRoleInfoPacket &packet,int roleid)
{
	crStreamBuf *streamBuf = new crStreamBuf;	
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeInt(0);
}
void crQueryRoleInfoPacket::buildRequestPacket2(crQueryRoleInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQueryRoleInfoPacket::buildReplyPacket(crQueryRoleInfoPacket &packet,int playerid,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crQueryRoleInfoPacket::buildReplyPacket2(crQueryRoleInfoPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQueryRoleInfoPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid() && m_streamBuf.get())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crQueryRoleInfoPacket packet;
				crQueryRoleInfoPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryRoleInfoPacket "<<std::endl;
			}
			else
			{
				crNetConductor *conductor = playerData->getGameServerLineConductor();
				if(conductor)
				{
					crQueryRoleInfo2Packet packet;
					crQueryRoleInfo2Packet::buildRequestPacket(packet,playerData->getPlayerID(),m_streamBuf.get());
					conductor->getNetManager()->sendPacket("all",packet);
					//CRCore::notify(CRCore::ALWAYS)<<"crQueryRoleInfoPacket "<<std::endl;
				}
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			crNetConductor *netConductor = playerData->getGameServerLineConductor();
			if(netConductor)
			{
				crQueryRoleInfo2Packet packet;
				crQueryRoleInfo2Packet::buildRequestPacket(packet,m_streamBuf.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crQueryRoleInfoPacket packet;
			crQueryRoleInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crQueryRoleInfoPacket::ID,count);
        //CRCore::notify(CRCore::ALWAYS)<<"crQueryRoleInfoPacket count= "<<count<<std::endl;
	}
}
/////////////////////////////////////////
//
//crQueryRoleInfo2Packet
//
/////////////////////////////////////////
crQueryRoleInfo2Packet::crQueryRoleInfo2Packet():
crStreamPacket(crQueryRoleInfo2Packet::ID)
{
}

crQueryRoleInfo2Packet::crQueryRoleInfo2Packet( const crQueryRoleInfo2Packet &p ):
crStreamPacket(crQueryRoleInfo2Packet::ID,p)
{
}

void crQueryRoleInfo2Packet::buildRequestPacket(crQueryRoleInfo2Packet &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crQueryRoleInfo2Packet::buildRequestPacket(crQueryRoleInfo2Packet &packet,int playerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crQueryRoleInfo2Packet::buildReplyPacket(crQueryRoleInfo2Packet &packet,int playerid,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(streamid);
    streamBuf->_writeInt(count);
}

void crQueryRoleInfo2Packet::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		int cAbstractID = m_streamBuf->_readInt();
        crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> gameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(gameServerPlayerData.valid())
		{
			ref_ptr<crRole> role;
			unsigned short gamedbid = gameServerPlayerData->getGameDBID();
			bool isMainRole = false;
			crPlayerGameData *playerGameData = gameServerPlayerData->getPlayerGameData();
			if(playerGameData && playerGameData->getMainRoleID() == roleid)
			{
				isMainRole = true;
				role = playerGameData->getMainRole();
			}
			if(role.valid())
			{
				if(isMainRole)
				{
					role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
				}

				/////write and send
				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

				writeItem(role.get(),stream,BufVec,count,isMainRole?PlayerStream:SaveStream);

				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crQueryRoleInfo2Packet packet;
					crQueryRoleInfo2Packet::buildReplyPacket(packet,playerid,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,isMainRole?crQueryRoleInfoPacket::ID:crQueryRoleInfo2Packet::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
				}
			}
			else
			{
				CRCore::ref_ptr<crItemChild> itemChild;
				crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
				if(gamedb)
				{
					ref_ptr<crDataBase> gameSession = gamedb->beginSession();
					bool game_ended = false;
					ref_ptr<crRoleQueryData> roleQuery = new crRoleQueryData;
					roleQuery->buildQuerySql(roleid);
					gameSession->executeQuery(roleQuery.get());
					crDataBase::QueryResultVec &roleQueryResultVec = gameSession->getQueryResult();
					int ametierid = 0;
					if(!roleQueryResultVec.empty())
					{
						role = new crRole;
						roleQuery = dynamic_cast<crRoleQueryData*>(roleQueryResultVec[0].get());
						role->setRoleID(roleid);
						role->setPlayerID(playerid);
						role->setInstanceItemID(roleQuery->getInstanceItemID());
						ametierid = roleQuery->getAbstractMetierID();
						role->setMetierStream(roleQuery->getMetierStream());
					}
					gameSession->releaseQuery();
					//gamedb->endSession(gameSession.get());

					if(role.valid())
					{
						crAbstractMetier *aMetier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(ametierid);
						role->setAbstractMetier(aMetier);

						if(role->getInstanceItemID()>0)
						{
							ref_ptr<crInstanceItemQueryData> instanceItemQuery = new crInstanceItemQueryData;
							instanceItemQuery->buildQuerySql(role->getInstanceItemID());
							gameSession->executeQuery(instanceItemQuery.get());
							crDataBase::QueryResultVec &instanceItemQueryResultVec = gameSession->getQueryResult();
							int abstractid = 0;
							if(!instanceItemQueryResultVec.empty())
							{
								instanceItemQuery = dynamic_cast<crInstanceItemQueryData*>(instanceItemQueryResultVec[0].get());
								abstractid = instanceItemQuery->getAbstractItemID();
								//role->setAbstractItemID();
								role->setItemtype(instanceItemQuery->getItemtype());
								role->setIName(instanceItemQuery->getIName());
								role->setSceneID(instanceItemQuery->getSceneID());
								role->setLayerID(instanceItemQuery->getLayerid());
								role->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
								role->setZoffset(instanceItemQuery->getZoffset());
								role->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
								role->setStream(instanceItemQuery->getStream());
							}
							gameSession->releaseQuery();

							if(cAbstractID!=0)
								abstractid = cAbstractID;
							crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
							role->setAbstractItem(aitem);

							//itemchild查询
							ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
							itemChildQuery->buildQuerySql(role->getInstanceItemID());
							gameSession->executeQuery(itemChildQuery.get());
							crDataBase::QueryResultVec &itemChildQueryResultVec = gameSession->getQueryResult();
							if(!itemChildQueryResultVec.empty())
							{
								for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
									citr != itemChildQueryResultVec.end();
									++citr )
								{
									itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
									itemChild = new crItemChild;
									itemChild->setItemChildID(itemChildQuery->getID());
									itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
									itemChild->setParentItem(role.get());
									role->insertChildItem(itemChild.get());
								}
							}
							gameSession->releaseQuery();

							//ChildItem查询
							role->childItemQuery(gameSession.get());
						}

						gamedb->endSession(gameSession.get());
						game_ended = true;

						if(isMainRole)
						{
							playerGameData->setMainRole(role.get());
							role->setSceneID(0);
							role->setRoomID(0);
							role->loadMetierData(SaveStream);
							role->loadItemData(SaveStream,true);
							role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
							role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
						}

						/////write and send
						int count = 0;
						std::vector< ref_ptr<crStreamBuf> > BufVec;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

						writeItem(role.get(),stream,BufVec,count,isMainRole?PlayerStream:SaveStream);

						if(stream->getBufSize()>0)
						{
							count += stream->getBufSize();
							BufVec.push_back(stream.get());
						}
						if(count>0)
						{
							int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
							crQueryRoleInfo2Packet packet;
							crQueryRoleInfo2Packet::buildReplyPacket(packet,playerid,streamid,count);
							m_netConductor->getNetManager()->sendPacket(sender,packet);
							crDataStreamPacket streamPacket;
							for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
								itr != BufVec.end();
								++itr )
							{
								crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,isMainRole?crQueryRoleInfoPacket::ID:crQueryRoleInfo2Packet::ID,itr->get());
								m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
							}
							crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
						}
					}
					if(!game_ended) gamedb->endSession(gameSession.get());
				}
			}
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		//crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		//crSceneServerPlayerData *playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
		//if(playerData)
		//{
			//m_streamBuf->seekBegin();
			//sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
			int streamid = m_streamBuf->_readInt();
			int count = m_streamBuf->_readInt();
			crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crQueryRoleInfo2Packet::ID,count);
			//CRCore::notify(CRCore::ALWAYS)<<"crQueryRoleInfoPacket2 count= "<<count<<std::endl;
		//}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crQueryRoleInfoPacket packet;
			crQueryRoleInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
}
/////////////////////////////////////////
//
//crRequestPlayerPacket
//
/////////////////////////////////////////
crRequestPlayerPacket::crRequestPlayerPacket():
crStreamPacket(crRequestPlayerPacket::ID)
{
}

crRequestPlayerPacket::crRequestPlayerPacket( const crRequestPlayerPacket &p ):
crStreamPacket(crRequestPlayerPacket::ID,p)
{
}

void crRequestPlayerPacket::buildRequestPacket(crRequestPlayerPacket &packet,int playerid,int roleid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(roleid);
}

void crRequestPlayerPacket::buildRequestPacket2(crRequestPlayerPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestPlayerPacket::buildReplyPacket(crRequestPlayerPacket &packet,int recvid,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crRequestPlayerPacket::buildReplyPacket2(crRequestPlayerPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestPlayerPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRequestPlayerPacket packet;
				crRequestPlayerPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestPlayerPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int sendid = m_streamBuf->_readInt();
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRole *role = playerData->getRole(roleid);
			if(role)
			{
				/////write and send
				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

				writeItem(role,stream,BufVec,count,NetStream);
				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crRequestPlayerPacket packet;
					crRequestPlayerPacket::buildReplyPacket(packet,sendid,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,sendid,streamid,crRequestPlayerPacket::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
				}
			}
			/*ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(MAXBUFFERSIZE);
			stream->_writeInt(sendid);
			//write role
			stream->_writeInt(role->getPlayerID());
			//abstractmetier
			stream->_writeString(role->getAbstractMetier()->getMetierName());
			//write instanceitem
			stream->_writeString(role->getIName());
			stream->_writeInt(role->getPosx());
			stream->_writeInt(role->getPosy());
			stream->_writeInt(role->getPosz());
			stream->_writeFloat(role->getDirx());
			stream->_writeFloat(role->getDiry());
			stream->_writeFloat(role->getDirz());
			crStreamBuf *itemStream = role->getStream();
			if(!itemStream || itemStream->getBufSize() == 0)
			{
				stream->_writeInt(0);//4
			}
			else
			{
				stream->_writeInt(itemStream->getBufSize());
				stream->write(itemStream->getBuf(),itemStream->getBufSize());
			}
			//abstractitem
		    stream->_writeString(role->getScript());
			stream->_writeString(role->getScriptModifyTime());
			//res
			stream->_writeString(role->getFileName());
			stream->_writeString(role->getFileModifyTime());
			stream->_writeString(role->getUpdatecfg());
			stream->_writeString(role->getUpdcfgModifyTime());
			stream->_writeString(role->getResScript());
			stream->_writeString(role->getResScriptModifyTime());
   //         ///role状态
			//void *param;
			//crData *data = role->getDataClass();
			//data->getParam(WCHDATA_RoleState,param);
			//unsigned short rolestate = *((unsigned short*)param);
			//stream->_writeUShort(rolestate);
			//if(rolestate == IS_Move)
			//{
			//	data->getParam(WCHDATA_TargetPos,param);
			//	crVector3 targetPosition = *((crVector3*)param);
			//	stream->_writeVec3(targetPosition);
			//}

			crRequestPlayerPacket packet;
			crRequestPlayerPacket::buildReplyPacket(packet,stream.get());
			m_netConductor->getNetManager()->sendPacket(sender,packet);*/
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRequestPlayerPacket packet;
			crRequestPlayerPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
        int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crRequestPlayerPacket::ID,count);
  //      ref_ptr<crRole> role = new crRole;
		//role->setPlayerID(m_streamBuf->_readInt());
		//role->getAbstractMetier()->setMetierName(m_streamBuf->_readString());
		//role->setIName(m_streamBuf->_readString());
	 //   role->setPosx(m_streamBuf->_readInt());
		//role->setPosy(m_streamBuf->_readInt());
		//role->setPosz(m_streamBuf->_readInt());
		//role->setDirx(m_streamBuf->_readFloat());
		//role->setDiry(m_streamBuf->_readFloat());
		//role->setDirz(m_streamBuf->_readFloat());
		//int streamCount = m_streamBuf->_readInt();
		//if(streamCount>0)
		//{
		//	crStreamBuf *itemStream = new CRCore::crStreamBuf;
		//	itemStream->createBuf(streamCount);
		//	itemStream->write(m_streamBuf->getPtr(),streamCount);
		//	m_streamBuf->seek(streamCount);
		//	role->setStream(itemStream);
		//}
		//role->setScript(m_streamBuf->_readString());
		//role->setScriptModifyTime(m_streamBuf->_readString());
		//role->setFileName(m_streamBuf->_readString());
		//role->setFileModifyTime(m_streamBuf->_readString());
		//role->setUpdatecfg(m_streamBuf->_readString());
		//role->setUpdcfgModifyTime(m_streamBuf->_readString());
		//role->setResScript(m_streamBuf->_readString());
		//role->setResScriptModifyTime(m_streamBuf->_readString());
  //      
		//CRCore::notify(CRCore::ALWAYS)<<"crRequestPlayerPacket "<<std::endl;
		//crGlobalHandle::getInstance()->doEvent(WCH_DownloadItem,MAKEINT64(role.get(),HttpD_DownLoadItem));
	}
}
/////////////////////////////////////////
//
//crItemInRangePacket
//
/////////////////////////////////////////
crItemInRangePacket::crItemInRangePacket():
crStreamPacket(crItemInRangePacket::ID)
{
}
crItemInRangePacket::crItemInRangePacket( const crItemInRangePacket &p ):
crStreamPacket(crItemInRangePacket::ID,p)
{
}
void crItemInRangePacket::buildRequestPacket(crItemInRangePacket &packet,int recvid,crInstanceItem *item,CRCore::crStreamBuf *itemStateStream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(17+itemStateStream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
	{
		streamBuf->_writeInt(item->getRoleID());
		streamBuf->_writeInt(item->getAbstractItemID());
	}
    streamBuf->write(itemStateStream->getBuf(),itemStateStream->getBufSize());
}
void crItemInRangePacket::buildRequestPacket2(crItemInRangePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crItemInRangePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(recvid));
		if(playerData.valid())
		{
			crItemInRangePacket packet;
			crItemInRangePacket::buildRequestPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crItemInRangePacket "<<std::endl;
		}
	}
	else if(netType == GameClient_Game)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		if(itemtype == crInstanceItem::Role)
		{
			int roleid = m_streamBuf->_readInt();
			int abstractid = m_streamBuf->_readInt();
			crMyPlayerData::RoleNpcPair roleNpcPair;
			if(!crMyPlayerData::getInstance()->findInRangePlayer(id,roleid,roleNpcPair))
			{
				if(crMyPlayerData::getInstance()->findOutRangePlayer(id,roleid,roleNpcPair) && roleNpcPair.first->getAbstractItemID() == abstractid)
				{
					roleNpcPair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
					crMyPlayerData::getInstance()->insertInRangePlayer(roleNpcPair);
					crMyPlayerData::getInstance()->removeOutRangePlayer(id,roleid);
				}
				else
				{//send packet to sceneserver
					if(id>0)
					{
						crRequestPlayerPacket packet;
						crRequestPlayerPacket::buildRequestPacket(packet,id,roleid);
						m_netConductor->getNetManager()->sendPacket("all",packet);
					}
					else
					{
						crRequestItemPacket packet;
						crRequestItemPacket::buildRequestPacket(packet,id);
						m_netConductor->getNetManager()->sendPacket("all",packet);
					}
				}
			}
			else if(roleNpcPair.first->getAbstractItemID() == abstractid)
			{
				roleNpcPair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
			}
			else
			{
				crMyPlayerData::getInstance()->removeInRangeRole(id,roleid);
				if(id>0)
				{
					crRequestPlayerPacket packet;
					crRequestPlayerPacket::buildRequestPacket(packet,id,roleid);
					m_netConductor->getNetManager()->sendPacket("all",packet);
				}
				else
				{
					crRequestItemPacket packet;
					crRequestItemPacket::buildRequestPacket(packet,id);
					m_netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
			//int roleid = m_streamBuf->_readInt();
			//crMyPlayerData::RoleNpcPair* roleNpcPair;
			//do 
			//{
			//	if(!crMyPlayerData::getInstance()->findInRangePlayer(id,roleNpcPair))
			//	{
			//		if(crMyPlayerData::getInstance()->findOutRangePlayer(id,roleNpcPair))
			//		{
			//			if(roleNpcPair->first->getRoleID() == roleid)
			//			{
			//				roleNpcPair->first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),roleNpcPair->second->getNPCBot()));
			//				crMyPlayerData::getInstance()->insertInRangePlayer(*roleNpcPair);
			//				crMyPlayerData::getInstance()->removeOutRangePlayer(id);
			//				break;
			//			}
			//			else
			//			{
			//				crMyPlayerData::getInstance()->removeOutRangePlayerInNextFrame(id);
			//			}
			//		}
			//	}
			//	else
			//	{
			//		if(roleNpcPair->first->getRoleID() == roleid)
			//		{
			//			roleNpcPair->first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),roleNpcPair->second->getNPCBot()));
			//			break;
			//		}
			//		else
			//		{
			//			roleNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,-1));
			//			crMyPlayerData::getInstance()->removeInRangePlayer(id);
			//			crMyPlayerData::getInstance()->removeOutRangePlayerInNextFrame(id);
			//		}
			//	}
			//	crRequestPlayerPacket packet;
			//	crRequestPlayerPacket::buildRequestPacket(packet,id);
			//	m_netConductor->getNetManager()->sendPacket("all",packet);
			//} while (0);
		}
		else if(itemtype == crInstanceItem::Npc)
		{
			crMyPlayerData::ItemNpcPair itemNpcPair;
			if(!crMyPlayerData::getInstance()->findInRangeNpc(id,itemNpcPair))
			{
				if(crMyPlayerData::getInstance()->findOutRangeNpc(id,itemNpcPair))
				{
					itemNpcPair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
					crMyPlayerData::getInstance()->insertInRangeNpc(itemNpcPair);
					crMyPlayerData::getInstance()->removeOutRangeNpc(id);
				}
				else
				{//send packet to sceneserver
					crRequestItemPacket packet;
					crRequestItemPacket::buildRequestPacket(packet,id);
					m_netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
			else
			{
				itemNpcPair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
			}
		}
		else
		{
			crMyPlayerData::ItemNodePair itemNodePair;
			if(!crMyPlayerData::getInstance()->findInRangeItem(id,itemNodePair))
			{
				if(crMyPlayerData::getInstance()->findOutRangeItem(id,itemNodePair))
				{
					itemNodePair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
					crMyPlayerData::getInstance()->insertInRangeItem(itemNodePair);
					crMyPlayerData::getInstance()->removeOutRangeItem(id);
				}
				else
				{//send packet to sceneserver
					crRequestItemPacket packet;
					crRequestItemPacket::buildRequestPacket(packet,id);
					m_netConductor->getNetManager()->sendPacket("all",packet);
					//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemPacket id = "<<id<<std::endl;
				}
			}
			else
			{
				itemNodePair.first->doEvent(WCH_RecvItemRTData,MAKEINT64(m_streamBuf.get(),NULL));
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crItemInRangePacket id = "<<id<<std::endl;
	}
}
/////////////////////////////////////////
//
//crItemOutRangePacket
//
/////////////////////////////////////////
crItemOutRangePacket::crItemOutRangePacket():
crStreamPacket(crItemOutRangePacket::ID)
{
}
crItemOutRangePacket::crItemOutRangePacket( const crItemOutRangePacket &p ):
crStreamPacket(crItemOutRangePacket::ID,p)
{
}
void crItemOutRangePacket::buildRequestPacket(crItemOutRangePacket &packet,int recvid,crInstanceItem *item)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(13);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role) 
		streamBuf->_writeInt(item->getRoleID());
}
void crItemOutRangePacket::buildRequestPacket2(crItemOutRangePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crItemOutRangePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		//CRCore::notify(CRCore::ALWAYS)<<"crItemOutRangePacket recvid="<<recvid<<std::endl;
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(recvid));
		if(playerData.valid())
		{
			crItemOutRangePacket packet;
			crItemOutRangePacket::buildRequestPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		if(itemtype == crInstanceItem::Role)
		{
			int roleid = m_streamBuf->_readInt();
			//crMyPlayerData::RoleNpcPair* roleNpcPair;
			//if(crMyPlayerData::getInstance()->findInRangePlayer(id,roleid,roleNpcPair))
			//{
				//roleNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
				crMyPlayerData::getInstance()->removeInRangeRole(id,roleid);
			//}
		}
		else if(itemtype == crInstanceItem::Npc)
		{
			//crMyPlayerData::ItemNpcPair* itemNpcPair;
			//if(crMyPlayerData::getInstance()->findInRangeNpc(id,itemNpcPair))
			//{
				//itemNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
				crMyPlayerData::getInstance()->removeInRangeNpc(id);
			//}
		}
		else
		{
			//crMyPlayerData::ItemNodePair* itemNodePair;
			//if(crMyPlayerData::getInstance()->findInRangeItem(id,itemNodePair))
			//{
				//itemNodePair->second->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));
				crMyPlayerData::getInstance()->removeInRangeItem(id);
			//}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crItemOutRangePacket id = "<<id<<std::endl;
	}
}
/*/////////////////////////////////////////
//
//crNpcInRangePacket
//
/////////////////////////////////////////
crNpcInRangePacket::crNpcInRangePacket():
crStreamPacket(crNpcInRangePacket::ID)
{
}
crNpcInRangePacket::crNpcInRangePacket( const crNpcInRangePacket &p ):
crStreamPacket(crNpcInRangePacket::ID,p)
{
}
void crNpcInRangePacket::buildRequestPacket(crNpcInRangePacket &packet,int recvid,int itemid,crInstanceItem *item)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(47);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(itemid);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getPosx());
	streamBuf->_writeInt(item->getPosy());
	streamBuf->_writeInt(item->getPosz());
	streamBuf->_writeFloat(item->getDirx());
	streamBuf->_writeFloat(item->getDiry());
	streamBuf->_writeFloat(item->getDirz());
	void *param;
	crData *data = item->getDataClass();
	data->getParam(WCHDATA_ItemState,param);
	unsigned short state = *((unsigned short*)param);
	streamBuf->_writeUShort(state);
	if(state == IS_Move)
	{
		data->getParam(WCHDATA_TargetPos,param);
		crVector3 targetPosition = *((crVector3*)param);
		streamBuf->_writeVec3(targetPosition);
	}
}
void crNpcInRangePacket::buildRequestPacket2(crNpcInRangePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crNpcInRangePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneConnectServerClient_Scene)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(SceneConnectServer);
		crSceneConnectServerPlayerData *playerData = dynamic_cast<crSceneConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(recvid));
		if(playerData)
		{
			crNpcInRangePacket packet;
			crNpcInRangePacket::buildRequestPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int itemid = m_streamBuf->_readInt();
		unsigned char itemtype = m_streamBuf->_readUChar();
		crMyPlayerData::ItemNpcPair itemNpcPair;
		if(!crMyPlayerData::getInstance()->findInRangeNpc(itemid,itemNpcPair))
		{
			if(crMyPlayerData::getInstance()->findOutRangeNpc(itemid,itemNpcPair))
			{
				crMyPlayerData::getInstance()->insertInRangeNpc(itemNpcPair);
				crMyPlayerData::getInstance()->removeOutRangeNpc(itemid);
				itemNpcPair.first->doEvent(WCH_RecvItemState,MAKEINT64(m_streamBuf.get(),itemNpcPair.second->getNPCBot()));
			}
			else
			{//send packet to sceneserver
				crRequestItemPacket packet;
				crRequestItemPacket::buildRequestPacket(packet,itemid);
				m_netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
		else
		{
            itemNpcPair.first->doEvent(WCH_RecvItemState,MAKEINT64(m_streamBuf.get(),itemNpcPair.second->getNPCBot()));
		}
		CRCore::notify(CRCore::ALWAYS)<<"crNpcInRangePacket "<<std::endl;
	}
}
/////////////////////////////////////////
//
//crNpcOutRangePacket
//
/////////////////////////////////////////
crNpcOutRangePacket::crNpcOutRangePacket():
crStreamPacket(crNpcOutRangePacket::ID)
{
}
crNpcOutRangePacket::crNpcOutRangePacket( const crNpcOutRangePacket &p ):
crStreamPacket(crNpcOutRangePacket::ID,p)
{
}
void crNpcOutRangePacket::buildRequestPacket(crNpcOutRangePacket &packet,int recvid,int itemid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(itemid);
}
void crNpcOutRangePacket::buildRequestPacket2(crNpcOutRangePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crNpcOutRangePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneConnectServerClient_Scene)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(SceneConnectServer);
		crSceneConnectServerPlayerData *playerData = dynamic_cast<crSceneConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(recvid));
		if(playerData)
		{
			crNpcOutRangePacket packet;
			crNpcOutRangePacket::buildRequestPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int itemid = m_streamBuf->_readInt();
		crMyPlayerData::ItemNpcPair itemNpcPair;
		if(crMyPlayerData::getInstance()->findInRangeNpc(itemid,itemNpcPair))
		{
			crMyPlayerData::getInstance()->removeInRangeNpc(itemid);
			itemNpcPair.second->getNPCBot()->doEvent(WCH_NodeOutRange);//隐藏
		}

		CRCore::notify(CRCore::ALWAYS)<<"crNpcOutRangePacket "<<std::endl;
	}
}*/
/////////////////////////////////////////
//
//crRequestItemPacket
//
/////////////////////////////////////////
crRequestItemPacket::crRequestItemPacket():
crStreamPacket(crRequestItemPacket::ID)
{
}

crRequestItemPacket::crRequestItemPacket( const crRequestItemPacket &p ):
crStreamPacket(crRequestItemPacket::ID,p)
{
}

void crRequestItemPacket::buildRequestPacket(crRequestItemPacket &packet,int itemid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(itemid);
}

void crRequestItemPacket::buildRequestPacket2(crRequestItemPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestItemPacket::buildReplyPacket(crRequestItemPacket &packet,int recvid,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crRequestItemPacket::buildReplyPacket2(crRequestItemPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestItemPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRequestItemPacket packet;
				crRequestItemPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int sendid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
        if(playerData.valid())
		{
			int itemid = m_streamBuf->_readInt();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crScene *scene = netCallback->findScene(playerData->getSceneID());
			if(scene)
			{
				crInstanceItem *item = scene->findSceneItem(itemid,playerData->getRoomID());
				if(item)
				{
					/////write and send
					int count = 0;
					std::vector< ref_ptr<crStreamBuf> > BufVec;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

					writeItem(item,stream,BufVec,count,NetStream);
					if(stream->getBufSize()>0)
					{
						count += stream->getBufSize();
						BufVec.push_back(stream.get());
					}
					if(count>0)
					{
						int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
						crRequestItemPacket packet;
						crRequestItemPacket::buildReplyPacket(packet,sendid,streamid,count);
						m_netConductor->getNetManager()->sendPacket(sender,packet);
						crDataStreamPacket streamPacket;
						for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
							itr != BufVec.end();
							++itr )
						{
							crDataStreamPacket::buildRequestPacket(streamPacket,sendid,streamid,crRequestItemPacket::ID,itr->get());
							m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
						}
						crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
					}
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRequestItemPacket packet;
			crRequestItemPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crRequestItemPacket::ID,count);

		//crGlobalHandle::getInstance()->doEvent(WCH_DownloadItem,MAKEINT64(item.get(),HttpD_DownLoadItem));
	}
}
///////////////////////////////////////////
////
////crRequestTaskAwardInfoPacket
////
///////////////////////////////////////////
//crRequestTaskAwardInfoPacket::crRequestTaskAwardInfoPacket():
//crStreamPacket(crRequestTaskAwardInfoPacket::ID)
//{
//}
//
//crRequestTaskAwardInfoPacket::crRequestTaskAwardInfoPacket( const crRequestTaskAwardInfoPacket &p ):
//crStreamPacket(crRequestTaskAwardInfoPacket::ID,p)
//{
//}
//
//void crRequestTaskAwardInfoPacket::buildRequestPacket(crRequestTaskAwardInfoPacket &packet,int roleid,int taskid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(8);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(roleid);
//	streamBuf->_writeInt(taskid);
//}
//
//void crRequestTaskAwardInfoPacket::buildRequestPacket2(crRequestTaskAwardInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crRequestTaskAwardInfoPacket::buildRequestPacket3(crRequestTaskAwardInfoPacket &packet,int abstractid,int playerid,int roleid,int taskid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(16);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(abstractid);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(roleid);
//	streamBuf->_writeInt(taskid);
//}
//void crRequestTaskAwardInfoPacket::buildReplyPacket(crRequestTaskAwardInfoPacket &packet,int playerid,int roleid,int taskid,crAbstractItem *aitem)
//{
//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
//	stream->createBuf(1024);
//	stream->_writeInt(playerid);
//	stream->_writeInt(roleid);
//	stream->_writeInt(taskid);
//	int count = 0;
//	std::vector< ref_ptr<crStreamBuf> > BufVec;
//	writeAbstractItem(aitem,stream,BufVec,count);
//	packet.setStreamBuf(stream.get());
//}
//void crRequestTaskAwardInfoPacket::buildReplyPacket2(crRequestTaskAwardInfoPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crRequestTaskAwardInfoPacket::parsePacket(const std::string &sender)
//{//id name
//	if(!m_netConductor) return;
//
//	int netType = m_netConductor->getNetType();
//	if(netType == GameConnectServer)
//	{//转发到GameServer
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *conductor = playerData->getSceneServerLineConductor();
//			if(conductor)
//			{
//				crRequestTaskAwardInfoPacket packet;
//				crRequestTaskAwardInfoPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
//				conductor->getNetManager()->sendPacket("all",packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crRequestTaskAwardInfoPacket "<<std::endl;
//			}
//		}
//	}
//	else if(netType == SceneServer)
//	{//SceneServer不连数据库
//		int sendid = m_streamBuf->_readInt();
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
//		if(playerData.valid())
//		{
//			//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
//			crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
//			crNetManager *netManager = m_netConductor->getNetManager();
//			int roleid = m_streamBuf->_readInt();
//			int taskid = m_streamBuf->_readInt();
//			void *param;
//			crRole *role = playerData->getRole(roleid);
//			crData *metierData = role->getMetierDataClass();
//			metierData->excHandle(MAKEINT64(WCH_LockData,1));
//			metierData->getParam(WCHDATA_GameTaskMap,param);
//			GameTaskMap* gameTaskMap = (GameTaskMap* )param;
//			GameTaskMap::iterator itr = gameTaskMap->find(taskid);
//			ref_ptr<crGameTask> gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
//			if(gameTask.valid())
//			{
//				AwardItemMap &awardItemMap = gameTask->getAwardItemMap();
//				int abstractid;
//				crInstanceItem* item;
//				crAbstractItem* aitem;
//				crData *data;
//				crRequestTaskAwardInfoPacket packet;
//				for( AwardItemMap::iterator itr = awardItemMap.begin();
//					itr != awardItemMap.end();
//					++itr)
//				{
//					abstractid = itr->first;
//					if(abstractid<=0)
//						continue;
//					item = itr->second.get();
//					if(!item)
//					{
//						aitem = crGlobalHandle::getInstance()->findAbstractItem(abstractid);
//						if(aitem)
//						{
//							item = new crInstanceItem;
//							item->setAbstractItem(aitem);
//							data = CREncapsulation::loadData(aitem->getScript());
//							if(data)
//							{
//								item->setDataClass(data);
//								//data->excHandle(MAKEINT64(WCH_InitData,NULL));
//							}
//							itr->second = item;
//						}
//						else
//						{
//							crRequestTaskAwardInfoPacket::buildRequestPacket3(packet,abstractid,sendid,roleid,taskid);
//							gameServerConductor->getNetManager()->sendPacket("all",packet);
//							continue;
//						}
//					}
//					crRequestTaskAwardInfoPacket::buildReplyPacket(packet,sendid,roleid,taskid,item->getAbstractItem());
//					netManager->sendPacket(sender,packet);
//				}
//			}
//			metierData->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//	}
//	else if(netType == GameServer)
//	{
//		int abstractid = m_streamBuf->_readInt();
//		crAbstractItem *abstractItem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
//		crRequestTaskAwardInfoPacket packet;
//		int sendid = m_streamBuf->_readInt();
//		int roleid = m_streamBuf->_readInt();
//		int taskid = m_streamBuf->_readInt();
//		crRequestTaskAwardInfoPacket::buildReplyPacket(packet,sendid,roleid,taskid,abstractItem);
//		m_netConductor->getNetManager()->sendPacket(sender,packet);
//	}
//	else if(netType == SceneServerClient_Game)
//	{
//		int sendid = m_streamBuf->_readInt();
//		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
//		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
//		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
//		if(playerData.valid())
//		{
//			//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
//			crNetManager *netManager = sceneServerConductor->getNetManager();
//			int roleid = m_streamBuf->_readInt();
//			int taskid = m_streamBuf->_readInt();
//			ref_ptr<crInstanceItem> item = new crInstanceItem;
//			ref_ptr<crAbstractItem> aitem; 
//			readAbstractItem(aitem,m_streamBuf.get());
//			crGlobalHandle::getInstance()->insertAbstractItem(aitem.get());
//			item->setAbstractItem(aitem.get());
//			crData *data = CREncapsulation::loadData(aitem->getScript());
//			if(data)
//			{
//				item->setDataClass(data);
//				//data->excHandle(MAKEINT64(WCH_InitData,NULL));
//			}
//
//			void *param;
//			crRole *role = playerData->getRole(roleid);
//			crData *metierData = role->getMetierDataClass();
//			metierData->excHandle(MAKEINT64(WCH_LockData,1));
//			metierData->getParam(WCHDATA_GameTaskMap,param);
//			GameTaskMap* gameTaskMap = (GameTaskMap* )param;
//			GameTaskMap::iterator itr = gameTaskMap->find(taskid);
//			ref_ptr<crGameTask> gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
//			if(gameTask.valid())
//			{
//				gameTask->setAwardItem(item.get());
//			}
//			metierData->excHandle(MAKEINT64(WCH_LockData,0));
//			m_streamBuf->seekBegin();
//			netManager->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
//		}
//	}
//	else if(netType == GameConnectServerClient_Scene)
//	{//转发到Client
//		int playerid = m_streamBuf->_readInt();
//		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
//		if(playerData.valid())
//		{
//			crRequestTaskAwardInfoPacket packet;
//			crRequestTaskAwardInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
//			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//		}
//	}
//	else if(netType == GameClient_Game)
//	{
//		int roleid = m_streamBuf->_readInt();
//		int taskid = m_streamBuf->_readInt();
//		ref_ptr<crInstanceItem> item = new crInstanceItem;
//		ref_ptr<crAbstractItem> aitem; 
//		readAbstractItem(aitem,m_streamBuf.get());
//		item->setAbstractItem(aitem.get());
//		crData *data = CREncapsulation::loadData(aitem->getScript());
//		if(data)
//		{
//			item->setDataClass(data);
//			//data->excHandle(MAKEINT64(WCH_InitData,NULL));
//		}
//
//		void *param;
//		crRole *role = crMyPlayerData::getInstance()->getRole(roleid);
//		crData *metierData = role->getMetierDataClass();
//		metierData->excHandle(MAKEINT64(WCH_LockData,1));
//		metierData->getParam(WCHDATA_GameTaskMap,param);
//		GameTaskMap* gameTaskMap = (GameTaskMap* )param;
//		GameTaskMap::iterator itr = gameTaskMap->find(taskid);
//		ref_ptr<crGameTask> gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
//		if(gameTask.valid())
//		{
//			gameTask->setAwardItem(item.get());
//			if(!gameTask->isRequestAwardItem())
//			{
//				crWaitNetReturnStreamLogic::netReturn(NULL);
//			}
//		}
//		metierData->excHandle(MAKEINT64(WCH_LockData,0));
//	}
//}
/////////////////////////////////////////
//
//crPlayerEventPacket
//
/////////////////////////////////////////
crPlayerEventPacket::crPlayerEventPacket():
crStreamPacket(crPlayerEventPacket::ID)
{
}

crPlayerEventPacket::crPlayerEventPacket( const crPlayerEventPacket &p ):
crStreamPacket(crPlayerEventPacket::ID,p)
{
}

//void crPlayerEventPacket::buildRequestPacket(crPlayerEventPacket &packet, int roleid, _crInt64 msg, CRCore::crStreamBuf *param)
//{//client send to server
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(12+(param?param->getBufSize():0));
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(roleid);
//	streamBuf->_writeInt64(msg);
//	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
//}
void crPlayerEventPacket::buildRequestPacket(crPlayerEventPacket &packet, crInstanceItem *role, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(role->getID());
	streamBuf->_writeInt(role->getRoleID());
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
//void crPlayerEventPacket::buildRequestPacket(crPlayerEventPacket &packet, int recvid, int playerid, int roleid, _crInt64 msg, CRCore::crStreamBuf *param)
//{//server send to client
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(20+(param?param->getBufSize():0));
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(recvid);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(roleid);
//	streamBuf->_writeInt64(msg);
//	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
//}
void crPlayerEventPacket::buildRequestPacket(crPlayerEventPacket &packet, int recvid, crInstanceItem *role, _crInt64 msg, CRCore::crStreamBuf *param)
{//server send to client
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(20+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(role->getID());
	streamBuf->_writeInt(role->getRoleID());
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerEventPacket::buildRequestPacket2(crPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

//void crPlayerEventPacket::buildReplyPacket(crPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+(stream?stream->getBufSize():0));
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
//}

void crPlayerEventPacket::buildReplyPacket2(crPlayerEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerEventPacket packet;
				crPlayerEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		int id = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			ref_ptr<crRole> role;
			if(id != playerid)
			{
				if(id<0)
				{
					crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
					crScene *scene = netCallback->findScene(netPlayerData->getSceneID());
					role = dynamic_cast<crRole *>(scene->findSceneItem(id,netPlayerData->getRoomID()));
				}
				//else
				//{
				//	ref_ptr<crSceneServerPlayerData> controlPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
				//	if(controlPlayerData.valid())
				//	{
				//		role = controlPlayerData->getRole(roleid);
				//	}
				//}
			}
			else
			{
				role = netPlayerData->getRole(roleid);
			}
			if(role.valid())
			{
				//if(msg == WCH_RecvPathFinding)
				//{
				//	char *ptr = m_streamBuf->getPtr();
				//	m_streamBuf->seekEnd();
				//	m_streamBuf->seek(-2);
				//	m_streamBuf->_writeShort(netPlayerData->getDelayTime());
				//	char *ptr2 = m_streamBuf->getPtr();
				//	m_streamBuf->seek(ptr - ptr2);
				//}
				//crPlayerEventPacket packet;
				//crPlayerEventPacket::buildReplyPacket(packet,0,m_streamBuf.get());
				_crInt64 msg = m_streamBuf->_readInt64();
				role->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(role->getSceneID());
				if(scene)
					scene->sendPacketToItemNeighbor(role.get(),*this);
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerEventPacket packet;
			crPlayerEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		_crInt64 msg = m_streamBuf->_readInt64();
		if(playerid == crMyPlayerData::getInstance()->getPlayerID())
		{
			ref_ptr<crRole> role = crMyPlayerData::getInstance()->getRole(roleid);
			if(role.valid()) role->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
		else
		{
			crMyPlayerData::RoleNpcPair roleNpcPair;
			if(crMyPlayerData::getInstance()->findInRangePlayer(playerid,roleid,roleNpcPair))
			{
				roleNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			//else if(crMyPlayerData::getInstance()->findOutRangePlayer(playerid,roleNpcPair))
			//{
			//	roleNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			//}
		}
	}
}
/////////////////////////////////////////
//
//crItemEventPacket
//
/////////////////////////////////////////
crItemEventPacket::crItemEventPacket():
crStreamPacket(crItemEventPacket::ID)
{
}

crItemEventPacket::crItemEventPacket( const crItemEventPacket &p ):
crStreamPacket(crItemEventPacket::ID,p)
{
}

void crItemEventPacket::buildRequestPacket(crItemEventPacket &packet, int recvid,crInstanceItem *item, _crInt64 msg, CRCore::crStreamBuf *param)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(21+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
		streamBuf->_writeInt(item->getRoleID());
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}

void crItemEventPacket::buildReplyPacket(crItemEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crItemEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
    if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crItemEventPacket packet;
			crItemEventPacket::buildReplyPacket(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		int roleid = itemtype == crInstanceItem::Role?m_streamBuf->_readInt():0;
		_crInt64 msg = m_streamBuf->_readInt64();
		if(itemtype == crInstanceItem::Role)
		{
			if(id == crMyPlayerData::getInstance()->getPlayerID())
			{
				ref_ptr<crRole> role = crMyPlayerData::getInstance()->getRole(roleid);
				if(role.valid()) role->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			else
			{
				crMyPlayerData::RoleNpcPair roleNpcPair;
				if(crMyPlayerData::getInstance()->findInRangePlayer(id,roleid,roleNpcPair))
				{
					roleNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				}
				//else if(crMyPlayerData::getInstance()->findOutRangePlayer(id,roleNpcPair))
				//{
				//	roleNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				//}
			}
		}
		else if(itemtype == crInstanceItem::Npc)
		{
			crMyPlayerData::ItemNpcPair itemNpcPair;
			if(crMyPlayerData::getInstance()->findInRangeNpc(id,itemNpcPair))
			{
				itemNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			else if(crMyPlayerData::getInstance()->findOutRangeNpc(id,itemNpcPair))
			{
				itemNpcPair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
		else if(itemtype == crInstanceItem::Emporium)
		{
			crInstanceItem *emporium = crMyPlayerData::getInstance()->getScene()->getEmporium();
			if(emporium)
			{
				emporium->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
		else
		{
			crMyPlayerData::ItemNodePair itemNodePair;
			if(crMyPlayerData::getInstance()->findInRangeItem(id,itemNodePair))
			{
				itemNodePair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			else if(crMyPlayerData::getInstance()->findOutRangeItem(id,itemNodePair))
			{
				itemNodePair.first->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
	}
}
/////////////////////////////////////////
//
//crSetClientRunningPacket
//
/////////////////////////////////////////
crSetClientRunningPacket::crSetClientRunningPacket():
crStreamPacket(crSetClientRunningPacket::ID)
{
}

crSetClientRunningPacket::crSetClientRunningPacket( const crSetClientRunningPacket &p ):
crStreamPacket(crSetClientRunningPacket::ID,p)
{
}

void crSetClientRunningPacket::buildRequestPacket(crSetClientRunningPacket &packet,bool runflg)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(runflg);
}

void crSetClientRunningPacket::buildRequestPacket2(crSetClientRunningPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crSetClientRunningPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crSetClientRunningPacket packet;
				crSetClientRunningPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crSetClientRunningPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			bool running = m_streamBuf->_readBool();
			//if(running && playerData->getRoomID()!=0)
			//{
			//	crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			//	crRoom* room = callback->findRoom(playerData->getRoomID());
			//	if(room)
			//	{
			//		playerData->getPlayerGameData()->doEvent(WCH_PlayerJoinRoom);
			//	}
			//}
			//crCheckPlayerDelayTimePacket packet;
			//crCheckPlayerDelayTimePacket::buildReplyPacket(packet,playerid);
			//m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			playerData->setClientRunning(running);
		}
	}
}
/////////////////////////////////////////
//
//crRemovePlayerPacket
//
/////////////////////////////////////////
crRemovePlayerPacket::crRemovePlayerPacket():
crStreamPacket(crRemovePlayerPacket::ID)
{
}
crRemovePlayerPacket::crRemovePlayerPacket( const crRemovePlayerPacket &p ):
crStreamPacket(crRemovePlayerPacket::ID,p)
{
}
void crRemovePlayerPacket::buildRequestPacket(crRemovePlayerPacket &packet,int recvid,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
}
void crRemovePlayerPacket::buildRequestPacket2(crRemovePlayerPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRemovePlayerPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData>playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(recvid));
		if(playerData.valid())
		{
			crRemovePlayerPacket packet;
			crRemovePlayerPacket::buildRequestPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crMyPlayerData::getInstance()->removeInRangePlayer(playerid);
		//crMyPlayerData::RoleNpcPair* roleNpcPair;
		//if(crMyPlayerData::getInstance()->findInRangePlayer(playerid,roleNpcPair))
		//{
		//	roleNpcPair->second->getNPCBot()->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_NodeOutRange,crGlobalHandle::gData()->gItemVisiableDelay()));//隐藏
		//	crMyPlayerData::getInstance()->removeInRangePlayer(playerid);
		//}
		//CRCore::notify(CRCore::ALWAYS)<<"crRemovePlayerPacket "<<std::endl;
	}
}
/////////////////////////////////////////
//
//crSaveItemDataPacket
//
/////////////////////////////////////////
crSaveItemDataPacket::crSaveItemDataPacket():
crStreamPacket(crSaveItemDataPacket::ID)
{
}

crSaveItemDataPacket::crSaveItemDataPacket( const crSaveItemDataPacket &p ):
crStreamPacket(crSaveItemDataPacket::ID,p)
{
}

void crSaveItemDataPacket::buildRequestPacket(crSaveItemDataPacket &packet,crInstanceItem *item,CRCore::crStreamBuf *itemStream,unsigned short gamedbid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(itemStream)
	    streamBuf->createBuf(itemStream->getBufSize()+41);
	else
	    streamBuf->createBuf(41);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(item->getItemtype());
    streamBuf->_writeUShort(gamedbid);
	streamBuf->_writeInt(item->getInstanceItemID());
	streamBuf->_writeInt(item->getSceneID());
	streamBuf->_writeUShort(item->getLayerID());
	streamBuf->_writeInt(item->getPosx());
	streamBuf->_writeInt(item->getPosy());
	streamBuf->_writeFloat(item->getDirx());
	streamBuf->_writeFloat(item->getDiry());
	streamBuf->_writeFloat(item->getDirz());
	if(itemStream)
	{
	    streamBuf->write(itemStream->getBuf(),itemStream->getBufSize());
	}
}
void crSaveItemDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		unsigned short gamedbid = m_streamBuf->_readUShort();
		//crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
		//if(gamedb)
		//{
			int instanceitemid = m_streamBuf->_readInt();
            int sceneid = m_streamBuf->_readInt();
			unsigned short layerid = m_streamBuf->_readUShort();
			int posx = m_streamBuf->_readInt();
			int posy = m_streamBuf->_readInt();
			float dirx = m_streamBuf->_readFloat();
			float diry = m_streamBuf->_readFloat();
			float dirz = m_streamBuf->_readFloat();
			//ref_ptr<crDataBase> gameSession = gamedb->beginSession();
			if(m_streamBuf->getRemainSize()>0)
			{
				ref_ptr<crUpdateInstanceItem> updateItem = new crUpdateInstanceItem;
				updateItem->buildUpdateSql(instanceitemid,sceneid,layerid,posx,posy,dirx,diry,dirz,m_streamBuf.get());
				crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
				crDBSavePacket packet;
				crDBSavePacket::buildRequestPacket(packet,GameDB,gamedbid,updateItem.get());
				dbConductor->getNetManager()->sendPacket("all",packet);
			}
			else
			{
				std::string logdata = "SaveItemDataPacket Faild,stream is NULL,instanceitemid=" + crArgumentParser::appItoa(instanceitemid)+" gamedbid="+crArgumentParser::appItoa(gamedbid);
				GameLogData gamelog(0,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(0,&gamelog));
				CRCore::notify(CRCore::ALWAYS)<<"SaveItemDataPacket Faild,stream is NULL,instanceitemid="<<instanceitemid<<std::endl;
			}
		//	if(gameSession->executeUpdate(updateItem.get()))
		//	{
		//		gameSession->commit();
		//		CRCore::notify(CRCore::ALWAYS)<<"crSaveItemDataPacket instanceitemid = "<<instanceitemid<<std::endl;
		//	}
  //          gamedb->endSession(gameSession.get());
		//}
	}
}
/////////////////////////////////////////
//
//crSaveItemStreamPacket
//
/////////////////////////////////////////
crSaveItemStreamPacket::crSaveItemStreamPacket():
	crStreamPacket(crSaveItemStreamPacket::ID)
{
}

crSaveItemStreamPacket::crSaveItemStreamPacket( const crSaveItemStreamPacket &p ):
	crStreamPacket(crSaveItemStreamPacket::ID,p)
{
}
void crSaveItemStreamPacket::buildRequestPacket(crSaveItemStreamPacket &packet,int itemid,CRCore::crStreamBuf *itemStream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(itemStream->getBufSize()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(itemid);
	streamBuf->write(itemStream->getBuf(),itemStream->getBufSize());
}
void crSaveItemStreamPacket::buildRequestPacket2(crSaveItemStreamPacket &packet, int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
//void crSaveItemStreamPacket::buildReplyPacket(crSaveItemStreamPacket &packet,int playerid,char code)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(5);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeChar(code);
//}
//void crSaveItemStreamPacket::buildReplyPacket2(crSaveItemStreamPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
void crSaveItemStreamPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crSaveItemStreamPacket packet;
				crSaveItemStreamPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		//char code = 0;
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			//crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
			//if(gamedb)
			//{
				int itemid = m_streamBuf->_readInt();
				//ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				if(m_streamBuf->getRemainSize()>0)
				{
					ref_ptr<crUpdateInstanceItem> updateItem = new crUpdateInstanceItem;
					updateItem->buildUpdateSql(itemid,m_streamBuf.get());
					crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
					crDBSavePacket packet;
					crDBSavePacket::buildRequestPacket(packet,GameDB,playerData->getGameDBID(),updateItem.get());
					dbConductor->getNetManager()->sendPacket("all",packet);
				}
				else
				{
					std::string logdata = "SaveItemStreamPacket Faild,stream is NULL,instanceitemid=" + crArgumentParser::appItoa(itemid)+" gamedbid="+crArgumentParser::appItoa(playerData->getGameDBID());
					GameLogData gamelog(0,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
					CRCore::notify(CRCore::ALWAYS)<<"SaveItemDataPacket Faild,stream is NULL,instanceitemid="<<itemid<<std::endl;
				}
			//	if(gameSession->executeUpdate(updateItem.get()))
			//	{
			//		gameSession->commit();
			//		//code = 1;
			//		//CRCore::notify(CRCore::ALWAYS)<<"crSaveItemStreamPacket instanceitemid = "<<itemid<<std::endl;
			//	}
			//	gamedb->endSession(gameSession.get());
			//}
		}
		//crSaveItemStreamPacket packet;
		//crSaveItemStreamPacket::buildReplyPacket(packet,playerid,code);
		//m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	//else if(netType == GameConnectServerClient_Game)
	//{
	//	int playerid = m_streamBuf->_readInt();
	//	crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
	//	ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
	//	if(playerData.valid())
	//	{
	//		crSaveItemStreamPacket packet;
	//		crSaveItemStreamPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//		connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//	}
	//}
	//else if(netType == GameClient_Game)
	//{
	//	crGlobalHandle::getInstance()->doEvent(WCH_NetReturnStream,MAKEINT64(m_streamBuf.get(),NULL));
	//}
}
/////////////////////////////////////////
//
//crSaveRoleDataPacket
//
/////////////////////////////////////////
crSaveRoleDataPacket::crSaveRoleDataPacket():
crStreamPacket(crSaveRoleDataPacket::ID)
{
}

crSaveRoleDataPacket::crSaveRoleDataPacket( const crSaveRoleDataPacket &p ):
crStreamPacket(crSaveRoleDataPacket::ID,p)
{
}

void crSaveRoleDataPacket::buildRequestPacket(crSaveRoleDataPacket &packet,crRole *role,CRCore::crStreamBuf *metierstream,unsigned short gamedbid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(metierstream)
		streamBuf->createBuf(metierstream->getBufSize()+6);
	else
		streamBuf->createBuf(6);
	packet.setStreamBuf(streamBuf);
	//streamBuf->_writeInt(playerid);
	streamBuf->_writeUShort(gamedbid);
	streamBuf->_writeInt(role->getRoleID());
	if(metierstream)
	{
		streamBuf->write(metierstream->getBuf(),metierstream->getBufSize());
	}
}
void crSaveRoleDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		//int playerid = m_streamBuf->_readInt();
	    unsigned short gamedbid = m_streamBuf->_readUShort();
		//crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
		//if(gamedb)
		//{
			int roleid = m_streamBuf->_readInt();
			if(m_streamBuf->getRemainSize()>0)
			{
				//ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crUpdateRole> updateRole = new crUpdateRole;
				updateRole->buildUpdateSql(roleid,m_streamBuf.get());
				crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
				crDBSavePacket packet;
				crDBSavePacket::buildRequestPacket(packet,GameDB,gamedbid,updateRole.get());
				dbConductor->getNetManager()->sendPacket("all",packet);
				//	if(gameSession->executeUpdate(updateRole.get()))
				//	{
				//		gameSession->commit();
				//		CRCore::notify(CRCore::ALWAYS)<<"crSaveRoleDataPacket roleid = "<<roleid<<std::endl;
				//	}
				//	gamedb->endSession(gameSession.get());
				//}
			}
			else
			{
				std::string logdata = "SaveRoleDataPacket Faild,stream is NULL,roleid=" + crArgumentParser::appItoa(roleid)+" gamedbid="+crArgumentParser::appItoa(gamedbid);
				GameLogData gamelog(0,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(0,&gamelog));
				CRCore::notify(CRCore::ALWAYS)<<"Save SaveRoleDataPacket Faild,stream is NULL,roleid="<<roleid<<std::endl;
			}
	}
}

/////////////////////////////////////////
//
//crCreateItemChildPacket
//
/////////////////////////////////////////
crCreateItemChildPacket::crCreateItemChildPacket():
crStreamPacket(crCreateItemChildPacket::ID)
{
}

crCreateItemChildPacket::crCreateItemChildPacket( const crCreateItemChildPacket &p ):
crStreamPacket(crCreateItemChildPacket::ID,p)
{
}

void crCreateItemChildPacket::buildRequestPacket(crCreateItemChildPacket &packet,crInstanceItem *item, int parentitemtid, int abstractitemid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(26);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
	{
		streamBuf->_writeInt((dynamic_cast<crRole *>(item))->getRoleID());
	}
	streamBuf->_writeInt(item->getSceneID());
	streamBuf->_writeInt(item->getRoomID());
	streamBuf->_writeInt(parentitemtid);
	streamBuf->_writeInt(abstractitemid);
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	crAbstractItem *aitem = crGlobalHandle::getInstance()->findAbstractItem(abstractitemid);
	streamBuf->_writeBool(aitem==NULL);
}

//void crCreateItemChildPacket::buildReplyPacket(crCreateItemChildPacket &packet,int playerid, int streamid, int count)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(12);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(streamid);
//	streamBuf->_writeInt(count);
//}
//
//void crCreateItemChildPacket::buildReplyPacket2(crCreateItemChildPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}

void crCreateItemChildPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		unsigned short gamedbid = 0;
		int id = m_streamBuf->_readInt();
		int roleid = 0;
		if(itemtype == crInstanceItem::Role)
		{
			roleid = m_streamBuf->_readInt();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(id));
			if(!playerData.valid()) 
				return;
			gamedbid = playerData->getGameDBID();
		}
		int sceneid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		int parentid = m_streamBuf->_readInt();
		int abstractitemid = m_streamBuf->_readInt();
		crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
		if(gamedb)
		{
			ref_ptr<crDataBase> gameSession = gamedb->beginSession();
			bool game_ended = false;
			ref_ptr<crCreateInstanceItem> createInstanceItem = new crCreateInstanceItem;
			createInstanceItem->buildUpdateSql(abstractitemid,crInstanceItem::instanceitem);
			if(gameSession->executeUpdate(createInstanceItem.get()))
			{//插入成功
				int instanceitemid = gameSession->getInsertID();
				ref_ptr<crCreateItemChild> createItemChild = new crCreateItemChild;
				createItemChild->buildUpdateSql(instanceitemid,parentid);
				if(gameSession->executeUpdate(createItemChild.get()))
				{
					int itemchildid = gameSession->getInsertID();
					gameSession->commit();

					gamedb->endSession(gameSession.get());
					game_ended = true;

					crCreateItemChildPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(224);
					stream->_writeUChar(itemtype);
					stream->_writeInt(id);
					if(itemtype == crInstanceItem::Role)
						stream->_writeInt(roleid);
					stream->_writeInt(sceneid);
					stream->_writeInt(roomid);
					stream->_writeInt(itemchildid);
					stream->_writeInt(instanceitemid);
					stream->_writeInt(parentid);
					stream->_writeInt(abstractitemid);
					bool queryAbstract = m_streamBuf->_readBool();
					stream->_writeBool(queryAbstract);
					if(queryAbstract)
					{
						crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractitemid);
						stream->_writeString(aitem->getName());//24
						stream->_writeString(aitem->getScript());//54
						stream->_writeInt(aitem->getResID());
						stream->_writeString(aitem->getFileName());//54
						//stream->_writeString(aitem->getUpdatecfg());//54
						stream->_writeString(aitem->getResScript());//54
					}
					packet.setStreamBuf(stream.get());
                    m_netConductor->getNetManager()->sendPacket(sender,packet);
					//CRCore::notify(CRCore::ALWAYS)<<"crCreateItemChildPacket sucess "<<std::endl;
				}
				else
				{
					gameSession->rollback();
				}
			}
			if(!game_ended) gamedb->endSession(gameSession.get());
		}
		//}
		//else
		//{
		//	CRCore::notify(CRCore::ALWAYS)<<"crCreateItemChildPacket 失败 itemscript 为空abstractid = "<<abstractitemid<<std::endl;
		//}
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		int roleid = 0;
		if(itemtype == crInstanceItem::Role)
		{
			roleid = m_streamBuf->_readInt();
		}
		int sceneid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
        ref_ptr<crInstanceItem> item;
		ref_ptr<crSceneServerPlayerData> playerData;
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		if(itemtype == crInstanceItem::Role)
		{
			playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
			if(playerData.valid())
				item = playerData->getRole(roleid);
		}
		else
		{
			crScene *scene = netCallback->findScene(sceneid);
			if(scene)
				item = scene->findSceneItem(id,roomid);
		}
		if(item.valid())
		{
			crData *itemData = item->getDataClass();
			if(itemData)
			{
				int itemchildid = m_streamBuf->_readInt();
				int childitemid = m_streamBuf->_readInt();
				int parentid = m_streamBuf->_readInt();

				crInstanceItem *parentItem = NULL;
				if(parentid == item->getInstanceItemID())
				{
					parentItem = item.get();
				}
				else
				{
					crItemChild *itemchild = item->findChildItem(parentid);
					if(itemchild)
						parentItem = itemchild->getInstanceItem();
				}
				if(parentItem)
				{
					ref_ptr<crItemChild> itemChild = new crItemChild;
					itemChild->setItemChildID(itemchildid);
					itemChild->setInstanceItemID(childitemid);
					itemChild->setParentItem(parentItem);

                    ref_ptr<crInstanceItem> childItem = new crInstanceItem;
					childItem->setInstanceItemID(childitemid);
					int abstractitemid = m_streamBuf->_readInt();
					bool queryAbstract = m_streamBuf->_readBool();
					if(queryAbstract)
					{
						childItem->setAbstractItemID(abstractitemid);
						childItem->getAbstractItem()->setName(m_streamBuf->_readString());
						childItem->getAbstractItem()->setScript(m_streamBuf->_readString());
						childItem->getAbstractItem()->setResID(m_streamBuf->_readInt());
						childItem->getAbstractItem()->setFileName(m_streamBuf->_readString());
						//childItem->getAbstractItem()->setUpdatecfg(m_streamBuf->_readString());
						childItem->getAbstractItem()->setResScript(m_streamBuf->_readString());
						crGlobalHandle::getInstance()->insertAbstractItem(childItem->getAbstractItem());
					}
					else
					{
						crAbstractItem *aitem = crGlobalHandle::getInstance()->findAbstractItem(abstractitemid);
						childItem->setAbstractItem(aitem);
					}
					itemChild->setInstanceItem(childItem.get());
					childItem->setSceneID(sceneid);
					childItem->setRoomID(roomid);
					//parentItem->insertChildItem(itemChild.get());
					crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild,MAKEINT64(itemChild.get(),parentItem));
					//crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild,MAKEINT64(NULL,NULL));//清理
                    parentItem->recvItemChild(itemChild.get());
					//CRCore::notify(CRCore::ALWAYS)<<"crCreateItemChildPacket abstractitemid = "<<abstractitemid<<" queryAbstract = "<<(int)queryAbstract<<std::endl;
					//crNetManager *netManager = sceneServerConductor->getNetManager();
					///////////write and send
					//int count = 0;
					//std::vector< ref_ptr<crStreamBuf> > BufVec;
					//ref_ptr<crStreamBuf> stream = new crStreamBuf;
					//stream->createBuf(MAXBUFFERSIZE-12);
					//stream->_writeUChar(itemtype);
					//stream->_writeInt(id);
					//stream->_writeInt(itemchildid);
					//writeItem(childItem,stream,BufVec,count);
					//crCreateItemChildPacket packet;
					//crCreateItemChildPacket::buildReplyPacket(packet,id,parentid,count);
					//if(itemtype == crInstanceItem::Role)
					//{
					//	netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					//	//同步给范围内的其它玩家.
					//	playerData->sendPacketToNeighbor(packet);
					//	crDataStreamPacket streamPacket;
					//	for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					//		itr != BufVec.end();
					//		++itr )
					//	{
					//		crDataStreamPacket::buildRequestPacket(streamPacket,id,parentid,crCreateItemChildPacket::ID,itr->get());
					//		netManager->sendPacket(playerData->getPlayerConnectServerAddress(),streamPacket);
					//		playerData->sendPacketToNeighbor(streamPacket);
					//	}
					//}
					//else
					//{
     //                   netCallback->sendPacketToItemNeighbor(item,packet);
					//	crDataStreamPacket streamPacket;
					//	for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					//		itr != BufVec.end();
					//		++itr )
					//	{
					//		crDataStreamPacket::buildRequestPacket(streamPacket,id,parentid,crCreateItemChildPacket::ID,itr->get());
					//		netCallback->sendPacketToItemNeighbor(item,streamPacket);
					//	}
					//}
					//CRCore::notify(CRCore::ALWAYS)<<"crCreateItemChildPacket"<<std::endl;
				}
				////////update stream & db
				//if(playerData)
				//{
				//	netCallback->savePlayerData(playerData,crSceneServerCallback::SaveItem);
				//}
				//else
				//{
				//	itemData->excHandle(MAKEINT64(WCH_BuildDataStream,NULL));
				//	void *param;
				//	itemData->getParam(WCHDATA_DataStream,param);
				//	ref_ptr<crStreamBuf> itemStream = *((ref_ptr<crStreamBuf>*)param);
				//	item->setStream(itemStream.get());
				//	crSaveItemDataPacket packet;
				//	crSaveItemDataPacket::buildRequestPacket(packet,item);
				//	m_netConductor->getNetManager()->sendPacket("all",packet);
				//}
			}
		}
	}
	//else if(netType == SceneConnectServerClient_Scene)
	//{
	//	int recvid = m_streamBuf->_readInt();
	//	crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(SceneConnectServer);
	//	if(connectServerConductor)
	//	{
	//		crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
	//		crSceneConnectServerPlayerData *playerData = dynamic_cast<crSceneConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
	//		if(playerData)
	//		{
	//			crCreateItemChildPacket packet;
	//			crCreateItemChildPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//			connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//			CRCore::notify(CRCore::ALWAYS)<<"crCreateItemChildPacket"<<std::endl;
	//		}
	//	}
	//}
	//else if(netType == GameClient_Game)
	//{//id name
	//	int streamid = m_streamBuf->_readInt();
	//	int count = m_streamBuf->_readInt();
	//	crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crCreateItemChildPacket::ID,count);
	//}
}
/////////////////////////////////////////
//
//crPlayerServerEventPacket
//
/////////////////////////////////////////
crPlayerServerEventPacket::crPlayerServerEventPacket():
crStreamPacket(crPlayerServerEventPacket::ID)
{
}

crPlayerServerEventPacket::crPlayerServerEventPacket( const crPlayerServerEventPacket &p ):
crStreamPacket(crPlayerServerEventPacket::ID,p)
{
}

//void crPlayerServerEventPacket::buildRequestPacket(crPlayerServerEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(8+param->getBufSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt64(msg);
//	streamBuf->write(param->getBuf(),param->getBufSize());
//}
void crPlayerServerEventPacket::buildRequestPacket(crPlayerServerEventPacket &packet, _crInt64 msg, crInstanceItem *item, CRCore::crStreamBuf *param)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	int size = 0;
	if(param) size = param->getBufSize();
	streamBuf->createBuf(17+size);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
		streamBuf->_writeInt(item->getRoleID());
	if(size > 0)
	    streamBuf->write(param->getBuf(),size);
}
void crPlayerServerEventPacket::buildRequestPacket2(crPlayerServerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerServerEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerServerEventPacket packet;
				crPlayerServerEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//可能会有安全性问题
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData>playerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			unsigned char itemtype = m_streamBuf->_readUChar();
			int id = m_streamBuf->_readInt();
			ref_ptr<crInstanceItem> item;
			if(itemtype == crInstanceItem::Role)
			{
				int roleid = m_streamBuf->_readInt();
				if(id == playerid)
				{
                    item = playerData->getRole(roleid);
				}
				else
				{
					if (id < 0)
					{
						crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
						crScene *scene = netCallback->findScene(playerData->getSceneID());
						item = dynamic_cast<crRole *>(scene->findSceneItem(id, playerData->getRoomID()));
					}
					else
					{
						ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(id));
						if (netPlayerData.valid())
						{
							item = netPlayerData->getRole(roleid);
						}
					}
				}
			}
			else if(itemtype == crInstanceItem::Emporium)
			{
				crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(playerData->getSceneID());
				if(scene)
					item = scene->getEmporium();
			}
			else
			{
				crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(playerData->getSceneID());
				if(scene)
					item = scene->findSceneItem(id,playerData->getRoomID());
			}
			if(item.valid())
			{
				item->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerEventPacket "<<std::endl;
			}
		}
	}
}
/////////////////////////////////////////
//
//crRequestItemRTDataPacket
//
/////////////////////////////////////////
crRequestItemRTDataPacket::crRequestItemRTDataPacket():
crStreamPacket(crRequestItemRTDataPacket::ID)
{
}

crRequestItemRTDataPacket::crRequestItemRTDataPacket( const crRequestItemRTDataPacket &p ):
crStreamPacket(crRequestItemRTDataPacket::ID,p)
{
}

void crRequestItemRTDataPacket::buildRequestPacket(crRequestItemRTDataPacket &packet,crInstanceItem *item)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
		streamBuf->_writeInt(item->getRoleID());
}

void crRequestItemRTDataPacket::buildRequestPacket2(crRequestItemRTDataPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestItemRTDataPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRequestItemRTDataPacket packet;
				crRequestItemRTDataPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemRTDataPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int sendid = m_streamBuf->_readInt();
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		if(itemtype == crInstanceItem::Role)
		{
			int roleid = m_streamBuf->_readInt();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
			if(playerData.valid())
			{
				ref_ptr<crStreamBuf> rtDataStream;
				ref_ptr<crRole> role = playerData->getRole(roleid);
				if(role.valid())
				{
					role->doEvent(WCH_GetItemRTData,MAKEINT64(&rtDataStream,NULL));
					crPlayerEventPacket packet;
					crPlayerEventPacket::buildRequestPacket(packet,sendid,role.get(),WCH_RecvItemRTData,rtDataStream.get());
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					//crRequestItemRTDataPacket packet;
					//crRequestItemRTDataPacket::buildReplyPacket(packet,sendid,itemtype,id,rtDataStream.get());
					//m_netConductor->getNetManager()->sendPacket(sender,packet);
				}
			}
		}
		else
		{
			ref_ptr<crSceneServerPlayerData> senderPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
            if(senderPlayerData.valid())
			{
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(senderPlayerData->getSceneID());
				if(scene)
				{
					ref_ptr<crInstanceItem> item = scene->findSceneItem(id,senderPlayerData->getRoomID());
					if(item.valid())
					{
						ref_ptr<crStreamBuf> rtDataStream;
						item->doEvent(WCH_GetItemRTData,MAKEINT64(&rtDataStream,NULL));
						crItemEventPacket packet;
						crItemEventPacket::buildRequestPacket(packet,sendid,item.get(),WCH_RecvItemRTData,rtDataStream.get());
						m_netConductor->getNetManager()->sendPacket(sender,packet);
						//crRequestItemRTDataPacket packet;
						//crRequestItemRTDataPacket::buildReplyPacket(packet,sendid,itemtype,id,rtDataStream.get());
						//m_netConductor->getNetManager()->sendPacket(sender,packet);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRequestItemChildPacket
//
/////////////////////////////////////////
crRequestItemChildPacket::crRequestItemChildPacket():
crStreamPacket(crRequestItemChildPacket::ID)
{
}

crRequestItemChildPacket::crRequestItemChildPacket( const crRequestItemChildPacket &p ):
crStreamPacket(crRequestItemChildPacket::ID,p)
{
}

void crRequestItemChildPacket::buildRequestPacket(crRequestItemChildPacket &packet,crInstanceItem *rootitem, int itemid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(13);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(rootitem->getItemtype());
	streamBuf->_writeInt(rootitem->getID());
	if(rootitem->getItemtype() == crInstanceItem::Role)
		streamBuf->_writeInt(rootitem->getRoleID());
	streamBuf->_writeInt(itemid);
}

void crRequestItemChildPacket::buildRequestPacket2(crRequestItemChildPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRequestItemChildPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRequestItemChildPacket packet;
				crRequestItemChildPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemRTDataPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int sendid = m_streamBuf->_readInt();
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		int roleid = itemtype == crInstanceItem::Role?m_streamBuf->_readInt():0;
		int itemid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		if(itemtype == crInstanceItem::Role)
		{
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
			if(playerData.valid())
			{
				ref_ptr<crStreamBuf> rtDataStream;
				ref_ptr<crRole> role = playerData->getRole(roleid);
				if(role.valid())
				{
					role->doEvent(WCH_GetItemChildData,MAKEINT64(&rtDataStream,itemid));

					crPlayerEventPacket packet;
					crPlayerEventPacket::buildRequestPacket(packet,sendid,role.get(),WCH_RecvItemChildData,rtDataStream.get());
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					//crRequestItemRTDataPacket packet;
					//crRequestItemRTDataPacket::buildReplyPacket(packet,sendid,itemtype,id,rtDataStream.get());
					//m_netConductor->getNetManager()->sendPacket(sender,packet);
				}
			}
		}
		else
		{
			ref_ptr<crSceneServerPlayerData> senderPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
			if(senderPlayerData.valid())
			{
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(senderPlayerData->getSceneID());
				if(scene)
				{
					ref_ptr<crInstanceItem>item = scene->findSceneItem(id,senderPlayerData->getRoomID());
					if(item.valid())
					{
						ref_ptr<crStreamBuf> rtDataStream;
						item->doEvent(WCH_GetItemChildData,MAKEINT64(&rtDataStream,itemid));
						crItemEventPacket packet;
						crItemEventPacket::buildRequestPacket(packet,sendid,item.get(),WCH_RecvItemChildData,rtDataStream.get());
						m_netConductor->getNetManager()->sendPacket(sender,packet);
						//crRequestItemRTDataPacket packet;
						//crRequestItemRTDataPacket::buildReplyPacket(packet,sendid,itemtype,id,rtDataStream.get());
						//m_netConductor->getNetManager()->sendPacket(sender,packet);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crPlayerServerMetierEventPacket
//
/////////////////////////////////////////
crPlayerServerMetierEventPacket::crPlayerServerMetierEventPacket():
crStreamPacket(crPlayerServerMetierEventPacket::ID)
{
}
crPlayerServerMetierEventPacket::crPlayerServerMetierEventPacket( const crPlayerServerMetierEventPacket &p ):
crStreamPacket(crPlayerServerMetierEventPacket::ID,p)
{
}
void crPlayerServerMetierEventPacket::buildRequestPacket(crPlayerServerMetierEventPacket &packet, int roleid, _crInt64 msg, CRCore::crStreamBuf *param)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerServerMetierEventPacket::buildRequestPacket2(crPlayerServerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerServerMetierEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerServerMetierEventPacket packet;
				crPlayerServerMetierEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerMetierEventPacket "<<playerData->getPlayerID()<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		_crInt64 msg = m_streamBuf->_readInt64();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crRole *role = netPlayerData->getRole(roleid);
			if(role) role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerMetierEventPacket "<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crPlayerMetierEventPacket
//
/////////////////////////////////////////
crPlayerMetierEventPacket::crPlayerMetierEventPacket():
crStreamPacket(crPlayerMetierEventPacket::ID)
{
}

crPlayerMetierEventPacket::crPlayerMetierEventPacket( const crPlayerMetierEventPacket &p ):
crStreamPacket(crPlayerMetierEventPacket::ID,p)
{
}

void crPlayerMetierEventPacket::buildRequestPacket(crPlayerMetierEventPacket &packet, int roleid, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerMetierEventPacket::buildRequestPacket(crPlayerMetierEventPacket &packet, int recvid, int playerid, int roleid, _crInt64 msg, CRCore::crStreamBuf *param)
{//server send to client
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(20+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerMetierEventPacket::buildRequestPacket2(crPlayerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerMetierEventPacket::buildReplyPacket(crPlayerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crPlayerMetierEventPacket::buildReplyPacket2(crPlayerMetierEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerMetierEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerMetierEventPacket packet;
				crPlayerMetierEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerMetierEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			int roleid = m_streamBuf->_readInt();
			crRole *role = netPlayerData->getRole(roleid);
			if(role)
			{
				crPlayerMetierEventPacket packet;
				crPlayerMetierEventPacket::buildReplyPacket(packet,0,m_streamBuf.get());
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				crScene *scene = netCallback->findScene(role->getSceneID());
				if(scene)
				{
					scene->sendPacketToItemNeighbor(role,packet);
				}
				_crInt64 msg = m_streamBuf->_readInt64();
				role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crPlayerMetierEventPacket "<<std::endl;
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerMetierEventPacket packet;
			crPlayerMetierEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		_crInt64 msg = m_streamBuf->_readInt64();
		if(playerid == crMyPlayerData::getInstance()->getPlayerID())
		{
			crRole *role = crMyPlayerData::getInstance()->getRole(roleid);
			if(role) role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
		else
		{
			crMyPlayerData::RoleNpcPair roleNpcPair;
			if(crMyPlayerData::getInstance()->findInRangePlayer(playerid,roleid,roleNpcPair))
			{
				roleNpcPair.first->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			//else if(crMyPlayerData::getInstance()->findOutRangePlayer(playerid,roleNpcPair))
			//{
			//	roleNpcPair.first->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			//}
		}
	}
}
/////////////////////////////////////////
//
//crQueryAbstractItemPacket
//
/////////////////////////////////////////
crQueryAbstractItemPacket::crQueryAbstractItemPacket():
crStreamPacket(crQueryAbstractItemPacket::ID)
{
}
crQueryAbstractItemPacket::crQueryAbstractItemPacket( const crQueryAbstractItemPacket &p ):
crStreamPacket(crQueryAbstractItemPacket::ID,p)
{
}
void crQueryAbstractItemPacket::buildRequestPacket(crQueryAbstractItemPacket &packet,int abstractid,crInstanceItem *item,int belongid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(32);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(item->getInstanceItemID());
	streamBuf->_writeInt(abstractid/*item->getAbstractItemID()*/);
	streamBuf->_writeInt(item->getSceneID());
	streamBuf->_writeInt(item->getRoomID());
	streamBuf->_writeInt(item->getLayerID());
	streamBuf->_writeInt(item->getPosx());
	streamBuf->_writeInt(item->getPosy());
	streamBuf->_writeInt(belongid);
}
void crQueryAbstractItemPacket::buildReplyPacket(crQueryAbstractItemPacket &packet,bool sucess,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9);
	packet.setStreamBuf(streamBuf);
    streamBuf->_writeBool(sucess);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crQueryAbstractItemPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int itemid = m_streamBuf->_readInt();
		int abstractid = m_streamBuf->_readInt();
		crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
		if(!aitem)
		{
			crQueryAbstractItemPacket packet;
			crQueryAbstractItemPacket::buildReplyPacket(packet,false,m_streamBuf->_readInt(),itemid);//无效的abstractid，通知sceneServer回收itemid
			return;
		}
		int count = 0;
		std::vector< ref_ptr<crStreamBuf> > BufVec;

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(MTUBUFFERSIZE);//(MAXBUFFERSIZE-12);
		stream->_writeInt(itemid);
		stream->write(m_streamBuf->getPtr(),m_streamBuf->getRemainSize());
		writeAbstractItem(aitem,stream,BufVec,count);
		if(stream->getBufSize()>0)
		{
			count += stream->getBufSize();
			BufVec.push_back(stream.get());
		}
		if(count>0)
		{
			int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
			crQueryAbstractItemPacket packet;
			crQueryAbstractItemPacket::buildReplyPacket(packet,true,streamid,count);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
			crDataStreamPacket streamPacket;
			for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
				itr != BufVec.end();
				++itr )
			{
				crDataStreamPacket::buildRequestPacket(streamPacket,0,streamid,crQueryAbstractItemPacket::ID,itr->get());
				m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
			}
			crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		bool sucess = m_streamBuf->_readBool();
		if(sucess)
		{
			int streamid = m_streamBuf->_readInt();
			int count = m_streamBuf->_readInt();
			crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crQueryAbstractItemPacket::ID,count);
		}
		else
		{
			int sceneid = m_streamBuf->_readInt();
			int itemid = m_streamBuf->_readInt();
			//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			//CRNetApp::crScene *scene = netCallback->findScene(sceneid);
			crGlobalHandle::recycleItemID(itemid);
		}
	}
}
/////////////////////////////////////////
//
//crCreateTemporaryItemChildPacket
//
/////////////////////////////////////////
crCreateTemporaryItemChildPacket::crCreateTemporaryItemChildPacket():
crStreamPacket(crCreateTemporaryItemChildPacket::ID)
{
}

crCreateTemporaryItemChildPacket::crCreateTemporaryItemChildPacket( const crCreateTemporaryItemChildPacket &p ):
crStreamPacket(crCreateTemporaryItemChildPacket::ID,p)
{
}

void crCreateTemporaryItemChildPacket::buildRequestPacket(crCreateTemporaryItemChildPacket &packet,crInstanceItem *item, int parentitemtid, int abstractitemid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(26);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeInt(item->getID());
	if(item->getItemtype() == crInstanceItem::Role)
	{
		streamBuf->_writeInt(item->getRoleID());
	}
	streamBuf->_writeInt(item->getSceneID());
	streamBuf->_writeInt(item->getRoomID());
	streamBuf->_writeInt(parentitemtid);
	streamBuf->_writeInt(abstractitemid);
	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	crAbstractItem *aitem = crGlobalHandle::getInstance()->findAbstractItem(abstractitemid);
	streamBuf->_writeBool(aitem==NULL);
}

//void crCreateTemporaryItemChildPacket::buildReplyPacket(crCreateTemporaryItemChildPacket &packet,int playerid, int streamid, int count)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(12);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(streamid);
//	streamBuf->_writeInt(count);
//}
//
//void crCreateTemporaryItemChildPacket::buildReplyPacket2(crCreateTemporaryItemChildPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}

void crCreateTemporaryItemChildPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
    if(netType == GameServer)
	{
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		int roleid = itemtype == crInstanceItem::Role?m_streamBuf->_readInt():0;
		int sceneid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		int parentid = m_streamBuf->_readInt();
		int abstractitemid = m_streamBuf->_readInt();
		bool queryAbstract = m_streamBuf->_readBool();
		crCreateTemporaryItemChildPacket packet;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(216);
		stream->_writeUChar(itemtype);
		stream->_writeInt(id);
		if(itemtype == crInstanceItem::Role)
		{
			stream->_writeInt(roleid);
		}
		stream->_writeInt(sceneid);
		stream->_writeInt(roomid);
		//stream->_writeInt(itemchildid);
		//stream->_writeInt(instanceitemid);
		stream->_writeInt(parentid);
		stream->_writeInt(abstractitemid);
		stream->_writeBool(queryAbstract);
		if(queryAbstract)
		{
			crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractitemid);
			if(aitem)
			{
				stream->_writeString(aitem->getName());//24
				stream->_writeString(aitem->getScript());//54
				stream->_writeInt(aitem->getResID());
				stream->_writeString(aitem->getFileName());//54
				//stream->_writeString(aitem->getUpdatecfg());//54
				stream->_writeString(aitem->getResScript());//54
			}
		}
		packet.setStreamBuf(stream.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		unsigned char itemtype = m_streamBuf->_readUChar();
		int id = m_streamBuf->_readInt();
		int roleid = itemtype == crInstanceItem::Role?m_streamBuf->_readInt():0;
		int sceneid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		ref_ptr<crInstanceItem> item;
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		crScene *scene = netCallback->findScene(sceneid);
		if(!scene) return;
		if(itemtype == crInstanceItem::Role)
		{
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(id));
			if(playerData.valid())
				item = playerData->getRole(roleid);
		}
		else if(itemtype == crInstanceItem::Emporium)
		{
			item = scene->getEmporium();
		}
		else
		{
			item = scene->findSceneItem(id,roomid);
		}
		if(item.get())
		{
			crData *itemData = item->getDataClass();
			if(itemData)
			{
				int itemchildid = crGlobalHandle::gainTemporaryItemChildID();
				int childitemid = crGlobalHandle::gainTemporaryItemID();
				int parentid = m_streamBuf->_readInt();

				crInstanceItem *parentItem = NULL;
				if(parentid == item->getInstanceItemID())
				{
					parentItem = item.get();
				}
				else
				{
					crItemChild *itemchild = item->findChildItem(parentid);
					if(itemchild) parentItem = itemchild->getInstanceItem();
				}
				if(parentItem)
				{
					ref_ptr<crItemChild> itemChild = new crItemChild;
					itemChild->setItemChildID(itemchildid);
					itemChild->setInstanceItemID(childitemid);
					itemChild->setParentItem(parentItem);

					ref_ptr<crInstanceItem> childItem = new crInstanceItem;
					childItem->setInstanceItemID(childitemid);
					int abstractitemid = m_streamBuf->_readInt();
					bool queryAbstract = m_streamBuf->_readBool();
					if(queryAbstract)
					{
						childItem->setAbstractItemID(abstractitemid);
						childItem->getAbstractItem()->setName(m_streamBuf->_readString());
						childItem->getAbstractItem()->setScript(m_streamBuf->_readString());
						childItem->getAbstractItem()->setResID(m_streamBuf->_readInt());
						childItem->getAbstractItem()->setFileName(m_streamBuf->_readString());
						//childItem->getAbstractItem()->setUpdatecfg(m_streamBuf->_readString());
						childItem->getAbstractItem()->setResScript(m_streamBuf->_readString());
						crGlobalHandle::getInstance()->insertAbstractItem(childItem->getAbstractItem());
					}
					else
					{
						crAbstractItem *aitem = crGlobalHandle::getInstance()->findAbstractItem(abstractitemid);
						childItem->setAbstractItem(aitem);
					}
					itemChild->setInstanceItem(childItem.get());
					childItem->setSceneID(sceneid);
					childItem->setRoomID(roomid);
					//parentItem->insertChildItem(itemChild.get());
					crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild,MAKEINT64(itemChild.get(),parentItem));
					//crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild,MAKEINT64(NULL,NULL));//清理
					parentItem->recvItemChild(itemChild.get());
					//CRCore::notify(CRCore::ALWAYS)<<"crCreateTemporaryItemChildPacket abstractitemid = "<<abstractitemid<<" queryAbstract = "<<(int)queryAbstract<<std::endl;

					//crNetManager *netManager = sceneServerConductor->getNetManager();
					///////////write and send
					//int count = 0;
					//std::vector< ref_ptr<crStreamBuf> > BufVec;
					//ref_ptr<crStreamBuf> stream = new crStreamBuf;
					//stream->createBuf(MAXBUFFERSIZE-12);
					//stream->_writeUChar(itemtype);
					//stream->_writeInt(id);
					//stream->_writeInt(itemchildid);
					//writeItem(childItem,stream,BufVec,count);
					//crCreateTemporaryItemChildPacket packet;
					//crCreateTemporaryItemChildPacket::buildReplyPacket(packet,id,parentid,count);
					//if(itemtype == crInstanceItem::Role)
					//{
					//	netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					//	//同步给范围内的其它玩家.
					//	playerData->sendPacketToNeighbor(packet);
					//	crDataStreamPacket streamPacket;
					//	for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					//		itr != BufVec.end();
					//		++itr )
					//	{
					//		crDataStreamPacket::buildRequestPacket(streamPacket,id,parentid,crCreateTemporaryItemChildPacket::ID,itr->get());
					//		netManager->sendPacket(playerData->getPlayerConnectServerAddress(),streamPacket);
					//		playerData->sendPacketToNeighbor(streamPacket);
					//	}
					//}
					//else
					//{
					//	netCallback->sendPacketToItemNeighbor(item,packet);
					//	crDataStreamPacket streamPacket;
					//	for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					//		itr != BufVec.end();
					//		++itr )
					//	{
					//		crDataStreamPacket::buildRequestPacket(streamPacket,id,parentid,crCreateTemporaryItemChildPacket::ID,itr->get());
					//		netCallback->sendPacketToItemNeighbor(item,streamPacket);
					//	}
					//}
					//CRCore::notify(CRCore::ALWAYS)<<"crCreateTemporaryItemChildPacket"<<std::endl;
				}
				////////update stream & db
				//if(playerData)
				//{
				//	netCallback->savePlayerData(playerData,crSceneServerCallback::SaveItem);
				//}
				//else
				//{
				//	itemData->excHandle(MAKEINT64(WCH_BuildDataStream,NULL));
				//	void *param;
				//	itemData->getParam(WCHDATA_DataStream,param);
				//	ref_ptr<crStreamBuf> itemStream = *((ref_ptr<crStreamBuf>*)param);
				//	item->setStream(itemStream.get());
				//	crSaveItemDataPacket packet;
				//	crSaveItemDataPacket::buildRequestPacket(packet,item);
				//	m_netConductor->getNetManager()->sendPacket("all",packet);
				//}
			}
		}
	}
	//else if(netType == SceneConnectServerClient_Scene)
	//{
	//	int recvid = m_streamBuf->_readInt();
	//	crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(SceneConnectServer);
	//	if(connectServerConductor)
	//	{
	//		crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
	//		crSceneConnectServerPlayerData *playerData = dynamic_cast<crSceneConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
	//		if(playerData)
	//		{
	//			crCreateTemporaryItemChildPacket packet;
	//			crCreateTemporaryItemChildPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//			connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//			CRCore::notify(CRCore::ALWAYS)<<"crCreateTemporaryItemChildPacket"<<std::endl;
	//		}
	//	}
	//}
	//else if(netType == GameClient_Game)
	//{//id name
	//	int streamid = m_streamBuf->_readInt();
	//	int count = m_streamBuf->_readInt();
	//	crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crCreateTemporaryItemChildPacket::ID,count);
	//}
}
/////////////////////////////////////////
//
//crRemoveItemChildPacket
//
/////////////////////////////////////////
crRemoveItemChildPacket::crRemoveItemChildPacket():
crStreamPacket(crRemoveItemChildPacket::ID)
{
}

crRemoveItemChildPacket::crRemoveItemChildPacket( const crRemoveItemChildPacket &p ):
crStreamPacket(crRemoveItemChildPacket::ID,p)
{
}

void crRemoveItemChildPacket::buildRequestPacket(crRemoveItemChildPacket &packet,crItemChild *itemchild,unsigned short gamedbid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(10);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(itemchild->getItemChildID());
	streamBuf->_writeInt(itemchild->getInstanceItemID());
	streamBuf->_writeUShort(gamedbid);
}
void crRemoveItemChildPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int itemchildid = m_streamBuf->_readInt();
		int itemid = m_streamBuf->_readInt();
        unsigned short gamedbid = m_streamBuf->_readUShort();
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		crDBSavePacket packet;
		//crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
		//if(gamedb)
		//{
		//	ref_ptr<crDataBase> gameSession = gamedb->beginSession();
		//	do 
		//	{
				ref_ptr<crRemoveItemChild> removeItemChild = new crRemoveItemChild;
				removeItemChild->buildUpdateSql(itemchildid);
				crDBSavePacket::buildRequestPacket(packet,GameDB,gamedbid,removeItemChild.get());
				dbConductor->getNetManager()->sendPacket("all",packet);
				//if(!gameSession->executeUpdate(removeItemChild.get()))
				//{
				//	CRCore::notify(CRCore::ALWAYS)<<"crRemoveItemChildPacket crRemoveItemChild faild id = "<<itemchildid<<std::endl;
				//	break;
				//}
				ref_ptr<crRemoveInstanceItem> removeItem = new crRemoveInstanceItem;
				removeItem->buildUpdateSql(itemid);
				crDBSavePacket::buildRequestPacket(packet,GameDB,gamedbid,removeItem.get());
				dbConductor->getNetManager()->sendPacket("all",packet);
			//	if(!gameSession->executeUpdate(removeItem.get()))
			//	{
			//		CRCore::notify(CRCore::ALWAYS)<<"crRemoveItemChildPacket crRemoveInstanceItem faild id = "<<itemid<<std::endl;
			//		break;
			//	}
			//	gameSession->commit();
			//} while (0);
			//gamedb->endSession(gameSession.get());
		//}
	}
}
/////////////////////////////////////////
//
//crSceneChatPacket
//
/////////////////////////////////////////
crSceneChatPacket::crSceneChatPacket():
crStreamPacket(crSceneChatPacket::ID)
{
}

crSceneChatPacket::crSceneChatPacket( const crSceneChatPacket &p ):
crStreamPacket(crSceneChatPacket::ID,p)
{
}

void crSceneChatPacket::buildRequestPacket(crSceneChatPacket &packet, const std::string &chatStr)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(chatStr.length()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(chatStr);
}
void crSceneChatPacket::buildRequestPacket2(crSceneChatPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crSceneChatPacket::buildReplyPacket(crSceneChatPacket &packet,int recvid,const std::string &senderName, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16+senderName.length()+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeString(senderName);
	streamBuf->_writeInt64(time(0));
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crSceneChatPacket::buildReplyPacket2(crSceneChatPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crSceneChatPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crSceneChatPacket packet;
				crSceneChatPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crSceneChatPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crNetManager *netManager = m_netConductor->getNetManager();
		ref_ptr<crSceneServerPlayerData> senderPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(senderPlayerData.valid())
		{
            crSceneChatPacket packet;
			crSceneChatPacket::buildReplyPacket(packet,0,senderPlayerData->getCharacterName(),m_streamBuf.get());
			ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
			int bufsize = packetStream->getBufSize();

			netDataManager->lockPlayerDataMap();
			crSceneServerPlayerData *recvPlayerData;
			crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
				itr != playerMap.end();
				++itr )
			{
				recvPlayerData = dynamic_cast<crSceneServerPlayerData *>(itr->second.get());
				if(recvPlayerData && recvPlayerData->getPlayerID() != playerid)
				{
					packetStream->seekBegin();
					packetStream->_writeInt(recvPlayerData->getPlayerID());
					packetStream->setBufSize(bufsize);
					netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
				}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crSceneChatPacket packet;
			crSceneChatPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		//std::string senderName = m_streamBuf->_readString();
		//std::string chatStr = m_streamBuf->_readString();
        crGlobalHandle::getInstance()->doEvent(WCH_RecvGameChat,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crGameChatPacket
//
/////////////////////////////////////////
crGameChatPacket::crGameChatPacket():
crStreamPacket(crGameChatPacket::ID)
{
}

crGameChatPacket::crGameChatPacket( const crGameChatPacket &p ):
crStreamPacket(crGameChatPacket::ID,p)
{
}

void crGameChatPacket::buildRequestPacket(crGameChatPacket &packet, const std::string &chatStr)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(chatStr.length()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(chatStr);
}
void crGameChatPacket::buildRequestPacket2(crGameChatPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crGameChatPacket::buildReplyPacket(crGameChatPacket &packet,int recvid,const std::string &senderName, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16+senderName.length()+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeString(senderName);
	streamBuf->_writeInt64(time(0));
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crGameChatPacket::buildReplyPacket2(crGameChatPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crGameChatPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crGameChatPacket packet;
				crGameChatPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crSceneChatPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crNetManager *netManager = m_netConductor->getNetManager();
		ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(senderPlayerData.valid())
		{
			crGameChatPacket packet;
			crGameChatPacket::buildReplyPacket(packet,0,senderPlayerData->getCharacterName(),m_streamBuf.get());
			ref_ptr<crStreamBuf> packetStream = packet.getStreamBuf();
			int bufsize = packetStream->getBufSize();

			netDataManager->lockPlayerDataMap();
			crGameServerPlayerData *recvPlayerData;
			crNetDataManager::PlayerDataMap &playerMap = netDataManager->getPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerMap.begin();
				itr != playerMap.end();
				++itr )
			{
				recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if(recvPlayerData && recvPlayerData->getPlayerID() != playerid)
				{
					packetStream->seekBegin();
					packetStream->_writeInt(recvPlayerData->getPlayerID());
					packetStream->setBufSize(bufsize);
					netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
				}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crGameChatPacket packet;
			crGameChatPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		//std::string senderName = m_streamBuf->_readString();
		//std::string chatStr = m_streamBuf->_readString();
		crGlobalHandle::getInstance()->doEvent(WCH_RecvGameChat,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crQuerySceneListPacket
//
/////////////////////////////////////////
crQuerySceneListPacket::crQuerySceneListPacket():
crStreamPacket(crQuerySceneListPacket::ID)
{
}

crQuerySceneListPacket::crQuerySceneListPacket( const crQuerySceneListPacket &p ):
crStreamPacket(crQuerySceneListPacket::ID,p)
{
}

void crQuerySceneListPacket::buildRequestPacket(crQuerySceneListPacket &packet,int rows, unsigned char offset)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(rows);
	streamBuf->_writeUChar(offset);
}

void crQuerySceneListPacket::buildRequestPacket2(crQuerySceneListPacket &packet,int playerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQuerySceneListPacket::buildReplyPacket(crQuerySceneListPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crQuerySceneListPacket::buildReplyPacket2(crQuerySceneListPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQuerySceneListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crQuerySceneListPacket packet;
				crQuerySceneListPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneListPacket "<<std::endl;
		int playerid = m_streamBuf->_readInt();
		int rows = m_streamBuf->_readInt();
		unsigned char offset = m_streamBuf->_readUChar(); 
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			crDataBase *resdb = crDataBaseManager::getInstance()->getDataBase(ResDB);
			if(resdb)
			{
				ref_ptr<crDataBase> sceneSession = resdb->beginSession();
				ref_ptr<crSceneListQueryData> sceneListQuery = new crSceneListQueryData;
				sceneListQuery->buildQuerySql(gameid,rows,offset);
				sceneSession->executeQuery(sceneListQuery.get());
				crDataBase::QueryResultVec& sceneListQueryResultVec = sceneSession->getQueryResult();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				unsigned char count = sceneListQueryResultVec.size();
                stream->createBuf(28*(int)count + 1);
				stream->_writeUChar(count);
				for( crDataBase::QueryResultVec::iterator itr = sceneListQueryResultVec.begin();
					 itr != sceneListQueryResultVec.end();
					 ++itr )
				{
					sceneListQuery = dynamic_cast<crSceneListQueryData *>(itr->get());
					stream->_writeInt(sceneListQuery->getSceneID());//4
					stream->_writeString(sceneListQuery->getSceneName());//20+4
					//stream->_writeString(sceneListQuery->getFileName());//50+4
					////stream->_writeString(sceneListQuery->getUpdatecfg());//50+4
					////file modify time
					////char mtime[20];
					//CRCore::Timer_t mtime = 0L;
					//struct stat buf;
					//std::string cookfilename = sceneListQuery->getFileName();
					//if(!cookfilename.empty())
					//{
					//	CRIOManager::GetCookedFileName(cookfilename);
					//	if (stat( cookfilename.c_str(), &buf )) 
					//	{
					//		CRCore::notify(CRCore::FATAL)<<cookfilename<<" 文件不存在"<<std::endl;
					//	}
					//	else
					//	{
					//		mtime = buf.st_mtime;
					//	}
					//}
					////stream->_writeString(mtime);//20+4
					//stream->_writeInt64(mtime);//8
					////updatecfg modify time
					//mtime = 0L;
					//cookfilename = crScene::getUpdatecfg(sceneListQuery->getFileName());
					//if(!cookfilename.empty())
					//{
					//	CRIOManager::GetCookedFileName(cookfilename);
					//	if (stat( cookfilename.c_str(), &buf )) 
					//	{
					//		CRCore::notify(CRCore::FATAL)<<cookfilename<<" 文件不存在"<<std::endl;
					//	}
					//	else
					//	{
					//		mtime = buf.st_mtime;
					//	}
					//}
					////stream->_writeString(mtime);//20+4
					//stream->_writeInt64(mtime);//8
				}
				sceneSession->releaseQuery();
				resdb->endSession(sceneSession.get());

				crQuerySceneListPacket packet;
				crQuerySceneListPacket::buildReplyPacket(packet,playerid,stream.get());
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crQuerySceneListPacket packet;
				crQuerySceneListPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//
		unsigned char count = m_streamBuf->_readUChar();
		ref_ptr<crQuerySceneInfoData> querySceneInfoData;
		QuerySceneList &sceneList = crMyPlayerData::getInstance()->getSceneList();
		sceneList.clear();
		sceneList.reserve(count);
		for(int i = 0; i<(int)count; ++i)
		{
			querySceneInfoData = new crQuerySceneInfoData;
			querySceneInfoData->setSceneID(m_streamBuf->_readInt());
			querySceneInfoData->setSceneName(m_streamBuf->_readString());
			//querySceneInfoData->setFileName(m_streamBuf->_readString());
			//querySceneInfoData->setUpdatecfg(m_streamBuf->_readString());
			//querySceneInfoData->setFileModifyTime(m_streamBuf->_readInt64());
			//querySceneInfoData->setUpdcfgModifyTime(m_streamBuf->_readInt64());
			sceneList.push_back(querySceneInfoData.get());
		}
		crGlobalHandle::getInstance()->doEvent(WCH_RecvSceneList,MAKEINT64(NULL,NULL));
	}
}
/////////////////////////////////////////
//
//crQuerySceneRoomsPacket
//
/////////////////////////////////////////
crQuerySceneRoomsPacket::crQuerySceneRoomsPacket():
crStreamPacket(crQuerySceneRoomsPacket::ID)
{
}

crQuerySceneRoomsPacket::crQuerySceneRoomsPacket( const crQuerySceneRoomsPacket &p ):
crStreamPacket(crQuerySceneRoomsPacket::ID,p)
{
}

void crQuerySceneRoomsPacket::buildRequestPacket(crQuerySceneRoomsPacket &packet)
{
}

void crQuerySceneRoomsPacket::buildRequestPacket2(crQuerySceneRoomsPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crQuerySceneRoomsPacket::buildReplyPacket(crQuerySceneRoomsPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}

void crQuerySceneRoomsPacket::buildReplyPacket2(crQuerySceneRoomsPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQuerySceneRoomsPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		//crQuerySceneRoomsPacket packet;
		//crQuerySceneRoomsPacket::buildRequestPacket2(packet,sender);
		//crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Scene,0);
		//if(netConductor)
		//{
		//	netConductor->getNetManager()->sendPacket("all",packet);
		//}
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crQuerySceneRoomsPacket packet;
				crQuerySceneRoomsPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneRoomsPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneRoomsPacket "<<std::endl;
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			if(callback)
			{
				callback->lockRoomMap();
				RoomMap &roomMap = callback->getRoomMap();
				unsigned short roomcount = roomMap.size();
				unsigned short maxcount = 16;
				unsigned short trycount = CRCore::minimum(roomcount,maxcount);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(6+trycount*(55+25*16));
				stream->_writeInt(playerid);
				stream->_writeUShort(16);//2
				crRoom *room;
				unsigned short relcount = 0;
				ref_ptr<crStreamBuf> roomData;
				int roomDataSize;
				for( RoomMap::iterator itr = roomMap.begin();
					itr != roomMap.end() && relcount<trycount;
					++itr )
				{
					room = itr->second.get();
					if(room->isCanJoin())
					{//还需要判断玩家等级（开房间式游戏根据playergamedata来定）和RoomLevel(RoomLevel根据创建者等级来定)
						relcount++;
						room->getRoomData(roomData);
						roomDataSize = roomData->getBufSize();
						stream->write(roomData->getBuf(),roomDataSize);
					}
				}
				int bufsize = stream->getBufSize();
				stream->seekBegin();
				stream->_writeUShort(relcount);
				stream->setBufSize(bufsize);
				callback->unlockRoomMap();
				crQuerySceneRoomsPacket packet;
				crQuerySceneRoomsPacket::buildReplyPacket(packet,stream.get());
				m_netConductor->getNetManager()->sendPacket(sender,packet);

			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crQuerySceneRoomsPacket packet;
				crQuerySceneRoomsPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneRoomsPacket 2"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//
		unsigned short count = m_streamBuf->_readUShort();
		ref_ptr<crRoom> room;
		ref_ptr<crRoomPlayer> roomPlayer;
		crMyPlayerData::getInstance()->lockRoomMap();
		RoomMap &roomMap = crMyPlayerData::getInstance()->getRoomMap();
		roomMap.clear();
		for(int i = 0; i<(int)count; ++i)
		{
			room = new crRoom(NULL);
			room->setRoomData(m_streamBuf.get());
			roomMap[room->getRoomID()] = room.get();
		}
		crMyPlayerData::getInstance()->unlockRoomMap();
		crGlobalHandle::getInstance()->doEvent(WCH_RecvSceneRooms,MAKEINT64(NULL,NULL));
	}
}
/////////////////////////////////////////
//
//crCreateSceneRoomPacket
//
/////////////////////////////////////////
crCreateSceneRoomPacket::crCreateSceneRoomPacket():
crStreamPacket(crCreateSceneRoomPacket::ID)
{
}

crCreateSceneRoomPacket::crCreateSceneRoomPacket( const crCreateSceneRoomPacket &p ):
crStreamPacket(crCreateSceneRoomPacket::ID,p)
{
}

void crCreateSceneRoomPacket::buildRequestPacket(crCreateSceneRoomPacket &packet,crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}

void crCreateSceneRoomPacket::buildRequestPacket2(crCreateSceneRoomPacket &packet,int playerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateSceneRoomPacket::buildReplyPacket(crCreateSceneRoomPacket &packet,int playerid,crRoom *room)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	ref_ptr<crStreamBuf> roomData;
	if(room)
	{
		room->getRoomData(roomData);
	    streamBuf->createBuf(5+roomData->getBufSize());
	}
	else
		streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeBool(room!=NULL);
	if(room)
	{
		//streamBuf->_writeUChar(room->getMember(playerid)->getGroupID());
		streamBuf->write(roomData->getBuf(),roomData->getBufSize());
	}
}

void crCreateSceneRoomPacket::buildReplyPacket2(crCreateSceneRoomPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateSceneRoomPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crCreateSceneRoomPacket packet;
				crCreateSceneRoomPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateSceneRoomPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crCreateSceneRoomPacket "<<std::endl;
		int playerid = m_streamBuf->_readInt();

		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
		if(playerData.valid() && callback)
		{
			int oldroomid = playerData->getRoomID();
			if(oldroomid!=0)
			{
				crRoom *oldroom = callback->findRoom(oldroomid);
				if(oldroom)
				{
					oldroom->removeMember(playerid,true);
					playerData->setRoomID(0);
				}
			}
			crRoom *room = callback->createRoom(playerData.get(),m_streamBuf.get());
			if(room)
			{
				playerData->setRoomID(room->getRoomID());
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateSceneRoomPacket success "<<std::endl;
			}
			crCreateSceneRoomPacket packet;
			crCreateSceneRoomPacket::buildReplyPacket(packet,playerid,room);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crCreateSceneRoomPacket packet;
				crCreateSceneRoomPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateSceneRoomPacket 2"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		bool success = m_streamBuf->_readBool();
		if(success)
		{
			RoomMap &roomMap = crMyPlayerData::getInstance()->getRoomMap();
			ref_ptr<crRoom> room;
			room = new crRoom(NULL);
			room->setRoomData(m_streamBuf.get());
			crRoomPlayer *me = room->getMember(crMyPlayerData::getInstance()->getPlayerID());
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			me->setPlayerGameData(playerGameData);
			roomMap[room->getRoomID()] = room.get();
			crMyPlayerData::getInstance()->setSelectedRoom(room.get());
		}
		//crGlobalHandle::getInstance()->doEvent(WCH_RecvCreateSceneRoom,MAKEINT64(success?1:0,NULL));
		m_streamBuf->seekBegin();
		crBrain::getInstance()->doEvent(WCH_RecvCreateSceneRoom,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crPreEnterSceneRoomPacket
//
/////////////////////////////////////////
crPreEnterSceneRoomPacket::crPreEnterSceneRoomPacket():
crStreamPacket(crPreEnterSceneRoomPacket::ID)
{
}

crPreEnterSceneRoomPacket::crPreEnterSceneRoomPacket( const crPreEnterSceneRoomPacket &p ):
crStreamPacket(crPreEnterSceneRoomPacket::ID,p)
{
}

void crPreEnterSceneRoomPacket::buildRequestPacket(crPreEnterSceneRoomPacket &packet,int roomid,const std::string &characterName/*,const std::string &password*/)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+characterName.size());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roomid);//4
	streamBuf->_writeString(characterName);
	//streamBuf->_writeString(password);//8+4
}

void crPreEnterSceneRoomPacket::buildRequestPacket2(crPreEnterSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPreEnterSceneRoomPacket::buildReplyPacket(crPreEnterSceneRoomPacket &packet,int playerid,bool success)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeBool(success);
}

void crPreEnterSceneRoomPacket::buildReplyPacket2(crPreEnterSceneRoomPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPreEnterSceneRoomPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPreEnterSceneRoomPacket packet;
				crPreEnterSceneRoomPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPreEnterSceneRoomPacket "<<std::endl;
			}
		}
		//crPreEnterSceneRoomPacket packet;
		//crPreEnterSceneRoomPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		//crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Scene,0);
		//if(netConductor)
		//{
		//	netConductor->getNetManager()->sendPacket("all",packet);
		//}
	}
	else if(netType == SceneServer)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket "<<std::endl;
		int playerid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		//std::string password = m_streamBuf->_readString();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			bool success = false;
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			if(callback)
			{
				crRoom *room = callback->findRoom(roomid);
				if(room && room->isCanJoin())
				{
					ref_ptr<crRoomPlayer> roomPlayer = new crRoomPlayer;
					//unsigned char groupid = room->assignGroupID();
					//roomPlayer->setGroupID(groupid);
					roomPlayer->setPlayerID(playerid);
					roomPlayer->setCharacterName(m_streamBuf->_readString());
					roomPlayer->setPlayerGameData(playerData->getPlayerGameData());
					success = room->addMember(roomPlayer.get()/*,false*//*true*/);//////
					if(success)
					{
						crPreEnterSceneRoomPacket packet;
						crPreEnterSceneRoomPacket::buildReplyPacket(packet,playerid,success);
						m_netConductor->getNetManager()->sendPacket(sender,packet);
					}
				}
			}
			if(!success)
			{
				crPreEnterSceneRoomPacket packet;
				crPreEnterSceneRoomPacket::buildReplyPacket(packet,playerid,false);
				m_netConductor->getNetManager()->sendPacket(sender,packet);
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crPreEnterSceneRoomPacket packet;
				crPreEnterSceneRoomPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}
		}
		//std::string playAddress = m_streamBuf->_readString();
		//crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		//if(connectServerConductor)
		//{
		//	crPreEnterSceneRoomPacket packet;
		//	crPreEnterSceneRoomPacket::buildReplyPacket2(packet,m_streamBuf.get());
		//	connectServerConductor->getNetManager()->sendPacket(playAddress,packet);	
		//}
	}
	else if(netType == GameClient_Game)
	{//
		bool success = m_streamBuf->_readBool();
		if(success)
		{
			//unsigned char groupid = m_streamBuf->_readUChar();
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			ref_ptr<crRoomPlayer> roomPlayer = new crRoomPlayer;
			//roomPlayer->setGroupID(groupid);
			roomPlayer->setPlayerID(crMyPlayerData::getInstance()->getPlayerID());
			roomPlayer->setCharacterName(crMyPlayerData::getInstance()->getCharacterName());
			room->addMember(roomPlayer.get()/*,false*/);
		}
		//crGlobalHandle::getInstance()->doEvent(WCH_RecvEnterSceneRoom,MAKEINT64(success?1:0,NULL));
		crBrain::getInstance()->doEvent(WCH_RecvEnterSceneRoom,MAKEINT64(success?1:0,NULL));
		//char mode = crMyPlayerData::getInstance()->getLoginGameSceneMode();
		//if(mode == 2)
		//{
		//	crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_RecvEnterSceneRoom,MAKEINT64(success?1:0,NULL));
		//}
		//else
		//{
		//	crBrain::getInstance()->doEvent(WCH_RecvEnterSceneRoom,MAKEINT64(success?1:0,NULL));
		//}
	}
}
///////////////////////////////////////////
////
////crRoomPlayerDataPacket
////
///////////////////////////////////////////
//crRoomPlayerDataPacket::crRoomPlayerDataPacket():
//crStreamPacket(crRoomPlayerDataPacket::ID)
//{
//}
//
//crRoomPlayerDataPacket::crRoomPlayerDataPacket( const crRoomPlayerDataPacket &p ):
//crStreamPacket(crRoomPlayerDataPacket::ID,p)
//{
//}
//void crRoomPlayerDataPacket::buildReplyPacket(crRoomPlayerDataPacket &packet,int playerid,int streamid,int count)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(12);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(streamid);
//	streamBuf->_writeInt(count);//4
//}
//
//void crRoomPlayerDataPacket::buildReplyPacket2(crRoomPlayerDataPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//
//void crRoomPlayerDataPacket::parsePacket(const std::string &sender)
//{//
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//    if(netType == GameConnectServerClient_Scene)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData.valid())
//			{
//				crRoomPlayerDataPacket packet;
//				crRoomPlayerDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crRoomInfoChangePacket"<<std::endl;
//			}	
//		}
//	}
//	else if(netType == GameClient_Game)
//	{//
//		int streamid = m_streamBuf->_readInt();
//		int count = m_streamBuf->_readInt();
//		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crRoomPlayerDataPacket::ID,count);
//	}
//}
///////////////////////////////////////////
////
////crLeaveSceneRoomPacket
////
///////////////////////////////////////////
//crLeaveSceneRoomPacket::crLeaveSceneRoomPacket():
//crStreamPacket(crLeaveSceneRoomPacket::ID)
//{
//}
//
//crLeaveSceneRoomPacket::crLeaveSceneRoomPacket( const crLeaveSceneRoomPacket &p ):
//crStreamPacket(crLeaveSceneRoomPacket::ID,p)
//{
//}
//
//void crLeaveSceneRoomPacket::buildRequestPacket(crLeaveSceneRoomPacket &packet)
//{
//}
//
//void crLeaveSceneRoomPacket::buildRequestPacket2(crLeaveSceneRoomPacket &packet,int playerid)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//}
//void crLeaveSceneRoomPacket::buildReplyPacket(crLeaveSceneRoomPacket &packet,int playerid,int leaver)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(8);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt(leaver);
//}
//
//void crLeaveSceneRoomPacket::buildReplyPacket2(crLeaveSceneRoomPacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crLeaveSceneRoomPacket::parsePacket(const std::string &sender)
//{//
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//	if(netType == GameConnectServer)
//	{//
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
//		if(playerData)
//		{
//			crNetConductor *conductor = playerData->getSceneServerLineConductor();
//			if(conductor)
//			{
//				crLeaveSceneRoomPacket packet;
//				crLeaveSceneRoomPacket::buildRequestPacket2(packet,playerData->getPlayerID());
//				conductor->getNetManager()->sendPacket("all",packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket "<<std::endl;
//			}
//		}
//	}
//	else if(netType == SceneServer)
//	{
//		//CRCore::notify(CRCore::ALWAYS)<<"crQuerySceneInfoPacket "<<std::endl;
//		int playerid = m_streamBuf->_readInt();
//		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
//		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
//		if(playerData.valid())
//		{
//			int roomid = playerData->getRoomID();
//			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
//			callback->lockRoomMap();
//			crRoom *room = callback->findRoom(roomid);
//			if(room)
//			{
//				playerData->setClientRunning(false);
//				playerData->setRoomID(0);
//				//if(room->getGameRunning())
//				//{//如果在游戏中退出则存储一次
//				//	callback->savePlayerData(playerData,crSceneServerCallback::SaveAll);
//				//}
//				room->removeMember(playerid);
//				if(room->getPlayerCount()==0)
//				{
//					callback->endRoomGame(room);
//					callback->removeRoom(roomid);
//				}
//				else
//				{
//					crLeaveSceneRoomPacket packet;
//					crLeaveSceneRoomPacket::buildReplyPacket(packet,0,playerid);
//					room->sendPacketToAll(packet,playerid);
//				}
//			}
//			callback->unlockRoomMap();
//		}
//	}
//	else if(netType == GameConnectServerClient_Scene)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData.valid())
//			{
//				crLeaveSceneRoomPacket packet;
//				crLeaveSceneRoomPacket::buildReplyPacket2(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crRoomInfoChangePacket"<<std::endl;
//			}	
//		}
//	}
//	else if(netType == GameClient_Game)
//	{//
//		int playerid = m_streamBuf->_readInt();
//		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
//		if(room)
//		{
//			room->removeMember(playerid);
//		}
//		//crGlobalHandle::getInstance()->doEvent(WCH_RecvLeaveSceneRoom,MAKEINT64(playerid,NULL));
//	}
//}
/////////////////////////////////////////
//
//crStartRoomGamePacket
//
/////////////////////////////////////////
crStartRoomGamePacket::crStartRoomGamePacket():
crStreamPacket(crStartRoomGamePacket::ID)
{
}

crStartRoomGamePacket::crStartRoomGamePacket( const crStartRoomGamePacket &p ):
crStreamPacket(crStartRoomGamePacket::ID,p)
{
}

void crStartRoomGamePacket::buildRequestPacket(crStartRoomGamePacket &packet)
{
}

void crStartRoomGamePacket::buildRequestPacket2(crStartRoomGamePacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crStartRoomGamePacket::buildReplyPacket(crStartRoomGamePacket &packet,CRCore::crStreamBuf *stream)
{
	//crStreamBuf *streamBuf = new crStreamBuf;
	//streamBuf->createBuf(16);
	//packet.setStreamBuf(streamBuf);
	//streamBuf->_writeInt(playerid);
	//streamBuf->_writeInt(posx);
	//streamBuf->_writeInt(posy);
	//streamBuf->_writeInt(posz);
	stream->seekBegin();
	packet.setStreamBuf(stream);
}

void crStartRoomGamePacket::buildReplyPacket2(crStartRoomGamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crStartRoomGamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crStartRoomGamePacket packet;
				crStartRoomGamePacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket "<<std::endl;
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			int roomid = playerData->getRoomID();
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			crRoom *room = callback->findRoom(roomid);
			if(room)
			{
				callback->startRoomGame(room);
				room->startGame();
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crStartRoomGamePacket packet;
				crStartRoomGamePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvStartRoomGame,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crEndRoomGamePacket
//
/////////////////////////////////////////
crEndRoomGamePacket::crEndRoomGamePacket():
crStreamPacket(crEndRoomGamePacket::ID)
{
}

crEndRoomGamePacket::crEndRoomGamePacket( const crEndRoomGamePacket &p ):
crStreamPacket(crEndRoomGamePacket::ID,p)
{
}

void crEndRoomGamePacket::buildReplyPacket(crEndRoomGamePacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crEndRoomGamePacket::buildReplyPacket2(crEndRoomGamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crEndRoomGamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
    if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crEndRoomGamePacket packet;
				crEndRoomGamePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvEndRoomGame,MAKEINT64(NULL,NULL));
		crMyPlayerData::getInstance()->clearAllInsights();
	}
}
/////////////////////////////////////////
//
//crPauseRoomGamePacket
//
/////////////////////////////////////////
crPauseRoomGamePacket::crPauseRoomGamePacket():
	crStreamPacket(crPauseRoomGamePacket::ID)
{
}

crPauseRoomGamePacket::crPauseRoomGamePacket( const crPauseRoomGamePacket &p ):
	crStreamPacket(crPauseRoomGamePacket::ID,p)
{
}

void crPauseRoomGamePacket::buildReplyPacket(crPauseRoomGamePacket &packet,int playerid,float pausetime)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeFloat(pausetime);
}

void crPauseRoomGamePacket::buildReplyPacket2(crPauseRoomGamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPauseRoomGamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crPauseRoomGamePacket packet;
				crPauseRoomGamePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		float pausetime = m_streamBuf->_readFloat();
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			room->pauseRoom(pausetime);
			room->doEvent(WCH_RecvPauseRoom);
		}
	}
}
/////////////////////////////////////////
//
//crRemoveRoomGamePacket
//
/////////////////////////////////////////
crRemoveRoomGamePacket::crRemoveRoomGamePacket():
	crStreamPacket(crRemoveRoomGamePacket::ID)
{
}

crRemoveRoomGamePacket::crRemoveRoomGamePacket( const crRemoveRoomGamePacket &p ):
	crStreamPacket(crRemoveRoomGamePacket::ID,p)
{
}

void crRemoveRoomGamePacket::buildReplyPacket(crRemoveRoomGamePacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crRemoveRoomGamePacket::buildReplyPacket2(crRemoveRoomGamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRemoveRoomGamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crRemoveRoomGamePacket packet;
				crRemoveRoomGamePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvRemoveRoomGame,MAKEINT64(NULL,NULL));
	}
}
/////////////////////////////////////////
//
//crRoomGameTimePacket
//
/////////////////////////////////////////
crRoomGameTimePacket::crRoomGameTimePacket():
crStreamPacket(crRoomGameTimePacket::ID)
{
}

crRoomGameTimePacket::crRoomGameTimePacket( const crRoomGameTimePacket &p ):
crStreamPacket(crRoomGameTimePacket::ID,p)
{
}

void crRoomGameTimePacket::buildReplyPacket(crRoomGameTimePacket &packet,int playerid,float time)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeFloat(time);
}

void crRoomGameTimePacket::buildReplyPacket2(crRoomGameTimePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRoomGameTimePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crRoomGameTimePacket packet;
				crRoomGameTimePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room) room->timeRemain() = m_streamBuf->_readFloat();
	}
}
/////////////////////////////////////////
//
//crRequestEmporiumPacket
//
/////////////////////////////////////////
crRequestEmporiumPacket::crRequestEmporiumPacket():
crStreamPacket(crRequestEmporiumPacket::ID)
{
}

crRequestEmporiumPacket::crRequestEmporiumPacket( const crRequestEmporiumPacket &p ):
crStreamPacket(crRequestEmporiumPacket::ID,p)
{
}

void crRequestEmporiumPacket::buildRequestPacket(crRequestEmporiumPacket &packet)
{
}

void crRequestEmporiumPacket::buildRequestPacket2(crRequestEmporiumPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crRequestEmporiumPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRequestEmporiumPacket packet;
				crRequestEmporiumPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer不连数据库
		int sendid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(sendid));
		if(playerData.valid())
		{
			crInstanceItem *item = NULL;
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crScene *scene = netCallback->findScene(playerData->getSceneID());
			if(scene)
				item = scene->getEmporium();
			if(item)
			{
				/////write and send
				int count = 0;
				std::vector< ref_ptr<crStreamBuf> > BufVec;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(MTUBUFFERSIZE);//(MAXBUFFERSIZE-12);

				writeItem(item,stream,BufVec,count,NetStream);
				if(stream->getBufSize()>0)
				{
					count += stream->getBufSize();
					BufVec.push_back(stream.get());
				}
				if(count>0)
				{
					int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
					crRequestItemPacket packet;
					crRequestItemPacket::buildReplyPacket(packet,sendid,streamid,count);
					m_netConductor->getNetManager()->sendPacket(sender,packet);
					crDataStreamPacket streamPacket;
					for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						itr != BufVec.end();
						++itr )
					{
						crDataStreamPacket::buildRequestPacket(streamPacket,sendid,streamid,crRequestItemPacket::ID,itr->get());
						m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
					}
					crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRemoveRolePacket
//
/////////////////////////////////////////
crRemoveRolePacket::crRemoveRolePacket():
crStreamPacket(crRemoveRolePacket::ID)
{
}

crRemoveRolePacket::crRemoveRolePacket( const crRemoveRolePacket &p ):
crStreamPacket(crRemoveRolePacket::ID,p)
{
}

void crRemoveRolePacket::buildRequestPacket(crRemoveRolePacket &packet,int roleid,std::string &code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+code.size());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeString(code);
}

void crRemoveRolePacket::buildRequestPacket2(crRemoveRolePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRemoveRolePacket::buildReplyPacket(crRemoveRolePacket &packet,int playerid,int roleid,bool success)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt(roleid);
	streamBuf->_writeBool(success);
}

void crRemoveRolePacket::buildReplyPacket2(crRemoveRolePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRemoveRolePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crRemoveRolePacket packet;
				crRemoveRolePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRequestItemPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		int roleid = m_streamBuf->_readInt();
		std::string code = m_streamBuf->_readString();//用于安全效验
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		bool success = false;
		if(playerData.valid() && code.compare("CreRemoveRole") == 0)
		{
			int gamedbid = playerData->getGameDBID();
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
			if(gamedb)
			{
				int itemid = 0;
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crRemoveRoleQueryData> roleQuery = new crRemoveRoleQueryData;
				roleQuery->buildQuerySql(roleid);
				gameSession->executeQuery(roleQuery.get());
				crDataBase::QueryResultVec& roleQueryResultVec = gameSession->getQueryResult();
				if(!roleQueryResultVec.empty())
				{
					roleQuery = dynamic_cast<crRemoveRoleQueryData *>(roleQueryResultVec[0].get());
					if(roleQuery->getPlayerID() == playerid)
					{
						itemid = roleQuery->getInstanceItemID();
					}
				}
				gameSession->releaseQuery();

				while(itemid!=0)
				{
					ref_ptr<crItemChild2QueryData> itemChild2Query = new crItemChild2QueryData;
					itemChild2Query->buildQuerySql(itemid);
					gameSession->executeQuery(itemChild2Query.get());
					crDataBase::QueryResultVec itemChild2QueryResultVec = gameSession->getQueryResult();
					gameSession->releaseQuery();
					ref_ptr<crRemoveInstanceItem> removeInstanceItem = new crRemoveInstanceItem;
					bool faild = false;
					for( crDataBase::QueryResultVec::iterator itr = itemChild2QueryResultVec.begin();
						 itr != itemChild2QueryResultVec.end();
						 ++itr )
					{
						itemChild2Query = dynamic_cast<crItemChild2QueryData *>(itr->get());
						removeInstanceItem->buildUpdateSql(itemChild2Query->getInstanceitemid());
						if(!gameSession->executeUpdate(removeInstanceItem.get()))
						{
							faild = true;
							gameSession->rollback();
							break;
						}
					}
					if(faild)
						break;
					removeInstanceItem->buildUpdateSql(itemid);
					if(!gameSession->executeUpdate(removeInstanceItem.get()))
					{
						gameSession->rollback();
						break;
					}
					ref_ptr<crRemoveItemChild> removeItemChild = new crRemoveItemChild;
					removeItemChild->buildUpdateSql2(itemid);
					if(!gameSession->executeUpdate(removeItemChild.get()))
					{
						gameSession->rollback();
						break;
					}
					ref_ptr<crRemoveRole> removeRole = new crRemoveRole;
					removeRole->buildUpdateSql(roleid);
					if(!gameSession->executeUpdate(removeRole.get()))
					{
						gameSession->rollback();
						break;
					}
					gameSession->commit();
					success = true;
					break;
				}
				gamedb->endSession(gameSession.get());
			}
		}
		crRemoveRolePacket packet;
		crRemoveRolePacket::buildReplyPacket(packet,playerid,roleid,success);
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crRemoveRolePacket packet;
				crRemoveRolePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRemoveRolePacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		int roleid = m_streamBuf->_readInt();
		bool success = m_streamBuf->_readBool();
		crGlobalHandle::getInstance()->doEvent(WCH_RecvRemoveRole,MAKEINT64(roleid,success));
	}
}
/////////////////////////////////////////
//
//crEnterSceneRoomPacket
//
/////////////////////////////////////////
crEnterSceneRoomPacket::crEnterSceneRoomPacket():
crStreamPacket(crEnterSceneRoomPacket::ID)
{
}

crEnterSceneRoomPacket::crEnterSceneRoomPacket( const crEnterSceneRoomPacket &p ):
crStreamPacket(crEnterSceneRoomPacket::ID,p)
{
}

void crEnterSceneRoomPacket::buildRequestPacket(crEnterSceneRoomPacket &packet,int roomid)

{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roomid);
}

void crEnterSceneRoomPacket::buildRequestPacket2(crEnterSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crEnterSceneRoomPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crEnterSceneRoomPacket packet;
				crEnterSceneRoomPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crEnterSceneRoomPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
		if(playerData.valid() && callback)
		{
			ref_ptr<crRoom> room = callback->findRoom(roomid);
			if(room.valid())
			{
				ref_ptr<crRoomPlayer>roomPlayer = room->getMember(playerid);
				if(roomPlayer.valid())
				{
					//roomPlayer->setPlayerName(playerData->getPlayerName());
					playerData->setRoomID(roomid);
					//room->playerJoined(playerid);
					//CRCore::notify(CRCore::ALWAYS)<<"crEnterSceneRoomPacket "<<roomid<<std::endl;
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRoomPlayerEventPacket
//
/////////////////////////////////////////
crRoomPlayerEventPacket::crRoomPlayerEventPacket():
	crStreamPacket(crRoomPlayerEventPacket::ID)
{
}

crRoomPlayerEventPacket::crRoomPlayerEventPacket( const crRoomPlayerEventPacket &p ):
	crStreamPacket(crRoomPlayerEventPacket::ID,p)
{
}

void crRoomPlayerEventPacket::buildRequestPacket(crRoomPlayerEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crRoomPlayerEventPacket::buildRequestPacket(crRoomPlayerEventPacket &packet, int recvid, int playerid, _crInt64 msg, CRCore::crStreamBuf *param)
{//server send to client
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crRoomPlayerEventPacket::buildRequestPacket2(crRoomPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomPlayerEventPacket::buildReplyPacket(crRoomPlayerEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crRoomPlayerEventPacket::buildReplyPacket2(crRoomPlayerEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomPlayerEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRoomPlayerEventPacket packet;
				crRoomPlayerEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRoomPlayerEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid() && netPlayerData->getPlayerGameData())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crRoom* room = callback->findRoom(netPlayerData->getRoomID());
			if(room)
			{
				crRoomPlayer* roomPlayer = room->getMember(playerid);
				if(roomPlayer)
				{
					_crInt64 msg = m_streamBuf->_readInt64();
					playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));

					crRoomPlayerEventPacket packet;
					crRoomPlayerEventPacket::buildReplyPacket(packet,0,m_streamBuf.get());
					room->sendPacketToAll(packet,playerid);
					//netPlayerData->sendPacketToNeighbor(packet);
					//CRCore::notify(CRCore::ALWAYS)<<"crRoomPlayerEventPacket "<<std::endl;
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomPlayerEventPacket packet;
			crRoomPlayerEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		_crInt64 msg = m_streamBuf->_readInt64();
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			crRoomPlayer *roomPlayer = room->getMember(playerid);
			 if(roomPlayer&&roomPlayer->getPlayerGameData())
			 {
				 roomPlayer->getPlayerGameData()->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			 }
		}
	}
	else if(netType == GameServer)
	{
		m_streamBuf->_readInt();
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid() && netPlayerData->getPlayerGameData())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			netPlayerData->getPlayerGameData()->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}
/////////////////////////////////////////
//
//crRoomAddMemberPacket
//
/////////////////////////////////////////
crRoomAddMemberPacket::crRoomAddMemberPacket():
	crStreamPacket(crRoomAddMemberPacket::ID)
{
}

crRoomAddMemberPacket::crRoomAddMemberPacket( const crRoomAddMemberPacket &p ):
	crStreamPacket(crRoomAddMemberPacket::ID,p)
{
}

void crRoomAddMemberPacket::buildReplyPacket(crRoomAddMemberPacket &packet,int recvid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crRoomAddMemberPacket::buildReplyPacket2(crRoomAddMemberPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRoomAddMemberPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomAddMemberPacket packet;
			crRoomAddMemberPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			ref_ptr<crRoomPlayer> roomPlayer = new crRoomPlayer;
			roomPlayer->setStream(m_streamBuf.get());
			room->addMember(roomPlayer.get()/*,false*/);
		}
	}
}
/////////////////////////////////////////
//
//crRoomRemoveMemberPacket
//
/////////////////////////////////////////
crRoomRemoveMemberPacket::crRoomRemoveMemberPacket():
	crStreamPacket(crRoomRemoveMemberPacket::ID)
{
}

crRoomRemoveMemberPacket::crRoomRemoveMemberPacket( const crRoomRemoveMemberPacket &p ):
	crStreamPacket(crRoomRemoveMemberPacket::ID,p)
{
}

void crRoomRemoveMemberPacket::buildReplyPacket(crRoomRemoveMemberPacket &packet,int recvid,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
}
void crRoomRemoveMemberPacket::buildReplyPacket2(crRoomRemoveMemberPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRoomRemoveMemberPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomRemoveMemberPacket packet;
			crRoomRemoveMemberPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			int playerid = m_streamBuf->_readInt();
			room->removeMember(playerid,false);
		}
	}
}
/////////////////////////////////////////
//
//crRoomSetPlayerGameDataPacket
//
/////////////////////////////////////////
crRoomSetPlayerGameDataPacket::crRoomSetPlayerGameDataPacket():
	crStreamPacket(crRoomSetPlayerGameDataPacket::ID)
{
}

crRoomSetPlayerGameDataPacket::crRoomSetPlayerGameDataPacket( const crRoomSetPlayerGameDataPacket &p ):
	crStreamPacket(crRoomSetPlayerGameDataPacket::ID,p)
{
}

void crRoomSetPlayerGameDataPacket::buildReplyPacket(crRoomSetPlayerGameDataPacket &packet,int recvid,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRoomSetPlayerGameDataPacket::buildReplyPacket2(crRoomSetPlayerGameDataPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crRoomSetPlayerGameDataPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomSetPlayerGameDataPacket packet;
			crRoomSetPlayerGameDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			int playerid = m_streamBuf->_readInt();
			crRoomPlayer *roomPlayer = room->getMember(playerid);
			if(roomPlayer)
			{
				std::string script = crMyPlayerData::getInstance()->getSelectedGame()->getGameName() + "/playergamedata.cfg";
				ref_ptr<crData> data = CREncapsulation::loadData(script);
				if (data.valid()) 
				{
					ref_ptr<crPlayerGameData> playerGameData = new crPlayerGameData;
					playerGameData->setPlayerID(playerid);
					playerGameData->setDataClass(data.get());
					data->inputParam(WCHDATA_PlayerStream,m_streamBuf.get());
					data->excHandle(MAKEINT64(WCH_InitData,playerGameData.get()));
					roomPlayer->setPlayerGameData(playerGameData.get());
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRoomPlayerGameDataUpdatePacket
//
/////////////////////////////////////////
crRoomPlayerGameDataUpdatePacket::crRoomPlayerGameDataUpdatePacket():
	crStreamPacket(crRoomPlayerGameDataUpdatePacket::ID)
{
}

crRoomPlayerGameDataUpdatePacket::crRoomPlayerGameDataUpdatePacket( const crRoomPlayerGameDataUpdatePacket &p ):
	crStreamPacket(crRoomPlayerGameDataUpdatePacket::ID,p)
{
}

//void crRoomPlayerGameDataUpdatePacket::buildRequestPacket(crRoomPlayerGameDataUpdatePacket &packet,int playerid,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(4+stream->getBufSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->write(stream->getBuf(),stream->getBufSize());
//}

void crRoomPlayerGameDataUpdatePacket::buildReplyPacket(crRoomPlayerGameDataUpdatePacket &packet,int recvid,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crRoomPlayerGameDataUpdatePacket::buildReplyPacket2(crRoomPlayerGameDataUpdatePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomPlayerGameDataUpdatePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomPlayerGameDataUpdatePacket packet;
			crRoomPlayerGameDataUpdatePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			int playerid = m_streamBuf->_readInt();
			crRoomPlayer *roomPlayer = room->getMember(playerid);
			if(roomPlayer)
			{
				ref_ptr<crPlayerGameData> playerGameData = roomPlayer->getPlayerGameData();
				if(playerGameData.valid() && playerGameData->getDataClass())
				{
					playerGameData->doEvent(WCH_RecvRoomPlayerGameDataUpdate,MAKEINT64(m_streamBuf.get(),netType));
					//ref_ptr<crData> data = playerGameData->getDataClass();
					//data->inputParam(WCHDATA_NetStream2,m_streamBuf.get());
				}
			}
		}
	}
	//else if(netType == GameServer)
	//{
	//	int playerid = m_streamBuf->_readInt();
	//	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	//	ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
	//	if(netPlayerData.valid())
	//	{
	//		ref_ptr<crPlayerGameData> playerGameData = netPlayerData->getPlayerGameData();
	//		if(playerGameData.valid() && playerGameData->getDataClass())
	//		{
	//			ref_ptr<crData> data = playerGameData->getDataClass();
	//			data->inputParam(WCHDATA_NetStream,m_streamBuf.get());
	//		}
	//	}
	//}
}
/////////////////////////////////////////
//
//crRoomPlayerReadyPacket
//
/////////////////////////////////////////
crRoomPlayerReadyPacket::crRoomPlayerReadyPacket():
	crStreamPacket(crRoomPlayerReadyPacket::ID)
{
}

crRoomPlayerReadyPacket::crRoomPlayerReadyPacket( const crRoomPlayerReadyPacket &p ):
	crStreamPacket(crRoomPlayerReadyPacket::ID,p)
{
}

void crRoomPlayerReadyPacket::buildReplyPacket(crRoomPlayerReadyPacket &packet,int recvid,int playerid,unsigned char groupid,bool ready)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(10);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeUChar(groupid);
	streamBuf->_writeBool(ready);
}

void crRoomPlayerReadyPacket::buildReplyPacket2(crRoomPlayerReadyPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomPlayerReadyPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomPlayerReadyPacket packet;
			crRoomPlayerReadyPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			int playerid = m_streamBuf->_readInt();
			crRoomPlayer *roomPlayer = room->getMember(playerid);
			if(roomPlayer)
			{
				roomPlayer->setGroupID(m_streamBuf->_readUChar());
				roomPlayer->setReady(m_streamBuf->_readBool());
			}
		}
	}
}
/////////////////////////////////////////
//
//crSavePlayerGameDataPacket
//
/////////////////////////////////////////
crSavePlayerGameDataPacket::crSavePlayerGameDataPacket():
	crStreamPacket(crSavePlayerGameDataPacket::ID)
{
}

crSavePlayerGameDataPacket::crSavePlayerGameDataPacket( const crSavePlayerGameDataPacket &p ):
	crStreamPacket(crSavePlayerGameDataPacket::ID,p)
{
}

void crSavePlayerGameDataPacket::buildRequestPacket(crSavePlayerGameDataPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crSavePlayerGameDataPacket::buildReplyPacket2(crSavePlayerGameDataPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crSavePlayerGameDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData && playerGameData->getDataClass())
			{
				crData *data = playerGameData->getDataClass();
				data->inputParam(WCHDATA_SceneSaveStream,m_streamBuf.get());
			}
		}
	}
	//else if(netType == GameConnectServerClient_Scene)
	//{
	//	int playerid = m_streamBuf->_readInt();
	//	crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
	//	ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
	//	if(playerData.valid())
	//	{
	//		crSavePlayerGameDataPacket packet;
	//		crSavePlayerGameDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//		connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//	}
	//}
	//else if(netType == GameClient_Game)
	//{
	//	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	//	if(playerGameData && playerGameData->getDataClass())
	//	{
	//		crData *data = playerGameData->getDataClass();
	//		data->inputParam(WCHDATA_SceneSaveStream,m_streamBuf.get());
	//	}
	//}
}
/////////////////////////////////////////
//
//crCheckPlayerGameNamePacket
//
/////////////////////////////////////////
crCheckPlayerGameNamePacket::crCheckPlayerGameNamePacket():
	crStreamPacket(crCheckPlayerGameNamePacket::ID)
{
}

crCheckPlayerGameNamePacket::crCheckPlayerGameNamePacket( const crCheckPlayerGameNamePacket &p ):
	crStreamPacket(crCheckPlayerGameNamePacket::ID,p)
{
}

void crCheckPlayerGameNamePacket::buildRequestPacket(crCheckPlayerGameNamePacket &packet, std::string &name)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(name.size()>20)
		name = name.substr(0,20);
	streamBuf->createBuf(4+name.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(name);
}

void crCheckPlayerGameNamePacket::buildRequestPacket2(crCheckPlayerGameNamePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crCheckPlayerGameNamePacket::buildReplyPacket(crCheckPlayerGameNamePacket &packet,int recvid, char code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(code);
}

void crCheckPlayerGameNamePacket::buildReplyPacket2(crCheckPlayerGameNamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crCheckPlayerGameNamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crCheckPlayerGameNamePacket packet;
				crCheckPlayerGameNamePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		char code = 0;
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			std::string name = m_streamBuf->_readString();
			if(!name.empty()&&name.size()<20)
			{
				//ref_ptr<crTableIO> InvalidNameTab;
				//InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNameLimitTab);
				//if (InvalidNameTab.valid())
				//{
				//	std::wstring wtext = CRIOManager::convertUTF8toUTF16(name.c_str());
				//	std::wstring invalidstring;
				//	int ipos = 0;
				//	int invalidwordsize;
				//	for (int i = 0; i < InvalidNameTab->getRowCount();i++)
				//	{
				//		invalidstring = CRIOManager::convertUTF8toUTF16(InvalidNameTab->getData(i,0).c_str());
				//		invalidwordsize = invalidstring.length();
				//		if (invalidwordsize <=  wtext.length())
				//		{
				//			ipos = wtext.find(invalidstring);
				//			if(ipos != std::string::npos)
				//			{
				//				nameinput->setColor(crVector4f(1.0f,0.0f,0.0f,1.0f));
				//				nameinput->setString(std::string("请勿使用符号、空格等非法字符"));
				//				cancelTask = true;
				//				break;
				//			}
				//		}
				//	}
				//}
				//InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidWordTab);
				//if (InvalidNameTab.valid())
				//{
				//	std::string invalidstring;
				//	for (int i = 0; i < InvalidNameTab->getRowCount();i++)
				//	{
				//		invalidstring = InvalidNameTab->getData(i,0);
				//		if(invalidstring.size() > 0 && name.find(invalidstring) != std::string::npos)
				//		{
				//			nameinput->setColor(crVector4f(1.0f,0.0f,0.0f,1.0f));
				//			nameinput->setString(std::string("不能含敏感词"));
				//			cancelTask = true;
				//			break;
				//		}
				//	}
				//}
				if(callback->insertCharacterName(playerid,name))
					code = 1;
			}
		}
		crCheckPlayerGameNamePacket packet;
		crCheckPlayerGameNamePacket::buildReplyPacket(packet,playerid,code);
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crCheckPlayerGameNamePacket packet;
			crCheckPlayerGameNamePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crBrain::getInstance()->doEvent(WCH_RecvCheckPlayerGameName,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crCreatePlayerGameDataPacket
//
/////////////////////////////////////////
crCreatePlayerGameDataPacket::crCreatePlayerGameDataPacket():
	crStreamPacket(crCreatePlayerGameDataPacket::ID)
{
}

crCreatePlayerGameDataPacket::crCreatePlayerGameDataPacket( const crCreatePlayerGameDataPacket &p ):
	crStreamPacket(crCreatePlayerGameDataPacket::ID,p)
{
}

void crCreatePlayerGameDataPacket::buildRequestPacket(crCreatePlayerGameDataPacket &packet)
{
}

void crCreatePlayerGameDataPacket::buildRequestPacket2(crCreatePlayerGameDataPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crCreatePlayerGameDataPacket::buildReplyPacket(crCreatePlayerGameDataPacket &packet,int recvid, char code, int mainroleid, int streamid, int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(17);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(code);
	if(code == 1 || code == 4)
	{
		streamBuf->_writeInt(mainroleid);
		streamBuf->_writeInt(streamid);
		streamBuf->_writeInt(count);
	}
}

void crCreatePlayerGameDataPacket::buildReplyPacket2(crCreatePlayerGameDataPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crCreatePlayerGameDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crCreatePlayerGameDataPacket packet;
				crCreatePlayerGameDataPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		char code = 0;
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		std::string name;
		if(playerData.valid() && callback->findCharacterName(playerid,name))
		{
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			if(playerGameData/* && playerGameData->getMainRoleID()==0*/)
			{
				int mainRoleAItemID = crGlobalHandle::gData()->gMainRoleAItemID();
				int mainRoleAMetierID = crGlobalHandle::gData()->gMainRoleAMetierID();
				crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
				if(gamedb)
				{
					if(playerGameData->getMainRoleID()==0)
					{
						if(mainRoleAItemID != 0 || mainRoleAMetierID != 0)
						{
							ref_ptr<crCreateInstanceItem> createInstanceItem = new crCreateInstanceItem;
							createInstanceItem->buildUpdateSql(mainRoleAItemID,crInstanceItem::Role,name,0,0,0,0,0);
							ref_ptr<crDataBase> gameSession = gamedb->beginSession();
							if(gameSession->executeUpdate(createInstanceItem.get()))
							{//插入成功
								int instanceitemid = gameSession->getInsertID();
								ref_ptr<crCreateRole> createRole = new crCreateRole;
								createRole->buildUpdateSql(playerid,instanceitemid,mainRoleAMetierID);
								if(gameSession->executeUpdate(createRole.get()))
								{
									int roleid = gameSession->getInsertID();

									ref_ptr<crInsertPlayerGameData> insertPlayerGameData = new crInsertPlayerGameData;
									insertPlayerGameData->buildUpdateSql(playerid,roleid);
									if(gameSession->executeUpdate(insertPlayerGameData.get()))
									{
										int id = gameSession->getInsertID();
										playerGameData->setID(id);
										gameSession->commit();
										gamedb->endSession(gameSession.get());
										code = 1;

										crDataBase *globalDB = crDataBaseManager::getInstance()->getGlobalDB();
										ref_ptr<crDataBase> globalSession = globalDB->beginSession();
										ref_ptr<crCharacterNameIDQueryData> characterNameIDQuery = new crCharacterNameIDQueryData;
										characterNameIDQuery->buildQuerySql(playerid,callback->getServerID(),roleid);

										globalSession->executeQuery(characterNameIDQuery.get());
										crDataBase::QueryResultVec& characterNameIDQueryResultVec = globalSession->getQueryResult();
										bool notexist = characterNameIDQueryResultVec.empty();
										globalSession->releaseQuery();
										if(notexist)
										{
											ref_ptr<crInsertCharacterName> insertCharacterName = new crInsertCharacterName;//如果已经存在，改成UPD
											insertCharacterName->buildUpdateSql(playerid,callback->getServerID(),roleid,name);
											if(globalSession->executeUpdate(insertCharacterName.get()))
											{
												globalSession->commit();
											}
										}
										else
										{
											ref_ptr<crUpdateCharacterName> updateCharacterName = new crUpdateCharacterName;//如果已经存在，改成UPD
											updateCharacterName->buildUpdateSql(playerid,callback->getServerID(),roleid,name);
											if(globalSession->executeUpdate(updateCharacterName.get()))
											{
												globalSession->commit();
											}
										}
										globalDB->endSession(globalSession.get());

										ref_ptr<crRole> role = new crRole;
										playerGameData->setMainRoleID(roleid);
										role->setItemtype(crInstanceItem::Role);
										role->setRoleID(roleid);
										role->setPlayerID(playerid);
										role->setInstanceItemID(instanceitemid);
										//role->setAbstractItemID(mainRoleAItemID);
										//role->getAbstractMetier()->setAbstractMetierID(mainRoleAMetierID);
										role->setIName(name);
										//int resid = 0;
										//std::string aname,itemscript,resscript;
										crAbstractMetier *ametier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(mainRoleAMetierID);
										role->setAbstractMetier(ametier);
										crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(mainRoleAItemID);
										role->setAbstractItem(aitem);

										playerGameData->setMainRole(role.get());
										role->setSceneID(0);
										role->setRoomID(0);
										role->loadMetierData(SaveStream);
										role->loadItemData(SaveStream,true);
										role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
										role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
										/////write and send
										int count = 0;
										std::vector< ref_ptr<crStreamBuf> > BufVec;
										ref_ptr<crStreamBuf> stream = new crStreamBuf;
										stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
										writeItem(role.get(),stream,BufVec,count,PlayerStream);
										if(stream->getBufSize()>0)
										{
											count += stream->getBufSize();
											BufVec.push_back(stream.get());
										}
										if(count>0)
										{
											int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
											crCreatePlayerGameDataPacket packet;
											crCreatePlayerGameDataPacket::buildReplyPacket(packet,playerid,code,roleid,streamid,count);
											m_netConductor->getNetManager()->sendPacket(sender,packet);
											crDataStreamPacket streamPacket;
											for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
												itr != BufVec.end();
												++itr )
											{
												crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,crCreatePlayerGameDataPacket::ID,itr->get());
												m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
											}
											crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
										}
										playerGameData->setDBValid(true);
									}
								}
							}
							if(code == 0)
							{
								gameSession->rollback();
								gamedb->endSession(gameSession.get());
							}
						}
						else
						{
							ref_ptr<crInsertPlayerGameData> insertPlayerGameData = new crInsertPlayerGameData;
							insertPlayerGameData->buildUpdateSql(playerid,0);
							ref_ptr<crDataBase> gameSession = gamedb->beginSession();
							if(gameSession->executeUpdate(insertPlayerGameData.get()))
							{
								int id = gameSession->getInsertID();
								playerGameData->setID(id);
								gameSession->commit();
								playerGameData->setDBValid(true);
								code = 2;
							}
							else
							{
								gameSession->rollback();
							}
							gamedb->endSession(gameSession.get());
						}
					}
					else
					{
						code = 5;//角色已经存在，不能重新创建角色
						//if(mainRoleAItemID != 0 || mainRoleAMetierID != 0)
						//{
						//	int roleid = playerGameData->getMainRoleID();
						//	crDataBase *globalDB = crDataBaseManager::getInstance()->getGlobalDB();
						//	ref_ptr<crDataBase> globalSession = globalDB->beginSession();
						//	ref_ptr<crUpdateCharacterName> updateCharacterName = new crUpdateCharacterName;//如果已经存在，改成UPD
						//	updateCharacterName->buildUpdateSql(playerid,callback->getServerID(),roleid,name);
						//	if(globalSession->executeUpdate(updateCharacterName.get()))
						//	{
						//		globalSession->commit();
						//	}
						//	globalDB->endSession(globalSession.get());

						//	ref_ptr<crDataBase> gameSession = gamedb->beginSession();
						//	int instanceitemid = 0;
						//	bool game_ended = false;
						//	ref_ptr<crRoleQuery2Data> roleQuery = new crRoleQuery2Data;
						//	roleQuery->buildQuerySql(roleid);
						//	gameSession->executeQuery(roleQuery.get());
						//	crDataBase::QueryResultVec &roleQueryResultVec = gameSession->getQueryResult();
						//	int ametierid = 0;
						//	if(!roleQueryResultVec.empty())
						//	{
						//		roleQuery = dynamic_cast<crRoleQuery2Data*>(roleQueryResultVec[0].get());
						//		instanceitemid = roleQuery->getInstanceItemID();
						//	}
						//	gameSession->releaseQuery();
						//	if(instanceitemid>0)
						//	{
						//		ref_ptr<crUpdateInstanceItem> updateInstraceItem = new crUpdateInstanceItem;
						//		updateInstraceItem->buildUpdateSql(instanceitemid,name);
						//		if(gameSession->executeUpdate(updateInstraceItem.get()))
						//		{
						//			gameSession->commit();
						//		}
						//	}
						//	gamedb->endSession(gameSession.get());
						//	if(instanceitemid>0)
						//	{
						//		code = 4;
						//		ref_ptr<crRole> role = new crRole;
						//		playerGameData->setMainRoleID(roleid);
						//		role->setItemtype(crInstanceItem::Role);
						//		role->setRoleID(roleid);
						//		role->setPlayerID(playerid);
						//		role->setInstanceItemID(instanceitemid);
						//		//role->setAbstractItemID(mainRoleAItemID);
						//		//role->getAbstractMetier()->setAbstractMetierID(mainRoleAMetierID);
						//		role->setIName(name);
						//		//int resid = 0;
						//		//std::string aname,itemscript,resscript;
						//		crAbstractMetier *ametier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(mainRoleAMetierID);
						//		role->setAbstractMetier(ametier);
						//		crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(mainRoleAItemID);
						//		role->setAbstractItem(aitem);

						//		playerGameData->setMainRole(role.get());
						//		role->setSceneID(0);
						//		role->setRoomID(0);
						//		role->loadMetierData(SaveStream);
						//		role->loadItemData(SaveStream,true);
						//		role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
						//		role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
						//		/////write and send
						//		int count = 0;
						//		std::vector< ref_ptr<crStreamBuf> > BufVec;
						//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
						//		stream->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
						//		writeItem(role.get(),stream,BufVec,count,PlayerStream);
						//		if(stream->getBufSize()>0)
						//		{
						//			count += stream->getBufSize();
						//			BufVec.push_back(stream.get());
						//		}
						//		if(count>0)
						//		{
						//			int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
						//			crCreatePlayerGameDataPacket packet;
						//			crCreatePlayerGameDataPacket::buildReplyPacket(packet,playerid,code,roleid,streamid,count);
						//			m_netConductor->getNetManager()->sendPacket(sender,packet);
						//			crDataStreamPacket streamPacket;
						//			for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
						//				itr != BufVec.end();
						//				++itr )
						//			{
						//				crDataStreamPacket::buildRequestPacket(streamPacket,playerid,streamid,crCreatePlayerGameDataPacket::ID,itr->get());
						//				m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
						//			}
						//			crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
						//		}
						//		playerGameData->setDBValid(true);
						//	}
						//}
					}
				}
			}
			else
			{
				code = 3;
			}
		}
		if(code != 1 && code != 4)
		{
			crCreatePlayerGameDataPacket packet;
			crCreatePlayerGameDataPacket::buildReplyPacket(packet,playerid,code);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crCreatePlayerGameDataPacket packet;
			crCreatePlayerGameDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		//char *ptr = m_streamBuf->getPtr();
		char code = m_streamBuf->_readChar();
		if(code==1||code==4)
		{
			if(code == 1)
				crMyPlayerData::getInstance()->setLoadMainRoleCase(3);//3创建角色
			else
				crMyPlayerData::getInstance()->setLoadMainRoleCase(5);//5重新创建角色
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			playerGameData->setDBValid(true);
			playerGameData->setMainRoleID(m_streamBuf->_readInt());
			int streamid = m_streamBuf->_readInt();
			int count = m_streamBuf->_readInt();
			if(count>0)
				crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crCreatePlayerGameDataPacket::ID,count);
			else
			{
				CRCore::notify(CRCore::WARN)<<"crCreatePlayerGameDataPacket faild"<<std::endl;
			}
		}
		else
		{
			int texid = code==3?1068:1098;//创建角色失败！|角色已经存在，请尝试重新登录，如果仍然无法进入游戏，请与客服联系！
			crHandle *noticeHandle = crBrain::getInstance()->getDataClass()->getHandle(WCH_USER+310);
			noticeHandle->inputParam(387,&texid);
			int mode = 1;
			noticeHandle->inputParam(388,&mode);
			crGlobalHandle::getInstance()->doEvent(WCH_USER+310);//WCH_GlobalNotice
		}
		//char *ptr2 = m_streamBuf->getPtr();
		//m_streamBuf->seek(ptr-ptr2);
		//crGlobalHandle::getInstance()->doEvent(WCH_NetReturnStream,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crPlayerDataCreateStreamPacket
//
/////////////////////////////////////////
crPlayerDataCreateStreamPacket::crPlayerDataCreateStreamPacket():
	crStreamPacket(crPlayerDataCreateStreamPacket::ID)
{
}

crPlayerDataCreateStreamPacket::crPlayerDataCreateStreamPacket( const crPlayerDataCreateStreamPacket &p ):
	crStreamPacket(crPlayerDataCreateStreamPacket::ID,p)
{
}

void crPlayerDataCreateStreamPacket::buildRequestPacket(crPlayerDataCreateStreamPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crPlayerDataCreateStreamPacket::buildRequestPacket2(crPlayerDataCreateStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataCreateStreamPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crPlayerDataCreateStreamPacket packet;
				crPlayerDataCreateStreamPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData && playerGameData->getDataClass())
			{
				crData *data = playerGameData->getDataClass();
				data->inputParam(WCHDATA_CreateStream,m_streamBuf.get());
				playerGameData->doEvent(WCH_RecvPlayerGameDataCreateStream);
				//角色创建成功。WCHDATA_JXJHomeID被设置上,立即存储
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
				callback->savePlayerData(netPlayerData.get());
			}
		}
	}
}
/////////////////////////////////////////
//
//crPlayerDataUpdateStreamPacket
//
/////////////////////////////////////////
crPlayerDataUpdateStreamPacket::crPlayerDataUpdateStreamPacket():
	crStreamPacket(crPlayerDataUpdateStreamPacket::ID)
{
}

crPlayerDataUpdateStreamPacket::crPlayerDataUpdateStreamPacket( const crPlayerDataUpdateStreamPacket &p ):
	crStreamPacket(crPlayerDataUpdateStreamPacket::ID,p)
{
}

void crPlayerDataUpdateStreamPacket::buildReplyPacket(crPlayerDataUpdateStreamPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crPlayerDataUpdateStreamPacket::buildReplyPacket2(crPlayerDataUpdateStreamPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataUpdateStreamPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataUpdateStreamPacket packet;
			crPlayerDataUpdateStreamPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			crData *data = gameData->getDataClass();
			data->inputParam(WCHDATA_UpdateStream,m_streamBuf.get());
		}
	}
}
/////////////////////////////////////////
//
//crPlayerDataEventPacket
//
/////////////////////////////////////////
crPlayerDataEventPacket::crPlayerDataEventPacket():
	crStreamPacket(crPlayerDataEventPacket::ID)
{
}

crPlayerDataEventPacket::crPlayerDataEventPacket( const crPlayerDataEventPacket &p ):
	crStreamPacket(crPlayerDataEventPacket::ID,p)
{
}

void crPlayerDataEventPacket::buildRequestPacket(crPlayerDataEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
//void crPlayerDataEventPacket::buildRequestPacket(crPlayerDataEventPacket &packet, int playerid, _crInt64 msg, CRCore::crStreamBuf *param)
//{//server send to client
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(12+(param?param->getBufSize():0));
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(playerid);
//	streamBuf->_writeInt64(msg);
//	streamBuf->write(param->getBuf(),param->getBufSize());
//}
void crPlayerDataEventPacket::buildRequestPacket2(crPlayerDataEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataEventPacket::buildRequestPacket3(crPlayerDataEventPacket &packet, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventPacket::buildReplyPacket(crPlayerDataEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventPacket::buildReplyPacket_p(crPlayerDataEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getRemainSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream) streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataEventPacket::buildReplyPacket2(crPlayerDataEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventPacket::buildReplyPacket3(crPlayerDataEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			//crNetConductor *conductor = playerData->getSceneServerLineConductor();
			//if(!conductor)
			//{
			//	conductor = playerData->getGameServerLineConductor();
			//}
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor && m_streamBuf.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				
				crNetConductor *conductor = netPlayerData->getGameServerLineConductor();
				if(conductor)
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket3(packet,m_streamBuf.get());
					conductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
				sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket3(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket3(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			_crInt64 msg = m_streamBuf->_readInt64();
			if(msg == WCH_SomeOneTestLogin)
			{
				//connectServer->getNetManager()->removeConnection(playerData->getPlayerAdress());
				connectServer->getNetDataManager()->removePlayerData(playerid);
			}
		}
	}
	else if(netType == GameClient_Game)
	{
		ref_ptr<crPlayerGameData> playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData.valid())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}
/////////////////////////////////////////
//
//crPlayerDataEventZipPacket
//
/////////////////////////////////////////
crPlayerDataEventZipPacket::crPlayerDataEventZipPacket():
	crStreamPacket(crPlayerDataEventZipPacket::ID)
{
}

crPlayerDataEventZipPacket::crPlayerDataEventZipPacket( const crPlayerDataEventZipPacket &p ):
	crStreamPacket(crPlayerDataEventZipPacket::ID,p)
{
}

void crPlayerDataEventZipPacket::buildRequestPacket(crPlayerDataEventZipPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerDataEventZipPacket::buildRequestPacket2(crPlayerDataEventZipPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataEventZipPacket::buildRequestPacket3(crPlayerDataEventZipPacket &packet, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventZipPacket::buildReplyPacket(crPlayerDataEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventZipPacket::buildReplyPacket_p(crPlayerDataEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getRemainSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream) streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataEventZipPacket::buildReplyPacket2(crPlayerDataEventZipPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataEventZipPacket::buildReplyPacket3(crPlayerDataEventZipPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataEventZipPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			//crNetConductor *conductor = playerData->getSceneServerLineConductor();
			//if(!conductor)
			//{
			//	conductor = playerData->getGameServerLineConductor();
			//}
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor && m_streamBuf.valid())
			{
				crPlayerDataEventZipPacket packet;
				crPlayerDataEventZipPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));

				crNetConductor *conductor = netPlayerData->getGameServerLineConductor();
				if(conductor)
				{
					crPlayerDataEventZipPacket packet;
					crPlayerDataEventZipPacket::buildRequestPacket3(packet,m_streamBuf.get());
					conductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
				crPlayerDataEventZipPacket packet;
				crPlayerDataEventZipPacket::buildReplyPacket2(packet,m_streamBuf.get());
				sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataEventZipPacket packet;
			crPlayerDataEventZipPacket::buildReplyPacket3(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataEventZipPacket packet;
			crPlayerDataEventZipPacket::buildReplyPacket3(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			_crInt64 msg = m_streamBuf->_readInt64();
			if(msg == WCH_SomeOneTestLogin)
			{
				//connectServer->getNetManager()->removeConnection(playerData->getPlayerAdress());
				connectServer->getNetDataManager()->removePlayerData(playerid);
			}
		}
	}
	else if(netType == GameClient_Game)
	{
		ref_ptr<crPlayerGameData> playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData.valid())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}
/////////////////////////////////////////
//
//crPlayerDataSceneEventPacket
//
/////////////////////////////////////////
crPlayerDataSceneEventPacket::crPlayerDataSceneEventPacket():
	crStreamPacket(crPlayerDataSceneEventPacket::ID)
{
}

crPlayerDataSceneEventPacket::crPlayerDataSceneEventPacket( const crPlayerDataSceneEventPacket &p ):
	crStreamPacket(crPlayerDataSceneEventPacket::ID,p)
{
}
crPlayerDataSceneEventPacket::crPlayerDataSceneEventPacket(int id):
	crStreamPacket(id)
{
}
crPlayerDataSceneEventPacket::crPlayerDataSceneEventPacket( int id, const crStreamPacket &p ):
	crStreamPacket(id,p)
{
}
void crPlayerDataSceneEventPacket::buildRequestPacket(crPlayerDataSceneEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerDataSceneEventPacket::buildRequestPacket2(crPlayerDataSceneEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataSceneEventPacket::buildReplyPacket(crPlayerDataSceneEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream)
		streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataSceneEventPacket::buildReplyPacket2(crPlayerDataSceneEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataSceneEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		ref_ptr<crPlayerGameData> playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData.valid())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}
/////////////////////////////////////////
//
//crPlayerDataSceneEventZipPacket
//
/////////////////////////////////////////
crPlayerDataSceneEventZipPacket::crPlayerDataSceneEventZipPacket():
	crStreamPacket(crPlayerDataSceneEventZipPacket::ID)
{
}

crPlayerDataSceneEventZipPacket::crPlayerDataSceneEventZipPacket( const crPlayerDataSceneEventZipPacket &p ):
	crStreamPacket(crPlayerDataSceneEventZipPacket::ID,p)
{
}
crPlayerDataSceneEventZipPacket::crPlayerDataSceneEventZipPacket(int id):
	crStreamPacket(id)
{
}
crPlayerDataSceneEventZipPacket::crPlayerDataSceneEventZipPacket( int id, const crStreamPacket &p ):
	crStreamPacket(id,p)
{
}
void crPlayerDataSceneEventZipPacket::buildRequestPacket(crPlayerDataSceneEventZipPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crPlayerDataSceneEventZipPacket::buildRequestPacket2(crPlayerDataSceneEventZipPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crPlayerDataSceneEventZipPacket::buildReplyPacket(crPlayerDataSceneEventZipPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(stream)
		streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crPlayerDataSceneEventZipPacket::buildReplyPacket2(crPlayerDataSceneEventZipPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crPlayerDataSceneEventZipPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crPlayerDataSceneEventZipPacket packet;
				crPlayerDataSceneEventZipPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			if(playerGameData)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crPlayerDataSceneEventZipPacket packet;
			crPlayerDataSceneEventZipPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		ref_ptr<crPlayerGameData> playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData.valid())
		{
			_crInt64 msg = m_streamBuf->_readInt64();
			playerGameData->doEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}
/////////////////////////////////////////
//
//crEyePointsPacket
//
/////////////////////////////////////////
crEyePointsPacket::crEyePointsPacket():
	crStreamPacket(crEyePointsPacket::ID)
{
}

crEyePointsPacket::crEyePointsPacket( const crEyePointsPacket &p ):
	crStreamPacket(crEyePointsPacket::ID,p)
{
}

void crEyePointsPacket::buildReplyPacket(crEyePointsPacket &packet, int recvid, std::vector<CRCore::crVector2i>&eyepoints)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5+eyepoints.size() * 8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUChar(eyepoints.size());
	for( std::vector<CRCore::crVector2i>::iterator itr = eyepoints.begin();
		 itr != eyepoints.end();
		 ++itr )
	{
		streamBuf->_writeVec2i(*itr);
	}
}
void crEyePointsPacket::buildReplyPacket2(crEyePointsPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crEyePointsPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crEyePointsPacket packet;
			crEyePointsPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		unsigned char size = m_streamBuf->_readChar();
		crVector2i eye;
		crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
		for(unsigned char i = 0; i<size; i++)
		{
			eye = m_streamBuf->_readVec2i();
			myPlayerData->insertEyePoint(eye);
		}
	}
}
/////////////////////////////////////////
//
//crCheckPlayerDelayTimePacket
//
/////////////////////////////////////////
crCheckPlayerDelayTimePacket::crCheckPlayerDelayTimePacket():
	crStreamPacket(crCheckPlayerDelayTimePacket::ID)
{
}
crCheckPlayerDelayTimePacket::crCheckPlayerDelayTimePacket( const crCheckPlayerDelayTimePacket &p ):
	crStreamPacket(crCheckPlayerDelayTimePacket::ID,p)
{
}
void crCheckPlayerDelayTimePacket::buildReplyPacket(crCheckPlayerDelayTimePacket &packet, int recvid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	Timer_t time = CRCore::Timer::instance()->tick();
	streamBuf->_writeInt64(time);
}
void crCheckPlayerDelayTimePacket::buildReplyPacket2(crCheckPlayerDelayTimePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crCheckPlayerDelayTimePacket::buildRequestPacket2(crCheckPlayerDelayTimePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crCheckPlayerDelayTimePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crCheckPlayerDelayTimePacket packet;
			crCheckPlayerDelayTimePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		m_netConductor->getNetManager()->sendPacket("all",*this);
	}
	else if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crCheckPlayerDelayTimePacket packet;
				crCheckPlayerDelayTimePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			Timer_t current = CRCore::Timer::instance()->tick();
			Timer_t t = m_streamBuf->_readInt64();
			float delay = CRCore::Timer::instance()->delta_m(t,current) * 0.5f;
			netPlayerData->setDelayTime(delay);
			CRCore::notify(CRCore::WARN)<<"crCheckPlayerDelayTimePacket Delay = "<<delay<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crStopMyRolesPacket
//
/////////////////////////////////////////
crStopMyRolesPacket::crStopMyRolesPacket():
	crStreamPacket(crStopMyRolesPacket::ID)
{
}

crStopMyRolesPacket::crStopMyRolesPacket( const crStopMyRolesPacket &p ):
	crStreamPacket(crStopMyRolesPacket::ID,p)
{
}

void crStopMyRolesPacket::buildRequestPacket(crStopMyRolesPacket &packet)
{
}

void crStopMyRolesPacket::buildRequestPacket2(crStopMyRolesPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crStopMyRolesPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crStopMyRolesPacket packet;
				crStopMyRolesPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStopMyRolesPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//SceneServer
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			crRole *role;
			void *param;
			crData *data;
			unsigned char itemstate;
			unsigned char stopstate = IS_Stop;
			for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
				 itr != roleMap.end();
				 ++itr )
			{
				role = itr->second.get();
				data = role->getDataClass();
				if(data)
				{
					data->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char*)param;
					if(itemstate != IS_Dead)
					{
						data->inputParam(WCHDATA_ItemState,&stopstate);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDBSavePacket
//
/////////////////////////////////////////
crDBSavePacket::crDBSavePacket():
	crStreamPacket(crDBSavePacket::ID)
{
}

crDBSavePacket::crDBSavePacket( const crDBSavePacket &p ):
	crStreamPacket(crDBSavePacket::ID,p)
{
}
void crDBSavePacket::buildRequestPacket(crDBSavePacket &packet,CRDataBase::crUpdateSql *sql,char flg)
{
 	ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
	crStreamBuf *sqlstream = sql->getSqlStream();
	if(sqlstream)
	{
		streamBuf->createBuf(1+sqlstream->getBufSize());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(flg);
		streamBuf->write(sqlstream->getBuf(),sqlstream->getBufSize());
	}
	else
	{
		const std::string &sqlstr = sql->getUpdateSql();
		streamBuf->createBuf(1+sqlstr.length());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(flg);
		streamBuf->write(sqlstr.c_str(),sqlstr.length());
	}
}

void crDBSavePacket::buildRequestPacket(crDBSavePacket &packet,unsigned short enumid, crUpdateSql *sql)
{
	ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
	crStreamBuf *sqlstream = sql->getSqlStream();
	if(sqlstream)
	{
		streamBuf->createBuf(3+sqlstream->getBufSize());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(1);
		streamBuf->_writeUShort(enumid);
		streamBuf->write(sqlstream->getBuf(),sqlstream->getBufSize());
	}
	else
	{
		const std::string &sqlstr = sql->getUpdateSql();
		streamBuf->createBuf(3+sqlstr.length());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(1);
		streamBuf->_writeUShort(enumid);
		streamBuf->write(sqlstr.c_str(),sqlstr.length());
	}
}

void crDBSavePacket::buildRequestPacket(crDBSavePacket &packet,unsigned short enumid, unsigned short dbid, crUpdateSql *sql)
{
	ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
	crStreamBuf *sqlstream = sql->getSqlStream();
	if(sqlstream)
	{
		streamBuf->createBuf(5+sqlstream->getBufSize());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(2);
		streamBuf->_writeUShort(enumid);
		streamBuf->_writeUShort(dbid);
		streamBuf->write(sqlstream->getBuf(),sqlstream->getBufSize());
	}
	else
	{
		const std::string &sqlstr = sql->getUpdateSql();
		streamBuf->createBuf(5+sqlstr.length());
		packet.setStreamBuf(streamBuf.get());
		streamBuf->_writeChar(2);
		streamBuf->_writeUShort(enumid);
		streamBuf->_writeUShort(dbid);
		streamBuf->write(sqlstr.c_str(),sqlstr.length());
	}
}

void crDBSavePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{
		crBlockDetectThread::getInstance()->update();
		if(!m_streamBuf.valid())
			return;
		crDataBase *db = NULL;
		char dbtype = m_streamBuf->_readChar();
		if(dbtype == -1)
		{
			db = crDataBaseManager::getInstance()->getAccountDB();
		}
		else if(dbtype == 0)
		{
			db = crDataBaseManager::getInstance()->getGlobalDB();
		}
		else if(dbtype == 1)
		{
			unsigned short enumid = m_streamBuf->_readUShort();
			db = crDataBaseManager::getInstance()->getDataBase(enumid);
		}
		else if(dbtype == 2)
		{
			unsigned short enumid = m_streamBuf->_readUShort();
			unsigned short dbid = m_streamBuf->_readUShort();
			db = crDataBaseManager::getInstance()->getDataBase(enumid,dbid);
		}
		if(db)
		{
			//crDBServerCallback *callback = dynamic_cast<crDBServerCallback*>(m_netConductor->getNetDataManager()->getNetCallback());
			//char *buf = new char[m_streamBuf->getRemainSize()+1];
			//memset(buf,0,m_streamBuf->getRemainSize()+1);
			//memcpy(buf,m_streamBuf->getPtr(),m_streamBuf->getRemainSize());
			//callback->saveDBLog(buf);
			//delete buf;
			CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
			ref_ptr<crDataBase> session = db->beginSession();
			bool success = false;
			//if(m_streamBuf->getRemainSize()>10000)
			//{
			//	crDataBaseManager::getInstance()->dbErrorRecord(m_streamBuf->getPtr(),m_streamBuf->getRemainSize(),0);
			//}
			if(session->executeUpdate(m_streamBuf->getPtr(),m_streamBuf->getRemainSize()))
			{
				session->commit();
				success = true;
			}
			else
			{
				session->rollback();
			}
			db->endSession(session.get());
			time_t t = time( 0 ); 
			char tmp[20];
			strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
			CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
			CRCore::notify(CRCore::ALWAYS)<<tmp<<" DBSave,大小"<<m_streamBuf->getRemainSize()<<" 耗时"<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<"s"<<std::endl;
			//if(success)
			//	callback->saveDBLog("crDBSavePacket成功");
			//else
			//	callback->saveDBLog("crDBSavePacket失败");
		}
	}
}
/////////////////////////////////////////
//
//crServerListUpdatePacket
//
/////////////////////////////////////////
crServerListUpdatePacket::crServerListUpdatePacket():
	crStreamPacket(crServerListUpdatePacket::ID)
{
}

crServerListUpdatePacket::crServerListUpdatePacket( const crServerListUpdatePacket &p ):
	crStreamPacket(crServerListUpdatePacket::ID,p)
{
}

void crServerListUpdatePacket::buildRequestPacket(crServerListUpdatePacket &packet,int serverid,short remaincount,short onlinecount)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(serverid);
	streamBuf->_writeShort(remaincount);
	streamBuf->_writeShort(onlinecount);
}

void crServerListUpdatePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{//DBServer
		int serverid = m_streamBuf->_readInt();
		short remaincount = m_streamBuf->_readShort();
		short onlinecount = m_streamBuf->_readShort();

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crUpdateServerList> updateServerList = new crUpdateServerList;
		updateServerList->buildUpdateSql(serverid,remaincount,onlinecount);
		if(globalSession->executeUpdate(updateServerList.get()))
		{//插入成功
			globalSession->commit();
		}
		db->endSession(globalSession.get());
	}
}
/////////////////////////////////////////
//
//crServerListQueryPacket
//
/////////////////////////////////////////
crServerListQueryPacket::crServerListQueryPacket():
	crStreamPacket(crServerListQueryPacket::ID)
{
}

crServerListQueryPacket::crServerListQueryPacket( const crServerListQueryPacket &p ):
	crStreamPacket(crServerListQueryPacket::ID,p)
{
}

void crServerListQueryPacket::buildRequestPacket(crServerListQueryPacket &packet,int gameid,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(gameid);
	streamBuf->_writeInt(playerid);
}
void crServerListQueryPacket::buildRequestPacket2(crServerListQueryPacket &packet,const std::string &playerAddress,crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crServerListQueryPacket::buildReplyPacket(crServerListQueryPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+5+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	if(stream)
	{
		streamBuf->_writeBool(true);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->_writeBool(false);
	}
}
void crServerListQueryPacket::buildReplyPacket2(crServerListQueryPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crServerListQueryPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到GameServer
		crServerListQueryPacket packet;
		crServerListQueryPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crServerListQueryPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//
		std::string playerAddress = m_streamBuf->_readString();
		int gameid = m_streamBuf->_readInt();
		int playerid = 0;
		if(!m_streamBuf->eof())
			playerid = m_streamBuf->_readInt();
		crServerListQueryPacket packet;
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crServerListQueryData> serverListQuery = new crServerListQueryData;
		if(crGlobalHandle::gData()->getGMAuthority(playerid)<0)
		{//非GM
			serverListQuery->buildQuerySql(gameid);
		}
		else
		{
			serverListQuery->buildQuerySql_gm(gameid);
		}
		globalSession->executeQuery(serverListQuery.get());
		crDataBase::QueryResultVec& serverListQueryResultVec = globalSession->getQueryResult();
		if(!serverListQueryResultVec.empty())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			unsigned short count = serverListQueryResultVec.size();
			stream->createBuf(count*31+2);
			stream->_writeUShort(count);
			for( crDataBase::QueryResultVec::iterator itr = serverListQueryResultVec.begin();
				 itr != serverListQueryResultVec.end();
				 ++itr )
			{
				serverListQuery = dynamic_cast<crServerListQueryData *>(itr->get());
				stream->_writeInt(serverListQuery->getID());//4
				stream->_writeString(serverListQuery->getServerName());//24
				stream->_writeUChar(serverListQuery->getISP());//1
				stream->_writeUShort(serverListQuery->getRemainCount());//2
			}
			crServerListQueryPacket::buildReplyPacket(packet,playerAddress,stream.get());
		}
		else
		{
			crServerListQueryPacket::buildReplyPacket(packet,playerAddress,NULL);
		}
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crServerListQueryPacket packet;
			crServerListQueryPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crServerListQueryPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvServerList,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crConnectServerInitPacket
//
/////////////////////////////////////////
crConnectServerInitPacket::crConnectServerInitPacket():
	crStreamPacket(crConnectServerInitPacket::ID)
{
}

crConnectServerInitPacket::crConnectServerInitPacket( const crConnectServerInitPacket &p ):
	crStreamPacket(crConnectServerInitPacket::ID,p)
{
}

void crConnectServerInitPacket::buildRequestPacket(crConnectServerInitPacket &packet,const std::string &ip,const std::string &ip2,const std::string &ip3,unsigned short port,short remaincount)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(ip.length()+ip2.length()+ip3.length()+16);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(ip);
	streamBuf->_writeString(ip2);
	streamBuf->_writeString(ip3);
	streamBuf->_writeUShort(port);
	streamBuf->_writeShort(remaincount);
}

void crConnectServerInitPacket::buildRequestPacket2(crConnectServerInitPacket &packet,int serverid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(serverid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crConnectServerInitPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{//转发到GameServer
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		crConnectServerInitPacket dbpacket;
		crConnectServerInitPacket::buildRequestPacket2(dbpacket,callback->getServerID(),m_streamBuf.get());
		dbConductor->getNetManager()->sendPacket("all",dbpacket);
	}
	else if(netType == DBServer)
	{//DBServer
		int serverid = m_streamBuf->_readInt();
		std::string ip = m_streamBuf->_readString();
		std::string ip2 = m_streamBuf->_readString();
		std::string ip3 = m_streamBuf->_readString();
		unsigned short port = m_streamBuf->_readUShort();
		short remaincount = m_streamBuf->_readShort();
		
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		//查询并返回GameList
		ref_ptr<crConnectServerIDQueryData> query = new crConnectServerIDQueryData;
		query->buildQuerySql(serverid,port);
		globalSession->executeQuery(query.get());
		crDataBase::QueryResultVec resultVec = globalSession->getQueryResult();
		globalSession->releaseQuery();
		if(resultVec.empty())
		{
			ref_ptr<crInsertConnectServers> insert = new crInsertConnectServers;
			insert->buildUpdateSql(serverid,ip,ip2,ip3,port,remaincount);
			if(globalSession->executeUpdate(insert.get()))
			{//插入成功
				globalSession->commit();
			}
		}
		else
		{
			ref_ptr<crUpdateConnectServers> update = new crUpdateConnectServers;
			update->buildUpdateSql(serverid,ip,ip2,ip3,port,remaincount);
			if(globalSession->executeUpdate(update.get()))
			{//插入成功
				globalSession->commit();
			}
		}
		db->endSession(globalSession.get());
	}
}
/////////////////////////////////////////
//
//crConnectServerUpdatePacket
//
/////////////////////////////////////////
crConnectServerUpdatePacket::crConnectServerUpdatePacket():
	crStreamPacket(crConnectServerUpdatePacket::ID)
{
}

crConnectServerUpdatePacket::crConnectServerUpdatePacket( const crConnectServerUpdatePacket &p ):
	crStreamPacket(crConnectServerUpdatePacket::ID,p)
{
}

void crConnectServerUpdatePacket::buildRequestPacket(crConnectServerUpdatePacket &packet,unsigned short port,short remaincount)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUShort(port);
	streamBuf->_writeShort(remaincount);
}

void crConnectServerUpdatePacket::buildRequestPacket2(crConnectServerUpdatePacket &packet,int serverid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(serverid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crConnectServerUpdatePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{//转发到GameServer
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		crConnectServerUpdatePacket dbpacket;
		crConnectServerUpdatePacket::buildRequestPacket2(dbpacket,callback->getServerID(),m_streamBuf.get());
		dbConductor->getNetManager()->sendPacket("all",dbpacket);
	}
	else if(netType == DBServer)
	{//DBServer
		int serverid = m_streamBuf->_readInt();
		unsigned short port = m_streamBuf->_readUShort();
		short remaincount = m_streamBuf->_readShort();

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crUpdateConnectServers> update = new crUpdateConnectServers;
		update->buildUpdateSql2(serverid,port,remaincount);
		if(globalSession->executeUpdate(update.get()))
		{//插入成功
			globalSession->commit();
		}
		db->endSession(globalSession.get());
	}
}
/////////////////////////////////////////
//
//crConnectServerQueryPacket
//
/////////////////////////////////////////
crConnectServerQueryPacket::crConnectServerQueryPacket():
	crStreamPacket(crConnectServerQueryPacket::ID)
{
}

crConnectServerQueryPacket::crConnectServerQueryPacket( const crConnectServerQueryPacket &p ):
	crStreamPacket(crConnectServerQueryPacket::ID,p)
{
}

void crConnectServerQueryPacket::buildRequestPacket(crConnectServerQueryPacket &packet,int serverid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(serverid);
	streamBuf->_writeInt(crMyPlayerData::getInstance()->getPlayerID());
}
void crConnectServerQueryPacket::buildRequestPacket2(crConnectServerQueryPacket &packet,const std::string &playerAddress,crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crConnectServerQueryPacket::buildReplyPacket(crConnectServerQueryPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+5+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	if(stream)
	{
		streamBuf->_writeBool(true);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->_writeBool(false);
	}
}
void crConnectServerQueryPacket::buildReplyPacket2(crConnectServerQueryPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crConnectServerQueryPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到GameServer
		crConnectServerQueryPacket packet;
		crConnectServerQueryPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crConnectServerQueryPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//
		std::string playerAddress = m_streamBuf->_readString();
		int serverid = m_streamBuf->_readInt();
		int playerid = 0;
		if(!m_streamBuf->eof())
			playerid = m_streamBuf->_readInt();
		bool success = false;
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crConnectServersQueryData> query = new crConnectServersQueryData;
		query->buildQuerySql(serverid);
		globalSession->executeQuery(query.get());
		crDataBase::QueryResultVec& queryResultVec = globalSession->getQueryResult();
		if(!queryResultVec.empty())
		{
			ref_ptr<crQueryConnectServerData> connectServerData;
			ConnectServerMap connectServerMap;
			for( crDataBase::QueryResultVec::iterator itr = queryResultVec.begin();
				itr != queryResultVec.end();
				++itr )
			{
				query = dynamic_cast<crConnectServersQueryData *>(itr->get());
				if(query->getRemainCount()>0)
				{
					connectServerData = new crQueryConnectServerData;
					connectServerData->setIP(query->getIP());//19
					connectServerData->setIP2(query->getIP2());//19
					connectServerData->setIP3(query->getIP3());//19
					connectServerData->setPort(query->getPort());//2
					connectServerData->setRemainCount(query->getRemainCount());
					connectServerMap.insert(std::make_pair(connectServerData->getRemainCount(),connectServerData.get()));
				}
			}
			crConnectServerQueryPacket packet;
			if(!connectServerMap.empty())
			{
				const std::string &ip = connectServerMap.begin()->second->getIP();
				const std::string &ip2 = connectServerMap.begin()->second->getIP2();
				const std::string &ip3 = connectServerMap.begin()->second->getIP3();
				unsigned short port = connectServerMap.begin()->second->getPort();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(ip.length()+ip2.length()+ip3.length()+14);
				stream->_writeString(ip);
				stream->_writeString(ip2);
				stream->_writeString(ip3);
				stream->_writeUShort(port);
				crConnectServerQueryPacket::buildReplyPacket(packet,playerAddress,stream.get());
				success = true;
			}
			else
			{
				crConnectServerQueryPacket::buildReplyPacket(packet,playerAddress,NULL);
			}
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
		globalSession->releaseQuery();
		if(success && playerid>0)
		{
			ref_ptr<crUpdateLoginServerID> updateLoginServerID = new crUpdateLoginServerID;
			updateLoginServerID->buildUpdateSql(playerid,serverid);
			if(globalSession->executeUpdate(updateLoginServerID.get()))
			{//插入成功
				globalSession->commit();
			}
		}
		db->endSession(globalSession.get());
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crConnectServerQueryPacket packet;
			crConnectServerQueryPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crConnectServerQueryPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvConnectServer,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crRoomEventPacket
//
/////////////////////////////////////////
crRoomEventPacket::crRoomEventPacket():
	crStreamPacket(crRoomEventPacket::ID)
{
}

crRoomEventPacket::crRoomEventPacket( const crRoomEventPacket &p ):
	crStreamPacket(crRoomEventPacket::ID,p)
{
}

void crRoomEventPacket::buildRequestPacket(crRoomEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crRoomEventPacket::buildRequestPacket2(crRoomEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomEventPacket::buildRequestPacket(crRoomEventPacket &packet, int playerid, _crInt64 msg, CRCore::crStreamBuf *param)
{//server send to client
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crRoomEventPacket::buildReplyPacket(crRoomEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crRoomEventPacket::buildReplyPacket2(crRoomEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crRoomEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crRoomEventPacket packet;
				crRoomEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRoomPlayerEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> netPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid() && netPlayerData->getPlayerGameData())
		{
			crPlayerGameData *playerGameData = netPlayerData->getPlayerGameData();
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			crRoom* room = callback->findRoom(netPlayerData->getRoomID());
			if(room)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				CRCore::crVector2i nettypepair(netType,playerid);
				room->doEvent(msg,MAKEINT64(m_streamBuf.get(),&nettypepair));
				//crRoomEventPacket packet;
				//crRoomEventPacket::buildReplyPacket(packet,0,m_streamBuf.get());
				//room->sendPacketToAll(packet,playerid);
				//CRCore::notify(CRCore::ALWAYS)<<"crRoomPlayerEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crRoomEventPacket packet;
			crRoomEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		_crInt64 msg = m_streamBuf->_readInt64();
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			CRCore::crVector2i nettypepair(netType,0);
			room->doEvent(msg,MAKEINT64(m_streamBuf.get(),&nettypepair));
		}
	}
}
/////////////////////////////////////////
//
//crKickoffPlayerPacket
//
/////////////////////////////////////////
crKickoffPlayerPacket::crKickoffPlayerPacket():
	crStreamPacket(crKickoffPlayerPacket::ID)
{
}

crKickoffPlayerPacket::crKickoffPlayerPacket( const crKickoffPlayerPacket &p ):
	crStreamPacket(crKickoffPlayerPacket::ID,p)
{
}

void crKickoffPlayerPacket::buildReplyPacket(crKickoffPlayerPacket &packet,int playerid)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}

void crKickoffPlayerPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		crNetDataManager *netDataManager = netConductor->getNetDataManager();
		crGameConnectServerPlayerData *playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(playerid));		
		if(playerData)
		{
			netConductor->getNetManager()->removeConnection(playerData->getPlayerAdress());
			netDataManager->removePlayerData(playerid);
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = netConductor->getNetDataManager();
		netDataManager->removePlayerData(playerid);
	}
}
/////////////////////////////////////////
//
//crSetGamelogPathPacket
//
/////////////////////////////////////////
crSetGamelogPathPacket::crSetGamelogPathPacket():
	crStreamPacket(crSetGamelogPathPacket::ID)
{
}

crSetGamelogPathPacket::crSetGamelogPathPacket( const crSetGamelogPathPacket &p ):
	crStreamPacket(crSetGamelogPathPacket::ID,p)
{
}
void crSetGamelogPathPacket::buildRequestPacket(crSetGamelogPathPacket &packet,const std::string &logpath)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+logpath.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(logpath);
}

void crSetGamelogPathPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{
		std::string logpath = m_streamBuf->_readString();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crDBServerCallback *callback = dynamic_cast<crDBServerCallback *>(netDataManager->getNetCallback());
		callback->insertGamelogPath(sender,logpath);
	}
}
/////////////////////////////////////////
//
//crThrowServerPacket
//
/////////////////////////////////////////
crThrowServerPacket::crThrowServerPacket():
	crStreamPacket(crThrowServerPacket::ID)
{
}

crThrowServerPacket::crThrowServerPacket( const crThrowServerPacket &p ):
	crStreamPacket(crThrowServerPacket::ID,p)
{
}

void crThrowServerPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{
		_asm   int   3   //只是为了让程序崩溃
	}
	else if(netType == SceneServerClient_Game)
	{
		_asm   int   3   //只是为了让程序崩溃
	}
}
/////////////////////////////////////////
//
//crServerNeedRestartPacket
//
/////////////////////////////////////////
crServerNeedRestartPacket::crServerNeedRestartPacket():
	crStreamPacket(crServerNeedRestartPacket::ID)
{
}

crServerNeedRestartPacket::crServerNeedRestartPacket( const crServerNeedRestartPacket &p ):
	crStreamPacket(crServerNeedRestartPacket::ID,p)
{
}

void crServerNeedRestartPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
			connectServer->getNetManager()->sendPacket("all",*this);
	}
	else if(netType == GameClient_Game)
	{
		crBrain::getInstance()->doEvent(WCH_ServerNeedRestart);
	}
}
/////////////////////////////////////////
//
//crClientReconnectPacket
//
/////////////////////////////////////////
crClientReconnectPacket::crClientReconnectPacket():
	crStreamPacket(crClientReconnectPacket::ID)
{
}

crClientReconnectPacket::crClientReconnectPacket( const crClientReconnectPacket &p ):
	crStreamPacket(crClientReconnectPacket::ID,p)
{
}
void crClientReconnectPacket::buildRequestPacket(crClientReconnectPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crClientReconnectPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crGameConnectServerCallback> callback = dynamic_cast<crGameConnectServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
		ref_ptr<crGameConnectServerPlayerData> playerdata;
		callback->remuseDropedPlayer(playerid,playerdata);
		if(playerdata.valid())
		{
			playerdata->setPlayerAdress(sender);
			m_netConductor->getNetDataManager()->insertPlayerData(playerdata.get());
			//CRCore::notify(CRCore::ALWAYS)<<"crClientReconnectPacket: "<<playerid<<" ip:"<<sender<<std::endl;
			std::string logdata = "断线重连："+sender;
			m_netConductor->getNetManager()->getNetBridge()->netErrorLog(playerid,logdata);
		}
		else
		{
			crClientReconnectPacket packet;
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crGlobalHandle::getInstance()->doEvent(WCH_GameConnectServerAbnormal);
	}
}
/////////////////////////////////////////
//
//crQueryGameInfoPacket
//
/////////////////////////////////////////
crQueryGameInfoPacket::crQueryGameInfoPacket():
	crStreamPacket(crQueryGameInfoPacket::ID)
{
}

crQueryGameInfoPacket::crQueryGameInfoPacket( const crQueryGameInfoPacket &p ):
	crStreamPacket(crQueryGameInfoPacket::ID,p)
{
}

void crQueryGameInfoPacket::buildRequestPacket(crQueryGameInfoPacket &packet,int gameid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(gameid);
}
void crQueryGameInfoPacket::buildRequestPacket2(crQueryGameInfoPacket &packet,const std::string &playerAddress,crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crQueryGameInfoPacket::buildReplyPacket(crQueryGameInfoPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+5+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	if(stream)
	{
		streamBuf->_writeBool(true);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->_writeBool(false);
	}
}
void crQueryGameInfoPacket::buildReplyPacket2(crQueryGameInfoPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crQueryGameInfoPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到GameServer
		crQueryGameInfoPacket packet;
		crQueryGameInfoPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameInfoPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//
		std::string playerAddress = m_streamBuf->_readString();
		int gameid = m_streamBuf->_readInt();
		crQueryGameInfoPacket packet;
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crGameInfoQueryData> gameinfoQuery = new crGameInfoQueryData;
		gameinfoQuery->buildQuerySql(gameid);
		globalSession->executeQuery(gameinfoQuery.get());
		crDataBase::QueryResultVec& gameinfoQueryResultVec = globalSession->getQueryResult();
		if(!gameinfoQueryResultVec.empty())
		{
			gameinfoQuery = dynamic_cast<crGameInfoQueryData *>(gameinfoQueryResultVec[0].get());
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(48);
			stream->_writeString(gameinfoQuery->getCName());//24
			stream->_writeString(gameinfoQuery->getPlatform());//24
			crQueryGameInfoPacket::buildReplyPacket(packet,playerAddress,stream.get());
		}
		else
		{
			crQueryGameInfoPacket::buildReplyPacket(packet,playerAddress,NULL);
		}
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crQueryGameInfoPacket packet;
			crQueryGameInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameInfoPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		bool success = m_streamBuf->_readBool();
		crQueryGameListData *selectedGame = crMyPlayerData::getInstance()->getSelectedGame();
		if(success)
		{
			std::string cname = m_streamBuf->_readString();
			std::string platform = m_streamBuf->_readString();
			selectedGame->setCName(cname);
			selectedGame->setPlatform(platform);
		}
		char gbuf[256];
		sprintf(gbuf,"%s,%s\n\0",selectedGame->getCName().c_str(),selectedGame->getPlatform().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
}
/////////////////////////////////////////
//
//crShutDownServerPacket
//
/////////////////////////////////////////
crShutDownServerPacket::crShutDownServerPacket():
	crStreamPacket(crShutDownServerPacket::ID)
{
}

crShutDownServerPacket::crShutDownServerPacket( const crShutDownServerPacket &p ):
	crStreamPacket(crShutDownServerPacket::ID,p)
{
}

void crShutDownServerPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{
		crBlockDetectThread::getInstance()->update();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crDBServerCallback *netCallback = dynamic_cast<crDBServerCallback *>(netDataManager->getNetCallback());
		netCallback->shutDown();
	}
	else if(netType == SceneServerClient_Game)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(sceneServerConductor)
		{
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			netCallback->shutDown();
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		crNetContainer::getInstance()->shutdown();
	}
}