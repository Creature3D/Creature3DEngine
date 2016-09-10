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
#include <CRPhysics/crCharacterMatterObject.h>
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crFilterRenderManager.h>

using namespace CRCore;
using namespace CRPhysics;

crCharacterMatterObject::crCharacterMatterObject():
  m_blockGeomID(0),
  m_blockRadius(0.35f)
{
	m_groundDetect = true;
	m_viewHeight = 1.6f;
}

crCharacterMatterObject::crCharacterMatterObject(const crCharacterMatterObject& matterObject,const crCopyOp& copyop):
  crViewMatterObject(matterObject,copyop),
  m_blockGeomID(0),
  m_blockRadius(matterObject.m_blockRadius)
{
}

void crCharacterMatterObject::setBlockRadius(float blockRadius)
{
    m_blockRadius = blockRadius;
}

//void crCharacterMatterObject::setBlockHeight(float blockHeight)
//{
//    m_blockHeight = blockHeight;
//}

void crCharacterMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	///////////////////////////////////////////////////////
	//m_sphereGeomID = dCreateSphere(spaceID,m_blockRadius);
	//crOdeWorld::getDynamicGeomMap()[m_sphereGeomID] = this;
	//m_viewHeight = m_bbox.zLength() * 0.5f - m_blockRadius;
	//if(m_enableIntersect)
	//{
	//	dGeomEnable(m_sphereGeomID);
	//}
	//else
	//{
	//	dGeomDisable(m_sphereGeomID);
	//}

	m_blockGeomID = dCreateCCylinder(spaceID,m_blockRadius,(m_physicsBox[2]-m_walkableHeight) * 0.5f);
	crOdeWorld::dynamicGeoMapInsert(m_blockGeomID,this);
	dGeomDisable(m_blockGeomID);
	crViewMatterObject::_create(worldID,spaceID);
}

void crCharacterMatterObject::releaseOde()
{
	if(m_blockGeomID)
	{
		dGeomDestroy(m_blockGeomID);
	    crOdeWorld::dynamicGeoMapErase(m_blockGeomID);
		m_blockGeomID = NULL;
	}

	crViewMatterObject::releaseOde();
}

void crCharacterMatterObject::updateOdeMatrix()
{
	setDummyMatrix();
	CRCore::crVector3 pos = m_futureMatrix.getTrans();

	dMatrix3 r;
	if(m_bodyID)
	{
		///////////////////////
		//if(	m_physicsBox[2] != m_viewHeight)
		//{
		//	m_physicsBox[2] = m_viewHeight;
		//	dGeomBoxSetLengths(m_geomID,m_physicsBox[0],m_physicsBox[1],m_physicsBox[2]);
		//	dMass newMass;
		//	dMassSetBoxTotal (&newMass,m_mass,m_physicsBox[0],m_physicsBox[1],m_physicsBox[2]);
		//	dMassTranslate (&newMass,m_barycenter[0],m_barycenter[1],m_barycenter[2]);
		//	dBodySetMass (m_bodyID,&newMass);
		//}

		//dGeomSetPosition (m_geomID,pos[0],pos[1],pos[2]);
		dBodySetPosition (m_bodyID,pos[0],pos[1],pos[2]);
	}

	float length =m_physicsBox[2] - m_walkableHeight - m_blockRadius * 2.0f;
	if(length<=0.0f)
		length = 0.01f;
	if(m_blockRadius>=0.0f)
	{
		dGeomCCylinderSetParams(m_blockGeomID,m_blockRadius,length);
	}
	dGeomSetPosition(m_blockGeomID,pos[0],pos[1],pos[2] + length * 0.5f + m_blockRadius + m_walkableHeight - m_physicsBox[2] * 0.5f);//

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

	//float height = getBoundBox().zLength();
	//float height = /*getBoundBox().zLength()*/m_physicsBox[2] * 0.5f;

	//if(m_currentWeaponMatterObject.valid() && getVisiable())
	//{
	//	const crBoundingBox &bbox = m_currentWeaponMatterObject->getBoundBox();
	//	crVector3 center = bbox.center();
	//	if(!m_weaponGeomID)
	//	{
	//		dSpaceID spaceID = dGeomGetSpace(m_blockGeomID);
	//		m_weaponGeomID = dCreateBox(spaceID,bbox.xLength(),bbox.yLength(),bbox.zLength());
	//		crOdeWorld::getDynamicGeomMap()[m_weaponGeomID] = this;
	//	}
	//	else
	//	{
	//		dGeomBoxSetLengths(m_weaponGeomID,bbox.xLength(),bbox.yLength(),bbox.zLength());
	//	}
	//	dGeomSetPosition(m_weaponGeomID,center[0],center[1],center[2]);
	//	if(m_enableIntersect) dGeomEnable(m_weaponGeomID);
	//}
	//else if(m_weaponGeomID) dGeomDisable(m_weaponGeomID);

	m_matrix_buf = m_futureMatrix;

	m_collided = false;
}

void crCharacterMatterObject::setEnableIntersect( bool enableIntersect)
{
	crViewMatterObject::setEnableIntersect(enableIntersect);
}

void crCharacterMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_blockGeomID)
			{
				if((int)m_enableIntersect^dGeomIsEnabled(m_blockGeomID))
				{
					if(m_enableIntersect)
					{
						dGeomEnable(m_blockGeomID);
					}
					else
					{
						dGeomDisable(m_blockGeomID);
					}
				}
			}
		}
		break;
	}

	crViewMatterObject::traverse(nv);
}
//////////////////////////////////////
//
//crCharacterObjectNearCallback
//
/////////////////////////////////////
crCharacterObjectNearCallback::crCharacterObjectNearCallback():
m_friendDamage(false)
{
}
void crCharacterObjectNearCallback::setFriendDamage(bool friendDamage)
{
	m_friendDamage = friendDamage;
}
crCharacterObjectNearCallback *crCharacterObjectNearCallback::instance()
{
	static CRCore::ref_ptr< crCharacterObjectNearCallback > s_nearCallback = new crCharacterObjectNearCallback;
	return s_nearCallback.get();
}

void crCharacterObjectNearCallback::nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 )
{
	//CRCore::notify(CRCore::ALWAYS)<<"crCharacterObjectNearCallback"<< std::endl;

	ref_ptr<crNode> node1,node2;
	if(!crOdeWorld::isCollideValid(o1,o2,node1,node2)) return;
//try
//{
	//if(dynamic_cast<crBulletMatterObject *>(node1) || dynamic_cast<crBulletMatterObject *>(node2)) 	
	//{
	//	return;
	//}

	//bool handleCollideCallback = false;
	dContact contact[MAX_CONTACTS];
	int n = 0;

	crCharacterMatterObject *viewMO = 0;
	//CRCore::crNode *collideNode;
	dGeomID voGeomID = 0;
	dGeomID collideGeomID = 0;

	crMatterObject *mo1 = 0;
	crMatterObject *mo2 = 0;

	if(node1.valid()) mo1 = dynamic_cast<crMatterObject *>(node1.get());
	else crOdeWorld::getStaticGeomNode(o1,node1);
	if(node2.valid()) mo2 = dynamic_cast<crMatterObject *>(node2.get());
	else crOdeWorld::getStaticGeomNode(o2,node2);

	crCharacterMatterObject *vo1 = 0;
	crCharacterMatterObject *vo2 = 0;
	if(mo1) vo1 = dynamic_cast<crCharacterMatterObject *>(mo1);
	if(mo2) vo2 = dynamic_cast<crCharacterMatterObject *>(mo2);

	crBulletMatterObject *bullet1 = 0;
	crBulletMatterObject *bullet2 = 0;
	if(mo1&&!vo1) bullet1 = dynamic_cast<crBulletMatterObject *>(mo1);
	if(mo2&&!vo2) bullet2 = dynamic_cast<crBulletMatterObject *>(mo2);

	n = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
	if(n==0) return;
	int collideTest = 1;
	if(node1.valid()) 
	{
		node1->doEvent(WCH_COLLIDETEST,MAKEINT64(node2.get(),&collideTest));
		if(collideTest == 0) return;
	}
	if(node2.valid())
	{
		collideTest = 1;
		node2->doEvent(WCH_COLLIDETEST,MAKEINT64(node1.get(),&collideTest));
		if(collideTest == 0) return;
	}
	//handleCollideCallback = true;
	/*if(vo1 && vo2)
	{

	}
	else */if(vo1||vo2)
	{

		if(vo1)
		{/* && !dynamic_cast<crBulletMatterObject *>(mo2)*/
			viewMO = vo1;
			voGeomID = o1;
			collideGeomID = o2;
			//if(bullet2 && !m_friendDamage && bullet2->isFriendFire(vo1))
			//{//如果同伴发射的
			//	return;
			//}
			if(!bullet2 && vo1->getDetectMode(o1) == crCharacterMatterObject::DetectBlock)  vo1->collided();
		}
		else if(vo2)
		{/*&& !dynamic_cast<crBulletMatterObject *>(mo1)*/
			viewMO = vo2;
			voGeomID = o2;
			collideGeomID = o1;
			//if(bullet1 && !m_friendDamage && bullet1->isFriendFire(vo2))
			//{//如果同伴发射的
			//	return;
			//}
			if(!bullet1 && vo2->getDetectMode(o2) == crCharacterMatterObject::DetectBlock)  vo2->collided();
		}
		crCharacterMatterObject::DetectMode detectMode = viewMO->getDetectMode(voGeomID);
		if(detectMode == crCharacterMatterObject::DetectGeom)
		{
			//if(!(vo1&&vo2))//屏蔽角色间碰撞
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
			//if(viewMO->getNumBotPhysicsObject()>0) handleCollideCallback = false;
		}
		//else if(detectMode == crCharacterMatterObject::DetectBlock)
		//{////
		//	handleCollideCallback = false;
		//}
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
			node1->doEvent(WCH_COLLIDE,MAKEINT64(node2.get(),&contact[0]));
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
			node2->doEvent(WCH_COLLIDE,MAKEINT64(node1.get(),&contact[0]));
			callback = node2->getCollideCallback();
			if (callback) (*callback)(node2.get(),node1.get(),o2,o1,contactPos,-contactNormal);
		}
	//}
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"crCharacterObjectNearCallback::s_nearCallback node1 = "<<node1->getName()<<" node2 = "<<node2->getName()<<std::endl;
	//}
}