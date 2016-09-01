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
#include <CRCore/crAITaskManager.h>
#include <CRCore/crRange.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

//////////////////////////////////////////////////////////////////
//
//crControlPoint
//
//////////////////////////////////////////////////////////////////
crControlPoint::crControlPoint() :m_needVelocity(false),m_ignoreZ(true)
{
}
crControlPoint::crControlPoint(const crControlPoint& point):
m_position(point.m_position),
m_orientation(point.m_orientation),
m_velocity(point.m_velocity),
m_needVelocity(point.m_needVelocity),
m_ignoreZ(point.m_ignoreZ)
{
}
void crControlPoint::setPosition(const CRCore::crVector3 &pos)
{
    m_position = pos;
}
void crControlPoint::setVelocity(const CRCore::crVector3 &velocity)
{
    m_velocity = velocity;
}
void crControlPoint::setOrientation(const CRCore::crVector3 &orientation)
{
	m_orientation = orientation;
}
void crControlPoint::setNeedVelocity(bool needVelocity)
{
	m_needVelocity = needVelocity;
}
void crControlPoint::setIgnoreZ(bool ignoreZ)
{
	m_ignoreZ = ignoreZ;
}
//////////////////////////////////////////////////////////////////
//
//crProbabilityMsgControlPoint
//
//////////////////////////////////////////////////////////////////
crProbabilityMsgControlPoint::crProbabilityMsgControlPoint(const crProbabilityMsgControlPoint& point):
m_msgProbabilityVec(point.m_msgProbabilityVec)
{
}
void crProbabilityMsgControlPoint::addMsgProbability(const Msg& msg, float probability)
{
	m_msgProbabilityVec.push_back(std::make_pair(msg,probability));
}

Msg* crProbabilityMsgControlPoint::getProbabilityMsg(/*Msg* lastmsg*/)
{//2008.5.24去掉lastmsg的可能性增加1倍的影响
	float count = 0.0f;
	//float lastEffect = 0.0f;
	MsgProbabilityVec::iterator itr;
	for( itr = m_msgProbabilityVec.begin();
		 itr != m_msgProbabilityVec.end(); 
		 ++itr )
	{
/*		if(itr->first == lastmsg) 
		{
			lastEffect = itr->second * 2.0f;
			count += lastEffect;
		}
		else */count += itr->second;
	}

	float random = CRCore::rangef(0.0f,count).get_random();
    count = 0.0f;
	for( itr = m_msgProbabilityVec.begin();
		itr != m_msgProbabilityVec.end(); 
		++itr )
	{
		/*if(itr->first == lastmsg) 
		{
			lastEffect = itr->second * 2.0f;
			count += lastEffect;
		}
		else */count += itr->second;
		if(count > random) return &(itr->first);
	}

	return NULL;//lastmsg;
}

//////////////////////////////////////////////////////////////////
//
//crOrdinalMsgControlPoint
//
//////////////////////////////////////////////////////////////////
crOrdinalMsgControlPoint::crOrdinalMsgControlPoint(const crOrdinalMsgControlPoint& task):
m_ordinalMsgVec(task.m_ordinalMsgVec)
{

}
void crOrdinalMsgControlPoint::addOrdinalMsg(const Msg& msg)
{
	m_ordinalMsgVec.push_back(msg);
}

Msg* crOrdinalMsgControlPoint::getOrdinalMsg(unsigned int& curmsgindex)
{
	if(curmsgindex<m_ordinalMsgVec.size())
	{
		return &m_ordinalMsgVec[curmsgindex++];
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////
//
//crAITask
//
//////////////////////////////////////////////////////////////////
crAITask::crAITask():m_swingFlg(true),m_loopMode(SWING),m_taskState(TaskEnd)
{
	reset();
}
crAITask::crAITask(const crAITask& task):
m_name(task.m_name),
m_loopMode(task.m_loopMode),
m_swingFlg(task.m_swingFlg),
m_taskState(task.m_taskState)
{
	for( TaskPointVec::const_iterator itr = task.m_taskPointVec.begin();
		 itr != task.m_taskPointVec.end();
		 ++itr )
	{
        m_taskPointVec.push_back((*itr)->clone());
	}
	reset();
}
void crAITask::setName(const std::string &name)
{
	m_name = name;
}

crAITask::TaskPointVec &crAITask::getTaskPointVec()
{
    return m_taskPointVec;
}

void crAITask::pushTaskPoint(crControlPoint *cp)
{
    m_taskPointVec.push_back(cp);
}

crControlPoint *crAITask::getNextTaskPoint(int curIndex, int &nextIndex)
{
	if(m_taskPointVec.empty())
	{
		nextIndex = 0;
		return NULL;
	}
	int size = m_taskPointVec.size();
	//if(size == 1) 
	//{
 //       nextIndex = 0;
	//	return m_taskPointVec[0].get();
	//}
    if(m_loopMode == RANDOM)
	{
        nextIndex = CRCore::rangei(0,size).get_random();
		if(nextIndex == curIndex)
		{
			nextIndex++;
			if(nextIndex==size)
				nextIndex = 0;
		}
		return m_taskPointVec[nextIndex].get();
	}

	if(curIndex + 1 < size)
	{   
		if(m_loopMode == SWING && !m_swingFlg) 
		{
			nextIndex = curIndex - 1;
			if(nextIndex<0)
			{
                m_swingFlg = true;
				nextIndex = 1;
			}
		}
		else
		    nextIndex = curIndex + 1;
		//CRCore::notify(CRCore::ALWAYS)<<"crMoveTask::getNextTaskPoint nextIndex = "<<nextIndex<<std::endl;
		return m_taskPointVec[nextIndex].get();
	}
	else if(curIndex < size)
	{
		switch(m_loopMode)
		{
		case SWING:
			if(m_swingFlg)
			{
				m_swingFlg = false;
			}
	        nextIndex = curIndex - 1;
			nextIndex = CRCore::minimum(0,nextIndex);
            return m_taskPointVec[nextIndex].get();
		case LOOP:
			nextIndex = 0;
            return m_taskPointVec[nextIndex].get();
		case NO_LOOPING:
            nextIndex = curIndex;
			return NULL;
		}
	}
	nextIndex = 0;
	return NULL;
}

crControlPoint *crAITask::getRandomTaskPoint()
{
	int size = m_taskPointVec.size();
	int random = CRCore::rangei(0,size).get_random();
	return m_taskPointVec[random].get();
}

crControlPoint *crAITask::getLastTaskPoint()
{
	int size = m_taskPointVec.size();
	return m_taskPointVec[size-1].get();
}

crControlPoint *crAITask::getTaskPoint(int i)
{
    if(i>=0 && i<m_taskPointVec.size())
		return m_taskPointVec[i].get();
	return m_taskPointVec.empty()?NULL:m_taskPointVec[0].get();
}
void crAITask::setTaskState(TaskState taskState)
{
	m_taskState = taskState;
}
crAITask::TaskState crAITask::getTaskState()
{
	return m_taskState;
}
void crAITask::clear()
{ 
	m_taskPointVec.clear(); 
	m_taskState = TaskEnd;
	reset();
}
void crAITask::reset()
{//这些数据需要与m_task相关联，否则容易产生crDoAITaskMethod error
	m_time_aimsg = 0.0f;
	m_currentIndex = 0;
	m_currentMsgIndex = 0;
	m_interval_aimsg = 0.0f;
	m_preControlPoint = NULL;
}
//////////////////////////////////////////////////////////////////
//
//crAITaskManager
//
//////////////////////////////////////////////////////////////////

ref_ptr<crAITaskManager> crAITaskManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crAITaskManager::crAITaskManager()
{
}

crAITaskManager* crAITaskManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crAITaskManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crAITaskManager::clear()
{
	m_instance = NULL;
}

void crAITaskManager::insertAITask(crAITask *aiTask)
{
	if(!aiTask) return;
    m_aiTaskMap[aiTask->getName()] = aiTask;
}

crAITask *crAITaskManager::getAITask(const std::string &id)
{
	if(id.empty()) return NULL;
	AITaskMap::iterator itr = m_aiTaskMap.find(id);
	if(itr != m_aiTaskMap.end()) return itr->second->clone();
    return NULL;
}