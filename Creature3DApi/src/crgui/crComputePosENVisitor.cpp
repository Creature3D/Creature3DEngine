/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include <CRGUI/crComputePosENVisitor.h>

using namespace CRCore;
using namespace CRGUI;

crComputePosENVisitor::crComputePosENVisitor():
  crElementNodeVisitor(TRAVERSE_EXPAND_CHILDREN),
  m_posX(TREEPOS_X),
  m_mode(ComputeStaticPos)
{
	m_size.set(TREEPOS_X,TREEPOS_Y);
}


crComputePosENVisitor::~crComputePosENVisitor()
{

}

void crComputePosENVisitor::computeStaticPos(crElementNode& enode)
{
	int posY; 

	if(m_nodeStack.empty())
	{	
		m_posX = enode.getRect()[0];
		m_nodeStack.push(&enode);
		traverse(enode);
		m_nodeStack.pop();
		return;
	}

	posY = m_nodeStack.top()->getRect()[1]+NODESIZE_Y+SPACEPERLEVEL;

	if( m_posX > m_size.x() ) m_size.x() = m_posX;
	if( posY > m_size.y() ) m_size.y() = posY;

	enode.setPos(m_posX,posY);

	m_nodeStack.push(&enode);

	traverse(enode);

	m_nodeStack.pop();

	if(enode.getNumChildren()==0) m_posX += NODESIZE_X+SPACEPERNODE;
}

void crComputePosENVisitor::computeMovingPos(crElementNode& enode)
{
	int posY; 

	if(m_nodeStack.empty())
	{	
		m_posX = enode.getPos2().x();
		m_nodeStack.push(&enode);
		traverse(enode);
		m_nodeStack.pop();
		return;
	}

	posY = m_nodeStack.top()->getPos2().y()+NODESIZE_Y+SPACEPERLEVEL;

	enode.setPos2(m_posX,posY);

	m_nodeStack.push(&enode);

	traverse(enode);

	m_nodeStack.pop();

	if(enode.getNumChildren()==0) m_posX += NODESIZE_X+SPACEPERNODE;
}

void crComputePosENVisitor::computeMovingSize(crElementNode& enode)
{
	int posY = 0;
    
    if(m_size.x() < m_posX)
	{
		m_size.x() = m_posX;
	}

	posY = m_nodePath.size() * (NODESIZE_Y+SPACEPERLEVEL);
    if(m_size.y()  < posY)
	{
		m_size.y() = posY;
	}

	traverse(enode);

	if(enode.getNumChildren()==0) m_posX += NODESIZE_X+SPACEPERNODE;

}

void crComputePosENVisitor::resetMovingPos(crElementNode& enode)
{
	enode.initPos2();
	traverse(enode);
}

void crComputePosENVisitor::apply(crElementNode& enode)
{
	switch(m_mode) 
	{
	case ComputeStaticPos:
		computeStaticPos(enode);
		break;
	case ComputeMovingPos:
		computeMovingPos(enode);
		break;
	case ComputeMovingSize:
		computeMovingSize(enode);
	case ResetMovingPos:
		resetMovingPos(enode);
	}
}

const CRCore::crVector2i crComputePosENVisitor::getTreeSize()
{
	crVector2i temp =  m_size+crVector2i(2*NODESIZE_X+SPACEPERNODE,2*NODESIZE_Y+SPACEPERLEVEL);
	return temp;
}

const CRCore::crVector2i &crComputePosENVisitor::getRealSize()
{
	if(m_size.x() < NODESIZE_X+SPACEPERNODE)
		m_size.x() = NODESIZE_X+SPACEPERNODE;
	if(m_size.y() < NODESIZE_Y+SPACEPERLEVEL)
		m_size.y() = NODESIZE_Y+SPACEPERLEVEL;

	return m_size;
}