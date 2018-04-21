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
#ifndef CRENCAPSULATION_CRMETHODCLASS2_H
#define CRENCAPSULATION_CRMETHODCLASS2_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crMultiSwitch.h>
#include <CRCore/crWidgetNode.h>
//#include <CREncapsulation/crWidgetExtend.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crLightModel.h>
#include <CRParticle/crParticleEffect.h>
#include <CRGUI/crElement.h>
//#include <CRAL/crSoundManager.h>
#include <CRCore/crAITaskManager.h>
#include <CRText/crText.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <rbody/Creature3D/CreBody.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crCameraNode.h>
#include <vector>
namespace CREncapsulation{
//PitchDof
class crPitchDofMethod : public CRCore::crMethod
{
public:
	crPitchDofMethod();
	crPitchDofMethod(const crPitchDofMethod& handle);
	MethodClass(CREncapsulation, PitchDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;

	std::string m_dofName;
	float m_min;
	float m_max;
	//float m_step;
	CRCore::crVector2 m_stepRange;

private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
class crRollDofMethod : public CRCore::crMethod
{
public:
	crRollDofMethod();
	crRollDofMethod(const crRollDofMethod& handle);
	MethodClass(CREncapsulation, RollDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;

	std::string m_dofName;
	float m_min;
	float m_max;
	//float m_step;
	CRCore::crVector2 m_stepRange;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
class crZTransDofMethod : public CRCore::crMethod
{
public:
	crZTransDofMethod();
	crZTransDofMethod(const crZTransDofMethod& handle);
	MethodClass(CREncapsulation, ZTransDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;

	std::string m_dofName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
class crYTransDofMethod : public CRCore::crMethod
{
public:
	crYTransDofMethod();
	crYTransDofMethod(const crYTransDofMethod& handle);
	MethodClass(CREncapsulation, YTransDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;

	std::string m_dofName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
class crStopDofMethod : public CRCore::crMethod
{
public:
	crStopDofMethod();
	crStopDofMethod(const crStopDofMethod& handle);
	MethodClass(CREncapsulation, StopDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;

	std::string m_dofName;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};

class crAircraftSequenceMethod : public CRCore::crMethod
{
public:
	crAircraftSequenceMethod();
	crAircraftSequenceMethod(const crAircraftSequenceMethod& handle);
	MethodClass(CREncapsulation, AircraftSequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;

	std::string m_sequenceName;
	int m_min;
	int m_max;
	bool m_vanishWhenStop;
	bool m_forwardAnim;
private:
	CRCore::ref_ptr<CRCore::crSequence> m_sequence;
};

class crAircraftWeaponSequenceMethod : public CRCore::crMethod
{
public:
	crAircraftWeaponSequenceMethod();
	crAircraftWeaponSequenceMethod(const crAircraftWeaponSequenceMethod& handle);
	MethodClass(CREncapsulation, AircraftWeaponSequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
    _crInt64 m_param;

	std::string m_sequenceName;
	typedef std::set<std::string> WeaponNameSet;
	WeaponNameSet m_weapons;
private:
	CRCore::ref_ptr<CRCore::crSequence> m_sequence;
};

class crAircraftWeaponSequenceExMethod : public CRCore::crMethod
{
public:
	crAircraftWeaponSequenceExMethod();
	crAircraftWeaponSequenceExMethod(const crAircraftWeaponSequenceExMethod& handle);
	MethodClass(CREncapsulation, AircraftWeaponSequenceEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
    _crInt64 m_param;

	std::string m_sequenceName;
	int m_min;
	int m_max;
	bool m_vanishWhenStop;
	bool m_forwardAnim;
	typedef std::set<std::string> WeaponNameSet;
	WeaponNameSet m_weapons;
private:
	CRCore::ref_ptr<CRCore::crSequence> m_sequence;
};

//class crDeadMethod : public CRCore::crMethod
//{
//public:
//	crDeadMethod();
//	crDeadMethod(const crDeadMethod& handle);
//	MethodClass(CREncapsulation, Dead)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//};

//class crPlayMoviesMethod : public CRCore::crMethod
//{
//public:
//	crPlayMoviesMethod();
//	crPlayMoviesMethod(const crPlayMoviesMethod& handle);
//	MethodClass(CREncapsulation, PlayMovies)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//    _crInt64 m_param;
//
//	typedef std::vector<int> MoviesVec;
//	MoviesVec m_movies;
//	int m_type;
//};

//class crCameraBotMoviesMethod : public CRCore::crMethod
//{
//public:
//	crCameraBotMoviesMethod();
//	crCameraBotMoviesMethod(const crCameraBotMoviesMethod& handle);
//	MethodClass(CREncapsulation, CameraBotMovies)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//    _crInt64 m_param;
//
//	typedef std::vector<int> MoviesVec;
//	MoviesVec m_movies;
//};

class crPlaySequenceMethod : public CRCore::crMethod
{
public:
	crPlaySequenceMethod();
	crPlaySequenceMethod(const crPlaySequenceMethod& handle);
	MethodClass(CREncapsulation, PlaySequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	std::string m_effectName;
private:
	CRCore::ref_ptr<CRCore::crSequence> m_sequenceNode;
};

//class crPlaySoundMethod : public CRCore::crMethod
//{
//public:
//	crPlaySoundMethod();
//	crPlaySoundMethod(const crPlaySoundMethod& handle);
//	MethodClass(CREncapsulation, PlaySound)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//	std::string m_effectName;
//private:
//	CRCore::ref_ptr<CRAL::crSoundState> m_soundState;
//};

class crCharacterActionMethod : public CRCore::crMethod
{
public:
	crCharacterActionMethod();
	crCharacterActionMethod(const crCharacterActionMethod& handle);
	MethodClass(CREncapsulation, CharacterAction)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	_crInt64 m_param;
	std::string m_actionName;
	bool m_prioritized;
	bool m_force;
};

//class crGndActionEffectMethod : public CRCore::crMethod
//{
//public:
//	crGndActionEffectMethod();
//	crGndActionEffectMethod(const crGndActionEffectMethod& handle);
//	MethodClass(CREncapsulation, GndActionEffect)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	enum ActionMask
//	{
//		GND = 0x01,
//		AIR = 0x02,
//		WATER = 0x04,
//		ALL = GND | AIR | WATER
//	};
//protected:
//	CRPhysics::crViewMatterObject* m_this;
//
//	CRParticle::crParticleEffect* m_particleEffectNode;
//	CRCore::crSequence* m_sequenceNode;
//	CRAL::crSoundState* m_soundState;
//	std::string m_actionAttrStr;
//	std::string m_bodyAttrStr;
//	ActionMask m_actionMask;
//};
class crStopActionMethod : public CRCore::crMethod
{
public:
	crStopActionMethod();
	crStopActionMethod(const crStopActionMethod& handle);
	MethodClass(CREncapsulation, StopAction)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	std::string m_actionName;
};
////////FPS
class crFpsCharacterUpdateMethod : public CRCore::crMethod
{
public:
	crFpsCharacterUpdateMethod();
	crFpsCharacterUpdateMethod(const crFpsCharacterUpdateMethod& handle);
	MethodClass(CREncapsulation, FpsCharacterUpdate)
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
		DS_Sprint,
		DS_Run,
		DS_Walk,
		DS_Crouch,
		DS_Crawl,
		DS_Dead
	};
	enum UpState
	{
		US_None,
		US_WeaponFire,
		US_ReplaceClip,
		US_WeaponUnSelect,
		US_WeaponSelect,
		US_WeaponAim,
		US_WeaponUnAim
	};

	enum Move
	{
		M_None,
		StopMove,
		MoveUp,
		MoveDown,
		RunUp,
		RunDown,
		SprintUp,
		SprintDown,
	};

	enum Pan
	{
		P_None,
		StopPan,
		MovePanLeft,
		MovePanRight,
		RunPanLeft,
		RunPanRight,
		SprintPanLeft,
		SprintPanRight
	};

	bool wantChangeDownState(DownState state, float interval);
	bool wantChangeUpState(UpState state, float interval);
	bool wantChangeMove(Move move);
	bool wantChangePan(Pan pan);

	unsigned int getCurrentDownState();
	unsigned int getCurrentUpState();

	float getSprintSpeed();
	float getRunSpeed();
	float getWalkSpeed();
	float getCrouchSpeed();
	float getCrawlSpeed();
	float getSprintPanSpeed();
	float getRunPanSpeed();
	float getWalkPanSpeed();
	float getCrouchPanSpeed();
	float getCrawlPanSpeed();
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
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

	bool m_isInWeaponAim;
	Move m_move;
	Pan m_pan; 
protected://EventParam
	float m_sprintSpeed;
	float m_runSpeed;
	float m_walkSpeed;
	float m_crouchSpeed;
	float m_crawlSpeed;

	float m_sprintPanSpeed;
	float m_runPanSpeed;
	float m_walkPanSpeed;
	float m_crouchPanSpeed;
	float m_crawlPanSpeed;

	//Height - walkableHeight - blockRadius * 2.0f > 0
	float m_standHeight;
	float m_crouchHeight;
	float m_crawlHeight;
	float m_walkableHeight;
	float m_crouchableHeight;
	float m_crawlableHeight;

	float m_sprintContinuableTime;
	float m_turnToIdleTime;

	float m_aimSpeedScale;
	//std::string m_actionName;
protected:
	void init();
	std::string m_postDofName[4];
	CRCore::crVector3 m_postDofParam[3];//stand = 0,Crouch = 1,Crawl=2
	float m_postTargetOffset;
	//float m_min;
	//float m_max;
	//float m_step;
	float m_fireScale;
	bool m_init;	
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_postDof[4];
};

class crFpsCharacterStateChangeMethod : public CRCore::crMethod
{
public:
	crFpsCharacterStateChangeMethod();
	crFpsCharacterStateChangeMethod(const crFpsCharacterStateChangeMethod& handle);
	MethodClass(CREncapsulation, FpsCharacterStateChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	crFpsCharacterUpdateMethod::DownState m_downState;
	crFpsCharacterUpdateMethod::UpState m_upState;
	float m_downInterval;
	float m_upInterval;
	float m_lastUpdTime;
	float m_updInterval;
	CRCore::ref_ptr<crFpsCharacterUpdateMethod> m_characterUpdateMethod;
};
class crFpsCharacterJumpMethod : public CRCore::crMethod
{
public:
	crFpsCharacterJumpMethod();
	crFpsCharacterJumpMethod(const crFpsCharacterJumpMethod& handle);
	MethodClass(CREncapsulation, FpsCharacterJump)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_jumpVel;
	float m_timeInterval;
};
class crFpsCharacterMoveChangeMethod : public CRCore::crMethod
{
public:
	crFpsCharacterMoveChangeMethod();
	crFpsCharacterMoveChangeMethod(const crFpsCharacterMoveChangeMethod& handle);
	MethodClass(CREncapsulation, FpsCharacterMoveChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<crFpsCharacterUpdateMethod> m_characterUpdateMethod;
	crFpsCharacterUpdateMethod::Move m_move;
	crFpsCharacterUpdateMethod::Pan m_pan;
};
////////RPG
class crRpgCharacterUpdateMethod : public CRCore::crMethod
{
public:
	crRpgCharacterUpdateMethod();
	crRpgCharacterUpdateMethod(const crRpgCharacterUpdateMethod& handle);
	MethodClass(CREncapsulation, RpgCharacterUpdate)
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
		DS_Sprint,
		DS_Run,
		DS_Walk,
		DS_Crouch,
		DS_Crawl,
		DS_Dead
	};
	enum UpState
	{
		US_None,
		US_WeaponFire,
		US_ReplaceClip,
		US_WeaponUnSelect,
		US_WeaponSelect,
		US_WeaponAim,
		US_WeaponUnAim
	};

	enum Move
	{
		M_None,
		StopMove,
		MoveUp,
		MoveDown,
		RunUp,
		RunDown,
		SprintUp,
		SprintDown,
	};

	enum Pan
	{
		P_None,
		StopPan,
		MovePanLeft,
		MovePanRight,
		RunPanLeft,
		RunPanRight,
		SprintPanLeft,
		SprintPanRight
	};

	bool wantChangeDownState(DownState state, float interval);
	bool wantChangeUpState(UpState state, float interval);
	bool wantChangeMove(Move move);
	bool wantChangePan(Pan pan);

	unsigned int getCurrentDownState();
	unsigned int getCurrentUpState();

	float getSprintSpeed();
	float getRunSpeed();
	float getWalkSpeed();
	float getCrouchSpeed();
	float getCrawlSpeed();
	float getSprintPanSpeed();
	float getRunPanSpeed();
	float getWalkPanSpeed();
	float getCrouchPanSpeed();
	float getCrawlPanSpeed();
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
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

	bool m_isInWeaponAim;
	Move m_move;
	Pan m_pan; 
protected://EventParam
	float m_sprintSpeed;
	float m_runSpeed;
	float m_walkSpeed;
	float m_crouchSpeed;
	float m_crawlSpeed;

	float m_sprintPanSpeed;
	float m_runPanSpeed;
	float m_walkPanSpeed;
	float m_crouchPanSpeed;
	float m_crawlPanSpeed;

	//Height - walkableHeight - blockRadius * 2.0f > 0
	float m_standHeight;
	float m_crouchHeight;
	float m_crawlHeight;
	float m_walkableHeight;
	float m_crouchableHeight;
	float m_crawlableHeight;

	float m_sprintContinuableTime;
	float m_turnToIdleTime;

	float m_aimSpeedScale;
	//std::string m_actionName;
protected:
	//void init();
	//std::string m_postDofName[4];
	//CRCore::crVector3 m_postDofParam[3];//stand = 0,Crouch = 1,Crawl=2
	//float m_postTargetOffset;
	//float m_min;
	//float m_max;
	//float m_step;
	//float m_fireScale;
	//bool m_init;	
//private:
//	CRCore::ref_ptr<CRCore::crDOFTransform> m_postDof[4];
};
class crRpgCharacterStateChangeMethod : public CRCore::crMethod
{
public:
	crRpgCharacterStateChangeMethod();
	crRpgCharacterStateChangeMethod(const crRpgCharacterStateChangeMethod& handle);
	MethodClass(CREncapsulation, RpgCharacterStateChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	crRpgCharacterUpdateMethod::DownState m_downState;
	crRpgCharacterUpdateMethod::UpState m_upState;
	float m_downInterval;
	float m_upInterval;
	float m_lastUpdTime;
	float m_updInterval;
	CRCore::ref_ptr<crRpgCharacterUpdateMethod> m_characterUpdateMethod;
};
class crRpgCharacterJumpMethod : public CRCore::crMethod
{
public:
	crRpgCharacterJumpMethod();
	crRpgCharacterJumpMethod(const crRpgCharacterJumpMethod& handle);
	MethodClass(CREncapsulation, RpgCharacterJump)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_jumpVel;
	float m_timeInterval;
};
class crRpgCharacterMoveChangeMethod : public CRCore::crMethod
{
public:
	crRpgCharacterMoveChangeMethod();
	crRpgCharacterMoveChangeMethod(const crRpgCharacterMoveChangeMethod& handle);
	MethodClass(CREncapsulation, RpgCharacterMoveChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<crRpgCharacterUpdateMethod> m_characterUpdateMethod;
	crRpgCharacterUpdateMethod::Move m_move;
	crRpgCharacterUpdateMethod::Pan m_pan;
};
///////////////////
class crDialogTalkMethod : public CRCore::crMethod
{
public:
	crDialogTalkMethod();
	crDialogTalkMethod(const crDialogTalkMethod& handle);
	MethodClass(CREncapsulation, DialogTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::ref_ptr<CRCore::crNode> m_node;
	std::string m_dialogID;
	std::string m_talkString;
};

class crCloseAllDialogMethod : public CRCore::crMethod
{
public:
	crCloseAllDialogMethod();
	crCloseAllDialogMethod(const crCloseAllDialogMethod& handle);
	MethodClass(CREncapsulation, CloseAllDialog)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};

class crShowManipulatorDialogsMethod : public CRCore::crMethod
{
public:
	crShowManipulatorDialogsMethod();
	crShowManipulatorDialogsMethod(const crShowManipulatorDialogsMethod& handle);
	MethodClass(CREncapsulation, ShowManipulatorDialogs)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<std::string> DialogIDVec;
	DialogIDVec m_dialogIDVec;
};

class crFilterSwitchMethod : public CRCore::crMethod
{
public:
	crFilterSwitchMethod();
	crFilterSwitchMethod(const crFilterSwitchMethod& handle);
	MethodClass(CREncapsulation, FilterSwitch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_switchName;
	int m_needActiveID;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_switch;
};

class crShowFilterNodeMethod : public CRCore::crMethod
{
public:
	crShowFilterNodeMethod();
	crShowFilterNodeMethod(const crShowFilterNodeMethod& handle);
	MethodClass(CREncapsulation, ShowFilterNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	bool m_visiable;
	CRCore::ref_ptr<CRCore::crNode> m_filterNode;
};

class crNodeSwitchMethod : public CRCore::crMethod
{
public:
	crNodeSwitchMethod();
	crNodeSwitchMethod(const crNodeSwitchMethod& handle);
	MethodClass(CREncapsulation, NodeSwitch)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	std::string m_switchName;
	int m_needActiveID;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_switch;
};
class crSwitchActiveMethod : public CRCore::crMethod
{
public:
	crSwitchActiveMethod();
	crSwitchActiveMethod(const crSwitchActiveMethod& handle);
	MethodClass(CREncapsulation, SwitchActive)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMultiSwitch* m_this;
	int m_needActiveID;
};
class crFilterNodeEventMethod : public CRCore::crMethod
{
public:
	crFilterNodeEventMethod();
	crFilterNodeEventMethod(const crFilterNodeEventMethod& handle);
	MethodClass(CREncapsulation, FilterNodeEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	std::string m_eventMsg1;
	std::string m_eventMsg2;
	std::string m_eventParam1;
	std::string m_eventParam2;
	_crInt64 m_eventMsg;
	_crInt64 m_eventParam;
	CRCore::ref_ptr<CRCore::crNode> m_eventNode;
};
class crNodeEventMethod : public CRCore::crMethod
{
public:
	crNodeEventMethod();
	crNodeEventMethod(const crNodeEventMethod& handle);
	MethodClass(CREncapsulation, NodeEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	std::string m_nodeName;
	std::string m_eventMsg1;
	std::string m_eventMsg2;
	std::string m_eventParam1;
	std::string m_eventParam2;
	_crInt64 m_eventMsg;
	_crInt64 m_eventParam;
	CRCore::ref_ptr<CRCore::crNode> m_eventNode;
};
//class crSetEndCodeMethod : public CRCore::crMethod
//{
//public:
//	crSetEndCodeMethod();
//	crSetEndCodeMethod(const crSetEndCodeMethod& handle);
//	MethodClass(CREncapsulation, SetEndCode)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//	int m_endCode;
//};
//class crEffectParticleMethod : public CRCore::crMethod
//{
//public:
//	crEffectParticleMethod();
//	crEffectParticleMethod(const crEffectParticleMethod& handle);
//	MethodClass(CREncapsulation, EffectParticle)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//	_crInt64 m_param;
//	std::string m_effectName;
//	float m_windPower;
//};
class crEffectSequenceMethod : public CRCore::crMethod
{
public:
	crEffectSequenceMethod();
	crEffectSequenceMethod(const crEffectSequenceMethod& handle);
	MethodClass(CREncapsulation, EffectSequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
	std::string m_effectName;
	float m_offset;
	CRCore::crVector3 m_positionOffset;
};
class crEffectLightMethod : public CRCore::crMethod
{
public:
	crEffectLightMethod();
	crEffectLightMethod(const crEffectLightMethod& handle);
	MethodClass(CREncapsulation, EffectLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
	std::string m_effectName;
};
//class crEffectSoundMethod : public CRCore::crMethod
//{
//public:
//	crEffectSoundMethod();
//	crEffectSoundMethod(const crEffectSoundMethod& handle);
//	MethodClass(CREncapsulation, EffectSound)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//	_crInt64 m_param;
//	std::string m_effectName;
//};
class crXTransTMDofMethod : public CRCore::crMethod
{
public:
	crXTransTMDofMethod();
	crXTransTMDofMethod(const crXTransTMDofMethod& handle);
	MethodClass(CREncapsulation, XTransTMDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
	std::string m_nodeName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};
class crYTransTMDofMethod : public CRCore::crMethod
{
public:
	crYTransTMDofMethod();
	crYTransTMDofMethod(const crYTransTMDofMethod& handle);
	MethodClass(CREncapsulation, YTransTMDof)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
	std::string m_nodeName;
	float m_min;
	float m_max;
	float m_step;
private:
	CRCore::ref_ptr<CRCore::crDOFTransform> m_dof;
};

class crMoveMethod : public CRCore::crMethod
{//WCH_KEYDOWN/WCH_KEYUP,WCH_KEY_W/WCH_KEY_S/WCH_KEY_UP/WCH_KEY_DOWN
public:
	crMoveMethod();
	crMoveMethod(const crMoveMethod& handle);
	MethodClass(CREncapsulation, Move)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_velocity;
};

class crPanMethod : public CRCore::crMethod
{//WCH_KEYDOWN/WCH_KEYUP,WCH_KEY_A/WCH_KEY_D
public:
	crPanMethod();
	crPanMethod(const crPanMethod& handle);
	MethodClass(CREncapsulation, Pan)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_velocity;
};

class crRaiseMethod : public CRCore::crMethod
{
public:
	crRaiseMethod();
	crRaiseMethod(const crRaiseMethod& handle);
	MethodClass(CREncapsulation, Raise)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_velocity;
};

class crRotateMethod : public CRCore::crMethod
{//WCH_KEYDOWN/WCH_KEYUP,WCH_KEY_LEFT/WCH_KEY_RIGHT
public:
	crRotateMethod();
	crRotateMethod(const crRotateMethod& handle);
	MethodClass(CREncapsulation, Rotate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	float m_h;
};

class crPointSelectMethod : public CRCore::crMethod
{
public:
	crPointSelectMethod();
	crPointSelectMethod(const crPointSelectMethod& handle);
	MethodClass(CREncapsulation, PointSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};

class crShowCursorMethod : public CRCore::crMethod
{
public:
	crShowCursorMethod();
	crShowCursorMethod(const crShowCursorMethod& handle);
	MethodClass(CREncapsulation, ShowCursor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_cursorName;
};

class crMouseControlViewMethod : public CRCore::crMethod
{
public:
	crMouseControlViewMethod();
	crMouseControlViewMethod(const crMouseControlViewMethod& handle);
	MethodClass(CREncapsulation, MouseControlView)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
    enum RotMode
	{
		XRot = 0x1,
		YRot = 0x2,
		XYRot = XRot|YRot
	};
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;

	CRCore::crVector2f m_turn_angle;
	float m_time;
	RotMode m_rotMode;
};

class crSetEditorCoordinateMethod : public CRCore::crMethod
{
public:
	crSetEditorCoordinateMethod();
	crSetEditorCoordinateMethod(const crSetEditorCoordinateMethod& handle);
	MethodClass(CREncapsulation, SetEditorCoordinate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
    CRCore::ref_ptr<CRCore::crMatrixTransform> m_editorCoordinate;
};

class crConvertToTransformMethod : public CRCore::crMethod
{
public:
	crConvertToTransformMethod();
	crConvertToTransformMethod(const crConvertToTransformMethod& handle);
	MethodClass(CREncapsulation, ConvertToTransform)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
};

class crPanNodeMethod : public CRCore::crMethod
{
public:
	crPanNodeMethod();
	crPanNodeMethod(const crPanNodeMethod& handle);
	MethodClass(CREncapsulation, PanNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
	int m_keyboard;
};

class crRotNodeMethod : public CRCore::crMethod
{
public:
	crRotNodeMethod();
	crRotNodeMethod(const crRotNodeMethod& handle);
	MethodClass(CREncapsulation, RotNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
    int m_keyboard;
};

class crScaleNodeMethod : public CRCore::crMethod
{
public:
	crScaleNodeMethod();
	crScaleNodeMethod(const crScaleNodeMethod& handle);
	MethodClass(CREncapsulation, ScaleNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	//_crInt64 m_param;
	std::vector<std::string> m_stageNames;
	std::string m_scaleAllf;
	std::string m_scaleXf;
	std::string m_scaleYf;
	std::string m_scaleZf;
};

class crSpawnNodeMethod : public CRCore::crMethod
{
public:
	crSpawnNodeMethod();
	crSpawnNodeMethod(const crSpawnNodeMethod& handle);
	MethodClass(CREncapsulation, SpawnNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRCore::crGroup> m_parent;
};

class crDeleteNodeMethod : public CRCore::crMethod
{
public:
	crDeleteNodeMethod();
	crDeleteNodeMethod(const crDeleteNodeMethod& handle);
	MethodClass(CREncapsulation, DeleteNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRCore::crGroup> m_parent;
};

class crAdjustEditorNodeMethod : public CRCore::crMethod
{
public:
	crAdjustEditorNodeMethod();
	crAdjustEditorNodeMethod(const crAdjustEditorNodeMethod& handle);
	MethodClass(CREncapsulation, AdjustEditorNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRCore::crNode> m_effectNode;
};
class crFocusNodeMethod : public CRCore::crMethod
{
public:
	crFocusNodeMethod();
	crFocusNodeMethod(const crFocusNodeMethod& handle);
	MethodClass(CREncapsulation, FocusNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
    CRCore::ref_ptr<CRCore::crNode> m_node;
};

class crSelectNodeMaterialMethod : public CRCore::crMethod
{
public:
	crSelectNodeMaterialMethod();
	crSelectNodeMaterialMethod(const crSelectNodeMaterialMethod& handle);
	MethodClass(CREncapsulation, SelectNodeMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
 //   CRCore::ref_ptr<CRCore::crStateSet> m_selectStateSet;
	//CRCore::ref_ptr<CRCore::crStateSet> m_nodeStateSet;
	//CRCore::ref_ptr<CRCore::crLightModel> m_lightModel;
};

class crUnHiddenNodeVisitor : public CRCore::crNodeVisitor
{
public:
	crUnHiddenNodeVisitor(const std::string &exceptNodeNames):m_exceptNodeNames(exceptNodeNames),CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crUnHiddenNodeVisitor(){}
	virtual void apply(CRCore::crNode& node);
protected:
	std::string m_exceptNodeNames;
};
class crHiddenNodeVisitor : public CRCore::crNodeVisitor
{
public:
	crHiddenNodeVisitor():CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crHiddenNodeVisitor(){}
	virtual void apply(CRCore::crNode& node);
	virtual void apply(CRCore::crObject& node);
	void pushUnHideNodeSet(CRCore::crNode *node);
	void reset();
protected:
	std::set< CRCore::ref_ptr<CRCore::crNode> > m_unHideNodeSet;
};

class crHiddenOrUnHiddenNodesMethod : public CRCore::crMethod
{
public:
	crHiddenOrUnHiddenNodesMethod();
	crHiddenOrUnHiddenNodesMethod(const crHiddenOrUnHiddenNodesMethod& handle);
	MethodClass(CREncapsulation, HiddenOrUnHiddenNodes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_hidden;//0表示显示所有，1表示隐藏当前选中的，2表示隐藏未选中的
};

class crHideNodeMethod : public CRCore::crMethod
{
public:
	crHideNodeMethod();
	crHideNodeMethod(const crHideNodeMethod& handle);
	MethodClass(CREncapsulation, HideNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crUnHideAllMethod : public CRCore::crMethod
{
public:
	crUnHideAllMethod();
	crUnHideAllMethod(const crUnHideAllMethod& handle);
	MethodClass(CREncapsulation, UnHideAll)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_exceptNodeNames;
};

class crShowAllUnRenderableNodeVisitor : public CRCore::crNodeVisitor
{
public:
	crShowAllUnRenderableNodeVisitor(bool show, const std::string &exceptNodeNames):m_show(show),m_exceptNodeNames(exceptNodeNames),CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crShowAllUnRenderableNodeVisitor(){}
	virtual void apply(CRCore::crNode& node);
protected:
	std::string m_exceptNodeNames;
	bool m_show;
};

class crShowAllUnRenderableNodeMethod : public CRCore::crMethod
{
public:
	crShowAllUnRenderableNodeMethod();
	crShowAllUnRenderableNodeMethod(const crShowAllUnRenderableNodeMethod& handle);
	MethodClass(CREncapsulation, ShowAllUnRenderableNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
	std::string m_exceptNodeNames;
};

//class crLightListSelectMethod : public CRCore::crMethod
//{//
//public:
//	crLightListSelectMethod();
//	crLightListSelectMethod(const crLightListSelectMethod& handle);
//	MethodClass(CREncapsulation, LightListSelect)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_stageDlgString;
//	std::string m_lightListElement;
//};
class crNodeListSelectMethod : public CRCore::crMethod
{//
public:
	crNodeListSelectMethod();
	crNodeListSelectMethod(const crNodeListSelectMethod& handle);
	MethodClass(CREncapsulation, NodeListSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageDlgString;
	std::string m_nodeListElement;
};
//
//class crPlaceLoadedNodeMethod : public CRCore::crMethod
//{//
//public:
//	crPlaceLoadedNodeMethod();
//	crPlaceLoadedNodeMethod(const crPlaceLoadedNodeMethod& handle);
//	MethodClass(CREncapsulation, PlaceLoadedNode)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
//};
class crEnablePhysicsMethod : public CRCore::crMethod
{//
public:
	crEnablePhysicsMethod();
	crEnablePhysicsMethod(const crEnablePhysicsMethod& handle);
	MethodClass(CREncapsulation, EnablePhysics)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_interval;
};
class crRemoveMatrixMethod : public CRCore::crMethod
{//
public:
	crRemoveMatrixMethod();
	crRemoveMatrixMethod(const crRemoveMatrixMethod& handle);
	MethodClass(CREncapsulation, RemoveMatrix)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crFreezeMatrixMethod : public CRCore::crMethod
{//
public:
	crFreezeMatrixMethod();
	crFreezeMatrixMethod(const crFreezeMatrixMethod& handle);
	MethodClass(CREncapsulation, FreezeMatrix)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crInsertNodeMethod : public CRCore::crMethod
{//
public:
	crInsertNodeMethod();
	crInsertNodeMethod(const crInsertNodeMethod& handle);
	MethodClass(CREncapsulation, InsertNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crRemoveNodeMethod : public CRCore::crMethod
{//
public:
	crRemoveNodeMethod();
	crRemoveNodeMethod(const crRemoveNodeMethod& handle);
	MethodClass(CREncapsulation, RemoveNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crShowCameraPosMethod : public CRCore::crMethod
{
public:
	crShowCameraPosMethod();
	crShowCameraPosMethod(const crShowCameraPosMethod& handle);
	MethodClass(CREncapsulation, ShowCameraPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_cameraPosText;
	std::string m_referencePosText;
	std::string m_drawableInfoText;
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_editorCoordinate;
};

class crPickTargetMethod : public CRCore::crMethod
{//
public:
	crPickTargetMethod();
	crPickTargetMethod(const crPickTargetMethod& handle);
	MethodClass(CREncapsulation, PickTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
    CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};
class crMouseMoveCursorMethod : public CRCore::crMethod
{//
public:
	crMouseMoveCursorMethod();
	crMouseMoveCursorMethod(const crMouseMoveCursorMethod& handle);
	MethodClass(CREncapsulation, MouseMoveCursor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	_crInt64 m_param;
	std::string m_enemyCursor;
	std::string m_friendCursor;
	std::string m_neutralityCursor;
};
class crTurnToTargetMethod : public CRCore::crMethod
{//
public:
	crTurnToTargetMethod();
	crTurnToTargetMethod(const crTurnToTargetMethod& handle);
	MethodClass(CREncapsulation, TurnToTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::crVector3 m_targerPosition;
};
class crMoveToTargetMethod : public CRCore::crMethod
{//
public:
	crMoveToTargetMethod();
	crMoveToTargetMethod(const crMoveToTargetMethod& handle);
	MethodClass(CREncapsulation, MoveToTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void init();
	CRPhysics::crViewMatterObject* m_this;
	CRCore::crVector3 m_targerPosition;

	CRCore::ref_ptr<CRCore::crAITask> m_aiTask;
	float m_velocity;
	CRCore::Timer_t m_time;
	bool m_init;
};

class crBotAISelectTargetMethod : public CRCore::crMethod
{//WCH_UPDATEVISITOR 
public:
	crBotAISelectTargetMethod();
	crBotAISelectTargetMethod(const crBotAISelectTargetMethod& handle);
	MethodClass(CREncapsulation, BotAISelectTarget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
    float m_time_lost;
	float m_interval_lost;
};

//class crBotAIPatrolMethod : public CRCore::crMethod
//{//WCH_UPDATEVISITOR 
//public:
//	crBotAIPatrolMethod();
//	crBotAIPatrolMethod(const crBotAIPatrolMethod& handle);
//	MethodClass(CREncapsulation, BotAIPatrol)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	float computeAngle(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir);
//	void turnToDir(CRCore::crEventGroup *bodyNC,const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir);
//
//	CRPhysics::crViewMatterObject* m_this;
//	CRCore::ref_ptr<CRCore::crAITask> m_aiTask;
//
//	float m_time_aimsg;
//	bool m_isAircraft;
//
//	int m_currentIndex;
//	unsigned int m_currentMsgIndex;
//	float m_interval_aimsg;
//	CRCore::crControlPoint *m_preControlPoint;
//};

class crShowBotHeadInfoMethod : public CRCore::crMethod
{//WCH_UPDATEVISITOR 
public:
	crShowBotHeadInfoMethod();
	crShowBotHeadInfoMethod(const crShowBotHeadInfoMethod& handle);
	MethodClass(CREncapsulation, ShowBotHeadInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	virtual ~crShowBotHeadInfoMethod();
	static std::string getNameTextStr();
	CRCore::ref_ptr<CRCore::crNode> m_node;
	int m_showflg;//0:None,1:name,2:blood,3:name&blood
	float m_maxRange;
	float m_zoffset;
	CRCore::ref_ptr<CRText::crText> m_nameText;
};

class crChangeViewOffsetMethod : public CRCore::crMethod
{//
public:
	crChangeViewOffsetMethod();
	crChangeViewOffsetMethod(const crChangeViewOffsetMethod& handle);
	MethodClass(CREncapsulation, ChangeViewOffset)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_step;
	CRCore::crVector2f m_range;
};

class crChangeMoveSpeedMethod : public CRCore::crMethod
{//
public:
	crChangeMoveSpeedMethod();
	crChangeMoveSpeedMethod(const crChangeMoveSpeedMethod& handle);
	MethodClass(CREncapsulation, ChangeMoveSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_step;
	CRCore::crVector2f m_range;
};

//class crBotAIAvoidCollideMethod : public CRCore::crMethod
//{//WCH_UPDATEVISITOR 
//public:
//	crBotAIAvoidCollideMethod();
//	crBotAIAvoidCollideMethod(const crBotAIAvoidCollideMethod& handle);
//	MethodClass(CREncapsulation, BotAIAvoidCollide)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRCore::crNode> m_node;
//	float m_time_ac;
//	float m_interval_ac;
//	bool m_avoidCollide;
//};

class crTalkWithBotMethod : public CRCore::crMethod
{// 
public:
	crTalkWithBotMethod();
	crTalkWithBotMethod(const crTalkWithBotMethod& handle);
	MethodClass(CREncapsulation, TalkWithBot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
    CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_targetNode;
};

class crTalkEndMethod : public CRCore::crMethod
{// 
public:
	crTalkEndMethod();
	crTalkEndMethod(const crTalkEndMethod& handle);
	MethodClass(CREncapsulation, TalkEnd)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
};

class crDoEventMethod : public CRCore::crMethod
{// 
public:
	crDoEventMethod();
	crDoEventMethod(const crDoEventMethod& handle);
	MethodClass(CREncapsulation, DoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	_crInt32 m_msgParam1;
	_crInt32 m_msgParam2;
};
class crTargetDoEventMethod : public CRCore::crMethod
{// 
public:
	crTargetDoEventMethod();
	crTargetDoEventMethod(const crTargetDoEventMethod& handle);
	MethodClass(CREncapsulation, TargetDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	_crInt32 m_msgParam1;
	_crInt32 m_msgParam2;
};

class crMudererDoEventMethod : public CRCore::crMethod
{// 
public:
	crMudererDoEventMethod();
	crMudererDoEventMethod(const crMudererDoEventMethod& handle);
	MethodClass(CREncapsulation, MudererDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	_crInt64 m_param;
	_crInt32 m_msgParam1;
	_crInt32 m_msgParam2;
};

class crReviveMosterMethod : public CRCore::crMethod
{// 
public:
	crReviveMosterMethod();
	crReviveMosterMethod(const crReviveMosterMethod& handle);
	MethodClass(CREncapsulation, ReviveMoster)
    virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_sceneMapID;
	int m_mapAreaID;
	int m_npcTroopID;//-1表示随机
	int m_revivePositionID;
};

class crIsStageOnShowMethod : public CRCore::crMethod
{// 
public:
	crIsStageOnShowMethod();
	crIsStageOnShowMethod(const crIsStageOnShowMethod& handle);
	MethodClass(CREncapsulation, IsStageOnShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
};

class crRackCharacterMeshMethod : public CRCore::crMethod
{//替换或装配蒙皮
public:
	crRackCharacterMeshMethod();
	crRackCharacterMeshMethod(const crRackCharacterMeshMethod& handle);
	MethodClass(CREncapsulation, RackCharacterMesh)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fileName;
};

class crLoadAnimMethod : public CRCore::crMethod
{//
public:
	crLoadAnimMethod();
	crLoadAnimMethod(const crLoadAnimMethod& handle);
	MethodClass(CREncapsulation, LoadAnim)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void loadAttributesCommonToAllAnimations(rcfg::ConfigScript &cfg_script, rbody::AnimationBase *anim);
	std::string m_fileName;
};

class crSelectNodeTypeCaseMethod : public CRCore::crMethod
{//
public:
	crSelectNodeTypeCaseMethod();
	crSelectNodeTypeCaseMethod(const crSelectNodeTypeCaseMethod& handle);
	MethodClass(CREncapsulation, SelectNodeTypeCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crTerrainEditMethod : public CRCore::crMethod
{//
public:
	crTerrainEditMethod();
	crTerrainEditMethod(const crTerrainEditMethod& handle);
	MethodClass(CREncapsulation, TerrainEdit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};

class crColorWeightEditMethod : public CRCore::crMethod
{//
public:
	crColorWeightEditMethod();
	crColorWeightEditMethod(const crColorWeightEditMethod& handle);
	MethodClass(CREncapsulation, ColorWeightEdit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};
class crTerrainEditDecalMethod : public CRCore::crMethod
{//
public:
	crTerrainEditDecalMethod();
	crTerrainEditDecalMethod(const crTerrainEditDecalMethod& handle);
	MethodClass(CREncapsulation, TerrainEditDecal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};
class crSaveHeightMapMethod : public CRCore::crMethod
{//
public:
	crSaveHeightMapMethod();
	crSaveHeightMapMethod(const crSaveHeightMapMethod& handle);
	MethodClass(CREncapsulation, SaveHeightMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_path;
	std::string m_heightMap;
};

class crSaveColorWeightMapMethod : public CRCore::crMethod
{//
public:
	crSaveColorWeightMapMethod();
	crSaveColorWeightMapMethod(const crSaveColorWeightMapMethod& handle);
	MethodClass(CREncapsulation, SaveColorWeightMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_path;
	std::string m_colorWeightMap;
};

class crShowTerrainEditDecalMethod : public CRCore::crMethod
{//
public:
	crShowTerrainEditDecalMethod();
	crShowTerrainEditDecalMethod(const crShowTerrainEditDecalMethod& handle);
	MethodClass(CREncapsulation, ShowTerrainEditDecal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRTerrain::crTerrainTile> m_terrain;
	CRCore::crVector3 m_hitPoint;
	CRCore::ref_ptr<CRCore::crHeightField> m_heightField;
	CRCore::ref_ptr<CRCore::crShapeDrawable> m_shapeDrawable;
	CRCore::ref_ptr<CRCore::crObject> m_object;
	std::string m_rootName;
	CRCore::crGroup* m_root;
	std::string m_textureName;
};

class crHideTerrainEditDecalMethod : public CRCore::crMethod
{//
public:
	crHideTerrainEditDecalMethod();
	crHideTerrainEditDecalMethod(const crHideTerrainEditDecalMethod& handle);
	MethodClass(CREncapsulation, HideTerrainEditDecal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rootName;
	CRCore::crGroup* m_root;
};

class crMoveWidgetMethod : public CRCore::crMethod
{
public:
	crMoveWidgetMethod();
	crMoveWidgetMethod(const crMoveWidgetMethod& handle);
	MethodClass(CREncapsulation, MoveWidget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::crVector2 m_lastMouse;
	float m_time;
};

class crMoveParentCanvasMethod : public CRCore::crMethod
{//用于窗口标题栏
public:
	crMoveParentCanvasMethod();
	crMoveParentCanvasMethod(const crMoveParentCanvasMethod& handle);
	MethodClass(CREncapsulation, MoveParentCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::crVector2 m_lastMouse;
	float m_time;
};

class crCloseParentCanvasMethod : public CRCore::crMethod
{
public:
	crCloseParentCanvasMethod();
	crCloseParentCanvasMethod(const crCloseParentCanvasMethod& handle);
	MethodClass(CREncapsulation, CloseParentCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crCloseMeMethod : public CRCore::crMethod
{
public:
	crCloseMeMethod();
	crCloseMeMethod(const crCloseMeMethod& handle);
	MethodClass(CREncapsulation, CloseMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};
class crWidgetShowMethod : public CRCore::crMethod
{
public:
	crWidgetShowMethod();
	crWidgetShowMethod(const crWidgetShowMethod& handle);
	MethodClass(CREncapsulation, WidgetShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	char m_show;
};

class crShowCanvasMethod : public CRCore::crMethod
{
public:
	crShowCanvasMethod();
	crShowCanvasMethod(const crShowCanvasMethod& handle);
	MethodClass(CREncapsulation, ShowCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	char m_show;
};

class crWidgetUpdateMethod : public CRCore::crMethod
{
public:
	crWidgetUpdateMethod();
	crWidgetUpdateMethod(const crWidgetUpdateMethod& handle);
	MethodClass(CREncapsulation, WidgetUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_widgetName;
};
class crWidgetUpdate2Method : public CRCore::crMethod
{
public:
	crWidgetUpdate2Method();
	crWidgetUpdate2Method(const crWidgetUpdate2Method& handle);
	MethodClass(CREncapsulation, WidgetUpdate2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_widgetName;
};
class crWidgetUpdate3Method : public CRCore::crMethod
{
public:
	crWidgetUpdate3Method();
	crWidgetUpdate3Method(const crWidgetUpdate3Method& handle);
	MethodClass(CREncapsulation, WidgetUpdate3)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_widgetName;
};
class crCanvasUpdateMethod : public CRCore::crMethod
{
public:
	crCanvasUpdateMethod();
	crCanvasUpdateMethod(const crCanvasUpdateMethod& handle);
	MethodClass(CREncapsulation, CanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
};
class crParentCanvasUpdateMethod : public CRCore::crMethod
{
public:
	crParentCanvasUpdateMethod();
	crParentCanvasUpdateMethod(const crParentCanvasUpdateMethod& handle);
	MethodClass(CREncapsulation, ParentCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crUpdateMeMethod : public CRCore::crMethod
{
public:
	crUpdateMeMethod();
	crUpdateMeMethod(const crUpdateMeMethod& handle);
	MethodClass(CREncapsulation, UpdateMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crShowMouseCanvasMethod : public CRCore::crMethod
{
public:
	crShowMouseCanvasMethod();
	crShowMouseCanvasMethod(const crShowMouseCanvasMethod& handle);
	MethodClass(CREncapsulation, ShowMouseCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_show;
};
class crChangeMouseMethod : public CRCore::crMethod
{
public:
	crChangeMouseMethod();
	crChangeMouseMethod(const crChangeMouseMethod& handle);
	MethodClass(CREncapsulation, ChangeMouse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_mouse;
};
class crAddTabWidgetMethod : public CRCore::crMethod
{
public:
	crAddTabWidgetMethod();
	crAddTabWidgetMethod(const crAddTabWidgetMethod& handle);
	MethodClass(CREncapsulation, AddTabWidget)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_widgetName;
};
class crSetMsgTraverseMethod : public CRCore::crMethod
{
public:
	crSetMsgTraverseMethod();
	crSetMsgTraverseMethod(const crSetMsgTraverseMethod& handle);
	MethodClass(CREncapsulation, SetMsgTraverse)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	unsigned short m_msgtraverse;
};
class crWidgetDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crWidgetDoEventMethod();
	crWidgetDoEventMethod(const crWidgetDoEventMethod& handle);
	MethodClass(CREncapsulation, WidgetDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_widget;
	_crInt32 m_msg;
	_crInt64 m_param;
};
class crWidgetDoEvent2Method : public CRCore::crMethod
{//GameClient Method
public:
	crWidgetDoEvent2Method();
	crWidgetDoEvent2Method(const crWidgetDoEvent2Method& handle);
	MethodClass(CREncapsulation, WidgetDoEvent2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_widget;
	_crInt32 m_msg;
	_crInt64 m_param;
};
class crCanvasDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCanvasDoEventMethod();
	crCanvasDoEventMethod(const crCanvasDoEventMethod& handle);
	MethodClass(CREncapsulation, CanvasDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	_crInt32 m_msg;
	_crInt64 m_param;
};
class crParentCanvasDoEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crParentCanvasDoEventMethod();
	crParentCanvasDoEventMethod(const crParentCanvasDoEventMethod& handle);
	MethodClass(CREncapsulation, ParentCanvasDoEvent)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	_crInt32 m_msg;
	_crInt64 m_param;
};
class crChangeCharacterSettingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crChangeCharacterSettingMethod();
	crChangeCharacterSettingMethod(const crChangeCharacterSettingMethod& handle);
	MethodClass(CREncapsulation, ChangeCharacterSetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float                           m_characterDispDistance;
	float                           m_characterShadowDistance;
	float                           m_characterUpdateFactor;
	float                           m_characterNearDistance;
};
class crSetWidgetEnableMethod : public CRCore::crMethod
{//GameClient Method
public:
	crSetWidgetEnableMethod();
	crSetWidgetEnableMethod(const crSetWidgetEnableMethod& handle);
	MethodClass(CREncapsulation, SetWidgetEnable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	bool m_enable;
};
class crIsRenderRunningMethod : public CRCore::crMethod
{//GameClient Method
public:
	crIsRenderRunningMethod();
	crIsRenderRunningMethod(const crIsRenderRunningMethod& handle);
	MethodClass(CREncapsulation, IsRenderRunning)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//class crGetRadioGroupValueMethod : public CRCore::crMethod
//{
//public:
//	crGetRadioGroupValueMethod();
//	crGetRadioGroupValueMethod(const crGetRadioGroupValueMethod& handle);
//	MethodClass(CREncapsulation, GetRadioGroupValue)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_radioGroup;
//};
//class crBakeLightMapMethod : public CRCore::crMethod
//{
//public:
//	crBakeLightMapMethod();
//	crBakeLightMapMethod(const crBakeLightMapMethod& handle);
//	MethodClass(CREncapsulation, BakeLightMap)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//	std::string m_imageBox;
//	CRCore::crVector2i m_texSize;
//	CRCore::crVector3i m_worldSize;
//	float m_updInterval;
//};
}
#endif