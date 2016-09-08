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
#include <CRNetApp/appDBUpdate.h>

using namespace CRNetApp;
using namespace CRCore;
using namespace CRDataBase;

/////////////////////////////////////////
//
//crUpdateLoginKeyDate
//
/////////////////////////////////////////
void crUpdateLoginKeyDate::buildUpdateSql(int playerid,const std::string &key)
{
	char sql[256];
	int size = key.size();
	char esckey[128];
	memset(esckey,0,128);
	crDataBaseManager::getInstance()->escapeString(esckey,key.c_str(),size);
	sprintf(sql,"update account set loginkey='%s',logindate=now() where id=%d\0",esckey,playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateLoginServerID
//
/////////////////////////////////////////
void crUpdateLoginServerID::buildUpdateSql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"update account set lastserverid=%d where id=%d\0",serverid,playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateChatState
//
/////////////////////////////////////////
void crUpdateChatState::buildUpdateSql(int playerid,char chatstate)
{
	char sql[128];
	sprintf(sql,"update account set chatstate=%d,updatedate=now() where id=%d\0",(int)chatstate,playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerFriendLeaveword
//
/////////////////////////////////////////
void crInsertPlayerFriendLeaveword::buildUpdateSql(int playerfriendid,const char *buf, int size)
{
	char *escbuf = new char[size*2+1];
	int escsize = crDataBaseManager::getInstance()->escapeString(escbuf,buf,size);
	char *sql = new char[escsize+256];
	sprintf(sql,"insert into playerfriendrec(playerfriendid,leaveword) values(%d,'%s')\0",playerfriendid,escbuf);
	setUpdateSql(sql);
	delete [] escbuf;
	delete [] sql;
}
/////////////////////////////////////////
//
//crRemovePlayerFriendLeaveword
//
/////////////////////////////////////////
void crRemovePlayerFriendLeaveword::buildUpdateSql(int playerfriendid)
{
	char sql[256];
	sprintf(sql,"delete from playerfriendrec where playerfriendid=%d\0",playerfriendid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerGroupLeaveword
//
/////////////////////////////////////////
void crInsertPlayerGroupLeaveword::buildUpdateSql(int playergroupid,const char *buf, int size)
{
	char *escbuf = new char[size*2+1];
	memset(escbuf,0,size*2+1);
	int escsize = crDataBaseManager::getInstance()->escapeString(escbuf,buf,size);
	char *sql = new char[escsize+256];
	sprintf(sql,"insert into playergrouprec(playergroupid,leaveword) values (%d,'%s')\0",playergroupid,escbuf);
	setUpdateSql(sql);
	delete [] escbuf;
	delete [] sql;
}
/////////////////////////////////////////
//
//crRemovePlayerGroupLeaveword
//
/////////////////////////////////////////
void crRemovePlayerGroupLeaveword::buildUpdateSql(int playergroupid)
{
	char sql[256];
	sprintf(sql,"delete from playergrouprec where playergroupid=%d\0",playergroupid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateNickName
//
/////////////////////////////////////////
void crUpdateNickName::buildUpdateSql(int playerid, const std::string &nickName)
{
	char sql[256];
	sprintf(sql,"update account set nickname='%s',updatedate=now() where id=%d\0",nickName.c_str(),playerid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crInsertPlayerFriend
//
/////////////////////////////////////////
void crInsertPlayerFriend::buildUpdateSql(int playerid, int friendid)
{
	char sql[256];
	sprintf(sql,"insert into playerfriend(playerid,friendid,createdate) values(%d,%d,now())\0",playerid,friendid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crRemovePlayerFriend
//
/////////////////////////////////////////
void crRemovePlayerFriend::buildUpdateSql(int playerfriendid)
{
	char sql[64];
	sprintf(sql,"delete from playerfriend where id=%d\0",playerfriendid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crInsertPlayerGroup
//
/////////////////////////////////////////
void crInsertPlayerGroup::buildUpdateSql(int playerid, int groupid)
{
	char sql[256];
	sprintf(sql,"insert into playergroup(playerid,groupid,createdate) values(%d,%d,now())\0",playerid,groupid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crRemovePlayerGroup
//
/////////////////////////////////////////
void crRemovePlayerGroup::buildUpdateSql(int playergroupid)
{
	char sql[64];
	sprintf(sql,"delete from playergroup where id=%d\0",playergroupid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crInsertAccount
//
/////////////////////////////////////////
void crInsertAccount::buildUpdateSql(const std::string &name, const std::string &password, unsigned short chatdbid, const std::string &email, const std::string &nickname, const std::string &relname, char sex)
{
	char sql[256];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);
	
	size = nickname.size();
	char escnickname[32];
	memset(escnickname,0,32);
	crDataBaseManager::getInstance()->escapeString(escnickname,nickname.c_str(),size);

	size = relname.size();
	char escrelname[32];
	memset(escrelname,0,32);
	crDataBaseManager::getInstance()->escapeString(escrelname,relname.c_str(),size);

	sprintf(sql,"insert into account(name,pwd,chatdbid,nickname,relname,sex,email,befriendvalid,onlinetime,createdate) values('%s',password('%s'),%d,'%s','%s',%d,'%s',1,0,now())\0",escname,password.c_str(),(char)chatdbid,escnickname,escrelname,(int)sex,email.c_str());
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crRemoveAccount
//
/////////////////////////////////////////
void crRemoveAccount::buildUpdateSql(const std::string &name)
{
	char sql[256];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);
	sprintf(sql,"delete from account where name='%s'\0",escname);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crUpdatePassword
//
/////////////////////////////////////////
void crUpdatePassword::buildUpdateSql(int playerid, const std::string &password)
{
	char sql[256];
	sprintf(sql,"update account set pwd=password('%s'),updatedate=now() where id=%d\0",password.c_str(),playerid);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crInsertChatGroup
//
/////////////////////////////////////////
void crInsertChatGroup::buildUpdateSql(const std::string &groupname, const std::string &introduce, int creator)
{
	char sql[512];
	int size = groupname.size();
	char escgroupname[32];
	memset(escgroupname,0,32);
	crDataBaseManager::getInstance()->escapeString(escgroupname,groupname.c_str(),size);

	size = introduce.size();
	char escintroduce[256];
	memset(escintroduce,0,256);
	crDataBaseManager::getInstance()->escapeString(escintroduce,introduce.c_str(),size);

	sprintf(sql,"insert into chatgroup(groupname,introduce,creator,createdate) values('%s','%s',%d,now())\0",escgroupname,escintroduce,creator);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crRemoveChatGroup
//
/////////////////////////////////////////
void crRemoveChatGroup::buildUpdateSql(int groupid)
{
	char sql[256];
	sprintf(sql,"delete from chatgroup where id=%d\0",groupid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertGroupMember
//
/////////////////////////////////////////
void crInsertGroupMember::buildUpdateSql(int groupid, int playerid, const std::string &membername)
{
	char sql[256];
	int size = membername.size();
	char escmembername[32];
	memset(escmembername,0,32);
	crDataBaseManager::getInstance()->escapeString(escmembername,membername.c_str(),size);

	sprintf(sql,"insert into groupmember(groupid,playerid,membername,createdate) values(%d,%d,'%s',now())\0",groupid,playerid,escmembername);
	setUpdateSql(sql);
}

/////////////////////////////////////////
//
//crRemoveGroupMember
//
/////////////////////////////////////////
void crRemoveGroupMember::buildUpdateSql(int id)
{
	char sql[256];
	sprintf(sql,"delete from groupmember where id=%d\0",id);
	setUpdateSql(sql);
}

void crRemoveGroupMember::buildUpdateSql(int groupid,int playerid)
{
	char sql[128];
	sprintf(sql,"delete from groupmember where groupid=%d and playerid=%d\0",groupid,playerid);
	setUpdateSql(sql);
}

///////////////////////////////////////////
////
////crLogoutDate
////
///////////////////////////////////////////
//void crLogoutDate::buildUpdateSql(int playerid)
//{
//	char sql[256];
//	sprintf(sql,"update account set loginkey=NULL, chatstate=0 where id=%d\0",playerid);
//	setUpdateSql(sql);
//}