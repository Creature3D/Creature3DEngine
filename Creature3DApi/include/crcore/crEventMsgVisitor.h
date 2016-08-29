/**********************************************************************
*
*	文件:	   crEventMsgVisitor.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
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