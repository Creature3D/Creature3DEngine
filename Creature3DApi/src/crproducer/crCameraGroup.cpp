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
#include <CRCore/crTimer.h>
#include <CRCore/crNotify.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTextureRectangle.h>
//#include <CRCore/crImageStream.h>

#include <CRUtil/crDisplayRequirementsVisitor.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crReadFile.h>
#include <CRProducer/crCameraGroup.h>
#include <CRProducer/crEventAdapter.h>
#include <CRUtil/crSceneView.h>
#include <CRProducer/Producer/RenderSurface>
#include <CRProducer/crSceneHandler.h>

#include <sys/stat.h>
#include "rcfg/ConfigScriptOO.h"

using namespace Producer;
using namespace CRProducer;
using namespace CRCore;

//#define DISABLE_PHYSICS
//#define DISABLE_AI

//#ifdef _WIN32
//class crLoadingThread : public CRCore::crThread
//{
//public:
//	crLoadingThread()
//	{
//        loadcfg();
//	}
//	virtual void run()
//	{
//	    HDC bufdc = CreateCompatibleDC(m_dc);
//		HBITMAP bufbmp;
//		bufbmp = CreateCompatibleBitmap(m_dc,m_width,m_height);
//		SelectObject(bufdc,bufbmp);
//
//		HDC imageBufdc = CreateCompatibleDC(NULL);
//		m_progress = 10;
//		while (!m_loaded) 
//		{	
//		    SelectObject(imageBufdc,m_loadingImage0);
//			BitBlt(bufdc,0,0,m_width,m_height,imageBufdc,0,0,SRCCOPY);
//			SelectObject(imageBufdc,m_loadingImage1);
//            BitBlt(bufdc,m_from,0,m_progress,m_height,imageBufdc,m_from,0,SRCCOPY);
//			BitBlt(m_dc,0,0,m_width,m_height,bufdc,0,0,SRCCOPY);
//			YieldCurrentThread();
//			if(m_progress<m_length) m_progress = float(m_progress) + m_rate;
//		}
//		SelectObject(imageBufdc,m_loadingImage1);
//        BitBlt(m_dc,0,0,m_width,m_height,imageBufdc,0,0,SRCCOPY);
//		DeleteDC(bufdc);
//		DeleteDC(imageBufdc);
//	}
//	void loaded(){ m_loaded = true; }
//	void loadcfg()
//	{
//		rcfg::ConfigScript cfg_script;
//
//		struct stat buf;
//		if (stat( "script/loadingThread.cfg", &buf )) 
//		{
//			CRCore::notify(CRCore::FATAL)<<"crLoadingThread::loadingThread(): file open error"<<std::endl;
//			return;
//		}
//
//		if (!cfg_script.Open("script/loadingThread.cfg", CreScriptParserHeader))
//		{
//			CRCore::notify(CRCore::FATAL)<<"crLoadingThread::loadingThread(): "<<cfg_script.GetLastError()<<std::endl;
//			return;
//		}
//
//        if(cfg_script.Push("LoadingThread"))
//		{
//			cfg_script.Get("Image", m_loadingImage0Name);
//			cfg_script.Get("DownImage", m_loadingImage1Name);
//			cfg_script.Get("ImageWidth", m_imageWidth);
//			cfg_script.Get("ProgressFrom", m_from);
//			cfg_script.Get("ProgressLength", m_length);
//			cfg_script.Get("Rate", m_rate);
//			
//			cfg_script.Pop();
//		}
//	}
//	void init(HDC dc, int width,int height)
//	{
//		m_dc = dc;
//		m_width = width;
//		m_height = height;
//	    m_loaded = false;
//		m_loadingImage0 = (HBITMAP)::LoadImage(NULL,m_loadingImage0Name.c_str(),IMAGE_BITMAP,m_width,m_height,LR_LOADFROMFILE);
//		m_loadingImage1 = (HBITMAP)::LoadImage(NULL,m_loadingImage1Name.c_str(),IMAGE_BITMAP,m_width,m_height,LR_LOADFROMFILE);
//		float scale = float(m_width)/(float)m_imageWidth;
//	    m_from = int(float(m_from) * scale);
//        m_length = int(float(m_length) * scale);
//	}
//	void setLoadSchedule(float schedule)
//	{
//		float s = float(m_progress) / float(m_length);
//		if(s < schedule)
//		{
//			m_rate += schedule / s;
//		}
//		else if(s > schedule)
//		{
//			m_rate -= s / schedule;
//			if(m_rate<0) m_rate = 0.1;
//		}
//	}
//	virtual ~crLoadingThread()
//	{
//		DeleteObject(m_loadingImage0);
//		DeleteObject(m_loadingImage1);
//	}
//protected:
//	bool m_loaded;
//	int m_width;
//	int m_height;
//	int m_from;
//	int m_length;
//	int m_progress;
//	int m_imageWidth;
//	float m_rate;
//	HDC m_dc;
//	HBITMAP m_loadingImage0;
//	HBITMAP m_loadingImage1;
//	std::string m_loadingImage0Name;
//	std::string m_loadingImage1Name;
//
//};
//#endif
class RenderSurfaceRealizeCallback : public Producer::RenderSurface::Callback
{
public:

    RenderSurfaceRealizeCallback(crCameraGroup* cameraGroup,crSceneHandler* sceneHandler):
        m_cameraGroup(cameraGroup),
        m_sceneHandler(sceneHandler) {}
    
    virtual void operator()( const Producer::RenderSurface & rs)
    {
    
 //       CRCore::Timer timer;
 //       CRCore::Timer_t start_t = timer.tick();
 //       
	//	//CRCore::notify(CRCore::ALWAYS) << "begin init "<<std::endl;

 //       //show loading map1
	//	bool showLoadingMap = false;
	//	if (getenv("CRE_USECRELOADINGMAP"))
	//	{
	//		showLoadingMap = atoi(getenv("CRE_USECRELOADINGMAP"));
	//	}

	//	Window win = rs.getWindow();
	//	Display dc = GetDC(win);
	//	if(showLoadingMap)
	//	{
	//#ifdef _WIN32
	//		m_loadingThread.init(dc,rs.getWindowWidth(),rs.getWindowHeight());
	//		//m_loadingThread.setSchedulePriority(CRCore::crThread::THREAD_PRIORITY_LOW);
	//		
	//		m_loadingThread.start();
	//#endif
	//	}

		//try{
		
		//CRCore::notify(CRCore::ALWAYS) << "loadingThread started"<<std::endl;

/*		if (m_cameraGroup->getRealizeCallback())
        {
            (*(m_cameraGroup->getRealizeCallback()))(*m_cameraGroup,*m_sceneHandler,rs);
        }
        else if (m_sceneHandler)*/ 
		
		m_sceneHandler->init();
  //     if(showLoadingMap) m_loadingThread.setLoadSchedule(0.3f);
        //m_sceneHandler->initRender();
		//if(showLoadingMap) m_loadingThread.setLoadSchedule(0.6f);
		//m_sceneHandler->initBufRender();
		//}
		//catch (...) {
		//	CRCore::notify(CRCore::ALWAYS) << "init error"<<std::endl;
		//}
        //CRCore::Timer_t end_t = timer.tick();
        //double time = timer.delta_m(start_t,end_t);
        //CRCore::notify(CRCore::INFO) << "Time to init = "<<time<<std::endl;

		//CRCore::notify(CRCore::ALWAYS) << "init sucess "<<std::endl;

	//	if(showLoadingMap)
	//	{
	//#ifdef _WIN32
	//		//CRCore::notify(CRCore::ALWAYS) << "end loadingThread "<<std::endl;

	//		m_loadingThread.loaded();
	//		CRCore::crThread::yieldCurrentThread();//等待m_loadingThread退出
	//		while (m_loadingThread.isRunning()) 
	//		{
	//			CRCore::crThread::yieldCurrentThread();
	//		}
	//		//() m_loadingThread.detach();//可能m_loadingThread线程没有成功退出，将其与主线程分离
	//#endif
	//		//CRCore::notify(CRCore::ALWAYS) << "loadingThread ended"<<std::endl;
	//	}
 //       ReleaseDC(win,dc);

	}

    crCameraGroup* m_cameraGroup;
    crSceneHandler* m_sceneHandler;
    //crLoadingThread m_loadingThread;
};

crCameraGroup::crCameraGroup() : Producer::CameraGroup() 
{
    _init();
}

crCameraGroup::crCameraGroup(Producer::CameraConfig *cfg):
    Producer::CameraGroup(cfg) 
{
    _init();
}

crCameraGroup::~crCameraGroup()
{
	// kill the DatabasePager and associated thread if one exists.

	CRIOManager::crRegistry::instance()->setDatabasePager(0);
}

void crCameraGroup::setGraphicsContextList(GraphicsContextList& gcList) { m_gcList = gcList; }
crCameraGroup::GraphicsContextList& crCameraGroup::getGraphicsContextList() { return m_gcList;}
const crCameraGroup::GraphicsContextList& crCameraGroup::getGraphicsContextList() const { return m_gcList;}

CRCore::crGroup* crCameraGroup::getSceneDecorator() { return m_scene_decorator.get(); }
const CRCore::crGroup* crCameraGroup::getSceneDecorator() const { return m_scene_decorator.get(); }

void crCameraGroup::_init()
{
	_thread_model = ThreadPerCameraEx;
	//_thread_model = ThreadPerCamera;

	//const char* str = getenv("CREATURE3D_CAMERA_THREADING");
	//if (str)
	//{
	//	if (strcmp(str,"SingleThreaded")==0) _thread_model = SingleThreaded;
	//	else if (strcmp(str,"ThreadPerCamera")==0) _thread_model = ThreadPerCamera;
	//	else if(strcmp(str,"ThreadPerCameraEx")==0) _thread_model = ThreadPerCameraEx;
	//}

	//str = getenv("CR_SHARE_GRAPHICS_CONTEXTS");
	//if (str)
	//{
	//	if (strcmp(str,"ON")==0) Producer::RenderSurface::shareAllGLContexts(true);
	//	else if (strcmp(str,"OFF")==0) Producer::RenderSurface::shareAllGLContexts(false);
	//}

    m_scene_data = NULL;
    //m_global_stateset = NULL;
    m_clear_color.set( 0.0f, 0.0f, 0.0f, 1.0f );

    m_fusionDistanceMode = CRUtil::crSceneView::PROPORTIONAL_TO_SCREEN_DISTANCE;
    m_fusionDistanceValue = 1.0f;

	m_realizeSceneViewOptions = CRUtil::crSceneView::STANDARD_SETTINGS/*|CRUtil::crSceneView::USEPHYSICS*/;

    m_initialized = false;

    // set up the time and frame counter.
   // m_frameNumber = 0;
    m_start_tick = m_timer.tick();

	//m_uiMouseOffset = 0.0f;

 //   if (!m_frameStamp) m_frameStamp = new CRCore::crFrameStamp;
	//CRCore::crBrain::getInstance()->setFrameStamp(m_frameStamp.get());
    // set up the maximum number of graphics contexts, before loading the scene graph
    // to ensure that texture objects and display buffers are configured to the correct size.
   // CRCore::crDisplaySettings::instance()->setMaxNumberOfGraphicsContexts( getNumberOfCameras() );

}

void crCameraGroup::setSceneData( CRCore::crNode *scene ) 
{ 
    if (m_scene_data==scene) return;

    if (m_scene_decorator.valid() && m_scene_data.valid())
    {
        m_scene_decorator->removeChild(m_scene_data.get());
    }
    
    m_scene_data = scene; 
    
    if (m_scene_decorator.valid() && m_scene_data.valid())
    {
        m_scene_decorator->addChild(scene);
    }
    updatedSceneData();
}
        
void crCameraGroup::setSceneDecorator( CRCore::crGroup* decorator)
{
    if (m_scene_decorator==decorator) return;

    m_scene_decorator = decorator;

    if (m_scene_data.valid() && decorator) 
    {
        decorator->addChild(m_scene_data.get());
    }
	updatedSceneData();  
}
     
void crCameraGroup::updatedSceneData()
{
	setUpSceneViewsWithData();
}

void crCameraGroup::setUpSceneViewsWithData()
{
    for(SceneHandlerList::iterator  p = m_shvec.begin(); p != m_shvec.end(); p++ )
    {
        CRUtil::crSceneView* sv = (*p)->getSceneView();
    
        if (m_scene_decorator.valid())
        {
            sv->setSceneData( m_scene_decorator.get() );
        }
        else if (m_scene_data.valid())
        {
            sv->setSceneData( m_scene_data.get() );
        }
        else
        {
            sv->setSceneData( 0 );
        }
        
        //sv->setFrameStamp( m_frameStamp.get() );
        //sv->setGlobalStateSet( m_global_stateset.get() );
        sv->setFusionDistance( m_fusionDistanceMode, m_fusionDistanceValue );
    }
}


//void crCameraGroup::setFrameStamp( CRCore::crFrameStamp* fs )
//{
//    m_frameStamp = fs;
//    setUpSceneViewsWithData();
//}


//void crCameraGroup::setGlobalStateSet( CRCore::crStateSet *sset ) 
//{ 
//    m_global_stateset = sset; 
//    setUpSceneViewsWithData();
//}

void crCameraGroup::setClearColor( const CRCore::crVector4f& clearColor ) 
{
	m_clear_color = clearColor;
	for(unsigned int i=0;i<getNumberOfCameras();++i)
	{
		Producer::Camera *cam = _cfg->getCamera(i);
		cam->setClearColor(m_clear_color[0],m_clear_color[1],m_clear_color[2],m_clear_color[3]);
	}
}

CRCore::crVector4f& crCameraGroup::getClearColor() { return m_clear_color; }
const CRCore::crVector4f& crCameraGroup::getClearColor() const { return m_clear_color; }

void crCameraGroup::setRealizeSceneViewOptions(unsigned int options) 
{
#ifdef DISABLE_PHYSICS
    options &= ~CRUtil::crSceneView::USEPHYSICS;
#endif
#ifdef DISABLE_AI
	options &= ~CRUtil::crSceneView::USEAI;
#endif
	m_realizeSceneViewOptions = options; 
}

unsigned int crCameraGroup::getRealizeSceneViewOptions() { return m_realizeSceneViewOptions; }

void crCameraGroup::setCullSettings( const CRCore::crCullSettings& cs) { m_cullSettings = cs; }
CRCore::crCullSettings& crCameraGroup::getCullSettings() { return m_cullSettings; }
const CRCore::crCullSettings& crCameraGroup::getCullSettings() const { return m_cullSettings; }

void crCameraGroup::setLODScale( float scale )
{
	getCullSettings().setLODScale(scale);
	setUpSceneViewsWithData();
}

float crCameraGroup::getLODScale() const
{
	return getCullSettings().getLODScale();
}

void crCameraGroup::setFusionDistance( CRUtil::crSceneView::FusionDistanceMode mode,float value)
{
    // need to set a local variable?
    m_fusionDistanceMode = mode;
    m_fusionDistanceValue = value;
    setUpSceneViewsWithData();
}

void crCameraGroup::setRealizeCallback( RealizeCallback* cb) { m_realizeCallback = cb; }
crCameraGroup::RealizeCallback* crCameraGroup::getRealizeCallback() { return m_realizeCallback.get(); }
const crCameraGroup::RealizeCallback* crCameraGroup::getRealizeCallback() const { return m_realizeCallback.get(); }

void crCameraGroup::advance()
{
    if( !m_initialized ) return;
    CameraGroup::advance();        
}

bool crCameraGroup::realize( ThreadingModel thread_model,Producer::Window parentWnd,Producer::Window renderWnd )
{
    //if( _realized ) return _realized;
    _thread_model = thread_model;
    return realize(parentWnd,renderWnd);
}

//CRCore::crStateSet *crCameraGroup::getGlobalStateSet() { return m_global_stateset.get(); }
//const CRCore::crStateSet *crCameraGroup::getGlobalStateSet() const { return m_global_stateset.get(); }

bool crCameraGroup::realize(Producer::Window parentWnd,Producer::Window renderWnd)
{
    if( m_initialized ) return _realized;

    if (!m_ds) m_ds = CRCore::crDisplaySettings::instance();
	
	//if(parentWnd)
	//{
		//m_ds->setFullScreen(false);
		//m_ds->setUseBoard(false);
		//RECT rect;
		//GetClientRect(parentWnd, &rect);
		//int windowWidth = (rect.right - rect.left);
		//int windowHeight = (rect.bottom - rect.top);
		//m_ds->setScreenWidth(windowWidth);
		//m_ds->setScreenHeight(windowHeight);
		//m_ds->setViewSize(windowWidth,windowHeight);
		//m_ds->calcViewScale();
	//}

    m_ds->setMaxNumberOfGraphicsContexts( _cfg->getNumberOfCameras() );
    
    //m_shvec.clear();
    
    CRCore::crNode* node = getTopMostSceneData();
    if (node)
    {
        // traverse the scene graphs gathering the requirements of the OpenGL buffers.
        CRUtil::crDisplayRequirementsVisitor drv;
        drv.setDisplaySettings(m_ds.get());
        node->accept(drv);
    }

    unsigned int numMultiSamples = 0;

    #ifdef __sgi
    // switch on anti-aliasing by default, just in case we have an Onyx :-)
    numMultiSamples = 4;
    #endif

    // set up each render stage to clear the appropriate buffers.
    GLbitfield clear_mask=0;
    if (m_ds->getRGB())              clear_mask |= GL_COLOR_BUFFER_BIT;
    if (m_ds->getDepthBuffer())      clear_mask |= GL_DEPTH_BUFFER_BIT;
    if (m_ds->getStencilBuffer())    clear_mask |= GL_STENCIL_BUFFER_BIT;

    // make sure any camera's which share the same render surface also share the same CRCore::crState.
    // use a std::map to keep track of what render surfaces are associated with what state.
    typedef std::map<Producer::RenderSurface*,CRCore::crState*> RenderSurfaceStateMap;
    RenderSurfaceStateMap renderSurfaceStateMap;
    
	bool needToCreatedNewContextID = true;
	unsigned int contextID = 0;

    for(unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        Producer::Camera *cam = _cfg->getCamera(i);

        Producer::RenderSurface* rs = cam->getRenderSurface();
		rs->setWindowName("Creature3D");
		rs->setParentWindow(parentWnd);
		rs->setWindow(renderWnd);
//#ifdef _DEBUG
		DEVMODE dm;
		memset(&dm,0,sizeof(dm));
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		int screenWidth = dm.dmPelsWidth;
		int screenHeight = dm.dmPelsHeight;
		rs->setScreenSize(screenWidth,screenHeight);
		//char gbuf[256];
		//sprintf(gbuf,"setScreenSize screenWidth=%d screenHeight=%d\n\0",screenWidth,screenHeight);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		int dsWidth = m_ds->getScreenWidth();
		int dsHeight = m_ds->getScreenHeight();
		int x = 0;
		int y = 0;
		//m_uiMouseOffset = 0.0f;
		if(renderWnd)
		{
			char *viewx = getenv("CRE_ViewX");
			if(viewx)
			{
				x = atoi(viewx);
			}
			char *viewy = getenv("CRE_ViewY");
			if(viewy)
			{
				y = atoi(viewy);
			}
		}
		else if(parentWnd)
		{
			rs->useBorder(false);
		}
		else if(!m_ds->getFullScreen() && m_ds->getUseBoard())
		{
			int shellH = 0;
			RECT   Rect; 
			HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
			if(GetWindowRect(hWnd,&Rect))   
			{ 
				shellH = Rect.bottom-Rect.top;//   就是任务栏的高度
				if(shellH >= screenHeight)
					shellH = 0;
			}
			int h = GetSystemMetrics(SM_CYCAPTION);//SM_CYSIZE
			int xFrame = GetSystemMetrics(SM_CXFRAME);
			int yFrame = GetSystemMetrics(SM_CYFRAME);
#if(_WIN32_WINNT >= 0x0600)
			yFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
			xFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
#endif 
			//int h = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
			int y1 = (screenHeight-shellH-dsHeight)*0.5f;
			//CRCore::notify(CRCore::FATAL)<<"y1="<<y1<<std::endl;
			//char gbuf[256];
			//sprintf(gbuf,"y1=%d\n\0",y1);
			//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			if(y1<0)
				y1 = 0;
			y = y1 + h + yFrame;
			x = (screenWidth-dsWidth)*0.5f + xFrame;
			rs->useBorder(m_ds->getUseBoard());
			//dsHeight -= h;
			//int viewHeight = m_ds->getViewHeight();
			//int h = dsHeight-viewHeight;
			//m_uiMouseOffset = -float(h)/float(dsHeight);
		}
		else
		{
			x = (screenWidth-dsWidth)*0.5f;
			y = (screenHeight-dsHeight)*0.5f;
			rs->useBorder(false);
		}
		//CRCore::notify(CRCore::FATAL)<<"x="<<x<<"y="<<y<<std::endl;
		//char gbuf[256];
		//sprintf(gbuf,"setWindowRectangle x=%d y=%d w=%d h=%d\n\0",x,y,(int)m_ds->getViewWidth(),(int)m_ds->getViewHeight());
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		rs->setWindowRectangle(x,y,m_ds->getViewWidth(),m_ds->getViewHeight(),true);
		rs->fullScreen(m_ds->getFullScreen());
//#endif
		cam->setClearColor(m_clear_color[0],m_clear_color[1],m_clear_color[2],m_clear_color[3]);
		// create the scene handler.
		Producer::ref_ptr<CRProducer::crSceneHandler> sh;
		if(m_shvec.empty())
		{
			//if(sh == NULL) sh = dynamic_cast<CRProducer::crSceneHandler*>(cam->getSceneHandler());
			sh = new CRProducer::crSceneHandler(new CRUtil::crSceneView(m_ds.get()));
			m_shvec.push_back( sh );
		}
		else
		{
			sh = m_shvec[0];
		}
		sh->setCameraGroup(this);

        CRUtil::crSceneView* sv = sh->getSceneView();

		//cam->setLensNearAndFarClip(1.0,crDisplaySettings::instance()->getMaxDepth());
		//cam->setLensNearAndFarClip(clip[0], clip[1]);

		//const crVector2 &fov = crDisplaySettings::instance()->getFov();
		//const crVector2 &clip = crDisplaySettings::instance()->getClip();
		//const crVector2 &shear = crDisplaySettings::instance()->getShear();
		//cam->setLensPerspective(fov[0], fov[1], clip[0], clip[1], shear[0], shear[1]);
  //      cam->getLens()->setAspectRatio( crDisplaySettings::instance()->getScreenWidth()/crDisplaySettings::instance()->getScreenHeight());
		//crDisplaySettings *ds = crDisplaySettings::instance();
		float fovy = m_ds->getFovy();
		float aspectRatio = m_ds->getViewWidth() / m_ds->getViewHeight();//CRCore::minimum(ds->getScreenWidth() / ds->getScreenHeight(),ds->getAspectRatio());
		const crVector2 &clip = crBrain::getInstance()->getClip();//crDisplaySettings::instance()->getClip();
		//CRCore::notify(CRCore::FATAL)<<"crCameraGroup::realize(): "<<fovy<<std::endl;
		cam->getLens()->setPerspective2(
			fovy,aspectRatio,clip[0], clip[1]);

		if(_thread_model == ThreadPerCameraEx)
			sv->setUseRenderDoubleBuffer(true);//doubleThreadRenderInit();
		else
			sv->setUseRenderDoubleBuffer(false);

        sv->setDefaults(m_realizeSceneViewOptions);

        if (renderSurfaceStateMap.count(rs)==0)
        {
            renderSurfaceStateMap[rs] = sv->getState();
			if (needToCreatedNewContextID)
			{
				// create a unique contextID for this graphics context.
				contextID = CRCore::crGraphicsContext::createNewContextID();

				// if we are sharing our graphics context then when needn't create any more.
				needToCreatedNewContextID = !Producer::RenderSurface::allGLContextsAreShared();
			}
			else
			{
				CRCore::crGraphicsContext::incrementContextIDUsageCount(contextID);
			}
			sv->getState()->setContextID(contextID);
        }
        else
        {
            sv->setState(renderSurfaceStateMap[rs]);
        }
			
        cam->setSceneHandler( sh.get() );
        
        // set up the clear mask.
        CRUtil::crRenderStage *stage = sv->getRenderStage();
        if (stage) stage->setClearMask(clear_mask);

        // set the realize callback.
        rs->setRealizeCallback( new RenderSurfaceRealizeCallback(this, sh.get()));

        // set up the visual chooser.
        if (m_ds.valid()/* || numMultiSamples!=0*/)
        {
        
            Producer::VisualChooser* rs_vc = rs->getVisualChooser();
            if (!rs_vc)
            {
                rs_vc = new Producer::VisualChooser;
                rs_vc->setSimpleConfiguration();
                rs->setVisualChooser(rs_vc);
            }

			unsigned int numStencilBits = 0;
			if (m_ds->getStereo())
			{
				switch(m_ds->getStereoMode())
				{
				case(CRCore::crDisplaySettings::QUAD_BUFFER): 
					rs_vc->useStereo();
					break;
				case(CRCore::crDisplaySettings::HORIZONTAL_INTERLACE):
				case(CRCore::crDisplaySettings::VERTICAL_INTERLACE):
					numStencilBits = 8;
					break;
				default:
					break;
				}
			}
			// set up stencil buffer if required.            
			numStencilBits = CRCore::maximum(numStencilBits,m_ds->getMinimumNumStencilBits());
			if (numStencilBits > 0)
			{
				rs_vc->setStencilSize(numStencilBits);
			}
            if (m_ds->getAlphaBuffer()) rs_vc->setAlphaSize(m_ds->getMinimumNumAlphaBits());

            rs_vc->setDepthSize(m_ds->getDepthBits());
			rs_vc->setBufferSize(m_ds->getColorBits());
			if (m_ds->getAccumBuffer())
			{
				rs_vc->setAccumRedSize(m_ds->getMinimumNumAccumRedBits());
				rs_vc->setAccumGreenSize(m_ds->getMinimumNumAccumGreenBits());
				rs_vc->setAccumBlueSize(m_ds->getMinimumNumAccumBlueBits());
				rs_vc->setAccumAlphaSize(m_ds->getMinimumNumAccumAlphaBits());
			}

            #ifdef WIN32
			    if(m_ds->useHardWareAA())
                    rs_vc->setNumMultiSamples(m_ds->getNumMultiSamples());
            #endif

            if (numMultiSamples)
            {
                #if defined( GLX_SAMPLES_SGIS )
                    rs_vc->addExtendedAttribute( GLX_SAMPLES_SGIS,  numMultiSamples);
                #endif
                #if defined( GLX_SAMPLES_BUFFER_SGIS )
                    rs_vc->addExtendedAttribute( GLX_SAMPLES_BUFFER_SGIS,  1);
                #endif
            }

			rs_vc->setVsync(m_ds->getVsync());
        }
    }

	//// now set up GraphicsContext wrappers for each of the render surfaces
	//// to all core Cre classes to keep track of the graphics context.
	for(RenderSurfaceStateMap::iterator ritr = renderSurfaceStateMap.begin();
		ritr != renderSurfaceStateMap.end();
		++ritr)
	{
		Producer::RenderSurface* rs = ritr->first;
		CRCore::crState* state = ritr->second;
		crGraphicsContextImplementation* gc = new crGraphicsContextImplementation(rs);
		gc->setState(state);
		state->setGraphicsContext(gc);
		m_gcList.push_back(gc);
	}
    
    //if( m_global_stateset == NULL && m_shvec.size() > 0 )
    //{
    //    SceneHandlerList::iterator p = m_shvec.begin();
    //    m_global_stateset = (*p)->getSceneView()->getGlobalStateSet();
    //}

    setUpSceneViewsWithData();
    
    m_initialized = CameraGroup::realize(parentWnd,renderWnd);
    return m_initialized;
}

void crCameraGroup::setDisplaySettings( CRCore::crDisplaySettings *ds ) { m_ds = ds; }
CRCore::crDisplaySettings *crCameraGroup::getDisplaySettings() { return m_ds.get(); }
const CRCore::crDisplaySettings *crCameraGroup::getDisplaySettings() const { return m_ds.get(); }

void crCameraGroup::setView(const CRCore::crMatrixd& matrix)
{
    Producer::Matrix pm(matrix.ptr());
            
    setViewByMatrix(pm);
}

CRCore::crMatrixd crCameraGroup::getViewMatrix() const
{
    CRCore::crMatrixd matrix;
    if (_cfg.valid() && _cfg->getNumberOfCameras()>=1)
    {
        const Producer::Camera *cam = _cfg->getCamera(0);
        matrix.set(cam->getViewMatrix());
    }
    return matrix;
}

CRCore::crMatrixd crCameraGroup::getProjectionMatrix()
{
	CRCore::crMatrixd matrix;
	if (_cfg.valid() && _cfg->getNumberOfCameras()>=1)
	{
		Producer::Camera *cam = _cfg->getCamera(0);
		matrix.set(cam->getProjectionMatrix());
	}
	return matrix;
}

void crCameraGroup::sync()
{
	//if( _thread_model != ThreadPerCameraEx )
	//{
		//double time_since_start = m_timer.delta_s(m_start_tick,m_timer.tick());
		//CRCore::crFrameStamp::getInstance()->setFrameNumber(CRCore::crFrameStamp::getInstance()->getFrameNumber()+1);
		//CRCore::crFrameStamp::getInstance()->setReferenceTime(time_since_start);
	//}

    CameraGroup::sync();
}

void crCameraGroup::frame()
{
////	m_frameStamp->update();
////	CRCore::notify(CRCore::ALWAYS)<<"FPS = "<<m_frameStamp->getFPS()<<std::endl;
//
	CRCore::crNode* node = getTopMostSceneData();

    if (node) node->getBound();

	// pass on the cull settings to the scene views to keep
	// the settings in sync.
	for(SceneHandlerList::iterator itr = m_shvec.begin();
		itr != m_shvec.end();
		++itr)
	{
		(*itr)->getSceneView()->inheritCullSettings(m_cullSettings);
	}

    CameraGroup::frame();
}

void crCameraGroup::cleanup_frame()
{
	// first relase all GL objects and switch on the flush of deleted objects
	// in the next frame.
	for(SceneHandlerList::iterator itr = m_shvec.begin();
		itr != m_shvec.end();
		++itr)
	{
		(*itr)->getSceneView()->releaseAllObjects();
		//(*itr)->getSceneView()->setRenderStage(0);
		(*itr)->setCleanUpOnNextFrame(true);
	}

	// make sure that the registry all flushes all its texture objects.
	CRIOManager::crRegistry::instance()->releaseObjects();

	// then run the frame to do the actuall OpenGL clean up.
	draw();
}
