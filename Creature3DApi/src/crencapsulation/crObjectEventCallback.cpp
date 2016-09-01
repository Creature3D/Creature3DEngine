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
#include <CREncapsulation/crObjectEventCallback.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRAL/crSoundState.h>
#include <CRAL/crSoundManager.h>
#include <CREncapsulation/crNodeCallbacks.h>
#include <CRCore/crDOFTransform.h>
#include <CRTerrain/crTerrainTile.h>
#include <CRCore/crShapeDrawable.h>
#include <CRIOManager/crLoadManager.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPolygonMode.h>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRParticle;
using namespace CRPhysics;

///////////////////////////////////////////
////
////crShowMouseCallback
////
///////////////////////////////////////////
//crShowMouseCallback::crShowMouseCallback(){}
//crShowMouseCallback::crShowMouseCallback(const crShowMouseCallback& callback):
//CRCore::crEventCallback(callback),
//m_showMouse(callback.m_showMouse)
//{
//}
//void crShowMouseCallback::addEventParam(int i, const std::string &str)
//{
//	m_showMouse = atoi(str.c_str());
//}
//
//void crShowMouseCallback::operator()(void)
//{
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
//
//	if(m_showMouse > -2)
//		crFilterRenderManager::getInstance()->showMouse(m_showMouse);
//}
//
///////////////////////////////////////////
////
////crShowMousePickMsgCallback
////
///////////////////////////////////////////
//crShowMousePickMsgCallback::crShowMousePickMsgCallback(){}
//crShowMousePickMsgCallback::crShowMousePickMsgCallback(const crShowMousePickMsgCallback& callback):
//CRCore::crEventCallback(callback),
//m_showMousePicMsg(callback.m_showMousePicMsg)
//{
//}
//void crShowMousePickMsgCallback::addEventParam(int i, const std::string &str)
//{
//	m_showMousePicMsg = atoi(str.c_str());
//}
//
//void crShowMousePickMsgCallback::operator()(void)
//{
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
//
//	if(m_showMousePicMsg > -2)
//		crFilterRenderManager::getInstance()->showMousePicMsg(m_showMousePicMsg);
//}
//
///////////////////////////////////////////
////
////crLightSwitchCallback
////
///////////////////////////////////////////
//crLightSwitchCallback::crLightSwitchCallback(){}
//crLightSwitchCallback::crLightSwitchCallback(const crLightSwitchCallback& callback):
//CRCore::crEventCallback(callback),
//m_lightSourceArray(callback.m_lightSourceArray)
//{
//}
//void crLightSwitchCallback::addEventParam(int i, const std::string &str)
//{
//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//	searchByNameVisitor.setNameId(str);
//	searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::LIGHTSOURCE);
//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
//	crLightSource *ls = dynamic_cast<crLightSource *>(searchByNameVisitor.getResult());
//	if(ls) m_lightSourceArray.push_back(ls);
//}
//
//void crLightSwitchCallback::operator()(void)
//{
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
//
//	crLightSource *ls;
//	for( LSVec::const_iterator itr = m_lightSourceArray.begin();
//		itr != m_lightSourceArray.end();
//		++itr )
//	{
//		ls = const_cast<crLightSource *>(itr->get());
//		if(ls) 
//		{
//			ls->setEnable(!ls->getEnable());
//		}
//	}
//}
//
///////////////////////////////////////////
////
////crParticleEffectCallback
////
///////////////////////////////////////////
//crParticleEffectCallback::crParticleEffectCallback(){}
//crParticleEffectCallback::crParticleEffectCallback(const crParticleEffectCallback& callback):
//CRCore::crObject::crObjectEvent::Callback(callback),
//m_effectTypeString(callback.m_effectTypeString)
//{
//}
//void crParticleEffectCallback::addEventParam(int i, const std::string &str)
//{
//	m_effectTypeString = str;
//}
//void crParticleEffectCallback::operator()(crEventGroup &, const CRCore::crObject::crObjectEvent &, const crVector3 &mousePickPoint)
//{
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
//
//	crParticleEffect *particleEffect = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(m_effectTypeString);
//	if(!particleEffect) return;
//	particleEffect->setEffectPosition(mousePickPoint);
//    particleEffect->startEffect();
//	//CRCore::notify(CRCore::FATAL)<<"crParticleEffectEventCallback(): particleEffect = "<<particleEffect<<std::endl;
//}
//
///////////////////////////////////////////
////
////crDrawableStateSetCallback
////
///////////////////////////////////////////
//crDrawableStateSetCallback::crDrawableStateSetCallback(){}
//crDrawableStateSetCallback::crDrawableStateSetCallback(const crDrawableStateSetCallback& callback):
//CRCore::crObject::crObjectEvent::Callback(callback)
//{
//}
//void crDrawableStateSetCallback::operator()(crEventGroup &	, const CRCore::crObject::crObjectEvent &, const crVector3 &mousePickPoint)
//{//
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
//
//	//取得drawable的stateset，打开stateset编辑对话框
//}

////////////////////////////////
//
//crXTransTMDofCallback
//
///////////////////////////////////
crXTransTMDofCallback::crXTransTMDofCallback():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{}

crXTransTMDofCallback::crXTransTMDofCallback(const crXTransTMDofCallback& callback):
crNode::PhysicsObjectCallback(callback),
m_nodeName(callback.m_nodeName),
m_min(callback.m_min),
m_max(callback.m_max),
m_step(callback.m_step)
{
}

void crXTransTMDofCallback::addEventParam(int i, const std::string &str)
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

void crXTransTMDofCallback::operator()(crNode &node, _crInt64 param)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_nodeName);
		searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::ALLNODE);
		node.accept(searchByNameVisitor);
		crNode *resultNode = searchByNameVisitor.getResult();
		if(resultNode)
		{
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
//crYTransTMDofCallback
//
///////////////////////////////////
crYTransTMDofCallback::crYTransTMDofCallback():
m_min(0.0f),
m_max(0.0f),
m_step(0.0f)
{}

crYTransTMDofCallback::crYTransTMDofCallback(const crYTransTMDofCallback& callback):
crNode::PhysicsObjectCallback(callback),
m_nodeName(callback.m_nodeName),
m_min(callback.m_min),
m_max(callback.m_max),
m_step(callback.m_step)
{
}

void crYTransTMDofCallback::addEventParam(int i, const std::string &str)
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

void crYTransTMDofCallback::operator()(crNode &node, _crInt64 param)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);

	if(!m_dof.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_nodeName);
		searchByNameVisitor.setSearchNodeType(crSearchNodeBYNameVisitor::ALLNODE);
		node.accept(searchByNameVisitor);
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
