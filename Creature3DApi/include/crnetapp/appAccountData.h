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
#ifndef CRNETAPP_ACCOUNTDATA_H
#define CRNETAPP_ACCOUNTDATA_H 1

#include <CRNetApp/appExport.h>
//#include <creature3d.h>
#include <CRNet/crNetDataManager.h>
#include <CRCore/crWchMsg.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <map>
#include <vector>
#include <deque>

namespace CRNetApp {

class CRNETAPP_EXPORT crAccountData : public CRCore::Referenced
{
public:
	crAccountData();
	void setNickName(const std::string &nickName);
	const std::string &getNickName() const;
	void setRelName(const std::string &relName);
	const std::string &getRelName() const;
	void setGameDBID(unsigned short gamedbid){ m_gamedbid = gamedbid; }
	unsigned short getGameDBID(){ return m_gamedbid; }
	void setChatDBID(unsigned short chatdbid){ m_chatdbid = chatdbid; }
	unsigned short getChatDBID(){ return m_chatdbid; }

	void setSex(char sex);
	char getSex();
	void setAddress(const std::string &address);
	const std::string &getAddress() const;
	void setBirthday(const std::string &birthday);
	const std::string &getBirthday() const;
	void setIdiograph(const std::string &idiograph);
	const std::string &getIdiograph() const;
	void setBloodtype(char bloodtype);
	char getBloodtype();
	void setWork(char work);
	char getWork();
	void setSchool(const std::string &school);
	const std::string &getSchool() const;
	void setEmail(const std::string &email);
	const std::string &getEmail() const;
	void setBlog(const std::string &blog);
	const std::string &getBlog() const;
	void setTel(const std::string &tel);
	const std::string &getTel() const;
	void setMobile(const std::string &mobile);
	const std::string &getMobile() const;
	void setNote(const std::string &note);
	const std::string &getNote() const;
	void setBeFriendValid(char befriendvalid);//0:拒绝任何人添加 1:同意任何人添加 2:需要消息验证  *3:需要正确回答问题
	char getBeFriendValid();
	void setPrelogindate(const std::string &prelogindate);
	const std::string &getPrelogindate() const;
	void setLastServerID(int id);
	int getLastServerID();
protected:
	std::string m_nickName;
	std::string m_relname;
	unsigned short m_gamedbid;
	unsigned short m_chatdbid;

	char m_sex;
	std::string m_address;
	std::string m_birthday;
	std::string m_idiograph;
	char m_bloodtype;
	char m_work;
	std::string m_school;
	std::string m_email;
	std::string m_blog;
	std::string m_tel;
	std::string m_mobile;
	std::string m_note;
	char m_beFriendValid;
	std::string m_prelogindate;
	int m_lastserverid;
};

}

#endif
