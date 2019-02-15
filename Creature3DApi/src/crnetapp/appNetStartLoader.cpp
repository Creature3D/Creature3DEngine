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
#include <rbody/Creature3D/CreBody.h>
#include <CRNetApp/appNetStartLoader.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crNotify.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRPhysics/crCamera.h>
//#include <CREncapsulation/crAILoader.h>
//#include <CRNetApp/appNetRoleManipulatorPPC.h>
#include <CRCore/crDataManager.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRUtil/crOptimizer.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRAL/crSoundRoot.h>
//#include <CRAL/crSoundManager.h>
#include <CREncapsulation/crNodeCallbacks.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CREncapsulation;
using namespace CRPhysics;
using namespace CRNet;
using namespace CRUtil;
using namespace rbody;
///////////////////////////////////////////////////////////////////////
//
//crNetStartLoader
//
//////////////////////////////////////////////////////////////////////
void crNetStartLoader::loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd)
{
	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	CRCore::Timer_t end_tick;

	CRCore::notify(CRCore::ALWAYS)<<"Loading..."<< std::endl;
	//CRCore::crNode* node = CRIOManager::readNodeFiles(arguments);
	//CRCore::notify(CRCore::ALWAYS)<<"Loading cfgloading..."<< std::endl;
	//CREncapsulation::cfgloading();

	//CREncapsulation::loadMaterialCfg();
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadShaderManagerCfg..."<< std::endl;
	//CREncapsulation::loadShaderManagerCfg();
	//end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to loadShaderManagerCfg = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	//CREncapsulation::loadLoadManagerCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading readObjectFile..."<< std::endl;
	const std::string &gameName = crMyPlayerData::getInstance()->getSelectedGame()->getGameName();
	const std::string &uifile = crMyPlayerData::getInstance()->getSceneInfoData()->getUIScene();
	crQuerySceneInfoData *sceneInfoData = crMyPlayerData::getInstance()->getSceneInfoData();
	if(!sceneInfoData)
	{
		CRCore::notify(CRCore::FATAL)<<"Loading Error 没有指定场景..."<< std::endl;
		return;
	}
	const std::string& sceneName = sceneInfoData->getSceneName();
	crSceneManager::getInstance()->setCurrentSceneName(sceneName);
	CRIOManager::readObjectFile(/*"scene.cre"*/sceneInfoData->getFileName());

	//std::string textTable = gameName + "/script/TextTable.cfg";
	//crMyPlayerData::getInstance()->loadTextMap(textTable);
	std::string brainScript = sceneInfoData->getBrainFile();
	if(brainScript.empty())
		brainScript = gameName + "/script/brain/" + sceneName + ".cfg";
	else
	{
		brainScript = crGlobalHandle::getInstance()->getScriptDir() + brainScript;
	}
	//std::string brainScript = gameName + "/script/brain/" + sceneName + ".cfg";
	ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	crBrain::getInstance()->setDataClass(brainData.get());

	CREncapsulation::loadCursorCfg(viewer,gameName);
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadImageStreamManagerCfg..."<< std::endl;
	//CREncapsulation::loadImageStreamManagerCfg(gameName);
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadFilterCfg..."<< std::endl;

	//加载braindata
	//ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	//crBrain::getInstance()->setDataClass(brainData.get());

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadWeaponManagerCfg..."<< std::endl;
	//CREncapsulation::loadWeaponManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadMoveTaskManagerCfg..."<< std::endl;
	//CREncapsulation::loadAITaskManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadEffectParticleManagerCfg..."<< std::endl;
	//CREncapsulation::loadEffectParticleManagerCfg(gameName);
	//CREncapsulation::loadEffectSequenceManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadSceneNodeEventCfg..."<< std::endl;
	//CREncapsulation::loadSceneNodeEventCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadLightCfg..."<< std::endl;
	//CREncapsulation::loadLightCfg();

	CRCore::notify(CRCore::ALWAYS)<<"Loading end..."<< std::endl;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;

	///InitRenderVec
	//searchByNameVisitor.reset();
	//searchByNameVisitor.setNameId("InitRenderVec");
	//searchByNameVisitor.setSearchNodeType(OBJECT);
	//CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	//crObject *obj = dynamic_cast<crObject*>(searchByNameVisitor.getResult());
	//if(obj)
	//{
	//	int vtxNumber = 0;
	//	CRCore::crArgumentParser::readKeyValue(obj->getDescriptions(),"VtxNumber",vtxNumber);
	//	if(vtxNumber > 0)
	//	{
	//		crGeometry *geom = obj->getDrawable(0)->asGeometry();
	//		Vec3Array *vertexArray = (Vec3Array *)(geom->getVertexArray());
	//           int vtxCount = 0;		
	//		for( Vec3Array::iterator itr = vertexArray->begin();
	//			vtxCount < vtxNumber && itr != vertexArray->end();
	//			++itr, vtxCount++ )
	//		{
	//			//CRCore::notify(CRCore::FATAL)<<"InitRenderVec: "<<*itr<<std::endl;
	//			crDisplaySettings::instance()->pushInitRenderVec(*itr);
	//		}
	//	}
	//}
	ref_ptr<crGroup> botRoot;
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Wucaihua");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	botRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!botRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到SceneRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setBotRoot(botRoot.get());
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Static");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	crGroup* staticRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!staticRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到StaticRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setStaticRoot(staticRoot);
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Dynamic");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	crGroup* dynamicRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!dynamicRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到DynamicRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setDynamicRoot(dynamicRoot);

	CRCore::ref_ptr<crNPC> npc;
	crRole *role;//= crMyPlayerData::getInstance()->getRole();
	crMyPlayerData::getInstance()->lockMyRoleNpcMap();
	crMyPlayerData::MyRoleNpcMap &myRoleNpcMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	crViewMatterObject *bot;
	crData *data;
	//crAcceptGIVisitor acceptGI(1);
	CreBodyNode *bodyNode;
	crCreBodyCullCallback *bodyCull;
	crCollectNodeBYClassNameVisitor crebodyVisitor;
	crVector3 halfPhysicsBox;
	crData *roleData;
	void *param;
	for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoleNpcMap.begin();
		itr != myRoleNpcMap.end();
		++itr )
	{
		role = itr->second.first.get();
		npc = new crNPC;
		//CREncapsulation::registerNetRoleManipulatorPPC(new crNetRoleManipulatorPPC);
		loadNPCCfg(role->getIName(),role->getAbstractItem()->getResScript(),npc.get(),role->getAbstractItem()->getFileName());
		bot = npc->getNPCBot();
		if(bot)
		{
			roleData = role->getDataClass();
			roleData->getParam(WCHDATA_HalfPhysicsBox,param);
			halfPhysicsBox = *(crVector3 *)param;
			bot->setPhysicsBox(halfPhysicsBox*2.0f);

			crebodyVisitor.reset();
			crebodyVisitor.insertClassNameId("CreBodyNode");
			bot->accept(crebodyVisitor);
			NodeArray &crebodyArray = crebodyVisitor.getResult();
			for( NodeArray::iterator bitr = crebodyArray.begin();
				bitr != crebodyArray.end();
				++bitr )
			{
				bodyNode = dynamic_cast<CreBodyNode *>(bitr->get());
				if(bodyNode)
				{
					bodyCull = dynamic_cast<crCreBodyCullCallback *>(bodyNode->getCullCallback("CreBodyCull"));
					if(bodyCull)
					{
						bodyCull->setFineFactor(0.0f);
					}
				}
			}
			//bot->accept(acceptGI);
			//int playerid = role->getPlayerID();
			bot->getDataClass()->inputParam(WCHDATA_Item,role);
			//crSceneManager::getInstance()->setPlayer(npc.get());
			role->doEvent(WCH_ItemCoordToNode,MAKECREPARAM(bot,crMatterObject::MD_LoadMatrix));
		}
		role->setNode(npc->getNPCBody());
		data = role->getDataClass();
		data->excHandle(MAKECREPARAM(/*WCH_CheckPlayerData*/WCH_CheckNetData,role));

		float rt = 0;
		role->doEvent(WCH_GetHP,MAKECREPARAM(&rt,NULL));
		data->inputParam(WCHDATA_RTHP,&rt);
		role->doEvent(WCH_GetMP,MAKECREPARAM(&rt,NULL));
		data->inputParam(WCHDATA_RTMP,&rt);
		unsigned char itemstate = IS_Dead;
		data->inputParam(WCHDATA_ItemState,&itemstate);
		//crMyPlayerData::getInstance()->setNPC(npc.get());
		itr->second.second = npc.get();
		bot->setVisiable(false);
		bot->setEnableIntersect(false);
	}

	crCamera *camera = new crCamera;
	camera->setName("MainCamera");
	crCameraManager::getInstance()->addCamera(camera);
	CREncapsulation::loadCameraManagerCfg(gameName);

	ref_ptr<crNode> node;
	//char *ptr;
	//if( (ptr = getenv( "CRE_RunMode" )) && strcmp(ptr,"Editor")!=0 )
	if(crDisplaySettings::instance()->getRunMode()>0)
	{
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId("RemovedGroup");
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		node = searchByNameVisitor.getResult();
		if(node.valid())
		{
			//CRCore::notify(CRCore::FATAL)<<"RemovedGroup: "<<node<<std::endl;
			node->getParent(0)->removeChild(node.get());
		}
		//searchByNameVisitor.reset();
		//searchByNameVisitor.setNameId("Static");
		//searchByNameVisitor.setSearchNodeType(GROUP);
		//CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		//crGroup *meshRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		//if(meshRoot)
		//{
		//	CRUtil::crOptimizer::crFreezeTransformsVisitor fztv;
		//	fztv.setIgnorePhysics(true);
		//	meshRoot->accept(fztv);
		//}
	}
	else
	{
		searchByNameVisitor.reset();
		searchByNameVisitor.setNameId("EditorRoot");
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		crGroup *editorRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
		if(editorRoot)
		{
			crSceneManager::getInstance()->setEditorRoot(editorRoot);
			editorRoot->addChild(createCoordinate());
			editorRoot->addChild(createGrid());
		}
	}

	CRCore::crDB *db = crBrain::getInstance()->getCurrentActiveDB();

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("EffectParticleManager");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		(dynamic_cast<crGroup *>(node->getParent(0)))->removeChild(node.get());
		//CRParticle::crEffectParticleManager *effectParticleManager = CRParticle::crEffectParticleManager::getInstance();
		//effectParticleManager->setName(node->getName());
		//effectParticleManager->setDescriptionsInString(node->getDescriptionsInString());
		//node->getParent(0)->replaceChild(node.get(),effectParticleManager);
		//CRParticle::crEffectParticleManager::getInstance()->setStateSet(node->getStateSet());
		////CRParticle::crEffectParticleManager::getInstance()->setStateSet(crShaderManager::getInstance()->getShaderStateSet("particle"));
	}

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("EffectSequenceManager");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		CRCore::crEffectSequenceManager *effectSequenceManager = CRCore::crEffectSequenceManager::getInstance();
		effectSequenceManager->setName(node->getName());
		effectSequenceManager->setDescriptionsInString(node->getDescriptionsInString());
		node->getParent(0)->replaceChild(node.get(),CRCore::crEffectSequenceManager::getInstance());
	}	
	else
	{
		CRCore::notify(CRCore::WARN)<<"not find EffectSequenceManager"<< std::endl;
	}

	db->addChild(crFilterRenderManager::getInstance());

	db->insertChild(0,crRenderToTextureManager::getInstance());

	//CRCore::ref_ptr<CRAL::crSoundRoot> sound_root = new CRAL::crSoundRoot;
	//db->insertChild(0,sound_root.get());

	CREncapsulation::crNeedInitLoadPageVisitor needInitLoadPageVisitor;
	db->accept(needInitLoadPageVisitor);

	db->addChild(CRPhysics::crVolumeNodeManager::getInstance());

	end_tick = CRCore::Timer::instance()->tick();
	//std::cout << "Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	//CRCore::notify(CRCore::ALWAYS)<<"Now Optimizing... "<< std::endl;
	//start_tick = CRCore::Timer::instance()->tick();
	//CRUtil::crOptimizer optimizer;
	//optimizer.optimize(db/*pSecotr.get()*/,CRUtil::crOptimizer::TEXTURE_SETTINGS/*|CRUtil::crOptimizer::CHECK_PHYSICS*/);

	////CRUtil::crSmoothingVisitor smoother;
	////db->accept(smoother);

	////   int sampleRatio = 1.0;
	////CRUtil::crSimplifier simplifier(sampleRatio);
	////root->accept(simplifier);

	//end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to Optimize = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	//crRunGameHandle::getInstance()->setLoadingProgress(1.0f);
	crStartHandler::getInstance()->endloading();
	CREncapsulation::loadRenderToTextureCfg(gameName);
	CREncapsulation::loadFilterCfg(gameName);
	CREncapsulation::loadUIFile(uifile);
	CREncapsulation::loadEngineUI();
	CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
		false,false, // client storage
		true,1.0, // anisotropic filtering
		NULL );
	tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
	//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
	crFilterRenderManager::getInstance()->accept(tv);
	viewer->setSceneData(db/*pSecotr.get()*/);

	//viewer->setViewEventNode(CRAI::crKeyboardMouseEventGroup::getInstance());
	//CRAI::crKeyboardMouseEventGroup::getInstance()->setBindViewer(viewer);
	role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role)
	{
		crSceneManager::getInstance()->setPlayer(dynamic_cast<crViewMatterObject *>(role->getRelNode()));
		camera->attachNode(crSceneManager::getInstance()->getPlayer());
	}
	camera->setKeyboardMouseMode(CRCore::KM_None);
	viewer->setViewCameraNode(camera);

	//CRAL::crSoundManager::instance()->loadSound("script/sound1.cfg",crSceneManager::getInstance()->getCurrentSceneName().c_str());

	CRCore::notify(CRCore::ALWAYS)<<"Create Render... "<< std::endl;
	start_tick = CRCore::Timer::instance()->tick();

	//for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoleNpcMap.begin();
	//	itr != myRoleNpcMap.end();
	//	++itr )
	//{
	//	role = itr->second.first.get();
	//	role->doEvent(WCH_CheckAndRackEquipOnMesh,NULL);
	//}

	viewer->realize(parentWnd,renderWnd);
	CRProducer::crKeyboardMouseHandle::getInstance()->setBindViewer(viewer);
	crGameClientUpdate::getInstance();

	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Create Render = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	CRCore::notify(CRCore::ALWAYS)<<"Startting..."<<std::endl;
	//bot->doEvent(MAKEINT64(WCH_AITASK,0),MAKEINT64(MAKEINT32(WCHAI_Begin,WCHAI_SetCPToBot/*|WCHAI_TakeoverControl*/),NULL));

	//crPathFindingManager::setPathFindingCount(2);
	crPathFindingUpdater::getInstance()->startThread();
	//crBrain::getInstance()->doEvent(WCH_SceneStart);
	//crMyPlayerData::getInstance()->iTransportCheck();

	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crRequestEmporiumPacket packet;
		netConductor->getNetManager()->sendPacket("all",packet);
	}
	crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
	//crGameClientUpdate::getInstance();
}
///////////////////////////////////////////////////////////////////////
//
//crNetStartLoader2
//
//////////////////////////////////////////////////////////////////////
void crNetStartLoader2::loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd)
{
	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	CRCore::Timer_t end_tick;

	CRCore::notify(CRCore::ALWAYS)<<"Loading..."<< std::endl;

	//CRCore::notify(CRCore::ALWAYS)<<"Loading readObjectFile..."<< std::endl;
	const std::string &gameName = crMyPlayerData::getInstance()->getSelectedGame()->getGameName();
	void *param;
	crData *data = crRunGameHandle::getInstance()->getDataClass();
	data->getParam(WCHDATA_SceneFile,param);
	std::string sceneFile = *(std::string *)param;
	data->getParam(WCHDATA_UIFile,param);
	std::string uifile = *(std::string *)param;
	data->getParam(WCHDATA_BrainScript,param);
	std::string brainScript = *(std::string *)param;
	CRIOManager::readObjectFile(sceneFile);
	ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	crBrain::getInstance()->setDataClass(brainData.get());

	CREncapsulation::loadCursorCfg(viewer,gameName);
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadImageStreamManagerCfg..."<< std::endl;
	//CREncapsulation::loadImageStreamManagerCfg(gameName);
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadFilterCfg..."<< std::endl;

	//ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	//crBrain::getInstance()->setDataClass(brainData.get());
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadWeaponManagerCfg..."<< std::endl;
	//CREncapsulation::loadWeaponManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadMoveTaskManagerCfg..."<< std::endl;
	//CREncapsulation::loadAITaskManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadEffectParticleManagerCfg..."<< std::endl;
	//CREncapsulation::loadEffectParticleManagerCfg(gameName);
	//CREncapsulation::loadEffectSequenceManagerCfg(gameName);

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadSceneNodeEventCfg..."<< std::endl;
	//CREncapsulation::loadSceneNodeEventCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadLightCfg..."<< std::endl;
	//CREncapsulation::loadLightCfg();

	CRCore::notify(CRCore::ALWAYS)<<"Loading end..."<< std::endl;
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;

	ref_ptr<crGroup> botRoot;
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Wucaihua");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	botRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!botRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到SceneRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setBotRoot(botRoot.get());
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Static");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	crGroup* staticRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!staticRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到StaticRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setStaticRoot(staticRoot);
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Dynamic");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	crGroup* dynamicRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!dynamicRoot)
	{
		CRCore::notify(CRCore::WARN)<<"没有找到DynamicRoot"<< std::endl;
		return;
	}
	crSceneManager::getInstance()->setDynamicRoot(dynamicRoot);

	if(crGlobalHandle::gData()->gShowDebugInfo())
	{
		crGroup *areaDisplay = crMyPlayerData::getInstance()->getScene()->getAreaDisplayNode();
		dynamicRoot->addChild(areaDisplay);
	}

	CRCore::ref_ptr<crNPC> npc;
	crRole *role;//= crMyPlayerData::getInstance()->getRole();
	crMyPlayerData::getInstance()->lockMyRoleNpcMap();
	crMyPlayerData::MyRoleNpcMap &myRoleNpcMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	crViewMatterObject *bot;
	//crAcceptGIVisitor acceptGI(1);
	CreBodyNode *bodyNode;
	crCreBodyCullCallback *bodyCull;
	crCollectNodeBYClassNameVisitor crebodyVisitor;
	crVector3 halfPhysicsBox;
	crData *roleData;
	for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoleNpcMap.begin();
		itr != myRoleNpcMap.end();
		++itr )
	{
		role = itr->second.first.get();
		npc = new crNPC;
		//CREncapsulation::registerNetRoleManipulatorPPC(new crNetRoleManipulatorPPC);
		loadNPCCfg(role->getIName(),role->getAbstractItem()->getResScript(),npc.get(),role->getAbstractItem()->getFileName());
		bot = npc->getNPCBot();
		if(bot)
		{
			roleData = role->getDataClass();
			roleData->getParam(WCHDATA_HalfPhysicsBox,param);
			halfPhysicsBox = *(crVector3 *)param;
			bot->setPhysicsBox(halfPhysicsBox*2.0f);

			crebodyVisitor.reset();
			crebodyVisitor.insertClassNameId("CreBodyNode");
			bot->accept(crebodyVisitor);
			NodeArray &crebodyArray = crebodyVisitor.getResult();
			for( NodeArray::iterator bitr = crebodyArray.begin();
				 bitr != crebodyArray.end();
				 ++bitr )
			{
				bodyNode = dynamic_cast<CreBodyNode *>(bitr->get());
				if(bodyNode)
				{
					bodyCull = dynamic_cast<crCreBodyCullCallback *>(bodyNode->getCullCallback("CreBodyCull"));
					if(bodyCull)
					{
						bodyCull->setFineFactor(0.0f);
					}
				}
			}
			//bot->accept(acceptGI);
			//int playerid = role->getPlayerID();
			bot->getDataClass()->inputParam(WCHDATA_Item,role);
			//crSceneManager::getInstance()->setPlayer(npc.get());
			role->doEvent(WCH_ItemCoordToNode,MAKECREPARAM(bot,crMatterObject::MD_LoadMatrix));
		}
		role->setNode(npc->getNPCBody());
		data = role->getDataClass();
		data->excHandle(MAKECREPARAM(/*WCH_CheckPlayerData*/WCH_CheckNetData,role));

		float rt = 0;
		role->doEvent(WCH_GetHP,MAKECREPARAM(&rt,NULL));
		data->inputParam(WCHDATA_RTHP,&rt);
		role->doEvent(WCH_GetMP,MAKECREPARAM(&rt,NULL));
		data->inputParam(WCHDATA_RTMP,&rt);
		unsigned char itemstate = IS_Dead;
		data->inputParam(WCHDATA_ItemState,&itemstate);

		//crMyPlayerData::getInstance()->setNPC(npc.get());
		itr->second.second = npc.get();
		bot->setVisiable(false);
		bot->setEnableIntersect(false);
	}
	CRCore::crDB *db = crBrain::getInstance()->getCurrentActiveDB();
	ref_ptr<crNode> node;
	crCamera *camera = new crCamera;
	camera->setName("MainCamera");
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("MainCamera");
	searchByNameVisitor.setSearchNodeType(ALLNODE);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		camera->setKeyboardMouseMode(CRCore::KM_All);
		camera->attachNode(node.get());
		std::string cameraScript;
		crArgumentParser::readKeyValue(node->getDescriptions(),"CameraScript",cameraScript);
		if(!cameraScript.empty())
		{
			std::string script = gameName + "/script/" + cameraScript;
			ref_ptr<crData> cameraData = CREncapsulation::loadData(script);
			node->setDataClass(cameraData.get());
			node->getParent(0)->removeChild(node.get());
			db->addChild(node.get());
			node->setNumChildrenRequiringUpdateTraversal(1);
			node->setVisiable(true);
		}
		float fov;
		if(crArgumentParser::readKeyValue(node->getDescriptions(),"Fov",fov))
		{
			crDisplaySettings::instance()->setFovy(fov);
		}
	}
	crCameraManager::getInstance()->addCamera(camera);
	CREncapsulation::loadCameraManagerCfg(gameName);

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("RemovedGroup");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		//CRCore::notify(CRCore::FATAL)<<"RemovedGroup: "<<node<<std::endl;
		node->getParent(0)->removeChild(node.get());
	}

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("EffectParticleManager");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		(dynamic_cast<crGroup *>(node->getParent(0)))->removeChild(node.get());
		//CRParticle::crEffectParticleManager *effectParticleManager = CRParticle::crEffectParticleManager::getInstance();
		//effectParticleManager->setName(node->getName());
		//effectParticleManager->setDescriptionsInString(node->getDescriptionsInString());
		//node->getParent(0)->replaceChild(node.get(),effectParticleManager);
		//CRParticle::crEffectParticleManager::getInstance()->setStateSet(node->getStateSet());
		////CRParticle::crEffectParticleManager::getInstance()->setStateSet(crShaderManager::getInstance()->getShaderStateSet("particle"));
	}

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("EffectSequenceManager");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		CRCore::crEffectSequenceManager *effectSequenceManager = CRCore::crEffectSequenceManager::getInstance();
		effectSequenceManager->setName(node->getName());
		effectSequenceManager->setDescriptionsInString(node->getDescriptionsInString());
		node->getParent(0)->replaceChild(node.get(),CRCore::crEffectSequenceManager::getInstance());
	}	
	else
	{
		CRCore::notify(CRCore::WARN)<<"not find EffectSequenceManager"<< std::endl;
	}
	db->addChild(crFilterRenderManager::getInstance());
	db->insertChild(0,crRenderToTextureManager::getInstance());

	//CRCore::ref_ptr<CRAL::crSoundRoot> sound_root = new CRAL::crSoundRoot;
	//db->insertChild(0,sound_root.get());

	CREncapsulation::crNeedInitLoadPageVisitor needInitLoadPageVisitor;
	db->accept(needInitLoadPageVisitor);

	db->addChild(CRPhysics::crVolumeNodeManager::getInstance());

	end_tick = CRCore::Timer::instance()->tick();
	//std::cout << "Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	//CRCore::notify(CRCore::ALWAYS)<<"Now Optimizing... "<< std::endl;
	//start_tick = CRCore::Timer::instance()->tick();
	//CRUtil::crOptimizer optimizer;
	//optimizer.optimize(db/*pSecotr.get()*/,CRUtil::crOptimizer::TEXTURE_SETTINGS/*|CRUtil::crOptimizer::CHECK_PHYSICS*/);

	////CRUtil::crSmoothingVisitor smoother;
	////db->accept(smoother);

	////   int sampleRatio = 1.0;
	////CRUtil::crSimplifier simplifier(sampleRatio);
	////root->accept(simplifier);

	//end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to Optimize = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	//crRunGameHandle::getInstance()->setLoadingProgress(1.0f);
	crStartHandler::getInstance()->endloading();
	CREncapsulation::loadRenderToTextureCfg(gameName);
	CREncapsulation::loadFilterCfg(gameName);
	CREncapsulation::loadUIFile(uifile);
	CREncapsulation::loadEngineUI();
	CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
		false,false, // client storage
		true,1.0, // anisotropic filtering
		NULL );
	tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
	//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
	crFilterRenderManager::getInstance()->accept(tv);

	viewer->setSceneData(db/*pSecotr.get()*/);

	//viewer->setViewEventNode(CRAI::crKeyboardMouseEventGroup::getInstance());
	//CRAI::crKeyboardMouseEventGroup::getInstance()->setBindViewer(viewer);
	role = crMyPlayerData::getInstance()->getCurrentRole();
	if(role)
	{
		crSceneManager::getInstance()->setPlayer(dynamic_cast<crViewMatterObject *>(role->getRelNode()));
	}
	if(!camera->getAttachedNode())
	{
		camera->attachNode(crSceneManager::getInstance()->getPlayer());
		camera->setKeyboardMouseMode(CRCore::KM_None);
	}
	viewer->setViewCameraNode(camera);

	//CRAL::crSoundManager::instance()->loadSound("script/sound1.cfg",crSceneManager::getInstance()->getCurrentSceneName().c_str());

	CRCore::notify(CRCore::ALWAYS)<<"Create Render... "<< std::endl;
	start_tick = CRCore::Timer::instance()->tick();

	//for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoleNpcMap.begin();
	//	itr != myRoleNpcMap.end();
	//	++itr )
	//{
	//	role = itr->second.first.get();
	//	role->doEvent(WCH_CheckAndRackEquipOnMesh,NULL);//std::string CoreBody::getCurrentPartMeshName(int partid) 有出现宕的情况
	//}

	viewer->realize(parentWnd,renderWnd);
	CRProducer::crKeyboardMouseHandle::getInstance()->setBindViewer(viewer);
	crGameClientUpdate::getInstance();

	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Create Render = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	CRCore::notify(CRCore::ALWAYS)<<"Startting..."<<std::endl;
	//bot->doEvent(MAKEINT64(WCH_AITASK,0),MAKEINT64(MAKEINT32(WCHAI_Begin,WCHAI_SetCPToBot/*|WCHAI_TakeoverControl*/),NULL));

	//crPathFindingManager::setPathFindingCount(5);
	crPathFindingUpdater::getInstance()->startThread();
	//crBrain::getInstance()->doEvent(WCH_SceneStart);
	//crMyPlayerData::getInstance()->iTransportCheck();

	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crRequestEmporiumPacket packet;
		netConductor->getNetManager()->sendPacket("all",packet);
	}
	crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
}