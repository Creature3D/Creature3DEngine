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
#ifndef CRUTIL_CRBROADCASTEVENTMSGVISITOR_H
#define CRUTIL_CRBROADCASTEVENTMSGVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crEventGroup.h>

#include <CRUtil/crExport.h>

namespace CRUtil {

class CRUTIL_EXPORT crBroadcastEventMsgVisitor : public CRCore::crNodeVisitor
{
public:
	crBroadcastEventMsgVisitor();
    virtual ~crBroadcastEventMsgVisitor();

    virtual void reset();

    virtual void apply(CRCore::crNode& node);
	virtual void apply(CRCore::crGroup& node);

    virtual void apply(CRCore::crEventGroup& node);

	inline void setMsg(unsigned int msg, unsigned long wparam = 0, unsigned long lparam = 0)
	{
		m_msg = msg;
		m_wparam = wparam;
		m_lparam = lparam;
	}

	inline unsigned int getMsg(){ return m_msg; }

	inline void setSender( CRCore::crEventGroup *eventGroup ){ m_senderGroup = eventGroup; }
	inline CRCore::crEventGroup *getSender(){ return m_senderGroup.get(); }
	
	enum BroadcastFlag
	{
		OneStop,
		Broadcast
	};

	inline void setBroadcastFlag(BroadcastFlag flag){ m_broadcastFlag = flag; }
	inline BroadcastFlag getBroadcastFlag(){ return m_broadcastFlag; }

protected:
	unsigned int m_msg;
	unsigned long m_wparam;
	unsigned long m_lparam;
	CRCore::ref_ptr<CRCore::crEventGroup> m_senderGroup;
	BroadcastFlag m_broadcastFlag;
};

}

#endif

