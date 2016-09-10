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
#include <CRNet/crConnectionServer.h>
#include <CRNet/crConnectionListener.h>
#include <CRNet/crNetBridge.h>
#include <CRCore/crNotify.h>

#include <gnelib/ConnectionParams.h>

using namespace CRNet;
using namespace CRCore;

crConnectionServer::crConnectionServer(int inRate, int outRate, int bufScale, int timeout, crNetBridge *netBridge):
   GNE::ServerConnectionListener(),
   m_inRate(inRate),
   m_outRate(outRate),
   m_bufScale(bufScale),
   m_timeout(timeout),
   m_netBridge(netBridge)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"Creating"<<std::endl;
}


crConnectionServer::~crConnectionServer(void)
{
	//CRCore::notify(CRCore::DEBUG_INFO)<<"Destroying"<<std::endl;
}


void crConnectionServer::onListenFailure(const GNE::Error& error, const GNE::Address& from, const GNE::ConnectionListener::sptr& listener)
{
   m_netBridge->onListenFailure( error, from, listener );
}

void crConnectionServer::onListenSuccess(const GNE::ConnectionListener::sptr &listener)
{
   m_netBridge->onListenSuccess();     
}

void crConnectionServer::getNewConnectionParams( GNE::ConnectionParams &params )
{
   params.setInRate(m_inRate);
   params.setOutRate(m_outRate);
   params.setUnrel(false); //?
   params.setBufSizeScale(m_bufScale);
   params.setTimeout(m_timeout);
   params.setListener( crConnectionListener::create(m_netBridge.get()) );
}

bool crConnectionServer::tryAddNewConnection(NLsocket socket)
{
    return m_netBridge->tryAddNewConnection(socket);
}

void crConnectionServer::removeNewConnectionTry(NLsocket socket)
{
	m_netBridge->removeNewConnectionTry(socket);
}