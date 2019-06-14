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
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRCore/crNotify.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRIOManager/crFileUtils.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CRDataBase;
using namespace CRIOManager;
///////////////////////////////////
//
//crGlobalHandle
//
/////////////////////////////////////
ref_ptr<crGlobalHandle> crGlobalHandle::m_instance = NULL;
bool crGlobalHandle::s_isClient = false;
bool crGlobalHandle::s_openGameLogin = true;
int crGlobalHandle::s_version = 0;
int crGlobalHandle::s_newversion = 0;
HANDLE crGlobalHandle::s_runProtectHandle = NULL;
std::string crGlobalHandle::s_argvstr;
void crGlobalHandle::setClient()
{
	s_isClient = true;
}
bool crGlobalHandle::isClient()
{
	return s_isClient;
}
bool crGlobalHandle::gOpenGameLogin()
{
	return s_openGameLogin;
}
void crGlobalHandle::gSetOpenGameLogin(int open)
{
	s_openGameLogin = open;
	char buf[16];
	sprintf(buf, "opengamelogin %d\0", s_openGameLogin ? 1 : 0);
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->write(buf, 16);
	stream->seekBegin();
	stream->saveToFile2("opengamelogin.ini");
}
int crGlobalHandle::getVersion()
{
	return s_version;
}
void crGlobalHandle::setVersion(int ver)
{
	s_version = ver;
}
void crGlobalHandle::setNewVersion(int ver)
{
	s_newversion = ver;
}
int crGlobalHandle::getNewVersion()
{
	return s_newversion;
}
void crGlobalHandle::setRunProtectHandle(HANDLE hangle)
{
	s_runProtectHandle = hangle;
}
HANDLE crGlobalHandle::getRunProtectHandle()
{
	return s_runProtectHandle;
}
crGlobalHandle::crGlobalHandle():
	m_currentMsg(0L),
	m_startupLoadingProgress(0.0f),
	m_curLoadingProgress(0.0f),
	m_timer(0.0)
{
	int open = 1;
	std::string filename = "opengamelogin.ini";
	if( CRIOManager::fileExists(filename))
	{
		std::ifstream fin(filename.c_str(), std::ios_base::in|std::ios::binary);
		fin.seekg(0, std::ios::end); 
		int count  =  fin.tellg();
		fin.seekg(0, std::ios::beg);
		char *buf = new char[count+1];
		memset(buf,0,count+1);
		fin.read(buf,count);
		crArgumentParser::readKeyValue(buf,"opengamelogin",open);
		fin.close();
		delete [] buf;
	}
	s_openGameLogin = open;
}
crGlobalHandle* crGlobalHandle::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crGlobalHandle;
	}
	return m_instance.get();
}

void crGlobalHandle::clear()
{
	s_recycleItemIDDeque.clear();
	s_recycleItemChildIDDeque.clear();
	s_recycleRoomIDDeque.clear();
	s_recycleHttpDownloadIDDeque.clear();
	m_dataClass = NULL;
	m_instance = NULL;
}

void crGlobalHandle::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crGlobalHandle::getDataClass()
{
	return m_dataClass.get();
}
void crGlobalHandle::doEvent(_crInt64 kbmsg, CREPARAM param)
{
	//GNE::LockMutex lock( m_globalEventMutex );
	m_currentMsg = kbmsg;
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"crGlobalHandle::doEvent error msg = "<<kbmsg<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
				char gbuf[256];
				sprintf(gbuf, "crGlobalHandle::doEvent error msg=%s,DataName=%s,DataClassName=%s\n\0", crArgumentParser::appI64toa(kbmsg).c_str(), data->_name(), data->className());
				gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}
void crGlobalHandle::insertAbstractItem(crAbstractItem *aitem)
{
	GNE::LockMutex lock( m_abstractItemMapMutex );
	m_abstractItemMap[aitem->getAbstractItemID()] = aitem;
}
crAbstractItem *crGlobalHandle::findAbstractItem(int abstractid)
{
	GNE::LockMutex lock( m_abstractItemMapMutex );
	AbstractItemMap::iterator itr = m_abstractItemMap.find(abstractid);
	return itr == m_abstractItemMap.end() ? NULL : itr->second.get();
}
crAbstractItem *crGlobalHandle::gameServerQueryAbstractItem(int abstractid)
{
	if(abstractid <= 0)
		return NULL;
	GNE::LockMutex lock( m_abstractItemMapMutex );
	AbstractItemMap::iterator itr = m_abstractItemMap.find(abstractid);
	if(itr != m_abstractItemMap.end())
	{
		return itr->second.get();
	}
	crDataBase *resdb = crDataBaseManager::getInstance()->getDataBase(ResDB);
	ref_ptr<crDataBase> resSession = resdb->beginSession();
	int resid = 0;
	std::string itemname,itemscript;
	ref_ptr<crAbstractItemQueryData> abstractItemQuery = new crAbstractItemQueryData;
	abstractItemQuery->buildQuerySql(abstractid);
	resSession->executeQuery(abstractItemQuery.get());
	crDataBase::QueryResultVec& abstractItemQueryResultVec = resSession->getQueryResult();
	if(!abstractItemQueryResultVec.empty())
	{
		abstractItemQuery = dynamic_cast<crAbstractItemQueryData *>(abstractItemQueryResultVec[0].get());
		resid = abstractItemQuery->getResID();
		itemname = abstractItemQuery->getName();
		itemscript = abstractItemQuery->getScript();
	}
	else
	{
		resSession->releaseQuery();
		resdb->endSession(resSession.get());
		//MessageBox(::GetDesktopWindow(),buf,"Creature3D",MB_OK);
		//_asm   int   3   //只是为了让程序崩溃
		CRCore::notify(CRCore::ALWAYS)<<"gameServerQueryAbstractItem没有找到AbstractItemID="<<abstractid<<std::endl;
		//sprintf(gDebugInfo->buf(),"gameServerQueryAbstractItem没有找到AbstractItemID=%d\n\0",abstractid);
		//gDebugInfo->debugInfo(CRCore::ALWAYS);
		return NULL;
	}
	resSession->releaseQuery();
	std::string filename, resscript;
	if(resid>0)
	{
		ref_ptr<crResQueryData> resQuery = new crResQueryData;
		resQuery->buildQuerySql(resid);
		resSession->executeQuery(resQuery.get());
		crDataBase::QueryResultVec& resQueryResultVec = resSession->getQueryResult();
		if(!resQueryResultVec.empty())
		{
			resQuery = dynamic_cast<crResQueryData *>(resQueryResultVec[0].get());
			filename = resQuery->getFileName();
			resscript = resQuery->getScript();
		}
		resSession->releaseQuery();
	}
	resdb->endSession(resSession.get());

	ref_ptr<crAbstractItem> aitem = new crAbstractItem;
	aitem->setAbstractItemID(abstractid);
	aitem->setName(itemname);
	aitem->setScript(itemscript);
	aitem->setResID(resid);
	aitem->setFileName(filename);
	aitem->setResScript(resscript);
	m_abstractItemMap[aitem->getAbstractItemID()] = aitem;
	return aitem.get();
}
void crGlobalHandle::insertAbstractMetier(crAbstractMetier *aitem)
{
	GNE::LockMutex lock( m_abstractMetierMapMutex );
	m_abstractMetierMap[aitem->getAbstractMetierID()] = aitem;
}
crAbstractMetier *crGlobalHandle::findAbstractMetier(int abstractid)
{
	GNE::LockMutex lock( m_abstractMetierMapMutex );
	AbstractMetierMap::iterator itr = m_abstractMetierMap.find(abstractid);
	return itr == m_abstractMetierMap.end() ? NULL : itr->second.get();
}
crAbstractMetier *crGlobalHandle::gameServerQueryAbstractMetier(int abstractid)
{
	GNE::LockMutex lock( m_abstractMetierMapMutex );
	AbstractMetierMap::iterator itr = m_abstractMetierMap.find(abstractid);
	if(itr != m_abstractMetierMap.end())
	{
		return itr->second.get();
	}
	ref_ptr<crAbstractMetier> ametier;
	crDataBase *resdb = crDataBaseManager::getInstance()->getDataBase(ResDB);
	ref_ptr<crDataBase> resSession = resdb->beginSession();
	//crAbstractMetierQueryData
	ref_ptr<crAbstractMetierQueryData> abstractMetierQuery = new crAbstractMetierQueryData;
	abstractMetierQuery->buildQuerySql(abstractid);
	resSession->executeQuery(abstractMetierQuery.get());
	crDataBase::QueryResultVec &abstractMetierQueryResultVec = resSession->getQueryResult();
	if(!abstractMetierQueryResultVec.empty())
	{
		abstractMetierQuery = dynamic_cast<crAbstractMetierQueryData*>(abstractMetierQueryResultVec[0].get());
		ametier = new crAbstractMetier;
		ametier->setAbstractMetierID(abstractid);
		ametier->setGameID(abstractMetierQuery->getGameID());
		ametier->setMetierName(abstractMetierQuery->getMetierName());
		ametier->setInitSceneID(abstractMetierQuery->getInitSceneID());
		ametier->setInitLayerID(abstractMetierQuery->getInitLayerID());
		ametier->setZoffset(abstractMetierQuery->getZoffset());
		ametier->setMetierScript(abstractMetierQuery->getScript());
		ametier->setTaskTable(abstractMetierQuery->getTaskTable());
		m_abstractMetierMap[ametier->getAbstractMetierID()] = ametier;
	}
	resSession->releaseQuery();
	resdb->endSession(resSession.get());
	return ametier.get();
}
int crGlobalHandle::gameServerQueryPlayerID(const std::string &charactername,int serverid)
{
	int playerid = 0;
	crDataBase *globaldb = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = globaldb->beginSession();
	//crAbstractMetierQueryData
	ref_ptr<crCharacterNamePlayerIDQueryData> query = new crCharacterNamePlayerIDQueryData;
	query->buildQuerySql(charactername,serverid);
	globalSession->executeQuery(query.get());
	crDataBase::QueryResultVec &resultVec = globalSession->getQueryResult();
	if(!resultVec.empty())
	{
		query = dynamic_cast<crCharacterNamePlayerIDQueryData*>(resultVec[0].get());
		playerid = query->getPlayerID();
	}
	globalSession->releaseQuery();
	globaldb->endSession(globalSession.get());
	return playerid;
}
crGlobalData *crGlobalHandle::gData()
{
	return dynamic_cast<crGlobalData *>(getInstance()->getDataClass());
}
void crGlobalHandle::setGameName(const std::string &gamename)
{
	m_gameName = gamename;
}
const std::string& crGlobalHandle::getGameName() const
{
	return m_gameName;
}
std::string crGlobalHandle::getGameDir() const
{
	if(m_gameName.empty())
		return "";
	return m_gameName + "/";
}
std::string crGlobalHandle::getScriptDir() const
{
	if(m_gameName.empty())
		return "script/";
	return m_gameName + "/script/";
}
std::string crGlobalHandle::getTextDir() const
{
	if(m_gameName.empty())
		return "script/text/";
	return m_gameName + "/script/text/";
}
std::string crGlobalHandle::getTableDir() const
{
	if(m_gameName.empty())
		return "script/table/";
	return m_gameName + "/script/table/";
}
std::string crGlobalHandle::getUITableDir() const
{
	if(m_gameName.empty())
		return "script/uitable/";
	return m_gameName + "/script/uitable/";
}
std::string crGlobalHandle::get3DUIDir() const
{
	if(m_gameName.empty())
		return "script/3dui/";
	return m_gameName + "/script/3dui/";
}
std::string crGlobalHandle::getScenarioTableDir() const
{
	if(m_gameName.empty())
		return "script/scenariotable/";
	return m_gameName + "/script/scenariotable/";
}
std::string crGlobalHandle::getScenarioDir() const
{
	if(m_gameName.empty())
		return "script/scenario/";
	return m_gameName + "/script/scenario/";
}
std::string crGlobalHandle::getTextureDir() const
{
	return "media/textures/";
}
std::string crGlobalHandle::getUISceneDir() const
{
	return "uiscene/";
}
void crGlobalHandle::getScriptStream(rcfg::ConfigScript& script, CRCore::ref_ptr<CRCore::crStreamBuf> &outstream)
{
	GNE::LockMutex lock( m_scriptStreamMutex );
	char file[128];
	sprintf(file,"%s%d%d.tmp\0","ScriptStream",GetCurrentThreadId(),time(0));
	script.Write(file,"");
	outstream = new crStreamBuf;
	outstream->loadFromFile2(file);
	DeleteFileA(file);
}
void crGlobalHandle::getStreamScript(CRCore::crStreamBuf *stream, rcfg::ConfigScript& outscript)
{
	GNE::LockMutex lock( m_scriptStreamMutex );
	char file[128];
	sprintf(file,"%s%d%d.tmp\0","StreamScript",GetCurrentThreadId(),time(0));
	stream->saveToFile2(file);
	outscript.Open(file,"");
	DeleteFileA(file);
}
crGlobalHandle::TimeRecycleIDDeque crGlobalHandle::s_recycleItemIDDeque;
GNE::Mutex crGlobalHandle::s_recycleItemIDDequeMutex;
int crGlobalHandle::s_temporaryItemID = -1;
crGlobalHandle::TimeRecycleIDDeque crGlobalHandle::s_recycleItemChildIDDeque;
GNE::Mutex crGlobalHandle::s_recycleItemChildIDDequeMutex;
int crGlobalHandle::s_temporaryItemChildID = -1;
int crGlobalHandle::gainTemporaryItemID()
{
	int id;
	GNE::LockMutex lock( s_recycleItemIDDequeMutex );
	if(!s_recycleItemIDDeque.empty())
	{
		TimeRecycleIDDeque::iterator itr = s_recycleItemIDDeque.begin();
		if(itr->second>crGlobalHandle::gData()->gRecycleIDTime())
		{
			id = itr->first;
			s_recycleItemIDDeque.pop_front();
			return id;
		}
	}
	id = s_temporaryItemID;
	s_temporaryItemID--;
	return id;
}
void crGlobalHandle::recycleItemID(int id)
{
	if(id<0)
	{
		GNE::LockMutex lock( s_recycleItemIDDequeMutex );
		for( TimeRecycleIDDeque::iterator itr = s_recycleItemIDDeque.begin();
			itr != s_recycleItemIDDeque.end();
			++itr )
		{
			if(itr->first == id)
			{
				itr->second = 0.0f;
				return;
			}
		}
		s_recycleItemIDDeque.push_back(std::make_pair(id,0.0f));
	}
}
int crGlobalHandle::gainTemporaryItemChildID()
{
	int id;
	GNE::LockMutex lock( s_recycleItemChildIDDequeMutex );
	if(!s_recycleItemChildIDDeque.empty())
	{
		TimeRecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
		if(itr->second>crGlobalHandle::gData()->gRecycleIDTime())
		{
			id = itr->first;
			s_recycleItemChildIDDeque.pop_front();
			return id;
		}
	}
	id = s_temporaryItemChildID;
	s_temporaryItemChildID--;
	return id;
}
void crGlobalHandle::recycleItemChildID(int id)
{
	if(id<0)
	{
		GNE::LockMutex lock( s_recycleItemChildIDDequeMutex );
		for( TimeRecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
			itr != s_recycleItemChildIDDeque.end();
			++itr )
		{
			if(itr->first == id)
			{
				itr->second = 0.0f;
				return;
			}
		}
		s_recycleItemChildIDDeque.push_back(std::make_pair(id,0.0f));
	}
}
void crGlobalHandle::updateTimeRecycleID(float dt)
{
	s_recycleItemIDDequeMutex.acquire();
	for( TimeRecycleIDDeque::iterator itr = s_recycleItemIDDeque.begin();
		itr != s_recycleItemIDDeque.end();
		++itr )
	{
		if(itr->second <= crGlobalHandle::gData()->gRecycleIDTime())
			itr->second+=dt;
	}
	s_recycleItemIDDequeMutex.release();

	s_recycleItemChildIDDequeMutex.acquire();
	for( TimeRecycleIDDeque::iterator itr = s_recycleItemChildIDDeque.begin();
		itr != s_recycleItemChildIDDeque.end();
		++itr )
	{
		if(itr->second <= crGlobalHandle::gData()->gRecycleIDTime())
			itr->second+=dt;
	}
	s_recycleItemChildIDDequeMutex.release();
}
int crGlobalHandle::s_temporaryRoomID = -1;
crGlobalHandle::RecycleIDDeque crGlobalHandle::s_recycleRoomIDDeque;
GNE::Mutex crGlobalHandle::s_recycleRoomIDDequeMutex;
int crGlobalHandle::gainTemporaryRoomID()
{
	int id;
	//GNE::LockMutex lock( s_recycleRoomIDDequeMutex );
	//if(!s_recycleRoomIDDeque.empty())
	//{
	//	RecycleIDDeque::iterator itr = s_recycleRoomIDDeque.begin();
	//	id = *itr;
	//	s_recycleRoomIDDeque.pop_front();
	//	return id;
	//}
	id = s_temporaryRoomID;
	s_temporaryRoomID--;
	if (s_temporaryRoomID == 0)
		s_temporaryRoomID = -1;
	return id;
}
void crGlobalHandle::recycleRoomID(int id)
{
	//if(id<0)
	//{
	//	GNE::LockMutex lock( s_recycleRoomIDDequeMutex );
	//	for( RecycleIDDeque::iterator itr = s_recycleRoomIDDeque.begin();
	//		itr != s_recycleRoomIDDeque.end();
	//		++itr )
	//	{
	//		if(*itr == id)
	//		{
	//			return;
	//		}
	//	}
	//	s_recycleRoomIDDeque.push_back(id);
	//}
}
unsigned short crGlobalHandle::s_httpDownloadID = 1;
crGlobalHandle::RecycleUSIDDeque crGlobalHandle::s_recycleHttpDownloadIDDeque;
GNE::Mutex crGlobalHandle::s_recycleHttpDownloadIDDequeMutex;
unsigned short crGlobalHandle::gainHttpDownloadID()
{
	int id;
	GNE::LockMutex lock( s_recycleHttpDownloadIDDequeMutex );
	if(!s_recycleHttpDownloadIDDeque.empty())
	{
		RecycleUSIDDeque::iterator itr = s_recycleHttpDownloadIDDeque.begin();
		id = *itr;
		s_recycleHttpDownloadIDDeque.pop_front();
		return id;
	}
	id = s_httpDownloadID;
	s_httpDownloadID++;
	return id;
}
void crGlobalHandle::recycleHttpDownloadID(unsigned short id)
{
	if(id!=0)
	{
		GNE::LockMutex lock( s_recycleHttpDownloadIDDequeMutex );
		for( RecycleUSIDDeque::iterator itr = s_recycleHttpDownloadIDDeque.begin();
			itr != s_recycleHttpDownloadIDDeque.end();
			++itr )
		{
			if(*itr == id)
			{
				return;
			}
		}
		s_recycleHttpDownloadIDDeque.push_back(id);
	}
}
void crGlobalHandle::setStartupLoadingProgress(float progress)
{
	m_curLoadingProgress = m_startupLoadingProgress;
	m_startupLoadingProgress = progress;
	if(m_curLoadingProgress>m_startupLoadingProgress)
		m_curLoadingProgress = m_startupLoadingProgress;
}
float crGlobalHandle::getStartupLoadingProgress()
{
	return m_startupLoadingProgress;
}
float &crGlobalHandle::curStartupLoadingProgress()
{
	return m_curLoadingProgress;
}
/////////////////////////////////////////
//
//crRunGameData
//
/////////////////////////////////////////
crRunGameData::crRunGameData():
m_parentWind(NULL),
m_sceneid(0)
{
}
crRunGameData::crRunGameData(const crRunGameData& data):
crData(data),
m_parentWind(NULL),
m_sceneFile(data.m_sceneFile),
m_uifile(data.m_uifile),
m_brainScript(data.m_brainScript),
m_sceneid(data.m_sceneid)
{
}
void crRunGameData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_SceneID:
		m_sceneid = atoi(relStr.c_str());
		break;
	}
}
void crRunGameData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_SceneFile:
		m_sceneFile = *(std::string*)(param);
		break;
	case WCHDATA_UIFile:
		m_uifile = *(std::string*)(param);
		break;
	case WCHDATA_BrainScript:
		m_brainScript = *(std::string*)(param);
		break;
	case WCHDATA_SceneID:
		m_sceneid = param?*(short*)(param):0;
		break;
	}
}
void crRunGameData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_SceneFile:
		param = &m_sceneFile;
		break;
	case WCHDATA_UIFile:
		param = &m_uifile;
		break;
	case WCHDATA_BrainScript:
		param = &m_brainScript;
		break;
	case WCHDATA_SceneID:
		param = &m_sceneid;
		break;
	default:
		param = NULL;
		break;
	}
}
///////////////////////////////////
//
//crRunGameHandle
//
/////////////////////////////////////
ref_ptr<crRunGameHandle> crRunGameHandle::m_instance = NULL;
crRunGameHandle::crRunGameHandle():
m_running(true),
m_runCode(0),
m_runningCode(-1),
m_parentWnd(NULL),
m_renderWnd(NULL),
m_done(false),
m_loadingProgress(0.0f),
m_curLoadingProgress(0.0f)
{
}
crRunGameHandle* crRunGameHandle::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crRunGameHandle;
	}
	return m_instance.get();
}

void crRunGameHandle::clear()
{
	m_dataClass = NULL;
	m_instance = NULL;
}

void crRunGameHandle::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crRunGameHandle::getDataClass()
{
	return m_dataClass.get();
}
void crRunGameHandle::doEvent(_crInt64 kbmsg, CREPARAM param)
{
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"crRunGameHandle::doEvent error msg = "<<kbmsg<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
				char gbuf[256];
				sprintf(gbuf, "crRunGameHandle::doEvent error msg=%s,DataName=%s,DataClassName=%s\n\0", crArgumentParser::appI64toa(kbmsg).c_str(), data->_name(), data->className());
				gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}
void crRunGameHandle::setRunCode(int runCode)
{
	m_runCode = runCode;
}
int crRunGameHandle::getRunCode()
{
	return m_runCode;
}
void crRunGameHandle::setRunningCode(int runCode)
{
	m_runningCode = runCode;
}
int crRunGameHandle::getRunningCode()
{
	if(m_runningCode==m_runCode)
		return m_runningCode;
	return -1;
}
void crRunGameHandle::setRunning(bool run)
{
	m_running = run;
}
bool crRunGameHandle::isRunning()
{
	return m_running;
}
void crRunGameHandle::setParentWnd(Producer::Window parentWnd)
{
	m_parentWnd = parentWnd;
}
Producer::Window crRunGameHandle::getParentWnd()
{
	return m_parentWnd;
}
void crRunGameHandle::setRenderWnd(Producer::Window renderWnd)
{
	m_renderWnd = renderWnd;
}
Producer::Window crRunGameHandle::getRenderWnd()
{
	return m_renderWnd;
}
void crRunGameHandle::start()
{
	m_done = false;
}
void crRunGameHandle::end()
{
	m_done = true;
}
void crRunGameHandle::addRole(crRole *role)
{
	m_roleVec.push_back(role);
}
crRunGameHandle::RoleVec &crRunGameHandle::getRoleVec()
{
	return m_roleVec;
}
void crRunGameHandle::setLoadingProgress(float progress)
{
	m_curLoadingProgress = m_loadingProgress;
	m_loadingProgress = progress;
	if(m_curLoadingProgress>m_loadingProgress)
		m_curLoadingProgress = m_loadingProgress;
}
float crRunGameHandle::getLoadingProgress()
{
	return m_loadingProgress;
}
float &crRunGameHandle::curLoadingProgress()
{
	return m_curLoadingProgress;
}
///////////////////////////////////
//
//crServerBrainHandle
//
/////////////////////////////////////
ref_ptr<crServerBrainHandle> crServerBrainHandle::m_instance = NULL;
crServerBrainHandle::crServerBrainHandle()
{
}
crServerBrainHandle* crServerBrainHandle::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crServerBrainHandle;
	}
	return m_instance.get();
}

void crServerBrainHandle::clear()
{
	m_dataClass = NULL;
	m_instance = NULL;
}

void crServerBrainHandle::setDataClass( CRCore::crData *data )
{
	m_dataClass = data;
}
CRCore::crData *crServerBrainHandle::getDataClass()
{
	return m_dataClass.get();
}
void crServerBrainHandle::doEvent(_crInt64 kbmsg, CREPARAM param)
{
	if(m_dataClass.valid())
	{
		ref_ptr<crData> data = m_dataClass;
		crHandle *handle = data->getHandle(kbmsg);
		if(handle)
		{
			crHandleManager::getInstance()->waitToExecuteHandle(handle);
			try
			{
				handle->inputParam(1,this);
				handle->inputParam(2,&param);
				data->excHandle(handle);
			}
			catch (...)
			{
				//CRCore::notify(CRCore::ALWAYS)<<"crServerBrainHandle::doEvent error msg = "<<kbmsg<<std::endl;
				//_asm   int   3   //只是为了让程序崩溃
				//sprintf(gDebugInfo->buf(),"crRunGameHandle::doEvent error msg=%s\n\0",crArgumentParser::appI64toa(kbmsg).c_str());
				char gbuf[256];
				sprintf(gbuf, "crServerBrainHandle::doEvent error msg=%s,DataName=%s,DataClassName=%s\n\0", crArgumentParser::appI64toa(kbmsg).c_str(), data->_name(), data->className());
				gDebugInfo->debugInfo(CRCore::ALWAYS, gbuf);
			}
			crHandleManager::getInstance()->endExecuteHandle(handle);
		}
	}
}