/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRPhysics/crRadarMatterObject.h>
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crCharacterMatterObject.h>
#include <CRCore/crArgumentParser.h>

using namespace CRCore;
using namespace CRPhysics;

crRadarMatterObject::crRadarMatterObject():
m_detectDistance(500.0f),
m_gndDetectDistance(1000.0f),
m_frontRayID(0),
//m_gndDetectHeight(0.0f),
//m_frontDetectDistance(0.0f),
m_gndGeomID(0),
m_gndOffset(0),
m_groundDetect(false)
{
	m_contactInfos = new ContactInfoVec;
	m_contactInfos_buf = new ContactInfoVec;
	m_direction = CRCore::Y_AXIS;
}

crRadarMatterObject::~crRadarMatterObject()
{
	delete m_contactInfos;
	delete m_contactInfos_buf;
}

crRadarMatterObject::crRadarMatterObject(const crRadarMatterObject& matterObject,const crCopyOp& copyop):
crMatterObject(matterObject,copyop),
m_detectDistance(matterObject.m_detectDistance),
m_gndDetectDistance(matterObject.m_gndDetectDistance),
m_frontRayID(matterObject.m_frontRayID),
m_groundDetect(matterObject.m_groundDetect)
{
	setDetectRaySizeTo(m_angles.size(),m_detectDistance,m_gndDetectDistance);
    m_direction = CRCore::Y_AXIS;
}

void crRadarMatterObject::setGroundDetect(bool groundDetect)
{
	m_groundDetect = groundDetect;
}

void crRadarMatterObject::setDetectRaySizeTo(int size,float detectDistance, float gndDetectDistance)
{
	m_detectDistance = detectDistance;
	m_angles.reserve(size);
	m_contactInfos->reserve(size+1);
	m_contactInfos_buf->reserve(size+1);
	switch (size)
	{
	case 1:
		m_angles.push_back(crVector3f(0.0f,0.0f,0.0f));
		m_frontRayID = 0;
		break;
	case 3:
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,0.0f));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 10.0f)));
		m_frontRayID = 1;
		break;
	case 5:
		m_angles.push_back(crVector3f(0.0f,DegreesToRadians(-10.0f),0.0f));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,0.0f));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 10.0f)));
		m_angles.push_back(crVector3f(0.0f,DegreesToRadians(10.0f),0.0f));
		m_frontRayID = 2;
		break;
	case 9:
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-90.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-50.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-30.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,0.0f));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 30.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 50.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 90.0f)));
		m_frontRayID = 4;
		break;
	case 12:
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-90.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-50.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-30.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians(-10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,0.0f));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 10.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 30.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 50.0f)));
		m_angles.push_back(crVector3f(0.0f,0.0f,DegreesToRadians( 90.0f)));

		m_angles.push_back(crVector3f(0.0f,DegreesToRadians(70.0f),0.0f));
		m_angles.push_back(crVector3f(0.0f,DegreesToRadians(-50.0f),0.0f));
		m_angles.push_back(crVector3f(0.0f,DegreesToRadians(-70.0f),0.0f));
		m_frontRayID = 4;
		break;
	}

	for(int i = 0; i<size+1; i++)
	{
		(*m_contactInfos).push_back(new crRadarMatterObject::ContactInfo(m_detectDistance));
		(*m_contactInfos_buf).push_back(new crRadarMatterObject::ContactInfo(m_detectDistance));
	}
}

int crRadarMatterObject::getFrontRayID()
{
    return m_frontRayID;
}

bool crRadarMatterObject::avoidCollide(int envmask,CRCore::crVector2& turnAngle)
{
	turnAngle.set(0.0f,0.0f);
	switch (getDetectRaySize())
	{
	case 3:
		if(envmask & 1<<1)
		{
			if(!(envmask & 1<<2))
			{//
				turnAngle[0] = m_angles[2][2];
			}
			else/* if(!(envmask & 1<<0))*/
			{
				turnAngle[0] = m_angles[0][2];
			}
			return true;
		}
		return false;
		break;
	case 5:
		if(envmask & 1<<2)
		{
			if(!(envmask & 1<<1))
			{//
				turnAngle[0] = m_angles[1][2];
			}
			else if(!(envmask & 1<<3))
			{
				turnAngle[0] = m_angles[3][2];
			}
			if(!(envmask & 1))
			{//
				turnAngle[1] = m_angles[0][1];
			}
			else if(!(envmask & 1<<4))
			{
				turnAngle[1] = m_angles[4][1];
			}
			if(turnAngle == crVector2(0.0f,0.0f))
				turnAngle[1] = m_angles[4][1];

			return true;
		}
		return false;
		break;
	case 9:
		if(envmask & 1<<4)
		{
			turnAngle[1] = 0.0f;
			if(!(envmask & 1<<5))
			{//
				turnAngle[0] = m_angles[5][2];
				//CRCore::notify(CRCore::FATAL)<<"5 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<3))
			{
				turnAngle[0] = m_angles[3][2];
				//CRCore::notify(CRCore::FATAL)<<"3 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<6))
			{
				turnAngle[0] = m_angles[6][2];
				//CRCore::notify(CRCore::FATAL)<<"6 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<2))
			{
				turnAngle[0] = m_angles[2][2];
				//CRCore::notify(CRCore::FATAL)<<"2 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<7))
			{
				turnAngle[0] = m_angles[7][2];
				//CRCore::notify(CRCore::FATAL)<<"7 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<1))
			{
				turnAngle[0] = m_angles[1][2];
				//CRCore::notify(CRCore::FATAL)<<"1 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else if(!(envmask & 1<<8))
			{
				turnAngle[0] = m_angles[8][2];
				//CRCore::notify(CRCore::FATAL)<<"8 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			else/*(!(m_envmask & 1))*/
			{
				turnAngle[0] = m_angles[0][2];
				//CRCore::notify(CRCore::FATAL)<<"0 m_turn_angle[0] = "<<m_turn_angle[0]<<std::endl;
			}
			return true;
		}
		return false;
		break;
	case 12:
		break;
	}
	return false;
}

void crRadarMatterObject::_create(dWorldID worldID, dSpaceID spaceID)
{
	//int i;
	int size = getDetectRaySize();
	m_radarGeomIDs.reserve(size);
	for(int i = 0; i<size; i++)
	{
		m_radarGeomIDs.push_back(dCreateRay(spaceID,m_detectDistance));
        //crOdeWorld::getDynamicGeomMap()[m_radarGeomIDs[i]] = this;
		crOdeWorld::dynamicGeoMapInsert(m_radarGeomIDs[i],this);
	}
	if(m_groundDetect)
	{
		m_gndGeomID = dCreateRay(spaceID,m_gndDetectDistance);
		//crOdeWorld::getDynamicGeomMap()[m_gndGeomID] = this;
		crOdeWorld::dynamicGeoMapInsert(m_gndGeomID,this);
	}

	//for(i = 9; i<12; i++)
	//{
	//	m_radarGeomID[i] = dCreateRay(spaceID,5);
	//	crOdeWorld::getDynamicGeomMap()[m_radarGeomID[i]] = this;
	//}
////创建的时候默认将射线disable
	if(m_gndGeomID)
	{
		if(m_enableIntersect)
			dGeomEnable(m_gndGeomID);
		else
			dGeomDisable(m_gndGeomID);
	}

	for( PhysicsGeomVec::iterator itr = m_radarGeomIDs.begin();
		itr != m_radarGeomIDs.end();
		++itr )
	{
		if(*itr)
		{
			if(m_enableIntersect)
				dGeomEnable(*itr);
			else
				dGeomDisable(*itr);
		}
	}
}

void crRadarMatterObject::releaseOde()
{
	//CRCore::notify(CRCore::ALWAYS)<<"crRadarMatterObject::releaseOde()"<< std::endl;
	for( PhysicsGeomVec::iterator itr = m_radarGeomIDs.begin();
		itr != m_radarGeomIDs.end();
		++itr )
	{
		if(*itr)
		{
			dGeomDestroy(*itr);
			crOdeWorld::dynamicGeoMapErase(*itr);
		}
	}
	m_radarGeomIDs.clear();
	if(m_gndGeomID)
	{
		dGeomDestroy(m_gndGeomID);
		crOdeWorld::dynamicGeoMapErase(m_gndGeomID);
		m_gndGeomID = NULL;
	}
	m_physicsInited = false;
	//CRCore::notify(CRCore::ALWAYS)<<"crRadarMatterObject::releaseOde() end"<< std::endl;
}

void crRadarMatterObject::setBodySpaceID(dSpaceID bodySpaceID)
{
    m_bodySpaceID = bodySpaceID;
}

void crRadarMatterObject::setEnableIntersect( bool enableIntersect)
{
	crMatterObject::setEnableIntersect(enableIntersect);
}

void crRadarMatterObject::traverse(CRCore::crNodeVisitor& nv)
{
	switch(nv.getVisitorType()) 
	{
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crRadarMatterObject::traverse() "<<m_enableIntersect<< std::endl;
			if(m_gndGeomID)
			{
				if((int)m_enableIntersect^dGeomIsEnabled(m_gndGeomID))
				{
					if(m_enableIntersect)
						dGeomEnable(m_gndGeomID);
					else
						dGeomDisable(m_gndGeomID);
				}
			}

			for( PhysicsGeomVec::iterator itr = m_radarGeomIDs.begin();
				itr != m_radarGeomIDs.end();
				++itr )
			{
				if(*itr)
				{
					if((int)m_enableIntersect^dGeomIsEnabled(*itr))
					{
						if(m_enableIntersect)
							dGeomEnable(*itr);
						else
							dGeomDisable(*itr);
					}
				}
			}
		}
		break;
	}

	crMatrixTransform::traverse(nv);
}

void crRadarMatterObject::updateCreMatrix()
{//sendmessage to ai
	CRCore::ScopedLock<CRCore::crMutex> lock(m_swapBufMutex);

	//crMatterGroup *mg = dynamic_cast<crMatterGroup *>(getParent(0));
	//if(mg)
	//{
	//	mg->sendEventMessage(mg->getAttachedEventGroup(),WCH_VISIONSENSEINFORMATIONPARSE,MAKEINT64(this,NULL),NULL);
	//}
	
	//ContactInfo *gndInfo = (*m_contactInfos)[getDetectRaySize()].get();
	//m_gndDetectHeight = gndInfo->m_distance;
 //   m_gndDetectNode = gndInfo->m_contactNode;
	//m_frontDetectDistance = (*m_contactInfos)[getFrontRayID()]->m_distance;

	ContactInfoVec *temp = m_contactInfos_buf;
	m_contactInfos_buf = m_contactInfos;
	m_contactInfos = temp;
}

float crRadarMatterObject::getGndDetectHeight()
{//Ray探测到离地面高度
	if(m_groundDetect)
	{
		ContactInfo *gndInfo = (*m_contactInfos_buf)[getDetectRaySize()].get();
		return gndInfo->m_distance;
	}
	else
	{
		return 0.0f;
	}
}

crNode* crRadarMatterObject::getGndDetectNode()
{//Ray探测到离正前方障碍物
	ContactInfo *gndInfo = (*m_contactInfos_buf)[getDetectRaySize()].get();
	return gndInfo->m_contactNode.get(); 
}

float crRadarMatterObject::getFrontDetectDistance()
{//Ray探测到离正前方障碍物距离
	ContactInfo *frontInfo = (*m_contactInfos_buf)[getFrontRayID()].get();
	return frontInfo->m_distance; 
}

crNode* crRadarMatterObject::getFrontDetectNode()
{//Ray探测到离正前方障碍物距离
	ContactInfo *frontInfo = (*m_contactInfos_buf)[getFrontRayID()].get();
	return frontInfo->m_contactNode.get(); 
}

const crVector3f& crRadarMatterObject::getFrontDetectPos()
{//Ray探测到离正前方障碍物距离
	ContactInfo *frontInfo = (*m_contactInfos_buf)[getFrontRayID()].get();
	return frontInfo->m_contactPos; 
}

const crVector3f& crRadarMatterObject::getFrontDetectNormal()
{//Ray探测到离正前方障碍物距离
	ContactInfo *frontInfo = (*m_contactInfos_buf)[getFrontRayID()].get();
	return frontInfo->m_contactNormal; 
}

void crRadarMatterObject::updateOdeMatrix()
{
	//if(!m_physicsInited||!getVisiable()) return;

	//CRCore::notify(CRCore::ALWAYS)<<"crRadarMatterObject::updateOdeMatrix() begin"<< std::endl;

	crVector3 dir;
	CRCore::crMatrixd rot_matrix;

	int size = getDetectRaySize();
	for(int i=0; i<size; ++i)
	{
		rot_matrix.makeRotate(m_angles[i][0],X_AXIS,m_angles[i][1],Y_AXIS,m_angles[i][2],Z_AXIS);
		dir = m_direction * rot_matrix;
		//dir.normalize();
		//CRCore::notify(CRCore::FATAL)<<" crRadarMatterObject::updateOdeMatrix i = "<<i<<"dir = "<<dir<<std::endl;
		dGeomRaySet(m_radarGeomIDs[i],m_radarPos[0],m_radarPos[1],m_radarPos[2],dir[0],dir[1],dir[2]);
	}

	if(m_gndGeomID) dGeomRaySet(m_gndGeomID,m_botPos[0],m_botPos[1],m_botPos[2],0.0,0.0,-1.0);

	resetContactInfo();

	//CRCore::notify(CRCore::ALWAYS)<<"crRadarMatterObject::updateOdeMatrix() end"<< std::endl;
}

void crRadarMatterObject::resetContactInfo()
{
	int size = getDetectRaySize();
	for(int i = 0; i<size; i++)
	{
        (*m_contactInfos)[i]->m_distance = m_detectDistance;
		(*m_contactInfos)[i]->m_contactNode = NULL;
	}
	(*m_contactInfos)[size]->m_distance = m_gndDetectDistance;
	(*m_contactInfos)[size]->m_contactNode = NULL;
}

void crRadarMatterObject::updateContactInfo(dGeomID id, float distance, const CRCore::crVector3f &contactPos, const CRCore::crVector3f &contactNormal, crNode *contactNode)
{
	int size = getDetectRaySize();
	if(m_gndGeomID == id)
	{
		if(!dynamic_cast<crCharacterMatterObject *>(contactNode))
		{//角色与角色之间不阻挡
			distance -= m_gndOffset;
			if(distance < (*m_contactInfos)[size]->m_distance)
			{
				(*m_contactInfos)[size]->m_distance = distance;
				(*m_contactInfos)[size]->m_contactPos = contactPos;
				(*m_contactInfos)[size]->m_contactNormal = contactNormal;
				(*m_contactInfos)[size]->m_contactNode = contactNode;
			}
		}
	}
	else
	{
		for(int i = 0; i<size; i++)
		{
			if(m_radarGeomIDs[i] == id)
			{
				//CRCore::notify(CRCore::FATAL)<<"crRadarMatterObject::updateContactInfo i = "<<i<<"distance = "<<distance<<std::endl;

				if(distance < (*m_contactInfos)[i]->m_distance)
				{
					(*m_contactInfos)[i]->m_distance = distance;
					(*m_contactInfos)[i]->m_contactPos = contactPos;
					(*m_contactInfos)[i]->m_contactNormal = contactNormal;
					(*m_contactInfos)[i]->m_contactNode = contactNode;
				}
				break;
			}
		}
	}
}
//
//void crRadarMatterObject::getContactInfo(CRCore::ref_ptr<ContactInfo> *contactInfo)
//{
//	int size = getDetectRaySize();
//	for(int i = 0; i <size; i++)
//	{
//        **(contactInfo + i) = *(m_contactInfos[i]);
//		//CRCore::notify(CRCore::FATAL)<<"crRadarMatterObject::getContactInfo i = "<<i<<"distance = "<<m_contactInfos[i]->m_distance<<std::endl;
//	}
//}

//////////////////////////////////////
//
//crRadarObjectNearCallback
//
/////////////////////////////////////
crRadarObjectNearCallback *crRadarObjectNearCallback::instance()
{
	static CRCore::ref_ptr< crRadarObjectNearCallback > s_nearCallback = new crRadarObjectNearCallback;
	return s_nearCallback.get();
}

void crRadarObjectNearCallback::nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 )
{
	//CRCore::notify(CRCore::ALWAYS)<<"crRadarObjectNearCallback"<< std::endl;
	ref_ptr<crNode> node1,node2;
	if(!crOdeWorld::isCollideValid(o1,o2,node1,node2)) return;
	if (dGeomGetClass(o1) == dRayClass && dGeomGetClass(o2) == dRayClass) return;
	if (dGeomGetClass(o1) != dRayClass && dGeomGetClass(o2) != dRayClass) return;

	//crMatterGroup *This = static_cast<crMatterGroup *>(data);

	dGeomID radarGeomID = 0;
	dGeomID collideGeomID = 0;
	if(dGeomGetClass(o1) == dRayClass)
	{
		radarGeomID = o1;
        collideGeomID = o2;
	}
	else
	{
		radarGeomID = o2;
		collideGeomID = o1;
	}
	ref_ptr<crNode> radarNode;
	crOdeWorld::getDynamicGeomNode(radarGeomID,radarNode);
	crRadarMatterObject *radarMatterObject = dynamic_cast<crRadarMatterObject *>(radarNode.get());
	if(!radarMatterObject)
	{
		CRCore::notify(CRCore::WARN)<<"crRadarObjectNearCallback::s_nearCallback error"<< std::endl;
		return;
	}
    
	if(dGeomGetSpace(collideGeomID) == radarMatterObject->getBodySpaceID()) return;

	ref_ptr<crNode> collideNode;
	crOdeWorld::getDynamicGeomNode(collideGeomID,collideNode);
   // CRCore::crNode *collideNode = crOdeWorld::getDynamicGeomNode(collideGeomID);
	if(!collideNode.valid())
		crOdeWorld::getStaticGeomNode(collideGeomID,collideNode);

	dContact contact[MAX_CONTACTS];
	int n = 0;
	//try
	//{
	n = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::WARN)<<"crRadarObjectNearCallback collide error "<<std::endl;
	//}
	crVector3 startVec,contactVec,normalVec;
	dVector3 start,dir;
	float distance = radarMatterObject->getDetectDistance();//MAXDISTANCE;
	int mini = 0;
	dGeomRayGet(radarGeomID,start,dir);
	startVec.set(start[0],start[1],start[2]);
	for(int i = 0; i<n; i++)
	{
		if(contact[i].geom.depth < distance)
		{
            distance = contact[i].geom.depth;
            mini = i;
		}
	}
	contactVec.set(contact[mini].geom.pos[0],contact[mini].geom.pos[1],contact[mini].geom.pos[2]);
    normalVec.set(contact[mini].geom.normal[0],contact[mini].geom.normal[1],contact[mini].geom.normal[2]);
	radarMatterObject->updateContactInfo(radarGeomID,distance,contactVec,normalVec,collideNode.get());
}
