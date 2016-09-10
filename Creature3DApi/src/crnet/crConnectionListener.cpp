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
#include <CRNet/crConnectionListener.h>
#include <CRNet/crNetBridge.h>
#include <CRCore/crNotify.h>

#include <gnelib/Error.h>
#include <gnelib/Connection.h>
#include <gnelib/SyncConnection.h>

using namespace CRNet;
using namespace CRCore;

crConnectionListener::crConnectionListener(crNetBridge *netBridge):
m_netBridge(netBridge)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"crConnectionListener created."<<std::endl;
}

crConnectionListener::~crConnectionListener(void)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"crConnectionListener destroyed."<<std::endl;
}

void crConnectionListener::onDisconnect( GNE::Connection& conn )
{
   m_netBridge->onDisconnect(conn);
}

void crConnectionListener::onExit( GNE::Connection& conn )
{
   m_netBridge->onExit( conn );
}

void crConnectionListener::onNewConn( GNE::SyncConnection& conn)
{
   m_netBridge->onNewConn(conn);
}

void crConnectionListener::onConnect( GNE::SyncConnection &conn )
{
   m_netBridge->onConnect(conn);
}

void crConnectionListener::onReceive( GNE::Connection& conn )
{
   m_netBridge->onReceive( conn );
}

void crConnectionListener::onFailure( GNE::Connection& conn, const GNE::Error& error )
{
   m_netBridge->onFailure(conn, error);
}

void crConnectionListener::onError( GNE::Connection& conn, const GNE::Error& error )
{
   m_netBridge->onError(conn, error);
}

void crConnectionListener::onConnectFailure( GNE::Connection &conn, const GNE::Error &error)
{
   m_netBridge->onConnectFailure(conn, error);
}

void crConnectionListener::onTimeout( GNE::Connection& conn )
{
   m_netBridge->onTimeout(conn);
}


