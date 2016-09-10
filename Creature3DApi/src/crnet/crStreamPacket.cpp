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
#include <CRNet/crStreamPacket.h>
#include <CRNet/crNetContainer.h>
#include <iostream>
#include <gnelib/PacketParser.h>
#include <gnelib/Buffer.h>
#include <CRIOManager/crCookFile.h>
#include <CRCore/crTimer.h>
#include <CRCore/crBrain.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crRange.h>
#include <CRNet/crNetWorkUpdater.h>
#include "zlib.h"
using namespace CRCore;
using namespace CRNet;

typedef std::set<int> IDSet;
static IDSet s_cookPacketID;
static IDSet s_asynParsePacketID;
static IDSet s_zipPacketID;
void CRNet::insertCookID(int id)
{
    s_cookPacketID.insert(id);
}
bool isCookID(int id)
{
	return s_cookPacketID.find(id) != s_cookPacketID.end();
}
void CRNet::insertAsynParseID(int id)
{//对于一些可能要长时间处理的包，比如加载数据等，应当采用异步方式解析，这样可以保证不阻塞心跳包
	s_asynParsePacketID.insert(id);
}
void CRNet::insertZipID(int id)
{
	s_zipPacketID.insert(id);
}
bool isZipID(int id)
{
	return s_zipPacketID.find(id) != s_zipPacketID.end();
}
#define Rnd(i) i * rand() / RAND_MAX

std::string _basecode = "B2CD6FHJ1LN4RSTE3VWX8YZA0Q7MUK9P5";//(33个字符)
//std::string CRNet::getBaseCode(const std::string &name,const std::string &address)
//{
//	std::string ipstr = address;
//    int i = ipstr.find_first_of(".");
//	ipstr.erase(ipstr.begin()+i);
//	i = ipstr.find_first_of(".");
//	ipstr.erase(ipstr.begin()+i);
//	i = ipstr.find_first_of(".");
//	ipstr.erase(ipstr.begin()+i);
//
//	i = ipstr.find_first_of(":");
//	ipstr = ipstr.substr(0,i);
//
//	i = 33-ipstr.length();
//	std::string namestr = name;
//	while(namestr.length()<i)
//		namestr+=name;
//    namestr = namestr.substr(0,i);
//	return namestr+ipstr;
//}
std::string CRNet::gainKey(/*const std::string &basecode*/)
{
	srand( (unsigned)time( NULL ) );

	std::string code;
	char abc[4];
	int i[4];
	int iabc[3];
	int j,k;
	for(j = 0; j<3; j++)
	{
		for(k = 0; k<4; k++)
		{
			i[k] = Rnd(33);
			abc[k] = _basecode[i[k]];

			code.push_back(abc[k]);
		}
		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
		code.push_back(_basecode[iabc[j]]);
		code.push_back('-');
	}

	int iend[5];
	iend[0] = iabc[0] + iabc[1] + iabc[2];
	iend[1] = iabc[1] + iabc[2] + iend[0];
	iend[2] = iabc[2] + iend[0] + iend[1];
	iend[3] = iend[0] + iend[1] + iend[2];
	iend[4] = iend[1] + iend[2] + iend[3];

	for(j = 0; j<5; j++)
	{
		code.push_back(_basecode[iend[j] % 21]);
	}

	return code;
}
bool CRNet::checkKey(const std::string &key/*, const std::string &basecode*/)
{
	char abc[4];
	char end;
	int i[4];
	int iabc[3];
	int j,k;
	for(j = 0; j<3; j++)
	{
		for(k = 0; k<4; k++)
		{
			abc[k] = key[(j*6+k)];
			i[k] = _basecode.find(abc[k]);
			if(i[k] == -1) return false;
		}

		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
		if(j == 0)
		{    
			end = key[4];
			if(key[5] != '-') 
				return false;
		}
		else if(j == 1)
		{
			end = key[10];
			if(key[11] != '-') 
				return false;
		}
		else if(j == 2)
		{
			end = key[16];
			if(key[17] != '-') 
				return false;
		}
		if(iabc[j]!=_basecode.find(end))
			return false;
	}

	int iend[5];
	iend[0] = iabc[0] + iabc[1] + iabc[2];
	iend[1] = iabc[1] + iabc[2] + iend[0];
	iend[2] = iabc[2] + iend[0] + iend[1];
	iend[3] = iend[0] + iend[1] + iend[2];
	iend[4] = iend[1] + iend[2] + iend[3];

	for(j = 0; j<5; j++)
	{
		if(iend[j] % 21 != _basecode.find(key[18+j]))
			return false;
	}

	return true;
}

std::string CRNet::gainCode(int length)
{
	static std::vector<char> s_baseCodeVec;
	if(s_baseCodeVec.empty())
	{
		char i;
		for ( i = 48; i<=57; i++ )
		{
			s_baseCodeVec.push_back(i);
		}
		for ( i = 65; i<=90; i++ )
		{
			s_baseCodeVec.push_back(i);
		}
		for ( i = 97; i<=122; i++ )
		{
			s_baseCodeVec.push_back(i);
		}
	}
	CRCore::rangei r(0,s_baseCodeVec.size());

	std::string code;
	code.resize(length);
	for (int i=0;i<length;i++)
	{
		code[i] = s_baseCodeVec[r.get_random()];
	}
	return code;
}
bool isSameComputer(const std::string &ip1, const std::string &ip2)
{
	int i = ip1.find(":");
	int j = ip2.find(":");
	return i==j && ip1.substr(0,i) == ip2.substr(0,j);
}
void CRNet::writeStreamVec(CRCore::ref_ptr<CRCore::crStreamBuf>&stream,int needsize,std::vector< ref_ptr<crStreamBuf> > &BufVec,int &count)
{
	if(stream->getRemainCapacity()<needsize)
	{
		count += stream->getBufSize();
		BufVec.push_back(stream);
		int size = stream->getRelMaxBufSize();
		stream = new crStreamBuf;
		stream->createBuf(size);
	}
}
void CRNet::writeLongStream(CRCore::ref_ptr<CRCore::crStreamBuf>&stream,CRCore::crStreamBuf *srcstream,std::vector< ref_ptr<crStreamBuf> > &BufVec,int &count)
{
	while(!srcstream->eof())
	{
		int capacity = stream->getRemainCapacity();
		if(capacity==0)
		{
			count += stream->getBufSize();
			BufVec.push_back(stream);
			int size = stream->getRelMaxBufSize();
			stream = new crStreamBuf;
			stream->createBuf(size);
		}
		if(capacity<srcstream->getRemainSize())
		{
			stream->write(srcstream->getPtr(),capacity);
			srcstream->seek(capacity);
		}
		else
		{
			stream->write(srcstream->getPtr(),srcstream->getRemainSize());
			srcstream->seekEnd();
			break;
		}
	}
}
static char gCharIDArray[128][1];
void CRNet::GInitCharIDArray(char maxid)
{
	for( int i = 0; i<=maxid; i++ )
	{
		gCharIDArray[i][0] = 0;
	}
}
void CRNet::GSetCharID(char id)
{
	if(id>=0) gCharIDArray[id][0] = 1;
}
char CRNet::GGainCharID(char maxid)
{
	for( int i = 0; i<=maxid; i++ )
	{
		if(gCharIDArray[i][0] == 0)
			return i;
	}
	return -1;
}
crStreamPacket::crStreamPacket(int id):
GNE::Packet(id),
m_netConductor(NULL)
{
	//m_streamBuf = new crStreamBuf;
}

crStreamPacket::crStreamPacket( int id, const crStreamPacket &p ):
GNE::Packet(id),
m_netConductor(NULL)
{
	if(p.m_streamBuf.valid())
	{
		m_streamBuf = new crStreamBuf;
		m_streamBuf->createBuf(p.m_streamBuf->getBufSize());
		m_streamBuf->write(p.m_streamBuf->getBuf(),p.m_streamBuf->getBufSize());
		//memcpy( m_streamBuf->getBuf(), p.m_streamBuf->getBuf(), p.m_streamBuf->getBufSize() );
	}
}

void crStreamPacket::writePacket( GNE::Buffer &raw ) const
{
	try
	{
		GNE::Packet::writePacket(raw);
		short size = 0;
		bool hasStream = false;
		if(m_streamBuf.valid())
		{
			size = (signed short)(m_streamBuf->getBufSize());
			if(size > 0)
			{
				//const char *buf = m_streamBuf->getBuf();
				raw << size;
				raw.writeRaw(m_streamBuf->getBuf(), size);
				//for(int i = 0; i<size; i++)
				//{
				//	raw<<(GNE::guint8)(buf[i]);
				//	//CRCore::notify(CRCore::NOTICE)<<(int)(buf[i]);
				//}

				hasStream = true;
			}
		}
		if(!hasStream)
		{
			size = 0;
			raw << size;
		}
	}
	catch (...)
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamPacket::writePacket error"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crStreamPacket::writePacket error\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
	}
	//CRCore::notify(CRCore::NOTICE)<<" crStreamPacket::writePacket end"<<std::endl;
	//CRCore::notify(CRCore::NOTICE)<<" getCapacity = "<<raw.getCapacity();
	//CRCore::notify(CRCore::NOTICE)<<" getLimit = "<<raw.getLimit();
	//CRCore::notify(CRCore::NOTICE)<<" getPosition = "<<raw.getPosition();
}

void crStreamPacket::readPacket( GNE::Buffer &raw)
{
	//CRCore::notify(CRCore::NOTICE)<<" getCapacity = "<<raw.getCapacity();
	//CRCore::notify(CRCore::NOTICE)<<" getLimit = "<<raw.getLimit();
	//CRCore::notify(CRCore::NOTICE)<<" getPosition = "<<raw.getPosition();
	try
	{
		GNE::Packet::readPacket(raw);
		short size;
		raw >> size;
		if(size> 0)
		{
			m_streamBuf = new crStreamBuf;
			m_streamBuf->createBuf(size);
            raw.readRaw(m_streamBuf->getBuf(),size);
			m_streamBuf->setBufSize(size);
			//GNE::guint8 c;
			//for(int i = 0; i<size; i++)
			//{
			//	raw >> c;
			//	m_streamBuf->_writeChar((char)c);
			//	//CRCore::notify(CRCore::NOTICE)<<(int)c;
			//}
			//m_streamBuf->seekBegin();
		}
	}
	catch (...)
	{
		CRCore::notify(CRCore::FATAL)<<"crStreamPacket::readPacket error"<<std::endl;
		//sprintf(gDebugInfo->buf(),"crStreamPacket::readPacket error\n\0");
		//gDebugInfo->debugInfo(CRCore::FATAL);
	}
}

//return the size in bytes
int crStreamPacket::getSize() const
{
	return Packet::getSize() + SHORTSIZE + (m_streamBuf.valid()?m_streamBuf->getBufSize():0);
}
bool CRNet::creCompress(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf)
{
	int uncomprLen = streamBuf->getBufSize();
	uLong comprLen = compressBound(uncomprLen);
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(comprLen+4);
	stream->_writeUInt(uncomprLen);
	int code = compress((Bytef*)stream->getPtr(), &comprLen, (const Bytef*)(streamBuf->getBuf()), (uLong)uncomprLen);
	if(code!=Z_OK)
	{
		CRCore::notify(CRCore::FATAL)<<"compress error:"<<code<<std::endl;
		//sprintf(gDebugInfo->buf(),"compress error:%d\n\0",code);
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return false;
	}
	//else
	//	CRCore::notify(CRCore::ALWAYS)<<"compress reduce "<<(int)comprLen - uncomprLen<<std::endl;
	//streamBuf = stream;
	//streamBuf->setBufSize((int)comprLen+4);
	stream->setBufSize((int)comprLen+4);
	streamBuf->createBuf(stream->getBufSize());
	streamBuf->write(stream->getBuf(),stream->getBufSize());
	streamBuf->seekBegin();
	return true;
}
bool CRNet::creUnCompress(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf)
{
	try{
		//streamBuf->seekBegin();
		//int comprLen = streamBuf->getBufSize()-4;
		int comprLen = streamBuf->getRemainSize()-4;
		uLong uncomprLen = streamBuf->_readUInt();
		ref_ptr<crStreamBuf>stream = new crStreamBuf;
		stream->createBuf(uncomprLen);
		int code = uncompress((Bytef*)stream->getBuf(), &uncomprLen, (const Bytef*)(streamBuf->getPtr()), (uLong)comprLen);
		if(code!=Z_OK)
		{
			CRCore::notify(CRCore::FATAL)<<"uncompress error:"<<code<<std::endl;
			//sprintf(gDebugInfo->buf(),"uncompress error:%d\n\0",code);
			//gDebugInfo->debugInfo(CRCore::FATAL);
			streamBuf->seek(-4);
			return false;
		}
		//else
		//	CRCore::notify(CRCore::ALWAYS)<<"uncompress sucess"<<std::endl;
		streamBuf = stream;
		streamBuf->setBufSize((int)uncomprLen);
		return true;
	}
	catch(...)
	{
		streamBuf->seek(-4);
		return false;
	}
}
static int s_cooki = 0;
void CRNet::creCook(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf)
{
	std::string tempname = "creCook"+crArgumentParser::appItoa(s_cooki++);
	//生成stream.mcook
	CRIOManager::crWriteNetCookFileM scopedWrite(tempname,const_cast<crStreamBuf *>(streamBuf.get()));
	//读取stream.mcook
	crStreamBuf* stream = crBrain::getInstance()->openMemFile(tempname+".mcook",std::ios::in);
	streamBuf = stream;
}
void CRNet::creUnCook(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf)
{
	std::string tempname = "creUnCook"+crArgumentParser::appItoa(s_cooki++);
	//CRCore::notify(CRCore::ALWAYS)<<"crStreamPacket::unCook() pkgid = "<<getType()<<std::endl;
	//生成stream
	CRIOManager::crLoadNetCookFileM scopedLoad(tempname,streamBuf.get());
	//读取stream
	crStreamBuf* stream = crBrain::getInstance()->openMemFile(tempname,std::ios::in);
	streamBuf = stream;
}
bool crStreamPacket::cook()
{
	if(m_streamBuf.valid())
	{
		if(isZipID(getType()))
		{
			//CRCore::notify(CRCore::ALWAYS)<<"Compress pkgid = "<<getType()<<std::endl;
			creCompress(m_streamBuf);
		}
		else if(CRIOManager::GetNetCooked() && isCookID(getType()))
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crStreamPacket::cook() pkgid = "<<getType()<<std::endl;
			//生成stream.mcook
			std::string tempname = "cookpkg"+crArgumentParser::appItoa(s_cooki++);
			CRIOManager::crWriteNetCookFileM scopedWrite(tempname,const_cast<crStreamBuf *>(m_streamBuf.get()));
			//读取stream.mcook
			crStreamBuf* stream = crBrain::getInstance()->openMemFile(tempname+".mcook",std::ios::in);
			/*if(stream) */m_streamBuf = stream;
			if(!m_streamBuf.valid())
				return false;
		}
		else
		{
			m_streamBuf->endian();
		}
		//if(getType() != 120)//crDBSavePacket::ID
		m_streamBuf->cook();
	}
	return true;
}

bool crStreamPacket::unCook(crNetConductor *netConductor,GNE::Connection &conn)
{
	if(m_streamBuf.valid()/* && !isCookID(getType())*/)
	{
		//if(getType() != 120)//crDBSavePacket::ID
		//{
		int code = 0;
		bool rnd = m_streamBuf->uncook(code);
		if(!rnd)
			return false;
		if(crNetBridgeUpdater::isvalid(netConductor))
		{//code驻留100－200ms，只是ConnectServer有效
			if(!conn.checkCode(code))
				return false;
		}
		//}
		if(isZipID(getType()))
		{
			//CRCore::notify(CRCore::ALWAYS)<<"UnCompress pkgid = "<<getType()<<std::endl;
			creUnCompress(m_streamBuf);
		}
		else if(CRIOManager::GetNetCooked() && isCookID(getType()))
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crStreamPacket::unCook() pkgid = "<<getType()<<std::endl;
			//生成stream
			std::string tempname = "uncookpkg"+crArgumentParser::appItoa(s_cooki++);
			CRIOManager::crLoadNetCookFileM scopedLoad(tempname,m_streamBuf.get());
			//读取stream
			crStreamBuf* stream = crBrain::getInstance()->openMemFile(tempname,std::ios::in);
			/*if(stream) */m_streamBuf = stream;
			if(!m_streamBuf.valid())
				return false;
		}
		else
		{
			m_streamBuf->endian();
		}
	}
	return true;
}

bool crStreamPacket::isAsynParsePacket()
{
	return s_asynParsePacketID.find(getType()) != s_asynParsePacketID.end();
}
//////////////////////////////////////////
//
//crHeartJumpPacket
//
////////////////////////////////////
crHeartJumpPacket::crHeartJumpPacket():
crStreamPacket(crHeartJumpPacket::ID)
{
	//m_streamBuf = new crStreamBuf;
	//m_streamBuf->createBuf(1);
	//m_streamBuf->_writeBool(false);
}

crHeartJumpPacket::crHeartJumpPacket( const crHeartJumpPacket &p ):
crStreamPacket(crHeartJumpPacket::ID,p)
{
}
void crHeartJumpPacket::buildRequestPacket(crHeartJumpPacket &packet)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(true);
}
void crHeartJumpPacket::buildReplyPacket(crHeartJumpPacket &packet)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(false);
}
void crHeartJumpPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;
	bool rep = m_streamBuf->_readBool();
	if(rep)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		//if(netManager && !netManager->getIsServer())
		//{
		//m_streamBuf->seekBegin();
		//m_streamBuf->_writeBool(false);
		crHeartJumpPacket packet;
		crHeartJumpPacket::buildReplyPacket(packet);
		netManager->sendPacket(sender,packet);
		//}
	}
}

//////////////////////////////////////////
//
//crUnKnownPacket
//
////////////////////////////////////
crUnKnownPacket::crUnKnownPacket():
crStreamPacket(crUnKnownPacket::ID)
{
}

crUnKnownPacket::crUnKnownPacket( const crUnKnownPacket &p ):
crStreamPacket(crUnKnownPacket::ID,p)
{
}

void crUnKnownPacket::parsePacket(const std::string &sender)
{
	CRCore::notify(CRCore::WARN)<<"收到crUnKnownPacket"<<std::endl;
	//sprintf(gDebugInfo->buf(),"收到crUnKnownPacket\n\0");
	//gDebugInfo->debugInfo(CRCore::WARN);
}