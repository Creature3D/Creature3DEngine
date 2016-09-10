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
#include <CREncapsulation/crStartHandler.h>
#include <CREncapsulation/crLoader.h>
#include <rbody/Creature3D/crCharacterSystemUpdater.h>
#include <rbody/Creature3D/CreBody.h>
#include <rbody/Creature3D/ReplicantBodyMgr.h>
#include <CRUtil/crOptimizer.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRAL/crSoundRoot.h>
#include <CRSound/crSoundSystem.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRCore/crBlockDetectThread.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRUtil/crRenderGraph.h>
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crSceneHandler.h>
#include <CRProducer/Producer/RenderSurface>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRPhysics;
using namespace CRParticle;
using namespace CRIOManager;
using namespace CRGUI;
using namespace CRUtil;
using namespace CRProducer;
//using namespace CRAL;
///////////////////////////////////////////////////////////////////////
//
//crStartLoader
//
//////////////////////////////////////////////////////////////////////
void crStartLoader::loading(CRProducer::crViewer *viewer, Producer::Window parentWnd,Producer::Window renderWnd)
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
	crSceneManager *sceneManager = crSceneManager::getInstance();
	crScene *scene = sceneManager->getCurrentScene();
	if(!scene)
	{
		CRCore::notify(CRCore::FATAL)<<"Loading Error 没有指定场景..."<< std::endl;
		return;
	}
	CRIOManager::readObjectFile(/*"scene.cre"*/scene->getSceneFileName());
	std::string brainScript = "script/brain/EditorBrain.cfg";
	ref_ptr<crData> brainData = CREncapsulation::loadData(brainScript);
	crBrain::getInstance()->setDataClass(brainData.get());

	CREncapsulation::loadCursorCfg(viewer);	
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadImageStreamManagerCfg..."<< std::endl;
	//CREncapsulation::loadImageStreamManagerCfg();
	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadFilterCfg..."<< std::endl;

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadWeaponManagerCfg..."<< std::endl;
	//CREncapsulation::loadWeaponManagerCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadMoveTaskManagerCfg..."<< std::endl;
	//CREncapsulation::loadAITaskManagerCfg();

	//CRCore::notify(CRCore::ALWAYS)<<"Loading loadEffectParticleManagerCfg..."<< std::endl;
	//CREncapsulation::loadEffectParticleManagerCfg();
	//CREncapsulation::loadEffectSequenceManagerCfg();

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

	crSave *save = sceneManager->getSave(sceneManager->getCurrentSaveIndex());
	if(!save)
	{
		CRCore::notify(CRCore::FATAL)<<"Loading Error Not Set Save..."<< std::endl;
		return;
	}
	//if(CRCore::crDisplaySettings::instance()->getRunMode() == 1) CREncapsulation::loadSceneManagerCfg(save,true);

	CREncapsulation::loadSave(save);
	//CREncapsulation::loadGameStageCfg();

	crCamera *camera = new crCamera;
	camera->setName("MainCamera");
	camera->setKeyboardMouseMode(CRCore::KM_All);
	crCameraManager::getInstance()->addCamera(camera);
	CREncapsulation::loadCameraManagerCfg();

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
		crSceneManager::getInstance()->getPlayer()->setEnableIntersect(false);
	}

	//if(1)
	//{//当碰撞体出错时，用这个移除场景里的碰撞体
	//	searchByNameVisitor.reset();
	//	searchByNameVisitor.setNameId("CollideRoot");
	//	searchByNameVisitor.setSearchNodeType(GROUP);
	//	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	//	crGroup *collideRoot = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
	//	if(collideRoot)
	//	{
	//		collideRoot->removeChild(0,collideRoot->getNumChildren());
	//	}
	//}

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
		//CRParticle::crEffectParticleManager::getInstance()->setStateSet(crShaderManager::getInstance()->getShaderStateSet("particle"));
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

	//db->addChild(CRParticle::crEffectParticleManager::getInstance());
	//db->addChild(CRParticle::crParticleSystemUpdater::getInstance());
	//db->addChild(CRAI::crObjectEventManager::getInstance());
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
	crStartHandler::getInstance()->endloading();
	CREncapsulation::loadRenderToTextureCfg();
	CREncapsulation::loadFilterCfg();
	CREncapsulation::loadEngineUI();
	CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
		false,false, // client storage
		true,1.0, // anisotropic filtering
		NULL );
	tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
	//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
	crFilterRenderManager::getInstance()->accept(tv);
	viewer->setSceneData(db/*pSecotr.get()*/);

	CRCore::notify(CRCore::ALWAYS)<<"Create Render... "<< std::endl;
	start_tick = CRCore::Timer::instance()->tick();

	viewer->realize(parentWnd,renderWnd);

	end_tick = CRCore::Timer::instance()->tick();
	CRCore::notify(CRCore::ALWAYS)<<"Time to Create Render = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	//viewer->setViewEventNode(CRAI::crKeyboardMouseEventGroup::getInstance());
	CRProducer::crKeyboardMouseHandle::getInstance()->setBindViewer(viewer);

	crMatrixTransform *cameraNode = crSceneManager::getInstance()->getPlayer();
	cameraNode->setAvoidIntersect(true);
	//crIntersectVisitor::insertStaticAvoidIntersectNode(cameraNode);//编辑器相机不拾取
	cameraNode->addDescription("#ScriptAddNode");
	db->addChild(cameraNode);
	camera->attachNode(cameraNode);
	viewer->setViewCameraNode(camera);

	CRCore::notify(CRCore::ALWAYS)<<"Startting..."<<std::endl;

	//CRAL::crSoundManager::instance()->loadSound("script/sound1.cfg",sceneManager->getCurrentSceneName().c_str());

	crSceneManager::getInstance()->getPlayer()->doEvent(MAKEINT64(WCH_AITASK,0),MAKEINT64(MAKEINT32(WCHAI_Begin,WCHAI_SetCPToBot/*|WCHAI_TakeoverControl*/),NULL));

	//crBrain::getInstance()->doEvent(WCH_SceneStart);
}
///////////////////////////////////////////////////////////////////////
//
//crStartHandler
//
//////////////////////////////////////////////////////////////////////
ref_ptr<crStartHandler> crStartHandler::m_instance = NULL;

crStartHandler::~crStartHandler()
{
}

crStartHandler* crStartHandler::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crStartHandler;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crStartHandler::clear()
{
	//crRenderGraph::clearReuseRenderGraph();
	crOdeWorld::odeWorldClear();
	crRegistry::instance()->clear();
	CRProducer::crEventAdapter::reset();
	CRSound::crSoundSystem::getInstance()->clear();
	
	crBrain::getInstance()->accept(*CRPhysics::crPhysicsReleaseVisitor::getInstance());
	crBrain::getInstance()->clear();
	//rbody::ReplicantBodyMgr::instance()->clear();
	//crTextureObjectManager::getInstance()->clear();
	crThread::yieldCurrentThread();
	//crFilterRenderManager::getInstance()->clear();
	//cr2DStageManager::getInstance()->clear();
	m_viewer = NULL;
	CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::clear()"<<std::endl;
	m_loader = NULL;
	m_cullAndUpdThread = NULL;
	//m_drawThread = NULL;
	m_running = false;
	if(m_endcode)
	{
		m_instance = NULL;
	}
}
void crStartHandler::setLoader(crStartLoader *loader)
{
	m_loader = loader;
}
//void crStartHandler::setViewer(CRProducer::crViewer* viewer)
//{
//	m_viewer = viewer;
//}
bool crStartHandler::changeDisplaySettings(int width, int height, int colorbit)
{
	DEVMODE dmScreenSettings;								
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth	= width;
	dmScreenSettings.dmPelsHeight	= height;
	dmScreenSettings.dmBitsPerPel	= colorbit;
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	return ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	//{
		//dmScreenSettings.dmPelsWidth	= 1024;
		//dmScreenSettings.dmPelsHeight	= 768;
		//ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
		//crDisplaySettings::instance()->setScreenWidth(1024);
  //      crDisplaySettings::instance()->setScreenHeight(768);
	//}
}

void crStartHandler::start(unsigned int options1, unsigned int options2, Producer::Window parentWnd, Producer::Window renderWnd)
{
	CREncapsulation::loadInitCfg();
	crFilterRenderManager::getInstance()->init();
	if(m_loadingViewer.valid()&&m_loadingViewer->getDefaultSceneHandler())
	{
		loadLoadingUI();
		crCanvasNode *loadingCanvas = crFilterRenderManager::getInstance()->getLoadingCanvas();
		if(loadingCanvas)
		{
			CRUtil::crOptimizer::crTextureVisitor tv(true,true, // unref image 
				false,false, // client storage
				true,1.0, // anisotropic filtering
				NULL );
			tv.setChangeFilter(crTexture::LINEAR,crTexture::LINEAR);
			//tv.setChangeWrap(crTexture::CLAMP_TO_EDGE);
			loadingCanvas->accept(tv);
			beginloading();	
		}
	}
	//CREncapsulation::createStatsText();
	//CREncapsulation::createLogoText();
	if(!m_viewer.valid())
	{
		m_viewer = new CRProducer::crViewer(m_loadingViewer->getCameraConfig());
		m_viewer->getSceneHandlerList() = m_loadingViewer->getSceneHandlerList();
	}

	m_viewer->setUpViewer(options1);
	m_viewer->setRealizeSceneViewOptions(options2);
    //m_viewer->setParentWindow(parentWnd);
	if(!m_loader.valid())
	{
		m_loader = new crStartLoader;
		CRCore::notify(CRCore::ALWAYS)<<"没有设置StartLoader，使用默认StartLoader"<<std::endl;
	}
	m_loader->loading(m_viewer.get(),parentWnd,renderWnd);
	m_running = true;
	//if(crDisplaySettings::instance()->getRunMode()!=crDisplaySettings::Editor)
	//{
	//	cr2DStageManager::getInstance()->closeAll();
	//	if(m_loadingViewer->getCameraWindow())
	//	{
	//		if(crDisplaySettings::instance()->getFullScreen())
	//			cr2DStageManager::getInstance()->setParentWnd(NULL);
	//		else
	//		{
	//			cr2DStageManager::getInstance()->setParentWnd(m_loadingViewer->getCameraWindow());
	//		}
	//	}
	//	crImageStage::resize();
	//}
	if(crDisplaySettings::instance()->getRunMode()==crDisplaySettings::Editor)
	{
		cr2DStageManager::getInstance()->close("loading");
	}
	m_loadingViewer->getSceneHandlerList() = m_viewer->getSceneHandlerList();
	//if(loadingCanvas)
	//{
	//	crFilterRenderManager::getInstance()->closeCanvas(loadingCanvas);
	//}
	//CRCore::notify(CRCore::ALWAYS)<<"loading end ShaderStateSetMap = "<<crShaderManager::getInstance()->getShaderStateSetMap().size()<<std::endl;
}
void crStartHandler::doMessaging()
{
	MSG msg;	// message info
	//////// are there any messages
	while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)>0)
	{
		// get the message off the queue
		if(GetMessage(&msg,NULL,0,0)<=0)
			break;

		// and send it off again
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)///*PM_REMOVE*/
	//{
	//	TranslateMessage( &msg );
	//	DispatchMessage( &msg );
	//}
}
void crStartHandler::run(/*CRGUI::crImageStage *main*/)
{
	if(!m_viewer) return;
	if(!m_cullAndUpdThread.valid())
		m_cullAndUpdThread = new crCullAndUpdThread(m_viewer.get());

	m_viewer->setDone(false);
	//CRUtil::crAISystemUpdater::getInstance()->start();
	CRPhysics::crPhysicsSystemUpdater::getInstance()->start();
	if(crDisplaySettings::instance()->getRunMode() < crDisplaySettings::Robot)
	{
		CRParticle::crParticleSystemUpdater::getInstance()->start();
		rbody::crCharacterSystemUpdater::getInstance()->start();
		crKeyboardMouseHandle::getInstance()->start();
	}

	CRCore::crThread::yieldCurrentThread();
	startCullAndUpdThread();
	//crSceneManager::getInstance()->start();
	CRCore::crThread::yieldCurrentThread();
	crDisplaySettings::instance()->restoreFpsControl();
	crBlockDetectThread::getInstance()->setTimeout(180);
	if(crDisplaySettings::instance()->getRunMode() >= crDisplaySettings::Robot)
	{
		crBlockDetectThread::getInstance()->setTimeout(360);
	}
	crBlockDetectThread::getInstance()->start();
	do
	{
	    m_viewer->draw();
		crBlockDetectThread::getInstance()->update();
		//if(main) main->getWindow()->_doMessaging();
		doMessaging();
	}
	while (!m_viewer->done());
	crBlockDetectThread::getInstance()->clear();
	m_running = false;
   //m_viewer->draw();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() m_viewer->done()"<<std::endl;
	//while(!m_viewer->beSafeToExit())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	//表示最后第二次的cull线程已经安全结束
    //crSceneManager::getInstance()->done();
    //CRCore::crBarrier *barrier;
	//CRCore::crBarrier *barrier = CRUtil::crAISystemUpdater::getInstance()->getAiUpdateBlock();
	//while(!barrier->numThreadsCurrentlyBlocked())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	//CRUtil::crAISystemUpdater::getInstance()->aiUpdateBlock();
	////此时AI已经进入最后一次循环并停留在frameBarrier上
	//CRUtil::crAISystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crAISystemUpdater::getInstance()->done()"<<std::endl;
	//barrier = rbody::crCharacterSystemUpdater::getInstance()->getUpdateBlock();
	//while(!barrier->numThreadsCurrentlyBlocked())
	//{
	//	CRCore::crThread::sleep(10);
	//	//CRCore::crThread::sleep(10);
	//}
	////此时表示crCharacterSystemUpdater已经进入最后一次循环并处于次cullBarrier等待状态
	//rbody::crCharacterSystemUpdater::getInstance()->done();

	//barrier = CRParticle::crParticleSystemUpdater::getInstance()->getUpdateBlock();
	//while(!barrier->numThreadsCurrentlyBlocked())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	//此时表示crParticleSystemUpdater已经进入最后一次循环并处于次cullBarrier等待状态
	//CRParticle::crParticleSystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crParticleSystemUpdater::getInstance()->done()"<<std::endl;

	//barrier = CRPhysics::crPhysicsSystemUpdater::getInstance()->getPhysicsUpdateBlock();
	//while(!barrier->numThreadsCurrentlyBlocked())
	//{
	//	CRCore::crThread::sleep(10);
	//}
	//此时crPhysicsSystemUpdater进入到最后一次循环并停留在updateBarrier上
	//CRPhysics::crPhysicsSystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crPhysicsSystemUpdater::getInstance()->done()"<<std::endl;

	//CRCore::crThread::sleep(10);

	//CRCore::notify(CRCore::ALWAYS)<<"crParticleSystemUpdater::getInstance()->done()"<<std::endl;
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() cancelCameraThreads"<<std::endl;
	m_viewer->cancelCameraThreads();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() cancelCameraThreads end"<<std::endl;

	//clear 
	//CRUtil::crAISystemUpdater::getInstance()->clear();
	//CRPhysics::crPhysicsSystemUpdater::getInstance()->clear();
	//CRParticle::crParticleSystemUpdater::getInstance()->clear();

	//m_cullAndUpdThread->done();
	//m_viewer->releaseUpdateThreads();
	while( m_cullAndUpdThread->isRunning() )
	{
		CRCore::crThread::yieldCurrentThread();
	}
	m_cullAndUpdThread = NULL;

	m_viewer->sync();

	CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() end"<<std::endl;

///////////////////////////////////////////////
/*	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() end"<<std::endl;
	CRUtil::crAISystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() crAISystemUpdater end"<<std::endl;
	CRPhysics::crPhysicsSystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() crPhysicsSystemUpdater end"<<std::endl;
	CRParticle::crParticleSystemUpdater::getInstance()->done();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() crParticleSystemUpdater end"<<std::endl;
	m_viewer->cancelCameraThreads();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() cancelCameraThreads end"<<std::endl;
	m_viewer->sync();
	//CRCore::notify(CRCore::ALWAYS)<<"crStartHandler::run() sync end"<<std::endl;
	*/
}

void crStartHandler::startCullAndUpdThread()
{
	CRCore::crThread::init();
	/*int status = */m_cullAndUpdThread->start();
	//assert(status == 0);
}

//void crStartHandler::startDrawThread()
//{
//	CRCore::crThread::init();
//	/*int status = */m_drawThread->start();
//	//assert(status == 0);
//}

void crStartHandler::end()
{
	//if(m_cullAndUpdThread.valid()) m_cullAndUpdThread->cancel();
	//if(m_drawThread.valid()) m_drawThread->cancel();
	clear();
}

void crCullAndUpdThread::run()
{
	m_done = false;
	do
	{
	    m_viewer->cull_update();
	}while (!m_viewer->done());
/////////////////////////
/*
	CRUtil::crAISystemUpdater::getInstance()->aiUpdateBlock();
	//CRCore::notify(CRCore::ALWAYS)<<"crCullAndUpdThread::run() crAISystemUpdater end"<<std::endl;
*/
}
void crStartHandler::beginloading()
{
	if(!m_loadindDrawThread.valid())
	{
		m_loadindDrawThread = new crLoadingDrawThread(m_loadingViewer.get());
	}
	m_loadindDrawThread->start();
}
void crStartHandler::endloading()//结束loading画面
{
	if(m_loadindDrawThread.valid())
	{
		crThread::sleep(100);
		m_loadindDrawThread->done();
		while(m_loadindDrawThread->isRunning())
		{
			crThread::yieldCurrentThread();
		}
	}
}
void crLoadingDrawThread::run()
{
	m_done = false;
	ref_ptr<crSceneView> sv = m_loadingViewer->getDefaultSceneHandler()->getSceneView();
	Producer::RenderSurface *rs = m_loadingViewer->getCameraConfig()->getRenderSurface(0);
	if(sv.valid() && rs)
	{
		//unsigned char mode = crDisplaySettings::instance()->getUIScaleMode();
		//crDisplaySettings::instance()->setUIScaleMode(1);
		crCanvasNode *loadingui = crFilterRenderManager::getInstance()->getLoadingCanvas();
		loadingui->accept(*(sv->getInitVisitor()));
		//crDisplaySettings::instance()->setUIScaleMode(mode);
		crFilterRenderManager::getInstance()->showCanvas(loadingui,loadingui->getName(),SH_Show);
		CRCore::Timer   timer;
		CRCore::Timer_t start_tick = timer.tick();
		ref_ptr<crFrameStamp> frameStamp = crFrameStamp::getInstance();
		frameStamp->setFrameNumber(8);
		frameStamp->setReferenceTime(0.0f);
		sv->setSceneData(crFilterRenderManager::getInstance());
		sv->setAbortRenderingPtr(NULL);
		CRCore::crDisplaySettings::instance()->setFpsControl(50.0f);
		rs->makeCurrent();
		while (!m_done)
		{
			frameStamp->setFrameNumber(frameStamp->getFrameNumber()+1);
			frameStamp->setReferenceTime(timer.delta_s(start_tick,timer.tick()));
			frameStamp->update();
			sv->update();
			sv->cull();
			sv->draw();
			rs->swapBuffers();
			crBlockDetectThread::getInstance()->update();
			crStartHandler::getInstance()->doMessaging();
		}
		//loadingui->releaseObjects();
		//crFilterRenderManager::getInstance()->removeLoadingUI();
		//sv->setSceneData(NULL);
		sv->releaseAllObjects();
		sv->flushAllDeletedObjects();
		//sv->unlockSwapBuffer();
		frameStamp->setFrameNumber(0);
		frameStamp->setReferenceTime(0.0f);
		CRCore::crDisplaySettings::instance()->restoreFpsControl();
		rs->releaseCurrent();
		crFilterRenderManager::getInstance()->init();
	}
	m_done = true;
}