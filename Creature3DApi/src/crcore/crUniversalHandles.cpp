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
#include <CRCore/crUniversalHandles.h>
using namespace CRCore;
/////////////////////////////////////////
//
//crProxyLogic
//
/////////////////////////////////////////
crProxyLogic::crProxyLogic()
{
}

crProxyLogic::crProxyLogic(const crProxyLogic& handle):
crNodeLogic(handle)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crProxyLogic::inputParam(int i, void *param)
{
	if(m_handle.valid())
	{
		m_handle->inputParam(i,param);
	}
}

void crProxyLogic::addParam(int i, const std::string& str)
{
}

void crProxyLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crProxyLogic::outputParam(int i, void *param)
{
}

void crProxyLogic::operator()(crHandle &handle)
{
	if(m_handle.valid())
	{
		(*m_handle)(handle);
	}
}
void crProxyLogic::releaseObjects(crState* state)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->releaseObjects(state);
		}
	}
}
void crProxyLogic::swapBuffers(int frameNumber)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->swapBuffers(frameNumber);
		}
	}
}
/////////////////////////////////////////
//
//crProxyArrayLogic
//
/////////////////////////////////////////
crProxyArrayLogic::crProxyArrayLogic()
{
}

crProxyArrayLogic::crProxyArrayLogic(const crProxyArrayLogic& handle):
crNodeLogic(handle)
{
	for( HandleVec::const_iterator itr = handle.m_handles.begin(); 
		 itr != handle.m_handles.end();
		 ++itr )
	{
        m_handles.push_back((*itr)->clone());
	}
}

void crProxyArrayLogic::inputParam(int i, void *param)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		(*itr)->inputParam(i,param);
	}
}

void crProxyArrayLogic::addParam(int i, const std::string& str)
{
}

void crProxyArrayLogic::inputHandle(int i, void *param)
{
	if(param)
	{
		m_handles.push_back((CRCore::crHandle *)param);
	}
}

void crProxyArrayLogic::outputParam(int i, void *param)
{
}

void crProxyArrayLogic::operator()(crHandle &handle)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		(*(itr->get()))(handle);
	}
}
void crProxyArrayLogic::releaseObjects(crState* state)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->releaseObjects(state);
		}
	}
}
void crProxyArrayLogic::swapBuffers(int frameNumber)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crProxyArrayLogic::getHandle(const std::string& name)
{
	crHandle *handle = crHandle::getHandle(name);
	if(!handle)
	{
		for( HandleVec::iterator itr = m_handles.begin();
			itr != m_handles.end();
			++itr )
		{
			handle = (*itr)->getHandle(name);
			if(handle) break;
		}
	}
	return handle;	
}
/////////////////////////////////////////
//
//crSequenceLogic
//
/////////////////////////////////////////
crSequenceLogic::crSequenceLogic():
m_cancel(false)
{
}

crSequenceLogic::crSequenceLogic(const crSequenceLogic& handle):
crNodeLogic(handle),
m_cancel(false)
{
	for( HandleVec::const_iterator itr = handle.m_handles.begin(); 
		itr != handle.m_handles.end();
		++itr )
	{
		m_handles.push_back((*itr)->clone());
	}
}

void crSequenceLogic::inputParam(int i, void *param)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		(*itr)->inputParam(i,param);
	}
}

void crSequenceLogic::addParam(int i, const std::string& str)
{
}

void crSequenceLogic::inputHandle(int i, void *param)
{
	if(param)
	{
		m_handles.push_back((CRCore::crHandle *)param);
	}
}

void crSequenceLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_cancel =*((bool*)param);
		break;
	}
}

void crSequenceLogic::operator()(crHandle &handle)
{
	m_cancel = false;
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		(*(itr->get()))(*this);
		if(m_cancel) break;
	}
}
void crSequenceLogic::releaseObjects(crState* state)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->releaseObjects(state);
		}
	}
}
void crSequenceLogic::swapBuffers(int frameNumber)
{
	for( HandleVec::iterator itr = m_handles.begin(); 
		itr != m_handles.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crSequenceLogic::getHandle(const std::string& name)
{
	crHandle *handle = crHandle::getHandle(name);
	if(!handle)
	{
		for( HandleVec::iterator itr = m_handles.begin();
			itr != m_handles.end();
			++itr )
		{
			handle = (*itr)->getHandle(name);
			if(handle) break;
		}
	}
	return handle;	
}
/////////////////////////////////////////
//
//crIF_ELSELogic
//
/////////////////////////////////////////
crIF_ELSELogic::crIF_ELSELogic():
m_not(false),
m_true(true)
{
}

crIF_ELSELogic::crIF_ELSELogic(const crIF_ELSELogic& handle):
crNodeLogic(handle),
m_not(handle.m_not),
m_true(handle.m_true)
{
	if(handle.m_conditionHandle.valid())
		m_conditionHandle = handle.m_conditionHandle->clone();
	if(handle.m_trueHandle.valid())
		m_trueHandle = handle.m_trueHandle->clone();
	if(handle.m_falseHandle.valid())
		m_falseHandle = handle.m_falseHandle->clone();
}

void crIF_ELSELogic::inputParam(int i, void *param)
{
	if(m_conditionHandle.valid())
	{
		m_conditionHandle->inputParam(i,param);
	}
	if(m_trueHandle.valid())
	{
		m_trueHandle->inputParam(i,param);
	}
	if(m_falseHandle.valid())
	{
		m_falseHandle->inputParam(i,param);
	}
}

void crIF_ELSELogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_not = (bool)(atoi(str.c_str()));
		break;
	}
}

void crIF_ELSELogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_conditionHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_trueHandle = (CRCore::crHandle *)param;
		break;
	case 2:
		m_falseHandle = (CRCore::crHandle *)param;
		break;
	}
}

void crIF_ELSELogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_true =*((bool*)param);
		m_true = m_not?!m_true:m_true;
		break;
	}
}

void crIF_ELSELogic::operator()(crHandle &handle)
{
	if(m_conditionHandle.valid())
	{
		(*m_conditionHandle)(*this);
		if(m_true)
		{
			if(m_trueHandle.valid()) (*m_trueHandle)(*this);
		}
		else
		{
			if(m_falseHandle.valid()) (*m_falseHandle)(*this);
		}
	}
}
void crIF_ELSELogic::releaseObjects(crState* state)
{
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->releaseObjects(state);
		}
	}
	if(m_trueHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_trueHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_trueHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_trueHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_trueHandle.get()))->releaseObjects(state);
		}
	}
	if(m_falseHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_falseHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_falseHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_falseHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_falseHandle.get()))->releaseObjects(state);
		}
	}
}
void crIF_ELSELogic::swapBuffers(int frameNumber)
{
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
	}
	if(m_trueHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_trueHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_trueHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_trueHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_trueHandle.get()))->swapBuffers(frameNumber);
		}
	}
	if(m_falseHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_falseHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_falseHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_falseHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_falseHandle.get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crIF_ELSELogic::getHandle(const std::string& name)
{
	crHandle *handle = NULL;
	do 
	{
		handle = crHandle::getHandle(name);
		if(handle) break;
		handle = m_conditionHandle->getHandle(name);
		if(handle) break;
		handle = m_trueHandle->getHandle(name);
		if(handle) break;
		handle = m_falseHandle->getHandle(name);
		if(handle) break;
	} while (0);
	return handle;	
}
/////////////////////////////////////////
//
//crSwitchLogic
//
/////////////////////////////////////////
crSwitchLogic::crSwitchLogic():
m_case(0)
{
}

crSwitchLogic::crSwitchLogic(const crSwitchLogic& handle):
crNodeLogic(handle),
m_case(handle.m_case)
{
	if(handle.m_caseHandle.valid())
		m_caseHandle = handle.m_caseHandle->clone();
	m_handleVec.resize(handle.m_handleVec.size());
	for( int i = 0; i<handle.m_handleVec.size(); ++i )
	{
		m_handleVec[i] = handle.m_handleVec[i]->clone();
	}
}

void crSwitchLogic::inputParam(int i, void *param)
{
	if(m_caseHandle.valid())
	{
		m_caseHandle->inputParam(i,param);
	}
	for( HandleVec::iterator itr = m_handleVec.begin();
		 itr != m_handleVec.end();
		 ++itr )
	{
		(*itr)->inputParam(i,param);
	}
}

void crSwitchLogic::addParam(int i, const std::string& str)
{
}

void crSwitchLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_caseHandle = (CRCore::crHandle *)param;
		break;
	default:
		m_handleVec.push_back((CRCore::crHandle *)param);
		break;
	}
}

void crSwitchLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_case =*((int*)param);
		break;
	}
}

void crSwitchLogic::operator()(crHandle &handle)
{
	if(m_caseHandle.valid())
	{
		m_case = 0;
		(*m_caseHandle)(*this);
		if(m_case>=0&&m_case<m_handleVec.size())
		{
			(*m_handleVec[m_case])(*this);
		}
	}
}
void crSwitchLogic::releaseObjects(crState* state)
{
	if(m_caseHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_caseHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_caseHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_caseHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_caseHandle.get()))->releaseObjects(state);
		}
	}
	for( HandleVec::iterator itr = m_handleVec.begin(); 
		itr != m_handleVec.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->releaseObjects(state);
		}
	}
}
void crSwitchLogic::swapBuffers(int frameNumber)
{
	if(m_caseHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_caseHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_caseHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_caseHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_caseHandle.get()))->swapBuffers(frameNumber);
		}
	}
	for( HandleVec::iterator itr = m_handleVec.begin(); 
		itr != m_handleVec.end();
		++itr )
	{
		if(dynamic_cast<crNodeMethod *>(itr->get()))
		{
			(dynamic_cast<crNodeMethod *>(itr->get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(itr->get()))
		{
			(dynamic_cast<crNodeLogic *>(itr->get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crSwitchLogic::getHandle(const std::string& name)
{
	crHandle *handle = NULL;
	do 
	{
		handle = crHandle::getHandle(name);
		if(handle) break;
		handle = m_caseHandle->getHandle(name);
		if(handle) break;
		for( HandleVec::iterator itr = m_handleVec.begin();
			itr != m_handleVec.end();
			++itr )
		{
			handle = (*itr)->getHandle(name);
			if(handle) break;
		}
	} while (0);
	return handle;	
}
/////////////////////////////////////////
//
//crWhileLogic
//
/////////////////////////////////////////
crWhileLogic::crWhileLogic():
m_not(false),
m_true(true)
{
}

crWhileLogic::crWhileLogic(const crWhileLogic& handle):
crNodeLogic(handle),
m_not(handle.m_not),
m_true(handle.m_true)
{
	if(handle.m_conditionHandle.valid())
		m_conditionHandle = handle.m_conditionHandle->clone();
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crWhileLogic::inputParam(int i, void *param)
{
	if(m_conditionHandle.valid())
	{
		m_conditionHandle->inputParam(i,param);
	}
	if(m_handle.valid())
	{
		m_handle->inputParam(i,param);
	}
}

void crWhileLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_not = (bool)(atoi(str.c_str()));
		break;
	}
}

void crWhileLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_conditionHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crWhileLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_true =*((bool*)param);
		m_true = m_not?!m_true:m_true;
		break;
	}
}

void crWhileLogic::operator()(crHandle &handle)
{
	if(m_handle.valid() && m_conditionHandle.valid())
	{
		(*m_conditionHandle)(*this);
		while(m_true)
		{
            (*m_handle)(*this);
			(*m_conditionHandle)(*this);
	    }
	}
}
void crWhileLogic::releaseObjects(crState* state)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->releaseObjects(state);
		}
	}
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->releaseObjects(state);
		}
	}
}
void crWhileLogic::swapBuffers(int frameNumber)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->swapBuffers(frameNumber);
		}
	}
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crWhileLogic::getHandle(const std::string& name)
{
	crHandle *handle = NULL;
	do 
	{
		handle = crHandle::getHandle(name);
		if(handle) break;
		handle = m_conditionHandle->getHandle(name);
		if(handle) break;
		handle = m_handle->getHandle(name);
		if(handle) break;
	} while (0);
	return handle;	
}
/////////////////////////////////////////
//
//crDoWhileLogic
//
/////////////////////////////////////////
crDoWhileLogic::crDoWhileLogic():
m_not(false),
m_true(true)
{
}

crDoWhileLogic::crDoWhileLogic(const crDoWhileLogic& handle):
crNodeLogic(handle),
m_not(handle.m_not),
m_true(handle.m_true)
{
	if(handle.m_conditionHandle.valid())
		m_conditionHandle = handle.m_conditionHandle->clone();
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crDoWhileLogic::inputParam(int i, void *param)
{
	if(m_conditionHandle.valid())
	{
		m_conditionHandle->inputParam(i,param);
	}
	if(m_handle.valid())
	{
		m_handle->inputParam(i,param);
	}
}

void crDoWhileLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_not = (bool)(atoi(str.c_str()));
		break;
	}
}

void crDoWhileLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_conditionHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crDoWhileLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_true =*((bool*)param);
		m_true = m_not?!m_true:m_true;
		break;
	}
}

void crDoWhileLogic::operator()(crHandle &handle)
{
	if(m_handle.valid() && m_conditionHandle.valid())
	{
		do
		{
			(*m_handle)(*this);
			(*m_conditionHandle)(*this);
		}while(m_true);
	}
}
void crDoWhileLogic::releaseObjects(crState* state)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->releaseObjects(state);
		}
	}
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->releaseObjects(state);
		}
	}
}
void crDoWhileLogic::swapBuffers(int frameNumber)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->swapBuffers(frameNumber);
		}
	}
	if(m_conditionHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_conditionHandle.get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crDoWhileLogic::getHandle(const std::string& name)
{
	crHandle *handle = NULL;
	do 
	{
		handle = crHandle::getHandle(name);
		if(handle) break;
		handle = m_conditionHandle->getHandle(name);
		if(handle) break;
		handle = m_handle->getHandle(name);
		if(handle) break;
	} while (0);
	return handle;	
}
/////////////////////////////////////////
//
//crCountWhileLogic
//
/////////////////////////////////////////
crCountWhileLogic::crCountWhileLogic():
m_count(-1),
m_break(false)
{
}

crCountWhileLogic::crCountWhileLogic(const crCountWhileLogic& handle):
crNodeLogic(handle),
m_count(handle.m_count),
m_break(handle.m_break)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
	if(handle.m_countHandle.valid())
		m_countHandle = handle.m_countHandle->clone();
}

void crCountWhileLogic::inputParam(int i, void *param)
{
	if(m_handle.valid())
	{
		m_handle->inputParam(i,param);
	}
	if(m_countHandle.valid())
	{
		m_countHandle->inputParam(i,param);
	}
}

void crCountWhileLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	}
}

void crCountWhileLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_handle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_countHandle = (CRCore::crHandle *)param;
		break;
	}
}

void crCountWhileLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_count =*((int*)param);
		break;
	case 1:
		m_break =*((bool*)param);
		break;
	}
}

void crCountWhileLogic::operator()(crHandle &handle)
{
	if(m_handle.valid())
	{
		if(m_countHandle.valid()) (*m_countHandle)(*this);
		for(int i = 0; !m_break&&(m_count==-1||i<m_count); ++i)
		{
			(*m_handle)(*this);
		    if(m_countHandle.valid()) (*m_countHandle)(*this);
		}
	}
}
void crCountWhileLogic::releaseObjects(crState* state)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->releaseObjects(state);
		}
	}
	if(m_countHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_countHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_countHandle.get()))->releaseObjects(state);
		}
		else if(dynamic_cast<crNodeLogic *>(m_countHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_countHandle.get()))->releaseObjects(state);
		}
	}
}
void crCountWhileLogic::swapBuffers(int frameNumber)
{
	if(m_handle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_handle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_handle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_handle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_handle.get()))->swapBuffers(frameNumber);
		}
	}
	if(m_countHandle.valid())
	{
		if(dynamic_cast<crNodeMethod *>(m_countHandle.get()))
		{
			(dynamic_cast<crNodeMethod *>(m_countHandle.get()))->swapBuffers(frameNumber);
		}
		else if(dynamic_cast<crNodeLogic *>(m_countHandle.get()))
		{
			(dynamic_cast<crNodeLogic *>(m_countHandle.get()))->swapBuffers(frameNumber);
		}
	}
}
crHandle *crCountWhileLogic::getHandle(const std::string& name)
{
	crHandle *handle = NULL;
	do 
	{
		handle = crHandle::getHandle(name);
		if(handle) break;
		handle = m_handle->getHandle(name);
		if(handle) break;
		handle = m_countHandle->getHandle(name);
		if(handle) break;
	} while (0);
	return handle;	
}
/////////////////////////////////////////
//
//crBaseEventContainerMethod
//
/////////////////////////////////////////
crBaseEventContainerMethod::crBaseEventContainerMethod()
{
}
crBaseEventContainerMethod::crBaseEventContainerMethod(const crBaseEventContainerMethod& handle):
crMethod(handle)
{
}
void crBaseEventContainerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crBase*)param;
		break;
	case 2:
		{
			CREPARAM _param = *(CREPARAM*)param;
			if(LOCREPARAM(_param)!=WCH_EXCEVENT)
			{
				m_eventListMutex.lock();
				m_eventList.push_back(_param);
				m_eventListMutex.unlock();
			}
			else
			{
				m_eventListBufMutex.lock();
                m_param = _param;
				m_eventListBufMutex.unlock();
			}
		}
		break;
	}
}

void crBaseEventContainerMethod::addParam(int i, const std::string& str)
{
}

void crBaseEventContainerMethod::operator()(crHandle &handle)
{
	if(m_this && LOCREPARAM(m_param)==WCH_EXCEVENT)
	{
		CRCore::ScopedLock<crMutex> lock(m_eventListBufMutex);

		m_eventListMutex.lock();
		int hparam = HICREPARAM(m_param);
		m_param = NULLPARAM;
		m_eventList_buf.swap(m_eventList);
		m_eventListMutex.unlock();

		CREPARAM param;
		for( EventList::iterator itr = m_eventList_buf.begin();
			 itr != m_eventList_buf.end();
		     ++itr )
		{
			param = *itr;
			m_this->doEvent(LOCREPARAM(param),MAKECREPARAM(HICREPARAM(param),hparam));
		}
		if(!m_eventList_buf.empty()) m_eventList_buf.clear();
	}
}
/////////////////////////////////////////
//
//crSleepMethod
//
/////////////////////////////////////////
crSleepMethod::crSleepMethod():
m_ms(0)
{
}
crSleepMethod::crSleepMethod(const crSleepMethod& handle):
crMethod(handle),
m_ms(handle.m_ms)
{
}
void crSleepMethod::inputParam(int i, void *param)
{
}

void crSleepMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_ms = atoi(str.c_str());
		break;
	}
}

void crSleepMethod::operator()(crHandle &handle)
{
	CRCore::crThread::sleep(m_ms);
}
/////////////////////////////////////////
//
//crNullMethod
//
/////////////////////////////////////////
crNullMethod::crNullMethod()
{
}
crNullMethod::crNullMethod(const crNullMethod& handle):
	crMethod(handle)
{
}