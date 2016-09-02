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
#ifndef CRCORE_CREVENTGROUP_H
#define CRCORE_CREVENTGROUP_H 1

#include <CRCore\crGroup.h>
#include <CRCore\crEventMsg.h>
#include <CRCore\crEventMsgVisitor.h>
#include <CRCore/thread/crMutex.h>

#include <vector>
#include <stack>
#include <string>
#include <queue>
#include <set>
namespace CRCore{
class CR_EXPORT crEventGroup : public crGroup
{
friend class crEventMsg;
friend class crIF_ELSE_EventMsg;
friend class crSwitch_EventMsg;
friend class crWhile_EventMsg;
public :
  crEventGroup();

  crEventGroup(const crEventGroup&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

  META_Node(CRCore, crEventGroup);
 
  //enum MsgPriority
  //{
	 // LOW,
	 // STANDARD,
	 // HIGH
  //};

  struct RelayMsg : public Referenced
  {
	  ref_ptr<crEventGroup> m_senderGroup;
	  ref_ptr<crEventMsg> m_eventMsg;
	  _crInt64       m_wparam;//message parameter   一般用于传递处理对象
	  _crInt64       m_lparam;//message parameter   一般用于传递处理参数
      //MsgPriority m_msgPriority;
	 
	  //bool operator <(const ref_ptr<RelayMsg>& item) const { return  m_msgPriority <  item->m_msgPriority; }
	  //bool operator ==(const ref_ptr<RelayMsg>& item) { return  m_msgPriority == item->m_msgPriority; }
      
	  bool compare(const RelayMsg *item)const
	  {//相等返回true 否则返回false
		  return m_senderGroup == item->m_senderGroup &&
			     m_eventMsg == item->m_eventMsg &&
				 m_wparam == item->m_wparam &&
				 m_lparam == item->m_lparam;
	  }
  };

  //struct LessRelayMsg
  //{
	 // bool operator () (const CRCore::ref_ptr<RelayMsg> lhs,const CRCore::ref_ptr<RelayMsg> rhs) const
	 // {
		//  return !(lhs->compare(rhs));
	 // }
  //};
  //typedef std::pair< ref_ptr<crEventMsg>, MsgPriority > EventMsgPriorityPair;
  //typedef std::priority_queue< ref_ptr<RelayMsg> > EventMsgRelayDeque;  
  typedef std::deque< ref_ptr<RelayMsg> > RelayMsgDeque;  
  typedef std::vector< ref_ptr<RelayMsg> > RelayMsgBuffer;  
  typedef std::map<unsigned int, ref_ptr<crEventMsg> > EventMsgMap;//first msg
  typedef std::stack<unsigned long> ParamStack;

  virtual void sendEventMessage( crEventGroup *eventGroup, unsigned int msg, _crInt64 wparam = 0, _crInt64 lparam = 0/*, MsgPriority priority = STANDARD*/ );
 
  virtual bool acceptEventMessage( unsigned int msg, _crInt64 wparam = 0, _crInt64 lparam = 0/*, MsgPriority priority = STANDARD*/ );
  virtual bool acceptEventMessage( crEventGroup& eventGroup, unsigned int msg, _crInt64 wparam = 0, _crInt64 lparam = 0/*, MsgPriority priority = STANDARD*/ );

  RelayMsg* getCurrentRelayMsg(){ return m_currentRelayMsg.get(); }
  const RelayMsg* getCurrentRelayMsg()const{ return m_currentRelayMsg.get(); }
  crEvent *getCurrentEvent(){ return m_currentEvent.get(); }
  const crEvent *getCurrentEvent()const{ return m_currentEvent.get(); }
  crEventMsg *getCurrentEventMsg(){ return m_currentEventMsg.get(); }
  const crEventMsg *getCurrentEventMsg()const{ return m_currentEventMsg.get(); }
  unsigned int getCurrentMsg(){ return m_currentEventMsg.valid()?m_currentEventMsg->m_msg:0; }

  void getEventMsgRelayDeque(RelayMsgDeque &msgDeque);

  void setParam(unsigned long param){ m_param = param; }
  int getParam(){ return m_param; }
  int getParam()const{ return m_param; }

  void setChildParam(unsigned int flg){ m_childParam = flg; }
  int getChildParam(){ return m_childParam; }
  int getChildParam()const{ return m_childParam; }

  void addEventMsgMap(crEventMsg *eventMsg){ m_eventMsgMap[eventMsg->m_msg] = eventMsg; }
  EventMsgMap &getEventMsgMap(){ return m_eventMsgMap; }
  const EventMsgMap &getEventMsgMap()const{ return m_eventMsgMap; }
  const crEventMsg *getEventMsg( unsigned int msg )const
  { 
	  EventMsgMap::const_iterator itr = m_eventMsgMap.find(msg);
	  if( itr == m_eventMsgMap.end() )
		  return NULL;
	  return itr->second.get(); 
  }
  crEventMsg *getEventMsg( unsigned int msg )
  { 
	  EventMsgMap::iterator itr = m_eventMsgMap.find(msg);
	  if( itr == m_eventMsgMap.end() )
		  return NULL;
	  return itr->second.get(); 
  }
  void deleteEventMsg(unsigned int msg) { m_eventMsgMap.erase(msg); }

  void pushParamStack( unsigned long param ){ m_paramStack.push(param); }
  unsigned long topParamStack(){ return m_paramStack.top(); }
  void popParamStack(){ m_paramStack.pop(); }

  virtual bool addChild( crNode *child );
  virtual bool insertChild( unsigned int index, crNode *child );
  virtual bool replaceChild( crNode *origChild, crNode* newChild );
  virtual bool setChild( unsigned  int i, crNode* node );

  virtual void traverse(crNodeVisitor& nv);

  //class CR_EXPORT ParseParamCallback : public virtual Referenced
  //{
  //public:
	 // virtual ParseParamCallback* cloneType() const { return NULL; };
	 // virtual const char* libraryName() const { return "CRCore"; }
	 // virtual const char* className() const { return "ParseParamCallback"; }

	 // virtual bool parseParamImplementation( crEventGroup& eg, crNodeVisitor& nv ){ return false; };
  //};

  typedef std::vector< ref_ptr<ParseParamCallback> > ParseParamCallbackList;

  inline const ParseParamCallbackList& getParseParamCallback()const{ return m_parseParamCallbackList; }
  inline ParseParamCallbackList& getParseParamCallback(){ return m_parseParamCallbackList; }
  inline void addParseParamCallback( ParseParamCallback *ppc ){ m_parseParamCallbackList.push_back(ppc); }
  inline void clearParseParamCallback(){ m_parseParamCallbackList.clear(); }
  
  virtual bool attachEventGroup( CRCore::crEventGroup *eg );
  virtual bool detachEventGroup(/* CRCore::crEventGroup *eg */);
  crEventGroup *getAttachedEventGroup() { return m_attachedEventGroup; }
  const crEventGroup *getAttachedEventGroup()const { return m_attachedEventGroup; }

 // void clearRelayDeque();

  struct Msg : public Referenced
  {
	  ref_ptr<crEventGroup> receiverGroup;
	  unsigned int msg;
	  _crInt64     wparam;
	  _crInt64     lparam;
	  inline Msg& operator = (const Msg& m) { msg=m.msg; wparam=m.wparam; lparam=m.lparam; return (*this); }
  };
  typedef std::deque< ref_ptr<Msg> > MsgDeque;

  virtual void releaseObjects(CRCore::crState* state);
  //static void clearRelayMsgBuffer();
protected:
  virtual ~crEventGroup();
  
  void addRelayEvent( crEventGroup *sender, crEventMsg *emsg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority = STANDARD*/ );
  void addRelayEvent( crEventGroup::RelayMsg *relayMsg );
  //const EventMsgRelayDeque& getEventMsgRelayDeque()const{ return m_eventMsgRelayDeque; }

  virtual void relayEvent();

  virtual void doEvent(crEvent *event);
  virtual bool checkCondition( ECondition *m_econdition );

  virtual bool parseParam(crNodeVisitor& nv);
  virtual bool parseParamImplementation(crNodeVisitor& nv);

  //static RelayMsg* createOrReuseRelayMsg(crEventGroup *sender, crEventMsg *emsg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority = STANDARD*/);
  //static RelayMsgBuffer m_relayMsgBuffer;
  //static crMutex s_mutex_relayMsgBuffer;

  unsigned long        m_param;
  int                  m_childParam;

  ref_ptr<crEvent>     m_currentEvent;
  ref_ptr<RelayMsg>    m_currentRelayMsg;
  ref_ptr<crEventMsg>  m_currentEventMsg;
  
  crNodeVisitor* m_currentVisitor;

  RelayMsgDeque          m_eventMsgRelayDeque;
  //EventMsgRelayDeque   m_eventMsgRelayDeque;//消息队列
                                  //消息处理完毕后即可退栈
                                   //循环消息将根据其生命值来决定是否驻留

  EventMsgMap        m_eventMsgMap;//自身所具有的事件消息处理表
  ParamStack         m_paramStack;
  ParseParamCallbackList m_parseParamCallbackList;

  mutable CRCore::crEventGroup *m_attachedEventGroup;
  //RelayMsgList m_relayMsgList;

  crMutex m_mutex_message;
  crMutex m_mutex_relayEvent;
  crMutex m_mutex_doEvent;
};

}

#endif