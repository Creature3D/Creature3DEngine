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
#include <CREncapsulation/crMethodClass.h>
#include <CREncapsulation/crDataClass.h>
#include <CREncapsulation/crLoader.h>
#include <CREncapsulation/crNodeCallbacks.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CREncapsulation/crTableIO.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crCameraGroup.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRCore/crBrain.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crShape.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPoint.h>
#include <CRCore/crDOFTransform.h>
#include <CRUtil/crPrepareVisitor.h>
#include <CRUtil/crSceneView.h>

#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crWriteFile.h>
#include <CRIOManager/crReadFile.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crVisionSenseInfor.h>

#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/crWindowContainer.h>

#include <rbody/Creature3D/CreBody.h>
#include <rbody/Creature3D/SubMeshDrawable.h>
#include "rcfg/ConfigScriptOO.h"
#include <rcfg/ItemMap.h>

#include <CRTerrain/crTerrainTile.h>
#include <CRTerrain/crGeometryTechnique.h>
#include <CRTerrain/crLayer.h>

#include <CRIOManager/crCookFile.h>
#include <CRUtil/crOptimizer.h>
#include <rbody/AnimationBase.h>
#include <CRParticle/crParticleEffect.h>
#include <CRParticle/crModularEmitter.h>
#include <CRParticle/crRandomRateCounter.h>
#include <CRParticle/crConstantRateCounter.h>
#include <CRParticle/crSectorPlacer.h>
#include <CRParticle/crBoxPlacer.h>
#include <CRParticle/crPointPlacer.h>
#include <CRParticle/crMultiSegmentPlacer.h>
#include <CRParticle/crSegmentPlacer.h>
#include <CRParticle/crRadialShooter.h>
#include <CRParticle/crModularProgram.h>
#include <CRParticle/crFluidProgram.h>
#include <CRParticle/crAccelOperator.h>
#include <CRParticle/crAngularAccelOperator.h>
#include <CRParticle/crFluidFrictionOperator.h>
#include <CRParticle/crForceOperator.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRParticle/crConnectedParticleSystem.h>
#include <CRParticle/crAngularDampingOperator.h>
#include <CRParticle/crDampingOperator.h>
#include <CRParticle/crExplosionOperator.h>
#include <CRParticle/crOrbitOperator.h>
#include <CRParticle/crConstantCounter.h>
#include <CRCore/crCalcNodeParentsMatrixVisitor.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <sys/stat.h>
#include <stdio.h>
#include <commdlg.h>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRGUI;
using namespace CRPhysics;
using namespace rbody;
using namespace CRIOManager;
using namespace CRTerrain;
using namespace CRUtil;
using namespace CRProducer;
using namespace CRParticle;
/////////////////////////////////////////
//
//crExcEventContainerMethod
//
/////////////////////////////////////////
crExcEventContainerMethod::crExcEventContainerMethod(){}
crExcEventContainerMethod::crExcEventContainerMethod(const crExcEventContainerMethod& handle):
crMethod(handle)
{
}
void crExcEventContainerMethod::inputParam(int i, void *param)
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
		m_this = (crBase*)param;
		break;
	case 2:
		m_param = param==NULL?NULL:*(_crInt64*)param;
		break;
	}
}

void crExcEventContainerMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_lmsg = parseEventMsg(str);
		break;
	case 1:
		m_hmsg = parseEventMsg(str);
		break;
	}
}

void crExcEventContainerMethod::operator()(crHandle &handle)
{
	m_this->doEvent(MAKEINT64(m_lmsg,m_hmsg),MAKEINT64(WCH_EXCEVENT,LOINT64(m_param)));
}
/////////////////////////////////////////
//
//crAITaskMethod
//
/////////////////////////////////////////
crAITaskMethod::crAITaskMethod()
{
}
crAITaskMethod::crAITaskMethod(const crAITaskMethod& handle):
crMethod(handle),
m_taskName(handle.m_taskName)
{
}
void crAITaskMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_param = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_param = LOINT64(param64);
		}
		else
		{
			m_param = NULL;
		}
		break;
	}
}

void crAITaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskName = str;
		m_task =  crAITaskManager::getInstance()->getAITask(m_taskName);
		break;
	}
}

void crAITaskMethod::operator()(crHandle &handle)
{
	if(m_task.valid())
	{
		m_this->doEvent(WCH_INPUTAITASK,MAKEINT64(m_param,m_task.get()));
	}
}
/////////////////////////////////////////
//
//crInputAITaskMethod
//
/////////////////////////////////////////
crInputAITaskMethod::crInputAITaskMethod():
m_rec(false)
{
}
crInputAITaskMethod::crInputAITaskMethod(const crInputAITaskMethod& handle):
crMethod(handle),
m_rec(handle.m_rec)
{
}
void crInputAITaskMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_param = NULL;
			m_task = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_param = LOINT64(param64);
			m_task = (CRCore::crAITask *)(HIINT64(param64));
		}
		else
		{
			m_param = NULL;
			m_task = NULL;
		}
		break;
	}
}

void crInputAITaskMethod::addParam(int i, const std::string& str)
{
}

void crInputAITaskMethod::operator()(crHandle &handle)
{
	if(m_task.valid())
	{
		int lparam = LOINT32(m_param);
		int hparam = HIINT32(m_param);
		if(lparam == WCHAI_Begin)
		{
			//if(m_task->getTaskState() != crAITask::TaskEnd)
			//	return;
			m_rec = false;
			if(hparam & WCHAI_RecBot)
			{
				m_rec = true;
				m_recMatrix = m_this->getMatrix();
			}
			crControlPoint *cp = m_task->getCurrentTaskPoint(0);
			if(hparam & WCHAI_SetCPToBot)
			{
				cp->setPosition(m_this->getTrans());
			}
			else if(hparam & WCHAI_SetBotToCP)
			{
				const crVector3 &orientation = cp->getOrientation();
				CRCore::crQuat q1(orientation[0], CRCore::crVector3(1, 0, 0)), q2(orientation[1], CRCore::crVector3(0, 1, 0)), q3(orientation[2], CRCore::crVector3(0, 0, 1));
				CRCore::crMatrix mrot;
				mrot.set(q1*q2*q3);
				mrot.setTrans(cp->getPosition());
				m_this->setMatterObjectMatrix(mrot,CRPhysics::crMatterObject::MD_FullMatrix);
			}
   //         if(hparam & WCHAI_TakeoverControl)
			//{
			//	CRPhysics::crCamera *camera = crKeyboardMouseHandle::getInstance()->getBindViewer()->getViewCameraNode();
			//	if(camera && camera->getAttachedNode() == m_this)
			//	{
			//		camera->setAcceptKeyboardMouse(false);
			//	}
			//}
			if(m_task->getTaskState() == crAITask::TaskEnd)
				m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_DOAITASK,m_task.get()));
			m_task->setTaskState(crAITask::TaskBegin);
		}
		else if(lparam == WCHAI_Pause)
		{
			m_task->setTaskState(crAITask::TaskPause);
		}
		else if(lparam == WCHAI_Resume)
		{
			m_task->setTaskState(crAITask::TaskDoing);
			m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_DOAITASK,m_task.get()));
		}
		else if(lparam == WCHAI_End)
		{
			m_task->setTaskState(crAITask::TaskEnd);
			if(m_rec)
			{
				m_this->setMatterObjectMatrix(m_recMatrix,crMatterObject::MD_FullMatrix);
			}
			//CRPhysics::crCamera *camera = crKeyboardMouseHandle::getInstance()->getBindViewer()->getViewCameraNode();
			//if(camera && camera->getAttachedNode() == m_this)
			//{
			//	camera->setAcceptKeyboardMouse(true);
			//}
		}
	}
}
/////////////////////////////////////////
//
//crDoAITaskMethod
//
/////////////////////////////////////////
crDoAITaskMethod::crDoAITaskMethod()
{
}
crDoAITaskMethod::crDoAITaskMethod(const crDoAITaskMethod& handle):
crMethod(handle)
{
}
void crDoAITaskMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_task = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crViewMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_task = (crAITask *)(LOINT64(param64));
		}
		else
		{
			m_task = NULL;
		}
		break;
	}
}

void crDoAITaskMethod::addParam(int i, const std::string& str)
{
}

//void crDoAITaskMethod::reset()
//{//这些数据需要与m_task相关联，否则容易产生crDoAITaskMethod error
//	m_time_aimsg = 0.0f;
//	m_currentIndex = 0;
//	m_currentMsgIndex = 0;
//	m_interval_aimsg = 0.0f;
//	m_preControlPoint = NULL;
//}

void crDoAITaskMethod::operator()(crHandle &handle)
{
	if(m_task.valid())
	{
		if(m_task->getTaskState() == crAITask::TaskBegin)
		{
			m_task->reset();
			m_task->setTaskState(crAITask::TaskDoing);
		}
		else if(m_task->getTaskState() == crAITask::TaskEnd)
		{
			return;
		}

		int &currentIndex = m_task->currentIndex();
		float interval = crFrameStamp::getInstance()->getFrameInterval();
		float &interval_aimsg = m_task->interval_aimsg();
		interval_aimsg += interval;
		crControlPoint *nextControlPoint = 0;
		crControlPoint *currentcp = m_task->getCurrentTaskPoint(currentIndex);
		if(!currentcp)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crDoAITaskMethod error"<<std::endl;
			return;
		}
		int nextIndex;
        crControlPoint*& preControlPoint = m_task->preControlPoint();
		unsigned int &currentMsgIndex = m_task->currentMsgIndex();
		float& time_aimsg = m_task->time_aimsg();
		crVector3f myPos = m_this->getTrans();
		float r = m_this->getGameBodyInfo()->getTaskPointRange();
		if(currentIndex == 0 || currentcp->isInRange(myPos,r))
		{
			nextControlPoint = m_task->getNextTaskPoint(m_task->currentIndex(),nextIndex);
			currentIndex = nextIndex;

			if(preControlPoint!=currentcp)
			{
				currentMsgIndex = 0;
				time_aimsg = 0;
				preControlPoint = currentcp;
				if(preControlPoint->getNeedVelocity())
				{
					m_this->setVelocity(preControlPoint->getVelocity());
				}
			}
			//if(nextControlPoint) targetPos = nextControlPoint->getPosition();
		}
		if(nextControlPoint)
		{
			crVector3f targetPos = nextControlPoint->getPosition();
			crVector3f targetDir = targetPos - myPos;
			targetDir[2] = 0.0f;
			targetDir.normalize();
			if(0/*!m_immediatelyTurn*/)
			{
				CRCore::crVector3f myDir = -m_this->getDirection();
				//crVector3f targetDir = targetPos - myPos;
				//targetDir.normalize();
				//if(body->getVelocity().x() > 0.0f)
				//{
					//CRCore::notify(CRCore::ALWAYS)<<" nextControlPoint "<<std::endl;
					//turn to cp
					turnToDir(targetDir,myDir);
				//}
			}
			else
			{
				//targetPos[2] = myPos[2];
				//if(targetPos!=myPos)
				//{
					//crVector3f targetDir = targetPos - myPos;
					//targetDir.normalize();
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
					m_this->setMatterObjectMatrix(newMat,CRPhysics::crMatterObject::MD_RotMatrix);
				//}
			}
		}
		if(preControlPoint)
		{
			if(interval_aimsg>time_aimsg)
			{//对于队友该如何选择CP？
				//队友不主动向CP移动，但它向主角靠近，随而触发进入到下一CP，又或者，可以通过外部设置让其进入到下一CP
				interval_aimsg = 0.0f;
				Msg* msg;
				if(dynamic_cast<crProbabilityMsgControlPoint *>(preControlPoint))
				{
					msg = (dynamic_cast<crProbabilityMsgControlPoint *>(preControlPoint))->getProbabilityMsg();
				}
				else if(dynamic_cast<crOrdinalMsgControlPoint *>(preControlPoint))
				{
					msg = (dynamic_cast<crOrdinalMsgControlPoint *>(preControlPoint))->getOrdinalMsg(currentMsgIndex);
				}
				if(msg)
				{
					//bodyNC->acceptEventMessage(msg->m_msg,msg->m_wparam,msg->m_lparam);
					if(msg->m_msg == WCH_TASKEND)
					{
						m_this->doEvent(WCH_INPUTAITASK,MAKEINT64(MAKEINT32(WCHAI_End,NULL),m_task.get()));
					}
					else
					{
					    m_this->doEvent(msg->m_msg,msg->m_param);
					}
					time_aimsg = msg->m_interval;
				}
				//CRCore::notify(CRCore::ALWAYS)<<"crBodyCaseAIUpdatePPC::parseParamImplementation ProbabilityMsg "<<m_lastmsg.m_msg<<" "<<m_lastmsg.m_wparam<<" "<<m_lastmsg.m_lparam<<std::endl;
			}
		}

		if(m_task->getTaskState() == crAITask::TaskDoing)
		    m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_DOAITASK,m_task.get()));
	}
}
float crDoAITaskMethod::computeAngle(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir)
{
	float anticlockwise = -1.0f;
	if(myDir[0]>=0.0f && myDir[1]>=0.0f)
	{//第一象限
		/*if(targetDir[1]>myDir[1] || (targetDir[0]<0.0f && targetDir[1]>-myDir[1]))
		anticlockwise = 1;*/
		if(targetDir[1]<myDir[1] && targetDir[0] > -myDir[0])
			anticlockwise = -1.0f;
		else anticlockwise = 1.0f;
	}
	else if(myDir[0]<0.0f && myDir[1]>=0.0f)
	{//第二象限
		if(targetDir[1]<myDir[1] && targetDir[0] < -myDir[0])
			anticlockwise = 1.0f;
	}
	else if(myDir[0]<0.0f && myDir[1]<0.0f)
	{//第三象限
		if(targetDir[1]>myDir[1] && targetDir[0] < -myDir[0])
			anticlockwise = -1.0f;
		else anticlockwise = 1.0f;
	}
	else if(myDir[0]>=0.0f && myDir[1]<0.0f)
	{//第四象限
		if(targetDir[1]>myDir[1] && targetDir[0] > -myDir[0])
			anticlockwise = 1.0f;
	}
	return anticlockwise * acos(myDir * targetDir);
}
void crDoAITaskMethod::turnToDir(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir)
{
	crVector3f _targetDir = targetDir;
	crVector3f _myDir = myDir;
	_targetDir[2] = 0.0f;
	_targetDir.normalize();
	_myDir[2] = 0.0f;
	_myDir.normalize();
	float angle = computeAngle(_targetDir,_myDir);

	if(angle>0.01f)//0.5弧度每秒，25帧的精度是0.02
	{
	    //bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNLEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
		m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNLEFT,crGUIEventAdapter::KEYDOWN),m_task.get()));
	}
	else if(angle<-0.01f)
	{
		//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNRIGHT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
		m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNRIGHT,crGUIEventAdapter::KEYDOWN),m_task.get()));
	}
	else
	{
		if(0/*m_isAircraft*/)
		{
			//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNLEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
			m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNLEFT,crGUIEventAdapter::KEYUP),m_task.get()));
			_targetDir = targetDir;
			_myDir = myDir;
			_targetDir[0] = 0.0f;
			_targetDir.normalize();
			_myDir[0] = 0.0f;
			_myDir.normalize();
			float cos_angle = _myDir * _targetDir;
			float anticlockwise = _targetDir[2]>_myDir[2]? 1.0f : -1.0f;
			angle = anticlockwise * acos(cos_angle);
			if(angle>0.02f)
			{
				//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNDOWN,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
				m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNDOWN,crGUIEventAdapter::KEYDOWN),m_task.get()));
			}
			else if(angle<-0.02f)
			{
				//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNUP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYDOWN));
				m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNUP,crGUIEventAdapter::KEYDOWN),m_task.get()));
			}
			else
			{
				//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNUP,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
				m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNUP,crGUIEventAdapter::KEYUP),m_task.get()));
			}
		}
		else
		{
			//bodyNC->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(WCH_TURNLEFT,NULL),MAKEINT64(WCH_INPUTPARAM,WCH_KEYUP));
			m_this->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(WCH_TURNLEFT,crGUIEventAdapter::KEYUP),m_task.get()));
		}
	}
}
/////////////////////////////////////////
//
//crGetRadarInfoMethod
//
/////////////////////////////////////////
crGetRadarInfoMethod::crGetRadarInfoMethod():
m_maskDistance(5.0f)
{
}
crGetRadarInfoMethod::crGetRadarInfoMethod(const crGetRadarInfoMethod& handle):
crMethod(handle),
m_maskDistance(handle.m_maskDistance)
{
}
void crGetRadarInfoMethod::inputParam(int i, void *param)
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
		m_this = (CRPhysics::crViewMatterObject*)param;
		break;
	}
}

void crGetRadarInfoMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_maskDistance = atoi(str.c_str());
		break;
	}
}

void crGetRadarInfoMethod::operator()(crHandle &handle)
{
	crRadarMatterObject *radar = m_this->getRadar();
	if(radar)
	{
		crVisionSenseInfor *vsi = m_this->getVisionSenseInfor();
		crRadarMatterObject::ContactInfoVec& contactInfoVec = vsi->getContactInfoVec();
		radar->getContactInfo(contactInfoVec);
		vsi->computerFrontContact(m_this);
		int envmask = 0;
		crMatterObject *matterObject;
		int size = radar->getDetectRaySize();
		float maxDistance = radar->getDetectDistance();
		for(int i = 0; i<size; i++)
		{
			if(contactInfoVec[i]->m_distance >= maxDistance)
				continue;
			matterObject = dynamic_cast<crMatterObject *>(contactInfoVec[i]->m_contactNode.get());
			if(!matterObject && contactInfoVec[i]->m_distance <= m_maskDistance)
			{//静态环境
                envmask |= 1<<i;
			}
		}
		vsi->setEnvInfo(envmask,contactInfoVec[size]->m_distance/* - body->getBoundBox().zLength() * 0.5f*/);
	}
}
/////////////////////////////////////////
//
//crAttachNodeUpdateMethod
//
/////////////////////////////////////////
crAttachNodeUpdateMethod::crAttachNodeUpdateMethod()
{
    m_scale.set(1.0f,1.0f,1.0f);
}
crAttachNodeUpdateMethod::crAttachNodeUpdateMethod(const crAttachNodeUpdateMethod& handle):
crMethod(handle),
m_offset(handle.m_offset),
m_scale(handle.m_scale),
m_rotAngle(handle.m_rotAngle),
m_boneName(handle.m_boneName),
m_dir(handle.m_dir)
{
}
void crAttachNodeUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_updateVisitor = NULL;
			//m_target = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	//case 2:
	//	if(param)
	//	{
	//		_crInt64 param64 = *(_crInt64*)param;
	//		m_updateVisitor = (CRUtil::crUpdateVisitor *)(LOINT64(param64));
	//	}
	//	else
	//	{
	//		m_updateVisitor = NULL;
	//	}
	//	break;
	case 3:
		m_target = (crNode*)param;
		break;
	case 4:
		m_offset = *(crVector3*)param;
		break;
	case 5:
		m_scale = *(crVector3*)param;
		break;
	case 6:
		m_rotAngle = *(float*)param;
		break;
	case 7:
		m_boneName = *(std::string *)param;
		break;
	case 8:
		m_dir = *(crVector3*)param;
		break;
	}
}

void crAttachNodeUpdateMethod::addParam(int i, const std::string& str)
{
}
void crAttachNodeUpdateMethod::operator()(crHandle &handle)
{
	if(m_target.valid()/* && m_updateVisitor.valid()*/)
	{
		if(!m_target->getVisiable())
		{
			m_this->setVisiable(false);
			return;
		}
		crMatrix matrix;
		if(dynamic_cast<crViewMatterObject*>(m_target.get()))
		{//默认是BBOX_BOTTOM
			crViewMatterObject *bot = dynamic_cast<crViewMatterObject*>(m_target.get());
			if(m_boneName.compare("BBOX_CENTER")==0)
			{
				matrix.setTrans(crVector3(0.0f,0.0f,bot->getPhysicsBox()[2]*0.5f));
			}
			else if(m_boneName.compare("BBOX_TOP")==0)
			{
				matrix.setTrans(crVector3(0.0f,0.0f,bot->getPhysicsBox()[2]));
			}
			//dynamic_cast<crTransform *>(m_target.get())->computeLocalToWorldMatrix(matrix,NULL/*m_updateVisitor.get()*/);
		}
		else if(dynamic_cast<crTransform *>(m_target.get()))
		{//默认是BBOX_BOTTOM
			if(m_boneName.compare("BBOX_CENTER")==0)
			{
				matrix.setTrans(crVector3(0.0f,0.0f,m_target->getBoundBox().zLength()*0.5f));
			}
			else if(m_boneName.compare("BBOX_TOP")==0)
			{
				matrix.setTrans(crVector3(0.0f,0.0f,m_target->getBoundBox().zLength()));
			}
			//dynamic_cast<crTransform *>(m_target.get())->computeLocalToWorldMatrix(matrix,NULL/*m_updateVisitor.get()*/);
		}
		else if(dynamic_cast<rbody::CreBodyNode *>(m_target.get()))
		{
			if(m_boneName.compare("BBOX_CENTER")==0)
			{
				matrix.setTrans(m_target->getBoundBox().center());
			}
			else if(m_boneName.compare("BBOX_TOP")==0)
			{
				matrix.setTrans(m_target->getBoundBox().center()+crVector3(0.0f,0.0f,m_target->getBoundBox().zLength()*0.5f));
			}
			else if(m_boneName.compare("BBOX_BOTTOM")==0 || m_boneName.empty())
			{
				matrix.setTrans(m_target->getBoundBox().center()+crVector3(0.0f,0.0f,-m_target->getBoundBox().zLength()*0.5f));
			}
			else
			{
				dynamic_cast<rbody::CreBodyNode *>(m_target.get())->getBoneMatrix(m_boneName,matrix);
			}
		}
		else
		{//默认是BBOX_BOTTOM
			if(m_boneName.compare("BBOX_CENTER")==0)
			{
				matrix.setTrans(m_target->getBoundBox().center());
			}
			else if(m_boneName.compare("BBOX_TOP")==0)
			{
				matrix.setTrans(m_target->getBoundBox().center()+crVector3(0.0f,0.0f,m_target->getBoundBox().zLength()*0.5f));
			}
			else/* if(m_boneName.compare("BBOX_BOTTOM"))*/
			{
				matrix.setTrans(m_target->getBoundBox().center()+crVector3(0.0f,0.0f,-m_target->getBoundBox().zLength()*0.5f));
			}
		}
		if(m_offset != crVector3(0.0f,0.0f,0.0f)) matrix.preMultTranslate(m_offset);
		crMatrix offsetmat;
		if(m_scale != crVector3(1.0f,1.0f,1.0f)) offsetmat.makeScale(m_scale);
		if(m_rotAngle != 0.0f) offsetmat.postMult(crMatrix::rotate(m_rotAngle,CRCore::Z_AXIS));
		if(m_dir != crVector3(0.0f,0.0f,0.0f))
		{
			crMatrix rotmat;
			if(m_dir[1] > 0.999)
			{
				rotmat.makeRotate(-CRCore::Y_AXIS,m_dir);
				CRCore::crVector3d ep,center,up;
				rotmat.getLookAt(ep,center,up);
				if(center[2]>0.0f)
				{
					center = -center;
				}
				rotmat.makeLookAt(ep,center,up);
			}
			else
				rotmat.makeRotate(-CRCore::Y_AXIS,m_dir);
			offsetmat.postMult(rotmat);
		}
		if(offsetmat != crMatrix::identity())
		{
			matrix.preMult(offsetmat);
		}

		crCalcNodeParentsMatrixVisitor parentMatrix;
		m_target->accept(parentMatrix);
		crMatrix &parentMat = parentMatrix.getResult();
		matrix.postMult(parentMat);

		//if(offsetmat != crMatrix::identity())
		//{
		//	crVector3 trans = matrix.getTrans();
		//	crMatrix put_mat = crMatrix::translate(trans);
		//	crMatrix inv_put = crMatrix::inverse(put_mat);
		//	matrix.postMult(inv_put);
		//	matrix.postMult(offsetmat);
		//	matrix.postMult(put_mat);
		//}
		if(dynamic_cast<crMatterObject *>(m_this))
		{
			dynamic_cast<crMatterObject *>(m_this)->setMatrix(matrix);
			dynamic_cast<crMatterObject *>(m_this)->setMatterObjectMatrix(matrix,crMatterObject::MD_FullMatrix);
		}
		else if(dynamic_cast<crMatrixTransform *>(m_this))
		{
			int attach = 0;
			if(crArgumentParser::readKeyValue(m_this->getDescriptions(),"PSAttach",attach) && attach)
			{
				crMatrix rotMat = matrix;
				rotMat.setTrans(0.0f,0.0f,0.0f);
				crVector3 dir = (Y_AXIS * rotMat).normalize();
				crVector3 position = matrix.getTrans();
				crParticleEffect *effect;
				CRCore::NodeArray& nodeArray = dynamic_cast<crMatrixTransform *>(m_this)->getChildArray();
				for( CRCore::NodeArray::iterator itr = nodeArray.begin();
					itr != nodeArray.end();
					++itr )
				{
					effect = dynamic_cast<crParticleEffect *>(itr->get());
					if(effect)
					{
						effect->setEffectDirection(dir);
						effect->setEffectPosition(position);
					}
				}
			}
		    else dynamic_cast<crMatrixTransform *>(m_this)->setMatrix(matrix);
		}
		else if(dynamic_cast<crLightSource *>(m_this))
		{
			crLightSource *ls = dynamic_cast<crLightSource *>(m_this);
			crLight *light = dynamic_cast<crLight *>(ls->getLight());

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
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////
////
////crPlayAmbSoundMethod
////
///////////////////////////////////////////
//crPlayAmbSoundMethod::crPlayAmbSoundMethod():
//m_soundState(NULL){}
//crPlayAmbSoundMethod::crPlayAmbSoundMethod(const crPlayAmbSoundMethod& handle):
//crMethod(handle),
//m_soundName(handle.m_soundName),
//m_soundState(NULL)
//{
//}
//
//void crPlayAmbSoundMethod::inputParam(int i, void *param)
//{
//}
//
//void crPlayAmbSoundMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_soundName = str;
//		break;
//	}
//}
//
//void crPlayAmbSoundMethod::operator()(crHandle &handle)
//{
//	if(!m_soundState)
//		m_soundState = CRAL::crSoundManager::instance()->findSoundState(m_soundName);
//	if(m_soundState && !m_soundState->isPlaying())
//		m_soundState->setPlay(true);
//}
/////////////////////////////////////////
//
//crCloseDlgMethod
//
/////////////////////////////////////////
crCloseDlgMethod::crCloseDlgMethod(){}
crCloseDlgMethod::crCloseDlgMethod(const crCloseDlgMethod& handle):
crMethod(handle)
{
}
void crCloseDlgMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crCloseDlgMethod::addParam(int i, const std::string& str)
{

}

void crCloseDlgMethod::operator()(crHandle &handle)
{
	crImageStage *stage = m_element->getParentStage();
	if(!stage) return;
	cr2DStageManager::getInstance()->close(stage);
}
/////////////////////////////////////////
//
//crHideDlgMethod
//
/////////////////////////////////////////
crHideDlgMethod::crHideDlgMethod(){}
crHideDlgMethod::crHideDlgMethod(const crHideDlgMethod& handle):
crMethod(handle)
{
}
void crHideDlgMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crHideDlgMethod::addParam(int i, const std::string& str)
{

}

void crHideDlgMethod::operator()(crHandle &handle)
{
	crImageStage *stage = m_element->getParentStage();
	if(!stage) return;
	//stage->showWindow(false);
	cr2DStageManager::getInstance()->showStage(stage,false);
}
/////////////////////////////////////////
//
//crCloseStageMethod
//
/////////////////////////////////////////
crCloseStageMethod::crCloseStageMethod(){}
crCloseStageMethod::crCloseStageMethod(const crCloseStageMethod& handle):
crMethod(handle),
m_stage(m_stage)
{
}
void crCloseStageMethod::inputParam(int i, void *param)
{
}
void crCloseStageMethod::addParam(int i, const std::string& str)
{
	if(i==0)
		m_stage = str;
}

void crCloseStageMethod::operator()(crHandle &handle)
{
	cr2DStageManager::getInstance()->close(m_stage);
}
/////////////////////////////////////////
//
//crPauseMethod
//
/////////////////////////////////////////
crPauseMethod::crPauseMethod():
m_threadType(2){}
crPauseMethod::crPauseMethod(const crPauseMethod& handle):
crMethod(handle),
m_threadType(handle.m_threadType)
{
}
void crPauseMethod::inputParam(int i, void *param)
{
}
void crPauseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_threadType = atoi(str.c_str());
		break;
	}
}
void crPauseMethod::operator()(crHandle &handle)
{
	crBrain::getInstance()->pauseGame(m_threadType);
}

/////////////////////////////////////////
//
//crResumeMethod
//
/////////////////////////////////////////
crResumeMethod::crResumeMethod(){}
crResumeMethod::crResumeMethod(const crResumeMethod& handle):
crMethod(handle)
{
}
void crResumeMethod::inputParam(int i, void *param)
{
}
void crResumeMethod::addParam(int i, const std::string& str)
{
}
void crResumeMethod::operator()(crHandle &handle)
{
	crBrain::getInstance()->resumeGame();
}
/////////////////////////////////////////
//
//crExitMethod
//
/////////////////////////////////////////
crExitMethod::crExitMethod(){}
crExitMethod::crExitMethod(const crExitMethod& handle):
crMethod(handle)
{
}
void crExitMethod::inputParam(int i, void *param)
{
}
void crExitMethod::addParam(int i, const std::string& str)
{
}
void crExitMethod::operator()(crHandle &handle)
{
	ref_ptr<CRProducer::crViewer>bindview = crStartHandler::getInstance()->getViewer();
	if(bindview.valid()) bindview->setDone(true);
}
/////////////////////////////////////////
//
//crHideCursorMethod
//
/////////////////////////////////////////
crHideCursorMethod::crHideCursorMethod():
m_capture(false){}
crHideCursorMethod::crHideCursorMethod(const crHideCursorMethod& handle):
crMethod(handle),
m_capture(handle.m_capture)
{
}
void crHideCursorMethod::inputParam(int i, void *param)
{
}
void crHideCursorMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_capture = (bool)(atoi(str.c_str()));
		break;
	}
}
void crHideCursorMethod::operator()(crHandle &handle)
{
	CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;
	bindview->hideCursor(m_capture);
}
/////////////////////////////////////////
//
//crShowStageMethod
//
/////////////////////////////////////////
crShowStageMethod::crShowStageMethod(){}
crShowStageMethod::crShowStageMethod(const crShowStageMethod& handle):
crMethod(handle),
m_stageDlgString(handle.m_stageDlgString),
m_show(handle.m_show)
{
}
void crShowStageMethod::inputParam(int i, void *param)
{
}
void crShowStageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageDlgString = str;
		break;
	case 1:
		m_show = atoi(str.c_str());
		break;
	//case 2:
	//	m_cursor = atoi(str.c_str());
	//  break;
	}
}
void crShowStageMethod::operator()(crHandle &handle)
{
	switch(m_show)
	{
	case 0:
		{
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)
			{
				imageStage->showWindow(false);
			}
		}
		break;
	case 1:
		cr2DStageManager::getInstance()->showStage(m_stageDlgString,true);
		break;
	case 2:
		cr2DStageManager::getInstance()->doModal(m_stageDlgString);
		break;
	case 3:
		{
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)// cr2DStageManager::getInstance()->showStage(imageStage,imageStage->isHide());
			{
				imageStage->showWindow(imageStage->isHide());
			}
		}
		break;
	case 4:
		{
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)
			{
				imageStage->showWindow(true);
			}
		}
		break;
	case 5:
		{//隐藏窗口，与窗口同线程
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)
			{
				imageStage->showWindow(false,false);
			}
		}
		break;
	case 6:
		{//显示窗口，与窗口同线程
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)
			{
				imageStage->showWindow(true,false);
			}
		}
		break;
	case 7:
		{//窗口显示在鼠标位置
			crImageStage * imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
			if(imageStage)
			{
				POINT point;
				GetCursorPos(&point);
				//RECT rect;
				//rect.left = 0;
				//rect.top = 0;
				if(imageStage->getNeedParentWindow())
				{
					Window parent = crStartHandler::getInstance()->getWindowHandle();
					if(parent)
					{
						//GetWindowRect(parent,&rect);
						//int h = GetSystemMetrics(SM_CYSIZE);
						//rect.top += h;
						ScreenToClient(parent,&point);
					}
				}
				imageStage->setPos(point.x/*-rect.left*/,point.y/*-rect.top*/);
				cr2DStageManager::getInstance()->showStage(imageStage,true);
			}
		}
		break;
	}

}
/////////////////////////////////////////
//
//crCloseAllModelessStageMethod
//
/////////////////////////////////////////
crCloseAllModelessStageMethod::crCloseAllModelessStageMethod(){}
crCloseAllModelessStageMethod::crCloseAllModelessStageMethod(const crCloseAllModelessStageMethod& handle):
crMethod(handle)
{
}
void crCloseAllModelessStageMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crCloseAllModelessStageMethod::addParam(int i, const std::string& str)
{
}
void crCloseAllModelessStageMethod::operator()(crHandle &handle)
{
	crImageStage *stage = NULL;
	if(m_element.valid())
		stage = m_element->getParentStage();
    cr2DStageManager::getInstance()->closeAllModelessStage(stage);
}
/////////////////////////////////////////
//
//crRedrawStageMethod
//
/////////////////////////////////////////
crRedrawStageMethod::crRedrawStageMethod(){}
crRedrawStageMethod::crRedrawStageMethod(const crRedrawStageMethod& handle):
crMethod(handle),
m_stageDlgString(handle.m_stageDlgString)
{
}
void crRedrawStageMethod::inputParam(int i, void *param)
{
}
void crRedrawStageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageDlgString = str;
		break;
	}
}
void crRedrawStageMethod::operator()(crHandle &handle)
{
	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
	if(imageStage && !imageStage->isHide()) 
	{
		imageStage->setNeedReDraw();
	}
}
/////////////////////////////////////////
//
//crUpdateAndRedrawStageMethod
//
/////////////////////////////////////////
crUpdateAndRedrawStageMethod::crUpdateAndRedrawStageMethod(){}
crUpdateAndRedrawStageMethod::crUpdateAndRedrawStageMethod(const crUpdateAndRedrawStageMethod& handle):
crMethod(handle),
m_stageDlgString(handle.m_stageDlgString)
{
}
void crUpdateAndRedrawStageMethod::inputParam(int i, void *param)
{
}
void crUpdateAndRedrawStageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageDlgString = str;
		break;
	}
}
void crUpdateAndRedrawStageMethod::operator()(crHandle &handle)
{
	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
	if(imageStage && !imageStage->isHide()) 
	{
		imageStage->updateWindow();
		imageStage->setNeedReDraw();
	}
}
/////////////////////////////////////////
//
//crInitStageMethod
//
/////////////////////////////////////////
crInitStageMethod::crInitStageMethod(){}
crInitStageMethod::crInitStageMethod(const crInitStageMethod& handle):
crMethod(handle),
m_stageDlgString(handle.m_stageDlgString)
{
}
void crInitStageMethod::inputParam(int i, void *param)
{
}
void crInitStageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageDlgString = str;
		break;
	}
}
void crInitStageMethod::operator()(crHandle &handle)
{
	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageDlgString);
	if(imageStage) imageStage->initWindow();
}
/////////////////////////////////////////
//
//crScrollBarInitMethod
//
/////////////////////////////////////////
crScrollBarInitMethod::crScrollBarInitMethod(){}
crScrollBarInitMethod::crScrollBarInitMethod(const crScrollBarInitMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID)
{
}
void crScrollBarInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_scrollbar = (crScrollBarElement*)(param);
}
void crScrollBarInitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_elementNameID = str;
		break;
	}
}
void crScrollBarInitMethod::operator()(crHandle &handle)
{
	crImageStage *stage = m_scrollbar->getParentStage();
	if(!stage) return;
	crElement *upd_element = stage->getElement(m_elementNameID);
	crVector4f scrollRange = upd_element->getScrollRange();
	if(upd_element)
	{
		switch(m_scrollbar->getScrollBarType()) 
		{
		case crScrollBarElement::HORIZONTALBAR:
			m_scrollbar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
			//upd_element->updateHScrollValue(scrollBar.getValue());
			break;
		case crScrollBarElement::VERTICALBAR:
			m_scrollbar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
			//upd_element->updateVScrollValue(scrollBar.getValue());
			break;
		}
	}
}
/////////////////////////////////////////
//
//crMultiSwitchListBoxUpdateMethod
//
/////////////////////////////////////////
crMultiSwitchListBoxUpdateMethod::crMultiSwitchListBoxUpdateMethod(){}
crMultiSwitchListBoxUpdateMethod::crMultiSwitchListBoxUpdateMethod(const crMultiSwitchListBoxUpdateMethod& handle):
crMethod(handle),
m_multiSwitch(handle.m_multiSwitch)
{
}
void crMultiSwitchListBoxUpdateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}
void crMultiSwitchListBoxUpdateMethod::addParam(int i, const std::string& str)
{
	if(i==0)
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId(str);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		m_multiSwitch = dynamic_cast<CRCore::crMultiSwitch *>(searchByNameVisitor.getResult());
	}
}
void crMultiSwitchListBoxUpdateMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		m_multiSwitch->setActiveSwitchSet(m_listbox->getSelect());
	}
}
/////////////////////////////////////////
//
//crListBoxFileInitMethod
//
/////////////////////////////////////////
crListBoxFileInitMethod::crListBoxFileInitMethod(){}
crListBoxFileInitMethod::crListBoxFileInitMethod(const crListBoxFileInitMethod& handle):
crMethod(handle),
m_filePath(handle.m_filePath),
m_fileExt(handle.m_fileExt)
{
}

bool crListBoxFileInitMethod::checkExt(const std::string &filename, const std::string &ext)
{
	std::string fileExt;
	size_t lastpoint = filename.find_last_of('.');
	if(lastpoint!= -1)
	{
		fileExt = filename.substr(lastpoint+1,filename.length());
	}

	return fileExt.compare(ext.c_str()) == 0;
}

void crListBoxFileInitMethod::findFileInDir(const char* rootDir, const std::string &ext, std::vector<std::string>& fileNameVec)
{
	char fname[1024];
	ZeroMemory(fname, 1024);

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hSearch;

	char filePathName[1024];
	char tmpPath[1024];
	ZeroMemory(filePathName, 1024);
	ZeroMemory(tmpPath, 1024);

	strcpy(filePathName, rootDir);

	bool bSearchFinished = FALSE;

	if( filePathName[strlen(filePathName) -1] != '/' )
	{
		strcat(filePathName, "/");
	}
	strcat(filePathName, "*");
	hSearch = FindFirstFile(filePathName, &fd);

	if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		strcpy(tmpPath, rootDir);
		strcat(tmpPath, fd.cFileName);
		strcat(tmpPath, "/"/*"\\"*/);
		findFileInDir(tmpPath, ext, fileNameVec);
	}
	else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") && checkExt(fd.cFileName,ext) )
	{
		if( rootDir[strlen(rootDir) -1] != '/' )
			sprintf(fname, "%s/%s\0",rootDir,fd.cFileName);
		else
			sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
		fileNameVec.push_back(fname);
	}

	while( !bSearchFinished )
	{
		if( FindNextFile(hSearch, &fd) )
		{
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
			{
				strcpy(tmpPath, rootDir);
				strcat(tmpPath, fd.cFileName);
				strcat(tmpPath, "/");
				findFileInDir(tmpPath, ext, fileNameVec);
			}
			else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") && checkExt(fd.cFileName,ext) )
			{
				if( rootDir[strlen(rootDir) -1] != '/' )
					sprintf(fname, "%s/%s\0",rootDir,fd.cFileName);
				else
					sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
				fileNameVec.push_back(fname);
			}
		}
		else
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				bSearchFinished = TRUE;
			}
			else
				bSearchFinished = TRUE;
		}
	}
	FindClose(hSearch);
}

void crListBoxFileInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListBoxFileInitMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePath = str;
		break;
	case 1:
		m_fileExt = str;
		break;
	}
}

void crListBoxFileInitMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		std::vector<std::string> fileNameVec;
		findFileInDir(m_filePath.c_str(),m_fileExt,fileNameVec);
		m_listbox->clearData();
		if(m_fileExt.compare("crs") == 0)
			m_listbox->addData("NewScene");
		std::string filename;
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
			filename = *itr;
			filename.erase(0,m_filePath.length());
			if(itr->find("NewScene.crs") == -1)
				m_listbox->addData(crArgumentParser::getFileNameEliminateExt(filename/*crArgumentParser::getSimpleFileName(*itr)*/));
		}
		m_listbox->updateData();
	}
}

/////////////////////////////////////////
//
//crListBoxTextUpdateExMethod
//
/////////////////////////////////////////
crListBoxTextUpdateExMethod::crListBoxTextUpdateExMethod(){}
crListBoxTextUpdateExMethod::crListBoxTextUpdateExMethod(const crListBoxTextUpdateExMethod& handle):
crMethod(handle),
m_staticTextBoxName(handle.m_staticTextBoxName),
m_idTextMap(handle.m_idTextMap)
{
}
void crListBoxTextUpdateExMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListBoxTextUpdateExMethod::addParam(int i, const std::string& str)
{//"id,text"
	if(i==0)
	{
		m_staticTextBoxName = str;
	}
	else if(i%2==0)
	{
		m_idTextMap.insert(std::make_pair(m_lastid,str));	
	}
	else
	{
		m_lastid = str;
	}
}

void crListBoxTextUpdateExMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crStaticTextBoxElement *upd_element = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_staticTextBoxName));
		if(upd_element)
		{
			int select = m_listbox->getSelect();
			if(select > -1 && select < m_idTextMap.size())
			{
				const CRCore::crVector4i &rect = upd_element->getRect();
				std::string data;
				m_listbox->getData(select,data);
				IDTextMap::iterator itr = m_idTextMap.find(data);
				if(itr != m_idTextMap.end())
					upd_element->setStringArrayByString(itr->second);
			}
		}
	}
}
/////////////////////////////////////////
//
//crListBoxImageInitMethod
//
/////////////////////////////////////////
crListBoxImageInitMethod::crListBoxImageInitMethod(){}
crListBoxImageInitMethod::crListBoxImageInitMethod(const crListBoxImageInitMethod& handle):
crMethod(handle)
{
}
void crListBoxImageInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListBoxImageInitMethod::addParam(int i, const std::string& str)
{
}

void crListBoxImageInitMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
        m_listbox->updateData();
	}
}
/////////////////////////////////////////
//
//crListBoxImageUpdateMethod
//
/////////////////////////////////////////
crListBoxImageUpdateMethod::crListBoxImageUpdateMethod(){}
crListBoxImageUpdateMethod::crListBoxImageUpdateMethod(const crListBoxImageUpdateMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID),
m_imageNameList(handle.m_imageNameList)
{
}
void crListBoxImageUpdateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListBoxImageUpdateMethod::addParam(int i, const std::string& str)
{//"id,text"
	if(i == 0)
		m_elementNameID = str;
	else
		m_imageNameList.push_back(str);
}

void crListBoxImageUpdateMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crElement *upd_element = stage->getElement(m_elementNameID);
		if(upd_element)
		{
			int select = m_listbox->getSelect();
			if(select > -1 && select < m_imageNameList.size())
			{
				const CRCore::crVector4i &rect = upd_element->getRect();
				upd_element->setElementImage(m_imageNameList[select],crVector2i(rect[2],rect[3]),crVector2i(0,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crListBoxImageUpdateExMethod
//
/////////////////////////////////////////
crListBoxImageUpdateExMethod::crListBoxImageUpdateExMethod(){}
crListBoxImageUpdateExMethod::crListBoxImageUpdateExMethod(const crListBoxImageUpdateExMethod& handle):
crMethod(handle),
m_imageBoxName(handle.m_imageBoxName),
m_idImageMap(handle.m_idImageMap)
{
}
void crListBoxImageUpdateExMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListBoxImageUpdateExMethod::addParam(int i, const std::string& str)
{//"id,imagefilename"
	if(i==0)
	{
		m_imageBoxName = str;
	}
	else if(i%2==0)
	{
		m_idImageMap.insert(std::make_pair(m_lastid,str));	
	}
	else
	{
		m_lastid = str;
	}
}

void crListBoxImageUpdateExMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crElement *upd_element = stage->getElement(m_imageBoxName);
		if(upd_element)
		{
			int select = m_listbox->getSelect();
			if(select > -1 && select < m_idImageMap.size())
			{
				const CRCore::crVector4i &rect = upd_element->getRect();
				std::string data;
				m_listbox->getData(select,data);
				IDImageMap::iterator itr = m_idImageMap.find(data);
				if(itr != m_idImageMap.end())
					upd_element->setElementImage(itr->second,crVector2i(rect[2],rect[3]),crVector2i(0,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crCheckBoxPlayPreDrawMethod
//
/////////////////////////////////////////
crCheckBoxPlayPreDrawMethod::crCheckBoxPlayPreDrawMethod(){}
crCheckBoxPlayPreDrawMethod::crCheckBoxPlayPreDrawMethod(const crCheckBoxPlayPreDrawMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID),
m_timeInterval(handle.m_timeInterval),
m_lastUpdateTime(0.0)
{
}
void crCheckBoxPlayPreDrawMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkbox = (CRGUI::crCheckBoxElement*)param;
}

void crCheckBoxPlayPreDrawMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_elementNameID = str;
		break;
	case 1:
		m_timeInterval = atof(str.c_str());
		break;
	}
}

void crCheckBoxPlayPreDrawMethod::operator()(crHandle &handle)
{
	if(m_checkbox.valid())
	{
		crImageStage *stage = m_checkbox->getParentStage();
		if(!stage) return;

		CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
		float dt = CRCore::Timer::instance()->delta_s( m_lastUpdateTime, t1 );
		if(dt < m_timeInterval) return;
		m_lastUpdateTime = t1;

		crListBoxElement *upd_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_elementNameID));
		if(upd_element)
		{
			if(m_checkbox->getSelect())
			{
				int i = upd_element->getSelect();
				int size = upd_element->getDataSize();
				if(i + 1 < size)
					upd_element->select( i + 1);
				else
					upd_element->select(0);
			}
		}
	}
}
/////////////////////////////////////////
//
//crButtonBoxNextPageMethod
//
/////////////////////////////////////////
crButtonBoxNextPageMethod::crButtonBoxNextPageMethod(){}
crButtonBoxNextPageMethod::crButtonBoxNextPageMethod(const crButtonBoxNextPageMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID)
{
}
void crButtonBoxNextPageMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_buttonbox = (CRGUI::crButtonElement*)param;
}

void crButtonBoxNextPageMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_elementNameID = str;
		break;
	}
}

void crButtonBoxNextPageMethod::operator()(crHandle &handle)
{
	if(m_buttonbox.valid())
	{
		crImageStage *stage = m_buttonbox->getParentStage();
		if(!stage) return;

		crListBoxElement *upd_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_elementNameID));
		if(upd_element)
		{
			int i = upd_element->getSelect();
			int size = upd_element->getDataSize();
			if(i + 1 < size)
				upd_element->select( i + 1);
		}
	}
}
/////////////////////////////////////////
//
//crButtonBoxPrePageMethod
//
/////////////////////////////////////////
crButtonBoxPrePageMethod::crButtonBoxPrePageMethod(){}
crButtonBoxPrePageMethod::crButtonBoxPrePageMethod(const crButtonBoxPrePageMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID)
{
}
void crButtonBoxPrePageMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_buttonbox = (CRGUI::crButtonElement*)param;
}

void crButtonBoxPrePageMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_elementNameID = str;
		break;
	}
}

void crButtonBoxPrePageMethod::operator()(crHandle &handle)
{
	if(m_buttonbox.valid())
	{
		crImageStage *stage = m_buttonbox->getParentStage();
		if(!stage) return;

		crListBoxElement *upd_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_elementNameID));
		if(upd_element)
		{
			int i = upd_element->getSelect();
			int size = upd_element->getDataSize();
			if(i > 0)
				upd_element->select(i - 1);
		}
	}
}
/////////////////////////////////////////
//
//crLoadingProgressInitMethod
//
/////////////////////////////////////////
crLoadingProgressInitMethod::crLoadingProgressInitMethod(){}
crLoadingProgressInitMethod::crLoadingProgressInitMethod(const crLoadingProgressInitMethod& handle):
crMethod(handle)
{
}
void crLoadingProgressInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_progress = (CRGUI::crProgressElement*)param;
}

void crLoadingProgressInitMethod::addParam(int i, const std::string& str)
{
}

void crLoadingProgressInitMethod::operator()(crHandle &handle)
{
	if(m_progress.valid())
	{
		m_progress->resetProgress();
	}
}

/////////////////////////////////////////
//
//crClearStaticTextMethod
//
/////////////////////////////////////////
crClearStaticTextMethod::crClearStaticTextMethod(){}
crClearStaticTextMethod::crClearStaticTextMethod(const crClearStaticTextMethod& handle):
crMethod(handle)
{
}
void crClearStaticTextMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crStaticTextBoxElement*)param;
}

void crClearStaticTextMethod::addParam(int i, const std::string& str)
{
}

void crClearStaticTextMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		m_element->clearString();
	}
}

/////////////////////////////////////////
//
//crClearEditTextMethod
//
/////////////////////////////////////////
crClearEditTextMethod::crClearEditTextMethod(){}
crClearEditTextMethod::crClearEditTextMethod(const crClearEditTextMethod& handle):
crMethod(handle)
{
}
void crClearEditTextMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crEditBoxElement*)param;
}

void crClearEditTextMethod::addParam(int i, const std::string& str)
{
}

void crClearEditTextMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		m_element->clearString();
	}
}
/////////////////////////////////////////
//
//crDSFullScreenInitMethod
//
/////////////////////////////////////////
crDSFullScreenInitMethod::crDSFullScreenInitMethod(){}
crDSFullScreenInitMethod::crDSFullScreenInitMethod(const crDSFullScreenInitMethod& handle):
crMethod(handle)
{
}
void crDSFullScreenInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}
void crDSFullScreenInitMethod::addParam(int i, const std::string& str)
{
}
void crDSFullScreenInitMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		m_checkBox->select(crDisplaySettings::instance()->getFullScreen());
	}
}
/////////////////////////////////////////
//
//crDSVsyncInitMethod
//
/////////////////////////////////////////
crDSVsyncInitMethod::crDSVsyncInitMethod(){}
crDSVsyncInitMethod::crDSVsyncInitMethod(const crDSVsyncInitMethod& handle):
crMethod(handle)
{
}
void crDSVsyncInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}
void crDSVsyncInitMethod::addParam(int i, const std::string& str)
{
}
void crDSVsyncInitMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		m_checkBox->select(crDisplaySettings::instance()->getVsync());
	}
}
/////////////////////////////////////////
//
//crDSShaderInitMethod
//
/////////////////////////////////////////
crDSShaderInitMethod::crDSShaderInitMethod(){}
crDSShaderInitMethod::crDSShaderInitMethod(const crDSShaderInitMethod& handle):
crMethod(handle)
{
}
void crDSShaderInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}
void crDSShaderInitMethod::addParam(int i, const std::string& str)
{
}
void crDSShaderInitMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		m_checkBox->select(crDisplaySettings::instance()->getDisableShader());
	}
}
/////////////////////////////////////////
//
//crDSResolutionComboInitMethod
//
/////////////////////////////////////////
crDSResolutionComboInitMethod::crDSResolutionComboInitMethod(){}
crDSResolutionComboInitMethod::crDSResolutionComboInitMethod(const crDSResolutionComboInitMethod& handle):
crMethod(handle)
{
}
void crDSResolutionComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSResolutionComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSResolutionComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		int width = crDisplaySettings::instance()->getScreenWidth();
		int height = crDisplaySettings::instance()->getScreenHeight();
		int resolution = 0;
		if(width <= 800 && height <= 600) resolution = 0;
		else if(width <= 1024 && height <= 768) resolution = 1;
		else if(width <= 1152 && height <= 864) resolution = 2;
		else if(width <= 1280 && height <= 800) resolution = 3;
		else if(width <= 1280 && height <= 1024) resolution = 4;
		else if(width <= 1366 && height <= 768) resolution = 5;
		else if(width <= 1440 && height <= 900) resolution = 6;
		else if(width <= 1680 && height <= 1050) resolution = 7;
		else if(width <= 1920 && height <= 1080) resolution = 8;

		m_combo->select(resolution);
		//CRCore::notify(CRCore::FATAL)<<"crResolutionComboInitCallback "<<width<<" "<<height<<" resolution = "<<resolution<<std::endl;
	}
}
/////////////////////////////////////////
//
//crDSBrightnessSliderInitMethod
//
/////////////////////////////////////////
crDSBrightnessSliderInitMethod::crDSBrightnessSliderInitMethod(){}
crDSBrightnessSliderInitMethod::crDSBrightnessSliderInitMethod(const crDSBrightnessSliderInitMethod& handle):
crMethod(handle)
{
}
void crDSBrightnessSliderInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_slider = (CRGUI::crSliderElement*)param;
}
void crDSBrightnessSliderInitMethod::addParam(int i, const std::string& str)
{
}
void crDSBrightnessSliderInitMethod::operator()(crHandle &handle)
{
	if(m_slider.valid())
	{
		float brightness = crDisplaySettings::instance()->getBrightness();
		m_slider->setValue(brightness);
	}
}
/////////////////////////////////////////
//
//crDSMouseSpeedSliderInitMethod
//
/////////////////////////////////////////
crDSMouseSpeedSliderInitMethod::crDSMouseSpeedSliderInitMethod(){}
crDSMouseSpeedSliderInitMethod::crDSMouseSpeedSliderInitMethod(const crDSMouseSpeedSliderInitMethod& handle):
crMethod(handle)
{
}
void crDSMouseSpeedSliderInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_slider = (CRGUI::crSliderElement*)param;
}
void crDSMouseSpeedSliderInitMethod::addParam(int i, const std::string& str)
{
}
void crDSMouseSpeedSliderInitMethod::operator()(crHandle &handle)
{
	if(m_slider.valid())
	{
		float mouseSpeed = crDisplaySettings::instance()->getMouseSpeed();
		m_slider->setValue(mouseSpeed);
	}
}
/////////////////////////////////////////
//
//crDSTextureDetailComboInitMethod
//
/////////////////////////////////////////
crDSTextureDetailComboInitMethod::crDSTextureDetailComboInitMethod(){}
crDSTextureDetailComboInitMethod::crDSTextureDetailComboInitMethod(const crDSTextureDetailComboInitMethod& handle):
crMethod(handle)
{
}
void crDSTextureDetailComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSTextureDetailComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSTextureDetailComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getTextureDetail());
	}
}
/////////////////////////////////////////
//
//crDSTextureAnisotropyComboInitMethod
//
/////////////////////////////////////////
crDSTextureAnisotropyComboInitMethod::crDSTextureAnisotropyComboInitMethod(){}
crDSTextureAnisotropyComboInitMethod::crDSTextureAnisotropyComboInitMethod(const crDSTextureAnisotropyComboInitMethod& handle):
crMethod(handle)
{
}
void crDSTextureAnisotropyComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSTextureAnisotropyComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSTextureAnisotropyComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		int anisotropy = crDisplaySettings::instance()->getMaxAnisotropy();
		int ap = 0;
		switch (anisotropy)
		{
		case 1:
			ap = 0;
			break;
		case 2:
			ap = 1;
			break;
		case 4:
			ap = 2;
			break;
		case 8:
			ap = 3;
			break;
		case 16:
			ap = 4;
			break;
		}
		m_combo->select(ap);
	}
}
/////////////////////////////////////////
//
//crDSShadowDetailComboInitMethod
//
/////////////////////////////////////////
crDSShadowDetailComboInitMethod::crDSShadowDetailComboInitMethod(){}
crDSShadowDetailComboInitMethod::crDSShadowDetailComboInitMethod(const crDSShadowDetailComboInitMethod& handle):
crMethod(handle)
{
}
void crDSShadowDetailComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSShadowDetailComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSShadowDetailComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getShadowDetail());
	}
}
/////////////////////////////////////////
//
//crDSAlphaShadowComboInitMethod
//
/////////////////////////////////////////
crDSAlphaShadowComboInitMethod::crDSAlphaShadowComboInitMethod(){}
crDSAlphaShadowComboInitMethod::crDSAlphaShadowComboInitMethod(const crDSAlphaShadowComboInitMethod& handle):
crMethod(handle)
{
}
void crDSAlphaShadowComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSAlphaShadowComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSAlphaShadowComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getEnableAlphaShadow());
	}
}
/////////////////////////////////////////
//
//crDSMotionBlurComboInitMethod
//
/////////////////////////////////////////
crDSMotionBlurComboInitMethod::crDSMotionBlurComboInitMethod(){}
crDSMotionBlurComboInitMethod::crDSMotionBlurComboInitMethod(const crDSMotionBlurComboInitMethod& handle):
crMethod(handle)
{
}
void crDSMotionBlurComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSMotionBlurComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSMotionBlurComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getMotionBlur());
	}
}
/////////////////////////////////////////
//
//crDSHDRComboInitMethod
//
/////////////////////////////////////////
crDSHDRComboInitMethod::crDSHDRComboInitMethod(){}
crDSHDRComboInitMethod::crDSHDRComboInitMethod(const crDSHDRComboInitMethod& handle):
crMethod(handle)
{
}
void crDSHDRComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSHDRComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSHDRComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getHDR());
	}
}
/////////////////////////////////////////
//
//crDSDepthOfFieldComboInitMethod
//
/////////////////////////////////////////
crDSDepthOfFieldComboInitMethod::crDSDepthOfFieldComboInitMethod(){}
crDSDepthOfFieldComboInitMethod::crDSDepthOfFieldComboInitMethod(const crDSDepthOfFieldComboInitMethod& handle):
crMethod(handle)
{
}
void crDSDepthOfFieldComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}

void crDSDepthOfFieldComboInitMethod::addParam(int i, const std::string& str)
{
}

void crDSDepthOfFieldComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		m_combo->select(crDisplaySettings::instance()->getDepthOfField());
	}
}
/////////////////////////////////////////
//
//crDSAntialiasingComboInitMethod
//
/////////////////////////////////////////
crDSAntialiasingComboInitMethod::crDSAntialiasingComboInitMethod(){}
crDSAntialiasingComboInitMethod::crDSAntialiasingComboInitMethod(const crDSAntialiasingComboInitMethod& handle):
crMethod(handle)
{
}
void crDSAntialiasingComboInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_combo = (CRGUI::crComboBoxElement*)param;
}
void crDSAntialiasingComboInitMethod::addParam(int i, const std::string& str)
{
}
void crDSAntialiasingComboInitMethod::operator()(crHandle &handle)
{
	if(m_combo.valid())
	{
		int fsaa = crDisplaySettings::instance()->getNumMultiSamples();
		int af = 0;
		switch (fsaa)
		{
		case 1:
			af = 0;
			break;
		case 2:
			af = 1;
			break;
		case 4:
			af = 2;
			break;
		case 8:
			af = 3;
			break;
		case 16:
			af = 4;
			break;
		}

		m_combo->select(af);
	}
}
/////////////////////////////////////////
//
//crDSResetDisplaySettingMethod
//
/////////////////////////////////////////
crDSResetDisplaySettingMethod::crDSResetDisplaySettingMethod():
m_fullScreen(false),
m_vsync(false),
m_resolution(0),
m_brightness(0.0f),
m_textureDetail(1),
m_textureAnisotropy(3),
m_shadowDetail(0),
m_alphaShadow(0),
m_motionBlur(0),
m_hdr(0),
m_depthOfField(0),
m_antialiasing(0),
m_mouseSpeed(0.3),
m_disableShader(false){}
crDSResetDisplaySettingMethod::crDSResetDisplaySettingMethod(const crDSResetDisplaySettingMethod& handle):
crMethod(handle),
m_fullScreenElement(handle.m_fullScreenElement),
m_vsyncElement(handle.m_vsyncElement),
m_resolutionComboElement(handle.m_resolutionComboElement),
m_brightnessSliderElement(handle.m_brightnessSliderElement),
m_textureDetailComboElement(handle.m_textureDetailComboElement),
m_textureAnisotropyComboElement(handle.m_textureAnisotropyComboElement),
m_shadowDetailComboElement(handle.m_shadowDetailComboElement),
m_alphaShadowComboElement(handle.m_alphaShadowComboElement),
m_motionBlurComboElement(handle.m_motionBlurComboElement),
m_hdrComboElement(handle.m_hdrComboElement),
m_depthOfFieldComboElement(handle.m_depthOfFieldComboElement),
m_antialiasingComboElement(handle.m_antialiasingComboElement),
m_mouseSpeedSliderElement(handle.m_mouseSpeedSliderElement),
m_disableShaderElement(handle.m_disableShaderElement),
m_fullScreen(handle.m_fullScreen),
m_vsync(handle.m_vsync),
m_resolution(handle.m_resolution),
m_brightness(handle.m_brightness),
m_textureDetail(handle.m_textureDetail),
m_textureAnisotropy(handle.m_textureAnisotropy),
m_shadowDetail(handle.m_shadowDetail),
m_alphaShadow(handle.m_alphaShadow),
m_motionBlur(handle.m_motionBlur),
m_hdr(handle.m_hdr),
m_depthOfField(handle.m_depthOfField),
m_antialiasing(handle.m_antialiasing),
m_mouseSpeed(handle.m_mouseSpeed),
m_disableShader(handle.m_disableShader)
{
}
void crDSResetDisplaySettingMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crDSResetDisplaySettingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fullScreenElement = str;
		break;
	case 1:
		m_fullScreen = atoi(str.c_str());
		break;
	case 2:
		m_vsyncElement = str;
		break;
	case 3:
		m_vsync = atoi(str.c_str());
		break;
	case 4:
		m_resolutionComboElement = str;
		break;
	case 5:
		m_resolution = atoi(str.c_str());
		break;
	case 6:
		m_brightnessSliderElement = str;		
		break;
	case 7:
		m_brightness = atof(str.c_str());
		break;
	case 8:
		m_textureDetailComboElement = str;
		break;
	case 9:
		m_textureDetail = atoi(str.c_str());
		break;
	case 10:
		m_textureAnisotropyComboElement = str;
		break;
	case 11:
		m_textureAnisotropy = atoi(str.c_str());
		break;
	case 12:
		m_shadowDetailComboElement = str;
		break;
	case 13:
		m_shadowDetail = atoi(str.c_str());
		break;
	case 14:
		m_alphaShadowComboElement = str;
		break;
	case 15:
		m_alphaShadow = atoi(str.c_str());
		break;
	case 16:
		m_motionBlurComboElement = str;
		break;
	case 17:
		m_motionBlur = atoi(str.c_str());
		break;
	case 18:
		m_hdrComboElement = str;
		break;
	case 19:
		m_hdr = atoi(str.c_str());
		break;
	case 20:
		m_depthOfFieldComboElement = str;
		break;
	case 21:
		m_depthOfField = atoi(str.c_str());
		break;
	case 22:
		m_antialiasingComboElement = str;
		break;
	case 23:
		m_antialiasing = atoi(str.c_str());
		break;
	case 24:
		m_mouseSpeedSliderElement = str;
		break;
	case 25:
		m_mouseSpeed = atoi(str.c_str());
		break;
	case 26:
		m_disableShaderElement = str;
		break;
	case 27:
		m_disableShader = atoi(str.c_str());
		break;
	}
}
void crDSResetDisplaySettingMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crCheckBoxElement *check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_fullScreenElement));
		if(check_element)
		{
			check_element->select(m_fullScreen);
		}
		check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_vsyncElement));
		if(check_element)
		{
			check_element->select(m_vsync);
		}
		crComboBoxElement *combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_resolutionComboElement));
		if(combo_element)
		{
			combo_element->select(m_resolution);
			//CRCore::notify(CRCore::FATAL)<<"resolution_element = "<<resolution<<std::endl;
		}
		crSliderElement *slider_element = dynamic_cast<crSliderElement *>(stage->getElement(m_brightnessSliderElement));
		if(slider_element)
		{
			slider_element->setValue(m_brightness);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_textureDetailComboElement));
		if(combo_element)
		{
			combo_element->select(m_textureDetail);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_textureAnisotropyComboElement));
		if(combo_element)
		{
			combo_element->select(m_textureAnisotropy);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_shadowDetailComboElement));
		if(combo_element)
		{
			combo_element->select(m_shadowDetail);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_alphaShadowComboElement));
		if(combo_element)
		{
			combo_element->select(m_alphaShadow);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_motionBlurComboElement));
		if(combo_element)
		{
			combo_element->select(m_motionBlur);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_hdrComboElement));
		if(combo_element)
		{
			combo_element->select(m_hdr);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_depthOfFieldComboElement));
		if(combo_element)
		{
			combo_element->select(m_depthOfField);
		}
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_antialiasingComboElement));
		if(combo_element)
		{
			combo_element->select(m_antialiasing);
		}
		slider_element = dynamic_cast<crSliderElement *>(stage->getElement(m_mouseSpeedSliderElement));
		if(slider_element)
		{
			slider_element->setValue(m_mouseSpeed);
		}
		check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_disableShaderElement));
		if(check_element)
		{
			check_element->select(m_disableShader);
		}
	}
}
/////////////////////////////////////////
//
//crDSUpdateInitCfgMethod
//
/////////////////////////////////////////
crDSUpdateInitCfgMethod::crDSUpdateInitCfgMethod(){}
crDSUpdateInitCfgMethod::crDSUpdateInitCfgMethod(const crDSUpdateInitCfgMethod& handle):
crMethod(handle),
m_fullScreenElement(handle.m_fullScreenElement),
m_vsyncElement(handle.m_vsyncElement),
m_resolutionComboElement(handle.m_resolutionComboElement),
m_brightnessSliderElement(handle.m_brightnessSliderElement),
m_textureDetailComboElement(handle.m_textureDetailComboElement),
m_textureAnisotropyComboElement(handle.m_textureAnisotropyComboElement),
m_shadowDetailComboElement(handle.m_shadowDetailComboElement),
m_alphaShadowComboElement(handle.m_alphaShadowComboElement),
m_motionBlurComboElement(handle.m_motionBlurComboElement),
m_hdrComboElement(handle.m_hdrComboElement),
m_depthOfFieldComboElement(handle.m_depthOfFieldComboElement),
m_antialiasingComboElement(handle.m_antialiasingComboElement),
m_mouseSpeedSliderElement(handle.m_mouseSpeedSliderElement),
m_disableShaderElement(handle.m_disableShaderElement)
{
}
void crDSUpdateInitCfgMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crDSUpdateInitCfgMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fullScreenElement = str;
		break;
	case 1:
		m_vsyncElement = str;
		break;
	case 2:
		m_resolutionComboElement = str;
		break;
	case 3:
		m_brightnessSliderElement = str;		
		break;
	case 4:
		m_textureDetailComboElement = str;
		break;	
	case 5:
		m_textureAnisotropyComboElement = str;
		break;
	case 6:
		m_shadowDetailComboElement = str;
		break;
	case 7:
		m_alphaShadowComboElement = str;
		break;
	case 8:
		m_motionBlurComboElement = str;
		break;
	case 9:
		m_hdrComboElement = str;
		break;
	case 10:
		m_depthOfFieldComboElement = str;
		break;
	case 11:
		m_antialiasingComboElement = str;
		break;
	case 12:
		m_mouseSpeedSliderElement = str;
		break;
	case 13:
		m_disableShaderElement = str;
		break;
	}
}
void crDSUpdateInitCfgMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		bool fullScreen = true;
		crCheckBoxElement *check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_fullScreenElement));
		if(check_element)
		{
			fullScreen = check_element->getSelect();
		}
        
		bool vsync = false;
		check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_vsyncElement));
		if(check_element)
		{
			vsync = check_element->getSelect();
		}

		int resolution = -1;
		crComboBoxElement *combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_resolutionComboElement));
		if(combo_element)
		{
			resolution = combo_element->getSelectIndex();
		}

		float brightness = 0.0001f;
		crSliderElement *slider_element = dynamic_cast<crSliderElement *>(stage->getElement(m_brightnessSliderElement));
		if(slider_element)
		{
			brightness = slider_element->getValue();
		}

		int textureDetail = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_textureDetailComboElement));
		if(combo_element)
		{
			textureDetail = combo_element->getSelectIndex();
		}

		int textureAnisotropy = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_textureAnisotropyComboElement));
		if(combo_element)
		{
			textureAnisotropy = combo_element->getSelectIndex();
		}
		switch (textureAnisotropy)
		{
		case 0:
			textureAnisotropy = 1;
			break;
		case 1:
			textureAnisotropy = 2;
			break;
		case 2:
			textureAnisotropy = 4;
			break;
		case 3:
			textureAnisotropy = 8;
			break;
		case 4:
			textureAnisotropy = 16;
			break;
		}

		int shadowDetail = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_shadowDetailComboElement));
		if(combo_element)
		{
			shadowDetail = combo_element->getSelectIndex();
		}

		int alphaShadow = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_alphaShadowComboElement));
		if(combo_element)
		{
			alphaShadow = combo_element->getSelectIndex();
		}

		int motionBlur = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_motionBlurComboElement));
		if(combo_element)
		{
			motionBlur = combo_element->getSelectIndex();
		}

		int hdr = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_hdrComboElement));
		if(combo_element)
		{
			hdr = combo_element->getSelectIndex();
		}

		int dof = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_depthOfFieldComboElement));
		if(combo_element)
		{
			dof = combo_element->getSelectIndex();
		}

		int antialiasing = 0;
		combo_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_antialiasingComboElement));
		if(combo_element)
		{
			antialiasing = combo_element->getSelectIndex();
		}
		switch (antialiasing)
		{
		case 0:
			antialiasing = 0;
			break;
		case 1:
			antialiasing = 2;
			break;
		case 2:
			antialiasing = 4;
			break;
		case 3:
			antialiasing = 8;
			break;
		case 4:
			antialiasing = 16;
			break;
		}

		float mouseSpeed = 0.3f;
		slider_element = dynamic_cast<crSliderElement *>(stage->getElement(m_mouseSpeedSliderElement));
		if(slider_element)
		{
			mouseSpeed = slider_element->getValue();
		}

		bool disableShader = false;
		check_element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_disableShaderElement));
		if(check_element)
		{
			disableShader = check_element->getSelect();
		}

		rcfg::ConfigScript cfg_script;
		std::string filename = "script/init.cfg";
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
		{
			CRCore::notify(CRCore::FATAL)<<"crDSUpdateInitCfgMethod(): file open error,"<<filename<<std::endl;
			return;
		}

		bool needRestart = false;
		crDisplaySettings *ds = crDisplaySettings::instance();
		int width = ds->getScreenWidth();
		int height = ds->getScreenHeight();
		int height2;
		if (ds->isOwnWindow())
		{//WebGame
			if(!ds->getFullScreen())
			{
				int shellH = 0;
				RECT   Rect; 
				HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
				if(GetWindowRect(hWnd,&Rect))   
				{ 
					shellH = Rect.bottom-Rect.top;//   就是任务栏的高度 
					if(shellH >= height)
						shellH = 0;
				}
				height2 = height + shellH;
			}
			if(fullScreen != ds->getFullScreen())
			{
				needRestart = true;
				ds->setFullScreen(fullScreen);
			}
			switch (resolution)
			{
			case 0:
				if(width != 800 || (height != 600 && height2 != 600))
					needRestart = true;
				width = 800;
				height = 600;
				break;
			case 1:
				if(width != 1024 || (height != 768 && height2 != 768))
					needRestart = true;
				width = 1024;
				height = 768;
				break;
			case 2:
				if(width != 1152 || (height != 864 && height2 != 864))
					needRestart = true;
				width = 1152;
				height = 864;
				break;
			case 3:
				if(width != 1280 || (height != 800 && height2 != 800))
					needRestart = true;
				width = 1280;
				height = 800;
				break;
			case 4:
				if(width != 1280 || (height != 1024 && height2 != 1024))
					needRestart = true;
				width = 1280;
				height = 1024;
				break;
			case 5:
				if(width != 1366 || (height != 768 && height2 != 768))
					needRestart = true;
				width = 1366;
				height = 768;
				break;
			case 6:
				if(width != 1440 || (height != 900 && height2 != 900))
					needRestart = true;
				width = 1440;
				height = 900;
				break;
			case 7:
				if(width != 1680 || (height != 1050 && height2 != 1050))
					needRestart = true;
				width = 1680;
				height = 1050;
				break;
			case 8:
				if(width != 1920 || (height != 1080 && height2 != 1080))
					needRestart = true;
				width = 1920;
				height = 1080;
				break;
			}
			//if(needRestart)
			//{
			//	CRGUI::cr2DStageManager::getInstance()->doModal(m_noticeDlg);
			//}
		}
		ds->setVsync(vsync);
		ds->setBrightness(brightness);
		ds->setTextureDetail(textureDetail);
		ds->setMaxAnisotropy(textureAnisotropy);
		ds->setShadowDetail(shadowDetail);
		ds->setEnableAlphaShadow(alphaShadow);
		ds->setMotionBlur(motionBlur);
		if(motionBlur)
		{
			ds->setMinimumNumAccumBits(8,8,8,8);
		}
		else
		{
			ds->setMinimumNumAccumBits(0,0,0,0);
		}
		ds->setHDR(hdr);
		ds->setDepthOfField(dof);
		if(dof)
		{
			ds->setNeedPixelDepth(true);
		}
		else
		{
			ds->setNeedPixelDepth(false);
		}
		ds->setNumMultiSamples(antialiasing);
		ds->setDisableShader(disableShader);
		//ds->setNPR(npr);
		if(cfg_script.Push("DisplaySetting"))
		{
			if(!cfg_script.Modify("FullScreen",fullScreen?1:0))
			{
				cfg_script.Add("FullScreen",fullScreen?1:0);
			}
			if(!cfg_script.Modify("Vsync",vsync?1:0))
			{
				cfg_script.Add("Vsync",vsync?1:0);
			}
			if(!cfg_script.Modify("FPSControl",60))
			{
				cfg_script.Add("FPSControl",60);
			}
			if(!cfg_script.Modify("DisableShader",disableShader?1:0))
			{
				cfg_script.Add("DisableShader",disableShader?1:0);
			}
			if(!cfg_script.Modify("screenWidth",width))
			{
				cfg_script.Add("screenWidth",width);
			}
			if(!cfg_script.Modify("screenHeight", height))
			{
				cfg_script.Add("screenHeight",height);
			}
			if(!cfg_script.Modify("Brightness", brightness))
			{
				cfg_script.Add("Brightness",brightness);
			}
			if(!cfg_script.Modify("TextureDetail", textureDetail))
			{
				cfg_script.Add("TextureDetail",textureDetail);
			}
			if(!cfg_script.Modify("MaxAnisotropy", textureAnisotropy))
			{
				cfg_script.Add("MaxAnisotropy",textureAnisotropy);
			}
			if(!cfg_script.Modify("ShadowDetail", shadowDetail))
			{
				cfg_script.Add("ShadowDetail",shadowDetail);
			}
			if(!cfg_script.Modify("EnableAlphaShadow", alphaShadow))
			{
				cfg_script.Add("EnableAlphaShadow",alphaShadow);
			}
			if(!cfg_script.Modify("MotionBlur", motionBlur))
			{
				cfg_script.Add("MotionBlur",motionBlur);
			}
			std::vector<float> v_i;
			if(motionBlur)
			{
				v_i.push_back(8);
				v_i.push_back(8);
				v_i.push_back(8);
				v_i.push_back(8);
			}
			else
			{
				v_i.push_back(0);
				v_i.push_back(0);
				v_i.push_back(0);
				v_i.push_back(0);
			}
			if(!cfg_script.Modify("minimumNumberAccumBits", v_i))
			{
				cfg_script.Add("minimumNumberAccumBits",v_i);
			}
			if(!cfg_script.Modify("HDR", hdr))
			{
				cfg_script.Add("HDR",hdr);
			}
			if(!cfg_script.Modify("DepthOfField", dof))
			{
				cfg_script.Add("DepthOfField",dof);
			}
			if(dof)
			{
				if(!cfg_script.Modify("NeedPixelDepth", 1))
				{
					cfg_script.Add("NeedPixelDepth",1);
				}
			}
			if(!cfg_script.Modify("numMultiSamples", antialiasing))
			{
				cfg_script.Add("numMultiSamples",antialiasing);
			}
			if(!cfg_script.Modify("MouseSpeed", mouseSpeed))
			{
				cfg_script.Add("MouseSpeed",mouseSpeed);
			}
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"crUpdateInitCfgMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
		}
		cfg_script.Write("script/init.cfg");
		cfg_script.Close();

		CRIOManager::crWriteCookFile scopedWrite("script/init.cfg");
		needRestart = false;
		if(needRestart)
		{
			_putenv("CRE_RESTART=1");
			cr2DStageManager::getInstance()->closeAll();
		}
		handle.outputParam(0,&needRestart);
	}
}
/////////////////////////////////////////
//
//crSaveListInitMethod
//
/////////////////////////////////////////
crSaveListInitMethod::crSaveListInitMethod(){}
crSaveListInitMethod::crSaveListInitMethod(const crSaveListInitMethod& handle):
crMethod(handle)
{
}
void crSaveListInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crSaveListInitMethod::addParam(int i, const std::string& str)
{
}

void crSaveListInitMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		m_listbox->clearData();
		crSceneManager *sceneManager = crSceneManager::getInstance();
		crSceneManager::SaveVec& saveVec = sceneManager->getSaveVec();
		for( crSceneManager::SaveVec::iterator itr = saveVec.begin();
			itr != saveVec.end();
			++itr )
		{
			m_listbox->addData((*itr)->getName()/* + " " + sceneManager->getSceneID((*itr)->m_sceneid)*/);
		}
		//m_listbox->select(0);
		m_listbox->updateData();
	}
}
/////////////////////////////////////////
//
//crSaveListUpdateMethod
//
/////////////////////////////////////////
crSaveListUpdateMethod::crSaveListUpdateMethod(){}
crSaveListUpdateMethod::crSaveListUpdateMethod(const crSaveListUpdateMethod& handle):
crMethod(handle),
m_imageBoxName(handle.m_imageBoxName),
m_staticTextBoxName(handle.m_staticTextBoxName),
m_imageNameList(handle.m_imageNameList)
{
}
void crSaveListUpdateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crSaveListUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{  
	case 0:
		m_imageBoxName = str;
		break;
	case 1:
		m_staticTextBoxName = str;
		break;
	default:
		m_imageNameList.push_back(str);
	}
}

void crSaveListUpdateMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crSceneManager *sceneManager = crSceneManager::getInstance();
		sceneManager->setCurrentSaveIndex(m_listbox->getSelect());

		crImageBoxElement *imageBoxElement = dynamic_cast<crImageBoxElement *>(stage->getElement(m_imageBoxName));
		if(imageBoxElement)
		{
			int sceneid = sceneManager->getCurrentSceneIndex();
			if(sceneid > -1 && sceneid < m_imageNameList.size())
			{
				const CRCore::crVector4i &rect = imageBoxElement->getRect();
				imageBoxElement->setElementImage(m_imageNameList[sceneid],crVector2i(rect[2],rect[3]),crVector2i(0,0));
			}
		}
		crStaticTextBoxElement *staticTextBoxElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_staticTextBoxName));
		if(staticTextBoxElement)
		{
			staticTextBoxElement->setStringArrayByString(sceneManager->getSave(sceneManager->getCurrentSaveIndex())->getSaveTime());
		}
	}
}
/////////////////////////////////////////
//
//crValidEditStrMethod
//
/////////////////////////////////////////
crValidEditStrMethod::crValidEditStrMethod(){}
crValidEditStrMethod::crValidEditStrMethod(const crValidEditStrMethod& handle):
crMethod(handle),
m_elementName(handle.m_elementName)
{
}
void crValidEditStrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editbox = (CRGUI::crEditBoxElement*)param;
}

void crValidEditStrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_elementName = str;
		break;
	}
}

void crValidEditStrMethod::operator()(crHandle &handle)
{
	if(m_editbox.valid())
	{
		crImageStage *stage = m_editbox->getParentStage();
		if(!stage) return;
		crElement *validElement = stage->getElement(m_elementName);
		if(validElement)
		{
			validElement->setEnable(!(m_editbox->getStringArrayInString().empty()));
		}
	}
}
/////////////////////////////////////////
//
//crCreateSaveMethod
//
/////////////////////////////////////////
crCreateSaveMethod::crCreateSaveMethod():
m_sceneid(0){}
crCreateSaveMethod::crCreateSaveMethod(const crCreateSaveMethod& handle):
crMethod(handle),
m_name(handle.m_name),
m_botName(handle.m_botName),
m_gameState(handle.m_gameState),
m_sceneid(handle.m_sceneid),
m_editBoxName(handle.m_editBoxName)
{
}
void crCreateSaveMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crCreateSaveMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		if(str.compare("SingleInit")==0)
			m_gameState = crSave::SingleInit;
		else if(str.compare("NetInit")==0)
			m_gameState = crSave::NetInit;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_botName = str;
		break;
	case 3:
		m_sceneid = atoi(str.c_str());
		break;
	case 4:
		m_editBoxName = str;
		break;
	}
}

void crCreateSaveMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crSceneManager *sceneManager = crSceneManager::getInstance();
		if(!m_editBoxName.empty())
		{
			crEditBoxElement *editBoxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_editBoxName));
			if(editBoxElement)
			{
				m_name = editBoxElement->getString(0);
			}
		}
		int id = sceneManager->getNumSave();
		crSave *save = new crSave;
        save->setName(m_name);
	    save->setBotName(m_botName);
		save->setSceneID(m_sceneid);

		time_t t = time( 0 ); 
		char tmp[64];
		strftime( tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t) );
		save->setSaveTime(tmp);
		save->setGameState(m_gameState);
		save->setTeam(1);
		sceneManager->pushSave(save);
		wantToLoadSave(id);
	}
}
/////////////////////////////////////////
//
//crDeleteSaveMethod
//
/////////////////////////////////////////
crDeleteSaveMethod::crDeleteSaveMethod(){}
crDeleteSaveMethod::crDeleteSaveMethod(const crDeleteSaveMethod& handle):
crMethod(handle)
{
}
void crDeleteSaveMethod::inputParam(int i, void *param)
{
}

void crDeleteSaveMethod::addParam(int i, const std::string& str)
{
}

void crDeleteSaveMethod::operator()(crHandle &handle)
{
	crSceneManager *sceneManager = crSceneManager::getInstance();
	sceneManager->deleteSave(sceneManager->getCurrentSaveIndex());
}
///////////////////////////////////////////
////
////crSaveMethod
////
///////////////////////////////////////////
//crSaveMethod::crSaveMethod(){}
//crSaveMethod::crSaveMethod(const crSaveMethod& handle):
//crMethod(handle)
//{
//}
//void crSaveMethod::inputParam(int i, void *param)
//{
//}
//
//void crSaveMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crSaveMethod::operator()(crHandle &handle)
//{
//	crSceneManager *sceneManager = crSceneManager::getInstance();
//	crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
//	CRPhysics::crBotManager *botManager = CRPhysics::crBotManager::getInstance();
//	CRPhysics::crViewMatterObject *cameraBot = botManager->getFirstPlayer();
//	save->setHp(cameraBot->getGameBodyInfo()->getHP());
//	save->setDefend(cameraBot->getGameBodyInfo()->getDefend());
//	//save->setTeam(cameraBot->getGameBodyInfo()->getTeam());
//	save->setExperience(cameraBot->getGameBodyInfo()->getExperience());
//	save->setPosition(cameraBot->getTrans());
//
//	crQuat orientation;
//	orientation.set(cameraBot->getMatrix());
//	//float x,y,z;
//	//orientation.getAngles(x,y,z);
//	save->setOrientation(orientation/*crVector3f(x,z,y)*/);
//
//	//CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(cameraBot->getParent(0));
//	//if(!bodyMG)
//	//{
//	//	CRCore::notify(CRCore::FATAL)<<"CRGUI::crSaveCallback(): cameraBot的父节点类型不正确"<<std::endl;
//	//	return;
//	//}
//	//CRAI::crNerveCellGroup *bodyNC = dynamic_cast<CRAI::crNerveCellGroup *>(bodyMG->getAttachedEventGroup());
//	//if(!bodyNC)
//	//{
//	//	CRCore::notify(CRCore::FATAL)<<"CRGUI::crSaveCallback(): bodyMG的AttachedEventGroup类型不正确"<<std::endl;
//	//	return;
//	//}
//	//CRAI::crAIGroup *ai = bodyNC->getAI();
//	//if(!ai)
//	//{
//	//	CRCore::notify(CRCore::FATAL)<<"CRGUI::crSaveCallback(): 智能节点不正确"<<std::endl;
//	//	return;
//	//}
//	//std::string *str = ai->getKeyEventString("3");
//	//if(str) save->setWeapon(0,*str);
//	//str = ai->getKeyEventString("4");
//	//if(str) save->setWeapon(1,*str);
//	//str = ai->getKeyEventString("5");
//	//if(str) save->setWeapon(2,*str);
//
//	time_t t = time( 0 ); 
//	char tmp[64];
//	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t) );
//	save->setSaveTime(tmp);
//}
/////////////////////////////////////////
//
//crSceneMapInitMethod
//
/////////////////////////////////////////
crSceneMapInitMethod::crSceneMapInitMethod(){}
crSceneMapInitMethod::crSceneMapInitMethod(const crSceneMapInitMethod& handle):
crMethod(handle),
m_sceneidMapFileMap(handle.m_sceneidMapFileMap)
{
}
void crSceneMapInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imagebox = (CRGUI::crImageBoxElement*)param;
}

void crSceneMapInitMethod::addParam(int i, const std::string& str)
{//"sceneid,imagefilename"
	if(i%2==0)
	{
		m_lastsceneid = str;
	}
	else
	{
		m_sceneidMapFileMap.insert(std::make_pair(m_lastsceneid,str));
	}
}

void crSceneMapInitMethod::operator()(crHandle &handle)
{
	if(m_imagebox.valid())
	{
		const CRCore::crVector4i &rect = m_imagebox->getRect();
		crSceneManager *sceneManager = crSceneManager::getInstance();
		std::string sceneid = sceneManager->getCurrentSceneName();
		m_imagebox->setElementImage(m_sceneidMapFileMap[sceneid],crVector2i(rect[2],rect[3]),crVector2i(0,0));
	}
}
/////////////////////////////////////////
//
//crSceneInfoInitMethod
//
/////////////////////////////////////////
crSceneInfoInitMethod::crSceneInfoInitMethod(){}
crSceneInfoInitMethod::crSceneInfoInitMethod(const crSceneInfoInitMethod& handle):
crMethod(handle),
m_sceneidInfoMap(handle.m_sceneidInfoMap)
{
}
void crSceneInfoInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_staticTextBox = (CRGUI::crStaticTextBoxElement*)param;
}

void crSceneInfoInitMethod::addParam(int i, const std::string& str)
{//"sceneid,infotext"
	if(i%2==0)
	{
		m_lastsceneid = str;
	}
	else
	{
		m_sceneidInfoMap.insert(std::make_pair(m_lastsceneid,str));
	}
}

void crSceneInfoInitMethod::operator()(crHandle &handle)
{
	if(m_staticTextBox.valid())
	{
		crSceneManager *sceneManager = crSceneManager::getInstance();
		std::string sceneid = sceneManager->getCurrentSceneName();
		m_staticTextBox->setStringArrayByString(m_sceneidInfoMap[sceneid]);
	}
}
/////////////////////////////////////////
//
//crSceneListUpdateMethod
//
/////////////////////////////////////////
crSceneListUpdateMethod::crSceneListUpdateMethod(){}
crSceneListUpdateMethod::crSceneListUpdateMethod(const crSceneListUpdateMethod& handle):
crMethod(handle)
{
}
void crSceneListUpdateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listBox = (CRGUI::crListBoxElement*)param;
}

void crSceneListUpdateMethod::addParam(int i, const std::string& str)
{
}

void crSceneListUpdateMethod::operator()(crHandle &handle)
{
	if(m_listBox.valid())
	{
		crSceneManager *sceneManager = crSceneManager::getInstance();
		crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
		save->setSceneID(m_listBox->getSelect());
	}
}

/////////////////////////////////////////
//
//crAircraftListInitMethod
//
/////////////////////////////////////////
crAircraftListInitMethod::crAircraftListInitMethod(){}
crAircraftListInitMethod::crAircraftListInitMethod(const crAircraftListInitMethod& handle):
crMethod(handle),
m_sceneidAircraftMap(handle.m_sceneidAircraftMap)
{
}
void crAircraftListInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listBox = (CRGUI::crListBoxElement*)param;
}

void crAircraftListInitMethod::addParam(int i, const std::string& str)
{//"sceneid,aircraftname"
	std::string sceneid,aircraftname;
	std::string::size_type comma = str.find_first_of(',');
	sceneid = std::string(str.begin(),str.begin()+comma);
	aircraftname = std::string(str.begin()+comma + 1,str.end());
	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::crAircraftListInitCallback(): str = "<<str<<" sceneid = "<<sceneid<<" aircraftname = "<<aircraftname<<std::endl;
	m_sceneidAircraftMap.insert(std::make_pair(sceneid,aircraftname));
}

void crAircraftListInitMethod::operator()(crHandle &handle)
{
	if(m_listBox.valid())
	{
		m_listBox->clearData();
		crSceneManager *sceneManager = crSceneManager::getInstance();
		std::string sceneid = sceneManager->getCurrentSceneName();
		SceneIDAircraftMap::iterator itr = m_sceneidAircraftMap.find(sceneid);
		for( ; itr != m_sceneidAircraftMap.end() && sceneid.compare(itr->first) == 0;
			++itr )
		{
			m_listBox->addData(itr->second);
		}
		//listBox.select(0);
		m_listBox->updateData();
	}
}
/////////////////////////////////////////
//
//crAircraftListUpdateMethod
//
/////////////////////////////////////////
crAircraftListUpdateMethod::crAircraftListUpdateMethod(){}
crAircraftListUpdateMethod::crAircraftListUpdateMethod(const crAircraftListUpdateMethod& handle):
crMethod(handle)
{
}
void crAircraftListUpdateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listBox = (CRGUI::crListBoxElement*)param;
}

void crAircraftListUpdateMethod::addParam(int i, const std::string& str)
{
}

void crAircraftListUpdateMethod::operator()(crHandle &handle)
{
	if(m_listBox.valid() && m_listBox->getDataSize()>0)
	{
		crSceneManager *sceneManager = crSceneManager::getInstance();
		crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
		std::string data;
		m_listBox->getData(m_listBox->getSelect(),data);
		save->setBotName(data);
	}
}
/////////////////////////////////////////
//
//crWeaponListInitMethod
//
/////////////////////////////////////////
crWeaponListInitMethod::crWeaponListInitMethod(){}
crWeaponListInitMethod::crWeaponListInitMethod(const crWeaponListInitMethod& handle):
crMethod(handle)
{
}
void crWeaponListInitMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listBox = (CRGUI::crListBoxElement*)param;
}

void crWeaponListInitMethod::addParam(int i, const std::string& str)
{//"aircraftname,weaponname"
	std::string aircraftname,weaponname;
	std::string::size_type comma = str.find_first_of(',');
	aircraftname = std::string(str.begin(),str.begin()+comma);
	weaponname = std::string(str.begin()+comma + 1,str.end());
	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::crAircraftListInitCallback(): str = "<<str<<" aircraftname = "<<aircraftname<<" weaponname = "<<weaponname<<std::endl;
	m_aircraftWeaponMap.insert(std::make_pair(aircraftname,weaponname));
}

void crWeaponListInitMethod::operator()(crHandle &handle)
{
	if(m_listBox.valid())
	{
		m_listBox->clearData();
		crSceneManager *sceneManager = crSceneManager::getInstance();
		crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
		std::string aircraftname = save->getBotName();
		AircraftWeaponMap::iterator itr = m_aircraftWeaponMap.find(aircraftname);
		m_listBox->addData("NULL");
		for( ; itr != m_aircraftWeaponMap.end() && aircraftname.compare(itr->first) == 0;
			++itr )
		{
			m_listBox->addData(itr->second);
		}
		//listBox.select(0);
		m_listBox->updateData();
	}
}
/////////////////////////////////////////
//
//crInitGameMethod
//
/////////////////////////////////////////
crInitGameMethod::crInitGameMethod(){}
crInitGameMethod::crInitGameMethod(const crInitGameMethod& handle):
crMethod(handle)
{
	m_weaponComboName[0] = handle.m_weaponComboName[0];
	m_weaponComboName[1] = handle.m_weaponComboName[1];
	m_weaponComboName[2] = handle.m_weaponComboName[2];
}
void crInitGameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crInitGameMethod::addParam(int i, const std::string& str)
{	
	if(i>=0 && i<3)
        m_weaponComboName[i] = str;
}

void crInitGameMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crSceneManager *sceneManager = crSceneManager::getInstance();
		crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
		if(save)
		{
			crComboBoxElement *upd_element;
			for(int i = 0; i<3; i++)
			{
				upd_element = dynamic_cast<crComboBoxElement *>(stage->getElement(m_weaponComboName[i]));
				save->setWeapon(i,upd_element->getValue());
			}

			switch (save->getGameState())
			{
			case crSave::SingleRunning:
				save->setGameState(crSave::SingleInit);
				break;
			case crSave::NetRunning:
				save->setGameState(crSave::NetInit);
				break;
			}
		}
		else
		{
			/*save = new crSave;
			save->setName("Default");
			sceneManager->pushSave(save);*/
			CRCore::notify(CRCore::FATAL)<<"没有设置记录，请检查save.cfg"<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crInitModelViewMethod
//
/////////////////////////////////////////
//crInitModelViewMethod::crInitModelViewMethod(){}
//crInitModelViewMethod::crInitModelViewMethod(const crInitModelViewMethod& handle):
//crMethod(handle),
//m_modelListName(handle.m_modelListName),
//m_typeListName(handle.m_typeListName)
//{
//}
//void crInitModelViewMethod::inputParam(int i, void *param)
//{
//	if(i==0)
//		m_element = (CRGUI::crElement*)param;
//}
//
//void crInitModelViewMethod::addParam(int i, const std::string& str)
//{	
//	switch(i)
//	{
//	case 0:
//		m_modelListName = str;
//		break;
//	case 1:
//		m_typeListName = str;
//		break;
//	}
//}
//
//void crInitModelViewMethod::operator()(crHandle &handle)
//{
//	if(m_element.valid())
//	{
//		crImageStage *stage = m_element->getParentStage();
//		if(!stage) return;
//
//		crListBoxElement *modelList_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_modelListName));
//		crListBoxElement *typeList_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_typeListName));
//		if(modelList_element && modelList_element->getDataSize()>0 && typeList_element && typeList_element->getDataSize()>0)
//		{
//			std::string modelName = modelList_element->getData(modelList_element->getSelect());
//			std::string typeName = typeList_element->getData(typeList_element->getSelect());
//			crSceneManager *sceneManager = crSceneManager::getInstance();
//			int id = sceneManager->getNumSave();
//			crSave *save;
//			if(id == 0)
//			{
//				save = new crSave;
//				save->setName("Scene");
//				sceneManager->pushSave(save);
//			}
//			else
//			{
//				save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
//			}
//
//			if(typeName.compare("NULL")==0)
//			{
//				save->m_sceneid = 1;
//				sceneManager->getSceneMap()["Editor"] = modelName;
//			}
//			else
//			{
//				save->m_sceneid = 0;
//				ItemMap itemMap;
//				rcfg::ConfigScript cfg_script(&itemMap);
//				cfg_script.Add(typeName);
//				cfg_script.Push(typeName);
//				std::vector<float> v_i;
//				v_i.push_back(0);
//				v_i.push_back(0);
//				v_i.push_back(0);
//				cfg_script.Add("Center",v_i);
//				cfg_script.Add("Radius",-1);
//				cfg_script.Add("RangeMode","DISTANCE_FROM_EYE_POINT");
//				cfg_script.Add("Scene");
//				cfg_script.Push("Scene");
//				cfg_script.Add("FileName",modelName);
//				v_i.clear();
//				v_i.push_back(0);
//				v_i.push_back(FLT_MAX);
//				cfg_script.Add("Range",v_i);
//				cfg_script.Add("TimeStamp",0);
//				cfg_script.Add("PriorityOffset",0);
//				cfg_script.Add("PriorityScale",1);
//				cfg_script.Pop();
//				cfg_script.Pop();
//
//				cfg_script.Write("script/editorpage.cfg");
//				cfg_script.Close();
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crPreviewImageMethod
//
/////////////////////////////////////////
crPreviewImageMethod::crPreviewImageMethod(){}
crPreviewImageMethod::crPreviewImageMethod(const crPreviewImageMethod& handle):
crMethod(handle),
m_elementNameID(handle.m_elementNameID),
m_filePath(handle.m_filePath)
{
}
void crPreviewImageMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crPreviewImageMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_elementNameID = str;
		break;
	case 1:
		m_filePath = str;
		break;
	}
}

void crPreviewImageMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crElement *upd_element = stage->getElement(m_elementNameID);
		if(upd_element)
		{
			std::string file;
			m_listbox->getData(m_listbox->getSelect(),file);
			std::string imgfile = m_filePath + "/" + file + ".bmp";
			const CRCore::crVector4i &rect = upd_element->getRect();
			upd_element->setElementImage(imgfile,crVector2i(rect[2],rect[3]),crVector2i(0,0));
		}
	}
}
/////////////////////////////////////////
//
//crAddListDataMethod
//
/////////////////////////////////////////
crAddListDataMethod::crAddListDataMethod(){}
crAddListDataMethod::crAddListDataMethod(const crAddListDataMethod& handle):
crMethod(handle),
m_strList(handle.m_strList)
{
}
void crAddListDataMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crAddListDataMethod::addParam(int i, const std::string& str)
{
	m_strList.push_back(str);
}

void crAddListDataMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		for( StrList::iterator itr = m_strList.begin();
			 itr != m_strList.end();
			 ++itr )
		{
			m_listbox->addData(*itr);
		}
		m_listbox->updateData();
	}
}
/////////////////////////////////////////
//
//crInitEditorMethod
//
/////////////////////////////////////////
crInitEditorMethod::crInitEditorMethod(){}
crInitEditorMethod::crInitEditorMethod(const crInitEditorMethod& handle):
crMethod(handle),
m_sceneListName(handle.m_sceneListName)
{
}
void crInitEditorMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crInitEditorMethod::addParam(int i, const std::string& str)
{	
	switch(i)
	{
	case 0:
		m_sceneListName = str;
		break;
	}
}

void crInitEditorMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crListBoxElement *sceneList_element = dynamic_cast<crListBoxElement *>(stage->getElement(m_sceneListName));
		if(sceneList_element && sceneList_element->getDataSize()>0)
		{
			std::string sceneName;
			sceneList_element->getData(sceneList_element->getSelect(),sceneName);

			crScene *scene = crSceneManager::getInstance()->getCurrentScene();
			if(!scene)
			{
				scene = new crScene;
				crSceneManager::getInstance()->setCurrentScene(scene);
			}
			//sceneName.erase(0,1);
			scene->setSceneNameID(sceneName);
			scene->setSceneFileName(sceneName + ".crs");
			//std::string editor = sceneManager->getSceneID(0);
			//if(editor.compare(sceneName)!=0)
			//{
			//	sceneManager->getSceneMap()[editor] = sceneName + ".crs";
			//	sceneManager->replaceSceneID(editor,sceneName);
			//}
		}
	}
}
///////////////////////////////////////////
////
////crSetRunModeMethod
////
///////////////////////////////////////////
//crSetRunModeMethod::crSetRunModeMethod(){}
//crSetRunModeMethod::crSetRunModeMethod(const crSetRunModeMethod& handle):
//crMethod(handle),
//m_runMode(handle.m_runMode)
//{
//}
//void crSetRunModeMethod::inputParam(int i, void *param)
//{
//}
//
//void crSetRunModeMethod::addParam(int i, const std::string& str)
//{	
//	switch(i)
//	{
//	case 0:
//		m_runMode = str;
//		break;
//	}
//}
//
//void crSetRunModeMethod::operator()(crHandle &handle)
//{
//    if(m_runMode.compare("Editor")==0)
//	{
//        _putenv( "CRE_RunMode=Editor" );
//		CRCore::crDisplaySettings::instance()->setRunMode(0);
//	}
//	else if(m_runMode.compare("Game")==0)
//	{
//		_putenv( "CRE_RunMode=Game" );
//		CRCore::crDisplaySettings::instance()->setRunMode(1);
//	}
//	else
//	{
//		_putenv( "CRE_RunMode=Editor" );
//		CRCore::crDisplaySettings::instance()->setRunMode(0);
//	}
//}
/////////////////////////////////////////
//
//crAddModelMethod
//
/////////////////////////////////////////
crAddModelMethod::crAddModelMethod(){}
crAddModelMethod::crAddModelMethod(const crAddModelMethod& handle):
crMethod(handle)
{
}
void crAddModelMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_root = NULL;
		}
		break;
	case 1:
		m_root = (crGroup*)param;
		break;
	case 3:
		m_dataFile = (std::string*)param;
		break;
	case 4:
		m_addDescription = (std::string*)param;
		break;
	}
}

void crAddModelMethod::addParam(int i, const std::string& str)
{
}

void crAddModelMethod::operator()(crHandle &handle)
{
    if(m_root.valid() && m_dataFile)
	{
		CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
		proxyNode->setName("ProxyNode");
		proxyNode->setDescriptionsInString("DataFile "+*m_dataFile);
		CRIOManager::crLoadManager::getInstance()->requestNode(*m_dataFile,proxyNode.get(),false);
		if(proxyNode->getNumChildren()==1)
		{
			crGroup *cloneGroup = dynamic_cast<crGroup *>(proxyNode->clone(crCopyOp::DEEP_COPY_NODES));
			crNode *loadedNode = cloneGroup->getChild(0);
			if(m_addDescription && !m_addDescription->empty())
				loadedNode->addDescription(*m_addDescription);

			CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root.get(),cloneGroup,true);
			handle.outputParam(1,loadedNode);
		}
	}
}
/////////////////////////////////////////
//
//crAddUIModelMethod
//
/////////////////////////////////////////
crAddUIModelMethod::crAddUIModelMethod(){}
crAddUIModelMethod::crAddUIModelMethod(const crAddUIModelMethod& handle):
crMethod(handle)
{
}
void crAddUIModelMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_root = NULL;
		}
		break;
	case 1:
		m_root = (crGroup*)param;
		break;
	case 3:
		m_dataFile = (std::string*)param;
		break;
	case 4:
		m_addDescription = (std::string*)param;
		break;
	}
}

void crAddUIModelMethod::addParam(int i, const std::string& str)
{
}

void crAddUIModelMethod::operator()(crHandle &handle)
{
	if(m_root.valid() && m_dataFile)
	{
		CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
		proxyNode->setName("ProxyNode");
		proxyNode->setDescriptionsInString("DataFile "+*m_dataFile);

		ref_ptr<crNode> node = CRIOManager::readNodeFile(*m_dataFile);
		if(node.valid())
		{
			ref_ptr<crGroup> dummyGroup = dynamic_cast<crGroup *>(node.get());
			if(dummyGroup.valid()&&(dummyGroup->getName().compare("LoadNodeDummy")==0))
			{
				node = dummyGroup->getChild(0);
				dummyGroup->removeChild(0,dummyGroup->getNumChildren());
			}
			proxyNode->addChild(node.get());
		}

		CRCore::crCollectNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
		searchByNameVisitor.insertNodeNameId("ProxyNode");
		searchByNameVisitor.setSearchNodeType(GROUP);
		crBrain::getInstance()->accept(searchByNameVisitor);
		NodeArray &nodeArray = searchByNameVisitor.getResult();
		ref_ptr<crNode> findNode;
		std::string dataFile;
		std::string loadfile = crArgumentParser::getSimpleFileName(*m_dataFile);
		for( NodeArray::iterator itr = nodeArray.begin();
			 itr != nodeArray.end();
			 ++itr )
		{
			findNode = *itr;
			if(crArgumentParser::readKeyValue(findNode->getDescriptions(),"DataFile",dataFile))
			{
				if(crArgumentParser::getSimpleFileName(dataFile) == loadfile)
				{
					CRIOManager::crLoadManager::getInstance()->requestRemoveNode(findNode->getParent(0),findNode.get());
					MessageBox(::GetActiveWindow(),"移除老的UI","Creature3D",MB_OK);
				}
			}
		}

		//CRIOManager::crLoadManager::getInstance()->requestNode(*m_dataFile,proxyNode.get(),false);
		if(proxyNode->getNumChildren()==1)
		{
			//crGroup *cloneGroup = dynamic_cast<crGroup *>(proxyNode->clone(crCopyOp::DEEP_COPY_NODES));
			crNode *loadedNode = proxyNode->getChild(0);
			if(m_addDescription && !m_addDescription->empty())
				loadedNode->addDescription(*m_addDescription);
			CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root.get(),proxyNode.get(),true);
			addUINodeToScene(dynamic_cast<crGroup *>(loadedNode));
			//handle.outputParam(1,loadedNode);
		}
	}
}
/////////////////////////////////////////
//
//crReloadUIModelMethod
//
/////////////////////////////////////////
crReloadUIModelMethod::crReloadUIModelMethod()
{}
crReloadUIModelMethod::crReloadUIModelMethod(const crReloadUIModelMethod& handle):
crMethod(handle)
{
}
void crReloadUIModelMethod::inputParam(int i, void *param)
{
}

void crReloadUIModelMethod::addParam(int i, const std::string& str)
{
}

void crReloadUIModelMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = body->getGameBodyInfo();
	crData *infoData = gamebodyInfo->getDataClass();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crNode *currentSelectNode = selectNodeVec[0].get();
		if(currentSelectNode)
		{
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setNameId("ProxyNode");
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			currentSelectNode->accept(searchByNameVisitor);
			crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(proxyNode)
			{
				std::string dataFile;
				if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",dataFile))
				{
					ref_ptr<crNode> node = CRIOManager::readNodeFile(dataFile);
					if(node.valid())
					{
						ref_ptr<crGroup> dummyGroup = dynamic_cast<crGroup *>(node.get());
						if(dummyGroup.valid()&&(dummyGroup->getName().compare("LoadNodeDummy")==0))
						{
							node = dummyGroup->getChild(0);
							dummyGroup->removeChild(0,dummyGroup->getNumChildren());
						}
						CRIOManager::crLoadManager::getInstance()->requestRemoveNode(proxyNode,proxyNode->getChild(0));
						CRIOManager::crLoadManager::getInstance()->requestAddNode(proxyNode,node.get(),true);
						addUINodeToScene(dynamic_cast<crGroup *>(node.get()));
					}
				}
			}
		}
	}
}
///////////////////////////////////////////
////
////crLoadUISceneMethod
////
///////////////////////////////////////////
//class LoadUISceneVisitor : public CRCore::crNodeVisitor
//{
//public:
//	LoadUISceneVisitor():
//	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}
//
//	  virtual ~LoadUISceneVisitor(){}
//
//	  void init(crGroup *root) { m_root= root; }
//	  virtual void apply(crGroup& node)
//	  { 
//		  if(node.getName().compare("ProxyNode") == 0)
//		  {
//			  CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root,&node,true);
//			  addUINodeToScene(dynamic_cast<crGroup *>(&node));
//		  }
//		  traverse(node);
//	  }
//protected:
//	crGroup *m_root;
//};
//
//static LoadUISceneVisitor s_loadUISceneVisitor;
crLoadUISceneMethod::crLoadUISceneMethod(){}
crLoadUISceneMethod::crLoadUISceneMethod(const crLoadUISceneMethod& handle):
crMethod(handle)
{
}
void crLoadUISceneMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_root = NULL;
		}
		break;
	case 1:
		m_root = (crGroup*)param;
		break;
	case 3:
		m_dataFile = (std::string*)param;
		break;
	}
}

void crLoadUISceneMethod::addParam(int i, const std::string& str)
{
}

void crLoadUISceneMethod::operator()(crHandle &handle)
{
	if(m_root.valid() && m_dataFile)
	{
		ref_ptr<crNode> node = CRIOManager::readNodeFile(*m_dataFile);
		if(node.valid())
		{
			crLoadUISceneVisitor visitor;
			visitor.init(m_root.get());
			node->accept(visitor);
		}
	}
}
////////////////////////////////
//
//crSaveUISceneMethod
//
///////////////////////////////////
crSaveUISceneMethod::crSaveUISceneMethod()
{}

crSaveUISceneMethod::crSaveUISceneMethod(const crSaveUISceneMethod& handle):
crMethod(handle),
m_fileName(handle.m_fileName)
{
}

void crSaveUISceneMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 3:
		if(param) m_fileName = *((std::string *)param);
		else m_fileName.clear();
		break;
	}
}

void crSaveUISceneMethod::addParam(int i, const std::string& str)
{
}
void crSaveUISceneMethod::operator()(crHandle &handle)
{
	crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId("Filter");
	searchByNameVisitor.setSearchNodeType(ALLNODE);
	crBrain::getInstance()->accept(searchByNameVisitor);
	crNode *node = searchByNameVisitor.getResult();
	if(node)
	{
		CRIOManager::writeNodeFile(*node,m_fileName);
	}
}
/////////////////////////////////////////
//
//crCreateModelMethod
//
/////////////////////////////////////////
crCreateModelMethod::crCreateModelMethod(){}
crCreateModelMethod::crCreateModelMethod(const crCreateModelMethod& handle):
crMethod(handle),
m_createNodeType(handle.m_createNodeType)
{
}
void crCreateModelMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_root = NULL;
		}
		break;
	case 1:
		m_root = (crGroup*)param;
		break;
	}
}

void crCreateModelMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_createNodeType = str;
		break;
	}
}

void crCreateModelMethod::operator()(crHandle &handle)
{
	if(m_root.valid())
	{
		CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
		proxyNode->setName("ProxyNode");
		std::string datafile = "new.cre";
		proxyNode->setDescriptionsInString("DataFile "+datafile);
		crNode *newNode;
		if(!m_createNodeType.empty())
		{
			if(m_createNodeType.compare("crGroup"))
				newNode = new crGroup;
            else if(m_createNodeType.compare("crMultiSwitch"))
				newNode = new crMultiSwitch;
			else if(m_createNodeType.compare("crDOFTransform"))
				newNode = new crDOFTransform;
			else if(m_createNodeType.compare("crLod"))
				newNode = new crLod;
			else if(m_createNodeType.compare("crPagedLOD"))
				newNode = new crPagedLOD;
		}
		else
			newNode = new crGroup;
		proxyNode->addChild(newNode);
		CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root.get(),proxyNode.get(),true);
		handle.outputParam(1,newNode);
	}
}
/////////////////////////////////////////
//
//crInitSceneNameMethod
//
/////////////////////////////////////////
crInitSceneNameMethod::crInitSceneNameMethod(){}
crInitSceneNameMethod::crInitSceneNameMethod(const crInitSceneNameMethod& handle):
crMethod(handle)
{
}
void crInitSceneNameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editbox = (CRGUI::crEditBoxElement*)param;
}
void crInitSceneNameMethod::addParam(int i, const std::string& str)
{
}

void crInitSceneNameMethod::operator()(crHandle &handle)
{
	crScene *scene = crSceneManager::getInstance()->getCurrentScene();
    std::string fileName = scene->getSceneFileName();
	//std::string fileName = crSceneManager::getInstance()->getSceneFileName(crSceneManager::getInstance()->getCurrentSceneName());
	fileName = crArgumentParser::getFileNameEliminateExt(fileName);
	if(fileName.compare("NewScene")==0)
	{
		fileName.clear();
	}
    m_editbox->setStringArrayByString(fileName);
}
/////////////////////////////////////////
//
//crSaveSceneMethod
//
/////////////////////////////////////////
crSaveSceneMethod::crSaveSceneMethod(){}
crSaveSceneMethod::crSaveSceneMethod(const crSaveSceneMethod& handle):
crMethod(handle),
m_fileNameElement(handle.m_fileNameElement),
m_filePath(handle.m_filePath)
{
}
void crSaveSceneMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crSaveSceneMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_fileNameElement = str;
		break;
	case 1:
		m_filePath = str;
		break;
	}
}

void crSaveSceneMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		CRGUI::crEditBoxElement* fileNameElement = dynamic_cast<CRGUI::crEditBoxElement*>(stage->getElement(m_fileNameElement));
		if(fileNameElement)
		{
			std::string fileName = fileNameElement->getStringArrayInString();
			if(fileName.find("NewScene")==std::string::npos)
			{
				fileName = crArgumentParser::getFileNameEliminateExt(crArgumentParser::getSimpleFileName(fileName));
				fileNameElement->setStringArrayByString(fileName);
				CRIOManager::writeObjectFile((*crBrain::getInstance()),m_filePath + fileName + ".crs");

				crScene *scene = crSceneManager::getInstance()->getCurrentScene();
				scene->setSceneFileName(fileName + ".crs");
			}
		}
	}
}
/////////////////////////////////////////
//
//crSaveScene2Method
//
/////////////////////////////////////////
crSaveScene2Method::crSaveScene2Method(){}
crSaveScene2Method::crSaveScene2Method(const crSaveScene2Method& handle):
	crMethod(handle),
	m_filePath(handle.m_filePath)
{
}
void crSaveScene2Method::inputParam(int i, void *param)
{
}
void crSaveScene2Method::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePath = str;
		break;
	}
}

void crSaveScene2Method::operator()(crHandle &handle)
{
	crScene *scene = crSceneManager::getInstance()->getCurrentScene();
	std::string fileName = scene->getSceneFileName();
	if(fileName.find("NewScene")==std::string::npos)
	{
		CRIOManager::writeObjectFile((*crBrain::getInstance()),m_filePath + fileName);
	}
}
/////////////////////////////////////////
//
//crUpdateSelectNodeListMethod
//
/////////////////////////////////////////
crUpdateSelectNodeListMethod::crUpdateSelectNodeListMethod(){}
crUpdateSelectNodeListMethod::crUpdateSelectNodeListMethod(const crUpdateSelectNodeListMethod& handle):
crMethod(handle)
{
}
void crUpdateSelectNodeListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateSelectNodeListMethod::addParam(int i, const std::string& str)
{
}

void crUpdateSelectNodeListMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo)
		{
			m_listbox->clearData();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			//int size = selectNodeVec.size();
			//std::string str;
			for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
				 itr != selectNodeVec.end();
				 ++itr )
			{
				//if(!crArgumentParser::readKeyValue((*itr)->getDescriptions(),"EditorNodeType",str))
				//{
				    m_listbox->addData((*itr)->getName());
				//}
			}
			m_listbox->checkSelect();
		}
	}
}

/////////////////////////////////////////
//
//crDisplayNodeAttrMethod
//
/////////////////////////////////////////
crDisplayNodeAttrMethod::crDisplayNodeAttrMethod(){}
crDisplayNodeAttrMethod::crDisplayNodeAttrMethod(const crDisplayNodeAttrMethod& handle):
crMethod(handle),
m_filePathElement(handle.m_filePathElement),
m_nameElement(handle.m_nameElement),
m_descriptionElement(handle.m_descriptionElement),
m_objSelectedLightElement(handle.m_objSelectedLightElement),
m_avoidLightElement(handle.m_avoidLightElement),
m_needLightElement(handle.m_needLightElement),
m_positionTextElement(handle.m_positionTextElement),
m_posxElement(handle.m_posxElement),
m_posyElement(handle.m_posyElement),
m_poszElement(handle.m_poszElement),
m_staticMeshElement(handle.m_staticMeshElement),
m_freezeElement(handle.m_freezeElement)
{
}
void crDisplayNodeAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crDisplayNodeAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePathElement = str;
		break;
	case 1:
		m_nameElement = str;
		break;
	case 2:
		m_descriptionElement = str;
		break;
	case 3:
		m_objSelectedLightElement = str;
		break;
	case 4:
		m_avoidLightElement = str;
		break;
	case 5:
		m_needLightElement = str;
		break;
	case 6:
		m_positionTextElement = str;
		break;
	case 7:
		m_posxElement = str;
		break;
	case 8:
		m_posyElement = str;
		break;
	case 9:
		m_poszElement = str;
		break;
	case 10:
		m_staticMeshElement = str;
		break;
	case 11:
		m_freezeElement = str;
		break;
	}
}

void crDisplayNodeAttrMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crImageStage *stage = m_listbox->getParentStage();
		if(!stage) return;
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo)
		{
			crData *infoData = gamebodyInfo->getDataClass();
		    crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			int currentSelect = m_listbox->getSelect();
			if(currentSelect<selectNodeVec.size())
			{
				crNode *currentSelectNode = selectNodeVec[currentSelect].get();
				if(currentSelectNode)
				{
					crGroup *proxyNode = NULL;
					std::string str;
					crEditBoxElement *filePathElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_filePathElement));
					if(filePathElement)
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						currentSelectNode->accept(searchByNameVisitor);
						proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(proxyNode)
						{
							if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
							{
								filePathElement->setStringArrayByString(str);
							}
							else
								filePathElement->clearString();

							crCheckBoxElement *staticMeshElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_staticMeshElement));
							if(staticMeshElement)
							{
								if(proxyNode->getParent(0)->getName().compare("Dynamic") == 0)
								{
									staticMeshElement->setSelect(false);
								}
								else
								{
									staticMeshElement->setSelect(true);
								}
							}
						}
						else
							filePathElement->clearString();
					}

					crCheckBoxElement *freezeElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_freezeElement));
					if(freezeElement && proxyNode)
					{
						void *param;
						infoData->getParam(EDP_FreezeProxySet,param);
						crEditorInfoData::FreezeProxySet* freezeProxySet = (crEditorInfoData::FreezeProxySet*)param;
						if(freezeProxySet->find(proxyNode) != freezeProxySet->end())
						{
							freezeElement->select(true);
						}
						else
						{
							freezeElement->select(false);
						}
					}
					crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
					if(nameElement)
					{
						nameElement->setStringArrayByString(currentSelectNode->getName());
					}
					crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
					if(descriptionElement)
					{
                        descriptionElement->setStringArrayByString(currentSelectNode->getDescriptionsInString());
					}
					///////
					crObject *currentSelectObject = dynamic_cast<crObject *>(currentSelectNode);
					crListBoxElement *objSelectedLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_objSelectedLightElement));
					crListBoxElement *avoidLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_avoidLightElement));
					crListBoxElement *needLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_needLightElement));
					if(currentSelectObject)
					{
						currentSelectObject->lock();
						if(objSelectedLightElement)
						{
							objSelectedLightElement->clearData();
							crObject::SelectedLightList &selectedLightList = currentSelectObject->getSelectedLightList();
							for( crObject::SelectedLightList::iterator itr = selectedLightList.begin();
								 itr != selectedLightList.end();
								 ++itr )
							{
								objSelectedLightElement->addData((*itr)->getName());
							}
							objSelectedLightElement->updateData();
						}
						if(avoidLightElement)
						{
							avoidLightElement->clearData();
							crObject::LightNameSet &lightNameSet = currentSelectObject->getAvoidLightSet();
							for( crObject::LightNameSet::iterator itr = lightNameSet.begin();
								 itr != lightNameSet.end();
								 ++itr )
							{
                                avoidLightElement->addData(*itr);
							}
							avoidLightElement->updateData();
						}
						if(needLightElement)
						{
							needLightElement->clearData();
							crObject::LightNameSet &lightNameSet = currentSelectObject->getNeedLightSet();
							for( crObject::LightNameSet::iterator itr = lightNameSet.begin();
								itr != lightNameSet.end();
								++itr )
							{
								needLightElement->addData(*itr);
							}
							needLightElement->updateData();
						}
						currentSelectObject->unlock();
					}
					else
					{
						if(objSelectedLightElement)
						{
							objSelectedLightElement->clearData();
						    objSelectedLightElement->updateData();
						}
						if(avoidLightElement)
						{
							avoidLightElement->clearData();
							avoidLightElement->updateData();
						}
						if(needLightElement)
						{
							needLightElement->clearData();
							needLightElement->updateData();
						}
					}

					void *_transformNodeMode;
					infoData->getParam(1,_transformNodeMode);
					int transformNodeMode = *((int*)_transformNodeMode);
					bool clearPositionText = true;
					crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(currentSelectNode);
					if(matrixTransform && strcmp(matrixTransform->className(),"crMatrixTransform") == 0)
					{
                        crVector3 vec3;
						if(transformNodeMode == 0||transformNodeMode == 1)
						{
							vec3 = matrixTransform->getTrans();
							clearPositionText = false;
							crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionTextElement));
							if(positionText)
							{
								positionText->setStringArrayByString("Position");
							}
						}
						else if(transformNodeMode == 2)
						{
							crMatrixf rotation = matrixTransform->getMatrix();
							rotation.setTrans(0.0f,0.0f,0.0f);
							vec3 = (-Y_AXIS * rotation).normalize();
							clearPositionText = false;
							crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionTextElement));
							if(positionText)
							{
								positionText->setStringArrayByString("Dir");
							}
						}
						else if(transformNodeMode == 3)
						{
							crMatrix mat = matrixTransform->getMatrix();
							mat.setTrans(0.0f,0.0f,0.0f);
							crVector3 dir = (-Y_AXIS * mat).normalize();
							crMatrix rot_mat = crMatrix::rotate(dir,-Y_AXIS);
							mat.postMult(rot_mat);
							vec3 = mat.getScale();
							clearPositionText = false;
							crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionTextElement));
							if(positionText)
							{
								positionText->setStringArrayByString("Scale");
							}
						}
						if(!clearPositionText)
						{
							crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posxElement));
							if(posxElement)
							{
								posxElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[0]));
							}
							crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posyElement));
							if(posyElement)
							{
								posyElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[1]));
							}
							crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_poszElement));
							if(poszElement)
							{
								poszElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[2]));
							}
						}
					}
					if(clearPositionText)
					{
						crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionTextElement));
						if(positionText)
						{
							positionText->clearString();
						}
						crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posxElement));
						if(posxElement)
						{
							posxElement->clearString();
						}
						crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posyElement));
						if(posyElement)
						{
							posyElement->clearString();
						}
						crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_poszElement));
						if(poszElement)
						{
							poszElement->clearString();
						}
					}
					return;
				}
			}
		}
        //clear data
		crEditBoxElement *filePathElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_filePathElement));
		if(filePathElement)
		{
			filePathElement->clearString();
		}
		crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
		if(nameElement)
		{
			nameElement->clearString();
		}
		crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
		if(descriptionElement)
		{
			descriptionElement->clearString();
		}
		crListBoxElement *objSelectedLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_objSelectedLightElement));
		if(objSelectedLightElement)
		{
			objSelectedLightElement->clearData();
			objSelectedLightElement->updateData();
		}
		crListBoxElement *avoidLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_avoidLightElement));
		if(avoidLightElement)
		{
			avoidLightElement->clearData();
			avoidLightElement->updateData();
		}
		crListBoxElement *needLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_needLightElement));
		if(needLightElement)
		{
			needLightElement->clearData();
			needLightElement->updateData();
		}

		crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionTextElement));
		if(positionText)
		{
			positionText->clearString();
		}
		crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posxElement));
		if(posxElement)
		{
			posxElement->clearString();
		}
		crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posyElement));
		if(posyElement)
		{
			posyElement->clearString();
		}
		crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_poszElement));
		if(poszElement)
		{
			poszElement->clearString();
		}
	}
}
/////////////////////////////////////////
//
//crModifyNodeAttrMethod
//
/////////////////////////////////////////
crModifyNodeAttrMethod::crModifyNodeAttrMethod(){}
crModifyNodeAttrMethod::crModifyNodeAttrMethod(const crModifyNodeAttrMethod& handle):
crMethod(handle),
m_listBoxElement(handle.m_listBoxElement),
m_filePathElement(handle.m_filePathElement),
m_nameElement(handle.m_nameElement),
m_descriptionElement(handle.m_descriptionElement),
m_posxElement(handle.m_posxElement),
m_posyElement(handle.m_posyElement),
m_poszElement(handle.m_poszElement),
m_staticMeshElement(handle.m_staticMeshElement)
{
}
void crModifyNodeAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyNodeAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_listBoxElement = str;
		break;
	case 1:
		m_filePathElement = str;
		break;
	case 2:
		m_nameElement = str;
		break;
	case 3:
		m_descriptionElement = str;
		break;
	case 4:
		m_posxElement = str;
		break;
	case 5:
		m_posyElement = str;
		break;
	case 6:
		m_poszElement = str;
		break;
	case 7:
		m_staticMeshElement = str;
		break;
	}
}

void crModifyNodeAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crListBoxElement *listBoxElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_listBoxElement));
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo && listBoxElement)
		{
			crData *infoData = gamebodyInfo->getDataClass();
			assert(!infoData);

			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			int currentSelect = listBoxElement->getSelect();
			if(currentSelect<selectNodeVec.size())
			{
				crNode *currentSelectNode = selectNodeVec[currentSelect].get();
				if(currentSelectNode)
				{
					gamebodyInfo->unSelectAll();
					crData *infoData = gamebodyInfo->getDataClass();
					assert(infoData);
					listBoxElement->clearData();
					crEditBoxElement *filePathElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_filePathElement));
					std::string matFilePath;
					if(filePathElement)
					{
						std::string fullName = filePathElement->getStringArrayInString();
						matFilePath = crArgumentParser::getFileNameEliminateExt(fullName) + ".mat";
					}
					crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
					if(descriptionElement)
					{
						std::string des = descriptionElement->getStringArrayInString();
						char buf[16];
						std::string cfgFile;
						for(int i = 0; i<10; i++)
						{
							sprintf(buf,"cfgFile%d\0",i);
							if(crArgumentParser::readKeyValue(des,buf,cfgFile))
							{//将该cfgFile拷贝到模型目录下
								if(CRIOManager::getFilePath(cfgFile).compare(matFilePath) != 0)
								{
									std::string newfilename = matFilePath+"/"+CRIOManager::getSimpleFileName(cfgFile);

									std::string cookFile = cfgFile;
									CRIOManager::GetCookedFileName(cookFile);
									std::string cookNewFile = newfilename;
									CRIOManager::GetCookedFileName(cookNewFile);
									CRIOManager::copyFile(cookFile,cookNewFile);
									//CRIOManager::copyFile(cfgFile,newfilename);

									crArgumentParser::updateOrAddKeyValue(des,buf,newfilename);
								}
							}
						}
						//if(crArgumentParser::readKeyValue(des,"EventScript",cfgFile))
						//{//将该cfgFile拷贝到模型目录下
						//	if(CRIOManager::getFilePath(cfgFile).compare(matFilePath) != 0)
						//	{
						//		std::string newfilename = matFilePath+"/"+CRIOManager::getSimpleFileName(cfgFile);
						//		CRIOManager::copyFile(cfgFile,newfilename);
						//		crArgumentParser::updateOrAddKeyValue(des,"EventScript",newfilename);
						//	}
						//}
						currentSelectNode->setDescriptionsInString(des);
					}
					crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
					if(nameElement)
					{
						currentSelectNode->setName(nameElement->getString(0));
						//std::string command = currentSelectNode->getDescriptionsInString();
						//crArgumentParser::updateOrAddKeyValue(command,"[NameID]",currentSelectNode->getName());
						//currentSelectNode->setDescriptionsInString(command);
					}
					///////
					//crListBoxElement *avoidLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_avoidLightElement));
					//crObject *currentSelectObject = dynamic_cast<crObject *>(currentSelectNode);
					//if(currentSelectObject)
					//{
					//	if(avoidLightElement && avoidLightElement->getDataSize()>0)
					//	{
					//		std::string command = currentSelectObject->getDescriptionsInString();
					//		crArgumentParser::updateOrAddKeyValue(command,"HasObjectLight",1);
					//		currentSelectObject->setDescriptionsInString(command);
					//	}
					//}

					void *_transformNodeMode;
					infoData->getParam(1,_transformNodeMode);
					int transformNodeMode = *((int*)_transformNodeMode);
					bool clearPositionText = true;
					if(transformNodeMode == 1)
					{
						crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(currentSelectNode);
						if(matrixTransform && strcmp(matrixTransform->className(),"crMatrixTransform") == 0)
						{
							crVector3 position;
							crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posxElement));
							if(posxElement)
							{
								position[0] = atof(posxElement->getStringArrayInString().c_str());
							}
							crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_posyElement));
							if(posyElement)
							{
								position[1] = atof(posyElement->getStringArrayInString().c_str());
							}
							crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_poszElement));
							if(poszElement)
							{
								position[2] = atof(poszElement->getStringArrayInString().c_str());
							}
							matrixTransform->setPosition(position);
						}
					}

					if(dynamic_cast<crLod *>(currentSelectNode))
					{
						return;
					}
					else if(dynamic_cast<crSwitch *>(currentSelectNode))
					{
						return;
					}
					else if(dynamic_cast<crMultiSwitch *>(currentSelectNode))
					{
						return;
					}
					else if(dynamic_cast<crWidgetNode*>(currentSelectNode))
					{
						return;
					}

					//修改为先removechild,然后adddchild;
					CRUtil::crPrepareVisitor prepareVisitor;
					currentSelectNode->accept(prepareVisitor);
					std::string tmpFile = "tmp.cre";
					crGroup *parent = currentSelectNode->getParent(0);
					CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,currentSelectNode);
					crCheckBoxElement *staticMeshElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_staticMeshElement));
					if(staticMeshElement)
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						currentSelectNode->accept(searchByNameVisitor);
						crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(proxyNode)
						{
							crGroup *root = proxyNode->getParent(0);
							if(staticMeshElement->getSelect())
							{//StaticMesh
								if( root->getName().compare("Dynamic") == 0 )
								{
									CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
									searchByNameVisitor.setNameId("Static");
									searchByNameVisitor.setSearchNodeType(GROUP);
									CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
									crGroup *newparent = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
									if(newparent)
									{
										CRIOManager::crLoadManager::getInstance()->requestAddNode(newparent,proxyNode,false);
										CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
									}
								}
							}
							else if( root->getName().compare("Static") == 0 )
							{//DynamicMesh
								CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
								searchByNameVisitor.setNameId("Dynamic");
								searchByNameVisitor.setSearchNodeType(GROUP);
								CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
								crGroup *newparent = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
								if(newparent)
								{
									CRIOManager::crLoadManager::getInstance()->requestAddNode(newparent,proxyNode,false);
									CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
								}
							}
						}
					}

					if(dynamic_cast<CreBodyNode *>(currentSelectNode))
					{
						CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(currentSelectNode);
						std::string filename;
						if(crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"FileName",filename))
						{
							rcfg::ConfigScript cfg_script;
							if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
							{
								CRCore::notify(CRCore::FATAL)<<"crModifyNodeAttrMethod(): file open error,"<<filename<<std::endl;
								return;
							}
							if(cfg_script.Push("Character"))
							{
								int enabletbn = 0;
								crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"EnableTangentSpace",enabletbn);
								if(!cfg_script.Modify("EnableTangentSpace",enabletbn))
									cfg_script.Add("EnableTangentSpace",enabletbn);
								if (!cfg_script.Pop())
									CRCore::notify(CRCore::FATAL)<<"crModifyNodeAttrMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;

								cfg_script.Write(filename.c_str());
							}
							cfg_script.Close();

							CRIOManager::crWriteCookFile scopedWrite(filename);
						}
						CRIOManager::writeNodeFile(*currentSelectNode,tmpFile);
						CRIOManager::crLoadManager::getInstance()->requestNode(tmpFile,parent,false);
						crFinalCommandParseVisitor finaParse;
						parent->accept(finaParse);
					}
					else if(dynamic_cast<crGroup *>(currentSelectNode))
					{
						crGroup *group = dynamic_cast<crGroup *>(currentSelectNode);
					    NodeArray childArray = group->getChildArray();
						group->removeChild(0,group->getNumChildren());

						CRIOManager::writeNodeFile(*currentSelectNode,tmpFile);
                        crGroup *newGroup = dynamic_cast<crGroup *>(CRIOManager::readNodeFile(tmpFile));
						if(newGroup)
						{
							crGroup *leaves = newGroup;
							while (leaves && leaves->getNumChildren()>0)
							{
								leaves = dynamic_cast<crGroup *>(leaves->getChild(0));
							}
							if(leaves)
							{
								for( NodeArray::iterator itr = childArray.begin();
									itr != childArray.end();
								 ++itr )
								{
									leaves->addChild(itr->get());
								}
							}
							crFinalCommandParseVisitor finaParse;
							newGroup->accept(finaParse);
							//parent->addChild(newGroup);
							//CRIOManager::crLoadManager::getInstance()->requestCompile(newGroup);
							CRIOManager::crLoadManager::getInstance()->requestAddNode(parent,newGroup,true);
						}
					}
					else if(dynamic_cast<crObject *>(currentSelectNode))
					{
                        crObject *object = dynamic_cast<crObject *>(currentSelectNode);
						SubMeshDrawable *subMesh = dynamic_cast<SubMeshDrawable *>(object->getDrawable(0));
						if(subMesh)
						{//需要保存object command
							CreBodyNode *bodyNode = subMesh->getBodyNode();
							assert(bodyNode);
							std::string filename;
							std::string meshName;
                            std::string strbuf;
							if(crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"FileName",filename))
							{
								meshName = subMesh->getName();
								rcfg::ConfigScript cfg_script;
								if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
								{
									CRCore::notify(CRCore::FATAL)<<"crModifyNodeAttrMethod(): file open error,"<<filename<<std::endl;
									return;
								}

								if(cfg_script.Push("Character"))
								{
									int meshMatID = 1;
									bool find = false;
									while (cfg_script.Push("MeshMaterial",meshMatID++))
									{
										cfg_script.Get("MeshName", strbuf);
										if(meshName.compare(strbuf) == 0) 
										{
											find = true;
											break;
										}
										if (!cfg_script.Pop())
											CRCore::notify(CRCore::FATAL)<<"crModifyNodeAttrMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
									}
									meshMatID -= 1;
									if(!find)
									{
										cfg_script.Add("MeshMaterial");
										cfg_script.Push("MeshMaterial",meshMatID);
										cfg_script.Add("MeshName",meshName);
									}
									strbuf = object->getDescriptionsInString();
									for(std::string::iterator itr = strbuf.begin(); itr != strbuf.end(); ++itr)
									{
										if((*itr) == '\n') (*itr) = '@';
									}
									if(!cfg_script.Modify("MeshObjDes",strbuf))
										cfg_script.Add("MeshObjDes",strbuf);

									cfg_script.Pop();

									if (!cfg_script.Pop())
										CRCore::notify(CRCore::FATAL)<<"crModifyNodeAttrMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;

									cfg_script.Write(filename.c_str());
								}
								cfg_script.Close();

								CRIOManager::crWriteCookFile scopedWrite(filename);
							}
						}

						crObject::DrawableList drawableList = object->getDrawableList();
						object->removeDrawable(0,object->getNumDrawables());

						CRIOManager::writeNodeFile(*currentSelectNode,tmpFile);

						crNode *newNode = CRIOManager::readNodeFile(tmpFile);
						crObject *newObj = NULL;
						if(dynamic_cast<crGroup *>(newNode))
						{
							crGroup *g = dynamic_cast<crGroup *>(newNode);
                            while(!newObj)
							{
                                newObj = dynamic_cast<crObject *>(g->getChild(0));
								g =  dynamic_cast<crGroup *>(g->getChild(0));
							}
						}
						else if(dynamic_cast<crObject *>(newNode))
						{
							newObj = dynamic_cast<crObject *>(newNode);
						}
						if(newObj)
						{
							for( crObject::DrawableList::iterator itr = drawableList.begin();
								itr != drawableList.end();
								++itr )
							{
								newObj->addDrawable(itr->get());
							}
							crStateSet *obj_ss = newObj->getStateSet();
							if(obj_ss) obj_ss->dirtyShaderStr();
							//parent->addChild(newNode);
							//CRIOManager::crLoadManager::getInstance()->requestCompile(newObj);
                            CRIOManager::crLoadManager::getInstance()->requestAddNode(parent,newObj,true);
						}
					}

					//CRIOManager::crLoadManager::getInstance()->requestNode(tmpFile,parent,true);

				    listBoxElement->updateData();
				}
			}
		}	
	}
}
/////////////////////////////////////////
//
//crShowScaleInfoMethod
//
/////////////////////////////////////////
crShowScaleInfoMethod::crShowScaleInfoMethod(){}
crShowScaleInfoMethod::crShowScaleInfoMethod(const crShowScaleInfoMethod& handle):
crMethod(handle),
m_listBoxElement(handle.m_listBoxElement),
m_scaleAllElement(handle.m_scaleAllElement),
m_scalexElement(handle.m_scalexElement),
m_scaleyElement(handle.m_scaleyElement),
m_scalezElement(handle.m_scalezElement)
{
}
void crShowScaleInfoMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_this = (CRGUI::crElement*)param;
}

void crShowScaleInfoMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_listBoxElement = str;
		break;
	case 1:
		m_scaleAllElement = str;
		break;
	case 2:
		m_scalexElement = str;
		break;
	case 3:
		m_scaleyElement = str;
		break;
	case 4:
		m_scalezElement = str;
		break;
	}
}

void crShowScaleInfoMethod::operator()(crHandle &handle)
{
	crImageStage *stage = m_this->getParentStage();
	if(!stage) return;
	crListBoxElement *listBoxElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_listBoxElement));
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	if(gamebodyInfo && listBoxElement)
	{
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		int currentSelect = listBoxElement->getSelect();
		if(currentSelect<selectNodeVec.size())
		{
			crMatrixTransform *matrixTransform = dynamic_cast<crMatrixTransform *>(selectNodeVec[currentSelect].get());
			crVector3 vec3(1.0f,1.0f,1.0f);
			crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleAllElement));
			if(positionText)
			{
				positionText->setStringArrayByString("Scale");
			}
			if(matrixTransform && strcmp(matrixTransform->className(),"crMatrixTransform") == 0)
			{
				crMatrix mat = matrixTransform->getMatrix();
				mat.setTrans(0.0f,0.0f,0.0f);
				crVector3 dir = (-Y_AXIS * mat).normalize();
				crMatrix rot_mat = crMatrix::rotate(dir,-Y_AXIS);
				mat.postMult(rot_mat);
				vec3 = mat.getScale();
			}
			crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scalexElement));
			if(posxElement)
			{
				posxElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[0]));
			}
			crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleyElement));
			if(posyElement)
			{
				posyElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[1]));
			}
			crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scalezElement));
			if(poszElement)
			{
				poszElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[2]));
			}
		}
	}
}
/////////////////////////////////////////
//
//crShowBoundLengthMethod
//
/////////////////////////////////////////
crShowBoundLengthMethod::crShowBoundLengthMethod(){}
crShowBoundLengthMethod::crShowBoundLengthMethod(const crShowBoundLengthMethod& handle):
crMethod(handle),
m_listBoxElement(handle.m_listBoxElement),
m_scaleAllElement(handle.m_scaleAllElement),
m_scalexElement(handle.m_scalexElement),
m_scaleyElement(handle.m_scaleyElement),
m_scalezElement(handle.m_scalezElement)
{
}
void crShowBoundLengthMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_this = (CRGUI::crElement*)param;
}

void crShowBoundLengthMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_listBoxElement = str;
		break;
	case 1:
		m_scaleAllElement = str;
		break;
	case 2:
		m_scalexElement = str;
		break;
	case 3:
		m_scaleyElement = str;
		break;
	case 4:
		m_scalezElement = str;
		break;
	}
}

void crShowBoundLengthMethod::operator()(crHandle &handle)
{
	crImageStage *stage = m_this->getParentStage();
	if(!stage) return;
	crListBoxElement *listBoxElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_listBoxElement));
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	if(gamebodyInfo && listBoxElement)
	{
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		int currentSelect = listBoxElement->getSelect();
		if(currentSelect<selectNodeVec.size())
		{
			crNode *selectNode = selectNodeVec[currentSelect].get();
			crVector3 vec3(selectNode->getBoundBox().xLength(),selectNode->getBoundBox().yLength(),selectNode->getBoundBox().zLength());
			crEditBoxElement *positionText = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleAllElement));
			if(positionText)
			{
				positionText->setStringArrayByString("长宽高");
			}
			crEditBoxElement *posxElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scalexElement));
			if(posxElement)
			{
				posxElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[0]));
			}
			crEditBoxElement *posyElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scaleyElement));
			if(posyElement)
			{
				posyElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[1]));
			}
			crEditBoxElement *poszElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_scalezElement));
			if(poszElement)
			{
				poszElement->setStringArrayByString(crArgumentParser::appFtoa(vec3[2]));
			}
		}
	}
}
///////////////////////////////////////////
////
////crSaveNodeAttrMethod
////
///////////////////////////////////////////
//crSaveNodeAttrMethod::crSaveNodeAttrMethod(){}
//crSaveNodeAttrMethod::crSaveNodeAttrMethod(const crSaveNodeAttrMethod& handle):
//crMethod(handle),
//m_listBoxElement(handle.m_listBoxElement),
//m_filePathElement(handle.m_filePathElement),
//m_nameElement(handle.m_nameElement),
//m_descriptionElement(handle.m_descriptionElement),
//m_avoidLightElement(handle.m_avoidLightElement)
//{
//}
//void crSaveNodeAttrMethod::inputParam(int i, void *param)
//{
//	if(i==0)
//		m_element = (CRGUI::crElement*)param;
//}
//
//void crSaveNodeAttrMethod::addParam(int i, const std::string& str)
//{
//	switch(i)
//	{
//	case 0:
//		m_listBoxElement = str;
//		break;
//	case 1:
//		m_filePathElement = str;
//		break;
//	case 2:
//		m_nameElement = str;
//		break;
//	case 3:
//		m_descriptionElement = str;
//		break;
//	case 4:
//		m_avoidLightElement = str;
//		break;
//	}
//}
//
//void crSaveNodeAttrMethod::operator()(crHandle &handle)
//{
//	if(m_element.valid())
//	{
//		crImageStage *stage = m_element->getParentStage();
//		if(!stage) return;
//		crListBoxElement *listBoxElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_listBoxElement));
//		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
//		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
//		if(gamebodyInfo && listBoxElement)
//		{
//			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
//			int currentSelect = listBoxElement->getSelect();
//			if(currentSelect<selectNodeVec.size())
//			{
//				crNode *currentSelectNode = selectNodeVec[currentSelect].get();
//				if(currentSelectNode)
//				{
//					gamebodyInfo->unSelectAll();
//					crData *infoData = gamebodyInfo->getDataClass();
//					assert(infoData);
//					listBoxElement->clearData();
//
//					crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
//					if(nameElement)
//					{
//						currentSelectNode->setName(nameElement->getString(0));
//					}
//					crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
//					if(descriptionElement)
//					{
//						currentSelectNode->setDescriptionsInString(descriptionElement->getStringArrayInString());
//					}
//					///////
//					crListBoxElement *avoidLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_avoidLightElement));
//					crObject *currentSelectObject = dynamic_cast<crObject *>(currentSelectNode);
//					if(currentSelectObject)
//					{
//						if(avoidLightElement && avoidLightElement->getDataSize()>0)
//						{
//							std::string command = currentSelectObject->getDescriptionsInString();
//							crArgumentParser::updateOrAddKeyValue(command,"HasObjectLight",1);
//							currentSelectObject->setDescriptionsInString(command);
//						}
//					}
//
//					std::string filePath;
//					crStaticTextBoxElement *filePathElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_filePathElement));
//					if(filePathElement)
//					{
//						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
//						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
//						searchByNameVisitor.setNameId("ProxyNode");
//						searchByNameVisitor.setSearchNodeType(ALLNODE);
//						currentSelectNode->accept(searchByNameVisitor);
//						crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
//						if(proxyNode)
//						{
//							filePath =  filePathElement->getStringArrayInString();
//							CRUtil::crPrepareVisitor prepareVisitor;
//							proxyNode->accept(prepareVisitor);
//							CRIOManager::writeNodeFile(*(proxyNode->getChild(0)),filePath);
//						}
//					}
//
//					listBoxElement->updateData();
//				}
//			}
//		}	
//	}
//}
/////////////////////////////////////////
//
//crInitSelectTypeMethod
//
/////////////////////////////////////////
crInitSelectTypeMethod::crInitSelectTypeMethod(){}
crInitSelectTypeMethod::crInitSelectTypeMethod(const crInitSelectTypeMethod& handle):
crMethod(handle)
{
}
void crInitSelectTypeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crInitSelectTypeMethod::addParam(int i, const std::string& str)
{
}

void crInitSelectTypeMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(infoData);

		void *_selectType;
		infoData->getParam(0,_selectType);
		int selectType = *((int*)_selectType);
		m_listbox->select(selectType<0?0:selectType);
	}
}
/////////////////////////////////////////
//
//crUpdateSelectTypeMethod
//
/////////////////////////////////////////
crUpdateSelectTypeMethod::crUpdateSelectTypeMethod(){}
crUpdateSelectTypeMethod::crUpdateSelectTypeMethod(const crUpdateSelectTypeMethod& handle):
crMethod(handle)
{
}
void crUpdateSelectTypeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateSelectTypeMethod::addParam(int i, const std::string& str)
{
}

void crUpdateSelectTypeMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(infoData);

		int select = m_listbox->getSelect();
		void *_selectType;
		infoData->getParam(0,_selectType);
		int selectType = *((int*)_selectType);
		if(select != selectType)
		{
            infoData->inputParam(0,&select);
			gamebodyInfo->unSelectAll();
		}
	}
}
/////////////////////////////////////////
//
//crSetSelectTypeMethod
//
/////////////////////////////////////////
crSetSelectTypeMethod::crSetSelectTypeMethod():
m_selectType(0){}
crSetSelectTypeMethod::crSetSelectTypeMethod(const crSetSelectTypeMethod& handle):
crMethod(handle),
m_selectType(handle.m_selectType)
{
}
void crSetSelectTypeMethod::inputParam(int i, void *param)
{
}

void crSetSelectTypeMethod::addParam(int i, const std::string& str)
{
	if(i==0)
		m_selectType = atoi(str.c_str());
}

void crSetSelectTypeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	assert(infoData);

	void *_selectType;
	infoData->getParam(0,_selectType);
	int selectType = *((int*)_selectType);
	if(m_selectType != selectType)
	{
		infoData->inputParam(0,&m_selectType);
		if(m_selectType >= 0)
		    gamebodyInfo->unSelectAll();
	}
}
/////////////////////////////////////////
//
//crLoadDesMethod
//
/////////////////////////////////////////
crLoadDesMethod::crLoadDesMethod():
m_flg(0){}
crLoadDesMethod::crLoadDesMethod(const crLoadDesMethod& handle):
crMethod(handle),
m_desEditElement(handle.m_desEditElement),
m_filePath(handle.m_filePath),
m_flg(handle.m_flg)
{
}
void crLoadDesMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crLoadDesMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_desEditElement = str;
		break;
	case 1:
		m_filePath = str;
		break;
	case 2:
		m_flg = atoi(str.c_str());
		break;
	}
}

void crLoadDesMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crEditBoxElement *desEditElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_desEditElement));
		if(desEditElement)
		{
			char programDir[256];
			GetCurrentDirectory(256,programDir);
			//std::string dir = programDir;
			//dir = dir + "/" + m_filePath;
			//SetCurrentDirectory(dir.c_str());

			OPENFILENAME ofn;       // common dialog box structure
			char szFile[256];       // buffer for file name
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ZeroMemory(szFile, sizeof(szFile));
			//sprintf(szFile,"%s\0",m_filePath.c_str());
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = crStartHandler::getInstance()->getWindowHandle();
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Des(.des)\0*.des\0All(*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = m_filePath.c_str();
			ofn.Flags = OFN_EXPLORER;

			// Display the Open dialog box. 
			if (GetOpenFileName(&ofn)==TRUE) 
			{
				std::ifstream fin(ofn.lpstrFile, std::ios::in);
				fin.seekg(0, std::ios::end); 
				int count  =  fin.tellg();
				fin.seekg(0, std::ios::beg);
				char *readbuf = new char[count+1];//(char *)(malloc(count+1));
				memset(readbuf,0,count+1);
				fin.read(readbuf,count);
				std::string bufstr = readbuf;
				free(readbuf);
				fin.close();

			//FILE *fp = fopen(ofn.lpstrFile,"r");
			//if(fp)
			//{
			//	struct stat sbuf;
			//	__int8 *buf = 0;
			//	std::string bufstr;

			//	if(fstat(fileno(fp),&sbuf)==0)
			//	{
			//		buf = (__int8*)(malloc(sbuf.st_size+1));
			//		ZeroMemory(buf,sbuf.st_size);
			//		fread(buf,1,sbuf.st_size,fp);
			//		buf[sbuf.st_size] = '\0';
			//		bufstr = buf;
			//	}

			//	fseek(fp,0,SEEK_END);
			//	int fsize = ftell(fp);
			//    char *readbuf = (char *)(malloc(fsize+1));
			//	ZeroMemory(readbuf,fsize+1);
			//	fseek(fp, 0, SEEK_SET);
			//	fread(readbuf, fsize, 1, fp);
			//	bufstr = readbuf;
			//	fclose(fp);

				if(m_flg == 1)
				{
                    desEditElement->setStringArrayByString(bufstr);
				}
				else if(m_flg == 2)
				{
					std::string str = desEditElement->getStringArrayInString();
					if(str.empty())
						str = bufstr;
					else
						str += "|"+bufstr;
					desEditElement->setStringArrayByString(str);
				}
				else
				{
					std::string str = desEditElement->getStringArrayInString();
					if(str.empty())
						str = bufstr;
					else
						str += "\n"+bufstr;
					desEditElement->setStringArrayByString(str);
				}
			}
			SetCurrentDirectory(programDir);
		}
	}
}

/////////////////////////////////////////
//
//crInitTransformNodeModeMethod
//
/////////////////////////////////////////
crInitTransformNodeModeMethod::crInitTransformNodeModeMethod(){}
crInitTransformNodeModeMethod::crInitTransformNodeModeMethod(const crInitTransformNodeModeMethod& handle):
crMethod(handle)
{
}
void crInitTransformNodeModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitTransformNodeModeMethod::addParam(int i, const std::string& str)
{
}

void crInitTransformNodeModeMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *_mode;
		infoData->getParam(1,_mode);
		int mode = *((int*)_mode);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crInitTransformNodeModeMethod
//
/////////////////////////////////////////
crInitTransformNodeSpeedMethod::crInitTransformNodeSpeedMethod(){}
crInitTransformNodeSpeedMethod::crInitTransformNodeSpeedMethod(const crInitTransformNodeSpeedMethod& handle):
crMethod(handle)
{
}
void crInitTransformNodeSpeedMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editbox = (CRGUI::crEditBoxElement*)param;
}

void crInitTransformNodeSpeedMethod::addParam(int i, const std::string& str)
{
}

void crInitTransformNodeSpeedMethod::operator()(crHandle &handle)
{
	if(m_editbox.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *_speed;
		infoData->getParam(5,_speed);
		float speed = *((float*)_speed);
		m_editbox->setStringArrayByString(crArgumentParser::appFtoa(speed));
	}
}
/////////////////////////////////////////
//
//crSetTransformNodeModeMethod
//
/////////////////////////////////////////
crSetTransformNodeModeMethod::crSetTransformNodeModeMethod():
m_transformNodeMode(0){}
crSetTransformNodeModeMethod::crSetTransformNodeModeMethod(const crSetTransformNodeModeMethod& handle):
crMethod(handle),
m_transformNodeMode(handle.m_transformNodeMode)
{
}
void crSetTransformNodeModeMethod::inputParam(int i, void *param)
{
}

void crSetTransformNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_transformNodeMode = atoi(str.c_str());
}

void crSetTransformNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(1,&m_transformNodeMode);
	}
}
/////////////////////////////////////////
//
//crInitPanNodeModeMethod
//
/////////////////////////////////////////
crInitPanNodeModeMethod::crInitPanNodeModeMethod(){}
crInitPanNodeModeMethod::crInitPanNodeModeMethod(const crInitPanNodeModeMethod& handle):
	crMethod(handle)
{
}
void crInitPanNodeModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitPanNodeModeMethod::addParam(int i, const std::string& str)
{
}

void crInitPanNodeModeMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *_mode;
		infoData->getParam(2,_mode);
		int mode = *((int*)_mode);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crSetPanNodeModeMethod
//
/////////////////////////////////////////
crSetPanNodeModeMethod::crSetPanNodeModeMethod():
m_panNodeMode(0){}
crSetPanNodeModeMethod::crSetPanNodeModeMethod(const crSetPanNodeModeMethod& handle):
crMethod(handle),
m_panNodeMode(handle.m_panNodeMode)
{
}
void crSetPanNodeModeMethod::inputParam(int i, void *param)
{
}

void crSetPanNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_panNodeMode = atoi(str.c_str());
}

void crSetPanNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(2,&m_panNodeMode);
	}
}
/////////////////////////////////////////
//
//crWalkPanNodeModeMethod
//
/////////////////////////////////////////
crWalkPanNodeModeMethod::crWalkPanNodeModeMethod():
	m_panNodeModeCount(4){}
crWalkPanNodeModeMethod::crWalkPanNodeModeMethod(const crWalkPanNodeModeMethod& handle):
	crMethod(handle),
	m_panNodeModeCount(handle.m_panNodeModeCount)
{
}
void crWalkPanNodeModeMethod::inputParam(int i, void *param)
{
}

void crWalkPanNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_panNodeModeCount = atoi(str.c_str());
}

void crWalkPanNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		void *param;
		infoData->getParam(2,param);
		int *panNodeMode = (int *)param;
		(*panNodeMode)++;
		if(*panNodeMode>=m_panNodeModeCount)
			*panNodeMode = 0;
	}
}
/////////////////////////////////////////
//
//crInitRotNodeModeMethod
//
/////////////////////////////////////////
crInitRotNodeModeMethod::crInitRotNodeModeMethod(){}
crInitRotNodeModeMethod::crInitRotNodeModeMethod(const crInitRotNodeModeMethod& handle):
	crMethod(handle)
{
}
void crInitRotNodeModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitRotNodeModeMethod::addParam(int i, const std::string& str)
{
}

void crInitRotNodeModeMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *_mode;
		infoData->getParam(3,_mode);
		int mode = *((int*)_mode);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crSetRotNodeModeMethod
//
/////////////////////////////////////////
crSetRotNodeModeMethod::crSetRotNodeModeMethod():
m_rotNodeMode(0){}
crSetRotNodeModeMethod::crSetRotNodeModeMethod(const crSetRotNodeModeMethod& handle):
crMethod(handle),
m_rotNodeMode(handle.m_rotNodeMode)
{
}
void crSetRotNodeModeMethod::inputParam(int i, void *param)
{
}

void crSetRotNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_rotNodeMode = atoi(str.c_str());
}

void crSetRotNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(3,&m_rotNodeMode);
	}
}
/////////////////////////////////////////
//
//crWalkRotNodeModeMethod
//
/////////////////////////////////////////
crWalkRotNodeModeMethod::crWalkRotNodeModeMethod():
	m_rotNodeModeCount(3){}
crWalkRotNodeModeMethod::crWalkRotNodeModeMethod(const crWalkRotNodeModeMethod& handle):
	crMethod(handle),
	m_rotNodeModeCount(handle.m_rotNodeModeCount)
{
}
void crWalkRotNodeModeMethod::inputParam(int i, void *param)
{
}

void crWalkRotNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_rotNodeModeCount = atoi(str.c_str());
}

void crWalkRotNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		void *param;
		infoData->getParam(3,param);
		int *rotNodeMode = (int *)param;
		(*rotNodeMode)++;
		if(*rotNodeMode>=m_rotNodeModeCount)
			*rotNodeMode = 0;
	}
}
/////////////////////////////////////////
//
//crInitScaleNodeModeMethod
//
/////////////////////////////////////////
crInitScaleNodeModeMethod::crInitScaleNodeModeMethod(){}
crInitScaleNodeModeMethod::crInitScaleNodeModeMethod(const crInitScaleNodeModeMethod& handle):
	crMethod(handle)
{
}
void crInitScaleNodeModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitScaleNodeModeMethod::addParam(int i, const std::string& str)
{
}

void crInitScaleNodeModeMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *_mode;
		infoData->getParam(4,_mode);
		int mode = *((int*)_mode);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crSetScaleNodeModeMethod
//
/////////////////////////////////////////
crSetScaleNodeModeMethod::crSetScaleNodeModeMethod():
m_scaleNodeMode(0){}
crSetScaleNodeModeMethod::crSetScaleNodeModeMethod(const crSetScaleNodeModeMethod& handle):
crMethod(handle),
m_scaleNodeMode(handle.m_scaleNodeMode)
{
}
void crSetScaleNodeModeMethod::inputParam(int i, void *param)
{
}

void crSetScaleNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_scaleNodeMode = atoi(str.c_str());
}

void crSetScaleNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(4,&m_scaleNodeMode);
	}
}
/////////////////////////////////////////
//
//crWalkScaleNodeModeMethod
//
/////////////////////////////////////////
crWalkScaleNodeModeMethod::crWalkScaleNodeModeMethod():
	m_scaleNodeModeCount(3){}
crWalkScaleNodeModeMethod::crWalkScaleNodeModeMethod(const crWalkScaleNodeModeMethod& handle):
	crMethod(handle),
	m_scaleNodeModeCount(handle.m_scaleNodeModeCount)
{
}
void crWalkScaleNodeModeMethod::inputParam(int i, void *param)
{
}

void crWalkScaleNodeModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_scaleNodeModeCount = atoi(str.c_str());
}

void crWalkScaleNodeModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		void *param;
		infoData->getParam(4,param);
		int *scaleNodeMode = (int *)param;
		(*scaleNodeMode)++;
		if(*scaleNodeMode>=m_scaleNodeModeCount)
			*scaleNodeMode = 0;
	}
}
/////////////////////////////////////////
//
//crSetTransformNodeSpeedMethod
//
/////////////////////////////////////////
crSetTransformNodeSpeedMethod::crSetTransformNodeSpeedMethod(){}
crSetTransformNodeSpeedMethod::crSetTransformNodeSpeedMethod(const crSetTransformNodeSpeedMethod& handle):
crMethod(handle)
{
}
void crSetTransformNodeSpeedMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editbox = (CRGUI::crEditBoxElement*)param;
}

void crSetTransformNodeSpeedMethod::addParam(int i, const std::string& str)
{
}

void crSetTransformNodeSpeedMethod::operator()(crHandle &handle)
{
	if(m_editbox.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		if(infoData)
		{
			float speed = atof(m_editbox->getStringArrayInString().c_str());
			infoData->inputParam(5,&speed);
		}
	}
}
/////////////////////////////////////////
//
//crResetMatrixLogic
//
/////////////////////////////////////////
crResetMatrixLogic::crResetMatrixLogic(){}
crResetMatrixLogic::crResetMatrixLogic(const crResetMatrixLogic& handle):
	crLogic(handle)
{
	if(handle.m_adjustEditorNodeMathod.valid())
		m_adjustEditorNodeMathod = handle.m_adjustEditorNodeMathod->clone();
}

void crResetMatrixLogic::inputParam(int i, void *param)
{
}

void crResetMatrixLogic::addParam(int i, const std::string& str)
{
}

void crResetMatrixLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_adjustEditorNodeMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crResetMatrixLogic::outputParam(int i, void *param)
{
}

void crResetMatrixLogic::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		void *_canmove;
		infoData->getParam(8,_canmove);
		int canmove = *((int*)_canmove);
		if(!canmove) return;

		void *_transformNodeMode;
		infoData->getParam(1,_transformNodeMode);
		int transformNodeMode = *((int*)_transformNodeMode);
		if(transformNodeMode!=2 && transformNodeMode!=3) return;

		crGameBodyInfo::SelectNodeVec& selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *matrixTransform;
			crMatterObject *matterObject;
			for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
				itr != selectNodeVec.end();
				++itr )
			{
				matrixTransform = dynamic_cast<crMatrixTransform *>(itr->get());
				if(matrixTransform)
				{
					crMatrix mat = matrixTransform->getMatrix();
					crMatrix resetMat = mat;
					resetMat.setTrans(crVector3(0.0f,0.0f,0.0f));
                    resetMat = crMatrix::inverse(resetMat);
					crVector3 trans = matrixTransform->getTrans();//matrixTransform->getBound().center();
					crMatrix put_mat = crMatrix::translate(trans);
					crMatrix inv_put = crMatrix::inverse(put_mat);
					mat.postMult(inv_put);
					mat.postMult(resetMat);
					mat.postMult(put_mat);

					matterObject = dynamic_cast<crMatterObject *>(itr->get());
					if(matterObject)
					{
						matterObject->setMatterObjectMatrix(mat,CRPhysics::crMatterObject::MD_FullMatrix);

						CRCore::crVector3 pos = mat.getTrans();
						std::string command = matterObject->getDescriptionsInString();
						CRCore::crArgumentParser::updateOrAddKeyValue(command,"GeomPosition",pos);
						CRCore::crQuat q;
						mat.get(q);
						CRCore::crArgumentParser::updateOrAddKeyValue(command,"GeomRotation",q.asVec4());
						matterObject->setDescriptionsInString(command);
					}
					else
					{
						matrixTransform->setMatrix(mat);
					}

					if(m_adjustEditorNodeMathod.valid())
					{
						m_adjustEditorNodeMathod->inputParam(1,itr->get());
						(*m_adjustEditorNodeMathod)(*this);
						m_adjustEditorNodeMathod->inputParam(1,NULL);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crNewLightMethod
//
/////////////////////////////////////////
crNewLightMethod::crNewLightMethod():
m_lsRoot(NULL){}
crNewLightMethod::crNewLightMethod(const crNewLightMethod& handle):
crMethod(handle),
m_lsRoot(NULL)
{
}
void crNewLightMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_copyData = (crLightSource *)param;
		break;
	}
}

void crNewLightMethod::addParam(int i, const std::string& str)
{
}

void crNewLightMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId("LSGroup");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_lsRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_lsRoot)
	{
		ref_ptr<crLightSource> ls;
		if(m_copyData.valid())
		{
            ls = dynamic_cast<crLightSource *>(m_copyData->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_STATEATTRIBUTES));
		}
		else
		{
			ref_ptr<crLight> light = new crLight;
			ls = new crLightSource;
			ls->setLight(light.get());
		}
		crLightSourceManager::getInstance()->lock();
		char buf[16];
		std::string lightName;
		int safeid = 0;
		int id;
		crLightSourceManager::LSVec &lsVec = crLightSourceManager::getInstance()->getLSVec();
		for( crLightSourceManager::LSVec::iterator itr = lsVec.begin();
			itr != lsVec.end();
			++itr )
		{
			lightName = (*itr)->getName();
			if(lightName.find("NewLight")==0)
			{
				lightName.erase(0,8);
                id = atoi(lightName.c_str());
				if(safeid<=id) safeid = id+1;
			}
		}
		crLightSourceManager::getInstance()->unlock();
		sprintf(buf,"NewLight%d\0",safeid);
		ls->setName(buf);
		crLightSourceManager::getInstance()->insertLightSource(ls.get());
		ls->initLightParamStruct();
		CRIOManager::crLoadManager::getInstance()->requestAddNode(m_lsRoot,ls.get(),false);
		std::string des = "EditorNodeType:LightModel\nEditorNodeName:"+ls->getName();
		handle.outputParam(0,&des);
	}
}
/////////////////////////////////////////
//
//crDeleteLightMethod
//
/////////////////////////////////////////
crDeleteLightMethod::crDeleteLightMethod():
m_lsRoot(NULL){}
crDeleteLightMethod::crDeleteLightMethod(const crDeleteLightMethod& handle):
crMethod(handle),
m_lsRoot(NULL)
{
}
void crDeleteLightMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_editorNodeName = (std::string *)param;
		break;
	}
}

void crDeleteLightMethod::addParam(int i, const std::string& str)
{
}

void crDeleteLightMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId("LSGroup");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_lsRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_lsRoot)
	{
		if(m_editorNodeName && !m_editorNodeName->empty())
		{
			crLightSource *ls = crLightSourceManager::getInstance()->findLightSource(*m_editorNodeName);
			if(ls)
			{
				CRCore::crLightSourceManager::getInstance()->removeLightSource(ls);
				crGroup *parent = ls->getParent(0);
				crGroup *child = ls;
				while(parent && parent != m_lsRoot)
				{
					child = parent;
					parent = parent->getParent(0);
				}
				if(parent && child)
					CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,child);
			}
		}
	}
}
/////////////////////////////////////////
//
//crDeleteParticleMethod
//
/////////////////////////////////////////
crDeleteParticleMethod::crDeleteParticleMethod(){}
crDeleteParticleMethod::crDeleteParticleMethod(const crDeleteParticleMethod& handle):
crMethod(handle)
{
}
void crDeleteParticleMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_editorNodeName = (std::string *)param;
		break;
	}
}

void crDeleteParticleMethod::addParam(int i, const std::string& str)
{
}

void crDeleteParticleMethod::operator()(crHandle &handle)
{
	if(m_editorNodeName && !m_editorNodeName->empty())
	{
		crGroup *effectGroup = crParticleSystemUpdater::getInstance()->findEffectGroup(*m_editorNodeName);
		if(effectGroup)
		{
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setNameId("ProxyNode");
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			effectGroup->accept(searchByNameVisitor);
			crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(proxyNode)
			{
				crGroup *root = proxyNode->getParent(0);
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
			}
		}
	}
}
/////////////////////////////////////////
//
//crUpdateLightListMethod
//
/////////////////////////////////////////
crUpdateLightListMethod::crUpdateLightListMethod(){}
crUpdateLightListMethod::crUpdateLightListMethod(const crUpdateLightListMethod& handle):
crMethod(handle)
{
}
void crUpdateLightListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateLightListMethod::addParam(int i, const std::string& str)
{
}

void crUpdateLightListMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		m_listbox->clearData();
		crLightSourceManager::getInstance()->lock();
		crLightSourceManager::LSVec &lsVec = crLightSourceManager::getInstance()->getLSVec();
		for( crLightSourceManager::LSVec::iterator itr = lsVec.begin();
			itr != lsVec.end();
			++itr )
		{
			m_listbox->addData((*itr)->getName());
		}
		crLightSourceManager::getInstance()->unlock();
		m_listbox->checkSelect();
	}
}
/////////////////////////////////////////
//
//crUpdateParticleListMethod
//
/////////////////////////////////////////
crUpdateParticleListMethod::crUpdateParticleListMethod(){}
crUpdateParticleListMethod::crUpdateParticleListMethod(const crUpdateParticleListMethod& handle):
crMethod(handle)
{
}
void crUpdateParticleListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateParticleListMethod::addParam(int i, const std::string& str)
{
}

void crUpdateParticleListMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		m_listbox->clearData();
		crParticleSystemUpdater::getInstance()->lock();
		std::set<crGroup *> effectGroupSet;
		CRCore::ref_ptr<crParticleEffect> setItr;
		crParticleSystemUpdater::ParticleEffectSet &effectSet = crParticleSystemUpdater::getInstance()->getParticleEffectSet();
		for( crParticleSystemUpdater::ParticleEffectSet::iterator itr = effectSet.begin();
			itr != effectSet.end();
			++itr )
		{
			setItr = *itr;
			effectGroupSet.insert(setItr->getParent(0));
		}
        
		for( std::set<crGroup *>::iterator itr = effectGroupSet.begin();
			itr != effectGroupSet.end();
			++itr )
		{
			m_listbox->addData((*itr)->getName());
		}

		crParticleSystemUpdater::getInstance()->unlock();
		m_listbox->checkSelect();
	}
}
/////////////////////////////////////////
//
//crSelectEditorNodeMethod
//
/////////////////////////////////////////
crSelectEditorNodeMethod::crSelectEditorNodeMethod(){}
crSelectEditorNodeMethod::crSelectEditorNodeMethod(const crSelectEditorNodeMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName)
{
}
void crSelectEditorNodeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crSelectEditorNodeMethod::addParam(int i, const std::string& str)
{
	if(i==0)
		m_rootName = str;
}

void crSelectEditorNodeMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if( m_root && m_listbox.valid() && m_listbox->getDataSize()>0 )
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
        assert(gamebodyInfo);
		std::string selectLightName;
		m_listbox->getData(m_listbox->getSelect(),selectLightName);
		//if(!gamebodyInfo->isNodeSelected(selectLightName))
		{
			gamebodyInfo->unSelectAll();
			crData *infoData = gamebodyInfo->getDataClass();
			assert(infoData);

			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId(selectLightName);
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			m_root->accept(searchByNameVisitor);
			crMatrixTransform* matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
			if(matrixTransform)
			{
				ref_ptr<crData> data = matrixTransform->getDataClass();
				if(!data.valid())
				{
					data = crDataManager::getInstance()->getData("Event");
					matrixTransform->setDataClass(data.get());
				}
				if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
				{
					crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
					driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
					data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
					data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
				}
				gamebodyInfo->selectNode(matrixTransform);
			}
		}
	}
}
/////////////////////////////////////////
//
//crSelectEditorNodeExMethod
//
/////////////////////////////////////////
crSelectEditorNodeExMethod::crSelectEditorNodeExMethod(){}
crSelectEditorNodeExMethod::crSelectEditorNodeExMethod(const crSelectEditorNodeExMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName),
m_lightListElement(handle.m_lightListElement)
{
}
void crSelectEditorNodeExMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSelectEditorNodeExMethod::addParam(int i, const std::string& str)
{
	if(i==0)
		m_rootName = str;
	else if(i == 1)
		m_lightListElement = str;
}

void crSelectEditorNodeExMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(m_root && m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_lightListElement));
		if(listbox && listbox->getDataSize()>0)
		{
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			assert(cameraBot);
			crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
			assert(gamebodyInfo);
			std::string selectLightName;
			listbox->getData(listbox->getSelect(),selectLightName);
			//if(!gamebodyInfo->isNodeSelected(selectLightName))
			{
				gamebodyInfo->unSelectAll();
				crData *infoData = gamebodyInfo->getDataClass();
				assert(infoData);

				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setNameId(selectLightName);
				searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
				m_root->accept(searchByNameVisitor);
				crMatrixTransform* matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
				if(matrixTransform)
				{
					ref_ptr<crData> data = matrixTransform->getDataClass();
					if(!data.valid())
					{
						data = crDataManager::getInstance()->getData("Event");
						matrixTransform->setDataClass(data.get());
					}
					if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
					{
						crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
						driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
						data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
						data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
					}
					gamebodyInfo->selectNode(matrixTransform);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisplayLightAttrMethod
//
/////////////////////////////////////////
crDisplayLightAttrMethod::crDisplayLightAttrMethod(){}
crDisplayLightAttrMethod::crDisplayLightAttrMethod(const crDisplayLightAttrMethod& handle):
crMethod(handle),
m_currentSelectLightElement(handle.m_currentSelectLightElement),
m_nameElement(handle.m_nameElement),
m_descriptionElement(handle.m_descriptionElement),
m_positionXElement(handle.m_positionXElement),
m_positionYElement(handle.m_positionYElement),
m_positionZElement(handle.m_positionZElement),
//m_azimuthElement(handle.m_azimuthElement),
//m_elevationElement(handle.m_elevationElement),
m_lightTypeGroupElement(handle.m_lightTypeGroupElement),
m_ambientColorElement(handle.m_ambientColorElement),
m_ambientColorRElement(handle.m_ambientColorRElement),
m_ambientColorGElement(handle.m_ambientColorGElement),
m_ambientColorBElement(handle.m_ambientColorBElement),
m_diffuseColorElement(handle.m_diffuseColorElement),
m_diffuseColorRElement(handle.m_diffuseColorRElement),
m_diffuseColorGElement(handle.m_diffuseColorGElement),
m_diffuseColorBElement(handle.m_diffuseColorBElement),
m_specularColorElement(handle.m_specularColorElement),
m_specularColorRElement(handle.m_specularColorRElement),
m_specularColorGElement(handle.m_specularColorGElement),
m_specularColorBElement(handle.m_specularColorBElement),
m_constantElement(handle.m_constantElement),
m_linearElement(handle.m_linearElement),
m_quadraticElement(handle.m_quadraticElement),
m_loweastLumElement(handle.m_loweastLumElement),
m_distanceElement(handle.m_distanceElement),
m_cutoffElement(handle.m_cutoffElement),
m_dropoffElement(handle.m_dropoffElement),
m_enableElement(handle.m_enableElement),
m_calcShadowElement(handle.m_calcShadowElement),
m_calcShadow2Element(handle.m_calcShadow2Element)
{
}
void crDisplayLightAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crDisplayLightAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_currentSelectLightElement = str;
		break;
	case 1:
		m_nameElement = str;
		break;
	case 2:
		m_descriptionElement = str;
		break;
	case 3:
		m_positionXElement = str;
		break;
	case 4:
		m_positionYElement = str;
		break;
	case 5:
		m_positionZElement = str;
		break;
	//case 6:
	//	m_azimuthElement = str;
	//	break;
	//case 7:
	//	m_elevationElement = str;
	//	break;
	case 6:
		m_lightTypeGroupElement = str;
		break;
	case 7:
		m_ambientColorElement = str;
		break;
	case 8:
		m_ambientColorRElement = str;
		break;
	case 9:
		m_ambientColorGElement = str;
		break;
	case 10:
		m_ambientColorBElement = str;
		break;
	case 11:
		m_diffuseColorElement = str;
		break;
	case 12:
		m_diffuseColorRElement = str;
		break;
	case 13:
		m_diffuseColorGElement = str;
		break;
	case 14:
		m_diffuseColorBElement = str;
		break;
	case 15:
		m_specularColorElement = str;
		break;
	case 16:
		m_specularColorRElement = str;
		break;
	case 17:
		m_specularColorGElement = str;
		break;
	case 18:
		m_specularColorBElement = str;
		break;
	case 19:
		m_constantElement = str;
		break;
	case 20:
		m_linearElement = str;
		break;
	case 21:
		m_quadraticElement = str;
		break;
	case 22:
		m_loweastLumElement = str;
		break;
	case 23:
		m_distanceElement = str;
		break;
	case 24:
		m_cutoffElement = str;
		break;
	case 25:
		m_dropoffElement = str;
		break;
	case 26:
		m_enableElement = str;
		break;
	case 27:
		m_calcShadowElement = str;
		break;
	case 28:
		m_calcShadow2Element = str;
		break;
	}
}

void crDisplayLightAttrMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crImageStage *stage = m_listbox->getParentStage();
		std::string selectLightName;
		m_listbox->getData(m_listbox->getSelect(),selectLightName);
		ref_ptr<crLightSource>selectLightSource = crLightSourceManager::getInstance()->findLightSource(selectLightName);
		assert(selectLightSource.valid());
		crLight *light = dynamic_cast<crLight *>(selectLightSource->getLight());
		assert(light);
		std::string str;
		crStaticTextBoxElement *currentSelectLightElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_currentSelectLightElement));
		if(currentSelectLightElement)
		{
			currentSelectLightElement->setStringArrayByString(selectLightName);
		}
		crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
		if(nameElement)
		{
			nameElement->setStringArrayByString(selectLightName);
		}
		crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
		if(descriptionElement)
		{
			descriptionElement->setStringArrayByString(selectLightSource->getDescriptionsInString());
		}
		crCheckBoxElement *enableElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableElement));
		if(enableElement)
		{
			enableElement->setSelect(!selectLightSource->getEnable());
		}
		crCheckBoxElement *calcShadowElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_calcShadowElement));
		if(calcShadowElement)
		{
			calcShadowElement->setSelect(!selectLightSource->getCalcShadow());
		}
		crCheckBoxElement *calcShadow2Element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_calcShadow2Element));
		if(calcShadow2Element)
		{
			calcShadow2Element->setSelect(!selectLightSource->getCalcShadow2());
		}
		const crVector4& position = light->getPosition();
		crEditBoxElement *positionXElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionXElement));
		if(positionXElement)
		{
			positionXElement->setStringArrayByString(crArgumentParser::appFtoa(position[0]));
		}
		crEditBoxElement *positionYElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionYElement));
		if(positionYElement)
		{
			positionYElement->setStringArrayByString(crArgumentParser::appFtoa(position[1]));
		}
		crEditBoxElement *positionZElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionZElement));
		if(positionZElement)
		{
			positionZElement->setStringArrayByString(crArgumentParser::appFtoa(position[2]));
		}
		//const crVector3 &dir = light->getDirection();
		//float azimuth;
		//float elevation = 0.0f;
		//crEditBoxElement *azimuthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_azimuthElement));
		//if(azimuthElement)
		//{
		//	azimuth = RadiansToDegrees(asin(dir[2]));
		//	azimuthElement->setStringArrayByString(crArgumentParser::appFtoa(azimuth));
		//}
		//crEditBoxElement *elevationElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_elevationElement));
		//if(elevationElement)
		//{
		//	if(dir[1] > 0.0f)
		//	    elevation = RadiansToDegrees(-atan(dir[0]/dir[1]));
		//	else if(dir[1] < 0.0f)
		//		elevation = RadiansToDegrees(atan(dir[0]/dir[1]));
		//	elevationElement->setStringArrayByString(crArgumentParser::appFtoa(elevation));
		//}
		crRadioGroupElement *lightTypeGroupElement = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_lightTypeGroupElement));
		if(lightTypeGroupElement)
		{
			if(selectLightSource->getLightIsSunLight())
			{
				lightTypeGroupElement->select(2);
			}
			else if(light->getLightType() == crLight::LIGHT_LOCAL)
			{
				lightTypeGroupElement->select(0);
			}
			else if(light->getLightType() == crLight::LIGHT_SPOT)
			{
				lightTypeGroupElement->select(1);
			}
		}
		const crVector4& ambientColor = light->getAmbient();
		//m_ambientColorElement
        crEditBoxElement *ambientColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorRElement));
        if(ambientColorRElement)
		{
			ambientColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[0] * 255.0f)));
		}
		crEditBoxElement *ambientColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorGElement));
		if(ambientColorGElement)
		{
			ambientColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[1] * 255.0f)));
		}
		crEditBoxElement *ambientColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorBElement));
		if(ambientColorBElement)
		{
			ambientColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[2] * 255.0f)));
		}
		const crVector4& diffuseColor = light->getDiffuse();
		//diffuseColorElement
		crEditBoxElement *diffuseColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorRElement));
		if(diffuseColorRElement)
		{
			diffuseColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[0] * 255.0f)));
		}
		crEditBoxElement *diffuseColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorGElement));
		if(diffuseColorGElement)
		{
			diffuseColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[1] * 255.0f)));
		}
		crEditBoxElement *diffuseColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorBElement));
		if(diffuseColorBElement)
		{
			diffuseColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[2] * 255.0f)));
		}
		const crVector4& specularColor = light->getSpecular();
		//specularColorElement
		crEditBoxElement *specularColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorRElement));
		if(specularColorRElement)
		{
			specularColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[0] * 255.0f)));
		}
		crEditBoxElement *specularColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorGElement));
		if(specularColorGElement)
		{
			specularColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[1] * 255.0f)));
		}
		crEditBoxElement *specularColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorBElement));
		if(specularColorBElement)
		{
			specularColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[2] * 255.0f)));
		}
		////////attenuation
		float constant = light->getConstantAttenuation();
		crEditBoxElement *constantElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_constantElement));
		if(constantElement)
		{
			constantElement->setStringArrayByString(crArgumentParser::appFtoa(constant));
		}
		float linear = light->getLinearAttenuation();
		crEditBoxElement *linearElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_linearElement));
		if(linearElement)
		{
			linearElement->setStringArrayByString(crArgumentParser::appFtoa(linear));
		}
		float quadratic = light->getQuadraticAttenuation();
		crEditBoxElement *quadraticElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_quadraticElement));
		if(quadraticElement)
		{
			quadraticElement->setStringArrayByString(crArgumentParser::appFtoa(quadratic));
		}
		float loweastLum = light->getLoweastLum();
		crEditBoxElement *loweastLumElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_loweastLumElement));
		if(loweastLumElement)
		{
			loweastLumElement->setStringArrayByString(crArgumentParser::appFtoa(loweastLum));
		}
		//float distance = 0.0f; 
		//crEditBoxElement *distanceElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_distanceElement));
		//if(distanceElement)
		//{
		//	if(loweastLum>0.0f)
		//	{
		//		if(quadratic==0.0f)
		//		{
		//			if(linear!=0.0f)
		//			{
		//				distance = (1.0f/loweastLum-constant)/linear;
		//			}
		//		}
		//		else
		//		{
		//			float a = linear*linear - 4.0f*quadratic*(constant-1.0f/loweastLum);
		//			if(a>0)
		//			{
		//				float x1 = (-linear+sqrt(a))/(2.0f*quadratic);
		//				distance = CRCore::maximum(0.0f,x1);
		//			}
		//		}
		//	}
		//	distanceElement->setStringArrayByString(crArgumentParser::appFtoa(distance));
		//}
		crEditBoxElement *distanceElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_distanceElement));
		if(distanceElement)
		{
			distanceElement->setStringArrayByString(crArgumentParser::appFtoa(light->getMaxDistance()));
		}
		float cutoff = light->getSpotCutoff();
		crEditBoxElement *cutoffElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_cutoffElement));
		if(cutoffElement)
		{
			cutoffElement->setStringArrayByString(crArgumentParser::appFtoa(cutoff));
		}
		float dropoff = light->getSpotExponent();
		crEditBoxElement *dropoffElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_dropoffElement));
		if(dropoffElement)
		{
			dropoffElement->setStringArrayByString(crArgumentParser::appFtoa(dropoff));
		}
	}
}
/////////////////////////////////////////
//
//crModifyLightAttrMethod
//
/////////////////////////////////////////
crModifyLightAttrMethod::crModifyLightAttrMethod(){}
crModifyLightAttrMethod::crModifyLightAttrMethod(const crModifyLightAttrMethod& handle):
crMethod(handle),
m_editorRootName(handle.m_editorRootName),
m_lightListElement(handle.m_lightListElement),
m_currentSelectLightElement(handle.m_currentSelectLightElement),
m_nameElement(handle.m_nameElement),
m_descriptionElement(handle.m_descriptionElement),
m_positionXElement(handle.m_positionXElement),
m_positionYElement(handle.m_positionYElement),
m_positionZElement(handle.m_positionZElement),
//m_azimuthElement(handle.m_azimuthElement),
//m_elevationElement(handle.m_elevationElement),
m_lightTypeGroupElement(handle.m_lightTypeGroupElement),
m_ambientColorElement(handle.m_ambientColorElement),
m_ambientColorRElement(handle.m_ambientColorRElement),
m_ambientColorGElement(handle.m_ambientColorGElement),
m_ambientColorBElement(handle.m_ambientColorBElement),
m_diffuseColorElement(handle.m_diffuseColorElement),
m_diffuseColorRElement(handle.m_diffuseColorRElement),
m_diffuseColorGElement(handle.m_diffuseColorGElement),
m_diffuseColorBElement(handle.m_diffuseColorBElement),
m_specularColorElement(handle.m_specularColorElement),
m_specularColorRElement(handle.m_specularColorRElement),
m_specularColorGElement(handle.m_specularColorGElement),
m_specularColorBElement(handle.m_specularColorBElement),
m_constantElement(handle.m_constantElement),
m_linearElement(handle.m_linearElement),
m_quadraticElement(handle.m_quadraticElement),
m_loweastLumElement(handle.m_loweastLumElement),
m_distanceElement(handle.m_distanceElement),
m_cutoffElement(handle.m_cutoffElement),
m_dropoffElement(handle.m_dropoffElement),
m_enableElement(handle.m_enableElement),
m_calcShadowElement(handle.m_calcShadowElement),
m_calcShadow2Element(handle.m_calcShadow2Element)
{
}
void crModifyLightAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crListBoxElement*)param;
}

void crModifyLightAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_editorRootName = str;
		break;
	case 1:
		m_lightListElement = str;
		break;
	case 2:
		m_currentSelectLightElement = str;
		break;
	case 3:
		m_nameElement = str;
		break;
	case 4:
		m_descriptionElement = str;
		break;
	case 5:
		m_positionXElement = str;
		break;
	case 6:
		m_positionYElement = str;
		break;
	case 7:
		m_positionZElement = str;
		break;
	//case 7:
	//	m_azimuthElement = str;
	//	break;
	//case 8:
	//	m_elevationElement = str;
	//	break;
	case 8:
		m_lightTypeGroupElement = str;
		break;
	case 9:
		m_ambientColorElement = str;
		break;
	case 10:
		m_ambientColorRElement = str;
		break;
	case 11:
		m_ambientColorGElement = str;
		break;
	case 12:
		m_ambientColorBElement = str;
		break;
	case 13:
		m_diffuseColorElement = str;
		break;
	case 14:
		m_diffuseColorRElement = str;
		break;
	case 15:
		m_diffuseColorGElement = str;
		break;
	case 16:
		m_diffuseColorBElement = str;
		break;
	case 17:
		m_specularColorElement = str;
		break;
	case 18:
		m_specularColorRElement = str;
		break;
	case 19:
		m_specularColorGElement = str;
		break;
	case 20:
		m_specularColorBElement = str;
		break;
	case 21:
		m_constantElement = str;
		break;
	case 22:
		m_linearElement = str;
		break;
	case 23:
		m_quadraticElement = str;
		break;
	case 24:
		m_loweastLumElement = str;
		break;
	case 25:
		m_distanceElement = str;
		break;
	case 26:
		m_cutoffElement = str;
		break;
	case 27:
		m_dropoffElement = str;
		break;
	case 28:
		m_enableElement = str;
		break;
	case 29:
		m_calcShadowElement = str;
		break;
	case 30:
		m_calcShadow2Element = str;
		break;
	}
}

void crModifyLightAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(!stage);
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_lightListElement));
		assert(!listbox);
		if(listbox->getDataSize()>0)
		{
			std::string selectLightName;
			listbox->getData(listbox->getSelect(),selectLightName);
			ref_ptr<crLightSource>selectLightSource = crLightSourceManager::getInstance()->findLightSource(selectLightName);
			assert(!selectLightSource.valid());
			crLight *light = dynamic_cast<crLight *>(selectLightSource->getLight());
			assert(!light);

			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId(m_editorRootName);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			m_editorRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(selectLightName);
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			m_editorRoot->accept(searchByNameVisitor);
			crMatrixTransform* matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());

			std::string str;
			crEditBoxElement *nameElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nameElement));
			if(nameElement)
			{
				selectLightName = nameElement->getStringArrayInString();
				selectLightSource->setName(selectLightName);
                listbox->getData(listbox->getSelect(),selectLightName);
			}
			crStaticTextBoxElement *currentSelectLightElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_currentSelectLightElement));
			if(currentSelectLightElement)
			{
				currentSelectLightElement->setStringArrayByString(selectLightName);
			}
			crEditBoxElement *descriptionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_descriptionElement));
			if(descriptionElement)
			{
				std::string command = descriptionElement->getStringArrayInString();
				selectLightSource->setDescriptionsInString(command);
				std::string cfgFile;
				char buf[16];
				for(int i = 0; i<10; i++)
				{
					sprintf(buf,"cfgFile%d\0",i);
					if(crArgumentParser::readKeyValue(command,buf,cfgFile))
					{
						CREncapsulation::loadCfgFile(cfgFile,selectLightSource.get());
					}
				}
				std::string script;
				if(crArgumentParser::readKeyValue(command,"EventScript",script))
				{
					CREncapsulation::loadEventScript(script,selectLightSource.get());
				}

				std::string str;
				if(CRCore::crArgumentParser::readKeyValue(command,"AttachToNode",str))
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setNameId(str);
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
					crNode* targetNode = searchByNameVisitor.getResult();
					if(targetNode)
					{
						ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
						ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
						ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

						crVector3 offset;
						CRCore::crArgumentParser::readKeyValue(command,"AttachOffset",offset);
						method->inputParam(3,targetNode);
						method->inputParam(4,&offset);
						float rotAngle = 0;
						CRCore::crArgumentParser::readKeyValue(command,"AttachRotAngle",rotAngle);
						method->inputParam(6,&rotAngle);
						if(CRCore::crArgumentParser::readKeyValue(command,"BoneName",str))
						{
							method->inputParam(7,&str);
						}
						driver->inputHandle(0,method.get());
						data->insertHandle(WCH_UPDATEVISITOR,driver.get());
						selectLightSource->setDataClass(data.get());
						matrixTransform->setDataClass(data.get());
					}
				}
			}
			crCheckBoxElement *enableElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableElement));
			if(enableElement)
			{
				selectLightSource->setEnable(!enableElement->getSelect());
			}
			crCheckBoxElement *calcShadowElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_calcShadowElement));
			if(calcShadowElement)
			{
				selectLightSource->setCalcShadow(!calcShadowElement->getSelect());
			}
			crCheckBoxElement *calcShadow2Element = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_calcShadow2Element));
			if(calcShadow2Element)
			{
				selectLightSource->setCalcShadow2(!calcShadow2Element->getSelect());
			}
			crVector4& position = light->getPosition();
			crEditBoxElement *positionXElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionXElement));
			if(positionXElement)
			{
				position[0] = atof(positionXElement->getStringArrayInString().c_str());
			}
			crEditBoxElement *positionYElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionYElement));
			if(positionYElement)
			{
				position[1] = atof(positionYElement->getStringArrayInString().c_str());
			}
			crEditBoxElement *positionZElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_positionZElement));
			if(positionZElement)
			{
				position[2] = atof(positionZElement->getStringArrayInString().c_str());
			}
			//float azimuth,elevation;
			//crEditBoxElement *azimuthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_azimuthElement));
			//if(azimuthElement)
			//{
			//	azimuth = CRCore::inDegrees(atof(azimuthElement->getStringArrayInString().c_str()));
			//}
			//crEditBoxElement *elevationElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_elevationElement));
			//if(elevationElement)
			//{
			//	elevation = CRCore::inDegrees(atof(elevationElement->getStringArrayInString().c_str()));
			//}
			const crVector3& dir = light->getDirection();
			//dir.set(cos(azimuth)*sin(-elevation),
			//	cos(azimuth)*cos(-elevation),
			//	sin(azimuth));
			//light->setDirection(dir);
			crRadioGroupElement *lightTypeGroupElement = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_lightTypeGroupElement));
			if(lightTypeGroupElement)
			{//SunLight不可在编辑器里设置，只支持1个SunLight
				switch(lightTypeGroupElement->getSelect())
				{
				case 0:
					light->setLightType(crLight::LIGHT_LOCAL);
					//selectLightSource->setLightToBeSunLight(false);
					break;
				case 1:
					light->setLightType(crLight::LIGHT_SPOT);
					//selectLightSource->setLightToBeSunLight(false);
					break;
					//case 2:
					//    light->setLightType(crLight::LIGHT_LOCAL);
					//             selectLightSource->setLightToBeSunLight(true);
					//	break;
				}
			}
			crVector4 ambientColor = light->getAmbient();
			//m_ambientColorElement
			crEditBoxElement *ambientColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorRElement));
			if(ambientColorRElement)
			{
				ambientColor[0] = atof(ambientColorRElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *ambientColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorGElement));
			if(ambientColorGElement)
			{
				ambientColor[1] = atof(ambientColorGElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *ambientColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorBElement));
			if(ambientColorBElement)
			{
				ambientColor[2] = atof(ambientColorBElement->getStringArrayInString().c_str()) / 255.0f;
			}
			light->setAmbient(ambientColor);
			crVector4 diffuseColor = light->getDiffuse();
			//diffuseColorElement
			crEditBoxElement *diffuseColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorRElement));
			if(diffuseColorRElement)
			{
				diffuseColor[0] = atof(diffuseColorRElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *diffuseColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorGElement));
			if(diffuseColorGElement)
			{
				diffuseColor[1] = atof(diffuseColorGElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *diffuseColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorBElement));
			if(diffuseColorBElement)
			{
				diffuseColor[2] = atof(diffuseColorBElement->getStringArrayInString().c_str()) / 255.0f;
			}
			light->setDiffuse(diffuseColor);
			crVector4 specularColor = light->getSpecular();
			//specularColorElement
			crEditBoxElement *specularColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorRElement));
			if(specularColorRElement)
			{
				specularColor[0] = atof(specularColorRElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *specularColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorGElement));
			if(specularColorGElement)
			{
				specularColor[1] = atof(specularColorGElement->getStringArrayInString().c_str()) / 255.0f;
			}
			crEditBoxElement *specularColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorBElement));
			if(specularColorBElement)
			{
				specularColor[2] = atof(specularColorBElement->getStringArrayInString().c_str()) / 255.0f;
			}
			light->setSpecular(specularColor);
			////////attenuation
			crEditBoxElement *constantElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_constantElement));
			if(constantElement)
			{
				light->setConstantAttenuation(atof(constantElement->getStringArrayInString().c_str()));
			}
			//float constant = light->getConstantAttenuation();
			crEditBoxElement *linearElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_linearElement));
			if(linearElement)
			{
				light->setLinearAttenuation(atof(linearElement->getStringArrayInString().c_str()));
			}
			//float linear = light->getLinearAttenuation();
			crEditBoxElement *quadraticElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_quadraticElement));
			if(quadraticElement)
			{
				light->setQuadraticAttenuation(atof(quadraticElement->getStringArrayInString().c_str()));
			}
			//float quadratic = light->getQuadraticAttenuation();
			crEditBoxElement *loweastLumElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_loweastLumElement));
			if(loweastLumElement)
			{
				light->setLoweastLum(atof(loweastLumElement->getStringArrayInString().c_str()));
			}
			crEditBoxElement *distanceElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_distanceElement));
			if(distanceElement)
			{
				distanceElement->setStringArrayByString(crArgumentParser::appFtoa(light->getMaxDistance()));
			}
			crEditBoxElement *cutoffElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_cutoffElement));
			if(cutoffElement)
			{
				light->setSpotCutoff(atof(cutoffElement->getStringArrayInString().c_str()));
			}
			crEditBoxElement *dropoffElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_dropoffElement));
			if(dropoffElement)
			{
				light->setSpotExponent(atof(dropoffElement->getStringArrayInString().c_str()));
			}
			selectLightSource->initLightParamStruct();

			if(matrixTransform)
			{
				matrixTransform->setName(selectLightName);
				std::string des = matrixTransform->getDescriptionsInString();
				crArgumentParser::updateOrAddKeyValue(des,"EditorNodeName",selectLightName);
				matrixTransform->setDescriptionsInString(des);
				crMatrix mat = CRCore::crMatrix::rotate(CRCore::Y_AXIS,dir);
				mat.setTrans(crVector3(position[0],position[1],position[2]));
				matrixTransform->setMatrix(mat);
			}
		}
	}
}
/////////////////////////////////////////
//
//crChooseColorMethod
//
/////////////////////////////////////////
crChooseColorMethod::crChooseColorMethod(){}
crChooseColorMethod::crChooseColorMethod(const crChooseColorMethod& handle):
crMethod(handle)
{
}
void crChooseColorMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crChooseColorElement*)param;
}

void crChooseColorMethod::addParam(int i, const std::string& str)
{
}

void crChooseColorMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		CHOOSECOLOR cc;                 
		static COLORREF acrCustClr[16];
		HWND hwnd = crWindowContainer::getWindow(m_element->getParentStage());

		// Initialize CHOOSECOLOR 
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = hwnd;
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = m_element->getColor();
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (ChooseColor(&cc)==TRUE) 
		{
			m_element->setColor(cc.rgbResult);
		}
	}
}
/////////////////////////////////////////
//
//crUpdateElementMethod
//
/////////////////////////////////////////
crUpdateElementMethod::crUpdateElementMethod(){}
crUpdateElementMethod::crUpdateElementMethod(const crUpdateElementMethod& handle):
crMethod(handle),
m_stageName(handle.m_stageName),
m_elementName(handle.m_elementName)
{
}
void crUpdateElementMethod::inputParam(int i, void *param)
{
}
void crUpdateElementMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageName = str;
		break;
	case 1:
		m_elementName = str;
		break;
	}
}
void crUpdateElementMethod::operator()(crHandle &handle)
{
	crImageStage* imageStage = cr2DStageManager::getInstance()->findStage(m_stageName);
	if(imageStage) 
	{
		crElement *element = imageStage->getElement(m_elementName);
		if(element)
		{
			element->updateData();
		}
	}
}
/////////////////////////////////////////
//
//crSetChooseColorMethod
//
/////////////////////////////////////////
crSetChooseColorMethod::crSetChooseColorMethod(){}
crSetChooseColorMethod::crSetChooseColorMethod(const crSetChooseColorMethod& handle):
crMethod(handle)
{
}
void crSetChooseColorMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crChooseColorElement*)param;
}

void crSetChooseColorMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_elementR = str;
		break;
	case 1:
		m_elementG = str;
		break;
	case 2:
		m_elementB = str;
		break;
	}
}

void crSetChooseColorMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *parentStage = m_element->getParentStage();
		assert(!parentStage);
		COLORREF color = m_element->getColor();
		BYTE r = GetRValue(color);
		BYTE g = GetGValue(color);
		BYTE b = GetBValue(color);
		crEditBoxElement *elementR = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementR));
		if(elementR)
		{
			r = (BYTE)(atoi(elementR->getStringArrayInString().c_str()));
		}
		crEditBoxElement *elementG = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementG));
		if(elementG)
		{
			g = (BYTE)(atoi(elementG->getStringArrayInString().c_str()));
		}
		crEditBoxElement *elementB = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementB));
		if(elementB)
		{
			b = (BYTE)(atoi(elementB->getStringArrayInString().c_str()));
		}
		m_element->setColor(RGB(r,g,b));
	}
}
/////////////////////////////////////////
//
//crSetComponentColorMethod
//
/////////////////////////////////////////
crSetComponentColorMethod::crSetComponentColorMethod(){}
crSetComponentColorMethod::crSetComponentColorMethod(const crSetComponentColorMethod& handle):
crMethod(handle)
{
}
void crSetComponentColorMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crChooseColorElement*)param;
}

void crSetComponentColorMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_elementR = str;
		break;
	case 1:
		m_elementG = str;
		break;
	case 2:
		m_elementB = str;
		break;
	}
}

void crSetComponentColorMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *parentStage = m_element->getParentStage();
		assert(!parentStage);
		COLORREF color = m_element->getColor();
		BYTE r = GetRValue(color);
		BYTE g = GetGValue(color);
		BYTE b = GetBValue(color);
		crEditBoxElement *elementR = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementR));
		if(elementR)
		{
			elementR->setStringArrayByString(crArgumentParser::appItoa(r));
		}
		crEditBoxElement *elementG = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementG));
		if(elementG)
		{
			elementG->setStringArrayByString(crArgumentParser::appItoa(g));
		}
		crEditBoxElement *elementB = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_elementB));
		if(elementB)
		{
			elementB->setStringArrayByString(crArgumentParser::appItoa(b));
		}
	}
}
/////////////////////////////////////////
//
//crDisplayCharacterAttrMethod
//
/////////////////////////////////////////
crDisplayCharacterAttrMethod::crDisplayCharacterAttrMethod(){}
crDisplayCharacterAttrMethod::crDisplayCharacterAttrMethod(const crDisplayCharacterAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_scaleElement(handle.m_scaleElement),
m_offsetXElement(handle.m_offsetXElement),
m_offsetYElement(handle.m_offsetYElement),
m_offsetZElement(handle.m_offsetZElement),
m_animListElement(handle.m_animListElement)
{
}
void crDisplayCharacterAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayCharacterAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_scaleElement = str;
		break;
	case 2:
		m_offsetXElement = str;
		break;
	case 3:
		m_offsetYElement = str;
		break;
	case 4:
		m_offsetZElement = str;
		break;
	case 5:
		m_animListElement = str;
		break;
	}
}

void crDisplayCharacterAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *firstSelectNode = dynamic_cast<crGroup *>(selectNodeVec[0].get());
			if(firstSelectNode)
			{
				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByClassNameVisitor.insertClassNameId("CreBodyNode");
				firstSelectNode->accept(searchByClassNameVisitor);
				CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(searchByClassNameVisitor.getResult());
				if(bodyNode)
				{
					//int select = 2;
					//infoData->inputParam(0,&select);
					//bodyInfo->unSelectAll();
					//bodyInfo->selectNode(bodyNode);

					crEditBoxElement *currentSelect = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
					if(currentSelect)
					{
                        currentSelect->setStringArrayByString(bodyNode->getName());
					}
					crEditBoxElement *scale = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_scaleElement));
					if(scale)
					{
						float flt1;
						if(!CRCore::crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"scale",flt1))
						{
                            flt1 = bodyNode->getBody()->getScale();
						}
						scale->setStringArrayByString(crArgumentParser::appFtoa(flt1));
					}
					crVector3 vec3;
					if(!CRCore::crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"trans_offset",vec3))
					{
						vec3 = bodyNode->getBody()->getCoreBody()->getTransOffset();
					}
					crEditBoxElement *offsetX = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_offsetXElement));
					if(offsetX)
					{
						offsetX->setStringArrayByString(crArgumentParser::appFtoa(vec3[0]));
					}
					crEditBoxElement *offsetY = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_offsetYElement));
					if(offsetY)
					{
						offsetY->setStringArrayByString(crArgumentParser::appFtoa(vec3[1]));
					}
					crEditBoxElement *offsetZ = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_offsetZElement));
					if(offsetZ)
					{
						offsetZ->setStringArrayByString(crArgumentParser::appFtoa(vec3[2]));
					}
					crColumnListElement *animList = dynamic_cast<crColumnListElement *>(m_imageStage->getElement(m_animListElement));
					if(animList)
					{
						animList->clearData();
						const Body::ActNameSet& actNameSet = bodyNode->getBody()->getActNameSet();
						std::string default_act;
						CRCore::crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"default_act",default_act);
						//int i = 0;
						//int currentAct = 0;
						std::list<AnimationBase *> act_anim_list;
						for( Body::ActNameSet::const_iterator itr = actNameSet.begin();
							 itr != actNameSet.end();
							 ++itr/*,i++*/ )
						{
							bodyNode->getBody()->getCoreBody()->getAnimationsWithActName(*itr,act_anim_list);
							//if(default_act.compare(*itr) == 0)
							//{
       //                         currentAct = i;
							//}
							//animList->addData(*itr);
						}
						ref_ptr<crColumnNode> columnNode;
						for( std::list<AnimationBase *>::iterator itr = act_anim_list.begin();
							 itr != act_anim_list.end();
							 ++itr )
						{
							columnNode = new crColumnNode;
							columnNode->addTitle((*itr)->getName());
							columnNode->addTitle((*itr)->getActName());
							animList->addData(columnNode.get());
							if(default_act.compare((*itr)->getActName()) == 0)
							{
                                animList->select(animList->getDataSize());
							}
						}
					}
				}	
			}
		}
	}
}
/////////////////////////////////////////
//
//crDoAnimMethod
//
/////////////////////////////////////////
crDoAnimMethod::crDoAnimMethod(){}
crDoAnimMethod::crDoAnimMethod(const crDoAnimMethod& handle):
crMethod(handle)
{
}
void crDoAnimMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_columnList = (CRGUI::crColumnListElement*)param;
}

void crDoAnimMethod::addParam(int i, const std::string& str)
{
}

void crDoAnimMethod::operator()(crHandle &handle)
{
	if(m_columnList.valid()&&m_columnList->getDataSize()>0)
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(!body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *firstSelectNode = dynamic_cast<crGroup *>(selectNodeVec[0].get());
			if(firstSelectNode)
			{
				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByClassNameVisitor.insertClassNameId("CreBodyNode");
				firstSelectNode->accept(searchByClassNameVisitor);
				CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(searchByClassNameVisitor.getResult());
				if(bodyNode)
				{
					crColumnNode *node = m_columnList->getData(m_columnList->getSelect()-1);
					if(node)
					{
						std::string anim;
						node->getTitle(1,anim);
						//m_listbox->getData(m_listbox->getSelect(),anim);
						ActionRequest* action = bodyNode->getBody()->getActionPrototype( anim );
						if (action) 
						{
							action->setPrioritized(false);
							bodyNode->getBody()->executeAction(action, false);
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyCharacterAttrMethod
//
/////////////////////////////////////////
crModifyCharacterAttrMethod::crModifyCharacterAttrMethod(){}
crModifyCharacterAttrMethod::crModifyCharacterAttrMethod(const crModifyCharacterAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_scaleElement(handle.m_scaleElement),
m_offsetXElement(handle.m_offsetXElement),
m_offsetYElement(handle.m_offsetYElement),
m_offsetZElement(handle.m_offsetZElement),
m_animListElement(handle.m_animListElement)
{
}
void crModifyCharacterAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyCharacterAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_scaleElement = str;
		break;
	case 2:
		m_offsetXElement = str;
		break;
	case 3:
		m_offsetYElement = str;
		break;
	case 4:
		m_offsetZElement = str;
		break;
	case 5:
		m_animListElement = str;
		break;
	}
}

void crModifyCharacterAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *parentStage = m_element->getParentStage();
		assert(!parentStage);
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(!body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *firstSelectNode = dynamic_cast<crGroup *>(selectNodeVec[0].get());
			if(firstSelectNode)
			{
				CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
				searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByClassNameVisitor.insertClassNameId("CreBodyNode");
				firstSelectNode->accept(searchByClassNameVisitor);
				CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(searchByClassNameVisitor.getResult());
				if(bodyNode)
				{
					crEditBoxElement *currentSelect = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_currentSelectElement));
					if(currentSelect)
					{
						if(currentSelect->getStringArrayInString().compare(bodyNode->getName())!=0)
							return;
					}
					std::string command = bodyNode->getDescriptionsInString();
					float scale = bodyNode->getBody()->getScale();
					crEditBoxElement *scaleElement = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_scaleElement));
					if(scaleElement)
					{
						scale = atof(scaleElement->getStringArrayInString().c_str());
					}
					crVector3 trans_offset = bodyNode->getBody()->getCoreBody()->getTransOffset();
					crEditBoxElement *offsetX = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_offsetXElement));
					if(offsetX)
					{
						trans_offset[0] = atof(offsetX->getStringArrayInString().c_str());
					}
					crEditBoxElement *offsetY = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_offsetYElement));
					if(offsetY)
					{
						trans_offset[1] = atof(offsetY->getStringArrayInString().c_str());
					}
					crEditBoxElement *offsetZ = dynamic_cast<crEditBoxElement *>(parentStage->getElement(m_offsetZElement));
					if(offsetZ)
					{
						trans_offset[2] = atof(offsetZ->getStringArrayInString().c_str());
					}
					CRCore::crArgumentParser::updateOrAddKeyValue(command,"scale",scale);
					CRCore::crArgumentParser::updateOrAddKeyValue(command,"trans_offset",trans_offset);
                    bodyNode->getBody()->setScale(scale);
					bodyNode->getBody()->getCoreBody()->setTransOffset(trans_offset);

					CRCore::crMatrix m;
					m.makeScale(scale, scale, scale);
					m.setTrans(trans_offset);
					CRCore::crMatrixTransform *bodyTransform = bodyNode->getBodyTransform();
					assert(!bodyTransform);
					bodyTransform->setMatrix(m);
					
					//crListBoxElement *animList = dynamic_cast<crListBoxElement *>(parentStage->getElement(m_animListElement));
					//if(animList&&animList->getDataSize()>0)
					//{
					//	std::string default_act;
					//	animList->getData(animList->getSelect(),default_act);
					//	CRCore::crArgumentParser::updateOrAddKeyValue(command,"default_act",default_act);
					//	bodyNode->getBody()->getCoreBody()->setDefaultActivity(default_act);

					//	rbody::ActionRequest *action = bodyNode->getBody()->getActionPrototype(default_act);
					//	if (action) 
					//	{
					//		action->setPrioritized(0);
					//		bodyNode->getBody()->executeAction(action, true);
					//	}
					//}
					crColumnListElement *animList = dynamic_cast<crColumnListElement *>(parentStage->getElement(m_animListElement));
					if(animList&&animList->getDataSize()>0)
					{
						crColumnNode *selectNode = animList->getData(animList->getSelect()-1);
						if(selectNode)
						{
							std::string default_act;
                            selectNode->getTitle(1,default_act);
							CRCore::crArgumentParser::updateOrAddKeyValue(command,"default_act",default_act);
							bodyNode->getBody()->getCoreBody()->setDefaultActivity(default_act);

							ActionRequest* action = bodyNode->getBody()->getActionPrototype( default_act);
							if (action) 
							{
								action->setPrioritized(false);
								bodyNode->getBody()->executeAction(action, false);
							}
						}
					}
					bodyNode->setDescriptionsInString(command);
				}	
			}
		}
	}
}

/////
/////////////////////////////////////////
//
//crAddToListMethod
//
/////////////////////////////////////////
crAddToListMethod::crAddToListMethod(){}
crAddToListMethod::crAddToListMethod(const crAddToListMethod& handle):
crMethod(handle),
m_toListBoxElement(handle.m_toListBoxElement)
{
}
void crAddToListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_fromListbox = (CRGUI::crListBoxElement*)param;
}

void crAddToListMethod::addParam(int i, const std::string& str)
{
	if(i==0)
		m_toListBoxElement = str;
}

void crAddToListMethod::operator()(crHandle &handle)
{
	if(m_fromListbox.valid() && m_fromListbox->getDataSize()>0)
	{
		crImageStage *parentStage = m_fromListbox->getParentStage();
		assert(!parentStage);
		crListBoxElement *toListBox = dynamic_cast<crListBoxElement *>(parentStage->getElement(m_toListBoxElement));
		if(toListBox)
		{
			std::string data,toData;
			m_fromListbox->getData(m_fromListbox->getSelect(),data);
			bool find = false;
            for(int i = 0; i<toListBox->getDataSize(); ++i)
			{
				toListBox->getData(i,toData);
                if(data.compare(toData) == 0)
				{
					find = true;
					break;
				}
			}
			if(!find)
			{
                toListBox->addData(data);
				toListBox->updateData();
			}
		}
	}
}
/////////////////////////////////////////
//
//crListDeleteCurrentDataMethod
//
/////////////////////////////////////////
crListDeleteCurrentDataMethod::crListDeleteCurrentDataMethod(){}
crListDeleteCurrentDataMethod::crListDeleteCurrentDataMethod(const crListDeleteCurrentDataMethod& handle):
crMethod(handle)
{
}
void crListDeleteCurrentDataMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crListDeleteCurrentDataMethod::addParam(int i, const std::string& str)
{
}

void crListDeleteCurrentDataMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		m_listbox->deleteCurrentRow();
		m_listbox->updateData();
	}
}

/////////////////////////////////////////
//
//crUpdateObjectAvoidLightListMethod
//
/////////////////////////////////////////
crUpdateObjectAvoidLightListMethod::crUpdateObjectAvoidLightListMethod(){}
crUpdateObjectAvoidLightListMethod::crUpdateObjectAvoidLightListMethod(const crUpdateObjectAvoidLightListMethod& handle):
crMethod(handle),
m_selectListElement(handle.m_selectListElement),
m_avoidLightElement(handle.m_avoidLightElement)
{
}
void crUpdateObjectAvoidLightListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crUpdateObjectAvoidLightListMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_selectListElement = str;
		break;
	case 1:
		m_avoidLightElement = str;
		break;
	}
}

void crUpdateObjectAvoidLightListMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crListBoxElement *selectList = dynamic_cast<crListBoxElement *>(stage->getElement(m_selectListElement));
		crListBoxElement *avoidLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_avoidLightElement));
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo && selectList && avoidLightElement)
		{
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			int currentSelect = selectList->getSelect();
			if(currentSelect<selectNodeVec.size())
			{
				crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[currentSelect].get());
				if(currentSelectObject)
				{
					currentSelectObject->lock();
					currentSelectObject->clearAvoidLight();
					std::string data;
					for(int i = 0; i<avoidLightElement->getDataSize(); ++i)
					{
						avoidLightElement->getData(i,data);
						currentSelectObject->addAvoidLight(data);
					}
					currentSelectObject->unlock();
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crUpdateObjectNeedLightListMethod
//
/////////////////////////////////////////
crUpdateObjectNeedLightListMethod::crUpdateObjectNeedLightListMethod(){}
crUpdateObjectNeedLightListMethod::crUpdateObjectNeedLightListMethod(const crUpdateObjectNeedLightListMethod& handle):
crMethod(handle),
m_selectListElement(handle.m_selectListElement),
m_needLightElement(handle.m_needLightElement)
{
}
void crUpdateObjectNeedLightListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crUpdateObjectNeedLightListMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_selectListElement = str;
		break;
	case 1:
		m_needLightElement = str;
		break;
	}
}

void crUpdateObjectNeedLightListMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crListBoxElement *selectList = dynamic_cast<crListBoxElement *>(stage->getElement(m_selectListElement));
		crListBoxElement *needLightElement = dynamic_cast<crListBoxElement *>(stage->getElement(m_needLightElement));
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo && selectList && needLightElement)
		{
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			int currentSelect = selectList->getSelect();
			if(currentSelect<selectNodeVec.size())
			{
				crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[currentSelect].get());
				if(currentSelectObject)
				{
					currentSelectObject->lock();
					currentSelectObject->clearAvoidLight();
					std::string data;
					for(int i = 0; i<needLightElement->getDataSize(); ++i)
					{
						needLightElement->getData(i,data);
						currentSelectObject->addNeedLight(data);
					}
					currentSelectObject->unlock();
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisplayDrawableMaterialMethod
//
/////////////////////////////////////////
crDisplayDrawableMaterialMethod::crDisplayDrawableMaterialMethod(){}
crDisplayDrawableMaterialMethod::crDisplayDrawableMaterialMethod(const crDisplayDrawableMaterialMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_baseMapElement(handle.m_baseMapElement),
m_normalMapElement(handle.m_normalMapElement),
m_lightMapElement(handle.m_lightMapElement),
m_envMapElement(handle.m_envMapElement),
m_diffuseElement(handle.m_diffuseElement),
m_specularElement(handle.m_specularElement),
m_emissiveElement(handle.m_emissiveElement),
m_normal2Element(handle.m_normal2Element),
m_diffuse2Element(handle.m_diffuse2Element),
m_specular2Element(handle.m_specular2Element),
m_emissive2Element(handle.m_emissive2Element),
m_normal3Element(handle.m_normal3Element),
m_specular3Element(handle.m_specular3Element),
m_emissive3Element(handle.m_emissive3Element),
m_uvsElement(handle.m_uvsElement),
m_colorWeightElement(handle.m_colorWeightElement),
m_ambientColorElement(handle.m_ambientColorElement),
m_ambientColorRElement(handle.m_ambientColorRElement),
m_ambientColorGElement(handle.m_ambientColorGElement),
m_ambientColorBElement(handle.m_ambientColorBElement),
m_diffuseColorElement(handle.m_diffuseColorElement),
m_diffuseColorRElement(handle.m_diffuseColorRElement),
m_diffuseColorGElement(handle.m_diffuseColorGElement),
m_diffuseColorBElement(handle.m_diffuseColorBElement),
m_specularColorElement(handle.m_specularColorElement),
m_specularColorRElement(handle.m_specularColorRElement),
m_specularColorGElement(handle.m_specularColorGElement),
m_specularColorBElement(handle.m_specularColorBElement),
m_shininessElement(handle.m_shininessElement),
m_emissiveColorElement(handle.m_emissiveColorElement),
m_emissiveColorRElement(handle.m_emissiveColorRElement),
m_emissiveColorGElement(handle.m_emissiveColorGElement),
m_emissiveColorBElement(handle.m_emissiveColorBElement),
m_detailGridElement(handle.m_detailGridElement)
{
}
void crDisplayDrawableMaterialMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayDrawableMaterialMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_baseMapElement = str;
		break;
	case 2:
		m_normalMapElement = str;
		break;
	case 3:
		m_lightMapElement = str;
		break;
	case 4:
		m_envMapElement = str;
		break;
	case 5:
		m_diffuseElement = str;
		break;
	case 6:
		m_specularElement = str;
		break;
	case 7:
		m_emissiveElement = str;
		break;
	case 8:
		m_normal2Element = str;
		break;
	case 9:
		m_diffuse2Element = str;
		break;
	case 10:
		m_specular2Element = str;
		break;
	case 11:
		m_emissive2Element = str;
		break;
	case 12:
		m_normal3Element = str;
		break;
	case 13:
		m_specular3Element = str;
		break;
	case 14:
		m_emissive3Element = str;
		break;
	case 15:
		m_uvsElement = str;
		break;
	case 16:
		m_colorWeightElement = str;
		break;
	case 17:
		m_ambientColorElement = str;
		break;
	case 18:
		m_ambientColorRElement = str;
		break;
	case 19:
		m_ambientColorGElement = str;
		break;
	case 20:
		m_ambientColorBElement = str;
		break;
	case 21:
		m_diffuseColorElement = str;
		break;
	case 22:
		m_diffuseColorRElement = str;
		break;
	case 23:
		m_diffuseColorGElement = str;
		break;
	case 24:
		m_diffuseColorBElement = str;
		break;
	case 25:
		m_specularColorElement = str;
		break;
	case 26:
		m_specularColorRElement = str;
		break;
	case 27:
		m_specularColorGElement = str;
		break;
	case 28:
		m_specularColorBElement = str;
		break;
	case 29:
		m_shininessElement = str;
		break;
	case 30:
		m_emissiveColorElement = str;
		break;
	case 31:
		m_emissiveColorRElement = str;
		break;
	case 32:
		m_emissiveColorGElement = str;
		break;
	case 33:
		m_emissiveColorBElement = str;
		break;
	case 34:
		m_detailGridElement = str;
		break;
	}
}
void crDisplayDrawableMaterialMethod::getFilterModeStr(crTexture::FilterMode filterMode,std::string &str)
{
	switch (filterMode)
	{
	case crTexture::LINEAR:
		str = "LINEAR";
		break;
	case crTexture::LINEAR_MIPMAP_LINEAR:
		str = "LINEAR_MIPMAP_LINEAR";
		break;
	case crTexture::LINEAR_MIPMAP_NEAREST:
		str = "LINEAR_MIPMAP_NEAREST";
		break;
	case crTexture::NEAREST:
		str = "NEAREST";
		break;
	case crTexture::NEAREST_MIPMAP_LINEAR:
		str = "NEAREST_MIPMAP_LINEAR";
		break;
	case crTexture::NEAREST_MIPMAP_NEAREST:
		str = "NEAREST_MIPMAP_NEAREST";
		break;
	}
}
void crDisplayDrawableMaterialMethod::getWrapModeStr(crTexture::WrapMode wrapMode,std::string &str)
{
	switch (wrapMode)
	{
	case crTexture::CLAMP:
		str = "CLAMP";
		break;
	case crTexture::CLAMP_TO_EDGE:
		str = "CLAMP_TO_EDGE";
		break;
	case crTexture::CLAMP_TO_BORDER:
		str = "CLAMP_TO_BORDER";
		break;
	case crTexture::REPEAT:
		str = "REPEAT";
		break;
	case crTexture::MIRROR:
		str = "MIRROR";
		break;
	}
}
void crDisplayDrawableMaterialMethod::getTextureOperateStr(int op, std::string &str)
{
	switch (op)
	{
	case 0:
		str = "Add";
		break;
	case 1:
		str = "Mult";
		break;
	case 2:
		str = "Replace";
		break;
	case 3:
		str = "Ignore";
		break;
	case 4:
		str = "AddETTo";
		break;
	case 5:
		str = "MultETTo";
		break;
	case 6:
		str = "ET3ToLM";
		break;
	}
}
void crDisplayDrawableMaterialMethod::getAlphaOperateStr(int op, std::string &str)
{
	switch (op)
	{
	case 0:
		str = "Ignore";
		break;
	case 1:
		str = "Add";
		break;
	case 2:
		str = "Mult";
		break;
	case 3:
		str = "Replace";
		break;
	}
}
void crDisplayDrawableMaterialMethod::getBinDetailStr(int binnum,std::string &str)
{
	switch (binnum)
	{
	case crStateSet::Trans:
		str = "Trans";
		break;
	case crStateSet::PreTrans:
		str = "PreTrans";
		break;
	case crStateSet::PostTrans:
		str = "PostTrans";
		break;
	case crStateSet::FX:
		str = "FXTrans";
		break;
	default:
		str = "默认";
		break;
	}
}
void crDisplayDrawableMaterialMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);

		bool clearData = true;
		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
			if(currentSelectObject)
			{
				crData *infoData = bodyInfo->getDataClass();
				assert(infoData);
				void *_selectDrawable;
				crDrawable *selectDrawable = NULL;
				infoData->getParam(7,_selectDrawable);
				if(_selectDrawable)
				{
					selectDrawable = (crDrawable*)_selectDrawable;
					clearData = false;

					std::string str;
					crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
					if(currentSelectElement)
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						currentSelectObject->accept(searchByNameVisitor);
						crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(proxyNode)
						{
							if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
							{
								currentSelectElement->setStringArrayByString(str);
							}
							else
								currentSelectElement->clearString();
						}
						else
							currentSelectElement->clearString();
					}
                    crStateSet *stateset = selectDrawable->getStateSet();
					if(!stateset)
						stateset = currentSelectObject->getStateSet();
					if(!stateset)
						return;
					//crStateSet *objStateset = currentSelectObject->getStateSet();

					crTexture *baseMap = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *baseMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_baseMapElement));
					if(baseMapElement)
					{
						if(baseMap) baseMapElement->setStringArrayByString(baseMap->getImageNameID());
						else baseMapElement->clearString();
					}
					crTexture *normalMap = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *normalMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normalMapElement));
					if(normalMapElement)
					{
						if(normalMap) normalMapElement->setStringArrayByString(normalMap->getImageNameID());
						else normalMapElement->clearString();
					}
					crTexture *lightMap = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *lightMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lightMapElement));
					if(lightMapElement)
					{
						if(lightMap) lightMapElement->setStringArrayByString(lightMap->getImageNameID());
						else lightMapElement->clearString();
					}
					crTextureCubeMap *envMap = dynamic_cast<crTextureCubeMap *>(stateset->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *envMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_envMapElement));
					if(envMapElement)
					{
						if(envMap) envMapElement->setStringArrayByString(envMap->getImageNameID());
						else envMapElement->clearString();
					}
					crTexture *diffuseMap = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *diffuseElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseElement));
					if(diffuseElement)
					{
						if(diffuseMap) diffuseElement->setStringArrayByString(diffuseMap->getImageNameID());
						else diffuseElement->clearString();
					}
					crTexture *specularMap = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *specularElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularElement));
					if(specularElement)
					{
						if(specularMap) specularElement->setStringArrayByString(specularMap->getImageNameID());
						else specularElement->clearString();
					}
					crTexture *emissiveMap = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE));
					crEditBoxElement *emissiveElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveElement));
					if(emissiveElement)
					{
						if(emissiveMap) emissiveElement->setStringArrayByString(emissiveMap->getImageNameID());
						else emissiveElement->clearString();
					}
					crTexture *normal2Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE));
					crEditBoxElement *normal2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normal2Element));
					if(normal2Element)
					{
						if(normal2Map) normal2Element->setStringArrayByString(normal2Map->getImageNameID());
						else normal2Element->clearString();
					}
					crTexture *diffuse2Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE));
					crEditBoxElement *diffuse2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuse2Element));
					if(diffuse2Element)
					{
						if(diffuse2Map) diffuse2Element->setStringArrayByString(diffuse2Map->getImageNameID());
						else diffuse2Element->clearString();
					}
					crTexture *specular2Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE));
					crEditBoxElement *specular2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specular2Element));
					if(specular2Element)
					{
						if(specular2Map) specular2Element->setStringArrayByString(specular2Map->getImageNameID());
						else specular2Element->clearString();
					}
					crTexture *emissive2Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_EMISSIVEMAP2,crStateAttribute::TEXTURE));
					crEditBoxElement *emissive2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissive2Element));
					if(emissive2Element)
					{
						if(emissive2Map) emissive2Element->setStringArrayByString(emissive2Map->getImageNameID());
						else emissive2Element->clearString();
					}
					crTexture *normal3Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE));
					crEditBoxElement *normal3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normal3Element));
					if(normal3Element)
					{
						if(normal3Map) normal3Element->setStringArrayByString(normal3Map->getImageNameID());
						else normal3Element->clearString();
					}
					crTexture *specular3Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE));
					crEditBoxElement *specular3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specular3Element));
					if(specular3Element)
					{
						if(specular3Map) specular3Element->setStringArrayByString(specular3Map->getImageNameID());
						else specular3Element->clearString();
					}
					crTexture *emissive3Map = dynamic_cast<crTexture *>(stateset->getTextureAttribute(TEXTURE_EMISSIVEMAP3,crStateAttribute::TEXTURE));
					crEditBoxElement *emissive3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissive3Element));
					if(emissive3Element)
					{
						if(emissive3Map) emissive3Element->setStringArrayByString(emissive3Map->getImageNameID());
						else emissive3Element->clearString();
					}
					/////////////////ExternTexture
					crTexture *uvsMap = NULL;
					crTexture *cwMap = NULL;
					crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(stateset->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
					crTexture::ExternTextureID externTextureID = crTexture::EXT_NONE;
					if(externTex2D)
					{
						externTextureID = externTex2D->getExternTextureID();
						switch (externTextureID)
						{
						case crTexture::EXT_NONE:
							break;
						case crTexture::UVSCRAMBLER:
							uvsMap = externTex2D;
							break;
						case crTexture::COLORWEIGHT:
							cwMap = externTex2D;
							break;
						}
					}
					crEditBoxElement *uvsElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_uvsElement));
					if(uvsElement)
					{
						if(uvsMap) uvsElement->setStringArrayByString(uvsMap->getImageNameID());
						else uvsElement->clearString();
					}
					crEditBoxElement *colorWeightElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_colorWeightElement));
					if(colorWeightElement)
					{
						if(cwMap) colorWeightElement->setStringArrayByString(cwMap->getImageNameID());
						else colorWeightElement->clearString();
					}
                    ///////////////////////////////////

					crMaterial *material = dynamic_cast<crMaterial *>(stateset->getAttribute(crStateAttribute::MATERIAL));
                    crVector4 ambientColor,diffuseColor,specularColor,emissiveColor;
					float shininess = 0.0f;
					if(material)
					{
						ambientColor = material->getAmbient(crMaterial::FRONT);
						diffuseColor = material->getDiffuse(crMaterial::FRONT);
						specularColor = material->getSpecular(crMaterial::FRONT);
						emissiveColor = material->getEmission(crMaterial::FRONT);
						shininess = material->getShininess(crMaterial::FRONT);
					}
					//m_ambientColorElement
					crEditBoxElement *ambientColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorRElement));
					if(ambientColorRElement)
					{
						ambientColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[0] * 255.0f)));
					}
					crEditBoxElement *ambientColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorGElement));
					if(ambientColorGElement)
					{
						ambientColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[1] * 255.0f)));
					}
					crEditBoxElement *ambientColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorBElement));
					if(ambientColorBElement)
					{
						ambientColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[2] * 255.0f)));
					}
					//diffuseColorElement
					crEditBoxElement *diffuseColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorRElement));
					if(diffuseColorRElement)
					{
						diffuseColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[0] * 255.0f)));
					}
					crEditBoxElement *diffuseColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorGElement));
					if(diffuseColorGElement)
					{
						diffuseColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[1] * 255.0f)));
					}
					crEditBoxElement *diffuseColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorBElement));
					if(diffuseColorBElement)
					{
						diffuseColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[2] * 255.0f)));
					}
					//specularColorElement
					crEditBoxElement *specularColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorRElement));
					if(specularColorRElement)
					{
						specularColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[0] * 255.0f)));
					}
					crEditBoxElement *specularColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorGElement));
					if(specularColorGElement)
					{
						specularColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[1] * 255.0f)));
					}
					crEditBoxElement *specularColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorBElement));
					if(specularColorBElement)
					{
						specularColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[2] * 255.0f)));
					}
					crEditBoxElement *shininessElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_shininessElement));
					if(shininessElement)
					{
						shininessElement->setStringArrayByString(crArgumentParser::appFtoa(shininess));
					}
					//emissiveColorElement
					crEditBoxElement *emissiveColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorRElement));
					if(emissiveColorRElement)
					{
						emissiveColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[0] * 255.0f)));
					}
					crEditBoxElement *emissiveColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorGElement));
					if(emissiveColorGElement)
					{
						emissiveColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[1] * 255.0f)));
					}
					crEditBoxElement *emissiveColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorBElement));
					if(emissiveColorBElement)
					{
						emissiveColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[2] * 255.0f)));
					}

					crKeyValueGridElement *detailGridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_detailGridElement));
					if(detailGridElement)
					{
						detailGridElement->clearData();
						crUniform *uniform;
						float flt1;
						crVector3 vec3;
						crVector4 vec4;

						crStateAttribute::VRModeValue cullface = stateset->getMode(GL_CULL_FACE);
						if(cullface == crStateAttribute::OFF)
						{
							detailGridElement->addData("双面显示",crArgumentParser::appItoa(1),"");
						}
						else
						{
							detailGridElement->addData("双面显示",crArgumentParser::appItoa(0),"");
						}
						crGeometry *geometry = dynamic_cast<crGeometry *>(selectDrawable);
						if(geometry && geometry->getColorBinding() != crGeometry::BIND_OFF)
						{
							detailGridElement->addData("颜色","顶点色","");
						}
						else
						{
							crVector4 color = selectDrawable->getColor();
							color *= 255.0f;
							detailGridElement->addData("颜色",crArgumentParser::appVectoa(crVector4i(color)),"");
						}

						float maskClipValue = 0.3333f;
						crBlendFunc *blendFunc = dynamic_cast<crBlendFunc *>(stateset->getAttribute(crStateAttribute::BLENDFUNC));
						if(blendFunc)
						{
							crBlendFunc::BlendFuncMode source,destination;
							source = (crBlendFunc::BlendFuncMode)(blendFunc->getSource());
							destination = (crBlendFunc::BlendFuncMode)(blendFunc->getDestination());

							//detailGridElement->addData("镂空值",crArgumentParser::appFtoa(maskClipValue),"");
							crAlphaFunc *alphaFunc = dynamic_cast<crAlphaFunc *>(stateset->getAttribute(crStateAttribute::ALPHAFUNC));
							if(alphaFunc)
							{
								maskClipValue = alphaFunc->getReferenceValue();
								detailGridElement->addData("镂空值",crArgumentParser::appFtoa(maskClipValue),"");
							}
							else
							{
								detailGridElement->addData("镂空值",crArgumentParser::appFtoa(0.0f),"");
							}
							if(source == crBlendFunc::SRC_ALPHA && destination == crBlendFunc::ONE_MINUS_SRC_ALPHA)
							{
                                detailGridElement->addData("融合模式","Trans","");
							}
							else if(source == crBlendFunc::ONE && destination == crBlendFunc::ONE)
							{
								detailGridElement->addData("融合模式","Add","");
							}
							else if(source == crBlendFunc::SRC_ALPHA && destination == crBlendFunc::ONE)
							{
								detailGridElement->addData("融合模式","Modulate","");
							}
							getBinDetailStr(stateset->getBinNumber(),str);
							detailGridElement->addData("渲染排序",str,"");
						}
						else
						{
							crAlphaFunc *alphaFunc = dynamic_cast<crAlphaFunc *>(stateset->getAttribute(crStateAttribute::ALPHAFUNC));
                            if(alphaFunc)
							{
								maskClipValue = alphaFunc->getReferenceValue();
								detailGridElement->addData("镂空值",crArgumentParser::appFtoa(maskClipValue),"");
								detailGridElement->addData("融合模式","Mask","");
							}
							else
							{
								detailGridElement->addData("镂空值",crArgumentParser::appFtoa(0.0f),"");
								detailGridElement->addData("融合模式","Opaque","");
							}
							getBinDetailStr(stateset->getBinNumber(),str);
							detailGridElement->addData("渲染排序",str,"");
						}
						crStateAttribute::VRModeValue depthTest = stateset->getMode(GL_DEPTH_TEST);
						if(depthTest == crStateAttribute::OFF)
						{
							detailGridElement->addData("禁用深度",crArgumentParser::appItoa(1),"");
						}
						else
						{
							detailGridElement->addData("禁用深度",crArgumentParser::appItoa(0),"");
						}
						crDepth *depth = dynamic_cast<crDepth *>(stateset->getAttribute(crStateAttribute::DEPTH));
						if(depth)
						{
							crDepth::Function func = depth->getFunction();
							switch (func)
							{
							case crDepth::NEVER:
								detailGridElement->addData("深度测试","NEVER","");
								break;
							case crDepth::LESS:
								detailGridElement->addData("深度测试","默认","");
								break;
							case crDepth::EQUAL:
								detailGridElement->addData("深度测试","EQUAL","");
								break;
							case crDepth::LEQUAL:
								detailGridElement->addData("深度测试","LEQUAL","");
								break;
							case crDepth::GREATER:
								detailGridElement->addData("深度测试","GREATER","");
								break;
							case crDepth::NOTEQUAL:
								detailGridElement->addData("深度测试","NOTEQUAL","");
								break;
							case crDepth::GEQUAL:
								detailGridElement->addData("深度测试","GEQUAL","");
								break;
							case crDepth::ALWAYS:
								detailGridElement->addData("深度测试","ALWAYS","");
								break;
							}
							crVector2f zvalue(depth->getZNear(),depth->getZFar());
							detailGridElement->addData("深度范围",crArgumentParser::appVectoa(zvalue),"");
							bool depthWrite = depth->getWriteMask();
							detailGridElement->addData("Z写入",crArgumentParser::appItoa(depthWrite?1:0),"");
						}
						else
						{
							detailGridElement->addData("深度测试","继承","");
						}
						crPolygonMode* polygonMode = dynamic_cast<crPolygonMode *>(stateset->getAttribute(crStateAttribute::POLYGONMODE));
						if(polygonMode)
						{
							crPolygonMode::Mode mode = polygonMode->getMode(crPolygonMode::FRONT_AND_BACK);
							switch (mode)
							{
							case crPolygonMode::POINT:
								detailGridElement->addData("绘制模式","Point","");
								break;
							case crPolygonMode::LINE:
								detailGridElement->addData("绘制模式","Line","");
								break;
							case crPolygonMode::FILL:
								detailGridElement->addData("绘制模式","Fill","");
								break;
							}
						}
						else
						{
							detailGridElement->addData("绘制模式","Fill","");
						}
                        ///////DT1
						if(baseMap)
						{
							str = baseMap->className();
							str.erase(0,2);
                            detailGridElement->addData(/*"BaseMap(DT1)"*/"颜色1",str,"Red13");
							detailGridElement->addData(/*"AlphaShadowEnable"*/"Alpha阴影",crArgumentParser::appItoa(baseMap->getAlphaShadowEnable()?1:0),"Red13");
						    detailGridElement->addData(/*"BlendShadowEnable"*/"Blend阴影",crArgumentParser::appItoa(baseMap->getBlendShadowEnable()?1:0),"Red13");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_BASEMAP),str);
							detailGridElement->addData(/*"OpBT"*/"通道函数",str,"Red13");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_BASEMAP)),"Red13");

							uniform= stateset->getUniform("param4AddBT");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddBT"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Red13");

							uniform= stateset->getUniform("param1MultBT");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultBT"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13");

							uniform= stateset->getUniform("param4MultBT");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultBT"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Red13");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_BASEMAP),str);
							detailGridElement->addData(/*"OpBTA"*/"Alpha半透明",str,"Red13");

							uniform= stateset->getUniform("param1AddBTa");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddBTa"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Red13");

							uniform= stateset->getUniform("param1MultBTa");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultBTa"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13");

							uniform = stateset->getUniform("baseMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Red13");
							}
							uniform = stateset->getUniform("baseMatA");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Red13");
							}

                            getFilterModeStr(baseMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Red13");
							getFilterModeStr(baseMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Red13");
							getWrapModeStr(baseMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Red13");
						}
						////////DT2
						if(diffuseMap)
						{
							str = diffuseMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"DT2"*/"颜色2",str,"Green13");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_DIFFUSEMAP),str);
							detailGridElement->addData(/*"OpDT"*/"通道函数",str,"Green13");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_DIFFUSEMAP)),"Green13");

							uniform= stateset->getUniform("param4AddDT");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddDT"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Green13");

							uniform= stateset->getUniform("param1MultDT");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultDT"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13");

							uniform= stateset->getUniform("param4MultDT");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultDT"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Green13");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_DIFFUSEMAP),str);
							detailGridElement->addData(/*"OpDTA"*/"Alpha半透明",str,"Green13");

							uniform= stateset->getUniform("param1AddDTa");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddDTa"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Green13");

							uniform= stateset->getUniform("param1MultDTa");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultDTa"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13");

							uniform = stateset->getUniform("diffuseMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Green13");
							}
							uniform = stateset->getUniform("diffuseMatA");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Green13");
							}

							getFilterModeStr(diffuseMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Green13");
							getFilterModeStr(diffuseMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Green13");
							getWrapModeStr(diffuseMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Green13");
						}
						///////DT3
						if(diffuse2Map)
						{
							str = diffuse2Map->className();
							str.erase(0,2);
							detailGridElement->addData(/*"DT3"*/"颜色3",str,"Blue13");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_DIFFUSEMAP2),str);
							detailGridElement->addData(/*"OpDT2"*/"通道函数",str,"Blue13");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_DIFFUSEMAP2)),"Blue13");

							uniform= stateset->getUniform("param4AddDT2");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddDT2"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Blue13");

							uniform= stateset->getUniform("param1MultDT2");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultDT2"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13");

							uniform= stateset->getUniform("param4MultDT2");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultDT2"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Blue13");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_DIFFUSEMAP2),str);
							detailGridElement->addData(/*"OpDT2A"*/"Alpha半透明",str,"Blue13");

							uniform= stateset->getUniform("param1AddDT2a");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddDT2a"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Blue13");

							uniform= stateset->getUniform("param1MultDT2a");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultDT2a"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13");

							uniform = stateset->getUniform("diffuseMat2");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Blue13");
							}
							uniform = stateset->getUniform("diffuseMat2A");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Blue13");
							}

							getFilterModeStr(diffuse2Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Blue13");
							getFilterModeStr(diffuse2Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Blue13");
							getWrapModeStr(diffuse2Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Blue13");
						}
						/////////NT1
						if(normalMap)
						{
							str = normalMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"NormalMap(NT1)"*/"法线1",str,"Red13_200");
							detailGridElement->addData(/*"UseParallax"*/"开启视察",crArgumentParser::appItoa(normalMap->getUseParallax()?1:0),"Red13_200");
							if(normalMap->getUseParallax())
							{
								uniform= stateset->getUniform("parallax");
								float parallax = 0.03f;
                                if(uniform) uniform->get(parallax);
								detailGridElement->addData(/*"parallax"*/"视察参数",crArgumentParser::appFtoa(parallax),"Red13_200");
							}

							uniform= stateset->getUniform("param3AddNT");
							vec3.set(0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3AddNT"*/"加法(3)",crArgumentParser::appVectoa(vec3),"Red13_200");

							uniform= stateset->getUniform("param1MultNT");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultNT"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13_200");

							uniform= stateset->getUniform("param3MultNT");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultNT"*/"乘法(3)",crArgumentParser::appVectoa(vec3),"Red13_200");

							uniform = stateset->getUniform("normalMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13_200");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Red13_200");
							}

							getFilterModeStr(normalMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Red13_200");
							getFilterModeStr(normalMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Red13_200");
							getWrapModeStr(normalMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Red13_200");
						}

						/////NT2
						if(normal2Map)
						{
							str = normal2Map->className();
							str.erase(0,2);
							detailGridElement->addData(/*"NT2"*/"法线2",str,"Green13_200");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_NORMALMAP2),str);
							detailGridElement->addData(/*"OpNT2"*/"通道函数",str,"Green13_200");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_NORMALMAP2)),"Green13_200");

							uniform= stateset->getUniform("param3AddNT2");
							vec3.set(0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3AddNT2"*/"加法(3)",crArgumentParser::appVectoa(vec3),"Green13_200");

							uniform= stateset->getUniform("param1MultNT2");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultNT2"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13_200");

							uniform= stateset->getUniform("param3MultNT2");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultNT2"*/"乘法(3)",crArgumentParser::appVectoa(vec3),"Green13_200");

							uniform = stateset->getUniform("normalMat2");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13_200");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Green13_200");
							}

							getFilterModeStr(normal2Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Green13_200");
							getFilterModeStr(normal2Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Green13_200");
							getWrapModeStr(normal2Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Green13_200");
						}
						//////NT3
						if(normal3Map)
						{
							str = normal3Map->className();
							str.erase(0,2);
							detailGridElement->addData(/*"NT3"*/"法线3",str,"Blue13_200");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_NORMALMAP3),str);
							detailGridElement->addData(/*"OpNT3"*/"通道函数",str,"Blue13_200");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_NORMALMAP3)),"Blue13_200");

							uniform= stateset->getUniform("param3AddNT3");
							vec3.set(0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3AddNT3"*/"加法(3)",crArgumentParser::appVectoa(vec3),"Blue13_200");

							uniform= stateset->getUniform("param1MultNT3");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultNT3"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13_200");

							uniform= stateset->getUniform("param3MultNT3");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultNT3"*/"乘法(3)",crArgumentParser::appVectoa(vec3),"Blue13_200");

							uniform = stateset->getUniform("normalMat3");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13_200");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Blue13_200");
							}

							getFilterModeStr(normal3Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Blue13_200");
							getFilterModeStr(normal3Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Blue13_200");
							getWrapModeStr(normal3Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Blue13_200");
						}
						/////ST1
						if(specularMap)
						{
							str = specularMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"SpecularMap(ST1)"*/"高光1",str,"Red13_150");
							//detailGridElement->addData("AlphaShininessEnable",crArgumentParser::appItoa(specularMap->getAlphaShininessEnable()?1:0));

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_SPECULARMAP),str);
							detailGridElement->addData(/*"OpST"*/"通道函数",str,"Red13_150");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_SPECULARMAP)),"Red13_150");

							uniform= stateset->getUniform("param4AddST");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddST"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Red13_150");

							uniform= stateset->getUniform("param1MultST");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultST"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13_150");

							uniform= stateset->getUniform("param4MultST");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultST"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Red13_150");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_SPECULARMAP),str);
							detailGridElement->addData(/*"OpSTA"*/"Alpha高光指数",str,"Red13_150");

							uniform= stateset->getUniform("param1AddSTa");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddSTa"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Red13_150");

							uniform= stateset->getUniform("param1MultSTa");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultSTa"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13_150");

							uniform = stateset->getUniform("specularMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13_150");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Red13_150");
							}
							uniform = stateset->getUniform("specularMatA");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13_150");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Red13_150");
							}

							getFilterModeStr(specularMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Red13_150");
							getFilterModeStr(specularMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Red13_150");
							getWrapModeStr(specularMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Red13_150");
						}
						///////ST2
						if(specular2Map)
						{
							str = specular2Map->className();
							str.erase(0,2);
							detailGridElement->addData(/*"ST2"*/"高光2",str,"Green13_150");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_SPECULARMAP2),str);
							detailGridElement->addData(/*"OpST2"*/"通道函数",str,"Green13_150");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_SPECULARMAP2)),"Green13_150");

							uniform= stateset->getUniform("param4AddST2");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddST2"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Green13_150");

							uniform= stateset->getUniform("param1MultST2");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultST2"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13_150");

							uniform= stateset->getUniform("param4MultST2");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultST2"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Green13_150");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_SPECULARMAP2),str);
							detailGridElement->addData(/*"OpST2A"*/"Alpha高光指数",str,"Green13_150");

							uniform= stateset->getUniform("param1AddST2a");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddST2a"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Green13_150");

							uniform= stateset->getUniform("param1MultST2a");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultST2a"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13_150");

							uniform = stateset->getUniform("specularMat2");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13_150");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Green13_150");
							}
							uniform = stateset->getUniform("specularMat2A");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13_150");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Green13_150");
							}

							getFilterModeStr(specular2Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Green13_150");
							getFilterModeStr(specular2Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Green13_150");
							getWrapModeStr(specular2Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Green13_150");
						}
						///////ST3
						if(specular3Map)
						{
							str = specular3Map->className();
							str.erase(0,2);
							detailGridElement->addData(/*"ST3"*/"高光3",str,"Blue13_150");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_SPECULARMAP3),str);
							detailGridElement->addData(/*"OpST3"*/"通道函数",str,"Blue13_150");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_SPECULARMAP3)),"Blue13_150");

							uniform= stateset->getUniform("param4AddST3");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddST3"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Blue13_150");

							uniform= stateset->getUniform("param1MultST3");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultST3"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13_150");

							uniform= stateset->getUniform("param4MultST3");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultST3"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Blue13_150");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_SPECULARMAP3),str);
							detailGridElement->addData(/*"OpST3A"*/"Alpha高光指数",str,"Blue13_150");

							uniform= stateset->getUniform("param1AddST3a");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddST3a"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Blue13_150");

							uniform= stateset->getUniform("param1MultST3a");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultST3a"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13_150");

							uniform = stateset->getUniform("specularMat3");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13_150");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Blue13_150");
							}
							uniform = stateset->getUniform("specularMat3A");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13_150");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Blue13_150");
							}

							getFilterModeStr(specular3Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Blue13_150");
							getFilterModeStr(specular3Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Blue13_150");
							getWrapModeStr(specular3Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Blue13_150");
						}
						//////ET1|NT4|扭曲
						if(emissiveMap)
						{
							str = emissiveMap->className();
							str.erase(0,2);
							detailGridElement->addData("ET1|NT4|A扭曲",str,"Red13_100");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_EMISSIVEMAP),str);
							detailGridElement->addData(/*"OpET"*/"通道函数",str,"Red13_100");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_EMISSIVEMAP)),"Red13_100");

							uniform= stateset->getUniform("param4AddET");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddET"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Red13_100");

							uniform= stateset->getUniform("param1MultET");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultET"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13_100");

							uniform= stateset->getUniform("param4MultET");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultET"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Red13_100");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_EMISSIVEMAP),str);
							detailGridElement->addData(/*"OpETA"*/"Alpha扭曲",str,"Red13_100");

							uniform= stateset->getUniform("param1AddETa");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddETa"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Red13_100");

							uniform= stateset->getUniform("param1MultETa");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultETa"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Red13_100");

							uniform = stateset->getUniform("emissiveMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13_100");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Red13_100");
							}
							uniform = stateset->getUniform("emissiveMatA");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Red13_100");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Red13_100");
							}

							getFilterModeStr(emissiveMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Red13_100");
							getFilterModeStr(emissiveMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Red13_100");
							getWrapModeStr(emissiveMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Red13_100");
						}
						/////ET2|DT4|扭曲
						if(emissive2Map)
						{
							str = emissive2Map->className();
							str.erase(0,2);
							detailGridElement->addData("ET2|DT4|A扭曲",str,"Green13_100");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_EMISSIVEMAP2),str);
							detailGridElement->addData(/*"OpET2"*/"通道函数",str,"Green13_100");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_EMISSIVEMAP2)),"Green13_100");

							uniform= stateset->getUniform("param4AddET2");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddET2"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Green13_100");

							uniform= stateset->getUniform("param1MultET2");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultET2"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13_100");

							uniform= stateset->getUniform("param4MultET2");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultET2"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Green13_100");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_EMISSIVEMAP2),str);
							detailGridElement->addData(/*"OpET2A"*/"Alpha扭曲",str,"Green13_100");

							uniform= stateset->getUniform("param1AddET2a");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddET2a"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Green13_100");

							uniform= stateset->getUniform("param1MultET2a");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultET2a"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Green13_100");

							uniform = stateset->getUniform("emissiveMat2");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13_100");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Green13_100");
							}
							uniform = stateset->getUniform("emissiveMat2A");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Green13_100");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Green13_100");
							}

							getFilterModeStr(emissive2Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Green13_100");
							getFilterModeStr(emissive2Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Green13_100");
							getWrapModeStr(emissive2Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Green13_100");
						}
						/////ET3|ST4|扭曲
						if(emissive3Map)
						{
							str = emissive3Map->className();
							str.erase(0,2);
							detailGridElement->addData("ET3|ST4|A扭曲",str,"Blue13_100");

							getTextureOperateStr(stateset->getTextureOperate(TEXTURE_EMISSIVEMAP3),str);
							detailGridElement->addData(/*"OpET3"*/"通道函数",str,"Blue13_100");
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_EMISSIVEMAP3)),"Blue13_100");

							uniform= stateset->getUniform("param4AddET3");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddET3"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Blue13_100");

							uniform= stateset->getUniform("param1MultET3");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultET3"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13_100");

							uniform= stateset->getUniform("param4MultET3");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultET3"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Blue13_100");

							getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_EMISSIVEMAP3),str);
							detailGridElement->addData(/*"OpET3A"*/"Alpha扭曲",str,"Blue13_100");

							uniform= stateset->getUniform("param1AddET3a");
							flt1 = 0.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1AddET3a"*/"加法(1)",crArgumentParser::appFtoa(flt1),"Blue13_100");

							uniform= stateset->getUniform("param1MultET3a");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultET3a"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Blue13_100");

							uniform = stateset->getUniform("emissiveMat3");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13_100");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Blue13_100");
							}
							uniform = stateset->getUniform("emissiveMat3A");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("Alpha缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Blue13_100");
							}
							else
							{
								detailGridElement->addData("Alpha缩放","1.0,1.0","Blue13_100");
							}

							getFilterModeStr(emissive3Map->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Blue13_100");
							getFilterModeStr(emissive3Map->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Blue13_100");
							getWrapModeStr(emissive3Map->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Blue13_100");
						}
						/////////////////ExternTexture
						////UVS
						if(uvsMap)
						{
							str = uvsMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"UVSMap"*/"UV扰动贴图",str,"Yellow13");

							//getTextureOperateStr(stateset->getTextureOperate(TEXTURE_UVSCRAMBLER),str);
							//detailGridElement->addData("OpDT3",str);
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_UVSCRAMBLER)),"Yellow13");

							uniform= stateset->getUniform("param4AddUVS");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddUVS"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Yellow13");

							uniform= stateset->getUniform("param1MultUVS");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultUVS"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Yellow13");

							uniform= stateset->getUniform("param4MultUVS");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultUVS"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Yellow13");

							//getAlphaOperateStr(stateset->getAlphaOperate(TEXTURE_DIFFUSEMAP3),str);
							//detailGridElement->addData("OpDT3A",str);

							//uniform= stateset->getUniform("param1AddDT3a");
							//flt1 = 0.0f;
							//if(uniform) uniform->get(flt1);
							//detailGridElement->addData("param1AddDT3a",crArgumentParser::appFtoa(flt1));

							//uniform= stateset->getUniform("param1MultDT3a");
							//flt1 = 1.0f;
							//if(uniform) uniform->get(flt1);
							//detailGridElement->addData("param1MultDT3a",crArgumentParser::appFtoa(flt1));

							uniform = stateset->getUniform("UVSMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Yellow13");
							}

							getFilterModeStr(uvsMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Yellow13");
							getFilterModeStr(uvsMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Yellow13");
							getWrapModeStr(uvsMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Yellow13");
						}
						//////CW
						if(cwMap)
						{
							str = cwMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"ColorWeight"*/"通道控制贴图",str,"Yellow13");

							//getTextureOperateStr(stateset->getTextureOperate(TEXTURE_COLORWEIGHT),str);
							//detailGridElement->addData("OpDT3",str);
							detailGridElement->addData("UV层",crArgumentParser::appItoa(stateset->getTextureUV(TEXTURE_COLORWEIGHT)),"Yellow13");

							uniform= stateset->getUniform("param4AddCW");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddCW"*/"加法(4)",crArgumentParser::appVectoa(vec4),"Yellow13");

							uniform= stateset->getUniform("param1MultCW");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultCW"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"Yellow13");

							uniform= stateset->getUniform("param4MultCW");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultCW"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"Yellow13");

							uniform= stateset->getUniform("param3MultCWN");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWN"*/"法线乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");

							uniform= stateset->getUniform("param3MultCWD");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWD"*/"颜色乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");
							uniform= stateset->getUniform("param3MultCWDa");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWDa"*/"Alpha乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");

							uniform= stateset->getUniform("param3MultCWS");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWS"*/"高光乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");
							uniform= stateset->getUniform("param3MultCWSa");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWSa"*/"高光指数乘(3)",crArgumentParser::appVectoa(vec3),"Yellow13");

							uniform= stateset->getUniform("param3MultCWE");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWE"*/"自发光乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");
							uniform= stateset->getUniform("param3MultCWEa");
							vec3.set(1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec3);
							detailGridElement->addData(/*"param3MultCWEa"*/"扭曲乘法(3)",crArgumentParser::appVectoa(vec3),"Yellow13");

							//bool uvw = true;
							//uniform = stateset->getUniform("uvw");
							//if(uniform) uniform->get(uvw);
							//detailGridElement->addData("UV通道",crArgumentParser::appItoa(uvw?1:0),"Yellow13");

							uniform = stateset->getUniform("CWMat");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("纹理缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("纹理缩放","1.0,1.0","Yellow13");
							}
							uniform = stateset->getUniform("CWMatR");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("R缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("R缩放","1.0,1.0","Yellow13");
							}
							uniform = stateset->getUniform("CWMatG");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("G缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("G缩放","1.0,1.0","Yellow13");
							}
							uniform = stateset->getUniform("CWMatB");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("B缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("B缩放","1.0,1.0","Yellow13");
							}
							uniform = stateset->getUniform("CWMatA");
							if(uniform)
							{
								crMatrixf m4;
								uniform->get(m4);
								crVector3f scale = m4.getScale();
								detailGridElement->addData("A缩放",crArgumentParser::appVectoa(crVector2f(scale[0],scale[1])),"Yellow13");
							}
							else
							{
								detailGridElement->addData("A缩放","1.0,1.0","Yellow13");
							}

							getFilterModeStr(cwMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"Yellow13");
							getFilterModeStr(cwMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"Yellow13");
							getWrapModeStr(cwMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"Yellow13");
						}
						///////////////////////////////////
						//////EnvMap
						if(envMap)
						{
							str = envMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"EnvMap"*/"环境贴图",str,"");

							uniform= stateset->getUniform("param4AddENV");
							vec4.set(0.0f,0.0f,0.0f,0.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4AddENV"*/"加法(4)",crArgumentParser::appVectoa(vec4),"");

							uniform= stateset->getUniform("param1MultENV");
							flt1 = 1.0f;
							if(uniform) uniform->get(flt1);
							detailGridElement->addData(/*"param1MultENV"*/"乘法(1)",crArgumentParser::appFtoa(flt1),"");

							uniform= stateset->getUniform("param4MultENV");
							vec4.set(1.0f,1.0f,1.0f,1.0f);
							if(uniform) uniform->get(vec4);
							detailGridElement->addData(/*"param4MultENV"*/"乘法(4)",crArgumentParser::appVectoa(vec4),"");

							getFilterModeStr(envMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"");
							getFilterModeStr(envMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"");
							getWrapModeStr(envMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"");
						}
						//////lightMap
						if(lightMap)
						{
							str = lightMap->className();
							str.erase(0,2);
							detailGridElement->addData(/*"LightMap"*/"烘焙阴影",str,"");
							getFilterModeStr(lightMap->getFilter(crTexture::MIN_FILTER),str);
							detailGridElement->addData("MIN_FILTER",str,"");
							getFilterModeStr(lightMap->getFilter(crTexture::MAG_FILTER),str);
							detailGridElement->addData("MAG_FILTER",str,"");
							getWrapModeStr(lightMap->getWrap(crTexture::WRAP_S),str);
							detailGridElement->addData("WRAP_MODE",str,"");
						}
						////////////////////////////
						detailGridElement->addData("END","NULL","");
				        detailGridElement->checkSelect();
						detailGridElement->updateInputElement(true);
					}
				}
			}
		}
        if(clearData)
		{
			crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
			if(currentSelectElement)
			{
				currentSelectElement->clearString();
			}
			crEditBoxElement *baseMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_baseMapElement));
			if(baseMapElement)
			{
                baseMapElement->clearString();
			}
			crEditBoxElement *normalMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normalMapElement));
			if(normalMapElement)
			{
				normalMapElement->clearString();
			}
			crEditBoxElement *lightMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lightMapElement));
			if(lightMapElement)
			{
				lightMapElement->clearString();
			}
			crEditBoxElement *envMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_envMapElement));
			if(envMapElement)
			{
				envMapElement->clearString();
			}
			crEditBoxElement *diffuseElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseElement));
			if(diffuseElement)
			{
				diffuseElement->clearString();
			}
			crEditBoxElement *specularElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularElement));
			if(specularElement)
			{
				specularElement->clearString();
			}
			crEditBoxElement *emissiveElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveElement));
			if(emissiveElement)
			{
				emissiveElement->clearString();
			}
			crEditBoxElement *normal2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normal2Element));
			if(normal2Element)
			{
				normal2Element->clearString();
			}
			crEditBoxElement *diffuse2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuse2Element));
			if(diffuse2Element)
			{
				diffuse2Element->clearString();
			}
			crEditBoxElement *specular2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specular2Element));
			if(specular2Element)
			{
				specular2Element->clearString();
			}
			crEditBoxElement *emissive2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissive2Element));
			if(emissive2Element)
			{
				emissive2Element->clearString();
			}
			crEditBoxElement *normal3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_normal3Element));
			if(normal3Element)
			{
				normal3Element->clearString();
			}
			crEditBoxElement *specular3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specular3Element));
			if(specular3Element)
			{
				specular3Element->clearString();
			}
			crEditBoxElement *emissive3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissive3Element));
			if(emissive3Element)
			{
				emissive3Element->clearString();
			}
			crEditBoxElement *uvsElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_uvsElement));
			if(uvsElement)
			{
				uvsElement->clearString();
			}
			crEditBoxElement *colorWeightElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_colorWeightElement));
			if(colorWeightElement)
			{
				colorWeightElement->clearString();
			}
			/////////////////
			crVector4 ambientColor,diffuseColor,specularColor,emissiveColor;
			float shininess = 0.0f;
			//m_ambientColorElement
			crEditBoxElement *ambientColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorRElement));
			if(ambientColorRElement)
			{
				ambientColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[0] * 255.0f)));
			}
			crEditBoxElement *ambientColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorGElement));
			if(ambientColorGElement)
			{
				ambientColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[1] * 255.0f)));
			}
			crEditBoxElement *ambientColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientColorBElement));
			if(ambientColorBElement)
			{
				ambientColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientColor[2] * 255.0f)));
			}
			//diffuseColorElement
			crEditBoxElement *diffuseColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorRElement));
			if(diffuseColorRElement)
			{
				diffuseColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[0] * 255.0f)));
			}
			crEditBoxElement *diffuseColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorGElement));
			if(diffuseColorGElement)
			{
				diffuseColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[1] * 255.0f)));
			}
			crEditBoxElement *diffuseColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_diffuseColorBElement));
			if(diffuseColorBElement)
			{
				diffuseColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(diffuseColor[2] * 255.0f)));
			}
			//specularColorElement
			crEditBoxElement *specularColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorRElement));
			if(specularColorRElement)
			{
				specularColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[0] * 255.0f)));
			}
			crEditBoxElement *specularColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorGElement));
			if(specularColorGElement)
			{
				specularColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[1] * 255.0f)));
			}
			crEditBoxElement *specularColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_specularColorBElement));
			if(specularColorBElement)
			{
				specularColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(specularColor[2] * 255.0f)));
			}
			crEditBoxElement *shininessElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_shininessElement));
			if(shininessElement)
			{
				shininessElement->setStringArrayByString(crArgumentParser::appFtoa(shininess));
			}
			//emissiveColorElement
			crEditBoxElement *emissiveColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorRElement));
			if(emissiveColorRElement)
			{
				emissiveColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[0] * 255.0f)));
			}
			crEditBoxElement *emissiveColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorGElement));
			if(emissiveColorGElement)
			{
				emissiveColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[1] * 255.0f)));
			}
			crEditBoxElement *emissiveColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_emissiveColorBElement));
			if(emissiveColorBElement)
			{
				emissiveColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(emissiveColor[2] * 255.0f)));
			}

			crKeyValueGridElement *detailGridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_detailGridElement));
			if(detailGridElement)
			{
				detailGridElement->clearData();
				detailGridElement->checkSelect();
			}
		}
	}
}
/////////////////////////////////////////
//
//crFocusTextureAttrMethod
//
/////////////////////////////////////////
crFocusTextureAttrMethod::crFocusTextureAttrMethod(){}
crFocusTextureAttrMethod::crFocusTextureAttrMethod(const crFocusTextureAttrMethod& handle):
crMethod(handle),
m_detailGridElement(handle.m_detailGridElement),
m_texTypeName(handle.m_texTypeName)
{
}
void crFocusTextureAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crFocusTextureAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_detailGridElement = str;
		break;
	case 1:
		m_texTypeName = str;
		break;
	}
}

void crFocusTextureAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
        crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *detailGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_detailGridElement));
		if(detailGridElement)
		{
			int id = detailGridElement->findDataIndex(m_texTypeName);
			if(id>=0) 
			{
                detailGridElement->select(detailGridElement->getDataSize()-1);
				detailGridElement->select(id);
			}
		}
	}
}
/////////////////////////////////////////
//
//crGetImageNameMethod
//
/////////////////////////////////////////
crGetImageNameMethod::crGetImageNameMethod():
m_isEnvMap(false),
m_filterIndex(5){}
crGetImageNameMethod::crGetImageNameMethod(const crGetImageNameMethod& handle):
crMethod(handle),
m_filePath(handle.m_filePath),
m_isEnvMap(handle.m_isEnvMap),
m_filterIndex(handle.m_filterIndex)
{
}
void crGetImageNameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editBox = (CRGUI::crEditBoxElement*)param;
}

void crGetImageNameMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePath = str;
		break;
	case 1:
		m_isEnvMap = atoi(str.c_str());
		break;
	case 2:
		m_filterIndex = atoi(str.c_str());
		break;
	}
}

void crGetImageNameMethod::operator()(crHandle &handle)
{
	if(m_editBox.valid())
	{
		char programDir[256];
		GetCurrentDirectory(256,programDir);
		//std::string dir = programDir;
		//dir = dir + "/" + m_filePath;
		//SetCurrentDirectory(dir.c_str());

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[256];       // buffer for file name

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ZeroMemory(szFile, sizeof(szFile));
		//sprintf(szFile,"%s\0",dir.c_str());
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = crStartHandler::getInstance()->getWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Img(.img)\0*.img\0tga(.tga)\0*.tga\0png(.png)\0*.png\0dds(.dds)\0*.dds\0All(*.*)\0*.*\0";
		ofn.nFilterIndex = m_filterIndex;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = m_filePath.c_str();
		ofn.Flags = OFN_EXPLORER;

		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn)==TRUE) 
		{
			std::string filename = ofn.lpstrFile;
			filename = crArgumentParser::getSimpleFileName(filename);
			if(m_isEnvMap)
			{
		        std::string::size_type envpos = filename.find_last_of('_');
				std::string::size_type dot = filename.find_last_of('.');
				if (envpos!=std::string::npos && dot!=std::string::npos)
				{
					std::string envcode = std::string(filename.begin()+envpos+1,filename.begin()+dot);
					if( envcode == "posx" || envcode == "posy" || envcode == "posz" ||
						envcode == "negx" || envcode == "negy" || envcode == "negz" )
					{
						filename.erase(envpos,dot-envpos);
						m_editBox->setStringArrayByString(filename);
					}
				}
			}
			else
			{
			    m_editBox->setStringArrayByString(filename);
			}
		}
		SetCurrentDirectory(programDir);
	}
}

/////////////////////////////////////////
//
//crModifyMaterialMethod
//
/////////////////////////////////////////
crModifyMaterialMethod::crModifyMaterialMethod(){}
crModifyMaterialMethod::crModifyMaterialMethod(const crModifyMaterialMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_baseMapElement(handle.m_baseMapElement),
m_normalMapElement(handle.m_normalMapElement),
m_lightMapElement(handle.m_lightMapElement),
m_envMapElement(handle.m_envMapElement),
m_diffuseElement(handle.m_diffuseElement),
m_specularElement(handle.m_specularElement),
m_emissiveElement(handle.m_emissiveElement),
m_normal2Element(handle.m_normal2Element),
m_diffuse2Element(handle.m_diffuse2Element),
m_specular2Element(handle.m_specular2Element),
m_emissive2Element(handle.m_emissive2Element),
m_normal3Element(handle.m_normal3Element),
m_specular3Element(handle.m_specular3Element),
m_emissive3Element(handle.m_emissive3Element),
m_uvsElement(handle.m_uvsElement),
m_colorWeightElement(handle.m_colorWeightElement),
m_ambientColorElement(handle.m_ambientColorElement),
m_ambientColorRElement(handle.m_ambientColorRElement),
m_ambientColorGElement(handle.m_ambientColorGElement),
m_ambientColorBElement(handle.m_ambientColorBElement),
m_diffuseColorElement(handle.m_diffuseColorElement),
m_diffuseColorRElement(handle.m_diffuseColorRElement),
m_diffuseColorGElement(handle.m_diffuseColorGElement),
m_diffuseColorBElement(handle.m_diffuseColorBElement),
m_specularColorElement(handle.m_specularColorElement),
m_specularColorRElement(handle.m_specularColorRElement),
m_specularColorGElement(handle.m_specularColorGElement),
m_specularColorBElement(handle.m_specularColorBElement),
m_shininessElement(handle.m_shininessElement),
m_emissiveColorElement(handle.m_emissiveColorElement),
m_emissiveColorRElement(handle.m_emissiveColorRElement),
m_emissiveColorGElement(handle.m_emissiveColorGElement),
m_emissiveColorBElement(handle.m_emissiveColorBElement),
m_detailGridElement(handle.m_detailGridElement)
{
}
void crModifyMaterialMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyMaterialMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_baseMapElement = str;
		break;
	case 2:
		m_normalMapElement = str;
		break;
	case 3:
		m_lightMapElement = str;
		break;
	case 4:
		m_envMapElement = str;
		break;
	case 5:
		m_diffuseElement = str;
		break;
	case 6:
		m_specularElement = str;
		break;
	case 7:
		m_emissiveElement = str;
		break;
	case 8:
		m_normal2Element = str;
		break;
	case 9:
		m_diffuse2Element = str;
		break;
	case 10:
		m_specular2Element = str;
		break;
	case 11:
		m_emissive2Element = str;
		break;
	case 12:
		m_normal3Element = str;
		break;
	case 13:
		m_specular3Element = str;
		break;
	case 14:
		m_emissive3Element = str;
		break;
	case 15:
		m_uvsElement = str;
		break;
	case 16:
		m_colorWeightElement = str;
		break;
	case 17:
		m_ambientColorElement = str;
		break;
	case 18:
		m_ambientColorRElement = str;
		break;
	case 19:
		m_ambientColorGElement = str;
		break;
	case 20:
		m_ambientColorBElement = str;
		break;
	case 21:
		m_diffuseColorElement = str;
		break;
	case 22:
		m_diffuseColorRElement = str;
		break;
	case 23:
		m_diffuseColorGElement = str;
		break;
	case 24:
		m_diffuseColorBElement = str;
		break;
	case 25:
		m_specularColorElement = str;
		break;
	case 26:
		m_specularColorRElement = str;
		break;
	case 27:
		m_specularColorGElement = str;
		break;
	case 28:
		m_specularColorBElement = str;
		break;
	case 29:
		m_shininessElement = str;
		break;
	case 30:
		m_emissiveColorElement = str;
		break;
	case 31:
		m_emissiveColorRElement = str;
		break;
	case 32:
		m_emissiveColorGElement = str;
		break;
	case 33:
		m_emissiveColorBElement = str;
		break;
	case 34:
		m_detailGridElement = str;
		break;
	}
}
crTexture::FilterMode crModifyMaterialMethod::getFilterModeFromStr(const std::string &str)
{
	if(str.compare("LINEAR") == 0)
		return crTexture::LINEAR;
	else if(str.compare("LINEAR_MIPMAP_LINEAR") == 0)
		return crTexture::LINEAR_MIPMAP_LINEAR;
	else if(str.compare("LINEAR_MIPMAP_NEAREST") == 0)
		return crTexture::LINEAR_MIPMAP_NEAREST;
	else if(str.compare("NEAREST") == 0)
		return crTexture::NEAREST;
	else if(str.compare("NEAREST_MIPMAP_LINEAR") == 0)
		return crTexture::NEAREST_MIPMAP_LINEAR;
	else if(str.compare("NEAREST_MIPMAP_NEAREST") == 0)
		return crTexture::NEAREST_MIPMAP_NEAREST;
	return crTexture::LINEAR_MIPMAP_LINEAR;
}
crTexture::WrapMode crModifyMaterialMethod::getWrapModeFromStr(const std::string &str)
{
	if(str.compare("CLAMP") == 0)
		return crTexture::CLAMP;
	else if(str.compare("CLAMP_TO_EDGE") == 0)
		return crTexture::CLAMP_TO_EDGE;
	else if(str.compare("CLAMP_TO_BORDER") == 0)
		return crTexture::CLAMP_TO_BORDER;
	else if(str.compare("REPEAT") == 0)
		return crTexture::REPEAT;
	else if(str.compare("MIRROR") == 0)
		return crTexture::MIRROR;
	return crTexture::REPEAT;
}
int crModifyMaterialMethod::getTextureOperateFromStr(const std::string &str)
{
	if(str.compare("Add") == 0)
		return 0;
	else if(str.compare("Mult") == 0)
		return 1;
	else if(str.compare("Replace") == 0)
		return 2;
	else if(str.compare("Ignore") == 0)
		return 3;
	else if(str.compare("AddETTo") == 0)
		return 4;
	else if(str.compare("MultETTo") == 0)
		return 5;
	else if(str.compare("ET3ToLM") == 0)
		return 6;
	return 0;
}
int crModifyMaterialMethod::getAlphaOperateFromStr(const std::string &str)
{
	if(str.compare("Ignore") == 0)
		return 0;
	else if(str.compare("Add") == 0)
		return 1;
	else if(str.compare("Mult") == 0)
		return 2;
	else if(str.compare("Replace") == 0)
		return 3;
	return 0;
}
int crModifyMaterialMethod::getDetailBin(const std::string &str)
{
	if(str.compare("Trans") == 0)
		return crStateSet::TRANSPARENT_BIN;
	else if(str.compare("PreTrans") == 0)
		return crStateSet::PRETRANS_BIN;
	else if(str.compare("PostTrans") == 0)
		return crStateSet::POSTTRANS_BIN;
	else if(str.compare("FXTrans") == 0)
		return crStateSet::FX_BIN;
	return crStateSet::TRANSPARENT_BIN;
}
void crModifyMaterialMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);

		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
			if(currentSelectObject)
			{
				crData *infoData = bodyInfo->getDataClass();
				assert(infoData);
				void *_selectDrawable;
				crDrawable* selectDrawable = NULL;
				infoData->getParam(7,_selectDrawable);
				if(_selectDrawable)
				{
					selectDrawable = (crDrawable*)_selectDrawable;
                    ref_ptr<crStateSet> origStateSet = selectDrawable->getStateSet();
					if(!origStateSet)
						origStateSet = currentSelectObject->getStateSet();
					//if(!origStateSet)
					//	origStateSet = new crStateSet;
					std::string str;
					crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_currentSelectElement));
					if(currentSelectElement)
					{

					}
					ref_ptr<crStateSet> newStateSet = new crStateSet;
					crUniform *uniform;
					float flt1;
					crVector2 vec2;
					crVector3 vec3;
					crVector4 vec4;
                    crVector3 mo3(1.0f,1.0f,1.0f);
					crVector3 ao3(0.0f,0.0f,0.0f);
					crVector4 mo4(1.0f,1.0f,1.0f,1.0f);
					crVector4 ao4(0.0f,0.0f,0.0f,0.0f);
					crKeyValueGridElement *detailGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_detailGridElement));
				    ref_ptr<crKeyValueNode> keyValueNode;
					int gridIndex = 0;
                    if(detailGridElement)
					{
						if(origStateSet.valid())
						{
							//双面显示
							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							if(atoi(str.c_str()) == 1)
							{
								newStateSet->setMode(GL_CULL_FACE,CRCore::crStateAttribute::OFF);
							}
							//颜色
							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							if(str.compare("顶点色")==0)
							{
								crGeometry *geometry = dynamic_cast<crGeometry *>(selectDrawable);
								if(geometry && geometry->getColorBinding() == crGeometry::BIND_OFF)
								{
									geometry->setColorBinding(crGeometry::BIND_PER_VERTEX);
									geometry->computeCorrectBindingsAndArraySizes(geometry->getColorData(),"ColorData");
								}
							}
							else
							{
								crVector4 color;
								crArgumentParser::appAtoVec(str,color);
								color /= 255.0f;
								selectDrawable->setColor(color);
								crGeometry *geometry = dynamic_cast<crGeometry *>(selectDrawable);
								if(geometry)
								{
									geometry->setColorBinding(crGeometry::BIND_OFF);
								}
							}

							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							float maskClipValue = atof(str.c_str());
						
							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							if(str.compare("Trans") == 0)
							{
								//if(maskClipValue>0.0f)
								//{
								//	crAlphaFunc *alphaFunc = new crAlphaFunc;
								//	alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
								//	newStateSet->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
								//}
								crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE_MINUS_SRC_ALPHA);
								newStateSet->setAttributeAndModes(blendFunc,crStateAttribute::ON);
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setRenderingHint((crStateSet::RenderingHint)getDetailBin(str));
							}
							else if(str.compare("Add") == 0)
							{
								crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::ONE,crBlendFunc::ONE);
								newStateSet->setAttributeAndModes(blendFunc,crStateAttribute::ON);
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setRenderingHint((crStateSet::RenderingHint)getDetailBin(str));
							}
							else if(str.compare("Modulate") == 0)
							{
								//if(maskClipValue>0.0f)
								//{
								//	crAlphaFunc *alphaFunc = new crAlphaFunc;
								//	alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
								//	newStateSet->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
								//}
								crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE);
								newStateSet->setAttributeAndModes(blendFunc,crStateAttribute::ON);
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setRenderingHint((crStateSet::RenderingHint)getDetailBin(str));
							}
							else if(str.compare("Mask") == 0)
							{
								if(maskClipValue == 0.0f)
									maskClipValue = 0.3333f;
								crAlphaFunc *alphaFunc = new crAlphaFunc;
								alphaFunc->setFunction(crAlphaFunc::GREATER,maskClipValue);
								newStateSet->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
								newStateSet->setRenderingHint(CRCore::crStateSet::ALPHATEST_BIN);
								gridIndex++;
							}
							else
							{
								gridIndex++;
							}

							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							if(atoi(str.c_str()) == 1)
							{
								newStateSet->setMode(GL_DEPTH_TEST,CRCore::crStateAttribute::OFF);
								//newStateSet->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
							}
							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							crDepth *origDepth = dynamic_cast<crDepth *>(origStateSet->getAttribute(crStateAttribute::DEPTH));
							if(str.compare("继承")!=0)
							{
								crDepth* depth = new crDepth;
								if(str.compare("NEVER") == 0)
								{
									depth->setFunction(crDepth::NEVER);
								}
								else if(str.compare("默认") == 0)
								{
									depth->setFunction(crDepth::LESS);
								}
								else if(str.compare("EQUAL") == 0)
								{
									depth->setFunction(crDepth::EQUAL);
								}
								else if(str.compare("LEQUAL") == 0)
								{
									depth->setFunction(crDepth::LEQUAL);
								}
								else if(str.compare("GREATER") == 0)
								{
									depth->setFunction(crDepth::GREATER);
								}
								else if(str.compare("NOTEQUAL") == 0)
								{
									depth->setFunction(crDepth::NOTEQUAL);
								}
								else if(str.compare("GEQUAL") == 0)
								{
									depth->setFunction(crDepth::GEQUAL);
								}
								else if(str.compare("ALWAYS") == 0)
								{
									depth->setFunction(crDepth::ALWAYS);
								}
								if(origDepth)
								{
									keyValueNode = detailGridElement->getData(gridIndex++);
									str = keyValueNode->getValue();//dataPair.second;
									crVector2f zvalue;
									crArgumentParser::appAtoVec(str,zvalue);
									depth->setRange(zvalue[0],zvalue[1]);
									keyValueNode = detailGridElement->getData(gridIndex++);
									str = keyValueNode->getValue();//dataPair.second;
									bool depthWrite = (bool)(atoi(str.c_str()));
									depth->setWriteMask(depthWrite);
								}
								newStateSet->setAttribute(depth,crStateAttribute::ON);
							}
							else if(origDepth)
							{
								gridIndex += 2;
							}
					
							keyValueNode = detailGridElement->getData(gridIndex++);
							str = keyValueNode->getValue();//dataPair.second;
							if(str.compare("Point") == 0)
							{
								crPolygonMode* polygonMode = new crPolygonMode;
								polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::POINT);
								newStateSet->setAttributeAndModes(polygonMode,crStateAttribute::ON);
							}
							else if(str.compare("Line") == 0)
							{
								crPolygonMode* polygonMode = new crPolygonMode;
								polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
								newStateSet->setAttributeAndModes(polygonMode,crStateAttribute::ON);
							}
						}
					}
				    crTexture *origMap;
					///////DT1
					crEditBoxElement *baseMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_baseMapElement));
					if(baseMapElement)
					{
						str = baseMapElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *baseMap = new crTexture2D;
							baseMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_BASEMAP,baseMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("BaseMap",str);
								gridIndex++;
								//detailGridElement->addData("AlphaShadowEnable",crArgumentParser::appItoa(baseMap->getAlphaShadowEnable()?1:0));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								baseMap->setAlphaShadowEnable(atoi(str.c_str()));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								baseMap->setBlendShadowEnable(atoi(str.c_str()));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureOperate(TEXTURE_BASEMAP,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_BASEMAP,atoi(str.c_str()));

								//detailGridElement->addData("param4AddBT",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddBT",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultBT",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultBT",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultBT",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultBT",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setAlphaOperate(TEXTURE_BASEMAP,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddBTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultBTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("baseMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("baseMatA",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								baseMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								baseMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								baseMap->setWrap(crTexture::WRAP_S,wrap);
								baseMap->setWrap(crTexture::WRAP_T,wrap);
								baseMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(baseMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setAlphaShadowEnable(baseMap->getAlphaShadowEnable());
									loadedTex->setBlendShadowEnable(baseMap->getBlendShadowEnable());
									loadedTex->setFilter(crTexture::MIN_FILTER,baseMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,baseMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,baseMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,baseMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,baseMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
                            if(detailGridElement && origMap)
							{
                                gridIndex+=16;
							}
						}
					}
					///////DT2
					crEditBoxElement *diffuseElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseElement));
					if(diffuseElement)
					{
						str = diffuseElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *diffuseMap = new crTexture2D;
							diffuseMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP,diffuseMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("DiffuseMap",str);
								gridIndex++;

								//detailGridElement->addData("OpDT",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureOperate(TEXTURE_DIFFUSEMAP,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_DIFFUSEMAP,atoi(str.c_str()));

								//detailGridElement->addData("param4AddDT",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddDT",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultDT",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultDT",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultDT",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultDT",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setAlphaOperate(TEXTURE_DIFFUSEMAP,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddDTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultDTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("diffuseMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("diffuseMatA",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								diffuseMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								diffuseMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								diffuseMap->setWrap(crTexture::WRAP_S,wrap);
								diffuseMap->setWrap(crTexture::WRAP_T,wrap);
								diffuseMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(diffuseMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,diffuseMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,diffuseMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,diffuseMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,diffuseMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,diffuseMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					///////DT3
					crEditBoxElement *diffuse2Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuse2Element));
					if(diffuse2Element)
					{
						str = diffuse2Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *diffuse2Map = new crTexture2D;
							diffuse2Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_DIFFUSEMAP2,diffuse2Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("DiffuseMap2",str);
								gridIndex++;
								//detailGridElement->addData("OpDT2",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureOperate(TEXTURE_DIFFUSEMAP2,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_DIFFUSEMAP2,atoi(str.c_str()));

								//detailGridElement->addData("param4AddDT2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddDT2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultDT2",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultDT2",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultDT2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultDT2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddDT2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setAlphaOperate(TEXTURE_DIFFUSEMAP2,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultDT2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("diffuseMat2",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("diffuseMat2A",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								diffuse2Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								diffuse2Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								diffuse2Map->setWrap(crTexture::WRAP_S,wrap);
								diffuse2Map->setWrap(crTexture::WRAP_T,wrap);
								diffuse2Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(diffuse2Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,diffuse2Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,diffuse2Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,diffuse2Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,diffuse2Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,diffuse2Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					///////NT1
					crEditBoxElement *normalMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_normalMapElement));
					if(normalMapElement)
					{
						str = normalMapElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *normalMap = new crTexture2D;
							normalMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_BUMPMAP,normalMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("NormalMap",str);
								gridIndex++;
								//detailGridElement->addData("UseParallax",crArgumentParser::appItoa(normalMap->getUseParallax()?1:0));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normalMap->setUseParallax(atoi(str.c_str()));
								if(origMap->getUseParallax())
								{
									keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
									if(normalMap->getUseParallax())
									{
										str = keyValueNode->getValue();//dataPair.second;
										uniform = newStateSet->getOrCreateUniform("parallax",CRCore::crUniform::FLOAT);
										uniform->setDataVariance(crBase::STATIC);
										float parallax = atof(str.c_str());
										uniform->set(parallax);
									}
								}
								//detailGridElement->addData("param3AddNT",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != ao3)
								{
									uniform = newStateSet->getOrCreateUniform("param3AddNT",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param1MultNT",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultNT",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param3MultNT",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultNT",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("normalMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normalMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normalMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								normalMap->setWrap(crTexture::WRAP_S,wrap);
								normalMap->setWrap(crTexture::WRAP_T,wrap);
								normalMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(normalMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setUseParallax(normalMap->getUseParallax());
									loadedTex->setFilter(crTexture::MIN_FILTER,normalMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,normalMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,normalMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,normalMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,normalMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=9;
								if(origMap->getUseParallax())
									gridIndex+=1;
							}
						}
					}

					//////////NT2
					crEditBoxElement *normal2Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_normal2Element));
					if(normal2Element)
					{
						str = normal2Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *normal2Map = new crTexture2D;
							normal2Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_NORMALMAP2,normal2Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("NormalMap2",str);
								gridIndex++;
								//detailGridElement->addData("OpNT2",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_NORMALMAP2,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_NORMALMAP2,atoi(str.c_str()));

								//detailGridElement->addData("param3AddNT2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != ao3)
								{
									uniform = newStateSet->getOrCreateUniform("param3AddNT2",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param1MultNT2",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultNT2",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param3MultNT2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultNT2",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("normalMat2",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normal2Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normal2Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								normal2Map->setWrap(crTexture::WRAP_S,wrap);
								normal2Map->setWrap(crTexture::WRAP_T,wrap);
								normal2Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(normal2Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,normal2Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,normal2Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,normal2Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,normal2Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,normal2Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=10;
							}
						}
					}
					////////NT3
					crEditBoxElement *normal3Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_normal3Element));
					if(normal3Element)
					{
						str = normal3Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *normal3Map = new crTexture2D;
							normal3Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_NORMALMAP3,normal3Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("NormalMap3",str);
								gridIndex++;
								//detailGridElement->addData("OpNT3",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_NORMALMAP3,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_NORMALMAP3,atoi(str.c_str()));

								//detailGridElement->addData("param3AddNT3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != ao3)
								{
									uniform = newStateSet->getOrCreateUniform("param3AddNT3",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param1MultNT3",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultNT3",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param3MultNT3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultNT3",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("normalMat3",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normal3Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								normal3Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								normal3Map->setWrap(crTexture::WRAP_S,wrap);
								normal3Map->setWrap(crTexture::WRAP_T,wrap);
								normal3Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(normal3Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,normal3Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,normal3Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,normal3Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,normal3Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,normal3Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=10;
							}
						}
					}
					////////ST1
					crEditBoxElement *specularElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularElement));
					if(specularElement)
					{
						str = specularElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *specularMap = new crTexture2D;
							specularMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_SPECULARMAP,specularMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("SpecularMap",str);
								gridIndex++;
								//str = detailGridElement->getData(gridIndex++).second;
								//specularMap->setAlphaShininessEnable(atoi(str.c_str()));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_SPECULARMAP,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_SPECULARMAP,atoi(str.c_str()));

								//detailGridElement->addData("param4AddST",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddST",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultST",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultST",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultST",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultST",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_SPECULARMAP,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddSTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultSTa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMatA",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specularMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specularMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								specularMap->setWrap(crTexture::WRAP_S,wrap);
								specularMap->setWrap(crTexture::WRAP_T,wrap);
								specularMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(specularMap->getImageNameID());
								if(loadedTex)
								{
									//loadedTex->setAlphaShininessEnable(specularMap->getAlphaShininessEnable());
									loadedTex->setFilter(crTexture::MIN_FILTER,specularMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,specularMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,specularMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,specularMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,specularMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					//////////////ST2
					crEditBoxElement *specular2Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specular2Element));
					if(specular2Element)
					{
						str = specular2Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *specular2Map = new crTexture2D;
							specular2Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_SPECULARMAP2,specular2Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("SpecularMap2",str);
								gridIndex++;
								//detailGridElement->addData("OpST2",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_SPECULARMAP2,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_SPECULARMAP2,atoi(str.c_str()));

								//detailGridElement->addData("param4AddST2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddST2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultST2",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultST2",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultST2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultST2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_SPECULARMAP2,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddST2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultST2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMat2",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMat2A",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specular2Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specular2Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								specular2Map->setWrap(crTexture::WRAP_S,wrap);
								specular2Map->setWrap(crTexture::WRAP_T,wrap);
								specular2Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(specular2Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,specular2Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,specular2Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,specular2Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,specular2Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,specular2Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					//////////ST3
					crEditBoxElement *specular3Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specular3Element));
					if(specular3Element)
					{
						str = specular3Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *specular3Map = new crTexture2D;
							specular3Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_SPECULARMAP3,specular3Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("SpecularMap3",str);
								gridIndex++;
								//detailGridElement->addData("OpST3",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_SPECULARMAP3,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_SPECULARMAP3,atoi(str.c_str()));

								//detailGridElement->addData("param4AddST3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddST3",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultST3",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultST3",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultST3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultST3",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_SPECULARMAP3,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddST3a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultST3a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMat3",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("specularMat3A",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specular3Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								specular3Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								specular3Map->setWrap(crTexture::WRAP_S,wrap);
								specular3Map->setWrap(crTexture::WRAP_T,wrap);
								specular3Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(specular3Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,specular3Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,specular3Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,specular3Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,specular3Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,specular3Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					////////ET1|NT4|alpha扭曲
					crEditBoxElement *emissiveElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissiveElement));
					if(emissiveElement)
					{
						str = emissiveElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *emissiveMap = new crTexture2D;
							emissiveMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP,emissiveMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("EmissiveMap",str);
								gridIndex++;

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_EMISSIVEMAP,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_EMISSIVEMAP,atoi(str.c_str()));

								//detailGridElement->addData("param4AddET",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddET",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultET",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultET",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultET",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultET",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_EMISSIVEMAP,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddETa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultETa",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMatA",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissiveMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissiveMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								emissiveMap->setWrap(crTexture::WRAP_S,wrap);
								emissiveMap->setWrap(crTexture::WRAP_T,wrap);
								emissiveMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(emissiveMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,emissiveMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,emissiveMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,emissiveMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,emissiveMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,emissiveMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					/////////ET2|DT4|alpha扭曲
					crEditBoxElement *emissive2Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissive2Element));
					if(emissive2Element)
					{
						str = emissive2Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP2,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *emissive2Map = new crTexture2D;
							emissive2Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP2,emissive2Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("EmissiveMap2",str);
								gridIndex++;
								//detailGridElement->addData("OpET2",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_EMISSIVEMAP2,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_EMISSIVEMAP2,atoi(str.c_str()));

								//detailGridElement->addData("param4AddET2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddET2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultET2",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultET2",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultET2",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultET2",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_EMISSIVEMAP2,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddET2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultET2a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMat2",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMat2A",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissive2Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissive2Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								emissive2Map->setWrap(crTexture::WRAP_S,wrap);
								emissive2Map->setWrap(crTexture::WRAP_T,wrap);
								emissive2Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(emissive2Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,emissive2Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,emissive2Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,emissive2Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,emissive2Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,emissive2Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					////////ET3|ST4|alpha扭曲
					crEditBoxElement *emissive3Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissive3Element));
					if(emissive3Element)
					{
						str = emissive3Element->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture *>(origStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP3,crStateAttribute::TEXTURE));
						if(!str.empty())
						{
							crTexture2D *emissive3Map = new crTexture2D;
							emissive3Map->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_EMISSIVEMAP3,emissive3Map,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("EmissiveMap3",str);
								gridIndex++;
								//detailGridElement->addData("OpET3",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setTextureOperate(TEXTURE_EMISSIVEMAP3,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_EMISSIVEMAP3,atoi(str.c_str()));

								//detailGridElement->addData("param4AddET3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddET3",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultET3",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultET3",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultET3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultET3",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								newStateSet->setAlphaOperate(TEXTURE_EMISSIVEMAP3,getAlphaOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 0.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1AddET3a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultET3a",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMat3",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("emissiveMat3A",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissive3Map->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								emissive3Map->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								emissive3Map->setWrap(crTexture::WRAP_S,wrap);
								emissive3Map->setWrap(crTexture::WRAP_T,wrap);
								emissive3Map->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(emissive3Map->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setFilter(crTexture::MIN_FILTER,emissive3Map->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,emissive3Map->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,emissive3Map->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,emissive3Map->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,emissive3Map->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=14;
							}
						}
					}
					//////////////////ExternTexture
					////////UVS
					crEditBoxElement *uvsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_uvsElement));
					if(uvsElement)
					{
						str = uvsElement->getStringArrayInString();
                        origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture2D *>(origStateSet->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
						if(origMap && origMap->getExternTextureID() != crTexture::UVSCRAMBLER)
						{
							origMap = NULL;
						}
						if(!str.empty())
						{
							crTexture2D *uvsMap = new crTexture2D;
							uvsMap->setExternTextureID(crTexture::UVSCRAMBLER);
							uvsMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_UVSCRAMBLER,uvsMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("DiffuseMap3",str);
								gridIndex++;
								//detailGridElement->addData("OpDT3",str);
								//str = detailGridElement->getData(gridIndex++).second;
								//newStateSet->setTextureOperate(TEXTURE_UVSCRAMBLER,getTextureOperateFromStr(str));

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_UVSCRAMBLER,atoi(str.c_str()));

								//detailGridElement->addData("param4AddDT3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddUVS",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultDT3",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultUVS",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultDT3",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultUVS",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//str = detailGridElement->getData(gridIndex++).second;
								//newStateSet->setAlphaOperate(TEXTURE_DIFFUSEMAP3,getAlphaOperateFromStr(str));

								//str = detailGridElement->getData(gridIndex++).second;
								//flt1 = atof(str.c_str());
								//if(flt1 != 0.0f)
								//{
								//	uniform = newStateSet->getOrCreateUniform("param1AddDT3a",CRCore::crUniform::FLOAT);
								//	uniform->setDataVariance(crBase::STATIC);
								//	uniform->set(flt1);
								//}

								//str = detailGridElement->getData(gridIndex++).second;
								//flt1 = atof(str.c_str());
								//if(flt1 != 1.0f)
								//{
								//	uniform = newStateSet->getOrCreateUniform("param1MultDT3a",CRCore::crUniform::FLOAT);
								//	uniform->setDataVariance(crBase::STATIC);
								//	uniform->set(flt1);
								//}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("UVSMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								uvsMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								uvsMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								uvsMap->setWrap(crTexture::WRAP_S,wrap);
								uvsMap->setWrap(crTexture::WRAP_T,wrap);
								uvsMap->setWrap(crTexture::WRAP_R,wrap);

								std::string extImageNameID = uvsMap->getImageNameID() + "_UVS";
								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(extImageNameID);
								if(loadedTex)
								{
									loadedTex->setExternTextureID(crTexture::UVSCRAMBLER);
									loadedTex->setFilter(crTexture::MIN_FILTER,uvsMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,uvsMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,uvsMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,uvsMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,uvsMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=9;
							}
						}
					}
					/////////CW
					crEditBoxElement *colorWeightElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_colorWeightElement));
					if(colorWeightElement)
					{
						str = colorWeightElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture2D *>(origStateSet->getTextureAttribute(TEXTURE_COLORWEIGHT,crStateAttribute::TEXTURE));
						if(origMap && origMap->getExternTextureID() != crTexture::COLORWEIGHT)
						{
							origMap = NULL;
						}
						if(!str.empty())
						{
							crTexture2D *colorWeightMap = new crTexture2D;
							colorWeightMap->setExternTextureID(crTexture::COLORWEIGHT);
							colorWeightMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_COLORWEIGHT,colorWeightMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("CWMap",str);
								gridIndex++;

								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second; 
								newStateSet->setTextureUV(TEXTURE_COLORWEIGHT,atoi(str.c_str()));

								//detailGridElement->addData("param4AddCW",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddCW",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultCW",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultCW",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultCW",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultCW",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}
								
								//detailGridElement->addData("param3MultCWN",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWN",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param3MultCWD",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWD",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}
								//detailGridElement->addData("param3MultCWDa",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWDa",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param3MultCWS",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWS",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}
								//detailGridElement->addData("param3MultCWSa",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWSa",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//detailGridElement->addData("param3MultCWE",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWE",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}
								//detailGridElement->addData("param3MultCWEa",crArgumentParser::appVectoa(vec3));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec3) && vec3 != mo3)
								{
									uniform = newStateSet->getOrCreateUniform("param3MultCWEa",CRCore::crUniform::FLOAT_VEC3);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec3);
								}

								//keyValueNode = detailGridElement->getData(gridIndex++);
								//str = keyValueNode->getValue();
								//bool uvw = (bool)(atoi(str.c_str()));
								//if(!uvw)
								//{
								//	uniform = newStateSet->getOrCreateUniform("uvw",CRCore::crUniform::BOOL);
								//	uniform->setDataVariance(crBase::STATIC);
								//	uniform->set(uvw);
								//}

								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("CWMat",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("CWMatR",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("CWMatG",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("CWMatB",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}
								keyValueNode = detailGridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec2);
								if(vec2 != crVector2(1,1))
								{
									uniform = newStateSet->getOrCreateUniform("CWMatA",CRCore::crUniform::FLOAT_MAT4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(crMatrixf::scale(crVector3(vec2[0],vec2[1],1.0f)));
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								colorWeightMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								colorWeightMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								colorWeightMap->setWrap(crTexture::WRAP_S,wrap);
								colorWeightMap->setWrap(crTexture::WRAP_T,wrap);
								colorWeightMap->setWrap(crTexture::WRAP_R,wrap);

								std::string extImageNameID = colorWeightMap->getImageNameID() + "_CW";
								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(extImageNameID);
								if(loadedTex)
								{
									loadedTex->setExternTextureID(crTexture::COLORWEIGHT);
									loadedTex->setFilter(crTexture::MIN_FILTER,colorWeightMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,colorWeightMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,colorWeightMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,colorWeightMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,colorWeightMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=21;
							}
						}
					}
					///////////////////////////
					/////////EnvMap
					crEditBoxElement *envMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_envMapElement));
					if(envMapElement)
					{
						str = envMapElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTextureCubeMap *>(origStateSet->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE));
						//if(origMap && origMap->getExternTextureID() != crTexture::ENVMAP)
						//{
						//	origMap = NULL;
						//}
						if(!str.empty())
						{
							crTextureCubeMap *envMap = new crTextureCubeMap;
							envMap->setExternTextureID(crTexture::ENVMAP);
							envMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_ENVMAP,envMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("EnvMap",str);
								gridIndex++;

								//detailGridElement->addData("param4AddENV",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != ao4)
								{
									uniform = newStateSet->getOrCreateUniform("param4AddENV",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("param1MultENV",crArgumentParser::appFtoa(flt1));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								flt1 = atof(str.c_str());
								if(flt1 != 1.0f)
								{
									uniform = newStateSet->getOrCreateUniform("param1MultENV",CRCore::crUniform::FLOAT);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(flt1);
								}

								//detailGridElement->addData("param4MultENV",crArgumentParser::appVectoa(vec4));
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								if(crArgumentParser::appAtoVec(str,vec4) && vec4 != mo4)
								{
									uniform = newStateSet->getOrCreateUniform("param4MultENV",CRCore::crUniform::FLOAT_VEC4);
									uniform->setDataVariance(crBase::STATIC);
									uniform->set(vec4);
								}

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								envMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								envMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								envMap->setWrap(crTexture::WRAP_S,wrap);
								envMap->setWrap(crTexture::WRAP_T,wrap);
								envMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(envMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setExternTextureID(crTexture::ENVMAP);
									loadedTex->setFilter(crTexture::MIN_FILTER,envMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,envMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,envMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,envMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,envMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=7;
							}
						}
					}
					////////LightMap
					crEditBoxElement *lightMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lightMapElement));
					if(lightMapElement)
					{
						str = lightMapElement->getStringArrayInString();
						origMap = !origStateSet.valid()?NULL:dynamic_cast<crTexture2D *>(origStateSet->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE));
						//if(origMap && origMap->getExternTextureID() != crTexture::LIGHTMAP)
						//{
						//	origMap = NULL;
						//}
						if(!str.empty())
						{
							crTexture2D *lightMap = new crTexture2D;
							lightMap->setExternTextureID(crTexture::LIGHTMAP);
							lightMap->setImageNameID(str);
							newStateSet->setTextureAttributeAndModes(TEXTURE_LIGHTMAP,lightMap,crStateAttribute::ON);

							if(detailGridElement && origMap)
							{
								//detailGridElement->addData("LightMap",str);
								gridIndex++;

								//detailGridElement->addData("MIN_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								lightMap->setFilter(crTexture::MIN_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("MAG_FILTER",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								lightMap->setFilter(crTexture::MAG_FILTER,getFilterModeFromStr(str));
								//detailGridElement->addData("WRAP_MODE",str);
								keyValueNode = detailGridElement->getData(gridIndex++/*,dataPair*/);
								str = keyValueNode->getValue();//dataPair.second;
								crTexture::WrapMode wrap = getWrapModeFromStr(str);
								lightMap->setWrap(crTexture::WRAP_S,wrap);
								lightMap->setWrap(crTexture::WRAP_T,wrap);
								lightMap->setWrap(crTexture::WRAP_R,wrap);

								crTexture2D *loadedTex = crBrain::getInstance()->getTexture2D(lightMap->getImageNameID());
								if(loadedTex)
								{
									loadedTex->setExternTextureID(crTexture::LIGHTMAP);
									loadedTex->setFilter(crTexture::MIN_FILTER,lightMap->getFilter(crTexture::MIN_FILTER));
									loadedTex->setFilter(crTexture::MAG_FILTER,lightMap->getFilter(crTexture::MAG_FILTER));
									loadedTex->setWrap(crTexture::WRAP_S,lightMap->getWrap(crTexture::WRAP_S));
									loadedTex->setWrap(crTexture::WRAP_T,lightMap->getWrap(crTexture::WRAP_T));
									loadedTex->setWrap(crTexture::WRAP_R,lightMap->getWrap(crTexture::WRAP_R));
								}
							}
						}
						else
						{
							if(detailGridElement && origMap)
							{
								gridIndex+=4;
							}
						}
					}
					//////////
					crVector4 ambientColor,diffuseColor,specularColor,emissiveColor;
					float shininess = 1.0f;
					crMaterial *material = new crMaterial;
                    newStateSet->setAttributeAndModes(material,crStateAttribute::ON);
					//m_ambientColorElement
					crEditBoxElement *ambientColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorRElement));
					if(ambientColorRElement)
					{
						ambientColor[0] = atof(ambientColorRElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *ambientColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorGElement));
					if(ambientColorGElement)
					{
						ambientColor[1] = atof(ambientColorGElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *ambientColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientColorBElement));
					if(ambientColorBElement)
					{
						ambientColor[2] = atof(ambientColorBElement->getStringArrayInString().c_str()) / 255.0f;
					}
					ambientColor[3] = 1.0f;
					material->setAmbient(crMaterial::FRONT_AND_BACK,ambientColor);

					//diffuseColorElement
					crEditBoxElement *diffuseColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorRElement));
					if(diffuseColorRElement)
					{
						diffuseColor[0] = atof(diffuseColorRElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *diffuseColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorGElement));
					if(diffuseColorGElement)
					{
						diffuseColor[1] = atof(diffuseColorGElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *diffuseColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_diffuseColorBElement));
					if(diffuseColorBElement)
					{
						diffuseColor[2] = atof(diffuseColorBElement->getStringArrayInString().c_str()) / 255.0f;
					}
					diffuseColor[3] = 1.0f;
					material->setDiffuse(crMaterial::FRONT_AND_BACK,diffuseColor);

					//specularColorElement
					crEditBoxElement *specularColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorRElement));
					if(specularColorRElement)
					{
						specularColor[0] = atof(specularColorRElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *specularColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorGElement));
					if(specularColorGElement)
					{
						specularColor[1] = atof(specularColorGElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *specularColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_specularColorBElement));
					if(specularColorBElement)
					{
						specularColor[2] = atof(specularColorBElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *shininessElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_shininessElement));
					if(shininessElement)
					{
						shininess = atof(shininessElement->getStringArrayInString().c_str());
					}
					specularColor[3] = 1.0f;
					material->setSpecular(crMaterial::FRONT_AND_BACK,specularColor);
					material->setShininess(crMaterial::FRONT_AND_BACK,shininess);

					//emissiveColorElement
					crEditBoxElement *emissiveColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissiveColorRElement));
					if(emissiveColorRElement)
					{
						emissiveColor[0] = atof(emissiveColorRElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *emissiveColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissiveColorGElement));
					if(emissiveColorGElement)
					{
						emissiveColor[1] = atof(emissiveColorGElement->getStringArrayInString().c_str()) / 255.0f;
					}
					crEditBoxElement *emissiveColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_emissiveColorBElement));
					if(emissiveColorBElement)
					{
						emissiveColor[2] = atof(emissiveColorBElement->getStringArrayInString().c_str()) / 255.0f;
					}
				    emissiveColor[3] = 1.0f;
					material->setEmission(crMaterial::FRONT_AND_BACK,emissiveColor);

					if(!selectDrawable->getStateSet() && !dynamic_cast<SubMeshDrawable *>(selectDrawable))
						currentSelectObject->setStateSet(newStateSet.get());
					else
						selectDrawable->setStateSet(newStateSet.get());
                    handle.outputParam(0,currentSelectObject);
					handle.outputParam(1,selectDrawable);
					bodyInfo->unSelectAll();
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crSaveNodeMethod
//
/////////////////////////////////////////
crSaveNodeMethod::crSaveNodeMethod(){}
crSaveNodeMethod::crSaveNodeMethod(const crSaveNodeMethod& handle):
crMethod(handle),
m_filePathElement(handle.m_filePathElement)
{
}
void crSaveNodeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveNodeMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePathElement = str;
		break;
	}
}

void crSaveNodeMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);

		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crNode *currentSelectNode = selectNodeVec[0].get();
			if(currentSelectNode)
			{
				bodyInfo->unSelectAll();
				crData *infoData = bodyInfo->getDataClass();
				assert(infoData);

				std::string filePath;
				crEditBoxElement *filePathElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_filePathElement));
				if(filePathElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					currentSelectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						filePath =  filePathElement->getStringArrayInString();
						std::string command = proxyNode->getDescriptionsInString();
					    crArgumentParser::updateOrAddKeyValue(command,"DataFile",filePath);
                        proxyNode->setDescriptionsInString(command);
						CRUtil::crPrepareVisitor prepareVisitor;
						proxyNode->accept(prepareVisitor);
						//ref_ptr<crMatrixTransform> matNode = dynamic_cast<crMatrixTransform *>(proxyNode->getChild(0));
						//if(matNode.valid() && matNode->getName().compare("TempM") == 0)
						//{
						//	int num = matNode->getNumChildren();
						//	if(num>1)
						//	{
						//		crGroup *group = new crGroup;
						//		proxyNode->replaceChildExt(matNode.get(),group);
						//		group->setName("TempG");
						//	}
						//	else
						//	{
						//		proxyNode->removeChild(matNode.get());
						//		proxyNode->addChild(matNode->getChild(0));
						//		matNode->removeChild(0,num);
						//	}
						//}
						CRIOManager::writeNodeFile(*(proxyNode->getChild(0)),filePath);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crModifyObjectDrawableMethod
//
/////////////////////////////////////////
crModifyObjectDrawableMethod::crModifyObjectDrawableMethod(){}
crModifyObjectDrawableMethod::crModifyObjectDrawableMethod(const crModifyObjectDrawableMethod& handle):
crMethod(handle)
{
}
void crModifyObjectDrawableMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_currentSelectObject = param?(crObject*)param:NULL;
		break;
	case 1:
		m_currentSelectDrawable = param?(crDrawable*)param:NULL;
		break;
	}
}

void crModifyObjectDrawableMethod::addParam(int i, const std::string& str)
{
}

void crModifyObjectDrawableMethod::operator()(crHandle &handle)
{
	if(m_currentSelectObject.valid() && m_currentSelectDrawable.valid())
	{
		SubMeshDrawable *subMesh = dynamic_cast<SubMeshDrawable *>(m_currentSelectDrawable.get());
		crStateSet *newStateSet = m_currentSelectDrawable->getStateSet();
		if(subMesh && newStateSet)
		{//SubMeshDrawable save and load
			CreBodyNode *bodyNode = subMesh->getBodyNode();
			//assert(bodyNode);
			std::string filename = bodyNode->getMeshMaterialFileName(subMesh->getMeshID());
			std::string meshName;
            
			if(!filename.empty()||crArgumentParser::readKeyValue(bodyNode->getDescriptions(),"FileName",filename))
			{
				meshName = subMesh->getName();
				rcfg::ConfigScript cfg_script;
				if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
				{
					CRCore::notify(CRCore::FATAL)<<"crModifyObjectDrawableMethod(): file open error,"<<filename<<std::endl;
					return;
				}

				if(cfg_script.Push("Character"))
				{
					std::string strbuf;
					crTexture *texture;
					crUniform *uniform;
					float flt1;
					std::vector<float> v_i;
					crVector3 vec3;
					crVector4 vec4;
					crVector3 mo3(1.0f,1.0f,1.0f);
					crVector3 ao3(0.0f,0.0f,0.0f);
					crVector4 mo4(1.0f,1.0f,1.0f,1.0f);
					crVector4 ao4(0.0f,0.0f,0.0f,0.0f);
					int meshMatID = 1;
					bool find = false;
					while (cfg_script.Push("MeshMaterial",meshMatID++))
					{
						cfg_script.Get("MeshName", strbuf);
						if(meshName.compare(strbuf) == 0) 
						{
							find = true;
							break;
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crModifyDrawableMaterialMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
					}
					meshMatID -= 1;
					if(!find)
					{
						cfg_script.Add("MeshMaterial");
						cfg_script.Push("MeshMaterial",meshMatID);
						cfg_script.Add("MeshName",meshName);
					}
					strbuf = m_currentSelectObject->getDescriptionsInString();
					for(std::string::iterator itr = strbuf.begin(); itr != strbuf.end(); ++itr)
					{
						if((*itr) == '\n') (*itr) = '@';
					}
					if(!cfg_script.Modify("MeshObjDes",strbuf))
						cfg_script.Add("MeshObjDes",strbuf);

					//双面显示...
					crStateAttribute::VRModeValue cullface = newStateSet->getMode(GL_CULL_FACE);
					if(cullface == crStateAttribute::OFF)
					{
						if(!cfg_script.Modify("TwoSided",1))
						{
							cfg_script.Add("TwoSided",1);
						}
					}
					else
					{
						if(!cfg_script.Modify("TwoSided",0))
						{
							cfg_script.Add("TwoSided",0);
						}
					}
					
					crVector4 color = subMesh->getColor();
					color *= 255.0f;
					v_i.resize(4);
					v_i[0] = color[0];
					v_i[1] = color[1];
					v_i[2] = color[2];
					v_i[3] = color[3];
					if(!cfg_script.Modify("Color",v_i))
					{
						cfg_script.Add("Color",v_i);
					}

					float maskClipValue = 0.3333f;
					crBlendFunc *blendFunc = dynamic_cast<crBlendFunc *>(newStateSet->getAttribute(crStateAttribute::BLENDFUNC));
					if(blendFunc)
					{
						crBlendFunc::BlendFuncMode source,destination;
						source = (crBlendFunc::BlendFuncMode)(blendFunc->getSource());
						destination = (crBlendFunc::BlendFuncMode)(blendFunc->getDestination());

						//if(!cfg_script.Modify("MaskClipValue",maskClipValue))
						//{
						//	cfg_script.Add("MaskClipValue",maskClipValue);
						//}
						crAlphaFunc *alphaFunc = dynamic_cast<crAlphaFunc *>(newStateSet->getAttribute(crStateAttribute::ALPHAFUNC));
						if(alphaFunc)
						{
							maskClipValue = alphaFunc->getReferenceValue();
							if(!cfg_script.Modify("MaskClipValue",maskClipValue))
							{
								cfg_script.Add("MaskClipValue",maskClipValue);
							}
						}
						else
						{
							if(!cfg_script.Modify("MaskClipValue",0.0f))
							{
								cfg_script.Add("MaskClipValue",0.0f);
							}
						}
						if(source == crBlendFunc::SRC_ALPHA && destination == crBlendFunc::ONE_MINUS_SRC_ALPHA)
						{
							if(!cfg_script.Modify("BlendMode","Trans"))
							{
								cfg_script.Add("BlendMode","Trans");
							}
						}
						else if(source == crBlendFunc::ONE && destination == crBlendFunc::ONE)
						{
							if(!cfg_script.Modify("BlendMode","Add"))
							{
								cfg_script.Add("BlendMode","Add");
							}
						}
						else if(source == crBlendFunc::SRC_ALPHA && destination == crBlendFunc::ONE)
						{
							if(!cfg_script.Modify("BlendMode","Modulate"))
							{
								cfg_script.Add("BlendMode","Modulate");
							}
						}
						std::string str;
						crDisplayDrawableMaterialMethod::getBinDetailStr(newStateSet->getBinNumber(),str);
						if(!cfg_script.Modify("RenderOrder",str))
						{
							cfg_script.Add("RenderOrder",str);
						}
					}
					else
					{
						crAlphaFunc *alphaFunc = dynamic_cast<crAlphaFunc *>(newStateSet->getAttribute(crStateAttribute::ALPHAFUNC));
						if(alphaFunc)
						{
							maskClipValue = alphaFunc->getReferenceValue();
							if(!cfg_script.Modify("MaskClipValue",maskClipValue))
							{
								cfg_script.Add("MaskClipValue",maskClipValue);
							}
							if(!cfg_script.Modify("BlendMode","Mask"))
							{
								cfg_script.Add("BlendMode","Mask");
							}
						}
						else
						{
							if(!cfg_script.Modify("MaskClipValue",maskClipValue))
							{
								cfg_script.Add("MaskClipValue",maskClipValue);
							}
							if(!cfg_script.Modify("BlendMode","Opaque"))
							{
								cfg_script.Add("BlendMode","Opaque");
							}
						}
					}
					crStateAttribute::VRModeValue depthTest = newStateSet->getMode(GL_DEPTH_TEST);
					if(depthTest == crStateAttribute::OFF)
					{
						if(!cfg_script.Modify("DisableDepthTest",1))
						{
							cfg_script.Add("DisableDepthTest",1);
						}
					}
					else
					{
						if(!cfg_script.Modify("DisableDepthTest",0))
						{
							cfg_script.Add("DisableDepthTest",0);
						}
					}
					crDepth *depth = dynamic_cast<crDepth *>(newStateSet->getAttribute(crStateAttribute::DEPTH));
					if(depth)
					{
						crDepth::Function func = depth->getFunction();
						switch (func)
						{
						case crDepth::NEVER:
							if(!cfg_script.Modify("DepthFunc","NEVER"))
							{
								cfg_script.Add("DepthFunc","NEVER");
							}
							break;
						case crDepth::LESS:
							if(!cfg_script.Modify("DepthFunc","LESS"))
							{
								cfg_script.Add("DepthFunc","LESS");
							}
							break;
						case crDepth::EQUAL:
							if(!cfg_script.Modify("DepthFunc","EQUAL"))
							{
								cfg_script.Add("DepthFunc","EQUAL");
							}
							break;
						case crDepth::LEQUAL:
							if(!cfg_script.Modify("DepthFunc","LEQUAL"))
							{
								cfg_script.Add("DepthFunc","LEQUAL");
							}
							break;
						case crDepth::GREATER:
							if(!cfg_script.Modify("DepthFunc","GREATER"))
							{
								cfg_script.Add("DepthFunc","GREATER");
							}
							break;
						case crDepth::NOTEQUAL:
							if(!cfg_script.Modify("DepthFunc","NOTEQUAL"))
							{
								cfg_script.Add("DepthFunc","NOTEQUAL");
							}
							break;
						case crDepth::GEQUAL:
							if(!cfg_script.Modify("DepthFunc","GEQUAL"))
							{
								cfg_script.Add("DepthFunc","GEQUAL");
							}
							break;
						case crDepth::ALWAYS:
							if(!cfg_script.Modify("DepthFunc","ALWAYS"))
							{
								cfg_script.Add("DepthFunc","ALWAYS");
							}
							break;
						}
						v_i.resize(0);
						v_i.push_back(depth->getZNear());
						v_i.push_back(depth->getZFar());
						if(!cfg_script.Modify("DepthRange",v_i))
						{
							cfg_script.Add("DepthRange",v_i);
						}
						int depthWrite = depth->getWriteMask()?1:0;
						if(!cfg_script.Modify("DepthWrite",depthWrite))
						{
							cfg_script.Add("DepthWrite",depthWrite);
						}
					}
					else
					{
						if(!cfg_script.Modify("DepthFunc","Inherit"))
						{
							cfg_script.Add("DepthFunc","Inherit");
						}
					}
					crPolygonMode* polygonMode = dynamic_cast<crPolygonMode *>(newStateSet->getAttribute(crStateAttribute::POLYGONMODE));
					if(polygonMode)
					{
						crPolygonMode::Mode mode = polygonMode->getMode(crPolygonMode::FRONT_AND_BACK);
						switch (mode)
						{
						case crPolygonMode::POINT:
							if(!cfg_script.Modify("PolygonMode","Point"))
							{
								cfg_script.Add("PolygonMode","Point");
							}
							break;
						case crPolygonMode::LINE:
							if(!cfg_script.Modify("PolygonMode","Line"))
							{
								cfg_script.Add("PolygonMode","Line");
							}
							break;
						case crPolygonMode::FILL:
							if(!cfg_script.Modify("PolygonMode","Fill"))
							{
								cfg_script.Add("PolygonMode","Fill");
							}
							break;
						}
					}
					else
					{
						if(!cfg_script.Modify("PolygonMode","Fill"))
						{
							cfg_script.Add("PolygonMode","Fill");
						}
					}

					crMaterial *material = dynamic_cast<crMaterial *>(newStateSet->getAttribute(crStateAttribute::MATERIAL));
					crVector4 ambientColor,diffuseColor,specularColor,emissiveColor;
					float shininess = 0.0f;
					if(material)
					{
						ambientColor = material->getAmbient(crMaterial::FRONT);
						diffuseColor = material->getDiffuse(crMaterial::FRONT);
						specularColor = material->getSpecular(crMaterial::FRONT);
						emissiveColor = material->getEmission(crMaterial::FRONT);
						shininess = material->getShininess(crMaterial::FRONT);
					}
					if(!cfg_script.Push("Material"))
					{
						cfg_script.Add("Material");
						cfg_script.Push("Material");
					}
					v_i.resize(0);
					v_i.push_back(ambientColor[0]);
					v_i.push_back(ambientColor[1]);
					v_i.push_back(ambientColor[2]);
					v_i.push_back(ambientColor[3]);
					if(!cfg_script.Modify("AmbientColor",v_i))
					{
						cfg_script.Add("AmbientColor",v_i);
					}
					v_i.resize(0);
					v_i.push_back(diffuseColor[0]);
					v_i.push_back(diffuseColor[1]);
					v_i.push_back(diffuseColor[2]);
					v_i.push_back(diffuseColor[3]);
					if(!cfg_script.Modify("DiffuseColor",v_i))
					{
						cfg_script.Add("DiffuseColor",v_i);
					}
					v_i.resize(0);
					v_i.push_back(specularColor[0]);
					v_i.push_back(specularColor[1]);
					v_i.push_back(specularColor[2]);
					v_i.push_back(specularColor[3]);
					if(!cfg_script.Modify("SpecularColor",v_i))
					{
						cfg_script.Add("SpecularColor",v_i);
					}
					if(!cfg_script.Modify("Shininess",shininess))
					{
						cfg_script.Add("Shininess",shininess);
					}
					v_i.resize(0);
					v_i.push_back(emissiveColor[0]);
					v_i.push_back(emissiveColor[1]);
					v_i.push_back(emissiveColor[2]);
					v_i.push_back(emissiveColor[3]);
					if(!cfg_script.Modify("EmissiveColor",v_i))
					{
						cfg_script.Add("EmissiveColor",v_i);
					}
					cfg_script.Pop();
					//
					//BaseMap DT1
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_BASEMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("BaseMap"))
					{
						cfg_script.Add("BaseMap");
						cfg_script.Push("BaseMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("BaseMap",strbuf))
						{
							cfg_script.Add("BaseMap",strbuf);
						}
						if(!cfg_script.Modify("AlphaShadowEnable",texture->getAlphaShadowEnable()?1:0))
						{
							cfg_script.Add("AlphaShadowEnable",texture->getAlphaShadowEnable()?1:0);
						}
						if(!cfg_script.Modify("BlendShadowEnable",texture->getBlendShadowEnable()?1:0))
						{
							cfg_script.Add("BlendShadowEnable",texture->getBlendShadowEnable()?1:0);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_BASEMAP),strbuf);
						if(!cfg_script.Modify("OpBT",strbuf))
						{
							cfg_script.Add("OpBT",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_BASEMAP)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_BASEMAP));
						}

						uniform= newStateSet->getUniform("param4AddBT");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddBT",v_i))
						{
							cfg_script.Add("param4AddBT",v_i);
						}

						uniform= newStateSet->getUniform("param1MultBT");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultBT",flt1))
						{
							cfg_script.Add("param1MultBT",flt1);
						}

						uniform= newStateSet->getUniform("param4MultBT");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultBT",v_i))
						{
							cfg_script.Add("param4MultBT",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_BASEMAP),strbuf);
						if(!cfg_script.Modify("OpBTA",strbuf))
						{
							cfg_script.Add("OpBTA",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddBTa");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddBTa",flt1))
						{
							cfg_script.Add("param1AddBTa",flt1);
						}
						uniform= newStateSet->getUniform("param1MultBTa");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultBTa",flt1))
						{
							cfg_script.Add("param1MultBTa",flt1);
						}

						uniform= newStateSet->getUniform("baseMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("baseMat",v_i))
						{
							cfg_script.Add("baseMat",v_i);
						}
						uniform= newStateSet->getUniform("baseMatA");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("baseMatA",v_i))
						{
							cfg_script.Add("baseMatA",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("BaseMap",strbuf);
					}
					cfg_script.Pop();
					//DiffuseMap DT2
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_DIFFUSEMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("DiffuseMap"))
					{
						cfg_script.Add("DiffuseMap");
						cfg_script.Push("DiffuseMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("DiffuseMap",strbuf))
						{
							cfg_script.Add("DiffuseMap",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_DIFFUSEMAP),strbuf);
						if(!cfg_script.Modify("OpDT",strbuf))
						{
							cfg_script.Add("OpDT",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_DIFFUSEMAP)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_DIFFUSEMAP));
						}

						uniform= newStateSet->getUniform("param4AddDT");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddDT",v_i))
						{
							cfg_script.Add("param4AddDT",v_i);
						}

						uniform= newStateSet->getUniform("param1MultDT");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultDT",flt1))
						{
							cfg_script.Add("param1MultDT",flt1);
						}

						uniform= newStateSet->getUniform("param4MultDT");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultDT",v_i))
						{
							cfg_script.Add("param4MultDT",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_DIFFUSEMAP),strbuf);
						if(!cfg_script.Modify("OpDTA",strbuf))
						{
							cfg_script.Add("OpDTA",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddDTa");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddDTa",flt1))
						{
							cfg_script.Add("param1AddDTa",flt1);
						}
						uniform= newStateSet->getUniform("param1MultDTa");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultDTa",flt1))
						{
							cfg_script.Add("param1MultDTa",flt1);
						}

						uniform= newStateSet->getUniform("diffuseMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("diffuseMat",v_i))
						{
							cfg_script.Add("diffuseMat",v_i);
						}
						uniform= newStateSet->getUniform("diffuseMatA");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("diffuseMatA",v_i))
						{
							cfg_script.Add("diffuseMatA",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("DiffuseMap",strbuf);
					}
					cfg_script.Pop();
					//DiffuseMap2 DT3
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_DIFFUSEMAP2,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("DiffuseMap2"))
					{
						cfg_script.Add("DiffuseMap2");
						cfg_script.Push("DiffuseMap2");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("DiffuseMap2",strbuf))
						{
							cfg_script.Add("DiffuseMap2",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_DIFFUSEMAP2),strbuf);
						if(!cfg_script.Modify("OpDT2",strbuf))
						{
							cfg_script.Add("OpDT2",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_DIFFUSEMAP2)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_DIFFUSEMAP2));
						}

						uniform= newStateSet->getUniform("param4AddDT2");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddDT2",v_i))
						{
							cfg_script.Add("param4AddDT2",v_i);
						}

						uniform= newStateSet->getUniform("param1MultDT2");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultDT2",flt1))
						{
							cfg_script.Add("param1MultDT2",flt1);
						}

						uniform= newStateSet->getUniform("param4MultDT2");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultDT2",v_i))
						{
							cfg_script.Add("param4MultDT2",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_DIFFUSEMAP2),strbuf);
						if(!cfg_script.Modify("OpDT2A",strbuf))
						{
							cfg_script.Add("OpDT2A",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddDT2a");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddDT2a",flt1))
						{
							cfg_script.Add("param1AddDT2a",flt1);
						}
						uniform= newStateSet->getUniform("param1MultDT2a");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultDT2a",flt1))
						{
							cfg_script.Add("param1MultDT2a",flt1);
						}

						uniform= newStateSet->getUniform("diffuseMat2");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("diffuseMat2",v_i))
						{
							cfg_script.Add("diffuseMat2",v_i);
						}
						uniform= newStateSet->getUniform("diffuseMat2A");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("diffuseMat2A",v_i))
						{
							cfg_script.Add("diffuseMat2A",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("DiffuseMap2",strbuf);
					}
					cfg_script.Pop();
					//NormalMap NT1
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_BUMPMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("NormalMap"))
					{
						cfg_script.Add("NormalMap");
						cfg_script.Push("NormalMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("NormalMap",strbuf))
						{
							cfg_script.Add("NormalMap",strbuf);
						}
						if(!cfg_script.Modify("UseParallax",texture->getUseParallax()?1:0))
						{
							cfg_script.Add("UseParallax",texture->getUseParallax()?1:0);
						}
						if(texture->getUseParallax())
						{
							uniform= newStateSet->getUniform("parallax");
							float parallax = 0.03f;
							if(uniform) uniform->get(parallax);
							if(!cfg_script.Modify("parallax",parallax))
							{
								cfg_script.Add("parallax",parallax);
							}
						}
						uniform= newStateSet->getUniform("param3AddNT");
						vec3.set(0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3AddNT",v_i))
						{
							cfg_script.Add("param3AddNT",v_i);
						}

						uniform= newStateSet->getUniform("param1MultNT");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultNT",flt1))
						{
							cfg_script.Add("param1MultNT",flt1);
						}

						uniform= newStateSet->getUniform("param3MultNT");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultNT",v_i))
						{
							cfg_script.Add("param3MultNT",v_i);
						}

						uniform= newStateSet->getUniform("normalMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("normalMat",v_i))
						{
							cfg_script.Add("normalMat",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("NormalMap",strbuf);
					}
					cfg_script.Pop();
					//NormalMap2 NT2
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_NORMALMAP2,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("NormalMap2"))
					{
						cfg_script.Add("NormalMap2");
						cfg_script.Push("NormalMap2");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("NormalMap2",strbuf))
						{
							cfg_script.Add("NormalMap2",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_NORMALMAP2),strbuf);
						if(!cfg_script.Modify("OpNT2",strbuf))
						{
							cfg_script.Add("OpNT2",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_NORMALMAP2)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_NORMALMAP2));
						}

						uniform= newStateSet->getUniform("param3AddNT2");
						vec3.set(0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3AddNT2",v_i))
						{
							cfg_script.Add("param3AddNT2",v_i);
						}

						uniform= newStateSet->getUniform("param1MultNT2");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultNT2",flt1))
						{
							cfg_script.Add("param1MultNT2",flt1);
						}

						uniform= newStateSet->getUniform("param3MultNT2");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultNT2",v_i))
						{
							cfg_script.Add("param3MultNT2",v_i);
						}

						uniform= newStateSet->getUniform("normalMat2");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("normalMat2",v_i))
						{
							cfg_script.Add("normalMat2",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("NormalMap2",strbuf);
					}
					cfg_script.Pop();
					//NormalMap3 NT3
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_NORMALMAP3,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("NormalMap3"))
					{
						cfg_script.Add("NormalMap3");
						cfg_script.Push("NormalMap3");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("NormalMap3",strbuf))
						{
							cfg_script.Add("NormalMap3",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_NORMALMAP3),strbuf);
						if(!cfg_script.Modify("OpNT3",strbuf))
						{
							cfg_script.Add("OpNT3",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_NORMALMAP3)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_NORMALMAP3));
						}

						uniform= newStateSet->getUniform("param3AddNT3");
						vec3.set(0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3AddNT3",v_i))
						{
							cfg_script.Add("param3AddNT3",v_i);
						}

						uniform= newStateSet->getUniform("param1MultNT3");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultNT3",flt1))
						{
							cfg_script.Add("param1MultNT3",flt1);
						}

						uniform= newStateSet->getUniform("param3MultNT3");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultNT3",v_i))
						{
							cfg_script.Add("param3MultNT3",v_i);
						}

						uniform= newStateSet->getUniform("normalMat3");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("normalMat3",v_i))
						{
							cfg_script.Add("normalMat3",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("NormalMap3",strbuf);
					}
					cfg_script.Pop();
					//SpecularMap ST1
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_SPECULARMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("SpecularMap"))
					{
						cfg_script.Add("SpecularMap");
						cfg_script.Push("SpecularMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("SpecularMap",strbuf))
						{
							cfg_script.Add("SpecularMap",strbuf);
						}
						//if(!cfg_script.Modify("AlphaShininessEnable",texture->getAlphaShininessEnable()?1:0))
						//{
						//	cfg_script.Add("AlphaShininessEnable",texture->getAlphaShininessEnable()?1:0);
						//}
						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_SPECULARMAP),strbuf);
						if(!cfg_script.Modify("OpST",strbuf))
						{
							cfg_script.Add("OpST",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP));
						}

						uniform= newStateSet->getUniform("param4AddST");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddST",v_i))
						{
							cfg_script.Add("param4AddST",v_i);
						}

						uniform= newStateSet->getUniform("param1MultST");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultST",flt1))
						{
							cfg_script.Add("param1MultST",flt1);
						}

						uniform= newStateSet->getUniform("param4MultST");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultST",v_i))
						{
							cfg_script.Add("param4MultST",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_SPECULARMAP),strbuf);
						if(!cfg_script.Modify("OpSTA",strbuf))
						{
							cfg_script.Add("OpSTA",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddSTa");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddSTa",flt1))
						{
							cfg_script.Add("param1AddSTa",flt1);
						}
						uniform= newStateSet->getUniform("param1MultSTa");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultSTa",flt1))
						{
							cfg_script.Add("param1MultSTa",flt1);
						}

						uniform= newStateSet->getUniform("specularMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMat",v_i))
						{
							cfg_script.Add("specularMat",v_i);
						}
						uniform= newStateSet->getUniform("specularMatA");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMatA",v_i))
						{
							cfg_script.Add("specularMatA",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("SpecularMap",strbuf);
					}
					cfg_script.Pop();
					//SpecularMap2 ST2
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_SPECULARMAP2,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("SpecularMap2"))
					{
						cfg_script.Add("SpecularMap2");
						cfg_script.Push("SpecularMap2");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("SpecularMap2",strbuf))
						{
							cfg_script.Add("SpecularMap2",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_SPECULARMAP2),strbuf);
						if(!cfg_script.Modify("OpST2",strbuf))
						{
							cfg_script.Add("OpST2",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP2)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP2));
						}
						uniform= newStateSet->getUniform("param4AddST2");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddST2",v_i))
						{
							cfg_script.Add("param4AddST2",v_i);
						}

						uniform= newStateSet->getUniform("param1MultST2");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultST2",strbuf))
						{
							cfg_script.Add("param1MultST2",strbuf);
						}

						uniform= newStateSet->getUniform("param4MultST2");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultST2",v_i))
						{
							cfg_script.Add("param4MultST2",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_SPECULARMAP2),strbuf);
						if(!cfg_script.Modify("OpST2A",strbuf))
						{
							cfg_script.Add("OpST2A",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddST2a");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddST2a",flt1))
						{
							cfg_script.Add("param1AddST2a",flt1);
						}
						uniform= newStateSet->getUniform("param1MultST2a");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultST2a",flt1))
						{
							cfg_script.Add("param1MultST2a",flt1);
						}

						uniform= newStateSet->getUniform("specularMat2");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMat2",v_i))
						{
							cfg_script.Add("specularMat2",v_i);
						}
						uniform= newStateSet->getUniform("specularMat2A");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMat2A",v_i))
						{
							cfg_script.Add("specularMat2A",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("SpecularMap2",strbuf);
					}
					cfg_script.Pop();
					//SpecularMap3 ST3
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_SPECULARMAP3,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("SpecularMap3"))
					{
						cfg_script.Add("SpecularMap3");
						cfg_script.Push("SpecularMap3");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("SpecularMap3",strbuf))
						{
							cfg_script.Add("SpecularMap3",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_SPECULARMAP3),strbuf);
						if(!cfg_script.Modify("OpST3",strbuf))
						{
							cfg_script.Add("OpST3",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP3)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_SPECULARMAP3));
						}
						uniform= newStateSet->getUniform("param4AddST3");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddST3",v_i))
						{
							cfg_script.Add("param4AddST3",v_i);
						}

						uniform= newStateSet->getUniform("param1MultST3");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultST3",flt1))
						{
							cfg_script.Add("param1MultST3",flt1);
						}

						uniform= newStateSet->getUniform("param4MultST3");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultST3",v_i))
						{
							cfg_script.Add("param4MultST3",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_SPECULARMAP3),strbuf);
						if(!cfg_script.Modify("OpST3A",strbuf))
						{
							cfg_script.Add("OpST3A",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddST3a");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddST3a",flt1))
						{
							cfg_script.Add("param1AddST3a",flt1);
						}

						uniform= newStateSet->getUniform("param1MultST3a");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultST3a",flt1))
						{
							cfg_script.Add("param1MultST3a",flt1);
						}

						uniform= newStateSet->getUniform("specularMat3");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMat3",v_i))
						{
							cfg_script.Add("specularMat3",v_i);
						}
						uniform= newStateSet->getUniform("specularMat3A");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("specularMat3A",v_i))
						{
							cfg_script.Add("specularMat3A",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("SpecularMap3",strbuf);
					}
					cfg_script.Pop();
					//EmissiveMap ET1
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("EmissiveMap"))
					{
						cfg_script.Add("EmissiveMap");
						cfg_script.Push("EmissiveMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("EmissiveMap",strbuf))
						{
							cfg_script.Add("EmissiveMap",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_EMISSIVEMAP),strbuf);
						if(!cfg_script.Modify("OpET",strbuf))
						{
							cfg_script.Add("OpET",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP));
						}
						uniform= newStateSet->getUniform("param4AddET");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddET",v_i))
						{
							cfg_script.Add("param4AddET",v_i);
						}

						uniform= newStateSet->getUniform("param1MultET");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultET",flt1))
						{
							cfg_script.Add("param1MultET",flt1);
						}

						uniform= newStateSet->getUniform("param4MultET");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultET",v_i))
						{
							cfg_script.Add("param4MultET",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_EMISSIVEMAP),strbuf);
						if(!cfg_script.Modify("OpETA",strbuf))
						{
							cfg_script.Add("OpETA",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddETa");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddETa",flt1))
						{
							cfg_script.Add("param1AddETa",flt1);
						}
						uniform= newStateSet->getUniform("param1MultETa");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultETa",flt1))
						{
							cfg_script.Add("param1MultETa",flt1);
						}

						uniform= newStateSet->getUniform("emissiveMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMat",v_i))
						{
							cfg_script.Add("emissiveMat",v_i);
						}
						uniform= newStateSet->getUniform("emissiveMatA");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMatA",v_i))
						{
							cfg_script.Add("emissiveMatA",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("EmissiveMap",strbuf);
					}
					cfg_script.Pop();
					//EmissiveMap2 ET2
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP2,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("EmissiveMap2"))
					{
						cfg_script.Add("EmissiveMap2");
						cfg_script.Push("EmissiveMap2");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("EmissiveMap2",strbuf))
						{
							cfg_script.Add("EmissiveMap2",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_EMISSIVEMAP2),strbuf);
						if(!cfg_script.Modify("OpET2",strbuf))
						{
							cfg_script.Add("OpET2",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP2)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP2));
						}
						uniform= newStateSet->getUniform("param4AddET2");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddET2",v_i))
						{
							cfg_script.Add("param4AddET2",v_i);
						}

						uniform= newStateSet->getUniform("param1MultET2");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultET2",flt1))
						{
							cfg_script.Add("param1MultET2",flt1);
						}

						uniform= newStateSet->getUniform("param4MultET2");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultET2",v_i))
						{
							cfg_script.Add("param4MultET2",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_EMISSIVEMAP2),strbuf);
						if(!cfg_script.Modify("OpET2A",strbuf))
						{
							cfg_script.Add("OpET2A",strbuf);
						}
						uniform= newStateSet->getUniform("param1AddET2a");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddET2a",flt1))
						{
							cfg_script.Add("param1AddET2a",flt1);
						}
						uniform= newStateSet->getUniform("param1MultET2a");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultET2a",flt1))
						{
							cfg_script.Add("param1MultET2a",flt1);
						}

						uniform= newStateSet->getUniform("emissiveMat2");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMat2",v_i))
						{
							cfg_script.Add("emissiveMat2",v_i);
						}
						uniform= newStateSet->getUniform("emissiveMat2A");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMat2A",v_i))
						{
							cfg_script.Add("emissiveMat2A",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("EmissiveMap2",strbuf);
					}
					cfg_script.Pop();
					//EmissiveMap3 ET3
					texture = dynamic_cast<crTexture *>(newStateSet->getTextureAttribute(TEXTURE_EMISSIVEMAP3,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("EmissiveMap3"))
					{
						cfg_script.Add("EmissiveMap3");
						cfg_script.Push("EmissiveMap3");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("EmissiveMap3",strbuf))
						{
							cfg_script.Add("EmissiveMap3",strbuf);
						}

						crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_EMISSIVEMAP3),strbuf);
						if(!cfg_script.Modify("OpET3",strbuf))
						{
							cfg_script.Add("OpET3",strbuf);
						}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP3)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_EMISSIVEMAP3));
						}
						uniform= newStateSet->getUniform("param4AddET3");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddET3",v_i))
						{
							cfg_script.Add("param4AddET3",v_i);
						}

						uniform= newStateSet->getUniform("param1MultET3");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultET3",flt1))
						{
							cfg_script.Add("param1MultET3",flt1);
						}

						uniform= newStateSet->getUniform("param4MultET3");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultET3",v_i))
						{
							cfg_script.Add("param4MultET3",v_i);
						}

						crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_EMISSIVEMAP3),strbuf);
						if(!cfg_script.Modify("OpET3A",strbuf))
						{
							cfg_script.Add("OpET3A",strbuf);
						}

						uniform= newStateSet->getUniform("param1AddET3a");
						flt1 = 0.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1AddET3a",flt1))
						{
							cfg_script.Add("param1AddET3a",flt1);
						}

						uniform= newStateSet->getUniform("param1MultET3a");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultET3a",flt1))
						{
							cfg_script.Add("param1MultET3a",flt1);
						}

						uniform= newStateSet->getUniform("emissiveMat3");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMat3",v_i))
						{
							cfg_script.Add("emissiveMat3",v_i);
						}
						uniform= newStateSet->getUniform("emissiveMat3A");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("emissiveMat3A",v_i))
						{
							cfg_script.Add("emissiveMat3A",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("EmissiveMap3",strbuf);
					}
					cfg_script.Pop();
					////////////////////ExternTexture
					crTexture *uvsMap = NULL;
					crTexture *cwMap = NULL;
					crTexture2D *externTex2D = dynamic_cast<crTexture2D *>(newStateSet->getTextureAttribute(TEXTURE_UVSCRAMBLER,crStateAttribute::TEXTURE));
					crTexture::ExternTextureID externTextureID = crTexture::EXT_NONE;
					if(externTex2D)
					{
						externTextureID = externTex2D->getExternTextureID();
						switch (externTextureID)
						{
						case crTexture::EXT_NONE:
							break;
						case crTexture::UVSCRAMBLER:
							uvsMap = externTex2D;
							break;
						case crTexture::COLORWEIGHT:
							cwMap = externTex2D;
							break;
						}
					}
					//UVSMap
					if(!cfg_script.Push("UVSMap"))
					{
						cfg_script.Add("UVSMap");
						cfg_script.Push("UVSMap");
					}
					if(uvsMap)
					{
						texture = uvsMap;
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("UVSMap",strbuf))
						{
							cfg_script.Add("UVSMap",strbuf);
						}

						//crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_UVSCRAMBLER),strbuf);
						//if(!cfg_script.Modify("OpDT3",strbuf))
						//{
						//	cfg_script.Add("OpDT3",strbuf);
						//}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_UVSCRAMBLER)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_UVSCRAMBLER));
						}
						uniform= newStateSet->getUniform("param4AddUVS");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddUVS",v_i))
						{
							cfg_script.Add("param4AddUVS",v_i);
						}

						uniform= newStateSet->getUniform("param1MultUVS");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultUVS",flt1))
						{
							cfg_script.Add("param1MultUVS",flt1);
						}

						uniform= newStateSet->getUniform("param4MultUVS");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultUVS",v_i))
						{
							cfg_script.Add("param4MultUVS",v_i);
						}

						//crDisplayDrawableMaterialMethod::getAlphaOperateStr(newStateSet->getAlphaOperate(TEXTURE_UVSCRAMBLER),strbuf);
						//if(!cfg_script.Modify("OpDT3A",strbuf))
						//{
						//	cfg_script.Add("OpDT3A",strbuf);
						//}
						//uniform= newStateSet->getUniform("param1AddDT3a");
						//flt1 = 0.0f;
						//if(uniform) uniform->get(flt1);
						//if(!cfg_script.Modify("param1AddDT3a",flt1))
						//{
						//	cfg_script.Add("param1AddDT3a",flt1);
						//}

						//uniform= newStateSet->getUniform("param1MultDT3a");
						//flt1 = 1.0f;
						//if(uniform) uniform->get(flt1);
						//if(!cfg_script.Modify("param1MultDT3a",flt1))
						//{
						//	cfg_script.Add("param1MultDT3a",flt1);
						//}
						uniform= newStateSet->getUniform("UVSMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("UVSMat",v_i))
						{
							cfg_script.Add("UVSMat",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("UVSMap",strbuf);
					}
					cfg_script.Pop();
					//CWMap
					if(!cfg_script.Push("CWMap"))
					{
						cfg_script.Add("CWMap");
						cfg_script.Push("CWMap");
					}
					if(cwMap)
					{
						texture = cwMap;
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("CWMap",strbuf))
						{
							cfg_script.Add("CWMap",strbuf);
						}

						//crDisplayDrawableMaterialMethod::getTextureOperateStr(newStateSet->getTextureOperate(TEXTURE_COLORWEIGHT),strbuf);
						//if(!cfg_script.Modify("OpDT3",strbuf))
						//{
						//	cfg_script.Add("OpDT3",strbuf);
						//}
						if(!cfg_script.Modify("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_COLORWEIGHT)))
						{
							cfg_script.Add("UVLayer",(int)newStateSet->getTextureUV(TEXTURE_COLORWEIGHT));
						}
						uniform= newStateSet->getUniform("param4AddCW");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddCW",v_i))
						{
							cfg_script.Add("param4AddCW",v_i);
						}

						uniform= newStateSet->getUniform("param1MultCW");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultCW",flt1))
						{
							cfg_script.Add("param1MultCW",flt1);
						}

						uniform= newStateSet->getUniform("param4MultCW");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultCW",v_i))
						{
							cfg_script.Add("param4MultCW",v_i);
						}

						uniform= newStateSet->getUniform("param3MultCWN");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWN",v_i))
						{
							cfg_script.Add("param3MultCWN",v_i);
						}

						uniform= newStateSet->getUniform("param3MultCWD");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWD",v_i))
						{
							cfg_script.Add("param3MultCWD",v_i);
						}
						uniform= newStateSet->getUniform("param3MultCWDa");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWDa",v_i))
						{
							cfg_script.Add("param3MultCWDa",v_i);
						}

						uniform= newStateSet->getUniform("param3MultCWS");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWS",v_i))
						{
							cfg_script.Add("param3MultCWS",v_i);
						}
						uniform= newStateSet->getUniform("param3MultCWSa");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWSa",v_i))
						{
							cfg_script.Add("param3MultCWSa",v_i);
						}

						uniform= newStateSet->getUniform("param3MultCWE");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWE",v_i))
						{
							cfg_script.Add("param3MultCWE",v_i);
						}
						uniform= newStateSet->getUniform("param3MultCWEa");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec3);
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						v_i.push_back(vec3[2]);
						if(!cfg_script.Modify("param3MultCWEa",v_i))
						{
							cfg_script.Add("param3MultCWEa",v_i);
						}

						//uniform= newStateSet->getUniform("uvw");
						//bool uvw = true;
						//if(uniform) uniform->get(uvw);
						//if(!cfg_script.Modify("uvw",uvw?1:0))
						//{
						//	cfg_script.Add("uvw",uvw?1:0);
						//}

						uniform= newStateSet->getUniform("CWMat");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("CWMat",v_i))
						{
							cfg_script.Add("CWMat",v_i);
						}
						uniform= newStateSet->getUniform("CWMatR");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("CWMatR",v_i))
						{
							cfg_script.Add("CWMatR",v_i);
						}
						uniform= newStateSet->getUniform("CWMatG");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("CWMatG",v_i))
						{
							cfg_script.Add("CWMatG",v_i);
						}
						uniform= newStateSet->getUniform("CWMatB");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("CWMatB",v_i))
						{
							cfg_script.Add("CWMatB",v_i);
						}
						uniform= newStateSet->getUniform("CWMatA");
						vec3.set(1.0f,1.0f,1.0f);
						if(uniform)
						{
							crMatrixf m4;
							uniform->get(m4);
							vec3 = m4.getScale();
						}
						v_i.resize(0);
						v_i.push_back(vec3[0]);
						v_i.push_back(vec3[1]);
						if(!cfg_script.Modify("CWMatA",v_i))
						{
							cfg_script.Add("CWMatA",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("CWMap",strbuf);
					}
					cfg_script.Pop();
					/////////////////////////////////////////
					//EnvMap
					texture = dynamic_cast<crTextureCubeMap *>(newStateSet->getTextureAttribute(TEXTURE_ENVMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("EnvMap"))
					{
						cfg_script.Add("EnvMap");
						cfg_script.Push("EnvMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("EnvMap",strbuf))
						{
							cfg_script.Add("EnvMap",strbuf);
						}

						uniform= newStateSet->getUniform("param4AddENV");
						vec4.set(0.0f,0.0f,0.0f,0.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4AddENV",v_i))
						{
							cfg_script.Add("param4AddENV",v_i);
						}

						uniform= newStateSet->getUniform("param1MultENV");
						flt1 = 1.0f;
						if(uniform) uniform->get(flt1);
						if(!cfg_script.Modify("param1MultENV",flt1))
						{
							cfg_script.Add("param1MultENV",flt1);
						}

						uniform= newStateSet->getUniform("param4MultENV");
						vec4.set(1.0f,1.0f,1.0f,1.0f);
						if(uniform) uniform->get(vec4);
						v_i.resize(0);
						v_i.push_back(vec4[0]);
						v_i.push_back(vec4[1]);
						v_i.push_back(vec4[2]);
						v_i.push_back(vec4[3]);
						if(!cfg_script.Modify("param4MultENV",v_i))
						{
							cfg_script.Add("param4MultENV",v_i);
						}

						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("EnvMap",strbuf);
					}
					cfg_script.Pop();
					//LightMap
					texture = dynamic_cast<crTexture2D *>(newStateSet->getTextureAttribute(TEXTURE_LIGHTMAP,crStateAttribute::TEXTURE));
					if(!cfg_script.Push("LightMap"))
					{
						cfg_script.Add("LightMap");
						cfg_script.Push("LightMap");
					}
					if(texture)
					{
						strbuf = texture->getImageNameID();
						if(!cfg_script.Modify("LightMap",strbuf))
						{
							cfg_script.Add("LightMap",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MIN_FILTER),strbuf);
						if(!cfg_script.Modify("MIN_FILTER",strbuf))
						{
							cfg_script.Add("MIN_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getFilterModeStr(texture->getFilter(crTexture::MAG_FILTER),strbuf);
						if(!cfg_script.Modify("MAG_FILTER",strbuf))
						{
							cfg_script.Add("MAG_FILTER",strbuf);
						}
						crDisplayDrawableMaterialMethod::getWrapModeStr(texture->getWrap(crTexture::WRAP_S),strbuf);
						if(!cfg_script.Modify("WRAP_MODE",strbuf))
						{
							cfg_script.Add("WRAP_MODE",strbuf);
						}
					}
					else
					{
						strbuf = "NULL";
						cfg_script.Modify("LightMap",strbuf);
					}
					cfg_script.Pop();
					///////////////////////////////
					cfg_script.Pop();

					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"crModifyDrawableMaterialMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;

					cfg_script.Write(filename.c_str());
				}
				cfg_script.Close();

				CRIOManager::crWriteCookFile scopedWrite(filename);

				//reload character
				std::string tmpFile = "tmp.cre";
				crGroup *parent = bodyNode->getParent(0);
				CRIOManager::writeNodeFile(*bodyNode,tmpFile);
				crNode *newNode = CRIOManager::readNodeFile(tmpFile);
				if(newNode)
				{
					//parent->addChild(newNode);
					//CRIOManager::crLoadManager::getInstance()->requestCompile(newNode);
					CreBodyNode *loadedNode = dynamic_cast<CreBodyNode *>(dynamic_cast<crGroup*>(newNode)->getChild(0));
					if(loadedNode)
					{
						const Body::MeshIDSet& meshIDSet = bodyNode->getBody()->getBodyMeshSet();
                        loadedNode->checkAndSetBodyMeshSet(meshIDSet);
					}
                    CRIOManager::crLoadManager::getInstance()->requestAddNode(parent,newNode,true);
					//newNode
				}
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,bodyNode);
			}
		}
		else
		{
			crGroup *parent = m_currentSelectObject->getParent(0);
			CRUtil::crPrepareVisitor prepareVisitor;
			m_currentSelectObject->accept(prepareVisitor);
			std::string tmpFile = "tmp.cre";
			CRIOManager::writeNodeFile(*m_currentSelectObject,tmpFile);

			crObject *newObj = dynamic_cast<crObject *>(CRIOManager::readNodeFile(tmpFile));
			if(newObj)
			{
				crStateSet *ss = newObj->getStateSet();
				if(ss) ss->dirtyShaderStr();
				crObject *parentObj;
				int i;
				crDrawable::ParentList &parents = m_currentSelectDrawable->getParents();
				std::list<crObject *> objList;
				int numDrawables = newObj->getNumDrawables();
				for( crDrawable::ParentList::iterator itr = parents.begin();
					itr != parents.end();
					++itr )
				{
					parentObj = dynamic_cast<crObject *>(*itr);
					if(parentObj != m_currentSelectObject.get() && parentObj->getNumDrawables() == numDrawables)
					{
						objList.push_back(parentObj);
					}
				}
				for( std::list<crObject *>::iterator itr = objList.begin();
					itr != objList.end();
					++itr )
				{
					parentObj = *itr;
					parentObj->removeDrawable(0,numDrawables);
					for(i = 0; i<numDrawables; ++i)
						parentObj->addDrawable(newObj->getDrawable(i));
					ss = parentObj->getStateSet();
					if(ss) ss->dirtyShaderStr();
				}
				for(i = 0; i<numDrawables; ++i)
				{
					ss = newObj->getDrawable(i)->getStateSet();
					if(ss) ss->dirtyShaderStr();
				}
                CRIOManager::crLoadManager::getInstance()->requestAddNode(parent,newObj,true);
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,m_currentSelectObject.get());
			}
		}
	}
}

/////////////////////////////////////////
//
//crSnapMaterialMethod
//
/////////////////////////////////////////
crSnapMaterialMethod::crSnapMaterialMethod():
m_selectType(2){}
crSnapMaterialMethod::crSnapMaterialMethod(const crSnapMaterialMethod& handle):
crMethod(handle),
m_selectType(handle.m_selectType)
{
}
void crSnapMaterialMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crSnapMaterialMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
	    m_selectType = atoi(str.c_str());
}

void crSnapMaterialMethod::operator()(crHandle &handle)
{
    if(m_checkBox.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);

		crData *infoData = bodyInfo->getDataClass();
		assert(infoData);

		void *param;
		infoData->getParam(EDP_SelectType,param);
		int selectType = *((int*)param);

		if(m_checkBox->getSelect())
		{
            if(selectType == m_selectType)
			{
                selectType = -1;//PickMaterial
				infoData->inputParam(EDP_SelectType,&selectType);
			}
		}
		else
		{
			if(selectType == -1)
			{
				selectType = m_selectType;
				infoData->inputParam(EDP_SelectType,&selectType);
			}
		}
	}
}
/////////////////////////////////////////
//
//crPickNeedLightMethod
//
/////////////////////////////////////////
crPickNeedLightMethod::crPickNeedLightMethod(){}
crPickNeedLightMethod::crPickNeedLightMethod(const crPickNeedLightMethod& handle):
crMethod(handle)
{
}
void crPickNeedLightMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crPickNeedLightMethod::addParam(int i, const std::string& str)
{
}

void crPickNeedLightMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
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

		if(m_checkBox->getSelect())
		{
			if(selectType == 1)
			{
				selectType = -3;//PickNeedLight
				infoData->inputParam(0,&selectType);
			}
		}
		else
		{
			if(selectType == -3)
			{
				selectType = 1;
				infoData->inputParam(0,&selectType);
			}
		}
	}
}
/////////////////////////////////////////
//
//crCheckBoxSetSelectMethod
//
/////////////////////////////////////////
crCheckBoxSetSelectMethod::crCheckBoxSetSelectMethod(){}
crCheckBoxSetSelectMethod::crCheckBoxSetSelectMethod(const crCheckBoxSetSelectMethod& handle):
crMethod(handle),
m_select(handle.m_select)
{
}
void crCheckBoxSetSelectMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crCheckBoxSetSelectMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_select = atoi(str.c_str());
		break;
	}
}

void crCheckBoxSetSelectMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		m_checkBox->setSelect(m_select);
	}
}

/////////////////////////////////////////
//
//crCheckBoxSetSelectExMethod
//
/////////////////////////////////////////
crCheckBoxSetSelectExMethod::crCheckBoxSetSelectExMethod(){}
crCheckBoxSetSelectExMethod::crCheckBoxSetSelectExMethod(const crCheckBoxSetSelectExMethod& handle):
crMethod(handle),
m_select(handle.m_select)
{
}
void crCheckBoxSetSelectExMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crCheckBoxSetSelectExMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_checkBoxElement = str;
		break;
	case 1:
		m_select = atoi(str.c_str());
		break;
	}
}

void crCheckBoxSetSelectExMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crCheckBoxElement *checkBoxElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_checkBoxElement));
		if(checkBoxElement)
		    checkBoxElement->setSelect(m_select);
	}
}

/////////////////////////////////////////
//
//crGetSaveFileNameMethod
//
/////////////////////////////////////////
crGetSaveFileNameMethod::crGetSaveFileNameMethod()
{
}

crGetSaveFileNameMethod::crGetSaveFileNameMethod(const crGetSaveFileNameMethod& handle):
crMethod(handle),
m_filePath(handle.m_filePath),
m_fileExt(handle.m_fileExt)
{
}

void crGetSaveFileNameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editorBox = (CRGUI::crEditBoxElement*)param;
}

void crGetSaveFileNameMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePath = str;
		break;
	case 1:
		m_fileExt = str;
		break;
	}
}

void crGetSaveFileNameMethod::operator()(crHandle &handle)
{
	if(m_editorBox.valid())
	{
		char programDir[256];
		GetCurrentDirectory(256,programDir);
		//std::string dir = programDir;
		//dir = dir + "/" + m_filePath;
		//SetCurrentDirectory(dir.c_str());

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[256];       // buffer for file name

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ZeroMemory(szFile, sizeof(szFile));
		//sprintf(szFile,"%s\0",m_filePath.c_str());
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = crStartHandler::getInstance()->getWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if(m_fileExt.compare("crs") == 0)
            ofn.lpstrFilter = "crs(.crs)\0*.crs\0";
		else if(m_fileExt.compare("cre") == 0)
            ofn.lpstrFilter = "cre(.cre)\0*.cre\0";
		else
			ofn.lpstrFilter = "All(*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = m_filePath.c_str();
		ofn.Flags = OFN_EXPLORER;

		// Display the Open dialog box. 
		if (GetSaveFileName(&ofn)==TRUE) 
		{
			std::string filename = ofn.lpstrFile;
			filename.erase(0,strlen(programDir)+1);
			filename = crArgumentParser::getFileNameEliminateExt(filename) + "." + m_fileExt;
			filename = CRIOManager::convertFileNameToUnixStyle(filename);
			//filename = m_filePath + "/" + crArgumentParser::getFileNameEliminateExt(crArgumentParser::getSimpleFileName(filename)) + "." + m_fileExt;
			m_editorBox->setStringArrayByString(filename);
		}
		SetCurrentDirectory(programDir);
	}
}

/////////////////////////////////////////
//
//crDisplayVolumeAttrMethod
//
/////////////////////////////////////////
crDisplayVolumeAttrMethod::crDisplayVolumeAttrMethod(){}
crDisplayVolumeAttrMethod::crDisplayVolumeAttrMethod(const crDisplayVolumeAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_enableElement(handle.m_enableElement),
m_maxTriggerCountElement(handle.m_maxTriggerCountElement),
m_reTriggerDelayElement(handle.m_reTriggerDelayElement),
m_desEditElement(handle.m_desEditElement)
{
}
void crDisplayVolumeAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayVolumeAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_enableElement = str;
		break;
	case 2:
		m_maxTriggerCountElement = str;
		break;
	case 3:
		m_reTriggerDelayElement = str;
		break;
	case 4:
		m_desEditElement = str;
		break;
	}
}

void crDisplayVolumeAttrMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		bool clearData = true;
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo)
		{
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crVolumeNode *volumeNode = dynamic_cast<crVolumeNode *>(selectNodeVec[0].get());
				if(volumeNode)
				{
					clearData = false;
					std::string str;
					crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
					if(currentSelectElement)
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						volumeNode->accept(searchByNameVisitor);
						crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(proxyNode)
						{
							if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
							{
								currentSelectElement->setStringArrayByString(str);
							}
							else
								currentSelectElement->clearString();
						}
						else
							currentSelectElement->clearString();
					}
					crCheckBoxElement *enableElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableElement));
					if(enableElement)
					{
                        enableElement->select(volumeNode->getEnable());
					}
					crEditBoxElement *maxTriggerCount = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_maxTriggerCountElement));
					if(maxTriggerCount)
					{
						maxTriggerCount->setStringArrayByString(crArgumentParser::appItoa(volumeNode->getMaxTriggerCount()));
					}
					crEditBoxElement *reTriggerDelay = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_reTriggerDelayElement));
					if(reTriggerDelay)
					{
						reTriggerDelay->setStringArrayByString(crArgumentParser::appFtoa(volumeNode->getReTriggerDelay()));
					}
					crEditBoxElement *desEdit = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_desEditElement));
					if(desEdit)
					{
						desEdit->setStringArrayByString(volumeNode->getDescriptionsInString());
					}
				}
			}
		}
		if(clearData)
		{
			crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
			if(currentSelectElement)
			{
				currentSelectElement->clearString();
			}
			crCheckBoxElement *enableElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableElement));
			if(enableElement)
			{
				enableElement->select(false);
			}
			crEditBoxElement *maxTriggerCount = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_maxTriggerCountElement));
			if(maxTriggerCount)
			{
				maxTriggerCount->clearString();
			}
			crEditBoxElement *reTriggerDelay = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_reTriggerDelayElement));
			if(reTriggerDelay)
			{
				reTriggerDelay->clearString();
			}
			crEditBoxElement *desEdit = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_desEditElement));
			if(desEdit)
			{
				desEdit->clearString();
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyVolumeAttrMethod
//
/////////////////////////////////////////
crModifyVolumeAttrMethod::crModifyVolumeAttrMethod(){}
crModifyVolumeAttrMethod::crModifyVolumeAttrMethod(const crModifyVolumeAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_enableElement(handle.m_enableElement),
m_maxTriggerCountElement(handle.m_maxTriggerCountElement),
m_reTriggerDelayElement(handle.m_reTriggerDelayElement),
m_desEditElement(handle.m_desEditElement)
{
}
void crModifyVolumeAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyVolumeAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_enableElement = str;
		break;
	case 2:
		m_maxTriggerCountElement = str;
		break;
	case 3:
		m_reTriggerDelayElement = str;
		break;
	case 4:
		m_desEditElement = str;
		break;
	}
}

void crModifyVolumeAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crVolumeNode *volumeNode = dynamic_cast<crVolumeNode *>(selectNodeVec[0].get());
			if(volumeNode)
			{
				std::string str;
				crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_currentSelectElement));
				if(currentSelectElement)
				{
				}
				std::string command;
				crEditBoxElement *desEdit = dynamic_cast<crEditBoxElement *>(stage->getElement(m_desEditElement));
				if(desEdit)
				{
					command = desEdit->getStringArrayInString();
				}
				crCheckBoxElement *enableElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableElement));
				if(enableElement)
				{
					volumeNode->setEnable(enableElement->getSelect());
					crArgumentParser::updateOrAddKeyValue(command,"Enable",(int)(volumeNode->getEnable()));
				}
				crEditBoxElement *maxTriggerCount = dynamic_cast<crEditBoxElement *>(stage->getElement(m_maxTriggerCountElement));
				if(maxTriggerCount)
				{
					volumeNode->setMaxTriggerCount(atoi(maxTriggerCount->getStringArrayInString().c_str()));
				    crArgumentParser::updateOrAddKeyValue(command,"MaxTriggerCount",volumeNode->getMaxTriggerCount());
				}
				crEditBoxElement *reTriggerDelay = dynamic_cast<crEditBoxElement *>(stage->getElement(m_reTriggerDelayElement));
				if(reTriggerDelay)
				{
					volumeNode->setReTriggerDelay(atof(reTriggerDelay->getStringArrayInString().c_str()));
					crArgumentParser::updateOrAddKeyValue(command,"ReTriggerDelay",volumeNode->getReTriggerDelay());
				}
				volumeNode->setDescriptionsInString(command);
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisplayMoviesAttrMethod
//
/////////////////////////////////////////
crDisplayMoviesAttrMethod::crDisplayMoviesAttrMethod(){}
crDisplayMoviesAttrMethod::crDisplayMoviesAttrMethod(const crDisplayMoviesAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_loopMode(handle.m_loopMode),
m_pivotX(handle.m_pivotX),
m_pivotY(handle.m_pivotY),
m_pivotZ(handle.m_pivotZ),
m_timeOffset(handle.m_timeOffset),
m_timeMult(handle.m_timeMult),
m_keyGrid(handle.m_keyGrid),
m_playMode(handle.m_playMode)
{
}
void crDisplayMoviesAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayMoviesAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_loopMode = str;
		break;
	case 2:
		m_pivotX = str;
		break;
	case 3:
		m_pivotY = str;
		break;
	case 4:
		m_pivotZ = str;
		break;
	case 5:
		m_timeOffset = str;
		break;
	case 6:
		m_timeMult = str;
		break;
	case 7:
		m_keyGrid = str;
		break;
	case 8:
		m_playMode = str;
		break;
	}
}

void crDisplayMoviesAttrMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		bool clearData = true;
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo)
		{
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
				if(currentSelectNode)
				{
					clearData = false;
					std::string str;
					crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
					if(currentSelectElement)
					{
						CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						currentSelectNode->accept(searchByNameVisitor);
						crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(proxyNode)
						{
							if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
							{
								currentSelectElement->setStringArrayByString(str);
							}
							else
								currentSelectElement->clearString();
						}
						else
							currentSelectElement->clearString();
					}

					crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
					if(animationPathCallback)
					{
						const crVector3d& pivotPoint = animationPathCallback->getPivotPoint();
						crEditBoxElement *pivotXElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotX));
						if(pivotXElement)
						{
							pivotXElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[0]));
						}
						crEditBoxElement *pivotYElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotY));
						if(pivotYElement)
						{
							pivotYElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[1]));
						}
						crEditBoxElement *pivotZElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotZ));
						if(pivotZElement)
						{
							pivotZElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[2]));
						}
						double timeOffset = animationPathCallback->getTimeOffset();
						crEditBoxElement *timeOffsetElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeOffset));
						if(timeOffsetElement)
						{
							timeOffsetElement->setStringArrayByString(crArgumentParser::appFtoa(timeOffset));
						}
						double timeMultiplier = animationPathCallback->getTimeMultiplier();
						crEditBoxElement *timeMultiplierElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeMult));
						if(timeMultiplierElement)
						{
							timeMultiplierElement->setStringArrayByString(crArgumentParser::appFtoa(timeMultiplier));
						}
						crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
						crAnimationPath::LoopMode loopMode = animationPath->getLoopMode();
						crRadioGroupElement *loopModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_loopMode));
						if(loopModeElement)
						{
							switch (loopMode)
							{
							case crAnimationPath::SWING:
								loopModeElement->select(0);
								break;
							case crAnimationPath::LOOP:
								loopModeElement->select(1);
								break;
							case crAnimationPath::NO_LOOPING:
								loopModeElement->select(2);
								break;
							}
						}
						crRadioGroupElement *playModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_playMode));
						if(playModeElement)
						{
							if(animationPathCallback->getPause())
                                playModeElement->select(1);
							else
					            playModeElement->select(0);
						}
						animationPath->lock();
						crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
						crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_keyGrid));
						if(keyGridElement)
						{
							keyGridElement->clearData();
							for( crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
								itr != timeControlPointMap.end();
								++itr )
							{
								keyGridElement->addData("KeyFrame",crArgumentParser::appFtoa(itr->first),"");
							}
							keyGridElement->addData("KeyFrame","End","");
							keyGridElement->checkSelect();
							keyGridElement->updateInputElement(true);
						}
						animationPath->unlock();
					}
					else
					{
						crEditBoxElement *pivotXElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotX));
						if(pivotXElement)
						{
							pivotXElement->clearString();
						}
						crEditBoxElement *pivotYElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotY));
						if(pivotYElement)
						{
							pivotYElement->clearString();
						}
						crEditBoxElement *pivotZElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotZ));
						if(pivotZElement)
						{
							pivotZElement->clearString();
						}
						crEditBoxElement *timeOffsetElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeOffset));
						if(timeOffsetElement)
						{
							timeOffsetElement->clearString();
						}
						crEditBoxElement *timeMultiplierElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeMult));
						if(timeMultiplierElement)
						{
							timeMultiplierElement->clearString();
						}
						crRadioGroupElement *loopModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_loopMode));
						if(loopModeElement)
						{
							loopModeElement->select(2);
						}
						crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_keyGrid));
						if(keyGridElement)
						{
							keyGridElement->clearData();
							keyGridElement->checkSelect();
						}
						crRadioGroupElement *playModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_playMode));
						if(playModeElement)
						{
							playModeElement->select(1);
						}
					}
				}
			}
		}
		if(clearData)
		{
			crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
			if(currentSelectElement)
			{
				currentSelectElement->clearString();
			}
			crEditBoxElement *pivotXElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotX));
			if(pivotXElement)
			{
				pivotXElement->clearString();
			}
			crEditBoxElement *pivotYElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotY));
			if(pivotYElement)
			{
				pivotYElement->clearString();
			}
			crEditBoxElement *pivotZElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_pivotZ));
			if(pivotZElement)
			{
				pivotZElement->clearString();
			}
			crEditBoxElement *timeOffsetElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeOffset));
			if(timeOffsetElement)
			{
				timeOffsetElement->clearString();
			}
			crEditBoxElement *timeMultiplierElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_timeMult));
			if(timeMultiplierElement)
			{
				timeMultiplierElement->clearString();
			}
			crRadioGroupElement *loopModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_loopMode));
			if(loopModeElement)
			{
				loopModeElement->select(2);
			}
			crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_keyGrid));
			if(keyGridElement)
			{
				keyGridElement->clearData();
				keyGridElement->checkSelect();
			}
			crRadioGroupElement *playModeElement = dynamic_cast<crRadioGroupElement *>(m_stage->getElement(m_playMode));
			if(playModeElement)
			{
				playModeElement->select(1);
			}
		}
	}
}
/////////////////////////////////////////
//
//crAddKeyFrameMethod
//
/////////////////////////////////////////
crAddKeyFrameMethod::crAddKeyFrameMethod(){}
crAddKeyFrameMethod::crAddKeyFrameMethod(const crAddKeyFrameMethod& handle):
crMethod(handle)
{
}
void crAddKeyFrameMethod::inputParam(int i, void *param)
{
}

void crAddKeyFrameMethod::addParam(int i, const std::string& str)
{
}

void crAddKeyFrameMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
			if(animationPathCallback)
			{
                crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
				if(animationPath)
				{
					crMatrix mat = currentSelectNode->getMatrix();
					crQuat rotation;
					mat.get(rotation);
					crAnimationPath::ControlPoint controlPoint(mat.getTrans(),rotation,mat.getScale());
					animationPath->insert(animationPath->getLastTime() + 1.0f,controlPoint);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyMoviesAttrMethod
//
/////////////////////////////////////////
crModifyMoviesAttrMethod::crModifyMoviesAttrMethod(){}
crModifyMoviesAttrMethod::crModifyMoviesAttrMethod(const crModifyMoviesAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_loopMode(handle.m_loopMode),
m_pivotX(handle.m_pivotX),
m_pivotY(handle.m_pivotY),
m_pivotZ(handle.m_pivotZ),
m_timeOffset(handle.m_timeOffset),
m_timeMult(handle.m_timeMult),
m_keyGrid(handle.m_keyGrid)
{
}
void crModifyMoviesAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyMoviesAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_loopMode = str;
		break;
	case 2:
		m_pivotX = str;
		break;
	case 3:
		m_pivotY = str;
		break;
	case 4:
		m_pivotZ = str;
		break;
	case 5:
		m_timeOffset = str;
		break;
	case 6:
		m_timeMult = str;
		break;
	case 7:
		m_keyGrid = str;
		break;
	}
}

void crModifyMoviesAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				std::string str;
				crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_currentSelectElement));
				if(currentSelectElement)
				{
				}
				crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
				if(animationPathCallback)
				{
					crVector3 pivotPoint;
					crEditBoxElement *pivotXElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotX));
					if(pivotXElement)
					{
						pivotPoint[0] = atof(pivotXElement->getStringArrayInString().c_str());
					}
					crEditBoxElement *pivotYElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotY));
					if(pivotYElement)
					{
						pivotPoint[1] = atof(pivotYElement->getStringArrayInString().c_str());
					}
					crEditBoxElement *pivotZElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotZ));
					if(pivotZElement)
					{
						pivotPoint[2] = atof(pivotZElement->getStringArrayInString().c_str());
					}
					animationPathCallback->setPivotPoint(pivotPoint);
					float timeOffset;
					crEditBoxElement *timeOffsetElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_timeOffset));
					if(timeOffsetElement)
					{
						timeOffset = atof(timeOffsetElement->getStringArrayInString().c_str());
					}
					animationPathCallback->setTimeOffset(timeOffset);
					float timeMultiplier;
					crEditBoxElement *timeMultiplierElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_timeMult));
					if(timeMultiplierElement)
					{
						timeMultiplier = atof(timeMultiplierElement->getStringArrayInString().c_str());
					}
					animationPathCallback->setTimeMultiplier(timeMultiplier);
					crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
					crAnimationPath::LoopMode loopMode;
					crRadioGroupElement *loopModeElement = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_loopMode));
					if(loopModeElement)
					{
						switch (loopModeElement->getSelect())
						{
						case 0:
							loopMode = crAnimationPath::SWING;
							break;
						case 1:
							loopMode = crAnimationPath::LOOP;
							break;
						case 2:
							loopMode = crAnimationPath::NO_LOOPING;
							break;
						}
					}
					animationPath->setLoopMode(loopMode);
					animationPath->lock();
					crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
					crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_keyGrid));
					if(keyGridElement)
					{
						//crKeyValueGridElement::DataPair dataPair;
						ref_ptr<crKeyValueNode> keyValueNode;
						int gridIndex = 0;
						crAnimationPath::TimeControlPointMap tmpTimeControlPointMap;
						float time;
						for( crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
							itr != timeControlPointMap.end();
							++itr )
						{
							keyValueNode = keyGridElement->getData(gridIndex++/*,dataPair*/);
                            time = atof(keyValueNode->getValue().c_str());
							tmpTimeControlPointMap[time] = itr->second;
						}
						timeControlPointMap.swap(tmpTimeControlPointMap);
					}
					animationPath->unlock();
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDelKeyFrameMethod
//
/////////////////////////////////////////
crDelKeyFrameMethod::crDelKeyFrameMethod(){}
crDelKeyFrameMethod::crDelKeyFrameMethod(const crDelKeyFrameMethod& handle):
crMethod(handle),
m_keyGrid(handle.m_keyGrid)
{
}
void crDelKeyFrameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crDelKeyFrameMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_keyGrid = str;
		break;
	}
}

void crDelKeyFrameMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);

		crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_keyGrid));
		if(keyGridElement)
		{
			int select = keyGridElement->getSelect();
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			assert(cameraBot);
			crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
			assert(gamebodyInfo);
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
				if(currentSelectNode)
				{
					crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
					if(animationPathCallback)
					{
						crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
						animationPath->lock();
						if(animationPath && !animationPath->empty())
						{
							crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
							crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
							int i = 0;
							while (i++<select)
							{
								itr++;
							}
							timeControlPointMap.erase(itr);
						}
						animationPath->unlock();
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSetAnimationPauseMethod
//
/////////////////////////////////////////
crSetAnimationPauseMethod::crSetAnimationPauseMethod(){}
crSetAnimationPauseMethod::crSetAnimationPauseMethod(const crSetAnimationPauseMethod& handle):
crMethod(handle),
m_pause(handle.m_pause)
{
}
void crSetAnimationPauseMethod::inputParam(int i, void *param)
{
}

void crSetAnimationPauseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_pause = atoi(str.c_str());
		break;
	}
}

void crSetAnimationPauseMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
			if(animationPathCallback)
			{
				animationPathCallback->setPause(m_pause);
			}
		}
	}
}
/////////////////////////////////////////
//
//crResetAnimationMethod
//
/////////////////////////////////////////
crResetAnimationMethod::crResetAnimationMethod(){}
crResetAnimationMethod::crResetAnimationMethod(const crResetAnimationMethod& handle):
crMethod(handle)
{
}
void crResetAnimationMethod::inputParam(int i, void *param)
{
}

void crResetAnimationMethod::addParam(int i, const std::string& str)
{
}

void crResetAnimationMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
			if(animationPathCallback)
			{
				animationPathCallback->reset();
				if(animationPathCallback->getPause())
				{
					crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
					if(animationPath)
					{
						animationPath->lock();
						crAnimationPath::ControlPoint controlPoint = animationPath->getTimeControlPointMap().begin()->second;
						crMatrix matrix;
						if (animationPathCallback->getUseInverseMatrix())
							controlPoint.getInverse(matrix);
						else
							controlPoint.getMatrix(matrix);
						currentSelectNode->setMatrix(matrix * crMatrix::translate(animationPathCallback->getPivotPoint()));
						animationPath->unlock();
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSaveMoviesAttrMethod
//
/////////////////////////////////////////
crSaveMoviesAttrMethod::crSaveMoviesAttrMethod(){}
crSaveMoviesAttrMethod::crSaveMoviesAttrMethod(const crSaveMoviesAttrMethod& handle):
crMethod(handle)
{
}
void crSaveMoviesAttrMethod::inputParam(int i, void *param)
{
}

void crSaveMoviesAttrMethod::addParam(int i, const std::string& str)
{
}

void crSaveMoviesAttrMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
			if(animationPathCallback)
			{
				crAnimationPath* animationPath = animationPathCallback->getAnimationPath();

				std::string cfgFile = animationPathCallback->getCfgFile();
				
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				//struct stat buf;
				//if (stat( cfgFile.c_str(), &buf )) 
				//{
				//	CRCore::notify(CRCore::FATAL)<<"crUpdateInitCfgMethod::operator(): file open error"<<std::endl;
				//	return;
				//}

				//if (!cfg_script.Open(cfgFile.c_str()))
				//{
				//	CRCore::notify(CRCore::FATAL)<<"crUpdateInitCfgMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
				//	return;
				//}
				std::string strbuf;

				cfg_script.Add("NodeCallback");
				cfg_script.Push("NodeCallback");
				cfg_script.Add("Type","UpdateCallback");
				cfg_script.Add("Class","AnimationPath");
                crAnimationPath::LoopMode loopMode = animationPath->getLoopMode();
				switch (loopMode)
				{
				case crAnimationPath::SWING:
					strbuf = "SWING";
					break;
				case crAnimationPath::LOOP:
					strbuf = "LOOP";
					break;
				case crAnimationPath::NO_LOOPING:
					strbuf = "NO_LOOPING";
					break;
				}
				cfg_script.Add("Param",strbuf);
				crVector3 pivotPoint = animationPathCallback->getPivotPoint();
				strbuf = crArgumentParser::appVectoa(pivotPoint);
				cfg_script.Add("Param",strbuf);
				bool useInverseMatrix = animationPathCallback->getUseInverseMatrix();
				strbuf = crArgumentParser::appItoa(useInverseMatrix);
				cfg_script.Add("Param",strbuf);
				float timeOffset = animationPathCallback->getTimeOffset();
				strbuf = crArgumentParser::appFtoa(timeOffset);
				cfg_script.Add("Param",strbuf);
				float timeMultiplier = animationPathCallback->getTimeMultiplier();
				strbuf = crArgumentParser::appFtoa(timeMultiplier);
				cfg_script.Add("Param",strbuf);
				bool pause = animationPathCallback->getPause();
				strbuf = crArgumentParser::appItoa(pause);
				cfg_script.Add("Param",strbuf);

				animationPath->lock();
				float time;
				crVector3 position;
				crVector4 rotation;
				crVector3 scale;
				crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
				for( crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
					itr != timeControlPointMap.end();
					++itr )
				{
					time = itr->first;
					strbuf = crArgumentParser::appFtoa(time);
					cfg_script.Add("Param",strbuf);

					position = itr->second.getPosition();
					strbuf = crArgumentParser::appVectoa(position);
					cfg_script.Add("Param",strbuf);

					rotation = itr->second.getRotation().asVec4();
					strbuf = crArgumentParser::appVectoa(rotation);
					cfg_script.Add("Param",strbuf);

					scale = itr->second.getScale();
					strbuf = crArgumentParser::appVectoa(scale);
					cfg_script.Add("Param",strbuf);
				}
				animationPath->unlock();
				cfg_script.Pop();
				cfg_script.Write(cfgFile);
				cfg_script.Close();

				CRIOManager::crWriteCookFile scopedWrite(cfgFile);
			}
		}
	}
}
/////////////////////////////////////////
//
//crCreateMoviePathMethod
//
/////////////////////////////////////////
crCreateMoviePathMethod::crCreateMoviePathMethod():
m_root(NULL){}
crCreateMoviePathMethod::crCreateMoviePathMethod(const crCreateMoviePathMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName),
m_root(NULL)
{
}
void crCreateMoviePathMethod::inputParam(int i, void *param)
{
}

void crCreateMoviePathMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rootName = str;
		break;
	}
}

void crCreateMoviePathMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	bool clear = true;
	if(m_root)
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
				if(animationPathCallback)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setNameId("AnimPath");
					searchByNameVisitor.setSearchNodeType(OBJECT);
					m_root->accept(searchByNameVisitor);
					crObject* animPath = dynamic_cast<crObject *>(searchByNameVisitor.getResult());
					if(animPath)
					{
						CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_root,animPath);
					}
					animPath = new crObject;
					animPath->setEffectByShadow(false);
					animPath->setCalcShadow(false);
					animPath->setEnableIntersect(false);
					animPath->setName("AnimPath");
					std::string des = "EditorNodeType:AnimPath\nEditorNodeName:AnimPath";
                    animPath->setDescriptionsInString(des);
					animPath->setCollideMode(crNode::CollideNone);

					crGeometry* geom = new crGeometry;
					crStateSet *stateset = geom->getOrCreateStateSet(); 
					stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
					//crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
					//stateset->setAttributeAndModes(depth,crStateAttribute::ON);
					crLineWidth *lineWidth = new crLineWidth(2.0f);
					stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);

					//crPoint *point = new crPoint;
					//point->setSize(5.0f);
					//stateset->setAttributeAndModes(point,crStateAttribute::ON);
					//stateset->setRenderingHint(crStateSet::TRANSPARENT_BIN);
					Vec3Array* coords = new Vec3Array;
					geom->setVertexArray(coords);
					CRCore::Vec4Array* colours = new CRCore::Vec4Array(1);
					(*colours)[0].set(0.0f,0.0f,1.0,1.0f);
					geom->setColorArray(colours);
					geom->setColorBinding(crGeometry::BIND_OVERALL);
					geom->setNormalBinding(crGeometry::BIND_OFF);
					animPath->addDrawable(geom);
					crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
					crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
					const crVector3d& pivotPoint = animationPathCallback->getPivotPoint();
					for( crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
						itr != timeControlPointMap.end();
						++itr )
					{
                        coords->push_back(itr->second.getPosition() + pivotPoint);
					}
					geom->addPrimitive(new DrawArrays(crPrimitive::PT_LINE_LOOP,0,coords->size()));
					CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root,animPath,true);
					clear = false;
				}
			}
		}
	}
	if(clear)
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId("AnimPath");
		searchByNameVisitor.setSearchNodeType(OBJECT);
		m_root->accept(searchByNameVisitor);
		crObject* animPath = dynamic_cast<crObject *>(searchByNameVisitor.getResult());
		if(animPath)
		{
			CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_root,animPath);
		}
	}
}
/////////////////////////////////////////
//
//crModifyKeyFrameMethod
//
/////////////////////////////////////////
crModifyKeyFrameMethod::crModifyKeyFrameMethod(){}
crModifyKeyFrameMethod::crModifyKeyFrameMethod(const crModifyKeyFrameMethod& handle):
crMethod(handle),
m_keyGrid(handle.m_keyGrid)
{
}
void crModifyKeyFrameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyKeyFrameMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_keyGrid = str;
		break;
	}
}

void crModifyKeyFrameMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);

		crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_keyGrid));
		if(keyGridElement)
		{
			int select = keyGridElement->getSelect();
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			assert(cameraBot);
			crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
			assert(gamebodyInfo);
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
				if(currentSelectNode)
				{
					crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
					if(animationPathCallback)
					{
						crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
						animationPath->lock();
						if(animationPath && !animationPath->empty())
						{
							crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
							crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
							int i = 0;
							while (i++<select)
							{
								itr++;
							}
							crMatrix mat = currentSelectNode->getMatrix();
							crQuat rotation;
							mat.get(rotation);
						    crAnimationPath::ControlPoint controlPoint(mat.getTrans() - animationPathCallback->getPivotPoint(),rotation,mat.getScale());
							itr->second = controlPoint;
						}
						animationPath->unlock();
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSelectKeyFrameMethod
//
/////////////////////////////////////////
crSelectKeyFrameMethod::crSelectKeyFrameMethod(){}
crSelectKeyFrameMethod::crSelectKeyFrameMethod(const crSelectKeyFrameMethod& handle):
crMethod(handle)
{
}
void crSelectKeyFrameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_keyGridElement = (CRGUI::crKeyValueGridElement*)param;
}

void crSelectKeyFrameMethod::addParam(int i, const std::string& str)
{
}

void crSelectKeyFrameMethod::operator()(crHandle &handle)
{
	if(m_keyGridElement.valid())
	{
		int select = m_keyGridElement->getSelect();
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
				if(animationPathCallback)
				{
					crAnimationPath* animationPath = animationPathCallback->getAnimationPath();
					animationPath->lock();
					if(animationPath && !animationPath->empty())
					{
						crAnimationPath::TimeControlPointMap &timeControlPointMap = animationPath->getTimeControlPointMap();
						crAnimationPath::TimeControlPointMap::iterator itr = timeControlPointMap.begin();
						int i = 0;
						while (i++<select)
						{
							itr++;
						}
						crAnimationPath::ControlPoint controlPoint = itr->second;
						crMatrix matrix;
						if (animationPathCallback->getUseInverseMatrix())
							controlPoint.getInverse(matrix);
						else
							controlPoint.getMatrix(matrix);
						currentSelectNode->setMatrix(matrix * crMatrix::translate(animationPathCallback->getPivotPoint()));
					}
					animationPath->unlock();
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crExcElementHandleMethod
//
/////////////////////////////////////////
crExcElementHandleMethod::crExcElementHandleMethod(){}
crExcElementHandleMethod::crExcElementHandleMethod(const crExcElementHandleMethod& handle):
crMethod(handle),
m_targetElementName(handle.m_targetElementName),
m_lParam(handle.m_lParam),
m_wParam(handle.m_wParam)
{
}
void crExcElementHandleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crExcElementHandleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_targetElementName = str;
		break;
	case 1:
		m_lParam = str;
		break;
	case 2:
		m_wParam = str;
		break;
	}
}

void crExcElementHandleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);

		crElement *targetElement = dynamic_cast<crElement *>(stage->getElement(m_targetElementName));
		if(targetElement)
		{
			_crInt32 msgParam1,msgParam2;
			msgParam1 = crImageStage::parseEventMsg(m_lParam);
			msgParam2 = crImageStage::parseEventMsg(m_wParam);
			crHandle *handle = targetElement->getHandle(MAKEINT64(msgParam1,msgParam2));
			if(handle)
			{
				targetElement->excHandle(handle);
			}
		}
	}
}
/////////////////////////////////////////
//
//crInitAnimationModeMethod
//
/////////////////////////////////////////
crInitAnimationModeMethod::crInitAnimationModeMethod(){}
crInitAnimationModeMethod::crInitAnimationModeMethod(const crInitAnimationModeMethod& handle):
crMethod(handle)
{
}
void crInitAnimationModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_playModeElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitAnimationModeMethod::addParam(int i, const std::string& str)
{
}

void crInitAnimationModeMethod::operator()(crHandle &handle)
{
	if(m_playModeElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
				if(animationPathCallback)
				{
					if(animationPathCallback->getPause())
					{
						m_playModeElement->select(1);
					}
					else
					{
						m_playModeElement->select(0);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRadioGroupSelectMethod
//
/////////////////////////////////////////
crRadioGroupSelectMethod::crRadioGroupSelectMethod(){}
crRadioGroupSelectMethod::crRadioGroupSelectMethod(const crRadioGroupSelectMethod& handle):
crMethod(handle),
m_radioGroupElement(handle.m_radioGroupElement),
m_select(handle.m_select)
{
}
void crRadioGroupSelectMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crRadioGroupSelectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radioGroupElement = str;
		break;
	case 1:
		m_select = atoi(str.c_str());
		break;
	}
}

void crRadioGroupSelectMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		crRadioGroupElement *radioGroup = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_radioGroupElement));
		if(radioGroup)
		{
			radioGroup->select(m_select);
		}
	}
}
/////////////////////////////////////////
//
//crMovePathMethod
//
/////////////////////////////////////////
crMovePathMethod::crMovePathMethod(){}
crMovePathMethod::crMovePathMethod(const crMovePathMethod& handle):
crMethod(handle),
m_pivotX(handle.m_pivotX),
m_pivotY(handle.m_pivotY),
m_pivotZ(handle.m_pivotZ)
{
}
void crMovePathMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crMovePathMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_pivotX = str;
		break;
	case 1:
		m_pivotY = str;
		break;
	case 2:
		m_pivotZ = str;
		break;
	}
}

void crMovePathMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);

		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMatrixTransform *currentSelectNode = dynamic_cast<crMatrixTransform *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				crAnimationPathCallback *animationPathCallback = dynamic_cast<crAnimationPathCallback *>(currentSelectNode->getUpdateCallback("AnimationPath"));
				if(animationPathCallback)
				{
					crVector3d pivotPoint = currentSelectNode->getTrans();
					crEditBoxElement *pivotXElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotX));
					if(pivotXElement)
					{
						pivotXElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[0]));
					}
					crEditBoxElement *pivotYElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotY));
					if(pivotYElement)
					{
						pivotYElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[1]));
					}
					crEditBoxElement *pivotZElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_pivotZ));
					if(pivotZElement)
					{
						pivotZElement->setStringArrayByString(crArgumentParser::appFtoa(pivotPoint[2]));
					}
					animationPathCallback->setPivotPoint(pivotPoint);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyGridMethod
//
/////////////////////////////////////////
crModifyGridMethod::crModifyGridMethod(){}
crModifyGridMethod::crModifyGridMethod(const crModifyGridMethod& handle):
crMethod(handle),
m_rowsElement(handle.m_rowsElement),
m_dimensionElement(handle.m_dimensionElement),
m_zoffsetElement(handle.m_zoffsetElement)
{
}
void crModifyGridMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyGridMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_rowsElement = str;
		break;
	case 1:
		m_dimensionElement = str;
		break;
	case 2:
		m_zoffsetElement = str;
		break;
	}
}

void crModifyGridMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);

		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId("ViewGrid");
		searchByNameVisitor.setSearchNodeType(OBJECT);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		crObject *viewGrid = dynamic_cast<crObject *>(searchByNameVisitor.getResult());
		if(viewGrid)
		{
			int rows = 100;
			float dimension = 1.0f;
			float zoffset = 0.0f;
			crEditBoxElement *rowsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_rowsElement));
			if(rowsElement)
			{
				rows = atoi(rowsElement->getStringArrayInString().c_str());
				rows = CRCore::clampTo(rows,10,250);
				rowsElement->setStringArrayByString(crArgumentParser::appItoa(rows));
			}
			crEditBoxElement *dimensionElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_dimensionElement));
			if(dimensionElement)
			{
				dimension = atof(dimensionElement->getStringArrayInString().c_str());
				if(dimension < 0.001f)
				{
					dimension = 0.001f;
				}
				dimensionElement->setStringArrayByString(crArgumentParser::appFtoa(dimension));
			}
			crEditBoxElement *zoffsetElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_zoffsetElement));
			if(zoffsetElement)
			{
				zoffset = atof(zoffsetElement->getStringArrayInString().c_str());
				zoffsetElement->setStringArrayByString(crArgumentParser::appFtoa(zoffset));
			}

            crGroup *parent = viewGrid->getParent(0);
			crLoadManager::getInstance()->requestRemoveNode(parent,viewGrid);
			crLoadManager::getInstance()->requestAddNode(parent,createGrid(rows,dimension,zoffset),true);
		}
	}
}
/////////////////////////////////////////
//
//crBuildTerrainMethod
//
/////////////////////////////////////////
crBuildTerrainMethod::crBuildTerrainMethod(){}
crBuildTerrainMethod::crBuildTerrainMethod(const crBuildTerrainMethod& handle):
crMethod(handle),
m_defaultFileName(handle.m_defaultFileName),
m_rootName(handle.m_rootName),
m_columnsElement(handle.m_rowsElement),
m_rowsElement(handle.m_rowsElement),
m_lengthElement(handle.m_lengthElement),
m_widthElement(handle.m_widthElement),
m_heightMapElement(handle.m_heightMapElement),
m_maxHeightElement(handle.m_maxHeightElement)
{
}
void crBuildTerrainMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crBuildTerrainMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_defaultFileName = str;
		break;
	case 1:
		m_rootName = str;
		break;
	case 2:
		m_columnsElement = str;
		break;
	case 3:
		m_rowsElement = str;
		break;
	case 4:
		m_widthElement = str;
		break;
	case 5:
		m_lengthElement = str;
		break;
	case 6:
		m_heightMapElement = str;
		break;
	case 7:
		m_maxHeightElement = str;
		break;
	}
}

void crBuildTerrainMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		assert(stage);
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_rootName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		crGroup *root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(root)
		{
			int numColumns = 256;
			int numRows = 256;
			float length = 512.0f;
			float width = 512.0f;
			float maxHeight = 200.0f;
			std::string heightmap;
			crEditBoxElement *columnsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_columnsElement));
			if(columnsElement)
			{
				numColumns = atoi(columnsElement->getStringArrayInString().c_str());
				numColumns = CRCore::clampTo(numColumns,10,256);
				columnsElement->setStringArrayByString(crArgumentParser::appItoa(numColumns));
			}
			crEditBoxElement *rowsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_rowsElement));
			if(rowsElement)
			{
				numRows = atoi(rowsElement->getStringArrayInString().c_str());
				numRows = CRCore::clampTo(numRows,10,256);
				rowsElement->setStringArrayByString(crArgumentParser::appItoa(numRows));
			}
			crEditBoxElement *widthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_widthElement));
			if(widthElement)
			{
				width = atof(widthElement->getStringArrayInString().c_str());
				width = CRCore::clampTo(width,1.0f,10000.0f);
				widthElement->setStringArrayByString(crArgumentParser::appFtoa(width));
			}
			crEditBoxElement *lengthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lengthElement));
			if(lengthElement)
			{
				length = atof(lengthElement->getStringArrayInString().c_str());
				length = CRCore::clampTo(length,1.0f,10000.0f);
				lengthElement->setStringArrayByString(crArgumentParser::appFtoa(length));
			}
			crEditBoxElement *maxHeightElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_maxHeightElement));
			if(maxHeightElement)
			{
				maxHeight = atof(maxHeightElement->getStringArrayInString().c_str());
				maxHeight = CRCore::clampTo(maxHeight,-10000.0f,10000.0f);
				maxHeightElement->setStringArrayByString(crArgumentParser::appItoa(maxHeight));
			}
			crEditBoxElement *heightMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_heightMapElement));
			if(heightMapElement)
			{
				heightmap = heightMapElement->getStringArrayInString().c_str();
			}
			if(!heightmap.empty())
			{
				ref_ptr<crImage> heightimage = CRIOManager::readImageFile(heightmap);
				if(heightimage.valid())
				{//物理引擎测试表明不能使用20*20*10*10（numColumns*numRows*length*width）地形
					if(length == 10.0f && width == 10.0f)
					{//避开一些引起物理引擎崩溃的特殊地形参数
						if(numColumns == numRows)
						{
							if(numColumns==10 || numColumns==20)
								numColumns++;
						}
					}
					crHeightField* grid = new crHeightField;
					grid->allocate(numColumns,numRows);
					grid->setOrigin(crVector3(-width*0.5f,-length*0.5f,0.0f));
					grid->setXInterval(width/(float)(numColumns-1));
					grid->setYInterval(length/(float)(numRows-1));
                    grid->buildFromHeightMap(heightimage.get(),maxHeight);
					//grid->setSkirtHeight(maxHeight);
					//int c,r;
					//float height;
					//float cs = float(numColumns-1);
					//float rs = float(numRows-1);
					//for(r=0;r<numRows;++r)
					//{
					//	for(c=0;c<numColumns;++c)
					//	{
					//		height = heightimage->getColor(crVector3(float(c)/cs,float(r)/rs,0.0f))[1];
					//		grid->setHeight(c,r,height * maxHeight);
					//	}
					//}

					ref_ptr<CRTerrain::crHeightFieldLayer> hfl = new CRTerrain::crHeightFieldLayer;
					hfl->setHeightField(grid);

					ref_ptr<CRTerrain::crTerrainTile> terrainTile = new CRTerrain::crTerrainTile;
					ref_ptr<CRTerrain::crLocator> locator = new CRTerrain::crLocator;
					ref_ptr<CRTerrain::crValidDataOperator> validDataOperator = new CRTerrain::crNoDataValue(0.0);
					terrainTile->setTreatBoundariesToValidDataAsDefaultValue(true);

					locator->setCoordinateSystemType(CRTerrain::crLocator::PROJECTED);
					locator->setTransformAsExtents(0,0, width, length);

					hfl->setLocator(locator.get());
					hfl->setValidDataOperator(validDataOperator.get());
					//if (offset!=0.0f || scale!=1.0f)
					//{
					//	hfl->transform(offset,scale);
					//}
					terrainTile->setElevationLayer(hfl.get());
					terrainTile->setName("Terrain");
					ref_ptr<CRTerrain::crGeometryTechnique> geometryTechnique = new CRTerrain::crGeometryTechnique;
					//geometryTechnique->setEditMode(true);
					terrainTile->setTerrainTechnique(geometryTechnique.get());

					ref_ptr<CRTerrain::crImageLayer> imageLayer = new CRTerrain::crImageLayer;
					imageLayer->setImage(heightimage.get());
					imageLayer->setLocator(locator.get());
					imageLayer->setValidDataOperator(validDataOperator.get());
					//if (offset!=0.0f || scale!=1.0f)
					//{
					//	imageLayer->transform(offset,scale);
					//}
					terrainTile->setColorLayer(0,imageLayer.get());

					CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
					proxyNode->setName("ProxyNode");
					proxyNode->setDescriptionsInString("DataFile "+m_defaultFileName);
					proxyNode->addChild(terrainTile.get());
					crLoadManager::getInstance()->requestAddNode(root,proxyNode.get(),true);

					crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
					assert(cameraBot);
					crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
					assert(gamebodyInfo);
					//ref_ptr<crData> data = terrainTile->getDataClass();
					//if(!data.valid())
					//{
					//	data = crDataManager::getInstance()->getData("Event");
					//	terrainTile->setDataClass(data.get());
					//}
					//if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
					//{
					//	crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
					//	driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
					//	data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
					//	data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
					//}
					gamebodyInfo->unSelectAll();
					gamebodyInfo->selectNode(terrainTile.get());
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyTerrainMethod
//
/////////////////////////////////////////
crModifyTerrainMethod::crModifyTerrainMethod(){}
crModifyTerrainMethod::crModifyTerrainMethod(const crModifyTerrainMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_columnsElement(handle.m_columnsElement),
m_rowsElement(handle.m_rowsElement),
m_lengthElement(handle.m_lengthElement),
m_widthElement(handle.m_widthElement),
m_heightMapElement(handle.m_heightMapElement),
m_maxHeightElement(handle.m_maxHeightElement)
{
}
void crModifyTerrainMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyTerrainMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_columnsElement = str;
		break;
	case 2:
		m_rowsElement = str;
		break;
	case 3:
		m_widthElement = str;
		break;
	case 4:
		m_lengthElement = str;
		break;
	case 5:
		m_heightMapElement = str;
		break;
	case 6:
		m_maxHeightElement = str;
		break;
	}
}

void crModifyTerrainMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crTerrainTile *currentSelectNode = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(currentSelectNode->getElevationLayer());
				if(hfl)
				{
					//crGroup *parent = currentSelectNode->getParent(0);
					int numColumns = 200;
					int numRows = 200;
					float length = 100.0f;
					float width = 100.0f;
					float maxHeight = 10.0f;
					std::string heightmap;
					crEditBoxElement *columnsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_columnsElement));
					if(columnsElement)
					{
						numColumns = atoi(columnsElement->getStringArrayInString().c_str());
						numColumns = CRCore::clampTo(numColumns,10,500);
						columnsElement->setStringArrayByString(crArgumentParser::appItoa(numColumns));
					}
					crEditBoxElement *rowsElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_rowsElement));
					if(rowsElement)
					{
						numRows = atoi(rowsElement->getStringArrayInString().c_str());
						numRows = CRCore::clampTo(numRows,10,500);
						rowsElement->setStringArrayByString(crArgumentParser::appItoa(numRows));
					}
					crEditBoxElement *widthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_widthElement));
					if(widthElement)
					{
						width = atof(widthElement->getStringArrayInString().c_str());
						width = CRCore::clampTo(width,1.0f,10000.0f);
						widthElement->setStringArrayByString(crArgumentParser::appFtoa(width));
					}
					crEditBoxElement *lengthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lengthElement));
					if(lengthElement)
					{
						length = atof(lengthElement->getStringArrayInString().c_str());
						length = CRCore::clampTo(length,1.0f,10000.0f);
						lengthElement->setStringArrayByString(crArgumentParser::appFtoa(length));
					}
					crEditBoxElement *maxHeightElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_maxHeightElement));
					if(maxHeightElement)
					{
						maxHeight = atof(maxHeightElement->getStringArrayInString().c_str());
						maxHeight = CRCore::clampTo(maxHeight,-10000.0f,10000.0f);
						maxHeightElement->setStringArrayByString(crArgumentParser::appItoa(maxHeight));
					}
					crEditBoxElement *heightMapElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_heightMapElement));
					if(heightMapElement)
					{
						heightmap = heightMapElement->getStringArrayInString().c_str();
					}
					if(!heightmap.empty())
					{
						ref_ptr<crImage> heightimage = CRIOManager::readImageFile(heightmap);
						if(heightimage.valid())
						{//物理引擎测试表明不能使用20*20*10*10（numColumns*numRows*length*width）地形
							if(length == 10.0f && width == 10.0f)
							{//避开一些引起物理引擎崩溃的特殊地形参数
								if(numColumns == numRows)
								{
									if(numColumns==10 || numColumns==20)
										numColumns++;
								}
							}
							crHeightField *grid = hfl->getHeightField();
							//crHeightField* grid = new crHeightField;
							grid->allocate(numColumns,numRows);
							grid->setOrigin(crVector3(-width*0.5f,-length*0.5f,0.0f));
							grid->setXInterval(width/(float)(numColumns-1));
							grid->setYInterval(length/(float)(numRows-1));
							grid->buildFromHeightMap(heightimage.get(),maxHeight);

							if(!hfl->getLocator())
							{
								ref_ptr<CRTerrain::crLocator> locator = new CRTerrain::crLocator;
								locator->setCoordinateSystemType(CRTerrain::crLocator::PROJECTED);
								locator->setTransformAsExtents(0,0, width, length);
								hfl->setLocator(locator.get());
							}
							else
							{
								hfl->getLocator()->setTransformAsExtents(0,0, width, length);
							}
							if(!hfl->getValidDataOperator())
							{
								ref_ptr<CRTerrain::crValidDataOperator> validDataOperator = new CRTerrain::crNoDataValue(0.0);
								currentSelectNode->setTreatBoundariesToValidDataAsDefaultValue(true);
								hfl->setValidDataOperator(validDataOperator.get());
							}

							ref_ptr<CRTerrain::crImageLayer> imageLayer;
							if(currentSelectNode->getNumColorLayers()==0)
							{
								imageLayer = new CRTerrain::crImageLayer;
								imageLayer->setLocator(hfl->getLocator());
								imageLayer->setValidDataOperator(hfl->getValidDataOperator());
								currentSelectNode->setColorLayer(0,imageLayer.get());
							}
							else
							{
								imageLayer = dynamic_cast<CRTerrain::crImageLayer *>(currentSelectNode->getColorLayer(0));
							}
							imageLayer->setImage(heightimage.get());

							ref_ptr<CRTerrain::crGeometryTechnique> geometryTechnique;
							if(!currentSelectNode->getTerrainTechnique())
							{
								geometryTechnique = new CRTerrain::crGeometryTechnique;
								currentSelectNode->setTerrainTechnique(geometryTechnique.get());
							}
							else
							{
                                geometryTechnique = dynamic_cast<CRTerrain::crGeometryTechnique *>(currentSelectNode->getTerrainTechnique());
							}
							geometryTechnique->setEditMode(true);
							currentSelectNode->setDirty(true);

							//ref_ptr<CRTerrain::crHeightFieldLayer> hfl = new CRTerrain::crHeightFieldLayer;
							//hfl->setHeightField(grid);

							//ref_ptr<CRTerrain::crTerrainTile> terrainTile = new CRTerrain::crTerrainTile;
							//ref_ptr<CRTerrain::crLocator> locator = new CRTerrain::crLocator;
							//ref_ptr<CRTerrain::crValidDataOperator> validDataOperator = new CRTerrain::crNoDataValue(0.0);
							//terrainTile->setTreatBoundariesToValidDataAsDefaultValue(true);

							//locator->setCoordinateSystemType(CRTerrain::crLocator::PROJECTED);
							//locator->setTransformAsExtents(0,0, length, width);

							//hfl->setLocator(locator.get());
							//hfl->setValidDataOperator(validDataOperator.get());
							////if (offset!=0.0f || scale!=1.0f)
							////{
							////	hfl->transform(offset,scale);
							////}
							//terrainTile->setElevationLayer(hfl.get());
							//terrainTile->setName(currentSelectNode->getName());
							//terrainTile->setDescriptionsInString(currentSelectNode->getDescriptionsInString());
							//ref_ptr<CRTerrain::crGeometryTechnique> geometryTechnique = new CRTerrain::crGeometryTechnique;
							//geometryTechnique->setEditMode(true);
							//terrainTile->setTerrainTechnique(geometryTechnique.get());

							//ref_ptr<CRTerrain::crImageLayer> imageLayer = new CRTerrain::crImageLayer;
							//imageLayer->setImage(heightimage.get());
							//imageLayer->setLocator(locator.get());
							//imageLayer->setValidDataOperator(validDataOperator.get());
							////if (offset!=0.0f || scale!=1.0f)
							////{
							////	imageLayer->transform(offset,scale);
							////}
							//terrainTile->setColorLayer(0,imageLayer.get());
							//crLoadManager::getInstance()->requestAddNode(parent,terrainTile.get(),true);
							//CRIOManager::crLoadManager::getInstance()->requestRemoveNode(parent,currentSelectNode);
							////ref_ptr<crData> data = terrainTile->getDataClass();
							////if(!data.valid())
							////{
							////	data = crDataManager::getInstance()->getData("Event");
							////	terrainTile->setDataClass(data.get());
							////}
							////if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
							////{
							////	crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
							////	driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
							////	data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
							////	data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
							////}
							//gamebodyInfo->unSelectAll();
							//gamebodyInfo->selectNode(terrainTile.get());
						}
					}
				}
			}
		}	
	}
}
/////////////////////////////////////////
//
//crShowTerrainGridMethod
//
/////////////////////////////////////////
crShowTerrainGridMethod::crShowTerrainGridMethod(){}
crShowTerrainGridMethod::crShowTerrainGridMethod(const crShowTerrainGridMethod& handle):
crMethod(handle)
{
}
void crShowTerrainGridMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkbox = (CRGUI::crCheckBoxElement*)param;
}

void crShowTerrainGridMethod::addParam(int i, const std::string& str)
{
}

void crShowTerrainGridMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(gamebodyInfo);
	crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crTerrainTile *currentSelectNode = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
		if(currentSelectNode)
		{
			crGeometryTechnique *geometryTechnique = dynamic_cast<crGeometryTechnique *>(currentSelectNode->getTerrainTechnique());
            if(geometryTechnique)
			{
				geometryTechnique->setShowGrid(m_checkbox->getSelect());
			}
		}
	}
}
/////////////////////////////////////////
//
//crHideAllTerrainsGridMethod
//
/////////////////////////////////////////
class HideAllTerrainsGridVisitor : public CRCore::crNodeVisitor
{
public:

	HideAllTerrainsGridVisitor():
	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	  virtual ~HideAllTerrainsGridVisitor(){}

	  virtual void apply(CRTerrain::crTerrainTile& node)
	  { 
		  crGeometryTechnique *geometryTechnique = dynamic_cast<crGeometryTechnique *>(node.getTerrainTechnique());
		  if(geometryTechnique)
		  {
			  geometryTechnique->setShowGrid(false);
		  }
		  traverse(node);
	  }
};

static HideAllTerrainsGridVisitor s_hideAllTerrainsGridVisitor;

crHideAllTerrainsGridMethod::crHideAllTerrainsGridMethod(){}
crHideAllTerrainsGridMethod::crHideAllTerrainsGridMethod(const crHideAllTerrainsGridMethod& handle):
crMethod(handle)
{
}
void crHideAllTerrainsGridMethod::inputParam(int i, void *param)
{
}

void crHideAllTerrainsGridMethod::addParam(int i, const std::string& str)
{
}

void crHideAllTerrainsGridMethod::operator()(crHandle &handle)
{
	crBrain::getInstance()->accept(s_hideAllTerrainsGridVisitor);
}
/////////////////////////////////////////
//
//crDisplayTerrainAttrMethod
//
/////////////////////////////////////////
crDisplayTerrainAttrMethod::crDisplayTerrainAttrMethod(){}
crDisplayTerrainAttrMethod::crDisplayTerrainAttrMethod(const crDisplayTerrainAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_columnsElement(handle.m_columnsElement),
m_rowsElement(handle.m_rowsElement),
m_lengthElement(handle.m_lengthElement),
m_widthElement(handle.m_widthElement),
m_heightMapElement(handle.m_heightMapElement),
m_maxHeightElement(handle.m_maxHeightElement),
m_showGridElement(handle.m_showGridElement),
m_colorWeightElement(handle.m_colorWeightElement)
{
}
void crDisplayTerrainAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayTerrainAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_columnsElement = str;
		break;
	case 2:
		m_rowsElement = str;
		break;
	case 3:
		m_widthElement = str;
		break;
	case 4:
		m_lengthElement = str;
		break;
	case 5:
		m_heightMapElement = str;
		break;
	case 6:
		m_maxHeightElement = str;
		break;
	case 7:
		m_showGridElement = str;
		break;
	case 8:
		m_colorWeightElement = str;
		break;
	}
}

void crDisplayTerrainAttrMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crTerrainTile *currentSelectNode = dynamic_cast<crTerrainTile *>(selectNodeVec[0].get());
			if(currentSelectNode)
			{
				std::string str;
				crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_currentSelectElement));
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					currentSelectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}

				CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer*>(currentSelectNode->getElevationLayer());
				if(hfl)
				{
					crHeightField *grid = hfl->getHeightField();
					crEditBoxElement *columnsElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_columnsElement));
					if(columnsElement)
					{
						columnsElement->setStringArrayByString(crArgumentParser::appItoa(grid->getNumColumns()));
					}
					crEditBoxElement *rowsElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_rowsElement));
					if(rowsElement)
					{
						rowsElement->setStringArrayByString(crArgumentParser::appItoa(grid->getNumRows()));
					}
					crEditBoxElement *widthElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_widthElement));
					if(widthElement)
					{
						widthElement->setStringArrayByString(crArgumentParser::appFtoa(float(grid->getNumColumns()-1) * grid->getXInterval()));
					}
					crEditBoxElement *lengthElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lengthElement));
					if(lengthElement)
					{
						lengthElement->setStringArrayByString(crArgumentParser::appFtoa(float(grid->getNumRows()-1) * grid->getYInterval()));
					}
					crEditBoxElement *maxHeightElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_maxHeightElement));
					if(maxHeightElement)
					{
						maxHeightElement->setStringArrayByString(crArgumentParser::appItoa(grid->getMaxHeight()));
					}
					crEditBoxElement *heightMapElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_heightMapElement));
					if(heightMapElement)
					{
						heightMapElement->setStringArrayByString(grid->getHeightMap());
					}
					crEditBoxElement *colorWeightElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_colorWeightElement));
					if(colorWeightElement)
					{
						colorWeightElement->clearString();
						crStateSet *ter_ss = NULL;
						CRCore::crMatrixTransform *transform = dynamic_cast<CRCore::crMatrixTransform *>(currentSelectNode->getChild(0));
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
						    colorWeightElement->setStringArrayByString(colorWeightTex->getImageNameID());
						}
					}
				}
				crCheckBoxElement *showGridElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_showGridElement));
				if(showGridElement)
				{
					crGeometryTechnique *geometryTechnique = dynamic_cast<crGeometryTechnique *>(currentSelectNode->getTerrainTechnique());
					if(geometryTechnique)
					{
                        showGridElement->select(geometryTechnique->getShowGrid());
					}
					else
					{
						showGridElement->select(false);
					}
				}
			}
		}	
	}
}

/////////////////////////////////////////
//
//crDisplayPostProcessAttrMethod
//
/////////////////////////////////////////
crDisplayPostProcessAttrMethod::crDisplayPostProcessAttrMethod(){}
crDisplayPostProcessAttrMethod::crDisplayPostProcessAttrMethod(const crDisplayPostProcessAttrMethod& handle):
crMethod(handle),
m_ambientIntensityElement(handle.m_ambientIntensityElement),
m_ambientIntensityRElement(handle.m_ambientIntensityRElement),
m_ambientIntensityGElement(handle.m_ambientIntensityGElement),
m_ambientIntensityBElement(handle.m_ambientIntensityBElement),
m_brightnessElement(handle.m_brightnessElement),
m_enableFogElement(handle.m_enableFogElement),
m_fogColorElement(handle.m_fogColorElement),
m_fogColorRElement(handle.m_fogColorRElement),
m_fogColorGElement(handle.m_fogColorGElement),
m_fogColorBElement(handle.m_fogColorBElement),
m_fogColorStartElement(handle.m_fogColorStartElement),
m_fogColorEndElement(handle.m_fogColorEndElement),
m_fogColorDensityElement(handle.m_fogColorDensityElement),
m_fogColorHeightElement(handle.m_fogColorHeightElement),
m_maxDepthElement(handle.m_maxDepthElement),
m_nearClipElement(handle.m_nearClipElement),
m_farClipElement(handle.m_farClipElement),
//m_enableMotionBlurElement(handle.m_enableMotionBlurElement),
m_motionBlurFactorElement(handle.m_motionBlurFactorElement),
//m_hdrElement(handle.m_hdrElement),
//m_hdrParam1Element(handle.m_hdrParam1Element),
//m_hdrParam2Element(handle.m_hdrParam2Element),
//m_hdrParam3Element(handle.m_hdrParam3Element),
//m_hdrParam4Element(handle.m_hdrParam4Element),
//m_glowSizeElement(handle.m_glowSizeElement),
//m_enableDOFElement(handle.m_enableDOFElement),
m_dofBlurStartElement(handle.m_dofBlurStartElement),
m_dofBlurFactorElement(handle.m_dofBlurFactorElement),
m_enableCollideElement(handle.m_enableCollideElement),
m_groundDetectElement(handle.m_groundDetectElement),
m_upperSkyColorElement(handle.m_upperSkyColorElement),
m_upperSkyColorRElement(handle.m_upperSkyColorRElement),
m_upperSkyColorGElement(handle.m_upperSkyColorGElement),
m_upperSkyColorBElement(handle.m_upperSkyColorBElement),
m_upperSkyColorLumElement(handle.m_upperSkyColorLumElement),
m_lowerSkyColorElement(handle.m_lowerSkyColorElement),
m_lowerSkyColorRElement(handle.m_lowerSkyColorRElement),
m_lowerSkyColorGElement(handle.m_lowerSkyColorGElement),
m_lowerSkyColorBElement(handle.m_lowerSkyColorBElement),
m_lowerSkyColorLumElement(handle.m_lowerSkyColorLumElement),
m_staticMeshShadowElement(handle.m_staticMeshShadowElement)
{
}
void crDisplayPostProcessAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayPostProcessAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_ambientIntensityElement = str;
		break;
	case 1:
		m_ambientIntensityRElement = str;
		break;
	case 2:
		m_ambientIntensityGElement = str;
		break;
	case 3:
		m_ambientIntensityBElement = str;
		break;
	case 4:
		m_brightnessElement = str;
		break;
	case 5:
		m_enableFogElement = str;
		break;
	case 6:
		m_fogColorElement = str;
		break;
	case 7:
		m_fogColorRElement = str;
		break;
	case 8:
		m_fogColorGElement = str;
		break;
	case 9:
		m_fogColorBElement = str;
		break;
	case 10:
		m_fogColorStartElement = str;
		break;
	case 11:
		m_fogColorEndElement = str;
		break;
	case 12:
		m_fogColorDensityElement = str;
		break;
	case 13:
		m_fogColorHeightElement = str;
		break;
	case 14:
		m_maxDepthElement = str;
		break;
	case 15:
		m_nearClipElement = str;
		break;
	case 16:
		m_farClipElement = str;
		break;
	//case 16:
	//	m_enableMotionBlurElement = str;
	//	break;
	case 17:
		m_motionBlurFactorElement = str;
		break;
	//case 18:
	//	m_hdrElement = str;
	//	break;
	//case 19:
	//	m_hdrParam1Element = str;
	//	break;
	//case 20:
	//	m_hdrParam2Element = str;
	//	break;
	//case 21:
	//	m_hdrParam3Element = str;
	//	break;
	//case 22:
	//	m_hdrParam4Element = str;
	//	break;
	//case 23:
	//	m_glowSizeElement = str;
	//	break;
	//case 24:
	//	m_enableDOFElement = str;
	//	break;
	case 18:
		m_dofBlurStartElement = str;
		break;
	case 19:
		m_dofBlurFactorElement = str;
		break;
	case 20:
		m_enableCollideElement = str;
		break;
	case 21:
		m_groundDetectElement = str;
		break;
	case 22:
		m_upperSkyColorElement = str;
		break;
	case 23:
		m_upperSkyColorRElement = str;
		break;
	case 24:
		m_upperSkyColorGElement = str;
		break;
	case 25:
		m_upperSkyColorBElement = str;
		break;
	case 26:
		m_upperSkyColorLumElement = str;
		break;
	case 27:
		m_lowerSkyColorElement = str;
		break;
	case 28:
		m_lowerSkyColorRElement = str;
		break;
	case 29:
		m_lowerSkyColorGElement = str;
		break;
	case 30:
		m_lowerSkyColorBElement = str;
		break;
	case 31:
		m_lowerSkyColorLumElement = str;
		break;
	case 32:
		m_staticMeshShadowElement = str;
		break;
	}
}

void crDisplayPostProcessAttrMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		//crDisplaySettings *ds = crDisplaySettings::instance();
		crBrain *brain = crBrain::getInstance();
		const crVector4& ambientIntensity = brain->getAmbientIntensity();
		//m_ambientIntensityElement
		crEditBoxElement *ambientIntensityRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientIntensityRElement));
		if(ambientIntensityRElement)
		{
			ambientIntensityRElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientIntensity[0] * 255.0f)));
		}
		crEditBoxElement *ambientIntensityGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientIntensityGElement));
		if(ambientIntensityGElement)
		{
			ambientIntensityGElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientIntensity[1] * 255.0f)));
		}
		crEditBoxElement *ambientIntensityBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_ambientIntensityBElement));
		if(ambientIntensityBElement)
		{
			ambientIntensityBElement->setStringArrayByString(crArgumentParser::appItoa((int)(ambientIntensity[2] * 255.0f)));
		}
		float brightness = brain->getBrightness();
		crEditBoxElement *brightnessElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_brightnessElement));
		if(brightnessElement)
		{
			brightnessElement->setStringArrayByString(crArgumentParser::appFtoa(brightness));
		}
		bool enableFog = brain->getFog() == 1;
		crCheckBoxElement *enableFogElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableFogElement));
		if(enableFogElement)
		{
			enableFogElement->setSelect(enableFog);
		}
		const crVector3& fogColor = brain->getFogColor();
		//m_fogColorElement
		crEditBoxElement *fogColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorRElement));
		if(fogColorRElement)
		{
			fogColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(fogColor[0] * 255.0f)));
		}
		crEditBoxElement *fogColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorGElement));
		if(fogColorGElement)
		{
			fogColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(fogColor[1] * 255.0f)));
		}
		crEditBoxElement *fogColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorBElement));
		if(fogColorBElement)
		{
			fogColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(fogColor[2] * 255.0f)));
		}
		const crVector4& fogParams = brain->getFogParams();
		crEditBoxElement *fogColorStartElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorStartElement));
		if(fogColorStartElement)
		{
			fogColorStartElement->setStringArrayByString(crArgumentParser::appFtoa(fogParams[0]));
		}
		crEditBoxElement *fogColorEndElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorEndElement));
		if(fogColorEndElement)
		{
			fogColorEndElement->setStringArrayByString(crArgumentParser::appFtoa(fogParams[1]));
		}
		crEditBoxElement *fogColorDensityElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorDensityElement));
		if(fogColorDensityElement)
		{
			fogColorDensityElement->setStringArrayByString(crArgumentParser::appFtoa(fogParams[2]));
		}
		crEditBoxElement *fogColorHeightElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_fogColorHeightElement));
		if(fogColorHeightElement)
		{
			fogColorHeightElement->setStringArrayByString(crArgumentParser::appFtoa(fogParams[3]));
		}
		float maxDepth = brain->getMaxDepth();
		crEditBoxElement *maxDepthElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_maxDepthElement));
		if(maxDepthElement)
		{
			maxDepthElement->setStringArrayByString(crArgumentParser::appFtoa(maxDepth));
		}
        const crVector2 &clip = brain->getClip();
		crEditBoxElement *nearClipElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_nearClipElement));
		if(nearClipElement)
		{
			nearClipElement->setStringArrayByString(crArgumentParser::appFtoa(clip[0]));
		}
		crEditBoxElement *farClipElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_farClipElement));
		if(farClipElement)
		{
			farClipElement->setStringArrayByString(crArgumentParser::appFtoa(clip[1]));
		}
		//bool motionBlur = ds->getMotionBlur();
		//crCheckBoxElement *enableMotionBlurElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableMotionBlurElement));
		//if(enableMotionBlurElement)
		//{
		//	enableMotionBlurElement->setSelect(motionBlur);
		//}
		float motionBlurFactor = brain->getMotionBlurFactor();
		crEditBoxElement *motionBlurFactorElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_motionBlurFactorElement));
		if(motionBlurFactorElement)
		{
			motionBlurFactorElement->setStringArrayByString(crArgumentParser::appFtoa(motionBlurFactor));
		}
		//int hdr = ds->getHDR();
		//crEditBoxElement *hdrElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_hdrElement));
		//if(hdrElement)
		//{
		//	hdrElement->setStringArrayByString(crArgumentParser::appItoa(hdr));
		//}
		//const crVector4 &hdrParam = ds->getHdrParam();
		//crEditBoxElement *hdrParam1Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_hdrParam1Element));
		//if(hdrParam1Element)
		//{
		//	hdrParam1Element->setStringArrayByString(crArgumentParser::appFtoa(hdrParam[0]));
		//}
		//crEditBoxElement *hdrParam2Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_hdrParam2Element));
		//if(hdrParam2Element)
		//{
		//	hdrParam2Element->setStringArrayByString(crArgumentParser::appFtoa(hdrParam[1]));
		//}
		//crEditBoxElement *hdrParam3Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_hdrParam3Element));
		//if(hdrParam3Element)
		//{
		//	hdrParam3Element->setStringArrayByString(crArgumentParser::appFtoa(hdrParam[2]));
		//}
		//crEditBoxElement *hdrParam4Element = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_hdrParam4Element));
		//if(hdrParam4Element)
		//{
		//	hdrParam4Element->setStringArrayByString(crArgumentParser::appFtoa(hdrParam[3]));
		//}
		//float glowSize = ds->getGlowSize();
		//crEditBoxElement *glowSizeElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_glowSizeElement));
		//if(glowSizeElement)
		//{
		//	glowSizeElement->setStringArrayByString(crArgumentParser::appFtoa(glowSize));
		//}
		//bool dof = ds->getDepthOfField();
		//crCheckBoxElement *enableDOFElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableDOFElement));
		//if(enableDOFElement)
		//{
		//	enableDOFElement->setSelect(dof);
		//}
		float dofStart = brain->getDofBlurStart();
		crEditBoxElement *dofBlurStartElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_dofBlurStartElement));
		if(dofBlurStartElement)
		{
			dofBlurStartElement->setStringArrayByString(crArgumentParser::appFtoa(dofStart));
		}
		float dofFactor = brain->getDofBlurFactor();
		crEditBoxElement *dofBlurFactorElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_dofBlurFactorElement));
		if(dofBlurFactorElement)
		{
			dofBlurFactorElement->setStringArrayByString(crArgumentParser::appFtoa(dofFactor));
		}
        ///////////////////////////////////////////////////
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		int runMode = crDisplaySettings::instance()->getRunMode();
		crDisplaySettings::instance()->setRunMode(1);
		bool enableCollide = cameraBot->getEnableIntersect();
		crDisplaySettings::instance()->setRunMode(runMode);
		crCheckBoxElement *enableCollideElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_enableCollideElement));
		if(enableCollideElement)
		{
			enableCollideElement->setSelect(enableCollide);
		}
		bool groundDetect = cameraBot->getGroundDetect();
		crCheckBoxElement *groundDetectElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_groundDetectElement));
		if(groundDetectElement)
		{
			groundDetectElement->setSelect(groundDetect);
		}

		/////////////////////////////////////////
		crVector3 upperSkyColor = brain->getUpperSkyColor();
		float scale = 1.0f;
		for(int i = 0; i<3; i++)
		{
			if(fabs(upperSkyColor[i]) > scale)
				scale = upperSkyColor[i];
		}
        upperSkyColor /= scale;
		//m_fogColorElement
		crEditBoxElement *upperSkyColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_upperSkyColorRElement));
		if(upperSkyColorRElement)
		{
			upperSkyColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(upperSkyColor[0] * 255.0f)));
		}
		crEditBoxElement *upperSkyColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_upperSkyColorGElement));
		if(upperSkyColorGElement)
		{
			upperSkyColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(upperSkyColor[1] * 255.0f)));
		}
		crEditBoxElement *upperSkyColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_upperSkyColorBElement));
		if(upperSkyColorBElement)
		{
			upperSkyColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(upperSkyColor[2] * 255.0f)));
		}
		crEditBoxElement *upperSkyColorLumElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_upperSkyColorLumElement));
		if(upperSkyColorLumElement)
		{
			upperSkyColorLumElement->setStringArrayByString(crArgumentParser::appFtoa(scale));
		}

		crVector3 lowerSkyColor = brain->getLowerSkyColor();
		scale = 1.0f;
		for(int i = 0; i<3; i++)
		{
			if(fabs(lowerSkyColor[i]) > scale)
				scale = lowerSkyColor[i];
		}
		//m_fogColorElement
		crEditBoxElement *lowerSkyColorRElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lowerSkyColorRElement));
		if(lowerSkyColorRElement)
		{
			lowerSkyColorRElement->setStringArrayByString(crArgumentParser::appItoa((int)(lowerSkyColor[0] * 255.0f)));
		}
		crEditBoxElement *lowerSkyColorGElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lowerSkyColorGElement));
		if(lowerSkyColorGElement)
		{
			lowerSkyColorGElement->setStringArrayByString(crArgumentParser::appItoa((int)(lowerSkyColor[1] * 255.0f)));
		}
		crEditBoxElement *lowerSkyColorBElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lowerSkyColorBElement));
		if(lowerSkyColorBElement)
		{
			lowerSkyColorBElement->setStringArrayByString(crArgumentParser::appItoa((int)(lowerSkyColor[2] * 255.0f)));
		}
		crEditBoxElement *lowerSkyColorLumElement = dynamic_cast<crEditBoxElement *>(m_stage->getElement(m_lowerSkyColorLumElement));
		if(lowerSkyColorLumElement)
		{
			lowerSkyColorLumElement->setStringArrayByString(crArgumentParser::appFtoa(scale));
		}
		crCheckBoxElement *staticMeshShadowElement = dynamic_cast<crCheckBoxElement *>(m_stage->getElement(m_staticMeshShadowElement));
		if(staticMeshShadowElement)
		{
			staticMeshShadowElement->setSelect(!brain->getCalcStaticMeshShadow());
		}
	}
}
/////////////////////////////////////////
//
//crSetEnableIntersectMethod
//
/////////////////////////////////////////
crSetEnableIntersectMethod::crSetEnableIntersectMethod(){}
crSetEnableIntersectMethod::crSetEnableIntersectMethod(const crSetEnableIntersectMethod& handle):
crMethod(handle)
{
}
void crSetEnableIntersectMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkbox = (CRGUI::crCheckBoxElement*)param;
}

void crSetEnableIntersectMethod::addParam(int i, const std::string& str)
{
}

void crSetEnableIntersectMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	cameraBot->setEnableIntersect(m_checkbox->getSelect());
}
/////////////////////////////////////////
//
//crSetGroundDetectMethod
//
/////////////////////////////////////////
crSetGroundDetectMethod::crSetGroundDetectMethod(){}
crSetGroundDetectMethod::crSetGroundDetectMethod(const crSetGroundDetectMethod& handle):
crMethod(handle)
{
}
void crSetGroundDetectMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkbox = (CRGUI::crCheckBoxElement*)param;
}

void crSetGroundDetectMethod::addParam(int i, const std::string& str)
{
}

void crSetGroundDetectMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	cameraBot->setGroundDetect(m_checkbox->getSelect());
}
/////////////////////////////////////////
//
//crModifyPostProcessAttrMethod
//
/////////////////////////////////////////
crModifyPostProcessAttrMethod::crModifyPostProcessAttrMethod(){}
crModifyPostProcessAttrMethod::crModifyPostProcessAttrMethod(const crModifyPostProcessAttrMethod& handle):
crMethod(handle),
m_ambientIntensityElement(handle.m_ambientIntensityElement),
m_ambientIntensityRElement(handle.m_ambientIntensityRElement),
m_ambientIntensityGElement(handle.m_ambientIntensityGElement),
m_ambientIntensityBElement(handle.m_ambientIntensityBElement),
m_brightnessElement(handle.m_brightnessElement),
m_enableFogElement(handle.m_enableFogElement),
m_fogColorElement(handle.m_fogColorElement),
m_fogColorRElement(handle.m_fogColorRElement),
m_fogColorGElement(handle.m_fogColorGElement),
m_fogColorBElement(handle.m_fogColorBElement),
m_fogColorStartElement(handle.m_fogColorStartElement),
m_fogColorEndElement(handle.m_fogColorEndElement),
m_fogColorDensityElement(handle.m_fogColorDensityElement),
m_fogColorHeightElement(handle.m_fogColorHeightElement),
m_maxDepthElement(handle.m_maxDepthElement),
m_nearClipElement(handle.m_nearClipElement),
m_farClipElement(handle.m_farClipElement),
//m_enableMotionBlurElement(handle.m_enableMotionBlurElement),
m_motionBlurFactorElement(handle.m_motionBlurFactorElement),
//m_hdrElement(handle.m_hdrElement),
//m_hdrParam1Element(handle.m_hdrParam1Element),
//m_hdrParam2Element(handle.m_hdrParam2Element),
//m_hdrParam3Element(handle.m_hdrParam3Element),
//m_hdrParam4Element(handle.m_hdrParam4Element),
//m_glowSizeElement(handle.m_glowSizeElement),
//m_enableDOFElement(handle.m_enableDOFElement),
m_dofBlurStartElement(handle.m_dofBlurStartElement),
m_dofBlurFactorElement(handle.m_dofBlurFactorElement),
m_upperSkyColorElement(handle.m_upperSkyColorElement),
m_upperSkyColorRElement(handle.m_upperSkyColorRElement),
m_upperSkyColorGElement(handle.m_upperSkyColorGElement),
m_upperSkyColorBElement(handle.m_upperSkyColorBElement),
m_upperSkyColorLumElement(handle.m_upperSkyColorLumElement),
m_lowerSkyColorElement(handle.m_lowerSkyColorElement),
m_lowerSkyColorRElement(handle.m_lowerSkyColorRElement),
m_lowerSkyColorGElement(handle.m_lowerSkyColorGElement),
m_lowerSkyColorBElement(handle.m_lowerSkyColorBElement),
m_lowerSkyColorLumElement(handle.m_lowerSkyColorLumElement),
m_staticMeshShadowElement(handle.m_staticMeshShadowElement)
{
}
void crModifyPostProcessAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyPostProcessAttrMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_ambientIntensityElement = str;
		break;
	case 1:
		m_ambientIntensityRElement = str;
		break;
	case 2:
		m_ambientIntensityGElement = str;
		break;
	case 3:
		m_ambientIntensityBElement = str;
		break;
	case 4:
		m_brightnessElement = str;
		break;
	case 5:
		m_enableFogElement = str;
		break;
	case 6:
		m_fogColorElement = str;
		break;
	case 7:
		m_fogColorRElement = str;
		break;
	case 8:
		m_fogColorGElement = str;
		break;
	case 9:
		m_fogColorBElement = str;
		break;
	case 10:
		m_fogColorStartElement = str;
		break;
	case 11:
		m_fogColorEndElement = str;
		break;
	case 12:
		m_fogColorDensityElement = str;
		break;
	case 13:
		m_fogColorHeightElement = str;
		break;
	case 14:
		m_maxDepthElement = str;
		break;
	case 15:
		m_nearClipElement = str;
		break;
	case 16:
		m_farClipElement = str;
		break;
	//case 16:
	//	m_enableMotionBlurElement = str;
	//	break;
	case 17:
		m_motionBlurFactorElement = str;
		break;
	//case 18:
	//	m_hdrElement = str;
	//	break;
	//case 19:
	//	m_hdrParam1Element = str;
	//	break;
	//case 20:
	//	m_hdrParam2Element = str;
	//	break;
	//case 21:
	//	m_hdrParam3Element = str;
	//	break;
	//case 22:
	//	m_hdrParam4Element = str;
	//	break;
	//case 23:
	//	m_glowSizeElement = str;
	//	break;
	//case 24:
	//	m_enableDOFElement = str;
	//	break;
	case 18:
		m_dofBlurStartElement = str;
		break;
	case 19:
		m_dofBlurFactorElement = str;
		break;
	case 20:
		m_upperSkyColorElement = str;
		break;
	case 21:
		m_upperSkyColorRElement = str;
		break;
	case 22:
		m_upperSkyColorGElement = str;
		break;
	case 23:
		m_upperSkyColorBElement = str;
		break;
	case 24:
		m_upperSkyColorLumElement = str;
		break;
	case 25:
		m_lowerSkyColorElement = str;
		break;
	case 26:
		m_lowerSkyColorRElement = str;
		break;
	case 27:
		m_lowerSkyColorGElement = str;
		break;
	case 28:
		m_lowerSkyColorBElement = str;
		break;
	case 29:
		m_lowerSkyColorLumElement = str;
		break;
	case 30:
		m_staticMeshShadowElement = str;
		break;
	}
}

void crModifyPostProcessAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crBrain *brain = crBrain::getInstance();
		crVector4 ambientIntensity;
        ambientIntensity[3] = 1.0f;
		//m_ambientIntensityElement
		crEditBoxElement *ambientIntensityRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientIntensityRElement));
		if(ambientIntensityRElement)
		{
			ambientIntensity[0] = atof(ambientIntensityRElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *ambientIntensityGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientIntensityGElement));
		if(ambientIntensityGElement)
		{
			ambientIntensity[1] = atof(ambientIntensityGElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *ambientIntensityBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_ambientIntensityBElement));
		if(ambientIntensityBElement)
		{
			ambientIntensity[2] = atof(ambientIntensityBElement->getStringArrayInString().c_str()) / 255.0f;
		}
		brain->setAmbientIntensity(ambientIntensity);
		crEditBoxElement *brightnessElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_brightnessElement));
		if(brightnessElement)
		{
			brain->setBrightness(atof(brightnessElement->getStringArrayInString().c_str()));
		}
		crCheckBoxElement *enableFogElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableFogElement));
		if(enableFogElement)
		{
			brain->setFog(enableFogElement->getSelect());
		}
		crVector3 fogColor;
		//m_fogColorElement
		crEditBoxElement *fogColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorRElement));
		if(fogColorRElement)
		{
			fogColor[0] = atof(fogColorRElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *fogColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorGElement));
		if(fogColorGElement)
		{
			fogColor[1] = atof(fogColorGElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *fogColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorBElement));
		if(fogColorBElement)
		{
			fogColor[2] = atof(fogColorBElement->getStringArrayInString().c_str()) / 255.0f;
		}
		brain->setFogColor(fogColor);
		crVector4 fogParams;
		crEditBoxElement *fogColorStartElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorStartElement));
		if(fogColorStartElement)
		{
			fogParams[0] = atof(fogColorStartElement->getStringArrayInString().c_str());
		}
		crEditBoxElement *fogColorEndElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorEndElement));
		if(fogColorEndElement)
		{
			fogParams[1] = atof(fogColorEndElement->getStringArrayInString().c_str());
		}
		crEditBoxElement *fogColorDensityElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorDensityElement));
		if(fogColorDensityElement)
		{
			fogParams[2] = atof(fogColorDensityElement->getStringArrayInString().c_str());
		}
		crEditBoxElement *fogColorHeightElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_fogColorHeightElement));
		if(fogColorHeightElement)
		{
			fogParams[3] = atof(fogColorHeightElement->getStringArrayInString().c_str());
		}
		brain->setFogParams(fogParams);
		crEditBoxElement *maxDepthElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_maxDepthElement));
		if(maxDepthElement)
		{
			brain->setMaxDepth(atof(maxDepthElement->getStringArrayInString().c_str()));
		}
		crVector2 clip;
		crEditBoxElement *nearClipElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_nearClipElement));
		if(nearClipElement)
		{
			clip[0] = atof(nearClipElement->getStringArrayInString().c_str());
		}
		crEditBoxElement *farClipElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_farClipElement));
		if(farClipElement)
		{
			clip[1] = atof(farClipElement->getStringArrayInString().c_str());
		}
		bool needresize = false;
		if(brain->getClip() != clip)
		{
			needresize = true;
		    brain->setClip(clip);
		}
		//crCheckBoxElement *enableMotionBlurElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableMotionBlurElement));
		//if(enableMotionBlurElement)
		//{
		//	ds->setMotionBlur(enableMotionBlurElement->getSelect());
		//}
		crEditBoxElement *motionBlurFactorElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_motionBlurFactorElement));
		if(motionBlurFactorElement)
		{
			brain->setMotionBlurFactor(atof(motionBlurFactorElement->getStringArrayInString().c_str()));
		}
		//crEditBoxElement *hdrElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_hdrElement));
		//if(hdrElement)
		//{
		//	ds->setHDR(atoi(hdrElement->getStringArrayInString().c_str()));
		//}
		//crVector4 hdrParam;
		//crEditBoxElement *hdrParam1Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_hdrParam1Element));
		//if(hdrParam1Element)
		//{
		//	hdrParam[0] = atof(hdrParam1Element->getStringArrayInString().c_str());
		//}
		//crEditBoxElement *hdrParam2Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_hdrParam2Element));
		//if(hdrParam2Element)
		//{
		//	hdrParam[1] = atof(hdrParam2Element->getStringArrayInString().c_str());
		//}
		//crEditBoxElement *hdrParam3Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_hdrParam3Element));
		//if(hdrParam3Element)
		//{
		//	hdrParam[2] = atof(hdrParam3Element->getStringArrayInString().c_str());
		//}
		//crEditBoxElement *hdrParam4Element = dynamic_cast<crEditBoxElement *>(stage->getElement(m_hdrParam4Element));
		//if(hdrParam4Element)
		//{
		//	hdrParam[3] = atof(hdrParam4Element->getStringArrayInString().c_str());
		//}
		//ds->setHdrParam(hdrParam);
		//crEditBoxElement *glowSizeElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_glowSizeElement));
		//if(glowSizeElement)
		//{
		//	ds->setGlowSize(atof(glowSizeElement->getStringArrayInString().c_str()));
		//}
		//crCheckBoxElement *enableDOFElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_enableDOFElement));
		//if(enableDOFElement)
		//{
		//	ds->setDepthOfField(enableDOFElement->getSelect());
		//}
		crEditBoxElement *dofBlurStartElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_dofBlurStartElement));
		if(dofBlurStartElement)
		{
			brain->setDofBlurStart(atof(dofBlurStartElement->getStringArrayInString().c_str()));
		}
		crEditBoxElement *dofBlurFactorElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_dofBlurFactorElement));
		if(dofBlurFactorElement)
		{
			brain->setDofBlurFactor(atof(dofBlurFactorElement->getStringArrayInString().c_str()));
		}

		crVector3 upperSkyColor;
		//m_fogColorElement
		crEditBoxElement *upperSkyColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_upperSkyColorRElement));
		if(upperSkyColorRElement)
		{
			upperSkyColor[0] = atof(upperSkyColorRElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *upperSkyColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_upperSkyColorGElement));
		if(upperSkyColorGElement)
		{
			upperSkyColor[1] = atof(upperSkyColorGElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *upperSkyColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_upperSkyColorBElement));
		if(upperSkyColorBElement)
		{
			upperSkyColor[2] = atof(upperSkyColorBElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *upperSkyColorLumElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_upperSkyColorLumElement));
		if(upperSkyColorLumElement)
		{
			upperSkyColor *= atof(upperSkyColorLumElement->getStringArrayInString().c_str());
		}
		brain->setUpperSkyColor(upperSkyColor);

		crVector3 lowerSkyColor;
		//m_fogColorElement
		crEditBoxElement *lowerSkyColorRElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lowerSkyColorRElement));
		if(lowerSkyColorRElement)
		{
			lowerSkyColor[0] = atof(lowerSkyColorRElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *lowerSkyColorGElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lowerSkyColorGElement));
		if(lowerSkyColorGElement)
		{
			lowerSkyColor[1] = atof(lowerSkyColorGElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *lowerSkyColorBElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lowerSkyColorBElement));
		if(lowerSkyColorBElement)
		{
			lowerSkyColor[2] = atof(lowerSkyColorBElement->getStringArrayInString().c_str()) / 255.0f;
		}
		crEditBoxElement *lowerSkyColorLumElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_lowerSkyColorLumElement));
		if(lowerSkyColorLumElement)
		{
			lowerSkyColor *= atof(lowerSkyColorLumElement->getStringArrayInString().c_str());
		}
		brain->setLowerSkyColor(lowerSkyColor);

		CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!bindview || !bindview->isInited())
			return;
		crSceneView *sv = bindview->getDefaultSceneHandler()->getSceneView();
		assert(sv);
		CRCore::crStateSet* globalStateSet = sv->getGlobalStateSet();
		crLightModel *lightModel = dynamic_cast<crLightModel *>(globalStateSet->getAttribute(crStateAttribute::LIGHTMODEL));
		float brightness = brain->getBrightness() + crDisplaySettings::instance()->getBrightness();
		lightModel->setAmbientIntensity(brain->getAmbientIntensity() + crVector4(brightness,brightness,brightness,0.0f));

		crUniform *uniform = globalStateSet->getOrCreateUniform("maxDepth",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(brain->getMaxDepth());
		if(brain->getFog()==1)
		{
			uniform = globalStateSet->getOrCreateUniform("fog_color",CRCore::crUniform::FLOAT_VEC3);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(brain->getFogColor());

			uniform = globalStateSet->getOrCreateUniform("fog_params",CRCore::crUniform::FLOAT_VEC4);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(brain->getFogParams());
		}
		
		uniform = globalStateSet->getOrCreateUniform("UpperSkyColor",CRCore::crUniform::FLOAT_VEC3);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crBrain::getInstance()->getUpperSkyColor());
		uniform = globalStateSet->getOrCreateUniform("LowerSkyColor",CRCore::crUniform::FLOAT_VEC3);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(crBrain::getInstance()->getLowerSkyColor());
		//if(needresize)
		//	bindview->realize();

		crCheckBoxElement *staticMeshShadowElement = dynamic_cast<crCheckBoxElement *>(stage->getElement(m_staticMeshShadowElement));
		if(staticMeshShadowElement)
		{
			brain->setCalcStaticMeshShadow(!staticMeshShadowElement->getSelect());
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("Static");
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			crGroup *group = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			group->setCalcShadow(brain->getCalcStaticMeshShadow());
		}
	}
}

/////////////////////////////////////////
//
//crDisplayMultiSwitchAttrMethod
//
/////////////////////////////////////////
crDisplayMultiSwitchAttrMethod::crDisplayMultiSwitchAttrMethod(){}
crDisplayMultiSwitchAttrMethod::crDisplayMultiSwitchAttrMethod(const crDisplayMultiSwitchAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_maskCountElement(handle.m_maskCountElement),
m_currentMaskElement(handle.m_currentMaskElement),
m_childGridElement(handle.m_childGridElement)
{
}
void crDisplayMultiSwitchAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayMultiSwitchAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_maskCountElement = str;
		break;
	case 2:
		m_currentMaskElement = str;
		break;
	case 3:
		m_childGridElement = str;
		break;
	}
}

void crDisplayMultiSwitchAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);

		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
            currentSelectElement->clearString();
		}
		crStaticTextBoxElement *maskCount = dynamic_cast<crStaticTextBoxElement *>(m_imageStage->getElement(m_maskCountElement));
		if(maskCount)
		{
			maskCount->clearString();
		}
		crEditBoxElement *currentMask = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentMaskElement));
		if(currentMask)
		{
			currentMask->clearString();
		}
		crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_childGridElement));
		if(keyGridElement)
		{
			keyGridElement->clearData();
			keyGridElement->checkSelect();
		}

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
			if(multiSwitchNode)
			{
				std::string str;
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					multiSwitchNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}

				if(maskCount)
				{
					maskCount->setStringArrayByString(crArgumentParser::appItoa(multiSwitchNode->getSwitchSetList().size()));
				}
				if(currentMask)
				{
					currentMask->setStringArrayByString(crArgumentParser::appItoa(multiSwitchNode->getActiveSwitchSet()));
				}
                
				if(keyGridElement)
				{
					const crMultiSwitch::ValueList& valueList = multiSwitchNode->getValueList(multiSwitchNode->getActiveSwitchSet());
					crNode *child;
					for( int i = 0; i<valueList.size(); ++i )
					{
                        child = multiSwitchNode->getChild(i);
						keyGridElement->addData(child->getName(),crArgumentParser::appItoa(valueList[i]?1:0),"");
					}
					keyGridElement->checkSelect();
					keyGridElement->updateInputElement(true);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSetActiveSwitchSetMethod
//
/////////////////////////////////////////
crSetActiveSwitchSetMethod::crSetActiveSwitchSetMethod(){}
crSetActiveSwitchSetMethod::crSetActiveSwitchSetMethod(const crSetActiveSwitchSetMethod& handle):
crMethod(handle)
{
}
void crSetActiveSwitchSetMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_editbox = (CRGUI::crEditBoxElement*)param;
}

void crSetActiveSwitchSetMethod::addParam(int i, const std::string& str)
{
}

void crSetActiveSwitchSetMethod::operator()(crHandle &handle)
{
	if(m_editbox.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
			if(multiSwitchNode)
			{
				multiSwitchNode->setActiveSwitchSet(atoi(m_editbox->getStringArrayInString().c_str()));
			}
		}
	}
}
/////////////////////////////////////////
//
//crMultiSwitchPreMaskMethod
//
/////////////////////////////////////////
crMultiSwitchPreMaskMethod::crMultiSwitchPreMaskMethod(){}
crMultiSwitchPreMaskMethod::crMultiSwitchPreMaskMethod(const crMultiSwitchPreMaskMethod& handle):
crMethod(handle)
{
}
void crMultiSwitchPreMaskMethod::inputParam(int i, void *param)
{
}

void crMultiSwitchPreMaskMethod::addParam(int i, const std::string& str)
{
}

void crMultiSwitchPreMaskMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
            int currentMask = multiSwitchNode->getActiveSwitchSet();
			if(currentMask>0)
				currentMask--;
			multiSwitchNode->setActiveSwitchSet(currentMask);
		}
	}
}
/////////////////////////////////////////
//
//crMultiSwitchNextMaskMethod
//
/////////////////////////////////////////
crMultiSwitchNextMaskMethod::crMultiSwitchNextMaskMethod(){}
crMultiSwitchNextMaskMethod::crMultiSwitchNextMaskMethod(const crMultiSwitchNextMaskMethod& handle):
crMethod(handle)
{
}
void crMultiSwitchNextMaskMethod::inputParam(int i, void *param)
{
}

void crMultiSwitchNextMaskMethod::addParam(int i, const std::string& str)
{
}

void crMultiSwitchNextMaskMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
			int currentMask = multiSwitchNode->getActiveSwitchSet();
			if(currentMask<multiSwitchNode->getSwitchSetList().size()-1)
				currentMask++;
			multiSwitchNode->setActiveSwitchSet(currentMask);
		}
	}
}
/////////////////////////////////////////
//
//crMultiSwitchAllOnMethod
//
/////////////////////////////////////////
crMultiSwitchAllOnMethod::crMultiSwitchAllOnMethod(){}
crMultiSwitchAllOnMethod::crMultiSwitchAllOnMethod(const crMultiSwitchAllOnMethod& handle):
crMethod(handle)
{
}
void crMultiSwitchAllOnMethod::inputParam(int i, void *param)
{
}

void crMultiSwitchAllOnMethod::addParam(int i, const std::string& str)
{
}

void crMultiSwitchAllOnMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
			multiSwitchNode->setAllChildrenOn(multiSwitchNode->getActiveSwitchSet());
		}
	}
}
/////////////////////////////////////////
//
//crMultiSwitchAllOffMethod
//
/////////////////////////////////////////
crMultiSwitchAllOffMethod::crMultiSwitchAllOffMethod(){}
crMultiSwitchAllOffMethod::crMultiSwitchAllOffMethod(const crMultiSwitchAllOffMethod& handle):
crMethod(handle)
{
}
void crMultiSwitchAllOffMethod::inputParam(int i, void *param)
{
}

void crMultiSwitchAllOffMethod::addParam(int i, const std::string& str)
{
}

void crMultiSwitchAllOffMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
			multiSwitchNode->setAllChildrenOff(multiSwitchNode->getActiveSwitchSet());
		}
	}
}

/////////////////////////////////////////
//
//crPickChildMethod
//
/////////////////////////////////////////
crPickChildMethod::crPickChildMethod():
m_selectType(0){}
crPickChildMethod::crPickChildMethod(const crPickChildMethod& handle):
crMethod(handle),
m_selectType(handle.m_selectType)
{
}
void crPickChildMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crPickChildMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_selectType = atoi(str.c_str());
		break;
	}
}

void crPickChildMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
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

		if(m_checkBox->getSelect())
		{
			if(m_selectType == 0 || selectType == m_selectType)
			{
				selectType = -2;
				infoData->inputParam(0,&selectType);
			}
		}
		else
		{
			if(selectType == -2)
			{
				selectType = m_selectType;
				infoData->inputParam(0,&selectType);
			}
		}
	}
}
/////////////////////////////////////////
//
//crAddChildMethod
//
/////////////////////////////////////////
crAddChildMethod::crAddChildMethod(){}
crAddChildMethod::crAddChildMethod(const crAddChildMethod& handle):
crMethod(handle)
{
}
void crAddChildMethod::inputParam(int i, void *param)
{
}

void crAddChildMethod::addParam(int i, const std::string& str)
{
}

void crAddChildMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crGroup *group = dynamic_cast<crGroup *>(selectNodeVec[0].get());
		if(group)
		{
			crGroup *newChild = new crGroup;
			newChild->setName("NewChild");
			group->addChild(newChild);
		}
	}
}
/////////////////////////////////////////
//
//crRemoveChildMethod
//
/////////////////////////////////////////
crRemoveChildMethod::crRemoveChildMethod(){}
crRemoveChildMethod::crRemoveChildMethod(const crRemoveChildMethod& handle):
crMethod(handle),
m_childGridElement(handle.m_childGridElement)
{
}
void crRemoveChildMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crCheckBoxElement*)param;
}

void crRemoveChildMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_childGridElement = str;
		break;
	}
}

void crRemoveChildMethod::operator()(crHandle &handle)
{//如果是删除PageLod的子节点则需要先完全加载
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *group = dynamic_cast<crGroup *>(selectNodeVec[0].get());
			if(group)
			{
				std::string str;
				crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_childGridElement));
				if(keyGridElement)
				{
					group->removeChild(keyGridElement->getSelect());
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRemovePagedLodChildMethod
//
/////////////////////////////////////////
crRemovePagedLodChildMethod::crRemovePagedLodChildMethod(){}
crRemovePagedLodChildMethod::crRemovePagedLodChildMethod(const crRemovePagedLodChildMethod& handle):
crMethod(handle),
m_childGridElement(handle.m_childGridElement)
{
}
void crRemovePagedLodChildMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crCheckBoxElement*)param;
}

void crRemovePagedLodChildMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_childGridElement = str;
		break;
	}
}

void crRemovePagedLodChildMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *group = dynamic_cast<crGroup *>(selectNodeVec[0].get());
			if(group)
			{
				std::string str;
				crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_childGridElement));
				if(keyGridElement)
				{
					int i = keyGridElement->getSelect();
					i = floor(float(i+1) * 0.5f);
					group->removeChild(i);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyMultiSwitchAttrMethod
//
/////////////////////////////////////////
crModifyMultiSwitchAttrMethod::crModifyMultiSwitchAttrMethod(){}
crModifyMultiSwitchAttrMethod::crModifyMultiSwitchAttrMethod(const crModifyMultiSwitchAttrMethod& handle):
crMethod(handle),
m_childGridElement(handle.m_childGridElement)
{
}
void crModifyMultiSwitchAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyMultiSwitchAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_childGridElement = str;
		break;
	}
}

void crModifyMultiSwitchAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
			if(multiSwitchNode)
			{
				std::string str;
				crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_childGridElement));
				if(keyGridElement)
				{
					//crKeyValueGridElement::DataPair dataPair;
					ref_ptr<crKeyValueNode> keyValueNode;
					crMultiSwitch::ValueList valueList = multiSwitchNode->getValueList(multiSwitchNode->getActiveSwitchSet());
					int gridIndex = 0;
					for( int i = 0; i<valueList.size(); ++i )
					{
						keyValueNode = keyGridElement->getData(gridIndex++/*,dataPair*/);
						valueList[i] = atoi(keyValueNode->getValue().c_str());
					}
					multiSwitchNode->setValueList(multiSwitchNode->getActiveSwitchSet(),valueList);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crAddSwitchSetMethod
//
/////////////////////////////////////////
crAddSwitchSetMethod::crAddSwitchSetMethod(){}
crAddSwitchSetMethod::crAddSwitchSetMethod(const crAddSwitchSetMethod& handle):
crMethod(handle)
{
}
void crAddSwitchSetMethod::inputParam(int i, void *param)
{
}

void crAddSwitchSetMethod::addParam(int i, const std::string& str)
{
}

void crAddSwitchSetMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
			multiSwitchNode->setAllChildrenOn(multiSwitchNode->getSwitchSetList().size());
		}
	}
}
/////////////////////////////////////////
//
//crDelSwitchSetMethod
//
/////////////////////////////////////////
crDelSwitchSetMethod::crDelSwitchSetMethod(){}
crDelSwitchSetMethod::crDelSwitchSetMethod(const crDelSwitchSetMethod& handle):
crMethod(handle)
{
}
void crDelSwitchSetMethod::inputParam(int i, void *param)
{
}

void crDelSwitchSetMethod::addParam(int i, const std::string& str)
{
}

void crDelSwitchSetMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crMultiSwitch *multiSwitchNode = dynamic_cast<crMultiSwitch *>(selectNodeVec[0].get());
		if(multiSwitchNode)
		{
			multiSwitchNode->removeSwitchSet(multiSwitchNode->getActiveSwitchSet());
		}
	}
}

/////////////////////////////////////////
//
//crUpdateNodeListMethod
//
/////////////////////////////////////////
crUpdateNodeListMethod::crUpdateNodeListMethod(){}
crUpdateNodeListMethod::crUpdateNodeListMethod(const crUpdateNodeListMethod& handle):
crMethod(handle),
m_nodeClassName(handle.m_nodeClassName)
{
}
void crUpdateNodeListMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateNodeListMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeClassName = str;
		break;
	}
}

void crUpdateNodeListMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crCollectNodeBYClassNameVisitor collectNodeByClassName;
		collectNodeByClassName.insertClassNameId(m_nodeClassName);
		if(m_nodeClassName.compare("crSequence") == 0)
			collectNodeByClassName.insertClassNameId("crMatrixSequence");
		crBrain::getInstance()->accept(collectNodeByClassName);
		CRCore::NodeArray &nodeArray = collectNodeByClassName.getResult();
		m_listbox->clearData();
		if(!nodeArray.empty())
		{
			//std::set<std::string> nodeNameSet;
			//CRCore::NodeArray::iterator itr = nodeArray.begin();
			//nodeNameSet.insert((*itr)->getName());
			//m_listbox->addData((*itr)->getName());
			//itr++;
			//std::string nodename;
			//int i = 0;
			//for(; itr != nodeArray.end(); ++itr )
			//{
			//	nodename = (*itr)->getName();
			//	if(nodeNameSet.find(nodename) != nodeNameSet.end())
			//	{
			//		nodename += crArgumentParser::appItoa(i++);
			//		(*itr)->setName(nodename);
			//	}
			//	m_listbox->addData(nodename);
			//}
			for( CRCore::NodeArray::iterator itr = nodeArray.begin();
				itr != nodeArray.end();
				++itr )
			{
				m_listbox->addData((*itr)->getName());
			}
		}
		m_listbox->checkSelect();
	}
}

/////////////////////////////////////////
//
//crSelectNodeMethod
//
/////////////////////////////////////////
crSelectNodeMethod::crSelectNodeMethod(){}
crSelectNodeMethod::crSelectNodeMethod(const crSelectNodeMethod& handle):
crMethod(handle),
m_nodeClassName(handle.m_nodeClassName)
{
}
void crSelectNodeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crSelectNodeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeClassName = str;
		break;
	}
}

void crSelectNodeMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		int selid = m_listbox->getSelect();
		crCollectNodeBYClassNameVisitor collectNodeByClassName;
		collectNodeByClassName.insertClassNameId(m_nodeClassName);
		if(m_nodeClassName.compare("crSequence") == 0)
			collectNodeByClassName.insertClassNameId("crMatrixSequence");
		crBrain::getInstance()->accept(collectNodeByClassName);
		CRCore::NodeArray &nodeArray = collectNodeByClassName.getResult();
		if(selid<nodeArray.size())
		{
			crNode *node = nodeArray[selid].get();
			gamebodyInfo->unSelectAll();
			ref_ptr<crData> data = node->getDataClass();
			if(!data.valid())
			{
				data = crDataManager::getInstance()->getData("Event");
				node->setDataClass(data.get());
			}
			if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
			{
				crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
				driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
				data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
				data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
			}
			gamebodyInfo->selectNode(node);
		}
		//std::string selectNodeName;
		//m_listbox->getData(m_listbox->getSelect(),selectNodeName);
		//if(!gamebodyInfo->isNodeSelected(selectNodeName))
		//{
		//	gamebodyInfo->unSelectAll();
		//	crData *infoData = gamebodyInfo->getDataClass();
		//	assert(infoData);

		//	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		//	searchByNameVisitor.setNameId(selectNodeName);
		//	SearchNodeType stype = ALLNODE;
		//	if(m_nodeClassName.compare("crGroup") == 0)
		//	{
		//		stype = GROUP;
		//	}
		//	else if(m_nodeClassName.compare("crLightSource") == 0)
		//	{
		//		stype = LIGHTSOURCE;
		//	}
		//	else if(m_nodeClassName.compare("crEventGroup") == 0)
		//	{
		//		stype = EVENTGROUP;
		//	}
		//	else if(m_nodeClassName.compare("crTransform") == 0)
		//	{
		//		stype = TRANSFORM;
		//	}
		//	else if(m_nodeClassName.compare("crMatrixTransform") == 0)
		//	{
		//		stype = MATRIXTRANSFORM;
		//	}
		//	else if(m_nodeClassName.compare("crObject") == 0)
		//	{
		//		stype = OBJECT;
		//	}
		//	else if(m_nodeClassName.compare("crNode") == 0)
		//	{
		//		stype = NODE;
		//	}
		//	else if(m_nodeClassName.compare("crMultiSwitch") == 0)
		//	{
		//		stype = MULTISWITCH;
		//	}
		//	else if(m_nodeClassName.compare("crLod") == 0)
		//	{
		//		stype = LOD;
		//	}
		//	else if(m_nodeClassName.compare("crPagedLOD") == 0)
		//	{
		//		stype = PAGEDLOD;
		//	}
		//	else if(m_nodeClassName.compare("crSequence") == 0)
		//	{
		//		stype = SEQUENCE;
		//	}
		//	searchByNameVisitor.setSearchNodeType(stype);
		//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		//	crNode* node = searchByNameVisitor.getResult();
		//	if(node)
		//	{
		//		ref_ptr<crData> data = node->getDataClass();
		//		if(!data.valid())
		//		{
		//			data = crDataManager::getInstance()->getData("Event");
		//			node->setDataClass(data.get());
		//		}
		//		if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
		//		{
		//			crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
		//			driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
		//			data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
		//			data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
		//		}
		//		gamebodyInfo->selectNode(node);
		//	}
		//}
	}
}
/////////////////////////////////////////
//
//crDisplayLodAttrMethod
//
/////////////////////////////////////////
crDisplayLodAttrMethod::crDisplayLodAttrMethod(){}
crDisplayLodAttrMethod::crDisplayLodAttrMethod(const crDisplayLodAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_centerModeElement(handle.m_centerModeElement),
m_centerXElement(handle.m_centerXElement),
m_centerYElement(handle.m_centerYElement),
m_centerZElement(handle.m_centerZElement),
m_radiusElement(handle.m_radiusElement),
m_childGridElement(handle.m_childGridElement)
{
}
void crDisplayLodAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayLodAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_centerModeElement = str;
		break;
	case 2:
		m_centerXElement = str;
		break;
	case 3:
		m_centerYElement = str;
		break;
	case 4:
		m_centerZElement = str;
		break;
	case 5:
		m_radiusElement = str;
		break;
	case 6:
		m_childGridElement = str;
		break;
	}
}

void crDisplayLodAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);

		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
			currentSelectElement->clearString();
		}
		crRadioGroupElement *centerMode = dynamic_cast<crRadioGroupElement *>(m_imageStage->getElement(m_centerModeElement));
		if(centerMode)
		{
			centerMode->select(0);
		}
		crEditBoxElement *centerX = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerXElement));
		if(centerX)
		{
			centerX->clearString();
		}
		crEditBoxElement *centerY = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerYElement));
		if(centerY)
		{
			centerY->clearString();
		}
		crEditBoxElement *centerZ = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerZElement));
		if(centerZ)
		{
			centerZ->clearString();
		}
		crEditBoxElement *radius = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_radiusElement));
		if(radius)
		{
			radius->clearString();
		}
		crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_childGridElement));
		if(keyGridElement)
		{
			keyGridElement->clearData();
			keyGridElement->checkSelect();
		}

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crLod *lod = dynamic_cast<crLod *>(selectNodeVec[0].get());
			if(lod && strcmp(lod->className(),"crLod")==0)
			{
				std::string str;
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					lod->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}

				if(lod->getCenterMode() == crLod::USER_DEFINED_CENTER)
				{
					if(centerMode)
					{
                        centerMode->select(1);
					}
					if(centerX) centerX->setEnable(true);
					if(centerY) centerY->setEnable(true);
					if(centerZ) centerZ->setEnable(true);
					if(radius) radius->setEnable(true);
				}
				else
				{
					if(centerX) centerX->setEnable(false);
					if(centerY) centerY->setEnable(false);
					if(centerZ) centerZ->setEnable(false);
					if(radius) radius->setEnable(false);
				}
				const crVector3f& center = lod->getCenter();
				if(centerX)
				{
					centerX->setStringArrayByString(crArgumentParser::appFtoa(center[0]));
				}
				if(centerY)
				{
					centerY->setStringArrayByString(crArgumentParser::appFtoa(center[1]));
				}
				if(centerZ)
				{
					centerZ->setStringArrayByString(crArgumentParser::appFtoa(center[2]));
				}
				if(radius)
				{
					radius->setStringArrayByString(crArgumentParser::appFtoa(lod->getRadius()));
				}
				if(keyGridElement)
				{
					const crLod::RangeList& rangeList = lod->getRangeList();
					crNode *child;
					for( int i = 0; i<rangeList.size(); ++i )
					{
						child = lod->getChild(i);
						keyGridElement->addData(child->getName(),crArgumentParser::appVectoa(crVector2(rangeList[i].first,rangeList[i].second)),"");
					}
					keyGridElement->checkSelect();
					keyGridElement->updateInputElement(true);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyLodAttrMethod
//
/////////////////////////////////////////
crModifyLodAttrMethod::crModifyLodAttrMethod(){}
crModifyLodAttrMethod::crModifyLodAttrMethod(const crModifyLodAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_centerModeElement(handle.m_centerModeElement),
m_centerXElement(handle.m_centerXElement),
m_centerYElement(handle.m_centerYElement),
m_centerZElement(handle.m_centerZElement),
m_radiusElement(handle.m_radiusElement),
m_childGridElement(handle.m_childGridElement)
{
}
void crModifyLodAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyLodAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_centerModeElement = str;
		break;
	case 2:
		m_centerXElement = str;
		break;
	case 3:
		m_centerYElement = str;
		break;
	case 4:
		m_centerZElement = str;
		break;
	case 5:
		m_radiusElement = str;
		break;
	case 6:
		m_childGridElement = str;
		break;
	}
}

void crModifyLodAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crLod *lod = dynamic_cast<crLod *>(selectNodeVec[0].get());
			if(lod && strcmp(lod->className(),"crLod")==0)
			{
				crRadioGroupElement *centerMode = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_centerModeElement));
				if(centerMode)
				{
					int select = centerMode->getSelect();
					if(select == 0)
					{
						lod->setCenterMode(crLod::USE_BOUNDING_SPHERE_CENTER);
					}
					else
					{
                        lod->setCenterMode(crLod::USER_DEFINED_CENTER);
						crVector3f center;
						crEditBoxElement *centerX = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerXElement));
						if(centerX)
						{
							center[0] = atof(centerX->getStringArrayInString().c_str());
						}
						crEditBoxElement *centerY = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerYElement));
						if(centerY)
						{
							center[1] = atof(centerY->getStringArrayInString().c_str());
						}
						crEditBoxElement *centerZ = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerZElement));
						if(centerZ)
						{
							center[2] = atof(centerZ->getStringArrayInString().c_str());
						}
					    lod->setCenter(center);
						crEditBoxElement *radius = dynamic_cast<crEditBoxElement *>(stage->getElement(m_radiusElement));
						if(radius)
						{
                            lod->setRadius(atof(radius->getStringArrayInString().c_str()));
						}
					}
				}
				crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_childGridElement));
				if(keyGridElement)
				{
					//crKeyValueGridElement::DataPair dataPair;
					ref_ptr<crKeyValueNode> keyValueNode;
					const crLod::RangeList& rangeList = lod->getRangeList();
				    int gridIndex = 0;
					crVector2 range;
					for( int i = 0; i<rangeList.size(); ++i )
					{
						keyValueNode = keyGridElement->getData(gridIndex++/*,dataPair*/);
						crArgumentParser::appAtoVec(keyValueNode->getValue().c_str(),range);
						lod->setRange(i,range[0],range[1]);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisplayPagedLodAttrMethod
//
/////////////////////////////////////////
crDisplayPagedLodAttrMethod::crDisplayPagedLodAttrMethod(){}
crDisplayPagedLodAttrMethod::crDisplayPagedLodAttrMethod(const crDisplayPagedLodAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_centerModeElement(handle.m_centerModeElement),
m_centerXElement(handle.m_centerXElement),
m_centerYElement(handle.m_centerYElement),
m_centerZElement(handle.m_centerZElement),
m_radiusElement(handle.m_radiusElement),
m_childGridElement(handle.m_childGridElement)
{
}
void crDisplayPagedLodAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayPagedLodAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_centerModeElement = str;
		break;
	case 2:
		m_centerXElement = str;
		break;
	case 3:
		m_centerYElement = str;
		break;
	case 4:
		m_centerZElement = str;
		break;
	case 5:
		m_radiusElement = str;
		break;
	case 6:
		m_childGridElement = str;
		break;
	}
}

void crDisplayPagedLodAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);

		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
			currentSelectElement->clearString();
		}
		crRadioGroupElement *centerMode = dynamic_cast<crRadioGroupElement *>(m_imageStage->getElement(m_centerModeElement));
		if(centerMode)
		{
			centerMode->select(0);
		}
		crEditBoxElement *centerX = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerXElement));
		if(centerX)
		{
			centerX->clearString();
		}
		crEditBoxElement *centerY = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerYElement));
		if(centerY)
		{
			centerY->clearString();
		}
		crEditBoxElement *centerZ = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_centerZElement));
		if(centerZ)
		{
			centerZ->clearString();
		}
		crEditBoxElement *radius = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_radiusElement));
		if(radius)
		{
			radius->clearString();
		}
		crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_childGridElement));
		if(keyGridElement)
		{
			keyGridElement->clearData();
			keyGridElement->checkSelect();
		}

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crPagedLOD *lod = dynamic_cast<crPagedLOD *>(selectNodeVec[0].get());
			if(lod && strcmp(lod->className(),"crPagedLOD")==0)
			{
				std::string str;
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					lod->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}

				if(lod->getCenterMode() == crLod::USER_DEFINED_CENTER)
				{
					if(centerMode)
					{
						centerMode->select(1);
					}
					if(centerX) centerX->setEnable(true);
					if(centerY) centerY->setEnable(true);
					if(centerZ) centerZ->setEnable(true);
					if(radius) radius->setEnable(true);
				}
				else
				{
					if(centerX) centerX->setEnable(false);
					if(centerY) centerY->setEnable(false);
					if(centerZ) centerZ->setEnable(false);
					if(radius) radius->setEnable(false);
				}
				const crVector3f& center = lod->getCenter();
				if(centerX)
				{
					centerX->setStringArrayByString(crArgumentParser::appFtoa(center[0]));
				}
				if(centerY)
				{
					centerY->setStringArrayByString(crArgumentParser::appFtoa(center[1]));
				}
				if(centerZ)
				{
					centerZ->setStringArrayByString(crArgumentParser::appFtoa(center[2]));
				}
				if(radius)
				{
					radius->setStringArrayByString(crArgumentParser::appFtoa(lod->getRadius()));
				}
				if(keyGridElement)
				{
					crNode *child;
					const crLod::RangeList& rangeList = lod->getRangeList();
					int numChild = lod->getNumChildren();
					for( int i = 0; i<rangeList.size(); ++i )
					{
					    if(i<numChild)
						{
						    child = lod->getChild(i);
						    keyGridElement->addData(child->getName(),crArgumentParser::appVectoa(crVector2(rangeList[i].first,rangeList[i].second)),"");
						}
						else
						{
							keyGridElement->addData("NotLoaded",crArgumentParser::appVectoa(crVector2(rangeList[i].first,rangeList[i].second)),"");
						}
						keyGridElement->addData("FileName",lod->getFileName(i),"");
					}
					keyGridElement->checkSelect();
					keyGridElement->updateInputElement(true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crModifyPagedLodAttrMethod
//
/////////////////////////////////////////
crModifyPagedLodAttrMethod::crModifyPagedLodAttrMethod(){}
crModifyPagedLodAttrMethod::crModifyPagedLodAttrMethod(const crModifyPagedLodAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_centerModeElement(handle.m_centerModeElement),
m_centerXElement(handle.m_centerXElement),
m_centerYElement(handle.m_centerYElement),
m_centerZElement(handle.m_centerZElement),
m_radiusElement(handle.m_radiusElement),
m_childGridElement(handle.m_childGridElement)
{
}
void crModifyPagedLodAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyPagedLodAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_centerModeElement = str;
		break;
	case 2:
		m_centerXElement = str;
		break;
	case 3:
		m_centerYElement = str;
		break;
	case 4:
		m_centerZElement = str;
		break;
	case 5:
		m_radiusElement = str;
		break;
	case 6:
		m_childGridElement = str;
		break;
	}
}

void crModifyPagedLodAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;

		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(bodyInfo);
		//crData *infoData = bodyInfo->getDataClass();
		//assert(infoData);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crPagedLOD *lod = dynamic_cast<crPagedLOD *>(selectNodeVec[0].get());
			if(lod && strcmp(lod->className(),"crPagedLOD")==0)
			{
				crRadioGroupElement *centerMode = dynamic_cast<crRadioGroupElement *>(stage->getElement(m_centerModeElement));
				if(centerMode)
				{
					int select = centerMode->getSelect();
					if(select == 0)
					{
						lod->setCenterMode(crLod::USE_BOUNDING_SPHERE_CENTER);
					}
					else
					{
						lod->setCenterMode(crLod::USER_DEFINED_CENTER);
						crVector3f center;
						crEditBoxElement *centerX = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerXElement));
						if(centerX)
						{
							center[0] = atof(centerX->getStringArrayInString().c_str());
						}
						crEditBoxElement *centerY = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerYElement));
						if(centerY)
						{
							center[1] = atof(centerY->getStringArrayInString().c_str());
						}
						crEditBoxElement *centerZ = dynamic_cast<crEditBoxElement *>(stage->getElement(m_centerZElement));
						if(centerZ)
						{
							center[2] = atof(centerZ->getStringArrayInString().c_str());
						}
						lod->setCenter(center);
						crEditBoxElement *radius = dynamic_cast<crEditBoxElement *>(stage->getElement(m_radiusElement));
						if(radius)
						{
							lod->setRadius(atof(radius->getStringArrayInString().c_str()));
						}
					}
				}
				crKeyValueGridElement *keyGridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_childGridElement));
				if(keyGridElement)
				{
					//crKeyValueGridElement::DataPair dataPair;
					ref_ptr<crKeyValueNode> keyValueNode;
					const crLod::RangeList& rangeList = lod->getRangeList();
					int gridIndex = 0;
					crVector2 range;
					for( int i = 0; i<rangeList.size(); ++i )
					{
						keyValueNode = keyGridElement->getData(gridIndex++/*,dataPair*/);
						crArgumentParser::appAtoVec(keyValueNode->getValue().c_str(),range);
						lod->setRange(i,range[0],range[1]);
						keyValueNode = keyGridElement->getData(gridIndex++/*,dataPair*/);
						lod->setFileName(i,keyValueNode->getValue()/*dataPair.second*/);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crFullLoadPagedLodMethod
//
/////////////////////////////////////////
crFullLoadPagedLodMethod::crFullLoadPagedLodMethod(){}
crFullLoadPagedLodMethod::crFullLoadPagedLodMethod(const crFullLoadPagedLodMethod& handle):
crMethod(handle)
{
}
void crFullLoadPagedLodMethod::inputParam(int i, void *param)
{
}

void crFullLoadPagedLodMethod::addParam(int i, const std::string& str)
{
}

void crFullLoadPagedLodMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	assert(body);
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	assert(bodyInfo);
	//crData *infoData = bodyInfo->getDataClass();
	//assert(infoData);
	crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crPagedLOD *lod = dynamic_cast<crPagedLOD *>(selectNodeVec[0].get());
		if(lod && strcmp(lod->className(),"crPagedLOD")==0)
		{
			const crLod::RangeList& rangeList = lod->getRangeList();
			int numChild = lod->getNumChildren();
			for( int i = 0; i<rangeList.size(); ++i )
			{
				if(i>=numChild)
				{
					crLoadManager::getInstance()->requestNode(lod->getFileName(i),lod,false);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crHiddenOrUnHiddenNodes2Method
//
/////////////////////////////////////////
crHiddenOrUnHiddenNodes2Method::crHiddenOrUnHiddenNodes2Method(){}
crHiddenOrUnHiddenNodes2Method::crHiddenOrUnHiddenNodes2Method(const crHiddenOrUnHiddenNodes2Method& handle):
crMethod(handle),
m_nodeNameArray(handle.m_nodeNameArray)
{
}
void crHiddenOrUnHiddenNodes2Method::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crHiddenOrUnHiddenNodes2Method::addParam(int i, const std::string& str)
{
    m_nodeNameArray.push_back(str);
}

void crHiddenOrUnHiddenNodes2Method::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		crNode *node;
		for( NodeNameArray::iterator itr = m_nodeNameArray.begin();
			itr != m_nodeNameArray.end();
			++itr )
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(*itr);
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			node = searchByNameVisitor.getResult();
			if(node) node->setEditorHidden(m_checkBox->getSelect());
		}
	}
}
/////////////////////////////////////////
//
//crWireDrawModeMethod
//
/////////////////////////////////////////
crWireDrawModeMethod::crWireDrawModeMethod(){}
crWireDrawModeMethod::crWireDrawModeMethod(const crWireDrawModeMethod& handle):
crMethod(handle)
{
}
void crWireDrawModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crWireDrawModeMethod::addParam(int i, const std::string& str)
{
}

void crWireDrawModeMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!bindview || !bindview->isInited())
			return;
		crSceneView *sv = bindview->getDefaultSceneHandler()->getSceneView();
		assert(sv);
		CRCore::crStateSet* globalStateSet = sv->getGlobalStateSet();
		
		CRCore::crPolygonMode* polygonMode = dynamic_cast<CRCore::crPolygonMode*>(globalStateSet->getAttribute(CRCore::crStateAttribute::POLYGONMODE));
		if (!polygonMode) 
		{
			polygonMode = new CRCore::crPolygonMode;
			globalStateSet->setAttributeAndModes(polygonMode,crStateAttribute::ON);
		}

		if(m_checkBox->getSelect())
		{
			polygonMode->setMode(CRCore::crPolygonMode::FRONT_AND_BACK,CRCore::crPolygonMode::LINE);
		}
		else
		{
			polygonMode->setMode(CRCore::crPolygonMode::FRONT_AND_BACK,CRCore::crPolygonMode::FILL);
		}
	}
}
/////////////////////////////////////////
//
//crSetLayeredWindowAttributesMethod
//
/////////////////////////////////////////
crSetLayeredWindowAttributesMethod::crSetLayeredWindowAttributesMethod(){}
crSetLayeredWindowAttributesMethod::crSetLayeredWindowAttributesMethod(const crSetLayeredWindowAttributesMethod& handle):
crMethod(handle)
{
}
void crSetLayeredWindowAttributesMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crSetLayeredWindowAttributesMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		{
			crVector3i rgb;
			crArgumentParser::appAtoVec(str,rgb);
			m_keycolor = RGB(rgb[0],rgb[1],rgb[2]);
		}
		break;
	case 1:
		m_alpha = (unsigned char)(atoi(str.c_str()));
		break;
	case 2:
		m_flg = (unsigned char)(atoi(str.c_str()));;
		break;
	}
}

void crSetLayeredWindowAttributesMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		if (CRGUI::s_SetLayeredWindowAttributes == NULL)
		{
			// 获取函数 SetLayeredWindowAttributes 在User32.dll中的指针 
			HINSTANCE hUser32 = GetModuleHandle("USER32.DLL"); 
			s_SetLayeredWindowAttributes = (SetLayeredWindowAttributesProc)GetProcAddress(hUser32, "SetLayeredWindowAttributes"); 
		}
		if(s_SetLayeredWindowAttributes)
		{
			HWND win = crWindowContainer::getWindow(m_stage.get());
			LONG lWindowLong = GetWindowLong(win, GWL_EXSTYLE) | WS_EX_LAYERED; 
			::SetWindowLong(win, GWL_EXSTYLE, lWindowLong); 
			s_SetLayeredWindowAttributes(win,m_keycolor, m_alpha, m_flg);
		}
	}
}
/////////////////////////////////////////
//
//crCombineObjectsMethod
//
/////////////////////////////////////////
crCombineObjectsMethod::crCombineObjectsMethod(){}
crCombineObjectsMethod::crCombineObjectsMethod(const crCombineObjectsMethod& handle):
crMethod(handle)
{
}
void crCombineObjectsMethod::inputParam(int i, void *param)
{
}

void crCombineObjectsMethod::addParam(int i, const std::string& str)
{
}

void crCombineObjectsMethod::operator()(crHandle &handle)
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
	bodyInfo->unSelectAll();
	if(selectType == 1 && selectNodeVec.size()>0)
	{
        crObject *firstObj = dynamic_cast<crObject *>(selectNodeVec[0].get());
		if(firstObj->getName() == "sky")
			return;
		crGroup *group = dynamic_cast<crGroup *>(firstObj->getParent(0));
		ref_ptr<crObject> selectObj;
		ref_ptr<crGroup> selectObjRoot;
		ref_ptr<crGroup> firstProxy;
		CRUtil::crOptimizer optimizer;
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.reset();
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		firstObj->accept(searchByNameVisitor);
		firstProxy = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(firstProxy.valid())
		{
			crDisconnectDrawableVisitor ddv;
			firstProxy->accept(ddv);
			CRUtil::crOptimizer::crFreezeTransformsVisitor fztv;
			fztv.setIgnorePhysics(true);
			firstProxy->accept(fztv);
			fztv.freeze();
			CRUtil::crPrepareVisitor prepareVisitor;
			firstProxy->accept(prepareVisitor);
			//firstProxy->releaseObjects();
		}
		std::set< ref_ptr<crGroup> > toRemoveRootSet;
		crGroup *combineGroup;
		for(int i = 1; i<selectNodeVec.size(); i++)
		{
			selectObj = dynamic_cast<crObject *>(selectNodeVec[i].get());
			if(selectObj.valid())
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setNameId("ProxyNode");
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				selectObj->accept(searchByNameVisitor);
				selectObjRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(selectObjRoot.valid())
				{
					crDisconnectDrawableVisitor ddv;
					selectObjRoot->accept(ddv);
					CRUtil::crOptimizer::crFreezeTransformsVisitor fztv;
					fztv.setIgnorePhysics(true);
					selectObjRoot->accept(fztv);
					fztv.freeze();
					CRUtil::crPrepareVisitor prepareVisitor;
					selectObjRoot->accept(prepareVisitor);
					selectObjRoot->releaseObjects();

					combineGroup = dynamic_cast<crGroup *>(selectObj->getParent(0));
					combineGroup->removeChild(selectObj.get());
					group->addChild(selectObj.get());
					if(combineGroup->getNumChildren() == 0)
					{
						toRemoveRootSet.insert(selectObjRoot.get());
					}
					//else
					//{
					//	crLoadManager::getInstance()->requestCompile(selectObjRoot.get());
					//}
				}
			}
		}
		optimizer.optimize(group,CRUtil::crOptimizer::UNITE_NODES|CRUtil::crOptimizer::MERGE_GEOMETRY|CRUtil::crOptimizer::MERGE_VERTEX);
		firstProxy->releaseObjects();
		//crLoadManager::getInstance()->requestCompile(firstProxy.get());
		//crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
        //crDatabasePager::LoadedMap &loadedMap = dp->getLoadedMap();
		std::string fileName;
		ref_ptr<crGroup> setItr;
		for( std::set< ref_ptr<crGroup> >::iterator itr = toRemoveRootSet.begin();
			 itr != toRemoveRootSet.end();
			 ++itr )
		{
			//if(crArgumentParser::readKeyValue((*itr)->getDescriptions(),"DataFile",fileName))
			//{
			//    loadedMap.erase(fileName);
			//}
			setItr = *itr;
			CRIOManager::crLoadManager::getInstance()->requestRemoveNode(dynamic_cast<crGroup *>(setItr->getParent(0)),setItr.get());
            //(dynamic_cast<crGroup *>((*itr)->getParent(0)))->removeChild(itr->get());
		}
	}
}
/////////////////////////////////////////
//
//crCombineDrawablesMethod
//
/////////////////////////////////////////
crCombineDrawablesMethod::crCombineDrawablesMethod(){}
crCombineDrawablesMethod::crCombineDrawablesMethod(const crCombineDrawablesMethod& handle):
	crMethod(handle)
{
}
void crCombineDrawablesMethod::inputParam(int i, void *param)
{
}

void crCombineDrawablesMethod::addParam(int i, const std::string& str)
{
}

void crCombineDrawablesMethod::operator()(crHandle &handle)
{//合并按照节点名字排序，粒子系统模型序列帧
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
	std::map<std::string,CRCore::ref_ptr<CRCore::crNode>,std::less<std::string> >SelectNodeMap;
	std::map<std::string,CRCore::ref_ptr<CRCore::crNode>,std::less<std::string> >::iterator smitr;
	for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
		 itr != selectNodeVec.end();
		 ++itr )
	{
		SelectNodeMap[(*itr)->getName()] = itr->get();
	}
	bodyInfo->unSelectAll();
	if(selectType == 1 && SelectNodeMap.size()>0)
	{
		smitr = SelectNodeMap.begin();
		crObject *firstObj = dynamic_cast<crObject *>(smitr->second.get());
		if( firstObj->getName() == "sky" ||
			firstObj->getName().compare("EditorAxis") == 0 || 
			firstObj->getName().compare("ViewGrid") == 0 ||
			firstObj->getName().compare("WaypointLink") == 0 )
			return;
		ref_ptr<crObject> selectObj;
		++smitr;
		for(; smitr!=SelectNodeMap.end(); ++smitr)
		{
			selectObj = dynamic_cast<crObject *>(smitr->second.get());
			if(selectObj.valid())
			{
				crObject::DrawableList &drawableList = selectObj->getDrawableList();
				for( crObject::DrawableList::iterator itr = drawableList.begin();
					 itr != drawableList.end();
					 ++itr )
				{
					if((*itr)->asGeometry())
						firstObj->addDrawable(itr->get());
				}
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(dynamic_cast<crGroup *>(selectObj->getParent(0)),selectObj.get());
			}
		}
	}
}
/////////////////////////////////////////
//
//crEditorMoveCameraMethod
//
/////////////////////////////////////////
crEditorMoveCameraMethod::crEditorMoveCameraMethod(){}
crEditorMoveCameraMethod::crEditorMoveCameraMethod(const crEditorMoveCameraMethod& handle):
crMethod(handle),
m_xPos(handle.m_xPos),
m_yPos(handle.m_yPos),
m_zPos(handle.m_zPos)
{
}
void crEditorMoveCameraMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crEditorMoveCameraMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_xPos = str;
		break;
	case 1:
		m_yPos = str;
		break;
	case 2:
		m_zPos = str;
		break;
	}
}

void crEditorMoveCameraMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crImageStage *stage = m_element->getParentStage();
	crEditBoxElement *xPos = dynamic_cast<crEditBoxElement *>(stage->getElement(m_xPos));
	crEditBoxElement *yPos = dynamic_cast<crEditBoxElement *>(stage->getElement(m_yPos));
	crEditBoxElement *zPos = dynamic_cast<crEditBoxElement *>(stage->getElement(m_zPos));
    crVector3 pos;
	if(xPos)
	{
		pos[0] = atoi(xPos->getStringArrayInString().c_str());
	}
	if(yPos)
	{
		pos[1] = atoi(yPos->getStringArrayInString().c_str());
	}
	if(zPos)
	{
		pos[2] = atoi(zPos->getStringArrayInString().c_str());
	}
	crMatrix matrix;
	matrix.setTrans(pos);
	cameraBot->setMatterObjectMatrix(matrix,crMatterObject::MD_FullMatrix);
}
class BuildNodeTreeVisitor : public CRCore::crNodeVisitor
{
public:
	BuildNodeTreeVisitor():
	  CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN/*TRAVERSE_ACTIVE_CHILDREN*/){}

    
	virtual ~BuildNodeTreeVisitor(){}

	void setRoot(crTreeNode *treeNode)
	{
		m_treeNodeStack.push(treeNode);
	}
	void clear()
	{
		while(!m_treeNodeStack.empty())
		{
            m_treeNodeStack.pop();
		}
	}

	virtual void apply(CRCore::crNode& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Red15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRCore::crLod& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Green15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRCore::crSequence& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Pink15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRCore::crMultiSwitch& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Pink15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRCore::crTransform& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Black15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRPhysics::crMatterObject& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Black15_50"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRParticle::crParticleEffect& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Green15_50"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		traverse(node);
		m_treeNodeStack.pop();
	}
	virtual void apply(CRCore::crObject& node)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		treeNode->setTitle(node.getName());
		parent->addChild(treeNode);

		if(node.getName().compare("PSObject") == 0)
		    treeNode->expand();
		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Blue15"));
		treeNode->setData(&node);

		m_treeNodeStack.push(treeNode);
		for(unsigned int i=0;i<node.getNumDrawables();++i)
		{
			crDrawable* drawable = node.getDrawable(i);
			if (drawable)
			{
				apply(*drawable);
			}
		}
		traverse(node);
		m_treeNodeStack.pop();
	}
	void apply(CRCore::crDrawable& drawable)
	{
		crTreeNode *parent = m_treeNodeStack.top().get();
		crTreeNode *treeNode = new crTreeNode;
		crGeometry* geo = drawable.asGeometry();
		if(geo)
		{
			char buf[32];
			sprintf(buf,"Drawable[%d]\0",geo->getNumPrimitives());
			treeNode->setTitle(buf);
		}
		else
		{
			treeNode->setTitle("Drawable");
		}
		parent->addChild(treeNode);

		treeNode->setRowHeight(parent->getRowHeight());
		treeNode->setColumnOffset(parent->getColumnOffset());
		treeNode->setElementWidth(parent->getElementWidth());
		treeNode->setTextAttribute(crTextAttribute::getTextAttribute("Black15"));
		treeNode->setData(&drawable);
	}
protected:
	std::stack< ref_ptr<crTreeNode> > m_treeNodeStack;
};
static BuildNodeTreeVisitor s_buildNodeTreeVisitor;
/////////////////////////////////////////
//
//crBuildNodeTreeMethod
//
/////////////////////////////////////////
crBuildNodeTreeMethod::crBuildNodeTreeMethod():
m_buildType(SelectNode){}
crBuildNodeTreeMethod::crBuildNodeTreeMethod(const crBuildNodeTreeMethod& handle):
crMethod(handle),
m_nodeTreeElement(handle.m_nodeTreeElement),
m_countElement(handle.m_countElement),
m_buildType(handle.m_buildType)
{
}
void crBuildNodeTreeMethod::inputParam(int i, void *param)
{
	if(i==0)
	{
		if(m_buildType == SelectNode)
		    m_stage = (CRGUI::crImageStage*)param;
		else
		{
			m_element = (crElement*)param;
			if(m_element.valid())
			    m_stage = m_element->getParentStage();
			else
				m_stage = NULL;
		}
	}
}

void crBuildNodeTreeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_nodeTreeElement = str;
		break;
	case 1:
		m_countElement = str;
		break;
	case 2:
		m_buildType = (unsigned char)(atoi(str.c_str()));
		break;
	}
}

void crBuildNodeTreeMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		crTreeElement *nodeTreeElement = dynamic_cast<crTreeElement *>(m_stage->getElement(m_nodeTreeElement));
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		if(gamebodyInfo && nodeTreeElement)
		{
			nodeTreeElement->lockTree();
			ref_ptr<crTreeNode> treeRoot = nodeTreeElement->getTreeRootNode();
			treeRoot->removeAllChild();

			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			if(m_buildType == SelectNode)
			{
				crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
				if(!selectNodeVec.empty())
				{
					ref_ptr<crTreeNode> treeNode1,treeNode2;
					ref_ptr<crNode> selectNode;
					ref_ptr<crGroup> rootNode;
					for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
						itr != selectNodeVec.end();
						++itr )
					{
						selectNode = itr->get();
						searchByNameVisitor.reset();
						searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
						searchByNameVisitor.setNameId("ProxyNode");
						searchByNameVisitor.setSearchNodeType(ALLNODE);
						selectNode->accept(searchByNameVisitor);
						rootNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
						if(rootNode.valid())
						{
							s_buildNodeTreeVisitor.setRoot(treeRoot.get());
							rootNode->accept(s_buildNodeTreeVisitor);
							s_buildNodeTreeVisitor.clear();
						}
					}
				}
			}
			else if(m_buildType == StaticModel)
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByNameVisitor.setNameId("Static");
				searchByNameVisitor.setSearchNodeType(GROUP);
				crBrain::getInstance()->accept(searchByNameVisitor);
				crGroup *root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(root)
				{
					NodeArray &childArray = root->getChildArray();
					for( NodeArray::iterator itr = childArray.begin();
						itr != childArray.end();
						++itr )
					{
						if((*itr)->getName().compare("ProxyNode") == 0)
						{
							s_buildNodeTreeVisitor.setRoot(treeRoot.get());
							(*itr)->accept(s_buildNodeTreeVisitor);
							s_buildNodeTreeVisitor.clear();
						}
					}
				}
			}
			else if(m_buildType == UI)
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByNameVisitor.setNameId("Filter");
				searchByNameVisitor.setSearchNodeType(GROUP);
				crBrain::getInstance()->accept(searchByNameVisitor);
				crGroup *filter = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(filter)
				{
					NodeArray &childArray = filter->getChildArray();
					for( NodeArray::iterator itr = childArray.begin();
						itr != childArray.end();
						++itr )
					{
						if((*itr)->getName().compare("ProxyNode") == 0)
						{
							s_buildNodeTreeVisitor.setRoot(treeRoot.get());
							(*itr)->accept(s_buildNodeTreeVisitor);
							s_buildNodeTreeVisitor.clear();
						}
					}
				}
			}
			else if(m_buildType == NotRender)
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByNameVisitor.setNameId("CollideRoot");
				searchByNameVisitor.setSearchNodeType(GROUP);
				crBrain::getInstance()->accept(searchByNameVisitor);
				crGroup *root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(root)
				{
					NodeArray &childArray = root->getChildArray();
					for( NodeArray::iterator itr = childArray.begin();
						itr != childArray.end();
						++itr )
					{
						if((*itr)->getName().compare("ProxyNode") == 0)
						{
							s_buildNodeTreeVisitor.setRoot(treeRoot.get());
							(*itr)->accept(s_buildNodeTreeVisitor);
							s_buildNodeTreeVisitor.clear();
						}
					}
				}
			}
			else if(m_buildType == Trigger)
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
				searchByNameVisitor.setNameId("RemovedGroup");
				searchByNameVisitor.setSearchNodeType(GROUP);
				crBrain::getInstance()->accept(searchByNameVisitor);
				crGroup *root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(root)
				{
					NodeArray &childArray = root->getChildArray();
					for( NodeArray::iterator itr = childArray.begin();
						 itr != childArray.end();
						 ++itr )
					{
						if((*itr)->getName().compare("ProxyNode") == 0)
						{
							s_buildNodeTreeVisitor.setRoot(treeRoot.get());
							(*itr)->accept(s_buildNodeTreeVisitor);
							s_buildNodeTreeVisitor.clear();
						}
					}
				}
			}
			crStaticTextBoxElement *countElement = dynamic_cast<crStaticTextBoxElement *>(m_stage->getElement(m_countElement));
			if(countElement)
			{
                int count = nodeTreeElement->getTreeRootNode()->getChildArray().size();
				countElement->setStringArrayByString(crArgumentParser::appItoa(count));
			}
			nodeTreeElement->unlockTree();
			nodeTreeElement->updateData();
		}
	}
}
/////////////////////////////////////////
//
//crSeletNodeByNodeTreeMethod
//
/////////////////////////////////////////
crSeletNodeByNodeTreeMethod::crSeletNodeByNodeTreeMethod(){}
crSeletNodeByNodeTreeMethod::crSeletNodeByNodeTreeMethod(const crSeletNodeByNodeTreeMethod& handle):
crMethod(handle),
m_nodeTreeElement(handle.m_nodeTreeElement)
{
}
void crSeletNodeByNodeTreeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_nodeTreeElement = (CRGUI::crTreeElement*)param;
}

void crSeletNodeByNodeTreeMethod::addParam(int i, const std::string& str)
{
}

void crSeletNodeByNodeTreeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	if(m_nodeTreeElement.valid() && gamebodyInfo)
	{
		crTreeNode *treeNode = m_nodeTreeElement->getSelectNode();
		if(treeNode)
		{
			if(treeNode->getTitle().find("Drawable")==0)
			{
				crDrawable *selectMesh = (crDrawable*)(treeNode->getData());
				if(selectMesh)
				{
					crTreeNode *parent = treeNode->getParent();
					crNode *selectNode = (crNode*)(parent->getData());
					gamebodyInfo->unSelectAll();
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
					gamebodyInfo->selectNode(selectNode);
					gamebodyInfo->getDataClass()->inputParam(EDP_CurrentSelectDrawable,selectMesh);
				}
			}
			else
			{
				crNode *selectNode = (crNode*)(treeNode->getData());
				if(selectNode)
				{
					gamebodyInfo->unSelectAll();
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
					gamebodyInfo->selectNode(selectNode);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crInitTerraformModeMethod
//
/////////////////////////////////////////
crInitTerraformModeMethod::crInitTerraformModeMethod(){}
crInitTerraformModeMethod::crInitTerraformModeMethod(const crInitTerraformModeMethod& handle):
crMethod(handle)
{
}
void crInitTerraformModeMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitTerraformModeMethod::addParam(int i, const std::string& str)
{
}

void crInitTerraformModeMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *param;
		infoData->getParam(EDP_TerEditMode,param);
		unsigned char mode = *((unsigned char*)param);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crSetTerraformModeMethod
//
/////////////////////////////////////////
crSetTerraformModeMethod::crSetTerraformModeMethod():
m_terraformMode(0){}
crSetTerraformModeMethod::crSetTerraformModeMethod(const crSetTerraformModeMethod& handle):
crMethod(handle),
m_terraformMode(handle.m_terraformMode)
{
}
void crSetTerraformModeMethod::inputParam(int i, void *param)
{
}

void crSetTerraformModeMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_terraformMode = (unsigned char)(atoi(str.c_str()));
}

void crSetTerraformModeMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(!cameraBot);
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	assert(!gamebodyInfo);
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(EDP_TerEditMode,&m_terraformMode);
	}
}
/////////////////////////////////////////
//
//crInitCurrentTerToolMethod
//
/////////////////////////////////////////
crInitCurrentTerToolMethod::crInitCurrentTerToolMethod(){}
crInitCurrentTerToolMethod::crInitCurrentTerToolMethod(const crInitCurrentTerToolMethod& handle):
crMethod(handle)
{
}
void crInitCurrentTerToolMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_radioGroupElement = (CRGUI::crRadioGroupElement*)param;
}

void crInitCurrentTerToolMethod::addParam(int i, const std::string& str)
{
}

void crInitCurrentTerToolMethod::operator()(crHandle &handle)
{
	if(m_radioGroupElement.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(!cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(!gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(!infoData);

		void *param;
		infoData->getParam(EDP_CurrentTerTool,param);
		unsigned char mode = *((unsigned char*)param);
		m_radioGroupElement->select(mode);
	}
}
/////////////////////////////////////////
//
//crSetCurrentTerToolMethod
//
/////////////////////////////////////////
crSetCurrentTerToolMethod::crSetCurrentTerToolMethod():
m_currentTerTool(0){}
crSetCurrentTerToolMethod::crSetCurrentTerToolMethod(const crSetCurrentTerToolMethod& handle):
crMethod(handle),
m_currentTerTool(handle.m_currentTerTool)
{
}
void crSetCurrentTerToolMethod::inputParam(int i, void *param)
{
}

void crSetCurrentTerToolMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_currentTerTool = (unsigned char)(atoi(str.c_str()));
}

void crSetCurrentTerToolMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crData *infoData = gamebodyInfo->getDataClass();
	if(infoData)
	{
		infoData->inputParam(EDP_CurrentTerTool,&m_currentTerTool);
	}
}
/////////////////////////////////////////
//
//crSetTerEditParamMethod
//
/////////////////////////////////////////
crSetTerEditParamMethod::crSetTerEditParamMethod(){}
crSetTerEditParamMethod::crSetTerEditParamMethod(const crSetTerEditParamMethod& handle):
crMethod(handle),
m_radius(handle.m_radius),
m_strength(handle.m_strength)
{
}
void crSetTerEditParamMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSetTerEditParamMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radius = str;
		break;
	case 1:
		m_strength = str;
		break;
	}
}

void crSetTerEditParamMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		int radius = 32;
		float strength = 20.0f;
		crEditBoxElement *radiusEdit = dynamic_cast<crEditBoxElement *>(stage->getElement(m_radius));
		if(radiusEdit)
		{
			radius = atoi(radiusEdit->getStringArrayInString().c_str());
		}
		crEditBoxElement *strengthEdit = dynamic_cast<crEditBoxElement *>(stage->getElement(m_strength));
		if(strengthEdit)
		{
			strength = atof(strengthEdit->getStringArrayInString().c_str());
		}

		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		crData *infoData = gamebodyInfo->getDataClass();

		infoData->inputParam(EDP_TerToolRadius,&radius);
		infoData->inputParam(EDP_TerToolHeight,&strength);
	}
}
/////////////////////////////////////////
//
//crDisplayDofParamMethod
//
/////////////////////////////////////////
crDisplayDofParamMethod::crDisplayDofParamMethod(){}
crDisplayDofParamMethod::crDisplayDofParamMethod(const crDisplayDofParamMethod& handle):
crMethod(handle),
m_gridElement(handle.m_gridElement)
{
}
void crDisplayDofParamMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_stage = (CRGUI::crImageStage*)param;
}

void crDisplayDofParamMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crDisplayDofParamMethod::operator()(crHandle &handle)
{
	if(m_stage.valid())
	{
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(m_stage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();

			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crDOFTransform *dof = dynamic_cast<crDOFTransform *>(selectNodeVec[0].get());
				if(!dof)
				{
					crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
					if(currentSelectObject)
					{
						void *param;
						crData *data = bodyInfo->getDataClass();
						data->getParam(EDP_CurrentFocusTexture,param);
						std::string focusTex = *(std::string *)param;
						if(!focusTex.empty())
						{
							crDofUniformTexMatCallback *dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(currentSelectObject->getCullCallback("DofUniformTexMat"));
							if(dofcallback)
							{
								dof = dofcallback->getDofTransform();
								if(dof->getName().compare(focusTex) != 0)
								{
                                    dof = NULL;
								}
								while(!dof)
								{
									dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(dofcallback->getNestedCallback("DofUniformTexMat"));
									if(!dofcallback)
										break;
									dof = dofcallback->getDofTransform();
									if(dof->getName().compare(focusTex) != 0)
									{
										dof = NULL;
									}
								}
							}
						}
					}
				}
				if(dof)
				{
					gridElement->addData("节点名",dof->getName(),"");
                    CRCore::crVector3 minHPR = dof->getMinHPR();
					crVector3 rotXYZ;
					rotXYZ[0] = CRCore::RadiansToDegrees(minHPR[1]);
					rotXYZ[1] = CRCore::RadiansToDegrees(minHPR[2]);
					rotXYZ[2] = CRCore::RadiansToDegrees(minHPR[0]);
					gridElement->addData("最小旋转",crArgumentParser::appVectoa(rotXYZ),"Red13");
					CRCore::crVector3 maxHPR = dof->getMaxHPR();
					rotXYZ[0] = CRCore::RadiansToDegrees(maxHPR[1]);
					rotXYZ[1] = CRCore::RadiansToDegrees(maxHPR[2]);
					rotXYZ[2] = CRCore::RadiansToDegrees(maxHPR[0]);
					gridElement->addData("最大旋转",crArgumentParser::appVectoa(rotXYZ),"Red13");
					CRCore::crVector3 currentHPR = dof->getCurrentHPR();
					rotXYZ[0] = CRCore::RadiansToDegrees(currentHPR[1]);
					rotXYZ[1] = CRCore::RadiansToDegrees(currentHPR[2]);
					rotXYZ[2] = CRCore::RadiansToDegrees(currentHPR[0]);
					gridElement->addData("当前旋转",crArgumentParser::appVectoa(rotXYZ),"Red13");
					CRCore::crVector3 stepHPR = dof->getIncrementHPR();
					rotXYZ[0] = CRCore::RadiansToDegrees(stepHPR[1]);
					rotXYZ[1] = CRCore::RadiansToDegrees(stepHPR[2]);
					rotXYZ[2] = CRCore::RadiansToDegrees(stepHPR[0]);
					gridElement->addData("旋转速度",crArgumentParser::appVectoa(rotXYZ),"Red13");
					const CRCore::crVector3b& endStopHPR = dof->getEndStopHPR();
					crVector3i endStop;
					endStop[0] = endStopHPR[1]?1:0;
					endStop[1] = endStopHPR[2]?1:0;
					endStop[2] = endStopHPR[0]?1:0;
					gridElement->addData("终点停止",crArgumentParser::appVectoa(endStop),"Red13");

					const CRCore::crVector3& minTrans = dof->getMinTranslate();
					gridElement->addData("最小移动",crArgumentParser::appVectoa(minTrans),"");
					const CRCore::crVector3& maxTrans = dof->getMaxTranslate();
					gridElement->addData("最大移动",crArgumentParser::appVectoa(maxTrans),"");
					const CRCore::crVector3& currentTrans = dof->getCurrentTranslate();
					gridElement->addData("当前移动",crArgumentParser::appVectoa(currentTrans),"");
					const CRCore::crVector3& stepTrans = dof->getIncrementTranslate();
					gridElement->addData("移动速度",crArgumentParser::appVectoa(stepTrans),"");
					const CRCore::crVector3b& endStopTrans = dof->getEndStopTrans();
					endStop[0] = endStopTrans[0]?1:0;
					endStop[1] = endStopTrans[1]?1:0;
					endStop[2] = endStopTrans[2]?1:0;
					gridElement->addData("终点停止",crArgumentParser::appVectoa(endStop),"");

					const CRCore::crVector3& minScale = dof->getMinScale();
					gridElement->addData("最小缩放",crArgumentParser::appVectoa(minScale),"Blue13");
					const CRCore::crVector3& maxScale = dof->getMaxScale();
					gridElement->addData("最大缩放",crArgumentParser::appVectoa(maxScale),"Blue13");
					const CRCore::crVector3& currentScale = dof->getCurrentScale();
					gridElement->addData("当前缩放",crArgumentParser::appVectoa(currentScale),"Blue13");
					const CRCore::crVector3& stepScale = dof->getIncrementScale();
					gridElement->addData("缩放速度",crArgumentParser::appVectoa(stepScale),"Blue13");
					const CRCore::crVector3b& endStopScale = dof->getEndStopScale();
					endStop[0] = endStopScale[0]?1:0;
					endStop[1] = endStopScale[1]?1:0;
					endStop[2] = endStopScale[2]?1:0;
					gridElement->addData("终点停止",crArgumentParser::appVectoa(endStop),"Blue13");

					const CRCore::crVector3& O = dof->getOrigin();
					gridElement->addData("原点",crArgumentParser::appVectoa(O),"");
					const CRCore::crVector3& xAxis = dof->getXAxis();
					gridElement->addData("X轴",crArgumentParser::appVectoa(xAxis),"");
					const CRCore::crVector3& xyPlane = dof->getXYPlane();
					gridElement->addData("Y轴",crArgumentParser::appVectoa(xyPlane),"");

					bool animationOn = dof->getAnimationOn();
					gridElement->addData("开启运动",animationOn?"1":"0","");

					unsigned short surgeFlags = dof->getSurgeFlags();
					std::string surgeStr;
					if(surgeFlags == crDOFTransform::ALL)
						surgeStr = "ALL";
					else
					{
						if(surgeFlags & (crDOFTransform::XTran|crDOFTransform::YTran|crDOFTransform::ZTran))
						{
                            surgeStr = "XYZTran";
						}
						else
						{
							if(surgeFlags & crDOFTransform::XTran)
								surgeStr = "XTran";
							if(surgeFlags & crDOFTransform::YTran)
							{
								if(surgeStr.empty())
									surgeStr = "YTran";
								else
									surgeStr += "|YTran";
							}
							if(surgeFlags & crDOFTransform::ZTran)
							{
								if(surgeStr.empty())
									surgeStr = "ZTran";
								else
									surgeStr += "|ZTran";
							}
						}
						if(surgeFlags & (crDOFTransform::XRot|crDOFTransform::YRot|crDOFTransform::ZRot))
						{
							surgeStr = "XYZRot";
						}
						else
						{
							if(surgeFlags & crDOFTransform::XRot)
							{
								if(surgeStr.empty())
									surgeStr = "XRot";
								else
									surgeStr += "|XRot";
							}
							if(surgeFlags & crDOFTransform::YRot)
							{
								if(surgeStr.empty())
									surgeStr = "YRot";
								else
									surgeStr += "|YRot";
							}
							if(surgeFlags & crDOFTransform::ZRot)
							{
								if(surgeStr.empty())
									surgeStr = "ZRot";
								else
									surgeStr += "|ZRot";
							}
						}
						if(surgeFlags & (crDOFTransform::XScale|crDOFTransform::YScale|crDOFTransform::ZScale))
						{
							surgeStr = "XYZScale";
						}
						else
						{
							if(surgeFlags & crDOFTransform::XScale)
							{
								if(surgeStr.empty())
									surgeStr = "XScale";
								else
									surgeStr += "|XScale";
							}
							if(surgeFlags & crDOFTransform::YScale)
							{
								if(surgeStr.empty())
									surgeStr = "YScale";
								else
									surgeStr += "|YScale";
							}
							if(surgeFlags & crDOFTransform::ZScale)
							{
								if(surgeStr.empty())
									surgeStr = "ZScale";
								else
									surgeStr += "|ZScale";
							}
						}
					}
					gridElement->addData("振荡标志",surgeStr,"");

                    bool endless = dof->getEndless();
					gridElement->addData("无穷",endless?"1":"0","");
					float lifeTime = dof->getLifeTime();
					gridElement->addData("生命周期",crArgumentParser::appFtoa(lifeTime),"");
                    float startTime = dof->getStartTime();
					gridElement->addData("开始时间",crArgumentParser::appFtoa(startTime),"");				
					float resetTime = dof->getResetTime();
					gridElement->addData("重置时间",crArgumentParser::appFtoa(resetTime),"");

					////////////////////////////
					gridElement->addData("END","NULL","");
				}
			}
			gridElement->checkSelect();
			gridElement->updateInputElement(true);
		}
	}
}
/////////////////////////////////////////
//
//crModifyDofParamMethod
//
/////////////////////////////////////////
crModifyDofParamMethod::crModifyDofParamMethod(){}
crModifyDofParamMethod::crModifyDofParamMethod(const crModifyDofParamMethod& handle):
crMethod(handle),
m_gridElement(handle.m_gridElement)
{
}
void crModifyDofParamMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyDofParamMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crModifyDofParamMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				bool isTexMat = false;
				crDOFTransform *dof = dynamic_cast<crDOFTransform *>(selectNodeVec[0].get());
				if(!dof)
				{
					crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
					if(currentSelectObject)
					{
						isTexMat = true;
						void *param;
						crData *data = bodyInfo->getDataClass();
						data->getParam(EDP_CurrentFocusTexture,param);
						std::string focusTex = *(std::string *)param;
						if(!focusTex.empty())
						{
							crDofUniformTexMatCallback *dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(currentSelectObject->getCullCallback("DofUniformTexMat"));
							if(dofcallback)
							{
								dof = dofcallback->getDofTransform();
								if(dof->getName().compare(focusTex) != 0)
								{
									dof = NULL;
								}
								while(!dof)
								{
									dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(dofcallback->getNestedCallback("DofUniformTexMat"));
									if(!dofcallback)
										break;
									dof = dofcallback->getDofTransform();
									if(dof->getName().compare(focusTex) != 0)
									{
										dof = NULL;
									}
								}
							}
						}
					}
				}
				if(dof)
				{
					int gridIndex = 0;
					crKeyValueNode *keyValueNode;
					std::string str;
					crVector3 vec3;
					crVector3i vec3i;
					crVector3b vec3b;
					keyValueNode = gridElement->getData(gridIndex++);
					if(!isTexMat)
					{
						str = keyValueNode->getValue();
						dof->setName(str);
					}

					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					crVector3 rotHPR;
					rotHPR[1] = CRCore::DegreesToRadians(vec3[0]);
					rotHPR[2] = CRCore::DegreesToRadians(vec3[1]);
					rotHPR[0] = CRCore::DegreesToRadians(vec3[2]);
					dof->setMinHPR(rotHPR);

					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					rotHPR[1] = CRCore::DegreesToRadians(vec3[0]);
					rotHPR[2] = CRCore::DegreesToRadians(vec3[1]);
					rotHPR[0] = CRCore::DegreesToRadians(vec3[2]);
					dof->setMaxHPR(rotHPR);

					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					rotHPR[1] = CRCore::DegreesToRadians(vec3[0]);
					rotHPR[2] = CRCore::DegreesToRadians(vec3[1]);
					rotHPR[0] = CRCore::DegreesToRadians(vec3[2]);
					dof->setCurrentHPR(rotHPR);
                    //"增量HPR"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					rotHPR[1] = CRCore::DegreesToRadians(vec3[0]);
					rotHPR[2] = CRCore::DegreesToRadians(vec3[1]);
					rotHPR[0] = CRCore::DegreesToRadians(vec3[2]);
					dof->setIncrementHPR(rotHPR);
					//终点停止
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3i);
					vec3b[1] = vec3i[0];
					vec3b[2] = vec3i[1];
					vec3b[0] = vec3i[2];
					dof->setEndStopHPR(vec3b);

					//最小移动
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setMinTranslate(vec3);
					//最大移动
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setMaxTranslate(vec3);
					//当前移动
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setCurrentTranslate(vec3);
					//增量移动
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setIncrementTranslate(vec3);
					//终点停止
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3i);
					vec3b[0] = vec3i[0];
					vec3b[1] = vec3i[1];
					vec3b[2] = vec3i[2];
					dof->setEndStopTrans(vec3b);

					//最小缩放
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setMinScale(vec3);
					//最大缩放
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setMaxScale(vec3);
					//当前缩放
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setCurrentScale(vec3);
					//增量缩放
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setIncrementScale(vec3);
					//终点停止
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3i);
					vec3b[0] = vec3i[0];
					vec3b[1] = vec3i[1];
					vec3b[2] = vec3i[2];
					dof->setEndStopScale(vec3b);

					//原点
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setOrigin(vec3);
					//X轴
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setXAxis(vec3);
					//Y轴
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					dof->setXYPlane(vec3);
					dof->computePutMatrix();

					////限定标志
					//keyValueNode = gridElement->getData(gridIndex++);
					//str = keyValueNode->getValue();
					//dof->addParam(18,str);
					dof->addParam(18,"ALL");
					//开启运动
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->setAnimationOn(atoi(str.c_str()));
					//振荡标志
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->addParam(20,str);

					//无穷
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->setEndless(atoi(str.c_str()));
					//生命周期
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->setLifeTime(atof(str.c_str()));
					//开始时间
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->setStartTime(atof(str.c_str()));
					//重置时间
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					dof->setResetTime(atof(str.c_str()));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crFocusDofTexMatMethod
//
/////////////////////////////////////////
crFocusDofTexMatMethod::crFocusDofTexMatMethod(){}
crFocusDofTexMatMethod::crFocusDofTexMatMethod(const crFocusDofTexMatMethod& handle):
crMethod(handle),
m_stageName(handle.m_stageName),
m_texTypeName(handle.m_texTypeName)
{
}
void crFocusDofTexMatMethod::inputParam(int i, void *param)
{
}

void crFocusDofTexMatMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_stageName = str;
		break;
	case 1:
		m_texTypeName = str;
		break;
	}
}

void crFocusDofTexMatMethod::operator()(crHandle &handle)
{
	crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_stageName);
	if(stage && !stage->isHide())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		crData *infoData = gamebodyInfo->getDataClass();
		infoData->inputParam(EDP_CurrentFocusTexture,&m_texTypeName);
		stage->updateWindow();
		stage->setNeedReDraw();
	}
}
/////////////////////////////////////////
//
//crSaveDofTexMatMethod
//
/////////////////////////////////////////
crSaveDofTexMatMethod::crSaveDofTexMatMethod(){}
crSaveDofTexMatMethod::crSaveDofTexMatMethod(const crSaveDofTexMatMethod& handle):
crMethod(handle),
m_gridElement(handle.m_gridElement)
{
}
void crSaveDofTexMatMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveDofTexMatMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crSaveDofTexMatMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crDOFTransform *dof = NULL;
			crDofUniformTexMatCallback *dofcallback = NULL;
			crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
			if(currentSelectObject)
			{
				void *param;
				crData *data = bodyInfo->getDataClass();
				data->getParam(EDP_CurrentFocusTexture,param);
				std::string focusTex = *(std::string *)param;
				if(!focusTex.empty())
				{
					dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(currentSelectObject->getCullCallback("DofUniformTexMat"));
					if(dofcallback)
					{
						dof = dofcallback->getDofTransform();
						if(dof->getName().compare(focusTex) != 0)
						{
							dof = NULL;
						}
						while(!dof)
						{
							dofcallback = dynamic_cast<crDofUniformTexMatCallback *>(dofcallback->getNestedCallback("DofUniformTexMat"));
							if(!dofcallback)
								break;
							dof = dofcallback->getDofTransform();
							if(dof->getName().compare(focusTex) != 0)
							{
								dof = NULL;
							}
						}
					}
				}
			}
			if(dof)
			{
				std::string filename = dofcallback->getCfgFile();
				std::string nameid = dofcallback->getUniformID();

				rcfg::ConfigScript cfg_script;
				if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
				{
					CRCore::notify(CRCore::FATAL)<<"crSaveDofTexMatMethod(): file open error,"<<filename<<std::endl;
					return;
				}

				int nCallback = 1;
				int nParam;
				std::string str;
				while(cfg_script.Push("NodeCallback", nCallback++))
				{
					cfg_script.Get("Class", str);
					if(str.compare("DofUniformTexMat") == 0)
					{//
						nParam = 1;
						if(cfg_script.Get("Param", str,nParam++))
						{
							if(str.compare(nameid) == 0)
							{//find
								//最小HPR
								CRCore::crVector3 minHPR = dof->getMinHPR();
								crVector3i rotHPR;
								rotHPR[0] = CRCore::RadiansToDegrees(minHPR[0]);
								rotHPR[1] = CRCore::RadiansToDegrees(minHPR[1]);
								rotHPR[2] = CRCore::RadiansToDegrees(minHPR[2]);
								str = crArgumentParser::appVectoa(rotHPR);
                                cfg_script.Modify("Param",str,nParam++);
                                //最大HPR
								CRCore::crVector3 maxHPR = dof->getMaxHPR();
								rotHPR[0] = CRCore::RadiansToDegrees(maxHPR[0]);
								rotHPR[1] = CRCore::RadiansToDegrees(maxHPR[1]);
								rotHPR[2] = CRCore::RadiansToDegrees(maxHPR[2]);
								str = crArgumentParser::appVectoa(rotHPR);
								cfg_script.Modify("Param",str,nParam++);
                                //当前HPR
								CRCore::crVector3 stepHPR = dof->getIncrementHPR();
								CRCore::crVector3 currentHPR = dof->getCurrentHPR();
								rotHPR[0] = stepHPR[0]!=0.0f?0:CRCore::RadiansToDegrees(currentHPR[0]);
								rotHPR[1] = stepHPR[1]!=0.0f?0:CRCore::RadiansToDegrees(currentHPR[1]);
								rotHPR[2] = stepHPR[2]!=0.0f?0:CRCore::RadiansToDegrees(currentHPR[2]);
								str = crArgumentParser::appVectoa(rotHPR);
								cfg_script.Modify("Param",str,nParam++);
								//增量HPR
								rotHPR[0] = CRCore::RadiansToDegrees(stepHPR[0]);
								rotHPR[1] = CRCore::RadiansToDegrees(stepHPR[1]);
								rotHPR[2] = CRCore::RadiansToDegrees(stepHPR[2]);
								str = crArgumentParser::appVectoa(rotHPR);
								cfg_script.Modify("Param",str,nParam++);
								//终点停止
								const CRCore::crVector3b& endStopHPR = dof->getEndStopHPR();
								crVector3i endStop;
								endStop[0] = endStopHPR[0]?1:0;
								endStop[1] = endStopHPR[1]?1:0;
								endStop[2] = endStopHPR[2]?1:0;
								str = crArgumentParser::appVectoa(endStop);
								cfg_script.Modify("Param",str,nParam++);

								//最小移动
								const CRCore::crVector3& minTrans = dof->getMinTranslate();
								str = crArgumentParser::appVectoa(minTrans);
								cfg_script.Modify("Param",str,nParam++);
								//最大移动
								const CRCore::crVector3& maxTrans = dof->getMaxTranslate();
								str = crArgumentParser::appVectoa(maxTrans);
								cfg_script.Modify("Param",str,nParam++);
								//当前移动
								const CRCore::crVector3& stepTrans = dof->getIncrementTranslate();
								const CRCore::crVector3& currentTrans = dof->getCurrentTranslate();
								crVector3 trans;
								trans[0] = stepTrans[0]!=0.0f?0.0f:currentTrans[0];
								trans[1] = stepTrans[1]!=0.0f?0.0f:currentTrans[1];
								trans[2] = stepTrans[2]!=0.0f?0.0f:currentTrans[2];
								str = crArgumentParser::appVectoa(trans);
								cfg_script.Modify("Param",str,nParam++);
								//增量移动
								str = crArgumentParser::appVectoa(stepTrans);
								cfg_script.Modify("Param",str,nParam++);
                                //终点停止
								const CRCore::crVector3b& endStopTrans = dof->getEndStopTrans();
								endStop[0] = endStopTrans[0]?1:0;
								endStop[1] = endStopTrans[1]?1:0;
								endStop[2] = endStopTrans[2]?1:0;
								str = crArgumentParser::appVectoa(endStop);
								cfg_script.Modify("Param",str,nParam++);

								//最小缩放
								const CRCore::crVector3& minScale = dof->getMinScale();
								str = crArgumentParser::appVectoa(minScale);
								cfg_script.Modify("Param",str,nParam++);
                                //最大缩放
								const CRCore::crVector3& maxScale = dof->getMaxScale();
								str = crArgumentParser::appVectoa(maxScale);
								cfg_script.Modify("Param",str,nParam++);
								//当前缩放
								const CRCore::crVector3& stepScale = dof->getIncrementScale();
								const CRCore::crVector3& currentScale = dof->getCurrentScale();
								crVector3 scale;
								scale[0] = stepScale[0]!=0.0f?0.0f:currentScale[0];
								scale[1] = stepScale[1]!=0.0f?0.0f:currentScale[1];
								scale[2] = stepScale[2]!=0.0f?0.0f:currentScale[2];
								str = crArgumentParser::appVectoa(scale);
								cfg_script.Modify("Param",str,nParam++);
                                //增量缩放
								str = crArgumentParser::appVectoa(stepScale);
								cfg_script.Modify("Param",str,nParam++);
                                //终点停止
								const CRCore::crVector3b& endStopScale = dof->getEndStopScale();
								endStop[0] = endStopScale[0]?1:0;
								endStop[1] = endStopScale[1]?1:0;
								endStop[2] = endStopScale[2]?1:0;
								str = crArgumentParser::appVectoa(endStop);
								cfg_script.Modify("Param",str,nParam++);

								//原点
								const CRCore::crVector3& O = dof->getOrigin();
								str = crArgumentParser::appVectoa(O);
								cfg_script.Modify("Param",str,nParam++);
                                //X轴
								const CRCore::crVector3& xAxis = dof->getXAxis();
								str = crArgumentParser::appVectoa(xAxis);
								cfg_script.Modify("Param",str,nParam++);
                                //Y轴
								const CRCore::crVector3& xyPlane = dof->getXYPlane();
								str = crArgumentParser::appVectoa(xyPlane);
								cfg_script.Modify("Param",str,nParam++);

								//限定标志
								//unsigned short limitationFlags = dof->getLimitationFlags();
								//std::string limitStr;
								//if(limitationFlags & crDOFTransform::XTran)
								//	limitStr = "XTran";
								//if(limitationFlags & crDOFTransform::YTran)
								//{
								//	if(limitStr.empty())
								//		limitStr = "YTran";
								//	else
								//		limitStr += "|YTran";
								//}
								//if(limitationFlags & crDOFTransform::ZTran)
								//{
								//	if(limitStr.empty())
								//		limitStr = "ZTran";
								//	else
								//		limitStr += "|ZTran";
								//}
								//if(limitationFlags & crDOFTransform::XRot)
								//{
								//	if(limitStr.empty())
								//		limitStr = "XRot";
								//	else
								//		limitStr += "|XRot";
								//}
								//if(limitationFlags & crDOFTransform::YRot)
								//{
								//	if(limitStr.empty())
								//		limitStr = "YRot";
								//	else
								//		limitStr += "|YRot";
								//}
								//if(limitationFlags & crDOFTransform::ZRot)
								//{
								//	if(limitStr.empty())
								//		limitStr = "ZRot";
								//	else
								//		limitStr += "|ZRot";
								//}
								//if(limitationFlags & crDOFTransform::XScale)
								//{
								//	if(limitStr.empty())
								//		limitStr = "XScale";
								//	else
								//		limitStr += "|XScale";
								//}
								//if(limitationFlags & crDOFTransform::YScale)
								//{
								//	if(limitStr.empty())
								//		limitStr = "YScale";
								//	else
								//		limitStr += "|YScale";
								//}
								//if(limitationFlags & crDOFTransform::ZScale)
								//{
								//	if(limitStr.empty())
								//		limitStr = "ZScale";
								//	else
								//		limitStr += "|ZScale";
								//}
								cfg_script.Modify("Param","ALL",nParam++);
								//开启运动
								bool animationOn = dof->getAnimationOn();
								cfg_script.Modify("Param",crArgumentParser::appItoa(animationOn?1:0),nParam++);
								//振荡标志
								unsigned short surgeFlags = dof->getSurgeFlags();
								std::string surgeStr;
								if(surgeFlags == crDOFTransform::ALL)
									surgeStr = "ALL";
								else
								{
									if(surgeFlags & (crDOFTransform::XTran|crDOFTransform::YTran|crDOFTransform::ZTran))
									{
                                        surgeStr = "XYZTran";
									}
									else
									{
										if(surgeFlags & crDOFTransform::XTran)
											surgeStr = "XTran";
										if(surgeFlags & crDOFTransform::YTran)
										{
											if(surgeStr.empty())
												surgeStr = "YTran";
											else
												surgeStr += "|YTran";
										}
										if(surgeFlags & crDOFTransform::ZTran)
										{
											if(surgeStr.empty())
												surgeStr = "ZTran";
											else
												surgeStr += "|ZTran";
										}
									}
									if(surgeFlags & (crDOFTransform::XRot|crDOFTransform::YRot|crDOFTransform::ZRot))
									{
										surgeStr = "XYZRot";
									}
									else
									{
										if(surgeFlags & crDOFTransform::XRot)
										{
											if(surgeStr.empty())
												surgeStr = "XRot";
											else
												surgeStr += "|XRot";
										}
										if(surgeFlags & crDOFTransform::YRot)
										{
											if(surgeStr.empty())
												surgeStr = "YRot";
											else
												surgeStr += "|YRot";
										}
										if(surgeFlags & crDOFTransform::ZRot)
										{
											if(surgeStr.empty())
												surgeStr = "ZRot";
											else
												surgeStr += "|ZRot";
										}
									}
									if(surgeFlags & (crDOFTransform::XScale|crDOFTransform::YScale|crDOFTransform::ZScale))
									{
										surgeStr = "XYZScale";
									}
									else
									{
										if(surgeFlags & crDOFTransform::XScale)
										{
											if(surgeStr.empty())
												surgeStr = "XScale";
											else
												surgeStr += "|XScale";
										}
										if(surgeFlags & crDOFTransform::YScale)
										{
											if(surgeStr.empty())
												surgeStr = "YScale";
											else
												surgeStr += "|YScale";
										}
										if(surgeFlags & crDOFTransform::ZScale)
										{
											if(surgeStr.empty())
												surgeStr = "ZScale";
											else
												surgeStr += "|ZScale";
										}
									}
								}
								cfg_script.Modify("Param",surgeStr,nParam++);
								//无穷
								bool endless = dof->getEndless();
								cfg_script.Modify("Param",crArgumentParser::appItoa(endless?1:0),nParam++);
								//生命周期
								float lifeTime = dof->getLifeTime();
								cfg_script.Modify("Param",crArgumentParser::appFtoa(lifeTime),nParam++);
								//开始时间
								float startTime = dof->getStartTime();
								cfg_script.Modify("Param",crArgumentParser::appFtoa(startTime),nParam++);
								//重置时间
								float resetTime = dof->getResetTime();
								cfg_script.Modify("Param",crArgumentParser::appFtoa(resetTime),nParam++);

								if (!cfg_script.Pop())
									CRCore::notify(CRCore::FATAL)<<"crSaveDofTexMatMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
								break;
							}
						}
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"crSaveDofTexMatMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
				}
				cfg_script.Write(filename);
				cfg_script.Close();
				CRIOManager::crWriteCookFile scopedWrite(filename);
			}
		}
	}
}
/////////////////////////////////////////
//
//crNextDofTexMatMethod
//
/////////////////////////////////////////
crNextDofTexMatMethod::crNextDofTexMatMethod(){}
crNextDofTexMatMethod::crNextDofTexMatMethod(const crNextDofTexMatMethod& handle):
crMethod(handle)
{
}
void crNextDofTexMatMethod::inputParam(int i, void *param)
{
	if(i == 0)
		m_element = (CRGUI::crElement*)param;
}

void crNextDofTexMatMethod::addParam(int i, const std::string& str)
{
}

void crNextDofTexMatMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	void *param;
	crData *data = gamebodyInfo->getDataClass();
	data->getParam(EDP_CurrentFocusTexture,param);
	std::string focusTex = *(std::string *)param;
	std::string nextTex;
	if(focusTex.compare("颜色1") == 0)
	{
        nextTex = "颜色1Alpha";
	}
	else if(focusTex.compare("颜色1Alpha") == 0)
	{
		nextTex = "颜色1";
	}
	else if(focusTex.compare("颜色2") == 0)
	{
		nextTex = "颜色2Alpha";
	}
	else if(focusTex.compare("颜色2Alpha") == 0)
	{
		nextTex = "颜色2";
	}
	else if(focusTex.compare("颜色3") == 0)
	{
		nextTex = "颜色3Alpha";
	}
	else if(focusTex.compare("颜色3Alpha") == 0)
	{
		nextTex = "颜色3";
	}
	else if(focusTex.compare("法线1") == 0)
	{
		nextTex = "法线1";
	}
	else if(focusTex.compare("法线2") == 0)
	{
		nextTex = "法线2";
	}
	else if(focusTex.compare("法线3") == 0)
	{
		nextTex = "法线3";
	}
	else if(focusTex.compare("高光1") == 0)
	{
		nextTex = "高光指数1Alpha";
	}
	else if(focusTex.compare("高光指数1Alpha") == 0)
	{
		nextTex = "高光1";
	}
	else if(focusTex.compare("高光2") == 0)
	{
		nextTex = "高光指数2Alpha";
	}
	else if(focusTex.compare("高光指数2Alpha") == 0)
	{
		nextTex = "高光2";
	}
	else if(focusTex.compare("高光3") == 0)
	{
		nextTex = "高光指数3Alpha";
	}
	else if(focusTex.compare("高光指数3Alpha") == 0)
	{
		nextTex = "高光3";
	}
	else if(focusTex.compare("自发光1或法线4") == 0)
	{
		nextTex = "Alpha扭曲1";
	}
	else if(focusTex.compare("Alpha扭曲1") == 0)
	{
		nextTex = "自发光1或法线4";
	}
	else if(focusTex.compare("自发光2或颜色4") == 0)
	{
		nextTex = "Alpha扭曲2";
	}
	else if(focusTex.compare("Alpha扭曲2") == 0)
	{
		nextTex = "自发光2或颜色4";
	}
	else if(focusTex.compare("自发光3或高光4") == 0)
	{
		nextTex = "Alpha扭曲3";
	}
	else if(focusTex.compare("Alpha扭曲3") == 0)
	{
		nextTex = "自发光3或高光4";
	}
	else if(focusTex.compare("UV扰动贴图") == 0)
	{
		nextTex = "UV扰动贴图";
	}
	else if(focusTex.compare("通道控制贴图") == 0)
	{
		nextTex = "通道控制贴图R";
	}
	else if(focusTex.compare("通道控制贴图R") == 0)
	{
		nextTex = "通道控制贴图G";
	}
	else if(focusTex.compare("通道控制贴图G") == 0)
	{
		nextTex = "通道控制贴图B";
	}
	else if(focusTex.compare("通道控制贴图B") == 0)
	{
		nextTex = "通道控制贴图A";
	}
	else if(focusTex.compare("通道控制贴图A") == 0)
	{
		nextTex = "通道控制贴图";
	}

	data->inputParam(EDP_CurrentFocusTexture,&nextTex);
	crImageStage *stage = m_element->getParentStage();
	stage->updateWindow();
	stage->setNeedReDraw();
}
/////////////////////////////////////////
//
//crShowEditorMenuMethod
//
/////////////////////////////////////////
crShowEditorMenuMethod::crShowEditorMenuMethod(){}
crShowEditorMenuMethod::crShowEditorMenuMethod(const crShowEditorMenuMethod& handle):
crMethod(handle),
m_editorMenu(handle.m_editorMenu)
{
}
void crShowEditorMenuMethod::inputParam(int i, void *param)
{
}

void crShowEditorMenuMethod::addParam(int i, const std::string& str)
{
	if(i == 0)
		m_editorMenu = str;
}

void crShowEditorMenuMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
	crData *data = gamebodyInfo->getDataClass();
	void *param;
	data->getParam(EDP_CurrentEditorMenu,param);
	std::string menu = *(std::string *)param;
	if(menu.compare(m_editorMenu) == 0)
		return;
	if(!menu.empty())
	{
		cr2DStageManager::getInstance()->close(menu);
	}
	data->inputParam(EDP_CurrentEditorMenu,&m_editorMenu);
	if(!m_editorMenu.empty())
	    cr2DStageManager::getInstance()->showStage(m_editorMenu,true);
}
/////////////////////////////////////////
//
//crNewParticleMethod
//
/////////////////////////////////////////
crNewParticleMethod::crNewParticleMethod(){}
crNewParticleMethod::crNewParticleMethod(const crNewParticleMethod& handle):
crMethod(handle),
m_rootName(handle.m_rootName)
{
}
void crNewParticleMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_copyData = (crGroup *)param;
		break;
	}
}

void crNewParticleMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_rootName = str;
		break;
	}
}

void crNewParticleMethod::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_root)
	{
		ref_ptr<crMatrixTransform> effectGroup;
		if(m_copyData.valid())
		{
			effectGroup = dynamic_cast<crMatrixTransform *>(m_copyData->clone(crCopyOp::DEEP_COPY_NODES)); 
		}
		else
		{
			effectGroup = new crMatrixTransform;
			ref_ptr<crParticleEffect>particleEffect = new crParticleEffect;
			particleEffect->setName("PS");
			ref_ptr<crParticleSystem> ps = new crParticleSystem;
			particleEffect->setParticleSystem(ps.get());
			ref_ptr<crModularEmitter> emitter = new crModularEmitter;
			particleEffect->setEmitter(emitter.get());
			ref_ptr<CRParticle::crProgram> program = new crFluidProgram;
			particleEffect->setProgram(program.get());
			effectGroup->addChild(particleEffect.get());
		}
		char buf[16];
		std::string psName;
		int safeid = 0;
		int id;
		crParticleSystemUpdater::getInstance()->lock();
		crParticleSystemUpdater::ParticleEffectSet &effectSet = crParticleSystemUpdater::getInstance()->getParticleEffectSet();
		for( crParticleSystemUpdater::ParticleEffectSet::iterator itr = effectSet.begin();
			itr != effectSet.end();
			++itr )
		{
			psName = (*itr)->getParent(0)->getName();
			if(psName.find("NewEffect")==0)
			{
				psName.erase(0,9);
				id = atoi(psName.c_str());
				if(safeid<=id) safeid = id+1;
			}
		}
		sprintf(buf,"NewEffect%d\0",safeid);
		effectGroup->setName(buf);
		crParticleSystemUpdater::getInstance()->unlock();

		CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
		proxyNode->setName("ProxyNode");
		proxyNode->setDescriptionsInString("DataFile tmp.cre");
		proxyNode->addChild(effectGroup.get());
		CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root,proxyNode.get(),true);

		std::string des = "EditorNodeType:ParticleModel\nEditorNodeName:"+effectGroup->getName();
		handle.outputParam(0,&des);
		handle.outputParam(2,effectGroup.get());
	}
}
/////////////////////////////////////////
//
//crDisplayParticleParamMethod
//
/////////////////////////////////////////
crDisplayParticleParamMethod::crDisplayParticleParamMethod(){}
crDisplayParticleParamMethod::crDisplayParticleParamMethod(const crDisplayParticleParamMethod& handle):
crMethod(handle),
m_filePath(handle.m_filePath),
m_currentSelectElement(handle.m_currentSelectElement),
m_indexElement(handle.m_indexElement),
m_gridElement(handle.m_gridElement)
{
}
void crDisplayParticleParamMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crDisplayParticleParamMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
        m_filePath = str;
		break;
	case 1:
		m_currentSelectElement = str;
		break;
	case 2:
		m_indexElement = str;
		break;
	case 3:
		m_gridElement = str;
		break;
	}
}
std::string crDisplayParticleParamMethod::getParticleDisplayName(const std::string &str)
{
	if(str.compare("crParticleSystem")==0)
		return "标准粒子系统";
	else if(str.compare("crModularEmitter")==0)
		return "标准发射器";
	else if(str.compare("crRandomRateCounter")==0)
		return "随机计数器";
	else if(str.compare("crConstantRateCounter")==0)
		return "常数计数器";
	else if(str.compare("crConstantCounter")==0)
		return "固定数量";
	else if(str.compare("crBoxPlacer")==0)
		return "箱体放置器";
	else if(str.compare("crSegmentPlacer")==0)
		return "区域放置器";
	else if(str.compare("crSectorPlacer")==0)
		return "平面范围放置器";
	else if(str.compare("crMultiSegmentPlacer")==0)
		return "多区域放置器";
	else if(str.compare("crPointPlacer")==0)
		return "点放置器";
	//操作
	else if(str.compare("crDampingOperator")==0)
		return "速度衰减";
	else if(str.compare("crOrbitOperator")==0)
		return "盘旋";
	else if(str.compare("crFluidFrictionOperator")==0)
		return "流体操作";
	else if(str.compare("crForceOperator")==0)
		return "力";
	else if(str.compare("crAngularDampingOperator")==0)
		return "角速度衰减";
	else if(str.compare("crAngularAccelOperator")==0)
		return "角加速度";
	else if(str.compare("crAccelOperator")==0)
		return "加速度";
	else if(str.compare("crExplosionOperator")==0)
		return "爆炸";
	//Shooter
	else if(str.compare("crRadialShooter")==0)
		return "标准Shooter";
	//Program
	else if(str.compare("crFluidProgram")==0)
		return "流体Program";
	else if(str.compare("crModularProgram")==0)
		return "标准Program";
	return "";
}
std::string crDisplayParticleParamMethod::getAlignedModeStr(int mode)
{
	switch (mode)
	{
	case crParticle::AXIAL_None:
		return "None";
	case crParticle::AXIAL_ROT_X_AXIS:
		return "XRot";
	case crParticle::AXIAL_ROT_Y_AXIS:
		return "YRot";
	case crParticle::AXIAL_ROT_Z_AXIS:
		return "ZRot";
	case crParticle::POINT_ROT_EYE:
		return "PointRot";
	case crParticle::POINT_ROT_WORLD_Z_AXIS:
		return "PointRotZ";
	}
	return "None";
}
void crDisplayParticleParamMethod::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crImageStage *stage = m_listbox->getParentStage();
		std::string selectName;
		m_listbox->getData(m_listbox->getSelect(),selectName);
		crGroup * selectEffect = dynamic_cast<crGroup *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));

		std::string str;
		crEditBoxElement *filePathElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_filePath));
		if(filePathElement)
		{
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setNameId("ProxyNode");
			searchByNameVisitor.setSearchNodeType(ALLNODE);
			selectEffect->accept(searchByNameVisitor);
			crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(proxyNode)
			{
				if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
				{
					filePathElement->setStringArrayByString(str);
				}
				else
					filePathElement->clearString();
			}
			else
				filePathElement->clearString();
		}
		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
			currentSelectElement->setStringArrayByString(selectName);
		}
		int index = 0;
		crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
		if(indexElement)
		{
			index = atoi(indexElement->getStringArrayInString().c_str());
			if(index<0 || index>=selectEffect->getNumChildren())
			{
				index = 0;
				char buf[16];
				sprintf(buf,"%d/%d\0",index,selectEffect->getNumChildren()-1);
				indexElement->setStringArrayByString(buf);
			}
		}
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();
			crParticleEffect *pe = dynamic_cast<crParticleEffect *>(selectEffect->getChild(index));
			if(pe)
			{
				gridElement->addData("节点名",pe->getName(),"");
				gridElement->addData("渲染阴影",pe->isCalcShadow()?"1":"0","");
					//ParticleSystem
				crParticleSystem* ps = pe->getParticleSystem();

				gridElement->addData("模版",getParticleDisplayName(ps->className()),"Red13");
				crParticleSystem::Alignment align = ps->getParticleAlignment();
                if(align == crParticleSystem::BILLBOARD)
					gridElement->addData("正对相机","1","Red13");
				else
					gridElement->addData("正对相机","0","Red13");
				std::string tsstr = ps->getTraverseString();
				gridElement->addData("渲染方式",tsstr,"Red13");
				gridElement->addData("LOD",crArgumentParser::appFtoa(ps->getLodValue()),"Red13");
				bool rendDepth = ps->getDoublePassRendering();
				gridElement->addData("渲染深度",rendDepth?"1":"0","Red13");
				crParticle& def_particle = ps->getDefaultParticleTemplate();
				crParticle::Shape shape = def_particle.getShape();
				switch (shape)
				{
				case crParticle::POINT:
					gridElement->addData("形状","点","Red13");
					break;
				case crParticle::QUAD:
					gridElement->addData("形状","正方形","Red13");
					break;
				case crParticle::QUAD2:
					gridElement->addData("形状","正方形2","Red13");
					break;
				case crParticle::QUAD_TRIANGLESTRIP:
					gridElement->addData("形状","正方形三角化","Red13");
					break;
				case crParticle::HEXAGON:
					gridElement->addData("形状","六边形","Red13");
					break;
				case crParticle::LINE:
					gridElement->addData("形状","线","Red13");
					break;
				case crParticle::Object:
					gridElement->addData("形状","模型"+crArgumentParser::appVectoa(crVector2i(def_particle.getDrawableIdRange().minimum,def_particle.getDrawableIdRange().maximum)),"Red13");
					break;
				}
				gridElement->addData("半径",crArgumentParser::appFtoa(def_particle.getRadius()),"Red13");
				float radius = def_particle.getRadius();
				float mass = def_particle.getMass();
				float density = mass/(radius*radius*radius*CRCore::PI*4.0f/3.0f);
				gridElement->addData("密度",crArgumentParser::appFtoa(density),"Red13");
				gridElement->addData("大小范围1",crArgumentParser::appVectoa(crVector3(def_particle.getSizeRange().minimum)),"Red13");
				gridElement->addData("大小范围2",crArgumentParser::appVectoa(crVector3(def_particle.getSizeRange().maximum)),"Red13");
				gridElement->addData("透明度范围",crArgumentParser::appVectoa(crVector2(def_particle.getAlphaRange().minimum,def_particle.getAlphaRange().maximum)),"Red13");
				gridElement->addData("颜色范围1",crArgumentParser::appVectoa(crVector4i(def_particle.getColorRange().minimum * 255.0f)),"Red13");
				gridElement->addData("颜色范围2",crArgumentParser::appVectoa(crVector4i(def_particle.getColorRange().maximum * 255.0f)),"Red13");
				gridElement->addData("生命周期",crArgumentParser::appFtoa(def_particle.getLifeTime()),"Red13");
				gridElement->addData("纹理阵列",crArgumentParser::appVectoa(crVector2i(def_particle.getTileS(),def_particle.getTileT())),"Red13");
				gridElement->addData("纹理数量",crArgumentParser::appItoa(def_particle.getNumTiles()),"Red13");
				crVector3 vec3 = def_particle.getStaticAngle();
				vec3[0] = CRCore::RadiansToDegrees(vec3[0]);
				vec3[1] = CRCore::RadiansToDegrees(vec3[1]);
				vec3[2] = CRCore::RadiansToDegrees(vec3[2]);
				gridElement->addData("旋转角度",crArgumentParser::appVectoa(vec3),"Red13");
				gridElement->addData("Billboard",getAlignedModeStr(def_particle.getAlignedMode()),"Red13");
				//Emitter
				crEmitter* emitter = pe->getEmitter();
				gridElement->addData("发射器",getParticleDisplayName(emitter->className()),"");
				if(dynamic_cast<CRParticle::crModularEmitter *>(emitter))
				{
					crModularEmitter *me = dynamic_cast<CRParticle::crModularEmitter *>(emitter);
					gridElement->addData("开启",crArgumentParser::appItoa(me->isEnabled()?1:0),"");
					gridElement->addData("无穷",crArgumentParser::appItoa(me->isEndless()?1:0),"");
					gridElement->addData("持续时间",crArgumentParser::appFtoa(me->getLifeTime()),"");
					gridElement->addData("开始时间",crArgumentParser::appFtoa(me->getStartTime()),"");
					gridElement->addData("重置时间",crArgumentParser::appFtoa(me->getResetTime()),"");
					gridElement->addData("比率",crArgumentParser::appFtoa(me->getNumParticlesToCreateMovementCompenstationRatio()),"");
					if(me->getReferenceFrame() == crParticleProcessor::RELATIVE_RF)
						gridElement->addData("绝对坐标","0","");
					else
						gridElement->addData("绝对坐标","1","");
                    //Counter
					CRParticle::crCounter *counter = me->getCounter();
					gridElement->addData("计数器",getParticleDisplayName(counter->className()),"");
					if(dynamic_cast<CRParticle::crRandomRateCounter *>(counter))
					{
						crRandomRateCounter *rrc = dynamic_cast<CRParticle::crRandomRateCounter *>(counter);
						gridElement->addData("数量范围",crArgumentParser::appVectoa(crVector2(rrc->getRateRange().minimum,rrc->getRateRange().maximum)),"");
					}
					else if(dynamic_cast<CRParticle::crConstantRateCounter *>(counter))
					{
						crConstantRateCounter *crc = dynamic_cast<CRParticle::crConstantRateCounter *>(counter);
						gridElement->addData("最小数量",crArgumentParser::appItoa(crc->getMinimumNumberOfParticlesToCreate()),"");
						gridElement->addData("每秒创建数量",crArgumentParser::appFtoa(crc->getNumberOfParticlesPerSecondToCreate()),"");
					}
					else if(dynamic_cast<CRParticle::crConstantCounter *>(counter))
					{
						crConstantCounter *cc = dynamic_cast<CRParticle::crConstantCounter *>(counter);
						gridElement->addData("数量",crArgumentParser::appItoa(cc->getNumberOfParticlesToCreate()),"");
					}
					//Placer
					CRParticle::crPlacer *placer = me->getPlacer();
					gridElement->addData("放置器",getParticleDisplayName(placer->className()),"");
					if(dynamic_cast<CRParticle::crSectorPlacer *>(placer))
					{
						crSectorPlacer *sp = dynamic_cast<CRParticle::crSectorPlacer *>(placer);
						gridElement->addData("中心点",crArgumentParser::appVectoa(sp->getCenter()),"");
						gridElement->addData("半径",crArgumentParser::appVectoa(crVector2(sp->getRadiusRange().minimum,sp->getRadiusRange().maximum)),"");
						//gridElement->addData("Phi范围",crArgumentParser::appVectoa(crVector2(sp->getPhiRange().minimum,sp->getPhiRange().maximum)),"");
					}
					else if(dynamic_cast<CRParticle::crBoxPlacer *>(placer))
					{
						crBoxPlacer *bp = dynamic_cast<CRParticle::crBoxPlacer *>(placer);
						gridElement->addData("中心点",crArgumentParser::appVectoa(bp->getCenter()),"");
						gridElement->addData("X范围",crArgumentParser::appVectoa(crVector2(bp->getXRange().minimum,bp->getXRange().maximum)),"");
						gridElement->addData("Y范围",crArgumentParser::appVectoa(crVector2(bp->getYRange().minimum,bp->getYRange().maximum)),"");
						gridElement->addData("Z范围",crArgumentParser::appVectoa(crVector2(bp->getZRange().minimum,bp->getZRange().maximum)),"");
					}
					else if(dynamic_cast<CRParticle::crPointPlacer *>(placer))
					{
						crPointPlacer *pp = dynamic_cast<CRParticle::crPointPlacer *>(placer);
						gridElement->addData("中心点",crArgumentParser::appVectoa(pp->getCenter()),"");
					}
					else if(dynamic_cast<CRParticle::crMultiSegmentPlacer *>(placer))
					{
						crMultiSegmentPlacer *msp = dynamic_cast<CRParticle::crMultiSegmentPlacer *>(placer);
						int num = msp->numVertices();
						gridElement->addData("坐标数量",crArgumentParser::appItoa(num),"");
						for( int i = 0; i<num; ++i )
						{
							gridElement->addData("坐标",crArgumentParser::appVectoa(msp->getVertex(i)),"");
						}
					}
					else if(dynamic_cast<CRParticle::crSegmentPlacer *>(placer))
					{
						crSegmentPlacer *sp = dynamic_cast<CRParticle::crSegmentPlacer *>(placer);
						gridElement->addData("坐标1",crArgumentParser::appVectoa(sp->getVertexA()),"");
						gridElement->addData("坐标2",crArgumentParser::appVectoa(sp->getVertexB()),"");
					}
					//Shooter
					CRParticle::crShooter *shooter = me->getShooter();
					gridElement->addData("Shooter",getParticleDisplayName(shooter->className()),"");
					if(dynamic_cast<CRParticle::crRadialShooter *>(shooter))
					{
						crRadialShooter *rs = dynamic_cast<CRParticle::crRadialShooter *>(shooter);
						gridElement->addData("速度范围",crArgumentParser::appVectoa(crVector2(rs->getInitialSpeedRange().minimum,rs->getInitialSpeedRange().maximum)),"");
						//gridElement->addData("Theta范围",crArgumentParser::appVectoa(crVector2(rs->getThetaRange().minimum,rs->getThetaRange().maximum)),"");
						//gridElement->addData("Phi范围",crArgumentParser::appVectoa(crVector2(rs->getPhiRange().minimum,rs->getPhiRange().maximum)),"");
						gridElement->addData("角速度范围1",crArgumentParser::appVectoa(rs->getInitialRotationalSpeedRange().minimum),"");
						gridElement->addData("角速度范围2",crArgumentParser::appVectoa(rs->getInitialRotationalSpeedRange().maximum),"");
					}
				}
				//Program
				CRParticle::crProgram* program = pe->getProgram();
				gridElement->addData("Program",getParticleDisplayName(program->className()),"Blue13");
				if(dynamic_cast<CRParticle::crModularProgram *>(program))
				{
					crModularProgram *mp = dynamic_cast<CRParticle::crModularProgram *>(program);
					int num = mp->numOperators();
					gridElement->addData("操作数量",crArgumentParser::appItoa(num),"Blue13");
					for( int i = 0; i<num; ++i )
					{
						crOperator* op = mp->getOperator(i);
						gridElement->addData("操作",getParticleDisplayName(op->className()),"Blue13");
						if(dynamic_cast<CRParticle::crAccelOperator *>(op))
						{
							crAccelOperator *aop = dynamic_cast<CRParticle::crAccelOperator *>(op);
							gridElement->addData("加速度",crArgumentParser::appVectoa(aop->getAcceleration()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crAngularAccelOperator *>(op))
						{
							crAngularAccelOperator *aaop = dynamic_cast<CRParticle::crAngularAccelOperator *>(op);
							gridElement->addData("角加速度",crArgumentParser::appVectoa(aaop->getAngularAcceleration()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crFluidFrictionOperator *>(op))
						{
							crFluidFrictionOperator *ffop = dynamic_cast<CRParticle::crFluidFrictionOperator *>(op);
							gridElement->addData("流体密度",crArgumentParser::appFtoa(ffop->getFluidDensity()),"Blue13");
							gridElement->addData("流体粘性",crArgumentParser::appFtoa(ffop->getFluidViscosity()),"Blue13");
							gridElement->addData("忽略粒子半径",crArgumentParser::appFtoa(ffop->getOverrideRadius()),"Blue13");
							gridElement->addData("风力",crArgumentParser::appVectoa(ffop->getWind()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crForceOperator *>(op))
						{
							crForceOperator *fop = dynamic_cast<CRParticle::crForceOperator *>(op);
							gridElement->addData("力",crArgumentParser::appVectoa(fop->getForce()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crAngularDampingOperator *>(op))
						{
							crAngularDampingOperator *_op = dynamic_cast<CRParticle::crAngularDampingOperator *>(op);
							gridElement->addData("衰减",crArgumentParser::appVectoa(_op->getDamping()),"Blue13");
							gridElement->addData("最小角速度",crArgumentParser::appFtoa(_op->getCutoffLow()),"Blue13");
							//gridElement->addData("最大角速度",crArgumentParser::appFtoa(_op->getCutoffHigh()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crDampingOperator *>(op))
						{
							crDampingOperator *_op = dynamic_cast<CRParticle::crDampingOperator *>(op);
							gridElement->addData("衰减",crArgumentParser::appVectoa(_op->getDamping()),"Blue13");
							gridElement->addData("最小速度",crArgumentParser::appFtoa(_op->getCutoffLow()),"Blue13");
							//gridElement->addData("最大速度",crArgumentParser::appFtoa(_op->getCutoffHigh()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crExplosionOperator *>(op))
						{
							crExplosionOperator *_op = dynamic_cast<CRParticle::crExplosionOperator *>(op);
							gridElement->addData("中心点",crArgumentParser::appVectoa(_op->getCenter()),"Blue13");
							gridElement->addData("半径",crArgumentParser::appFtoa(_op->getRadius()),"Blue13");
							gridElement->addData("Magnitude",crArgumentParser::appFtoa(_op->getMagnitude()),"Blue13");
							gridElement->addData("Epsilon",crArgumentParser::appFtoa(_op->getEpsilon()),"Blue13");
							gridElement->addData("Sigma",crArgumentParser::appFtoa(_op->getSigma()),"Blue13");
						}
						else if(dynamic_cast<CRParticle::crOrbitOperator *>(op))
						{
							crOrbitOperator *_op = dynamic_cast<CRParticle::crOrbitOperator *>(op);
							gridElement->addData("中心点",crArgumentParser::appVectoa(_op->getCenter()),"Blue13");
							gridElement->addData("Magnitude",crArgumentParser::appFtoa(_op->getMagnitude()),"Blue13");
							gridElement->addData("Epsilon",crArgumentParser::appFtoa(_op->getEpsilon()),"Blue13");
							//gridElement->addData("最大半径",crArgumentParser::appFtoa(_op->getMaxRadius()),"Blue13");
						}
					}
				}
				else if(dynamic_cast<CRParticle::crFluidProgram *>(program))
				{
					crFluidProgram *fp = dynamic_cast<CRParticle::crFluidProgram *>(program);
					gridElement->addData("风力",crArgumentParser::appVectoa(fp->getWind()),"Blue13");
					switch(fp->getFluidType())
					{
					case crFluidProgram::Air:
						gridElement->addData("流体类型","空气","Blue13");
						break;
					case crFluidProgram::Water:
						gridElement->addData("流体类型","水","Blue13");
						break;
					case crFluidProgram::UserDefine:
						gridElement->addData("流体类型","自定义","Blue13");
						gridElement->addData("流体密度",crArgumentParser::appFtoa(fp->getFluidDensity()),"Blue13");
						gridElement->addData("流体粘性",crArgumentParser::appFtoa(fp->getFluidViscosity()),"Blue13");
						gridElement->addData("加速度",crArgumentParser::appVectoa(fp->getAcceleration()),"Blue13");
						break;
					}
				}
				////////////////////////////
				gridElement->addData("END","NULL","");
			}
			gridElement->checkSelect();
			gridElement->updateInputElement(true);
		}
	}
}
/////////////////////////////////////////
//
//crModifyParticleParamMethod
//
/////////////////////////////////////////
crModifyParticleParamMethod::crModifyParticleParamMethod(){}
crModifyParticleParamMethod::crModifyParticleParamMethod(const crModifyParticleParamMethod& handle):
crMethod(handle),
m_editorRootName(handle.m_editorRootName),
m_listElement(handle.m_listElement),
m_currentSelectElement(handle.m_currentSelectElement),
m_indexElement(handle.m_indexElement),
m_gridElement(handle.m_gridElement)
{
}
void crModifyParticleParamMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyParticleParamMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_editorRootName = str;
		break;
	case 1:
		m_listElement = str;
		break;
	case 2:
		m_currentSelectElement = str;
		break;
	case 3:
		m_indexElement = str;
		break;
	case 4:
		m_gridElement = str;
		break;
	}
}
std::string crModifyParticleParamMethod::getParticleClassName(const std::string &str)
{
    if(str.compare("标准粒子系统")==0)
		return "crParticleSystem";
	else if(str.compare("标准发射器")==0)
		return "crModularEmitter";
	else if(str.compare("随机计数器")==0)
		return "crRandomRateCounter";
	else if(str.compare("常数计数器")==0)
		return "crConstantRateCounter";
	else if(str.compare("固定数量")==0)
		return "crConstantCounter";
	else if(str.compare("箱体放置器")==0)
		return "crBoxPlacer";
	else if(str.compare("区域放置器")==0)
		return "crSegmentPlacer";
	else if(str.compare("平面范围放置器")==0)
		return "crSectorPlacer";
	else if(str.compare("多区域放置器")==0)
		return "crMultiSegmentPlacer";
	else if(str.compare("点放置器")==0)
		return "crPointPlacer";
	//操作
	else if(str.compare("速度衰减")==0)
		return "crDampingOperator";
	else if(str.compare("盘旋")==0)
		return "crOrbitOperator";
	else if(str.compare("流体操作")==0)
		return "crFluidFrictionOperator";
	else if(str.compare("力")==0)
		return "crForceOperator";
	else if(str.compare("角速度衰减")==0)
		return "crAngularDampingOperator";
	else if(str.compare("角加速度")==0)
		return "crAngularAccelOperator";
	else if(str.compare("加速度")==0)
		return "crAccelOperator";
	else if(str.compare("爆炸")==0)
		return "crExplosionOperator";
	//Shooter
	else if(str.compare("标准Shooter")==0)
		return "crRadialShooter";
	//Program
	else if(str.compare("流体Program")==0)
		return "crFluidProgram";
	else if(str.compare("标准Program")==0)
		return "crModularProgram";
	return "";
}
int crModifyParticleParamMethod::getAlignedMode(const std::string &str)
{
	if(str.compare("None")==0)
		return crParticle::AXIAL_None;
	else if(str.compare("XRot")==0)
		return crParticle::AXIAL_ROT_X_AXIS;
	else if(str.compare("YRot")==0)
		return crParticle::AXIAL_ROT_Y_AXIS;
	else if(str.compare("ZRot")==0)
		return crParticle::AXIAL_ROT_Z_AXIS;
	else if(str.compare("PointRot")==0)
		return crParticle::POINT_ROT_EYE;
	else if(str.compare("PointRotZ")==0)
		return crParticle::POINT_ROT_WORLD_Z_AXIS;
	return crParticle::AXIAL_None;
}
void crModifyParticleParamMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffect = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(!selectEffect) return;

			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId(m_editorRootName);
			searchByNameVisitor.setSearchNodeType(GROUP);
			CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
			m_editorRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(selectEffect->getName());
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			m_editorRoot->accept(searchByNameVisitor);
			crMatrixTransform* matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());

			std::string str;
			crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(stage->getElement(m_currentSelectElement));
			if(currentSelectElement)
			{
				str = currentSelectElement->getStringArrayInString();
				selectEffect->setName(str);
				listbox->modifyData(listbox->getSelect(),str);

				if(matrixTransform)
				{
					matrixTransform->setName(str);
					std::string des = matrixTransform->getDescriptionsInString();
					crArgumentParser::updateOrAddKeyValue(des,"EditorNodeName",str);
					matrixTransform->setDescriptionsInString(des);
					matrixTransform->setMatrix(selectEffect->getMatrix());
				}
			}
			int index = 0;
			crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
			if(indexElement)
			{
				index = atoi(indexElement->getStringArrayInString().c_str());
				if(index<0 || index>=selectEffect->getNumChildren())
				{
					index = 0;
					char buf[16];
					sprintf(buf,"%d/%d\0",index,selectEffect->getNumChildren()-1);
					indexElement->setStringArrayByString(buf);
					return;
				}
			}
			crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
			if(gridElement/*&& proxyObject*/)
			{
				crParticleEffect *pe = dynamic_cast<crParticleEffect *>(selectEffect->getChild(index));
				if(pe)
				{
					int gridIndex = 0;
					crKeyValueNode *keyValueNode;
					std::string str;
					crVector2 vec2;
					crVector3 vec3;
				    int int1;

					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					pe->setName(str);
					//"渲染阴影"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					pe->setCalcShadow(atoi(str.c_str()));
					//ParticleSystem
					crParticleSystem* ps = pe->getParticleSystem();
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					str = getParticleClassName(str);
					if(str.compare(ps->className()) != 0)
					{
						if(str.compare("crParticleSystem") == 0)
						{
							ps = new crParticleSystem;
						}
						else if(str.compare("crConnectedParticleSystem") == 0)
						{
							ps = new crConnectedParticleSystem;
						}
						if(pe->getParticleSystem()->getParticleModelObject())
							ps->setParticleModelObject(pe->getParticleSystem()->getParticleModelObject());
						else
						    ps->setStateSet(pe->getParticleSystem()->getStateSet());
						pe->setParticleSystem(ps);
					}
					//"正对相机"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					int1 = atoi(str.c_str());
					if(int1)
						ps->setParticleAlignment(crParticleSystem::BILLBOARD);
					else
						ps->setParticleAlignment(crParticleSystem::FIXED);
					//"渲染方式"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					ps->setTraverseString(str);
					//"LOD"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					ps->setLodValue(atof(str.c_str()));
					//"渲染深度"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					ps->setDoublePassRendering(atoi(str.c_str()));
                    //DefaultParticleTemplate
					crParticle& def_particle = ps->getDefaultParticleTemplate();
                    //"形状"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					if(str.compare("点") == 0)
					{
						def_particle.setShape(crParticle::POINT);
					}
					else if(str.compare("正方形") == 0)
					{
						def_particle.setShape(crParticle::QUAD);
					}
					else if(str.compare("正方形2") == 0)
					{
						def_particle.setShape(crParticle::QUAD2);
					}
					else if(str.compare("正方形三角化") == 0)
					{
						def_particle.setShape(crParticle::QUAD_TRIANGLESTRIP);
					}
					else if(str.compare("六边形") == 0)
					{
						def_particle.setShape(crParticle::HEXAGON);
					}
					else if(str.compare("线") == 0)
					{
						def_particle.setShape(crParticle::LINE);
					}
					else if(str.find("模型") == 0)
					{
						def_particle.setShape(crParticle::Object);
					}
					//"半径"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					def_particle.setRadius(atof(str.c_str()));
                    //"密度"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					float density = atof(str.c_str());
					float radius = def_particle.getRadius();
				    def_particle.setMass(density*radius*radius*radius*CRCore::PI*4.0f/3.0f);
					//def_particle.setMass(atof(str.c_str()));
					//"大小范围"
					crVector3 minsize, maxsize;
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,minsize);
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,maxsize);
					def_particle.setSizeRange(rangev3(minsize,maxsize));
					//"透明度范围"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec2);
					def_particle.setAlphaRange(rangef(vec2[0],vec2[1]));
					//"颜色范围1"
					crVector4 mincolor, maxcolor;
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,mincolor);
					mincolor /= 255.0f;
					//"颜色范围2"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,maxcolor);
					maxcolor /= 255.0f;
					def_particle.setColorRange(rangev4(mincolor,maxcolor));
					//"生命周期"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					def_particle.setLifeTime(atof(str.c_str()));
					//"纹理阵列"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec2);
					//"纹理数量"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					def_particle.setTextureTile(vec2[0],vec2[1],atof(str.c_str()));
					//旋转角度
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					crArgumentParser::appAtoVec(str,vec3);
					vec3[0] = CRCore::DegreesToRadians(vec3[0]);
					vec3[1] = CRCore::DegreesToRadians(vec3[1]);
					vec3[2] = CRCore::DegreesToRadians(vec3[2]);
					def_particle.setStaticAngle(vec3);
					//Billboard
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					int mode = getAlignedMode(str);
					def_particle.setAlignedMode((crParticle::AxisAligned)mode);

					//Emitter
					ref_ptr<crEmitter> emitter = pe->getEmitter();
					//"发射器"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					str = getParticleClassName(str);
					if(str.compare(emitter->className()) != 0)
					{
						crEmitter* newemitter = NULL;
						if(str.compare("crModularEmitter") == 0)
						{
							newemitter = new crModularEmitter;
						}
						if(newemitter) pe->setEmitter(newemitter);
					}
					if(dynamic_cast<CRParticle::crModularEmitter *>(emitter.get()))
					{
						crModularEmitter *me = dynamic_cast<CRParticle::crModularEmitter *>(emitter.get());
						//"开启"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setEnabled(atoi(str.c_str()));
						//"无穷"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setEndless(atoi(str.c_str()));
						//"持续时间"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setLifeTime(atof(str.c_str()));
						//"开始时间"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setStartTime(atof(str.c_str()));
						//"重置时间"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setResetTime(atof(str.c_str()));
						//"比率"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						me->setNumParticlesToCreateMovementCompenstationRatio(atof(str.c_str()));
						//"绝对坐标"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						if(atoi(str.c_str()))
							me->setReferenceFrame(crParticleProcessor::ABSOLUTE_RF);
						else
							me->setReferenceFrame(crParticleProcessor::RELATIVE_RF);
						//Counter
						ref_ptr<CRParticle::crCounter> counter = me->getCounter();
						//"计数器"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						str = getParticleClassName(str);
						if(str.compare(counter->className()) != 0)
						{
							CRParticle::crCounter *newcounter = NULL;
							if(str.compare("crRandomRateCounter") == 0)
							{
								newcounter = new crRandomRateCounter;
							}
							else if(str.compare("crConstantRateCounter") == 0)
							{
								newcounter = new crConstantRateCounter;
							}
							else if(str.compare("crConstantCounter") == 0)
							{
								newcounter = new crConstantCounter;
							}
							if(newcounter) me->setCounter(newcounter);
						}
						if(dynamic_cast<CRParticle::crRandomRateCounter *>(counter.get()))
						{
							crRandomRateCounter *rrc = dynamic_cast<CRParticle::crRandomRateCounter *>(counter.get());
							//"数量范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
							rrc->setRateRange(vec2[0],vec2[1]);
						}
						else if(dynamic_cast<CRParticle::crConstantRateCounter *>(counter.get()))
						{
							crConstantRateCounter *crc = dynamic_cast<CRParticle::crConstantRateCounter *>(counter.get());
							//最小数量
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crc->setMinimumNumberOfParticlesToCreate(atoi(str.c_str()));
							//每秒创建数量
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crc->setNumberOfParticlesPerSecondToCreate(atof(str.c_str()));
						}
						else if(dynamic_cast<CRParticle::crConstantCounter *>(counter.get()))
						{
							crConstantCounter *cc = dynamic_cast<CRParticle::crConstantCounter *>(counter.get());
							//"数量范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							cc->setNumberOfParticlesToCreate(atoi(str.c_str()));
						}
						//Placer
						ref_ptr<CRParticle::crPlacer> placer = me->getPlacer();
						//"放置器"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						str = getParticleClassName(str);
						if(str.compare(placer->className()) != 0)
						{
							CRParticle::crPlacer *newplacer = NULL;
							if(str.compare("crSectorPlacer") == 0)
							{
								newplacer = new crSectorPlacer;
							}
							else if(str.compare("crBoxPlacer") == 0)
							{
								newplacer = new crBoxPlacer;
							}
							else if(str.compare("crPointPlacer") == 0)
							{
								newplacer = new crPointPlacer;
							}
							else if(str.compare("crMultiSegmentPlacer") == 0)
							{
								newplacer = new crMultiSegmentPlacer;
							}
							else if(str.compare("crSegmentPlacer") == 0)
							{
								newplacer = new crSegmentPlacer;
							}
							if(newplacer) me->setPlacer(newplacer);
						}
						if(dynamic_cast<CRParticle::crSectorPlacer *>(placer.get()))
						{
							crSectorPlacer *sp = dynamic_cast<CRParticle::crSectorPlacer *>(placer.get());
							//"中心点"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
							sp->setCenter(vec3);
							//"半径"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
							sp->setRadiusRange(vec2[0],vec2[1]);
							//gridElement->addData("Phi范围",crArgumentParser::appVectoa(crVector2(sp->getPhiRange().minimum,sp->getPhiRange().maximum)),"");
						}
						else if(dynamic_cast<CRParticle::crBoxPlacer *>(placer.get()))
						{
							crBoxPlacer *bp = dynamic_cast<CRParticle::crBoxPlacer *>(placer.get());
							//"中心点"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
							bp->setCenter(vec3);
							//"X范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
							bp->setXRange(vec2[0],vec2[1]);
							//"Y范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
							bp->setYRange(vec2[0],vec2[1]);
							//"Z范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
							bp->setZRange(vec2[0],vec2[1]);
						}
						else if(dynamic_cast<CRParticle::crPointPlacer *>(placer.get()))
						{
							crPointPlacer *pp = dynamic_cast<CRParticle::crPointPlacer *>(placer.get());
							//"中心点"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
							pp->setCenter(vec3);
						}
						else if(dynamic_cast<CRParticle::crMultiSegmentPlacer *>(placer.get()))
						{
							crMultiSegmentPlacer *msp = dynamic_cast<CRParticle::crMultiSegmentPlacer *>(placer.get());
							msp->reset();
							//"坐标数量"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							int num = atoi(str.c_str());
							for( int i = 0; i<num; ++i )
							{
								//"坐标"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								msp->addVertex(vec3);
							}
						}
						else if(dynamic_cast<CRParticle::crSegmentPlacer *>(placer.get()))
						{
							crSegmentPlacer *sp = dynamic_cast<CRParticle::crSegmentPlacer *>(placer.get());
							//"坐标1"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
                            sp->setVertexA(vec3);
							//"坐标2"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
							sp->setVertexB(vec3);
						}
						//Shooter
						ref_ptr<CRParticle::crShooter> shooter = me->getShooter();
						//"Shooter"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						str = getParticleClassName(str);
						if(str.compare(shooter->className()) != 0)
						{
							CRParticle::crShooter* newshooter = NULL;
							if(str.compare("crRadialShooter") == 0)
							{
								newshooter = new crRadialShooter;
							}
							if(newshooter) me->setShooter(newshooter);
						}
						if(dynamic_cast<CRParticle::crRadialShooter *>(shooter.get()))
						{
							crRadialShooter *rs = dynamic_cast<CRParticle::crRadialShooter *>(shooter.get());
							//"速度范围"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec2);
                            rs->setInitialSpeedRange(vec2[0],vec2[1]);

							//gridElement->addData("Theta范围",crArgumentParser::appVectoa(crVector2(rs->getThetaRange().minimum,rs->getThetaRange().maximum)),"");
							//gridElement->addData("Phi范围",crArgumentParser::appVectoa(crVector2(rs->getPhiRange().minimum,rs->getPhiRange().maximum)),"");

							crVector3 minvec3,maxvec3;
							//"角速度范围1"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,minvec3);
							//"角速度范围2"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,maxvec3);
							rs->setInitialRotationalSpeedRange(minvec3,maxvec3);
						}
					}
					//Program
					ref_ptr<CRParticle::crProgram> program = pe->getProgram();
					//"Program"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					str = getParticleClassName(str);
					if(str.compare(program->className()) != 0)
					{
                        CRParticle::crProgram* newprogram = NULL;
						if(str.compare("crModularProgram") == 0)
						{
							newprogram = new crModularProgram;
						}
						else if(str.compare("crFluidProgram") == 0)
						{
							newprogram = new crFluidProgram;
						}
						if(newprogram) pe->setProgram(newprogram);
					}
					if(dynamic_cast<CRParticle::crModularProgram *>(program.get()))
					{
						crModularProgram *mp = dynamic_cast<CRParticle::crModularProgram *>(program.get());
					    int num = mp->numOperators();
						//"操作数量"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						int newnum = atoi(str.c_str());
						ref_ptr<crOperator> op;
						for( int i = 0; i<num; ++i )
						{
							op = mp->getOperator(i);
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							str = getParticleClassName(str);
							if(str.compare(op->className()) != 0)
							{
								crOperator* newop = NULL;
								if(str.compare("crAccelOperator") == 0)
								{
									newop = new crAccelOperator;
								}
								else if(str.compare("crAngularAccelOperator") == 0)
								{
									newop = new crAngularAccelOperator;
								}
								else if(str.compare("crFluidFrictionOperator") == 0)
								{
									newop = new crFluidFrictionOperator;
								}
								else if(str.compare("crForceOperator") == 0)
								{
									newop = new crForceOperator;
								}
								else if(str.compare("crAngularDampingOperator") == 0)
								{
									newop = new crAngularDampingOperator;
								}
								else if(str.compare("crDampingOperator") == 0)
								{
									newop = new crDampingOperator;
								}
								else if(str.compare("crExplosionOperator") == 0)
								{
									newop = new crExplosionOperator;
								}
								else if(str.compare("crOrbitOperator") == 0)
								{
									newop = new crOrbitOperator;
								}
								if(newop) mp->replaceOperator(i,newop);
							}

							if(dynamic_cast<CRParticle::crAccelOperator *>(op.get()))
							{
								crAccelOperator *aop = dynamic_cast<CRParticle::crAccelOperator *>(op.get());
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								aop->setAcceleration(vec3);
							}
							else if(dynamic_cast<CRParticle::crAngularAccelOperator *>(op.get()))
							{
								crAngularAccelOperator *aaop = dynamic_cast<CRParticle::crAngularAccelOperator *>(op.get());
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								aaop->setAngularAcceleration(vec3);
							}
							else if(dynamic_cast<CRParticle::crFluidFrictionOperator *>(op.get()))
							{
								crFluidFrictionOperator *ffop = dynamic_cast<CRParticle::crFluidFrictionOperator *>(op.get());
								//"流体密度"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								ffop->setFluidDensity(atof(str.c_str()));
								//"流体粘性"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								ffop->setFluidViscosity(atof(str.c_str()));
								//"忽略粒子半径"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								ffop->setOverrideRadius(atof(str.c_str()));
								//"风力"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								ffop->setWind(vec3);
							}
							else if(dynamic_cast<CRParticle::crForceOperator *>(op.get()))
							{
								//"力"
								crForceOperator *fop = dynamic_cast<CRParticle::crForceOperator *>(op.get());
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								fop->setForce(vec3);
							}
							else if(dynamic_cast<CRParticle::crAngularDampingOperator *>(op.get()))
							{
								crAngularDampingOperator *_op = dynamic_cast<CRParticle::crAngularDampingOperator *>(op.get());
								//"衰减"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								_op->setDamping(vec3);
								//"最小角速度"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setCutoffLow(atof(str.c_str()));
								////"最大角速度"
								//keyValueNode = gridElement->getData(gridIndex++);
								//str = keyValueNode->getValue();
								//_op->setCutoffHigh(atof(str.c_str()));
							}
							else if(dynamic_cast<CRParticle::crDampingOperator *>(op.get()))
							{
								crDampingOperator *_op = dynamic_cast<CRParticle::crDampingOperator *>(op.get());
								//"衰减"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								_op->setDamping(vec3);
								//"最小速度"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setCutoffLow(atof(str.c_str()));
								////"最大速度"
								//keyValueNode = gridElement->getData(gridIndex++);
								//str = keyValueNode->getValue();
								//_op->setCutoffHigh(atof(str.c_str()));
							}
							else if(dynamic_cast<CRParticle::crExplosionOperator *>(op.get()))
							{
								crExplosionOperator *_op = dynamic_cast<CRParticle::crExplosionOperator *>(op.get());
								//"中心点"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								_op->setCenter(vec3);
								//"半径"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setRadius(atof(str.c_str()));
								//"Magnitude"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setMagnitude(atof(str.c_str()));
								//"Epsilon"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setEpsilon(atof(str.c_str()));
								//"Sigma"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setSigma(atof(str.c_str()));
							}
							else if(dynamic_cast<CRParticle::crOrbitOperator *>(op.get()))
							{
								crOrbitOperator *_op = dynamic_cast<CRParticle::crOrbitOperator *>(op.get());
								//"中心点"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								crArgumentParser::appAtoVec(str,vec3);
								_op->setCenter(vec3);
								//"Magnitude"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setMagnitude(atof(str.c_str()));
								//"Epsilon"
								keyValueNode = gridElement->getData(gridIndex++);
								str = keyValueNode->getValue();
								_op->setEpsilon(atof(str.c_str()));
								////"最大半径"
								//keyValueNode = gridElement->getData(gridIndex++);
								//str = keyValueNode->getValue();
								//_op->setMaxRadius(atof(str.c_str()));
							}
						}
                        if(num < newnum)
						{
						    while(num < newnum)
							{
								mp->addOperator(new crAccelOperator);
								num++;
							}
						}
						else if(num > newnum)
						{
							while(num > newnum)
							{
								mp->removeLastOperator();
								num--;
							}
						}
					}
					else if(dynamic_cast<CRParticle::crFluidProgram *>(program.get()))
					{
						crFluidProgram *fp = dynamic_cast<CRParticle::crFluidProgram *>(program.get());
						//"风力"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						crArgumentParser::appAtoVec(str,vec3);
						fp->setWind(vec3);
                        //"流体类型"
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						crFluidProgram::FluidType oldfluid = fp->getFluidType();
						switch (oldfluid)
						{
						case crFluidProgram::Air:
							break;
						case crFluidProgram::Water:
							break;
						case crFluidProgram::UserDefine:
							//"流体密度"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							fp->setFluidDensity(atof(str.c_str()));
							//"流体粘性"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							fp->setFluidViscosity(atof(str.c_str()));
							//"加速度"
							keyValueNode = gridElement->getData(gridIndex++);
							str = keyValueNode->getValue();
							crArgumentParser::appAtoVec(str,vec3);
							fp->setAcceleration(vec3);
							break;
						}
						if(str.compare("空气") == 0)
						{
							fp->setFluidType(crFluidProgram::Air);
						}
						else if(str.compare("水") == 0)
						{
							fp->setFluidType(crFluidProgram::Water);
						}
						else if(str.compare("自定义") == 0)
						{
							fp->setFluidType(crFluidProgram::UserDefine);
						}
					}
					//pe->buildEffect();
					crNode *child = pe->getChild(0);
					if(child)
						crLoadManager::getInstance()->requestRemoveNode(pe,child);
					pe->setDirty(true);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crAddParticleMethod
//
/////////////////////////////////////////
crAddParticleMethod::crAddParticleMethod(){}
crAddParticleMethod::crAddParticleMethod(const crAddParticleMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement),
m_indexElement(handle.m_indexElement)
{
}
void crAddParticleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crAddParticleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crAddParticleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				ref_ptr<crParticleEffect> particleEffect = new crParticleEffect;
				particleEffect->setName("PS");
				ref_ptr<crParticleSystem> ps = new crParticleSystem;
				particleEffect->setParticleSystem(ps.get());
				ref_ptr<crModularEmitter> emitter = new crModularEmitter;
				particleEffect->setEmitter(emitter.get());
				ref_ptr<CRParticle::crProgram> program = new crFluidProgram;
				particleEffect->setProgram(program.get());

				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					index = atoi(indexElement->getStringArrayInString().c_str());
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
					}
					sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren());
					indexElement->setStringArrayByString(buf);
				}
				CRIOManager::crLoadManager::getInstance()->requestAddNode(selectEffectGroup,particleEffect.get(),true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crDelParticleMethod
//
/////////////////////////////////////////
crDelParticleMethod::crDelParticleMethod(){}
crDelParticleMethod::crDelParticleMethod(const crDelParticleMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement),
m_indexElement(handle.m_indexElement)
{
}
void crDelParticleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crDelParticleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crDelParticleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					index = atoi(indexElement->getStringArrayInString().c_str());
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
						sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-1);
						indexElement->setStringArrayByString(buf);
						return;
					}

					crParticleEffect *pe = dynamic_cast<crParticleEffect *>(selectEffectGroup->getChild(index));
					if(pe)
					{
						if(selectEffectGroup->getNumChildren() > 1)
						{
							index = 0;
							sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-2);
							indexElement->setStringArrayByString(buf);
							CRIOManager::crLoadManager::getInstance()->requestRemoveNode(selectEffectGroup,pe);
						}
						else
						{
							CRCore::notify(CRCore::NOTICE)<<"crDelParticleMethod 删除失败，仅剩一个粒子系统，请通过删除节点方式删除"<<std::endl;
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crNextParticleMethod
//
/////////////////////////////////////////
crNextParticleMethod::crNextParticleMethod(){}
crNextParticleMethod::crNextParticleMethod(const crNextParticleMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement),
m_indexElement(handle.m_indexElement)
{
}
void crNextParticleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crNextParticleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crNextParticleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					index = atoi(indexElement->getStringArrayInString().c_str());
					index++;
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
					}
					sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-1);
					indexElement->setStringArrayByString(buf);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crCopyParticleMethod
//
/////////////////////////////////////////
crCopyParticleMethod::crCopyParticleMethod(){}
crCopyParticleMethod::crCopyParticleMethod(const crCopyParticleMethod& handle):
	crMethod(handle),
	m_listElement(handle.m_listElement),
	m_indexElement(handle.m_indexElement)
{
}
void crCopyParticleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crCopyParticleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crCopyParticleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					index = atoi(indexElement->getStringArrayInString().c_str());
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
						sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-1);
						indexElement->setStringArrayByString(buf);
						return;
					}

					crParticleEffect *pe = dynamic_cast<crParticleEffect *>(selectEffectGroup->getChild(index));
					if(pe)
					{//copy pe
						ref_ptr<crParticleEffect> copype = dynamic_cast<crParticleEffect *>(pe->clone(crCopyOp::DEEP_COPY_NODES));
						crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
						crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
						crData *infoData = bodyInfo->getDataClass();
						infoData->inputParam(Edit_CopiedParticle,copype.get());
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crPastParticleMethod
//
/////////////////////////////////////////
crPastParticleMethod::crPastParticleMethod(){}
crPastParticleMethod::crPastParticleMethod(const crPastParticleMethod& handle):
	crMethod(handle),
	m_listElement(handle.m_listElement),
	m_indexElement(handle.m_indexElement)
{
}
void crPastParticleMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crPastParticleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crPastParticleMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					index = atoi(indexElement->getStringArrayInString().c_str());
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
						sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-1);
						indexElement->setStringArrayByString(buf);
						return;
					}

					crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
					crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
					crData *infoData = bodyInfo->getDataClass();
					void *param;
					infoData->getParam(Edit_CopiedParticle,param);
					ref_ptr<crParticleEffect> copype = (crParticleEffect *)param;
					if(copype.valid())
					{
						ref_ptr<crParticleEffect> copype2 = dynamic_cast<crParticleEffect *>(copype->clone(crCopyOp::DEEP_COPY_NODES));
						selectEffectGroup->setChild(index,copype2.get());
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crResetParticleIndexMethod
//
/////////////////////////////////////////
crResetParticleIndexMethod::crResetParticleIndexMethod(){}
crResetParticleIndexMethod::crResetParticleIndexMethod(const crResetParticleIndexMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement),
m_indexElement(handle.m_indexElement)
{
}
void crResetParticleIndexMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crResetParticleIndexMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crResetParticleIndexMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					char buf[16];
					sprintf(buf,"%d/%d\0",0,selectEffectGroup->getNumChildren()-1);
					indexElement->setStringArrayByString(buf);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crElementDoEventMethod
//
/////////////////////////////////////////
crElementDoEventMethod::crElementDoEventMethod():
m_msg(NULL),
m_param(NULL){}
crElementDoEventMethod::crElementDoEventMethod(const crElementDoEventMethod& handle):
crMethod(handle),
m_stageName(handle.m_stageName),
m_elementName(handle.m_elementName),
m_msg(handle.m_msg),
m_param(handle.m_param)
{
}
void crElementDoEventMethod::inputParam(int i, void *param)
{
}
void crElementDoEventMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageName = str;
		break;
	case 1:
		m_elementName = str;
		break;
	case 2:
		m_msg = parseEventMsg(str);
		break;
	case 3:
		m_msg = MAKEINT64(LOINT64(m_msg),parseEventMsg(str));
		break;
	case 4:
		m_param = atoi(str.c_str());
		break;
	case 5:
		m_param =  MAKEINT64(LOINT64(m_param),atoi(str.c_str()));
		break;
	}
}
void crElementDoEventMethod::operator()(crHandle &handle)
{
	crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_stageName);
	if(stage)
	{
		crElement *element = stage->getElement(m_elementName);
		if(element)
		{
			crHandle *handle = element->getHandle(m_msg);
			if(handle)
			{
				handle->inputParam(2,&m_param);
				element->excHandle(handle);
			}
		}
	}
}
/////////////////////////////////////////
//
//crSelectParticleGroupMethod
//
/////////////////////////////////////////
crSelectParticleGroupMethod::crSelectParticleGroupMethod(){}
crSelectParticleGroupMethod::crSelectParticleGroupMethod(const crSelectParticleGroupMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement)
{
}
void crSelectParticleGroupMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crSelectParticleGroupMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	}
}
void crSelectParticleGroupMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
				crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
				bodyInfo->unSelectAll();
				bodyInfo->selectNode(selectEffectGroup);
			}
		}
	}
}
/////////////////////////////////////////
//
//crSelectParticleEffectMethod
//
/////////////////////////////////////////
crSelectParticleEffectMethod::crSelectParticleEffectMethod(){}
crSelectParticleEffectMethod::crSelectParticleEffectMethod(const crSelectParticleEffectMethod& handle):
crMethod(handle),
m_listElement(handle.m_listElement),
m_indexElement(handle.m_indexElement)
{
}
void crSelectParticleEffectMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}
void crSelectParticleEffectMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_listElement = str;
		break;
	case 1:
		m_indexElement = str;
		break;
	}
}
void crSelectParticleEffectMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crListBoxElement *listbox = dynamic_cast<crListBoxElement *>(stage->getElement(m_listElement));
		if(listbox->getDataSize()>0)
		{
			std::string selectName;
			listbox->getData(listbox->getSelect(),selectName);
			crMatrixTransform* selectEffectGroup = dynamic_cast<crMatrixTransform *>(crParticleSystemUpdater::getInstance()->findEffectGroup(selectName));
			if(selectEffectGroup)
			{
				int index = 0;
				crStaticTextBoxElement *indexElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_indexElement));
				if(indexElement)
				{
					index = atoi(indexElement->getStringArrayInString().c_str());
					if(index<0 || index>=selectEffectGroup->getNumChildren())
					{
						index = 0;
						char buf[16];
						sprintf(buf,"%d/%d\0",index,selectEffectGroup->getNumChildren()-1);
						indexElement->setStringArrayByString(buf);
						return;
					}
				}
				crParticleEffect *particleEffect = dynamic_cast<crParticleEffect *>(selectEffectGroup->getChild(index));
				if(particleEffect)
				{
					crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
					crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
					bodyInfo->unSelectAll();
					bodyInfo->selectNode(particleEffect);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisplaySequenceAttrMethod
//
/////////////////////////////////////////
crDisplaySequenceAttrMethod::crDisplaySequenceAttrMethod(){}
crDisplaySequenceAttrMethod::crDisplaySequenceAttrMethod(const crDisplaySequenceAttrMethod& handle):
crMethod(handle),
m_currentSelectElement(handle.m_currentSelectElement),
m_gridElement(handle.m_gridElement)
{
}
void crDisplaySequenceAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplaySequenceAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_gridElement = str;
		break;
	}
}

void crDisplaySequenceAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();

		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
			currentSelectElement->clearString();
		}
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();
			gridElement->checkSelect();
		}

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crNode *selectNode = selectNodeVec[0].get();
			crSeqTextureTileCallback *seqcallback = NULL;
			crSequence *seq = dynamic_cast<crSequence *>(selectNode);
			if(!seq)
			{
				crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
				if(currentSelectObject)
				{
					seqcallback = dynamic_cast<crSeqTextureTileCallback *>(currentSelectObject->getUpdateCallback("SeqTextureTile"));
					if(seqcallback)
					{
						seq = seqcallback->getSequence();
					}
				}
			}
			if(seq)
			{
				std::string str;
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					selectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}
				
				gridElement->addData("节点名",seq->getName(),"");
                if(seqcallback)
				{
					gridElement->addData("纹理阵列",crArgumentParser::appVectoa(crVector2i(seqcallback->getTileS(),seqcallback->getTileT())),"Blue13");
					gridElement->addData("纹理数量",crArgumentParser::appItoa(seqcallback->getNumTiles()),"Blue13");           
				}
				crSequence::LoopMode loopmode;
				int begin;
				int end;
				int frameCount;
				seq->getInterval(loopmode,begin,end,frameCount);
				switch(loopmode)
				{
				case crSequence::LOOP:
					gridElement->addData("循环模式","LOOP","Red13");
					break;
				case crSequence::SWING:
					gridElement->addData("循环模式","SWING","Red13");
					break;
				}
				gridElement->addData("开始",crArgumentParser::appItoa(begin),"Red13");
				gridElement->addData("结束",crArgumentParser::appItoa(end),"Red13");
				gridElement->addData("帧数",crArgumentParser::appItoa(frameCount),"Red13");
				float speed;
				int nreps;
				seq->getDuration(speed,nreps);
				speed = speed == 0.0f?0.0f:1.0f/speed;
				gridElement->addData("速度",crArgumentParser::appFtoa(speed),"Red13");
				gridElement->addData("循环次数",crArgumentParser::appItoa(nreps),"Red13");
				switch(seq->getMode())
				{
				case crSequence::START:
					gridElement->addData("播放模式","START","Red13");
					break;
				case crSequence::STOP:
					gridElement->addData("播放模式","STOP","Red13");
					break;
				case crSequence::PAUSE:
					gridElement->addData("播放模式","PAUSE","Red13");
					break;
				case crSequence::RESUME:
					gridElement->addData("播放模式","RESUME","Red13");
					break;
				}
				gridElement->addData("结束隐藏",seq->getVanishWhenStop()?"1":"0","Red13");
				////////////////////////////
				gridElement->addData("END","NULL","");

				gridElement->checkSelect();
				gridElement->updateInputElement(true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifySequenceAttrMethod
//
/////////////////////////////////////////
crModifySequenceAttrMethod::crModifySequenceAttrMethod(){}
crModifySequenceAttrMethod::crModifySequenceAttrMethod(const crModifySequenceAttrMethod& handle):
crMethod(handle),
m_gridElement(handle.m_gridElement)
{
}
void crModifySequenceAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifySequenceAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crModifySequenceAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crSeqTextureTileCallback *seqcallback = NULL;
				crSequence *seq = dynamic_cast<crSequence *>(selectNodeVec[0].get());
				if(!seq)
				{
					crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
					if(currentSelectObject)
					{
						seqcallback = dynamic_cast<crSeqTextureTileCallback *>(currentSelectObject->getUpdateCallback("SeqTextureTile"));
						if(seqcallback)
						{
							seq = seqcallback->getSequence();
						}
					}
				}
				if(seq)
				{
					int gridIndex = 0;
					crKeyValueNode *keyValueNode;
					std::string str;
					keyValueNode = gridElement->getData(gridIndex++);
					if(!seqcallback)
					{
						str = keyValueNode->getValue();
						seq->setName(str);
					}
					else
					{
						keyValueNode = gridElement->getData(gridIndex++);
                        str = keyValueNode->getValue();
						crVector2i tile;
						crArgumentParser::appAtoVec(str,tile);
						
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						int num = atoi(str.c_str());
						seqcallback->setTextureTile(tile[0],tile[1],num);
					}
					crSequence::LoopMode loopmode = crSequence::LOOP;
					int begin;
					int end;
					int frameCount;
					//"循环模式"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					if(str.compare("LOOP") == 0)
						loopmode = crSequence::LOOP;
					else if(str.compare("SWING") == 0)
						loopmode = crSequence::SWING;
                    //"开始"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					begin = atoi(str.c_str());
					//"结束"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					end = atoi(str.c_str());
					//"帧数"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					frameCount = atoi(str.c_str());
					seq->setInterval(loopmode,begin,end,frameCount);
					float speed;
					int nreps;
                    //"速度"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					speed = atof(str.c_str());
					//"循环次数"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					nreps = atoi(str.c_str());
					seq->setDuration(speed==0.0f?0.0f:1.0f/speed,nreps);
					//"播放模式"
					crSequence::SequenceMode mode;
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					if(str.compare("START") == 0)
						mode = crSequence::START;
					else if(str.compare("STOP") == 0)
						mode = crSequence::STOP;
					else if(str.compare("PAUSE") == 0)
						mode = crSequence::PAUSE;
					else if(str.compare("RESUME") == 0)
						mode = crSequence::RESUME;
					seq->setMode(mode);
					//"结束隐藏"
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					seq->setVanishWhenStop(atoi(str.c_str()));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSelectNodeIsObjectMethod
//
/////////////////////////////////////////
crSelectNodeIsObjectMethod::crSelectNodeIsObjectMethod()
{
}

crSelectNodeIsObjectMethod::crSelectNodeIsObjectMethod(const crSelectNodeIsObjectMethod& handle):
crMethod(handle)
{
}

void crSelectNodeIsObjectMethod::inputParam(int i, void *param)
{
}

void crSelectNodeIsObjectMethod::addParam(int i, const std::string& str)
{
}

void crSelectNodeIsObjectMethod::operator()(crHandle &handle)
{
	bool isobject = false;
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		if(dynamic_cast<crObject *>(selectNodeVec[0].get()))
			isobject = true;
	}
	handle.outputParam(0,&isobject);
}
/////////////////////////////////////////
//
//crSaveSequenceMethod
//
/////////////////////////////////////////
crSaveSequenceMethod::crSaveSequenceMethod(){}
crSaveSequenceMethod::crSaveSequenceMethod(const crSaveSequenceMethod& handle):
crMethod(handle)
{
}
void crSaveSequenceMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveSequenceMethod::addParam(int i, const std::string& str)
{
}

void crSaveSequenceMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crSeqTextureTileCallback *seqcallback = NULL;
			crSequence *seq;
			crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
			if(currentSelectObject)
			{
				seqcallback = dynamic_cast<crSeqTextureTileCallback *>(currentSelectObject->getUpdateCallback("SeqTextureTile"));
				if(seqcallback)
				{
					seq = seqcallback->getSequence();
				}
			}
			if(seq && seqcallback)
			{
				std::string filename = seqcallback->getCfgFile();

				rcfg::ConfigScript cfg_script;
				if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
				{
					CRCore::notify(CRCore::FATAL)<<"crSaveSequenceMethod(): file open error,"<<filename<<std::endl;
					return;
				}

				int nParam;
				std::string str;
				if(cfg_script.Push("NodeCallback"))
				{
					cfg_script.Get("Class", str);
					if(str.compare("SeqTextureTile") == 0)
					{//
						nParam = 1;
						//"纹理阵列"
						str = crArgumentParser::appVectoa(crVector2i(seqcallback->getTileS(),seqcallback->getTileT()));
						cfg_script.Modify("Param",str,nParam++);
						//"纹理数量"
						str = crArgumentParser::appItoa(seqcallback->getNumTiles());
						cfg_script.Modify("Param",str,nParam++);    

						crSequence::LoopMode loopmode;
						int begin;
						int end;
						int frameCount;
						seq->getInterval(loopmode,begin,end,frameCount);
						switch(loopmode)
						{
						case crSequence::LOOP:
							str = "LOOP";
							break;
						case crSequence::SWING:
							str = "SWING";
							break;
						}
						//"循环模式"
						cfg_script.Modify("Param",str,nParam++);
						//"开始"
						str = crArgumentParser::appItoa(begin);
						cfg_script.Modify("Param",str,nParam++);
						//"结束"
						str = crArgumentParser::appItoa(end);
						cfg_script.Modify("Param",str,nParam++);
						//"帧数"
						str = crArgumentParser::appItoa(frameCount);
						cfg_script.Modify("Param",str,nParam++);

						float speed;
						int nreps;
						seq->getDuration(speed,nreps);
						speed = speed == 0.0f?0.0f:1.0f/speed;
						//"速度"
						str = crArgumentParser::appFtoa(speed);
						cfg_script.Modify("Param",str,nParam++);
						//"循环次数"
						str = crArgumentParser::appItoa(nreps);
						cfg_script.Modify("Param",str,nParam++);
						//"播放模式"
						switch(seq->getMode())
						{
						case crSequence::START:
							str = "START";
							break;
						case crSequence::STOP:
							str = "STOP";
							break;
						case crSequence::PAUSE:
							str = "PAUSE";
							break;
						case crSequence::RESUME:
							str = "RESUME";
							break;
						}
						cfg_script.Modify("Param",str,nParam++);
						//"结束隐藏"
						str = seq->getVanishWhenStop()?"1":"0";
						cfg_script.Modify("Param",str,nParam++);

						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crSaveSequenceMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"crSaveSequenceMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
				}
				cfg_script.Write(filename);
				cfg_script.Close();
				CRIOManager::crWriteCookFile scopedWrite(filename);
			}
		}
	}
}
/////////////////////////////////////////
//
//crEditChangeCameraMethod
//
/////////////////////////////////////////
crEditChangeCameraMethod::crEditChangeCameraMethod()
{
}

crEditChangeCameraMethod::crEditChangeCameraMethod(const crEditChangeCameraMethod& handle):
crMethod(handle)
{
}

void crEditChangeCameraMethod::inputParam(int i, void *param)
{
}

void crEditChangeCameraMethod::addParam(int i, const std::string& str)
{
}

void crEditChangeCameraMethod::operator()(crHandle &handle)
{
	crCamera *camera = crCameraManager::getInstance()->getCamera("Camera0");
	if(camera)
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *gamebodyInfo = body->getGameBodyInfo();
		crData *infoData = gamebodyInfo->getDataClass();
		crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crNode *currentSelectNode = selectNodeVec[0].get();
			if(currentSelectNode)
			{
				currentSelectNode->setEditorHidden(true);
				camera->attachNode(currentSelectNode);
				body->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_USER+177,camera));
			}
		}
	}
}
/////////////////////////////////////////
//
//crEditReturnCameraMethod
//
/////////////////////////////////////////
crEditReturnCameraMethod::crEditReturnCameraMethod(){}
crEditReturnCameraMethod::crEditReturnCameraMethod(const crEditReturnCameraMethod& handle):
crMethod(handle)
{
}
void crEditReturnCameraMethod::inputParam(int i, void *param)
{
}

void crEditReturnCameraMethod::addParam(int i, const std::string& str)
{
}

void crEditReturnCameraMethod::operator()(crHandle &handle)
{
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;
	crCamera *camera = bindview->getViewCameraNode();
	crViewMatterObject *player = crSceneManager::getInstance()->getPlayer();
	if(player != camera->getAttachedNode())
	{
		camera->getAttachedNode()->setEditorHidden(false);
		crCamera *mainCamera = crCameraManager::getInstance()->getCamera("MainCamera");
		bindview->setViewCameraNode(mainCamera);

		//crNode *lenui = crFilterRenderManager::getInstance()->getFilterNode("LenUI",OBJECT);
		//if(lenui && !lenui->getVisiable())
		//{
		//	lenui->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFadeInit,MAKERGBA(0,0,0,255)));
		//	short speed = 200;
		//	short timeDelay = 500;
		//	lenui->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKEINT64(WCH_LenFade,MAKEINT32(speed,timeDelay)));
		//}
	}
}
/////////////////////////////////////////
//
//crFreezeModelMethod
//
/////////////////////////////////////////
crFreezeModelMethod::crFreezeModelMethod(){}
crFreezeModelMethod::crFreezeModelMethod(const crFreezeModelMethod& handle):
crMethod(handle)
{
}
void crFreezeModelMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_checkBox = (CRGUI::crCheckBoxElement*)param;
}

void crFreezeModelMethod::addParam(int i, const std::string& str)
{
}

void crFreezeModelMethod::operator()(crHandle &handle)
{
	if(m_checkBox.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		crData *infoData = bodyInfo->getDataClass();
		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			void *param;
			infoData->getParam(EDP_FreezeProxySet,param);
			crEditorInfoData::FreezeProxySet* freezeProxySet = (crEditorInfoData::FreezeProxySet*)param;
			crNode *currentSelectNode;
			for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
				 itr != selectNodeVec.end();
				 ++itr)
			{
				currentSelectNode = itr->get();
				if(currentSelectNode)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					currentSelectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(m_checkBox->getSelect())
						{
							freezeProxySet->insert(proxyNode);
						}
						else
						{
							freezeProxySet->erase(proxyNode);
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crFreezeModel2Method
//
/////////////////////////////////////////
crFreezeModel2Method::crFreezeModel2Method():
m_freeze(false){}
crFreezeModel2Method::crFreezeModel2Method(const crFreezeModel2Method& handle):
	crMethod(handle),
	m_freeze(handle.m_freeze)
{
}
void crFreezeModel2Method::inputParam(int i, void *param)
{
}

void crFreezeModel2Method::addParam(int i, const std::string& str)
{
	if(i==0)
		m_freeze = (bool)(atoi(str.c_str()));
}

void crFreezeModel2Method::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	crData *infoData = bodyInfo->getDataClass();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		void *param;
		infoData->getParam(EDP_FreezeProxySet,param);
		crEditorInfoData::FreezeProxySet* freezeProxySet = (crEditorInfoData::FreezeProxySet*)param;
		crNode *currentSelectNode;
		for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
			itr != selectNodeVec.end();
			++itr)
		{
			currentSelectNode = itr->get();
			if(currentSelectNode)
			{
				CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setNameId("ProxyNode");
				searchByNameVisitor.setSearchNodeType(ALLNODE);
				currentSelectNode->accept(searchByNameVisitor);
				crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(proxyNode)
				{
					if(m_freeze)
					{
						freezeProxySet->insert(proxyNode);
					}
					else
					{
						freezeProxySet->erase(proxyNode);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDisconnectDrawableMethod
//
/////////////////////////////////////////
crDisconnectDrawableMethod::crDisconnectDrawableMethod(){}
crDisconnectDrawableMethod::crDisconnectDrawableMethod(const crDisconnectDrawableMethod& handle):
crMethod(handle)
{
}
void crDisconnectDrawableMethod::inputParam(int i, void *param)
{
}

void crDisconnectDrawableMethod::addParam(int i, const std::string& str)
{
}

void crDisconnectDrawableMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crObject *currentSelectObject = dynamic_cast<crObject *>(selectNodeVec[0].get());
		if(currentSelectObject)
		{
			crData *infoData = bodyInfo->getDataClass();
			assert(infoData);
			void *_selectDrawable;
			CRCore::ref_ptr<crDrawable> selectDrawable = NULL;
			CRCore::ref_ptr<crDrawable> newDrawable;
			infoData->getParam(7,_selectDrawable);
			if(_selectDrawable)
			{
				selectDrawable = (crDrawable*)_selectDrawable;
				if(!dynamic_cast<SubMeshDrawable *>(selectDrawable.get()))
				{
					bodyInfo->unSelectAll();
					newDrawable = dynamic_cast<crDrawable *>(selectDrawable->clone(crCopyOp::DEEP_COPY_DRAWABLES));
					currentSelectObject->releaseObjects(NULL);
					currentSelectObject->replaceDrawable(selectDrawable.get(),newDrawable.get());
					crLoadManager::getInstance()->requestCompile(currentSelectObject);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crRunSingleGameMethod
//
/////////////////////////////////////////
void crRunSingleGameMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crRunSingleGameMethod::addParam(int i, const std::string& str)
{
	//switch(i) 
	//{
	//case 0:
	//	m_loadingDlgID = str;
	//	break;
	//case 1:
	//	m_loadingProgress = str;
	//	break;
	//}
}

void crRunSingleGameMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		//CREncapsulation::crStartHandler::getInstance()->start( CRProducer::crViewer::VIEW_MATRIX_BIND|CRProducer::crViewer::ESCAPE_SETS_DONE,
		//	CRUtil::crSceneView::STANDARD_SETTINGS|CRUtil::crSceneView::USEPHYSICS|CRUtil::crSceneView::USEAI );
		//crImageStage *stage = m_element->getParentStage();
		//CREncapsulation::crStartHandler::getInstance()->start( CRProducer::crViewer::CURSOR_NULL,
		//	CRUtil::crSceneView::STANDARD_SETTINGS|CRUtil::crSceneView::USEPHYSICS|CRUtil::crSceneView::USEAI );
		CREncapsulation::crStartHandler::getInstance()->start(CRProducer::crViewer::CURSOR_NULL,
			CRUtil::crSceneView::STANDARD_SETTINGS | CRUtil::crSceneView::USEPHYSICS | CRUtil::crSceneView::USEAI,
			CRGUI::cr2DStageManager::getInstance()->getParentWnd());
		//CRGUI::crImageStage *imageStage = CRGUI::cr2DStageManager::getInstance()->findStage(m_loadingDlgID);
		//if(imageStage)
		//{//可以做一个延迟几秒关闭的函数
		//	CRGUI::crProgressElement *upd_element = dynamic_cast<CRGUI::crProgressElement *>(imageStage->getElement(m_loadingProgress));
		//	upd_element->fetchEnd();
		//	CRGUI::cr2DStageManager::getInstance()->close(imageStage);
		//}
		crBrain::getInstance()->doEvent(WCH_SceneStart);
		//crFilterRenderManager::getInstance()->closeCanvas("LoadingUI");
		CREncapsulation::crStartHandler::getInstance()->run();
		crShaderManager::getInstance()->clear();
		crLightSourceManager::getInstance()->clear();
		//CRAL::crSoundManager::instance()->loadSound("script/sound0.cfg");
		//crBrain::getInstance()->pushInstance(CRAL::crSoundManager::instance());
		CREncapsulation::crStartHandler::getInstance()->end();
	}
}
/////////////////////////////////////////
//
//crDisplayDuplicateAttrMethod
//
/////////////////////////////////////////
crDisplayDuplicateAttrMethod::crDisplayDuplicateAttrMethod(){}
crDisplayDuplicateAttrMethod::crDisplayDuplicateAttrMethod(const crDisplayDuplicateAttrMethod& handle):
	crMethod(handle),
	m_currentSelectElement(handle.m_currentSelectElement),
	m_gridElement(handle.m_gridElement)
{
}
void crDisplayDuplicateAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayDuplicateAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_currentSelectElement = str;
		break;
	case 1:
		m_gridElement = str;
		break;
	}
}

void crDisplayDuplicateAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();

		crEditBoxElement *currentSelectElement = dynamic_cast<crEditBoxElement *>(m_imageStage->getElement(m_currentSelectElement));
		if(currentSelectElement)
		{
			currentSelectElement->clearString();
		}
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();
			gridElement->checkSelect();
		}

		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crNode *selectNode = selectNodeVec[0].get();
			crDuplicateCallback *dupcallback = NULL;
			if(selectNode)
			{
				dupcallback = dynamic_cast<crDuplicateCallback *>(selectNode->getCullCallback("Duplicate"));
			}
			if(dupcallback)
			{
				std::string str;
				if(currentSelectElement)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					selectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",str))
						{
							currentSelectElement->setStringArrayByString(str);
						}
						else
							currentSelectElement->clearString();
					}
					else
						currentSelectElement->clearString();
				}

				gridElement->addData("节点名",selectNode->getName(),"");
				gridElement->addData("复制数量",crArgumentParser::appItoa(dupcallback->getDuplicateCount()),"Blue13");
				//复制矩阵
				crVector3 pos,dir;
				crDuplicateCallback::MatrixVec &matrixVec = dupcallback->getMatrixVec();
				for( crDuplicateCallback::MatrixVec::iterator itr = matrixVec.begin();
					itr != matrixVec.end();
					++itr )
				{
					crMatrix &mat = *itr;
					pos = mat.getTrans();
					crMatrixf rotation = mat;
					rotation.setTrans(0.0f,0.0f,0.0f);
					dir = (-Y_AXIS * rotation).normalize();
					gridElement->addData("Dir",crArgumentParser::appVectoa(dir),"Red13");
					gridElement->addData("Pos",crArgumentParser::appVectoa(pos),"Blue13");
				}
				////////////////////////////
				gridElement->addData("END","NULL","");

				gridElement->checkSelect();
				gridElement->updateInputElement(true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crModifyDuplicateAttrMethod
//
/////////////////////////////////////////
crModifyDuplicateAttrMethod::crModifyDuplicateAttrMethod(){}
crModifyDuplicateAttrMethod::crModifyDuplicateAttrMethod(const crModifyDuplicateAttrMethod& handle):
	crMethod(handle),
	m_gridElement(handle.m_gridElement)
{
}
void crModifyDuplicateAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyDuplicateAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crModifyDuplicateAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
			crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crNode *selectNode = selectNodeVec[0].get();
				crDuplicateCallback *dupcallback = NULL;
				if(selectNode)
				{
					dupcallback = dynamic_cast<crDuplicateCallback *>(selectNode->getCullCallback("Duplicate"));
				}
				if(dupcallback)
				{
					int gridIndex = 0;
					crKeyValueNode *keyValueNode;
					std::string str;
					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					selectNode->setName(str);

					keyValueNode = gridElement->getData(gridIndex++);
					str = keyValueNode->getValue();
					int count = atoi(str.c_str());
					dupcallback->setDuplicateCount(count);
					crDuplicateCallback::MatrixVec &matrixVec = dupcallback->getMatrixVec();
					crVector3 pos,dir;
					int i = 0;
					if(count<matrixVec.size())
					{
						matrixVec.resize(count);
					}
					for( crDuplicateCallback::MatrixVec::iterator itr = matrixVec.begin();
						itr != matrixVec.end() && i<count;
						++itr,i++ )
					{
						crMatrix &mat = *itr;
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						crArgumentParser::appAtoVec(str,dir);
						keyValueNode = gridElement->getData(gridIndex++);
						str = keyValueNode->getValue();
						crArgumentParser::appAtoVec(str,pos);

						if(dir == crVector3(0.0f,0.0f,0.0f))
						{
							dir[1] = -1.0f;
						}
						if(dir[1] > 0.999)
						{
							mat.makeRotate(-CRCore::Y_AXIS,dir);
							CRCore::crVector3d ep,center,up;
							mat.getLookAt(ep,center,up);
							if(center[2]>0.0f)
							{
								center = -center;
							}
							mat.makeLookAt(ep,center,up);
						}
						else
							mat.makeRotate(-CRCore::Y_AXIS,dir);

						mat.setTrans(pos);
					}
					for(;i<count;i++)
					{
						crMatrix newmat;
						matrixVec.push_back(newmat);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crSaveDuplicateMethod
//
/////////////////////////////////////////
crSaveDuplicateMethod::crSaveDuplicateMethod(){}
crSaveDuplicateMethod::crSaveDuplicateMethod(const crSaveDuplicateMethod& handle):
	crMethod(handle)
{
}
void crSaveDuplicateMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveDuplicateMethod::addParam(int i, const std::string& str)
{
}

void crSaveDuplicateMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
		crGameBodyInfo::SelectNodeVec &selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crDuplicateCallback *duplicateCallback = NULL;
			crNode *currentSelectNode = selectNodeVec[0].get();
			if(currentSelectNode)
			{
				duplicateCallback = dynamic_cast<crDuplicateCallback *>(currentSelectNode->getCullCallback("Duplicate"));
			}
			if(duplicateCallback)
			{
				std::string filename = duplicateCallback->getCfgFile();

				rcfg::ConfigScript cfg_script;
				if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
				{
					CRCore::notify(CRCore::FATAL)<<"crSaveDuplicateMethod(): file open error,"<<filename<<std::endl;
					return;
				}

				int nParam;
				std::string str;
				if(cfg_script.Push("NodeCallback"))
				{
					cfg_script.Get("Class", str);
					if(str.compare("Duplicate") == 0)
					{//
						nParam = 1;
						//复制数量
						str = crArgumentParser::appItoa(duplicateCallback->getDuplicateCount());
						cfg_script.Modify("Param",str,nParam++);
						//复制矩阵
						crVector3 pos,dir;
						int paramid;
						crDuplicateCallback::MatrixVec &matrixVec = duplicateCallback->getMatrixVec();
						for( crDuplicateCallback::MatrixVec::iterator itr = matrixVec.begin();
								itr != matrixVec.end();
								++itr )
						{
							crMatrix &mat = *itr;
							pos = mat.getTrans();
							crMatrixf rotation = mat;
							rotation.setTrans(0.0f,0.0f,0.0f);
							dir = (-Y_AXIS * rotation).normalize();
							str = crArgumentParser::appVectoa(dir);
							paramid = nParam++;
							if(!cfg_script.Modify("Param",str,paramid))
							{
								cfg_script.Add("Param",str);
							}
							str = crArgumentParser::appVectoa(pos);
							paramid = nParam++;
							if(!cfg_script.Modify("Param",str,paramid))
							{
								cfg_script.Add("Param",str);
							}
						}
						if (!cfg_script.Pop())
							CRCore::notify(CRCore::FATAL)<<"crSaveSequenceMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
					}
					if (!cfg_script.Pop())
						CRCore::notify(CRCore::FATAL)<<"crSaveSequenceMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
				}
				cfg_script.Write(filename);
				cfg_script.Close();
				CRIOManager::crWriteCookFile scopedWrite(filename);
			}
		}
	}
}
/////////////////////////////////////////
//
//crUpdateNodeList2Method
//
/////////////////////////////////////////
crUpdateNodeList2Method::crUpdateNodeList2Method(){}
crUpdateNodeList2Method::crUpdateNodeList2Method(const crUpdateNodeList2Method& handle):
	crMethod(handle),
	m_callbackName(handle.m_callbackName)
{
}
void crUpdateNodeList2Method::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crUpdateNodeList2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_callbackName = str;
		break;
	}
}

void crUpdateNodeList2Method::operator()(crHandle &handle)
{
	if(m_listbox.valid())
	{
		crCollectNodeBYCallbackVisitor collectNodeByCallback;
		collectNodeByCallback.setCallback(m_callbackName);
		crBrain::getInstance()->accept(collectNodeByCallback);
		CRCore::NodeArray &nodeArray = collectNodeByCallback.getResult();
		m_listbox->clearData();
		if(!nodeArray.empty())
		{
			//std::set<std::string> nodeNameSet;
			//CRCore::NodeArray::iterator itr = nodeArray.begin();
			//nodeNameSet.insert((*itr)->getName());
			//m_listbox->addData((*itr)->getName());
			//itr++;
			//std::string nodename;
			//int i = 0;
			//for(; itr != nodeArray.end(); ++itr )
			//{
			//	nodename = (*itr)->getName();
			//	if(nodeNameSet.find(nodename) != nodeNameSet.end())
			//	{
			//		nodename += crArgumentParser::appItoa(i++);
			//		(*itr)->setName(nodename);
			//	}
			//	m_listbox->addData(nodename);
			//}
			for( CRCore::NodeArray::iterator itr = nodeArray.begin();
				 itr != nodeArray.end();
				 ++itr )
			{
				m_listbox->addData((*itr)->getName());
			}
		}
		m_listbox->checkSelect();
	}
}
/////////////////////////////////////////
//
//crSelectNode2Method
//
/////////////////////////////////////////
crSelectNode2Method::crSelectNode2Method(){}
crSelectNode2Method::crSelectNode2Method(const crSelectNode2Method& handle):
	crMethod(handle),
	m_callbackName(handle.m_callbackName)
{
}
void crSelectNode2Method::inputParam(int i, void *param)
{
	if(i==0)
		m_listbox = (CRGUI::crListBoxElement*)param;
}

void crSelectNode2Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_callbackName = str;
		break;
	}
}

void crSelectNode2Method::operator()(crHandle &handle)
{
	if(m_listbox.valid() && m_listbox->getDataSize()>0)
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		int selid = m_listbox->getSelect();
		
		crCollectNodeBYCallbackVisitor collectNodeByCallback;
		collectNodeByCallback.setCallback(m_callbackName);
		crBrain::getInstance()->accept(collectNodeByCallback);
		CRCore::NodeArray &nodeArray = collectNodeByCallback.getResult();
		if(selid<nodeArray.size())
		{
			crNode *node = nodeArray[selid].get();
			gamebodyInfo->unSelectAll();
			ref_ptr<crData> data = node->getDataClass();
			if(!data.valid())
			{
				data = crDataManager::getInstance()->getData("Event");
				node->setDataClass(data.get());
			}
			if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
			{
				crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
				driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
				data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
				data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
			}
			gamebodyInfo->selectNode(node);
		}
	}
}
/////////////////////////////////////////
//
//crDisplayInitSettingMethod
//
/////////////////////////////////////////
crDisplayInitSettingMethod::crDisplayInitSettingMethod(){}
crDisplayInitSettingMethod::crDisplayInitSettingMethod(const crDisplayInitSettingMethod& handle):
	crMethod(handle),
	m_gridElement(handle.m_gridElement)
{
}
void crDisplayInitSettingMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayInitSettingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crDisplayInitSettingMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();
			crDisplaySettings *ds = crDisplaySettings::instance();

			gridElement->addData("文件名","init","");
			float fps = ds->getFpsControl();
			if(fps != 0.0f)
				fps = 1000.0f/fps;
			gridElement->addData("帧率控制",crArgumentParser::appItoa(fps),"");
			gridElement->addData("UI设计分辨率",crArgumentParser::appVectoa(crVector2(ds->getUIDesignWidth(),ds->getUIDesignHeight())),"");
			gridElement->addData("UI缩放模式",crArgumentParser::appItoa(ds->getUIScaleMode()),"");
			//OutLineColor
			crVector3 color = ds->getOutLineColor();
			color *= 255.0f;
			gridElement->addData("轮廓线颜色",crArgumentParser::appVectoa(color),"Red13");
			color = ds->getSelectOutLineColor();
			color *= 255.0f;
			gridElement->addData("选择轮廓线颜色",crArgumentParser::appVectoa(color),"Red13");
			gridElement->addData("轮廓线宽",crArgumentParser::appFtoa(ds->getOutLineWidth()),"Red13");
			gridElement->addData("轮廓线显示距离",crArgumentParser::appFtoa(ds->getOutLineDispDistance()),"Red13");
			//CharacterDispDistance
			gridElement->addData("角色显示距离",crArgumentParser::appFtoa(ds->getCharacterDispDistance()),"Blue13");
			gridElement->addData("角色阴影距离",crArgumentParser::appFtoa(ds->getCharacterShadowDistance()),"Blue13");
			gridElement->addData("角色更新因子",crArgumentParser::appFtoa(ds->getCharacterUpdateFactor()),"Blue13");
			gridElement->addData("角色精细距离",crArgumentParser::appFtoa(ds->getCharacterNearDistance()),"Blue13");
			gridElement->addData("角色消失距离",crArgumentParser::appFtoa(ds->getCharacterFarDistance()),"Blue13");
			//
			gridElement->addData("禁用LOD",crArgumentParser::appItoa(ds->getDisableLOD()),"");
			gridElement->addData("END","NULL","");

			gridElement->checkSelect();
			gridElement->updateInputElement(true);
		}
	}
}
/////////////////////////////////////////
//
//crModifyInitSettingMethod
//
/////////////////////////////////////////
crModifyInitSettingMethod::crModifyInitSettingMethod(){}
crModifyInitSettingMethod::crModifyInitSettingMethod(const crModifyInitSettingMethod& handle):
	crMethod(handle),
	m_gridElement(handle.m_gridElement)
{
}
void crModifyInitSettingMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyInitSettingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crModifyInitSettingMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			crDisplaySettings *ds = crDisplaySettings::instance();
			int gridIndex = 1;
			crKeyValueNode *keyValueNode;
			std::string str;
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setFpsControl(atoi(str.c_str()));
			//UI设计分辨率
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			crVector2 vec2;
			crArgumentParser::appAtoVec(str,vec2);
			ds->setUIDesignWidth(vec2[0]);
			ds->setUIDesignHeight(vec2[1]);
			//UI缩放模式
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setUIScaleMode(atoi(str.c_str()));
			//OutLineColor
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			crVector3 color;
			crArgumentParser::appAtoVec(str,color);
			color /= 255.0f;
			ds->setOutLineColor(color);
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			crArgumentParser::appAtoVec(str,color);
			color /= 255.0f;
			ds->setSelectOutLineColor(color);
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setOutLineWidth(atof(str.c_str()));
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setOutLineDispDistance(atof(str.c_str()));
			//CharacterDispDistance
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setCharacterDispDistance(atof(str.c_str()));
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setCharacterShadowDistance(atof(str.c_str()));
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setCharacterUpdateFactor(atof(str.c_str()));
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setCharacterNearDistance(atof(str.c_str()));
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			ds->setCharacterFarDistance(atof(str.c_str()));
		}
	}
}
/////////////////////////////////////////
//
//crSaveInitSettingMethod
//
/////////////////////////////////////////
crSaveInitSettingMethod::crSaveInitSettingMethod(){}
crSaveInitSettingMethod::crSaveInitSettingMethod(const crSaveInitSettingMethod& handle):
	crMethod(handle)
{
}
void crSaveInitSettingMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crSaveInitSettingMethod::addParam(int i, const std::string& str)
{
}

void crSaveInitSettingMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		std::string filename = "script/init.cfg";

		rcfg::ConfigScript cfg_script;
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
		{
			CRCore::notify(CRCore::FATAL)<<"crSaveInitSettingMethod(): file open error,"<<filename<<std::endl;
			return;
		}

		std::string str;
		if(cfg_script.Push("DisplaySetting"))
		{
			crDisplaySettings *ds = crDisplaySettings::instance();
			float fps = ds->getFpsControl();
			if(fps != 0.0f)
				fps = 1000.0f/fps;
			int ifps = (int)fps;
			if(!cfg_script.Modify("FPSControl",fps))
			{
				cfg_script.Add("FPSControl",fps);
			}
			//UI设计分辨率
			if(!cfg_script.Modify("UIWidth",ds->getUIDesignWidth()))
			{
				cfg_script.Add("UIWidth",ds->getUIDesignWidth());
			}
			if(!cfg_script.Modify("UIHeight",ds->getUIDesignHeight()))
			{
				cfg_script.Add("UIHeight",ds->getUIDesignHeight());
			}
			if(!cfg_script.Modify("UIScaleMode",(int)ds->getUIScaleMode()))
			{
				cfg_script.Add("UIScaleMode",(int)ds->getUIScaleMode());
			}
			//OutLineColor
			crVector3 color = ds->getOutLineColor();
			color *= 255.0f;
			std::vector<float> v_i;
			v_i.push_back(color[0]);
			v_i.push_back(color[1]);
			v_i.push_back(color[2]);
			if(!cfg_script.Modify("OutLineColor",v_i))
			{
				cfg_script.Add("OutLineColor",v_i);
			}
			color = ds->getSelectOutLineColor();
			color *= 255.0f;
			v_i.clear();
			v_i.push_back(color[0]);
			v_i.push_back(color[1]);
			v_i.push_back(color[2]);
			if(!cfg_script.Modify("SelectOutLineColor",v_i))
			{
				cfg_script.Add("SelectOutLineColor",v_i);
			}
			float flt1;
			flt1 = ds->getOutLineWidth();
			if(!cfg_script.Modify("OutLineWidth",flt1))
			{
				cfg_script.Add("OutLineWidth",flt1);
			}
			flt1 = ds->getOutLineDispDistance();
			if(!cfg_script.Modify("OutLineDispDistance",flt1))
			{
				cfg_script.Add("OutLineDispDistance",flt1);
			}
			//CharacterDispDistance
			flt1 = ds->getCharacterDispDistance();
			if(!cfg_script.Modify("CharacterDispDistance",flt1))
			{
				cfg_script.Add("CharacterDispDistance",flt1);
			}
			flt1 = ds->getCharacterShadowDistance();
			if(!cfg_script.Modify("CharacterShadowDistance",flt1))
			{
				cfg_script.Add("CharacterShadowDistance",flt1);
			}
			flt1 = ds->getCharacterUpdateFactor();
			if(!cfg_script.Modify("CharacterUpdateFactor",flt1))
			{
				cfg_script.Add("CharacterUpdateFactor",flt1);
			}
			flt1 = ds->getCharacterNearDistance();
			if(!cfg_script.Modify("CharacterNearDistance",flt1))
			{
				cfg_script.Add("CharacterNearDistance",flt1);
			}
			flt1 = ds->getCharacterFarDistance();
			if(!cfg_script.Modify("CharacterFarDistance",flt1))
			{
				cfg_script.Add("CharacterFarDistance",flt1);
			}
			cfg_script.Write(filename);
			cfg_script.Close();
			CRIOManager::crWriteCookFile scopedWrite(filename);
		}
	}
}
/////////////////////////////////////////
//
//crDisplayBrainAttrMethod
//
/////////////////////////////////////////
crDisplayBrainAttrMethod::crDisplayBrainAttrMethod(){}
crDisplayBrainAttrMethod::crDisplayBrainAttrMethod(const crDisplayBrainAttrMethod& handle):
	crMethod(handle),
	m_gridElement(handle.m_gridElement)
{
}
void crDisplayBrainAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_imageStage = (CRGUI::crImageStage*)param;
}

void crDisplayBrainAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crDisplayBrainAttrMethod::operator()(crHandle &handle)
{
	if(m_imageStage.valid())
	{
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(m_imageStage->getElement(m_gridElement));
		if(gridElement)
		{
			gridElement->clearData();
			crBrain *brain = crBrain::getInstance();

			gridElement->addData("名称","场景设置","");
			const crVector3i& worldSize = brain->getWorldSize();
			gridElement->addData("场景尺寸",crArgumentParser::appVectoa(worldSize),"Red13");
			const crVector2i& mapSize = brain->getMapSize();
			gridElement->addData("地图分辨率",crArgumentParser::appVectoa(mapSize),"Red13");
			gridElement->addData("END","NULL","");

			gridElement->checkSelect();
			gridElement->updateInputElement(true);
		}
	}
}
/////////////////////////////////////////
//
//crModifyBrainAttrMethod
//
/////////////////////////////////////////
crModifyBrainAttrMethod::crModifyBrainAttrMethod(){}
crModifyBrainAttrMethod::crModifyBrainAttrMethod(const crModifyBrainAttrMethod& handle):
	crMethod(handle),
	m_gridElement(handle.m_gridElement)
{
}
void crModifyBrainAttrMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crModifyBrainAttrMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gridElement = str;
		break;
	}
}

void crModifyBrainAttrMethod::operator()(crHandle &handle)
{
	if(m_element.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		crKeyValueGridElement *gridElement = dynamic_cast<crKeyValueGridElement *>(stage->getElement(m_gridElement));
		if(gridElement)
		{
			crBrain *brain = crBrain::getInstance();
			int gridIndex = 1;
			crKeyValueNode *keyValueNode;
			std::string str;
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			crVector3i worldSize;
			crArgumentParser::appAtoVec(str,worldSize);
			brain->setWorldSize(worldSize);
			keyValueNode = gridElement->getData(gridIndex++);
			str = keyValueNode->getValue();
			crVector2i mapSize;
			crArgumentParser::appAtoVec(str,mapSize);
			brain->setMapSize(mapSize);
		}
	}
}
/////////////////////////////////////////
//
//crGetModKeyMaskMethod
//
/////////////////////////////////////////
crGetModKeyMaskMethod::crGetModKeyMaskMethod(){}
crGetModKeyMaskMethod::crGetModKeyMaskMethod(const crGetModKeyMaskMethod& handle):
	crMethod(handle)
{
}
void crGetModKeyMaskMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	case 2:
		if(param)
		{
			m_param = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(m_param));
		}
		else
		{
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	}
}

void crGetModKeyMaskMethod::addParam(int i, const std::string& str)
{
}

void crGetModKeyMaskMethod::operator()(crHandle &handle)
{
	int _case = 0;
	if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL)
	{
		_case = 1;
	}
	else if(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_ALT)
	{
		_case = 2;
	}
	handle.outputParam(0,&_case);
}
/////////////////////////////////////////
//
//crSceneModifiedMethod
//
/////////////////////////////////////////
crSceneModifiedMethod::crSceneModifiedMethod():
m_modify(false){}
crSceneModifiedMethod::crSceneModifiedMethod(const crSceneModifiedMethod& handle):
	crMethod(handle),
	m_modify(handle.m_modify)
{
}
void crSceneModifiedMethod::inputParam(int i, void *param)
{
}

void crSceneModifiedMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_modify = (bool)(atoi(str.c_str()));
		break;
	}
}

void crSceneModifiedMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	crData *infoData = bodyInfo->getDataClass();
	infoData->inputParam(EDP_SceneModified,&m_modify);
}
/////////////////////////////////////////
//
//crNodeUpdateMethod
//
/////////////////////////////////////////
crNodeUpdateMethod::crNodeUpdateMethod(){}
crNodeUpdateMethod::crNodeUpdateMethod(const crNodeUpdateMethod& handle):
	crMethod(handle)
{
}
void crNodeUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_updateVisitor = NULL;
		}
		break;
	case 1:
		m_this = (crNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_updateVisitor = (CRUtil::crUpdateVisitor *)(LOINT64(param64));
		}
		else
		{
			m_updateVisitor = NULL;
		}
		break;
	}
}

void crNodeUpdateMethod::addParam(int i, const std::string& str)
{
}

void crNodeUpdateMethod::operator()(crHandle &handle)
{
	if(m_this && m_updateVisitor.valid())
	{
		float dt = crFrameStamp::getInstance()->getFrameInterval();
		m_this->doEvent(WCH_UPDATE,MAKEINT64(&dt,m_updateVisitor.get()));
	}
}
/////////////////////////////////////////
//
//crAutoSaveSceneMethod
//
/////////////////////////////////////////
crAutoSaveSceneMethod::crAutoSaveSceneMethod(){}
crAutoSaveSceneMethod::crAutoSaveSceneMethod(const crAutoSaveSceneMethod& handle):
	crMethod(handle),
	m_fileName(handle.m_fileName)
{
}
void crAutoSaveSceneMethod::inputParam(int i, void *param)
{
}
void crAutoSaveSceneMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_fileName = str;
		break;
	}
}

void crAutoSaveSceneMethod::operator()(crHandle &handle)
{
	crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
	crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
	crData *infoData = bodyInfo->getDataClass();
	void *param;
	infoData->getParam(EDP_SceneModified,param);
	bool modified = *(bool *)param;
	if(modified)
	{
		infoData->inputParam(EDP_SceneModified,NULL);
		crScene *scene = crSceneManager::getInstance()->getCurrentScene();
		CRIOManager::writeObjectFile((*crBrain::getInstance()),m_fileName);
		CRText::crText *noticeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("Notify"));
		if(noticeText)
		{
			noticeText->setText(L"自动保存");
			noticeText->setVisiable(true);
		}
	}
}
/////////////////////////////////////////
//
//crUnSelectAllMethod
//
/////////////////////////////////////////
crUnSelectAllMethod::crUnSelectAllMethod(){}
crUnSelectAllMethod::crUnSelectAllMethod(const crUnSelectAllMethod& handle):
	crMethod(handle)
{
}
void crUnSelectAllMethod::inputParam(int i, void *param)
{
}

void crUnSelectAllMethod::addParam(int i, const std::string& str)
{
}

void crUnSelectAllMethod::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	if(cameraBot)
	{
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		gamebodyInfo->unSelectAll();
	}
}
/////////////////////////////////////////
//
//crExportCharacterPosMethod
//
/////////////////////////////////////////
crExportCharacterPosMethod::crExportCharacterPosMethod(){}
crExportCharacterPosMethod::crExportCharacterPosMethod(const crExportCharacterPosMethod& handle) :
crMethod(handle)
{
}
void crExportCharacterPosMethod::inputParam(int i, void *param)
{
}

void crExportCharacterPosMethod::addParam(int i, const std::string& str)
{
}

void crExportCharacterPosMethod::operator()(crHandle &handle)
{
	crCollectNodeBYClassNameVisitor collectNodeByClassName;
	collectNodeByClassName.insertClassNameId("CreBodyNode");
	crBrain::getInstance()->accept(collectNodeByClassName);
	CRCore::NodeArray &nodeArray = collectNodeByClassName.getResult();
	if (!nodeArray.empty())
	{
		CREncapsulation::crTableIO::StrVec title;
		title.push_back("name");
		title.push_back("x");
		title.push_back("y");
		title.push_back("z");
		title.push_back("dirx");
		title.push_back("diry");
		title.push_back("dirz");
		CREncapsulation::crTableIO::StrVec record;
		record.resize(7);
		ref_ptr<CREncapsulation::crTableIO> tab = new CREncapsulation::crTableIO;
		tab->setTitleVec(title);
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setSearchNodeType(ALLNODE);
		std::string datafile;
		for (CRCore::NodeArray::iterator itr = nodeArray.begin();
			itr != nodeArray.end();
			++itr)
		{
			searchByNameVisitor.clearResult();
			(*itr)->accept(searchByNameVisitor);
			crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if (proxyNode)
			{
				crMatrixTransform *matrix = dynamic_cast<crMatrixTransform *>(proxyNode->getChild(0));
				if (matrix)
				{
					crArgumentParser::readKeyValue(proxyNode->getDescriptions(), "DataFile", datafile);
					record[0] = crArgumentParser::getSimpleFileName(datafile);
					crVector3 pos = matrix->getTrans();
					pos *= 100.0f;
					crVector3i ipos(pos[0], pos[1], pos[2]);
					crMatrixf rotation = matrix->getMatrix();
					rotation.setTrans(0.0f, 0.0f, 0.0f);
					crVector3 dir = (-Y_AXIS * rotation).normalize();
					record[1] = crArgumentParser::appItoa(ipos[0]);
					record[2] = crArgumentParser::appItoa(ipos[1]);
					record[3] = crArgumentParser::appItoa(ipos[2]);
					record[4] = crArgumentParser::appItoa(dir[0]);
					record[5] = crArgumentParser::appItoa(dir[1]);
					record[6] = crArgumentParser::appItoa(dir[2]);
					tab->addData(record);
				}
			}
		}
		tab->saveToFileNoCook("CharacterPos.tab");
	}
}