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
#include <CRGUI/crWindowContainer.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crViewer.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <assert.h>
#include <windows.h>
#include <stdio.h>

using namespace CRGUI;
using namespace CRCore;
crWindowContainer::WindowThreadList crWindowContainer::m_windowThreads;
CRCore::crCriticalMutex crWindowContainer::m_threadsMutex;
crWindowContainer::crWindowContainer( crImageStage* stage ):m_stage(stage),m_close(false)
{
}
crWindowContainer::~crWindowContainer()
{
	////CRCore::notify(CRCore::ALWAYS)<<"crWindowContainer ~crWindowContainer"<<std::endl;
	//if(m_window.valid())
	//{
	//	m_window->close();
	//	m_window = NULL;
	//}	
	//m_stage = NULL;
	////m_close = true;
	//cancel();
	////CRCore::notify(CRCore::ALWAYS)<<" crWindowContainer closed11 "<<std::endl;

	////sleep(1);
	////while (isRunning()) 
	////{
	////	YieldCurrentThread();
	////}
	close();
	while (isRunning()) 
	{
		sleep(10);
	}
}

void crWindowContainer::close()
{
    m_close = true;
	//if(m_stage.valid())
	//{
	//	m_stage->showWindow(false);
	//	//if(isRunning())
	// //       m_stage->wakeUpThread();
	//}
}
crWindowContainer *crWindowContainer::getWindowThread( crImageStage* stage )
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_threadsMutex);
	CRCore::ref_ptr<crWindowContainer> setItr;
	for(WindowThreadList::iterator itr=m_windowThreads.begin();
		itr!=m_windowThreads.end();
		++itr)
	{
		setItr = *itr;
		if(setItr->m_stage == stage) return setItr.get();
	}
	return 0;
}

HWND crWindowContainer::getWindow(crImageStage* stage)
{
    crWindowContainer *container = getWindowThread( stage );
	if(container && container->m_window.valid())
	{
		return container->m_window->getHandle();
	}
	return GetActiveWindow();
}

crWindowContainer *crWindowContainer::getOrCreateAnIdleWindowThread( crImageStage* stage )
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_threadsMutex);
	crWindowContainer *idledt = NULL;
	CRCore::ref_ptr<crWindowContainer> setItr;
	for(WindowThreadList::iterator itr=m_windowThreads.begin();
		itr!=m_windowThreads.end();
		++itr)
	{
		//if((*itr)->m_close) 
		//{
		//	m_windowThreads.erase(itr);
		//	//idledt = itr->get();
		//	//continue;
		//}
		//if((*itr)->m_stage == stage) return itr->get();
		setItr = *itr;
		if(setItr->isIdle())
		{
			idledt = setItr.get();
			return idledt;
		}
	}
	idledt = new crWindowContainer;
	m_windowThreads.insert(idledt);
	return idledt;
}
void crWindowContainer::shutdown()
{
	CRCore::ScopedLock<crCriticalMutex> lock(m_threadsMutex);
	m_windowThreads.clear(); 
}
bool crWindowContainer::createrWindow()
{
	if(!m_window.valid())
	{
        m_window = new crWindow;
	}
	Window parent = NULL;
	crImageStage *parentStage = m_stage->getParentWindowStage();
	if(parentStage)
	{
		crWindowContainer *wc = getWindowThread(parentStage);
		if(wc) parent = wc->m_window->getHandle();
	}
	else if(m_stage->getNeedParentWindow())
	{
		CRProducer::crViewer *viewer =  CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(viewer && viewer->isInited())
			parent = viewer->getCameraWindow();
	}
	else if(m_stage->getName()!="main")
	{
		parent = cr2DStageManager::getInstance()->getParentWnd();
	}
	RECT rect;
	if(parent)
	{
		::GetClientRect(parent,&rect);
	}
	else
	{
		DEVMODE dm;
		memset(&dm,0,sizeof(dm));
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		rect.left = 0;
		rect.top = 0;
		rect.right = dm.dmPelsWidth;
		rect.bottom = dm.dmPelsHeight;
	}
	int x,y;
	unsigned int width,height;
	x = rect.left;
	y = rect.top;
	width = rect.right - x;
	height = rect.bottom - y;
	//if(!CRCore::crDisplaySettings::instance()->getFullScreen())
	//	height -= 26;
	m_stage->correctWinRect(x,y,width,height);
	if(parent)
	{
		if(!(GetWindowLong(parent, GWL_STYLE) & WS_POPUP))
		{
			m_window->setParentWnd(parent);
		}		
	}
	//else
	//{   
	//	//RECT rect;
	//	//::GetWindowRect(m_window->getParentWnd(),&rect);
	//	//x = rect.left;
	//	//y = rect.top;
	//	//width = rect.right - rect.left;
	//	//height = rect.bottom - rect.top;
	//	//m_stage->correctWinRect(x,y,width,height);
	//	m_stage->correctWinRect(x,y,width,height);
	//}

	//char  name[20];
	//sprintf(name,"crWindowContainer%d",m_windowThreads.size());
	//CRCore::notify(CRCore::FATAL)<<"crWindowContainer::createrWindow() "<<m_stage->getName()<<std::endl;
	return m_window->setup(x,y,width,height,m_stage->getName().c_str(),m_stage->getWinExStyle());
}

void crWindowContainer::showWindow(int show)
{ 	
	//if(crDisplaySettings::instance()->getRunMode() == 3)
	//	m_stage->showWindow(show,true);
	//else
	m_stage->showWindow(show,false);
	
	if(show&&!isRunning())
	{
		start(); 
	}
}
std::string crWindowContainer::doModal()
{
	m_close = false;
	m_stage->showWindow(1);

	int		mouseButtonMsg;		// mouse buttons
	int		mx,my;			// mouse position

	if(!m_window.valid()) m_window = new crWindow;

	if(!m_window->getHandle())
	{
		if(!createrWindow())
		{
			CRCore::notify(CRCore::FATAL)<<m_stage->getName()<<" crWindowContainer::doModal() createrWindow error"<<std::endl;
			return " ";
		}
	}
	//else
	//{
	//	RECT rect;
	//	if(m_window->getParentWnd())
	//	{
	//		::GetWindowRect(m_window->getParentWnd(),&rect);
	//	}
	//	else
	//	{
	//		DEVMODE dm;
	//		memset(&dm,0,sizeof(dm));
	//		dm.dmSize = sizeof(dm);
	//		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	//		rect.left = 0;
	//		rect.top = 0;
	//		rect.right = dm.dmPelsWidth;
	//		rect.bottom = dm.dmPelsHeight;
	//	}
	//	int x,y;
	//	unsigned int width,height;
	//	x = rect.left;
	//	y = rect.top;
	//	width = rect.right - x;
	//	height = rect.bottom - y;
	//	m_stage->correctWinRect(x,y,width,height);
	//	MoveWindow(m_window->getHandle(),x,y,width,height,false);
	//}
	Window      wnd = m_window->getHandle();
	Window      parentWnd = m_window->getParentWnd();
	Display     dc = GetDC(wnd);
	//CRCore::notify(CRCore::FATAL)<<"窗口打开:"<<wnd<<" threadid: "<<getThreadId()<<std::endl;
	// go into input loop
	bool onshow = false;
	int key;
	wchar_t charmsg;
	//m_window->showWindow(m_stage->getShow());
	int frameCount = 0;
	int frameDelay = m_stage->getFrameDelay();
	//SetForegroundWindow(wnd);

#ifdef _DEBUG
try
{
#endif
	m_stage->attachWindow(m_window.get());
	CRCore::Timer_t start;
	CRCore::Timer_t t1;
	float dt = 0.0f;

	while( !m_close )
	{
		if(m_stage->isHide())
		{
			onshow = false;
			sleep(m_stage->getFrameInterval());//10000
			continue;
		}
		if(m_stage->needScreenImage()) cr2DStageManager::getInstance()->refreshScreen();

		if(!onshow)
		{
			m_stage->initWindow();            
			m_window->showWindow(1,false);
			SetForegroundWindow(wnd);
			onshow = true;
			start = CRCore::Timer::instance()->tick();
			//sleep(m_stage->getFrameInterval());//10000
			frameCount = 0;
			m_stage->setNeedReDraw();
			m_stage->setIsOnShow(true);
			continue;
		}
		m_window->_doMessaging();

		if(m_window->beenSized())
			m_stage->moveWindow(m_window->getWindowRect());

		m_window->getMousePosition(mx,my);
		if(m_window->getMouseButtonMsg(mouseButtonMsg))
			m_stage->testButtons(mx,my,mouseButtonMsg);

		key = m_window->getPressedKey();
		if( key != -1)
			m_stage->inputKey(key);
		charmsg = m_window->getCharMsg();
		if(charmsg!=0)
			m_stage->inputChar(charmsg);

		if(frameCount == 0)
		{
			t1 = CRCore::Timer::instance()->tick();
			dt = CRCore::Timer::instance()->delta_s( start, t1 );
			start = t1;

			m_stage->frameWindow(dt);
		}
		frameCount = frameCount>frameDelay?0:frameCount+1;
		if(m_window->getNeedRedraw() || m_stage->getNeedReDraw()) m_stage->drawStage(dc);
		m_window->resetKey();

		SetFocus(wnd);
		sleep(m_stage->getFrameInterval());//10000
		//YieldCurrentThread();
	}
#ifdef _DEBUG
}
catch (...)
{
	CRCore::notify(CRCore::WARN)<<"crWindowContainer::doModal() Error Dlg = "<<m_stage->getName()<<std::endl;
}
#endif
	std::string id = m_stage->getFocusElement()->getName();
	//CRCore::notify(CRCore::ALWAYS)<<"id = "<<id.c_str()<<std::endl;
	m_stage->destroyWindow();
	ReleaseDC(wnd,dc);
	if(m_window.valid())
	{
		m_window->close();
		m_window = NULL;
	}
	if(parentWnd) 
	{
		SetForegroundWindow(parentWnd);
	}
	m_stage->attachWindow(NULL);
	m_stage->showWindow(false);
	m_stage = NULL;
	//m_windowThreads.erase(this);

	/*m_window->showWindow(0);
	m_window->resetKey();*/
	//if(parentWnd) 
	//{
	//	SetActiveWindow(parentWnd);
	//	SetFocus(parentWnd);
	//	SetForegroundWindow(parentWnd);
	//}
	return id;
}

void crWindowContainer::run()
{
	m_close = false;
	int		mouseButtonMsg;		// mouse buttons
	int		mx,my;			// mouse position
	
	if(!m_window.valid()) m_window = new crWindow;

	if(!m_window->getHandle())
	{
		if(!createrWindow())
		{
			CRCore::notify(CRCore::FATAL)<<m_stage->getName()<<" crWindowContainer::doModal() createrWindow error"<<std::endl;
			return;
		}
	}
	//else
	//{
	//	RECT rect;
	//	if(m_window->getParentWnd())
	//	{
	//		::GetWindowRect(m_window->getParentWnd(),&rect);
	//	}
	//	else
	//	{
	//		DEVMODE dm;
	//		memset(&dm,0,sizeof(dm));
	//		dm.dmSize = sizeof(dm);
	//		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	//		rect.left = 0;
	//		rect.top = 0;
	//		rect.right = dm.dmPelsWidth;
	//		rect.bottom = dm.dmPelsHeight;
	//	}
	//	int x,y;
	//	unsigned int width,height;
	//	x = rect.left;
	//	y = rect.top;
	//	width = rect.right - x;
	//	height = rect.bottom - y;
	//	m_stage->correctWinRect(x,y,width,height);
	//	MoveWindow(m_window->getHandle(),x,y,width,height,false);
	//}
	Window      wnd = m_window->getHandle();
	Window      parentWnd = m_window->getParentWnd();
	Display     dc = GetDC(wnd);

	//CRCore::notify(CRCore::FATAL)<<"窗口打开:"<<wnd<<" threadid: "<<getThreadId()<<std::endl;
	// go into input loop
	bool onshow = false;
	int key;
	wchar_t charmsg;
	int frameCount = 0;
	int frameDelay = m_stage->getFrameDelay();
	
#ifdef _DEBUG
try
{
#endif
    m_stage->attachWindow(m_window.get());
	CRCore::Timer_t start;
	CRCore::Timer_t t1;
	float dt = 0.0f;
	while( !m_close && m_stage.valid() )
	{
		//m_window->showWindow(m_stage->getShow());
		if(!m_stage->isHide())
		{
			//if(m_stage->needScreenImage()) 
			//{// showStage can not be used to show Draw_Mask Dialogs
			//	cr2DStageManager::getInstance()->snapScreen();
			//	cr2DStageManager::getInstance()->refreshScreen();
			//}
			if(!onshow)
			{
				m_stage->initWindow();
				m_window->showWindow(1,false);
				SetForegroundWindow(wnd);
				onshow = true;
				frameCount = 0;
				start = CRCore::Timer::instance()->tick();
			    m_stage->setNeedReDraw();
				m_stage->setIsOnShow(true);
				//sleep(m_stage->getFrameInterval());//10000
				continue;
			}
            m_window->_doMessaging();

			if(m_window->beenSized())
                m_stage->moveWindow(m_window->getWindowRect());

			m_window->getMousePosition(mx,my);
			if(m_window->getMouseButtonMsg(mouseButtonMsg))
				m_stage->testButtons(mx,my,mouseButtonMsg);

			key = m_window->getPressedKey();
			if( key != -1)
			    m_stage->inputKey(key);
			charmsg = m_window->getCharMsg();
			if(charmsg!=0)
                m_stage->inputChar(charmsg);

			if(frameCount == 0)
			{
				t1 = CRCore::Timer::instance()->tick();
				dt = CRCore::Timer::instance()->delta_s( start, t1 );
				start = t1;
				m_stage->frameWindow(dt);
			}
			frameCount = frameCount>frameDelay?0:frameCount+1;
			if(m_window->getNeedRedraw() || m_stage->getNeedReDraw()) m_stage->drawStage(dc);
			m_window->resetKey();
		}
		//else if(onshow)
		//{
  //          onshow = false;
  //          m_stage->destroyWindow();
		//	m_window->resetKey();
		//	m_window->showWindow(0);
		//	if(parentWnd) 
		//	{
		//		SetForegroundWindow(parentWnd);
		//	}
		//}
		//else
		//{
	 //       m_stage->threadWait();
		//}
		//else
		//{
		//	if(onshow)
		//	{
		//		m_stage->destroyWindow();
		//		m_window->showWindow(0);
		//		m_window->resetKey();
		//		if(parentWnd) 
		//		{
		//			SetActiveWindow(parentWnd);
		//			SetFocus(parentWnd);
		//			SetForegroundWindow(parentWnd);
		//		}
		//		//if(m_stage->needScreenImage())
		//		//{
		//		//	cr2DStageManager::getInstance()->snapScreen();
		//		//	cr2DStageManager::getInstance()->refreshScreen();
		//		//}
		//		onshow = false;
		//	}
		//}
		sleep(m_stage->getFrameInterval());//10000
		//YieldCurrentThread();
	}
#ifdef _DEBUG
}
catch (...)
{
	CRCore::notify(CRCore::WARN)<<"crWindowContainer::run() Error Dlg = "<<m_stage->getName()<<std::endl;
}
#endif
	//m_stage->showWindow(false);
	if(m_stage.valid()) m_stage->destroyWindow();
	ReleaseDC(wnd,dc);
	if(m_window.valid())
	{
		//m_window->showWindow(0,false);
		//m_window->resetKey();
		m_window->close();
		m_window = NULL;
	}
	if(parentWnd) 
	{
		SetForegroundWindow(parentWnd);
	}
	if(m_stage.valid())
	{
		m_stage->attachWindow(NULL);
		m_stage->showWindow(false);
		m_stage = NULL;
	}
	//if(parentWnd) 
	//{
	//	SetActiveWindow(parentWnd);
	//	SetFocus(parentWnd);
	//	SetForegroundWindow(parentWnd);
	//}
	//m_windowThreads.erase(this);
}

//void crWindowContainer::run()
//{ 
//	bool		left,right;		// mouse buttons
//	int			mx,my;			// mouse position
//	Window      wnd = m_window->getHandle();
//	Display     dc = GetDC(wnd);
//	m_stage->drawStage(dc);
//	// go into input loop
//	while(!m_stage->isHide())
//	{
//		// is a button pressed?
//		m_window->getMouseButton(left,right);
//		// yes
//		if(left|right)
//		{
//			// get mouse position
//			m_window->getMousePosition(mx,my);
//
//			// send it to the button test
//			if(m_stage->testButtons(mx,my,left?1:2))
//			{
//				// draw the splash window
//				m_stage->drawStage(dc);
//
//				// update the display
//				UpdateWindow(wnd);
//			}
//
//			// wait for mouse button to go up
//			sleep(10);
//			m_window->getMouseButton(left,right);
//			while(left|right)
//			{
//				if(m_stage->testButtons(mx,my,left?3:4))//3左键拖动，4右键拖动
//				{
//					// draw the splash window
//					m_stage->drawStage(dc);
//					// update the display
//					UpdateWindow(wnd);
//				}
//				m_window->getMouseButton(left,right);
//				sleep(20);
//			}
//		}
//		else
//		{
//			// get mouse position
//			m_window->getMousePosition(mx,my);
//
//			// send it to the button test
//			if(m_stage->testButtons(mx,my,0))//0表示鼠标未点
//			{
//				// draw the splash window
//				m_stage->drawStage(dc);
//
//				// update the display
//				UpdateWindow(wnd);
//			}
//		}
//
//		sleep(100);
//	}
//
//	m_window->showWindow(0);
//	m_closeMutex.unlock();
//}
