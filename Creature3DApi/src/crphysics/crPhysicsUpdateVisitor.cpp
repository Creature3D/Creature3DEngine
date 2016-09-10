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
#include <CRPhysics/crPhysicsUpdateVisitor.h>

using namespace CRCore;
using namespace CRPhysics;

crPhysicsUpdateVisitor::crPhysicsUpdateVisitor():
  crNodeVisitor(PHYSICS_UPDATE_VISITOR,TRAVERSE_ALL_CHILDREN)
{
}

crPhysicsUpdateVisitor::~crPhysicsUpdateVisitor()
{
}

void crPhysicsUpdateVisitor::apply(CRCore::crDB& node)
{
	if(node.getEnableIntersect() /*&& node.getVisiable()*/)
	{
		m_currentDB = &node;

		//crPhysicsSystemUpdater::getInstance()->physicsUpdateBlock();
		traverse(node);//crematrix->ode

		//crMatterGroup::resetContactInfoList();
		//crOdeWorld *odeWorld = crOdeWorld::getWorldMap()[&node].get();
		//odeWorld->collide();
		//odeWorld->update();
	}
}


void crPhysicsUpdateVisitor::apply(crTransform& node)
{//对于物理体，不要有缩放矩阵。
	if(node.getEnableIntersect() /*&& node.getVisiable()*/)
	{
		if(node.getPhysicsInited())
		{//地形
			dGeomID obj = (dGeomID)(node.getPhysicsObjID());
			if(!obj) return;

			if(node.getName().compare("TerrainMatrix") == 0)
			{
				CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
				node.computeLocalToWorldMatrix(*matrix,this);

				if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
				{
					if(!m_matrixList.empty())
						matrix->preMult(*(m_matrixList.back().get()));
				}
				dMatrix3 r;
				crOdeWorld::creatureToOdeMat(*matrix,r);
				dMatrix3 R;
				dRSetIdentity( R );
				dRFromAxisAndAngle( R, 1, 0, 0, DegreesToRadians(90.0f) );
				dMatrix3 actual_R;
				dMULTIPLY0_333 (actual_R,r,R);
				dGeomSetRotation(obj,actual_R);//对于物理体，不要有缩放矩阵。
				crVector3 pos = matrix->getTrans();
				dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
			}
			else 
			{
				if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
				{
					if(!m_matrixList.empty())
					{
						CRCore::crVector3 pos = node.getBoundBox().center();
						crMatrix &mat = *(m_matrixList.back().get());
						pos = pos * mat;

						dMatrix3 r;
						crOdeWorld::creatureToOdeMat(mat,r);

						dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
						dGeomSetRotation(obj,r);//对于物理体，不要有缩放矩阵。
					}
				}
			}
		}
		else
		{
			CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
			node.computeLocalToWorldMatrix(*matrix,this);

			if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
			{
				if(!m_matrixList.empty())
					matrix->preMult(*(m_matrixList.back().get()));
			}

			pushMatrix(matrix.get());

			traverse(node); 

			popMatrix();
		}
	}
}

void crPhysicsUpdateVisitor::apply(crMatterGroup& node)
{
	if(node.getPhysicsInited() /*&& node.getVisiable() && node.getEnableIntersect()*/) traverse(node);
}

void crPhysicsUpdateVisitor::apply(crMatterObject& node)
{
	if(node.getPhysicsInited() /*&& node.getVisiable() && node.getEnableIntersect()*/) 
	{
		node.updateOdeMatrix();
		//node.swapPhysicsBuffers();
	}
}

void crPhysicsUpdateVisitor::apply(crViewMatterObject& node)
{
	//if(!node.isDead())
	//{
 //       node.getGameBodyInfo()->updateLockedTarget(node.getTargetCount(),node.getCurrentWeaponMask());
	//}
    apply((crMatterObject&) node);
}

void crPhysicsUpdateVisitor::apply(crBillboard& node)
{
	return;
}

void crPhysicsUpdateVisitor::apply(crObject& node)
{
	if(node.getEnableIntersect() && node.getPhysicsInited())
	{
		dGeomID obj = (dGeomID)(node.getPhysicsObjID());
		if(!obj) return;

		if(!m_matrixList.empty())
		{
			crMatrix &mat = *(m_matrixList.back().get());
            CRCore::crVector3 pos;

			switch(node.getCollideMode()) 
			{
			case CRCore::crObject::PrecisionCollide:
				//dGeomTriMeshDataSet( (dTriMeshDataID)node.getTriDataID(),
				//	TRIMESH_LAST_TRANSFORMATION,
				//	(void *) mat.ptr() );
                pos = mat.getTrans();
				break;
			case CRCore::crObject::CollideWithBoundBox:
			case CRCore::crObject::CollideWithBoundSphere:
				pos = node.getBoundBox().center();
				pos = pos * mat;
				break;
			}
		    dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
			dMatrix3 r;
			crOdeWorld::creatureToOdeMat(mat,r);
			dGeomSetRotation(obj,r);//对于物理体，不要有缩放矩阵。
		}
	}
}

void crPhysicsUpdateVisitor::apply(CRCore::crGroup& node)
{
	if(node.getEnableIntersect())
	{
		if(node.getPhysicsInited())
		{
			dGeomID obj = (dGeomID)(node.getPhysicsObjID());
			if(!obj) return;

			if(!m_matrixList.empty())
			{
				crMatrix &mat = *(m_matrixList.back().get());

				CRCore::crVector3 pos = node.getBoundBox().center();
				pos = pos * mat;

				dMatrix3 r;
				crOdeWorld::creatureToOdeMat(mat,r);

				dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
				dGeomSetRotation(obj,r);//对于物理体，不要有缩放矩阵。

				//switch(node.getCollideMode()) 
				//{
				//case CRCore::crObject::PrecisionCollide:
				//	traverse(node);
				//	break;
				//case CRCore::crObject::CollideWithBoundBox:
				//case CRCore::crObject::CollideWithBoundSphere:
				//	dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
				//	dGeomSetRotation(obj,r);
				//	break;
				//}
			}
		}
		else
		{
	        traverse(node);
		}
	}

	//if(node.getEnableIntersect() /*&& node.getVisiable()*/)
	//{
	//	traverse(node);
	//	//dGeomID obj = (dGeomID)(node.getPhysicsObjID());
	//	//if(!obj) 
	//	//{
	//	//	traverse(node);
	//	//	return;
	//	//}

	//	//if(!m_matrixList.empty())
	//	//{
	//	//	crMatrix &mat = *(m_matrixList.back().get());
	//	//	CRCore::crVector3d pos = node.getBoundBox().center();
	//	//	pos = pos * mat;
	//	//	dMatrix3 r;
	//	//	crOdeWorld::creatureToOdeMat(mat,r);

	//	//	switch(node.getCollideMode()) 
	//	//	{
	//	//	case CRCore::crObject::PrecisionCollide:
	//	//		traverse(node);
	//	//		break;
	//	//	case CRCore::crObject::CollideWithBoundBox:
	//	//	case CRCore::crObject::CollideWithBoundSphere:
	//	//		dGeomSetPosition(obj,pos.x(),pos.y(),pos.z());
	//	//		dGeomSetRotation(obj,r);
	//	//		break;
	//	//	}
	//	//}
	//}
}

void crPhysicsUpdateVisitor::apply(CRCore::crSwitch& node)
{
	apply((CRCore::crGroup&) node);
}
void crPhysicsUpdateVisitor::apply(CRCore::crSequence& node)
{
	apply((CRCore::crGroup&) node);
}
//void crPhysicsUpdateVisitor::apply(CRCore::crLod& node)
//{
//	if(!node.getEnableIntersect()) return;
//
//	int pos = 0;
//	crLod::RangeList::const_iterator itr = node.getRangeList().begin();
//	float max = itr->first;
//	for( int i = 0;
//		itr != node.getRangeList().end();
//		++itr,++i )
//	{
//		if( itr->first > max )
//		{
//			max = itr->first;
//			pos = i;
//		}
//	}
//
//	node.getChild(pos)->accept(*this);
//	return;
//}
//void crPhysicsUpdateVisitor::apply(CRCore::crOccluderNode& node)
//{
//	return;
//}
//void crPhysicsUpdateVisitor::apply(CRTerrain::crTerrainTile& node)
//{
//	return apply((CRCore::crGroup&) node);
//
//	//if(node.getEnableIntersect() && node.getPhysicsInited())
//	//{
//	//	dGeomID obj = (dGeomID)(node.getPhysicsObjID());
//	//	if(!obj) return;
//	//	crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(node.getChild(0));
//	//	if(matrixTransform)
//	//	{
//	//		//CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
//	//		//matrixTransform->computeLocalToWorldMatrix(*matrix,this);
//
//	//		//if( matrixTransform->getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
//	//		//{
//	//		//	if(!m_matrixList.empty())
//	//		//		matrix->preMult(*(m_matrixList.back().get()));
//	//		//}
//
//	//		//pushMatrix(matrix.get());
//	//		//if(!m_matrixList.empty())
//	//		//{
//	//		    //crMatrix mat;// = matrixTransform->getMatrix();
//	//			//crMatrix &mat = *(m_matrixList.back().get());
//	//			crMatrix mat = crMatrix::rotate(90,CRCore::X_AXIS);
// //				CRCore::crVector3 pos = node.getBoundBox().center();
//	//			//pos[2] = mat.getTrans()[2];
// //				//pos = pos * mat;
//	//			//CRCore::crVector3 pos = mat.getTrans();
//	//			dMatrix3 r;
//	//			crOdeWorld::creatureToOdeMat(mat,r);
//	//			dGeomSetRotation(obj,r);
//	//			dGeomSetPosition(obj,pos[0],pos[1],pos[2]);
//	//		//}
//	//	 //   popMatrix();
//	//	}
//	//}
//}
