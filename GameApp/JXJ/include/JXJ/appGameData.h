/**********************************************************************
*
*	文件:	   appGameData.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPGAMEDATA_H
#define JXJ_APPGAMEDATA_H
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRNetApp/appNetGameData.h>
namespace JXJ{
class crJXJConsortia : public CRCore::crDataObject
{
public:
	crJXJConsortia();

	void setID(int id);
	void setName(std::string & name);
	void getName(std::string &);
	void setCreateDate(const std::string &createdate);
	void setUpdateDate(const std::string &date);
	void getLeaderName( unsigned char idx ,std::string &name);
	void setStream(CRCore::crStreamBuf *stream);
	void dayZero();
	void weekZero();
	void twoSecZero();
	void setFunds(int funds);
	void loadData();
	void updateLeaderVec();
	void updateToRank(CRCore::crData * gsBrainData);
	void delFromRank(CRCore::crData * gsBrainData);
	void settleEarngins();

	bool checkPermission(int id,unsigned int permission);
	bool willDismiss();

	unsigned char getLevel() ;
	unsigned char getMemberCount();
	unsigned char getShiLi();
	unsigned char getUserGroupPos(int id);
	bool isUserNewer(int id);

	int getExploit();
	int getTroops();
	int getFunds();
	int getDismissTime();
	int getRankNum(CRCore::crData * gsBrainData);
	int getID();
	int getTotalActiveVal();

	std::string &getCreateDate();
	std::string &getUpateDate();
	CRCore::crStreamBuf *getStream();
	virtual void clearData(){}
protected:
	virtual ~crJXJConsortia();
	int m_id;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_createdate;
	std::string m_updateDate;
};
class crJXJAccomplish : public CRCore::crDataObject
{
public:
	crJXJAccomplish();
	void setID(unsigned short id);
	unsigned short getID();
	void setComplete(bool complete);
	bool getComplete();
	void setStep(int step);
	unsigned char getStep();
	void setLardgeStep(unsigned int step);
	unsigned int getLardgeStep();
	void loadData();
	unsigned char getStepCount();
	unsigned short getAccomplishPoint();
	inline bool isComplete(){ return m_complete || m_step >= m_stepCount; }
protected:
	virtual ~crJXJAccomplish();
	unsigned short m_id;
	bool m_complete;//
	unsigned char m_step;
	unsigned int m_largestep;//>=255的step
	//table表数据
	unsigned char m_stepCount;
	unsigned short m_accomplishPoint;
};
class crJXJShili : public CRCore::crDataObject
{
public:
	crJXJShili();
	void setID(int id);
	int getID();
	void setStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();
	void loadData();
	virtual void clearData(){}
protected:
	virtual ~crJXJShili();
	unsigned char m_id;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJPlayerStore : public CRCore::crDataObject
{
public:
	crJXJPlayerStore();
	void setID(int id);
	int getID();
	void setStream(CRCore::crStreamBuf *stream);
	CRCore::crStreamBuf *getStream();
	void loadData();
protected:
	virtual ~crJXJPlayerStore();
	int m_id;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
//class crJXJHuodong : public CRCore::crDataObject
//{
//public:
//	enum CompleteStatus
//	{
//		NotStart,
//		Started,
//		CannotGet,
//		CanGet,
//		HadGot,
//		Finished_hadGot,
//		Finished_notGot,
//		Finished_cannotGot,
//		Finished,
//		MAX = 255
//	};
//
//	crJXJHuodong();
//	void setID(unsigned short id);
//	unsigned short getID();
//	void setComplete(CompleteStatus complete);
//	void changeComplete(CompleteStatus complete,int playerId );
//	CompleteStatus getComplete();
//	void setStep(int step,int playerId ,bool syn = true);
//	void resetStep(int step,int playerId,bool syn = true);
//	bool canGet();
//	int getStep();
//	void setStepCount(int stepCount,int playerId,bool syn = true);
//	int getStepCount();
//	void synStep(int playerId );
//	void checkStartOrFinished(int playerId );
//	bool checkPreCompleted(int playerID);
//	void loadData();
//	void setDelFlag(int delFlag) {m_delFlag = delFlag;}
//	int getDelFlag() {return m_delFlag;}
//	bool getOpenFlag() {return m_isOpen;}
//	void clearData(); 
//	inline bool isComplete()
//	{ 
//		return m_complete == Finished_notGot || m_complete == Finished_hadGot || m_complete == Finished_cannotGot;
//	}
//	bool isDoing() 
//	{
//		return m_complete != NotStart && m_complete != Finished_hadGot 
//		&& m_complete != Finished_notGot && m_complete != Finished_cannotGot;
//	}
//	bool hadGot() {return m_complete == HadGot;}
//	bool isOpen();
//	void setUsedTimes(unsigned short times)
//	{
//		m_usedTimes = times;
//	}
//	unsigned short getUsedTimes()
//	{
//		return m_usedTimes;
//	}
//	const std::string & getStartTime(){ return m_startTime;}
//	const std::string & getEndTime(){ return m_endTime;}
//	bool needSave();
//	int getgoalStep()
//	{
//		return m_goalStep;
//	}
//
//protected:
//
//	void addUsedTimes(short times)
//	{
//		m_usedTimes += times;
//	}
//	
//	virtual ~crJXJHuodong();
//	unsigned short m_id;
//	CompleteStatus m_complete;
//	int m_step;
//	int m_stepCount;
//	//table表数据
//	int m_goalStep;
//	// 删除标记，只要这个值和表里的不同，则认为策划配了新活动，重用了这个活动ID
//	int m_delFlag;			
//	bool m_isOpen;
//	unsigned short m_usedTimes;
//	std::string m_startTime;
//	std::string m_endTime;
//};

class crJXJNewGuide : public CRCore::crDataObject
{
public:
	crJXJNewGuide();
	void setID(unsigned short id);
	unsigned short getID();
	void setComplete(bool complete);
	bool getComplete();
	void setStep(unsigned char step);
	unsigned char getStep();
	void loadData();
	unsigned char getStepCount();
	inline bool isComplete(){ return m_complete || m_step >= m_stepCount; }
protected:
	virtual ~crJXJNewGuide();
	unsigned short m_id;
	bool m_complete;//
	unsigned char m_step;
	//table表数据
	unsigned char m_stepCount;
	unsigned short m_accomplishPoint;
};
//活跃度
class crJXJHuoyuedu : public CRCore::crDataObject
{
public:
	crJXJHuoyuedu();
	void setID(unsigned short id);
	unsigned short getID();
	void setComplete(bool complete);
	bool getComplete();
	void setStep(unsigned char step);
	unsigned char getStep();
	void loadData();
	unsigned char getStepCount();
	unsigned short getPoint();
	inline bool isComplete(){ return m_complete || m_step >= m_stepCount; }
	unsigned char getNeedlv();
protected:
	virtual ~crJXJHuoyuedu();
	unsigned short m_id;
	bool m_complete;//
	unsigned char m_step;
	//table表数据
	unsigned char m_stepCount;
	unsigned short m_Point;
	unsigned char m_needlv;
};
typedef std::vector<CRCore::crVector4i> HuodongAwardVec;
class crJXJNewHuodong : public CRCore::crDataObject
{
public:
	crJXJNewHuodong();
	void setID(unsigned short id);
	unsigned short getID();
	void setStep(int step,int playerid);
	int getStep();
	void loadData();
	int getStepCount();
	char getOpenFlg();
	void setCircles(unsigned char circles);
	unsigned char getCircles();
	void setDelFlag(unsigned char delflag);
	unsigned char getDelFlag();
	std::string getStartTime();
	std::string getEndTime();
	inline bool isCanAdvance(){ return m_step >= m_stepCount; }
	inline bool getComplete(){ return m_activation==2; }
	void reset();
	void refresh(int circles);
	bool isNeedSave();
	void setActivation(unsigned char activation,int playerid);//playerid = 0表示不发包给客户端，如果是客户端调用参数输入0
	inline bool isActivation() { return m_activation == 1; }//活动是否处于激活状态
	unsigned char getActivation();
	void activationCheck(int playerid);//服务器调用
	bool isIntimeCheck();
	int isPreCompleted(int playerID);//1表示完成，0表示未完成 －1表示player未加载
	void setAutoAdvance(bool bln) { m_autoAdvance = bln; }
	bool getAutoAdvance() { return m_autoAdvance; }
	HuodongAwardVec &getAwards() { return m_awards; }
protected:
	virtual ~crJXJNewHuodong();
	unsigned short m_id;
	int m_step;
	//table表数据
	int m_stepCount;
	// 删除标记，只要这个值和表里的不同，则认为策划配了新活动，重用了这个活动ID
	unsigned char m_delFlag;
	char m_openFlag;//开启标志
	unsigned char m_circles;//循环次数,0表示无限次，1默认
	std::string m_startTime;
	std::string m_endTime;
	unsigned char m_activation;//活动激活标识 0:未激活,1激活,2已完成，活动由服务器判定激活(激活条件是活动时间和前置活动完成) 客户端界面只显示处于激活状态的活动
	bool m_autoAdvance;//自动领取
	HuodongAwardVec m_awards;
};
}
#endif