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
#ifndef CRCORE_CREVENTMSGVISITOR_H
#define CRCORE_CREVENTMSGVISITOR_H 1

//#include <CRCore\crEventGroup.h>
#include <CRCore\crEventMsg.h>

namespace CRCore{
class crEventGroup;
class CR_EXPORT crEventMsgVisitor : public virtual Referenced
{
public:
	crEventMsgVisitor(){}
	crEventMsgVisitor(unsigned int msg):m_msg(msg){}


	virtual void apply(crEventMsg& emsg)                  { emsg.traverse(*this); }

	virtual void apply(crIF_ELSE_EventMsg& emsg)          { apply((crEventMsg&)emsg); }

	virtual void apply(crSwitch_EventMsg& emsg)           { apply((crEventMsg&)emsg); }

	virtual void apply(crWhile_EventMsg& emsg)            { apply((crEventMsg&)emsg); }

	void setMsg(unsigned int msg){ m_msg = msg; }
	unsigned int getMsg(){ return m_msg; }

	void setSender( crEventGroup *eventGroup ){ m_senderGroup = eventGroup; }
	crEventGroup *getSender(){ return m_senderGroup.get(); }

	void setReceiver( crEventGroup *eventGroup ){ m_receiverGroup = eventGroup; }
	crEventGroup *getReceiver(){ return m_receiverGroup.get(); }

protected:
	unsigned int m_msg;
	ref_ptr<crEventGroup> m_senderGroup;
	ref_ptr<crEventGroup> m_receiverGroup;

};

}
#endif