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
#include <CRNet/crServerRefusePacket.h>
#include <CRNet/crNetManager.h>
#include <CRNet/crNetDataManager.h>
#include <CRNet/crNetContainer.h>

using namespace CRCore;
using namespace CRNet;

crServerRefusePacket::crServerRefusePacket():
crStreamPacket(crServerRefusePacket::ID)
{
}

crServerRefusePacket::crServerRefusePacket( const crServerRefusePacket &p ):
crStreamPacket(crServerRefusePacket::ID,p)
{
}

void crServerRefusePacket::buildRequestPacket(crServerRefusePacket &packet,int code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(128);//108
	packet.setStreamBuf(streamBuf);

	streamBuf->_writeBool(false);//1
	streamBuf->_writeInt(code);//name---8+4
}

void crServerRefusePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	m_streamBuf->_readBool();

	crNetManager *netManager = m_netConductor->getNetManager();

	if(!netManager->getIsServer())
	{
		int code = m_streamBuf->_readInt();
        netManager->setServerRefuseCode(code);
	}
}
