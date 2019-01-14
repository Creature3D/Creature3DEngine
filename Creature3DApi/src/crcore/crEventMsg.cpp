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
#include <CRCore/crEventMsg.h>
#include <CRCore/crEventMsgVisitor.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

ParseParamCallback::~ParseParamCallback()
{

}

crEvent::~crEvent()
{

}

crEventMsg::~crEventMsg()
{

}

void crEventMsg::accept(crEventMsgVisitor& ev) 
{ 
	ev.apply(*this); 
} 

void crEventMsg::traverse(crEventMsgVisitor& ev) 
{
    switch(m_msg) 
	{
    case WCH_MOVE:
		m_event->m_param = ev.getSender()->getParam();
    	break;
    case WCH_INQUIRE:
		ev.getSender()->pushParamStack(ev.getReceiver()->getParam());
    	break;
	case WCH_FEEDBACK:
		ev.getReceiver()->pushParamStack(ev.getSender()->getParam());
		break;
    }
    ev.getReceiver()->m_currentEventMsg = this;
	ev.getReceiver()->doEvent(m_event.get());
}


void crIF_ELSE_EventMsg::traverse(crEventMsgVisitor& ev) 
{
	crEventMsg::traverse(ev);

	if(ev.getReceiver()->checkCondition(m_econdition.get()))
	{  
		for(NestedEventMsgs::iterator itr = m_nestedEventMsg[0].begin();
			itr != m_nestedEventMsg[0].end();
			++itr)
		{
            (*itr)->accept(ev);
		}
	}
	else
	{
		for(NestedEventMsgs::iterator itr = m_nestedEventMsg[1].begin();
			itr != m_nestedEventMsg[1].end();
			++itr)
		{
			(*itr)->accept(ev);
		}
	}
}

void crSwitch_EventMsg::traverse(crEventMsgVisitor& ev) 
{
	crEventMsg::traverse(ev);

	for( SWITCHMAP::iterator itr = m_switchmap.begin();
		 itr != m_switchmap.end();
		 ++itr)
	{
		if(ev.getReceiver()->checkCondition(const_cast<ECondition*>(itr->first.get())))
		{
			for(NestedEventMsgs::iterator nitr = itr->second.begin();
				nitr != itr->second.end();
				++nitr)
			{
				(*nitr)->accept(ev);
			}
		
			break;
		}
	}
}

void crWhile_EventMsg::traverse(crEventMsgVisitor& ev) 
{
    //if( dead() ) return;

	crEventMsg::traverse(ev);

	for(NestedEventMsgs::iterator itr = m_nestedEventMsg.begin();
		itr != m_nestedEventMsg.end();
		++itr)
	{
		(*itr)->accept(ev);
	}
//循环分两种模式，一种是条件循环，当条件不符合的时候进入循环的生命计时器
//条件符合则一直循环
//另一种是只是生命计时器来设定，此时条件为NULL,判断返回的总为false
	if(!(ev.getReceiver()->checkCondition(m_econdition.get())))
        LOOP();
}

bool crEvent::parseParam(crEventGroup& eg,crNodeVisitor& nv)
{
    if(m_parseParamCallback.valid())
        if(m_parseParamCallback->parseParamImplementation(eg,nv))
			return true;
	return parseParamImplementation(eg,nv);
}

bool crEvent::parseParamImplementation(crEventGroup& eg, crNodeVisitor& nv)
{
	return false; 
}




