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
#ifndef CRPRODUCER_CRSCENEHANDLER
#define CRPRODUCER_CRSCENEHANDLER 1

#include <CRProducer/crExport.h>
#include <CRCore/crTimer.h>
#include <CRProducer/Producer/Camera>
#include <CRCore/thread/crMutex.h>
#include <CRIOManager/crRegistry.h>
#include <CRProducer/crCameraGroup.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRUtil/crSceneView.h>
#include <CRPhysics/crOdeWorld.h>

//#include <CRCore/crPlane.h>
namespace CRProducer {

class CRPRODUCER_EXPORT crSceneHandler : public Producer::Camera::SceneHandler
{
    public :
    
        crSceneHandler(CRUtil::crSceneView *sv);
        
        /// set the scene view to which will manage rendering of the CRE scene.
        void setSceneView(CRUtil::crSceneView* sceneView);
        
        /// get the scene view.
        inline CRUtil::crSceneView* getSceneView() { return m_sceneView.get(); }
        
        /// get the const scene view.
        inline const CRUtil::crSceneView* getSceneView() const { return m_sceneView.get(); }

		virtual void release();
        virtual void init();
        //virtual bool useAutoView() { return false; }
        class Callback : public CRCore::Referenced
        {
        public:
               virtual ~Callback() {}
               virtual void operator()(crSceneHandler&, Producer::Camera &) = 0;             
        };
        
        virtual void clear(Producer::Camera& camera)
        {
            if (m_clearCallback.valid()) (*m_clearCallback)(*this,camera);
            else clearImplementation(camera);
        }

        virtual void clearImplementation(Producer::Camera& camera);

        void setClearCallback(Callback* callback);
        Callback* getClearCallback();
        const Callback* getClearCallback() const;

        inline virtual void cull(Producer::Camera& camera)
        {
            if (m_cullCallback.valid()) (*m_cullCallback)(*this,camera);
            else cullImplementation(camera);
        }

        inline virtual void cullImplementation(Producer::Camera& camera);

        void setCullCallback(Callback* callback);
        Callback* getCullCallback();
        const Callback* getCullCallback() const;

		inline virtual void update(Producer::Camera & camera) 
		{
			CRCore::crBrain::getInstance()->gameLoop(0);//ÔÝÍ£UpdateThread
		    if (m_updateCallback.valid()) (*m_updateCallback)(*this,camera);
		    else updateImplementation(camera);
		}

		virtual void updateImplementation(Producer::Camera& camera);

		void setUpdateCallback(Callback* callback);
		Callback* getUpdateCallback();
		const Callback* getUpdateCallback() const;

        inline virtual void draw(Producer::Camera& camera)
        {
			//if (m_cleanUpOnNextFrame && m_sceneView.valid())
			//{
			//	m_sceneView->flushAllDeletedObjects();
			//	m_flushOfAllDeletedObjectsOnNextFrame = false;
			//	m_cleanUpOnNextFrame = false;
			//}
			//else
			//{
				//if (m_flushOfAllDeletedObjectsOnNextFrame && m_sceneView.valid()) 
				//{
				//	m_sceneView->flushAllDeletedObjects();
				//}
				if (m_drawCallback.valid()) (*m_drawCallback)(*this,camera);
				else drawImplementation(camera);

				//m_flushOfAllDeletedObjectsOnNextFrame = false;
			//}
			CRCore::crBrain::getInstance()->gameLoop(1);//ÔÝÍ£DrawThread
        }
        
        inline virtual void drawImplementation(Producer::Camera& camera);

        void setDrawCallback(Callback* callback) { m_drawCallback = callback; }
        Callback* getDrawCallback() { return m_drawCallback.get(); }
        const Callback* getDrawCallback() const { return m_drawCallback.get(); }


        void setContextID( int id );

		void setFlushOfAllDeletedObjectsOnNextFrame(bool flag) { m_flushOfAllDeletedObjectsOnNextFrame = flag; }

		bool getFlushOfAllDeletedObjectsOnNextFrame() const { return m_flushOfAllDeletedObjectsOnNextFrame; }
   
		void setCleanUpOnNextFrame(bool flag) { m_cleanUpOnNextFrame = flag; }

		bool getCleanUpOnNextFrame() const { return m_cleanUpOnNextFrame; }

		void setCameraGroup(crCameraGroup *cameraGroup){ m_cameraGroup = cameraGroup; }

		virtual bool useAutoView() { return false; }

		//void initRender();
		//void initBufRender();
    protected:
    
        virtual ~crSceneHandler() {}
        
        
        CRCore::ref_ptr<CRUtil::crSceneView> m_sceneView;
    
        CRCore::ref_ptr<Callback> m_clearCallback;
        CRCore::ref_ptr<Callback> m_cullCallback;
        CRCore::ref_ptr<Callback> m_drawCallback;
		CRCore::ref_ptr<Callback> m_updateCallback;

		CRCore::Timer_t           m_frameStartTick;
		CRCore::Timer_t           m_previousFrameStartTick;

		bool                      m_flushOfAllDeletedObjectsOnNextFrame;
		bool                      m_cleanUpOnNextFrame;
		crCameraGroup            *m_cameraGroup;
		CRCore::crVector3         m_nearCameraPos;
		static CRCore::crMutex s_mutex;

};

inline void crSceneHandler::cullImplementation(Producer::Camera &cam) 
{
	s_mutex.lock();
	//m_sceneView->getProjectionMatrix()->set(cam.getProjectionMatrix());
	//m_sceneView->getViewMatrix()->set(cam.getPositionAndAttitudeMatrix());
	//m_sceneView->updateHorizontalFov(cam.getLensHorizontalFov());
	//   m_sceneView->updateVerticalFov(cam.getLensVerticalFov());

	//int x, y;
	//unsigned int w, h;
	//cam.getProjectionRectangle( x, y, w, h );
	//m_sceneView->setViewport( x, y, w, h );

	//CRCore::crVector4f clear_color;
	//cam.getClearColor(clear_color[0],clear_color[1],clear_color[2],clear_color[3]);
	//m_sceneView->setClearColor(clear_color);

	m_sceneView->cull();

	if(CRCore::crDisplaySettings::instance()->getRunMode() < CRCore::crDisplaySettings::Robot)
	{
		crKeyboardMouseHandle::getInstance()->frameBlock();
	}

	s_mutex.unlock();

	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::cullImplementation 1"<<std::endl;
}

inline void crSceneHandler::drawImplementation(Producer::Camera &cam) 
{
	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::drawImplementation 0"<<std::endl;

	// dipatch the draw traversal of the scene graph
	m_sceneView->draw();

	// for the database pager now manage any GL object operations that are required.
	CRIOManager::crDatabasePager* dp = CRIOManager::crRegistry::instance()->getDatabasePager();
	if (dp)
	{
        dp->flushDeletedObjects(m_sceneView->getState());
#if 1
		double timeForCullAndDraw = CRCore::Timer::instance()->delta_s(m_frameStartTick, CRCore::Timer::instance()->tick());

		double targeFrameTime = 1.0/dp->getTargetFrameRate();

		double drawCostFactor = 2.0; // must be greater than 1 to account for the extra cost of emptying the OpenGL fifo.
		double frameFactor = 0.9; // must be less than 1, to compensate for extra time spent in update and swap buffers etc.
		double timeLeftTillEndOfFrame = targeFrameTime*frameFactor - timeForCullAndDraw*drawCostFactor;
		double availableTime = timeLeftTillEndOfFrame / drawCostFactor; // account for the fifo when download texture objects.

		// clamp the available time by the prescribed minimum
		if (availableTime<dp->getMinimumTimeAvailableForGLCompileAndDeletePerFrame())
		{ 
			availableTime = dp->getMinimumTimeAvailableForGLCompileAndDeletePerFrame();
		}

		static unsigned int s_numFramesThatNoTimeAvailable = 0;
		static unsigned int s_maxNumFramesThatNoTimeAvailable = 10;

		if (s_numFramesThatNoTimeAvailable>s_maxNumFramesThatNoTimeAvailable)
		{
			availableTime = 0.0025; // 2.5ms.
		}

		if (availableTime>0.0)
		{
			s_numFramesThatNoTimeAvailable = 0;

			// CRCore::notify(CRCore::NOTICE)<<"Time available = "<<availableTime<<std::endl;

			if(dp->requiresCompile())
				dp->compile(*(getSceneView()->getCurrentRenderState()),availableTime);
			// flush deleted GL objects.
			getSceneView()->flushDeletedObjects(availableTime/*,getSceneView()->getCurrentRenderState()*/);
		}
		else
		{
			++s_numFramesThatNoTimeAvailable;
		}
#else    
		double timeForPreviousFrame = CRCore::Timer::instance()->delta_s(m_previousFrameStartTick, m_frameStartTick);
		double timeForCullAndDraw = CRCore::Timer::instance()->delta_s(m_frameStartTick, CRCore::Timer::instance()->tick());

		double minimumTargetMaxFrameTime = 0.010; // 10ms.
		double targetMaxFrameTime = CRCore::minimum(timeForPreviousFrame, minimumTargetMaxFrameTime);

		double drawCostFactor = 2.0; // must be greater than 1 to account for the extra cost of emptying the OpenGL fifo.
		double frameFactor = 0.9; // must be less than 1, to compensate for extra time spent in update and swap buffers etc.
		double timeLeftTillEndOfFrame = targetMaxFrameTime*frameFactor - timeForCullAndDraw*drawCostFactor;
		double availableTime = timeLeftTillEndOfFrame / drawCostFactor; // account for the fifo when download texture objects.

		static unsigned int s_numFramesThatNoTimeAvailable = 0;
		static unsigned int s_maxNumFramesThatNoTimeAvailable = 10;

		if (s_numFramesThatNoTimeAvailable>s_maxNumFramesThatNoTimeAvailable)
		{
			availableTime = 0.0025; // 2.5ms.
		}

		if (availableTime>0.0)
		{
			s_numFramesThatNoTimeAvailable = 0;

			// CRCore::notify(CRCore::NOTICE)<<"Time available = "<<availableTime<<std::endl;

			if(dp->requiresCompile())
				dp->compile(*(getSceneView()->getCurrentRenderState()),availableTime);

			// flush deleted GL objects.
			getSceneView()->flushDeletedObjects(availableTime);
		}
		else
		{
			++s_numFramesThatNoTimeAvailable;
		}
#endif
		dp->signalEndFrame();
	}
	//CRCore::notify(CRCore::FATAL)<<"crSceneHandler::drawImplementation 1"<<std::endl;
}

}

#endif
