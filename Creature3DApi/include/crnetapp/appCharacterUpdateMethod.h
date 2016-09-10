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
#ifndef CRNETAPP_CHARACTERUPDATEMETHOD_H
#define CRNETAPP_CHARACTERUPDATEMETHOD_H 1

#include <rbody/Creature3D/CreBody.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crHandleManager.h>
//#include <CRGUI/cr2DStageManager.h>
//#include <CRNetApp/appClientPlayerData.h>
#include <CRParticle/crParticleEffect.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crSoundManager.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{
//mmo rpg½»»¥·½Ê½
class CRNETAPP_EXPORT crRpgGameCharacterUpdateMethod : public CRCore::crMethod
{
public:
	crRpgGameCharacterUpdateMethod();
	crRpgGameCharacterUpdateMethod(const crRpgGameCharacterUpdateMethod& handle);
	MethodClass(CRNetApp, RpgGameCharacterUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	enum DownState
	{
		DS_None,
		DS_Stand,
		DS_Idle,
		DS_Jump,
		DS_Falldown,
		DS_Landfall,
		DS_Move,
		DS_Dead,
		DS_Hurt
	};
	enum UpState
	{
		US_None,
		US_WeaponFire,
		US_WeaponUnSelect,
		US_WeaponSelect,
	};

	enum Move
	{
		M_None,
		StopMove,
		MoveUp,
		//MoveDown,
		//MoveLeft,
		//MoveRight,
	};

	bool wantChangeDownState(DownState state, float interval);
	bool wantChangeUpState(UpState state, float interval);
	bool wantChangeMove(Move move);

	unsigned int getCurrentDownState();
	unsigned int getCurrentUpState();
protected:
	crInstanceItem* m_this;
	float m_dt;
	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_bot;

	CRParticle::crParticleEffect* m_particleEffectNode;
	CRCore::crSequence* m_sequenceNode;
	//CRAL::crSoundState* m_soundState;

	DownState m_currentDownState;
	DownState m_wantChangeDownState;
	UpState m_currentUpState;
	UpState m_wantChangeUpState;
	float m_currentDownStateStartTime;
	float m_currentUpStateStartTime;
	float m_wantDownStateInterval;
	float m_wantUpStateInterval;
	float m_downStateInterval;
	float m_upStateInterval;
	float m_idleStateInterval;
	Move m_move;

	std::string m_actState;
	float m_actDuration;
};
class CRNETAPP_EXPORT crRpgGameCharacterStateChangeMethod : public CRCore::crMethod
{
public:
	crRpgGameCharacterStateChangeMethod();
	crRpgGameCharacterStateChangeMethod(const crRpgGameCharacterStateChangeMethod& handle);
	MethodClass(CRNetApp, RpgGameCharacterStateChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	crRpgGameCharacterUpdateMethod::DownState m_downState;
	crRpgGameCharacterUpdateMethod::UpState m_upState;
	float m_downInterval;
	float m_upInterval;
	float m_lastUpdTime;
	float m_updInterval;
	float m_actDuration;
	CRCore::ref_ptr<crRpgGameCharacterUpdateMethod> m_characterUpdateMethod;
};
class CRNETAPP_EXPORT crRpgGameCharacterMoveChangeMethod : public CRCore::crMethod
{
public:
	crRpgGameCharacterMoveChangeMethod();
	crRpgGameCharacterMoveChangeMethod(const crRpgGameCharacterMoveChangeMethod& handle);
	MethodClass(CRNetApp, RpgGameCharacterMoveChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	crRpgGameCharacterUpdateMethod::Move m_move;
	CRCore::ref_ptr<crRpgGameCharacterUpdateMethod> m_characterUpdateMethod;
};
}
#endif