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
#include <CRUI/crWidgetExtend.h>
#include <CRText/crText.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crStateSet.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crCookFile.h>
#include <CRCore/crBrain.h>
#include <CRCore/crCalcNodeParentsMatrixVisitor.h>
#include <CREncapsulation/crLoader.h>
#include <CRText/crString.h>
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#endif
#include <sys/stat.h>
using namespace CRUI;
using namespace CRCore;
using namespace CRText;
using namespace CRProducer;
using namespace CRIOManager;

//#pragma comment( lib, "Coreimm.lib" )
//#pragma comment( lib, "imm32.lib" )

//////////////////////////////////////////////////////////////////////////
//
//crButtonWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crButtonWidgetNode::crButtonWidgetNode()
{
}
crButtonWidgetNode::crButtonWidgetNode(const crButtonWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop)//SHALLOW_COPY会存在问题
{
}
crButtonWidgetNode::~crButtonWidgetNode()
{

}
void crButtonWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		if(_ea->getEventType() == crGUIEventAdapter::PUSH)
		{
			m_button->setActiveSwitchSet(2);
			if(m_parentWidget)
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_LBtnDown);
			}
		}
		else if(_ea->getEventType() == crGUIEventAdapter::RELEASE)
		{
			m_button->setActiveSwitchSet(1);
			if(m_parentWidget)
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_LBtnUp);
			}
		}
	}
}
void crButtonWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Return:
		m_button->setActiveSwitchSet(2);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this);
		}
		break;
	}
}
void crButtonWidgetNode::kernelKeyUp(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Return:
		m_button->setActiveSwitchSet(1);
		break;
	}
}
void crButtonWidgetNode::kernelGetFocus()
{
	m_button->setActiveSwitchSet(1);
}
void crButtonWidgetNode::kernelLostFocus()
{
	m_button->setActiveSwitchSet(0);
}
void crButtonWidgetNode::initWindow()
{
	NodeArray &nodeArray = getChildArray();
	for( NodeArray::iterator itr = nodeArray.begin();
		 itr != nodeArray.end();
		 ++itr )
	{
		m_button = dynamic_cast<crMultiSwitch *>(itr->get());
		if(m_button.valid())
			break;
	}
	if(!m_button)
	{
		std::string str = "ButtonWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	if(m_enable)
	{
		m_button->setActiveSwitchSet(0);
	}
	else
	{
		m_button->setActiveSwitchSet(2);
	}
}
void crButtonWidgetNode::setEnable(bool enable)
{
	if (!m_button)
	{
		return;
	}
	crWidgetNode::setEnable(enable);
	if(enable)
	{
		if(m_button->getActiveSwitchSet() == 3) m_button->setActiveSwitchSet(0);
	}
	else
	{
		m_button->setActiveSwitchSet(3);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crScrollBarWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crScrollBarWidgetNode::crScrollBarWidgetNode():
m_value(0.0),
m_lineValue(0.05f),
m_pageValue(0.1f),
m_scrollBarType(HORIZONTALBAR),
m_drag(false),
m_lmax(1.0f),
m_step(0)
{
}
crScrollBarWidgetNode::crScrollBarWidgetNode(const crScrollBarWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_value(node.m_value),
m_lineValue(node.m_lineValue),
m_pageValue(node.m_pageValue),
m_scrollBarType(node.m_scrollBarType),
m_drag(false),
m_lmax(1.0f),
m_step(0)
{
}
crScrollBarWidgetNode::~crScrollBarWidgetNode()
{
}
void crScrollBarWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	if(!m_enable || m_range[1] == m_range[0])
		return;
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	crCanvasNode *canvas = getParentCanvas();
	canvas->mouseTrans(mx,my);
	const crBoundingBox &bt0bbox = m_button0->getBoundBox();
	const crBoundingBox &bt1bbox = m_button1->getBoundBox();
	const crBoundingBox &barbox = m_scrollBar->getBoundBox();
	float vmax = m_range[1] - m_range[0];
	//float lmax;
	float newvalue = m_value;
	m_step = 0;
	//if(m_scrollBarType == HORIZONTALBAR)
	//{
	//	lmax = bt1bbox.m_max[0] - bt0bbox.m_min[0] - barbox.xLength();
	//}
	//else
	//{
	//	lmax = bt1bbox.m_max[1] - bt0bbox.m_min[1] - barbox.yLength();
	//}
	// m_drag = false;
	do 
	{
		if(_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			m_step = 0;
		}
		/////button0 test
		m_button0->setActiveSwitchSet(0);
		if( mx>=bt0bbox.m_min[0] && mx<=bt0bbox.m_max[0] && 
			my>=bt0bbox.m_min[1] && my<=bt0bbox.m_max[1] )
		{
			if(_ea->getEventType() == crGUIEventAdapter::MOVE)
				m_button0->setActiveSwitchSet(1);
			else if(_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				m_button0->setActiveSwitchSet(1);
				m_drag = false;
				setCanCaptureMouse(false);
				m_step = 0;
			}
			else if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON && (_ea->getEventType() == crGUIEventAdapter::PUSH/* || _ea->getEventType() == crGUIEventAdapter::DRAG*/))
			{
				m_button0->setActiveSwitchSet(2);
				newvalue -= m_lineValue;
				m_step = -m_lineValue;
			}
			break;
		}
		//////button1 test
		m_button1->setActiveSwitchSet(0);
		if( mx>=bt1bbox.m_min[0] && mx<=bt1bbox.m_max[0] && 
			my>=bt1bbox.m_min[1] && my<=bt1bbox.m_max[1] )
		{
			if(_ea->getEventType() == crGUIEventAdapter::MOVE)
				m_button1->setActiveSwitchSet(1);
			else if(_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				m_button1->setActiveSwitchSet(1);
				m_drag = false;
				setCanCaptureMouse(false);
				m_step = 0;
			}
			else if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON && (_ea->getEventType() == crGUIEventAdapter::PUSH/* || _ea->getEventType() == crGUIEventAdapter::DRAG*/))
			{
				m_button1->setActiveSwitchSet(2);
				newvalue += m_lineValue;
				m_step = m_lineValue;
			}
			break;
		}
		/////////bar test
		crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_scrollBar->getChild(0));
		bar->setActiveSwitchSet(0);
		if(_ea->getEventType() == crGUIEventAdapter::PUSH && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if(m_scrollBarType == VERTICALBAR)
			{
				if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] &&
				    my>barbox.m_max[1] && my<bt0bbox.m_min[1] )
				{
					newvalue -= m_pageValue;
					//m_step = -m_lineValue;
				}
				else if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] &&
					my>bt1bbox.m_max[1] && my<barbox.m_min[1] )
				{
					newvalue += m_pageValue;
					//m_step = m_lineValue;
				}
				else
				{//点击在BAR上
					bar->setActiveSwitchSet(2);
				}
			}
			else
			{
				if( mx>bt0bbox.m_max[0] && mx<barbox.m_min[0] &&
					my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					newvalue -= m_pageValue;
					//m_step = -m_lineValue;
				}
				else if( mx>barbox.m_max[0] && mx<bt1bbox.m_min[0] &&
					my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					newvalue += m_pageValue;
					//m_step = m_lineValue;
				}
				else
				{//点击在BAR上
					bar->setActiveSwitchSet(2);
				}
			}
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::MOVE)
		{
			if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] && 
				my>=barbox.m_min[1] && my<=barbox.m_max[1] )
			{
				bar->setActiveSwitchSet(1);
			}
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			m_drag = false;
			setCanCaptureMouse(false);
			m_step = 0;
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::DRAG && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if(m_scrollBarType == VERTICALBAR)
			{
				if(m_drag)
				{
					newvalue += (my - m_lastmouse[1]) * vmax / m_lmax;
					bar->setActiveSwitchSet(2);
				}
				if(	my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					m_drag = true;
					m_lastmouse.set(mx,my);
					setCanCaptureMouse(true);
				}
			}
			else
			{
				if(m_drag)
				{
					newvalue += (mx - m_lastmouse[0]) * vmax / m_lmax;
					bar->setActiveSwitchSet(2);
				}
				if(	mx>=barbox.m_min[0] && mx<=barbox.m_max[0] )
				{
					m_drag = true;
					m_lastmouse.set(mx,my);
					setCanCaptureMouse(true);
				}
			}
		}
	} while (0);
	//newvalue = CRCore::clampTo(newvalue,m_range[0],m_range[1]);
	//m_value = newvalue;
	setValue(newvalue);
	float a = m_lmax / vmax;
	float y = a * m_value;
	//计算 bar pos
	if(m_scrollBarType == HORIZONTALBAR)
	{
		m_scrollBar->setPosition(crVector3(y,0.0f,0.0f));
	}
	else
	{
		m_scrollBar->setPosition(crVector3(0.0f,y,0.0f));
	}
}
void crScrollBarWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Page_Down:
		setValue(m_value + m_pageValue);
		break;
	case crGUIEventAdapter::KEY_Page_Up:
		setValue(m_value - m_pageValue);
		break;
	}
}
void crScrollBarWidgetNode::kernelLostFocus()
{
	m_button0->setActiveSwitchSet(0);
	m_button1->setActiveSwitchSet(0);
	crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_scrollBar->getChild(0));
	bar->setActiveSwitchSet(0);
}
void crScrollBarWidgetNode::kernelLostCapture()
{
	m_button0->setActiveSwitchSet(0);
	m_button1->setActiveSwitchSet(0);
	crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_scrollBar->getChild(0));
	bar->setActiveSwitchSet(0);
}
void crScrollBarWidgetNode::initWindow()
{
	NodeArray &childArray = getChildArray();
	for( NodeArray::iterator itr = childArray.begin();
		 itr != childArray.end();
		 ++itr )
	{
		if(!m_button0.valid() && (*itr)->getName().compare("button0") == 0)
			m_button0 = dynamic_cast<crMultiSwitch *>(itr->get());
		else if(!m_button1.valid() && (*itr)->getName().compare("button1") == 0)
			m_button1 = dynamic_cast<crMultiSwitch *>(itr->get());
		else if(!m_scrollBar.valid() && (*itr)->getName().compare("Bar") == 0)
			m_scrollBar = dynamic_cast<crMatrixTransform *>(itr->get());
	}
	if(!m_button0||!m_button1||!m_scrollBar)
	{
		std::string str = "ScrollBarWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	setCanCaptureMouse(true);
	setRange(m_range);
	//float vmax = m_range[1] - m_range[0];
	//float a = m_lmax / vmax;
	//float y = a * m_value;
	//计算 bar pos
	//if(m_scrollBarType == HORIZONTALBAR)
	//{
	//	m_scrollBar->setPosition(crVector3(y,0.0f,0.0f));
	//}
	//else
	//{
	//	m_scrollBar->setPosition(crVector3(0.0f,y,0.0f));
	//}
	//if(m_scrollBarType == HORIZONTALBAR)
	//{
	//	m_lmax = childArray[0]->getBoundBox().xLength() - m_scrollBar->getBoundBox().xLength();
	//}
	//else
	//{
	//	m_lmax = -childArray[0]->getBoundBox().yLength() - m_scrollBar->getBoundBox().yLength();
	//}
	//m_range.set(0,10);
	//m_lineValue = 1;
	//m_pageValue = 2;
	//if(m_scrollBarType == HORIZONTALBAR)
	//	m_scrollBar->setMatrix(crMatrix::scale(2.0f,1.0f,1.0f));
	//else
	//	m_scrollBar->setMatrix(crMatrix::scale(1.0f,2.0f,1.0f));
}
void crScrollBarWidgetNode::updateData()
{
	if(!m_enable || m_range[1] == m_range[0])
	{
		m_value = 0;
		m_scrollBar->setVisiable(false);
	}
	else
	{
		m_scrollBar->setVisiable(true);
		//const crBoundingBox &bt0bbox = m_button0->getBoundBox();
		//const crBoundingBox &bt1bbox = m_button1->getBoundBox();
		//const crBoundingBox &barbox = m_scrollBar->getBoundBox();
		//float vmax = m_range[1] - m_range[0];
		//float lmax;
		//if(m_scrollBarType == HORIZONTALBAR)
		//{
		//	lmax = bt1bbox.m_max[0] - bt0bbox.m_min[0] - barbox.xLength();
		//}
		//else
		//{
		//	lmax = bt1bbox.m_max[1] - bt0bbox.m_min[1] + barbox.yLength();
		//}
		if (m_step!=0)
		{
			setValue(m_value + m_step);
		}

		float vmax = m_range[1] - m_range[0];
		float a = m_lmax / vmax;
		float y = a * m_value;
		//计算 bar pos
		if(m_scrollBarType == HORIZONTALBAR)
		{
			m_scrollBar->setPosition(crVector3(y,0.0f,0.0f));
		}
		else
		{
			m_scrollBar->setPosition(crVector3(0.0f,y,0.0f));
		}
	}
}

void crScrollBarWidgetNode::setScrollBarType(ScrollBarType bar)
{
	m_scrollBarType = bar;
}

crScrollBarWidgetNode::ScrollBarType crScrollBarWidgetNode::getScrollBarType()
{
	return m_scrollBarType;
}

void crScrollBarWidgetNode::setRange(const CRCore::crVector2f &range)
{
	m_range = range;
	if(m_range[1]<m_range[0])
	{
		float temp = m_range[0];
		m_range[0] = m_range[1];
		m_range[1] = temp;
	}

	if(m_value<m_range[0]) m_value = m_range[0];
	else if(m_value>m_range[1]) m_value = m_range[1];
	if(m_button0.valid() && m_button1.valid() && m_scrollBar.valid())
	{
		//if(m_scrollBarType == HORIZONTALBAR)
		//	m_scrollBar->setMatrix(crMatrix::scale(2.0f,1.0f,1.0f));
		//else
		//	m_scrollBar->setMatrix(crMatrix::scale(1.0f,2.0f,1.0f));

		const crBoundingBox &bt0bbox = m_button0->getBoundBox();
		const crBoundingBox &bt1bbox = m_button1->getBoundBox();
		const crBoundingBox &barbox = m_scrollBar->getBoundBox();
		float vmax = m_range[1] - m_range[0];
		if(m_scrollBarType == HORIZONTALBAR)
		{
			m_lmax = bt1bbox.m_min[0] - bt0bbox.m_max[0] - barbox.xLength();
		}
		else
		{
			m_lmax = bt1bbox.m_max[1] - bt0bbox.m_min[1] + barbox.yLength();
		}
	}
}

void crScrollBarWidgetNode::setRange(float min, float max)
{
	setRange(crVector2f(min,max));
}

const CRCore::crVector2f &crScrollBarWidgetNode::getRange()
{
	return m_range;
}

void crScrollBarWidgetNode::setValue(float value)
{
	value = CRCore::clampTo(value,m_range[0],m_range[1]);
	if(m_value != value)
	{
		//////////////////test
		//float value2 = value;
		//if(value > m_value)
		//{
		//	value2 = (int)(ceil(value /m_lineValue)) * m_lineValue;
		//}
		//else
		//{

		//	value2 = (int)(value /m_lineValue) * m_lineValue;
		//}
		//////////////////testend
		m_value = value;
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this);
		}
		doEvent(WCH_UI_ValueChange);
	}
}
void crScrollBarWidgetNode::outerSetValue(float value)
{
	value = CRCore::clampTo(value,m_range[0],m_range[1]);
	if(m_value != value)
	{
		////////////////test
		//float value2 = value;
		//if(value > m_value)
		//{
		//	value2 = (int)(ceil(value /m_lineValue)) * m_lineValue;
		//}
		//else
		//{

		//	value2 = (int)(m_value /m_lineValue) * m_lineValue;
		//}
		////////////////testend
		m_value = value;
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this);
		}
	}
}
float crScrollBarWidgetNode::getValue()
{
	////////////////test
	float value2 = m_value;
	if(m_lineValue > 0)
	{
		value2 = (int)(m_value / m_lineValue + 0.5f) * m_lineValue;
	}
	/////////////////testend
	return value2;
}

void crScrollBarWidgetNode::setLineValue(float lineValue)
{
	m_lineValue = lineValue;
}

float crScrollBarWidgetNode::getLineValue()
{
	return m_lineValue;
}

void crScrollBarWidgetNode::setPageValue(float pageValue)
{
	m_pageValue = pageValue;
}

float crScrollBarWidgetNode::getPageValue()
{
	return m_pageValue;
}
//////////////////////////////////////////////////////////////////////////
//
//crTextAttrWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crTextAttrWidgetNode::crTextAttrWidgetNode():
m_drawMode(crText::TEXT),
m_lineSpacing(3.0f),
m_underLineSpacing(3.0f),
m_uplineSpacing(1.0f),
m_characterSize(16),
m_fontSize(32,32),
m_font("simhei.ttf"),
m_alignment(crText::BASE_LINE),
m_scrollDirty(false),
m_scrollChanged(false),
m_backdropType(crText::NONE),
m_backdropColor(0.0f, 0.0f, 0.0f, 1.0f),
m_textDirty(false),
m_defaultScrollPos(0),
m_needScissor(true)
{
	m_backdropOffset.set(0.075f,0.075f);
	m_color.set(0.0f,0.0f,0.0f,1.0f);
}
crTextAttrWidgetNode::crTextAttrWidgetNode(const crTextAttrWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_drawMode(node.m_drawMode),
m_lineSpacing(node.m_lineSpacing),
m_underLineSpacing(node.m_underLineSpacing),
m_uplineSpacing(node.m_uplineSpacing),
m_characterSize(node.m_characterSize),
m_fontSize(node.m_fontSize),
m_font(node.m_font),
m_color(node.m_color),
m_alignment(node.m_alignment),
m_rect(node.m_rect),
m_rectOffset(node.m_rectOffset),
m_vScrollBarName(node.m_vScrollBarName),
m_scrollDirty(node.m_scrollDirty),
m_scrollChanged(false),
m_backdropType(node.m_backdropType),
m_backdropOffset(node.m_backdropOffset),
m_backdropColor(node.m_backdropColor),
m_defaultScrollPos(node.m_defaultScrollPos),
m_needScissor(node.m_needScissor)
{
	dirtyText();
}
crTextAttrWidgetNode::~crTextAttrWidgetNode()
{

}
bool crTextAttrWidgetNode::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_bbox.expandBy(getChild(0)->getBoundBox());

	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_boundSphere.expandBy(m_bbox);
	m_bBoundSphere_computed = true;
	//computeRect();
	return true;
}
void crTextAttrWidgetNode::setNeedScissor(bool need)
{
	m_needScissor = need;
}
bool crTextAttrWidgetNode::getNeedScissor()
{
	return m_needScissor;
}
void crTextAttrWidgetNode::initWindow()
{
	if(m_parentCanvas && !m_vScrollBarName.empty())
	{
		m_vScrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_parentCanvas->getWidget(m_vScrollBarName));
		if(m_vScrollBar.valid())
			m_vScrollBar->setParentWidget(this);	
	}
	//
	//crVector3 minpos = getBoundBox().m_min;
	//crVector3 maxpos = getBoundBox().m_max;
	//minpos[0] += m_rectOffset[0];
	//minpos[1] += m_rectOffset[1];
	//maxpos[0] -= m_rectOffset[2];
	//maxpos[1] -= m_rectOffset[3];
	//minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	//maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	//m_rect.set(minpos[0],maxpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);
	computeRect();

	m_mvpwNode = new crMatrixTransform;
	m_mvpwNode->setName("InverseMVPW");
	m_mvpwNode->addDescription("#ScriptAddNode");
	m_mvpwNode->setMatrix(crFilterRenderManager::getInstance()->getInverseMVPW());
	addChild(m_mvpwNode.get());

	m_textObject = new crObject;
	m_textObject->setName("TextObject");
	m_mvpwNode->addChild(m_textObject.get());

	//m_text = new crText;
	//m_text->setBackdropType(crText::OUTLINE);
	//m_text->setFont(m_font);
	//m_text->setColor(m_color);
	//m_text->setFontResolution((unsigned int)m_characterSize,(unsigned int)m_characterSize);
	//m_text->setCharacterSize(m_characterSize);
	//m_text->setLineSpacing(0.0f);
	//m_text->setAlignment((crText::AlignmentType)m_alignment);
	//m_text->setMaximumWidth(m_rect[2]);
	//m_text->setMaximumHeight(m_rect[3]);
	//m_text->setPosition(crVector3(m_rect[0],m_rect[1] - m_characterSize,0.0f));
	//m_text->setVisiable(false);
	//textObject->addDrawable(m_text.get());

	if(m_vScrollBar.valid())
	{
		//m_vScrollBar->setRange(0,100);
		m_vScrollBar->setLineValue(getLineHeight());
		m_vScrollBar->setPageValue(m_rect[3]);
		m_scrollDirty = true;
	}
	////
	if(m_needScissor)
	{
		m_scissor = new crScissor;
		crStateSet *ss = new crStateSet;
		m_textObject->setStateSet(ss);
		ss->setAttributeAndModes(m_scissor.get(),crStateAttribute::ON);
		computeScissor();
	}
}
void crTextAttrWidgetNode::resize()
{
	computeRect();
	m_mvpwNode->setMatrix(crFilterRenderManager::getInstance()->getInverseMVPW());
	if (m_vScrollBar.valid())
	{
		m_vScrollBar->setLineValue(getLineHeight());
		m_vScrollBar->setPageValue(m_rect[3]);
		m_scrollDirty = true;
	}
	if (m_needScissor)
	{
		computeScissor();
	}
}
void crTextAttrWidgetNode::computeRect()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos[0] += m_rectOffset[0];
	minpos[1] += m_rectOffset[1];
	maxpos[0] -= m_rectOffset[2];
	maxpos[1] -= m_rectOffset[3];
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0],maxpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);
}
void crTextAttrWidgetNode::updateParentWidget(crWidgetNode *widget,_crInt32 msg)
{
	if(m_vScrollBar.get() == widget)
	{
		m_scrollChanged = true;
	}
}
void crTextAttrWidgetNode::setVisiable( bool visiable )
{
	crWidgetNode::setVisiable(visiable);
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setVisiable(visiable);
	}
}
void crTextAttrWidgetNode::computeScissor()
{
	if(m_scissor.valid())
	{
		crBoundingBox bbox = getBoundBox();
		//crMatrix matrix;
		//if(this->getEnableMatrix())
		//{
		//	matrix = this->getMatrix();
		//}
		//matrix.preMult(getParentCanvas()->getMatrix());
		//bbox.m_min = bbox.m_min * matrix;
		//bbox.m_max = bbox.m_max * matrix;
		//bbox.correct();
		CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
		accept(parentMatrix);
		bbox.m_max = bbox.m_max * parentMatrix.getResult();
		bbox.m_min = bbox.m_min * parentMatrix.getResult();
		bbox.correct();

		crVector3 minpos = bbox.m_min;
		minpos[0] += m_rectOffset[0];
		minpos[1] += m_rectOffset[1];
		minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
		m_scissor->setScissor(minpos[0],minpos[1],m_rect[2],m_rect[3]);
	}
}
void crTextAttrWidgetNode::setDrawMode(unsigned int mode)
{
	if(m_drawMode != mode)
	{
		m_drawMode = mode;
		dirtyTextAttr();
	}
}
unsigned int crTextAttrWidgetNode::getDrawMode()
{
	return m_drawMode;
}
void crTextAttrWidgetNode::setLineSpacing(float lineSpacing)
{
	lineSpacing = CRCore::maximum(3.0f,lineSpacing);
	if(m_lineSpacing != lineSpacing)
	{
		m_lineSpacing = lineSpacing;
		dirtyTextAttr();
	}
}
void crTextAttrWidgetNode::setUpLineSpacing(float uplineSpacing)
{
	if(m_uplineSpacing != uplineSpacing)
	{
		m_uplineSpacing = uplineSpacing;
		dirtyTextAttr();
	}
}
float crTextAttrWidgetNode::getLineSpacing()
{
	return m_lineSpacing * crDisplaySettings::instance()->getUIViewScale()[1];
}
float crTextAttrWidgetNode::getUpLineSpacing()
{
	return m_uplineSpacing * crDisplaySettings::instance()->getUIViewScale()[1];
}
void crTextAttrWidgetNode::setUnderLineSpacing(float underLineSpacing)
{
	if(m_underLineSpacing != underLineSpacing)
	{
		m_underLineSpacing = underLineSpacing;
		dirtyTextAttr();
	}
}
float crTextAttrWidgetNode::getUnderLineSpacing()
{
	return m_underLineSpacing * crDisplaySettings::instance()->getUIViewScale()[1];
}
void crTextAttrWidgetNode::setCharacterSize(float characterSize)
{
	//characterSize = CRCore::maximum(14.0f,characterSize);
	if(m_characterSize != characterSize)
	{
		m_characterSize = characterSize;
		m_fontSize = crVector2s(characterSize*2,characterSize*2);
		//setFontSize(crVector2s(characterSize*2,characterSize*2));
		dirtyTextAttr();
	}
}
float crTextAttrWidgetNode::getCharacterSize()
{
	return m_characterSize * crDisplaySettings::instance()->getUIViewScale()[1];
}
void crTextAttrWidgetNode::setFontSize(const crVector2s& fontSize)
{
	//if(m_fontSize != fontSize)
	//{
	//	m_fontSize = fontSize;
	//	dirtyTextAttr();
	//}
}
const crVector2s& crTextAttrWidgetNode::getFontSize()
{
	return m_fontSize;
}
void crTextAttrWidgetNode::setFont(const std::string &font)
{
	//m_font = "msyh.ttf";
	if(m_font != font)
	{
		m_font = font;
		dirtyTextAttr();
	}
}
const std::string &crTextAttrWidgetNode::getFont()const
{
	return m_font;
}
void crTextAttrWidgetNode::setColor(const CRCore::crVector4 &color)
{
	//m_backdropType = crText::OUTLINE;
	//m_backdropColor = color;
	//m_backdropOffset.set(0.0f,0.0f);
	if(m_color != color)
	{
		m_color = color;
		dirtyTextAttr();
	}
}
const crVector4 &crTextAttrWidgetNode::getColor()
{
	return m_color;
}
void crTextAttrWidgetNode::setAlignment(unsigned char alignment)
{
	if(m_alignment != alignment)
	{
		m_alignment = alignment;
		dirtyTextAttr();
	}
}
unsigned char crTextAttrWidgetNode::getAlignment()
{
	return m_alignment;
}
void crTextAttrWidgetNode::setBackdropType(unsigned char backdroptype)
{
	if(m_backdropType != backdroptype)
	{
		m_backdropType = backdroptype;
		dirtyTextAttr();
	}
}
unsigned char crTextAttrWidgetNode::getBackdropType()
{
	return m_backdropType;
}
void crTextAttrWidgetNode::setBackdropOffset(const CRCore::crVector2& offset)
{
	//if(m_backdropOffset != offset)
	//{
	//	m_backdropOffset = offset;
	//	dirtyTextAttr();
	//}
}
const CRCore::crVector2& crTextAttrWidgetNode::getBackdropOffset()
{
	return m_backdropOffset;
}
void crTextAttrWidgetNode::setBackdropColor(const CRCore::crVector4 &color)
{
	if(m_backdropColor != color)
	{
		m_backdropColor = color;
		dirtyTextAttr();
	}
}
const CRCore::crVector4 &crTextAttrWidgetNode::getBackdropColor()
{
	return m_backdropColor;
}
const CRCore::crVector4f &crTextAttrWidgetNode::getRect() const
{
	return m_rect;
}
void crTextAttrWidgetNode::setRectOffset(const CRCore::crVector4f &offset)
{
	float pixelRatio = crDisplaySettings::instance()->getUIPixelMeter();
	m_rectOffset = offset * pixelRatio;
}
const CRCore::crVector4f &crTextAttrWidgetNode::getRectOffset() const
{
	return m_rectOffset;
}
void crTextAttrWidgetNode::setVScrollBarName(const std::string &name)
{
	m_vScrollBarName = name;
}
const std::string &crTextAttrWidgetNode::getVScrollBarName()const
{
	return m_vScrollBarName;
}
void crTextAttrWidgetNode::parseText(std::string &text)
{
	for( std::string::iterator itr = text.begin();
		itr != text.end();
		++itr )
	{
		if(*itr == '\\' || *itr == '/')
		{
			std::string::iterator itr_flag = itr;
			if(++itr_flag != text.end()/* && *itr_flag == 'n'*/)
			{
				if(*itr_flag == 'n')
				{
					text.erase(itr);
					*itr = '\n';
				}
				else if(*itr_flag == 'r')
				{
					text.erase(itr);
					*itr = '\r';
				}
				else if(*itr_flag == 't')
				{
					text.erase(itr);
					*itr = '\t';
				}
			}
			//itr = text.erase(itr);
			//if(*itr == 'n')
			//{
			//	*itr = '\n';
			//}
		}
	}
}
void crTextAttrWidgetNode::transformText(std::string &text)
{
	for (std::string::iterator itr_str = text.begin();itr_str!=text.end();itr_str++)
	{
		if(*itr_str == '\n')
		{
			*itr_str = '/';
			itr_str++;
			itr_str = text.insert(itr_str,'n');
		}
		else if(*itr_str == '\r')
		{
			*itr_str = '/';
			itr_str++;
			itr_str = text.insert(itr_str,'r');
		}
		else if(*itr_str == '\t')
		{
			*itr_str = '/';
			itr_str++;
			itr_str = text.insert(itr_str,'t');
		}
	}
}
crObject *crTextAttrWidgetNode::getTextObject()
{
	return m_textObject.get();
}
CRCore::crMatrixTransform *crTextAttrWidgetNode::getTextTransform()
{
	return m_mvpwNode.get();
}
float crTextAttrWidgetNode::getLineHeight()
{
	return getCharacterSize()+getLineSpacing();
}
float crTextAttrWidgetNode::getUpLineHeight()
{
	return getCharacterSize()+getUpLineSpacing();
}
void crTextAttrWidgetNode::setDefaultScrollPos(char pos)
{
	m_defaultScrollPos = pos;
}
char crTextAttrWidgetNode::getDefaultScrollPos()
{
	return m_defaultScrollPos;
}
crScrollBarWidgetNode *crTextAttrWidgetNode::getVScrollBar()
{
	return m_vScrollBar.get();
}
void crTextAttrWidgetNode::scrollChanged()
{
	if(m_vScrollBar.get())
		m_scrollChanged = true;
}
void crTextAttrWidgetNode::setVScrollBar(crScrollBarWidgetNode* scrollbar)
{
	m_vScrollBar = scrollbar;
}
//////////////////////////////////////////////////////////////////////////
//
//crStaticTextWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crStaticTextWidgetNode::crStaticTextWidgetNode():
m_startSelectText(-1),
m_canSelectText(false),
m_password(false),
m_textAttrDirty(false)
{
}
crStaticTextWidgetNode::crStaticTextWidgetNode(const crStaticTextWidgetNode& node,const crCopyOp& copyop):
crTextAttrWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_wstring(node.m_wstring),
m_startSelectText(-1),
m_canSelectText(node.m_canSelectText),
m_password(node.m_password)
{
	dirtyTextAttr();
}
crStaticTextWidgetNode::~crStaticTextWidgetNode()
{
}
void crStaticTextWidgetNode::setPassword(bool password)
{
	m_password = password;
}
bool crStaticTextWidgetNode::getPassword()
{
	return m_password;
}
void crStaticTextWidgetNode::kernelMouseEvent(float mx, float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	if(_ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		if(m_canSelectText)
		{
			if(_ea->getEventType() == crGUIEventAdapter::DRAG)
			{
				crCanvasNode *canvas = getParentCanvas();
				canvas->mouseTrans(mx,my);
				crVector3 mouse(mx,my,0.0f);
				mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
				crVector3 textPos = m_text->getPosition();
				int height = (m_vScrollBar.valid()?m_vScrollBar->getValue():0) - getUpLineHeight();
				crVector2 cursorPos(mouse[0] - textPos[0], mouse[1] + height - textPos[1]);
				int focusPos = m_text->getCursorIndex(cursorPos);
				if(m_startSelectText == -1)
				{
					m_startSelectText = focusPos;
				}
				else
				{
					selectText(m_startSelectText,focusPos);
				}
				setFocusPos(focusPos);
			}
			else if (_ea->getEventType() == crGUIEventAdapter::RELEASE)
			{
				m_startSelectText = -1;
			}
			else if (_ea->getEventType() == crGUIEventAdapter::PUSH)
			{
				selectText(0,0);
			}
		}

		if (_ea->getEventType() == crGUIEventAdapter::RELEASE)
		{
			if(m_parentWidget && typeid(*this)==typeid(crStaticTextWidgetNode))
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_LBtnUp);
			}
		}
		if (_ea->getEventType() == crGUIEventAdapter::PUSH)
		{
			if(m_parentWidget && typeid(*this)==typeid(crStaticTextWidgetNode))
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_LBtnDown);
			}
		}
	}
}
void crStaticTextWidgetNode::initWindow()
{
	//setAlignment(crText::CENTER_BASE_LINE);
	//m_rectOffset.set(0,0,0,0);
	//setCharacterSize(16);
	//setFont("simhei.ttf");
	//m_color.set(1.0f,1.0f,1.0f,1.0f);
	//setBackdropType(crText::OUTLINE);
	crTextAttrWidgetNode::initWindow();

	m_text = new crText;
	m_text->setFont(m_font);
	m_text->setColor(m_color);
	m_text->setFontResolution(m_fontSize[0],m_fontSize[1]);
	m_text->setCharacterSize(getCharacterSize());
	m_text->setLineSpacing(getLineSpacing());
	m_text->setAlignment((crText::AlignmentType)m_alignment);
	m_text->setMaximumWidth(m_rect[2]);
	//m_text->setMaximumHeight(m_rect[3]);
	m_text->setBackdropType((crText::BackdropType)m_backdropType);
	m_text->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
	m_text->setBackdropColor(m_backdropColor);
	m_text->setPosition(crVector3(m_rect[0],m_rect[1] - getUpLineHeight(),0.0f));
	m_text->setVisiable(false);
	m_textObject->addDrawable(m_text.get());

	if(m_canSelectText)
	{
		m_seltext = new crText;
		m_seltext->setFont(m_font);
		m_seltext->setColor(CRCore::crVector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_seltext->setDrawMode(crText::TEXT|crText::FILLEDBOUNDINGBOX);
		m_seltext->setBoundingBoxColor(CRCore::crVector4(0.0f, 0.0f, 1.0f, 0.8f));
		m_seltext->setFontResolution(m_fontSize[0],m_fontSize[1]);
		m_seltext->setCharacterSize(getCharacterSize());
		m_seltext->setAlignment((crText::AlignmentType)m_alignment);
		m_seltext->setMaximumWidth(m_rect[2]);
		//m_seltext->setMaximumHeight(m_rect[3]);
		m_seltext->setPosition(crVector3(m_rect[0],m_rect[1] - getUpLineHeight(),0.0f));
		m_seltext->setVisiable(false);
		m_textObject->addDrawable(m_seltext.get());
	}
	m_textAttrDirty = false;
	///测试数据
	//addString("1将所有的模型转成patch");
	//addString("2将所有的模型转成patch");
	//selectText(0,10);
	/////////////////
}
void crStaticTextWidgetNode::resize()
{
	crTextAttrWidgetNode::resize();
	if (m_text.valid())
	{
		m_text->setFontResolution(m_fontSize[0], m_fontSize[1]);
		m_text->setCharacterSize(getCharacterSize());
		m_text->setLineSpacing(getLineSpacing());
		m_text->setAlignment((crText::AlignmentType)m_alignment);
		m_text->setMaximumWidth(m_rect[2]);
		//m_text->setMaximumHeight(m_rect[3]);
		m_text->setBackdropType((crText::BackdropType)m_backdropType);
		m_text->setBackdropOffset(m_backdropOffset[0], m_backdropOffset[1]);
		m_text->setBackdropColor(m_backdropColor);
		m_text->setPosition(crVector3(m_rect[0], m_rect[1] - getUpLineHeight(), 0.0f));
	}
	if (m_seltext.valid())
	{
		m_seltext->setFontResolution(m_fontSize[0], m_fontSize[1]);
		m_seltext->setCharacterSize(getCharacterSize());
		m_seltext->setAlignment((crText::AlignmentType)m_alignment);
		m_seltext->setMaximumWidth(m_rect[2]);
		//m_seltext->setMaximumHeight(m_rect[3]);
		m_seltext->setPosition(crVector3(m_rect[0], m_rect[1] - getUpLineHeight(), 0.0f));
	}
}
void crStaticTextWidgetNode::updateData()
{
	if(m_scrollChanged)
	{
		int height = (m_vScrollBar.valid()?m_vScrollBar->getValue():0) - getUpLineHeight();
		m_text->setPosition(crVector3(m_rect[0],m_rect[1] + height,0.0f));
		if(m_canSelectText) m_seltext->setPosition(crVector3(m_rect[0],m_rect[1] + height,0.0f));
		m_scrollChanged = false;
	}
	if(m_textAttrDirty)
	{
		m_text->setFont(m_font);
		m_text->setColor(m_color);
		m_text->setFontResolution(m_fontSize[0],m_fontSize[1]);
		m_text->setCharacterSize(getCharacterSize());
		m_text->setLineSpacing(getLineSpacing());
		m_text->setAlignment((crText::AlignmentType)m_alignment);
		m_text->setMaximumWidth(m_rect[2]);
		//m_text->setMaximumHeight(m_rect[3]);
		m_text->setBackdropType((crText::BackdropType)m_backdropType);
		m_text->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
		m_text->setBackdropColor(m_backdropColor);
		m_text->setPosition(crVector3(m_rect[0],m_rect[1] - getUpLineHeight(),0.0f));
		m_textAttrDirty = false;
	}
	if(m_scrollDirty)
	{
		if(m_vScrollBar.valid())
		{
			if(m_text->getVisiable())
			{
				const crBoundingBox &bbox = m_text->getBoundBox();
				float len = bbox.yLength() - m_rect[3] + getCharacterSize();
				m_vScrollBar->setRange(0.0f,len>0.0f?len:0.0f);
				m_vScrollBar->setEnable(true);
				if(m_defaultScrollPos>=0 && m_defaultScrollPos<=1)
					m_vScrollBar->setValue(m_vScrollBar->getRange()[m_defaultScrollPos]);
			}
			else
			{
				m_vScrollBar->setEnable(false);
			}
			m_scrollChanged = true;
		}
		m_scrollDirty = false;
	}
	if(m_textDirty)
	{
		if(m_password)
		{
			std::wstring password = m_wstring;
			for(int i = 0; i<password.size(); ++i)
			{
				if((password)[i] != L'\n') (password)[i] = L'*';
			}
			m_text->setText(password.c_str());
		}
		else
			m_text->setText(m_wstring.c_str()/*CRIOManager::convertUTF8toUTF16(m_wstring).c_str()*/);
		m_text->setVisiable(!m_wstring.empty());
		m_textDirty = false;
		m_scrollDirty = true;
	}
	if(m_text->getVisiable())
	{
		if(m_canSelectText && m_seltext.valid())
		{
			if(m_textSelect != crVector2i(0,0))
			{
				m_textSelect[0] = CRCore::clampTo(m_textSelect[0],0,(int)m_wstring.size());
				m_textSelect[1] = CRCore::clampTo(m_textSelect[1],0,(int)m_wstring.size());
				if(m_textSelect[0] == m_textSelect[1])
				{
					m_textSelect.set(0,0);
					m_seltext->setVisiable(false);
				}
				else
				{
					std::wstring selstr = m_wstring.substr(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
					if(m_password)
					{
						for(int i = 0; i<selstr.size(); ++i)
						{
							if((selstr)[i] != L'\n') (selstr)[i] = L'*';
						}
					}
					m_seltext->setText(selstr.c_str());
					crVector2 cursorPos;
					m_text->getCursorPosition(m_textSelect[0],cursorPos);
					//crVector3 textPos = m_text->getPosition();
					//textPos[0] += cursorPos[0];
					//textPos[1] += cursorPos[1];
					m_seltext->setStartCoord(cursorPos);
					m_seltext->setVisiable(true);
				}
			}
			else
			{
				m_seltext->setVisiable(false);
			}
		}
		if(m_scissor.valid())
		{
			computeScissor();
		}
	}
	else if(m_canSelectText)
	{
		m_seltext->setVisiable(false);
	}
}
void crStaticTextWidgetNode::setString(std::string &str)
{
	parseText(str);
	setString(CRIOManager::convertUTF8toUTF16(str));
}
void crStaticTextWidgetNode::setString(const std::wstring &wstr)
{
	if(m_wstring.compare(wstr) != 0)
	{
		m_wstring = wstr;
		dirtyText();
	}
}
void crStaticTextWidgetNode::addString(std::string &str)
{
	parseText(str);
	std::wstring wstr = CRIOManager::convertUTF8toUTF16(str);
	addString(wstr);
}
void crStaticTextWidgetNode::addString(const std::wstring &wstr)
{
	m_wstring.append(wstr.c_str(),wstr.length());
	dirtyText();
}
const std::wstring& crStaticTextWidgetNode::getString()const
{
	return m_wstring;
}
std::string crStaticTextWidgetNode::getUTF8String()
{
	std::string str = CRIOManager::convertUTF16toUTF8(m_wstring);
	return str;
}
void crStaticTextWidgetNode::clearString()
{
	m_wstring.clear();
	dirtyText();
}
void crStaticTextWidgetNode::setCanSelectText(bool canSelectText)
{
	m_canSelectText = canSelectText;
}
bool crStaticTextWidgetNode::getCanSelectText()
{
	return m_canSelectText;
}
void crStaticTextWidgetNode::selectText(int from, int to)
{
	if(from<to)
		m_textSelect.set(from,to);
	else if(to<from)
		m_textSelect.set(to,from);
	else
		m_textSelect.set(0,0);
}
void crStaticTextWidgetNode::selectAllText()
{
	selectText(0,m_wstring.size());
}
std::wstring crStaticTextWidgetNode::getSelectString()
{
	std::wstring str;
	if(!m_wstring.empty() && m_textSelect[1]-m_textSelect[0]>0)
	{
		str = m_wstring.substr(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////
//
//crHypertextWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crHypertextWidgetNode::crHypertextWidgetNode():
m_validTextSize(0), m_validImageSize(0),m_validModelSize(0),m_bSingleLine(false)
{
}
crHypertextWidgetNode::crHypertextWidgetNode(const crHypertextWidgetNode& node,const crCopyOp& copyop):
crTextAttrWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_validTextSize(0), m_validImageSize(0),m_validModelSize(0),m_bSingleLine(false)
{
}
crHypertextWidgetNode::~crHypertextWidgetNode()
{
}
void crHypertextWidgetNode::kernelMouseEvent(float mx, float my,void *ea)
{
	// mouse left button down
	// mouse move
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	_crInt32 msg = 0;
	if(_ea->getEventType() == crGUIEventAdapter::PUSH && _ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		msg = WCH_UI_LBtnDown;
	else if(_ea->getEventType() == crGUIEventAdapter::MOVE)
		msg = WCH_UI_MouseMove;
	if(!msg) return;

	// mouse trace 
	CRText::crText *hit_text = NULL;
	crCanvasNode *parent_canvas = getParentCanvas();
	parent_canvas->mouseTrans(mx,my);
	crVector3 mouse(mx,my,0.0f);
	mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
	crVector3 text_pos;
	crVector2 cursor_pos;
	std::wstring *wstr = NULL;

	// find mouse traced text
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	crText *text;
	for (int i = 0; i < m_validTextSize; ++i)
	{
		text = m_textArray[i].first.get();
		if (text)
		{
			text_pos = text->getPosition();
			cursor_pos.set(mouse[0] - text_pos[0], mouse[1] - text_pos[1]);
			if (text->hitTest(cursor_pos))
			{
				hit_text = text;
				wstr = &(m_textArray[i].second);
				break;
			}
		}

	}

	// check link
	if(hit_text)
	{
		LinkInfoPair linkInfoPair;
		LinkMap::iterator itr = m_linkMap.find(hit_text);
		if(itr != m_linkMap.end())
		{
			int msgid = itr->second;
			linkInfoPair.first = *wstr;
			linkInfoPair.second = hit_text;
			doEvent(MAKEINT64(msg,msgid),MAKECREPARAM(ea,&linkInfoPair));
		}
	}

	if(m_parentWidget)
	{
		m_parentWidget->updateParentWidget(this,msg);
	}

	if(msg == WCH_UI_LBtnDown && m_lastHitText.get() != hit_text)
	{
		doEvent(MAKEINT64(WCH_UI_LostFocus,NULL),MAKECREPARAM(hit_text,NULL));
		m_lastHitText = hit_text;
	}
}
void crHypertextWidgetNode::initWindow()
{
	crTextAttrWidgetNode::initWindow();
	crVector2 cursor;
	// show text
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	for( TextVec::iterator itr =  m_textArray.begin();
		 itr != m_textArray.end();
		 ++itr )
	{
		if (itr->first.valid())
		{
			if (m_bSingleLine)
			{
				itr->first->setMaximumWidth(0.0f);
			}
			else
			{
				itr->first->setMaximumWidth(m_rect[2]);
			}
			itr->first->setPosition(crVector3(m_rect[0], m_rect[1] - getUpLineHeight(), 0.0f));
			itr->first->setStartCoord(cursor);
			itr->first->swapBuffers();
			cursor = itr->first->getEndCoord();
			itr->first->setVisiable(true);
			m_textObject->addDrawable(itr->first.get());
		}
	}

	m_imageRoot = new crGroup;
	m_mvpwNode->addChild(m_imageRoot.get());
	m_modelRoot = new crGroup;
	m_mvpwNode->addChild(m_modelRoot.get());
	//addChild(m_imageRoot.get());
	m_textDirty = false;
	m_bSingleLine = false;
}
void crHypertextWidgetNode::resize()
{
	crTextAttrWidgetNode::resize();
	crVector2 cursor;
	// show text
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	for (TextVec::iterator itr = m_textArray.begin();
		itr != m_textArray.end();
		++itr)
	{
		if (itr->first.valid())
		{
			if (m_bSingleLine)
			{
				itr->first->setMaximumWidth(0.0f);
			}
			else
			{
				itr->first->setMaximumWidth(m_rect[2]);
			}
			itr->first->setPosition(crVector3(m_rect[0], m_rect[1] - getUpLineHeight(), 0.0f));
			itr->first->setStartCoord(cursor);
			itr->first->swapBuffers();
			cursor = itr->first->getEndCoord();
		}
	}
}
void crHypertextWidgetNode::updateData()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	if(m_scrollChanged)
	{
		int height = (m_vScrollBar.valid()?m_vScrollBar->getValue():0) - getUpLineHeight();
		crVector3 pos(m_rect[0],m_rect[1] + height,0.0f);
		ref_ptr<crText>text;
		if(m_validTextSize>m_textArray.size())
			m_validTextSize = m_textArray.size();
		for (int i = 0; i < m_validTextSize; ++i)
		{
			text = m_textArray[i].first.get();
			if(text.valid()) text->setPosition(pos);
		}
		m_scrollChanged = false;
	}

	if(m_textDirty)
	{
		crVector2 cursor;
		ref_ptr<crText>text;
		int height = (m_vScrollBar.valid()?m_vScrollBar->getValue():0) - getUpLineHeight();
		crVector3 pos(m_rect[0],m_rect[1] + height,0.0f);
		int vimage = 0;
		int vmodel = 0;
		if(m_validTextSize>m_textArray.size())
			m_validTextSize = m_textArray.size();
		for (int i = 0; i < m_validTextSize; ++i)
		{
			text = m_textArray[i].first.get();
			if(!text)
				continue;
			text->setPosition(pos);
			if(m_bSingleLine)
			{
				text->setMaximumWidth(0.0f);
			}
			else
			{
				text->setMaximumWidth(m_rect[2]);
			}
			text->setStartCoord(cursor);
			text->setText(m_textArray[i].second.c_str());
			text->swapBuffers();
			cursor = text->getEndCoord();
			text->setVisiable(true);


			if(vimage >= m_validImageSize) continue;
			ref_ptr<crMatrixTransform> image;
			while(vimage < m_imageNodeVec.size())
			{
				if(m_imageNodeVec[vimage].second != i) break;
				
				// position
				const crBoundingBox& box = m_imageNodeVec[vimage].first->getBoundBox();
				if(cursor[0] + box.xLength() > m_rect[2])
				{
					cursor[0] = 0;
					cursor[1] += getCharacterSize();
				}

				image = m_imageNodeVec[vimage].first.get();
				image->setPosition(crVector3(cursor[0] + getCharacterSize(), m_rect[1] + height + cursor[1], 0.0f));
				m_imageRoot->addChild(image.get());
				image->setVisiable(true);
				cursor[0] += box.xLength();

				vimage++;
			}

			if(vmodel >= m_validModelSize) continue;
			while(vmodel < m_modelNodeVec.size())
			{
				if(m_modelNodeVec[vmodel].second != i) break;

				// position
				const crBoundingBox& box = m_modelNodeVec[vmodel].first->getBoundBox();
				if(cursor[0] + box.xLength() > m_rect[2])
				{
					cursor[0] = 0;
					cursor[1] += getCharacterSize();
				}

				image = m_modelNodeVec[vmodel].first.get();
				image->setPosition(crVector3(cursor[0] + getCharacterSize(), m_rect[1] + height + cursor[1], 0.0f));
				m_modelRoot->addChild(image.get());
				image->setVisiable(true);
				cursor[0] += box.xLength();

				vmodel++;
			}
		}
		m_textObject->removeDrawable(0,m_textObject->getNumDrawables());
		for(int i = 0; i<m_validTextSize; ++i)
		{
			m_textObject->addDrawable(m_textArray[i].first.get());
		}
		m_imageRoot->removeChild(0,m_imageRoot->getNumChildren());
		for(int i = 0; i<m_validImageSize; ++i)
		{
			m_imageRoot->addChild(m_imageNodeVec[i].first.get());
		}
		m_modelRoot->removeChild(0,m_modelRoot->getNumChildren());
		for(int i = 0; i<m_validModelSize; ++i)
		{
			m_modelRoot->addChild(m_modelNodeVec[i].first.get());
		}
		//int childCount = m_textObject->getNumDrawables();
		//if(m_validTextSize < childCount)
		//{
		//	m_textObject->removeDrawable(m_validTextSize,childCount - m_validTextSize);
		//}
		//else if(m_validTextSize>childCount)
		//{
		//	for (int i = childCount; i < m_validTextSize; ++i)
		//	{
		//		text = m_textArray[i].first.get();
		//		if(text) m_textObject->addDrawable(text);
		//	}
		//}

		//childCount = m_imageRoot->getNumChildren();
		//if(m_validImageSize < childCount)
		//{
		//	m_imageRoot->removeChild(m_validImageSize, childCount - m_validImageSize);
		//}
		//else if(m_validImageSize > childCount)
		//{
		//	for (int i = childCount; i < m_validImageSize; ++i)
		//	{
		//		m_imageRoot->addChild(m_imageNodeVec[i].first.get());
		//	}
		//}
		//childCount = m_modelRoot->getNumChildren();
		//if(m_validModelSize < childCount)
		//{
		//	m_modelRoot->removeChild(m_validModelSize, childCount - m_validModelSize);
		//}
		//else if(m_validModelSize > childCount)
		//{
		//	for (int i = childCount; i < m_validModelSize; ++i)
		//	{
		//		m_modelRoot->addChild(m_modelNodeVec[i].first.get());
		//	}
		//}
		m_textDirty = false;
		m_scrollDirty = true;
	}

	if(m_scrollDirty)
	{
		if(m_vScrollBar.valid())
		{
			if(!m_textArray.empty())
			{
				const crBoundingBox &bbox = m_textObject->getBoundBox();
				float len = bbox.yLength() - m_rect[3] + getCharacterSize();
				m_vScrollBar->setRange(0.0f,len>0.0f?len:0.0f);
				m_vScrollBar->setEnable(true);
				if(m_defaultScrollPos>=0 && m_defaultScrollPos<=1)
					m_vScrollBar->setValue(m_vScrollBar->getRange()[m_defaultScrollPos]);
			}
			else
			{
				m_vScrollBar->setValue(0);
				m_vScrollBar->setEnable(false);
			}
			m_scrollChanged = true;
		}
		m_scrollDirty = false;
	}

	if(!m_textArray.empty() && m_scissor.valid())
	{
		computeScissor();
	}
}
void crHypertextWidgetNode::setHypertext(std::string filename,bool app)
{
	if(filename.empty() || (!app && m_hyperFile == filename))
		return;
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	m_hyperFile = filename;
	do 
	{
		rcfg::ConfigScript cfg_script;
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename))
		{
			CRCore::notify(CRCore::FATAL)<<"setHypertext(): file open error,"<<filename<<std::endl;
			return;
		}

		setHypertext(cfg_script,app);
		return;
	} while (0);
	if(!app)
	{
		m_validTextSize = 0;
		dirtyText();
	}
}
void crHypertextWidgetNode::setHypertext(rcfg::ConfigScript& hypertext,bool app)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	if(hypertext.Push("Hypertext"))
	{
		// temp usage
		ref_ptr<CRCore::crMatrixTransform> image;
		ref_ptr<CRText::crText> text;
		std::string str,imgfile;
		std::vector<float> v_i;
		int int1;
		float flt1;
		std::wstring wstr;

		// - - - - - - - - - - - - - - - - - parse global  attributes  - - - - - - - - - - - - - - - - -
		// draw mode
		if(hypertext.Get("DrawMode", str))
		{
			int1 = 0;
			if(str.find("TEXT")!=std::string::npos)
				int1 |= crText::TEXT;
			if(str.find("BOUNDINGBOX")!=std::string::npos)
				int1 |= crText::BOUNDINGBOX;
			if(str.find("FILLEDBOUNDINGBOX")!=std::string::npos)
				int1 |= crText::FILLEDBOUNDINGBOX;
			if(str.find("UNDERLINE")!=std::string::npos)
				int1 |= crText::UNDERLINE;
			setDrawMode(int1);
		}

		// spacing
		if(hypertext.Get("LineSpacing", flt1))
			setLineSpacing(flt1);
		if(hypertext.Get("UpLineSpacing", flt1))
			setUpLineSpacing(flt1);
		if(hypertext.Get("UnderLineSpacing", flt1))
			setUnderLineSpacing(flt1);
		if(hypertext.Get("Font", str))
			setFont(str);

		// size
		if(hypertext.Get("CharacterSize", flt1))
			setCharacterSize(flt1);
		//if(hypertext.Get("FontSize", v_i))
		//{
		//	setFontSize(crVector2s(v_i[0],v_i[1]));
		//}

		// color
		if(hypertext.Get("Color", v_i))
		{
			crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
			color /= 255.0f; 
			setColor(color);
		}

		// align
		if(hypertext.Get("Alignment", str))
		{
			if(str.compare("LEFT_TOP")==0)
				setAlignment(CRText::crText::LEFT_TOP);
			else if(str.compare("LEFT_CENTER")==0)
				setAlignment(CRText::crText::LEFT_CENTER);
			else if(str.compare("LEFT_BOTTOM")==0)
				setAlignment(CRText::crText::LEFT_BOTTOM);
			else if(str.compare("CENTER_TOP")==0)
				setAlignment(CRText::crText::CENTER_TOP);
			else if(str.compare("CENTER_CENTER")==0)
				setAlignment(CRText::crText::CENTER_CENTER);
			else if(str.compare("CENTER_BOTTOM")==0)
				setAlignment(CRText::crText::CENTER_BOTTOM);
			else if(str.compare("RIGHT_TOP")==0)
				setAlignment(CRText::crText::RIGHT_TOP);
			else if(str.compare("RIGHT_CENTER")==0)
				setAlignment(CRText::crText::RIGHT_CENTER);
			else if(str.compare("RIGHT_BOTTOM")==0)
				setAlignment(CRText::crText::RIGHT_BOTTOM);
			else if(str.compare("LEFT_BASE_LINE")==0)
				setAlignment(CRText::crText::LEFT_BASE_LINE);
			else if(str.compare("CENTER_BASE_LINE")==0)
				setAlignment(CRText::crText::CENTER_BASE_LINE);
			else if(str.compare("RIGHT_BASE_LINE")==0)
				setAlignment(CRText::crText::RIGHT_BASE_LINE);
			else if(str.compare("BASE_LINE")==0)
				setAlignment(CRText::crText::BASE_LINE);
		}

		// back drop
		if(hypertext.Get("BackdropType", str))
		{
			if(str.compare("BOTTOM_RIGHT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_RIGHT);
			else if(str.compare("CENTER_RIGHT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_CENTER_RIGHT);
			else if(str.compare("TOP_RIGHT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_TOP_RIGHT);
			else if(str.compare("BOTTOM_CENTER")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_CENTER);
			else if(str.compare("TOP_CENTER")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_TOP_CENTER);
			else if(str.compare("BOTTOM_LEFT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_LEFT);
			else if(str.compare("CENTER_LEFT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_CENTER_LEFT);
			else if(str.compare("TOP_LEFT")==0)
				setBackdropType(CRText::crText::DROP_SHADOW_TOP_LEFT);
			else if(str.compare("OUTLINE")==0)
				setBackdropType(CRText::crText::OUTLINE);
			else if(str.compare("NONE")==0)
				setBackdropType(CRText::crText::NONE);
		}
		if(hypertext.Get("BackdropOffset", v_i))
		{
			setBackdropOffset(crVector2(v_i[0],v_i[1]));
		}
		if(hypertext.Get("BackdropColor", v_i))
		{
			crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
			color /= 255.0f; 
			setBackdropColor(color);
		}

		// - - - - - - - - - - - - - - - - - parse contents  - - - - - - - - - - - - - - - - -
		int nContent = 1;
		int text_count = 0;
		int image_count = 0;
		int model_count = 0;
		std::wstring nulstr;
		int textArraySize = m_textArray.size();
		int imageArraySize = m_imageNodeVec.size();
		int modelArraySize = m_modelNodeVec.size();
		int start_text = app ? m_validTextSize : 0;
		int start_image = app ? m_validImageSize : 0;
		int start_model = app ? m_validModelSize : 0;
		while (hypertext.Push("Content", nContent++))
		{
			// parse image or text
			if(hypertext.Get("DrawMode", str))
			{
				if(str.find("IMAGE")!=std::string::npos)
				{
					// parse image
					float width = 0.0f, height = 0.0f;
					hypertext.Get("ImageWidth", width);
					hypertext.Get("ImageHeight", height);
					hypertext.Get("ImageFile", imgfile);
					if(imageArraySize < (start_image + image_count + 1))
					{
						image = new crMatrixTransform;
						image->setMatrix(crMatrix::scale(width, height, 1.0f));
						image->getOrCreateStateSet();
						crObject* obj = new crObject;
						//obj->getOrCreateStateSet();
						crGeometry* geo =  createTexturedQuadGeometry(crVector3(0, 0, 0),crVector3(1, 0, 0),crVector3(0, 1, 0));
						//geo->getOrCreateStateSet();
						obj->addDrawable(geo);
						image->addChild(obj);
						m_imageNodeVec.push_back(std::make_pair(image.get(), start_text + text_count));
						CRCore::crStateSet *state_set = image->getStateSet();
						ref_ptr<crTexture2D> texture_2d = 
							dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
						texture_2d = new crTexture2D;
						texture_2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
						texture_2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
						//texture_2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
						//texture_2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
						//texture_2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
						texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(imgfile,texture_2d.get()));
						state_set->setTextureAttributeAndModes(0,texture_2d.get(),crStateAttribute::ON);
					}
					else
					{
						image = m_imageNodeVec[start_image + image_count].first;
						m_imageNodeVec[start_image + image_count].second = start_text + text_count;
						image->setMatrix(crMatrix::scale(width, height,1.0f));
						CRCore::crStateSet *state_set = image->getStateSet();
						ref_ptr<crTexture2D> texture_2d = 
							dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
						texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(imgfile,texture_2d.get()));
					}
					
					// TODO - update size
					image_count++;
					hypertext.Pop();
					continue;
				}
				else if(str.find("MODEL")!=std::string::npos)
				{
					// parse image
					float width = 0.0f, height = 0.0f;
					hypertext.Get("ImageWidth", width);
					hypertext.Get("ImageHeight", height);
					hypertext.Get("CreFile", imgfile);
					if(modelArraySize < (start_model + model_count + 1))
					{
						image = new crMatrixTransform;
						CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
						proxyNode->setName("ProxyNode");
						proxyNode->setDescriptionsInString("DataFile "+imgfile);
						CRIOManager::crLoadManager::getInstance()->requestNode(imgfile,proxyNode.get(),false);
						if(proxyNode->getNumChildren()==1)
						{
							crGroup *cloneGroup = dynamic_cast<crGroup *>(proxyNode->clone(crCopyOp::DEEP_COPY_NODES));
							crNode *loadedNode = cloneGroup->getChild(0);
							CRIOManager::crLoadManager::getInstance()->requestAddNode(image.get(),cloneGroup,true);
						}
						image->setMatrix(crMatrix::scale(width, height, 1.0f));
						
						m_modelNodeVec.push_back(std::make_pair(image.get(), start_text + text_count));
					}
					else
					{
						image = new crMatrixTransform;
						CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
						proxyNode->setName("ProxyNode");
						proxyNode->setDescriptionsInString("DataFile "+imgfile);
						CRIOManager::crLoadManager::getInstance()->requestNode(imgfile,proxyNode.get(),false);
						if(proxyNode->getNumChildren()==1)
						{
							crGroup *cloneGroup = dynamic_cast<crGroup *>(proxyNode->clone(crCopyOp::DEEP_COPY_NODES));
							crNode *loadedNode = cloneGroup->getChild(0);
							CRIOManager::crLoadManager::getInstance()->requestAddNode(image.get(),cloneGroup,true);
						}
						image->setMatrix(crMatrix::scale(width, height, 1.0f));

						m_imageNodeVec[start_model + model_count].first = image;
						m_imageNodeVec[start_model + model_count].second = start_text + text_count;
					}
					// TODO - update size
					model_count++;
					hypertext.Pop();
					continue;
				}
				else
				{
					int1 = 0;
					if(str.find("TEXT")!=std::string::npos)
						int1 |= crText::TEXT;
					if(str.find("BOUNDINGBOX")!=std::string::npos)
						int1 |= crText::BOUNDINGBOX;
					if(str.find("FILLEDBOUNDINGBOX")!=std::string::npos)
						int1 |= crText::FILLEDBOUNDINGBOX;
					if(str.find("UNDERLINE")!=std::string::npos)
						int1 |= crText::UNDERLINE;
				}
			}
			else
			{
				int1 = -1;
			}

			// parse text
			if(textArraySize < start_text + text_count + 1)
			{
				text = new crText;
				m_textArray.push_back(TextPair(text.get(),nulstr));
			}
			else
			{
				text = m_textArray[start_text + text_count].first;
			}

			if(int1 >= 0)
			{
				text->setDrawMode(int1);
			}
			if(hypertext.Get("Name", str))
				text->setName(str);
			if(hypertext.Get("LineSpacing", flt1))
				text->setLineSpacing(flt1);
			else
				text->setLineSpacing(getLineSpacing());
			if(hypertext.Get("UnderLineSpacing", flt1))
				text->setUnderLineSpacing(flt1);
			else
				text->setUnderLineSpacing(getUnderLineSpacing());
			if(hypertext.Get("Font", str))
				text->setFont(str);
			else
				text->setFont(m_font);
			if(hypertext.Get("CharacterSize", int1))
			{
				text->setCharacterSize(int1);
				text->setFontResolution(int1*4,int1*4);
			}
			else
			{
				text->setCharacterSize(getCharacterSize());
				text->setFontResolution(m_fontSize[0],m_fontSize[1]);
			}
			//if(hypertext.Get("FontSize", v_i))
			//{
			//	text->setFontResolution((unsigned int)(v_i[0]),(unsigned int)(v_i[1]));
			//}
			//else
			//{
			//	text->setFontResolution(m_fontSize[0],m_fontSize[1]);
			//}
			if(hypertext.Get("Color", v_i))
			{
				crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
				color /= 255.0f; 
				text->setColor(color);
			}
			else
				text->setColor(m_color);
			text->setAlignment((crText::AlignmentType)m_alignment);

			if(hypertext.Get("BackdropType", str))
			{
				if(str.compare("BOTTOM_RIGHT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_RIGHT);
				else if(str.compare("CENTER_RIGHT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_CENTER_RIGHT);
				else if(str.compare("TOP_RIGHT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_RIGHT);
				else if(str.compare("BOTTOM_CENTER")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_CENTER);
				else if(str.compare("TOP_CENTER")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_CENTER);
				else if(str.compare("BOTTOM_LEFT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_BOTTOM_LEFT);
				else if(str.compare("CENTER_LEFT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_CENTER_LEFT);
				else if(str.compare("TOP_LEFT")==0)
					text->setBackdropType(CRText::crText::DROP_SHADOW_TOP_LEFT);
				else if(str.compare("OUTLINE")==0)
					text->setBackdropType(CRText::crText::OUTLINE);
				else if(str.compare("NONE")==0)
					text->setBackdropType(CRText::crText::NONE);
			}
			else
			{
				text->setBackdropType((crText::BackdropType)m_backdropType);
			}
			if(hypertext.Get("BackdropOffset", v_i))
			{
				text->setBackdropOffset(v_i[0],v_i[1]);
			}
			else
			{
				text->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
			}
			if(hypertext.Get("BackdropColor", v_i))
			{
				crVector4 color(v_i[0],v_i[1],v_i[2],v_i[3]);
				color /= 255.0f; 
				text->setBackdropColor(color);
			}
			else
			{
				text->setBackdropColor(m_backdropColor);
			}

			if(hypertext.Get("Text", str))
			{
				parseText(str);
				wstr = CRIOManager::convertUTF8toUTF16(str);
				text->setText(wstr.c_str());
				m_textArray[start_text + text_count].second = wstr;
			}
			if(hypertext.Get("Link", int1))
			{
				m_linkMap[text] = int1;
			}
			if (!hypertext.Pop())
				CRCore::notify(CRCore::FATAL)<<"crStaticTextWidgetNode::setHypertext: "<<hypertext.GetLastError()<<std::endl;

			text_count++;
		}

		hypertext.Pop();
		m_validTextSize = start_text + text_count ;
		m_validImageSize = start_image + image_count;
		m_validModelSize = start_model + model_count;
		dirtyText();
	}

	if(m_dataClass.valid())
	{
		CREncapsulation::loadExternData(hypertext,m_dataClass.get());
	}
	else
	{
		setDataClass(CREncapsulation::loadData(hypertext));
	}
}
//void crHypertextWidgetNode::releaseObjects(crState* state)
//{
//	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
//	for (TextVec::iterator itr = m_textArray.begin();
//		itr != m_textArray.end();
//		++itr)
//	{
//		if (itr->first.valid())
//			itr->first->releaseObjects(state);
//	}
//}
void crHypertextWidgetNode::getString(unsigned int i,std::wstring &outstr)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	if(i<m_validTextSize)
	{
		outstr = m_textArray[i].second;
	}
}
void crHypertextWidgetNode::modifyString(unsigned int i,const std::wstring &instr)
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	if(i<m_validTextSize)
	{
		m_textArray[i].second = instr;
		//m_textArray[i].first->setText(instr.c_str());
		dirtyText();
	}
}
void crHypertextWidgetNode::modifyString(unsigned int i,const std::string &instr)
{
	modifyString(i,CRIOManager::convertUTF8toUTF16(instr));
}
int crHypertextWidgetNode::getValidTextSize()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	return m_validTextSize;
}
void crHypertextWidgetNode::popTexts(unsigned int count)
{
	if(count == 0)
		return;
	{
		CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
		if(count>=m_validTextSize)
		{
			clearText();
		}
		else
		{
			for(int i = count; i<m_validTextSize; i++)
			{
				if(m_linkMap.find(m_textArray[i-count].first) != m_linkMap.end())
					m_linkMap.erase(m_textArray[i-count].first);
				m_textArray[i-count].first.swap(m_textArray[i].first);
				m_textArray[i-count].second = m_textArray[i].second;
			}
			m_validTextSize -= count;
		}
		dirtyText();
	}
}
void crHypertextWidgetNode::clearText()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
	m_validTextSize = 0;
	m_validImageSize = 0;
	m_validModelSize = 0;
	m_bSingleLine = false;
	//m_textArray.resize(0);//不能clear
	m_hyperFile.clear();
	m_linkMap.clear();
	dirtyText();
}
void crHypertextWidgetNode::setSingleLine(bool bIsSingle)
{
	m_bSingleLine = bIsSingle;
}
bool crHypertextWidgetNode::isSingleLine()const
{
	return m_bSingleLine;
}
//void crHypertextWidgetNode::popText(int count)
//{
//	CRCore::ScopedLock<crCriticalMutex> lock(m_textArrayMutex);
//	m_validTextSize -= count;
//	if(m_validTextSize<=0)
//	{
//		m_validTextSize = 0;
//		clearText();
//	}
//	else
//	{
//		TextVec::iterator itr =  m_textArray.begin();
//		int index = 0;
//		for (;itr!=m_textArray.end() && index<count;++index)
//		{
//			itr = m_textArray.erase(itr);
//			if(m_linkMap.find(itr->first) != m_linkMap.end())
//				m_linkMap.erase(itr->first);
//		}
//		dirtyText();
//	}
//}
//void crHypertextWidgetNode::recreateTextObject()
//{
//	if(m_textObject.valid())
//	{
//		crObject *newTextObject = new crObject;
//		newTextObject->setStateSet(m_textObject->getStateSet());
//		crVector2 cursor;
//		for( TextVec::iterator itr =  m_textArray.begin();
//			itr != m_textArray.end();
//			++itr )
//		{
//			itr->first->setMaximumWidth(m_rect[2]);
//			itr->first->setMaximumHeight(m_rect[3]);
//			itr->first->setPosition(crVector3(m_rect[0],m_rect[1] - m_characterSize,0.0f));
//			itr->first->setStartCoord(cursor);
//			itr->first->swapBuffers();
//			cursor = itr->first->getEndCoord();
//			itr->first->setVisiable(true);
//			newTextObject->addDrawable(itr->first.get());
//		}
//		m_textObject->setVisiable(false);
//		crLoadManager::getInstance()->requestRemoveNode(m_mvpwNode.get(),m_textObject.get());
//		crLoadManager::getInstance()->requestAddNode(m_mvpwNode.get(),newTextObject,false);
//		m_textObject = newTextObject;
//		m_scrollDirty = true;
//	}
//}
//////////////////////////////////////////////////////////////////////////
//
//crEditWidgetNode
//
//////////////////////////////////////////////////////////////////////////
float crEditWidgetNode::s_focusInterval = 0.5f;
crEditWidgetNode::crEditWidgetNode():
m_multiLine(false),
m_numeric(false),
m_autoSelectAll(false),
m_focusPos(0),
m_focusChanged(false),
m_focusInterval(0.0f)
{
	setCanSelectText(true);
}
crEditWidgetNode::crEditWidgetNode(const crEditWidgetNode& node,const crCopyOp& copyop):
crStaticTextWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_multiLine(node.m_multiLine),
m_numeric(node.m_numeric),
m_autoSelectAll(node.m_autoSelectAll),
m_focusPos(0),
m_focusChanged(false),
m_focusInterval(0.0f)
{
}
crEditWidgetNode::~crEditWidgetNode()
{

}
void crEditWidgetNode::setMultiLine(bool multiline)
{
	m_multiLine = multiline;
}
bool crEditWidgetNode::getMultiLine()
{
	return m_multiLine;
}
void crEditWidgetNode::setNumeric(bool numeric)
{
	m_numeric = numeric;
}
bool crEditWidgetNode::getNumeric()
{
	return m_numeric;
}
void crEditWidgetNode::setAutoSelectAll(bool autoselect)
{
	m_autoSelectAll = autoselect;
}
bool crEditWidgetNode::getAutoSelectAll()
{
	return m_autoSelectAll;
}
void crEditWidgetNode::setAlignment(unsigned char alignment)
{
	crStaticTextWidgetNode::setAlignment(alignment);
	if(m_focusCursor.valid())
		m_focusCursor->setAlignment((crText::AlignmentType)getAlignment());
}
void crEditWidgetNode::initWindow()
{
	crStaticTextWidgetNode::initWindow();
	if(!m_multiLine)m_text->setMaximumWidth(0.0f);
	//m_wstring.clear();
	m_focusCursor = new crText;
	m_focusCursor->setFont(/*"fonts/arial.ttf"*/m_font);
	m_focusCursor->setColor(m_color);
	m_focusCursor->setCharacterSize(getCharacterSize()+2);
	m_focusCursor->setFontResolution(m_fontSize[0],m_fontSize[1]);
	m_focusCursor->setAlignment((crText::AlignmentType)getAlignment()/*crText::BASE_LINE*/);
	//m_focusCursor->setMaximumWidth(m_rect[2]);
	//m_focusCursor->setMaximumHeight(m_rect[3]);
	//m_focusCursor->setBackdropType(crText::OUTLINE);
	//m_focusCursor->setBackdropColor(crVector4(0.0f,0.0f,0.0f,1.0f));
	m_focusCursor->setPosition(crVector3(m_rect[0],m_rect[1]-getUpLineHeight(),0.0f));
	m_focusCursor->setVisiable(false);
	m_focusCursor->setText("|");
	dynamic_cast<crObject *>(m_text->getParent(0))->addDrawable(m_focusCursor.get());
	setCanCaptureInput(true);
}
void crEditWidgetNode::resize()
{
	crStaticTextWidgetNode::resize();
	if (m_focusCursor.valid())
	{
		m_focusCursor->setCharacterSize(getCharacterSize() + 2);
		m_focusCursor->setFontResolution(m_fontSize[0], m_fontSize[1]);
	}
	validFocusPos();
}
void crEditWidgetNode::updateData()
{
	bool texditry = m_textDirty;
	if(m_textAttrDirty)
	{
		m_focusCursor->setColor(m_color);
	}
	crStaticTextWidgetNode::updateData();
	if(!m_multiLine && (texditry||m_focusChanged))
	{
		m_text->setMaximumWidth(0.0f);
		m_seltext->setMaximumWidth(0.0f);
		m_focusCursor->setMaximumWidth(0.0f);
		crVector2 cursorPos;
		m_text->getCursorPosition(m_focusPos,cursorPos);
		crVector3 textPos = m_text->getPosition();
		cursorPos[0] += textPos[0];
		if(cursorPos[0]>m_rect[0]+m_rect[2]-m_text->getCharacterWidth())
		{
			textPos[0] -= cursorPos[0] - (m_rect[0]+m_rect[2]-m_text->getCharacterWidth());
			m_text->setPosition(textPos);
			m_seltext->setPosition(textPos);
		}
		else if(textPos[0]<m_rect[0] && cursorPos[0]<m_rect[0]+m_text->getCharacterWidth())
		{
			textPos[0] += (m_rect[0]+m_rect[2]-m_text->getCharacterWidth())-cursorPos[0];
			if(textPos[0]>m_rect[0])
				textPos[0] = m_rect[0];
			m_text->setPosition(textPos);
			m_seltext->setPosition(textPos);
		}
		//else if(textPos[0]<m_rect[0] && cursorPos[0]<m_rect[0]+m_text->getCharacterWidth())
		//{
		//	textPos[0] += m_rect[0]+m_text->getCharacterWidth()-cursorPos[0];
		//	if(textPos[0]>m_rect[0])
		//		textPos[0] = m_rect[0];
		//	m_text->setPosition(textPos);
		//	m_seltext->setPosition(textPos);
		//}
	}

	if(crFilterRenderManager::getInstance()->getInputCapturer() == this)
	{
		m_focusInterval += crFrameStamp::getInstance()->getFrameInterval();
		if(m_focusInterval >= 0.0f && m_focusInterval <= s_focusInterval)
		{
			if(m_focusChanged/* && !m_scrollDirty*/)
			{
				//计算cursor坐标
				crVector2 cursorPos;
				m_text->getCursorPosition(m_focusPos,cursorPos);
				const crVector3& textPos = m_text->getPosition();
				crVector3 curPos = textPos;
				curPos[0] += cursorPos[0]-m_text->getCharacterWidth()*0.3f;
				curPos[1] += cursorPos[1];
				m_focusCursor->setPosition(curPos);

				if(m_vScrollBar.valid())
				{
					float textHeight = m_text->getLineHeight();
					float y = -cursorPos[1];
					int vscrollValue = m_vScrollBar->getValue();
					int _select = y - vscrollValue;
					if(_select<0)
					{
						vscrollValue += _select;
						m_vScrollBar->setValue(vscrollValue);
					}
					else if(_select>m_rect[3]-textHeight)
					{	
						vscrollValue += _select - m_rect[3]+textHeight;
						m_vScrollBar->setValue(vscrollValue);
					}
				}
				m_focusChanged = false;
			}
			m_focusCursor->setVisiable(true);
			if(m_scissor.valid() && !m_text->getVisiable())
			{
				computeScissor();
			}
		}
		else if(m_focusInterval>s_focusInterval)
		{
			m_focusInterval = -s_focusInterval;
			m_focusCursor->setVisiable(false);
		}
	}
	else
	{
		m_focusInterval = 0.0f;
		m_focusCursor->setVisiable(false);
	}
}

void crEditWidgetNode::kernelGetCapture()
{
	crStaticTextWidgetNode::kernelGetCapture();
	//if(crDisplaySettings::instance()->getRunMode() != crDisplaySettings::Editor)
	//{
	//	if (m_password)
	//	{
	//		::LoadKeyboardLayout( "0x0409", KLF_ACTIVATE|KLF_SETFORPROCESS );
	//	}
	//	else
	//	{
	//		if (NULL == ::LoadKeyboardLayout( "E0200804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
	//			if (NULL == ::LoadKeyboardLayout( "E0210804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
	//				if (NULL == ::LoadKeyboardLayout( "E0220804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
	//					::LoadKeyboardLayout( "E0230804", KLF_ACTIVATE|KLF_SETFORPROCESS );
	//	}
	//}
}

void crEditWidgetNode::kernelLostCapture()
{
	crStaticTextWidgetNode::kernelLostCapture();

	//if(crDisplaySettings::instance()->getRunMode() != crDisplaySettings::Editor)
 //		::LoadKeyboardLayout( "0x0409", KLF_ACTIVATE|KLF_SETFORPROCESS );
}

void crEditWidgetNode::kernelMouseEvent(float mx, float my,void *ea)
{
	crStaticTextWidgetNode::kernelMouseEvent(mx,my,ea);
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	if(_ea->getEventType() == crGUIEventAdapter::PUSH && _ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		if(m_autoSelectAll)
		{
			selectText(0,m_wstring.size());
		}
		else
		{
			crCanvasNode *canvas = getParentCanvas();
			canvas->mouseTrans(mx,my);
			crVector3 mouse(mx,my,0.0f);
			mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
			crVector3 textPos = m_text->getPosition();
			int height = (m_vScrollBar.valid()?m_vScrollBar->getValue():0) - getUpLineHeight();
			crVector2 cursorPos(mouse[0] - textPos[0], mouse[1] + height - textPos[1]);
			int focusPos = m_text->getCursorIndex(cursorPos);
			setFocusPos(focusPos);
		}
	}
}
void crEditWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	std::wstring strbuf = m_wstring;
	validFocusPos();
	int focusPos = m_focusPos;
	bool strchanged = false;
	switch(key)
	{
	case crGUIEventAdapter::KEY_Left:
		setFocusPos(m_focusPos-1);
		break;
	case crGUIEventAdapter::KEY_Right:
		setFocusPos(m_focusPos+1);
		break;
	case crGUIEventAdapter::KEY_Up:
		{
			crVector2 cursorPos;
			m_text->getCursorPosition(m_focusPos,cursorPos);
			cursorPos[1] += m_text->getLineHeight() * 0.5f;
			setFocusPos(m_text->getCursorIndex(cursorPos));
			if(m_parentWidget)
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_KeyUp);
			}
		}
		break;
	case crGUIEventAdapter::KEY_Down:
		{
			crVector2 cursorPos;
			m_text->getCursorPosition(m_focusPos,cursorPos);
			cursorPos[1] -= m_text->getLineHeight()*1.5f;
			setFocusPos(m_text->getCursorIndex(cursorPos));
			if(m_parentWidget)
			{
				m_parentWidget->updateParentWidget(this,WCH_UI_KeyDown);
			}
		}
		break;
	case 'c':
	case 'C':
		if(modKeyMask & crGUIEventAdapter::MODKEY_CTRL)
		{
			std::wstring seltext = getSelectString();
			std::string selstr = CRIOManager::convertUTF16toUTF8(seltext.c_str());
			if(OpenClipboard(NULL))
			{
				HGLOBAL clipbuffer;
				char * buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, selstr.length()+1);
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer, LPCSTR(selstr.c_str()));
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
			}
		}
		break;
	case 'v':
	case 'V':
		if(modKeyMask & crGUIEventAdapter::MODKEY_CTRL)
		{
			char * buffer = NULL;
			//打开剪贴板
			std::string fromClipboard;
			if ( OpenClipboard(NULL) )
			{
				HANDLE hData = GetClipboardData(CF_TEXT);
				if(hData)
				{
					char * buffer = (char*)GlobalLock(hData);
					fromClipboard = buffer;
					GlobalUnlock(hData);
					CloseClipboard();
					addString(fromClipboard);
				}
			}
		}
		
		break;
	case crGUIEventAdapter::KEY_Delete:
		if(!strbuf.empty())
		{
			if(m_textSelect[1]-m_textSelect[0]>0)
			{
				strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
				focusPos = m_textSelect[0];
				m_textSelect.set(0,0);
				strchanged = true;
			}
			else if(focusPos < strbuf.size())
			{
				if(strbuf.size() > focusPos)
					strbuf.erase(focusPos,1);
				strchanged = true;
			}
		}
		//m_focusPos = CRCore::clampTo(m_focusPos,0,(int)strbuf.size());
		break;
		
	}

	if(strchanged)
	{
		setString(strbuf);
		setFocusPos(focusPos);
		doEvent(WCH_UI_ValueChange);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_ValueChange);
		}
	}
}
void crEditWidgetNode::setFocusPos(int pos)
{
 	if(m_focusPos!=pos)
	{
		m_focusPos = pos;
		validFocusPos();
		m_focusChanged = true;
	}
}
void crEditWidgetNode::validFocusPos()
{
	int focusPos = CRCore::clampTo(m_focusPos,0,(int)m_wstring.size());
	if(m_focusPos!=focusPos)
	{
		m_focusPos = focusPos;
		m_focusChanged = true;
	}
}
void crEditWidgetNode::kernelInputChar(wchar_t c)
{
	std::wstring strbuf = m_wstring;
	validFocusPos();
	int focusPos = m_focusPos;
	bool strchanged = false;
	switch(c)
	{
	case 9://tab
	case 27://esc
		break;
	case 8:////////////////////////////BackSpace
		if(!strbuf.empty())
		{
			if(m_textSelect[1]-m_textSelect[0]>0)
			{
				strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
				focusPos = m_textSelect[0];
				m_textSelect.set(0,0);
				strchanged = true;
			}
			else if(focusPos>0)
			{
				--focusPos;
				if(strbuf.size()>=focusPos+1)
					strbuf.erase(focusPos,1);
				strchanged = true;
			}
		}
		//m_focusPos = CRCore::clampTo(m_focusPos,0,(int)strbuf.size());
		break;
	//case 127:////////////////////////////Delete
	//	if(!strbuf.empty())
	//	{
	//		if(m_textSelect[1]-m_textSelect[0]>0)
	//		{
	//			strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
	//			focusPos = m_textSelect[0];
	//			m_textSelect.set(0,0);
	//			strchanged = true;
	//		}
	//		else if(focusPos < strbuf.size())
	//		{
	//			if(strbuf.size() > focusPos +1)
	//				strbuf.erase(focusPos + 1,1);
	//			strchanged = true;
	//		}
	//	}
	//	//m_focusPos = CRCore::clampTo(m_focusPos,0,(int)strbuf.size());
	//	break;
	case 13:
		if(m_multiLine)
		{
			if(m_textSelect[1]-m_textSelect[0]>0)
			{
				strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
				focusPos = m_textSelect[0];
				m_textSelect.set(0,0);
			}

			strbuf.insert(focusPos,1,L'\n');
			focusPos++;

			strchanged = true;
		}
		break;
	default:
		if(c>256 || (c>=32 && c<=126))
		{
			if(m_textSelect[1]-m_textSelect[0]>0)
			{
				strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
				focusPos = m_textSelect[0];
				m_textSelect.set(0,0);
			}
			if(!m_numeric)
			{
				char hz[2];
				if(c>256)
				{
					hz[0]=(char)(c>>8);   
					hz[1]=(char)c;
				}
				if(c>256) 
				{
					std::wstring wstr = CRIOManager::convertUTF8toUTF16(hz,2);
					strbuf.insert(focusPos,wstr.c_str(),1);
					focusPos++;
				}
				else
				{
					strbuf.insert(focusPos,1,c);
					focusPos++;
				}

				strchanged = true;
			}
			else if(c>='-' && c<='9')
			{
				strbuf.insert(focusPos,1,c);
				focusPos++;
				strchanged = true;
			}
		}
		break;
	}
	
	if(strchanged)
	{
		setString(strbuf);
		setFocusPos(focusPos);
		doEvent(WCH_UI_ValueChange);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_ValueChange);
		}
	}
}
void crEditWidgetNode::setString(std::string &str)
{
	parseText(str);
	setString(CRIOManager::convertUTF8toUTF16(str));
}
void crEditWidgetNode::setString(const std::wstring &wstr)
{
	if(m_wstring.compare(wstr) != 0)
	{
		m_wstring = wstr;
		dirtyText();
	}
	setFocusPos(m_wstring.size());
}
void crEditWidgetNode::addString(std::string &str)
{
	std::wstring wstr = CRIOManager::convertUTF8toUTF16(str);
	addString(wstr);
}
void crEditWidgetNode::addString(const std::wstring &str)
{
	if(!str.empty())
	{
		int focusPos = m_focusPos;
		std::wstring strbuf = m_wstring;
		if(m_textSelect[1]-m_textSelect[0]>0)
		{
			strbuf.erase(m_textSelect[0],m_textSelect[1]-m_textSelect[0]);
			focusPos = m_textSelect[0];
			m_textSelect.set(0,0);
		}
		strbuf.insert(focusPos,str);
		focusPos+=str.length();
		setString(strbuf);
		setFocusPos(focusPos);
		doEvent(WCH_UI_ValueChange);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_ValueChange);
		}
	}
}
void crEditWidgetNode::clearString()
{
	crStaticTextWidgetNode::clearString();
	setFocusPos(0);
}


//////////////////////////////////////////////////////////////////////////
//
//crListBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crListBoxWidgetNode::crListBoxWidgetNode():
m_select(0),
m_mouseMoveSelect(false)
{
}
crListBoxWidgetNode::crListBoxWidgetNode(const crListBoxWidgetNode& node,const crCopyOp& copyop):
crStaticTextWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_select(node.m_select),
m_mouseMoveSelect(node.m_mouseMoveSelect)
{
}
crListBoxWidgetNode::~crListBoxWidgetNode()
{

}
void crListBoxWidgetNode::initWindow()
{
	setCanSelectText(true);
	setAlignment(crText::LEFT_BASE_LINE);
	crStaticTextWidgetNode::initWindow();
	if(m_text.valid()) m_text->setMaximumWidth(0.0f);
	if(m_seltext.valid()) m_seltext->setMaximumWidth(0.0f);
	//m_text->setMaximumWidth(m_rect[2]);
	//m_seltext->setMaximumWidth(m_rect[2]);
	//m_wstring.clear();
	////////test
	//m_dataList.clear();
	//addData("1ListTest");
	//addData("2ListTest");
	//addData("3ListTest");
	//addData("4ListTest");
	//addData("5ListTest");
	//addData("6ListTest");
	//addData("7ListTest");
	//addData("8ListTest");
	//addData("9ListTest");
	//addData("10ListTest");
	//addData("11ListTest");
	//addData("12ListTest");
	//addData("13ListTest");
	//addData("14ListTest");
	//addData("15ListTest");
	//addData("16ListTest");
	//addData("17ListTest");
	//addData("18ListTest");
	//addData("19ListTest");
	//addData("20ListTest");
	//addData("21ListTest");
	//addData("22ListTest");
	//addData("23ListTest");
	//addData("24ListTest");
	//select(1);
}
void crListBoxWidgetNode::updateData()
{
	//if(m_select<m_dataList.size())
	//{
	//	int from = 0;
	//	int to = 0;
	//	int count = 0;
	//	int i = 0;
	//	for (DataList::iterator itr = m_dataList.begin();
	//		itr != m_dataList.end();
	//		++itr)
	//	{
	//		if(i == m_select)
	//		{
	//			from = count;
	//			to = count + (*itr).length();
	//			break;
	//		}
	//		else
	//		{
	//			count += (*itr).length();
	//			i++;
	//		}
	//	}
	//	selectText(from,to);
	//}
	crStaticTextWidgetNode::updateData();
}
void crListBoxWidgetNode::kernelMouseEvent(float mx, float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	_crInt32 msg = 0;
	if(_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		msg = WCH_UI_LBtnUp;
	else if(_ea->getEventType() == crGUIEventAdapter::DOUBLECLICK && _ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		msg = WCH_UI_LBtnDblClk;
	else if(m_mouseMoveSelect && _ea->getEventType() == crGUIEventAdapter::MOVE)
		msg = WCH_UI_MouseMove;

	if(msg!=0)
	{
		crCanvasNode *canvas = getParentCanvas();
		canvas->mouseTrans(mx,my);
		crVector3 mouse(mx,my,0.0f);
		mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
		crVector3 textPos = m_text->getPosition();
		int height =/* (m_vScrollBar.valid()?m_vScrollBar->getValue():0)*/ - getUpLineHeight();
		crVector2 cursorPos(mouse[0] - textPos[0], mouse[1] + height - textPos[1]);
		int cursorpos = m_text->getCursorIndex(cursorPos);
		if(cursorpos<(int)m_wstring.size())
		{
			cursorpos = CRCore::clampTo(cursorpos,0,(int)m_wstring.size());
			int count = 0;
			int i = 0;
			for (DataList::iterator itr = m_dataList.begin();
				itr != m_dataList.end();
				++itr)
			{
				if(cursorpos < count)
				{
					break;
				}
				else
				{
					count += (*itr).size();
					i++;
				}
			}
			select(i-1);
		}

		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,msg);
			m_parentWidget->doEvent(MAKEINT32(_ea->getButton(),_ea->getEventType()),MAKECREPARAM(_ea,NULL));
		}
	}
}
void crListBoxWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Up:
		select(m_select-1);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_KeyUp);
		}
		break;
	case crGUIEventAdapter::KEY_Down:
		select(m_select+1);
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_KeyDown);
		}
		break;
	}
}
std::wstring crListBoxWidgetNode::getStringArrayInString()
{
	std::wstring str;
	for( DataList::const_iterator itr = m_dataList.begin();
		itr != m_dataList.end();
		++itr )
	{
		str += *itr;
	}
	return str;
}
void crListBoxWidgetNode::clearData()
{
	m_dataList.clear();
	clearString();
}
void crListBoxWidgetNode::addData(std::string data)
{
	parseText(data);
	std::wstring wdata = CRIOManager::convertUTF8toUTF16(data+'\n');
	m_dataList.push_back(wdata);
	addString(wdata);
}
void crListBoxWidgetNode::deleteCurrentRow()
{
	if(!isDataEmpty())
	{
		int i = getSelect();
		DataList::iterator itr = m_dataList.begin();
		itr+=i;
		m_dataList.erase(itr);
		setString(getStringArrayInString());
	}
}
void crListBoxWidgetNode::insertData(unsigned int index, const std::wstring &data)
{
	if (index >= m_dataList.size())
	{
		m_dataList.push_back(data);
	}
	else
	{
		m_dataList.insert(m_dataList.begin()+index, data);
	}
	setString(getStringArrayInString());
}
int crListBoxWidgetNode::getDataSize()
{
	return m_dataList.size(); 
}
bool crListBoxWidgetNode::isDataEmpty()
{
	return m_dataList.empty();
}
bool crListBoxWidgetNode::getData(unsigned int index,std::wstring &data)
{
	if(index < m_dataList.size())
	{
		data = m_dataList[index];
		if(!data.empty())
			data.erase(data.length()-1,1);
		return true;
	}
	return false;
}
bool crListBoxWidgetNode::modifyData(unsigned int index,const std::wstring &data)
{
	if(index < m_dataList.size())
	{
		m_dataList[index] = data;
		setString(getStringArrayInString());
		return true;
	}
	return false;
}
void crListBoxWidgetNode::select(unsigned int index)
{
	if(/*m_select!=index && */index<m_dataList.size())
	{
		m_select = index;
		if(m_vScrollBar.valid())
		{
			float textHeight = m_text->getLineHeight();
			float y = m_select * textHeight;
			int vscrollValue = m_vScrollBar->getValue();
			int _select = y - vscrollValue;
			if(_select<0)
			{
				vscrollValue += _select;
				m_vScrollBar->setValue(vscrollValue);
			}
			else if(_select>m_rect[3]-textHeight)
			{	
				vscrollValue += _select - m_rect[3]+textHeight;
				m_vScrollBar->setValue(vscrollValue);
			}
		}
		doEvent(WCH_UI_SelectChange);
	}
}
unsigned int crListBoxWidgetNode::getSelect()
{
	return m_select;
}
void crListBoxWidgetNode::setMouseMoveSelect(bool mouseMoveSelect)
{
	m_mouseMoveSelect = mouseMoveSelect;
}
bool crListBoxWidgetNode::validSelect()
{
	return m_select<m_dataList.size();
}
//////////////////////////////////////////////////////////////////////////
//
//crImageBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crImageBoxWidgetNode::crImageBoxWidgetNode():
m_canDragImage(true),
m_imageDirty(false)
{
	//m_rectOffset.set(0.0f,0.0f,0.0f,0.0f);
	for (int i = 0; i < 8; i++)
	{
		m_mode[i] = crTexEnv::MODULATE;
	}
}
crImageBoxWidgetNode::crImageBoxWidgetNode(const crImageBoxWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
//m_rectOffset(node.m_rectOffset),
m_canDragImage(node.m_canDragImage),
m_imageDirty(node.m_imageDirty),
m_imageStateSet(NULL)
{
	for (int i = 0; i < 8; i++)
	{
		m_imageFile[i] = node.m_imageFile[i];
		m_mode[i] = node.m_mode[i];
	}
}
crImageBoxWidgetNode::~crImageBoxWidgetNode()
{

}
void crImageBoxWidgetNode::initWindow()
{
	crSearchNodeBYClassNameVisitor searchObject;
	searchObject.insertClassNameId("crObject");
	accept(searchObject);
	m_imageObject = dynamic_cast<crObject *>(searchObject.getResult());
	if(!m_imageObject)
	{
		std::string str = "ImageBoxWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	m_imageQuad = dynamic_cast<crGeometry *>(m_imageObject->getDrawable(0));
	m_imageQuad->setStateSet(NULL);
	if(!m_imageStateSet.valid())
	{
		m_imageStateSet = new crStateSet;
	}
	m_imageObject->setStateSet(m_imageStateSet.get());
	//if(!m_imageFile.empty())
	//{
	//	crTexture2D *tex2d = new crTexture2D;
	//	tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
	//	tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
	//	tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
	//	tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
	//	tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
	//	tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile));
	//	m_imageStateSet->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
	//}
	//else
	//{
	//	m_imageObject->setVisiable(false);
	//}
	////m_imageFile = "black.img";
	//crObject *imageObject = getOrCreateImageObject();
	////setRectOffset(crVector4f(0.0f,0.0f,0.0f,0.0f));
	//crVector4f rect;
	//crVector3 minpos = getBoundBox().m_min;
	//crVector3 maxpos = getBoundBox().m_max;
	//minpos[0] += m_rectOffset[0];
	//minpos[1] += m_rectOffset[1];
	//maxpos[0] -= m_rectOffset[2];
	//maxpos[1] -= m_rectOffset[3];
	//rect.set(minpos[0],minpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);

	//crGeometry *quad = createTexturedQuadGeometry(crVector3(rect[0],rect[1],0),crVector3(rect[2],0,0),crVector3(0,rect[3],0));
	//m_imageStateSet = imageObject->getOrCreateStateSet();
	//if(!m_imageFile.empty())
	//{
	//	crTexture2D *tex2d = new crTexture2D;
	//	tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
	//	tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
	//	tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile));
	//	m_imageStateSet->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
	//}
	//else
	//{
	//	imageObject->setVisiable(false);
	//}
	//imageObject->addDrawable(quad);
	//addChild(imageObject);
}
void crImageBoxWidgetNode::setImageName(const std::string &image, unsigned int i, CRCore::crTexEnv::Mode mode)
{
	if(i<8)
	{
		if(m_imageFile[i].compare(image)!=0)
		{
			m_imageFile[i] = image;
			if(i>0)m_mode[i] = mode;
			if(m_imageStateSet.valid())
			{
				dirtyImage();
			}
		}
	}
}
void crImageBoxWidgetNode::setImage(CRCore::crImage *image, unsigned int i, CRCore::crTexEnv::Mode mode)
{
	setImageName("RTT", i, mode);
	ref_ptr<crTexture2D> tex2d = new crTexture2D;
	tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
	tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
	tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
	tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
	tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
	tex2d->setImage(0,image);
	getImageStateSet()->setTextureAttributeAndModes(i,tex2d.get(),crStateAttribute::ON);
	dirtyImage();
}
void crImageBoxWidgetNode::clearImage()
{
	for (int i = 0; i < 8; i++)
	{
		m_imageFile[i].clear();
	}
	if(m_imageStateSet.valid()) dirtyImage();
}
void crImageBoxWidgetNode::dirtyImage()
{
	m_imageDirty = true;
}
void crImageBoxWidgetNode::updateData()
{
	if(m_imageDirty && m_imageStateSet.valid())
	{
		m_imageDirty = false;
		bool visable = false;
		crTexture2D *tex2d = NULL;
		crTexEnv *texenv = NULL;
		for (int i = 0; i<8; i++)
		{
			if(!m_imageFile[i].empty())
			{
				visable = true;
				if(m_imageFile[i].compare("RTT")!=0)
				{
					tex2d = dynamic_cast<crTexture2D *>(m_imageStateSet->getTextureAttribute(i,crStateAttribute::TEXTURE));
					if(tex2d)
					{
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile[i],tex2d));
					}
					else 
					{
						crTexture2D *tex2d = new crTexture2D;
						tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
						tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile[i],tex2d));
						m_imageStateSet->setTextureAttributeAndModes(i,tex2d,crStateAttribute::ON);
					}
				}
				//if(i>0)
				//{
					texenv = dynamic_cast<crTexEnv *>(m_imageStateSet->getTextureAttribute(i,crStateAttribute::TEXENV));
					if(!texenv)
					{
						texenv = new crTexEnv(m_mode[i]);
						m_imageStateSet->setTextureAttribute(i,texenv,crStateAttribute::ON);
					}
					if(!m_imageQuad->getTexCoordArray(i))
					{
						m_imageQuad->setTexCoordArray(i,m_imageQuad->getTexCoordArray(0));
					}
				//}
			}
			else
			{
				m_imageStateSet->removeTextureAttribute(i,crStateAttribute::TEXTURE);
				m_imageStateSet->removeTextureAttribute(i,crStateAttribute::TEXENV);
			}
		}
		m_imageObject->setVisiable(visable);
	}
	//if(m_imageDirty && m_imageStateSet.valid())
	//{
	//	m_imageDirty = false;
	//	crTexture2D *tex2d;
	//	crTexEnv *texenv;
	//	for (int i = 0; i<8; i++)
	//	{
	//		if(m_imageFile.empty())
	//		{
	//			m_imageObject->setVisiable(false);
	//		}
	//		else
	//		{
	//			m_imageObject->setVisiable(true);
	//			if(m_imageFile.compare("RTT")!=0)
	//			{
	//				ref_ptr<crTexture2D> tex2d = dynamic_cast<crTexture2D *>(m_imageStateSet->getTextureAttribute(0,crStateAttribute::TEXTURE));
	//				if(tex2d.valid())
	//				{
	//					tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile,tex2d.get()));
	//				}
	//				else
	//				{
	//					tex2d = new crTexture2D;
	//					tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
	//					tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
	//					//tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP);
	//					//tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP);
	//					//tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP);
	//					tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile,tex2d.get()));
	//					m_imageStateSet->setTextureAttributeAndModes(0,tex2d.get(),crStateAttribute::ON);
	//				}
	//			}
	//		}
	//	}
	//}
}
const std::string &crImageBoxWidgetNode::getImageName(unsigned int i)const
{
	if(i>8)
		i = 0;
	return m_imageFile[i];
}
CRCore::crObject *crImageBoxWidgetNode::getImageObject()
{
	//if(!m_imageObject.valid())
	//{
	//	m_imageObject = new crObject;
	//	m_imageObject->setName("ImageObj");
	//	m_imageObject->addDescription("#ScriptAddNode");
	//}
	return m_imageObject.get();
}
CRCore::crGeometry *crImageBoxWidgetNode::getImageGeometry()
{
	return m_imageQuad.get();
}
CRCore::crStateSet *crImageBoxWidgetNode::getImageStateSet()
{
	if(!m_imageStateSet.valid())
	{
		initWindow();
	}
	return m_imageStateSet.get();
}

void crImageBoxWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	//if(m_canDragImage)
	//{
	//	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	//	if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
	//	{
	//	}
	//}
}
void crImageBoxWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
}
//void crImageBoxWidgetNode::setRectOffset(const CRCore::crVector4f &offset)
//{
//	m_rectOffset = offset;
//}
//const CRCore::crVector4f &crImageBoxWidgetNode::getRectOffset() const
//{
//	return m_rectOffset;
//}
void crImageBoxWidgetNode::setCanDragImage(bool canDrag)
{
	m_canDragImage = canDrag;
}
bool crImageBoxWidgetNode::getCanDragImage() const
{
	return m_canDragImage;
}


//////////////////////////////////////////////////////////////////////////
//
//crCheckBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crCheckBoxWidgetNode::crCheckBoxWidgetNode():
m_select(false)
{
}
crCheckBoxWidgetNode::crCheckBoxWidgetNode(const crCheckBoxWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_select(node.m_select)
{
}
crCheckBoxWidgetNode::~crCheckBoxWidgetNode()
{

}
void crCheckBoxWidgetNode::initWindow()
{
	m_button = dynamic_cast<crMultiSwitch *>(getChild(0));
	if(!m_button)
	{
		std::string str = "CheckBoxWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
}
void crCheckBoxWidgetNode::updateData()
{
	if(m_enable)
	{
		m_button->setActiveSwitchSet(m_select?1:0);
	}
	else
	{
		m_button->setActiveSwitchSet(2);
	}
}
void crCheckBoxWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON && _ea->getEventType() == crGUIEventAdapter::RELEASE)
	{
		select(!m_select);
	}
}
void crCheckBoxWidgetNode::kernelKeyUp(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Return:
		select(!m_select);
		break;
	}
}
void crCheckBoxWidgetNode::select(bool sel)
{
	if(m_select != sel)
	{
		m_select = sel;
		doEvent(WCH_UI_SelectChange);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crRadioWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crRadioWidgetNode::crRadioWidgetNode():
m_select(false),
m_acceptDrag(false),
m_mouseMoveSelect(false)
{
}
crRadioWidgetNode::crRadioWidgetNode(const crRadioWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_select(node.m_select),
m_mouseMoveSelect(node.m_mouseMoveSelect)
{
}
crRadioWidgetNode::~crRadioWidgetNode()
{

}
void crRadioWidgetNode::initWindow()
{
	NodeArray &childArray = getChildArray();
	for( NodeArray::iterator itr = childArray.begin();
		itr != childArray.end();
		++itr )
	{
		m_button = dynamic_cast<crMultiSwitch *>(itr->get());
		if(m_button.valid())
			break;
	}
	if(!m_button)
	{
		std::string str = "RadioWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	//m_button = dynamic_cast<crMultiSwitch *>(getChild(0));
}
void crRadioWidgetNode::updateData()
{
	if(m_enable)
		m_button->setActiveSwitchSet(m_select?1:0);
	else
		m_button->setActiveSwitchSet(2);
}
void crRadioWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	if(m_enable)
	{
		CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
		if(_ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON && (_ea->getEventType() == crGUIEventAdapter::RELEASE))
		{
			select(true);
		}
		else if(m_acceptDrag)
		{
			if (_ea->getEventType() == crGUIEventAdapter::DRAG && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				select(true);
			}
		}
		else if (m_mouseMoveSelect)
		{
			if(_ea->getEventType() == crGUIEventAdapter::MOVE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				select(true);
			}
		}
	}
}
void crRadioWidgetNode::kernelKeyUp(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Return:
		select(!m_select);
		break;
	}
}
void crRadioWidgetNode::select(bool sel)
{
	if(sel)
	{
		m_select = sel;
		if(m_parentWidget)
			m_parentWidget->updateParentWidget(this);
	}
	else
	{
		m_select = sel;
	}
}
void crRadioWidgetNode::kernelSelect(bool sel)
{
	m_select = sel;
}
void crRadioWidgetNode::setEnable(bool enable)
{
	crWidgetNode::setEnable(enable);
	if(!enable)
	{
		select(false);
	}
}
void crRadioWidgetNode::setAcceptDrag(bool acceptDrag)
{
	m_acceptDrag = acceptDrag;
}
void crRadioWidgetNode::setMouseMoveSelect(bool mouseMoveSelect)
{
	m_mouseMoveSelect = mouseMoveSelect;
}
//////////////////////////////////////////////////////////////////////////
//
//crRadioGroupWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crRadioGroupWidgetNode::crRadioGroupWidgetNode():
m_currentSelect(0)
{
}
crRadioGroupWidgetNode::crRadioGroupWidgetNode(const crRadioGroupWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_radioGroup(node.m_radioGroup),
m_currentSelect(0)
{
}
crRadioGroupWidgetNode::~crRadioGroupWidgetNode()
{

}
void crRadioGroupWidgetNode::addRadioName(const std::string &name)
{
	m_radioGroup.push_back(RadioNamePair(name,(crRadioWidgetNode*)NULL));
}
void crRadioGroupWidgetNode::initWindow()
{
	if(!m_parentCanvas) return;
	crRadioWidgetNode *radio;
	for( RadioGroup::iterator itr = m_radioGroup.begin();
		itr != m_radioGroup.end();
		++itr )
	{
		radio = dynamic_cast<crRadioWidgetNode *>(m_parentCanvas->getWidget(itr->first));
		if(radio)
		{
			itr->second = radio;
			radio->kernelSelect(false);
			radio->setParentWidget(this);
		}
		else
		{
			//CRCore::notify(CRCore::WARN)<<"crRadioGroupWidgetNode::initWindow(): RadioGroup("<<this->getName()<<") Not Find RadioElement("<<itr->first.c_str()<<")"<<std::endl;
			std::string str = "RadioGroup["+getName()+"] RadioWidget["+itr->first+"]没有找到";
			MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		}
	}
	//if(m_radioGroup.empty())
	//{
	//	CRCore::notify(CRCore::WARN)<<"crRadioGroupWidgetNode::initWindow(): RadioGroup("<<this->getName()<<") is empty"<<std::endl;
	//	std::string str = "RadioGroupWidget是空的["+getName()+"]";
	//	MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
	//	return;
	//}
	if(!m_radioGroup.empty())
	{
		if(m_currentSelect>=m_radioGroup.size())
			m_currentSelect = m_radioGroup.size() - 1;
		if(m_radioGroup[m_currentSelect].second)
			m_radioGroup[m_currentSelect].second->kernelSelect(true);
	}
}
void crRadioGroupWidgetNode::updateParentWidget(crWidgetNode *widget, _crInt32 msg)
{
	crRadioWidgetNode *radio = dynamic_cast<crRadioWidgetNode *>(widget);
	if(radio)
	{
		unsigned int i = 0;
		unsigned int selectIndex = 0;
		for( RadioGroup::iterator itr = m_radioGroup.begin();
			itr != m_radioGroup.end();
			++itr, ++i )
		{
			if(itr->second == radio)
			{
				selectIndex = i;
				break;
			}
		}
		if(selectIndex != m_currentSelect)
		{
			m_radioGroup[m_currentSelect].second->kernelSelect(false);
			m_currentSelect = selectIndex;
		}
		doEvent(WCH_UI_SelectChange);
	}
}
void crRadioGroupWidgetNode::select(int index)
{
	if(!m_radioGroup.empty() && index>=0 && index < m_radioGroup.size())
	{
		if(index != m_currentSelect) 
		{
			if(m_radioGroup[m_currentSelect].second) m_radioGroup[m_currentSelect].second->kernelSelect(false);
			m_currentSelect = index;
		}
		if(m_radioGroup[m_currentSelect].second) m_radioGroup[m_currentSelect].second->kernelSelect(true);
	}
}
int crRadioGroupWidgetNode::getSelect()
{
    return m_currentSelect;
}
crRadioWidgetNode *crRadioGroupWidgetNode::getCurrentSelectedRadio()
{
	return m_radioGroup[m_currentSelect].second;
}
crRadioGroupWidgetNode::RadioGroup &crRadioGroupWidgetNode::getRadioGroup()
{
	return m_radioGroup;
}
crRadioWidgetNode* crRadioGroupWidgetNode::getRadio(int i)
{
	if(i>=0 && i<m_radioGroup.size())
		return m_radioGroup[i].second;
	return NULL;
}
int crRadioGroupWidgetNode::getRadioIndex(crRadioWidgetNode* radio)
{
	for( int i = 0; i<m_radioGroup.size(); i++ )
	{
		if(m_radioGroup[i].second == radio)
			return i;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
//crProgressWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crProgressWidgetNode::crProgressWidgetNode():
m_progressBarType(HORIZONTALBAR),
m_progress(0)
{
}
crProgressWidgetNode::crProgressWidgetNode(const crProgressWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_progressBarType(node.m_progressBarType),
m_progress(0),
m_rectOffset(node.m_rectOffset)
{
}
crProgressWidgetNode::~crProgressWidgetNode()
{

}
void crProgressWidgetNode::initWindow()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos[0] += m_rectOffset[0];
	minpos[1] += m_rectOffset[1];
	maxpos[0] -= m_rectOffset[2];
	maxpos[1] -= m_rectOffset[3];
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0],minpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);

	NodeArray &childArray = getChildArray();
	for( NodeArray::iterator itr = childArray.begin();
		itr != childArray.end();
		++itr )
	{
		if(!m_upObject.valid() && (*itr)->getName().compare("UpObj") == 0)
		{
			m_upObject = dynamic_cast<crNode *>(itr->get());
			break;
		}
	}
	if(!m_upObject.valid())
	{
		m_upObject = getChild(0);
	}
	if(m_upObject.valid())
	{
		crStateSet *ss = new crStateSet;
		m_upObject->setStateSet(ss);
		m_scissor = new crScissor;
		ss->setAttributeAndModes(m_scissor.get(),crStateAttribute::ON);
		computeScissor();
	}
}
void crProgressWidgetNode::resize()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos[0] += m_rectOffset[0];
	minpos[1] += m_rectOffset[1];
	maxpos[0] -= m_rectOffset[2];
	maxpos[1] -= m_rectOffset[3];
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0], minpos[1], maxpos[0] - minpos[0], maxpos[1] - minpos[1]);
	computeScissor();
}
void crProgressWidgetNode::computeScissor()
{
	if(m_scissor.valid())
	{
		crBoundingBox bbox = getBoundBox();
		//crMatrix matrix;
		//if(this->getEnableMatrix())
		//{
		//	matrix = this->getMatrix();
		//}
		//matrix.preMult(getParentCanvas()->getMatrix());
		//bbox.m_min = bbox.m_min * matrix;
		//bbox.m_max = bbox.m_max * matrix;
		//bbox.correct();
		CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
		accept(parentMatrix);
		bbox.m_max = bbox.m_max * parentMatrix.getResult();
		bbox.m_min = bbox.m_min * parentMatrix.getResult();
		bbox.correct();
		crVector3 minpos = bbox.m_min;
		minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
		minpos[0] += m_rectOffset[0];
		minpos[1] += m_rectOffset[1];
		if(m_progressBarType == HORIZONTALBAR)
		{
			m_scissor->setScissor(minpos[0],minpos[1],m_progress*m_rect[2],m_rect[3]);
		}
		else
		{
			m_scissor->setScissor(minpos[0],minpos[1],m_rect[2],m_progress*m_rect[3]);
		}
	}
}
CRCore::crVector2 crProgressWidgetNode::getBarPosition()
{
	crBoundingBox bbox = getBoundBox();
	crMatrix matrix;
	if(this->getEnableMatrix())
	{
		matrix = this->getMatrix();
	}
	matrix.preMult(getParentCanvas()->getMatrix());
	bbox.m_min = bbox.m_min * matrix;
	bbox.m_max = bbox.m_max * matrix;
	bbox.correct();
	crVector2 barpos(bbox.m_min[0],bbox.m_min[1]);
	if(m_progressBarType == HORIZONTALBAR)
	{
		barpos[0] += m_progress*bbox.xLength();
	}
	else
	{
		barpos[1] += m_progress*bbox.yLength();
	}
	return barpos;
}
CRCore::crVector2 crProgressWidgetNode::getBarOffsetPosition()
{
	crBoundingBox bbox = getBoundBox();
	crMatrix matrix;
	if(this->getEnableMatrix())
	{
		matrix = this->getMatrix();
	}
	matrix.preMult(getParentCanvas()->getMatrix());
	bbox.m_min = bbox.m_min * matrix;
	bbox.m_max = bbox.m_max * matrix;
	bbox.correct();
	crVector2 barpos;
	if(m_progressBarType == HORIZONTALBAR)
	{
		barpos[0] = m_progress*bbox.xLength();
	}
	else
	{
		barpos[1] = m_progress*bbox.yLength();
	}
	return barpos;
}
void crProgressWidgetNode::updateData()
{
	computeScissor();
}
void crProgressWidgetNode::updateProgress(float rate)
{
	m_progress += rate;
	m_progress = CRCore::clampTo(m_progress,0.0f,1.0f);
	doEvent(WCH_UI_ValueChange);
}
void crProgressWidgetNode::setProgress(float progress)
{
	m_progress = progress;
	m_progress = CRCore::clampTo(m_progress,0.0f,1.0f);
	doEvent(WCH_UI_ValueChange);
}
float crProgressWidgetNode::getProgress()
{
	return m_progress;
}
void crProgressWidgetNode::resetProgress()
{
	m_progress = 0.0f;
}
void crProgressWidgetNode::fetchEnd()
{
	m_progress = 1.0f;
}
bool crProgressWidgetNode::isProgressFetchEnd()
{
	return m_progress == 1.0f;
}
void crProgressWidgetNode::setProgressBarType(ProgressBarType bar)
{
	m_progressBarType = bar;
}
crProgressWidgetNode::ProgressBarType crProgressWidgetNode::getProgressBarType()
{
	return m_progressBarType;
}
//////////////////////////////////////////////////////////////////////////
//
//crSliderWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crSliderWidgetNode::crSliderWidgetNode():
m_lineValue(0.05f),
m_drag(false)
{
}
crSliderWidgetNode::crSliderWidgetNode(const crSliderWidgetNode& node,const crCopyOp& copyop):
crProgressWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_lineValue(node.m_lineValue),
m_drag(false)
{
}
crSliderWidgetNode::~crSliderWidgetNode()
{

}
void crSliderWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	crCanvasNode *canvas = getParentCanvas();
	canvas->mouseTrans(mx,my);
	const crBoundingBox &barbox = m_sliderBar->getBoundBox();
	const crBoundingBox &bbox = getBoundBox();
	float lmax;
	float newvalue = m_progress;
	if(m_progressBarType == HORIZONTALBAR)
	{
		lmax = bbox.m_max[0] - bbox.m_min[0] - barbox.xLength();
	}
	else
	{
		lmax = bbox.m_max[1] - bbox.m_min[1] + barbox.yLength();
	}
	do 
	{
		/////////bar test
		crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_sliderBar->getChild(0));
		bar->setActiveSwitchSet(0);
		if(_ea->getEventType() == crGUIEventAdapter::PUSH && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if(m_progressBarType == VERTICALBAR)
			{
				if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] &&
					my>barbox.m_max[1] )
				{
					updateProgress(-m_lineValue);
				}
				else if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] &&
						my<barbox.m_min[1] )
				{
					updateProgress(m_lineValue);
				}
				else
				{//点击在BAR上
					bar->setActiveSwitchSet(2);
				}
			}
			else
			{
				if( mx<barbox.m_min[0] &&
					my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					updateProgress(-m_lineValue);
				}
				else if( mx>barbox.m_max[0] &&
					my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					updateProgress(m_lineValue);
				}
				else
				{//点击在BAR上
					bar->setActiveSwitchSet(2);
				}
			}
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::MOVE)
		{
			if( mx>=barbox.m_min[0] && mx<=barbox.m_max[0] && 
				my>=barbox.m_min[1] && my<=barbox.m_max[1] )
			{
				bar->setActiveSwitchSet(1);
			}
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::RELEASE && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			m_drag = false;
			setCanCaptureMouse(false);
			break;
		}
		else if (_ea->getEventType() == crGUIEventAdapter::DRAG && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if(m_progressBarType == VERTICALBAR)
			{
				if(m_drag)
				{
					newvalue += (my - m_lastmouse[1]) / lmax;
					bar->setActiveSwitchSet(2);
				}
				if(	my>=barbox.m_min[1] && my<=barbox.m_max[1] )
				{
					m_drag = true;
					m_lastmouse.set(mx,my);
					setCanCaptureMouse(true);
				}
			}
			else
			{
				if(m_drag)
				{
					newvalue += (mx - m_lastmouse[0]) / lmax;
					bar->setActiveSwitchSet(2);
				}
				if(	mx>=barbox.m_min[0] && mx<=barbox.m_max[0] )
				{
					m_drag = true;
					m_lastmouse.set(mx,my);
					setCanCaptureMouse(true);
				}
			}
		}
	} while (0);
	newvalue = CRCore::clampTo(newvalue,0.0f,1.0f);
	setValue(newvalue);
	//m_progress = newvalue;
	//计算 bar pos
	if(m_progressBarType == HORIZONTALBAR)
	{
		m_sliderBar->setPosition(crVector3(lmax * m_progress,0.0f,0.0f));
	}
	else
	{
		m_sliderBar->setPosition(crVector3(0.0f,lmax * m_progress,0.0f));
	}
}
void crSliderWidgetNode::kernelKeyDown(int key,unsigned int modKeyMask)
{
	switch(key)
	{
	case crGUIEventAdapter::KEY_Left:
		updateProgress(-m_lineValue);
		break;
	case crGUIEventAdapter::KEY_Right:
		updateProgress(m_lineValue);
		break;
	case crGUIEventAdapter::KEY_Up:
		updateProgress(m_lineValue);
		break;
	case crGUIEventAdapter::KEY_Down:
		updateProgress(-m_lineValue);
		break;
	}
}
void crSliderWidgetNode::kernelLostFocus()
{
	crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_sliderBar->getChild(0));
	bar->setActiveSwitchSet(0);
}
void crSliderWidgetNode::kernelLostCapture()
{
	crMultiSwitch *bar = dynamic_cast<crMultiSwitch *>(m_sliderBar->getChild(0));
	bar->setActiveSwitchSet(0);
}
void crSliderWidgetNode::initWindow()
{
	crProgressWidgetNode::initWindow();
	NodeArray &childArray = getChildArray();
	for( NodeArray::iterator itr = childArray.begin();
		itr != childArray.end();
		++itr )
	{
		if(!m_sliderBar.valid() && (*itr)->getName().compare("Bar") == 0)
		{
			m_sliderBar = dynamic_cast<crMatrixTransform *>(itr->get());
			break;
		}
	}
	if(!m_sliderBar)
	{
		std::string str = "SliderWidget错误，没有滑块（MatrixTransform）["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
}
void crSliderWidgetNode::updateData()
{
	const crBoundingBox &bbox = getBoundBox();
	const crBoundingBox &barbox = m_sliderBar->getBoundBox();
	float lmax;
	if(m_progressBarType == HORIZONTALBAR)
	{
		lmax = bbox.m_max[0] - bbox.m_min[0] - barbox.xLength();
		m_sliderBar->setPosition(crVector3(lmax * m_progress,0.0f,0.0f));
	}
	else
	{
		lmax = bbox.m_max[1] - bbox.m_min[1] - barbox.yLength();
		m_sliderBar->setPosition(crVector3(0.0f,lmax * m_progress,0.0f));
	}
}
void crSliderWidgetNode::setValue(float value)
{
	setProgress(value);
}

float crSliderWidgetNode::getValue()
{
	return getProgress();
}

void crSliderWidgetNode::setLineValue(float lineValue)
{
	m_lineValue = lineValue;
}

float crSliderWidgetNode::getLineValue()
{
	return m_lineValue;
}
//////////////////////////////////////////////////////////////////////////
//
//crChooseColorWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crChooseColorWidgetNode::crChooseColorWidgetNode()
{
	m_color.set(1.0f,1.0f,1.0f,1.0f);
}
crChooseColorWidgetNode::crChooseColorWidgetNode(const crChooseColorWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_color(node.m_color)
{
}
crChooseColorWidgetNode::~crChooseColorWidgetNode()
{

}
void crChooseColorWidgetNode::initWindow()
{
	m_drawable = dynamic_cast<crObject *>(getChild(0))->getDrawable(0);
	if(!m_drawable)
	{
		std::string str = "ChooseColorWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	m_drawable->setColor(m_color);
	m_drawable->setStateSet(NULL);
}
void crChooseColorWidgetNode::updateData()
{
	m_drawable->setColor(m_color);
}
void crChooseColorWidgetNode::kernelMouseEvent(float mx,float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	if(_ea->getEventType() == crGUIEventAdapter::PUSH && _ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
#ifdef WIN32
		CHOOSECOLOR cc;                 
		static COLORREF acrCustClr[16];
		HWND hwnd = GetActiveWindow();

		// Initialize CHOOSECOLOR 
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = hwnd;
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = RGB(m_color[0]*255,m_color[1]*255,m_color[2]*255);
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (ChooseColor(&cc)==TRUE) 
		{
			BYTE r = GetRValue(cc.rgbResult);
			BYTE g = GetGValue(cc.rgbResult);
			BYTE b = GetBValue(cc.rgbResult);
			m_color.set(r/255,g/255,b/255,1.0f);
		}
#endif
	}
}
void crChooseColorWidgetNode::setColor(const CRCore::crVector4 &color)
{
	m_color = color;
}
const CRCore::crVector4 &crChooseColorWidgetNode::getColor()const
{
	return m_color;
}
//////////////////////////////////////////////////////////////////////////
//
//crTableWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crTableWidgetNode::crTableWidgetNode():
m_currentSelectRow(0),
m_hasTitle(false),
m_defnull(0),
m_maskHeight(0),
m_mouseMoveSelect(false)

{
}
crTableWidgetNode::crTableWidgetNode(const crTableWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_currentSelectRow(0),
m_listGroup(node.m_listGroup),
m_hasTitle(node.m_hasTitle),
m_defnull(node.m_defnull),
m_maskHeight(node.m_maskHeight),
m_mouseMoveSelect(node.m_mouseMoveSelect)
{
}
crTableWidgetNode::~crTableWidgetNode()
{

}
void crTableWidgetNode::initWindow()
{
	if(!m_parentCanvas) return;
	for( NodeArray::iterator itr = m_childArray.begin();
		 itr != m_childArray.end();
		 ++itr )
	{
		if((*itr)->getName().compare("SelectMask") == 0)
		{
			m_selectMask = itr->get();
			break;
		}
	}
	if(m_selectMask.valid())
	{
		m_maskScissor = new crScissor;
		crStateSet *ss = new crStateSet;
		ss->setAttributeAndModes(m_maskScissor.get(),crStateAttribute::ON);
		m_selectMask->setStateSet(ss);
	}
	if(m_parentCanvas && !m_vScrollBarName.empty())
	{
		m_vScrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_parentCanvas->getWidget(m_vScrollBarName));
		if(m_vScrollBar.valid())
			m_vScrollBar->setParentWidget(this);	
	}
	crListBoxWidgetNode *listBox;
	for( ListGroup::iterator itr = m_listGroup.begin();
		itr != m_listGroup.end();
		++itr )
	{
		listBox = dynamic_cast<crListBoxWidgetNode *>(m_parentCanvas->getWidget(itr->first));
		if(listBox)
		{
			itr->second = listBox;
			listBox->setParentWidget(this);
			listBox->setMouseMoveSelect(m_mouseMoveSelect);
			if(m_maskHeight<listBox->getLineHeight())
				m_maskHeight = listBox->getLineHeight();

			if(m_vScrollBar.valid())
			{
				m_vScrollBar->setLineValue(listBox->getLineHeight());
				m_vScrollBar->setPageValue(listBox->getRect()[3]);
				listBox->setVScrollBar(m_vScrollBar.get());
			}
		}
		else
		{
			std::string str = "TableWidget["+getName()+"] ListBoxWidget["+itr->first+"]没有找到";
			MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
			CRCore::notify(CRCore::WARN)<<"crTableWidgetNode::initWindow(): 错误:"<<str<<std::endl;
			m_listGroup.clear();
			break;
		}
	}
	//m_maskHeight *= crDisplaySettings::instance()->getUIViewScale()[1];
	//if(m_listGroup.empty())
	//{
	//	CRCore::notify(CRCore::WARN)<<"crTableWidgetNode::initWindow(): ListGroup("<<this->getName()<<") is empty. CanvasNameID = "<<m_parentCanvas->getName()<<std::endl;
	//	std::string str = "TableWidget是空的["+getName()+"]";
	//	MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
	//	return;
	//}
	if(!m_listGroup.empty())
	{
		for( ListGroup::iterator itr = m_listGroup.begin();
			itr != m_listGroup.end();
			++itr )
		{
			itr->second->select(m_currentSelectRow);
		}
	}
}
void crTableWidgetNode::resize()
{
	m_maskHeight = 0;
	crListBoxWidgetNode *listBox;
	for (ListGroup::iterator itr = m_listGroup.begin();
		itr != m_listGroup.end();
		++itr)
	{
		listBox = itr->second;
		if (listBox)
		{
			listBox->resize();
			if (m_maskHeight < listBox->getLineHeight())
				m_maskHeight = listBox->getLineHeight();
		}
	}
}
void crTableWidgetNode::computeMaskScissor()
{
	if(m_maskScissor.valid())
	{
		crBoundingBox bbox = getBoundBox();
		const crMatrix& matrix = getParentCanvas()->getMatrix();
		bbox.m_min = bbox.m_min * matrix;
		bbox.m_max = bbox.m_max * matrix;
		bbox.correct();
		crVector3 minpos = bbox.m_min;
		crVector3 maxpos = bbox.m_max;
		minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
		maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
		int height = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
		m_maskScissor->setScissor(minpos[0],maxpos[1]-(m_currentSelectRow+1) * m_maskHeight+height,maxpos[0] - minpos[0],m_maskHeight);
	}
}
void crTableWidgetNode::updateData()
{
	if(m_selectMask.valid() && m_selectMask->getVisiable())
	{
		computeMaskScissor();
	}
}
void crTableWidgetNode::updateParentWidget(crWidgetNode *widget, _crInt32 msg)
{
	if(dynamic_cast<crScrollBarWidgetNode *>(widget))
	{
		for( ListGroup::iterator itr = m_listGroup.begin();
			itr != m_listGroup.end();
			++itr )
		{
			itr->second->scrollChanged();
		}
	}
	else
	{
		crListBoxWidgetNode *listBox = dynamic_cast<crListBoxWidgetNode *>(widget);
		if(listBox && listBox->validSelect())
		{
			selectRow(listBox->getSelect());
		}
	}
	if (msg == WCH_UI_LBtnDown)
	{
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_LBtnDown);
		}
	}
	if (msg == WCH_UI_LBtnUp)
	{
		if(m_parentWidget)
		{
			m_parentWidget->updateParentWidget(this,WCH_UI_LBtnUp);
		}
	}
}
void crTableWidgetNode::addData(unsigned int col, unsigned int row, const std::string &data)
{
	if(col < m_listGroup.size())
	{
		crListBoxWidgetNode *listBox = m_listGroup[col].second;
		while(listBox->getDataSize()<row)
		{
			listBox->addData("");
		}
		listBox->addData(data);
	}
}
bool crTableWidgetNode::getData(unsigned int col, unsigned int row, std::wstring &outdata)
{
	if(col < m_listGroup.size())
	{
		return m_listGroup[col].second->getData(row,outdata);
	}
	return false;
}
int crTableWidgetNode::getRowCount()
{
	int row = 0;
	for( ListGroup::iterator itr = m_listGroup.begin();
		itr != m_listGroup.end();
		++itr )
	{
		row = CRCore::maximum(row,itr->second->getDataSize());
	}
	return row;
}
void crTableWidgetNode::selectRow(int row)
{
	int rowcount = getRowCount();
	m_currentSelectRow = CRCore::minimum(row,rowcount>0?rowcount-1:0);
	for( ListGroup::iterator itr = m_listGroup.begin();
		itr != m_listGroup.end();
		++itr )
	{
		if(m_hasTitle)
			itr->second->select(m_currentSelectRow == 0?-1:m_currentSelectRow);
		else
			itr->second->select(m_currentSelectRow);
	}
	if(m_selectMask.valid())
	{
		if(rowcount == 0 || (m_hasTitle && m_currentSelectRow == 0))
		{
			m_selectMask->setVisiable(false);
		}
		else
		{
			computeMaskScissor();
			m_selectMask->setVisiable(true);
		}
	}
	doEvent(WCH_UI_SelectChange);
}
void crTableWidgetNode::clearData()
{
	m_currentSelectRow = 0;
	for( ListGroup::iterator itr = m_listGroup.begin();
		itr != m_listGroup.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_dataVec.clear();
	if(m_selectMask.valid())
	{
		m_selectMask->setVisiable(false);
	}
}
void crTableWidgetNode::clearColumn()
{
	m_listGroup.clear();
}
void crTableWidgetNode::addColumn(const std::string &columnName)
{
	crListBoxWidgetNode *listBox = NULL;
	if(m_parentCanvas)
	{
		for( ListGroup::iterator itr = m_listGroup.begin();
			itr != m_listGroup.end();
			++itr )
		{
			listBox = dynamic_cast<crListBoxWidgetNode *>(m_parentCanvas->getWidget(columnName));
		}
	}
	m_listGroup.push_back(std::make_pair(columnName,listBox));
}
unsigned int crTableWidgetNode::getSelect()
{
	return m_currentSelectRow;
}
void crTableWidgetNode::setDefNull(int defnull)
{
	m_defnull = defnull;
}
void crTableWidgetNode::addData(int row,int data)
{
	while(row>=m_dataVec.size())
	{
		m_dataVec.push_back(m_defnull);
	}
	m_dataVec[row] = data;
}
int crTableWidgetNode::getData(int row)
{
	if(row<m_dataVec.size())
		return m_dataVec[row];
	return m_defnull;
}
int crTableWidgetNode::getSelectData()
{
	return getData(getSelect());
}
void crTableWidgetNode::setHasTitle(bool hasTitle)
{
	m_hasTitle = hasTitle;
}
bool crTableWidgetNode::getHasTitle()
{
	return m_hasTitle;
}

void crTableWidgetNode::setVScrollBarName(const std::string &name)
{
	m_vScrollBarName = name;
}
const std::string &crTableWidgetNode::getVScrollBarName()const
{
	return m_vScrollBarName;
}
void crTableWidgetNode::setMouseMoveSelect(bool mouseMoveSelect)
{
	m_mouseMoveSelect = mouseMoveSelect;
}
//////////////////////////////////////////////////////////////////////////
//
//crComboBoxWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crComboBoxWidgetNode::crComboBoxWidgetNode()
{
}
crComboBoxWidgetNode::crComboBoxWidgetNode(const crComboBoxWidgetNode& node,const crCopyOp& copyop):
	crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
	m_comboTextBoxName(node.m_comboTextBoxName),
	m_comboButtonName(node.m_comboButtonName),
	m_comboListBoxName(node.m_comboListBoxName)
{
}
crComboBoxWidgetNode::~crComboBoxWidgetNode()
{

}
void crComboBoxWidgetNode::initWindow()
{
	if(m_parentCanvas)
	{
		if(!m_comboTextBoxName.empty())
		{
			m_comboTextBox = dynamic_cast<crStaticTextWidgetNode *>(m_parentCanvas->getWidget(m_comboTextBoxName));
			if(m_comboTextBox.valid())
				m_comboTextBox->setParentWidget(this);	
		}
		if(!m_comboButtonName.empty())
		{
			m_comboButton = dynamic_cast<crButtonWidgetNode *>(m_parentCanvas->getWidget(m_comboButtonName));
			if(m_comboButton.valid())
				m_comboButton->setParentWidget(this);	
		}
		if(!m_comboListBoxName.empty())
		{
			m_comboList = dynamic_cast<crTableWidgetNode *>(m_parentCanvas->getWidget(m_comboListBoxName));
			if(m_comboList.valid())
				m_comboList->setParentWidget(this);	
		}
	}
	if(!m_comboList||!m_comboTextBox)
	{
		std::string str = "ComboBoxWidget错误["+getName()+"]";
		MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		return;
	}
	m_comboList->setVisiable(false);
	m_comboList->setEnable(false);
	m_comboList->setCanCaptureInput(false);
	m_comboList->setMouseMoveSelect(true);
	for (int i = 0 ; i < m_comboList->getChildArray().size(); i++)
	{
		m_comboList->getChild(i)->setVisiable(false);
	}
	//init select
	//std::wstring data;
	//m_comboList->getData(0,m_comboList->getSelect(),data);
	//m_comboTextBox->setString(data);

}
void crComboBoxWidgetNode::updateParentWidget(crWidgetNode *widget,_crInt32 msg)
{
	if(msg != WCH_UI_LBtnUp && msg != WCH_UI_KeyUp && msg != WCH_UI_KeyDown)
		return;
	if(m_comboButton.get() == widget)
	{
		//显示listbox
		bool listVisiable = !(m_comboList->getVisiable());
		m_comboList->setVisiable(listVisiable);
		m_comboList->setEnable(listVisiable);
		//m_comboList->setCanCaptureInput(listVisiable);
		//m_comboList->setCanCaptureMouse(listVisiable);
		for (int i = 0 ; i < m_comboList->getChildArray().size(); i++)
		{
			m_comboList->getChild(i)->setVisiable(listVisiable);
			crListBoxWidgetNode *listnode = dynamic_cast<crListBoxWidgetNode *>(m_comboList->getChild(i));
			if (listnode)
			{
				listnode->setCanCaptureInput(listVisiable);
				listnode->setCanCaptureMouse(listVisiable);
				crFilterRenderManager::getInstance()->setMouseCapturer(listVisiable?listnode:NULL);
				crFilterRenderManager::getInstance()->setInputCapturer(listVisiable?listnode:NULL);
			}
		}
		//crFilterRenderManager::getInstance()->setInputCapturer(listVisiable?m_comboList.get():NULL);
	}
	else if(m_comboTextBox.get() == widget)
	{
		//if(dynamic_cast<crStaticTextWidgetNode *>(m_comboTextBox.get()))
		//{
		//	m_comboList->setVisiable(true);
		//	m_comboList->setEnable(true);
		//	m_comboList->setCanCaptureInput(true);
		//	m_comboList->setCanCaptureMouse(true);
		//	for (int i = 0 ; i < m_comboList->getChildArray().size(); i++)
		//	{
		//		m_comboList->getChild(i)->setVisiable(true);
		//	}
		//	crFilterRenderManager::getInstance()->setInputCapturer(m_comboList.get());
		//	if(msg == WCH_UI_KeyUp)
		//	{
		//		m_comboList->selectRow(m_comboList->getSelect()-1);
		//	}
		//	else if (msg == WCH_UI_KeyDown)
		//	{
		//		m_comboList->selectRow(m_comboList->getSelect()+1);
		//	}
		//	else if(msg == WCH_UI_ValueChange)
		//	{//可接受输入，根据输入单词去list搜索
		//		const std::wstring& str = m_comboTextBox->getString();
		//	}
		//}
		//else
		//{
		bool listVisiable = !(m_comboList->getVisiable());
		m_comboList->setVisiable(listVisiable);
		m_comboList->setEnable(listVisiable);
		//m_comboList->setCanCaptureInput(listVisiable);
		//m_comboList->setCanCaptureMouse(listVisiable);
		for (int i = 0 ; i < m_comboList->getChildArray().size(); i++)
		{
			m_comboList->getChild(i)->setVisiable(listVisiable);
			crListBoxWidgetNode *listnode = dynamic_cast<crListBoxWidgetNode *>(m_comboList->getChild(i));
			if (listnode)
			{
				listnode->setCanCaptureInput(listVisiable);
				listnode->setCanCaptureMouse(listVisiable);
				crFilterRenderManager::getInstance()->setMouseCapturer(listVisiable?listnode:NULL);
				crFilterRenderManager::getInstance()->setInputCapturer(listVisiable?listnode:NULL);
			}
		}
		//crFilterRenderManager::getInstance()->setInputCapturer(listVisiable?m_comboList.get():NULL);
		//crFilterRenderManager::getInstance()->setMouseCapturer(listVisiable?m_comboList.get():NULL);
		//}
	}
	else if(m_comboList.get() == widget)
	{
		std::wstring data;
		m_comboList->getData(0,m_comboList->getSelect(),data);
		m_comboTextBox->setString(data);
		m_comboList->setVisiable(false);
		m_comboList->setEnable(false);
		//	m_comboList->setCanCaptureInput(false);
		for (int i = 0 ; i < m_comboList->getChildArray().size(); i++)
		{
			m_comboList->getChild(i)->setVisiable(false);
			crListBoxWidgetNode *listnode = dynamic_cast<crListBoxWidgetNode *>(m_comboList->getChild(i));
			if (listnode)
			{
				listnode->setCanCaptureInput(false);
				listnode->setCanCaptureMouse(false);
			}
		}
		crFilterRenderManager::getInstance()->setInputCapturer(NULL);
		crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
		doEvent(WCH_UI_SelectChange);
	}
}
void crComboBoxWidgetNode::select(unsigned int index)
{
	m_comboList->selectRow(index);
	std::wstring data;
	m_comboList->getData(0,m_comboList->getSelect(),data);
	m_comboTextBox->setString(data);
}

std::wstring crComboBoxWidgetNode::getValue()
{
	return m_comboTextBox->getString();
}

unsigned int crComboBoxWidgetNode::getSelectIndex()
{ 
	return m_comboList->getSelect(); 
}
void crComboBoxWidgetNode::setComboTextBoxName(const std::string &textbox)
{
	m_comboTextBoxName = textbox;
}
void crComboBoxWidgetNode::setComboButtonName(const std::string &button)
{
	m_comboButtonName = button;
}
void crComboBoxWidgetNode::setComboListBoxName(const std::string &list)
{
	m_comboListBoxName = list;
}
//////////////////////////////////////////////////////////////////////////
//
//crListControlNode
//
//////////////////////////////////////////////////////////////////////////
crListControlNode::crListControlNode():
m_imageDirty(false),
m_selected(false),
m_data(0)
{
	for (int i = 0; i < 8; i++)
	{
		m_mode[i] = crTexEnv::MODULATE;
	}
}
crListControlNode::crListControlNode(const crListControlNode& node,const crCopyOp& copyop):
crTextAttrWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_imageDirty(node.m_imageDirty),
m_title(node.m_title),
m_title2(node.m_title2),
m_texPosOffset(node.m_texPosOffset),
m_selected(node.m_selected),
m_data(node.m_data)
{
	for (int i = 0; i < 8; i++)
	{
		m_imageFile[i] = node.m_imageFile[i];
		m_mode[i] = node.m_mode[i];
	}
}
crListControlNode::~crListControlNode()
{

}
void crListControlNode::setRect(CRCore::crVector4f &rect)
{
	m_rect = rect;
	if(!m_imageObject.valid())
	{
		m_imageObject = new crObject;
		m_imageObject->setName("ImageObj");
		//addChild(m_imageObject.get());
	}
	if(!m_imageQuad.valid())
	{
		m_imageQuad = createTexturedQuadGeometry(crVector3(m_rect[0],m_rect[1]-m_rect[3],0),crVector3(m_rect[2],0,0),crVector3(0,m_rect[3],0));
		m_imageQuad->setUseDisplayList(false);
		m_imageQuad->setUseVertexBufferObjects(false);
		m_imageStateSet = m_imageQuad->getOrCreateStateSet();
		//bool visable = false;
		//crTexture2D *tex2d;
		//for (int i = 0; i<8; i++)
		//{
		//	if(!m_imageFile[i].empty())
		//	{
		//		tex2d = new crTexture2D;
		//		tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
		//		tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
		//		tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile[i],tex2d));
		//		m_imageStateSet->setTextureAttributeAndModes(i,tex2d,crStateAttribute::ON);
		//		visable = true;
		//	}
		//}
		//m_imageObject->setVisiable(visable);
		m_imageObject->addDrawable(m_imageQuad.get());
		//CRIOManager::crLoadManager::getInstance()->requestCompile(m_imageObject.get());
	}
	else
	{//调整rect
		crVector3f corner = crVector3(m_rect[0],m_rect[1]-m_rect[3],0);
		crVector3f widthVec = crVector3(m_rect[2],0,0);
		crVector3f heightVec = crVector3(0,m_rect[3],0);
		Vec3Array* coords = dynamic_cast<Vec3Array*>(m_imageQuad->getVertexArray());
		(*coords)[0] = corner+heightVec;
		(*coords)[1] = corner;
		(*coords)[2] = corner+widthVec;
		(*coords)[3] = corner+widthVec+heightVec;
	}

	if(!m_textObject.valid())
	{
		m_textObject = new crObject;
		m_textObject->setName("TextObject");
		//addChild(m_textObject.get());
	}
	if(!m_titleText.valid())
	{
		m_texPosOffset[1] = 0;
		setColor(crVector4(1,1,1,1));
		//setBackdropType(crText::OUTLINE);
		setCharacterSize(10);
		setFontSize(crVector2s(16,16));
		m_titleText = new crText;
		m_titleText->setFont(m_font);
		m_titleText->setColor(m_color);
		m_titleText->setFontResolution(m_fontSize[0],m_fontSize[1]);
		m_titleText->setCharacterSize(getCharacterSize());
		m_titleText->setLineSpacing(getLineSpacing());
		m_titleText->setAlignment(crText::LEFT_BASE_LINE);
		m_titleText->setMaximumWidth(m_rect[2]);
		//m_titleText->setMaximumHeight(m_rect[3]);
		m_titleText->setBackdropType((crText::BackdropType)m_backdropType);
		m_titleText->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
		m_titleText->setBackdropColor(m_backdropColor);
		//m_titleText->setBackdropType(crText::OUTLINE);
		//m_titleText->setBackdropOffset(0.05f,0.05f);
		//m_titleText->setBackdropColor(crVector4(0,0,0,1));
		m_titleText->setPosition(crVector3(m_rect[0] + m_texPosOffset[0],m_rect[1] - getUpLineHeight()+ m_texPosOffset[1],0.0f));
		m_titleText->setVisiable(false);
		m_textObject->addDrawable(m_titleText.get());
	}
	else
	{
		m_titleText->setMaximumWidth(m_rect[2]);
		//m_titleText->setMaximumHeight(m_rect[3]);
	}
	if(!m_titleText2.valid())
	{
		m_texPosOffset[1] = 0;
		setColor(crVector4(0,1,0,1));
		//setBackdropType(crText::OUTLINE);
		setCharacterSize(16);
		setFontSize(crVector2s(16,16));
		m_titleText2 = new crText;
		m_titleText2->setFont(m_font);
		m_titleText2->setColor(m_color);
		m_titleText2->setFontResolution(m_fontSize[0],m_fontSize[1]);
		m_titleText2->setCharacterSize(getCharacterSize());
		m_titleText2->setLineSpacing(getLineSpacing());
		m_titleText2->setAlignment(crText::RIGHT_BASE_LINE);
		m_titleText2->setMaximumWidth(m_rect[2]);
		m_titleText2->setMaximumHeight(m_rect[3]);
		//m_titleText2->setBackdropType((crText::BackdropType)m_backdropType);
		//m_titleText2->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
		//m_titleText2->setBackdropColor(m_backdropColor);
		m_titleText2->setBackdropType(crText::OUTLINE);
		m_titleText2->setBackdropOffset(m_backdropOffset[0],m_backdropOffset[1]);
		m_titleText2->setBackdropColor(crVector4(0,0,0,1));
		m_titleText2->setPosition(crVector3(m_rect[0] + m_texPosOffset[0],m_rect[1] - m_rect[3] + m_texPosOffset[1],0.0f));
		m_titleText2->setVisiable(false);
		m_textObject->addDrawable(m_titleText2.get());
	}
	else
	{
		m_titleText2->setMaximumWidth(m_rect[2]);
		//m_titleText2->setMaximumHeight(m_rect[3]);
	}
}
void crListControlNode::updateData()
{
	if(m_textDirty)
	{
		m_titleText->setText(CRIOManager::convertUTF8toUTF16(m_title).c_str());
		m_titleText->setVisiable(!m_title.empty());
		m_titleText2->setText(CRIOManager::convertUTF8toUTF16(m_title2).c_str());
		m_titleText2->setVisiable(!m_title2.empty());
		m_textDirty = false;
	}
	if(m_imageDirty && m_imageStateSet.valid())
	{
		m_imageDirty = false;
		bool visable = false;
		crTexture2D *tex2d = NULL;
		crTexEnv *texenv = NULL;
		for (int i = 0; i<8; i++)
		{
			if(!m_imageFile[i].empty())
			{
				visable = true;
				if(m_imageFile[i].compare("RTT")!=0)
				{
					tex2d = dynamic_cast<crTexture2D *>(m_imageStateSet->getTextureAttribute(i,crStateAttribute::TEXTURE));
					if(tex2d)
					{
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile[i],tex2d));
					}
					else
					{
						crTexture2D *tex2d = new crTexture2D;
						tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
						tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
						tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(m_imageFile[i],tex2d));
						m_imageStateSet->setTextureAttributeAndModes(i,tex2d,crStateAttribute::ON);
					}
				}
				//if(i>0)
				//{
					texenv = dynamic_cast<crTexEnv *>(m_imageStateSet->getTextureAttribute(i,crStateAttribute::TEXENV));
					if(!texenv)
					{
						//if(i == 0)
						//	texenv = new crTexEnv(crTexEnv::MODULATE);
						//else
						//	texenv = new crTexEnv(crTexEnv::ADD);
						texenv = new crTexEnv(m_mode[i]);
						m_imageStateSet->setTextureAttribute(i,texenv,crStateAttribute::ON);
					}
					if(!m_imageQuad->getTexCoordArray(i))
					{
						m_imageQuad->setTexCoordArray(i,m_imageQuad->getTexCoordArray(0));
					}
				//}
			}
			else
			{
				m_imageStateSet->removeTextureAttribute(i,crStateAttribute::TEXTURE);
				m_imageStateSet->removeTextureAttribute(i,crStateAttribute::TEXENV);
			}
		}
		m_imageObject->setVisiable(visable);
	}
}
void crListControlNode::setTextPosOffset(CRCore::crVector2 &offset)
{
	m_texPosOffset = offset;
	if(m_titleText.valid())
	{
		m_titleText->setPosition(crVector3(m_rect[0] + m_texPosOffset[0],m_rect[1] - getUpLineHeight() + m_texPosOffset[1],0.0f));
	}
	if(m_titleText2.valid())
	{
		m_titleText2->setPosition(crVector3(m_rect[0] + m_texPosOffset[0],m_rect[1] - getUpLineHeight() + m_texPosOffset[1],0.0f));
	}
}
////Image
void crListControlNode::setImageName(const std::string &image, unsigned int i, CRCore::crTexEnv::Mode mode)
{
	if(i<8)
	{
		if(m_imageFile[i].compare(image)!=0)
		{
			m_imageFile[i] = image;
			if(i>0)m_mode[i] = mode;
			if(m_imageStateSet.valid())
			{
				dirtyImage();
			}
		}
	}
}
void crListControlNode::dirtyImage()
{
	m_imageDirty = true;
}
const std::string &crListControlNode::getImageName(unsigned int i)const
{
	if(i>8)
		i = 0;
	return m_imageFile[i];
}
CRCore::crObject *crListControlNode::getImageObject()
{
	return m_imageObject.get();
}
CRCore::crGeometry *crListControlNode::getImageQuad()
{
	return m_imageQuad.get();
}
//list node
void crListControlNode::setSelect(bool select)
{
	m_selected = select;
}
bool crListControlNode::getSelect()
{
	return m_selected;
}
void crListControlNode::setData(void* data)
{
	m_data = (int)data;
}
void crListControlNode::setData(int data)
{
	m_data = data;
}
int crListControlNode::getData()
{
	return m_data;
}
void crListControlNode::setVisiable( bool visiable )
{
	crTextAttrWidgetNode::setVisiable(visiable);
	m_imageObject->setVisiable(visiable);
	m_textObject->setVisiable(visiable);
	getImageQuad()->setColor(crVector4(1.0f,1.0f,1.0f,1.0f));
}
void crListControlNode::setTitle(const std::string &title)
{
	m_title = title;
	dirtyText();
}
const std::string &crListControlNode::getTitle() const
{
	return m_title;
}
void crListControlNode::setTitle2(const std::string &title2)
{
	m_title2 = title2;
	dirtyText();
}
const std::string &crListControlNode::getTitle2() const
{
	return m_title2;
}
const CRCore::crVector4f &crListControlNode::getRect() const
{
	return m_rect;
}


//////////////////////////////////////////////////////////////////////////
//
//crListControlWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crListControlWidgetNode::crListControlWidgetNode():
m_scrollDirty(false),
m_scrollChanged(false),
m_validNodeCount(0),
m_select(0),
m_defaultScrollPos(0),
m_canDragNode(true),
m_mouseMoveSelect(true),
m_startYPos(0)
{
	m_rectOffset.set(0,0,0,0);
	m_nodeSize.set(32,32);
	m_spaceBetween.set(7,6);
	m_selectColor.set(0.5f, 0.5f, 1.0f, 1.0f);
}
crListControlWidgetNode::crListControlWidgetNode(const crListControlWidgetNode& node,const crCopyOp& copyop):
crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
m_nodeSize(node.m_nodeSize),
m_spaceBetween(node.m_spaceBetween),
m_listNodeVec(node.m_listNodeVec),
m_rectOffset(node.m_rectOffset),
m_vScrollBarName(node.m_vScrollBarName),
m_validNodeCount(node.m_validNodeCount),
m_scrollDirty(false),
m_scrollChanged(false),
m_select(node.m_select),
m_defaultScrollPos(node.m_defaultScrollPos),
m_selectColor(node.m_selectColor),
m_canDragNode(node.m_canDragNode),
m_mouseMoveSelect(node.m_mouseMoveSelect)
{
}
crListControlWidgetNode::~crListControlWidgetNode()
{

}
void crListControlWidgetNode::initWindow()
{
	if(m_parentCanvas && !m_vScrollBarName.empty())
	{
		m_vScrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_parentCanvas->getWidget(m_vScrollBarName));
		if(m_vScrollBar.valid())
			m_vScrollBar->setParentWidget(this);	
	}
	//
	//m_rectOffset.set(-0.005,-0.01,-0.005,-0.01);
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos[0] += m_rectOffset[0];
	minpos[1] += m_rectOffset[1];
	maxpos[0] -= m_rectOffset[2];
	maxpos[1] -= m_rectOffset[3];
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0],maxpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);

	m_mvpwNode = new crMatrixTransform;
	m_mvpwNode->setName("InverseMVPW");
	m_mvpwNode->addDescription("#ScriptAddNode");
	m_mvpwNode->setMatrix(crFilterRenderManager::getInstance()->getInverseMVPW());
	addChild(m_mvpwNode.get());
	m_imageGroup = new crGroup;
	m_mvpwNode->addChild(m_imageGroup.get());
	m_textGroup = new crGroup;
	m_mvpwNode->addChild(m_textGroup.get());
	m_currentDragNode = new crMatrixTransform;
	m_currentDragNode->setVisiable(false);
	m_mvpwNode->addChild(m_currentDragNode.get());
	//m_nodeSize.set(32,32);
	//m_spaceBetween.set(7,6);
	//float scale = 1.0f;//crDisplaySettings::instance()->getWidthScale();
	//m_nodeSize[0] *= scale;
	//m_nodeSize[1] *= scale;
	//m_spaceBetween[0] *= scale;
	//m_spaceBetween[1] *= scale;
	crVector2 uiviewscale = crDisplaySettings::instance()->getUIViewScale();
	m_nodeSize[0] *= uiviewscale[0];
	m_nodeSize[1] *= uiviewscale[1];
	m_spaceBetween[0] *= uiviewscale[0];
	m_spaceBetween[1] *= uiviewscale[1];

	if(m_validNodeCount>0)
	{
		short count = m_validNodeCount;
		m_validNodeCount = 0;
		setListNodeCount(count);
	}

	if(m_vScrollBar.valid())
	{
		//m_vScrollBar->setRange(0,100); 
		m_vScrollBar->setLineValue(m_nodeSize[1]+m_spaceBetween[1]);//控件高度
		m_vScrollBar->setPageValue(m_rect[3]);//窗口高度
		m_scrollDirty = true;
	}
	////
	m_scissor = new crScissor;
	crStateSet *ss = new crStateSet;
	ss->setAttributeAndModes(m_scissor.get(),crStateAttribute::ON);
	m_imageGroup->setStateSet(ss);
	m_textGroup->setStateSet(ss);
	computeScissor();
}
void crListControlWidgetNode::resize()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos[0] += m_rectOffset[0];
	minpos[1] += m_rectOffset[1];
	maxpos[0] -= m_rectOffset[2];
	maxpos[1] -= m_rectOffset[3];
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0], maxpos[1], maxpos[0] - minpos[0], maxpos[1] - minpos[1]);
	m_mvpwNode->setMatrix(crFilterRenderManager::getInstance()->getInverseMVPW());

	if (m_vScrollBar.valid())
	{
		//m_vScrollBar->setRange(0,100); 
		m_vScrollBar->setLineValue(m_nodeSize[1] + m_spaceBetween[1]);//控件高度
		m_vScrollBar->setPageValue(m_rect[3]);//窗口高度
		m_scrollDirty = true;
	}
	computeScissor();
	crVector4f rect;
	float deta = 1;//容忍值
	crListControlNode *listControl;
	rect.set(m_rect[0], m_rect[1], m_nodeSize[0], m_nodeSize[1]);
	for (ListNodeVec::iterator itr = m_listNodeVec.begin();
		itr != m_listNodeVec.end();
		++itr)
	{
		listControl = itr->get();
		listControl->setRect(rect);

		rect[0] += m_nodeSize[0] + m_spaceBetween[0];
		if (rect[0] + m_nodeSize[0]>m_rect[0] + m_rect[2] + deta)
		{//下一行
			rect[0] = m_rect[0]/*+m_spaceBetween[0]*/;
			rect[1] -= m_nodeSize[1] + m_spaceBetween[1];
		}
	}
}
bool crListControlWidgetNode::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_bbox.expandBy(getChild(0)->getBoundBox());

	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_boundSphere.expandBy(m_bbox);
	m_bBoundSphere_computed = true;
	return true;
}
void crListControlWidgetNode::computeScissor()
{
	if(m_scissor.valid())
	{
		crBoundingBox bbox = getBoundBox();
		//crMatrix matrix;
		//if(this->getEnableMatrix())
		//{
		//	matrix = this->getMatrix();
		//}
		//matrix.preMult(getParentCanvas()->getMatrix());
		//bbox.m_min = bbox.m_min * matrix;
		//bbox.m_max = bbox.m_max * matrix;
		//bbox.correct();
		CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
		accept(parentMatrix);
		bbox.m_max = bbox.m_max * parentMatrix.getResult();
		bbox.m_min = bbox.m_min * parentMatrix.getResult();
		bbox.correct();
		crVector3 minpos = bbox.m_min;
		minpos[0] += m_rectOffset[0];
		minpos[1] += m_rectOffset[1];
		minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
		m_scissor->setScissor(minpos[0],minpos[1],m_rect[2],m_rect[3]);
	}
}
void crListControlWidgetNode::setListNodeCount(short count)
{
	//select(0);
	//if(m_validNodeCount == count)
	//{
	//	m_scrollDirty = true;
	//	return;
	//}
	if(m_mvpwNode.valid() && m_validNodeCount != count)
	{
		short nodeCount = m_listNodeVec.size();
		if(nodeCount < count)
		{//分配剩余的
			crVector4f rect;
			float deta = 1;//容忍值
			if(nodeCount==0)
			{
				rect.set(m_rect[0]/*+m_spaceBetween[0]*/,m_rect[1]/*-m_spaceBetween[1]*/,m_nodeSize[0],m_nodeSize[1]);
			}
			else
			{
				rect = m_listNodeVec[nodeCount-1]->getRect();
				rect[0] += m_nodeSize[0] + m_spaceBetween[0];
				if(rect[0]+m_nodeSize[0]>m_rect[0]+m_rect[2]+deta)
				{//下一行
					rect[0] = m_rect[0]/*+m_spaceBetween[0]*/;
					rect[1] -= m_nodeSize[1]+m_spaceBetween[1];
				}
			}
			crListControlNode *listControl;
			for(int i = nodeCount; i<count; i++)
			{
				listControl = new crListControlNode;
				m_listNodeVec.push_back(listControl);
				listControl->setRect(rect);
				//m_imageGroup->addChild(listControl->getImageObject());
				//m_textGroup->addChild(listControl->getTextObject());
				crLoadManager::getInstance()->requestAddNode(m_imageGroup.get(),listControl->getImageObject(),false);
				crLoadManager::getInstance()->requestAddNode(m_textGroup.get(),listControl->getTextObject(),false);

				rect[0] += m_nodeSize[0] + m_spaceBetween[0];
				if(rect[0]+m_nodeSize[0]>m_rect[0]+m_rect[2]+deta)
				{//下一行
					rect[0] = m_rect[0]/*+m_spaceBetween[0]*/;
					rect[1] -= m_nodeSize[1]+m_spaceBetween[1];
				}
			}
		}
		else if(nodeCount > count)
		{//
			for(int i = count; i<nodeCount; i++)
			{
				m_listNodeVec[i]->setVisiable(false);
			}
		}
	}
	m_validNodeCount = count;
	//m_scrollDirty = true;
	if(m_validNodeCount>0)
	{
		crListControlNode *node = m_listNodeVec[m_validNodeCount-1].get();
		CRCore::crVector4f m_rect1 = node->getRect();
		float len = m_rect[1] - m_rect1[1] - m_spaceBetween[1];// - m_nodeSize[1] - m_spaceBetween[1];
		if(m_vScrollBar!=NULL)
		{
			m_vScrollBar->setRange(0.0f,len>0.0f?len:0.0f);
			m_vScrollBar->setEnable(true);
			if(m_defaultScrollPos>=0 && m_defaultScrollPos<=1)
				m_vScrollBar->setValue(m_vScrollBar->getRange()[m_defaultScrollPos]);
		}
	}
}
void crListControlWidgetNode::updateParentWidget(crWidgetNode *widget,_crInt32 msg)
{
	if(m_vScrollBar.get() == widget)
	{
		m_scrollChanged = true;
	}
}
void crListControlWidgetNode::setStartYPos(int pagenum)
{
	m_startYPos = (m_rect[3] + m_spaceBetween[1])*pagenum ;
	m_scrollChanged = true;
}
void crListControlWidgetNode::updateData()
{
	if(m_scrollChanged)//滑动滑块
	{
		float ypos = (m_vScrollBar.valid()?m_vScrollBar->getValue():0)+m_startYPos;
		m_mvpwNode->setMatrix(crMatrix::translate(0,ypos,0) * crFilterRenderManager::getInstance()->getInverseMVPW());
		m_scrollChanged = false;
	}
	//if(m_scrollDirty)//加入控件，滑块长度变化
	//{
	//	if(m_vScrollBar.valid())
	//	{
	//		if(!m_listNodeVec.empty())
	//		{
	//			crBoundingBox bbox = m_imageGroup->getBoundBox();
	//			float len = bbox.yLength();
	//			//float len = m_validNodeCount;///(m_nodeSize[1]+m_spaceBetween[1]);////crDisplaySettings::instance()->getUIPixelMeter();
	//			m_vScrollBar->setRange(0.0f,len>0.0f?len:0.0f);
	//			m_vScrollBar->setEnable(true);
	//			if(m_defaultScrollPos>=0 && m_defaultScrollPos<=1)
	//				m_vScrollBar->setValue(m_vScrollBar->getRange()[m_defaultScrollPos]);
	//		}
	//		else
	//		{
	//			m_vScrollBar->setValue(0);
	//			m_vScrollBar->setEnable(false);
	//		}
	//	}
	//	m_scrollDirty = false;
	//	m_scrollChanged = true;
	//}
	if(m_scissor.valid())
	{
		computeScissor();
	}
	for( int i = 0; i<m_validNodeCount; ++i )
	{
		m_listNodeVec[i]->updateData();
	}
}

void crListControlWidgetNode::removeSelectListNode()
{
	if(m_select<m_validNodeCount)
	{
		m_listNodeVec[m_select]->setVisiable(false);
		int i = m_select+1;
		if(i==m_validNodeCount)
			i = 0;
		select(i);
	}
}
void crListControlWidgetNode::clearList()
{
	for(int i = 0; i<m_validNodeCount; i++)
	{
		m_listNodeVec[i]->setVisiable(false);
		m_listNodeVec[i]->getImageQuad()->setColor(crVector4(1.0f,1.0f,1.0f,1.0f));
	}
	m_validNodeCount = 0;
	m_select = 0;
	m_scrollDirty = true;
}
crListControlWidgetNode::ListNodeVec &crListControlWidgetNode::getListNodeVec()
{
	return m_listNodeVec;
}
crListControlNode *crListControlWidgetNode::getSelectNode()
{
	if(m_select>=0 && m_select<m_validNodeCount)
		return m_listNodeVec[m_select].get();
	return NULL;
}
crListControlNode *crListControlWidgetNode::getListNode(int i)
{
	if(i>=0 && i<m_validNodeCount)
		return m_listNodeVec[i].get();
	return NULL;
}
void crListControlWidgetNode::select(short i)
{
	if(m_listNodeVec.empty())
		return;
	if(m_select>=0 && m_select<m_validNodeCount)
	{
		m_listNodeVec[m_select]->getImageQuad()->setColor(crVector4(1.0f,1.0f,1.0f,1.0f));
	}
	m_select = i;
	if(m_select>=0 && m_select<m_validNodeCount && m_listNodeVec[m_select]->getEnable())
	{
		m_listNodeVec[m_select]->getImageQuad()->setColor(m_selectColor);
	}
	doEvent(WCH_UI_SelectChange);
}
int crListControlWidgetNode::MouseOnNodeIndex(float mx, float my)
{
	crCanvasNode *canvas = getParentCanvas();
	canvas->mouseTrans(mx,my);
	crVector3 mouse(mx,my,0.0f);
	mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
	float ypos = (m_vScrollBar.valid()?m_vScrollBar->getValue():0)+m_startYPos;
	mouse[1] -= ypos;
	for(int i = 0; i<m_validNodeCount; i++)
	{
		const crVector4i &rect = m_listNodeVec[i]->getRect();
		if( mouse[0]>rect[0] && mouse[0]<rect[0]+rect[2] &&
			mouse[1]>rect[1]-rect[3] && mouse[1]<rect[1] )
		{
			return i;
		}
	}
	return -1;
}
void crListControlWidgetNode::kernelMouseEvent(float mx, float my,void *ea)
{
	CRProducer::crGUIEventAdapter* _ea = (CRProducer::crGUIEventAdapter*)ea;
	_crInt32 msg = crKeyboardMouseHandle::getInstance()->getMouseMsg(_ea);
	crCanvasNode *canvas = getParentCanvas();
	canvas->mouseTrans(mx,my);
	crVector3 mouse(mx,my,0.0f);
	mouse = crFilterRenderManager::getInstance()->getWindowPosition(mouse);
	float ypos = (m_vScrollBar.valid()?m_vScrollBar->getValue():0)+m_startYPos;
	if(m_canDragNode && m_currentDragNode.valid() && m_currentDragNode->getVisiable())
	{
		if(msg == WCH_UI_LBtnDrag && getSelectNode())
		{
			crListControlNode *node = getSelectNode();
			const CRCore::crVector4f &rect = node->getRect();
			crVector3 pos;
			pos[0] = mouse[0] - rect[0] - m_nodeSize[0]*0.5f;
			pos[1] = mouse[1] - rect[1] - ypos + m_nodeSize[1]*0.5f;
			m_currentDragNode->setPosition(pos);
		}
		else if(msg == WCH_UI_LBtnUp)
		{
			m_currentDragNode->setVisiable(false);
			m_currentDragNode->removeChild(0,1);
			crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
			crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
			bindview->showCursor("Default");
			doEvent(WCH_UI_ListItemDrag, MAKECREPARAM(m_select,ea));
		}
	}
	else
	{
		bool doselect = false;;
		if(m_mouseMoveSelect && msg == WCH_UI_MouseMove)
		{
			doselect = true;
		}
		else if(msg == WCH_UI_LBtnDown || msg == WCH_UI_LBtnDrag)
		{
 			doselect = true;
		}
		if(doselect)
		{
			//float ypos = (m_vScrollBar.valid()?m_vScrollBar->getValue():0)+m_startYPos;
			mouse[1] -= ypos;
			for(int i = 0; i<m_validNodeCount; i++)
			{
				const crVector4i &rect = m_listNodeVec[i]->getRect();
				if( mouse[0]>rect[0] && mouse[0]<rect[0]+rect[2] &&
					mouse[1]>rect[1]-rect[3] && mouse[1]<rect[1] )
				{
					select(i);
					if(msg == WCH_UI_LBtnDrag)
					{
						if(m_canDragNode)
						{
							crListControlNode *node = getSelectNode();
							if(node && node->getEnable())
							{
								m_currentDragNode->addChild(node->getImageObject());
								m_currentDragNode->setPosition(CRCore::crVector3(0.0f,0.0f,0.0f));
								m_currentDragNode->setVisiable(true);
								crFilterRenderManager::getInstance()->setMouseCapturer(this);
								crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
								bindview->hideCursor(false);
							}
						}
					}
					else
					{
						doEvent(msg,MAKECREPARAM(i,ea));
					}
					return;
				}
			}
			select(-1);
			doEvent(msg,MAKECREPARAM(-1,ea));
		}
		else
		{
			doEvent(msg,MAKECREPARAM(m_select,ea));
		}
	}
}
short crListControlWidgetNode::getListNodeCount()
{
	return m_validNodeCount;
}
void crListControlWidgetNode::setDefaultScrollPos(char pos)
{
	m_defaultScrollPos = pos;
}
char crListControlWidgetNode::getDefaultScrollPos()
{
	return m_defaultScrollPos;
}
const CRCore::crVector4f &crListControlWidgetNode::getRect() const
{
	return m_rect;
}
void crListControlWidgetNode::setRectOffset(const CRCore::crVector4f &offset)
{
	float pixelRatio = crDisplaySettings::instance()->getUIPixelMeter();
	m_rectOffset = offset * pixelRatio;
}
const CRCore::crVector4f &crListControlWidgetNode::getRectOffset() const
{
	return m_rectOffset;
}
void crListControlWidgetNode::setVScrollBarName(const std::string &name)
{
	m_vScrollBarName = name;
}
const std::string &crListControlWidgetNode::getVScrollBarName()const
{
	return m_vScrollBarName;
}
void crListControlWidgetNode::setNodeSize(CRCore::crVector2f &nodesize)
{
	m_nodeSize = nodesize;
}
const CRCore::crVector2f &crListControlWidgetNode::getNodeSize()const
{
	return m_nodeSize;
}
void crListControlWidgetNode::setSpaceBetween(CRCore::crVector2f &space)
{
	m_spaceBetween = space;
}
const CRCore::crVector2f &crListControlWidgetNode::getSpaceBetween()const
{
	return m_spaceBetween;
}
void crListControlWidgetNode::setSelectColor(const CRCore::crVector4f &color)
{
	m_selectColor = color;
}
const CRCore::crVector4f &crListControlWidgetNode::getSelectColor()const
{
	return m_selectColor;
}
void crListControlWidgetNode::setCanDragNode(bool can)
{
	m_canDragNode = can;
}
bool crListControlWidgetNode::getCanaDragNode()
{
	return m_canDragNode;
}
void crListControlWidgetNode::setMouseMoveSelect(bool mouseMoveSelect)
{
	m_mouseMoveSelect = mouseMoveSelect;
}
CRCore::crMatrixTransform *crListControlWidgetNode::getMvpwNode()
{
	return m_mvpwNode.get();
}
//////////////////////////////////////////////////////////////////////////
//
//crScissorWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crScissorWidgetNode::crScissorWidgetNode()
{
}
crScissorWidgetNode::crScissorWidgetNode(const crScissorWidgetNode& node,const crCopyOp& copyop):
	crWidgetNode(node,copyop)//SHALLOW_COPY会存在问题
{
}
crScissorWidgetNode::~crScissorWidgetNode()
{

}
void crScissorWidgetNode::initWindow()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0],maxpos[1],maxpos[0]-minpos[0],maxpos[1]-minpos[1]);
	m_scissor = new crScissor;
	crStateSet *ss = getOrCreateStateSet();
	ss->setAttributeAndModes(m_scissor.get(),crStateAttribute::ON|crStateAttribute::OVERRIDE);
	computeScissor();
}
void crScissorWidgetNode::resize()
{
	crVector3 minpos = getBoundBox().m_min;
	crVector3 maxpos = getBoundBox().m_max;
	minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
	maxpos = crFilterRenderManager::getInstance()->getWindowPosition(maxpos);
	m_rect.set(minpos[0], maxpos[1], maxpos[0] - minpos[0], maxpos[1] - minpos[1]);
	computeScissor();
}
bool crScissorWidgetNode::computeBound() const
{
	m_boundSphere.init();
	m_bbox.init();

	if (m_childArray.empty()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_bbox.expandBy(getChild(0)->getBoundBox());

	if (!m_bbox.valid()) 
	{
		m_bBoundSphere_computed = true;
		return false;
	}

	m_boundSphere.expandBy(m_bbox);
	m_bBoundSphere_computed = true;
	return true;
}
void crScissorWidgetNode::computeScissor()
{
	if(m_scissor.valid())
	{
		crBoundingBox bbox = getBoundBox();
		//crMatrix matrix;
		//if(this->getEnableMatrix())
		//{
		//	matrix = this->getMatrix();
		//}
		//matrix.preMult(getParentCanvas()->getMatrix());
		//bbox.m_min = bbox.m_min * matrix;
		//bbox.m_max = bbox.m_max * matrix;
		//bbox.correct();
		CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
		accept(parentMatrix);
		bbox.m_max = bbox.m_max * parentMatrix.getResult();
		bbox.m_min = bbox.m_min * parentMatrix.getResult();
		bbox.correct();
		crVector3 minpos = bbox.m_min;
		minpos = crFilterRenderManager::getInstance()->getWindowPosition(minpos);
		m_scissor->setScissor(minpos[0],minpos[1],m_rect[2],m_rect[3]);
	}
}
void crScissorWidgetNode::updateData()
{
	if(m_scissor.valid())
	{
		computeScissor();
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crGroupWidgetNode
//
//////////////////////////////////////////////////////////////////////////
crGroupWidgetNode::crGroupWidgetNode()
{
}
crGroupWidgetNode::crGroupWidgetNode(const crGroupWidgetNode& node,const crCopyOp& copyop):
	crWidgetNode(node,copyop),//SHALLOW_COPY会存在问题
	m_widgetGroup(node.m_widgetGroup)
{
}
crGroupWidgetNode::~crGroupWidgetNode()
{

}
void crGroupWidgetNode::addWidgetName(const std::string &name)
{
	m_widgetGroup.push_back(WidgetNamePair(name,(crWidgetNode*)NULL));
}
void crGroupWidgetNode::initWindow()
{
	if(!m_parentCanvas) return;
	crWidgetNode *node;
	for( WidgetGroup::iterator itr = m_widgetGroup.begin();
		itr != m_widgetGroup.end();
		++itr )
	{
		node = m_parentCanvas->getWidget(itr->first);
		if(node)
		{
			itr->second = node;
			node->setParentWidget(this);
		}
		else
		{
			//CRCore::notify(CRCore::WARN)<<"crRadioGroupWidgetNode::initWindow(): RadioGroup("<<this->getName()<<") Not Find RadioElement("<<itr->first.c_str()<<")"<<std::endl;
			std::string str = "GroupWidget["+getName()+"] Widget["+itr->first+"]没有找到";
			MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
		}
	}
	//if(m_radioGroup.empty())
	//{
	//	CRCore::notify(CRCore::WARN)<<"crRadioGroupWidgetNode::initWindow(): RadioGroup("<<this->getName()<<") is empty"<<std::endl;
	//	std::string str = "RadioGroupWidget是空的["+getName()+"]";
	//	MessageBox(::GetActiveWindow(),str.c_str(),"Creature3D",MB_OK);
	//	return;
	//}
}
void crGroupWidgetNode::updateParentWidget(crWidgetNode *widget, _crInt32 msg)
{
	if(widget)
	{
		doEvent(msg,MAKECREPARAM(widget,NULL));
	}
}
crGroupWidgetNode::WidgetGroup &crGroupWidgetNode::getWidgetGroup()
{
	return m_widgetGroup;
}
crWidgetNode* crGroupWidgetNode::getWidget(int i)
{
	if(i>=0 && i<m_widgetGroup.size())
		return m_widgetGroup[i].second;
	return NULL;
}
int crGroupWidgetNode::getWidgetIndex(crWidgetNode* widget)
{
	for( int i = 0; i<m_widgetGroup.size(); i++ )
	{
		if(m_widgetGroup[i].second == widget)
			return i;
	}
	return 0;
}