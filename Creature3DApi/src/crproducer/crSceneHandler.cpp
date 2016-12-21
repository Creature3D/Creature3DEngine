/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRProducer/crSceneHandler.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crRenderToTextureManager.h>
//#include <CRUtil/crAISystemUpdater.h>
#include <CRPhysics/crOdeWorld.h>
#include <rbody/Creature3D/crCharacterSystemUpdater.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRCore/crStatistics.h>
using namespace CRCore;
using namespace CRUtil;
using namespace CRProducer;

//crSceneHandler::crSceneHandler( CRCore::crDisplaySettings *ds ) :
//    m_sceneView(new CRUtil::crSceneView(ds))
//{
//}
CRCore::crMutex crSceneHandler::s_mutex;

class MotionBlurDrawCallback: public crSceneHandler::Callback
{
public:
	MotionBlurDrawCallback(double persistence)
		:m_cleared(false),
		m_persistence(persistence)
	{
	}

	virtual void operator()(crSceneHandler &handler, Producer::Camera &camera,int vreye)
	{
		double t = crFrameStamp::getInstance()->getReferenceTime();

		if (!m_cleared)
		{
			// clear the accumulation buffer
			glClearColor(0, 0, 0, 0);
			glClear(GL_ACCUM_BUFFER_BIT);
			m_cleared = true;
			m_t0 = t;
		}

		double dt = fabs(t - m_t0);
		m_t0 = t;

		// call the scene handler's draw function
		handler.drawImplementation(camera, vreye);

		// compute the blur factor
		double s = powf(0.2, dt / m_persistence);

		// scale, accumulate and return
		glAccum(GL_MULT, s);
		glAccum(GL_ACCUM, 1 - s);
		glAccum(GL_RETURN, 1.0f);
	}

private:
	bool m_cleared;
	double m_t0;
	double m_persistence;
};

crSceneHandler::crSceneHandler( CRUtil::crSceneView *sv ) :
    m_sceneView(sv)
{
	m_frameStartTick = 0;
	m_previousFrameStartTick = 0;
	m_flushOfAllDeletedObjectsOnNextFrame = false;
	m_cleanUpOnNextFrame = false;
}

void crSceneHandler::setSceneView(CRUtil::crSceneView* sceneView) { m_sceneView = sceneView; }

void crSceneHandler::init()
{
    s_mutex.lock();
    //CRCore::notify(CRCore::INFO)<<"   running "<<this<<" init."<<std::endl;

	if(crDisplaySettings::instance()->getMotionBlur())
	{
		MotionBlurDrawCallback *callback = new MotionBlurDrawCallback(crBrain::getInstance()->getMotionBlurFactor());
		setDrawCallback(callback);
	}

    m_sceneView->init();

	CRIOManager::crDatabasePager* dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
	dp->setPhysicsInitVisitor(m_sceneView->getPhysicsInitVisitor());
	dp->setRenderInitVisitor(m_sceneView->getInitVisitor());
    dp->inited();

	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::init1"<<std::endl;
    s_mutex.unlock();
    //CRCore::notify(CRCore::INFO)<<"   unlocked "<<this<<" init."<<std::endl;
}

void crSceneHandler::clearImplementation(Producer::Camera& /*camera*/)
{
	m_previousFrameStartTick = m_frameStartTick;
	m_frameStartTick = CRCore::Timer::instance()->tick();

	//CRIOManager::crDatabasePager* dp = CRIOManager::crRegistry::instance()->getDatabasePager();
	//if (dp)
	//{
	//	dp->signalBeginFrame(getSceneView()->getCurrentRenderState()->getFrameStamp());
	//}
}

void crSceneHandler::setClearCallback(Callback* callback) { m_clearCallback = callback; }
crSceneHandler::Callback* crSceneHandler::getClearCallback() { return m_clearCallback.get(); }
const crSceneHandler::Callback* crSceneHandler::getClearCallback() const { return m_clearCallback.get(); }

void crSceneHandler::setCullCallback(Callback* callback) { m_cullCallback = callback; }
crSceneHandler::Callback* crSceneHandler::getCullCallback() { return m_cullCallback.get(); }
const crSceneHandler::Callback* crSceneHandler::getCullCallback() const { return m_cullCallback.get(); }

void crSceneHandler::setUpdateCallback(Callback* callback) { m_updateCallback = callback; }
crSceneHandler::Callback* crSceneHandler::getUpdateCallback() { return m_updateCallback.get(); }
const crSceneHandler::Callback* crSceneHandler::getUpdateCallback() const { return m_updateCallback.get(); }

void crSceneHandler::setContextID( int id )
{
    //m_sceneView->getState()->setContextID( id );
	m_sceneView->setContextID( id );
}

void crSceneHandler::release()
{
	//try
	//{
	s_mutex.lock();
	
	////CRUtil::crAISystemUpdater::getInstance()->clear();
	//CRPhysics::crPhysicsSystemUpdater::getInstance()->clear();
	//CRParticle::crParticleSystemUpdater::getInstance()->clear();
	CRIOManager::crRegistry::instance()->releaseObjects();
	m_sceneView->releaseAllObjects();
	m_sceneView->flushAllDeletedObjects();
	m_nearCameraPos.set(0,0,0);
	s_mutex.unlock();
	//}
	//catch (...)
	//{
	//	CRCore::notify(CRCore::WARN)<<" crSceneHandler::release() error "<<std::endl;
	//}
}
void crSceneHandler::updateImplementation(Producer::Camera& camera)
{
	CRCore::Timer_t start_tick = 0;
	CRCore::Timer_t end_tick = 0;
	if (crStatistics::getInstance()->getStat())
	{
		start_tick = CRCore::Timer::instance()->tick();
	}

	s_mutex.lock();
	
	m_sceneView->getProjectionMatrix()->set(camera.getProjectionMatrix());
	m_sceneView->getViewMatrix()->set(camera.getPositionAndAttitudeMatrix());
	m_sceneView->getCamera()->computerViewInverseMatrix();

	int x, y;
	unsigned int w, h;
	camera.getProjectionRectangle( x, y, w, h );
	m_sceneView->setViewport( x, y, w, h );

	CRPhysics::crPhysicsSystemUpdater::getInstance()->physicsUpdateBlock();//enabled body not tagged
	if(crDisplaySettings::instance()->getRunMode() < crDisplaySettings::Robot)
	{
		CRParticle::crParticleSystemUpdater::getInstance()->updateBlock();
		rbody::crCharacterSystemUpdater::getInstance()->updateBlock();
		crKeyboardMouseHandle::getInstance()->updateBlock();
	}

	CRCore::crNode* node = m_cameraGroup->getTopMostSceneData();
	if (node) node->getBound();

	CRIOManager::crDatabasePager* dp = CRIOManager::crRegistry::instance()->getDatabasePager();
	if (dp/*&&dp->requiresUpdateSceneGraph()*/)
	{
		// update the scene graph by remove expired subgraphs and merge newly loaded subgraphs

		dp->signalBeginFrame(/*m_cameraGroup->getFrameStamp()*/);
		dp->updateSceneGraph(crFrameStamp::getInstance()->getReferenceTime());
	}
	m_sceneView->update();

	CRPhysics::crPhysicsSystemUpdater::getInstance()->frameBlock();//20140131 add
	if(crDisplaySettings::instance()->getRunMode() < crDisplaySettings::Robot)
	{
		CRParticle::crParticleSystemUpdater::getInstance()->frameBlock();
		rbody::crCharacterSystemUpdater::getInstance()->frameBlock();
	}

	if(crFrameStamp::getInstance()->getFrameNumber()>INITFRAMENUMBER)
	{
		const CRCore::crVector3 &curCampos = CRCore::crBrain::getInstance()->getCameraPosition();
		if((curCampos-m_nearCameraPos).length()>1.0f)
		{
			CRCore::crIntersectVisitor::HitList hits;
			dynamic_cast<crViewer *>(m_cameraGroup)->computeIntersections(0,-1,hits);
			if(!hits.empty())
			{
				CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
				CRCore::crRenderToTextureManager::getInstance()->setNearPoint(pickPoint);
				m_nearCameraPos = curCampos;
			}
			//else
			//{
			//	CRCore::crRenderToTextureManager::getInstance()->setNearPoint(curCampos);
			//}
			//if(crFrameStamp::getInstance()->getFrameNumber()>INITFRAMENUMBER1)
			//	m_nearCameraPos = curCampos;
		}
	}
	s_mutex.unlock();
	if (crStatistics::getInstance()->getStat())
	{
		end_tick = CRCore::Timer::instance()->tick();
		float updtime = CRCore::Timer::instance()->delta_m(start_tick, end_tick);
		crStatistics::getInstance()->setUpdTime(updtime);
		//CRText::crText *updtimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("updTimeText"));
		//updtimeText->setText(crArgumentParser::appFtoa(updtime));
	}
	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::updateImplementation 1"<<std::endl;
}
//void crSceneHandler::initRender()
//{
//	//try
//	//{
//	const crBrain::InitRenderVecBuf &initRenderVecBuf = crBrain::getInstance()->getInitRenderVecBuf();
//	if(initRenderVecBuf.empty() || initRenderVecBuf.size()%2 != 0) return;
//	crVector3f pos, center;
//	for( crBrain::InitRenderVecBuf::const_iterator itr = initRenderVecBuf.begin();
//		 itr != initRenderVecBuf.end();
//		 ++itr )
//	{
//        pos = *itr;
//		itr++;
//		center = *itr;
//
//		//CRCore::notify(CRCore::FATAL)<<"initRender: pos = "<<pos<<" center = "<<center<<std::endl;
//
//		m_sceneView->getViewMatrix()->makeLookAt(pos,center,CRCore::Z_AXIS);
//		m_sceneView->setViewport(0, 0, crDisplaySettings::instance()->getViewWidth(), crDisplaySettings::instance()->getViewHeight());
//		////CRCore::notify(CRCore::FATAL)<<"update"<<std::endl;
//		m_sceneView->update();
//		////CRCore::notify(CRCore::FATAL)<<"cull"<<std::endl;
//		m_sceneView->cull();
//		////CRCore::notify(CRCore::FATAL)<<"draw"<<std::endl;
//		m_sceneView->draw();
//
//		if(m_sceneView->getUseRenderDoubleBuffer())
//		{
//			//CRCore::notify(CRCore::FATAL)<<"initRender DoubleBuffer: pos = "<<pos<<" center = "<<center<<std::endl;
//
//			m_sceneView->getViewMatrix()->makeLookAt(pos,center,CRCore::Z_AXIS);
//			m_sceneView->setViewport(0, 0, crDisplaySettings::instance()->getViewWidth(), crDisplaySettings::instance()->getViewHeight());
//			////CRCore::notify(CRCore::FATAL)<<"update"<<std::endl;
//			m_sceneView->update();
//			////CRCore::notify(CRCore::FATAL)<<"cull"<<std::endl;
//			m_sceneView->cull();
//			////CRCore::notify(CRCore::FATAL)<<"draw"<<std::endl;
//			m_sceneView->draw();
//		}
//	}
//	//}
//	//catch (...)
//	//{
// //       CRCore::notify(CRCore::FATAL)<<"crSceneHandler::initRender() error "<<std::endl;
//	//}
//	////CRCore::notify(CRCore::FATAL)<<"crSceneHandler::initRender"<<std::endl;
//	//const CRCore::crBoundingSphere& boundingSphere = m_sceneView->getCamera()->getBound();
//	//m_sceneView->getViewMatrix()->makeLookAt(boundingSphere.m_center+CRCore::crVector3( 0.0,-3.5f * boundingSphere.m_fRadius,0.0f),boundingSphere.m_center,CRCore::Z_AXIS);
//	//m_sceneView->setViewport(0, 0, crDisplaySettings::instance()->getScreenWidth(), crDisplaySettings::instance()->getScreenHeight());
//	////CRCore::notify(CRCore::FATAL)<<"update"<<std::endl;
//	//m_sceneView->update();
//	////CRCore::notify(CRCore::FATAL)<<"cull"<<std::endl;
//	//m_sceneView->cull();
//	////CRCore::notify(CRCore::FATAL)<<"draw"<<std::endl;
//	//m_sceneView->draw();
//	////CRCore::notify(CRCore::FATAL)<<"crSceneHandler::initRender end"<<std::endl;
//}
//
//void crSceneHandler::initBufRender()
//{
//	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::initBufRender"<<std::endl;
//
//	if(m_sceneView->getUseRenderDoubleBuffer())
//	{
//		const CRCore::crBoundingSphere& boundingSphere = m_sceneView->getCamera()->getBound();
//		m_sceneView->getViewMatrix()->makeLookAt(boundingSphere.m_center+CRCore::crVector3( 0.0,-3.5f * boundingSphere.m_fRadius,0.0f),boundingSphere.m_center,CRCore::Z_AXIS);
//		m_sceneView->setViewport(0, 0, crDisplaySettings::instance()->getScreenWidth(), crDisplaySettings::instance()->getScreenHeight());
//		m_sceneView->update();	
//		m_sceneView->cull();
//		m_sceneView->draw();
//	}
//
//	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::initBufRender end"<<std::endl;
//}
