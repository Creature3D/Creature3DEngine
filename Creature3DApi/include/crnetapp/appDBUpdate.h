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
#ifndef CRNETAPP_DBUPDATE_H
#define CRNETAPP_DBUPDATE_H 1

#include <CRNetApp/appExport.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crStreamBuf.h>
#include <string>

namespace CRNetApp{

class CRNETAPP_EXPORT crUpdateLoginKeyDate : public CRDataBase::crUpdateSql
{
public:
	crUpdateLoginKeyDate(){}
	void buildUpdateSql(int playerid,const std::string &key);
};
class CRNETAPP_EXPORT crUpdateLoginServerID : public CRDataBase::crUpdateSql
{
public:
	crUpdateLoginServerID(){}
	void buildUpdateSql(int playerid,int serverid);
};
class CRNETAPP_EXPORT crUpdateChatState : public CRDataBase::crUpdateSql
{
public:
	crUpdateChatState(){}
	void buildUpdateSql(int playerid,char chatstate);
};

class CRNETAPP_EXPORT crInsertPlayerFriendLeaveword : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerFriendLeaveword(){}
	void buildUpdateSql(int playerfriendid, const char *buf, int size);
};

class CRNETAPP_EXPORT crRemovePlayerFriendLeaveword : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerFriendLeaveword(){}
	void buildUpdateSql(int playerfriendid);
};

class CRNETAPP_EXPORT crInsertPlayerGroupLeaveword : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerGroupLeaveword(){}
	void buildUpdateSql(int playergroupid, const char *buf, int size);
};

class CRNETAPP_EXPORT crRemovePlayerGroupLeaveword : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerGroupLeaveword(){}
	void buildUpdateSql(int playergroupid);
};

class CRNETAPP_EXPORT crUpdateNickName : public CRDataBase::crUpdateSql
{
public:
	crUpdateNickName(){}
	void buildUpdateSql(int playerid, const std::string &nickName);
};

class CRNETAPP_EXPORT crInsertPlayerFriend : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerFriend(){}
	void buildUpdateSql(int playerid, int friendid);
};

class CRNETAPP_EXPORT crRemovePlayerFriend : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerFriend(){}
	void buildUpdateSql(int playerfriendid);
};

class CRNETAPP_EXPORT crInsertPlayerGroup : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerGroup(){}
	void buildUpdateSql(int playerid, int groupid);
};

class CRNETAPP_EXPORT crRemovePlayerGroup : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerGroup(){}
	void buildUpdateSql(int playergroupid);
};

class CRNETAPP_EXPORT crInsertAccount : public CRDataBase::crUpdateSql
{
public:
	crInsertAccount(){}
	void buildUpdateSql(const std::string &name, const std::string &password, unsigned short chatdbid, const std::string &email, const std::string &nickname, const std::string &relname, char sex);
};

class CRNETAPP_EXPORT crRemoveAccount : public CRDataBase::crUpdateSql
{
public:
	crRemoveAccount(){}
	void buildUpdateSql(const std::string &name);
};

class CRNETAPP_EXPORT crUpdatePassword : public CRDataBase::crUpdateSql
{
public:
	crUpdatePassword(){}
	void buildUpdateSql(int playerid, const std::string &password);
};

class CRNETAPP_EXPORT crInsertChatGroup : public CRDataBase::crUpdateSql
{
public:
	crInsertChatGroup(){}
	void buildUpdateSql(const std::string &groupname, const std::string &introduce, int creator);
};

class CRNETAPP_EXPORT crRemoveChatGroup : public CRDataBase::crUpdateSql
{
public:
	crRemoveChatGroup(){}
	void buildUpdateSql(int groupid);
};

class CRNETAPP_EXPORT crInsertGroupMember : public CRDataBase::crUpdateSql
{
public:
	crInsertGroupMember(){}
	void buildUpdateSql(int groupid, int playerid, const std::string &membername);
};

class CRNETAPP_EXPORT crRemoveGroupMember : public CRDataBase::crUpdateSql
{
public:
	crRemoveGroupMember(){}
	void buildUpdateSql(int id);
	void buildUpdateSql(int groupid,int playerid);
};

//class CRNETAPP_EXPORT crLogoutDate : public CRDataBase::crUpdateSql
//{
//public:
//	crLogoutDate(){}
//	void buildUpdateSql(int playerid);
//};

}
#endif
