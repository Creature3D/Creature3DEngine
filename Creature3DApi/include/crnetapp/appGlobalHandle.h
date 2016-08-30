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
#ifndef CRNETAPP_GLOBALHANDLE_H
#define CRNETAPP_GLOBALHANDLE_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crDisplaySettings.h>
#include <CRProducer/crViewer.h>
#include <CREncapsulation/crTableIO.h>
#include <gnelib.h>
namespace CRNetApp{
class crAbstractItem;
class crAbstractMetier;
class CRNETAPP_EXPORT crGlobalHandle : public CRCore::Referenced
{
public:
	crGlobalHandle();
	static crGlobalHandle *getInstance();
	void clear();

	void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
	inline _crInt64 getCurrentMsg() const { return m_currentMsg; }

	//abstractitem缓冲
	typedef std::map< int,CRCore::ref_ptr<crAbstractItem> > AbstractItemMap;
	void insertAbstractItem(crAbstractItem *aitem);
	crAbstractItem *findAbstractItem(int abstractid);
	//GameServer abstractitem查询
	crAbstractItem *gameServerQueryAbstractItem(int abstractid);

	typedef std::map< int,CRCore::ref_ptr<crAbstractMetier> > AbstractMetierMap;
	void insertAbstractMetier(crAbstractMetier *metier);
	crAbstractMetier *findAbstractMetier(int metierid);
	//GameServer abstractmetier查询
	crAbstractMetier *gameServerQueryAbstractMetier(int metierid);

	//GameServer PlayerID查询
	int gameServerQueryPlayerID(const std::string &charactername,int serverid);//返回0表示未找到

	static crGlobalData *gData();
	static void setClient();
	static bool isClient();
	static bool &gOpenGameLogin();
	static void setVersion(int ver);
	static int getVersion();
	static void setNewVersion(int ver);
	static int getNewVersion();
	static void setRunProtectHandle(HANDLE hangle);
	static HANDLE getRunProtectHandle();
	void setGameName(const std::string &gamename);
	const std::string& getGameName() const;
	std::string getGameDir() const;
	std::string getScriptDir() const;
	std::string getTextDir() const;
	std::string getTableDir() const;
	std::string getUITableDir() const;
	std::string get3DUIDir() const;
	std::string getScenarioTableDir() const;
	std::string getScenarioDir() const;
	std::string getTextureDir() const;
	std::string getUISceneDir() const;

	void getScriptStream(rcfg::ConfigScript& script, CRCore::ref_ptr<CRCore::crStreamBuf> &outstream);
	void getStreamScript(CRCore::crStreamBuf *stream, rcfg::ConfigScript& outscript);

	typedef std::pair<int,float>TimeRecycleIDPair;
	typedef std::deque<TimeRecycleIDPair> TimeRecycleIDDeque;
	static int gainTemporaryItemID();
	static void recycleItemID(int id,float delay = crGlobalHandle::gData()->gRecycleIDTime());
	static int gainTemporaryItemChildID();
	static void recycleItemChildID(int id,float delay = crGlobalHandle::gData()->gRecycleIDTime());
	static void updateTimeRecycleID(float dt);
	typedef std::deque<int> RecycleIDDeque;
	static int gainTemporaryRoomID();
	static void recycleRoomID(int id);

	typedef std::deque<unsigned short> RecycleUSIDDeque;
	static unsigned short gainHttpDownloadID();
	static void recycleHttpDownloadID(unsigned short id);
	void setStartupLoadingProgress(float progress);
	float getStartupLoadingProgress();
	float &curStartupLoadingProgress();
	void setTimer(double t){ m_timer = t;}
	double &timer(){ return m_timer; }
	static std::string &argvstr() { return s_argvstr; }
protected:
	virtual ~crGlobalHandle(){}
	static CRCore::ref_ptr<crGlobalHandle> m_instance;

	CRCore::ref_ptr<CRCore::crData> m_dataClass;
	_crInt64 m_currentMsg;
	//GNE::Mutex m_globalEventMutex;

	AbstractItemMap m_abstractItemMap;
	GNE::Mutex m_abstractItemMapMutex;
	AbstractMetierMap m_abstractMetierMap;
	GNE::Mutex m_abstractMetierMapMutex;
	static bool s_isClient;
	static bool s_openGameLogin;//false 只能gm帐号可以登录， ture，普通玩家可以登录
	static int s_version;
	static int s_newversion;
	std::string m_gameName;
	static HANDLE s_runProtectHandle;
	GNE::Mutex m_scriptStreamMutex;

	static TimeRecycleIDDeque s_recycleItemIDDeque;
	static GNE::Mutex s_recycleItemIDDequeMutex;
	static int s_temporaryItemID;

	static TimeRecycleIDDeque s_recycleItemChildIDDeque;
	static GNE::Mutex s_recycleItemChildIDDequeMutex;
	static int s_temporaryItemChildID;

	static RecycleIDDeque s_recycleRoomIDDeque;
	static GNE::Mutex s_recycleRoomIDDequeMutex;
	static int s_temporaryRoomID;

	static RecycleUSIDDeque s_recycleHttpDownloadIDDeque;
	static GNE::Mutex s_recycleHttpDownloadIDDequeMutex;
	static unsigned short s_httpDownloadID;
	float m_startupLoadingProgress;//启动加载进度
	float m_curLoadingProgress;//
	double m_timer;//
	static std::string s_argvstr;
};

class CRNETAPP_EXPORT crRunGameData : public CRCore::crData
{
public:
	crRunGameData();
	crRunGameData(const crRunGameData& data);
	DataClass(CRNetApp, RunGame)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	int m_parentWind;
	std::string m_sceneFile;
	std::string m_uifile;
	std::string m_brainScript;
	//add param
	short m_sceneid;
};
class CRNETAPP_EXPORT crRunGameHandle : public CRCore::Referenced
{
public:
	crRunGameHandle();
	static crRunGameHandle *getInstance();
	void clear();

	void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);

	enum RunCode
	{
		Login,
		RoleSelect,//or	RoleCreate, or 领地场景
		NetGame,//或者NetRoomGame
		SingleGame,
		SwitchScene,
	};
	void setRunCode(int runCode);
	int getRunCode();
	void setRunningCode(int runCode);
	int getRunningCode();
	void setRunning(bool run);
	bool isRunning();
	void setParentWnd(Producer::Window parentWnd);
	Producer::Window getParentWnd();
	void setRenderWnd(Producer::Window renderWnd);
	Producer::Window getRenderWnd();
	void start();
	void end();
	inline bool isInGame() { return m_running && !m_done && CRCore::crDisplaySettings::instance()->getRunMode() != 0; }
	inline bool isInManor() { return m_running && !m_done && m_runningCode == RoleSelect && m_runningCode == m_runCode; }
	inline bool isInBattle() { return m_running && !m_done && m_runningCode == NetGame && m_runningCode == m_runCode; }
	inline bool isSwitchScene() { return m_running && m_runningCode !=  m_runCode; }
	typedef std::vector< CRCore::ref_ptr<crRole> >RoleVec;
	void addRole(crRole *role);
	RoleVec &getRoleVec();
	void setLoadingProgress(float progress);
	float getLoadingProgress();
	float &curLoadingProgress();
	typedef std::vector<std::string> StrVec;
	void addStartupDownloadFile(const std::string &file){ m_startupDownloadFileVec.push_back(file); }
	StrVec &getStartupDownloadFileVec(){ return m_startupDownloadFileVec; }
protected:
	virtual ~crRunGameHandle(){}
	static CRCore::ref_ptr<crRunGameHandle> m_instance;

	CRCore::ref_ptr<CRCore::crData> m_dataClass;
	int m_runCode;
	int m_runningCode;//当前正在渲染的场景的runcode,-1表示正在切换场景
	bool m_running;
	bool m_done;
	Producer::Window m_parentWnd;
	Producer::Window m_renderWnd;
	RoleVec m_roleVec;
	float m_loadingProgress;//加载进度
	float m_curLoadingProgress;
	StrVec m_startupDownloadFileVec;
};
class CRNETAPP_EXPORT crServerBrainHandle : public CRCore::Referenced
{
public:
	crServerBrainHandle();
	static crServerBrainHandle *getInstance();
	void clear();

	void setDataClass( CRCore::crData *data );
	CRCore::crData *getDataClass();
	virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);
protected:
	virtual ~crServerBrainHandle(){}
	static CRCore::ref_ptr<crServerBrainHandle> m_instance;
	CRCore::ref_ptr<CRCore::crData> m_dataClass;
};
}
#endif