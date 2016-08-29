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
#ifndef CRCORE_CRAITASKMANAGER
#define CRCORE_CRAITASKMANAGER 1

#include <CRCore/crExport.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector2.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crWchMsg.h>
#include <vector>
#include <map>
#include <string>
namespace CRCore {
#define WCHAI_Begin              0x01
#define WCHAI_Pause              0x02
#define WCHAI_Resume             0x03
#define WCHAI_End                0x04
#define WCHAI_SetBotToCP         0x01
#define WCHAI_SetCPToBot         0x02
#define WCHAI_RecBot             0x04
//#define WCHAI_TakeoverControl    0x08
struct CR_EXPORT Msg
{
public :
	Msg():m_msg(NULL),m_param(NULL),m_interval(NULL){}
	_crInt64   m_msg;
	_crInt64   m_param;
	int        m_interval;
	inline bool operator < (const Msg& item) const
	{
		if (m_msg<item.m_msg) return true;
		else if (m_msg>item.m_msg) return false;
		else if (m_param<item.m_param) return true;
		else if (m_param>item.m_param) return false;
		else if (m_interval>item.m_interval) return false;
		else return (m_interval<item.m_interval);
	}
	inline bool operator ==(const Msg& item) const
	{
		return m_msg == item.m_msg &&
			m_param == item.m_param &&
			m_interval == item.m_interval;
	}
	inline Msg& operator = (const Msg& item) 
	{ m_msg = item.m_msg;  m_param = item.m_param; m_interval = item.m_interval; return (*this); }
};

#define TaskPointClass(library,name) \
virtual CRCore::crControlPoint* clone() const { return new name(*this); }

class CR_EXPORT crControlPoint : public CRCore::Referenced
{
public :
	crControlPoint();
	crControlPoint(const crControlPoint& point);
	TaskPointClass(CRCore,crControlPoint)

	void setPosition(const CRCore::crVector3 &pos);
	inline const CRCore::crVector3 &getPosition() const { return m_position; }
	inline bool isInRange(CRCore::crVector3 currentPos, float range) 
	{
		if(m_ignoreZ)
		{
			currentPos[2] = m_position[2];
		}
		return (currentPos - m_position).length2() < range*range; 
	}
	void setVelocity(const CRCore::crVector3 &velocity);
	inline const CRCore::crVector3 &getVelocity() const { return m_velocity; }
	void setOrientation(const CRCore::crVector3 &orientation);
	inline const CRCore::crVector3 &getOrientation() const { return m_orientation; }
	void setNeedVelocity(bool needVelocity);
	inline bool getNeedVelocity() const { return m_needVelocity; }
	void setIgnoreZ(bool ignoreZ);
	inline bool getIgnoreZ() const { return m_ignoreZ; }
	inline virtual void clear() {}
protected :
	virtual ~crControlPoint() {}
	CRCore::crVector3 m_position;
	CRCore::crVector3 m_orientation;
	CRCore::crVector3 m_velocity;
    bool m_needVelocity;
	bool m_ignoreZ;
};

class CR_EXPORT crProbabilityMsgControlPoint : public crControlPoint
{
public :
	crProbabilityMsgControlPoint(){}
	crProbabilityMsgControlPoint(const crProbabilityMsgControlPoint& task);
	TaskPointClass(CRCore,crProbabilityMsgControlPoint)

	typedef std::pair<Msg,float> MsgProbabilityPair;
	typedef std::vector<MsgProbabilityPair> MsgProbabilityVec;
	void addMsgProbability(const Msg& msg, float probability);
	Msg* getProbabilityMsg();
	inline virtual void clear() { m_msgProbabilityVec.clear(); }
protected :
	virtual ~crProbabilityMsgControlPoint() {}
	MsgProbabilityVec  m_msgProbabilityVec;
};

class CR_EXPORT crOrdinalMsgControlPoint : public crControlPoint
{
public :
	crOrdinalMsgControlPoint(){}
	crOrdinalMsgControlPoint(const crOrdinalMsgControlPoint& task);
	TaskPointClass(CRCore,crOrdinalMsgControlPoint)

	typedef std::vector<Msg> OrdinalMsgVec;
	void addOrdinalMsg(const Msg &msg);
	Msg *getOrdinalMsg(unsigned int& curmsgindex);
	inline virtual void clear() { m_ordinalMsgVec.clear(); }
protected :
	virtual ~crOrdinalMsgControlPoint() {}
	OrdinalMsgVec  m_ordinalMsgVec;
};

class CR_EXPORT crAITask : public CRCore::Referenced
{
public :
	crAITask* clone() const { return new crAITask(*this); }
	crAITask();
	crAITask(const crAITask& task);
	inline void setName(const std::string &name);
	inline const std::string &getName() const { return m_name; }

	typedef std::vector< CRCore::ref_ptr<crControlPoint> > TaskPointVec;
	TaskPointVec &getTaskPointVec();
	void pushTaskPoint(crControlPoint *cp);
	virtual void clear();
	inline crControlPoint *getCurrentTaskPoint(int curIndex){ return curIndex<m_taskPointVec.size()? m_taskPointVec[curIndex].get(): 0;}
	crControlPoint *getNextTaskPoint(int curIndex, int &nextIndex);
	crControlPoint *getRandomTaskPoint();
	crControlPoint *getLastTaskPoint();
	crControlPoint *getTaskPoint(int i);
	inline int getNumTaskPoints() const { return m_taskPointVec.size(); }

	enum LoopMode
	{
		SWING,
		LOOP,
		NO_LOOPING,
		RANDOM
	};
	void setLoopMode(LoopMode lm) { m_loopMode = lm; }
	LoopMode getLoopMode() const { return m_loopMode; }

	enum TaskState
	{
		TaskBegin,
		TaskDoing,
		TaskPause,
		TaskEnd
	};
	void setTaskState(TaskState taskState);
	TaskState getTaskState();
	
	virtual void reset();
	inline float& time_aimsg(){ return m_time_aimsg; }
	inline int &currentIndex() { return m_currentIndex; }
	inline unsigned int &currentMsgIndex() { return m_currentMsgIndex; }
	inline float &interval_aimsg() { return m_interval_aimsg; }
	inline crControlPoint*& preControlPoint() { return m_preControlPoint; }
protected :
	virtual ~crAITask() {}
	std::string m_name;
	TaskPointVec m_taskPointVec;
	LoopMode            m_loopMode;
	bool m_swingFlg;
	TaskState m_taskState;

	//DoAITask
	float m_time_aimsg;
	int m_currentIndex;
	unsigned int m_currentMsgIndex;
	float m_interval_aimsg;
	crControlPoint *m_preControlPoint;
};

class CR_EXPORT crAITaskManager : public CRCore::Referenced
{
public :
	crAITaskManager();
	static crAITaskManager *getInstance();
    
	typedef std::map<std::string, CRCore::ref_ptr<crAITask> > AITaskMap;
    void insertAITask(crAITask *aiTask);
	crAITask *getAITask(const std::string &id);
	virtual void clear();
protected :
    virtual ~crAITaskManager() {}
	static CRCore::ref_ptr<crAITaskManager> m_instance;
	AITaskMap m_aiTaskMap;
};

}

#endif
