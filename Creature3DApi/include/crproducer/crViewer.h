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
#ifndef CRPRODUCER_CRVIEWER_H
#define CRPRODUCER_CRVIEWER_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crAnimationPath.h>
#include <CRCore/RefNodePath.h>
#include <CRCore/crIntersectVisitor.h>

#include <CRProducer/crCameraGroup.h>
#include <CRProducer/crKeyboardMouseCallback.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/avi_utils.h>
#include <CRCore/crEventGroup.h>
#include <CRPhysics/crCamera.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crDatabasePager.h>
#include <CRCore/crFilterRenderManager.h>
#include <CRText/crText.h>
#include <CRSound/crSoundSystem.h>
//#include <CRUtil/crAISystemUpdater.h>

#include <list>

namespace CRProducer {
class crSceneHandler;
class crKeyboardMouseHandle;
class CRPRODUCER_EXPORT crViewer : public crCameraGroup/*, public CRGA::crGUIActionAdapter*/
{
friend class crKeyboardMouseHandle;
    public :

        crViewer();

        crViewer(Producer::CameraConfig *cfg);

        virtual ~crViewer();


        enum ViewerOptions
        {
			NO_EVENT_HANDLERS       = 0x00,
			HEAD_LIGHT_SOURCE       = 0x01,
			SKY_LIGHT_SOURCE        = 0x02,
			ESCAPE_SETS_DONE        = 0x04,
			CURSOR_NULL             = 0x08,
			STANDARD_SETTINGS       = CURSOR_NULL
        };
        
        void setUpViewer(unsigned int options = STANDARD_SETTINGS);
        
		void setDoneAtElapsedTime(double elapsedTime);
		double getDoneAtElapsedTime() const;
		void setDoneAtElapsedTimeEnabled(bool enabled);
		bool getDoneAtElapsedTimeEnabled() const;
		
		void setDoneAtFrameNumber(unsigned int frameNumber);
		unsigned int getDoneAtFrameNumber() const;
		void setDoneAtFrameNumberEnabled(bool enabled);
		bool getDoneAtFrameNumberEnabled() const;


        void setDone(bool done);
        bool getDone() const;

        /** return true if the application is done and should exit.*/
        virtual bool done();

		void setWriteImageWhenDone(bool enabled);
		bool getWriteImageWhenDone() const;

        /** Override the Producer::CameraGroup::setViewByMatrix to catch all changes to view.*/
        virtual void setViewByMatrix( const Producer::Matrix & pm);

        /** Set the threading model and then call realize().*/
        virtual bool realize(ThreadingModel thread_model,Producer::Window parentWnd,Producer::Window renderWnd);

        virtual bool realize(Producer::Window parentWnd,Producer::Window renderWnd);
		
		Producer::Window getCameraWindow();
		virtual void cancelCameraThreads();

        /** Updated the scene.  Handle any queued up events, do an update traversal and set the CameraGroup's setViewByMatrix if any camera manipulators are active.*/
        inline virtual void update();
        
        /** set the update visitor which does the update traversal of the scene graph. Automatically called by the update() method.*/
        void setUpdateVisitor(CRCore::crNodeVisitor* nv);
        
        /** get the update visitor.*/
        CRCore::crNodeVisitor* getUpdateVisitor();
        
        /** get the const update visitor.*/
        const CRCore::crNodeVisitor* getUpdateVisitor() const;
       
		void computeActiveCoordindateSystemNodePath();

		void setCoordindateSystemNodePath(const CRCore::RefNodePath& nodePath);

		void setCoordindateSystemNodePath(const CRCore::NodePath& nodePath);

		const CRCore::RefNodePath& getCoordindateSystemNodePath() const;

        /** Dispatch the draw for each of the Camera's for this frame.*/
        virtual void frame();

        //virtual void requestRedraw();
        //virtual void requestContinuousUpdate(bool);
        //virtual void requestWarpPointer(float x,float y);


        /** compute, from normalized mouse coords, for sepecified Camera, the pixel coords relative to that Camera's RenderSurface.*/
        bool computePixelCoords(float x,float y,unsigned int cameraNum,float& pixel_x,float& pixel_y);

        /** compute, from normalized mouse coords, for sepecified Camera, the near and far points in worlds coords.*/
        bool computeNearFarPoints(float x,float y,unsigned int cameraNum,CRCore::crVector3& near, CRCore::crVector3& far);
        
        /** compute, from normalized mouse coords, for all Cameras, intersections with the specified subgraph.*/
        bool computeIntersections(float x,float y,unsigned int cameraNum,CRCore::crNode *node,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask = 0xffffffff);

        /** compute, from normalized mouse coords, for sepecified Camera, intersections with the scene.*/
        bool computeIntersections(float x,float y,unsigned int cameraNum,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask = 0xffffffff);

        /** compute, from normalized mouse coords, for all Cameras, intersections with specified subgraph.*/
        inline bool computeIntersections(float x,float y,CRCore::crNode *node,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask = 0xffffffff)
		{
			//m_camera->setEnableIntersect(false);
			bool hitFound = false;
			for(unsigned int i=0;i<getNumberOfCameras();++i)
			{
				if (computeIntersections(x,y,i,node,hits,traversalMask)) 
				{
					hitFound = true;
					break;
				}
			}
			//m_camera->setEnableIntersect(true);
			return hitFound;
		}
        /** compute, from normalized mouse coords, for all Cameras, intersections with the scene.*/
        inline bool computeIntersections(float x,float y,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask = 0xffffffff)
		{
			return computeIntersections(x,y,getSceneData(),hits,traversalMask);
		}
        
		CRCore::crVector3 computePickPoint(float x,float y,CRCore::crNode::NodeMask traversalMask = 0xffffffff);

        void setKeyboardMouse(Producer::KeyboardMouse* kbm);
        inline Producer::KeyboardMouse* getKeyboardMouse() { return m_kbm.get(); }
        inline const Producer::KeyboardMouse* getKeyboardMouse() const { return m_kbm.get(); }

        void setKeyboardMouseCallback(CRProducer::crKeyboardMouseCallback* kbmcb);
        CRProducer::crKeyboardMouseCallback* getKeyboardMouseCallback();
        const CRProducer::crKeyboardMouseCallback* getKeyboardMouseCallback() const;

        //void setRecordingAnimationPath(bool on);
        //inline bool getRecordingAnimationPath() const { return m_recordingAnimationPath; }

        //void setAnimationPath(CRCore::crAnimationPath* path);
        //inline CRCore::crAnimationPath* getAnimationPath() { return m_animationPath.get(); }
        //inline const CRCore::crAnimationPath* getAnimationPath() const { return m_animationPath.get(); }

		//CRCore::crVector3d getPosition() const;
        //double getSpeed() const { return m_speed; }
        //CRCore::crQuat getOrientation() const;
        
        //virtual void setParentWindow( Producer::Window window );
        
		//virtual void setVelocity( float velocity );
		//virtual float getVelocity()const;

		virtual void updatedSceneData();

		//void setViewEventNode( CRCore::crEventGroup *node);
		//const CRCore::crEventGroup *getViewEventNode()const;

		void setViewCameraNode( CRPhysics::crCamera *node);
		CRPhysics::crCamera *getViewCameraNode();

		//void setAnimationPathControl(bool bln);
		//bool getAnimationPathControl()const;

		class CRPRODUCER_EXPORT SnapImageDrawCallback : public Producer::Camera::Callback 
		{
		public:

			SnapImageDrawCallback(const std::string& filename);

			void setSnapImageOnNextFrame(bool flag);
			bool getSnapImageOnNextFrame() const;

			void setFileName(const std::string& filename);
			virtual void operator()( const Producer::Camera & camera);
			static void setIndex( int index );
		protected:
			std::string m_filename;
			bool        m_snapImageOnNextFrame;
			static int m_index;
		};
        void snapImageOnNextFrame();

		//class CRPRODUCER_EXPORT MakeAviDrawCallback : public Producer::Camera::Callback 
		//{
		//public:

		//	MakeAviDrawCallback(/*Producer::Window hwin*/);

		//	void record();

		//	void setFileName(const std::string& filename);
		//	virtual void operator()( const Producer::Camera & camera);
		//	static void setIndex( int index );
		//protected:
		//	virtual ~MakeAviDrawCallback();
		//	std::string m_filename;
		//	int        m_makeAviOnNextFrame;
		//	static int m_index;
		//	HAVI m_avi;
		//};
		//void record();

		//class CRPRODUCER_EXPORT StatsUpdateCallback : public Producer::CameraGroup::StatsHandler
		//{
		//public:
		//	StatsUpdateCallback(CRCore::crFrameStamp *fs);
		//	virtual void operator() (const Producer::CameraGroup &cg );
		//	void setShowMode(int mode);
		//protected:
		//	int m_showMode;
		//	//Producer::CameraGroup::FrameStats m_fs[2];
		//	//float m_timePerFrameCount;
		//	CRCore::ref_ptr<CRCore::crFrameStamp> m_frameStamp;
		//	CRCore::ref_ptr<CRText::crText> m_frameRateLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_frameRateCounterText;
		//	CRCore::ref_ptr<CRText::crText> m_frameTimeLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_frameTimeText;

		//	CRCore::ref_ptr<CRText::crText> m_drawLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_drawTimeText;
		//	CRCore::ref_ptr<CRText::crText> m_referDrawLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_referDrawTimeText;

		//	CRCore::ref_ptr<CRText::crText> m_stateLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_stateCounterText;
		//	CRCore::ref_ptr<CRText::crText> m_drawableLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_drawableCounterText;

		//	CRCore::ref_ptr<CRText::crText> m_vertexLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_vertexCounterText;
		//	CRCore::ref_ptr<CRText::crText> m_primLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_primCounterText;

		//	CRCore::ref_ptr<CRText::crText> m_particleLabelText;
		//	CRCore::ref_ptr<CRText::crText> m_particleCounterText;
		//};
		//void showStats(int show);
        
		typedef std::map<std::string, Producer::Cursor> CursorMap;
		void insertCursor(const std::string &cursorName, Producer::Cursor cursor);
		void hideCursor(bool capture = true);
		void showCursor(const std::string &cursorName);
		std::string getCurrentCursorName();
		bool isCursorOnShow();

		bool beSafeToExit();

		inline unsigned int getViewOptions() const {return m_options;}

		void resetPointer();//将鼠标置于屏幕中心
    protected :

		bool            m_setDoneAtElapsedTimeEnabled;
		double          m_setDoneAtElapsedTime;

		bool            m_setDoneAtFrameNumberEnabled;
		unsigned int    m_setDoneAtFrameNumber;

        bool            m_done;

		bool            m_writeImageWhenDone;

        CRCore::ref_ptr<Producer::KeyboardMouse> m_kbm;

        CRCore::ref_ptr<CRProducer::crKeyboardMouseCallback> m_kbmcb;

        CRCore::ref_ptr<CRCore::crNodeVisitor>      m_updateVisitor;

		CRCore::RefNodePath                    m_coordinateSystemNodePath;

  //      bool                                m_recordingAnimationPath;
		//double                              m_recordingStartTime;
  //      CRCore::ref_ptr<CRCore::crAnimationPath>    m_animationPath;
        // record the current position and orientation of the view.
        //double                              m_position[3];
		//CRCore::crVector3                   m_position;
		//CRCore::crVector3                   m_center;
		//bool                                m_animationPathControl;
        //double                              m_speed;

		CRCore::ref_ptr<CRPhysics::crCamera>       m_viewCameraNode;
		//CRCore::ref_ptr<CRCore::crEventGroup>            m_viewEventGroup;

		//CRProducer::crKeyboardMouseCallback::EventQueue  m_queue;

		unsigned int         m_options;//当前视图的ViewerOptions参数

	    CRCore::ref_ptr<CRProducer::crEventAdapter> m_frame_event;
		CRCore::crIntersectVisitor               m_iv;
		CRCore::ref_ptr<CRCore::crLineSegment>   m_lineSegment;
		CRCore::crMatrixd                        m_viewMatrix;

		Producer::ref_ptr<SnapImageDrawCallback> m_snapImageDrawCallback;
		//Producer::ref_ptr<MakeAviDrawCallback> m_makeAviDrawCallback;
		//Producer::ref_ptr<StatsUpdateCallback> m_statsUpdateCallback;

        CursorMap m_cursormap;
		std::string m_currentCursorName;
		float m_mouseInterval;
};

#ifdef __APPLE__
#define SINGLE_THREAD_KEYBOARDMOUSE
#endif

inline void crViewer::update()
{
	//try
	//{
	//CRUtil::crAISystemUpdater::getInstance()->aiUpdateBlock();
	int frameNum = CRCore::crFrameStamp::getInstance()->increaseFrameNumber();
	CRCore::crFrameStamp::getInstance()->setReferenceTime(m_timer.delta_s(m_start_tick,m_timer.tick()));
	CRCore::crFrameStamp::getInstance()->update();

//try
//{
#ifdef SINGLE_THREAD_KEYBOARDMOUSE
	if (m_kbm.valid() && !m_kbm->isRunning()) m_kbm->update(*(m_kbm->getCallback()));
#endif
	//if (m_kbmcb.valid()) 
	//{
	//	m_kbmcb->getEventQueue(m_queue);
	//}

	//m_frame_event->adaptFrame(m_frameStamp->getReferenceTime());
	//m_queue.push_back(m_frame_event);
	//if(/*!m_animationPathControl &&*/ m_viewEventGroup.valid())
	//{
	//	m_viewEventGroup->acceptEventMessage(WCH_KEYBOARDMOUSE,MAKEINT64(this,NULL),MAKEINT64(&m_queue,m_frameStamp->getFrameNumber()));
	//}
	if(m_viewCameraNode.valid() && frameNum>INITFRAMENUMBER)
	{
		//for( CRProducer::crKeyboardMouseCallback::EventQueue::iterator event_itr=m_queue.begin();
		//	event_itr!=m_queue.end();
		//	++event_itr )
		//{
		//	crKeyboardMouseHandle::getInstance()->handle(event_itr->get());
		//}
		//m_viewMatrix = m_viewCameraNode->getCameraMatrix();
		//CRCore::notify(CRCore::ALWAYS)<<m_frameStamp->getFrameNumber()<<"crViewer::update(): m_position = "<<m_position<<std::endl;
		m_viewMatrix = m_viewCameraNode->getCameraMatrix();
		//CRCore::notify(CRCore::ALWAYS)<<m_frameStamp->getFrameNumber()<<"crViewer::update(): m_position = "<<m_position<<std::endl;
		CameraGroup::setViewByMatrix(Producer::Matrix(m_viewMatrix.ptr()));
		CRSound::crSoundSystem::getInstance()->update(m_viewMatrix,CRCore::crFrameStamp::getInstance()->getFrameInterval());
		//setFusionDistance();
		//if (getRecordingAnimationPath() && getAnimationPath())
		//{
		//	if (getAnimationPath()->empty()) m_recordingStartTime = m_frameStamp->getReferenceTime(); 
		//	CRCore::crQuat orientation;
		//	m_viewMatrix.get(orientation);
		//	getAnimationPath()->insert(m_frameStamp->getReferenceTime(),CRCore::crAnimationPath::ControlPoint(m_viewMatrix.getTrans(),orientation));
		//}
	}
	else
	{
		m_viewMatrix = CRCore::crMatrix::lookAt(CRCore::crVector3(0.0f,0.0f,-1000000.0f),CRCore::crVector3(0.0f,0.0f,-1000001.0f),CRCore::Z_AXIS);
		CameraGroup::setViewByMatrix(Producer::Matrix(m_viewMatrix.ptr()));
	}
	if(m_options&CURSOR_NULL)
	{
		m_mouseInterval += CRCore::crFrameStamp::getInstance()->getFrameInterval();
		if(m_mouseInterval > 0.02f)/*m_frameStamp->getFrameNumber()%3==0*/
		{
			m_mouseInterval = 0.0f;
			resetPointer();
		}
		//m_viewerEventHandler->setDisplayPost(getCamera(0)->getRenderSurface()->getFocuse());
	}
	//CRCore::crNode* node = getTopMostSceneData();
	//if (node) node->getBound();
//}
//catch (...)
//{
//	CRCore::notify(CRCore::ALWAYS)<<"crViewer::update() error"<<std::endl;
//}
}
}

#endif
