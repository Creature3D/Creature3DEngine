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
#ifndef CRCORE_CREVENTCALLBACKMANAGER_H
#define CRCORE_CREVENTCALLBACKMANAGER_H 1

#include <CRCore/crExport.h>
#include <CRCore/Referenced.h>	
#include <CRCore/ref_ptr.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/crState.h>
#include <map>
#include <string>
namespace CRCore {


#define META_EventCallback(library,name) \
	virtual CRCore::crEventCallback* cloneType() const { return new cr##name##Callback(); } \
	virtual CRCore::crEventCallback* clone(const CRCore::crCopyOp& copyop) const { return new cr##name##Callback(*this,copyop); } \
	virtual bool isSameKindAs(const CRCore::crEventCallback* obj) const { return dynamic_cast<const cr##name##Callback *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Callback"; }\
	virtual const char* _name() const { return #name; }

	class CR_EXPORT crEventCallback : public Referenced
	{
	public :
		crEventCallback():m_callbackTaskCanceled(false){}
		virtual crEventCallback* cloneType() const = 0;
		virtual crEventCallback* clone(const CRCore::crCopyOp& copyop) const = 0;
		virtual bool isSameKindAs(const crEventCallback* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crEventCallback(const crEventCallback& callback,const crCopyOp&copyop):Referenced(),m_callbackTaskCanceled(false){}

		inline virtual void addEventParam(int i, const std::string &str){}
		inline virtual void operator()(void){};

		inline void initCallbackTask(){ m_callbackTaskCanceled = false;}
		inline void cancelCallbackTask(){ m_callbackTaskCanceled = true;}
		inline bool continueCallbackTask(){ return !m_callbackTaskCanceled;}
		virtual void releaseObjects(CRCore::crState* state){}
	protected :
		virtual ~crEventCallback() {}
		//crMutex m_mutex;
		bool m_callbackTaskCanceled;
	};

	class CR_EXPORT crEventCallbackManager : public Referenced
	{
	public :
		crEventCallbackManager(){};
	    static crEventCallbackManager *getInstance();
        
		static void RegisterEventCallbackProxy(crEventCallback *callback){ getInstance()->registerEventCallback(callback->_name(),callback); }

		crEventCallback *getEventCallback(const std::string &name);
		virtual void clear();
	protected :
		virtual ~crEventCallbackManager() {}
		static ref_ptr<crEventCallbackManager> m_instance;

		void registerEventCallback(const std::string &name, crEventCallback *callback);

		typedef std::map< const std::string, ref_ptr<crEventCallback> > EventCallbackMap;
		EventCallbackMap m_eventCallbackMap;
	};
}

#endif
