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

#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crState.h>
#include <CRCore/crUniversalHandles.h>
//#include <gnelib.h>
using namespace CRCore;
crHandle::crHandle()
{
}
crHandle::~crHandle()
{
}
ref_ptr<crHandleManager> crHandleManager::m_instance = NULL;

//static GNE::Mutex  s_executeMutex;
//static GNE::ConditionVariable s_condition;
crHandleManager* crHandleManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crHandleManager;
		crHandleManager::RegisterHandleProxy(new crOneSlotDrive);
		crHandleManager::RegisterHandleProxy(new crMoreSlotDrive);
		crHandleManager::RegisterHandleProxy(new crNodeOneSlotDrive);
		crHandleManager::RegisterHandleProxy(new crNodeMoreSlotDrive);

		crHandleManager::RegisterHandleProxy(new crProxyLogic);
		crHandleManager::RegisterHandleProxy(new crProxyArrayLogic);
		crHandleManager::RegisterHandleProxy(new crSequenceLogic);
		crHandleManager::RegisterHandleProxy(new crIF_ELSELogic);
		crHandleManager::RegisterHandleProxy(new crSwitchLogic);
		crHandleManager::RegisterHandleProxy(new crWhileLogic);
		crHandleManager::RegisterHandleProxy(new crDoWhileLogic);
		crHandleManager::RegisterHandleProxy(new crCountWhileLogic);
		crHandleManager::RegisterHandleProxy(new crBaseEventContainerMethod);
		crHandleManager::RegisterHandleProxy(new crSleepMethod);
		crHandleManager::RegisterHandleProxy(new crNullMethod);
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crHandleManager::clear()
{
    m_instance = NULL;
}

crHandle *crHandleManager::getHandle(const std::string &name)
{
	HandleMap::iterator itr = m_handleMap.find(name);
	if(itr != m_handleMap.end())
		return itr->second->clone();
	return 0;
}

void crHandleManager::_register(const std::string &name, crHandle *handle)
{
	if(m_handleMap.find(name)==m_handleMap.end())
		m_handleMap[name] = handle;
	else
		CRCore::notify(CRCore::ALWAYS)<<"Handle["<<name<<"]已经存在了"<<std::endl;
}
void crHandleManager::waitToExecuteHandle(crHandle *handle)
{
	int id = CRCore::crThread::getCurrentThreadId();
	ExecuteHandleMap::iterator itr;
	while(1)
	{
		m_executeMutex.acquire();
		itr = m_executeHandleMap.find(handle);
		if(itr == m_executeHandleMap.end())
		{
			m_executeHandleMap[handle] = id;
			m_executeMutex.release();
			break;
		}
		else if(itr->second == id)
		{
			m_executeMutex.release();
			break;
		}
		m_executeMutex.release();
		crThread::sleep(1);
		//m_condition.wait(&m_waitmutex);
	}
}
void crHandleManager::endExecuteHandle(crHandle *handle)
{
	m_executeMutex.acquire();
	m_executeHandleMap.erase(handle);
	m_executeMutex.release();
	////m_condition.signal();
	//m_condition.broadcast();
}
/////////////////////////////////////////
//
//crOneSlotDrive
//
/////////////////////////////////////////
crOneSlotDrive::crOneSlotDrive(){}
crOneSlotDrive::crOneSlotDrive(const crOneSlotDrive& handle):
crDrive(handle)
{
	if(handle.m_handle.valid())
	    m_handle = handle.m_handle->clone();
}
void crOneSlotDrive::operator()(crData &data)
{
	if(m_handle.valid())
	{
		//crHandleManager::getInstance()->waitToExecuteHandle(m_handle.get());
		//for( InputParamVec::iterator ipitr = m_inputParamVec.begin();
		//	 ipitr != m_inputParamVec.end();
		//	 ++ipitr )
		//{
		//	m_handle->inputParam(ipitr->first,ipitr->second);
		//}
		//m_handle->inputParam(0,&data);
		(*m_handle)(*this);
		//m_handle->inputParam(0,NULL);
		//crHandleManager::getInstance()->endExecuteHandle(m_handle.get());
	}
	//clearInputParam();
}
void crOneSlotDrive::inputHandle(int i, void *param)
{
	if(param!=NULL) 
		m_handle = (CRCore::crHandle *)param;
}
void crOneSlotDrive::inputParam(int i, void *param)
{
	if(m_handle.valid())
        m_handle->inputParam(i,param);
}
void crOneSlotDrive::outputParam(int i, void *param)
{
}
void crOneSlotDrive::getOutputParam(int i, void*& param)
{
}
crHandle *crOneSlotDrive::getHandle(const std::string& name)
{
	crHandle *handle = crHandle::getHandle(name);
	if(!handle && m_handle.valid()) handle = m_handle->getHandle(name);

	return handle;	
}
void crOneSlotDrive::releaseObjects(crState* state)
{
	if(m_handle.valid())
	{
		m_handle->releaseObjects(state);
	}
}
void crOneSlotDrive::swapBuffers(int frameNumber)
{
	if(m_handle.valid())
	{
		m_handle->swapBuffers(frameNumber);
	}
}
/////////////////////////////////////////
//
//crMoreSlotDrive
//
/////////////////////////////////////////
crMoreSlotDrive::crMoreSlotDrive():m_taskCanceled(false){}
crMoreSlotDrive::crMoreSlotDrive(const crMoreSlotDrive& handle):
crDrive(handle),
m_taskCanceled(false)
{
	for( HandleVec::const_iterator itr = handle.m_handleVec.begin();
		itr != handle.m_handleVec.end();
		++itr )
	{
		m_handleVec.push_back((*itr)->clone());
	}
}
void crMoreSlotDrive::operator()(crData &data)
{
	m_taskCanceled = false;
	for( HandleVec::iterator itr = m_handleVec.begin();
		itr != m_handleVec.end();
		++itr )
	{
		//crHandleManager::getInstance()->waitToExecuteHandle(itr->get());
		//for( InputParamVec::iterator ipitr = m_inputParamVec.begin();
		//	ipitr != m_inputParamVec.end();
		//	++ipitr )
		//{
		//	(*itr)->inputParam(ipitr->first,ipitr->second);
		//}
		//(*itr)->inputParam(0,&data);
		(*(itr->get()))(*this);
		//(*itr)->inputParam(0,NULL);
		//crHandleManager::getInstance()->endExecuteHandle(itr->get());
		if(m_taskCanceled) break;
	}
	//clearInputParam();
}
void crMoreSlotDrive::inputHandle(int i, void *param)
{
	if(param!=NULL) 
		m_handleVec.push_back((CRCore::crHandle *)param);
}
void crMoreSlotDrive::inputParam(int i, void *param)
{
	for( HandleVec::iterator itr = m_handleVec.begin();
		itr != m_handleVec.end();
		++itr )
	{
		(*itr)->inputParam(i,param);
	}
}
void crMoreSlotDrive::outputParam(int i, void *param)
{
	if(i==0) m_taskCanceled = *((bool *)param);
}
void crMoreSlotDrive::getOutputParam(int i, void*& param)
{
}
crHandle *crMoreSlotDrive::getHandle(const std::string& name)
{
	crHandle *handle = crHandle::getHandle(name);
	if(!handle)
	{
		for( HandleVec::iterator itr = m_handleVec.begin();
			itr != m_handleVec.end();
			++itr )
		{
			handle = (*itr)->getHandle(name);
			if(handle) break;
		}
	}
	return handle;	
}
void crMoreSlotDrive::releaseObjects(crState* state)
{
	for( HandleVec::iterator itr = m_handleVec.begin();
		itr != m_handleVec.end();
		++itr )
	{
		(*itr)->releaseObjects(state);
	}
}
void crMoreSlotDrive::swapBuffers(int frameNumber)
{
	for( HandleVec::iterator itr = m_handleVec.begin();
		itr != m_handleVec.end();
		++itr )
	{
		(*itr)->swapBuffers(frameNumber);
	}
}
/////////Node
///////////////////////////////////////////
////
////crNodeOneSlotDrive
////
///////////////////////////////////////////
//crNodeOneSlotDrive::crNodeOneSlotDrive(){}
//crNodeOneSlotDrive::crNodeOneSlotDrive(const crNodeOneSlotDrive& handle):
//crNodeDrive(handle)
//{
//	if(handle.m_handle.valid())
//		m_handle = handle.m_handle->clone();
//}
//void crNodeOneSlotDrive::operator()(crData &data)
//{
//	if(m_handle.valid())
//	{
//		//crHandleManager::getInstance()->waitToExecuteHandle(m_handle.get());
//		//for( InputParamVec::iterator ipitr = m_inputParamVec.begin();
//		//	ipitr != m_inputParamVec.end();
//		//	++ipitr )
//		//{
//		//	m_handle->inputParam(ipitr->first,ipitr->second);
//		//}
//		m_handle->inputParam(0,&data);
//		(*m_handle)(*this);
//		m_handle->inputParam(0,NULL);
//		//crHandleManager::getInstance()->endExecuteHandle(m_handle.get());
//	}
//	//clearInputParam();
//}
//void crNodeOneSlotDrive::inputHandle(int i, void *param)
//{
//	if(param!=NULL) 
//		m_handle = (CRCore::crHandle *)param;
//}
//void crNodeOneSlotDrive::inputParam(int i, void *param)
//{
//	if(m_handle.valid())
//		m_handle->inputParam(i,param);
//}
//void crNodeOneSlotDrive::outputParam(int i, void *param)
//{
//}
//void crNodeOneSlotDrive::getOutputParam(int i, void*& param)
//{
//}
//crHandle *crNodeOneSlotDrive::getHandle(const std::string& name)
//{
//	crHandle *handle = crHandle::getHandle(name);
//	if(!handle && m_handle.valid()) handle = m_handle->getHandle(name);
//
//	return handle;	
//}
//void crNodeOneSlotDrive::releaseObjects(crState* state)
//{
//	if(m_handle.valid())
//	{
//		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
//		{
//			(dynamic_cast<crNodeMethod *>(m_handle.get()))->releaseObjects(state);
//		}
//		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
//		{
//			(dynamic_cast<crNodeLogic *>(m_handle.get()))->releaseObjects(state);
//		}
//	}
//}
//void crNodeOneSlotDrive::swapBuffers(int frameNumber)
//{
//	if(m_swapFrameNumber != frameNumber)
//	{
//		if(m_handle.valid())
//		{
//			if(dynamic_cast<crNodeMethod *>(m_handle.get()))
//			{
//				(dynamic_cast<crNodeMethod *>(m_handle.get()))->swapBuffers(frameNumber);
//			}
//			else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
//			{
//				(dynamic_cast<crNodeLogic *>(m_handle.get()))->swapBuffers(frameNumber);
//			}
//		}
//		m_swapFrameNumber = frameNumber;
//	}
//}
///////////////////////////////////////////
////
////crNodeMoreSlotDrive
////
///////////////////////////////////////////
//crNodeMoreSlotDrive::crNodeMoreSlotDrive():m_taskCanceled(false){}
//crNodeMoreSlotDrive::crNodeMoreSlotDrive(const crNodeMoreSlotDrive& handle):
//crNodeDrive(handle),
//m_taskCanceled(false)
//{
//	for( HandleVec::const_iterator itr = handle.m_handleVec.begin();
//		itr != handle.m_handleVec.end();
//		++itr )
//	{
//		m_handleVec.push_back((*itr)->clone());
//	}
//}
//void crNodeMoreSlotDrive::operator()(crData &data)
//{
//	m_taskCanceled = false;
//	for( HandleVec::iterator itr = m_handleVec.begin();
//		itr != m_handleVec.end();
//		++itr )
//	{
//		//crHandleManager::getInstance()->waitToExecuteHandle(itr->get());
//		//for( InputParamVec::iterator ipitr = m_inputParamVec.begin();
//		//	ipitr != m_inputParamVec.end();
//		//	++ipitr )
//		//{
//		//	(*itr)->inputParam(ipitr->first,ipitr->second);
//		//}
//		(*itr)->inputParam(0,&data);
//		(*(itr->get()))(*this);
//		(*itr)->inputParam(0,NULL);
//		//crHandleManager::getInstance()->endExecuteHandle(itr->get());
//		if(m_taskCanceled) break;
//	}
//	//clearInputParam();
//}
//void crNodeMoreSlotDrive::inputHandle(int i, void *param)
//{
//	if(param!=NULL) 
//		m_handleVec.push_back((CRCore::crHandle *)param);
//}
//void crNodeMoreSlotDrive::inputParam(int i, void *param)
//{
//	for( HandleVec::iterator itr = m_handleVec.begin();
//		itr != m_handleVec.end();
//		++itr )
//	{
//		(*itr)->inputParam(i,param);
//	}
//}
//void crNodeMoreSlotDrive::outputParam(int i, void *param)
//{
//	if(i==0) m_taskCanceled = *((bool *)param);
//}
//void crNodeMoreSlotDrive::getOutputParam(int i, void*& param)
//{
//}
//crHandle *crNodeMoreSlotDrive::getHandle(const std::string& name)
//{
//	crHandle *handle = crHandle::getHandle(name);
//	if(!handle)
//	{
//		for( HandleVec::iterator itr = m_handleVec.begin();
//			itr != m_handleVec.end();
//			++itr )
//		{
//			handle = (*itr)->getHandle(name);
//			if(handle) break;
//		}
//	}
//	return handle;	
//}
//void crNodeMoreSlotDrive::releaseObjects(crState* state)
//{
//	for( HandleVec::iterator itr = m_handleVec.begin();
//		itr != m_handleVec.end();
//		++itr )
//	{
//		if(dynamic_cast<crNodeMethod *>(itr->get()))
//		{
//			(dynamic_cast<crNodeMethod *>(itr->get()))->releaseObjects(state);
//		}
//		else if(dynamic_cast<crNodeLogic *>(itr->get()))
//		{
//			(dynamic_cast<crNodeLogic *>(itr->get()))->releaseObjects(state);
//		}
//	}
//}
//void crNodeMoreSlotDrive::swapBuffers(int frameNumber)
//{
//	if(m_swapFrameNumber != frameNumber)
//	{
//		for( HandleVec::iterator itr = m_handleVec.begin();
//			itr != m_handleVec.end();
//			++itr )
//		{
//			if(dynamic_cast<crNodeMethod *>(itr->get()))
//			{
//				(dynamic_cast<crNodeMethod *>(itr->get()))->swapBuffers(frameNumber);
//			}
//			else if(dynamic_cast<crNodeLogic *>(itr->get()))
//			{
//				(dynamic_cast<crNodeLogic *>(itr->get()))->swapBuffers(frameNumber);
//			}
//		}
//		m_swapFrameNumber = frameNumber;
//	}
//}