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
#include <CRNetApp/appLoginStartLoader.h>
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
#include <CRNetApp/appGlobalHandle.h>
#include <CRUtil/crOptimizer.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRAL/crSoundRoot.h>
//#include <CRAL/crSoundManager.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CREncapsulation;
using namespace CRPhysics;
using namespace CRNet;
///////////////////////////////////////////////////////////////////////
//
//appLoginStartLoader
//
//////////////////////////////////////////////////////////////////////
void crLoginStartLoader::loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd)
{
	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	CRCore::Timer_t end_tick;

	//CRCore::notify(CRCore::ALWAYS)<<"Loading..."<< std::endl;
	char gbuf[256];
	sprintf(gbuf,"登录界面Loading,使用最低画质...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	crDisplaySettings::instance()->setHDR(0);//对于低端显卡开启HDR后，无法进入游戏，可以在启动界面重设显示设置
	crDisplaySettings::instance()->setShadowDetail(0);
	crDisplaySettings::instance()->setEnableAlphaShadow(false);
	//crDisplaySettings::instance()->setNumMultiSamples(0);
	crDisplaySettings::instance()->setMaxAnisotropy(8);

	void *param;
	crData *data = crRunGameHandle::getInstance()->getDataClass();
	data->getParam(WCHDATA_SceneFile,param);
	std::string sceneFile = *(std::string *)param;
	data->getParam(WCHDATA_UIFile,param);
	std::string uifile = *(std::string *)param;
	data->getParam(WCHDATA_BrainScript,param);
	std::string brainScript = *(std::string *)param;
	CRIOManager::readObjectFile(sceneFile);

	CREncapsulation::loadCursorCfg(viewer);
	
	ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	crBrain::getInstance()->setDataClass(brainData.get());

	//CREncapsulation::loadAITaskManagerCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading end..."<< std::endl;
	sprintf(gbuf,"Loading end...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;

	ref_ptr<crGroup> botRoot;
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Wucaihua");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	botRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!botRoot)
	{
		//CRCore::notify(CRCore::WARN)<<"没有找到SceneRoot"<< std::endl;
		sprintf(gbuf,"没有找到SceneRoot\n\0");
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
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
		//CRCore::notify(CRCore::WARN)<<"没有找到StaticRoot"<< std::endl;
		sprintf(gbuf,"没有找到StaticRoot\n\0");
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
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
		//CRCore::notify(CRCore::WARN)<<"没有找到DynamicRoot"<< std::endl;
		sprintf(gbuf,"没有找到DynamicRoot\n\0");
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return;
	}
	crSceneManager::getInstance()->setDynamicRoot(dynamicRoot);

	CRCore::crDB *db = crBrain::getInstance()->getCurrentActiveDB();
	ref_ptr<crNode> node;
	crCamera *camera = new crCamera;
	camera->setName("MainCamera");
	camera->setKeyboardMouseMode(CRCore::KM_All);
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("MainCamera");
	searchByNameVisitor.setSearchNodeType(ALLNODE);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
		camera->attachNode(node.get());
		std::string cameraScript;
		crArgumentParser::readKeyValue(node->getDescriptions(),"CameraScript",cameraScript);
		if(!cameraScript.empty())
		{
			std::string script = "script/" + cameraScript;
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
	CREncapsulation::loadCameraManagerCfg();

	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("RemovedGroup");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
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
		//CRCore::notify(CRCore::WARN)<<"not find EffectSequenceManager"<< std::endl;
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
	//CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	sprintf(gbuf,"Time to load = %f\n\0",CRCore::Timer::instance()->delta_s(start_tick,end_tick));
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	//crRunGameHandle::getInstance()->setLoadingProgress(1.0f);
	crStartHandler::getInstance()->endloading();
	CREncapsulation::loadRenderToTextureCfg();
	CREncapsulation::loadUIFile(uifile);
	CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
		false,false, // client storage
		true,1.0, // anisotropic filtering
		NULL );
	tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
	//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
	crFilterRenderManager::getInstance()->accept(tv);
	viewer->setSceneData(db/*pSecotr.get()*/);

	//camera->attachNode(crSceneManager::getInstance()->getPlayer()->getNPCBot());
	//camera->setKeyboardMouseMode(crCamera::KM_None);
	viewer->setViewCameraNode(camera);

	//CRAL::crSoundManager::instance()->loadSound("script/sound1.cfg",crSceneManager::getInstance()->getCurrentSceneName().c_str());

	//CRCore::notify(CRCore::ALWAYS)<<"Create Render... "<< std::endl;
	sprintf(gbuf,"Create Render...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	start_tick = CRCore::Timer::instance()->tick();

	viewer->realize(parentWnd,renderWnd);
	CRProducer::crKeyboardMouseHandle::getInstance()->setBindViewer(viewer);

	end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to Create Render = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	sprintf(gbuf,"Time to Create Render = %f\n\0",CRCore::Timer::instance()->delta_s(start_tick,end_tick));
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);

	//CRCore::notify(CRCore::ALWAYS)<<"Startting..."<<std::endl;
	sprintf(gbuf,"Startting...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	//crBrain::getInstance()->doEvent(WCH_SceneStart);
}
///////////////////////////////////////////////////////////////////////
//
//crLoginStartLoader2
//
//////////////////////////////////////////////////////////////////////
void crLoginStartLoader2::loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd)
{
	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	CRCore::Timer_t end_tick;

	//CRCore::notify(CRCore::ALWAYS)<<"Loading..."<< std::endl;
	char gbuf[256];
	sprintf(gbuf,"Loading...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	//crDisplaySettings::instance()->setHDR(0);//对于低端显卡开启HDR后，无法进入游戏，可以在启动界面重设显示设置

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
	
	//ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	//crBrain::getInstance()->setDataClass(brainData.get());

	//CREncapsulation::loadAITaskManagerCfg();
	//CRCore::notify(CRCore::ALWAYS)<<"Loading end..."<< std::endl;
	sprintf(gbuf,"Loading end...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;

	ref_ptr<crGroup> botRoot;
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("Wucaihua");
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	botRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	if(!botRoot)
	{
		//CRCore::notify(CRCore::WARN)<<"没有找到SceneRoot"<< std::endl;
		sprintf(gbuf,"没有找到SceneRoot\n\0");
		gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
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
		//CRCore::notify(CRCore::WARN)<<"没有找到StaticRoot"<< std::endl;
		sprintf(gbuf,"没有找到StaticRoot\n\0");
		gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
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
		//CRCore::notify(CRCore::WARN)<<"没有找到DynamicRoot"<< std::endl;
		sprintf(gbuf,"没有找到DynamicRoot\n\0");
		gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
		return;
	}
	crSceneManager::getInstance()->setDynamicRoot(dynamicRoot);

	CRCore::crDB *db = crBrain::getInstance()->getCurrentActiveDB();
	ref_ptr<crNode> node;
	crCamera *camera = new crCamera;
	camera->setName("MainCamera");
	camera->setKeyboardMouseMode(CRCore::KM_All);
	searchByNameVisitor.reset();
	searchByNameVisitor.setNameId("MainCamera");
	searchByNameVisitor.setSearchNodeType(ALLNODE);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	node = searchByNameVisitor.getResult();
	if(node.valid())
	{
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
		//CRCore::notify(CRCore::WARN)<<"not find EffectSequenceManager"<< std::endl;
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
	//CRCore::notify(CRCore::ALWAYS)<<"Time to load = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	sprintf(gbuf,"Time to load =%f\n\0",CRCore::Timer::instance()->delta_s(start_tick,end_tick));
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);

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

	//camera->attachNode(crSceneManager::getInstance()->getPlayer()->getNPCBot());
	//camera->setKeyboardMouseMode(crCamera::KM_None);
	viewer->setViewCameraNode(camera);

	//CRAL::crSoundManager::instance()->loadSound("script/sound1.cfg",crSceneManager::getInstance()->getCurrentSceneName().c_str());

	//CRCore::notify(CRCore::ALWAYS)<<"Create Render... "<< std::endl;
	sprintf(gbuf,"Create Render...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	start_tick = CRCore::Timer::instance()->tick();

	viewer->realize(parentWnd,renderWnd);
	CRProducer::crKeyboardMouseHandle::getInstance()->setBindViewer(viewer);
	crGameClientUpdate::getInstance();

	end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to Create Render = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	sprintf(gbuf,"Time to Create Render =%f\n\0",CRCore::Timer::instance()->delta_s(start_tick,end_tick));
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);

	//CRCore::notify(CRCore::ALWAYS)<<"Startting..."<<std::endl;
	sprintf(gbuf,"Startting...\n\0");
	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
	//crBrain::getInstance()->doEvent(WCH_SceneStart);
	//crGameClientUpdate::getInstance();
}