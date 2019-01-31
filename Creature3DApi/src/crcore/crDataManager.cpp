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

#include <CRCore/crDataManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crState.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;
ref_ptr<crDataManager> crDataManager::m_instance = NULL;

crDataManager* crDataManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDataManager;
		crDataManager::RegisterDataProxy(new crEventData);
		crDataManager::RegisterDataProxy(new crNodeData);
		crDataManager::RegisterDataProxy(new crUIData);
		crDataManager::RegisterDataProxy(new crWindowUIData);
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crDataManager::clear()
{
    m_instance = NULL;
}

crData *crDataManager::getData(const std::string &name)
{
	DataMap::iterator itr = m_dataMap.find(name);
	if(itr != m_dataMap.end())
		return itr->second->clone(crCopyOp::DEEP_COPY_ALL);
	return 0;
}

void crDataManager::_register(const std::string &name, crData *callback)
{
	if(m_dataMap.find(name)==m_dataMap.end())
		m_dataMap[name] = callback;
	else
		CRCore::notify(CRCore::ALWAYS)<<"Data["<<name<<"]已经存在了"<<std::endl;
}

/////////////////////////////////////////
//
//crData
//
/////////////////////////////////////////
crData::crData():m_swapFrameNumber(0)
{
}
crData::~crData() 
{
}
void crData::releaseObjects(crState* state)
{
	for( HandleMap::iterator itr = m_handleMap.begin();
		itr != m_handleMap.end();
		++itr )
	{
		itr->second->releaseObjects(state);
	}
}
void crData::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		for( HandleMap::iterator itr = m_handleMap.begin();
			itr != m_handleMap.end();
			++itr )
		{
			itr->second->swapBuffers(frameNumber);
		}
		m_swapFrameNumber = frameNumber;
	}
}
/////////////////////////////////////////
//
//crDataObject
//
/////////////////////////////////////////
void crDataObject::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crDataObject::getDataClass()
{
	return m_dataClass.get();
}
void crDataObject::doEvent(_crInt64 kbmsg, CREPARAM param)
{///m_currentMsg并非多线程安全
	m_currentMsg = kbmsg;
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"crDataObject::doEvent error msg = "<<kbmsg<<std::endl;
				char gbuf[256];
				sprintf(gbuf, "crDataObject::doEvent error msg=%s,DataName=%s,DataClassName=%s\n\0", crArgumentParser::appI64toa(kbmsg).c_str(), data->_name(), data->className());
				gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}
crHandle* crDataObject::getHandle(__int64 kbmsg, const std::string& name)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		if(handle)
		{
			handle = handle->getHandle(name);
		}
		if(handle) return handle;
	}
	return NULL;
}

bool crDataObject::hasHandle(__int64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crDataObject::clearData()
{
	m_dataClass = NULL;
}