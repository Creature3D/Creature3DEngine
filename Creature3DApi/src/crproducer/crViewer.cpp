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
#include <CRCore/crLightSource.h>
#include <CRCore/crCoordinateSystemNode.h>
#include <CRCore/crGroup.h>

#include <CRUtil/crUpdateVisitor.h>

#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileUtils.h>

#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>

#include <CRCore/crMatrixTransform.h>
#include <CRCore/crMath.h>
#include <CRIOManager/crWriteFile.h>
#include <CRCore/crStatistics.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRPhysics/crCamera.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <iterator>
using namespace Producer;
using namespace CRProducer;
using namespace CRCore;
using namespace CRUtil;
using namespace CRParticle;
using namespace CRPhysics;
//////////////////////////////////////////////////////////////////////////////

class CollectedCoordinateSystemNodesVisitor : public CRCore::crNodeVisitor
{
public:

	CollectedCoordinateSystemNodesVisitor():
	  crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN) {}


	  virtual void apply(CRCore::crNode& node)
	  {
		  traverse(node);
	  }

	  virtual void apply(CRCore::crCoordinateSystemNode& node)
	  {
		  if (m_pathToCoordinateSystemNode.empty())
		  {
			  //CRCore::notify(CRCore::INFO)<<"Found CoordianteSystemNode node"<<std::endl;
			  //CRCore::notify(CRCore::INFO)<<"     CoordinateSystem = "<<node.getCoordinateSystem()<<std::endl;
			  m_pathToCoordinateSystemNode = getNodePath();
		  }
		  //else
		  //{
			 // CRCore::notify(CRCore::INFO)<<"Found additional CoordianteSystemNode node, but ignoring"<<std::endl;
			 // CRCore::notify(CRCore::INFO)<<"     CoordinateSystem = "<<node.getCoordinateSystem()<<std::endl;
		  //}
		  traverse(node);
	  }

	  NodePath m_pathToCoordinateSystemNode;
};



//class ViewerCoordinateFrameCallback : public CRGA::crMatrixManipulator::CoordinateFrameCallback
//{
//public:
//
//	ViewerCoordinateFrameCallback(crViewer* viewer):
//	  m_viewer(viewer) {}
//
//
//	  virtual CRCore::CoordinateFrame getCoordinateFrame(const CRCore::crVector3d& position) const
//	  {
//		  CRCore::notify(CRCore::INFO)<<"getCoordinateFrame("<<position<<")"<<std::endl;
//
//		  // do automatic conversion between RefNodePath and NodePath.
//		  CRCore::NodePath tmpPath = m_viewer->getCoordindateSystemNodePath();
//
//		  if (!tmpPath.empty())
//		  {        
//			  CRCore::crMatrixd coordinateFrame;
//
//			  CRCore::crCoordinateSystemNode* csn = dynamic_cast<CRCore::crCoordinateSystemNode*>(tmpPath.back());
//			  if (csn)
//			  {
//				  CRCore::crVector3f local_position = position*CRCore::computeWorldToLocal(tmpPath);
//
//				  // get the coordinate frame in world coords.
//				  coordinateFrame = csn->computeLocalCoordinateFrame(local_position)* CRCore::computeLocalToWorld(tmpPath);
//
//				  // keep the position of the coordinate frame to reapply after rescale.
//				  CRCore::crVector3d pos = coordinateFrame.getTrans();
//
//				  // compensate for any scaling, so that the coordinate frame is a unit size
//				  CRCore::crVector3d x(1.0,0.0,0.0);
//				  CRCore::crVector3d y(0.0,1.0,0.0);
//				  CRCore::crVector3d z(0.0,0.0,1.0);
//				  x = CRCore::crMatrixd::transform3x3(x,coordinateFrame);
//				  y = CRCore::crMatrixd::transform3x3(y,coordinateFrame);
//				  z = CRCore::crMatrixd::transform3x3(z,coordinateFrame);
//				  coordinateFrame.preMult(CRCore::crMatrixd::scale(1.0/x.length(),1.0/y.length(),1.0/z.length()));
//
//				  // reapply the position.
//				  coordinateFrame.setTrans(pos);
//
//				  CRCore::notify(CRCore::INFO)<<"csn->computeLocalCoordinateFrame(position)* CRCore::computeLocalToWorld(tmpPath)"<<coordinateFrame<<std::endl;
//
//			  }
//			  else
//			  {
//				  CRCore::notify(CRCore::INFO)<<"CRCore::computeLocalToWorld(tmpPath)"<<std::endl;
//				  coordinateFrame =  CRCore::computeLocalToWorld(tmpPath);
//			  }
//			  return coordinateFrame;
//		  }
//		  else
//		  {
//			  CRCore::notify(CRCore::INFO)<<"   no coordinate system found, using default orientation"<<std::endl;
//			  return CRCore::crMatrixd::translate(position);
//		  }
//	  }
//
//protected:
//	virtual ~ViewerCoordinateFrameCallback() {}
//	crViewer* m_viewer;
//};

//////////////////////////////////////////////////////////////////////////////
//
// CRProducer::crViewer implemention
//
crViewer::crViewer():
    m_setDoneAtElapsedTimeEnabled(false),
    m_setDoneAtElapsedTime(0.0),
    m_setDoneAtFrameNumberEnabled(false),
    m_setDoneAtFrameNumber(0),
    m_done(false),
    m_writeImageWhenDone(false),
    //m_recordingAnimationPath(false),
    //m_recordingStartTime(0.0),
	m_options(STANDARD_SETTINGS),
	m_mouseInterval(0.0f)
	//m_animationPathControl(false)
{
	m_lineSegment = new CRCore::crLineSegment;
    m_frame_event = new CRProducer::crEventAdapter;
}

crViewer::crViewer(Producer::CameraConfig *cfg):
    crCameraGroup(cfg),
	m_setDoneAtElapsedTimeEnabled(false),
	m_setDoneAtElapsedTime(0.0),
	m_setDoneAtFrameNumberEnabled(false),
	m_setDoneAtFrameNumber(0),
	m_done(false),
	m_writeImageWhenDone(false),
	//m_recordingAnimationPath(false),
	//m_recordingStartTime(0.0),
	m_options(STANDARD_SETTINGS),
	m_mouseInterval(0.0f)
	//m_animationPathControl(false)
{
	m_lineSegment = new CRCore::crLineSegment;
	m_frame_event = new CRProducer::crEventAdapter;
}

crViewer::~crViewer()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crViewer()"<< std::endl;
}
void crViewer::setCoordindateSystemNodePath(const CRCore::RefNodePath& nodePath) { m_coordinateSystemNodePath = nodePath; }

void crViewer::setCoordindateSystemNodePath(const CRCore::NodePath& nodePath)
{
	m_coordinateSystemNodePath.clear();
	std::copy(nodePath.begin(),
		nodePath.end(),
		std::back_inserter(m_coordinateSystemNodePath));
}

void crViewer::computeActiveCoordindateSystemNodePath()
{
	// now search for crCoordinateSystemNode's for which we want to track.
	CRCore::crNode* subgraph = getTopMostSceneData();

	if (subgraph)
	{
		CollectedCoordinateSystemNodesVisitor ccsnv;
		subgraph->accept(ccsnv);

		if (!ccsnv.m_pathToCoordinateSystemNode.empty())
		{
			setCoordindateSystemNodePath(ccsnv.m_pathToCoordinateSystemNode);
			return;
		}
	}  
	// otherwise no node path found so reset to empty.
	setCoordindateSystemNodePath(CRCore::NodePath());
}

const CRCore::RefNodePath& crViewer::getCoordindateSystemNodePath() const { return m_coordinateSystemNodePath; }

void crViewer::updatedSceneData()
{
	crCameraGroup::updatedSceneData();
	// refresh the coordinate system node path.
	computeActiveCoordindateSystemNodePath();
}

void crViewer::setKeyboardMouse(Producer::KeyboardMouse* kbm)
{
    m_kbm = kbm;
    if (m_kbm.valid() && m_kbmcb.valid()) m_kbm->setCallback(m_kbmcb.get());
}

void crViewer::setKeyboardMouseCallback(CRProducer::crKeyboardMouseCallback* kbmcb)
{
    m_kbmcb = kbmcb;
    if (m_kbm.valid() && m_kbmcb.valid()) m_kbm->setCallback(m_kbmcb.get());
}

CRProducer::crKeyboardMouseCallback* crViewer::getKeyboardMouseCallback() { return m_kbmcb.get(); }
const CRProducer::crKeyboardMouseCallback* crViewer::getKeyboardMouseCallback() const { return m_kbmcb.get(); }

void crViewer::setUpViewer(unsigned int options)
{
	m_start_tick = m_timer.tick();
    // set up the keyboard and mouse handling.
    Producer::InputArea *ia = getCameraConfig()->getInputArea();
    
    if (!m_kbm)
    {
        m_kbm = ia ?
                   (new Producer::KeyboardMouse(ia)) : 
                   (new Producer::KeyboardMouse(getCamera(0)->getRenderSurface()));
                   
    }
    
    // set the keyboard mouse callback to catch the events from the windows.
    if (!m_kbmcb)
        m_kbmcb = new CRProducer::crKeyboardMouseCallback( m_kbm.get(), m_done, (options & ESCAPE_SETS_DONE)!=0 );
        
    m_kbmcb->setStartTick(m_start_tick);
    
    // register the callback with the keyboard mouse manger.
    m_kbm->setCallback( m_kbmcb.get() );
    //kbm->allowContinuousMouseMotionUpdate(true);

    //setSceneDecorator( new CRCore::crGroup );

    if (!m_updateVisitor) m_updateVisitor = new CRUtil::crUpdateVisitor;
    
	crFrameStamp::getInstance()->setFrameNumber(0);
	crFrameStamp::getInstance()->setReferenceTime(0.0f);
    //m_updateVisitor->setFrameStamp(m_frameStamp.get());
	//CRCore::crBrain::getInstance()->setFrameStamp(m_frameStamp.get());

	m_options = options;
}

void crViewer::setDoneAtElapsedTime(double elapsedTime) { m_setDoneAtElapsedTimeEnabled = true; m_setDoneAtElapsedTime = elapsedTime; }
double crViewer::getDoneAtElapsedTime() const { return m_setDoneAtElapsedTime; }
void crViewer::setDoneAtElapsedTimeEnabled(bool enabled) { m_setDoneAtElapsedTimeEnabled = enabled; }
bool crViewer::getDoneAtElapsedTimeEnabled() const { return m_setDoneAtElapsedTimeEnabled; }

void crViewer::setDoneAtFrameNumber(unsigned int frameNumber) { m_setDoneAtFrameNumberEnabled = true; m_setDoneAtFrameNumber = frameNumber; }
unsigned int crViewer::getDoneAtFrameNumber() const { return m_setDoneAtFrameNumber; }
void crViewer::setDoneAtFrameNumberEnabled(bool enabled) { m_setDoneAtFrameNumberEnabled = enabled; }
bool crViewer::getDoneAtFrameNumberEnabled() const { return m_setDoneAtFrameNumberEnabled; }

void crViewer::setDone(bool done) { if(done) m_done = done; } 
bool crViewer::getDone() const { return m_done; } 

bool crViewer::done()
{
	if(m_done || 
		!validForRendering() || 
		(m_setDoneAtElapsedTimeEnabled && m_setDoneAtElapsedTime<=CRCore::crFrameStamp::getInstance()->getReferenceTime()) ||
		(m_setDoneAtFrameNumberEnabled && m_setDoneAtFrameNumber<=CRCore::crFrameStamp::getInstance()->getFrameNumber()))
	{
	/*	if(_thread_model == ThreadPerCameraEx)
		{
		    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
			{
			    _cfg->getCamera(i)->setDone(true);
			}
		}*/
		return true;
	}
	return false;
}

void crViewer::setWriteImageWhenDone(bool enabled) { m_writeImageWhenDone = enabled; }
bool crViewer::getWriteImageWhenDone() const { return m_writeImageWhenDone; }

//void crViewer::setParentWindow( Producer::Window window )
//{
//	for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
//	{
//		_cfg->getCamera(i)->getRenderSurface()->setParentWindow(window);
//	}
//}

void crViewer::setViewByMatrix( const Producer::Matrix & pm)
{
    CameraGroup::setViewByMatrix(pm);
}

bool crViewer::realize( ThreadingModel thread_model, Producer::Window parentWnd,Producer::Window renderWnd )
{
	if (_realized) 
	{
		for(SceneHandlerList::iterator p=m_shvec.begin(); p!=m_shvec.end(); p++ )
		{
			(*p)->getSceneView()->init();
		}
		return _realized;
	}

    _thread_model = thread_model;
    return realize(parentWnd,renderWnd);
}

bool crViewer::realize(Producer::Window parentWnd, Producer::Window renderWnd)
{
    if (_realized) 
	{
		for(SceneHandlerList::iterator p=m_shvec.begin(); p!=m_shvec.end(); p++ )
		{
			(*p)->getSceneView()->init();
		}
		return _realized;
	}

    crCameraGroup::realize(parentWnd,renderWnd);

	//Window hwnd = getCameraWindow();
	//RECT rect;
	//GetClientRect(hwnd,&rect);
	//int x, y;
	//unsigned int width, height;
	//getCamera(0)->getRenderSurface()->getWindowRectangle(x,y,width,height);
	//crDisplaySettings::instance()->setScreenWidth(width);
	//crDisplaySettings::instance()->setScreenHeight(height);

    // kick start the keyboard mouse if needed.
    if (m_kbm.valid() && !m_kbm->isRunning()) m_kbm->startThread();

    // by default set up the crDatabasePager.
    {    
        CRIOManager::crDatabasePager* databasePager = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
        databasePager->registerPagedLODs(getTopMostSceneData());
		databasePager->registerOptimizerData(getTopMostSceneData());

        for(SceneHandlerList::iterator p=m_shvec.begin();
            p!=m_shvec.end();
            ++p)
        {
            // pass the database pager to the cull visitor so node can send requests to the pager.
            (*p)->getSceneView()->getCullVisitor()->setDatabaseRequestHandler(databasePager);
           
            // tell the database pager which graphic context the compile of rendering objexts is needed.
            databasePager->setCompileForContextID((*p)->getSceneView()->getCurrentRenderState()->getContextID(),true);
            databasePager->setPhysicsInitVisitor((*p)->getSceneView()->getPhysicsInitVisitor());
        }
    
        // set up a post swap callback to flush deleted GL objects and compile new GL objects            
        for(unsigned int cameraNum=0;cameraNum<getNumberOfCameras();++cameraNum)
        {
            Producer::Camera* camera=getCamera(cameraNum);
			if(m_options&CURSOR_NULL)
			    camera->getRenderSurface()->useCursor(false);
        }

    }

	m_snapImageDrawCallback = new SnapImageDrawCallback("image.jpg");
	//m_makeAviDrawCallback = new MakeAviDrawCallback(/*_cfg->getCamera(0)->getRenderSurface()->getWindow()*/);
	_cfg->getCamera(0)->addPostDrawCallback(m_snapImageDrawCallback.get());
	//_cfg->getCamera(0)->addPostSwapCallback(m_makeAviDrawCallback.get());

	char filename[256];// = "output/photos/image_1.jpg";
	int i = 1;
	while(1)
	{
		sprintf(filename,"output/photos/image_%d.jpg\0",i);
		if(CRIOManager::fileExists(filename))
		{
			i++;
		}
		else
		{
			break;
		}
	}
	SnapImageDrawCallback::setIndex(i);

	//i = 1;
	//while(1)
	//{
	//	sprintf(filename,"output/movie/movie_%d.avi\0",i);
	//	if(CRIOManager::fileExists(filename))
	//	{
	//		i++;
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
	//MakeAviDrawCallback::setIndex(i);

	//m_statsUpdateCallback = new StatsUpdateCallback(m_frameStamp.get());
	//setStatsHandler(m_statsUpdateCallback.get());

    // force a sync before we intialize the keyswitch manipulator to home
    // so that Producer has a chance to set up the windows before we do
    // any work on them.
    crCameraGroup::sync();

    // set up CRCore::crState objects with the m_done prt to allow early termination of 
    // draw traversal.
    //for(SceneHandlerList::iterator p=m_shvec.begin(); p!=m_shvec.end(); p++ )
    //{
    //    (*p)->getSceneView()->setAbortRenderingPtr(&m_done);
    //}

    return _realized;
}
Producer::Window crViewer::getCameraWindow()
{
	return getCamera(0)->getRenderSurface()->getWindow();
}
void crViewer::cancelCameraThreads()
{
	CameraGroup::cancelCameraThreads();
	//clearData();
	m_scene_data = NULL;
	//m_frameNumber = 0;
	m_initialized = false;
	_realized = false;
	m_gcList.clear();
	//m_shvec.clear();

	for( CursorMap::iterator itr = m_cursormap.begin();
		itr != m_cursormap.end();
		++itr )
	{
		DeleteObject(itr->second);
	}
	m_cursormap.clear();
}
void crViewer::setUpdateVisitor(CRCore::crNodeVisitor* nv) { m_updateVisitor = nv; }
CRCore::crNodeVisitor* crViewer::getUpdateVisitor() { return m_updateVisitor.get(); }
const CRCore::crNodeVisitor* crViewer::getUpdateVisitor() const { return m_updateVisitor.get(); }

void crViewer::hideCursor(bool capture)
{
	for(unsigned int cameraNum=0;cameraNum<getNumberOfCameras();++cameraNum)
	{
		Producer::Camera* camera=getCamera(cameraNum);
		RenderSurface* rs = camera->getRenderSurface();
		rs->useCursor(false);
	}
	
	if(capture)
	{
		m_options |= CURSOR_NULL;
		resetPointer();
	}
	else
	{
		m_options &= ~CURSOR_NULL;
	}
	//crFilterRenderManager::getInstance()->showMouse(0);
	//m_currentCursorName.clear();
	if(m_viewCameraNode.valid())
		m_viewCameraNode->getAttachedNode()->doEvent(WCH_HIDECURSOR);
}

void crViewer::insertCursor(const std::string &cursorName, Producer::Cursor cursor)
{
    m_cursormap[cursorName] = cursor;
}
void crViewer::showCursor(const std::string &cursorName)
{
	if(cursorName.empty()||cursorName == "NULL") return hideCursor();
	//if(m_currentCursorName == cursorName) return;
	CursorMap::iterator itr = m_cursormap.find(cursorName);
	if(itr != m_cursormap.end())
	{
		for(unsigned int cameraNum=0;cameraNum<getNumberOfCameras();++cameraNum)
		{
			Producer::Camera* camera=getCamera(cameraNum);
			RenderSurface* rs = camera->getRenderSurface();
			rs->useCursor(true);
			rs->setCursor(itr->second);
		}
		m_options &= ~CURSOR_NULL;
		//crFilterRenderManager::getInstance()->showMouse(-1);
		m_currentCursorName = cursorName;
	}
}

std::string crViewer::getCurrentCursorName()
{
    return m_currentCursorName;
}

bool crViewer::isCursorOnShow()
{
    return !(m_options&CURSOR_NULL)/*!m_currentCursorName.empty()*/;
}

//void crViewer::setAnimationPathControl(bool bln){ m_animationPathControl = bln; }
//bool crViewer::getAnimationPathControl()const { return m_animationPathControl; }

void crViewer::frame()
{
    // record the position of the view point.
//    CRCore::crMatrixd matrix;
//    matrix.invert(getViewMatrix());
//    matrix.get(m_orientation);
//
//    double newPosition[3];
//    newPosition[0] = matrix(3,0);
//    newPosition[1] = matrix(3,1);
//    newPosition[2] = matrix(3,2);
//    
//    m_speed = sqrtf(CRCore::square(newPosition[0]-m_position[0])+CRCore::square(newPosition[1]-m_position[1])+CRCore::square(newPosition[2]-m_position[2]));
//    m_position[0] = newPosition[0];
//    m_position[1] = newPosition[1];
//    m_position[2] = newPosition[2];
//    
//#if 1
//    CRCore::crQuat::value_type angle;
//    CRCore::crVector3 axis;
//    
//    CRCore::crQuat roll;
//    roll.makeRotate(-CRCore::PI/2.0f,1,0,0);
//    
//    m_orientation = roll*m_orientation;
//    
//    m_orientation.getRotate(angle,axis);
//    
//#endif    
//    
  //  if (getRecordingAnimationPath() && getAnimationPath())
  //  {
		//if (getAnimationPath()->empty()) m_recordingStartTime = m_frameStamp->getReferenceTime(); 
		//getAnimationPath()->insert(m_frameStamp->getReferenceTime(),CRCore::crAnimationPath::ControlPoint(CRCore::crVector3(m_position[0],m_position[1],m_position[2]),m_orientation));
  //  }
	if (done() && getWriteImageWhenDone())
	{
		snapImageOnNextFrame();
	}
    crCameraGroup::frame();
}

bool crViewer::computePixelCoords(float x,float y,unsigned int cameraNum,float& pixel_x,float& pixel_y)
{
    Producer::KeyboardMouse* km = getKeyboardMouse();
    if (!km) return false;

    if (cameraNum>=getNumberOfCameras()) return false;

    Producer::Camera* camera=getCamera(cameraNum);
    Producer::RenderSurface* rs = camera->getRenderSurface();

    if (km->computePixelCoords(x,y,rs,pixel_x,pixel_y))
    {
        int pr_wx, pr_wy;
        unsigned int pr_width, pr_height;
        camera->getProjectionRectangle( pr_wx, pr_wy, pr_width, pr_height );

        int rs_wx, rs_wy;
        unsigned int rs_width, rs_height;
        rs->getWindowRectangle( rs_wx, rs_wy, rs_width, rs_height );

        pixel_x -= (float)rs_wx;
		pixel_y -= (float)rs_wy;
		//pixel_y-=10;窗口模式鼠标偏移
		//DWORD s = GetWindowLong(rs->getWindow(), GWL_STYLE);
		//if(s&WS_CAPTION)
		//{
  //          pixel_y -= GetSystemMetrics(SM_CYSIZE);
		//}

        if (pixel_x<(float)pr_wx) return false;
        if (pixel_x>(float)(pr_wx+pr_width)) return false;

        if (pixel_y<(float)pr_wy) return false;
        if (pixel_y>(float)(pr_wy+pr_height)) return false;

        return true;
    }
    return false;
}

bool crViewer::computeNearFarPoints(float x,float y,unsigned int cameraNum,CRCore::crVector3& near_point, CRCore::crVector3& far_point)
{
    if (cameraNum>=getSceneHandlerList().size()) return false;

    crSceneHandler* scenehandler = getSceneHandlerList()[cameraNum].get();
    CRUtil::crSceneView* sv = scenehandler->getSceneView();
    
    float pixel_x,pixel_y;
    if (computePixelCoords(x,y,cameraNum,pixel_x,pixel_y))
    {
        return sv->projectWindowXYIntoObject((int)(pixel_x+0.5f),(int)(pixel_y+0.5f),near_point,far_point);
    }
    return false;

}

bool crViewer::computeIntersections(float x,float y,unsigned int cameraNum,CRCore::crNode *node,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask)
{
	//CRCore::notify(CRCore::ALWAYS)<<"computeIntersections"<< std::endl;

	float pixel_x,pixel_y;
	if (node && computePixelCoords(x,y,cameraNum,pixel_x,pixel_y))
	{

		Producer::Camera* camera=getCamera(cameraNum);

		CRProducer::crSceneHandler* sh = dynamic_cast<CRProducer::crSceneHandler*>(camera->getSceneHandler());
		CRUtil::crSceneView* sv = sh ? sh->getSceneView() : 0;
		CRCore::crMatrixd proj;
		CRCore::crMatrixd view;
		const CRCore::crViewPort* viewport = 0;
		CRCore::crNode* rootNode = 0; 
		if (sv!=0)
		{
			viewport = sv->getViewport();
			proj = *sv->getProjectionMatrix();
			view = *sv->getViewMatrix();
			rootNode = sv->getSceneData();
		}
		else
		{
			viewport = 0;
			proj = CRCore::crMatrixd(camera->getProjectionMatrix());
			view = CRCore::crMatrixd(camera->getViewMatrix());
		}

		unsigned int numHitsBefore = hits.size();
        
		CRCore::NodePathList parentNodePaths = node->getParentalNodePaths(rootNode);
		CRCore::crMatrixd modelview;
		for(unsigned int i=0;i<parentNodePaths.size();++i)
		{
			CRCore::NodePath& nodePath = parentNodePaths[i];

			// remove the intersection node from the nodePath as it'll be accounted for
			// in the PickVisitor traversal, so we don't double account for its transform.
			if (!nodePath.empty()) nodePath.pop_back();
			modelview.set(view);
			// modify the view matrix so that it accounts for this nodePath's accumulated transform
			if (!nodePath.empty()) modelview.preMult(computeLocalToWorld(nodePath));

			CRCore::crPickVisitor pick(viewport, proj, modelview, pixel_x, pixel_y);
			pick.setTraversalMask(traversalMask & ~CulledMask);

			node->accept(pick);
			if(pick.hits())
			{
				// copy all the hits across to the external hits list
				for(CRCore::crPickVisitor::LineSegmentHitListMap::iterator itr = pick.getSegHitList().begin();
					itr != pick.getSegHitList().end();
					++itr)
				{
					hits.insert(hits.end(),itr->second.begin(), itr->second.end());
				}
			}
		}
		// return true if we now have more hits than before
		return hits.size()>numHitsBefore;
	}
	return false;
}

bool crViewer::computeIntersections(float x,float y,unsigned int cameraNum,CRCore::crIntersectVisitor::HitList& hits,CRCore::crNode::NodeMask traversalMask)
{
    return computeIntersections(x,y,cameraNum,getSceneData(),hits,traversalMask);
}

CRCore::crVector3 crViewer::computePickPoint(float x,float y,CRCore::crNode::NodeMask traversalMask)
{
	//CRCore::notify(CRCore::ALWAYS)<<"computePickPoint = "<< std::endl;

	CRCore::crVector3 near_point;
	CRCore::crVector3 far_point;
	for(unsigned int i=0;i<getNumberOfCameras();++i)
	{
		if(computeNearFarPoints(x,y,i,near_point,far_point))
		{
			CRCore::crLineSegment* lineSegment = new CRCore::crLineSegment;
			lineSegment->set(near_point, far_point);
			crIntersectVisitor iv;
			//m_iv.insertAvoidIntersectNodeType("crBulletMatterObject");
			//m_iv.insertAvoidIntersectNodeType("crWeaponMatterObject");
			iv.addLineSegment(lineSegment);
			iv.setTraversalMask(traversalMask);
			iv.insertAvoidIntersectNode(m_viewCameraNode->getAttachedNode());
			getSceneData()->accept(m_iv);
			if (iv.hits())
			{

				CRCore::crIntersectVisitor::HitList& hitList = iv.getHitList(lineSegment);
				if (!hitList.empty())
				{
					//CRCore::notify(CRCore::ALWAYS)<<"hit object = "<<hitList.front().getObject()->getName()<< std::endl;

					return hitList.front().getWorldIntersectPoint();
				}
			}
			return far_point;
		}
	}
	return far_point;
}

//void crViewer::setRecordingAnimationPath(bool on) { m_recordingAnimationPath = on; }
//void crViewer::setAnimationPath(CRCore::crAnimationPath* path) { m_animationPath = path; }

//CRCore::crVector3d crViewer::getPosition() const { return m_position; }
//CRCore::crQuat crViewer::getOrientation() const { return m_orientation; }

//void crViewer::requestRedraw()
//{
//	//CRCore::notify(CRCore::INFO)<<"crViewer::requestRedraw() called"<<std::endl;
//}
//
//void crViewer::requestContinuousUpdate(bool)
//{
//	//CRCore::notify(CRCore::INFO)<<"crViewer::requestContinuousUpdate("<<flag<<") called"<<std::endl;
//}
//
//void crViewer::requestWarpPointer(float x,float y)
//{
//    if (m_kbmcb.valid())
//    {
//        //CRCore::notify(CRCore::INFO) << "requestWarpPointer x= "<<x<<" y="<<y<<std::endl;
//    
//        crEventAdapter::m_s_mx = x;
//        crEventAdapter::m_s_my = y;
//        m_kbmcb->getKeyboardMouse()->positionPointer(x,y);
//        return;
//    }   
//}

//void crViewer::setViewEventNode( CRCore::crEventGroup *node)
//{ 
//	m_viewEventGroup = node;
//}
//const CRCore::crEventGroup *crViewer::getViewEventNode()const{ return m_viewEventGroup.get(); }

void crViewer::setViewCameraNode( CRPhysics::crCamera *node)
{ 
	if(!node) return;

	m_viewCameraNode = node;
	crCameraManager::getInstance()->setCurrentCamera(node);
}
CRPhysics::crCamera *crViewer::getViewCameraNode() { return m_viewCameraNode.get(); }

int crViewer::SnapImageDrawCallback::m_index = 1;
crViewer::SnapImageDrawCallback::SnapImageDrawCallback(const std::string& filename):
m_filename(filename),
m_snapImageOnNextFrame(false)
{
}

void crViewer::SnapImageDrawCallback::setSnapImageOnNextFrame(bool flag) { m_snapImageOnNextFrame = flag; }
bool crViewer::SnapImageDrawCallback::getSnapImageOnNextFrame() const { return m_snapImageOnNextFrame; }
void crViewer::SnapImageDrawCallback::setFileName(const std::string& filename){ m_filename = filename; }
void crViewer::SnapImageDrawCallback::setIndex( int index ){ m_index = index; }

void crViewer::SnapImageDrawCallback::operator()( const Producer::Camera & camera)
{
	if (!m_snapImageOnNextFrame) return;

	int x,y;
	unsigned int width,height;
	camera.getProjectionRectangle(x,y,width,height);

	CRCore::ref_ptr<CRCore::crImage> image = new CRCore::crImage;
	image->readPixels(x,y,width,height,
		GL_RGB,GL_UNSIGNED_BYTE);

	char filename[256];
	sprintf(filename,"output/photos/image_%d.jpg\0",m_index++);
	m_filename = filename;
	CRIOManager::writeImageFile(*image,m_filename);
	m_snapImageOnNextFrame = false;
}
void crViewer::snapImageOnNextFrame()
{
	m_snapImageDrawCallback->setSnapImageOnNextFrame(true);
}
////
//int crViewer::MakeAviDrawCallback::m_index = 1;
//crViewer::MakeAviDrawCallback::MakeAviDrawCallback(/*Producer::Window hwin*/):
//    m_makeAviOnNextFrame(0),
//	m_avi(0)
//{
//}
//
//crViewer::MakeAviDrawCallback::~MakeAviDrawCallback()
//{
//}
//
//void crViewer::MakeAviDrawCallback::record() 
//{
//	if(!m_makeAviOnNextFrame)
//	{
//		char filename[256];
//		sprintf(filename,"output/movie/movie_%d.avi",m_index++);
//		m_filename = filename;
//
//		m_makeAviOnNextFrame = 1;
//	}
//	else
//	{
//		m_makeAviOnNextFrame = 0;
//	}
//
//	crFilterRenderManager::getInstance()->getDrawable("Rec")->setVisiable(m_makeAviOnNextFrame);
//}
//
//void crViewer::MakeAviDrawCallback::setFileName(const std::string& filename) { m_filename = filename; }
//void crViewer::MakeAviDrawCallback::setIndex( int index ){ m_index = index; }
//
//void crViewer::MakeAviDrawCallback::operator()( const Producer::Camera & camera)
//{
//	if (!m_makeAviOnNextFrame) 
//	{
//		if(m_avi)
//		{
//			CloseAvi(m_avi);
//			m_avi = 0;
//		}
//		return;
//	}
//
//	HDC  screenMemoryDC;
//	HBITMAP screenBmp;
//
//	HDC dc = GetDC(camera.getRenderSurface()->getWindow());
//	screenMemoryDC = CreateCompatibleDC(dc);
//	BITMAPINFO bi;
//	ZeroMemory(&bi,sizeof(bi)); 
//	BITMAPINFOHEADER &bih = bi.bmiHeader;
//	bih.biSize=sizeof(bih);
//	bih.biWidth=GetDeviceCaps(dc, HORZRES);
//	bih.biHeight=GetDeviceCaps(dc, VERTRES);
//	bih.biPlanes=1;
//	bih.biBitCount=24;
//	bih.biCompression=BI_RGB;
//	bih.biSizeImage = bih.biWidth * bih.biHeight * bih.biBitCount / 8;//((bih.biWidth*bih.biBitCount/8+3)&0xFFFFFFFC)*bih.biHeight;
//	//bih.biXPelsPerMeter=10000;
//	//bih.biYPelsPerMeter=10000;
//	//bih.biClrUsed=0;
//	//bih.biClrImportant=0;
//	void *bits;
//	screenBmp=CreateDIBSection(dc,(BITMAPINFO*)&bih,DIB_RGB_COLORS,&bits,NULL,NULL);
//	SelectObject(screenMemoryDC, screenBmp);
//	BitBlt(screenMemoryDC,0,0,bih.biWidth,bih.biHeight,dc,0,0,SRCCOPY);
//	//SetStretchBltMode(screenMemoryDC, COLORONCOLOR); 
//	//StretchBlt(screenMemoryDC,0,0,bih.biWidth,bih.biHeight,dc,0,0,GetDeviceCaps(dc, HORZRES),GetDeviceCaps(dc, VERTRES),SRCCOPY);
//
////	char buf[64];
//
//	if(m_makeAviOnNextFrame == 1)
//	{
//		m_avi = CreateAvi(m_filename.c_str(),25,NULL);
//
//		//AVICOMPRESSOPTIONS opts; 
//		//ZeroMemory(&opts,sizeof(opts));
//		//opts.fccHandler=mmioFOURCC('d','i','v','x');
//		HRESULT hr = SetAviVideoCompression(m_avi,screenBmp,NULL,true,camera.getRenderSurface()->getWindow());
//		//FormatAviMessage(hr,buf,64);
//		//CRCore::notify(CRCore::FATAL)<<"SetAviVideoCompression = "<<buf<<std::endl;
//		m_makeAviOnNextFrame++;
//	}
//
//	/*HRESULT code = */AddAviFrame(m_avi,screenBmp);
//
//	DeleteObject(screenBmp);
//	DeleteDC(screenMemoryDC);
//	ReleaseDC(camera.getRenderSurface()->getWindow(),dc);
//	//FormatAviMessage(code,buf,64);
//	//CRCore::notify(CRCore::FATAL)<<"AddAviFrame = "<<buf<<std::endl;
//}
//
//void crViewer::record()
//{
//	//m_makeAviDrawCallback->record();
//}

//crViewer::StatsUpdateCallback::StatsUpdateCallback(CRCore::crFrameStamp *fs)
//{
//	m_frameStamp = fs;
//    m_showMode = 0;
//	//m_timePerFrameCount = 0.0f;
//	m_frameRateLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("frameRateLabelText"));
//    m_frameRateCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("frameRateCounterText"));
//	//m_frameTimeLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("frameTimeLabelText"));
//	//m_frameTimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("frameTimeText"));
//
//	//m_drawLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("drawLabelText"));
//	//m_drawTimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("drawTimeText"));
//	//m_referDrawLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("referDrawLabelText"));
//	//m_referDrawTimeText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("referDrawTimeText"));
//
//	m_stateLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("stateLabelText"));
//	m_stateCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("stateCounterText"));
//	m_drawableLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("drawableLabelText"));
//	m_drawableCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("drawableCounterText"));
//
//	m_vertexLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("vertexLabelText"));
//	m_vertexCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("vertexCounterText"));
//	m_primLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("primLabelText"));
//	m_primCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("primCounterText"));
//
//	m_particleLabelText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("particleLabelText"));
//	m_particleCounterText = dynamic_cast<CRText::crText *>(crFilterRenderManager::getInstance()->getDrawable("particleCounterText"));
//}
//
//void crViewer::StatsUpdateCallback::operator() (const Producer::CameraGroup &cg )
//{
//	if(m_showMode == 3)
//	{
//		m_frameStamp->setFrameNumber(0);
//	}
//	else if(m_showMode>0 && m_frameRateCounterText.valid())
//	{
//		//m_fs[1] = cg.getFrameStats();
//		//char tmpText[32];
//		//if(m_fs[1].getFrameNumber() % 10 == 0)
//		//{
//		//	m_timePerFrameCount = (m_fs[1]._startOfFrame-m_fs[0]._startOfFrame);
//		//	if(m_timePerFrameCount>0.0f)
//		//	{
//		//		sprintf(tmpText,"%d\0",(int)(10.0f/m_timePerFrameCount)/*,crBrain::getInstance()->getFrameStamp()->getFPS()*/);
//		//		m_frameRateCounterText->setText(tmpText);
//		//	}
//		//	m_fs[0] = m_fs[1];
//		//}   
//		m_frameRateCounterText->setText(crArgumentParser::appItoa(m_frameStamp->getFPS()));
//		if(m_showMode==2)
//		{
//			//sprintf(tmpText,"%4.2f\0",1000.0 * timePerFrame);
//			//m_frameTimeText->setText(tmpText);
//
//			//double drawTimes = 0;
//			//double referDrawTimes = 0;
//			//Producer::Camera::FrameTimeStampSet fts0,fts1; 
//			//for(unsigned int i = 0; i < m_fs[0].getNumFrameTimeStampSets(); i++ )
//			//{
//			//	fts0 = m_fs[0].getFrameTimeStampSet(i);
//			//	fts1 = m_fs[1].getFrameTimeStampSet(i);
//			//	drawTimes += fts1[Producer::Camera::BeginCameraFrame] - fts0[Producer::Camera::BeginCameraFrame];
//			//	referDrawTimes += fts0[Producer::Camera::EndCameraFrame] - fts0[Producer::Camera::BeginCameraFrame];
//			//}
//			//sprintf(tmpText,"%4.2f\0",1000.0*drawTimes);
//   //         m_drawTimeText->setText(tmpText);
//
//			//sprintf(tmpText,"%4.2f\0",1000.0*referDrawTimes);
//			//m_referDrawTimeText->setText(tmpText);
//
//			///////////statistics
//			crStatistics *statistics = crStatistics::getInstance();
//			m_stateCounterText->setText(crArgumentParser::appItoa(statistics->numStates));
//			m_drawableCounterText->setText(crArgumentParser::appItoa(statistics->numDrawables));
//			m_vertexCounterText->setText(crArgumentParser::appItoa(statistics->m_vertexCount));
//			m_primCounterText->setText(crArgumentParser::appItoa(statistics->getPrimCount()));
//			m_particleCounterText->setText(crArgumentParser::appItoa(crParticleSystemUpdater::getInstance()->getDrawCount()));
//		}
//	}
//}
//
//void crViewer::StatsUpdateCallback::setShowMode(int mode)
//{
//	switch(mode) 
//	{
//	case 0:
//	case 3:
//		m_frameRateLabelText->setVisiable(false);
//		m_frameRateCounterText->setVisiable(false);
//		//m_frameTimeLabelText->setVisiable(false);
//		//m_frameTimeText->setVisiable(false);
//
//		//m_drawLabelText->setVisiable(false);
//		//m_drawTimeText->setVisiable(false);
//		//m_referDrawLabelText->setVisiable(false);
//		//m_referDrawTimeText->setVisiable(false);
//
//		m_stateLabelText->setVisiable(false);
//		m_stateCounterText->setVisiable(false);
//		m_drawableLabelText->setVisiable(false);
//		m_drawableCounterText->setVisiable(false);
//
//		m_vertexLabelText->setVisiable(false);
//		m_vertexCounterText->setVisiable(false);
//		m_primLabelText->setVisiable(false);
//		m_primCounterText->setVisiable(false);
//
//		m_particleLabelText->setVisiable(false);
//		m_particleCounterText->setVisiable(false);
//		break;
//	case 1:
//		m_frameRateLabelText->setVisiable(true);
//		m_frameRateCounterText->setVisiable(true);
//		break;
//	case 2:
//		m_frameRateLabelText->setVisiable(true);
//		m_frameRateCounterText->setVisiable(true);
//		//m_frameTimeLabelText->setVisiable(true);
//		//m_frameTimeText->setVisiable(true);
//
//		//m_drawLabelText->setVisiable(true);
//		//m_drawTimeText->setVisiable(true);
//		//m_referDrawLabelText->setVisiable(true);
//		//m_referDrawTimeText->setVisiable(true);
//
//		m_stateLabelText->setVisiable(true);
//		m_stateCounterText->setVisiable(true);
//		m_drawableLabelText->setVisiable(true);
//		m_drawableCounterText->setVisiable(true);
//
//		m_vertexLabelText->setVisiable(true);
//		m_vertexCounterText->setVisiable(true);
//		m_primLabelText->setVisiable(true);
//		m_primCounterText->setVisiable(true);
//
//		m_particleLabelText->setVisiable(true);
//		m_particleCounterText->setVisiable(true);
//		break;
//	}
//	m_showMode = mode;
//}
//
//void crViewer::showStats(int show)
//{
//	switch(show) 
//	{
//	case 0:
//		//setInstrumentationMode(false);
//		crStatistics::getInstance()->setStat(false);
//		//crFilterRenderManager::getInstance()->setVisiable(true);
//		break;
//	case 1:
//		//setInstrumentationMode(true);
//		crStatistics::getInstance()->setStat(true);
//		//crFilterRenderManager::getInstance()->setVisiable(true);
//		break;
//	case 2:
//		//setInstrumentationMode(true);
//		crStatistics::getInstance()->setStat(true);
//		//crFilterRenderManager::getInstance()->setVisiable(false);
//		break;
//	case 3:
//		crStatistics::getInstance()->setStat(false);
//		break;
//	}
//	//m_statsUpdateCallback->setShowMode(show);
//}

bool crViewer::beSafeToExit()
{
    return _updateBarrier->numThreadsCurrentlyBlocked() == _cfg->getNumberOfCameras();
}

void crViewer::resetPointer()
{
	int x,y;
	unsigned int width,height;
	//RECT rect;
	Producer::RenderSurface* rs;
	for(unsigned int cameraNum=0;cameraNum<getNumberOfCameras();++cameraNum)
	{
		rs = getCamera(cameraNum)->getRenderSurface();
		if(rs->getFocuse())
		{
			//GetClientRect(rs->getWindow(), &rect);
			//width = rect.right - rect.left;
			//height = rect.bottom - rect.top;

			rs->getWindowRectangle(x,y,width,height);
			//if(GetWindowLong(rs->getWindow(), GWL_STYLE)&WS_CAPTION)
			//{
			//	rs->positionPointer(width*0.5f,height*0.5f - GetSystemMetrics(SM_CYSIZE));
			//}
			//else
			//{
				rs->positionPointer(width*0.5f,height*0.5f);
			//}
		}
    }
}