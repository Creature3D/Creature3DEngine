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
#include <CRPhysics/crVolumeNode.h>
using namespace CRCore;
using namespace CRPhysics;
ref_ptr<crVolumeNodeManager> crVolumeNodeManager::m_instance = NULL;
////////////////////////////////////
//
//crVolumeNode
//
/////////////////////////////////////
crVolumeNode::crVolumeNode():
m_enable(true),
m_maxTriggerCount(0),
m_reTriggerDelay(0.0f),
//m_previousFrameTime(0.0f),
m_interval(0.0f),
m_triggerCount(0)
{
}
crVolumeNode::crVolumeNode(const crVolumeNode &volumeNode,const CRCore::crCopyOp& copyop):
CRCore::crGroup(volumeNode,copyop),
m_enable(volumeNode.m_enable),
m_maxTriggerCount(volumeNode.m_maxTriggerCount),
m_reTriggerDelay(volumeNode.m_reTriggerDelay),
//m_previousFrameTime(0.0f),
m_interval(0.0f),
m_triggerCount(0)
{
}

void crVolumeNode::releaseObjects(CRCore::crState* state)
{
	crGroup::releaseObjects(state);
	crVolumeNodeManager::getInstance()->removeVolumeNode(const_cast<crVolumeNode *>(this));
}

void crVolumeNode::traverse(crNodeVisitor& nv)
{
    if(crDisplaySettings::instance()->getRunMode() == 0)//Editor
	{
		crGroup::traverse(nv);
	}
}

void crVolumeNode::trigger(/*const crFrameStamp* frameStamp*/)
{
	std::list<crMatterObject *> toErase;
	CRCore::ref_ptr<crMatterObject> setItr;
	for( InVolumeNodeSet::iterator itr = m_inVolumeNodeSet.begin();
		itr != m_inVolumeNodeSet.end();
		++itr )
	{
		setItr = *itr;
		if(setItr->referenceCount()>1)
		{
			if(!m_enable)
			{
				doEvent(WCH_OUTVOLUME,MAKEINT64(setItr.get(),NULL)); 
			}
		}
		else
		{//该对象已经从场景中删除
			toErase.push_back(setItr.get());
		}
	}
	if(!m_enable)
	{
		m_inVolumeNodeSet.clear();
	}
	else
	{
		for( std::list<crMatterObject *>::iterator itr = toErase.begin();
			itr != toErase.end();
			++itr )
		{
			m_inVolumeNodeSet.erase(*itr);
		}
	}

	if(m_reTriggerDelay>0.0f && m_enable && !m_inVolumeNodeSet.empty())
	{
		//float referenceTime = frameStamp->getReferenceTime();
		//m_interval += (m_previousFrameTime != 0.0) ? (referenceTime-m_previousFrameTime) : 0.0;
		//m_previousFrameTime = referenceTime;
		m_interval += crFrameStamp::getInstance()->getFrameInterval();

		if(m_interval>m_reTriggerDelay)
		{
			m_interval = 0.0f;
			m_triggerCount = 0;
			//m_enable = true;
			ref_ptr<crMatterObject> node;
			for( InVolumeNodeSet::iterator itr = m_inVolumeNodeSet.begin();
				itr != m_inVolumeNodeSet.end();
				++itr )
			{
				if(m_enable)
				{
					node = *itr;
					doVolumeEvent(node.get());
				}
			}
		}
	}
	else m_interval = 0.0f;
}

bool crVolumeNode::doVolumeEvent(crMatterObject *materObject)
{
	bool isvalid = true;
	crNode::doEvent(WCH_INVOLUME,MAKEINT64(materObject,&isvalid));
	if(isvalid)
	{
		if(m_maxTriggerCount>0)
		{
			m_triggerCount++;
			if(m_triggerCount>=m_maxTriggerCount) m_enable = false;
		}
	}
	return isvalid;
}
void crVolumeNode::testVolumeEvent(crMatterObject *materObject,bool tested)
{
	bool bInVolume = isInVolume(materObject);
	if(tested && !bInVolume)
	{
		inVolume(materObject);
	}
	else if(!tested && bInVolume)
	{
		outVolume(materObject);
	}
}
void crVolumeNode::testVolumeImplementation(crMatterObject *materObject)
{
	bool tested = intersects(materObject->getTrans());
    testVolumeEvent(materObject,tested);
}
//bool crVolumeNode::intersects(const CRCore::crBoundingBox &bbox)
//{
//	getBound();
//	return m_bbox.intersects(bbox);
//}
bool crVolumeNode::intersects(const CRCore::crVector3 &pos)
{
	getBound();
	return m_bbox.contains(pos);
}

void crVolumeNode::inVolume(crMatterObject *materObject) 
{ 
	if(m_enable) 
	{
		if(doVolumeEvent(materObject))
			m_inVolumeNodeSet.insert(materObject);
	}
}

void crVolumeNode::outVolume(crMatterObject *materObject) 
{
	doEvent(WCH_OUTVOLUME,MAKEINT64(materObject,NULL)); 
	m_inVolumeNodeSet.erase(materObject); 
}
void crVolumeNode::tryEnableVolume(bool enable)
{
	doEvent(WCH_ENABLEVOLUME,MAKEINT64(enable?1:0,NULL));
}
void crVolumeNode::setEnable(bool enable)
{
    m_enable = enable;
}

bool crVolumeNode::getEnable()
{
    return m_enable;
}

void crVolumeNode::setMaxTriggerCount(int triggerCount)
{
    m_maxTriggerCount = triggerCount;
}

int crVolumeNode::getMaxTriggerCount()
{
    return m_maxTriggerCount;
}

void crVolumeNode::setReTriggerDelay(float delay)
{
    m_reTriggerDelay = delay;
}

float crVolumeNode::getReTriggerDelay()
{
    return m_reTriggerDelay;
}

void crVolumeNode::setMainBody(CRCore::crNode *mainBody)
{
    m_mainBody = mainBody;
}

CRCore::crNode *crVolumeNode::getMainBody()
{
    return m_mainBody.get();
}

void crVolumeNode::reset()
{
	for( InVolumeNodeSet::iterator itr = m_inVolumeNodeSet.begin();
		itr != m_inVolumeNodeSet.end();
		++itr )
	{
		doEvent(WCH_OUTVOLUME,MAKEINT64(itr->get(),NULL)); 
	}
	m_inVolumeNodeSet.clear();
	m_interval = 0.0f;
	m_triggerCount = 0;
}
////////////////////////////////////
//
//crSphereVolumeNode
//
/////////////////////////////////////
crSphereVolumeNode::crSphereVolumeNode()
{

}
crSphereVolumeNode::crSphereVolumeNode(const crSphereVolumeNode &volumeNode,const CRCore::crCopyOp& copyop):
	crVolumeNode(volumeNode,copyop)
{

}
void crSphereVolumeNode::testVolumeImplementation(crMatterObject *materObject)
{
	bool tested = intersects(materObject->getTrans());
	testVolumeEvent(materObject,tested);
}
bool crSphereVolumeNode::intersects(const CRCore::crVector3 &pos)
{
	getBound();
	return m_boundSphere.contains(pos);
}
////////////////////////////////////
//
//crPolytopeVolumeNode
//
/////////////////////////////////////
crPolytopeVolumeNode::crPolytopeVolumeNode()
{

}
crPolytopeVolumeNode::crPolytopeVolumeNode(const crPolytopeVolumeNode &volumeNode,const CRCore::crCopyOp& copyop):
crVolumeNode(volumeNode,copyop),
m_polytope(volumeNode.m_polytope)
{

}
void crPolytopeVolumeNode::testVolumeImplementation(crMatterObject *materObject)
{
	bool tested = intersects(materObject->getTrans());
    testVolumeEvent(materObject,tested);
}
//bool crPolytopeVolumeNode::intersects(const CRCore::crBoundingBox &bbox)
//{
//	return m_polytope.contains(bbox);
//}
bool crPolytopeVolumeNode::intersects(const CRCore::crVector3 &pos)
{
	return m_polytope.contains(pos);
}
////////////////////////////////////
//
//crMatrixVolumeNode
//
/////////////////////////////////////
crMatrixVolumeNode::crMatrixVolumeNode()
{

}
crMatrixVolumeNode::crMatrixVolumeNode(const crMatrixVolumeNode &volumeNode,const CRCore::crCopyOp& copyop):
crVolumeNode(volumeNode,copyop),
m_matrix(volumeNode.m_matrix)
{

}
void crMatrixVolumeNode::testVolumeImplementation(crMatterObject *materObject)
{//需要优化
	//getBound();
	//crBoundingBox newBB;
	//newBB.m_max = m_bbox.m_max * m_matrix;
	//newBB.m_min = m_bbox.m_min * m_matrix;
	//newBB.correct();
	//bool tested = newBB.intersects(materObject->getBoundBox());	
	bool tested = intersects(materObject->getTrans());
    testVolumeEvent(materObject,tested);
}
//bool crMatrixVolumeNode::intersects(const CRCore::crBoundingBox &bbox)
//{
//	crBoundingBox _bbox = bbox;
//	crMatrix invmat = crMatrix::inverse(m_matrix);
//	_bbox.m_max = _bbox.m_max * invmat;
//	_bbox.m_min = _bbox.m_min * invmat;
//	_bbox.correct();
//	return m_bbox.contains(_bbox);
//}
bool crMatrixVolumeNode::intersects(const CRCore::crVector3 &pos)
{
	getBound();
	crMatrix invmat = crMatrix::inverse(m_matrix);
	CRCore::crVector3 _pos = pos * invmat;
	return m_bbox.contains(_pos);
}
////////////////////////////////////
//
//crVolumeNodeManager
//
/////////////////////////////////////
crVolumeNodeManager::crVolumeNodeManager()
{
	m_bCullingActive = false;
	m_isCalcShadow = false;
	m_renderable = false;
	m_visiable = false;
	setEnableIntersect(false);
	setNumChildrenRequiringUpdateTraversal(1);
}

crVolumeNodeManager* crVolumeNodeManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crVolumeNodeManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crVolumeNodeManager::clear()
{
	m_instance = NULL;
}

void crVolumeNodeManager::wantAddVolumeNode(crVolumeNode *volumeNode)
{
	m_addVolumeNodeBuf.push_back(volumeNode);
}

void crVolumeNodeManager::addVolumeNode(crVolumeNode *volumeNode)
{
	m_volumeNodeMap.insert(std::make_pair(volumeNode->getName(),volumeNode));
}

void crVolumeNodeManager::removeVolumeNode(crVolumeNode *volumeNode)
{
    m_removeVolumeNodeBuf.push_back(volumeNode);
}

void crVolumeNodeManager::testVolume(crMatterObject *materObject)
{//if inVolume, node->doEvent(WCH_INVOLUME,volume)
	for( VolumeNodeMap::iterator itr = m_volumeNodeMap.begin();
		 itr != m_volumeNodeMap.end();
		 ++itr )
	{
		itr->second->testVolumeImplementation(materObject);
	}
}

crVolumeNode *crVolumeNodeManager::getVolumeNode(const std::string &name)
{
	VolumeNodeMap::iterator itr = m_volumeNodeMap.find(name);
	if(itr != m_volumeNodeMap.end())
		return itr->second.get();
	return NULL;
}

//crVolumeNode *crVolumeNodeManager::reuseOrgetOrCreateAnIdleEffectNodes(const std::string &name)
//{
//	VolumeNodeMap::iterator beginItr = m_volumeNodeMap.find(name);
//	if(beginItr == m_volumeNodeMap.end()) return 0;
//	crVolumeNode *effect = 0;
//	for(VolumeNodeMap::iterator itr = beginItr;itr!= m_volumeNodeMap.end() && name.compare(itr->first)==0; ++itr)
//	{
//		if(effect->isEffectIdle())
//			return effect;
//	}
//	effect = dynamic_cast<crVolumeNode *>(beginItr->second->clone(crCopyOp::SHALLOW_COPY));
//	m_addVolumeNodeBuf.push_back(effect);
//	return effect;
//}

void crVolumeNodeManager::traverse(crNodeVisitor& nv)
{
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if(vt==crNodeVisitor::UPDATE_VISITOR)
	{
		VolumeNodeMap::iterator vmitr;
		for( VolumeNodeList::iterator itr = m_removeVolumeNodeBuf.begin();
			itr != m_removeVolumeNodeBuf.end();
			++itr )
		{
			vmitr = m_volumeNodeMap.find((*itr)->getName());
			for( ;
				vmitr != m_volumeNodeMap.end() && vmitr->first.compare((*itr)->getName()) == 0;
				++vmitr )
			{
				if(vmitr->second == itr->get())
				{
					vmitr->second->reset();
					m_volumeNodeMap.erase(vmitr);
					break;
				}
			}
		}
		m_removeVolumeNodeBuf.clear();

		for( VolumeNodeList::iterator itr = m_addVolumeNodeBuf.begin();
			itr != m_addVolumeNodeBuf.end();
			++itr )
		{
			//CRCore::notify(CRCore::FATAL)<<"crVolumeNodeManager::traverse: addEffectNodes: "<<itr->second.get()<<std::endl;
			addVolumeNode(itr->get());
		}
		m_addVolumeNodeBuf.clear();

		crVolumeNode *node;
		for( VolumeNodeMap::iterator itr = m_volumeNodeMap.begin();
			itr != m_volumeNodeMap.end();
			++itr )
		{
			node = itr->second.get();
			node->doEvent(WCH_UPDATEVISITOR,MAKEINT64(&nv,NULL));
			node->trigger(/*nv.getFrameStamp()*/);
			//node->accept(nv);
		}
	}
	//else
	//{//physics update 会出异常
	//	crVolumeNode *node;
	//	for( VolumeNodeMap::iterator itr = m_volumeNodeMap.begin();
	//		itr != m_volumeNodeMap.end();
	//		++itr )
	//	{
	//		node->accept(nv);
	//	}
	//}
}
