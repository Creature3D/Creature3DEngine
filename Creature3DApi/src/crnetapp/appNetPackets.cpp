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
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/crNotify.h>
#include <CRNetApp/appDBQuery.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRIOManager/crFileUtils.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appClientPlayerData.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;
using namespace CRDataBase;
/////////////////////////////////////////
//
//crAppServerRefusePacket
//
/////////////////////////////////////////
crAppServerRefusePacket::crAppServerRefusePacket():
crStreamPacket(crAppServerRefusePacket::ID)
{
}
crAppServerRefusePacket::crAppServerRefusePacket( const crAppServerRefusePacket &p ):
crStreamPacket(crAppServerRefusePacket::ID,p)
{
}
void crAppServerRefusePacket::buildRequestPacket(crAppServerRefusePacket &packet, int recvid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	//streamBuf->_writeString(playerAddress);
}

void crAppServerRefusePacket::buildRequestPacket2(crAppServerRefusePacket &packet, int code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(code);
}

void crAppServerRefusePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServerClient_Chat)
	{//转发到Client
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServer)
		{
			crNetDataManager *netDataManager = connectServer->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(playerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					crNetConductor *cameraServer = playerData->getCameraServerLineConductor();
					if(cameraServer)
					{
						m_streamBuf->seekBegin();
						cameraServer->getNetManager()->sendPacket("all",*this);
					}
					crNetConductor *voiceServer = playerData->getVoiceServerLineConductor();
					if(voiceServer)
					{
						m_streamBuf->seekBegin();
						voiceServer->getNetManager()->sendPacket("all",*this);
					}

					crAppServerRefusePacket packet;
					crAppServerRefusePacket::buildRequestPacket2(packet,-1);
					connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);

					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == ParentServer)
	{
		int recvid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		netDataManager->lockPlayerDataMap();
		ref_ptr<crParentServerPlayerData> playerData = dynamic_cast<crParentServerPlayerData *>(netDataManager->getPlayerData(recvid));
		if(playerData.valid())
		{
			//std::string playerAddress = m_streamBuf->_readString();
			//if(playerData->getPlayerAdress().compare(playerAddress) == 0)
			//{
				m_streamBuf->seekBegin();
				crNetConductor *parentConductor = playerData->getParentServerLineConductor();
				if(parentConductor)
				{
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);

				//crParentServerCallback *callback = dynamic_cast<crParentServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
				//if(callback)
				//{
				//	callback->playerDroped(playerData.get());
				//}
				netDataManager->removePlayerData(recvid);
				//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
			//}
		}
		netDataManager->unLockPlayerDataMap();
	}
	else if(netType == ParentServerClient_Parent)
	{//不需要removePlayerData，利用connectserver的logoutpacket退出
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			crNetDataManager *netDataManager = parentServerConductor->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crParentServerPlayerData>recvPlayerData = dynamic_cast<crParentServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(recvPlayerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					m_streamBuf->seekBegin();
					parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
					//crParentServerCallback *callback = dynamic_cast<crParentServerCallback *>(parentServerConductor->getNetDataManager()->getNetCallback());
					//if(callback)
					//{
					//	callback->playerDroped(recvPlayerData.get());
					//}
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == ChatServerClient_Parent)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			crNetDataManager *netDataManager = chatServerConductor->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(recvPlayerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					m_streamBuf->seekBegin();
					chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
					//crChatServerCallback *callback = dynamic_cast<crChatServerCallback *>(chatServerConductor->getNetDataManager()->getNetCallback());
					//if(callback)
					//{
					//	callback->playerDroped(recvPlayerData.get());
					//}
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == GameServerClient_Parent)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(gameServerConductor)
		{
			crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crGameServerPlayerData> recvPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(recvPlayerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					m_streamBuf->seekBegin();
					const std::string &sceneServerAddress = recvPlayerData->getSceneServerAddress();
					if(!sceneServerAddress.empty()) 
						gameServerConductor->getNetManager()->sendPacket(sceneServerAddress,*this);

					gameServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
					
					//crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServerConductor->getNetDataManager()->getNetCallback());
					//if(callback)
					//{
					//	callback->playerDroped(recvPlayerData.get());
					//}
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
		{
			crNetDataManager *netDataManager = connectServer->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(playerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					//crNetConductor *downloadServer = playerData->getDownloadServerLineConductor();
					//if(downloadServer)
					//{
					//	m_streamBuf->seekBegin();
					//	downloadServer->getNetManager()->sendPacket("all",*this);
					//}
					crAppServerRefusePacket packet;
					crAppServerRefusePacket::buildRequestPacket2(packet,-1);
					connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
				//没有必要，该连接会由连接超时负责清理
				//connectServer->getNetManager()->refuseConnection(playerData->getPlayerAdress());
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == DownloadServer || netType == CameraServer || netType == VoiceServer)
	{
		int recvid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		netDataManager->lockPlayerDataMap();
		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
		if(playerData.valid())
		{
			//std::string playerAddress = m_streamBuf->_readString();
			//if(playerData->getPlayerAdress().compare(playerAddress) == 0)
			//{
				//playerDroped
				netDataManager->removePlayerData(recvid);
				//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
			//}
		}
		netDataManager->unLockPlayerDataMap();
	}
	else if(netType == SceneServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(sceneServerConductor)
		{
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crSceneServerPlayerData> recvPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(recvPlayerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					//m_streamBuf->seekBegin();
					//sceneServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
					
					//crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServerConductor->getNetDataManager()->getNetCallback());
					//if(callback)
					//{
					//	callback->playerDroped(recvPlayerData.get());
					//}
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	//else if(netType == DownloadConnectServerClient_Download)
	//{
	//	int recvid = m_streamBuf->_readInt();
	//	crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(DownloadConnectServer);
	//	if(connectServer)
	//	{
	//		crNetDataManager *netDataManager = connectServer->getNetDataManager();
	//		netDataManager->lockPlayerDataMap();
	//		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
	//		if(playerData.valid())
	//		{
	//			std::string playerAddress = m_streamBuf->_readString();
	//			if(playerData->getPlayerAdress().compare(playerAddress) == 0)
	//			{
	//				crAppServerRefusePacket packet;
	//				crAppServerRefusePacket::buildRequestPacket2(packet,-1);
	//				connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//				netDataManager->removePlayerData(recvid);
	//				CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
	//			}
	//		}
	//		netDataManager->unLockPlayerDataMap();
	//	}
	//}
	else if(netType == LoginConnectServerClient_Login)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			crNetDataManager *netDataManager = connectServer->getNetDataManager();
			netDataManager->lockPlayerDataMap();
			ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				//std::string playerAddress = m_streamBuf->_readString();
				//if(playerData->getPlayerAdress().compare(playerAddress) == 0)
				//{
					crAppServerRefusePacket packet;
					crAppServerRefusePacket::buildRequestPacket2(packet,-1);
					connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
					netDataManager->removePlayerData(recvid);
					//CRCore::notify(CRCore::ALWAYS)<<"crAppServerRefusePacket "<<recvid<<std::endl;
				//}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
	else if(netType == GameClient_Chat)
	{
		int code = m_streamBuf->_readInt();
		m_netConductor->getNetManager()->setServerRefuseCode(code);
		//crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse,MAKEINT64(code,GameClient_Chat));
		crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse);
	}
	else if(netType == GameClient_Game)
	{
		int code = m_streamBuf->_readInt();
		m_netConductor->getNetManager()->setServerRefuseCode(code);
		//crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse,MAKEINT64(code,GameClient_Game));
		crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse);
	}
	//else if(netType == GameClient_Download)
	//{
	//	int code = m_streamBuf->_readInt();
	//	m_netConductor->getNetManager()->setServerRefuseCode(code);
	//	crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse,MAKEINT64(code,GameClient_Download));
	//}
	else if(netType == GameClient_Login)
	{
		int code = m_streamBuf->_readInt();
		m_netConductor->getNetManager()->setServerRefuseCode(code);
		//crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse,MAKEINT64(code,GameClient_Login));
		crGlobalHandle::getInstance()->doEvent(WCH_ServerRefuse);
	}
}

/////////////////////////////////////////
//
//crAppLogoutPacket
//
/////////////////////////////////////////
crAppLogoutPacket::crAppLogoutPacket():
crStreamPacket(crAppLogoutPacket::ID)
{
}

crAppLogoutPacket::crAppLogoutPacket( const crAppLogoutPacket &p ):
crStreamPacket(crAppLogoutPacket::ID,p)
{
}

void crAppLogoutPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == ChatServer)
	{//
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> playerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerConnectServerAddress() == sender)
		{
			if(playerData->getParentServerLineConductor())
			{
				m_streamBuf->seekBegin();
                playerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",*this);
			}

			//crChatServerCallback *callback = dynamic_cast<crChatServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
			//if(callback)
			//{
			//	callback->playerDroped(playerData.get());
			//}

			m_netConductor->getNetDataManager()->removePlayerData(playerid);
			//CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
		}
	}
	else if(netType == ParentServer)
	{//
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> playerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			if(playerData->getParentServerLineConductor())
			{
				m_streamBuf->seekBegin();
				playerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",*this);
			}
			m_netConductor->getNetDataManager()->removePlayerData(playerid);
			//CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
		}
	}
	else if(netType == GameServer)
	{//
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerConnectServerAddress() == sender)
		{
			const std::string &sceneServerAddress = playerData->getSceneServerAddress();
			if(sceneServerAddress.compare(sender) == 0)
			{
				playerData->setSceneServerAddress("");
				playerData->setSceneID(0);
			}
			else
			{//做个延迟，等待sceneserver返回
				//while (playerData->getSceneID()>0)
				//{
				//	CRCore::crThread::sleep(10);
				//}
				if(playerData->getParentServerLineConductor())
				{
					m_streamBuf->seekBegin();
					playerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",*this);
				}
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
				if(!sceneServerAddress.empty())
				{
					callback->insertDelayRemovePlayer(playerData.get());
				}
				else
				{
					//callback->playerDroped(playerData.get());
					m_netConductor->getNetDataManager()->removePlayerData(playerid);
				}
				//CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{//
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerConnectServerAddress() == sender)
		{
			//crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
			//if(callback)
			//{
			//	callback->playerDroped(playerData.get());
			//}
			//if(playerData->getGameServerLineConductor())
			//{
			//	m_streamBuf->seekBegin();
			//	playerData->getGameServerLineConductor()->getNetManager()->sendPacket("all",*this);
			//}
			m_netConductor->getNetDataManager()->removePlayerData(playerid);
			//CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
		}
	}
	else if(netType == DownloadServer)
	{//
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerConnectServerAddress() == sender)
		{
			m_netConductor->getNetDataManager()->removePlayerData(playerid);
			//CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
		}
	}
	//else if(netType == LoginServer)
	//{//
	//	int playerid = m_streamBuf->_readInt();
	//	crServerPlayerData *playerData = dynamic_cast<crServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
	//	if(playerData)
	//	{
	//		m_netConductor->getNetDataManager()->removePlayerData(playerid);
	//		CRCore::notify(CRCore::ALWAYS)<<"crAppLogoutPacket RemovePlayerData:"<<playerid<<std::endl;
	//	}
	//}
}
/////////////////////////////////////////
//
//crLogoutScenePacket
//
/////////////////////////////////////////
crLogoutScenePacket::crLogoutScenePacket():
crStreamPacket(crLogoutScenePacket::ID)
{
}
crLogoutScenePacket::crLogoutScenePacket( const crLogoutScenePacket &p ):
crStreamPacket(crLogoutScenePacket::ID,p)
{
}
void crLogoutScenePacket::buildRequestPacket(crLogoutScenePacket &packet, int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crLogoutScenePacket::parsePacket(const std::string &sender)
{
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
				crLogoutScenePacket packet;
				crLogoutScenePacket::buildRequestPacket(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crLogoutScenePacket "<<std::endl;
				playerData->setSceneServerLineConductor(NULL);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			//crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(m_netConductor->getNetDataManager()->getNetCallback());
			//if(callback)
			//{
			//	callback->playerDroped(playerData.get());
			//}
			crNetConductor *netConductor = playerData->getGameServerLineConductor();
			if(netConductor)
			{
				m_streamBuf->seekBegin();
				netConductor->getNetManager()->sendPacket("all",*this);
			}
			m_netConductor->getNetDataManager()->removePlayerData(playerid);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"SceneServer crLogoutScenePacket "<<playerid<<std::endl;
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(sceneServerConductor)
		{
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			netDataManager->removePlayerData(playerid);
			//CRCore::notify(CRCore::ALWAYS)<<"SceneServerClient_Game crLogoutScenePacket "<<playerid<<std::endl;
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			netPlayerData->setSceneServerAddress("");
			netPlayerData->setSceneID(0);
		}
	}
}

/////////////////////////////////////////
//
//crLoginPacket
//
/////////////////////////////////////////
//int crLoginPacket::s_success = 0;
crLoginPacket::crLoginPacket():
crStreamPacket(crLoginPacket::ID)
{
}

crLoginPacket::crLoginPacket( const crLoginPacket &p ):
crStreamPacket(crLoginPacket::ID,p)
{
}

void crLoginPacket::buildRequestPacket(crLoginPacket &packet,std::string &userName,std::string &userPassword)
{//Client->ConnectServer ConnectServerClient_Login->LoginServer
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(256);
	packet.setStreamBuf(streamBuf);
	//crLoginPacket::s_success = 0;
	//if(userName.size()>16)
	//	userName = userName.substr(0,16);
	streamBuf->_writeString(userName);//name---最大20+4
	//if(userPassword.size()>16)
	//	userPassword = userPassword.substr(0,16);
	streamBuf->_writeString(userPassword);//name---最大20+4
}

void crLoginPacket::buildRequestPacket2(crLoginPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
{//Client->ConnectServer ConnectServerClient_Login->LoginServer
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

//void crLoginPacket::buildReplyPacket(crLoginPacket &packet,const std::string &address,char loginCode,int playerid,/*const std::string &playerName,*/const std::string &key,crAccountData *accountData)
//{//LoginServer->ConnectServerClient_Login
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(512);
//	packet.setStreamBuf(streamBuf);
//
//	streamBuf->_writeString(address);
//	streamBuf->_writeChar(loginCode);//id---4
//	if(loginCode>0)
//	{
//		streamBuf->_writeInt(playerid);//name---最大20+4
//		//streamBuf->_writeString(playerName);//name---最大20+4
//		streamBuf->_writeString(key);
//		/////////
//		streamBuf->_writeString(accountData->getNickName());
//		streamBuf->_writeString(accountData->getRelName());
//		streamBuf->_writeUShort(accountData->getChatDBID());
//		streamBuf->_writeChar(accountData->getSex());
//		streamBuf->_writeString(accountData->getAddress());
//		streamBuf->_writeString(accountData->getBirthday());
//		streamBuf->_writeString(accountData->getIdiograph());
//		streamBuf->_writeChar(accountData->getBloodtype());
//		streamBuf->_writeChar(accountData->getWork());
//        streamBuf->_writeString(accountData->getSchool());
//        streamBuf->_writeString(accountData->getEmail());
//        streamBuf->_writeString(accountData->getBlog());
//		streamBuf->_writeString(accountData->getTel());
//		streamBuf->_writeString(accountData->getMobile());
//		streamBuf->_writeString(accountData->getNote());
//		streamBuf->_writeChar(accountData->getBeFriendValid());
//		streamBuf->_writeString(accountData->getPrelogindate());
//		streamBuf->_writeInt(accountData->getLastServerID());
//	}
//}
void crLoginPacket::buildReplyPacket(crLoginPacket &packet,const std::string &address,char loginCode,int playerid,int lastserverid,const std::string &key)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+key.length()+17);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->_writeChar(loginCode);
	if(loginCode>0)
	{
		streamBuf->_writeInt(playerid);
		streamBuf->_writeInt(lastserverid);
		streamBuf->_writeString(key);
	}
}
void crLoginPacket::buildReplyPacket2(crLoginPacket &packet,CRCore::crStreamBuf *stream)
{//LoginServer->ConnectServerClient_Login
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
    streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crLoginPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到LoginServer
		crLoginPacket packet;
		crLoginPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
        if(netConductor)
		{
		    netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//LoginServer发回到LoginConnectServer
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		int playerid = 0;
		int lastserverid = 0;
		unsigned char locked = 0;
		std::string userName,userPassword,playerAddress,key;
		char loginCode = 0;//0登录失败，用户名密码不正确，1登录成功,2帐号被锁定
		playerAddress = m_streamBuf->_readString();
		userName = m_streamBuf->_readString();
		//if(userName.size()>16)
		//	userName = userName.substr(0,16);
		userPassword = m_streamBuf->_readString();
		//if(userPassword.size()>16)
		//	userPassword = userPassword.substr(0,16);

		//数据库检索登陆账户，如果密码正确，则返回账户信息
		//ref_ptr<crAccountData> accountData;
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();

		ref_ptr<crLoginQueryData> loginQueryData = new crLoginQueryData;
		loginQueryData->buildQuerySql(userName,userPassword);

		globalSession->executeQuery(loginQueryData.get());
		crDataBase::QueryResultVec &loginQueryResultVec = globalSession->getQueryResult();
		if(!loginQueryResultVec.empty())
		{
			loginQueryData = dynamic_cast<crLoginQueryData *>(loginQueryResultVec[0].get());
			playerid = loginQueryData->getPlayerID();
			lastserverid = loginQueryData->getLastServerID();
			locked = loginQueryData->getLocked();
			if(locked==0)
				loginCode = 1;
			else
				loginCode = 2;
			//const std::string &password = loginQueryData->getPassword();
			//////password解密
			////ref_ptr<crStreamBuf> passwordstream = new crStreamBuf;
			////passwordstream->createBuf(password.length());
			////passwordstream->write(password.c_str(),password.length());
			////CRNet::creUnCook(passwordstream);
			//if(userPassword.compare(password) == 0)
			//{
			//	loginCode = 1;
			//}
			//else
			//{
			//	CRCore::notify(CRCore::ALWAYS)<<"userPassword "<<userPassword<<" password "<<password<<std::endl;
			//}
		}
		globalSession->releaseQuery();

		if(loginCode==1)
		{//登陆成功
			//动态连接，服务器不需要保存客户数据
			//netDataManager->insertPlayerData(playerData.get());
			//crServerPlayerData *connectedPlayerData  = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(playerid));
			//if(connectedPlayerData)
			//{
			//	crAppServerRefusePacket refusePacket;
			//	crAppServerRefusePacket::buildRequestPacket(refusePacket,connectedPlayerData->getPlayerID(),connectedPlayerData->getPlayerAdress());
			//	netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),refusePacket);
			//	//CRCore::crThread::yieldCurrentThread();
			//}

			//ref_ptr<crServerPlayerData> playerData = new crServerPlayerData;
			//playerData->setPlayerID(playerid);
			////playerData->setPlayerName(userName);
			//playerData->setPlayerAdress(playerAddress);
			//playerData->setPlayerConnectServerAddress(sender);

			//accountData = new crAccountData;
			//accountData->setNickName(loginQueryData->getNickName());
			//accountData->setRelName(loginQueryData->getRelName());
			//accountData->setChatDBID(loginQueryData->getChatDBID());
			//accountData->setSex(loginQueryData->getSex());
			//accountData->setAddress(loginQueryData->getAddress());
			//accountData->setBirthday(loginQueryData->getBirthday());
			//accountData->setIdiograph(loginQueryData->getIdiograph());
			//accountData->setBloodtype(loginQueryData->getBloodtype());
			//accountData->setWork(loginQueryData->getWork());
			//accountData->setSchool(loginQueryData->getSchool());
			//accountData->setEmail(loginQueryData->getEmail());
   //         accountData->setBlog(loginQueryData->getBlog());
   //         accountData->setTel(loginQueryData->getTel());
			//accountData->setMobile(loginQueryData->getMobile());
			//accountData->setNote(loginQueryData->getNote());
			//accountData->setBeFriendValid(loginQueryData->getBeFriendValid());
			//accountData->setPrelogindate(loginQueryData->getLogindate());
			//accountData->setLastServerID(loginQueryData->getLastServerID());
			//std::string basecode = CRNet::getBaseCode(userName,playerAddress);
			key = CRNet::gainCode();//CRNet::gainKey(/*basecode*/);
			//std::string::size_type i = playerAddress.find_last_of(':');
			//std::string ip = std::string(playerAddress.begin(),playerAddress.begin()+i+1);

			ref_ptr<crUpdateLoginKeyDate> updateLoginDate = new crUpdateLoginKeyDate;
			updateLoginDate->buildUpdateSql(loginQueryData->getPlayerID(),/*ip+*/key);
			if(globalSession->executeUpdate(updateLoginDate.get()))
			{//插入成功
				globalSession->commit();
			}

   //         //查询并返回GameList
			//ref_ptr<crGameListLimitQueryData> gameListLimitQuery = new crGameListLimitQueryData;
			//gameListLimitQuery->buildQuerySql(0,50);
			//globalSession->executeQuery(gameListLimitQuery.get());
			//crDataBase::QueryResultVec gameListLimitQueryResultVec = globalSession->getQueryResult();
			//globalSession->releaseQuery();
			//db->endSession(globalSession.get());

			//if(!gameListLimitQueryResultVec.empty())
			//{
			//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
			//	int maxsize = 2+83*gameListLimitQueryResultVec.size();
			//	stream->createBuf(maxsize);
			//	stream->_writeShort(gameListLimitQueryResultVec.size());
			//	for( crDataBase::QueryResultVec::iterator itr = gameListLimitQueryResultVec.begin();
			//		itr != gameListLimitQueryResultVec.end();
			//		++itr )
			//	{
			//		gameListLimitQuery = dynamic_cast<crGameListLimitQueryData *>(itr->get());
			//		//CRCore::notify(CRCore::ALWAYS)<<"crLoginPacket CName="<<gameListLimitQuery->getCName()<<std::endl;
			//		stream->_writeInt(gameListLimitQuery->getID());
			//		stream->_writeString(gameListLimitQuery->getGameName());//20+4
			//		stream->_writeString(gameListLimitQuery->getCName());//20+4
			//		stream->_writeString(gameListLimitQuery->getUIFile());//50+4
			//		stream->_writeUChar(gameListLimitQuery->getRoleCount());//1
			//	}

			//	crRecvGameListPacket packet;
			//	crRecvGameListPacket::buildReplyPacket(packet,playerAddress,stream.get());
			//	netManager->sendPacket(sender,packet);
		}
		db->endSession(globalSession.get());
		crLoginPacket packet;
		crLoginPacket::buildReplyPacket(packet,playerAddress,loginCode,playerid,lastserverid,/*userName,*/key);
		netManager->sendPacket(sender,packet);
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginPacket "<<userName<<" "<<(int)loginCode<<std::endl;
	}
	else if(netType == LoginConnectServerClient_Login)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			//char *ptr = m_streamBuf->getPtr();
			//char loginCode = m_streamBuf->_readChar();
			//if(loginCode>0)
			//{
			//	int playerid = m_streamBuf->_readInt();
			//	crNetDataManager *netDataManager = connectServer->getNetDataManager();
			//	netDataManager->lockPlayerDataMap();
			//	ref_ptr<crGameConnectServerPlayerData> oldPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(playerid));
			//	if(oldPlayerData.valid())
			//	{
			//		crAppServerRefusePacket packet;
			//		crAppServerRefusePacket::buildRequestPacket2(packet,-1);
			//		connectServer->getNetManager()->sendPacket(oldPlayerData->getPlayerAdress(),packet);
			//	}
			//	crLoginConnectServerPlayerData *playerData = new crLoginConnectServerPlayerData;
			//	playerData->setPlayerID(playerid);
			//	//playerData->setPlayerName(m_streamBuf->_readString());
			//	playerData->setPlayerAdress(address);
			//	playerData->setLoginServerLineConductor(m_netConductor);

			//	netDataManager->insertPlayerData(playerData);
			//	netDataManager->unLockPlayerDataMap();
			//}
   //         //m_streamBuf->seekBegin();
			//char *ptr2 = m_streamBuf->getPtr();
			//m_streamBuf->seek(ptr - ptr2);
			crLoginPacket packet;
			crLoginPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crLoginPacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{
		char loginCode = m_streamBuf->_readChar();
		if(loginCode==1)
		{//登陆成功，返回账户信息。
			crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
			myPlayerData->setPlayerID(m_streamBuf->_readInt());
			myPlayerData->setLastServerID(m_streamBuf->_readInt());
			myPlayerData->setKey(m_streamBuf->_readString());

			//crAccountData *accountData = new crAccountData;
			//accountData->setNickName(m_streamBuf->_readString());
			//accountData->setRelName(m_streamBuf->_readString());
			//accountData->setChatDBID(m_streamBuf->_readUShort());
			//accountData->setSex(m_streamBuf->_readChar());
			//accountData->setAddress(m_streamBuf->_readString());
			//accountData->setBirthday(m_streamBuf->_readString());
			//accountData->setIdiograph(m_streamBuf->_readString());
			//accountData->setBloodtype(m_streamBuf->_readChar());
			//accountData->setWork(m_streamBuf->_readChar());
			//accountData->setSchool(m_streamBuf->_readString());
			//accountData->setEmail(m_streamBuf->_readString());
			//accountData->setBlog(m_streamBuf->_readString());
			//accountData->setTel(m_streamBuf->_readString());
			//accountData->setMobile(m_streamBuf->_readString());
			//accountData->setNote(m_streamBuf->_readString());
			//accountData->setBeFriendValid(m_streamBuf->_readChar());
			//accountData->setPrelogindate(m_streamBuf->_readString());
			//accountData->setLastServerID(m_streamBuf->_readInt());
			//myPlayerData->setAccountData(accountData);
		}
		//s_success = loginCode==0?-1:loginCode;
		crGlobalHandle::getInstance()->doEvent(WCH_LoginReturn,MAKEINT64(loginCode,NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginPacket "<<loginCode<<std::endl;
	}
}

/////////////////////////////////////////
//
//crLoginChatPacket
//
/////////////////////////////////////////
//int crLoginChatPacket::s_success = 0;
crLoginChatPacket::crLoginChatPacket():
crStreamPacket(crLoginChatPacket::ID)
{
}

crLoginChatPacket::crLoginChatPacket( const crLoginChatPacket &p ):
crStreamPacket(crLoginChatPacket::ID,p)
{
}

void crLoginChatPacket::buildRequestPacket(crLoginChatPacket &packet)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);
	//crLoginChatPacket::s_success = 0;
    crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
	streamBuf->_writeInt(myPlayerData->getPlayerID());
	//streamBuf->_writeString(myPlayerData->getPlayerName());
	streamBuf->_writeString(myPlayerData->getKey());
}

void crLoginChatPacket::buildRequestPacket2(crLoginChatPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(sender.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(sender);
    streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crLoginChatPacket::buildReplyPacket(crLoginChatPacket &packet,const std::string &address,char loginCode,crChatServerPlayerData *playerData)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->_writeChar(loginCode);
	if(loginCode>0)
	{
	    streamBuf->_writeInt(playerData->getPlayerID());//4
	   // streamBuf->_writeString(playerData->getPlayerName());//24
	}
}

void crLoginChatPacket::buildReplyPacket2(crLoginChatPacket &packet,char loginCode)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);
}
//注意：Leaveword不能超过MAXBUFFERSIZE-12-52
void crLoginChatPacket::chatPlayerDataQuery(crChatServerPlayerData *playerData,CRDataBase::crDataBase *globalSession, CRDataBase::crDataBase *chatSession)
{
	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
	crNetManager *netManager = m_netConductor->getNetManager();
	crNetConductor *parentServer = playerData->getParentServerLineConductor();
    crChatStateChangePacket chatStateChangePacket;
	ref_ptr<crChatServerPlayerData> loginedChatPlayer;

	std::set<int> InformedPlayerSet;
	bool informed = false;

	std::set<crChatServerPlayerData *> NeedInformPlayerSet;//收集处于登陆状态的好友
	{//////////////查询好友列表
		ref_ptr<crPlayerFriendTableQueryData> playerFriendTabQuery = new crPlayerFriendTableQueryData;
		playerFriendTabQuery->buildQuerySql(playerData->getPlayerID());

		chatSession->executeQuery(playerFriendTabQuery.get());
		crDataBase::QueryResultVec playerFriendTabQueryResultVec = chatSession->getQueryResult();
		chatSession->releaseQuery();
		crNetChatData::crChatFriend *chatFriend,*chatFriend2;

		if(!playerFriendTabQueryResultVec.empty())
		{
			std::vector< ref_ptr<crStreamBuf> > FriendBufVec;
			int maxNeedSize = 81;
			int count = 0;
			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
			char chatstate;
			for( crDataBase::QueryResultVec::iterator itr = playerFriendTabQueryResultVec.begin();
				itr != playerFriendTabQueryResultVec.end();
				++itr )
			{
				playerFriendTabQuery = dynamic_cast<crPlayerFriendTableQueryData *>(itr->get());
				
				chatFriend = new crNetChatData::crChatFriend;
				chatFriend->setID(playerFriendTabQuery->getID());
				chatFriend->setPlayerID(playerFriendTabQuery->getFriendID());
				chatFriend->setColumnName(playerFriendTabQuery->getColumnName());
				chatFriend->setRemark(playerFriendTabQuery->getRemark());

				if(chatFriend->getPlayerID() != playerData->getPlayerID())
				{
					InformedPlayerSet.insert(chatFriend->getPlayerID());
					loginedChatPlayer = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(chatFriend->getPlayerID()));
					if(loginedChatPlayer.valid())
					{
						chatstate = loginedChatPlayer->getNetChatData()->getMyChatState();
						chatFriend->setFriendChatState(loginedChatPlayer->getNetChatData()->getMyChatState());
						chatFriend->setNickName(loginedChatPlayer->getNickName());

						crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),loginedChatPlayer->getPlayerID(),crNetChatData::OnLine,playerData->getNickName());
						m_netConductor->getNetManager()->sendPacket(loginedChatPlayer->getPlayerConnectServerAddress(),chatStateChangePacket);

						chatFriend2 = loginedChatPlayer->getNetChatData()->getFriend(playerData->getPlayerID());
						if(chatFriend2)
						{
							chatFriend2->setFriendChatState((char)(crNetChatData::OnLine));
							chatFriend2->setNickName(playerData->getNickName());
						}
					}
					else
					{//从数据库检查是否在线
						ref_ptr<crChatStateNickNameQueryData> chatStateNickNameQuery = new crChatStateNickNameQueryData;
						chatStateNickNameQuery->buildQuerySql(chatFriend->getPlayerID());

						globalSession->executeQuery(chatStateNickNameQuery.get());
						crDataBase::QueryResultVec &chatStateNickNameQueryResultVec = globalSession->getQueryResult();
						if(!chatStateNickNameQueryResultVec.empty())
						{
							chatStateNickNameQuery = dynamic_cast<crChatStateNickNameQueryData *>(chatStateNickNameQueryResultVec[0].get());
							chatstate = chatStateNickNameQuery->getChatState();
							const std::string& nickName = chatStateNickNameQuery->getNickName();
							chatFriend->setNickName(nickName);
						}
						globalSession->releaseQuery();

						chatFriend->setFriendChatState(chatstate);
						if(chatstate>0 && parentServer)
						{//如在线
							crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),chatFriend->getPlayerID(),crNetChatData::OnLine,playerData->getNickName());
							parentServer->getNetManager()->sendPacket("all",chatStateChangePacket);
						}
					}
				}
				else
				{
					chatFriend->setFriendChatState(playerData->getNetChatData()->getMyChatState());
					chatFriend->setNickName(playerData->getNickName());
				}
				playerData->getNetChatData()->insertFriend(chatFriend);
				
				if(streamBuf->getRemainCapacity()>maxNeedSize)
				{
					streamBuf->_writeInt(chatFriend->getID());//4
					streamBuf->_writeInt(chatFriend->getPlayerID());//4
					streamBuf->_writeString(chatFriend->getColumnName());//20+4
					streamBuf->_writeString(chatFriend->getRemark());//20+4
					streamBuf->_writeChar(chatFriend->getFriendChatState());//1
					streamBuf->_writeString(chatFriend->getNickName());//20+4
				}
				else
				{
					count += streamBuf->getBufSize();
					FriendBufVec.push_back(streamBuf.get());
					streamBuf = new crStreamBuf;
					streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

					streamBuf->_writeInt(chatFriend->getID());//4
					streamBuf->_writeInt(chatFriend->getPlayerID());//4
					streamBuf->_writeString(chatFriend->getColumnName());//20+4
					streamBuf->_writeString(chatFriend->getRemark());//20+4
					streamBuf->_writeChar(chatFriend->getFriendChatState());//1
					streamBuf->_writeString(chatFriend->getNickName());//20+4
				}
			}
            if(streamBuf->getBufSize()>0)
			{
				count += streamBuf->getBufSize();
				FriendBufVec.push_back(streamBuf.get());
			}
			//向客户端发送好友列表
			if(count>0)
			{
				int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
				crChatFriendListPacket chatPlayerDataPacket;
				crChatFriendListPacket::buildRequestPacket(chatPlayerDataPacket,playerData->getPlayerID(),streamid,count);
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),chatPlayerDataPacket);
				crDataStreamPacket streamPacket;
				for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = FriendBufVec.begin();
					itr != FriendBufVec.end();
					++itr )
				{
					crDataStreamPacket::buildRequestPacket(streamPacket,playerData->getPlayerID(),streamid,crChatFriendListPacket::ID,itr->get());
					m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),streamPacket);
				}
				crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
			}
		}
	}

	{//////////查询群列表
		ref_ptr<crPlayerGroupTableQueryData> playerGroupTabQuery = new crPlayerGroupTableQueryData;
		playerGroupTabQuery->buildQuerySql(playerData->getPlayerID());

		chatSession->executeQuery(playerGroupTabQuery.get());
		crDataBase::QueryResultVec playerGroupTabQueryResultVec = chatSession->getQueryResult();
		chatSession->releaseQuery();
		crNetChatData::crChatGroup *chatGroup;
		if(!playerGroupTabQueryResultVec.empty())
		{
			std::vector< ref_ptr<crStreamBuf> > BufVec;
			int maxNeedSize = 72;
			int count = 0;
			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
			ref_ptr<crChatGroupTableByIDQueryData> groupTableByIDQuery = new crChatGroupTableByIDQueryData;
			for( crDataBase::QueryResultVec::iterator itr = playerGroupTabQueryResultVec.begin();
				itr != playerGroupTabQueryResultVec.end();
				++itr )
			{
				playerGroupTabQuery = dynamic_cast<crPlayerGroupTableQueryData *>(itr->get());

				groupTableByIDQuery->buildQuerySql(playerGroupTabQuery->getGroupID());
				globalSession->executeQuery(groupTableByIDQuery.get());
				crDataBase::QueryResultVec groupTableByIDQueryResultVec = globalSession->getQueryResult();
				globalSession->releaseQuery();
				if(!groupTableByIDQueryResultVec.empty())
				{
					groupTableByIDQuery = dynamic_cast<crChatGroupTableByIDQueryData *>(groupTableByIDQueryResultVec[0].get());

					if(streamBuf->getRemainCapacity()>maxNeedSize)
					{
						streamBuf->_writeInt(playerGroupTabQuery->getID());//4
						streamBuf->_writeInt(playerGroupTabQuery->getGroupID());//4
						streamBuf->_writeString(playerGroupTabQuery->getRemark());//20+4

						streamBuf->_writeString(groupTableByIDQuery->getGroupName());//20+4
						streamBuf->_writeInt(groupTableByIDQuery->getCreator());
						streamBuf->_writeInt(groupTableByIDQuery->getManager1());
						streamBuf->_writeInt(groupTableByIDQuery->getManager2());
						streamBuf->_writeInt(groupTableByIDQuery->getManager3());
					}
					else
					{
						count += streamBuf->getBufSize();
						BufVec.push_back(streamBuf.get());
						streamBuf = new crStreamBuf;
						streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

						streamBuf->_writeInt(playerGroupTabQuery->getID());//4
						streamBuf->_writeInt(playerGroupTabQuery->getGroupID());//4
						streamBuf->_writeString(playerGroupTabQuery->getRemark());//20+4

						streamBuf->_writeString(groupTableByIDQuery->getGroupName());//20+4
						streamBuf->_writeInt(groupTableByIDQuery->getCreator());
						streamBuf->_writeInt(groupTableByIDQuery->getManager1());
						streamBuf->_writeInt(groupTableByIDQuery->getManager2());
						streamBuf->_writeInt(groupTableByIDQuery->getManager3());
					}

					chatGroup = new crNetChatData::crChatGroup;
					chatGroup->setID(playerGroupTabQuery->getID());
					chatGroup->setGroupID(playerGroupTabQuery->getGroupID());
					chatGroup->setGroupRemark(playerGroupTabQuery->getRemark());

					chatGroup->setGroupName(groupTableByIDQuery->getGroupName());
					chatGroup->setCreator(groupTableByIDQuery->getCreator());
					chatGroup->setManager1(groupTableByIDQuery->getManager1());
					chatGroup->setManager2(groupTableByIDQuery->getManager2());
					chatGroup->setManager3(groupTableByIDQuery->getManager3());
					playerData->getNetChatData()->insertChatGroup(chatGroup);
				}
			}
			if(streamBuf->getBufSize()>0)
			{
				count += streamBuf->getBufSize();
				BufVec.push_back(streamBuf.get());
			}
			if (count>0)
			{
				int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
				crChatGroupListPacket chatPlayerDataPacket;
				crChatGroupListPacket::buildRequestPacket(chatPlayerDataPacket,playerData->getPlayerID(),streamid,count);
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),chatPlayerDataPacket);
				crDataStreamPacket streamPacket;
				for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					itr != BufVec.end();
					++itr )
				{
					crDataStreamPacket::buildRequestPacket(streamPacket,playerData->getPlayerID(),streamid,crChatGroupListPacket::ID,itr->get());
					m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),streamPacket);
				}
				crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
			}
		}
	}

	//////查询群成员
	ref_ptr<crGroupMemberTableQueryData> groupMemberTabQuery;
	ref_ptr<crNetChatData::crChatGroupMember> chatGroupMember,chatGroupMember2;
	playerData->getNetChatData()->lockGroupMap();
	crNetChatData::ChatGroupMap &chatGroupMap = playerData->getNetChatData()->getChatGroupMap();
	for( crNetChatData::ChatGroupMap::iterator gitr = chatGroupMap.begin();
		gitr != chatGroupMap.end();
		++gitr )
	{
		groupMemberTabQuery = new crGroupMemberTableQueryData;
		groupMemberTabQuery->buildQuerySql(gitr->second->getGroupID());

		globalSession->executeQuery(groupMemberTabQuery.get());
		crDataBase::QueryResultVec groupMemberTabQueryResultVec = globalSession->getQueryResult();
		globalSession->releaseQuery();

		if(!groupMemberTabQueryResultVec.empty())
		{
			std::vector< ref_ptr<crStreamBuf> > BufVec;
			int maxNeedSize = 81;
			int count;
			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
			char chatstate = 0;
			streamBuf->_writeInt(gitr->first);
			count = 0;
			for( crDataBase::QueryResultVec::iterator itr = groupMemberTabQueryResultVec.begin();
				itr != groupMemberTabQueryResultVec.end();
				++itr )
			{
				groupMemberTabQuery = dynamic_cast<crGroupMemberTableQueryData *>(itr->get());

				chatGroupMember = new crNetChatData::crChatGroupMember;
				chatGroupMember->setMemberID(groupMemberTabQuery->getID());
				chatGroupMember->setPlayerID(groupMemberTabQuery->getPlayerID());
				chatGroupMember->setMemberName(groupMemberTabQuery->getMemberName());
				chatGroupMember->setMemberRemark(groupMemberTabQuery->getMemberRemark());
				
				if(chatGroupMember->getPlayerID() != playerData->getPlayerID())
				{
					informed = false;
					if(InformedPlayerSet.find(chatGroupMember->getPlayerID())!=InformedPlayerSet.end())
					{
						informed = true;
					}
					else
					{
						InformedPlayerSet.insert(chatGroupMember->getPlayerID());
					}

					loginedChatPlayer = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(chatGroupMember->getPlayerID()));
					if(loginedChatPlayer.valid())
					{
						chatstate = loginedChatPlayer->getNetChatData()->getMyChatState();
						chatGroupMember->setMemberChatState(loginedChatPlayer->getNetChatData()->getMyChatState());
						chatGroupMember->setNickName(loginedChatPlayer->getNickName());

						if(!informed)
						{
							crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),chatGroupMember->getPlayerID(),crNetChatData::OnLine,playerData->getNickName());
							m_netConductor->getNetManager()->sendPacket(loginedChatPlayer->getPlayerConnectServerAddress(),chatStateChangePacket);
						}

						crNetChatData::crChatGroup *chatGroup = loginedChatPlayer->getNetChatData()->findChatGroup(gitr->first);
						if(chatGroup)
						{
							chatGroupMember2 = chatGroup->getMember(playerData->getPlayerID());
							if(chatGroupMember2.valid())
							{
								chatGroupMember2->setMemberChatState(crNetChatData::OnLine);
								chatGroupMember2->setNickName(playerData->getNickName());
							}
						}
					}
					else if(!informed)
					{//从数据库检查是否在线
						ref_ptr<crChatStateNickNameQueryData> chatStateNickNameQuery = new crChatStateNickNameQueryData;
						chatStateNickNameQuery->buildQuerySql(chatGroupMember->getPlayerID());

						globalSession->executeQuery(chatStateNickNameQuery.get());
						crDataBase::QueryResultVec &chatStateNickNameQueryResultVec = globalSession->getQueryResult();
						if(!chatStateNickNameQueryResultVec.empty())
						{
							chatStateNickNameQuery = dynamic_cast<crChatStateNickNameQueryData *>(chatStateNickNameQueryResultVec[0].get());
							chatstate = chatStateNickNameQuery->getChatState();
							const std::string& nickName = chatStateNickNameQuery->getNickName();
							chatGroupMember->setNickName(nickName);
						}
						globalSession->releaseQuery();

						chatGroupMember->setMemberChatState(chatstate);
						if(chatstate>0 && parentServer)
						{//如在线
							crChatStateChangePacket::buildRequestPacket(chatStateChangePacket,playerData->getPlayerID(),chatGroupMember->getPlayerID(),crNetChatData::OnLine,playerData->getNickName());
							parentServer->getNetManager()->sendPacket("all",chatStateChangePacket);
						}
					}
				}
				else
				{
					chatGroupMember->setMemberChatState(playerData->getNetChatData()->getMyChatState());
					chatGroupMember->setNickName(playerData->getNickName());
				}
				gitr->second->insertMember(chatGroupMember.get());

				if(streamBuf->getRemainCapacity()>maxNeedSize)
				{
					streamBuf->_writeInt(chatGroupMember->getMemberID());//4
					streamBuf->_writeInt(chatGroupMember->getPlayerID());//4
					streamBuf->_writeString(chatGroupMember->getMemberName());//20+4
					streamBuf->_writeString(chatGroupMember->getMemberRemark());//20+4
					streamBuf->_writeChar(chatGroupMember->getMemberChatState());//1
					streamBuf->_writeString(chatGroupMember->getNickName());//20+4
				}
				else
				{
					count += streamBuf->getBufSize();
					BufVec.push_back(streamBuf.get());
					streamBuf = new crStreamBuf;
					streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

					streamBuf->_writeInt(chatGroupMember->getMemberID());//4
					streamBuf->_writeInt(chatGroupMember->getPlayerID());//4
					streamBuf->_writeString(chatGroupMember->getMemberName());//20+4
					streamBuf->_writeString(chatGroupMember->getMemberRemark());//20+4
					streamBuf->_writeChar(chatGroupMember->getMemberChatState());//1
					streamBuf->_writeString(chatGroupMember->getNickName());//20+4
				}
			}
			if(streamBuf->getBufSize()>0)
			{
				count += streamBuf->getBufSize();
				BufVec.push_back(streamBuf.get());
			}
			if (count>0)
			{
				int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
				crChatGroupMemberListPacket chatPlayerDataPacket;
				crChatGroupMemberListPacket::buildRequestPacket(chatPlayerDataPacket,playerData->getPlayerID(),streamid,count);
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),chatPlayerDataPacket);
				crDataStreamPacket streamPacket;
				for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
					itr != BufVec.end();
					++itr )
				{
					crDataStreamPacket::buildRequestPacket(streamPacket,playerData->getPlayerID(),streamid,crChatGroupMemberListPacket::ID,itr->get());
					m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),streamPacket);
				}
				crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
			}
		}
	}
	playerData->getNetChatData()->unlockGroupMap();

	crChatDataTransferEndPacket packet;
	crChatDataTransferEndPacket::buildRequestPacket(packet,playerData->getPlayerID());
    m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
	playerData->getNetChatData()->setMyChatState(crNetChatData::OnLine);

	//接收留言
	{
		ref_ptr<crPlayerFriendLeavewordQueryData> chatFriendLeavewordQuery;
		playerData->getNetChatData()->lockFriendMap();
		crNetChatData::FriendMap &friendMap = playerData->getNetChatData()->getFriendMap();
		crChatPacket chatPacket;
		for( crNetChatData::FriendMap::iterator itr = friendMap.begin();
			itr != friendMap.end();
			++itr )
		{
			chatFriendLeavewordQuery = new crPlayerFriendLeavewordQueryData;
			chatFriendLeavewordQuery->buildQuerySql(itr->second->getID());
			chatSession->executeQuery(chatFriendLeavewordQuery.get());
			crDataBase::QueryResultVec chatFriendLeavewordQueryResultVec = chatSession->getQueryResult();
            chatSession->releaseQuery();
			if(!chatFriendLeavewordQueryResultVec.empty())
			{
				for( crDataBase::QueryResultVec::iterator qitr = chatFriendLeavewordQueryResultVec.begin();
					qitr != chatFriendLeavewordQueryResultVec.end();
					++qitr )
				{
					chatFriendLeavewordQuery = dynamic_cast<crPlayerFriendLeavewordQueryData *>(qitr->get());
					crChatPacket::buildReplyPacket2(chatPacket,const_cast<crStreamBuf *>(chatFriendLeavewordQuery->getLeaveword()));
					m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),chatPacket);
				}

				//清除DB里的留言
				ref_ptr<crRemovePlayerFriendLeaveword> clearLeaveword = new crRemovePlayerFriendLeaveword;
				clearLeaveword->buildUpdateSql(itr->second->getID());
				if(chatSession->executeUpdate(clearLeaveword.get()))
				{//成功
					chatSession->commit();
				}
			}
		}
		playerData->getNetChatData()->unlockFriendMap();
	}
	//接收群留言
	{
		ref_ptr<crPlayerGroupLeavewordQueryData> chatGroupLeavewordQuery;
		playerData->getNetChatData()->lockGroupMap();
		crNetChatData::ChatGroupMap &groupMap = playerData->getNetChatData()->getChatGroupMap();
		crGroupChatPacket chatPacket;
		for( crNetChatData::ChatGroupMap::iterator itr = groupMap.begin();
			itr != groupMap.end();
			++itr )
		{
			chatGroupLeavewordQuery = new crPlayerGroupLeavewordQueryData;
			chatGroupLeavewordQuery->buildQuerySql(itr->second->getID());
			chatSession->executeQuery(chatGroupLeavewordQuery.get());
			crDataBase::QueryResultVec chatGroupLeavewordQueryResultVec = chatSession->getQueryResult();
			chatSession->releaseQuery();
			if(!chatGroupLeavewordQueryResultVec.empty())
			{
				for( crDataBase::QueryResultVec::iterator qitr = chatGroupLeavewordQueryResultVec.begin();
					qitr != chatGroupLeavewordQueryResultVec.end();
					++qitr )
				{
					chatGroupLeavewordQuery = dynamic_cast<crPlayerGroupLeavewordQueryData *>(qitr->get());
					crGroupChatPacket::buildReplyPacket2(chatPacket,const_cast<crStreamBuf *>(chatGroupLeavewordQuery->getLeaveword()));
					m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),chatPacket);
				}

				//清除DB里的留言
				ref_ptr<crRemovePlayerGroupLeaveword> clearLeaveword = new crRemovePlayerGroupLeaveword;
				clearLeaveword->buildUpdateSql(itr->second->getID());
				if(chatSession->executeUpdate(clearLeaveword.get()))
				{//成功
					chatSession->commit();
				}
			}
		}
		playerData->getNetChatData()->unlockGroupMap();
	}
}

void crLoginChatPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发到ChatServer
		crLoginChatPacket packet;
		crLoginChatPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServerClient_Chat);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginChatPacket SendTo ChatServer"<<std::endl;
	}
	else if(netType == ChatServer)
	{
		//从数据库读入我的聊天信息数据：如好友列表、群列表
		std::string playerAddress = m_streamBuf->_readString();
        int playerid = m_streamBuf->_readInt();
		//std::string name = m_streamBuf->_readString();
		std::string key = m_streamBuf->_readString();
		//std::string basecode = CRNet::getBaseCode(name,playerAddress);
		char loginCode = 0; 
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crNetManager *netManager = m_netConductor->getNetManager();
		ref_ptr<crChatServerPlayerData> playerData;
		//if(CRNet::checkKey(key/*,basecode*/))
		//{//key验证通过
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();

			ref_ptr<crLoginChatQueryData> queryResult;
			ref_ptr<crLoginChatQueryData> loginChatQueryData = new crLoginChatQueryData;
			loginChatQueryData->buildQuerySql(playerid);

			globalSession->executeQuery(loginChatQueryData.get());
			crDataBase::QueryResultVec &queryResultVec = globalSession->getQueryResult();
			if(!queryResultVec.empty())
			{
				queryResult = dynamic_cast<crLoginChatQueryData *>(queryResultVec[0].get());
			}
			globalSession->releaseQuery();

			unsigned short chatdbid = queryResult->getChatDBID();
			crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
			const std::string &dbkey = queryResult->getKey();
			if(chatdb && dbkey.compare(key) == 0)
			{
				loginCode = 1;
				//bool foundPreLogin = false;
				ref_ptr<crChatServerPlayerData> connectedPlayerData  = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(playerid));
				if(connectedPlayerData.valid())
				{
					//foundPreLogin = true;
					crAppServerRefusePacket refusePacket;
					crAppServerRefusePacket::buildRequestPacket(refusePacket,connectedPlayerData->getPlayerID()/*,connectedPlayerData->getPlayerAdress()*/);
					netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),refusePacket);
					crNetConductor *parentConductor = connectedPlayerData->getParentServerLineConductor();
					if(parentConductor)
					{
						parentConductor->getNetManager()->sendPacket("all",refusePacket);
					}
					//CRCore::crThread::yieldCurrentThread();
				}

				crNetChatData *netChatData = new crNetChatData;
				netChatData->setMyChatState(crNetChatData::OnLine);
				ref_ptr<crUpdateChatState> updateLoginDate = new crUpdateChatState;
				updateLoginDate->buildUpdateSql(playerid,crNetChatData::OnLine);
				if(globalSession->executeUpdate(updateLoginDate.get()))
				{//插入成功
					globalSession->commit();
				}

				playerData = new crChatServerPlayerData;
				playerData->setPlayerID(playerid);
				//playerData->setPlayerName(name);
				playerData->setPlayerAdress(playerAddress);
				playerData->setPlayerConnectServerAddress(sender);
				playerData->setChatDBID(queryResult->getChatDBID());
				playerData->setNickName(queryResult->getNickName());

				playerData->setNetChatData(netChatData);

				crLoginChatPacket packet;
				crLoginChatPacket::buildReplyPacket(packet,playerAddress,loginCode,playerData.get());
				netManager->sendPacket(sender,packet);

				crNetConductor *parentServer = crNetContainer::getInstance()->getNetConductor(ChatServerClient_Parent);
				if(parentServer)
				{
					crLoginParentPacket loginParentPacket;
					crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerAddress);
					parentServer->getNetManager()->sendPacket("all",loginParentPacket);
					playerData->setParentServerLineConductor(parentServer);
				}

				ref_ptr<crDataBase> chatSession = chatdb->beginSession();
				chatPlayerDataQuery(playerData.get(),globalSession.get(),chatSession.get());
				chatdb->endSession(chatSession.get());

				netDataManager->insertPlayerData(playerData.get());
			}

			db->endSession(globalSession.get());
		//}
		if(loginCode==0)
		{
			crLoginChatPacket packet;
			crLoginChatPacket::buildReplyPacket(packet,playerAddress,loginCode,playerData.get());
			netManager->sendPacket(sender,packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginChatPacket "<<loginCode<<std::endl;
	}
	else if(netType == ChatConnectServerClient_Chat)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			char loginCode = m_streamBuf->_readChar();
			if(loginCode>0)
			{
				int playerid = m_streamBuf->_readInt();
				crNetDataManager *netDataManager = connectServer->getNetDataManager();
				netDataManager->lockPlayerDataMap();
				//ref_ptr<crChatConnectServerPlayerData> oldPlayerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(playerid));
				//if(oldPlayerData.valid())
				//{
				//	crAppServerRefusePacket refusePacket;
				//	crAppServerRefusePacket::buildRequestPacket(refusePacket,playerid/*,oldPlayerData->getPlayerAdress()*/);
				//	crNetConductor *cameraServer = oldPlayerData->getCameraServerLineConductor();
				//	if(cameraServer)
				//	{
				//		cameraServer->getNetManager()->sendPacket("all",refusePacket);
				//	}
				//	crNetConductor *voiceServer = oldPlayerData->getVoiceServerLineConductor();
				//	if(voiceServer)
				//	{
				//		voiceServer->getNetManager()->sendPacket("all",refusePacket);
				//	}
				//	crAppServerRefusePacket packet;
				//	crAppServerRefusePacket::buildRequestPacket2(packet,-1);
				//	connectServer->getNetManager()->sendPacket(oldPlayerData->getPlayerAdress(),packet);
				//}

				crChatConnectServerPlayerData *playerData = new crChatConnectServerPlayerData;
				playerData->setPlayerID(playerid);
				//playerData->setPlayerName(m_streamBuf->_readString());
				playerData->setPlayerAdress(address);
				playerData->setChatServerLineConductor(m_netConductor);

				netDataManager->insertPlayerData(playerData);
				netDataManager->unLockPlayerDataMap();
			}
			crLoginChatPacket packet;
			crLoginChatPacket::buildReplyPacket2(packet,loginCode);
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crLoginChatPacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		char loginCode = m_streamBuf->_readChar();
		//s_success = loginCode==0?-1:loginCode;
		if(loginCode>0)
		{
			crNetChatData *netChatData = crMyPlayerData::getInstance()->getMyChatData();
			if(!netChatData)
			{
				netChatData = new crNetChatData;
				crMyPlayerData::getInstance()->setMyChatData(netChatData);
			}
			netChatData->setMyChatState(crNetChatData::OnLine);
		}
		crGlobalHandle::getInstance()->doEvent(WCH_LoginChatReturn,MAKEINT64(loginCode,NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"crLoginChatPacket "<<loginCode<<std::endl;
	}
}

/////////////////////////////////////////
//
//crLoginParentPacket
//
/////////////////////////////////////////
crLoginParentPacket::crLoginParentPacket():
crStreamPacket(crLoginParentPacket::ID)
{
}

crLoginParentPacket::crLoginParentPacket( const crLoginParentPacket &p ):
crStreamPacket(crLoginParentPacket::ID,p)
{
}

void crLoginParentPacket::buildRequestPacket(crLoginParentPacket &packet,int playerid,const std::string &playerAddress)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+playerAddress.size());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeString(playerAddress);
}

void crLoginParentPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ParentServer)
	{
		int id = m_streamBuf->_readInt();
		std::string playerAddress = m_streamBuf->_readString();
        //bool foundPreLogin = m_streamBuf->_readBool();

		//if(!foundPreLogin)
		//{
			ref_ptr<crParentServerPlayerData> connectedPlayerData  = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(id));
			if(connectedPlayerData.valid())
			{
				//foundPreLogin = true;
				crAppServerRefusePacket refusePacket;
				crAppServerRefusePacket::buildRequestPacket(refusePacket,connectedPlayerData->getPlayerID()/*,connectedPlayerData->getPlayerAdress()*/);
				m_netConductor->getNetManager()->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),refusePacket);

				crNetConductor *parentConductor = connectedPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					parentConductor->getNetManager()->sendPacket("all",refusePacket);
				}
				//CRCore::crThread::yieldCurrentThread();
			}
		//}

        crParentServerPlayerData *playerData = new crParentServerPlayerData;
		playerData->setPlayerID(id);
		playerData->setPlayerAdress(playerAddress);
		playerData->setPlayerConnectServerAddress(sender);
		m_netConductor->getNetDataManager()->insertPlayerData(playerData);
		crNetConductor *parentConductor = crNetContainer::getInstance()->getNetConductor(ParentServerClient_Parent);
		if(parentConductor)
		{
			crLoginParentPacket loginParentPacket;
			crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerAddress);
			parentConductor->getNetManager()->sendPacket("all",loginParentPacket);
			playerData->setParentServerLineConductor(parentConductor);
		}
	}
}
/////////////////////////////////////////
//
//crChatStateChangePacket
//
/////////////////////////////////////////
crChatStateChangePacket::crChatStateChangePacket():
crStreamPacket(crChatStateChangePacket::ID)
{
}

crChatStateChangePacket::crChatStateChangePacket( const crChatStateChangePacket &p ):
crStreamPacket(crChatStateChangePacket::ID,p)
{
}

void crChatStateChangePacket::buildRequestPacket(crChatStateChangePacket &packet,int senderid, int recvid, char chatState,const std::string &nickName)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(chatState);
	streamBuf->_writeString(nickName);
}

void crChatStateChangePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ParentServer)
	{
		int sendid = m_streamBuf->_readInt();
        int recvid = m_streamBuf->_readInt();
		m_streamBuf->seekBegin();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
		if(playerData.valid())
		{
			m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{
            ref_ptr<crParentServerPlayerData> playerData = dynamic_cast<crParentServerPlayerData *>(netDataManager->getPlayerData(sendid));
			if(playerData.valid())
			{
                crNetConductor *parentConductor = playerData->getParentServerLineConductor();
				if(parentConductor)
				{
				    parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crChatStateChangePacket"<<std::endl;
	}
	else if(netType == ChatServerClient_Parent)
	{
		int sendid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			crNetDataManager *netDataManager = chatServerConductor->getNetDataManager();
			ref_ptr<crChatServerPlayerData> playerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				char chatState = m_streamBuf->_readChar();
				std::string nickName = m_streamBuf->_readString();

				crNetChatData::crChatFriend *chatFriend;
				crNetChatData::crChatGroupMember *chatMember;
				chatFriend = playerData->getNetChatData()->getFriend(sendid);
				if(chatFriend)
				{
					chatFriend->setFriendChatState(chatState);
					chatFriend->setNickName(nickName);
				}
				playerData->getNetChatData()->lockGroupMap();
				crNetChatData::ChatGroupMap &chatGroupMap = playerData->getNetChatData()->getChatGroupMap();
				for( crNetChatData::ChatGroupMap::iterator cgitr = chatGroupMap.begin();
					cgitr != chatGroupMap.end();
					cgitr++ )
				{
					chatMember = cgitr->second->getMember(sendid);
					if(chatMember)
					{
						chatMember->setMemberChatState(chatState);
						chatMember->setNickName(nickName);
					}
				}
                playerData->getNetChatData()->unlockGroupMap();
				m_streamBuf->seekBegin();
                chatServerConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);

				//CRCore::notify(CRCore::ALWAYS)<<"crChatStateChangePacket"<<std::endl;
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatStateChangePacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		crGlobalHandle::getInstance()->doEvent(WCH_ChatStateChange,MAKEINT64(m_streamBuf.get(),NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"crChatStateChangePacket"<<std::endl;
	}
}
/////////////////////////////////////////
//
//crCreateDataStreamPacket
//
/////////////////////////////////////////
crCreateDataStreamPacket::crCreateDataStreamPacket():
crStreamPacket(crCreateDataStreamPacket::ID)
{
}

crCreateDataStreamPacket::crCreateDataStreamPacket( const crCreateDataStreamPacket &p ):
crStreamPacket(crCreateDataStreamPacket::ID,p)
{
}

void crCreateDataStreamPacket::buildRequestPacket(crCreateDataStreamPacket &packet,int recvid,int streamid,int packetid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(packetid);
	streamBuf->_writeInt(count);
}

void crCreateDataStreamPacket::buildReplyPacket(crCreateDataStreamPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateDataStreamPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{//
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			ref_ptr<crGameConnectServerPlayerData> recvPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				crCreateDataStreamPacket packet;
				crCreateDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),packet);
			}
		}
	}
	else if(netType == GameClient_Game)
	{
		int streamid = m_streamBuf->_readInt();
		int packetid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,packetid,count);
		//CRCore::notify(CRCore::ALWAYS)<<"crCreateDataStreamPacket count = "<<count<<std::endl;
	}
}
/////////////////////////////////////////
//
//crDataStreamPacket
//
/////////////////////////////////////////
crDataStreamPacket::crDataStreamPacket():
crStreamPacket(crDataStreamPacket::ID)
{
}

crDataStreamPacket::crDataStreamPacket( const crDataStreamPacket &p ):
crStreamPacket(crDataStreamPacket::ID,p)
{
}

void crDataStreamPacket::buildRequestPacket(crDataStreamPacket &packet,int recvid,int streamid,int packetid,CRCore::crStreamBuf *buf)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+buf->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(packetid);
	streamBuf->write(buf->getBuf(),buf->getBufSize());
}

void crDataStreamPacket::buildRequestPacket(crDataStreamPacket &packet,const std::string &address,int streamid,int packetid,CRCore::crStreamBuf *buf)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+12+buf->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(packetid);
	streamBuf->write(buf->getBuf(),buf->getBufSize());
}

void crDataStreamPacket::buildReplyPacket(crDataStreamPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crDataStreamPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	////////ChatServer
	if(netType == ChatConnectServer)
	{//GameClient_Chat->ChatConnectServer->ChatServer
		int recvid = m_streamBuf->_readInt();
		int senderid = m_streamBuf->_readInt();
		ref_ptr<crChatConnectServerPlayerData> recvPlayerData = dynamic_cast<crChatConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),*this);
		}
		else
		{
			ref_ptr<crChatConnectServerPlayerData> senderPlayerData = dynamic_cast<crChatConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *netConductor = senderPlayerData->getChatServerLineConductor();
				if(netConductor)
				{
					m_streamBuf->seekBegin();
					netConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ChatServer)
	{//ChatConnectServer->ChatServer|recvid;
		int recvid = m_streamBuf->_readInt();
		int senderid = m_streamBuf->_readInt();
		if(recvid>0)
		{//如果有目标
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
			else
			{//senderid必须>0 发送到parentServer
				ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
				if(senderPlayerData.valid())
				{
					crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
					if(parentConductor)
					{
						m_streamBuf->seekBegin();
						parentConductor->getNetManager()->sendPacket("all",*this);
					}
				}
			}
		}
		else
		{//此数据流是发送给ChatServer的
			ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				//senderPlayerData->doEvent(WCH_StreamDataRevChat,MAKEINT64(m_streamBuf.get(),NULL));
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{//ChatServer->ChatConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			ref_ptr<crChatConnectServerPlayerData> recvPlayerData = dynamic_cast<crChatConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				crDataStreamPacket streamPacket;
				crDataStreamPacket::buildReplyPacket(streamPacket,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),streamPacket);
			}
		}
	}
	else if(netType == ParentServer)
	{
		int recvid = m_streamBuf->_readInt();
		int senderid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{//
			ref_ptr<crParentServerPlayerData> senderPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					m_streamBuf->seekBegin();
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ParentServerClient_Parent)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(parentServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == ChatServerClient_Parent)
	{//ChatServer->ChatConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{
		//crHeartJumpPacket packet;
		//m_netConductor->getNetManager()->sendPacket("all",packet);
		crDataStreamTransfer::getInstance()->recvStream(m_streamBuf.get());
		//crGlobalHandle::getInstance()->doEvent(WCH_StreamDataRevChat,MAKEINT64(m_streamBuf.get(),NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"StreamDataRevChat count = "<<m_streamBuf->getRemainSize()<<std::endl;
	}
	///////////DownloadServer
	else if(netType == GameConnectServerClient_Download)
	{//DownloadServer->DownloadConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			ref_ptr<crGameConnectServerPlayerData> recvPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				crDataStreamPacket packet;
				crDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),packet);
			}
		}
	}
	else if(netType == DownloadConnectServerClient_Download)
	{//DownloadServer->DownloadConnectServer->recvid
		//int recvid = m_streamBuf->_readInt();
		//crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(DownloadConnectServer);
		//if(connectServerConductor)
		//{
		//	crDownloadConnectServerPlayerData *recvPlayerData = dynamic_cast<crDownloadConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
		//	if(recvPlayerData)
		//	{
		//		crDataStreamPacket packet;
		//		crDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
		//		connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),packet);
		//	}
		//}
		std::string playerAddress = m_streamBuf->_readString();
		crDataStreamPacket packet;
		crDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(DownloadConnectServer);
		connectServerConductor->getNetManager()->sendPacket(playerAddress,packet);
	}
	else if(netType == GameClient_Download)
	{
		//crHeartJumpPacket packet;
		//m_netConductor->getNetManager()->sendPacket("all",packet);
		crDataStreamTransfer::getInstance()->recvStream(m_streamBuf.get());
		//CRCore::notify(CRCore::ALWAYS)<<"WCH_StreamDataRevDownload count = "<<m_streamBuf->getRemainSize()<<std::endl;
	}
	////////GameServer
	if(netType == GameConnectServer)
	{//GameClient_Game->GameConnectServer->GameServer
		int recvid = m_streamBuf->_readInt();
		int senderid = m_streamBuf->_readInt();
		ref_ptr<crGameConnectServerPlayerData> recvPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),*this);
		}
		else
		{
			ref_ptr<crGameConnectServerPlayerData> senderPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *netConductor = senderPlayerData->getGameServerLineConductor();
				if(netConductor)
				{
					m_streamBuf->seekBegin();
					netConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == GameServer)
	{//GameConnectServer->GameServer|recvid;
		int recvid = m_streamBuf->_readInt();
		int senderid = m_streamBuf->_readInt();
		if(recvid>0)
		{//如果有目标
			ref_ptr<crGameServerPlayerData> recvPlayerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
			else
			{//senderid必须>0 发送到parentServer
				ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
				if(senderPlayerData.valid())
				{
					crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
					if(parentConductor)
					{
						m_streamBuf->seekBegin();
						parentConductor->getNetManager()->sendPacket("all",*this);
					}
				}
			}
		}
		else
		{//此数据流是发送给GameServer的
			ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				//senderPlayerData->doEvent(CSM_StreamDataRev,MAKEINT64(m_streamBuf.get(),NULL));
			}
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{//GameServer->GameConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			ref_ptr<crGameConnectServerPlayerData> recvPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				crDataStreamPacket packet;
				crDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),packet);
			}
		}
	}
	else if(netType == GameServerClient_Parent)
	{//GameServer->GameConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(gameServerConductor)
		{
			ref_ptr<crGameServerPlayerData> recvPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				gameServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Game)
	{
		//crHeartJumpPacket packet;
		//m_netConductor->getNetManager()->sendPacket("all",packet);
		crDataStreamTransfer::getInstance()->recvStream(m_streamBuf.get());
		//CRCore::notify(CRCore::ALWAYS)<<"WCH_StreamDataRevGame count = "<<m_streamBuf->getRemainSize()<<std::endl;
	}
	//////SceneServer
	//if(netType == SceneConnectServer)
	//{//GameClient_Game->SceneConnectServer->SceneServer
	//	int recvid = m_streamBuf->_readInt();
	//	int senderid = m_streamBuf->_readInt();
	//	crSceneConnectServerPlayerData *recvPlayerData = dynamic_cast<crSceneConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
	//	if(recvPlayerData)
	//	{
	//		m_streamBuf->seekBegin();
	//		m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),*this);
	//	}
	//	else
	//	{
	//		crSceneConnectServerPlayerData *senderPlayerData = dynamic_cast<crSceneConnectServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
	//		if(senderPlayerData)
	//		{
	//			crNetConductor *netConductor = senderPlayerData->getSceneServerLineConductor();
	//			if(netConductor)
	//			{
	//				m_streamBuf->seekBegin();
	//				netConductor->getNetManager()->sendPacket("all",*this);
	//			}
	//		}
	//	}
	//}
	//else if(netType == SceneServer)
	//{//GameConnectServer->GameServer|recvid;
	//	int recvid = m_streamBuf->_readInt();
	//	int senderid = m_streamBuf->_readInt();
	//	if(recvid>0)
	//	{//如果有目标
	//		crSceneServerPlayerData *recvPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
	//		if(recvPlayerData)
	//		{
	//			m_streamBuf->seekBegin();
	//			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
	//		}
	//	}
	//	else
	//	{//此数据流是发送给GameServer的
	//		crSceneServerPlayerData *senderPlayerData = dynamic_cast<crSceneServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
	//		if(senderPlayerData)
	//		{
	//			//senderPlayerData->doEvent(CSM_StreamDataRev,MAKEINT64(m_streamBuf.get(),NULL));
	//		}
	//	}
	//}
	else if(netType == GameConnectServerClient_Scene)
	{//GameServer->GameConnectServer->recvid
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			ref_ptr<crGameConnectServerPlayerData> recvPlayerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				crDataStreamPacket packet;
				crDataStreamPacket::buildReplyPacket(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerAdress(),packet);
			}
		}
	}
	//else if(netType == GameClient_Game)
	//{
	//	crHeartJumpPacket packet;
	//	m_netConductor->getNetManager()->sendPacket("all",packet);
	//	CRCore::notify(CRCore::ALWAYS)<<"WCH_StreamDataRevScene count = "<<m_streamBuf->getRemainSize()<<std::endl;
	//	crDataStreamTransfer::getInstance()->recvStream(m_streamBuf.get());
	//}
	else if(netType == SceneServerClient_Game)
	{//gameserver发给sceneserver
		//int recvid = m_streamBuf->peekInt();
		//if(recvid>0)
		//{//同时转给客户端
		//	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		//	crSceneServerPlayerData *playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(recvid));
		//	if(playerData)
		//	{
		//		sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
		//	}
		//}
		//CRCore::notify(CRCore::ALWAYS)<<"WCH_StreamDataRevScene count = "<<m_streamBuf->getRemainSize()<<std::endl;
		m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->recvStream(m_streamBuf.get());
	}
}
/////////////////////////////////////////
//
//crChatFriendListPacket
//LoginChat返回
/////////////////////////////////////////
crChatFriendListPacket::crChatFriendListPacket():
crStreamPacket(crChatFriendListPacket::ID)
{
}

crChatFriendListPacket::crChatFriendListPacket( const crChatFriendListPacket &p ):
crStreamPacket(crChatFriendListPacket::ID,p)
{
}

void crChatFriendListPacket::buildRequestPacket(crChatFriendListPacket &packet,int recvid, int streamid, int count)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crChatFriendListPacket::buildRequestPacket2(crChatFriendListPacket &packet,CRCore::crStreamBuf *stream)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crChatFriendListPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crChatFriendListPacket packet;
				crChatFriendListPacket::buildRequestPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatFriendListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crChatFriendListPacket::ID,count);
		//CRCore::notify(CRCore::ALWAYS)<<"crChatFriendListPacket count = "<<count<<std::endl;
	}
}
/////////////////////////////////////////
//
//crChatGroupListPacket
//LoginChat返回
/////////////////////////////////////////
crChatGroupListPacket::crChatGroupListPacket():
crStreamPacket(crChatGroupListPacket::ID)
{
}

crChatGroupListPacket::crChatGroupListPacket( const crChatGroupListPacket &p ):
crStreamPacket(crChatGroupListPacket::ID,p)
{
}

void crChatGroupListPacket::buildRequestPacket(crChatGroupListPacket &packet,int recvid,int streamid,int count)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crChatGroupListPacket::buildRequestPacket2(crChatGroupListPacket &packet,CRCore::crStreamBuf *stream)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crChatGroupListPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crChatGroupListPacket packet;
				crChatGroupListPacket::buildRequestPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatGroupListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crChatGroupListPacket::ID,count);
	}
}
/////////////////////////////////////////
//
//crChatGroupMemberListPacket
//LoginChat返回
/////////////////////////////////////////
crChatGroupMemberListPacket::crChatGroupMemberListPacket():
crStreamPacket(crChatGroupMemberListPacket::ID)
{
}

crChatGroupMemberListPacket::crChatGroupMemberListPacket( const crChatGroupMemberListPacket &p ):
crStreamPacket(crChatGroupMemberListPacket::ID,p)
{
}

void crChatGroupMemberListPacket::buildRequestPacket(crChatGroupMemberListPacket &packet,int recvid,int streamid,int count)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}

void crChatGroupMemberListPacket::buildRequestPacket2(crChatGroupMemberListPacket &packet,CRCore::crStreamBuf *stream)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crChatGroupMemberListPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crChatGroupMemberListPacket packet;
				crChatGroupMemberListPacket::buildRequestPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatGroupMemberListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int streamid = m_streamBuf->_readInt();
		int count = m_streamBuf->_readInt();
		crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crChatGroupMemberListPacket::ID,count);
	}
}
/////////////////////////////////////////
//
//crChatDataTransferEndPacket
//LoginChat返回
/////////////////////////////////////////
crChatDataTransferEndPacket::crChatDataTransferEndPacket():
crStreamPacket(crChatDataTransferEndPacket::ID)
{
}

crChatDataTransferEndPacket::crChatDataTransferEndPacket( const crChatDataTransferEndPacket &p ):
crStreamPacket(crChatDataTransferEndPacket::ID,p)
{
}

void crChatDataTransferEndPacket::buildRequestPacket(crChatDataTransferEndPacket &packet,int recvid)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
}

void crChatDataTransferEndPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crChatDataTransferEndPacket packet;
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatDataTransferEndPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		crGlobalHandle::getInstance()->doEvent(WCH_ChatDataTransferEnd,MAKEINT64(NULL,NULL));
	    //CRCore::notify(CRCore::ALWAYS)<<"crChatDataTransferEndPacket"<<std::endl;
	}
}
/////////////////////////////////////////
//
//crAddChatFriendPacket
//
/////////////////////////////////////////
crAddChatFriendPacket::crAddChatFriendPacket():
crStreamPacket(crAddChatFriendPacket::ID)
{
}

crAddChatFriendPacket::crAddChatFriendPacket( const crAddChatFriendPacket &p ):
crStreamPacket(crAddChatFriendPacket::ID,p)
{
}

void crAddChatFriendPacket::buildRequestPacket(crAddChatFriendPacket &packet,int friendPlayerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(friendPlayerid);
}
void crAddChatFriendPacket::buildRequestPacket2(crAddChatFriendPacket &packet,int myPlayerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(myPlayerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crAddChatFriendPacket::buildReplyPacket(crAddChatFriendPacket &packet,int recvid, unsigned char befriendvalid, crNetChatData::crChatFriend *chatFriend)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);

	streamBuf->_writeInt(recvid);
	streamBuf->_writeUChar(befriendvalid);
	if(befriendvalid == 1)
	{
		streamBuf->_writeInt(chatFriend->getPlayerID());
		streamBuf->_writeChar((char)(chatFriend->getFriendChatState()));
		streamBuf->_writeString(chatFriend->getNickName());
	}
}

void crAddChatFriendPacket::buildReplyPacket2(crAddChatFriendPacket &packet, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
    streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crAddChatFriendPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crAddChatFriendPacket packet;
				crAddChatFriendPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crAddChatFriendPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int myPlayerid = m_streamBuf->_readInt();
		int friendPlayerid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> myPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(myPlayerid));
		if(myPlayerData.valid())
		{
			crNetChatData::crChatFriend *chatFriend = myPlayerData->getNetChatData()->getFriend(friendPlayerid);
			if(!chatFriend)
			{
				ref_ptr<crBeFriendValidQueryData> beFriendValidQueryData = new crBeFriendValidQueryData;
				unsigned char befriendvalid = 0;
				char chatstate = 0;
				std::string nickname;
				beFriendValidQueryData->buildQuerySql(friendPlayerid);
				crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
				ref_ptr<crDataBase> globalSession = db->beginSession();

				ref_ptr<crBeFriendValidQueryData> queryResult;
				globalSession->executeQuery(beFriendValidQueryData.get());
				crDataBase::QueryResultVec &queryResultVec = globalSession->getQueryResult();
				if(!queryResultVec.empty())
				{
					queryResult = dynamic_cast<crBeFriendValidQueryData *>(queryResultVec[0].get());
					befriendvalid = queryResult->getBeFriendValid();
					chatstate = queryResult->getChatState();
					nickname = queryResult->getNickName();
				}

				globalSession->releaseQuery();

				if(befriendvalid==1)
				{
					bool success = false;
					chatFriend = new crNetChatData::crChatFriend;
					chatFriend->setPlayerID(friendPlayerid);
					myPlayerData->getNetChatData()->insertFriend(chatFriend);
					chatFriend->setNickName(nickname);
					chatFriend->setFriendChatState(chatstate);

					unsigned short myChatdbid = myPlayerData->getChatDBID();

					ref_ptr<crChatServerPlayerData> friendPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(friendPlayerid));
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(40);
					stream->_writeInt(myPlayerid);//4
					stream->_writeString(myPlayerData->getNickName());//24
					unsigned short friendChatdbid = 0;
					if(friendPlayerData.valid())
					{
						friendChatdbid = friendPlayerData->getChatDBID();
						stream->seekBegin();
						crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_ChatSystemMsg,CD_AddChatFriend),MAKEINT64(stream.get(),friendPlayerData.get()));
					}
					else
					{
						ref_ptr<crChatDBIDQueryData> queryResult;
						ref_ptr<crChatDBIDQueryData> chatDBIDQueryData = new crChatDBIDQueryData;
						chatDBIDQueryData->buildQuerySql(friendPlayerid);

						globalSession->executeQuery(chatDBIDQueryData.get());
						crDataBase::QueryResultVec& queryResultVec = globalSession->getQueryResult();
						if(!queryResultVec.empty())
						{
							queryResult = dynamic_cast<crChatDBIDQueryData *>(queryResultVec[0].get());
							friendChatdbid = queryResult->getChatDBID();
						}
						globalSession->releaseQuery();

						CRNet::crNetConductor *parentLine = myPlayerData->getParentServerLineConductor();
						if(parentLine)
						{
							crChatPacket friendAddPacket;
							crChatPacket::buildRequestPacket(friendAddPacket,0,friendPlayerid,CD_AddChatFriend,stream.get());
							parentLine->getNetManager()->sendPacket("all",friendAddPacket);
						}
					}

					ref_ptr<crInsertPlayerFriend> insertChatFriend = new crInsertPlayerFriend;
					crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,myChatdbid);
					if(chatdb)
					{
						ref_ptr<crDataBase> chatSession = chatdb->beginSession();
						insertChatFriend->buildUpdateSql(myPlayerid,friendPlayerid);
						if(chatSession->executeUpdate(insertChatFriend.get()))
						{
							if(friendChatdbid == myChatdbid)
							{
								insertChatFriend->buildUpdateSql(friendPlayerid,myPlayerid);
								chatSession->executeUpdate(insertChatFriend.get());
							}
							chatSession->commit();
							success = true;
						}
						chatdb->endSession(chatSession.get());
					}
                    if(friendChatdbid != myChatdbid)
					{
						chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,friendChatdbid);
						if(chatdb)
						{
							ref_ptr<crDataBase> chatSession = chatdb->beginSession();
							insertChatFriend->buildUpdateSql(friendPlayerid,myPlayerid);
							if(chatSession->executeUpdate(insertChatFriend.get()))
							{
								chatSession->commit();
							}
							chatdb->endSession(chatSession.get());
						}
					}
					if(!success)
						befriendvalid = 0;
				}
				db->endSession(globalSession.get());

				crAddChatFriendPacket packet;
				crAddChatFriendPacket::buildReplyPacket(packet,myPlayerid,befriendvalid,chatFriend);
				m_netConductor->getNetManager()->sendPacket(myPlayerData->getPlayerConnectServerAddress(),packet);

				//CRCore::notify(CRCore::ALWAYS)<<"crAddChatFriendPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crAddChatFriendPacket packet;
				crAddChatFriendPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crAddChatFriendPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		unsigned char befriendvalid = m_streamBuf->_readUChar();
		if(befriendvalid==1)
		{
			crNetChatData *myChatData = crMyPlayerData::getInstance()->getMyChatData();
			if(!myChatData)
			{
				myChatData = new crNetChatData;
				crMyPlayerData::getInstance()->setMyChatData(myChatData);
			}
			if(myChatData)
			{
				crNetChatData::crChatFriend *chatFriend = new crNetChatData::crChatFriend;
				chatFriend->setPlayerID(m_streamBuf->_readInt());
				char chatstate = m_streamBuf->_readChar();
				chatFriend->setFriendChatState((crNetChatData::ChatState)(chatstate));
				chatFriend->setNickName(m_streamBuf->_readString());
				myChatData->insertFriend(chatFriend);

				crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_AddChatFriend,1),MAKEINT64(chatFriend->getPlayerID(),NULL));
			}
		}
		else if(befriendvalid == 0)
		{//提示对方拒绝添加
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_AddChatFriend,0),MAKEINT64(NULL,NULL));
		}
		else if(befriendvalid == 2)
		{//提示对方需要消息验证
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_AddChatFriend,2),MAKEINT64(NULL,NULL));
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crAddChatFriendPacket "<<std::endl;
	}
}
/////////////////////////////////////////
//
//crRemoveChatFriendPacket
//
/////////////////////////////////////////
crRemoveChatFriendPacket::crRemoveChatFriendPacket():
crStreamPacket(crRemoveChatFriendPacket::ID)
{
}

crRemoveChatFriendPacket::crRemoveChatFriendPacket( const crRemoveChatFriendPacket &p ):
crStreamPacket(crRemoveChatFriendPacket::ID,p)
{
}

void crRemoveChatFriendPacket::buildRequestPacket(crRemoveChatFriendPacket &packet,int friendPlayerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(friendPlayerid);
}
void crRemoveChatFriendPacket::buildRequestPacket2(crRemoveChatFriendPacket &packet,int myPlayerid, int friendPlayerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(myPlayerid);
	streamBuf->_writeInt(friendPlayerid);
}

void crRemoveChatFriendPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crRemoveChatFriendPacket packet;
				crRemoveChatFriendPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf->_readInt());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crRemoveChatFriendPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int myPlayerid = m_streamBuf->_readInt();
		int friendPlayerid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> myPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(myPlayerid));
		if(myPlayerData.valid())
		{
			crNetChatData::crChatFriend *chatFriend = myPlayerData->getNetChatData()->getFriend(friendPlayerid);
			if(chatFriend)
			{
				crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,myPlayerData->getChatDBID());
				if(chatdb)
				{
					ref_ptr<crDataBase> chatSession = chatdb->beginSession();

					ref_ptr<crRemovePlayerFriendLeaveword> removeLeaveword = new crRemovePlayerFriendLeaveword;
					removeLeaveword->buildUpdateSql(chatFriend->getID());
					if(chatSession->executeUpdate(removeLeaveword.get()))
					{
						chatSession->commit();
					}

					ref_ptr<crRemovePlayerFriend> removeChatFriend = new crRemovePlayerFriend;
					removeChatFriend->buildUpdateSql(chatFriend->getID());
					if(chatSession->executeUpdate(removeChatFriend.get()))
					{
						chatSession->commit();
					}

					chatdb->endSession(chatSession.get());
				}
				myPlayerData->getNetChatData()->removeFriend(friendPlayerid);
				//CRCore::notify(CRCore::ALWAYS)<<"crRemoveChatFriendPacket "<<std::endl;
			}
		}
	}
}
/////////////////////////////////////////
//
//crChatPacket
//
/////////////////////////////////////////
crChatPacket::crChatPacket():
crStreamPacket(crChatPacket::ID)
{
}

crChatPacket::crChatPacket( const crChatPacket &p ):
crStreamPacket(crChatPacket::ID,p)
{
}

void crChatPacket::buildRequestPacket(crChatPacket &packet,int recvid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crChatPacket::buildRequestPacket2(crChatPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crChatPacket::buildReplyPacket(crChatPacket &packet,int senderid, int recvid, /*const std::string &time,*/CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	//streamBuf->_writeString(time);
	streamBuf->_writeInt64(time(0));
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crChatPacket::buildRequestPacket(crChatPacket &packet,int senderid, int recvid, unsigned short msg, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(10+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUShort(msg);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crChatPacket::buildRequestPacket2(crChatPacket &packet,int senderid, int recvid, unsigned short msg, /*const std::string &time,*/ CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(18+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeUShort(msg);
	//streamBuf->_writeString(time);
	streamBuf->_writeInt64(time(0));
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crChatPacket::buildReplyPacket2(crChatPacket &packet,CRCore::crStreamBuf *stream)
{
	stream->seekBegin();
	packet.setStreamBuf(stream);
}
void crChatPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crChatPacket packet;
				crChatPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			crNetChatData::crChatFriend *chatFriend = senderPlayerData->getNetChatData()->getFriend(recvid);
			//if(!chatFriend)
			//{//将recvid加为陌生人

			//}
			if(chatFriend)
			{
				//CRCore::Timer_t t = time( 0 ); 
				//char tmp[20];
				//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
				crChatPacket packet;
				crChatPacket::buildReplyPacket(packet,senderid,recvid,/*tmp,*/m_streamBuf.get());
				if(chatFriend->getFriendChatState() > crNetChatData::OffLine)
				{
					ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
					if(recvPlayerData.valid())
					{
						m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
					}
					else
					{
						CRNet::crNetConductor *parentLine = senderPlayerData->getParentServerLineConductor();
						if(parentLine)
						{
							parentLine->getNetManager()->sendPacket("all",packet);
						}
					}
				}
				else
				{//存数据库
					crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
					ref_ptr<crDataBase> globalSession = db->beginSession();

					ref_ptr<crChatDBIDQueryData> queryResult;
					ref_ptr<crChatDBIDQueryData> chatDBIDQueryData = new crChatDBIDQueryData;
					chatDBIDQueryData->buildQuerySql(recvid);

					globalSession->executeQuery(chatDBIDQueryData.get());
					crDataBase::QueryResultVec queryResultVec = globalSession->getQueryResult();
					globalSession->releaseQuery();
					db->endSession(globalSession.get());
					if(!queryResultVec.empty())
					{
						queryResult = dynamic_cast<crChatDBIDQueryData *>(queryResultVec[0].get());
						unsigned short chatdbid = queryResult->getChatDBID();

						crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
						if(chatdb)
						{
							ref_ptr<crDataBase> chatSession = chatdb->beginSession();

							ref_ptr<crPlayerFriendIDQueryData> chatFriendID = new crPlayerFriendIDQueryData;
							chatFriendID->buildQuerySql(recvid,senderid);
							chatSession->executeQuery(chatFriendID.get());
							crDataBase::QueryResultVec chatFriendQueryResultVec = chatSession->getQueryResult();
							chatSession->releaseQuery();
							if(!chatFriendQueryResultVec.empty())
							{
								chatFriendID = dynamic_cast<crPlayerFriendIDQueryData *>(chatFriendQueryResultVec[0].get());
								int chatfriendid = chatFriendID->getID();

								ref_ptr<crInsertPlayerFriendLeaveword> insertLeaverword = new crInsertPlayerFriendLeaveword;
								insertLeaverword->buildUpdateSql(chatfriendid,packet.getStreamBuf()->getBuf(),packet.getStreamBuf()->getBufSize());
								if(chatSession->executeUpdate(insertLeaverword.get()))
								{
									chatSession->commit();
								}
							}
							chatdb->endSession(chatSession.get());
						}
					}
				}
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket"<<std::endl;
			}
		}
	}
	else if(netType == ParentServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{//
			ref_ptr<crParentServerPlayerData> senderPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					m_streamBuf->seekBegin();
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ParentServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(parentServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == ChatServerClient_Parent)
	{//ChatServer->ChatConnectServer->recvid
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			if(senderid == 0)
			{//系统消息
				unsigned short msg = m_streamBuf->_readUShort();
				crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_ChatSystemMsg,msg),MAKEINT64(m_streamBuf.get(),recvPlayerData.get()));
			}
			else
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
        int senderid = m_streamBuf->peekInt();
		if(senderid == 0)
		{//系统消息
			int recvid = m_streamBuf->_readInt();
			unsigned short msg = m_streamBuf->_readUShort();
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_ChatSystemMsg,msg),MAKEINT64(m_streamBuf.get(),NULL));
		}
		else
		{
			crGlobalHandle::getInstance()->doEvent(WCH_FriendChat,MAKEINT64(m_streamBuf.get(),NULL));
		}
	}
}
/////////////////////////////////////////
//
//crGroupChatPacket
//
/////////////////////////////////////////
crGroupChatPacket::crGroupChatPacket():
crStreamPacket(crGroupChatPacket::ID)
{
}

crGroupChatPacket::crGroupChatPacket( const crGroupChatPacket &p ):
crStreamPacket(crGroupChatPacket::ID,p)
{
}

void crGroupChatPacket::buildRequestPacket(crGroupChatPacket &packet,int playergroupid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playergroupid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crGroupChatPacket::buildRequestPacket2(crGroupChatPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crGroupChatPacket::buildReplyPacket(crGroupChatPacket &packet,int senderid, int recvid,int groupid,/*const std::string &time,*/CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(20+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
	streamBuf->_writeInt64(time(0));
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crGroupChatPacket::buildReplyPacket2(crGroupChatPacket &packet,CRCore::crStreamBuf *stream)
{
	stream->seekBegin();
	packet.setStreamBuf(stream);
}
void crGroupChatPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crGroupChatPacket packet;
				crGroupChatPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int playergroupid = m_streamBuf->_readInt();
		int recvid;
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			crNetChatData::crChatGroup *chatGroup = senderPlayerData->getNetChatData()->getChatGroup(playergroupid);
			if(chatGroup)
			{
				//CRCore::Timer_t t = time( 0 ); 
				//char tmp[20];
				//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
				crGroupChatPacket packet;
				crNetChatData::crChatGroupMember *chatMember;
				chatGroup->lockMemberMap();
				crNetChatData::crChatGroup::ChatMemberMap &chatMemberMap = chatGroup->getMemberMap();
				for( crNetChatData::crChatGroup::ChatMemberMap::iterator itr = chatMemberMap.begin();
					 itr != chatMemberMap.end();
					 ++itr )
				{
					chatMember = itr->second.get();
					recvid = chatMember->getPlayerID();
					crGroupChatPacket::buildReplyPacket(packet,senderid,recvid,chatGroup->getGroupID(),/*tmp,*/m_streamBuf.get());
					if(chatMember->getMemberChatState() > crNetChatData::OffLine)
					{
						ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
						if(recvPlayerData.valid())
						{
							m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
						}
						else
						{
							CRNet::crNetConductor *parentLine = senderPlayerData->getParentServerLineConductor();
							if(parentLine)
							{
								parentLine->getNetManager()->sendPacket("all",packet);
							}
						}
					}
					else
					{//存数据库
						crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
						ref_ptr<crDataBase> globalSession = db->beginSession();

						ref_ptr<crChatDBIDQueryData> queryResult;
						ref_ptr<crChatDBIDQueryData> chatDBIDQueryData = new crChatDBIDQueryData;
						chatDBIDQueryData->buildQuerySql(recvid);

						globalSession->executeQuery(chatDBIDQueryData.get());
						crDataBase::QueryResultVec queryResultVec = globalSession->getQueryResult();
						globalSession->releaseQuery();
						db->endSession(globalSession.get());
						if(!queryResultVec.empty())
						{
							queryResult = dynamic_cast<crChatDBIDQueryData *>(queryResultVec[0].get());
							unsigned short chatdbid = queryResult->getChatDBID();

							crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
							if(chatdb)
							{
								ref_ptr<crDataBase> chatSession = chatdb->beginSession();

								ref_ptr<crPlayerGroupIDQueryData> chatGroupID = new crPlayerGroupIDQueryData;
								chatGroupID->buildQuerySql(recvid,chatGroup->getGroupID());
								chatSession->executeQuery(chatGroupID.get());
								crDataBase::QueryResultVec chatGroupQueryResultVec = chatSession->getQueryResult();
								chatSession->releaseQuery();
								if(!chatGroupQueryResultVec.empty())
								{
									chatGroupID = dynamic_cast<crPlayerGroupIDQueryData *>(queryResultVec[0].get());
									int playergroupid = chatGroupID->getID();

									ref_ptr<crInsertPlayerGroupLeaveword> insertLeaverword = new crInsertPlayerGroupLeaveword;
									insertLeaverword->buildUpdateSql(playergroupid,packet.getStreamBuf()->getBuf(),packet.getStreamBuf()->getBufSize());
									if(chatSession->executeUpdate(insertLeaverword.get()))
									{
										chatSession->commit();
									}
								}

								chatdb->endSession(chatSession.get());
							}
						}
					}
				}
				chatGroup->unlockMemberMap();
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket"<<std::endl;
			}
		}
	}
	else if(netType == ParentServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{//
			ref_ptr<crParentServerPlayerData> senderPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					m_streamBuf->seekBegin();
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ParentServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(parentServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == ChatServerClient_Parent)
	{//ChatServer->ChatConnectServer->recvid
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt(); 
		crGlobalHandle::getInstance()->doEvent(WCH_GroupChat,MAKEINT64(groupid,m_streamBuf.get()));
		//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket"<<std::endl;
	}
}
/////////////////////////////////////////
//
//crApplyJoinGroupPacket
//
/////////////////////////////////////////
crApplyJoinGroupPacket::crApplyJoinGroupPacket():
crStreamPacket(crApplyJoinGroupPacket::ID)
{
}

crApplyJoinGroupPacket::crApplyJoinGroupPacket( const crApplyJoinGroupPacket &p ):
crStreamPacket(crApplyJoinGroupPacket::ID,p)
{
}

void crApplyJoinGroupPacket::buildRequestPacket(crApplyJoinGroupPacket &packet,int groupid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(groupid);
}
void crApplyJoinGroupPacket::buildRequestPacket2(crApplyJoinGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crApplyJoinGroupPacket::buildReplyPacket(crApplyJoinGroupPacket &packet,int senderid, int recvid,int groupid/*,const std::string &time*/)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(20);
	packet.setStreamBuf(streamBuf);
    streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
    streamBuf->_writeInt(groupid);
    streamBuf->_writeInt64(time(0));
}

void crApplyJoinGroupPacket::sendToGroupManager(int senderid,int recvid,int groupid,/*const std::string &time,*/ CRDataBase::crDataBase *global_query,crChatServerPlayerData *senderPlayerData)
{
	ref_ptr<crChatServerPlayerData> creatorPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
	if(creatorPlayerData.valid())
	{
		crApplyJoinGroupPacket packet;
		crApplyJoinGroupPacket::buildReplyPacket(packet,senderid,recvid,groupid/*,time*/);
		m_netConductor->getNetManager()->sendPacket(creatorPlayerData->getPlayerConnectServerAddress(),packet);
	}
	else
	{
		ref_ptr<crChatStateQueryData> chatStateQuery = new crChatStateQueryData;
		chatStateQuery->buildQuerySql(recvid);

		global_query->executeQuery(chatStateQuery.get());
		crDataBase::QueryResultVec chatStateQueryResultVec = global_query->getQueryResult();
		global_query->releaseQuery();
		if(!chatStateQueryResultVec.empty())
		{
			chatStateQuery = dynamic_cast<crChatStateQueryData *>(chatStateQueryResultVec[0].get());
			char chatstate = chatStateQuery->getChatState();

			if(chatstate>0)
			{
				crApplyJoinGroupPacket packet;
				crApplyJoinGroupPacket::buildReplyPacket(packet,senderid,recvid,groupid/*,time*/);
				senderPlayerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",packet);
			}
			else
			{//存数据库
				ref_ptr<crChatDBIDQueryData> chatDBIDQueryResult;
				ref_ptr<crChatDBIDQueryData> chatDBIDQueryData = new crChatDBIDQueryData;
				chatDBIDQueryData->buildQuerySql(recvid);

				global_query->executeQuery(chatDBIDQueryData.get());
				crDataBase::QueryResultVec chatDBIDQueryResultVec = global_query->getQueryResult();
				global_query->releaseQuery();
				if(!chatDBIDQueryResultVec.empty())
				{
					chatDBIDQueryResult = dynamic_cast<crChatDBIDQueryData *>(chatDBIDQueryResultVec[0].get());
					unsigned short chatdbid = chatDBIDQueryResult->getChatDBID();

					crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
					if(chatdb)
					{
						ref_ptr<crDataBase> chatSession = chatdb->beginSession();

						ref_ptr<crPlayerGroupIDQueryData> chatGroupID = new crPlayerGroupIDQueryData;
						chatGroupID->buildQuerySql(recvid,groupid);
						chatSession->executeQuery(chatGroupID.get());
						crDataBase::QueryResultVec chatGroupQueryResultVec = chatSession->getQueryResult();
						chatSession->releaseQuery();
						if(!chatGroupQueryResultVec.empty())
						{
							chatGroupID = dynamic_cast<crPlayerGroupIDQueryData *>(chatGroupQueryResultVec[0].get());
							int playergroupid = chatGroupID->getID();

							ref_ptr<crInsertPlayerGroupLeaveword> insertLeaverword = new crInsertPlayerGroupLeaveword;
							ref_ptr<crStreamBuf> stream;
							stream->createBuf(64);
							stream->_writeInt(senderid);
							stream->_writeInt(recvid);
							stream->_writeInt(groupid);
							stream->_writeInt64(time(0));
							std::string str = "[System] ApplyJoinGroup";
							stream->_writeString(str);

							insertLeaverword->buildUpdateSql(playergroupid,stream->getBuf(),stream->getBufSize());
							if(chatSession->executeUpdate(insertLeaverword.get()))
							{
								chatSession->commit();
							}
						}

						chatdb->endSession(chatSession.get());
					}
				}
			}
		}
	}
}

void crApplyJoinGroupPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crApplyJoinGroupPacket packet;
				crApplyJoinGroupPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crApplyJoinGroupPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			ref_ptr<crChatGroupTableByIDQueryData> queryResult;
			int creator = 0;
			int manager1 = 0;
			int manager2 = 0;
			int manager3 = 0;
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase>	globalSession = db->beginSession();

			ref_ptr<crChatGroupTableByIDQueryData> groupTableByIDQuery = new crChatGroupTableByIDQueryData;
			groupTableByIDQuery->buildQuerySql(groupid);

			globalSession->executeQuery(groupTableByIDQuery.get());
			crDataBase::QueryResultVec &queryResultVec = globalSession->getQueryResult();
			if(!queryResultVec.empty())
			{
				queryResult = dynamic_cast<crChatGroupTableByIDQueryData *>(queryResultVec[0].get());
				creator = queryResult->getCreator();
				manager1 = queryResult->getManager1();
				manager2 = queryResult->getManager2();
				manager3 = queryResult->getManager3();
			}
			globalSession->releaseQuery();
			
			//CRCore::Timer_t t = time( 0 ); 
			//char tmp[20];
			//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
			//std::string time = tmp;
			if(creator>0)
			{
				sendToGroupManager(creator,senderid,groupid,/*time,*/globalSession.get(),senderPlayerData.get());
			}
			if(manager1>0)
			{
				sendToGroupManager(manager1,senderid,groupid,/*time,*/globalSession.get(),senderPlayerData.get());
			}
			if(manager2>0)
			{
				sendToGroupManager(manager2,senderid,groupid,/*time,*/globalSession.get(),senderPlayerData.get());
			}
			if(manager3>0)
			{
				sendToGroupManager(manager3,senderid,groupid,/*time,*/globalSession.get(),senderPlayerData.get());
			}
			db->endSession(globalSession.get());
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crChatPacket"<<std::endl;
			}
		}
	}
	else if(netType == ParentServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{//
			ref_ptr<crParentServerPlayerData> senderPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					m_streamBuf->seekBegin();
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ParentServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(parentServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == ChatServerClient_Parent)
	{//ChatServer->ChatConnectServer->recvid
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt(); 
		crGlobalHandle::getInstance()->doEvent(WCH_ApplyJoinGroup,MAKEINT64(groupid,m_streamBuf.get()));
		//CRCore::notify(CRCore::ALWAYS)<<"crApplyJoinGroupPacket"<<std::endl;
	}
}

/////////////////////////////////////////
//
//crAcceptJoinGroupPacket
//
/////////////////////////////////////////
crAcceptJoinGroupPacket::crAcceptJoinGroupPacket():
crStreamPacket(crAcceptJoinGroupPacket::ID)
{
}

crAcceptJoinGroupPacket::crAcceptJoinGroupPacket( const crAcceptJoinGroupPacket &p ):
crStreamPacket(crAcceptJoinGroupPacket::ID,p)
{
}

void crAcceptJoinGroupPacket::buildRequestPacket(crAcceptJoinGroupPacket &packet,int recvid,int groupid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
}
void crAcceptJoinGroupPacket::buildRequestPacket2(crAcceptJoinGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crAcceptJoinGroupPacket::buildReplyPacket(crAcceptJoinGroupPacket &packet,int senderid, int recvid,int groupid/*,const std::string &time*/)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(20);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
	streamBuf->_writeInt64(time(0));
}

void crAcceptJoinGroupPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crAcceptJoinGroupPacket packet;
				crAcceptJoinGroupPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crAcceptJoinGroupPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();

			std::string memberName;
			unsigned short chatdbid = 0;
			char chatstate = 0;
			//CRCore::Timer_t t = time( 0 ); 
			//char tmp[20];
			//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
			//std::string time = tmp;
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
                memberName = recvPlayerData->getNickName();
				chatdbid = recvPlayerData->getChatDBID();

				crAcceptJoinGroupPacket packet;
				crAcceptJoinGroupPacket::buildReplyPacket(packet,senderid,recvid,groupid/*,time*/);
				m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
				chatstate = 1;
			}
			else
			{//查询数据库
				ref_ptr<crJoinGroupQueryData> joinGroupQuery = new crJoinGroupQueryData;
				joinGroupQuery->buildQuerySql(recvid);

				globalSession->executeQuery(joinGroupQuery.get());
				crDataBase::QueryResultVec &joinGroupQueryResultVec = globalSession->getQueryResult();
				if(!joinGroupQueryResultVec.empty())
				{
					joinGroupQuery = dynamic_cast<crJoinGroupQueryData *>(joinGroupQueryResultVec[0].get());
					chatstate = joinGroupQuery->getChatState();
					chatdbid = joinGroupQuery->getChatDBID();
					memberName = joinGroupQuery->getNickName();
				}
				globalSession->releaseQuery();
				
				if(chatstate>0)
				{
					crAcceptJoinGroupPacket packet;
					crAcceptJoinGroupPacket::buildReplyPacket(packet,senderid,recvid,groupid/*,time*/);
					senderPlayerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",packet);
				}
			}

			ref_ptr<crInsertGroupMember> insertGroupMember = new crInsertGroupMember;
			insertGroupMember->buildUpdateSql(groupid,recvid,memberName);
			if(globalSession->executeUpdate(insertGroupMember.get()))
			{
				globalSession->commit();
			}
			db->endSession(globalSession.get());

			crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
			if(chatdb)
			{
				ref_ptr<crDataBase> chatSession = chatdb->beginSession();
				int playergroupid = 0;
				ref_ptr<crInsertPlayerGroup> insertPlayerGroup = new crInsertPlayerGroup;
				insertPlayerGroup->buildUpdateSql(recvid,groupid);
				if(chatSession->executeUpdate(insertPlayerGroup.get()))
				{
					playergroupid = chatSession->getInsertID();
					chatSession->commit();
				}

				if(playergroupid>0 && chatstate==0)
				{//存留言
					ref_ptr<crInsertPlayerGroupLeaveword> insertLeaverword = new crInsertPlayerGroupLeaveword;
					ref_ptr<crStreamBuf> stream;
					stream->createBuf(64);
					stream->_writeInt(senderid);
					stream->_writeInt(recvid);
					stream->_writeInt(groupid);
					stream->_writeInt64(time(0));
					std::string str = "[System] AcceptJoinGroup";
					stream->_writeString(str);

					insertLeaverword->buildUpdateSql(playergroupid,stream->getBuf(),stream->getBufSize());
					if(chatSession->executeUpdate(insertLeaverword.get()))
					{
						chatSession->commit();
					}
				}

				chatdb->endSession(chatSession.get());
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crAcceptJoinGroupPacket"<<std::endl;
			}
		}
	}
	else if(netType == ParentServer)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(recvid));
		if(recvPlayerData.valid())
		{
			m_streamBuf->seekBegin();
			m_netConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{//
			ref_ptr<crParentServerPlayerData> senderPlayerData = dynamic_cast<crParentServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
			if(senderPlayerData.valid())
			{
				crNetConductor *parentConductor = senderPlayerData->getParentServerLineConductor();
				if(parentConductor)
				{
					m_streamBuf->seekBegin();
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
	}
	else if(netType == ParentServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *parentServerConductor = crNetContainer::getInstance()->getNetConductor(ParentServer);
		if(parentServerConductor)
		{
			ref_ptr<crParentServerPlayerData> recvPlayerData = dynamic_cast<crParentServerPlayerData *>(parentServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				parentServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == ChatServerClient_Parent)
	{//ChatServer->ChatConnectServer->recvid
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt(); 
		crGlobalHandle::getInstance()->doEvent(WCH_AcceptJoinGroup,MAKEINT64(groupid,m_streamBuf.get()));
		//CRCore::notify(CRCore::ALWAYS)<<"crAcceptJoinGroupPacket"<<std::endl;
	}
}

/////////////////////////////////////////
//
//crNewMemberComingPacket
//
/////////////////////////////////////////
crNewMemberComingPacket::crNewMemberComingPacket():
crStreamPacket(crNewMemberComingPacket::ID)
{
}

crNewMemberComingPacket::crNewMemberComingPacket( const crNewMemberComingPacket &p ):
crStreamPacket(crNewMemberComingPacket::ID,p)
{
}

void crNewMemberComingPacket::buildRequestPacket(crNewMemberComingPacket &packet,int groupid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(groupid);
}
void crNewMemberComingPacket::buildRequestPacket2(crNewMemberComingPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crNewMemberComingPacket::buildReplyPacket(crNewMemberComingPacket &packet,int senderid, int recvid,int groupid,const std::string &time)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(36);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
	streamBuf->_writeString(time);
}

void crNewMemberComingPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crNewMemberComingPacket packet;
				crNewMemberComingPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crNewMemberComingPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		int senderid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			crNetChatData::crChatGroup *chatGroup = senderPlayerData->getNetChatData()->findChatGroup(groupid);
			if(!chatGroup)
			{//loadFrom DB
				crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,senderPlayerData->getChatDBID());
				if(chatdb)
				{	
				    ref_ptr<crDataBase> chatSession = chatdb->beginSession();
					int playergroupid = 0;
					ref_ptr<crPlayerGroupIDQueryData> playerGroupIDQuery = new crPlayerGroupIDQueryData;
					playerGroupIDQuery->buildQuerySql(senderid,groupid);

					chatSession->executeQuery(playerGroupIDQuery.get());
					crDataBase::QueryResultVec &playerGroupIDQueryResultVec = chatSession->getQueryResult();
					if(!playerGroupIDQueryResultVec.empty())
					{
						playerGroupIDQuery = dynamic_cast<crPlayerGroupIDQueryData *>(playerGroupIDQueryResultVec[0].get());
						playergroupid = playerGroupIDQuery->getID();
					}
					chatSession->releaseQuery();
					chatdb->endSession(chatSession.get());

					if(playergroupid>0)
					{
                        chatGroup = new crNetChatData::crChatGroup;
						chatGroup->setID(playergroupid);
						chatGroup->setGroupID(groupid);
						senderPlayerData->getNetChatData()->insertChatGroup(chatGroup);

						crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
						ref_ptr<crDataBase> globalSession = db->beginSession();

						ref_ptr<crChatGroupTableByIDQueryData> groupTableByIDQuery = new crChatGroupTableByIDQueryData;
						groupTableByIDQuery->buildQuerySql(groupid);

						globalSession->executeQuery(groupTableByIDQuery.get());
						crDataBase::QueryResultVec &groupTableByIDQueryResultVec = globalSession->getQueryResult();
						if(!groupTableByIDQueryResultVec.empty())
						{
							groupTableByIDQuery = dynamic_cast<crChatGroupTableByIDQueryData *>(groupTableByIDQueryResultVec[0].get());
							chatGroup->setGroupName(groupTableByIDQuery->getGroupName());
							chatGroup->setCreator(groupTableByIDQuery->getCreator());
							chatGroup->setManager1(groupTableByIDQuery->getManager1());
							chatGroup->setManager2(groupTableByIDQuery->getManager2());
							chatGroup->setManager3(groupTableByIDQuery->getManager3());
						}
						globalSession->releaseQuery();
						//将chatGroup信息发回客户端
                        crAddChatGroupPacket addChatGroupPacket;
						crAddChatGroupPacket::buildRequestPacket(addChatGroupPacket,senderid,chatGroup);
						m_netConductor->getNetManager()->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),addChatGroupPacket);

						crAddGroupMemberPacket addGroupMemberPacket;
						ref_ptr<crChatServerPlayerData> loginedChatPlayer;
						ref_ptr<crGroupMemberTableQueryData> groupMemberTabQuery;
						ref_ptr<crNetChatData::crChatGroupMember> chatGroupMember,chatGroupMember2;

						groupMemberTabQuery = new crGroupMemberTableQueryData;
						groupMemberTabQuery->buildQuerySql(groupid);

						globalSession->executeQuery(groupMemberTabQuery.get());
						crDataBase::QueryResultVec groupMemberTabQueryResultVec = globalSession->getQueryResult();
						globalSession->releaseQuery();

						if(!groupMemberTabQueryResultVec.empty())
						{
							std::vector< ref_ptr<crStreamBuf> > BufVec;
							int maxNeedSize = 101;
							int count = 0;
							ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
							streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);
							char chatstate = 0;
							int senderMemberID = 0;
							for( crDataBase::QueryResultVec::iterator itr = groupMemberTabQueryResultVec.begin();
								itr != groupMemberTabQueryResultVec.end();
								++itr )
							{
								groupMemberTabQuery = dynamic_cast<crGroupMemberTableQueryData *>(itr->get());
								if(groupMemberTabQuery->getPlayerID() == senderid)
									senderMemberID = groupMemberTabQuery->getID();
							}
							for( crDataBase::QueryResultVec::iterator itr = groupMemberTabQueryResultVec.begin();
								itr != groupMemberTabQueryResultVec.end();
								++itr )
							{
								groupMemberTabQuery = dynamic_cast<crGroupMemberTableQueryData *>(itr->get());

								chatGroupMember = new crNetChatData::crChatGroupMember;
								chatGroupMember->setMemberID(groupMemberTabQuery->getID());
								chatGroupMember->setPlayerID(groupMemberTabQuery->getPlayerID());
								chatGroupMember->setMemberName(groupMemberTabQuery->getMemberName());
								chatGroupMember->setMemberRemark(groupMemberTabQuery->getMemberRemark());

								if(senderid != chatGroupMember->getPlayerID())
								{
									loginedChatPlayer = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(chatGroupMember->getPlayerID()));
									if(loginedChatPlayer.valid())
									{
										chatstate = loginedChatPlayer->getNetChatData()->getMyChatState();
										chatGroupMember->setMemberChatState(loginedChatPlayer->getNetChatData()->getMyChatState());
										chatGroupMember->setNickName(loginedChatPlayer->getNickName());

										crAddGroupMemberPacket::buildRequestPacket(addGroupMemberPacket,senderid,chatGroupMember->getPlayerID(),groupid,senderMemberID,crNetChatData::OnLine,senderPlayerData->getNickName());
										m_netConductor->getNetManager()->sendPacket(loginedChatPlayer->getPlayerConnectServerAddress(),addGroupMemberPacket);

										chatGroupMember2 = new crNetChatData::crChatGroupMember;
										chatGroupMember2->setMemberID(senderMemberID);
										chatGroupMember2->setPlayerID(senderid);
										chatGroupMember2->setMemberChatState(crNetChatData::OnLine);
										chatGroupMember2->setNickName(senderPlayerData->getNickName());
										crNetChatData::crChatGroup *chatGroup = loginedChatPlayer->getNetChatData()->findChatGroup(groupid);
										if(chatGroup)
											chatGroup->insertMember(chatGroupMember2.get());
									}
									else
									{//从数据库检查是否在线
										ref_ptr<crChatStateNickNameQueryData> chatstateNickNameQuery = new crChatStateNickNameQueryData;
										chatstateNickNameQuery->buildQuerySql(chatGroupMember->getPlayerID());

										globalSession->executeQuery(chatstateNickNameQuery.get());
										crDataBase::QueryResultVec &chatstateNickNameQueryResultVec = globalSession->getQueryResult();
										if(!chatstateNickNameQueryResultVec.empty())
										{
											chatstateNickNameQuery = dynamic_cast<crChatStateNickNameQueryData *>(chatstateNickNameQueryResultVec[0].get());
											chatstate = chatstateNickNameQuery->getChatState();
											const std::string& nickName = chatstateNickNameQuery->getNickName();
											chatGroupMember->setNickName(nickName);
										}
										globalSession->releaseQuery();

										chatGroupMember->setMemberChatState(chatstate);
										if(chatstate>0 && senderPlayerData->getParentServerLineConductor())
										{//如在线
											crAddGroupMemberPacket::buildRequestPacket(addGroupMemberPacket,senderid,chatGroupMember->getPlayerID(),groupid,senderMemberID,crNetChatData::OnLine,senderPlayerData->getNickName());
											senderPlayerData->getParentServerLineConductor()->getNetManager()->sendPacket("all",addGroupMemberPacket);
										}
									}
								}
								chatGroup->insertMember(chatGroupMember.get());

								if(streamBuf->getRemainCapacity()>maxNeedSize)
								{
									streamBuf->_writeInt(chatGroupMember->getMemberID());//4
									streamBuf->_writeInt(chatGroupMember->getPlayerID());//20+4
									streamBuf->_writeString(chatGroupMember->getMemberName());//20+4
									streamBuf->_writeString(chatGroupMember->getMemberRemark());//20+4
									streamBuf->_writeChar(chatGroupMember->getMemberChatState());//1
									streamBuf->_writeString(chatGroupMember->getNickName());//20+4
								}
								else
								{
									count += streamBuf->getBufSize();
									BufVec.push_back(streamBuf.get());
									streamBuf = new crStreamBuf;
									streamBuf->createBuf(MTUBUFFERSIZE);//MAXBUFFERSIZE-12);

									streamBuf->_writeInt(chatGroupMember->getMemberID());//4
									streamBuf->_writeInt(chatGroupMember->getPlayerID());//20+4
									streamBuf->_writeString(chatGroupMember->getMemberName());//20+4
									streamBuf->_writeString(chatGroupMember->getMemberRemark());//20+4
									streamBuf->_writeChar(chatGroupMember->getMemberChatState());//1
									streamBuf->_writeString(chatGroupMember->getNickName());//20+4
								}
							}
							if(streamBuf->getBufSize()>0)
							{
								count += streamBuf->getBufSize();
								BufVec.push_back(streamBuf.get());
							}
							if(count>0)
							{
								int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
								crChatGroupMemberListPacket chatPlayerDataPacket;
								crChatGroupMemberListPacket::buildRequestPacket(chatPlayerDataPacket,senderid,streamid,count);
								m_netConductor->getNetManager()->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),chatPlayerDataPacket);
								crDataStreamPacket streamPacket;
								for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
									itr != BufVec.end();
									++itr )
								{
									crDataStreamPacket::buildRequestPacket(streamPacket,senderid,streamid,crChatGroupMemberListPacket::ID,itr->get());
									m_netConductor->getNetManager()->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),streamPacket);
								}
								crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_out());
							}
						}
                        db->endSession(globalSession.get());
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crAddChatGroupPacket
//crNewMemberComingPacket返回
/////////////////////////////////////////
crAddChatGroupPacket::crAddChatGroupPacket():
crStreamPacket(crAddChatGroupPacket::ID)
{
}

crAddChatGroupPacket::crAddChatGroupPacket( const crAddChatGroupPacket &p ):
crStreamPacket(crAddChatGroupPacket::ID,p)
{
}

void crAddChatGroupPacket::buildRequestPacket(crAddChatGroupPacket &packet,int recvid, crNetChatData::crChatGroup *chatGroup)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	//streamBuf->_writeInt(count);
}

void crAddChatGroupPacket::buildRequestPacket2(crAddChatGroupPacket &packet,int count)
{//
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(count);
}

void crAddChatGroupPacket::parsePacket(const std::string &sender)
{//id name
	//if(!m_netConductor) return;
	//int netType = m_netConductor->getNetType();
	//if(netType == ChatConnectServerClient_Chat)
	//{//转发给客户端
	//	int recvid = m_streamBuf->_readInt();
	//	crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
	//	if(connectServerConductor)
	//	{
	//		crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
	//		crChatConnectServerPlayerData *playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
	//		if(playerData)
	//		{
	//			crAddChatGroupPacket packet;
	//			crAddChatGroupPacket::buildRequestPacket2(packet,m_streamBuf->_readInt());
	//			connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//			CRCore::notify(CRCore::ALWAYS)<<"crAddChatGroupPacket"<<std::endl;
	//		}
	//	}
	//}
	//else if(netType == GameClient_Chat)
	//{//id name
	//	int count = m_streamBuf->_readInt();
	//	crGlobalHandle::getInstance()->doEvent(WCH_AddChatGroup,MAKEINT64(count,NULL));
	//	CRCore::notify(CRCore::ALWAYS)<<"crAddChatGroupPacket count = "<<count<<std::endl;
	//}
}
/////////////////////////////////////////
//
//crAddGroupMemberPacket
//
/////////////////////////////////////////
crAddGroupMemberPacket::crAddGroupMemberPacket():
crStreamPacket(crAddGroupMemberPacket::ID)
{
}

crAddGroupMemberPacket::crAddGroupMemberPacket( const crAddGroupMemberPacket &p ):
crStreamPacket(crAddGroupMemberPacket::ID,p)
{
}

void crAddGroupMemberPacket::buildRequestPacket(crAddGroupMemberPacket &packet,int senderid, int recvid, int groupid,int senderMemberID,char chatState,const std::string &nickName)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(41);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
	streamBuf->_writeInt(senderMemberID);
	streamBuf->_writeChar(chatState);
	streamBuf->_writeString(nickName);//20+4
}

void crAddGroupMemberPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ParentServer)
	{
		int sendid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		m_streamBuf->seekBegin();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
		if(playerData.valid())
		{
			m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{
			ref_ptr<crParentServerPlayerData> playerData = dynamic_cast<crParentServerPlayerData *>(netDataManager->getPlayerData(sendid));
			if(playerData.valid())
			{
				crNetConductor *parentConductor = playerData->getParentServerLineConductor();
				if(parentConductor)
				{
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crChatStateChangePacket"<<std::endl;
	}
	else if(netType == ChatServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			crNetDataManager *netDataManager = chatServerConductor->getNetDataManager();
			ref_ptr<crChatServerPlayerData> playerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				int groupid = m_streamBuf->_readInt();
                int senderMemberID = m_streamBuf->_readInt();
				char chatState = m_streamBuf->_readChar();
				std::string nickName = m_streamBuf->_readString();

				ref_ptr<crNetChatData::crChatGroupMember> chatMember = new crNetChatData::crChatGroupMember;
				chatMember->setMemberID(senderMemberID);
				chatMember->setPlayerID(senderid);
				chatMember->setMemberChatState(chatState);
				chatMember->setNickName(nickName);
				crNetChatData::crChatGroup *chatGroup = playerData->getNetChatData()->findChatGroup(groupid);
				if(chatGroup)
					chatGroup->insertMember(chatMember.get());

				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);

				//CRCore::notify(CRCore::ALWAYS)<<"crAddGroupMemberPacket"<<std::endl;
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		int senderMemberID = m_streamBuf->_readInt();
		char chatState = m_streamBuf->_readChar();
		std::string nickName = m_streamBuf->_readString();

		crNetChatData *myChatData = crMyPlayerData::getInstance()->getMyChatData();
		if(myChatData)
		{
			ref_ptr<crNetChatData::crChatGroupMember> chatMember = new crNetChatData::crChatGroupMember;
			chatMember->setMemberID(senderMemberID);
			chatMember->setPlayerID(senderid);
			chatMember->setMemberChatState(chatState);
			chatMember->setNickName(nickName);
			crNetChatData::crChatGroup *chatGroup = myChatData->findChatGroup(groupid);
			if(chatGroup)
				chatGroup->insertMember(chatMember.get());

			crGlobalHandle::getInstance()->doEvent(WCH_AddGroupMember,MAKEINT64(chatMember.get(),NULL));
			//CRCore::notify(CRCore::ALWAYS)<<"crAddGroupMemberPacket"<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crExitGroupPacket
//
/////////////////////////////////////////
crExitGroupPacket::crExitGroupPacket():
crStreamPacket(crExitGroupPacket::ID)
{
}

crExitGroupPacket::crExitGroupPacket( const crExitGroupPacket &p ):
crStreamPacket(crExitGroupPacket::ID,p)
{
}

void crExitGroupPacket::buildRequestPacket(crExitGroupPacket &packet,int groupid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(groupid);
}
void crExitGroupPacket::buildRequestPacket2(crExitGroupPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crExitGroupPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crExitGroupPacket packet;
				crExitGroupPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crExitGroupPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		int senderid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(senderid));
		if(senderPlayerData.valid())
		{
			crNetChatData::crChatGroup *chatGroup = senderPlayerData->getNetChatData()->findChatGroup(groupid);
			if(chatGroup)
			{//clear db
				crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,senderPlayerData->getChatDBID());
				if(chatdb)
				{
					ref_ptr<crDataBase> chatSession = chatdb->beginSession();
					ref_ptr<crRemovePlayerGroupLeaveword> removeLeaveword = new crRemovePlayerGroupLeaveword;
					removeLeaveword->buildUpdateSql(chatGroup->getID());
					if(chatSession->executeUpdate(removeLeaveword.get()))
					{
						chatSession->commit();
					}

					ref_ptr<crRemovePlayerGroup> removeChatGroup = new crRemovePlayerGroup;
					removeChatGroup->buildUpdateSql(chatGroup->getID());
					if(chatSession->executeUpdate(removeChatGroup.get()))
					{
						chatSession->commit();
					}
					chatdb->endSession(chatSession.get());
				}

				crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
				if(db)
				{
					ref_ptr<crDataBase> globalSession = db->beginSession();
					ref_ptr<crRemoveGroupMember> removeGroupMember = new crRemoveGroupMember;
					removeGroupMember->buildUpdateSql(groupid,senderid);
					if(globalSession->executeUpdate(removeGroupMember.get()))
					{
						globalSession->commit();
					}
					db->endSession(globalSession.get());
				}

				//通知其他在线成员
				chatGroup->lockMemberMap();
				crRemoveGroupMemberPacket removeGroupMemberPacket;
				ref_ptr<crChatServerPlayerData> memberPlayerData;
				CRNet::crNetConductor *parentServer = senderPlayerData->getParentServerLineConductor();
				crNetChatData::crChatGroup::ChatMemberMap &chatMemberMap = chatGroup->getMemberMap();
				for( crNetChatData::crChatGroup::ChatMemberMap::iterator itr = chatMemberMap.begin();
					 itr != chatMemberMap.end();
					 ++itr )
				{
					if(itr->second->getMemberChatState()>0)
					{//在线
						crRemoveGroupMemberPacket::buildRequestPacket(removeGroupMemberPacket,senderid,itr->second->getPlayerID(),groupid);
                        memberPlayerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(itr->second->getPlayerID()));
						if(memberPlayerData.valid())
						{
                            memberPlayerData->getNetChatData()->findChatGroup(groupid)->removeMember(senderid);
                            m_netConductor->getNetManager()->sendPacket(memberPlayerData->getPlayerConnectServerAddress(),removeGroupMemberPacket);
						}
						else if(parentServer)
						{
							parentServer->getNetManager()->sendPacket("all",removeGroupMemberPacket);
						}
					}
				}
				chatGroup->unlockMemberMap();

				senderPlayerData->getNetChatData()->removeChatGroup(chatGroup->getID());
			}
		}
	}
}
/////////////////////////////////////////
//
//crRemoveGroupMemberPacket
//
/////////////////////////////////////////
crRemoveGroupMemberPacket::crRemoveGroupMemberPacket():
crStreamPacket(crRemoveGroupMemberPacket::ID)
{
}

crRemoveGroupMemberPacket::crRemoveGroupMemberPacket( const crRemoveGroupMemberPacket &p ):
crStreamPacket(crRemoveGroupMemberPacket::ID,p)
{
}

void crRemoveGroupMemberPacket::buildRequestPacket(crRemoveGroupMemberPacket &packet,int senderid, int recvid, int groupid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(groupid);
}

void crRemoveGroupMemberPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ParentServer)
	{
		int sendid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		m_streamBuf->seekBegin();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crServerPlayerData> playerData = dynamic_cast<crServerPlayerData *>(netDataManager->getPlayerData(recvid));
		if(playerData.valid())
		{
			m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),*this);
		}
		else
		{
			ref_ptr<crParentServerPlayerData> playerData = dynamic_cast<crParentServerPlayerData *>(netDataManager->getPlayerData(sendid));
			if(playerData.valid())
			{
				crNetConductor *parentConductor = playerData->getParentServerLineConductor();
				if(parentConductor)
				{
					parentConductor->getNetManager()->sendPacket("all",*this);
				}
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crRemoveGroupMemberPacket"<<std::endl;
	}
	else if(netType == ChatServerClient_Parent)
	{
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			crNetDataManager *netDataManager = chatServerConductor->getNetDataManager();
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				int groupid = m_streamBuf->_readInt();
				crNetChatData::crChatGroup *chatGroup = recvPlayerData->getNetChatData()->findChatGroup(groupid);
				if(chatGroup)
				{
					chatGroup->removeMember(senderid);
				}
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crRemoveGroupMemberPacket"<<std::endl;
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{//转发给客户端
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		crNetConductor *chatServerConductor = crNetContainer::getInstance()->getNetConductor(ChatServer);
		if(chatServerConductor)
		{
			ref_ptr<crChatServerPlayerData> recvPlayerData = dynamic_cast<crChatServerPlayerData *>(chatServerConductor->getNetDataManager()->getPlayerData(recvid));
			if(recvPlayerData.valid())
			{
				m_streamBuf->seekBegin();
				chatServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),*this);
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int senderid = m_streamBuf->_readInt();
		int recvid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		int senderMemberID = m_streamBuf->_readInt();
		char chatState = m_streamBuf->_readChar();
		std::string nickName = m_streamBuf->_readString();

		crNetChatData *myChatData = crMyPlayerData::getInstance()->getMyChatData();
		if(myChatData)
		{
			ref_ptr<crNetChatData::crChatGroupMember> chatMember = new crNetChatData::crChatGroupMember;
			chatMember->setMemberID(senderMemberID);
			chatMember->setPlayerID(senderid);
			chatMember->setMemberChatState(chatState);
			chatMember->setNickName(nickName);
			crNetChatData::crChatGroup *chatGroup = myChatData->findChatGroup(groupid);
			if(chatGroup)
				chatGroup->insertMember(chatMember.get());

			crGlobalHandle::getInstance()->doEvent(WCH_AddGroupMember,MAKEINT64(chatMember.get(),NULL));
			//CRCore::notify(CRCore::ALWAYS)<<"crRemoveGroupMemberPacket"<<std::endl;
		}
	}
}

/////////////////////////////////////////
//
//crQueryOnlinePlayerListPacket
//
/////////////////////////////////////////
crQueryOnlinePlayerListPacket::crQueryOnlinePlayerListPacket():
crStreamPacket(crQueryOnlinePlayerListPacket::ID)
{
}

crQueryOnlinePlayerListPacket::crQueryOnlinePlayerListPacket( const crQueryOnlinePlayerListPacket &p ):
crStreamPacket(crQueryOnlinePlayerListPacket::ID,p)
{
}

void crQueryOnlinePlayerListPacket::buildRequestPacket(crQueryOnlinePlayerListPacket &packet,int rows,int offset)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(rows);
	streamBuf->_writeInt(offset);
}

void crQueryOnlinePlayerListPacket::buildRequestPacket2(crQueryOnlinePlayerListPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQueryOnlinePlayerListPacket::buildReplyPacket(crQueryOnlinePlayerListPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crQueryOnlinePlayerListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crQueryOnlinePlayerListPacket packet;
				crQueryOnlinePlayerListPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryOnlinePlayerListPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int rows = m_streamBuf->_readInt();
		int offset = m_streamBuf->_readInt();

		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid() && offset>0 && offset<=100)
		{
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();
			ref_ptr<crOnchatAccountLimitQueryData> onlineAccountLimitQuery = new crOnchatAccountLimitQueryData;
			onlineAccountLimitQuery->buildQuerySql(rows,offset,senderid);
			globalSession->executeQuery(onlineAccountLimitQuery.get());
			crDataBase::QueryResultVec onlineAccountLimitQueryResultVec = globalSession->getQueryResult();
			globalSession->releaseQuery();
			db->endSession(globalSession.get());
			if(!onlineAccountLimitQueryResultVec.empty())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				int maxsize = 2+64*onlineAccountLimitQueryResultVec.size();
				stream->createBuf(maxsize);
				stream->_writeShort(onlineAccountLimitQueryResultVec.size());
				for( crDataBase::QueryResultVec::iterator itr = onlineAccountLimitQueryResultVec.begin();
						itr != onlineAccountLimitQueryResultVec.end();
						++itr )
				{
					onlineAccountLimitQuery = dynamic_cast<crOnchatAccountLimitQueryData *>(itr->get());
                    stream->_writeInt(onlineAccountLimitQuery->getID());
					stream->_writeString(onlineAccountLimitQuery->getNickName());//20+4
					stream->_writeString(onlineAccountLimitQuery->getAddress());//32+4
				}

                crQueryOnlinePlayerListPacket packet;
				crQueryOnlinePlayerListPacket::buildReplyPacket(packet,senderid,stream.get());
				m_netConductor->getNetManager()->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryOnlinePlayerListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
        int recvid = m_streamBuf->_readInt();
		//short count = m_streamBuf->_readShort();
		crGlobalHandle::getInstance()->doEvent(WCH_QueryOnlinePlayerList,MAKEINT64(m_streamBuf.get(),NULL));
	}
}

/////////////////////////////////////////
//
//crQueryChatGroupListPacket
//
/////////////////////////////////////////
crQueryChatGroupListPacket::crQueryChatGroupListPacket():
crStreamPacket(crQueryChatGroupListPacket::ID)
{
}

crQueryChatGroupListPacket::crQueryChatGroupListPacket( const crQueryChatGroupListPacket &p ):
crStreamPacket(crQueryChatGroupListPacket::ID,p)
{
}

void crQueryChatGroupListPacket::buildRequestPacket(crQueryChatGroupListPacket &packet,int rows,int offset)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(rows);
	streamBuf->_writeInt(offset);
}

void crQueryChatGroupListPacket::buildRequestPacket2(crQueryChatGroupListPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crQueryChatGroupListPacket::buildReplyPacket(crQueryChatGroupListPacket &packet,int senderid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(senderid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crQueryChatGroupListPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crQueryChatGroupListPacket packet;
				crQueryChatGroupListPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryChatGroupListPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int senderid = m_streamBuf->_readInt();
		int rows = m_streamBuf->_readInt();
		int offset = m_streamBuf->_readInt();

		ref_ptr<crChatServerPlayerData> senderPlayerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(senderid));
		if(senderPlayerData.valid() && offset>0 && offset<=100)
		{
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();

			ref_ptr<crChatGroupLimitQueryData> groupLimitQuery = new crChatGroupLimitQueryData;
			groupLimitQuery->buildQuerySql(rows,offset);
			globalSession->executeQuery(groupLimitQuery.get());
			crDataBase::QueryResultVec groupLimitQueryResultVec = globalSession->getQueryResult();
			globalSession->releaseQuery();
			db->endSession(globalSession.get());

			if(!groupLimitQueryResultVec.empty())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				int maxsize = 2+32*groupLimitQueryResultVec.size();
				stream->createBuf(maxsize);
				stream->_writeShort(groupLimitQueryResultVec.size());
				for( crDataBase::QueryResultVec::iterator itr = groupLimitQueryResultVec.begin();
					itr != groupLimitQueryResultVec.end();
					++itr )
				{
					groupLimitQuery = dynamic_cast<crChatGroupLimitQueryData *>(itr->get());
					stream->_writeInt(groupLimitQuery->getID());
					stream->_writeString(groupLimitQuery->getGroupName());//20+4
					stream->_writeInt(groupLimitQuery->getCreator());//4
				}

				crQueryChatGroupListPacket packet;
				crQueryChatGroupListPacket::buildReplyPacket(packet,senderid,stream.get());
				m_netConductor->getNetManager()->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				m_streamBuf->seekBegin();
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),*this);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryChatGroupListPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name
		int recvid = m_streamBuf->_readInt();
		crGlobalHandle::getInstance()->doEvent(WCH_QueryGroupList,MAKEINT64(m_streamBuf.get(),NULL));
	}
}

/////////////////////////////////////////
//
//crRegisterAccountPacket
//
/////////////////////////////////////////
//int crRegisterAccountPacket::s_success = 0;
crRegisterAccountPacket::crRegisterAccountPacket():
crStreamPacket(crRegisterAccountPacket::ID)
{
}

crRegisterAccountPacket::crRegisterAccountPacket( const crRegisterAccountPacket &p ):
crStreamPacket(crRegisterAccountPacket::ID,p)
{
}

void crRegisterAccountPacket::buildRequestPacket(crRegisterAccountPacket &packet, std::string &name, std::string &password, std::string &email, std::string &nickname, std::string &relname, char sex,std::string &code)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(256);
	packet.setStreamBuf(streamBuf);
	//crRegisterAccountPacket::s_success = 0;
	//if(name.size()>16)
	//	name = name.substr(0,16);
	streamBuf->_writeString(name);
	//if(password.size()>16)
	//	password = password.substr(0,16);
	streamBuf->_writeString(password);
	//if(email.size()>20)
	//	email = email.substr(0,20);
	streamBuf->_writeString(email);
	//if(nickname.size()>16)
	//	nickname = nickname.substr(0,16);
	streamBuf->_writeString(nickname);
	//if(relname.size()>16)
	//	relname = relname.substr(0,16);
	streamBuf->_writeString(relname);
	streamBuf->_writeChar(sex);//0女性，1男性
	if(!code.empty())
	{
		//if(code.size()>10)
		//	code = code.substr(0,10);
		streamBuf->_writeString(code);
	}
}

void crRegisterAccountPacket::buildRequestPacket2(crRegisterAccountPacket &packet, const std::string &address, crStreamBuf *buf)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(buf->getRemainSize() + address.length()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(buf->getPtr(),buf->getRemainSize());
}

void crRegisterAccountPacket::buildReplyPacket(crRegisterAccountPacket &packet,const std::string &address,char ret,const std::string &validdate)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	if(ret == -11)
	{
		streamBuf->createBuf(address.length()+9+validdate.length());
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(address);
		streamBuf->_writeChar(ret);//1
		streamBuf->_writeString(validdate);
	}
	else
	{
		streamBuf->createBuf(address.length()+5);
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(address);
		streamBuf->_writeChar(ret);//1
	}
}

void crRegisterAccountPacket::buildReplyPacket2(crRegisterAccountPacket &packet,CRCore::crStreamBuf* buf)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(buf->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(buf->getPtr(),buf->getRemainSize());
}

void crRegisterAccountPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(connectServer)
		{
			crRegisterAccountPacket packet;
			crRegisterAccountPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket("all",packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crRegisterAccountPacket "<<std::endl;
		}
	}
	else if(netType == LoginServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		std::string address = m_streamBuf->_readString();
		std::string name = m_streamBuf->_readString();
		std::string validdate;
		char ret = 0;
		do 
		{
			if(name.empty()/* || name.find("GM")!=std::string::npos || name.find("gm")!=std::string::npos || name.find("Gm")!=std::string::npos || name.find("gM")!=std::string::npos*/)
			{
				ret = -1;//帐号名格式不正确
				break;
			}
			if(name.size()>20)
			{
				ret = -2;//帐号名字符数量超限
				break;
			}
			std::string password = m_streamBuf->_readString();
			if(password.empty() || password.size()>32)
			{
				ret = -3;//密码不正确
				break;
			}
			std::string email = m_streamBuf->_readString();
			if(email.size()>50)
			{
				ret = -4;//邮箱地址字符数量超限
				break;
			}
			std::string nickname = m_streamBuf->_readString();
			if(nickname.size()>20)
			{
				ret = -5;//昵称字符数量超限
				break;
			}
			std::string relname = m_streamBuf->_readString();
			if(relname.size()>20)
			{
				ret = -6;//真实名字字符数量超限
				break;
			}

			char sex = m_streamBuf->_readChar();
			if(!m_streamBuf->eof())
			{
				std::string code = m_streamBuf->_readString();
				if(!code.empty())
				{
					int id = 0;
					bool used = true;
					crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
					ref_ptr<crDataBase> globalSession = db->beginSession();

					ref_ptr<crActivecodeQueryData> activeCodeQuery = new crActivecodeQueryData;
					activeCodeQuery->buildQuerySql(code);
					globalSession->executeQuery(activeCodeQuery.get());
					crDataBase::QueryResultVec& activeCodeQueryResultVec = globalSession->getQueryResult();
					if(!activeCodeQueryResultVec.empty())
					{
						activeCodeQuery = dynamic_cast<crActivecodeQueryData *>(activeCodeQueryResultVec[0].get());
						id = activeCodeQuery->getID();
						used = activeCodeQuery->getUsed();
						validdate = activeCodeQuery->getValidDate();
					}
					globalSession->releaseQuery();
					if(id == 0)
					{
						ret = -7;//激活码输入错误
					}
					else if(used)
					{
						ret = -8;//该激活码已经失效
					}
					else
					{
						if(!validdate.empty())
						{
							time_t t = time( 0 ); 
							char tmp[20];
							strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
							if(strcmp(tmp,validdate.c_str())<0)
							{
								ret = -11;//未到激活码有效期
							}
						}
						if(ret == 0)
						{
							ref_ptr<crInsertAccount> insertAccount = new crInsertAccount;
							insertAccount->buildUpdateSql(name,password,0,email,nickname,relname,sex);
							if(globalSession->executeUpdate(insertAccount.get()))
							{//插入成功
								ref_ptr<crUpdateActiveCode> updateActiveCode = new crUpdateActiveCode;
								updateActiveCode->buildUpdateSql(id);
								globalSession->executeUpdate(updateActiveCode.get());
								globalSession->commit();
							}
							else
							{
								ret = -9;//帐号冲突
							}
						}
					}
					db->endSession(globalSession.get());
				}
				else
				{
					ret = -10;//激活码不能为空
				}
			}
			else
			{
				ref_ptr<crInsertAccount> insertAccount = new crInsertAccount;
				insertAccount->buildUpdateSql(name,password,0,email,nickname,relname,sex);
				crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
				ref_ptr<crDataBase> globalSession = db->beginSession();
				if(globalSession->executeUpdate(insertAccount.get()))
				{//插入成功
					globalSession->commit();
				}
				else
				{
					ret = -9;//帐号冲突
				}
				db->endSession(globalSession.get());
			}
		} while (0);
		crRegisterAccountPacket packet;
		crRegisterAccountPacket::buildReplyPacket(packet,address,ret,validdate);
		netManager->sendPacket(sender,packet);
		//CRCore::notify(CRCore::ALWAYS)<<"crRegisterAccountPacket SendTo ConnectServer "<<std::endl;
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crRegisterAccountPacket packet;
			crRegisterAccountPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crRegisterAccountPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		//CRCore::notify(CRCore::ALWAYS)<<"crRegisterAccountPacket "<<std::endl;
		//bool sucess = m_streamBuf->_readBool();
		//s_success = sucess?1:-1;
		crGlobalHandle::getInstance()->doEvent(WCH_RegisterAccountReturn,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crDeleteAccountPacket
//
/////////////////////////////////////////
crDeleteAccountPacket::crDeleteAccountPacket():
crStreamPacket(crDeleteAccountPacket::ID)
{
}

crDeleteAccountPacket::crDeleteAccountPacket( const crDeleteAccountPacket &p ):
crStreamPacket(crDeleteAccountPacket::ID,p)
{
}

void crDeleteAccountPacket::buildRequestPacket(crDeleteAccountPacket &packet, const std::string &name)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(24);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(name);//20+4
}

void crDeleteAccountPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(connectServer)
		{
			connectServer->getNetManager()->sendPacket("all",*this);
			//CRCore::notify(CRCore::ALWAYS)<<"crDeleteAccountPacket "<<std::endl;
		}
	}
	else if(netType == LoginServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		std::string name = m_streamBuf->_readString();
		ref_ptr<crRemoveAccount> removeAccount = new crRemoveAccount;
		removeAccount->buildUpdateSql(name);

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		if(globalSession->executeUpdate(removeAccount.get()))
		{//插入成功
			globalSession->commit();
			//CRCore::notify(CRCore::ALWAYS)<<"crDeleteAccountPacket 成功删除账号 "<<name<<std::endl;
		}
		db->endSession(globalSession.get());
	}
}

/////////////////////////////////////////
//
//crUpdateAccountPasswordPacket
//
/////////////////////////////////////////
crUpdateAccountPasswordPacket::crUpdateAccountPasswordPacket():
crStreamPacket(crUpdateAccountPasswordPacket::ID)
{
}

crUpdateAccountPasswordPacket::crUpdateAccountPasswordPacket( const crUpdateAccountPasswordPacket &p ):
crStreamPacket(crUpdateAccountPasswordPacket::ID,p)
{
}

void crUpdateAccountPasswordPacket::buildRequestPacket(crUpdateAccountPasswordPacket &packet, int playerid, const std::string &password)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(24);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeString(password);//20+4
}

void crUpdateAccountPasswordPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(connectServer)
		{
			connectServer->getNetManager()->sendPacket("all",*this);
			//CRCore::notify(CRCore::ALWAYS)<<"crUpdateAccountPasswordPacket "<<std::endl;
		}
	}
	else if(netType == LoginServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		int playerid = m_streamBuf->_readInt();
		std::string password = m_streamBuf->_readString();
		ref_ptr<crUpdatePassword> updPassword = new crUpdatePassword;
		updPassword->buildUpdateSql(playerid,password);

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		if(globalSession->executeUpdate(updPassword.get()))
		{//插入成功
			globalSession->commit();
			//CRCore::notify(CRCore::ALWAYS)<<"crUpdateAccountPasswordPacket 成功 "<<playerid<<std::endl;
		}
		db->endSession(globalSession.get());
	}
}

/////////////////////////////////////////
//
//crCreateChatGroupPacket
//
/////////////////////////////////////////
crCreateChatGroupPacket::crCreateChatGroupPacket():
crStreamPacket(crCreateChatGroupPacket::ID)
{
}

crCreateChatGroupPacket::crCreateChatGroupPacket( const crCreateChatGroupPacket &p ):
crStreamPacket(crCreateChatGroupPacket::ID,p)
{
}

void crCreateChatGroupPacket::buildRequestPacket(crCreateChatGroupPacket &packet,const std::string &groupName, const std::string &introduce)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(groupName.length()+introduce.length()+8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(groupName);
	streamBuf->_writeString(introduce);
}
void crCreateChatGroupPacket::buildRequestPacket2(crCreateChatGroupPacket &packet,int playerid, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crCreateChatGroupPacket::buildReplyPacket(crCreateChatGroupPacket &packet,int recvid, int memberid, crNetChatData::crChatGroup *chatGroup)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(chatGroup->getID());
	streamBuf->_writeInt(memberid);
	streamBuf->_writeInt(chatGroup->getGroupID());
	streamBuf->_writeString(chatGroup->getGroupName());
}

void crCreateChatGroupPacket::buildReplyPacket2(crCreateChatGroupPacket &packet, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crCreateChatGroupPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == ChatConnectServer)
	{//转发给客户端
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getChatServerLineConductor();
			if(conductor)
			{
				crCreateChatGroupPacket packet;
				crCreateChatGroupPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateChatGroupPacket "<<std::endl;
			}
		}
	}
	else if(netType == ChatServer)
	{
		int playerid = m_streamBuf->_readInt();
		std::string groupName = m_streamBuf->_readString();
		std::string introduce = m_streamBuf->_readString();
		ref_ptr<crChatServerPlayerData> playerData = dynamic_cast<crChatServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> globalSession = db->beginSession();
			bool globalended = false;
			ref_ptr<crInsertChatGroup> insertGroup = new crInsertChatGroup;
			insertGroup->buildUpdateSql(groupName,introduce,playerid);
			if(globalSession->executeUpdate(insertGroup.get()))
			{
				int groupid = globalSession->getInsertID();

				ref_ptr<crInsertGroupMember> insertGroupMember = new crInsertGroupMember;
				insertGroupMember->buildUpdateSql(groupid,playerid,playerData->getNickName());
				if(globalSession->executeUpdate(insertGroupMember.get()))
				{
					int memberid = globalSession->getInsertID();

					unsigned short chatdbid = playerData->getChatDBID();
					crDataBase *chatdb = crDataBaseManager::getInstance()->getDataBase(ChatDB,chatdbid);
					if(chatdb)
					{
						ref_ptr<crDataBase> chatSession = chatdb->beginSession();
						ref_ptr<crInsertPlayerGroup> insertPlayerGroup = new crInsertPlayerGroup;
						insertPlayerGroup->buildUpdateSql(playerid,groupid);
						if(chatSession->executeUpdate(insertPlayerGroup.get()))
						{
							int playergroupid = chatSession->getInsertID();

							chatSession->commit();
							globalSession->commit();

							chatdb->endSession(chatSession.get());
							db->endSession(globalSession.get());
							globalended = true;

							ref_ptr<crNetChatData::crChatGroup> chatGroup = new crNetChatData::crChatGroup;
							chatGroup->setID(playergroupid);
							chatGroup->setGroupID(groupid);
							chatGroup->setGroupName(groupName);
							chatGroup->setCreator(playerid);

							ref_ptr<crNetChatData::crChatGroupMember> member = new crNetChatData::crChatGroupMember;
							member->setMemberID(memberid);
							member->setMemberName(playerData->getNickName());
							member->setMemberChatState(playerData->getNetChatData()->getMyChatState());
							member->setNickName(playerData->getNickName());
							member->setPlayerID(playerid);
							chatGroup->insertMember(member.get());

							playerData->getNetChatData()->insertChatGroup(chatGroup.get());

							crCreateChatGroupPacket packet;
							crCreateChatGroupPacket::buildReplyPacket(packet,playerid,memberid,chatGroup.get());
							m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

							//CRCore::notify(CRCore::ALWAYS)<<"crCreateChatGroupPacket "<<std::endl;
						}
						else
						{
                            chatdb->endSession(chatSession.get());
						}
					}
				}
			}
			if(!globalended) db->endSession(globalSession.get());
		}
	}
	else if(netType == ChatConnectServerClient_Chat)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(ChatConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crChatConnectServerPlayerData> playerData = dynamic_cast<crChatConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crCreateChatGroupPacket packet;
				crCreateChatGroupPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crCreateChatGroupPacket"<<std::endl;
			}
		}
	}
	else if(netType == GameClient_Chat)
	{//id name

		crNetChatData *myChatData = crMyPlayerData::getInstance()->getMyChatData();
		if(!myChatData)
		{
			myChatData = new crNetChatData;
			myChatData->setMyChatState(crNetChatData::OnLine);
			crMyPlayerData::getInstance()->setMyChatData(myChatData);
		}
		int memberid = m_streamBuf->_readInt();
		ref_ptr<crNetChatData::crChatGroup> chatGroup = new crNetChatData::crChatGroup;
		chatGroup->setID(m_streamBuf->_readInt());
		chatGroup->setGroupID(m_streamBuf->_readInt());
		chatGroup->setGroupName(m_streamBuf->_readString());
		chatGroup->setCreator(crMyPlayerData::getInstance()->getPlayerID());

		ref_ptr<crNetChatData::crChatGroupMember> member = new crNetChatData::crChatGroupMember;
		member->setMemberID(memberid);
		member->setMemberName(crMyPlayerData::getInstance()->getCharacterName());
		member->setMemberChatState(crMyPlayerData::getInstance()->getMyChatData()->getMyChatState());
		member->setNickName(crMyPlayerData::getInstance()->getCharacterName());
		member->setPlayerID(crMyPlayerData::getInstance()->getPlayerID());
		chatGroup->insertMember(member.get());

		myChatData->insertChatGroup(chatGroup.get());

		crGlobalHandle::getInstance()->doEvent(WCH_CreateChatGroup,MAKEINT64(chatGroup->getGroupID(),NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"crCreateChatGroupPacket "<<std::endl;
	}
}