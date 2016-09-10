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
#include <CRGUI/crElement.h>
#include <CRGUI/crImageStage.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>
#include <CRCore/thread/crScopedLock.h>
using namespace CRGUI;
using namespace CRCore;

#define TextRectOffset 2.0f

crElement::crElement()
{
	//m_hscrollValue = m_vscrollValue = 0.0f;

	m_hImage = NULL;
	m_hMouseOnImage = NULL;
	m_hDownImage = NULL;
	m_hMaskImage = NULL;
    m_hDisableImage = NULL;

	m_isMouseOnElement = false;
	m_focus = false;
	m_canDrag = 0;//1：可拖动，0：不可拖动
	
	m_parentStage = NULL;
	m_parentElement = NULL;

	m_drawMode = Draw_Copy;

	m_enable = true;
	m_show = true;
	m_canCapture = false;
	m_capture = false;
	m_canFocus = true;
//	m_needRedraw = true;
}

crElement::crElement(const crElement& element):
 //   m_hscrollValue(0.0f),
	//m_vscrollValue(0.0f),
    m_hImage(element.m_hImage),
	m_hMouseOnImage(element.m_hMouseOnImage),
    m_hDownImage(element.m_hDownImage),
	m_hMaskImage(element.m_hMaskImage),
	m_hDisableImage(element.m_hDisableImage),
	m_isMouseOnElement(false),
	m_canFocus(element.m_canFocus),
	m_focus(false),
	m_canDrag(element.m_canDrag),
	m_parentStage(element.m_parentStage),
	m_drawMode(element.m_drawMode),
	m_enable(element.m_enable),
	m_show(element.m_show),
	m_canCapture(element.m_canCapture),
	m_capture(false),
	m_rect(element.m_rect),
	m_imageName(element.m_imageName),
	m_downImageName(element.m_downImageName),
    m_maskImageName(element.m_maskImageName),
    m_disableImageName(element.m_disableImageName),
	m_parentElement(element.m_parentElement),
	m_parentElementName(element.m_parentElementName)
{
    m_name = "复件" + element.m_name;
	m_mouseMoveEventCallbacks = element.m_mouseMoveEventCallbacks;
    m_lButtonDownEventCallbacks = element.m_lButtonDownEventCallbacks;
	m_rButtonDownEventCallbacks = element.m_rButtonDownEventCallbacks;
	m_mButtonDownEventCallbacks = element.m_mButtonDownEventCallbacks;
	m_lButtonUpEventCallbacks = element.m_lButtonUpEventCallbacks;
	m_rButtonUpEventCallbacks = element.m_rButtonUpEventCallbacks;
	m_mButtonUpEventCallbacks = element.m_mButtonUpEventCallbacks;
	m_lButtonDblClkEventCallbacks = element.m_lButtonDblClkEventCallbacks;
	m_rButtonDblClkEventCallbacks = element.m_rButtonDblClkEventCallbacks;
	m_mButtonDblClkEventCallbacks = element.m_mButtonDblClkEventCallbacks;
	m_mouseWheelEventCallbacks = element.m_mouseWheelEventCallbacks;
	m_mouseOnEventCallbacks = element.m_mouseOnEventCallbacks;
	m_getFocusEventCallbacks = element.m_getFocusEventCallbacks;
	m_lostFocusEventCallbacks = element.m_lostFocusEventCallbacks;
	m_initWindowEventCallbacks = element.m_initWindowEventCallbacks;
	m_destroyWindowEventCallbacks = element.m_destroyWindowEventCallbacks;
	m_inputKeyEventCallbacks = element.m_inputKeyEventCallbacks;
	m_inputCharEventCallbacks = element.m_inputCharEventCallbacks;
	m_updateDataEventCallbacks = element.m_updateDataEventCallbacks;
	m_preDrawEventCallbacks = element.m_preDrawEventCallbacks;
}

crElement::~crElement()
{
	if(m_hImage)
		DeleteObject(m_hImage);
	if(m_hMouseOnImage)
		DeleteObject(m_hMouseOnImage);
	if(m_hDownImage)
		DeleteObject(m_hDownImage);
	if(m_hMaskImage)
		DeleteObject(m_hMaskImage);
	if(m_hDisableImage)
		DeleteObject(m_hDisableImage);
}

void crElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
    int int1;
    std::vector<float> v_i;

	if(cfg_script.Get("ElementName", str))
		setName(str);
	if(cfg_script.Get("ParentElementName", str))
	    setParentElementName(str);
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

	if (cfg_script.Get("DrawMode", str))
	{
		if(str.compare("Copy")==0)
			setDrawMode(crElement::Draw_Copy);
		else if(str.compare("Mask")==0)
			setDrawMode(crElement::Draw_Mask);
		else if(str.compare("Transparent")==0)
			setDrawMode(crElement::Draw_Transparent);
	}
	
	if (cfg_script.Get("ElementRect", v_i)) 
	{
		if (v_i.size() != 4)
			throw std::runtime_error("crImageStage::loadElements(): " + cfg_script.CurrentScope() + "Rect should be a vector with 4 elements");

		setRect(v_i[0]*widthScale, v_i[1]*heightScale, (v_i[2]-v_i[0])*widthScale, (v_i[3]-v_i[1])*heightScale);
	}

	//if (cfg_script.Get("Image", str))
	//	setElementImage(str);
	//if (cfg_script.Get("DownImage", str))
	//	setElementDownImage(str);
	//if (cfg_script.Get("MaskImage", str))
	//	setElementMaskImage(str);

	crVector2i imageSize;
	crVector2i elementPos;
	if (cfg_script.Push("Image")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("MouseOnImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementMouseOnImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("DownImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementDownImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("MaskImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementMaskImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("DisableImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementDisableImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crElement::resize(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	std::vector<float> v_i;

	if (cfg_script.Get("ElementRect", v_i)) 
	{
		if (v_i.size() != 4)
			throw std::runtime_error("crImageStage::loadElements(): " + cfg_script.CurrentScope() + "Rect should be a vector with 4 elements");

		setRect(v_i[0]*widthScale, v_i[1]*heightScale, (v_i[2]-v_i[0])*widthScale, (v_i[3]-v_i[1])*heightScale);
	}

	crVector2i imageSize;
	crVector2i elementPos;
	if (cfg_script.Push("Image")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("DownImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementDownImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("MaskImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementMaskImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
	if (cfg_script.Push("DisableImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementDisableImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crElement::setParentElementName(const std::string &elementName)
{
    m_parentElementName = elementName;
}

std::string &crElement::getParentElementName()
{
    return m_parentElementName;
}

void crElement::setCanFocus(bool canFocus)
{
    m_canFocus = canFocus;
}

void crElement::setCanDrag(bool canDrag)
{
	m_canDrag = canDrag;
	if(m_canDrag)
        setCanCapture(true);
}

void crElement::setCanCapture(bool canCapture)
{
    m_canCapture = canCapture;
}

void crElement::setRect(float x,float y,float wid,float hit)
{
    m_rect.set(x,y,wid,hit);
}

void crElement::setRect(const CRCore::crVector4i &rect)
{
    m_rect = rect;
}

const CRCore::crVector4i &crElement::getRect()
{
    return m_rect;
}

int crElement::getElementRight()
{
    return m_rect[0] + m_rect[2];
}

int crElement::getElementBottom()
{
    return m_rect[1] + m_rect[3];
}

bool crElement::pressed(int x,int y)
{
	if(x>=m_rect[0] && x<=m_rect[0]+m_rect[2])
	{
		if(y>=m_rect[1] && y<=m_rect[1]+m_rect[3])
		{
			if(!m_isMouseOnElement)
				mouseOn();
			return true;
		}
	}
	return false;
}

void crElement::setEnable(bool enable) 
{
	m_enable = enable; 
	if(!m_enable)
	{
		m_isMouseOnElement = false;
		if(m_focus && m_parentStage)
            m_parentStage->setFocusElement(NULL);
	}
}

void crElement::setShow(bool show)
{
	m_show = show; 
	if(!m_show)
	{
		m_isMouseOnElement = false;
		if(m_focus && m_parentStage)
			m_parentStage->setFocusElement(NULL);
	}
}

void crElement::setCapture()
{
    m_capture = true;
}

void crElement::releaseCapture()
{
    m_capture = false;
}

bool crElement::onCaptureMouse(int mx, int my, int mouseButtonMsg)
{
    testButtons(mx,my,mouseButtonMsg);
	if(m_canDrag && mouseButtonMsg == WM_MOUSEMOVE && m_parentStage)
	{
		int lastmx,lastmy;
		m_parentStage->getLastMousePosition(lastmx,lastmy);
		moveOffset(mx-lastmx,my-lastmy);
	}
	return true;
}

void crElement::setPos(float x, float y)
{
	m_rect[0] = x;
	m_rect[1] = y;
}

void crElement::moveOffset(int offsetx, int offsety)
{
	m_rect[0] += offsetx;
	m_rect[1] += offsety;
}

bool crElement::inputKey(int key)
{
	if( m_inputKeyEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_inputKeyEventCallbacks.begin(); itr != m_inputKeyEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this,key);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_InputKey,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-3,&key);
		excHandle(handle);
	}
	return true;
}

bool crElement::inputChar(wchar_t c)
{
	if( m_inputCharEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_inputCharEventCallbacks.begin(); itr != m_inputCharEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this,c);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_InputChar,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-4,&c);
		excHandle(handle);
	}
	return true;
}

bool crElement::testButtons(int mx, int my, int mouseButtonMsg )
{
    if(m_isMouseOnElement)
	{
		switch(mouseButtonMsg) 
		{
		case WM_MOUSEMOVE:
			mouseMove(mx,my);
			break;
		case WM_LBUTTONDOWN:
			if(m_parentStage) 
				m_parentStage->setFocusElement(this);
			else if(dynamic_cast<crImageStage *>(this))
			{
				if(getCanCapture()) setCapture();
			}
			lButtonDown(mx,my);
			break;
		case WM_LBUTTONUP:
		    if(dynamic_cast<crImageStage *>(this))
			{
				if(getCanCapture()) releaseCapture();
			}
			lButtonUp(mx,my);
			break;
		case WM_LBUTTONDBLCLK:
			lButtonDblClk(mx,my);
			break;
		case WM_RBUTTONDOWN:
			rButtonDown(mx,my);
			break;
		case WM_RBUTTONUP:
			rButtonUp(mx,my);
			break;
		case WM_RBUTTONDBLCLK:
			rButtonDblClk(mx,my);
			break;
		case WM_MBUTTONDOWN:
			mButtonDown(mx,my);
			break;
		case WM_MBUTTONUP:
			mButtonUp(mx,my);
			break;
		case WM_MBUTTONDBLCLK:
			mButtonDblClk(mx,my);
			break;
		case WM_MOUSEDRAG:
			mouseDrag(mx,my);
			break;
		case WM_MOUSEWHEEL:
			mouseWheel(mx,my);
			break;
		}
	}
	if(m_parentStage && !m_parentElement) 
	{//有m_parentElement的由m_parentElement触发其lostFocusElement，解决Combobox的问题
		if(getCapture())
		{
		    if(mouseButtonMsg == WM_LBUTTONUP)
				m_parentStage->lostFocusElement(this);
		}
	}

	updateParentElement(this);
	return m_isMouseOnElement;
}


int crElement::drawElement(Display dc, Display down_dc)
{
	if(!getShow()) return 1;
	preDraw();
	switch(m_drawMode)
	{
	case Draw_Copy:
		return drawStageCopy(dc, down_dc);
	case Draw_Mask:
		return drawStageMask(dc, down_dc);
	case Draw_Transparent:
		return drawStageTransparent(dc, down_dc);
	}
	return 1;
}

void crElement::initWindow()
{
	if(m_parentStage && !m_parentElementName.empty())
	    m_parentElement = m_parentStage->getElement(m_parentElementName);

	if( m_initWindowEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_initWindowEventCallbacks.begin(); itr != m_initWindowEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_InitWindow,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}

void crElement::destroyWindow()
{
	if( m_destroyWindowEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_destroyWindowEventCallbacks.begin(); itr != m_destroyWindowEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
		    if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_DestroyWindow,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}

void crElement::mouseOn()
{
	if( m_mouseOnEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mouseOnEventCallbacks.begin(); itr != m_mouseOnEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MouseOn,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}

void crElement::getFocus()
{
	if(m_focus) return;
	m_focus = true;
	if( m_getFocusEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_getFocusEventCallbacks.begin(); itr != m_getFocusEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_GetFocus,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}

void crElement::lostFocus()
{
	if(getCapture())
		releaseCapture();

	m_focus = false;
	if( m_lostFocusEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_lostFocusEventCallbacks.begin(); itr != m_lostFocusEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_LostFocus,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}

	updateData();
}

void crElement::setFocus()
{ 
	if(m_canFocus && m_parentStage) m_parentStage->setFocusElement(this);
}

void crElement::updateData()
{
	//updateParentElement(this);

	if( m_updateDataEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_updateDataEventCallbacks.begin(); itr != m_updateDataEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_UpdateData,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}
void crElement::frame(float dt)
{
	crHandle *handle = getHandle(MAKEINT64(WCH_UI_Frame,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-3,&dt);
		excHandle(handle);
	}
}
void crElement::preDraw()
{
	if( m_preDrawEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_preDrawEventCallbacks.begin(); itr != m_preDrawEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_PreDraw,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		excHandle(handle);
	}
}

void crElement::mouseMove(int mx, int my)
{
	if( m_mouseMoveEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mouseMoveEventCallbacks.begin(); itr != m_mouseMoveEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MouseMove,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::lButtonDown(int mx, int my)
{
	if( m_lButtonDownEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_lButtonDownEventCallbacks.begin(); itr != m_lButtonDownEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_LBtnDown,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::rButtonDown(int mx, int my)
{
	if( m_rButtonDownEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_rButtonDownEventCallbacks.begin(); itr != m_rButtonDownEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_RBtnDown,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::mButtonDown(int mx, int my)
{
	if( m_mButtonDownEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mButtonDownEventCallbacks.begin(); itr != m_mButtonDownEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MBtnDown,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::lButtonUp(int mx, int my)
{
	if( m_lButtonUpEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_lButtonUpEventCallbacks.begin(); itr != m_lButtonUpEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_LBtnUp,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::rButtonUp(int mx, int my)
{
	if( m_rButtonUpEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_rButtonUpEventCallbacks.begin(); itr != m_rButtonUpEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_RBtnUp,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::mButtonUp(int mx, int my)
{
	if( m_mButtonUpEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mButtonUpEventCallbacks.begin(); itr != m_mButtonUpEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MBtnUp,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::lButtonDblClk(int mx, int my)
{
	if( m_lButtonDblClkEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_lButtonDblClkEventCallbacks.begin(); itr != m_lButtonDblClkEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_LBtnDblClk,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::rButtonDblClk(int mx, int my)
{
	if( m_rButtonDblClkEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_rButtonDblClkEventCallbacks.begin(); itr != m_rButtonDblClkEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_RBtnDblClk,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::mButtonDblClk(int mx, int my)
{
	if( m_mButtonDblClkEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mButtonDblClkEventCallbacks.begin(); itr != m_mButtonDblClkEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MBtnDblClk,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}

void crElement::mouseWheel(int mx, int my)
{
	if( m_mouseWheelEventCallbacks.size() > 0 )
	{
		for( CallbackVec::iterator itr = m_mouseWheelEventCallbacks.begin(); itr != m_mouseWheelEventCallbacks.end(); itr++ )
		{
			(*itr)->initCallbackTask();
			if(dynamic_cast<Callback *>(itr->get()))
				(*(dynamic_cast<Callback *>(itr->get())))(*this);
			else (*(itr->get()))();
			if(!(*itr)->continueCallbackTask()) break;
		}
	}

	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MouseWheel,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}
void crElement::mouseDrag(int mx, int my)
{
	crHandle *handle = getHandle(MAKEINT64(WCH_UI_MouseDrag,NULL));
	if(handle)
	{
		handle->inputParam(0,this);
		handle->inputParam(-1,&mx);
		handle->inputParam(-2,&my);
		excHandle(handle);
	}
}
crElement::BMPCacheMap crElement::s_bmpCacheMap;
void crElement::clearBmpCache()
{
	for( BMPCacheMap::iterator itr = s_bmpCacheMap.begin();
		 itr != s_bmpCacheMap.end();
		 ++itr )
	{
        DeleteObject(itr->second);
	}
    s_bmpCacheMap.clear();
}

HBITMAP crElement::getOrLoadBmp(const std::string &fileName,const crVector2i& imageSize)
{
    BMPCacheMap::iterator itr = s_bmpCacheMap.find(fileName);
	if(itr == s_bmpCacheMap.end())
	{
        HBITMAP hImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),fileName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
		s_bmpCacheMap[fileName] = hImage;
		return hImage;
	}
	return itr->second;
}
void crElement::setElementImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{ 
	m_imageName = image;
	if(m_hImage)
		DeleteObject(m_hImage);
	
	//CRCore::notify(CRCore::ALWAYS)<<"setElementImage "<<m_imageName.c_str()<<imageSize<<elementPos<<std::endl;
    //HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_imageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	HBITMAP hLoadedImage = getOrLoadBmp(m_imageName,imageSize);
	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);

	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC = GetDC(desktopWnd);
	m_hImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);//注：如果是内存DC，CreateCompatibleBitmap返回的img在绘图时候会出错
	SelectObject(dcDest,m_hImage);
	SelectObject(dcSrc,hLoadedImage);
	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);
	
	//DeleteObject(hLoadedImage);
	DeleteDC(dcSrc);
	DeleteDC(dcDest);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}
void crElement::setElementMouseOnImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{ 
	m_mouseOnImageName = image;
	if(m_hMouseOnImage)
		DeleteObject(m_hMouseOnImage);

	//CRCore::notify(CRCore::ALWAYS)<<"setElementImage "<<m_imageName.c_str()<<imageSize<<elementPos<<std::endl;
	//HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_imageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	HBITMAP hLoadedImage = getOrLoadBmp(m_mouseOnImageName,imageSize);
	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);

	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC = GetDC(desktopWnd);
	m_hMouseOnImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);//注：如果是内存DC，CreateCompatibleBitmap返回的img在绘图时候会出错
	SelectObject(dcDest,m_hMouseOnImage);
	SelectObject(dcSrc,hLoadedImage);
	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);

	//DeleteObject(hLoadedImage);
	DeleteDC(dcSrc);
	DeleteDC(dcDest);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}
void crElement::setElementDownImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{
	m_downImageName = image; 
	if(m_hDownImage)
		DeleteObject(m_hDownImage);
	//HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_downImageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	//CRCore::notify(CRCore::ALWAYS)<<"setElementDownImage "<<m_downImageName.c_str()<<std::endl;
	HBITMAP hLoadedImage = getOrLoadBmp(m_downImageName,imageSize);
	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);
	
	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC = GetDC(desktopWnd);
	m_hDownImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);
	SelectObject(dcDest,m_hDownImage);
	SelectObject(dcSrc,hLoadedImage);

	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);

    //DeleteObject(hLoadedImage);
	DeleteDC(dcDest);
	DeleteDC(dcSrc);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}

void crElement::setElementMaskImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{ 
	m_maskImageName = image; 
	if(m_hMaskImage)
		DeleteObject(m_hMaskImage);
	//HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_maskImageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	HBITMAP hLoadedImage = getOrLoadBmp(m_maskImageName,imageSize);

	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);

	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC = GetDC(desktopWnd);
	m_hMaskImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);
	SelectObject(dcDest,m_hMaskImage);
	SelectObject(dcSrc,hLoadedImage);

	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);

	//DeleteObject(hLoadedImage);
    DeleteDC(dcDest);
	DeleteDC(dcSrc);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}

void crElement::setElementDisableImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{
	m_disableImageName = image; 
	if(m_hDisableImage)
		DeleteObject(m_hDisableImage);
	//HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_disableImageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	HBITMAP hLoadedImage = getOrLoadBmp(m_disableImageName,imageSize);

	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);

	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC = GetDC(desktopWnd);
	m_hDisableImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);
	SelectObject(dcDest,m_hDisableImage);
	SelectObject(dcSrc,hLoadedImage);

	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);

	//DeleteObject(hLoadedImage);
	DeleteDC(dcDest);
	DeleteDC(dcSrc);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}

//////////////////////////////////////////////////////////////////////////
//
//crButtonElement
//
//////////////////////////////////////////////////////////////////////////
crButtonElement::crButtonElement(const crButtonElement& element):
    crElement(element)
{

}

void crButtonElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::load(cfg_script, widthScale, heightScale);
}

bool crButtonElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

int crButtonElement::drawStageCopy(Display dc, Display down_dc)
{
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(dc);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(/*m_focus||*/m_isMouseOnElement)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crButtonElement::drawStageMask(Display dc, Display down_dc)
{
	HDC		bufdc,mdc;
    HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(/*m_focus||*/m_isMouseOnElement)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crCheckBoxElement
//
//////////////////////////////////////////////////////////////////////////
crCheckBoxElement::crCheckBoxElement(const crCheckBoxElement& element):
    crElement(element),
    m_select(element.m_select)
{
}

void crCheckBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	int int1;
	if(cfg_script.Get("Select", int1))
		setSelect(int1);
	crElement::load(cfg_script,widthScale,heightScale);
}

bool crCheckBoxElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	if(pressed(mx,my))
	{
		m_isMouseOnElement = true;
		if(mouseButtonMsg == WM_LBUTTONDOWN)
		{
			setSelect(!m_select);
		}
	}
	else m_isMouseOnElement = false;

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crCheckBoxElement::setSelect( bool select )
{
    m_select = select;
}

void crCheckBoxElement::select(bool select)
{
	if(m_select != select)
	{
		m_select = select;
		updateData();
	}
}

int crCheckBoxElement::drawStageCopy(Display dc, Display down_dc)
{
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_select)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crCheckBoxElement::drawStageMask(Display dc, Display down_dc)
{
	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_select)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crRadioElement
//
//////////////////////////////////////////////////////////////////////////
crRadioElement::crRadioElement(const crRadioElement& element):
crElement(element),
m_select(element.m_select)
{
}

void crRadioElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::load(cfg_script,widthScale,heightScale);
}

bool crRadioElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	if(pressed(mx,my))
	{
		m_isMouseOnElement = true;
		if(mouseButtonMsg == WM_LBUTTONDOWN)
		{
			setSelect(true);
		}
	}
	else m_isMouseOnElement = false;

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crRadioElement::setSelect( bool select )
{
	if(m_select != select)
	{
		m_select = select;
		updateData();
	}
}

int crRadioElement::drawStageCopy(Display dc, Display down_dc)
{
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_select)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crRadioElement::drawStageMask(Display dc, Display down_dc)
{
	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_select)
	{
		if(m_hDownImage)
		{
			SelectObject(mdc,m_hDownImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crRadioGroupElement
//
//////////////////////////////////////////////////////////////////////////
crRadioGroupElement::crRadioGroupElement():
  m_initSelectedIndex(0),
  m_currentSelect(0)
{
}

crRadioGroupElement::crRadioGroupElement(const crRadioGroupElement& element):
crElement(element),
m_initSelectedIndex(element.m_initSelectedIndex),
m_currentSelect(element.m_currentSelect)
{
}

crRadioGroupElement::~crRadioGroupElement()
{
	//for( RadioGroup::iterator itr = m_radioGroup.begin();
	//	itr != m_radioGroup.end();
	//	++itr )
	//{
	//	(*itr)->setParentElement(NULL);
	//}
}

void crRadioGroupElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	int nRadio = 1;
	std::string str;
	while(cfg_script.Get("RadioElement",str, nRadio++))
	{
		m_radioNameGroup.push_back(str);
	}
	cfg_script.Get("InitSelectedIndex",m_initSelectedIndex);
	crElement::load(cfg_script,widthScale,heightScale);
}

void crRadioGroupElement::initWindow()
{
	crRadioElement *radio;
	for( RadioNameGroup::iterator itr = m_radioNameGroup.begin();
		itr != m_radioNameGroup.end();
		++itr )
	{
		radio = dynamic_cast<crRadioElement *>(m_parentStage->getElement(*itr));
		if(radio)
			m_radioGroup.push_back(radio);
		else
			CRCore::notify(CRCore::WARN)<<"crRadioGroupElement::initWindow(): RadioElement("<<this->getName()<<") Not Find RadioElement("<<itr->c_str()<<"). StageNameID = "<<m_parentStage->getName()<<std::endl;
	}
	if(m_radioGroup.empty())
	{
		CRCore::notify(CRCore::WARN)<<"crRadioGroupElement::initWindow(): RadioElement("<<this->getName()<<") is empty. StageNameID = "<<m_parentStage->getName()<<std::endl;
	}
	else
	{
		if(m_initSelectedIndex<0 || m_initSelectedIndex>m_radioGroup.size())
		{
			m_initSelectedIndex = 0;
			CRCore::notify(CRCore::WARN)<<"crRadioGroupElement::initWindow(): RadioElement("<<this->getName()<<") InitSelectedIndex error. StageNameID = "<<m_parentStage->getName()<<std::endl;
		}
		if(m_currentSelect != m_initSelectedIndex)
		{
			m_radioGroup[m_currentSelect]->setSelect(false);
			m_currentSelect = m_initSelectedIndex;
		}
		m_radioGroup[m_currentSelect]->setSelect(true);
	}
	crElement::initWindow();
}

void crRadioGroupElement::updateParentElement(crElement *element)
{
    crRadioElement *radio = dynamic_cast<crRadioElement *>(element);
	if(radio && radio->getSelect())
	{
		int i = 0;
		int selectIndex = 0;
		for( RadioGroup::iterator itr = m_radioGroup.begin();
			 itr != m_radioGroup.end();
			 ++itr, ++i )
		{
			if(itr->get() == radio)
			{
				selectIndex = i;
				break;
			}
		}
		if(selectIndex != m_currentSelect)
		{
			m_radioGroup[m_currentSelect]->setSelect(false);
			m_currentSelect = selectIndex;
		}
	}
	crElement::updateParentElement(this);
}

void crRadioGroupElement::select(int index)
{
    if(!m_radioGroup.empty())
	{
		if(index<0) index = 0;
		if (index>m_radioGroup.size())
		{
			index = m_radioGroup.size();
		}
		if(index != m_currentSelect)
		{
			m_radioGroup[m_currentSelect]->setSelect(false);
			m_currentSelect = index;
			m_radioGroup[m_currentSelect]->setSelect(true);
		}
	}
}
int crRadioGroupElement::getSelect()
{
	return m_currentSelect;
}
crRadioGroupElement::RadioGroup &crRadioGroupElement::getRadioGroup()
{
	return m_radioGroup;
}
//////////////////////////////////////////////////////////////////////////
//
//crScrollBarElement
//
//////////////////////////////////////////////////////////////////////////
crScrollBarElement::crScrollBarElement():
  m_value(0.0),
  m_backup_value(0.0),
  m_lineValue(1.0),
  m_pageValue(5.0),
  m_buttonRange(32),
  m_hold(false),
  m_buttonClicked(0),
  m_lastMouseValue(0),
  m_scrollBarType(HORIZONTALBAR)
{
	//m_range.set(0.0,10.0);
	setCanFocus(false);
}
crScrollBarElement::crScrollBarElement(const crScrollBarElement& element):
    crElement(element),
	m_value(element.m_value),
	m_lineValue(element.m_lineValue),
	m_pageValue(element.m_pageValue),
	m_buttonRange(element.m_buttonRange),
	m_hold(false),
	m_buttonClicked(0),
	m_lastMouseValue(0),
	m_scrollBarType(element.m_scrollBarType)
{
}
void crScrollBarElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	int temp,int1;
	float flt1;
	std::vector<float> v_i;
	cfg_script.Get("ScrollBarType", str);
	temp = 0;
	if(str.compare("HORIZONTALBAR")==0)
	{
		temp = 0;
		setScrollBarType(crScrollBarElement::HORIZONTALBAR);
	}
	else if(str.compare("VERTICALBAR")==0)
	{
		temp = 1;
		setScrollBarType(crScrollBarElement::VERTICALBAR);
	}
	if(cfg_script.Get("ButtonRange", int1))
	{
		if(temp == 0)
			setButtonRange((float)int1 * widthScale);
		else if(temp == 1)
			setButtonRange((float)int1 * heightScale);
	}
	if(cfg_script.Get("LineValue", flt1))
		setLineValue(flt1);
	if(cfg_script.Get("PageValue", flt1))
		setPageValue(flt1);
	if(cfg_script.Get("Range", v_i))
		setRange(v_i[0],v_i[1]);

	crElement::load(cfg_script,widthScale,heightScale);
}

bool crScrollBarElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	if(m_range[1] == m_range[0])
		return false;
	if(pressed(mx,my))
	{
		m_isMouseOnElement = true;
        float scrollValueRange;//滚动条值范围
		float barWindowSize;//滚动条实际长度
		float scrollBtnWidth;//滚动条按钮长度
		float _pos;//滚动条按钮起点位置
		float _far;//滚动条按钮终点位置
		switch(m_scrollBarType) 
		{
		case HORIZONTALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[2] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[0] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue ;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);
			break;
		case VERTICALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[3] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[1] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);
			break;
		}
		if(mouseButtonMsg == WM_LBUTTONDOWN)
		{
			switch(m_scrollBarType) 
			{
			case HORIZONTALBAR:
				if(mx - m_rect[0] < m_buttonRange)
				{
					m_value -= m_lineValue;
					m_buttonClicked = -1;
					break;
				}
				else if(getElementRight() - mx < m_buttonRange)
				{
					m_value += m_lineValue;
					m_buttonClicked = 1;
					break;
				}
				if(mx < _pos)
				{
					m_value -= m_pageValue;
				}
				else if(mx > _far)
				{
					m_value += m_pageValue;
				}
				else 
				{
					m_lastMouseValue = mx;
					m_hold = true;
				}
				break;
			case VERTICALBAR:
				if(my - m_rect[1] < m_buttonRange)
				{
					m_value -= m_lineValue;
					m_buttonClicked = -1;
					break;
				}
				else if(getElementBottom() - my < m_buttonRange)
				{
					m_value += m_lineValue;
					m_buttonClicked = 1;
					break;
				}
				if(my < _pos)
				{
					m_value -= m_pageValue;
				}
				else if(my > _far)
				{
					m_value += m_pageValue;
				}
				else 
				{
					m_lastMouseValue = my;
					m_hold = true;
				}
				break;
			}
		}
		else if(m_hold && mouseButtonMsg == WM_MOUSEMOVE)
		{
			switch(m_scrollBarType) 
			{
			case HORIZONTALBAR:
				if(mx - m_rect[0] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementRight() - mx < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else
				{
					m_value += float((mx - m_lastMouseValue)/barWindowSize) * scrollValueRange + m_range[0];
					m_lastMouseValue = mx;
				}
				break;
			case VERTICALBAR:
				if(my - m_rect[1] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementBottom() - my < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else
				{
				    m_value += float((my - m_lastMouseValue)/barWindowSize) * scrollValueRange + m_range[0];
					m_lastMouseValue = my;
				}
				break;
			}
		}
		else if(m_hold && mouseButtonMsg == WM_LBUTTONUP)
		{
			m_hold = false;
			switch(m_scrollBarType) 
			{
			case HORIZONTALBAR:
				if(mx - m_rect[0] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementRight() - mx < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else if(mx < _pos || mx > _far)
				    m_value = float((mx - m_rect[0] - m_buttonRange)/barWindowSize) * scrollValueRange + m_range[0];
				break;
			case VERTICALBAR:
				if(my - m_rect[1] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementBottom() - my < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else if(my < _pos || my > _far)
				    m_value = float((my - m_rect[1] - m_buttonRange)/barWindowSize) * scrollValueRange + m_range[0];
				break;
			}
		}

		if(m_value<m_range[0]) m_value = m_range[0];
		else if(m_value>m_range[1]) m_value = m_range[1];
		
		updateData();
	}
	else
	{
		if(m_hold && mouseButtonMsg == WM_MOUSEMOVE)
		{
			float scrollValueRange;
			float barWindowSize;
			float scrollBtnWidth;
			float _pos;
			float _far;
			switch(m_scrollBarType) 
			{
			case HORIZONTALBAR:
				scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
				barWindowSize = m_rect[2] - m_buttonRange * 2;
				scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
				_pos = m_rect[0] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue ;
				_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);
				break;
			case VERTICALBAR:
				scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
				barWindowSize = m_rect[3] - m_buttonRange * 2;
				scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
				_pos = m_rect[1] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue;
				_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);
				break;
			}
			switch(m_scrollBarType) 
			{
			case HORIZONTALBAR:
				if(mx - m_rect[0] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementRight() - mx < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else
				{
					m_value += float((mx - m_lastMouseValue)/barWindowSize) * scrollValueRange + m_range[0];
					m_lastMouseValue = mx;
				}
				break;
			case VERTICALBAR:
				if(my - m_rect[1] < m_buttonRange)
				{
					m_value = m_range[0];
					break;
				}
				else if(getElementBottom() - my < m_buttonRange)
				{
					m_value = m_range[1];
					break;
				}
				else
				{
					m_value += float((my - m_lastMouseValue)/barWindowSize) * scrollValueRange + m_range[0];
					m_lastMouseValue = my;
				}
				break;
			}
			if(m_value<m_range[0]) m_value = m_range[0];
			else if(m_value>m_range[1]) m_value = m_range[1];
			
			updateData();
		}
		else if(mouseButtonMsg == WM_LBUTTONUP)
		    m_hold = false;
		m_isMouseOnElement = false;
	}

    return crElement::testButtons(mx,my,mouseButtonMsg);
}
int crScrollBarElement::drawStageCopy(Display dc, Display down_dc)
{
	if(m_range[1] == m_range[0])
		return true;
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(m_hDownImage) 
			SelectObject(mdc,m_hDownImage);

		float scrollValueRange,barWindowSize,scrollBtnWidth,_pos,_far;

		switch(m_scrollBarType) 
		{
		case HORIZONTALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[2] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[0] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue ;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);

			if(m_hDownImage) 
			{
				BitBlt(dc,_pos,m_rect[1],scrollBtnWidth,m_rect[3],mdc,_pos - m_rect[0],0,SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_buttonRange,m_rect[3],mdc,0,0,SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,getElementRight() - m_buttonRange,m_rect[1],m_buttonRange,m_rect[3],mdc,m_rect[2],0,SRCCOPY);
				m_buttonClicked = 0;
			}
			else 
			{
				BitBlt(dc,_pos,m_rect[1],scrollBtnWidth,m_rect[3],down_dc,_pos,m_rect[1],SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_buttonRange,m_rect[3],down_dc,m_rect[0],m_rect[1],SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,getElementRight() - m_buttonRange,m_rect[1],m_buttonRange,m_rect[3],down_dc,getElementRight() - m_buttonRange,m_rect[1],SRCCOPY);
				m_buttonClicked = 0;
			}
			//BitBlt(dc,m_rect[0],m_rect[1],_far,m_rect[3],mdc,m_rect[0],m_rect[1],SRCCOPY);
			break;
		case VERTICALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[3] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[1] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);

			if(m_hDownImage) 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,mdc,0,_pos - m_rect[1],SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_buttonRange,mdc,0,0,SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,m_rect[0],getElementBottom() - m_buttonRange,m_rect[2],m_buttonRange,mdc,0,m_rect[3],SRCCOPY);
				m_buttonClicked = 0;
			}
			else 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,down_dc,m_rect[0],_pos,SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_buttonRange,down_dc,m_rect[0],m_rect[1],SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,m_rect[0],getElementBottom() - m_buttonRange,m_rect[2],m_buttonRange,down_dc,m_rect[0],getElementBottom() - m_buttonRange,SRCCOPY);
				m_buttonClicked = 0;
			}
			//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,m_rect[0],m_rect[1],SRCCOPY);
			break;
		}
		//else	//need draw by parent
		//	noNeedFatherDraw = false;
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crScrollBarElement::drawStageMask(Display dc, Display down_dc)
{
	if(m_range[1] == m_range[0])
		return true;
	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		if(m_hDownImage) 
			SelectObject(mdc,m_hDownImage);

		float scrollValueRange,barWindowSize,scrollBtnWidth,_pos,_far;
		switch(m_scrollBarType) 
		{
		case HORIZONTALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[2] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[0] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue ;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);

			if(m_hDownImage) 
			{
				BitBlt(dc,_pos,m_rect[1],scrollBtnWidth,m_rect[3],mdc,_pos - m_rect[0],0,SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_buttonRange,m_rect[3],mdc,0,0,SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,getElementRight() - m_buttonRange,m_rect[1],m_buttonRange,m_rect[3],mdc,m_rect[2],0,SRCCOPY);
				m_buttonClicked = 0;
			}
			else 
			{
				BitBlt(dc,_pos,m_rect[1],scrollBtnWidth,m_rect[3],down_dc,_pos,m_rect[1],SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_buttonRange,m_rect[3],down_dc,m_rect[0],m_rect[1],SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,getElementRight() - m_buttonRange,m_rect[1],m_buttonRange,m_rect[3],down_dc,getElementRight() - m_buttonRange,m_rect[1],SRCCOPY);
				m_buttonClicked = 0;
			}
			break;
		case VERTICALBAR:
			scrollValueRange = m_range[1] - m_range[0] + m_lineValue;
			barWindowSize = m_rect[3] - m_buttonRange * 2;
			scrollBtnWidth = m_lineValue * barWindowSize / scrollValueRange;
			_pos = m_rect[1] + m_buttonRange + (m_value - m_range[0]) * scrollBtnWidth/m_lineValue;
			_far = _pos + CRCore::maximum(5.0f,scrollBtnWidth);
			
			if(m_hDownImage) 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,mdc,0,_pos - m_rect[1],SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_buttonRange,mdc,0,0,SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,m_rect[0],getElementRight() - m_buttonRange,m_rect[2],m_buttonRange,mdc,0,m_rect[3],SRCCOPY);
				m_buttonClicked = 0;
			}
			else 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,down_dc,m_rect[0],_pos,SRCCOPY);
				if(m_buttonClicked == -1)
					BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_buttonRange,down_dc,m_rect[0],m_rect[1],SRCCOPY);
				else if(m_buttonClicked == 1)
					BitBlt(dc,m_rect[0],getElementRight() - m_buttonRange,m_rect[2],m_buttonRange,down_dc,m_rect[0],getElementRight() - m_buttonRange,SRCCOPY);
				m_buttonClicked = 0;
			}
			break;
		}
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

void crScrollBarElement::setScrollBarType(ScrollBarType bar)
{
    m_scrollBarType = bar;
}

crScrollBarElement::ScrollBarType crScrollBarElement::getScrollBarType()
{
    return m_scrollBarType;
}

void crScrollBarElement::setRange(const CRCore::crVector2f &range)
{
    m_range = range;
	if(m_range[1]<m_range[0])
	{
		float temp = m_range[0];
		m_range[0] = m_range[1];
		m_range[1] = temp;
	}

	if(m_value<m_range[0]) m_value = m_range[0];
	else if(m_value>m_range[1]) m_value = m_range[1];
}

void crScrollBarElement::setRange(float min, float max)
{
	m_range[0] = min;
	m_range[1] = max;
	if(m_range[1]<m_range[0])
	{
		float temp = m_range[0];
		m_range[0] = m_range[1];
		m_range[1] = temp;
	}

	if(m_value<m_range[0]) m_value = m_range[0];
	else if(m_value>m_range[1]) m_value = m_range[1];
}

const CRCore::crVector2f &crScrollBarElement::getRange()
{
    return m_range;
}

void crScrollBarElement::setValue(float value)
{
	if(value<m_range[0]) value = m_range[0];
	else if(value>m_range[1]) value = m_range[1];
    m_value = value;
}

float crScrollBarElement::getValue()
{
	return m_value;
}

void crScrollBarElement::setLineValue(float lineValue)
{
	m_lineValue = lineValue;
}

float crScrollBarElement::getLineValue()
{
	return m_lineValue;
}

void crScrollBarElement::setPageValue(float pageValue)
{
	m_pageValue = pageValue;
}

float crScrollBarElement::getPageValue()
{
	return m_pageValue;
}

void crScrollBarElement::setButtonRange(int buttonRange)
{
	m_buttonRange = buttonRange;
}

float crScrollBarElement::getButtonRange()
{
	return m_buttonRange;
}

//////////////////////////////////////////////////////////////////////////
//
//crSliderElement
//
//////////////////////////////////////////////////////////////////////////
crSliderElement::crSliderElement():
m_value(0.0),
m_backup_value(0.0),
m_sliderBtnWidth(10),
m_lastMouseValue(0),
m_hold(false),
m_sliderBarType(HORIZONTALBAR)
{
}
crSliderElement::crSliderElement(const crSliderElement& element):
crElement(element),
m_value(element.m_value),
m_sliderBtnWidth(element.m_sliderBtnWidth),
m_lastMouseValue(0),
m_hold(false),
m_sliderBarType(element.m_sliderBarType)
{
}
void crSliderElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	int temp,int1;
	std::vector<float> v_i;
	cfg_script.Get("SliderBarType", str);
	temp = 0;
	if(str.compare("HORIZONTALBAR")==0)
	{
		temp = 0;
		setSliderBarType(crSliderElement::HORIZONTALBAR);
	}
	else if(str.compare("VERTICALBAR")==0)
	{
		temp = 1;
		setSliderBarType(crSliderElement::VERTICALBAR);
	}
	if(cfg_script.Get("SliderBtnWidth", int1))
	{
		if(temp == 0)
			setSliderBtnWidth((float)int1 * widthScale);
		else if(temp == 1)
			setSliderBtnWidth((float)int1 * heightScale);
	}
	if(cfg_script.Get("Range", v_i))
		setRange(v_i[0],v_i[1]);

	crElement::load(cfg_script,widthScale,heightScale);
}

bool crSliderElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	if(m_range[1] == m_range[0])
		return false;
	if(pressed(mx,my))
	{
		m_isMouseOnElement = true;
		float sliderValueRange;//Slider值范围
		float barWindowSize;//Slider实际长度

		switch(m_sliderBarType) 
		{
		case HORIZONTALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[2];
			break;
		case VERTICALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[3];
			break;
		}
		if(mouseButtonMsg == WM_LBUTTONDOWN)
		{
			switch(m_sliderBarType) 
			{
			case HORIZONTALBAR:
				m_lastMouseValue = mx;
				m_hold = true;
				break;
			case VERTICALBAR:
				m_lastMouseValue = my;
				m_hold = true;
				break;
			}
		}
		else if(m_hold && mouseButtonMsg == WM_MOUSEMOVE)
		{
			switch(m_sliderBarType) 
			{
			case HORIZONTALBAR:		
				m_value += float((mx - m_lastMouseValue)/barWindowSize) * sliderValueRange + m_range[0];
				m_lastMouseValue = mx;
				break;
			case VERTICALBAR:
				m_value += float((my - m_lastMouseValue)/barWindowSize) * sliderValueRange + m_range[0];
				m_lastMouseValue = my;
				break;
			}
		}
		else if(m_hold && mouseButtonMsg == WM_LBUTTONUP)
		{
			m_hold = false;
			switch(m_sliderBarType) 
			{
			case HORIZONTALBAR:
				m_value = float((mx - m_rect[0])/barWindowSize) * sliderValueRange + m_range[0];
				break;
			case VERTICALBAR:
				m_value = float((my - m_rect[1])/barWindowSize) * sliderValueRange + m_range[0];
				break;
			}
		}

		if(m_value<m_range[0]) m_value = m_range[0];
		else if(m_value>m_range[1]) m_value = m_range[1];

		updateData();
	}
	else
	{
		if(m_hold && mouseButtonMsg == WM_MOUSEMOVE)
		{
			float sliderValueRange;//Slider值范围
			float barWindowSize;//Slider实际长度

			switch(m_sliderBarType) 
			{
			case HORIZONTALBAR:
				sliderValueRange = m_range[1] - m_range[0];
				barWindowSize = m_rect[2];
				break;
			case VERTICALBAR:
				sliderValueRange = m_range[1] - m_range[0];
				barWindowSize = m_rect[3];
				break;
			}
			switch(m_sliderBarType) 
			{
			case HORIZONTALBAR:		
				m_value += float((mx - m_lastMouseValue)/barWindowSize) * sliderValueRange + m_range[0];
				m_lastMouseValue = mx;
				break;
			case VERTICALBAR:
				m_value += float((my - m_lastMouseValue)/barWindowSize) * sliderValueRange + m_range[0];
				m_lastMouseValue = my;
				break;
			}
			if(m_value<m_range[0]) m_value = m_range[0];
			else if(m_value>m_range[1]) m_value = m_range[1];

			updateData();
		}
		else if(mouseButtonMsg == WM_LBUTTONUP)
			m_hold = false;
		m_isMouseOnElement = false;
	}

	return crElement::testButtons(mx,my,mouseButtonMsg);
}
int crSliderElement::drawStageCopy(Display dc, Display down_dc)
{
	if(m_range[1] == m_range[0])
		return true;
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(m_hDownImage) 
			SelectObject(mdc,m_hDownImage);

		float sliderValueRange;//Slider值范围
		float barWindowSize;//Slider实际长度
		float _pos,_far;

		switch(m_sliderBarType) 
		{
		case HORIZONTALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[2] - m_sliderBtnWidth;
			_pos = m_rect[0] + (m_value - m_range[0]) * barWindowSize/sliderValueRange;
			_far = _pos + (float)m_sliderBtnWidth;

			if(m_hDownImage) 
			{
				BitBlt(dc,_pos,m_rect[1],m_sliderBtnWidth,m_rect[3],mdc,_pos - m_rect[0],0,SRCCOPY);
			}
			else 
			{
				BitBlt(dc,_pos,m_rect[1],m_sliderBtnWidth,m_rect[3],down_dc,_pos,m_rect[1],SRCCOPY);
			}
			break;
		case VERTICALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[3] - m_sliderBtnWidth;
			_pos = m_rect[1] + (m_value - m_range[0]) * barWindowSize/sliderValueRange;
			_far = _pos + (float)m_sliderBtnWidth;

			if(m_hDownImage) 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,mdc,0,_pos - m_rect[1],SRCCOPY);
			}
			else 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,down_dc,m_rect[0],_pos,SRCCOPY);
			}
			break;
		}
		//else	//need draw by parent
		//	noNeedFatherDraw = false;
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crSliderElement::drawStageMask(Display dc, Display down_dc)
{
	if(m_range[1] == m_range[0])
		return true;
	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		if(m_hDownImage) 
			SelectObject(mdc,m_hDownImage);

		float sliderValueRange;//Slider值范围
		float barWindowSize;//Slider实际长度
		float _pos,_far;

		switch(m_sliderBarType) 
		{
		case HORIZONTALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[2]- m_sliderBtnWidth;
			_pos = m_rect[0] + (m_value - m_range[0]) * barWindowSize/sliderValueRange;
			_far = _pos + (float)m_sliderBtnWidth;

			if(m_hDownImage) 
			{
				BitBlt(dc,_pos,m_rect[1],m_sliderBtnWidth,m_rect[3],mdc,_pos - m_rect[0],0,SRCCOPY);
			}
			else 
			{
				BitBlt(dc,_pos,m_rect[1],m_sliderBtnWidth,m_rect[3],down_dc,_pos,m_rect[1],SRCCOPY);
			}
			break;
		case VERTICALBAR:
			sliderValueRange = m_range[1] - m_range[0];
			barWindowSize = m_rect[3]- m_sliderBtnWidth;
			_pos = m_rect[1] + (m_value - m_range[0]) * barWindowSize/sliderValueRange;
			_far = _pos + (float)m_sliderBtnWidth;

			if(m_hDownImage) 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,mdc,0,_pos - m_rect[1],SRCCOPY);
			}
			else 
			{
				BitBlt(dc,m_rect[0],_pos,m_rect[2],_far - _pos,down_dc,m_rect[0],_pos,SRCCOPY);
			}
			break;
		}
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

void crSliderElement::setSliderBarType(SliderBarType bar)
{
	m_sliderBarType = bar;
}

crSliderElement::SliderBarType crSliderElement::getSliderBarType()
{
	return m_sliderBarType;
}

void crSliderElement::setRange(const CRCore::crVector2f &range)
{
	m_range = range;
	if(m_range[1]<m_range[0])
	{
		float temp = m_range[0];
		m_range[0] = m_range[1];
		m_range[1] = temp;
	}
}

void crSliderElement::setRange(float min, float max)
{
	m_range[0] = min;
	m_range[1] = max;
	if(m_range[1]<m_range[0])
	{
		float temp = m_range[0];
		m_range[0] = m_range[1];
		m_range[1] = temp;
	}
}

const CRCore::crVector2f &crSliderElement::getRange()
{
	return m_range;
}

void crSliderElement::setValue(float value)
{
	if(value<m_range[0]) value = m_range[0];
	else if(value>m_range[1]) value = m_range[1];
	m_value = value;
}

float crSliderElement::getValue()
{
	return m_value;
}

void crSliderElement::setSliderBtnWidth(int buttonRange)
{
	m_sliderBtnWidth = buttonRange;
}

float crSliderElement::setSliderBtnWidth()
{
	return m_sliderBtnWidth;
}
//////////////////////////////////////////////////////////////////////////
//
//crListBoxElement
//
//////////////////////////////////////////////////////////////////////////
crListBoxElement::crListBoxElement(const crListBoxElement& element):
    crElement(element),
    m_dataList(element.m_dataList),
	m_select(element.m_select),
    m_rowHeight(element.m_rowHeight),
    m_textAttribute(element.m_textAttribute),
	m_useLeftBtnSelect(element.m_useLeftBtnSelect),
	m_listScrollBarName(element.m_listScrollBarName)
{
}

void crListBoxElement::setUseLeftBtnSelect(bool leftBtnSelect)
{
    m_useLeftBtnSelect = leftBtnSelect;
}

CRCore::crVector4f crListBoxElement::getScrollRange()
{
	float data_height = m_rowHeight * (getDataSize()+1);
	float height = m_rect[3];
	CRCore::crVector4f scrollRange;
	if(data_height > height)
	{
		float x = (data_height - height) / float(m_rowHeight);
		if(x > floor(x))
		{
			scrollRange[3] = floor(x) + 1;
		}
		else
			scrollRange[3] = floor(x);
	}
	return scrollRange;
}

void crListBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
    int int1;
	if(cfg_script.Get("TextAttribute", str))
	{
        setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	int nData = 1;
	while (cfg_script.Get("Data", str,nData++)) 
	{
		addData(str);
	}
	if(cfg_script.Get("RowHeight", int1))
		setRowHeight((float)int1 /* heightScale*/);
	if(cfg_script.Get("Select", int1))
		select(int1);
	if(cfg_script.Get("UseLeftBtnSelect", int1))
		setUseLeftBtnSelect(int1);
	cfg_script.Get("ListScrollBarName",m_listScrollBarName);
	crElement::load(cfg_script,widthScale,heightScale);
}

void crListBoxElement::initWindow()
{
	m_listScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_listScrollBarName));
	crElement::initWindow();
	if(m_listScrollBar.valid())
	{
		crVector4f scrollRange = this->getScrollRange();
		m_listScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));

		int vscrollValue = m_listScrollBar->getValue();
		int _select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_listScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_listScrollBar->setValue(vscrollValue);
		}
	}
}
void crListBoxElement::checkSelect()
{
	if(isDataEmpty())
		m_select = 0;
	else
	{
		int size = getDataSize() - 1;
		if(m_select>size) m_select = size;
	}
}
void crListBoxElement::updateData()
{
	checkSelect();
	crElement::updateData();
	
	if(m_listScrollBar.valid())
	{
		crVector4f scrollRange = this->getScrollRange();
		m_listScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));

		int vscrollValue = m_listScrollBar->getValue();
		int _select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_listScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_listScrollBar->setValue(vscrollValue);
		}
	}
}

bool crListBoxElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	bool test = false;
	if(getCapture() && m_listScrollBar.valid())
	{
		test = m_listScrollBar->testButtons(mx,my,mouseButtonMsg);
	}
	if(!test)
	{
		if(pressed(mx,my))
		{
			int vscrollValue = 0;
			if(m_listScrollBar.valid()) vscrollValue = m_listScrollBar->getValue();

			m_isMouseOnElement = true;
			if(!m_useLeftBtnSelect || mouseButtonMsg == WM_LBUTTONDOWN)
			{
				int i = (my-m_rect[1])/m_rowHeight + vscrollValue;
				if(i>=0 && i < getDataSize() && m_select != i)
				{
					select(i);
				}
			}
		}
		else
		{
			m_isMouseOnElement = false;
		}
		test = crElement::testButtons(mx,my,mouseButtonMsg);
	}

	return test; 
}

int crListBoxElement::getMaxRowCanBeDisplay()
{
	float x = float(m_rect[3])/float(m_rowHeight);
	return x > floor(x)?floor(x)-1:floor(x)-2;
}

int crListBoxElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = 0;
	if(m_listScrollBar.valid()) vscrollValue = m_listScrollBar->getValue();

	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		if(m_select>-1)
		{
			int _select = m_select - vscrollValue;
			if(_select<0)
			{
				_select = 0;
				//m_select = vscrollValue;
				select(vscrollValue);
			}
			else
			{
				int maxrow = getMaxRowCanBeDisplay();
				if(_select>maxrow) _select = maxrow;
				//m_select = vscrollValue + _select;
				select(vscrollValue + _select);
			}
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,mdc,0,0,SRCCOPY);
			}
			else	//need draw by parent
			{
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,down_dc,m_rect[0],m_rect[1]+_select*m_rowHeight,SRCCOPY);
			}
		}
		//draw data
		m_dataMutex.lock();
		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		int i = 0;
		RECT rect;
		for( DataList::iterator itr = m_dataList.begin() + vscrollValue;
			itr != m_dataList.end();
			++itr, ++i )
		{
			rect.left   = m_rect[0] + TextRectOffset;
			rect.top    = m_rect[1]+i*m_rowHeight + TextRectOffset;
			rect.right  = getElementRight() - TextRectOffset;
			rect.bottom = m_rect[1]+(i+1)*m_rowHeight - TextRectOffset;
			if(rect.bottom>getElementBottom()) break;
			DrawText(dc,itr->c_str(),itr->length(),&rect,DT_WORD_ELLIPSIS);
			//ExtTextOut(dc,rect.left,rect.top,ETO_OPAQUE,&rect,itr->c_str(),itr->length(),NULL);

			//TextOut(hdc,m_rect[0]+m_texOffset[0],m_rect[1]+i*m_rowHeight+m_texOffset[1],itr->c_str(),itr->length());
		}
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
		m_dataMutex.unlock();
		
		//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crListBoxElement::drawStageMask(Display dc, Display down_dc)
{
	int vscrollValue = m_listScrollBar.valid()?m_listScrollBar->getValue():0;

	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		if(m_select>-1)
		{
			int _select = m_select - vscrollValue;
			if(_select<0)
			{
				_select = 0;
				m_select = vscrollValue;
			}
			else
			{
				int maxrow = getMaxRowCanBeDisplay();
				if(_select>maxrow) _select = maxrow;
				m_select = vscrollValue + _select;
			}
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(bufdc,0,_select*m_rowHeight,m_rect[2],m_rowHeight,mdc,0,0,SRCCOPY);
			}
			else	//need draw by parent
			{
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,down_dc,m_rect[0],m_rect[1]+_select*m_rowHeight,SRCCOPY);
			}
		}
		//draw data
		m_dataMutex.lock();
		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(bufdc);
		int i = 0;
		RECT rect;
		for( DataList::iterator itr = m_dataList.begin() + vscrollValue;
			itr != m_dataList.end();
			++itr, ++i )
		{
			rect.left   = m_rect[0] + TextRectOffset;
			rect.top    = m_rect[1]+i*m_rowHeight + TextRectOffset;
			rect.right  = getElementRight() - TextRectOffset;
			rect.bottom = m_rect[1]+(i+1)*m_rowHeight - TextRectOffset;
			if(rect.bottom>getElementBottom()) break;
			DrawText(bufdc,itr->c_str(),itr->length(),&rect,DT_WORD_ELLIPSIS);
			//ExtTextOut(bufdc,rect.left,rect.top,ETO_OPAQUE,&rect,itr->c_str(),itr->length(),NULL);

			//TextOut(hdc,m_rect[0]+m_texOffset[0],m_rect[1]+i*m_rowHeight+m_texOffset[1],itr->c_str(),itr->length());
		}
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(bufdc);
		m_dataMutex.unlock();
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

void crListBoxElement::clearData()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
    m_dataList.clear();
}

void crListBoxElement::addData(const std::string& data)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
    m_dataList.push_back(data);
}

void crListBoxElement::deleteCurrentRow()
{
    if(!isDataEmpty())
	{
		m_dataMutex.lock();
        int i = getSelect();
		DataList::iterator itr = m_dataList.begin();
		itr+=i;
        m_dataList.erase(itr);
		m_dataMutex.unlock();
	}
}

void crListBoxElement::insertData(unsigned int index, const std::string &data)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if (index >= m_dataList.size())
	{
		m_dataList.push_back(data);
	}
	else
	{
		m_dataList.insert(m_dataList.begin()+index, data);
	}
}

int crListBoxElement::getDataSize()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataList.size(); 
}
bool crListBoxElement::isDataEmpty()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataList.empty();
}
bool crListBoxElement::getData(unsigned int index,std::string &data)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if(index < m_dataList.size())
	{
		data = m_dataList[index];
        return true;
	}
	return false;
}
bool crListBoxElement::modifyData(unsigned int index,const std::string &data)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if(index < m_dataList.size())
	{
		m_dataList[index] = data;
		return true;
	}
	return false;
}
void crListBoxElement::select(int index)
{
	if(m_select!=index)
	{
		m_select = index;
		if(m_listScrollBar.valid())
		{
			int vscrollValue = m_listScrollBar->getValue();
			int _select = m_select - vscrollValue;
			int maxrow = getMaxRowCanBeDisplay();
			if(_select<0)
			{
                vscrollValue += _select;
				m_listScrollBar->setValue(vscrollValue);
			}
			else if(_select>maxrow)
			{	
				vscrollValue += (_select - maxrow);
				m_listScrollBar->setValue(vscrollValue);
			}
		}
	    updateData();
	}
}

void crListBoxElement::setSelect(int index)
{
	if(m_select!=index)
	{
		m_select = index;
		if(m_listScrollBar.valid())
		{
			int vscrollValue = m_listScrollBar->getValue();
			int _select = m_select - vscrollValue;
			int maxrow = getMaxRowCanBeDisplay();
			if(_select<0)
			{
				vscrollValue += _select;
				m_listScrollBar->setValue(vscrollValue);
			}
			else if(_select>maxrow)
			{	
				vscrollValue += (_select - maxrow);
				m_listScrollBar->setValue(vscrollValue);
			}
		}
	}
}

int crListBoxElement::getSelect()
{
	return m_select;
}

void crListBoxElement::setRowHeight(int height)
{
    m_rowHeight = height;
}

int crListBoxElement::getRowHeight()
{
	return m_rowHeight;
}

//////////////////////////////////////////////////////////////////////////
//
//crImageBoxElement
//
//////////////////////////////////////////////////////////////////////////
crImageBoxElement::crImageBoxElement(const crImageBoxElement& element):
    crElement(element),
	m_hScrollBarName(element.m_hScrollBarName),
    m_vScrollBarName(element.m_vScrollBarName)
{
}
void crImageBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);
	crElement::load(cfg_script,widthScale,heightScale);
}

CRCore::crVector4f crImageBoxElement::getScrollRange()
{
    if(!m_hImage) return crVector4f(0.0,0.0,0.0,0.0);
	BITMAP bmp;
	GetObject(m_hImage, sizeof(BITMAP), (LPSTR)&bmp);
	int width = bmp.bmWidth - m_rect[2];
	width = maximum(0,width);
	int height = bmp.bmHeight - m_rect[3];
	height = maximum(0,height);
    return crVector4f(0.0f,width,0.0f,height);
}

void crImageBoxElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	crVector4f scrollRange = this->getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

void crImageBoxElement::updateData()
{
	crElement::updateData();
	crVector4f scrollRange = this->getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

bool crImageBoxElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);

	return crElement::testButtons(mx,my,mouseButtonMsg);
}
bool crImageBoxElement::setImageFile( const std::string& file )
{
	if(m_imageFile.compare(file) != 0)
	{
		if(m_hImage)
			DeleteObject(m_hImage);
		m_hImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),file.c_str(),IMAGE_BITMAP,m_rect[2],m_rect[3],LR_LOADFROMFILE);
		m_imageFile = file;
		return true;
	}
	return false;
}
void crImageBoxElement::resize(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::resize(cfg_script,widthScale,heightScale);
	if(!m_imageFile.empty())
	{
		if(m_hImage)
			DeleteObject(m_hImage);
		m_hImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_imageFile.c_str(),IMAGE_BITMAP,m_rect[2],m_rect[3],LR_LOADFROMFILE);
	}
}
int crImageBoxElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,hscrollValue,vscrollValue,SRCCOPY);
		}
		if(/*m_focus||*/m_isMouseOnElement)
		{
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,hscrollValue,vscrollValue,SRCCOPY);
			}
		}
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crImageBoxElement::drawStageMask(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,hscrollValue,vscrollValue,SRCCOPY);
		}
		if(/*m_focus||*/m_isMouseOnElement)
		{
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,hscrollValue,vscrollValue,SRCCOPY);
			}
		}
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,hscrollValue,vscrollValue,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,hscrollValue,vscrollValue,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,hscrollValue,vscrollValue,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crStaticTextBoxElement
//
//////////////////////////////////////////////////////////////////////////
crStaticTextBoxElement::crStaticTextBoxElement(const crStaticTextBoxElement& element):
    crElement(element),
    m_textAttribute(element.m_textAttribute),
    m_stringArray(element.m_stringArray),
	m_hScrollBarName(element.m_hScrollBarName),
	m_vScrollBarName(element.m_vScrollBarName),
	m_textFormat(element.m_textFormat)
{
}

void crStaticTextBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	if(cfg_script.Get("TextAttribute", str))
	{
        setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	if(cfg_script.Get("Text", str))
	{
		setStringArrayByString(str);
	}
	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);
	m_textFormat = 0;
	if(cfg_script.Get("TextFormat", str))
	{
		if(str.find("DT_LEFT") != std::string::npos)
		{
			m_textFormat |= DT_LEFT;
		}
		if(str.find("DT_CENTER") != std::string::npos)
		{
			m_textFormat |= DT_CENTER;
		}
		if(str.find("DT_RIGHT") != std::string::npos)
		{
			m_textFormat |= DT_RIGHT;
		}
		if(str.find("DT_VCENTER") != std::string::npos)
		{
			m_textFormat |= DT_VCENTER;
		}
		if(str.find("DT_BOTTOM") != std::string::npos)
		{
			m_textFormat |= DT_BOTTOM;
		}
		if(str.find("DT_WORDBREAK") != std::string::npos)
		{
			m_textFormat |= DT_WORDBREAK;
		}
		if(str.find("DT_SINGLELINE") != std::string::npos)
		{
			m_textFormat |= DT_SINGLELINE;
		}
		if(str.find("DT_EXPANDTABS") != std::string::npos)
		{
			m_textFormat |= DT_EXPANDTABS;
		}
		if(str.find("DT_TABSTOP") != std::string::npos)
		{
			m_textFormat |= DT_TABSTOP;
		}
		if(str.find("DT_NOCLIP") != std::string::npos)
		{
			m_textFormat |= DT_NOCLIP;
		}
		if(str.find("DT_EXTERNALLEADING") != std::string::npos)
		{
			m_textFormat |= DT_EXTERNALLEADING;
		}
		if(str.find("DT_CALCRECT") != std::string::npos)
		{
			m_textFormat |= DT_CALCRECT;
		}
		if(str.find("DT_NOPREFIX") != std::string::npos)
		{
			m_textFormat |= DT_NOPREFIX;
		}
		if(str.find("DT_INTERNAL") != std::string::npos)
		{
			m_textFormat |= DT_INTERNAL;
		}
	}
	crElement::load(cfg_script,widthScale,heightScale);
}

void crStaticTextBoxElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	calcScroll();
}

void crStaticTextBoxElement::lockArray()
{
    m_arrayMutex.lock();
}

void crStaticTextBoxElement::unLockArray()
{
    m_arrayMutex.unlock();
}

void crStaticTextBoxElement::clearString()
{
	m_arrayMutex.lock();
	m_stringArray.clear();
	m_arrayMutex.unlock();
	updateData();
}

void crStaticTextBoxElement::updateData()
{
	crElement::updateData();
	calcScroll();
}

const crStaticTextBoxElement::StringArray& crStaticTextBoxElement::getStringArray() const 
{ 
	return m_stringArray; 
}

crStaticTextBoxElement::StringArray& crStaticTextBoxElement::getStringArray() 
{ 
	return m_stringArray; 
}

std::string crStaticTextBoxElement::getString(unsigned int i) 
{
	return i>=0&&!m_stringArray.empty()&&i<m_stringArray.size()?m_stringArray[i]:""; 
}

unsigned int crStaticTextBoxElement::getNumStrings() 
{
	return m_stringArray.size(); 
}

void crStaticTextBoxElement::addString(const std::string& desc)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_arrayMutex);
	m_stringArray.push_back(desc);
}

std::string crStaticTextBoxElement::getStringArrayInString()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_arrayMutex);
	std::string str;
	for( StringArray::const_iterator itr = m_stringArray.begin();
		itr != m_stringArray.end();
		++itr )
	{
		str += *itr;
	}
	return str;
}

void crStaticTextBoxElement::setStringArrayByString( const std::string &command )
{
	m_arrayMutex.lock();
	m_stringArray.clear();
	m_arrayMutex.unlock();

	//unsigned int stringLength = command.length();
	unsigned int front_of_line = 0;
	unsigned int end_of_line = 0;
	while (end_of_line<command.size())
	{
		/*if (command[end_of_line]=='\r')
		{
		addString( std::string( command, front_of_line, end_of_line-front_of_line) );

		if (end_of_line<command.size() &&
		command[end_of_line]=='\n') ++end_of_line;
		++end_of_line;
		front_of_line = end_of_line;
		}
		else */if (command[end_of_line]=='\n')
		{	
			++end_of_line;
			addString( std::string( command, front_of_line, end_of_line-front_of_line) );

			front_of_line = end_of_line;
		}
		else ++end_of_line;
	}
	if (front_of_line<end_of_line)
	{
		addString( std::string( command, front_of_line, end_of_line-front_of_line) );
	}
	//  if(command[end_of_line-1]=='\n')
	//addString( std::string( command, end_of_line-1, 1) );

	updateData();
}

bool crStaticTextBoxElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);
	return crElement::testButtons(mx,my,mouseButtonMsg);
   // return false;
}

void crStaticTextBoxElement::calcScroll()
{
	CRCore::crVector4f scrollRange;
	if(!m_stringArray.empty())
	{
		int num_line = getNumStrings();
		HDC		dc;
		dc=CreateCompatibleDC(NULL);	

		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		//RECT rect;
		//rect.left   = m_rect[0];
		//rect.top    = m_rect[1];
		//rect.right  = getElementRight();
		//rect.bottom = getElementBottom();
		//int rowHeight = DrawText(dc,"h",1,&rect,DT_INTERNAL);
		SIZE   s;
		::GetTextExtentPoint32(dc,"H",1,&s);
		int w = s.cx+1;
		int h = s.cy+1;

		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
		DeleteDC(dc);

		int height = m_rect[3];
		int data_height = h * num_line;
		if(data_height>height)
		{
			scrollRange[3] = data_height - height;
		}
		if(m_vScrollBar.valid())
		{
			m_vScrollBar->setLineValue(h);
			m_vScrollBar->setPageValue(4 * h);
		}

		m_arrayMutex.lock();
		int maxsize = 0;
		for( StringArray::iterator itr = m_stringArray.begin();
			itr != m_stringArray.end();
			++itr )
		{
			maxsize = CRCore::maximum((int)(itr->length()),maxsize);
		}
		m_arrayMutex.unlock();

		int width = m_rect[2];
		int data_width = w * (maxsize+1);
		if(data_width>width)
		{
			scrollRange[1] = data_width - width;
		}
		if(m_hScrollBar.valid())
		{
			m_hScrollBar->setLineValue(w);
			m_hScrollBar->setPageValue(4 * w);
		}
	}
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

int crStaticTextBoxElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);	
	// draw bitmap
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	if(hscrollValue != 0 || vscrollValue != 0)
	{
		int width = GetDeviceCaps(dc, HORZRES);
		int height = GetDeviceCaps(dc, VERTRES);
		copy_dc=CreateCompatibleDC(dc);
		copybmp = CreateCompatibleBitmap(dc,width,height);
		SelectObject(copy_dc,copybmp);

		BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
	}

	//draw data
	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	RECT rect;
	rect.left   = m_rect[0] - hscrollValue + TextRectOffset;
	rect.top    = m_rect[1] - vscrollValue + TextRectOffset;
	rect.right  = getElementRight() - TextRectOffset;
	rect.bottom = getElementBottom() - TextRectOffset;
	//DrawText(dc,m_texString.c_str(),m_texString.length(),&rect,DT_WORDBREAK);
	std::string strbuf = getStringArrayInString();
    DrawText(dc,strbuf.c_str(),strbuf.length(),&rect,m_textFormat/*DT_INTERNAL|DT_WORDBREAK*/);
	//int height = 0;
	//for( StringArray::iterator itr = m_stringArray.begin();
	//	 itr != m_stringArray.end();
	//	 ++itr )
	//{
 //       height = DrawText(dc,(*itr).c_str(),(*itr).length(),&rect,DT_WORDBREAK/*|DT_CALCRECT*/);
	//	//TextOut(dc,rect.left,rect.bottom,"aaa",3);
	//	//CRCore::notify(CRCore::ALWAYS)<<"DrawText str = "<<(*itr).c_str()<<std::endl;
 //       rect.top += height;
	//	if(rect.top>=rect.bottom)
	//	    break;
	//}

	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

	if(hscrollValue != 0 || vscrollValue != 0)
	{
		crVector4i v4;
		if(hscrollValue != 0)
		{
			v4[0] = m_rect[0] - hscrollValue;
			v4[1] = m_rect[1];
			v4[2] = hscrollValue;
			v4[3] = m_rect[3];
			BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
		}
		if(vscrollValue != 0)
		{
			v4[0] = m_rect[0];
			v4[1] = m_rect[1]-vscrollValue;
			v4[2] = m_rect[2];
			v4[3] = vscrollValue;
			BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
		}
		DeleteDC(copy_dc);
		DeleteObject(copybmp);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crStaticTextBoxElement::drawStageMask(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc,copy_dc;
	HBITMAP bufbmp,copybmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
    if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}
	
	if(hscrollValue != 0 || vscrollValue != 0)
	{
		int width = GetDeviceCaps(dc, HORZRES);
		int height = GetDeviceCaps(dc, VERTRES);
		copy_dc=CreateCompatibleDC(dc);
		copybmp = CreateCompatibleBitmap(dc,width,height);
		SelectObject(copy_dc,copybmp);

		BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
	}

	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	RECT rect;
	rect.left   = m_rect[0] - hscrollValue + TextRectOffset;
	rect.top    = m_rect[1] - vscrollValue + TextRectOffset;
	rect.right  = getElementRight() - TextRectOffset;
	rect.bottom = getElementBottom() - TextRectOffset;
	std::string strbuf = getStringArrayInString();
	DrawText(dc,strbuf.c_str(),strbuf.length(),&rect,m_textFormat/*DT_INTERNAL|DT_WORDBREAK*/);
	//int height = 0;
	//for( StringArray::iterator itr = m_stringArray.begin();
	//	itr != m_stringArray.end();
	//	++itr )
	//{
	//	height = DrawText(dc,(*itr).c_str(),(*itr).length(),&rect,DT_WORDBREAK/*|DT_CALCRECT*/);
	//	rect.top += height;
	//	if(rect.top>=rect.bottom)
	//	    break;
	//}
	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

	if(hscrollValue != 0 || vscrollValue != 0)
	{
		crVector4i v4;
		if(hscrollValue != 0)
		{
			v4[0] = m_rect[0] - hscrollValue;
			v4[1] = m_rect[1];
			v4[2] = hscrollValue;
			v4[3] = m_rect[3];
			BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
		}
		if(vscrollValue != 0)
		{
			v4[0] = m_rect[0];
			v4[1] = m_rect[1]-vscrollValue;
			v4[2] = m_rect[2];
			v4[3] = vscrollValue;
			BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
		}
		DeleteDC(copy_dc);
		DeleteObject(copybmp);
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crEditBoxElement
//
//////////////////////////////////////////////////////////////////////////
float crEditBoxElement::s_focusInterval = 0.5f;

crEditBoxElement::crEditBoxElement(const crEditBoxElement& element):
crElement(element),
m_textAttribute(element.m_textAttribute),
m_stringArray(element.m_stringArray),
m_hScrollBarName(element.m_hScrollBarName),
m_vScrollBarName(element.m_vScrollBarName),
m_multiLine(element.m_multiLine),
m_password(element.m_password),
m_textFormat(element.m_textFormat),
m_time(0L),m_hasfocurs(false),m_focusPos(0)
{
}

void crEditBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	if(cfg_script.Get("TextAttribute", str))
	{
        setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	if(cfg_script.Get("Text", str))
	{
		setStringArrayByString(str);
	}
	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);
	int int1;
	if(cfg_script.Get("MultiLine",int1))
		m_multiLine = int1;
	if(cfg_script.Get("Password",int1))
	    m_password = int1;
	m_textFormat = 0;
	if(cfg_script.Get("TextFormat", str))
	{
		if(str.find("DT_LEFT") != std::string::npos)
		{
			m_textFormat |= DT_LEFT;
		}
		if(str.find("DT_CENTER") != std::string::npos)
		{
			m_textFormat |= DT_CENTER;
		}
		if(str.find("DT_RIGHT") != std::string::npos)
		{
			m_textFormat |= DT_RIGHT;
		}
		if(str.find("DT_VCENTER") != std::string::npos)
		{
			m_textFormat |= DT_VCENTER;
		}
		if(str.find("DT_BOTTOM") != std::string::npos)
		{
			m_textFormat |= DT_BOTTOM;
		}
		if(str.find("DT_WORDBREAK") != std::string::npos)
		{
			m_textFormat |= DT_WORDBREAK;
		}
		if(str.find("DT_SINGLELINE") != std::string::npos)
		{
			m_textFormat |= DT_SINGLELINE;
		}
		if(str.find("DT_EXPANDTABS") != std::string::npos)
		{
			m_textFormat |= DT_EXPANDTABS;
		}
		if(str.find("DT_TABSTOP") != std::string::npos)
		{
			m_textFormat |= DT_TABSTOP;
		}
		if(str.find("DT_NOCLIP") != std::string::npos)
		{
			m_textFormat |= DT_NOCLIP;
		}
		if(str.find("DT_EXTERNALLEADING") != std::string::npos)
		{
			m_textFormat |= DT_EXTERNALLEADING;
		}
		if(str.find("DT_CALCRECT") != std::string::npos)
		{
			m_textFormat |= DT_CALCRECT;
		}
		if(str.find("DT_NOPREFIX") != std::string::npos)
		{
			m_textFormat |= DT_NOPREFIX;
		}
		if(str.find("DT_INTERNAL") != std::string::npos)
		{
			m_textFormat |= DT_INTERNAL;
		}
	}
	crElement::load(cfg_script,widthScale,heightScale);

	//setCanCapture(true);
}

void crEditBoxElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	calcScroll();
}

void crEditBoxElement::clearString()
{
	m_arrayMutex.lock();
    m_stringArray.clear();
	m_arrayMutex.unlock();
	updateData();
}

void crEditBoxElement::updateData()
{
	crElement::updateData();
	calcScroll();

	std::string strbuf = getStringArrayInString();
	if(isFocusOnShow())
		m_focusPos = CRCore::maximum(CRCore::minimum(m_focusPos,(int)strbuf.size()+1),1);
	else
		m_focusPos = CRCore::maximum(CRCore::minimum(m_focusPos,(int)strbuf.size()),0);
}
void crEditBoxElement::calcScroll()
{
	CRCore::crVector4f scrollRange;
	if(!m_stringArray.empty())
	{
		int num_line = getNumStrings();
		HDC		dc;
		dc=CreateCompatibleDC(NULL);	

		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		//RECT rect;
		//rect.left   = m_rect[0];
		//rect.top    = m_rect[1];
		//rect.right  = getElementRight();
		//rect.bottom = getElementBottom();
		//int rowHeight = DrawText(dc,"h",1,&rect,DT_INTERNAL);
		SIZE   s;
		::GetTextExtentPoint32(dc,"H",1,&s);
		int w = s.cx+1;
		int h = s.cy+1;

		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
		DeleteDC(dc);

		int height = m_rect[3];
		int data_height = h * num_line;
		if(data_height>height)
		{
			scrollRange[3] = data_height - height;
		}
		if(m_vScrollBar.valid())
		{
			m_vScrollBar->setLineValue(h);
			m_vScrollBar->setPageValue(4 * h);
		}

		m_arrayMutex.lock();
		int maxsize = 0;
		for( StringArray::iterator itr = m_stringArray.begin();
			itr != m_stringArray.end();
			++itr )
		{
			maxsize = CRCore::maximum((int)(itr->length()),maxsize);
		}
		m_arrayMutex.unlock();
        int width = m_rect[2];
		int data_width = w * (maxsize+1);
		if(data_width>width)
		{
			scrollRange[1] = data_width - width;
		}
		if(m_hScrollBar.valid())
		{
			m_hScrollBar->setLineValue(w);
			m_hScrollBar->setPageValue(4 * w);
		}
	}
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}
const crEditBoxElement::StringArray& crEditBoxElement::getStringArray() const 
{
	return m_stringArray; 
}
crEditBoxElement::StringArray& crEditBoxElement::getStringArray() 
{ 
	return m_stringArray; 
}
std::string crEditBoxElement::getString(unsigned int i) 
{ 
	return i>=0&&!m_stringArray.empty()&&i<m_stringArray.size()?m_stringArray[i]:""; 
}
unsigned int crEditBoxElement::getNumStrings() 
{ 
	return m_stringArray.size(); 
}
void crEditBoxElement::addString(const std::string& desc) 
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_arrayMutex);
	m_stringArray.push_back(desc); 
}
std::string crEditBoxElement::getStringArrayInString()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_arrayMutex);
	std::string str;
	for( StringArray::const_iterator itr = m_stringArray.begin();
		itr != m_stringArray.end();
		++itr )
	{
		str += *itr;
	}
	//if(!str.empty())
	//    str.erase(str.size()-1);
	return str;
}

void crEditBoxElement::setStringArrayByString( const std::string &command )
{
	m_arrayMutex.lock();
	m_stringArray.clear();
    m_arrayMutex.unlock();
	//unsigned int stringLength = command.length();
	unsigned int front_of_line = 0;
	unsigned int end_of_line = 0;
	while (end_of_line<command.size())
	{
		/*if (command[end_of_line]=='\r')
		{
			addString( std::string( command, front_of_line, end_of_line-front_of_line) );

			if (end_of_line<command.size() &&
				command[end_of_line]=='\n') ++end_of_line;
			++end_of_line;
			front_of_line = end_of_line;
		}
		else */if (command[end_of_line]=='\n')
		{	
			++end_of_line;
			addString( std::string( command, front_of_line, end_of_line-front_of_line) );

			front_of_line = end_of_line;
		}
		else ++end_of_line;
	}
	if (front_of_line<end_of_line)
	{
		addString( std::string( command, front_of_line, end_of_line-front_of_line) );
	}
  //  if(command[end_of_line-1]=='\n')
		//addString( std::string( command, end_of_line-1, 1) );

	updateData();
}

bool crEditBoxElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);
 //   if(m_isMouseOnElement && mouseButtonMsg == WM_LBUTTONDOWN)
	//{
	//	int hscrollValue = 0;
	//	int vscrollValue = 0;
	//	if(m_hScrollBar.valid()) hscrollValue = m_hScrollBar->getValue();
	//	if(m_vScrollBar.valid()) vscrollValue = m_vScrollBar->getValue();

 //       int x = mx - m_rect[0] + hscrollValue;
	//	int y = my - m_rect[1] + vscrollValue;

	//	int front = 16;
	//	x /= front;
	//	y /= front;

 //       if(y<m_stringArray.size())
	//	{
 //           std::string selectedStr = m_stringArray[y];


	//	}

	//}
	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crEditBoxElement::getFocus()
{
	//if(m_focus) return;
	std::string strbuf = getStringArrayInString();
	m_focusPos = strbuf.size();
	m_hasfocurs = false;
	return crElement::getFocus();
}

void crEditBoxElement::lostFocus()
{
 //   if(m_focusCounter>= 0 && m_focusCounter<s_focusInterval)
	//{
	//	std::string strbuf = getStringArrayInString();

	//	strbuf.erase(--m_focusPos);

	//	setStringArrayByString(strbuf);
	//}
	//m_focusCounter = -s_focusInterval;

	//if(m_hasfocurs)
	//{
	//	//std::string strbuf = getStringArrayInString();
	//	//strbuf.erase(--m_focusPos);
	//	//setStringArrayByString(strbuf);
	//	//--m_focusPos;
	//	m_hasfocurs = false;
	//}
	m_hasfocurs = false;
	return crElement::lostFocus();
}

bool crEditBoxElement::isFocusOnShow()
{
	return m_hasfocurs;/*m_focus && m_focusCounter>= 0 && m_focusCounter<s_focusInterval;*/
}

bool crEditBoxElement::inputKey(int key)
{
	std::string strbuf = getStringArrayInString();
	switch(key)
	{
	case 37:
		if(isFocusOnShow()) m_focusPos--;
		if(m_focusPos>0)
		{
			int hz;
			hz = strbuf[m_focusPos-1];
			if(hz<0 && m_focusPos>1)
			{//双字节字符
				m_focusPos-=2;
			}
			else m_focusPos--;
		}
		if(isFocusOnShow())
		{
			m_focusPos++;
			m_focusPos = CRCore::maximum(m_focusPos,1);
		}
		else
			m_focusPos = CRCore::maximum(m_focusPos,0);
		break;
	case 39:
	   if(isFocusOnShow()) m_focusPos--;
		if(m_focusPos<(int)strbuf.size())
		{
			int hz;
			hz = strbuf[m_focusPos];
			if(hz<0 && m_focusPos<(int)strbuf.size()-1)
			{//双字节字符
				m_focusPos+=2;
			}
			else m_focusPos++;
		}
		if(isFocusOnShow()) 
		{
			m_focusPos++;
			m_focusPos = CRCore::minimum(m_focusPos,(int)strbuf.size()+1);
		}
		else
			m_focusPos = CRCore::minimum(m_focusPos,(int)strbuf.size());
		break;
	case 38:
		m_focusPos-=10;
		if(isFocusOnShow())
			m_focusPos = CRCore::maximum(m_focusPos,1);
		else
			m_focusPos = CRCore::maximum(m_focusPos,0);
		break;
	case 40:
		m_focusPos+=10;
		if(isFocusOnShow())
			m_focusPos = CRCore::minimum(m_focusPos,(int)strbuf.size()+1);
		else
			m_focusPos = CRCore::minimum(m_focusPos,(int)strbuf.size());
		break;
	}
	return crElement::inputKey(key);
}

bool crEditBoxElement::inputChar(wchar_t c)
{ 
	std::string strbuf = getStringArrayInString();
	bool strchanged = false;
	switch(c)
	{
	case 9://tab
	case 27://esc
		break;
	case 8:////////////////////////////BackSpace
		if(!strbuf.empty())
		{
		    if(isFocusOnShow()) m_focusPos--;
			if(m_focusPos>0)
			{
				int hz;
				hz = strbuf[m_focusPos-1];
				if(hz<0 && m_focusPos>1)
				{//双字节字符
					m_focusPos-=2;
					if(strbuf.size()>=m_focusPos+2)
					    strbuf.erase(m_focusPos,2);
				}
				else 
				{
					--m_focusPos;
                    if(strbuf.size()>=m_focusPos+1)
					    strbuf.erase(m_focusPos,1);
				}
				//CRCore::notify(CRCore::ALWAYS)<<"crEaaaaaaaa hz = "<<hz<<std::endl;
				strchanged = true;
			}
			if(isFocusOnShow()) m_focusPos++;
		}
		if(isFocusOnShow())
			m_focusPos = CRCore::maximum(CRCore::minimum(m_focusPos,(int)strbuf.size()+1),1);
		else
			m_focusPos = CRCore::maximum(CRCore::minimum(m_focusPos,(int)strbuf.size()),0);
		break;
	case 13:
		if(m_multiLine)
		{
			//c = '\n';
			if(isFocusOnShow()) m_focusPos--;
			strbuf.insert(m_focusPos,1,'\n');
			m_focusPos++;
			if(isFocusOnShow()) m_focusPos++;

			strchanged = true;
		}
		break;
	default:
		char hz[2];
		if(c>256)
		{
			hz[0]=(char)(c>>8);   
			hz[1]=(char)c;
		}
		if(isFocusOnShow()) m_focusPos--;
		if(c>256) 
		{
			strbuf.insert(m_focusPos,hz,2);
            m_focusPos+=2;
		}
		else
		{
			strbuf.insert(m_focusPos,1,c);
            m_focusPos++;
		}
		if(isFocusOnShow()) m_focusPos++;

		strchanged = true;
		break;
	}
	
    if(strchanged) setStringArrayByString(strbuf);
	//CRCore::notify(CRCore::ALWAYS)<<"crEditBoxElement str = "<<strbuf<<std::endl;
	return crElement::inputChar(c);
}

//CRCore::crVector4f  crEditBoxElement::getScrollRange()
//{
//	CRCore::crVector4f scrollRange;
//	if(!m_stringArray.empty())
//	{
//		int num_line = m_stringArray.size();
//		HDC		dc;
//		dc=CreateCompatibleDC(NULL);	
//
//		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
//		RECT rect;
//		rect.left   = m_rect[0];
//		rect.top    = m_rect[1];
//		rect.right  = getElementRight();
//		rect.bottom = getElementBottom();
//		
//		int rowHeight = DrawText(dc,"h",1,&rect,DT_INTERNAL);
//        int data_height = rowHeight * num_line;
//
//		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
//		DeleteDC(dc);
//
//		int height = m_rect[3];
//		if(data_height>height)
//		{
//			float x = (data_height - height) / float(rowHeight) + 1;
//			if(x > floor(x))
//			{
//				scrollRange[3] = floor(x) + 1;
//			}
//			else
//				scrollRange[3] = floor(x);
//		}
//	}
//
//	return scrollRange;
//}

int crEditBoxElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);	
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(/*m_focus||*/m_isMouseOnElement)
		{
			if(m_hDownImage)
				SelectObject(mdc,m_hDownImage);
			else if(m_hImage) SelectObject(mdc,m_hImage);
		}
		else if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
		}
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		//if(m_hImage)
		//{
		//	SelectObject(mdc,m_hImage);
		//	BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		//}

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		//draw data
		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		RECT rect;
		rect.left   = m_rect[0] - hscrollValue + TextRectOffset;
		rect.top    = m_rect[1] - vscrollValue + TextRectOffset;
		rect.right  = getElementRight() - TextRectOffset;
		rect.bottom = getElementBottom() - TextRectOffset;
		//DrawText(dc,m_texString.c_str(),m_texString.length(),&rect,DT_WORDBREAK);
		
		std::string strbuf = getStringArrayInString();
		if(m_focus)
		{
			/////draw focus
			CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
			float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
			if(dt>s_focusInterval)
			{
				m_time = t1;
				if(m_hasfocurs)
				{
					//strbuf.erase(--m_focusPos);
					--m_focusPos;
					m_hasfocurs = false;
				}
				else
				{
					strbuf.insert(m_focusPos++,1,'|');
					m_hasfocurs = true;
				}
			}
			else
			{
				if(m_hasfocurs)
					strbuf.insert(m_focusPos-1,1,'|');
			}
			//if(m_focusCounter == s_focusInterval)
			//{
			//	strbuf.insert(m_focusPos++,1,'|');
			//}
			//else if(m_focusCounter == 0)
			//{
			//	strbuf.erase(--m_focusPos);
			//}
			//else if(m_focusCounter == -s_focusInterval) 	
			//{
			//	m_focusCounter = s_focusInterval+1;
			//}

			//m_focusCounter--;
			//setStringArrayByString(strbuf);
		}

		/*StringArray stringArray;
		unsigned int front_of_line = 0;
		unsigned int end_of_line = 0;
		while (end_of_line<strbuf.size())
		{
			if (strbuf[end_of_line]=='\r')
			{
				stringArray.push_back(std::string( strbuf, front_of_line, end_of_line-front_of_line));
				if (end_of_line+1<strbuf.size() &&
					strbuf[end_of_line+1]=='\n') ++end_of_line;
				++end_of_line;
				front_of_line = end_of_line;
			}
			else if (strbuf[end_of_line]=='\n')
			{				
				++end_of_line;
				stringArray.push_back( std::string( strbuf, front_of_line, end_of_line-front_of_line) );
				front_of_line = end_of_line;
			}
			else ++end_of_line;
		}
		if (front_of_line<end_of_line)
		{
			stringArray.push_back( std::string( strbuf, front_of_line, end_of_line-front_of_line) );
		}
		//if(strbuf[end_of_line-1]=='\n')
		//	stringArray.push_back( std::string( strbuf, end_of_line-1, 1) );

		int height = 0;
		for( StringArray::iterator itr = stringArray.begin();
			itr != stringArray.end();
			++itr )
		{
			if(m_password)
			{
				for(int i = 0; i<itr->size(); ++i)
				{
					if((*itr)[i] != '|' && (*itr)[i] != '\n') (*itr)[i] = '*';
				}
			}

			height = DrawText(dc,(*itr).c_str(),(*itr).length(),&rect,DT_INTERNAL);
			rect.top += height;
			if(rect.top>=rect.bottom)
				break;
		}*/
		if(m_password)
		{
			for(int i = 0; i<strbuf.size(); ++i)
			{
				if((strbuf)[i] != '|' && (strbuf)[i] != '\n') (strbuf)[i] = '*';
			}
		}
        DrawText(dc,strbuf.c_str(),strbuf.length(),&rect,m_textFormat/*DT_INTERNAL*/);
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			crVector4i v4;
			if(hscrollValue != 0)
			{
				v4[0] = m_rect[0] - hscrollValue;
				v4[1] = m_rect[1];
				v4[2] = hscrollValue;
				v4[3] = m_rect[3];
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2];
				v4[3] = vscrollValue;
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crEditBoxElement::drawStageMask(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc,copy_dc;
	HBITMAP bufbmp,copybmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(/*m_focus||*/m_isMouseOnElement)
		{
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(bufdc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
			}
		}
		else if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(bufdc);
		RECT rect;
		rect.left   = m_rect[0] - hscrollValue + TextRectOffset;
		rect.top    = m_rect[1] - vscrollValue + TextRectOffset;
		rect.right  = getElementRight() - TextRectOffset;
		rect.bottom = getElementBottom() - TextRectOffset;
		//DrawText(dc,m_texString.c_str(),m_texString.length(),&rect,DT_WORDBREAK);

		if(m_focus)
		{
			/////draw focus
			std::string strbuf = getStringArrayInString();
			CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
			float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
			if(dt>s_focusInterval)
			{
				m_time = t1;
				if(m_hasfocurs)
				{
					strbuf.erase(--m_focusPos);
					m_hasfocurs = true;
				}
				else
				{
					strbuf.insert(m_focusPos++,1,'|');
					m_hasfocurs = false;
				}
			}
			//if(m_focusCounter == s_focusInterval)
			//{
			//	strbuf.insert(m_focusPos++,1,'|');
			//}
			//else if(m_focusCounter == 0)
			//{
			//	strbuf.erase(--m_focusPos);
			//}
			//else if(m_focusCounter == -s_focusInterval) 	
			//{
			//	m_focusCounter = s_focusInterval+1;
			//}

			//m_focusCounter--;
			setStringArrayByString(strbuf);
		}

		int height = 0;
		for( StringArray::iterator itr = m_stringArray.begin();
			itr != m_stringArray.end();
			++itr )
		{
			if(m_password)
			{
				for(int i = 0; i<itr->size(); ++i)
				{
					if((*itr)[i] != '|') (*itr)[i] = '*';
				}
			}
			height = DrawText(bufdc,(*itr).c_str(),(*itr).length(),&rect,m_textFormat/*DT_WORDBREAK*/);
			rect.top += height;
			if(rect.top>=rect.bottom)
				break;
		}
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(bufdc);

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			crVector4i v4;
			if(hscrollValue != 0)
			{
				v4[0] = m_rect[0] - hscrollValue;
				v4[1] = m_rect[1];
				v4[2] = hscrollValue;
				v4[3] = m_rect[3];
				BitBlt(bufdc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2];
				v4[3] = vscrollValue;
				BitBlt(bufdc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}
    }
	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crComboBoxElement
//
//////////////////////////////////////////////////////////////////////////
crComboBoxElement::crComboBoxElement(const crComboBoxElement& element):
crElement(element)
{
}

crComboBoxElement::~crComboBoxElement()
{
	//m_comboEditBox->setParentElement(NULL);
	//m_comboButton->setParentElement(NULL);
	//m_comboList->setParentElement(NULL);
}

void crComboBoxElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	cfg_script.Get("ComboEditBoxName",m_comboEditBoxName);
	cfg_script.Get("ComboButtonName",m_comboButtonName);
	cfg_script.Get("ComboListBoxName",m_comboListBoxName);
	cfg_script.Get("ComboScrollBarName",m_comboScrollBarName);
	crElement::load(cfg_script,widthScale,heightScale);
}

void crComboBoxElement::initWindow()
{
    m_comboEditBox = dynamic_cast<crEditBoxElement *>(m_parentStage->getElement(m_comboEditBoxName));
	if(!m_comboEditBox.valid())
	{
		CRCore::notify(CRCore::WARN)<<"crComboBoxElement::initWindow(): ComboEditBox("<<this->getName()<<") is empty. StageNameID = "<<m_parentStage->getName()<<std::endl;
	}

	m_comboButton = dynamic_cast<crButtonElement *>(m_parentStage->getElement(m_comboButtonName));
	if(!m_comboButton.valid())
	{
		CRCore::notify(CRCore::WARN)<<"crComboBoxElement::initWindow(): ComboButton("<<this->getName()<<") is empty. StageNameID = "<<m_parentStage->getName()<<std::endl;
	}

	m_comboList = dynamic_cast<crListBoxElement *>(m_parentStage->getElement(m_comboListBoxName));
	if(!m_comboList.valid())
	{
		CRCore::notify(CRCore::WARN)<<"crComboBoxElement::initWindow(): ComboListBox("<<this->getName()<<") is empty. StageNameID = "<<m_parentStage->getName()<<std::endl;
	}

	m_comboScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_comboScrollBarName));
	//if(!m_comboScrollBar.valid())
	//{
	//	CRCore::notify(CRCore::WARN)<<"crComboBoxElement::initWindow(): ComboScrollBar("<<this->getName()<<") is empty. StageNameID = "<<m_parentStage->getName()<<std::endl;
	//}

	m_comboList->setShow(false);
	m_comboList->setEnable(false);
	m_comboList->setCanCapture(true);
	m_comboList->setUseLeftBtnSelect(false);

	if(m_comboScrollBar.valid())
	{
		m_comboScrollBar->setShow(false);
		m_comboScrollBar->setEnable(false);
	}

	//init select
	std::string data;
	m_comboList->getData(m_comboList->getSelect(),data);
	m_comboEditBox->setStringArrayByString(data);

	crElement::initWindow();
}

void crComboBoxElement::select(int index)
{
	m_comboList->select(index);
	std::string data;
	m_comboList->getData(m_comboList->getSelect(),data);
	m_comboEditBox->setStringArrayByString(data);
}

std::string crComboBoxElement::getValue()
{
    return m_comboEditBox->getStringArrayInString();
}

int crComboBoxElement::getSelectIndex()
{ 
	return m_comboList->getSelect(); 
}

void crComboBoxElement::updateParentElement(crElement *element)
{
	bool isMouseOn = element->getMouseOnElement();
	int mouseButtonMsg = m_parentStage->getCurrentMouseButtonMsg();
	if(m_comboButton.get() == element)
	{
        if(isMouseOn && mouseButtonMsg == WM_LBUTTONDOWN)
		{//显示listbox
			m_comboList->setShow(true);
			m_comboList->setEnable(true);
			if(m_comboScrollBar.valid())
			{
				m_comboScrollBar->setShow(true);
				m_comboScrollBar->setEnable(true);
			}
			m_parentStage->setFocusElement(m_comboList.get());
		}
	}
	else if(m_comboEditBox.get() == element)
	{
		if(!m_comboEditBox->getCanFocus())
		{
			if(isMouseOn && mouseButtonMsg == WM_LBUTTONDOWN)
			{
				m_comboList->setShow(true);
				m_comboList->setEnable(true);
				if(m_comboScrollBar.valid())
				{
					m_comboScrollBar->setShow(true);
					m_comboScrollBar->setEnable(true);
				}
				m_parentStage->setFocusElement(m_comboList.get());
			}
		}
		else
		{//可接受输入，根据输入单词去list搜索
			//std::string data = m_comboEditBox->getStringArrayInString();
		}
	}
	else if(m_comboList.get() == element)
	{
		if(mouseButtonMsg == WM_LBUTTONDOWN)
		{
			if(isMouseOn)
			{//
                std::string data;
				m_comboList->getData(m_comboList->getSelect(),data);
                m_comboEditBox->setStringArrayByString(data);
			}
			m_comboList->setShow(false);
			m_comboList->setEnable(false);
			if(m_comboScrollBar.valid())
			{
				m_comboScrollBar->setShow(false);
				m_comboScrollBar->setEnable(false);
			}
			m_parentStage->lostFocusElement(m_comboList.get());
		}
	}
	crElement::updateParentElement(this);
}

//////////////////////////////////////////////////////////////////////////
//
//crProgressElement
//
//////////////////////////////////////////////////////////////////////////
crProgressElement::crProgressElement():
m_progressBarType(HORIZONTALBAR),
m_progress(0.0f)
{
}
crProgressElement::crProgressElement(const crProgressElement& element):
crElement(element),
m_progressBarType(element.m_progressBarType),
m_progress(0.0f)
{

}

void crProgressElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::load(cfg_script, widthScale, heightScale);
}

bool crProgressElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

void crProgressElement::updateProgress(float rate)
{
	m_progress += rate;
	m_progress = CRCore::clampTo(m_progress,0.0f,1.0f);
	m_parentStage->setNeedReDraw();
}
//void crProgressElement::updateProgress(const CRCore::crVector2f& m_rate)
//{
//	m_progress[0] = float(m_progress[0]) + m_rate[0];
//	m_progress[1] = float(m_progress[1]) + m_rate[1];
//	m_parentStage->setNeedReDraw();
//}
void crProgressElement::setProgress(float progress)
{
	m_progress = progress;
	m_progress = CRCore::clampTo(m_progress,0.0f,1.0f);
	m_parentStage->setNeedReDraw();
}
float crProgressElement::getProgress()
{
	return m_progress;
}
void crProgressElement::resetProgress()
{
	//m_progress.set(0,0);
	m_progress = 0.0f;
}

void crProgressElement::fetchEnd()
{
	//m_progress.set(m_rect[2],m_rect[3]);
	m_progress = 1.0f;
}

bool crProgressElement::isProgressFetchEnd()
{
	//return m_progress[0]>=m_rect[2]||m_progress[1]>=m_rect[3];
	return m_progress == 1.0f;
}

int crProgressElement::drawStageCopy(Display dc, Display down_dc)
{
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(dc);
	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}
	if(m_hDownImage)
	{
		SelectObject(mdc,m_hDownImage);
		if(m_progressBarType == HORIZONTALBAR)
			BitBlt(dc,m_rect[0],m_rect[1],m_progress*m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		else
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_progress*m_rect[3],mdc,0,0,SRCCOPY);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crProgressElement::drawStageMask(Display dc, Display down_dc)
{
	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}
	if(m_hDownImage)
	{
		SelectObject(mdc,m_hDownImage);
		if(m_progressBarType == HORIZONTALBAR)
			BitBlt(bufdc,m_rect[0],m_rect[1],m_progress*m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		else
			BitBlt(bufdc,m_rect[0],m_rect[1],m_rect[2],m_progress*m_rect[3],mdc,0,0,SRCCOPY);
		//BitBlt(bufdc,0,0,m_progress[0],m_progress[1],mdc,0,0,SRCCOPY);
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);
	return noNeedFatherDraw;
}

//////////////////////////////////////////////////////////////////////////
//
//crPixelTestElement
//
//////////////////////////////////////////////////////////////////////////
crPixelTestElement::crPixelTestElement(const crPixelTestElement& element):
crElement(element)
{

}

void crPixelTestElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::load(cfg_script, widthScale, heightScale);
	std::string str;
	std::vector<float> v_i;
	crVector2i imageSize;
	crVector2i elementPos;
	if (cfg_script.Push("PixelColorImage")) 
	{
		cfg_script.Get("FileName", str);
		cfg_script.Get("ImageSize", v_i);
		imageSize.set(v_i[0] * widthScale, v_i[1] * heightScale);
		cfg_script.Get("ElementPosOnImage", v_i);
		elementPos.set(v_i[0] * widthScale,v_i[1] * heightScale);
		setElementPixelColorImage(str,imageSize,elementPos);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	}
}

void crPixelTestElement::setElementPixelColorImage( const std::string& image, const crVector2i& imageSize, const crVector2i& elementPos )
{
	m_pixelColorImageName = image; 
	if(m_hPixelColorImage)
		DeleteObject(m_hPixelColorImage);
	//HBITMAP hLoadedImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),m_disableImageName.c_str(),IMAGE_BITMAP,imageSize[0],imageSize[1],LR_LOADFROMFILE);
	HBITMAP hLoadedImage = getOrLoadBmp(m_pixelColorImageName,imageSize);

	HDC dcDest=CreateCompatibleDC(NULL);
	HDC dcSrc=CreateCompatibleDC(NULL);

	HWND desktopWnd = GetDesktopWindow();
	HDC safeDC= GetDC(desktopWnd);
	m_hPixelColorImage = CreateCompatibleBitmap(safeDC,m_rect[2],m_rect[3]);
	SelectObject(dcDest,m_hPixelColorImage);
	SelectObject(dcSrc,hLoadedImage);

	BitBlt(dcDest,0,0,m_rect[2],m_rect[3],dcSrc,elementPos[0],elementPos[1],SRCCOPY);

	//DeleteObject(hLoadedImage);
	DeleteDC(dcDest);
	DeleteDC(dcSrc);
	ReleaseDC(desktopWnd,safeDC);
	//DeleteDC(safeDC);
}

bool crPixelTestElement::nearColor(COLORREF c1, COLORREF c2, int error)
{
    return fabs(float(GetRValue(c1) - GetRValue(c2))) < error && 
	       fabs(float(GetGValue(c1) - GetGValue(c2))) < error &&
	       fabs(float(GetBValue(c1) - GetBValue(c2))) < error;
}

bool crPixelTestElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = false;
	if(pressed(mx,my))
	{
		//if(mouseButtonMsg == WM_LBUTTONDOWN)
		//{
			HDC dc=CreateCompatibleDC(NULL);
			SelectObject(dc,m_hPixelColorImage);
			m_currentPixelColor = ::GetPixel(dc, mx - m_rect[0], my - m_rect[1]);
			if(!nearColor(m_currentPixelColor,RGB(255, 255, 255),5))
			{
				m_isMouseOnElement = true;
				//CRCore::notify(CRCore::ALWAYS)<<"crPixelTestElement GetPixel = "<<crVector3i(GetRValue(m_currentPixelColor),GetGValue(m_currentPixelColor),GetBValue(m_currentPixelColor))<<std::endl;
			}
			DeleteDC(dc);
		//}
	}
	return crElement::testButtons(mx,my,mouseButtonMsg);
}

int crPixelTestElement::drawStageCopy(Display dc, Display down_dc)
{
	HDC		mdc;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(/*m_focus||*/m_isMouseOnElement)
	{
		if(m_hDownImage)
		{
			HDC bufdc=CreateCompatibleDC(NULL);
			HBITMAP bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
			SelectObject(bufdc,bufbmp);
			SelectObject(mdc,m_hDownImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);

		    SelectObject(mdc,m_hPixelColorImage);
			SetBkColor(mdc, m_currentPixelColor); // 设置彩色位图背景色为当前拾取的颜色

			HDC maskdc=CreateCompatibleDC(NULL);
			HBITMAP hMaskBMP = CreateBitmap(m_rect[2],m_rect[3], 1, 1, NULL); // 建立单色位图
			SelectObject(maskdc,hMaskBMP);
			BitBlt(maskdc, 0, 0, m_rect[2],m_rect[3], mdc, 0, 0, SRCCOPY); // 拷贝到maskdc
			BitBlt(maskdc, 0, 0, m_rect[2],m_rect[3], maskdc, 0, 0, NOTSRCCOPY); //取反

			//设置背景色为黑色，前景色为白色，将掩码位图与DownImage进行“与”运算
			SetBkColor(bufdc, RGB(0,0,0));
			SetTextColor(bufdc, RGB(255,255,255));
			BitBlt(bufdc, 0, 0, m_rect[2], m_rect[3], maskdc, 0, 0, SRCAND);
              
			//设置背景色为白色，前景色为黑色，将掩码位图与背景图进行“与”运算
			SetBkColor(dc,RGB(255,255,255));
			SetTextColor(dc,RGB(0,0,0));
			BitBlt(dc, m_rect[0],m_rect[1],m_rect[2],m_rect[3], maskdc, 0, 0, SRCAND);

			//将bufdc与背景进行“或”运算 
			BitBlt(dc, m_rect[0],m_rect[1],m_rect[2],m_rect[3], bufdc, 0, 0, SRCPAINT);

			DeleteDC(bufdc);
			DeleteDC(maskdc);
			DeleteObject(bufbmp);
			DeleteObject(hMaskBMP);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else if(m_hImage)
	{
		SelectObject(mdc,m_hImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
	}

	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crPixelTestElement::drawStageMask(Display dc, Display down_dc)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////
//
//crChooseColorElement
//
//////////////////////////////////////////////////////////////////////////
crChooseColorElement::crChooseColorElement(const crChooseColorElement& element):
crElement(element)
{

}

void crChooseColorElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	crElement::load(cfg_script, widthScale, heightScale);
}

bool crChooseColorElement::testButtons(int mx, int my, int mouseButtonMsg)
{
	m_isMouseOnElement = pressed(mx,my);

	return crElement::testButtons(mx,my,mouseButtonMsg);
}

COLORREF crChooseColorElement::getColor()
{
	return m_color;
}

void crChooseColorElement::setColor(COLORREF color)
{
	m_color = color;
}

int crChooseColorElement::drawStageCopy(Display dc, Display down_dc)
{
	bool noNeedFatherDraw = true;

    HBRUSH hbrush = CreateSolidBrush(m_color);
	RECT rect;
	rect.left   = m_rect[0]+1;
	rect.top    = m_rect[1]+1;
	rect.right  = getElementRight();
	rect.bottom = getElementBottom();
	FillRect(dc,&rect,hbrush);

	DeleteObject((HGDIOBJ)hbrush);   
	return noNeedFatherDraw;
}

int crChooseColorElement::drawStageMask(Display dc, Display down_dc)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////
//
//crKeyValueNode
//
//////////////////////////////////////////////////////////////////////////
crKeyValueNode::crKeyValueNode(const std::string &key, const std::string &value)
{
	m_key = key;
	m_value = value;
}
crKeyValueNode::~crKeyValueNode()
{
}
crKeyValueNode::crKeyValueNode(const crKeyValueNode& node):
m_key(node.m_key),
m_value(node.m_value)
{
}
void crKeyValueNode::setKey(const std::string &key)
{
	m_key = key;
}
void crKeyValueNode::setValue(const std::string &value)
{
	m_value = value;
}
const std::string &crKeyValueNode::getKey()
{
	return m_key;
}
const std::string &crKeyValueNode::getValue()
{
	return m_value;
}
//////////////////////////////////////////////////////////////////////////
//
//crKeyValueGridElement
//
//////////////////////////////////////////////////////////////////////////
crKeyValueGridElement::crKeyValueGridElement(const crKeyValueGridElement& element):
crElement(element),
m_dataVec(element.m_dataVec),
m_select(element.m_select),
m_useLeftBtnSelect(element.m_useLeftBtnSelect),
m_rowHeight(element.m_rowHeight),
m_colWidth(element.m_colWidth),
m_textAttribute(element.m_textAttribute),
m_hScrollBarName(element.m_hScrollBarName),
m_vScrollBarName(element.m_vScrollBarName)
{
}

CRCore::crVector4f crKeyValueGridElement::getScrollRange()
{
	float data_height = m_rowHeight * getDataSize();
	float height = m_rect[3];
	CRCore::crVector4f scrollRange;
	if(data_height > height)
	{
		float x = (data_height - height) / float(m_rowHeight);
		if(x > floor(x))
		{
			scrollRange[3] = floor(x) + 1;
		}
		else
			scrollRange[3] = floor(x);
	}
	return scrollRange;
}

void crKeyValueGridElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	int int1;
	if(cfg_script.Get("TextAttribute", str))
	{
        setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	int nData = 1;
	std::string key,value;
	while (cfg_script.Push("Data", nData++))
	{
		cfg_script.Get("Key", key);
		cfg_script.Get("Value", value);
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crKeyValueGridElement::load(): "<<cfg_script.GetLastError()<<std::endl;
	    addData(key,value,"");
	}

	if(cfg_script.Get("RowHeight", int1))
		setRowHeight((float)int1/* * heightScale*/);
	if(cfg_script.Get("ColWidth", int1))
		setColWidth((float)int1/* * widthScale*/);
	
	if(cfg_script.Get("Select", int1))
		m_select = int1;
	if(cfg_script.Get("UseLeftBtnSelect", int1))
		setUseLeftBtnSelect(int1);

	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);
	cfg_script.Get("InputElementName",m_inputElementName);
	crElement::load(cfg_script,widthScale,heightScale);
}

void crKeyValueGridElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	m_inputElement = dynamic_cast<crEditBoxElement *>(m_parentStage->getElement(m_inputElementName));

	crElement::initWindow();
    int vscrollValue = 0;
	int _select;
	if(m_vScrollBar.valid())
	{
		crVector4f scrollRange = this->getScrollRange();
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));

		vscrollValue = m_vScrollBar->getValue();
		_select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_vScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_vScrollBar->setValue(vscrollValue);
		}
	}
	updateInputElement(true);
}

void crKeyValueGridElement::updateInputElement(bool flg)
{
	if(m_inputElement.valid())
	{
		if(isDataEmpty())
		{
            m_inputElement->clearString();
			if(m_inputElement->isFocus()) m_inputElement->lostFocus();
		}
		else if(m_select>=0 && m_select < m_dataVec.size())
		{
			crKeyValueNode* node = m_dataVec[m_select].get();
			if(node)
			{
				if(flg)
				{
					m_inputElement->setStringArrayByString(node->getValue());
					//m_inputElement->setFocus();
				}
				else
				{
					//data.second = m_inputElement->getStringArrayInString();
					node->setValue(m_inputElement->getStringArrayInString());
				}
			}
		}
	}
}

void crKeyValueGridElement::updateData()
{
	checkSelect();
	crElement::updateData();

	if(m_vScrollBar.valid())
	{
		crVector4f scrollRange = this->getScrollRange();
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));

		int vscrollValue = m_vScrollBar->getValue();
		int _select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_vScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_vScrollBar->setValue(vscrollValue);
		}
	}
}

bool crKeyValueGridElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	bool test = false;
	if(getCapture())
	{
		if(m_vScrollBar.valid())
		    test |= m_vScrollBar->testButtons(mx,my,mouseButtonMsg);
		if(m_hScrollBar.valid())
			test |= m_hScrollBar->testButtons(mx,my,mouseButtonMsg);
	}
	if(!test)
	{
		if(pressed(mx,my))
		{
			int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;

			m_isMouseOnElement = true;
			updateInputElement(false);
			if(!m_useLeftBtnSelect || mouseButtonMsg == WM_LBUTTONDOWN)
			{
				int i = (my-m_rect[1])/m_rowHeight + vscrollValue;
				if(i>=0 && i < getDataSize() && m_select != i)
				{
					select(i);
				}
			}
		}
		else
		{
			m_isMouseOnElement = false;
		}
		test = crElement::testButtons(mx,my,mouseButtonMsg);
	}

	return test; 
}

int crKeyValueGridElement::getMaxRowCanBeDisplay()
{
	float x = float(m_rect[3])/float(m_rowHeight);
	return x > floor(x)?floor(x)-1:floor(x)-2;
}

int crKeyValueGridElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	vscrollValue = CRCore::clampTo(vscrollValue,0,(int)(m_dataVec.size()));
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		int _select = m_select - vscrollValue;
		if(m_select>-1)
		{
			if(_select<0)
			{
				_select = 0;
				//m_select = vscrollValue;
				select(vscrollValue);
			}
			else
			{
				int maxrow = getMaxRowCanBeDisplay();
				if(_select>maxrow) _select = maxrow;
				//m_select = vscrollValue + _select;
				select(vscrollValue + _select);
			}
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,mdc,0,0,SRCCOPY);
			}
			else	//need draw by parent
			{
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,down_dc,m_rect[0],m_rect[1]+_select*m_rowHeight,SRCCOPY);
			}
		}
		//draw data
		m_dataMutex.lock();
		if(!m_dataVec.empty())
		{
			//if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
			int i = 0;
			RECT rect;
			KeyValueNodeVec::iterator itr = m_dataVec.begin();
			while (i++<vscrollValue)
			{
				itr++;
			}
			i=0;
			POINT point;
			int _colWidth = m_colWidth-hscrollValue;
			if(_colWidth>0)
			{
				MoveToEx(dc,m_rect[0]+_colWidth,m_rect[1],&point);
				LineTo(dc,m_rect[0]+_colWidth,getElementBottom());
			}
			if(m_inputElement.valid()) 
			{
				int __colWidth = CRCore::maximum(0,_colWidth);
				m_inputElement->setRect(m_rect[0]+__colWidth,m_rect[1]+_select*m_rowHeight,m_rect[2]-__colWidth,m_rowHeight);
			}
			crTextAttribute *textAttr;
			crKeyValueNode *node;
			for( ;
				itr != m_dataVec.end();
				++itr, ++i )
			{
				rect.top    = m_rect[1]+i*m_rowHeight + TextRectOffset;
				rect.bottom = m_rect[1]+(i+1)*m_rowHeight - TextRectOffset;
				if(rect.bottom>getElementBottom()) break;

				node = itr->get();
				if(!node)
					continue;
				textAttr = node->getTextAttribute();
				if(!textAttr)
					textAttr = m_textAttribute.get();
				if(textAttr) textAttr->drawTextAttribute(dc);

				if(_colWidth>0)
				{
					rect.left   = m_rect[0] + TextRectOffset;
					rect.right  = m_rect[0]+_colWidth - TextRectOffset;
					DrawText(dc,node->getKey().c_str(),node->getKey().length(),&rect,DT_WORD_ELLIPSIS);
				}

				if(i!=_select)
				{
					rect.left   = m_rect[0]+_colWidth + TextRectOffset;
					rect.right  = getElementRight() - TextRectOffset;
					DrawText(dc,node->getValue().c_str(),node->getValue().length(),&rect,DT_WORD_ELLIPSIS);
				}
				rect.bottom += TextRectOffset;
				MoveToEx(dc,m_rect[0],rect.bottom,&point);
				LineTo(dc,getElementRight(),rect.bottom);
				//ExtTextOut(dc,rect.left,rect.top,ETO_OPAQUE,&rect,itr->c_str(),itr->length(),NULL);

				//TextOut(hdc,m_rect[0]+m_texOffset[0],m_rect[1]+i*m_rowHeight+m_texOffset[1],itr->c_str(),itr->length());

				if(textAttr) textAttr->endDrawText(dc);
			}
			//if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
		}
		m_dataMutex.unlock();
		if(hscrollValue != 0 || vscrollValue != 0)
		{
			crVector4i v4;
			if(hscrollValue != 0)
			{
				v4[0] = m_rect[0] - hscrollValue;
				v4[1] = m_rect[1];
				v4[2] = hscrollValue;
				v4[3] = m_rect[3];
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2];
				v4[3] = vscrollValue;
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}

		//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crKeyValueGridElement::drawStageMask(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;

	HDC		bufdc,mdc;
	HBITMAP bufbmp;
	bool noNeedFatherDraw = true;

	bufdc=CreateCompatibleDC(NULL);
	mdc=CreateCompatibleDC(NULL);
	bufbmp = CreateCompatibleBitmap(dc,m_rect[2],m_rect[3]);
	SelectObject(bufdc,bufbmp);

	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(bufdc,0,0,m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		int _select = m_select - vscrollValue;
		if(m_select>-1)
		{
			if(_select<0)
			{
				_select = 0;
				m_select = vscrollValue;
			}
			else
			{
				int maxrow = getMaxRowCanBeDisplay();
				if(_select>maxrow) _select = maxrow;
				m_select = vscrollValue + _select;
			}
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(bufdc,0,_select*m_rowHeight,m_rect[2],m_rowHeight,mdc,0,0,SRCCOPY);
			}
			else	//need draw by parent
			{
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,down_dc,m_rect[0],m_rect[1]+_select*m_rowHeight,SRCCOPY);
			}
		}
		//draw data
		//if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(bufdc);
		int i = 0;
		RECT rect;
		m_dataMutex.lock();
		KeyValueNodeVec::iterator itr = m_dataVec.begin();
		while (i++<vscrollValue)
		{
			itr++;
		}
		i=0;
		POINT point;
		int _colWidth = m_colWidth-hscrollValue;
		if(_colWidth>0)
		{
			MoveToEx(dc,m_rect[0]+_colWidth,m_rect[1],&point);
			LineTo(dc,m_rect[0]+_colWidth,getElementBottom());
		}
		if(m_inputElement.valid()) 
		{
			int __colWidth = CRCore::maximum(0,_colWidth);
			m_inputElement->setRect(m_rect[0]+__colWidth,m_rect[1]+_select*m_rowHeight,m_rect[2]-__colWidth,m_rowHeight);
		}
		crTextAttribute *textAttr;
		crKeyValueNode *node;
		for( ;
			itr != m_dataVec.end();
			++itr, ++i )
		{
			rect.top    = m_rect[1]+i*m_rowHeight + TextRectOffset;
			rect.bottom = m_rect[1]+(i+1)*m_rowHeight - TextRectOffset;
			if(rect.bottom>getElementBottom()) break;

			node = itr->get();
			if(!node)
				continue;
			textAttr = node->getTextAttribute();
			if(!textAttr)
				textAttr = m_textAttribute.get();
			if(textAttr) textAttr->drawTextAttribute(dc);

			if(_colWidth>0)
			{
				rect.left   = m_rect[0] + TextRectOffset;
				rect.right  = m_rect[0]+_colWidth - TextRectOffset;
				DrawText(dc,node->getKey().c_str(),node->getKey().length(),&rect,DT_WORD_ELLIPSIS);
			}

			if(i!=_select)
			{
				rect.left   = m_rect[0]+_colWidth + TextRectOffset;
				rect.right  = getElementRight() - TextRectOffset;
				DrawText(dc,node->getValue().c_str(),node->getValue().length(),&rect,DT_WORD_ELLIPSIS);
			}
			rect.bottom += TextRectOffset;
			MoveToEx(dc,m_rect[0],rect.bottom,&point);
			LineTo(dc,getElementRight(),rect.bottom);
			//ExtTextOut(dc,rect.left,rect.top,ETO_OPAQUE,&rect,itr->c_str(),itr->length(),NULL);

			//TextOut(hdc,m_rect[0]+m_texOffset[0],m_rect[1]+i*m_rowHeight+m_texOffset[1],itr->c_str(),itr->length());

			if(textAttr) textAttr->endDrawText(dc);
		}
		//if(m_textAttribute.valid()) crTextAttribute::endDrawText(bufdc);
		m_dataMutex.unlock();
	}

	if(m_hMaskImage)
	{
		SelectObject(mdc,m_hMaskImage);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCAND);
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCPAINT);
	}
	else 
		BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);

	DeleteDC(mdc);
	DeleteDC(bufdc);
	DeleteObject(bufbmp);

	return noNeedFatherDraw;
}

void crKeyValueGridElement::clearData()
{
	m_dataMutex.lock();
	m_dataVec.clear();
	m_dataMutex.unlock();
	m_inputElement->clearString();
	if(m_inputElement->isFocus()) m_inputElement->lostFocus();
	//m_select = 0;
}

void crKeyValueGridElement::addData(const std::string& key,const std::string &value, const std::string &texattr)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	crKeyValueNode *node = new crKeyValueNode(key,value);
	if(!texattr.empty())
	{
		node->setTextAttribute(crTextAttribute::getTextAttribute(texattr));
	}
	m_dataVec.push_back(node);
}

crKeyValueGridElement::KeyValueNodeVec &crKeyValueGridElement::getDataList()
{
	return m_dataVec;
}

int crKeyValueGridElement::findDataIndex(const std::string &key)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	int i = 0;
	for( KeyValueNodeVec::iterator itr = m_dataVec.begin();
		 itr != m_dataVec.end();
		 ++itr,i++ )
	{
		if((*itr)->getKey().compare(key) == 0)
		{
			return i;
		}
	}
	return -1;
}

void crKeyValueGridElement::setRowHeight(int height)
{
	m_rowHeight = height;
}

int crKeyValueGridElement::getRowHeight()
{
	return m_rowHeight;
}

void crKeyValueGridElement::setColWidth(int width)
{
	m_colWidth = width;
}

int crKeyValueGridElement::getColWidth()
{
	return m_colWidth;
}

void crKeyValueGridElement::insertData(unsigned int index, const std::string& key,const std::string &value, const std::string &texattr)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	ref_ptr<crKeyValueNode> node = new crKeyValueNode(key,value);
	if(!texattr.empty())
	{
		node->setTextAttribute(crTextAttribute::getTextAttribute(texattr));
	}
	if (index >= m_dataVec.size())
	{
		m_dataVec.push_back(node.get());
	}
	else
	{
		m_dataVec.insert(m_dataVec.begin()+index, node.get());
	}
}
crKeyValueNode * crKeyValueGridElement::getData(unsigned int index)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if(index < m_dataVec.size())
	{
	    return m_dataVec[index].get();
	}
	return NULL;
}
void crKeyValueGridElement::select(int index)
{
	if(m_select!=index)
	{
		updateInputElement(false);
		m_select = index;
		updateData();
		updateInputElement(true);
	}
}

int crKeyValueGridElement::getSelect()
{
	return m_select;
}

void crKeyValueGridElement::setUseLeftBtnSelect(bool leftBtnSelect)
{
    m_useLeftBtnSelect = leftBtnSelect;
}

void crKeyValueGridElement::checkSelect()
{
	if(isDataEmpty())
		m_select = 0;
	else
	{
		int size = getDataSize() - 1;
		if(m_select>size) m_select = size;
	}
}
int crKeyValueGridElement::getDataSize() 
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataVec.size(); 
}
bool crKeyValueGridElement::isDataEmpty()
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataVec.empty();
}
//////////////////////////////////////////////////////////////////////////
//
//crTreeNode
//
//////////////////////////////////////////////////////////////////////////
crTreeNode::crTreeNode():
m_selected(false),
m_expand(false),
m_currentBitmap(0),
m_data(NULL),
m_rowHeight(20),
m_columnOffset(5),
m_elementWidth(100),
m_parent(NULL)
{

}
crTreeNode::~crTreeNode()
{
    clearBitMap();
	m_parent = NULL;
}
crTreeNode::crTreeNode(const crTreeNode& treenode):
m_title(treenode.m_title),
m_selected(false),
m_expand(false),
m_bitmapArray(treenode.m_bitmapArray),
m_currentBitmap(treenode.m_currentBitmap),
m_data(treenode.m_data),
m_textAttribute(treenode.m_textAttribute),
m_rowHeight(treenode.m_rowHeight),
m_columnOffset(treenode.m_columnOffset),
m_elementWidth(treenode.m_elementWidth)
{
	for( TreeNodeArray::const_iterator itr = treenode.m_childArray.begin();
		 itr != treenode.m_childArray.end();
		 ++itr )
	{
		addChild((*itr)->clone());
	}
}

void crTreeNode::setRowHeight(int rowHeight)
{
    m_rowHeight = rowHeight;
}

void crTreeNode::setColumnOffset(int columnOffset)
{
    m_columnOffset = columnOffset;
}

void crTreeNode::setElementWidth(int elementWidth)
{
    m_elementWidth = elementWidth;
}

int crTreeNode::getRowHeight()
{
	return m_rowHeight;
}
int crTreeNode::getColumnOffset()
{
	return m_columnOffset;
}
int crTreeNode::getElementWidth()
{
	return m_elementWidth;
}

void crTreeNode::setTitle(const std::string &title)
{
	m_title = title;
}
const std::string &crTreeNode::getTitle() const
{
	return m_title;
}
void crTreeNode::setSelect(bool select)
{
    m_selected = select;
}
bool crTreeNode::getSelect()
{
	return m_selected;
}
void crTreeNode::expand()
{
	m_expand = !m_expand;
}
bool crTreeNode::getExpand()
{
	return m_expand;
}
void crTreeNode::addBitMap(HBITMAP bitmap)
{
	m_bitmapArray.push_back(bitmap);
}
void crTreeNode::clearBitMap()
{
	for( BitMapArray::iterator itr = m_bitmapArray.begin();
		itr != m_bitmapArray.end();
		++itr )
	{
		DeleteObject(*itr);	
	}
	m_bitmapArray.clear();
}
void crTreeNode::setCurrentBitmap(unsigned char index)
{
	if(index<m_bitmapArray.size())
	    m_currentBitmap = index;
}
unsigned char crTreeNode::getCurentBitmap()
{
	return m_currentBitmap;
}
void crTreeNode::setData(void *data)
{
	m_data = data;
}
void *crTreeNode::getData()
{
	return m_data;
}
void crTreeNode::addChild(crTreeNode *child)
{
	m_childArray.push_back(child);
	child->setParent(this);
}
void crTreeNode::setParent(crTreeNode *parent)
{
	m_parent = parent;
}
crTreeNode *crTreeNode::getParent()
{
    return m_parent;
}
crTreeNode *crTreeNode::getChild(unsigned int index)
{
	if(index<m_childArray.size())
		return m_childArray[index].get();
	return NULL;
}
crTreeNode::TreeNodeArray &crTreeNode::getChildArray()
{
	return m_childArray;
}
unsigned int crTreeNode::getChildIndex( const crTreeNode* node ) const
{
	for (unsigned int childNum = 0;childNum < m_childArray.size();++childNum)
	{
		if (m_childArray[childNum] == node) return childNum;
	}
	return m_childArray.size(); // node not found.
}
void crTreeNode::removeChild(unsigned int index)
{
	if(index<m_childArray.size())
		m_childArray.erase(m_childArray.begin()+index);
}
void crTreeNode::removeChild(crTreeNode *child)
{
	removeChild(getChildIndex(child));
}

crTreeNode *crTreeNode::findChild(const std::string &title)
{
	for( TreeNodeArray::iterator itr = m_childArray.begin();
		 itr != m_childArray.end();
		 ++itr )
	{
		if( (*itr)->getTitle().compare(title) == 0 )
		{
			return itr->get();
		}
	}
	return NULL;
}
crTreeNode *crTreeNode::findChildByData(void *data)
{
	for( TreeNodeArray::iterator itr = m_childArray.begin();
		itr != m_childArray.end();
		++itr )
	{
		if( (*itr)->getData() == data)
		{
			return itr->get();
		}
	}
	return NULL;
}
void crTreeNode::removeAllChild()
{
	m_childArray.clear();
}

void crTreeNode::rootDraw(Display dc,int xpos,int ypos,int elementBottom)
{
	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	for( TreeNodeArray::iterator itr = m_childArray.begin();
		itr != m_childArray.end();
		++itr )
	{
		(*itr)->draw(dc,xpos,ypos,elementBottom);
	}
	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
}
void crTreeNode::draw(Display dc,int& xpos,int& ypos,int elementBottom)
{
	if(ypos+m_rowHeight/*-TextRectOffset*/>=elementBottom) return;

	RECT rect;
	rect.left   = xpos;
	rect.top    = ypos;
	rect.right  = m_elementWidth;
	rect.bottom = rect.top + m_rowHeight;

	if(m_selected)
	{
        int bkmode = GetBkMode(dc);
		HBRUSH brush = CreateSolidBrush(RGB(0,0,128));
		FillRect(dc,&rect,brush);
		SetBkMode(dc,bkmode);
		DeleteObject((HGDIOBJ)brush);
	}

	if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
	if(!m_title.empty())
	{
		rect.left += TextRectOffset;
        rect.top += TextRectOffset;
        rect.right -= TextRectOffset;
		rect.bottom -= TextRectOffset;
		DrawText(dc,m_title.c_str(),m_title.length(),&rect,DT_WORD_ELLIPSIS);
	}
	ypos+=m_rowHeight;

    if(m_expand)
	{
        xpos+=m_columnOffset;
		for( TreeNodeArray::iterator itr = m_childArray.begin();
			 itr != m_childArray.end();
			 ++itr )
		{
            (*itr)->draw(dc,xpos,ypos,elementBottom);
		}
		xpos-=m_columnOffset;
	}
	if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
}
void crTreeNode::rootCalcSize(int& xsize,int& ysize)
{
	xsize = m_elementWidth;
	ysize = m_rowHeight;
	for( TreeNodeArray::iterator itr = m_childArray.begin();
		itr != m_childArray.end();
		++itr )
	{
		(*itr)->calcSize(xsize,ysize);
	}
}
void crTreeNode::calcSize(int& xsize,int& ysize)
{
	ysize+=m_rowHeight;
	if(m_expand)
	{
		xsize = m_elementWidth>xsize?m_elementWidth:xsize;
		for( TreeNodeArray::iterator itr = m_childArray.begin();
			itr != m_childArray.end();
			++itr )
		{
			(*itr)->calcSize(xsize,ysize);
		}
	}
}
crTreeNode *crTreeNode::rootSelect(int x, int y)
{
	crTreeNode *selectNode = NULL;
	int xpos = 0;
	int ypos = 0;
	for( TreeNodeArray::iterator itr = m_childArray.begin();
		itr != m_childArray.end();
		++itr )
	{
		selectNode = (*itr)->select(x,y,xpos,ypos);
		if(selectNode)
			return selectNode;
	}
	return selectNode;
}
crTreeNode *crTreeNode::select(int x, int y, int& xpos, int& ypos)
{
	crTreeNode *selectNode = NULL;
	RECT rect;
	rect.left   = xpos;
	rect.top    = ypos;
	rect.right  = m_elementWidth;
	rect.bottom = rect.top + m_rowHeight;

	if(x>rect.left && x<rect.right && y>rect.top && y<rect.bottom)
	{
		setSelect(true);
		return this;
	}

	ypos+=m_rowHeight;
	if(m_expand)
	{
		xpos+=m_columnOffset;
		for( TreeNodeArray::iterator itr = m_childArray.begin();
			itr != m_childArray.end();
			++itr )
		{
			selectNode = (*itr)->select(x,y,xpos,ypos);
			if(selectNode)
				return selectNode;
		}
		xpos-=m_columnOffset;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
//
//crTreeElement
//
//////////////////////////////////////////////////////////////////////////
crTreeElement::crTreeElement(const crTreeElement& element):
crElement(element)
{
}

CRCore::crVector4f crTreeElement::getScrollRange()
{
	int xsize = 0;
	int ysize = 0;
	if(m_root.valid())
	{
		m_root->rootCalcSize(xsize,ysize);		
	}
	xsize -= m_rect[2];
	ysize -= m_rect[3];
	xsize = xsize>0?xsize:0;
	ysize = ysize>0?ysize:0;
    return CRCore::crVector4f(0,xsize,0,ysize);
}

void crTreeElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	m_root = new crTreeNode;
	std::string str;
	int int1;
	if(cfg_script.Get("TextAttribute", str))
	{
		m_root->setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	if(cfg_script.Get("RowHeight", int1))
		m_root->setRowHeight(int1);
	if(cfg_script.Get("ColumnOffset", int1))
		m_root->setColumnOffset(int1);

	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);

	crElement::load(cfg_script,widthScale,heightScale);
	m_root->setElementWidth(m_rect[2]);
}

void crTreeElement::lockTree()
{
    m_treeMutex.lock();
}

crTreeNode *crTreeElement::getTreeRootNode()
{
	return m_root.get();
}

void crTreeElement::unlockTree()
{
	m_treeMutex.unlock();
}
crTreeNode *crTreeElement::getSelectNode()
{
    return m_selectNode.get();
}
void crTreeElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	crVector4f scrollRange = getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

void crTreeElement::updateData()
{
	crElement::updateData();

	crVector4f scrollRange = getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

bool crTreeElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	bool test = false;
	if(getCapture())
	{
		if(m_hScrollBar.valid())
		{
			test = m_hScrollBar->testButtons(mx,my,mouseButtonMsg);
		}
		if(!test && m_vScrollBar.valid())
		{
			test = m_vScrollBar->testButtons(mx,my,mouseButtonMsg);
		}
	}
	if(!test)
	{
		if(pressed(mx,my))
		{
			m_isMouseOnElement = true;
			if(m_root.valid() && mouseButtonMsg == WM_LBUTTONDOWN)
			{
				int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;
				int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
				int x = mx - m_rect[0] + hscrollValue;
				int y = my - m_rect[1] + vscrollValue;
                crTreeNode *selectNode = m_root->rootSelect(x,y);
				if(m_selectNode != selectNode)
				{
					if(m_selectNode.valid())
						m_selectNode->setSelect(false);
					m_selectNode = selectNode;
				}
				if(selectNode)
				{
					selectNode->expand();
					updateData();
				}
			}
		}
		else
		{
			m_isMouseOnElement = false;
		}
		test = crElement::testButtons(mx,my,mouseButtonMsg);
	}

	return test; 
}

int crTreeElement::drawStageCopy(Display dc, Display down_dc)
{
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		//draw data
		//HDC		bufdc;
		//HBITMAP bufbmp;
		//bufdc=CreateCompatibleDC(dc);
		//bufbmp = CreateCompatibleBitmap(dc,m_rect[2]+hscrollValue,m_rect[3]+vscrollValue);
		//SelectObject(bufdc,bufbmp);
		m_treeMutex.lock();
        m_root->rootDraw(dc,m_rect[0]-hscrollValue,m_rect[1]-vscrollValue,getElementBottom());
		m_treeMutex.unlock();
		//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,hscrollValue,vscrollValue,SRCCOPY);

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			crVector4i v4;
			if(hscrollValue != 0)
			{
				v4[0] = m_rect[0] - hscrollValue;
				v4[1] = m_rect[1];
				v4[2] = hscrollValue;
				v4[3] = m_rect[3];
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2];
				v4[3] = vscrollValue;
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crTreeElement::drawStageMask(Display dc, Display down_dc)
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//
//crColumnNode
//
//////////////////////////////////////////////////////////////////////////
crColumnNode::crColumnNode():
m_data(NULL)
{

}
crColumnNode::~crColumnNode()
{
    clearBitMap();
}
crColumnNode::crColumnNode(const crColumnNode& treenode):
m_titleList(treenode.m_titleList),
m_bitmapArray(treenode.m_bitmapArray),
m_data(treenode.m_data)
{
}

void crColumnNode::addTitle(const std::string &title)
{
	m_titleList.push_back(title);
}

bool crColumnNode::getTitle(unsigned int index,std::string &title) const
{
    if(index<m_titleList.size())
	{
		title = m_titleList[index];
		return true;
	}
	return false;
}

void crColumnNode::addBitMap(HBITMAP bitmap)
{
	m_bitmapArray.push_back(bitmap);
}
void crColumnNode::clearBitMap()
{
	for( BitMapArray::iterator itr = m_bitmapArray.begin();
		 itr != m_bitmapArray.end();
		 ++itr )
	{
		DeleteObject(*itr);	
	}
	m_bitmapArray.clear();
}
void crColumnNode::setData(void *data)
{
	m_data = data;
}
void *crColumnNode::getData()
{
	return m_data;
}

void crColumnNode::draw(Display dc,unsigned int index, RECT &rect)
{
	if(index<m_titleList.size())
	{
		const std::string &title = m_titleList[index];
		DrawText(dc,title.c_str(),title.length(),&rect,DT_WORD_ELLIPSIS);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crColumnListElement
//
//////////////////////////////////////////////////////////////////////////
crColumnListElement::crColumnListElement(const crColumnListElement& element):
crElement(element),
m_columnList(element.m_columnList),
m_dataList(element.m_dataList),
m_rowHeight(element.m_rowHeight),
m_textAttribute(element.m_textAttribute),
m_useLeftBtnSelect(element.m_useLeftBtnSelect),
m_hScrollBarName(element.m_hScrollBarName),
m_vScrollBarName(element.m_vScrollBarName)
{
}

void crColumnListElement::setUseLeftBtnSelect(bool leftBtnSelect)
{
	m_useLeftBtnSelect = leftBtnSelect;
}

CRCore::crVector4f crColumnListElement::getScrollRange()
{
	float data_height = m_rowHeight * (getDataSize()+1);
	float height = m_rect[3];
	CRCore::crVector4f scrollRange;
	if(data_height > height)
	{
		float x = (data_height - height) / float(m_rowHeight);
		if(x > floor(x))
		{
			scrollRange[3] = floor(x) + 1;
		}
		else
			scrollRange[3] = floor(x);
	}

	int width = 0;
	for( ColumnList::iterator itr = m_columnList.begin();
		 itr != m_columnList.end();
		 ++itr )
	{
        width += itr->second;
	}
    width -= m_rect[2];
    if(width>0) scrollRange[1] = width;

	return scrollRange;
}
void crColumnListElement::addColumn(const std::string &columnName,int width)
{
    if(width<1) width = 1;
	m_columnList.push_back(std::make_pair(columnName,width));
}
void crColumnListElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	int int1;
	if(cfg_script.Get("TextAttribute", str))
	{
		setTextAttribute(crTextAttribute::getTextAttribute(str));
	}
	int nColumn = 1;
	while(cfg_script.Push("Column",nColumn++))
	{
		cfg_script.Get("ColumnName", str);
		cfg_script.Get("ColumnWidth", int1);
		addColumn(str,(float)int1 * widthScale);
		cfg_script.Pop();
	}
	if(cfg_script.Get("RowHeight", int1))
		setRowHeight((float)int1 /* heightScale*/);
	if(cfg_script.Get("Select", int1))
		select(int1);
	if(cfg_script.Get("UseLeftBtnSelect", int1))
		setUseLeftBtnSelect(int1);

	cfg_script.Get("HScrollBarName",m_hScrollBarName);
	cfg_script.Get("VScrollBarName",m_vScrollBarName);

	crElement::load(cfg_script,widthScale,heightScale);
}

void crColumnListElement::initWindow()
{
	m_hScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_hScrollBarName));
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	crVector4f scrollRange = getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
		int vscrollValue = m_vScrollBar->getValue();
		int _select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_vScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_vScrollBar->setValue(vscrollValue);
		}
	}
}
void crColumnListElement::checkSelect()
{
	if(isDataEmpty())
		m_select = 0;
	else
	{
		int size = getDataSize();
		if(m_select>size) m_select = size;
	}
}
void crColumnListElement::updateData()
{
	checkSelect();
	crElement::updateData();

	crVector4f scrollRange = getScrollRange();
	if(m_hScrollBar.valid())
	{
		m_hScrollBar->setRange(crVector2f(scrollRange[0],scrollRange[1]));
	}
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
		int vscrollValue = m_vScrollBar->getValue();
		int _select = m_select - vscrollValue;
		int maxrow = getMaxRowCanBeDisplay();
		if(_select<0)
		{
			vscrollValue += _select;
			m_vScrollBar->setValue(vscrollValue);
		}
		else if(_select>maxrow)
		{	
			vscrollValue += (_select - maxrow);
			m_vScrollBar->setValue(vscrollValue);
		}
	}
}

bool crColumnListElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	bool test = false;
	if(getCapture())
	{
		if(m_hScrollBar.valid())
		{
			test = m_hScrollBar->testButtons(mx,my,mouseButtonMsg);
		}
		if(!test && m_vScrollBar.valid())
		{
			test = m_vScrollBar->testButtons(mx,my,mouseButtonMsg);
		}
	}
	if(!test)
	{
		if(pressed(mx,my))
		{
			int vscrollValue = 0;
			if(m_vScrollBar.valid()) vscrollValue = m_vScrollBar->getValue();

			m_isMouseOnElement = true;
			if(!m_useLeftBtnSelect || mouseButtonMsg == WM_LBUTTONDOWN)
			{
				int i = (my-m_rect[1])/m_rowHeight + vscrollValue;
				if(i>=0 && i < getDataSize()+1 && m_select != i)
				{
					select(i);
				}
			}
		}
		else
		{
			m_isMouseOnElement = false;
		}
		test = crElement::testButtons(mx,my,mouseButtonMsg);
	}

	return test; 
}

int crColumnListElement::getMaxRowCanBeDisplay()
{
	float x = float(m_rect[3])/float(m_rowHeight);
	return x > floor(x)?floor(x)-1:floor(x)-2;
}

int crColumnListElement::drawStageCopy(Display dc, Display down_dc)
{
	int hscrollValue = m_hScrollBar.valid()?m_hScrollBar->getValue():0;
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		if(m_select>-1)
		{
			int _select = m_select - vscrollValue;
			if(_select<0)
			{
				_select = 0;
				//m_select = vscrollValue;
				select(vscrollValue);
			}
			else
			{
				int maxrow = getMaxRowCanBeDisplay();
				if(_select>maxrow) _select = maxrow;
				//m_select = vscrollValue + _select;
				select(vscrollValue + _select);
			}
			if(m_hDownImage)
			{
				SelectObject(mdc,m_hDownImage);
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,mdc,0,0,SRCCOPY);
			}
			else	//need draw by parent
			{
				BitBlt(dc,m_rect[0],m_rect[1]+_select*m_rowHeight,m_rect[2],m_rowHeight,down_dc,m_rect[0],m_rect[1]+_select*m_rowHeight,SRCCOPY);
			}
		}
		//draw data
		m_dataMutex.lock();
		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		RECT rect;
		rect.left   = m_rect[0] + TextRectOffset - hscrollValue;
		rect.top    = m_rect[1] + TextRectOffset;
		rect.right  = getElementRight() - TextRectOffset;
		rect.bottom = m_rect[1] + m_rowHeight - TextRectOffset;
		int prewidth = 0;
		unsigned int i,j;
		i = 0;
		for ( ColumnList::iterator itr = m_columnList.begin();
			  itr != m_columnList.end();
			  ++itr,++i )
		{
			rect.top    = m_rect[1] + TextRectOffset;
			rect.bottom = m_rect[1] + m_rowHeight - TextRectOffset;

            rect.left += prewidth;
			rect.right = rect.left + itr->second;
			DrawText(dc,itr->first.c_str(),itr->first.length(),&rect,DT_WORD_ELLIPSIS);
			prewidth = itr->second;

			j = 1;
			for( DataList::iterator ditr = m_dataList.begin() + vscrollValue;
				ditr != m_dataList.end();
				++ditr, ++j )
			{
				rect.top    = m_rect[1]+j*m_rowHeight + TextRectOffset;
				rect.bottom = m_rect[1]+(j+1)*m_rowHeight - TextRectOffset;
				if(rect.bottom>getElementBottom()) break;
				
                (*ditr)->draw(dc,i,rect);
			}
		}
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);
		m_dataMutex.unlock();

		if(hscrollValue != 0 || vscrollValue != 0)
		{
			crVector4i v4;
			if(hscrollValue != 0)
			{
				v4[0] = m_rect[0] - hscrollValue;
				v4[1] = m_rect[1];
				v4[2] = hscrollValue;
				v4[3] = m_rect[3];
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2];
				v4[3] = vscrollValue;
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}

		//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,0,0,SRCCOPY);
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crColumnListElement::drawStageMask(Display dc, Display down_dc)
{
	return 1;
}

void crColumnListElement::clearData()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	m_dataList.clear();
}

void crColumnListElement::addData(crColumnNode *data)
{
	if(data) 
	{
		m_dataMutex.lock();
		m_dataList.push_back(data);
		m_dataMutex.unlock();
	}
}

void crColumnListElement::deleteCurrentRow()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if(!m_dataList.empty())
	{
		int i = getSelect();
		DataList::iterator itr = m_dataList.begin();
		itr+=i;
		m_dataList.erase(itr);
	}
}

void crColumnListElement::insertData(unsigned int index, crColumnNode *data)
{
	if(data)
	{
		m_dataMutex.lock();
		if (index >= m_dataList.size())
		{
			m_dataList.push_back(data);
		}
		else
		{
			m_dataList.insert(m_dataList.begin()+index, data);
		}
		m_dataMutex.unlock();
	}
}

crColumnNode *crColumnListElement::getData(unsigned int index)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	if(index < m_dataList.size())
	    return m_dataList[index].get();
	return NULL;
}

void crColumnListElement::select(int index)
{
	if(m_select!=index)
	{
		m_select = index;
		if(m_vScrollBar.valid())
		{
			int vscrollValue = m_vScrollBar->getValue();
			int _select = m_select - vscrollValue;
			int maxrow = getMaxRowCanBeDisplay();
			if(_select<0)
			{
				vscrollValue += _select;
				m_vScrollBar->setValue(vscrollValue);
			}
			else if(_select>maxrow)
			{	
				vscrollValue += (_select - maxrow);
				m_vScrollBar->setValue(vscrollValue);
			}
		}
		updateData();
	}
}

void crColumnListElement::setSelect(int index)
{
	if(m_select!=index)
	{
		m_select = index;
		if(m_vScrollBar.valid())
		{
			int vscrollValue = m_vScrollBar->getValue();
			int _select = m_select - vscrollValue;
			int maxrow = getMaxRowCanBeDisplay();
			if(_select<0)
			{
				vscrollValue += _select;
				m_vScrollBar->setValue(vscrollValue);
			}
			else if(_select>maxrow)
			{	
				vscrollValue += (_select - maxrow);
				m_vScrollBar->setValue(vscrollValue);
			}
		}
	}
}

int crColumnListElement::getSelect()
{
	return m_select;
}

void crColumnListElement::setRowHeight(int height)
{
	m_rowHeight = height;
}

int crColumnListElement::getRowHeight()
{
	return m_rowHeight;
}

int crColumnListElement::getDataSize()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataList.size(); 
}
bool crColumnListElement::isDataEmpty()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_dataMutex);
	return m_dataList.empty();
}
//////////////////////////////////////////////////////////////////////////
//
//crListNode
//
//////////////////////////////////////////////////////////////////////////
crListNode::crListNode():
m_selected(false),
m_currentBitmap(0),
m_data(NULL)
{

}
crListNode::~crListNode()
{
    clearBitMap();
}
crListNode::crListNode(const crListNode& treenode):
m_title(treenode.m_title),
m_title2(treenode.m_title2),
m_selected(false),
m_bitmapArray(treenode.m_bitmapArray),
m_currentBitmap(treenode.m_currentBitmap),
m_data(treenode.m_data)
{
}
void crListNode::setTitle(const std::string &title)
{
	m_title = title;
}
const std::string &crListNode::getTitle() const
{
	return m_title;
}
void crListNode::setTitle2(const std::string &title2)
{
	m_title2 = title2;
}
const std::string &crListNode::getTitle2() const
{
	return m_title2;
}
void crListNode::setSelect(bool select)
{
	m_selected = select;
}
bool crListNode::getSelect()
{
	return m_selected;
}
void crListNode::addBitMapFile(const std::string& file)
{
	HBITMAP hImage = (HBITMAP)LoadImage(GetModuleHandle(NULL),file.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    addBitMap(hImage);
}
void crListNode::addBitMap(HBITMAP bitmap)
{
	m_bitmapArray.push_back(bitmap);
}
void crListNode::clearBitMap()
{
	for( BitMapArray::iterator itr = m_bitmapArray.begin();
		itr != m_bitmapArray.end();
		++itr )
	{
		DeleteObject(*itr);	
	}
	m_bitmapArray.clear();
}
void crListNode::setCurrentBitmap(unsigned char index)
{
	if(index<m_bitmapArray.size())
		m_currentBitmap = index;
}
unsigned char crListNode::getCurentBitmap()
{
	return m_currentBitmap;
}
void crListNode::setData(void *data)
{
	m_data = data;
}
void *crListNode::getData()
{
	return m_data;
}
void crListNode::draw(Display dc,Display mdc,int xpos,int ypos,const CRCore::crVector2i& nodeSize,int titleHeight,int format,int format2)
{
	RECT rect;
	rect.left   = xpos - 1;
	rect.top    = ypos - 1;
	rect.right  = xpos + nodeSize[0] + 1;
	rect.bottom = ypos + nodeSize[1] + 1;

	if(m_selected)
	{
		int bkmode = GetBkMode(dc);
		HBRUSH brush = CreateSolidBrush(RGB(0,0,128));
		FillRect(dc,&rect,brush);
		SetBkMode(dc,bkmode);
		DeleteObject((HGDIOBJ)brush);
	}
	rect.left   = xpos;
	rect.top    = ypos;
	rect.right  = xpos + nodeSize[0];
	rect.bottom = ypos + nodeSize[1];

	//draw img
	if(!m_bitmapArray.empty())
	{
		if(m_currentBitmap>=m_bitmapArray.size())
			m_currentBitmap = 0;
		HBITMAP bitmap = m_bitmapArray[m_currentBitmap];
		if(bitmap)
		{
			SelectObject(mdc,bitmap);
			BitBlt(dc,rect.left,rect.top,nodeSize[0],nodeSize[1],mdc,0,0,SRCCOPY);
		}
	}

	if(!m_title.empty())
	{
		rect.top = rect.bottom-titleHeight;
		DrawText(dc,m_title.c_str(),m_title.length(),&rect,format);
	}
	if(!m_title2.empty())
	{
		rect.top = ypos;
		rect.bottom = ypos + titleHeight;
		DrawText(dc,m_title2.c_str(),m_title2.length(),&rect,format2);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crListControlElement
//
//////////////////////////////////////////////////////////////////////////
crListControlElement::crListControlElement():
m_titleHeight(0),
m_titleFormat(0),
m_titleFormat2(0)
{}
crListControlElement::crListControlElement(const crListControlElement& element):
crElement(element),
m_titleHeight(element.m_titleHeight),
m_titleFormat(element.m_titleFormat),
m_titleFormat2(element.m_titleFormat2)
{
}

void crListControlElement::rootCalcSize(int& xsize, int& ysize)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
	xsize = m_spaceBetween[0];
	ysize = m_nodeSize[1]+m_spaceBetween[1];
	for( ListNodeVec::iterator itr = m_listNodeVec.begin();
		itr != m_listNodeVec.end();
		++itr )
	{
		xsize += m_nodeSize[0] + m_spaceBetween[0];
		if(xsize>m_rect[2])
		{
			xsize = m_spaceBetween[0];
			ysize+=m_nodeSize[1]+m_spaceBetween[1];
		}
	}
}

CRCore::crVector4f crListControlElement::getScrollRange()
{
	int xsize = 0;
	int ysize = 0;
    rootCalcSize(xsize,ysize);
	
	xsize -= m_rect[2];
	ysize -= m_rect[3];
	xsize = xsize>0?xsize:0;
	ysize = ysize>0?ysize:0;
	return CRCore::crVector4f(0,xsize,0,ysize);
}

void crListControlElement::load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale)
{
	std::string str;
	int int1;
	if(cfg_script.Get("TextAttribute", str))
	{
		m_textAttribute = crTextAttribute::getTextAttribute(str);
	}
	std::vector<float> v_i;
	if(cfg_script.Get("NodeSize", v_i))
		m_nodeSize.set(v_i[0],v_i[1]);
	if(cfg_script.Get("SpaceBetween", v_i))
		m_spaceBetween.set(v_i[0],v_i[1]);
	if(cfg_script.Get("TitleHeight", int1))
		m_titleHeight = int1;

	cfg_script.Get("VScrollBarName",m_vScrollBarName);

	m_titleFormat = 0;
	if(cfg_script.Get("TitleFormat", str))
	{
		if(str.find("DT_LEFT") != std::string::npos)
		{
			m_titleFormat |= DT_LEFT;
		}
		if(str.find("DT_CENTER") != std::string::npos)
		{
			m_titleFormat |= DT_CENTER;
		}
		if(str.find("DT_RIGHT") != std::string::npos)
		{
			m_titleFormat |= DT_RIGHT;
		}
		if(str.find("DT_VCENTER") != std::string::npos)
		{
			m_titleFormat |= DT_VCENTER;
		}
		if(str.find("DT_BOTTOM") != std::string::npos)
		{
			m_titleFormat |= DT_BOTTOM;
		}
		if(str.find("DT_WORDBREAK") != std::string::npos)
		{
			m_titleFormat |= DT_WORDBREAK;
		}
		if(str.find("DT_SINGLELINE") != std::string::npos)
		{
			m_titleFormat |= DT_SINGLELINE;
		}
		if(str.find("DT_EXPANDTABS") != std::string::npos)
		{
			m_titleFormat |= DT_EXPANDTABS;
		}
		if(str.find("DT_TABSTOP") != std::string::npos)
		{
			m_titleFormat |= DT_TABSTOP;
		}
		if(str.find("DT_NOCLIP") != std::string::npos)
		{
			m_titleFormat |= DT_NOCLIP;
		}
		if(str.find("DT_EXTERNALLEADING") != std::string::npos)
		{
			m_titleFormat |= DT_EXTERNALLEADING;
		}
		if(str.find("DT_CALCRECT") != std::string::npos)
		{
			m_titleFormat |= DT_CALCRECT;
		}
		if(str.find("DT_NOPREFIX") != std::string::npos)
		{
			m_titleFormat |= DT_NOPREFIX;
		}
		if(str.find("DT_INTERNAL") != std::string::npos)
		{
			m_titleFormat |= DT_INTERNAL;
		}
	}
	m_titleFormat2 = 0;
	if(cfg_script.Get("TitleFormat2", str))
	{
		if(str.find("DT_LEFT") != std::string::npos)
		{
			m_titleFormat2 |= DT_LEFT;
		}
		if(str.find("DT_CENTER") != std::string::npos)
		{
			m_titleFormat2 |= DT_CENTER;
		}
		if(str.find("DT_RIGHT") != std::string::npos)
		{
			m_titleFormat2 |= DT_RIGHT;
		}
		if(str.find("DT_VCENTER") != std::string::npos)
		{
			m_titleFormat2 |= DT_VCENTER;
		}
		if(str.find("DT_BOTTOM") != std::string::npos)
		{
			m_titleFormat2 |= DT_BOTTOM;
		}
		if(str.find("DT_WORDBREAK") != std::string::npos)
		{
			m_titleFormat2 |= DT_WORDBREAK;
		}
		if(str.find("DT_SINGLELINE") != std::string::npos)
		{
			m_titleFormat2 |= DT_SINGLELINE;
		}
		if(str.find("DT_EXPANDTABS") != std::string::npos)
		{
			m_titleFormat2 |= DT_EXPANDTABS;
		}
		if(str.find("DT_TABSTOP") != std::string::npos)
		{
			m_titleFormat2 |= DT_TABSTOP;
		}
		if(str.find("DT_NOCLIP") != std::string::npos)
		{
			m_titleFormat2 |= DT_NOCLIP;
		}
		if(str.find("DT_EXTERNALLEADING") != std::string::npos)
		{
			m_titleFormat2 |= DT_EXTERNALLEADING;
		}
		if(str.find("DT_CALCRECT") != std::string::npos)
		{
			m_titleFormat2 |= DT_CALCRECT;
		}
		if(str.find("DT_NOPREFIX") != std::string::npos)
		{
			m_titleFormat2 |= DT_NOPREFIX;
		}
		if(str.find("DT_INTERNAL") != std::string::npos)
		{
			m_titleFormat2 |= DT_INTERNAL;
		}
	}
	crElement::load(cfg_script,widthScale,heightScale);
}
void crListControlElement::addListNode(crListNode *listNode)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
    m_listNodeVec.push_back(listNode);
}
void crListControlElement::removeSelectListNode()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
	ListNodeVec::iterator itr;
	ListNodeVec::iterator nextitr;
	for( itr = m_listNodeVec.begin();
		 itr != m_listNodeVec.end();
		 ++itr )
	{
		if(*itr == m_selectNode.get())
		{
			nextitr = itr;
			nextitr++;
			if(nextitr != m_listNodeVec.end())
			{
				m_selectNode = nextitr->get();
				m_selectNode->setSelect(true);
			}
			else
			{
				if(itr!=m_listNodeVec.begin())
				{
					nextitr = m_listNodeVec.begin();
					m_selectNode = nextitr->get();
					m_selectNode->setSelect(true);
				}
				else
				{
					m_selectNode = NULL;
				}
			}
            m_listNodeVec.erase(itr);
			break;
		}
	}
}
void crListControlElement::clear()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
	m_listNodeVec.clear();
	m_selectNode = NULL;
}
void crListControlElement::lockList()
{
	m_listMutex.lock();
}
crListControlElement::ListNodeVec &crListControlElement::getListNodeVec()
{
	return m_listNodeVec;
}
void crListControlElement::unlockList()
{
	m_listMutex.unlock();
}
crListNode *crListControlElement::getSelectNode()
{
	return m_selectNode.get();
}
void crListControlElement::initWindow()
{
	m_vScrollBar = dynamic_cast<crScrollBarElement *>(m_parentStage->getElement(m_vScrollBarName));
	crElement::initWindow();
	crVector4f scrollRange = getScrollRange();
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}

void crListControlElement::updateData()
{
	crElement::updateData();

	crVector4f scrollRange = getScrollRange();
	if(m_vScrollBar.valid())
	{
		m_vScrollBar->setRange(crVector2f(scrollRange[2],scrollRange[3]));
	}
}
crListNode *crListControlElement::rootSelect(int x, int y)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
	RECT rect;
	int xpos = m_spaceBetween[0];
	int ypos = m_spaceBetween[1];
	for( ListNodeVec::iterator itr = m_listNodeVec.begin();
		itr != m_listNodeVec.end();
		++itr )
	{
		rect.left   = xpos;
		rect.top    = ypos;
		rect.right  = xpos + m_nodeSize[0];
		rect.bottom = ypos + m_nodeSize[1];

		if(x>rect.left && x<rect.right && y>rect.top && y<rect.bottom)
		{
			return itr->get();
		}

		xpos += m_nodeSize[0] + m_spaceBetween[0];
		if(xpos>m_rect[2])
		{
			xpos = m_spaceBetween[0];
			ypos+=m_nodeSize[1]+m_spaceBetween[1];
		}
	}
	return NULL;
}

bool crListControlElement::inputChar(wchar_t c)
{ 
	switch(c)
	{
	case 9://tab
		if(!m_listNodeVec.empty())
		{
			crListNode *selectNode = m_selectNode.get();
			if(!selectNode)
				selectNode = m_listNodeVec.begin()->get();
			else
			{
				CRCore::ScopedLock<CRCore::crMutex> lock(m_listMutex);
				ListNodeVec::iterator itr;
				for( itr = m_listNodeVec.begin();
					itr != m_listNodeVec.end();
					++itr )
				{
					if( *itr == selectNode )
					{
						break;
					}
				}
				itr++;
				if(itr == m_listNodeVec.end())
					itr = m_listNodeVec.begin();
				selectNode = itr->get();
			}
			if(m_selectNode != selectNode)
			{
				if(m_selectNode.valid())
					m_selectNode->setSelect(false);
				m_selectNode = selectNode;
				if(m_selectNode.valid())
					m_selectNode->setSelect(true);
				updateData();
			}
		}
		break;
	}
	return crElement::inputChar(c);
}


bool crListControlElement::testButtons(int mx, int my, int mouseButtonMsg )
{
	bool test = false;
	if(getCapture())
	{
		if(m_vScrollBar.valid())
		{
			test = m_vScrollBar->testButtons(mx,my,mouseButtonMsg);
		}
	}
	if(!test)
	{
		if(pressed(mx,my))
		{
			m_isMouseOnElement = true;
			if(mouseButtonMsg == WM_LBUTTONDOWN || mouseButtonMsg == WM_RBUTTONDOWN || mouseButtonMsg == WM_LBUTTONDBLCLK)
			{
				int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;
				int x = mx - m_rect[0];
				int y = my - m_rect[1] + vscrollValue;
				crListNode *selectNode = rootSelect(x,y);
				if(m_selectNode != selectNode)
				{
					if(m_selectNode.valid())
						m_selectNode->setSelect(false);
					m_selectNode = selectNode;
					if(m_selectNode.valid())
					    m_selectNode->setSelect(true);
					updateData();
				}
			}
		}
		else
		{
			m_isMouseOnElement = false;
		}
		test = crElement::testButtons(mx,my,mouseButtonMsg);
	}

	return test; 
}
void crListControlElement::rootDraw(Display dc,Display mdc,int xpos,int ypos)
{
	xpos+=m_spaceBetween[0];
	ypos+=m_spaceBetween[1];
	int xbegin = xpos;
	for( ListNodeVec::iterator itr = m_listNodeVec.begin();
		itr != m_listNodeVec.end();
		++itr )
	{
		if(m_textAttribute.valid()) m_textAttribute->drawTextAttribute(dc);
		(*itr)->draw(dc,mdc,xpos,ypos,m_nodeSize,m_titleHeight,m_titleFormat,m_titleFormat2);
		if(m_textAttribute.valid()) crTextAttribute::endDrawText(dc);

		xpos += m_nodeSize[0] + m_spaceBetween[0];
		if(xpos>getElementRight())
		{
			xpos = xbegin;
			ypos+=m_nodeSize[1]+m_spaceBetween[1];
			if(ypos + m_nodeSize[1] > getElementBottom()) return;
		}
	}
}
int crListControlElement::drawStageCopy(Display dc, Display down_dc)
{
	int vscrollValue = m_vScrollBar.valid()?m_vScrollBar->getValue():0;

	HDC		mdc,copy_dc;
	HBITMAP copybmp;

	bool noNeedFatherDraw = true;

	mdc=CreateCompatibleDC(NULL);
	// draw bitmap
	if(!m_enable)
	{
		if(m_hDisableImage)
		{
			SelectObject(mdc,m_hDisableImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}
		else	//need draw by parent
			noNeedFatherDraw = false;
	}
	else
	{
		if(m_hImage)
		{
			SelectObject(mdc,m_hImage);
			BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],mdc,0,0,SRCCOPY);
		}

		if(vscrollValue != 0)
		{
			int width = GetDeviceCaps(dc, HORZRES);
			int height = GetDeviceCaps(dc, VERTRES);
			copy_dc=CreateCompatibleDC(dc);
			copybmp = CreateCompatibleBitmap(dc,width,height);
			SelectObject(copy_dc,copybmp);

			BitBlt(copy_dc,0,0,width,height,dc,0,0,SRCCOPY);
		}

		//draw data
		//HDC		bufdc;
		//HBITMAP bufbmp;
		//bufdc=CreateCompatibleDC(dc);
		//bufbmp = CreateCompatibleBitmap(dc,m_rect[2]+hscrollValue,m_rect[3]+vscrollValue);
		//SelectObject(bufdc,bufbmp);
		m_listMutex.lock();
		rootDraw(dc,mdc,m_rect[0],m_rect[1]-vscrollValue);
		m_listMutex.unlock();
		//BitBlt(dc,m_rect[0],m_rect[1],m_rect[2],m_rect[3],bufdc,hscrollValue,vscrollValue,SRCCOPY);

		if(vscrollValue != 0)
		{
			crVector4i v4;
			if(vscrollValue != 0)
			{
				v4[0] = m_rect[0];
				v4[1] = m_rect[1]-vscrollValue;
				v4[2] = m_rect[2]+1;
				v4[3] = vscrollValue;
				BitBlt(dc,v4[0],v4[1],v4[2],v4[3],copy_dc,v4[0],v4[1],SRCCOPY);
			}
			DeleteDC(copy_dc);
			DeleteObject(copybmp);
		}
	}
	DeleteDC(mdc);

	return noNeedFatherDraw;
}

int crListControlElement::drawStageMask(Display dc, Display down_dc)
{
	return 1;
}
