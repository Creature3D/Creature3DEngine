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
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBrain.h>
#include <CRCore/crArray.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
//#include <CRPhysics/crCollideCallback.h>
#include <CRCore/crShape.h>
#include <ode/ode.h>
#include <iterator>
using namespace CRCore;
using namespace CRPhysics;

class crSetObjectPhysicsInitVisitor : public CRCore::crNodeVisitor
{
public:
	crSetObjectPhysicsInitVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crSetObjectPhysicsInitVisitor(){}
	virtual void apply(CRCore::crObject& node){node.setPhysicsInited(true);}
};
static crSetObjectPhysicsInitVisitor s_setObjectPhysicsInit;
void CollectMeshInfoVisitor::copyIndexArray(const crGeometry *geometry, CRCore::UIntArray &outIndexArray, int offset)
{
	int i = 0;
	for(crGeometry::PrimitiveList::const_iterator pitr = geometry->getPrimitiveList().begin();
		pitr != geometry->getPrimitiveList().end();
		++pitr)
	{
		if((*pitr)->getNumIndices()<3) continue;

		unsigned int mode = (*pitr)->getMode();
		if(mode==crPrimitive::PT_TRIANGLES)
		{
			outIndexArray.reserve(outIndexArray.size()+(*pitr)->getNumIndices());
		}
		else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
		{
			outIndexArray.reserve(outIndexArray.size()+3*((*pitr)->getNumIndices()-2));
		}
		else
		{
			CRCore::notify(CRCore::NOTICE)<<"Warning: in CRPhysics::CollectMeshInfoVisitor::_create() primitive mode error"<< std::endl;
			//CRCore::notify(CRCore::NOTICE)<<"    开始输出节点Path:"<< std::endl;
			//for( const crNode *parent = geometry->getParent(0); parent; parent = parent->getParent(0) )
			//{
			//	CRCore::notify(CRCore::NOTICE)<<" parentName = "<<parent->getName()<< std::endl;
			//	if(parent->getName().compare("DB") == 0) break;
			//}
			//CRCore::notify(CRCore::NOTICE)<<"    节点Path输出结束:"<< std::endl;
			continue;
		}

		switch((*pitr)->getType())
		{
		case (CRCore::crPrimitive::DrawArraysPrimitiveType):
			{
				const CRCore::DrawArrays* drawarrays = static_cast<const CRCore::DrawArrays*>(pitr->get());
				const DrawArrays::PT_IndexArray &indexArray = drawarrays->getIndexArray();
				if(mode==crPrimitive::PT_TRIANGLES)
				{
					for( i = drawarrays->getFirst(); i < drawarrays->getFirst() + drawarrays->getCount(); ++i )
					{
                        outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
					}
				}
				else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
				{
					for( i = drawarrays->getFirst(); i < drawarrays->getFirst() + drawarrays->getCount()-2; ++i )
					{
						outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+1])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+2])+offset);
					}
				}
			}
			break;
		case (CRCore::crPrimitive::DrawArrayLengthsPrimitiveType):
			{
				//const CRCore::DrawArrayLengths* drawarraysLengths = static_cast<const CRCore::DrawArrayLengths*>(pitr->get());
				//const DrawArrayLengths::PT_IndexArray &indexArray = drawarraysLengths->getIndexArray();
				//if(mode==crPrimitive::PT_TRIANGLES)
				//{
				//	for( DrawArrayLengths::PT_IndexArray::const_iterator iitr = indexArray.begin();
				//		iitr != indexArray.end();
				//		++iitr )
				//	{
				//		outIndexArray.push_back((unsigned int)(*iitr)+offset);
				//	}
				//}
				//else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
				//{
				//	for( i = 0; i < indexArray.size()-2; ++i )
				//	{
				//		outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
				//		outIndexArray.push_back((unsigned int)(indexArray[i+1])+offset);
				//		outIndexArray.push_back((unsigned int)(indexArray[i+2])+offset);
				//	}
				//}
			}
			break;
		case (CRCore::crPrimitive::DrawElementsUBytePrimitiveType):
			{
				const CRCore::DrawElementsUByte* primitiveUByte = static_cast<const CRCore::DrawElementsUByte*>(pitr->get());
				const DrawElementsUByte::PT_IndexArray &indexArray = primitiveUByte->getIndexArray();
				if(mode==crPrimitive::PT_TRIANGLES)
				{
					for( DrawElementsUByte::PT_IndexArray::const_iterator iitr = indexArray.begin();
						iitr != indexArray.end();
						++iitr )
					{
						outIndexArray.push_back((unsigned int)(*iitr)+offset);
					}
				}
				else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
				{
					for( i = 0; i < indexArray.size()-2; ++i )
					{
						outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+1])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+2])+offset);
					}
				}
			}
			break;
		case(CRCore::crPrimitive::DrawElementsUShortPrimitiveType):
			{
				const DrawElementsUShort* primitiveUShort = static_cast<const DrawElementsUShort*>(pitr->get());
				const DrawElementsUShort::PT_IndexArray &indexArray = primitiveUShort->getIndexArray();
				if(mode==crPrimitive::PT_TRIANGLES)
				{
					for( DrawElementsUShort::PT_IndexArray::const_iterator iitr = indexArray.begin();
						iitr != indexArray.end();
						++iitr )
					{
						outIndexArray.push_back((unsigned int)(*iitr)+offset);
					}
				}
				else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
				{
					for( i = 0; i < indexArray.size()-2; ++i )
					{
						outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+1])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+2])+offset);
					}
				}
			}
			break;
		case(CRCore::crPrimitive::DrawElementsUIntPrimitiveType):
			{
				const CRCore::DrawElementsUInt* primitiveUInt = static_cast<const CRCore::DrawElementsUInt*>(pitr->get());
				const DrawElementsUInt::PT_IndexArray &indexArray = primitiveUInt->getIndexArray();
				if(mode==crPrimitive::PT_TRIANGLES)
				{
					for( DrawElementsUInt::PT_IndexArray::const_iterator iitr = indexArray.begin();
						iitr != indexArray.end();
						++iitr )
					{
						outIndexArray.push_back((unsigned int)(*iitr)+offset);
					}
				}
				else if(mode==crPrimitive::PT_TRIANGLE_STRIP)
				{
					for( i = 0; i < indexArray.size()-2; ++i )
					{
						outIndexArray.push_back((unsigned int)(indexArray[i])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+1])+offset);
						outIndexArray.push_back((unsigned int)(indexArray[i+2])+offset);
					}
				}
				//std::copy(indexArray.begin(),indexArray.end(),std::back_inserter(outIndexArray));
			}
			break;
		}
	}
}

dTriMeshDataID CollectMeshInfoVisitor::createTriMeshData( CRCore::crNode* inNode )
{
	CRCore::ref_ptr<CRCore::Vec3Array> _vertexArray = new CRCore::Vec3Array;
	CRCore::ref_ptr<CRCore::UIntArray> _indexArray = new CRCore::UIntArray;

	CollectMeshInfoVisitor cmiv;
	inNode->accept(cmiv);

	GeometryList &geometryList = cmiv.getGeometryList();
	crGeometry *geometry;
	//crPrimitive *prim;
	Vec3Array *vertexArray;

	dTriMeshDataID outTriDataID = dGeomTriMeshDataCreate();
    
	if(geometryList.size()>1)
	{
		int indexOffset = 0;
		for( GeometryList::iterator itr = geometryList.begin(); 
			itr != geometryList.end();
			++itr )	
		{
			indexOffset = _vertexArray->size();

			vertexArray = (Vec3Array*)((*itr)->getVertexArray());
            
			_vertexArray->reserve(_vertexArray->size()+vertexArray->size());
			std::copy(vertexArray->begin(),vertexArray->end(),std::back_inserter(*_vertexArray));

			copyIndexArray(*itr,*_indexArray,indexOffset);
		}
	}
	else
	{
		geometry = geometryList.front();
		vertexArray = (Vec3Array*)(geometry->getVertexArray());
		_vertexArray = vertexArray;
		
		copyIndexArray(geometry,*_indexArray);
	}

	if(_indexArray->size()>=3)
	{
		dGeomTriMeshDataBuildSingle( outTriDataID, _vertexArray->getDataPointer(),
			_vertexArray->getStride(),_vertexArray->getNumElements(),
			_indexArray->getDataPointer(), _indexArray->getNumElements(),
			_indexArray->getStride() );

		inNode->setPhysicsMeshVertexArray(_vertexArray.get());
		inNode->setPhysicsMeshIndexArray(_indexArray.get());
	}
	return outTriDataID;
}

crPhysicsInitVisitor::crPhysicsInitVisitor()
{
    setTraversalMode(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN);
}

crPhysicsInitVisitor::~crPhysicsInitVisitor()
{

}

void crPhysicsInitVisitor::apply(CRCore::crDB& node)
{
	m_currentDB = &node;
	//crOdeWorld::WORLDMAP &worldmap = crOdeWorld::getWorldMap();
	m_currentWorld = crOdeWorld::getOdeWorld(&node);
	if(!m_currentWorld.valid())
	{
		m_currentWorld = new crOdeWorld;

		m_currentWorld->setGravity(m_currentDB->getGravity());
		m_currentWorld->setGroundPlane(m_currentDB->getGroundPlane());
		m_currentWorld->setMaxiterations(m_currentDB->getMaxIterations());

		crOdeWorld::insertOdeWorld(&node,m_currentWorld.get());
	}

    m_dynamicSpaceStack.push(m_currentWorld->getDynamicSpace());
	m_staticSpaceID = m_currentWorld->getStaticSpace();
	traverse(node);
	//m_dynamicSpaceStack.pop();
}

void crPhysicsInitVisitor::apply(CRCore::crBillboard& node)
{
	return;
}

void crPhysicsInitVisitor::apply(CRCore::crObject& node)
{
	if( !node.getEnableIntersect() || node.getPhysicsInited() ) return;
	if(node.getCollideMode() == crNode::CollideNone || node.getCollideMode() == crNode::AvoidCollide) return;
	dGeomID obj = 0;

	const crBoundingBox &bbox = node.getBoundBox();
	crVector3 pos = bbox.center();

	//disable PrecisionCollide
	//if(node.getCollideMode() == crNode::PrecisionCollide) node.setCollideMode(crNode::CollideWithBoundBox);

	switch(node.getCollideMode()) 
	{
	case CRCore::crNode::PrecisionCollide:
		{
			//break;
			//CRCore::notify(CRCore::NOTICE)<<"crPhysicsInitVisitor::apply(CRCore::crObject& node): Use PrecisionCollide Node = "<<node.getName().c_str()<<std::endl;
			dTriMeshDataID triData = CollectMeshInfoVisitor::createTriMeshData(&node);
            if( triData ) 
			{
				node.setTriDataID(triData);
				obj = dCreateTriMesh(m_staticSpaceID, triData, 0, 0, 0);
			}
			else
			{
				obj = dCreateBox(m_staticSpaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
				dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
			}
		}
		break;
	case CRCore::crNode::CollideWithBoundBox:
		{
		    obj = dCreateBox(m_staticSpaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
		}
		break;
	case CRCore::crNode::CollideWithBoundSphere:
		{
			const crBoundingSphere &bs = node.getBound();
			obj = dCreateSphere(m_staticSpaceID,bs.radius());
			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
		}
        break;
	}
	if(obj)
	{
		crVector4f vec4;
		crVector3f vec3;
		if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"GeomRotation",vec4))
		{
			crQuat q(vec4);
			crMatrixf mat;
			mat.set(q);

			dMatrix3 r;
			crOdeWorld::creatureToOdeMat(mat,r);

			dGeomSetRotation(obj,r);
		}
		if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"GeomPosition",vec3))
		{
			dGeomSetPosition (obj,vec3[0],vec3[1],vec3[2]);
		}
		node.setPhysicsObjID(obj);
		//crOdeWorld::getStaticGeomMap()[obj] = &node;
		crOdeWorld::staticGeoMapInsert(obj,&node);

		node.setPhysicsInited(true);
	}

	//std::string str;
	//if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"CollideCallback",str))
	//{
	//	if(str.compare("DeepGroundBoxCollide")==0)
	//	{
	//		node.setCollideCallback(CRPhysics::crGroundBoxCollideCallback::getInstance());
	//	}
	//}
}

void crPhysicsInitVisitor::apply(CRCore::crGeometry& node)
{
}

void crPhysicsInitVisitor::apply(CRCore::crShapeDrawable& node)
{
}


void crPhysicsInitVisitor::apply(crMatterGroup& node)
{
	if( node.getPhysicsInited() ) return;

	std::string str;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"matterGroupMaterialString",str))
	{
		node.setMatterGroupMaterialString(str);
	}

	if(!node.getMainMatterObject() || node.getPhysicsInited())
	{
		//traverse(node);
	    return;
	}

	dSpaceID space;
	if(node.getParentSpace())
		space = node.getParentSpace()->getSpaceID();
	else 
	{
		space = dHashSpaceCreate(m_dynamicSpaceStack.top());
		//crOdeWorld::getDynamicGeomMap()[(dGeomID)space] = &node;
		crOdeWorld::dynamicGeoMapInsert((dGeomID)space,&node);
	}
	node.setSpaceID(space);
	if(node.getEnableIntersect())
		dGeomEnable((dGeomID)space);
	else
		dGeomDisable((dGeomID)space);
	//dSpaceSetCleanup(space,0);
	m_dynamicSpaceStack.push(space);
	node.setOdeWorld(m_currentWorld.get());
	
	//crCollisionData::getInstance()->addCollisionData(m_currentDB.get(),node);

	traverse(node);

	readJointsInfo(node);
    node.createJoints(m_currentWorld->getWorld(),0/*m_currentWorld->getJointGroup()*/);
	//createMatterGroupChildMap();
	m_dynamicSpaceStack.pop();

	node.setPhysicsInited(true);
}

void crPhysicsInitVisitor::apply(crMatterObject& node)
{
	if( node.getPhysicsInited() ) return;

	preReadMatterObjectInfo(node);

	node.create(m_currentWorld->getWorld(),m_dynamicSpaceStack.top());

	//postReadMatterObjectInfo(node);

	node.setPhysicsInited(true);
	if(node.isStaticMatterObject())
	{
		node.accept(s_setObjectPhysicsInit);
	}
}

void crPhysicsInitVisitor::apply(CRCore::crGroup& node)
{
	if( !node.getEnableIntersect() || node.getPhysicsInited() ) return;
	if(node.getCollideMode() == crNode::AvoidCollide) return;
	dGeomID obj = 0;
	const crBoundingBox &bbox = node.getBoundBox();
	crVector3 pos = bbox.center();
	switch(node.getCollideMode()) 
	{
	case CRCore::crNode::PrecisionCollide:
		break;
	case CRCore::crNode::CollideWithBoundBox:
		{
			obj = dCreateBox(m_staticSpaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
		}
		break;
	case CRCore::crNode::CollideWithBoundSphere:
		{
			const crBoundingSphere &bs = node.getBound();
			obj = dCreateSphere(m_staticSpaceID,bs.radius());
			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
		}
		break;
	}
	if(obj)
	{
		crVector4f vec4;
		crVector3f vec3;
		if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"GeomRotation",vec4))
		{
			crQuat q(vec4);
			crMatrixf mat;
			mat.set(q);

			dMatrix3 r;
			crOdeWorld::creatureToOdeMat(mat,r);

			dGeomSetRotation(obj,r);
		}
		if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"GeomPosition",vec3))
		{
			dGeomSetPosition (obj,vec3[0],vec3[1],vec3[2]);
		}

		node.setPhysicsObjID(obj);
		//crOdeWorld::getStaticGeomMap()[obj] = &node;
		crOdeWorld::staticGeoMapInsert(obj,&node);

		node.setPhysicsInited(true);
		node.accept(s_setObjectPhysicsInit);
	}
	else
        traverse(node);
}
void crPhysicsInitVisitor::apply(CRCore::crMatrixTransform& node)
{
	if( !node.getEnableIntersect() || node.getPhysicsInited() ) return;
    if(node.getName().compare("TerrainMatrix") == 0)
	{
		crObject *geode = dynamic_cast<crObject *>(node.getChild(0));
	    crHeightField *heightField = dynamic_cast<crHeightField *>(geode->getDrawable(0)->getShape());
		if(heightField)
		{
			if(heightField)
			{
			/////////////////////////////////////////////////////////////////////////////
				//测试表明不能使用20*20*10*10（wstep*dstep*width*depth）地形
				//if(width == 10.0f && depth == 10.0f)
				//{//避开一些引起物理引擎崩溃的特殊地形参数
				//	if(wstep == dstep)
				//	{
				//		if(wstep==10 || wstep==20)
				//			wstep++;
				//	}
				//}
				//////////////////需要在创建地形的时候避开
			 /////////////////////////////////////////////////////////////////
				int wstep = heightField->getNumColumns();
				int dstep = heightField->getNumRows();
				int width = (wstep-1) * heightField->getXInterval();
				int depth = (dstep-1) * heightField->getYInterval();
	
				dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();
				dGeomHeightfieldDataBuildSingle( heightid,(float *)(heightField->getOdeFloatArray()->getDataPointer()),
					0,width, depth, wstep, dstep,
					REAL( 1.0 ), REAL( 0.0 ), REAL( 0.0 ), 0 );
				//dGeomHeightfieldDataBuildCallback( heightid, heightField, heightfield_callback,
				//	width, depth, wstep, dstep,
				//	REAL( 1.0 ), REAL( 0.0 ), REAL( 0.0 ), 0 );
				dGeomHeightfieldDataSetBounds( heightid, -heightField->getMaxHeight(), heightField->getMaxHeight());
	
				dGeomID height = dCreateHeightfield( m_staticSpaceID, heightid, 1 );

				// Rotate so Z is up, not Y (which is the default orientation)
				dMatrix3 R;
				dRSetIdentity( R );
				dRFromAxisAndAngle( R, 1, 0, 0, DegreesToRadians(90.0f) );
				//dRFromAxisAndAngle( R, 0, 0, 1, DegreesToRadians(180.0f) );
				dGeomSetRotation( height, R );

				crVector3 pos = node.getTrans();
				dGeomSetPosition (height,pos[0],pos[1],pos[2]);
	
			    node.setHeightfieldDataID(heightid);	
				node.setPhysicsObjID(height);
				//crOdeWorld::getStaticGeomMap()[height] = &node;
				crOdeWorld::staticGeoMapInsert(height,&node);
	
				node.setPhysicsInited(true);
				crStateSet *ss = node.getOrCreateStateSet();
				crUniform *uniform = ss->getOrCreateUniform("walkable",CRCore::crUniform::FLOAT);
				uniform->setDataVariance(crBase::STATIC);
				uniform->set(1.0f);
				ss->setStateSetMode(crStateSet::SS_CollideMapRender);
				node.accept(s_setObjectPhysicsInit);
			}
		}
	}
	else apply((CRCore::crGroup&) node);
}
//void crPhysicsInitVisitor::apply(CRCore::crMatrixTransform& node)
//{
//	if( !node.getEnableIntersect() || node.getPhysicsInited() ) return;
//
//    dGeomID obj = 0;
//	crNode::CollideMode collideMode = node.getCollideMode();
//	if(collideMode == crNode::CollideWithBoundBox || collideMode == crNode::CollideWithBoundSphere)
//	{
//		crMatrix mat = node.getMatrix();
//		crMatrix oldMat = mat;
//		crMatrix resetMat = mat;
//		resetMat.setTrans(crVector3(0.0f,0.0f,0.0f));
//		resetMat = crMatrix::inverse(resetMat);
//
//		crVector3 trans = node.getBound().center();
//		crMatrix put_mat = crMatrix::translate(trans);
//		crMatrix inv_put = crMatrix::inverse(put_mat);
//		mat.postMult(inv_put);
//		mat.postMult(resetMat);
//		//mat.postMult(CRCore::crMatrix::scale(oldMat.getScale()));
//		mat.postMult(put_mat);
//
//		node.setMatrix(mat);
//		const crBoundingBox &bbox = node.getBoundBox();
//		crVector3 pos = bbox.center();
//	    if(collideMode == crNode::CollideWithBoundBox)
//		{
//			obj = dCreateBox(m_staticSpaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
//			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
//		}
//		else if(collideMode == crNode::CollideWithBoundSphere)
//		{
//			const crBoundingSphere &bs = node.getBound();
//			obj = dCreateSphere(m_staticSpaceID,bs.radius());
//			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
//		}
//        node.setMatrix(oldMat);
//		if(obj)
//		{
//			//crVector3 scale = oldMat.getScale();
//			//scale[0] = 1.0f/scale[0];
//			//scale[1] = 1.0f/scale[1];
//			//scale[2] = 1.0f/scale[2];
//			//oldMat.postMult(inv_put);
//			//oldMat.postMult(CRCore::crMatrix::scale(scale));
//   //         mat.postMult(put_mat);
//
//			dMatrix3 r;
//			crOdeWorld::creatureToOdeMat(oldMat,r);
//			dGeomSetRotation(obj,r);
//			dGeomSetPosition(obj,trans[0],trans[1],trans[2]);
//
//			node.setPhysicsObjID(obj);
//			crOdeWorld::getStaticGeomMap()[obj] = &node;
//
//			node.setPhysicsInited(true);
//		}
//	}
//    if(!obj)
//	{
//		traverse(node);
//	}
//}

//void crPhysicsInitVisitor::apply(CRCore::crOccluderNode& node)
//{
//	return;
//}

void crPhysicsInitVisitor::apply(CRCore::crSwitch& node)
{
    apply((CRCore::crGroup&) node);
}
void crPhysicsInitVisitor::apply(CRCore::crSequence& node)
{
    apply((CRCore::crGroup&) node);
}
//void crPhysicsInitVisitor::apply(CRCore::crLod& node)
//{
//	if(!node.getEnableIntersect()) return;
//
//	int pos = 0;
//    crLod::RangeList::const_iterator itr = node.getRangeList().begin();
//	float max = itr->first;
//	for( int i = 0;
//		 itr != node.getRangeList().end();
//		 ++itr,++i )
//	{
//          if( itr->first > max )
//		  {
//			  max = itr->first;
//			  pos = i;
//		  }
//	}
//
//	node.getChild(pos)->accept(*this);
//	return;
//}
//void crPhysicsInitVisitor::apply(CRCore::crEventGroup& node)
//{
//    apply((CRCore::crGroup&) node);
//}
//void crPhysicsInitVisitor::apply(CRTerrain::crTerrainTile& node)
//{
//	return apply((CRCore::crGroup&) node);
//
//	//if( !node.getEnableIntersect() || node.getPhysicsInited() ) return;
//	//dGeomID obj = 0;
//	//CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(node.getElevationLayer());
//	//if(hfl)
//	//{
// //       crHeightField *heightField = hfl->getHeightField();
//	//	if(heightField)
//	//	{
//	//		float wstep = heightField->getNumColumns();
//	//		float dstep = heightField->getNumRows();
//	//		float width = float(heightField->getNumColumns()-1) * heightField->getXInterval();
//	//		float depth = float(heightField->getNumRows()-1) * heightField->getYInterval();
//
//	//		dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();
//	//		dGeomHeightfieldDataBuildSingle( heightid,(float *)(heightField->getFloatArray()->getDataPointer()),
//	//			1,width, depth, wstep, dstep,
//	//			REAL( 1.0 ), REAL( 0.0 ), REAL( 0.05 ), 0 );
//	//		//dGeomHeightfieldDataBuildCallback( heightid, heightField, heightfield_callback,
//	//		//	width, depth, wstep, dstep,
//	//		//	REAL( 1.0 ), REAL( 0.0 ), REAL( 0.0 ), 0 );
//	//		//dGeomHeightfieldDataSetBounds( heightid, 0.0f, heightField->getMaxHeight() );
//
//	//		obj = dCreateHeightfield( m_staticSpaceID, heightid, 1 );
//	//		const crBoundingBox &bbox = node.getBoundBox();
//	//		crVector3 pos = bbox.center();
//	//		dGeomSetPosition (obj,pos[0],pos[1],pos[2]);
//
//	//		node.setHeightfieldDataID(heightid);
//	//		node.setPhysicsObjID(obj);
//	//		crOdeWorld::getStaticGeomMap()[obj] = &node;
//
//	//		node.setPhysicsInited(true);
//	//	}
//	//}
//}
//
/*
#Joint  JointHinge2 
  MGJOINTID MOTOR_LINE                     |如果省略则为WCH_MGJOINTID_DEFAULT
  JointObject 0,1                          
  anchor  25.932572, -2.461065, 0.170185   |可任意调换顺序\ 或  anchor   JointObject1.pos 或  anchor   JointObject0.pos
  axis1      0,0,1                         |并可缺项
  axis2      0,1,0                         |
#JointParameter  
  ParamSuspensionERP  0.4                  |可任意调换顺序\
  ParamSuspensionCFM  0.4                  |并可缺项
  ParamLoStop                0             |
  ParamHiStop                0             |
#JointParameterEnd
#JointEnd
 */
void crPhysicsInitVisitor::readJointsInfo(crMatterGroup& node)
{
	CRCore::crNode::DescriptionArray& desArray = node.getDescriptions();
    if(desArray.empty())
		return;

	std::string temp,strjp1,strjp2,strjp3,strMsgid,str;
	std::string *strjp;
	crJointParameter *jp = NULL;
    crJointParameter *jp1 = NULL;
	crJointParameter *jp2 = NULL;
	crJointParameter *jp3 = NULL;
	crJoint *joint = NULL;
	float f1;
	int int1;
	crVector2 vc2;
	crVector3 vc3;

	crMatterGroup::JointMap &jointMap = node.getJointMap();

    _crInt32 msgid;

	for( CRCore::crNode::DescriptionArray::iterator itr = desArray.begin();
		 itr != desArray.end(); )
	{
		crArgumentParser::readKeyValue(*itr++,"#Joint",temp);
		if(temp.empty()) continue;

		for(;(int)(itr->find("#JointEnd"))<0; itr++)
		{
			if((int)(itr->find("#JointParameter")) >= 0)
			{
				++itr;
				if(strjp1.empty())
				{
					strjp = &strjp1;
				}
				else if(strjp2.empty())
				{
					strjp = &strjp2;
				}
				else
				{
					strjp = &strjp3;
				}
				for(;(int)(itr->find("#JointParameterEnd"))<0; itr++)
					*strjp += *itr+'\n';
				continue;
			}
			temp+='\n';
			temp+=*itr;
		}

		if( (int)(temp.find("JointHinge2")) >= 0 )
		{
			crJointHinge2 *jointHinge2 = new crJointHinge2;
			joint = jointHinge2;

			if(crArgumentParser::readKeyValue(temp,"JointObject",vc2))
				jointHinge2->setJointMatterObject( dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[0])),
				dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[1])) );

			if(crArgumentParser::readKeyValue(temp,"anchor",str))
			{
				if( (int)(str.find("JointObject0.pos"))>= 0 )
				{
					vc3 = jointHinge2->getJointMatterObject(0)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.pos")) >= 0 )
				{
					vc3 = jointHinge2->getJointMatterObject(1)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.back")) >= 0 )
				{
					vc3 = jointHinge2->getJointMatterObject(1)->getInitPosition();
					vc3.y()-=jointHinge2->getJointMatterObject(1)->getLengths().y();
				}
				else if( (int)(str.find("JointObject1.top")) >= 0 )
				{
					vc3 = jointHinge2->getJointMatterObject(1)->getInitPosition();
					vc3.z()+=jointHinge2->getJointMatterObject(1)->getLengths().z()*0.5f;
				}
				jointHinge2->setAnchor1(vc3);  
			}
			else if(crArgumentParser::readKeyValue(temp,"anchor",vc3))
				jointHinge2->setAnchor1(vc3);

			if(crArgumentParser::readKeyValue(temp,"axis1",vc3))
				jointHinge2->setAxis1(vc3);

			if(crArgumentParser::readKeyValue(temp,"axis2",vc3))
				jointHinge2->setAxis2(vc3);
        }
		else if( (int)(temp.find("JointUniversal")) >= 0 )
		{
			crJointUniversal *jointUniversal = new crJointUniversal;
			joint = jointUniversal;

			if(crArgumentParser::readKeyValue(temp,"JointObject",vc2))
				jointUniversal->setJointMatterObject( dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[0])),
				    dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[1])) );

			if(crArgumentParser::readKeyValue(temp,"anchor",str))
			{
				if( (int)(str.find("JointObject0.pos"))>= 0 )
				{
					vc3 = jointUniversal->getJointMatterObject(0)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.pos")) >= 0 )
				{
					vc3 = jointUniversal->getJointMatterObject(1)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.back")) >= 0 )
				{
					vc3 = jointUniversal->getJointMatterObject(1)->getInitPosition();
					vc3.y()-=jointUniversal->getJointMatterObject(1)->getLengths().y()*0.5f;
				}
				else if( (int)(str.find("JointObject1.top")) >= 0 )
				{
					vc3 = jointUniversal->getJointMatterObject(1)->getInitPosition();
					vc3.z()+=jointUniversal->getJointMatterObject(1)->getLengths().z()*0.5f;
				}
				jointUniversal->setAnchor1(vc3);  
			}
			else if(crArgumentParser::readKeyValue(temp,"anchor",vc3))
				jointUniversal->setAnchor1(vc3);

			if(crArgumentParser::readKeyValue(temp,"axis1",vc3))
				jointUniversal->setAxis1(vc3);

			if(crArgumentParser::readKeyValue(temp,"axis2",vc3))
				jointUniversal->setAxis2(vc3);
		}

		else if( (int)(temp.find("JointHinge")) >= 0 )
		{
			crJointHinge *jointHinge = new crJointHinge;
			joint = jointHinge;

			if(crArgumentParser::readKeyValue(temp,"JointObject",vc2))
				jointHinge->setJointMatterObject( dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[0])),
				dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[1])) );

			if(crArgumentParser::readKeyValue(temp,"anchor",str))
			{
				if( (int)(str.find("JointObject0.pos"))>= 0 )
				{
					vc3 = jointHinge->getJointMatterObject(0)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.pos")) >= 0 )
				{
					vc3 = jointHinge->getJointMatterObject(1)->getInitPosition();
				}
				else if( (int)(str.find("JointObject1.back")) >= 0 )
				{
					vc3 = jointHinge->getJointMatterObject(1)->getInitPosition();
					vc3.y()-=jointHinge->getJointMatterObject(1)->getLengths().y()*0.5f;
				}
				else if( (int)(str.find("JointObject1.top")) >= 0 )
				{
					vc3 = jointHinge->getJointMatterObject(1)->getInitPosition();
					vc3.z()+=jointHinge->getJointMatterObject(1)->getLengths().z()*0.5f;
				}
				jointHinge->setAnchor1(vc3);  
			}
			else if(crArgumentParser::readKeyValue(temp,"anchor",vc3))
				jointHinge->setAnchor1(vc3);

			if(crArgumentParser::readKeyValue(temp,"axis1",vc3))
				jointHinge->setAxis1(vc3);
		}
	/*	else if( (int)(temp.find("JointBall")) >= 0 )
		{
			crJointBall *jointBall = new crJointBall;
			joint = jointBall;
		}*/
		else if( (int)(temp.find("JointAMotor")) >= 0 )
		{
			crJointAMotor *jointAMotor = new crJointAMotor;
			joint = jointAMotor;

			if(crArgumentParser::readKeyValue(temp,"JointObject",vc2))
				jointAMotor->setJointMatterObject( dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[0])),
				dynamic_cast<crMatterObject *>(node.getChild((unsigned int)vc2[1])) );

			if(crArgumentParser::readKeyValue(temp,"AMotorMode",str))
			{
				if(str.compare("AMotorUser")==0)
				{
					jointAMotor->setAMotorMode(crJointAMotor::AMotorUser);
				}
				else
				{
					jointAMotor->setAMotorMode(crJointAMotor::AMotorEuler);
				}
			}
			if(crArgumentParser::readKeyValue(temp,"AMotorNumAxes",int1))
				jointAMotor->setAMotorNumAxes(int1);
			if(crArgumentParser::readKeyValue(temp,"AMotorAxisRel1",int1))
				jointAMotor->setRels(0,int1);
			if(crArgumentParser::readKeyValue(temp,"AMotorAxisRel2",int1))
				jointAMotor->setRels(1,int1);
			if(crArgumentParser::readKeyValue(temp,"AMotorAxisRel3",int1))
				jointAMotor->setRels(2,int1);
			if(crArgumentParser::readKeyValue(temp,"axis1",vc3))
				jointAMotor->setAxis(0,vc3);
			if(crArgumentParser::readKeyValue(temp,"axis2",vc3))
				jointAMotor->setAxis(1,vc3);
			if(crArgumentParser::readKeyValue(temp,"axis3",vc3))
				jointAMotor->setAxis(2,vc3);
			if(crArgumentParser::readKeyValue(temp,"AMotorAngle1",f1))
				jointAMotor->setAMotorAngle(0,f1);
			if(crArgumentParser::readKeyValue(temp,"AMotorAngle2",f1))
				jointAMotor->setAMotorAngle(1,f1);
			if(crArgumentParser::readKeyValue(temp,"AMotorAngle3",f1))
				jointAMotor->setAMotorAngle(2,f1);
		}

		if(!joint) continue;

		msgid = WCH_MGJOINTID_DEFAULT;
		if(crArgumentParser::readKeyValue(temp,"MGJOINTID",strMsgid))
		{
			if(strMsgid.compare("MOTOR_LINE")==0)
			{
                msgid = WCH_MGJOINTID_MOTOR_LINE;
			}
			else if(strMsgid.compare("MOTOR_ANGLE")==0)
			{
				msgid = WCH_MGJOINTID_MOTOR_ANGLE;
			}
			else if(strMsgid.compare("STEER")==0)
			{
				msgid = WCH_MGJOINTID_STEER;
			}
			else if(strMsgid.compare("WEAPON")==0)
			{
				msgid = WCH_MGJOINTID_WEAPON;
			}
			else if(strMsgid.compare("LARM")==0)
			{
				msgid = WCH_MGJOINTID_LARM;
			}
			else if(strMsgid.compare("LHAND")==0)
			{
				msgid = WCH_MGJOINTID_LHAND;
			}
			else if(strMsgid.compare("RARM")==0)
			{
				msgid = WCH_MGJOINTID_RARM;
			}
			else if(strMsgid.compare("RHAND")==0)
			{
				msgid = WCH_MGJOINTID_RHAND;
			}
			else if(strMsgid.compare("LTHIGH")==0)
			{
				msgid = WCH_MGJOINTID_LTHIGH;
			}
			else if(strMsgid.compare("LCALF")==0)
			{
				msgid = WCH_MGJOINTID_LCALF;
			}
			else if(strMsgid.compare("RTHIGH")==0)
			{
				msgid = WCH_MGJOINTID_RTHIGH;
			}
			else if(strMsgid.compare("RCALF")==0)
			{
				msgid = WCH_MGJOINTID_RCALF;
			}
		}

		temp.clear();

		///read crJointParameter
        while(!strjp1.empty()||!strjp2.empty()||!strjp3.empty())
		{
			if(!strjp1.empty())
			{
				jp1 = new crJointParameter;
                jp = jp1;
				strjp = &strjp1;
			}
			else if(!strjp2.empty())
			{
				jp2 = new crJointParameter;
				jp = jp2;
				strjp = &strjp2;
			}
			else
			{
				jp3 = new crJointParameter;
				jp = jp3;
				strjp = &strjp3;
			}

			if(crArgumentParser::readKeyValue(*strjp,"ParamLoStop",f1))
				jp->setParamLoStop(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamHiStop",f1))
				jp->setParamHiStop(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamVel",f1))
				jp->setParamVel(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamFMax",f1))
				jp->setParamFMax(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamFudgeFactor",f1))
				jp->setParamFudgeFactor(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamBounce",f1))
				jp->setParamBounce(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamCFM",f1))
				jp->setParamCFM(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamStopERP",f1))
				jp->setParamStopERP(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamStopCFM",f1))
				jp->setParamStopCFM(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamSuspensionERP",f1))
				jp->setParamSuspensionERP(f1);
			if(crArgumentParser::readKeyValue(*strjp,"ParamSuspensionCFM",f1))
				jp->setParamSuspensionCFM(f1);

			strjp->clear();
		}

		joint->setJointParameters(jp1,jp2,jp3);

		jp = jp1 = jp2 = jp3 = NULL;

		jointMap.insert(std::make_pair(msgid,joint));

		joint = NULL;
	}
}

void crPhysicsInitVisitor::preReadMatterObjectInfo(crMatterObject& node)
{
	CRCore::crNode::DescriptionArray& desArray = node.getDescriptions();
	if(desArray.empty())
		return;
	float f1;
	crVector3 vc3;
	std::string strbuf;
	for( CRCore::crNode::DescriptionArray::iterator itr = desArray.begin();
		itr != desArray.end(); itr++ )
	{
		if(crArgumentParser::readKeyValue(*itr,"mass",f1))
		{
			node.setMass(f1);
		}
		if(crArgumentParser::readKeyValue(*itr,"initPos",vc3))
		{
			node.setInitPosition(vc3);
		}
		if(crArgumentParser::readKeyValue(*itr,"baryCenter",vc3))
		{
			node.setBarycenter(vc3);
		}
	}
}

void crPhysicsInitVisitor::postReadMatterObjectInfo(crMatterObject& node)
{
	//CRCore::crNode::DescriptionArray& desArray = node.getDescriptions();
	//if(desArray.empty())
	//	return;

	//crVector3 vc3;

	//for( CRCore::crNode::DescriptionArray::iterator itr = desArray.begin();
	//	itr != desArray.end(); )
	//{
	//}
}

//////////////////////////////////////
//
//crPhysicsReleaseVisitor
//
////////////////////////////
crPhysicsReleaseVisitor::crPhysicsReleaseVisitor()
{
	setTraversalMode(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN);
}

crPhysicsReleaseVisitor::~crPhysicsReleaseVisitor()
{

}

crPhysicsReleaseVisitor* crPhysicsReleaseVisitor::getInstance()
{
	static ref_ptr<crPhysicsReleaseVisitor> m_instance = new crPhysicsReleaseVisitor;
	return m_instance.get();
}

void crPhysicsReleaseVisitor::apply(CRCore::crNode& node)
{
	dGeomID geomID = (dGeomID)(node.getPhysicsObjID());
	if(geomID)
	{
		crOdeWorld::staticGeoMapErase(geomID);
	}
	node.releaseOde();
	traverse(node);
}

void crPhysicsReleaseVisitor::apply(crMatterGroup& node)
{
	traverse(node);//先dGeomDestroy
	node.releaseOde();//后dSpaceDestroy
}

void crPhysicsReleaseVisitor::apply(crMatterObject& node)
{
	node.releaseOde();
}