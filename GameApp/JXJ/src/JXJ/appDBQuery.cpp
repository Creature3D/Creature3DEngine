/**********************************************************************
*
*	文件:	   appDBQuery.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*	HISTORY:		Created 2013.6.4
*
**********************************************************************/
#include <JXJ/appDBQuery.h>
#include <JXJ/appDataParam.h>
using namespace JXJ;
using namespace CRDataBase;
using namespace CRCore;
/////////////////////////////////////////
//
//crMailQueryData
//
/////////////////////////////////////////
crMailQueryData::crMailQueryData():
	m_id(0),
	m_stats(0)
{
}
crMailQueryData::crMailQueryData(const crMailQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_sendername(data.m_sendername),
	m_title(data.m_title),
	m_stats(data.m_stats),
	m_createdate(data.m_createdate)
{
}
void crMailQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_sendername = buf;
		break;
	case 2:
		m_title = buf;
		break;
	case 3:
		m_stats = (unsigned char)(atoi(buf));
		break;
	case 4:
		m_createdate = buf;
		break;
	}
}
void crMailQueryData::buildQuerySql(int playerid,int start,int count)
{
	char sql[512];
	sprintf(sql,"select id,sendername,title,stats,createdate from mail where playerid=%d and stats<>%d order by id desc limit %d,%d\0",playerid,Mail_Deleted,start,count);
	setQuerySql(sql);
	setNumColumn(5);
}
int crMailQueryData::getID()
{
	return m_id;
}
const std::string &crMailQueryData::getSenderName() const
{
	return m_sendername;
}
const std::string &crMailQueryData::getTitle() const
{
	return m_title;
}
unsigned char crMailQueryData::getStats()
{
	return m_stats;
}
const std::string &crMailQueryData::getCreateDate() const
{
	return m_createdate;
}
/////////////////////////////////////////
//
//crMailByStatsQueryData
//
/////////////////////////////////////////
crMailByStatsQueryData::crMailByStatsQueryData():
	m_id(0)
{
}
crMailByStatsQueryData::crMailByStatsQueryData(const crMailByStatsQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_sendername(data.m_sendername),
	m_title(data.m_title),
	m_createdate(data.m_createdate)
{
}
void crMailByStatsQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_sendername = buf;
		break;
	case 2:
		m_title = buf;
		break;
	case 3:
		m_createdate = buf;
		break;
	}
}
void crMailByStatsQueryData::buildQuerySql(int playerid,unsigned char stats,int start,int count)
{
	char sql[512];
	if (stats == Mail_Readed)
	{
		sprintf(sql, "select id,sendername,title,createdate from mail where playerid=%d and (stats=%d or stats=%d) order by id desc limit %d,%d\0", playerid, (int)stats,Mail_BeenGetted, start, count);
	}
	else
	{
		sprintf(sql, "select id,sendername,title,createdate from mail where playerid=%d and stats=%d order by id desc limit %d,%d\0", playerid, (int)stats, start, count);
	}
	
	setQuerySql(sql);
	setNumColumn(4);
}
int crMailByStatsQueryData::getID()
{
	return m_id;
}
const std::string &crMailByStatsQueryData::getSenderName() const
{
	return m_sendername;
}
const std::string &crMailByStatsQueryData::getTitle() const
{
	return m_title;
}
const std::string &crMailByStatsQueryData::getCreateDate() const
{
	return m_createdate;
}
/////////////////////////////////////////
//
//crCountMailQueryData
//
/////////////////////////////////////////
crCountMailQueryData::crCountMailQueryData():
	m_count(0)
{
}
crCountMailQueryData::crCountMailQueryData(const crCountMailQueryData& data):
	crQueryData(data),
	m_count(data.m_count)
{
}
void crCountMailQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crCountMailQueryData::buildQuerySql(int playerid)
{
	char sql[128];
	sprintf(sql,"select count(id) from mail where playerid=%d and stats<>%d\0",playerid,Mail_Deleted);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCountMailQueryData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crCountMailByStatsQueryData
//
/////////////////////////////////////////
crCountMailByStatsQueryData::crCountMailByStatsQueryData():
	m_count(0)
{
}
crCountMailByStatsQueryData::crCountMailByStatsQueryData(const crCountMailByStatsQueryData& data):
	crQueryData(data),
	m_count(data.m_count)
{
}
void crCountMailByStatsQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crCountMailByStatsQueryData::buildQuerySql(int playerid,unsigned char stats)
{
	char sql[256];
	sprintf(sql,"select count(id) from mail where playerid=%d and stats=%d\0",playerid,(int)stats);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCountMailByStatsQueryData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crMailBodyQueryData
//
/////////////////////////////////////////
crMailBodyQueryData::crMailBodyQueryData()
{
}
crMailBodyQueryData::crMailBodyQueryData(const crMailBodyQueryData& data):
	crQueryData(data),
	m_body(data.m_body)
{
}
void crMailBodyQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		if(length>0)
		{
			m_body = new crStreamBuf;
			m_body->createBuf(length);
			m_body->write(buf,length);
			m_body->seekBegin();
		}
		break;
	}
}
void crMailBodyQueryData::buildQuerySql(int id)
{
	char sql[512];
	sprintf(sql,"select body from mail where id = %d\0",id);
	setQuerySql(sql);
	setNumColumn(1);
}
CRCore::crStreamBuf *crMailBodyQueryData::getBody()
{
	return m_body.get();
}
/////////////////////////////////////////
//
//crGiftsPackKeyValidQueryData
//
/////////////////////////////////////////
crGiftsPackKeyValidQueryData::crGiftsPackKeyValidQueryData():
	m_count(0)
{
}
crGiftsPackKeyValidQueryData::crGiftsPackKeyValidQueryData(const crGiftsPackKeyValidQueryData& data):
	crQueryData(data),
	m_count(data.m_count)
{
}
void crGiftsPackKeyValidQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crGiftsPackKeyValidQueryData::buildQuerySql(const std::string &key)
{
	char sql[128];
	int size = key.size();
	char esckey[32];
	memset(esckey,0,32);
	crDataBaseManager::getInstance()->escapeString(esckey,key.c_str(),size);
	sprintf(sql,"select count(id) from giftspack where keyword='%s'\0",esckey);
	setQuerySql(sql);
	setNumColumn(1);
}
int crGiftsPackKeyValidQueryData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crGiftsPackQueryData
//
/////////////////////////////////////////
crGiftsPackQueryData::crGiftsPackQueryData():
	m_id(0),
	m_playerid(0),
	m_distill(0)
{
}
crGiftsPackQueryData::crGiftsPackQueryData(const crGiftsPackQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_playerid(data.m_playerid),
	m_pack(data.m_pack)
{
}
void crGiftsPackQueryData::addParam(int i, char *buf, int length)
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
		m_distill = atoi(buf);
		break;
	case 3:
		if(length>0)
		{
			m_pack = new crStreamBuf;
			m_pack->createBuf(length);
			m_pack->write(buf,length);
			m_pack->seekBegin();
		}
		break;
	}
}
void crGiftsPackQueryData::buildQuerySql(const std::string &key)
{
	char sql[128];
	int size = key.size();
	char esckey[32];
	memset(esckey,0,32);
	crDataBaseManager::getInstance()->escapeString(esckey,key.c_str(),size);
	sprintf(sql,"select id,playerid,distill,pack from giftspack where keyword='%s'\0",esckey);
	setQuerySql(sql);
	setNumColumn(4);
}
int crGiftsPackQueryData::getID()
{
	return m_id;
}
int crGiftsPackQueryData::getPlayerID()
{
	return m_playerid;
}
int crGiftsPackQueryData::getDistill()
{
	return m_distill;
}
CRCore::crStreamBuf *crGiftsPackQueryData::getPack()
{
	return m_pack.get();
}
/////////////////////////////////////////
//
//crGiftsPackOverdueQueryData
//
/////////////////////////////////////////
crGiftsPackOverdueQueryData::crGiftsPackOverdueQueryData():
	m_id(0)
{
}
crGiftsPackOverdueQueryData::crGiftsPackOverdueQueryData(const crGiftsPackOverdueQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_key(data.m_key)
{
}
void crGiftsPackOverdueQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_key = buf;
		break;
	}
}
void crGiftsPackOverdueQueryData::buildQuerySql(int overdue)
{
	char sql[128];
	sprintf(sql,"select id,keyword from giftspack where to_days(now())-to_days(createdate)>=%d\0",overdue);
	setQuerySql(sql);
	setNumColumn(2);
}
int crGiftsPackOverdueQueryData::getID()
{
	return m_id;
}
const std::string &crGiftsPackOverdueQueryData::getKey()const
{
	return m_key;
}
/////////////////////////////////////////
//
//crMailOverdueQueryData
//
/////////////////////////////////////////
crMailOverdueQueryData::crMailOverdueQueryData():
	m_id(0)
{
}
crMailOverdueQueryData::crMailOverdueQueryData(const crMailOverdueQueryData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crMailOverdueQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crMailOverdueQueryData::buildQuerySql(int overdue)
{
	char sql[128];
	sprintf(sql,"select id from mail where to_days(now())-to_days(createdate)>=%d\0",overdue);
	setQuerySql(sql);
	setNumColumn(1);
}
int crMailOverdueQueryData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crConsortiaQueryData
//
/////////////////////////////////////////
crConsortiaQueryData::crConsortiaQueryData():
	m_id(0)
{
}
crConsortiaQueryData::crConsortiaQueryData(const crConsortiaQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_stream(data.m_stream),
	m_createdate(data.m_createdate)
{
}
void crConsortiaQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	case 2:
		m_createdate = buf;
		break;
	case 3:
		m_updateDate = buf;
		break;
	}
}
void crConsortiaQueryData::buildQuerySql()
{
	//char sql[128];
	//sprintf(sql,"select id,stream,createdate from consortia\0");
	//setQuerySql(sql);
	//setNumColumn(3);

	char sql[128];
	sprintf(sql,"select id,stream,createdate ,updatedate from consortia\0");
	setQuerySql(sql);
	setNumColumn(4);
}
int crConsortiaQueryData::getID()
{
	return m_id;
}
CRCore::crStreamBuf *crConsortiaQueryData::getStream()
{
	return m_stream.get();
}
const std::string &crConsortiaQueryData::getCreateDate()
{
	return m_createdate;
}
/////////////////////////////////////////
//
//crChengchi2QueryData
//
/////////////////////////////////////////
crChengchi2QueryData::crChengchi2QueryData():
	m_id(0)//,
	//m_shili(0),
	//m_defend(0)
{
}
crChengchi2QueryData::crChengchi2QueryData(const crChengchi2QueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_stream(data.m_stream)
	//m_shili(data.m_shili),
	//m_defend(data.m_defend)
{
}
void crChengchi2QueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = (unsigned short)(atoi(buf));
		break;
	case 1:
		if(length>0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf,length);
			m_stream->seekBegin();
		}
		break;
	//case 1:
	//	m_shili = (unsigned char)(atoi(buf));
	//	break;
	//case 2:
	//	m_defend = (unsigned short)(atoi(buf));
	//	break;
	}
}
void crChengchi2QueryData::buildQuerySql()
{
	char sql[128];
	//sprintf(sql,"select id,shili,defend from chengchi2\0");
	sprintf(sql,"select id,stream from chengchi2\0");
	setQuerySql(sql);
	setNumColumn(2);
}
unsigned short crChengchi2QueryData::getID()
{
	return m_id;
}
CRCore::crStreamBuf *crChengchi2QueryData::getStream()
{
	return m_stream.get();
}
//unsigned char crChengchi2QueryData::getShili()
//{
//	return m_shili;
//}
//unsigned short crChengchi2QueryData::getDefend()
//{
//	return m_defend;
//}
/////////////////////////////////////////
//
//crShiliQueryData
//
/////////////////////////////////////////
crShiliQueryData::crShiliQueryData():
	m_id(0)
{
}
crShiliQueryData::crShiliQueryData(const crShiliQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_stream(data.m_stream)
{
}
void crShiliQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = (unsigned char)(atoi(buf));
		break;
	case 1:
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
void crShiliQueryData::buildQuerySql()
{
	char sql[128];
	sprintf(sql,"select id,stream from shili\0");
	setQuerySql(sql);
	setNumColumn(2);
}
unsigned char crShiliQueryData::getID()
{
	return m_id;
}
CRCore::crStreamBuf *crShiliQueryData::getStream()
{
	return m_stream.get();
}
/////////////////////////////////////////
//
//crInsertMail
//
/////////////////////////////////////////
void crInsertMail::buildUpdateSql(int playerid, int senderid,const std::string &sendername,const std::string &title,CRCore::crStreamBuf *body,int stats)
{
	int size = sendername.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,sendername.c_str(),size);

	char esctitle[100];
	memset(esctitle,0,100);
	crDataBaseManager::getInstance()->escapeString(esctitle,title.c_str(),title.size());

	int streamSize = 0;
	if(body) streamSize = body->getBufSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql, "insert into mail(playerid,senderid,sendername,title,stats,createdate) values(%d,%d,'%s','%s',%d,now())\0", playerid, senderid, escname, esctitle, stats);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"insert into mail(playerid,senderid,sendername,title,body,stats,createdate) values(%d,%d,'%s','%s','\0",playerid,senderid,escname,esctitle);
		m_sqlstream->write(sql,strlen(sql));
		m_sqlstream->write(body->getBuf(),body->getBufSize());
		sprintf(sql, "',%d,now())\0", stats);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crUpdateMail
//
/////////////////////////////////////////
void crUpdateMail::buildUpdateSql(int id,unsigned char stats)
{
	char sql[128];
	sprintf(sql,"update mail set stats=%d where id=%d\0",(int)stats,id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateMailTitle
//
/////////////////////////////////////////
void crUpdateMailTitle::buildUpdateSql(int id,const std::string &title)
{
	int size = title.size();
	char esctitle[50];
	memset(esctitle,0,50);
	crDataBaseManager::getInstance()->escapeString(esctitle,title.c_str(),size);

	char sql[128];
	sprintf(sql,"update mail set title='%s' where id=%d\0",esctitle,id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveMail
//
/////////////////////////////////////////
void crRemoveMail::buildUpdateSql(int id)
{
	char sql[64];
	sprintf(sql,"delete from mail where id=%d\0",id);
	setUpdateSql(sql);
}
void crRemoveMail::buildUpdateSql_Overdue(int overdue)
{
	char sql[128];
	sprintf(sql,"delete from mail where to_days(now())-to_days(createdate)>=%d\0",overdue);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertGiftsPack
//
/////////////////////////////////////////
void crInsertGiftsPack::buildUpdateSql(const std::string &key,CRCore::crStreamBuf *pack,int playerid)
{
	int size = key.size();
	char esckey[32];
	memset(esckey,0,32);
	crDataBaseManager::getInstance()->escapeString(esckey,key.c_str(),size);

	int streamSize = 0;
	if(pack) streamSize = pack->getBufSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"insert into giftspack(keyword,playerid,distill,createdate) values('%s',%d,0,now())\0",esckey,playerid);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"insert into giftspack(keyword,playerid,distill,pack,createdate) values('%s',%d,0,'\0",esckey,playerid);
		m_sqlstream->write(sql,strlen(sql));
		m_sqlstream->write(pack->getBuf(),pack->getBufSize());
		memcpy(sql,"',now())\0",9);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crUpdateGiftsPack
//
/////////////////////////////////////////
void crUpdateGiftsPack::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"update giftspack set distill=1 where id=%d\0",id);
	setUpdateSql(sql);
}
void crUpdateGiftsPack::buildUpdateSql(const std::string &keyword)
{
	char sql[128];
	int size = keyword.size();
	char esckey[32];
	memset(esckey,0,32);
	crDataBaseManager::getInstance()->escapeString(esckey,keyword.c_str(),size);
	sprintf(sql,"update giftspack set distill=1 where keyword='%s'\0",esckey);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveGiftsPack
//
/////////////////////////////////////////
void crRemoveGiftsPack::buildUpdateSql(int id)
{
	char sql[64];
	sprintf(sql,"delete from giftspack where id=%d\0",id);
	setUpdateSql(sql);
}
void crRemoveGiftsPack::buildUpdateSql(const std::string &keyword)
{
	char sql[128];
	int size = keyword.size();
	char esckey[32];
	memset(esckey,0,32);
	crDataBaseManager::getInstance()->escapeString(esckey,keyword.c_str(),size);
	sprintf(sql,"delete from giftspack where keyword='%s'\0",esckey);
	setUpdateSql(sql);
}
void crRemoveGiftsPack::buildUpdateSql_Overdue(int overdue)
{
	char sql[128];
	sprintf(sql,"delete from giftspack where to_days(now())-to_days(createdate)>=%d\0",overdue);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertConsortia
//
/////////////////////////////////////////
void crInsertConsortia::buildUpdateSql(CRCore::crStreamBuf *stream)
{
	int streamSize = 0;
	if(stream) streamSize = stream->getBufSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"insert into consortia(createdate) values(now())\0");
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"insert into consortia(stream,createdate) values('\0");
		m_sqlstream->write(sql,strlen(sql));
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getBufSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		//m_sqlstream->write(stream->getBuf(),stream->getBufSize());
		sprintf(sql,"',now())\0");
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crUpdateConsortia
//
/////////////////////////////////////////
void crUpdateConsortia::buildUpdateSql(int id,CRCore::crStreamBuf *stream)
{
	if(stream && stream->getRemainSize()>0)
	{
		int streamSize = stream->getRemainSize();

		m_sqlstream = new crStreamBuf;
		m_sqlstream->createBuf(streamSize+256);
		char sql[256];
		sprintf(sql,"update consortia set stream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"',updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crRemoveConsortia
//
/////////////////////////////////////////
void crRemoveConsortia::buildUpdateSql(int id)
{
	char sql[64];
	sprintf(sql,"delete from consortia where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateChengchi2
//
/////////////////////////////////////////
//void crUpdateChengchi2::buildUpdateSql(unsigned short id,unsigned char shili,unsigned short defend)
//{
//	char sql[128];
//	sprintf(sql,"update chengchi2 set shili=%d,defend=%d,updatedate=now() where id=%d\0",(int)shili,(int)defend,(int)id);
//	setUpdateSql(sql);
//}
void crUpdateChengchi2::buildUpdateSql(unsigned short id,CRCore::crStreamBuf *stream)
{
	if(stream && stream->getBufSize()>0)
	{
		int streamSize = stream->getBufSize();

		m_sqlstream = new crStreamBuf;
		m_sqlstream->createBuf(streamSize+256);
		char sql[256];
		sprintf(sql,"update chengchi2 set stream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getBuf(),stream->getBufSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"',updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crUpdateShili
//
/////////////////////////////////////////
void crUpdateShili::buildUpdateSql(unsigned char id,/*int king,const std::string &kingname,const std::string &affiche,*/CRCore::crStreamBuf *stream)
{
	//int size = kingname.size();
	//char *escname = new char[size+32];
	//memset(escname,0,size+32);
	//crDataBase::escapeString(escname,kingname.c_str(),size);

	//size = affiche.size();
	//char *escbuf = new char[size+32];
	//memset(escbuf,0,size+32);
	//crDataBase::escapeString(escbuf,affiche.c_str(),size);

	int streamSize = 0;
	if(stream) streamSize = stream->getRemainSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"update shili set stream=null,updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"update shili set stream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"', updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
	//delete [] escname;
	//delete [] escbuf;
}
/////////////////////////////////////////
//
//crQueryPlayerData
//
/////////////////////////////////////////
crQueryPlayerData::crQueryPlayerData():
	m_id(0)
{
}
crQueryPlayerData::crQueryPlayerData(const crQueryPlayerData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_accountname(data.m_accountname),
	m_logindate(data.m_logindate),
	m_createdate(data.m_createdate),
	m_onlinetime(data.m_onlinetime),
	m_email(data.m_email)
{
}
void crQueryPlayerData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_accountname = buf;
		break;
	case 2:
		m_logindate = buf;
		break;
	case 3:
		m_createdate = buf;
		break;
	case 4:
		m_onlinetime = buf;
		break;
	case 5:
		m_email = buf;
		break;
	}
}
void crQueryPlayerData::buildQuerySql()
{
	char sql[512];
	sprintf(sql,"select id,name,logindate,createdate,onlinetime,email from account\0");
	setQuerySql(sql);
	setNumColumn(6);
}
int crQueryPlayerData::getID()
{
	return m_id;
}
const std::string &crQueryPlayerData::getAccountName() const
{
	return m_accountname;
}
const std::string &crQueryPlayerData::getLoginDate() const
{
	return m_logindate;
}
const std::string &crQueryPlayerData::getCreateDate() const
{
	return m_createdate;
}
const std::string &crQueryPlayerData::getOnlinetime() const
{
	return m_onlinetime;
}
const std::string &crQueryPlayerData::getEmail() const
{
	return m_email;
}
/////////////////////////////////////////
//
//crPlayerGameStoreQueryData
//
/////////////////////////////////////////
crPlayerGameStoreQueryData::crPlayerGameStoreQueryData():
	m_id(0),
	m_cyclecount(0),
	m_money(0),
	m_totaladdmoney(0),
	m_vipexp(0)
{
	m_cardBagVec.resize(CycleCardCount);
	m_itemBagVec.resize(CycleItemCount);
}
crPlayerGameStoreQueryData::crPlayerGameStoreQueryData(const crPlayerGameStoreQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_cyclecount(data.m_cyclecount),
	m_money(data.m_money),
	m_totaladdmoney(data.m_totaladdmoney),
	m_vipexp(data.m_vipexp),
	m_cardBagVec(data.m_cardBagVec),
	m_itemBagVec(data.m_itemBagVec),
	m_super(data.m_super)
{
}
void crPlayerGameStoreQueryData::addParam(int i, char *buf, int length)
{
	ref_ptr<crStoreBagItemData> bag;
	std::vector<int> valueVec;

	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_cyclecount = (unsigned short)atoi(buf);
		break;
	case 2:
		m_money = atoi(buf);
		break;
	case 3:
		m_totaladdmoney = atoi(buf);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
		if (length > 0)//0,id,1,count,2,serverid,3,iscycle
		{
			valueVec.clear();
			crArgumentParser::appAtoVec(buf, valueVec);
			bag = new crStoreBagItemData;

			bag->setItemID(valueVec[0]);
			bag->setItemCount(valueVec[1]);
			bag->setItemServerID(valueVec[2]);
			bag->setItemHasCycle(valueVec[3]);
			//CRCore::notify(CRCore::ALWAYS) << "PlayerGameStoreQueryCard,i=" << i - 4 << " value=" << buf << std::endl;
			m_cardBagVec[i-4] = bag;
		}
		break;
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
	case 101:
	case 102:
	case 103:
		if (length > 0)//0,id,1,magicid,2,count,3,serverid,4,iscycle
		{
			valueVec.clear();
			crArgumentParser::appAtoVec(buf, valueVec);
			bag = new crStoreBagItemData;

			bag->setItemID(valueVec[0]);
			bag->setEquipMagic(valueVec[1]);
			bag->setItemCount(valueVec[2]);
			bag->setItemServerID(valueVec[3]);
			bag->setItemHasCycle(valueVec[4]);
			//CRCore::notify(CRCore::ALWAYS) << "PlayerGameStoreQueryItem,i="<<i- 64<<" value="<< buf << std::endl;
			m_itemBagVec[i - 64] = bag;
		}
		//else
		//{
		//	m_itemBagVec.push_back(NULL);
		//}
	case 104:
		m_vipexp = atoi(buf);
		break;
	case 105:
		m_super = buf;
		break;
	}
}
void crPlayerGameStoreQueryData::getCardAndItemStr(std::string &cardstr, std::string &itemstr)
{
	char buf[16];
	for (int i = 0; i < CycleCardCount; i++)
	{
		sprintf(buf, "card%d,\0", i);
		cardstr += buf;
	}
	for (int i = 0; i < CycleItemCount; i++)
	{
		sprintf(buf, "item%d,\0", i);
		itemstr += buf;
	}
}
void crPlayerGameStoreQueryData::buildQuerySql(int playerid,int gameid)
{
	char sql[1024];
	std::string cardstr, itemstr;
	getCardAndItemStr(cardstr, itemstr);
	sprintf(sql, "select id,cyclecount,money,totaladdmoney,%s%s vipexp,super from playergamestore where playerid=%d and gameid=%d\0", cardstr.c_str(), itemstr.c_str(), playerid, gameid);
	setQuerySql(sql);
	setNumColumn(106);//41
	//CRCore::notify(CRCore::ALWAYS) << "PlayerGameStoreQuery sql:" <<sql<< std::endl;
}
int crPlayerGameStoreQueryData::getID()
{
	return m_id;
}
unsigned short crPlayerGameStoreQueryData::getCyclecount()
{
	return m_cyclecount;
}
int crPlayerGameStoreQueryData::getMoney()
{
	return m_money;
}
int crPlayerGameStoreQueryData::getTotaladdmoney()
{
	return m_totaladdmoney;
}
int crPlayerGameStoreQueryData::getVipExp()
{
	return m_vipexp;
}
JXJStoreItemBagVec &crPlayerGameStoreQueryData::getCardBagVec()
{
	return m_cardBagVec;
}
JXJStoreItemBagVec &crPlayerGameStoreQueryData::getItemBagVec()
{
	return m_itemBagVec;
}
const std::string &crPlayerGameStoreQueryData::getSuper()
{
	return m_super;
}
//CRCore::crStreamBuf *crPlayerGameStoreQueryData::getStream()
//{
//	ItemMap itemMap;
//	rcfg::ConfigScript cfg_script(&itemMap);
//	std::vector<std::string> v_str;
//	char buf[128];
//	sprintf(buf, "%d,%d\0", WCHDATA_Money, m_money);
//	cfg_script.Add("Param", buf);
//	sprintf(buf, "%d,%d\0", WCHDATA_JXJPlayerTotalAddMoney, m_usedmoney);
//	cfg_script.Add("Param", buf);
//	
//	/////////背包列表
//	if (!m_itemBagVec.empty())
//	{
//		int count = m_itemBagVec.size();
//		cfg_script.Add("JXJItemStoreBagVec");
//		cfg_script.Push("JXJItemStoreBagVec");
//		v_str.resize(5);
//		for (int i = 0; i < count; i++)
//		{
//			if ((m_itemBagVec)[i].valid() && (m_itemBagVec)[i]->getItemCount()>0)
//			{
//				v_str[0] = crArgumentParser::appItoa((m_itemBagVec)[i]->getItemID());
//				v_str[1] = crArgumentParser::appItoa((m_itemBagVec)[i]->getItemCount());
//				v_str[2] = crArgumentParser::appItoa((m_itemBagVec)[i]->getEquipMagic());
//				v_str[3] = crArgumentParser::appItoa((m_itemBagVec)[i]->getItemServerID());
//				v_str[4] = crArgumentParser::appItoa((m_itemBagVec)[i]->getItemHasCycle());
//			}
//			else
//			{
//				v_str[0] = "";
//				v_str[1] = "";
//				v_str[2] = "";
//				v_str[3] = "";
//				v_str[4] = "";
//			}
//			cfg_script.Add("Param", v_str);
//		}
//		cfg_script.Pop();
//	}
//	///////////将卡背包列表
//	if (!m_cradBagVec.empty())
//	{
//		int bagcount = m_cradBagVec.size();
//		cfg_script.Add("JXJJiangkaItemBagVec");
//		cfg_script.Push("JXJJiangkaItemBagVec");
//		v_str.resize(4);
//		for (int i = 0; i<bagcount; i++)
//		{
//			if (m_cradBagVec[i].valid() && m_cradBagVec[i]->getItemCount()>0)
//			{
//				v_str[0] = crArgumentParser::appItoa(m_cradBagVec[i]->getItemID());
//				v_str[1] = crArgumentParser::appItoa(m_cradBagVec[i]->getItemCount());
//				v_str[2] = crArgumentParser::appItoa((m_cradBagVec)[i]->getItemServerID());
//				v_str[3] = crArgumentParser::appItoa((m_cradBagVec)[i]->getItemHasCycle());
//			}
//			else
//			{
//				v_str[0] = "";
//				v_str[1] = "";
//				v_str[2] = "";
//				v_str[3] = "";
//			}
//			cfg_script.Add("Param", v_str);
//		}
//		cfg_script.Pop();
//	}
//	///////////////////
//	{
//		sprintf(buf, "%s%d%d.tmp\0", _name(), GetCurrentThreadId(), time(0));
//		cfg_script.Write(buf, "");
//		cfg_script.Close();
//
//		if (!m_stream.valid())
//			m_stream = new crStreamBuf;
//		m_stream->loadFromFile2(buf);
//		m_stream->seekBegin();
//
//		DeleteFileA(buf);
//	}
//
//	return m_stream.get();
//}
/////////////////////////////////////////
//
//crPlayerGameStoreQuery2Data
//
/////////////////////////////////////////
crPlayerGameStoreQuery2Data::crPlayerGameStoreQuery2Data():
	m_id(0)
{
}
crPlayerGameStoreQuery2Data::crPlayerGameStoreQuery2Data(const crPlayerGameStoreQuery2Data& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crPlayerGameStoreQuery2Data::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crPlayerGameStoreQuery2Data::buildQuerySql(int playerid,int gameid)
{
	char sql[128];
	sprintf(sql,"select id from playergamestore where playerid=%d and gameid=%d\0",playerid,gameid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crPlayerGameStoreQuery2Data::getID()
{
	return m_id;
}

/////////////////////////////////////////
//
//crPlayerGameStoreCycleCountQueryData
//
/////////////////////////////////////////
crPlayerGameStoreCycleCountQueryData::crPlayerGameStoreCycleCountQueryData() :
m_cycleCount(0)
{
}
crPlayerGameStoreCycleCountQueryData::crPlayerGameStoreCycleCountQueryData(const crPlayerGameStoreCycleCountQueryData& data) :
crQueryData(data),
m_cycleCount(data.m_cycleCount)
{
}
void crPlayerGameStoreCycleCountQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_cycleCount = atoi(buf);
		break;
	}
}
void crPlayerGameStoreCycleCountQueryData::buildQuerySql(int playerid, int gameid)
{
	char sql[128];
	sprintf(sql, "select cyclecount from playergamestore where playerid=%d and gameid=%d\0", playerid, gameid);
	setQuerySql(sql);
	setNumColumn(1);
}
short crPlayerGameStoreCycleCountQueryData::getCycleCount()
{
	return m_cycleCount;
}
/////////////////////////////////////////
//
//crUpdatePlayerGameStoreCycleCount
//
/////////////////////////////////////////
// void crUpdatePlayerGameStoreCycleCount::buildUpdateSql(int playerid,int gameid, int cyclecount)
// {
// 	m_sqlstream = new crStreamBuf;
// 	m_sqlstream->createBuf(256);
// 	char sql[256];
// 	sprintf(sql, "UPDATE PlayerGameStore SET cyclecount=%d where playerid=%d and gameid=%d\0", cyclecount, playerid, gameid);
// 	m_sqlstream->write(sql, strlen(sql));
// }
/////////////////////////////////////////
//
//crUpdatePlayerGameStore
//
/////////////////////////////////////////
void crUpdatePlayerGameStore::buildUpdateSql(int id,int serverid,JXJStoreItemBagVec & cardVec,JXJStoreItemBagVec & itemVec)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(4096);
	char sql[256];
	sprintf(sql, "update playergamestore set \0");
	m_sqlstream->write(sql, strlen(sql));

	std::vector<int> tempVec;
	std::string strValue;
	std::string strCluName;
	int itemcount = cardVec.size();
	for (int i = 0; i < CycleCardCount; ++i)
	{
		strValue.clear();
		if(i<itemcount)
		{
			if(!cardVec[i].valid() || cardVec[i]->getItemServerID()!=serverid)
				continue;
			tempVec.clear();

			tempVec.push_back(cardVec[i]->getItemID());
			tempVec.push_back(cardVec[i]->getItemCount());
			tempVec.push_back(cardVec[i]->getItemServerID());
			tempVec.push_back(cardVec[i]->getItemHasCycle());
			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		strCluName = std::string("card") + crArgumentParser::appItoa(i);
		sprintf(sql, "%s='%s',\0",strCluName.c_str(),strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	itemcount = itemVec.size();
	for (int i = 0; i < CycleItemCount; ++i)
	{
		strValue.clear();
		if(i<itemcount)
		{
			if(!itemVec[i].valid() || itemVec[i]->getItemServerID()!=serverid)
				continue;
			tempVec.clear();

			tempVec.push_back(itemVec[i]->getItemID());
			tempVec.push_back(itemVec[i]->getEquipMagic());
			tempVec.push_back(itemVec[i]->getItemCount());
			tempVec.push_back(itemVec[i]->getItemServerID());
			tempVec.push_back(itemVec[i]->getItemHasCycle());
			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		strCluName = std::string("item") + crArgumentParser::appItoa(i);
		sprintf(sql, "%s='%s',\0", strCluName.c_str(), strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	sprintf(sql, "updatedate=now() where id=%d\0", id);
	m_sqlstream->write(sql, strlen(sql));
	//CRCore::notify(CRCore::ALWAYS) << "UpdatePlayerGameStore = " << m_sqlstream->getBuf() << std::endl;
// 	int streamSize = 0;
// 	if(stream) streamSize = stream->getRemainSize();
// 	m_sqlstream = new crStreamBuf;
// 	m_sqlstream->createBuf(streamSize+256);
// 	char sql[256];
// 	if(streamSize == 0)
// 	{
// 		sprintf(sql,"UPDATE PlayerGameStore SET stream=NULL,updatedate=NOW() where id=%d\0",id);
// 		m_sqlstream->write(sql,strlen(sql));
// 	}
// 	else
// 	{
// 		
// 
// 		char* escstream = new char[streamSize*2+1];
// 		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
// 		m_sqlstream->write(escstream,len);
// 		delete [] escstream;
// 		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
// 		
// 	}
}
void crUpdatePlayerGameStore::buildUpdateSql_Cycle(int id, int serverid, int cycleCountAdd, int money, int totaladdmoney, int vipexp, JXJStoreItemBagVec &cardVec, JXJStoreItemBagVec &itemVec,const std::string &super)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(4096);
	char sql[512];
	sprintf(sql, "update playergamestore set cyclecount=cyclecount+%d,money=money+%d,totaladdmoney=totaladdmoney+%d,vipexp=vipexp+%d,super='%s',\0", cycleCountAdd, money, totaladdmoney, vipexp, super.c_str());
	m_sqlstream->write(sql, strlen(sql));

	std::vector<int> tempVec;
	std::string strValue;
	std::string strCluName;
	int itemcount = cardVec.size();
	for (int i = 0; i < CycleCardCount; ++i)
	{
		strValue.clear();
		if(i<itemcount)
		{
			if(!cardVec[i].valid() || cardVec[i]->getItemServerID()!=serverid)
				continue;
			tempVec.clear();

			tempVec.push_back(cardVec[i]->getItemID());
			tempVec.push_back(cardVec[i]->getItemCount());
			tempVec.push_back(cardVec[i]->getItemServerID());
			tempVec.push_back(cardVec[i]->getItemHasCycle());
			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		strCluName = std::string("card") + crArgumentParser::appItoa(i);
		sprintf(sql, "%s='%s',\0",strCluName.c_str(),strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	itemcount = itemVec.size();
	for (int i = 0; i < CycleItemCount; ++i)
	{
		strValue.clear();
		if(i<itemcount)
		{
			if(!itemVec[i].valid() || itemVec[i]->getItemServerID()!=serverid)
				continue;
			tempVec.clear();

			tempVec.push_back(itemVec[i]->getItemID());
			tempVec.push_back(itemVec[i]->getEquipMagic());
			tempVec.push_back(itemVec[i]->getItemCount());
			tempVec.push_back(itemVec[i]->getItemServerID());
			tempVec.push_back(itemVec[i]->getItemHasCycle());
			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		strCluName = std::string("item") + crArgumentParser::appItoa(i);
		sprintf(sql, "%s='%s',\0", strCluName.c_str(), strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	sprintf(sql, "updatedate=now() where id=%d\0", id);
	m_sqlstream->write(sql, strlen(sql));
 //	char sql[256];
 //	sprintf(sql, "UPDATE PlayerGameStore SET cyclecount=cyclecount+%d,money=money+%d,totaladdmoney=totaladdmoney+%d,updatedate=NOW() WHERE id=%d\0", cycleCountAdd, money, totaladdmoney,id);
	//setUpdateSql(sql);
}
void crUpdatePlayerGameStore::buildUpdateSql_Distill(int id, int distillmoney, int distilltotalmoney, int distillvipexp,std::vector<int>&cardVec, std::vector<int>&itemVec)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(4096);
	char sql[512];
	sprintf(sql, "update playergamestore set money=money-%d,totaladdmoney=totaladdmoney-%d,vipexp=vipexp-%d,\0", distillmoney, distilltotalmoney, distillvipexp);
	m_sqlstream->write(sql, strlen(sql));

	std::string strCluName;
	int itemcount = cardVec.size();
	for (int i = 0; i < itemcount; ++i)
	{
		strCluName = std::string("card") + crArgumentParser::appItoa(cardVec[i]);
		sprintf(sql, "%s='',\0",strCluName.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	itemcount = itemVec.size();
	for (int i = 0; i < itemcount; ++i)
	{
		strCluName = std::string("item") + crArgumentParser::appItoa(itemVec[i]);
		sprintf(sql, "%s='',\0", strCluName.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}
	sprintf(sql, "updatedate=now() where id=%d\0", id);
	m_sqlstream->write(sql, strlen(sql));
	//char sql[128];
	//sprintf(sql, "UPDATE PlayerGameStore SET money=money-%d,totaladdmoney=totaladdmoney-%d,updatedate=NOW() where id=%d\0",id,distillmoney,distilltotalmoney);
	//setUpdateSql(sql);
}
void crUpdatePlayerGameStore::buildUpdateSql_Distill(int id, const std::string &super)
{
	char sql[512];
	sprintf(sql, "update playergamestore set super='%s',updatedate=now() where id=%d\0", super.c_str(), id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerGameStore
//
/////////////////////////////////////////
void crInsertPlayerGameStore::buildUpdateSql(int playerid, int gameid,JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(2048);
	char sql[1024];
	std::string cardstr, itemstr;
	crPlayerGameStoreQueryData::getCardAndItemStr(cardstr, itemstr);
	sprintf(sql, "insert into playergamestore(playerid,gameid,%s%s createdate) values(%d,%d,\0", cardstr.c_str(), itemstr.c_str(),playerid,gameid);
	m_sqlstream->write(sql, strlen(sql));

	std::vector<int> tempVec;
	std::string strValue;

	int size = cardVec.size();
	for (int index = 0; index < CycleCardCount; ++index)
	{
		strValue.clear();
		if (index < size)
		{
			if(!cardVec[index].valid())
				continue;
			tempVec.clear();
			tempVec.push_back(cardVec[index]->getItemID());
			tempVec.push_back(cardVec[index]->getItemCount());
			tempVec.push_back(cardVec[index]->getItemServerID());
			tempVec.push_back(cardVec[index]->getItemHasCycle());

			strValue = crArgumentParser::appVectoa(tempVec, '|');			
		}
		sprintf(sql, "'%s',\0", strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}

	size = itemVec.size();
	for (int index = 0; index < CycleItemCount; ++index)
	{
		strValue.clear();
		if (index < size)
		{
			if(!itemVec[index].valid())
				continue;
			tempVec.clear();
			tempVec.push_back(itemVec[index]->getItemID());
			tempVec.push_back(itemVec[index]->getEquipMagic());
			tempVec.push_back(itemVec[index]->getItemCount());
			tempVec.push_back(itemVec[index]->getItemServerID());
			tempVec.push_back(itemVec[index]->getItemHasCycle());

			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		sprintf(sql, "'%s',\0", strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}

	sprintf(sql, "now())\0");
	m_sqlstream->write(sql, strlen(sql));
}
void crInsertPlayerGameStore::buildUpdateSql_Cycle(int playerid, int gameid, int cycleCountAdd, int money, int totaladdmoney, int vipexp, const std::string &super)
{
	char sql[512];
	sprintf(sql, "insert into playergamestore(playerid,gameid,cyclecount,money,totaladdmoney,vipexp,super,createdate) values(%d,%d,%d,%d,%d,%d,'%s',now())\0", playerid, gameid, cycleCountAdd, money, totaladdmoney, vipexp,super.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertGameStoreDistillRecord
//
/////////////////////////////////////////
void crInsertGameStoreDistillRecord::buildUpdateSql(int playerid, int gameid, int cyclecount, int money, int totaladdmoney, int vipexp, JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec, const std::string &super)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(4096);
	char sql[1024];
	std::string cardstr, itemstr;
	crPlayerGameStoreQueryData::getCardAndItemStr(cardstr, itemstr);
	sprintf(sql, "insert into gamestoredistillrecord(playerid,gameid,cyclecount,money,totaladdmoney,vipexp,super,%s%s createdate) values(%d,%d,%d,%d,%d,%d,'%s',\0", cardstr.c_str(), itemstr.c_str(), playerid, gameid, cyclecount, money, totaladdmoney, vipexp, super.c_str());
	m_sqlstream->write(sql, strlen(sql));

	std::vector<int> tempVec;
	std::string strValue;

	int size = cardVec.size();
	for (int index = 0; index < CycleCardCount; ++index)
	{
		strValue.clear();
		if (index < size && cardVec[index].valid())
		{
			tempVec.clear();
			tempVec.push_back(cardVec[index]->getItemID());
			tempVec.push_back(cardVec[index]->getItemCount());
			tempVec.push_back(cardVec[index]->getItemServerID());
			tempVec.push_back(cardVec[index]->getItemHasCycle());

			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		sprintf(sql, "'%s',\0", strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}

	size = itemVec.size();
	for (int index = 0; index < CycleItemCount; ++index)
	{
		strValue.clear();
		if (index < size && itemVec[index].valid())
		{
			tempVec.clear();
			tempVec.push_back(itemVec[index]->getItemID());
			tempVec.push_back(itemVec[index]->getEquipMagic());
			tempVec.push_back(itemVec[index]->getItemCount());
			tempVec.push_back(itemVec[index]->getItemServerID());
			tempVec.push_back(itemVec[index]->getItemHasCycle());

			strValue = crArgumentParser::appVectoa(tempVec, '|');
		}
		sprintf(sql, "'%s',\0", strValue.c_str());
		m_sqlstream->write(sql, strlen(sql));
	}

	sprintf(sql, "NOW())\0");
	m_sqlstream->write(sql, strlen(sql));
}
/////////////////////////////////////////
//
//crPeopleCycledQueryData
//
/////////////////////////////////////////
crPeopleCycledQueryData::crPeopleCycledQueryData() :
	m_cycled(0)
{
}
crPeopleCycledQueryData::crPeopleCycledQueryData(const crPeopleCycledQueryData& data) :
	crQueryData(data),
	m_cycled(data.m_cycled)
{
}
void crPeopleCycledQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_cycled = atoi(buf);
		break;
	}
}
void crPeopleCycledQueryData::buildQuerySql(int playerid)
{
	char sql[128];
	sprintf(sql, "select cycled from peoplemap where playerid=%d\0", playerid);
	setQuerySql(sql);
	setNumColumn(1);
}
char crPeopleCycledQueryData::getCycled()
{
	return m_cycled;
}
/////////////////////////////////////////
//
//crUpdatePeopleCycled
//
/////////////////////////////////////////
void crUpdatePeopleCycled::buildUpdateSql(int playerid)
{
	char sql[256];
	sprintf(sql,"update peoplemap set cycled=1,updatedate=now() where playerid=%d\0",playerid);
	setUpdateSql(sql);
}
////////////////////////////////////////
//
//crPlayerTotalAddMoneyQueryData
//
/////////////////////////////////////////
crPlayerTotalAddMoneyQueryData::crPlayerTotalAddMoneyQueryData():
	m_money(0)
{
}
crPlayerTotalAddMoneyQueryData::crPlayerTotalAddMoneyQueryData(const crPlayerTotalAddMoneyQueryData& data):
	crQueryData(data),
	m_money(data.m_money)
{
}
void crPlayerTotalAddMoneyQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_money = atoi(buf);
		break;
	}
}
void crPlayerTotalAddMoneyQueryData::buildQuerySql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"select sum(money) from moneyrecord where playerid=%d and serverid=%d and (str='首次充值' or str='充值')\0",playerid,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crPlayerTotalAddMoneyQueryData::getMoney()
{
	return m_money;
}
/////////////////////////////////////////
//
//crUpdatePlayerStore
//
/////////////////////////////////////////
// void crUpdatePlayerStore::buildUpdateSql(int id,CRCore::crStreamBuf *stream)
// {
// 	int streamSize = 0;
// 	if(stream) streamSize = stream->getRemainSize();
// 	m_sqlstream = new crStreamBuf;
// 	m_sqlstream->createBuf(streamSize+256);
// 	char sql[256];
// 	if(streamSize == 0)
// 	{
// 		sprintf(sql,"UPDATE PlayerStore SET stream=NULL,updatedate=NOW() where id=%d\0",id);
// 		m_sqlstream->write(sql,strlen(sql));
// 	}
// 	else
// 	{
// 		sprintf(sql,"UPDATE PlayerStore SET stream='\0");
// 		m_sqlstream->write(sql,strlen(sql));
// 		m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
// 		sprintf(sql,"', updatedate=NOW() where id=%d\0",id);
// 		m_sqlstream->write(sql,strlen(sql));
// 	}
// }
/////////////////////////////////////////
//
//crInsertPlayerStore
//
/////////////////////////////////////////
// void crInsertPlayerStore::buildUpdateSql(int id,CRCore::crStreamBuf *stream)
// {
// 	int streamSize = 0;
// 	if(stream) streamSize = stream->getBufSize();
// 	m_sqlstream = new crStreamBuf;
// 	m_sqlstream->createBuf(streamSize+256);
// 	char sql[256];
// 	if(streamSize == 0)
// 	{
// 		sprintf(sql,"INSERT INTO PlayerStore(playerid,createdate) VALUES(%d,NOW())\0",id);
// 		m_sqlstream->write(sql,strlen(sql));
// 	}
// 	else
// 	{
// 		sprintf(sql,"INSERT INTO PlayerStore(stream,createdate,playerid) VALUES('\0");
// 		m_sqlstream->write(sql,strlen(sql));
// 		m_sqlstream->write(stream->getBuf(),stream->getBufSize());
// 		sprintf(sql,"',NOW(),%d)\0",id);
// 		m_sqlstream->write(sql,strlen(sql));
// 	}
// }
/////////////////////////////////////////
//
//crRemoveConsortia
//
/////////////////////////////////////////
// void crRemovePlayerStore::buildUpdateSql(int id)
// {
// 	char sql[64];
// 	sprintf(sql,"DELETE from PlayerStore where playerid=%d\0",id);
// 	setUpdateSql(sql);
// }

/////////////////////////////////////////
//
//crPlayerStoreQueryData
//
/////////////////////////////////////////
// crPlayerStoreQueryData::crPlayerStoreQueryData():
// 	m_id(0),
// 	m_playerid(0)
// {
// }
// crPlayerStoreQueryData::crPlayerStoreQueryData(const crPlayerStoreQueryData& data):
// 	crQueryData(data),
// 	m_id(data.m_id),
// 	m_playerid(data.m_playerid)
// {
// }
// void crPlayerStoreQueryData::addParam(int i, char *buf, int length)
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		m_id = atoi(buf);
// 		break;
// 	case 1:
// 		m_playerid = atoi(buf);
// 		break;
// 	case 2:
// 		if(length>0)
// 		{
// 			m_stream = new crStreamBuf;
// 			m_stream->createBuf(length);
// 			m_stream->write(buf,length);
// 			m_stream->seekBegin();
// 		}
// 		break;
// 	}
// }
// void crPlayerStoreQueryData::buildQuerySql()
// {
// 	char sql[128];
// 	sprintf(sql,"select id,playerid,stream from playerstore\0");
// 	setQuerySql(sql);
// 	setNumColumn(3);
// }
// void crPlayerStoreQueryData::buildQuerySql(int playerid)
// {
// 	char sql[128];
// 	sprintf(sql,"select id,stream from playerstore where playerid=%d\0",playerid);
// 	setQuerySql(sql);
// 	setNumColumn(2);
// }
// unsigned char crPlayerStoreQueryData::getID()
// {
// 	return m_id;
// }
// int crPlayerStoreQueryData::getPlayerid()
// {
// 	return m_playerid;
// }
// CRCore::crStreamBuf * crPlayerStoreQueryData::getStream()
// {
// 	return m_stream.get();
// }

/////////////////////////////////////////
//
//crPlayerStoreQuery2Data
//
/////////////////////////////////////////
// crPlayerStoreQuery2Data::crPlayerStoreQuery2Data():
// 	m_id(0)
// {
// }
// crPlayerStoreQuery2Data::crPlayerStoreQuery2Data(const crPlayerStoreQuery2Data& data):
// 	crQueryData(data),
// 	m_id(data.m_id)
// {
// }
// void crPlayerStoreQuery2Data::addParam(int i, char *buf, int length)
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		m_id = atoi(buf);
// 		break;
// 	}
// }
// void crPlayerStoreQuery2Data::buildQuerySql(int playerid)
// {
// 	char sql[128];
// 	sprintf(sql,"select id from playerstore where playerid=%d\0",playerid);
// 	setQuerySql(sql);
// 	setNumColumn(1);
// }
// unsigned char crPlayerStoreQuery2Data::getID()
// {
// 	return m_id;
// }

///////////////////////////////////////////
////
////crQueryOnePlayerData
////
///////////////////////////////////////////
//crQueryOnePlayerData::crQueryOnePlayerData():
//	m_mainroleid(0)
//{
//}
//crQueryOnePlayerData::crQueryOnePlayerData(const crQueryOnePlayerData& data):
//	crQueryData(data),
//	m_mainroleid(data.m_mainroleid),
//	m_stream(data.m_stream)
//{
//}
//void crQueryOnePlayerData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	case 0:
//		m_mainroleid = atoi(buf);
//		break;
//	case 1:
//		if(length>0)
//		{
//			m_stream = new crStreamBuf;
//			m_stream->createBuf(length);
//			m_stream->write(buf,length);
//			m_stream->seekBegin();
//		}
//		break;
//	}
//}
//void crQueryOnePlayerData::buildQuerySql(int playerid)
//{
//	char sql[512];
//	sprintf(sql,"select mainroleid,stream from playergamedata where id=%d\0",playerid);
//	setQuerySql(sql);
//	setNumColumn(4);
//}
//int crQueryOnePlayerData::getID()
//{
//	return m_mainroleid;
//}
//CRCore::crStreamBuf *crQueryOnePlayerData::getBody()
//{
//	return m_stream.get();
//}
///////////////////////////////////////////
////
////crQueryOnePlayerMainRoleData
////
///////////////////////////////////////////
//crQueryOnePlayerMainRoleData::crQueryOnePlayerMainRoleData():
//	m_stream(NULL)
//{
//}
//crQueryOnePlayerMainRoleData::crQueryOnePlayerMainRoleData(const crQueryOnePlayerMainRoleData& data):
//	crQueryData(data),
//	m_charactername(data.m_charactername),
//	m_stream(data.m_stream)
//{
//}
//void crQueryOnePlayerMainRoleData::addParam(int i, char *buf, int length)
//{
//	switch(i) 
//	{
//	case 0:
//		m_charactername = buf;
//		break;
//	case 1:
//		if(length>0)
//		{
//			m_stream = new crStreamBuf;
//			m_stream->createBuf(length);
//			m_stream->write(buf,length);
//			m_stream->seekBegin();
//		}
//		break;
//	}
//}
//void crQueryOnePlayerMainRoleData::buildQuerySql(int mainroleid)
//{
//	char sql[512];
//	sprintf(sql,"select iname,stream,createdate from instanceitem where id=%d\0",mainroleid);
//	setQuerySql(sql);
//	setNumColumn(4);
//}
//const std::string &crQueryOnePlayerMainRoleData::getCharacterName() const
//{
//	return m_charactername;
//}
//CRCore::crStreamBuf *crQueryOnePlayerMainRoleData::getBody()
//{
//	return m_stream.get();
//}
/////////////////////////////////////////
//
//crCheckRemainPlayerData
//
/////////////////////////////////////////
crCheckRemainPlayerData::crCheckRemainPlayerData():
	m_id(0)
{
}
crCheckRemainPlayerData::crCheckRemainPlayerData(const crCheckRemainPlayerData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crCheckRemainPlayerData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crCheckRemainPlayerData::buildQuerySql(std::string& selcolname,std::string& tabname,std::string& colname,int overdue)
{
	char sql[128];
	sprintf(sql,"select %s from %s where to_days(now())-to_days(%s)<=%d\0",selcolname.c_str(),tabname.c_str(),colname.c_str(),overdue);
	//sprintf(sql,"select id from account where to_days(now())-to_days(logoutdate)>%d or logoutdate is NULL\0",overdue);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCheckRemainPlayerData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crCheckLostPlayerData
//
/////////////////////////////////////////
crCheckLostPlayerData::crCheckLostPlayerData():
	m_id(0)
{
}
crCheckLostPlayerData::crCheckLostPlayerData(const crCheckLostPlayerData& data):
	crQueryData(data),
	m_id(data.m_id)
{
}
void crCheckLostPlayerData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	}
}
void crCheckLostPlayerData::buildQuerySql(int overdue)
{
	char sql[128];
	//sprintf(sql,"select id from account where to_days(now())-to_days(logoutdate)<=%d\0",overdue);
	sprintf(sql,"select id from account where to_days(now())-to_days(logoutdate)>%d or logoutdate is null\0",overdue);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCheckLostPlayerData::getID()
{
	return m_id;
}
/////////////////////////////////////////
//
//crCheckTableCountData
//
/////////////////////////////////////////
crCheckTableCountData::crCheckTableCountData():
	m_count(0)
{
}
crCheckTableCountData::crCheckTableCountData(const crCheckTableCountData& data):
	crQueryData(data),
	m_count(data.m_count)
{
}
void crCheckTableCountData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crCheckTableCountData::buildQuerySql(std::string& tabname)
{
	char sql[128];
	sprintf(sql,"select count(*) from %s\0",tabname.c_str());
	setQuerySql(sql);
	setNumColumn(1);
}
void crCheckTableCountData::buildQuerySql(std::string& tabname,std::string& colname,int overdue)
{
	char sql[128];
	sprintf(sql,"select count(*) from %s where to_days(now())-to_days(%s)=%d\0",tabname,colname,overdue);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCheckTableCountData::getTabCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crCheckMoneyRecordData
//
/////////////////////////////////////////
crCheckMoneyRecordData::crCheckMoneyRecordData():
	m_playerid(0),
	m_moneycount(0)
{
}
crCheckMoneyRecordData::crCheckMoneyRecordData(const crCheckMoneyRecordData& data):
	crQueryData(data),
	m_playerid(data.m_playerid),
	m_str(data.m_str),
	m_createdate(data.m_createdate),
	m_moneycount(data.m_moneycount)
{
}
void crCheckMoneyRecordData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	case 1:
		m_moneycount = atoi(buf);
		break;
	case 2:
		m_str = buf;
		break;
	case 3:
		m_createdate = buf;
		break;
	}
}
void crCheckMoneyRecordData::buildQuerySql()
{
	char sql[128];
	sprintf(sql,"select playerid,money,str,createdate from moneyrecord\0");
	setQuerySql(sql);
	setNumColumn(4);
}
int crCheckMoneyRecordData::getPlayerID()
{
	return m_playerid;
}
int crCheckMoneyRecordData::getMoneyCount()
{
	return m_moneycount;
}
std::string &crCheckMoneyRecordData::getStr()
{
	return m_str;
}
std::string &crCheckMoneyRecordData::getCreatedate()
{
	return m_createdate;
}


const std::string & JXJ::crConsortiaQueryData::getUpdateDate()
{
	return m_updateDate;
}

/////////////////////////////////////////
//
//crJXJLoginQueryData
//
/////////////////////////////////////////
crJXJLoginQueryData::crJXJLoginQueryData():
	m_id(0),
	m_lastserverid(0),
	m_locked(0)
{
}
crJXJLoginQueryData::crJXJLoginQueryData(const crJXJLoginQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_lastserverid(data.m_lastserverid),
	m_locked(data.m_locked)
{
}


void crJXJLoginQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_lastserverid = atoi(buf);
		break;
	case 2:
		m_locked = (unsigned char)(atoi(buf));
		break;
	}
}

void crJXJLoginQueryData::buildQuerySql(const std::string& name,const std::string& pwd)
{
	char sql[128];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	sprintf(sql,"select id,lastserverid,locked from account where name='%s' and pwd=password('%s')\0",escname,pwd.c_str());
	setQuerySql(sql);
	setNumColumn(3);
}

int crJXJLoginQueryData::getPlayerID()
{
	return m_id;
}
unsigned char crJXJLoginQueryData::getLocked()
{
	return m_locked;
}
int crJXJLoginQueryData::getLastServerID()
{
	return m_lastserverid;
}
/////////////////////////////////////////
//
//crJXJLoginKeyQueryData
//
/////////////////////////////////////////
crJXJLoginKeyQueryData::crJXJLoginKeyQueryData()
{
}
crJXJLoginKeyQueryData::crJXJLoginKeyQueryData(const crJXJLoginKeyQueryData& data):
	crQueryData(data),
	m_key(data.m_key),
	m_username(data.m_username),
	m_advert_id(data.m_advert_id)
{
}
void crJXJLoginKeyQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_key = buf;
		break;
	case 1:
		m_username = buf;
		break;
	case 2:
		m_advert_id = buf;
		break;
	}
}
void crJXJLoginKeyQueryData::buildQuerySql(int playerid)
{
	char sql[256];
	sprintf(sql,"select loginkey,name,advert_id from account where id=%d and timediff(now(),logindate)<='00:10:00'\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}
const std::string& crJXJLoginKeyQueryData::getKey() const
{
	return m_key;
}
const std::string& crJXJLoginKeyQueryData::getUserName() const
{
	return m_username;
}
const std::string &crJXJLoginKeyQueryData::getAdvertid() const
{
	return m_advert_id;
}
/////////////////////////////////////////
//
//crJXJUpdateLoginKeyDate
//
/////////////////////////////////////////
void crJXJUpdateLoginKeyDate::buildUpdateSql(int playerid,const std::string &key,const std::string &ip)
{
	char sql[256];
	int size = key.size();
	char esckey[128];
	memset(esckey,0,128);
	crDataBaseManager::getInstance()->escapeString(esckey,key.c_str(),size);
	sprintf(sql,"update account set loginkey='%s',ip='%s',logindate=now() where id=%d\0",esckey,ip.c_str(),playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdateLoginServerID
//
/////////////////////////////////////////
void crJXJUpdateLoginServerID::buildUpdateSql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"update account set lastserverid=%d where id=%d\0",serverid,playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJInsertAccount
//
/////////////////////////////////////////
void crJXJInsertAccount::buildUpdateSql(const std::string &name, const std::string &password, unsigned short chatdbid, const std::string &email, const std::string &nickname, const std::string &relname, char sex)
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
//crJXJRemoveAccount
//
/////////////////////////////////////////
void crJXJRemoveAccount::buildUpdateSql(const std::string &name)
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
//crJXJUpdatePassword
//
/////////////////////////////////////////
void crJXJUpdatePassword::buildUpdateSql(int playerid, const std::string &password)
{
	char sql[256];
	sprintf(sql,"update account set pwd=password('%s'),updatedate=now() where id=%d\0",password.c_str(),playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdateOnlineTime
//
/////////////////////////////////////////
void crJXJUpdateOnlineTime::buildUpdateSql(int playerid)
{
	char sql[128];
	//sprintf(sql,"UPDATE Account SET onlinetime=addtime(onlinetime,timediff(NOW(),logindate)),logoutdate=NOW() where id=%d\0",playerid);
	sprintf(sql,"update account set onlinetime=onlinetime+timestampdiff(second,logindate,now()),logindate=now(),logoutdate=now() where id=%d\0",playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJLockAccount
//
/////////////////////////////////////////
void crJXJLockAccount::buildUpdateSql(int playerid,unsigned char locked)
{
	char sql[128];
	sprintf(sql,"update account set locked=%d,logoutdate=now() where id=%d\0",playerid,locked);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJActivecodeQueryData
//
/////////////////////////////////////////
crJXJActivecodeQueryData::crJXJActivecodeQueryData():
	m_id(0),
	m_used(false)
{
}
crJXJActivecodeQueryData::crJXJActivecodeQueryData(const crJXJActivecodeQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_used(data.m_used),
	m_validdate(data.m_validdate)
{
}
void crJXJActivecodeQueryData::addParam(int i, char *buf, int length)
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
void crJXJActivecodeQueryData::buildQuerySql(const std::string &code)
{
	char sql[128];
	sprintf(sql,"select id,used,validdate from activecode where code='%s'\0",code.c_str());
	setQuerySql(sql);
	setNumColumn(3);
}
int crJXJActivecodeQueryData::getID()
{
	return m_id;
}
bool crJXJActivecodeQueryData::getUsed()
{
	return m_used;
}
const std::string &crJXJActivecodeQueryData::getValidDate()
{
	return m_validdate;
}
/////////////////////////////////////////
//
//crJXJUpdateActiveCode
//
/////////////////////////////////////////
void crJXJUpdateActiveCode::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"update activecode set used=1,updatedate=now() where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJInsertActiveCode
//
/////////////////////////////////////////
void crJXJInsertActiveCode::buildUpdateSql(const std::string &code)
{
	char sql[256];
	sprintf(sql,"insert into activecode(code,used) values('%s',0)\0",code.c_str());
	setUpdateSql(sql);
}
void crJXJInsertActiveCode::buildUpdateSql(const std::string &code,const std::string &validdate)
{
	if(validdate.empty())
	{
		buildUpdateSql(code);
	}
	else
	{
		char sql[256];
		sprintf(sql,"insert into activecode(code,used,validdate) values('%s',0,'%s')\0",code.c_str(),validdate.c_str());
		setUpdateSql(sql);
	}
}
/////////////////////////////////////////
//
//crJXJPlayerdataChangeQueryData
//
/////////////////////////////////////////
crJXJPlayerdataChangeQueryData::crJXJPlayerdataChangeQueryData():
	m_id(0),
	m_lastserverid(0),
	m_locked(0)
{
}
crJXJPlayerdataChangeQueryData::crJXJPlayerdataChangeQueryData(const crJXJPlayerdataChangeQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_lastserverid(data.m_lastserverid),
	m_locked(data.m_locked)
{
}


void crJXJPlayerdataChangeQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_lastserverid = atoi(buf);
		break;
	case 2:
		m_locked = (unsigned char)(atoi(buf));
		break;
	}
}

void crJXJPlayerdataChangeQueryData::buildQuerySql(const std::string& name,const std::string& pwd)
{
	char sql[128];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	sprintf(sql,"select id,lastserverid,locked from account where name='%s' and pwd=password('%s')\0",escname,pwd.c_str());
	setQuerySql(sql);
	setNumColumn(3);
}

int crJXJPlayerdataChangeQueryData::getPlayerID()
{
	return m_id;
}
unsigned char crJXJPlayerdataChangeQueryData::getLocked()
{
	return m_locked;
}
int crJXJPlayerdataChangeQueryData::getLastServerID()
{
	return m_lastserverid;
}
/////////////////////////////////////////
//
//crJXJInsertPlayerdataChangeList
//
/////////////////////////////////////////
void crJXJInsertPlayerdataChangeList::buildUpdateSql(int playerid, int serverid, int gmid, int changetype, int itemid, int count, const std::string &info)
{
	char sql[256];
	sprintf(sql, "insert into datachangelist(playerid,serverid,gmid,type,itemid,count,information,stats,createdate) values(%d,%d,%d,%d,%d,%d,'%s',1,now())\0", playerid, serverid, gmid, changetype, itemid, count, info.c_str());//stats1表示需要修改，0表示充值已经修改
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdatePlayerdataChangeList
//
/////////////////////////////////////////
void crJXJUpdatePlayerdataChangeList::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"update datachangelist set stats=0 where id=%d\0",id);//stats1表示需要修改，0表示充值已经修改
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJRemovePlayerdataChangeList 
//
/////////////////////////////////////////
void crJXJRemovePlayerdataChangeList::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"delete from datachangelist where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJInsertPlayerdataChangeRecord
//
/////////////////////////////////////////
void crJXJInsertPlayerdataChangeRecord::buildUpdateSql(int playerid, int serverid, int gmid, int changetype, int itemid, int count, const std::string &info)
{
	char sql[256];
	sprintf(sql, "insert into datachangerecord(playerid,serverid,gmid,type,itemid,count,information,createdate) values(%d,%d,%d,%d,%d,%d,'%s',now())\0", playerid, serverid, gmid, changetype, itemid, count, info.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crPlayerdataChangeListQueryData
//
/////////////////////////////////////////
crPlayerdataChangeListQueryData::crPlayerdataChangeListQueryData():
	m_id(0),
	m_playerid(0),
	m_type(0),
	m_itemid(0),
	m_count(0),
	m_gmid(0)
{
}
crPlayerdataChangeListQueryData::crPlayerdataChangeListQueryData(const crPlayerdataChangeListQueryData& data):
	crQueryData(data),
	m_id(data.m_id),
	m_playerid(data.m_playerid),
	m_type(data.m_type),
	m_itemid(data.m_itemid),
	m_count(data.m_count),
	m_gmid(data.m_gmid),
	m_information(data.m_information)
{
}
void crPlayerdataChangeListQueryData::addParam(int i, char *buf, int length)
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
		m_gmid = atoi(buf);
		break;
	case 3:
		m_type = atoi(buf);
		break;
	case 4:
		m_itemid = atoi(buf);
		break;
	case 5:
		m_count = atoi(buf);
		break;
	case 6:
		m_information = buf;
		break;
	}
}
void crPlayerdataChangeListQueryData::buildQuerySql(int serverid,int count)
{
	char sql[128];
	sprintf(sql,"select id,playerid,gmid,type,itemid,count,information from datachangelist where serverid=%d and stats=1 limit %d\0",serverid,count);
	setQuerySql(sql);
	setNumColumn(7);
}
int crPlayerdataChangeListQueryData::getID()
{
	return m_id;
}
int crPlayerdataChangeListQueryData::getPlayerID()
{
	return m_playerid;
}
int crPlayerdataChangeListQueryData::getType()
{
	return m_type;
}
int crPlayerdataChangeListQueryData::getItemID()
{
	return m_itemid;
}
int crPlayerdataChangeListQueryData::getCount()
{
	return m_count ;
}
int crPlayerdataChangeListQueryData::getGmid()
{
	return m_gmid;
}
const std::string& crPlayerdataChangeListQueryData::getInformation()
{
	return m_information;
}
/////////////////////////////////////////
//
//crJXJUpdatePlayerdataChangeList
//
/////////////////////////////////////////
void crJXJLockPlayerdata::buildUpdateSql(int id)
{//锁定玩家帐号
	char sql[128];
	sprintf(sql,"update account set locked=1 where id=%d\0",id);//1表示锁定0表示解锁
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdatePlayerdataChangeList
//
/////////////////////////////////////////
void crJXJUnLockPlayerdata::buildUpdateSql(int id)
{//解除锁定玩家帐号
	char sql[128];
	sprintf(sql,"update account set locked=0 where id=%d\0",id);//1表示锁定0表示解锁
	setUpdateSql(sql);
}

/**************************************************************************

METHOD: crJXJActiveCodeQueryData
 
**************************************************************************/

JXJ::crJXJActiveCodeQueryData::crJXJActiveCodeQueryData():
	m_codeType(0),
	m_getterID(0),
	m_serverID(0),
	m_used(0),
	m_userID(0)
{

}

JXJ::crJXJActiveCodeQueryData::crJXJActiveCodeQueryData( const crJXJActiveCodeQueryData & data ):
	crQueryData(data),
	m_codeType(data.m_codeType),
	m_getterID(data.m_getterID),
	m_serverID(data.m_serverID),
	m_used(data.m_used),
	m_userID(data.m_userID)
{

}

void JXJ::crJXJActiveCodeQueryData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_used = atoi(buf);
		break;
	case 1:
		m_codeType = atoi(buf);
		break;
	case 2:
		m_serverID = atoi(buf);
	case 3:
		m_getterID = atoi(buf);
		break;
	case 4:
		m_userID = atoi(buf);
		break;
	default:
		break;
	}
}

void JXJ::crJXJActiveCodeQueryData::buildQuerySql( const std::string & activeCode)
{
	char sql[128];
	sprintf(sql,"select used,codetype,serverid,getterid,userid from activecode where code = '%s'\0",activeCode.c_str());
	setQuerySql(sql);
	setNumColumn(5);
}

char JXJ::crJXJActiveCodeQueryData::getUsed()
{
	return m_used;
}

short JXJ::crJXJActiveCodeQueryData::getCodeType()
{
	return m_codeType;
}

int JXJ::crJXJActiveCodeQueryData::getServerID()
{
	return m_serverID;
}

int JXJ::crJXJActiveCodeQueryData::getGetterID()
{
	return m_getterID;
}

int JXJ::crJXJActiveCodeQueryData::getUserID()
{
	return m_userID;
}

/**************************************************************************

METHOD: crJXJCodeTypeQueryData
 
**************************************************************************/

JXJ::crJXJCodeTypeQueryData::crJXJCodeTypeQueryData()
{

}

JXJ::crJXJCodeTypeQueryData::crJXJCodeTypeQueryData( const crJXJCodeTypeQueryData & data ):
	crQueryData(data),
	m_validDate(data.m_validDate),
	m_overDate(data.m_overDate)
{

}

void JXJ::crJXJCodeTypeQueryData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_validDate = buf;
		break;
	case 1:
		m_overDate = buf;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCodeTypeQueryData::buildQuerySql(short codeType)
{
	char sql[128];
	sprintf(sql,"select validdate,overdate from codetype where codetype = %d\0",codeType);
	setQuerySql(sql);
	setNumColumn(2);
}

const std::string & JXJ::crJXJCodeTypeQueryData::getValidDate()
{
	return m_validDate;
}

const std::string & JXJ::crJXJCodeTypeQueryData::getOverDate()
{
	return m_overDate;
}

/**************************************************************************

METHOD: crJXJActiveCodeUpdate
 
**************************************************************************/
void JXJ::crJXJActiveCodeUpdate::buildUpdateSql( std::string & activeCode,int playerID )
{
	char sql[128];
	sprintf(sql,"update activecode set used=1,userid=%d,updatedate=now() where code='%s'\0",playerID,activeCode.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdatePlayerPassword
//
/////////////////////////////////////////
void crJXJUpdatePlayerPassword::buildUpdateSql(int id, std::string& newpassword)
{//锁定玩家帐号
	char sql[128];
	sprintf(sql,"update account set pwd=password('%s') where id=%d\0",newpassword.c_str() ,id);//修改玩家密码
	setUpdateSql(sql);
}

/**************************************************************************

METHOD: crJXJCheckPasswordQueryData
 
**************************************************************************/

JXJ::crJXJCheckPasswordQueryData::crJXJCheckPasswordQueryData()
{

}

JXJ::crJXJCheckPasswordQueryData::crJXJCheckPasswordQueryData( const crJXJCheckPasswordQueryData & data ):
	crQueryData(data)
{

}

void JXJ::crJXJCheckPasswordQueryData::buildQuerySql( int playerid ,std::string& password)
{
	char sql[256];
	sprintf(sql,"select id from account where id=%d and pwd=password('%s')\0",playerid,password.c_str());
	setQuerySql(sql);
	setNumColumn(1);
}

/////////////////////////////////////////
//
//crCheckPayMoneyCountData
//
/////////////////////////////////////////
crCheckPayMoneyCountData::crCheckPayMoneyCountData():
	m_count(0)
{
}
crCheckPayMoneyCountData::crCheckPayMoneyCountData(const crCheckPayMoneyCountData& data):
	crQueryData(data),
	m_count(data.m_count)
{
}
void crCheckPayMoneyCountData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(buf);
		break;
	}
}
void crCheckPayMoneyCountData::buildQuerySql(int playerid,int serverid)
{
	char sql[128];
	sprintf(sql,"select count(*) from moneyrecord where playerid=%d and serverid=%d and str='首次充值'\0",playerid,serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crCheckPayMoneyCountData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crInsertPlayerGiftGold
//
/////////////////////////////////////////
void crInsertPlayerGiftGold::buildUpdateSql(int playerid,int serverid,int money,const std::string &str,const std::string &advert_id)
{
	char sql[256];
	sprintf(sql,"insert into giftgoldrecord(playerid,serverid,giftgold,str,advert_id,createdate) values(%d,%d,%d,'%s','%s',now())\0",playerid,serverid,money,str.c_str(),advert_id.c_str());
	setUpdateSql(sql);
}

/**************************************************************************

METHOD: crGetTodayLoginPlayerData
 
**************************************************************************/

JXJ::crGetTodayLoginPlayerData::crGetTodayLoginPlayerData():
	m_count(0)
{

}

JXJ::crGetTodayLoginPlayerData::crGetTodayLoginPlayerData( const crGetTodayLoginPlayerData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crGetTodayLoginPlayerData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crGetTodayLoginPlayerData::buildQuerySql(int serverid,int overdue,int permission)
{
	char sql[900];

	// 确保在凌晨以前执行，否则天数不对
	sprintf(sql,
"select count(*) \
from \
(\
select creglobaldb.charactername.playerid \
from creglobaldb.charactername inner join creaccountdb.account \
on creglobaldb.charactername.playerid = creaccountdb.account.id \
and creglobaldb.charactername.serverid = %d \
and to_days(now()) - to_days(creaccountdb.account.logindate) = 0 \
and to_days(now()) - to_days(creglobaldb.charactername.createdate) = %d \
) as tmptable \
where tmptable.playerid not in ( \
select creglobaldb.gm.playerid \
from creglobaldb.gm \
where creglobaldb.gm.serverid = %d \
and creglobaldb.gm.authorityid != %d)\0"
		,serverid,overdue ,serverid,permission);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crGetTodayLoginPlayerData::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crSelectCountFromTableData
 
**************************************************************************/

JXJ::crSelectCountCharacternameExceptGMData::crSelectCountCharacternameExceptGMData():
	m_count(0)
{

}

JXJ::crSelectCountCharacternameExceptGMData::crSelectCountCharacternameExceptGMData( const crSelectCountCharacternameExceptGMData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crSelectCountCharacternameExceptGMData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crSelectCountCharacternameExceptGMData::buildQuerySql(int serverid,int permission)
{
	char sql[512];
	sprintf(sql,
"select count(*) \
from creglobaldb.charactername \
where creglobaldb.charactername.serverid = %d and \
creglobaldb.charactername.playerid not in ( \
select creglobaldb.gm.playerid \
from creglobaldb.gm \
where creglobaldb.gm.serverid = %d \
and creglobaldb.gm.authorityid != %d)\0",serverid,serverid,permission);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crSelectCountCharacternameExceptGMData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crJXJInsertMoneyrecord
//
/////////////////////////////////////////
void crJXJInsertMoneyrecord::buildUpdateSql(int playerid, int serverid, int money, int oldmoney, const std::string &str, const std::string &consortia, const std::string &name,const std::string &advert_id,const std::string &ordernumber)
{
	char sql[512];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	size = consortia.size();
	char escconsortia[32];
	memset(escconsortia,0,32);
	crDataBaseManager::getInstance()->escapeString(escconsortia,consortia.c_str(),size);

	sprintf(sql, "insert into moneyrecord(playerid,serverid,name,money,oldmoney,str,consortia,advert_id,ordernumber,createdate) values(%d,%d,'%s',%d,%d,'%s','%s','%s','%s',now())\0", playerid, serverid, escname,money, oldmoney, str.c_str(), escconsortia, advert_id.c_str(), ordernumber.c_str());
	setUpdateSql(sql);
}

/**************************************************************************

METHOD: 
 
**************************************************************************/

JXJ::crDataQueryTaskData::crDataQueryTaskData():
	m_id(0),
	m_playerid(0),
	m_queryID(0),
	m_queryType(0),
	m_gmID(0)
{

}

JXJ::crDataQueryTaskData::crDataQueryTaskData( const crDataQueryTaskData & data ):
	crQueryData(data),
	m_id(data.m_id),
	m_playerid(data.m_playerid),
	m_queryID(data.m_queryID),
	m_queryType(data.m_queryType),
	m_gmID(data.m_gmID)
{

}

void JXJ::crDataQueryTaskData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_playerid = atoi(buf);
		break;
	case 2:
		m_gmID = atoi(buf);
		break;
	case 3:
		m_queryID = atoi(buf);
		break;
	case 4:
		m_queryType = atoi(buf);
		break;
	default:
		break;
	}
}

void JXJ::crDataQueryTaskData::buildQuerySql(std::string tabname,int limitCount,int serverid)
{
	char sql[128];
	sprintf(sql,"select id,playerid,gmid,queryid,querytype from %s where serverid=%d limit %d\0",tabname.c_str(),serverid,limitCount);
	setQuerySql(sql);
	setNumColumn(5);
}

int JXJ::crDataQueryTaskData::getID()
{
	return m_id;
}

int JXJ::crDataQueryTaskData::getPlayerID()
{
	return m_playerid; 
}

int JXJ::crDataQueryTaskData::getQueryID()
{
	return m_queryID;

}

int JXJ::crDataQueryTaskData::getQueryType()
{
	return m_queryType;
}

int JXJ::crDataQueryTaskData::getGMID()
{
	return m_gmID;
}
//
void JXJ::crJXJRemoveDataQueryTaskRecord::buildUpdateSql( int id )
{
	char sql[128];
	sprintf(sql,"delete from dataquerytask where id=%d\0",id);
	setUpdateSql(sql);
}
//
void JXJ::crJXJInsertDataQueryRecord::buildUpdateSql( int playerid,int serverid,int queryid,int gmid,int querytype,unsigned char result,const std::string & retStr)
{
	char sql[256];
	sprintf(sql,"insert into dataqueryrecord(playerid,serverid,gmid,queryid,querytype,queryresult,returnstr,createdate) values(%d,%d,%d,%d,%d,%d,'%s', now())\0",playerid,serverid,gmid,queryid,querytype,result,retStr.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJRechargerecordQueryData
//
/////////////////////////////////////////
crJXJRechargerecordQueryData::crJXJRechargerecordQueryData() :
m_id(0),
m_playerid(0),
m_money(0)
{
}
crJXJRechargerecordQueryData::crJXJRechargerecordQueryData(const crJXJRechargerecordQueryData& data) :
crQueryData(data),
m_id(data.m_id),
m_playerid(data.m_playerid),
m_money(data.m_money),
m_ordernumber(data.m_ordernumber)
{
}
void crJXJRechargerecordQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_playerid = atoi(buf);
		break;
	case 2:
		m_money = atoi(buf);
		break;
	case 3:
		m_ordernumber = buf;
		break;
	}
}
void crJXJRechargerecordQueryData::buildQuerySql(int serverid, int count)
{
	char sql[128];
	sprintf(sql, "select id,playerid,money,ordernumber from rechargerecord where serverid=%d and stats=1 limit %d\0", serverid, count);
	setQuerySql(sql);
	setNumColumn(4);
}
int crJXJRechargerecordQueryData::getID()
{
	return m_id;
}
int crJXJRechargerecordQueryData::getPlayerID()
{
	return m_playerid;
}
int crJXJRechargerecordQueryData::getMoney()
{
	return m_money;
}
const std::string &crJXJRechargerecordQueryData::getOrderNumber()
{
	return m_ordernumber;
}
/////////////////////////////////////////
//
//crJXJInsertRechargerecord
//
/////////////////////////////////////////
void crJXJInsertRechargerecord::buildUpdateSql(int playerid,int serverid,int money,const std::string &str)
{
	char sql[256];
	sprintf(sql,"insert into rechargerecord(playerid,serverid,money,str,stats,createdate) values(%d,%d,%d,'%s',1,now())\0",playerid,serverid,money,str.c_str());//stats1表示充值，0表示充值已经提取
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdateRechargerecord
//
/////////////////////////////////////////
void crJXJUpdateRechargerecord::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"update rechargerecord set stats=0 where id=%d\0",id);//stats1表示充值，0表示充值已经提取
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJRemoveRechargerecord
//
/////////////////////////////////////////
void crJXJRemoveRechargerecord::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"delete from rechargerecord where id=%d or to_days(now())-to_days(createdate)>=3\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crBattleReportsInsert
//
/////////////////////////////////////////
crBattleReportsInsert::crBattleReportsInsert()
{}
void crBattleReportsInsert::buildUpdateSql(int battleid, CRCore::ref_ptr<CRCore::crStreamBuf> stream)
{
	int streamSize = 0;
	if (stream.valid()) streamSize = stream->getBufSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize + 256);
	char sql[256];
	if (streamSize == 0)
	{
		sprintf(sql, "insert into battlereport(createdate) values(now())\0");
		m_sqlstream->write(sql, strlen(sql));
	}
	else
	{
		sprintf(sql, "insert into battlereport(battleid,stream,createdate) values(%d,'\0",battleid);
		m_sqlstream->write(sql, strlen(sql));
		char* escstream = new char[streamSize * 2 + 1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream, stream->getPtr(), stream->getBufSize());
		m_sqlstream->write(escstream, len);
		delete[] escstream;
		//m_sqlstream->write(stream->getBuf(),stream->getBufSize());
		sprintf(sql, "',now())\0");
		m_sqlstream->write(sql, strlen(sql));
	}
}

/////////////////////////////////////////
//
//crBattleReportsQueryData
//
/////////////////////////////////////////
crBattleReportsQueryData::crBattleReportsQueryData() :m_battleid(0)
{
}
crBattleReportsQueryData::crBattleReportsQueryData(const crBattleReportsQueryData& data) :
crQueryData(data),
m_battleid(data.m_battleid),
m_stream(data.m_stream)
{
}
void crBattleReportsQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_battleid = atoi(buf);
		break;
	case 1:
		if (length > 0)
		{
			m_stream = new crStreamBuf;
			m_stream->createBuf(length);
			m_stream->write(buf, length);
			m_stream->seekBegin();
		}
		break;
	}
}
void crBattleReportsQueryData::buildQuerySql(int id)
{
	char sql[128];
	sprintf(sql, "select battleid,stream from battlereport where id=%d\0", id);
	setQuerySql(sql);
	setNumColumn(2);
}

int crBattleReportsQueryData::getBattleID()
{
	return m_battleid;
}

CRCore::crStreamBuf * crBattleReportsQueryData::getStream()
{
	return m_stream.get();
}

/////////////////////////////////////////
//
//crJXJGmQueryData
//
/////////////////////////////////////////
crJXJGmQueryData::crJXJGmQueryData():
	m_playerid(0),
	m_authorityid(0)
{
}
crJXJGmQueryData::crJXJGmQueryData(const crJXJGmQueryData& data):
	crQueryData(data),
	m_playerid(data.m_playerid),
	m_authorityid(data.m_authorityid)
{
}
void crJXJGmQueryData::addParam(int i, char *buf, int length)
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
void crJXJGmQueryData::buildQuerySql(int serverid)
{
	char sql[128];
	sprintf(sql,"select playerid,authorityid from gm where serverid=%d or serverid=0\0",serverid);
	setQuerySql(sql);
	setNumColumn(2);
}
int crJXJGmQueryData::getPlayerID()
{
	return m_playerid;
}
unsigned char crJXJGmQueryData::getAuthorityid()
{
	return m_authorityid;
}
/////////////////////////////////////////
//
//crJXJNoChatQueryData
//
/////////////////////////////////////////
crJXJNoChatQueryData::crJXJNoChatQueryData():
	m_playerid(0)
{
}
crJXJNoChatQueryData::crJXJNoChatQueryData(const crJXJNoChatQueryData& data):
	crQueryData(data),
	m_playerid(data.m_playerid)
{
}
void crJXJNoChatQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	}
}
void crJXJNoChatQueryData::buildQuerySql(int serverid)
{
	char sql[128];
	sprintf(sql,"select playerid from nochat where serverid=%d\0",serverid);
	setQuerySql(sql);
	setNumColumn(1);
}
int crJXJNoChatQueryData::getPlayerID()
{
	return m_playerid;
}

void JXJ::crJXJInsertPeopleMapRecord::buildUpdateSql( int playerid,unsigned char shili,const std::string & name,
	unsigned char lv, unsigned char troopsLv, int consortiaID, const std::string &consortiaName,
	unsigned char ismallacc,int killNum,int newerStep,int deadTroopsNum,
	int giftGold, int totalRecharge, int troopsnum, int exploit)
{
	char sql[1024];
	sprintf(sql,"insert into peoplemap(playerid,shili,name,\
level,troopslv,consortiaid,consortianame,issmallacc,killnum,newerstep,\
deadtroopsnum,giftgold,totalrecharge,troopsnum,exploit,updatedate) values(%d,%d,'%s',%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,NOW())\0",
playerid, shili, name.c_str(), lv, troopsLv, consortiaID, consortiaName.c_str(), ismallacc, killNum, newerStep, deadTroopsNum, giftGold, totalRecharge, troopsnum, exploit);
	setUpdateSql(sql);
}

void JXJ::crJXJUpdatePeopleMapRecord::buildUpdateSql( int playerid,unsigned char shili,const std::string & name, 
	unsigned char lv, unsigned char troopsLv, int consortiaID, const std::string &consortiaName,
	unsigned char ismallacc,int killNum,int newerStep,int deadTroopsNum,int giftGold,
	int totalRecharge,int troopsnum,int exploit, const std::string &super)
{
	char sql[1024];
	sprintf(sql,"update peoplemap set shili=%d,name='%s',\
level=%d,troopslv=%d,consortiaid=%d,consortianame='%s',issmallacc=%d,killnum=%d,newerstep=%d,\
deadtroopsnum=%d,giftgold=%d,totalrecharge=%d,troopsnum=%d,exploit=%d,super='%s',updatedate=now() where playerid=%d\0",
shili, name.c_str(), lv, troopsLv, consortiaID, consortiaName.c_str(), ismallacc, killNum, newerStep, deadTroopsNum, giftGold, totalRecharge, troopsnum, exploit, super.c_str(),playerid);
	setUpdateSql(sql);

}

/**************************************************************************

METHOD: crJXJPeopleMapQueryData
 
**************************************************************************/

JXJ::crJXJPeopleMapQueryData::crJXJPeopleMapQueryData():
	m_playerid(0),
	m_shili(0),
	m_lv(0),
	m_troopsLv(0),
	m_consortiaID(0),
	m_ismallacc(0),
	m_killNum(0),
	m_newerStep(0),
	m_deadTroopsNum(0),
	m_giftGold(0),
	m_totalRecharge(0),
	m_troopsnum(0),
	m_exploit(0)
{

}

JXJ::crJXJPeopleMapQueryData::crJXJPeopleMapQueryData( const crJXJPeopleMapQueryData & data ):
	m_playerid(data.m_playerid),
	m_shili(data.m_shili),
	m_name(data.m_name),
	m_lv(data.m_lv),
	m_troopsLv(data.m_troopsLv),
	m_consortiaID(data.m_consortiaID),
	m_ismallacc(data.m_ismallacc),
	m_killNum(data.m_killNum),
	m_newerStep(data.m_newerStep),
	m_deadTroopsNum(data.m_deadTroopsNum),
	m_giftGold(data.m_giftGold),
	m_totalRecharge(data.m_totalRecharge),
	m_troopsnum(data.m_troopsnum),
	m_exploit(data.m_exploit),
	m_super(data.m_super)
{
}

void JXJ::crJXJPeopleMapQueryData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	case 1:
		m_shili = atoi(buf);
		break;
	case 2:
		m_name = buf;
		break;
	case 3:
		m_lv = atoi(buf);
		break;
	case 4:
		m_troopsLv = atoi(buf);
		break;
	case 5:
		m_consortiaID = atoi(buf);
		break;
	case 6:
		m_ismallacc = atoi(buf);
		break;
	case 7:
		m_killNum = atoi(buf);
		break;
	case 8:
		m_newerStep = atoi(buf);
		break;
	case 9:
		m_deadTroopsNum = atoi(buf);
		break;
	case 10:
		m_giftGold = atoi(buf);
		break;
	case 11:
		m_totalRecharge = atoi(buf);
		break;
	case 12:
		m_troopsnum = atoi(buf);
		break;
	case 13:
		m_exploit = atoi(buf);
		break;
	case 14:
		m_super = buf;
		break;
	default:
		break;
	}
}

void JXJ::crJXJPeopleMapQueryData::buildQuerySql(int start,int count)
{
	char sql[512];
	sprintf(sql,"select playerid,shili,name,level,troopslv,consortiaid,issmallacc,killnum,newerstep,deadtroopsnum,giftgold,totalrecharge,troopsnum,exploit,super from peoplemap order by id limit %d,%d\0",start,count);
	setQuerySql(sql);
	setNumColumn(15);
}

int JXJ::crJXJPeopleMapQueryData::getPlayerID()
{
	return m_playerid;
}

int JXJ::crJXJPeopleMapQueryData::getShili()
{
	return m_shili;
}

const std::string & JXJ::crJXJPeopleMapQueryData::getName()
{
	return m_name;
}

unsigned char JXJ::crJXJPeopleMapQueryData::getLv()
{
	return m_lv;
}

unsigned char JXJ::crJXJPeopleMapQueryData::getTroopsLv()
{
	return m_troopsLv;
}

int JXJ::crJXJPeopleMapQueryData::getConsortiaID()
{
	return m_consortiaID;
}

unsigned char JXJ::crJXJPeopleMapQueryData::getSmallAcc()
{
	return m_ismallacc;
}

int JXJ::crJXJPeopleMapQueryData::getKillNum()
{
	return m_killNum;
}

int JXJ::crJXJPeopleMapQueryData::getNewerStep()
{
	return m_newerStep;
}

int JXJ::crJXJPeopleMapQueryData::getDeadTroopsNum()
{
	return m_deadTroopsNum;
}

int JXJ::crJXJPeopleMapQueryData::getGiftGold()
{
	return m_giftGold;
}

int JXJ::crJXJPeopleMapQueryData::getRecharge()
{
	return m_totalRecharge;
}

int JXJ::crJXJPeopleMapQueryData::gettroopsnum()
{
	return m_troopsnum;
}

int JXJ::crJXJPeopleMapQueryData::getexploit()
{
	return m_exploit;
}
const std::string &crJXJPeopleMapQueryData::getSuper()
{
	return m_super;
}
/**************************************************************************

METHOD: crSelectCountTableData 
**************************************************************************/
JXJ::crSelectCountTableData ::crSelectCountTableData ():
	m_count(0)
{

}

JXJ::crSelectCountTableData ::crSelectCountTableData ( const crSelectCountTableData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crSelectCountTableData ::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crSelectCountTableData ::buildQuerySql(std::string &tabName)
{
	char sql[128];
	sprintf(sql, "select count(*) from %s\0", tabName.c_str());
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crSelectCountTableData ::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crJXJSelectGMIDData
 
**************************************************************************/
JXJ::crJXJSelectGMIDData::crJXJSelectGMIDData():
	m_playerid(0)
{

}

JXJ::crJXJSelectGMIDData::crJXJSelectGMIDData( const crJXJSelectGMIDData & data ):
	crQueryData(data),
	m_playerid(data.m_playerid)
{

}

void JXJ::crJXJSelectGMIDData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_playerid = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crJXJSelectGMIDData::buildQuerySql(int serverid,int permission )
{
	char sql[128];
	sprintf(sql, "select playerid from gm where serverid = %d and authorityid != %d\0",serverid,permission);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crJXJSelectGMIDData::getPlayerID()
{
	return m_playerid;
}
//////////////////////////
//GM邮件crJXJRemoveGMMail//
//////////////////////////
void JXJ::crJXJRemoveGMMail::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql, "delete from gmmail where id=%d\0", id);
	setUpdateSql(sql);
}
////////////////////////////////
//GM邮件crJXJInsertGMMailRecord//
////////////////////////////////
void JXJ::crJXJInsertGMMailRecord::buildUpdateSql(int gmid,int serverid,unsigned char mailtype, int playerid, const std::string& playername, const std::string& title, const std::string& content, const std::string &item1,const std::string& item2,const std::string& item3,const std::string& item4)
{
	//int itemid[4] = { 0 };
	//int itemcount[4] = { 0 };
	//for (int i = 0; i < itemidvec.size();++i)
	//{
	//	if (i < itemcountvec.size() && itemcountvec[i] > 0)
	//	{
	//		itemid[i] = itemidvec[i];
	//		itemcount[i] = itemcountvec[i];
	//	}
	//}
	char sql[4096];
	//sprintf(sql, "insert into gmmailrecord(gmid,serverid,type,playerid,playername,title,content,itemid1,itemcount1,itemid2,itemcount2,itemid3,itemcount3,itemid4,itemcount4,createdate)values(%d,%d,%d,%d,'%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,now())\0", \
	//	gmid,serverid,mailtype, playerid, playername.c_str(), title.c_str(), content.c_str(), itemid[0], itemcount[0], itemid[1], itemcount[1], itemid[2], itemcount[2], itemid[3], itemcount[3]);
	sprintf(sql, "insert into gmmailrecord(gmid,serverid,type,playerid,playername,title,content,item1,item2,item3,item4,createdate)values(%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s',now())\0", \
		gmid,serverid,mailtype, playerid, playername.c_str(), title.c_str(), content.c_str(),item1.c_str(),item2.c_str(),item3.c_str(),item4.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJGMMailQueryData
//
/////////////////////////////////////////
crJXJGMMailQueryData::crJXJGMMailQueryData() :
m_id(0),
m_gmid(0),
m_type(0)
{
}
crJXJGMMailQueryData::crJXJGMMailQueryData(const crJXJGMMailQueryData& data) :
crQueryData(data),
m_id(data.m_id),
m_gmid(data.m_gmid),
m_type(data.m_type),
m_playername(data.m_playername),
m_title(data.m_title),
m_content(data.m_content),
m_item1(data.m_item1),
m_item2(data.m_item2),
m_item3(data.m_item3),
m_item4(data.m_item4)
{
}
void crJXJGMMailQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_gmid = atoi(buf);
		break;
	case 2:
		m_type = atoi(buf);
		break;
	case 3:
		m_playername = buf;
		break;
	case 4:
		m_title = buf;
		break;
	case 5:
		m_content = buf;
		break;
	case 6:
		m_item1 = buf;
		break;
	case 7:
		m_item2 = buf;
		break;
	case 8:
		m_item3 = buf;
		break;
	case 9:
		m_item4 = buf;
		break;
	//case 6:
	//case 8:
	//case 10:
	//case 12:
	//	m_itemidvec.push_back(atoi(buf));
	//	break;
	//case 7:
	//case 9:
	//case 11:
	//case 13:
	//	m_itemcountvec.push_back(atoi(buf));
	//	break;
	}
}
void crJXJGMMailQueryData::buildQuerySql(int count,int serverid)
{
	//m_itemidvec.clear();
	//m_itemcountvec.clear();
	char sql[256];
	sprintf(sql, "select id,gmid,type,playername,title,content,item1,item2,item3,item4 from gmmail where serverid=%d limit %d\0",serverid,count);
	setQuerySql(sql);
	setNumColumn(10);
}
int crJXJGMMailQueryData::getID()
{
	return m_id;
}
int crJXJGMMailQueryData::getGMID()
{
	return m_gmid;
}
unsigned char crJXJGMMailQueryData::getType()
{
	return m_type;
}
const std::string & crJXJGMMailQueryData::getPlayerName()
{
	return m_playername;
}
const std::string & crJXJGMMailQueryData::getTitle()
{
	return m_title;
}
const std::string & crJXJGMMailQueryData::getContent()
{
	return m_content;
}
const std::string & crJXJGMMailQueryData::getItem1()
{
	return m_item1;
}
const std::string & crJXJGMMailQueryData::getItem2()
{
	return m_item2;
}
const std::string & crJXJGMMailQueryData::getItem3()
{
	return m_item3;
}
const std::string & crJXJGMMailQueryData::getItem4()
{
	return m_item4;
}
//std::vector<int> crJXJGMMailQueryData::getItemIDVec()
//{
//	return m_itemidvec;
//}
//std::vector<int> crJXJGMMailQueryData::getItemCountVec()
//{
//	return m_itemcountvec;
//}
////////////////////////////
////GM公告crJXJRemoveGMNotice//
////////////////////////////
//void JXJ::crJXJRemoveGMNotice::buildUpdateSql(int id)
//{
//	char sql[128];
//	sprintf(sql, "DELETE from gmnotice where id=%d\0", id);
//	setUpdateSql(sql);
//}
//////////////////////////////////
////GM公告crJXJInsertGMNoticeRecord//
//////////////////////////////////
//void JXJ::crJXJInsertGMNoticeRecord::buildUpdateSql(int gmid, std::string& content)
//{
//	char sql[256];
//	sprintf(sql, "INSERT INTO gmnoticerecord(gmid,content,createdate) VALUES(%d,'%s',NOW())\0", gmid, content.c_str());
//	setUpdateSql(sql);
//}
/////////////////////////////////////////
//
//crJXJGMNoticeQueryData
//
/////////////////////////////////////////
crJXJGMNoticeQueryData::crJXJGMNoticeQueryData() :
m_id(0)
{
}
crJXJGMNoticeQueryData::crJXJGMNoticeQueryData(const crJXJGMNoticeQueryData& data) :
crQueryData(data),
m_id(data.m_id),
m_content(data.m_content)
{
}
void crJXJGMNoticeQueryData::addParam(int i, char *buf, int length)
{
	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_content = buf;
		break;
	}
}
void crJXJGMNoticeQueryData::buildQuerySql(int serverid)
{
	char sql[128];
	sprintf(sql, "select id,content from gmnotice where serverid=%d and starttime<=curtime() and curtime()<=endtime order by id desc limit 1\0",serverid);
	setQuerySql(sql);
	setNumColumn(5);
}
int crJXJGMNoticeQueryData::getID()
{
	return m_id;
}
const std::string & crJXJGMNoticeQueryData::getContent()
{
	return m_content;
}

/**************************************************************************

METHOD: crJXJSelectCountFromAccountByCreateDateData
 
**************************************************************************/
JXJ::crJXJSelectAllAccountByCreateDateData::crJXJSelectAllAccountByCreateDateData():
	m_count (0)
{

}

JXJ::crJXJSelectAllAccountByCreateDateData::crJXJSelectAllAccountByCreateDateData( const crJXJSelectAllAccountByCreateDateData & data ):
	crQueryData(data ),
	m_count(0)
{

}

void JXJ::crJXJSelectAllAccountByCreateDateData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crJXJSelectAllAccountByCreateDateData::buildQuerySql(int diffDay)
{
	char sql[128];
	sprintf(sql, "select count(*) from account where to_days(now()) - to_days(createdate) = %d\0", diffDay);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crJXJSelectAllAccountByCreateDateData::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crJXJSelectGMAccountByCreateDateData
 
**************************************************************************/

JXJ::crJXJSelectGMAccountByCreateDateData::crJXJSelectGMAccountByCreateDateData():
	m_count(0)
{

}

JXJ::crJXJSelectGMAccountByCreateDateData::crJXJSelectGMAccountByCreateDateData( const crJXJSelectGMAccountByCreateDateData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crJXJSelectGMAccountByCreateDateData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crJXJSelectGMAccountByCreateDateData::buildQuerySql(int serverID, int diffDay,int permission)
{
	char sql[512];
	sprintf(sql, 
"select count(*) \
from creglobaldb.gm inner join creaccountdb.account \
on creglobaldb.gm.playerid = creaccountdb.account.id \
where creglobaldb.gm.serverid = %d \
and creglobaldb.gm.authorityid != %d \
and to_days(now()) - to_days(creaccountdb.account.createdate) = %d\0"
,serverID,permission,diffDay);

	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crJXJSelectGMAccountByCreateDateData::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crJXJSelectAllCharacterByCreateDateData
 
**************************************************************************/

JXJ::crJXJSelectAllCharacterByCreateDateData::crJXJSelectAllCharacterByCreateDateData():
	m_count(0)
{

}

JXJ::crJXJSelectAllCharacterByCreateDateData::crJXJSelectAllCharacterByCreateDateData( const crJXJSelectAllCharacterByCreateDateData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crJXJSelectAllCharacterByCreateDateData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crJXJSelectAllCharacterByCreateDateData::buildQuerySql(int serverID,int diffDay)
{
	char sql[128];
	sprintf(sql, "select count(*) from charactername where serverid = %d and to_days(now()) - to_days(createdate) = %d \0",serverID, diffDay);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crJXJSelectAllCharacterByCreateDateData::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crJXJSelectGMCharacterByCreateDateData
 
**************************************************************************/

JXJ::crJXJSelectGMCharacterByCreateDateData::crJXJSelectGMCharacterByCreateDateData():
	m_count(0)
{

}

JXJ::crJXJSelectGMCharacterByCreateDateData::crJXJSelectGMCharacterByCreateDateData( const crJXJSelectGMCharacterByCreateDateData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crJXJSelectGMCharacterByCreateDateData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}

}

void JXJ::crJXJSelectGMCharacterByCreateDateData::buildQuerySql(int serverID,int permission, int diffDay)
{
	char sql[512];
	sprintf(sql, 
"select count(*) from gm inner join charactername \
on gm.playerid = charactername.playerid \
where gm.serverid = %d \
and gm.authorityid != %d \
and to_days(now()) - to_days(charactername.createdate) = %d\0" 
,serverID,permission,diffDay);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crJXJSelectGMCharacterByCreateDateData::getCount()
{
	return m_count;
}

/**************************************************************************

METHOD: crGetTodayLoginGMData
 
**************************************************************************/
JXJ::crGetTodayLoginGMData::crGetTodayLoginGMData():
	m_count(0)
{

}

JXJ::crGetTodayLoginGMData::crGetTodayLoginGMData( const crGetTodayLoginGMData & data ):
	crQueryData(data),
	m_count(0)
{

}

void JXJ::crGetTodayLoginGMData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_count = atoi(buf);
		break;
	default:
		break;
	}
}

void JXJ::crGetTodayLoginGMData::buildQuerySql( int serverid,int overdue ,int permission)
{
	char sql[900];

	// 确保在凌晨以前执行，否则天数不对

	sprintf_s(sql,
"select count(*) \
from creglobaldb.gm inner join creglobaldb.charactername inner join creaccountdb.account \
on creglobaldb.gm.playerid = creglobaldb.charactername.playerid \
and creglobaldb.gm.playerid = creaccountdb.account.id \
where creglobaldb.gm.serverid = %d \
and creglobaldb.gm.authorityid != %d \
and to_days(now()) - to_days(creglobaldb.charactername.createdate) = %d \
and to_days(now()) - to_days(creaccountdb.account.logindate) = 0\0"
,serverid,permission,overdue);
	setQuerySql(sql);
	setNumColumn(1);
}

int JXJ::crGetTodayLoginGMData::getCount()
{
	return m_count;
}
/////////////////////////////////////////
//
//crInsertDatamonitor
//
/////////////////////////////////////////
void crInsertDatamonitor::buildUpdateSql(int playerid, int serverid, std::string& str)
{
	char sql[256];
	sprintf(sql,"insert into datamonitor(playerid,serverid,str,createdate) values(%d,%d,'%s',now())\0",playerid,serverid,str.c_str());
	setUpdateSql(sql);
}

/************************************************************************/
/* 黄钻信息查询                                                         */
/************************************************************************/
crJXJYellowInfoQueryData::crJXJYellowInfoQueryData():
	m_isyellow(0),
	m_isyearyellow(0),
	m_yellowlevel(0)
{

}

crJXJYellowInfoQueryData::crJXJYellowInfoQueryData(const crJXJYellowInfoQueryData& data):
	crQueryData(data),
	m_isyellow(data.m_isyellow),
	m_isyearyellow(data.m_isyearyellow),
	m_yellowlevel(data.m_yellowlevel)
{

}

void crJXJYellowInfoQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_isyellow = (unsigned char)atoi(buf);
		break;
	case 1:
		m_isyearyellow = (unsigned char)atoi(buf);
		break;
	case 2:
		m_yellowlevel = (unsigned char)atoi(buf);
		break;
	}
}

void crJXJYellowInfoQueryData::buildQuerySql(int playerid)
{
	char sql[128];
	sprintf(sql,"select isyellow,isyearyellow,yellowlevel from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(3);
}

/************************************************************************/
/*                                                         */
/************************************************************************/
crTXMarketTaskQueryData::crTXMarketTaskQueryData():
	m_id(0),
	m_playerid(0),
	m_step(0)
{

}

crTXMarketTaskQueryData::crTXMarketTaskQueryData( const crTXMarketTaskQueryData & data ):
	crQueryData(data),
	m_id(data.m_id),
	m_playerid(data.m_playerid),
	m_step(data.m_step)
{

}

void crTXMarketTaskQueryData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_id = atoi(buf);
		break;
	case 1:
		m_playerid = atoi(buf);
		break;
	case 2:
		m_step = atoi(buf);
		break;
	case 3:
		m_taskid = buf;
		break;
	default:
		break;
	}
}

void crTXMarketTaskQueryData::buildQuerySql(int serverid, int count)
{
	char sql[128];
	sprintf(sql, "select id,playerid,step,taskid from tx_markettask where serverid=%d limit %d\0", serverid, count);
	setQuerySql(sql);
	setNumColumn(4);
}

int crTXMarketTaskQueryData::getID()
{
	return m_id;
}

int crTXMarketTaskQueryData::getPlayerID()
{
	return m_playerid; 
}

int crTXMarketTaskQueryData::getStep()
{
	return m_step;

}

const std::string& crTXMarketTaskQueryData::getTaskID()
{
	return m_taskid;
}

/************************************************************************/
/* crJXJRemoveTXMarketTask                                              */
/************************************************************************/
void crJXJRemoveTXMarketTask::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"delete from tx_markettask where id=%d\0",id);
	setUpdateSql(sql);
}

/************************************************************************/
/*                                                         */
/************************************************************************/
crTXMarketTaskRecordQueryData::crTXMarketTaskRecordQueryData():
	m_step(0)
{

}

crTXMarketTaskRecordQueryData::crTXMarketTaskRecordQueryData( const crTXMarketTaskRecordQueryData & data ):
	crQueryData(data),
	m_step(data.m_step)
{

}

void crTXMarketTaskRecordQueryData::addParam( int i, char *buf, int length )
{
	switch (i)
	{
	case 0:
		m_step = atoi(buf);
		break;
	default:
		break;
	}
}

void crTXMarketTaskRecordQueryData::buildQuerySql(int playerid, int serverid,const std::string& taskid)
{
	char sql[256];
	sprintf(sql, "select step from tx_marketrecod where playerid=%d and serverid=%d and taskid='%s'\0", playerid, serverid,taskid.c_str());
	setQuerySql(sql);
	setNumColumn(1);
}

int crTXMarketTaskRecordQueryData::getStep()
{
	return m_step;

}

/////////////////////////////////////////
//
//crJXJInsertTXTaskrecord
//
/////////////////////////////////////////
void crJXJInsertTXTaskrecord::buildUpdateSql(int playerid, int step,const std::string& taskid,int serverid, int state)
{
	char sql[256];
	sprintf(sql, "replace into tx_marketrecod(playerid,step,taskid,serverid,state,updatedate) values(%d,%d,'%s',%d,%d,now())\0", playerid,step,taskid.c_str(),serverid, state);
	setUpdateSql(sql);
}
void crJXJInsertTXTaskrecord::buildUpdateSql_task(const std::string& taskid,int playerid,int serverid,int step)
{
	char sql[256];
	sprintf(sql, "insert into tx_markettask(taskid,playerid,serverid,step,createdate) values('%s',%d,%d,%d,now())\0", taskid.c_str(),playerid,serverid,step);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJInsertTaskrecord
//
/////////////////////////////////////////
void crJXJInsertTaskrecord::buildUpdateSql(int playerid, int serverid, const std::string &name, int taskid, char status, int progress)
{
	char sql[256];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	sprintf(sql, "insert into taskrecord(playerid,serverid,name,taskid,status,progress,createdate) values(%d,%d,'%s',%d,%d,%d,now())\0", playerid, serverid, escname,taskid,status,progress);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJInsertOpinions
//
/////////////////////////////////////////
void crJXJInsertOpinions::buildUpdateSql(int playerid, int serverid, const std::string &name, const std::string &opinion)
{
	char sql[512];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);
	
	size = opinion.size();
	char escopin[300];
	memset(escopin,0,300);
	crDataBaseManager::getInstance()->escapeString(escopin,opinion.c_str(),size);

	sprintf(sql, "insert into opinions(playerid,serverid,name,opinion,createdate) values(%d,%d,'%s','%s',now())\0", playerid, serverid, escname,escopin);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJUpdateCharacterLevel
//
/////////////////////////////////////////
void crJXJUpdateCharacterLevel::buildUpdateSql(int playerid, int serverid, int roleid, int lv)
{
	char sql[128];
	sprintf(sql,"update charactername set level=%d where playerid=%d and serverid=%d and roleid=%d\0",lv,playerid,serverid,roleid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crJXJChildQueryData
//
/////////////////////////////////////////
crJXJChildQueryData::crJXJChildQueryData():
	m_child(0)
{
}

crJXJChildQueryData::crJXJChildQueryData(const crJXJChildQueryData& data):
	crQueryData(data),
	m_child(data.m_child)
{
}

void crJXJChildQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_child = (char)atoi(buf);
		break;
	}
}

void crJXJChildQueryData::buildQuerySql(int playerid)
{
	char sql[128];
	sprintf(sql,"select child from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(1);
}
/////////////////////////////////////////
//
//crJXJTelQueryData
//
/////////////////////////////////////////
crJXJTelQueryData::crJXJTelQueryData()
{
}

crJXJTelQueryData::crJXJTelQueryData(const crJXJTelQueryData& data):
	crQueryData(data),
	m_tel(data.m_tel)
{
}

void crJXJTelQueryData::addParam(int i, char *buf, int length)
{
	switch(i) 
	{
	case 0:
		m_tel = buf;
		break;
	}
}

void crJXJTelQueryData::buildQuerySql(int playerid)
{
	char sql[128];
	sprintf(sql,"select tel from account where id=%d\0",playerid);
	setQuerySql(sql);
	setNumColumn(1);
}
/////////////////////////////////////////
//
//crJXJUpdateTel
//
/////////////////////////////////////////
void crJXJUpdateTel::buildUpdateSql(int playerid, const std::string &tel)
{
	char sql[128];
	sprintf(sql,"update account set tel=%s where id=%d\0",tel.c_str(),playerid);
	setUpdateSql(sql);
}