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
#ifndef CRGUI_CRWINDOWCONTAINER_H
#define CRGUI_CRWINDOWCONTAINER_H 1

#include <CRGUI/crExport.h>
#include <CRGUI/crTypes.h>
#include <CRGUI/crWin32.h>
#include <CRGUI/crImageStage.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/referenced.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <set>

namespace CRGUI {

#define WS_EX_LAYERED 0x00080000
#define	LWA_COLORKEY 0x1
#define	LWA_ALPHA 0x2
typedef BOOL (WINAPI *SetLayeredWindowAttributesProc) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
static SetLayeredWindowAttributesProc s_SetLayeredWindowAttributes = NULL;

class CRGUI_EXPORT crWindowContainer : public CRCore::crThread, public CRCore::Referenced
{
public:
	crWindowContainer( crImageStage* stage = NULL );
	
	bool isIdle(){ return m_close&&!m_stage.valid()&&!isRunning(); }
	
	bool attach( crImageStage* stage )
	{
		if(stage && m_stage.valid())
		{
			close();
			while (isRunning()) 
			{
				sleep(10);
			}
		}
		m_stage = stage;
		return true;		
	}

	void showWindow(int show);
	std::string doModal();

	virtual void run();
	void close();
    
	typedef std::set< CRCore::ref_ptr<crWindowContainer> > WindowThreadList;
	static crWindowContainer *getOrCreateAnIdleWindowThread( crImageStage* stage = NULL );
	static crWindowContainer *getWindowThread( crImageStage* stage );
	static HWND getWindow(crImageStage* stage);
	static void shutdown();
protected:
	~crWindowContainer();
	bool createrWindow();
	CRCore::ref_ptr<crWindow> m_window;
	CRCore::ref_ptr<crImageStage> m_stage;
	bool m_close;
	static WindowThreadList  m_windowThreads;
	static CRCore::crCriticalMutex m_threadsMutex;
};

}
#endif