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
#ifndef CRNETAPP_DBQUERY_H
#define CRNETAPP_DBQUERY_H 1

#include <CRNetApp/appExport.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crStreamBuf.h>
#include <string>

namespace CRNetApp{

///////GlobalDB
class CRNETAPP_EXPORT crLoginQueryData : public CRDataBase::crQueryData
{//根据userName查询account表，单组
public:
	crLoginQueryData();
	crLoginQueryData(const crLoginQueryData& data);
	DataClass(CRNetApp, LoginQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string& name,const std::string& pwd);
	int getPlayerID();

	//unsigned short getChatDBID();
	//const std::string &getNickName() const;
	//const std::string &getRelName() const;
	//char getSex();
 //   const std::string &getAddress() const;
	//const std::string &getBirthday() const;
	//const std::string &getIdiograph() const;
 //   char getBloodtype();
	//char getWork();
 //   const std::string &getSchool() const;
 //   const std::string &getEmail() const;
 //   const std::string &getBlog() const;
 //   const std::string &getTel() const;
 //   const std::string &getMobile() const;
 //   const std::string &getNote() const;
	//char getBeFriendValid();
	//const std::string& getLogindate() const;
	int getLastServerID();
	unsigned char getLocked();
protected:
	int m_id;

	//std::string m_nickname;
	//std::string m_relname;
	//unsigned short m_chatdbid;

	//char m_sex;//性别，0为女，1为男
	//std::string m_address;
	//std::string m_birthday;
	//std::string m_idiograph;
	//char m_bloodtype;
	//char m_work;
	//std::string m_school;
	//std::string m_email;
	//std::string m_blog;
	//std::string m_tel;
	//std::string m_mobile;
	//std::string m_note;
	//char m_beFriendValid;
	//std::string m_logindate;
	int m_lastserverid;
	unsigned char m_locked;
};

class CRNETAPP_EXPORT crLoginChatQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crLoginChatQueryData();
	crLoginChatQueryData(const crLoginChatQueryData& data);
	DataClass(CRNetApp, LoginChatQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	unsigned short getChatDBID();
	const std::string &getNickName() const;
	const std::string& getKey() const;
protected:
	unsigned short m_chatdbid;
	std::string m_nickname;
	std::string m_key;
};

class CRNETAPP_EXPORT crChatStateQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crChatStateQueryData();
	crChatStateQueryData(const crChatStateQueryData& data);
	DataClass(CRNetApp, ChatStateQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	char getChatState();
protected:
	char m_chatstate;
};

class CRNETAPP_EXPORT crChatStateNickNameQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crChatStateNickNameQueryData();
	crChatStateNickNameQueryData(const crChatStateNickNameQueryData& data);
	DataClass(CRNetApp, ChatStateNickNameQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	char getChatState();
	const std::string &getNickName() const;
protected:
	char m_chatstate;
	std::string m_nickname;
};

class CRNETAPP_EXPORT crJoinGroupQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crJoinGroupQueryData();
	crJoinGroupQueryData(const crJoinGroupQueryData& data);
	DataClass(CRNetApp, JoinGroupQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	char getChatState();
	unsigned short getChatDBID();
	const std::string &getNickName() const;
protected:
	char m_chatstate;
	unsigned short m_chatdbid;
	std::string m_nickname;
};

class CRNETAPP_EXPORT crPlayerFriendTableQueryData : public CRDataBase::crQueryData
{//根据playerid查询playerfriend表，多组
public:
	crPlayerFriendTableQueryData();
	crPlayerFriendTableQueryData(const crPlayerFriendTableQueryData& data);
	DataClass(CRNetApp, PlayerFriendTableQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int myid);
	int getID();
	int getFriendID();
	const std::string& getColumnName() const;
	const std::string& getRemark() const;
protected:
	int m_id;
	int m_friendid;
	std::string m_columnname;
	std::string m_remark;
};

class CRNETAPP_EXPORT crPlayerFriendLeavewordQueryData : public CRDataBase::crQueryData
{//根据playerfriendid查询playerfriendrec表，多组
public:
	crPlayerFriendLeavewordQueryData();
	crPlayerFriendLeavewordQueryData(const crPlayerFriendLeavewordQueryData& data);
	DataClass(CRNetApp, PlayerFriendLeavewordQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerfriendid);
	const CRCore::crStreamBuf *getLeaveword() const;
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_leaveword;
};

class CRNETAPP_EXPORT crPlayerGroupTableQueryData : public CRDataBase::crQueryData
{//根据playerid查询playergroup表，多组
public:
	crPlayerGroupTableQueryData();
	crPlayerGroupTableQueryData(const crPlayerGroupTableQueryData& data);
	DataClass(CRNetApp, PlayerGroupTableQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int myid);
	int getID();
	const std::string& getRemark() const;
	int getGroupID();
protected:
	int m_id;
	int m_groupid;
	std::string m_remark;
};

class CRNETAPP_EXPORT crChatGroupTableQueryData : public CRDataBase::crQueryData
{//根据groupname查询chatgroup表，多组
public:
	crChatGroupTableQueryData();
	crChatGroupTableQueryData(const crChatGroupTableQueryData& data);
	DataClass(CRNetApp, ChatGroupTableQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string& groupname);
	int getID();
	int getCreator();
	int getManager1();
	int getManager2();
	int getManager3();
protected:
	int m_id;
	int m_creator;
	int m_manager1;
	int m_manager2;
	int m_manager3;
};

class CRNETAPP_EXPORT crChatGroupTableByIDQueryData : public CRDataBase::crQueryData
{//根据id查询Group表，多组
public:
	crChatGroupTableByIDQueryData();
	crChatGroupTableByIDQueryData(const crChatGroupTableByIDQueryData& data);
	DataClass(CRNetApp, ChatGroupTableByIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	const std::string& getGroupName() const;
	int getCreator();
	int getManager1();
	int getManager2();
	int getManager3();
protected:
	std::string m_groupname;
	int m_creator;
	int m_manager1;
	int m_manager2;
	int m_manager3;
};

class CRNETAPP_EXPORT crGroupMemberTableQueryData : public CRDataBase::crQueryData
{//根据groupid查询groupmember表，多组
public:
	crGroupMemberTableQueryData();
	crGroupMemberTableQueryData(const crGroupMemberTableQueryData& data);
	DataClass(CRNetApp, GroupMemberTableQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int groupid);
	int getID();
	int getPlayerID();
	const std::string& getMemberName() const;
	const std::string& getMemberRemark() const;
protected:
	int m_id;
	int m_playerid;
	std::string m_membername;
	std::string m_memberremark;
};

class CRNETAPP_EXPORT crPlayerGroupLeavewordQueryData : public CRDataBase::crQueryData
{//根据playergroupid查询playergrouprec表，多组
public:
	crPlayerGroupLeavewordQueryData();
	crPlayerGroupLeavewordQueryData(const crPlayerGroupLeavewordQueryData& data);
	DataClass(CRNetApp, PlayerGroupLeavewordQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playergroupid);
	const CRCore::crStreamBuf *getLeaveword() const;
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_leaveword;
};

class CRNETAPP_EXPORT crBeFriendValidQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crBeFriendValidQueryData();
	crBeFriendValidQueryData(const crBeFriendValidQueryData& data);
	DataClass(CRNetApp, BeFriendValidQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	unsigned char getBeFriendValid();
    const std::string &getNickName() const;
	char getChatState();
protected:
	unsigned char m_beFriendValid;
	std::string m_nickname;
	char m_chatstate;
};

class CRNETAPP_EXPORT crChatDBIDQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crChatDBIDQueryData();
	crChatDBIDQueryData(const crChatDBIDQueryData& data);
	DataClass(CRNetApp, ChatDBIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	unsigned short getChatDBID(); 
protected:
	unsigned short m_chatdbid;
};

class CRNETAPP_EXPORT crPlayerFriendIDQueryData : public CRDataBase::crQueryData
{//根据playerid,friendid查询playerfriend表，单组
public:
	crPlayerFriendIDQueryData();
	crPlayerFriendIDQueryData(const crPlayerFriendIDQueryData& data);
	DataClass(CRNetApp, PlayerFriendIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int friendid);
	int getID(); 
protected:
	int m_id;
};

class CRNETAPP_EXPORT crPlayerGroupIDQueryData : public CRDataBase::crQueryData
{//根据playerid,groupid查询playergroup表，单组
public:
	crPlayerGroupIDQueryData();
	crPlayerGroupIDQueryData(const crPlayerGroupIDQueryData& data);
	DataClass(CRNetApp, PlayerGroupIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int groupid);
	int getID(); 
protected:
	int m_id;
};

class CRNETAPP_EXPORT crOnchatAccountLimitQueryData : public CRDataBase::crQueryData
{//查询account表，多组
public:
	crOnchatAccountLimitQueryData();
	crOnchatAccountLimitQueryData(const crOnchatAccountLimitQueryData& data);
	DataClass(CRNetApp, OnchatAccountLimitQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int rows,int offset,int exceptid = 0);
	int getID();
	const std::string& getNickName() const;
	const std::string& getAddress() const;
protected:
	int m_id;
	std::string m_nickname;
	std::string m_address;
};

class CRNETAPP_EXPORT crChatGroupLimitQueryData : public CRDataBase::crQueryData
{//查询Group表，多组
public:
	crChatGroupLimitQueryData();
	crChatGroupLimitQueryData(const crChatGroupLimitQueryData& data);
	DataClass(CRNetApp, ChatGroupLimitQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int rows,int offset);
	int getID();
	const std::string& getGroupName() const;
	int getCreator();
protected:
	int m_id;
	std::string m_groupname;
	int m_creator;
};
}
#endif