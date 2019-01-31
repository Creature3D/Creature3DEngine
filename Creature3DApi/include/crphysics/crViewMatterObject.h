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
#ifndef CRPHYSICS_CRVIEWMATTEROBJECT_H
#define CRPHYSICS_CRVIEWMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crMatterObject.h>
#include <CRPhysics/crRadarMatterObject.h>
#include <CRCore/crMath.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crMultiSwitch.h>
#include <CRCore/crTimer.h>
#include <CRPhysics/crGameBodyInfo.h>
#include <CRCore/crIntersectVisitor.h>
#include <CRPhysics/crVisionSenseInfor.h>
#include <CRPhysics/crCamera.h>
//#define Height 1.5f
namespace CRPhysics{
class crWeaponMatterObject;
class CRPHYSICS_EXPORT crViewMatterObject : public crMatterObject
{
	friend class crCamera;
public:
	crViewMatterObject();
	crViewMatterObject(const crViewMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crViewMatterObject);

	virtual void releaseObjects(CRCore::crState* state);
	virtual void releaseOde();

	virtual void traverse(CRCore::crNodeVisitor& nv);

	virtual void updateCreMatrix();
	virtual void updateOdeMatrix();

	void setGroundDetect(bool groundDetect);
	bool getGroundDetect(){ return m_groundDetect; }

	void setCanFalldown(bool canFalldown) { m_canFalldown = canFalldown; }
	bool getCanFalldown(){ return m_canFalldown; }

	void rotate(const CRCore::crVector2f& angle);//mouse rotate
	void rotate(const CRCore::crVector3f& angle);

	void setVelocity(const CRCore::crVector3f& speed/*, int flg*/);
	inline CRCore::crVector3f& getVelocity(){ return m_velocity; }
	void setVelocity0(float vx);
	void setVelocity1(float vy);
	void setVelocity2(float vz);

	void setMaxSpeed(const CRCore::crVector3f& speed);
	void setMinSpeed(const CRCore::crVector3f& speed);
	CRCore::crVector3f& getMaxSpeed(){ return m_maxSpeed; }
    CRCore::crVector3f& getMinSpeed(){ return m_minSpeed; }
    
	float getSpeedRate0(){ float r = m_maxSpeed[0] - m_minSpeed[0]; return r > 0 ? (m_velocity[0] - m_minSpeed[0]) / r : 1.0f;}
	float getSpeedRate1(){ float r = m_maxSpeed[1] - m_minSpeed[1]; return r > 0 ? (m_velocity[1] - m_minSpeed[1]) / r : 1.0f;}

	void setAcceleration(const CRCore::crVector3f& acceleration);
	void setAcceleration0(float ax);
	void setAcceleration1(float ay);
	void setAcceleration2(float az);

	void setHPR(const CRCore::crVector3f& hpr);
	void setH(float H);
	void setP(float P);
	void setR(float R);

	virtual CRCore::crMatrixd getViewMatrix();
	virtual CRCore::crVector3f getViewDirection();//返回视点方向向量
	virtual CRCore::crVector3f getViewUpVector();

	//virtual CRCore::crVector3f getDirection();
	//virtual CRCore::crVector3f getDirectionX();

	inline void resetRotation() { /*m_currentRotation.set(0.0f,0.0f,0.0f);*/ }
	inline void resetView()
	{
		resetViewPitch();
		resetViewRoll();
		//resetRotation();
	}
	void updateViewPitch( float delta );
	inline void resetViewPitch(){ m_viewPitch = 0.0f; }
	inline float getViewPitch(){ return m_viewPitch; }

	void updateViewRoll( float delta );
	inline void resetViewRoll(){ m_viewRoll = 0.0f; }
	inline float getViewRoll(){ return m_viewRoll; }

	void setViewOffset( float offset ) { if(!m_lockViewOffset) m_viewOffset = offset; }
	inline float getViewOffset(){ return m_viewOffset; }
	inline void setViewHeight( float height ) { m_viewHeight = height; }
	inline float getViewHeight(){ return m_viewHeight; }
	void setViewOffsetRange( const CRCore::crVector2& range ) { m_viewOffsetRange = range; }
	void setViewPitchRange( const CRCore::crVector2& range ) { m_viewPitchRange = range; }
	void setViewPitch( float pitch ) { m_viewPitch = pitch; }
	void setViewRoll( float roll ) { m_viewRoll = roll; }

	void setViewShake(float range, float inteval) { m_viewShakeRange = range; m_viewShakeInterval = inteval; }
	//void setCameraBind(bool cameraBind);
	//bool getCameraBind() const;

	///radar
	void attachRadar(crRadarMatterObject *radar);
	crRadarMatterObject *getRadar();
	inline crVisionSenseInfor *getVisionSenseInfor() { return m_visionSenseInfor.get(); }

	void setHideBody(bool hidebody);
	bool getHideBody() const;

	void setGameBodyInfo(crGameBodyInfo *gameBodyInfo);
	inline crGameBodyInfo *getGameBodyInfo(){ return m_gameBodyInfo.get(); }

	void kill( int killflg = 0 );
	void addExcution(int excution,crViewMatterObject *firePerson = NULL);
	bool isDead() const;
    int checkTeam(int team) const;
	void increaseKilledCount();
	int getTeam();
	int getHP();
	int getDefend();

	//weapon
	typedef std::map<std::string, CRCore::ref_ptr<crWeaponMatterObject> > WeaponMatterObjectMap;
	bool testToAddWeaponMatterObject(crWeaponMatterObject *weapon);
	void addWeaponMatterObject(crWeaponMatterObject *weapon);
	void selectWeaponMatterObject(const std::string &weaponName);
	void selectWeaponMatterObject(crWeaponMatterObject *weapon);
	crWeaponMatterObject *getCurrentWeaponMatterObject();
	inline WeaponMatterObjectMap &getWeaponMap(){ return m_weaponMatterObjects; }
	inline bool isWeaponEquiped(const std::string &weapon) { return m_weaponMatterObjects.find(weapon) != m_weaponMatterObjects.end(); }
	inline crWeaponMatterObject *getWeaponMatterObject(const std::string &weaponName)
	{
		WeaponMatterObjectMap::iterator itr = m_weaponMatterObjects.find(weaponName);
		return itr == m_weaponMatterObjects.end()?NULL:itr->second.get();
	}
	void removeWeapon(const std::string &weaponName);
	void removeWeapon(crWeaponMatterObject *weapon);
	bool addCaisson(crCaissonMatterObject *caisson);
	bool addHp(crHpMatterObject *hp);
	bool addArmor(crArmorMatterObject *armor);

	//void refreshBulletCountInfo();该函数用于切换掉没有弹药的武器。delete 2011.2.27
	//void refreshArmor_HpInfo();
	
	inline bool getDeadFlag() 
	{ 
		bool rtn = m_deadFlag;
		if(m_deadFlag) m_deadFlag = false;
		return rtn;
	}
	void setRequireRenascence( bool require );
	bool getRequireRenascence();
	virtual void renascence();

	//enum BodyPhysicsType
	//{
	//	RayDetect,
	//	RealPhysics
	//};
	//bool physicsSwap();
	//inline BodyPhysicsType getCurrentBodyPhysicsType(){ return m_currentBodyPhysicsType; }

	////virtual bool isInAir(){ return false; };
	//void setLinearVel(const CRCore::crVector3f& vel, float interval);
	//void addForce(const CRCore::crVector3f& force, float interval);
	//void enablePhysics(float interval);
	//void resetPhysicsSwap();

	void swapToFutureMatrix();

    virtual CRCore::crMatrix getCameraMatrix();

	inline const std::string &getBotModelName(){ return m_botModelName; }
	void setBotModelName(const std::string &name);

	typedef std::multimap< std::string, std::string > WeaponRackNameMap;//有可能是dof或matrixtransform
	void insertWeaponRackName(const std::string &rackName, const std::string &nodeName);
	typedef std::multimap< std::string, CRCore::ref_ptr<crNode> > WeaponRackMap;//有可能是dof或matrixtransform

	inline virtual void swapPhysicsBuffers() { m_futureMatrix = m_matrix_buf; }
	inline int getTargetCount() { return m_targetCount; }

	unsigned int getCurrentWeaponMask();

	inline void setFutureMatrix(const CRCore::crMatrix &mat) { m_futureMatrix = mat; }

	//void changeCamera(unsigned int index);
	//unsigned int getActiveCamera(){ return m_cameraSwitch.valid()?m_cameraSwitch->getActiveSwitchSet():0;}
	//CRCore::crNode *getActiveCameraFirstChild(){ return m_cameraSwitch.valid()?m_cameraSwitch->getFirstActiveChild():NULL;}
	//void changeBody(unsigned int index);
	//unsigned int getActiveBody(){ return m_bodySwitch.valid()?m_bodySwitch->getActiveSwitchSet():0;}
	//CRCore::crNode *getActiveBodyFirstChild(){ return m_bodySwitch.valid()?m_bodySwitch->getFirstActiveChild():NULL;}
	//CRCore::crMultiSwitch *getBodySwitch(){ return m_bodySwitch.valid() ? m_bodySwitch.get():NULL;}
	inline CRCore::crGroup *getBody(){ return m_body.get(); }
	CRCore::crNode *getCurrentActiveBody();
	inline CRCore::crNode *getBodyChild(/*unsigned int i = 0*/){ return getCurrentActiveBody(); }//return m_body.valid()?m_body->getChild(i):getChild(i); }

	virtual void setVisiable( bool visiable );

	virtual void setEnableIntersect( bool enableIntersect);
	float getDeathInterval();
	virtual void doEvent(_crInt64 kbmsg, CREPARAM param = NULLPARAM);
	void collided();

	struct CRPHYSICS_EXPORT BotPhysicsObject : public CRCore::Referenced
	{
	public:
		BotPhysicsObject():m_geomid(NULL),m_executionrate(0.0f){};
        CRCore::ref_ptr<const CRCore::crMatrixTransform> m_node;
		dGeomID m_geomid;
        float m_executionrate;
		inline bool operator == (dGeomID geomid) const { return m_geomid==geomid; }
	};
	typedef std::vector< CRCore::ref_ptr<BotPhysicsObject> > BotPhysicsObjectVec;
	BotPhysicsObject *getBotPhysicsObject(void *geomid);
	inline int getNumBotPhysicsObject() const { return m_botPhysicsObjectVec.size(); }

	inline void setIsNetBot(bool isNetBot){ m_isNetBot = isNetBot; }
	inline bool getIsNetBot() const { return m_isNetBot; }

	typedef std::map<std::string, CRCore::ref_ptr<CRCore::crGroup> > RackMap;
	CRCore::crGroup *getRackNode(const std::string &rackName);

	void setLockViewPitch(bool viewPitch);
	bool getLockViewPitch();
	void setLockViewRoll(bool viewRoll);
	bool getLockViewRoll();
	void setLockViewOffset(bool viewOffset);
	bool getLockViewOffset();

	void setPhysicsBox(const CRCore::crVector3f &boxlength);
	inline virtual CRCore::crVector3f getLengths()
	{
		return m_physicsBox;
	}
	virtual CRCore::crVector3 getPhysicsCenter() const
	{
		CRCore::crVector3 pos = getTrans();
		pos[2] += m_physicsBox[2] * 0.5f;
		return pos;
	}
	inline const CRCore::crVector3f& getPhysicsBox()const { return m_physicsBox; }
	void setWalkableHeight(float height);
	inline float getWalkableHeight() const { return m_walkableHeight; }

	inline void cameraSwaped(){ m_cameraSwaped = true; }

	void setViewPitchVelocity(float v){ m_viewPitchVelocity = v; }
	void setViewOffsetVelocity(float v){ m_viewOffsetVelocity = v; }
	inline float getViewPitchVelocity() const { return m_viewPitchVelocity; }
	inline float getViewOffsetVelocity() const { return m_viewOffsetVelocity; }

	inline void lockMatrix(){ m_lockMatrix = true; }
	inline void unlockMatrix(){ m_lockMatrix = false; }
	void setTargetViewRoll(float targetViewRoll,float times);
	void setTargetViewRoll2(float targetViewRoll,float speed);
	void setTargetViewPitch(float targetViewPitch,float times);
	void setTargetViewPitch2(float targetViewPitch,float speed);
	void setTargetViewHeight(float targetViewHeight,float times);
	void setTargetViewHeight2(float targetViewHeight,float speed);
	void setTargetViewOffset(float targetViewOffset,float times);
	void setTargetViewOffset2(float targetViewOffset,float speed);
	void setSlopeX(float x);
	void setSlopeY(float y);
	void setFreeCamera(bool bln);
	bool getFreeCamera();
	void setCameraPitchDelta(float pitch);
	float getCameraPitchDelta();
protected:
	virtual ~crViewMatterObject();
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
	void setDummyMatrix();
	virtual bool computeBound() const;

	void move();
	void doHPR();
	void doAdjust();
	void dead( int deadflg = 0, crViewMatterObject *firePerson = NULL);

    //virtual void updateWeaponMatrix();

	CRCore::crVector3f m_velocity;
	CRCore::crVector3f m_acceleration;
	CRCore::crVector3f m_hpr;
	CRCore::crVector3f m_rotateAngles;//rotate hpr

	CRCore::crVector3f m_currentRotation;//

	CRCore::crVector3f m_maxSpeed;
	CRCore::crVector3f m_minSpeed;

	mutable CRCore::ref_ptr<crGameBodyInfo> m_gameBodyInfo;

	///////////
    ////radar
	mutable CRCore::ref_ptr<crRadarMatterObject> m_radarMatterObject;
    CRCore::ref_ptr<crVisionSenseInfor> m_visionSenseInfor;

	bool       m_lockViewPitch;
	float      m_viewPitch;
	float      m_currentViewPitch;
	bool       m_lockViewRoll;
	float      m_viewRoll;
	float      m_viewOffset;
	float      m_currentViewOffset;
	float      m_viewVelocity;
	float      m_viewHeight;
	float      m_walkableHeight;
	CRCore::crVector2f m_viewOffsetRange;
	CRCore::crVector2f m_viewPitchRange;

	float m_viewShakeRange;
	float m_viewShakeInterval;

	//bool        m_cameraBind;
	bool        m_cameraSwaped;

	bool        m_isNetBot;

	bool        m_hideBody;
	bool        m_groundDetect;
	bool        m_deadFlag;
	bool        m_requireRenascence;

	bool        m_collided;
	bool        m_canFalldown;

	//double m_previousFrameTime;
	//double m_frameInterval;

	////weapon
	WeaponMatterObjectMap m_weaponMatterObjects;
	CRCore::ref_ptr<crWeaponMatterObject> m_currentWeaponMatterObject;
	CRCore::crMatrix m_futureMatrix;
	std::string m_botModelName;

	/////////physics
	//float m_physicsSwapInterval;
	//BodyPhysicsType m_currentBodyPhysicsType;

	WeaponRackMap m_weaponRackMap;
	WeaponRackNameMap m_weaponRackNameMap;
	int m_targetCount;

	//mutable CRCore::ref_ptr<CRCore::crMultiSwitch> m_cameraSwitch;
	//CRCore::ref_ptr<CRCore::crMultiSwitch> m_bodySwitch;
	CRCore::ref_ptr<CRCore::crGroup> m_body;
	CRCore::Timer_t m_deathTime;//rec death time etc

	CRCore::crVector3f m_lastSecurePos;
	CRCore::crVector3f m_lastPos;

	mutable CRCore::crVector3f m_radarPos;
	BotPhysicsObjectVec m_botPhysicsObjectVec;
	CRCore::crVector2i m_transtest;

	RackMap m_rackMap;

	CRCore::crVector3f m_physicsBox;

	CRCore::crIntersectVisitor               m_iv;
	CRCore::ref_ptr<CRCore::crLineSegment>   m_lineSegment;

	float m_viewPitchVelocity;
	float m_viewOffsetVelocity;

	bool m_lockMatrix;//
	float m_targetHeight;
	float m_heightAdjustVelocity;
	float m_heightAdjustTime;

	bool m_adjustViewRoll;
	float m_targetViewRoll;
	float m_viewRollAdjustVelocity;
	float m_viewRollAdjustTime;
	bool m_adjustViewPitch;
	float m_targetViewPitch;
	float m_viewPitchAdjustVelocity;
	float m_viewPitchAdjustTime;
	bool m_adjustViewHeight;
	float m_targetViewHeight;
	float m_viewHeightAdjustVelocity;
	float m_viewHeightAdjustTime;
	bool m_adjustViewOffset;
	float m_targetViewOffset;
	float m_viewOffsetAdjustVelocity;
	float m_viewOffsetAdjustTime;
	bool m_lockViewOffset;
	float m_slopeX;//自适应地面斜坡
	float m_slopeY;//自适应地面斜坡
	bool m_freeCamera;//
	float m_cameraPithDelta;
};

class CRPHYSICS_EXPORT crViewObjectNearCallback : public crMatterGroup::NearCallback
{
public:
	virtual crViewObjectNearCallback* cloneType() const { return NULL; };
	virtual const char* libraryName() const { return "CRPhysics"; }
	virtual const char* className() const { return "crViewObjectNearCallback"; }

	virtual void nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 );

	static crViewObjectNearCallback* instance();
};

}

#endif