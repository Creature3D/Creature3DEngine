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
#include <CRCore/crDrawable.h>
#include <CRCore/crNode.h>
#include <CRCore/crTimer.h>
#include <CRCore/crNotify.h>
#include <algorithm>

using namespace std;
using namespace CRCore;

unsigned int crDrawable::s_numberDrawablesReusedLastInLastFrame = 0;
unsigned int crDrawable::s_numberNewDrawablesInLastFrame = 0;
unsigned int crDrawable::s_numberDeletedDrawablesInLastFrame = 0;
unsigned int crDrawable::s_minimumNumberOfDisplayListsToRetainInCache = 0;
unsigned int crDrawable::s_numberVBOs = 0;
crMutex crDrawable::s_mutex_deletedDisplayListCache;
crMutex crDrawable::s_mutex_deletedVertexBufferObjectCache;
crDrawable::DeletedDisplayListCache crDrawable::s_deletedDisplayListCache;
crDrawable::DeletedDisplayListCache crDrawable::s_deletedVertexBufferObjectCache;

void crDrawable::setMinimumNumberOfDisplayListsToRetainInCache(unsigned int minimum)
{
	s_minimumNumberOfDisplayListsToRetainInCache = minimum;
}

unsigned int crDrawable::getMinimumNumberOfDisplayListsToRetainInCache()
{
	return s_minimumNumberOfDisplayListsToRetainInCache;
}

void crDrawable::deleteDisplayList(unsigned int contextID,unsigned int globj, unsigned int sizeHint)
{
	if (globj!=0)
	{
		CRCore::ScopedLock<crMutex> lock(s_mutex_deletedDisplayListCache);

		// insert the globj into the cache for the appropriate context.
		s_deletedDisplayListCache[contextID].insert(DisplayListMap::value_type(sizeHint,globj));
	}
}

void crDrawable::deleteVertexBufferObject(unsigned int contextID,GLuint globj)
{
	if (globj!=0)
	{
		CRCore::ScopedLock<crMutex> lock(s_mutex_deletedVertexBufferObjectCache);
		// insert the globj into the cache for the appropriate context.
		s_deletedVertexBufferObjectCache[contextID].insert(DisplayListMap::value_type(0,globj));
	}
}

crDrawable::crDrawable():
	m_color(1.0f,1.0f,1.0f,1.0f)
{
	m_bbox_computed = false;

	// Note, if your are defining a subclass from drawable which is
	// dynamically updated then you should set both the following to
	// to false in your constructor.  This will prevent any display
	// lists from being automatically created and safeguard the
	// dynamic updating of data.
	m_supportsDisplayList = false;
	m_useDisplayList = false;

	m_supportsVertexBufferObjects = false;
	m_useVertexBufferObjects = false;
	m_useIndexBufferObjects = false;
	//m_culled = -1;

	m_numChildrenRequiringUpdateTraversal = 0;
	//m_numChildrenRequiringEventTraversal = 0;
 //   m_numChildrenRequiringUpdateUniformTraversal = 0;

	m_type = DUMMY;

	m_visiable = true;
	m_abortCurrentRendering = false;
	//m_stateset_optimizerd = false;
}

crDrawable::crDrawable(const crDrawable& drawable,const crCopyOp& copyop):
  crBase(drawable,copyop),
  m_color(drawable.m_color),
  m_parents(), // leave empty as parentList is managed by Geode
  m_stateset(copyop(drawable.m_stateset.get())),
  m_bbox(drawable.m_bbox),
  m_bbox_computed(drawable.m_bbox_computed),
  //m_culled(drawable.m_culled),
  m_shape(copyop(drawable.m_shape.get())),
  m_supportsDisplayList(drawable.m_supportsDisplayList),
  m_useDisplayList(drawable.m_useDisplayList),
  m_supportsVertexBufferObjects(drawable.m_supportsVertexBufferObjects),
  m_useVertexBufferObjects(drawable.m_useVertexBufferObjects),
  m_updateCallback(drawable.m_updateCallback),
  m_numChildrenRequiringUpdateTraversal(drawable.m_numChildrenRequiringUpdateTraversal),
 // m_updateUniformCallback(drawable.m_updateUniformCallback),
  //m_numChildrenRequiringUpdateUniformTraversal(drawable.m_numChildrenRequiringUpdateUniformTraversal),
//  m_eventCallback(drawable.m_eventCallback),
 // m_numChildrenRequiringEventTraversal(drawable.m_numChildrenRequiringEventTraversal),
  m_cullCallback(drawable.m_cullCallback),
  m_type(drawable.m_type),
  m_visiable(drawable.m_visiable),
  m_abortCurrentRendering(drawable.m_abortCurrentRendering)
  //m_stateset_optimizerd(drawable.m_stateset_optimizerd),
  //m_textures_stateset(copyop(drawable.m_textures_stateset.get())),
  //m_others_stateset(copyop(drawable.m_others_stateset.get()))
  //m_material_stateset(copyop(drawable.m_material_stateset.get()))
{
	m_drawCallback = dynamic_cast<crDrawable::DrawCallback *>(copyop(drawable.m_drawCallback.get()));
}

crDrawable::~crDrawable()
{
	//setStateSet(0);
	//dirtyDisplayList();
}

//void crDrawable::setStateSetOptimizerd(bool stateSetOptimizerd) 
//{ 
//	m_stateset_optimizerd = stateSetOptimizerd; 
//}

//void crDrawable::setTextureStateSet(crStateSet *ss) 
//{ 
//	m_textures_stateset = ss; 
//}
//
//void crDrawable::setOthersStateSet(crStateSet *ss) 
//{
//	m_others_stateset = ss; 
//}
//
//void crDrawable::setMaterialStateSet(crStateSet *ss) 
//{ 
//	m_material_stateset = ss; 
//}

void crDrawable::addParent(crNode* node)
{
    m_parents.push_back(node);    
}
void crDrawable::setColor(const crVector4& color) { m_color = color; }
void crDrawable::setColor(const crVector4& color)const { m_color = color; }
const crVector4& crDrawable::getColor()const { return m_color; }
crVector4& crDrawable::getColor() { return m_color; }
const CRCore::crVector3& crDrawable::getPosition() const { return m_bbox.m_max; }//对于drawable是无意义的
void crDrawable::removeParent(crNode* node)
{
    ParentList::iterator pitr = find(m_parents.begin(),m_parents.end(),node);
    if (pitr != m_parents.end()) 
      m_parents.erase(pitr);
}

struct ComputeBound : public crDrawable::PrimitiveFunctor
{
        ComputeBound():m_vertices(0) {}
        
        virtual void setVertexArray(unsigned int,const crVector2*) 
        {
            notify(WARN)<<"ComputeBound does not support Vec2* vertex arrays"<<std::endl;
        }

        virtual void setVertexArray(unsigned int,const crVector3* vertices) { m_vertices = vertices; }

        virtual void setVertexArray(unsigned int,const crVector4*) 
        {
            notify(WARN)<<"ComputeBound does not support crVector4* vertex arrays"<<std::endl;
        }

        virtual void drawArrays(unsigned int,int first,int count)
        {
            if (m_vertices)
            {
                const CRCore::crVector3* vert = m_vertices+first;
                for(;count>0;--count,++vert)
                {
                    m_bb.expandBy(*vert);
                }
            }
        }

        virtual void drawElements(unsigned int,int count,const unsigned char* indices)
        {
            if (m_vertices)
            {
                for(;count>0;--count,++indices)
                {
                    m_bb.expandBy(m_vertices[*indices]);
                }
            }
        }

        virtual void drawElements(unsigned int,int count,const unsigned short* indices)
        {
            if (m_vertices)
            {
                for(;count>0;--count,++indices)
                {
                    m_bb.expandBy(m_vertices[*indices]);
                }
            }
        }

        virtual void drawElements(unsigned int,int count,const unsigned int* indices)
        {
            if (m_vertices)
            {
                for(;count>0;--count,++indices)
                {
                    m_bb.expandBy(m_vertices[*indices]);
                }
            }
        }

        virtual void begin(unsigned int) {}
        virtual void vertex(const crVector2& vert) { m_bb.expandBy(crVector3(vert[0],vert[1],0.0f)); }
        virtual void vertex(const crVector3& vert) { m_bb.expandBy(vert); }
        virtual void vertex(const crVector4& vert) { if (vert[3]!=0.0f) m_bb.expandBy(crVector3(vert[0],vert[1],vert[2])/vert[3]); }
        virtual void vertex(float x,float y) { m_bb.expandBy(x,y,1.0f); }
        virtual void vertex(float x,float y,float z) { m_bb.expandBy(x,y,z); }
        virtual void vertex(float x,float y,float z,float w) { if (w!=0.0f) m_bb.expandBy(x/w,y/w,z/w); }
        virtual void end() {}
        
        const crVector3*  m_vertices;
        crBoundingBox     m_bb;
};

bool crDrawable::computeBound() const
{
    ComputeBound cb;

    crDrawable* non_const_this = const_cast<crDrawable*>(this);
    non_const_this->accept(cb);
    //this->accept(cb);
    m_bbox = cb.m_bb;
    m_bbox_computed = true;

    return true;
}

void crDrawable::setStateSet(crStateSet* stateset)
{
	// do nothing if nothing changed.
	if (m_stateset==stateset) return;

	// track whether we need to account for the need to do a update or event traversal.
	int delta_update = 0;
	int delta_event = 0;

	// remove this node from the current statesets parent list 
	if (m_stateset.valid())
	{
		m_stateset->removeParent(this);
		if (m_stateset->requiresUpdateTraversal()) --delta_update;
		//if (m_stateset->requiresEventTraversal()) --delta_event;
	}

	// set the stateset.
	m_stateset = stateset;

	// add this node to the new stateset to the parent list.
	if (m_stateset.valid())
	{
		m_stateset->addParent(this);
		if (m_stateset->requiresUpdateTraversal()) ++delta_update;
		//if (m_stateset->requiresEventTraversal()) ++delta_event;
	}


	// only inform parents if change occurs and drawable doesn't already have an update callback
	if (delta_update!=0 && !m_updateCallback)
	{
		for(ParentList::iterator itr=m_parents.begin();
			itr!=m_parents.end();
			++itr)
		{
			(*itr)->setNumChildrenRequiringUpdateTraversal( (*itr)->getNumChildrenRequiringUpdateTraversal()+delta_update );
		}
	}

	// only inform parents if change occurs and drawable doesn't already have an event callback
	//if (delta_event!=0 && !m_eventCallback)
	//{
	//	for(ParentList::iterator itr=m_parents.begin();
	//		itr!=m_parents.end();
	//		++itr)
	//	{
	//		(*itr)->setNumChildrenRequiringEventTraversal( (*itr)->getNumChildrenRequiringEventTraversal()+delta_event );
	//	}
	//}
}

void crDrawable::setNumChildrenRequiringUpdateTraversal(unsigned int num)
{
	// if no changes just return.
	if (m_numChildrenRequiringUpdateTraversal==num) return;

	// note, if _updateCallback is set then the
	// parents won't be affected by any changes to
	// _numChildrenRequiringUpdateTraversal so no need to inform them.
	if (!m_updateCallback && !m_parents.empty())
	{
		// need to pass on changes to parents.        
		int delta = 0;
		if (m_numChildrenRequiringUpdateTraversal>0) --delta;
		if (num>0) ++delta;
		if (delta!=0)
		{
			// the number of callbacks has changed, need to pass this
			// on to parents so they know whether app traversal is
			// reqired on this subgraph.
			for(ParentList::iterator itr =m_parents.begin();
				itr != m_parents.end();
				++itr)
			{    
				(*itr)->setNumChildrenRequiringUpdateTraversal( (*itr)->getNumChildrenRequiringUpdateTraversal()+delta );
			}
		}
	}

	// finally update this objects value.
	m_numChildrenRequiringUpdateTraversal=num;

}

//void crDrawable::setNumChildrenRequiringUpdateUniformTraversal(unsigned int num)
//{
//	// if no changes just return.
//	if (m_numChildrenRequiringUpdateUniformTraversal==num) return;
//
//	// note, if _updateCallback is set then the
//	// parents won't be affected by any changes to
//	// _numChildrenRequiringUpdateTraversal so no need to inform them.
//	if (!m_updateCallback && !m_parents.empty())
//	{
//		// need to pass on changes to parents.        
//		int delta = 0;
//		if (m_numChildrenRequiringUpdateUniformTraversal>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentList::iterator itr =m_parents.begin();
//				itr != m_parents.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringUpdateUniformTraversal( (*itr)->getNumChildrenRequiringUpdateUniformTraversal()+delta );
//			}
//		}
//	}
//
//	// finally update this objects value.
//	m_numChildrenRequiringUpdateTraversal=num;
//
//}

//void crDrawable::setNumChildrenRequiringEventTraversal(unsigned int num)
//{
//	// if no changes just return.
//	if (m_numChildrenRequiringEventTraversal==num) return;
//
//	// note, if _eventCallback is set then the
//	// parents won't be affected by any changes to
//	// _numChildrenRequiringEventTraversal so no need to inform them.
//	if (!m_eventCallback && !m_parents.empty())
//	{
//		// need to pass on changes to parents.        
//		int delta = 0;
//		if (m_numChildrenRequiringEventTraversal>0) --delta;
//		if (num>0) ++delta;
//		if (delta!=0)
//		{
//			// the number of callbacks has changed, need to pass this
//			// on to parents so they know whether app traversal is
//			// reqired on this subgraph.
//			for(ParentList::iterator itr =m_parents.begin();
//				itr != m_parents.end();
//				++itr)
//			{    
//				(*itr)->setNumChildrenRequiringEventTraversal( (*itr)->getNumChildrenRequiringEventTraversal()+delta );
//			}
//
//		}
//	}
//
//	// finally Event this objects value.
//	m_numChildrenRequiringEventTraversal=num;
//}
//
crStateSet* crDrawable::getOrCreateStateSet()
{
	if (!m_stateset) m_stateset = new crStateSet;
	return m_stateset.get();
}

void crDrawable::setSupportsDisplayList(bool flag)
{
	// if value unchanged simply return.
	if (m_supportsDisplayList==flag) return;

	// if previously set to true then need to check about display lists.
	if (m_supportsDisplayList)
	{
		if (m_useDisplayList)
		{
			// used to support display lists and display lists switched
			// on so now delete them and turn useDisplayList off.
			dirtyDisplayList();
			m_useDisplayList = false;
		}
	}

	// set with new value.
	m_supportsDisplayList=flag;
}

void crDrawable::setUseDisplayList(bool flag)
{
	return;
	// if value unchanged simply return.
	if (m_useDisplayList==flag) return;

	// if was previously set to true, remove display list.
	if (m_useDisplayList)
	{
		dirtyDisplayList();
	}

	if (m_supportsDisplayList)
	{

		// set with new value.
		m_useDisplayList = flag;

	}
	else // does not support display lists.
	{
		if (flag)
		{
			notify(WARN)<<"Warning: attempt to setUseDisplayList(true) on a drawable with does not support display lists."<<std::endl;
		}
		else 
		{
			// set with new value.
			m_useDisplayList = false;
		}
	}
}


void crDrawable::setUseVertexBufferObjects(bool flag)
{
	// if value unchanged simply return.
	if (m_useVertexBufferObjects==flag) return;

	// if was previously set to true, remove display list.
	if (m_useVertexBufferObjects)
	{
		dirtyDisplayList();
	}

	m_useVertexBufferObjects = flag;

}

void crDrawable::setUseIndexBufferObjects(bool flag)
{
	// if value unchanged simply return.
	if (m_useIndexBufferObjects==flag) return;

	// if was previously set to true, remove display list.
	if (m_useIndexBufferObjects)
	{
		dirtyDisplayList();
	}

	m_useIndexBufferObjects = flag;

}


void crDrawable::dirtyDisplayList()
{
	unsigned int i;
	for(i=0;i<m_globjList.size();++i)
	{
		if (m_globjList[i] != 0)
		{
			crDrawable::deleteDisplayList(i,m_globjList[i],getObjectSizeHint());
			m_globjList[i] = 0;
		}
	}

	for(i=0;i<m_vboList.size();++i)
	{
		if (m_vboList[i] != 0) 
		{
			crDrawable::deleteVertexBufferObject(i,m_vboList[i]);
			m_vboList[i] = 0;
		}
	}
}

void crDrawable::dirtyBound()
{
	if (m_bbox_computed)
	{
		m_bbox_computed = false;

		// dirty parent bounding sphere's to ensure that all are valid.
		for(ParentList::iterator itr=m_parents.begin();
			itr!=m_parents.end();
			++itr)
		{
			(*itr)->dirtyBound();
		}
	}
}

void crDrawable::setUpdateCallback(UpdateCallback* ac)
{
	if (m_updateCallback==ac) return;

	int delta = 0;
	if (m_updateCallback.valid()) --delta;
	if (ac) ++delta;

	m_updateCallback = ac;

	if (delta!=0 && !(m_stateset.valid() && m_stateset->requiresUpdateTraversal()))
	{
		for(ParentList::iterator itr=m_parents.begin();
			itr!=m_parents.end();
			++itr)
		{
			(*itr)->setNumChildrenRequiringUpdateTraversal((*itr)->getNumChildrenRequiringUpdateTraversal()+delta);
		}
	}
}

//void crDrawable::setUpdateUniformCallback(UpdateCallback* ac)
//{
//	if (m_updateUniformCallback==ac) return;
//
//	int delta = 0;
//	if (m_updateUniformCallback.valid()) --delta;
//	if (ac) ++delta;
//
//	m_updateUniformCallback = ac;
//
//	if (delta!=0)
//	{
//		for(ParentList::iterator itr=m_parents.begin();
//			itr!=m_parents.end();
//			++itr)
//		{
//			(*itr)->setNumChildrenRequiringUpdateUniformTraversal((*itr)->getNumChildrenRequiringUpdateUniformTraversal()+delta);
//		}
//	}
//}
//
//void crDrawable::setEventCallback(EventCallback* ac)
//{
//	if (m_eventCallback==ac) return;
//
//	int delta = 0;
//	if (m_eventCallback.valid()) --delta;
//	if (ac) ++delta;
//
//	m_eventCallback = ac;
//
//	if (delta!=0 && !(m_stateset.valid() && m_stateset->requiresEventTraversal()))
//	{
//		for(ParentList::iterator itr=m_parents.begin();
//			itr!=m_parents.end();
//			++itr)
//		{
//			(*itr)->setNumChildrenRequiringEventTraversal( (*itr)->getNumChildrenRequiringEventTraversal()+delta );
//		}
//	}
//}
//
#include <Driver/GLDrv/crDrawableDrv.h>