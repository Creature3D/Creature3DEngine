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
#ifndef CRNET_CRSTREAMPACKET_H
#define CRNET_CRSTREAMPACKET_H 1

#include <CRCore\ref_ptr.h>
#include <CRCore\Referenced.h>
#include <CRNet\crExport.h>
#include <CRCore\crStreamBuf.h>
#include <gnelib.h>

#define WCHNETID               5
#define WCHNET_UNKNOWN         WCHNETID
#define WCHNET_HEARTJUMP       WCHNETID+1
#define WCHNET_ERROR           WCHNETID+2
#define WCHNET_CONNECTSERVER   WCHNETID+3
#define WCHNET_SERVERREFUSE    WCHNETID+4//服务器拒绝连接，客户端受到该包后停止自动重连机制
//#define WCHNET_REGSERVERCLIENT WCHNETID+5

//#define WCHNET_LANCONNECT     GNE::PacketParser::MIN_USER_ID+3
//#define WCHNET_LANAI          GNE::PacketParser::MIN_USER_ID+4
//
//#define WCHNET_GAMESERVERLOGIN     GNE::PacketParser::MIN_USER_ID+6
//#define WCHNET_GAMESERVERUPDATE    GNE::PacketParser::MIN_USER_ID+7
//#define WCHNET_CLIENTLOGINSERVER   GNE::PacketParser::MIN_USER_ID+8
//RAW_PACKET_LEN = 10240 max buffer size = 10240  RAW_PACKET_LEN-6
//#define MAXBUFFERSIZE         8181//16373//16k - 6
//NL_MAX_PACKET_LENGTH - 6
//#define MAXBUFFERSIZE        8182
#define MAXBUFFERSIZE        NL_MAX_PACKET_LENGTH - 216
#define MTUBUFFERSIZE		 MAXBUFFERSIZE-12
//6=typeid+streamsize=4+2
namespace CRNet {
class crNetConductor;
extern CRNET_EXPORT void insertCookID(int id);
extern CRNET_EXPORT void insertAsynParseID(int id);
extern CRNET_EXPORT void insertZipID(int id);
//extern CRNET_EXPORT std::string getBaseCode(const std::string &name,const std::string &address);
extern CRNET_EXPORT std::string gainKey(/*const std::string &basecode*/);
extern CRNET_EXPORT bool checkKey(const std::string &key/*, const std::string &basecode*/);//返回true表示成功否则失败
extern CRNET_EXPORT std::string gainCode(int length = 16);
extern CRNET_EXPORT void writeStreamVec(CRCore::ref_ptr<CRCore::crStreamBuf>&stream,int needsize,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count);
extern CRNET_EXPORT void writeLongStream(CRCore::ref_ptr<CRCore::crStreamBuf>&stream,CRCore::crStreamBuf *srcstream,std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > &BufVec,int &count);
extern CRNET_EXPORT bool creCompress(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf);
extern CRNET_EXPORT bool creUnCompress(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf);
extern CRNET_EXPORT void GInitCharIDArray(char maxid = 127);
extern CRNET_EXPORT void GSetCharID(char id);
extern CRNET_EXPORT char GGainCharID(char maxid = 127);
extern CRNET_EXPORT void creCook(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf);
extern CRNET_EXPORT void creUnCook(CRCore::ref_ptr<CRCore::crStreamBuf>& streamBuf);
class CRNET_EXPORT crStreamPacket : public GNE::Packet
{
public:
	// pointer used by GNE
	typedef GNE::SmartPtr<crStreamPacket> sptr;
	// pointer used by GNE
	typedef GNE::WeakPtr<crStreamPacket> wptr;
public :
	crStreamPacket(int id = crStreamPacket::ID);
	crStreamPacket( int id, const crStreamPacket &p );

	virtual ~crStreamPacket() {}
public:
	static const int ID = WCHNETID;

	virtual int getSize() const;

	virtual void writePacket( GNE::Buffer &raw ) const;
	virtual void readPacket( GNE::Buffer &raw );
public:
    bool cook();
	bool unCook(crNetConductor *netConductor,GNE::Connection &conn);
	bool isAsynParsePacket();
	virtual void parsePacket(const std::string &sender){};
	inline CRCore::crStreamBuf *getStreamBuf(){ return m_streamBuf.get(); }
	inline void setStreamBuf(CRCore::crStreamBuf *streambuf){ m_streamBuf = streambuf; }
	inline void resetStream(){ m_streamBuf->resetBuf(); }

	void attachNetConductor(crNetConductor *netConductor){ m_netConductor = netConductor; }
	inline crNetConductor *getNetConductor(){ return m_netConductor; }
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_streamBuf;
	crNetConductor* m_netConductor;
};

class CRNET_EXPORT crHeartJumpPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crHeartJumpPacket> sptr;
	typedef GNE::WeakPtr<crHeartJumpPacket> wptr;
public :
	crHeartJumpPacket();
	crHeartJumpPacket( const crHeartJumpPacket &p );

	virtual ~crHeartJumpPacket() {}
public:
	static const int ID = WCHNET_HEARTJUMP;
public:
	static void buildRequestPacket(crHeartJumpPacket &packet);
	static void buildReplyPacket(crHeartJumpPacket &packet);
	virtual void parsePacket(const std::string &sender);
};

class CRNET_EXPORT crUnKnownPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crUnKnownPacket> sptr;
	typedef GNE::WeakPtr<crUnKnownPacket> wptr;
public :
	crUnKnownPacket();
	crUnKnownPacket( const crUnKnownPacket &p );

	virtual ~crUnKnownPacket() {}
public:
	static const int ID = WCHNET_UNKNOWN;
public:
	virtual void parsePacket(const std::string &sender);
};
}

#endif
