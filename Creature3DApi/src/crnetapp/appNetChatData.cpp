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

#include <CRNetApp/appNetChatData.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataParam.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStreamBuf.h>
#include <CRNet/crNetContainer.h>
#include <CRIOManager/crFileUtils.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crCookFile.h>
#include <stdio.h>
#include <sys/stat.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRNetApp;

///////////////////////////////////
//
//crNetChatData
//
/////////////////////////////////////
crNetChatData::crNetChatData():
m_myChatState(OffLine)
{
}
///////////////////////////////////
//
//crChatFriend
//
/////////////////////////////////////
crNetChatData::crChatFriend::crChatFriend():
m_playerid(0),
m_friendChatState(crNetChatData::OffLine),
m_hasNewMsg(false)
{
}
crNetChatData::crChatFriend::~crChatFriend()
{
	if(m_chatRec.valid() && m_chatRec->getBufSize()>0)
	{
		m_chatRec->seekBegin();
		char file[32];
		sprintf(file,"rec/%d/%d.cfg\0",crMyPlayerData::getInstance()->getPlayerID(),getPlayerID());

		CRIOManager::crLoadCookFile scopedLoad(file);
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);
	}
}
void crNetChatData::crChatFriend::setID(int id)
{
	m_id = id;
}
int crNetChatData::crChatFriend::getID()
{
	return m_id;
}
void crNetChatData::crChatFriend::setPlayerID(int playerid)
{
	m_playerid = playerid;
}
int crNetChatData::crChatFriend::getPlayerID()
{
	return m_playerid;
}
void crNetChatData::crChatFriend::setColumnName(const std::string& columnName)
{
	m_columnName = columnName;
}
const std::string &crNetChatData::crChatFriend::getColumnName()const
{
	return m_columnName;
}
void crNetChatData::crChatFriend::setRemark(const std::string &remark)
{
	m_remark = remark;
}
const std::string &crNetChatData::crChatFriend::getRemark() const
{
	return m_remark;
}
void crNetChatData::crChatFriend::setFriendChatState(char chatstate)
{
	m_friendChatState = chatstate;
}
char crNetChatData::crChatFriend::getFriendChatState()
{
	return m_friendChatState;
}
void crNetChatData::crChatFriend::setNickName(const std::string &nickName)
{
	m_nickName = nickName;
}

const std::string &crNetChatData::crChatFriend::getNickName() const
{
	return m_nickName;
}
void crNetChatData::crChatFriend::recChat(const std::string &str)
{
	GNE::LockMutex lock( m_recMutex );
	if(!m_chatRec.valid())
	{
		m_chatRec = new crStreamBuf;
		m_chatRec->createBuf(1024);
	}
	int size = str.length() + 4;
	m_chatRec->_writeString(str);
	if(m_chatRec->getBufSize()>crGlobalHandle::gData()->gMaxRecBufSize())
	{
		m_chatRec->seekBegin();
		char file[32];
		sprintf(file,"rec/%d/%d.cfg\0",crMyPlayerData::getInstance()->getPlayerID(),getPlayerID());
		CRIOManager::crLoadCookFile scopedLoad(file);
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);

		m_chatRec = new crStreamBuf;
		m_chatRec->createBuf(1024);
	}
}
void crNetChatData::crChatFriend::getChatRec(std::vector<std::string> &recArray)
{
	GNE::LockMutex lock( m_recMutex );
	recArray.clear();
    if(m_chatRec.valid())
	{
        m_chatRec->seekBegin();
		while (!m_chatRec->eof())
		{
			recArray.push_back(m_chatRec->_readString());
		}
	}
}
void crNetChatData::crChatFriend::loadChatRec(std::vector<std::string> &recArray)
{
	GNE::LockMutex lock( m_recMutex );
	if(!m_chatRec.valid())
	{
		m_chatRec = new crStreamBuf;
	}
	char file[32];
	sprintf(file,"rec/%d/%d.cfg\0",crMyPlayerData::getInstance()->getPlayerID(),getPlayerID());
	CRIOManager::crLoadCookFile scopedLoad(file);
	if(m_chatRec->getBufSize()>0)
	{
		m_chatRec->seekBegin();
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);
		m_chatRec->loadFromFile2(file);
	}
	else
	{
		m_chatRec->loadFromFile2(file);
	}
	recArray.clear();
	m_chatRec->seekBegin();
	while (!m_chatRec->eof())
	{
		recArray.push_back(m_chatRec->_readString());
	}
	m_chatRec->clearBuf();
	m_chatRec->createBuf(1024);
}
void crNetChatData::crChatFriend::setHasNewMsg(bool newmsg)
{
	m_hasNewMsg = newmsg;
}
bool crNetChatData::crChatFriend::getHasNewMsg()
{
	return m_hasNewMsg;
}
///////////////////////////////////
//
//crChatGroupMember
//
/////////////////////////////////////
crNetChatData::crChatGroupMember::crChatGroupMember():
m_memberid(0),
m_playerid(0),
m_memberidChatState(crNetChatData::OffLine)
{
}
void crNetChatData::crChatGroupMember::setMemberID(int memberid)
{
	m_memberid = memberid;
}
int crNetChatData::crChatGroupMember::getMemberID()
{
	return m_memberid;
}
void crNetChatData::crChatGroupMember::setPlayerID(int playerid)
{
	m_playerid = playerid;
}
int crNetChatData::crChatGroupMember::getPlayerID()
{
	return m_playerid;
}
void crNetChatData::crChatGroupMember::setMemberName(const std::string &memberName)
{
	m_memberName = memberName;
}
const std::string &crNetChatData::crChatGroupMember::getMemberName() const
{
	return m_memberName;
}
void crNetChatData::crChatGroupMember::setMemberRemark(const std::string &remark)
{
	m_memberRemark = remark;
}
const std::string &crNetChatData::crChatGroupMember::getMemberRemark() const
{
	return m_memberRemark;
}
void crNetChatData::crChatGroupMember::setMemberChatState(char chatstate)
{
	m_memberidChatState = chatstate;
}
char crNetChatData::crChatGroupMember::getMemberChatState()
{
	return m_memberidChatState;
}
void crNetChatData::crChatGroupMember::setNickName(const std::string &nickName)
{
	m_nickName = nickName;
}

const std::string &crNetChatData::crChatGroupMember::getNickName() const
{
	return m_nickName;
}
///////////////////////////////////
//
//crChatGroup
//
/////////////////////////////////////
crNetChatData::crChatGroup::crChatGroup():
m_id(0),
m_groupid(0),
m_creator(0),
m_manager1(0),
m_manager2(0),
m_manager3(0),
m_hasNewMsg(false)
{
}
crNetChatData::crChatGroup::~crChatGroup()
{
	if(m_chatRec.valid() && m_chatRec->getBufSize()>0)
	{
		m_chatRec->seekBegin();
		char file[32];
		sprintf(file,"rec/group/%d.cfg\0",getGroupID());
		CRIOManager::crLoadCookFile scopedLoad(file);
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);
	}
}
void crNetChatData::crChatGroup::setID(int id)
{
	m_id = id;
}
int crNetChatData::crChatGroup::getID()
{
	return m_id;
}
void crNetChatData::crChatGroup::setGroupID(int groupid)
{
	m_groupid = groupid;
}
int crNetChatData::crChatGroup::getGroupID()
{
	return m_groupid;
}
void crNetChatData::crChatGroup::setGroupName(const std::string &groupName)
{
	m_groupName = groupName;
}
const std::string &crNetChatData::crChatGroup::getGroupName() const
{
	return m_groupName;
}
void crNetChatData::crChatGroup::setGroupRemark(const std::string &remark)
{
	m_remark = remark;
}
const std::string &crNetChatData::crChatGroup::getGroupRemark() const
{
	return m_remark;
}
void crNetChatData::crChatGroup::setCreator(int creator)
{
	m_creator = creator;
}
int crNetChatData::crChatGroup::getCreator()
{
	return m_creator;
}
void crNetChatData::crChatGroup::setManager1(int manager)
{
	m_manager1 = manager;
}
int crNetChatData::crChatGroup::getManager1()
{
	return m_manager1;
}
void crNetChatData::crChatGroup::setManager2(int manager)
{
	m_manager2 = manager;
}
int crNetChatData::crChatGroup::getManager2()
{
	return m_manager2;
}
void crNetChatData::crChatGroup::setManager3(int manager)
{
	m_manager3 = manager;
}
int crNetChatData::crChatGroup::getManager3()
{
	return m_manager3;
}
void crNetChatData::crChatGroup::insertMember(crNetChatData::crChatGroupMember *data)
{
	if(!data) return;
	CRCore::ScopedLock<CRCore::crMutex> lock(m_memberMutex);
	m_chatMemberMap[data->getPlayerID()] = data;
}
void crNetChatData::crChatGroup::removeMember(int playerid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_memberMutex);
	m_chatMemberMap.erase(playerid);
}
void crNetChatData::crChatGroup::clearMemberMap()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_memberMutex);
	m_chatMemberMap.clear();
}
crNetChatData::crChatGroupMember *crNetChatData::crChatGroup::getMember(int playerid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_memberMutex);
	ChatMemberMap::iterator itr = m_chatMemberMap.find(playerid);
	return itr != m_chatMemberMap.end() ? itr->second.get() : NULL;
}
void crNetChatData::crChatGroup::lockMemberMap()
{
	m_memberMutex.lock();
}
crNetChatData::crChatGroup::ChatMemberMap &crNetChatData::crChatGroup::getMemberMap()
{
	return m_chatMemberMap;
}
void crNetChatData::crChatGroup::unlockMemberMap()
{
	m_memberMutex.unlock();
}
void crNetChatData::crChatGroup::recChat(const std::string &str)
{
	GNE::LockMutex lock( m_recMutex );
	if(!m_chatRec.valid())
	{
		m_chatRec = new crStreamBuf;
		m_chatRec->createBuf(1024);
	}
	int size = str.length() + 4;
	m_chatRec->_writeString(str);
	if(m_chatRec->getBufSize()>crGlobalHandle::gData()->gMaxRecBufSize())
	{
		m_chatRec->seekBegin();
		char file[32];
		sprintf(file,"rec/group/%d.cfg\0",getGroupID());
		CRIOManager::crLoadCookFile scopedLoad(file);
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);

		m_chatRec = new crStreamBuf;
		m_chatRec->createBuf(1024);
	}
}
void crNetChatData::crChatGroup::getChatRec(std::vector<std::string> &recArray)
{
	GNE::LockMutex lock( m_recMutex );
	recArray.clear();
	if(m_chatRec.valid())
	{
		m_chatRec->seekBegin();
		while (!m_chatRec->eof())
		{
			recArray.push_back(m_chatRec->_readString());
		}
	}
}
void crNetChatData::crChatGroup::loadChatRec(std::vector<std::string> &recArray)
{
	GNE::LockMutex lock( m_recMutex );
	if(!m_chatRec.valid())
	{
		m_chatRec = new crStreamBuf;
	}
	char file[32];
	sprintf(file,"rec/group/%d.cfg\0",getGroupID());
	CRIOManager::crLoadCookFile scopedLoad(file);
	if(m_chatRec->getBufSize()>0)
	{
		m_chatRec->saveToFile2(file,true);
		CRIOManager::crWriteCookFile scopedWrite(file);
		m_chatRec->loadFromFile2(file);
	}
	else
	{
		m_chatRec->loadFromFile2(file);
	}
	recArray.clear();
	m_chatRec->seekBegin();
	while (!m_chatRec->eof())
	{
		recArray.push_back(m_chatRec->_readString());
	}
	m_chatRec->clearBuf();
	m_chatRec->createBuf(1024);
}
void crNetChatData::crChatGroup::setHasNewMsg(bool newmsg)
{
	m_hasNewMsg = newmsg;
}
bool crNetChatData::crChatGroup::getHasNewMsg()
{
	return m_hasNewMsg;
}
///////////////////////////////////
//
//crNetChatData
//
/////////////////////////////////////
void crNetChatData::insertFriend(crNetChatData::crChatFriend *data)
{
	if(!data) return;
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendMutex);
	m_friendMap[data->getPlayerID()] = data;
}

void crNetChatData::removeFriend(int playerid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendMutex);
	m_friendMap.erase(playerid);
}

void crNetChatData::clearFriendMap()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendMutex);
	m_friendMap.clear();
}

crNetChatData::crChatFriend *crNetChatData::getFriend(int playerid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendMutex);
	FriendMap::iterator itr = m_friendMap.find(playerid);
	return itr != m_friendMap.end() ? itr->second.get() : NULL;
}

bool crNetChatData::isFriend(int playerid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_friendMutex);
	FriendMap::iterator itr = m_friendMap.find(playerid);
	return itr != m_friendMap.end() ? true : false;
}

void crNetChatData::lockFriendMap()
{
    m_friendMutex.lock();
}
crNetChatData::FriendMap &crNetChatData::getFriendMap()
{
	return m_friendMap;
}
void crNetChatData::unlockFriendMap()
{
	m_friendMutex.unlock();
}
void crNetChatData::insertChatGroup(crNetChatData::crChatGroup *chatGroup)
{
	if(chatGroup)
	{
	    CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
		m_chatGroupMap[chatGroup->getID()] = chatGroup;
	}
}

crNetChatData::crChatGroup *crNetChatData::getChatGroup(int playergroupid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
	ChatGroupMap::iterator itr = m_chatGroupMap.find(playergroupid);
	return itr != m_chatGroupMap.end() ? itr->second.get() : NULL;
}

crNetChatData::crChatGroup *crNetChatData::findChatGroup(int groupid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
	for( ChatGroupMap::iterator itr = m_chatGroupMap.begin();
		itr != m_chatGroupMap.end();
		++itr )
	{
		if(itr->second->getGroupID() == groupid)
		{
			return itr->second.get();
		}
	}
	return NULL;
}

crNetChatData::crChatGroup *crNetChatData::findChatGroup(const std::string &groupname)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
	for( ChatGroupMap::iterator itr = m_chatGroupMap.begin();
		 itr != m_chatGroupMap.end();
		 ++itr )
    {
		if(itr->second->getGroupName().compare(groupname) == 0)
		{
			return itr->second.get();
		}
    }
	return NULL;
}

void crNetChatData::removeChatGroup(int playergroupid)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
	m_chatGroupMap.erase(playergroupid);
}

void crNetChatData::clearGroupMap()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);	
	m_chatGroupMap.clear();
}

int crNetChatData::getNumChatGroup()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_groupMutex);
	return m_chatGroupMap.size();
}
void crNetChatData::lockGroupMap()
{
	m_groupMutex.lock();
}
crNetChatData::ChatGroupMap &crNetChatData::getChatGroupMap()
{
	return m_chatGroupMap;
}
void crNetChatData::unlockGroupMap()
{
	m_groupMutex.unlock();
}
void crNetChatData::setMyChatState(crNetChatData::ChatState chatState)
{
	m_myChatState = chatState;
}
crNetChatData::ChatState crNetChatData::getMyChatState()
{
	return m_myChatState;
}
void crNetChatData::setChatTarget(unsigned char targetType, Referenced *target)
{
	m_chatTargetType = targetType;
	m_chatTarget = target;
}
unsigned char crNetChatData::getChatTargetType()
{
	return m_chatTargetType;
}
Referenced *crNetChatData::getChatTarget()
{
    return m_chatTarget.get();
}