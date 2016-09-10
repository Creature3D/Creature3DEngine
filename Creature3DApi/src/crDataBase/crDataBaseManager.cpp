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
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>
#include <errmsg.h>
#include <my_getopt.h>
#include <string>
#include <CRCore/thread/crScopedLock.h>
using namespace CRDataBase;
using namespace CRCore;

/////////////////////////////////////////
//
//crUpdateSql
//
/////////////////////////////////////////
crUpdateSql::crUpdateSql()
{

}
void crUpdateSql::setUpdateSql(const std::string &sql)
{
    m_updateSql = sql;
	m_sqlstream = NULL;
}
const std::string &crUpdateSql::getUpdateSql()const
{
    return m_updateSql;
}
void crUpdateSql::setUpdateSql(CRCore::crStreamBuf *sql)
{
	m_sqlstream = sql;
	m_updateSql.clear();
}
CRCore::crStreamBuf *crUpdateSql::getSqlStream()
{
	return m_sqlstream.get();
}
/////////////////////////////////////////
//
//crQueryData
//
/////////////////////////////////////////
crQueryData::crQueryData():
m_numColumn(0)
{
}
crQueryData::crQueryData(const crQueryData& data):
crData(data),
m_querySql(data.m_querySql),
m_numColumn(data.m_numColumn)
{
}
void crQueryData::setQuerySql(const std::string &sql)
{
    m_querySql = sql;
	m_sqlstream = NULL;
}
const std::string &crQueryData::getQuerySql()const
{
	return m_querySql;
}
void crQueryData::setQuerySql(CRCore::crStreamBuf *sql)
{
	m_sqlstream = sql;
	m_querySql.clear();
}
CRCore::crStreamBuf *crQueryData::getSqlStream()
{
	return m_sqlstream.get();
}
void crQueryData::setNumColumn(int numColumn)
{
	m_numColumn = numColumn;
}
int crQueryData::getNumColumn()
{
	return m_numColumn;
}
/////////////////////////////////////////
//
//crDataBase
//
/////////////////////////////////////////
crDataBase::crDataBase()
{
	//m_updateDB = NULL;
	//m_queryDB = NULL;
	m_db = NULL;
	m_port = MYSQL_PORT;
}

crDataBase::~crDataBase()
{
	releaseQuery();
	//if(m_updateDB != NULL)
	//	mysql_close( (MYSQL *)m_updateDB );
	//if(m_queryDB != NULL)
	//	mysql_close( (MYSQL *)m_queryDB );
	
	//m_updateDB = NULL;
	//m_queryDB = NULL;

	if(m_db != NULL)
		mysql_close( (MYSQL *)m_db );
	m_db = NULL;
	//CRCore::notify(CRCore::NOTICE)<<"~crNetContainer"<<std::endl;
}

//void crDataBase::init(const std::string &updateIp,const std::string &queryIp,int dbport,const std::string &user,const std::string &password,const std::string &database)
//{
//	char value = 1;
//	if(!m_updateDB)
//	{
//	    m_updateDB = mysql_init((MYSQL*) 0);
//		mysql_options((MYSQL *)m_updateDB, MYSQL_OPT_RECONNECT, (char *)&value);
//		mysql_options((MYSQL *)m_updateDB, MYSQL_SET_CHARSET_NAME, "gbk");
//	}
//	if(!m_queryDB)
//	{
//		m_queryDB = mysql_init((MYSQL*) 0);
//		mysql_options((MYSQL *)m_queryDB, MYSQL_OPT_RECONNECT, (char *)&value);
//		mysql_options((MYSQL *)m_queryDB, MYSQL_SET_CHARSET_NAME, "gbk");
//	}
//	m_updateDBIP = updateIp;
//    m_queryDBIP = queryIp;
//	m_port = dbport;
//	m_userName = user;
//	m_password = password;
//	m_database = database;
//	//mysql_options((MYSQL*)m_myData,MYSQL_OPT_COMPRESS,0);
//	//mysql_options((MYSQL*)m_myData,MYSQL_READ_DEFAULT_GROUP,"odbc");
//}
void crDataBase::init(const std::string &dbIp,int dbport,const std::string &user,const std::string &password,const std::string &database)
{
	char value = 1;
	if(!m_db)
	{
		m_db = mysql_init((MYSQL*) 0);
		mysql_options((MYSQL *)m_db, MYSQL_OPT_RECONNECT, (char *)&value);
		mysql_options((MYSQL *)m_db, MYSQL_SET_CHARSET_NAME, "gbk");
	}
	m_dbip = dbIp;
	m_port = dbport;
	m_userName = user;
	m_password = password;
	m_database = database;
	//mysql_options((MYSQL*)m_myData,MYSQL_OPT_COMPRESS,0);
	//mysql_options((MYSQL*)m_myData,MYSQL_READ_DEFAULT_GROUP,"odbc");
}
//int crDataBase::openUpdateDataBase()
//{//return 1;
//	if ( mysql_real_connect( (MYSQL *)m_updateDB, m_updateDBIP.c_str(), m_userName.c_str(), m_password.c_str(), m_database.c_str(), m_port/*MYSQL_PORT*/, NULL, 0 ) )
//	{
//		((MYSQL *)m_updateDB)->reconnect = 1;
//		return 1;
//	}
//	CRCore::notify(CRCore::WARN)<<"crDataBase connect faild: "<<getUpdateError()<<std::endl;
//	return 0;
//}
//
//int crDataBase::openQueryDataBase()
//{//return 1;
//	if ( mysql_real_connect( (MYSQL *)m_queryDB, m_queryDBIP.c_str(), m_userName.c_str(), m_password.c_str(), m_database.c_str(), m_port/*MYSQL_PORT*/, NULL, 0 ) )
//	{
//		((MYSQL *)m_queryDB)->reconnect = 1;
//		return 1;
//	}
//	CRCore::notify(CRCore::WARN)<<"crDataBase connect faild: "<<getQueryError()<<std::endl;
//	return 0;
//}
int crDataBase::openDataBase()
{
	if ( mysql_real_connect( (MYSQL *)m_db, m_dbip.c_str(), m_userName.c_str(), m_password.c_str(), m_database.c_str(), m_port/*MYSQL_PORT*/, NULL, 0 ) )
	{
		((MYSQL *)m_db)->reconnect = 1;
		return 1;
	}
	CRCore::notify(CRCore::WARN)<<"crDataBase connect faild: "<<getQueryError()<<std::endl;
	return 0;
}

//void crDataBase::closeUpdateDataBase()
//{
//	if(m_updateDB != NULL)
//		mysql_close( (MYSQL *)m_updateDB );
//
//	m_updateDB = NULL;
//}
//
//void crDataBase::closeQueryDataBase()
//{
//	if(m_queryDB != NULL)
//		mysql_close( (MYSQL *)m_queryDB );
//
//	m_queryDB = NULL;
//}
void crDataBase::closeDataBase()
{
	if(m_db != NULL)
		mysql_close( (MYSQL *)m_db );

	m_db = NULL;
}

int crDataBase::executeUpdate(crUpdateSql *sql)
{
	if(!sql) return 0;
	MYSQL_RES *results;
	crStreamBuf *sqlstream = sql->getSqlStream();
	if(sqlstream)
	{
		//CRCore::ScopedLock<CRCore::crMutex> lock(m_updateDBMutex);
		if (!mysql_real_query((MYSQL *)/*m_updateDB*/m_db, sqlstream->getBuf(),sqlstream->getBufSize()))
		{
			results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
			mysql_free_result(results);
			return 1;
		}
		const char *err = getUpdateError();
		crDataBaseManager::getInstance()->dbErrorRecord(sqlstream->getBuf(),sqlstream->getBufSize(),err);
		results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
		mysql_free_result(results);
		CRCore::notify(CRCore::WARN)<<"crDataBase::executeUpdate faild: "<<err<<std::endl;
		return 0;
	}
    return executeUpdate(sql->getUpdateSql());
}
int crDataBase::executeUpdate(char *sql,int length)
{
	if(!sql) return 0;
	MYSQL_RES *results;
	if (!mysql_real_query((MYSQL *)/*m_updateDB*/m_db, sql,length))
	{
		results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
		mysql_free_result(results);
		return 1;
	}
	const char *err = getUpdateError();
	crDataBaseManager::getInstance()->dbErrorRecord(sql,length,err);
	results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
	mysql_free_result(results);
	CRCore::notify(CRCore::WARN)<<"crDataBase::executeUpdate faild: "<<err<<std::endl;
	return 0;
}	
int crDataBase::executeUpdate(const std::string &sql)
{
	//CRCore::ScopedLock<CRCore::crMutex> lock(m_updateDBMutex);
	MYSQL_RES *results;
	if (!mysql_query((MYSQL *)/*m_updateDB*/m_db, sql.c_str()))
	{
		results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
		mysql_free_result(results);
        return 1;
	}
	const char *err = getUpdateError();
	crDataBaseManager::getInstance()->dbErrorRecord(sql.c_str(),sql.length(),err);
	results = mysql_use_result((MYSQL *)/*m_updateDB*/m_db);
	mysql_free_result(results);
	CRCore::notify(CRCore::WARN)<<"crDataBase::executeUpdate faild: "<<err<<std::endl;
	return 0;
}

int crDataBase::executeQuery(const std::string &sql)
{
	//CRCore::ScopedLock<CRCore::crMutex> lock(m_queryDBMutex);
	MYSQL_RES *results;
	if (!mysql_query((MYSQL *)/*m_queryDB*/m_db, sql.c_str()))
	{
		results = mysql_use_result((MYSQL *)/*m_queryDB*/m_db);
		mysql_free_result(results);
		return 1;
	}
	const char *err = getQueryError();
	crDataBaseManager::getInstance()->dbErrorRecord(sql.c_str(),sql.length(),err);
	results = mysql_use_result((MYSQL *)/*m_queryDB*/m_db);
	mysql_free_result(results);
	CRCore::notify(CRCore::WARN)<<"crDataBase::executeQuery faild: "<<err<<std::endl;
	return 0;
}
int crDataBase::executeQuery(crQueryData *query)
{
	if(!query)
		return 0;
	//m_queryDBMutex.lock();
	MYSQL_RES *results;
	MYSQL_ROW record;
    m_queryResultVec.clear();
	int queryresut = 1;
	crStreamBuf *sqlstream = query->getSqlStream();
	if(sqlstream)
	{
        queryresut = mysql_real_query((MYSQL *)/*m_queryDB*/m_db,sqlstream->getBuf(),sqlstream->getBufSize());
	}
	else
	{
		queryresut = mysql_query((MYSQL *)/*m_queryDB*/m_db,query->getQuerySql().c_str());
	}
	if (!queryresut)
	{
		results = mysql_store_result((MYSQL *)/*m_queryDB*/m_db);
		int nRecord = 0;
		int nColumn = 0;
		//std::string str;
		char *buf;
		crQueryData *queryResult;
		unsigned int num_fields;
		unsigned long *lengths;
		while((record = mysql_fetch_row(results))) 
		{
			num_fields = mysql_num_fields(results);
			lengths = mysql_fetch_lengths(results);
			if(nRecord == 0)
                queryResult = query;
			else
			    queryResult = dynamic_cast<crQueryData *>(query->cloneType());
			nRecord++;
			nColumn = 0;
			for(; nColumn</*query->getNumColumn()*/num_fields; nColumn++)
			{
				try
				{
				    buf = record[nColumn];
					if(buf && lengths[nColumn]>0)
				        queryResult->addParam(nColumn,buf,lengths[nColumn]);
				}
				catch(...)
				{
					CRCore::notify(CRCore::WARN)<<"crDataBase::executeQuery addParam error: "<<queryResult->className()<<std::endl;
				}
			}
			m_queryResultVec.push_back(queryResult);
		}
		mysql_free_result(results);
		return 1;
	}
	const char *err = getQueryError();
	if(sqlstream)
	{
		crDataBaseManager::getInstance()->dbErrorRecord(sqlstream->getBuf(),sqlstream->getBufSize(),err);
	}
	else
	{
		crDataBaseManager::getInstance()->dbErrorRecord(query->getQuerySql().c_str(),query->getQuerySql().length(),err);
	}
	results = mysql_use_result((MYSQL *)/*m_queryDB*/m_db);
	mysql_free_result(results);
	CRCore::notify(CRCore::WARN)<<"crDataBase::executeQuery faild: "<<err<<std::endl;
	return 0;
}

crDataBase::QueryResultVec &crDataBase::getQueryResult()
{
    return m_queryResultVec;
}

void crDataBase::releaseQuery()
{
    m_queryResultVec.clear();
	//m_queryDBMutex.unlock();
}

void crDataBase::commit()
{
    mysql_commit((MYSQL *)/*m_updateDB*/m_db);
}

void crDataBase::rollback()
{
    mysql_rollback((MYSQL *)/*m_updateDB*/m_db);
}

int crDataBase::getInsertID()
{
    return mysql_insert_id((MYSQL *)/*m_updateDB*/m_db);
}

unsigned long crDataBase::realEscapeString(char *to, const char *from, int from_length)
{
	return mysql_real_escape_string((MYSQL *)/*m_updateDB*/m_db,to,from,from_length);
}

crDataBase *crDataBase::beginSession()
{
	crDataBase *session = this;
	session->m_sessionMutex.lock();
    return session;
}

void crDataBase::endSession(crDataBase *session)
{
	session->m_sessionMutex.unlock();
}
void crDataBase::dbping()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_sessionMutex);
	if(/*m_queryDB*/m_db) mysql_ping((MYSQL *)/*m_queryDB*/m_db);
	//if(m_updateDB) mysql_ping((MYSQL *)m_updateDB);
}
const char *crDataBase::getQueryError()
{
	return mysql_error((MYSQL *)/*m_queryDB*/m_db);
}
const char *crDataBase::getUpdateError()
{
	return mysql_error((MYSQL *)/*m_updateDB*/m_db);
}

/////////////////////////////////////////
//
//crDataBaseManager
//
/////////////////////////////////////////
ref_ptr<crDataBaseManager> crDataBaseManager::m_instance = NULL;

crDataBaseManager* crDataBaseManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDataBaseManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crDataBaseManager::clear()
{
	m_instance = NULL;
}

void crDataBaseManager::setAccountDB(crDataBase *db)
{
	m_accountDB = db;
}
crDataBase *crDataBaseManager::getAccountDB()
{
	return m_accountDB.get();
}

void crDataBaseManager::setGlobalDB(crDataBase *db)
{
    m_globalDB = db;
}

crDataBase *crDataBaseManager::getGlobalDB()
{
    return m_globalDB.get();
}

void crDataBaseManager::initDBMapVec(int size)
{
	m_dbMapVec.resize(size);
}

void crDataBaseManager::insertDataBase(unsigned short enumid,unsigned short dbid, crDataBase *db)
{
	if(enumid<m_dbMapVec.size())
        m_dbMapVec[enumid][dbid] = db;
}

crDataBase *crDataBaseManager::getDataBase(unsigned short enumid,unsigned short dbid)
{
	if(enumid<m_dbMapVec.size())
	{
		DBMap::iterator itr = m_dbMapVec[enumid].find(dbid);
		return itr != m_dbMapVec[enumid].end() ? itr->second.get(): NULL;
	}
    return NULL;
}

void crDataBaseManager::initDBVec(int size)
{
	m_dbVec.resize(size);
}

void crDataBaseManager::insertDataBase(unsigned short enumid,crDataBase *db)
{
	if(enumid<m_dbVec.size())
	    m_dbVec[enumid] = db;
}

crDataBase *crDataBaseManager::getDataBase(unsigned short enumid)
{
	if(enumid<m_dbVec.size())
	{
		return m_dbVec[enumid].get();
	}
	return NULL;
}
void crDataBaseManager::dbping()
{
	CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	float dt = CRCore::Timer::instance()->delta_s( m_pingTime, t1 );
	if(dt>3600.0f)
	{
		m_pingTime = t1;
		if(m_accountDB.valid())
			m_accountDB->dbping();
		if(m_globalDB.valid())
			m_globalDB->dbping();
		for( DBVec::iterator itr = m_dbVec.begin();
			itr != m_dbVec.end();
			++itr )
		{
			(*itr)->dbping();
		}
		for( DBMapVec::iterator itr = m_dbMapVec.begin();
			itr != m_dbMapVec.end();
			++itr )
		{
			for( DBMap::iterator ditr = itr->begin();
				ditr != itr->end();
				++ditr )
			{
				ditr->second->dbping();
			}
		}
	}
}
void crDataBaseManager::dbErrorRecord(const char *sql,int length,const char *err)
{
	char *buf = new char[length+1];
	if(buf)
	{
		memset(buf,0,length+1);
		memcpy(buf,sql,length);
		std::string filename = "dberror.log";
		std::ofstream fout(filename.c_str(), std::ios::binary|std::ios::app);
		if(fout)
		{
			time_t t = time( 0 ); 
			char tmp[20];
			strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
			if(err)
			{
				fout<<tmp<<" sql:"<<buf<<" err:"<<err<<std::endl;
			}
			else
			{
				fout<<tmp<<" sql:"<<buf<<std::endl;
			}
			fout.close();
		}
		delete buf;
	}
}
unsigned long crDataBaseManager::escapeString(char *to, const char *from, int from_length)
{
	if(m_globalDB.valid())
		return m_globalDB->realEscapeString(to,from,from_length);
	return mysql_escape_string(to,from,from_length);
}