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
#include <CRCore/crEventGroup.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crNotify.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crTransform.h>

using namespace CRCore;

//#ifndef MAXEVENTRELAYSIZE
//#define MAXEVENTRELAYSIZE 500
//#endif

//crEventGroup::RelayMsgBuffer crEventGroup::m_relayMsgBuffer;
//crMutex   crEventGroup::s_mutex_relayMsgBuffer;

#define  COMPARE_Condition(condition,group) \
{\
	if(condition->m_param!=group->m_param) return false;\
	if(condition->m_childParam!=-1&&condition->m_childParam!=group->m_childParam) return false;\
}//m_childParam=-1表示不做显示状态判断
//-1表示保持当前显示状态
//-2表示全部显示
crEventGroup::crEventGroup():
	m_param(0),
	m_childParam(-2)
{
	setNumChildrenRequiringUpdateTraversal(1);
	m_dataVariance = DYNAMIC;
	m_attachedEventGroup = NULL;
} 

crEventGroup::crEventGroup(const crEventGroup& group,const crCopyOp& copyop):
    crGroup(group,copyop),
    m_param(group.m_param),
	m_childParam(group.m_childParam),
	m_attachedEventGroup(group.m_attachedEventGroup)
{
	//m_eventMsgRelayDeque.clear();
	//m_eventMsgRelayDeque.insert(m_eventMsgRelayDeque.begin(),group.m_eventMsgRelayDeque.begin(),group.m_eventMsgRelayDeque.end());
	//m_eventMsgMap.clear();
	//m_eventMsgMap.insert(m_eventMsgMap.begin(),group.m_eventMsgMap.begin(),group.m_eventMsgMap.end());

	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crEventGroup::~crEventGroup()
{
}

//void crEventGroup::clearRelayMsgBuffer()
//{
//    m_relayMsgBuffer.clear();
//}

void crEventGroup::sendEventMessage( crEventGroup *eventGroup, unsigned int msg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority*/ )
{
	//try
	//{
		eventGroup->acceptEventMessage( *this, msg, wparam, lparam );
	//}
	//catch (...) 
	//{
	//	notify(WARN)<<"WARNING: in CRCore::crEventGroup::sendEventMessage( crEventGroup *eventGroup, unsigned int msg, _crInt64 wparam, _crInt64 lparam, MsgPriority priority )"<<
	//		"sendto:"<<eventGroup<<"msg:"<<msg<<"wparam:"<<wparam<<"lparam:"<<lparam<<"priority:"<<priority<<std::endl;
	//}
}

bool crEventGroup::acceptEventMessage( unsigned int msg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority */)
{
	//解析msg
	EventMsgMap::iterator itr = m_eventMsgMap.find(msg);

	if (itr==m_eventMsgMap.end()) return false;
    
	crEventMsg *eventMsg = itr->second.get();

	if(!eventMsg) return false;

	addRelayEvent(NULL,eventMsg,wparam,lparam/*,priority*/);

	return true;
}

bool crEventGroup::acceptEventMessage( crEventGroup& eventGroup, unsigned int msg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority*/ )
{
    //解析msg
	EventMsgMap::iterator itr = m_eventMsgMap.find(msg);

	if (itr==m_eventMsgMap.end()) return false;

	crEventMsg *eventMsg = itr->second.get();

	if(!eventMsg) return false;
    
    addRelayEvent(&eventGroup,eventMsg,wparam,lparam/*,priority*/);

    return true;
}

bool crEventGroup::checkCondition( ECondition* econdition )
{
	if(!econdition) return false;

	COMPARE_Condition(econdition,this);

	crSearchNodeBYNameVisitor searchNodeBYNameVisitor;
	for( ECondition::ConditionList::iterator itr = econdition->m_exterParams.begin();
		itr != econdition->m_exterParams.end();
		++itr)
	{
		if(!(*itr)->m_eventGroup.valid())
		{
			searchNodeBYNameVisitor.reset();
			searchNodeBYNameVisitor.setNameId((*itr)->m_groupNameID);
			searchNodeBYNameVisitor.setSearchNodeType(EVENTGROUP);
			//for(int i=0; i<crBrain::getInstance()->getDBCount();i++)
			//{
				crBrain::getInstance()->getCurrentActiveDB()->accept(searchNodeBYNameVisitor);
				if(searchNodeBYNameVisitor.getResult())
				{
					(*itr)->m_eventGroup = dynamic_cast<crEventGroup*>(searchNodeBYNameVisitor.getResult());
					break;
				}
			//}
		}

		if(!(*itr)->m_eventGroup.valid())
		{
			notify(WARN)<<"Warning: in CRCore::crIF_ELSE_EventMsg::checkCondition(const crEventMsgVisitor& ev): eventGroup invalid"<<std::endl;
		}

		COMPARE_Condition(econdition,(*itr)->m_eventGroup);
	}	

	return true;
}

//crEventGroup::RelayMsg* crEventGroup::createOrReuseRelayMsg(crEventGroup *sender, crEventMsg *emsg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority*/ )
//{
//	CRCore::ScopedLock<crMutex> lock(s_mutex_relayMsgBuffer);
//	CRCore::ref_ptr<RelayMsg> idle = NULL;
//    for(RelayMsgBuffer::iterator itr=m_relayMsgBuffer.begin();
//		itr!=m_relayMsgBuffer.end();
//		++itr)
//	{
//		if((*itr)->referenceCount()==1)
//		{
//			idle = itr->get();
//			break;
//		}
//	}
//	if(!idle.valid())
//	{
//		idle = new RelayMsg;
//        m_relayMsgBuffer.push_back(idle);
//	}
//
//	idle->m_senderGroup = sender;
//	idle->m_eventMsg = emsg;
//	idle->m_wparam = wparam;
//	idle->m_lparam = lparam;
//	//idle->m_msgPriority = priority;
//	//m_relayMsgBuffer.push_back(idle);
//	//notify(WARN)<<"createOrReuseRelayMsg size = "<<m_relayMsgBuffer.size()<<std::endl;
//
//	return idle.get();
//}

void crEventGroup::addRelayEvent( crEventGroup *sender, crEventMsg *emsg, _crInt64 wparam, _crInt64 lparam/*, MsgPriority priority*/ )
{
	RelayMsg *rmsg = new RelayMsg;
	rmsg->m_senderGroup = sender;
	rmsg->m_eventMsg = emsg;
	rmsg->m_wparam = wparam;
	rmsg->m_lparam = lparam;
	addRelayEvent( rmsg );//crEventGroup::createOrReuseRelayMsg(sender,emsg,wparam,lparam/*,priority*/)
}

void crEventGroup::addRelayEvent( RelayMsg* relayMsg )
{
	if(!relayMsg)
	{
		return;
	}

	//CRCore::ScopedLock<crMutex> lock(m_mutex_message);

	//for( RelayMsgList::iterator itr = m_relayMsgList.begin();
	//	 itr != m_relayMsgList.end();
	//	 ++itr )
	//{
	//	if((*itr)->compare(relayMsg.get()))
	//		return;
	//}

	m_mutex_message.lock();
	//if(m_eventMsgRelayDeque.size()<MAXEVENTRELAYSIZE)
	//{
	//	m_eventMsgRelayDeque.push_back(relayMsg);
	//	//m_relayMsgList.push_back(relayMsg);
	//}
	//else
	//{
	//	//m_eventMsgRelayDeque.pop_front();
 //       m_eventMsgRelayDeque.push_back(relayMsg);
	//	//notify(WARN)<<"WARNING: in crEventGroup::addRelayEvent(): 超出队列 "<<this->getName()<<" "<<this->className()<<" size = "<<m_eventMsgRelayDeque.size()<<std::endl;
	//}
	m_eventMsgRelayDeque.push_back(relayMsg);
    m_mutex_message.unlock();

	//if(m_eventMsgRelayDeque.size()<MAXEVENTRELAYSIZE)
	//	m_eventMsgRelayDeque.insert(relayMsg);

//	if(m_eventMsgRelayDeque.empty()) 
//	{
//		m_eventMsgRelayDeque.push(relayMsg);
//		return;
//	}
//
//#ifndef MAXEVENTRELAYSIZE
//#define MAXEVENTRELAYSIZE 200
//#endif
//
//    while(m_eventMsgRelayDeque.size()>MAXEVENTRELAYSIZE)
//	{
//		m_eventMsgRelayDeque.pop();//如果信息阻塞超过队列最大值则释放队尾的语义消息。
//		//CRCore::crThread::yieldCurrentThread();
//	}
//
//	m_eventMsgRelayDeque.push(relayMsg);
	//EventMsgRelayDeque::iterator itr = m_eventMsgRelayDeque.end();

	//for(;itr != m_eventMsgRelayDeque.begin();
	//	--itr)
	//{
	//	if((*itr)->m_msgPriority>=relayMsg->m_msgPriority)
	//	{
	//		m_eventMsgRelayDeque.insert(++itr,relayMsg);
	//		return;
	//	}
	//}

	//if((*itr)->m_msgPriority>=relayMsg->m_msgPriority)
	//	m_eventMsgRelayDeque.insert(++itr,relayMsg);
	//else
	//    m_eventMsgRelayDeque.push_front(relayMsg);
}

//void crEventGroup::clearRelayDeque()
//{
//    m_eventMsgRelayDeque.clear();
//}

void crEventGroup::getEventMsgRelayDeque(RelayMsgDeque &msgDeque)
{
	m_mutex_message.lock();
	msgDeque = m_eventMsgRelayDeque;
	m_mutex_message.unlock();
}

void crEventGroup::relayEvent()
{//所有消息在当前处理完
	//try{
	//notify(WARN)<<"crEventGroup::relayEvent() "<<this->getName()<<std::endl;
		//CRCore::ScopedLock<crMutex> lock(m_mutex_message);

		//for( EventMsgRelayDeque::iterator itr = m_eventMsgRelayDeque.begin();
		//	m_eventMsgRelayDeque.size()>0;
		//	++itr)
		//{
		//	m_currentRelayMsg = *itr;

		//	eventMsgVisitor.setSender(m_currentRelayMsg->m_senderGroup.get());
		//	eventMsgVisitor.setReceiver(this);
		//	m_currentEventMsg = m_currentRelayMsg->m_eventMsg;

		//	//event input param
		//	if( /*emsg->m_event->m_param == WCH_INPUTPARAM &&*/
		//		LOINT64(m_currentRelayMsg->m_lparam) == WCH_INPUTPARAM )
		//	{
		//		m_currentEventMsg->m_event->m_param = HIINT64(m_currentRelayMsg->m_lparam);
		//	}

		//	eventMsgVisitor.setMsg(m_currentEventMsg->m_msg);
		//	m_currentEventMsg->accept(eventMsgVisitor);

		//	if(m_currentEventMsg->getMsgType() == crEventMsg::WHILE_EVENTMSG)
		//	{
		//		if(!((crWhile_EventMsg*)(m_currentEventMsg.get()))->dead())
		//			continue;//需要在下一个周期继续处理循环消息，知道循环结束
		//	}

		//	m_eventMsgRelayDeque.pop();
		//}
/////////////////////////////priority_queue
		//std::vector< ref_ptr<RelayMsg> > while_msgs;
		//
		//while(!m_eventMsgRelayDeque.empty())
		//{
		//	m_currentRelayMsg = m_eventMsgRelayDeque.top();
  //          if(!m_currentRelayMsg) 
		//	{
		//	    m_eventMsgRelayDeque.pop();			
		//		break;
		//	}
		//	eventMsgVisitor.setSender(m_currentRelayMsg->m_senderGroup.get());
		//	eventMsgVisitor.setReceiver(this);
		//	m_currentEventMsg = m_currentRelayMsg->m_eventMsg;

		//	//event input param
		//	if( /*emsg->m_event->m_param == WCH_INPUTPARAM &&*/
		//		LOINT64(m_currentRelayMsg->m_lparam) == WCH_INPUTPARAM )
		//	{
		//		m_currentEventMsg->m_event->m_param = HIINT64(m_currentRelayMsg->m_lparam);
		//	}

		//	eventMsgVisitor.setMsg(m_currentEventMsg->m_msg);
		//	m_currentEventMsg->accept(eventMsgVisitor);

		//	if(m_currentEventMsg->getMsgType() == crEventMsg::WHILE_EVENTMSG)
		//	{
		//		if(!((crWhile_EventMsg*)(m_currentEventMsg.get()))->dead())
		//		{   
		//			while_msgs.push_back(m_eventMsgRelayDeque.top());
		//		}
		//	}

		//	m_eventMsgRelayDeque.pop();
		//}
	 //   
		//int size = m_relayMsgList.size();
		//m_relayMsgList.clear();
		//m_relayMsgList.reserve(size);
		//for(int i = 0; i<while_msgs.size(); i++)
		//{
		//	m_eventMsgRelayDeque.push(while_msgs[i]);
		//	m_relayMsgList.push_back(while_msgs[i]);
		//}
////////////////////////////////priority_queue

		//std::vector< ref_ptr<RelayMsg> > while_msgs;
    m_mutex_message.lock();
	RelayMsgDeque relayDeque;
	if(m_eventMsgRelayDeque.empty()) 
	{
		m_mutex_message.unlock();
		return;
	}
    m_eventMsgRelayDeque.swap(relayDeque);
    m_mutex_message.unlock();

    CRCore::ScopedLock<crMutex> lock(m_mutex_relayEvent);
	crEventMsgVisitor eventMsgVisitor;
	while(!relayDeque.empty())
	{
		m_currentRelayMsg = relayDeque.front();
		if(!m_currentRelayMsg) 
		{
			relayDeque.pop_front();			
			continue;
		}
		eventMsgVisitor.setSender(m_currentRelayMsg->m_senderGroup.get());
		eventMsgVisitor.setReceiver(this);
		m_currentEventMsg = m_currentRelayMsg->m_eventMsg;

		//event input param
		if( /*emsg->m_event->m_param == WCH_INPUTPARAM &&*/
			LOINT64(m_currentRelayMsg->m_lparam) == WCH_INPUTPARAM )
		{
			m_currentEventMsg->m_event->m_param = HIINT64(m_currentRelayMsg->m_lparam);
		}

		eventMsgVisitor.setMsg(m_currentEventMsg->m_msg);
		m_currentEventMsg->accept(eventMsgVisitor);

		if(m_currentEventMsg->getMsgType() == crEventMsg::WHILE_EVENTMSG)
		{
			if(!((crWhile_EventMsg*)(m_currentEventMsg.get()))->dead())
			{   
				addRelayEvent(m_currentRelayMsg.get());
			}
		}

		relayDeque.pop_front();
	}
		//notify(WARN)<<"WARNING: in crEventGroup::relayEvent(): "<<this->getName()<<" 剩余"<<m_eventMsgRelayDeque.size()<<std::endl;

		//int size = m_relayMsgList.size();
		//m_relayMsgList.clear();
		//m_relayMsgList.reserve(size);
		//for(int i = 0; i<while_msgs.size(); i++)
		//{
		//	m_eventMsgRelayDeque.push(while_msgs[i]);
		//	m_relayMsgList.push_back(while_msgs[i]);
		//}

	//}
	//catch (...) 
	//{
	//	notify(WARN)<<"WARNING: in crEventGroup::relayEvent(): EventMsgRelayDeque size:"<<m_eventMsgRelayDeque.size()<<
 //                    "LastRelayMsg:"<<m_currentEventMsg->m_msg<<
	//				 "LastMsgSender:"<<m_currentRelayMsg->m_senderGroup.get()<<std::endl;

	//	//while(!m_eventMsgRelayDeque.empty())
	//	//    m_eventMsgRelayDeque.pop();
	//}
}

void crEventGroup::doEvent(crEvent *event)
{
	if(!event) return;

	CRCore::ScopedLock<crMutex> lock(m_mutex_doEvent);
	m_currentEvent = event;
	m_param = event->m_param;

	if(event->m_childParam!=-1)
        m_childParam = event->m_childParam;

	crSearchNodeBYNameVisitor searchNodeBYNameVisitor;
	for( crEvent::SendMsgList::iterator itr = event->m_sendMsgList.begin();
		itr != event->m_sendMsgList.end();
		++itr)
	{
		if(!(*itr)->m_eventGroup.valid())
		{
		    searchNodeBYNameVisitor.reset();
			searchNodeBYNameVisitor.setNameId((*itr)->m_groupNameID);
			searchNodeBYNameVisitor.setSearchNodeType(EVENTGROUP);
			//for(int i=0; i<crBrain::getInstance()->getDBCount();i++)
			{
				crBrain::getInstance()->getCurrentActiveDB()->accept(searchNodeBYNameVisitor);
				if(searchNodeBYNameVisitor.getResult())
				{
					(*itr)->m_eventGroup = dynamic_cast<crEventGroup*>(searchNodeBYNameVisitor.getResult());
					break;
				}
			}
		}

		if(!(*itr)->m_eventGroup.valid())
		{
			notify(WARN)<<"Warning: in CRCore::crEventMsg::traverse(crEventMsgVisitor& ev): eventGroup invalid"<<std::endl;
		}

		sendEventMessage((*itr)->m_eventGroup.get(),(*itr)->m_msg, (unsigned long)event );
	}

	parseParam(*m_currentVisitor);

}

bool crEventGroup::addChild( crNode *child )
{
//	return crEventGroup::insertChild( m_childArray.size(), child );
	return crGroup::insertChild( m_childArray.size(), child );
}

bool crEventGroup::insertChild( unsigned int index, crNode *child )
{
	//if( !dynamic_cast<crTransform *>(child) && 
	//	!dynamic_cast<crSwitch *>(child) &&
	//	!dynamic_cast<crEventGroup *>(child) ) return false;

	return crGroup::insertChild(index, child);
}

bool crEventGroup::replaceChild( crNode *origNode, crNode *newNode )
{
	//if( !dynamic_cast<crTransform *>(newNode) && 
	//	!dynamic_cast<crSwitch *>(newNode) &&
	//	!dynamic_cast<crEventGroup *>(newNode) ) return false;

	return crGroup::replaceChild(origNode, newNode);
}

bool crEventGroup::setChild( unsigned  int i, crNode* newNode )
{
	//if( !dynamic_cast<crTransform *>(newNode) && 
	//	!dynamic_cast<crSwitch *>(newNode) &&
	//	!dynamic_cast<crEventGroup *>(newNode) ) return false;

	return crGroup::setChild(i, newNode);
}

void crEventGroup::traverse(crNodeVisitor& nv)
{
//try{
	if(nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		m_currentVisitor = &nv;
		relayEvent();
		
		if(m_childParam>=0&&m_childParam<m_childArray.size())
			m_childArray[m_childParam]->accept(nv);
		else
		{
			crGroup::traverse(nv);
		}

		return;
	}
/*   if (nv.getTraversalMode()==crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN)
	{
		parseParam(nv);
		if(m_childParam>=0&&m_childParam<m_childArray.size())
			m_childArray[m_childParam]->accept(nv);
		else
		{
			crGroup::traverse(nv);
		}
	}*/
	else
	{
		crGroup::traverse(nv);
	}
//}
//catch (...)
//{
//	notify(FATAL)<<"crEventGroup traverse error nodename = "<<getName()<<" childNum "<<getNumChildren()<<" child0 = "<<getChild(0)<<std::endl;
//	for(int i = 0; i<getNumChildren();++i)
//	    notify(FATAL)<<"crEventGroup traverse error childName = "<<getChild(i)->getName()<<std::endl;
//}
}

bool crEventGroup::parseParam(crNodeVisitor& nv)
{
#ifdef _DEBUG
	try
	{
#endif
		if(!m_currentEvent.valid()) return false;
	    
		if(m_currentEvent->parseParam(*this,nv))
			return true;

		if( m_parseParamCallbackList.size() > 0 )
		{	
			ParseParamCallbackList::iterator iter;
			for(iter=m_parseParamCallbackList.begin();iter!=m_parseParamCallbackList.end();iter++)
			{
				if((*iter)->parseParamImplementation(*this,nv))
					return true;
			}
		}
		return parseParamImplementation(nv);
#ifdef _DEBUG
	}
	catch (...) 
	{
		notify(WARN)<<"WARN: in crEventGroup::parseParam(crNodeVisitor& nv): "<<this->getName()<<std::endl;
        return false;
	}
#endif
}

bool crEventGroup::parseParamImplementation(crNodeVisitor& nv)
{
	return false;
}

bool crEventGroup::attachEventGroup( CRCore::crEventGroup *eg )
{
	m_attachedEventGroup = eg;
	return true;
}

bool crEventGroup::detachEventGroup(/* CRCore::crEventGroup *eg */)
{
	m_attachedEventGroup = NULL;
	return true;
}

void crEventGroup::releaseObjects(CRCore::crState* state)
{
	if(m_attachedEventGroup)
	{
		if(this == m_attachedEventGroup->m_attachedEventGroup)
			m_attachedEventGroup->m_attachedEventGroup = NULL;
		m_attachedEventGroup->releaseObjects(state);
		m_attachedEventGroup = NULL;
	}

	crGroup::releaseObjects(state);
}
