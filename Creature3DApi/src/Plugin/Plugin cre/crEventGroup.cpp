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

#include <Plugin cre/crException.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crEventGroup.h>
using namespace cre;

void crEventGroup::write(crDataOutputStream* out){
    out->writeInt(CREEVENTGROUP);
    CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
    if(node){
        static_cast<cre::crGroup*>(node)->write(out);
    }
    else
        throw crException("crEventGroup::write(): Could not cast this CRCore::crEventGroup to an CRCore::crGroup.");

	//从脚本解析
	//out->writeULong(getParam());
	//out->writeInt(getChildParam());
 //   
	//out->writeInt(m_eventMsgMap.size());
	//for( EventMsgMap::iterator itr = m_eventMsgMap.begin();
	//	 itr != m_eventMsgMap.end();
	//	 ++itr )
	//{
	//	out->writeUInt(itr->first);
	//	out->writeEventMsg(itr->second.get());
	//}

	//out->writeBool(m_attachedEventGroup!=0);
	//if(m_attachedEventGroup)
	//{
	//	out->writeNode(m_attachedEventGroup);
	//}

	//out->writeInt(m_parseParamCallbackList.size());
	//for( ParseParamCallbackList::iterator itr = m_parseParamCallbackList.begin();
	//	 itr != m_parseParamCallbackList.end();
	//	 ++itr )
	//{
	//	out->writePPC(itr->get());
	//}

}

void crEventGroup::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CREEVENTGROUP){
        id = in->readInt();

        CRCore::crGroup* node = dynamic_cast<CRCore::crGroup*>(this);
        if(node){
            ((cre::crGroup*)(node))->read(in);
        }
        else
            throw crException("crEventGroup::read(): Could not cast this CRCore::crEventGroup to an CRCore::crGroup.");

		//从脚本解析
		//setParam(in->readULong());
		//setChildParam(in->readInt());

		//int size, i;
		//unsigned int msg;
  //      size = in->readInt();
		//for( i = 0; i < size; i++ )
		//{
		//	msg = in->readUInt();
		//	m_eventMsgMap[msg] = in->readEventMsg();
		//}

		//if(in->readBool())
		//{
		//	m_attachedEventGroup = dynamic_cast<CRCore::crEventGroup*>(in->readNode());
		//}

		//size = in->readInt();
		//m_parseParamCallbackList.reserve(size);
		//for( i = 0; i < size; i++ ) 
		//{
		//	m_parseParamCallbackList.push_back(in->readPPC());
		//}

    }
    else{
        throw crException("crGroup::read(): Expected crGroup identification");
    }
}
