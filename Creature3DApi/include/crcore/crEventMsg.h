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
#ifndef CRCORE_CREVENTMSG_H
#define CRCORE_CREVENTMSG_H 1

#include <CRCore/crBrain.h>
#include <CRCore/crWchMsg.h>
#include <vector>
#include <string>

namespace CRCore{

class crEventGroup;

class crEventMsgVisitor;

class CR_EXPORT Condition : public Referenced
{
public:
	Condition(unsigned long p1 = 0,int p2 = 0):m_param(p1),m_childParam(p2){}
	std::string	           m_groupNameID;
    ref_ptr<crEventGroup>  m_eventGroup;
	unsigned long          m_param;
	int                    m_childParam;
};

class CR_EXPORT SendMsg : public Referenced
{
public:
	SendMsg(unsigned int msg = 0):m_msg(msg){}
	std::string	           m_groupNameID;
	ref_ptr<crEventGroup>  m_eventGroup;
	unsigned int           m_msg;
};

class CR_EXPORT ECondition : public Referenced
{
public:
	ECondition(unsigned long p1 = 0,int p2 = 0):m_param(p1),m_childParam(p2){}
	//条件
	unsigned long    m_param;//自身条件参数
	int              m_childParam;
	typedef std::vector< ref_ptr<Condition> > ConditionList;
	ConditionList    m_exterParams;//外部条件
};

#define META_PPC(library,name) \
	virtual CRCore::ParseParamCallback* cloneType() const { return new name (); } \
	virtual CRCore::ParseParamCallback* clone(const CRCore::crCopyOp& copyop) const { return new name (*this,copyop); } \
	virtual bool isSameKindAs(const CRCore::ParseParamCallback* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return #name; }
class crEventMsg;
class CR_EXPORT ParseParamCallback : public Referenced
{
public:
	ParseParamCallback(){}
	ParseParamCallback(const ParseParamCallback&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY){}
	META_PPC(CRCore,ParseParamCallback)
	inline virtual bool parseParamImplementation( crEventGroup& eg, crNodeVisitor& nv ){ return false; }
	inline virtual void addParam(int i, const std::string &str){}
	inline virtual CRCore::crEventMsg *virtualMakeEventMsg(){ return NULL; }
protected:
	virtual ~ParseParamCallback();
};

class CR_EXPORT crEvent : public Referenced
{
public:
	crEvent(unsigned long p1 = 0,int p2 = 0):m_param(p1),m_childParam(p2){}

	inline crEvent& operator = (const crEvent& event) 
	{
		m_param = event.m_param;
		m_sendMsgList.clear();
		m_sendMsgList.resize(event.m_sendMsgList.size());
        m_sendMsgList.insert(m_sendMsgList.end(),event.m_sendMsgList.begin(),event.m_sendMsgList.end());
		return(*this); 
	}

	typedef std::vector< ref_ptr<SendMsg> > SendMsgList;
	//条件满足时候要处理的事件
	unsigned long 	 m_param;//自身条件参数改变
	int              m_childParam;//自身显示参数改变
	SendMsgList      m_sendMsgList;//要向相关EventGroup发送的消息列表

	virtual bool parseParam(crEventGroup& eg,crNodeVisitor& nv);
    
	inline void setParseParamCallback( ParseParamCallback *ppc ){ m_parseParamCallback = ppc; }
	inline const ParseParamCallback *getParseParamCallback()const{ return m_parseParamCallback.get(); }
	inline ParseParamCallback *getParseParamCallback(){ return m_parseParamCallback.get(); }
protected:
	virtual ~crEvent();
    ref_ptr<ParseParamCallback> m_parseParamCallback;

	virtual bool parseParamImplementation(crEventGroup& eg, crNodeVisitor& nv);
};

class CR_EXPORT crEventMsg : public Referenced
{
public:
	enum Type
	{
		EVENTMSG,
		IF_ELSE_EVENTMSG,
		SWITCH_EVENTMSG,
		WHILE_EVENTMSG
	};

	enum MsgMode
	{
		DEFAULT,
		INHERIT //用于嵌套事件，继承其父消息    
	};

	crEventMsg(unsigned int msg = 0, MsgMode msgMode = DEFAULT):m_type(EVENTMSG),m_msg(msg),m_msgMode(msgMode){}

	typedef std::vector< ref_ptr<crEventMsg> > NestedEventMsgs;

	virtual void accept(crEventMsgVisitor& ev); 

	virtual void traverse(crEventMsgVisitor& ev);

	inline Type getMsgType(){ return m_type; }

	//消息
	unsigned int	 m_msg;
	MsgMode          m_msgMode;

	ref_ptr<crEvent>   m_event;

protected:
    virtual ~crEventMsg();
	Type             m_type;
};

class CR_EXPORT crIF_ELSE_EventMsg : public crEventMsg
{
public:
	crIF_ELSE_EventMsg(unsigned int msg = 0, MsgMode msgMode = DEFAULT)
	{
		m_type = IF_ELSE_EVENTMSG;
		m_msg = msg;
		m_msgMode = msgMode;
	}

	virtual void traverse(crEventMsgVisitor& ev);

	//条件
	ref_ptr<ECondition> m_econdition;

	NestedEventMsgs m_nestedEventMsg[2];//0:true, 1:false

};

class CR_EXPORT crSwitch_EventMsg : public crEventMsg
{
public:
	crSwitch_EventMsg(unsigned int msg = 0, MsgMode msgMode = DEFAULT)
	{
		m_type = SWITCH_EVENTMSG;
		m_msg = msg;
		m_msgMode = msgMode;
	}

	virtual void traverse(crEventMsgVisitor& ev);

	typedef std::map<ref_ptr<ECondition>,NestedEventMsgs> SWITCHMAP;
	typedef std::pair<ref_ptr<ECondition>,NestedEventMsgs> SwitchPair;
	SWITCHMAP m_switchmap;
};

class CR_EXPORT crWhile_EventMsg : public crEventMsg
{
public:
	crWhile_EventMsg(unsigned int msg = 0, MsgMode msgMode = DEFAULT, int life = -1):
	  m_life(life)
	  {
		  m_type = WHILE_EVENTMSG;
		  m_msg = msg;
		  m_msgMode = msgMode;
	  }

	  virtual void traverse(crEventMsgVisitor& ev);

	  int m_life; //该循环消息的生命周期，－1：无限生命， 0：死亡， n，没循环执行一次n-1
	  inline bool dead(){ return m_life==0; }
	  inline void kill(){ m_life = 0; }
	  inline void setLife(int life){ m_life = life; }
	  inline int LOOP(){ return m_life > 0 ? --m_life : m_life; }

	  ref_ptr<ECondition> m_econdition;
	  NestedEventMsgs m_nestedEventMsg;
};

}
#endif