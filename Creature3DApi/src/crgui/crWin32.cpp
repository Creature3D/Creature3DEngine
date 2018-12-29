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
#include <CRGUI/crWin32.h>
#include <CRCore/crNotify.h>
#include <CRCore/crArgumentParser.h>
#include <CRGUI/crElement.h>
#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <winuser.h>
using namespace CRGUI;
HCURSOR crWindow::s_defaultCursor = LoadCursor(NULL,IDC_ARROW);
crWindow :: crWindow()
{
	// clear everything
	m_mx = m_my = m_mouseWheelDelta = 0;
	m_sized = false;
	m_mouseButtonMsg = 0;
	m_mouseEvent = false;
	m_hwnd = NULL;
	m_hParentWnd = NULL;
	m_hins = NULL;
	//m_redraw = false;
	ZeroMemory(m_keys,sizeof(bool)*256);
}

crWindow :: ~crWindow()
{
	this->close();
}

#define WINMX		0x0013
#define WINMY		0x0014
#define WINBUT		0x0015
#define WINWHEELZ	0x0016
#define WINKEYUP	0x0017
#define WINKEYDOWN	0x0018
#define	WINSIZED	0x0019
#define WINWIDTH	0x0020
#define WINHEIGHT	0x0021
#define WINX	    0x0023
#define WINY	    0x0024
//#define WITH_IME
#ifdef WITH_IME
bool bSupportsIME			= true;
int CurrentIMESize			= 0;
#endif
//static bool s_imestart = false;
LRESULT CALLBACK WndProc(HWND m_hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	crWindow *win=(crWindow *)GetWindowLong(m_hwnd,GWLP_USERDATA);
	if(!win) return DefWindowProc(m_hwnd,msg,wParam,lParam);
	// which message
	//int lRet;
	switch(msg)
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
	case WM_MOUSEMOVE:
		::SendMessage(m_hwnd,WM_ACTIVATE,WA_ACTIVE,NULL);
		break;
	//case WM_ERASEBKGND:
	//	return true;
	//case WM_INPUTLANGCHANGEREQUEST://切换输入法时导致窗口拾取输入焦点
	//	std::cerr << "WM_INPUTLANGCHANGEREQUEST wParam = "<<wParam<<" lParam = "<<lParam<< std::endl;
	//       lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
	//	std::cerr << "WM_INPUTLANGCHANGEREQUEST "<<lRet<< std::endl;
	//	return lRet;
	}
	//std::cerr <<msg<< std::endl;
	switch(msg)
	{
		case WM_CLOSE:
			//exit(0);
			PostQuitMessage(0);
			return 0;
		case WM_DESTROY:
			SetWindowLong(m_hwnd, GWLP_USERDATA,0);	// pretend we weren't here
			return 0;
		case WM_SIZE:
			win->dataAccess(WINSIZED,1);				// say it's been resized
			win->dataAccess(WINWIDTH,(short) LOWORD(lParam));	// get the new m_width
			win->dataAccess(WINHEIGHT,(short) HIWORD(lParam));	// get the new m_height
			return 0;
		case WM_MOVE:
			win->dataAccess(WINSIZED,1);
			win->dataAccess(WINX,(short) LOWORD(lParam));
			win->dataAccess(WINY,(short) HIWORD(lParam));
			return 0;
		case WM_KEYDOWN:
			// set the key as being down
			win->dataAccess(WINKEYDOWN,wParam);
			return 0;
		case WM_KEYUP:
			// set the key as being up
			win->dataAccess(WINKEYUP,wParam);
			return 0;
		case WM_CHAR:
			{
				char c = LOWORD(wParam);
				if(c>=0)
				    win->pushChar(c);
			}
			return 0;
		case WM_IME_CHAR:
			{
				//处理输入字符串
				wchar_t c = LOWORD(wParam);
				win->pushChar(c);
			}
			return 0;
		case WM_MOUSEMOVE:
			// send x and y
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_MOUSEMOVE);
			return 0;
		case WM_LBUTTONUP:
			// set left mouse button up
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_LBUTTONUP);
			return 0;
		case WM_RBUTTONUP:
			// set right mouse button up
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_RBUTTONUP);
			return 0;
		case WM_MBUTTONUP:
			// set right mouse button up
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_MBUTTONUP);
			return 0;
		case WM_LBUTTONDOWN:
			// set left mouse button down
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_LBUTTONDOWN);
			return 0;
		case WM_RBUTTONDOWN:
			// set right mouse button down
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_RBUTTONDOWN);
			return 0;
		case WM_MBUTTONDOWN:
			// set right mouse button up
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_MBUTTONDOWN);
			return 0;
		case WM_LBUTTONDBLCLK:
			// set left mouse button down
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_LBUTTONDBLCLK);
			return 0;
		case WM_RBUTTONDBLCLK:
			// set right mouse button down
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_RBUTTONDBLCLK);
			return 0;
		case WM_MBUTTONDBLCLK:
			// set right mouse button up
			win->dataAccess(WINMX,(short)LOWORD(lParam));
			win->dataAccess(WINMY,(short)HIWORD(lParam));
			win->dataAccess(WINBUT,WM_MBUTTONDBLCLK);
			return 0;
		//case WM_SETFOCUS:
  //          SetFocus(m_hwnd);
		//	break;
		//case WM_KILLFOCUS:
		//	// set left mouse button up
		//	win->dataAccess(WINMX,(short)LOWORD(lParam));
		//	win->dataAccess(WINMY,(short)HIWORD(lParam));
		//	win->dataAccess(WINBUT,WM_LBUTTONUP);
		//	break;
		case WM_MOUSEWHEEL:
			// ????
			win->dataAccess(WINMX,LOWORD(lParam));
			win->dataAccess(WINMY,HIWORD(lParam));
			win->dataAccess(WINWHEELZ,HIWORD(lParam));
			win->dataAccess(WINBUT,WM_MOUSEWHEEL);
			return 0;
		case WM_PAINT:
			PAINTSTRUCT    ps;
			BeginPaint(m_hwnd, &ps);
            win->needRedraw(true);
			EndPaint(m_hwnd, &ps);
			return 0;
#ifdef WITH_IME
		case WM_IME_COMPOSITION:
			{
				HIMC Imc = ImmGetContext( m_hwnd );
				if( !Imc )
				{
					//appErrorf( TEXT( "No IME context" ) );
				}

				if( lParam & GCS_RESULTSTR )
				{
					// Get the size of the result string.
					int dwSize = ImmGetCompositionString( Imc, GCS_RESULTSTR, NULL, 0 );

					wchar_t* lpstr = new wchar_t[dwSize + 1];
					ZeroMemory( lpstr, sizeof( wchar_t ) * ( dwSize + 1 ) );

					// Get the result strings that is generated by IME.
					dwSize = ImmGetCompositionString( Imc, GCS_RESULTSTR, lpstr, dwSize );
					dwSize /= sizeof( wchar_t );
					//int utf8Num = WideCharToMultiByte(CP_UTF8, NULL, lpstr, dwSize, NULL, 0, NULL, NULL);
					//char * str = new char[utf8Num + 1];
					//WideCharToMultiByte(CP_UTF8, NULL, lpstr, dwSize, str, utf8Num, NULL, NULL);
					//str[utf8Num] = '\0';

					for( int i = 0; i < CurrentIMESize; i++ )
					{
						win->pushChar(8);//BackSpace
						//win->pushChar(8);
					}

					//int j = 0;
					//while(lpstr[j])
					//{
					//	win->pushChar(lpstr[j]);
					//	j++;
					//}
					for( int i = 0; i < dwSize; i++ )
					{
						//char c = str[i];
						win->pushChar(lpstr[i]);
						//CRCore::notify(CRCore::ALWAYS)<<c;
					}
					delete [] lpstr;
					//delete [] str;
					ImmReleaseContext( m_hwnd, Imc );

					CurrentIMESize = 0;
				}
				else if( lParam & GCS_COMPSTR ) 
				{
					// Get the size of the result string.
					int dwSize = ImmGetCompositionString( Imc, GCS_COMPSTR, NULL, 0 );

					wchar_t* lpstr = new wchar_t[dwSize + 1];
					ZeroMemory( lpstr, sizeof( wchar_t ) * ( dwSize + 1 ) );

					// Get the result strings that is generated by IME.
					dwSize = ImmGetCompositionString( Imc, GCS_COMPSTR, lpstr, dwSize );
					
					//dwSize /= sizeof( wchar_t );
					int utf8Num = WideCharToMultiByte(CP_ACP, NULL, lpstr, dwSize, NULL, 0, NULL, NULL);
					char * str = new char[utf8Num + 1];
					WideCharToMultiByte(CP_ACP, NULL, lpstr, dwSize, str, utf8Num, NULL, NULL);
					str[utf8Num] = '\0';
                    CRCore::notify(CRCore::ALWAYS)<<str;
					for( int i = 0; i < CurrentIMESize; i++ )
					{
						win->pushChar(8);//BackSpace
						//win->pushChar(8);
					}

					int j = 0;
					while(str[j])
					{
                        win->pushChar((unsigned char)lpstr[j]);
						j++;
					}
					//if(j>0) j--;
					//for( int i = 0; i<dwSize; i++ )
					//{
					//	//char c = str[i];
					//	win->pushChar(lpstr[i]);
					//	CRCore::notify(CRCore::ALWAYS)<<lpstr[i];
					//}
				    ImmSetCompositionString(Imc, SCS_SETSTR, lpstr, dwSize, NULL, 0);
					delete [] lpstr;
					delete [] str;

					ImmReleaseContext( m_hwnd, Imc );

					CurrentIMESize = dwSize;
				}

			}
			return 0;
#endif
	};
	return DefWindowProc(m_hwnd,msg,wParam,lParam);
}

void crWindow :: dataAccess(int key,short value)
{
	// if called before anything else
	if(!this)
		return;

	// Convert messages to variables
	switch(key)
	{
		case WINMX:			m_mx=value;	break;
		case WINMY:			m_my=value;	break;
		case WINBUT:
			if(value == WM_MOUSEMOVE&&(m_mouseButtonMsg == WM_LBUTTONDOWN || m_mouseButtonMsg == WM_MOUSEDRAG)) 
				m_mouseButtonMsg = WM_MOUSEDRAG;
			else
				m_mouseButtonMsg = value;
			m_mouseEvent = true;
			needRedraw(); 
			break;
		case WINWHEELZ:		m_mouseWheelDelta = value; break;
		case WINKEYUP:		m_keys[value]=false;	break;
		case WINKEYDOWN:	m_keys[value]=true;	needRedraw(); break;
		case WINSIZED:		m_sized=true;		break;
		case WINX:		    m_rect[0]=value;		break;
		case WINY:		    m_rect[1]=value;		break;
		case WINWIDTH:		m_rect[2]=value;		break;
		case WINHEIGHT:		m_rect[3]=value;		break;
	}
}

void crWindow :: _doMessaging()
{
	MSG msg;	// message info
	////// are there any messages
	while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)>0)
	{
		// get the message off the queue
		if(GetMessage(&msg,NULL,0,0)<=0)
			break;

		// and send it off again
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
	//{
	//	TranslateMessage( &msg );
	//	DispatchMessage( &msg );
	//}
}
// register the window
BOOL crWindow :: _register(const char *name)
{
	WNDCLASS	wcls;	// Windows class structure

	wcls.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;//CS_PARENTDC/*CS_HREDRAW | CS_VREDRAW*/;
	wcls.lpfnWndProc=WndProc;
	wcls.cbClsExtra=0;
	wcls.cbWndExtra=0;
	wcls.hInstance=m_hins;
	wcls.hIcon=NULL;//LoadIcon(NULL,IDI_APPLICATION);
	wcls.hCursor=s_defaultCursor;/*LoadCursor(NULL,IDC_ARROW);*/
	wcls.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
	wcls.lpszMenuName=NULL;
	wcls.lpszClassName=name;
    m_windowName = name;
	return RegisterClass(&wcls);
}

bool crWindow :: setup(int x,int y,int wid,int hit, const char *name, int exstyle)
{
	// get the dimensions
	m_rect.set(x,y,wid,hit);

	// get the instance
	m_hins=GetModuleHandle(NULL);

	// register the window
	if(!this->_register(name))
		return false;

	if(m_hParentWnd)
	    m_hwnd=CreateWindowEx(exstyle/*|WS_EX_NOPARENTNOTIFY*/,name,name,WS_CHILD,x,y,wid,hit,m_hParentWnd,NULL,m_hins,NULL);
	else
		m_hwnd=CreateWindowEx(exstyle,name,name,/*WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX*/WS_POPUP,x,y,wid,hit,HWND_DESKTOP,NULL,m_hins,NULL);
	// did that work?
	if(!m_hwnd)
		return false;
	// store a pointer to the class in HWND
	SetWindowLong(m_hwnd, GWLP_USERDATA,(long)this);
	//insertWindowMap(m_hwnd,this);
	
	// show the window
	//ShowWindow(m_hwnd,SW_SHOW);
	//UpdateWindow(m_hwnd);
	//SetForegroundWindow(m_hwnd);
	return true;
}

void crWindow::setParentWnd(HWND win)
{
	m_hParentWnd = win;
	if(m_hwnd)
	{
		SetWindowLong(m_hwnd,GWL_STYLE,WS_CHILD );
        SetParent(m_hwnd,m_hParentWnd);
	}
}

bool crWindow :: setupTitle(int x,int y,int wid,int hit, const char *name)
{
	// get the dimensions
	m_rect.set(x,y,wid,hit);

	// get the instance
	m_hins=GetModuleHandle(NULL);

	// register the window
	if(!this->_register(name))
		return false;

	// now create the window
	m_hwnd=CreateWindow(name,name,WS_OVERLAPPEDWINDOW,
		x,y,wid,hit,NULL,NULL,m_hins,NULL);

	// did that work?
	if(!m_hwnd)
		return false;
	// store a pointer to the class in HWND
	SetWindowLong(m_hwnd, GWLP_USERDATA,(long)this);
	//insertWindowMap(m_hwnd,this);
	// show the window
	ShowWindow(m_hwnd,SW_SHOW);
	//UpdateWindow(m_hwnd);
	//SetForegroundWindow(m_hwnd);
	// success
	return true;
}

void crWindow::resetKey()
{
	//m_mx=m_my=0;
	m_mouseEvent = false;
	ZeroMemory(m_keys,sizeof(bool)*256);
}

void crWindow::showWindow(int show, bool async)
{
	if(show) 
	{
		if(async)
			ShowWindowAsync(m_hwnd,SW_SHOWNORMAL);
		else
		{
			ShowWindow(m_hwnd,SW_SHOWNORMAL);
			//UpdateWindow(m_hwnd);
			//SetForegroundWindow(m_hwnd);
			//SetFocus(m_hwnd);
		}
	}
	else
	{
		if(async)
		    ShowWindowAsync(m_hwnd,SW_HIDE);
		else
		    ShowWindow(m_hwnd,SW_HIDE);
	}
}

void crWindow :: close()
{
	// anything to do?
	if(!m_hwnd)
		return;

	//removeWindowMap(m_hwnd);
	// pretend we weren't here
	SetWindowLong(m_hwnd, GWLP_USERDATA,0);
	DestroyWindow(m_hwnd);
	UnregisterClass(m_windowName.c_str(), m_hins);
	m_hwnd = NULL;
}