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
#include <CRNet/crRegServerClientPacket.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRNet/crNetContainer.h>
using namespace CRCore;
using namespace CRNet;

crRegServerClientPacket::crRegServerClientPacket():
crStreamPacket(crRegServerClientPacket::ID)
{
}

crRegServerClientPacket::crRegServerClientPacket( const crRegServerClientPacket &p ):
crStreamPacket(crRegServerClientPacket::ID,p)
{
}

void crRegServerClientPacket::buildRequestPacket(crRegServerClientPacket &packet,const std::string &serverName)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(32);
	packet.setStreamBuf(streamBuf);

	streamBuf->_writeBool(false);//1异步解析标识符，客户端发给服务器
	streamBuf->_writeString(serverName);//name---最大20+4
}

void crRegServerClientPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	m_streamBuf->_readBool();

	crNetManager *netManager = m_netConductor->getNetManager();
	if(netManager->getIsServer())
	{
        crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		netDataManager->regServerClient(m_streamBuf->_readString(),sender);
	}
}
