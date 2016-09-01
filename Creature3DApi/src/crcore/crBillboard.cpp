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
#include <stdio.h>
#include <math.h>

#include <CRCore/crBillboard.h>
#include <CRCore/crArray.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crGroup.h>
#include <CRCore\crShapeDrawable.h>

using namespace CRCore;
crBillboard::NewBBVec crBillboard::m_newbbvec;

#define square(x)   ((x)*(x))

crBillboard::crBillboard()
{
    m_mode = AXIAL_ROT;
    m_axis.set(0.0f,0.0f,1.0f);
    m_normal.set(0.0f,-1.0f,0.0f);
    updateCache();
	m_composeGeo = new crGeometry;
	m_applyMatrix = new ApplyMatrix;
	m_init = false;
    m_enableIntersect = false;
}

crBillboard::crBillboard(const crBillboard& billboard,const crCopyOp& copyop):
        crObject(billboard,copyop),
        m_mode(billboard.m_mode),
        m_axis(billboard.m_axis),
        m_normal(billboard.m_normal),
		m_indexOffsetList(billboard.m_indexOffsetList),
        m_positionList(billboard.m_positionList),
        m_cachedMode(billboard.m_cachedMode),
        m_side(billboard.m_side),
		m_init(false)
{
	//if(m_init)
	//{
	//	m_composeGeo = dynamic_cast<crGeometry *>(billboard.m_composeGeo->clone(crCopyOp::DEEP_COPY_PRIMITIVES|crCopyOp::DEEP_COPY_BASES));
	//	m_vertices = dynamic_cast<Vec3Array *>(m_composeGeo->getVertexArray());
	//	m_vertices_buf = new Vec3Array;
	//	m_vertices_buf->resize(m_vertices->size());
	//	m_applyMatrix = new ApplyMatrix;
	//	m_applyMatrix->m_outvertices = m_vertices_buf.get();
	//	m_composeGeo->getBound();
	//	dirtyBound();
 //       m_composeGeo->addParent(this);
	//}
	//else
	//{
		m_composeGeo = new crGeometry;
		m_applyMatrix = new ApplyMatrix;
		//init();
	//}
}

crBillboard::~crBillboard()
{
}

void crBillboard::setMode(Mode mode)
{
    m_mode = mode;
    m_cachedMode = CACHE_DIRTY;
    updateCache();
}

void crBillboard::setAxis(const crVector3& axis)
{
    m_axis = axis;
    m_axis.normalize();
    updateCache();
}

void crBillboard::setNormal(const crVector3& normal)
{
    m_normal = normal;
    m_normal.normalize();
    updateCache();
}

void crBillboard::updateCache()
{
    if (m_mode==AXIAL_ROT)
    {
        if      (m_axis==crVector3(1.0f,0.0,0.0f) && m_normal==crVector3(0.0f,-1.0,0.0f)) m_cachedMode = AXIAL_ROT_X_AXIS;
        else if (m_axis==crVector3(0.0f,1.0,0.0f) && m_normal==crVector3(1.0f, 0.0,0.0f)) m_cachedMode = AXIAL_ROT_Y_AXIS;
        else if (m_axis==crVector3(0.0f,0.0,1.0f) && m_normal==crVector3(0.0f,-1.0,0.0f)) m_cachedMode = AXIAL_ROT_Z_AXIS;
        else                                                                    m_cachedMode = AXIAL_ROT;
    }
	else if( m_mode == POINT_ROT_WORLD )
	{
		if(m_axis==crVector3(0.0f, 0.0, 1.0f) && m_normal==crVector3(0.0f, -1.0f, 0.0f))  m_cachedMode = POINT_ROT_WORLD_Z_AXIS;
		else m_cachedMode = m_mode;

	}
    else m_cachedMode = m_mode;
    
    m_side = m_axis^m_normal;
    m_side.normalize();   
}

bool crBillboard::expand(const crBillboard &billborad)
{   
	if(m_mode!=billborad.m_mode) return false;

	m_positionList.insert(m_positionList.end(),billborad.m_positionList.begin(),billborad.m_positionList.end());
	for(int i=0; i<billborad.getNumDrawables(); i++)
	{
		crObject::addDrawable(const_cast<crDrawable *>(billborad.getDrawable(i)));
	}

	dirtyBound();
	m_init = false;
	return true;
}

crBillboard *crBillboard::getOrCreateBillboard(crStateSet *ss)
{
	for( NewBBVec::iterator itr = m_newbbvec.begin();
		 itr != m_newbbvec.end();
		 ++itr )
	{
        if(/*(*itr)->getStateSet() == ss*/(*itr)->getStateSet()->compare(*ss,true)==0)
		    return itr->get();
	}

    crBillboard *new_bb = new crBillboard;
	new_bb->setStateSet(ss);
    m_newbbvec.push_back(new_bb);
	return new_bb;
}

bool crBillboard::addBBDrawable(crDrawable *gset)
{
	crGeometry *geo = gset->asGeometry();
	if(!geo)
	{
		CRCore::notify(WARN)<<"crBillboard::addDrawable<Warning>:Billboard必须为Geometry组成"<< std::endl;
	}

	if(/*m_stateset!=geo->getStateSet()*/m_stateset->compare(*geo->getStateSet(),true)!=0)
	{
        crBillboard *new_bb = getOrCreateBillboard(geo->getStateSet());
		for(ParentArray::iterator itr = m_parentArray.begin();
			itr != m_parentArray.end();
			++itr)
		{
			(*itr)->addChild(new_bb);
		}
		new_bb->addBBDrawable(geo);
		return true;
	}
    
	if (crObject::addDrawable(gset))
	{
		crVector3 pos(geo->getBoundBox().center());
		//if(m_mode==AXIAL_ROT)
		//    pos.z() = geo->getBoundBox().zMin(); 
		while (m_positionList.size()<m_drawables.size())
		{
			m_positionList.push_back(pos);
		}

		Vec3Array *rhs_vertex = (Vec3Array *)geo->getVertexArray();
		for(Vec3Array::iterator itr = rhs_vertex->begin();
			itr != rhs_vertex->end();
			++itr)
		{
			(*itr)-=pos;
		}

		dirtyBound();
		m_init = false;

		return true;
	}
	return false;
}


bool crBillboard::addBBDrawable(crDrawable *gset,const crVector3& pos)
{
	crGeometry *geo = gset->asGeometry();
	if(!geo)
	{
		CRCore::notify(WARN)<<"crBillboard::addDrawable<Warning>:Billboard必须为Geometry组成"<< std::endl;
	}

	if(/*m_stateset!=geo->getStateSet()*/(m_stateset->compare(*geo->getStateSet(),true)!=0))
	{
		crBillboard *new_bb = getOrCreateBillboard(geo->getStateSet());
		for(ParentArray::iterator itr = m_parentArray.begin();
			itr != m_parentArray.end();
			++itr)
		{
			(*itr)->addChild(new_bb);
		}
		new_bb->addBBDrawable(geo);
		return true;
	}

    if (crObject::addDrawable(gset))
    {
        while (m_positionList.size()<m_drawables.size())
        {
            m_positionList.push_back(pos);
        }

		Vec3Array *rhs_vertex = (Vec3Array *)geo->getVertexArray();
		for(Vec3Array::iterator itr = rhs_vertex->begin();
			itr != rhs_vertex->end();
			++itr)
		{
			(*itr)-=pos;
		}

		dirtyBound();
		m_init = false;

		return true;
	}
	return false;
}


bool crBillboard::removeDrawable( crDrawable *gset )
{
    PositionList::iterator pitr = m_positionList.begin();
    for (DrawableList::iterator itr=m_drawables.begin();
        itr!=m_drawables.end();
        ++itr,++pitr)
    {
        if (itr->get()==gset)
        {
			if ((*itr)->requiresUpdateTraversal()/*getUpdateCallback()*/)
				setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
			//if ((*itr)->requiresUpdateUniformTraversal()/*getUpdateUniformCallback()*/)
			//	setNumChildrenRequiringUpdateUniformTraversal(getNumChildrenRequiringUpdateUniformTraversal()-1);
			//if ((*itr)->requiresEventTraversal()/*getEventCallback()*/)
			//	setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);

			(*itr)->removeParent(this);

            m_drawables.erase(itr);
            m_positionList.erase(pitr);
            m_bBoundSphere_computed = false;

			dirtyBound();
			m_init = false;

            return true;
        }
    }
    return false;
}

bool crBillboard::computeMatrix(crMatrix& modelview, const crVector3& eye_local, int i)
{
	//crVector3 up_local(matrix(0,1),matrix(1,1),matrix(2,1));
    crMatrix matrix;
    const crVector3& pos_local = m_positionList[i];
	//CRCore::notify(CRCore::FATAL)<<"crBillboard::computeMatrix(): "<<pos_local<<std::endl;
    crVector3 ev(eye_local-pos_local);
	crGeometry *geometry = m_drawables[i]->asGeometry();
	//if(m_cachedMode != POINT_ROT_WORLD_Z_AXIS)
	//{
	//	if(!geometry->getNormalArray())
	//	{
	//		CRCore::notify(CRCore::FATAL)<<"crBillboard::computeMatrix(): 该Billboard未计算法线"<<std::endl;
	//		return false;
	//	}
	//	const crVector3 &normal = (*(geometry->getNormalArray()))[0];

	//	if(m_normal != normal )
	//		setNormal(normal);
	//}

    switch(m_cachedMode)
    {
        case(AXIAL_ROT_Z_AXIS):
        {

            ev.z() = 0.0f;
            float ev_length = ev.length();
            if (ev_length>0.0f)
            {
    //            float rotation_z = atan2f(ev.x(),ev.y());
    //            //mat.makeRotate(inRadians(rotation_z),0.0f,0.0f,1.0f);
				//matrix.makeRotate(inRadians(rotation_z),0.0f,0.0f,1.0f);
                float inv = 1.0f/ev_length;
                float s = ev.x()*inv;
                float c = -ev.y()*inv;
                matrix(0,0) = c;
                matrix(1,0) = -s;
                matrix(0,1) = s;
                matrix(1,1) = c;
            }
            break;
        }
        case(AXIAL_ROT_Y_AXIS):
        {
            ev.y() = 0.0f;
            float ev_length = ev.length();
            if (ev_length>0.0f)
            {
                //float rotation_zrotation_z = atan2f(ev.x(),ev.y());
                //mat.makeRotate(inRadians(rotation_z),0.0f,0.0f,1.0f);
                float inv = 1.0f/ev_length;
                float s = -ev.z()*inv;
                float c = ev.x()*inv;
                matrix(0,0) = c;
                matrix(2,0) = s;
                matrix(0,2) = -s;
                matrix(2,2) = c;
            }
            break;
        }
        case(AXIAL_ROT_X_AXIS):
        {
            ev.x() = 0.0f;
            float ev_length = ev.length();
            if (ev_length>0.0f)
            {

                //float rotation_zrotation_z = atan2f(ev.x(),ev.y());
                //mat.makeRotate(inRadians(rotation_z),0.0f,0.0f,1.0f);
                float inv = 1.0f/ev_length;
                float s = -ev.z()*inv;
                float c = -ev.y()*inv;
                matrix(1,1) = c;
                matrix(2,1) = -s;
                matrix(1,2) = s;
                matrix(2,2) = c;
            }
            break;
        }
        case(AXIAL_ROT): // need to implement 
        {
            float ev_side = ev*m_side;
            float ev_normal = ev*m_normal;
            float rotation = atan2f(ev_side,ev_normal);
            matrix.makeRotate(rotation,m_axis);
            break;
        }
        case(POINT_ROT_WORLD):
        case(POINT_ROT_EYE):
        {
            // currently treat both POINT_ROT_WORLD and POINT_ROT_EYE the same,
            // this is only a temporary and 'incorrect' implementation, will
            // implement fully on second pass of crBillboard.
            // Will also need up vector to calc POINT_ROT_EYE, so this will
            // have to be added to the above method paramters.
            // Robert Osfield, Jan 2001.

            float ev_len = ev.length();
            if (ev_len != 0.0f)
            {
                ev /= ev_len;

                crVector3 cp(ev^m_normal);
                float dot = ev*m_normal;

                float cp_len = cp.length();
                if (cp_len != 0.0f)
                {
                    cp /= cp_len;

                    float rotation_cp = acosf(dot);
                    matrix.makeRotate(-inRadians(rotation_cp),cp[0],cp[1],cp[2]);
                }
            }
            break;
        }
		case(POINT_ROT_WORLD_Z_AXIS):
		{
			// float rotation_about_z = atan2( -ev.y(), ev.x() );
			// float xy_distance = sqrt( ev.x()*ev.x() + ev.y()*ev.y() );
			// float rotation_from_xy = atan2( xy_distance, -ev.z() );

			crVector2   about_z( -ev.y(), ev.x() );
			if( about_z.length() == 0.0f ) about_z.x() = 1.0f;
			about_z.normalize();
			float  xy_distance = sqrt( ev.x()*ev.x() + ev.y()*ev.y() );
			crVector2   from_xy( xy_distance, -ev.z() );
			if( from_xy.length() == 0.0f ) from_xy.x() = 1.0f;
			from_xy.normalize();

			matrix(0,0) =  about_z.x();
			matrix(0,1) =  about_z.y();
			matrix(1,0) = -about_z.y()*from_xy.x();
			matrix(1,1) =  about_z.x()*from_xy.x();
			matrix(1,2) =  from_xy.y();
			matrix(2,0) =  about_z.y()*from_xy.y();
			matrix(2,1) = -about_z.x()*from_xy.y();
			matrix(2,2) =  from_xy.x();

			break;
		}
    }

    matrix.setTrans(pos_local);

    modelview.preMult(matrix);

	m_applyMatrix->m_mat = modelview;
	m_applyMatrix->m_offset = m_indexOffsetList[i];
	geometry->accept(*m_applyMatrix);
    return true;
}

void crBillboard::swapBuffers(int frameNumber)
{
	if(m_swapFrameNumber != frameNumber)
	{
		Vec3Array *temp = m_vertices.get();
		m_vertices = m_vertices_buf.get();
		m_vertices_buf = temp;
    
		m_composeGeo->setVertexArray(m_vertices.get());
		m_applyMatrix->m_outvertices = m_vertices_buf.get();
		//float radius = m_boundSphere.radius();
	 //   m_composeGeo->setBoundBox(m_boundSphere.center(),crVector3(radius,radius,radius));
		// m_vertices->swap(*m_vertices_buf);
		//m_vertices->resize(m_vertices_buf->size()); 
		//std::copy(m_vertices_buf->begin(),m_vertices_buf->end(),m_vertices->begin()/*std::back_inserter(m_indexArray)*/);

		crNode::swapBuffers(frameNumber);
		m_swapFrameNumber = frameNumber;
	}
}
bool crBillboard::computeBound() const
{
	//int i;
	//int ngsets = m_drawables.size();

	//if( ngsets == 0 ) return false;

	m_boundSphere.init();
	m_bbox.init();
	m_boundSphere.expandBy(m_composeGeo->getBoundBox());

	if (m_boundSphere.valid())
	{
		m_bbox.expandBy(m_boundSphere);
		m_bBoundSphere_computed=true;
		return true;
	}
	else
	{
		m_bBoundSphere_computed=true;
		return false;
	}
	//crBoundingSphere bs;
	//float radius = 1.0f;
	//for( PositionList::const_iterator itr = m_positionList.begin();
	//	 itr != m_positionList.end();
	//	 ++itr)
	//{
	//	//const crDrawable *gset = m_drawables[i].get();		
	//	bs.set(*itr,radius);
	//	m_bbox.expandBy(bs);
	//	m_boundSphere.expandBy(bs);
	//}

	//m_bBoundSphere_computed=true;
	//return true;
}
void crBillboard::init()
{
	if(	m_init ) return;

	crGeometry* geo; 
	int i = 0;
	m_indexOffsetList.resize(m_drawables.size());
	m_vertices = new Vec3Array;
	m_vertices_buf = new Vec3Array;
	m_composeGeo->setVertexArray(m_vertices.get());
	//if(m_drawables.size()>1)
	//	CRCore::notify(CRCore::FATAL)<<"crBillboard::init() m_drawables size = "<<m_drawables.size()<<std::endl;

	for( DrawableList::iterator itr = m_drawables.begin();
		 itr != m_drawables.end();
		 ++itr,++i )
	{
		geo = (crGeometry*)(itr->get());
		m_composeGeo->expand(*geo,m_indexOffsetList[i]);
	}
    m_vertices_buf->resize(m_vertices->size());
    //m_vertices_buf = m_vertices;
	m_composeGeo->setSupportsDisplayList(false);
	m_composeGeo->setUseVertexBufferObjects(false);
	m_composeGeo->setUseIndexBufferObjects(m_vertices->size()>64);
	m_applyMatrix->m_outvertices = m_vertices_buf.get();
	m_composeGeo->getBound();
	//m_composeGeo->addParent(this);
	dirtyBound();
	m_init = true;
    
	if(m_newbbvec.size()>0)
		m_newbbvec.clear();
}

void crBillboard::releaseObjects(crState* state)
{
	crObject::releaseObjects(state);
	m_composeGeo->releaseObjects(state);
}
