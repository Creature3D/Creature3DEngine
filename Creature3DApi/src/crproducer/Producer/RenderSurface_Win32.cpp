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
#include <stdio.h>
#include <iostream>
#include <map>
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <CRProducer/Producer/RenderSurface>
#include <CRProducer/Producer/Events>
#include <CRProducer/Producer/Keyboard>
#include <CRProducer/Producer/wglext.h>		//WGL extensions
#include <CRProducer/Producer/glext.h>		//GL extensions
#include "WGLExtensions.h"
#include <CRCore/crBrain.h>
#include <CRCore/crBlockDetectThread.h>
#include <CRCore/crStreamBuf.h>
//#include <CRCore/crDisplaySettings.h>
#include <CRCore/crFilterRenderManager.h>
//#include <imm.h>
#pragma comment( lib, "imm32.lib" )
using namespace Producer;

// Class statics
std::map <Window, RenderSurface *>RenderSurface::registry;

unsigned int RenderSurface::getNumberOfScreens(void) { return 1; }

RenderSurface::Client::Client(unsigned long mask): _mask(mask)
{
    q = new EventQueue;
}

void RenderSurface::Client::queue( ref_ptr<Event> ev ) 
{ 
    q->push_back( ev ); 
}


LONG WINAPI RenderSurface::s_proc( Window hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    std::map <Window, RenderSurface *>::iterator p;
    p = registry.find( hWnd );
    if( p == registry.end() )
        return DefWindowProc( hWnd, uMsg, wParam, lParam );
    else
        return p->second->proc( hWnd, uMsg, wParam, lParam );
        
}

void RenderSurface::dispatch( ref_ptr<Event> ev )
{
    if( clients.size() == 0/* || !_focuse */) return;

    std::vector <Producer::ref_ptr<Client> >::iterator p;
    for( p = clients.begin(); p != clients.end(); p++ )
    {
#if 0
        if( ev->type() == Event::MouseMotion )
        {
            // If MouseMotionMask is set, then send event regardless of
            // Mouse button state.
            if( (*p)->mask() & Event::MouseMotionMask )
                (*p)->queue(ev);
            // Otherwise, send event only if mouse button is pressed
            else if( (*p)->mask() & Event::ButtonMotionMask && _mbutton )
                (*p)->queue(ev);
        }
        else if( (*p)->mask() & (1<<ev->type()) )
            (*p)->queue( ev ); 
#endif

        if( (*p)->mask() & (1<<ev->type()) )
        {                
            (*p)->queue( ev ); 
        }
    } 
}


LONG WINAPI RenderSurface::proc( Window hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    LONG lRet = 1;
    //RECT rect;
    ref_ptr<Event> ev;

	if (CRCore::crDisplaySettings::instance()->getRunMode() == 0)
	{
		switch(uMsg)
		{
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		//case WM_XBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		//case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		//case WM_XBUTTONUP:
		//case WM_MOUSEMOVE:
			::SendMessage(hWnd,WM_ACTIVATE,WA_ACTIVE,NULL);
			break;
		}
	}
    switch (uMsg)
    {
	//case WM_IME_NOTIFY:
	//	//if (_ownWindow)
	//	//	lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	//	//else
	//	//	lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
	//	break;
	case WM_ERASEBKGND:
		return true;
	case WM_USER+1:
		if(wParam==0)
		{
			HIMC hImc = ImmGetContext(hWnd);
			if(hImc)
			{
				::ImmSetOpenStatus(hImc, FALSE);
				::ImmReleaseContext(hWnd,hImc);
			}
		}
		else
		{
			HIMC hImc = ImmGetContext(hWnd);
			if(hImc)
			{
				::ImmSetOpenStatus(hImc, TRUE);
				::ImmReleaseContext(hWnd,hImc);
			}
		}
		break;
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 522
#endif
    case WM_MOUSEWHEEL:
        {
            KeyboardMouseCallback::ScrollingMotion scroll = 
                    (signed int)(wParam) < 0 ? KeyboardMouseCallback::ScrollDown:
                    KeyboardMouseCallback::ScrollUp;
            ev = new MouseScrollEvent(hWnd,scroll);
            dispatch(ev);
        }
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
         break;

    case WM_MOUSEMOVE :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new MouseMotionEvent( hWnd, _mx, _my );
        dispatch( ev );
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_LBUTTONDOWN :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new ButtonPressEvent( hWnd, 1, _mx, _my );
        dispatch( ev );
        _mbutton |= 1L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_LBUTTONUP :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev =  new ButtonReleaseEvent( hWnd, 1, _mx, _my );
        dispatch( ev );
        _mbutton &= ~1L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_LBUTTONDBLCLK :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new DoubleButtonPressEvent( hWnd, 1, _mx, _my );
        dispatch( ev );
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_RBUTTONDOWN :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new ButtonPressEvent( hWnd, 3, _mx, _my );
        dispatch( ev );
        _mbutton |= 4L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_RBUTTONUP  :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new ButtonReleaseEvent( hWnd, 3, _mx, _my );
        dispatch( ev );
        _mbutton &= ~4L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_RBUTTONDBLCLK : 
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        dispatch( new DoubleButtonPressEvent( hWnd, 3, _mx, _my ));
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_MBUTTONDOWN  :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new ButtonPressEvent( hWnd, 2, _mx, _my );
        dispatch( ev );
        _mbutton |= 2L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_MBUTTONUP :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new ButtonReleaseEvent( hWnd, 2, _mx, _my );
        dispatch( ev );
        _mbutton &= ~2L;
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_MBUTTONDBLCLK :
        _mx = ((int)(short)LOWORD(lParam));
        _my = ((int)(short)HIWORD(lParam));

        ev = new DoubleButtonPressEvent( hWnd, 2, _mx, _my );
        dispatch( ev );
        //if (!_ownWindow)
        //    lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

	case WM_PAINT:
		if (_ownWindow)
		{
			PAINTSTRUCT    ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		else
			lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		break;
	case WM_SETFOCUS:
		CRCore::crDisplaySettings::instance()->restoreFpsControl();
		if(CRCore::crDisplaySettings::instance()->getFullScreen())
			::ShowWindow(hWnd,SW_RESTORE);
		//CRCore::crBlockDetectThread::getInstance()->resume();
		//CRCore::crBrain::getInstance()->resumeGame();
		//ev = new WindowRestoredEvent( hWnd,1 );
		//dispatch(ev);
		if (!_ownWindow)
			lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		break;
	case WM_KILLFOCUS:
		if(CRCore::crDisplaySettings::instance()->getRunMode()!=0)
		{
			CRCore::crDisplaySettings::instance()->setFpsControl(20.0f);
			if(CRCore::crDisplaySettings::instance()->getFullScreen())
				::ShowWindow(hWnd,SW_MINIMIZE);
		}
		//CRCore::crBlockDetectThread::getInstance()->pause();
		//CRCore::crBrain::getInstance()->pauseGame();
		ev = new WindowRestoredEvent( hWnd,0 );
		dispatch(ev);
		if (!_ownWindow)
			lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		break;
    case WM_SIZE:
		{
			int newWidth = (int)(short)(LOWORD(lParam));
			int newHeight = (int)(short)(HIWORD(lParam));
			if (newWidth != _windowWidth || newHeight != _windowHeight)
			{
				CRCore::crDisplaySettings::instance()->setViewSize(newWidth, newHeight);
				CRCore::crFilterRenderManager::getInstance()->resize();
				setWindowRectangle(_windowX, _windowY, newWidth, newHeight, false);
			}
			if (wParam == SIZE_MINIMIZED)
			{
				CRCore::crDisplaySettings::instance()->setFpsControl(10.0f);
				//CRCore::crBlockDetectThread::getInstance()->pause();
				//CRCore::crBrain::getInstance()->pauseGame();
				ev = new WindowRestoredEvent(hWnd, 0);
				dispatch(ev);
			}
			else if (wParam == SIZE_RESTORED)
			{
				CRCore::crDisplaySettings::instance()->restoreFpsControl();
				//CRCore::crBlockDetectThread::getInstance()->resume();
				//CRCore::crBrain::getInstance()->resumeGame();
				ev = new WindowRestoredEvent(hWnd, 1);
				dispatch(ev);
			}
			if (!_ownWindow)
				lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		}
		break;
	//case WM_EXITSIZEMOVE:
	//	setWindowRectangle(_windowX, _windowY, CRCore::crDisplaySettings::instance()->getViewWidth(), CRCore::crDisplaySettings::instance()->getViewHeight(), false);
	//	break;
	case WM_CREATE:
    case WM_MOVE:
        // only change window dimensions for non-child, 
        // non-fullscreen windows (which are all that 
        // should be moved or resized directly.
        //if (!(_isFullScreen || _parent))
        //{
        //    GetClientRect(hWnd, &rect);
        //    _windowWidth = rect.right - rect.left;
        //    _windowHeight = rect.bottom - rect.top;

        //    // map the window origin to the screen
        //    POINT p;
        //    p.x = rect.left;
        //    p.y = rect.top;
        //    ClientToScreen(hWnd, &p);

        //    // flip to Y up convention
        //    _windowX = p.x;
        //    _windowY = _screenHeight - p.y - _windowHeight;

        //}

        ev = new WindowConfigEvent( hWnd, _windowX, _windowY,
                                        _windowWidth,
                                        _windowHeight);
        dispatch(ev);
        if (!_ownWindow)
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_CLOSE:
        if (!_ownWindow)
        {
            // PROBLEM: can't call KillGLWindow() here because the GL context
            // was created in another thread and releasing it now would cause 
            // an error!
            //KillGLWindow();
            _realized = false;
            SetWindowLong(_win, GWL_WNDPROC, (long)_oldWndProc);
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
            ev = new ShutdownEvent(hWnd,1);
            dispatch(ev);
        }
        else
		{
			ev = new ShutdownEvent(hWnd,0);
			dispatch(ev);
			//DestroyWindow (hWnd);                
			//std::cerr << "DestroyWindow"<<hWnd<< std::endl;
		}
        break;

    case WM_QUIT:
    case WM_DESTROY:
        if (!_ownWindow)
        {
                        // PROBLEM: can't call KillGLWindow() here because the GL context
                        // was created in another thread and releasing it now would cause 
                        // an error!
            //KillGLWindow();
            _realized = false;
            SetWindowLong(_win, GWL_WNDPROC, (long)_oldWndProc);
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        }
        //else
        //{
        //    PostQuitMessage (0);
        //    _realized = false;
        //}
        ev = new ShutdownEvent(hWnd,1);
        dispatch(ev);
        break;

    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        {
            KeyCharacter keychar = Keyboard::getSingleton()->win32KeyEventToKeyChar( lParam, wParam );
            ev = new KeyPressEvent(hWnd, keychar);
            dispatch( ev );
        }
        if (!_ownWindow)
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_SYSKEYUP:
    case WM_KEYUP:
        {
            KeyCharacter keychar = Keyboard::getSingleton()->win32KeyEventToKeyChar( lParam, wParam );
            ev = new KeyReleaseEvent(hWnd, keychar);
            dispatch( ev );
        }
        if (!_ownWindow)
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;
	case WM_CHAR:
		{
			//char c = LOWORD(wParam);
			//if(c>=0)
			//	win->pushChar(c);
			wchar_t c = LOWORD(wParam);
			ev = new InputCharEvent(hWnd, c);
			dispatch( ev );
		}
		if (!_ownWindow)
			lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_IME_CHAR:
		{
			wchar_t c = LOWORD(wParam);
			ev = new InputCharEvent(hWnd, c);
			dispatch( ev );
		}
		if (!_ownWindow)
			lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
		return 0;
        /*
    case WM_CHAR:
        if (lParam >> 31) {     // this bit will be set if this event is a key release
            ev = new KeyReleaseEvent(wParam);
        } else {
            ev = new KeyPressEvent(wParam);
        }
        dispatch( ev );

        if( _use_default_esc && wParam == VK_ESCAPE )
        {
            PostQuitMessage(0);
            return 0;
        }
        */

    case WM_TIMER:
        ev = new TimerEvent(hWnd, wParam);
        dispatch( ev );
        if (!_ownWindow)
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;

    case WM_SETCURSOR:
    {
        long lRet = TRUE;
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (!_ownWindow)
                lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
            _setCursor(_currentCursor);
        }
        else
        {
            if (_ownWindow)
                lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
            else
                lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        }
        break;
    }
	//case WM_SETFOCUS:
	//{
	//	if(CRCore::crDisplaySettings::instance()->getRunMode() == 0)
	//		CRCore::crBrain::getInstance()->resumeGame();
	//	//SetFocus(hWnd);
	//	//_focuse = true;
	//	//ev = new WindowFocusEvent(hWnd, 1, (Producer::Window)wParam);
	//	//dispatch( ev );
	//	break;
	//}
	//case WM_KILLFOCUS:
	//{
	//	if(CRCore::crDisplaySettings::instance()->getRunMode() == 0)
	//		CRCore::crBrain::getInstance()->pauseGame();
	//	//ev = new WindowFocusEvent(hWnd, 0, (Producer::Window)wParam);
	//	//dispatch( ev );
	//	//_focuse = false;
	//	break;
	//}
	//case WM_ACTIVATE:
	//	{
	//		if(LOWORD(wParam)==WA_INACTIVE)
	//		{
	//			Keyboard::getSingleton()->clearKeyModifier();
	//		}
	//		break;
	//	}
	//{
		//ev = new WindowFocusEvent(hWnd, LOWORD(wParam)!=WA_INACTIVE, (Producer::Window)lParam);
		//dispatch( ev );
		//verifyPosition();
		//break;
	//}
    default:
        if (_ownWindow)
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
        else
            lRet = CallWindowProc(_oldWndProc, hWnd, uMsg, wParam, lParam);
        break;
    }
    return lRet;
} 

bool RenderSurface::realize( VisualChooser *vc, GLContext sharedGLContext )
{
    if( _realized ) 
	{
		return _realized;
	}

    if( sharedGLContext != 0L )
        _sharedGLContext = sharedGLContext;
    else
        _sharedGLContext = _globallySharedGLContext;

    if( vc != NULL )
        _visualChooser = vc;
    if( _useConfigEventThread && _ownWindow && _drawableType == DrawableType_Window )
    {
        if(!isRunning())
		{
			startThread();
			_threadReady->block();
		}
		//else
		//{
		//	_initgl();
		//}
		ShowWindowAsync(_win, SW_SHOW );
    }
    else
	{
        _init();
		if(_drawableType == DrawableType_Window)
		{
			ShowWindowAsync(_win, SW_SHOW );
		}
	}
        
    makeCurrent();

    if (_sharedGLContext)
        wglShareLists(_sharedGLContext, _glcontext);

    _realized = true;

    std::vector <Producer::ref_ptr<Callback> >::iterator p;
    for( p = _realizeCallbacks.begin(); p != _realizeCallbacks.end(); p++ )
    {
        if( (*p).valid() )
            (*(*p).get())( *this );
    }
    _realizeCallbacks.clear();

    _realizeBlock->release();

    return _realized;
}


static void doInternalError( char *msg )
{
    return;
    LPVOID lpMsgBuf;
    FormatMessage(   FORMAT_MESSAGE_ALLOCATE_BUFFER |
                     FORMAT_MESSAGE_FROM_SYSTEM |
                     FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,\
                     0,NULL 
                     );
    char *szMessage = new char[strlen(msg)+5+strlen("Producer: Internal Error \n ")+2];
    sprintf( szMessage, "Producer: Internal Error \n %s\0", msg );
    MessageBox( NULL, (LPCTSTR)lpMsgBuf, szMessage, MB_OK | MB_ICONINFORMATION );
    LocalFree( lpMsgBuf );
    delete szMessage;
}


void RenderSurface::KillGLWindow()
{
    //if (_glcontext)
    //{
    //    makeCurrent();
    //}

    //if (_isFullScreen)
    //{
    //    // Switch Back To The Desktop
    //    ChangeDisplaySettings(NULL,0);    
        ShowCursor(TRUE);
    //}

    wglMakeCurrent( NULL, NULL );
    
    if (_glcontext && !wglDeleteContext(_glcontext))
    {
        //doInternalError("Release Rendering Context Failed.");
    }
    _glcontext = NULL;

    if (_hdc && !ReleaseDC(_win,_hdc))
    {
        //doInternalError("Release Device Context Failed.");
    }
    _hdc = NULL;
    
	//_fini();
    if (_ownWindow)
    {
		//std::cerr << "KillGLWindow "<<_win<< std::endl;
		//if(_parent)
		//{
		//	::SetParent(_win,::GetDesktopWindow());
		//}
        if (_win && !DestroyWindow(_win))
        {
            //doInternalError("Could Not Release hWnd.");
        }
        _win = NULL;
        if (!UnregisterClass(_windowName.c_str(),_hinstance))
        {
            //doInternalError("Could Not Unregister Class.");
        }
        _hinstance = NULL;
    }
}
//void RenderSurface::verifyPosition()
//{
//	RECT winrect;
//	::GetWindowRect( _win, &winrect );
//	RECT screenrect;
//	screenrect.left = ::GetSystemMetrics( SM_XVIRTUALSCREEN );
//	screenrect.top = ::GetSystemMetrics( SM_YVIRTUALSCREEN );
//	screenrect.right = ::GetSystemMetrics( SM_CXVIRTUALSCREEN );
//	screenrect.bottom = ::GetSystemMetrics( SM_CYVIRTUALSCREEN );
//
//	if( winrect.left >= screenrect.right+4 || winrect.left < screenrect.left-4 )
//		winrect.left = 0;
//	if( winrect.top >= screenrect.bottom+4 || winrect.top < screenrect.top-4 )
//		winrect.top = 0;
//
//	::SetWindowPos( _win, HWND_TOP, winrect.left, winrect.top, winrect.right, winrect.bottom, SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOSIZE );
//}
void RenderSurface::setParentWindow( Window parent )
{
	if(_parent == parent)
		return;
	_parent = parent;
	if(_inited && _win)
		::SetParent(_win,_parent);
}
bool RenderSurface::_init()
{
	if(_inited) return true;
	//static multiSampleFormat = 0;

    if (_drawableType == DrawableType_Window)
    {
        _hinstance    = GetModuleHandle(NULL);
		
		DEVMODE dm;
        RECT rect;
        memset(&dm,0,sizeof(dm));
        dm.dmSize = sizeof(dm);
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
        int x, y, w, h;

        _screenWidth  = dm.dmPelsWidth;
        _screenHeight = dm.dmPelsHeight;


        if (_ownWindow )
        {
            DWORD dwExStyle = 0;
            DWORD dwStyle = 0;
            WNDCLASS wndclass;

            _defaultCursor = LoadCursor( NULL, IDC_ARROW);
            _currentCursor = _defaultCursor;

            wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS; 
            wndclass.lpfnWndProc   = (WNDPROC)s_proc;
            wndclass.cbClsExtra    = 0;
            wndclass.cbWndExtra    = 0;
            wndclass.hInstance     = _hinstance;
            wndclass.hIcon         = LoadIcon( _hinstance, "CREATURE3D_ICON" );
            wndclass.hCursor       = _defaultCursor;
            wndclass.hbrBackground = NULL;
            wndclass.lpszMenuName  = 0;

            wndclass.lpszClassName = _windowName.c_str();

            // mew 2003-12-14
            // if the .exe doesn't override the PRODUCER_ICON resource
            // definition, fall back to the icon in the producer .dll.
//#ifdef _DEBUG
//#  define LIBNAME "Producerd.dll"
//#else
//#  define LIBNAME "Producer.dll"
//#endif
//            if (wndclass.hIcon == NULL)
//            {
//                // Don - 2004-10-28  
//                // Huh??  If this code is running, the library is loaded, no?  
//                // Why do we have to LoadLibrary here?
//                HINSTANCE hDLLInstance = LoadLibrary( LIBNAME );
//                if (hDLLInstance != NULL)
//                {
//                    wndclass.hIcon = LoadIcon(hDLLInstance, "PRODUCER_ICON");
//                }
//            }
			//std::cerr << "RegisterClass "<<_windowName.c_str()<< std::endl;

            //RegisterClass(&wndclass);
			if (!RegisterClass(&wndclass))
			{
				UnregisterClass(_windowName.c_str(), _hinstance);
				RegisterClass(&wndclass);
			}

            if (_parent)
            {
                _isFullScreen = false;
            }

            if (_isFullScreen)
            {
                if (ChangeDisplaySettings(&dm,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
                {
                    // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
                    if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
                    {
                        _isFullScreen = FALSE;
                    }
                    else
                    {
                        doInternalError("Program Will Now Close.");
                        return FALSE;
                    }
                }
            }

            if (_parent)
            {
                //GetClientRect(_parent, &rect);

                // style is a child window
                //dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
				dwExStyle = NULL/*WS_EX_NOPARENTNOTIFY*/;
                dwStyle   = WS_CHILD;

                //AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

                //_windowWidth = (rect.right - rect.left);
                //_windowX = 0;

                //_windowHeight = (rect.bottom - rect.top);
                //_windowY = 0;

                x = _windowX;
                y = _windowY;
                w = _windowWidth;
                h = _windowHeight;
				//x = (_screenWidth - _windowWidth) * 0.5f;
				//y = _windowHeight - _screenHeight;
				//w = _windowWidth;
				//h = _windowHeight;
            }
            else if (_isFullScreen)
            {
                dwExStyle = WS_EX_APPWINDOW;
                dwStyle   = WS_POPUP | WS_MAXIMIZE;
				//dwStyle   = WS_THICKFRAME | WS_MAXIMIZE | WS_SYSMENU;

                //if (_useCustomFullScreen)
                //{
                //    x = _customFullScreenOriginX;
                //    y = _screenHeight - (_customFullScreenOriginY + _customFullScreenHeight);
                //    w = _customFullScreenWidth;
                //    h = _customFullScreenHeight;
                //}
                //else
                //{
                    x = 0;
                    y = 0;
                    w = _screenWidth;
                    h = _screenHeight;
                //}
            }
            else // own window, no parent, not fullscreen
            {
                if( _windowWidth == UnknownDimension ) 
                {
                    _windowWidth = _screenWidth/2;
                    _windowX = _windowWidth/2;
                } 

                if ( _windowHeight == UnknownDimension ) 
                {
                    _windowHeight = _screenHeight/2;
                    _windowY = _windowHeight/2;
                } 

                dwExStyle = WS_EX_APPWINDOW;
                // map the Y up dimensions to Y down for window
                rect.left = _windowX;
                rect.top = _windowY;//_screenHeight - _windowY - _windowHeight;
                rect.bottom = rect.top + _windowHeight;//_screenHeight - _windowY;
                rect.right = rect.left + _windowWidth;

                if(_decorations) {
                  // map from client area dimensions to outer window dimensions
                  //AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
					dwStyle = WS_DLGFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;//WS_OVERLAPPEDWINDOW;//WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;//
					//int shellH = 0;
					//RECT   Rect; 
					//HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
					//if(GetWindowRect(hWnd,&Rect))   
					//{ 
					//	shellH = Rect.bottom-Rect.top;//   就是任务栏的高度 
					//}
					//rect.top -= shellH+GetSystemMetrics(SM_CYSIZE);
					int h = GetSystemMetrics(SM_CYCAPTION);//SM_CYSIZE
					int xFrame = GetSystemMetrics(SM_CXFRAME);
					int yFrame = GetSystemMetrics(SM_CYFRAME);
#if(_MSC_VER <= 1600)
					xFrame *= 0.5f;
					yFrame *= 0.5f;
#endif
#if(_WIN32_WINNT >= 0x0600)
					yFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
					xFrame += GetSystemMetrics(SM_CXPADDEDBORDER);
#endif
					rect.top -= h + yFrame;
					rect.bottom += yFrame;
					rect.left -= xFrame;
					rect.right += xFrame;
#if(_MSC_VER <= 1600)
					rect.top += 1.0f;
					rect.bottom -= 1.0f;
					rect.left += 1.0f;
					rect.right -= 1.0f;
#endif
					char gbuf[256];
					sprintf(gbuf,"rect.top=%d rect.bottom=%d rect.left=%d rect.right=%d\n\0",rect.top,rect.bottom,rect.left,rect.right);
					gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
					//CRCore::notify(CRCore::FATAL)<<"rect.top="<<rect.top<<"rect.bottom="<<rect.bottom<<"rect.left="<<rect.left<<"rect.right="<<rect.right<<std::endl;
                }
                else {
                  dwStyle=WS_POPUP;
                }

                x = rect.left;
                y = rect.top;
                w = rect.right - rect.left;
                h = rect.bottom - rect.top;
            }

            if (!(_win = CreateWindowEx(dwExStyle,
                                        _windowName.c_str(),
                                        TEXT( _windowName.c_str() ),
                                        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                        x,
                                        y,
                                        w,
                                        h,
                                        _parent,
                                        NULL,
                                        _hinstance,
                                        NULL)))
            {
                doInternalError("Window Creation Error.");
                KillGLWindow();
                return FALSE;
            }
			//verifyPosition();
        }
        else/* if( (_windowWidth == UnknownDimension) && (_windowHeight == UnknownDimension) )*/
        {
            //POINT p;

            //GetClientRect(_win, &rect);
            //_windowWidth = rect.right - rect.left;
            //_windowHeight = rect.bottom - rect.top;

            //// map window origin to screen
            //p.x = rect.left;
            //p.y = rect.top;
            //ClientToScreen(_win, &p);

            //_windowX = p.x;
            //_windowY = _screenHeight - _windowHeight - p.y;
            //_isFullScreen = false;
            _oldWndProc = (WNDPROC)SetWindowLong(_win, GWL_WNDPROC, (long)((WNDPROC)s_proc));
        }

        if (!(_hdc=GetDC(_win)))
        {
            doInternalError("Can't Create A GL Device Context.");
            KillGLWindow();
            return FALSE;
        }
    }
    _setCursor(_defaultCursor);

    ref_ptr<TemporaryWindow> tempwnd;

    if (_drawableType == DrawableType_PBuffer)
    {
        tempwnd = new TemporaryWindow();
        _hdc = tempwnd->getDC();
        if (!_hdc)
        { 
            doInternalError("Can't create the temporary window.");
            return false;
        }
        if (!tempwnd->makeCurrent())
        {
            doInternalError("Can't make the temporary window's context current");
            return false;
        }
    }

	//if(!_initgl())
	//	return false;
	unsigned int visualId;

	ref_ptr<VisualChooser> lvc;
	if (_visualChooser.valid())
	{
		lvc = _visualChooser;
	}
	else
	{
		lvc = new VisualChooser;
		lvc->setSimpleConfiguration(_drawableType != DrawableType_PBuffer);
	}

	if (_drawableType == DrawableType_PBuffer)
	{
		lvc->addExtendedAttribute(WGL_DRAW_TO_PBUFFER_ARB, true);
		switch (_rtt_mode)
		{
		case RenderToRGBTexture: 
			lvc->addExtendedAttribute(WGL_BIND_TO_TEXTURE_RGB_ARB, true);
			break;
		case RenderToRGBATexture: 
			lvc->addExtendedAttribute(WGL_BIND_TO_TEXTURE_RGBA_ARB, true);
			break;
		default: ;
		}
	}

	if (_drawableType == DrawableType_Window)
	{
		lvc->addExtendedAttribute(WGL_DRAW_TO_WINDOW_ARB, true);
	}

	unsigned int numMultiSamples = lvc->getNumMultiSamples();
	if(numMultiSamples > 1)
	{
		lvc->addExtendedAttribute(WGL_SAMPLE_BUFFERS_ARB, GL_TRUE);
		lvc->addExtendedAttribute(WGL_SAMPLES_ARB, numMultiSamples);
	}

	VisualInfo *visualInfo = lvc->choose(&_hdc, _screen);
	visualId = lvc->getVisualID();

	//if(multiSampleFormat>0) 
	//{
	//	visualId = multiSampleFormat;
	//	multiSampleFormat = -2;
	//}

	if (_drawableType == DrawableType_Window)
	{
		PIXELFORMATDESCRIPTOR pfd;
		if (!SetPixelFormat(_hdc, visualId, &pfd))
		{
			doInternalError("Can't Set The PixelFormat.");
			KillGLWindow();
			return FALSE;
		}
	}
	else
	{
		WGLExtensions *ext = WGLExtensions::instance();
		if (!ext)
		{
			std::cerr << "Producer::RenderSurface: WGL extensions not available" << std::endl;
			return false;
		}

		if (!ext->isSupported(WGLExtensions::ARB_pbuffer))
		{
			std::cerr << "Producer::RenderSurface: PBuffers are not supported" << std::endl;
			return false;
		}

		std::vector<int> pbattr(_user_pbattr);

		if (_rtt_mode != RenderToTextureMode_None)
		{
			if (!ext->isSupported(WGLExtensions::ARB_render_texture))
			{
				std::cerr << "Producer::RenderSurface: Render-To-Texture is not supported" << std::endl;
			}
			else
			{
				pbattr.push_back(WGL_TEXTURE_FORMAT_ARB);

				switch (_rtt_mode)
				{
				case RenderToRGBTexture:
					pbattr.push_back(WGL_TEXTURE_RGB_ARB);
					break;
				case RenderToRGBATexture:
					pbattr.push_back(WGL_TEXTURE_RGBA_ARB);
					break;
				default: 
					pbattr.push_back(static_cast<int>(_rtt_mode));
				}

				pbattr.push_back(WGL_TEXTURE_TARGET_ARB);

				switch (_rtt_target)
				{
				case TextureCUBE:
					pbattr.push_back(WGL_TEXTURE_CUBE_MAP_ARB);
					break;
				case Texture1D:
					pbattr.push_back(WGL_TEXTURE_1D_ARB);
					break;
				case Texture2D:
					pbattr.push_back(WGL_TEXTURE_2D_ARB);
					break;
				default: 
					pbattr.push_back(static_cast<int>(_rtt_target));
				}

				if (_rtt_options & RequestSpaceForMipMaps)
				{
					pbattr.push_back(WGL_MIPMAP_TEXTURE_ARB);
					pbattr.push_back(1);
				}

				if (_rtt_options & RequestLargestPBuffer)
				{
					pbattr.push_back(WGL_PBUFFER_LARGEST_ARB);
					pbattr.push_back(1);
				}
			}
		}

		// Terminate array
		pbattr.push_back(0);

		_win = reinterpret_cast<Window>(ext->wglCreatePBuffer(_hdc, visualId, _windowWidth, _windowHeight, &pbattr.front()));
		if (!_win)
		{
			doInternalError("Can't create the PBuffer");
			return false;
		}

		_hdc = ext->wglGetPBufferDC(reinterpret_cast<HPBUFFERARB>(_win));
		if (!_hdc)
		{
			doInternalError("Can't get a device context for the PBuffer");
			return false;
		}

		int actual_width = _windowWidth;
		int actual_height = _windowHeight;
		ext->wglQueryPBuffer(reinterpret_cast<HPBUFFERARB>(_win), WGL_PBUFFER_WIDTH_ARB, &actual_width);
		ext->wglQueryPBuffer(reinterpret_cast<HPBUFFERARB>(_win), WGL_PBUFFER_HEIGHT_ARB, &actual_height);
		_windowWidth = actual_width;
		_windowHeight = actual_height;
	}

	if (!(_glcontext = wglCreateContext( _hdc )))
	{
		doInternalError("Can't Create A GL Rendering Context.");
		KillGLWindow();
		return FALSE;
	}

	// Initialize globallySharedGLContext if global sharing is requested and 
	// the global variable is still NULL.
	// Caveat is if the user attempted to share GL contexts already through realize()
	if( _shareAllGLContexts == true && 
		_globallySharedGLContext == 0L &&
		_sharedGLContext == 0L )
	{
		_globallySharedGLContext = _glcontext;
	}

	// ?? WHY DOES THIS CRASH HERE ??  nhv
	//    if (!makeCurrent())
	//    {
	//        KillGLWindow();
	//        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	//        return FALSE;
	//    }

	//if(numMultiSamples>0)///add by wch
	//{
	//	if (!makeCurrent())
	//	{
	//		KillGLWindow();
	//		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	//		return FALSE;
	//	}

	//	if(multiSampleFormat==0)
	//	{
	//		multiSampleFormat = initMultisample(_hinstance,_win,visualInfo,numMultiSamples);
	//	}

	//	if(multiSampleFormat>0)
	//	{
	//		KillGLWindow();
	//		ChangeDisplaySettings(&dm,CDS_FULLSCREEN);
	//	    return _init();
	//	}

	//	releaseCurrent();
	//}

    if (_drawableType == DrawableType_Window)
    {
        _setCursor(_currentCursor);
        //ShowWindow(_win, SW_SHOW );
		//if(_ownWindow)
		//{
		//	UpdateWindow(_win );
		//}
		//SetForegroundWindow(_win);
		//SetFocus(_win);
		////ImmDisableIME(GetCurrentThreadId());
		////ImmAssociateContext(_win,NULL);

        registry.insert(std::pair<Window,RenderSurface *>(_win, this));
    }

	_inited = true;
    return true;
}
//bool RenderSurface::_initgl()
//{
//	if(_glcontext)
//		return true;
//	unsigned int visualId;
//
//	ref_ptr<VisualChooser> lvc;
//	if (_visualChooser.valid())
//	{
//		lvc = _visualChooser;
//	}
//	else
//	{
//		lvc = new VisualChooser;
//		lvc->setSimpleConfiguration(_drawableType != DrawableType_PBuffer);
//	}
//
//	if (_drawableType == DrawableType_PBuffer)
//	{
//		lvc->addExtendedAttribute(WGL_DRAW_TO_PBUFFER_ARB, true);
//		switch (_rtt_mode)
//		{
//		case RenderToRGBTexture: 
//			lvc->addExtendedAttribute(WGL_BIND_TO_TEXTURE_RGB_ARB, true);
//			break;
//		case RenderToRGBATexture: 
//			lvc->addExtendedAttribute(WGL_BIND_TO_TEXTURE_RGBA_ARB, true);
//			break;
//		default: ;
//		}
//	}
//
//	if (_drawableType == DrawableType_Window)
//	{
//		lvc->addExtendedAttribute(WGL_DRAW_TO_WINDOW_ARB, true);
//	}
//
//	unsigned int numMultiSamples = lvc->getNumMultiSamples();
//	if(numMultiSamples > 1)
//	{
//		lvc->addExtendedAttribute(WGL_SAMPLE_BUFFERS_ARB, GL_TRUE);
//		lvc->addExtendedAttribute(WGL_SAMPLES_ARB, numMultiSamples);
//	}
//
//	VisualInfo *visualInfo = lvc->choose(&_hdc, _screen);
//	visualId = lvc->getVisualID();
//
//	//if(multiSampleFormat>0) 
//	//{
//	//	visualId = multiSampleFormat;
//	//	multiSampleFormat = -2;
//	//}
//
//	if (_drawableType == DrawableType_Window)
//	{
//		PIXELFORMATDESCRIPTOR pfd;
//		if (!SetPixelFormat(_hdc, visualId, &pfd))
//		{
//			doInternalError("Can't Set The PixelFormat.");
//			KillGLWindow();
//			return FALSE;
//		}
//	}
//	else
//	{
//		WGLExtensions *ext = WGLExtensions::instance();
//		if (!ext)
//		{
//			std::cerr << "Producer::RenderSurface: WGL extensions not available" << std::endl;
//			return false;
//		}
//
//		if (!ext->isSupported(WGLExtensions::ARB_pbuffer))
//		{
//			std::cerr << "Producer::RenderSurface: PBuffers are not supported" << std::endl;
//			return false;
//		}
//
//		std::vector<int> pbattr(_user_pbattr);
//
//		if (_rtt_mode != RenderToTextureMode_None)
//		{
//			if (!ext->isSupported(WGLExtensions::ARB_render_texture))
//			{
//				std::cerr << "Producer::RenderSurface: Render-To-Texture is not supported" << std::endl;
//			}
//			else
//			{
//				pbattr.push_back(WGL_TEXTURE_FORMAT_ARB);
//
//				switch (_rtt_mode)
//				{
//				case RenderToRGBTexture:
//					pbattr.push_back(WGL_TEXTURE_RGB_ARB);
//					break;
//				case RenderToRGBATexture:
//					pbattr.push_back(WGL_TEXTURE_RGBA_ARB);
//					break;
//				default: 
//					pbattr.push_back(static_cast<int>(_rtt_mode));
//				}
//
//				pbattr.push_back(WGL_TEXTURE_TARGET_ARB);
//
//				switch (_rtt_target)
//				{
//				case TextureCUBE:
//					pbattr.push_back(WGL_TEXTURE_CUBE_MAP_ARB);
//					break;
//				case Texture1D:
//					pbattr.push_back(WGL_TEXTURE_1D_ARB);
//					break;
//				case Texture2D:
//					pbattr.push_back(WGL_TEXTURE_2D_ARB);
//					break;
//				default: 
//					pbattr.push_back(static_cast<int>(_rtt_target));
//				}
//
//				if (_rtt_options & RequestSpaceForMipMaps)
//				{
//					pbattr.push_back(WGL_MIPMAP_TEXTURE_ARB);
//					pbattr.push_back(1);
//				}
//
//				if (_rtt_options & RequestLargestPBuffer)
//				{
//					pbattr.push_back(WGL_PBUFFER_LARGEST_ARB);
//					pbattr.push_back(1);
//				}
//			}
//		}
//
//		// Terminate array
//		pbattr.push_back(0);
//
//		_win = reinterpret_cast<Window>(ext->wglCreatePBuffer(_hdc, visualId, _windowWidth, _windowHeight, &pbattr.front()));
//		if (!_win)
//		{
//			doInternalError("Can't create the PBuffer");
//			return false;
//		}
//
//		_hdc = ext->wglGetPBufferDC(reinterpret_cast<HPBUFFERARB>(_win));
//		if (!_hdc)
//		{
//			doInternalError("Can't get a device context for the PBuffer");
//			return false;
//		}
//
//		int actual_width = _windowWidth;
//		int actual_height = _windowHeight;
//		ext->wglQueryPBuffer(reinterpret_cast<HPBUFFERARB>(_win), WGL_PBUFFER_WIDTH_ARB, &actual_width);
//		ext->wglQueryPBuffer(reinterpret_cast<HPBUFFERARB>(_win), WGL_PBUFFER_HEIGHT_ARB, &actual_height);
//		_windowWidth = actual_width;
//		_windowHeight = actual_height;
//	}
//
//	if (!(_glcontext = wglCreateContext( _hdc )))
//	{
//		doInternalError("Can't Create A GL Rendering Context.");
//		KillGLWindow();
//		return FALSE;
//	}
//
//	// Initialize globallySharedGLContext if global sharing is requested and 
//	// the global variable is still NULL.
//	// Caveat is if the user attempted to share GL contexts already through realize()
//	if( _shareAllGLContexts == true && 
//		_globallySharedGLContext == 0L &&
//		_sharedGLContext == 0L )
//	{
//		_globallySharedGLContext = _glcontext;
//	}
//
//	// ?? WHY DOES THIS CRASH HERE ??  nhv
//	//    if (!makeCurrent())
//	//    {
//	//        KillGLWindow();
//	//        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
//	//        return FALSE;
//	//    }
//
//	//if(numMultiSamples>0)///add by wch
//	//{
//	//	if (!makeCurrent())
//	//	{
//	//		KillGLWindow();
//	//		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
//	//		return FALSE;
//	//	}
//
//	//	if(multiSampleFormat==0)
//	//	{
//	//		multiSampleFormat = initMultisample(_hinstance,_win,visualInfo,numMultiSamples);
//	//	}
//
//	//	if(multiSampleFormat>0)
//	//	{
//	//		KillGLWindow();
//	//		ChangeDisplaySettings(&dm,CDS_FULLSCREEN);
//	//	    return _init();
//	//	}
//
//	//	releaseCurrent();
//	//}
//	return true;
//}
// WGLisExtensionSupported: This Is A Form Of The Extension For WGL
//bool RenderSurface::isExtensionSupported(const char *extension)///add by wch
//{
//	const size_t extlen = strlen(extension);
//	const char *supported = NULL;
//
//	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
//	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
//
//	if (wglGetExtString)
//		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());
//
//	// If That Failed, Try Standard Opengl Extensions String
//	if (supported == NULL)
//		supported = (char*)glGetString(GL_EXTENSIONS);
//
//	// If That Failed Too, Must Be No Extensions Supported
//	if (supported == NULL)
//		return false;
//
//	// Begin Examination At Start Of String, Increment By 1 On False Match
//	for (const char* p = supported; ; p++)
//	{
//		// Advance p Up To The Next Possible Match
//		p = strstr(p, extension);
//
//		if (p == NULL)
//			return false;															// No Match
//
//		// Make Sure That Match Is At The Start Of The String Or That
//		// The Previous Char Is A Space, Or Else We Could Accidentally
//		// Match "wglFunkywglExtension" With "wglExtension"
//
//		// Also, Make Sure That The Following Character Is Space Or NULL
//		// Or Else "wglExtensionTwo" Might Match "wglExtension"
//		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
//			return true;															// Match
//	}
//}
//
//// InitMultisample: Used To Query The Multisample Frequencies
//int RenderSurface::initMultisample(HINSTANCE hInstance,Window hWnd,VisualInfo *visualInfo,unsigned int numMultiSamples)///add by wch
//{  
//	// See If The String Exists In WGL!
//	if (!isExtensionSupported("WGL_ARB_multisample"))
//	{
//		return -1;
//	}
//
//	// Get Our Pixel Format
//	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
//	if (!wglChoosePixelFormatARB) 
//	{
//		return -1;
//	}
//
//	// Get Our Current Device Context
//	HDC hDC = GetDC(hWnd);
//
//	int		pixelFormat;
//	int		valid;
//	unsigned int  numFormats;
//	float	fAttributes[] = {0,0};
//
//	// These Attributes Are The Bits We Want To Test For In Our Sample
//	// Everything Is Pretty Standard, The Only One We Want To 
//	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
//	// These Two Are Going To Do The Main Testing For Whether Or Not
//	// We Support Multisampling On This Hardware.
//	int iAttributes[] =
//	{
//		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
//		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
//		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
//		WGL_COLOR_BITS_ARB,visualInfo->cColorBits,
//		WGL_ALPHA_BITS_ARB,visualInfo->cAlphaBits,
//		WGL_DEPTH_BITS_ARB,visualInfo->cDepthBits,
//		WGL_STENCIL_BITS_ARB,visualInfo->cStencilBits,
//		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
//		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
//		WGL_SAMPLES_ARB,numMultiSamples,
//		0,0
//	};
//
//	// First We Check To See If We Can Get A Pixel Format For 4 Samples
//	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
//
//	// If We Returned True, And Our Format Count Is Greater Than 1
//	if (valid && numFormats >= 1)
//	{
//		MessageBox(NULL,"wglChoosePixelFormatARB sucess ","ERROR",MB_OK|MB_ICONEXCLAMATION);
//
//		return numFormats;
//	}
//
//	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
//	iAttributes[19] = 2;
//	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
//	if (valid && numFormats >= 1)
//	{
//		return numFormats;
//	}
//MessageBox(NULL,"wglChoosePixelFormatARB error ","ERROR",MB_OK|MB_ICONEXCLAMATION);
//	// Return The Valid Format
//	return  -1;
//}
//

void RenderSurface::fullScreen( bool flag )
{

    if( flag == _isFullScreen )
        return;

    if( _drawableType != DrawableType_Window )
        return;

    if (_parent)
        return;

    _isFullScreen = flag;

    if (_isFullScreen)
    {
        /* force fullscreen mode */
        DWORD s = GetWindowLong(_win, GWL_STYLE);
        s &= ~WS_OVERLAPPEDWINDOW;
        s |= WS_POPUP;
        SetWindowLong(_win, GWL_STYLE, s);
        //if( _useCustomFullScreen )
        //    SetWindowPos(_win, 
        //                 HWND_TOP,
        //                 _customFullScreenOriginX,
        //                 _screenHeight - (_customFullScreenOriginY + _customFullScreenHeight),
        //                 _customFullScreenWidth,
        //                 _customFullScreenHeight,
        //                 SWP_FRAMECHANGED);
        //else
            SetWindowPos(_win, 
                         HWND_TOP,
                         0, 0, 
                         _screenWidth, _screenHeight, 
                         SWP_FRAMECHANGED);

    } 
    else 
    {
        UINT flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING |
                     SWP_NOSIZE | SWP_NOZORDER;

        DWORD style = GetWindowLong(_win, GWL_STYLE);

        /* Get rid of fullscreen mode, if it exists */
        if ( style & WS_POPUP ) {
            if(_decorations) { // Only add border if we want decorations
                style &= ~WS_POPUP;
                style |= WS_OVERLAPPEDWINDOW;
            }
            SetWindowLong(_win, GWL_STYLE, style);
            flags |= SWP_FRAMECHANGED;
        }

        flags &= ~SWP_NOMOVE;
        flags &= ~SWP_NOSIZE;

        RECT rect;

        // map Y up convention to Y down for windows
        rect.left = _windowX;
        rect.top = _screenHeight - _windowY - _windowHeight;
        rect.bottom = _screenHeight - _windowY;
        rect.right = _windowWidth + _windowX;

        if(_decorations) {
            // adjust from client dimensions to window outer dimentions
            AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        }

        //int x = rect.left;
        //int y = rect.top;
        //int w = rect.right - rect.left;
        //int h = rect.bottom - rect.top;
		///////////////////////////////////////
		int x, y, h, w;
		// map from Y up to Y down
		rect.left = _windowX;
		rect.top = _screenHeight - _windowY - _windowHeight;
		rect.bottom = _screenHeight - _windowY;
		rect.right = _windowWidth + _windowX;

		style = GetWindowLong(_win,GWL_STYLE);
		LONG styleEx = GetWindowLong(_win,GWL_EXSTYLE);
		AdjustWindowRectEx(&rect, style, FALSE, styleEx);

		int shellH = 0;
		RECT   Rect; 
		HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
		if(GetWindowRect(hWnd,&Rect))   
		{ 
			shellH = Rect.bottom-Rect.top;//   就是任务栏的高度
			if(shellH >= _screenHeight)
				shellH = 0;
		}
		int cy = GetSystemMetrics(SM_CYCAPTION);//SM_CYSIZE

		rect.top -= shellH-cy;
		rect.bottom -= shellH-cy;

		x = rect.left;
		y = rect.top;
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;

		SetWindowPos(_win,
			HWND_TOP,
			x, y, w, h,
			flags);
    }
}

bool RenderSurface::makeCurrent( void ) const
{
    if (_rtt_mode != RenderToTextureMode_None && _drawableType == DrawableType_PBuffer)    
    {        
        WGLExtensions *tempext = WGLExtensions::instance();
        if (tempext && tempext->isSupported(WGLExtensions::ARB_render_texture))
        {
            tempext->wglReleaseTexImage(reinterpret_cast<HPBUFFERARB>(_win), WGL_FRONT_LEFT_ARB);
            tempext->wglReleaseTexImage(reinterpret_cast<HPBUFFERARB>(_win), WGL_BACK_LEFT_ARB);
        }
    }

    if(_readDrawableRenderSurface && _readDrawableRenderSurface->isRealized())
    {
        WGLExtensions *tempext = WGLExtensions::instance();
        if (tempext && tempext->isSupported(WGLExtensions::ARB_make_current_read))
        {
            if (!tempext->wglMakeContextCurrent(_hdc, _readDrawableRenderSurface->_hdc, _glcontext))
                return false;
        }
        else
            return false;
    }
    else
    {
        if (!wglMakeCurrent(_hdc, _glcontext))
            return false;
    }

    if (_rtt_dirty_mipmap && (_rtt_options & RequestSpaceForMipMaps) != 0)
    {
        if (_ext->isSupported(WGLExtensions::ARB_render_texture))
        {
            std::vector<int> pbattr;
            pbattr.push_back(WGL_MIPMAP_LEVEL_ARB);
            pbattr.push_back(_rtt_mipmap);
            _ext->wglSetPBufferAttrib(reinterpret_cast<HPBUFFERARB>(_win), &pbattr.front());
        }
    }

    if (_rtt_dirty_face && _rtt_target == TextureCUBE)
    {
        if (_ext->isSupported(WGLExtensions::ARB_render_texture))
        {
            std::vector<int> pbattr;
            pbattr.push_back(WGL_CUBE_MAP_FACE_ARB);
            pbattr.push_back(0x207D + _rtt_face);
            _ext->wglSetPBufferAttrib(reinterpret_cast<HPBUFFERARB>(_win), &pbattr.front());
        }
    }

    return true;
}

bool RenderSurface::releaseCurrent( void )
{
	//KillGLWindow();
	//WGLExtensions::clear();//modify by wch
	//_fini();
	//wglMakeCurrent( NULL, NULL );
	//if (_glcontext && !wglDeleteContext(_glcontext))
	//{
	//	//doInternalError("Release Rendering Context Failed.");
	//}
	//_glcontext = NULL;

	_realized = false;
	//return true;
	return(wglMakeCurrent( NULL, NULL ));
}

void RenderSurface::swapBuffers( void )
{
    //makeCurrent();
    //_ext->wglSwapBuffers( _hdc );
	SwapBuffers(_hdc);
}

EventQueue * RenderSurface::selectInput( unsigned int mask )
{
    Client *client = new Client(mask);
    clients.push_back( client );
    // Need to inform clients that the windows has already
    // been created and what its size is
    if( _realized )
    {        client->queue( new WindowConfigEvent( _win,
            _windowX,
            _windowY,
            _windowWidth,
            _windowHeight ));
    }
    return client->getQueue();
}

void RenderSurface::sync( int divisor )
{
    divisor = divisor;
    glFinish();
}


void RenderSurface::run( void )
{
    _init();
	makeCurrent();
	WGLExtensions::instance()->setVSync(_visualChooser->getVsync());
	WGLExtensions::clear();//modify by wch

    if( _threadReady != 0L)
        _threadReady->block();
	//::ShowWindowAsync(_win, SW_SHOW );
    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0) > 0 )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
        testCancel();
    }
    yieldCurrentThread();

	KillGLWindow();
}

//void RenderSurface::doMessaging()
//{//add by wch
//	MSG msg;
//
//	while(PeekMessage(&msg,_win,0,0,PM_NOREMOVE))
//	{
//		if(!GetMessage(&msg,_win,0,0))
//			return;
//
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//}

void RenderSurface::_resizeWindow()
{
    RECT rect;
    int x, y, h, w;

    if (_parent)
    {
        // get the client area of the parent
        GetClientRect(_parent, &rect);

        // clip to width of parent
        if ((_windowWidth + _windowX) < (unsigned int)rect.right)
        {
            w = _windowWidth;
            x = _windowX;
        }
        else
        {
            w = rect.right - _windowX;
            x = _windowX;
        }

        // clip to height of parent and flip from
        // Y up convention to Y down for window
        if ((_windowY + _windowHeight) < (unsigned int)rect.bottom)
        {
            h = _windowHeight;
            y = rect.bottom - _windowY - _windowHeight;
        }
        else
        {
            h = rect.bottom - _windowY;
            y = 0;
        }
    }
    else if (_isFullScreen)
    {
        x = 0;
        y = 0;
        h = _screenHeight;
        w = _screenWidth;
    }
    else // normal window
    {
        RECT rect;

        // map from Y up to Y down
        rect.left = _windowX;
        rect.top = _screenHeight - _windowY - _windowHeight;
        rect.bottom = _screenHeight - _windowY;
        rect.right = _windowWidth + _windowX;

        //// adjust from client dimension to outside dimension
        ////AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        //// Above line replaced with this contribution by Trajce Nikolov (Nick):
        //// Leaving the old line in commented out in case...
        LONG style = GetWindowLong(_win,GWL_STYLE);
        LONG styleEx = GetWindowLong(_win,GWL_EXSTYLE);
        AdjustWindowRectEx(&rect, style, FALSE, styleEx);
		
		//char gbuf[256];
		//sprintf(gbuf,"AdjustWindowRectEx style=%d styleEx=%d\n\0",style,styleEx);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//sprintf(gbuf,"ResizeWindow _windowX=%d _windowY=%d _windowWidth=%d _windowHeight=%d _screenWidth=%d _screenHeight=%d\n\0",_windowX,_windowY,_windowWidth,_windowHeight,_screenWidth,_screenHeight);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		//sprintf(gbuf,"AdjustWindowRectEx rect.top=%d rect.bottom=%d rect.left=%d rect.right=%d\n\0",rect.top,rect.bottom,rect.left,rect.right);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		int shellH = 0;
		RECT   Rect; 
		HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
		if(GetWindowRect(hWnd,&Rect))   
		{ 
			shellH = Rect.bottom-Rect.top;//   就是任务栏的高度
			if(shellH >= _screenHeight)
				shellH = 0;
		}
		int cy = GetSystemMetrics(SM_CYCAPTION);//SM_CYSIZE

		//sprintf(gbuf,"shellH=%d cy=%d\n\0",shellH,cy);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

		rect.top -= shellH-cy;
		rect.bottom -= shellH-cy;

        x = rect.left;
        y = rect.top;
        w = rect.right - rect.left;
        h = rect.bottom - rect.top;
    }
	//char gbuf[256];
	//sprintf(gbuf,"SetWindowPos x=%d y=%d w=%d h=%d\n\0",x,y,w,h);
	//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	SetWindowPos(_win,HWND_TOP, x, y, w,
		h, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
}

void RenderSurface::_setBorder( bool flag )
{
    //Flags that will be used for SetWindowPos
	UINT flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING |
				 SWP_NOSIZE | SWP_NOZORDER;

    //Get current style of window
    DWORD style = GetWindowLong(_win, GWL_STYLE);

    //Get current client dimensions
    RECT rect;
    GetClientRect( _win, &rect );

    //Variables that will hold new size of window
    int cx, cy;

    if(flag)
    {
        //Add border style to window
        //style &= ~WS_POPUP;
        //style |= WS_DLGFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
		style = WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;//WS_OVERLAPPEDWINDOW;

        //Calculate new size of window with border
        rect.left = 0;
        rect.top = 0;
        rect.right = rect.right - rect.left;
        rect.bottom = rect.bottom - rect.top;
        
		//char gbuf[256];
		//sprintf(gbuf,"SetBorder1 rect.top=%d rect.bottom=%d rect.left=%d rect.right=%d\n\0",rect.top,rect.bottom,rect.left,rect.right);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

        LONG styleEx = GetWindowLong(_win,GWL_EXSTYLE);
        AdjustWindowRectEx(&rect, style, FALSE, styleEx);

		//sprintf(gbuf,"SetBorder2 rect.top=%d rect.bottom=%d rect.left=%d rect.right=%d\n\0",rect.top,rect.bottom,rect.left,rect.right);
		//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);

        cx = rect.right - rect.left;
        cy = rect.bottom - rect.top;
        //flags &= ~(SWP_NOSIZE);
    }
    else
    {
        //Remove border style from window
        style &= ~WS_OVERLAPPEDWINDOW;
        style |= WS_POPUP;

        //Calculate new size of window without border
        cx = rect.right - rect.left;
        cy = rect.bottom - rect.top;
        //flags &= ~(SWP_NOSIZE);
    }

    //Apply new window style
    SetWindowLong(_win, GWL_STYLE, style);
    
    //Need to call this to change window size and update frame style
    SetWindowPos(_win, 
                    HWND_TOP,
                    0, 0, 
                    cx, cy, 
                    flags);
}

void RenderSurface::_setWindowName( const std::string & name )
{
	if(_win)
        SetWindowText(_win, name.c_str());
}

void RenderSurface::_useCursor(bool flag)
{
    _useCursorFlag = flag;
    if( _realized )
        _setCursor(_currentCursor);
}

void RenderSurface::_setCursor(Cursor cursor)
{
    if( _useCursorFlag == false )
    {
        ::SetCursor(_nullCursor);
    }
    else
    {
        _currentCursor = cursor ? cursor : _nullCursor;

        if ( _currentCursor == NULL )
        {
            _currentCursor = ::LoadCursor( NULL, IDC_ARROW );
        }

        ::SetCursor(_currentCursor);
    }
}

void RenderSurface::_setCursorToDefault()
{
    _setCursor(_defaultCursor);
}

void RenderSurface::_positionPointer(int x, int y)
{
    //****TODO***
    // This needs to be mapped from an x, y that
    // are interpreted in the Y up Producer world
    // to Y down in the window world.  Should x, y
    // be screen or window coordinates?

    POINT coords = { x, y };

    // First find the new screen-relative coordinates of the mouse
    ClientToScreen( _win, &coords );

    // Now set the new mouse cursor position...
    SetCursorPos( coords.x, coords.y );
}

void RenderSurface::_computeScreenSize( unsigned int &width, unsigned int &height ) const
{
    DEVMODE dm;
    memset(&dm,0,sizeof(dm));
    dm.dmSize = sizeof(dm);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    width  = dm.dmPelsWidth;
    height = dm.dmPelsHeight;
}

void RenderSurface::resizeToParent()
{
    RECT rect;

    if (_parent)
    {
        GetClientRect(_parent, &rect);

        _windowWidth = (rect.right - rect.left);
        _windowX = 0;

        _windowHeight = (rect.bottom - rect.top);
        _windowY = 0;

        _resizeWindow();
    }
}

void RenderSurface::mapWindow()
{
    fprintf( stderr, "RenderSurface::mapWindow() - Unimplemented under win32\n" );
}

void RenderSurface::unmapWindow()
{
    fprintf( stderr, "RenderSurface::unMapWindow() - Unimplemented under win32\n" );
}

void RenderSurface::_fini()
{
    if( _win/*_realized*/ && _drawableType == DrawableType_Window )
    {
        if (!_ownWindow)
            SetWindowLong(_win, GWL_WNDPROC, (long)_oldWndProc);
        PostThreadMessage(getThreadId(), WM_QUIT, 0, 0);
    }
	_ext = 0;
	_inited = false;
}

void RenderSurface::bindPBufferToTexture(BufferType buffer) const
{
    WGLExtensions *ext = WGLExtensions::instance();
    if (!ext || !ext->isSupported(WGLExtensions::ARB_render_texture))
    {
        // RTT is not supported, fall back to good old copy-to-texture.
        glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, _windowWidth, _windowHeight);
        return;
    }

    switch (buffer)
    {
        case BackBuffer:
            ext->wglBindTexImage(reinterpret_cast<HPBUFFERARB>(_win), WGL_BACK_LEFT_ARB);
            break;
        case FrontBuffer:
            ext->wglBindTexImage(reinterpret_cast<HPBUFFERARB>(_win), WGL_FRONT_LEFT_ARB);
            break;
        default:
            ext->wglBindTexImage(reinterpret_cast<HPBUFFERARB>(_win), static_cast<GLenum>(buffer));
    }        
}

void RenderSurface::_useOverrideRedirect( bool )
{
    // Ignored on Windows.  Flag value remains false.
}

void RenderSurface::_initThreads()
{
    // Stub
}


#endif  // WIN32

