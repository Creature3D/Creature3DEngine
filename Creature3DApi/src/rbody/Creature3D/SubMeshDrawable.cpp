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

#include <rbody/Creature3D/SubMeshDrawable.h>
#include <CRCore/crNotify.h>
//#include <CRCore/Statistics>
#include <CRCore/crGeometry.h>
#include <CRCore/crPrimitive.h>
//#include <CRUtil/crTangentSpaceGenerator.h>
#include <vector>
#include <string>
#include <rbody/Creature3D/CreBody.h>
using namespace rbody;
using namespace CRCore;

SubMeshDrawable::SubMeshDrawable(CreBodyNode *body, CRCore::crGeometry *geometry, 
				 int mesh_id, int submesh_id/*, bool enableTangentSpace*/): 
  m_body(body), m_geometry(geometry), m_mesh_id(mesh_id), 
  m_submesh_id(submesh_id),
  m_started(0)/*, m_dirty_meshes(true),
  m_lastUpdateFrameNumber(-1),
  m_enableTangentSpace(enableTangentSpace), m_noNeedTangentSpace(false)*/
{
	m_type = PUPPETGEOMETRY;

  // Debug
  //assert(m_geometry.valid());
  //assert(m_body);

  CalRenderer *cal_renderer = m_body->getBody()->getCalModel()->getRenderer();
  //assert(cal_renderer->selectMeshSubmesh(m_mesh_id, m_submesh_id));
  
  m_geometry_buf = new CRCore::crGeometry;
  int vertex_count = 0;//cal_renderer->getVertexCount();
  
  // Allocate memory for vertices, normals
  m_geometry->setVertexArray(new CRCore::Vec3Array(vertex_count));
  m_geometry_buf->setVertexArray(new CRCore::Vec3Array(vertex_count));

  m_geometry->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
  m_geometry_buf->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
  m_geometry->setNormalArray(new CRCore::Vec3Array(vertex_count));
  m_geometry_buf->setNormalArray(new CRCore::Vec3Array(vertex_count));
  
  m_geometry->setTexCoordArray(0, new CRCore::Vec2Array(vertex_count));
  m_geometry_buf->setTexCoordArray(0, new CRCore::Vec2Array(vertex_count));
  // Loop through all texture layers and allocate memory for texture coords
 // for (int map_id = 0; map_id < cal_renderer->getMapCount(); map_id++) 
 // {
	//  if(map_id==1) continue;

 //   if (!m_geometry->getTexCoordArray(map_id)) 
	//{
 //     m_geometry->setTexCoordArray(map_id, new CRCore::Vec2Array(vertex_count));
 //     cal_renderer->getTextureCoordinates(map_id, 
	//				  (float *) m_geometry->getTexCoordArray(map_id)->getDataPointer());
 //   }
 // }
  //if (!m_geometry->getTexCoordArray(0)) 
  //{
	 // m_geometry->setTexCoordArray(0, new CRCore::Vec2Array(vertex_count));
	 // cal_renderer->getTextureCoordinates(0, 
		//  (float *) m_geometry->getTexCoordArray(0)->getDataPointer());
  //}

    // Construct primitive set (will store the faces)
    if(sizeof(CalIndex)==2)
	{
		CRCore::DrawElementsUShort *prim_set = new CRCore::DrawElementsUShort();
	    prim_set->setMode(CRCore::crPrimitive::PT_TRIANGLES);
	    m_geometry->addPrimitive(prim_set);

		prim_set = new CRCore::DrawElementsUShort();
		prim_set->setMode(CRCore::crPrimitive::PT_TRIANGLES);
		m_geometry_buf->addPrimitive(prim_set);
	}
	else
	{
		CRCore::DrawElementsUInt *prim_set = new CRCore::DrawElementsUInt();
		prim_set->setMode(CRCore::crPrimitive::PT_TRIANGLES);
		m_geometry->addPrimitive(prim_set);

		prim_set = new CRCore::DrawElementsUInt();
		prim_set->setMode(CRCore::crPrimitive::PT_TRIANGLES);
		m_geometry_buf->addPrimitive(prim_set);
	}

  // Set cal renderer to skip re-normalization of normals, let opengl handle this instead
  cal_renderer->setNormalization(m_body->getEnableTangentSpace()/*m_enableTangentSpace*/);

  if(m_body->getEnableTangentSpace())
  {
	  m_geometry->setVertexAttribData(ATTRIBUTE_TANGENT, CRCore::crGeometry::ArrayData(new CRCore::Vec3Array(vertex_count), CRCore::crGeometry::BIND_PER_VERTEX, false));
	  //m_geometry->setVertexAttribData(ATTRIBUTE_BINORMAL, CRCore::crGeometry::ArrayData(new CRCore::Vec3Array(vertex_count), CRCore::crGeometry::BIND_PER_VERTEX, false));
	  m_geometry_buf->setVertexAttribData(ATTRIBUTE_TANGENT, CRCore::crGeometry::ArrayData(new CRCore::Vec3Array(vertex_count), CRCore::crGeometry::BIND_PER_VERTEX, false));
	  //m_geometry_buf->setVertexAttribData(ATTRIBUTE_BINORMAL, CRCore::crGeometry::ArrayData(new CRCore::Vec3Array(vertex_count), CRCore::crGeometry::BIND_PER_VERTEX, false));

	  m_tangentSpacesArray = new CRCore::Vec4Array(vertex_count);
  }

  // Update vertices from animation and recalculate bbox
  //update();
  //swapBuffers();
  //computeBound();
  //m_geometry->dirtyBound();

  setSupportsDisplayList(false);
  m_geometry->setSupportsDisplayList(false);
  m_geometry->setUseVertexBufferObjects(false);
  m_geometry->setUseIndexBufferObjects(false);
  m_geometry_buf->setSupportsDisplayList(false);
  m_geometry_buf->setUseVertexBufferObjects(false);
  m_geometry_buf->setUseIndexBufferObjects(false);
}

SubMeshDrawable::~SubMeshDrawable()
{
}

CreBodyNode *SubMeshDrawable::getBodyNode()
{
    return m_body;
}

bool SubMeshDrawable::computeBound() const 
{
  
  // This could be done if each sub mesh drawable has its own geometry (no sharing)
  m_bbox = m_geometry_buf->getBound();
  
  //// Instead get bounding box from bones (fast)
  ////CRCore::crVector3 min, max;
  //m_body->getBody()->getCalModel()->getSkeleton()->getBoneBoundingBox(m_bbox.m_min.ptr(), m_bbox.m_max.ptr());

  ////m_bbox.set(min,max);
  ////CRCore::notify(CRCore::WARN)<<"SubMeshDrawable::computeBound(),xlength:"<<m_bbox.xLength()<<"ylength:"<<m_bbox.yLength()<<"zlength:"<<m_bbox.zLength()<< std::endl;
  ////CRCore::crBoundingBox bbox(min, max);
  
  if (!m_bbox.valid())
  {
	  return false;
      //CRCore::notify(CRCore::WARN) << "******* CreBodyNode::computeBound() problem" << std::endl;
  }
  
  m_bbox_computed = true;

  return true;

  //return bbox;

}


void SubMeshDrawable::update(/*int fn*/)
{
	//if(fn==m_lastUpdateFrameNumber) return;
	//
	//m_lastUpdateFrameNumber = fn;
	m_body->setInternalUpdateMode(m_body->getUpdateMode());
	
  // Get the cal3d renderer of the model
  //m_body->getBody()->setLodPoly(m_body->getLodLevel());
  CalRenderer *cal_renderer = m_body->getBody()->getCalModel()->getRenderer();

  // Select mesh and submesh for further data access
  if(cal_renderer->selectMeshSubmesh(m_mesh_id, m_submesh_id))
  {
	// Get vertex count
	int vertex_count = cal_renderer->getVertexCount();

	CRCore::Vec3Array *vtxArray,*normalArray;
    vtxArray = static_cast<CRCore::Vec3Array *> (m_geometry->getVertexArray());
	vtxArray->resize(vertex_count);
	cal_renderer->getVertices((float *) vtxArray->getDataPointer());

	normalArray = static_cast<CRCore::Vec3Array *> (m_geometry->getNormalArray());
	normalArray->resize(vertex_count);
	cal_renderer->getNormals((float *) normalArray->getDataPointer());

	if(m_body->getNeedTangentSpace())
	{
		CRCore::Vec3Array* tangentArray;
		//CRCore::Vec3Array* binormalArray;
		tangentArray = static_cast<CRCore::Vec3Array *> (m_geometry->getVertexAttribArray(ATTRIBUTE_TANGENT));
		tangentArray->resize(vertex_count);
		//binormalArray = static_cast<CRCore::Vec3Array *> (m_geometry->getVertexAttribArray(ATTRIBUTE_BINORMAL));
		//binormalArray->resize(vertex_count);

		m_tangentSpacesArray->resize(vertex_count);
		cal_renderer->getTangentSpaces(TEXTURE_BASEMAP/*TEXTURE_BUMPMAP*/,(float *) m_tangentSpacesArray->getDataPointer());
		for( int i = 0; i < vertex_count; i++ )
		{
			(*tangentArray)[i].set((*m_tangentSpacesArray)[i][0], (*m_tangentSpacesArray)[i][1], (*m_tangentSpacesArray)[i][2]);
			//(*binormalArray)[i] = ((*normalArray)[i] ^ (*tangentArray)[i]) * (*m_tangentSpacesArray)[i][3];
		}
	}
	// Get the transformed vertices and normals of the submesh
	//cal_renderer->getVertices((float *) m_geometry->getVertexArray()->getDataPointer());
	//cal_renderer->getNormals((float *) m_geometry->getNormalArray()->getDataPointer());
	
	// Get the texture coordinate buffer for all maps (multi texturing support)
	// Only necessary if texture coords change during run time
	//for (int map_id = 0; map_id < cal_renderer->getMapCount(); map_id++) 
	//{    
	//	if(map_id==1) continue;
	//	//if (m_geometry->getTexCoordArray(map_id)->getNumElements() < vertex_count) 
	//	{
	//	    CRCore::Vec2Array *tex_array = dynamic_cast<CRCore::Vec2Array *> (m_geometry->getTexCoordArray(map_id));
	//	    assert(tex_array);
	//	    tex_array->resize(vertex_count);
	//	}
	//    
	//	cal_renderer->getTextureCoordinates(map_id, (float *) m_geometry->getTexCoordArray(map_id)->getDataPointer());
	//}
	CRCore::Vec2Array *tex_array = dynamic_cast<CRCore::Vec2Array *> (m_geometry->getTexCoordArray(0));
	tex_array->resize(vertex_count);
	cal_renderer->getTextureCoordinates(0, (float *) tex_array->getDataPointer());

	if(sizeof(CalIndex)==2)
	{
		CRCore::DrawElementsUShort *prim_set = dynamic_cast<CRCore::DrawElementsUShort *>(m_geometry->getPrimitive(0));
		CRCore::DrawElementsUShort::PT_IndexArray &indexArray = prim_set->getIndexArray();
		indexArray.resize(cal_renderer->getFaceCount() * 3);
		cal_renderer->getFaces( (CalIndex*)(&indexArray.front()) );
	}
	else
	{
		CRCore::DrawElementsUInt *prim_set = dynamic_cast<CRCore::DrawElementsUInt *>(m_geometry->getPrimitive(0));
		CRCore::DrawElementsUInt::PT_IndexArray &indexArray = prim_set->getIndexArray();
		indexArray.resize(cal_renderer->getFaceCount() * 3);
		cal_renderer->getFaces( (CalIndex*)(&indexArray.front()) );
	}

	//m_geometry->dirtyBound();
	//computeBound();
	//for(ParentList::iterator itr=m_parents.begin();
	//	itr!=m_parents.end();
	//	++itr)
	//{
	//	(*itr)->dirtyBound();
	//}
	if(m_started==0) m_started = 1;
  }
  m_geometry->setColor(getColor());
  //m_dirty_meshes = true;
	// Get the number of faces and resize array
	// Only necessary if faces change during run time (for example LOD)
	//int nr = cal_renderer->getFaceCount();
	//nr *= 3;
	//  
	//if (m_draw_elem->size() != nr) 
	//{
	//	m_draw_elem->resize(nr);
		//CRCore::DrawElementsUShort *prim_set = dynamic_cast<CRCore::DrawElementsUShort *>(m_geometry->getPrimitive(0));
		//if(prim_set)
		//{
		//	CRCore::DrawElementsUShort::PT_IndexArray &indexArray = prim_set->getIndexArray();
		//	indexArray.resize(cal_renderer->getFaceCount() * 3);
		//	cal_renderer->getFaces( (CalIndex*)(&indexArray.front()) );
		//}
	//}
	  
	// Meshes updated, they are not dirty anymore
	//m_dirty_meshes = false; 

  //m_geometry->dirtyBound();

  // Temporary solution, shouldn't call explicity in a perfect world
  // but dirtybound doesn't seem to work right now (no idea why)
  // computeBound();
}

void SubMeshDrawable::draw(CRCore::crState& state) const
{
	drawImplementation(state);
}

void SubMeshDrawable::drawImplementation(CRCore::crState& state) const
{
    if(m_started==2)
		m_geometry_buf->draw(state);
}

void SubMeshDrawable::swapBuffers(int frameNumber)
{
	//if(m_dirty_meshes)
	//{
	if(m_swapFrameNumber != frameNumber)
	{
		//CRCore::ref_ptr<CRCore::Array> tmpArray = m_geometry->getVertexArray();
		//m_geometry->setVertexArray(m_geometry_buf->getVertexArray());
		//m_geometry_buf->setVertexArray(tmpArray.get());

		//tmpArray = m_geometry->getNormalArray();
		//m_geometry->setNormalArray(m_geometry_buf->getNormalArray());
		//m_geometry_buf->setNormalArray(static_cast<CRCore::Vec3Array *>(tmpArray.get()));

		//tmpArray = m_geometry->getTexCoordArray(0);
		//m_geometry->setTexCoordArray(0,m_geometry_buf->getTexCoordArray(0));
		//m_geometry_buf->setTexCoordArray(0,tmpArray.get());

		//if(m_body->getNeedTangentSpace())
		//{
		//	tmpArray = m_geometry->getVertexAttribArray(ATTRIBUTE_TANGENT);
		//	m_geometry->setVertexAttribArray(ATTRIBUTE_TANGENT,m_geometry_buf->getVertexAttribArray(ATTRIBUTE_TANGENT));
		//	m_geometry_buf->setVertexAttribArray(ATTRIBUTE_TANGENT,tmpArray.get());

		//	//tmpArray = m_geometry->getVertexAttribArray(ATTRIBUTE_BINORMAL);
		//	//m_geometry->setVertexAttribArray(ATTRIBUTE_BINORMAL,m_geometry_buf->getVertexAttribArray(ATTRIBUTE_BINORMAL));
		//	//m_geometry_buf->setVertexAttribArray(ATTRIBUTE_BINORMAL,tmpArray.get());
		//}
		//CRCore::ref_ptr<crPrimitive> tmpPrim = m_geometry->getPrimitive(0);
		//m_geometry->setPrimitive(0,m_geometry_buf->getPrimitive(0));
		//m_geometry_buf->setPrimitive(0,tmpPrim.get());
		////if(sizeof(CalIndex)==2)
		////{
		////	CRCore::DrawElementsUShort *prim_set_buf = dynamic_cast<CRCore::DrawElementsUShort *>(m_geometry_buf->getPrimitive(0));
		////	CRCore::DrawElementsUShort *prim_set = dynamic_cast<CRCore::DrawElementsUShort *>(m_geometry->getPrimitive(0));
		////	prim_set_buf->setIndexArray(prim_set->getIndexArray());
		////}
		////else
		////{
		////	CRCore::DrawElementsUInt *prim_set_buf = dynamic_cast<CRCore::DrawElementsUInt *>(m_geometry_buf->getPrimitive(0));
		////	CRCore::DrawElementsUInt *prim_set = dynamic_cast<CRCore::DrawElementsUInt *>(m_geometry->getPrimitive(0));
		////	prim_set_buf->setIndexArray(prim_set->getIndexArray());
		////}
		////m_geometry_buf->setColor(getColor());

		//m_geometry->dirtyBound();
		//computeBound();
		//for(ParentList::iterator itr=m_parents.begin();
		//	itr!=m_parents.end();
		//	++itr)
		//{
		//	(*itr)->dirtyBound();
		//}

		CRCore::ref_ptr<crGeometry> tmp = m_geometry;
		m_geometry = m_geometry_buf;
		m_geometry_buf = tmp;
		crDrawable::swapBuffers(frameNumber);
		if(m_started==1)
			m_started = 2;
		//const_cast<SubMeshDrawable *>(this)->dirtyBound();
		//for(ParentList::iterator itr=m_parents.begin();
		//	itr!=m_parents.end();
		//	++itr)
		//{
		//	(*itr)->dirtyBound();
		//}
		//	m_dirty_meshes = false;
		//}
		m_swapFrameNumber = frameNumber;
	}
}

void SubMeshDrawable::accept(PrimitiveFunctor &prim) const
{
  // This could be done if each sub mesh drawable has its own geometry (no sharing)
	if(m_started==2)
		m_geometry_buf->accept(prim);
  
  // Pass bounding box for collision testing (fast)
  /*prim.begin(GL_LINE_LOOP);
  
  prim.vertex(_bbox._min[0], _bbox._min[1], _bbox._min[2]);
  prim.vertex(_bbox._min[0], _bbox._max[1], _bbox._min[2]);
  prim.vertex(_bbox._max[0], _bbox._max[1], _bbox._min[2]);
  prim.vertex(_bbox._max[0], _bbox._min[1], _bbox._min[2]);
  prim.end();
  
  prim.begin(GL_LINE_LOOP);
  prim.vertex(_bbox._min[0], _bbox._min[1], _bbox._max[2]);
  prim.vertex(_bbox._min[0], _bbox._max[1], _bbox._max[2]);
  prim.vertex(_bbox._max[0], _bbox._max[1], _bbox._max[2]);
  prim.vertex(_bbox._max[0], _bbox._min[1], _bbox._max[2]);
  prim.end();
  
  prim.begin(GL_LINES);
  prim.vertex(_bbox._min[0], _bbox._min[1], _bbox._min[2]);
  prim.vertex(_bbox._min[0], _bbox._min[1], _bbox._max[2]);
  
  prim.vertex(_bbox._min[0], _bbox._max[1], _bbox._min[2]);
  prim.vertex(_bbox._min[0], _bbox._max[1], _bbox._max[2]);
  
  prim.vertex(_bbox._max[0], _bbox._max[1], _bbox._min[2]);
  prim.vertex(_bbox._max[0], _bbox._max[1], _bbox._max[2]);
  
  prim.vertex(_bbox._max[0], _bbox._min[1], _bbox._min[2]);
  prim.vertex(_bbox._max[0], _bbox._min[1], _bbox._max[2]);
  prim.end();
  */
}

CRCore::crGeometry *SubMeshDrawable::getPuppetGeometry()
{
	return m_geometry.get();
}

CRCore::crGeometry *SubMeshDrawable::getPuppetBufGeometry()
{
    return m_geometry_buf.get();
}

void SubMeshDrawable::releaseObjects(CRCore::crState* state)
{
    m_geometry->releaseObjects(state);
	m_geometry_buf->releaseObjects(state);
	CRCore::crDrawable::releaseObjects(state);
}

//void SubMeshDrawable::setNoNeedTangentSpaceFlag(bool noNeedTangentSpace)
//{
//	m_noNeedTangentSpace = noNeedTangentSpace;
//}
