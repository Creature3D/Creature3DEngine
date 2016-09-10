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
#include <CRPhysics\crMatterObject.h>
#include <CRPhysics\crOdeWorld.h>
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRCore\crBoundingBox.h>
#include <CRCore\crBoundingSphere.h>
#include <CRCore\crArray.h>
#include <CRCore\crNotify.h>
#include <CRCore/crMath.h>
#include <CRCore/crArgumentParser.h>
#include <CRPhysics/crVolumeNode.h>

//#include <CRNet/crNetManager.h>

using namespace CRCore;
using namespace CRPhysics;

//const CRCore::crVector3f Z_AXIS(0.0f,0.0f,1.0f);

crMatterObject::crMatterObject():
    m_mass(0.1),
	m_density(1.0),
    m_geomID(0),
	m_bodyID(0),
	m_matrixDummyType(MD_None),
	m_gravityMode(1),
	//m_previousFrameTime(0.0f),
	m_frameInterval(0.0f),
	m_currentBodyPhysicsType(RayDetect),
	m_physicsSwapInterval(0.0f),
	m_bStaticMatterObject(false)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}
crMatterObject::~crMatterObject()
{
}
crMatterObject::crMatterObject(const crMatterObject& matterObject,const crCopyOp& copyop):
    crMatrixTransform(matterObject,copyop),    
    m_mass(matterObject.m_mass),
	m_density(matterObject.m_density),
	m_barycenter(matterObject.m_barycenter),
    m_inertiaMass(matterObject.m_inertiaMass),
	m_initPosition(matterObject.m_initPosition),
	m_matrixDummyType(MD_None),
	m_gravityMode(matterObject.m_gravityMode),
	//m_previousFrameTime(0.0f),
	m_frameInterval(0.0f),
	m_currentBodyPhysicsType(RayDetect),
	m_physicsSwapInterval(0.0f),
	m_geomID(0),
	m_bodyID(0),
	m_bStaticMatterObject(matterObject.m_bStaticMatterObject)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

void crMatterObject::setStaticMatterObject(bool s)
{
    m_bStaticMatterObject = s;
}
bool crMatterObject::isStaticMatterObject() const
{
    return m_bStaticMatterObject;
}

void crMatterObject::releaseObjects(CRCore::crState* state)
{
	m_physicsInited = false;
	crMatrixTransform::releaseObjects(state);
	//releaseOde();
}

void crMatterObject::releaseOde()
{
	if(m_bodyID) 
	{
		dBodyDestroy(m_bodyID);
		m_bodyID = NULL;
	}
	if(m_geomID)
	{
		dGeomDestroy(m_geomID);
		crOdeWorld::dynamicGeoMapErase(m_geomID);
		m_geomID = NULL;
	}
	m_physicsInited = false;
}

bool crMatterObject::computeBound() const
{
	return crMatrixTransform::computeBound();
}

void crMatterObject::setMass( float mass )
{ 
	m_mass = mass; 
	//if(m_bodyID)
	//{
 //       dMass m;	
	//	dMassAdjust(&m,m_mass);
	//	dBodySetMass (m_bodyID,&m);
	//}
}

CRCore::crVector3f crMatterObject::getLengths()
{
	return CRCore::crVector3f(getBoundBox().xLength(),getBoundBox().yLength(),getBoundBox().zLength());
}

void crMatterObject::create(dWorldID worldID, dSpaceID spaceID)
{
	if(!m_bbox.valid()) getBound();

	crMatrix dummyMatrix;
	unsigned char matrixType;
	getMatterObjectMatrix(dummyMatrix,matrixType);
	if(matrixType!=MD_None)
	{
		switch (matrixType)
		{
		case MD_RotMatrix:
			dummyMatrix.setTrans(m_matrix.getTrans());
			m_matrix = dummyMatrix;
			break;
		case MD_TransMatrix:
			m_matrix.setTrans(dummyMatrix.getTrans());
			break;
		case MD_FullMatrix:
			m_matrix = dummyMatrix;
			break;
		}
	}
	_create(worldID,spaceID);

    if(m_bodyID)
	{
		if(m_enableIntersect)
			dBodyEnable(m_bodyID);
		else
			dBodyDisable(m_bodyID);
	}
	if(m_geomID)
	{
	    //crOdeWorld::getDynamicGeomMap()[m_geomID] = this;
		crOdeWorld::dynamicGeoMapInsert(m_geomID,this);
		if(m_enableIntersect)
			dGeomEnable(m_geomID);
		else
			dGeomDisable(m_geomID);
	}

	crVector4f vec4;
	crVector3f vec3;
	if(CRCore::crArgumentParser::readKeyValue(getDescriptions(),"GeomRotation",vec4))
	{
		crQuat q(vec4);
		crMatrixf mat;
		mat.set(q);

		dMatrix3 r;
		crOdeWorld::creatureToOdeMat(mat,r);

		if(m_geomID)
		    dGeomSetRotation(m_geomID,r);
		if(m_bodyID)
		    dBodySetRotation(m_bodyID,r);
		//dMatrix3 R;
		//if(vec3[0]!=0.0f) dRFromAxisAndAngle (R,1,0,0,vec3[0]);
		//if(vec3[1]!=0.0f) dRFromAxisAndAngle (R,0,1,0,vec3[1]);
		//if(vec3[2]!=0.0f) dRFromAxisAndAngle (R,0,0,1,vec3[2]);

		//if(m_bodyID)
		//{
		//	dMass m;
		//	dBodyGetMass(m_bodyID,&m);
		//	dBodySetRotation(m_bodyID,R);
		//	dMassRotate(&m,R);
  //          dBodySetMass (m_bodyID,&m);
		//}
		//else if(m_geomID)
		//    dGeomSetRotation(m_geomID,R);
	}
	if(CRCore::crArgumentParser::readKeyValue(getDescriptions(),"GeomPosition",vec3))
	{
		//vec3 += m_initPosition;
		if(m_bodyID)
			dBodySetPosition (m_bodyID,vec3[0],vec3[1],vec3[2]);
	    if(m_geomID)
			dGeomSetPosition (m_geomID,vec3[0],vec3[1],vec3[2]);
	}

	initPhysicsBuffers();

	m_physicsInited = true;

	if(!dynamic_cast<crViewMatterObject *>(this) && crDisplaySettings::instance()->getRunMode() > 0)//Game
	    enablePhysics(1.0f);
}

void crMatterObject::setEnableIntersect( bool enableIntersect)
{
	crMatrixTransform::setEnableIntersect(enableIntersect);
}

void crMatterObject::setGravityMode(int mode)
{
	m_gravityMode = mode;
}

void crMatterObject::updateCreMatrix()
{
	if(!m_geomID) return;

	const dReal *d = dGeomGetPosition(m_geomID);
	const dReal *r = dGeomGetRotation(m_geomID);

	m_matrix_buf.set( r[0], r[4], r[8], 0,
		r[1], r[5], r[9], 0,
		r[2], r[6], r[10], 0,
		d[0]/*-m_initPosition[0]*/, d[1]/*-m_initPosition[1]*/, d[2]/*-m_initPosition[2]*/, 1.0 );

	//if(CRNet::crNetManager::getInstance()->getNetType() == CRNet::crNetManager::Server)
	//{
	//	CRNet::crNetManager::getInstance()->pushCreMatrix(getName(),m_matrix);
	//}

	dirtyBound();
}

void crMatterObject::updateOdeMatrix()
{//对于矩阵并不是只被ODE控制，在外部有变化的时候需要继承
	if(m_bodyID) dBodySetGravityMode(m_bodyID,m_gravityMode);

	crMatrix dummyMatrix;
	unsigned char matrixType;
	getMatterObjectMatrix(dummyMatrix,matrixType);
	if(m_geomID && matrixType!=MD_None)
	{
		switch (matrixType)
		{
		case MD_RotMatrix:
			dummyMatrix.setTrans(m_matrix.getTrans());
			m_matrix = dummyMatrix;
			break;
		case MD_TransMatrix:
			m_matrix.setTrans(dummyMatrix.getTrans());
			break;
		case MD_FullMatrix:
			m_matrix = dummyMatrix;
			break;
		}
		CRCore::crVector3 pos = m_matrix.getTrans();
		dGeomSetPosition (m_geomID,pos[0],pos[1],pos[2]);

		dMatrix3 r;
		crOdeWorld::creatureToOdeMat(m_matrix,r);
		dGeomSetRotation(m_geomID,r);

		if(m_bodyID)
		{
			dBodySetPosition (m_bodyID,pos[0],pos[1],pos[2]);
			dBodySetRotation(m_bodyID,r);
		}
	}
	m_matrix_buf = m_matrix;
}

bool crMatterObject::physicsSwap()
{
	if(!m_geomID || !m_bodyID) return false;

	if(m_currentBodyPhysicsType==RayDetect)
	{
		m_currentBodyPhysicsType = RealPhysics;
		//dGeomEnable(m_geomID);
		//dBodyEnable(m_bodyID);
	}
	else// if(m_canDisablePhysics)
	{
		m_currentBodyPhysicsType = RayDetect;
		//dGeomDisable(m_geomID);
		//dBodyDisable(m_bodyID);
		m_linearVel.set(0.0f,0.0f,0.0f);
		m_force.set(0.0f,0.0f,0.0f);
	}

	return true;
}

void crMatterObject::resetPhysicsSwap()
{
	if(!m_geomID || !m_bodyID) return;

	m_currentBodyPhysicsType = RayDetect;
	//dBodyDisable(m_bodyID);
	m_physicsSwapInterval = 0;
	m_linearVel.set(0.0f,0.0f,0.0f);
	m_force.set(0.0f,0.0f,0.0f);
}

void crMatterObject::setLinearVel(const CRCore::crVector3f& vel, float interval)
{
	if(!m_geomID || !m_bodyID) return;

	if(m_currentBodyPhysicsType != RealPhysics)
	{
		if(!physicsSwap()) 
			return;
		else
			m_physicsSwapInterval = interval;
	}
	else
		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);

	m_linearVel = vel;
	//dBodySetLinearVel(m_bodyID,vel[0],vel[1],vel[2]);
}

void crMatterObject::addForce(const CRCore::crVector3f& force, float interval)
{
	if(!m_geomID || !m_bodyID) return;

	if(m_currentBodyPhysicsType != RealPhysics)
	{
		if(!physicsSwap()) 
			return;
		else
			m_physicsSwapInterval = interval;
	}
	else
		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);

	//dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
	m_force = force;
}

void crMatterObject::enablePhysics(float interval)
{
	if(!m_geomID || !m_bodyID) return;

	if(m_currentBodyPhysicsType != RealPhysics)
	{
		if(!physicsSwap()) 
			return;
		else
			m_physicsSwapInterval = interval;
	}
	else
		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);
}

void crMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			m_frameInterval = crFrameStamp::getInstance()->getFrameInterval();
			if(m_bodyID)
			{
				//double referenceTime = nv.getFrameStamp()->getReferenceTime();
				//m_frameInterval = (m_previousFrameTime != 0.0) ? (referenceTime-m_previousFrameTime) : 0.0;
				//m_previousFrameTime = referenceTime;
				if(m_physicsSwapInterval>0.0f)
					m_physicsSwapInterval -= m_frameInterval;
				else if(m_physicsSwapInterval < 0.0f)
				{
					physicsSwap();
					m_physicsSwapInterval = 0.0f;
					//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::traverse(): physicsSwap to RayDetect = "<<(m_currentBodyPhysicsType==RayDetect)<<std::endl;
				}

				if(m_enableIntersect && m_currentBodyPhysicsType==RealPhysics)
				{
					dBodyEnable(m_bodyID);
					if(m_linearVel!=crVector3f(0.0f,0.0f,0.0f)) dBodySetLinearVel(m_bodyID,m_linearVel[0],m_linearVel[1],m_linearVel[2]);
					if(m_force!=crVector3f(0.0f,0.0f,0.0f)) dBodyAddForce(m_bodyID,m_force[0],m_force[1],m_force[2]);
					m_linearVel.set(0.0f,0.0f,0.0f);
					m_force.set(0.0f,0.0f,0.0f);
				}
				else
				{
					dBodyDisable(m_bodyID);
				}
			}
			if(m_geomID)
			{
				if((int)m_enableIntersect^dGeomIsEnabled(m_geomID))
				{
					if(m_enableIntersect)
						dGeomEnable(m_geomID);
					else
						dGeomDisable(m_geomID);
				}
			}

			//if(m_visiable && hasVolumeEvent())
			crVolumeNodeManager::getInstance()->testVolume(this);
		}
		break;
	}

	crMatrixTransform::traverse(nv);
}

//bool crMatterObject::hasVolumeEvent()
//{
//    return hasMsgEvent(WCH_INVOLUME) || hasMsgEvent(WCH_OUTVOLUME);
//}

void crMatterObject::setMatterObjectMatrix(CRCore::crMatrix &matrix,unsigned char matrixtype)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_matrixDummyMutex);
	if(m_matrixDummyType!=MD_None)
	{
		if(matrixtype & MD_TransMatrix)
		{
			m_matrixDummy.setTrans(matrix.getTrans());
		}
		if(matrixtype & MD_RotMatrix)
		{
			matrix.setTrans(m_matrixDummy.getTrans());
			m_matrixDummy = matrix;
		}
		m_matrixDummyType |= matrixtype;
	}
	else
	{
		m_matrixDummyType = matrixtype;
        m_matrixDummy = matrix;
	}
}
void crMatterObject::getMatterObjectMatrix(CRCore::crMatrix &dummyMatrix,unsigned char& matrixtype)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_matrixDummyMutex);
	dummyMatrix = m_matrixDummy;
    matrixtype = m_matrixDummyType;
	m_matrixDummyType = MD_None;
}

CRCore::crVector3f crMatterObject::getDirection() const
{
	crMatrixf rotation = m_matrix;
	rotation.setTrans(0.0f,0.0f,0.0f);
	return (Y_AXIS * rotation).normalize();
}

CRCore::crVector3f crMatterObject::getDirectionX() const
{
	crMatrixf rotation = m_matrix;
	rotation.setTrans(0.0f,0.0f,0.0f);
	return (X_AXIS * rotation).normalize();
}

double crMatterObject::checkUpVector()
{
	crMatrixd rotation = m_matrix;
	rotation.setTrans(0.0,0.0,0.0);
	return (Z_AXIS * rotation).normalize()*Z_AXIS;
}

CRCore::crMatrix crMatterObject::getMatterObjectRotation() const
{
    crMatrix mat = m_matrix;
	mat.setTrans(0,0,0);
	return mat; 
}

//CRCore::crVector3f crMatterObject::getMatterObjectPosition() const
//{ 
//	return this->getBound().center();
//	//return m_matrix.getTrans(); 
//}

//crMatterObject::EventMap &crMatterObject::getEventMap()
//{
//    return m_eventMap;
//}
//
//void crMatterObject::insertEvent(_crInt64 kbmsg, CRCore::crEventCallback *callback)
//{
//	m_eventMap.insert(std::make_pair(kbmsg,callback));
//}
//
//void crMatterObject::doEvent(_crInt64 kbmsg, _crInt64 param)
//{
//	for( EventMap::iterator itr = m_eventMap.find(kbmsg); 
//		itr != m_eventMap.end() && itr->first == kbmsg; 
//		itr++ )
//	{
//		itr->second->initCallbackTask();
//		if(dynamic_cast<Callback *>(itr->second.get()))
//			(*(dynamic_cast<Callback *>(itr->second.get())))(*this,param);
//		else (*(itr->second))();
//		if(!itr->second->continueCallbackTask()) break;
//	}
//}

//void crMatterObject::setCollideCallback( CRCore::crCollideCallback *collidecbk )
//{ 
//    if(m_collideCallback.valid())
//		m_collideCallback->addNestedCallback(collidecbk);
//	else
//	    m_collideCallback = collidecbk; 
//}


crBoxMatterObject::crBoxMatterObject(const crBoxMatterObject& boxmatterObject,const crCopyOp& copyop):
    crMatterObject(boxmatterObject,copyop)
{

}

void crBoxMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	if(!m_bStaticMatterObject)
	{
		dMass m;
		m_bodyID = dBodyCreate(worldID);
		//m_initPosition = m_bbox.center();
		//dBodySetPosition(m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
		dMassSetBox (&m,m_density,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
		dMassAdjust (&m,m_mass);
		dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		dBodySetMass (m_bodyID,&m);
		dGeomSetBody (m_geomID,m_bodyID);
		dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	}
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

/////crSphereMatterObject

crSphereMatterObject::crSphereMatterObject(const crSphereMatterObject& spherematterObject,const crCopyOp& copyop):
crMatterObject(spherematterObject,copyop)
{

}

void crSphereMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	float radius = m_boundSphere.radius();
	radius *= radius < 0.8f ? 0.67382f : 0.58f;//0.7071
	m_geomID = dCreateSphere(spaceID,radius);
	if(!m_bStaticMatterObject)
	{
		dMass m;
		m_bodyID = dBodyCreate(worldID);
		//	m_initPosition = m_bbox.center();
		//m_initPosition[0] += m_bbox.radius()/2;
		//m_initPosition[1] += m_bbox.radius()/2;
		//m_initPosition[2] += m_bbox.radius()/2;
		//if(radius<0.8f) radius = radius/2.0f+radius/6.0f+0.0715f;
		//else radius *= 0.58f;
		dMassSetSphere (&m,m_density,radius);
		dMassAdjust (&m,m_mass);
		dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		dBodySetMass (m_bodyID,&m);
		dGeomSetBody (m_geomID,m_bodyID);
		dBodySetPosition(m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	}
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

/////crCCylinderMatterObject

crCCylinderMatterObject::crCCylinderMatterObject(const crCCylinderMatterObject& matterObject,const crCopyOp& copyop):
crMatterObject(matterObject,copyop)
{

}

void crCCylinderMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	float radius = m_bbox.xLength()*0.8f;
	float length = m_bbox.zLength();
	m_geomID = dCreateCCylinder(spaceID,radius,length);
	if(!m_bStaticMatterObject)
	{
		dMass m;
		m_bodyID = dBodyCreate(worldID);
		//dMassSetCappedCylinder(&m,m_density,3,radius,length);
		dMassSetCapsule(&m,m_density,3,radius,length);
		dMassAdjust (&m,m_mass);
		dBodySetMass (m_bodyID,&m);
		dGeomSetBody (m_geomID,m_bodyID);

		dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
		dMassTranslate (&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		dBodySetFiniteRotationMode(m_bodyID,1);//高速旋转时,有助于提高准确率
	}
	//dMatrix3 Rtx;
	//dRFromAxisAndAngle(Rtx,0.0f,1.0f,0.0f,PI_2);
	//dGeomSetRotation (m_geomID,Rtx);
	//dMassRotate (&m,Rtx);
	dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
}

/////crCylinderMatterObject

crCylinderMatterObject::crCylinderMatterObject(const crCylinderMatterObject& matterObject,const crCopyOp& copyop):
crMatterObject(matterObject,copyop)
{

}

void crCylinderMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	//dMass m;
	//m_bodyID = dBodyCreate(worldID);
	//float radius = m_bbox.xLength()*0.8f;
	//float length = m_bbox.zLength();
	//dMassSetCylinder(&m,m_density,3,radius,length);
	//dMassAdjust (&m,m_mass);
	//dBodySetMass (m_bodyID,&m);

	//m_geomID = dCreateCylinder(spaceID,radius,length);
	//dGeomSetBody (m_geomID,m_bodyID);

	////dMatrix3 Rtx;
	////dRFromAxisAndAngle(Rtx,0.0f,1.0f,0.0f,PI_2);
	////dGeomSetRotation (m_geomID,Rtx);
	////dMassRotate (&m,Rtx);

	//dGeomSetPosition (m_geomID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	//dBodySetPosition (m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
	//dMassTranslate (&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);

	//dBodySetFiniteRotationMode(m_bodyID,1);//高速旋转时,有助于提高准确率
}

/////crMeshMatterObject
crMeshMatterObject::crMeshMatterObject(const crMeshMatterObject& meshmatterObject,const crCopyOp& copyop):
  crMatterObject(meshmatterObject,copyop)
{

}

void crMeshMatterObject::releaseOde()
{
	if(m_triData)
	{
		dGeomTriMeshDataDestroy(m_triData);
	    m_triData = NULL;
	}

	crMatterObject::releaseOde();
}

void crMeshMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	m_triData = CollectMeshInfoVisitor::createTriMeshData(this);

	if(m_triData)
	{
		m_geomID = dCreateTriMesh(spaceID, m_triData, 0, 0, 0);
	}
	else
	{
		m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	}

	if(!m_bStaticMatterObject)
	{
		dMass m;

		m_bodyID = dBodyCreate(worldID);
		//m_initPosition = m_bbox.center();
		dBodySetPosition(m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
		dMassSetBox (&m,1,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
		dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		dMassAdjust (&m,m_mass);
		dBodySetMass (m_bodyID,&m);
		dGeomSetBody (m_geomID,m_bodyID);
	}
}

void crMeshMatterObject::triMeshMatrixUpdata()
{
	const dReal *d = dGeomGetPosition(m_geomID);
	const dReal *r = dGeomGetRotation(m_geomID);

	//crMatrixd matrix;
	//matrix.set( r[0], r[4], r[8], 0,
	//	r[1], r[5], r[9], 0,
	//	r[2], r[6], r[10], 0,
	//	d[0], d[1], d[2], 1.0 );

	//dGeomTriMeshDataSet( m_triData,
	//	TRIMESH_LAST_TRANSFORMATION,
	//	(void *) matrix.ptr() );

	const dReal transform[16] = 
	{
		r[0], r[4], r[8],  0,
		r[1], r[5], r[9],  0,
		r[2], r[6], r[10], 0,
		d[0], d[1], d[2],  1
	};
    dGeomTriMeshSetLastTransform( m_geomID, *(dMatrix4*)(&transform) );
}

/////crRayMatterObject
//crRayMatterObject::crRayMatterObject():
//	m_length(200.0f)
//{
//	m_origin.set(0.0f,0.0f,0.0f);
//	m_direction.set(0.0f,1.0f,0.0f);
//	//m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,0.0f/*DegreesToRadians(0.0f)*/));
//	m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(10.0f)));
//	m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(-10.0f)));
//	m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(30.0f)));
//	m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(-30.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(50.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0.0f,0.0f,DegreesToRadians(-50.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(90.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(-90.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(130.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(-130.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(150.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(-150.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(170.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(-170.0f)));
//	//m_angleList.push_back(CRCore::crVector3f(0,0,DegreesToRadians(180.0f)));
//	m_currentAngleIndex = 0;
//}
//
//crRayMatterObject::crRayMatterObject(const crRayMatterObject& raymatterObject,const crCopyOp& copyop):
//    crMatterObject(raymatterObject,copyop),
//	m_length(raymatterObject.m_length),
//	m_hostMatterObject(NULL)
//{
//	m_origin.set(0.0f,0.0f,0.0f);
//	m_direction.set(0.0f,1.0f,0.0f);
//    //m_offset.set(0.0f,0.0f,-0.40f);//按人的一般高度设置
//	//m_angleList.clear(); 
//	//m_angleList.reserve(raymatterObject.m_angleList.size()); 
//	//std::copy(m_angleList.begin(),m_angleList.end(),std::back_inserter(raymatterObject.m_angleList));
//
//	m_currentAngleIndex = 0;
//}
//
//void crRayMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
//{
//	m_geomID = dCreateRay(spaceID,m_length);
//	dGeomRaySet(m_geomID, m_origin[0], m_origin[1], m_origin[2], m_direction[0], m_direction[1], m_direction[2]);
//}
//
//void crRayMatterObject::setHostMatterObject( crMatterObject *matterObject )
//{
//	m_hostMatterObject = matterObject;
//}
//
//void crRayMatterObject::setRayLength( float length )
//{
//	m_length = length;
//}
//
//float crRayMatterObject::getRayLength()
//{
//	return m_length;
//}
//
//void crRayMatterObject::setOrigin( const CRCore::crVector3f &origin )
//{
//    m_origin = origin;
//}
//
//const CRCore::crVector3f &crRayMatterObject::getOrigin()
//{
//    return m_origin;
//}
//
//void crRayMatterObject::setDirection( const CRCore::crVector3f &direction )
//{
//    m_direction = direction;
//}
//
//CRCore::crVector3f crRayMatterObject::getDirection()
//{
//	return m_direction;
//}
//
//void crRayMatterObject::updateDirection(  const CRCore::crVector3f &dir )
//{
//	m_direction = dir;
//	m_direction.normalize();
//
//	if(m_geomID)
//		dGeomRaySet(m_geomID, m_origin[0], m_origin[1], m_origin[2], m_direction[0], m_direction[1], m_direction[2]);
//}
//
//void crRayMatterObject::updateDirection( const CRCore::crMatrixf &mat )
//{
//	m_direction = m_direction * mat;
//	m_direction.normalize();
//
//	if(m_geomID)
//		dGeomRaySet(m_geomID, m_origin[0], m_origin[1], m_origin[2], m_direction[0], m_direction[1], m_direction[2]);
//}
//
//void crRayMatterObject::updateOrigin()
//{
//    //m_hostMatrix = m_hostMatterObject->getMatrix();
//
//	//const CRCore::crVector3f& lengths = m_hostMatterObject->getLengths();
//
//	///*float ey = lengths[1]/2.0f;*/
//	//float ez = lengths[2]*0.2f;
//
//	//m_hostep.set(0.0f,0.0f,ez);
// //   m_hostcenter.set(0.0,1.0f,ez);
// //   
//	//m_hostep = m_hostep * m_hostMatrix;
// //   m_hostcenter = m_hostcenter * m_hostMatrix;
//    m_origin = m_hostMatterObject->getBound().center();
//	//m_origin = m_hostMatrix.getTrans();
//	m_origin += m_offset;
//
//    if(m_geomID)
//	    dGeomRaySet(m_geomID, m_origin[0], m_origin[1], m_origin[2], m_direction[0], m_direction[1], m_direction[2]);
//}
//
//CRCore::crVector3f crRayMatterObject::getHostDirection()
//{
//	if(m_hostMatterObject)
//	{
//		//crVector3f dir =  m_hostcenter - m_hostep;
//  //      dir.normalize();
//	 //   return dir;
//		return -m_hostMatterObject->getDirection();//注：Cal3d默认前向量与系统前向量反相
//		                                           //系统默认为正Y轴，Cal3D为负Y轴
//	}
//	else
//		return CRCore::crVector3f(0.0f,-1.0f,0.0f);
//}
//
//void crRayMatterObject::updateRay()
//{
//	if(m_hostMatterObject)
//	{
//		//CRCore::crMatrixd matrix = m_hostMatterObject->getMatrix();
//
//		//CRCore::crVector3d up(0.0f,0.0f,1.0f);
//
//		//float ey;
//		//float ez;
//
//		//ey = m_hostMatterObject->getBoundBox().yLength();
//
//		//ez = m_hostMatterObject->getBoundBox().zLength()/2.0f;
//
//		//CRCore::crVector3d ep(0.0,ey,ez);
//		//CRCore::crVector3d center(0.0,ey+1.0f,ez);
//
//		if( m_currentAngleIndex == m_angleList.size() )
//		{
//			updateOrigin();
//			m_currentAngleIndex = 0;
//		}
//		//if( m_currentAngleIndex == m_angleList.size() )
//		//{
//		//	m_currentAngleIndex = 0;
//		//}
//
//		CRCore::crVector3f lv = getHostDirection();
//		/*if(m_currentAngleIndex!=0)*/
//		{
//		    CRCore::crMatrixd rot_matrix;
//		    CRCore::crVector3f rot_angle = m_angleList[m_currentAngleIndex];
//		    rot_matrix.makeRotate(rot_angle[0],X_AXIS,rot_angle[1],Y_AXIS,rot_angle[2],Z_AXIS);//rotAngle[1] = 0;
//
//		    lv = lv * rot_matrix;
//		}
//
//        updateDirection(lv);
//
//		m_currentAngleIndex++;
//	}
//}
//
//double crRayMatterObject::checkUpVector()
//{
//	if(m_hostMatterObject)
//	{
//		try
//		{
//            return m_hostMatterObject->checkUpVector();
//		}
//		catch (...) 
//		{
//			return 1;
//		}
//	}
//
//	return 1;
//}

//crTransformMatterObject::crTransformMatterObject(const crTransformMatterObject& matterObject,const crCopyOp& copyop):
//crMatterObject(matterObject,copyop)
//{
//
//}
//
//void crTransformMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
//{
//	//
//	//m_geomID = dCreateGeomTransform(spaceID);
//	//dGeomTransformSetCleanup();
//	//dMass m;
//	//m_bodyID = dBodyCreate(worldID);
//	////m_initPosition = m_bbox.center();
//	//dBodySetPosition(m_bodyID,m_initPosition[0],m_initPosition[1],m_initPosition[2]);
//	//dMassSetBox (&m,m_density,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
//	//dMassAdjust (&m,m_mass);
//	////m_barycenter = m_initPosition;
//	////dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
//	//dBodySetMass (m_bodyID,&m);
//
//	//m_geomID = dCreateBox(spaceID,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
//	//dGeomSetBody (m_geomID,m_bodyID);
//
//}

//dMass m;
//dMass m2;
//dMassSetZero (&m);
//
//dGeomID g2[2];
//crVector3 spherePos,boxPos;
//
//m_sphereGeomID = dCreateGeomTransform (spaceID);
//dGeomTransformSetCleanup (m_sphereGeomID,1);
//g2[0] = dCreateSphere (0,0.5f);
//dMassSetSphere (&m2,m_density,0.5f);
//dGeomTransformSetGeom (m_sphereGeomID,g2[0]);
//
//dGeomSetPosition (g2[0],spherePos[0],spherePos[1],spherePos[2]);
//dMassTranslate (&m,spherePos[0],spherePos[1],spherePos[2]);
//dMassAdd (&m,&m2);
//
//////////////
//m_geomID = dCreateGeomTransform (spaceID);
//dGeomTransformSetCleanup (m_geomID,1);
//g2[1] = dCreateBox(0,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
//dMassSetBox (&m2,m_density,m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
//dGeomTransformSetGeom (m_geomID,g2[1]);
//
//dGeomSetPosition (g2[1],boxPos[0],boxPos[1],boxPos[2]);
//dMassTranslate (&m,boxPos[0],boxPos[1],boxPos[2]);
//dMassAdd (&m,&m2);
//
//dGeomSetPosition (g2[0],spherePos[0]-m.c[0],spherePos[1]-m.c[1],spherePos[2]-m.c[2]);
//dGeomSetPosition (g2[1],boxPos[0]-m.c[0],boxPos[1]-m.c[1],boxPos[2]-m.c[2]);
//dMassTranslate (&m,-m.c[0],-m.c[1],-m.c[2]);
//
//m_bodyID = dBodyCreate(worldID);
//dGeomSetBody (m_sphereGeomID,m_bodyID);
//dGeomSetBody (m_geomID,m_bodyID);
//
//dBodySetMass (m_bodyID,&m);
