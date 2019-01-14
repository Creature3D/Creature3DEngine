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
#ifndef CRGUI_CRIMAGESTAGE_H
#define CRGUI_CRIMAGESTAGE_H 1

#include <CRGUI/crExport.h>
#include <CRGUI/crTypes.h>
#include <CRGUI/crWin32.h>
#include <CRGUI/crElement.h>
#include <CRGUI/crTextAttribute.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crCopyOp.h>
#include "rcfg/ConfigScriptOO.h"
#include <map>
#include <set>
#include <string>

namespace CRGUI{

class CRGUI_EXPORT crImageStage : public crElement
{
public:
	crImageStage();
	crImageStage(const crImageStage& element);
	META_Element(CRGUI,crImageStage)

	static crImageStage *testLoad(const std::string &fileName);
	virtual void load(const std::string &fileName);

	void correctWinRect( int &x, int &y, unsigned int &width, unsigned int &height );

	virtual void setElementImage( const std::string& image );
	virtual void setElementDownImage( const std::string& image );
	virtual void setElementMaskImage( const std::string& image );
	virtual void setElementDisableImage( const std::string& image );

	void attachWindow(crWindow* window);
	crWindow *getWindow();

	void showWindow(bool show, bool async = true);
	bool isHide(){ return !m_show; }

	void setNeedParentWindow(bool need){ m_needParentWindow = need; }
	bool getNeedParentWindow(){ return m_needParentWindow; }

	int getCurrentMouseButtonMsg(){ return m_currentMouseButtonMsg; }
	int getLastMouseButtonMsg(){ return m_lastMouseButtonMsg; }
	void getCurrentMousePosition(int &x, int &y);
	void getLastMousePosition(int &x, int &y);

	//typedef std::map<std::string, CRCore::ref_ptr<crElement> > ElementMap;
	//ElementMap &getElementMap() { return m_elementMap; }
	//crElement *getElement(const std::string &nameid);
	//void addElement(crElement *element);
	typedef std::vector< CRCore::ref_ptr<crElement> > ElementVec;
	ElementVec &getElementVec() { return m_elementVec; }
	crElement *getElement(const std::string &nameid);
	void addElement(crElement *element);

	void setDefaultFocusElementName(const std::string &name){ m_defaultFocusElementName = name; }
	void setFocusElement(crElement *element);
	void lostFocusElement(crElement *element);
	crElement* getFocusElement();

	enum ScaleWithScreen
	{
		Scale_None,
		Scale_Width,
		Scale_Height,
		Scale_Both
	};

	void setScaleWithScreen(ScaleWithScreen scaleWithScreen);
	ScaleWithScreen getScaleWithScreen();

	bool needScreenImage();

	static void resize();
	void resize(const std::string &fileName);

	void setParentWindowStage(crImageStage *stage) { m_parentWindowStage = stage; }
	crImageStage *getParentWindowStage(){ return m_parentWindowStage; }
	virtual void setRect(float x,float y,float wid,float hit);
	virtual void setRect(const CRCore::crVector4i &rect);
public://window events
    virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual bool inputKey(int key);
	virtual bool inputChar(wchar_t c);

    int drawStage(Display dc);

	void backup();
	void resumeBackupData();

	//void setNeedRedraw(bool bln){ m_needRedraw = bln; }
	virtual void initWindow();
	virtual void updateWindow();
	virtual void frameWindow(float dt);
	virtual void destroyWindow();
    
	void moveWindow(const CRCore::crVector4i &rect);
    
	void setFrameInterval(int frameInterval);
	inline int getFrameInterval() { return m_frameInterval; }
	void setFrameDelay(int frameDelay);
	inline int getFrameDelay() { return m_frameDelay; }

	virtual bool pressed(int x,int y);
	static _crInt32 parseEventMsg( const std::string &str );

	void setNeedReDraw();
	bool getNeedReDraw();
	//void threadWait();
	//void wakeUpThread();
	int getWinExStyle();
	virtual void setPos(float x, float y);
	void setIsOnShow(bool bln);
	bool getIsOnShow();

	static void shutdown(){ m_stageFileMap.clear(); }
protected:
    void loadElementEvents(rcfg::ConfigScript& cfg_script,crElement *element);
	void loadElements(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	
	void loadElementHandles(rcfg::ConfigScript& cfg_script,crElement *element);
	void loadHandles(rcfg::ConfigScript& cfg_script,CRCore::crHandle *handle);

	virtual void setCapture();
	virtual void releaseCapture();

	virtual int drawStageCopy(Display dc);
	virtual int drawStageMask(Display dc);
	virtual int drawStageTransparent(Display dc);

	ElementVec m_elementVec;
	//ElementMap m_elementMap;
	CRCore::ref_ptr<crElement> m_focusElement;
	typedef std::map<std::string,CRCore::ref_ptr<crImageStage> > StageFileMap;
	static StageFileMap m_stageFileMap;

	int    m_currentMouseButtonMsg;
	int    m_currentMousePosx;
	int    m_currentMousePosy;
	int    m_lastMouseButtonMsg;
    int    m_lastMousePosx;
    int    m_lastMousePosy;

	std::string m_defaultFocusElementName;
	ScaleWithScreen m_scaleWithScreen;

	bool   m_needParentWindow;
	crImageStage *m_parentWindowStage;

	int m_frameInterval;
	int m_frameDelay;
	bool m_needRedraw;
	
    //Window m_hwnd;
	CRCore::ref_ptr<crWindow> m_window;

	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
	//CRCore::crMutex m_stageMutex;
	//CRCore::crCondition m_stageCondition;

	CRCore::crVector4i m_rect_copy;
	int m_winExstyle;
	bool m_isOnShow;
};

}
#endif