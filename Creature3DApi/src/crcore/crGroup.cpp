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
//Modified by Wucaihua
#include <CRCore/crGroup.h>
//#include <CRCore/crAABB.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crTransform.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crNotify.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>

using namespace CRCore;

crGroup::crGroup()
{
	m_NodeType = NT_GROUP;
	m_attributeMask = AttrAll;
}

crGroup::crGroup(const crGroup& group,const crCopyOp& copyop):
crNode(group,copyop),
m_attributeMask(group.m_attributeMask)
{
  m_NodeType = NT_GROUP;
  for(NodeArray::const_iterator itr=group.m_childArray.begin();
    itr!=group.m_childArray.end();
    ++itr)
  {
    crNode* child = copyop(itr->get());
    if (child) addChild(child);
  }
}

crGroup::~crGroup()
{
  // remove reference to this from children's parent lists.
  for(NodeArray::iterator itr=m_childArray.begin();
    itr!=m_childArray.end();
    ++itr)
  {
	  (*itr)->removeParent(this);
  }

}


void crGroup::traverse(crNodeVisitor& nv)
{
  for(NodeArray::iterator itr=m_childArray.begin();
    itr!=m_childArray.end();
    ++itr)
  {
	  (*itr)->accept(nv);
  }
}

/** return true if node is contained within crGroup.*/
bool crGroup::containsNode( const crNode* node )
{
	const ParentArray &parents = node->getParents();
	for(ParentArray::const_iterator itr = parents.begin();
		itr != parents.end();
		++itr)
	{
		if( (*itr) == this ) return true;
	}
	return false;
}

bool crGroup::containsNode( const crNode* node ) const
{
	const ParentArray &parents = node->getParents();
	for(ParentArray::const_iterator itr = parents.begin();
		itr != parents.end();
		++itr)
	{
		if( (*itr) == this ) return true;
	}
	return false;
}


bool crGroup::addChild( crNode *child )
{
  return crGroup::insertChild( m_childArray.size(), child );
}

bool crGroup::insertChild( unsigned int index, crNode *child )
{
    if (!child||child==this||containsNode(child)) return false;

    // note ref_ptr<> automatically handles incrementing child's reference count.
    if (index >= m_childArray.size())
    {
      m_childArray.push_back(child);
    }
    else
    {
      m_childArray.insert(m_childArray.begin()+index, child);
    }

    // register as parent of child.
    
    child->addParent(this);

    dirtyBound();

    //// could now require app traversal thanks to the new subgraph,
    //// so need to check and update if required.
    if (child->getNumChildrenRequiringUpdateTraversal()>0 ||
      child->getUpdateCallback())
    {
      setNumChildrenRequiringUpdateTraversal(
        getNumChildrenRequiringUpdateTraversal()+1
        );
    }

	//// could now require updateUniform traversal thanks to the new subgraph,
	//// so need to check and update if required.
	//if (child->getNumChildrenRequiringUpdateUniformTraversal()>0 ||
	//	child->getUpdateCallback())
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(
	//		getNumChildrenRequiringUpdateUniformTraversal()+1
	//		);
	//}

    //// could now require disabling of culling thanks to the new subgraph,
    //// so need to check and update if required.
    if (child->getNumChildrenWithCullingDisabled()>0 ||
      !child->getCullingActive())
    {
      setNumChildrenWithCullingDisabled(
        getNumChildrenWithCullingDisabled()+1
        );
    }
    if (child->getNumChildrenWithOccluderNodes()>0 ||
      dynamic_cast<CRCore::crOccluderNode*>(child))
    {
		//CRCore::notify(CRCore::FATAL)<<"crGroup::insertChild childname = "<<child->getName()<<std::endl;

      setNumChildrenWithOccluderNodes(
        getNumChildrenWithOccluderNodes()+1
        );
    }
	if (child->getNumChildrenWithEventGroups()>0 ||
		dynamic_cast<CRCore::crEventGroup*>(child))
	{
		setNumChildrenWithEventGroups(
			getNumChildrenWithEventGroups()+1
			);
	}
	//if (child->getNumChildrenRequiringCalcShadow()>0)
	//{
	//	setNumChildrenRequiringCalcShadow(
	//		getNumChildrenRequiringCalcShadow()+1
	//		);
	//}
    return true;

}

bool crGroup::removeChild( crNode *child )
{
  return removeChild(getChildIndex(child));
}

bool crGroup::removeChild(unsigned int pos,unsigned int numChildrenToRemove)
{
  if (pos<m_childArray.size() && numChildrenToRemove>0)
  {
    unsigned int endOfRemoveRange = pos+numChildrenToRemove;
    if (endOfRemoveRange>m_childArray.size())
    {
     // notify(DEBUG_INFO)<<"Warning: crGroup::removeChild(i,numChildrenToRemove) has been passed an excessive number"<<std::endl;
     // notify(DEBUG_INFO)<<"         of chilren to remove, trimming just to end of child list."<<std::endl;
      endOfRemoveRange=m_childArray.size();
    }

    unsigned int appCallbackRemoved = 0;
	unsigned int numUpdateUniformRemoved = 0;
    unsigned int numChildrenWithCullingDisabledRemoved = 0;
    unsigned int numChildrenWithOccludersRemoved = 0;
	unsigned int numChildrenWithEventGroupsRemoved = 0;
	unsigned int numChildrenRequiringCalcShadowRemoved = 0;

    for(unsigned i=pos;i<endOfRemoveRange;++i)
    {
      CRCore::crNode* child = m_childArray[i].get();
      // remove this Geode from the child parent list.
      child->removeParent(this);

      if (child->getNumChildrenRequiringUpdateTraversal()>0 || child->getUpdateCallback()) ++appCallbackRemoved;
 
	  //if (child->getNumChildrenRequiringUpdateUniformTraversal()>0 || child->getUpdateUniformCallback()) ++numUpdateUniformRemoved;

      if (child->getNumChildrenWithCullingDisabled()>0 || !child->getCullingActive()) ++numChildrenWithCullingDisabledRemoved;

      if (child->getNumChildrenWithOccluderNodes()>0 || dynamic_cast<CRCore::crOccluderNode*>(child)) ++numChildrenWithOccludersRemoved;
	 
	  if (child->getNumChildrenWithEventGroups()>0 || dynamic_cast<CRCore::crEventGroup*>(child)) ++numChildrenWithEventGroupsRemoved;
	 
	  //if (child->getNumChildrenRequiringCalcShadow()>0) ++numChildrenRequiringCalcShadowRemoved;

    }

    m_childArray.erase(m_childArray.begin()+pos,m_childArray.begin()+endOfRemoveRange);

    if (appCallbackRemoved)
    {
      setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-appCallbackRemoved);
    }

	//if (numUpdateUniformRemoved)
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()-numUpdateUniformRemoved);
	//}

    if (numChildrenWithCullingDisabledRemoved)
    {
      setNumChildrenWithCullingDisabled(getNumChildrenWithCullingDisabled()-numChildrenWithCullingDisabledRemoved);
    }

    if (numChildrenWithOccludersRemoved)
    {
      setNumChildrenWithOccluderNodes(getNumChildrenWithOccluderNodes()-numChildrenWithOccludersRemoved);
    }

	if (numChildrenWithEventGroupsRemoved)
	{
		setNumChildrenWithEventGroups(getNumChildrenWithEventGroups()-numChildrenWithEventGroupsRemoved);
	}

	//if (numChildrenRequiringCalcShadowRemoved)
	//{
	//	setNumChildrenRequiringCalcShadow(getNumChildrenRequiringCalcShadow()-numChildrenRequiringCalcShadowRemoved);
	//}

    dirtyBound();

    return true;
  }
  else return false;
}


bool crGroup::replaceChild( crNode *origNode, crNode *newNode )
{
  if (newNode==NULL || origNode==newNode || newNode==this) return false;

  unsigned int pos = getChildIndex(origNode);
  if (pos<m_childArray.size())
  {
    return setChild(pos,newNode);
  }
  return false;
}

bool crGroup::replaceChildExt(crNode *origNode, crNode *newNode)
{
	if (newNode==NULL || origNode==newNode || newNode==this) return false;

	newNode->setName(origNode->getName());
	newNode->setVisiable(origNode->getVisiable());
	newNode->setCalcShadow(origNode->isCalcShadow());
	newNode->setEnableIntersect(origNode->getEnableIntersect());
	newNode->setRenderable(origNode->getRenderable());
	newNode->setCollideMode(origNode->getCollideMode());
	newNode->setCullingActive(origNode->getCullingActive());
	newNode->setDescriptionsInString(origNode->getDescriptionsInString());
	newNode->setStateSet(origNode->getStateSet());
	newNode->setDataClass(origNode->getDataClass());
	crGroup* newGroup = dynamic_cast<crGroup *>(newNode);
	crGroup* origGroup = dynamic_cast<crGroup *>(origNode);
	if(newGroup && origGroup)
	{
		int num = origGroup->getNumChildren();
		for(int i = 0; i<num; i++)
			newGroup->addChild(origGroup->getChild(i));
		origGroup->removeChild(0,num);
	}
    return replaceChild(origNode,newNode);
}

bool crGroup::setChild( unsigned  int i, crNode* newNode )
{
  if (i<m_childArray.size() && newNode && newNode!=this)
  {

    ref_ptr<crNode> origNode = m_childArray[i];

    // first remove for origNode's parent list.
    origNode->removeParent(this);

    // note ref_ptr<> automatically handles decrementing origNode's reference count,
    // and inccrementing newNode's reference count.
    m_childArray[i] = newNode;

    // register as parent of child.
    newNode->addParent(this);

    dirtyBound();


    // could now require app traversal thanks to the new subgraph,
    // so need to check and update if required.
    int delta_numChildrenRequiringAppTraversal = 0;
    if (origNode->getNumChildrenRequiringUpdateTraversal()>0 ||
      origNode->getUpdateCallback())
    {
      --delta_numChildrenRequiringAppTraversal;
    }
    if (newNode->getNumChildrenRequiringUpdateTraversal()>0 ||
      newNode->getUpdateCallback())
    {
      ++delta_numChildrenRequiringAppTraversal;
    }

    if (delta_numChildrenRequiringAppTraversal!=0)
    {
      setNumChildrenRequiringUpdateTraversal(
        getNumChildrenRequiringUpdateTraversal()+delta_numChildrenRequiringAppTraversal
        );
    }

	// could now require updateUniform traversal thanks to the new subgraph,
	// so need to check and update if required.
	//int delta_numChildrenRequiringUpdateUniformTraversal = 0;
	//if (origNode->getNumChildrenRequiringUpdateUniformTraversal()>0 ||
	//	origNode->getUpdateUniformCallback())
	//{
	//	--delta_numChildrenRequiringUpdateUniformTraversal;
	//}
	//if (newNode->getNumChildrenRequiringUpdateUniformTraversal()>0 ||
	//	newNode->getUpdateUniformCallback())
	//{
	//	++delta_numChildrenRequiringUpdateUniformTraversal;
	//}

	//if (delta_numChildrenRequiringUpdateUniformTraversal!=0)
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(
	//		getNumChildrenRequiringUpdateUniformTraversal()+delta_numChildrenRequiringUpdateUniformTraversal
	//		);
	//}

    // could now require disabling of culling thanks to the new subgraph,
    // so need to check and update if required.
    int delta_numChildrenWithCullingDisabled = 0;
    if (origNode->getNumChildrenWithCullingDisabled()>0 &&
      !origNode->getCullingActive())
    {
      --delta_numChildrenWithCullingDisabled;
    }
    if (newNode->getNumChildrenWithCullingDisabled()>0 &&
      !newNode->getCullingActive())
    {
      ++delta_numChildrenWithCullingDisabled;
    }

    if (delta_numChildrenWithCullingDisabled!=0)
    {
      setNumChildrenWithCullingDisabled(
        getNumChildrenWithCullingDisabled()+delta_numChildrenWithCullingDisabled
        );
    }

    // could now require disabling of culling thanks to the new subgraph,
    // so need to check and update if required.
    int delta_numChildrenWithOccluderNodes = 0;
    if (origNode->getNumChildrenWithOccluderNodes()>0 &&
      !origNode->getCullingActive())
    {
      --delta_numChildrenWithOccluderNodes;
    }
    if (newNode->getNumChildrenWithOccluderNodes()>0 &&
      !newNode->getCullingActive())
    {
      ++delta_numChildrenWithOccluderNodes;
    }

    if (delta_numChildrenWithOccluderNodes!=0)
    {
		//CRCore::notify(CRCore::FATAL)<<"crGroup::setChild delta_numChildrenWithOccluderNodes = "<<delta_numChildrenWithOccluderNodes<<std::endl;
		//CRCore::notify(CRCore::FATAL)<<"crGroup::setChild name = "<<newNode->getName()<<std::endl;

      setNumChildrenWithOccluderNodes(
        getNumChildrenWithOccluderNodes()+delta_numChildrenWithOccluderNodes
        );
    }

    //event group
	int delta_numChildrenWithEventGroups = 0;
	if (origNode->getNumChildrenWithEventGroups()>0 &&
		!origNode->getCullingActive())
	{
		--delta_numChildrenWithEventGroups;
	}
	if (newNode->getNumChildrenWithEventGroups()>0 &&
		!newNode->getCullingActive())
	{
		++delta_numChildrenWithEventGroups;
	}

	if (delta_numChildrenWithEventGroups!=0)
	{
		setNumChildrenWithEventGroups(
			getNumChildrenWithEventGroups()+delta_numChildrenWithEventGroups
			);
	}
    //calcShadow set
	//int delta_numChildrenRequiringCalcShadow = 0;
	//if (origNode->getNumChildrenRequiringCalcShadow()>0)
	//{
	//	--delta_numChildrenRequiringCalcShadow;
	//}
	//if (newNode->getNumChildrenRequiringCalcShadow()>0)
	//{
	//	++delta_numChildrenRequiringCalcShadow;
	//}

	//if (delta_numChildrenRequiringCalcShadow!=0)
	//{
	//	setNumChildrenRequiringCalcShadow(
	//		getNumChildrenRequiringCalcShadow()+delta_numChildrenRequiringCalcShadow
	//		);
	//}
    return true;
  }
  else return false;

}

bool crGroup::computeBound() const
{
  m_boundSphere.init();
  m_bbox.init();

  if (m_childArray.empty()) 
  {
    m_bBoundSphere_computed = true;
    return false;
  }

  // note, special handling of the case when a child is an Transform,
  // such that only Transforms which are relative to their parents coordinates frame (i.e this group)
  // are handled, Transform relative to and absolute reference frame are ignored.

  NodeArray::const_iterator itr;
  for(itr=m_childArray.begin();
    itr!=m_childArray.end();
    ++itr)
  {
    //const CRCore::crTransform* transform = (*itr)->asTransform();
    //if (!transform || transform->getReferenceFrame()==CRCore::crTransform::RELATIVE_TO_PARENTS)
    {
      m_bbox.expandBy((*itr)->getBoundBox());
    }
  }

  if (!m_bbox.valid()) 
  {
    m_bBoundSphere_computed = true;
    return false;
  }

  m_boundSphere.expandBy(m_bbox);
  //m_boundSphere.m_center = m_bbox.center();
  //m_boundSphere.m_fRadius = 0.0f;
  //for(itr=m_childArray.begin();
  //  itr!=m_childArray.end();
  //  ++itr)
  //{
  //  const CRCore::crTransform* transform = (*itr)->asTransform();
  //  if (!transform || transform->getReferenceFrame()==CRCore::crTransform::RELATIVE_TO_PARENTS)
  //  {
  //    m_boundSphere.expandRadiusBy((*itr)->getBound());
  //  }
  //}

  m_bBoundSphere_computed = true;
  return true;
}

void crGroup::releaseObjects(CRCore::crState* state)
{
    crNode::releaseObjects(state);
    for(NodeArray::iterator itr=m_childArray.begin();
        itr!=m_childArray.end();
        ++itr)
    {
		if(itr->valid())
			(*itr)->releaseObjects(state);
    }
}
