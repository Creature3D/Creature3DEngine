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
#include <CRCore/crEnvironmentManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
using namespace CRCore;
ref_ptr<crEnvironmentManager> crEnvironmentManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crEnvironmentManager::crEnvironmentManager()
{
}

crEnvironmentManager* crEnvironmentManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crEnvironmentManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();

	//static ref_ptr<crEnvironmentManager> s_manager = new crEnvironmentManager;
	//return s_manager.get();
}

void crEnvironmentManager::clear()
{
	m_instance = NULL;
}

void crEnvironmentManager::insertEnvironmentMap( const crVector3f &local, crTextureCubeMap *env )
{
	m_envMap[local] = env;
}

crTextureCubeMap *crEnvironmentManager::getSuitableEnvMap( const crVector3f &local )
{//返回最近的环境贴图
    crTextureCubeMap *env = 0;
	float mindistance = FLT_MAX;
	float distance;
	for( EnvironmentMap::iterator itr = m_envMap.begin();
		 itr != m_envMap.end();
		 ++itr )
	{
        distance = (local - itr->first).length();
		if(distance<mindistance)
		{
			mindistance = distance;
            env = itr->second.get();
		}
	}
    return env;
}

void crEnvironmentManager::releaseObjects(crState* state)
{
	for( EnvironmentMap::iterator itr = m_envMap.begin();
		itr != m_envMap.end();
		++itr )
	{
		itr->second->releaseObjects(state);
	}
}

crEnvironmentManager::EnvironmentMap &crEnvironmentManager::getEnvMap()
{
	return m_envMap;
}