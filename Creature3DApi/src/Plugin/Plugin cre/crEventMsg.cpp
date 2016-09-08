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

#include <Plugin cre/crException.h>
#include <Plugin cre/crEventMsg.h>
#include <CRCore/crEventGroup.h>
using namespace cre;

void Condition::write(crDataOutputStream* out){

	out->writeInt(CRECONDITION);

	out->writeString(m_groupNameID);
	out->writeBool(m_eventGroup.valid());
	if(m_eventGroup.valid())
	{
		out->writeNode(m_eventGroup.get());
	}
	out->writeULong(m_param);
	out->writeInt(m_childParam);
}

void Condition::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CRECONDITION)
	{
		id = in->readInt();

		m_groupNameID = in->readString();
		
		if(in->readBool())
		{
			m_eventGroup = dynamic_cast<CRCore::crEventGroup *>(in->readNode());
		}
		m_param = in->readULong();
		m_childParam = in->readInt();
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void SendMsg::write(crDataOutputStream* out){

	out->writeInt(CRESENDMSG);

	out->writeString(m_groupNameID);
	out->writeBool(m_eventGroup.valid());
	if(m_eventGroup.valid())
	{
		out->writeNode(m_eventGroup.get());
	}
	out->writeUInt(m_msg);
}

void SendMsg::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CRESENDMSG)
	{
		id = in->readInt();

		m_groupNameID = in->readString();

		if(in->readBool())
		{
			m_eventGroup = dynamic_cast<CRCore::crEventGroup *>(in->readNode());
		}

		m_msg = in->readUInt();
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void ECondition::write(crDataOutputStream* out){

	out->writeInt(CREECONDITION);

	out->writeULong(m_param);
	out->writeInt(m_childParam);
	out->writeInt(m_exterParams.size());
	for( ConditionList::iterator itr = m_exterParams.begin();
		itr != m_exterParams.end();
		++itr )
	{
		out->writeCondition(itr->get());
	}
}

void ECondition::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREECONDITION)
	{
		id = in->readInt();

		m_param = in->readULong();
		m_childParam = in->readInt();

		int size = in->readInt();
		m_exterParams.reserve(size);
		for( int i = 0; i<size; i++)
		{
            m_exterParams.push_back(in->readCondition());
		}
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void crEvent::write(crDataOutputStream* out){

	out->writeInt(CREEVENT);

	out->writeULong(m_param);
	out->writeInt(m_childParam);

	out->writeInt(m_sendMsgList.size());
	for( SendMsgList::iterator itr = m_sendMsgList.begin();
		itr != m_sendMsgList.end();
		++itr )
	{
        out->writeSendMsg(itr->get());
	}
    
	out->writeBool(m_parseParamCallback.valid());
	if(m_parseParamCallback.valid())
	{
		//out->writePPC(m_parseParamCallback.get());
	}
}

void crEvent::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREEVENT)
	{
		id = in->readInt();

		m_param = in->readULong();
        m_childParam = in->readInt();

		int size = in->readInt();
		m_sendMsgList.reserve(size);
		for( int i = 0; i<size; i++)
		{
			m_sendMsgList.push_back(in->readSendMsg());
		}

		if(in->readBool())
		{
			//m_parseParamCallback = in->readPPC();
		}
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void crEventMsg::write(crDataOutputStream* out){

	out->writeInt(CREEVENTMSG);

	out->writeUInt(m_msg);
	out->writeInt(m_msgMode);
	out->writeBool(m_event.valid());
	if(m_event.valid())
		out->writeEvent(m_event.get());
}

void crEventMsg::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREEVENTMSG)
	{
		id = in->readInt();

		m_msg = in->readUInt();
		m_msgMode = (MsgMode)(in->readInt());
		if(in->readBool())
		{
			m_event = in->readEvent();
		}
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void crIF_ELSE_EventMsg::write(crDataOutputStream* out){

	out->writeInt(CREIFELSEEVENTMSG);

	CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
	if(eventMsg)
	{
		static_cast<cre::crEventMsg*>(eventMsg)->write(out);
	}
	else
		throw crException("crIF_ELSE_EventMsg::write(): Could not cast this CRCore::crIF_ELSE_EventMsg to an CRCore::crEventMsg.");

	out->writeBool(m_econdition.valid());
	if(m_econdition.valid())
		out->writeECondition(m_econdition.get());

	out->writeInt(m_nestedEventMsg[0].size());
	for( NestedEventMsgs::iterator itr = m_nestedEventMsg[0].begin();
		 itr != m_nestedEventMsg[0].end();
		 ++itr )
	{
        out->writeEventMsg(itr->get());
	}

	out->writeInt(m_nestedEventMsg[1].size());
	for( NestedEventMsgs::iterator itr = m_nestedEventMsg[1].begin();
		itr != m_nestedEventMsg[1].end();
		++itr )
	{
		out->writeEventMsg(itr->get());
	}
}

void crIF_ELSE_EventMsg::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREIFELSEEVENTMSG)
	{
		id = in->readInt();

		CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
		if(eventMsg)
		{
			((cre::crEventMsg*)(eventMsg))->read(in);
		}
		else
			throw crException("crIF_ELSE_EventMsg::read(): Could not cast this CRCore::crIF_ELSE_EventMsg to an CRCore::crEventMsg.");

		if(in->readBool())
		{
			m_econdition = in->readECondition();
		}

		int i,size;
		size = in->readInt();
		m_nestedEventMsg[0].reserve(size);
		for( i = 0; i<size; i++)
		{
			m_nestedEventMsg[0].push_back(in->readEventMsg());
		}
		size = in->readInt();
		m_nestedEventMsg[1].reserve(size);
		for( i = 0; i<size; i++)
		{
			m_nestedEventMsg[1].push_back(in->readEventMsg());
		}
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}


void crSwitch_EventMsg::write(crDataOutputStream* out){

	out->writeInt(CRESWITCHEVENTMSG);

	CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
	if(eventMsg)
	{
		static_cast<cre::crEventMsg*>(eventMsg)->write(out);
	}
	else
		throw crException("crSwitch_EventMsg::write(): Could not cast this CRCore::crSwitch_EventMsg to an CRCore::crEventMsg.");

	out->writeInt(m_switchmap.size());
	for( SWITCHMAP::iterator itr = m_switchmap.begin();
		 itr != m_switchmap.end();
		 ++itr )
	{
	    out->writeBool(itr->first.valid());
		if(itr->first.valid())
		{
			out->writeECondition(itr->first.get());
		}
		out->writeInt(itr->second.size());
		for( NestedEventMsgs::iterator nitr = itr->second.begin();
			nitr != itr->second.end();
			++nitr )
		{
			out->writeEventMsg(nitr->get());
		}
	}

}

void crSwitch_EventMsg::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CRESWITCHEVENTMSG)
	{
		id = in->readInt();

		CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
		if(eventMsg)
		{
			((cre::crEventMsg*)(eventMsg))->read(in);
		}
		else
			throw crException("crSwitch_EventMsg::read(): Could not cast this CRCore::crSwitch_EventMsg to an CRCore::crEventMsg.");

		int mapsize,vecsize,i,j;
		mapsize = in->readInt();
		CRCore::ECondition *econdition = NULL;
		NestedEventMsgs *nestedEventMsgs = NULL;
		for( i = 0; i<mapsize; i++)
		{
			if(in->readBool())
			{
                econdition = in->readECondition();
			}
            vecsize = in->readInt();
            nestedEventMsgs = new NestedEventMsgs;
			nestedEventMsgs->reserve(vecsize);
			for( j = 0; j < vecsize; j++ )
			{
				nestedEventMsgs->push_back(in->readEventMsg());
			}

			m_switchmap[econdition] = *nestedEventMsgs;

		}
	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}

void crWhile_EventMsg::write(crDataOutputStream* out){

	out->writeInt(CREWHILEEVENTMSG);

	CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
	if(eventMsg)
	{
		static_cast<cre::crEventMsg*>(eventMsg)->write(out);
	}
	else
		throw crException("crWhile_EventMsg::write(): Could not cast this CRCore::crWhile_EventMsg to an CRCore::crEventMsg.");

	out->writeInt(m_life);
	out->writeBool(m_econdition.valid());
	if(m_econdition.valid())
		out->writeECondition(m_econdition.get());
    out->writeInt(m_nestedEventMsg.size());
	for( NestedEventMsgs::iterator itr = m_nestedEventMsg.begin();
		 itr != m_nestedEventMsg.end();
		 ++itr )
	{
		out->writeEventMsg(itr->get());
	}

}

void crWhile_EventMsg::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREWHILEEVENTMSG)
	{
		id = in->readInt();
		
		CRCore::crEventMsg* eventMsg = dynamic_cast<CRCore::crEventMsg*>(this);
		if(eventMsg)
		{
			((cre::crEventMsg*)(eventMsg))->read(in);
		}
		else
			throw crException("crWhile_EventMsg::read(): Could not cast this CRCore::crWhile_EventMsg to an CRCore::crEventMsg.");
        
		m_life = in->readInt();
		if(in->readBool())
		{
			m_econdition = in->readECondition();
		}

		int size,i;
		size = in->readInt();
		m_nestedEventMsg.reserve(size);
		for( i = 0; i < size; i++ )
		{
			m_nestedEventMsg.push_back(in->readEventMsg());
		}

	}
	else
	{
		throw crException("crEvent::read(): Expected crEvent identification");
	}
}