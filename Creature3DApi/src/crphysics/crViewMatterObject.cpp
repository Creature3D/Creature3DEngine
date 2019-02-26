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
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crGameBodyInfo.h>
//#include <CRPhysics/crBotManager.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crDrawPixels.h>
#include <CRCore/crDOFTransform.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>

using namespace CRCore;
using namespace CRPhysics;

crViewMatterObject::crViewMatterObject():
  m_viewPitch(0.0f),
  m_currentViewPitch(0.0f),
  m_viewRoll(0.0f),
  m_viewOffset(0.0f),
  m_viewHeight(1.7f),
  m_hideBody(false),
  m_groundDetect(false),
  m_deadFlag(false),
  m_requireRenascence(false),
  m_targetCount(0),
  m_collided(false),
  m_isNetBot(false),
  m_canFalldown(true),
  m_lockViewPitch(false),
  m_lockViewRoll(false),
  m_physicsBox(0.3f,0.3f,1.7f),
  m_walkableHeight(0.5f),
  m_viewVelocity(0.0f),
  m_currentViewOffset(0.0f),
  m_cameraSwaped(false),
  m_viewShakeRange(0.1f),
  m_viewShakeInterval(0.0f),
  m_viewOffsetVelocity(10.0f),
  m_viewPitchVelocity(0.5f),
  m_lockMatrix(false),
  m_targetHeight(0.0f),
  m_heightAdjustVelocity(0.0f),
  m_heightAdjustTime(0.0f),
  m_adjustViewRoll(false),
  m_targetViewRoll(0.0f),
  m_viewRollAdjustVelocity(0.0f),
  m_viewRollAdjustTime(0.0f),
  m_adjustViewPitch(false),
  m_targetViewPitch(0.0f),
  m_viewPitchAdjustVelocity(0.0f),
  m_viewPitchAdjustTime(0.0f),
  m_adjustViewHeight(false),
  m_targetViewHeight(0.0f),
  m_viewHeightAdjustVelocity(0.0f),
  m_viewHeightAdjustTime(0.0f),
  m_adjustViewOffset(false),
  m_targetViewOffset(0.0f),
  m_viewOffsetAdjustVelocity(0.0f),
  m_viewOffsetAdjustTime(0.0f),
  m_lockViewOffset(false),
  m_slopeX(0.0f),
  m_slopeY(0.0f),
  m_freeCamera(true),
  m_cameraPithDelta(0.0f)
{
	m_viewOffsetRange.set(0.45,20);
	m_viewPitchRange.set(-PI_2,PI_2);
}

crViewMatterObject::crViewMatterObject(const crViewMatterObject& matterObject,const crCopyOp& copyop):
  crMatterObject(matterObject,copyop)
{
	//不能克隆。
}

crViewMatterObject::~crViewMatterObject()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crViewMatterObject() "<<getName()<< std::endl;
}

void crViewMatterObject::setPhysicsBox(const CRCore::crVector3 &boxlength)
{
    m_physicsBox = boxlength;
}

void crViewMatterObject::setWalkableHeight(float height)
{
	m_walkableHeight = height;
}

void crViewMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	BotPhysicsObject *botPhysicsObject;
	CRCore::crVector3 pos;
	for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
		itr != m_botPhysicsObjectVec.end();
		++itr )
	{
		botPhysicsObject = itr->get();
        const crBoundingBox &bbox = botPhysicsObject->m_node->getBoundBox();
        botPhysicsObject->m_geomid = dCreateBox(spaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
		pos = botPhysicsObject->m_node->getMatrix().getTrans();
		dGeomSetPosition(botPhysicsObject->m_geomid,pos[0],pos[1],pos[2]);

		//crOdeWorld::getDynamicGeomMap()[botPhysicsObject->m_geomid] = this;
		crOdeWorld::dynamicGeoMapInsert(botPhysicsObject->m_geomid,this);
	}
#if 0
	dMass m;
	m_bodyID = dBodyCreate(worldID);
	//m_physicsBox.set(m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength());
	dMassSetBox (&m,m_density,m_physicsBox[0],m_physicsBox[1],m_physicsBox[2]);
	dMassAdjust (&m,m_mass);
	dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
	dBodySetMass (m_bodyID,&m);
#endif
	//if(!m_bbox.valid())
	//{
	//	CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::create error 物理对象 "<<this->getName()<<" 的包围盒未设定"<<std::endl;
	//	return;
	//}
	m_geomID = dCreateBox(spaceID,m_physicsBox[0],m_physicsBox[1],m_physicsBox[2]);
	if(m_bodyID) dGeomSetBody (m_geomID,m_bodyID);

	/*float r = getBound().radius();
	////CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::_create()  "<<r<<std::endl;
	m_geomID = dCreateSphere(spaceID,r);
	dMass m;
	m_bodyID = dBodyCreate(worldID);
	dMassSetSphere (&m,m_density,r);
	dMassAdjust (&m,m_mass);
	dMassTranslate(&m,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
	dBodySetMass (m_bodyID,&m);
	dGeomSetBody (m_geomID,m_bodyID);
*/
	pos = m_matrix.getTrans();
	m_lastSecurePos = pos;
	m_lastPos = pos;
	m_initPosition = pos;
	dGeomSetPosition(m_geomID,pos[0],pos[1],pos[2]);
	if(m_bodyID) dBodySetPosition (m_bodyID,pos[0],pos[1],pos[2]);

	//dBodySetGravityMode(m_bodyID,m_groundDetect?1:0);
	setGravityMode(m_groundDetect?1:0);
	if(m_bodyID) dBodyDisable(m_bodyID);
	m_currentBodyPhysicsType = RayDetect;
	//resetPhysicsSwap();
    //CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::_create CreateBox = "<<crVector3(m_bbox.xLength(),m_bbox.yLength(),m_bbox.zLength())<<std::endl;
	if(m_radarMatterObject.valid())
	    m_radarMatterObject->setBodySpaceID(spaceID);
	m_radarPos  = pos;
	m_futureMatrix = m_matrix;
}

void crViewMatterObject::collided()
{
	if(m_groundDetect)
		m_collided = true;
}

void crViewMatterObject::updateCreMatrix()
{
	if(m_lockMatrix) return;
	crVector3f trans = m_matrix_buf.getTrans();
	if(m_collided) 
	{
		if(trans == m_lastPos)
		{
			m_matrix_buf.setTrans(m_lastSecurePos);
			m_lastPos = m_lastSecurePos;
		}
		else
		{
			m_matrix_buf.setTrans(m_lastPos);
		}
		if(m_groundDetect && m_radarMatterObject.valid())
		{
			float height = m_radarMatterObject->getGndDetectHeight()/* - getBoundBox().zLength() * 0.5f*/;
            if(height<0.0f)
			{//上台阶
                m_lastPos[2] += 0.05f;
			}
		}
	}
	else 
	{
		if(m_geomID && m_currentBodyPhysicsType == RealPhysics)
		{
			const dReal *d = dGeomGetPosition(m_geomID);

			m_lastPos = trans;

			//m_futureMatrix.setTrans(d[0],d[1],d[2]);
			m_matrix_buf.setTrans(d[0],d[1],d[2]);
			setVelocity2(0.0f);
			setAcceleration2(0.0f);
			//const dReal *r = dGeomGetRotation(m_geomID);

			//m_futureMatrix.set( r[0], r[4], r[8], 0,
			//	r[1], r[5], r[9], 0,
			//	r[2], r[6], r[10], 0,
			//	d[0]/*-m_initPosition[0]*/, d[1]/*-m_initPosition[1]*/, d[2]/*-m_initPosition[2]*/, 1.0 );
			//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::updateCreMatrix() RealPhysics "<<m_matrix_buf.getTrans()<< std::endl;
			//dirtyBound();
		}
		else
		{
			if(m_groundDetect && m_radarMatterObject.valid())
			{
				float height = m_radarMatterObject->getGndDetectHeight()/* - getBoundBox().zLength() * 0.5f*/;
				if(height>m_walkableHeight)
				{//下落
					if(m_canFalldown)
						setAcceleration2(-9.81f);//enablePhysics(CRCore::minimum((float)(m_frameInterval * 5.0f),0.5f));
					else 
						m_matrix_buf.setTrans(m_lastPos);

					//m_matrix_buf.setTrans(m_matrix_buf.getTrans() + crVector3(0.0f,0.0f,-height));
					//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::updateOdeMatrix() height enablePhysics "<<height<< std::endl;
				}
				else if(height>0.1f)
				{//台阶
					//m_radarMatterObject->getBotPosition(trans);
					//m_matrix_buf.setTrans(trans + crVector3(0.0f,0.0f,-height));
                    setVelocity2(-5.0f);
					setAcceleration2(0.0f);
					//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::updateOdeMatrix() height enablePhysics "<<height<< std::endl;
				}
				else if(height>=-0.1f)
				{
					trans[2]-=height;
					m_lastPos = trans;
					m_radarMatterObject->getBotPosition(m_lastSecurePos);
					setVelocity2(0.0f);
					setAcceleration2(0.0f);
					m_matrix_buf.setTrans(trans);
				}
				else if(height>=-m_walkableHeight && height<-0.1f)
				{//上台阶
					//m_radarMatterObject->getBotPosition(trans);
					//m_matrix_buf.setTrans(trans + crVector3(0.0f,0.0f,-height));
                    setVelocity2(5.0f);
					setAcceleration2(0.0f);
				}
				//else if(height>-0.5f)
				//{//可跳跃通过，放在AI判断是否跳跃
				//}
				else if(height<-m_walkableHeight)
				{//can't pass
					m_matrix_buf.setTrans(m_lastPos);
					setVelocity2(0.0f);
					setAcceleration2(0.0f);
					//m_futureMatrix.setTrans(m_lastSecurePos);
					//m_matrix.setTrans(m_lastSecurePos);
					//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::updateOdeMatrix() height collided "<<height<< std::endl;
				}
			}
			m_lastPos = m_matrix_buf.getTrans();//test
		}
	}
}
void crViewMatterObject::setDummyMatrix()
{
	if(m_bodyID) dBodySetGravityMode(m_bodyID,m_gravityMode);

	//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::updateOdeMatrix() name = "<<getName()<< std::endl;
	crMatrix dummyMatrix;
	unsigned char matrixType;
	CRCore::crVector3 pos;
	getMatterObjectMatrix(dummyMatrix,matrixType);
	switch (matrixType)
	{
	case MD_RotMatrix:
		dummyMatrix.setTrans(m_futureMatrix.getTrans());
		m_futureMatrix = dummyMatrix;
		break;
	case MD_TransMatrix:
		pos = dummyMatrix.getTrans();
		m_targetHeight = pos[2];
		m_heightAdjustTime = 0.1f;
		m_heightAdjustVelocity = (m_targetHeight - m_lastPos[2])/m_heightAdjustTime;
		m_lastPos[0] = pos[0];
		m_lastPos[1] = pos[1];
		m_futureMatrix.setTrans(m_lastPos);
		//m_lastPos = m_futureMatrix.getTrans();
		m_lastSecurePos = m_lastPos;
		break;
	case MD_FullMatrix:
		m_futureMatrix = dummyMatrix;
		//m_lastPos = m_futureMatrix.getTrans();
		//m_lastSecurePos = m_lastPos;
		pos = dummyMatrix.getTrans();
		m_targetHeight = pos[2];
		m_heightAdjustTime = 0.1f;
		m_heightAdjustVelocity = (m_targetHeight - m_lastPos[2])/m_heightAdjustTime;
		m_lastPos[0] = pos[0];
		m_lastPos[1] = pos[1];
		m_futureMatrix.setTrans(m_lastPos);
		//m_lastPos = m_futureMatrix.getTrans();
		m_lastSecurePos = m_lastPos;
		break;
	}
	if(m_heightAdjustTime>0.0f)
	{
		m_heightAdjustTime -= m_frameInterval;
		if(m_heightAdjustTime>0.0f)
			m_lastPos[2] += m_heightAdjustVelocity * m_frameInterval;
		else
			m_lastPos[2] = m_targetHeight;
		m_futureMatrix.setTrans(m_lastPos);
	}
	else
	{
		m_heightAdjustVelocity = 0.0f;
	}
}
bool crViewMatterObject::computeBound() const
{
	crVector3 center = getPhysicsCenter();
	crVector3 halfBox = m_physicsBox * 0.5f;
	m_bbox.set(center-halfBox,center+halfBox);
	m_boundSphere.init();
	m_boundSphere.expandBy(m_bbox);

	m_bBoundSphere_computed = true;
	return true;
}
void crViewMatterObject::updateOdeMatrix()
{
	setDummyMatrix();
	CRCore::crVector3 pos = m_futureMatrix.getTrans();
	dMatrix3 r;
	if(m_geomID)
	{
		///////////////////////
		dGeomSetPosition (m_geomID,pos[0],pos[1],pos[2]);

		crOdeWorld::creatureToOdeMat(m_futureMatrix,r);
		dGeomSetRotation(m_geomID,r);
		if(m_bodyID)
		{
			dBodySetRotation(m_bodyID,r);
			dBodySetPosition (m_bodyID,pos[0],pos[1],pos[2]);
		}
	}

	BotPhysicsObject *botPhysicsObject;
	for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
		itr != m_botPhysicsObjectVec.end();
		++itr )
	{
		botPhysicsObject = itr->get();
		if(botPhysicsObject->m_geomid)
		{
			const crMatrix& mat = botPhysicsObject->m_node->getMatrix();
			pos = mat.getTrans();
			dGeomSetPosition(botPhysicsObject->m_geomid,pos[0],pos[1],pos[2]);

			crOdeWorld::creatureToOdeMat(mat,r);
			dGeomSetRotation(m_geomID,r);
		}
	}

	m_matrix_buf = m_futureMatrix;

	m_collided = false;
}

//bool crViewMatterObject::physicsSwap()
//{
//	if(!m_enableIntersect || !m_geomID) return false;
//
//	if(m_currentBodyPhysicsType==RayDetect)
//	{
//		m_currentBodyPhysicsType = RealPhysics;
//		//dGeomEnable(m_geomID);
//		dBodyEnable(m_bodyID);
//	}
//	else
//	{
//		m_currentBodyPhysicsType = RayDetect;
//		//dGeomDisable(m_geomID);
//		dBodyDisable(m_bodyID);
//	}
//
//	return true;
//}
//
//void crViewMatterObject::resetPhysicsSwap()
//{
//	if(!m_enableIntersect || !m_geomID) return;
//
//	m_currentBodyPhysicsType = RayDetect;
//	dBodyDisable(m_bodyID);
//	m_physicsSwapInterval = 0;
//}
//
//void crViewMatterObject::setLinearVel(const CRCore::crVector3f& vel, float interval)
//{
//	if(m_currentBodyPhysicsType != RealPhysics)
//	{
//		if(!physicsSwap()) 
//			return;
//		else
//			m_physicsSwapInterval = interval;
//	}
//	else
//		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);
//
//	dBodySetLinearVel(m_bodyID,vel[0],vel[1],vel[2]);
//}
//
//void crViewMatterObject::addForce(const CRCore::crVector3f& force, float interval)
//{
//	if(m_currentBodyPhysicsType != RealPhysics)
//	{
//		if(!physicsSwap()) 
//			return;
//		else
//			m_physicsSwapInterval = interval;
//	}
//	else
//		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);
//
//	dBodyAddForce(m_bodyID,force[0],force[1],force[2]);
//}
//
//void crViewMatterObject::enablePhysics(float interval)
//{
//	if(m_currentBodyPhysicsType != RealPhysics)
//	{
//		if(!physicsSwap()) 
//			return;
//		else
//			m_physicsSwapInterval = interval;
//	}
//	else
//		m_physicsSwapInterval = maximum(m_physicsSwapInterval,interval);
//}

void crViewMatterObject::releaseObjects(CRCore::crState* state)
{
	if(m_currentWeaponMatterObject.valid())
	{
		m_currentWeaponMatterObject->releaseObjects(state);
	}
	for( WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.begin();
		 itr != m_weaponMatterObjects.end();
		 ++itr )
	{
		if(itr->second != m_currentWeaponMatterObject)
			itr->second->releaseObjects(state);
	}
    m_gameBodyInfo = 0;
	crMatterObject::releaseObjects(state);
	if(m_radarMatterObject.valid())
	{
		(dynamic_cast<crMatterGroup *>(m_radarMatterObject->getParent(0)))->attachEventGroup(0);
		m_radarMatterObject = 0;
	}
}

void crViewMatterObject::releaseOde()
{
	if(m_currentWeaponMatterObject.valid())
	{
		m_currentWeaponMatterObject->releaseOde();
	}
	for( WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.begin();
		itr != m_weaponMatterObjects.end();
		++itr )
	{
		if(itr->second != m_currentWeaponMatterObject)
			itr->second->releaseOde();
	}
	BotPhysicsObject *botPhysicsObject;
	for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
		itr != m_botPhysicsObjectVec.end();
		++itr )
	{
		botPhysicsObject = itr->get();
		if(botPhysicsObject->m_geomid)
		{
			dGeomDestroy(botPhysicsObject->m_geomid);
			crOdeWorld::dynamicGeoMapErase(botPhysicsObject->m_geomid);
			botPhysicsObject->m_geomid = NULL;
		}
	}
	crMatterObject::releaseOde();
}

void crViewMatterObject::setGameBodyInfo(crGameBodyInfo *info)
{
    m_gameBodyInfo = info;
}

void crViewMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			crMatterObject::traverse(nv);
			BotPhysicsObject *botPhysicsObject;
			for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
				itr != m_botPhysicsObjectVec.end();
				++itr )
			{
				botPhysicsObject = itr->get();
				if(botPhysicsObject->m_geomid && (int)m_enableIntersect^dGeomIsEnabled(botPhysicsObject->m_geomid))
				{
					if(m_enableIntersect)
						dGeomEnable(botPhysicsObject->m_geomid);
					else
						dGeomDisable(botPhysicsObject->m_geomid);
				}
			}

			//double referenceTime = nv.getFrameStamp()->getReferenceTime();
			//m_frameInterval = (m_previousFrameTime != 0.0) ? (referenceTime-m_previousFrameTime) : 0.0;
			//m_frameInterval = CRCore::minimum(m_frameInterval,0.1);
			//m_previousFrameTime = referenceTime;

			//if(m_physicsSwapInterval>0.0f)
			//	m_physicsSwapInterval -= m_frameInterval;
			//else if(m_physicsSwapInterval < 0.0f)
			//{
			//	physicsSwap();
			//	m_physicsSwapInterval = 0.0f;
			//	//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::traverse(): physicsSwap to RayDetect = "<<(m_currentBodyPhysicsType==RayDetect)<<std::endl;
			//}

			//if(!getEnableIntersect() || isDead()) return;

			//m_lastPos = m_matrix.getTrans();
            //m_futureMatrix = m_matrix;
			if(m_cameraSwaped)
				m_cameraSwaped = false;
			else
			    swapToFutureMatrix();

			//m_radarPos = m_matrix.getTrans();
			
			//if(getVisiable())
			//{
			//	//CRCore::notify(CRCore::ALWAYS)<<nv.getFrameStamp()->getFrameNumber()<<"crViewMatterObject::traverse(): m_position = "<<m_matrix.getTrans()<<std::endl;
				doHPR();
				move();
				doAdjust();
				if(m_slopeX!=0.0f)
				{
					m_matrix.preMult(CRCore::crMatrix::rotate(m_slopeX, CRCore::X_AXIS));
				}
				if(m_slopeY!=0.0f)
				{
					m_matrix.preMult(CRCore::crMatrix::rotate(m_slopeY, CRCore::Y_AXIS));
				}
				//updateWeaponMatrix();

			//	//getGameBodyInfo()->updateLockedTarget(getTargetCount());
			//	//if(!m_groundDetect)
			//	//{
			//	//	if(m_velocity[0] <= getGameBodyInfo()->getStallVelocity())
			//	//	{
			//	//		doEvent(WCH_STALL);
			//	//	}
			//	//	else
			//	//	{
			//	//		doEvent(WCH_UNSTALL);
			//	//	}
			//	//}

			//	if(/*!m_isNetBot && */m_radarMatterObject.valid())
			//	{
			//		crVector3 botPos = m_matrix.getTrans();
			//		crVector3 dir = -getViewDirection() + m_gameBodyInfo->getTargetOffset();
			//		dir.normalize();
			//		m_radarMatterObject->updatePositionAndDirection(m_radarPos,botPos,dir);
			//		//float z = botPos.z();
			//		//m_radarMatterObject->setGndOffset(m_radarPos.z() - z + getBoundBox().zLength() * 0.5f);
			//		m_radarMatterObject->setGndOffset(/*getBoundBox().zLength()*/m_physicsBox[2] * 0.5f);
			//	}

			//	//if(m_bodySwitch.valid())
			//	//{
			//	//	crGroup *currentActiveNode = dynamic_cast<crGroup *>(m_bodySwitch->getFirstActiveChild());
			//	//	if(currentActiveNode&&strcmp(currentActiveNode->className(),"CreBodyNode")==0)
			//	//	{
			//	//		currentActiveNode->updateCharacterLod((nv.getEyePoint() - getBound().center()).length2());
			//	//	}
			//	//}
   //             
			//	//crVolumeNodeManager::getInstance()->testVolume(this);
			//	//doEvent(WCH_UPDATE);
			//}

			//BotPhysicsObject *botPhysicsObject;
			//for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
			//	itr != m_botPhysicsObjectVec.end();
			//	++itr )
			//{
			//	botPhysicsObject = itr->get();
			//	if(botPhysicsObject->m_geomid && (int)m_enableIntersect^dGeomIsEnabled(botPhysicsObject->m_geomid))
			//	{
			//		if(m_enableIntersect)
			//			dGeomEnable(botPhysicsObject->m_geomid);
			//		else
			//			dGeomDisable(botPhysicsObject->m_geomid);
			//	}
			//}

			//if(m_bodyID)
			//{
			//	if(m_enableIntersect && m_currentBodyPhysicsType==RealPhysics)
			//	{
			//		dBodyEnable(m_bodyID);
			//	}
			//	else
			//	{
			//		dBodyDisable(m_bodyID);
			//	}
			//}
			//if(m_geomID)
			//{
			//	if((int)m_enableIntersect^dGeomIsEnabled(m_geomID))
			//	{
			//		if(m_enableIntersect)
			//			dGeomEnable(m_geomID);
			//		else
			//			dGeomDisable(m_geomID);
			//	}
			//}
		}
	    //crMatrixTransform::traverse(nv);
		return;
	case CRCore::crNodeVisitor::CULL_VISITOR:
		//if(m_gameBodyInfo.valid())
		//	m_gameBodyInfo->clearDetectMap();
         break;
	case CRCore::crNodeVisitor::INIT_VISITOR:
		{
			m_currentViewOffset = m_viewOffset;
			m_currentViewPitch = m_viewPitch;
			//if(nv.getTraverseString().compare("RenderToTexture") == 0)
			//{
			//	return;
			//}
			m_futureMatrix = m_matrix;
			m_targetHeight = m_futureMatrix.getTrans()[2];
			if(m_radarMatterObject.valid())
				(dynamic_cast<crMatterGroup *>(m_radarMatterObject->getParent(0)))->attachEventGroup((dynamic_cast<crMatterGroup *>(this->getParent(0)))->getAttachedEventGroup());
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			crNode *node;
			for( WeaponRackNameMap::iterator itr = m_weaponRackNameMap.begin();
				itr != m_weaponRackNameMap.end();
				++itr )
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(itr->second);
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				accept(searchByNameVisitor);
				node = searchByNameVisitor.getResult();
				if(node)
				{
					m_weaponRackMap.insert(std::make_pair(itr->first,node));
				}
			}

			//searchByNameVisitor.reset();
			//searchByNameVisitor.setNameId("CameraRack");
			//searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::GROUP);
			//accept(searchByNameVisitor);
			//m_cameraSwitch = dynamic_cast<CRCore::crMultiSwitch *>(searchByNameVisitor.getResult());

			//searchByNameVisitor.reset();
			//searchByNameVisitor.setNameId("BodySwitch");
			//searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::MULTISWITCH);
			//accept(searchByNameVisitor);
			//m_bodySwitch = dynamic_cast<CRCore::crMultiSwitch *>(searchByNameVisitor.getResult());
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("BodySwitch");
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			accept(searchByNameVisitor);
			m_body = dynamic_cast<CRCore::crGroup *>(searchByNameVisitor.getResult());

			//用于物理碰撞检测
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("BotPhysicsObject");
			searchByNameVisitor.setSearchNodeType(GROUP);
			accept(searchByNameVisitor);
			crGroup *botPhysicsObject = dynamic_cast<CRCore::crGroup *>(searchByNameVisitor.getResult());
			BotPhysicsObject *physicsObject;
			if(botPhysicsObject)
			{
				const CRCore::NodeArray &childArray = botPhysicsObject->getChildArray();
				for( CRCore::NodeArray::const_iterator itr = childArray.begin();
					 itr != childArray.end();
					 ++itr )
				{
					physicsObject = new BotPhysicsObject;
					physicsObject->m_node = dynamic_cast<const CRCore::crMatrixTransform *>(itr->get());
					if(!physicsObject->m_node.valid())
					{
						CRCore::notify(CRCore::NOTICE)<<"crViewMatterObject::INIT_VISITOR BotPhysicsObject的子节点必需为crMatrixTransform类型"<<std::endl;
					}
					else
					{
						CRCore::crArgumentParser::readKeyValue((*itr)->getDescriptions(),"Executionrate",physicsObject->m_executionrate);
						m_botPhysicsObjectVec.push_back(physicsObject);
					}
				}
			}

			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId("RackRoot");
			searchByNameVisitor.setSearchNodeType(GROUP);
			accept(searchByNameVisitor);
			crGroup *rackRoot = dynamic_cast<CRCore::crGroup *>(searchByNameVisitor.getResult());
			crGroup *rackNode;
			if(rackRoot)
			{
				CRCore::NodeArray &childArray = rackRoot->getChildArray();
				for( CRCore::NodeArray::iterator itr = childArray.begin();
					itr != childArray.end();
					++itr )
				{
					rackNode = dynamic_cast<crGroup *>(itr->get());
					if(rackNode)
					{
						m_rackMap[rackNode->getName()] = rackNode;
					}
				}
			}
		}

		break;
	}

	CRPhysics::crMatterObject::traverse(nv);
}

CRCore::crGroup *crViewMatterObject::getRackNode(const std::string &rackName)
{
	RackMap::iterator itr = m_rackMap.find(rackName); 
	return itr == m_rackMap.end()? NULL : itr->second.get();
}

crViewMatterObject::BotPhysicsObject *crViewMatterObject::getBotPhysicsObject(void *geomid)
{
	for( BotPhysicsObjectVec::iterator itr = m_botPhysicsObjectVec.begin();
		 itr != m_botPhysicsObjectVec.end();
		 ++itr )
	{
		if((*(itr->get())) == (dGeomID)geomid)
			return itr->get();
	}
	return NULL;
}

void crViewMatterObject::insertWeaponRackName(const std::string &rackName, const std::string &nodeName)
{
	//CRCore::notify(CRCore::ALWAYS)<<"insertWeaponRackName "<<rackName<<" "<<nodeName<<std::endl;
     m_weaponRackNameMap.insert(std::make_pair(rackName,nodeName));
}

unsigned int crViewMatterObject::getCurrentWeaponMask()
{ 
	return m_currentWeaponMatterObject.valid()?m_currentWeaponMatterObject->getWeaponObject()->getAttackMask():0;
}

//void crViewMatterObject::updateWeaponMatrix()
//{
//	//CRCore::notify(CRCore::ALWAYS)<<"updateWeaponMatrix "<<m_weaponRackNameMap.size()<<" "<<m_weaponRackMap.size()<<std::endl;
//	m_targetCount = 0;
//	if(m_currentWeaponMatterObject.valid())
//	{
//		const std::string &rackName = m_currentWeaponMatterObject->getWeaponRackName();
//		WeaponRackMap::iterator itr = m_weaponRackMap.find(rackName);
//		crMatrixTransform *matrixTransform;
//		crMatrix weaponMatrix1,weaponMatrix2;
//		crVector3 weaponPos,newPos;
//
//		crMatrix bodyMatrix1 = m_matrix;
//		crMatrix bodyMatrix2 = m_futureMatrix;
//
//		m_currentWeaponMatterObject->clearWeaponMatrix();
//		bool added = false;
//		int needViewPitch;
//		for( ; itr != m_weaponRackMap.end() && itr->first.compare(rackName) == 0;
//		     ++itr )
//		{
//			matrixTransform = dynamic_cast<crMatrixTransform *>(itr->second.get());
//			if(matrixTransform)
//			{
//				//const crMatrix& matrix = matrixTransform->getMatrix();
//				weaponMatrix1 =  matrixTransform->getMatrix() * bodyMatrix1;
//				weaponMatrix2 =  matrixTransform->getMatrix() * bodyMatrix2;
//
//				//position = itr->second->getBound().center();
//				//position = matrix.getTrans();
//				//position = position * bodyMatrix;
//				//CRCore::notify(CRCore::ALWAYS)<<"bodyMatrix = "<<bodyMatrix.getTrans()<<std::endl;
//				//CRCore::notify(CRCore::ALWAYS)<<"matrix = "<<matrix.getTrans()<<std::endl;
//				//weaponMatrix.setTrans(position);
//			}
//			else
//			{
//                weaponPos = itr->second->getBound().center();
//				newPos = weaponPos * bodyMatrix1;//m_futureMatrix;
//				weaponMatrix1 = bodyMatrix1;//m_futureMatrix;
//				weaponMatrix1.setTrans(newPos);
//
//				newPos = weaponPos * bodyMatrix2;//m_futureMatrix;
//				weaponMatrix2 = bodyMatrix2;//m_futureMatrix;
//				weaponMatrix2.setTrans(newPos);
//			}
//
//			if(m_lockViewPitch)
//			{
//				needViewPitch = 0;
//				CRCore::crArgumentParser::readKeyValue(itr->second->getDescriptions(),"NeedViewPitch",needViewPitch);
//				if(needViewPitch && m_viewPitch!=0.0f)
//				{
//					weaponMatrix1 = crMatrixd::rotate(m_viewPitch,X_AXIS) * weaponMatrix1;
//					weaponMatrix2 = crMatrixd::rotate(m_viewPitch,X_AXIS) * weaponMatrix2;
//				}
//			}
//
//			added = true;
//			m_currentWeaponMatterObject->addWeaponMatrix(weaponMatrix1,weaponMatrix2);
//			m_targetCount++;
//		}
//		if(!added)
//		{
//			m_currentWeaponMatterObject->addWeaponMatrix(bodyMatrix1,bodyMatrix2);
//			m_targetCount++;
//		}
//	    //m_currentWeaponMatterObject->setMatrix(m_matrix);
//	}
//}
//
void crViewMatterObject::doEvent(_crInt64 kbmsg, CREPARAM param)
{			
	crMatterObject::doEvent(kbmsg,param);
}

void crViewMatterObject::setGroundDetect(bool groundDetect) 
{ 
	m_groundDetect = groundDetect;
	setGravityMode(m_groundDetect?1:0);
	if(m_radarMatterObject.valid())
		m_radarMatterObject->setGroundDetect(m_groundDetect);
	//if(m_bodyID) dBodySetGravityMode(m_bodyID,m_groundDetect?1:0);
}

void crViewMatterObject::rotate(const CRCore::crVector2f &angle)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::rotate angle= "<<angle<< std::endl;
	//crMatrix mat = m_futureMatrix;
	//mat.preMult(CRCore::crMatrix::rotate( angle[0], Z_AXIS ));
	//setMatterObjectMatrix(mat);
	
	crGameBodyInfo::ManipulatorMode manipulatorMode = m_gameBodyInfo->getManipulatorMode();
	if(manipulatorMode == crGameBodyInfo::MM_FPS)
	{
        m_rotateAngles[0] = angle[0];
	}
	else
	{
        updateViewRoll(angle[0]);
	}
	updateViewPitch(angle[1]);
}

void crViewMatterObject::rotate(const CRCore::crVector3f &angle)
{
	m_rotateAngles = angle;
}

void crViewMatterObject::setMaxSpeed(const CRCore::crVector3f& speed)
{
    m_maxSpeed = speed;
}

void crViewMatterObject::setMinSpeed(const CRCore::crVector3f& speed)
{
    m_minSpeed = speed;
}

void crViewMatterObject::setVelocity(const crVector3f& speed/*, int flg*/) 
{
    m_velocity = speed;
	//switch(flg) 
	//{
	//case 1:
	//	m_velocity[0] = speed[0];
	//	break;
	//case 2:
	//	m_velocity[0] = -speed[0];
	//	break;
	//case 3:
	//	m_velocity[1] = speed[1];
	//	break;
	//case 4:
	//	m_velocity[1] = -speed[1];
	//	break;
	//case 5:
	//	m_velocity[0] = 0.0f;
	//	break;
	//case 6:
	//	m_velocity[1] = 0.0f;
	//	break;
	//case 7:
	//	m_velocity.set(0.0f,0.0f);
	//	break;
	//}
}
void crViewMatterObject::setVelocity0(float vx)
{
	m_velocity[0] = vx;
}
void crViewMatterObject::setVelocity1(float vy)
{
	m_velocity[1] = vy;
}
void crViewMatterObject::setVelocity2(float vz)
{
	m_velocity[2] = vz;
}
void crViewMatterObject::setAcceleration(const CRCore::crVector3f& acceleration)
{
	m_acceleration = acceleration;
}
void crViewMatterObject::setAcceleration0(float ax)
{
	m_acceleration[0] = ax;
}
void crViewMatterObject::setAcceleration1(float ay)
{
	m_acceleration[1] = ay;
}
void crViewMatterObject::setAcceleration2(float az)
{
	m_acceleration[2] = az;
}

void crViewMatterObject::swapToFutureMatrix()
{
    m_matrix = m_futureMatrix;
	//updateWeaponMatrix();
    dirtyBound();
	//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::swapToFutureMatrix(): m_position = "<<m_matrix.getTrans()<<std::endl;
}

void crViewMatterObject::move()
{
	if(m_acceleration[0] != 0.0f) m_velocity[0] += m_acceleration[0] * m_frameInterval;
	if(m_acceleration[1] != 0.0f) m_velocity[1] += m_acceleration[1] * m_frameInterval;
	if(m_acceleration[2] != 0.0f) m_velocity[2] += m_acceleration[2] * m_frameInterval;
	//m_acceleration.set(0.0f,0.0f);
//	if(!m_cameraBind) CRCore::notify(CRCore::FATAL)<<" crViewMatterObject::move(): m_velocity = "<<m_velocity<<" id = "<<this->getName()<<std::endl;
	//if(!m_cameraBind) m_velocity[0] = 6.0f;
    if(m_velocity[2] != 0.0f)
	{
		//m_velocity[2] = CRCore::clampTo(m_velocity[2],m_minSpeed[2],m_maxSpeed[2]);
		crVector3 pos = m_futureMatrix.getTrans();
		pos[2] += m_velocity[2] * m_frameInterval;
        m_futureMatrix.setTrans(pos);
	}
	if(m_velocity[0] == 0.0f && m_velocity[1] == 0.0f) 
	{
		m_transtest.set(0,0);
	}
	else
	{
		m_velocity[0] = CRCore::clampTo(m_velocity[0],m_minSpeed[0],m_maxSpeed[0]);
		m_velocity[1] = CRCore::clampTo(m_velocity[1],m_minSpeed[1],m_maxSpeed[1]);

		crVector3f dir,dir2;
		crVector3f trans;

		if(m_groundDetect)
			dir = -getDirection();
		else
			dir = -getViewDirection();
		dir2 = getDirectionX();

		if(m_velocity[0] != 0.0f)
		{
			trans = dir * m_velocity[0] * m_frameInterval;
		}
		if(m_velocity[1] != 0.0f)
		{
			trans = dir2 * m_velocity[1] * m_frameInterval;
		}

		if(m_collided)
		{
			if(m_transtest[0] == 0)
			{//test x
				m_transtest[0] = 1;
				m_transtest[1] = 0;
				trans[1] = 0.0f;
				trans[0] *= 0.5f;
			}
			else
			{//test y
				m_transtest[0] = 0;
				m_transtest[1] = 1;
				trans[0] = 0.0f;
				trans[1] *= 0.5f;
			}
		}
		else
		{
			if(m_transtest[0] > 0)
			{
				m_transtest[0] += 1;
				trans[1] = 0.0f;
				trans[0] *= 0.5f;
			}
			else if(m_transtest[1] > 0)
			{
				m_transtest[1] += 1;
				trans[0] = 0.0f;
				trans[1] *= 0.5f;
			}
			if(m_transtest[0]>10 || m_transtest[1]>10)
			    m_transtest.set(0,0);
		}
		m_futureMatrix.setTrans(m_futureMatrix.getTrans() + trans);
		//CRCore::notify(CRCore::FATAL)<<" crViewMatterObject::move(): dirLength = "<<dir.length()<<" movedistance = "<<m_velocity[0] * m_frameInterval<<" m_frameInterval = "<<m_frameInterval<<" m_velocity = "<<m_velocity<<std::endl;
	}

	//m_matrix.setTrans(m_lastPos + trans);
	//if(!m_cameraBind) 
}

void crViewMatterObject::setHPR(const CRCore::crVector3f& hpr)
{
    m_hpr = hpr;
}

void crViewMatterObject::setH(float H)
{
    m_hpr[0] = H;
}

void crViewMatterObject::setP(float P)
{
	m_hpr[1] = P;
}

void crViewMatterObject::setR(float R)
{
	m_hpr[2] = R;
}

void crViewMatterObject::setLockViewPitch(bool viewPitch)
{
    m_lockViewPitch = viewPitch;
}
bool crViewMatterObject::getLockViewPitch()
{
	return m_lockViewPitch;
}
void crViewMatterObject::setLockViewRoll(bool viewRoll)
{
	m_lockViewRoll = viewRoll;
}
bool crViewMatterObject::getLockViewRoll()
{
	return m_lockViewRoll;
}
void crViewMatterObject::setLockViewOffset(bool viewOffset)
{
	m_lockViewOffset = viewOffset;
}
bool crViewMatterObject::getLockViewOffset()
{
	return m_lockViewOffset;
}
void crViewMatterObject::doHPR()
{//需要检查一下hpr乘法的先后顺序
	crGameBodyInfo::ManipulatorMode manipulatorMode = m_gameBodyInfo->getManipulatorMode();
	if(manipulatorMode == crGameBodyInfo::MM_RPG||manipulatorMode==crGameBodyInfo::MM_RTS)
	{
		crVector3f hpr = m_hpr * m_frameInterval + m_rotateAngles;
		if(!m_lockViewPitch) 
		{
			hpr[1] += m_viewPitch;
			m_viewPitch = 0.0f;
		}
		if(hpr[1] != 0.0f) m_futureMatrix.preMult(CRCore::crMatrix::rotate(hpr[1], 1.0, 0.0, 0.0));//pitch
		if(hpr[2] != 0.0f) m_futureMatrix.preMult(CRCore::crMatrix::rotate(hpr[2], 0.0, 1.0, 0.0));//roll
		if(hpr[0] != 0.0f) m_futureMatrix.preMult(CRCore::crMatrix::rotate(hpr[0], 0.0, 0.0, 1.0));//heading
		m_rotateAngles.set(0.0f,0.0f,0.0f);
	}
	else if(manipulatorMode == crGameBodyInfo::MM_FPS)
	{
		crVector3f hpr = m_hpr * m_frameInterval;
		m_currentRotation += m_rotateAngles;
		m_currentRotation += hpr;
		if(!m_lockViewPitch) m_currentRotation[1] += m_viewPitch;
		crMatrix mat;
		if(m_currentRotation[0] != 0.0f) mat.preMult(CRCore::crMatrix::rotate(m_currentRotation[0], 0.0, 0.0, 1.0));//heading
		if(m_currentRotation[1] != 0.0f) mat.preMult(CRCore::crMatrix::rotate(m_currentRotation[1], 1.0, 0.0, 0.0));//pitch
		if(m_currentRotation[2] != 0.0f) mat.preMult(CRCore::crMatrix::rotate(m_currentRotation[2], 0.0, 1.0, 0.0));//roll
		mat.setTrans(m_futureMatrix.getTrans());
		m_futureMatrix = mat;
		m_rotateAngles.set(0.0f,0.0f,0.0f);
		if(!m_lockViewPitch) m_currentRotation[1] -= m_viewPitch;
	}
}
void crViewMatterObject::doAdjust()
{
	if(m_adjustViewRoll)
	{
		if(m_viewRollAdjustTime>0.0f)
		{
			m_viewRollAdjustTime -= m_frameInterval;
			m_viewRoll += m_viewRollAdjustVelocity * m_frameInterval;
		}
		else
		{
			m_viewRollAdjustTime = 0.0f;
			m_viewRollAdjustVelocity = 0.0f;
			m_viewRoll = m_targetViewRoll;
			m_adjustViewRoll = false;
		}
	}
	if(m_adjustViewPitch)
	{
		if(m_viewPitchAdjustTime>0.0f)
		{
			m_viewPitchAdjustTime -= m_frameInterval;
			m_viewPitch += m_viewPitchAdjustVelocity * m_frameInterval;
		}
		else
		{
			m_viewPitchAdjustTime = 0.0f;
			m_viewPitchAdjustVelocity = 0.0f;
			m_viewPitch = m_targetViewPitch;
			m_adjustViewPitch = false;
		}
	}
	if(m_adjustViewHeight)
	{
		if(m_viewHeightAdjustTime>0.0f)
		{
			m_viewHeightAdjustTime -= m_frameInterval;
			m_viewHeight += m_viewHeightAdjustVelocity * m_frameInterval;
		}
		else
		{
			m_viewHeightAdjustTime = 0.0f;
			m_viewHeightAdjustVelocity = 0.0f;
			m_viewHeight = m_targetViewHeight;
			m_adjustViewHeight = false;
		}
	}
	if(m_adjustViewOffset)
	{
		if(m_viewOffsetAdjustTime>0.0f)
		{
			m_viewOffsetAdjustTime -= m_frameInterval;
			m_viewOffset += m_viewOffsetAdjustVelocity * m_frameInterval;
			m_currentViewOffset = m_viewOffset;
		}
		else
		{
			m_viewOffsetAdjustTime = 0.0f;
			m_viewOffsetAdjustVelocity = 0.0f;
			m_viewOffset = m_targetViewOffset;
			m_adjustViewOffset = false;
		}
	}
}
//void crViewMatterObject::changeCamera(unsigned int index)
//{
//	if(m_cameraSwitch.valid())
//	{
//		m_cameraSwitch->setActiveSwitchSet(index);
//	}
//}

//void crViewMatterObject::changeBody(unsigned int index)
//{
//	if(m_bodySwitch.valid())
//	{
//		m_bodySwitch->setActiveSwitchSet(index);
//	}
//}
void crViewMatterObject::setTargetViewRoll(float targetViewRoll,float times)
{
	if(m_targetViewRoll != targetViewRoll)
	{
		m_adjustViewRoll = true;
		m_targetViewRoll = targetViewRoll;
		m_viewRollAdjustTime = times;
		m_viewRollAdjustVelocity = (m_targetViewRoll - m_viewRoll)/m_viewRollAdjustTime;
	}
}
void crViewMatterObject::setTargetViewRoll2(float targetViewRoll,float speed)
{
	if(m_targetViewRoll != targetViewRoll)
	{
		m_adjustViewRoll = true;
		m_targetViewRoll = targetViewRoll;
		if(m_targetViewRoll<m_viewRoll)
		{
			speed = -speed;
		}
		m_viewRollAdjustTime = (m_targetViewRoll - m_viewRoll)/speed;
		m_viewRollAdjustVelocity = speed;
	}
}
void crViewMatterObject::setTargetViewPitch(float targetViewPitch,float times)
{
	if(m_targetViewPitch != targetViewPitch)
	{
		m_adjustViewPitch = true;
		m_targetViewPitch = targetViewPitch;
		m_viewPitchAdjustTime = times;
		m_viewPitchAdjustVelocity = (m_targetViewPitch - m_viewPitch)/m_viewPitchAdjustTime;
	}
}
void crViewMatterObject::setTargetViewPitch2(float targetViewPitch,float speed)
{
	if(m_targetViewPitch != targetViewPitch)
	{
		m_adjustViewPitch = true;
		m_targetViewPitch = targetViewPitch;
		if(m_targetViewPitch<m_viewPitch)
		{
			speed = -speed;
		}
		m_viewPitchAdjustTime = (m_targetViewPitch - m_viewPitch)/speed;
		m_viewPitchAdjustVelocity = speed;
	}
}
void crViewMatterObject::setTargetViewHeight(float targetViewHeight,float times)
{
	if(m_targetViewHeight != targetViewHeight)
	{
		m_adjustViewHeight = true;
		m_targetViewHeight = targetViewHeight;
		m_viewHeightAdjustTime = times;
		m_viewHeightAdjustVelocity = (m_targetViewHeight - m_viewHeight)/m_viewHeightAdjustTime;
	}
}
void crViewMatterObject::setTargetViewHeight2(float targetViewHeight,float speed)
{
	if(m_targetViewHeight != targetViewHeight)
	{
		m_adjustViewHeight = true;
		m_targetViewHeight = targetViewHeight;
		if(m_targetViewHeight<m_viewHeight)
		{
			speed = -speed;
		}
		m_viewHeightAdjustTime = (m_targetViewHeight - m_viewHeight)/speed;
		m_viewHeightAdjustVelocity = speed;
	}
}
void crViewMatterObject::setTargetViewOffset(float targetViewOffset,float times)
{
	if(m_targetViewOffset != targetViewOffset)
	{
		m_adjustViewOffset = true;
		m_targetViewOffset = targetViewOffset;
		m_viewOffsetAdjustTime = times;
		m_viewOffsetAdjustVelocity = (m_targetViewOffset - m_viewOffset)/m_viewOffsetAdjustTime;
	}
}
void crViewMatterObject::setTargetViewOffset2(float targetViewOffset,float speed)
{
	if(m_targetViewOffset != targetViewOffset)
	{
		m_adjustViewOffset = true;
		m_targetViewOffset = targetViewOffset;
		if(m_targetViewOffset<m_viewOffset)
		{
			speed = -speed;
		}
		m_viewOffsetAdjustTime = (m_targetViewOffset - m_viewOffset)/speed;
		m_viewOffsetAdjustVelocity = speed;
	}
}
void crViewMatterObject::setFreeCamera(bool bln)
{
	m_freeCamera = bln;
}
bool crViewMatterObject::getFreeCamera()
{
	return m_freeCamera;
}
void crViewMatterObject::setCameraPitchDelta(float pitch)
{
	m_cameraPithDelta = pitch;
}
float crViewMatterObject::getCameraPitchDelta()
{
	return m_cameraPithDelta;
}
CRCore::crMatrix crViewMatterObject::getCameraMatrix()
{
	CRCore::crVector3f dir,up,pos;
	crMatrix cameraMatrix;
	float height = /*getBoundBox().zLength()*/m_physicsBox[2] * 0.5f;
	crGameBodyInfo::ManipulatorMode manipulatorMode = m_gameBodyInfo->getManipulatorMode();
	if(!m_freeCamera)
		cameraMatrix = m_matrix;
	if(manipulatorMode == crGameBodyInfo::MM_FPS)
	{
		if(m_currentViewPitch!=0.0f)
			cameraMatrix = crMatrixd::rotate(m_currentViewPitch + m_cameraPithDelta,X_AXIS) * m_matrix;
		else
			cameraMatrix = m_matrix;
	}
	else if(manipulatorMode == crGameBodyInfo::MM_TANK)
	{
		if(m_viewRoll != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewRoll, Z_AXIS));//heading
		if(m_viewPitch != 0.0f) cameraMatrix.preMult(crMatrixd::rotate(m_viewPitch + m_cameraPithDelta,X_AXIS));
		cameraMatrix.setTrans(m_matrix.getTrans());
	}
	else
	{
		if(!m_lockViewPitch)
		{
			float pitchdelta = m_viewPitchVelocity * m_frameInterval;
			if(m_currentViewPitch < m_viewPitch-pitchdelta)
			{
				m_currentViewPitch += pitchdelta;
			}
			else if(m_currentViewPitch>m_viewPitch+pitchdelta)
			{
				m_currentViewPitch -= pitchdelta;
			}
			else
			{
				m_currentViewPitch = m_viewPitch;
			}
		}
		if(m_viewRoll != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewRoll, 0.0, 0.0, 1.0));//heading
		if(m_currentViewPitch != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_currentViewPitch + m_cameraPithDelta,X_AXIS));//pitch
		//if(manipulatorMode != crGameBodyInfo::MM_RTS)
		cameraMatrix.setTrans(m_matrix.getTrans());
		//cameraMatrix = crMatrixd::translate(m_matrix.getTrans());
	}

	//if(m_cameraSwitch.valid())
	//{
	//    CRCore::crVector3f lookAt;
	//	crNode *currentActiveNode = m_cameraSwitch->getFirstActiveChild();
	//	if(currentActiveNode)
	//	{
	//		if(currentActiveNode->getDescriptionsInString().find("#CameraPathNode") != -1 )
	//		{
	//			crMatrixTransform *cameraPath,*lookAtPath;
	//			crGroup *group = dynamic_cast<crGroup *>(currentActiveNode);
	//			cameraPath = dynamic_cast<crMatrixTransform *>(group->getChild(0));
	//		    pos = cameraPath->getBound().center();
	//			pos[1] += m_viewOffset;
	//			pos[2] += m_viewHeight - height;
	//			cameraPath->computeLocalToWorldMatrix(cameraMatrix,NULL);
 //               pos = pos * cameraMatrix;

	//			lookAtPath = dynamic_cast<crMatrixTransform *>(group->getChild(1));
	//			lookAt = lookAtPath->getBound().center();
	//			lookAt = lookAt * m_matrix;
	//			dir = pos - lookAt;
	//			dir.normalize();

	//			cameraMatrix.setTrans(0,0,0);
	//			up = (Z_AXIS*cameraMatrix).normalize();
	//		}
	//		else
	//		{
	//			crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(currentActiveNode);
	//			bool useLookAt = crArgumentParser::readKeyValue(currentActiveNode->getDescriptions(),"LookAt",lookAt);
	//			bool computed = false;
	//			if(matrixTransform)
	//			{
	//				pos = currentActiveNode->getBound().center();
	//				pos[1] += m_viewOffset;
	//				pos[2] += m_viewHeight - height;
	//				matrixTransform->computeLocalToWorldMatrix(cameraMatrix,NULL);
	//				//cameraMatrix = matrixTransform->getMatrix() * cameraMatrix;
	//				//pos = cameraMatrix.getTrans();
	//				pos = pos * cameraMatrix;
	//				computed = true;
	//			}
	//			crSequence *sequence = dynamic_cast<crSequence *>(currentActiveNode);
	//			if(sequence)
	//			{
	//				crNode *currentSequenceChild = sequence->getChild(sequence->getValue());
	//				useLookAt = crArgumentParser::readKeyValue(currentSequenceChild->getDescriptions(),"LookAt",lookAt);
	//				matrixTransform = dynamic_cast<crMatrixTransform *>(currentSequenceChild);
	//				if(matrixTransform)
	//				{
	//					pos = currentSequenceChild->getBound().center();
	//					pos[1] += m_viewOffset;
	//					pos[2] += m_viewHeight - height;
	//					matrixTransform->computeLocalToWorldMatrix(cameraMatrix,NULL);
	//					//cameraMatrix = matrixTransform->getMatrix() * cameraMatrix;
	//					//pos = cameraMatrix.getTrans();
	//					pos = pos * cameraMatrix;

	//					////pos = pos * cameraMatrix;
	//					//cameraMatrix = matrixTransform->getMatrix() * cameraMatrix;
	//					//pos = cameraMatrix.getTrans();
	//					computed = true;
	//				}
	//				else
	//				{
	//					pos = currentSequenceChild->getBound().center();
	//					pos[1] += m_viewOffset;
	//					pos[2] += m_viewHeight - height;
	//					pos = pos * cameraMatrix;
	//					//cameraMatrix = m_matrix;
	//					computed = true;
	//				}
	//			}
	//			crDOFTransform *dof = dynamic_cast<crDOFTransform *>(currentActiveNode);
	//			if(dof)
	//			{
	//				pos = currentActiveNode->getBound().center();
	//				pos[1] += m_viewOffset;
	//				pos[2] += m_viewHeight - height;
	//				dof->computeLocalToWorldMatrix(cameraMatrix,NULL);
	//				pos = pos * cameraMatrix;
	//				computed = true;
	//			}
	//			if(!computed)
	//			{
	//				pos = currentActiveNode->getBound().center();
	//				pos[1] += m_viewOffset;
	//				pos[2] += m_viewHeight - height;
	//				pos = pos * cameraMatrix;
	//				//cameraMatrix = m_matrix;
	//			}
	//			//CRCore::notify(CRCore::FATAL)<<" crViewMatterObject::getCameraMatrix(): m_matrix = "<<m_matrix<<std::endl;
	//			//CRCore::notify(CRCore::FATAL)<<" crViewMatterObject::getCameraMatrix(): cameraMatrix = "<<cameraMatrix<<std::endl;
	//			//CRCore::notify(CRCore::FATAL)<<" crViewMatterObject::getCameraMatrix(): m_viewPitch = "<<m_viewPitch<<std::endl;
	//			//return cameraMatrix;
	//			cameraMatrix.setTrans(0,0,0);
	//			up = (Z_AXIS*cameraMatrix).normalize();
	//			if(useLookAt)
	//			{
	//				lookAt = lookAt * m_matrix;
	//				dir = pos - lookAt;
	//				dir.normalize();
	//			}
	//			else
	//				dir = (Y_AXIS*cameraMatrix).normalize();
	//		}
	//		////pos += crVector3f(dir[0],dir[1],0.0f) * m_viewOffset;
	//		//m_radarPos = pos;//m_matrix.getTrans();
	//		////m_radarPos[2] += m_viewHeight - height;
	//		return crMatrix::lookAt(pos,pos-dir,up);
	//	}
	//}
	float delta = 0.0f;
	if(m_viewVelocity!=0.0f)
	{
		m_currentViewOffset += m_viewVelocity * m_frameInterval;
		if( (m_viewVelocity>0.0f && m_currentViewOffset>m_viewOffset) || 
			(m_viewVelocity<0.0f && m_currentViewOffset<m_viewOffset) )
		{
			m_currentViewOffset = m_viewOffset;
			m_viewVelocity = 0.0f;
		}
		m_currentViewOffset = CRCore::maximum(m_currentViewOffset,0.0f);
	}
	//else
	//{
	//	delta = m_currentViewOffset-m_viewOffset;
	//	if(delta>0.5f)
	//	{
	//		m_viewVelocity = -m_viewOffsetVelocity;
	//	}
	//	else if(delta>-0.5f)
	//	{
	//		m_currentViewOffset = m_viewOffset;
	//	}
	//	else
	//	{
	//		m_viewVelocity = m_viewOffsetVelocity;
	//	}
	//}

	cameraMatrix.setTrans(0,0,0);
	dir = (Y_AXIS*cameraMatrix).normalize();
	up = (Z_AXIS*cameraMatrix).normalize();
	crVector3f center = m_matrix.getTrans();//getBound().center();如果无效bound，则其center返回值始终是0，如编辑器模式。
	center[2] += m_viewHeight - height;
	pos = center + dir * m_currentViewOffset;

	if(m_currentViewOffset>=m_viewOffsetRange[0])
	{
		if(!m_lineSegment.valid())
			m_lineSegment = new crLineSegment;
		m_lineSegment->set(center,pos);
		m_iv.reset();
		m_iv.addLineSegment(m_lineSegment.get());
		m_iv.insertAvoidIntersectNode(const_cast<crGroup *>(this->getParent(0)));
		//m_iv.insertAvoidIntersectNodeType("crViewMatterObject");
		//m_iv.insertAvoidIntersectNodeType("crCharacterMatterObject");
		crBrain::getInstance()->accept(m_iv);
		if (m_iv.hits())
		{
			CRCore::crIntersectVisitor::HitList& hitList = m_iv.getHitList(m_lineSegment.get());
			if (!hitList.empty())
			{
				crVector3f lv = center - hitList.front().getWorldIntersectPoint();
				m_currentViewOffset = lv.length();
				m_viewVelocity = 0.0f;
			}
		}
		else
		{
			if(m_currentViewOffset<m_viewOffset)
				m_viewVelocity = m_viewOffsetVelocity;
			else if(m_currentViewOffset>m_viewOffset)
				m_viewVelocity = -m_viewOffsetVelocity;
		}
	}
	m_currentViewOffset = clampTo(m_currentViewOffset,m_viewOffsetRange[0],m_viewOffsetRange[1]);
	pos = center + dir * m_currentViewOffset;
	//pos 振动
	if(m_viewShakeInterval>0.0f)
	{
		m_viewShakeInterval -= m_frameInterval;
		CRCore::rangef shake(-m_viewShakeRange,m_viewShakeRange);
		pos += crVector3f(shake.get_random(),shake.get_random(),shake.get_random());
	}
	return crMatrix::lookAt(pos,center,up);
}

CRCore::crMatrixd crViewMatterObject::getViewMatrix()
{
	if(!m_lockViewPitch)
		return m_matrix;
    return crMatrixd::rotate(m_viewPitch,X_AXIS) * m_matrix;
}

CRCore::crVector3f crViewMatterObject::getViewDirection()
{//
	if(!m_lockViewPitch)
		return (Y_AXIS*getMatterObjectRotation()).normalize();

	return (Y_AXIS*crMatrixd::rotate(m_viewPitch,X_AXIS)*getMatterObjectRotation()).normalize();
}

CRCore::crVector3f crViewMatterObject::getViewUpVector()
{//
	return (Y_AXIS*crMatrixd::rotate(m_viewPitch,X_AXIS)).normalize();
}

//CRCore::crVector3f crViewMatterObject::getDirection()
//{
//	return (Y_AXIS * CRCore::crMatrix::rotate(m_currentRotation[0], CRCore::Z_AXIS)).normalize();
//}
//CRCore::crVector3f crViewMatterObject::getDirectionX()
//{
//	return (X_AXIS * CRCore::crMatrix::rotate(m_currentRotation[0], CRCore::Z_AXIS)).normalize();
//}

void crViewMatterObject::updateViewPitch( float delta )
{
	if(!m_lockViewPitch && !m_adjustViewPitch)
	{
		float pitch = m_viewPitch+delta;	
		pitch = CRCore::clampTo(pitch,m_viewPitchRange[0],m_viewPitchRange[1]);
		//crGameBodyInfo::ManipulatorMode manipulatorMode = m_gameBodyInfo->getManipulatorMode();
		//if(manipulatorMode == crGameBodyInfo::MM_FPS)
		//{
		//	pitch = CRCore::clampTo(pitch,-0.8f,0.8f);
		//	if(pitch>0.8f) pitch = 0.8f;
		//	else if(pitch<-0.8f) pitch = -0.8f; 
		//}
		//else if(manipulatorMode == crGameBodyInfo::MM_RPG)
		//{
		//	pitch = CRCore::clampTo(pitch,0.1f,1.5f);
		//}
		//else if(manipulatorMode == crGameBodyInfo::MM_RTS)
		//{
		//	return;
		//}
		m_viewPitch = pitch;
		//CRCore::notify(CRCore::FATAL)<<"crMatterObject::updateViewPitch: m_viewPitch = "<<m_viewPitch<<" delta ="<<delta<<std::endl;
	}
}

void crViewMatterObject::updateViewRoll( float delta )
{
	if(!m_lockViewRoll && !m_adjustViewRoll)
	{
		m_viewRoll += delta;
		if(fabs(m_viewRoll)>PI)
		{
			m_viewRoll = m_viewRoll>0.0f?(m_viewRoll - 2.0f*PI):(m_viewRoll+2.0f*PI);
		}
	}
	//CRCore::notify(CRCore::FATAL)<<"crMatterObject::updateViewRoll: m_viewRoll = "<<m_viewRoll<<std::endl;
}

//void crViewMatterObject::setCameraBind(bool cameraBind)
//{
//	m_cameraBind = cameraBind;
//}
//
//bool crViewMatterObject::getCameraBind() const
//{
//	return m_cameraBind;
//}

//////////////////
//
//radar
//
/////////////////
void crViewMatterObject::attachRadar(crRadarMatterObject *radar)
{
    m_radarMatterObject = radar;
	m_radarMatterObject->setGroundDetect(m_groundDetect);
	m_visionSenseInfor = new crVisionSenseInfor;
}

crRadarMatterObject *crViewMatterObject::getRadar()
{
	return m_radarMatterObject.get();
}

void crViewMatterObject::kill( int killflg )
{
	if(!m_gameBodyInfo.valid()) return;

    m_gameBodyInfo->kill();
	//refreshArmor_HpInfo();
	dead(killflg);
	//crDrawable *text = crFilterRenderManager::getInstance()->getDrawable("KillInfo");
	//if(text)
	//{
	//	text->setText("killed " + getName());
	//	text->setVisiable(true);
	//}
	//crFilterRenderManager::getInstance()->addDequeString("killed " + getName());
}

void crViewMatterObject::addExcution(int excution,crViewMatterObject *firePerson)
{
	if(!m_gameBodyInfo.valid() || isDead()) return;

    m_gameBodyInfo->addExcution(excution);
	//refreshArmor_HpInfo();
	if(m_gameBodyInfo->isDead()) 
	{
		dead(0,firePerson);
	}
}
CRCore::crNode *crViewMatterObject::getCurrentActiveBody()
{
	if(dynamic_cast<crMultiSwitch *>(m_body.get()))
	{
		return dynamic_cast<crMultiSwitch *>(m_body.get())->getFirstActiveChild();
	}
	return m_body.valid()?m_body->getChild(0):getChild(0);
}
void crViewMatterObject::setVisiable( bool visiable )
{ 
	m_visiable = visiable; 
	if(m_currentWeaponMatterObject.valid()) m_currentWeaponMatterObject->setVisiable(visiable);
}

void crViewMatterObject::setEnableIntersect( bool enableIntersect)
{
	crMatterObject::setEnableIntersect(enableIntersect);
	if(m_radarMatterObject.valid()) m_radarMatterObject->setEnableIntersect(enableIntersect);
}

void crViewMatterObject::dead( int deadflg, crViewMatterObject *firePerson)
{
	if(m_deadFlag || !m_gameBodyInfo.valid()) return;

	//m_velocity.set(0.0f,0.0f);
	//setVisiable(false);
	//if(m_currentWeaponMatterObject.valid()) m_currentWeaponMatterObject->setVisiable(false);
	//setEnableIntersect(false);
	
	//if(m_bodySwitch.valid()) m_bodySwitch->setVisiable(true);

	m_gameBodyInfo->increaseDeathCount();
	if(m_radarMatterObject.valid()) m_radarMatterObject->setEnableIntersect(false);

	//crBotManager::getInstance()->killBot(this);
	//deadcallback
	doEvent(WCH_DEAD,MAKECREPARAM(deadflg,firePerson));

	setVelocity(crVector3f(0.0f,0.0f,0.0f));
	m_deadFlag = true;
	m_deathTime = CRCore::Timer::instance()->tick();
}

float crViewMatterObject::getDeathInterval()
{
	return isDead()?CRCore::Timer::instance()->delta_s(m_deathTime,CRCore::Timer::instance()->tick()):0.0f;
}

void crViewMatterObject::setRequireRenascence( bool require )
{
	m_requireRenascence = require;
}

bool crViewMatterObject::getRequireRenascence()
{
	return m_requireRenascence;
}

void crViewMatterObject::setHideBody(bool hidebody)
{
	m_hideBody = hidebody;
	setVisiable(!m_hideBody);
	//setEnableIntersect(!m_hideBody);
	//if(m_botModel.valid())
	//{
	//	m_botModel->setVisiable(!m_hideBody);
	//}
}

bool crViewMatterObject::getHideBody() const
{
	return m_hideBody;
}

void crViewMatterObject::renascence()
{
	//CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::renascence()00 "<<this->getName()<<std::endl;
	//CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::renascence()0: "<<getMatterObjectPosition()<<std::endl;
	if(!m_gameBodyInfo.valid()) return;
	
	m_gameBodyInfo->renascence();
	//refreshArmor_HpInfo();

	//m_currentWeaponMatterObject = NULL;
	//std::vector<std::string> toEraseList;
	//for( WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.begin();
	//	 itr != m_weaponMatterObjects.end();
	//	 ++itr )
	//{
	//	if(itr->first.compare("Default")!=0)
	//	{
	//		itr->second->releaseObjects();
 //           getParent(0)->removeChild(itr->second.get());
 //           toEraseList.push_back(itr->first);
	//	}
	//}
	//for(std::vector<std::string>::iterator eitr=toEraseList.begin();
	//	eitr!=toEraseList.end();
	//	++eitr)
	//{
	//	m_weaponMatterObjects.erase(*eitr);
	//}

/*	setVelocity(crVector2f((m_minSpeed[0] + m_maxSpeed[0]) * 0.5f,0.0f));
	changeCamera(0);
	changeBody(0);

	crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(getParent(0));
	if(!bodyMG)
	{
		CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::renascence(): ViewMatterObject的父节点类型不正确 "<<this->getName()<<std::endl;
		return;
	}
	crEventGroup *bodyNC = bodyMG->getAttachedEventGroup();
	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_SPACE,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_UP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_LEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_Q,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_O,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));

	selectWeaponMatterObject("Default");
  //  if(m_currentWeaponMatterObject.valid())
		//m_currentWeaponMatterObject->setBulletsInWeapon()

	if(!m_cameraBind || !m_hideBody) setVisiable(true);
	setEnableIntersect(true);
	if(m_radarMatterObject.valid()) m_radarMatterObject->setEnableIntersect(true);
*/
	//crBotManager::getInstance()->renascenceBot(this);
	//renascencecallback
	doEvent(WCH_RENASCENCE);
	//CRCore::notify(CRCore::FATAL)<<"crViewMatterObject::renascence()1: "<<getMatterObjectPosition()<<std::endl;

	m_deadFlag = false;
	m_requireRenascence = false;
	//crFilterRenderManager::getInstance()->addDequeString(getName() + " renascent.");
	//enablePhysics(0.1f);
	////setLinearVel(crVector3f(0.0f,0.0f,1.0f), 0.5f);//jump

	//CRCore::notify(CRCore::ALWAYS)<<"crViewMatterObject::renascence()"<< std::endl;
}

//////////////////
//
//weapon
//
/////////////////

bool crViewMatterObject::testToAddWeaponMatterObject(crWeaponMatterObject *weapon)
{
	WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.find(weapon->getName());
	if(itr != m_weaponMatterObjects.end())
	{
		itr->second->addBulletCount(weapon->getBulletCount());
		//refreshBulletCountInfo();

		return true;
	}

	return false;
}

bool crViewMatterObject::addCaisson(crCaissonMatterObject *caisson)
{
	WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.find(caisson->getCaissonObject()->getWeaponName());
	if(itr != m_weaponMatterObjects.end())
	{
		itr->second->addBulletCount(caisson->getBulletCount());
		//refreshBulletCountInfo();

		return true;
	}

	return false;
}

bool crViewMatterObject::addHp(crHpMatterObject *hp)
{
	if(!m_gameBodyInfo.valid()) return false;
	int myHp = m_gameBodyInfo->getHP();
	if(myHp == 100) 
		return false;
	myHp += hp->getHpObject()->getHpValue();
	myHp = myHp>100?100:myHp;
    m_gameBodyInfo->setHP(myHp);

	//refreshArmor_HpInfo();
	return true;
}

bool crViewMatterObject::addArmor(crArmorMatterObject *armor)
{
	if(!m_gameBodyInfo.valid()) return false;

	int defend = m_gameBodyInfo->getDefend();
	if(defend == 100) 
		return false;
	defend += armor->getArmorObject()->getArmorValue();
	defend = defend>100?100:defend;
	m_gameBodyInfo->setDefend(defend);

	//refreshArmor_HpInfo();
	return true;
}

void crViewMatterObject::addWeaponMatterObject(crWeaponMatterObject *weapon)
{
	m_weaponMatterObjects[weapon->getName()] = weapon;

	getParent(0)->addChild(weapon);//存在线程不安全因素，需要修正

	weapon->setVisiable(false);
	weapon->setEnableIntersect(false);

	//selectWeaponMatterObject(weapon);
}

//void crViewMatterObject::setBotModel(CRCore::crMatrixTransform *botModel)
//{
//	crGroup *parent = getParent(0);
//    if(m_botModel.valid())
//	{
//        parent->removeChild(m_botModel.get());
//	}
//    parent->addChild(botModel);
//	m_botModel = botModel;
//	m_botModel->setVisiable(!m_hideBody);
//}

void crViewMatterObject::selectWeaponMatterObject(const std::string &weaponName)
{
	WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.find(weaponName);
	if(itr != m_weaponMatterObjects.end())
	{
		/*doEvent(WCH_WEAPON_SELECT,MAKECREPARAM(&(itr->first),NULL));
		if(m_currentWeaponMatterObject.valid()) 
		{
			doEvent(WCH_WEAPON_UNSELECT,MAKECREPARAM(&(m_currentWeaponMatterObject->getName()),NULL));
			m_currentWeaponMatterObject->setVisiable(false);
		}
		m_currentWeaponMatterObject = itr->second.get();
		m_currentWeaponMatterObject->setVisiable(true);*/

        selectWeaponMatterObject(itr->second.get());
	}

	//refreshBulletCountInfo();
}

void crViewMatterObject::selectWeaponMatterObject(crWeaponMatterObject *weapon)
{
	if(weapon->isBulletEmpty()) return;
	if(m_currentWeaponMatterObject.valid()) 
	{
		if(m_currentWeaponMatterObject == weapon)
		{
			return;
		}
		m_currentWeaponMatterObject->setVisiable(false);
		//doEvent(WCH_WEAPON_UNSELECT,MAKECREPARAM(&(m_currentWeaponMatterObject->getName()),NULL));
		doEvent(WCH_WEAPON_UNSELECT,MAKECREPARAM(m_currentWeaponMatterObject.get(),NULL));
		m_currentWeaponMatterObject->doEvent(WCH_WEAPON_UNSELECT,MAKECREPARAM(this,NULL));
	}
	m_currentWeaponMatterObject = weapon;
	m_currentWeaponMatterObject->setVisiable(true);
	//doEvent(WCH_WEAPON_SELECT,MAKECREPARAM(&(m_currentWeaponMatterObject->getName()),NULL));
	doEvent(WCH_WEAPON_SELECT,MAKECREPARAM(m_currentWeaponMatterObject.get(),NULL));
	m_currentWeaponMatterObject->doEvent(WCH_WEAPON_SELECT,MAKECREPARAM(this,NULL));
	//refreshBulletCountInfo();

	//updateWeaponMatrix();
}

void crViewMatterObject::removeWeapon(const std::string &weaponName)
{
    removeWeapon(getWeaponMatterObject(weaponName));
}

void crViewMatterObject::removeWeapon(crWeaponMatterObject *weapon)
{
    if(!weapon) return;

	weapon->setVisiable(false);
	getParent(0)->removeChild(weapon);//存在线程不安全因素，需要修正
	m_weaponMatterObjects.erase(weapon->getName());
}

//void crViewMatterObject::refreshBulletCountInfo()
//{
//    if(m_weaponMatterObjects.empty())
//	{
//        if(m_currentWeaponMatterObject.valid())
//		{
//			removeWeapon(m_currentWeaponMatterObject.get());
//		}
//	}
//
//	//bool noWeapon = false;
//	if(!m_currentWeaponMatterObject.valid())
//	{
//        if(!m_weaponMatterObjects.empty())
//		{
//			for( WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.begin();
//				itr != m_weaponMatterObjects.end();
//				++itr )
//			{
//				if(!itr->second->isBulletEmpty())
//				{
//					return selectWeaponMatterObject(itr->second.get());
//				}
//			}
//		}
//	}
//	else
//	{
//		if(m_currentWeaponMatterObject->isBulletEmpty())
//		{
//			for( WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.begin();
//				itr != m_weaponMatterObjects.end();
//				++itr )
//			{
//				if(!itr->second->isBulletEmpty())
//				{
//					return selectWeaponMatterObject(itr->second.get());
//				}
//			}
//		}
//	}
//	//if(m_cameraBind)
//	//{
//	//	crDrawable *text = crFilterRenderManager::getInstance()->getDrawable("BulletCount");
//	//	if(!text) return;
//	//	if(noWeapon)
//	//	{
// //           text->setVisiable(false);
//	//	}
//	//	else
//	//	{
//	//		int bulletsInWeapon = m_currentWeaponMatterObject->getBulletsInWeapon();
//	//		if(m_currentWeaponMatterObject.valid() && bulletsInWeapon > -1)
//	//		{
//	//			text->setVisiable(true);
//	//			char tmpText[8];
//	//			sprintf(tmpText,"%3d/%3d\0",bulletsInWeapon,m_currentWeaponMatterObject->getBulletCount());
//	//			text->setText(tmpText);
//	//		}
//	//		else
//	//			text->setVisiable(false);
//	//	}
//	//}
//}
//
//void crViewMatterObject::refreshArmor_HpInfo()
//{
//	//if(!m_gameBodyInfo.valid()) return;
//	//if(m_cameraBind)
//	//{
//	//	crDrawable *text = crFilterRenderManager::getInstance()->getDrawable("Armor_Life");
//	//	if(!text) return;
//	//	char tmpText[8];
//	//	sprintf(tmpText,"%3d/%3d\0",m_gameBodyInfo->getDefend(),m_gameBodyInfo->getHP());
//	//	text->setText(tmpText);
//	//}
//}

crWeaponMatterObject *crViewMatterObject::getCurrentWeaponMatterObject()
{
	return m_currentWeaponMatterObject.get();
}

bool crViewMatterObject::isDead()const{ if(m_gameBodyInfo.valid()) return m_gameBodyInfo->isDead(); return false; }
int crViewMatterObject::checkTeam(int team) const { if(m_gameBodyInfo.valid()) return m_gameBodyInfo->checkTeam(team); return 0; }
void crViewMatterObject::increaseKilledCount() { if(m_gameBodyInfo.valid()) m_gameBodyInfo->increaseKilledCount(); }
int crViewMatterObject::getTeam() { if(m_gameBodyInfo.valid()) return m_gameBodyInfo->getTeam(); return 0; }
int crViewMatterObject::getHP() { if(m_gameBodyInfo.valid()) return m_gameBodyInfo->getHP(); return 0; }
int crViewMatterObject::getDefend() { if(m_gameBodyInfo.valid()) return m_gameBodyInfo->getDefend(); return 0; }

void crViewMatterObject::setBotModelName(const std::string &name){ m_botModelName = name; }

//////////////////////////////////////
//
//crViewObjectNearCallback
//
/////////////////////////////////////
crViewObjectNearCallback *crViewObjectNearCallback::instance()
{
	static CRCore::ref_ptr< crViewObjectNearCallback > s_nearCallback = new crViewObjectNearCallback;
	return s_nearCallback.get();
}

void crViewObjectNearCallback::nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 )
{
	//CRCore::notify(CRCore::ALWAYS)<<"crViewObjectNearCallback"<< std::endl;
	ref_ptr<crNode> node1,node2;
	if(!crOdeWorld::isCollideValid(o1,o2,node1,node2)) return;

	//CRCore::crNode *node1 = crOdeWorld::getDynamicGeomNode(o1);
	//CRCore::crNode *node2 = crOdeWorld::getDynamicGeomNode(o2);
	//bool handleCollideCallback = false;
	dContact contact[MAX_CONTACTS];
	int n = 0;

	crViewMatterObject *viewMO = 0;
	dGeomID voGeomID = 0;
	dGeomID collideGeomID = 0;

	crMatterObject *mo1 = 0;
	crMatterObject *mo2 = 0;

	if(node1.valid()) mo1 = dynamic_cast<crMatterObject *>(node1.get());
	else crOdeWorld::getStaticGeomNode(o1,node1);
	if(node2.valid()) mo2 = dynamic_cast<crMatterObject *>(node2.get());
	else crOdeWorld::getStaticGeomNode(o2,node2);

	crViewMatterObject *vo1 = 0;
	crViewMatterObject *vo2 = 0;
	if(mo1) vo1 = dynamic_cast<crViewMatterObject *>(mo1);
	if(mo2) vo2 = dynamic_cast<crViewMatterObject *>(mo2);

	n = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
	if(n==0) return;
	int collideTest = 1;
	if(node1.valid()) 
	{
		node1->doEvent(WCH_COLLIDETEST,MAKECREPARAM(node2.get(),&collideTest));
		if(collideTest == 0) return;
	}
	if(node2.valid())
	{
		collideTest = 1;
		node2->doEvent(WCH_COLLIDETEST,MAKECREPARAM(node1.get(),&collideTest));
		if(collideTest == 0) return;
	}

	//handleCollideCallback = true;
	if(vo1||vo2)
	{
		if(vo1)
		{/* && */
			viewMO = vo1;
			voGeomID = o1;
			collideGeomID = o2;
            if(!dynamic_cast<crBulletMatterObject *>(mo2)) vo1->collided();
		}
		else if(vo2)
		{/* && !dynamic_cast<crBulletMatterObject *>(mo1)*/
			viewMO = vo2;
			voGeomID = o2;
			collideGeomID = o1;
			if(!dynamic_cast<crBulletMatterObject *>(mo1)) vo2->collided();
		}

		if(voGeomID == viewMO->getGeomID())
		{
			//if(!(vo1&&vo2))
			{
				crMatterGroup *This = static_cast<crMatterGroup *>(data);

				dBodyID b1 = dGeomGetBody(o1);
				dBodyID b2 = dGeomGetBody(o2);
				if(b1) b1 = dBodyIsEnabled(b1)?b1:0;
				if(b2) b2 = dBodyIsEnabled(b2)?b2:0;

				if ((b1 || b2) && !(b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)))
				{
					for(int i=0; i<n; i++) 
					{
						//contact[i].surface.mode = /*dContactMu2 |*/dContactSlip1 | dContactSlip2 | 
						//	/*dContactBounce |dContactSoftERP |*/  dContactSoftCFM | dContactApprox1;
						//	contact[i].surface.mu = 0.5;
						////contact[i].surface.mu2 = 0.5;
						//contact[i].surface.slip1 = 0.001;
						//contact[i].surface.slip2 = 0.001;
						////contact[i].surface.bounce = 0.001;
						////contact[i].surface.bounce_vel = 0.00001;
						//contact[i].surface.soft_cfm = 0.0001;
						////contact[i].surface.soft_erp = 0.0001;

						contact[i].surface.mode = dContactBounce | dContactSoftCFM;
						contact[i].surface.mu = dInfinity;
						contact[i].surface.mu2 = 0;
						contact[i].surface.bounce = 0.1;
						contact[i].surface.bounce_vel = 0.1;
						contact[i].surface.soft_cfm = 0.001;

						dJointID c = dJointCreateContact(This->getOdeWorld()->getWorld(),This->getOdeWorld()->getContactJointGroup(),&contact[i]);
						dJointAttach (c,b1,b2);
					}
				}

				//dBodyID collideBodyID = dGeomGetBody(collideGeomID);

				//if(collideBodyID&&dBodyIsEnabled(collideBodyID))
				//{
				//	crVector3 contactNormal(-contact[0].geom.normal[0], -contact[0].geom.normal[1], -contact[0].geom.normal[2]);
				//	if(voGeomID == o2)
				//		contactNormal = -contactNormal;
				//	contactNormal *= 10.0f;
				//	dBodyAddForceAtPos(collideBodyID,contactNormal[0],contactNormal[1],contactNormal[2],
				//		contact[0].geom.pos[0],contact[0].geom.pos[1],contact[0].geom.pos[2] );
				//}
			}

			//if(viewMO->getNumBotPhysicsObject()>0) handleCollideCallback = false;
		}
	}
	else
	{
		crMatterGroup *This = static_cast<crMatterGroup *>(data);
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		if(b1) b1 = dBodyIsEnabled(b1)?b1:0;
		if(b2) b2 = dBodyIsEnabled(b2)?b2:0;

		if ((b1 || b2) && !(b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)))
		{
			for(int i=0; i<n; i++) 
			{
				//contact[i].surface.mode = /*dContactMu2 |*/dContactSlip1 | dContactSlip2 | 
				//	/*dContactBounce |dContactSoftERP |*/  dContactSoftCFM | dContactApprox1;
				//	contact[i].surface.mu = 0.5;
				////contact[i].surface.mu2 = 0.5;
				//contact[i].surface.slip1 = 0.001;
				//contact[i].surface.slip2 = 0.001;
				////contact[i].surface.bounce = 0.001;
				////contact[i].surface.bounce_vel = 0.00001;
				//contact[i].surface.soft_cfm = 0.0001;
				////contact[i].surface.soft_erp = 0.0001;

				contact[i].surface.mode = dContactBounce | dContactSoftCFM;
				contact[i].surface.mu = dInfinity;
				contact[i].surface.mu2 = 0;
				contact[i].surface.bounce = 0.1;
				contact[i].surface.bounce_vel = 0.1;
				contact[i].surface.soft_cfm = 0.001;

				dJointID c = dJointCreateContact(This->getOdeWorld()->getWorld(),This->getOdeWorld()->getContactJointGroup(),&contact[i]);
				dJointAttach (c,b1,b2);
			}
		}
	}

	//if(handleCollideCallback)
	//{
		crCollideCallback* callback;
        crVector3 contactPos(contact[0].geom.pos[0],contact[0].geom.pos[1],contact[0].geom.pos[2]);
		crVector3 contactNormal(contact[0].geom.normal[0], contact[0].geom.normal[1], contact[0].geom.normal[2]);
		if(node1.valid())
		{
			node1->doEvent(WCH_COLLIDE,MAKECREPARAM(node2.get(),&contact[0]));
			callback = node1->getCollideCallback();
			if (callback) (*callback)(node1.get(),node2.get(),o1,o2,contactPos,contactNormal);
		}

		if(node2.valid())
		{
			dGeomID g = contact[0].geom.g1;
			contact[0].geom.g1 = contact[0].geom.g2;
			contact[0].geom.g2 = g;
			contact[0].geom.normal[0] = -contact[0].geom.normal[0];
			contact[1].geom.normal[1] = -contact[1].geom.normal[1];
			contact[2].geom.normal[2] = -contact[2].geom.normal[2];
			node2->doEvent(WCH_COLLIDE,MAKECREPARAM(node1.get(),&contact[0]));
			callback = node2->getCollideCallback();
			if (callback) (*callback)(node2.get(),node1.get(),o2,o1,contactPos,-contactNormal);
		}
	//}
}
void crViewMatterObject::setSlopeX(float x)
{
	m_slopeX = x;
}
void crViewMatterObject::setSlopeY(float y)
{
	m_slopeY = y;
}