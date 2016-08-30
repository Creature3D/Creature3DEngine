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
#ifndef CRNET_CRREGSERVERCLIENTPACKET_H
#define CRNET_CRREGSERVERCLIENTPACKET_H 1

#include <CRNet\crExport.h>
#include <CRNet\crStreamPacket.h>

namespace CRNet {

class CRNET_EXPORT crRegServerClientPacket : public CRNet::crStreamPacket
{
public:
	// pointer used by GNE
	typedef GNE::SmartPtr<crRegServerClientPacket> sptr;
	// pointer used by GNE
	typedef GNE::WeakPtr<crRegServerClientPacket> wptr;
public :
	crRegServerClientPacket();
	crRegServerClientPacket( const crRegServerClientPacket &p );

	virtual ~crRegServerClientPacket() {}
public:
	static const int ID = WCHNET_REGSERVERCLIENT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crRegServerClientPacket &packet,const std::string &serverName);
};

}

#endif
