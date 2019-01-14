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
#include <CRPhysics/crCamera.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crMath.h>
using namespace CRPhysics;
using namespace CRCore;
ref_ptr<crCameraManager> crCameraManager::m_instance = NULL;
crCamera::crCamera():
m_viewPitch(0.0f),
m_viewRoll(0.0f),
m_viewOffset(1.0f),
m_viewHeight(0.0f),
//m_viewVelocity(0.0f),
//m_currentViewOffset(0.0f),
m_keyboardMouseMode(KM_None)
{
	//m_viewOffsetRange.set(0.45,20);
}

crCamera::crCamera(const crCamera &camera,const CRCore::crCopyOp& copyop):
	crBase(camera,copyop),
	m_viewPitch(camera.m_viewPitch),
	m_viewRoll(camera.m_viewRoll),
	m_viewOffset(camera.m_viewOffset),
	m_viewHeight(camera.m_viewHeight),
	//m_viewOffsetRange(camera.m_viewOffsetRange),
	//m_viewVelocity(0.0f),
	//m_currentViewOffset(0.0f),
	m_keyboardMouseMode(camera.m_keyboardMouseMode)
{
}

void crCamera::setKeyboardMouseMode(unsigned short mode)
{
	m_keyboardMouseMode = (KeyboardMouseMode)mode;
}
unsigned short crCamera::getKeyboardMouseMode()
{
	return m_keyboardMouseMode;
}

void crCamera::attachNode(crNode *node)
{
    m_attachedNode = node;
	//if(dynamic_cast<crViewMatterObject *>(m_attachedNode.get()))
	//{
	//	dynamic_cast<crViewMatterObject *>(m_attachedNode.get())->setCameraBind(true);
	//}
}

//CRCore::crMatrix crCamera::getCameraMatrix()
//{
//	if(m_attachedNode.valid())
//	{
//		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(m_attachedNode.get());
//		if(vo)
//		{
//			vo->swapToFutureMatrix();
//			vo->cameraSwaped();
//			return vo->getCameraMatrix();
//		}
//	}
//
//	if(m_viewVelocity!=0.0f)
//	{
//		m_currentViewOffset += m_viewVelocity * crCameraManager::getInstance()->getFrameInterval();
//		//m_currentViewOffset = CRCore::clampTo(m_currentViewOffset,0.0f,m_viewOffset);
//		m_currentViewOffset = CRCore::maximum(m_currentViewOffset,0.0f);
//	}
//
//	float delta = m_currentViewOffset-m_viewOffset;
//	if(delta>0.5f)
//	{
//		m_viewVelocity = -m_viewOffsetVelocity;
//	}
//	else if(delta>-0.5f)
//	{
//		m_currentViewOffset = m_viewOffset;
//		m_viewVelocity = 0.0f;
//	}
//	else
//	{
//		m_viewVelocity = m_viewOffsetVelocity;
//	}
//
//	crMatrix cameraMatrix;
//	if(m_viewRoll != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewRoll, 0.0, 0.0, 1.0));//heading
//	if(m_viewPitch != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewPitch, 1.0, 0.0, 0.0));//pitch
//    cameraMatrix.setTrans(0,0,0);
//	crVector3 dir,up,pos;
//	dir = (Y_AXIS*cameraMatrix).normalize();
//	up = (Z_AXIS*cameraMatrix).normalize();
//	crVector3f center = m_attachedNode->getBound().center();
//	float height = m_attachedNode->getBoundBox().zLength() * 0.5f;
//    center[2] += m_viewHeight - height;
//	pos = center + dir * m_currentViewOffset;
//
//	if(m_currentViewOffset>m_attachedNode->getBoundBox().yLength() * 0.5f)
//	{
//		if(!m_lineSegment.valid())
//			m_lineSegment = new crLineSegment;
//		m_lineSegment->set(center,pos);
//		m_iv.reset();
//		m_iv.addLineSegment(m_lineSegment.get());
//		m_iv.insertAvoidIntersectNode(const_cast<crGroup *>(m_attachedNode->getParent(0)));
//		m_iv.insertAvoidIntersectNodeType("crViewMatterObject");
//		m_iv.insertAvoidIntersectNodeType("crCharacterMatterObject");
//		crBrain::getInstance()->accept(m_iv);
//		if (m_iv.hits())
//		{
//			CRCore::crIntersectVisitor::HitList& hitList = m_iv.getHitList(m_lineSegment.get());
//			if (!hitList.empty())
//			{
//				crVector3f lv = center - hitList.front().getWorldIntersectPoint();
//				m_currentViewOffset = lv.length();
//				m_viewVelocity = 0.0f;
//			}
//		}
//		else
//		{
//			if(m_currentViewOffset<m_viewOffset)
//				m_viewVelocity = m_viewOffsetVelocity;
//			else if(m_currentViewOffset>m_viewOffset)
//				m_viewVelocity = -m_viewOffsetVelocity;
//		}
//	}
//	m_currentViewOffset = clampTo(m_currentViewOffset,m_viewOffsetRange[0],m_viewOffsetRange[1]);
//	pos = center + dir * m_currentViewOffset;
//	return crMatrix::lookAt(pos,center,up);
//
//}

CRCore::crMatrix crCamera::getCameraMatrix()
{
	if(!m_attachedNode.valid()) return crMatrix::identity();
	if(dynamic_cast<crViewMatterObject *>(m_attachedNode.get()))
	{
		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(m_attachedNode.get());
		vo->swapToFutureMatrix();
		vo->cameraSwaped();
		return vo->getCameraMatrix();
	}

	//if(m_viewVelocity!=0.0f)
	//{
	//	m_currentViewOffset += m_viewVelocity * crBrain::getInstance()->getFrameInterval();
	//	m_currentViewOffset = CRCore::maximum(m_currentViewOffset,0.0f);
	//}

	//float delta = m_currentViewOffset-m_viewOffset;
	//if(delta>0.5f)
	//{
	//	m_viewVelocity = -m_viewOffsetVelocity;
	//}
	//else if(delta>-0.5f)
	//{
	//	m_currentViewOffset = m_viewOffset;
	//	m_viewVelocity = 0.0f;
	//}
	//else
	//{
	//	m_viewVelocity = m_viewOffsetVelocity;
	//}

	crVector3 dir,up,pos,center;
	crMatrix cameraMatrix;
	if(m_viewRoll != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewRoll, 0.0, 0.0, 1.0));//heading
	if(m_viewPitch != 0.0f) cameraMatrix.preMult(CRCore::crMatrix::rotate(m_viewPitch, 1.0, 0.0, 0.0));//pitch
	cameraMatrix.setTrans(0,0,0);
	
	if(dynamic_cast<crTransform *>(m_attachedNode.get()))
	{
		crTransform *transform = dynamic_cast<crTransform *>(m_attachedNode.get());
		center = m_attachedNode->getBound().center();
		center[2] += m_viewHeight;
		transform->computeLocalToWorldMatrix(cameraMatrix,NULL);
	}
	else
	{
		center = m_attachedNode->getBound().center();
		center[2] += m_viewHeight;
	}
	cameraMatrix.setTrans(0,0,0);
	up = (Z_AXIS*cameraMatrix).normalize();
	dir = (Y_AXIS*cameraMatrix).normalize();
	//crVector3f center = pos + dir * m_viewOffset;
	pos = center - dir * m_viewOffset;
	return crMatrix::lookAt(pos,center,up);
}
CRCore::crVector3 crCamera::getCameraDir()
{
	if(dynamic_cast<crViewMatterObject *>(m_attachedNode.get()))
	{
		crViewMatterObject *vo = dynamic_cast<crViewMatterObject *>(m_attachedNode.get());
		return vo->getDirection();
	}
	crMatrix cameraMatrix;
	if(dynamic_cast<crTransform *>(m_attachedNode.get()))
	{
		crTransform *transform = dynamic_cast<crTransform *>(m_attachedNode.get());
		transform->computeLocalToWorldMatrix(cameraMatrix,NULL);
		cameraMatrix.setTrans(0.0f,0.0f,0.0f);
	}
	return (Y_AXIS * cameraMatrix).normalize();
}
void crCamera::setViewPitch( float viewPitch )
{
	m_viewPitch = viewPitch;
	//CRCore::notify(CRCore::FATAL)<<"crMatterObject::updateViewPitch: m_viewPitch = "<<m_viewPitch<<" delta ="<<delta<<std::endl;
}

void crCamera::setViewRoll( float viewRoll )
{
	m_viewRoll = viewRoll;
	//CRCore::notify(CRCore::FATAL)<<"crMatterObject::updateViewRoll: m_viewRoll = "<<m_viewRoll<<std::endl;
}

void crCamera::setViewOffset(float viewOffset)
{
    m_viewOffset = viewOffset;
}

void crCamera::setViewHeight(float viewHeight)
{
    m_viewHeight = viewHeight;
}

////////////////////////////////////
//
//crCameraManager
//
/////////////////////////////////////
crCameraManager::crCameraManager()
{
}

crCameraManager* crCameraManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crCameraManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crCameraManager::clear()
{
	m_instance = NULL;
}

void crCameraManager::addCamera(crCamera *camera)
{
	m_cameraMap[camera->getName()] = camera;
	if(camera->getName().compare("MainCamera") == 0)
		m_mainCamera = camera;
}

crCamera *crCameraManager::getCamera(const std::string &name)
{
	CameraMap::iterator itr = m_cameraMap.find(name);
	return itr != m_cameraMap.end() ? itr->second.get() : NULL;
}

void crCameraManager::removeCamera(crCamera *camera)
{
	m_cameraMap.erase(camera->getName());
}
void crCameraManager::setCurrentCamera(crCamera *cam)
{
	m_currentCamera = cam;
}
crCamera *crCameraManager::getCurrentCamera()
{
	return m_currentCamera.get();
}
crCamera *crCameraManager::getMainCamera()
{
	return m_mainCamera.get();
}
