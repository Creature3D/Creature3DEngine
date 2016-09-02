/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#ifndef CRCORE_CRUNIVERSALHANDLES_H
#define CRCORE_CRUNIVERSALHANDLES_H 1

#include <CRCore/crExport.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crNode.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crThread.h>
#include <vector>

namespace CRCore{

class CR_EXPORT crProxyLogic : public CRCore::crNodeLogic
{//代理，用于将Method转换为Logic
public:
	crProxyLogic();
	crProxyLogic(const crProxyLogic& handle);
	LogicClass(CRCore, Proxy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name){ return m_handle->getHandle(name); }
	crHandle *getHandle(){ return m_handle.get(); }
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
};

class CR_EXPORT crProxyArrayLogic : public CRCore::crNodeLogic
{//
public:
	crProxyArrayLogic();
	crProxyArrayLogic(const crProxyArrayLogic& handle);
	LogicClass(CRCore, ProxyArray)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	HandleVec &getHandleVec(){ return m_handles; }
protected:
	HandleVec m_handles;
};

class CR_EXPORT crSequenceLogic : public CRCore::crNodeLogic
{//
public:
	crSequenceLogic();
	crSequenceLogic(const crSequenceLogic& handle);
	LogicClass(CRCore, Sequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	HandleVec &getHandleVec(){ return m_handles; }
protected:
	HandleVec m_handles;
	bool m_cancel;
};

class CR_EXPORT crIF_ELSELogic : public CRCore::crNodeLogic
{//
public:
	crIF_ELSELogic();
	crIF_ELSELogic(const crIF_ELSELogic& handle);
	LogicClass(CRCore, IF_ELSE)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	crHandle *getConditionHandle(){ return m_conditionHandle.get(); }
	crHandle *getTrueHandle(){ return m_trueHandle.get(); }
	crHandle *getFalseHandle(){ return m_falseHandle.get(); }
protected:
	bool m_not;
	bool m_true;
	CRCore::ref_ptr<CRCore::crHandle> m_conditionHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_trueHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_falseHandle;
};

class CR_EXPORT crSwitchLogic : public CRCore::crNodeLogic
{//
public:
	crSwitchLogic();
	crSwitchLogic(const crSwitchLogic& handle);
	LogicClass(CRCore, Switch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	HandleVec &getHandleVec(){ return m_handleVec; }
	crHandle *getCaseHandle(){ return m_caseHandle.get(); }
protected:
	int m_case;
	CRCore::ref_ptr<CRCore::crHandle> m_caseHandle;
	HandleVec m_handleVec;
};

class CR_EXPORT crWhileLogic : public CRCore::crNodeLogic
{//
public:
	crWhileLogic();
	crWhileLogic(const crWhileLogic& handle);
	LogicClass(CRCore, While)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	crHandle *getConditionHandle(){ return m_conditionHandle.get(); }
protected:
	bool m_not;
	bool m_true;
	CRCore::ref_ptr<CRCore::crHandle> m_conditionHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
};

class CR_EXPORT crDoWhileLogic : public CRCore::crNodeLogic
{//
public:
	crDoWhileLogic();
	crDoWhileLogic(const crDoWhileLogic& handle);
	LogicClass(CRCore, DoWhile)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	crHandle *getConditionHandle(){ return m_conditionHandle.get(); }
protected:
	bool m_not;
	bool m_true;
	CRCore::ref_ptr<CRCore::crHandle> m_conditionHandle;
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
};

class CR_EXPORT crCountWhileLogic : public CRCore::crNodeLogic
{//
public:
	crCountWhileLogic();
	crCountWhileLogic(const crCountWhileLogic& handle);
	LogicClass(CRCore, CountWhile)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	virtual void releaseObjects(crState* state= 0);
	virtual void swapBuffers(int frameNumber);
	virtual crHandle *getHandle(const std::string& name);
	crHandle *getCountHandle(){ return m_countHandle.get(); }
protected:
	int m_count;//-1表示无限循环
	bool m_break;
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
	CRCore::ref_ptr<CRCore::crHandle> m_countHandle;
};
//Msg容器，用于储存eventmsg跨线程等待执行信号。
//使用方法如下：
//A线程执行node->doEvent(MAKEINT64(WCH_MSGCONTAINER,id),MAKEINT64(msg1,lparam1));
//B线程执行node->doEvent(MAKEINT64(WCH_MSGCONTAINER,id),MAKEINT64(msg2,lparam2));
//C线程执行node->doEvent(MAKEINT64(WCH_MSGCONTAINER,id),MAKEINT64(WCH_EXCEVENT,hparam));
//C线程将自动执行node->doEvent(msg1,MAKEINT64(lparam1,hparam)); node->doEvent(msg2,MAKEINT64(lparam2,hparam));
class CR_EXPORT crBaseEventContainerMethod : public CRCore::crMethod
{//
public:
	crBaseEventContainerMethod();
	crBaseEventContainerMethod(const crBaseEventContainerMethod& handle);
	MethodClass(CRCore, BaseEventContainer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crBase* m_this;
	_crInt64 m_param;

	typedef std::vector<_crInt64> EventList;
	EventList m_eventList;
	EventList m_eventList_buf;
	crMutex m_eventListMutex;
	crMutex m_eventListBufMutex;
};
class CR_EXPORT crSleepMethod : public CRCore::crMethod
{//
public:
	crSleepMethod();
	crSleepMethod(const crSleepMethod& handle);
	MethodClass(CRCore, Sleep)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_ms;
};
class crNullMethod : public CRCore::crMethod
{//
public:
	crNullMethod();
	crNullMethod(const crNullMethod& handle);
	MethodClass(CRCore, Null)
	virtual void operator()(crHandle &handle){}
	virtual void inputParam(int i, void *param){}
	virtual void addParam(int i, const std::string& str){}
};
}
#endif