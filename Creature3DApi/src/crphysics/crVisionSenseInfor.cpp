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
#include <CRPhysics/crVisionSenseInfor.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crMath.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crThread.h>
#include "rcfg/ConfigScriptOO.h"
#include <sys/stat.h>
using namespace CRCore;
using namespace CRPhysics;
using namespace CRParticle;

//static float s_angles[9] = {DegreesToRadians(-90.0f),DegreesToRadians(-50.0f),DegreesToRadians(-30.0f),DegreesToRadians(-10.0f),
//0.0f,DegreesToRadians( 10.0f),DegreesToRadians( 30.0f),DegreesToRadians( 50.0f),DegreesToRadians( 90.0f)};

crVisionSenseInfor::crVisionSenseInfor():
m_frontRayID(0),
m_maxTouchDistance(2.0f)
{
	m_temporaryMoveTask = new crAITask;
	m_temporaryMoveTask->setName("temporaryMoveTask");
	m_temporaryMoveTask->setLoopMode(crAITask::NO_LOOPING);
}

crVisionSenseInfor::~crVisionSenseInfor()
{
}

////////////以下为AI运算
void crVisionSenseInfor::pushTemporaryMoveToTask(const CRCore::crVector3f &position)
{
	crControlPoint *cp = new crControlPoint;
	cp->setPosition(position);
	m_temporaryMoveTask->pushTaskPoint(cp);
}

void crVisionSenseInfor::dead()
{
	m_enemyMap.clear();
	//m_enemySet.clear();
	m_temporaryMoveTask->clear();
	//m_currentMoveTask = NULL;
}

void crVisionSenseInfor::setFrontRayID(int index)
{
	m_frontRayID = index;
}

CRCore::crNode *crVisionSenseInfor::getGndContactNode()
{
    return m_contactInfoVec[m_contactInfoVec.size()-1]->m_contactNode.get();
}

const CRCore::crVector3f& crVisionSenseInfor::getGndContactPosition()
{
	return m_contactInfoVec[m_contactInfoVec.size()-1]->m_contactPos;
}
void crVisionSenseInfor::computerFrontContact(CRPhysics::crViewMatterObject *bot) 
{ 
	CRPhysics::crRadarMatterObject::ContactInfo *ci = m_contactInfoVec[m_frontRayID].get(); 
	m_frontContactPosition = ci->m_contactPos; 
	if(ci->m_distance<m_maxTouchDistance)
	{
		m_frontContactNode = m_contactInfoVec[m_frontRayID]->m_contactNode.get();
		if(m_frontContactNode.valid())
		{
			m_frontContactNode->doEvent(WCH_FRONTCONTACT,MAKEINT64(bot,NULL));
		}
	}
	else m_frontContactNode = NULL;
}