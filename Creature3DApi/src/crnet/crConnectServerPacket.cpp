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
#include <CRNet/crConnectServerPacket.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRNet/crErrorPacket.h>
#include <CRNet/crNetContainer.h>
#include <CRNet/crServerRefusePacket.h>
#include <CRCore/crTimer.h>
#include <time.h>
using namespace CRCore;
using namespace CRNet;

int crConnectServerPacket::s_success = 0;

crConnectServerPacket::crConnectServerPacket():
crStreamPacket(crConnectServerPacket::ID)
{
}

crConnectServerPacket::crConnectServerPacket( const crConnectServerPacket &p ):
crStreamPacket(crConnectServerPacket::ID,p)
{
}

void crConnectServerPacket::buildRequestPacket(crConnectServerPacket &packet,int playerid,const std::string &serverName,const std::string &serverPassword)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);

	crConnectServerPacket::s_success = 0;
	//streamBuf->_writeBool(false);//1异步解析标识符，客户端发给服务器
	//streamBuf->_writeInt(crConnectServerPacket::ID);//id---4
	streamBuf->_writeInt(playerid);//name---最大20+4
	streamBuf->_writeString(serverName);//name---最大20+4
	streamBuf->_writeString(serverPassword);//name---最大6+4
}

void crConnectServerPacket::buildReplyPacket(crConnectServerPacket &packet,char accept)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);

	crConnectServerPacket::s_success = 0;
	//streamBuf->_writeBool(false);//1异步解析标识符，服务器发回给客户端
	//streamBuf->_writeInt(crConnectServerPacket::ID);//id---4
	streamBuf->_writeChar(accept);//name---最大20+4
}

void crConnectServerPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	crNetManager *netManager = m_netConductor->getNetManager();
	crNetDataManager *netDataManager = m_netConductor->getNetDataManager();

	if(netManager->getIsServer())
	{
		std::string serverName,serverPassword;
		char accept = 1;
		int playerid = m_streamBuf->_readInt();
		serverName = m_streamBuf->_readString();
		serverPassword = m_streamBuf->_readString();
		//if(!netManager->checkConnectCount())
		//{//超出服务器最大连接数
		//	//CRCore::notify(CRCore::FATAL)<<"crLanLoginPacket::parsePacket Server checkConnectCount = "<<std::endl;
  //          accept = -1;
		//}
		//else if(netDataManager->findNameClient(myName))
		//{//同名客户端已经存在
  //          accept = -2;
		//}
		if(serverName.compare(netDataManager->getServerName()) != 0)
		{//要求连接的服务端名字与本服务器名不同
			//CRCore::notify(CRCore::FATAL)<<"crLanLoginPacket::parsePacket Server findNameClient = "<<name<<std::endl;
			accept = -3;
		}
		else if(serverPassword.compare(netDataManager->getServerPassword()) != 0)
		{//服务器密码不正确
			//CRCore::notify(CRCore::FATAL)<<"crLanLoginPacket::parsePacket Server password = "<<password<<std::endl;
			accept = -4;
		}
        
		//if(accept == -2)
		//{
		//	std::string adress;
  //          netDataManager->getNameClientAddress(myName,adress);
		//	crServerRefusePacket refusePacket;
		//	crServerRefusePacket::buildRequestPacket(refusePacket,-1);
		//	netManager->sendPacket(adress,refusePacket);

		//	netManager->removeConnection(adress);

		//	accept = 1;
		//}
		//if(accept>0)
		//{
		//	netDataManager->insertNameClient(myName,sender);
		//	crClientData *data;
		//	if(m_netConductor->getNetType() == ConnectServer)
		//	{
		//		data = new crConnectServerClientData;
		//	}
		//	else
		//	{
		//		data = new crClientData;
		//	}
		//	data->setStamp(-59000.0f);//登陆之后60s内如果没有发送心跳消息，则自动断开
		//	netDataManager->insertClientData(sender,data);
		//}

		crConnectServerPacket packet;
		crConnectServerPacket::buildReplyPacket(packet,accept);
		netManager->sendPacket(sender,packet);
		if(accept<0) 
			netManager->removeConnection(sender);
		else
		{
			std::string filename = netDataManager->getServerName() + ".log";
			std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
			if(fout)
			{
				time_t t = time( 0 ); 
				char tmp[20];
				strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
				fout<<tmp<<" 客户端["<<sender<<" "<<playerid<<"]登陆已被服务器接受，当前连接的客户端数量："<<netManager->getNumConnections()<<" 尝试连接数量："<<netManager->getNumTryConnections()<<std::endl;
				fout.close();
			}
		}

	}
	else
	{//id name
		s_success = m_streamBuf->_readChar();
	}
}
