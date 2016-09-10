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
#include <CRCore/crEffectSequenceManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crScopedLock.h>
using namespace CRCore;
ref_ptr<crEffectSequenceManager> crEffectSequenceManager::m_instance = NULL;

crEffectSequenceManager::crEffectSequenceManager()
{
	m_effectRoot = new crGroup;
	m_effectRoot->setName("EffectSequenceRoot");
	setNumChildrenRequiringUpdateTraversal(1);
}

crEffectSequenceManager::crEffectSequenceManager(const crEffectSequenceManager& node,const crCopyOp& copyop):
crNode(node,copyop)
{
}

crEffectSequenceManager::~crEffectSequenceManager()
{
	//CRCore::notify(CRCore::FATAL)<<"~crEffectSequenceManager()"<<std::endl;
}

crEffectSequenceManager* crEffectSequenceManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crEffectSequenceManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crEffectSequenceManager::clear()
{
	CRCore::ScopedLock<crMutex> lock(m_effectNodeMapMutex);
	m_instance = NULL;
}

void crEffectSequenceManager::addEffectNodes( const std::string &name, crSequence *node )
{
	CRCore::ScopedLock<crMutex> lock(m_effectNodeMapMutex);
	m_effectNodeMap.insert(std::make_pair(name,node));
}

void crEffectSequenceManager::clearEffectNodes()
{
	CRCore::ScopedLock<crMutex> lock(m_effectNodeMapMutex);
	m_effectRoot->releaseObjects(0);
	m_effectRoot->removeChild(0,m_effectRoot->getNumChildren());
	m_effectNodeMap.clear();
	m_effectNodeMap_buf.clear();
	dirtyBound();
}

crSequence *crEffectSequenceManager::reuseOrgetOrCreateAnIdleEffectNodes(const std::string &name)
{	
	CRCore::ScopedLock<crMutex> lock(m_effectNodeMapMutex);
	EffectNodeMap::iterator beginItr = m_effectNodeMap.find(name);
	if(beginItr == m_effectNodeMap.end()) return 0;
	crSequence *effect = 0;
	int size = 0;
	std::vector<crSequence *> idleEffects;
	for(EffectNodeMap::iterator itr = beginItr;itr!= m_effectNodeMap.end() && name.compare(itr->first)==0; ++itr)
	{
		size++;
		effect = itr->second.get();
		if(effect->isEffectIdle())
		{
            idleEffects.push_back(effect);
		}
	}
	int nIdle = idleEffects.size();
	if(nIdle>0)
	{
        m_rangei.set(0,nIdle-1);
		return idleEffects[m_rangei.get_random()];
	}
	//create
	int clonePos = 0;
	if(size==0) return 0;
	else if(size>1)
	{
        m_rangei.set(0,size);
		clonePos = m_rangei.get_random();
	}
    for(int i = 0; i<clonePos; i++)
	{
        beginItr++;
	}
	effect = dynamic_cast<crSequence *>(beginItr->second->clone(crCopyOp::DEEP_COPY_NODES/*|crCopyOp::DEEP_COPY_BASES*/));
    //addEffectNodes(name,effect);
	m_effectNodeMap_buf.insert(EffectNodeMapPair(name,effect));
	return effect;
}

CRCore::crGroup *crEffectSequenceManager::getEffectRoot()
{
	return m_effectRoot.get();
}

void crEffectSequenceManager::traverse(crNodeVisitor& nv)
{
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
    if (vt==crNodeVisitor::CULL_VISITOR || vt == crNodeVisitor::SWAPBUFFER_VISITOR || vt==crNodeVisitor::INIT_VISITOR )
	{
		m_effectRoot->accept(nv);
	}
	else if(vt == CRCore::crNodeVisitor::UPDATE_VISITOR)
	{
		for( EffectNodeMap::iterator itr = m_effectNodeMap_buf.begin();
			itr != m_effectNodeMap_buf.end();
			++itr )
		{
			addEffectNodes(itr->first,itr->second.get());
			m_effectRoot->addChild(itr->second.get());
		}
		m_effectNodeMap_buf.clear();

		m_effectRoot->accept(nv);
	}
}

void crEffectSequenceManager::releaseObjects(crState* state)
{
	m_effectRoot->releaseObjects(state);
}

//void crEffectSequenceManager::swapBuffers()
//{
//	m_effectRoot->swapBuffers();
//}

bool crEffectSequenceManager::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	m_bbox.expandBy(m_effectRoot->getBoundBox());
	m_boundSphere.expandBy(m_effectRoot->getBound());

	m_bBoundSphere_computed = true;
	return true;
}