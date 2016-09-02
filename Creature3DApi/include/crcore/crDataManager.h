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
#ifndef CRCORE_CRDATAMANAGER_H
#define CRCORE_CRDATAMANAGER_H 1

#include <CRCore/crExport.h>
#include <CRCore/Referenced.h>	
#include <CRCore/ref_ptr.h>
#include <CRCore\crWchMsg.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
#include <map>
#include <string>
namespace CRCore {
class crState;
#define DataClass(library,name) \
	virtual CRCore::crData* cloneType() const { return new cr##name##Data(); } \
	virtual CRCore::crData* clone(const CRCore::crCopyOp& copyop) const { return new cr##name##Data(*this); } \
	virtual bool isSameKindAs(const CRCore::crData* obj) const { return dynamic_cast<const cr##name##Data *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Data"; }\
	virtual const char* _name() const { return #name; }

	class CR_EXPORT crData : public Referenced
	{
	public :
		crData();
		virtual crData* cloneType() const = 0;
		virtual crData* clone(const CRCore::crCopyOp& copyop) const = 0;
		virtual bool isSameKindAs(const crData* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crData(const crData& data):Referenced(),m_swapFrameNumber(0)
		{
			for( HandleMap::const_iterator itr = data.m_handleMap.begin();
				itr != data.m_handleMap.end();
				++itr )
			{
				insertHandle(itr->first,itr->second->clone());
			}
		}

		typedef std::map<_crInt64, ref_ptr<crHandle> > HandleMap;

		inline void insertHandle(_crInt64 msg, crHandle *handle)
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			m_handleMap[msg] = handle;
		}
		inline crHandle *getHandle(_crInt64 msg) 
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			HandleMap::iterator itr = m_handleMap.find(msg); 
			return itr!=m_handleMap.end()?itr->second.get():NULL; 
		}
		inline void removeHandle(_crInt64 msg) 
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			m_handleMap.erase(msg); 
		}
		inline virtual void excHandle(_crInt64 msg){ excHandle(getHandle(msg)); }
		inline void excHandle(crHandle *handle) 
		{
			if(handle)
			{
				(*handle)(*this);
			}
		}

		inline virtual void addParam(int i, const std::string& str)=0;
		inline virtual void inputParam(int i, void *param)=0;
		inline virtual void getParam(int i, void*& param){ param = NULL; }
		inline virtual void clear(){ m_handleMap.clear(); }
		inline void lock(){ excHandle(MAKEINT64(WCH_USER+5,1)); }//WCH_USER+5:WCH_LockData
		inline void unlock(){ excHandle(MAKEINT64(WCH_USER+5,0)); }//WCH_USER+5:WCH_LockData
		inline virtual void lockParam(int i){}
		inline virtual void unlockParam(int i){}
		virtual void releaseObjects(crState* state);
		virtual void swapBuffers(int frameNumber);
	protected :
		virtual ~crData();
		HandleMap m_handleMap;
		crMutex                  m_mutex;
		int m_swapFrameNumber;
	};

	class CR_EXPORT crDataManager : public Referenced
	{
	public :
		crDataManager(){};
	    static crDataManager *getInstance();
        
		static void RegisterDataProxy(crData *data){ getInstance()->_register(data->_name(),data); }

		crData *getData(const std::string &name);
		virtual void clear();
	protected :
		virtual ~crDataManager() {}
		static ref_ptr<crDataManager> m_instance;

		void _register(const std::string &name, crData *data);

		typedef std::map< const std::string, ref_ptr<crData> > DataMap;
		DataMap m_dataMap;
	};

	class CR_EXPORT crEventData : public crData
	{
	public:
		crEventData(){};
		crEventData(const crEventData& data):crData(data){};
		DataClass(CRCore, Event)
		inline virtual void addParam(int i, const std::string& str){}
		inline virtual void inputParam(int i, void *param){}
	};
	class CR_EXPORT crNodeData : public crData
	{
	public:
		crNodeData(){};
		crNodeData(const crNodeData& data):crData(data){};
		DataClass(CRCore, Node)
		inline virtual void addParam(int i, const std::string& str){}
		inline virtual void inputParam(int i, void *param){}
	};

	class CR_EXPORT crUIData : public crData
	{
	public:
		crUIData(){};
		crUIData(const crUIData& data):crData(data){};
		DataClass(CRCore, UI)
		inline virtual void addParam(int i, const std::string& str){}
		inline virtual void inputParam(int i, void *param){}
	};
	class CR_EXPORT crWindowUIData : public crUIData
	{
	public:
		crWindowUIData(){};
		crWindowUIData(const crWindowUIData& data):crUIData(data){};
		DataClass(CRCore, WindowUI)
		inline virtual void addParam(int i, const std::string& str){}
		inline virtual void inputParam(int i, void *param){}
	};

	class CR_EXPORT crDataObject : public Referenced
	{
	public:
		crDataObject(){};
		/////////////////////
		virtual void setDataClass( CRCore::crData *data );
		CRCore::crData *getDataClass();
		virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
		inline _crInt64 getCurrentMsg() const { return m_currentMsg; }
		CRCore::crHandle* getHandle(__int64 kbmsg, const std::string& name);
		bool hasHandle(__int64 kbmsg);
		virtual void clearData();
		//////////////////////////
	protected:
		CRCore::ref_ptr<CRCore::crData> m_dataClass;
		_crInt64 m_currentMsg;
	};
}

#endif
