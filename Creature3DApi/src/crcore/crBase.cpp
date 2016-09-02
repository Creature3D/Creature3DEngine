/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by 吴财华

#include <CRCore/crBase.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

crBase::crBase(const crBase& obj,const crCopyOp& copyop):
    Referenced(),
    m_name(obj.m_name),
    m_dataVariance(obj.m_dataVariance),
	m_currentMsg(0),
	m_swapFrameNumber(0)
{
	if(obj.m_dataClass.valid())
		m_dataClass = copyop(obj.m_dataClass.get());
}

///////////////////////////////////////////////////////////
void crBase::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crBase::getDataClass()
{
	return m_dataClass.get();
}
void crBase::doEvent(_crInt64 kbmsg, _crInt64 param)
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
			catch(...)
			{
				CRCore::notify(CRCore::ALWAYS)<<"crBase::doEvent error msg = "<<kbmsg<<std::endl;
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}
crHandle* crBase::getHandle(__int64 kbmsg, const std::string& name)
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

bool crBase::hasHandle(__int64 kbmsg)
{
	if(m_dataClass.valid())
	{
		crHandle *handle = m_dataClass->getHandle(kbmsg);
		return handle != NULL;
	}
	return false;
}
void crBase::clearData()
{
	m_dataClass = NULL;
}
const CRCore::crData *crBase::getDataClass() const
{
	return m_dataClass.get();
}
void crBase::releaseObjects(CRCore::crState* state) 
{
	if(m_dataClass.valid() && dynamic_cast<crNodeData *>(m_dataClass.get()))
	{
		(dynamic_cast<crNodeData *>(m_dataClass.get()))->releaseObjects(state);
	}
	//m_dataClass->clear();
	clearData();
}
