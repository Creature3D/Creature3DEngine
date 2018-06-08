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
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crDepth.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/crFrameBufferObject.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;
ref_ptr<crFilterRenderManager> crFilterRenderManager::m_instance = NULL;

crFilterRenderManager::crFilterRenderManager()
  //m_lastMouse(-1),
  //m_lastMousePicMsg(-1),
  //m_dequeTextInterval(1.0f),
  //m_lastFrameTime(0.0)
{
	m_inited = false;
	m_faildShowType = SH_Hide;
	m_faildShowCanvas.clear();
	setCullingActive(false);
	setEnableIntersect(false);
	setNumChildrenRequiringUpdateTraversal(1);
	
	m_cameraStateSet = new crStateSet;
	//crDepth *depth = new CRCore::crDepth(crDepth::ALWAYS);
	//m_cameraStateSet->setAttribute(depth,crStateAttribute::ON);
	m_cameraStateSet->setMode(GL_DEPTH_TEST,CRCore::crStateAttribute::OFF);
	//crAlphaFunc *alphaFunc = new crAlphaFunc;
	//alphaFunc->setFunction(crAlphaFunc::GREATER,0.02f);
	//m_cameraStateSet->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
	crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE_MINUS_SRC_ALPHA);
	m_cameraStateSet->setAttributeAndModes(blendFunc,crStateAttribute::ON);
	////m_cameraStateSet->setMode(GL_MULTISAMPLE_ARB,crStateAttribute::OFF);
	m_cameraStateSet->setRenderingHint(crStateSet::UI_BIN);

	////m_cameraStateSet->setMode(GL_BLEND,crStateAttribute::ON);
	////m_cameraStateSet->setMode(GL_LIGHTING,crStateAttribute::OFF);

	//m_mainCanvas = new crCanvasNode;
	//m_mainCanvas->setName("MainUI");

	//computeMVPW();
	////createOrthoCamera();
	//createIdenticCamera();
}
void crFilterRenderManager::computeMVPW()
{
	int viewWidth = crDisplaySettings::instance()->getViewWidth();
	int viewHeight = crDisplaySettings::instance()->getViewHeight();
	m_mvpw = CRCore::crMatrix::translate(1.0f,1.0f,1.0f)*CRCore::crMatrix::scale(0.5f*viewWidth,0.5f*viewHeight,0.5f);
	m_inverseMVPW.invert(m_mvpw);
	crVector3 pos = m_inverseMVPW.getTrans();
	pos[2] = 0.0f;
	m_inverseMVPW.setTrans(pos);
	crDisplaySettings::instance()->setupUIScaleMatrix();
}
crVector3 crFilterRenderManager::getIdenticPosition(float x, float y)
{
	return getIdenticPosition(crVector3(x,y,0.0f));
}
crVector3 crFilterRenderManager::getIdenticPosition(const crVector3 &windowPos)
{
	return windowPos * m_inverseMVPW;
}
crVector3 crFilterRenderManager::getWindowPosition(const crVector3 &identicPos)
{
	return identicPos * m_mvpw;
}

crFilterRenderManager::crFilterRenderManager(const crFilterRenderManager& node,const crCopyOp& copyop):
crNode(node,copyop)
{
}

crFilterRenderManager* crFilterRenderManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crFilterRenderManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crFilterRenderManager::init()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	m_inited = false;
	m_faildShowType = SH_Hide;
	m_faildShowCanvas.clear();
   // m_instance = NULL;
	m_modalCanvas = NULL;
	m_mouseCanvas = NULL;
	m_loadingCanvas = NULL;
	m_currentMouseCapturer = NULL;
	m_currentInputCapturer = NULL;
	m_focusNode = NULL;
	m_activeCanvasMutex.acquire();
	m_activeCanvasDeque.clear();
	m_activeCanvasTemp.clear();
	m_activeCanvasMutex.release();
	m_asyncDrawableListMutex.acquire();
	m_asyncAddDrawableList.clear();
	m_asyncRemoveDrawableList.clear();
	m_asyncDrawableListMutex.release();

	setRenderInited(false);
	setNumChildrenRequiringUpdateTraversal(1);
	m_mainCanvas = new crCanvasNode;
	m_mainCanvas->setName("MainUI");
	computeMVPW();
	//createOrthoCamera();
	createIdenticCamera();
	//if(m_loadingCanvas.valid())
	//	m_identicCamera->addChild(m_loadingCanvas.get());
	m_inited = true;
}

//void crFilterRenderManager::createOrthoCamera()
//{	
//	m_orthoCamera = new crCameraNode;
//	m_orthoCamera->setName("OrthoUI");
//
//	m_orthoCamera->setEnableIntersect(false);
//	m_orthoCamera->setRenderMode(crCameraNode::NormalRender);
//
//	//m_orthoCamera->setClearColor(crVector4(1.0,1.0,1.0,1.0));
//	m_orthoCamera->setClearMask(GL_NONE);
//	m_orthoCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);
//
//	m_orthoCamera->setRenderOrder(crCameraNode::POST_RENDER);
//	m_orthoCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);
//
//	m_orthoCamera->setDrawBuffer(GL_BACK);
//	m_orthoCamera->setReadBuffer(GL_BACK);
//	m_orthoCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);
//
//	m_orthoCamera->setCullingMode( crCameraNode::NO_CULLING );
//
//	float width = crDisplaySettings::instance()->getViewWidth();
//	float height = crDisplaySettings::instance()->getViewHeight();
//	m_orthoCamera->setViewport(0,0,width,height);
//
//	m_orthoCamera->setProjectionMatrix(CRCore::crMatrix::ortho2D(0,width,0,height));
//	m_orthoCamera->setBufProjectionMatrix(CRCore::crMatrix::ortho2D(0,width,0,height));
//	m_orthoCamera->setViewMatrix(crMatrix::identity());
//	m_orthoCamera->setBufViewMatrix(crMatrix::identity());
//
//	m_orthoObject = new crObject;
//	m_orthoObject->setCullingActive(false);
//    m_orthoCamera->setCullingActive(false);
//	m_orthoCamera->addChild(m_orthoObject.get());
//	m_orthoCamera->setStateSet(m_cameraStateSet.get());
//}
//
void crFilterRenderManager::createIdenticCamera()
{
	m_identicCamera = new crCameraNode;
	m_identicCamera->setName("IdenticUI");

	m_identicCamera->setEnableIntersect(false);
	m_identicCamera->setRenderMode(crCameraNode::NormalRender);

	//m_orthoCamera->setClearColor(crVector4(1.0,1.0,1.0,1.0));
	m_identicCamera->setClearMask(GL_NONE);
	m_identicCamera->setComputeNearFarMode(crCameraNode::DO_NOT_COMPUTE_NEAR_FAR);

	m_identicCamera->setRenderOrder(crCameraNode::POST_RENDER);
	m_identicCamera->setRenderTargetImplementation(crCameraNode::FRAME_BUFFER);

	m_identicCamera->setDrawBuffer(GL_BACK);
	m_identicCamera->setReadBuffer(GL_BACK);
	m_identicCamera->setReferenceFrame(crCameraNode::RELATIVE_TO_ABSOLUTE);

	m_identicCamera->setCullingMode( crCameraNode::VIEW_FRUSTUM_CULLING );

	float width = crDisplaySettings::instance()->getViewWidth();
	float height = crDisplaySettings::instance()->getViewHeight();
	m_identicCamera->setViewport(0,0,width,height);

	//float aspectRatio = width / height;
	
	m_identicCamera->setProjectionMatrix(CRCore::crMatrix::ortho2D(-1,1,-1,1));
	m_identicCamera->setBufProjectionMatrix(CRCore::crMatrix::ortho2D(-1,1,-1,1));
	m_identicCamera->setViewMatrix(crMatrix::identity());
	m_identicCamera->setBufViewMatrix(crMatrix::identity());

	m_identicCamera->setStateSet(m_cameraStateSet.get());

	m_orthoObject = new crObject;
	m_mvpwNode = new crMatrixTransform;
	m_mvpwNode->setMatrix(m_inverseMVPW);
	m_mvpwNode->addChild(m_orthoObject.get());
	m_mainCanvas->addChild(m_mvpwNode.get());
	m_identicCamera->addChild(m_mainCanvas.get());
}
crGroup *crFilterRenderManager::getCanvasRoot()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	return m_identicCamera.get();
}
bool crFilterRenderManager::addDrawable(crDrawable *drawable, bool async)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return false;
	if(async)
	{
		m_asyncDrawableListMutex.acquire();
		m_asyncAddDrawableList.push_back(drawable);
		m_asyncDrawableListMutex.release();
		return true;
	}
	else
	{
		return m_orthoObject->addDrawable(drawable);
	}
}
bool crFilterRenderManager::removeDrawable( crDrawable *drawable, bool async )
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return false;
	if(async)
	{
		m_asyncDrawableListMutex.acquire();
		m_asyncRemoveDrawableList.push_back(drawable);
		m_asyncDrawableListMutex.release();
		return true;
	}
	else
	{
		return m_orthoObject->removeDrawable(drawable);
	}
}
crDrawable *crFilterRenderManager::getDrawable(const std::string &name)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	for( crObject::DrawableList::iterator itr = m_orthoObject->getDrawableList().begin();
		 itr != m_orthoObject->getDrawableList().end();
		 ++itr )
	{
		if(name.compare((*itr)->getName()) == 0)
			return itr->get();
	}
	return NULL;
}
//void crFilterRenderManager::addMouse(crDrawable *drawable)
//{
//	if(addDrawable(drawable))
//	{
//	    m_mouse.push_back(drawable);
//		if(drawable->getVisiable()) showMouse(m_mouse.size()-1);
//	}
//}
//
//void crFilterRenderManager::showMouse(int i)
//{
//    if(i<0 && m_lastMouse >= 0 && m_lastMouse<m_mouse.size()) 
//	{
//		m_mouse[m_lastMouse]->setVisiable(false);
//	}
//	else if(i < m_mouse.size() && i != m_lastMouse)
//	{
//		if(m_lastMouse>=0 && m_lastMouse<m_mouse.size()) m_mouse[m_lastMouse]->setVisiable(false);
//		m_mouse[i]->setVisiable(true);
//	}
//
//	m_lastMouse = i;
//}
//
//void crFilterRenderManager::addMousePicMsg(crDrawable *drawable)
//{
//	if(addDrawable(drawable))
//		m_mousePicMsg.push_back(drawable);
//}
//
//void crFilterRenderManager::showMousePicMsg(int i)
//{
//	if(i<0 && m_lastMousePicMsg >= 0) 
//	{
//		m_mousePicMsg[m_lastMousePicMsg]->setVisiable(false);
//		m_lastMousePicMsg = i;
//	}
//	else if(i < m_mousePicMsg.size() && i != m_lastMousePicMsg)
//	{
//		if(m_lastMousePicMsg>=0)
//		    m_mousePicMsg[m_lastMousePicMsg]->setVisiable(false);
//		m_mousePicMsg[i]->setVisiable(true);
//		m_lastMousePicMsg = i;
//	}
//}
//
//void crFilterRenderManager::addDequeText(crDrawable *drawable)
//{
//	if(addDrawable(drawable))
//		m_dequeTextVec.push_back(drawable);
//}
//
//void crFilterRenderManager::addDequeString(const std::string &str)
//{
//	if(m_dequeTextVec.empty()) return;
//	int size = m_dequeTextVec.size();
//	int i = m_dequeString.size();
//	if(i==size)
//		m_dequeString.pop_front();
//	//else
//	//{
//	//	for(; i < size - 1; ++i)
//	//	{
//	//		m_dequeString.push_back(" ");
//	//	}
//	//}
//	m_dequeString.push_back(str);
//}
//
//void crFilterRenderManager::updateDequeText()
//{
//	if(m_dequeTextVec.empty()) return;
//
//	int i = 0;
//	for( DequeString::iterator itr =m_dequeString.begin();
//		itr != m_dequeString.end();
//		++itr, ++i )
//	{
//		if(itr->compare(" ") != 0)
//		{
//			m_dequeTextVec[i]->setText(*itr);
//			m_dequeTextVec[i]->setVisiable(true);
//		}
//		else m_dequeTextVec[i]->setVisiable(false);
//	}
//	for(;i<m_dequeTextVec.size();++i)
//	{
//		m_dequeTextVec[i]->setVisiable(false);
//	}
//}
//
//void crFilterRenderManager::setDequeTextInterval(float interval)
//{
//	m_dequeTextInterval = interval;
//}

//bool crFilterRenderManager::add3DDrawable(crDrawable *drawable)
//{
//	return crObject::addDrawable(drawable);
//}
//
//crDrawable *crFilterRenderManager::get3DDrawable(const std::string &name)
//{
//	for( DrawableList::iterator itr = getDrawableList().begin();
//		itr != getDrawableList().end();
//		++itr )
//	{
//		if(name.compare((*itr)->getName()) == 0)
//			return itr->get();
//	}
//	return NULL;
//}
//
void crFilterRenderManager::traverse(crNodeVisitor& nv)
{
	//crObject::traverse(nv);
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if( vt==crNodeVisitor::UPDATE_VISITOR )
	{
		//double curFrameTime = nv.getFrameStamp()->getReferenceTime();
		//if(curFrameTime - m_lastFrameTime > m_dequeTextInterval)
		//{
		//	//m_dequeString.pop_front();
		//	addDequeString(" ");
		//	m_lastFrameTime = curFrameTime;
		//}
	 //   updateDequeText();
		//m_mutex.lock();
		if(m_faildShowType>SH_Hide)
		{
			crCanvasNode *canvas = findCanvas(m_faildShowCanvas);
			if(canvas)
			{
				if(m_faildShowType==SH_DoModal)
					doModal(canvas,m_faildShowCanvas);
				else
					showCanvas(canvas,m_faildShowCanvas,m_faildShowType);
				m_faildShowCanvas.clear();
				m_faildShowType = SH_Hide;
				crBrain::getInstance()->doEvent(WCH_ShowCanvasRecover);
			}
		}
		m_asyncDrawableListMutex.acquire();
		if(!m_asyncAddDrawableList.empty())
		{
			for( crObject::DrawableList::iterator itr = m_asyncAddDrawableList.begin();
				itr != m_asyncAddDrawableList.end();
				++itr )
			{
				m_orthoObject->addDrawable(itr->get());
			}
			m_asyncAddDrawableList.resize(0);
			//m_asyncAddDrawableList.clear();
		}
		if(!m_asyncRemoveDrawableList.empty())
		{
			for( crObject::DrawableList::iterator itr = m_asyncRemoveDrawableList.begin();
				itr != m_asyncRemoveDrawableList.end();
				++itr )
			{
				m_orthoObject->removeDrawable(itr->get());
			}
			m_asyncRemoveDrawableList.resize(0);
			//m_asyncRemoveDrawableList.clear();
		}
		m_asyncDrawableListMutex.release();
		m_identicCamera->accept(nv);
		//m_mutex.unlock();
		//m_orthoCamera->accept(nv);
	}
	else if(vt == crNodeVisitor::SWAPBUFFER_VISITOR)
	{
		m_identicCamera->accept(nv);
	}
	else if(vt==crNodeVisitor::CULL_VISITOR)
	{
		if(crFrameStamp::getInstance()->getFrameNumber()<=10/*INITFRAMENUMBER*/)
			return;
		std::string str = nv.getTraverseString();
		nv.setTraverseString("UIRender");
		//crVector3 pos;
		//float i = 0;
		//NodeArray &childArray = m_identicCamera->getChildArray();
		//for ( NodeArray::iterator itr = childArray.begin();
		//	  itr != childArray.end();
		//	  ++itr )
		//{
		//	canvasNode = dynamic_cast<crCanvasNode *>(itr->get());
		//	if(canvasNode)
		//	{
		//		pos = canvasNode->getTrans();
		//		pos[2] = i;
		//		i-=0.02f;
		//		canvasNode->setPosition(pos);
		//	}
		//}
		crCanvasNode *canvas;
		m_activeCanvasMutex.acquire();
		while(!m_activeCanvasTemp.empty())
		{
			canvas = m_activeCanvasTemp.front().get();
			m_activeCanvasTemp.pop_front();
			for( ActiveCanvasDeque::iterator itr = m_activeCanvasDeque.begin();
				itr != m_activeCanvasDeque.end();
				++itr )
			{
				if((*itr) == canvas)
				{
					m_activeCanvasDeque.erase(itr);
					break;
				}
			}
			m_activeCanvasDeque.push_back(canvas);
		}
		ActiveCanvasDeque activeCanvasDeque;
		ActiveCanvasDeque TopMostDeq;
		ActiveCanvasDeque BottomDeq;
		ActiveCanvasDeque NonTopMostDeq;
		activeCanvasDeque.swap(m_activeCanvasDeque);
		bool hasmodal = false;
		for( ActiveCanvasDeque::iterator itr = activeCanvasDeque.begin(); itr != activeCanvasDeque.end(); ++itr )
		{
			canvas = itr->get();
			if(m_modalCanvas == canvas)
			{
				//TopMostDeq.push_back(canvas);
				hasmodal = true;
				continue;
			}
			else
			{
				switch (canvas->getCanvasPos())
				{
				case CANVAS_TOP:
					m_activeCanvasDeque.push_back(canvas);
					break;
				case CANVAS_BOTTOM:
					BottomDeq.push_front(canvas);
					break;
				case CANVAS_TOPMOST:
					TopMostDeq.push_back(canvas);
					break;
				case CANVAS_NOTOPMOST:
					NonTopMostDeq.push_front(canvas);
					break;
				}
			}
		}
		if(hasmodal)
			m_activeCanvasDeque.push_back(m_modalCanvas.get());
		for( ActiveCanvasDeque::iterator itr = TopMostDeq.begin(); itr != TopMostDeq.end(); ++itr )
		{
			m_activeCanvasDeque.push_back(*itr);
		}
		for( ActiveCanvasDeque::iterator itr = NonTopMostDeq.begin(); itr != NonTopMostDeq.end(); ++itr )
		{
			m_activeCanvasDeque.push_front(*itr);
		}
		for( ActiveCanvasDeque::iterator itr = BottomDeq.begin(); itr != BottomDeq.end(); ++itr )
		{
			m_activeCanvasDeque.push_front(*itr);
		}
		//float i = 0.0f;
		int binnum = 0;
		//crVector3 pos;
		for( ActiveCanvasDeque::iterator itr = m_activeCanvasDeque.begin();
			 itr != m_activeCanvasDeque.end();)
		{
			canvas = dynamic_cast<crCanvasNode *>(itr->get());
			if(!canvas || !canvas->getVisiable())
			{
				itr = m_activeCanvasDeque.erase(itr);
				continue;
			}
			//pos = canvas->getTrans();
			//i+=0.02f;
			//pos[2] = i;
			//canvas->setPosition(pos);
			canvas->getOrCreateStateSet()->setRenderBinDetails(binnum++,"UIBin");
			++itr;
		}
		m_activeCanvasMutex.release();
		if(m_mouseCanvas.valid())
		{
			m_mouseCanvas->getOrCreateStateSet()->setRenderBinDetails(binnum++,"UIBin");
		}
		m_identicCamera->accept(nv);
		nv.setTraverseString(str);
	}
	else if ( vt==crNodeVisitor::INIT_VISITOR )
	{
		//if(vt==crNodeVisitor::CULL_VISITOR)
		//    CRCore::notify(CRCore::ALWAYS)<<"crFilterRenderManager::traverse "<<nv.getFrameStamp()->getFrameNumber()<< std::endl;
		//m_mutex.lock();
		m_identicCamera->accept(nv);
		//m_orthoCamera->accept(nv);
		//for( CanvasArray::iterator itr = m_canvasArray.begin();
		//	 itr != m_canvasArray.end();
		//	 ++itr )
		//{
		//	(*itr)->accept(nv);
		//}
		//m_mutex.unlock();
	}
}

void crFilterRenderManager::releaseObjects(crState* state)
{
	m_identicCamera->releaseObjects(state);
	//m_orthoCamera->releaseObjects(state);
}

bool crFilterRenderManager::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	m_bBoundSphere_computed = true;
	return true;
}
//void crFilterRenderManager::addFilterNode(crNode *filterNode)
//{
//    m_identicCamera->addChild(filterNode);
//}
//
//crNode *crFilterRenderManager::getFilterNode(const std::string &name,crSearchNodeBYNameVisitor::SearchNodeType nodeType)
//{
//    if(!m_searchVisitor.valid())
//		m_searchVisitor = new crSearchNodeBYNameVisitor;
//	m_searchVisitor->reset();
//	m_searchVisitor->setNameId(name);
//	m_searchVisitor->setSearchNodeType(nodeType);
//	m_identicCamera->accept(*m_searchVisitor);
//	return m_searchVisitor->getResult();
//}
//
//crGroup *crFilterRenderManager::getFilterRoot()
//{
//	return m_identicCamera.get();
//}
//void crFilterRenderManager::removeCanvas(const std::string &id)
//{
//	NodeArray &canvasArray = m_identicCamera->getChildArray();
//	for( NodeArray::iterator itr = canvasArray.begin();
//		itr != canvasArray.end();
//		++itr )
//	{
//		if((*itr)->getName().compare(id) == 0)
//		{
//			//closeCanvas(itr->get());
//			canvasArray.erase(itr);
//			break;
//		}
//	}
//}
//void crFilterRenderManager::addCanvas(crCanvasNode *canvas)
//{
//	if(canvas->getName().compare(m_mainCanvas->getName()) == 0)
//		return;
//	//removeCanvas(canvas->getName());
//	m_canvasArray.push_back(canvas);
//
//	CRCore::crCollectNodeBYClassNameVisitor searchVisitor;
//	searchVisitor.insertClassNameId("crWidgetNode");
//	canvas->accept(searchVisitor);
//	NodeArray &widgetArray = searchVisitor.getResult();
//	for( NodeArray::iterator itr = widgetArray.begin();
//		itr != widgetArray.end();
//		++itr )
//	{
//		(dynamic_cast<crWidgetNode *>(itr->get()))->setParentCanvas(canvas);
//	}
//}

crCanvasNode *crFilterRenderManager::findCanvas(const std::string& id)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	if(id.compare(m_mainCanvas->getName()) == 0)
		return m_mainCanvas.get();
	NodeArray canvasArray = m_identicCamera->getChildArray();
	for( NodeArray::iterator itr = canvasArray.begin();
		 itr != canvasArray.end();
		 ++itr )
	{
		if((*itr)->getName().compare(id) == 0)
			return dynamic_cast<crCanvasNode *>(itr->get());
	}
	return NULL;
}
void crFilterRenderManager::closeAllCanvas()
{
	if(!m_inited)
		return;
	NodeArray &canvasArray = m_identicCamera->getChildArray();
	for( NodeArray::iterator itr = canvasArray.begin();
		itr != canvasArray.end();
		++itr )
	{
		if(m_mainCanvas.get() != itr->get() && dynamic_cast<crCanvasNode *>(itr->get()))
			closeCanvas(dynamic_cast<crCanvasNode *>(itr->get()));
	}
}
void crFilterRenderManager::showCanvas(const std::string& id, bool show)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	ref_ptr<crCanvasNode>canvas = findCanvas(id);
	//if(canvas.valid())
	showCanvas(canvas.get(),id,show?SH_Show:SH_Hide);
}
void crFilterRenderManager::showCanvas(crCanvasNode *canvas, bool show)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited || !canvas)
		return;
	showCanvas(canvas,canvas->getName(),show?SH_Show:SH_Hide);
}
void crFilterRenderManager::showCanvas(crCanvasNode *canvas, const std::string& id, ShowMode show)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(!canvas)
	{
		if(show>0)
		{
			m_faildShowCanvas = id;
			m_faildShowType = show;
			char gbuf[256];
			sprintf(gbuf,"ShowCanvasFaild:%s,mode:%d\n\0",m_faildShowCanvas.c_str(),(int)m_faildShowType);
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			crBrain::getInstance()->doEvent(WCH_ShowCanvasFaild);
		}
		return;
	}
	if(m_mainCanvas == canvas)
		m_mainCanvas->setVisiable(show);
	else if(show)
	{
		canvas->setVisiable(true);
		canvas->setEditorHidden(false);
		setActiveCanvas(canvas);
		canvas->doEvent(MAKEINT64(WCH_UI_OnShow,1));
	}
	else
	{
		closeCanvas(canvas);
	}
}
void crFilterRenderManager::doModal(const std::string& id)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	ref_ptr<crCanvasNode>canvas = findCanvas(id);
	//if(canvas.valid())
	doModal(canvas.get(),id);
}
void crFilterRenderManager::doModal(crCanvasNode *canvas)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited||!canvas)
		return;
	doModal(canvas,canvas->getName());
}
void crFilterRenderManager::doModal(crCanvasNode *canvas,const std::string& id)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(m_mainCanvas != canvas/* && !m_modalCanvas.valid()*/)
	{
		//canvas->setCanvasPos(CANVAS_TOPMOST);
		showCanvas(canvas,id,SH_DoModal);
		m_modalCanvas = canvas;
		//canvas->setIsModalDlg(true);
	}
}
void crFilterRenderManager::closeCanvas(const std::string& id)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	ref_ptr<crCanvasNode> canvas = findCanvas(id);
	if(canvas.valid())
		closeCanvas(canvas.get());
}

void crFilterRenderManager::closeCanvas(crCanvasNode *canvas)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited || !canvas)
		return;
	if(canvas->getVisiable())
	{
		//if(m_mainCanvas != canvas)
		//{
		//	//CRCore::ScopedLock<crMutex> lock(m_mutex);
		//	m_identicCamera->removeChild(canvas);
		//}
		canvas->setVisiable(false);
		canvas->setEditorHidden(true);
		if(m_currentMouseCapturer.valid())
		{
			if(dynamic_cast<crCanvasNode *>(m_currentMouseCapturer.get()))
			{
				if(m_currentMouseCapturer == canvas)
					setMouseCapturer(NULL);
			}
			else if(m_currentMouseCapturer->getParentCanvas() == canvas)
			{
				setMouseCapturer(NULL);
			}
		}
		if(m_currentInputCapturer.valid())
		{
			if(dynamic_cast<crCanvasNode *>(m_currentInputCapturer.get()))
			{
				if(m_currentInputCapturer == canvas)
					setInputCapturer(NULL);
			}
			else if(m_currentInputCapturer->getParentCanvas() == canvas)
			{
				setInputCapturer(NULL);
			}
		}
		if(m_modalCanvas == canvas)
			m_modalCanvas = NULL;
		canvas->doEvent(MAKEINT64(WCH_UI_OnShow,0));
	}
}
crWidgetNode *crFilterRenderManager::getMouseCapturer()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	//if(m_currentMouseCapturer.valid())
	//{
	//	if (!m_currentMouseCapturer->getCanCaptureMouse())
	//	{
	//		setMouseCapturer(NULL);
	//	}
	//}
	return m_currentMouseCapturer.get(); 
}
crWidgetNode *crFilterRenderManager::getInputCapturer()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	//if(m_currentInputCapturer.valid())
	//{
	//	if (!m_currentInputCapturer->getCanCaptureInput())
	//	{
	//		setInputCapturer(NULL);
	//	}
	//}
	return m_currentInputCapturer.get(); 
}
void crFilterRenderManager::mousePick(const crVector2& mouse,crWidgetNode *&pickedWidget)
{//只能对m_identicCamera进行pick，m_orthoCamera不做pick
	//返回picked object
	//CRCore::ScopedLock<crMutex> lock(m_mutex);
	crWidgetNode *mouseCapturer = getMouseCapturer();
	if(m_modalCanvas.valid())
	{
		const crBoundingBox& bbox = m_modalCanvas->getBoundBox();
		if( mouse[0]>=bbox.m_min[0] && mouse[0]<=bbox.m_max[0] && 
			mouse[1]>=bbox.m_min[1] && mouse[1]<=bbox.m_max[1])
		{
			m_mousePickVisitor.setMouse(mouse);
			m_modalCanvas->accept(m_mousePickVisitor);
			m_mousePickVisitor.distillPickedUI(pickedWidget);
		}
		if(!pickedWidget)
		{
			pickedWidget = m_modalCanvas.get();
		}
	}
	else if(mouseCapturer)
	{
		if(dynamic_cast<crCanvasNode *>(mouseCapturer))
		{
			m_mousePickVisitor.setMouse(mouse);
			mouseCapturer->accept(m_mousePickVisitor);
			m_mousePickVisitor.distillPickedUI(pickedWidget);
		}
		if(!pickedWidget)
			pickedWidget = mouseCapturer;
	}
	else
	{
		m_mousePickVisitor.setMouse(mouse);
		//CRCore::ScopedLock<crMutex> lock(m_mutex);
		crCanvasNode *canvas;
		m_activeCanvasMutex.acquire();
		for( ActiveCanvasDeque::reverse_iterator itr = m_activeCanvasDeque.rbegin();
			itr != m_activeCanvasDeque.rend();
			++itr )
		{
			canvas = dynamic_cast<crCanvasNode *>(itr->get());
			if(!canvas || !canvas->getVisiable() || !canvas->getCanFocus()/* || !canvas->getEnable()*/)
			{
				continue;
			}
			const crBoundingBox& bbox = canvas->getBoundBox();
			if( mouse[0]>=bbox.m_min[0] && mouse[0]<=bbox.m_max[0] && 
				mouse[1]>=bbox.m_min[1] && mouse[1]<=bbox.m_max[1])
			{
				canvas->accept(m_mousePickVisitor);
				m_mousePickVisitor.distillPickedUI(pickedWidget);
				if(pickedWidget) break;
			}
		}
		m_activeCanvasMutex.release();
		if(!pickedWidget)
		{
			m_mainCanvas->accept(m_mousePickVisitor);
			m_mousePickVisitor.distillPickedUI(pickedWidget);
			if(pickedWidget == m_mainCanvas.get())
				pickedWidget = NULL;
		}
		//NodeArray &childArray = m_identicCamera->getChildArray();
		//int count = childArray.size();
		//crNode *node;
		//for( int i = count-1; i>=0; --i)
		//{
		//	node = childArray[i].get();
		//	const crBoundingBox& bbox = node->getBoundBox();
		//	if( mouse[0]>=bbox.m_min[0] && mouse[0]<=bbox.m_max[0] && 
		//		mouse[1]>=bbox.m_min[1] && mouse[1]<=bbox.m_max[1])
		//	{
		//		node->accept(m_mousePickVisitor);
		//		m_mousePickVisitor.distillPickedUI(pickedWidget);
		//		if(pickedWidget == m_mainCanvas.get())
		//			pickedWidget = NULL;
		//		break;
		//	}
		//}
	}
}
void crFilterRenderManager::setFocusNode(crWidgetNode *node) 
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(m_focusNode == node || node == m_mainCanvas.get()) return;

	if(m_focusNode.valid())
	{
		if(m_focusNode->getEnable()) m_focusNode->kernelLostFocus();
		m_focusNode->doEvent(WCH_FILTERLOSTFOCUS);
	}
	if(node && node->getCanFocus())
	{
		m_focusNode = node; 
		if(node->getEnable()) node->kernelGetFocus();
		node->doEvent(WCH_FILTERGETFOCUS); 
	}
	else
	{
		m_focusNode = NULL;
	}
}
void crFilterRenderManager::setMouseCapturer(crWidgetNode *node)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	//CRCore::notify(CRCore::FATAL)<<"setMouseCapturer Current "<<m_currentMouseCapturer.get()<<" SetNode "<<node<<std::endl;
	if(m_currentMouseCapturer == node) return;
	
	if(m_currentMouseCapturer.valid())
	{
		m_currentMouseCapturer->kernelLostCapture();
		m_currentMouseCapturer->doEvent(WCH_WIDGETLOSTCAPTURE);
	}
	m_currentMouseCapturer = node; 
	if(node && node->getCanCaptureMouse())
	{
		node->kernelGetCapture();
		node->doEvent(WCH_WIDGETGETCAPTURE);
	}
}
void crFilterRenderManager::setInputCapturer(crWidgetNode *node)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(m_currentInputCapturer == node) return;

	if(m_currentInputCapturer.valid())
	{
		m_currentInputCapturer->kernelLostCapture();
		m_currentInputCapturer->doEvent(WCH_WIDGETLOSTCAPTURE);
	}
	m_currentInputCapturer = node; 
	if(node && node->getCanCaptureInput())
	{
		node->kernelGetCapture();
		node->doEvent(WCH_WIDGETGETCAPTURE,MAKEINT64(NULL,node));
	}
}
void crFilterRenderManager::setActiveCanvas(crCanvasNode *canvas)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(canvas && m_mainCanvas != canvas)
	{
		m_activeCanvasMutex.acquire();
		m_activeCanvasTemp.push_back(canvas);
		m_activeCanvasMutex.release();
		////CRCore::ScopedLock<crMutex> lock(m_mutex);
		//int visableCanvasCount = m_identicCamera->getNumChildren();
		//if(visableCanvasCount>1)
		//{
		//	crCanvasNode *node = dynamic_cast<crCanvasNode *>(m_identicCamera->getChild(visableCanvasCount-1));
		//	if(node == canvas)
		//	{
		//		return;
		//	}
		//	else
		//	{
		//		m_identicCamera->removeChild(canvas);
		//	}
		//}
		//m_identicCamera->addChild(canvas);
		//crCanvasNode *canvasNode;
		//crVector3 pos;
		//float i = 0;
		//NodeArray &childArray = m_identicCamera->getChildArray();
		//for ( NodeArray::iterator itr = childArray.begin();
		//	  itr != childArray.end();
		//	  ++itr )
		//{
		//	canvasNode = dynamic_cast<crCanvasNode *>(itr->get());
		//	if(canvasNode)
		//	{
		//		pos = canvasNode->getTrans();
		//		pos[2] = i;
		//		i-=0.02f;
		//		canvasNode->setPosition(pos);
		//	}
		//}
	}
}
crWidgetNode* crFilterRenderManager::getCurrentInputWidget()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
    if(m_currentInputCapturer.valid())
		return m_currentInputCapturer.get();
	else if(m_focusNode.valid())
		return m_focusNode.get();
	return NULL;
}
crCanvasNode* crFilterRenderManager::getMainCanvas()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	return m_mainCanvas.get();
}
crCanvasNode* crFilterRenderManager::getModalCanvas()
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return NULL;
	return m_modalCanvas.get();
}
void crFilterRenderManager::setMouseCanvas(crCanvasNode *mouseCanvas)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(!m_mouseCanvas.valid() && mouseCanvas)
	{
		m_mouseCanvas = mouseCanvas;
		m_mouseCanvas->setName("MouseUI");
		m_identicCamera->addChild(m_mouseCanvas.get());
	}
	else
	{
		if(m_mouseCanvas.valid())
		{
			char gbuf[256];
			sprintf(gbuf,"crFilterRenderManager::setMouseCanvas 错误,MouseCanvas已经存在\n\0");
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
		else
		{
			char gbuf[256];
			sprintf(gbuf,"crFilterRenderManager::setMouseCanvas 错误,MouseCanvas不能设为NULL\n\0");
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
	}
}
void crFilterRenderManager::setLoadingCanvas(crCanvasNode *loadingCanvas)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_initMutex);
	if(!m_inited)
		return;
	if(!m_loadingCanvas.valid() && loadingCanvas)
	{
		m_loadingCanvas = loadingCanvas;
		m_loadingCanvas->setName("LoadingUI");
		m_identicCamera->addChild(m_loadingCanvas.get());
	}
	else
	{
		if(m_loadingCanvas.valid())
		{
			char gbuf[256];
			sprintf(gbuf,"crFilterRenderManager::setLoadingCanvas 错误,LoadingCanvas已经存在\n\0");
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
		else
		{
			char gbuf[256];
			sprintf(gbuf,"crFilterRenderManager::setLoadingCanvas 错误,LoadingCanvas不能设为NULL\n\0");
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
	}
}
void crFilterRenderManager::removeLoadingUI()
{
	if(m_loadingCanvas.valid())
	{
		closeCanvas(m_loadingCanvas.get());
		m_identicCamera->removeChild(m_loadingCanvas.get());
		m_loadingCanvas = NULL;
	}
}
void crFilterRenderManager::resize()
{
	if (m_inited)
	{
		computeMVPW();
		float width = crDisplaySettings::instance()->getViewWidth();
		float height = crDisplaySettings::instance()->getViewHeight();
		m_identicCamera->setViewport(0, 0, width, height);
		m_mvpwNode->setMatrix(m_inverseMVPW);
		crResizeUIVisitor resizeVisitor;
		m_identicCamera->accept(resizeVisitor);
	}
}