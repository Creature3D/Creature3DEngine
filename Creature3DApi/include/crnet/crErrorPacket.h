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
#ifndef CRNET_CRERRORPACKET_H
#define CRNET_CRERRORPACKET_H 1

#include <CRNet\crExport.h>
#include <CRNet\crStreamPacket.h>
#include <CRCore\crStreamBuf.h>
namespace CRNet {

class CRNET_EXPORT crErrorPacket : public crStreamPacket
{
public:
	// pointer used by GNE
	typedef GNE::SmartPtr<crErrorPacket> sptr;
	// pointer used by GNE
	typedef GNE::WeakPtr<crErrorPacket> wptr;
public :
	crErrorPacket();
	crErrorPacket( const crErrorPacket &p );

	virtual ~crErrorPacket() {}
public:
	static const int ID = WCHNET_ERROR;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crErrorPacket &packet,const std::string &errorCode);
};

}

#endif
