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

#include <CRNetApp/appAccountData.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appSightInfo.h>
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
//crAccountData
//
/////////////////////////////////////
crAccountData::crAccountData():
m_gamedbid(0),
m_chatdbid(0),
m_sex(0),
m_bloodtype(0),
m_work(0),
m_beFriendValid(0),
m_lastserverid(0)
{
}
void crAccountData::setNickName(const std::string &nickName)
{
	m_nickName = nickName;
}
const std::string &crAccountData::getNickName() const
{
	return m_nickName;
}
void crAccountData::setRelName(const std::string &relName)
{
	m_relname = relName;
}
const std::string &crAccountData::getRelName() const
{
	return m_relname;
}
void crAccountData::setSex(char sex)
{
	m_sex = sex;
}
char crAccountData::getSex()
{
	return m_sex;
}
void crAccountData::setAddress(const std::string &address)
{
	m_address = address;
}
const std::string &crAccountData::getAddress() const
{
	return m_address;
}
void crAccountData::setBirthday(const std::string &birthday)
{
	m_birthday = birthday;
}
const std::string &crAccountData::getBirthday() const
{
	return m_birthday;
}
void crAccountData::setIdiograph(const std::string &idiograph)
{
	m_idiograph = idiograph;
}
const std::string &crAccountData::getIdiograph() const
{
	return m_idiograph;
}
void crAccountData::setBloodtype(char bloodtype)
{
	m_bloodtype = bloodtype;
}
char crAccountData::getBloodtype()
{
	return m_bloodtype;
}
void crAccountData::setWork(char work)
{
	m_work = work;
}
char crAccountData::getWork()
{
	return m_work;
}
void crAccountData::setSchool(const std::string &school)
{
	m_school = school;
}
const std::string &crAccountData::getSchool() const
{
	return m_school;
}
void crAccountData::setEmail(const std::string &email)
{
	m_email = email;
}
const std::string &crAccountData::getEmail() const
{
	return m_email;
}
void crAccountData::setBlog(const std::string &blog)
{
	m_blog = blog;
}
const std::string &crAccountData::getBlog() const
{
	return m_blog;
}
void crAccountData::setTel(const std::string &tel)
{
	m_tel = tel;
}
const std::string &crAccountData::getTel() const
{
	return m_tel;
}
void crAccountData::setMobile(const std::string &mobile)
{
	m_mobile = m_mobile;
}
const std::string &crAccountData::getMobile() const
{
	return m_mobile;
}
void crAccountData::setNote(const std::string &note)
{
	m_note = note;
}
const std::string &crAccountData::getNote() const
{
	return m_note;
}
void crAccountData::setBeFriendValid(char befriendvalid)
{
	m_beFriendValid = befriendvalid;
}
char crAccountData::getBeFriendValid()
{
	return m_beFriendValid;
}
void crAccountData::setPrelogindate(const std::string &prelogindate)
{
	m_prelogindate = prelogindate;
}
const std::string &crAccountData::getPrelogindate() const
{
	return m_prelogindate;
}
void crAccountData::setLastServerID(int id)
{
	m_lastserverid = id;
}
int crAccountData::getLastServerID()
{
	return m_lastserverid;
}