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
#ifndef CRNET_CRCONNECTIONLISTENER_H
#define CRNET_CRCONNECTIONLISTENER_H

#include <CRNet\crExport.h>
#include <gnelib/ConnectionListener.h>
#include <CRCore/ref_ptr.h>

namespace CRNet
{
   class crNetBridge;

   /** Provides the interface to a GNE::Connection.  This class is used internally
    *  by the crNetBridge and is typically not used directly by the end user.
    *  This class contains a reference to an instance of crNetBridge and calls it's
    *  virtual methods, mimicking the GNE interface.
    */
   class CRNET_EXPORT crConnectionListener : public GNE::ConnectionListener
   {
   public:
      typedef GNE::SmartPtr<crConnectionListener> sptr;
      typedef GNE::WeakPtr<crConnectionListener> wptr;


      virtual ~crConnectionListener(void);

      ///static method used to create a new crConnectionListener
      static sptr create( crNetBridge *netBridge)
      {
         return sptr( new crConnectionListener(netBridge) );
      }

      virtual void onDisconnect( GNE::Connection& conn );

      virtual void onExit( GNE::Connection& conn );

      virtual void onConnect( GNE::SyncConnection &conn );

      virtual void onConnectFailure( GNE::Connection &conn, const GNE::Error &error);

      virtual void onNewConn( GNE::SyncConnection& conn);

      virtual void onReceive( GNE::Connection& conn );

      virtual void onFailure( GNE::Connection& conn, const GNE::Error& error );
         
      virtual void onError( GNE::Connection& conn, const GNE::Error& error );
      
	  virtual void onTimeout( GNE::Connection& conn );
   protected:
      crConnectionListener(crNetBridge *netBridge);
   private:
      CRCore::ref_ptr<crNetBridge> m_netBridge; ///Reference to a crNetBridge
   };
}

#endif //CONNECTIONLISTENER_INCLUDE
