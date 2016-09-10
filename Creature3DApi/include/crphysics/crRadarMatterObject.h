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
#ifndef CRPHYSICS_CRRADARMATTEROBJECT_H
#define CRPHYSICS_CRRADARMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRCore/crMath.h>
#include <CRCore/crNode.h>
#include <CRCore/thread/crMutex.h>
//#define MAXDISTANCE 50.0f
	
namespace CRPhysics{

class CRPHYSICS_EXPORT crRadarMatterObject : public crMatterObject
{
public:
	crRadarMatterObject();
	crRadarMatterObject(const crRadarMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crRadarMatterObject);

	virtual void releaseOde();
	inline void updatePositionAndDirection(const CRCore::crVector3 &radarpos, const CRCore::crVector3 &botpos, const CRCore::crVector3 &dir)
	{ m_radarPos = radarpos; m_botPos = botpos; m_direction = dir; }
	virtual void updateCreMatrix();
	virtual void updateOdeMatrix();

	class CRPHYSICS_EXPORT ContactInfo : public CRCore::Referenced
	{
	public:
		ContactInfo(float distance):m_distance(distance){}
		inline ContactInfo& operator = (const ContactInfo& info) { m_distance=info.m_distance; m_contactPos=info.m_contactPos; m_contactNode=info.m_contactNode; return (*this); }
		float                m_distance;
		CRCore::crVector3f   m_contactPos;
		CRCore::crVector3f   m_contactNormal;
		CRCore::ref_ptr<CRCore::crNode> m_contactNode;
	};
	void resetContactInfo();
	void updateContactInfo(dGeomID id, float distance, const CRCore::crVector3f &contactPos, const CRCore::crVector3f &contactNormal, crNode *contactNode);
	//void getContactInfo(CRCore::ref_ptr<ContactInfo> *contactInfo);
	
	inline void getDirection(CRCore::crVector3 &direction){ direction = m_direction; }
    inline void getRadarPosition(CRCore::crVector3 &position){ position = m_radarPos; }
	inline void getBotPosition(CRCore::crVector3 &position){ position = m_botPos; }
	
	void setBodySpaceID(dSpaceID bodySpaceID);
	inline dSpaceID getBodySpaceID() { return m_bodySpaceID; }

	typedef std::vector<CRCore::crVector3f> AngleVec;
    typedef std::vector<dGeomID> PhysicsGeomVec;
	typedef std::vector< CRCore::ref_ptr<ContactInfo> > ContactInfoVec;
    
    void setDetectRaySizeTo(int size,float detectDistance,float gndDetectDistance);
	inline int getDetectRaySize() const { return m_angles.size(); }
	inline float getDetectDistance() const { return m_detectDistance; }
	inline float getGndDetectDistance() const { return m_gndDetectDistance; }

	inline void getContactInfo(ContactInfoVec& contactInfos)
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_swapBufMutex);
		contactInfos = *m_contactInfos_buf;
	}
	//inline void unLockSwapBufMutex() { m_swapBufMutex.unlock(); }
	virtual void setEnableIntersect( bool enableIntersect);

	bool avoidCollide(int envmask,CRCore::crVector2& turnAngle);
	int getFrontRayID();

	float getGndDetectHeight();
	CRCore::crNode* getGndDetectNode();
	float getFrontDetectDistance();
	CRCore::crNode* getFrontDetectNode();
	const CRCore::crVector3f& getFrontDetectPos();
	const CRCore::crVector3f& getFrontDetectNormal();

	virtual void traverse(CRCore::crNodeVisitor& nv);

	inline void setGndOffset(float offset){ m_gndOffset = offset; }
	inline float getGndOffset(){ return m_gndOffset; }
	void setGroundDetect(bool groundDetect);
protected:
	virtual ~crRadarMatterObject();
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
	CRCore::crVector3 m_direction;
	CRCore::crVector3 m_radarPos;
	CRCore::crVector3 m_botPos;
	mutable PhysicsGeomVec   m_radarGeomIDs;
	mutable dGeomID m_gndGeomID;
	AngleVec m_angles;
	ContactInfoVec *m_contactInfos;
	ContactInfoVec *m_contactInfos_buf;

	float m_detectDistance;
	float m_gndDetectDistance;
	float m_gndOffset;
	int m_frontRayID;
	bool        m_groundDetect;
	dSpaceID  m_bodySpaceID;
	CRCore::crMutex m_swapBufMutex;
};

class CRPHYSICS_EXPORT crRadarObjectNearCallback : public crMatterGroup::NearCallback
{
public:
	virtual crRadarObjectNearCallback* cloneType() const { return NULL; };
	virtual const char* libraryName() const { return "CRPhysics"; }
	virtual const char* className() const { return "crRadarObjectNearCallback"; }

	virtual void nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 );

	static crRadarObjectNearCallback* instance();
};

}

#endif