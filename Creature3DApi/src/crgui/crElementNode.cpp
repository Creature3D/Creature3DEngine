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
#include <CRGUI/crElementNode.h>
#include <CRGUI/crImageStage.h>
#include <CRGUI/crElementNodeVisitor.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>

using namespace CRGUI;
using namespace CRCore;

HPEN crElementNode::m_whitePen0 = CreatePen(PS_SOLID,0,RGB(255,255,255));
HPEN crElementNode::m_whitePen2 = CreatePen(PS_SOLID,2,RGB(255,255,255));
bool crElementNode::m_rectoHistory = true;
crElementNode::crElementNode():
    m_mode(EXPAND),
    m_bSelected(false)
{
}

crElementNode::crElementNode(const crElementNode& element):
    crElement(element),
    m_mode(element.m_mode),
	m_bSelected(false),
	m_hScrollBarName(element.m_hScrollBarName),
	m_vScrollBarName(element.m_vScrollBarName)
{
}

crElementNode::~crElementNode()
{
	DeleteObject(m_whitePen0);
    DeleteObject(m_whitePen2);
}

void crElementNode::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	if(cfg_script.Get("Caption", str))
		setCaption(str);

	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);
	crElement::load(cfg_script,widthScale,heightScale);
}

void crElementNode::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
}

bool crElementNode::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crElementNode::setCaption(const std::string &caption)
{
    m_caption = caption;
}

int crElementNode::drawStageCopy(Display dc, Display down_dc)
{
	int hscrollValue = 0;
	if(m_hScrollBar.valid()) hscrollValue = m_hScrollBar->getValue();
	int vscrollValue = 0;
	if(m_vScrollBar.valid()) vscrollValue = m_vScrollBar->getValue();

	HDC		mdc;
	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);	
	// draw bitmap
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	//draw data
	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	RECT rect;
	rect.left   = m_rect[0];
	rect.top    = m_rect[1];
	rect.right  = getElementRight();
	rect.bottom = getElementBottom();

	DrawText(dc,m_caption.c_str(),m_caption.length(),&rect,DT_WORDBREAK);

	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

    ///////////draw line
	HGDIOBJ oldBrush = SelectObject(dc,GetStockObject(NULL_BRUSH));
	HGDIOBJ oldPen = SelectObject(dc,m_whitePen0);

	int num = m_childs.size();
	if(num>0)
	{
		crVector2i line;
		line[0] = m_rect[0] + NODESIZE_X/2 - hscrollValue;
		line[1] = m_rect[1] + NODESIZE_Y - vscrollValue;
		MoveToEx(dc,line[0],line[1],(LPPOINT) NULL);
		line[1] += SPACEPERLEVEL;
		LineTo(dc,line[0],line[1]);
		line[1] -= SPACEPERLEVEL/2;
		MoveToEx(dc,line[0],line[1],(LPPOINT) NULL);
		if(m_mode==EXPAND)
		{
			line[0] = m_childs[num-1]->getRect()[0] + NODESIZE_X/2 - hscrollValue;
			LineTo(dc,line[0],line[1]);
		}
	}

	if(m_bSelected)
	{//drawSelecteRect
		SelectObject(dc,m_whitePen2);

		Rectangle(dc,m_rect[0] - 3 - hscrollValue,
			         m_rect[1] - 3 - vscrollValue,
					 m_rect[0] + NODESIZE_X + 3 - hscrollValue,
					 m_rect[1] + NODESIZE_Y + 3 - vscrollValue);
	}

	SelectObject(dc,oldBrush);
	SelectObject(dc,oldPen);

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crElementNode::drawStageMask(Display dc, Display down_dc)
{
	int hscrollValue = 0;
	if(m_hScrollBar.valid()) hscrollValue = m_hScrollBar->getValue();
	int vscrollValue = 0;
	if(m_vScrollBar.valid()) vscrollValue = m_vScrollBar->getValue();

	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	RECT rect;
	rect.left   = m_rect[0] - hscrollValue;
	rect.top    = m_rect[1] - vscrollValue;
	rect.right  = getElementRight();
	rect.bottom = getElementBottom();
	DrawText(dc,m_caption.c_str(),m_caption.length(),&rect,DT_WORDBREAK);
	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	///////////draw line
	HGDIOBJ oldBrush = SelectObject(dc,GetStockObject(NULL_BRUSH));
	HGDIOBJ oldPen = SelectObject(dc,m_whitePen0);

	int num = m_childs.size();
	if(num>0)
	{
		crVector2i line;
		line[0] = m_rect[0] + NODESIZE_X/2 - hscrollValue;
		line[1] = m_rect[1] + NODESIZE_Y - vscrollValue;
		MoveToEx(dc,line[0],line[1],(LPPOINT) NULL);
		line[1] += SPACEPERLEVEL;
		LineTo(dc,line[0],line[1]);
		line[1] -= SPACEPERLEVEL/2;
		MoveToEx(dc,line[0],line[1],(LPPOINT) NULL);
		if(m_mode==EXPAND)
		{
			line[0] = m_childs[num-1]->getRect()[0] + NODESIZE_X/2 - hscrollValue;
			LineTo(dc,line[0],line[1]);
		}
	}

	if(m_bSelected)
	{//drawSelecteRect
		SelectObject(dc,m_whitePen2);

		Rectangle(dc,m_rect[0] - 3 - hscrollValue,
			m_rect[1] - 3 - vscrollValue,
			m_rect[0] + NODESIZE_X + 3 - hscrollValue,
			m_rect[1] + NODESIZE_Y + 3 - vscrollValue);
	}

	SelectObject(dc,oldBrush);
	SelectObject(dc,oldPen);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

///////////
void crElementNode::addChild(crElementNode *nodeElement)
{
    m_childs.push_back(nodeElement);
	nodeElement->addParent(this);
}

void crElementNode::addParent(crElementNode *nodeElement)
{
    m_parents.push_back(nodeElement);
}

void crElementNode::setNode(CRCore::crNode *node)
{
    m_node = node;
	//setTextAttribute(getTextAttribute(*node));
	setCaption(node->getName());
}

//crTextAttribute *crElementNode::getTextAttribute(CRCore::crNode &node)
//{
//    return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crGroup &node)
//{
//    return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crLod &node)
//{
//    return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crSequence &node)
//{
//	return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crSwitch &node)
//{
//    return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crPagedLOD &node)
//{
//    return NULL;
//}
//
//crTextAttribute *crElementNode::getTextAttribute(CRCore::crObject &node)
//{
//    return NULL;
//}

void crElementNode::accept(crElementNodeVisitor& nv)
{
	nv.pushOntoNodePath(this);
	nv.apply(*this);
	nv.popFromNodePath();
}

void crElementNode::ascend(crElementNodeVisitor& nv)
{
	std::for_each(m_parents.begin(),m_parents.end(),crElementNodeAcceptOp(nv));
}

void crElementNode::traverse(crElementNodeVisitor& nv)
{	
	switch(nv.getTraversalMode())
	{
	case(crElementNodeVisitor::TRAVERSE_ALL_CHILDREN):
		std::for_each(m_childs.begin(),m_childs.end(),crElementNodeAcceptOp(nv));
		break;
	case(crElementNodeVisitor::TRAVERSE_EXPAND_CHILDREN):
		{
			if(m_mode==EXPAND)
			{
				std::for_each(m_childs.begin(),m_childs.end(),crElementNodeAcceptOp(nv));
			}
		}
	default:
		break;
	}
}

void crElementNode::setMode( Mode mode )
{
    if(m_mode != mode)
	{
        m_mode = mode; 
  //      if(m_mode == EXPAND)
		//{//computer node pos?? be done by crEditTreePreDrawCallback

		//}
	}
}

bool crElementNode::_insert(crElementNode *insertNode, int insertAt)
{
	//if( insertNode == this || !m_data->asGroup() )
	//	return false;

	//if(m_rectoHistory)
	//{
	//	crHistory *history = new crHistory;
	//	history->setModifyFlag(crHistory::MOVE);
	//	crHistory::MoveParam *modifyParam = new crHistory::MoveParam;
	//	crHistory::pushHistory(history);

	//	modifyParam->m_modifiedNode = insertNode;
	//	modifyParam->m_srcParentNode = insertNode->getParent();
	//	if(modifyParam->m_srcParentNode.valid())
	//		modifyParam->m_srcIndex = insertNode->getParent()->getChildIndex(insertNode);

	//	history->pushMoveParam(modifyParam);
	//}

	//if(insertNode->getParent())
	//{
	//	crRemoveNodeCallback *removenc = new crRemoveNodeCallback;
	//	removenc->setMovingNode(insertNode->getData());
	//	insertNode->getParent()->getData()->setUpdateCallback(removenc);
	//	insertNode->getParent()->removeChild(insertNode);
	//}

	//crInsertNodeCallback *insertnc = new crInsertNodeCallback;
	//insertnc->setMovingNode(insertNode->getData());
	//insertnc->setInsertAt(insertAt);

	//if(m_data->getUpdateCallback())
	//	m_data->getUpdateCallback()->addNestedCallback(insertnc);
	//else
	//	m_data->setUpdateCallback(insertnc);

	//this->insertChild(insertAt,insertNode);

	return true;
}

bool crElementNode::_delete()
{
	//crHistory *history;
	//if(m_rectoHistory)
	//{
	//	history = new crHistory;
	//	history->setModifyFlag(crHistory::MOVE);
	//	crHistory::pushHistory(history);
	//}

	//crEditNode *selectedENode = NULL;

	//for( Creature3DHandler::SelectedNodeList::iterator itr = Creature3DHandler::getInstance()->getSelectedNodes()->begin();
	//	itr != Creature3DHandler::getInstance()->getSelectedNodes()->end();
	//	++itr )
	//{
	//	selectedENode = itr->get();

	//	if(m_rectoHistory)
	//	{
	//		crHistory::MoveParam *modifyParam = new crHistory::MoveParam;
	//		modifyParam->m_modifiedNode = selectedENode;
	//		modifyParam->m_srcParentNode = selectedENode->getParent();
	//		if(modifyParam->m_srcParentNode.valid())
	//			modifyParam->m_srcIndex = selectedENode->getParent()->getChildIndex(selectedENode);
	//		history->pushMoveParam(modifyParam);
	//	}

	//	crRemoveNodeCallback *removenc = new crRemoveNodeCallback;
	//	removenc->setMovingNode(selectedENode->getData());

	//	if(selectedENode->getParent()->getData()->getUpdateCallback())
	//		selectedENode->getParent()->getData()->getUpdateCallback()->addNestedCallback(removenc);
	//	else
	//		selectedENode->getParent()->getData()->setUpdateCallback(removenc);

	//	//selectedENode->getParent()->getData()->setUpdateCallback(removenc);
	//	selectedENode->getParent()->removeChild(selectedENode);
	//}

	//Creature3DHandler::getInstance()->resetSelect();

	return true;
}

bool crElementNode::_replace(CRCore::crNode *sampleNode, bool isAll)
{//未完全正确完成
	//crEditNode *selectedENode = NULL;

	//for( Creature3DHandler::SelectedNodeList::iterator itr = Creature3DHandler::getInstance()->getSelectedNodes()->begin();
	//	itr != Creature3DHandler::getInstance()->getSelectedNodes()->end();
	//	++itr )
	//{
	//	selectedENode = itr->get();

	//	crReplaceNodeCallback *replacenc = new crReplaceNodeCallback;
	//	replacenc->setOrigNode(selectedENode->getData());
	//	CRCore::crNode *newNode = (crNode*)(sampleNode->cloneType());
	//	replacenc->setNewNode(newNode);
	//	if(isAll)
	//	{
	//		for( CRCore::crNode::ParentArray::iterator itr = m_data->getParents().begin();
	//			itr != m_data->getParents().end();
	//			++itr)
	//		{
	//			(*itr)->setUpdateCallback(replacenc);
	//		}
	//		///////未完全正确完成，应该把所有的crEditNode的m_data替换成newNode
	//		m_data = newNode;
	//	}
	//	else
	//	{
	//		this->getParent()->getData()->setUpdateCallback(replacenc);
	//		m_data = newNode;
	//	}
	//}
	return true;
}


