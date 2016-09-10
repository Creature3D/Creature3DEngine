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

#include <CRGUI\crHistory.h>
#include <CRGUI\crElementNode.h>
#include <CRGUI\crElementNodeManager.h>

using namespace CRGUI;

crHistory::HistoryStack crHistory::m_historyStack;

void crHistory::undo()
{
	crElementNode::recToHistory(false);

    crHistory *history = m_historyStack.top().get();
	
	crElementNodeManager *ch = crElementNodeManager::getInstance();
	ch->resetSelect();

	if(history->m_modifyFlag == MOVE)
	{
		for( MoveParamList::iterator mpitr = history->m_moveParams.begin();
			 mpitr != history->m_moveParams.end();
			 ++mpitr )
		{
            ch->select((*mpitr)->m_modifiedNode.get());
			if((*mpitr)->m_srcParentNode.valid())
                (*mpitr)->m_srcParentNode->_insert((*mpitr)->m_modifiedNode.get(),(*mpitr)->m_srcIndex);
			else
			{
				crElementNode::_delete();
			}
		}
	}

    m_historyStack.pop();

	crElementNode::recToHistory(true);
}

void crHistory::pushHistory( crHistory *history )
{
    m_historyStack.push(history);
	crElementNodeManager::getInstance()->modify(true);
}

void crHistory::popHistory()
{ 
	m_historyStack.pop(); 
	if( m_historyStack.size() == 0 )
	{
		crElementNodeManager::getInstance()->modify(false);
	}
}
