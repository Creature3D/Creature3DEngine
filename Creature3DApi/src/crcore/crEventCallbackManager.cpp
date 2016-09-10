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

#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crBrain.h>
using namespace CRCore;
ref_ptr<crEventCallbackManager> crEventCallbackManager::m_instance = NULL;

crEventCallbackManager* crEventCallbackManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crEventCallbackManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crEventCallbackManager::clear()
{
    m_instance = NULL;
}

crEventCallback *crEventCallbackManager::getEventCallback(const std::string &name)
{
	EventCallbackMap::iterator itr = m_eventCallbackMap.find(name);
	if(itr != m_eventCallbackMap.end())
		return itr->second->clone(crCopyOp::DEEP_COPY_ALL);
	return 0;
}

void crEventCallbackManager::registerEventCallback(const std::string &name, crEventCallback *callback)
{
	if(m_eventCallbackMap.find(name)==m_eventCallbackMap.end())
		m_eventCallbackMap[name] = callback;
	else
		CRCore::notify(CRCore::ALWAYS)<<"EventCallback["<<name<<"]已经存在了"<<std::endl;
}

