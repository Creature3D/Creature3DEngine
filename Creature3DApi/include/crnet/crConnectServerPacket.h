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
#ifndef CRNET_CRCONNECTSERVERPACKET_H
#define CRNET_CRCONNECTSERVERPACKET_H 1

#include <CRNet\crExport.h>
#include <CRNet\crStreamPacket.h>

namespace CRNet {

class CRNET_EXPORT crConnectServerPacket : public CRNet::crStreamPacket
{
public:
	// pointer used by GNE
	typedef GNE::SmartPtr<crConnectServerPacket> sptr;
	// pointer used by GNE
	typedef GNE::WeakPtr<crConnectServerPacket> wptr;
public :
	crConnectServerPacket();
	crConnectServerPacket( const crConnectServerPacket &p );

	virtual ~crConnectServerPacket() {}
public:
	static const int ID = WCHNET_CONNECTSERVER;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crConnectServerPacket &packet,int playerid,const std::string &serverName,const std::string &serverPassword);
	static void buildReplyPacket(crConnectServerPacket &packet,char accept);
	static int s_success;
};

}

#endif
