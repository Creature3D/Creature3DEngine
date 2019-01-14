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
#ifndef CRENCAPSULATION_CRLOADER_H
#define CRENCAPSULATION_CRLOADER_H 1

#include <CREncapsulation/crExport.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crDataManager.h>
#include <CRProducer/crViewer.h>
#include <string>
//以下函数需要考虑多线程使用时的问题
namespace rcfg{
	class ConfigScript;
}
namespace CREncapsulation{

	extern CRENCAPSULATION_EXPORT void loadCfgMaterial(const std::string &cfgFile, CRCore::crStateSet *nodestateset);
    extern CRENCAPSULATION_EXPORT void loadCfgFile(const std::string &cfgFile, CRCore::crNode *node);

	//extern CRENCAPSULATION_EXPORT void loadAI(const std::string& filename, CRPhysics::crMatterGroup& node);
	//extern CRENCAPSULATION_EXPORT void cfgloading();
	//extern CRENCAPSULATION_EXPORT void loadSounds(CRCore::crGroup *group);

	extern CRENCAPSULATION_EXPORT void loadStageCfg(const std::string &filename = std::string("script/stage.cfg"));

	extern CRENCAPSULATION_EXPORT void loadGameStageCfg(const std::string &path = std::string(""));

	extern CRENCAPSULATION_EXPORT void loadInitCfg(bool wantScreanSize = false,const std::string &path = std::string(""));
	//extern CRENCAPSULATION_EXPORT void reLoadDisplaySettingCfg();

	//extern CRENCAPSULATION_EXPORT void loadSoundCfg();
	//extern CRENCAPSULATION_EXPORT void loadLightCfg();
	extern CRENCAPSULATION_EXPORT void loadRenderToTextureCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadFilterCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadUIFile(const std::string &uifile);
	extern CRENCAPSULATION_EXPORT void addUINodeToScene(CRCore::crGroup *uiroot,bool async = true);
	extern CRENCAPSULATION_EXPORT void loadEngineUI();
	extern CRENCAPSULATION_EXPORT void loadLoadingUI();
	//extern CRENCAPSULATION_EXPORT void loadSceneNodeEventCfg();
	//extern CRENCAPSULATION_EXPORT void loadNodeEventCfg(CRCore::crNode *node);
	//extern CRENCAPSULATION_EXPORT void loadObjectLightCfg(CRCore::crObject *object);
	extern CRENCAPSULATION_EXPORT void loadEffectSequenceManagerCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadWeaponManagerCfg(const std::string &path = std::string(""));
	//extern CRENCAPSULATION_EXPORT void loadEffectParticleManagerCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadParticle(const std::string &fileName,CRCore::crGroup *group = NULL, float loddist = 0.0f);
	//extern CRENCAPSULATION_EXPORT CRPhysics::crViewMatterObject* loadBotCfg(const char* sceneName = NULL, CRCore::crGroup *botParent = NULL,const char* fileName = NULL, const char* botName = NULL, bool isNetBot = false, int netTeam = 0);
	//extern CRENCAPSULATION_EXPORT void loadBotIDCfg(const char* fileName);

	extern CRENCAPSULATION_EXPORT void loadCursorCfg(CRProducer::crViewer *viewer,const std::string &path = std::string(""));

	//extern CRENCAPSULATION_EXPORT void loadPagedLodCfg(CRCore::crPagedLOD *pagedLod, rcfg::ConfigScript& cfg_script);

	//extern CRENCAPSULATION_EXPORT void loadImageStreamManagerCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadLoadManagerCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT void loadAITaskManagerCfg(const std::string &path = std::string(""));

	extern CRENCAPSULATION_EXPORT void loadSaveCfg(const std::string &path = std::string(""));
	extern CRENCAPSULATION_EXPORT bool wantToLoadSave(int index);
	extern CRENCAPSULATION_EXPORT bool loadSave(crSave *save);
	//extern CRENCAPSULATION_EXPORT void writeSaveCfg(int index);
	extern CRENCAPSULATION_EXPORT void writeSaveCfg(const std::string &path = std::string(""));
	//extern CRENCAPSULATION_EXPORT void deleteSaveCfg(int index);

	extern CRENCAPSULATION_EXPORT void registerEventCallback();
	//extern CRENCAPSULATION_EXPORT void registerDefUIEventCallback();
	extern CRENCAPSULATION_EXPORT void registerDataClass();
	extern CRENCAPSULATION_EXPORT void registerHandleClass();

	extern CRENCAPSULATION_EXPORT bool checkSN();
	extern CRENCAPSULATION_EXPORT bool incorrectRegistrationCode(std::string &code);////返回true表示成功否则失败
	extern CRENCAPSULATION_EXPORT bool registrationCodeInput(std::string computerName, std::string userName);

	extern CRENCAPSULATION_EXPORT void createStatsText();
	extern CRENCAPSULATION_EXPORT void createLogoText();

	extern CRENCAPSULATION_EXPORT _crInt32 parseEventMsg( const std::string &str );
	extern CRENCAPSULATION_EXPORT _crInt16 parseEventMsg16( const std::string &str );
	
    extern CRENCAPSULATION_EXPORT void loadEventScript(const std::string &scriptFile, CRCore::crNode *node);
	extern CRENCAPSULATION_EXPORT CRCore::crData *loadData(const std::string &scriptFile);
	extern CRENCAPSULATION_EXPORT CRCore::crData *loadData(rcfg::ConfigScript& cfg_script);
	extern CRENCAPSULATION_EXPORT void loadIncludeData(const std::string &scriptFile,CRCore::crData *data);
	extern CRENCAPSULATION_EXPORT void loadExternData(rcfg::ConfigScript& cfg_script,CRCore::crData *data);
	extern CRENCAPSULATION_EXPORT void loadDrivers(rcfg::ConfigScript& cfg_script,CRCore::crData *data);
	extern CRENCAPSULATION_EXPORT void loadHandles(rcfg::ConfigScript& cfg_script,CRCore::crHandle *data);

	class CRENCAPSULATION_EXPORT crNeedInitLoadPageVisitor : public CRCore::crNodeVisitor
	{
	public:
		crNeedInitLoadPageVisitor();
		virtual ~crNeedInitLoadPageVisitor();
		virtual void apply(CRCore::crPagedLOD& node);
	};

	extern CRENCAPSULATION_EXPORT CRCore::crMatrixTransform *createCoordinate();
	extern CRENCAPSULATION_EXPORT CRCore::crObject *createGrid(int rows = 100, float interval = 1.0f, float zoffset = 0.0f);

	extern CRENCAPSULATION_EXPORT void loadSceneManagerCfg(crSave *save,bool loadDetail);
	extern CRENCAPSULATION_EXPORT void NPCQueryFromCfg(int npcid,crNPC *npc);
	extern CRENCAPSULATION_EXPORT void loadNPCCfg(const std::string &npcName, const std::string &scriptFile, crNPC *npc, const std::string &botFile = std::string(""));

	extern CRENCAPSULATION_EXPORT void loadSkillCfg(const std::string &scriptFile, CRCore::crGroup *loadedNode);//loadItemChildCfg
	extern CRENCAPSULATION_EXPORT void loadItemCfg(const std::string &scriptFile, CRCore::crGroup *loadedNode/*,bool staticRoot*/);
	extern CRENCAPSULATION_EXPORT void loadCameraManagerCfg(const std::string &path = std::string(""));
}

#endif