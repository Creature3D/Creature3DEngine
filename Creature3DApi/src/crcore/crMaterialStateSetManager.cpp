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
#include <CRCore/crMaterialStateSetManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
using namespace CRCore;
ref_ptr<crMaterialStateSetManager> crMaterialStateSetManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crMaterialStateSetManager::crMaterialStateSetManager()
{
}

crMaterialStateSetManager* crMaterialStateSetManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crMaterialStateSetManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
	//static ref_ptr<crMaterialStateSetManager> s_manager = new crMaterialStateSetManager;
	//return s_manager.get();
}

void crMaterialStateSetManager::clear()
{
	m_instance = NULL;
}

void crMaterialStateSetManager::insertMaterial( crStateSet *stateset )
{
	if(stateset)
	{
		if(stateset->getName().empty())
		{
			CRCore::notify(CRCore::WARN)<<"crMaterialStateSetManager::insertMaterial(): 材质没有被命名"<<std::endl;
			return;
		}
		if(m_materialStateSetMap.find(stateset->getName()) != m_materialStateSetMap.end())
		{
			CRCore::notify(CRCore::WARN)<<"crMaterialStateSetManager::insertMaterial(): 同名材质已经存在"<<std::endl;
			return;
		}
		for( MaterialStateSetMap::iterator itr = m_materialStateSetMap.begin();
			 itr != m_materialStateSetMap.end();
			 itr++ )
		{
			if(itr->second->compare(*stateset,true) == 0)
			{
				CRCore::notify(CRCore::WARN)<<"crMaterialStateSetManager::insertMaterial(): 属性相同材质已经存在，已存在的材质名为："<<itr->first<<std::endl;
				return;
			}
		}
	    m_materialStateSetMap[stateset->getName()] = stateset;
	}
}

crStateSet *crMaterialStateSetManager::getMaterial( const std::string& name )
{
	MaterialStateSetMap::iterator itr = m_materialStateSetMap.find(name);
	if(itr != m_materialStateSetMap.end()) return itr->second.get();
	else return NULL;
}
