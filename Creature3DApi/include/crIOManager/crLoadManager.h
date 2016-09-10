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
#ifndef CRIOMANAGER_CRLOADMANAGER
#define CRIOMANAGER_CRLOADMANAGER 1

#include <CRIOManager/crExport.h>
#include <CRIOManager/crDatabasePager.h>
#include <CRCore/crGroup.h>
#include <CRCore/crTransform.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crVector3.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crViewMatterObject.h>
#include "rcfg/ConfigScriptOO.h"
#include <map>
#include <string>
namespace CRIOManager {

class CRIOMANAGER_EXPORT crCreBotModel : public CRCore::Referenced
{
public:
	crCreBotModel():m_mass(1.0f){}
	void request(CRCore::crGroup* parent,bool asyncLoad);
	typedef std::map< std::string, CRCore::ref_ptr<CRCore::crTransform> > Components;
	void setName(const std::string &id);
	const std::string &getName() const;
	void insertComponent(const std::string &fileName, CRCore::crTransform *dof);
	void setMass(float mass);
	float getMass();
protected:
	std::string m_name;
	Components m_components;
	float m_mass;
};

class CRIOMANAGER_EXPORT crCharacterBotModel : public CRCore::Referenced
{
public:
	crCharacterBotModel():m_mass(1.0f){}
	void request(const std::string& botName, CRCore::crGroup* parent,bool asyncLoad);
	void setMass(float mass);
	float getMass();
protected:
	//std::string &createTempCfg(const std::string& botName);
	//void deleteTempCfg(const std::string& fileName);
	std::string m_name;
	std::string m_coreName;
	bool m_enableTangentSpace;
	float m_mass;
};
class CRIOMANAGER_EXPORT crLoadManager : public CRCore::Referenced
{
public:
	crLoadManager();
	static crLoadManager *getInstance();
    
	typedef std::map< std::string, CRCore::ref_ptr<crCreBotModel> > CreBotModelMap;
	typedef std::map< std::string, CRCore::ref_ptr<crCharacterBotModel> > CharacterBotModelMap;
    
	void requestCreBot(const std::string& id,CRPhysics::crViewMatterObject* parent, bool asyncLoad);
	void requestCharacterBot(const std::string& id, const std::string& botName, CRPhysics::crViewMatterObject* parent,bool asyncLoad);
	void requestNode(const std::string& fileName,CRCore::crGroup* parent,bool asyncLoad,bool reload = false);
    
	void insertCreBotModel(crCreBotModel *model);
	void insertCharacterBotModel(crCharacterBotModel *model);

	void requestAddNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode,bool needCompile);
    void requestRemoveNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode);
	void requestCompile(CRCore::crNode* loadedNode);
	//void requestCompileStateSet(CRCore::crStateSet* ss);

	virtual void clear();
protected:
    virtual ~crLoadManager() {}
	static CRCore::ref_ptr<crLoadManager> m_instance;
	CreBotModelMap m_creBotMap;
	CharacterBotModelMap m_characterBotMap;
};

class CRIOMANAGER_EXPORT crScriptLoadManager : public CRCore::Referenced
{
public:
	crScriptLoadManager();
	static crScriptLoadManager *getInstance();
	bool loadConfigScript(rcfg::ConfigScript& cfg_script,const std::string &fileName,bool loadfromfile = false,const std::string& head = CreScriptParserHeader);//loadfromfile=false表示从缓存中读取
	void eraseLoadedConfig(const std::string &fileName);

	CRCore::Referenced* findLoadedRefeanced(const std::string &fileName);
	void insertLoadedRefeanced(const std::string &fileName,CRCore::Referenced* ref);
	void eraseLoadedRefeanced(const std::string &fileName);
	virtual void clear();
	typedef std::map<CRCore::ref_ptr<Referenced>,float> RemovedReferencedMap;
	void updateRemovedReferencedMap(float dt);
protected:
	virtual ~crScriptLoadManager();
	ItemMap *findLoadedItemMap(const std::string &fileName);
	void insertLoadedItemMap(const std::string &fileName,ItemMap *itemmap);
	void insertRemovedReferencedMap(Referenced *ref);
	static CRCore::ref_ptr<crScriptLoadManager> m_instance;
	typedef std::map<std::string, ItemMap*> LoadedItemMap;
	typedef std::map<std::string, CRCore::ref_ptr<Referenced> > LoadedReferencedMap;
	LoadedItemMap m_loadedItemMap;
	LoadedReferencedMap m_loadedReferencedMap;
	CRCore::crCriticalMutex m_loadedItemMapMutex;
	CRCore::crCriticalMutex m_loadedReferencedMapMutex;
	RemovedReferencedMap m_removedReferencedMap;
	CRCore::crCriticalMutex m_removedReferencedMapMutex;
};
}

#endif
