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
#include <CRUtil/crBroadcastEventMsgVisitor.h>

using namespace CRCore;
using namespace CRUtil;

crBroadcastEventMsgVisitor::crBroadcastEventMsgVisitor():
    crNodeVisitor(BROADCASTEVENTMSGVISITOR,TRAVERSE_ALL_CHILDREN),
    m_msg(0),
	m_broadcastFlag(Broadcast)
{
}


crBroadcastEventMsgVisitor::~crBroadcastEventMsgVisitor()
{
}


void crBroadcastEventMsgVisitor::reset()
{

}

void crBroadcastEventMsgVisitor::apply(CRCore::crNode& node)
{ 
	if (m_traversalMode==TRAVERSE_PARENTS)
		traverse(node);
	else if(node.getNumChildrenWithEventGroups()>0)
		traverse(node); 
}

void crBroadcastEventMsgVisitor::apply(CRCore::crGroup& node)      
{ 
	if (m_traversalMode==TRAVERSE_PARENTS)
		traverse(node);
	else if(node.getNumChildrenWithEventGroups()>0)
		traverse(node); 
}

void crBroadcastEventMsgVisitor::apply(CRCore::crEventGroup& node) 
{ 
	bool sendmsg = false;
	if(m_senderGroup.valid())
	    sendmsg = node.acceptEventMessage(*m_senderGroup,m_msg,m_wparam,m_lparam); 
	else
		sendmsg = node.acceptEventMessage(m_msg,m_wparam,m_lparam);

	if(sendmsg&&m_broadcastFlag == OneStop) return;

	if (m_traversalMode==TRAVERSE_PARENTS)
		traverse(node);
	else if(node.getNumChildrenWithEventGroups()>0)
		traverse(node); 
}