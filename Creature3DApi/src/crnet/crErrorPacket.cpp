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
#include <CRNet/crErrorPacket.h>
#include <CRCore/crNotify.h>
#include <string>

using namespace CRCore;
using namespace CRNet;

crErrorPacket::crErrorPacket():
crStreamPacket(crErrorPacket::ID)
{
}

crErrorPacket::crErrorPacket( const crErrorPacket &p ):
crStreamPacket(crErrorPacket::ID,p)
{
}

void crErrorPacket::buildRequestPacket(crErrorPacket &packet,const std::string &errorCode)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(256);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(true);//1异步解析标识符
	streamBuf->_writeString(errorCode);
}

void crErrorPacket::parsePacket(const std::string &sender)
{//id errorcode
	m_streamBuf->_readBool();
	std::string errorCode;
	errorCode = m_streamBuf->_readString();
	CRCore::notify(CRCore::FATAL)<<"NetError: "<<errorCode.c_str()<<std::endl;
	//sprintf(gDebugInfo->buf(),"NetError:%s\n\0",errorCode.c_str());
	//gDebugInfo->debugInfo(CRCore::FATAL);
}