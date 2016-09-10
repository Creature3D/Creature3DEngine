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
#ifndef CRNET_CRNETCALLBACKMANAGER_H
#define CRNET_CRNETCALLBACKMANAGER_H 1

#include <CRNet/crStreamPacket.h>
#include <CRNet/crExport.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <map>
#include <string>

#define META_NetCallback(library,name) \
	virtual CRNet::crNetCallback* cloneType() const { return new cr##name##Callback(); } \
	virtual CRNet::crNetCallback* clone() const { return new cr##name##Callback(*this); } \
	virtual bool isSameKindAs(const CRNet::crNetCallback* obj) const { return dynamic_cast<const cr##name##Callback *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Callback"; }\
	virtual const char* _name() const { return #name; }

namespace CRNet {
class crNetConductor;
class crPlayerData;
	class CRNET_EXPORT crNetCallback : public CRCore::Referenced
	{
	public:
		crNetCallback():m_netConductor(NULL),m_needReconnect(false){}
		~crNetCallback(){ m_netConductor = NULL; }
		virtual crNetCallback* cloneType() const = 0;
		virtual crNetCallback* clone() const = 0;
		virtual bool isSameKindAs(const crNetCallback* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crNetCallback(const crNetCallback& callback){}

		void init(crNetConductor *netConductor);
		virtual void serverInitImplementation() = 0;
		virtual void clientInitImplementation() = 0;
		virtual void serverUpdateImplementation(float dt)=0;
		virtual void clientUpdateImplementation(float dt)=0;
		virtual void serverRemoveConnectionImplementation(const std::string& address){}
		virtual void serverShutdown() = 0;
		virtual void clientShutdown() = 0;
		virtual bool clientReconnectToServer(){ return true; }
		virtual void playerDroped(CRNet::crPlayerData *playerData){}
		virtual void removeConnection(CRNet::crPlayerData *playerData){}
		inline crNetConductor *getNetConductor() { return m_netConductor; } 
	protected:
		crNetConductor *m_netConductor;
		bool m_needReconnect;
	};

	class CRNET_EXPORT crNetCallbackManager : public CRCore::Referenced
	{
	public :
		crNetCallbackManager(){};
	    static crNetCallbackManager *getInstance();
        
		static void RegisterNetCallbackProxy(crNetCallback *callback){ getInstance()->registerNetCallback(callback->_name(),callback); }

		crNetCallback *getNetCallback(const std::string &name);
		virtual void clear();

		typedef std::set< CRCore::ref_ptr<crNetCallback> > CallbackSet;
		void insertNeedReconnectCallback(crNetCallback *callback);
		void removeNeedReconnectCallback(crNetCallback *callback);
		void reconnectLock();
		void reconnectUnlock();
		int getNeedReconnectSize();
		void clientReconnectToServer();
	protected :
		virtual ~crNetCallbackManager();
		static CRCore::ref_ptr<crNetCallbackManager> m_instance;

		void registerNetCallback(const std::string &name, crNetCallback *callback);

		typedef std::map< const std::string, CRCore::ref_ptr<crNetCallback> > NetCallbackMap;
		NetCallbackMap m_netCallbackMap;

		CallbackSet m_needReconnetCallbackSet;
	    CRCore::crCriticalMutex m_reconnectMutex;
	};
}

#endif
