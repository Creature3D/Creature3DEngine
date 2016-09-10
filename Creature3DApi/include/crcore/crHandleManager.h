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
#ifndef CRCORE_CRHANDLEMANAGER_H
#define CRCORE_CRHANDLEMANAGER_H 1

#include <CRCore/crExport.h>
#include <CRCore/Referenced.h>	
#include <CRCore/ref_ptr.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/thread/crCondition.h>
#include <map>
#include <string>
#include <vector>
#include <set>

namespace CRCore {
class crState;
#define HandleClass(library,name) \
	virtual CRCore::crHandle* cloneType() const { return new cr##name##Handle(); } \
	virtual CRCore::crHandle* clone() const { return new cr##name##Handle(*this); } \
	virtual bool isSameKindAs(const CRCore::crHandle* obj) const { return dynamic_cast<const cr##name##Handle *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Handle"; }\
	virtual const char* _name() const { return #name; }

#define MethodClass(library,name) \
	virtual CRCore::crHandle* cloneType() const { return new cr##name##Method(); } \
	virtual CRCore::crHandle* clone() const { return new cr##name##Method(*this); } \
	virtual bool isSameKindAs(const CRCore::crHandle* obj) const { return dynamic_cast<const cr##name##Method *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Method"; }\
	virtual const char* _name() const { return #name; }

#define LogicClass(library,name) \
	virtual CRCore::crHandle* cloneType() const { return new cr##name##Logic(); } \
	virtual CRCore::crHandle* clone() const { return new cr##name##Logic(*this); } \
	virtual bool isSameKindAs(const CRCore::crHandle* obj) const { return dynamic_cast<const cr##name##Logic *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Logic"; }\
	virtual const char* _name() const { return #name; }

#define DriveClass(library,name) \
	virtual CRCore::crHandle* cloneType() const { return new cr##name##Drive(); } \
	virtual CRCore::crHandle* clone() const { return new cr##name##Drive(*this); } \
	virtual bool isSameKindAs(const CRCore::crHandle* obj) const { return dynamic_cast<const cr##name##Drive *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return "cr"#name"Drive"; }\
	virtual const char* _name() const { return #name; }

	class crData;
	class CR_EXPORT crHandle : public Referenced
	{
	public :
		crHandle();
		virtual crHandle* cloneType() const = 0;
		virtual crHandle* clone() const = 0;
		virtual bool isSameKindAs(const crHandle* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crHandle(const crHandle& handle):Referenced(){}

		virtual void operator()(crData &data)=0;
		virtual void operator()(crHandle &handle)=0;

		virtual void inputHandle(int i, void *param)=0;
		virtual void inputParam(int i, void *param)=0;
		virtual void outputParam(int i, void *param)=0;
		virtual void getOutputParam(int i, void*& param)=0;

		virtual void addParam(int i, const std::string& str)=0;

		virtual crHandle *getHandle(const std::string& name){ return name.compare(_name())==0?this:NULL; }
		virtual void releaseObjects(crState* state= 0){}
		virtual void swapBuffers(int frameNumber){}
	protected :
		virtual ~crHandle();
	};
	class CR_EXPORT crMethod : public crHandle
	{
	public:
		crMethod(){}
		virtual crHandle* cloneType() const = 0;
		virtual crHandle* clone() const = 0;
		virtual bool isSameKindAs(const crHandle* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crMethod(const crMethod& method):crHandle(method){}

		virtual void operator()(crHandle &handle)=0;
		virtual void inputParam(int i, void *param) = 0;
		virtual void addParam(int i, const std::string& str){}
	private:
		virtual void operator()(crData &data){}
		virtual void inputHandle(int i, void *param){}
		virtual void outputParam(int i, void *param){}
		virtual void getOutputParam(int i, void*& param){}
	};
	class CR_EXPORT crLogic : public crHandle
	{
	public:
		crLogic(){}
		virtual crHandle* cloneType() const = 0;
		virtual crHandle* clone() const = 0;
		virtual bool isSameKindAs(const crHandle* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crLogic(const crLogic& logic):crHandle(logic){}

		virtual void operator()(crHandle &handle)=0;
		virtual void inputHandle(int i, void *param) = 0;
		virtual void inputParam(int i, void *param) = 0;
		virtual void outputParam(int i, void *param) = 0;
		virtual void addParam(int i, const std::string& str){}
	private:
		virtual void operator()(crData &data){}
		virtual void getOutputParam(int i, void*& param){}
	};
	class CR_EXPORT crDrive : public crHandle
	{
	public:
		crDrive(){}
		virtual crHandle* cloneType() const = 0;
		virtual crHandle* clone() const = 0;
		virtual bool isSameKindAs(const crHandle* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;

		crDrive(const crDrive& drive):crHandle(drive){}

		virtual void operator()(crData &data)=0;
		virtual void inputHandle(int i, void *param) = 0;
		virtual void outputParam(int i, void *param) = 0;
		virtual void getOutputParam(int i, void*& param) = 0;
		virtual void inputParam(int i, void *param) {/* m_inputParamVec.push_back(InputParamPair(i,param)); */}
		virtual void addParam(int i, const std::string& str){}
		virtual void releaseObjects(crState* state= 0){}
		virtual void swapBuffers(int frameNumber){}
	//protected:
		//typedef std::pair<int,void*>InputParamPair;
		//typedef std::vector<InputParamPair> InputParamVec;
		//InputParamVec m_inputParamVec;
		//void clearInputParam() { m_inputParamVec.clear(); }
	private:
		virtual void operator()(crHandle &handle){}
	};
	typedef std::vector< CRCore::ref_ptr<CRCore::crHandle> > HandleVec;
	class CR_EXPORT crHandleManager : public Referenced
	{
	public :
		crHandleManager(){};
	    static crHandleManager *getInstance();
        
		static void RegisterHandleProxy(crHandle *handle){ getInstance()->_register(handle->_name(),handle); }

		crHandle *getHandle(const std::string &name);
		virtual void clear();
		typedef std::map<crHandle*,int> ExecuteHandleMap;
		//typedef std::set<crHandle*> ExecuteHandleSet;
		void waitToExecuteHandle(crHandle *handle);
		void endExecuteHandle(crHandle *handle);
	protected :
		virtual ~crHandleManager() {}
		static ref_ptr<crHandleManager> m_instance;

		void _register(const std::string &name, crHandle *handle);

		typedef std::map< const std::string, ref_ptr<crHandle> > HandleMap;
		HandleMap m_handleMap;
		ExecuteHandleMap m_executeHandleMap;
		crCriticalMutex m_executeMutex;
		//crMutex m_waitmutex;
		//CRCore::crCondition m_condition;
	};

	class CR_EXPORT crOneSlotDrive : public CRCore::crDrive
	{
	public:
		crOneSlotDrive();
		crOneSlotDrive(const crOneSlotDrive& handle);
		DriveClass(CRCore, OneSlot)
		virtual void operator()(crData &data);
		virtual void inputHandle(int i, void *param);
		virtual void inputParam(int i, void *param);
		virtual void outputParam(int i, void *param);
		virtual void getOutputParam(int i, void*& param);
		virtual crHandle *getHandle(const std::string& name);
		virtual void releaseObjects(crState* state= 0);
		virtual void swapBuffers(int frameNumber);
	protected:
	    CRCore::ref_ptr<CRCore::crHandle> m_handle;
	};

	class CR_EXPORT crMoreSlotDrive : public CRCore::crDrive
	{
	public:
		crMoreSlotDrive();
		crMoreSlotDrive(const crMoreSlotDrive& handle);
		DriveClass(CRCore, MoreSlot)
		virtual void operator()(crData &data);
		virtual void inputHandle(int i, void *param);
		virtual void inputParam(int i, void *param);
		virtual void outputParam(int i, void *param);
		virtual void getOutputParam(int i, void*& param);
		virtual crHandle *getHandle(const std::string& name);
		virtual void releaseObjects(crState* state= 0);
		virtual void swapBuffers(int frameNumber);
	protected:
		HandleVec m_handleVec;
		bool m_taskCanceled;
	};
    ////////////Node
	//class CR_EXPORT crNodeDrive : public crDrive
	//{
	//public:
	//	crNodeDrive():m_swapFrameNumber(0){}
	//	crNodeDrive(const crNodeDrive& drive):crDrive(drive),m_swapFrameNumber(0){}
	//	virtual void releaseObjects(crState* state= 0){}
	//	virtual void swapBuffers(int frameNumber){}
	//protected:
	//	int m_swapFrameNumber;
	//};
	class CR_EXPORT crNodeOneSlotDrive : public CRCore::crOneSlotDrive
	{
	public:
		crNodeOneSlotDrive(){}
		crNodeOneSlotDrive(const crNodeOneSlotDrive& handle):crOneSlotDrive(handle){}
		DriveClass(CRCore, NodeOneSlot)
	};

	class CR_EXPORT crNodeMoreSlotDrive : public CRCore::crMoreSlotDrive
	{
	public:
		crNodeMoreSlotDrive(){}
		crNodeMoreSlotDrive(const crNodeMoreSlotDrive& handle):crMoreSlotDrive(handle){}
		DriveClass(CRCore, NodeMoreSlot)
	};
	typedef crMethod crNodeMethod;
	typedef crLogic crNodeLogic;
	//class CR_EXPORT crNodeMethod : public crMethod
	//{
	//public:
	//	crNodeMethod(){}
	//	crNodeMethod(const crNodeMethod& method):crMethod(method){}
	//};
	//class CR_EXPORT crNodeLogic : public crLogic
	//{
	//public:
	//	crNodeLogic(){}
	//	crNodeLogic(const crNodeLogic& logic):crLogic(logic){}
	//};
}

#endif
