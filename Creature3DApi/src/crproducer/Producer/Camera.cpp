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
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string.h>
#include <algorithm>
//#include <GL/gl.h>
#include <GL/glu.h>

#include <CRProducer/Producer/Math>
#include <CRProducer/Producer/Camera>
//#include <CRCore/crStreamBuf.h>
using namespace Producer;
using namespace CRCore;

Camera::Camera( void ) 
{
    _index = 0;

    _projrectLeft   = 0.0;
    _projrectRight  = 1.0;
    _projrectBottom = 0.0;
    _projrectTop    = 1.0;

    Matrix::value_type  id[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    memcpy( _viewMatrix, id, sizeof(Matrix::value_type[16]));
    _offset._xshear = _offset._yshear = 0.0f;
    memcpy( _offset._matrix, id, sizeof(Matrix::value_type[16]));
    _offset._multiplyMethod = Offset::PreMultiply;

    _lens = new Lens;
    _lens->setAutoAspect(true);
    _rs = new RenderSurface;
    _sh = NULL;
    
    _clear_color[0] = 0.2f;
    _clear_color[1] = 0.2f;
    _clear_color[2] = 0.4f;
    _clear_color[3] = 1.0f;
    
    _focal_distance = 1.0;
    
    _instrumented = false;
    _frameCount = 0;
    _initTime = _timer.tick();
    _block_on_vsync = false;
    //char *ptr = getenv( "PRODUCER_CAMERA_BLOCK_ON_VSYNC" );
    //if( ptr )
    //    _block_on_vsync = true;

    _shareLens = true;
    _shareView = true;

    _enabled = true;
    _initialized = false;

    _done = 0;
	_vrHMD = true;
}

void Camera::initChildThread()
{
	if(!_cull_updatethread) _cull_updatethread = new Cull_UpdateThread( this );
	if(!_drawthread) _drawthread = new DrawThread( this );
}
void Camera::startChildThread()
{
	if(!_cull_updatethread->isRunning()) _cull_updatethread->startThread();
	if(!_drawthread->isRunning()) _drawthread->startThread();
}
Camera::~Camera( void )
{
	if(_rs.valid())
		_rs->_fini();
	//std::cerr << "~Camera()" << std::endl;
}

//void Camera::cull_update() //add by wch
//{
//	if( !_enabled ) 
//		return;
//
//	if( _updateCallback.valid() )
//		(*((_updateCallback).get()))(*this);
//
//	if( _instrumented )
//		_cull_updateInstrumented();
//	else
//		_cull_update();
//}

void Camera::update() //add by wch
{
	//if( !_enabled ) 
	//	return;
	if( !_initialized ) _initialize();

	//if ( _sh == NULL )
	//{
	//	std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
	//	std::cerr << "   please call setSceneHandler() first\n";
	//	return;
	//}
	//以下进行update
	_sh->update( *this );
	////以下进行cull
	//if( preCullCallbacks.size() )
	//{
	//	std::vector <ref_ptr<Callback> >::iterator p;
	//	for( p = preCullCallbacks.begin(); p != preCullCallbacks.end(); p++ )
	//		(*((*p).get()))(*this);
	//}
	//_sh->cull( *this );

	//if( postCullCallbacks.size() )
	//{
	//	std::vector <ref_ptr<Callback> >::iterator p;
	//	for( p = postCullCallbacks.begin(); p != postCullCallbacks.end(); p++ )
	//		(*((*p).get()))(*this);
	//}
}
void Camera::cull(int vreye) //add by wch
{
	//以下进行cull
	if (preCullCallbacks.size())
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for (p = preCullCallbacks.begin(); p != preCullCallbacks.end(); p++)
			(*((*p).get()))(*this);
	}
	_sh->cull(*this,vreye);

	if (postCullCallbacks.size())
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for (p = postCullCallbacks.begin(); p != postCullCallbacks.end(); p++)
			(*((*p).get()))(*this);
	}
}
//void Camera::_cull_updateInstrumented( ) //add by wch
//{
//	if( !_initialized ) _initialize();
//
//	if ( _sh == NULL )
//	{
//		std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
//		std::cerr << "   please call setSceneHandler() first\n";
//		return;
//	}
//
//	//Timer_t stamps[LastStatsID];
//	//memset( stamps, 0, sizeof(stamps));
//
//	//以下进行update
//	_sh->update( *this );
//	//以下进行cull
//	//	stamps[BeginCull] = _timer.tick();
//	if( preCullCallbacks.size() )
//	{
//		//	stamps[BeginPreCullCallbacks] = _timer.tick();
//		std::vector <ref_ptr<Callback> >::iterator p;
//		for( p = preCullCallbacks.begin(); p != preCullCallbacks.end(); p++ )
//		{
//			(*((*p).get()))(*this);
//		}
//		//	stamps[EndPreCullCallbacks] = _timer.tick();
//	}
//
//
//	//	stamps[BeginInnerCull] = _timer.tick();
//	_sh->cull( *this );
//	//	stamps[EndInnerCull] = _timer.tick();
//
//	if( postCullCallbacks.size() )
//	{
//		//	stamps[BeginPostCullCallbacks] = _timer.tick();
//		std::vector <ref_ptr<Callback> >::iterator p;
//		for( p = postCullCallbacks.begin(); p != postCullCallbacks.end(); p++ )
//			(*((*p).get()))(*this);
//		//	stamps[EndPostCullCallbacks] = _timer.tick();
//	}
//	//	stamps[EndCull] = _timer.tick();
//
//	//for( int i = BeginCull; i < EndCull; i++ )
//	//	_frameStamps[StatsID(i)] = _timer.delta_s(_initTime, stamps[i]);
//
//}

void Camera::draw( bool doSwap,int vreye ) //add by wch
{
	if( !_enabled ) 
		return;

	if( preFrameCallbacks.size() )
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = preFrameCallbacks.begin(); p != preFrameCallbacks.end(); p++ )
			(*((*p).get()))(*this);
	}

	if( _instrumented )
		_drawInstrumented(doSwap, vreye);
	else
		_draw(doSwap, vreye);

	if( postFrameCallbacks.size() )
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = postFrameCallbacks.begin(); p != postFrameCallbacks.end(); p++ )
			(*((*p).get()))(*this);
	}
}

void Camera::_draw( bool doSwap,int vreye ) //add by wch
{
	if( !_initialized ) _initialize();

	if ( _sh == NULL )
	{
		std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
		std::cerr << "   please call setSceneHandler() first\n";
		return;
	}

    ++_frameCount;

	_rs->makeCurrent();
	if( _block_on_vsync ) 
		_rs->sync();
	_sh->clear( *this );
	if( _sh->useAutoView() )
	{
		if( _lens->getAutoAspect() )
		{
			int x, y;
			unsigned int width, height;
			_rs->getWindowRectangle(x,y,width,height);
			//_lens->setAspectRatio( float(width)/float(height));
			double ar = (float(width) * (_projrectRight - _projrectLeft))/
				(float(height) * (_projrectTop - _projrectBottom ));
			_lens->setAspectRatio( ar );
		}

		_lens->apply( _offset._xshear, _offset._yshear );
		applyView();
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		std::cerr << "Camera::_draw makeCurrent error "<<gluErrorString(errorNo)<< std::endl;
		//char gbuf[256];
		//sprintf(gbuf,"Camera::_draw makeCurrent OpenGL error %s\n\0",gluErrorString(errorNo));
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//printf("Camera::_draw makeCurrent error");
	}
#endif

	if( preDrawCallbacks.size() )
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = preDrawCallbacks.begin(); p != preDrawCallbacks.end(); p++ )
			(*((*p).get()))(*this);
	}

	_sh->draw( *this,vreye );

	if( postDrawCallbacks.size() )
	{
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = postDrawCallbacks.begin(); p != postDrawCallbacks.end(); p++ )
			(*((*p).get()))(*this);
	}

	if( doSwap )
	{
		_rs->swapBuffers();

		if( postSwapCallbacks.size() )
		{
			std::vector <ref_ptr<Callback> >::iterator p;
			for( p = postSwapCallbacks.begin(); p != postSwapCallbacks.end(); p++ )
				(*((*p).get()))(*this);
		}
	}
}

void Camera::_drawInstrumented( bool doSwap,int vreye ) //add by wch
{
	if( !_initialized ) _initialize();

	if ( _sh == NULL )
	{
		std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
		std::cerr << "   please call setSceneHandler() first\n";
		return;
	}

	++_frameCount;
	Timer_t stamps[LastStatsID];
	memset( stamps, 0, sizeof(stamps));

	stamps[BeginCameraFrame] = _timer.tick();

	_rs->makeCurrent();

	if( _block_on_vsync )
	{
		_rs->sync();
		stamps[Vsync] = _timer.tick();
	}
	else
		stamps[Vsync] = 0;

	stamps[BeginDraw] = _timer.tick();
	stamps[BeginClear] = _timer.tick();
	_sh->clear( *this );
	stamps[EndClear] = _timer.tick();

	if( _sh->useAutoView() )
	{
		if( _lens->getAutoAspect() )
		{
			int x, y;
			unsigned int width, height;
			_rs->getWindowRectangle(x,y,width,height);
			double ar = (float(width) * (_projrectRight - _projrectLeft))/
				(float(height) * (_projrectTop - _projrectBottom ));
			_lens->setAspectRatio( ar );
		}

		_lens->apply( _offset._xshear , _offset._yshear);
		applyView();
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		//printf("Camera::_draw makeCurrent error");
		std::cerr << "Camera::_drawInstrumented makeCurrent error "<<gluErrorString(errorNo)<< std::endl;
		//char gbuf[256];
		//sprintf(gbuf,"Camera::_drawInstrumented makeCurrent OpenGL error %s\n\0",gluErrorString(errorNo));
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

	if( preDrawCallbacks.size() )
	{
		stamps[BeginPreDrawCallbacks] = _timer.tick();
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = preDrawCallbacks.begin(); p != preDrawCallbacks.end(); p++ )
			(*((*p).get()))(*this);
		stamps[EndPreDrawCallbacks] = _timer.tick();
	}

	stamps[BeginInnerDraw] = _timer.tick();
	_sh->draw(*this, vreye);
	stamps[EndInnerDraw] = _timer.tick();

	if( postDrawCallbacks.size() )
	{
		stamps[BeginPostDrawCallbacks] = _timer.tick();
		std::vector <ref_ptr<Callback> >::iterator p;
		for( p = postDrawCallbacks.begin(); p != postDrawCallbacks.end(); p++ )
			(*((*p).get()))(*this);
		stamps[EndPostDrawCallbacks] = _timer.tick();
	}

	if( doSwap )
	{
		_rs->swapBuffers();

		if( postSwapCallbacks.size() )
		{
			stamps[BeginPostSwapCallbacks] = _timer.tick();
			std::vector <ref_ptr<Callback> >::iterator p;
			for( p = postSwapCallbacks.begin(); p != postSwapCallbacks.end(); p++ )
				(*((*p).get()))(*this);
			stamps[EndPostSwapCallbacks] = _timer.tick();
		}
	}

	stamps[EndDraw] = _timer.tick();
	stamps[EndCameraFrame] = _timer.tick();

	_frameStamps.clear();
	_frameStamps.setFrameNumber( _frameCount );
	for( int i = 0; i < LastStatsID; i++ )
		_frameStamps[StatsID(i)] = _timer.delta_s(_initTime, stamps[i]);

}

void Camera::frame( bool doSwap )
{
    if( !_enabled ) 
        return;

    if( _updateCallback.valid() )
        (*((_updateCallback).get()))(*this);


    if( preFrameCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = preFrameCallbacks.begin(); p != preFrameCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }

    if( _instrumented )
        _frameInstrumented( doSwap );
    else
        _frame(doSwap);

    if( postFrameCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postFrameCallbacks.begin(); p != postFrameCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }
}


void Camera::_frameInstrumented( bool doSwap )
{
    if( !_initialized ) _initialize();

    if ( _sh == NULL )
    {
        std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
        std::cerr << "   please call setSceneHandler() first\n";
        return;
    }

    ++_frameCount;
    Timer_t stamps[LastStatsID];
	memset( stamps, 0, sizeof(stamps));

	stamps[BeginCameraFrame] = _timer.tick();

    if( _sh->useAutoView() )
    {
        if( _lens->getAutoAspect() )
		{
			int x, y;
            unsigned int width, height;
            _rs->getWindowRectangle(x,y,width,height);
            double ar = (float(width) * (_projrectRight - _projrectLeft))/
                        (float(height) * (_projrectTop - _projrectBottom ));
            _lens->setAspectRatio( ar );
        }
    
        _lens->apply( _offset._xshear , _offset._yshear);
    }

	if( _sh->frame( *this ) == true )
	{
		stamps[EndCameraFrame] = _timer.tick();
		return;
	}


    stamps[BeginCull] = _timer.tick();
    if( preCullCallbacks.size() )
    {
        stamps[BeginPreCullCallbacks] = _timer.tick();
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = preCullCallbacks.begin(); p != preCullCallbacks.end(); p++ )
        {
            (*((*p).get()))(*this);
        }
        stamps[EndPreCullCallbacks] = _timer.tick();
    }


    stamps[BeginInnerCull] = _timer.tick();
    _sh->cull( *this,0 );
    stamps[EndInnerCull] = _timer.tick();

    if( postCullCallbacks.size() )
    {
        stamps[BeginPostCullCallbacks] = _timer.tick();
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postCullCallbacks.begin(); p != postCullCallbacks.end(); p++ )
            (*((*p).get()))(*this);
        stamps[EndPostCullCallbacks] = _timer.tick();
    }
    stamps[EndCull] = _timer.tick();

    _rs->makeCurrent();

    if( _block_on_vsync )
    {
        _rs->sync();
        stamps[Vsync] = _syncTick = _timer.tick();
    }
    else
        stamps[Vsync] = 0;
    
    
    stamps[BeginDraw] = _timer.tick();

    stamps[BeginClear] = _timer.tick();
    _sh->clear( *this );
    stamps[EndClear] = _timer.tick();

    if( _sh->useAutoView() )
    {
        applyView();
    }

    if( preDrawCallbacks.size() )
    {
        stamps[BeginPreDrawCallbacks] = _timer.tick();
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = preDrawCallbacks.begin(); p != preDrawCallbacks.end(); p++ )
            (*((*p).get()))(*this);
        stamps[EndPreDrawCallbacks] = _timer.tick();
    }

    stamps[BeginInnerDraw] = _timer.tick();
    _sh->draw( *this,0 );
    stamps[EndInnerDraw] = _timer.tick();

    if( postDrawCallbacks.size() )
    {
        stamps[BeginPostDrawCallbacks] = _timer.tick();
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postDrawCallbacks.begin(); p != postDrawCallbacks.end(); p++ )
            (*((*p).get()))(*this);
        stamps[EndPostDrawCallbacks] = _timer.tick();
    }

    if( doSwap )
        _rs->swapBuffers();

    if( postSwapCallbacks.size() )
    {
        stamps[BeginPostSwapCallbacks] = _timer.tick();
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postSwapCallbacks.begin(); p != postSwapCallbacks.end(); p++ )
            (*((*p).get()))(*this);
        stamps[EndPostSwapCallbacks] = _timer.tick();
    }

    stamps[EndDraw] = _timer.tick();
    stamps[EndCameraFrame] = _timer.tick();

    _frameStamps.clear();
    _frameStamps.setFrameNumber( _frameCount );
    for( int i = 0; i < LastStatsID; i++ )
        _frameStamps[StatsID(i)] = _timer.delta_s(_initTime, stamps[i]);
}

void Camera::_frame( bool doSwap )
{
    if( !_initialized ) _initialize();

    if ( _sh == NULL )
    {
        std::cerr << "Producer::Camera::frame() : No Producer::Camera::SceneHandler\n";
        std::cerr << "   please call setSceneHandler() first\n";
        return;
    }

    ++_frameCount;

    if( _sh->useAutoView() )
    {
        if( _lens->getAutoAspect() )
        {
            int x, y;
            unsigned int width, height;
            _rs->getWindowRectangle(x,y,width,height);
            double ar = (float(width) * (_projrectRight - _projrectLeft))/
                        (float(height) * (_projrectTop - _projrectBottom ));
            _lens->setAspectRatio( ar );
        }
    
        _lens->apply( _offset._xshear , _offset._yshear);
    }

    if( _sh->frame( *this ) == true )
    {
        return;
    }

    if( preCullCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = preCullCallbacks.begin(); p != preCullCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }
    _sh->cull( *this,0 );

    if( postCullCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postCullCallbacks.begin(); p != postCullCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }

    _rs->makeCurrent();

    if( _block_on_vsync ) 
    {
        _rs->sync();
        _syncTick = _timer.tick();
    }

    _sh->clear( *this );

    if( _sh->useAutoView() )
    {
        applyView();
    }

    if( preDrawCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = preDrawCallbacks.begin(); p != preDrawCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }

    _sh->draw( *this,0 );

    if( postDrawCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postDrawCallbacks.begin(); p != postDrawCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }

    if( doSwap )
        _rs->swapBuffers();

    if( postSwapCallbacks.size() )
    {
        std::vector <ref_ptr<Callback> >::iterator p;
        for( p = postSwapCallbacks.begin(); p != postSwapCallbacks.end(); p++ )
            (*((*p).get()))(*this);
    }
}


const Matrix::value_type * Camera::getViewMatrix( void ) const
{
    return _viewMatrix;
}

void Camera::setViewByMatrix( const Matrix &mat )
{
    Matrix m;
    if ( _offset._multiplyMethod == Offset::PostMultiply )
        m = Matrix( _offset._matrix ) * mat;
    else if( _offset._multiplyMethod == Offset::PreMultiply )
        m = mat * Matrix( _offset._matrix );
    memcpy( _viewMatrix, m.ptr(), sizeof( Matrix::value_type[16] ));
}

void Camera::setViewByLookat( const Vec3 &eye, const Vec3 &center, const Vec3 &up )
{
    Matrix m;
    m.makeLookAt(eye,center,up);
    setViewByMatrix( m );
}

void Camera::setViewByLookat( float eyeX, float eyeY, float eyeZ,
                              float centerX, float centerY, float centerZ,
                              float upX, float upY, float upZ )
{
    setViewByLookat( Vec3(eyeX, eyeY, eyeZ),
                     Vec3(centerX, centerY, centerZ ),
                     Vec3(upX, upY, upZ) );
}


bool Camera::_initialize( void )
{
    if( _rs->realize() == false )
    {
        std::cerr << "Camera::_initialize\n";
        throw 1; 
    }
    if( _lens->getAutoAspect() )
    {
        int x, y;
        unsigned int width, height; 
        _rs->getWindowRectangle(x,y,width,height);
        double ar = (float(width) * (_projrectRight - _projrectLeft))/
                    (float(height) * (_projrectTop - _projrectBottom ));
        _lens->setAspectRatio( ar );
    }
    glEnable( GL_SCISSOR_TEST );
    return _initialized = true;
}

void Camera::applyView()
{
    Matrix::glLoadMatrix( _viewMatrix );
}

Camera::Lens::Lens( void )
{
// original defaults.
//     _left         = -0.5;
//     _right        =  0.5;
//     _bottom       = -0.5;
//     _top          =  0.5;
    
    // Setting of the frustum which are appropriate for
    // a monitor which is 26cm high, 50cm distant from the
    // viewer and an horzintal/vetical aspect ratio of 1.25.
    // This assumed to be a reasonable average setting for end users.
    _left         = -0.32;
    _right        =  0.32;
    _bottom       = -0.26;
    _top          =  0.26;
    _ortho_left   = -1.0;
    _ortho_right  =  1.0;
    _ortho_bottom = -1.0;
    _ortho_top    =  1.0;
    _nearClip     =  1.0;
    _farClip      =  1e6;//100.0;
    _updateFOV();
    _projection = Perspective;
}

void Camera::Lens::setNearAndFarClip(double nearClip, double farClip) //add by wuch
{
	_nearClip     =  nearClip;
	_farClip      =  farClip;
	_updateFOV();
}

void Camera::Lens::setAspectRatio( double aspectRatio )
{
    _aspect_ratio = aspectRatio;
    _left  = -0.5 * (_top - _bottom) * _aspect_ratio;
    _right =  0.5 * (_top - _bottom) * _aspect_ratio;
    _ortho_left  = -0.5 * (_ortho_top - _ortho_bottom) * _aspect_ratio;
    _ortho_right =  0.5 * (_ortho_top - _ortho_bottom) * _aspect_ratio;

    if( _projection == Perspective )
    _updateFOV();
}

void Camera::Lens::setPerspective( double hfov,   double vfov, 
                       double nearClip,   double farClip )
{
    _hfov = deg2rad(hfov);
    _vfov = deg2rad(vfov);
    _aspect_ratio = tan(0.5*_hfov)/tan(0.5*_vfov);

    _nearClip = nearClip;
    _farClip  = farClip;

    _left   = -_nearClip * tan(_hfov/2.0); 
    _right  =  _nearClip * tan(_hfov/2.0); 
    _bottom = -_nearClip * tan(_vfov/2.0);
    _top    =  _nearClip * tan(_vfov/2.0);

    _projection = Perspective;
    setAutoAspect(false);
}

void Camera::Lens::setPerspective2( double fovy,   double aspectRatio, 
								  double nearClip,   double farClip )
{
	double tan_fovy = tan(deg2rad(fovy*0.5));
	_right  =  tan_fovy * aspectRatio * nearClip;
	_left   = -_right;
	_top    =  tan_fovy * nearClip;
	_bottom =  -_top;

	_aspect_ratio = aspectRatio;

	_nearClip = nearClip;
	_farClip  = farClip;

	_projection = Perspective;
	setAutoAspect(false);
}

void Camera::Lens::setFrustum( double left,   double right, 
                                  double bottom, double top, 
                   double nearClip,   double farClip )
{
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
    _nearClip = nearClip;
    _farClip = farClip;
    _projection = Perspective;
    _updateFOV();
    setAutoAspect(false);
}

void Camera::Lens::setOrtho( double left, double right, 
               double bottom, double top, 
               double nearClip, double farClip )
{
    _ortho_left = left;
    _ortho_right = right;
    _ortho_bottom = bottom;
    _ortho_top = top;
    _nearClip = nearClip;
    _farClip = farClip;
    _projection = Orthographic;
    setAutoAspect(false);
}

void Camera::Lens::setMatrix( const Matrix::value_type matrix[16] )
{
    memcpy( _matrix, matrix, sizeof(Matrix::value_type[16]) );
    _projection = Manual;
    setAutoAspect(false);
}

bool Camera::Lens::getFrustum( double& left, double& right,
                double& bottom, double& top,
                double& zNear, double& zFar ) const
{
    //The following code was taken from osg's matrix implementation of getFrustum
    if (_matrix[3]!=0.0 || _matrix[7]!=0.0 || _matrix[11]!=-1.0 || _matrix[15]!=0.0) return false;

    zNear = _matrix[14] / (_matrix[10]-1.0);
    zFar = _matrix[14] / (1.0+_matrix[10]);
    
    left = zNear * (_matrix[8]-1.0) / _matrix[0];
    right = zNear * (1.0+_matrix[8]) / _matrix[0];

    top = zNear * (1.0+_matrix[9]) / _matrix[5];
    bottom = zNear * (_matrix[9]-1.0) / _matrix[5];

    return true;
}

bool Camera::Lens::getOrtho( double& left, double& right,
                double& bottom, double& top,
                double& zNear, double& zFar ) const
{
    //The following code was taken from osg's matrix implementation of getOrtho
    if (_matrix[3]!=0.0 || _matrix[7]!=0.0 || _matrix[11]!=0.0 || _matrix[15]!=1.0) return false;

    zNear = (_matrix[14]+1.0) / _matrix[10];
    zFar = (_matrix[14]-1.0) / _matrix[10];
    
    left = -(1.0+_matrix[12]) / _matrix[0];
    right = (1.0-_matrix[12]) / _matrix[0];

    bottom = -(1.0+_matrix[13]) / _matrix[5];
    top = (1.0-_matrix[13]) / _matrix[5];

    return true;
}

bool Camera::Lens::convertToOrtho( float d )
{

    if( _projection == Manual )
    {
        //Need to extract frustum values from manual matrix
        if( !getFrustum(_left,_right,_bottom,_top,_nearClip,_farClip) )
            return false;

        _updateFOV();
    }

    double s = d * tan(_vfov*0.5);
    _ortho_bottom = -s;
    _ortho_top = s;
    _ortho_left = -s*_aspect_ratio;
    _ortho_right = s*_aspect_ratio;
    _projection = Orthographic;
    return true;
}

bool Camera::Lens::convertToPerspective( float d )
{

    if( _projection == Manual )
    {
        //Need to extract ortho values from manual matrix
        if( !getOrtho(_ortho_left,_ortho_right,_ortho_bottom,_ortho_top,_nearClip,_farClip) )
            return false;
    }

    double hfov = 2 * atan( 0.5 * (_ortho_right - _ortho_left)/d);
    double vfov = 2 * atan( 0.5 * (_ortho_top - _ortho_bottom)/d);

    _left   = -_nearClip * tan(hfov*0.5);
    _right  =  _nearClip * tan(hfov*0.5);
    _bottom = -_nearClip * tan(vfov*0.5);
    _top    =  _nearClip * tan(vfov*0.5);

    _projection = Perspective;
    //_updateMatrix();

    return true;
}

void Camera::Lens::apply(float xshear, float yshear)
{
    glMatrixMode( GL_PROJECTION );
    Matrix::value_type _matrix[16];
    generateMatrix(xshear,yshear,_matrix);
    Matrix::glLoadMatrix( _matrix );
    glMatrixMode( GL_MODELVIEW );
}

void Camera::Lens::getParams( double &left, double &right, double &bottom, double &top,
                double &nearClip, double &farClip )
{
    if( _projection == Perspective )
    {
        left   = _left;
        right  = _right;
        bottom = _bottom;
        top    = _top;
    }
    else if( _projection == Orthographic )
    {
        left   = _ortho_left;
        right  = _ortho_right;
        bottom = _ortho_bottom;
        top    = _ortho_top;
    }
    else if( _projection == Manual ) // could only be Manual, but best to make this clear
    {
        // Check if Manual matrix is either a valid perspective or orthographic matrix
        // If neither, then return bogus values -- nothing better we can do
        if(getFrustum(left,right,bottom,top,nearClip,farClip))
            return;

        if(getOrtho(left,right,bottom,top,nearClip,farClip))
            return;

        left   = _left;
        right  = _right;
        bottom = _bottom;
        top    = _top;
    }
    nearClip   = _nearClip;
    farClip    = _farClip;
}

void Camera::setProjectionRectangle( const float left, const float right,
                    const float bottom, const float top )
{
    _projrectLeft   = left;    
    _projrectRight  = right;    
    _projrectBottom = bottom;    
    _projrectTop    = top;    
}

void Camera::getProjectionRectangle( float &left, float &right,
                float &bottom, float &top ) const
{
    left   = _projrectLeft;
    right  = _projrectRight;
    bottom = _projrectBottom;
    top    = _projrectTop;
}

void Camera::setProjectionRectangle( int x, int y, unsigned int width, unsigned int height ) 
{
    int _x, _y;
    unsigned int _w, _h;

    _rs->getWindowRectangle( _x, _y, _w, _h );
    if( _w == Producer::RenderSurface::UnknownDimension || _h == Producer::RenderSurface::UnknownDimension)
    {
        unsigned int ww;
        unsigned int hh;
        _rs->getScreenSize( ww, hh );
        if( _w == Producer::RenderSurface::UnknownDimension )
            _w = ww;
        if( _h == Producer::RenderSurface::UnknownDimension )
            _h = hh;
    }

    _projrectLeft  = float(x - _x)/float(_w);
    _projrectRight = float((x + width) - _x)/float(_w);
    _projrectBottom = float(y - _y)/float(_h);
    _projrectTop    = float((y+height) - _y)/float(_h);
}

void Camera::getProjectionRectangle( int &x, int &y, unsigned int &width, unsigned int &height ) const
{
    int _x, _y;
    unsigned int _w, _h;
    float fx, fy, fw, fh;

    _rs->getWindowRectangle( _x, _y, _w, _h );
    if( _w == Producer::RenderSurface::UnknownDimension || _h == Producer::RenderSurface::UnknownDimension )
    {
        unsigned int ww;
        unsigned int hh;
        _rs->getScreenSize( ww, hh );
        if( _w == Producer::RenderSurface::UnknownDimension )
            _w = ww;
        if( _h == Producer::RenderSurface::UnknownDimension )
            _h = hh;
    }

    fx = _projrectLeft * _w;
    fy = _projrectBottom * _h;
    fw = _w * _projrectRight;
    fh = _h * _projrectTop;

    x = int(fx);
    y = int(fy);

    width = int(fw) - x;
    height = int(fh) - y;
}

void Camera::setClearColor( float r, float g, float b, float a )
{
    _clear_color[0] = r;
    _clear_color[1] = g;
    _clear_color[2] = b;
    _clear_color[3] = a;
}

void Camera::getClearColor( float& red, float& green, float& blue, float& alpha)
{
    red = _clear_color[0];
    green = _clear_color[1];
    blue = _clear_color[2];
    alpha = _clear_color[3];
}


void Camera::clear( void ) 
{
    if( !_initialized ) _initialize();
    int x, y; 
    unsigned int w, h;
    getProjectionRectangle( x, y, w, h );
    glViewport( x, y, w, h );
    glScissor( x, y, w, h );
    glClearColor( _clear_color[0], _clear_color[1], _clear_color[2], _clear_color[3] );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}


#if 0
void Camera::Lens::_updateMatrix( void )
{
    switch( _projection )
    {
        case Perspective :
            _matrix[ 0] = (2 * _nearClip)/(_right - _left);
            _matrix[ 1] = 0.0;
            _matrix[ 2] = 0.0;
            _matrix[ 3] = 0.0;

            _matrix[ 4] = 0.0;
            _matrix[ 5] = (2 * _nearClip)/(_top-_bottom);
            _matrix[ 6] = 0.0;
            _matrix[ 7] = 0.0;

            _matrix[ 8] = (_right + _left)/(_right-_left);
            _matrix[ 9] = (_top+_bottom)/(_top-_bottom);
            _matrix[10] = -(_farClip + _nearClip)/(_farClip-_nearClip);
            _matrix[11] = -1.0;

            _matrix[12] = 0.0;
            _matrix[13] = 0.0;
            _matrix[14] = -(2 * _farClip * _nearClip)/(_farClip-_nearClip);
            _matrix[15] = 0.0;

            _matrix[ 8] += -_xshear;
            _matrix[ 9] += -_yshear;

        _hfov = 2.0 * atan(((_right - _left) * 0.5)/_nearClip);
        _vfov = 2.0 * atan(((_top - _bottom) * 0.5)/_nearClip);

        break;

    case Orthographic :

            _matrix[ 0] = 2/(_ortho_right - _ortho_left);
            _matrix[ 1] = 0.0;
            _matrix[ 2] = 0.0;
            _matrix[ 3] = 0.0;

            _matrix[ 4] = 0.0;
            _matrix[ 5] = 2/(_ortho_top - _ortho_bottom);
            _matrix[ 6] = 0.0;
            _matrix[ 7] = 0.0;

            _matrix[ 8] = 0.0;
            _matrix[ 9] = 0.0;
            //_matrix[10] = -2.0/(_farClip - (-_farClip));
            _matrix[10] = -2.0/(_farClip - _nearClip);
            _matrix[11] = 0.0;

            _matrix[12] = -(_ortho_right+_ortho_left)/(_ortho_right-_ortho_left);
            _matrix[13] = -(_ortho_top+_ortho_bottom)/(_ortho_top-_ortho_bottom);
            //_matrix[14] = -(_farClip+(-_farClip))/(_farClip-(-_farClip));
            _matrix[14] = -(_farClip+_nearClip)/(_farClip-_nearClip);
            _matrix[15] = 1.0;

            _matrix[12] += _xshear;
            _matrix[13] += _yshear;

            //_hfov = 0.0;
            //_vfov = 0.0;

        break;
    }
}
#endif

void Camera::Lens::generateMatrix(float xshear, float yshear, Matrix::value_type matrix[16] )
{
    switch( _projection )
    {
        case Perspective :
            matrix[ 0] = (2 * _nearClip)/(_right - _left);
            matrix[ 1] = 0.0;
            matrix[ 2] = 0.0;
            matrix[ 3] = 0.0;

            matrix[ 4] = 0.0;
            matrix[ 5] = (2 * _nearClip)/(_top-_bottom);
            matrix[ 6] = 0.0;
            matrix[ 7] = 0.0;

            matrix[ 8] = (_right + _left)/(_right-_left);
            matrix[ 9] = (_top+_bottom)/(_top-_bottom);
            matrix[10] = -(_farClip + _nearClip)/(_farClip-_nearClip);
            matrix[11] = -1.0;

            matrix[12] = 0.0;
            matrix[13] = 0.0;
            matrix[14] = -(2 * _farClip * _nearClip)/(_farClip-_nearClip);
            matrix[15] = 0.0;

            matrix[ 8] += -xshear;
            matrix[ 9] += -yshear;


        break;

    case Orthographic :

            matrix[ 0] = 2/(_ortho_right - _ortho_left);
            matrix[ 1] = 0.0;
            matrix[ 2] = 0.0;
            matrix[ 3] = 0.0;

            matrix[ 4] = 0.0;
            matrix[ 5] = 2/(_ortho_top - _ortho_bottom);
            matrix[ 6] = 0.0;
            matrix[ 7] = 0.0;

            matrix[ 8] = 0.0;
            matrix[ 9] = 0.0;
            //_matrix[10] = -2.0/(_farClip - (-_farClip));
            matrix[10] = -2.0/(_farClip - _nearClip);
            matrix[11] = 0.0;

            matrix[12] = -(_ortho_right+_ortho_left)/(_ortho_right-_ortho_left);
            matrix[13] = -(_ortho_top+_ortho_bottom)/(_ortho_top-_ortho_bottom);
            //_matrix[14] = -(_farClip+(-_farClip))/(_farClip-(-_farClip));
            matrix[14] = -(_farClip+_nearClip)/(_farClip-_nearClip);
            matrix[15] = 1.0;

            matrix[12] += xshear;
            matrix[13] += yshear;

        break;

    case Manual:

        memcpy( matrix, _matrix, sizeof(Matrix::value_type[16]));

        if(xshear || yshear)
        {
            if (matrix[3]!=0.0 || matrix[7]!=0.0 || matrix[11]!=0.0 || matrix[15]!=1.0)
            {
                // It's not an orthographic matrix so just assume a perspective shear
                matrix[ 8] += -xshear;
                matrix[ 9] += -yshear;
            } 
            else
            {
                 matrix[12] += xshear;
                 matrix[13] += yshear;
            }
        }
        break;
    }
}

void Camera::Lens::_updateFOV()
{
    _hfov = 2.0 * atan(((_right - _left) * 0.5)/_nearClip);
    _vfov = 2.0 * atan(((_top - _bottom) * 0.5)/_nearClip);
    _aspect_ratio = tan(0.5*_hfov)/tan(0.5*_vfov);
}


void Camera::setOffset( const Matrix::value_type matrix[16], double xshear, double yshear )
{
    memcpy( _offset._matrix, matrix, sizeof(Matrix::value_type[16]));
    _offset._xshear = xshear;
    _offset._yshear = yshear;
}

void Camera::setOffset(  double xshear, double yshear )
{
    _offset._xshear = xshear;
    _offset._yshear = yshear;
}

void Camera::setSyncBarrier( RefBarrier *b )
{
    _syncBarrier = b;
}

void Camera::setFrameBarrier(RefBarrier *b, RefBarrier *b2)
{
    _frameBarrier = b;
	_frameBarrier2 = b2;
}

void Camera::setCullBarrier( Producer::RefBarrier *b )
{
	_cullBarrier = b;
}

void Camera::setUpdateBarrier( Producer::RefBarrier *b )
{
	_updateBarrier = b;
}

void Camera::setDrawBarrier( Producer::RefBarrier *b )
{
	_drawBarrier = b;
}

int Camera::cancel()
{
#if 1
    _done = 1;
#endif

	//if(_cull_updatethread.valid()) _cull_updatethread->cancel(); //add by wch
	//if(_drawthread.valid()) _drawthread->cancel();               //add by wch
	
	while(_cull_updatethread.valid() && _cull_updatethread->isRunning())
	{
		CRCore::crThread::yieldCurrentThread();
	}
	while(_drawthread.valid() && _drawthread->isRunning())
	{
		CRCore::crThread::yieldCurrentThread();
	}

    crThread::cancel();
    return 0;    
}

void Camera::run( void )
{
    if( !_syncBarrier.valid() || !_frameBarrier.valid() )
    {
        std::cerr << "Camera::run() : Threaded Camera requires a Barrier\n";
        return;
    }

    _done = 0;

    _initialize();
    _syncBarrier->block();
    while( !_done )
    {
        // printf("   Camera::run before frame block\n");

        _frameBarrier->block();
        
        if (_done) break;

        // printf("   Camera::run after frame block\n");

        frame(false);

        if (_done) break;

        // printf("   Camera::run before sycn block\n");

        _syncBarrier->block();

        if (_done) break;

        // printf("   Camera::run after sycn block\n");

        advance();
    }

	_sh->release(); //add by wch
//#ifdef _WIN32_IMPLEMENTATION //add by wch
	_rs->releaseCurrent(); //add by wch
//#endif                     //add by wch

    // printf("Exiting Camera::run cleanly\n");
}


const Camera::FrameTimeStampSet &Camera::getFrameStats()
{ 
    return _frameStamps; 
}

bool Camera::removePreCullCallback( Callback *cb )
{
    return _removeCallback( preCullCallbacks, cb );
}

bool Camera::removePostCullCallback( Callback *cb )
{
    return _removeCallback( postCullCallbacks, cb );
}

bool Camera::removePreDrawCallback( Callback *cb ) 
{
    return _removeCallback( preDrawCallbacks, cb );
}

bool Camera::removePostDrawCallback( Callback *cb ) 
{
    return _removeCallback( postDrawCallbacks, cb );
}

bool Camera::removePostSwapCallback( Callback *cb )
{
    return _removeCallback( postSwapCallbacks, cb );
}


bool Camera::_removeCallback( std::vector < ref_ptr<Callback> > &callbackList, Callback *callback )
{
    std::vector < Producer::ref_ptr< Producer::Camera::Callback> >::iterator p;
    p = std::find( callbackList.begin(), callbackList.end(), callback );
    if( p == callbackList.end() )
        return false;

    callbackList.erase( p );
    return true;
}

Cull_UpdateThread::~Cull_UpdateThread() //add by wch
{
	//std::cerr << "~Cull_UpdateThread()" << std::endl;
}

DrawThread::~DrawThread() //add by wch
{
	//std::cerr << "~DrawThread()" << std::endl;
}

void Cull_UpdateThread::run( void ) //add by wch
{
	if( !_camera->_syncBarrier.valid() || !_camera->_cullBarrier.valid() || !_camera->_updateBarrier.valid())
	{
		std::cerr << "Camera::run() : Threaded Camera requires a Barrier\n";
		return;
	}
	setSchedulePriority(THREAD_PRIORITY_HIGH);
	_camera->_syncBarrier->block();
	//_camera->_done = false;
	while (!_camera->_done)
	{
		// printf("   Camera::run before frame block\n");

		_camera->_updateBarrier->block();
		//if (_camera->_done) break;

		_camera->_cullBarrier->block();

		if (_camera->isEnabled())
		{
			_camera->update();
			if (_camera->_vrHMD)
			{
				_camera->cull(1);
				_camera->_frameBarrier2->block();
				_camera->cull(2);
				_camera->_frameBarrier->block();
			}
			else
			{
				_camera->cull();
				_camera->_frameBarrier->block();
			}
		}
		//_camera->cull_update();

		//if (_camera->_done) break;

		// printf("   Camera::run after frame block\n");

		//if (_camera->_done) break;


		// printf("   Camera::run before sycn block\n");

		//if (_camera->_done) break;

		// printf("   Camera::run after sycn block\n");

	}
	_camera->_done = 2;
	//_camera->_vrHMD = false;
	//printf("   Cull_UpdateThread::run end before sync\n");
	_camera->_syncBarrier->block();
	//printf("   Cull_UpdateThread::run end\n");
}

void DrawThread::run( void ) //add by wch
{	

	if( !_camera->_syncBarrier.valid() || !_camera->_drawBarrier.valid() )
	{
		std::cerr << "Camera::run() : Threaded Camera requires a Barrier\n";
		return;
	}
	setSchedulePriority(THREAD_PRIORITY_HIGH);
	_camera->_initialize();
	_camera->_syncBarrier->block();
	//_camera->_done = false;
	do
	{
		// printf("   Camera::run before frame block\n");

		//if (_camera->_done) break;

		// printf("   Camera::run after frame block\n");

		//this->setSchedulePriority(crThread::THREAD_PRIORITY_HIGH);
		if (_camera->isEnabled())
		{
			if (_camera->_vrHMD)
			{
				_camera->_frameBarrier2->block();
				_camera->draw(false,1);
				//_camera->advance();
				_camera->_frameBarrier->block();
				_camera->draw(false,2);
				_camera->advance();
			}
			else
			{
				_camera->_frameBarrier->block();
				_camera->draw(false);
				_camera->advance();
			}
		}
		//if (_camera->_done) break;

		// printf("   Camera::run before sycn block\n");
		//_camera->advance();
		_camera->_drawBarrier->block();

		//if (_camera->_done) break;

		// printf("   Camera::run after sycn block\n");

		//this->setSchedulePriority(crThread::THREAD_PRIORITY_LOW);
		//_camera->advance();////20150915移动到_camera->_drawBarrier->block();前
	}
	while (_camera->_done!=2);
	//_camera->_frameBarrier->release();
	//printf("   DrawThread::run end \n");
	_camera->_sh->release();
	//printf("   _camera->_sh->release() end \n");
//#ifdef _WIN32_IMPLEMENTATION
	_camera->_rs->releaseCurrent();
//#endif
	//printf("   DrawThread::run end before sync\n");
	//printf("   DrawThread::run end\n");
	_camera->_sh = NULL;
	_camera->_updateCallback = NULL;
	_camera->preFrameCallbacks.clear();
	_camera->preCullCallbacks.clear();
	_camera->postCullCallbacks.clear();
	_camera->preDrawCallbacks.clear();
	_camera->postDrawCallbacks.clear();
	_camera->postSwapCallbacks.clear();
	_camera->postFrameCallbacks.clear();

	_camera->_syncBarrier->block();
}
