/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include <CRPhysics\crBodyCaseMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRCore\crMath.h>
#include <CRCore\crSearchNodeBYNameVisitor.h>
//#include <CRPhysics\crBotManager.h>

using namespace CRPhysics;
using namespace CRCore;

///////////////////////
//
//FindDofTransformVisitor
//
/////////////////////////////
class FindDofTransformVisitor : public CRCore::crNodeVisitor
{
public:

	FindDofTransformVisitor():
	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	  virtual ~FindDofTransformVisitor(){}

	  virtual void apply(crDOFTransform& node)
	  { 
		  m_body->addDofTransform(&node);
		  traverse(node);
	  }
	  void setBody(crMissileSetOrFlackMatterObject *body) {m_body = body;}
protected:
      crMissileSetOrFlackMatterObject *m_body;
};

static FindDofTransformVisitor s_findDofTransformVisitor;

crMissileSetOrFlackMatterObject::crMissileSetOrFlackMatterObject()
{
}

crMissileSetOrFlackMatterObject::crMissileSetOrFlackMatterObject(const crMissileSetOrFlackMatterObject &matterObject,const CRCore::crCopyOp& copyop):
	crViewMatterObject(matterObject,copyop)
{
}

void crMissileSetOrFlackMatterObject::addDofTransform(CRCore::crDOFTransform *dof)
{
    m_dofVec.push_back(dof);
}

void crMissileSetOrFlackMatterObject::computerRotMatrix()
{
	m_rotmat.makeIdentity();
	for( DofVec::iterator itr = m_dofVec.begin();
		itr != m_dofVec.end();
		++itr )
	{
		(*itr)->computeLocalToWorldMatrix(m_rotmat,NULL);
	}
	//CRCore::notify(CRCore::ALWAYS)<<"computerRotMatrix "<<m_rotmat<<std::endl;

	//m_rotmat.setTrans(0.0f,0.0f,0.0f);
}

CRCore::crVector3f crMissileSetOrFlackMatterObject::getDirection()
{//
	crMatrix bodyMatrix = m_rotmat * m_matrix;
	bodyMatrix.setTrans(0.0f,0.0f,0.0f);
	return (Y_AXIS * bodyMatrix).normalize();
}

CRCore::crMatrix crMissileSetOrFlackMatterObject::getCameraMatrix()const
{
	crMatrix bodyMatrix = m_rotmat * m_matrix;
	bodyMatrix.setTrans(0.0f,0.0f,0.0f);
	CRCore::crVector3f dir = (Y_AXIS*bodyMatrix).normalize();
	CRCore::crVector3f up = (Z_AXIS*bodyMatrix).normalize();
	CRCore::crVector3f pos = m_matrix.getTrans() + dir * m_viewOffset;
	pos[2] += m_viewHeight;	
	return crMatrix::lookAt(pos,pos-dir,up);
}

void crMissileSetOrFlackMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			crMatterObject::traverse(nv);

			//double referenceTime = nv.getFrameStamp()->getReferenceTime();
			//m_frameInterval = (m_previousFrameTime != 0.0) ? (referenceTime-m_previousFrameTime) : 0.0;
			//m_previousFrameTime = referenceTime;

			//if(m_physicsSwapInterval>0.0f)
			//	m_physicsSwapInterval -= m_frameInterval;
			//else if(m_physicsSwapInterval < 0.0f)
			//{
			//	physicsSwap();
			//	m_physicsSwapInterval = 0.0f;
			//}
			//if(!getEnableIntersect() || isDead()) return;

			if(m_cameraSwaped)
				m_cameraSwaped = false;
			else
				swapToFutureMatrix();

			//CRPhysics::crMatterObject::traverse(nv);

			if(getVisiable())
			{
				computerRotMatrix();
				//CRCore::notify(CRCore::ALWAYS)<<nv.getFrameStamp()->getFrameNumber()<<"crViewMatterObject::traverse(): m_position = "<<m_matrix.getTrans()<<std::endl;
				//updateWeaponMatrix();
			}
		}
		return;
	case CRCore::crNodeVisitor::INIT_VISITOR:
		{
			//if(nv.getTraverseString().compare("RenderToTexture") == 0)
			//{
			//	return;
			//}
			m_futureMatrix = m_matrix;
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

			s_findDofTransformVisitor.setBody(this);
			this->accept(s_findDofTransformVisitor);
		}

		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}


//void crMissileSetOrFlackMatterObject::updateWeaponMatrix()
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
//		m_currentWeaponMatterObject->clearWeaponMatrix();
//		bool added = false;
//		crMatrix bodyMatrix1 = m_rotmat * m_matrix;
//		crMatrix bodyMatrix2 = m_futureMatrix * m_matrix;
//		for( ; itr != m_weaponRackMap.end() && itr->first.compare(rackName) == 0;
//			++itr )
//		{
//			matrixTransform = dynamic_cast<crMatrixTransform *>(itr->second.get());
//			if(matrixTransform)
//			{
//				weaponMatrix1 = matrixTransform->getMatrix() * bodyMatrix1;
//				weaponMatrix2 = matrixTransform->getMatrix() * bodyMatrix2;
//				//weaponPos = itr->second->getBound().center();
//				//newPos = weaponPos * bodyMatrix1;//m_futureMatrix;
//				//weaponMatrix1 = bodyMatrix1 * matrixTransform->getMatrix();
//				//weaponMatrix1.setTrans(newPos);
//
//				//newPos = weaponPos * bodyMatrix2;//m_futureMatrix;
//				//weaponMatrix2 = bodyMatrix2 * matrixTransform->getMatrix();
//				//weaponMatrix2.setTrans(newPos);
//			}
//			else
//			{
//				weaponPos = itr->second->getBound().center();
//				newPos = weaponPos * bodyMatrix1;//m_futureMatrix;
//				weaponMatrix1 = bodyMatrix1;//m_futureMatrix;
//				weaponMatrix1.setTrans(newPos);
//
//				newPos = weaponPos * bodyMatrix2;//m_futureMatrix;
//				weaponMatrix2 = bodyMatrix2;//m_futureMatrix;
//				weaponMatrix2.setTrans(newPos);
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
//		//m_currentWeaponMatterObject->setMatrix(m_matrix);
//	}
//}
//////////////////////////////////////////
//
//crBodyCaseMatterObject
//
////////////////////////////////////////////
crBodyCaseMatterObject::crBodyCaseMatterObject()
{
}

crBodyCaseMatterObject::crBodyCaseMatterObject(const crBodyCaseMatterObject &matterObject,const CRCore::crCopyOp& copyop):
crViewMatterObject(matterObject,copyop)
{
}

void crBodyCaseMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			//double referenceTime = nv.getFrameStamp()->getReferenceTime();
			//m_frameInterval = (m_previousFrameTime != 0.0) ? (referenceTime-m_previousFrameTime) : 0.0;
			//m_previousFrameTime = referenceTime;

			//if(m_physicsSwapInterval>0.0f)
			//	m_physicsSwapInterval -= m_frameInterval;
			//else if(m_physicsSwapInterval < 0.0f)
			//{
			//	physicsSwap();
			//	m_physicsSwapInterval = 0.0f;
			//}
			crMatterObject::traverse(nv);

			bool dead = true;
			for( BodyRackMap::iterator itr = m_bodyRackMap.begin();
				 itr != m_bodyRackMap.end();
				 ++itr )
			{
				if(!itr->second->isDead()) 
				{
					dead = false;
					break;
				}
			}
			if(dead) kill(0);

			//if(!getEnableIntersect() || isDead()) return;

			//m_lastPos = m_matrix.getTrans();
			//m_futureMatrix = m_matrix;
			if(m_cameraSwaped)
				m_cameraSwaped = false;
			else
				swapToFutureMatrix();

			//if(getVisiable())
			{
				move();
				doHPR();
				//CRCore::notify(CRCore::ALWAYS)<<nv.getFrameStamp()->getFrameNumber()<<"crViewMatterObject::traverse(): m_position = "<<m_matrix.getTrans()<<std::endl;
				updateBodyMatrix();

				//getGameBodyInfo()->updateLockedTarget(getTargetCount());
				if(m_velocity[0] <= getGameBodyInfo()->getStallVelocity())
				{
					doEvent(WCH_STALL);
				}
				else
				{
					doEvent(WCH_UNSTALL);
				}

				if(!m_isNetBot && m_radarMatterObject.valid())
				{
					crVector3 botPos = m_matrix.getTrans();
					m_radarMatterObject->updatePositionAndDirection(botPos,botPos, -getViewDirection());
					m_radarMatterObject->setGndOffset(/*getBoundBox().zLength()*/m_physicsBox[2] * 0.5f);
				}
				//doEvent(WCH_UPDATE);
			}

			//if(m_currentBodyPhysicsType==RayDetect)
			//{
			//	dBodyDisable(m_bodyID);
			//}
			//else
			//{
			//	dBodyEnable(m_bodyID);
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
	case CRCore::crNodeVisitor::INIT_VISITOR:
		{
			//if(nv.getTraverseString().compare("RenderToTexture") == 0)
			//{
			//	return;
			//}
			m_futureMatrix = m_matrix;
			if(m_radarMatterObject.valid())
				(dynamic_cast<crMatterGroup *>(m_radarMatterObject->getParent(0)))->attachEventGroup((dynamic_cast<crMatterGroup *>(this->getParent(0)))->getAttachedEventGroup());
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			crNode *node;
			//crMissileSetOrFlackMatterObject *bot;
			for( BodyRackNameMap::iterator itr = m_bodyRackNameMap.begin();
				itr != m_bodyRackNameMap.end();
				++itr )
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(itr->first);
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				accept(searchByNameVisitor);
				node = searchByNameVisitor.getResult();
				//delete crBotManager 20110807
				//if(node)
				//{
				//	bot = dynamic_cast<crMissileSetOrFlackMatterObject *>(crBotManager::getInstance()->getFirstPlayer());
				//	if(bot && bot->getName().compare(itr->second) == 0)
				//	{
				//		m_bodyRackMap[node] = bot;
				//		(dynamic_cast<crMatterGroup *>(bot->getParent(0)))->setParentSpace(dynamic_cast<crMatterGroup *>(this->getParent(0)));
				//	}
				//	else
				//	{
				//		bot = dynamic_cast<crMissileSetOrFlackMatterObject *>(crBotManager::getInstance()->getBot(itr->second));
				//		if(bot)
				//		{
				//			m_bodyRackMap[node] = bot;
				//			//bot->setBodyCaseParent(dynamic_cast<crMatterGroup *>(this->getParent(0)));
				//			(dynamic_cast<crMatterGroup *>(bot->getParent(0)))->setParentSpace(dynamic_cast<crMatterGroup *>(this->getParent(0)));
				//		}
				//	}
				//}
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crBodyCaseMatterObject init "<<std::endl;
		}
		break;
	}
	CRPhysics::crMatterObject::traverse(nv);
}

void crBodyCaseMatterObject::insertBodyRackName(const std::string &rackName, const std::string &bodyName)
{
	m_bodyRackNameMap.insert(std::make_pair(rackName,bodyName));
}

void crBodyCaseMatterObject::updateBodyMatrix()
{
	//CRCore::notify(CRCore::ALWAYS)<<"crBodyCaseMatterObject::updateBodyMatrix(): "<<m_bodyRackMap.size()<<std::endl;

	crMatrix bodyMatrix;
	crVector3 position;
	for( BodyRackMap::iterator itr = m_bodyRackMap.begin(); 
		itr != m_bodyRackMap.end();
		++itr )
	{
		position = itr->first->getBound().center();
		position = position * m_matrix;
		bodyMatrix = m_matrix;
		bodyMatrix.setTrans(position);
		//CRCore::notify(CRCore::ALWAYS)<<"crBodyCaseMatterObject::updateBodyMatrix(): "<<itr->second->className()<<std::endl;
		itr->second->setFutureMatrix(bodyMatrix);
	}
}