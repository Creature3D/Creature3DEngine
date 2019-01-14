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
#ifndef CRGUI_CRWIN32_H
#define CRGUI_CRWIN32_H 1

#include <CRGUI/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crVector4.h>
#include <deque>
//#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
//#include <winuser.h>
namespace CRGUI{
#define WM_MOUSEWHEEL                   0x020A
#define WM_MOUSEDRAG                    0x020E
class crWindowContainer;
class CRGUI_EXPORT crWindow : public CRCore::Referenced
{
friend class crWindowContainer;
public:
	crWindow();
	~crWindow();
	bool setup(int x,int y,int wid,int hit,const char *name,int exstyle);
	bool setupTitle(int x,int y,int wid,int hit,const char *name);
	void close();
	bool beenSized() { bool temp=m_sized; m_sized=false; return temp; }

	const CRCore::crVector4i& getWindowRect()const{ return m_rect; }

	void setHandle(HWND win){ m_hwnd = win; }
	HWND getHandle() const		{ return m_hwnd; };
	HINSTANCE getInstance() const { return m_hins; };
	 
	void getMousePosition(int &x,int &y) 
	{
		x=m_mx;	y=m_my;
	};
	bool getMouseButtonMsg(int &mouseButtonMsg)
	{
        mouseButtonMsg = m_mouseButtonMsg;
		return m_mouseEvent;
	}

	bool getKey(int val) 
	{ 
		if(val<0 || val>255)
			return false; 
		return m_keys[val];	
	}

	bool *getKeys()	
	{ 
		return (bool *)m_keys;
	}
	int getPressedKey()
	{
		for(int i = 0; i < 256; i++)
		{
			if(m_keys[i])
				return i;
		}
        return -1;
	}

	void dataAccess(int key,short value);
    
	void setParentWnd(HWND win);
	HWND getParentWnd(){ return m_hParentWnd;}
	void showWindow(int show, bool async);
	void resetKey();
	void needRedraw(bool redraw = true){ m_needRedraw = redraw; }
    bool getNeedRedraw(){ bool temp=m_needRedraw; m_needRedraw=false; return temp;  }

	typedef std::deque<wchar_t> CharMsgDeque;
	inline wchar_t getCharMsg() 
	{
		wchar_t c = 0;
		if(!m_charMsgDeque.empty())
		{
            c = m_charMsgDeque.front();
            m_charMsgDeque.pop_front();
		}
		return c;
	}
	inline void pushChar(wchar_t c){ m_charMsgDeque.push_back(c); needRedraw(); }
	
	static HCURSOR s_defaultCursor;
	void _doMessaging();
protected:
	CRCore::crVector4i m_rect;
	int			m_mx,m_my;
	int         m_mouseWheelDelta;
	int         m_mouseButtonMsg;
	bool        m_mouseEvent;
	bool		m_keys[256];
	bool		m_sized;
	CharMsgDeque m_charMsgDeque;
	//bool        m_redraw;
	HWND		m_hwnd;	
    HWND		m_hParentWnd;	
	HINSTANCE	m_hins;
	std::string m_windowName;
	bool        m_needRedraw;
	BOOL _register(const char *name);
};

}

#endif