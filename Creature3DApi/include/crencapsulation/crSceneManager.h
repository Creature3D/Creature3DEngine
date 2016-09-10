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
#ifndef CRENCAPSULATION_CRSCENEMANAGER
#define CRENCAPSULATION_CRSCENEMANAGER 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crQuat.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crShape.h>
#include <CRCore/crAITaskManager.h>
#include <CRCore/thread/crThread.h>
#include <map>
#include <string>
#include <vector>
#include <set>
namespace CREncapsulation {
class CRENCAPSULATION_EXPORT crSave : public CRCore::Referenced
{
public:
	crSave();
	void setName(const std::string& name);
	const std::string& getName()const;
	void setBotName(const std::string& name);
	const std::string& getBotName()const;
	std::string getBotFile(bool isNet = false)const;
	void setHp(int hp);
	int getHp()const;
	void setDefend(int defend);
	int getDefend()const;
	void setTeam(int team);
	int getTeam()const;
	void setExperience(int experience);
	int getExperience()const;
	void setOrientation(const CRCore::crQuat &orientation);
	const CRCore::crQuat& getOrientation()const;
	void setPosition(const CRCore::crVector3 &position);
	const CRCore::crVector3& getPosition()const;
	//typedef std::set<std::string> KilledBotNameSet;
	//void clearKilledBotNameVec();
	//void insertKilledBotName(const std::string &name);
	//bool isBotKilled(const std::string &name);
	void setProgress(int progress);
	inline int getProgress()const{ return m_progress; }
	void setWeapon(int index, const std::string &weaponName);
	const std::string &getWeapon1() const;
	const std::string &getWeapon2() const;
	const std::string &getWeapon3() const;
	enum GameState 
	{
		SingleInit = 0,
		SingleRunning,
		NetInit,
		NetRunning
	};
	void setGameState(GameState gameState);
	GameState getGameState();
	virtual void addParam(int i, const std::string &str);

	void setSceneID(int sceneid) { m_sceneid = sceneid; }
	int getSceneID(){ return m_sceneid; }
	void setSaveTime(const std::string &saveTime) { m_saveTime = saveTime; }
	const std::string& getSaveTime(){ return m_saveTime; }

	//enum TeamMode 
	//{
	//	RedTeam = 0,
	//	BlueTeam
	//};
	//void setTeamMode(unsigned int teamMode);
	//TeamMode getTeamMode()const;

	inline void setDataClass(CRCore::crData* dataClass) { m_dataClass = dataClass; }
	inline CRCore::crData* getDataClass() { return m_dataClass.get(); }
	inline const CRCore::crData* getDataClass() const { return m_dataClass.get(); }
private:
	std::string m_name;
	int m_sceneid;
	std::string m_saveTime;

	//bot信息
	std::string m_botName;
	//std::string m_botFile;
    int m_hp;
	int m_defend;
	int m_team;
	int m_experience;
	int m_progress;
    CRCore::crQuat m_orientation;
	CRCore::crVector3 m_position;
	std::string m_weapons[3];
	//KilledBotNameSet m_killedBotNameSet;
	//int m_initGame;
	GameState m_gameState;
	//TeamMode m_teamMode;
	CRCore::ref_ptr<CRCore::crData> m_dataClass;
};
class crMapArea;
class CRENCAPSULATION_EXPORT crNPC : public CRCore::Referenced
{//NPC
public :
	crNPC();
    crNPC(const crNPC& npc);
	virtual crNPC* clone() const { return new crNPC(*this); }

	void setNPCID(int npcid);
	int getNPCID();
	void setName(const std::string &name);
	const std::string &getName();
	void setScriptFileName(const std::string &scriptFileName);
	const std::string &getScriptFileName();
	void setReviveTaskByName(const std::string &taskName);
	void setReviveTask(CRCore::crAITask *aiTask);
	void revive(int i);//i=-1表示随机诞生点
	bool isDead();

	void setNPCBody(CRCore::crEventGroup *body);
	CRCore::crEventGroup *getNPCBody();
	void setNPCRadar(CRCore::crEventGroup *radar);
	CRCore::crEventGroup *getNPCRadar();
	void setNPCParent(CRCore::crGroup *parent);
	CRCore::crGroup *getNPCParent();
    
	CRPhysics::crViewMatterObject *getNPCBot();

	void setMaxRange(float range);
	float getMaxRange();

	void addToScene();
	void removeFromScene();
	virtual void update(crMapArea *maparea);

	bool isInScene();
	CRCore::crVector3 getPosition();
protected:
	virtual ~crNPC(){}
	int m_npcid;
	std::string m_name;
	std::string m_scriptFileName;
	CRCore::ref_ptr<CRCore::crEventGroup> m_npcBody;
    CRCore::ref_ptr<CRCore::crEventGroup> m_npcRadar;
	CRCore::ref_ptr<CRCore::crGroup> m_npcParent;
	CRCore::ref_ptr<CRCore::crAITask> m_reviveTask;
	float m_maxRange;
};

class CRENCAPSULATION_EXPORT crNPCTroop : public CRCore::Referenced
{//
public :
	crNPCTroop();
	typedef std::vector< CRCore::ref_ptr<crNPC> > NPCVec;
	void addNPC(crNPC *npc);
	inline crNPC *getNPC(int i){ return i<m_npcVec.size()?m_npcVec[i].get():NULL; }
	void revive(int i = -1);//i表示诞生点索引，-1表示随机诞生点
	bool isAllDead();

	virtual void update(crMapArea *maparea);
protected:
	virtual ~crNPCTroop(){}
	NPCVec m_npcVec;
};

class CRENCAPSULATION_EXPORT crMapArea : public CRCore::Referenced
{//地图区域
public :
	crMapArea(){}
	void setName(const std::string &name);
	const std::string &getName();
	void setMapArea(const CRCore::crVector3 &area);
	bool isInMapArea(const CRCore::crVector3 &coord);
	typedef std::vector< CRCore::ref_ptr<crNPCTroop> > NPCTroopVec;//该区域可能出现的NPC组，组ID，NPCID
	void addNPCTroop(crNPCTroop *npcTroop);
	int getNumTroop();
    crNPCTroop *getTroop(int i);

	virtual void update();
protected:
	virtual ~crMapArea(){}
	std::string m_name;
	NPCTroopVec m_npcTroopVec;
	CRCore::crVector3 m_area;//NPC不可走出该范围,(x,y,radius)
};

class CRENCAPSULATION_EXPORT crSceneMap : public CRCore::Referenced
{
public :
	crSceneMap();
	void setName(const std::string &name);
	const std::string &getName();

	typedef std::vector< CRCore::ref_ptr<crMapArea> > MapAreaVec;
	void addMapArea(crMapArea *mapArea);
	inline crMapArea *getMapArea(int i){ return i<m_mapAreaVec.size()?m_mapAreaVec[i].get():NULL; }

	virtual void update();
protected:
	virtual ~crSceneMap(){}
	std::string m_name;
	MapAreaVec m_mapAreaVec;
};

class CRENCAPSULATION_EXPORT crScene : public CRCore::Referenced
{
public :
	crScene();
	void setSceneNameID(const std::string &name);
	const std::string& getSceneNameID(); 
	void setSceneFileName(const std::string &fileName);
	const std::string& getSceneFileName(); 
	typedef std::vector< CRCore::ref_ptr<crSceneMap> > SceneMapVec;
	inline crSceneMap *getSceneMap(int i){ return i<m_sceneMapVec.size()?m_sceneMapVec[i].get():NULL; }
	void addSceneMap(crSceneMap *scenemap);

	virtual void update();
protected:
	virtual ~crScene(){}
	std::string m_sceneNameID;
	SceneMapVec m_sceneMapVec;
	std::string m_fileName;
};

class CRENCAPSULATION_EXPORT crSceneManager : public CRCore::Referenced
{
public :
	crSceneManager();
	static crSceneManager *getInstance();
	void setCurrentScene( crScene *scene );
	inline crScene *getCurrentScene(){ return m_currentScene.get(); }

	typedef std::vector< CRCore::ref_ptr<crSave> > SaveVec;
	void pushSave(crSave *save);
	crSave *getSave(int index);
	SaveVec &getSaveVec();
	int getNumSave();
    void deleteSave(int index);
    
	bool setCurrentSaveIndex(int index);
	int getCurrentSaveIndex();
	
	int getCurrentSceneIndex();
	void setCurrentSceneName(const std::string &sceneName);
	std::string& getCurrentSceneName();
	void changeSceneID(/*const std::string &origSceneID, */const std::string &newSceneID);
	virtual void clear();

	void setPlayer(CRPhysics::crViewMatterObject *player);
	CRPhysics::crViewMatterObject *getPlayer();

	//virtual void update();

	//virtual void run();
	//void done();
	void setStaticRoot(CRCore::crGroup *staticRoot);
	CRCore::crGroup *getStaticRoot();
	void setDynamicRoot(CRCore::crGroup *dynamicRoot);
	CRCore::crGroup *getDynamicRoot();
	void setBotRoot(CRCore::crGroup *botRoot);
	CRCore::crGroup *getBotRoot();
	void setEditorRoot(CRCore::crGroup *editorRoot);
	CRCore::crGroup *getEditorRoot();
protected :
    virtual ~crSceneManager();
	static CRCore::ref_ptr<crSceneManager> m_instance;
	CRCore::ref_ptr<crScene> m_currentScene;
	SaveVec m_saveVec;
	int m_currentSaveIndex;

	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_player;

	//bool m_done;
	std::string m_currentSceneName;

	CRCore::ref_ptr<CRCore::crGroup> m_staticRoot;
	CRCore::ref_ptr<CRCore::crGroup> m_dynamicRoot;
	CRCore::ref_ptr<CRCore::crGroup> m_botRoot;//
	CRCore::ref_ptr<CRCore::crGroup> m_editorRoot;//
};

//class CRENCAPSULATION_EXPORT crSceneManagerUpdater : public CRUtil::crSceneView::SceneManagerUpdater
//{
//	inline virtual void operator()(void){ crSceneManager::getInstance()->update(); }
//};

}

#endif
//typedef std::vector<std::string> SceneIDVec;
//void pushSceneID(const std::string &sceneid);
//std::string getSceneID(int index);
//std::string getNextSceneID(const std::string &sceneid);
//SceneIDVec &getSceneIDVec();

////bool setCurrentSceneIndex(int index);

//typedef std::map< std::string, std::string > SceneMap;
//void insertSceneMap( const std::string &sceneid, const std::string &sceneFileName ); 
//std::string getSceneFileName(const std::string &sceneid);
//SceneMap &getSceneMap();
//SceneIDVec m_sceneIDVec;
//SceneMap m_sceneMap;
//int m_currentSceneIndex;