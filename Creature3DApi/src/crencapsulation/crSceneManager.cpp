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
#include <CREncapsulation/crSceneManager.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
//#include <CRPhysics/crBotManager.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crRegistry.h>
using namespace CRCore;
using namespace CREncapsulation;
using namespace CRPhysics;
/////////////////////////////////////////
//
//crSave
//
////////////////////////////////////////////
crSave::crSave():
m_sceneid(0),
m_name("DefaultName"),
m_botName("DefaultName"),
m_hp(0),
m_defend(0),
m_team(1),
m_experience(0),
m_progress(0),
m_gameState(SingleInit)
//m_teamMode(RedTeam)
{
	m_weapons[0] = "NULL"; 
	m_weapons[1] = "NULL";
	m_weapons[2] = "NULL";
}

void crSave::setName(const std::string& name) 
{ m_name = name; }

const std::string& crSave::getName()const 
{ return m_name; }

void crSave::setBotName(const std::string& name) 
{ m_botName = name; }

const std::string& crSave::getBotName()const 
{ return m_botName; }

std::string crSave::getBotFile(bool isNet)const 
{ return isNet ? "script/netbot/" + m_botName + ".cfg" : "script/bot/" + m_botName + ".cfg"; }

void crSave::setHp(int hp)
{
    m_hp = hp;
}

int crSave::getHp()const
{
    return m_hp;
}

void crSave::setDefend(int defend)
{
    m_defend = defend;
}

int crSave::getDefend()const
{
    return m_defend;
}

void crSave::setTeam(int team)
{
    m_team = team;
}

int crSave::getTeam()const
{
    return m_team;
}

//void crSave::setTeamMode(unsigned int teamMode)
//{
//    m_teamMode = (TeamMode)teamMode;
//}
//
//crSave::TeamMode crSave::getTeamMode()const
//{
//    return m_teamMode;
//}

void crSave::setExperience(int experience)
{
    m_experience = experience;
}

int crSave::getExperience()const
{
    return m_experience;
}

void crSave::setOrientation(const crQuat &orientation)
{
    m_orientation = orientation;
}

const crQuat& crSave::getOrientation()const
{
    return m_orientation;
}

void crSave::setPosition(const crVector3& position)
{
    m_position = position;
}

const crVector3& crSave::getPosition()const
{
    return m_position;
}

//void crSave::clearKilledBotNameVec()
//{
//    m_killedBotNameSet.clear();
//}
//
//void crSave::insertKilledBotName(const std::string &name)
//{
//    m_killedBotNameSet.insert(name);
//}
//
//bool crSave::isBotKilled(const std::string &name)
//{
//    return m_killedBotNameSet.find(name) != m_killedBotNameSet.end();
//}

void crSave::setProgress(int progress)
{
    m_progress = progress;
}

void crSave::setWeapon(int index, const std::string &weaponName)
{
	if(index>=0 && index<3)
        m_weapons[index] = weaponName;
}

const std::string &crSave::getWeapon1() const
{
    return m_weapons[0];
}

const std::string &crSave::getWeapon2() const
{
    return m_weapons[1];
}

const std::string &crSave::getWeapon3() const
{
    return m_weapons[2];
}

void crSave::setGameState(crSave::GameState gameState)
{
    m_gameState = gameState;
}

crSave::GameState crSave::getGameState()
{
    return m_gameState;
}

void crSave::addParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		setName(str);
		break;
	case 1:
        setBotName(str);
		break;
	case 2:
		m_sceneid = atoi(str.c_str());
		break;
	case 3:
        m_saveTime = str;
		break;
	case 4:
		m_hp = atoi(str.c_str());
		break;
	case 5:
		m_defend = atoi(str.c_str());
		break;
	case 6:
		m_team = atoi(str.c_str());
		break;
	case 7:
		m_experience = atoi(str.c_str());
		break;
	case 8:
		{
			crVector4 orientation;
            CRCore::crArgumentParser::appAtoVec(str,orientation);
			m_orientation.set(orientation);
		}
		break;
	case 9:
		CRCore::crArgumentParser::appAtoVec(str,m_position);
	case 10:
		m_weapons[0] = str;
		break;
	case 11:
		m_weapons[1] = str;
		break;
	case 12:
		m_weapons[2] = str;
		break;
	case 13:
		m_progress = atoi(str.c_str());
		break;
	case 14:
		m_gameState = (GameState)(atoi(str.c_str()));
		break;
	//default:
	//	insertKilledBotName(str);
	}
}
/////////////////////////////////////////
//
//crNPC
//
/////////////////////////////////////////
crNPC::crNPC():
m_npcid(0),
m_maxRange(200)
{
}

crNPC::crNPC(const crNPC& npc):
m_npcid(npc.m_npcid),
m_name(npc.m_name),
m_scriptFileName(npc.m_scriptFileName),
m_npcParent(npc.m_npcParent),
m_reviveTask(npc.m_reviveTask),
m_maxRange(npc.m_maxRange)
{//不能克隆
	//m_npcBody = npc.m_npcBody.get();
 //   m_npcRadar = npc.m_npcRadar.get();
}

void crNPC::setNPCID(int npcid)
{
	m_npcid = npcid;
}

int crNPC::getNPCID()
{
	return m_npcid;
}

void crNPC::setName(const std::string &name)
{
    m_name = name;
}

const std::string &crNPC::getName()
{
    return m_name;
}

void crNPC::setScriptFileName(const std::string &scriptFileName)
{
    m_scriptFileName = scriptFileName;
}

const std::string &crNPC::getScriptFileName()
{
	return m_scriptFileName;
}

void crNPC::setReviveTaskByName(const std::string &taskName)
{
	m_reviveTask = crAITaskManager::getInstance()->getAITask(taskName);
	//if(!m_reviveTask)
	//	CRCore::notify(CRCore::INFO)<<"crNPC::setReviveTaskByName(): not find aitask "<<taskName<<std::endl;
}

void crNPC::setReviveTask(CRCore::crAITask *aiTask)
{
	m_reviveTask = aiTask;
}

void crNPC::revive(int i)
{//i=-1表示随机诞生点
	crViewMatterObject *bot;
	bot = dynamic_cast<crViewMatterObject *>(getNPCBody()->getChild(0));
	if(bot && bot->isDead() /*&& !m_npcParent->containsNode(m_npcBody.get())*/)
	{
		if(m_reviveTask.valid())
		{
			crControlPoint *cp = i == -1?m_reviveTask->getRandomTaskPoint():m_reviveTask->getTaskPoint(i);
			if(cp)
			{
				const crVector3 &orientation = cp->getOrientation();
				CRCore::crQuat q1(CRCore::DegreesToRadians(orientation[0]), CRCore::crVector3(1, 0, 0)), 
					q2(CRCore::DegreesToRadians(orientation[1]), CRCore::crVector3(0, 1, 0)), 
					q3(CRCore::DegreesToRadians(orientation[2]), CRCore::crVector3(0, 0, 1));
				CRCore::crMatrix mrot;
				mrot.set(q1*q2*q3);
				mrot.setTrans(cp->getPosition());
				bot->setMatterObjectMatrix(mrot,CRPhysics::crMatterObject::MD_FullMatrix);
				bot->setVelocity(cp->getVelocity());
			}
		}
		//if(m_npcRadar.valid()) CRIOManager::crLoadManager::getInstance()->requestAddNode(m_npcParent.get(),m_npcRadar.get(),true);
		//CRIOManager::crLoadManager::getInstance()->requestAddNode(m_npcParent.get(),m_npcBody.get(),true);
		bot->renascence();
	}
}

bool crNPC::isDead()
{
	crViewMatterObject *bot;
	bot = dynamic_cast<crViewMatterObject *>(getNPCBody()->getChild(0));
	if(bot && !bot->isDead()) return false;
	return true;
}

void crNPC::setNPCBody(crEventGroup *body)
{
	m_npcBody = body;
}

crEventGroup *crNPC::getNPCBody()
{
	return m_npcBody.get();
}

void crNPC::setNPCRadar(CRCore::crEventGroup *radar)
{
	m_npcRadar = radar;
}

CRCore::crEventGroup *crNPC::getNPCRadar()
{
	return m_npcRadar.get();
}

void crNPC::setNPCParent(CRCore::crGroup *parent)
{
	m_npcParent = parent;
}

CRCore::crGroup *crNPC::getNPCParent()
{
    return m_npcParent.get();
}

CRPhysics::crViewMatterObject *crNPC::getNPCBot()
{
	return m_npcBody.valid()?dynamic_cast<crViewMatterObject *>(m_npcBody->getChild(0)):NULL;
}

void crNPC::setMaxRange(float range)
{
    m_maxRange = range;
}

float crNPC::getMaxRange()
{
    return m_maxRange;
}

void crNPC::update(crMapArea *maparea)
{
    if(m_npcParent.valid() && m_npcBody.valid() && m_maxRange>0)
	{//m_maxRange<=0表示不进行距离检测
        crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_npcBody->getChild(0));
		if(bot)
		{
			bool addtoscene = true;
			if(!bot->isDead())
			{
				CRCore::crVector3 eyePoint = CRCore::crBrain::getInstance()->getCameraPosition();
				crVector3 botPosition = bot->getTrans();
				float distance = (botPosition - eyePoint).length();
				if(distance<m_maxRange)
				{
					addtoscene = true;
					if(!maparea->isInMapArea(botPosition))
					{//NPC走出了巡逻范围
                        bot->doEvent(WCH_OUTMAPAREA);
					}
				}
				else
				{
					addtoscene = false;
				}
			}
			else if(bot->getDeathInterval()>5.0)
			{//死后超过5s
                addtoscene = false;
			}
			if(addtoscene)
			{
                addToScene();
			}
			else
			{
                removeFromScene();
			}
		}
	}
}
void crNPC::addToScene()
{
	if(m_npcParent.valid() && m_npcBody.valid() && !m_npcParent->containsNode(m_npcBody.get()))
	{
		crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_npcBody->getChild(0));
		if(bot)
		{
			m_npcBody->setEnableIntersect(true);
			bot->setEnableIntersect(true);
			bool needCompile = !bot->getPhysicsInited();
			CRIOManager::crLoadManager::getInstance()->requestAddNode(m_npcParent.get(),m_npcBody.get(),needCompile);
			if(m_npcRadar.valid())
			{
			    CRIOManager::crLoadManager::getInstance()->requestAddNode(m_npcParent.get(),m_npcRadar.get(),needCompile);
			}
			//CRIOManager::crLoadManager::getInstance()->requestAddNode(CRCore::crBrain::getInstance()->getAITreeHead(),bot,false);
			//crBotManager::getInstance()->renascenceBot(bot);
		}
	}
}
void crNPC::removeFromScene()
{
    if(m_npcParent.valid() && m_npcBody.valid()/* && m_npcParent->containsNode(m_npcBody.get())*/)
	{
		//crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_npcBody->getChild(0));
		//if(bot)
		//{
			//m_npcBody->setEnableIntersect(false);
			//bot->setEnableIntersect(false);
			CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_npcParent.get(),m_npcBody.get());			
			if(m_npcRadar.valid())
			{
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_npcParent.get(),m_npcRadar.get());
			}
			//CRIOManager::crLoadManager::getInstance()->requestRemoveNode(CRCore::crBrain::getInstance()->getAITreeHead(),bot);
			//crBotManager::getInstance()->killBot(bot);
		//}
	}
}

bool crNPC::isInScene()
{
    return m_npcParent.valid() && m_npcBody.valid() && m_npcParent->containsNode(m_npcBody.get());
}
crVector3 crNPC::getPosition()
{
	crViewMatterObject *bot = getNPCBot();
	if(bot) return bot->getTrans();
	return crVector3(0,0,0);
}
/////////////////////////////////////////
//
//crNPCTroop
//
/////////////////////////////////////////
crNPCTroop::crNPCTroop()
{
}
void crNPCTroop::addNPC(crNPC *npc)
{
	if(npc) m_npcVec.push_back(npc);
}
void crNPCTroop::revive(int i)
{
	for( NPCVec::iterator itr = m_npcVec.begin();
		 itr != m_npcVec.end();
		 ++itr )
	{
        (*itr)->revive(i);
	}
}
bool crNPCTroop::isAllDead()
{
	for( NPCVec::iterator itr = m_npcVec.begin();
		itr != m_npcVec.end();
		++itr )
	{
		if(!(*itr)->isDead())
		{
			return false;
		}
	}
	return true;
}
void crNPCTroop::update(crMapArea *maparea)
{
	for( NPCVec::iterator itr = m_npcVec.begin();
		itr != m_npcVec.end();
		++itr )
	{
		(*itr)->update(maparea);	
	}
}
/////////////////////////////////////////
//
//crMapArea
//
/////////////////////////////////////////
void crMapArea::setName(const std::string &name)
{
	m_name = name;
}
const std::string &crMapArea::getName()
{
	return m_name;
}
void crMapArea::setMapArea(const CRCore::crVector3 &area)
{
	m_area = area;
}
bool crMapArea::isInMapArea(const CRCore::crVector3 &coord)
{
	crVector3 center(m_area[0],m_area[1],coord[2]);
	return (coord-center).length()<m_area[2];
}
void crMapArea::addNPCTroop(crNPCTroop *npcTroop)
{
	if(npcTroop) m_npcTroopVec.push_back(npcTroop);
}
int crMapArea::getNumTroop()
{
    return m_npcTroopVec.size();
}
crNPCTroop *crMapArea::getTroop(int i)
{
     return i<m_npcTroopVec.size()?m_npcTroopVec[i].get():NULL; 
}
void crMapArea::update()
{
	crViewMatterObject *firstPlayer = crSceneManager::getInstance()->getPlayer();
	if(firstPlayer)
	{
		if(isInMapArea(firstPlayer->getTrans()))
		{
			firstPlayer->doEvent(WCH_INMAPAREA,MAKEINT64(&m_name,NULL));
		}
		else
		{
			firstPlayer->doEvent(WCH_OUTMAPAREA,MAKEINT64(&m_name,NULL));
		}

	}
	for( NPCTroopVec::iterator itr = m_npcTroopVec.begin();
		itr != m_npcTroopVec.end();
		++itr )
	{
		(*itr)->update(this);
	}
}
/////////////////////////////////////////
//
//crSceneMap
//
/////////////////////////////////////////
crSceneMap::crSceneMap()
{
}
void crSceneMap::setName(const std::string &name)
{
	m_name = name;
}
const std::string &crSceneMap::getName()
{
	return m_name;
}
void crSceneMap::addMapArea(crMapArea *mapArea)
{
	if(mapArea)
		m_mapAreaVec.push_back(mapArea);
}
void crSceneMap::update()
{
	for( MapAreaVec::iterator itr = m_mapAreaVec.begin();
		itr != m_mapAreaVec.end();
		++itr )
	{
		(*itr)->update();
	}
}
/////////////////////////////////////////
//
//crScene
//
////////////////////////////////////////////
crScene::crScene()
{
}
void crScene::setSceneNameID(const std::string &name)
{
	m_sceneNameID = name;
}
const std::string& crScene::getSceneNameID()
{
	return m_sceneNameID;
}
void crScene::setSceneFileName(const std::string &fileName)
{
	m_fileName = fileName;
}
const std::string& crScene::getSceneFileName()
{
	return m_fileName;
}
void crScene::addSceneMap(crSceneMap *scenemap)
{
	m_sceneMapVec.push_back(scenemap);
}

void crScene::update()
{
	for( SceneMapVec::iterator itr = m_sceneMapVec.begin();
		 itr != m_sceneMapVec.end();
		 ++itr )
	{
		(*itr)->update();
	}
}
/////////////////////////////////////////
//
//crSceneManager
//
////////////////////////////////////////////
ref_ptr<crSceneManager> crSceneManager::m_instance = NULL;

//////////////////////////////////////////////////////////////////
crSceneManager::crSceneManager():
m_currentSaveIndex(0)
//m_done(true)
{
	//setSchedulePriority(THREAD_PRIORITY_LOW);
}
crSceneManager::~crSceneManager() 
{
	//m_done = true;
	//while (isRunning()) 
	//{
	//	CRCore::crThread::sleep(10);
	//}
}

crSceneManager* crSceneManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crSceneManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crSceneManager::clear()
{
	m_instance = NULL;
	//m_currentScene = NULL;
	//m_player = NULL;

	//m_done = true;
}

void crSceneManager::setCurrentScene( crScene *scene )
{
    m_currentScene = scene;
	setCurrentSceneName(m_currentScene->getSceneNameID());
}
void crSceneManager::setCurrentSceneName(const std::string &sceneName)
{
    m_currentSceneName = sceneName;
}
std::string& crSceneManager::getCurrentSceneName()
{
    return m_currentSceneName;
}
//void crSceneManager::pushSceneID(const std::string &sceneid)
//{
//    m_sceneIDVec.push_back(sceneid);
//}
//
//std::string crSceneManager::getSceneID(int index)
//{
//	if(index<0 || index >= m_sceneIDVec.size()) return "0";
//    return m_sceneIDVec[index];
//}
//
//std::string crSceneManager::getNextSceneID(const std::string &sceneid)
//{
//	int id = 0;
//	for( SceneIDVec::const_iterator itr = m_sceneIDVec.begin();
//		 itr != m_sceneIDVec.end();
//		 ++itr)
//	{
//		if(itr->compare(sceneid) == 0)
//		{
//			break;
//		}
//		++id;
//	}
//    return getSceneID(id+1);
//}
//
//crSceneManager::SceneIDVec &crSceneManager::getSceneIDVec()
//{
//    return m_sceneIDVec;
//}
//
//void crSceneManager::insertSceneMap( const std::string &sceneid, const std::string &sceneFileName )
//{
//	m_sceneMap.insert(std::make_pair(sceneid,sceneFileName));
//}
//
//std::string crSceneManager::getSceneFileName(const std::string &sceneid)
//{
//	SceneMap::iterator itr = m_sceneMap.find(sceneid);
//	return itr != m_sceneMap.end()? itr->second:"0";
//}
//
//crSceneManager::SceneMap &crSceneManager::getSceneMap()
//{
//	return m_sceneMap;
//}

void crSceneManager::pushSave(crSave *save)
{
    m_saveVec.push_back(save);
}

int crSceneManager::getNumSave()
{
    return m_saveVec.size();
}

crSave *crSceneManager::getSave(int index)
{
	if(index<0 || index >= m_saveVec.size()) return NULL;
	return m_saveVec[index].get();
}

crSceneManager::SaveVec &crSceneManager::getSaveVec()
{
    return m_saveVec;
}

void crSceneManager::deleteSave(int index)
{
	if(index>=0 || index < m_saveVec.size())
	{
		m_saveVec.erase(m_saveVec.begin()+index);
	}
}

bool crSceneManager::setCurrentSaveIndex(int index)
{
	if(index>=0 || index < m_saveVec.size())
	{
		m_currentSaveIndex = index;
		return true;
		//return setCurrentSceneIndex(m_saveVec[m_currentSaveIndex]->m_sceneid);
	}
	return false;
}

int crSceneManager::getCurrentSaveIndex()
{
    return m_currentSaveIndex;
}

//bool crSceneManager::setCurrentSceneIndex(int index)
//{
//	if(index>=0 || index < m_sceneIDVec.size())
//	{
//		m_currentSceneIndex = index;
//		return true;
//	}
//	return false;
//}
int crSceneManager::getCurrentSceneIndex()
{
	return m_saveVec.empty()?0:m_saveVec[m_currentSaveIndex]->getSceneID();
}
//std::string crSceneManager::getCurrentSceneID()
//{
//	return m_currentScene.valid()?m_currentScene->getSceneNameID():"";
//}

void crSceneManager::changeSceneID(/*const std::string &origSceneID, */const std::string &newSceneID)
{
	if(m_currentScene.valid())
		m_currentScene->setSceneNameID(newSceneID);
	//for( SceneIDVec::iterator itr = m_sceneIDVec.begin();
	//	 itr != m_sceneIDVec.end();
	//	 ++itr )
	//{
	//	if(origSceneID.compare(*itr) == 0)
	//	{
	//		*itr = newSceneID;
	//		break;
	//	}
	//}
	//SceneMap::iterator mitr = m_sceneMap.find(origSceneID);
	//if(mitr != m_sceneMap.end())
	//{
	//	m_sceneMap.insert(std::make_pair(newSceneID,mitr->second));
	//	m_sceneMap.erase(origSceneID);
	//}
}

void crSceneManager::setPlayer(CRPhysics::crViewMatterObject *player)
{
    m_player = player;
	//m_player->addToScene();
	//crBotManager::getInstance()->setFirstPlayer(dynamic_cast<crViewMatterObject *>(m_player->getNPCBody()->getChild(0)));
	//CRAI::crKeyboardMouseEventGroup* kmeg = CRAI::crKeyboardMouseEventGroup::getInstance();
	//kmeg->attachEventGroup(m_player->getNPCBody()->getAttachedEventGroup());
	//kmeg->setDefaultEventGroup(m_player->getNPCBody()->getAttachedEventGroup());
}

CRPhysics::crViewMatterObject *crSceneManager::getPlayer()
{
	return m_player.get();
}

void crSceneManager::setStaticRoot(CRCore::crGroup *sceneRoot)
{
	m_staticRoot = sceneRoot;
}
CRCore::crGroup *crSceneManager::getStaticRoot()
{
	return m_staticRoot.get();
}
void crSceneManager::setDynamicRoot(CRCore::crGroup *dynamicRoot)
{
	m_dynamicRoot = dynamicRoot;
}
CRCore::crGroup *crSceneManager::getDynamicRoot()
{
	return m_dynamicRoot.get();
}
void crSceneManager::setBotRoot(CRCore::crGroup *botRoot)
{
	m_botRoot = botRoot;
}
CRCore::crGroup *crSceneManager::getBotRoot()
{
	return m_botRoot.get();
}
void crSceneManager::setEditorRoot(CRCore::crGroup *editorRoot)
{
	m_editorRoot = editorRoot;
}
CRCore::crGroup *crSceneManager::getEditorRoot()
{
	return m_editorRoot.get();
}
//void crSceneManager::update()
//{
//	CRIOManager::crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
//    if(dp->getInited() && m_currentScene.valid())
//	{
//		m_currentScene->update();
//		//CRCore::notify(CRCore::ALWAYS)<<"crSceneManager update"<<std::endl;
//	}
//}

//void crSceneManager::done()
//{
//	m_done = true;
//}

//void crSceneManager::run()
//{
//#if _DEBUG
//	try
//	{
//#endif
//		m_done = false;
//		while(!m_done)
//		{
//			CRCore::crThread::sleep(100);
//			update();
//		}
//#if _DEBUG
//		CRCore::notify(CRCore::ALWAYS)<<"crSceneManager end"<<std::endl;
//	}
//	catch (...)
//	{
//		CRCore::notify(CRCore::ALWAYS)<<"crSceneManager::run() error "<<std::endl;
//	}
//#endif
//}