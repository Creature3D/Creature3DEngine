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
#include <CRCore/crWidgetNode.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crScissor.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crShapeDrawable.h>

using namespace CRCore;
crWidgetNode::crWidgetNode():
m_canFocus(true),
m_enable(true),
m_canCaptureMouse(false),
m_canCaptureInput(false),
m_matrixEnable(false),
m_parentCanvas(NULL),
m_parentWidget(NULL),
m_inited(false),
m_scaled(false),
m_swapEnableTime(0.0f),
m_msgtraverse(KM_None)
{
	setEnableIntersect(false);
}
crWidgetNode::crWidgetNode(const crWidgetNode& node,const crCopyOp& copyop):
crMatrixTransform(node,copyop),
m_canFocus(node.m_canFocus),
m_enable(node.m_enable),
m_canCaptureMouse(node.m_canCaptureMouse),
m_canCaptureInput(node.m_canCaptureInput),
m_matrixEnable(node.m_matrixEnable),
m_parentCanvas(NULL),
m_parentWidget(NULL),
m_inited(false),
m_scaled(node.m_scaled),
m_swapEnableTime(0.0f),
m_msgtraverse(node.m_msgtraverse)
{    
}
crWidgetNode::~crWidgetNode()
{
	doEvent(WCH_UI_DestroyWindow);
}
void crWidgetNode::mouseTrans(float &mx, float &my)
{
	if(m_matrixEnable)
	{
		crVector3 mouse(mx,my,0.0f);
		mouse = mouse * getInverseMatrix();
		mx = mouse[0];
		my = mouse[1];
	}	
}
void crWidgetNode::swapEnable(float interval)
{
	//setEnable(!m_enable);
	setEnable(false);
	m_swapEnableTime = interval;
}
void crWidgetNode::traverse(crNodeVisitor& nv)
{
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if ( vt==crNodeVisitor::INIT_VISITOR )
	{
		if(!m_inited)
		{
			doEvent(MAKEINT64(WCH_UI_InitWindow,NULL));
			initWindow();
			doEvent(MAKEINT64(WCH_UI_InitWindow,1));
			m_inited = true;
		}
	}
	else if ( vt==crNodeVisitor::CULL_VISITOR )
	{
		//if(!m_inited)
		//{
		//	doEvent(MAKEINT64(WCH_UI_InitWindow,NULL));
		//	initWindow();
		//	doEvent(MAKEINT64(WCH_UI_InitWindow,1));
		//	m_inited = true; 
		//}
		//else
		//{
		if(m_swapEnableTime>0.0f)
		{
			m_swapEnableTime -= crFrameStamp::getInstance()->getFrameInterval();
			if(m_swapEnableTime<0.0f)
			{
				m_swapEnableTime = 0.0f;
				if(!m_enable) setEnable(true);
			}
		}
		//doEvent(MAKEINT64(WCH_UI_UpdateData,NULL));
		updateData();
		float dt = crFrameStamp::getInstance()->getFrameInterval();
		doEvent(WCH_UI_Frame,MAKEINT64(&dt,NULL));
		//}
	}
	crMatrixTransform::traverse(nv);
}
void crWidgetNode::initWindow()
{
}
void crWidgetNode::updateData()
{
}
void crWidgetNode::setEnable(bool enable)
{
	m_enable = enable;
}
void crWidgetNode::setCanFocus(bool canFocus)
{
	m_canFocus = canFocus;
}
void crWidgetNode::setEnableMatrix(bool enable)
{
	m_matrixEnable = enable;
}
void crWidgetNode::setCanCaptureMouse(bool canCapture)
{
	m_canCaptureMouse = canCapture;
}
void crWidgetNode::setCanCaptureInput(bool canCapture)
{
	m_canCaptureInput = canCapture;
}
bool crWidgetNode::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	if(m_matrixEnable)
	{
		if (m_referenceFrame==RELATIVE_TO_PARENTS)
		{
			matrix.preMult(m_matrix);
		}
		else // absolute
		{
			matrix = m_matrix;
		}
	}
	return true;
}
bool crWidgetNode::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	if(m_matrixEnable)
	{
		const crMatrix& inverse = getInverseMatrix();

		if (m_referenceFrame==RELATIVE_TO_PARENTS)
		{
			matrix.postMult(inverse);
		}
		else // absolute
		{
			matrix = inverse;
		}
	}
	return true;
}
bool crWidgetNode::computeBound() const
{
	if(m_matrixEnable)
		return crMatrixTransform::computeBound();
	else
		return crGroup::computeBound();
}
void crWidgetNode::setParentWidget(crWidgetNode *parent)
{
	m_parentWidget = parent;
}
crWidgetNode *crWidgetNode::getParentWidget()
{
	return m_parentWidget;
}
void crWidgetNode::setMsgTraverse(unsigned short msgtravers)
{
	m_msgtraverse = (KeyboardMouseMode)msgtravers;
}
unsigned short crWidgetNode::getMsgTraverse()
{
	return m_msgtraverse;
}
void crWidgetNode::setScaled(bool bln)
{
	m_scaled = bln;
}
bool crWidgetNode::getScaled()
{
	return m_scaled;
}
////////////////////////////////////////////////
//
//crCanvasNode
//
/////////////////////////////////////////////////
crCanvasNode::crCanvasNode():
	m_canvasPos(CANVAS_TOP)
{
	setCanCaptureMouse(true);
	setEnableMatrix(true);
}

crCanvasNode::crCanvasNode(const crCanvasNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),
m_canvasPos(node.m_canvasPos)
{
}
crCanvasNode::~crCanvasNode()
{
}
void crCanvasNode::releaseObjects(crState* state)
{
	crFilterRenderManager::getInstance()->closeCanvas(this);
	crWidgetNode::releaseObjects(state);
}
void crCanvasNode::initWindow()
{
	////crVector3 minpos = getBoundBox().m_min;
	////crVector3 maxpos = getBoundBox().m_max;
	////minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	////maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	////setStateSet(new crStateSet);
	////crScissor *scissor = new crScissor;
	////scissor->setScissor(minpos[0],minpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);
	////m_stateset->setAttributeAndModes(scissor,crStateAttribute::ON);
	if(!m_scaled && crDisplaySettings::instance()->getRunMode() > 0)
	{//游戏模式
		m_scaled = true;
		//float viewWidth = crDisplaySettings::instance()->getViewWidth();
		//float viewHeight = crDisplaySettings::instance()->getViewHeight();
		//float uiScale = crDisplaySettings::instance()->getUIScale();
		//float heightScale = viewWidth/viewHeight * uiScale;//crDisplaySettings::instance()->getStandardWinRatio();
		const crBoundingBox &bbox = getBoundBox();
		crVector3 wndCenter = bbox.center();
		crVector2 centerCoord(wndCenter[0],wndCenter[1]);
		crVector4 rect(bbox.m_min[0],bbox.m_min[1],bbox.m_max[0],bbox.m_max[1]);
		
		//crMatrix scaleMat = crMatrix::scale(uiScale,heightScale,1.0f);
		float heightScale = crDisplaySettings::instance()->getUIHeightScale();
		float uiScale = crDisplaySettings::instance()->getUIScale();
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		crAcceptMatrixVisitor acceptMatrix(scaleMat);
		traverse(acceptMatrix);

		int notTrans = 0;//1表示不移动，2表示左右不移动，3表示上下不移动
		crArgumentParser::readKeyValue(getDescriptions(),"NotTrans",notTrans);
		if(notTrans != 1 && crFilterRenderManager::getInstance()->getMainCanvas() != this && crFilterRenderManager::getInstance()->getMouseCanvas() != this)
		{
			const crBoundingBox &bbox2 = getBoundBox();
			crVector4 rect2(bbox2.m_min[0],bbox2.m_min[1],bbox2.m_max[0],bbox2.m_max[1]);
			crVector2 trans;
			crVector4 offset,offset2;
			offset[0] = 1.0f + rect[0];
			offset[1] = 1.0f + rect[1];
			offset[2] = 1.0f - rect[2];
			offset[3] = 1.0f - rect[3];

			offset2[0] = uiScale + rect2[0];
			offset2[1] = heightScale + rect2[1];
			offset2[2] = uiScale - rect2[2];
			offset2[3] = heightScale - rect2[3];
			if(notTrans!=2)
			{
				if(centerCoord[0]<-0.1f)
				{//左对齐
					//trans[0] = rect[0] - rect2[0];
					trans[0] = offset[0]/uiScale - offset2[0] + uiScale - 1.0f;
				}
				else if(centerCoord[0]>0.1f)
				{//右对齐
					trans[0] = offset2[2]-offset[2]*uiScale - uiScale + 1.0f;
					//trans[0] = rect[2] - rect2[2];
				}
			}
			if(notTrans!=3)
			{
				if(centerCoord[1]<-0.1f)
				{//上对齐
					trans[1] = offset[1]/heightScale - offset2[1] + heightScale - 1.0f;
					//trans[1] = rect[1] - rect2[1];
				}
				else if(centerCoord[1]>0.1f)
				{//下对齐
					trans[1] = offset2[3]-offset[3]*heightScale - heightScale + 1.0f;
					//trans[1] = rect[3] - rect2[3];
				}
			}

			if(trans != crVector2(0.0f,0.0f))
			{
				acceptMatrix.setMatrix(crMatrix::translate(trans[0],trans[1],0.0f));
				traverse(acceptMatrix);
			}
			//if(bbox.m_max[1]>1.0f)
			//{
			//	acceptMatrix.setMatrix(crMatrix::translate(0.0f,1.0f-bbox.m_max[1],0.0f));
			//	traverse(acceptMatrix);
			//}
			//else if(bbox.m_min[1]<-1.0f)
			//{
			//	acceptMatrix.setMatrix(crMatrix::translate(0.0f,-1.0f - bbox.m_min[1],0.0f));
			//	traverse(acceptMatrix);
			//}
			//if(bbox.m_max[0]>1.0f)
			//{
			//	acceptMatrix.setMatrix(crMatrix::translate(1.0f-bbox.m_max[0],0.0f,0.0f));
			//	traverse(acceptMatrix);
			//}
			//else if(bbox.m_min[0]<-1.0f)
			//{
			//	acceptMatrix.setMatrix(crMatrix::translate(-1.0f - bbox.m_min[0],0.0f,0.0f));
			//	traverse(acceptMatrix);
			//}
		}
	}
	crSetParentCanvasVisitor vistor(this);
	traverse(vistor);
}
crWidgetNode *crCanvasNode::getWidget(const std::string &name)
{
	crSearchNodeBYNameVisitor search;
	search.reset();
	search.setNameId(name);
	search.setSearchNodeType(WIDGETNODE);
	accept(search);
	return dynamic_cast<crWidgetNode *>(search.getResult());
}
crNode *crCanvasNode::getChildNode(const std::string &name)
{
	//for( NodeArray::iterator itr = m_childArray.begin();
	//	 itr != m_childArray.end();
	//	 ++itr )
	//{
	//	if((*itr)->getName().compare(name)==0)
	//	{
	//		return itr->get();
	//	}
	//}
	//return NULL;
	if(name.empty())
	{
		return NULL;
	}
	crSearchNodeBYNameVisitor search;
	search.reset();
	search.setNameId(name);
	search.setSearchNodeType(ALLNODE);
	accept(search);
	return search.getResult();
}
void crCanvasNode::addTabWidget(const std::string &name)
{
	ref_ptr<crWidgetNode> widget = getWidget(name);
	if(widget.valid())
		m_tabWidgetVec.push_back(widget.get());
}
void crCanvasNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case 0xFF09:
		if(!m_tabWidgetVec.empty())
		{
			int  curForcusIndex = 0;
			crWidgetNode *curFocusWidget = crFilterRenderManager::getInstance()->getInputCapturer();
			if(curFocusWidget)
			{
				int i = 0;
				for( TabWidgetVec::iterator itr = m_tabWidgetVec.begin();
					 itr != m_tabWidgetVec.end();
					 ++itr,++i )
				{
					if(itr->get() == curFocusWidget)
					{
						curForcusIndex = i;
						break;
					}
				}
			}
			curForcusIndex++;
			if(curForcusIndex>=m_tabWidgetVec.size())
				curForcusIndex = 0;
			crWidgetNode *focusWidget = m_tabWidgetVec[curForcusIndex].get();
			crFilterRenderManager::getInstance()->setFocusNode(focusWidget);
			if (focusWidget->getEnable())
			{
				if(focusWidget->getCanCaptureMouse())
					crFilterRenderManager::getInstance()->setMouseCapturer(focusWidget);
				if(focusWidget->getCanCaptureInput())
					crFilterRenderManager::getInstance()->setInputCapturer(focusWidget);
				else
					crFilterRenderManager::getInstance()->setInputCapturer(NULL);
			}
		}
		break;
	}
}
void crCanvasNode::setDefFocusIndex(int index)
{
	if(index<m_tabWidgetVec.size())
	{
		int currentFocusIndex = index;
		crWidgetNode *focusWidget = m_tabWidgetVec[currentFocusIndex].get();
		crFilterRenderManager::getInstance()->setFocusNode(focusWidget);
		if (focusWidget->getEnable())
		{
			if(focusWidget->getCanCaptureMouse())
				crFilterRenderManager::getInstance()->setMouseCapturer(focusWidget);
			if(focusWidget->getCanCaptureInput())
				crFilterRenderManager::getInstance()->setInputCapturer(focusWidget);
			else
				crFilterRenderManager::getInstance()->setInputCapturer(NULL);
		}
	}
}
////////////////////////////////////////////////
//
//crAcceptMatrixVisitor
//
/////////////////////////////////////////////////
void crAcceptMatrixVisitor::apply(crObject& node)
{
	Vec3Array *vertexArray = NULL;
	for(CRCore::crObject::DrawableList::iterator itr = node.getDrawableList().begin();
		itr != node.getDrawableList().end();
		++itr)
	{
		if(dynamic_cast<CRCore::crGeometry *>(itr->get()))
		{
			crGeometry *geometry = dynamic_cast<CRCore::crGeometry *>(itr->get());
			vertexArray = (Vec3Array*)(geometry->getVertexArray());

			for( Vec3Array::iterator vitr = vertexArray->begin();
				vitr != vertexArray->end();
				++vitr )
			{
				*vitr = *vitr * m_mat;
			}
		}
		else if(dynamic_cast<CRCore::crShapeDrawable *>(itr->get()))
		{

		}

		(*itr)->dirtyBound();
	}
}