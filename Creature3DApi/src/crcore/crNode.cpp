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
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNotify.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crEventGroup.h>
#include <ode/ode.h>
#include <algorithm>

using namespace CRCore;
namespace CRCore
{
	/// Helper class for generating NodePathList.
	class CollectParentPaths : public crNodeVisitor
	{
	public:
		CollectParentPaths(CRCore::crNode* haltTraversalAtNode=0) : 
		  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_PARENTS),
			  m_haltTraversalAtNode(haltTraversalAtNode)
		  {
		  }

		  virtual void apply(CRCore::crNode& node)
		  {
			  if (node.getNumParents()==0 || &node==m_haltTraversalAtNode)
			  {
				  m_nodePaths.push_back(getNodePath());
			  }
			  else
			  {
				  traverse(node);
			  }
		  }

		  crNode*         m_haltTraversalAtNode;
		  NodePath        m_nodePath;
		  NodePathList    m_nodePaths;
	};
}
crNode::crNode()
{    
	m_NodeType = NT_NODE;

    m_bBoundSphere_computed = false;
    m_nodeMask = 0xffffffff;    
    m_nNumChildrenRequiringUpdateTraversal = 0;
	//m_nNumChildrenRequiringUpdateUniformTraversal = 0;
	//m_numChildrenRequiringEventTraversal = 0;
    m_bCullingActive = true;
    m_nNumChildrenWithCullingDisabled = 0;
    m_nNumChildrenWithOccluderNodes = 0;
	m_nNumChildrenWithEventGroups = 0;
	//m_nNumChildrenRequiringCalcShadow = 0;
	//m_culled = -1;
	m_enableIntersect = true;
	//m_isShowBoundingBox = false;
	m_isCalcShadow = true;
    m_visiable = true;
	m_renderable = true;
	m_physicsInited = false;
	m_mapCollide = false;
	m_collideMode = CollideNone;
	m_physicsObjID = NULL;
	m_heightData = NULL;
	m_boundSetted = false;
	m_currentMsg = 0;
	m_editorHidden = false;
	m_editorIntersectable = true;

	m_renderInited = false;
	m_avoidIntersect = false;
}

crNode::crNode(const crNode& node,const crCopyOp& copyop):
        crBase(node,copyop),
        m_boundSphere(node.m_boundSphere),
        m_bBoundSphere_computed(node.m_bBoundSphere_computed),
		m_bbox(node.m_bbox),
		m_boundSetted(node.m_boundSetted),
		//m_culled(node.m_culled),
		m_enableIntersect(node.m_enableIntersect),
        m_parentArray(), // leave empty as parentList is managed by crGroup.
        m_nNumChildrenRequiringUpdateTraversal(0),
		//m_refUpdateUniformCallback(node.m_refUpdateUniformCallback),
		//m_nNumChildrenRequiringUpdateUniformTraversal(0),
		//m_eventCallback(node.m_eventCallback),
		//m_numChildrenRequiringEventTraversal(0),
        m_bCullingActive(node.m_bCullingActive),
        m_nNumChildrenWithCullingDisabled(0), // assume no children yet.
        m_nNumChildrenWithOccluderNodes(0),
		m_nNumChildrenWithEventGroups(0),
		//m_nNumChildrenRequiringCalcShadow(0),
        m_nodeMask(node.m_nodeMask), 
        m_descriptionArray(node.m_descriptionArray),
        m_stateset(copyop(node.m_stateset.get())),
		//m_isShowBoundingBox(node.m_isShowBoundingBox),
		m_isCalcShadow(node.m_isCalcShadow),
		m_visiable(node.m_visiable),
		m_renderable(node.m_renderable),
		m_physicsInited(false),
		m_mapCollide(node.m_mapCollide),
		m_collideMode(node.m_collideMode),
		m_editorHidden(false),
		m_editorIntersectable(true),
		m_physicsObjID(NULL),
		m_heightData(NULL),
		m_renderInited(false),
		m_avoidIntersect(node.m_avoidIntersect)
{
	if(node.m_refUpdateCallback.valid())
	{
         //addUpdateCallback(dynamic_cast<crNodeCallback *>(node.m_refUpdateCallback->clone()));
		addUpdateCallback(dynamic_cast<crNodeCallback *>(copyop(node.m_refUpdateCallback.get())));
	}
	if(node.m_refCullCallback.valid())
	{
		//addCullCallback(dynamic_cast<crNodeCallback *>(node.m_refCullCallback->clone()));
		addCullCallback(dynamic_cast<crNodeCallback *>(copyop(node.m_refCullCallback.get())));
	}
	if(node.m_collideCallback.valid())
	{
		//addCollideCallback(dynamic_cast<crCollideCallback *>(node.m_collideCallback->clone()));
		addCollideCallback(dynamic_cast<crCollideCallback *>(copyop(node.m_collideCallback.get())));
	}

	for( EventMap::const_iterator itr = node.m_eventMap.begin();
		itr != node.m_eventMap.end();
		++itr )
	{
		//insertEvent(itr->first,itr->second->clone());
		insertEvent(itr->first,dynamic_cast<crEventCallback *>(copyop(itr->second.get())));
	}
	if(hasHandle(WCH_UPDATEVISITOR))
	{
		setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	}

	m_currentMsg = 0;
}

crNode::~crNode()
{
	setStateSet(0);

	//CRCore::notify(CRCore::ALWAYS)<<"~crNode() = "<<getName().c_str()<<std::endl;
}

void crNode::addParent(CRCore::crGroup* node)
{
    m_parentArray.push_back(node);
}

void crNode::removeParent(CRCore::crGroup* node)
{
    ParentArray::iterator pitr = std::find(m_parentArray.begin(),m_parentArray.end(),node);
    if (pitr!=m_parentArray.end()) m_parentArray.erase(pitr);
}

void crNode::accept(crNodeVisitor& nv)
{
    if (nv.validNodeMask(*this)) 
    {
        nv.pushOntoNodePath(this);
        nv.apply(*this);
        nv.popFromNodePath();
    }
}


void crNode::ascend(crNodeVisitor& nv)
{
    std::for_each(m_parentArray.begin(),m_parentArray.end(),crNodeAcceptOp(nv));
}

void crNode::setStateSet(CRCore::crStateSet* stateset)
{
	if (m_stateset==stateset) return;

	int delta_update = 0;
	//int delta_event = 0;

	if (m_stateset.valid())
	{
		m_stateset->removeParent(this);
		if (m_stateset->requiresUpdateTraversal()) --delta_update;
		//if (m_stateset->requiresEventTraversal()) --delta_event;
	}

	m_stateset = stateset;

	if (m_stateset.valid())
	{
		m_stateset->addParent(this);
		if (m_stateset->requiresUpdateTraversal()) ++delta_update;
		//if (m_stateset->requiresEventTraversal()) ++delta_event;
	}

	if (delta_update!=0)
	{
		setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+delta_update);
	}

	//if (delta_event!=0)
	//{
	//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+delta_event);
	//}
}

CRCore::crStateSet* crNode::getOrCreateStateSet()
{
    if (!m_stateset) m_stateset = new crStateSet;
    return m_stateset.get();
}

NodePathList crNode::getParentalNodePaths(CRCore::crNode* haltTraversalAtNode) const
{
	CollectParentPaths cpp(haltTraversalAtNode);
	const_cast<crNode*>(this)->accept(cpp);
	return cpp.m_nodePaths;
}
void crNode::setDataClass(CRCore::crData *data)
{
	crBase::setDataClass(data);
	if(hasHandle(WCH_UPDATEVISITOR))
	{
        setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	}
}
void crNode::addUpdateCallback(crNodeCallback* nc)
{
	if(!nc) return;

    if (m_nNumChildrenRequiringUpdateTraversal==0 && !m_parentArray.empty())
    {
        int delta = 0;
        if (m_refUpdateCallback.valid()) --delta;
        if (nc) ++delta;
        if (delta!=0)
        {
            for(ParentArray::iterator itr =m_parentArray.begin();
                itr != m_parentArray.end();
                ++itr)
            {    
                (*itr)->setNumChildrenRequiringUpdateTraversal(
                        (*itr)->getNumChildrenRequiringUpdateTraversal()+delta );
            }

        }
    }

	if(m_refUpdateCallback.valid())
		m_refUpdateCallback->addNestedCallback(nc);
	else
		m_refUpdateCallback = nc;
}

void crNode::removeUpdateCallback(const std::string &name)
{
	if(m_refUpdateCallback.valid())
	{
		if(name.compare(m_refUpdateCallback->_name()) == 0)
		{
			m_refUpdateCallback = m_refUpdateCallback->getNestedCallback();
		}
		else m_refUpdateCallback->removeNestedCallback(name);
	}
}

crNodeCallback* crNode::getUpdateCallback(const std::string &name)
{
	if(m_refUpdateCallback.valid())
	{
		if(name.compare(m_refUpdateCallback->_name()) == 0)
		{
			return m_refUpdateCallback.get();
		}
		else return m_refUpdateCallback->getNestedCallback(name);
	}
	return NULL;
}

void crNode::setNumChildrenRequiringUpdateTraversal(unsigned int num)
{
    if (m_nNumChildrenRequiringUpdateTraversal==num) return;

    // note, if m_refUpdateCallback is set then the
    // parents won't be affected by any changes to
    // _numChildrenRequiringUpdateTraversal so no need to inform them.
    if (!m_refUpdateCallback && !m_parentArray.empty())
    {
    
        // need to pass on changes to parents.        
        int delta = 0;
        if (m_nNumChildrenRequiringUpdateTraversal>0) --delta;
        if (num>0) ++delta;
        if (delta!=0)
        {
            // the number of callbacks has changed, need to pass this
            // on to parents so they know whether app traversal is
            // reqired on this subgraph.
            for(ParentArray::iterator itr =m_parentArray.begin();
                itr != m_parentArray.end();
                ++itr)
            {    
                (*itr)->setNumChildrenRequiringUpdateTraversal(
                    (*itr)->getNumChildrenRequiringUpdateTraversal()+delta
                    );
            }

        }
    }
    
    // finally update this objects value.
    m_nNumChildrenRequiringUpdateTraversal=num;    
}

//void crNode::setUpdateUniformCallback(crNodeCallback* nc)
//{
//	// if no changes just return.
//	if (m_refUpdateUniformCallback==nc) return;
//
//	// app callback has been changed, will need to update
//	// both m_refUpdateCallback and possibly the numChildrenRequiringAppTraversal
//	// if the number of callbacks changes.
//
//
//	// update the parents numChildrenRequiringAppTraversal
//	// note, if _numChildrenRequiringUpdateTraversal!=0 then the
//	// parents won't be affected by any app callback change,
//	// so no need to inform them.
//	if (m_nNumChildrenRequiringUpdateUniformTraversal==0 && !m_parentArray.empty())
//	{
//		int delta = 0;
//		if (m_refUpdateUniformCallback.valid()) --delta;
//		if (nc) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentArray::iterator itr =m_parentArray.begin();
//				itr != m_parentArray.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringUpdateUniformTraversal(
//					(*itr)->getNumChildrenRequiringUpdateUniformTraversal()+delta );
//			}
//
//		}
//	}
//
//	// set the app callback itself.
//	m_refUpdateUniformCallback = nc;
//}
//
//void crNode::setNumChildrenRequiringUpdateUniformTraversal(unsigned int num)
//{
//	// if no changes just return.
//	if (m_nNumChildrenRequiringUpdateUniformTraversal==num) return;
//
//	// note, if m_refUpdateCallback is set then the
//	// parents won't be affected by any changes to
//	// _numChildrenRequiringUpdateTraversal so no need to inform them.
//	if (!m_refUpdateUniformCallback && !m_parentArray.empty())
//	{
//
//		// need to pass on changes to parents.        
//		int delta = 0;
//		if (m_nNumChildrenRequiringUpdateUniformTraversal>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentArray::iterator itr =m_parentArray.begin();
//				itr != m_parentArray.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringUpdateUniformTraversal(
//					(*itr)->getNumChildrenRequiringUpdateUniformTraversal()+delta
//					);
//			}
//
//		}
//	}
//
//	// finally update this objects value.
//	m_nNumChildrenRequiringUpdateUniformTraversal=num;    
//}
//
//void crNode::setEventCallback(crNodeCallback* nc)
//{
//	// if no changes just return.
//	if (m_eventCallback==nc) return;
//
//	// event callback has been changed, will need to Event
//	// both _EventCallback and possibly the numChildrenRequiringAppTraversal
//	// if the number of callbacks changes.
//
//
//	// Event the parents numChildrenRequiringAppTraversal
//	// note, if m_numChildrenRequiringEventTraversal!=0 then the
//	// parents won't be affected by any app callback change,
//	// so no need to inform them.
//	if (m_numChildrenRequiringEventTraversal==0 && !m_parentArray.empty())
//	{
//		int delta = 0;
//		if (m_eventCallback.valid()) --delta;
//		if (nc) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentArray::iterator itr =m_parentArray.begin();
//				itr != m_parentArray.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringEventTraversal(
//					(*itr)->getNumChildrenRequiringEventTraversal()+delta );
//			}
//
//		}
//	}
//
//	// set the app callback itself.
//	m_eventCallback = nc;
//
//}
//
//void crNode::setNumChildrenRequiringEventTraversal(unsigned int num)
//{
//	// if no changes just return.
//	if (m_numChildrenRequiringEventTraversal==num) return;
//
//	// note, if _EventCallback is set then the
//	// parents won't be affected by any changes to
//	// m_numChildrenRequiringEventTraversal so no need to inform them.
//	if (!m_eventCallback && !m_parentArray.empty())
//	{
//
//		// need to pass on changes to parents.        
//		int delta = 0;
//		if (m_numChildrenRequiringEventTraversal>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentArray::iterator itr =m_parentArray.begin();
//				itr != m_parentArray.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringEventTraversal(
//					(*itr)->getNumChildrenRequiringEventTraversal()+delta
//					);
//			}
//
//		}
//	}
//
//	// finally Event this objects value.
//	m_numChildrenRequiringEventTraversal=num;
//
//}
//

void crNode::setCullingActive(bool active)
{
    // if no changes just return.
    if (m_bCullingActive == active) return;
    
    // culling active has been changed, will need to update
    // both _cullActive and possibly the parents numChildrenWithCullingDisabled
    // if culling disabled changes.

    // update the parents _numChildrenWithCullingDisabled
    // note, if _numChildrenWithCullingDisabled!=0 then the
    // parents won't be affected by any app callback change,
    // so no need to inform them.
    if (m_nNumChildrenWithCullingDisabled==0 && !m_parentArray.empty())
    {
        int delta = 0;
        if (!m_bCullingActive) --delta;
        if (!active) ++delta;
        if (delta!=0)
        {
            // the number of callbacks has changed, need to pass this
            // on to parents so they know whether app traversal is
            // reqired on this subgraph.
            for(ParentArray::iterator itr =m_parentArray.begin();
                itr != m_parentArray.end();
                ++itr)
            {    
                (*itr)->setNumChildrenWithCullingDisabled(
                        (*itr)->getNumChildrenWithCullingDisabled()+delta );
            }

        }
    }

    // set the cullingActive itself.
    m_bCullingActive = active;
}

void crNode::setNumChildrenWithCullingDisabled(unsigned int num)
{
    // if no changes just return.
    if (m_nNumChildrenWithCullingDisabled==num) return;

    // note, if m_bCullingActive is false then the
    // parents won't be affected by any changes to
    // _numChildrenWithCullingDisabled so no need to inform them.
    if (m_bCullingActive && !m_parentArray.empty())
    {
    
        // need to pass on changes to parents.        
        int delta = 0;
        if (m_nNumChildrenWithCullingDisabled>0) --delta;
        if (num>0) ++delta;
        if (delta!=0)
        {
            // the number of callbacks has changed, need to pass this
            // on to parents so they know whether app traversal is
            // reqired on this subgraph.
            for(ParentArray::iterator itr =m_parentArray.begin();
                itr != m_parentArray.end();
                ++itr)
            {    
                (*itr)->setNumChildrenWithCullingDisabled(
                    (*itr)->getNumChildrenWithCullingDisabled()+delta
                    );
            }

        }
    }
    
	// finally update this objects value.
	m_nNumChildrenWithCullingDisabled=num;
}


void crNode::setNumChildrenWithOccluderNodes(unsigned int num)
{
	//CRCore::notify(CRCore::FATAL)<<"setNumChildrenWithOccluderNodes name = "<<m_name<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<"setNumChildrenWithOccluderNodes num = "<<num<<std::endl;

	// if no changes just return.
	if (m_nNumChildrenWithOccluderNodes==num) return;

	// note, if this node is a OccluderNode then the
	// parents won't be affected by any changes to	
	// _numChildrenWithOccluderNodes so no need to inform them.
	if (!dynamic_cast<crOccluderNode*>(this) && !m_parentArray.empty())
	{

		// need to pass on changes to parents.        
		int delta = 0;
		if (m_nNumChildrenWithOccluderNodes>0) --delta;
		if (num>0) ++delta;
		if (delta!=0)
		{
			// the number of callbacks has changed, need to pass this
			// on to parents so they know whether app traversal is
			// reqired on this subgraph.
			for(ParentArray::iterator itr =m_parentArray.begin();
				itr != m_parentArray.end();
				++itr)
			{    
				(*itr)->setNumChildrenWithOccluderNodes(
					(*itr)->getNumChildrenWithOccluderNodes()+delta
					);
			}

		}
	}

	// finally update this objects value.
	m_nNumChildrenWithOccluderNodes=num;
}


bool crNode::containsOccluderNodes() const
{
	return m_nNumChildrenWithOccluderNodes>0 || dynamic_cast<const crOccluderNode*>(this);
}

void crNode::setNumChildrenWithEventGroups(unsigned int num)
{
	// if no changes just return.
	if (m_nNumChildrenWithEventGroups==num) return;

	// note, if this node is a OccluderNode then the
	// parents won't be affected by any changes to
	// _numChildrenWithOccluderNodes so no need to inform them.
	if (!dynamic_cast<crEventGroup*>(this) && !m_parentArray.empty())
	{

		// need to pass on changes to parents.        
		int delta = 0;
		if (m_nNumChildrenWithEventGroups>0) --delta;
		if (num>0) ++delta;
		if (delta!=0)
		{
			// the number of callbacks has changed, need to pass this
			// on to parents so they know whether app traversal is
			// reqired on this subgraph.
			for(ParentArray::iterator itr =m_parentArray.begin();
				itr != m_parentArray.end();
				++itr)
			{    
				(*itr)->setNumChildrenWithEventGroups(
					(*itr)->getNumChildrenWithEventGroups()+delta
					);
			}

		}
	}

	// finally update this objects value.
	m_nNumChildrenWithEventGroups=num;
}

//void crNode::setNumChildrenRequiringCalcShadow(unsigned int num)
//{
//	if (m_nNumChildrenRequiringCalcShadow == num) return;
//
//	if (!m_parentArray.empty())
//	{    
//		int delta = 0;
//		if (m_nNumChildrenRequiringCalcShadow>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			for(ParentArray::iterator itr =m_parentArray.begin();
//				itr != m_parentArray.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringCalcShadow(
//					(*itr)->getNumChildrenRequiringCalcShadow()+delta
//					);
//			}
//
//		}
//	}
//
//	m_nNumChildrenRequiringCalcShadow = num;
//}
//
bool crNode::containsEventGroups() const
{
	return m_nNumChildrenWithEventGroups>0 || dynamic_cast<const crEventGroup*>(this);
}

void crNode::setBoundBox(const crVector3& center,const crVector3& length)
{
	//CRCore::notify(CRCore::FATAL)<<" crNode::setBoundBox: center= "<<center<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<" crNode::setBoundBox: length= "<<length<<std::endl;

	m_bbox.set(center-length,center+length);
	m_boundSphere.init();
	m_boundSphere.expandBy(m_bbox);
	m_boundSetted = true;
}

bool crNode::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();
	return false;
}

void crNode::dirtyBound()
{
	if (m_bBoundSphere_computed)
	{
		m_bBoundSphere_computed = false;

		// dirty parent bounding sphere's to ensure that all are valid.
		for(ParentArray::iterator itr=m_parentArray.begin();
			itr!=m_parentArray.end();
			++itr)
		{
			(*itr)->dirtyBound();
		}
	}
}

void crNode::releaseObjects(crState* state)
{
	crBase::releaseObjects(state);
    if (m_stateset.valid())
	{
		m_stateset->releaseObjects(state);
	}
	//if(m_physicsInited)
	//	releaseOde();
}

void crNode::setHeightfieldDataID(void *id)
{
	m_heightData = id;
}

void crNode::releaseOde() 
{
	if(m_physicsObjID) 
	{
		dGeomDestroy((dGeomID)m_physicsObjID);
		//crOdeWorld::staticGeoMapErase((dGeomID)m_physicsObjID);
		m_physicsObjID = NULL;
	}
	if(m_heightData) 
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crObject::releaseOde() dGeomTriMeshDataDestroy"<<std::endl;
		dGeomHeightfieldDataDestroy((dHeightfieldDataID)m_heightData);
		m_heightData = NULL;
		//CRCore::notify(CRCore::ALWAYS)<<"crObject::releaseOde() dGeomTriMeshDataDestroy end"<<std::endl;
	}
	//if(m_physicsMeshVtxArray.valid()) m_physicsMeshVtxArray->clear();
	//if(m_physicsMeshIndexArray.valid()) m_physicsMeshIndexArray->clear();
	m_physicsInited = false;
}

std::string crNode::getDescriptionsInString()const
{
	std::string description;
	for( DescriptionArray::const_iterator itr = m_descriptionArray.begin();
		 itr != m_descriptionArray.end();
		 ++itr )
	{
        description += *itr/* + '\n'*/;
	}
	return description;
}

void crNode::setDescriptionsInString( const std::string &command )
{
    m_descriptionArray.clear();

	//unsigned int stringLength = command.length();
	unsigned int front_of_line = 0;
	unsigned int end_of_line = 0;
	while (end_of_line<command.size())
	{
/*		if (command[end_of_line]=='\r')
		{
			addDescription( std::string( command, front_of_line, end_of_line-front_of_line) );

			if (end_of_line+1<command.size() &&
				command[end_of_line+1]=='\n') ++end_of_line;

			++end_of_line;
			front_of_line = end_of_line;
		}
		else */if (command[end_of_line]=='\n')
		{
			++end_of_line;
			addDescription( std::string( command, front_of_line, end_of_line-front_of_line) );

			front_of_line = end_of_line;
		}
		else ++end_of_line;
	}
	if (front_of_line<end_of_line)
	{
		addDescription( std::string( command, front_of_line, end_of_line-front_of_line) );
	}
}

void crNode::addCullCallback(crNodeCallback* nc)
{
	if(!nc) return;
	if(m_refCullCallback.valid())
		m_refCullCallback->addNestedCallback(nc);
	else
        m_refCullCallback = nc;
}

void crNode::removeCullCallback(const std::string &name)
{
	if(m_refCullCallback.valid())
	{
		if(name.compare(m_refCullCallback->_name()) == 0)
		{
			m_refCullCallback = m_refCullCallback->getNestedCallback();
		}
		m_refCullCallback->removeNestedCallback(name);
	}
}

crNodeCallback* crNode::getCullCallback(const std::string &name)
{
	if(m_refCullCallback.valid())
	{
		if(name.compare(m_refCullCallback->_name()) == 0)
		{
			return m_refCullCallback.get();
		}
		else return m_refCullCallback->getNestedCallback(name);
	}
	return NULL;
}

void crNode::addCollideCallback(crCollideCallback* nc)
{
	if(!nc) return;
	if(m_collideCallback.valid())
		m_collideCallback->addNestedCallback(nc);
	else
		m_collideCallback = nc;
}

void crNode::removeCollideCallback(const std::string &name)
{
	if(m_collideCallback.valid())
	{
		if(name.compare(m_collideCallback->_name()) == 0)
		{
			m_collideCallback = m_collideCallback->getNestedCallback();
		}
		m_collideCallback->removeNestedCallback(name);
	}
}

crCollideCallback* crNode::getCollideCallback(const std::string &name)
{
	if(m_collideCallback.valid())
	{
		if(name.compare(m_collideCallback->_name()) == 0)
		{
			return m_collideCallback.get();
		}
		else return m_collideCallback->getNestedCallback(name);
	}
	return NULL;
}

//void crNode::physicsCulled(bool culled)
//{
//	//if(m_physicsObjID)
//	//{
//	//	if(culled)
//	//	    dGeomDisable((dGeomID)m_physicsObjID);
//	//	else
//	//		dGeomEnable((dGeomID)m_physicsObjID);
//	//}
//}

crNode::EventMap &crNode::getEventMap()
{
	return m_eventMap;
}

void crNode::insertEvent(__int64 kbmsg, CRCore::crEventCallback *callback)
{
	m_eventMap.insert(std::make_pair(kbmsg,callback));
}

void crNode::doEvent(__int64 kbmsg, __int64 param)
{
	crBase::doEvent(kbmsg,param);

	for( EventMap::iterator itr = m_eventMap.find(kbmsg); 
		itr != m_eventMap.end() && itr->first == kbmsg; 
		itr++ )
	{
#if _DEBUG
		try
		{
#endif
		itr->second->initCallbackTask();
		if(dynamic_cast<PhysicsObjectCallback *>(itr->second.get()))
			(*(dynamic_cast<PhysicsObjectCallback *>(itr->second.get())))(*this,param);
		else (*(itr->second))();
		if(!itr->second->continueCallbackTask()) break;
#if _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crNode::doEvent error "<<itr->second->_name()<<std::endl;
		}
#endif
	}
}

crEventCallback* crNode::getEventCallback(__int64 kbmsg, const std::string& name)
{
	if(m_eventMap.empty()) return NULL;

	for( EventMap::iterator itr = m_eventMap.find(kbmsg); 
		itr != m_eventMap.end() && itr->first == kbmsg; 
		itr++ )
	{
		if(name.compare(itr->second->_name()) == 0)
			return itr->second.get();
	}
	return NULL;
}
void crNode::setRenderInited(bool bln)
{
	m_renderInited = bln;
}
bool crNode::getRenderInited()
{
	return m_renderInited;
}