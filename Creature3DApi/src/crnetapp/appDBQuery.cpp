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
#include <CRNetApp/appDBQuery.h>

using namespace CRCore;
using namespace CRDataBase;
using namespace CRNetApp;

/////////////////////////////////////////
//
//crLoginQueryData
//
/////////////////////////////////////////
crLoginQueryData::crLoginQueryData():
m_id(0),
//m_chatdbid(0),
//m_sex(0),
//m_bloodtype(0),
//m_work(0),
//m_beFriendValid(0),
m_lastserverid(0),
m_locked(0)
{
}
crLoginQueryData::crLoginQueryData(const crLoginQueryData& data):
crQueryData(data),
m_id(data.m_id),
//m_nickname(data.m_nickname),
//m_relname(data.m_relname),
//m_chatdbid(data.m_chatdbid),
//m_sex(data.m_sex),
//m_address(data.m_address),
//m_birthday(data.m_birthday),
//m_idiograph(data.m_idiograph),
//m_bloodtype(data.m_bloodtype),
//m_work(data.m_work),
//m_school(data.m_school),
//m_email(data.m_email),
//m_blog(data.m_blog),
//m_tel(data.m_tel),
//m_mobile(data.m_mobile),
//m_note(data.m_note),
//m_beFriendValid(data.m_beFriendValid),
//m_logindate(data.m_logindate),
m_lastserverid(data.m_lastserverid),
m_locked(data.m_locked)
{
}


void crLoginQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	//case 1:
	//	m_nickname = buf;
	//	break;
	//case 2:
	//	m_relname = buf;
	//	break;
	//case 3:
	//	m_chatdbid = (unsigned short)(atoi(buf));
	//	break;
	//case 4:
	//	m_sex = (char)(atoi(buf));
	//	break;
	//case 5:
	//	m_address = buf;
	//	break;
	//case 6:
	//	m_birthday = buf;
	//	break;
	//case 7:
	//	m_idiograph = buf;
	//	break;
	//case 8:
	//	m_bloodtype = (char)(atoi(buf));
	//	break;
	//case 9:
	//	m_work = (char)(atoi(buf));
	//	break;
	//case 10:
	//	m_school = buf;
	//	break;
	//case 11:
	//	m_email = buf;
	//	break;
	//case 12:
	//	m_blog = buf;
	//	break;
	//case 13:
	//	m_tel = buf;
	//	break;
	//case 14:
	//	m_mobile = buf;
	//	break;
	//case 15:
	//	m_note = buf;
	//	break;
	//case 16:
	//	m_beFriendValid = (char)(atoi(buf));
	//	break;
	//case 17:
	//	m_logindate = buf;
	//	break;
	case 1:
		m_lastserverid = atoi(buf);
		break;
	case 2:
		m_locked = (unsigned char)(atoi(buf));
		break;
	}
}

void crLoginQueryData::buildQuerySql(const std::string& name,const std::string& pwd)
{
	char sql[128];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	//sprintf(sql,"select id,nickname,relname,chatdbid,sex,address,birthday,idiograph,bloodtype,work,school,email,blog,tel,mobile,note,befriendvalid,logindate,lastserverid from account where name='%s' and pwd=password('%s')\0",escname,pwd.c_str());
	sprintf(sql,"select id,lastserverid,locked from account where name='%s' and pwd=password('%s')\0",escname,pwd.c_str());
	setQuerySql(sql);
	setNumColumn(3);
}

int crLoginQueryData::getPlayerID()
{
	return m_id;
}
unsigned char crLoginQueryData::getLocked()
{
	return m_locked;
}
//unsigned short crLoginQueryData::getChatDBID()
//{
//	return m_chatdbid;
//}
//const std::string &crLoginQueryData::getNickName() const
//{
//	return m_nickname;
//}
//const std::string &crLoginQueryData::getRelName() const
//{
//	return m_relname;
//}
//char crLoginQueryData::getSex()
//{
//	return m_sex;
//}
//const std::string &crLoginQueryData::getAddress() const
//{
//	return m_address;
//}
//const std::string &crLoginQueryData::getBirthday() const
//{
//	return m_birthday;
//}
//const std::string &crLoginQueryData::getIdiograph() const
//{
//	return m_idiograph;
//}
//char crLoginQueryData::getBloodtype()
//{
//	return m_bloodtype;
//}
//char crLoginQueryData::getWork()
//{
//	return m_work;
//}
//const std::string &crLoginQueryData::getSchool() const
//{
//	return m_school;
//}
//const std::string &crLoginQueryData::getEmail() const
//{
//	return m_email;
//}
//const std::string &crLoginQueryData::getBlog() const
//{
//	return m_blog;
//}
//const std::string &crLoginQueryData::getTel() const
//{
//	return m_tel;
//}
//const std::string &crLoginQueryData::getMobile() const
//{
//	return m_mobile;
//}
//const std::string &crLoginQueryData::getNote() const
//{
//	return m_note;
//}
//char crLoginQueryData::getBeFriendValid()
//{
//	return m_beFriendValid;
//}
//const std::string &crLoginQueryData::getLogindate() const
//{
//	return m_logindate;
//}
int crLoginQueryData::getLastServerID()
{
	return m_lastserverid;
}
/////////////////////////////////////////
//
//crLoginChatQueryData
//
/////////////////////////////////////////
crLoginChatQueryData::crLoginChatQueryData():
m_chatdbid(0)
{
}
crLoginChatQueryData::crLoginChatQueryData(const crLoginChatQueryData& data):
crQueryData(data),
m_chatdbid(data.m_chatdbid),
m_nickname(data.m_nickname),
m_key(data.m_key)
{
}
void crLoginChatQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_chatdbid = (unsigned short)(atoi(buf));
		break;
	case 1:
		m_nickname = buf;
		break;
	case 2:
		m_key = buf;
		break;
	}
}
void crLoginChatQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select chatdbid,nickname,loginkey from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}
unsigned short crLoginChatQueryData::getChatDBID()
{
	return m_chatdbid;
}
const std::string &crLoginChatQueryData::getNickName() const
{
	return m_nickname;
}
const std::string& crLoginChatQueryData::getKey() const
{
	return m_key;
}
/////////////////////////////////////////
//
//crChatStateQueryData
//
/////////////////////////////////////////
crChatStateQueryData::crChatStateQueryData():
m_chatstate(0)
{
}
crChatStateQueryData::crChatStateQueryData(const crChatStateQueryData& data):
crQueryData(data),
m_chatstate(data.m_chatstate)
{
}
void crChatStateQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_chatstate = (char)(atoi(buf));
		break;
	}
}
void crChatStateQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select chatstate from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(1);
}
char crChatStateQueryData::getChatState()
{
	return m_chatstate;
}
/////////////////////////////////////////
//
//crChatStateNickNameQueryData
//
/////////////////////////////////////////
crChatStateNickNameQueryData::crChatStateNickNameQueryData():
m_chatstate(0)
{
}
crChatStateNickNameQueryData::crChatStateNickNameQueryData(const crChatStateNickNameQueryData& data):
crQueryData(data),
m_chatstate(data.m_chatstate),
m_nickname(data.m_nickname)
{
}
void crChatStateNickNameQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_chatstate = (char)(atoi(buf));
		break;
	case 1:
		m_nickname = buf;
		break;
	}
}
void crChatStateNickNameQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select chatstate,nickname from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(2);
}
char crChatStateNickNameQueryData::getChatState()
{
	return m_chatstate;
}
const std::string& crChatStateNickNameQueryData::getNickName() const
{
	return m_nickname;
}
/////////////////////////////////////////
//
//crJoinGroupQueryData
//
/////////////////////////////////////////
crJoinGroupQueryData::crJoinGroupQueryData():
m_chatstate(0),
m_chatdbid(0)
{
}
crJoinGroupQueryData::crJoinGroupQueryData(const crJoinGroupQueryData& data):
crQueryData(data),
m_chatstate(data.m_chatstate),
m_chatdbid(data.m_chatdbid),
m_nickname(data.m_nickname)
{
}
void crJoinGroupQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_chatstate = (char)(atoi(buf));
		break;
	case 1:
		m_chatdbid = (unsigned short)(atoi(buf));
		break;
	case 2:
		m_nickname = buf;
		break;
	}
}
void crJoinGroupQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select chatstate,chatdbid,nickname from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}
char crJoinGroupQueryData::getChatState()
{
	return m_chatstate;
}
unsigned short crJoinGroupQueryData::getChatDBID()
{
	return m_chatdbid;
}
const std::string& crJoinGroupQueryData::getNickName() const
{
	return m_nickname;
}
/////////////////////////////////////////
//
//crPlayerFriendTableQueryData
//
/////////////////////////////////////////
crPlayerFriendTableQueryData::crPlayerFriendTableQueryData():
m_id(0),
m_friendid(0)
{
}
crPlayerFriendTableQueryData::crPlayerFriendTableQueryData(const crPlayerFriendTableQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_friendid(data.m_friendid),
m_columnname(data.m_columnname),
m_remark(data.m_remark)
{
}


void crPlayerFriendTableQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_friendid = atoi(buf);
		break;
	case 2:
		m_columnname = buf;
		break;
	case 3:
		m_remark = buf;
		break;
	}
}

void crPlayerFriendTableQueryData::buildQuerySql(int myid)
{
	char sql[256];
	sprintf(sql,"select id,friendid,columnname,remark from playerfriend where playerid=%d\0",myid);
	setQuerySql(sql);
	setNumColumn(4);
}

int crPlayerFriendTableQueryData::getID()
{
    return m_id;
}

int crPlayerFriendTableQueryData::getFriendID()
{
	return m_friendid;
}

const std::string& crPlayerFriendTableQueryData::getColumnName() const
{
	return m_columnname;
}

const std::string& crPlayerFriendTableQueryData::getRemark() const
{
	return m_remark;
}
/////////////////////////////////////////
//
//crPlayerFriendLeavewordQueryData
//
/////////////////////////////////////////
crPlayerFriendLeavewordQueryData::crPlayerFriendLeavewordQueryData()
{
}
crPlayerFriendLeavewordQueryData::crPlayerFriendLeavewordQueryData(const crPlayerFriendLeavewordQueryData& data):
crQueryData(data)
{
}

void crPlayerFriendLeavewordQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		if(length>0)
		{
			m_leaveword = new crStreamBuf;
			m_leaveword->createBuf(length);
			m_leaveword->write(buf,length);
			m_leaveword->seekBegin();
		}
		break;
	}
}

void crPlayerFriendLeavewordQueryData::buildQuerySql(int playerfriendid)
{
	char sql[256];
	sprintf(sql,"select leaveword from playerfriendrec where playerfriendid=%d\0",playerfriendid);
	setQuerySql(sql);
	setNumColumn(1);
}

const crStreamBuf *crPlayerFriendLeavewordQueryData::getLeaveword() const
{
	return m_leaveword.get();
}
/////////////////////////////////////////
//
//crPlayerGroupTableQueryData
//
/////////////////////////////////////////
crPlayerGroupTableQueryData::crPlayerGroupTableQueryData():
m_id(0),
m_groupid(0)
{
}
crPlayerGroupTableQueryData::crPlayerGroupTableQueryData(const crPlayerGroupTableQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_groupid(data.m_groupid),
m_remark(data.m_remark)
{
}


void crPlayerGroupTableQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_groupid = atoi(buf);
		break;
	case 2:
		m_remark = buf;
		break;
	}
}

void crPlayerGroupTableQueryData::buildQuerySql(int myid)
{
	char sql[256];
	sprintf(sql,"select id,groupid,remark from playergroup where playerid=%d\0",myid);
	setQuerySql(sql);
	setNumColumn(3);
}

int crPlayerGroupTableQueryData::getID()
{
	return m_id;
}
int crPlayerGroupTableQueryData::getGroupID()
{
	return m_groupid;
}
const std::string &crPlayerGroupTableQueryData::getRemark() const
{
	return m_remark;
}
/////////////////////////////////////////
//
//crChatGroupTableQueryData
//
/////////////////////////////////////////
crChatGroupTableQueryData::crChatGroupTableQueryData():
m_id(0),
m_creator(0),
m_manager1(0),
m_manager2(0),
m_manager3(0)
{
}
crChatGroupTableQueryData::crChatGroupTableQueryData(const crChatGroupTableQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_creator(data.m_creator),
m_manager1(data.m_manager1),
m_manager2(data.m_manager2),
m_manager3(data.m_manager3)
{
}


void crChatGroupTableQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_creator = atoi(buf);
		break;
	case 2:
		m_manager1 = atoi(buf);
		break;
	case 3:
		m_manager2 = atoi(buf);
		break;
	case 4:
		m_manager3 = atoi(buf);
		break;
	}
}

void crChatGroupTableQueryData::buildQuerySql(const std::string& groupname)
{
	char sql[512];
	int size = groupname.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,groupname.c_str(),size);
	sprintf(sql,"select id,creator,manager1,manager2,manager3 from chatgroup where groupname='%s'\0",escname);
	setQuerySql(sql);
	setNumColumn(5);
}

int crChatGroupTableQueryData::getID()
{
	return m_id;
}
int crChatGroupTableQueryData::getCreator()
{
	return m_creator;
}
int crChatGroupTableQueryData::getManager1()
{
	return m_manager1;
}
int crChatGroupTableQueryData::getManager2()
{
	return m_manager2;
}
int crChatGroupTableQueryData::getManager3()
{
	return m_manager3;
}
/////////////////////////////////////////
//
//crChatGroupTableByIDQueryData
//
/////////////////////////////////////////
crChatGroupTableByIDQueryData::crChatGroupTableByIDQueryData():
m_creator(0),
m_manager1(0),
m_manager2(0),
m_manager3(0)
{
}
crChatGroupTableByIDQueryData::crChatGroupTableByIDQueryData(const crChatGroupTableByIDQueryData& data):
crQueryData(data),
m_creator(data.m_creator),
m_manager1(data.m_manager1),
m_manager2(data.m_manager2),
m_manager3(data.m_manager3)
{
}


void crChatGroupTableByIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_groupname = buf;
		break;
	case 1:
		m_creator = atoi(buf);
		break;
	case 2:
		m_manager1 = atoi(buf);
		break;
	case 3:
		m_manager2 = atoi(buf);
		break;
	case 4:
		m_manager3 = atoi(buf);
		break;
	}
}

void crChatGroupTableByIDQueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select groupname,creator,manager1,manager2,manager3 from chatgroup where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(5);
}

const std::string& crChatGroupTableByIDQueryData::getGroupName() const
{
	return m_groupname;
}
int crChatGroupTableByIDQueryData::getCreator()
{
	return m_creator;
}
int crChatGroupTableByIDQueryData::getManager1()
{
	return m_manager1;
}
int crChatGroupTableByIDQueryData::getManager2()
{
	return m_manager2;
}
int crChatGroupTableByIDQueryData::getManager3()
{
	return m_manager3;
}
/////////////////////////////////////////
//
//crGroupMemberTableQueryData
//
/////////////////////////////////////////
crGroupMemberTableQueryData::crGroupMemberTableQueryData():
m_id(0),
m_playerid(0)
{
}
crGroupMemberTableQueryData::crGroupMemberTableQueryData(const crGroupMemberTableQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_playerid(data.m_playerid),
m_membername(data.m_membername),
m_memberremark(data.m_memberremark)
{
}


void crGroupMemberTableQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_playerid = atoi(buf);
		break;
	case 2:
		m_membername = buf;
		break;
	case 3:
		m_memberremark = buf;
		break;
	}
}

void crGroupMemberTableQueryData::buildQuerySql(int groupid)
{
	char sql[256];
	sprintf(sql,"select id,playerid,membername,memberremark from groupmember where groupid=%d\0",groupid);
	setQuerySql(sql);
	setNumColumn(4);
}

int crGroupMemberTableQueryData::getID()
{
	return m_id;
}

int crGroupMemberTableQueryData::getPlayerID()
{
	return m_playerid;
}

const std::string &crGroupMemberTableQueryData::getMemberName() const
{
	return m_membername;
}
const std::string &crGroupMemberTableQueryData::getMemberRemark() const
{
	return m_memberremark;
}

/////////////////////////////////////////
//
//crPlayerGroupLeavewordQueryData
//
/////////////////////////////////////////
crPlayerGroupLeavewordQueryData::crPlayerGroupLeavewordQueryData()
{
}
crPlayerGroupLeavewordQueryData::crPlayerGroupLeavewordQueryData(const crPlayerGroupLeavewordQueryData& data):
crQueryData(data)
{
}

void crPlayerGroupLeavewordQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		if(length>0)
		{
			m_leaveword = new crStreamBuf;
			m_leaveword->createBuf(length);
			m_leaveword->write(buf,length);
			m_leaveword->seekBegin();
		}
		break;
	}
}

void crPlayerGroupLeavewordQueryData::buildQuerySql(int playergroupid)
{
	char sql[256];
	sprintf(sql,"select leaveword from playergrouprec where playergroupid=%d\0",playergroupid);
	setQuerySql(sql);
	setNumColumn(1);
}

const crStreamBuf *crPlayerGroupLeavewordQueryData::getLeaveword() const
{
	return m_leaveword.get();
}

/////////////////////////////////////////
//
//crBeFriendValidQueryData
//
/////////////////////////////////////////
crBeFriendValidQueryData::crBeFriendValidQueryData():
m_beFriendValid(0),
m_chatstate(0)
{
}
crBeFriendValidQueryData::crBeFriendValidQueryData(const crBeFriendValidQueryData& data):
crQueryData(data),
m_beFriendValid(data.m_beFriendValid),
m_chatstate(data.m_chatstate)
{
}


void crBeFriendValidQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_beFriendValid = (unsigned char)(atoi(buf));
		break;
	case 1:
		m_nickname = buf;
		break;
	case 2:
		m_chatstate = (char)(atoi(buf));
		break;
	}
}

void crBeFriendValidQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select befriendvalid,nickname,chatstate from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}

unsigned char crBeFriendValidQueryData::getBeFriendValid()
{
	return m_beFriendValid;
}

const std::string &crBeFriendValidQueryData::getNickName() const
{
	return m_nickname;
}

char crBeFriendValidQueryData::getChatState()
{
	return m_chatstate;
}
/////////////////////////////////////////
//
//crChatDBIDQueryData
//
/////////////////////////////////////////
crChatDBIDQueryData::crChatDBIDQueryData():
m_chatdbid(0)
{
}
crChatDBIDQueryData::crChatDBIDQueryData(const crChatDBIDQueryData& data):
crQueryData(data),
m_chatdbid(data.m_chatdbid)
{
}

void crChatDBIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_chatdbid = (unsigned short)(atoi(buf));
		break;
	}
}

void crChatDBIDQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select chatdbid from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(1);
}

unsigned short crChatDBIDQueryData::getChatDBID()
{
    return m_chatdbid;
}
/////////////////////////////////////////
//
//crPlayerFriendIDQueryData
//
/////////////////////////////////////////
crPlayerFriendIDQueryData::crPlayerFriendIDQueryData():
m_id(0)
{
}
crPlayerFriendIDQueryData::crPlayerFriendIDQueryData(const crPlayerFriendIDQueryData& data):
crQueryData(data),
m_id(data.m_id)
{
}

void crPlayerFriendIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}

void crPlayerFriendIDQueryData::buildQuerySql(int playerid,int friendid)
{
	char sql[256];
	sprintf(sql,"select id from playerfriend where playerid=%d and friendid=%d\0",playerid,friendid);
	setQuerySql(sql);
	setNumColumn(1);
}

int crPlayerFriendIDQueryData::getID()
{
	return m_id;
}

/////////////////////////////////////////
//
//crPlayerGroupIDQueryData
//
/////////////////////////////////////////
crPlayerGroupIDQueryData::crPlayerGroupIDQueryData():
m_id(0)
{
}
crPlayerGroupIDQueryData::crPlayerGroupIDQueryData(const crPlayerGroupIDQueryData& data):
crQueryData(data),
m_id(data.m_id)
{
}

void crPlayerGroupIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}

void crPlayerGroupIDQueryData::buildQuerySql(int playerid,int groupid)
{
	char sql[256];
	sprintf(sql,"select id from playergroup where playerid=%d and groupid=%d\0",playerid,groupid);
	setQuerySql(sql);
	setNumColumn(1);
}

int crPlayerGroupIDQueryData::getID()
{
	return m_id;
}

/////////////////////////////////////////
//
//crOnchatAccountLimitQueryData
//
/////////////////////////////////////////
crOnchatAccountLimitQueryData::crOnchatAccountLimitQueryData():
m_id(0)
{
}
crOnchatAccountLimitQueryData::crOnchatAccountLimitQueryData(const crOnchatAccountLimitQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_nickname(data.m_nickname),
m_address(data.m_address)
{
}

void crOnchatAccountLimitQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_nickname = buf;
		break;
	case 2:
		m_address = buf;
		break;
	}
}

void crOnchatAccountLimitQueryData::buildQuerySql(int rows,int offset,int exceptid)
{
	char sql[256];
	//sprintf(sql,"select id, nickname, address from account limit %d,%d where chatstate > 0\0",rows,offset);
	if(exceptid>0)
	    sprintf(sql,"select id,nickname,address from account where id!=%d limit %d,%d\0",exceptid,rows,offset);
	else
        sprintf(sql,"select id,nickname,address from account limit %d,%d\0",rows,offset);
	setQuerySql(sql);
	setNumColumn(3);
}

int crOnchatAccountLimitQueryData::getID()
{
	return m_id;
}

const std::string& crOnchatAccountLimitQueryData::getNickName() const
{
    return m_nickname;
}

const std::string& crOnchatAccountLimitQueryData::getAddress() const
{
    return m_address;
}

/////////////////////////////////////////
//
//crChatGroupLimitQueryData
//
/////////////////////////////////////////
crChatGroupLimitQueryData::crChatGroupLimitQueryData():
m_id(0)
{
}
crChatGroupLimitQueryData::crChatGroupLimitQueryData(const crChatGroupLimitQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_groupname(data.m_groupname),
m_creator(data.m_creator)
{
}

void crChatGroupLimitQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_groupname = buf;
		break;
	case 2:
		m_creator = atoi(buf);
		break;
	}
}

void crChatGroupLimitQueryData::buildQuerySql(int rows,int offset)
{
	char sql[256];
	sprintf(sql,"select id,groupname,creator from chatgroup limit %d,%d\0",rows,offset);
	setQuerySql(sql);
	setNumColumn(3);
}

int crChatGroupLimitQueryData::getID()
{
	return m_id;
}

const std::string& crChatGroupLimitQueryData::getGroupName() const
{
	return m_groupname;
}

int crChatGroupLimitQueryData::getCreator()
{
	return m_creator;
}