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
#ifndef CRNET_CRCONNECTIONSERVER_H
#define CRNET_CRCONNECTIONSERVER_H

#include <CRNet\crExport.h>
#include <CRCore/ref_ptr.h>

#include <gnelib/ServerConnectionListener.h>
#include <gnelib/ConnectionListener.h>
#include <gnelib/SmartPtr.h>

namespace CRNet
{
class crNetBridge;
/** This class is used as an interface to the GNE::Server connection.  It is
*  used internally by the crNetBridge and is typically not used directly by the
*  end user.
*  This class takes in a reference to a crNetBridge and calls it's virtual methods
*  to mimic the GNE::ServerConnectionLister's callbacks.
*/
class  CRNET_EXPORT crConnectionServer : public GNE::ServerConnectionListener
{
public:

    virtual ~crConnectionServer(void);

    typedef GNE::SmartPtr<crConnectionServer> sptr;
    typedef GNE::WeakPtr<crConnectionServer> wptr;

    ///Method used to create a new instance of crConnectionServer
    static sptr create( int inRate, int outRate, int bufScale, int timeout, crNetBridge *netBridge)
    {
        sptr ret( new crConnectionServer(inRate, outRate, bufScale, timeout, netBridge) );         
        ret->setThisPointer( ret );
        return ret;
    }

    virtual void getNewConnectionParams( GNE::ConnectionParams &params );

    virtual void onListenFailure(const GNE::Error& error, const GNE::Address& from, const GNE::ConnectionListener::sptr &listener);

    virtual void onListenSuccess(const GNE::ConnectionListener::sptr &listener);

	virtual bool tryAddNewConnection(NLsocket socket);
	virtual void removeNewConnectionTry(NLsocket socket);
protected:
	/** @param inRate : the incoming bandwidth throttle
	*  @param outRate : the outgoing bandwidth throttlw
	*  @param netBridge  : instance of a valid crNetBridge
	*/
	crConnectionServer(int inRate, int outRate, int bufScale, int timeout,  crNetBridge *netBridge );
private:
    int m_inRate; ///<The incoming bandwidth rate
    int m_outRate;///<The outgoing bandwidth rate
	int m_timeout;
	int m_bufScale;
	CRCore::ref_ptr<crNetBridge> m_netBridge; ///Reference to a crNetBridge
    GNE::Mutex m_mutex;
};
}


#endif //CONNECTIONSERVER_INCLUDE

