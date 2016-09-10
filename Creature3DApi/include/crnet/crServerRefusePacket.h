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
#ifndef CRNET_CRSERVERREFUSEPACKET_H
#define CRNET_CRSERVERREFUSEPACKET_H 1

#include <CRNet\crExport.h>
#include <CRNet\crStreamPacket.h>

namespace CRNet {
//服务器拒绝连接，客户端受到该包后停止自动重连机制
class CRNET_EXPORT crServerRefusePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crServerRefusePacket> sptr;
	typedef GNE::WeakPtr<crServerRefusePacket> wptr;
public :
	crServerRefusePacket();
	crServerRefusePacket( const crServerRefusePacket &p );

	virtual ~crServerRefusePacket() {}
public:
	static const int ID = WCHNET_SERVERREFUSE;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crServerRefusePacket &packet, int code);//-1表示同名客户端登陆
};

}

#endif
