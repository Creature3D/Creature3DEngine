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
#include <CRNetApp/appCharacterUpdateMethod.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRCore/crArgumentParser.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crLoader.h>
#include <CREncapsulation/crNodeCallbacks.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRCore/thread/crThread.h>
#include "rcfg/ConfigScriptOO.h"
#include <CRCore/crEffectSequenceManager.h>
#include <CRPhysics/crRadarMatterObject.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <sys/stat.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRGUI;
using namespace CRNetApp;
using namespace rbody;
using namespace CRPhysics;
using namespace CRParticle;
using namespace CREncapsulation;
/////////////////////////////////////////
//
//crRpgGameCharacterUpdateMethod
//
/////////////////////////////////////////
crRpgGameCharacterUpdateMethod::crRpgGameCharacterUpdateMethod():
m_particleEffectNode(NULL),
m_sequenceNode(NULL),
//m_soundState(NULL),
m_currentDownState(DS_None),
m_currentUpState(US_None),
m_wantChangeDownState(DS_None),
m_wantChangeUpState(US_None),
m_currentDownStateStartTime(0.0f),
m_currentUpStateStartTime(0.0f),
m_wantDownStateInterval(0.0f),
m_wantUpStateInterval(0.0f),
m_downStateInterval(0.0f),
m_upStateInterval(0.0f),
m_move(M_None),
m_dt(0.0f),
m_idleStateInterval(1.0f),
m_actDuration(0)
//m_init(false)
{
}

crRpgGameCharacterUpdateMethod::crRpgGameCharacterUpdateMethod(const crRpgGameCharacterUpdateMethod& handle):
crMethod(handle),
m_particleEffectNode(NULL),
m_sequenceNode(NULL),
//m_soundState(NULL),
m_currentDownState(DS_None),
m_currentUpState(US_None),
m_wantChangeDownState(DS_None),
m_wantChangeUpState(US_None),
m_currentDownStateStartTime(0.0f),
m_currentUpStateStartTime(0.0f),
m_wantDownStateInterval(0.0f),
m_wantUpStateInterval(0.0f),
m_downStateInterval(0.0f),
m_upStateInterval(0.0f),
m_move(M_None),
m_dt(0.0f),
m_idleStateInterval(handle.m_idleStateInterval),
m_actDuration(0)
//m_init(false),
{
}

void crRpgGameCharacterUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_bot = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float*)(LOCREPARAM(param64));
			m_bot = (CRPhysics::crViewMatterObject*)(HICREPARAM(param64));
		}
		else
		{
			m_dt = 0.0f;
			m_bot = NULL;
		}
		break;
	}
}

void crRpgGameCharacterUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_idleStateInterval = atof(str.c_str());
		break;
	}
}
//void crRpgGameCharacterUpdateMethod::init()
//{
//	for(int i = 0; i<4; i++)
//	{
//		if(!m_postDof[i].valid() && m_postDofName[i].compare("NULL")!=0)
//		{
//			m_postDof[i] = dynamic_cast<crDOFTransform *>(crFilterRenderManager::getInstance()->getFilterNode(m_postDofName[i],crSearchNodeBYNameVisitor::TRANSFORM));
//		}
//	}
//
//	m_init = true;
//}
void crRpgGameCharacterUpdateMethod::operator()(crHandle &handle)
{
	if(!m_bot.valid() || m_dt <= 0.0f) return;

    void *param;
	crData *this_data = m_this->getDataClass();
	crData *bot_data = m_bot->getDataClass();

	//this_data->getParam(WCHDATA_RTHP,param);
	//float rthp  = *(float*)param;
	////bool taskCanceled = false;
	//if(rthp <= 0)
	//{
 //       //taskCanceled = true;
	//	m_currentDownState = DS_Dead;
	//}

    CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(m_bot->getBodyChild());
	//if(!bodyNode) bodyNode = dynamic_cast<CreBodyNode *>((dynamic_cast<crGroup *>(body->getActiveBodyFirstChild()))->getChild(0));
	crRadarMatterObject *radar = m_bot->getRadar();

	//if(!m_init) init();

	if(this_data && bot_data && radar)
	{
		this_data->getParam(WCHDATA_CurActState,param);
		unsigned short specState = *(unsigned short *)param;
		if(specState > 0)
		{
			this_data->getParam(WCHDATA_ActStateMap,param);
			ActStateMap* actStateMap = (ActStateMap *)param;
			ActStateMap::iterator itr = actStateMap->find(specState);
			if(itr != actStateMap->end() && m_actState.compare(itr->second) != 0)
			{
				m_actState = itr->second;
				ActionRequest* action = bodyNode->getBody()->getActionPrototype( m_actState );
				if (action)
				{
					action->setPrioritized(false);
					this_data->getParam(WCHDATA_CurActDuration,param);
					m_actDuration = *(float *)param;
					int loopmode = action->getPropertyInt("loop");
					if(loopmode!=1 && m_actDuration>0.0f)
					{
						float duration = action->getPropertyFloat("duration")/m_actDuration;
						bodyNode->setAnimPlaySpeed(duration);
					}
					else
					{
						bodyNode->setAnimPlaySpeed(1.0f);
					}
					bodyNode->getBody()->executeAction(action, false);
					m_bot->setVelocity(crVector3(0.0f,0.0f,0.0f));
					m_bot->setAcceleration(crVector3(0.0f,0.0f,0.0f));
					m_bot->setHPR(crVector3(0.0f,0.0f,0.0f));
					m_bot->lockMatrix();
				}
			}
			return;
		}
		m_bot->unlockMatrix();

		std::string botEffectStr;
		//Height - walkableHeight - blockRadius * 2.0f > 0
		float botSpeed,botViewHeight,botWalkableHeight,botTurnToIdleTime;
		short roleSpeed;
		this_data->getParam(WCHDATA_Speed,param);
		roleSpeed = *((short*)param);
		botSpeed = roleSpeed *crGlobalHandle::gData()->gUnitScale();

		bot_data->getParam(WCHDATA_ViewHeight,param);
		botViewHeight = *((float*)param);

		bot_data->getParam(WCHDATA_WalkableHeight,param);
		botWalkableHeight = *((float*)param);

		bot_data->getParam(WCHDATA_TurnToIdleTime,param);
		botTurnToIdleTime = *((float*)param);

		bot_data->getParam(WCHDATA_BotEffectStr,param);
		botEffectStr = *((std::string*)param);

		std::string downAct,effectStr;
		float height = radar->getGndDetectHeight();
		std::string weaponActStr;
		crWeaponMatterObject *weapon = m_bot->getCurrentWeaponMatterObject();
		float weaponSpeedScale = 1.0f;
		//float postIncrement = 0.0f;
		//float postMin = 0.0f;
		//float postMax = 0.0f;
		if(weapon) 
		{
			crData *weaponData = weapon->getDataClass();
			weaponData->getParam(WCHDATA_WeaponActStr,param);
			if(param) weaponActStr = *((std::string*)param);

			weaponData->getParam(WCHDATA_WeaponSpeedScale,param);
			if(param) weaponSpeedScale = *((float*)param);
		}

		m_currentUpStateStartTime += m_dt;
		if(m_currentUpStateStartTime>=m_upStateInterval)
		{//== 0.0f为无穷
			if(m_wantChangeUpState != US_None && m_wantChangeUpState != m_currentUpState)
			{
				m_currentUpState = m_wantChangeUpState;
				m_wantChangeUpState = US_None;
				m_upStateInterval = m_wantUpStateInterval;
				m_wantUpStateInterval = 0.0f;
				m_actDuration = m_upStateInterval + 0.1f;
				m_currentUpStateStartTime = 0.0f;
			}
			else if(m_currentUpStateStartTime > 0.0f)
			{
				m_currentUpState = US_None;
				m_currentUpStateStartTime = 0.0f;
				m_upStateInterval = 0.0f;
				m_actDuration = 0.0f;
			}
		}

		m_currentDownStateStartTime += m_dt;
		if(m_currentDownStateStartTime>=m_downStateInterval)
		{
			DownState downState = DS_None;
			if(height>0.5f)
			{//下落过程中
				if(m_currentDownState!=DS_Jump)
				{
					if(m_bot->getCanFalldown())
					{
						downAct = "Falldown";
						effectStr = "Falldown";
						downState = DS_Falldown;
					}
					else
					{
						downAct = "Stand";
						downState = DS_Stand;
					}
				}
			}
			else if(m_currentDownState == DS_Falldown/*||m_currentDownState == DS_Jump*/)
			{
				downAct = "Landfall";
				effectStr = "Landfall";
				downState = DS_Landfall;
				m_downStateInterval = 0.1f;
			}
			if(downState == DS_None && m_wantChangeDownState != DS_None)
			{
				downState = m_wantChangeDownState;
				m_wantChangeDownState = DS_None;
				m_downStateInterval = m_wantDownStateInterval;
				m_wantDownStateInterval = 0.0f;
			}
			if(downState != DS_None && downState != m_currentDownState)
			{
				m_currentDownState = downState;
				m_currentDownStateStartTime = 0.0f;
			}
			else if(m_downStateInterval>0.0f)
			{//==0.0f表示无穷
				m_currentDownState = m_currentDownState == DS_Jump?DS_Falldown:DS_Stand;
				m_currentDownStateStartTime = 0.0f;
				m_downStateInterval = 0.0f;
			}
			if(m_currentDownState == DS_Stand && m_currentUpState == US_None)
			{
				if(m_currentDownStateStartTime>=botTurnToIdleTime)
				{
					m_currentDownState = DS_Idle;
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f/*m_idleStateInterval*/;
				}
			}
			else if(m_currentDownState == DS_Idle)
			{
				if(m_currentUpState != US_None)
				{
					m_currentDownState = DS_Stand;
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f;
				}
			}
		}

		crVector2f baseVel;
		switch (m_currentDownState)
		{
		case DS_Stand:
			downAct = "Stand";
			//postIncrement = -m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			break;
		case DS_Idle:
			downAct = "Idle";
			//postIncrement = -m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			break;
		case DS_Jump:
			downAct = "Jump";
			effectStr = "Jump";
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			break;
		case DS_Falldown:
			downAct = "Falldown";
			effectStr = "Falldown";
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			break;
		case DS_Landfall:
			downAct = "Landfall";
			effectStr = "Landfall";
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			break;
		case DS_Move:
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			if(m_move == MoveUp)
			{
				baseVel.set(botSpeed,0.0f);
				downAct = "Run";
				effectStr = "Run";
			}
			//else if(m_move == MoveDown)
			//{
			//	baseVel.set(-botSpeed,0.0f);
			//	downAct = "Run";
			//	effectStr = "Run";
			//}
			//else if(m_move == MoveLeft)
			//{
			//	baseVel.set(0.0f,botSpeed);
			//	downAct = "Run";
			//	effectStr = "Run";
			//}
			//else if(m_move == MoveRight)
			//{
			//	baseVel.set(0.0f,-botSpeed);
			//	downAct = "Run";
			//	effectStr = "Run";
			//}
			break;
		case DS_Dead:
			downAct = "Dead";
			effectStr = "Dead";
			break;
		case DS_Hurt:
			downAct = "Hurt";
			effectStr = "Hurt";
			break;
		}
		baseVel *= weaponSpeedScale;

		if(m_currentDownState != DS_Jump && m_currentDownState != DS_Falldown)
		{
			switch (m_move)
			{
			case StopMove:
				m_bot->setVelocity0(0.0f);
				//m_bot->setVelocity1(0.0f);
				break;
			case MoveUp:
			//case MoveDown:
				wantChangeDownState(DS_Move,0.0f);
				m_bot->setVelocity0(/*baseVel[0]*/0.0f);
				break;
			//case MoveLeft:
			//case MoveRight:
			//	wantChangeDownState(DS_Move,0.0f);
			//	m_bot->setVelocity1(baseVel[1]);
			//	break;
			}
			if(m_move <= StopMove)
			{
				if(m_currentDownState == DS_None || m_currentDownState == DS_Move)
				{
					m_currentDownState = DS_Stand;
					m_downStateInterval = 0.0f;
				}
			}
		}

		if(downAct.empty()) downAct = "Stand";
		//downAct += weaponActStr;

		std::string upAct,finalAct;

		if(m_currentUpState == US_WeaponFire)
		{
			upAct = "WeaponFire" + weaponActStr;
			//postIncrement = fabs(postIncrement) * m_fireScale;
		}
		else if(m_currentUpState == US_WeaponUnSelect)
		{
			upAct = "WeaponUnSelect" + weaponActStr;
		}
		else if(m_currentUpState == US_WeaponSelect)
		{
			upAct = "WeaponSelect" + weaponActStr;
		}

		if(m_currentDownState == DS_Dead)
		{
			finalAct = "Dead";
		}
		else
		{
			finalAct = upAct+downAct;
		}

		if(bodyNode)
		{
			finalAct = "ACT_"+finalAct;
			//if(finalAct.compare(m_actState) != 0)
			{
				m_actState = finalAct;
				ActionRequest* action = bodyNode->getBody()->getActionPrototype( m_actState );
				if (action) 
				{
					action->setPrioritized(false);
					int loopmode = action->getPropertyInt("loop");
					if(loopmode!=1 && m_actDuration>0.0f)
					{
						float duration = action->getPropertyFloat("duration")/(m_actDuration/*+0.2f*/);
						bodyNode->setAnimPlaySpeed(duration);
					}
					else
					{
						bodyNode->setAnimPlaySpeed(1.0f);
					}
					bodyNode->getBody()->executeAction(action, false);
				}
			}
		}

		m_bot->setViewHeight(botViewHeight);
		m_bot->setWalkableHeight(botWalkableHeight);

		//if(m_postDof[0].valid())
		//{
		//	crVector3f incrementTrans,trans;
		//	for(int i = 0; i<4; i++)
		//	{
		//		if(m_postDof[i].valid())
		//		{
		//			incrementTrans = m_postDof[i]->getIncrementTranslate();
		//			m_postDof[i]->setAnimationOn(true);
		//			incrementTrans[1] = postIncrement;
		//			m_postDof[i]->setIncrementTranslate(incrementTrans);

		//			trans = m_postDof[i]->getMaxTranslate();
		//			trans[1] = postMax;
		//			m_postDof[i]->setMaxTranslate(trans);

		//			trans = m_postDof[i]->getMinTranslate();
		//			trans[1] = postMin;
		//			m_postDof[i]->setMinTranslate(trans);
		//		}
		//	}

		//	body->getGameBodyInfo()->setTargetRange(m_postDof[0]->getCurrentTranslate()[1] + m_postTargetOffset);
		//}

		//GndAttr
		std::string attrStr;
		crNode *gndNode = radar->getGndDetectNode();
		if(gndNode) CRCore::crArgumentParser::readKeyValue(gndNode->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty())
			attrStr = "stone";
		if(attrStr.compare("Mirror") == 0)
		{
			crGndReflectMaterialCallback *reflect = dynamic_cast<crGndReflectMaterialCallback *>(m_bot->getCullCallback("GndReflectMaterial"));
			if(!reflect)
			{
				reflect = new crGndReflectMaterialCallback;
				m_bot->setCullCallback(reflect);
			}
			reflect->setGroundOffset(-m_bot->getViewHeight() * 0.5f);
			reflect->setEnable(true);
		}
		else
		{
			crGndReflectMaterialCallback *reflect = dynamic_cast<crGndReflectMaterialCallback *>(m_bot->getCullCallback("GndReflectMaterial"));
			if(reflect)
			{
				reflect->setEnable(false);
			}
		}
		//move effect
		if(!effectStr.empty())
		{
			std::string bodyAttrStr;
			if(botEffectStr.empty()) botEffectStr = "human";

			std::string effectString = botEffectStr+"_"+attrStr+"_"+effectStr;

			//CRCore::notify(CRCore::FATAL)<<" crCharacterMoveActionCallback::operator(): effectString = "<<effectString<<std::endl;

			crVector3f position = m_bot->getTrans();
			//position[2] -= body->getPhysicsBox()[2] * 0.5f + 0.01f;

			//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			//if(m_particleEffectNode)
			//{
			//	m_particleEffectNode->setEffectPosition(position);
			//	m_particleEffectNode->startEffect();
			//}
			m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
			if(m_sequenceNode)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"m_sequenceNode "<<effectString<< std::endl;
				m_sequenceNode->setEffectPosition(position);
				m_sequenceNode->startEffect();
			}
			////m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
			////m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
			//m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(m_bot->getName(),effectString);
			//if(m_soundState && !m_soundState->isPlaying())
			//{
			//	m_soundState->setPosition(position);
			//	//CRCore::crVector3f listener;
			//	//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
			//	//CRCore::crVector3f dir = position - listener;
			//	//dir.normalize();
			//	//CRCore::notify(CRCore::ALWAYS)<<"m_sequenceNode "<<position<< std::endl;
			//	//m_soundState->setDirection(body->getViewDirection());
			//	if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
			//		m_soundState->setPlay(true);
			//}
		}
	}
	//handle.outputParam(0,&taskCanceled);
}
bool crRpgGameCharacterUpdateMethod::wantChangeDownState(DownState state, float interval)
{
	switch (state)
	{
	case DS_Jump:
		if(m_currentDownState == DS_Falldown || m_currentDownState == DS_Jump)
			return false;
		break;
	}
	m_wantDownStateInterval = interval;
	m_wantChangeDownState = state;
	return true;
}
bool crRpgGameCharacterUpdateMethod::wantChangeUpState(UpState state, float interval)
{
	switch (state)
	{
	case US_WeaponFire:
		if(m_currentUpState != US_None)
			return false;
		break;
	case US_WeaponUnSelect:
		if(m_currentUpState != US_None)
			return false;
		break;
	case US_WeaponSelect:
		if(m_currentUpState != US_None)
			return false;
		break;
	}
	m_wantUpStateInterval = interval;
	m_wantChangeUpState = state;
	return true;
}
bool crRpgGameCharacterUpdateMethod::wantChangeMove(Move move)
{
	m_move = move;
	return true;
}
unsigned int crRpgGameCharacterUpdateMethod::getCurrentDownState()
{
	return m_currentDownState;
}

unsigned int crRpgGameCharacterUpdateMethod::getCurrentUpState()
{
	return m_currentUpState;
}
/////////////////////////////////////////
//
//crRpgGameCharacterStateChangeMethod
//
/////////////////////////////////////////
crRpgGameCharacterStateChangeMethod::crRpgGameCharacterStateChangeMethod():
m_downState(crRpgGameCharacterUpdateMethod::DS_None),
m_upState(crRpgGameCharacterUpdateMethod::US_None),
m_downInterval(0.0f),
m_upInterval(0.0f),
m_lastUpdTime(0.0f),
m_updInterval(0.5f),
m_actDuration(0.0f)
{
}

crRpgGameCharacterStateChangeMethod::crRpgGameCharacterStateChangeMethod(const crRpgGameCharacterStateChangeMethod& handle):
crMethod(handle),
m_downState(handle.m_downState),
m_upState(handle.m_upState),
m_downInterval(handle.m_downInterval),
m_upInterval(handle.m_upInterval),
m_lastUpdTime(handle.m_lastUpdTime),
m_updInterval(handle.m_updInterval),
m_actDuration(handle.m_actDuration)
{
}

void crRpgGameCharacterStateChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_actDuration = *(float *)(LOCREPARAM(param64));
		}
		else
		{
			m_actDuration = 0.0f;
		}
		break;
	}
}

void crRpgGameCharacterStateChangeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_updInterval = atof(str.c_str());
		break;
	case 1:
		if(str.compare("None")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_None;
		else if(str.compare("Stand")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Stand;
		else if(str.compare("Idle")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Idle;
		else if(str.compare("Jump")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Jump;
		else if(str.compare("Falldown")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Falldown;
		else if(str.compare("Move")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Move;
		else if(str.compare("Dead")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Dead;
		else if(str.compare("Hurt")==0)
			m_downState = crRpgGameCharacterUpdateMethod::DS_Hurt;
		break;
	case 2:
		m_downInterval = atof(str.c_str());
		break;
	case 3:
		if(str.compare("None")==0)
			m_upState = crRpgGameCharacterUpdateMethod::US_None;
		else if(str.compare("WeaponFire")==0)
			m_upState = crRpgGameCharacterUpdateMethod::US_WeaponFire;
		else if(str.compare("WeaponUnSelect")==0)
			m_upState = crRpgGameCharacterUpdateMethod::US_WeaponUnSelect;
		else if(str.compare("WeaponSelect")==0)
			m_upState = crRpgGameCharacterUpdateMethod::US_WeaponSelect;
	case 4:
		m_upInterval = atof(str.c_str());
		break;
	}
}

void crRpgGameCharacterStateChangeMethod::operator()(crHandle &handle)
{
	bool cancelTask = true;
	if(!m_characterUpdateMethod.valid())
	{
		m_characterUpdateMethod = dynamic_cast<crRpgGameCharacterUpdateMethod *>(m_this->getHandle(WCH_UPDATEVISITOR,"RpgGameCharacterUpdate"));
	}
	if(m_characterUpdateMethod.valid())
	{
		if(m_downState != crRpgGameCharacterUpdateMethod::DS_None)
		{
			if(m_characterUpdateMethod->wantChangeDownState(m_downState,m_actDuration>0.0f?m_actDuration:m_downInterval))
			{
				cancelTask = false;
			}
		}
		if(m_upState != crRpgGameCharacterUpdateMethod::US_None)
		{
			if(m_characterUpdateMethod->wantChangeUpState(m_upState,m_actDuration>0.0f?m_actDuration:m_upInterval))
			{
				cancelTask = false;
			}
		}
	}
	if(cancelTask)
	{
		handle.outputParam(0,&cancelTask);
	}
}
/////////////////////////////////////////
//
//crRpgGameCharacterMoveChangeMethod
//
/////////////////////////////////////////
crRpgGameCharacterMoveChangeMethod::crRpgGameCharacterMoveChangeMethod():
m_move(crRpgGameCharacterUpdateMethod::M_None)
{
}

crRpgGameCharacterMoveChangeMethod::crRpgGameCharacterMoveChangeMethod(const crRpgGameCharacterMoveChangeMethod& handle):
crMethod(handle),
m_move(handle.m_move)
{
}

void crRpgGameCharacterMoveChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	}
}

void crRpgGameCharacterMoveChangeMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		if(str.compare("None")==0)
			m_move = crRpgGameCharacterUpdateMethod::M_None;
		else if(str.compare("StopMove")==0)
			m_move = crRpgGameCharacterUpdateMethod::StopMove;
		else if(str.compare("MoveUp")==0)
			m_move = crRpgGameCharacterUpdateMethod::MoveUp;
		//else if(str.compare("MoveDown")==0)
		//	m_move = crRpgGameCharacterUpdateMethod::MoveDown;
		//else if(str.compare("MoveLeft")==0)
		//	m_move = crRpgGameCharacterUpdateMethod::MoveLeft;
		//else if(str.compare("MoveRight")==0)
		//	m_move = crRpgGameCharacterUpdateMethod::MoveRight;
	}
}

void crRpgGameCharacterMoveChangeMethod::operator()(crHandle &handle)
{
	if(!m_characterUpdateMethod.valid())
	{
		m_characterUpdateMethod = dynamic_cast<crRpgGameCharacterUpdateMethod *>(m_this->getHandle(WCH_UPDATEVISITOR,"RpgGameCharacterUpdate"));
	}
	if(m_characterUpdateMethod.valid())
	{
		if(m_move != crRpgGameCharacterUpdateMethod::M_None) 
		{
			m_characterUpdateMethod->wantChangeMove(m_move);
		}
	}
}