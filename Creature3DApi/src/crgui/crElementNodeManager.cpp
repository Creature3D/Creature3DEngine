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
#include <CRGUI/crElementNodeManager.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crNode.h>
#include <CRCore/crBrain.h>
#include <CRCore/crArgumentParser.h>
using namespace CRGUI;
using namespace CRCore;
ref_ptr<crElementNodeManager> crElementNodeManager::m_instance = NULL;

crElementNodeManager::crElementNodeManager():
    m_modifyFlag(false)
{
	m_selectedNodes = new SelectedNodeList;
	m_lastSelectedNodes = new SelectedNodeList;
}

crElementNodeManager::~crElementNodeManager()
{
	//delete m_mainDlg;
}

crElementNodeManager* crElementNodeManager::getInstance()
{
	if (m_instance==NULL)
	{
		m_instance = new crElementNodeManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crElementNodeManager::clear()
{//crElementNodeManager 不需要重新载入
	m_instance = NULL;
}

bool crElementNodeManager::validToReSelect(const CRCore::crVector2i& point)
{
	for( SelectedNodeList::iterator itr = m_selectedNodes->begin();
		itr != m_selectedNodes->end();
		++itr )
	{
		if( point.x() >= (*itr)->getRect().x() && 
			point.x() <= (*itr)->getRect().x() + NODESIZE_X &&
			point.y() >= (*itr)->getRect().y() &&
			point.y() <= (*itr)->getRect().y() + NODESIZE_Y)

			return false;
	}
	return true;
}

void crElementNodeManager::initSelect()
{
	m_lastSelectedNodes->clear();

	//for( SelectedNodeList::iterator itr = m_selectedNodes->begin();
	//	itr != m_selectedNodes->end();
	//	++itr )
	//{
	//	(*itr)->select(false);
	//}

	clearSelectBox(m_selectedNodes);

	SelectedNodeList *temp;
	temp = m_selectedNodes;
	m_selectedNodes = m_lastSelectedNodes;
	m_lastSelectedNodes = temp;
}

void crElementNodeManager::select(CRGUI::crElementNode* enode)
{ 
	m_selectedNodes->push_back(enode);
	enode->select(true);
	showSelectBox(enode);
}

void crElementNodeManager::resetSelect()
{
	if(m_selectedNodes) 
	{
		clearSelectBox(m_selectedNodes);
		m_selectedNodes->clear();
	}
	if(m_lastSelectedNodes) 
	{
		m_lastSelectedNodes->clear();
	}
}

void crElementNodeManager::clearSelectBox(  SelectedNodeList *selectedNodes )
{
	//for( SelectedNodeList::iterator enodeitr = selectedNodes->begin();
	//	enodeitr != selectedNodes->end();
	//	++enodeitr )
	//{
	//	(*enodeitr)->select(false);

	//	(*enodeitr)->getData()->showBoundingBox(false);
	//}
}

void crElementNodeManager::showSelectBox( crElementNode* enode )
{
	//enode->getData()->showBoundingBox(true);
}

bool crElementNodeManager::isNodeBeSelected(crElementNode *enode)
{
	for( SelectedNodeList::iterator itr = m_selectedNodes->begin();
		itr != m_selectedNodes->end();
		++itr )
	{
		if((*itr) == enode) return true;
	}

	return false;
}

void crElementNodeManager::reset()
{
	resetSelect();
	m_copyedNodes.clear();
	m_modifyFlag = false;
}

void crElementNodeManager::copySelectedNodes()
{
	m_copyedNodes.clear();
	crElementNode *eNode;
	m_copyedNodes.reserve(m_selectedNodes->size());
	for( SelectedNodeList::iterator itr = m_selectedNodes->begin();
		itr != m_selectedNodes->end();
		++itr )
	{
		//crElementNode *editNode = new crElementNode((CRCore::crNode*)((*itr)->getData()->clone(CRCore::crCopyOp::SHALLOW_COPY)));
		eNode = dynamic_cast<crElementNode *>((*itr)->clone(CRCore::crCopyOp::DEEP_COPY_ALL));
		m_copyedNodes.push_back(eNode);
	}
}

void crElementNodeManager::modify( bool modifyFlag ) 
{ 
	m_modifyFlag = modifyFlag; 
}

bool crElementNodeManager::isModify()
{ 
	return m_modifyFlag; 
}
