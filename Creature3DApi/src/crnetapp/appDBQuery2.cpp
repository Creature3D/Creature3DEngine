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
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDataParam.h>
using namespace CRCore;
using namespace CRDataBase;
using namespace CRNetApp;

/////////////////////////////////////////
//
//crGameListLimitQueryData
//
/////////////////////////////////////////
crGameListLimitQueryData::crGameListLimitQueryData():
m_id(0),
m_rolecount(0)
{
}
crGameListLimitQueryData::crGameListLimitQueryData(const crGameListLimitQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_gamename(data.m_gamename),
m_cname(data.m_cname),
m_rolecount(data.m_rolecount)
{
}

void crGameListLimitQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_gamename = buf;
		break;
	case 2:
		m_cname = buf;
		break;
	case 3:
		m_rolecount = (unsigned char)(atoi(buf));
		break;
	}
}

void crGameListLimitQueryData::buildQuerySql(int rows,int offset)
{
	char sql[256];
	sprintf(sql,"select id,gamename,cname,rolecount from game limit %d,%d\0",rows,offset);
	setQuerySql(sql);
	setNumColumn(5);
}

int crGameListLimitQueryData::getID()
{
	return m_id;
}

const std::string& crGameListLimitQueryData::getGameName() const
{
	return m_gamename;
}

const std::string& crGameListLimitQueryData::getCName() const
{
	return m_cname;
}

//const std::string& crGameListLimitQueryData::getUIFile() const
//{
//	return m_uifile;
//}

unsigned char crGameListLimitQueryData::getRoleCount()
{
	return m_rolecount;
}
/////////////////////////////////////////
//
//crLoginKeyQueryData
//
/////////////////////////////////////////
crLoginKeyQueryData::crLoginKeyQueryData()
{
}
crLoginKeyQueryData::crLoginKeyQueryData(const crLoginKeyQueryData& data):
crQueryData(data),
m_key(data.m_key),
m_username(data.m_username)
{
}
void crLoginKeyQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_key = buf;
		break;
	case 1:
		m_username = buf;
		break;
	}
}
void crLoginKeyQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select loginkey,name from account where id=%d and timediff(now(),logindate)<='00:10:00'\0",playerid);
	setQuerySql(sql);
	setNumColumn(2);
}
const std::string& crLoginKeyQueryData::getKey() const
{
	return m_key;
}
const std::string &crLoginKeyQueryData::getUserName() const
{
	return m_username;
}
/////////////////////////////////////////
//
//crPlayerGameDBQueryData
//
/////////////////////////////////////////
crPlayerGameDBQueryData::crPlayerGameDBQueryData():
m_gamedbid(0)
{
}
crPlayerGameDBQueryData::crPlayerGameDBQueryData(const crPlayerGameDBQueryData& data):
crQueryData(data),
m_gamedbid(data.m_gamedbid)
{
}
void crPlayerGameDBQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_gamedbid = (unsigned short)(atoi(buf));
		break;
	}
}
void crPlayerGameDBQueryData::buildQuerySql(int playerid,int serverid)
{
	char sql[256];
	sprintf(sql,"select gamedbid from playergamedb where playerid=%d and serverid=%d\0",playerid,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
unsigned short crPlayerGameDBQueryData::getGameDBID()
{
	return m_gamedbid;
}
/////////////////////////////////////////
//
//crPlayerGameDataQueryData
//
/////////////////////////////////////////
crPlayerGameDataQueryData::crPlayerGameDataQueryData():
	m_id(0),
	m_mainroleid(0)
{
}
crPlayerGameDataQueryData::crPlayerGameDataQueryData(const crPlayerGameDataQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_mainroleid(data.m_mainroleid),
	m_stream(data.m_stream)
{
}
void crPlayerGameDataQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_mainroleid = atoi(buf);
		break;
	case 2:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crPlayerGameDataQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select id,mainroleid,stream from playergamedata where playerid=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}
int crPlayerGameDataQueryData::getID()
{
	return m_id;
}
int crPlayerGameDataQueryData::getMainRoleID()
{
	return m_mainroleid;
}
CRCore::crStreamBuf *crPlayerGameDataQueryData::getStream()
{
	return m_stream.get();
}
/////////////////////////////////////////
//
//crCharacterNameValidQueryData
//
/////////////////////////////////////////
crCharacterNameValidQueryData::crCharacterNameValidQueryData():
m_playerid(0)
{
}
crCharacterNameValidQueryData::crCharacterNameValidQueryData(const crCharacterNameValidQueryData& data):
crQueryData(data),
m_playerid(data.m_playerid)
{
}
void crCharacterNameValidQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	}
}
void crCharacterNameValidQueryData::buildQuerySql(const std::string &name, int serverid)
{
	char sql[128];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);
	sprintf(sql,"select playerid from charactername where name='%s' and serverid=%d\0",escname,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCharacterNameValidQueryData::getPlayeID()
{
	return m_playerid;
}
/////////////////////////////////////////
//
//crCharacterNameIDQueryData
//
/////////////////////////////////////////
crCharacterNameIDQueryData::crCharacterNameIDQueryData():
	m_id(0)
{
}
crCharacterNameIDQueryData::crCharacterNameIDQueryData(const crCharacterNameIDQueryData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crCharacterNameIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crCharacterNameIDQueryData::buildQuerySql(int playerid, int serverid, int roleid)
{
	char sql[128];
	sprintf(sql,"select id from charactername where playerid=%d and serverid=%d and roleid=%d\0",playerid,serverid,roleid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCharacterNameIDQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crCharacterNameQueryData
//
/////////////////////////////////////////
crCharacterNameQueryData::crCharacterNameQueryData()
{
}
crCharacterNameQueryData::crCharacterNameQueryData(const crCharacterNameQueryData& data):
	crQueryData(data),
	m_name(data.m_name)
{
}
void crCharacterNameQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_name = buf;
		break;
	}
}
void crCharacterNameQueryData::buildQuerySql(int playerid,int serverid,int roleid)
{
	char sql[128];
	sprintf(sql,"select name from charactername where playerid=%d and serverid=%d and roleid=%d\0",playerid,serverid,roleid);
	setQuerySql(sql);
	setNumColumn(1);
}
const std::string &crCharacterNameQueryData::getName() const
{
	return m_name;
}
/////////////////////////////////////////
//
//crCharacterNamePlayerIDQueryData
//
/////////////////////////////////////////
crCharacterNamePlayerIDQueryData::crCharacterNamePlayerIDQueryData()
{
}
crCharacterNamePlayerIDQueryData::crCharacterNamePlayerIDQueryData(const crCharacterNamePlayerIDQueryData& data):
	crQueryData(data),
	m_playerid(data.m_playerid)
{
}
void crCharacterNamePlayerIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	}
}
void crCharacterNamePlayerIDQueryData::buildQuerySql(const std::string &name, int serverid)
{
	char sql[128];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);
	sprintf(sql,"select playerid from charactername where name='%s'and serverid=%d\0",escname,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCharacterNamePlayerIDQueryData::getPlayerID()
{
	return m_playerid;
}
/////////////////////////////////////////
//
//crGameDBCountQueryData
//
/////////////////////////////////////////
crGameDBCountQueryData::crGameDBCountQueryData():
m_gamedbcount(0)
{
}
crGameDBCountQueryData::crGameDBCountQueryData(const crGameDBCountQueryData& data):
crQueryData(data),
m_gamedbcount(data.m_gamedbcount)
{
}
void crGameDBCountQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_gamedbcount = (unsigned short)(atoi(buf));
		break;
	}
}
void crGameDBCountQueryData::buildQuerySql(int gameid)
{
	char sql[256];
	sprintf(sql,"select dbcount from game where id=%d\0",gameid);
	setQuerySql(sql);
	setNumColumn(1);
}
unsigned short crGameDBCountQueryData::getGameDBCount()
{
	return m_gamedbcount;
}

/////////////////////////////////////////
//
//crGameRoleCountQueryData
//
/////////////////////////////////////////
crGameRoleCountQueryData::crGameRoleCountQueryData():
m_rolecount(0)
{
}
crGameRoleCountQueryData::crGameRoleCountQueryData(const crGameRoleCountQueryData& data):
crQueryData(data),
m_rolecount(data.m_rolecount)
{
}
void crGameRoleCountQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_rolecount = (unsigned char)(atoi(buf));
		break;
	}
}
void crGameRoleCountQueryData::buildQuerySql(int gameid)
{
	char sql[256];
	sprintf(sql,"select rolecount from game where id=%d\0",gameid);
	setQuerySql(sql);
	setNumColumn(1);
}
unsigned char crGameRoleCountQueryData::getRoleCount()
{
	return m_rolecount;
}

/////////////////////////////////////////
//
//crGameRoleQueryData
//
/////////////////////////////////////////
crGameRoleQueryData::crGameRoleQueryData():
m_id(0),
m_instanceitemid(0),
m_metierid(0)
{
}
crGameRoleQueryData::crGameRoleQueryData(const crGameRoleQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_instanceitemid(data.m_instanceitemid),
m_metierid(data.m_metierid)
{
}
void crGameRoleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_instanceitemid = atoi(buf);
		break;
	case 2:
		m_metierid = atoi(buf);
		break;
	}
}
void crGameRoleQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select id,instanceitemid,metierid from role where playerid=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}
int crGameRoleQueryData::getID()
{
	return m_id;
}
int crGameRoleQueryData::getInstanceItemID()
{
	return m_instanceitemid;
}
int crGameRoleQueryData::getMetierID()
{
	return m_metierid;
}
/////////////////////////////////////////
//
//crRoleCountQueryData
//
/////////////////////////////////////////
crRoleCountQueryData::crRoleCountQueryData():
m_count(0)
{
}
crRoleCountQueryData::crRoleCountQueryData(const crRoleCountQueryData& data):
crQueryData(data),
m_count(data.m_count)
{
}
void crRoleCountQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crRoleCountQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select count(id) from role where playerid=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crRoleCountQueryData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crRoleStreamQueryData
//
/////////////////////////////////////////
crRoleStreamQueryData::crRoleStreamQueryData()
{
}
crRoleStreamQueryData::crRoleStreamQueryData(const crRoleStreamQueryData& data):
crQueryData(data),
m_stream(data.m_stream)
{
}
void crRoleStreamQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crRoleStreamQueryData::buildQuerySql(int roleid)
{
	char sql[128];
	sprintf(sql,"select stream from role where id=%d\0",roleid);
	setQuerySql(sql);
	setNumColumn(1);
}
CRCore::crStreamBuf *crRoleStreamQueryData::getStream()
{
	return m_stream.get();
}

/////////////////////////////////////////
//
//crRoleMetierStreamQueryData
//
/////////////////////////////////////////
crRoleMetierStreamQueryData::crRoleMetierStreamQueryData()
{
}
crRoleMetierStreamQueryData::crRoleMetierStreamQueryData(const crRoleMetierStreamQueryData& data):
crQueryData(data),
m_metierStream(data.m_metierStream)
{
}
void crRoleMetierStreamQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		if(length>0)
		{
			m_metierStream = new crStreamBuf;
			m_metierStream->createBuf(length);
			m_metierStream->write(buf,length);
			m_metierStream->seekBegin();
		}
		break;
	}
}
void crRoleMetierStreamQueryData::buildQuerySql(int roleid)
{
	char sql[128];
	sprintf(sql,"select metierstream from role where id=%d\0",roleid);
	setQuerySql(sql);
	setNumColumn(1);
}
CRCore::crStreamBuf *crRoleMetierStreamQueryData::getMetierStream()
{
	return m_metierStream.get();
}
///////////////////////////////////////////
////
////crSceneInfoQueryData
////
///////////////////////////////////////////
//crSceneInfoQueryData::crSceneInfoQueryData()
//{
//}
//crSceneInfoQueryData::crSceneInfoQueryData(const crSceneInfoQueryData& data):
//crQueryData(data),
//m_scenename(data.m_scenename),
//m_filename(data.m_filename),
//m_collidefile(data.m_collidefile)
//{
//}
//void crSceneInfoQueryData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	case 0:
//		m_scenename = buf;
//		break;
//	case 1:
//		m_filename = buf;
//		break;
//	case 2:
//		m_collidefile = buf;
//		break;
//	}
//}
//void crSceneInfoQueryData::buildQuerySql(int sceneid)
//{
//	char sql[128];
//	sprintf(sql,"select scenename,filename,collidefile from scene where id = %d\0",sceneid);
//	setQuerySql(sql);
//	setNumColumn(3);
//}
//const std::string& crSceneInfoQueryData::getSceneName() const
//{
//    return m_scenename;
//}
//const std::string& crSceneInfoQueryData::getFileName() const
//{
//	return m_filename;
//}
//const std::string& crSceneInfoQueryData::getCollideFile() const
//{
//	return m_collidefile;
//}
/////////////////////////////////////////
//
//crGameIDQueryData
//
/////////////////////////////////////////
crGameIDQueryData::crGameIDQueryData():
m_id(0)
{
}
crGameIDQueryData::crGameIDQueryData(const crGameIDQueryData& data):
crQueryData(data),
m_id(data.m_id)
{
}
void crGameIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crGameIDQueryData::buildQuerySql(const std::string &gamename)
{
	char sql[128];
	int size = gamename.size();
	char escgamename[32];
	memset(escgamename,0,32);
	crDataBaseManager::getInstance()->escapeString(escgamename,gamename.c_str(),size);

	sprintf(sql,"select id from game where gamename='%s'\0",escgamename);
	setQuerySql(sql);
	setNumColumn(1);
}
int crGameIDQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crScene2QueryData
//
/////////////////////////////////////////
crScene2QueryData::crScene2QueryData()
{
}
crScene2QueryData::crScene2QueryData(const crScene2QueryData& data):
crQueryData(data),
m_gameid(data.m_gameid)
{
}
void crScene2QueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_gameid = atoi(buf);
		break;
	}
}
void crScene2QueryData::buildQuerySql(int id)
{
	char sql[128];
	sprintf(sql,"select gameid from scene where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(1);
}
int crScene2QueryData::getGameID()
{
	return m_gameid;
}
/////////////////////////////////////////
//
//crSceneIDQueryData
//
/////////////////////////////////////////
crSceneIDQueryData::crSceneIDQueryData():
m_id(0)
{
}
crSceneIDQueryData::crSceneIDQueryData(const crSceneIDQueryData& data):
crQueryData(data),
m_id(data.m_id)
{
}
void crSceneIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crSceneIDQueryData::buildQuerySql(int gameid,const std::string &scenename)
{
	char sql[128];
	int size = scenename.size();
	char escscenename[32];
	memset(escscenename,0,32);
	crDataBaseManager::getInstance()->escapeString(escscenename,scenename.c_str(),size);

	sprintf(sql,"select id from scene where gameid=%d and scenename='%s'\0",gameid,escscenename);
	setQuerySql(sql);
	setNumColumn(1);
}
int crSceneIDQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crSceneItemQueryData
//
/////////////////////////////////////////
crSceneItemQueryData::crSceneItemQueryData():
m_id(0),
m_abstractitemid(0),
m_itemtype(0),
m_layerid(0),
m_posx(0),
m_posy(0),
m_zoffset(0),
m_dirx(0),
m_diry(-1),
m_dirz(0)
{
}
crSceneItemQueryData::crSceneItemQueryData(const crSceneItemQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_abstractitemid(data.m_abstractitemid),
m_itemtype(data.m_itemtype),
m_iname(data.m_iname),
m_layerid(data.m_layerid),
m_posx(data.m_posx),
m_posy(data.m_posy),
m_zoffset(data.m_zoffset),
m_dirx(data.m_dirx),
m_diry(data.m_diry),
m_dirz(data.m_dirz),
m_stream(data.m_stream)
{
}
void crSceneItemQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_abstractitemid = atoi(buf);
		break;
	case 2:
		m_itemtype = (unsigned char)(atoi(buf));
		break;	
	case 3:
		m_iname = buf;
		break;
	case 4:
		m_layerid = (unsigned short)(atoi(buf));
		break;
	case 5:
		m_posx = atoi(buf);
		break;
	case 6:
		m_posy = atoi(buf);
		break;
	case 7:
		m_zoffset = (short)(atoi(buf));
		break;
	case 8:
		m_dirx = atof(buf);
		break;
	case 9:
		m_diry = atof(buf);
		break;
	case 10:
		m_dirz = atof(buf);
		break;
	case 11:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crSceneItemQueryData::buildQuerySql(int sceneid)
{
	char sql[256];
	sprintf(sql,"select id,abstractitemid,itemtype,iname,layerid,posx,posy,zoffset,dirx,diry,dirz,stream from instanceitem where sceneid=%d and itemtype!=2\0",sceneid);
	setQuerySql(sql);
	setNumColumn(12);
}
int crSceneItemQueryData::getID()
{
	return m_id;
}
int crSceneItemQueryData::getAbstractItemID()
{
	return m_abstractitemid;
}
unsigned char crSceneItemQueryData::getItemtype()
{
	return m_itemtype;
}
const std::string& crSceneItemQueryData::getIName() const
{
	return m_iname;
}
unsigned short crSceneItemQueryData::getLayerid()
{
	return m_layerid;
}
int crSceneItemQueryData::getPosx()
{
	return m_posx;
}
int crSceneItemQueryData::getPosy()
{
	return m_posy;
}
short crSceneItemQueryData::getZoffset()
{
	return m_zoffset;
}
float crSceneItemQueryData::getDirx()
{
	return m_dirx;
}
float crSceneItemQueryData::getDiry()
{
	return m_diry;
}
float crSceneItemQueryData::getDirz()
{
	return m_dirz;
}
CRCore::crStreamBuf *crSceneItemQueryData::getStream()
{
	return m_stream.get();
}
/////////////////////////////////////////
//
//crItemChildQueryData
//
/////////////////////////////////////////
crItemChildQueryData::crItemChildQueryData():
m_id(0),
m_instanceitemid(0)
{
}
crItemChildQueryData::crItemChildQueryData(const crItemChildQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_instanceitemid(data.m_instanceitemid)
{
}
void crItemChildQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_instanceitemid = atoi(buf);
		break;
	}
}
void crItemChildQueryData::buildQuerySql(int parentitemid)
{
	char sql[128];
	sprintf(sql,"select id,instanceitemid from itemchild where parentitemid=%d\0",parentitemid);
	setQuerySql(sql);
	setNumColumn(2);
}
int crItemChildQueryData::getID()
{
	return m_id;
}
int crItemChildQueryData::getInstanceitemid()
{
	return m_instanceitemid;
}
/////////////////////////////////////////
//
//crItemChild2QueryData
//
/////////////////////////////////////////
crItemChild2QueryData::crItemChild2QueryData():
m_instanceitemid(0)
{
}
crItemChild2QueryData::crItemChild2QueryData(const crItemChild2QueryData& data):
crQueryData(data),
m_instanceitemid(data.m_instanceitemid)
{
}
void crItemChild2QueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_instanceitemid = atoi(buf);
		break;
	}
}
void crItemChild2QueryData::buildQuerySql(int parentitemid)
{
	char sql[128];
	sprintf(sql,"select instanceitemid from itemchild where parentitemid=%d\0",parentitemid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crItemChild2QueryData::getInstanceitemid()
{
	return m_instanceitemid;
}
/////////////////////////////////////////
//
//crInstanceItemQueryData
//
/////////////////////////////////////////
crInstanceItemQueryData::crInstanceItemQueryData():
m_abstractitemid(0),
m_itemtype(0),
m_sceneid(0),
m_layerid(0),
m_posx(0),
m_posy(0),
m_zoffset(0),
m_dirx(0),
m_diry(-1),
m_dirz(0)
{
}
crInstanceItemQueryData::crInstanceItemQueryData(const crInstanceItemQueryData& data):
crQueryData(data),
m_abstractitemid(data.m_abstractitemid),
m_itemtype(data.m_itemtype),
m_iname(data.m_iname),
m_sceneid(data.m_sceneid),
m_layerid(data.m_layerid),
m_posx(data.m_posx),
m_posy(data.m_posy),
m_zoffset(data.m_zoffset),
m_dirx(data.m_dirx),
m_diry(data.m_diry),
m_dirz(data.m_dirz),
m_stream(data.m_stream)
{
}
void crInstanceItemQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_abstractitemid = atoi(buf);
		break;
	case 1:
		m_itemtype = (unsigned char)(atoi(buf));
		break;	
	case 2:
		m_iname = buf;
		break;
	case 3:
		m_sceneid = atoi(buf);
		break;
	case 4:
		m_layerid = (unsigned short)(atoi(buf));
		break;
	case 5:
		m_posx = atoi(buf);
		break;
	case 6:
		m_posy = atoi(buf);
		break;
	case 7:
		m_zoffset = (short)(atoi(buf));
		break;
	case 8:
		m_dirx = atof(buf);
		break;
	case 9:
		m_diry = atof(buf);
		break;
	case 10:
		m_dirz = atof(buf);
		break;
	case 11:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crInstanceItemQueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select abstractitemid,itemtype,iname,sceneid,layerid,posx,posy,zoffset,dirx,diry,dirz,stream from instanceitem where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(12);
}
int crInstanceItemQueryData::getAbstractItemID()
{
	return m_abstractitemid;
}
unsigned char crInstanceItemQueryData::getItemtype()
{
	return m_itemtype;
}
const std::string& crInstanceItemQueryData::getIName() const
{
	return m_iname;
}
int crInstanceItemQueryData::getSceneID()
{
	return m_sceneid;
}
unsigned short crInstanceItemQueryData::getLayerid()
{
	return m_layerid;
}
int crInstanceItemQueryData::getPosx()
{
	return m_posx;
}
int crInstanceItemQueryData::getPosy()
{
	return m_posy;
}
int crInstanceItemQueryData::getZoffset()
{
	return m_zoffset;
}
float crInstanceItemQueryData::getDirx()
{
	return m_dirx;
}
float crInstanceItemQueryData::getDiry()
{
	return m_diry;
}
float crInstanceItemQueryData::getDirz()
{
	return m_dirz;
}
CRCore::crStreamBuf *crInstanceItemQueryData::getStream()
{
	return m_stream.get();
}
/////////////////////////////////////////
//
//crInstanceItem2QueryData
//
/////////////////////////////////////////
crInstanceItem2QueryData::crInstanceItem2QueryData():
m_abstractitemid(0),
m_sceneid(0),
m_layerid(0)
{
}
crInstanceItem2QueryData::crInstanceItem2QueryData(const crInstanceItem2QueryData& data):
crQueryData(data),
m_abstractitemid(data.m_abstractitemid),
m_iname(data.m_iname),
m_sceneid(data.m_sceneid),
m_layerid(data.m_layerid),
m_stream(data.m_stream)
{
}
void crInstanceItem2QueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_abstractitemid = atoi(buf);
		break;
	case 1:
		m_iname = buf;
		break;
	case 2:
		m_sceneid = atoi(buf);
		break;
	case 3:
		m_layerid = (unsigned short)(atoi(buf));
		break;
	case 4:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crInstanceItem2QueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select abstractitemid,iname,sceneid,layerid,stream from instanceitem where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(5);
}
int crInstanceItem2QueryData::getAbstractItemID()
{
	return m_abstractitemid;
}
const std::string& crInstanceItem2QueryData::getIName() const
{
	return m_iname;
}
int crInstanceItem2QueryData::getSceneID()
{
	return m_sceneid;
}
unsigned short crInstanceItem2QueryData::getLayerid()
{
	return m_layerid;
}
CRCore::crStreamBuf *crInstanceItem2QueryData::getStream()
{
	return m_stream.get();
}
/////////////////////////////////////////
//
//crRoleQueryData
//
/////////////////////////////////////////
crRoleQueryData::crRoleQueryData():
m_instanceitemid(0),
m_abstractmetierid(0)
{
}
crRoleQueryData::crRoleQueryData(const crRoleQueryData& data):
crQueryData(data),
m_instanceitemid(data.m_instanceitemid),
m_abstractmetierid(data.m_abstractmetierid),
m_metierstream(data.m_metierstream)
{
}
void crRoleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_instanceitemid = atoi(buf);
		break;
	case 1:
		m_abstractmetierid = atoi(buf);
		break;
	case 2:
		if(length>0)
		{
			m_metierstream = new crStreamBuf;
			m_metierstream->createBuf(length);
			m_metierstream->write(buf,length);
			m_metierstream->seekBegin();
		}
		break;
	}
}
void crRoleQueryData::buildQuerySql(int roleid)
{
	char sql[256];
	sprintf(sql,"select instanceitemid,metierid,metierstream from role where id=%d\0",roleid);
	setQuerySql(sql);
	setNumColumn(3);
}
int crRoleQueryData::getInstanceItemID()
{
	return m_instanceitemid;
}
int crRoleQueryData::getAbstractMetierID()
{
	return m_abstractmetierid;
}
CRCore::crStreamBuf *crRoleQueryData::getMetierStream()
{
    return m_metierstream.get();
}
/////////////////////////////////////////
//
//crRoleQuery2Data
//
/////////////////////////////////////////
crRoleQuery2Data::crRoleQuery2Data():
	m_instanceitemid(0)
{
}
crRoleQuery2Data::crRoleQuery2Data(const crRoleQuery2Data& data):
	crQueryData(data),
	m_instanceitemid(data.m_instanceitemid)
{
}
void crRoleQuery2Data::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_instanceitemid = atoi(buf);
		break;
	}
}
void crRoleQuery2Data::buildQuerySql(int roleid)
{
	char sql[256];
	sprintf(sql,"select instanceitemid from role where id=%d\0",roleid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crRoleQuery2Data::getInstanceItemID()
{
	return m_instanceitemid;
}
/////////////////////////////////////////
//
//crAbstractItemQueryData
//
/////////////////////////////////////////
crAbstractItemQueryData::crAbstractItemQueryData():
m_resid(0)
{
}
crAbstractItemQueryData::crAbstractItemQueryData(const crAbstractItemQueryData& data):
crQueryData(data),
m_resid(data.m_resid),
m_name(data.m_name),
m_script(data.m_script)
{
}
void crAbstractItemQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_resid = atoi(buf);
		break;
	case 1:
		m_name = buf;
		break;
	case 2:
		m_script = buf;
		break;
	}
}
void crAbstractItemQueryData::buildQuerySql(int id)
{
	char sql[128];
	sprintf(sql,"select resid,name,script from abstractitem where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(3);
}
int crAbstractItemQueryData::getResID()
{
	return m_resid;
}
const std::string& crAbstractItemQueryData::getName() const
{
	return m_name;
}
const std::string& crAbstractItemQueryData::getScript() const
{
	return m_script;
}
///////////////////////////////////////////
////
////crAbstractItem2QueryData
////
///////////////////////////////////////////
//crAbstractItem2QueryData::crAbstractItem2QueryData()
////m_resid(0)
//{
//}
//crAbstractItem2QueryData::crAbstractItem2QueryData(const crAbstractItem2QueryData& data):
//crQueryData(data),
////m_resid(data.m_resid),
//m_script(data.m_script)
//{
//}
//void crAbstractItem2QueryData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	//case 0:
//	//	m_resid = atoi(buf);
//	//	break;
//	case 0:
//		m_script = buf;
//		break;
//	}
//}
//void crAbstractItem2QueryData::buildQuerySql(int id)
//{
//	char sql[128];
//	sprintf(sql,"select script from abstractitem where id = %d\0",id);
//	setQuerySql(sql);
//	setNumColumn(1);
//}
////int crAbstractItem2QueryData::getResID()
////{
////	return m_resid;
////}
//const std::string& crAbstractItem2QueryData::getScript() const
//{
//	return m_script;
//}
/////////////////////////////////////////
//
//crResQueryData
//
/////////////////////////////////////////
crResQueryData::crResQueryData()
{
}
crResQueryData::crResQueryData(const crResQueryData& data):
crQueryData(data),
m_filename(data.m_filename),
m_script(data.m_script)
{
}
void crResQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_filename = buf;
		break;
	case 1:
		m_script = buf;
		break;
	}
}
void crResQueryData::buildQuerySql(int id)
{
	char sql[128];
	sprintf(sql,"select filename,script from res where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(2);
}
const std::string& crResQueryData::getFileName() const
{
	return m_filename;
}
const std::string& crResQueryData::getScript() const
{
	return m_script;
}
/////////////////////////////////////////
//
//crRes2QueryData
//
/////////////////////////////////////////
crRes2QueryData::crRes2QueryData()
{
}
crRes2QueryData::crRes2QueryData(const crRes2QueryData& data):
crQueryData(data),
m_script(data.m_script)
{
}
void crRes2QueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_script = buf;
		break;
	}
}
void crRes2QueryData::buildQuerySql(int id)
{
	char sql[128];
	sprintf(sql,"select script from res where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(1);
}
const std::string& crRes2QueryData::getScript() const
{
	return m_script;
}
/////////////////////////////////////////
//
//crAbstractMetierQueryData
//
/////////////////////////////////////////
crAbstractMetierQueryData::crAbstractMetierQueryData():
m_gameid(0),
m_initsceneid(0),
m_initlayerid(0)
{
}
crAbstractMetierQueryData::crAbstractMetierQueryData(const crAbstractMetierQueryData& data):
crQueryData(data),
m_gameid(data.m_gameid),
m_metiername(data.m_metiername),
m_initsceneid(data.m_initsceneid),
m_initlayerid(data.m_initlayerid),
m_script(data.m_script),
m_tasktable(data.m_tasktable)
{
}
void crAbstractMetierQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_gameid = atoi(buf);
		break;
	case 1:
		m_metiername = buf;
		break;
	case 2:
		m_initsceneid = atoi(buf);
		break;
	case 3:
		m_initlayerid = (unsigned short)(atoi(buf));
		break;
	case 4:
		m_zoffset = (short)(atoi(buf));
		break;
	case 5:
		m_script = buf;
		break;
	case 6:
		m_tasktable = buf;
		break;
	}
}
void crAbstractMetierQueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select gameid,metiername,initsceneid,initlayerid,zoffset,script,tasktable from abstractmetier where id=%d\0",id);
	setQuerySql(sql);
	setNumColumn(7);
}
int crAbstractMetierQueryData::getGameID()
{
	return m_gameid;
}
const std::string& crAbstractMetierQueryData::getMetierName() const
{
	return m_metiername;
}
int crAbstractMetierQueryData::getInitSceneID()
{
	return m_initsceneid;
}
unsigned short crAbstractMetierQueryData::getInitLayerID()
{
	return m_initlayerid;
}
short crAbstractMetierQueryData::getZoffset()
{
	return m_zoffset;
}
const std::string& crAbstractMetierQueryData::getScript() const
{
	return m_script;
}
const std::string& crAbstractMetierQueryData::getTaskTable() const
{
	return m_tasktable;
}
///////////////////////////////////////////
////
////crAbstractMetier2QueryData
////
///////////////////////////////////////////
//crAbstractMetier2QueryData::crAbstractMetier2QueryData():
//m_initsceneid(0),
//m_initlayerid(0),
//m_zoffset(0)
//{
//}
//crAbstractMetier2QueryData::crAbstractMetier2QueryData(const crAbstractMetier2QueryData& data):
//crQueryData(data),
//m_initsceneid(data.m_initsceneid),
//m_initlayerid(data.m_initlayerid),
//m_zoffset(data.m_zoffset)
////m_script(data.m_script),
////m_tasktable(data.m_tasktable)
//{
//}
//void crAbstractMetier2QueryData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	case 0:
//		m_initsceneid = atoi(buf);
//		break;
//	case 1:
//		m_initlayerid = (unsigned short)(atoi(buf));
//		break;
//	case 2:
//		m_zoffset = (short)(atoi(buf));
//		break;
//	//case 3:
//	//	m_script = buf;
//	//	break;
//	//case 4:
//	//	m_tasktable = buf;
//	//	break;
//	}
//}
//void crAbstractMetier2QueryData::buildQuerySql(int id/*, int gameid*/)
//{
//	char sql[256];
//	sprintf(sql,"select initsceneid, initlayerid, zoffset from abstractmetier where id = %d\0",id);
//	setQuerySql(sql);
//	setNumColumn(3);
//}
//int crAbstractMetier2QueryData::getInitSceneID()
//{
//	return m_initsceneid;
//}
//unsigned short crAbstractMetier2QueryData::getInitLayerID()
//{
//	return m_initlayerid;
//}
////int crAbstractMetier2QueryData::getInitx()
////{
////	return m_initx;
////}
////int crAbstractMetier2QueryData::getInity()
////{
////	return m_inity;
////}
////unsigned short crAbstractMetier2QueryData::getInitR()
////{
////	return m_initr;
////}
//short crAbstractMetier2QueryData::getZoffset()
//{
//	return m_zoffset;
//}
////const std::string& crAbstractMetier2QueryData::getScript() const
////{
////	return m_script;
////}
////const std::string& crAbstractMetier2QueryData::getTaskTable() const
////{
////	return m_tasktable;
////}
/////////////////////////////////////////
//
//crSceneListQueryData
//
/////////////////////////////////////////
crSceneListQueryData::crSceneListQueryData()
{
}
crSceneListQueryData::crSceneListQueryData(const crSceneListQueryData& data):
crQueryData(data),
m_sceneid(data.m_sceneid),
m_scenename(data.m_scenename)
{
}
void crSceneListQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_sceneid = atoi(buf);
		break;
	case 1:
		m_scenename = buf;
		break;
	}
}
void crSceneListQueryData::buildQuerySql(int gameid,int rows, int offset)
{
	char sql[128];
	sprintf(sql,"select id,scenename from scene where gameid=%d limit %d,%d\0",gameid,rows,offset);
	setQuerySql(sql);
	setNumColumn(2);
}
int crSceneListQueryData::getSceneID()
{
	return m_sceneid;
}
const std::string& crSceneListQueryData::getSceneName() const
{
	return m_scenename;
}
/////////////////////////////////////////
//
//crRemoveRoleQueryData
//
/////////////////////////////////////////
crRemoveRoleQueryData::crRemoveRoleQueryData():
m_playerid(0),
m_instanceitemid(0)
{
}
crRemoveRoleQueryData::crRemoveRoleQueryData(const crRemoveRoleQueryData& data):
crQueryData(data),
m_playerid(data.m_playerid),
m_instanceitemid(data.m_instanceitemid)
{
}
void crRemoveRoleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	case 1:
		m_instanceitemid = atoi(buf);
		break;
	}
}
void crRemoveRoleQueryData::buildQuerySql(int roleid)
{
	char sql[256];
	sprintf(sql,"select playerid,instanceitemid from role where id=%d\0",roleid);
	setQuerySql(sql);
	setNumColumn(2);
}
int crRemoveRoleQueryData::getPlayerID()
{
	return m_playerid;
}
int crRemoveRoleQueryData::getInstanceItemID()
{
	return m_instanceitemid;
}
/////////////////////////////////////////
//
//crPlayerSaleQueryData
//
/////////////////////////////////////////
crPlayerSaleQueryData::crPlayerSaleQueryData():
m_id(0),
m_price(0),
m_salecount(0)
{
}
crPlayerSaleQueryData::crPlayerSaleQueryData(const crPlayerSaleQueryData& data):
crQueryData(data),
m_id(data.m_id),
m_playername(data.m_playername),
m_price(data.m_price),
m_salecount(data.m_salecount)
{
}
void crPlayerSaleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_playername = buf;
		break;
	case 2:
		m_price = atoi(buf);
		break;
	case 3:
		m_salecount = atoi(buf);
		break;
	}
}
void crPlayerSaleQueryData::buildQuerySql(int goodstype,int goodsid,unsigned char salestate,int start,int count)
{
	char sql[256];
	sprintf(sql,"select id,playername,price,salecount from sale where goodstype=%d and goodsid=%d and salestate=%d limit %d,%d\0",goodstype,goodsid,(char)salestate,start,count);
	setQuerySql(sql);
	setNumColumn(4);
}
int crPlayerSaleQueryData::getID()
{
	return m_id;
}
const std::string &crPlayerSaleQueryData::getPlayerName() const
{
	return m_playername;
}
int crPlayerSaleQueryData::getPrice()
{
	return m_price;
}
int crPlayerSaleQueryData::getSaleCount()
{
	return m_salecount;
}
/////////////////////////////////////////
//
//crCountPlayerSaleQueryData
//
/////////////////////////////////////////
crCountPlayerSaleQueryData::crCountPlayerSaleQueryData():
m_count(0)
{
}
crCountPlayerSaleQueryData::crCountPlayerSaleQueryData(const crCountPlayerSaleQueryData& data):
crQueryData(data),
m_count(data.m_count)
{
}
void crCountPlayerSaleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crCountPlayerSaleQueryData::buildQuerySql(int goodstype,int goodsid)
{
	char sql[256];
	sprintf(sql,"select count(id) from sale where goodstype=%d and goodsid=%d and salestate=%d\0",goodstype,goodsid,Sale_OnSale);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCountPlayerSaleQueryData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crBuySaleQueryData
//
/////////////////////////////////////////
crBuySaleQueryData::crBuySaleQueryData():
	m_goodstype(0),
	m_goodsid(0),
	m_price(0),
	m_salecount(0)
{
}
crBuySaleQueryData::crBuySaleQueryData(const crBuySaleQueryData& data):
	crQueryData(data),
	m_goodstype(data.m_goodstype),
	m_goodsid(data.m_goodsid),
	m_price(data.m_price),
	m_salecount(data.m_salecount)
{
}
void crBuySaleQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_goodstype = atoi(buf);
		break;
	case 1:
		m_goodsid = atoi(buf);
		break;
	case 2:
		m_price = atoi(buf);
		break;
	case 3:
		m_salecount = atoi(buf);
		break;
	}
}
void crBuySaleQueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select goodstype,goodsid,price,salecount from sale where id=%d and salestate=%d\0",id,Sale_OnSale);
	setQuerySql(sql);
	setNumColumn(4);
}
int crBuySaleQueryData::getGoodsType()
{
	return m_goodstype;
}
int crBuySaleQueryData::getGoodsID()
{
	return m_goodsid;
}
int crBuySaleQueryData::getPrice()
{
	return m_price;
}
int crBuySaleQueryData::getSaleCount()
{
	return m_salecount;
}
/////////////////////////////////////////
//
//crMySaleStateQueryData
//
/////////////////////////////////////////
crMySaleStateQueryData::crMySaleStateQueryData():
	m_salestate(0),
	m_gaincash(0)
{
}
crMySaleStateQueryData::crMySaleStateQueryData(const crMySaleStateQueryData& data):
	crQueryData(data),
	m_salestate(data.m_salestate),
	m_gaincash(data.m_gaincash)
{
}
void crMySaleStateQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_salestate = (unsigned char)(atoi(buf));
		break;
	case 1:
		m_gaincash = atoi(buf);
		break;
	}
}
void crMySaleStateQueryData::buildQuerySql(int id)
{
	char sql[256];
	sprintf(sql,"select salestate,gaincash from sale where id=%d and salestate<>%d\0",id,Sale_OnSale);
	setQuerySql(sql);
	setNumColumn(2);
}
unsigned char crMySaleStateQueryData::getSaleState()
{
	return m_salestate;
}
int crMySaleStateQueryData::getGaincash()
{
	return m_gaincash;
}
/////////////////////////////////////////
//
//crServerListIDQueryData
//
/////////////////////////////////////////
crServerListIDQueryData::crServerListIDQueryData():
	m_id(0),
	m_webserverid(0)
{
}
crServerListIDQueryData::crServerListIDQueryData(const crServerListIDQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_webserverid(data.m_webserverid),
	m_site(data.m_site)
{
}
void crServerListIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_webserverid = atoi(buf);
		break;
	case 2:
		m_site = buf;
		break;
	}
}
void crServerListIDQueryData::buildQuerySql(int gameid,const std::string &serverid)
{
	char sql[256];
	int size = serverid.size();
	char escserverid[32];
	memset(escserverid,0,32);
	crDataBaseManager::getInstance()->escapeString(escserverid,serverid.c_str(),size);
	sprintf(sql,"select id,webserverid,site from serverlist where gameid=%d and serverid='%s'\0",gameid,escserverid);
	setQuerySql(sql);
	setNumColumn(3);
}
int crServerListIDQueryData::getID()
{
	return m_id;
}
int crServerListIDQueryData::getWebServerID()
{
	return m_webserverid;
}
const std::string &crServerListIDQueryData::getSite() const
{
	return m_site;
}

/////////////////////////////////////////
//
//crServerListQueryData
//
/////////////////////////////////////////
crServerListQueryData::crServerListQueryData():
	m_id(0),
	m_isp(0),
	m_remaincount(0)
{
}
crServerListQueryData::crServerListQueryData(const crServerListQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_servername(data.m_servername),
	m_isp(data.m_isp),
	m_remaincount(data.m_remaincount)
{
}
void crServerListQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_servername = buf;
		break;
	case 2:
		m_isp = (unsigned char)(atoi(buf));
		break;
	case 3:
		m_remaincount = atoi(buf);
		break;
	}
}
void crServerListQueryData::buildQuerySql(int gameid)
{//过滤掉已经轮回的
	char sql[256];
	sprintf(sql,"select id,servername,isp,remaincount from serverlist where gameid=%d and remaincount<>-1001 and site is null\0",gameid);
	setQuerySql(sql);
	setNumColumn(4);
}
void crServerListQueryData::buildQuerySql_gm(int gameid)
{//过滤掉已经轮回的
	char sql[256];
	sprintf(sql,"select id,servername,isp,remaincount from serverlist where gameid=%d and remaincount<>-1001\0",gameid);
	setQuerySql(sql);
	setNumColumn(4);
}
int crServerListQueryData::getID()
{
	return m_id;
}
const std::string &crServerListQueryData::getServerName() const
{
	return m_servername;
}
unsigned char crServerListQueryData::getISP()
{
	return m_isp;
}
short crServerListQueryData::getRemainCount()
{
	return m_remaincount;
}
/////////////////////////////////////////
//
//crConnectServerIDQueryData
//
/////////////////////////////////////////
crConnectServerIDQueryData::crConnectServerIDQueryData():
	m_id(0)
{
}
crConnectServerIDQueryData::crConnectServerIDQueryData(const crConnectServerIDQueryData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crConnectServerIDQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crConnectServerIDQueryData::buildQuerySql(int serverid,unsigned short port)
{
	char sql[256];
	//int size = ip.size();
	//char escip[32];
	//memset(escip,0,32);
	//crDataBaseManager::getInstance()->escapeString(escip,ip.c_str(),size);
	sprintf(sql,"select id from connectservers where serverid=%d and port=%d\0",serverid,(short)port);
	setQuerySql(sql);
	setNumColumn(1);
}
int crConnectServerIDQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crConnectServersQueryData
//
/////////////////////////////////////////
crConnectServersQueryData::crConnectServersQueryData():
	m_port(0),
	m_remaincount(0)
{
}
crConnectServersQueryData::crConnectServersQueryData(const crConnectServersQueryData& data):
	crQueryData(data),
	m_ip(data.m_ip),
	m_ip2(data.m_ip2),
	m_ip3(data.m_ip3),
	m_port(data.m_port),
	m_remaincount(data.m_remaincount)
{
}
void crConnectServersQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_ip = buf;
		break;
	case 1:
		m_ip2 = buf;
		break;
	case 2:
		m_ip3 = buf;
		break;
	case 3:
		m_port = atoi(buf);
		break;
	case 4:
		m_remaincount = atoi(buf);
		break;
	}
}
void crConnectServersQueryData::buildQuerySql(int serverid)
{
	char sql[256];
	sprintf(sql,"select ip,ip2,ip3,port,remaincount from connectservers where serverid=%d and timediff(now(),updatedate)<='00:02:00'\0 \0",serverid);
	setQuerySql(sql);
	setNumColumn(5);
}
const std::string &crConnectServersQueryData::getIP() const
{
	return m_ip;
}
const std::string &crConnectServersQueryData::getIP2() const
{
	return m_ip2;
}
const std::string &crConnectServersQueryData::getIP3() const
{
	return m_ip3;
}
unsigned short crConnectServersQueryData::getPort()
{
	return m_port;
}
short crConnectServersQueryData::getRemainCount()
{
	return m_remaincount;
}
/////////////////////////////////////////
//
//crActivecodeQueryData
//
/////////////////////////////////////////
crActivecodeQueryData::crActivecodeQueryData():
	m_id(0),
	m_used(false)
{
}
crActivecodeQueryData::crActivecodeQueryData(const crActivecodeQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_used(data.m_used),
	m_validdate(data.m_validdate)
{
}
void crActivecodeQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_used = bool(atoi(buf));
		break;
	case 2:
		m_validdate = buf;
		break;
	}
}
void crActivecodeQueryData::buildQuerySql(const std::string &code)
{
	char sql[128];
	sprintf(sql,"select id,used,validdate from activecode where code='%s'\0",code.c_str());
	setQuerySql(sql);
	setNumColumn(3);
}
int crActivecodeQueryData::getID()
{
	return m_id;
}
bool crActivecodeQueryData::getUsed()
{
	return m_used;
}
const std::string &crActivecodeQueryData::getValidDate()
{
	return m_validdate;
}
///////////////////////////////////////////
////
////crPlayerMoneyExistQueryData
////
///////////////////////////////////////////
//crPlayerMoneyExistQueryData::crPlayerMoneyExistQueryData()
//{
//}
//crPlayerMoneyExistQueryData::crPlayerMoneyExistQueryData(const crPlayerMoneyExistQueryData& data):
//	crQueryData(data)
//{
//}
//void crPlayerMoneyExistQueryData::addParam(int i, char *buf, int length)
//{
//}
//void crPlayerMoneyExistQueryData::buildQuerySql(int playerid,int serverid)
//{
//	char sql[128];
//	sprintf(sql,"select 1 from playermoney where playerid=%d and serverid=%d limit 1\0",playerid,serverid);
//	setQuerySql(sql);
//}
/////////////////////////////////////////
//
//crPlayerMoneyQueryData
//
/////////////////////////////////////////
crPlayerMoneyQueryData::crPlayerMoneyQueryData():
	m_money(0)
{
}
crPlayerMoneyQueryData::crPlayerMoneyQueryData(const crPlayerMoneyQueryData& data):
	crQueryData(data),
	m_money(data.m_money)
{
}
void crPlayerMoneyQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_money = atoi(buf);
		break;
	}
}
void crPlayerMoneyQueryData::buildQuerySql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"select money from playermoney where playerid=%d and serverid=%d\0",playerid,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crPlayerMoneyQueryData::getMoney()
{
	return m_money;
}
///////////////////////////////////////////
////
////crRechargerecordQueryData
////
///////////////////////////////////////////
//crRechargerecordQueryData::crRechargerecordQueryData():
//	m_id(0),
//	m_playerid(0),
//	m_money(0)
//{
//}
//crRechargerecordQueryData::crRechargerecordQueryData(const crRechargerecordQueryData& data):
//	crQueryData(data),
//	m_id(data.m_id),
//	m_playerid(data.m_playerid),
//	m_money(data.m_money)
//{
//}
//void crRechargerecordQueryData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	case 0:
//		m_id = atoi(buf);
//		break;
//	case 1:
//		m_playerid = atoi(buf);
//		break;
//	case 2:
//		m_money = atoi(buf);
//		break;
//	}
//}
//void crRechargerecordQueryData::buildQuerySql(int serverid,int count)
//{
//	char sql[128];
//	sprintf(sql,"select id,playerid,money from Rechargerecord where serverid=%d and stats=1 limit %d\0",serverid,count);
//	setQuerySql(sql);
//	setNumColumn(3);
//}
//int crRechargerecordQueryData::getID()
//{
//	return m_id;
//}
//int crRechargerecordQueryData::getPlayerID()
//{
//	return m_playerid;
//}
//int crRechargerecordQueryData::getMoney()
//{
//	return m_money;
//}
/////////////////////////////////////////
//
//crGamelogOverdueQueryData
//
/////////////////////////////////////////
crGamelogOverdueQueryData::crGamelogOverdueQueryData():
	m_id(0)
{
}
crGamelogOverdueQueryData::crGamelogOverdueQueryData(const crGamelogOverdueQueryData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crGamelogOverdueQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crGamelogOverdueQueryData::buildQuerySql(int overdue)
{
	char sql[128];
	sprintf(sql,"select id from gamelog where to_days(now())-to_days(createdate)>=%d\0",overdue);
	setQuerySql(sql);
	setNumColumn(1);
}
int crGamelogOverdueQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crPlayerVipQueryData
//
/////////////////////////////////////////
crPlayerVipQueryData::crPlayerVipQueryData():
	m_viplv(0),
	m_vipexp(0)
{
}
crPlayerVipQueryData::crPlayerVipQueryData(const crPlayerVipQueryData& data):
	crQueryData(data),
	m_viplv(data.m_viplv),
	m_vipexp(data.m_vipexp)
{
}
void crPlayerVipQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_viplv = (unsigned char)atoi(buf);
		break;
	case 1:
		m_vipexp = atoi(buf);
		break;
	}
}
void crPlayerVipQueryData::buildQuerySql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"select viplv,vipexp from playervip where playerid=%d and serverid=%d\0",playerid,serverid);
	setQuerySql(sql);
	setNumColumn(2);
}
unsigned char crPlayerVipQueryData::getViplv()
{
	return m_viplv;
}
int crPlayerVipQueryData::getVipexp()
{
	return m_vipexp;
}
/////////////////////////////////////////
//
//crGameInfoQueryData
//
/////////////////////////////////////////
crGameInfoQueryData::crGameInfoQueryData()
{
}
crGameInfoQueryData::crGameInfoQueryData(const crGameInfoQueryData& data):
	crQueryData(data),
	m_cname(data.m_cname),
	m_platform(data.m_platform)
{
}
void crGameInfoQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_cname = buf;
		break;
	case 1:
		m_platform = buf;
		break;
	}
}
void crGameInfoQueryData::buildQuerySql(int gameid)
{
	char sql[128];
	sprintf(sql,"select cname,platform from game where id=%d\0",gameid);
	setQuerySql(sql);
	setNumColumn(2);
}
const std::string &crGameInfoQueryData::getCName() const
{
	return m_cname;
}
const std::string &crGameInfoQueryData::getPlatform() const
{
	return m_platform;
}
/////////////////////////////////////////
//
//crGmQueryData
//
/////////////////////////////////////////
crGmQueryData::crGmQueryData():
	m_playerid(0),
	m_authorityid(0)
{
}
crGmQueryData::crGmQueryData(const crGmQueryData& data):
	crQueryData(data),
	m_playerid(data.m_playerid),
	m_authorityid(data.m_authorityid)
{
}
void crGmQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	case 1:
		m_authorityid = atoi(buf);
		break;
	}
}
void crGmQueryData::buildQuerySql()
{
	char sql[128];
	sprintf(sql,"select playerid,authorityid from gm where serverid=0\0");
	setQuerySql(sql);
	setNumColumn(2);
}
int crGmQueryData::getPlayerID()
{
	return m_playerid;
}
unsigned char crGmQueryData::getAuthorityid()
{
	return m_authorityid;
}