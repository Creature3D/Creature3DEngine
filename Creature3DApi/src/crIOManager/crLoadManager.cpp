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
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crReadFile.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <sys/stat.h>
using namespace CRIOManager;
using namespace CRCore;


//////////////////////////////////////////////////////////////////
//
//crCreBotModel
//
//////////////////////////////////////////////////////////////////

void crCreBotModel::setName(const std::string &id)
{
    m_name = id;
}

void crCreBotModel::setMass(float mass)
{
    m_mass = mass;
}

float crCreBotModel::getMass()
{
    return m_mass;
}

const std::string &crCreBotModel::getName() const
{
    return m_name;
}

void crCreBotModel::insertComponent(const std::string &fileName, CRCore::crTransform *dof)
{
	m_components[fileName] = dof;
}

void crCreBotModel::request(CRCore::crGroup* parent,bool asyncLoad)
{
	CRCore::crTransform *transform = 0;
	CRCore::crTransform *newTransform = 0;
	for( Components::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr )
	{
		transform = itr->second.get();
		if(transform)
		{
			newTransform = dynamic_cast<CRCore::crTransform *>(transform->clone(crCopyOp::SHALLOW_COPY));
			parent->addChild(newTransform);
			crLoadManager::getInstance()->requestNode(itr->first,newTransform,asyncLoad);
		}
		else crLoadManager::getInstance()->requestNode(itr->first,parent,asyncLoad);
	}
}

//////////////////////////////////////////////////////////////////
//
//crCharacterBotModel
//
//////////////////////////////////////////////////////////////////

void crCharacterBotModel::setMass(float mass)
{
	m_mass = mass;
}

float crCharacterBotModel::getMass()
{
	return m_mass;
}

void crCharacterBotModel::request(const std::string& botName, CRCore::crGroup* parent,bool asyncLoad)
{
	//std::string &fileName = createTempCfg(botName);
    //
//    crLoadManager::getInstance()->requestNode(fileName,parent);

    //deleteTempCfg(fileName);
}

//std::string &crCharacterBotModel::createTempCfg()
//{//建立临时用于加载的cfg文件，内容相当于girl1.char
//
//}
//
//void crCharacterBotModel::deleteTempCfg(const std::string &fileName)
//{//销毁该临时文件
//
//}

//////////////////////////////////////////////////////////////////
//
//crLoadManager
//
//////////////////////////////////////////////////////////////////

ref_ptr<crLoadManager> crLoadManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crLoadManager::crLoadManager()
{
}

crLoadManager* crLoadManager::getInstance()
{
	if (m_instance==NULL)
	{
		m_instance = new crLoadManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crLoadManager::clear()
{//crLoadManager 不需要重新载入
	m_instance = NULL;
}

void crLoadManager::insertCreBotModel(crCreBotModel *model)
{
	m_creBotMap[model->getName()] = model;
}

void crLoadManager::insertCharacterBotModel(crCharacterBotModel *model)
{

}

void crLoadManager::requestCreBot(const std::string& id,CRPhysics::crViewMatterObject* parent,bool asyncLoad)
{
	CreBotModelMap::iterator itr = m_creBotMap.find(id);
	if(itr != m_creBotMap.end())
	{
		parent->setMass(itr->second->getMass());
        itr->second->request(parent,asyncLoad);
	}
	else
	{
		CRCore::notify(CRCore::NOTICE)<<"crLoadManager::requestCreBot() 没有找到 "<<id.c_str()<<" 请检查LoadManager.cfg并确认在程序启动时已经执行过loadLoadManagerCfg"<< std::endl;
	}
}

void crLoadManager::requestCharacterBot(const std::string& id,const std::string& botName,CRPhysics::crViewMatterObject* parent,bool asyncLoad)
{
	CharacterBotModelMap::iterator itr = m_characterBotMap.find(id);
	if(itr != m_characterBotMap.end())
	{
		parent->setMass(itr->second->getMass());
		itr->second->request(botName,parent,asyncLoad);
	}
	else
	{
		CRCore::notify(CRCore::NOTICE)<<"crLoadManager::requestCharacterBot() 没有找到 "<<id.c_str()<<" 请检查LoadManager.cfg并确认在程序启动时已经执行过loadLoadManagerCfg"<< std::endl;
	}
}

void crLoadManager::requestNode(const std::string& fileName,CRCore::crGroup* parent,bool asyncLoad,bool reload)
{
	//CRCore::notify(CRCore::FATAL)<<"crLoadManager::requestNode(): fileName = "<<fileName.c_str()<<std::endl;
	do 
	{
		if(!parent || fileName.empty())
			break;
		crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
		if(!reload && asyncLoad && dp->getInited())
		{
			dp->requestNodeFile(fileName,parent,0);
			break;
		}
		CRCore::ScopedLock<CRCore::crMutex> lock(dp->getLoadedMapMutex());
		crDatabasePager::LoadedMap &loadedMap = dp->getLoadedMap();
		if(!reload)
		{
			crDatabasePager::LoadedMap::iterator itr = loadedMap.find(fileName);
			if(itr != loadedMap.end())
			{
				parent->addChild(itr->second.get());
				break;
			}
		}
		ref_ptr<crNode> node = CRIOManager::readNodeFile(fileName);
		if(node.valid())
		{
			ref_ptr<crGroup> dummyGroup = dynamic_cast<crGroup *>(node.get());
			if(dummyGroup.valid()&&dummyGroup->getName().compare("LoadNodeDummy")==0)
			{
				node = dummyGroup->getChild(0);
				dummyGroup->removeChild(0,dummyGroup->getNumChildren());
			}
			if(!fileName.empty() && fileName.find("tmp.cre") == std::string::npos) 
			{
				//CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				//searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				//searchByClassNameVisitor.insertClassNameId("CreBodyNode");
				//node->accept(searchByClassNameVisitor);
				//if(!searchByClassNameVisitor.getResult())
				//{
				loadedMap[fileName] = node.get();
				//}
			}
			parent->addChild(node.get());
		}
	} while (0);
}

void crLoadManager::requestAddNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode,bool needCompile)
{
	if(parent && loadedNode)
	{
		ref_ptr<crGroup> dummyGroup = dynamic_cast<crGroup *>(loadedNode);
		if(dummyGroup.valid()&&dummyGroup->getName().compare("LoadNodeDummy")==0)
		{
			loadedNode = dummyGroup->getChild(0);
		}
		if (loadedNode)
		{
			crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
			if (dp->getInited())
			{
				//CRCore::notify(CRCore::FATAL)<<"crLoadManager::requestAddNode() "<<loadedNode->getName()<<std::endl;
				dp->requestAddNode(parent, loadedNode, needCompile);
			}
			else
			{
				parent->addChild(loadedNode);
			}
		}
	}
}

void crLoadManager::requestRemoveNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode)
{
	if (parent && loadedNode)
	{
		crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
		if (dp->getInited())
		{
			dp->requestRemoveNode(parent, loadedNode);
		}
		else
		{
			loadedNode->releaseObjects();
			parent->removeChild(loadedNode);
		}
	}
}

void crLoadManager::requestCompile(CRCore::crNode* loadedNode)
{
	if (loadedNode)
	{
		crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
		if (dp->getInited())
		{
			//CRCore::notify(CRCore::FATAL)<<"crLoadManager::requestCompile() "<<loadedNode->getName()<<std::endl;
			dp->requestCompile(loadedNode);
		}
	}
}
//void crLoadManager::requestCompileStateSet(CRCore::crStateSet* ss)
//{
//	crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
//	if(dp->getInited())
//	{
//		CRRender::crRenderInitVisitor *initVisitor = dynamic_cast<CRRender::crRenderInitVisitor *>(dp->getRenderInitVisitor());
//		//initVisitor->apply(*ss);
//		ss->compile(*initVisitor->getState());
//	}
//}

//////////////////////////////////////////////////////////////////
//
//crScriptLoadManager
//
//////////////////////////////////////////////////////////////////
ref_ptr<crScriptLoadManager> crScriptLoadManager::m_instance = NULL;
//////////////////////////////////////////////////////////////////
crScriptLoadManager::crScriptLoadManager()
{

}
crScriptLoadManager::~crScriptLoadManager()
{
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedItemMapMutex);
		for( LoadedItemMap::iterator itr = m_loadedItemMap.begin();
			itr != m_loadedItemMap.end();
			++itr )
		{
			if(itr->second)
			{
				delete itr->second;
				itr->second = NULL;
			}
		}
		m_loadedItemMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedReferencedMapMutex);
		m_loadedReferencedMap.clear();
	}
}

crScriptLoadManager* crScriptLoadManager::getInstance()
{
	if (m_instance==NULL)
	{
		m_instance = new crScriptLoadManager;
	}
	return m_instance.get();
}

void crScriptLoadManager::clear()
{
	m_instance = NULL;
}

bool crScriptLoadManager::loadConfigScript(rcfg::ConfigScript& cfg_script,const std::string &fileName,bool loadfromfile,const std::string& head)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedItemMapMutex);
	if(loadfromfile)
	{
		CRIOManager::crLoadCookFile scopedLoad(fileName);
		struct stat buf;
		if (stat( scopedLoad.getFileName().c_str(), &buf )) 
		{
			return false;
		}

		if (!cfg_script.Open(scopedLoad.getFileName().c_str(), head))
		{
			CRCore::notify(CRCore::FATAL)<<"loadConfigScript(): "<<cfg_script.GetLastError()<<std::endl;
			return false;
		}
	}
	else
	{
		ItemMap *itemmap = findLoadedItemMap(fileName);
		if(!itemmap)
		{
			CRIOManager::crLoadCookFile scopedLoad(fileName);
			struct stat buf;
			if (stat( scopedLoad.getFileName().c_str(), &buf )) 
			{
				return false;
			}

			if (!cfg_script.Open(scopedLoad.getFileName().c_str(), head))
			{
				CRCore::notify(CRCore::FATAL)<<"loadConfigScript(): "<<cfg_script.GetLastError()<<std::endl;
				return false;
			}
			itemmap = cfg_script.GetItemMap();
			//if(clone)
			//{
			//	ItemMap *itemmap_copy = new ItemMap(*itemmap);
			//	for( ItemMap::iterator itr = itemmap_copy->begin();
			//		itr != itemmap_copy->end();
			//		++itr )
			//	{
			//		itr->second = itr->second->Copy();
			//	}
			//	cfg_script.Open(itemmap_copy,true);
			//}
			insertLoadedItemMap(fileName,itemmap);
		}
		else
		{
			//if(clone)
			//{
			//	ItemMap *itemmap_copy = new ItemMap(*itemmap);
			//	for( ItemMap::iterator itr = itemmap_copy->begin();
			//		itr != itemmap_copy->end();
			//		++itr )
			//	{
			//		itr->second = itr->second->Copy();
			//	}
			//	cfg_script.Open(itemmap_copy,true);
			//}
			//else
			cfg_script.Open(itemmap);
		}
	}
	return true;
}
ItemMap *crScriptLoadManager::findLoadedItemMap(const std::string &fileName)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedItemMapMutex);
	LoadedItemMap::iterator itr = m_loadedItemMap.find(fileName);
	return itr != m_loadedItemMap.end()?itr->second:NULL;
}
void crScriptLoadManager::insertLoadedItemMap(const std::string &fileName,ItemMap *itemmap)
{
	m_loadedItemMap[fileName] = itemmap;
}
CRCore::Referenced* crScriptLoadManager::findLoadedRefeanced(const std::string &fileName)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedReferencedMapMutex);
	LoadedReferencedMap::iterator itr = m_loadedReferencedMap.find(fileName);
	return itr != m_loadedReferencedMap.end()?itr->second.get():NULL;
}
void crScriptLoadManager::insertLoadedRefeanced(const std::string &fileName,CRCore::Referenced* ref)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedReferencedMapMutex);
	m_loadedReferencedMap[fileName] = ref;
}
void crScriptLoadManager::eraseLoadedRefeanced(const std::string &fileName)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedReferencedMapMutex);
	LoadedReferencedMap::iterator itr = m_loadedReferencedMap.find(fileName);
	if(itr != m_loadedReferencedMap.end())
	{
		insertRemovedReferencedMap(itr->second.get());
		m_loadedReferencedMap.erase(itr);
	}
}
void crScriptLoadManager::eraseLoadedConfig(const std::string &fileName)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_loadedItemMapMutex);
	LoadedItemMap::iterator itr = m_loadedItemMap.find(fileName);
	if(itr != m_loadedItemMap.end())
	{
		delete itr->second;
		m_loadedItemMap.erase(itr);
	}
}
void crScriptLoadManager::insertRemovedReferencedMap(Referenced *ref)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_removedReferencedMapMutex);
	m_removedReferencedMap[ref] = 1.0f;
}
void crScriptLoadManager::updateRemovedReferencedMap(float dt)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_removedReferencedMapMutex);
	for( RemovedReferencedMap::iterator itr = m_removedReferencedMap.begin();
		itr != m_removedReferencedMap.end();)
	{
		itr->second -= dt;
		if(itr->second<0.0f)
		{
			itr = m_removedReferencedMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}