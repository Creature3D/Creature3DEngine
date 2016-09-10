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
#include <CREncapsulation/crMethodClass2.h>
#include <CREncapsulation/crDataClass.h>
#include <CREncapsulation/crLoader.h>
#include <CREncapsulation/crStartHandler.h>
#include <CREncapsulation/crNodeCallbacks.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crBrain.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/crDOFTransform.h>
#include <CRCore/crEffectSequenceManager.h>
#include <CRCore/crLightSourceManager.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crCharacterMatterObject.h>
#include <CRPhysics/crMatterObject.h>

#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/cr2DStageManager.h>
#include <rvrutils/Property.h>
#include "rcfg/ConfigScriptOO.h"
#include <rcfg/ItemMap.h>

#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crReadFile.h>
#include <sys/stat.h>
#include <stdio.h>

#include <CRTerrain/crTerrainTile.h>
#include <CRTerrain/crGeometryTechnique.h>
#include <CRTerrain/crLayer.h>
#include <CRCore/crImageUtils.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRUtil/crOptimizer.h>
#include <CRUtil/crPrepareVisitor.h>
#include <CRProducer/crSceneHandler.h>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRGUI;
using namespace CRPhysics;
using namespace rbody;
using namespace CRParticle;
using namespace CRProducer;
using namespace CRTerrain;
using namespace CRIOManager;
using namespace CRUtil;
/////////////////////////////////////////
//
//crPitchDofMethod
//
/////////////////////////////////////////
crPitchDofMethod::crPitchDofMethod():
m_min(0.0f),
m_max(0.0f){}
crPitchDofMethod::crPitchDofMethod(const crPitchDofMethod& handle):
crMethod(handle),
m_dofName(handle.m_dofName),
m_min(handle.m_min),
m_max(handle.m_max),
m_stepRange(handle.m_stepRange)
{
}

void crPitchDofMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crPitchDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dofName = str;
		break;
	case 1:
		m_min = CRCore::inDegrees(atof(str.c_str()));
		break;
	case 2:
		m_max = CRCore::inDegrees(atof(str.c_str()));
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_stepRange);
		m_stepRange[0] = CRCore::inDegrees(m_stepRange[0]);
		m_stepRange[1] = CRCore::inDegrees(m_stepRange[1]);
		break;
	}
}

void crPitchDofMethod::operator()(crHandle &handle)
{
	if(!m_this) return;

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_dofName);
		searchByNameVisitor.setSearchNodeType(TRANSFORM);
		m_this->accept(searchByNameVisitor);
		m_dof = dynamic_cast<crDOFTransform *>(searchByNameVisitor.getResult());
		if(!m_dof.valid()) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crPitchDofCallback::operator() not find Dof "<<m_dofName.c_str()<<std::endl;
			return;
		}
	}

	float step = lerp(m_stepRange[0],m_stepRange[1],m_this->getSpeedRate0());

	crVector3f currentHPR = m_dof->getCurrentHPR();
	crVector3f incrementHPR = m_dof->getIncrementHPR();

	crVector3f hpr = m_dof->getMaxHPR();
	hpr[1] = m_max;
	m_dof->setMaxHPR(hpr);
	hpr = m_dof->getMinHPR();
	hpr[1] = m_min;
	m_dof->setMinHPR(hpr);

	if(step>0)
	{
		if(currentHPR[1] < m_max)
		{
			m_dof->setAnimationOn(true);
			incrementHPR[1] = step;
			m_dof->setIncrementHPR(incrementHPR);
		}
	}
	else
	{
		if(currentHPR[1] > m_min)
		{
			m_dof->setAnimationOn(true);
			incrementHPR[1] = step;
			m_dof->setIncrementHPR(incrementHPR);
		}
	}
}
/////////////////////////////////////////
//
//crRollDofMethod
//
/////////////////////////////////////////
crRollDofMethod::crRollDofMethod():
m_min(0.0f),
m_max(0.0f){}
crRollDofMethod::crRollDofMethod(const crRollDofMethod& handle):
crMethod(handle),
m_dofName(handle.m_dofName),
m_min(handle.m_min),
m_max(handle.m_max),
m_stepRange(handle.m_stepRange)
{
}

void crRollDofMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crRollDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dofName = str;
		break;
	case 1:
		m_min = CRCore::inDegrees(atof(str.c_str()));
		break;
	case 2:
		m_max = CRCore::inDegrees(atof(str.c_str()));
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_stepRange);
		m_stepRange[0] = CRCore::inDegrees(m_stepRange[0]);
		m_stepRange[1] = CRCore::inDegrees(m_stepRange[1]);
		break;
	}
}

void crRollDofMethod::operator()(crHandle &handle)
{
	if(!m_this) return;

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_dofName);
		searchByNameVisitor.setSearchNodeType(TRANSFORM);
		m_this->accept(searchByNameVisitor);
		m_dof = dynamic_cast<crDOFTransform *>(searchByNameVisitor.getResult());
		if(!m_dof.valid()) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crPitchDofCallback::operator() not find Dof "<<m_dofName.c_str()<<std::endl;
			return;
		}
	}

	float step = lerp(m_stepRange[0],m_stepRange[1],m_this->getSpeedRate0());

	crVector3f currentHPR = m_dof->getCurrentHPR();
	crVector3f incrementHPR = m_dof->getIncrementHPR();

	crVector3f hpr = m_dof->getMaxHPR();
	hpr[1] = m_max;
	m_dof->setMaxHPR(hpr);
	hpr = m_dof->getMinHPR();
	hpr[1] = m_min;
	m_dof->setMinHPR(hpr);

	if(step>0)
	{
		if(currentHPR[1] < m_max)
		{
			m_dof->setAnimationOn(true);
			incrementHPR[1] = step;
			m_dof->setIncrementHPR(incrementHPR);
		}
	}
	else
	{
		if(currentHPR[1] > m_min)
		{
			m_dof->setAnimationOn(true);
			incrementHPR[1] = step;
			m_dof->setIncrementHPR(incrementHPR);
		}
	}
}
/////////////////////////////////////////
//
//crZTransDofMethod
//
/////////////////////////////////////////
crZTransDofMethod::crZTransDofMethod():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{
}

crZTransDofMethod::crZTransDofMethod(const crZTransDofMethod& handle):
crMethod(handle),
m_dofName(handle.m_dofName),
m_min(handle.m_min),
m_max(handle.m_max),
m_step(handle.m_step)
{
}

void crZTransDofMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crZTransDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dofName = str;
		break;
	case 1:
		m_min = atof(str.c_str());
		break;
	case 2:
		m_max = atof(str.c_str());
		break;
	case 3:
		m_step = atof(str.c_str());
		break;
	}
}

void crZTransDofMethod::operator()(crHandle &handle)
{
	if(!m_this) return;

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_dofName);
		searchByNameVisitor.setSearchNodeType(TRANSFORM);
		m_this->accept(searchByNameVisitor);
		m_dof = dynamic_cast<crDOFTransform *>(searchByNameVisitor.getResult());
		if(!m_dof.valid()) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crPitchDofCallback::operator() not find Dof "<<m_dofName.c_str()<<std::endl;
			return;
		}
	}

	crVector3f currentTrans = m_dof->getCurrentTranslate();
	crVector3f incrementTrans = m_dof->getIncrementTranslate();

	crVector3f trans = m_dof->getMaxTranslate();
	trans[2] = m_max;
	m_dof->setMaxTranslate(trans);
	trans = m_dof->getMinTranslate();
	trans[2] = m_min;
	m_dof->setMinTranslate(trans);

	if(m_step>0)
	{
		if(currentTrans[2] < m_max)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[2] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
	else
	{
		if(currentTrans[2] > m_min)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[2] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
}
/////////////////////////////////////////
//
//crYTransDofMethod
//
/////////////////////////////////////////
crYTransDofMethod::crYTransDofMethod():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{
}

crYTransDofMethod::crYTransDofMethod(const crYTransDofMethod& handle):
crMethod(handle),
m_dofName(handle.m_dofName),
m_min(handle.m_min),
m_max(handle.m_max),
m_step(handle.m_step)
{
}

void crYTransDofMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crYTransDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dofName = str;
		break;
	case 1:
		m_min = atof(str.c_str());
		break;
	case 2:
		m_max = atof(str.c_str());
		break;
	case 3:
		m_step = atof(str.c_str());
		break;
	}
}

void crYTransDofMethod::operator()(crHandle &handle)
{
	if(!m_this) return;

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_dofName);
		searchByNameVisitor.setSearchNodeType(TRANSFORM);
		m_this->accept(searchByNameVisitor);
		m_dof = dynamic_cast<crDOFTransform *>(searchByNameVisitor.getResult());
		if(!m_dof.valid()) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crPitchDofCallback::operator() not find Dof "<<m_dofName.c_str()<<std::endl;
			return;
		}
		//m_dof = dynamic_cast<crDOFTransform *>(crFilterRenderManager::getInstance()->getFilterNode(m_dofName,TRANSFORM));
	}
	if(m_dof.valid()) 
	{
		crVector3f currentTrans = m_dof->getCurrentTranslate();
		crVector3f incrementTrans = m_dof->getIncrementTranslate();

		crVector3f trans = m_dof->getMaxTranslate();
		trans[1] = m_max;
		m_dof->setMaxTranslate(trans);
		trans = m_dof->getMinTranslate();
		trans[1] = m_min;
		m_dof->setMinTranslate(trans);

		if(m_step>0)
		{
			if(currentTrans[1] < m_max)
			{
				m_dof->setAnimationOn(true);
				incrementTrans[1] = m_step;
				m_dof->setIncrementTranslate(incrementTrans);
			}
		}
		else
		{
			if(currentTrans[1] > m_min)
			{
				m_dof->setAnimationOn(true);
				incrementTrans[1] = m_step;
				m_dof->setIncrementTranslate(incrementTrans);
			}
		}
	}
}
/////////////////////////////////////////
//
//crStopDofMethod
//
/////////////////////////////////////////
crStopDofMethod::crStopDofMethod()
{
}

crStopDofMethod::crStopDofMethod(const crStopDofMethod& handle):
crMethod(handle),
m_dofName(handle.m_dofName)
{
}

void crStopDofMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crStopDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dofName = str;
		break;
	}
}

void crStopDofMethod::operator()(crHandle &handle)
{
	if(!m_this) return;

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_dofName);
		searchByNameVisitor.setSearchNodeType(TRANSFORM);
		m_this->accept(searchByNameVisitor);
		m_dof = dynamic_cast<crDOFTransform *>(searchByNameVisitor.getResult());
		if(!m_dof.valid()) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crPitchDofCallback::operator() not find Dof "<<m_dofName.c_str()<<std::endl;
			return;
		}
	}

	m_dof->setAnimationOn(false);
}
/////////////////////////////////////////
//
//crAircraftSequenceMethod
//
/////////////////////////////////////////
crAircraftSequenceMethod::crAircraftSequenceMethod():
m_min(0),
m_max(-1),
m_vanishWhenStop(false),
m_forwardAnim(true)
{
}

crAircraftSequenceMethod::crAircraftSequenceMethod(const crAircraftSequenceMethod& handle):
crMethod(handle),
m_sequenceName(handle.m_sequenceName),
m_min(handle.m_min),
m_max(handle.m_max),
m_vanishWhenStop(handle.m_vanishWhenStop),
m_forwardAnim(handle.m_forwardAnim)
{
}

void crAircraftSequenceMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crAircraftSequenceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sequenceName = str;
		break;
	case 1:
		m_min = atoi(str.c_str());
		break;
	case 2:
		m_max = atoi(str.c_str());
		break;
	case 3:
		m_vanishWhenStop = atoi(str.c_str());
		break;
	case 4:
		m_forwardAnim = atoi(str.c_str());
		break;
	}
}

void crAircraftSequenceMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	if(!m_sequence.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_sequenceName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		m_node->accept(searchByNameVisitor);
		m_sequence = dynamic_cast<crSequence *>(searchByNameVisitor.getResult());
		if(!m_sequence) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crAircraftSequenceCallback::operator() not find Sequence "<<m_sequenceName.c_str()<<std::endl;
			return;
		}
	}

	unsigned int nch = m_sequence->getNumChildren();
	m_min = (m_min < 0 ? nch + m_min : m_min);
	m_max = (m_max < 0 ? nch + m_max : m_max);

	m_sequence->setVanishWhenStop(m_vanishWhenStop);
	int value = m_sequence->getValue();
	if(m_forwardAnim)
	{//向前播放直到m_max，停留在m_max
		m_sequence->setInterval( crSequence::LOOP, m_min, m_max );
		if(value>=m_max)
		{
			m_sequence->setMode(crSequence::STOP);
			m_sequence->setValue(m_max);
		}
		else
		{
			m_sequence->setMode(crSequence::START);
			m_sequence->setValue(value);
		}	
	}
	else
	{//回退，直到m_min,停留在m_min
		m_sequence->setInterval( crSequence::LOOP, m_max, m_min );
		if(value<=m_min)
		{
			m_sequence->setMode(crSequence::STOP);
			m_sequence->setValue(m_min);
		}
		else
		{
			m_sequence->setMode(crSequence::START);
			m_sequence->setValue(value);
		}
	}
}
/////////////////////////////////////////
//
//crAircraftWeaponSequenceMethod
//
/////////////////////////////////////////
crAircraftWeaponSequenceMethod::crAircraftWeaponSequenceMethod()
{
}

crAircraftWeaponSequenceMethod::crAircraftWeaponSequenceMethod(const crAircraftWeaponSequenceMethod& handle):
crMethod(handle),
m_sequenceName(handle.m_sequenceName),
m_weapons(handle.m_weapons)
{
}

void crAircraftWeaponSequenceMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crAircraftWeaponSequenceMethod::addParam(int i, const std::string& str)
{
	if(i==0) m_sequenceName = str;
	else m_weapons.insert(str);
}

void crAircraftWeaponSequenceMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	if(!m_sequence.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_sequenceName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		m_node->accept(searchByNameVisitor);
		m_sequence = dynamic_cast<crSequence *>(searchByNameVisitor.getResult());
		if(!m_sequence) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crAircraftSequenceCallback::operator() not find Sequence "<<m_sequenceName.c_str()<<std::endl;
			return;
		}
	}
	std::string *str = (std::string *)(LOINT64(m_param));
	if(str && m_weapons.find(*str) != m_weapons.end())
	{
		m_sequence->startEffect();
	}
}
/////////////////////////////////////////
//
//crAircraftWeaponSequenceExMethod
//
/////////////////////////////////////////
crAircraftWeaponSequenceExMethod::crAircraftWeaponSequenceExMethod():
m_min(0),
m_max(-1),
m_vanishWhenStop(false),
m_forwardAnim(true)
{
}

crAircraftWeaponSequenceExMethod::crAircraftWeaponSequenceExMethod(const crAircraftWeaponSequenceExMethod& handle):
crMethod(handle),
m_sequenceName(handle.m_sequenceName),
m_min(handle.m_min),
m_max(handle.m_max),
m_vanishWhenStop(handle.m_vanishWhenStop),
m_forwardAnim(handle.m_forwardAnim),
m_weapons(handle.m_weapons)
{
}

void crAircraftWeaponSequenceExMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crAircraftWeaponSequenceExMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sequenceName = str;
		break;
	case 1:
		m_min = atoi(str.c_str());
		break;
	case 2:
		m_max = atoi(str.c_str());
		break;
	case 3:
		m_vanishWhenStop = atoi(str.c_str());
		break;
	case 4:
		m_forwardAnim = atoi(str.c_str());
		break;
	default:
		m_weapons.insert(str);
	}
}

void crAircraftWeaponSequenceExMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	if(!m_sequence.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_sequenceName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		m_node->accept(searchByNameVisitor);
		m_sequence = dynamic_cast<crSequence *>(searchByNameVisitor.getResult());
		if(!m_sequence) 
		{
			//CRCore::notify(CRCore::ALWAYS)<<"crAircraftWeaponSequenceExCallback::operator() not find Sequence "<<m_sequenceName.c_str()<<std::endl;
			return;
		}
	}

	std::string *str = (std::string *)(LOINT64(m_param));
	if(str && m_weapons.find(*str) != m_weapons.end())
	{
		unsigned int nch = m_sequence->getNumChildren();
		m_min = (m_min < 0 ? nch + m_min : m_min);
		m_max = (m_max < 0 ? nch + m_max : m_max);

		m_sequence->setVanishWhenStop(m_vanishWhenStop);
		int value = m_sequence->getValue();
		if(m_forwardAnim)
		{//向前播放直到m_max，停留在m_max
			m_sequence->setInterval( crSequence::LOOP, m_min, m_max );
			if(value>=m_max)
			{
				m_sequence->setMode(crSequence::STOP);
				m_sequence->setValue(m_max);
			}
			else
			{
				m_sequence->setMode(crSequence::START);
				m_sequence->setValue(value);
			}	
		}
		else
		{//回退，直到m_min,停留在m_min
			m_sequence->setInterval( crSequence::LOOP, m_max, m_min );
			if(value<=m_min)
			{
				m_sequence->setMode(crSequence::STOP);
				m_sequence->setValue(m_min);
			}
			else
			{
				m_sequence->setMode(crSequence::START);
				m_sequence->setValue(value);
			}
		}
	}
}
///////////////////////////////////////////
////
////crDeadMethod
////
///////////////////////////////////////////
//crDeadMethod::crDeadMethod()
//{
//}
//
//crDeadMethod::crDeadMethod(const crDeadMethod& handle):
//crMethod(handle)
//{
//}
//
//void crDeadMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	}
//}
//
//void crDeadMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crDeadMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//
//	//crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(&node);
//	crBotManager *botManager = crBotManager::getInstance();
//	//CRCore::notify(CRCore::FATAL)<<"CRPhysics::crDeadCallback(): "<<node.getName()<<" "<<botManager->getNumEnemyBot()<<std::endl;
//
//	if(botManager->getNumEnemyBot()<=0)
//	{
//		crViewMatterObject *cameraBot = botManager->getFirstPlayer();
//		CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(cameraBot->getParent(0));
//		if(!bodyMG)
//		{
//			CRCore::notify(CRCore::FATAL)<<"crDeadCallback::operator(): cameraBot的父节点类型不正确"<<std::endl;
//			return;
//		}
//		crEventGroup *bodyNC = bodyMG->getAttachedEventGroup();
//		if(!bodyNC)
//		{
//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
//			return;
//		}
//		//CRCore::notify(CRCore::FATAL)<<"CRPhysics::crDeadCallback():WCH_NEXTPROGRESS "<<std::endl;
//		bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_BOTCONTROLLER,WCH_NEXTPROGRESS),MAKEINT64(NULL,NULL));
//	}
//}
///////////////////////////////////////////
////
////crPlayMoviesMethod
////
///////////////////////////////////////////
//crPlayMoviesMethod::crPlayMoviesMethod():
//m_type(1)
//{
//}
//
//crPlayMoviesMethod::crPlayMoviesMethod(const crPlayMoviesMethod& handle):
//crMethod(handle),
//m_movies(handle.m_movies),
//m_type(handle.m_type)
//{
//}
//
//void crPlayMoviesMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	case 2:
//		if(param) m_param = *(_crInt64*)param;
//		else m_param = 0;
//		break;
//	}
//}
//
//void crPlayMoviesMethod::addParam(int i, const std::string& str)
//{
//	if(i == 0) m_type = atoi(str.c_str());
//	else m_movies.push_back(atoi(str.c_str()));
//}
//
//void crPlayMoviesMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//
//	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
//    if(!body) return;
//
//	CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(body->getParent(0));
//	if(!bodyMG)
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::crPlayMoviesCallback(): cameraBot的父节点类型不正确"<<std::endl;
//		return;
//	}
//	crEventGroup *bodyNC = bodyMG->getAttachedEventGroup();
//	if(!bodyNC)
//	{
//		CRCore::notify(CRCore::FATAL)<<"CRPhysics::crPlayMoviesCallback(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
//		return;
//	}
//
//	_crInt32 id = LOINT64(m_param);
//	if(id>=0 && id<m_movies.size())
//	{
//		//CRCore::notify(CRCore::FATAL)<<"CRPhysics::crPlayMoviesCallback(): "<<m_type<<std::endl;
//		bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVBEGIN,m_movies[id]),MAKEINT64(m_type,NULL));//0:init movetask index
//	}
//}
///////////////////////////////////////////
////
////crCameraBotMoviesMethod
////
///////////////////////////////////////////
//crCameraBotMoviesMethod::crCameraBotMoviesMethod()
//{
//}
//
//crCameraBotMoviesMethod::crCameraBotMoviesMethod(const crCameraBotMoviesMethod& handle):
//crMethod(handle),
//m_movies(handle.m_movies)
//{
//}
//
//void crCameraBotMoviesMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	case 2:
//		if(param) m_param = *(_crInt64*)param;
//		else m_param = 0;
//		break;
//	}
//}
//
//void crCameraBotMoviesMethod::addParam(int i, const std::string& str)
//{
//	m_movies.push_back(atoi(str.c_str()));
//}
//
//void crCameraBotMoviesMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//
//	crBotManager *botManager = crBotManager::getInstance();
//	crViewMatterObject *cameraBot = botManager->getFirstPlayer();
//	CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(cameraBot->getParent(0));
//	if(!bodyMG)
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): cameraBot的父节点类型不正确"<<std::endl;
//		return;
//	}
//	crEventGroup *bodyNC = bodyMG->getAttachedEventGroup();
//	if(!bodyNC)
//	{
//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadSave(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
//		return;
//	}
//	_crInt32 id = LOINT64(m_param);
//	if(id>=0 && id<m_movies.size())
//		bodyNC->acceptEventMessage(WCH_AIMOVIES,MAKEINT64(WCH_AIMOVBEGIN,m_movies[id]),MAKEINT64(2,NULL));//0:init movetask index
//}
/////////////////////////////////////////
//
//crPlaySequenceMethod
//
/////////////////////////////////////////
crPlaySequenceMethod::crPlaySequenceMethod()
{
}

crPlaySequenceMethod::crPlaySequenceMethod(const crPlaySequenceMethod& handle):
crMethod(handle),
m_effectName(handle.m_effectName)
{
}

void crPlaySequenceMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crPlaySequenceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_effectName = str;
		break;
	}
}

void crPlaySequenceMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	if(!m_sequenceNode.valid())
		m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
	if(m_sequenceNode.valid())
	{
		m_sequenceNode->setEffectPosition(m_node->getBound().center());
		m_sequenceNode->startEffect();
	}
}
///////////////////////////////////////////
////
////crPlaySoundMethod
////
///////////////////////////////////////////
//crPlaySoundMethod::crPlaySoundMethod()
//{
//}
//
//crPlaySoundMethod::crPlaySoundMethod(const crPlaySoundMethod& handle):
//crMethod(handle),
//m_effectName(handle.m_effectName)
//{
//}
//
//void crPlaySoundMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_node = NULL;
//		}
//		break;
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	}
//}
//
//void crPlaySoundMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_effectName = str;
//		break;
//	}
//}
//
//void crPlaySoundMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//
//	if(!m_soundState.valid())
//		m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(m_effectName);
//	if(m_soundState.valid())
//	{
//		crVector3f pos = m_node->getBound().center();
//		m_soundState->setPosition(pos);
//		//CRCore::crVector3f listener;
//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
//		//CRCore::crVector3f dir = pos - listener;
//		//dir.normalize();
//		//m_soundState->setDirection(dir);
//		if(!CRAL::crSoundManager::instance()->isCulled(m_soundState.get()))
//			m_soundState->setPlay(true);
//	}
//}
/////////////////////////////////////////
//
//crCharacterActionMethod
//
/////////////////////////////////////////
crCharacterActionMethod::crCharacterActionMethod():
m_prioritized(false),
m_force(true)
{
}

crCharacterActionMethod::crCharacterActionMethod(const crCharacterActionMethod& handle):
crMethod(handle),
m_actionName(handle.m_actionName),
m_prioritized(handle.m_prioritized),
m_force(handle.m_force)
{
}

void crCharacterActionMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crCharacterActionMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_actionName = str;
		break;
	case 1:
		m_prioritized = atoi(str.c_str());
		break;
	case 2:
		m_force = atoi(str.c_str());
		break;
	}
}

void crCharacterActionMethod::operator()(crHandle &handle)
{
	CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(m_this->getBodyChild());
	if(bodyNode)
	{
		ActionRequest* action = bodyNode->getBody()->getActionPrototype( m_actionName );
		if (action) 
		{
			action->setPrioritized(m_prioritized);
			bodyNode->getBody()->executeAction(action, m_force);
		}
	}
}
///////////////////////////////////////////
////
////crGndActionEffectMethod
////
///////////////////////////////////////////
//crGndActionEffectMethod::crGndActionEffectMethod()
//{
//}
//
//crGndActionEffectMethod::crGndActionEffectMethod(const crGndActionEffectMethod& handle):
//crMethod(handle),
//m_actionAttrStr(handle.m_actionAttrStr),
//m_bodyAttrStr(handle.m_bodyAttrStr)
//{
//}
//
//void crGndActionEffectMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crViewMatterObject*)param;
//		break;
//	}
//}
//
//void crGndActionEffectMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_actionAttrStr = str;
//		break;
//	case 1:
//		m_bodyAttrStr = str;
//		break;
//	}
//}
//
//void crGndActionEffectMethod::operator()(crHandle &handle)
//{
//	if(!m_this) return;
//
//	bool doaction = false;
//	crNode *gndNode = NULL;
//	crVector3f position;
//	crVisionSenseInfor *vsi = m_this->getVisionSenseInfor();
//	if(vsi)
//	{
//		if(vsi->isInAir())
//		{
//			doaction = m_actionMask&AIR;
//		}
//		else
//		{
//			doaction = m_actionMask&GND;
//		}
//		if(doaction)
//		{
//			gndNode = vsi->getGndContactNode();
//			position = vsi->getGndContactPosition();
//		}
//	}
//	if(doaction)
//	{
//		std::string attrStr;
//		if(gndNode) CRCore::crArgumentParser::readKeyValue(gndNode->getDescriptions(),"AttributeString",attrStr);
//		if(attrStr.empty()) attrStr = "stone";
//
//		std::string effectString = m_bodyAttrStr+"_"+attrStr+"_"+m_actionAttrStr;
//
//		//m_particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		//if(m_particleEffectNode)
//		//{
//		//	m_particleEffectNode->setEffectPosition(position);
//		//	m_particleEffectNode->startEffect();
//		//}
//		m_sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
//		if(m_sequenceNode)
//		{
//			//CRCore::notify(CRCore::ALWAYS)<<"m_sequenceNode "<<effectString<< std::endl;
//			m_sequenceNode->setEffectPosition(position);
//			m_sequenceNode->startEffect();
//		}
//		//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
//		//m_soundState = CRAL::crSoundManager::instance()->findSoundState(effectString);
//		m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(m_this->getName(),effectString);
//		if(m_soundState)
//		{
//			m_soundState->setPosition(position);
//			//CRCore::crVector3f listener;
//			//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
//			//CRCore::crVector3f dir = position - listener;
//			//dir.normalize();
//			//m_soundState->setDirection(dir);
//			if(!CRAL::crSoundManager::instance()->isCulled(m_soundState))
//				m_soundState->setPlay(true);
//		}
//	}
//}
/////////////////////////////////////////
//
//crStopActionMethod
//
/////////////////////////////////////////
crStopActionMethod::crStopActionMethod()
{
}

crStopActionMethod::crStopActionMethod(const crStopActionMethod& handle):
crMethod(handle),
m_actionName(handle.m_actionName)
{
}

void crStopActionMethod::inputParam(int i, void *param)
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
		m_this = (crViewMatterObject*)param;
		break;
	}
}

void crStopActionMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_actionName = str;
		break;
	}
}

void crStopActionMethod::operator()(crHandle &handle)
{
	CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(m_this->getBodyChild());
	if(bodyNode)
	{
		bodyNode->getBody()->stopAction(m_actionName);
	}
}
/////////////////////////////////////////
//
//crFpsCharacterUpdateMethod
//
/////////////////////////////////////////
crFpsCharacterUpdateMethod::crFpsCharacterUpdateMethod():
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
m_isInWeaponAim(false),
m_aimSpeedScale(0.0f),
m_move(M_None),
m_pan(P_None),
m_init(false),
m_standHeight(1.6f),
m_walkableHeight(0.5f),
m_crouchHeight(1.2f),
m_crouchableHeight(0.4f),
m_crawlHeight(0.81f),
m_crawlableHeight(0.1f)
{
}

crFpsCharacterUpdateMethod::crFpsCharacterUpdateMethod(const crFpsCharacterUpdateMethod& handle):
crMethod(handle),
m_sprintSpeed(handle.m_sprintSpeed),
m_runSpeed(handle.m_runSpeed),
m_walkSpeed(handle.m_walkSpeed),
m_crouchSpeed(handle.m_crouchSpeed),
m_crawlSpeed(handle.m_crawlSpeed),
m_sprintPanSpeed(handle.m_sprintPanSpeed),
m_runPanSpeed(handle.m_runPanSpeed),
m_walkPanSpeed(handle.m_walkPanSpeed),
m_crouchPanSpeed(handle.m_crouchPanSpeed),
m_crawlPanSpeed(handle.m_crawlPanSpeed),
m_sprintContinuableTime(handle.m_sprintContinuableTime),
m_turnToIdleTime(handle.m_turnToIdleTime),
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
m_isInWeaponAim(false),
m_aimSpeedScale(handle.m_aimSpeedScale),
m_move(M_None),
m_pan(P_None),
m_init(false),
m_standHeight(handle.m_standHeight),
m_walkableHeight(handle.m_walkableHeight),
m_crouchHeight(handle.m_crouchHeight),
m_crouchableHeight(handle.m_crouchableHeight),
m_crawlHeight(handle.m_crawlHeight),
m_crawlableHeight(handle.m_crawlableHeight)
{
}

void crFpsCharacterUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crFpsCharacterUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sprintSpeed = atof(str.c_str());
		break;
	case 1:
		m_runSpeed = atof(str.c_str());
		break;
	case 2:
		m_walkSpeed = atof(str.c_str());
		break;
	case 3:
		m_crouchSpeed = atof(str.c_str());
		break;
	case 4:
		m_crawlSpeed = atof(str.c_str());
		break;
	case 5:
		m_sprintPanSpeed = atof(str.c_str());
		break;
	case 6:
		m_runPanSpeed = atof(str.c_str());
		break;
	case 7:
		m_walkPanSpeed = atof(str.c_str());
		break;
	case 8:
		m_crouchPanSpeed = atof(str.c_str());
		break;
	case 9:
		m_crawlPanSpeed = atof(str.c_str());
		break;
	case 10:
		m_standHeight = atof(str.c_str());
		break;
	case 11:
		m_walkableHeight = atof(str.c_str());
		break;
	case 12:
		m_crouchHeight = atof(str.c_str());
		break;
	case 13:
		m_crouchableHeight = atof(str.c_str());
		break;
	case 14:
		m_crawlHeight = atof(str.c_str());
		break;
	case 15:
		m_crawlableHeight = atof(str.c_str());
		break;
	case 16:
		m_sprintContinuableTime = atof(str.c_str());
		break;
	case 17:
		m_turnToIdleTime = atof(str.c_str());
		break;
	case 18:
		m_postDofName[0] = str;
		break;
	case 19:
		m_postDofName[1] = str;
		break;
	case 20:
		m_postDofName[2] = str;
		break;
	case 21:
		m_postDofName[3] = str;
		break;
	case 22:
		//m_min = atof(str.c_str());
		crArgumentParser::appAtoVec(str,m_postDofParam[0]);
		break;
	case 23:
		//m_max = atof(str.c_str());
		crArgumentParser::appAtoVec(str,m_postDofParam[1]);
		break;
	case 24:
		//m_step = atof(str.c_str());
		crArgumentParser::appAtoVec(str,m_postDofParam[2]);
		break;
	case 25://可考虑放到武器属性里
		m_postTargetOffset = atof(str.c_str());
		break;
	case 26://可考虑放到武器属性里
		m_fireScale = atof(str.c_str());
		break;
	}
}
void crFpsCharacterUpdateMethod::init()
{
	//for(int i = 0; i<4; i++)
	//{
	//	if(!m_postDof[i].valid() && m_postDofName[i].compare("NULL")!=0)
	//	{
	//		m_postDof[i] = dynamic_cast<crDOFTransform *>(crFilterRenderManager::getInstance()->getFilterNode(m_postDofName[i],TRANSFORM));
	//	}
	//}

	m_init = true;
}
void crFpsCharacterUpdateMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(body->getBodyChild());
	//if(!bodyNode) bodyNode = dynamic_cast<CreBodyNode *>((dynamic_cast<crGroup *>(body->getActiveBodyFirstChild()))->getChild(0));
	crRadarMatterObject *radar = body->getRadar();

	if(!m_init) init();

	if(bodyNode && radar)
	{
		std::string downAct,effectStr;
		float height = radar->getGndDetectHeight();
		std::string weaponActStr;
		crWeaponMatterObject *weapon = body->getCurrentWeaponMatterObject();
		float weaponSpeedScale = 1.0f;
		float postIncrement = 0.0f;
		float postMin = 0.0f;
		float postMax = 0.0f;
		if(weapon) 
		{
			CRCore::crArgumentParser::readKeyValue(weapon->getDescriptions(),"WeaponActStr",weaponActStr);
			CRCore::crArgumentParser::readKeyValue(weapon->getDescriptions(),"SpeedScale",weaponSpeedScale);
		}
		DownState downState = DS_None;
		UpState upState = US_None;
		if(height>0.5f)
		{//下落过程中
			if(m_currentDownState!=DS_Jump)
			{
				if(body->getCanFalldown())
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
		else
		{
			if(m_wantChangeDownState != DS_None)
			{
				downState = m_wantChangeDownState;
				m_wantChangeDownState = DS_None;
				m_downStateInterval = m_wantDownStateInterval;
				m_wantDownStateInterval = 0.0f;
			}
			else if(m_currentDownState == DS_Falldown/*||m_currentDownState == DS_Jump*/)
			{
				downAct = "Landfall";
				effectStr = "Landfall";
				downState = DS_Landfall;
				m_downStateInterval = 0.1;
			}  
		}

		if(m_wantChangeUpState != US_None)
		{
			upState = m_wantChangeUpState;
			m_wantChangeUpState = US_None;
			m_upStateInterval = m_wantUpStateInterval;
			m_wantUpStateInterval = 0.0f;
		}
		float frameInterval = body->getCurrentFrameInterval();
		if(downState != DS_None && downState != m_currentDownState)
		{
			m_currentDownState = downState;
			m_currentDownStateStartTime = 0.0f;
		}
		else
		{
			m_currentDownStateStartTime += frameInterval;
			if(m_currentDownState == DS_Stand && m_currentUpState == US_None)
			{
				if(m_currentDownStateStartTime>=m_turnToIdleTime)
				{
					m_currentDownState = DS_Idle;
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f;
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
			else if(m_currentDownState == DS_Sprint)
			{
				if(m_currentDownStateStartTime>=m_sprintContinuableTime)
				{
					m_currentDownState = DS_Walk;//在DS_Walk状态下不可以直接切换回DS_Sprint，必须在DS_Run状态下，才可以。
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f;
				}
			}
			else if(m_downStateInterval>0.0f && m_currentDownStateStartTime>=m_downStateInterval)
			{//== 0.0f为无穷
				m_currentDownState = m_currentDownState == DS_Jump?DS_Falldown:DS_Stand;
				m_currentDownStateStartTime = 0.0f;
				m_downStateInterval = 0.0f;
			}
		}

		if(upState != US_None && upState != m_currentUpState)
		{
			m_currentUpState = upState;
			m_currentUpStateStartTime = 0.0f;
		}
		else
		{
			m_currentUpStateStartTime += frameInterval;
			if(m_upStateInterval>0.0f && m_currentUpStateStartTime>=m_upStateInterval)
			{//== 0.0f为无穷
				m_currentUpState = US_None;
				m_currentUpStateStartTime = 0.0f;
				m_upStateInterval = 0.0f;
			}
		}

		if(m_currentUpState == US_WeaponAim)
			m_isInWeaponAim = true;
		else if(m_currentUpState == US_WeaponUnAim)
			m_isInWeaponAim = false;

		crVector2f baseVel;
		switch (m_currentDownState)
		{
		case DS_Stand:
			downAct = "Stand";
			postIncrement = -m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			break;
		case DS_Idle:
			downAct = "Idle";
			postIncrement = -m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			break;
		case DS_Jump:
			downAct = "Jump";
			effectStr = "Jump";
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			break;
		case DS_Falldown:
			downAct = "Falldown";
			effectStr = "Falldown";
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			break;
		case DS_Landfall:
			downAct = "Landfall";
			effectStr = "Landfall";
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			break;
		case DS_Sprint:
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			baseVel.set(m_sprintSpeed,m_sprintPanSpeed);
			if(m_move == SprintUp)
				downAct = "SprintFront";
			else if(m_move == SprintDown)
				downAct = "SprintBack";
			if(!downAct.empty())
			{
				if(m_pan == SprintPanLeft)
					downAct += "Left";
				else if(m_pan == SprintPanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == SprintPanLeft)
					downAct = "SprintLeft";
				else if(m_pan == SprintPanRight)
					downAct = "SprintRight";
			}
			effectStr = "Sprint";
			break;
		case DS_Run:
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			baseVel.set(m_runSpeed,m_runPanSpeed);
			if(m_move == RunUp)
				downAct = "RunFront";
			else if(m_move == RunDown)
				downAct = "RunBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft)
					downAct = "RunLeft";
				else if(m_pan == RunPanRight)
					downAct = "RunRight";
			}
			effectStr = "Run";
			break;
		case DS_Walk:
			postIncrement = m_postDofParam[0][2];
			postMin = m_postDofParam[0][0];
			postMax = m_postDofParam[0][1];
			baseVel.set(m_walkSpeed,m_walkPanSpeed);
			if(m_move == MoveUp)
				downAct = "WalkFront";
			else if(m_move == MoveDown)
				downAct = "WalkBack";
			if(!downAct.empty())
			{
				if(m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == MovePanLeft)
					downAct = "WalkLeft";
				else if(m_pan == MovePanRight)
					downAct = "WalkRight";
			}
			effectStr = "Walk";
			break;
		case DS_Crouch:
			postMin = m_postDofParam[1][0];
			postMax = m_postDofParam[1][1];
			baseVel.set(m_crouchSpeed,m_crouchPanSpeed);
			if(m_move == RunUp || m_move == MoveUp)
				downAct = "CrouchFront";
			else if(m_move == RunDown || m_move == MoveDown)
				downAct = "CrouchBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct = "CrouchLeft";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct = "CrouchRight";
			}
			if(downAct.find("Crouch")!=std::string::npos)
				postIncrement = m_postDofParam[1][2];
			else
				postIncrement = -m_postDofParam[1][2];
			effectStr = "Crouch";
			break;
		case DS_Crawl:
			postIncrement = -m_postDofParam[2][2];
			postMin = m_postDofParam[2][0];
			postMax = m_postDofParam[2][1];
			baseVel.set(m_crawlSpeed,m_crawlPanSpeed);
			if(m_move == RunUp || m_move == MoveUp)
				downAct = "CrawlFront";
			else if(m_move == RunDown || m_move == MoveDown)
				downAct = "CrawlBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct = "CrawlLeft";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct = "CrawlRight";
			}
			if(downAct.find("Crawl")!=std::string::npos)
				postIncrement = m_postDofParam[1][2];
			else
				postIncrement = -m_postDofParam[1][2];
			effectStr = "Crawl";
			break;
		case DS_Dead:
			downAct = "Dead";
			effectStr = "Dead";
			break;
		}
		if(m_isInWeaponAim)
		{
			baseVel *= m_aimSpeedScale;
		}
		else
		{
			baseVel *= weaponSpeedScale;
		}

		if(m_currentDownState != DS_Jump && m_currentDownState != DS_Falldown)
		{
			switch (m_move)
			{
			case StopMove:
				body->setVelocity0(0.0f);
				break;
			case MoveUp:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case RunUp:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case SprintUp:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case MoveDown:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			case RunDown:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			case SprintDown:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			}

			switch (m_pan)
			{
			case StopPan:
				body->setVelocity1(0.0f);
				break;
			case MovePanLeft:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case RunPanLeft:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case SprintPanLeft:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case MovePanRight:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
			case RunPanRight:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
			case SprintPanRight:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
				break;
			}
			if(m_move <= StopMove && m_pan <= StopPan)
			{
				if(m_currentDownState == DS_None || m_currentDownState == DS_Run || m_currentDownState == DS_Walk || m_currentDownState == DS_Sprint)
				{
					m_currentDownState = DS_Stand;
					m_downStateInterval = 0.0f;
				}
			}
		}

		if(downAct.empty()) downAct = "Stand";
		downAct += weaponActStr;

		std::string upAct,finalAct;

		if(m_currentUpState == US_WeaponFire)
		{
			if(m_isInWeaponAim)
			{
				upAct = "WeaponAimFire";
				//postIncrement = fabs(postIncrement) * m_fireScale;
			}
			else
			{
				upAct = "WeaponFire";
				postIncrement = fabs(postIncrement) * m_fireScale;
			}
		}
		else if(m_currentUpState == US_ReplaceClip)
		{
			upAct = "ReplaceClip";
		}
		else if(m_currentUpState == US_WeaponUnSelect)
		{
			upAct = "WeaponUnSelect";
		}
		else if(m_currentUpState == US_WeaponSelect)
		{
			upAct = "WeaponSelect";
		}
		else if(m_currentUpState == US_WeaponAim)
		{
			upAct = "WeaponAim";
		}
		else if(m_currentUpState == US_WeaponUnAim)
		{
			upAct = "WeaponUnAim";
		}
		else
		{
			if(m_isInWeaponAim)
			{
				upAct = "WeaponInAim";
			}
		}

		if(m_currentDownState == DS_Dead)
		{
			finalAct = "Dead";
		}
		else
		{
			finalAct = upAct+downAct;
		}

		ActionRequest* action = bodyNode->getBody()->getActionPrototype( "ACT_"+finalAct );
		if (action) 
		{
			action->setPrioritized(false);
			bodyNode->getBody()->executeAction(action, false);
		}

		if(m_currentDownState == DS_Crouch)
		{
			body->setViewHeight(m_crouchHeight);
			body->setWalkableHeight(m_crouchableHeight);
		}
		else if(m_currentDownState == DS_Crawl)
		{
			body->setViewHeight(m_crawlHeight);
			body->setWalkableHeight(m_crawlableHeight);
		}
		else
		{
			body->setViewHeight(m_standHeight);
			body->setWalkableHeight(m_walkableHeight);
		}

		if(m_postDof[0].valid())
		{
			crVector3f incrementTrans,trans;
			for(int i = 0; i<4; i++)
			{
				if(m_postDof[i].valid())
				{
					incrementTrans = m_postDof[i]->getIncrementTranslate();
					m_postDof[i]->setAnimationOn(true);
					incrementTrans[1] = postIncrement;
					m_postDof[i]->setIncrementTranslate(incrementTrans);

					trans = m_postDof[i]->getMaxTranslate();
					trans[1] = postMax;
					m_postDof[i]->setMaxTranslate(trans);

					trans = m_postDof[i]->getMinTranslate();
					trans[1] = postMin;
					m_postDof[i]->setMinTranslate(trans);
				}
			}

			body->getGameBodyInfo()->setTargetRange(m_postDof[0]->getCurrentTranslate()[1] + m_postTargetOffset);
		}

		//move effect
		if(!effectStr.empty())
		{
			std::string attrStr;
			crNode *gndNode = radar->getGndDetectNode();
			if(gndNode) CRCore::crArgumentParser::readKeyValue(gndNode->getDescriptions(),"AttributeString",attrStr);
			if(attrStr.empty()) attrStr = "stone";

			std::string bodyAttrStr;
			CRCore::crArgumentParser::readKeyValue(body->getDescriptions(),"AttributeString",bodyAttrStr);
			if(bodyAttrStr.empty()) bodyAttrStr = "human";

			std::string effectString = bodyAttrStr+"_"+attrStr+"_"+effectStr;

			//CRCore::notify(CRCore::FATAL)<<" crCharacterMoveActionCallback::operator(): effectString = "<<effectString<<std::endl;

			crVector3f position = body->getTrans();
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
			//m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(body->getName(),effectString);
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
}
bool crFpsCharacterUpdateMethod::wantChangeDownState(DownState state, float interval)
{
	switch (state)
	{
	case DS_Jump:
		if(m_currentDownState == DS_Falldown || m_currentDownState == DS_Jump)
			return false;
		else if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		break;
	case DS_Crouch:
		if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Crawl:
		if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Sprint:
		if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Run:
		if( m_currentDownState == DS_Jump || 
			m_currentDownState == DS_Sprint || 
			m_currentDownState == DS_Crouch || 
			m_currentDownState == DS_Crawl )
		{
			return false;
		}
		break;
	case DS_Walk:
		if( m_currentDownState == DS_Jump || 
			m_currentDownState == DS_Crouch || 
			m_currentDownState == DS_Crawl )
		{
			return false;
		}
		break;
	}
	m_wantDownStateInterval = interval;
	m_wantChangeDownState = state;
	return true;
}

bool crFpsCharacterUpdateMethod::wantChangeUpState(UpState state, float interval)
{
	switch (state)
	{
	case US_WeaponFire:
		if(m_currentUpState != US_None)
			return false;
		break;
	case US_ReplaceClip:
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
	case US_WeaponAim:
		if(m_currentUpState != US_None)
			return false;
		if(m_isInWeaponAim)
			state = US_WeaponUnAim;
		break;
	}
	m_wantUpStateInterval = interval;
	m_wantChangeUpState = state;
	return true;
}

bool crFpsCharacterUpdateMethod::wantChangeMove(Move move)
{
	switch (move)
	{
	case SprintUp:
		if(m_move != RunUp) 
		{
			m_move = MoveUp;
			return false;
		}
		break;
	case SprintDown:
		if(m_move != RunDown)
		{
			m_move = MoveDown;
			return false;
		}
		break;
	}
	m_move = move;
	return true;
}
bool crFpsCharacterUpdateMethod::wantChangePan(Pan pan)
{
	switch (pan)
	{
	case SprintPanLeft:
		if(m_pan != RunPanLeft)
		{
			m_pan = MovePanLeft;
			return false;
		}
		break;
	case SprintPanRight:
		if(m_pan != RunPanRight)
		{
			m_pan = MovePanRight;
			return false;
		}
		break;
	}
	m_pan = pan;
	return true;
}

unsigned int crFpsCharacterUpdateMethod::getCurrentDownState()
{
	return m_currentDownState;
}

unsigned int crFpsCharacterUpdateMethod::getCurrentUpState()
{
	return m_currentUpState;
}

float crFpsCharacterUpdateMethod::getSprintSpeed()
{
	return m_sprintSpeed;
}
float crFpsCharacterUpdateMethod::getRunSpeed()
{
	return m_runSpeed;
}
float crFpsCharacterUpdateMethod::getWalkSpeed()
{
	return m_walkSpeed;
}
float crFpsCharacterUpdateMethod::getCrouchSpeed()
{
	return m_crouchSpeed;
}
float crFpsCharacterUpdateMethod::getCrawlSpeed()
{
	return m_crawlSpeed;
}
float crFpsCharacterUpdateMethod::getSprintPanSpeed()
{
	return m_sprintPanSpeed;
}
float crFpsCharacterUpdateMethod::getRunPanSpeed()
{
	return m_runPanSpeed;
}
float crFpsCharacterUpdateMethod::getWalkPanSpeed()
{
	return m_walkPanSpeed;
}
float crFpsCharacterUpdateMethod::getCrouchPanSpeed()
{
	return m_crouchPanSpeed;
}
float crFpsCharacterUpdateMethod::getCrawlPanSpeed()
{
	return m_crawlPanSpeed;
}
/////////////////////////////////////////
//
//crFpsCharacterStateChangeMethod
//
/////////////////////////////////////////
crFpsCharacterStateChangeMethod::crFpsCharacterStateChangeMethod():
m_downState(crFpsCharacterUpdateMethod::DS_None),
m_upState(crFpsCharacterUpdateMethod::US_None),
m_downInterval(0.0f),
m_upInterval(0.0f),
m_lastUpdTime(0.0f),
m_updInterval(0.5f)
{
}

crFpsCharacterStateChangeMethod::crFpsCharacterStateChangeMethod(const crFpsCharacterStateChangeMethod& handle):
crMethod(handle),
m_downState(handle.m_downState),
m_upState(handle.m_upState),
m_downInterval(handle.m_downInterval),
m_upInterval(handle.m_upInterval),
m_lastUpdTime(handle.m_lastUpdTime),
m_updInterval(handle.m_updInterval)
{
}

void crFpsCharacterStateChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crFpsCharacterStateChangeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_updInterval = atof(str.c_str());
		break;
	case 1:
		if(str.compare("None")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_None;
		else if(str.compare("Stand")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Stand;
		else if(str.compare("Idle")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Idle;
		else if(str.compare("Jump")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Jump;
		else if(str.compare("Falldown")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Falldown;
		else if(str.compare("Sprint")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Sprint;
		else if(str.compare("Run")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Run;
		else if(str.compare("Walk")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Walk;
		else if(str.compare("Crouch")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Crouch;
		else if(str.compare("Crawl")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Crawl;
		else if(str.compare("Dead")==0)
			m_downState = crFpsCharacterUpdateMethod::DS_Dead;
		break;
	case 2:
		m_downInterval = atof(str.c_str());
		break;
	case 3:
		if(str.compare("None")==0)
			m_upState = crFpsCharacterUpdateMethod::US_None;
		else if(str.compare("WeaponFire")==0)
			m_upState = crFpsCharacterUpdateMethod::US_WeaponFire;
		else if(str.compare("ReplaceClip")==0)
			m_upState = crFpsCharacterUpdateMethod::US_ReplaceClip;
		else if(str.compare("WeaponUnSelect")==0)
			m_upState = crFpsCharacterUpdateMethod::US_WeaponUnSelect;
		else if(str.compare("WeaponSelect")==0)
			m_upState = crFpsCharacterUpdateMethod::US_WeaponSelect;
		else if(str.compare("WeaponAim")==0)
			m_upState = crFpsCharacterUpdateMethod::US_WeaponAim;
		break;
	case 4:
		m_upInterval = atof(str.c_str());
		break;
	}
}

void crFpsCharacterStateChangeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	bool cancelTask = true;
	if(!m_characterUpdateMethod.valid())
	{
		m_characterUpdateMethod = dynamic_cast<crFpsCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"FpsCharacterUpdate"));
	}
	if(m_characterUpdateMethod.valid())
	{
		//crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
		//float frameInterval = body->getCurrentFrameInterval();
		////if(m_downState == crFpsCharacterUpdateCallback::DS_Run)
		////	CRCore::notify(CRCore::ALWAYS)<<"DS_Run "<<frametime<< std::endl;
  //      m_lastUpdTime += frameInterval;
		//if(m_lastUpdTime > m_updInterval)
		//{
			if(m_downState != crFpsCharacterUpdateMethod::DS_None)
			{
				if(m_characterUpdateMethod->wantChangeDownState(m_downState,m_downInterval))
				{
					//if(m_downState == crFpsCharacterUpdateCallback::DS_Run)
					//	CRCore::notify(CRCore::ALWAYS)<<"DS_Run1 "<<frametime<< std::endl;
					//m_lastUpdTime = 0.0f;
					cancelTask = false;
				}
			}
			if(m_upState != crFpsCharacterUpdateMethod::US_None)
			{
				if(m_characterUpdateMethod->wantChangeUpState(m_upState,m_upInterval))
				{
					//m_lastUpdTime = 0.0f;
					cancelTask = false;
				}
			}
		//}
	}
	if(cancelTask)
	{
		handle.outputParam(0,&cancelTask);
	}
}
/////////////////////////////////////////
//
//crFpsCharacterJumpMethod
//
/////////////////////////////////////////
crFpsCharacterJumpMethod::crFpsCharacterJumpMethod():
m_jumpVel(3.5f),
m_timeInterval(0.8f)
{
}

crFpsCharacterJumpMethod::crFpsCharacterJumpMethod(const crFpsCharacterJumpMethod& handle):
crMethod(handle),
m_jumpVel(handle.m_jumpVel),
m_timeInterval(handle.m_timeInterval)
{
}

void crFpsCharacterJumpMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crFpsCharacterJumpMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jumpVel = atof(str.c_str());
		break;
	case 1:
		m_timeInterval = atof(str.c_str());
		break;
	}
}

void crFpsCharacterJumpMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	//CRCore::notify(CRCore::FATAL)<<" crStopActionCallback::operator(): m_actionName = "<<m_actionName<<std::endl;
	//需要修改
	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	if(body)
	{
		body->setLinearVel(crVector3f(0.0f,0.0f,m_jumpVel), m_timeInterval);
	}
}
/////////////////////////////////////////
//
//crFpsCharacterMoveChangeMethod
//
/////////////////////////////////////////
crFpsCharacterMoveChangeMethod::crFpsCharacterMoveChangeMethod():
m_move(crFpsCharacterUpdateMethod::M_None),
m_pan(crFpsCharacterUpdateMethod::P_None)
{
}

crFpsCharacterMoveChangeMethod::crFpsCharacterMoveChangeMethod(const crFpsCharacterMoveChangeMethod& handle):
crMethod(handle),
m_move(handle.m_move),
m_pan(handle.m_pan)
{
}

void crFpsCharacterMoveChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crFpsCharacterMoveChangeMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		if(str.compare("None")==0)
			m_move = crFpsCharacterUpdateMethod::M_None;
		else if(str.compare("StopMove")==0)
			m_move = crFpsCharacterUpdateMethod::StopMove;
		else if(str.compare("MoveUp")==0)
			m_move = crFpsCharacterUpdateMethod::MoveUp;
		else if(str.compare("MoveDown")==0)
			m_move = crFpsCharacterUpdateMethod::MoveDown;
		else if(str.compare("RunUp")==0)
			m_move = crFpsCharacterUpdateMethod::RunUp;
		else if(str.compare("RunDown")==0)
			m_move = crFpsCharacterUpdateMethod::RunDown;
		else if(str.compare("SprintUp")==0)
			m_move = crFpsCharacterUpdateMethod::SprintUp;
		else if(str.compare("SprintDown")==0)
			m_move = crFpsCharacterUpdateMethod::SprintDown;
	}
	else if(i==1)
	{
		if(str.compare("None")==0)
			m_pan = crFpsCharacterUpdateMethod::P_None;
		if(str.compare("StopPan")==0)
			m_pan = crFpsCharacterUpdateMethod::StopPan;
		else if(str.compare("MovePanLeft")==0)
			m_pan = crFpsCharacterUpdateMethod::MovePanLeft;
		else if(str.compare("MovePanRight")==0)
			m_pan = crFpsCharacterUpdateMethod::MovePanRight;
		else if(str.compare("RunPanLeft")==0)
			m_pan = crFpsCharacterUpdateMethod::RunPanLeft;
		else if(str.compare("RunPanRight")==0)
			m_pan = crFpsCharacterUpdateMethod::RunPanRight;
		else if(str.compare("SprintPanLeft")==0)
			m_pan = crFpsCharacterUpdateMethod::SprintPanLeft;
		else if(str.compare("SprintPanRight")==0)
			m_pan = crFpsCharacterUpdateMethod::SprintPanRight;
	}
}

void crFpsCharacterMoveChangeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	if(body)
	{
		if(!m_characterUpdateMethod.valid())
		{
			m_characterUpdateMethod = dynamic_cast<crFpsCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"FpsCharacterUpdate"));
		}
		if(m_characterUpdateMethod.valid())
		{
			if(m_move != crFpsCharacterUpdateMethod::M_None) 
			{
				m_characterUpdateMethod->wantChangeMove(m_move);
			}
			if(m_pan != crFpsCharacterUpdateMethod::P_None) 
			{
				m_characterUpdateMethod->wantChangePan(m_pan);
			}
		}
	}
}
/////////////////////////////////////////
//
//crRpgCharacterUpdateMethod
//
/////////////////////////////////////////
crRpgCharacterUpdateMethod::crRpgCharacterUpdateMethod():
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
m_isInWeaponAim(false),
m_aimSpeedScale(0.0f),
m_move(M_None),
m_pan(P_None),
//m_init(false),
m_standHeight(1.6f),
m_walkableHeight(0.5f),
m_crouchHeight(1.2f),
m_crouchableHeight(0.4f),
m_crawlHeight(0.81f),
m_crawlableHeight(0.1f)
{
}

crRpgCharacterUpdateMethod::crRpgCharacterUpdateMethod(const crRpgCharacterUpdateMethod& handle):
crMethod(handle),
m_sprintSpeed(handle.m_sprintSpeed),
m_runSpeed(handle.m_runSpeed),
m_walkSpeed(handle.m_walkSpeed),
m_crouchSpeed(handle.m_crouchSpeed),
m_crawlSpeed(handle.m_crawlSpeed),
m_sprintPanSpeed(handle.m_sprintPanSpeed),
m_runPanSpeed(handle.m_runPanSpeed),
m_walkPanSpeed(handle.m_walkPanSpeed),
m_crouchPanSpeed(handle.m_crouchPanSpeed),
m_crawlPanSpeed(handle.m_crawlPanSpeed),
m_sprintContinuableTime(handle.m_sprintContinuableTime),
m_turnToIdleTime(handle.m_turnToIdleTime),
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
m_isInWeaponAim(false),
m_aimSpeedScale(handle.m_aimSpeedScale),
m_move(M_None),
m_pan(P_None),
//m_init(false),
m_standHeight(handle.m_standHeight),
m_walkableHeight(handle.m_walkableHeight),
m_crouchHeight(handle.m_crouchHeight),
m_crouchableHeight(handle.m_crouchableHeight),
m_crawlHeight(handle.m_crawlHeight),
m_crawlableHeight(handle.m_crawlableHeight)
{
}

void crRpgCharacterUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRpgCharacterUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sprintSpeed = atof(str.c_str());
		break;
	case 1:
		m_runSpeed = atof(str.c_str());
		break;
	case 2:
		m_walkSpeed = atof(str.c_str());
		break;
	case 3:
		m_crouchSpeed = atof(str.c_str());
		break;
	case 4:
		m_crawlSpeed = atof(str.c_str());
		break;
	case 5:
		m_sprintPanSpeed = atof(str.c_str());
		break;
	case 6:
		m_runPanSpeed = atof(str.c_str());
		break;
	case 7:
		m_walkPanSpeed = atof(str.c_str());
		break;
	case 8:
		m_crouchPanSpeed = atof(str.c_str());
		break;
	case 9:
		m_crawlPanSpeed = atof(str.c_str());
		break;
	case 10:
		m_standHeight = atof(str.c_str());
		break;
	case 11:
		m_walkableHeight = atof(str.c_str());
		break;
	case 12:
		m_crouchHeight = atof(str.c_str());
		break;
	case 13:
		m_crouchableHeight = atof(str.c_str());
		break;
	case 14:
		m_crawlHeight = atof(str.c_str());
		break;
	case 15:
		m_crawlableHeight = atof(str.c_str());
		break;
	case 16:
		m_sprintContinuableTime = atof(str.c_str());
		break;
	case 17:
		m_turnToIdleTime = atof(str.c_str());
		break;
	//case 18:
	//	m_postDofName[0] = str;
	//	break;
	//case 19:
	//	m_postDofName[1] = str;
	//	break;
	//case 20:
	//	m_postDofName[2] = str;
	//	break;
	//case 21:
	//	m_postDofName[3] = str;
	//	break;
	//case 22:
	//	//m_min = atof(str.c_str());
	//	crArgumentParser::readKeyValue(str,"VEC3",m_postDofParam[0]);
	//	break;
	//case 23:
	//	//m_max = atof(str.c_str());
	//	crArgumentParser::readKeyValue(str,"VEC3",m_postDofParam[1]);
	//	break;
	//case 24:
	//	//m_step = atof(str.c_str());
	//	crArgumentParser::readKeyValue(str,"VEC3",m_postDofParam[2]);
	//	break;
	//case 25://可考虑放到武器属性里
	//	m_postTargetOffset = atof(str.c_str());
	//	break;
	//case 26://可考虑放到武器属性里
	//	m_fireScale = atof(str.c_str());
	//	break;
	}
}
//void crRpgCharacterUpdateMethod::init()
//{
//	for(int i = 0; i<4; i++)
//	{
//		if(!m_postDof[i].valid() && m_postDofName[i].compare("NULL")!=0)
//		{
//			m_postDof[i] = dynamic_cast<crDOFTransform *>(crFilterRenderManager::getInstance()->getFilterNode(m_postDofName[i],TRANSFORM));
//		}
//	}
//
//	m_init = true;
//}
void crRpgCharacterUpdateMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(body->getBodyChild());
	//if(!bodyNode) bodyNode = dynamic_cast<CreBodyNode *>((dynamic_cast<crGroup *>(body->getActiveBodyFirstChild()))->getChild(0));
	crRadarMatterObject *radar = body->getRadar();

	//if(!m_init) init();

	if(bodyNode && radar)
	{
		std::string downAct,effectStr;
		float height = radar->getGndDetectHeight();
		std::string weaponActStr;
		crWeaponMatterObject *weapon = body->getCurrentWeaponMatterObject();
		float weaponSpeedScale = 1.0f;
		//float postIncrement = 0.0f;
		//float postMin = 0.0f;
		//float postMax = 0.0f;
		if(weapon) 
		{
			CRCore::crArgumentParser::readKeyValue(weapon->getDescriptions(),"WeaponActStr",weaponActStr);
			CRCore::crArgumentParser::readKeyValue(weapon->getDescriptions(),"SpeedScale",weaponSpeedScale);
		}
		DownState downState = DS_None;
		UpState upState = US_None;
		if(height>0.5f)
		{//下落过程中
			if(m_currentDownState!=DS_Jump)
			{
				if(body->getCanFalldown())
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
		else
		{
			if(m_wantChangeDownState != DS_None)
			{
				downState = m_wantChangeDownState;
				m_wantChangeDownState = DS_None;
				m_downStateInterval = m_wantDownStateInterval;
				m_wantDownStateInterval = 0.0f;
			}
			else if(m_currentDownState == DS_Falldown/*||m_currentDownState == DS_Jump*/)
			{
				downAct = "Landfall";
				effectStr = "Landfall";
				downState = DS_Landfall;
				m_downStateInterval = 0.1;
			}  
		}

		if(m_wantChangeUpState != US_None)
		{
			upState = m_wantChangeUpState;
			m_wantChangeUpState = US_None;
			m_upStateInterval = m_wantUpStateInterval;
			m_wantUpStateInterval = 0.0f;
		}

		float frameInterval = body->getCurrentFrameInterval();
		if(downState != DS_None && downState != m_currentDownState)
		{
			m_currentDownState = downState;
			m_currentDownStateStartTime = 0.0f;
		}
		else
		{
			m_currentDownStateStartTime += frameInterval;
			if(m_currentDownState == DS_Stand && m_currentUpState == US_None)
			{
				if(m_currentDownStateStartTime>=m_turnToIdleTime)
				{
					m_currentDownState = DS_Idle;
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f;
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
			else if(m_currentDownState == DS_Sprint)
			{
				if(m_currentDownStateStartTime>=m_sprintContinuableTime)
				{
					m_currentDownState = DS_Walk;//在DS_Walk状态下不可以直接切换回DS_Sprint，必须在DS_Run状态下，才可以。
					m_currentDownStateStartTime = 0.0f;
					m_downStateInterval = 0.0f;
				}
			}
			else if(m_downStateInterval>0.0f && m_currentDownStateStartTime>=m_downStateInterval)
			{//== 0.0f为无穷
				m_currentDownState = m_currentDownState == DS_Jump?DS_Falldown:DS_Stand;
				m_currentDownStateStartTime = 0.0f;
				m_downStateInterval = 0.0f;
			}
		}

		if(upState != US_None && upState != m_currentUpState)
		{
			m_currentUpState = upState;
			m_currentUpStateStartTime = 0.0f;
		}
		else
		{
			m_currentUpStateStartTime += frameInterval;
			if(m_upStateInterval>0.0f && m_currentUpStateStartTime>=m_upStateInterval)
			{//== 0.0f为无穷
				m_currentUpState = US_None;
				m_currentUpStateStartTime = 0.0f;
				m_upStateInterval = 0.0f;
			}
		}

		if(m_currentUpState == US_WeaponAim)
			m_isInWeaponAim = true;
		else if(m_currentUpState == US_WeaponUnAim)
			m_isInWeaponAim = false;

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
		case DS_Sprint:
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			baseVel.set(m_sprintSpeed,m_sprintPanSpeed);
			if(m_move == SprintUp)
				downAct = "SprintFront";
			else if(m_move == SprintDown)
				downAct = "SprintBack";
			if(!downAct.empty())
			{
				if(m_pan == SprintPanLeft)
					downAct += "Left";
				else if(m_pan == SprintPanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == SprintPanLeft)
					downAct = "SprintLeft";
				else if(m_pan == SprintPanRight)
					downAct = "SprintRight";
			}
			effectStr = "Sprint";
			break;
		case DS_Run:
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			baseVel.set(m_runSpeed,m_runPanSpeed);
			if(m_move == RunUp)
				downAct = "RunFront";
			else if(m_move == RunDown)
				downAct = "RunBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft)
					downAct = "RunLeft";
				else if(m_pan == RunPanRight)
					downAct = "RunRight";
			}
			effectStr = "Run";
			break;
		case DS_Walk:
			//postIncrement = m_postDofParam[0][2];
			//postMin = m_postDofParam[0][0];
			//postMax = m_postDofParam[0][1];
			baseVel.set(m_walkSpeed,m_walkPanSpeed);
			if(m_move == MoveUp)
				downAct = "WalkFront";
			else if(m_move == MoveDown)
				downAct = "WalkBack";
			if(!downAct.empty())
			{
				if(m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == MovePanLeft)
					downAct = "WalkLeft";
				else if(m_pan == MovePanRight)
					downAct = "WalkRight";
			}
			effectStr = "Walk";
			break;
		case DS_Crouch:
			//postMin = m_postDofParam[1][0];
			//postMax = m_postDofParam[1][1];
			baseVel.set(m_crouchSpeed,m_crouchPanSpeed);
			if(m_move == RunUp || m_move == MoveUp)
				downAct = "CrouchFront";
			else if(m_move == RunDown || m_move == MoveDown)
				downAct = "CrouchBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct = "CrouchLeft";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct = "CrouchRight";
			}
			//if(downAct.find("Crouch")!=std::string::npos)
			//	postIncrement = m_postDofParam[1][2];
			//else
			//	postIncrement = -m_postDofParam[1][2];
			effectStr = "Crouch";
			break;
		case DS_Crawl:
			//postIncrement = -m_postDofParam[2][2];
			//postMin = m_postDofParam[2][0];
			//postMax = m_postDofParam[2][1];
			baseVel.set(m_crawlSpeed,m_crawlPanSpeed);
			if(m_move == RunUp || m_move == MoveUp)
				downAct = "CrawlFront";
			else if(m_move == RunDown || m_move == MoveDown)
				downAct = "CrawlBack";
			if(!downAct.empty())
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct += "Left";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct += "Right";
			}
			else
			{
				if(m_pan == RunPanLeft || m_pan == MovePanLeft)
					downAct = "CrawlLeft";
				else if(m_pan == RunPanRight || m_pan == MovePanRight)
					downAct = "CrawlRight";
			}
			//if(downAct.find("Crawl")!=std::string::npos)
			//	postIncrement = m_postDofParam[1][2];
			//else
			//	postIncrement = -m_postDofParam[1][2];
			effectStr = "Crawl";
			break;
		case DS_Dead:
			downAct = "Dead";
			effectStr = "Dead";
			break;
		}
		if(m_isInWeaponAim)
		{
			baseVel *= m_aimSpeedScale;
		}
		else
		{
			baseVel *= weaponSpeedScale;
		}

		if(m_currentDownState != DS_Jump && m_currentDownState != DS_Falldown)
		{
			switch (m_move)
			{
			case StopMove:
				body->setVelocity0(0.0f);
				break;
			case MoveUp:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case RunUp:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case SprintUp:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity0(baseVel[0]);
				break;
			case MoveDown:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			case RunDown:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			case SprintDown:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity0(-baseVel[0]);
				break;
			}

			switch (m_pan)
			{
			case StopPan:
				body->setVelocity1(0.0f);
				break;
			case MovePanLeft:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case RunPanLeft:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case SprintPanLeft:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(baseVel[1]);
				break;
			case MovePanRight:
				wantChangeDownState(DS_Walk,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
			case RunPanRight:
				wantChangeDownState(DS_Run,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
			case SprintPanRight:
				wantChangeDownState(DS_Sprint,0.0f);
				body->setVelocity1(-baseVel[1]);
				break;
				break;
			}
			if(m_move <= StopMove && m_pan <= StopPan)
			{
				if(m_currentDownState == DS_None || m_currentDownState == DS_Run || m_currentDownState == DS_Walk || m_currentDownState == DS_Sprint)
				{
					m_currentDownState = DS_Stand;
					m_downStateInterval = 0.0f;
				}
			}
		}

		if(downAct.empty()) downAct = "Stand";
		downAct += weaponActStr;

		std::string upAct,finalAct;

		if(m_currentUpState == US_WeaponFire)
		{
			if(m_isInWeaponAim)
			{
				upAct = "WeaponAimFire";
			}
			else
			{
				upAct = "WeaponFire";
				//postIncrement = fabs(postIncrement) * m_fireScale;
			}
		}
		else if(m_currentUpState == US_ReplaceClip)
		{
			upAct = "ReplaceClip";
		}
		else if(m_currentUpState == US_WeaponUnSelect)
		{
			upAct = "WeaponUnSelect";
		}
		else if(m_currentUpState == US_WeaponSelect)
		{
			upAct = "WeaponSelect";
		}
		else if(m_currentUpState == US_WeaponAim)
		{
			upAct = "WeaponAim";
		}
		else if(m_currentUpState == US_WeaponUnAim)
		{
			upAct = "WeaponUnAim";
		}
		else
		{
			if(m_isInWeaponAim)
			{
				upAct = "WeaponInAim";
			}
		}

		if(m_currentDownState == DS_Dead)
		{
			finalAct = "Dead";
		}
		else
		{
			finalAct = upAct+downAct;
		}

		ActionRequest* action = bodyNode->getBody()->getActionPrototype( "ACT_"+finalAct );
		if (action)
		{
			action->setPrioritized(false);
			bodyNode->getBody()->executeAction(action, false);
		}

		if(m_currentDownState == DS_Crouch)
		{
			body->setViewHeight(m_crouchHeight);
			body->setWalkableHeight(m_crouchableHeight);
		}
		else if(m_currentDownState == DS_Crawl)
		{
			body->setViewHeight(m_crawlHeight);
			body->setWalkableHeight(m_crawlableHeight);
		}
		else
		{
			body->setViewHeight(m_standHeight);
			body->setWalkableHeight(m_walkableHeight);
		}

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

		//move effect
		if(!effectStr.empty())
		{
			std::string attrStr;
			crNode *gndNode = radar->getGndDetectNode();
			if(gndNode) CRCore::crArgumentParser::readKeyValue(gndNode->getDescriptions(),"AttributeString",attrStr);
			if(attrStr.empty()) attrStr = "stone";

			std::string bodyAttrStr;
			CRCore::crArgumentParser::readKeyValue(body->getDescriptions(),"AttributeString",bodyAttrStr);
			if(bodyAttrStr.empty()) bodyAttrStr = "human";

			std::string effectString = bodyAttrStr+"_"+attrStr+"_"+effectStr;

			//CRCore::notify(CRCore::FATAL)<<" crCharacterMoveActionCallback::operator(): effectString = "<<effectString<<std::endl;

			crVector3f position = body->getTrans();
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
			//m_soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(body->getName(),effectString);
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
}
bool crRpgCharacterUpdateMethod::wantChangeDownState(DownState state, float interval)
{
	switch (state)
	{
	case DS_Jump:
		if(m_currentDownState == DS_Falldown || m_currentDownState == DS_Jump)
			return false;
		else if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		break;
	case DS_Crouch:
		if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Crawl:
		if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Sprint:
		if(m_currentDownState == DS_Crouch)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Crawl)
		{
			m_wantDownStateInterval = 0.0f;
			m_wantChangeDownState = DS_Stand;
			return false;
		}
		else if(m_currentDownState == DS_Falldown)
			return false;
		else if(m_currentDownState == DS_Jump)
			return false;
		break;
	case DS_Run:
		if( m_currentDownState == DS_Jump || 
			m_currentDownState == DS_Sprint || 
			m_currentDownState == DS_Crouch || 
			m_currentDownState == DS_Crawl )
		{
			return false;
		}
		break;
	case DS_Walk:
		if( m_currentDownState == DS_Jump || 
			m_currentDownState == DS_Crouch || 
			m_currentDownState == DS_Crawl )
		{
			return false;
		}
		break;
	//case DS_Stand:
	//	if( m_currentDownState == DS_Jump || 
	//		m_currentDownState == DS_Falldown ||
	//		m_currentDownState == DS_Landfall)
	//	{
	//		return false;
	//	}
		break;
	}
	m_wantDownStateInterval = interval;
	m_wantChangeDownState = state;
	return true;
}

bool crRpgCharacterUpdateMethod::wantChangeUpState(UpState state, float interval)
{
	switch (state)
	{
	case US_WeaponFire:
		if(m_currentUpState != US_None)
			return false;
		break;
	case US_ReplaceClip:
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
	case US_WeaponAim:
		if(m_currentUpState != US_None)
			return false;
		if(m_isInWeaponAim)
			state = US_WeaponUnAim;
		break;
	}
	m_wantUpStateInterval = interval;
	m_wantChangeUpState = state;
	return true;
}

bool crRpgCharacterUpdateMethod::wantChangeMove(Move move)
{
	switch (move)
	{
	case SprintUp:
		if(m_move != RunUp) 
		{
			m_move = MoveUp;
			return false;
		}
		break;
	case SprintDown:
		if(m_move != RunDown)
		{
			m_move = MoveDown;
			return false;
		}
		break;
	}
	m_move = move;
	return true;
}
bool crRpgCharacterUpdateMethod::wantChangePan(Pan pan)
{
	switch (pan)
	{
	case SprintPanLeft:
		if(m_pan != RunPanLeft)
		{
			m_pan = MovePanLeft;
			return false;
		}
		break;
	case SprintPanRight:
		if(m_pan != RunPanRight)
		{
			m_pan = MovePanRight;
			return false;
		}
		break;
	}
	m_pan = pan;
	return true;
}

unsigned int crRpgCharacterUpdateMethod::getCurrentDownState()
{
	return m_currentDownState;
}

unsigned int crRpgCharacterUpdateMethod::getCurrentUpState()
{
	return m_currentUpState;
}

float crRpgCharacterUpdateMethod::getSprintSpeed()
{
	return m_sprintSpeed;
}
float crRpgCharacterUpdateMethod::getRunSpeed()
{
	return m_runSpeed;
}
float crRpgCharacterUpdateMethod::getWalkSpeed()
{
	return m_walkSpeed;
}
float crRpgCharacterUpdateMethod::getCrouchSpeed()
{
	return m_crouchSpeed;
}
float crRpgCharacterUpdateMethod::getCrawlSpeed()
{
	return m_crawlSpeed;
}
float crRpgCharacterUpdateMethod::getSprintPanSpeed()
{
	return m_sprintPanSpeed;
}
float crRpgCharacterUpdateMethod::getRunPanSpeed()
{
	return m_runPanSpeed;
}
float crRpgCharacterUpdateMethod::getWalkPanSpeed()
{
	return m_walkPanSpeed;
}
float crRpgCharacterUpdateMethod::getCrouchPanSpeed()
{
	return m_crouchPanSpeed;
}
float crRpgCharacterUpdateMethod::getCrawlPanSpeed()
{
	return m_crawlPanSpeed;
}
/////////////////////////////////////////
//
//crFpsCharacterStateChangeMethod
//
/////////////////////////////////////////
crRpgCharacterStateChangeMethod::crRpgCharacterStateChangeMethod():
m_downState(crRpgCharacterUpdateMethod::DS_None),
m_upState(crRpgCharacterUpdateMethod::US_None),
m_downInterval(0.0f),
m_upInterval(0.0f),
m_lastUpdTime(0.0f),
m_updInterval(0.5f)
{
}

crRpgCharacterStateChangeMethod::crRpgCharacterStateChangeMethod(const crRpgCharacterStateChangeMethod& handle):
crMethod(handle),
m_downState(handle.m_downState),
m_upState(handle.m_upState),
m_downInterval(handle.m_downInterval),
m_upInterval(handle.m_upInterval),
m_lastUpdTime(handle.m_lastUpdTime),
m_updInterval(handle.m_updInterval)
{
}

void crRpgCharacterStateChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRpgCharacterStateChangeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_updInterval = atof(str.c_str());
		break;
	case 1:
		if(str.compare("None")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_None;
		else if(str.compare("Stand")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Stand;
		else if(str.compare("Idle")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Idle;
		else if(str.compare("Jump")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Jump;
		else if(str.compare("Falldown")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Falldown;
		else if(str.compare("Sprint")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Sprint;
		else if(str.compare("Run")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Run;
		else if(str.compare("Walk")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Walk;
		else if(str.compare("Crouch")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Crouch;
		else if(str.compare("Crawl")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Crawl;
		else if(str.compare("Dead")==0)
			m_downState = crRpgCharacterUpdateMethod::DS_Dead;
		break;
	case 2:
		m_downInterval = atof(str.c_str());
		break;
	case 3:
		if(str.compare("None")==0)
			m_upState = crRpgCharacterUpdateMethod::US_None;
		else if(str.compare("WeaponFire")==0)
			m_upState = crRpgCharacterUpdateMethod::US_WeaponFire;
		else if(str.compare("ReplaceClip")==0)
			m_upState = crRpgCharacterUpdateMethod::US_ReplaceClip;
		else if(str.compare("WeaponUnSelect")==0)
			m_upState = crRpgCharacterUpdateMethod::US_WeaponUnSelect;
		else if(str.compare("WeaponSelect")==0)
			m_upState = crRpgCharacterUpdateMethod::US_WeaponSelect;
		else if(str.compare("WeaponAim")==0)
			m_upState = crRpgCharacterUpdateMethod::US_WeaponAim;
		break;
	case 4:
		m_upInterval = atof(str.c_str());
		break;
	}
}

void crRpgCharacterStateChangeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	bool cancelTask = true;
	if(!m_characterUpdateMethod.valid())
	{
		m_characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
	}
	if(m_characterUpdateMethod.valid())
	{
		//crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
		//float frameInterval = body->getCurrentFrameInterval();
		////if(m_downState == crFpsCharacterUpdateCallback::DS_Run)
		////	CRCore::notify(CRCore::ALWAYS)<<"DS_Run "<<frametime<< std::endl;
  //      m_lastUpdTime += frameInterval;
		//if(m_lastUpdTime > m_updInterval)
		//{
			if(m_downState != crRpgCharacterUpdateMethod::DS_None)
			{
				if(m_characterUpdateMethod->wantChangeDownState(m_downState,m_downInterval))
				{
					//if(m_downState == crFpsCharacterUpdateCallback::DS_Run)
					//	CRCore::notify(CRCore::ALWAYS)<<"DS_Run1 "<<frametime<< std::endl;
					//m_lastUpdTime = m_updInterval;
					cancelTask = false;
				}
			}
			if(m_upState != crRpgCharacterUpdateMethod::US_None)
			{
				if(m_characterUpdateMethod->wantChangeUpState(m_upState,m_upInterval))
				{
					//m_lastUpdTime = m_updInterval;
					cancelTask = false;
				}
			}
		//}
	}
	if(cancelTask)
	{
		handle.outputParam(0,&cancelTask);
	}
}
/////////////////////////////////////////
//
//crRpgCharacterJumpMethod
//
/////////////////////////////////////////
crRpgCharacterJumpMethod::crRpgCharacterJumpMethod():
m_jumpVel(3.5f),
m_timeInterval(0.8f)
{
}

crRpgCharacterJumpMethod::crRpgCharacterJumpMethod(const crRpgCharacterJumpMethod& handle):
crMethod(handle),
m_jumpVel(handle.m_jumpVel),
m_timeInterval(handle.m_timeInterval)
{
}

void crRpgCharacterJumpMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRpgCharacterJumpMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_jumpVel = atof(str.c_str());
		break;
	case 1:
		m_timeInterval = atof(str.c_str());
		break;
	}
}

void crRpgCharacterJumpMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;

	//CRCore::notify(CRCore::FATAL)<<" crStopActionCallback::operator(): m_actionName = "<<m_actionName<<std::endl;
	//需要修改
	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	if(body)
	{
		body->setLinearVel(crVector3f(0.0f,0.0f,m_jumpVel), m_timeInterval);
		//body->setVelocity2(crVector3f(0.0f,0.0f,m_jumpVel), m_timeInterval);
		//body->setAcceleration2(crVector3f(0.0f,0.0f,m_jumpVel), m_timeInterval);
	}
}
/////////////////////////////////////////
//
//crRpgCharacterMoveChangeMethod
//
/////////////////////////////////////////
crRpgCharacterMoveChangeMethod::crRpgCharacterMoveChangeMethod():
m_move(crRpgCharacterUpdateMethod::M_None),
m_pan(crRpgCharacterUpdateMethod::P_None)
{
}

crRpgCharacterMoveChangeMethod::crRpgCharacterMoveChangeMethod(const crRpgCharacterMoveChangeMethod& handle):
crMethod(handle),
m_move(handle.m_move),
m_pan(handle.m_pan)
{
}

void crRpgCharacterMoveChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 1:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRpgCharacterMoveChangeMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		if(str.compare("None")==0)
			m_move = crRpgCharacterUpdateMethod::M_None;
		else if(str.compare("StopMove")==0)
			m_move = crRpgCharacterUpdateMethod::StopMove;
		else if(str.compare("MoveUp")==0)
			m_move = crRpgCharacterUpdateMethod::MoveUp;
		else if(str.compare("MoveDown")==0)
			m_move = crRpgCharacterUpdateMethod::MoveDown;
		else if(str.compare("RunUp")==0)
			m_move = crRpgCharacterUpdateMethod::RunUp;
		else if(str.compare("RunDown")==0)
			m_move = crRpgCharacterUpdateMethod::RunDown;
		else if(str.compare("SprintUp")==0)
			m_move = crRpgCharacterUpdateMethod::SprintUp;
		else if(str.compare("SprintDown")==0)
			m_move = crRpgCharacterUpdateMethod::SprintDown;
	}
	else if(i==1)
	{
		if(str.compare("None")==0)
			m_pan = crRpgCharacterUpdateMethod::P_None;
		if(str.compare("StopPan")==0)
			m_pan = crRpgCharacterUpdateMethod::StopPan;
		else if(str.compare("MovePanLeft")==0)
			m_pan = crRpgCharacterUpdateMethod::MovePanLeft;
		else if(str.compare("MovePanRight")==0)
			m_pan = crRpgCharacterUpdateMethod::MovePanRight;
		else if(str.compare("RunPanLeft")==0)
			m_pan = crRpgCharacterUpdateMethod::RunPanLeft;
		else if(str.compare("RunPanRight")==0)
			m_pan = crRpgCharacterUpdateMethod::RunPanRight;
		else if(str.compare("SprintPanLeft")==0)
			m_pan = crRpgCharacterUpdateMethod::SprintPanLeft;
		else if(str.compare("SprintPanRight")==0)
			m_pan = crRpgCharacterUpdateMethod::SprintPanRight;
	}
}

void crRpgCharacterMoveChangeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crCharacterMatterObject *body = dynamic_cast<crCharacterMatterObject *>(m_node.get());
	if(body)
	{
		if(!m_characterUpdateMethod.valid())
		{
			m_characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
		}
		if(m_characterUpdateMethod.valid())
		{
			if(m_move != crRpgCharacterUpdateMethod::M_None) 
			{
				m_characterUpdateMethod->wantChangeMove(m_move);
			}
			if(m_pan != crRpgCharacterUpdateMethod::P_None) 
			{
				m_characterUpdateMethod->wantChangePan(m_pan);
			}
		}
	}
}
/////////////////////////////////////////
//
//crDialogTalkMethod
//
/////////////////////////////////////////
crDialogTalkMethod::crDialogTalkMethod()
{
}

crDialogTalkMethod::crDialogTalkMethod(const crDialogTalkMethod& handle):
crMethod(handle),
m_dialogID(handle.m_dialogID),
m_talkString(handle.m_talkString)
{
}

void crDialogTalkMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 1:
	//	m_node = (crNode*)param;
	//	break;
	//	//case 2:
	//	//	m_param = *(_crInt64*)param;
	//	//	break;
	//}
}

void crDialogTalkMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_dialogID = str;
		break;
	case 1:
		m_talkString = str;
		break;
	}
}

void crDialogTalkMethod::operator()(crHandle &handle)
{
	//if(!m_node.valid()) return;
	//CRCore::notify(CRCore::FATAL)<<"crDialogTalkCallback node = "<<node.getName()<<std::endl;
	crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_dialogID);
	if(stage)
	{
		crStaticTextBoxElement *staticTextBox = dynamic_cast<crStaticTextBoxElement *>(stage->getElement("TalkElement"));
		if(staticTextBox)
		{
			staticTextBox->setStringArrayByString(m_talkString);
		}

		CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!bindview || !bindview->isInited())
			return;
		bindview->showCursor("Default");
		cr2DStageManager::getInstance()->showStage(stage,true);
	}
}
/////////////////////////////////////////
//
//crCloseAllDialogMethod
//
/////////////////////////////////////////
crCloseAllDialogMethod::crCloseAllDialogMethod()
{
}

crCloseAllDialogMethod::crCloseAllDialogMethod(const crCloseAllDialogMethod& handle):
crMethod(handle)
{
}

void crCloseAllDialogMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crCloseAllDialogMethod::addParam(int i, const std::string& str)
{
}

void crCloseAllDialogMethod::operator()(crHandle &handle)
{
	crImageStage *stage = NULL;
	if(m_element.valid())
		stage = m_element->getParentStage();
	cr2DStageManager::getInstance()->closeAll(stage);
}

////////////////////////////////
//
//crShowManipulatorDialogsMethod
//
///////////////////////////////////
crShowManipulatorDialogsMethod::crShowManipulatorDialogsMethod()
{}

crShowManipulatorDialogsMethod::crShowManipulatorDialogsMethod(const crShowManipulatorDialogsMethod& handle):
crMethod(handle),
m_dialogIDVec(handle.m_dialogIDVec)
{
}

void crShowManipulatorDialogsMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 1:
	//	m_node = (crNode*)param;
	//	break;
	//	//case 2:
	//	//	m_param = *(_crInt64*)param;
	//	//	break;
	//}
}

void crShowManipulatorDialogsMethod::addParam(int i, const std::string& str)
{
	m_dialogIDVec.push_back(str);
}

void crShowManipulatorDialogsMethod::operator()(crHandle &handle)
{
	cr2DStageManager *stageManager = cr2DStageManager::getInstance();
	for( DialogIDVec::iterator itr = m_dialogIDVec.begin();
		itr != m_dialogIDVec.end();
		++itr )
	{
		stageManager->showStage(*itr,true);
	}
}

////////////////////////////////
//
//crFilterSwitchMethod
//
///////////////////////////////////
crFilterSwitchMethod::crFilterSwitchMethod():
m_needActiveID(0)
{}

crFilterSwitchMethod::crFilterSwitchMethod(const crFilterSwitchMethod& handle):
crMethod(handle),
m_switchName(handle.m_switchName),
m_needActiveID(handle.m_needActiveID)
{
}

void crFilterSwitchMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 3:
		m_needActiveID = *(int*)param;
		break;
	}
}

void crFilterSwitchMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_switchName = str;
		break;
	case 1:
		m_needActiveID = atoi(str.c_str());
		break;
	}
}

void crFilterSwitchMethod::operator()(crHandle &handle)
{
	if(!m_switch.valid())
	{
		//m_switch = dynamic_cast<crMultiSwitch *>(crFilterRenderManager::getInstance()->getFilterNode(m_switchName));
	}
	if(m_switch.valid())
	{
		//CRCore::ScopedLock<CRCore::crMutex> lock(crFilterRenderManager::getInstance()->getNodeMutex());
		m_switch->setActiveSwitchSet(m_needActiveID);
	}
}
////////////////////////////////
//
//crShowFilterNodeMethod
//
///////////////////////////////////
crShowFilterNodeMethod::crShowFilterNodeMethod():
m_visiable(0)
{}

crShowFilterNodeMethod::crShowFilterNodeMethod(const crShowFilterNodeMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_visiable(handle.m_visiable)
{
}

void crShowFilterNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 3:
		m_visiable = *(bool*)param;
		break;
	}
}

void crShowFilterNodeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_visiable = bool(atoi(str.c_str()));
		break;
	}
}

void crShowFilterNodeMethod::operator()(crHandle &handle)
{
	if(!m_filterNode.valid())
	{
		//m_filterNode = crFilterRenderManager::getInstance()->getFilterNode(m_nodeName);
	}
	if(m_filterNode.valid())
	{
		m_filterNode->setVisiable(m_visiable);
	}
}

////////////////////////////////
//
//crNodeSwitchMethod
//
///////////////////////////////////
crNodeSwitchMethod::crNodeSwitchMethod():
m_needActiveID(0)
{}

crNodeSwitchMethod::crNodeSwitchMethod(const crNodeSwitchMethod& handle):
crMethod(handle),
m_switchName(handle.m_switchName),
m_needActiveID(handle.m_needActiveID)
{
}

void crNodeSwitchMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crNodeSwitchMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_switchName = str;
		break;
	case 1:
		m_needActiveID = atoi(str.c_str());
		break;
	}
}

void crNodeSwitchMethod::operator()(crHandle &handle)
{
	if(!m_switch.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_switchName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		m_node->accept(searchByNameVisitor);
		m_switch = dynamic_cast<crMultiSwitch *>(searchByNameVisitor.getResult());
	}
	if(m_switch.valid()) 
	{
		m_switch->setActiveSwitchSet(m_needActiveID);
	}
}
////////////////////////////////
//
//crSwitchActiveMethod
//
///////////////////////////////////
crSwitchActiveMethod::crSwitchActiveMethod():
m_needActiveID(0)
{}

crSwitchActiveMethod::crSwitchActiveMethod(const crSwitchActiveMethod& handle):
crMethod(handle),
m_needActiveID(handle.m_needActiveID)
{
}

void crSwitchActiveMethod::inputParam(int i, void *param)
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
		m_this = (crMultiSwitch*)param;
		break;
	}
}

void crSwitchActiveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_needActiveID = atoi(str.c_str());
		break;
	}
}

void crSwitchActiveMethod::operator()(crHandle &handle)
{
	m_this->setActiveSwitchSet(m_needActiveID);
}
////////////////////////////////
//
//crFilterNodeEventMethod
//
///////////////////////////////////
crFilterNodeEventMethod::crFilterNodeEventMethod():
m_eventMsg(0),
m_eventParam(0)
{}

crFilterNodeEventMethod::crFilterNodeEventMethod(const crFilterNodeEventMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_eventMsg1(handle.m_eventMsg1),
m_eventMsg2(handle.m_eventMsg2),
m_eventMsg(handle.m_eventMsg),
m_eventParam1(handle.m_eventParam1),
m_eventParam2(handle.m_eventParam2),
m_eventParam(handle.m_eventParam),
m_eventNode(handle.m_eventNode)
{
}

void crFilterNodeEventMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 1:
	//	m_node = (crNode*)param;
	//	break;
	//	//case 2:
	//	//	m_param = *(_crInt64*)param;
	//	//	break;
	//}
}

void crFilterNodeEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_eventMsg1 = str;
		break;
	case 2:
		m_eventMsg2 = str;
		break;
	case 3:
		m_eventParam1 = str;
		break;
	case 4:
		m_eventParam2 = str;
		break;
	}
}

void crFilterNodeEventMethod::operator()(crHandle &handle)
{
	if(!m_eventNode.valid())
	{
		//m_eventNode = crFilterRenderManager::getInstance()->getFilterNode(m_nodeName);
	}
	if(m_eventNode.valid())
	{
		if(m_eventMsg == NULL)
		{
			m_eventMsg = MAKEINT64(parseEventMsg(m_eventMsg1),parseEventMsg(m_eventMsg2));
			m_eventParam = MAKEINT64(parseEventMsg(m_eventParam1),parseEventMsg(m_eventParam2));
		}
		m_eventNode->doEvent(m_eventMsg,m_eventParam);
	}
}
////////////////////////////////
//
//crNodeEventMethod
//
///////////////////////////////////
crNodeEventMethod::crNodeEventMethod():
m_eventMsg(0),
m_eventParam(0)
{}

crNodeEventMethod::crNodeEventMethod(const crNodeEventMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_eventMsg1(handle.m_eventMsg1),
m_eventMsg2(handle.m_eventMsg2),
m_eventMsg(handle.m_eventMsg),
m_eventParam1(handle.m_eventParam1),
m_eventParam2(handle.m_eventParam2),
m_eventParam(handle.m_eventParam),
m_eventNode(handle.m_eventNode)
{
}

void crNodeEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crNodeEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_eventMsg1 = str;
		break;
	case 2:
		m_eventMsg2 = str;
		break;
	case 3:
		m_eventParam1 = str;
		break;
	case 4:
		m_eventParam2 = str;
		break;
	}
}

void crNodeEventMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	if(!m_eventNode.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_nodeName);
		searchByNameVisitor.setSearchNodeType(NODE);
		m_node->accept(searchByNameVisitor);
		m_eventNode = searchByNameVisitor.getResult();
	}
	if(m_eventNode.valid())
	{
		if(m_eventMsg == NULL)
		{
			m_eventMsg = MAKEINT64(parseEventMsg(m_eventMsg1),parseEventMsg(m_eventMsg2));
			m_eventParam = MAKEINT64(parseEventMsg(m_eventParam1),parseEventMsg(m_eventParam2));
		}
		m_eventNode->doEvent(m_eventMsg,m_eventParam);
	}
}
//////////////////////////////////
////
////crSetEndCodeMethod
////
/////////////////////////////////////
//crSetEndCodeMethod::crSetEndCodeMethod():
//m_endCode(0)
//{}
//
//crSetEndCodeMethod::crSetEndCodeMethod(const crSetEndCodeMethod& handle):
//crMethod(handle),
//m_endCode(handle.m_endCode)
//{
//}
//
//void crSetEndCodeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_node = NULL;
//		}
//		break;
//	case 1:
//		m_node = (crNode*)param;
//		break;
//		//case 2:
//		//	m_param = *(_crInt64*)param;
//		//	break;
//	}
//}
//
//void crSetEndCodeMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_endCode = atoi(str.c_str());
//		break;
//	}
//}
//
//void crSetEndCodeMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//	crStartHandler::getInstance()->setEndCode(m_endCode);
//}
//////////////////////////////////
////
////crEffectParticleMethod
////
/////////////////////////////////////
//crEffectParticleMethod::crEffectParticleMethod():
//m_windPower(0.0f)
//{}
//
//crEffectParticleMethod::crEffectParticleMethod(const crEffectParticleMethod& handle):
//crMethod(handle),
//m_effectName(handle.m_effectName),
//m_windPower(handle.m_windPower)
//{
//}
//
//void crEffectParticleMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	case 2:
//		if(param) m_param = *(_crInt64*)param;
//		else m_param = 0;
//		break;
//	}
//}
//
//void crEffectParticleMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_effectName = str;
//		break;
//	case 1:
//		m_windPower = atoi(str.c_str());
//		break;
//	}
//}
//
//void crEffectParticleMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//	CRParticle::crParticleEffect* particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
//	if(particleEffectNode)
//	{
//		int currentMsg = m_node->getCurrentMsg();
//		crVector3 wind;
//		switch(currentMsg) 
//		{
//		case WCH_WEAPON_FIRE:
//			{
//				CRPhysics::crWeaponMatterObject *weapon = dynamic_cast<CRPhysics::crWeaponMatterObject *>(m_node.get());
//				if(weapon)
//				{
//					crVector3 *gunpoint = (crVector3*)(LOINT64(m_param));
//					crVector3 *dir = (crVector3*)(HIINT64(m_param));
//					wind = (*dir) * m_windPower;
//					particleEffectNode->setEffectPosition(*gunpoint);
//				}
//			}
//			break;
//		case WCH_BULLET_FLY:
//			{
//				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
//				if(bullet)
//				{
//					particleEffectNode->setEffectPosition(bullet->getBound().center() - bullet->getFireDirect() * bullet->getBulletObject()->getParticleFlyOffset());
//					if(LOINT64(m_param))
//					{
//						crVector3 *linearVel = (crVector3*)(LOINT64(m_param));
//						wind = (*linearVel) * m_windPower;
//					}
//				}
//			}
//			break;
//		case WCH_BULLET_EXPLODE:
//			{
//				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
//				if(bullet)
//				{
//					particleEffectNode->setEffectPosition(bullet->getFireTarget());
//				}
//			}
//			break;
//		}
//
//		particleEffectNode->setEffectDirection(wind);
//		particleEffectNode->startEffect();
//	}
//}
////////////////////////////////
//
//crEffectSequenceMethod
//
///////////////////////////////////
crEffectSequenceMethod::crEffectSequenceMethod():
m_offset(0.0f)
{}

crEffectSequenceMethod::crEffectSequenceMethod(const crEffectSequenceMethod& handle):
crMethod(handle),
m_effectName(handle.m_effectName),
m_offset(handle.m_offset),
m_positionOffset(handle.m_positionOffset)
{
}

void crEffectSequenceMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crEffectSequenceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_effectName = str;
		break;
	case 1:
		m_offset = atof(str.c_str());
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	}
}

void crEffectSequenceMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	CRCore::crSequence* sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
	if(sequenceNode)
	{
		int currentMsg = m_node->getCurrentMsg();

		switch(currentMsg) 
		{
		case WCH_WEAPON_FIRE:
			{
				CRPhysics::crWeaponMatterObject *weapon = dynamic_cast<CRPhysics::crWeaponMatterObject *>(m_node.get());
				if(weapon)
				{
					crVector3 *gunpoint = (crVector3*)(LOINT64(m_param));
					crVector3 *dir = (crVector3*)(HIINT64(m_param));
					//crVector3 pos = *gunpoint + (*dir) * m_offset;

					sequenceNode->setEffectPosition(*gunpoint + (*dir) * m_offset + m_positionOffset);
				}
			}
			break;
		case WCH_BULLET_FLY:
			{
				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
				if(bullet)
				{
					//sequenceNode->setEffectPosition(bullet->getMatterObjectPosition() - bullet->getFireDirect() * bullet->getBulletObject()->getParticleFlyOffset());
					sequenceNode->setEffectPosition(bullet->getBound().center() + bullet->getFireDirect() * m_offset + m_positionOffset);

					//if(LOINT64(param))
					//{
					//	crVector3 *linearVel = (crVector3*)(LOINT64(param));
					//}
				}
			}
			break;
		case WCH_BULLET_EXPLODE:
			{
				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
				if(bullet)
				{
					sequenceNode->setEffectPosition(bullet->getFireTarget() + m_positionOffset);
				}
			}
			break;
		}

		sequenceNode->startEffect();
	}
}
////////////////////////////////
//
//crEffectLightMethod
//
///////////////////////////////////
crEffectLightMethod::crEffectLightMethod()
{}

crEffectLightMethod::crEffectLightMethod(const crEffectLightMethod& handle):
crMethod(handle),
m_effectName(handle.m_effectName)
{
}

void crEffectLightMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crEffectLightMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_effectName = str;
		break;
	}
}

void crEffectLightMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	CRCore::crLightSource* ls = crLightSourceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectName);
	if(ls)
	{
		int currentMsg = m_node->getCurrentMsg();
		switch(currentMsg) 
		{
		case WCH_WEAPON_FIRE:
			{
				CRPhysics::crWeaponMatterObject *weapon = dynamic_cast<CRPhysics::crWeaponMatterObject *>(m_node.get());
				if(weapon)
				{
					crVector3 *gunpoint = (crVector3*)(LOINT64(m_param));
					//crVector3 *dir = (crVector3*)(HIINT64(param));
					//crVector3 pos = *gunpoint + (*dir) * m_offset;
					ls->setEffectPosition(*gunpoint);
				}
			}
			break;
		case WCH_BULLET_FLY:
			{
				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
				if(bullet)
				{
					//sequenceNode->setEffectPosition(bullet->getMatterObjectPosition() - bullet->getFireDirect() * bullet->getBulletObject()->getParticleFlyOffset());

					ls->setEffectPosition(bullet->getBound().center());

					//if(LOINT64(param))
					//{
					//	crVector3 *linearVel = (crVector3*)(LOINT64(param));
					//}
				}
			}
			break;
		case WCH_BULLET_EXPLODE:
			{
				CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
				if(bullet)
				{
					ls->setEffectPosition(bullet->getFireTarget());
				}
			}
			break;
		}

		ls->startEffect();
	}
}
//////////////////////////////////
////
////crEffectSoundMethod
////
/////////////////////////////////////
//crEffectSoundMethod::crEffectSoundMethod()
//{}
//
//crEffectSoundMethod::crEffectSoundMethod(const crEffectSoundMethod& handle):
//crMethod(handle),
//m_effectName(handle.m_effectName)
//{
//}
//
//void crEffectSoundMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_node = NULL;
//		}
//		break;
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	case 2:
//		if(param) m_param = *(_crInt64*)param;
//		else m_param = 0;
//		break;
//	}
//}
//
//void crEffectSoundMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_effectName = str;
//		break;
//	}
//}
//
//void crEffectSoundMethod::operator()(crHandle &handle)
//{
//	if(!m_node.valid()) return;
//	int currentMsg = m_node->getCurrentMsg();
//	CRAL::crSoundState *soundState;
//	switch(currentMsg) 
//	{
//	case WCH_WEAPON_FIRE:
//		{
//			CRPhysics::crWeaponMatterObject *weapon = dynamic_cast<CRPhysics::crWeaponMatterObject *>(m_node.get());
//			if(weapon)
//			{
//				soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(weapon->getFireBody()->getName(),m_effectName);
//				if(soundState)
//				{
//					crVector3 *gunpoint = (crVector3*)(LOINT64(m_param));
//					crVector3 *dir = (crVector3*)(HIINT64(m_param));
//					//crVector3 pos = *gunpoint + (*dir) * m_offset;
//					soundState->setPosition(*gunpoint);
//					soundState->setDirection(*dir);
//				}
//			}
//		}
//		break;
//	case WCH_BULLET_FLY:
//		{
//			CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
//			if(bullet)
//			{
//				soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(bullet->getFireWeapon()->getFireBody()->getName(),m_effectName);
//				if(soundState)
//				{
//					soundState->setPosition(bullet->getBound().center());
//					//if(LOINT64(param))
//					//{
//					//	crVector3 *linearVel = (crVector3*)(LOINT64(param));
//					//}
//				}
//			}
//		}
//		break;
//	case WCH_BULLET_EXPLODE:
//		{
//			CRPhysics::crBulletMatterObject *bullet = dynamic_cast<CRPhysics::crBulletMatterObject *>(m_node.get());
//			if(bullet)
//			{
//				soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(bullet->getFireWeapon()->getFireBody()->getName(),m_effectName);
//				if(soundState)
//				{
//					soundState->setPosition(bullet->getFireTarget());
//				}
//			}
//		}
//		break;
//	}
//
//	if(!CRAL::crSoundManager::instance()->isCulled(soundState))
//		soundState->setPlay(true);
//}
////////////////////////////////
//
//crXTransTMDofMethod
//
///////////////////////////////////
crXTransTMDofMethod::crXTransTMDofMethod():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{}

crXTransTMDofMethod::crXTransTMDofMethod(const crXTransTMDofMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_min(handle.m_min),
m_max(handle.m_max),
m_step(handle.m_step)
{
}

void crXTransTMDofMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crXTransTMDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_min = atof(str.c_str());
		break;
	case 2:
		m_max = atof(str.c_str());
		break;
	case 3:
		m_step = atof(str.c_str());
		break;
	}
}

void crXTransTMDofMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_nodeName);
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		m_node->accept(searchByNameVisitor);
		crNode *resultNode = searchByNameVisitor.getResult();
		if(resultNode)
		{
			//crDofTexMatCallback dofTMCallback;
			crDofTexMatCallback *callback = dynamic_cast<crDofTexMatCallback *>(resultNode->getCullCallback("DofTexMat"));
			if(callback)
			{
				m_dof = callback->getDofTransform();
			}
		}
		if(!m_dof.valid()) 
		{
			CRCore::notify(CRCore::ALWAYS)<<"crXTransTMDofCallback::operator() not find node "<<m_nodeName.c_str()<<std::endl;
			return;
		}
	}

	crVector3f currentTrans = m_dof->getCurrentTranslate();
	crVector3f incrementTrans = m_dof->getIncrementTranslate();

	crVector3f trans = m_dof->getMaxTranslate();
	trans[0] = m_max;
	m_dof->setMaxTranslate(trans);
	trans = m_dof->getMinTranslate();
	trans[0] = m_min;
	m_dof->setMinHPR(trans);

	if(m_step>0)
	{
		if(currentTrans[0] > m_max)
		{
			m_dof->setAnimationOn(false);
			currentTrans[0] = m_max;
			m_dof->setCurrentTranslate(currentTrans);
		}
		else if(currentTrans[0] < m_max)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[0] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
	else
	{
		if(currentTrans[0] < m_min)
		{
			m_dof->setAnimationOn(false);
			currentTrans[0] = m_min;
			m_dof->setCurrentTranslate(currentTrans);
		}
		else if(currentTrans[0] > m_min)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[0] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
}
////////////////////////////////
//
//crYTransTMDofMethod
//
///////////////////////////////////
crYTransTMDofMethod::crYTransTMDofMethod():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{}

crYTransTMDofMethod::crYTransTMDofMethod(const crYTransTMDofMethod& handle):
crMethod(handle),
m_nodeName(handle.m_nodeName),
m_min(handle.m_min),
m_max(handle.m_max),
m_step(handle.m_step)
{
}

void crYTransTMDofMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crYTransTMDofMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeName = str;
		break;
	case 1:
		m_min = atof(str.c_str());
		break;
	case 2:
		m_max = atof(str.c_str());
		break;
	case 3:
		m_step = atof(str.c_str());
		break;
	}
}

void crYTransTMDofMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_nodeName);
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		m_node->accept(searchByNameVisitor);
		crNode *resultNode = searchByNameVisitor.getResult();
		if(resultNode)
		{
			//crDofTexMatCallback dofTMCallback;
			crDofTexMatCallback *callback = dynamic_cast<crDofTexMatCallback *>(resultNode->getCullCallback("DofTexMat"));
			if(callback)
			{
				m_dof = callback->getDofTransform();
			}
		}
		if(!m_dof.valid()) 
		{
			CRCore::notify(CRCore::ALWAYS)<<"crXTransTMDofCallback::operator() not find node "<<m_nodeName.c_str()<<std::endl;
			return;
		}
	}

	crVector3f currentTrans = m_dof->getCurrentTranslate();
	crVector3f incrementTrans = m_dof->getIncrementTranslate();

	crVector3f trans = m_dof->getMaxTranslate();
	trans[1] = m_max;
	m_dof->setMaxTranslate(trans);
	trans = m_dof->getMinTranslate();
	trans[1] = m_min;
	m_dof->setMinHPR(trans);

	if(m_step>0)
	{
		if(currentTrans[1] > m_max)
		{
			m_dof->setAnimationOn(false);
			currentTrans[1] = m_max;
			m_dof->setCurrentTranslate(currentTrans);
		}
		else if(currentTrans[1] < m_max)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[1] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
	else
	{
		if(currentTrans[1] < m_min)
		{
			m_dof->setAnimationOn(false);
			currentTrans[1] = m_min;
			m_dof->setCurrentTranslate(currentTrans);
		}
		else if(currentTrans[1] > m_min)
		{
			m_dof->setAnimationOn(true);
			incrementTrans[1] = m_step;
			m_dof->setIncrementTranslate(incrementTrans);
		}
	}
}
/////////////////////////////////////////
//
//crMoveMethod
//
/////////////////////////////////////////
crMoveMethod::crMoveMethod():
m_velocity(5.0f)
{
}

crMoveMethod::crMoveMethod(const crMoveMethod& handle):
crMethod(handle),
m_velocity(handle.m_velocity)
{
}

void crMoveMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crMoveMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		m_velocity = atof(str.c_str());
	}
}

void crMoveMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
	    body->setVelocity0(m_velocity);
	}
}
/////////////////////////////////////////
//
//crPanMethod
//
/////////////////////////////////////////
crPanMethod::crPanMethod():
m_velocity(5.0f)
{
}

crPanMethod::crPanMethod(const crPanMethod& handle):
crMethod(handle),
m_velocity(handle.m_velocity)
{
}

void crPanMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crPanMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		m_velocity = atof(str.c_str());
	}
}

void crPanMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		body->setVelocity1(m_velocity);
	}
}
/////////////////////////////////////////
//
//crRaiseMethod
//
/////////////////////////////////////////
crRaiseMethod::crRaiseMethod():
m_velocity(5.0f)
{
}

crRaiseMethod::crRaiseMethod(const crRaiseMethod& handle):
crMethod(handle),
m_velocity(handle.m_velocity)
{
}

void crRaiseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRaiseMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		m_velocity = atof(str.c_str());
	}
}

void crRaiseMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		body->setVelocity2(m_velocity);
	}
}
/////////////////////////////////////////
//
//crRotateMethod
//
/////////////////////////////////////////
crRotateMethod::crRotateMethod():
m_h(5.0f)
{
}

crRotateMethod::crRotateMethod(const crRotateMethod& handle):
crMethod(handle),
m_h(handle.m_h)
{
}

void crRotateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crRotateMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		m_h = atof(str.c_str());
	}
}

void crRotateMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		body->setH(m_h);
	}
}
/////////////////////////////////////////
//
//crPointSelectMethod
//
/////////////////////////////////////////
crPointSelectMethod::crPointSelectMethod()
{
}

crPointSelectMethod::crPointSelectMethod(const crPointSelectMethod& handle):
crMethod(handle)
{
}

void crPointSelectMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crPointSelectMethod::addParam(int i, const std::string& str)
{
}

void crPointSelectMethod::operator()(crHandle &handle)
{
	if(!m_ea.valid()) return;
	crGameBodyInfo *bodyInfo = m_this->getGameBodyInfo();
    assert(bodyInfo);

	crData *infoData = bodyInfo->getDataClass();
	assert(infoData);

	void *_selectType;
	infoData->getParam(0,_selectType);
	int selectType = *((int*)_selectType);

	crObject *mousePickObj = NULL;
	crDrawable *mousePickMesh = NULL;
	crGroup *proxyNode = NULL;
	CRCore::crIntersectVisitor::HitList hits;
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;
	bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),hits);
	if(!hits.empty())
	{
		mousePickObj = hits[0].getObject();
		mousePickMesh = hits[0].getDrawable();
		if(!mousePickObj)
			return;
		if( mousePickObj->getName().compare("EditorAxis") == 0 || 
			mousePickObj->getName().compare("ViewGrid") == 0 ||
			mousePickObj->getName().compare("WaypointLink") == 0 )
		{
			bindview->showCursor("NULL");
			bool cancelTask = true;
		    handle.outputParam(0,&cancelTask);
            return;
		}

		if(selectType == -1)
		{//snap material
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(mousePickMesh && !selectNodeVec.empty() && mousePickMesh->getStateSet())
			{	
				if(dynamic_cast<crObject *>(selectNodeVec[0].get()))
				{
					crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
					void *_selectDrawable;
					infoData->getParam(7,_selectDrawable);
					if(_selectDrawable)
					{
						crDrawable *selectDrawable = (crDrawable*)_selectDrawable;
                        selectDrawable->setStateSet(mousePickMesh->getStateSet());
						//currentSelectObject->setStateSet(mousePickObj->getStateSet());
						std::string str = mousePickObj->getDescriptionsInString();
						crArgumentParser::deleteKey(str,"[NameID]");
						currentSelectObject->setDescriptionsInString(str);
						//load object stateset
						char buf[16];
						std::string cfgFile;
						for(int i = 0; i<10; i++)
						{
							sprintf(buf,"cfgFile%d\0",i);
							if(crArgumentParser::readKeyValue(str,buf,cfgFile))
							{
								CREncapsulation::loadCfgFile(cfgFile,currentSelectObject);
							}
						}
					}
				}
				else if(dynamic_cast<crParticleEffect *>(selectNodeVec[0].get()))
				{
					crParticleEffect *particleEffect = dynamic_cast<crParticleEffect *>(selectNodeVec[0].get());
                    crParticleSystem *ps = particleEffect->getParticleSystem();
					ps->setStateSet(mousePickMesh->getStateSet());
					std::string str = mousePickObj->getDescriptionsInString();
					crArgumentParser::deleteKey(str,"[NameID]");
					particleEffect->setDescriptionsInString(str);
                    //load object stateset
					crObject *object = dynamic_cast<crObject*>(ps->getParent(0));
					char buf[16];
					std::string cfgFile;
					for(int i = 0; i<10; i++)
					{
						sprintf(buf,"cfgFile%d\0",i);
						if(crArgumentParser::readKeyValue(str,buf,cfgFile))
						{
							CREncapsulation::loadCfgFile(cfgFile,object);
						}
					}
				}
			}
		}
		else if(selectType == -2)
		{//////////pick child
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(mousePickObj && !selectNodeVec.empty())
			{
				if(dynamic_cast<crParticleEffect *>(selectNodeVec[0].get()))
				{
					crParticleEffect *particleEffect = dynamic_cast<crParticleEffect *>(selectNodeVec[0].get());
					crParticleSystem *ps = particleEffect->getParticleSystem();
					ps->setParticleModelObject(mousePickObj);
					//object stateset cfg
					std::string str = mousePickObj->getDescriptionsInString();
					crArgumentParser::deleteKey(str,"[NameID]");
					particleEffect->setDescriptionsInString(str);
					
					//load object stateset
					crObject *object = dynamic_cast<crObject*>(ps->getParent(0));
					char buf[16];
					std::string cfgFile;
					for(int i = 0; i<10; i++)
					{
						sprintf(buf,"cfgFile%d\0",i);
						if(crArgumentParser::readKeyValue(str,buf,cfgFile))
						{
							CREncapsulation::loadCfgFile(cfgFile,object);
						}
					}
				}
				else if(dynamic_cast<crGroup *>(selectNodeVec[0].get()))
				{
					crGroup *currentSelectGroup = dynamic_cast<crGroup *>(selectNodeVec[0].get());
					//currentSelectGroup->addChild(mousePickObj);
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					mousePickObj->accept(searchByNameVisitor);
					proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						crContainNodeVisitor containVisitor;
						containVisitor.setNode(currentSelectGroup);
						proxyNode->accept(containVisitor);
						if(!containVisitor.getResult())
						{
							crNode *hitNode = proxyNode->getChild(0);
							crNode *selectNode = NULL;
							std::string editorNodeType;
							if(crArgumentParser::readKeyValue(hitNode->getDescriptions(),"EditorNodeType",editorNodeType))
							{
								if(editorNodeType.compare("ParticleModel") == 0)
								{
									std::string editorNodeName;
									crArgumentParser::readKeyValue(hitNode->getDescriptions(),"EditorNodeName",editorNodeName);
									selectNode = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(editorNodeName));
								}
							}
							if(selectNode)
							{
								bodyInfo->unSelectAll();
								bodyInfo->selectNode(selectNode);
							}
							else selectNode = hitNode;
							currentSelectGroup->addChild(selectNode);
						}
					}
					//if(!hits.empty())
					//    proxyNode = dynamic_cast<crGroup *>(hits[0].getHitNodeByName("ProxyNode"));
					//else
					//{
					//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					//	searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					//	searchByNameVisitor.setNameId("ProxyNode");
					//	searchByNameVisitor.setSearchNodeType(ALLNODE);
					//	mousePickObj->accept(searchByNameVisitor);
					//	proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					//}
					//if(proxyNode)
					//{
					//	crNode *selectNode = proxyNode->getChild(0);
     //                   currentSelectGroup->addChild(selectNode);
					//}
				}
				else if(dynamic_cast<crObject *>(selectNodeVec[0].get()))
				{
					crObject *currentSelect = dynamic_cast<crObject *>(selectNodeVec[0].get());
					currentSelect->addDrawable(mousePickMesh);
				}
			}
		}
		else if(selectType == -3)
		{//PickNeedLight
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(mousePickObj && !selectNodeVec.empty())
			{
				crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
				if(currentSelectObject)
				{
					if(!hits.empty())
						proxyNode = dynamic_cast<crGroup *>(hits[0].getHitNodeByName("ProxyNode"));
					else
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						mousePickObj->accept(searchByNameVisitor);
						proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					}
					if(proxyNode)
					{
						crNode *selectNode = proxyNode->getChild(0);
						assert(selectNode);
						std::string currentSelectLight = selectNode->getName();
						crLightSource *pickLight = crLightSourceManager::getInstance()->findLightSource(currentSelectLight);
						if(pickLight)
						{
							currentSelectObject->lock();
							currentSelectObject->addNeedLight(currentSelectLight);
							currentSelectObject->unlock();
						}
					}
				}
			}
		}
		else if(selectType == 0)
		{//PickRoot
			if(!hits.empty())
				proxyNode = dynamic_cast<crGroup *>(hits[0].getHitNodeByName("ProxyNode"));
			else
			{
				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setNameId("ProxyNode");
				searchByNameVisitor.setSearchNodeType(GROUP);
				mousePickObj->accept(searchByNameVisitor);
				proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			}
			if(proxyNode)
			{
				crNode *selectNode = proxyNode->getChild(0);
				if(bodyInfo->isNodeSelected(selectNode))
				{
					if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
					{
						bodyInfo->unSelectNode(selectNode);
					}
					else
					{
						if(!dynamic_cast<crTerrainTile *>(selectNode))
						{
							bindview->showCursor("NULL");
						}
						bool cancelTask = true;
						handle.outputParam(0,&cancelTask);
					}
					return;
				}
				else
				{
					if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
					{
						bodyInfo->unSelectAll();
					}
					if(!dynamic_cast<crTerrainTile *>(selectNode))
					{
						ref_ptr<crData> data = selectNode->getDataClass();
						if(!data.valid())
						{
							data = crDataManager::getInstance()->getData("Event");
							selectNode->setDataClass(data.get());
						}
						if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
						{
							crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
							driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
							data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
							data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
						}
					}
					bodyInfo->selectNode(selectNode);
				}
			}
		}
		else if(selectType == 1 || selectType == 2)
		{//PickObject or PickMesh
			if(selectType == 2 || !(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
			{
				bodyInfo->unSelectAll();
			}
			if(!bodyInfo->isNodeSelected(mousePickObj))
			{
				ref_ptr<crData> data = mousePickObj->getDataClass();
				if(!data.valid())
				{
					data = crDataManager::getInstance()->getData("Event");
					mousePickObj->setDataClass(data.get());
				}
				if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
				{
					crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
					driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
					data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
					data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
				}
				bodyInfo->selectNode(mousePickObj);

				if(selectType == 2)
				{//set currentSelectDrawable
					infoData->inputParam(7,mousePickMesh);
				}
			}
			else
			{
				if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
				{
					bodyInfo->unSelectNode(mousePickObj);
				}
			}
		}
		else if(selectType == 21)
		{//PickEditorNode
			if(!hits.empty())
				proxyNode = dynamic_cast<crGroup *>(hits[0].getHitNodeByName("ProxyNode"));
			else
			{
				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setNameId("ProxyNode");
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				mousePickObj->accept(searchByNameVisitor);
				proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			}
			if(proxyNode)
			{
				crNode *hitNode = proxyNode->getChild(0);
				crNode *selectNode = NULL;
				std::string editorNodeType;
				if(crArgumentParser::readKeyValue(hitNode->getDescriptions(),"EditorNodeType",editorNodeType))
				{
					if(editorNodeType.compare("ParticleModel") == 0)
					{
						std::string editorNodeName;
						crArgumentParser::readKeyValue(hitNode->getDescriptions(),"EditorNodeName",editorNodeName);
						selectNode = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(editorNodeName));
					}
				}
				if(selectNode)
				{
					bodyInfo->unSelectAll();
					bodyInfo->selectNode(selectNode);
				}
			}
		}
		else if(selectType == 22)
		{//Waypoint
			crNode *selectNode = NULL;
			if(!hits.empty())
				selectNode = hits[0].getHitNodeByName("#Waypoint");
			else
			{
				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setNameId("#Waypoint");
				searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
				mousePickObj->accept(searchByNameVisitor);
				selectNode = searchByNameVisitor.getResult();
			}
			if(selectNode)
			{
				if(bodyInfo->isNodeSelected(selectNode))
				{
					if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
					{
						bodyInfo->unSelectNode(selectNode);
					}
					else
					{
						if(!dynamic_cast<crTerrainTile *>(selectNode))
						{
							bindview->showCursor("NULL");
						}
						bool cancelTask = true;
						handle.outputParam(0,&cancelTask);
					}
					return;
				}
				else
				{
					if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
					{
						bodyInfo->unSelectAll();
					}
					if(!dynamic_cast<crTerrainTile *>(selectNode))
					{
						ref_ptr<crData> data = selectNode->getDataClass();
						if(!data.valid())
						{
							data = crDataManager::getInstance()->getData("Event");
							selectNode->setDataClass(data.get());
						}
						if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
						{
							crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
							driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
							data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
							data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
						}
					}
					bodyInfo->selectNode(selectNode);
				}
			}
		}
		else if(selectType > 2)
		{//CreBodyNode
			crNode *selectNode = NULL;
			void *_classname;
			infoData->getParam(6,_classname);
			std::string* classname = (std::string *)_classname;

			if(!hits.empty())
				selectNode = hits[0].getHitNodeByClassName(*classname);
			else
			{
				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByClassNameVisitor.insertClassNameId(*classname);
				mousePickObj->accept(searchByClassNameVisitor);
				selectNode = searchByClassNameVisitor.getResult();
			}
			if(selectNode)
			{
				if(bodyInfo->isNodeSelected(selectNode))
				{
					if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
					{
						bodyInfo->unSelectNode(selectNode);
					}
					else
					{
						if(!dynamic_cast<crTerrainTile *>(selectNode))
						{
							bindview->showCursor("NULL");
						}
						bool cancelTask = true;
						handle.outputParam(0,&cancelTask);
					}
					return;
				}
				else
				{
					if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
					{
						bodyInfo->unSelectAll();
					}
					if(!dynamic_cast<crTerrainTile *>(selectNode))
					{
						ref_ptr<crData> data = selectNode->getDataClass();
						if(!data.valid())
						{
							data = crDataManager::getInstance()->getData("Event");
							selectNode->setDataClass(data.get());
						}
						if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
						{
							crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
							driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
							data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
							data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
						}
					}
					bodyInfo->selectNode(selectNode);
				}
			}
		}
	}
	else if(selectType >= 0)
	{
        bodyInfo->unSelectAll();
	}
}
/////////////////////////////////////////
//
//crShowCursorMethod
//
/////////////////////////////////////////
crShowCursorMethod::crShowCursorMethod()
{
}

crShowCursorMethod::crShowCursorMethod(const crShowCursorMethod& handle):
crMethod(handle),
m_cursorName(handle.m_cursorName)
{
}

void crShowCursorMethod::inputParam(int i, void *param)
{
}

void crShowCursorMethod::addParam(int i, const std::string& str)
{
	if(i == 0) m_cursorName = str;
}

void crShowCursorMethod::operator()(crHandle &handle)
{
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(bindview && bindview->isInited()) bindview->showCursor(m_cursorName);
}
/////////////////////////////////////////
//
//crMouseControlViewMethod
//
/////////////////////////////////////////
crMouseControlViewMethod::crMouseControlViewMethod():
m_rotMode(XYRot),
m_time(0.0f)
{
}

crMouseControlViewMethod::crMouseControlViewMethod(const crMouseControlViewMethod& handle):
crMethod(handle),
m_rotMode(handle.m_rotMode),
m_time(0.0f)
{
}

void crMouseControlViewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crMouseControlViewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("XRot") == 0)
		{
			m_rotMode = XRot;
		}
		else if(str.compare("YRot") == 0)
		{
			m_rotMode = YRot;
		}
		else if(str.compare("XYRot") == 0)
		{
			m_rotMode = XYRot;
		}
		break;
	}
}
void crMouseControlViewMethod::operator()(crHandle &handle)
{
	if(m_ea.valid() && m_this == crSceneManager::getInstance()->getPlayer())
	{
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited() && !bindview->isCursorOnShow())
		{
			crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
			float dt = m_ea->time() - m_time;
			if(dt>0.5f)
			{
				m_time = m_ea->time();
				return;
			}
			//if(dt>0.05) dt = 0.05;
			float x = 0.0f;
			float y = 0.0f;
			float mouseSpeed = crDisplaySettings::instance()->getMouseSpeed();
			if(m_rotMode & XRot)
				x = -inDegrees(mouse[0]*200.0f*mouseSpeed/*dt*/);
			if(m_rotMode & YRot)
				y = -inDegrees(mouse[1]*100.0f*mouseSpeed/*dt*/);
			//CRCore::notify(CRCore::ALWAYS)<<"m_mouse_speed = "<<m_mouse_speed<< std::endl;
			//CRCore::notify(CRCore::ALWAYS)<<"x = "<<mouse[0]<<"y = "<<mouse[1]<<"dt = "<<dt<< std::endl;
			//if(fabs(y)<0.01f) y = 0.0; 
			m_time = m_ea->time();

			m_turn_angle.set(x,y);
			m_this->rotate(m_turn_angle);
		}
	}
}
/////////////////////////////////////////
//
//crSetEditorCoordinateMethod
//
/////////////////////////////////////////
crSetEditorCoordinateMethod::crSetEditorCoordinateMethod()
{
}

crSetEditorCoordinateMethod::crSetEditorCoordinateMethod(const crSetEditorCoordinateMethod& handle):
crMethod(handle)
{
}

void crSetEditorCoordinateMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 1:
	//	m_node = (crNode*)param;
	//	break;
	////case 2:
	////	m_param = *(_crInt64*)param;
	////	break;
	//}
}

void crSetEditorCoordinateMethod::addParam(int i, const std::string& str)
{
}

void crSetEditorCoordinateMethod::operator()(crHandle &handle)
{
	//if(!m_node.valid()) return;
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(!body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(!bodyInfo);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
    if(!selectNodeVec.empty())
	{
		crData *infoData = bodyInfo->getDataClass();
		assert(!infoData);

		crNode *selectNode = selectNodeVec[0].get();
		if(!m_editorCoordinate.valid())
		{
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("EditorAxis");
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			crBrain::getInstance()->getCurrentActiveDB()->accept(searchByNameVisitor);
			m_editorCoordinate = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
		}
		if(m_editorCoordinate.valid())
		{
			void *_canmove;
			infoData->getParam(8,_canmove);
			int canmove = *((int*)_canmove);
		    if(canmove)
			{//PickRoot
				crMatrix matrix;
				if(dynamic_cast<crMatrixTransform *>(selectNode))
					matrix.setTrans(dynamic_cast<crMatrixTransform *>(selectNode)->getTrans());
				else
				    matrix.setTrans(selectNode->getBound().center());
				m_editorCoordinate->setMatrix(matrix);
			}
		}
	}
}
/////////////////////////////////////////
//
//crConvertToTransformMethod
//
/////////////////////////////////////////
crConvertToTransformMethod::crConvertToTransformMethod()
{
}

crConvertToTransformMethod::crConvertToTransformMethod(const crConvertToTransformMethod& handle):
crMethod(handle)
{
}

void crConvertToTransformMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
		//case 2:
		//	m_param = *(_crInt64*)param;
		//	break;
	}
}

void crConvertToTransformMethod::addParam(int i, const std::string& str)
{
}

void crConvertToTransformMethod::operator()(crHandle &handle)
{
	if(!m_node.valid() || dynamic_cast<crMatrixTransform *>(m_node.get())) return;
	crGroup *group = dynamic_cast<crGroup *>(m_node.get());
	crMatrixTransform *child = NULL;
    if(group)
	{
		if(group->getNumChildren() == 1)
		{
            child = dynamic_cast<crMatrixTransform *>(group->getChild(0));
		}
	}
	if(child)
	{
		handle.outputParam(1,child);
	}
	else
	{
		CRCore::ref_ptr<crGroup> parent = m_node->getParent(0);
		ref_ptr<crMatrixTransform> nodeTransform = new crMatrixTransform;
		//if(!m_node->getBoundSetted() && m_node->getName().compare("CreBodyRoot") != 0 && typeid(*m_node)==typeid(CRCore::crGroup))
		//{
		//	nodeTransform->setName("tempGroupM");
		//	parent->replaceChildExt(m_node.get(),nodeTransform.get());
		//}
		//else
		//{
			nodeTransform->setName("TempM");
			parent->removeChild(m_node.get());
			nodeTransform->addChild(m_node.get());
			parent->addChild(nodeTransform.get());
		//}
		handle.outputParam(1,nodeTransform.get());
	}
}

/////////////////////////////////////////
//
//crPanNodeMethod
//
/////////////////////////////////////////
crPanNodeMethod::crPanNodeMethod():
m_keyboard(0)
{
}

crPanNodeMethod::crPanNodeMethod(const crPanNodeMethod& handle):
crMethod(handle),
m_keyboard(handle.m_keyboard)
{
}

void crPanNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crPanNodeMethod::addParam(int i, const std::string& str)
{
	if(i==0) m_keyboard = atoi(str.c_str());
}

void crPanNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);
		crData *infoData = bodyInfo->getDataClass();
		assert(!infoData);
        if(!bodyInfo->getSelectNodeVec().empty())
		{
			crVector3 trans;
			void *_panNodeMode;
			infoData->getParam(2,_panNodeMode);
			int panNodeMode = *((int*)_panNodeMode);

			void *_speed;
			infoData->getParam(EDP_TransformNodeSpeed,_speed);
			float speed = *((float*)_speed);
			if(speed>1.0f)
			{
				speed = 1.0f;
				infoData->inputParam(EDP_TransformNodeSpeed,&speed);
			}
			if(m_keyboard==0)
			{
				CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
				if(bindview && bindview->isInited())
				{
					CRProducer::crGUIEventAdapter* ea = (crGUIEventAdapter *)(LOINT64(m_param));

					float x = inDegrees(ea->getXnormalized()*200.0f*speed);
					float y = inDegrees(ea->getYnormalized()*100.0f*speed);
					crMatrix viewerMat = bindview->getViewMatrix();
					viewerMat.setTrans(0,0,0);

					switch (panNodeMode)
					{
					case 0://xy
						trans.set(x,0.0f,-y);
						trans = viewerMat * trans;
						trans[2] = 0.0f;
						break;
					case 1://x
						trans.set(x,0.0f,-y);
						trans = viewerMat * trans;
						trans[1] = 0.0f;
						trans[2] = 0.0f;
						break;
					case 2://y
						trans.set(x,0.0f,-y);
						trans = viewerMat * trans;
						trans[0] = 0.0f;
						trans[2] = 0.0f;
						break;
					case 3://z
						trans.set(0.0f,0.0f,y);
						break;
					}
				}
			}
			else
			{
				speed *= m_keyboard;
				switch (panNodeMode)
				{
				case 0://xy
					trans[0] = speed;
				    trans[1] = -speed;
					break;
				case 1://x
					trans[0] = speed;
					break;
				case 2://y
					trans[1] = -speed;
					break;
				case 3://z
					trans[2] = speed;
					break;
				}
			}
			bodyInfo->translateSelectedNodes(crMatrix::translate(trans));
		}
	}
}
/////////////////////////////////////////
//
//crRotNodeMethod
//
/////////////////////////////////////////
crRotNodeMethod::crRotNodeMethod():
m_keyboard(0)
{
}

crRotNodeMethod::crRotNodeMethod(const crRotNodeMethod& handle):
crMethod(handle),
m_keyboard(handle.m_keyboard)
{
}

void crRotNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crRotNodeMethod::addParam(int i, const std::string& str)
{
	if(i==0) m_keyboard = atoi(str.c_str());
}

void crRotNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);
		crData *infoData = bodyInfo->getDataClass();
		assert(!infoData);
		if(!bodyInfo->getSelectNodeVec().empty())
		{
			void *_rotNodeMode;
			infoData->getParam(3,_rotNodeMode);
			int rotNodeMode = *((int*)_rotNodeMode);

			void *_speed;
			infoData->getParam(5,_speed);
			float speed = *((float*)_speed);

			if(m_keyboard==0)
			{
				CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
				if(bindview && bindview->isInited())
				{
					CRProducer::crGUIEventAdapter* ea = (crGUIEventAdapter *)(LOINT64(m_param));

					float x = inDegrees(ea->getXnormalized()*200.0f*speed);
					float y = inDegrees(ea->getYnormalized()*100.0f*speed);
					x+=y;
					switch (rotNodeMode)
					{
					case 0://x
						if(x != 0.0f) bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(x, X_AXIS));
						break;
					case 1://y
						if(x != 0.0f) bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(x, Y_AXIS));
						break;
					case 2://x
						if(x != 0.0f) bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(x, Z_AXIS));
						break;
					}
				}
			}
			else
			{
				//speed = CRCore::PI_4;
				speed = CRCore::DegreesToRadians(float((int)speed * m_keyboard));
				if(speed!=0)
				{
					switch (rotNodeMode)
					{
					case 0://x
						bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(speed, X_AXIS));
						break;
					case 1://y
						bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(speed, Y_AXIS));
						break;
					case 2://x
						bodyInfo->translateSelectedNodes(CRCore::crMatrix::rotate(speed, Z_AXIS));
						break;
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crScaleNodeMethod
//
/////////////////////////////////////////
crScaleNodeMethod::crScaleNodeMethod()
{
}

crScaleNodeMethod::crScaleNodeMethod(const crScaleNodeMethod& handle):
crMethod(handle),
m_scaleAllf(handle.m_scaleAllf),
m_scaleXf(handle.m_scaleXf),
m_scaleYf(handle.m_scaleYf),
m_scaleZf(handle.m_scaleZf),
m_stageNames(handle.m_stageNames)
{
}

void crScaleNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	//case 2:
	//	m_param = *(_crInt64*)param;
	//	break;
	}
}

void crScaleNodeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_scaleAllf = str;
		break;
	case 1:
		m_scaleXf = str;
		break;
	case 2:
		m_scaleYf = str;
		break;
	case 3:
		m_scaleZf = str;
		break;
	default:
		m_stageNames.push_back(str);
		break;
	}
}

void crScaleNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);
		crData *infoData = bodyInfo->getDataClass();
		assert(!infoData);
		if(!bodyInfo->getSelectNodeVec().empty())
		{
			crImageStage *stage;
			for( std::vector<std::string>::iterator itr = m_stageNames.begin();
				 itr != m_stageNames.end();
				 ++itr )
			{
				stage = cr2DStageManager::getInstance()->findStage(*itr);
				if(stage && stage->getShow())
				{
					void *_scaleNodeMode;
					infoData->getParam(4,_scaleNodeMode);
					int scaleNodeMode = *((int*)_scaleNodeMode);

					float scaleValue = 1.0f;
					crEditBoxElement *editbox;
					switch (scaleNodeMode)
					{
					case 0://ScaleAll
						editbox = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleAllf));
						if(editbox)
						{
							scaleValue = atof(editbox->getStringArrayInString().c_str());
							editbox->setStringArrayByString("1.0");
						}
						if(scaleValue > 0.0f) bodyInfo->scaleSelectedNodes(CRCore::crMatrix::scale(scaleValue,scaleValue,scaleValue));
						break;
					case 1://ScaleX
						editbox = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleXf));
						if(editbox)
						{
							scaleValue = atof(editbox->getStringArrayInString().c_str());
							editbox->setStringArrayByString("1.0");
						}
						if(scaleValue > 0.0f) bodyInfo->scaleSelectedNodes(CRCore::crMatrix::scale(scaleValue,1.0f,1.0f));
						break;
					case 2://ScaleY
						editbox = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleYf));
						if(editbox)
						{
							scaleValue = atof(editbox->getStringArrayInString().c_str());
							editbox->setStringArrayByString("1.0");
						}
						if(scaleValue > 0.0f) bodyInfo->scaleSelectedNodes(CRCore::crMatrix::scale(1.0f,scaleValue,1.0f));
						break;
					case 3://ScaleZ
						editbox = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleZf));
						if(editbox)
						{
							scaleValue = atof(editbox->getStringArrayInString().c_str());
							editbox->setStringArrayByString("1.0");
						}
						if(scaleValue > 0.0f) bodyInfo->scaleSelectedNodes(CRCore::crMatrix::scale(1.0f,1.0f,scaleValue));
						break;
					}
					break;
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSpawnNodeMethod
//
/////////////////////////////////////////
crSpawnNodeMethod::crSpawnNodeMethod()
{
}

crSpawnNodeMethod::crSpawnNodeMethod(const crSpawnNodeMethod& handle):
crMethod(handle)
{
}

void crSpawnNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_parent = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 3:
		m_parent = (crGroup*)param;
		break;
	}
}

void crSpawnNodeMethod::addParam(int i, const std::string& str)
{
}

void crSpawnNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid() || !m_parent.valid()) return;
    
	CRIOManager::crLoadManager::getInstance()->requestAddNode(m_parent.get(),dynamic_cast<crNode *>(m_node->clone(crCopyOp::DEEP_COPY_NODES)),false);
}
/////////////////////////////////////////
//
//crDeleteNodeMethod
//
/////////////////////////////////////////
crDeleteNodeMethod::crDeleteNodeMethod()
{
}

crDeleteNodeMethod::crDeleteNodeMethod(const crDeleteNodeMethod& handle):
crMethod(handle)
{
}

void crDeleteNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_parent = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 3:
		m_parent = (crGroup*)param;
		break;
	}
}

void crDeleteNodeMethod::addParam(int i, const std::string& str)
{
}

void crDeleteNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid() || !m_parent.valid()) return;

	CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_parent.get(),m_node.get());
}

/////////////////////////////////////////
//
//crAdjustEditorNodeMethod
//
/////////////////////////////////////////
crAdjustEditorNodeMethod::crAdjustEditorNodeMethod()
{
}

crAdjustEditorNodeMethod::crAdjustEditorNodeMethod(const crAdjustEditorNodeMethod& handle):
crMethod(handle)
{
}

void crAdjustEditorNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_effectNode = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		m_effectNode = (crNode*)param;
		break;
	}
}

void crAdjustEditorNodeMethod::addParam(int i, const std::string& str)
{
}

void crAdjustEditorNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()||!dynamic_cast<crMatrixTransform *>(m_node.get())) return;
	
	crMatrixTransform *editorNode = dynamic_cast<crMatrixTransform *>(m_node.get());
    assert(!editorNode);
	std::string editorNodeType;
	if(crArgumentParser::readKeyValue(editorNode->getDescriptions(),"EditorNodeType",editorNodeType))
	{
		std::string editorNodeName;
	    crArgumentParser::readKeyValue(editorNode->getDescriptions(),"EditorNodeName",editorNodeName);
		if(editorNodeType.compare("LightModel") == 0)
		{
			crLightSource *ls = crLightSourceManager::getInstance()->findLightSource(editorNodeName);
			assert(!ls);
			crLight *light = dynamic_cast<crLight *>(ls->getLight());
			assert(!light);

			const crMatrix& matrix = editorNode->getMatrix();
			crMatrix rotMat = matrix;
			rotMat.setTrans(0.0f,0.0f,0.0f);
            crVector3 dir = (Y_AXIS * rotMat).normalize();
			light->setDirection(dir);
			crVector3 position = matrix.getTrans();
			light->setPosition(crVector4(position,1.0f));

			ls->lock();
			ls->getLightParamStruct()->m_lsPosition = position;
			ls->getLightParamStruct()->m_lsDirection = dir;
			ls->unlock();
		}
		else if(editorNodeType.compare("ParticleModel") == 0)
		{
			crMatrixTransform *effectGroup = dynamic_cast<crMatrixTransform *>(m_effectNode.get());
			if(!effectGroup)
			{
				effectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(editorNodeName));
			}
			if(effectGroup)
			{
				effectGroup->setMatrix(editorNode->getMatrix());
			}
		}
	}
}
/////////////////////////////////////////
//
//crFocusNodeMethod
//
/////////////////////////////////////////
crFocusNodeMethod::crFocusNodeMethod()
{
}

crFocusNodeMethod::crFocusNodeMethod(const crFocusNodeMethod& handle):
crMethod(handle)
{
}

void crFocusNodeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crFocusNodeMethod::addParam(int i, const std::string& str)
{
}

void crFocusNodeMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
	searchByNameVisitor.setNameId("ProxyNode");
	searchByNameVisitor.setSearchNodeType(ALLNODE);
	m_node->accept(searchByNameVisitor);
	crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(proxyNode)
	{
		m_node = proxyNode;
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crVector3 pos = m_node->getBound().center();
		float viewHeight = body->getViewHeight();
		pos[2] -= viewHeight * 0.5f;
		body->resetView();
		body->setMatterObjectMatrix(crMatrix::translate(pos+Y_AXIS),CRPhysics::crMatterObject::MD_FullMatrix);
	}
}
/////////////////////////////////////////
//
//crSelectNodeMaterialMethod
//
/////////////////////////////////////////
crSelectNodeMaterialMethod::crSelectNodeMaterialMethod()
{
	//m_selectStateSet = new crStateSet;

	////crMaterial *selectMaterial = new crMaterial;
	////selectMaterial->setColorMode(crMaterial::OFF);
	////selectMaterial->setAmbient(crMaterial::FRONT_AND_BACK,crVector4(0.0,0.0,0.5,1.0));
	////selectMaterial->setSpecular(crMaterial::FRONT_AND_BACK,crVector4(0.0,0.0,0.5,1.0));
	////selectMaterial->setDiffuse(crMaterial::FRONT_AND_BACK,crVector4(0.0,0.0,0.5,1.0));
	////selectMaterial->setEmission(crMaterial::FRONT_AND_BACK,crVector4(0.0,0.0,0.5,1.0));
	////selectMaterial->setShininess(crMaterial::FRONT_AND_BACK,8.0f);
	////selectMaterial->setDrawCallback(crOglMaterialDraw::getInstance());
	////m_selectStateSet->setAttributeAndModes(selectMaterial,CRCore::crStateAttribute::OVERRIDE|CRCore::crStateAttribute::ON);
	//m_lightModel = new CRCore::crLightModel;
	//m_lightModel->setAmbientIntensity(CRCore::crVector4(0.0f,0.0f,1.0f,1.0f));
	//m_lightModel->setColorControl(CRCore::crLightModel::SEPARATE_SPECULAR_COLOR);
	//m_lightModel->setLocalViewer(true);
	//m_selectStateSet->setAttributeAndModes(m_lightModel.get(), CRCore::crStateAttribute::ON);
}

crSelectNodeMaterialMethod::crSelectNodeMaterialMethod(const crSelectNodeMaterialMethod& handle):
crMethod(handle)/*,
m_selectStateSet(handle.m_selectStateSet),
m_lightModel(handle.m_lightModel)*/
{
}

void crSelectNodeMaterialMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crSelectNodeMaterialMethod::addParam(int i, const std::string& str)
{
}

void crSelectNodeMaterialMethod::operator()(crHandle &handle)
{
	if(!m_node.valid()) return;
	int currentMsg = m_node->getCurrentMsg();
	crMatterObject *matterObject = dynamic_cast<crMatterObject *>(m_node.get());
	if(currentMsg == WCH_SELECTNODE) 
	{
		//m_nodeStateSet = m_node->getStateSet();
		//if(m_nodeStateSet.valid())
		//{
		//	m_nodeStateSet->setAttributeAndModes(m_lightModel.get(), CRCore::crStateAttribute::ON);
		//}
		//else
		//{
		//	m_node->setStateSet(m_selectStateSet.get());
		//}
		if(matterObject) 
		{
			matterObject->setEnableIntersect(false);
			crVector4f vec4;
			crVector3f vec3;
			crMatrixf mat;
			if(CRCore::crArgumentParser::readKeyValue(matterObject->getDescriptions(),"GeomRotation",vec4))
			{
				crQuat q(vec4);
				mat.set(q);
			}
			if(CRCore::crArgumentParser::readKeyValue(matterObject->getDescriptions(),"GeomPosition",vec3))
			{
				mat.setTrans(vec3);
			}
			matterObject->setMatterObjectMatrix(mat,CRPhysics::crMatterObject::MD_FullMatrix);
		}
		crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Edit);
		m_node->accept(selectNodeVisitor);
	}
	else if(currentMsg == WCH_UNSELECTNODE)
	{
		//m_selectStateSet->clear();
		//m_selectStateSet->setAttributeAndModes(m_lightModel.get(), CRCore::crStateAttribute::ON);
  //      if(m_nodeStateSet.valid())
		//{
		//	m_nodeStateSet->removeAttribute(m_lightModel.get());
		//	m_nodeStateSet = NULL;
		//}
		//else m_node->setStateSet(NULL);
		if(matterObject) matterObject->setEnableIntersect(true);
		crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
		m_node->accept(selectNodeVisitor);
	}
}

/////////////////////////////////////////
//
//crUnHiddenNodeVisitor
//
/////////////////////////////////////////
void crUnHiddenNodeVisitor::apply(crNode& node)
{
	if(m_exceptNodeNames.empty() || m_exceptNodeNames.find(node.getName())==std::string::npos)
	{
        node.setEditorHidden(false);
    	traverse(node);
	}
}
/////////////////////////////////////////
//
//crHiddenNodeVisitor
//
/////////////////////////////////////////
void crHiddenNodeVisitor::apply(crNode& node)
{
	if(m_unHideNodeSet.find(&node) == m_unHideNodeSet.end())
	{
		traverse(node);
	}
}
void crHiddenNodeVisitor::apply(CRCore::crObject& node)
{
	if(m_unHideNodeSet.find(&node) == m_unHideNodeSet.end())
	{
		node.setEditorHidden(true);
		traverse(node);
	}
}
void crHiddenNodeVisitor::pushUnHideNodeSet(CRCore::crNode *node)
{
	m_unHideNodeSet.insert(node);
}
void crHiddenNodeVisitor::reset()
{
	m_unHideNodeSet.clear();
}
/////////////////////////////////////////
//
//crHiddenOrUnHiddenNodesMethod
//
/////////////////////////////////////////
crHiddenOrUnHiddenNodesMethod::crHiddenOrUnHiddenNodesMethod():
m_hidden(true)
{
}

crHiddenOrUnHiddenNodesMethod::crHiddenOrUnHiddenNodesMethod(const crHiddenOrUnHiddenNodesMethod& handle):
crMethod(handle),
m_hidden(handle.m_hidden)
{
}

void crHiddenOrUnHiddenNodesMethod::inputParam(int i, void *param)
{
}

void crHiddenOrUnHiddenNodesMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_hidden = atoi(str.c_str());
		break;
	}
}

void crHiddenOrUnHiddenNodesMethod::operator()(crHandle &handle)
{
	switch (m_hidden)
	{
	case 0:
		{//UnHiddenAll
			std::string str;
			crUnHiddenNodeVisitor unHiddenNode(str);
			crBrain::getInstance()->accept(unHiddenNode);
		}
		break;
	case 1:
		{//1表示隐藏当前选中的
			crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
			assert(body);
			crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
			assert(bodyInfo);
			crData *infoData = bodyInfo->getDataClass();
			assert(infoData);

			crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				bodyInfo->unSelectAll();

				for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
					itr != selectNodeVec.end();
					++itr )
				{
					(*itr)->setEditorHidden(true);
				}
			}
		}
		break;
	case 2:
		{//1表示隐藏未选中的
			crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
			assert(body);
			crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
			assert(bodyInfo);
			crData *infoData = bodyInfo->getDataClass();
			assert(infoData);

			crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crHiddenNodeVisitor hiddenNode;
				for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
					itr != selectNodeVec.end();
					++itr )
				{
					hiddenNode.pushUnHideNodeSet((*itr).get());
				}
				crBrain::getInstance()->accept(hiddenNode);
			}
		}
		break;
	}
}
/////////////////////////////////////////
//
//crHideNodeMethod
//
/////////////////////////////////////////
crHideNodeMethod::crHideNodeMethod()
{
}

crHideNodeMethod::crHideNodeMethod(const crHideNodeMethod& handle):
crMethod(handle)
{
}

void crHideNodeMethod::inputParam(int i, void *param)
{
}

void crHideNodeMethod::addParam(int i, const std::string& str)
{
}

void crHideNodeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	crData *infoData = bodyInfo->getDataClass();
	assert(infoData);

	crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		bodyInfo->unSelectAll();

		for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
			itr != selectNodeVec.end();
			++itr )
		{
			(*itr)->setEditorHidden(true);
		}
	}
}
/////////////////////////////////////////
//
//crUnHideAllMethod
//
/////////////////////////////////////////
crUnHideAllMethod::crUnHideAllMethod()
{
}

crUnHideAllMethod::crUnHideAllMethod(const crUnHideAllMethod& handle):
crMethod(handle),
m_exceptNodeNames(handle.m_exceptNodeNames)
{
}

void crUnHideAllMethod::inputParam(int i, void *param)
{
}

void crUnHideAllMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_exceptNodeNames = str;
		break;
	}
}

void crUnHideAllMethod::operator()(crHandle &handle)
{
	crUnHiddenNodeVisitor unHiddenNode(m_exceptNodeNames);
	crBrain::getInstance()->accept(unHiddenNode);
}
/////////////////////////////////////////
//
//crShowAllUnRenderableNodeVisitor
//
/////////////////////////////////////////
void crShowAllUnRenderableNodeVisitor::apply(crNode& node)
{
	if(m_exceptNodeNames.empty() || m_exceptNodeNames.find(node.getName())==std::string::npos)
	{
		if(!node.getRenderable2())
			node.setEditorHidden(!m_show);
		traverse(node);
	}
}
/////////////////////////////////////////
//
//crShowAllUnRenderableNodeMethod
//
/////////////////////////////////////////
crShowAllUnRenderableNodeMethod::crShowAllUnRenderableNodeMethod()
{
}

crShowAllUnRenderableNodeMethod::crShowAllUnRenderableNodeMethod(const crShowAllUnRenderableNodeMethod& handle):
crMethod(handle),
m_exceptNodeNames(handle.m_exceptNodeNames)
{
}

void crShowAllUnRenderableNodeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crShowAllUnRenderableNodeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_exceptNodeNames = str;
		break;
	}
}

void crShowAllUnRenderableNodeMethod::operator()(crHandle &handle)
{
	crShowAllUnRenderableNodeVisitor showNodeVisitor(!m_checkBox->getSelect(),m_exceptNodeNames);
	crBrain::getInstance()->accept(showNodeVisitor);
	//crFilterRenderManager::getInstance()->getFilterNode("Filter")->setEditorHidden(m_checkBox->getSelect());
}
///////////////////////////////////////////
////
////crLightListSelectMethod
////
///////////////////////////////////////////
//crLightListSelectMethod::crLightListSelectMethod()
//{
//}
//
//crLightListSelectMethod::crLightListSelectMethod(const crLightListSelectMethod& handle):
//crMethod(handle),
//m_stageDlgString(handle.m_stageDlgString),
//m_lightListElement(handle.m_lightListElement)
//{
//}
//
//void crLightListSelectMethod::inputParam(int i, void *param)
//{
//}
//
//void crLightListSelectMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_stageDlgString = str;
//		break;
//	case 1:
//		m_lightListElement = str;
//		break;
//	}
//}
//
//void crLightListSelectMethod::operator()(crHandle &handle)
//{
//	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
//	if(imageStage && !imageStage->isHide())
//	{
//        crListBoxElement *listBox = dynamic_cast<crListBoxElement *>(imageStage->getElement(m_lightListElement));
//		if(listBox)
//		{
//			crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
//			assert(!body);
//			crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
//			assert(!bodyInfo);
//			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
//			if(!selectNodeVec.empty())
//			{
//				std::string currentSelectLight = selectNodeVec[0]->getName();
//				std::string data;
//				for(int i = 0; i<listBox->getDataSize(); ++i)
//				{
//					listBox->getData(i,data);
//					if(currentSelectLight.compare(data)==0)
//					{
//						listBox->select(i);
//					}
//				}
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crNodeListSelectMethod
//
/////////////////////////////////////////
crNodeListSelectMethod::crNodeListSelectMethod()
{
}

crNodeListSelectMethod::crNodeListSelectMethod(const crNodeListSelectMethod& handle):
crMethod(handle),
m_stageDlgString(handle.m_stageDlgString),
m_nodeListElement(handle.m_nodeListElement)
{
}

void crNodeListSelectMethod::inputParam(int i, void *param)
{
}

void crNodeListSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageDlgString = str;
		break;
	case 1:
		m_nodeListElement = str;
		break;
	}
}

void crNodeListSelectMethod::operator()(crHandle &handle)
{
	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
	if(imageStage && !imageStage->isHide())
	{
		crListBoxElement *listBox = dynamic_cast<crListBoxElement *>(imageStage->getElement(m_nodeListElement));
		if(listBox)
		{
			crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
			assert(!body);
			crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
			assert(!bodyInfo);
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				std::string currentSelect = selectNodeVec[0]->getName();
				std::string data;
				for(int i = 0; i<listBox->getDataSize(); ++i)
				{
					listBox->getData(i,data);
					if(currentSelect.compare(data)==0)
					{
						listBox->select(i);
					}
				}
			}
		}
	}
}
///////////////////////////////////////////
////
////crPlaceLoadedNodeMethod
////
///////////////////////////////////////////
//crPlaceLoadedNodeMethod::crPlaceLoadedNodeMethod()
//{
//}
//
//crPlaceLoadedNodeMethod::crPlaceLoadedNodeMethod(const crPlaceLoadedNodeMethod& handle):
//crMethod(handle)
//{
//}
//
//void crPlaceLoadedNodeMethod::inputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 1:
//		m_loadedNode = param?(crNode*)param:NULL;
//		break;
//	}
//}
//
//void crPlaceLoadedNodeMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crPlaceLoadedNodeMethod::operator()(crHandle &handle)
//{
//	if(m_loadedNode.valid())
//	{
//		CRProducer::crViewer *viewer = crKeyboardMouseHandle::getInstance()->getBindViewer();
//		assert(!viewer);
//
//		crMatrix viewerMat = viewer->getViewMatrix();       
//		crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(m_loadedNode.get());
//		if(!matrixTransform)
//		{
//			matrixTransform = new crMatrixTransform;
//			matrixTransform->setName("PlaceLoadedNode");
//			m_loadedNode->getParent(0)->replaceChild(m_loadedNode.get(),matrixTransform);
//			matrixTransform->addChild(m_loadedNode.get());
//		}
//
//		crVector3d eye,center,up;
//		viewerMat.getLookAt(eye,center,up);
//		crVector3d lv = center - eye;
//		lv.normalize();
//		matrixTransform->setMatrix(crMatrix::translate(center + lv * m_loadedNode->getBound().radius()));
//
//		m_loadedNode = matrixTransform;
//		handle.outputParam(1,m_loadedNode.get());
//	}
//}
/////////////////////////////////////////
//
//crEnablePhysicsMethod
//
/////////////////////////////////////////
crEnablePhysicsMethod::crEnablePhysicsMethod()
{
}

crEnablePhysicsMethod::crEnablePhysicsMethod(const crEnablePhysicsMethod& handle):
crMethod(handle),
m_interval(handle.m_interval)
{
}

void crEnablePhysicsMethod::inputParam(int i, void *param)
{
}

void crEnablePhysicsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crEnablePhysicsMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	crMatterObject *matterObject;
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
		itr != selectNodeVec.end();
		++itr )
	{
		matterObject = dynamic_cast<crMatterObject *>(itr->get());
		if(matterObject)
		{
			matterObject->enablePhysics(m_interval);
		}
	}
	bodyInfo->unSelectAll();
}

/////////////////////////////////////////
//
//crRemoveMatrixMethod
//
/////////////////////////////////////////
crRemoveMatrixMethod::crRemoveMatrixMethod()
{
}

crRemoveMatrixMethod::crRemoveMatrixMethod(const crRemoveMatrixMethod& handle):
crMethod(handle)
{
}

void crRemoveMatrixMethod::inputParam(int i, void *param)
{
}

void crRemoveMatrixMethod::addParam(int i, const std::string& str)
{
}

void crRemoveMatrixMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	crMatrixTransform *matrixTransform;
	crGroup *newGroup;
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
		itr != selectNodeVec.end();
		++itr )
	{
		matrixTransform = dynamic_cast<crMatrixTransform *>(itr->get());
		if(matrixTransform)
		{
			newGroup = new crGroup;
			matrixTransform->getParent(0)->replaceChildExt(matrixTransform,newGroup);
		    (*itr)->doEvent(WCH_UNSELECTNODE);
			*itr = newGroup;
			(*itr)->doEvent(WCH_SELECTNODE);
		}
	}
}
/////////////////////////////////////////
//
//crFreezeMatrixMethod
//
/////////////////////////////////////////
crFreezeMatrixMethod::crFreezeMatrixMethod()
{
}

crFreezeMatrixMethod::crFreezeMatrixMethod(const crFreezeMatrixMethod& handle):
crMethod(handle)
{
}

void crFreezeMatrixMethod::inputParam(int i, void *param)
{
}

void crFreezeMatrixMethod::addParam(int i, const std::string& str)
{
}

void crFreezeMatrixMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	crData *infoData = bodyInfo->getDataClass();
	assert(infoData);

	void *_selectType;
	infoData->getParam(0,_selectType);
	int selectType = *((int*)_selectType);
	crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
	if(selectType == 0 && !selectNodeVec.empty())
	{
		bodyInfo->unSelectAll();
		CRCore::crGroup *proxyNode;
		for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
			itr != selectNodeVec.end();
			++itr )
		{
		    //(*itr)->doEvent(WCH_UNSELECTNODE);
			proxyNode = dynamic_cast<crGroup *>((*itr)->getParent(0));
			crDisconnectDrawableVisitor ddv;
			proxyNode->accept(ddv);
			CRUtil::crOptimizer::crFreezeTransformsVisitor fztv;
			fztv.setIgnorePhysics(true);
            proxyNode->accept(fztv);
			fztv.freeze();
			CRUtil::crPrepareVisitor prepareVisitor;
			proxyNode->accept(prepareVisitor);
			proxyNode->releaseObjects();
			crLoadManager::getInstance()->requestCompile(proxyNode);
			//*itr = proxyNode->getChild(0);
			//(*itr)->doEvent(WCH_SELECTNODE);
		}
	}
}
/////////////////////////////////////////
//
//crInsertNodeMethod
//
/////////////////////////////////////////
crInsertNodeMethod::crInsertNodeMethod()
{
}

crInsertNodeMethod::crInsertNodeMethod(const crInsertNodeMethod& handle):
crMethod(handle)
{
}

void crInsertNodeMethod::inputParam(int i, void *param)
{
}

void crInsertNodeMethod::addParam(int i, const std::string& str)
{
}

void crInsertNodeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	ref_ptr<crNode> node;
	ref_ptr<crGroup> newGroup;
	ref_ptr<crGroup> parent;
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
		itr != selectNodeVec.end();
		++itr )
	{
		node = itr->get();
		if(node.get())
		{
			newGroup = new crGroup;
			newGroup->setName("newGroup");
			parent = node->getParent(0);
			parent->removeChild(node.get());
			newGroup->addChild(node.get());
			parent->addChild(newGroup.get());
			(*itr)->doEvent(WCH_UNSELECTNODE);
			*itr = newGroup.get();
			(*itr)->doEvent(WCH_SELECTNODE);
		}
	}
}
/////////////////////////////////////////
//
//crRemoveNodeMethod
//
/////////////////////////////////////////
crRemoveNodeMethod::crRemoveNodeMethod()
{
}

crRemoveNodeMethod::crRemoveNodeMethod(const crRemoveNodeMethod& handle):
crMethod(handle)
{
}

void crRemoveNodeMethod::inputParam(int i, void *param)
{
}

void crRemoveNodeMethod::addParam(int i, const std::string& str)
{
}

void crRemoveNodeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	ref_ptr<crNode> node;
	ref_ptr<crGroup> group;
	ref_ptr<crGroup> parent;
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
		itr != selectNodeVec.end();
		++itr )
	{
		node = itr->get();
		if(node.get())
		{
			parent = node->getParent(0);
			parent->removeChild(node.get());
			group = dynamic_cast<crGroup *>(node.get());
            if(group.get())
			{
				int num = group->getNumChildren();
				for(int i = 0; i<num; i++)
					parent->addChild(group->getChild(i));
				group->removeChild(0,num);
			}
		}
	}
	bodyInfo->unSelectAll();
}

/////////////////////////////////////////
//
//crShowCameraPosMethod
//
/////////////////////////////////////////
crShowCameraPosMethod::crShowCameraPosMethod()
{
}

crShowCameraPosMethod::crShowCameraPosMethod(const crShowCameraPosMethod& handle):
crMethod(handle),
m_cameraPosText(handle.m_cameraPosText),
m_referencePosText(handle.m_referencePosText),
m_drawableInfoText(handle.m_drawableInfoText)
{
}

void crShowCameraPosMethod::inputParam(int i, void *param)
{
}

void crShowCameraPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_cameraPosText = str;
		break;
	case 1:
		m_referencePosText = str;
		break;
	case 2:
		m_drawableInfoText = str;
		break;
	}
}

void crShowCameraPosMethod::operator()(crHandle &handle)
{
	CRText::crText *cameraPosText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable(m_cameraPosText));
	if(cameraPosText)
	{
		std::string cameraPos;
		crVector3 pos = crBrain::getInstance()->getCameraPosition();
		crArgumentParser::updateOrAddKeyValue(cameraPos,"CameraPos",pos);
		cameraPosText->setText(cameraPos);
		cameraPosText->setVisiable(true);
	}

	CRText::crText *referencePosText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable(m_referencePosText));
    if(referencePosText)
	{
		if(!m_editorCoordinate.valid())
		{
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("EditorAxis");
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			crBrain::getInstance()->getCurrentActiveDB()->accept(searchByNameVisitor);
			m_editorCoordinate = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
		}
		if(m_editorCoordinate.valid())
		{
			crMatrix matrix = m_editorCoordinate->getMatrix();
			std::string referencePos;
			crVector3 pos = matrix.getTrans();
			crArgumentParser::updateOrAddKeyValue(referencePos,"ReferencePos",pos);
			referencePosText->setText(referencePos);
			referencePosText->setVisiable(true);
		}
	}
	CRText::crText *drawableInfoText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable(m_drawableInfoText));
	if(drawableInfoText)
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		crData *infoData = bodyInfo->getDataClass();
		void *param;
		infoData->getParam(7,param);
		crDrawable *selectDrawable = (crDrawable*)param;
		if(selectDrawable)
		{
			crGeometry *geom = selectDrawable->asGeometry();
			if(geom)
			{
				int vtxcount = geom->getVertexArray()->getNumElements();
				int primcount = 0;
				for( crGeometry::PrimitiveList::iterator itr = geom->getPrimitiveList().begin();
					itr != geom->getPrimitiveList().end();
					++itr )
				{
					primcount+=(*itr)->getNumPrimitives();
				}
				crVector2i vec2(vtxcount,primcount);
				std::string info;
				crArgumentParser::updateOrAddKeyValue(info,"Drawable(Vtx,Prim)",vec2);
				drawableInfoText->setText(info);
				drawableInfoText->setVisiable(true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crPickTargetMethod
//
/////////////////////////////////////////
crPickTargetMethod::crPickTargetMethod()
{
}

crPickTargetMethod::crPickTargetMethod(const crPickTargetMethod& handle):
crMethod(handle)
{
}

void crPickTargetMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	//case 1:
	//	m_node = (crNode*)param;
	//	break;
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
		}
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crPickTargetMethod::addParam(int i, const std::string& str)
{
}

void crPickTargetMethod::operator()(crHandle &handle)
{
	crObject *mousePickObj = NULL;
	crDrawable *mousePickMesh = NULL;
    int targetType = 0;

	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;
	CRCore::crIntersectVisitor::HitList hits;
	bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),hits);

	if(!hits.empty())
	{
		mousePickObj = hits[0].getObject();
		std::string groundString;
		crArgumentParser::readKeyValue(mousePickObj->getDescriptions(),"GroundString",groundString);
		if(groundString.compare("cannotpass") != 0)
		{
			CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
			searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByClassNameVisitor.insertClassNameId("crViewMatterObject");
			searchByClassNameVisitor.insertClassNameId("crCharacterMatterObject");
			searchByClassNameVisitor.insertClassNameId("crMissileSetOrFlackMatterObject");
			searchByClassNameVisitor.insertClassNameId("crBodyCaseMatterObject");
			mousePickObj->accept(searchByClassNameVisitor);
			crNode *targetNode = searchByClassNameVisitor.getResult();
			if(targetNode)
			{
				handle.outputParam(4,targetNode);
				targetType = 1;
			}
			else
			{
				crVector3 pickPoint = hits[0].getWorldIntersectPoint();
				handle.outputParam(5,&pickPoint);
				targetType = 2;
			}
		}
	}
	handle.outputParam(3,&targetType);
	if(targetType!=1)
	{
	    handle.outputParam(4,NULL);
	}
}
/////////////////////////////////////////
//
//crMouseMoveCursorMethod
//
/////////////////////////////////////////
crMouseMoveCursorMethod::crMouseMoveCursorMethod()
{
}

crMouseMoveCursorMethod::crMouseMoveCursorMethod(const crMouseMoveCursorMethod& handle):
crMethod(handle),
m_enemyCursor(handle.m_enemyCursor),
m_friendCursor(handle.m_friendCursor),
m_neutralityCursor(handle.m_neutralityCursor)
{
}

void crMouseMoveCursorMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crMouseMoveCursorMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_enemyCursor = str;
		break;
	case 1:
		m_friendCursor = str;
		break;
	case 2:
		m_neutralityCursor = str;
		break;
	}
}

void crMouseMoveCursorMethod::operator()(crHandle &handle)
{
	if(m_this == crSceneManager::getInstance()->getPlayer())
	{
		crObject *mousePickObj = NULL;
		crDrawable *mousePickMesh = NULL;
		int targetType = 0;

		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!bindview || !bindview->isInited())
			return;
		CRCore::crIntersectVisitor::HitList hits;
		bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),hits);

		if(!hits.empty())
		{
			mousePickObj = hits[0].getObject();
			std::string groundString;
			crArgumentParser::readKeyValue(mousePickObj->getDescriptions(),"GroundString",groundString);
			if(groundString.compare("cannotpass") != 0)
			{
				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByClassNameVisitor.insertClassNameId("crViewMatterObject");
				searchByClassNameVisitor.insertClassNameId("crCharacterMatterObject");
				searchByClassNameVisitor.insertClassNameId("crMissileSetOrFlackMatterObject");
				searchByClassNameVisitor.insertClassNameId("crBodyCaseMatterObject");
				mousePickObj->accept(searchByClassNameVisitor);
				crViewMatterObject *targetNode = dynamic_cast<crViewMatterObject *>(searchByClassNameVisitor.getResult());
				if(targetNode)
				{
					int teamflg = targetNode->getTeam() * m_this->getTeam();
					if(teamflg>0)
					{
						bindview->showCursor(m_friendCursor);
					}
					else if(teamflg<0)
					{//敌人
						bindview->showCursor(m_enemyCursor);
					}
					else
					{
						bindview->showCursor(m_neutralityCursor);
					}
				}
				else
				{
                    bindview->showCursor("Default");
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crTurnToTargetMethod
//
/////////////////////////////////////////
crTurnToTargetMethod::crTurnToTargetMethod()
{
}

crTurnToTargetMethod::crTurnToTargetMethod(const crTurnToTargetMethod& handle):
crMethod(handle)
{
}

void crTurnToTargetMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 3:
		m_targerPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	}
}

void crTurnToTargetMethod::addParam(int i, const std::string& str)
{
}

void crTurnToTargetMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(bot)
	{
		crVector3f myPos = bot->getTrans();
		//turnToTarget
		crVector3f targetPos = m_targerPosition;
		targetPos[2] = myPos[2];
		if(targetPos!=myPos)
		{
			crVector3f targetDir = targetPos - myPos;
			targetDir.normalize();
			CRCore::crMatrix newMat;
			//targetDir = CRCore::Y_AXIS;//test
			if(targetDir[1] > 0.999)
			{
				newMat.makeRotate(-CRCore::Y_AXIS,targetDir);
				CRCore::crVector3d ep,center,up;
				newMat.getLookAt(ep,center,up);
				if(center[2]>0.0f)
				{
					center = -center;
				}
				newMat.makeLookAt(ep,center,up);
			}
			else
				newMat.makeRotate(-CRCore::Y_AXIS,targetDir);

			newMat.setTrans(myPos);
			bot->setMatterObjectMatrix(newMat,CRPhysics::crMatterObject::MD_RotMatrix);
		}
	}
}

/////////////////////////////////////////
//
//crMoveToTargetMethod
//
/////////////////////////////////////////
crMoveToTargetMethod::crMoveToTargetMethod():
m_time(0L),
m_init(false),
m_velocity(3.0f)
{
}

crMoveToTargetMethod::crMoveToTargetMethod(const crMoveToTargetMethod& handle):
crMethod(handle),
m_aiTask(NULL),
m_velocity(handle.m_velocity),
m_time(0L),
m_init(false)
{
}

void crMoveToTargetMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_targerPosition = *((crVector3*)(LOINT64(param64)));
			m_velocity = *((float*)(HIINT64(param64)));
		}
		break;
	case 3:
		m_targerPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	case 4:
		m_velocity = param?*((float*)param):3.0f;
		break;
	}
}

void crMoveToTargetMethod::addParam(int i, const std::string& str)
{
}

void crMoveToTargetMethod::init()
{
	if(!m_init)
	{
        m_aiTask = new crAITask;
        //m_moveTask->setName(bot->getName());
		m_aiTask->setLoopMode(crAITask::NO_LOOPING);
        crOrdinalMsgControlPoint *omcp = new crOrdinalMsgControlPoint;
		omcp->setNeedVelocity(false);
		omcp->setIgnoreZ(true);
		CRCore::Msg msg;
		msg.m_msg = MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE);
		msg.m_param = MAKEINT64(MAKEINT32(WCH_UP,crGUIEventAdapter::KEYDOWN),NULL);
		msg.m_interval = 1.0f;
        omcp->addOrdinalMsg(msg);

		msg.m_msg = MAKEINT64(WCH_INPUTAITASK,NULL);
		msg.m_param = MAKEINT64(MAKEINT32(WCHAI_End,NULL),m_aiTask.get());
		msg.m_interval = 0.0f;
        omcp->addOrdinalMsg(msg);

		m_aiTask->pushTaskPoint(omcp);

		omcp = new crOrdinalMsgControlPoint;
		omcp->setNeedVelocity(false);
		omcp->setIgnoreZ(true);
		msg.m_msg = MAKEINT64(WCH_INPUTAITASK,NULL);
		msg.m_param = MAKEINT64(MAKEINT32(WCHAI_End,NULL),m_aiTask.get());
		msg.m_interval = 0.0f;
		omcp->addOrdinalMsg(msg);

		m_aiTask->pushTaskPoint(omcp);

		m_init = true;
	}
}

void crMoveToTargetMethod::operator()(crHandle &handle)
{
	//CRCore::notify(CRCore::FATAL)<<"m_targetType = "<<m_targetType<<std::endl;
	if(m_this)
	{
		if(!m_init) init();

		crOrdinalMsgControlPoint *omcp;
		CRCore::Msg* msg;
		CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
		float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
		if(dt>0.5f)
		{
			//CRCore::notify(CRCore::FATAL)<<"6"<<std::endl;
			//距离判断
			crVector3f myPos = m_this->getTrans();
			float dis = (m_targerPosition - myPos).length();
			if(dis>0)
			{
				crAITask::TaskPointVec &taskPointVec = m_aiTask->getTaskPointVec();
				float time = dis/m_velocity + 1.0f;//0.1余量
				omcp = dynamic_cast<crOrdinalMsgControlPoint *>(taskPointVec[0].get());
				unsigned int msgid = 0;
				msg = omcp->getOrdinalMsg(msgid);
				msg->m_interval = time;

				omcp = dynamic_cast<crOrdinalMsgControlPoint *>(taskPointVec[1].get());
				omcp->setPosition(m_targerPosition);

                m_this->doEvent(MAKEINT64(WCH_INPUTAITASK,NULL),MAKEINT64(MAKEINT32(WCHAI_Begin,WCHAI_SetCPToBot),m_aiTask.get()));
				m_time = t1;
			}
		}
	}
}
/////////////////////////////////////////
//
//crBotAISelectTargetMethod
//
/////////////////////////////////////////
crBotAISelectTargetMethod::crBotAISelectTargetMethod():
m_time_lost(0.1f),
m_interval_lost(0.0f)
{
}

crBotAISelectTargetMethod::crBotAISelectTargetMethod(const crBotAISelectTargetMethod& handle):
crMethod(handle),
m_time_lost(handle.m_time_lost),
m_interval_lost(handle.m_interval_lost)
{
}

void crBotAISelectTargetMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crBotAISelectTargetMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_interval_lost = atof(str.c_str());
		break;
	}
}

void crBotAISelectTargetMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(bot)
	{
	    crData *data = bot->getDataClass();
		crGameBodyInfo *bodyInfo = bot->getGameBodyInfo();
		if(bodyInfo && data)
		{
			crGameBodyInfo::LockedTargetSet &lockedSet = bodyInfo->getLockedTargetSet();
			if(!lockedSet.empty())
			{
				m_time_lost = 0.0f;
				CRCore::ref_ptr<crViewMatterObject> setItr = *(lockedSet.begin());
                data->inputParam(5,setItr.get());
			}
			else
			{
				if(m_time_lost>m_interval_lost)
					data->inputParam(5,NULL);
				else
					m_time_lost += bot->getCurrentFrameInterval();
			}
		}
	}
}

///////////////////////////////////////////
////
////crBotAIPatrolMethod
////
///////////////////////////////////////////
//crBotAIPatrolMethod::crBotAIPatrolMethod():
//m_time_aimsg(0.0f),
//m_currentIndex(0),
//m_currentMsgIndex(0),
//m_interval_aimsg(0.0f),
//m_isAircraft(false),
//m_preControlPoint(NULL)
//{
//}
//
//crBotAIPatrolMethod::crBotAIPatrolMethod(const crBotAIPatrolMethod& handle):
//crMethod(handle),
//m_time_aimsg(0.0f),
//m_currentIndex(0),
//m_currentMsgIndex(0),
//m_interval_aimsg(0.0f),
//m_isAircraft(false),
//m_preControlPoint(NULL),
//m_aiTask(handle.m_aiTask)
//{
//}
//
//void crBotAIPatrolMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_this = (crViewMatterObject*)param;
//		break;
//	}
//}
//
//void crBotAIPatrolMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_aiTask = crAITaskManager::getInstance()->getAITask(str);
//		if(!m_aiTask)
//			CRCore::notify(CRCore::FATAL)<<"crBotAIPatrolMethod::addParam(): not find aitask "<<str<<std::endl;
//		break;
//	}
//}
//
//void crBotAIPatrolMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_aiTask.valid())
//	{
//		crGameBodyInfo *bodyInfo = m_this->getGameBodyInfo();
//		if(!bodyInfo) return;
//		float interval = m_this->getCurrentFrameInterval();
//		m_interval_aimsg += interval;
//		//m_previousFrameTime = referenceTime;
//		crControlPoint *nextControlPoint = 0;
//		crControlPoint *currentcp = m_aiTask->getCurrentTaskPoint(m_currentIndex);
//		if(!currentcp) return;
//		int nextIndex;
//
//		//body->resetViewPitch();
//		crVector3f myPos = m_this->getTrans();
//		//myPos[2]-=body->getPhysicsBox()[2] * 0.5f;//body->getBoundBox().zLength() * 0.5f;
//		CRCore::crVector3f myDir = -m_this->getDirection();
//
//		//CRCore::notify(CRCore::ALWAYS)<<"crMoviesAIUpdatePPC::parseParamImplementation myPos = "<<myPos<<" name = "<<body->getName()<<" cp = "<<currentcp->getPosition()<<std::endl;
//		if(currentcp->isInRange(myPos,bodyInfo->getTaskPointRange()))
//		{
//			//CRCore::notify(CRCore::FATAL)<<"crMoviesAIUpdatePPC::parseParamImplementation isInRange: "<<body->getName()<<std::endl;
//			//CRCore::notify(CRCore::ALWAYS)<<"crMoviesAIUpdatePPC::parseParamImplementation InRange "<<currentcp->getPosition()<<" "<<m_currentIndex<<std::endl;
//			nextControlPoint = m_aiTask->getNextTaskPoint(m_currentIndex,nextIndex);
//			m_currentIndex = nextIndex;
//
//			if(m_preControlPoint!=currentcp)
//			{
//				m_currentMsgIndex = 0;
//				m_time_aimsg = 0;
//				m_preControlPoint = currentcp;
//				if(m_preControlPoint->getNeedVelocity())
//				{
//					m_this->setVelocity(m_preControlPoint->getVelocity());
//				}
//			}
//		}
//		if(!nextControlPoint)
//			nextControlPoint = currentcp;
//		if(!m_preControlPoint)
//		{
//			m_currentMsgIndex = 0;
//			m_time_aimsg = 0;
//			m_preControlPoint = currentcp;
//			if(m_preControlPoint->getNeedVelocity())
//			{
//				m_this->setVelocity(m_preControlPoint->getVelocity());
//			}
//		}
//		if(nextControlPoint)
//		{
//			crVector3f targetPos = nextControlPoint->getPosition();
//			if(m_isAircraft)
//			{
//				crVector3f targetDir = targetPos - myPos;
//				targetDir.normalize();
//				//if(body->getVelocity().x() > 0.0f)
//				{
//					//CRCore::notify(CRCore::ALWAYS)<<" nextControlPoint "<<std::endl;
//					//turn to cp
//					turnToDir(bodyNC,targetDir,myDir);
//				}
//			}
//			else
//			{
//				targetPos[2] = myPos[2];
//				if(targetPos!=myPos)
//				{
//					crVector3f targetDir = targetPos - myPos;
//					targetDir.normalize();
//					CRCore::crMatrixd newMat;
//					//targetDir = CRCore::Y_AXIS;//test
//					if(targetDir[1] > 0.999)
//					{
//						newMat.makeRotate(-CRCore::Y_AXIS,targetDir);
//						CRCore::crVector3d ep,center,up;
//						newMat.getLookAt(ep,center,up);
//						if(center[2]>0.0f)
//						{
//							center = -center;
//						}
//						newMat.makeLookAt(ep,center,up);
//					}
//					else
//						newMat.makeRotate(-CRCore::Y_AXIS,targetDir);
//
//					newMat.setTrans(myPos);
//					m_this->setMatterObjectMatrix(newMat,CRPhysics::crMatterObject::MD_RotMatrix);
//				}
//			}
//		}
//		if(m_preControlPoint)
//		{
//			if(m_interval_aimsg>m_time_aimsg)
//			{//对于队友该如何选择CP？
//				//队友不主动向CP移动，但它向主角靠近，随而触发进入到下一CP，又或者，可以通过外部设置让其进入到下一CP
//				m_interval_aimsg = 0.0f;
//				Msg* msg;
//				if(dynamic_cast<crProbabilityMsgControlPoint *>(m_preControlPoint))
//				{
//					msg = (dynamic_cast<crProbabilityMsgControlPoint *>(m_preControlPoint))->getProbabilityMsg();
//				}
//				else if(dynamic_cast<crOrdinalMsgControlPoint *>(m_preControlPoint))
//				{
//					msg = (dynamic_cast<crOrdinalMsgControlPoint *>(m_preControlPoint))->getOrdinalMsg(m_currentMsgIndex);
//				}
//				if(msg)
//				{
//					bodyNC->acceptEventMessage(msg->m_msg,msg->m_wparam,msg->m_lparam);
//					m_time_aimsg = msg->m_interval;
//				}
//				//CRCore::notify(CRCore::ALWAYS)<<"crBodyCaseAIUpdatePPC::parseParamImplementation ProbabilityMsg "<<m_lastmsg.m_msg<<" "<<m_lastmsg.m_wparam<<" "<<m_lastmsg.m_lparam<<std::endl;
//			}
//		}
//
//	}
//}
//
//float crBotAIPatrolMethod::computeAngle(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir)
//{
//	float anticlockwise = -1.0f;
//	if(myDir[0]>=0.0f && myDir[1]>=0.0f)
//	{//第一象限
//		/*if(targetDir[1]>myDir[1] || (targetDir[0]<0.0f && targetDir[1]>-myDir[1]))
//		anticlockwise = 1;*/
//		if(targetDir[1]<myDir[1] && targetDir[0] > -myDir[0])
//			anticlockwise = -1.0f;
//		else anticlockwise = 1.0f;
//	}
//	else if(myDir[0]<0.0f && myDir[1]>=0.0f)
//	{//第二象限
//		if(targetDir[1]<myDir[1] && targetDir[0] < -myDir[0])
//			anticlockwise = 1.0f;
//	}
//	else if(myDir[0]<0.0f && myDir[1]<0.0f)
//	{//第三象限
//		if(targetDir[1]>myDir[1] && targetDir[0] < -myDir[0])
//			anticlockwise = -1.0f;
//		else anticlockwise = 1.0f;
//	}
//	else if(myDir[0]>=0.0f && myDir[1]<0.0f)
//	{//第四象限
//		if(targetDir[1]>myDir[1] && targetDir[0] > -myDir[0])
//			anticlockwise = 1.0f;
//	}
//	return anticlockwise * acos(myDir * targetDir);
//}
//void crBotAIPatrolMethod::turnToDir(crEventGroup *bodyNC,const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir)
//{
//	crVector3f _targetDir = targetDir;
//	crVector3f _myDir = myDir;
//	_targetDir[2] = 0.0f;
//	_targetDir.normalize();
//	_myDir[2] = 0.0f;
//	_myDir.normalize();
//	float angle = computeAngle(_targetDir,_myDir);
//
//	if(angle>0.01f)//0.5弧度每秒，25帧的精度是0.02
//	{
//		if(m_isAircraft)
//			bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_Q,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//		else
//		    bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_LEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//	}
//	else if(angle<-0.01f)
//	{
//		if(m_isAircraft)
//		    bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_E,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//		else
//			bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_RIGHT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//	}
//	else
//	{
//		if(m_isAircraft)
//		{
//			bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_Q,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
//			_targetDir = targetDir;
//			_myDir = myDir;
//			_targetDir[0] = 0.0f;
//			_targetDir.normalize();
//			_myDir[0] = 0.0f;
//			_myDir.normalize();
//			float cos_angle = _myDir * _targetDir;
//			float anticlockwise = _targetDir[2]>_myDir[2]? 1.0f : -1.0f;
//			angle = anticlockwise * acos(cos_angle);
//			if(angle>0.02f)
//			{
//				bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_DOWN,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//			}
//			else if(angle<-0.02f)
//			{
//				bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_UP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//			}
//			else
//			{
//				bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_UP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
//			}
//		}
//		else
//		{
//			bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_LEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
//		}
//	}
//}
/////////////////////////////////////////
//
//crShowBotHeadInfoMethod
//
/////////////////////////////////////////
crShowBotHeadInfoMethod::crShowBotHeadInfoMethod():
m_showflg(1),
m_maxRange(50.0f),
m_zoffset(0.3f)
{
}

crShowBotHeadInfoMethod::crShowBotHeadInfoMethod(const crShowBotHeadInfoMethod& handle):
crMethod(handle),
m_showflg(handle.m_showflg),
m_maxRange(handle.m_maxRange),
m_zoffset(handle.m_zoffset)
{
}
crShowBotHeadInfoMethod::~crShowBotHeadInfoMethod()
{
 //   if(m_nameText.valid())
	//{
	//	m_nameText->releaseObjects();
 //       crFilterRenderManager::getInstance()->removeDrawable(m_nameText.get());
	//	CRCore::notify(CRCore::ALWAYS)<<"crShowBotHeadInfoMethod remove text "<<m_nameText->getName()<<std::endl;
	//}
}
void crShowBotHeadInfoMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
}

void crShowBotHeadInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_showflg = atoi(str.c_str());
		break;
	case 1:
		m_maxRange = atof(str.c_str());
		break;
	case 2:
		m_zoffset = atof(str.c_str());
		break;
	}
}
std::string crShowBotHeadInfoMethod::getNameTextStr()
{
	static int index = 0;
	char str[10];
	sprintf(str,"Bot\0",index++);
    return str;	
}
void crShowBotHeadInfoMethod::operator()(crHandle &handle)
{
	if(m_showflg>0)
	{
		crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
		if(bot)
		{
			if(!m_nameText.valid())
			{
				m_nameText = new CRText::crText;
				m_nameText->setName(getNameTextStr());
				m_nameText->setFont("simhei.ttf");
				m_nameText->setColor(CRCore::crVector4(1.0f,0.0f,0.0f,1.0f));
				float characterSize = 15.0f;
				m_nameText->setFontResolution((unsigned int)characterSize,(unsigned int)characterSize);
				m_nameText->setCharacterSize(characterSize);
				m_nameText->setAlignment(CRText::crText::CENTER_CENTER);
				//int wstr_len = (strlen(bot->getName().c_str())+1) * sizeof(wchar_t);
				//wchar_t* wstr_value = (wchar_t*)alloca(wstr_len);
				////mbstowcs( wstr_value, str_value, strlen(str_value)+1);
				//MultiByteToWideChar(CP_ACP, 0, bot->getName().c_str(), wstr_len/sizeof(wchar_t), wstr_value, wstr_len/sizeof(wchar_t) );
				m_nameText->setText(CRIOManager::convertUTF8toUTF16(bot->getName()).c_str());
				m_nameText->setVisiable(false);
				crFilterRenderManager::getInstance()->addDrawable(m_nameText.get(),true);
			}
			if(bot->getVisiable())
			{
				crData *data = bot->getDataClass();
				crGameBodyInfo *bodyInfo = bot->getGameBodyInfo();
				if(bodyInfo && data)
				{
					crVector3 botPos = bot->getTrans();
					crVector3 cameraPos = crBrain::getInstance()->getCameraPosition();
					float dist = (botPos-cameraPos).length();
					if(dist<m_maxRange)
					{
						botPos[2] += bot->getBoundBox().zLength() * 0.5f + m_zoffset;
						crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
						if(bindview && bindview->isInited())
						{
							crVector3 window;
							bindview->getDefaultSceneHandler()->getSceneView()->projectObjectIntoWindow(botPos,window);
							window[2] = 0.0f;
							//m_nameText->setText(bot->getName());
							m_nameText->setPosition(window);
							m_nameText->setVisiable(true);
						}
					}
					else
					{
						m_nameText->setVisiable(false);
					}
				}
			}
			else
			{
                m_nameText->setVisiable(false);
			}
		}
	}
}

/////////////////////////////////////////
//
//crChangeViewOffsetMethod
//
/////////////////////////////////////////
crChangeViewOffsetMethod::crChangeViewOffsetMethod():
m_step(-1.0f)
{
}

crChangeViewOffsetMethod::crChangeViewOffsetMethod(const crChangeViewOffsetMethod& handle):
crMethod(handle),
m_step(handle.m_step),
m_range(handle.m_range)
{
}

void crChangeViewOffsetMethod::inputParam(int i, void *param)
{
}

void crChangeViewOffsetMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_step = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_range);
		break;
	}
}

void crChangeViewOffsetMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = crSceneManager::getInstance()->getPlayer();
	float viewOffset = bot->getViewOffset();
	viewOffset += m_step;
	viewOffset = CRCore::clampTo(viewOffset,m_range[0],m_range[1]);
	bot->setViewOffset(viewOffset);
}
/////////////////////////////////////////
//
//crChangeMoveSpeedMethod
//
/////////////////////////////////////////
crChangeMoveSpeedMethod::crChangeMoveSpeedMethod():
m_step(-1.0f)
{
}

crChangeMoveSpeedMethod::crChangeMoveSpeedMethod(const crChangeMoveSpeedMethod& handle):
crMethod(handle),
m_step(handle.m_step),
m_range(handle.m_range)
{
}

void crChangeMoveSpeedMethod::inputParam(int i, void *param)
{
}

void crChangeMoveSpeedMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_step = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_range);
		break;
	}
}

void crChangeMoveSpeedMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = crSceneManager::getInstance()->getPlayer();
	float speed = bot->getMaxSpeed()[0];
	speed += m_step;
	speed = CRCore::clampTo(speed,m_range[0],m_range[1]);
	bot->setMaxSpeed(crVector3(speed,speed,speed));
	bot->setMinSpeed(crVector3(-speed,-speed,-speed));
}

///////////////////////////////////////////
////
////crBotAIAvoidCollideMethod
////
///////////////////////////////////////////
//crBotAIAvoidCollideMethod::crBotAIAvoidCollideMethod():
//m_time_ac(0.1f),
//m_interval_ac(0.0f),
//m_avoidCollide(false)
//{
//}
//
//crBotAIAvoidCollideMethod::crBotAIAvoidCollideMethod(const crBotAIAvoidCollideMethod& handle):
//crMethod(handle),
//m_time_ac(handle.m_time_ac),
//m_interval_ac(handle.m_interval_ac),
//m_avoidCollide(false)
//{
//}
//
//void crBotAIAvoidCollideMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 1:
//		m_node = (crNode*)param;
//		break;
//	}
//}
//
//void crBotAIAvoidCollideMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_interval_ac = atof(str.c_str());
//		break;
//	}
//}
//
//void crBotAIAvoidCollideMethod::operator()(crHandle &handle)
//{
//	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
//	if(bot)
//	{
//		CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(bot->getParent(0));
//		CRAI::crNerveCellGroup *bodyNC = dynamic_cast<CRAI::crNerveCellGroup *>(bodyMG->getAttachedEventGroup());
//		crAIGroup *ai = bodyNC->getAI();
//		crVisionSenseInformation *vsi = ai->getVisionSenseInformation();
//		crRadarMatterObject *radar = bot->getRadar();
//		if(vsi && radar)
//		{
//			CRCore::crVector2 turnAngle;
//			//float height = vsi->getHeight();
//			if(radar->avoidCollide(vsi->getEnvMask(),turnAngle)/* || height<=m_pullUpHeight*/)
//			{//需要避障
//				///
//				if(turnAngle[0]>0.0f)
//					bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_LEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//				else if(turnAngle[0]<0.0f)
//					bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_RIGHT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//				//if(height<=m_pullUpHeight || turnAngle[1]>0.0f)
//				//	bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_DOWN,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//				//else if(turnAngle[1]<0.0f)
//				//				bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_UP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
//				m_interval_ac = 0.0;
//				m_avoidCollide = true;
//			}
//			if(m_avoidCollide)
//			{
//				m_interval_ac += bot->getCurrentFrameInterval();
//				if(m_interval_ac > m_time_ac)
//				{
//					bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_KEY_LEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
//					m_avoidCollide = false;
//				}
//			}
//		}
//	}
//	handle.outputParam(0,&m_avoidCollide);
//}
/////////////////////////////////////////
//
//crTalkWithBotMethod
//
/////////////////////////////////////////
crTalkWithBotMethod::crTalkWithBotMethod()
{
}

crTalkWithBotMethod::crTalkWithBotMethod(const crTalkWithBotMethod& handle):
crMethod(handle)
{
}

void crTalkWithBotMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_targetNode = NULL;
		}
		break;
	case 1:
		m_node = param?(crNode *)(param):NULL;
		break;
	case 3:
		m_targetNode = param?(crViewMatterObject *)(param):NULL;
		break;
	}
}

void crTalkWithBotMethod::addParam(int i, const std::string& str)
{
}

void crTalkWithBotMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(bot && m_targetNode.valid())
	{
        crData *data = bot->getDataClass();
		int gamestate = 1;
	    data->inputParam(5,m_targetNode.get());
		data->inputParam(1,&gamestate);


		data = m_targetNode->getDataClass();
		data->inputParam(5,bot);
		data->inputParam(1,&gamestate);
		if(bot == crSceneManager::getInstance()->getPlayer())
		    m_targetNode->doEvent(MAKEINT64(WCH_TALK,NULL));
	}
}
/////////////////////////////////////////
//
//crTalkEndMethod
//
/////////////////////////////////////////
crTalkEndMethod::crTalkEndMethod()
{
}

crTalkEndMethod::crTalkEndMethod(const crTalkEndMethod& handle):
crMethod(handle)
{
}

void crTalkEndMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = param?(crNode *)(param):NULL;
		break;
	}
}

void crTalkEndMethod::addParam(int i, const std::string& str)
{
}

void crTalkEndMethod::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(bot)
	{
		crData *data = bot->getDataClass();
		int gamestate = 0;
		data->inputParam(1,&gamestate);
		void *_targetNode;
		data->getParam(5,_targetNode);
		if(_targetNode!=NULL)
		{
            crViewMatterObject *targetNode = (crViewMatterObject *)_targetNode;
			crData *targetData = targetNode->getDataClass();
			targetData->inputParam(1,&gamestate);
			targetData->inputParam(5,NULL);
		}
		data->inputParam(5,NULL);
	}
}
/////////////////////////////////////////
//
//crDoEventMethod
//
/////////////////////////////////////////
crDoEventMethod::crDoEventMethod():
m_msgParam1(NULL),
m_msgParam2(NULL)
{
}

crDoEventMethod::crDoEventMethod(const crDoEventMethod& handle):
crMethod(handle),
m_msgParam1(handle.m_msgParam1),
m_msgParam2(handle.m_msgParam2)
{
}

void crDoEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = param?(crNode *)(param):NULL;
		break;
	}
}

void crDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msgParam1 = parseEventMsg(str);
		break;
	case 1:
		m_msgParam2 = parseEventMsg(str);
		break;
	}
}

void crDoEventMethod::operator()(crHandle &handle)
{
    if(m_node.valid())
		m_node->doEvent(MAKEINT64(m_msgParam1,m_msgParam2));
}
/////////////////////////////////////////
//
//crTargetDoEventMethod
//
/////////////////////////////////////////
crTargetDoEventMethod::crTargetDoEventMethod():
m_msgParam1(NULL),
m_msgParam2(NULL)
{
}

crTargetDoEventMethod::crTargetDoEventMethod(const crTargetDoEventMethod& handle):
crMethod(handle),
m_msgParam1(handle.m_msgParam1),
m_msgParam2(handle.m_msgParam2)
{
}

void crTargetDoEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = param?(crNode *)(param):NULL;
		break;
	}
}

void crTargetDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msgParam1 = parseEventMsg(str);
		break;
	case 1:
		m_msgParam2 = parseEventMsg(str);
		break;
	}
}

void crTargetDoEventMethod::operator()(crHandle &handle)
{
	if(m_node.valid())
	{
		crData *data = m_node->getDataClass();
		void *_targetNode;
		data->getParam(5,_targetNode);
		if(_targetNode!=NULL)
		{
			crNode *targetNode = (crNode *)_targetNode;
		    targetNode->doEvent(MAKEINT64(m_msgParam1,m_msgParam2));
		}
	}
}
/////////////////////////////////////////
//
//crMudererDoEventMethod
//
/////////////////////////////////////////
crMudererDoEventMethod::crMudererDoEventMethod():
m_msgParam1(NULL),
m_msgParam2(NULL)
{
}

crMudererDoEventMethod::crMudererDoEventMethod(const crMudererDoEventMethod& handle):
crMethod(handle),
m_msgParam1(handle.m_msgParam1),
m_msgParam2(handle.m_msgParam2)
{
}

void crMudererDoEventMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		m_param = param?*(_crInt64*)param:NULL;
		break;
	}
}

void crMudererDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msgParam1 = parseEventMsg(str);
		break;
	case 1:
		m_msgParam2 = parseEventMsg(str);
		break;
	}
}

void crMudererDoEventMethod::operator()(crHandle &handle)
{
	if(HIINT64(m_param))
	{
		crViewMatterObject *muderer = (crViewMatterObject *)(HIINT64(m_param));
		muderer->doEvent(MAKEINT64(m_msgParam1,m_msgParam2));
	}
}
/////////////////////////////////////////
//
//crReviveMosterMethod
//
/////////////////////////////////////////
crReviveMosterMethod::crReviveMosterMethod():
m_sceneMapID(0),
m_mapAreaID(0),
m_npcTroopID(-1),
m_revivePositionID(-1)
{
}

crReviveMosterMethod::crReviveMosterMethod(const crReviveMosterMethod& handle):
crMethod(handle),
m_sceneMapID(handle.m_sceneMapID),
m_mapAreaID(handle.m_mapAreaID),
m_npcTroopID(handle.m_npcTroopID),
m_revivePositionID(handle.m_revivePositionID)
{
}

void crReviveMosterMethod::inputParam(int i, void *param)
{
}

void crReviveMosterMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sceneMapID = atoi(str.c_str());
		break;
	case 1:
		m_mapAreaID = atoi(str.c_str());
		break;
	case 2:
		m_npcTroopID = atoi(str.c_str());
		break;
	case 3:
		m_revivePositionID = atoi(str.c_str());
		break;
	}
}

void crReviveMosterMethod::operator()(crHandle &handle)
{
	crSceneMap *sceneMap = crSceneManager::getInstance()->getCurrentScene()->getSceneMap(m_sceneMapID);
	if(sceneMap)
	{
        crMapArea *mapArea = sceneMap->getMapArea(m_mapAreaID);
		if(mapArea)
		{
			int id = m_npcTroopID;
			if(m_npcTroopID==-1)
			{
                id =  CRCore::rangei(0,mapArea->getNumTroop()).get_random();
			}
			crNPCTroop *troop = mapArea->getTroop(id);
			if(troop)
			{
                troop->revive(m_revivePositionID);
			}
		}
	}
}
/////////////////////////////////////////
//
//crIsStageOnShowMethod
//
/////////////////////////////////////////
crIsStageOnShowMethod::crIsStageOnShowMethod()
{
}

crIsStageOnShowMethod::crIsStageOnShowMethod(const crIsStageOnShowMethod& handle):
crMethod(handle),
m_stageName(handle.m_stageName)
{
}

void crIsStageOnShowMethod::inputParam(int i, void *param)
{
}

void crIsStageOnShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageName = str;
		break;
	}
}

void crIsStageOnShowMethod::operator()(crHandle &handle)
{
	crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_stageName);
	bool onshow = stage->getShow();
	handle.outputParam(0,&onshow);
}
////////////////////////////////
//
//crRackCharacterMeshMethod
//
///////////////////////////////////
crRackCharacterMeshMethod::crRackCharacterMeshMethod()
{}

crRackCharacterMeshMethod::crRackCharacterMeshMethod(const crRackCharacterMeshMethod& handle):
crMethod(handle),
m_fileName(handle.m_fileName)
{
}

void crRackCharacterMeshMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 3:
		if(param) m_fileName = *((std::string *)param);
		else m_fileName.clear();
		break;
	}
}

void crRackCharacterMeshMethod::addParam(int i, const std::string& str)
{
}

void crRackCharacterMeshMethod::operator()(crHandle &handle)
{
	if(m_fileName.empty()) return;
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		CreBodyNode *currentSelectNode = dynamic_cast<CreBodyNode *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			rcfg::ConfigScript cfg_script;
			if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_fileName))
			{
				CRCore::notify(CRCore::FATAL)<<"crRackCharacterMeshMethod(): file open error,"<<m_fileName<<std::endl;
				return;
			}

			if(cfg_script.Push("Character"))
			{
				std::string str;
				Mesh *mesh;
				int nMesh = 1;
				int int1;
				std::string currentPartMeshName;
				std::vector<Mesh *> addMeshVec;
				std::vector<Mesh *>::iterator addMeshItr;
				while(cfg_script.Push("Mesh",nMesh++))
				{
					cfg_script.Get("partid", int1);
					currentPartMeshName = currentSelectNode->getCurrentPartMeshName(int1);
					cfg_script.Get("name", str);
					if(currentPartMeshName.compare(str)==0) 
					{
                        currentSelectNode->attachMesh(str);
						continue;
					}
					if(currentPartMeshName.compare("NULL")!=0)
						currentSelectNode->deleteMesh(currentPartMeshName);
					mesh = new Mesh;
					mesh->setPartID(int1);
					mesh->setMaterialFileName(m_fileName);
					mesh->setName(str);
					cfg_script.Get("filename", str);
					mesh->setFilename(str);
					mesh->setIsDefault(false);

					addMeshVec.push_back(mesh);
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"crRackCharacterCallback::operator(): "<<cfg_script.GetLastError()<<std::endl;
				}

				if(!addMeshVec.empty())
				{
					std::string path;
					cfg_script.Get("path", path);

					currentSelectNode->getBody()->getCoreBody()->setPath(path);
					for( addMeshItr = addMeshVec.begin();
						addMeshItr != addMeshVec.end();
						++addMeshItr )
						currentSelectNode->addMesh(*addMeshItr/*,0materialsetid*/);

					cfg_script.Pop();
				}
                cfg_script.Pop();
			}
		}
	}
}
////////////////////////////////
//
//crLoadAnimMethod
//
///////////////////////////////////
crLoadAnimMethod::crLoadAnimMethod()
{}

crLoadAnimMethod::crLoadAnimMethod(const crLoadAnimMethod& handle):
crMethod(handle),
m_fileName(handle.m_fileName)
{
}

void crLoadAnimMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 3:
		if(param) m_fileName = *((std::string *)param);
		else m_fileName.clear();
		break;
	}
}

void crLoadAnimMethod::addParam(int i, const std::string& str)
{
}

void crLoadAnimMethod::loadAttributesCommonToAllAnimations(rcfg::ConfigScript &cfg_script, AnimationBase *anim)
{
	std::string str;
	float f_value;

	if (!cfg_script.Get("name", str))
	{
		CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAttributesCommonToAllAnimations(): "<<cfg_script.GetLastError()<<std::endl;
		return;
	}
	anim->setName(str);

	if (!cfg_script.Get("act_name", str))
	{
		CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAttributesCommonToAllAnimations(): "<<cfg_script.GetLastError()<<std::endl;
		return;
	}
	anim->setActName(str);

	if (!cfg_script.Get("act_weight", f_value)) 
		anim->setWeight(1.0f);
	else
		anim->setWeight(f_value);

	anim->setIsDefault(false);
}


void crLoadAnimMethod::operator()(crHandle &handle)
{
	if(m_fileName.empty()) return;
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		CreBodyNode *currentSelectNode = dynamic_cast<CreBodyNode *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			rcfg::ConfigScript cfg_script;
			if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_fileName))
			{
				CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod(): file open error,"<<m_fileName<<std::endl;
				return;
			}

			if(cfg_script.Push("Character"))
			{
				std::string str;
				std::string path;
				cfg_script.Get("path", path);
				currentSelectNode->getBody()->getCoreBody()->setPath(path);

				float f_value;
				int i_value;
				int i = 1;
				{
					//////////////////////////////Animation
					Animation *anim;
					while (cfg_script.Push("Animation",i)) 
					{
						anim = new Animation();

						loadAttributesCommonToAllAnimations(cfg_script,anim);

						if (!cfg_script.Get("filename", str))
						{
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
							cfg_script.Pop();
							continue;
						}
						anim->setFilename(str);

						if (cfg_script.Get("speed", f_value)) 
							anim->setSpeed(f_value);
						else
							anim->setSpeed(1.0f);

						if (cfg_script.Get("is_looped", i_value)) 
							anim->setIsLooped(i_value);
						else
							anim->setIsLooped(false);

						currentSelectNode->addAnimation(anim);

						i++;
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
					}
				}
				i = 1;
				{
					///////////////////////AnimationSpeed
					AnimationSpeed *animSpeed;
					while (cfg_script.Push("AnimationSpeed",i)) 
					{
						animSpeed = new AnimationSpeed();

						loadAttributesCommonToAllAnimations(cfg_script,animSpeed);

						int k = 1;
						while (cfg_script.Push("Animation", k) && (k < 3)) 
						{
							if (!cfg_script.Get("name", str))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}
							animSpeed->addAnimation(str);

							k++;

							if (!cfg_script.Pop())
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
						}

						currentSelectNode->addAnimation(animSpeed);

						i++;

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;

					}
				}
				i = 1;
				{
					///////////////////////AnimationDegree
					AnimationDegree *animDegree;
					while (cfg_script.Push("AnimationDegree", i)) 
					{
						animDegree = new AnimationDegree();

						loadAttributesCommonToAllAnimations(cfg_script,animDegree);

						int k = 1;
						while (cfg_script.Push("Animation", k) && (k < 3)) 
						{
							if (!cfg_script.Get("name", str))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}

							if (!cfg_script.Get("degree", f_value))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}

							animDegree->addAnimation(str,f_value);
							k++;

							if (!cfg_script.Pop())
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
						}

						currentSelectNode->addAnimation(animDegree);
						i++;

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
					}
				}
				i = 1;
				{
					////////////////////////////////////////AnimationBlend
					AnimationBlend *animBlend;
					while (cfg_script.Push("AnimationBlend", i)) 
					{
						animBlend = new AnimationBlend();

						loadAttributesCommonToAllAnimations(cfg_script,animBlend);

						int k = 1;
						while (cfg_script.Push("Animation", k)) 
						{
							if (!cfg_script.Get("name", str))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}
							if (!cfg_script.Get("weight", f_value))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}

							animBlend->addAnimation(str,f_value);

							k++;

							if (!cfg_script.Pop())
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
						}

						currentSelectNode->addAnimation(animBlend);

						i++;

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
					}
				}
				i = 1;
				{
					////////////////////////////////////////AnimationScript
					AnimationScript *animScript;
					while (cfg_script.Push("AnimationScript", i)) 
					{
						animScript = new AnimationScript();
						loadAttributesCommonToAllAnimations(cfg_script,animScript);

						int k = 1;
						while (cfg_script.Push("Animation", k)) 
						{
							if (!cfg_script.Get("name", str))
							{
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
								cfg_script.Pop();
								continue;
							}

							if (!cfg_script.Get("degree", f_value))
								f_value = 0.0f;

							animScript->addAnimation(str,f_value);

							k++;

							if (!cfg_script.Pop())
								CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
						}

						currentSelectNode->addAnimation(animScript);

						i++;

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crLoadAnimMethod::loadAnimations(): "<<cfg_script.GetLastError()<<std::endl;
					}
				}
				cfg_script.Pop();
			}
		}
	}
}
////////////////////////////////
//
//crSelectNodeTypeCaseMethod
//
///////////////////////////////////
crSelectNodeTypeCaseMethod::crSelectNodeTypeCaseMethod()
{}

crSelectNodeTypeCaseMethod::crSelectNodeTypeCaseMethod(const crSelectNodeTypeCaseMethod& handle):
crMethod(handle)
{
}

void crSelectNodeTypeCaseMethod::inputParam(int i, void *param)
{
}

void crSelectNodeTypeCaseMethod::addParam(int i, const std::string& str)
{
}

void crSelectNodeTypeCaseMethod::operator()(crHandle &handle)
{
	int _case = 0;
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *currentSelectNode = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			void *param;
			crData *infoData = gamebodyInfo->getDataClass();
			infoData->getParam(EDP_TerEditMode,param);
			unsigned char terraformMode = *((unsigned char*)param);
			if(terraformMode>crEditorInfoData::TER_Pick && terraformMode<crEditorInfoData::TER_ControlR)
			{
				_case = 1;
			}
			else if(terraformMode>=crEditorInfoData::TER_ControlR && terraformMode<=crEditorInfoData::TER_ControlA)
			{
				_case = 2;
			}
			else
			{
				_case = 3;
			}
		}
	}
	if(_case == 0)
	{
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited() && bindview->isCursorOnShow())
		{
			_case = 4;
		}
	}
	handle.outputParam(0,&_case);
}
////////////////////////////////
//
//crTerrainEditMethod
//
///////////////////////////////////
crTerrainEditMethod::crTerrainEditMethod()
{}

crTerrainEditMethod::crTerrainEditMethod(const crTerrainEditMethod& handle):
crMethod(handle)
{
}

void crTerrainEditMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crTerrainEditMethod::addParam(int i, const std::string& str)
{
}

void crTerrainEditMethod::operator()(crHandle &handle)
{
	crGameBodyInfo *gamebodyInfo = m_this->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *terrain = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(terrain)
		{
			void *param;
			crData *infoData = gamebodyInfo->getDataClass();
			infoData->getParam(EDP_TerEditMode,param);
			unsigned char terraformMode = *((unsigned char*)param);
			if(terraformMode>crEditorInfoData::TER_Pick && terraformMode<crEditorInfoData::TER_ControlR)
			{
				CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(terrain->getElevationLayer());
				if(hfl)
				{
					crHeightField *grid = hfl->getHeightField();
					crImage *image = grid->getHeightImage();
					if(!image)
					{
						image = CRIOManager::readImageFile(grid->getHeightMap());
						grid->setHeightImage(image);
					}
					if(image)
					{
						crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();

						CRCore::crIntersectVisitor::HitList hits;
						bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),terrain,hits);
						if(!hits.empty())
						{
							infoData->getParam(EDP_TerToolRadius,param);
							int toolRadius = *((int*)param);
							infoData->getParam(EDP_TerToolHeight,param);
							float toolHeight = *((float*)param);

							crVector3 hitPoint = hits[0].getLocalIntersectPoint();
                            //显示编辑框
							m_this->doEvent(WCH_ShowTerrainEditDecal,MAKEINT64(terrain,&hitPoint));

							infoData->getParam(EDP_CurrentTerTool,param);
							unsigned char currentTerTool = *((unsigned char*)param);

							infoData->getParam(EDP_TerTools,param);
							crEditorInfoData::TerToolVec *terTools = (crEditorInfoData::TerToolVec *)param;
							crImage *toolImage = (*terTools)[currentTerTool].get();
							if(toolImage)
							{
								crVector3 origin = grid->getOrigin();
								crVector3 pos(hitPoint[0],hitPoint[1],0.0f);
								pos -= origin;
								pos[0] /= grid->getXInterval();
								pos[1] /= grid->getYInterval();

								float tools = float(toolImage->s())/float(toolRadius);
								float ws = float(grid->getNumColumns()) / float(image->s());
								float ts = float(grid->getNumRows()) / float(image->t());

								pos[0] /= ws;
								pos[1] /= ts;

								if(pos[0]<0||pos[0]>image->s()||pos[1]<0||pos[1]>image->t())
									return;

								AddColorOperator addOp;
								LerpColorOperator lerpOp;
								lerpOp.setPickl(image->getColor(pos[0],pos[1],0)[0]);

								float heightScale = toolHeight/grid->getMaxHeight();
								if(heightScale>1.0f)
									heightScale = 1.0f;
								if(terraformMode == crEditorInfoData::TER_Lower)
								{
									heightScale = -heightScale;
								}
								int r_2 = toolRadius>>1;
								float l;
								int s1,t1;

								for(int s = 0; s<toolRadius; ++s)
								{
									for(int t=0;t<toolRadius;++t)
									{
										s1 = s+pos[0]-r_2;
										t1 = t+pos[1]-r_2;
										if(s1<0||s1>image->s()||t1<0||t1>image->t())
											continue;
										if(terraformMode == crEditorInfoData::TER_Lerp)
										{
											l = toolImage->getColor(s * tools,t * tools)[0];
											lerpOp.setTooll(l);

											CRCore::modifyRow(image->r(), image->getPixelFormat(), image->getDataType(), image->data(s1,t1,0), lerpOp);
										}
										else
										{
											l = toolImage->getColor(s * tools,t * tools)[0];
											l *= heightScale;
											addOp.setLuminance(l);

											CRCore::modifyRow(image->r(), image->getPixelFormat(), image->getDataType(), image->data(s1,t1,0), addOp);
										}
										l = image->getColor(s1,t1,0)[0];
										l -= 0.5f;//高度偏移
										grid->setHeight(s1 * ws,t1 * ts,l * grid->getMaxHeight());
									}
								}

								if(!hfl->getLocator())
								{
									ref_ptr<CRTerrain::crLocator> locator = new CRTerrain::crLocator;
									locator->setCoordinateSystemType(CRTerrain::crLocator::PROJECTED);
									locator->setTransformAsExtents(0,0, grid->getXInterval() * grid->getNumColumns(), grid->getYInterval() * grid->getNumRows());
									hfl->setLocator(locator.get());
								}
								if(!hfl->getValidDataOperator())
								{
									ref_ptr<CRTerrain::crValidDataOperator> validDataOperator = new CRTerrain::crNoDataValue(0.0);
									terrain->setTreatBoundariesToValidDataAsDefaultValue(true);
									hfl->setValidDataOperator(validDataOperator.get());
								}

								ref_ptr<CRTerrain::crImageLayer> imageLayer;
								if(terrain->getNumColorLayers()==0)
								{
									imageLayer = new CRTerrain::crImageLayer;
									imageLayer->setLocator(hfl->getLocator());
									imageLayer->setValidDataOperator(hfl->getValidDataOperator());
									terrain->setColorLayer(0,imageLayer.get());
								}
								else
								{
									imageLayer = dynamic_cast<CRTerrain::crImageLayer *>(terrain->getColorLayer(0));
								}
								imageLayer->setImage(image);

								ref_ptr<CRTerrain::crGeometryTechnique> geometryTechnique;
								if(!terrain->getTerrainTechnique())
								{
									geometryTechnique = new CRTerrain::crGeometryTechnique;
									terrain->setTerrainTechnique(geometryTechnique.get());
								}
								else
								{
									geometryTechnique = dynamic_cast<CRTerrain::crGeometryTechnique *>(terrain->getTerrainTechnique());
								}
								geometryTechnique->setEditMode(true);

								//grid->buildFromHeightMap(image,grid->getMaxHeight());
								terrain->setDirty(true);
							}
						}
					}
				}
			}
		}
	}
}
////////////////////////////////
//
//crColorWeightEditMethod
//
///////////////////////////////////
crColorWeightEditMethod::crColorWeightEditMethod()
{}

crColorWeightEditMethod::crColorWeightEditMethod(const crColorWeightEditMethod& handle):
crMethod(handle)
{
}

void crColorWeightEditMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crColorWeightEditMethod::addParam(int i, const std::string& str)
{
}

void crColorWeightEditMethod::operator()(crHandle &handle)
{
	crGameBodyInfo *gamebodyInfo = m_this->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *terrain = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(terrain)
		{
			void *param;
			crData *infoData = gamebodyInfo->getDataClass();
			infoData->getParam(EDP_TerEditMode,param);
			unsigned char terraformMode = *((unsigned char*)param);
			if(terraformMode>=crEditorInfoData::TER_ControlR && terraformMode<=crEditorInfoData::TER_ControlA)
			{
				crStateSet *ter_ss = NULL;
				CRCore::crMatrixTransform *transform = dynamic_cast<CRCore::crMatrixTransform *>(terrain->getChild(0));
				if(transform)
				{
					crObject *object = dynamic_cast<CRCore::crObject *>(transform->getChild(0));
					crGeometry *geometry = dynamic_cast<CRCore::crGeometry *>(object->getDrawable(0));
					ter_ss = geometry->getStateSet();
				}
				crTexture2D *colorWeightTex = NULL;
				if(ter_ss)
				{
					crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(ter_ss->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
					if(externTex2D)
					{
						if(externTex2D->getExternTextureID() == crTexture::COLORWEIGHT)
						{
							colorWeightTex = externTex2D;
						}
					}
				}
                if(colorWeightTex)
				{
					crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
					CRCore::crIntersectVisitor::HitList hits;
					bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),terrain,hits);
					if(!hits.empty())
					{
						infoData->getParam(EDP_TerToolRadius,param);
						int toolRadius = *((int*)param);
						infoData->getParam(EDP_TerToolHeight,param);
						float toolHeight = *((float*)param);

						crVector3 hitPoint = hits[0].getLocalIntersectPoint();
						//显示编辑框
						m_this->doEvent(WCH_ShowTerrainEditDecal,MAKEINT64(terrain,&hitPoint));

						ref_ptr<crImage> colorWeightImage = colorWeightTex->getImage();
						if(!colorWeightImage.valid())
						{
							colorWeightImage = CRIOManager::readImageFile(colorWeightTex->getImageNameID());
							colorWeightTex->setUnRefImageDataAfterApply(false);
						}
						CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(terrain->getElevationLayer());
						if(colorWeightImage.valid() && hfl)
						{
							crHeightField *grid = hfl->getHeightField();

							infoData->getParam(EDP_CurrentTerTool,param);
							unsigned char currentTerTool = *((unsigned char*)param);

							infoData->getParam(EDP_TerTools,param);
							crEditorInfoData::TerToolVec *terTools = (crEditorInfoData::TerToolVec *)param;
							crImage *toolImage = (*terTools)[currentTerTool].get();
							if(toolImage)
							{
								if(colorWeightImage->isCompressed())
								{
									colorWeightTex->setUnRefImageDataAfterApply(true);
									CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::crColorWeightEditMethod(): 该贴图格式不运行编辑，如需编辑请使用tga格式做控制贴图"<<std::endl;
									return;
								}

								crVector3 origin = grid->getOrigin();
								//crMatrix matrix = layer->getRotationMatrix();
								//matrix.setTrans(origin);
								crVector3 pos(hitPoint[0],hitPoint[1],0.0f);
								pos -= origin;
								pos[0] /= grid->getXInterval();
								pos[1] /= grid->getYInterval();

								float tools = float(toolImage->s())/float(toolRadius);
								float ws = float(grid->getNumColumns()) / float(colorWeightImage->s());
								float ts = float(grid->getNumRows()) / float(colorWeightImage->t());
								pos[0] /= ws;
								pos[1] /= ts;

								float toolstrength = toolHeight/255.0f;

								ColorWeightOperator cwOp;
								crVector4 cw;

								int r_2 = toolRadius>>1;
								float l;
								int s1,t1;
								for(int s = 0; s<toolRadius; ++s)
								{
									for(int t=0;t<toolRadius;++t)
									{
										s1 = s+pos[0]-r_2;
										t1 = t+pos[1]-r_2;
										if(s1<0||s1>colorWeightImage->s()||t1<0||t1>colorWeightImage->t())
											continue;

										l = toolImage->getColor(s * tools,t * tools)[0];
										cw.set(0,0,0,0);
										switch(terraformMode)
										{
										case crEditorInfoData::TER_ControlR:
                                            cw[0] = toolstrength;
											break;
										case crEditorInfoData::TER_ControlG:
											cw[1] = toolstrength;
											break;
										case crEditorInfoData::TER_ControlB:
											cw[2] = toolstrength;
											break;
										case crEditorInfoData::TER_ControlA:
											cw[3] = toolstrength;
											break;
										}
										cwOp.setWeight(cw);
										cwOp.setTooll(l);

										CRCore::modifyRow(colorWeightImage->r(), colorWeightImage->getPixelFormat(), colorWeightImage->getDataType(), colorWeightImage->data(s1,t1,0), cwOp);
									}
								}
								colorWeightImage->setModifiedCount(colorWeightImage->getModifiedCount()+1);
                                colorWeightTex->setImage(colorWeightImage.get());
							}
						}
					}
				}
			}
		}
	}
}
////////////////////////////////
//
//crTerrainEditDecalMethod
//
///////////////////////////////////
crTerrainEditDecalMethod::crTerrainEditDecalMethod()
{}

crTerrainEditDecalMethod::crTerrainEditDecalMethod(const crTerrainEditDecalMethod& handle):
crMethod(handle)
{
}

void crTerrainEditDecalMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crTerrainEditDecalMethod::addParam(int i, const std::string& str)
{
}

void crTerrainEditDecalMethod::operator()(crHandle &handle)
{
	crGameBodyInfo *gamebodyInfo = m_this->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *terrain = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(terrain)
		{
			CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(terrain->getElevationLayer());
			if(hfl)
			{
				crHeightField *grid = hfl->getHeightField();
				crImage *image = grid->getHeightImage();
				if(!image)
				{
					image = CRIOManager::readImageFile(grid->getHeightMap());
					grid->setHeightImage(image);
				}
				if(image)
				{
					crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();

					CRCore::crIntersectVisitor::HitList hits;
					bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),terrain,hits);
					if(!hits.empty())
					{
						crVector3 hitPoint = hits[0].getLocalIntersectPoint();
						//显示编辑框
						m_this->doEvent(WCH_ShowTerrainEditDecal,MAKEINT64(terrain,&hitPoint));
					}
				}
			}
		}
	}
}
////////////////////////////////
//
//crSaveHeightMapMethod
//
///////////////////////////////////
crSaveHeightMapMethod::crSaveHeightMapMethod()
{}

crSaveHeightMapMethod::crSaveHeightMapMethod(const crSaveHeightMapMethod& handle):
crMethod(handle),
m_path(handle.m_path),
m_heightMap(handle.m_heightMap)
{
}

void crSaveHeightMapMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveHeightMapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_path = str;
		break;
	case 1:
		m_heightMap = str;
		break;
	}
}

void crSaveHeightMapMethod::operator()(crHandle &handle)
{
	std::string fileName;
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crEditBoxElement *heightMapEdit = dynamic_cast<crEditBoxElement *>(stage->getElement(m_heightMap));
		if(heightMapEdit)
		{
			fileName = heightMapEdit->getStringArrayInString();
		}
	}
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *terrain = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(terrain)
		{
			CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(terrain->getElevationLayer());
			if(hfl)
			{
				crHeightField *grid = hfl->getHeightField();
				crImage *image = grid->getHeightImage();
				if(!image)
				{
					image = CRIOManager::readImageFile(grid->getHeightMap());
					grid->setHeightImage(image);
				}
				if(image)
				{
					if(fileName.empty())
					{
						fileName = grid->getHeightMap();
					}
					fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".img";

					grid->setHeightMapName(fileName);
					CRIOManager::writeImageFile(*image,m_path+fileName);
				}
			}
		}
	}
}
////////////////////////////////
//
//crSaveColorWeightMapMethod
//
///////////////////////////////////
crSaveColorWeightMapMethod::crSaveColorWeightMapMethod()
{}

crSaveColorWeightMapMethod::crSaveColorWeightMapMethod(const crSaveColorWeightMapMethod& handle):
crMethod(handle),
m_path(handle.m_path),
m_colorWeightMap(handle.m_colorWeightMap)
{
}

void crSaveColorWeightMapMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveColorWeightMapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_path = str;
		break;
	case 1:
		m_colorWeightMap = str;
		break;
	}
}

void crSaveColorWeightMapMethod::operator()(crHandle &handle)
{
	std::string fileName;
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crEditBoxElement *colorWeightMapEdit = dynamic_cast<crEditBoxElement *>(stage->getElement(m_colorWeightMap));
		if(colorWeightMapEdit)
		{
			fileName = colorWeightMapEdit->getStringArrayInString();
		}
	}
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *terrain = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(terrain)
		{
			crStateSet *ter_ss = NULL;
			CRCore::crMatrixTransform *transform = dynamic_cast<CRCore::crMatrixTransform *>(terrain->getChild(0));
			if(transform)
			{
				crObject *object = dynamic_cast<CRCore::crObject *>(transform->getChild(0));
				crGeometry *geometry = dynamic_cast<CRCore::crGeometry *>(object->getDrawable(0));
				ter_ss = geometry->getStateSet();
			}
			crTexture2D *colorWeightTex = NULL;
			if(ter_ss)
			{
				crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(ter_ss->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
				if(externTex2D)
				{
					if(externTex2D->getExternTextureID() == crTexture::COLORWEIGHT)
					{
						colorWeightTex = externTex2D;
					}
				}
			}
			if(colorWeightTex)
			{
				ref_ptr<crImage> colorWeightImage = colorWeightTex->getImage();
				//if(!colorWeightImage.valid())
				//{
				//	colorWeightImage = CRIOManager::readImageFile(colorWeightTex->getImageNameID());
				//}
				if(colorWeightImage.valid() && !colorWeightImage->isCompressed())
				{
					if(fileName.empty())
					{
						fileName = colorWeightImage->getFileName();
					}
					fileName = CRCore::crArgumentParser::getFileNameEliminateExt(fileName) + ".tga";

					colorWeightImage->setFileName(fileName);
					colorWeightTex->setImageNameID(fileName);
					CRIOManager::writeImageFile(*colorWeightImage,m_path+fileName);
				}
			}
		}
	}
}
////////////////////////////////
//
//crShowTerrainEditDecalMethod
//
///////////////////////////////////
crShowTerrainEditDecalMethod::crShowTerrainEditDecalMethod()
{}

crShowTerrainEditDecalMethod::crShowTerrainEditDecalMethod(const crShowTerrainEditDecalMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName),
m_textureName(handle.m_textureName)
{
}

void crShowTerrainEditDecalMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_terrain = NULL;
		}
		break;
	case 1:
		m_this = (crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_terrain = (crTerrainTile *)(LOINT64(param64));
			m_hitPoint = *((crVector3 *)(HIINT64(param64)));
		}
		break;
	}
}

void crShowTerrainEditDecalMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_rootName = str;
		break;
	case 1:
		m_textureName = str;
		break;
	}
}

void crShowTerrainEditDecalMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_root)
	{
		crGameBodyInfo *gamebodyInfo = m_this->getGameBodyInfo();
		if(m_terrain.valid())
		{
			CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(m_terrain->getElevationLayer());
			if(hfl)
			{
				crHeightField *grid = hfl->getHeightField();
				crImage *image = grid->getHeightImage();
				if(!image)
				{
					image = CRIOManager::readImageFile(grid->getHeightMap());
					grid->setHeightImage(image);
				}
				if(image)
				{
					void *param;
					crData *infoData = gamebodyInfo->getDataClass();

					infoData->getParam(EDP_TerToolRadius,param);
					int toolRadius = *((int*)param);

					float ws = float(grid->getNumColumns()) / float(image->s());
					float ts = float(grid->getNumRows()) / float(image->t());

					crVector3 origin = grid->getOrigin();
					crVector3 pos(m_hitPoint[0],m_hitPoint[1],0.0f);

					if(!m_heightField.valid())
						m_heightField = new crHeightField;
					int numColumns = toolRadius * ws;
					int numRows = toolRadius * ts;
					if(numColumns<2) numColumns = 2;
					if(numRows<2) numRows = 2;
					int c_2 = numColumns>>1;
					int r_2 = numRows>>1;
					m_heightField->allocate(numColumns,numRows);
					m_heightField->setXInterval(grid->getXInterval());
					m_heightField->setYInterval(grid->getYInterval());
					m_heightField->setOrigin(crVector3(pos[0] - c_2 * grid->getXInterval(),pos[1] - r_2 * grid->getYInterval(),0.0f));

					pos -= origin;
					pos[0] /= grid->getXInterval();
					pos[1] /= grid->getYInterval();
					int s1,t1;
					for(int s = 0; s<numColumns; ++s)
					{
						for(int t=0;t<numRows;++t)
						{
							s1 = s + pos[0] - c_2;
							t1 = t + pos[1] - r_2;
							m_heightField->setHeight(s,t,grid->getHeight(s1,t1));
						}
					}
					if(!m_shapeDrawable.valid())
					{
						m_shapeDrawable = new crShapeDrawable(m_heightField.get());
						m_shapeDrawable->setColor(crVector4(1.0f,0.0f,0.0f,1.0f));

						crStateSet *stateset = m_shapeDrawable->getOrCreateStateSet();
						stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
						stateset->setMode(GL_CULL_FACE,crStateAttribute::OFF);
						stateset->setMode(GL_DEPTH_TEST,CRCore::crStateAttribute::OFF);

						//stateset->setRenderBinDetails(1,"StateSortedBin");
						//crLineWidth *lineWidth = new crLineWidth(1.0f);
						//stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
						//crPolygonMode *polygonMode = new crPolygonMode;
						//polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
						//stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
						/*crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
						stateset->setAttributeAndModes(depth,crStateAttribute::ON);*/

						crTexture2D *tex2d = new crTexture2D;
						tex2d->setImage(CRIOManager::readImageFile(m_textureName));
						stateset->setTextureAttributeAndModes(TEXTURE_BASEMAP,tex2d,crStateAttribute::ON);
						crAlphaFunc *alphaFunc = new crAlphaFunc;
						alphaFunc->setFunction(crAlphaFunc::GREATER,0.5f);
						stateset->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
						stateset->setRenderingHint(CRCore::crStateSet::ALPHATEST_BIN);
						m_shapeDrawable->setUseDisplayList(false);
					}
					if(!m_object)
					{
						m_object = new crObject;
						m_object->addDrawable(m_shapeDrawable.get());
						m_object->setCollideMode(crNode::CollideNone);
						m_object->setName("TerrainEditDecal");
						m_object->setEditorIntersectable(false);
						m_object->setEffectByShadow(false);
						m_object->setCalcShadow(false);
						CRCore::crTraverseStringNode *tsNode = new CRCore::crTraverseStringNode;
						tsNode->setName("NoLight");
						tsNode->addChild(m_object.get());
						CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root,tsNode,true);
					}
					m_object->setEditorHidden(false);
					m_shapeDrawable->dirtyBound();
				}
			}
		}
	}
}
////////////////////////////////
//
//crHideTerrainEditDecalMethod
//
///////////////////////////////////
crHideTerrainEditDecalMethod::crHideTerrainEditDecalMethod()
{}

crHideTerrainEditDecalMethod::crHideTerrainEditDecalMethod(const crHideTerrainEditDecalMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName)
{
}

void crHideTerrainEditDecalMethod::inputParam(int i, void *param)
{
}

void crHideTerrainEditDecalMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_rootName = str;
		break;
	}
}

void crHideTerrainEditDecalMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_root)
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId("TerrainEditDecal");
		searchByNameVisitor.setSearchNodeType(OBJECT);
		m_root->accept(searchByNameVisitor);
		crObject *object = dynamic_cast<crObject *>(searchByNameVisitor.getResult());
		if(object)
			object->setEditorHidden(true);
	}
}
/////////////////////////////////////////
//
//crMoveWidgetMethod
//
/////////////////////////////////////////
crMoveWidgetMethod::crMoveWidgetMethod():
m_time(0.0f)
{
}

crMoveWidgetMethod::crMoveWidgetMethod(const crMoveWidgetMethod& handle):
crMethod(handle),
m_time(0.0f)
{
}

void crMoveWidgetMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crMoveWidgetMethod::addParam(int i, const std::string& str)
{
}
void crMoveWidgetMethod::operator()(crHandle &handle)
{
	if(m_ea.valid())
	{
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		float dt = m_ea->time() - m_time;
		if(m_time == 0.0f || dt>0.5f)
		{
			m_time = m_ea->time();
			m_lastMouse = mouse;
			return;
		}

		crVector2 delta = mouse - m_lastMouse;
		crVector3 trans = m_this->getTrans();
		trans[0] += delta[0];
		trans[1] += delta[1];
		m_this->setPosition(trans);
		m_lastMouse = mouse;
	}
}
/////////////////////////////////////////
//
//crMoveParentCanvasMethod
//
/////////////////////////////////////////
crMoveParentCanvasMethod::crMoveParentCanvasMethod():
m_time(0.0f)
{
}

crMoveParentCanvasMethod::crMoveParentCanvasMethod(const crMoveParentCanvasMethod& handle):
crMethod(handle),
m_time(0.0f)
{
}

void crMoveParentCanvasMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crMoveParentCanvasMethod::addParam(int i, const std::string& str)
{
}
void crMoveParentCanvasMethod::operator()(crHandle &handle)
{
	if(m_ea.valid())
	{
		crCanvasNode *parentCanvas = m_this->getParentCanvas();
		if(parentCanvas)
		{
			crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
			float dt = m_ea->time() - m_time;
			if(m_time == 0.0f || dt>0.5f)
			{
				m_time = m_ea->time();
				m_lastMouse = mouse;
				return;
			}
			float m_delta = 0.01;
			bool mousemove = true;
			if(mouse[0]<-1.0f+m_delta)
			{
				mousemove = false;
			}
			else if(mouse[0]>1.0f-m_delta)
			{
				mousemove = false;
			}
			if(mouse[1]<-1.0f+m_delta)
			{
				mousemove = false;
			}
			else if(mouse[1]>1.0f-m_delta)
			{
				mousemove = false;
			}
			if(mousemove)
			{
				crVector2 delta = mouse - m_lastMouse;
				crVector3 trans = parentCanvas->getTrans();
				trans[0] += delta[0];
				trans[1] += delta[1];
				parentCanvas->setPosition(trans);
				m_lastMouse = mouse;
			}
		}
	}
}
////////////////////////////////
//
//crCloseParentCanvasMethod
//
///////////////////////////////////
crCloseParentCanvasMethod::crCloseParentCanvasMethod()
{}

crCloseParentCanvasMethod::crCloseParentCanvasMethod(const crCloseParentCanvasMethod& handle):
crMethod(handle)
{
}

void crCloseParentCanvasMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == NULL)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crCloseParentCanvasMethod::addParam(int i, const std::string& str)
{
}

void crCloseParentCanvasMethod::operator()(crHandle &handle)
{
	if(m_this->getParentCanvas())
	{
		crFilterRenderManager::getInstance()->closeCanvas(m_this->getParentCanvas());
	}
}
////////////////////////////////
//
//crCloseMeMethod
//
///////////////////////////////////
crCloseMeMethod::crCloseMeMethod()
{}

crCloseMeMethod::crCloseMeMethod(const crCloseMeMethod& handle):
	crMethod(handle)
{
}

void crCloseMeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == NULL)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crCloseMeMethod::addParam(int i, const std::string& str)
{
}

void crCloseMeMethod::operator()(crHandle &handle)
{
	crFilterRenderManager::getInstance()->closeCanvas(m_this);
}
////////////////////////////////
//
//crWidgetShowMethod
//
///////////////////////////////////
crWidgetShowMethod::crWidgetShowMethod():
m_show(0)
{}

crWidgetShowMethod::crWidgetShowMethod(const crWidgetShowMethod& handle):
crMethod(handle),
m_show(handle.m_show)
{
}

void crWidgetShowMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crWidgetShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_show = atoi(str.c_str());
		break;
	}
}

void crWidgetShowMethod::operator()(crHandle &handle)
{
	crMultiSwitch *node = dynamic_cast<crMultiSwitch *>(m_this->getChild(0));
	node->setActiveSwitchSet(m_show);
}
////////////////////////////////
//
//crShowCanvasMethod
//
///////////////////////////////////
crShowCanvasMethod::crShowCanvasMethod():
m_show(1)
{}

crShowCanvasMethod::crShowCanvasMethod(const crShowCanvasMethod& handle):
crMethod(handle),
m_canvas(handle.m_canvas),
m_show(handle.m_show)
{
}

void crShowCanvasMethod::inputParam(int i, void *param)
{
}

void crShowCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_show = (char)(atoi(str.c_str()));
		break;
	}
}

void crShowCanvasMethod::operator()(crHandle &handle)
{
	switch(m_show)
	{
	case 0:
		crFilterRenderManager::getInstance()->showCanvas(m_canvas,false);
		break;
	case 1:
		crFilterRenderManager::getInstance()->showCanvas(m_canvas,true);
		break;
	case 2:
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),m_canvas,canvas->getVisiable()?SH_Hide:SH_Show);
			}
			else
			{
				crFilterRenderManager::getInstance()->showCanvas(m_canvas,true);
			}
		}
		break;
	case 3:
		crFilterRenderManager::getInstance()->doModal(m_canvas);
		break;
	case 4:
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crFilterRenderManager::getInstance()->doModal(canvas.get());
			}
		}
		break;
	}
}
////////////////////////////////
//
//crWidgetUpdateMethod
//
///////////////////////////////////
crWidgetUpdateMethod::crWidgetUpdateMethod():
	m_this(NULL)
{}

crWidgetUpdateMethod::crWidgetUpdateMethod(const crWidgetUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_widgetName(handle.m_widgetName)
{
}

void crWidgetUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crWidgetUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_widgetName = str;
		break;
	}
}

void crWidgetUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		ref_ptr<crWidgetNode>widget = canvas->getWidget(m_widgetName);
		if(widget.valid()) widget->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crWidgetUpdate2Method
//
///////////////////////////////////
crWidgetUpdate2Method::crWidgetUpdate2Method():
	m_this(NULL)
{}

crWidgetUpdate2Method::crWidgetUpdate2Method(const crWidgetUpdate2Method& handle):
	crMethod(handle),
	m_this(NULL),
	m_widgetName(handle.m_widgetName)
{
}

void crWidgetUpdate2Method::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crWidgetUpdate2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_widgetName = str;
		break;
	}
}

void crWidgetUpdate2Method::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crWidgetNode> widget = m_this->getWidget(m_widgetName);
		if(widget.valid()) widget->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crWidgetUpdate3Method
//
///////////////////////////////////
crWidgetUpdate3Method::crWidgetUpdate3Method()
{}

crWidgetUpdate3Method::crWidgetUpdate3Method(const crWidgetUpdate3Method& handle):
	crMethod(handle),
	m_canvasName(handle.m_canvasName),
	m_widgetName(handle.m_widgetName)
{
}

void crWidgetUpdate3Method::inputParam(int i, void *param)
{
}

void crWidgetUpdate3Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_widgetName = str;
		break;
	}
}

void crWidgetUpdate3Method::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	if(canvas.valid())
	{
		ref_ptr<crWidgetNode>widget = canvas->getWidget(m_widgetName);
		if(widget.valid()) widget->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crCanvasUpdateMethod
//
///////////////////////////////////
crCanvasUpdateMethod::crCanvasUpdateMethod()
{}

crCanvasUpdateMethod::crCanvasUpdateMethod(const crCanvasUpdateMethod& handle):
	crMethod(handle),
	m_canvasName(handle.m_canvasName)
{
}

void crCanvasUpdateMethod::inputParam(int i, void *param)
{
}

void crCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	}
}

void crCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	if(canvas.valid() && canvas->getVisiable())
	{
		canvas->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crParentCanvasUpdateMethod
//
///////////////////////////////////
crParentCanvasUpdateMethod::crParentCanvasUpdateMethod():
	m_this(NULL)
{}

crParentCanvasUpdateMethod::crParentCanvasUpdateMethod(const crParentCanvasUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}

void crParentCanvasUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crParentCanvasUpdateMethod::addParam(int i, const std::string& str)
{
}

void crParentCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		crCanvasNode *canvas = m_this->getParentCanvas();
		canvas->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crUpdateMeMethod
//
///////////////////////////////////
crUpdateMeMethod::crUpdateMeMethod():
	m_this(NULL)
{}

crUpdateMeMethod::crUpdateMeMethod(const crUpdateMeMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}

void crUpdateMeMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crUpdateMeMethod::addParam(int i, const std::string& str)
{
}

void crUpdateMeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		m_this->doEvent(WCH_UI_UpdateData);
	}
}
////////////////////////////////
//
//crShowMouseCanvasMethod
//
///////////////////////////////////
crShowMouseCanvasMethod::crShowMouseCanvasMethod():
	m_show(false)
{}

crShowMouseCanvasMethod::crShowMouseCanvasMethod(const crShowMouseCanvasMethod& handle):
	crMethod(handle),
	m_show(handle.m_show)
{
}

void crShowMouseCanvasMethod::inputParam(int i, void *param)
{
}

void crShowMouseCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_show = (bool)(atoi(str.c_str()));
		break;
	}
}

void crShowMouseCanvasMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = crFilterRenderManager::getInstance()->getMouseCanvas();
	if(canvas) canvas->setVisiable(m_show);
}
////////////////////////////////
//
//crChangeMouseMethod
//
///////////////////////////////////
crChangeMouseMethod::crChangeMouseMethod():
	m_mouse(0)
{}

crChangeMouseMethod::crChangeMouseMethod(const crChangeMouseMethod& handle):
	crMethod(handle),
	m_mouse(handle.m_mouse)
{
}

void crChangeMouseMethod::inputParam(int i, void *param)
{
}

void crChangeMouseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_mouse = atoi(str.c_str());
		break;
	}
}

void crChangeMouseMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = crFilterRenderManager::getInstance()->getMouseCanvas();
	if(canvas)
	{
		crMultiSwitch *mouseSW = dynamic_cast<crMultiSwitch *>(canvas->getChild(0));
		if(mouseSW)
		{
			mouseSW->setActiveSwitchSet(m_mouse);
		}
	}
}
////////////////////////////////
//
//crAddTabWidgetMethod
//
///////////////////////////////////
crAddTabWidgetMethod::crAddTabWidgetMethod()
{}

crAddTabWidgetMethod::crAddTabWidgetMethod(const crAddTabWidgetMethod& handle):
	crMethod(handle),
	m_widgetName(handle.m_widgetName)
{
}

void crAddTabWidgetMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crAddTabWidgetMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_widgetName = str;
		break;
	}
}

void crAddTabWidgetMethod::operator()(crHandle &handle)
{
	m_this->addTabWidget(m_widgetName);
}
////////////////////////////////
//
//crSetMsgTraverseMethod
//
///////////////////////////////////
crSetMsgTraverseMethod::crSetMsgTraverseMethod():
	m_msgtraverse(KM_None)
{}

crSetMsgTraverseMethod::crSetMsgTraverseMethod(const crSetMsgTraverseMethod& handle):
	crMethod(handle),
	m_msgtraverse(handle.m_msgtraverse)
{
}

void crSetMsgTraverseMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crSetMsgTraverseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msgtraverse = atoi(str.c_str());
		break;
	}
}

void crSetMsgTraverseMethod::operator()(crHandle &handle)
{
	m_this->setMsgTraverse(m_msgtraverse);
}
/////////////////////////////////////////
//
//crWidgetDoEventMethod
//
/////////////////////////////////////////
crWidgetDoEventMethod::crWidgetDoEventMethod():
	m_msg(NULL),
	m_param(NULL),
	m_this(NULL)
{
}
crWidgetDoEventMethod::crWidgetDoEventMethod(const crWidgetDoEventMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_widget(handle.m_widget),
	m_msg(handle.m_msg),
	m_param(handle.m_param)
{
}
void crWidgetDoEventMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crWidgetDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_widget = str;
		break;
	case 1:
		m_msg = parseEventMsg16(str);
		break;
	case 2:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 3:
		m_param = atoi(str.c_str());
		break;
	case 4:
		m_param =  MAKEINT64(LOINT64(m_param),atoi(str.c_str()));
		break;
	}
}
void crWidgetDoEventMethod::operator()(crHandle &handle)
{
	ref_ptr<crWidgetNode> widget = m_this->getWidget(m_widget);
	if(widget.valid() && widget->getEnable())
		widget->doEvent(m_msg,m_param);
}
/////////////////////////////////////////
//
//crWidgetDoEvent2Method
//
/////////////////////////////////////////
crWidgetDoEvent2Method::crWidgetDoEvent2Method():
	m_msg(NULL),
	m_param(NULL)
{
}
crWidgetDoEvent2Method::crWidgetDoEvent2Method(const crWidgetDoEvent2Method& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_widget(handle.m_widget),
	m_msg(handle.m_msg),
	m_param(handle.m_param)
{
}
void crWidgetDoEvent2Method::inputParam(int i, void *param)
{
}
void crWidgetDoEvent2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_widget = str;
		break;
	case 2:
		m_msg = parseEventMsg16(str);
		break;
	case 3:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 4:
		m_param = atoi(str.c_str());
		break;
	case 5:
		m_param =  MAKEINT64(LOINT64(m_param),atoi(str.c_str()));
		break;
	}
}
void crWidgetDoEvent2Method::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crWidgetNode>widget = canvas->getWidget(m_widget);
		if(widget.valid() && widget->getEnable())
			widget->doEvent(m_msg,m_param);
	}
}
/////////////////////////////////////////
//
//crCanvasDoEventMethod
//
/////////////////////////////////////////
crCanvasDoEventMethod::crCanvasDoEventMethod():
	m_msg(NULL),
	m_param(NULL)
{
}
crCanvasDoEventMethod::crCanvasDoEventMethod(const crCanvasDoEventMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_msg(handle.m_msg),
	m_param(handle.m_param)
{
}
void crCanvasDoEventMethod::inputParam(int i, void *param)
{
}
void crCanvasDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_msg = parseEventMsg16(str);
		break;
	case 2:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 3:
		m_param = atoi(str.c_str());
		break;
	case 4:
		m_param =  MAKEINT64(LOINT64(m_param),atoi(str.c_str()));
		break;
	}
}
void crCanvasDoEventMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid() && canvas->getEnable())
	{
		canvas->doEvent(m_msg,m_param);
	}
}
/////////////////////////////////////////
//
//crParentCanvasDoEventMethod
//
/////////////////////////////////////////
crParentCanvasDoEventMethod::crParentCanvasDoEventMethod():
	m_this(NULL),
	m_msg(NULL),
	m_param(NULL)
{
}
crParentCanvasDoEventMethod::crParentCanvasDoEventMethod(const crParentCanvasDoEventMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_msg(handle.m_msg),
	m_param(handle.m_param)
{
}
void crParentCanvasDoEventMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crParentCanvasDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msg = parseEventMsg16(str);
		break;
	case 1:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 2:
		m_param = atoi(str.c_str());
		break;
	case 3:
		m_param =  MAKEINT64(LOINT64(m_param),atoi(str.c_str()));
		break;
	}
}
void crParentCanvasDoEventMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = m_this->getParentCanvas();
	if(canvas && canvas->getEnable())
	{
		canvas->doEvent(m_msg,m_param);
	}
}
/////////////////////////////////////////
//
//crChangeCharacterSettingMethod
//
/////////////////////////////////////////
crChangeCharacterSettingMethod::crChangeCharacterSettingMethod():
	m_characterDispDistance(150),
	m_characterShadowDistance(80),
	m_characterUpdateFactor(0.1),
	m_characterNearDistance(50)
{
}
crChangeCharacterSettingMethod::crChangeCharacterSettingMethod(const crChangeCharacterSettingMethod& handle):
	crMethod(handle),
	m_characterDispDistance(handle.m_characterDispDistance),
	m_characterShadowDistance(handle.m_characterShadowDistance),
	m_characterUpdateFactor(handle.m_characterUpdateFactor),
	m_characterNearDistance(handle.m_characterNearDistance)
{
}
void crChangeCharacterSettingMethod::inputParam(int i, void *param)
{
}
void crChangeCharacterSettingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_characterDispDistance = atof(str.c_str());
		break;
	case 1:
		m_characterShadowDistance = atof(str.c_str());
		break;
	case 2:
		m_characterUpdateFactor = atof(str.c_str());
		break;
	case 3:
		m_characterNearDistance = atof(str.c_str());
		break;
	}
}
void crChangeCharacterSettingMethod::operator()(crHandle &handle)
{
	crDisplaySettings::instance()->setCharacterDispDistance(m_characterDispDistance);
	crDisplaySettings::instance()->setCharacterShadowDistance(m_characterShadowDistance);
	crDisplaySettings::instance()->setCharacterUpdateFactor(m_characterUpdateFactor);
	crDisplaySettings::instance()->setCharacterNearDistance(m_characterNearDistance);
}
////////////////////////////////
//
//crSetWidgetEnableMethod
//
///////////////////////////////////
crSetWidgetEnableMethod::crSetWidgetEnableMethod():
	m_this(NULL),
	m_enable(false)
{}

crSetWidgetEnableMethod::crSetWidgetEnableMethod(const crSetWidgetEnableMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_enable(handle.m_enable)
{
}

void crSetWidgetEnableMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crSetWidgetEnableMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_enable = atoi(str.c_str());
		break;
	}
}

void crSetWidgetEnableMethod::operator()(crHandle &handle)
{
	m_this->setEnable(m_enable);
}
////////////////////////////////
//
//crIsRenderRunningMethod
//
///////////////////////////////////
crIsRenderRunningMethod::crIsRenderRunningMethod()
{}

crIsRenderRunningMethod::crIsRenderRunningMethod(const crIsRenderRunningMethod& handle):
	crMethod(handle)
{
}

void crIsRenderRunningMethod::inputParam(int i, void *param)
{
}

void crIsRenderRunningMethod::addParam(int i, const std::string& str)
{
}

void crIsRenderRunningMethod::operator()(crHandle &handle)
{
	bool running = crStartHandler::getInstance()->isRunning();
	handle.outputParam(0,&running);
}
//////////////////////////////////
////
////crBakeLightMapMethod
////
/////////////////////////////////////
//crBakeLightMapMethod::crBakeLightMapMethod():
//m_updInterval(0.5f)
//{
//	m_texSize.set(512,512);
//	m_worldSize.set(512,512,512);
//}
//
//crBakeLightMapMethod::crBakeLightMapMethod(const crBakeLightMapMethod& handle):
//crMethod(handle),
//m_imageBox(handle.m_imageBox),
//m_texSize(handle.m_texSize),
//m_worldSize(handle.m_worldSize),
//m_updInterval(handle.m_updInterval)
//{
//}
//
//void crBakeLightMapMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crWidgetNode*)param;
//		break;
//	}
//}
//
//void crBakeLightMapMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_imageBox = str;
//		break;
//	case 1:
//		crArgumentParser::appAtoVec(str,m_texSize);
//		break;
//	case 2:
//		crArgumentParser::appAtoVec(str,m_worldSize);
//		break;
//	case 3:
//		m_updInterval = atof(str.c_str());
//		break;
//	}
//}
//
//void crBakeLightMapMethod::operator()(crHandle &handle)
//{
//	crImageBoxWidgetNode *imageBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getParentCanvas()->getWidget(m_imageBox));
//	if(imageBox)
//	{
//		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
//		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
//		if(gamebodyInfo)
//		{
//			crData *infoData = gamebodyInfo->getDataClass();
//			assert(!infoData);
//
//			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
//			if(!selectNodeVec.empty())
//			{
//				crNode *currentSelectNode = selectNodeVec[0].get();
//				if(currentSelectNode)
//				{
//					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
//					searchByNameVisitor.setNameId("ProxyNode");
//					searchByNameVisitor.setSearchNodeType(ALLNODE);
//					currentSelectNode->accept(searchByNameVisitor);
//					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
//					if(proxyNode)
//					{
//						crGroup *rootNode = proxyNode->getChild(0);
//						crRenderToTexture *rtt = crRenderToTextureManager::getInstance()->findRenderToTexture("BakeLightMap");
//						if(!rtt)
//						{
//							crRenderToTexture *rtt = new crRenderToTexture;
//							rtt->setName("BakeLightMap");
//							rtt->setMode(crRenderToTexture::RenderToTextureMode(crRenderToTexture::RenderToTexture|crRenderToTexture::RenderCollideMap|crRenderToTexture::RealTimeUpd|crRenderToTexture::ExternMVP));
//							rtt->setMapSize(m_texSize[0],m_texSize[1]);
//							rtt->setUpdateInterval(m_updInterval);
//
//							rtt->addRenderNode(rootNode);
//							rtt->addTexturedNode(imageBox->getImageObject());
//							//float terHeight = heightField->getMaxHeight();
//							rtt->setProjectionMatrix(CRCore::crMatrix::ortho2D(-1,1,-1,1));
//							rtt->setViewMatrix(crMatrix::identity());
//							rtt->init(1.0f);
//							crStateSet *camerass = rtt->getCameraNode()->getOrCreateStateSet();
//							CRIOManager::crLoadManager::getInstance()->requestCompileStateSet(camerass);
//							///////////
//							crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("bakelightmap");
//							crUniform *uniform = camerass->getOrCreateUniform("walkable",CRCore::crUniform::FLOAT);
//							uniform->setDataVariance(crBase::STATIC);
//							uniform->set(0.0f);
//							uniform = camerass->getOrCreateUniform("maxheight",CRCore::crUniform::FLOAT);
//							uniform->setDataVariance(crBase::STATIC);
//							uniform->set((float)m_worldSize[2]);
//							camerass->merge(*shader);
//							camerass->setStateSetMode(crStateSet::SS_CollideMapRender);
//							crRenderToTextureManager::getInstance()->addRenderToTexture(rtt);
//						}
//						else
//						{
//							rtt->setEnable(true);
//						}
//					}
//				}
//			}
//		}
//	}
//}
//extern float GenerateBakingSampleRay( const Camera* camera , const Scene* scene , Intersection* isect , const CameraSample& sample , RayDifferential* rd )
//{
//	Point uvSample(sample.imageX/camera->film->xResolution , sample.imageY/camera->film->xResolution , -1 );
//	Ray uvspaceRay( uvSample , Vector( 0 , 0 , 1 ) , -1.0f  );
//
//	float rayWeight = 0.0f;
//
//	if( scene->IntersectBakingSurface( uvspaceRay , isect ) )
//	{
//		Normal n = isect->dg.nn;
//		rd->o = isect->dg.p + Point( n.x , n.y , n.z ) ;
//		rd->d = Vector( -isect->dg.nn.x , -isect->dg.nn.y , -isect->dg.nn.z );
//		rd->maxt = 1.0f;
//		rd->depth = 0;
//		rd->time = Lerp(sample.time, camera->shutterOpen, camera->shutterClose);
//		rayWeight = 1.0f;
//	}
//	else
//	{
//		rayWeight = 0.0f;
//	}
//	return rayWeight;
//}