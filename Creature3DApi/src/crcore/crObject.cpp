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
#include <CRCore\crObject.h>
#include <CRCore\crPolygonMode.h>
#include <CRCore\crStateAttribute.h>
#include <CRCore\crShapeDrawable.h>
#include <CRCore\crDisplaySettings.h>
#include <ode/ode.h>

using namespace std;
using namespace CRCore;


crObject::crObject()
{
	m_NodeType = NT_OBJECT;
	m_triData = NULL;
    
	//m_bboxDrawable = new crShapeDrawable();

	//crPolygonMode *polygonMode = new crPolygonMode;
	//polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
	//m_bboxDrawable->getOrCreateStateSet()->setAttribute(polygonMode, CRCore::crStateAttribute::ON);
	//m_bboxDrawable->getOrCreateStateSet()->setMode( CRCore::crStateAttribute::VRMode(GL_LIGHTING,NULL), CRCore::crStateAttribute::OFF );
 //   m_bboxDrawable->setUseDisplayList(false);
	//m_bboxDrawable->setUseVertexBufferObjects(false);
	//setName("O");
	m_effectByShadow = true;
	m_needSelectLight = true;
	m_acceptGI = 1;
	m_drawOutLine = 0;
	m_outlineColorMode = OL_Black;
}

crObject::crObject(const crObject& object,const crCopyOp& copyop):
  crNode(object,copyop),
  m_effectByShadow(object.m_effectByShadow),
  m_needSelectLight(object.m_needSelectLight),
  m_triData(NULL),
  m_needLight(object.m_needLight),
  m_avoidLight(object.m_avoidLight),
  m_acceptGI(object.m_acceptGI),
  m_drawOutLine(object.m_drawOutLine),
  m_outlineColorMode(OL_Black)
{
  m_NodeType = NT_OBJECT;
  for (int i = 0; i<object.m_drawables.size(); i++)
  {
	  crDrawable* drawable = copyop(object.m_drawables[i].get());
	  if (drawable) addDrawable(drawable);
  }
  //for(DrawableList::const_iterator itr=object.m_drawables.begin();
  //  itr!=object.m_drawables.end();
  //  ++itr)
  //{
  //  crDrawable* drawable = copyop(itr->get());
  //  if (drawable) addDrawable(drawable);
  //}
}

crObject::~crObject()
{
  // remove reference to this from children's parent lists.
	for (int i = 0; i<m_drawables.size(); i++)
	{
		m_drawables[i]->removeParent(this);
	}
  //for(DrawableList::iterator itr=m_drawables.begin();
  //  itr!=m_drawables.end();
  //  ++itr)
  //{
  //  (*itr)->removeParent(this);
  //}
}

void crObject::releaseObjects(CRCore::crState* state)
{
	//if(m_physicsInited)
	//	releaseOde();

	crNode::releaseObjects(state);

	int count = m_drawables.size();
	for (int i = 0; i < count; i++)
	{
		if (m_drawables[i].valid())
			m_drawables[i]->releaseObjects(state);
	}
	//for(DrawableList::iterator itr=m_drawables.begin();
	//	itr!=m_drawables.end();
	//	++itr)
	//{
	//	if (itr->valid())
	//		(*itr)->releaseObjects(state);
	//}
}

void crObject::releaseOde()
{
	//if(m_physicsObjID) 
	//{
	//	dGeomDestroy((dGeomID)m_physicsObjID);
	//	m_physicsObjID = NULL;
	//}
	if(m_triData) 
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crObject::releaseOde() dGeomTriMeshDataDestroy"<<std::endl;
		dGeomTriMeshDataDestroy((dTriMeshDataID)m_triData);
		m_triData = NULL;
		//CRCore::notify(CRCore::ALWAYS)<<"crObject::releaseOde() dGeomTriMeshDataDestroy end"<<std::endl;
	}

	crNode::releaseOde();
}

void crObject::compileDrawables(crState& state)
{
	for (int i = 0; i<m_drawables.size(); i++)
	{
		m_drawables[i]->compile(state);
	}
	//for(DrawableList::iterator itr = m_drawables.begin();
	//	itr!=m_drawables.end();
	//	++itr)
	//{
	//	(*itr)->compile(state);
	//}
}

bool crObject::containsDrawable(const crDrawable* gset)
{
	const crDrawable::ParentList &parents = gset->getParents();
	for (int i = 0; i < parents.size(); i++)
	{
		if (parents[i] == this) return true;
	}
	//for(crDrawable::ParentList::const_iterator itr = parents.begin();
	//	itr != parents.end();
	//	++itr)
	//{
	//	if( (*itr) == this ) return true;
	//}
	return false;
}

bool crObject::containsDrawable(const crDrawable* gset) const
{
	const crDrawable::ParentList &parents = gset->getParents();
	for (int i = 0; i < parents.size(); i++)
	{
		if (parents[i] == this) return true;
	}
	//for(crDrawable::ParentList::const_iterator itr = parents.begin();
	//	itr != parents.end();
	//	++itr)
	//{
	//	if( (*itr) == this ) return true;
	//}
	return false;
}

bool crObject::addDrawable( crDrawable *drawable )
{
  if (drawable && !containsDrawable(drawable))
  {
    // note ref_ptr<> automatically handles incrementing drawable's reference count.
    m_drawables.push_back(drawable);

    // register as parent of drawable.
    drawable->addParent(this);

    if (drawable->requiresUpdateTraversal()/*getUpdateCallback()*/)
    {
      setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
    }

	//if (drawable->requiresEventTraversal()/*getEventCallback()*/)
	//{
	//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+1);
	//}

	//if (drawable->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/)
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()+1);
	//}

    dirtyBound();        

    return true;
  }
  else return false;
}


bool crObject::removeDrawable( crDrawable *drawable )
{
	DrawableList::iterator pitr = find(m_drawables.begin(),m_drawables.end(),drawable);
	if (pitr != m_drawables.end()) 
	{
		if ((*pitr)->requiresUpdateTraversal()/*getUpdateCallback()*/)
		    setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
		//if ((*pitr)->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/)
		//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()-1);
		//if ((*pitr)->requiresEventTraversal()/*getEventCallback()*/)
		//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);

		(*pitr)->removeParent(this);
		m_drawables.erase(pitr);
		dirtyBound();
		return true;
	}
	return false;
	//return removeDrawable(getDrawableIndex(drawable));
}

bool crObject::removeDrawable(unsigned int pos,unsigned int numDrawablesToRemove)
{
  if (pos<m_drawables.size() && numDrawablesToRemove>0)
  {
    unsigned int endOfRemoveRange = pos+numDrawablesToRemove;
    if (endOfRemoveRange > m_drawables.size())
    {
//      notify(DEBUG_INFO)<<"Warning: crObject::removeDrawable(i,numDrawablesToRemove) has been passed an excessive number"<<std::endl;
//      notify(DEBUG_INFO)<<"         of drawables to remove, trimming just to end of drawable list."<<std::endl;
      endOfRemoveRange = m_drawables.size();
    }

    unsigned int updateCallbackRemoved = 0;
	unsigned int updateUniformCallbackRemoved = 0;
	unsigned int eventCallbackRemoved = 0;

    for(unsigned i=pos;i<endOfRemoveRange;++i)
    {
      // remove this crObject from the child parent list.
      m_drawables[i]->removeParent(this);
      // update the number of app calbacks removed
      if (m_drawables[i]->requiresUpdateTraversal()/* getUpdateCallback()*/) ++updateCallbackRemoved;
	  //if (m_drawables[i]->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/) ++updateUniformCallbackRemoved;
	  //if (m_drawables[i]->requiresEventTraversal()/*getEventCallback()*/) ++eventCallbackRemoved;
    }

    m_drawables.erase( m_drawables.begin()+pos,
                       m_drawables.begin()+endOfRemoveRange);

    if (updateCallbackRemoved)
    {
      setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-updateCallbackRemoved);
    }
	//if (updateUniformCallbackRemoved)
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()-updateUniformCallbackRemoved);
	//}
	//if (eventCallbackRemoved)
	//{
	//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-eventCallbackRemoved);
	//}

    dirtyBound();

    return true;
  }
  else return false;
}

bool crObject::replaceDrawable( crDrawable *origDrawable, crDrawable *newDrawable )
{
  if (newDrawable==NULL || origDrawable==newDrawable) return false;

  unsigned int pos = getDrawableIndex(origDrawable);
  if (pos<m_drawables.size())
  {
    return setDrawable(pos,newDrawable);
  }
  return false;
}

bool crObject::setDrawable( unsigned  int i, crDrawable* newDrawable )
{
  if (i<m_drawables.size() && newDrawable)
  {

    crDrawable* origDrawable = m_drawables[i].get();

    int delta = 0;
	int delta_updateUniform = 0;
	int delta_event = 0;
    if (origDrawable->requiresUpdateTraversal()/*getUpdateCallback()*/) --delta;
    if (newDrawable->requiresUpdateTraversal()/*getUpdateCallback()*/) ++delta;
    if (delta!=0)
    {
      setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+delta);
    }

	//if (origDrawable->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/) --delta_updateUniform;
	//if (newDrawable->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/) ++delta_updateUniform;
	//if (delta_updateUniform!=0)
	//{
	//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()+delta_updateUniform);
	//}

	//if (origDrawable->requiresEventTraversal()/*getEventCallback()*/) --delta_event;
	//if (newDrawable->requiresEventTraversal()/*getEventCallback()*/) ++delta_event;
	//if (delta_event!=0)
	//{
	//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+delta_event);
	//}

    // remove from origDrawable's parent list.
    origDrawable->removeParent(this);

    // note ref_ptr<> automatically handles decrementing origGset's reference count,
    // and inccrementing newGset's reference count.
    m_drawables[i] = newDrawable;

    // register as parent of child.
    newDrawable->addParent(this);


    dirtyBound();

    return true;
  }
  else return false;

}


bool crObject::computeBound() const
{
	m_boundSphere.init();

	m_bbox.init();

	for (int i = 0; i < m_drawables.size(); i++)
	{
		m_bbox.expandBy(m_drawables[i]->getBoundBox());
	}
 //   DrawableList::const_iterator itr;
 //   for(itr = m_drawables.begin();
 //       itr != m_drawables.end();
 //       ++itr)
	//{
	//	m_bbox.expandBy((*itr)->getBoundBox());
	//}

	if (m_bbox.valid())
	{
		m_boundSphere.expandBy(m_bbox);
		m_bBoundSphere_computed=true;

/*		switch(m_collideMode) 
		{
		case PrecisionCollide:
			break;
		case CollideWithBoundBox:
			if(m_physicsObjID)
			{
				dGeomBoxSetLengths((dGeomID)m_physicsObjID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
				dGeomSetPosition((dGeomID)m_physicsObjID,m_bbox.center().x(),m_bbox.center().y(),m_bbox.center().z());
			}
			break;
		case CollideWithBoundSphere:
			if(m_physicsObjID)
			{
				dGeomSphereSetRadius((dGeomID)m_physicsObjID,m_boundSphere.radius());
				dGeomSetPosition((dGeomID)m_physicsObjID,m_boundSphere.center().x(),m_boundSphere.center().y(),m_boundSphere.center().z());
			}
			break;
		}  */    

		//crBox *box = dynamic_cast<crBox *>(m_bboxDrawable->getShape());
		//if(!box)
		//{
		//	m_bboxDrawable->setShape(new crBox(m_bbox.center(),m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength()));
		//}
		//else
		//{
		//	box->set(m_bbox.center(),CRCore::crVector3(m_bbox.xLength()*0.5f,m_bbox.yLength()*0.5f,m_bbox.zLength()*0.5f));
		//}
		return true;
	}
	else
	{
		m_bBoundSphere_computed=true;
		return false;
	}
}

//void crObject::addObjectEvent(crObjectEvent *oe)
//{
//	if(!oe) return;
//	oe->setParentObject(this);
//    m_objectEventMap[oe->getEventType()] = oe;
//}
//
//crObject::crObjectEvent *crObject::getObjectEvent(crObjectEvent::EventType et)
//{
//	ObjectEventMap::iterator itr = m_objectEventMap.find(et);
//	if(itr != m_objectEventMap.end())
//		return itr->second.get();
//	else return NULL;
//}

void crObject::setEffectByShadow(bool effectByShadow)
{
	m_effectByShadow = effectByShadow;
}

void crObject::setNeedSelectLight(bool needSelectLight)
{
	m_needSelectLight = needSelectLight;
}

void crObject::swapBuffers(int frameNumber)
{//剪裁线程与绘制线程的同步
	if(m_swapFrameNumber != frameNumber)
	{
		int count = m_drawables.size();
		for (int i = 0; i<count; i++)
		{
			m_drawables[i]->swapBuffers(frameNumber);
		}
		//for( DrawableList::iterator itr = m_drawables.begin();
		//	itr != m_drawables.end();
		//	++itr )
		//{
		//	if(itr->valid())
		//		(*itr)->swapBuffers(frameNumber);
		//}
		crNode::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}

void crObject::addAvoidLight(const std::string &lsname)
{
    m_avoidLight.insert(lsname);
}

void crObject::addNeedLight(const std::string &lsname)
{
    m_needLight.insert(lsname);
}

void crObject::clearNeedLight()
{
	m_needLight.clear();
}

void crObject::clearAvoidLight()
{
	m_avoidLight.clear();
}

void crObject::resetSelectedLightList()
{
	if(crDisplaySettings::instance()->getRunMode()==0)
		m_selectedLightList.clear();
}
void crObject::selectedLight(crLightSource *ls)
{
	if(crDisplaySettings::instance()->getRunMode()==0)
		m_selectedLightList.push_back(ls);
}

crObject::SelectedLightList &crObject::getSelectedLightList()
{
	return m_selectedLightList;
}

crObject::LightNameSet &crObject::getAvoidLightSet()
{
    return m_avoidLight;
}

crObject::LightNameSet &crObject::getNeedLightSet()
{
	return m_needLight;
}
void crObject::setDrawOutLine(char outline)
{
	m_drawOutLine = outline;
}
void crObject::setOutlineColorMode(OutlineColorMode mode)
{
	m_outlineColorMode = mode;
}