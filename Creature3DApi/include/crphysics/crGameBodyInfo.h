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
#ifndef CRPHYSICS_CRGAMEBODYINFO_H
#define CRPHYSICS_CRGAMEBODYINFO_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crBase.h>
#include <CRCore/crRange.h>
#include <CRCore/crMatrix.h>
#include <map>
#include <set>
#include <vector>

namespace CRPhysics{

class CRPHYSICS_EXPORT crGameBodyInfo : public CRCore::crBase
{
friend class crViewMatterObject;
public:
	crGameBodyInfo();
	crGameBodyInfo(const crGameBodyInfo&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Base(CRPhysics, crGameBodyInfo);

	inline int getHP() { return m_hp; }
	inline void setHP(int hp) { m_hp = hp; }
	inline int getMaxHP() { return m_maxhp; }
	inline void setMaxHP(int hp) { m_maxhp = hp; }
	inline int getDefend() { return m_defend; }
	inline void setDefend(int defend) { m_defend = defend; }
	inline bool isDead(){ return m_hp <= 0; }
    void setTeam(int team);
	inline int getTeam() { return m_team; }
	inline int checkTeam(int team) { return team * m_team; }//返回值＝0表示中立，>0表示队友，<0表示敌人 
	void kill();
    void addExcution(int excution);
	virtual void renascence();

	void increaseKilledCount();

	int getKilledCount();
	int getDeathCount();

	virtual void addParam(int i, const std::string &str);

	void setInitExperience(int experience);
	inline void increaseExperience(int experience){ m_experience += experience; levelUp(); }
	inline void decreaseExperience(int experience){ m_experience -= experience; }
	inline int getExperience(){ return m_experience; }
	inline int getLevel(){ return m_level; }
	inline void setExperience(int experience){ m_experience = experience; }
    inline void setLevel(int level){ m_level = level; }
 
	void setMaxDetectCount(int detectCount);
	inline int getMaxDetectCount(){ return m_maxDetectCount; }
	void setMaxDetectRange(float detectRange);
	inline float getMaxDetectRange(){ return m_maxDetectRange; }

	struct DetectParam
	{
		DetectParam(float cosa, float distance):m_cosa(cosa),m_distance(distance){}
		float m_cosa;
		float m_distance;
	};
	struct LessFunctor
	{
		bool operator () (const DetectParam& lhs,const DetectParam& rhs) const
		{
			if(lhs.m_cosa>rhs.m_cosa) return true;
			//if(rhs.m_cosa>lhs.m_cosa) return false;
			if(lhs.m_distance<rhs.m_distance) return true;
			//if(rhs.m_distance>lhs.m_distance) return false;
			return false;
		}
	};
	typedef std::multimap< DetectParam, CRCore::ref_ptr<crViewMatterObject>, LessFunctor > DetectMap;

	inline DetectMap &getDetectMap(){ return m_detectMap; }
	inline void clearDetectMap(){ if(!m_detectMap.empty()) m_detectMap.clear(); }
	//void changeLockedTarget();
	void updateLockedTarget(int count, unsigned int weaponMask);
	//inline int getCurrentLockedTargetIndex(){ return m_currentLockedTargetIndex; }
	//crViewMatterObject *getCurrentLockedTarget();
	typedef std::set< CRCore::ref_ptr<crViewMatterObject> >LockedTargetSet;
	inline LockedTargetSet &getLockedTargetSet(){ return m_lockedTargetSet; }
	//void addLockedTargetByName(const std::string &name);

	void setTaskPointRange(float taskPointRange);
	inline float getTaskPointRange(){ return m_taskPointRange; }

	void setTargetRange(float range);
	inline CRCore::crVector3 getTargetOffset(){ return CRCore::crVector3(m_targetRange.get_random(),m_targetRange.get_random(),m_targetRange.get_random()); }

	//inline void setSceneID(int sceneid){ m_sceneid = sceneid; }
	int getSceneID();

	void setStallVelocity(float stallVelocity);
	inline float getStallVelocity(){ return m_stallVelocity; }

	void setStealth(float stealth);
	inline float getStealth(){ return m_stealth; }

	//inline void lockBodyInfo() { m_bodyInfoMutex.lock(); }
 //   inline void unlockBodyInfo() { m_bodyInfoMutex.unlock(); }

	enum ManipulatorMode
	{
		MM_FPS,
		MM_RPG,
		MM_RTS,
		MM_TANK,
	};
	inline void setManipulatorMode(ManipulatorMode mode){ m_manipulatorMode = mode; }
	ManipulatorMode getManipulatorMode() const;
	
	typedef std::vector< CRCore::ref_ptr<CRCore::crNode> > SelectNodeVec;
	inline bool hasNodeSelected() const { return !m_selectNodeVec.empty(); }
	bool isNodeSelected(CRCore::crNode *node) const;
	bool isNodeSelected(const std::string &nodeName) const;
	void selectNode(CRCore::crNode *node);
	void selectNodeEvent(__int64 kbmsg, __int64 param = 0);
	void unSelectNode(CRCore::crNode *node);
	void unSelectAll();
	void translateSelectedNodes(CRCore::crMatrix &transMatrix);
	void scaleSelectedNodes(CRCore::crMatrix &scaleMatrix);

	inline SelectNodeVec &getSelectNodeVec(){ return m_selectNodeVec; }
	CRCore::crNode *getFirstSelectNode();
protected:
	virtual ~crGameBodyInfo(){}
	void increaseDeathCount();
    void levelUp();

	/////关于body的信息
	int m_hp;//生命值
	int m_maxhp;
	int m_defend;//防御力
	int m_team;//0表示中立，>0表示队友，<0表示敌人 
	int m_killedCount;
	int m_deathCount;

	//int m_sceneid;//关卡编号,用于代表目前处于第几关
	//////空战游戏属性
	int m_level;
	int m_experience;
	int m_maxDetectCount;//最多侦测目标的数量
	float m_maxDetectRange;//最大侦测距离
	DetectMap m_detectMap;//在cullvisitor时获取detectMap并进行处理
	//int m_currentLockedTargetIndex;

	float m_taskPointRange;

	//CRCore::ref_ptr<crViewMatterObject> m_lockedTarget;
	LockedTargetSet m_lockedTargetSet;
	//CRCore::rangev3 m_targetRange;
	CRCore::rangef m_targetRange;
	float m_stallVelocity;//失速
	float m_stealth;//隐形参数

    ManipulatorMode m_manipulatorMode;
	//CRCore::crMutex m_bodyInfoMutex;

	SelectNodeVec m_selectNodeVec;
};

}

#endif