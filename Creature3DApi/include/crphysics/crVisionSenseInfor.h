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
#ifndef CRPHYSICS_CRVISIONSENSEINFOR_H
#define CRPHYSICS_CRVISIONSENSEINFOR_H 1

#include <CRPhysics\crExport.h>
#include <CRCore\crAITaskManager.h>
#include <CRCore\crVector3.h>
#include <CRCore\crNode.h>
#include <CRCore/crRange.h>
#include <CRPhysics/crRadarMatterObject.h>
#include <map>

namespace CRPhysics{
class crViewMatterObject;
class CRPHYSICS_EXPORT crVisionSenseInfor : public CRCore::Referenced
{
	////////////以下为AI运算函数
public:
	crVisionSenseInfor();
	inline void clearEnvInfos()
	{
		m_enemyMap.clear();
	}

	void dead();

	typedef std::map< float, CRCore::ref_ptr<CRPhysics::crRadarMatterObject::ContactInfo>, std::less<float> > EnemyMap;
	inline void pushToEnemyMap(CRPhysics::crRadarMatterObject::ContactInfo *ci)
	{
		m_enemyMap[ci->m_distance] = ci;
	}

	inline void setEnvInfo(int envmask, float height){ m_envmask = envmask; m_height = height; }
	inline int getEnvMask() { return m_envmask; }
	inline float getHeight() { return m_height; }
	inline bool isInAir() { return m_height>0.05f; }
	void pushTemporaryMoveToTask(const CRCore::crVector3f &position);
	inline void clearTemporaryMoveToTask() { m_temporaryMoveTask->clear(); }
	//void compute();
	//void programEvent();

	//void setTargetRange(const CRCore::rangev3 &targetrange);
	//struct MoveToTask : public CRCore::Referenced
	//{
	//	CRCore::crVector3f m_movetoPoint;
	//	MsgDeque m_msgDeque;
	//	inline relayMsg(CRCore::crEventGroup *sender)
	//	{
	//		CRCore::ref_ptr<Msg> currentMsg;
	//		while(!m_msgDeque.empty())
	//		{
	//			currentMsg = m_msgDeque.front();
	//			if(!currentMsg) 
	//		{
	//			m_msgDeque.pop_front();			
	//			continue;
	//		}
	//		sender->sendEventMessage(currentMsg->receiverGroup.get(),currentMsg->msg,currentMsg->wparam,currentMsg->lparam);

	//		m_msgDeque.pop_front();
	//		}
	//	}
	//};

	//void turnToTarget(const CRCore::crVector3f &target);

	void setFrontRayID(int index);
	inline CRPhysics::crRadarMatterObject::ContactInfoVec &getContactInfoVec() { return m_contactInfoVec; }
	void computerFrontContact(CRPhysics::crViewMatterObject *bot);
	inline CRCore::crVector3f &getFrontContactPosition() { return m_frontContactPosition; }
	inline CRCore::crNode *getFrontContactNode() { return m_frontContactNode.get(); }
	CRCore::crNode *getGndContactNode();
	const CRCore::crVector3f& getGndContactPosition();
protected:
    virtual ~crVisionSenseInfor();
	CRCore::ref_ptr<CRCore::crAITask> m_temporaryMoveTask;
	EnemyMap m_enemyMap;
	int m_envmask;
	float m_height;
	int m_frontRayID;
	CRPhysics::crRadarMatterObject::ContactInfoVec m_contactInfoVec;
	CRCore::crVector3f m_frontContactPosition;
	CRCore::ref_ptr<CRCore::crNode> m_frontContactNode;
	float m_maxTouchDistance;
};



}

#endif