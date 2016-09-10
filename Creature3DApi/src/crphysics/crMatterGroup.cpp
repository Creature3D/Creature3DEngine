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
#include <CRPhysics\crMatterGroup.h>
#include <CRPhysics\crMatterObject.h>
#include <CRPhysics/crJointSetParamPPC.h>
#include <CRPhysics/crSetForceAndTorquePPC.h>
#include <CRPhysics/crSetGravityModePPC.h>
#include <CRPhysics\crViewMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crCharacterMatterObject.h>
#include <CRCore\crNotify.h>

using namespace CRCore;
using namespace CRPhysics;

crMatterGroup::NearCallbackMap crMatterGroup::m_nearCallbackMap;

crMatterGroup::NearCallback *crMatterGroup::getMatterGroupNearCallback( const std::string &mg1, const std::string &mg2 )
{
	//CRCore::notify(CRCore::ALWAYS)<<"crMatterGroup::getMatterGroupNearCallback() mg1 = "<<mg1<<" mg2 = "<<mg2<< std::endl;

	NearCallbackMap::iterator itr;

	if(mg1.compare("RadarCollideDetect")==0||mg2.compare("RadarCollideDetect")==0)
	{
		itr = m_nearCallbackMap.find("RadarCollideDetect");
		if( itr != m_nearCallbackMap.end() )
			return itr->second.get();	
	}

	if(mg1.compare("CharacterCollideDetect")==0||mg2.compare("CharacterCollideDetect")==0)
	{
		itr = m_nearCallbackMap.find("CharacterCollideDetect");
		if( itr != m_nearCallbackMap.end() )
			return itr->second.get();	
	}

	if(mg1.compare("ViewCollideDetect")==0||mg2.compare("ViewCollideDetect")==0)
	{
		itr = m_nearCallbackMap.find("ViewCollideDetect");
		if( itr != m_nearCallbackMap.end() )
			return itr->second.get();	
	}

	std::string str = mg1+mg2;
	
	itr = m_nearCallbackMap.find(str);
	if( itr != m_nearCallbackMap.end() )
		return itr->second.get();

    str = mg2+mg1;

	itr = m_nearCallbackMap.find(str);
	if( itr != m_nearCallbackMap.end() )
		return itr->second.get();

	str = mg1;

	itr = m_nearCallbackMap.find(str);
	if( itr != m_nearCallbackMap.end() )
		return itr->second.get();

	str = mg2;

	itr = m_nearCallbackMap.find(str);
	if( itr != m_nearCallbackMap.end() )
		return itr->second.get();

	return NULL;
}

void crMatterGroup::addNearCallback(const std::string& str, NearCallback *nc)
{
    m_nearCallbackMap[str] = nc;
}

crMatterGroup::crMatterGroup():
    m_spaceID(0),
	m_parentSpace(NULL),
	m_collideSpy(0)
{
	//addEventMsgMap(crJointSetParamPPC::makeEventMsg());
	//addEventMsgMap(crSetForceAndTorquePPC::makeEventMsg());
	//addEventMsgMap(crSetGravityModePPC::makeEventMsg());
}
crMatterGroup::~crMatterGroup()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crMatterGroup() "<<getName()<< std::endl;
}
crMatterGroup::crMatterGroup(const crMatterGroup& matterGroup,const crCopyOp& copyop):
crEventGroup(matterGroup,copyop),
m_matterGroupMaterialString(matterGroup.m_matterGroupMaterialString),
m_spaceID(0),
m_collideSpy(0),
m_parentSpace(0)
{
//不能克隆
}

void crMatterGroup::releaseObjects(CRCore::crState* state)
{
	//m_physicsInited = false;//必须在crEventGroup::releaseObjects(state)前设置
	crEventGroup::releaseObjects(state);
	//releaseOde();
}

void crMatterGroup::setParentSpace(crMatterGroup *parent)
{
    m_parentSpace = parent;
}

crMatterGroup *crMatterGroup::getParentSpace()
{
    return m_parentSpace;
}

void crMatterGroup::releaseOde()
{	
	if(!m_parentSpace)
	{
		if(m_spaceID)  
		{
			dSpaceDestroy (m_spaceID);
			crOdeWorld::dynamicGeoMapErase((dGeomID)m_spaceID);
			m_spaceID = NULL;
		}
	}
	else m_parentSpace = NULL;
	for( JointMap::iterator itr = m_joints.begin(); 
		 itr != m_joints.end();
		 ++itr )
	{
		itr->second->release();
	}
	m_odeWorld = NULL;
	m_physicsInited = false;
}

//void crMatterGroup::traverse(CRCore::crNodeVisitor& nv)
//{
//    //if(nv.getVisitorType() == CRCore::crNodeVisitor::UPDATE_VISITOR && !getVisiable()) return;
//	//if(nv.getVisitorType() == CRCore::crNodeVisitor::UPDATE_VISITOR)
//	//{
//	//	m_currentVisitor = &nv;
//
//	//	relayEvent();
//	//	if(m_childParam>=0&&m_childParam<m_childArray.size())
//	//		m_childArray[m_childParam]->accept(nv);
//	//	else
//	//	{
//	//		crGroup::traverse(nv);
//	//	}
//	//}
//	//else
//	    crEventGroup::traverse(nv);
//}

void crMatterGroup::createJoints(dWorldID worldID, dJointGroupID jointGroupID)
{
	for(JointMap::iterator itr = m_joints.begin();
		itr != m_joints.end();
		++itr)
	{
        itr->second->create(worldID, jointGroupID);
	}
}

bool crMatterGroup::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	CRCore::NodeArray::const_iterator itr;
	for(itr=m_childArray.begin();
		itr!=m_childArray.end();
		++itr)
	{
		m_bbox.expandBy((*itr)->getBoundBox());
	}

	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_boundSphere.expandBy(m_bbox);

	m_bBoundSphere_computed = true;
	return true;
}

void crMatterGroup::resetCollideSpy()
{
    m_collideSpy = 0;
}
void crMatterGroup::collideSpy( int n )
{
	m_collideSpy += n;
}
int crMatterGroup::getLastCollideSpy()const
{
	return m_collideSpy;
}

void crMatterGroup::collide(crMatterGroup &mg,dGeomID o1,dGeomID o2,bool needsearchnearcb)
{
	if(!dGeomIsEnabled(o1)||!dGeomIsEnabled(o2)) return;
	/*if(!getEnableIntersect()||!mg.getEnableIntersect())
		return;*/

    resetCollideSpy();
//搜寻匹配的nearCallback函数
    if(needsearchnearcb)
	{
        NearCallback* nearCallback = getMatterGroupNearCallback(m_matterGroupMaterialString, mg.m_matterGroupMaterialString);
    //装载nearCallback函数
	    setNearCallback(nearCallback);
	    mg.setNearCallback(nearCallback);
	}
    
	dSpaceCollide2(o1,o2,this,s_nearCallback);

	if(m_collideSpy>0)
	{//碰撞
        dealWithCollide(mg,true);
	}
}

void crMatterGroup::collide(dGeomID o1,dGeomID o2,bool needsearchnearcb)
{
	//if(!o1 || !o2) return;
	if(!dGeomIsEnabled(o1)||!dGeomIsEnabled(o2)) return;

	resetCollideSpy();

	//搜寻匹配的nearCallback函数
	if(needsearchnearcb)
	{
		NearCallback* nearCallback = getMatterGroupNearCallback(m_matterGroupMaterialString, "");
		//装载nearCallback函数
		setNearCallback(nearCallback);
	}
	dSpaceCollide2(o1,o2,this,s_nearCallback);

	if(m_collideSpy>0)
	{//碰撞
		dealWithCollide();
	}
}

void crMatterGroup::collide(crMatterObject *mo,dGeomID o1,dGeomID o2,bool needsearchnearcb)
{
	//if(!mo) return;
	//if(!getEnableIntersect()||!mo->getEnableIntersect())
	//	return;
	if(!dGeomIsEnabled(o1)||!dGeomIsEnabled(o2)) return;

	resetCollideSpy();
    if(needsearchnearcb)
	{
		//搜寻匹配的nearCallback函数
		NearCallback* nearCallback = getMatterGroupNearCallback(m_matterGroupMaterialString, "");
		//装载nearCallback函数
		setNearCallback(nearCallback);
	}
	dSpaceCollide2(o1,o2,this,s_nearCallback);

	if(m_collideSpy>0)
	{//碰撞
		dealWithCollide(mo);
	}
}

void crMatterGroup::dealWithCollide( crMatterGroup &mg, bool doCall )
{
//组合碰撞语义消息，将该语义消息发送给其神经元节点
	if(doCall)
	{
	    mg.dealWithCollide(*this,false);
	}
    
	//if(m_attachedEventGroup)
	//{
	//	//const dReal *temp;
	// //   //其第一个孩子是该mg的主体，只计算主体所受到的力和扭矩
	// //   dBodyID bodyID = getMainMatterObject()->getBodyID();
 // //      if(bodyID)
	//	//{
	//	//    temp = dBodyGetForce(bodyID);
	// //       m_force.set(*temp,*(temp+1),*(temp+2));
	// //       temp = dBodyGetTorque(bodyID);
	// //       m_torque.set(*temp,*(temp+1),*(temp+2));

	//	//	sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHMG,&mg),MAKEINT64(&m_force,&m_torque));
	//	//}
	//	//else
	//		//sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHMG,&mg),NULL);
	//}
}

void crMatterGroup::dealWithCollide( crMatterObject *mo )
{
	//if(m_attachedEventGroup)
	//{
	//	//const dReal *temp;
	//	////其第一个孩子是该mg的主体，只计算主体所受到的力和扭矩
	//	//dBodyID bodyID = getMainMatterObject()->getBodyID();
	//	//if(bodyID)
	//	//{
	//	//	temp = dBodyGetForce(bodyID);
	//	//	m_force.set(*temp,*(temp+1),*(temp+2));
	//	//	temp = dBodyGetTorque(bodyID);
	//	//	m_torque.set(*temp,*(temp+1),*(temp+2));

	//	//	sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHMO,mo),MAKEINT64(&m_force,&m_torque));
	//	//}
	//	//else
	//		//sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHMO,mo),NULL);
	//}
}

void crMatterGroup::dealWithCollide()
{
	//if(m_attachedEventGroup)
	//{
	//	//const dReal *temp;
	//	////其第一个孩子是该mg的主体，只计算主体所受到的力和扭矩
	//	//dBodyID bodyID = getMainMatterObject()->getBodyID();
	//	//if(bodyID)
	//	//{
	//	//	//temp = dBodyGetForce(bodyID);
	//	//	//m_force.set(*temp,*(temp+1),*(temp+2));
	//	//	//temp = dBodyGetTorque(bodyID);
	//	//	//m_torque.set(*temp,*(temp+1),*(temp+2));

	//	//	sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHEM,NULL),MAKEINT64(&m_force,&m_torque));
	//	//}
	//	//else
	//		//sendEventMessage(m_attachedEventGroup,WCH_COLLIDE,MAKEINT64(COLLIDEWITHEM,NULL),NULL);
	//}
}

crMatterObject *crMatterGroup::getMainMatterObject()
{
	crMatterObject *mainObject = dynamic_cast<crMatterObject *>(m_childArray[0].get());
    if(!mainObject)
		CRCore::notify(CRCore::NOTICE) <<"Warning: crMatterGroup::getMainMatterObject() mainMatterObject invalid"<<std::endl;

	return mainObject;
}

void crMatterGroup::s_nearCallback( void *data, dGeomID o1, dGeomID o2 )
{
	ref_ptr<crNode> node1,node2;
	if(!crOdeWorld::isCollideValid(o1,o2,node1,node2)) return;

	crMatterGroup *This = static_cast<crMatterGroup *>(data);

	if( This != 0L )
	{
		if(This->m_nearCallback.valid())
			This->m_nearCallback->nearCallbackImplementation( data, o1, o2 );
		else
		{
			dBodyID b1 = dGeomGetBody(o1);
			dBodyID b2 = dGeomGetBody(o2);
			if(b1) b1 = dBodyIsEnabled(b1)?b1:0;
            if(b2) b2 = dBodyIsEnabled(b2)?b2:0;

			dContact contact[MAX_CONTACTS];
			int n = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));

			if(n > 0) 
			{
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

				This->collideSpy(n);
                
				crMatterObject *mo1 = 0;
				crMatterObject *mo2 = 0;
				if(node1.valid()) mo1 = dynamic_cast<crMatterObject *>(node1.get());
				else crOdeWorld::getStaticGeomNode(o1,node1);
				if(node2.valid()) mo2 = dynamic_cast<crMatterObject *>(node2.get());
				else crOdeWorld::getStaticGeomNode(o2,node2);

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

				if ((b1 || b2) && !(b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)))
				{
					for(int i=0; i<n; i++) 
					{
						//contact[i].surface.mode = /*dContactMu2 |*/dContactSlip1 | dContactSlip2 | 
						//	/*dContactBounce |dContactSoftERP |*/  dContactSoftCFM | dContactApprox1;
						//contact[i].surface.mu = 0.5;
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
		}
	}
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"crMatterGroup::s_nearCallback error node1 = "<<node1->getName()<<" node2 = "<<node2->getName()<<std::endl;
	//}
	//crOdeWorld::insertConnectedExcluding(o1,o2);
}

void crMatterGroup::setEnableIntersect( bool enableIntersect )
{
    crEventGroup::setEnableIntersect(enableIntersect);
}

void crMatterGroup::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			if(m_spaceID)
			{
				if((int)m_enableIntersect^dGeomIsEnabled((dGeomID)(m_spaceID)))
				{
					if(m_enableIntersect)
						dGeomEnable((dGeomID)(m_spaceID));
					else
						dGeomDisable((dGeomID)(m_spaceID));
				}
			}
		}
		break;
	}

	crEventGroup::traverse(nv);
}
