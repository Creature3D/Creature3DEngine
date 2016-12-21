/* -*-c++-*- Producer - Copyright (C) 2001-2004  Don Burns
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
#include <string>
#include <set>
#include <CRProducer/Producer/CameraGroup>
using namespace Producer; 

// implementation now placed in .cpp to avoid Cygwin errors.
CameraGroup::StatsHandler::StatsHandler() {}
CameraGroup::StatsHandler::~StatsHandler() {}

void CameraGroup::_initVariables()
{
    _stack_size  = 0;
    _frame_count = 0;
    _sync_count  = 0;
    _block_on_vsync = false;
    //char *ptr = getenv( "PRODUCER_CAMERA_BLOCK_ON_VSYNC" );
    //if( ptr )
    //    _block_on_vsync = true;
    ////_lens = new Camera::Lens;
    _realized    = false;
    _instrumented = false;
    _thread_model = SingleThreaded;

    //ptr = getenv( "PRODUCER_STACK_SIZE" );
    //if( ptr )
    //{
    //    setStackSize(atoi(ptr));
    //}
    //else
    //{
#if defined(__sgi)
        // IRIX pthreads has the default stack size of 128k, which
        // is way too small for typical OSG apps, so override this
        // and set to 8Mb to ensure that we don't run out of stack space.
        setStackSize(8388608); 
#endif        
    //}

}

void CameraGroup::_initLens()
{
    if( _cfg->getNumberOfCameras() == 1 )
    {
        _lens = _cfg->getCamera(0)->getLens();
        return;
    }
    
    /*
    if( !_lens.valid() && _cfg->getNumberOfCameras() > 0 )
        _lens = _cfg->getCamera(0)->getLens();
     */

    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        Camera *cam = _cfg->getCamera(i);
        if( !_lens.valid()  && cam->getShareLens() == true )
            _lens = cam->getLens();
        if( cam->getShareLens() )
            cam->setLens( _lens.get() );
    }
}

CameraGroup::CameraGroup( CameraConfig *cfg )
{
    _initVariables();
    _cfg = cfg;
    //_initLens();
}

CameraGroup::CameraGroup()
{
    _initVariables();
    _cfg = new CameraConfig;
    _cfg->defaultConfig();
    //_initLens();
}

//CameraGroup::CameraGroup( const std::string& configFile )
//{
//    _initVariables();
//    _cfg = new CameraConfig;
//    if( configFile.empty() )
//        _cfg->defaultConfig();
//    else
//    {
//        std::string fullpath = CameraConfig::findFile( configFile );
//        _cfg->parseFile( fullpath.c_str() );
//    }
//    //_initLens();
//}

CameraGroup::~CameraGroup()
{
	if( _thread_model == ThreadPerCameraEx )
	{// add by wch
		unsigned int i;
		for( i= 0; i < getNumberOfCameras(); i++ )
		{
			getCamera(i)->cancel();
		}
	}
	else
	{
		// first dispatch cancel
		unsigned int i;
		for( i= 0; i < getNumberOfCameras(); i++ )
		{
			if( getCamera(i)->isRunning() )
			{
				getCamera(i)->cancel();
				//getCamera(i)->join();
			}
		}


#if 1    
		//
		// Brut force stoppage.
		//
		bool stopped = false;
		while (!stopped)
		{
			if (_syncBarrier.valid()) _syncBarrier->release();
			if (_frameBarrier.valid()) _frameBarrier->release();

			unsigned int numStopped = 0;
			for(unsigned int i=0; i<getNumberOfCameras(); ++i)
			{
				if(getCamera(i)->isRunning())
				{
					// do nothing
					CRCore::crThread::yieldCurrentThread();
				}
				else 
				{
					++numStopped;
				}
			}
			if(numStopped == getNumberOfCameras())
			{
				stopped = true;
			}
		}

#else        

		// then wait for all the threads to stop running.
		Timer timer;
		for( i= 0; i < getNumberOfCameras(); i++ )
		{
			Timer_t start_wait = timer.tick();
			double waitTime = 1.0; // in seconds

			// while the camera is running wait for it to stop naturally.
			// However, under OSX this seems to be wiating for ever, so we'll cut our losses and just wait for
			// a fixed period, then let the CameraGroup destruct, this might cause an app crash
			// due to deleting data while a thread is still running, but at least the app will exit...    
			// Clearly this needs to be fixed in a more robust way, possible thoughts for the cause of this wait
			// for ever on exit is a bug in OSX pthreads.
			// Robert Osfield, September 13th 2003.
			while(getCamera(i)->isRunning() && timer.delta_s(start_wait,timer.tick())<waitTime)
			{
				//if (_syncBarrier.valid()) _syncBarrier->reset();
				//if (_frameBarrier.valid()) _frameBarrier->reset();   

				CRCore::crThread::yieldCurrentThread();
			}

			if (getCamera(i)->isRunning())
			{
				//std::cout<<"Wait time ("<<waitTime<<" seconds) for camera "<<i<<" to cancel has expired, but "<<std::endl
				//	<<"camera thread still running, continuing CameraGroup destructor regardless."<<std::endl;
			}
		}

#endif
	}
	//std::cerr << "~CameraGroup()" << std::endl;
}

void CameraGroup::cancelCameraThreads()// add by wch
{
	if(_thread_model == ThreadPerCameraEx)
	{
		int nthreads = _cfg->getNumberOfCameras();
		for (int i = 0; i < nthreads; i++)
		{
			_cfg->getCamera(i)->setDone(1);
		}
		while(1)
		{
			CRCore::crThread::sleep(10);
			if( _updateBarrier->numThreadsCurrentlyBlocked() )
				_updateBarrier->release();
			if( _cullBarrier->numThreadsCurrentlyBlocked() )
				_cullBarrier->release();
			if (_drawBarrier->numThreadsCurrentlyBlocked())
				_drawBarrier->release();

			//if( _drawBarrier->numThreadsCurrentlyBlocked() == nthreads &&
			//	_frameBarrier->numThreadsCurrentlyBlocked() == nthreads )
			//{
			//	break;
			//}
			if ( _syncBarrier->numThreadsCurrentlyBlocked() == nthreads )
			{
				break;
			}
		}
		while (1)
		{
			CRCore::crThread::sleep(10);
			if (_frameBarrier2->numThreadsCurrentlyBlocked())
				_frameBarrier2->release();
			if (_frameBarrier->numThreadsCurrentlyBlocked())
				_frameBarrier->release();
			if (_drawBarrier->numThreadsCurrentlyBlocked())
				_drawBarrier->release();
			if (_syncBarrier->numThreadsCurrentlyBlocked() == nthreads * 2)
			{
				break;
			}
		}
		//for( int i = 0; i < nthreads; i++ )
		//{
		//	_cfg->getCamera(i)->setDone(true);
		//}
		//CRCore::crThread::sleep(1);
		//_drawBarrier->release();
		//std::cerr << "_drawBarrier->release()" << std::endl;
		//_frameBarrier->release();
		//std::cerr << "_frameBarrier->release()" << std::endl;
	}
		//_initVariables();
}
void CameraGroup::releaseUpdateThreads()
{
	if(_updateBarrier.valid())
	{
		_updateBarrier->invalidate();
		_updateBarrier = NULL;
	}
	if(_cullBarrier.valid())
	{
		_cullBarrier->invalidate();
		_cullBarrier = NULL;
	}
}
void CameraGroup::setStackSize( size_t size )
{
    if( _realized )
    {
        std::cerr << "CameraGroup::setStackSize(): Camera Group is already realized.\n"
                     "SetStackSize may only be called before CameraGroup::realize();\n";
        return ;
    }
    _stack_size = size;
}


bool CameraGroup::realize( ThreadingModel thread_model,Producer::Window parentWnd,Producer::Window renderWnd )
{
    if( _realized ) return _realized;
    _thread_model = thread_model;
    return realize(parentWnd,renderWnd);
}

bool CameraGroup::realize(Producer::Window parentWnd,Producer::Window renderWnd)
{
    if( _realized ) return _realized;
	_sync_count = 0;
    _frameStats._frameTimeStampSets.resize(_cfg->getNumberOfCameras());
    _initTime = _timer.tick();

    if( _thread_model == SingleThreaded )
    {
        for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
        {
            RenderSurface *rs = _cfg->getCamera(i)->getRenderSurface();
#ifndef WIN32
            rs->useConfigEventThread(false);
#endif
            rs->realize();
            _cfg->getCamera(i)->setBlockOnVsync(_block_on_vsync);
        }
        _realized = true;
    }
    else if( _thread_model == ThreadPerCamera )
    {
#ifdef _X11_IMPLEMENTATION
        RenderSurface::initThreads();
#endif
        int nthreads = _cfg->getNumberOfCameras();

        if(!_syncBarrier) _syncBarrier  = new RefBarrier( nthreads + 1 );
        if(!_frameBarrier) _frameBarrier = new RefBarrier( nthreads + 1 );

        for( int i = 0; i < nthreads; i++ )
        {
            Camera *cam = _cfg->getCamera(i);
			//cam->setRenderSurface(RenderSurface::getRenderSurface(i));
            cam->setSyncBarrier( _syncBarrier.get() );
            cam->setFrameBarrier( _frameBarrier.get(),NULL );
            if( _stack_size != 0 )
                cam->setStackSize( _stack_size );
            cam->setInitTime( _initTime );
            cam->setBlockOnVsync(_block_on_vsync);
			cam->setDone(false);
            cam->startThread();
        }
        _realized = true;
    }
	else if( _thread_model == ThreadPerCameraEx )
	{
#ifdef _X11_IMPLEMENTATION
		RenderSurface::InitThreads();
#endif
		int nthreads = _cfg->getNumberOfCameras();

		if(!_syncBarrier) _syncBarrier  = new RefBarrier( 2*nthreads+1 );
		if(!_frameBarrier) _frameBarrier = new RefBarrier( 2*nthreads );
		if (!_frameBarrier2) _frameBarrier2 = new RefBarrier(2 * nthreads);

		if(!_cullBarrier) _cullBarrier = new RefBarrier( nthreads + 1 );
		if(!_updateBarrier) _updateBarrier = new RefBarrier( nthreads + 1 );
		if(!_drawBarrier) _drawBarrier  = new RefBarrier( nthreads + 1 );

		for( int i = 0; i < nthreads; i++ )
		{
			Camera *cam = _cfg->getCamera(i);
			//cam->setRenderSurface(RenderSurface::getRenderSurface(i));
			cam->initChildThread();

			cam->setSyncBarrier( _syncBarrier.get() );
			cam->setFrameBarrier(_frameBarrier.get(), _frameBarrier2.get());

			cam->setCullBarrier( _cullBarrier.get() );
			cam->setUpdateBarrier( _updateBarrier.get() );
			cam->setDrawBarrier( _drawBarrier.get() );

			if( _stack_size != 0 )
			{
				//cam->setStackSize( _stack_size );
				cam->getCull_UpdateThread()->setStackSize( _stack_size );
				cam->getDrawThread()->setStackSize( _stack_size );
			}
			cam->setInitTime( _initTime );
			cam->setBlockOnVsync(_block_on_vsync);

			cam->setDone(false);
			//cam->getCull_UpdateThread()->startThread();
			//cam->getDrawThread()->startThread();
			cam->startChildThread();
		}
		_realized = true;
	}
    else
    {
        return _realized ;
    }
    _initLens();
    waitForRealize();
    return _realized;
}

void CameraGroup::setViewByLookat(  float eyex, float eyey, float eyez,
                                    float centerx, float centery, float centerz,
                                    float upx, float upy, float upz )
{
    setViewByLookat( Vec3(eyex, eyey, eyez),
                     Vec3(centerx, centery, centerz ),
                     Vec3(upx, upy, upz) );
}

void CameraGroup::setViewByLookat(  const Vec3& eye,
                                    const Vec3& center,
                                    const Vec3& up )
{
    Matrix m;
    m.makeLookAt(eye,center,up);
    setViewByMatrix( m );
}

void CameraGroup::setViewByMatrix(  const Producer::Matrix &mat )
{
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        Camera *cam = _cfg->getCamera(i);
        if( cam->getShareView() )
            cam->setViewByMatrix( mat );
    }
}

void CameraGroup::setInstrumentationMode( bool mode )
{
    _instrumented = mode;
    for( unsigned int i = 0 ; i < _cfg->getNumberOfCameras(); i++ )
        _cfg->getCamera(i)->setInstrumentationMode(_instrumented);
}

void CameraGroup::cull_update( ) //add by wch
{
	if( _instrumented )
		_cull_updateInstrumented();
	else
		_cull_update();
}
void CameraGroup::_cull_update() //add by wch
{
	if( !_realized )
	{
		std::cerr << "CameraGroup::cull_update() : Please call realize() first\n";
		return;
	}
	if( _thread_model == SingleThreaded )
	{
		for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
		{
			_cfg->getCamera(i)->update();
			_cfg->getCamera(i)->cull();
		}
	}
	else if( _thread_model == ThreadPerCameraEx )
	{
		//if( _sync_count == _frame_count ) 
		//	sync();
		if (_updateBarrier.valid())
		{
			_updateBarrier->block();
			update();
		}
		if (_cullBarrier.valid())
		{
			_cullBarrier->block();
		}
	}
}
void CameraGroup::_cull_updateInstrumented() //add by wch
{
	if( !_realized )
	{
		std::cerr << "CameraGroup::cull_update() : Please call realize() first\n";
		return;
	}

	if( _thread_model == SingleThreaded )
	{
		//_endOfUpdate = _timer.tick();
		//_startOfFrame = _timer.tick(); 
		std::map <RenderSurface *, Camera *> u;
		for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ ) 
		{
			_cfg->getCamera(i)->update();
			_cfg->getCamera(i)->cull();
		}

		/*if( markStartOfUpdate )
		_startOfUpdate = _timer.tick();*/
	}
	else if( _thread_model == ThreadPerCameraEx )
	{
		_startOfUpdate =  _timer.tick();
		if (_updateBarrier.valid())
		{
			_updateBarrier->block();
			update();
		}
		_endOfUpdate = _timer.tick();

		if (_cullBarrier.valid())
		{
			_cullBarrier->block();
		}
	}
}

void CameraGroup::draw( ) //add by wch
{
	if( _instrumented )
		_drawInstrumented();
	else
	{
		_draw();
		if( _statsHandler.valid() )
			(*_statsHandler)(*this);
	}
}

void CameraGroup::_draw() //add by wch
{
	if( !_realized )
	{
		std::cerr << "CameraGroup::draw() : Please call realize() first\n";
		return;
	}
	if( _thread_model == SingleThreaded )
	{
		std::map <RenderSurface *, Camera *> u;
		for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
		{
			u.insert( std::pair <RenderSurface *, Camera *>(
				_cfg->getCamera(i)->getRenderSurface(),
				_cfg->getCamera(i) ));
			_cfg->getCamera(i)->draw(false);
		}

		std::map <RenderSurface *, Camera *>::iterator p;
		for( p = u.begin(); p != u.end(); p++ )
			p->second->advance();
	}
	else if( _thread_model == ThreadPerCameraEx )
	{
		//if( _sync_count == _frame_count ) 
		//	sync();
		if (_drawBarrier.valid())
		{
			_drawBarrier->block();
		}
	}
	_frame_count++;
}

void CameraGroup::_drawInstrumented() //add by wch
{
	if( !_realized )
	{
		std::cerr << "CameraGroup::draw() : Please call realize() first\n";
		return;
	}

	//_endOfUpdate = _timer.tick();
	//bool markStartOfUpdate = false;
	////if( _sync_count == _frame_count ) 
	////{
	////	_syncInstrumented(false);
	////	markStartOfUpdate = true;
	////}
	_startOfFrame = _timer.tick(); 

	if( _thread_model == SingleThreaded )
	{
		_endOfUpdate = _timer.tick();
		//_startOfFrame = _timer.tick(); 
		std::map <RenderSurface *, Camera *> u;
		for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ ) 
		{
			u.insert( std::pair <RenderSurface *, Camera *>(
				_cfg->getCamera(i)->getRenderSurface(),
				_cfg->getCamera(i) ));
			_cfg->getCamera(i)->draw(false);
		}

		std::map <RenderSurface *, Camera *>::iterator p;
		for( p = u.begin(); p != u.end(); p++ )
			p->second->advance();

		//if( markStartOfUpdate )
		_startOfUpdate = _timer.tick();
	}
	else if( _thread_model == ThreadPerCameraEx )
	{
		//_endOfUpdate =  _timer.tick();
		//_startOfFrame = _timer.tick();

		if (_drawBarrier.valid())
		{
			_drawBarrier->block();
		}
	}
	_updateStats();
	_frame_count++;
}

void CameraGroup::frame()
{
    if( _instrumented )
        _frameInstrumented();
    else
        _frame();
}

void CameraGroup::_singleThreadedFrame()
{
    // Keep a unique set of RenderSurfaces.
    // Since there is not necesarily a one-to-one mapping of Cameras to
    // RenderSurfaces (there can be multiple Cameras per RenderSurface), we
    // don't want to swap buffer every Camera's RenderSurface on advance(),
    // because we can potentially swap a buffer more than once per frame.
    std::set<RenderSurface *> u;
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        u.insert( _cfg->getCamera(i)->getRenderSurface() );
        _cfg->getCamera(i)->frame(false);
    }
    
    std::set<RenderSurface *>::iterator p;
    
    bool _syncronizeSwapBuffers = true;
    if (_syncronizeSwapBuffers && u.size()>1)
    {
        for( p = u.begin(); p != u.end(); p++ )
        {
            (*p)->makeCurrent();
            glFinish();
        }
    }
    
    for( p =  u.begin(); p != u.end(); p++ )
    {
        (*p)->swapBuffers();
    }
}

void CameraGroup::_threadPerCameraFrame()
{
    if (_frameBarrier.valid())
        _frameBarrier->block();
}

void CameraGroup::_frame()
{
    if( !_realized )
    {
        std::cerr << "CameraGroup::frame() : Please call realize() first\n";
        return;
    }
    if( _thread_model == SingleThreaded )
    {
        _singleThreadedFrame();
    }
    else if( _thread_model == ThreadPerCamera )
    {
        if( _sync_count == _frame_count ) 
            sync();

        _threadPerCameraFrame();
    }
    _frame_count++;
}


void CameraGroup::_frameInstrumented()
{
    if( !_realized )
    {
        std::cerr << "CameraGroup::frame() : Please call realize() first\n";
        return;
    }

    _endOfUpdate = _timer.tick();
    bool markStartOfUpdate = false;
    if( _sync_count == _frame_count ) 
    {
        _syncInstrumented(false);
        markStartOfUpdate = true;
    }
    _startOfFrame = _timer.tick(); 

    if( _thread_model == SingleThreaded )
    {
        //_endOfUpdate = _timer.tick();
        //_startOfFrame = _timer.tick(); 
        _singleThreadedFrame();

        if( markStartOfUpdate )
            _startOfUpdate = _timer.tick();
    }
    else if( _thread_model == ThreadPerCamera )
    {
        //_endOfUpdate =  _timer.tick();
        //_startOfFrame = _timer.tick();
        _threadPerCameraFrame();
    }

    _frame_count++;
}

void CameraGroup::sync()
{
    if( _instrumented )
        _syncInstrumented(true);
    else
        _sync();
}

void CameraGroup::_sync()
{
    if( _sync_count > _frame_count ) return;

    if( _thread_model == SingleThreaded )
    {
        ; // ignored
    }
    else if( _thread_model == ThreadPerCamera )
    {
        if (_syncBarrier.valid())
        {
            _syncBarrier->block();
        }
    }
	else if( _thread_model == ThreadPerCameraEx )
	{
		if (_syncBarrier.valid())
		{
			_syncBarrier->block();
		}
	}
    _sync_count = _frame_count + 1;
}

void CameraGroup::_updateStats()
{
    _frameStats._frameNumber = _frame_count;
    _frameStats._startOfUpdate = _timer.delta_s(_initTime, _startOfUpdate );
    _frameStats._endOfUpdate = _timer.delta_s(_initTime, _endOfUpdate );
    _frameStats._startOfFrame = _timer.delta_s( _initTime, _startOfFrame );
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        _frameStats._frameTimeStampSets[i] = _cfg->getCamera(i)->getFrameStats();
        _frameStats._frameTimeStampSets[i].setFrameNumber(_frameStats._frameNumber);
    }

    if( _statsHandler.valid() )
        (*_statsHandler)(*this);
}

void CameraGroup::_syncInstrumented( bool markStartOfUpdate )
{
    if( _sync_count > _frame_count ) return;
    _sync_count = _frame_count + 1;

    if( _thread_model == ThreadPerCamera )
    {
        if (_syncBarrier.valid())
        {
            _syncBarrier->block();
        }
    }
	else if( _thread_model == ThreadPerCameraEx )
	{
		if (_syncBarrier.valid())
		{
			_syncBarrier->block();
		}
	}

    _updateStats();
    if( markStartOfUpdate )
        _startOfUpdate = _timer.tick();
}


void CameraGroup::advance()
{
    /*
    for( unsigned int i = 0; i < _cfg->getNumberOfRenderSurfaces(); i++ )
    {
        _cfg->getRenderSurface(i)->makeCurrent();
        _cfg->getRenderSurface(i)->swapBuffers();
    }
    */

    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        //_cfg->getCamera(i)->getRenderSurface()->makeCurrent();
        _cfg->getCamera(i)->getRenderSurface()->swapBuffers();
    }
}

Camera *CameraGroup::getCamera(int i) 
{ 
    return _cfg->getCamera(i); 
}

const Camera *CameraGroup::getCamera(int i)  const
{ 
    return _cfg->getCamera(i); 
}

unsigned int CameraGroup::getNumberOfCameras() const
{
    return _cfg->getNumberOfCameras();
}

CameraConfig *CameraGroup::getCameraConfig()
{
    return _cfg.get();
}

const CameraConfig *CameraGroup::getCameraConfig() const
{
    return _cfg.get();
}

void CameraGroup::setSceneHandler(Camera::SceneHandler *sh )
{
    for( unsigned int i = 0; i <  _cfg->getNumberOfCameras(); i++ )
    _cfg->getCamera(i)->setSceneHandler(sh);    
}


#if 0
bool CameraGroup::convertLensToOrtho( float d) 
{ 
    bool retval = true;

    for( unsigned int i = 0; i <  _cfg->getNumberOfCameras(); i++ )
        if( _cfg->getCamera(i)->convertLensToOrtho(d) == false )
        retval = false;

    return retval;
}

bool CameraGroup::convertLensToPerspective( float d )
{
    bool retval = true;

    for( unsigned int i = 0; i <  _cfg->getNumberOfCameras(); i++ )
        if( _cfg->getCamera(i)->convertLensToPerspective(d) == false )
        retval = false;

    return retval;
}
#endif


bool CameraGroup::waitForRealize()
{
    bool retval = true;
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        if( _cfg->getCamera(i)->getRenderSurface()->waitForRealize() == false )
            retval = false;
    }
    return retval;
}

bool CameraGroup::validForRendering() const
{
    bool retval = true;
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
    {
        if( _cfg->getCamera(i)->getRenderSurface()->isRealized() == false )
        {
            retval = false;
            break;
        }
    }
    return retval;
}

void CameraGroup::setBlockOnVsync(bool block )
{
    _block_on_vsync = block;
    for( unsigned int i = 0; i < _cfg->getNumberOfCameras(); i++ )
        _cfg->getCamera(i)->setBlockOnVsync(_block_on_vsync);
}
