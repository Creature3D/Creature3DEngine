/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRGUI/crImageStage.h>
#include <CRGUI/crElementNode.h>
//#include <CRGUI/crEventCallback.h>
#include <CRGUI/cr2DStageManager.h>
//#include <CRAI/crAIGroup.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crNotify.h>
#include <CRCore/crMultiSwitch.h>
#include <CRIOManager/crLoadManager.h>
#include <sys/stat.h>
#include <stdio.h>
using namespace CRCore;
using namespace CRGUI;
using namespace CRIOManager;
crImageStage::StageFileMap crImageStage::m_stageFileMap;

crImageStage::crImageStage()
{
	m_winExstyle = 0;
	m_rect.set(0,0,640,480);
	m_show = false;

	m_currentMouseButtonMsg = 0;
	m_currentMousePosx = 0;
    m_currentMousePosy = 0;
	m_lastMouseButtonMsg = 0;
	m_lastMousePosx = 0;
    m_lastMousePosy = 0;

	m_drawMode = Draw_Copy;

	m_focusElement = NULL;
	m_scaleWithScreen = Scale_None;

	m_needParentWindow = true;
	m_frameInterval = 10;
	m_frameDelay = 10;
	m_needRedraw = false;

	m_parentWindowStage = NULL;
	m_isOnShow = false;
}

crImageStage::crImageStage(const crImageStage& element):
    crElement(element),
	m_winExstyle(element.m_scaleWithScreen),
	m_scaleWithScreen(element.m_scaleWithScreen),
	m_currentMouseButtonMsg(0),
    m_currentMousePosx(0),
	m_currentMousePosy(0),
	m_lastMouseButtonMsg(0),
	m_lastMousePosx(0),
	m_lastMousePosy(0),
	m_focusElement(NULL),
	m_needParentWindow(true),
	m_needRedraw(false),
	m_parentWindowStage(element.m_parentWindowStage),
	m_hScrollBarName(element.m_hScrollBarName),
	m_vScrollBarName(element.m_vScrollBarName),
	m_frameInterval(element.m_frameInterval),
	m_frameDelay(element.m_frameDelay),
	m_isOnShow(false)
{
	for( ElementVec::const_iterator itr = element.m_elementVec.begin();
		itr != element.m_elementVec.end();
		++itr )
	{
		m_elementVec.push_back((*itr)->clone(CRCore::crCopyOp::DEEP_COPY_ALL));
	}
}

void crImageStage::setFrameInterval(int frameInterval)
{
    m_frameInterval = frameInterval;
}

void crImageStage::setFrameDelay(int frameDelay)
{
    m_frameDelay = frameDelay;
}

void crImageStage::setScaleWithScreen(crImageStage::ScaleWithScreen scaleWithScreen)
{
    m_scaleWithScreen = scaleWithScreen;
}

crImageStage::ScaleWithScreen crImageStage::getScaleWithScreen()
{
    return m_scaleWithScreen;
}

void crImageStage::resize()
{	
	for( StageFileMap::iterator itr = m_stageFileMap.begin();
		 itr != m_stageFileMap.end();
		 ++itr )
	{
        itr->second->resize(itr->first);
	}
	CRGUI::crElement::clearBmpCache();
}

void crImageStage::resize(const std::string &fileName)
{
	float widthScale = 1.0f;
	float heightScale = 1.0f;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	switch (m_scaleWithScreen)
	{
	case Scale_Width:
		widthScale = screenWidth / float(m_rect[2]);
		break;
	case Scale_Height:
		heightScale = screenHeight / float(m_rect[3]);
		break;
	case Scale_Both:
		widthScale = screenWidth / float(m_rect[2]);
		heightScale = screenHeight / float(m_rect[3]);
		break;
	}

	if(widthScale == 1.0f && heightScale == 1.0f) return;

	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
	{
		CRCore::notify(CRCore::FATAL)<<"crImageStage::load(): file open error,"<<fileName<<std::endl;
		return;
	}

	std::string str;
	std::vector<float> v_i;
	if (cfg_script.Get("StageRect", v_i)) 
	{
		if (v_i.size() != 4)
			throw std::runtime_error("crImageStage::load(): " + cfg_script.CurrentScope() + "Rect should be a vector with 4 elements");

		m_rect.set(v_i[0], v_i[1], v_i[2], v_i[3]);
	}

	widthScale = 1.0;
	heightScale = 1.0;
	if (cfg_script.Get("ScaleWithScreen", str))
	{
		if(str.compare("Scale_Width")==0)
		{
			setScaleWithScreen(Scale_Width);
			widthScale = screenWidth / float(m_rect[2]);
			m_rect[0] = 0;
			m_rect[2] = screenWidth;
		}
		else if(str.compare("Scale_Height")==0)
		{
			setScaleWithScreen(Scale_Height);
			heightScale = screenHeight / float(m_rect[3]);
			m_rect[1] = 0;
			m_rect[3] = screenHeight;
		}
		else if(str.compare("Scale_Both")==0)
		{
			setScaleWithScreen(Scale_Both);
			widthScale = screenWidth / float(m_rect[2]);
			heightScale = screenHeight / float(m_rect[3]);
			m_rect.set(0,0,screenWidth,screenHeight);
		}
		else
		{
			setScaleWithScreen(Scale_None);
		}
	}

	if (cfg_script.Get("Image", str))
		setElementImage(str);
	if (cfg_script.Get("DownImage", str))
		setElementDownImage(str);
	if (cfg_script.Get("MaskImage", str))
		setElementMaskImage(str);
	if (cfg_script.Get("DisableImage", str))
		setElementDisableImage(str);
	int nElement = 1;
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		cfg_script.Push("Element", nElement++);
		(*itr)->resize(cfg_script,widthScale,heightScale);
		cfg_script.Pop();
	}
	m_rect_copy = m_rect;
}

crImageStage *crImageStage::testLoad(const std::string &fileName)
{
    StageFileMap::iterator itr= m_stageFileMap.find(fileName);
    if (itr!=m_stageFileMap.end()) return itr->second.get();
    return NULL;
}

void crImageStage::load(const std::string &fileName)
{
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
	{
		CRCore::notify(CRCore::FATAL)<<"crImageStage::load(): file open error,"<<fileName<<std::endl;
		return;
	}

	std::string str;
	int int1;

	if(cfg_script.Get("StageName", str))
		setName(str);
	if(cfg_script.Get("FrameInterval", int1))
		setFrameInterval(int1);
	if(cfg_script.Get("FrameDelay", int1))
		setFrameDelay(int1);
	std::vector<float> v_i;
	if (cfg_script.Get("StageRect", v_i)) 
	{
		if (v_i.size() != 4)
			throw std::runtime_error("crImageStage::load(): " + cfg_script.CurrentScope() + "Rect should be a vector with 4 elements");

		m_rect.set(v_i[0], v_i[1], v_i[2], v_i[3]);
	}
    
	float widthScale = 1.0;
	float heightScale = 1.0;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	if (cfg_script.Get("ScaleWithScreen", str))
	{
		if(str.compare("Scale_Width")==0)
		{
			setScaleWithScreen(Scale_Width);
			widthScale = screenWidth / float(m_rect[2]);
			//m_rect[0] = 0;
			m_rect[2] = screenWidth;
		}
		else if(str.compare("Scale_Height")==0)
		{
			setScaleWithScreen(Scale_Height);
			heightScale = screenHeight / float(m_rect[3]);
			//m_rect[1] = 0;
			m_rect[3] = screenHeight;
		}
		else if(str.compare("Scale_Both")==0)
		{
			setScaleWithScreen(Scale_Both);
			widthScale = screenWidth / float(m_rect[2]);
			heightScale = screenHeight / float(m_rect[3]);
			m_rect[2] = screenWidth;
            m_rect[3] = screenHeight;
			//m_rect.set(0,0,screenWidth,screenHeight);
		}
		else
		{
			setScaleWithScreen(Scale_None);
		}
	}

    if (cfg_script.Get("DrawMode", str))
	{
        if(str.compare("Copy")==0)
			m_drawMode = Draw_Copy;
		else if(str.compare("Mask")==0)
			m_drawMode = Draw_Mask;
		else if(str.compare("Transparent")==0)
			m_drawMode = Draw_Transparent;
	}

	if (cfg_script.Get("NeedParentWindow", int1))
		setNeedParentWindow(int1);
	if (cfg_script.Get("ParentStage", str))
		setParentWindowStage(cr2DStageManager::getInstance()->findStage(str));

	m_winExstyle = 0;
	if (cfg_script.Get("WinExStyle", str))
	{
		if(str.find("WS_EX_TOOLWINDOW") != std::string::npos)
		{
			m_winExstyle |= WS_EX_TOOLWINDOW;
		}
		if(str.find("WS_EX_APPWINDOW") != std::string::npos)
		{
			m_winExstyle |= WS_EX_APPWINDOW;
		}
	}
	if(m_winExstyle == 0)
		m_winExstyle = WS_EX_APPWINDOW;
	if (cfg_script.Get("Image", str))
		setElementImage(str);
	if (cfg_script.Get("DownImage", str))
		setElementDownImage(str);
	if (cfg_script.Get("MaskImage", str))
		setElementMaskImage(str);
	if (cfg_script.Get("DisableImage", str))
		setElementDisableImage(str);

	if (cfg_script.Get("DefaultFocusElementName", str))
		setDefaultFocusElementName(str);

	if (cfg_script.Get("Enable", int1))
		setEnable(int1);
	if (cfg_script.Get("Show", int1))
		setShow(int1);
	if (cfg_script.Get("CanDrag", int1))
		setCanDrag(int1);
	if (cfg_script.Get("CanFocus", int1))
		setCanFocus(int1);
	if(cfg_script.Get("CanCapture", int1))
		setCanCapture(int1);

	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);

	loadElementEvents(cfg_script,this);
	loadElementHandles(cfg_script,this);
	loadElements(cfg_script,widthScale,heightScale);


	//加入到Map
	m_stageFileMap[fileName] = this;

	m_rect_copy = m_rect;
}
int crImageStage::getWinExStyle()
{
	return m_winExstyle;
}
#define addEventCallback(event) \
if(str1.compare("LBtnUp")==0) \
    element->addLButtonUpEvent(event); \
else if(str1.compare("LBtnDown")==0) \
	element->addLButtonDownEvent(event); \
else if(str1.compare("LBtnDblClk")==0) \
	element->addLButtonDblClkEvent(event); \
else if(str1.compare("RBtnUp")==0) \
	element->addRButtonUpEvent(event); \
else if(str1.compare("RBtnDown")==0) \
	element->addRButtonDownEvent(event); \
else if(str1.compare("RBtnDblClk")==0) \
	element->addRButtonDblClkEvent(event); \
else if(str1.compare("MBtnUp")==0) \
	element->addMButtonUpEvent(event); \
else if(str1.compare("MBtnDown")==0) \
	element->addMButtonDownEvent(event); \
else if(str1.compare("MBtnDblClk")==0) \
	element->addMButtonDblClkEvent(event); \
else if(str1.compare("MouseMove")==0) \
	element->addMouseMoveEvent(event); \
else if(str1.compare("MouseWheel")==0) \
	element->addMouseWheelEvent(event); \
else if(str1.compare("MouseOn")==0) \
	element->addMouseOnEvent(event); \
else if(str1.compare("GetFocus")==0) \
	element->addGetFocusEvent(event); \
else if(str1.compare("LostFocus")==0) \
	element->addLostFocusEvent(event); \
else if(str1.compare("InitWindow")==0) \
	element->addInitWindowEvent(event); \
else if(str1.compare("DestroyWindow")==0) \
	element->addDestroyWindowEvent(event); \
else if(str1.compare("InputKey")==0) \
	element->addInputKeyEvent(event); \
else if(str1.compare("InputChar")==0) \
	element->addInputCharEvent(event); \
else if(str1.compare("UpdateData")==0) \
	element->addUpdateDataEvent(event); \
else if(str1.compare("PreDraw")==0) \
	element->addPreDrawEvent(event);

void crImageStage::loadElementEvents(rcfg::ConfigScript& cfg_script,crElement *element)
{
	ref_ptr<CRCore::crEventCallback> event;
	int i = 1;
	int j,k;
	std::string str,str1;
	while (cfg_script.Push("ElementEvent", i++)) 
	{
		cfg_script.Get("EventType", str1);
		j = 1;
		while (cfg_script.Push("Event", j++))
		{
			cfg_script.Get("EventFunction", str);
			event = crEventCallbackManager::getInstance()->getEventCallback(str);
            if(!event) 
			{
				CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadStageEvents(): 未找到EventFunction: "<<str<<std::endl;
				continue;
			}
			k = 1;
			while (cfg_script.Get("EventParam", str,k++)) 
			{
				event->addEventParam(k-2,str);
			}

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementEvents(): "<<cfg_script.GetLastError()<<std::endl;
			addEventCallback(event.get());
		}
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementEvents(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

_crInt32 crImageStage::parseEventMsg( const std::string &str )
{
	if(str.compare("NULL") == 0)
		return NULL;
	if(str.compare("LBtnUp") == 0)
		return WCH_UI_LBtnUp;
	if(str.compare("LBtnDown") == 0)
		return WCH_UI_LBtnDown;
	if(str.compare("LBtnDblClk") == 0)
		return WCH_UI_LBtnDblClk;
	if(str.compare("RBtnUp") == 0)
		return WCH_UI_RBtnUp;
	if(str.compare("RBtnDown") == 0)
		return WCH_UI_RBtnDown;
	if(str.compare("RBtnDblClk") == 0)
		return WCH_UI_RBtnDblClk;
	if(str.compare("MBtnUp") == 0)
		return WCH_UI_MBtnUp;
	if(str.compare("MBtnDown") == 0)
		return WCH_UI_MBtnDown;
	if(str.compare("MBtnDblClk") == 0)
		return WCH_UI_MBtnDblClk;
	if(str.compare("MouseMove") == 0)
		return WCH_UI_MouseMove;
	if(str.compare("MouseWheel") == 0)
		return WCH_UI_MouseWheel;
	if(str.compare("MouseOn") == 0)
		return WCH_UI_MouseOn;
	if(str.compare("MouseDrag") == 0)
		return WCH_UI_MouseDrag;

	if(str.compare("GetFocus") == 0)
		return WCH_UI_GetFocus;
	if(str.compare("LostFocus") == 0)
		return WCH_UI_LostFocus;
	if(str.compare("InitWindow") == 0)
		return WCH_UI_InitWindow;
	if(str.compare("DestroyWindow") == 0)
		return WCH_UI_DestroyWindow;
	if(str.compare("InputKey") == 0)
		return WCH_UI_InputKey;
	if(str.compare("InputChar") == 0)
		return WCH_UI_InputChar;
	if(str.compare("UpdateData") == 0)
		return WCH_UI_UpdateData;
	if(str.compare("PreDraw") == 0)
		return WCH_UI_PreDraw;
	if(str.compare("Frame") == 0)
		return WCH_UI_Frame;

	if(str.compare("WinFocus") == 0)
		return WCH_UI_WINFOCUS;

	if(str.compare("WCH_USER") == 0)
		return WCH_USER;
	if(str.find("WCH_USER+")==0)
	{
		int keylen = strlen("WCH_USER+");
		std::string user = str;
		user.erase(0,keylen);
		return WCH_USER + atoi(user.c_str());
	}

	//CRCore::notify(CRCore::FATAL)<<"CREncapsulation::parseEventMsg(): 该消息目前不支持 "<<str<<std::endl;
	return atoi(str.c_str());
}

void crImageStage::loadHandles(rcfg::ConfigScript& cfg_script,crHandle *handle)
{
	ref_ptr<CRCore::crHandle> logic,method;
	int nLogic,nMethod,/*nMethod2,*/nParam;
	std::string str;

	nMethod = 1;
	while (cfg_script.Push("Method", nMethod++))
	{
		cfg_script.Get("Class", str);
		method = crHandleManager::getInstance()->getHandle(str);
		if(!method) 
		{
			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			method->addParam(nParam-2,str);
		}

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		handle->inputHandle(nMethod-2,method.get());
	}

	nLogic = 1;
	while (cfg_script.Push("Logic", nLogic++))
	{
		cfg_script.Get("Class", str);
		logic = crHandleManager::getInstance()->getHandle(str);
		if(!logic) 
		{
			CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
			continue;
		}
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			logic->addParam(nParam-2,str);
		}

		//nMethod2 = 1;
		//while (cfg_script.Push("Method", nMethod2++))
		//{
		//	cfg_script.Get("Class", str);
		//	method = crHandleManager::getInstance()->getHandle(str);
		//	if(!method) 
		//	{
		//		CRCore::notify(CRCore::NOTICE)<<"CREncapsulation::loadHandles: 未找到Handle: "<<str<<std::endl;
		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;
		//		continue;
		//	}
		//	nParam = 1;
		//	while (cfg_script.Get("Param", str,nParam++)) 
		//	{
		//		method->addParam(nParam-2,str);
		//	}

		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		//	logic->inputHandle(nMethod2-2,method.get());
		//}

	    loadHandles(cfg_script,logic.get());

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"CREncapsulation::loadHandles: "<<cfg_script.GetLastError()<<std::endl;

		handle->inputHandle(nLogic+nMethod-4,logic.get());
	}
}

void crImageStage::loadElementHandles(rcfg::ConfigScript& cfg_script,crElement *element)
{
	ref_ptr<CRCore::crHandle> driver/*,logic,method*/;
	int nHandle = 1;
	int /*nLogic,nMethod,*/nParam;
	std::string str;
	_crInt32 msgParam1,msgParam2;
	while (cfg_script.Push("ElementHandle", nHandle++)) 
	{
		cfg_script.Get("MsgParam", str,1);
		msgParam1 = parseEventMsg(str);
		cfg_script.Get("MsgParam", str,2);
		msgParam2 = parseEventMsg(str);

		cfg_script.Push("Driver");
		cfg_script.Get("Class", str);
		driver = crHandleManager::getInstance()->getHandle(str);
		nParam = 1;
		while (cfg_script.Get("Param", str,nParam++)) 
		{
			driver->addParam(nParam-2,str);
		}

		//nLogic = 1;
		//while (cfg_script.Push("Logic", nLogic++))
		//{
		//	cfg_script.Get("Class", str);
		//	logic = crHandleManager::getInstance()->getHandle(str);
		//	if(!logic) 
		//	{
		//		CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
		//		continue;
		//	}
		//	nParam = 1;
		//	while (cfg_script.Get("Param", str,nParam++)) 
		//	{
		//		logic->addParam(nParam-2,str);
		//	}

		//	nMethod = 1;
		//	while (cfg_script.Push("Method", nMethod++))
		//	{
		//		cfg_script.Get("Class", str);
		//		method = crHandleManager::getInstance()->getHandle(str);
		//		if(!method) 
		//		{
		//			CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
		//			if (!cfg_script.Pop())
		//				CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
		//			continue;
		//		}
		//		nParam = 1;
		//		while (cfg_script.Get("Param", str,nParam++)) 
		//		{
		//			method->addParam(nParam-2,str);
		//		}

		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

		//		logic->inputHandle(nMethod-2,method.get());
		//	}

		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

		//	driver->inputHandle(nLogic-2,logic.get());
		//}

		//nMethod = 1;
		//while (cfg_script.Push("Method", nMethod++))
		//{
		//	cfg_script.Get("Class", str);
		//	method = crHandleManager::getInstance()->getHandle(str);
		//	if(!method) 
		//	{
		//		CRCore::notify(CRCore::NOTICE)<<"crImageStage::loadElementHandles(): 未找到Handle: "<<str<<std::endl;
		//		if (!cfg_script.Pop())
		//			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
		//		continue;
		//	}
		//	nParam = 1;
		//	while (cfg_script.Get("Param", str,nParam++)) 
		//	{
		//		method->addParam(nParam-2,str);
		//	}

		//	if (!cfg_script.Pop())
		//		CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

		//	driver->inputHandle(nLogic+nMethod-4,method.get());
		//}
	    loadHandles(cfg_script,driver.get());
		
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;

		element->insertHandle(MAKEINT64(msgParam1,msgParam2),driver.get());

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElementHandles(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crImageStage::loadElements(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement* element;
	std::string str;
	int i = 1;
	while (cfg_script.Push("Element", i++)) 
	{	
        if(!cfg_script.Get("ElementType", str))
		{
            throw std::runtime_error("crImageStage::loadElements(): load ElementType error");
		}
		if(str.compare("Button")==0)
		{
            element = new crButtonElement;
		}
		else if(str.compare("CheckBox")==0)
		{
			element = new crCheckBoxElement;
		}
		else if(str.compare("RadioBox")==0)
		{
			element = new crRadioElement;
		}
		else if(str.compare("RadioGroupBox")==0)
		{
			element = new crRadioGroupElement;
		}
		else if(str.compare("StaticTextBox")==0)
		{
			element = new crStaticTextBoxElement;
		}
		else if(str.compare("EditBox")==0)
		{
			element = new crEditBoxElement;
		}
		else if(str.compare("ScrollBar")==0)
		{
			element = new crScrollBarElement;
		}
		else if(str.compare("SliderBox")==0)
		{
			element = new crSliderElement;
		}
		else if(str.compare("ImageBox")==0)
		{
			element = new crImageBoxElement;
		}
		else if(str.compare("ListBox")==0)
		{
			element = new crListBoxElement;
		}
		else if(str.compare("ComboBox")==0)
		{
			element = new crComboBoxElement;
		}
		else if(str.compare("Progress")==0)
		{
			element = new crProgressElement;
		}
		else if(str.compare("ElementNode")==0)
		{
			element = new crElementNode;
		}
		else if(str.compare("PixelTest")==0)
		{
			element = new crPixelTestElement;
		}
		else if(str.compare("ChooseColor")==0)
		{
			element = new crChooseColorElement;
		}
		else if(str.compare("KeyValueGrid")==0)
		{
			element = new crKeyValueGridElement;
		}
		else if(str.compare("Tree")==0)
		{
            element = new crTreeElement;
		}
		else if(str.compare("ColumnList")==0)
		{
			element = new crColumnListElement;
		}
		else if(str.compare("ListControl")==0)
		{
			element = new crListControlElement;
		}
		else
		{
			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElements(): unknown ElementType "<<str<<std::endl;
			cfg_script.Pop();
			continue;
		}

		element->load(cfg_script,widthScale,heightScale);

		loadElementEvents(cfg_script,element);
        loadElementHandles(cfg_script,element);

		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crImageStage::loadElements(): "<<cfg_script.GetLastError()<<std::endl;
		
        addElement(element);
	}

	//if (i == 1)
	//	CRCore::notify(CRCore::INFO) << "crImageStage::loadElements(): Warning: No Element loaded from config file" << std::endl;

}

void crImageStage::setElementImage( const std::string& image )
{ 
	int width = 0;
	int height = 0;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	switch(m_scaleWithScreen) 
	{
	case Scale_Width:
		width = screenWidth;
		break;
	case Scale_Height:
		height = screenHeight;
		break;
	case Scale_Both:
		width = screenWidth;
		height = screenHeight;
		break;
	}	
	
	m_imageName = image;
	if(m_hImage)
		DeleteObject(m_hImage);
	
	m_hImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_imageName.c_str(),IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
}

void crImageStage::setElementDownImage( const std::string& image )
{ 
	int width = 0;
	int height = 0;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	switch(m_scaleWithScreen) 
	{
	case Scale_Width:
		width = screenWidth;
		break;
	case Scale_Height:
		height = screenHeight;
		break;
	case Scale_Both:
		width = screenWidth;
		height = screenHeight;
		break;
	}

	m_downImageName = image; 
	if(m_hDownImage)
		DeleteObject(m_hDownImage);
	m_hDownImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_downImageName.c_str(),IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
}

void crImageStage::setElementMaskImage( const std::string& image )
{ 
	int width = 0;
	int height = 0;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	switch(m_scaleWithScreen) 
	{
	case Scale_Width:
		width = screenWidth;
		break;
	case Scale_Height:
		height = screenHeight;
		break;
	case Scale_Both:
		width = screenWidth;
		height = screenHeight;
		break;
	}

	m_maskImageName = image; 
	if(m_hMaskImage)
		DeleteObject(m_hMaskImage);
	m_hMaskImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_maskImageName.c_str(),IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
}

void crImageStage::setElementDisableImage( const std::string& image )
{ 
	int width = 0;
	int height = 0;
	//DEVMODE dm;
	//memset(&dm,0,sizeof(dm));
	//dm.dmSize = sizeof(dm);
	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	unsigned int screenWidth = crDisplaySettings::instance()->getScreenWidth();//dm.dmPelsWidth;
	unsigned int screenHeight = crDisplaySettings::instance()->getScreenHeight();//dm.dmPelsHeight;
	switch(m_scaleWithScreen) 
	{
	case Scale_Width:
		width = screenWidth;
		break;
	case Scale_Height:
		height = screenHeight;
		break;
	case Scale_Both:
		width = screenWidth;
		height = screenHeight;
		break;
	}

	m_disableImageName = image; 
	if(m_hDisableImage)
		DeleteObject(m_hDisableImage);
	m_hDisableImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_disableImageName.c_str(),IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
}

void crImageStage::correctWinRect( int &x, int &y, unsigned int &width, unsigned int &height )
{//-1表示居中，-2表示靠右边或下边，0表示靠左边或者上面
	m_rect = m_rect_copy;
	int iwidth = width;
	int iheight = height;
	if(m_rect[0]==-1)
		m_rect[0] = (iwidth-m_rect[2])*0.5f;
	else if(m_rect[0]==-2) 
		m_rect[0] = iwidth-m_rect[2];
	else if(m_rect[0]<0)
		m_rect[0] = iwidth+m_rect[0];
    //m_rect[0] += x;

	int h = 0;
	if(!crDisplaySettings::instance()->getFullScreen())
	{
		RECT   Rect; 
		HWND   hWnd = FindWindow("Shell_TrayWnd", NULL);
		if(GetWindowRect(hWnd,&Rect))   
		{ 
			h = Rect.bottom-Rect.top;//   就是任务栏的高度 
			if(h >= crDisplaySettings::instance()->getScreenHeight())
				h = 0;
		}
		//h = GetSystemMetrics(SM_CYSIZE);
	}
	if(m_rect[1]==-1) 
		m_rect[1] = (iheight-m_rect[3]-h)*0.5f;
	else if(m_rect[1]==-2) 
		m_rect[1] = iheight-m_rect[3];
	else if(m_rect[1]<0)
		m_rect[1] = iheight+m_rect[1];
	//m_rect[1] += y;
	
	if(m_rect[0]<0)
		m_rect[0] = 0;
	if(m_rect[1]<0)
		m_rect[1] = 0;

	x = m_rect[0];
	y = m_rect[1];
	width = m_rect[2];
	height = m_rect[3];
}

void crImageStage::setPos(float x, float y)
{
	m_rect[0] = x;
	m_rect[1] = y;
	m_rect_copy[0] = x;
	m_rect_copy[1] = y;
}
void crImageStage::setRect(float x,float y,float wid,float hit)
{
	crElement::setRect(x,y,wid,hit);
	m_rect_copy = m_rect;
}

void crImageStage::setRect(const CRCore::crVector4i &rect)
{
	crElement::setRect(rect);
	m_rect_copy = m_rect;
}
void crImageStage::moveWindow(const CRCore::crVector4i &rect)
{
    m_rect = rect;
}

bool crImageStage::needScreenImage()
{
	return false;
	//return (m_needParentWindow && (m_canDrag || m_scaleWithScreen != Scale_Both));
}

void crImageStage::attachWindow(crWindow* window)
{
    m_window = window;
}

crWindow *crImageStage::getWindow()
{
	return m_window.get();
}

void crImageStage::showWindow(bool show,bool async)
{ 
	m_show = show;
	if(!m_show)
		setIsOnShow(false);
	if(m_window.valid()) m_window->showWindow(show,async);
}
//void crImageStage::threadWait()
//{
//    //m_stageCondition.wait(&m_stageMutex,INFINITE);
//}
//void crImageStage::wakeUpThread()
//{
//	//m_stageCondition.signal();
//}
//static CRCore::crMutex s_drawStageMutex;
int crImageStage::drawStage(Display dc)
{
	//CRCore::ScopedLock<CRCore::crMutex> lock(s_drawStageMutex);
	m_needRedraw = false;
	preDraw();
	return drawStageCopy(dc);
	//switch(m_drawMode)
	//{
	//case Draw_Copy:
	//	return drawStageCopy(dc);
	//case Draw_Mask:
	//	return drawStageMask(dc);
	//case Draw_Transparent:
	//	return drawStageTransparent(dc);
	//}
    //return 1;
}

int crImageStage::drawStageCopy(Display dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc;
    HBITMAP bufbmp;
	bufdc=CreateCompatibleDC(dc);
	mdc=CreateCompatibleDC(dc);
    bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);
	//RECT rect;
	//rect.left = 0;
	//rect.top = 0;
	//rect.right = m_rect[2];
	//rect.bottom = m_rect[3];
	//FillRect(bufdc,&rect,RGB(0,0,0));
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}
	if(m_hDownImage)
	{
		SelectObject(mdc,m_hDownImage);
	}
	////
	////	Now draw the buttons
	////
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		if(!(*itr)->getEnable()&&m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			const CRCore::crVector4i &rect = (*itr)->getRect();
			BitBlt(bufdc,rect[0],rect[1],rect[2],rect[3],mdc,rect[0],rect[1],SRCCOPY);
			SelectObject(mdc,m_hDownImage);
			continue;
		}
        if(!(*itr)->drawElement(bufdc,mdc)&&m_hDownImage)
		{
			const CRCore::crVector4i &rect = (*itr)->getRect();
			BitBlt(bufdc,rect[0],rect[1],rect[2],rect[3],mdc,rect[0],rect[1],SRCCOPY);
		}
	}

    BitBlt(dc,0,0,m_rect[2],m_rect[3],bufdc,vscrollValue,hscrollValue,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
    DeleteObject(bufbmp);
	return 1;
}

int crImageStage::drawStageMask(Display dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc,maskbufdc;
	HBITMAP bufbmp,maskbufbmp;
    
	bufdc = CreateCompatibleDC(dc);
	mdc = CreateCompatibleDC(dc);
	maskbufdc = CreateCompatibleDC(dc);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
    maskbufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);

	SelectObject(bufdc,bufbmp);
	SelectObject(maskbufdc,maskbufbmp);
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}
	if(m_hDownImage)
	{
		SelectObject(mdc,m_hDownImage);
	}
	//
	//	Now draw the buttons
	//
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		if(!(*itr)->getEnable()&&m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			const CRCore::crVector4i &rect = (*itr)->getRect();
			BitBlt(bufdc,rect[0],rect[1],rect[2],rect[3],mdc,rect[0],rect[1],SRCCOPY);
			SelectObject(mdc,m_hDownImage);
			continue;
		}
		if(!(*itr)->drawElement(bufdc,mdc)&&m_hDownImage)
		{
			const CRCore::crVector4i &rect = (*itr)->getRect();
			BitBlt(bufdc,rect[0],rect[1],rect[2],rect[3],mdc,rect[0],rect[1],SRCCOPY);
		}
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(maskbufdc,0,0,m_rect[2],m_rect[3],cr2DStageManager::getInstance()->getScreenMemorryDC(),m_rect[0],m_rect[1],SRCCOPY);
		BitBlt(maskbufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(maskbufdc,0,0,m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
		BitBlt(dc,0,0,m_rect[2],m_rect[3],maskbufdc,vscrollValue,hscrollValue,SRCCOPY);
	}
	else
	    BitBlt(dc,0,0,m_rect[2],m_rect[3],bufdc,vscrollValue,hscrollValue,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
    DeleteDC(maskbufdc);
    DeleteObject(bufbmp);
	DeleteObject(maskbufbmp);
	return 1;
}

int crImageStage::drawStageTransparent(Display dc)
{
	return 1;
}

void crImageStage::initWindow()
{
	if(!m_elementVec.empty())
	{
		ElementVec::iterator itr;
		for( itr = m_elementVec.begin(); 
			itr != m_elementVec.end();
			++itr )
		{
			(*itr)->initWindow();
		}

		crElement *element = getElement(m_defaultFocusElementName);
		if(!element)
		{
			itr = m_elementVec.begin();
			element = itr->get();
			m_defaultFocusElementName = element->getName();
		}
		setFocusElement(element);
	}

	m_hScrollBar = dynamic_cast<crScrollBarElement *>(getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(getElement(m_vScrollBarName));
	crElement::initWindow();
}
void crImageStage::setNeedReDraw()
{
	m_needRedraw = true;
}
bool crImageStage::getNeedReDraw()
{
	if(m_focusElement.valid() && dynamic_cast<crEditBoxElement *>(m_focusElement.get()))
	{
		return true;
	}
	return m_needRedraw;
}
void crImageStage::updateWindow()
{
	crElement::updateData();
	ElementVec::iterator itr;
	for( itr = m_elementVec.begin(); 
		itr != m_elementVec.end();
		++itr )
	{
		(*itr)->updateData();
	}
}
void crImageStage::frameWindow(float dt)
{
	crElement::frame(dt);
	ElementVec::iterator itr;
	for( itr = m_elementVec.begin(); 
		itr != m_elementVec.end();
		++itr )
	{
		(*itr)->frame(dt);
	}
}

void crImageStage::destroyWindow()
{
	if(m_focusElement.valid())
		m_focusElement->lostFocus();

	crElement::destroyWindow();
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		(*itr)->destroyWindow();
	}
}

void crImageStage::getCurrentMousePosition(int &x, int &y)
{
    x = m_currentMousePosx;
	y = m_currentMousePosy;
}

void crImageStage::getLastMousePosition(int &x, int &y)
{
	x = m_lastMousePosx;
	y = m_lastMousePosy;
}

bool crImageStage::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_lastMouseButtonMsg = m_currentMouseButtonMsg;
	m_lastMousePosx = m_currentMousePosx;
	m_lastMousePosy = m_currentMousePosy;

	m_currentMouseButtonMsg = mouseButtonMsg;
	m_currentMousePosx = mx;
	m_currentMousePosy = my;

	if(getCapture())
	{
		crElement::testButtons(mx,my,mouseButtonMsg);
		if(m_canDrag && mouseButtonMsg == WM_MOUSEDRAG)
		{
			int lastmx,lastmy;
			getLastMousePosition(lastmx,lastmy);
			int x = (mx-lastmx);
			int y = (my-lastmy);
			moveOffset(x,y);
			m_currentMousePosx -= x;
			m_currentMousePosy -= y;
			MoveWindow(m_window->getHandle(),m_rect[0],m_rect[1],m_rect[2],m_rect[3],true);
			//CRCore::notify(CRCore::WARN)<<"mouseButtonMsg = "<<mouseButtonMsg<<std::endl;
		}
		return true;
	}

	bool test = false;
	if(m_focusElement.valid()&&m_focusElement->getCapture() && m_focusElement->getEnable())
	{
        m_focusElement->onCaptureMouse(mx,my,mouseButtonMsg);
		test = true;
	}
	else
	{
		for( ElementVec::iterator itr = m_elementVec.begin();
			itr != m_elementVec.end();
			++itr )
		{
			if((*itr)->getEnable())
			{
				if(m_focusElement == itr->get() && m_focusElement->getCapture())
				{
					continue;
				}
				test |= (*itr)->testButtons(mx,my,mouseButtonMsg);
				//if(test) return true;
			}
		}
	}
	if(test)
	{
		releaseCapture();
		return true;
	}

	m_isMouseOnElement = pressed(mx,my);
	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crImageStage::setCapture()
{
	crElement::setCapture();
	SetCapture(m_window->getHandle());
	//char buf[32];
	//sprintf(buf,"CRE_Captured=%d\0",(int)m_hwnd);
	//_putenv(buf);
}

void crImageStage::releaseCapture()
{
    crElement::releaseCapture();
	ReleaseCapture();
    //_putenv( "CRE_Captured=0" );
}

bool crImageStage::pressed(int x,int y)
{
	if(x>=0 && x<=0+m_rect[2])
	{
		if(y>=0 && y<=0+m_rect[3])
		{
			if(!m_isMouseOnElement)
				mouseOn();
			return true;
		}
	}
	return false;
}

bool crImageStage::inputKey(int key)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crImageStage::inputKey key = "<<key<<std::endl;
	if(m_focusElement.valid() && m_focusElement->getEnable() && m_focusElement->isFocus())
        m_focusElement->inputKey(key);
	else crElement::inputKey(key);

	return true;
}

bool crImageStage::inputChar(wchar_t c)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crImageStage::inputChar c = "<<c<<std::endl;
	if(m_focusElement.valid() && m_focusElement->getEnable() && m_focusElement->isFocus())
		m_focusElement->inputChar(c);
	else crElement::inputChar(c);

	return true;
}

void crImageStage::setFocusElement(crElement *element)
{ 
	if(element==this) return;
	if(!element || !element->getEnable() || !element->getCanFocus()) return;
		//setFocusElement(getElement(m_defaultFocusElementName));
	//if(m_focusElement == element)
	//{
	//	if(m_focusElement->getCanCapture())
	//		m_focusElement->setCapture();
	//	return;
	//}
	if(m_focusElement.valid() && m_focusElement != element)
	    lostFocusElement(m_focusElement.get());

	element->getFocus();
	m_focusElement = element;

	if(m_focusElement->getCanCapture())
		m_focusElement->setCapture();
}

void crImageStage::lostFocusElement(crElement *element)
{
	if(element==this) return;
	//if(m_focusElement.valid() && m_focusElement == element)
	//	m_focusElement->lostFocus();
	element->lostFocus();
	m_focusElement = NULL;
}

crElement* crImageStage::getFocusElement()
{
    return m_focusElement.get();
}

crElement *crImageStage::getElement(const std::string &nameid)
{
	for( ElementVec::iterator itr = m_elementVec.begin();
		 itr != m_elementVec.end();
		 ++itr )
	{
		if(nameid.compare((*itr)->getName()) == 0)
		{
			return itr->get();
		}
	}
	return 0L;
}

void crImageStage::addElement(crElement *element)
{
    element->setParentStage(this);
	m_elementVec.push_back(element);
}

void crImageStage::backup()
{
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		(*itr)->backup();
	}
}

void crImageStage::resumeBackupData()
{
	for( ElementVec::iterator itr = m_elementVec.begin();
		itr != m_elementVec.end();
		++itr )
	{
		(*itr)->resumeBackupData();
	}
}
void crImageStage::setIsOnShow(bool bln)
{
	m_isOnShow = bln;
}
bool crImageStage::getIsOnShow()
{
	return m_isOnShow;
}
//void crImageStage::traverse(CRCore::crNodeVisitor& nv)
//{
//	if( nv.getVisitorType()==CRCore::crNodeVisitor::AI_UPDATE_VISITOR ||
//		nv.getVisitorType()==CRCore::crNodeVisitor::UPDATE_VISITOR )
//	{
//		m_currentVisitor = &nv;
//
//		relayEvent();
//		if(m_childParam>=0&&m_childParam<m_childArray.size())
//			m_childArray[m_childParam]->accept(nv);
//		else
//		{
//			crGroup::traverse(nv);
//		}
//	}
//	else
//	{
//		if(nv.getTraversalMode() == CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN)
//		{
//			crGroup::traverse(nv);
//		}
//	}
//}