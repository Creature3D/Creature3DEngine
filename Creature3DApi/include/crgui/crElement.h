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
#ifndef CRGUI_CRELEMENT_H
#define CRGUI_CRELEMENT_H 1

#include <CRGUI/crExport.h>
#include <CRGUI/crTypes.h>
#include <CRGUI/crTextAttribute.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector4.h>
#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crTimer.h>
#include "rcfg/ConfigScriptOO.h"
#include <string>
#include <vector>
#include <CRCore/thread/crMutex.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <list>
namespace CRGUI{
class crImageStage;

#define META_Element(library,name) \
	virtual CRGUI::crElement* cloneType() const { return new name (); } \
	virtual CRGUI::crElement* clone(const CRCore::crCopyOp& copyop) const { return new name (*this); } \
	virtual bool isSameKindAs(const CRGUI::crElement* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
	virtual const char* libraryName() const { return #library; }\
	virtual const char* className() const { return #name; }\
    virtual const char* _name() const { return #name; }
	
class CRGUI_EXPORT crElement : public CRCore::crData
{
friend class crImageStage;
public:
	crElement();
	crElement(const crElement& element);

    META_Element(CRGUI,crElement)
	virtual void addParam(int i, const std::string& str){}
	virtual void inputParam(int i, void *param){}
	virtual void getParam(int i, void*& param){}

    virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void resize(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);

	void setName(const std::string &name){ m_name = name; }
	inline const std::string &getName()const { return m_name; }

	virtual void setRect(float x,float y,float wid,float hit);
	virtual void setRect(const CRCore::crVector4i &rect);
	const CRCore::crVector4i &getRect();
    int getElementRight();
    int getElementBottom();
	virtual void setPos(float x, float y);
	void moveOffset(int offsetx, int offsety);

	virtual bool pressed(int x,int y);
    
	virtual void setElementImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );
	virtual void setElementMouseOnImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );
	virtual void setElementDownImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );
	virtual void setElementMaskImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );
	virtual void setElementDisableImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );

	int drawElement(Display dc, Display down_dc);
	virtual void backup(){}
	virtual void resumeBackupData(){}

	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual bool inputKey(int key);
	virtual bool inputChar(wchar_t c);
	virtual void initWindow();
	virtual void destroyWindow();

	//virtual void updateHScrollValue(float hscrollValue){ m_hscrollValue = hscrollValue; }
	//virtual void updateVScrollValue(float vscrollValue){ m_vscrollValue = vscrollValue; }
	virtual CRCore::crVector4f getScrollRange(){ return CRCore::crVector4f(0.0,0.0,0.0,0.0); }

	bool getMouseOnElement(){ return m_isMouseOnElement; }

	//void setNeedRedraw(bool bln){ m_needRedraw = bln; if(bln) m_parentStage->setNeedRedraw(bln); }

	enum DrawMode 
	{
		Draw_Copy,
		Draw_Mask,
		Draw_Transparent
	};

	void setDrawMode(DrawMode drawMode) { m_drawMode = drawMode; }
	DrawMode getDrawMode() { return m_drawMode; }
	inline bool getTransparent(){ return m_drawMode == Draw_Transparent; }

	void setEnable(bool enable);
	inline bool getEnable(){ return m_enable; }

	void setShow(bool show);
	inline bool getShow(){ return m_show; }

	void setCanFocus(bool canFocus);
	inline bool getCanFocus() { return m_canFocus; }

	void setCanDrag(bool canDrag);
	inline bool getCanDrag() { return m_canDrag; }

	void setCanCapture(bool canCapture);
	inline bool getCanCapture(){ return m_canCapture; }

	inline bool getCapture() { return m_capture; }
	bool onCaptureMouse(int mx, int my, int mouseButtonMsg);

public:
	class Callback : public CRCore::crEventCallback
	{
	public:
		Callback(){}
		virtual crEventCallback* cloneType() const = 0;
		virtual crEventCallback* clone() const = 0;
		virtual bool isSameKindAs(const crEventCallback* obj) const { return true; }
		virtual const char* libraryName() const = 0;
		virtual const char* className() const = 0;
		virtual const char* _name() const = 0;
		Callback(const Callback& callback):CRCore::crEventCallback(callback){}
		virtual void operator()(crElement &){};
		virtual void operator()(crElement &element,int key){};
		virtual void operator()(crElement &element,wchar_t c){};
	protected:
		virtual ~Callback(){}
	};

	void addInitWindowEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_initWindowEventCallbacks.push_back( cb ); 
	}
	void addDestroyWindowEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_destroyWindowEventCallbacks.push_back( cb ); 
	}
	void addGetFocusEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_getFocusEventCallbacks.push_back( cb ); 
	}
	void addLostFocusEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_lostFocusEventCallbacks.push_back( cb ); 
	}
	void addInputKeyEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_inputKeyEventCallbacks.push_back( cb ); 
	}
	void addInputCharEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_inputCharEventCallbacks.push_back( cb ); 
	}
	void addUpdateDataEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_updateDataEventCallbacks.push_back( cb ); 
	}
	void addPreDrawEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_preDrawEventCallbacks.push_back( cb ); 
	}
	void addMouseOnEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mouseOnEventCallbacks.push_back( cb ); 
	}
	void addMouseMoveEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mouseMoveEventCallbacks.push_back( cb ); 
	}
	void addLButtonDownEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_lButtonDownEventCallbacks.push_back( cb ); 
	}
	void addLButtonUpEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_lButtonUpEventCallbacks.push_back( cb ); 
	}
	void addLButtonDblClkEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_lButtonDblClkEventCallbacks.push_back( cb ); 
	}
	void addRButtonDownEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_rButtonDownEventCallbacks.push_back( cb ); 
	}
	void addRButtonUpEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_rButtonUpEventCallbacks.push_back( cb ); 
	}
	void addRButtonDblClkEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_rButtonDblClkEventCallbacks.push_back( cb ); 
	}
	void addMButtonDownEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mButtonDownEventCallbacks.push_back( cb ); 
	}
	void addMButtonUpEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mButtonUpEventCallbacks.push_back( cb ); 
	}
	void addMButtonDblClkEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mButtonDblClkEventCallbacks.push_back( cb ); 
	}
	void addMouseWheelEvent( CRCore::crEventCallback *cb ) 
	{ 
		m_mouseWheelEventCallbacks.push_back( cb ); 
	}

	void setParentStage(crImageStage *stage) { m_parentStage = stage; }
	crImageStage *getParentStage(){ return m_parentStage; }

	void setParentElementName(const std::string &elementName);
	std::string &getParentElementName();
	void setParentElement(crElement *element) { m_parentElement = element; }
	crElement *getParentElement(){ return m_parentElement; }
	inline virtual void updateParentElement(crElement *element){ if(m_parentElement) m_parentElement->updateParentElement(this); }

	void setFocus();
	virtual void updateData();
	virtual void frame(float dt);
	virtual void preDraw();
	virtual void mouseOn();
    virtual void mouseMove(int mx, int my);
	virtual void lButtonDown(int mx, int my);
	virtual void rButtonDown(int mx, int my);
    virtual void mButtonDown(int mx, int my);
	virtual void lButtonUp(int mx, int my);
	virtual void rButtonUp(int mx, int my);
	virtual void mButtonUp(int mx, int my);
	virtual void lButtonDblClk(int mx, int my);
	virtual void rButtonDblClk(int mx, int my);
    virtual void mButtonDblClk(int mx, int my);
	virtual void mouseWheel(int mx, int my);
	virtual void mouseDrag(int mx, int my);

	static void clearBmpCache();
	inline bool isFocus(){ return m_focus; }
protected:
	virtual ~crElement();

	virtual void getFocus();
	virtual void lostFocus();
	virtual void setCapture();
	virtual void releaseCapture();

	virtual int drawStageCopy(Display dc, Display down_dc){ return 1; }
	virtual int drawStageMask(Display dc, Display down_dc){ return 1; }
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
    
	//int		m_posx,m_posy;		// position of button
	//int		m_farx,m_fary;		// position of bottom right
	CRCore::crVector4i m_rect;//left, top, wid, hit

	//float m_hscrollValue;
 //   float m_vscrollValue;

	HBITMAP m_hImage;
	HBITMAP m_hMouseOnImage;
    HBITMAP m_hDownImage;
	HBITMAP m_hMaskImage;
	HBITMAP m_hDisableImage;

	std::string m_name;
	std::string m_imageName;
	std::string m_mouseOnImageName;
	std::string m_downImageName;
	std::string m_maskImageName;
	std::string m_disableImageName;

//bool   m_needRedraw;
	bool   m_isMouseOnElement;
	bool   m_canFocus;
    bool   m_focus;
	bool   m_enable;
	bool   m_show;
	bool   m_canCapture;
	bool   m_capture;
	bool   m_canDrag;//1：可拖动，0：不可拖动

	typedef std::vector < CRCore::ref_ptr<CRCore::crEventCallback> > CallbackVec;
	CallbackVec m_mouseMoveEventCallbacks;
	CallbackVec m_lButtonDownEventCallbacks;
	CallbackVec m_rButtonDownEventCallbacks;
	CallbackVec m_mButtonDownEventCallbacks;
	CallbackVec m_lButtonUpEventCallbacks;
	CallbackVec m_rButtonUpEventCallbacks;
	CallbackVec m_mButtonUpEventCallbacks;
	CallbackVec m_lButtonDblClkEventCallbacks;
	CallbackVec m_rButtonDblClkEventCallbacks;
	CallbackVec m_mButtonDblClkEventCallbacks;
	CallbackVec m_mouseWheelEventCallbacks;

	CallbackVec m_mouseOnEventCallbacks;
	CallbackVec m_getFocusEventCallbacks;
	CallbackVec m_lostFocusEventCallbacks;
	
	CallbackVec m_initWindowEventCallbacks;
	CallbackVec m_destroyWindowEventCallbacks;

	CallbackVec m_inputKeyEventCallbacks;
	CallbackVec m_inputCharEventCallbacks;
    CallbackVec m_updateDataEventCallbacks;
	CallbackVec m_preDrawEventCallbacks;
    
    crImageStage *m_parentStage;
	crElement *m_parentElement;//用于组合控件
	std::string m_parentElementName;

	DrawMode m_drawMode;
	typedef std::map<std::string, HBITMAP> BMPCacheMap;
	HBITMAP getOrLoadBmp(const std::string &fileName,const CRCore::crVector2i& imageSize);
	static BMPCacheMap s_bmpCacheMap;
};

class CRGUI_EXPORT crButtonElement : public crElement
{
public:
	crButtonElement(){}
	crButtonElement(const crButtonElement& element);
	META_Element(CRGUI,crButtonElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
};

class CRGUI_EXPORT crCheckBoxElement : public crElement
{
public:
	crCheckBoxElement():m_select(false),m_backup_select(false){}
	crCheckBoxElement(const crCheckBoxElement& element);
	META_Element(CRGUI,crCheckBoxElement)
    virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	void setSelect( bool select );
	void select(bool select);
	bool getSelect() { return m_select; }
	virtual void backup(){ m_backup_select = m_select; }
	virtual void resumeBackupData(){ m_select = m_backup_select; }

protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

    bool m_select;
	bool m_backup_select;
};

class CRGUI_EXPORT crRadioElement : public crElement
{
public:
	crRadioElement():m_select(false),m_backup_select(false){}
	crRadioElement(const crRadioElement& element);
	META_Element(CRGUI,crRadioElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	void setSelect( bool select );
	bool getSelect() { return m_select; }
	virtual void backup(){ m_backup_select = m_select; }
	virtual void resumeBackupData(){ m_select = m_backup_select; }
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	bool m_select;
	bool m_backup_select;
};

class CRGUI_EXPORT crRadioGroupElement : public crElement
{
public:
	crRadioGroupElement();
	crRadioGroupElement(const crRadioGroupElement& element);
	META_Element(CRGUI,crRadioGroupElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
    virtual void initWindow();
    virtual void updateParentElement(crElement *element);
	inline crRadioElement *getCurrentSelectedRadio(){ return m_radioGroup[m_currentSelect].get(); }
	void select(int index);
	int getSelect();
	typedef std::vector< std::string > RadioNameGroup;
	typedef std::vector< CRCore::ref_ptr<crRadioElement> > RadioGroup;
	RadioGroup &getRadioGroup();
protected:
	virtual ~crRadioGroupElement();
	RadioNameGroup m_radioNameGroup;
	RadioGroup m_radioGroup;
	int m_initSelectedIndex;
    //CRCore::ref_ptr<crRadioElement> m_currentSelectRadio;
	int m_currentSelect;
};

class CRGUI_EXPORT crScrollBarElement : public crElement
{
public:
	crScrollBarElement();
	crScrollBarElement(const crScrollBarElement& element);
	META_Element(CRGUI,crScrollBarElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual void backup(){ m_backup_value = m_value; }
	virtual void resumeBackupData(){ m_value = m_backup_value; }
	enum ScrollBarType
	{
        HORIZONTALBAR,
		VERTICALBAR
	};
	void setScrollBarType(ScrollBarType bar);
	ScrollBarType getScrollBarType();
	void setRange(const CRCore::crVector2f &range);
	void setRange(float min, float max);
	const CRCore::crVector2f &getRange();
	void setValue(float value);
	float getValue();
	void setLineValue(float lineValue);
	float getLineValue();
	void setPageValue(float pageValue);
	float getPageValue();
	void setButtonRange(int buttonRange);
	float getButtonRange();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	float m_value;
	float m_backup_value;
	CRCore::crVector2f m_range;
    float m_lineValue;
	float m_pageValue;
	int m_buttonRange;
	bool m_hold;
	int m_buttonClicked;//==1 表示left/up clicked ==2 表示right/down clicked  ==0 表示没有clicked
	int m_lastMouseValue;
	ScrollBarType m_scrollBarType;
};

class CRGUI_EXPORT crSliderElement : public crElement
{
public:
	crSliderElement();
	crSliderElement(const crSliderElement& element);
	META_Element(CRGUI,crSliderElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual void backup(){ m_backup_value = m_value; }
	virtual void resumeBackupData(){ m_value = m_backup_value; }
	enum SliderBarType
	{
		HORIZONTALBAR,
		VERTICALBAR
	};
	void setSliderBarType(SliderBarType bar);
	SliderBarType getSliderBarType();
	void setRange(const CRCore::crVector2f &range);
	void setRange(float min, float max);
	const CRCore::crVector2f &getRange();
	void setValue(float value);
	float getValue();
	void setSliderBtnWidth(int buttonRange);
	float setSliderBtnWidth();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	float m_value;
	float m_backup_value;
	CRCore::crVector2f m_range;
	int m_sliderBtnWidth;//Slider按钮长度
	int m_lastMouseValue;
	bool m_hold;
	SliderBarType m_sliderBarType;
};

class CRGUI_EXPORT crListBoxElement : public crElement
{
public:
	crListBoxElement():m_select(0),m_backup_select(0),m_rowHeight(15),m_useLeftBtnSelect(true){}
	crListBoxElement(const crListBoxElement& element);
	META_Element(CRGUI,crListBoxElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	void clearData();
	void addData(const std::string& data);
	void deleteCurrentRow();
	void insertData(unsigned int index, const std::string &data);
	int getDataSize();
	bool isDataEmpty();
	bool getData(unsigned int index,std::string &data);
	bool modifyData(unsigned int index,const std::string &data);
    void select(int index);
	void setSelect(int index);
	int getSelect();
	void setRowHeight(int height);
	int getRowHeight();
	virtual void backup(){ m_backup_select = m_select; }
	virtual void resumeBackupData(){ m_select = m_backup_select; }
	virtual CRCore::crVector4f getScrollRange();
	void setUseLeftBtnSelect(bool leftBtnSelect);
	void checkSelect();
protected:
	int getMaxRowCanBeDisplay();
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	typedef std::vector<std::string> DataList;
	DataList m_dataList;
	int m_select;
	DataList m_backup_dataList;
	int m_backup_select;
	int m_rowHeight;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	bool m_useLeftBtnSelect;
	std::string m_listScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_listScrollBar;
	CRCore::crMutex m_dataMutex;
};

class CRGUI_EXPORT crImageBoxElement : public crElement
{
public:
	crImageBoxElement(){}
	crImageBoxElement(const crImageBoxElement& element);
	META_Element(CRGUI,crImageBoxElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual CRCore::crVector4f getScrollRange();
	bool setImageFile( const std::string& file );
	virtual void resize(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	std::string m_imageFile;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
};
//
class CRGUI_EXPORT crStaticTextBoxElement : public crElement
{
public:
	crStaticTextBoxElement(){}
	crStaticTextBoxElement(const crStaticTextBoxElement& element);
	META_Element(CRGUI,crStaticTextBoxElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	//void setTexString(const std::string &text) { m_texString = text; }
	//const std::string &getTexString();
	//virtual void backup(){ m_backup_texString = m_texString; }
	//virtual void resumeBackupData(){ m_texString = m_backup_texString; }
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	//virtual CRCore::crVector4f getScrollRange();
	
	typedef std::vector<std::string> StringArray;
	void lockArray();
	void unLockArray();
	const StringArray& getStringArray() const;//need lock
	StringArray& getStringArray();//need lock
	std::string getString(unsigned int i);
	unsigned int getNumStrings();
	void addString(const std::string& desc);
	std::string getStringArrayInString();
	void setStringArrayByString( const std::string &command );
	void clearString();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
	void calcScroll();
	//std::string m_texString;
	//std::string m_backup_texString;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;

	StringArray m_stringArray;

	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
	CRCore::crMutex m_arrayMutex;
	int m_textFormat;
};

class CRGUI_EXPORT crEditBoxElement : public crElement
{
public:
	crEditBoxElement():m_time(0L),m_hasfocurs(false),m_focusPos(0),m_multiLine(0),m_password(0){}
	crEditBoxElement(const crEditBoxElement& element);
	META_Element(CRGUI,crEditBoxElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	//void setTexString(const std::string &text) { m_texString = text; }
	//const std::string &getTexString();
	//virtual void backup(){ m_backup_texString = m_texString; }
	//virtual void resumeBackupData(){ m_texString = m_backup_texString; }
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual void getFocus();
	virtual void lostFocus();

	virtual bool inputChar(wchar_t c);
	virtual bool inputKey(int key);
	
	//virtual CRCore::crVector4f getScrollRange();
	typedef std::vector<std::string> StringArray;
	void lockArray();
	void unLockArray();
	const StringArray& getStringArray() const;//need lock
	StringArray& getStringArray();//need lock
	std::string getString(unsigned int i);
	unsigned int getNumStrings();
	void addString(const std::string& desc);
	std::string getStringArrayInString();
	void setStringArrayByString( const std::string &command );
	void clearString();
	static float s_focusInterval;
protected:
	bool isFocusOnShow();
	void calcScroll();
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
	//std::string m_texString;
	//std::string m_backup_texString;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;

	StringArray m_stringArray;
	//int m_focusCounter;
	int m_focusPos;
	CRCore::Timer_t m_time;
	bool m_hasfocurs;
    bool m_multiLine;
	bool m_password;

	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
	CRCore::crMutex m_arrayMutex;
	int m_textFormat;
};

class CRGUI_EXPORT crComboBoxElement : public crElement
{
public:
	crComboBoxElement(){}
	crComboBoxElement(const crComboBoxElement& element);
	META_Element(CRGUI,crComboBoxElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateParentElement(crElement *element);
	std::string getValue();
	int getSelectIndex();
	void select(int index);
protected:
	virtual ~crComboBoxElement();
	std::string m_comboEditBoxName;
	std::string m_comboButtonName;
	std::string m_comboListBoxName;
	std::string m_comboScrollBarName;

	CRCore::ref_ptr<crEditBoxElement> m_comboEditBox;
	CRCore::ref_ptr<crButtonElement> m_comboButton;
	CRCore::ref_ptr<crListBoxElement> m_comboList;
	CRCore::ref_ptr<crScrollBarElement> m_comboScrollBar;
    //bool m_canAcceptInput;m_comboEditBox->setCanFocus(false);表示不接受输入
};
//class CRGUI_EXPORT crSpinElement : public crElement//可用两个button模拟
//{
//public:
//	crSliderElement(){}
//	void setTexString(const std::string &text);
//	const std::string &getTexString();
//	virtual void backup(){ m_backup_texString = m_texString; }
//	virtual void resumeBackupData(){ m_texString = m_backup_texString; }
//protected:
//	virtual int drawStageCopy(Display dc, Display down_dc);
//	virtual int drawStageMask(Display dc, Display down_dc);
//	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
//	std::string m_texString;
//	std::string m_backup_texString;
//};

class CRGUI_EXPORT crProgressElement : public crElement
{
public:
	crProgressElement();
	crProgressElement(const crProgressElement& element);
	META_Element(CRGUI,crProgressElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	enum ProgressBarType
	{
		HORIZONTALBAR,
		VERTICALBAR
	};
	void setProgressBarType(ProgressBarType bar);
	ProgressBarType setProgressBarType();
	void updateProgress(float rate);
	void setProgress(float progress);
	float getProgress();
	void resetProgress();
	void fetchEnd();
	bool isProgressFetchEnd();

	//void updateProgress(const CRCore::crVector2f& m_rate);
	//void resetProgress();
	//void fetchEnd();
	//bool isProgressFetchEnd();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	ProgressBarType m_progressBarType;
	float m_progress;
	//CRCore::crVector2i m_progress;
};

class CRGUI_EXPORT crPixelTestElement : public crElement
{
public:
	crPixelTestElement():m_hPixelColorImage(NULL){ m_currentPixelColor = RGB(255,255,255); }
	crPixelTestElement(const crPixelTestElement& element);
	META_Element(CRGUI,crPixelTestElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	void setElementPixelColorImage( const std::string& image, const CRCore::crVector2i& imageSize, const CRCore::crVector2i& elementPos );
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
protected:
	bool nearColor(COLORREF c1, COLORREF c2, int error = 5);
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
	HBITMAP m_hPixelColorImage;
	std::string m_pixelColorImageName;
	COLORREF m_currentPixelColor;
};

class CRGUI_EXPORT crChooseColorElement : public crElement
{
public:
	crChooseColorElement(){ m_color = RGB(255,255,255); }
	crChooseColorElement(const crChooseColorElement& element);
	META_Element(CRGUI,crChooseColorElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	COLORREF getColor();
	void setColor(COLORREF color);
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
	COLORREF m_color;
};
class CRGUI_EXPORT crKeyValueNode : public CRCore::Referenced
{
public:
	crKeyValueNode(const std::string &key, const std::string &value);
	crKeyValueNode(const crKeyValueNode& node);
	virtual crKeyValueNode* clone() const { return new crKeyValueNode(*this); }

	void setKey(const std::string &key);
	void setValue(const std::string &value);
	const std::string &getKey();
	const std::string &getValue();
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	crTextAttribute *getTextAttribute() { return m_textAttribute.get(); }
protected:
	virtual ~crKeyValueNode();
	std::string m_key;
	std::string m_value;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
};
class CRGUI_EXPORT crKeyValueGridElement : public crElement
{
public:
	crKeyValueGridElement():m_rowHeight(15),m_colWidth(50),m_select(0),m_useLeftBtnSelect(true){}
	crKeyValueGridElement(const crKeyValueGridElement& element);
	META_Element(CRGUI,crKeyValueGridElement)
	virtual CRCore::crVector4f getScrollRange();
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	void clearData();
	void addData(const std::string& key,const std::string &value, const std::string &texattr);
	void insertData(unsigned int index, const std::string& key,const std::string &value, const std::string &texattr);
	int getDataSize();
	bool isDataEmpty();
	void select(int index);
	int getSelect();
	//typedef std::pair<std::string,std::string> DataPair;
	//typedef std::vector<DataPair> DataList;
	typedef std::vector< CRCore::ref_ptr<crKeyValueNode> > KeyValueNodeVec;
	//bool getData(unsigned int index,DataPair&data);
	crKeyValueNode *getData(unsigned int index);
	void lockData();
	KeyValueNodeVec &getDataList();//need lock
	void unlockData();
	void setRowHeight(int height);
	int getRowHeight();
	void setColWidth(int width);
	int getColWidth();
	void setUseLeftBtnSelect(bool leftBtnSelect);
	void checkSelect();
	void updateInputElement(bool flg);//true: data->InputElement; false: InputElement->data
	int findDataIndex(const std::string &key);
protected:
	int getMaxRowCanBeDisplay();
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	//DataList m_dataList;
	KeyValueNodeVec m_dataVec;
	int m_select;
	bool m_useLeftBtnSelect;
	int m_rowHeight;
	int m_colWidth;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;

	std::string m_inputElementName;
    CRCore::ref_ptr<crEditBoxElement> m_inputElement;

	CRCore::crMutex m_dataMutex;
};

class CRGUI_EXPORT crTreeNode : public CRCore::Referenced
{
public:
	crTreeNode();
	crTreeNode(const crTreeNode& treenode);
	virtual crTreeNode* clone() const { return new crTreeNode(*this); }

	void setTitle(const std::string &title);
	const std::string &getTitle() const;
	void setSelect(bool select);
	bool getSelect();
	void expand();
	bool getExpand();
	typedef std::vector< HBITMAP > BitMapArray;
	void addBitMap(HBITMAP bitmap);
	void clearBitMap();
	void setCurrentBitmap(unsigned char index);
	unsigned char getCurentBitmap();
	void setData(void *data);
	void *getData();
	typedef std::vector< CRCore::ref_ptr<crTreeNode> > TreeNodeArray;
	void addChild(crTreeNode *child);
	crTreeNode *getChild(unsigned int index);
    TreeNodeArray &getChildArray();
	unsigned int getChildIndex( const crTreeNode* node ) const;
	void removeChild(unsigned int index);
	void removeChild(crTreeNode *child);
	crTreeNode *findChild(const std::string &title);
	crTreeNode *findChildByData(void *data);
	void removeAllChild();

	void setRowHeight(int rowHeight);
	void setColumnOffset(int columnOffset);
	void setElementWidth(int elementWidth);
	int getRowHeight();
	int getColumnOffset();
	int getElementWidth();

	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	void rootDraw(Display dc,int xpos,int ypos,int elementBottom);
	void rootCalcSize(int& xsize, int& ysize);
	crTreeNode *rootSelect(int x, int y);

	void setParent(crTreeNode *parent);
	crTreeNode *getParent();
protected:
	virtual ~crTreeNode();
	void draw(Display dc,int& xpos,int& ypos,int elementBottom);
	void calcSize(int& xsize, int& ysize);
	crTreeNode *select(int x, int y, int& xpos, int& ypos);
	std::string m_title;
    bool m_selected;//是否被选中
	bool m_expand;//是否展开
	BitMapArray m_bitmapArray;
	unsigned char m_currentBitmap;
	void *m_data;
	TreeNodeArray m_childArray;
	crTreeNode *m_parent;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	int m_rowHeight;
	int m_columnOffset;
	int m_elementWidth;
};

class CRGUI_EXPORT crTreeElement : public crElement
{
public:
	crTreeElement(){}
	crTreeElement(const crTreeElement& element);
	META_Element(CRGUI,crTreeElement)
    virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual CRCore::crVector4f getScrollRange();
	void lockTree();
    crTreeNode *getTreeRootNode();
	void unlockTree();
    crTreeNode *getSelectNode();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	CRCore::ref_ptr<crTreeNode> m_root;
	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;

	CRCore::ref_ptr<crTreeNode> m_selectNode;
	CRCore::crMutex m_treeMutex;
};

class CRGUI_EXPORT crColumnNode : public CRCore::Referenced
{
public:
	crColumnNode();
	crColumnNode(const crColumnNode& treenode);
	virtual crColumnNode* clone() const { return new crColumnNode(*this); }

	typedef std::vector<std::string> TitleList;
	void addTitle(const std::string &title);
	bool getTitle(unsigned int index,std::string &title) const;
	typedef std::vector< HBITMAP > BitMapArray;
	void addBitMap(HBITMAP bitmap);
	void clearBitMap();
	void setData(void *data);
	void *getData();
	void draw(Display dc,unsigned int index, RECT &rect);
protected:
	virtual ~crColumnNode();
	TitleList m_titleList;
	BitMapArray m_bitmapArray;
	void *m_data;
};

class CRGUI_EXPORT crColumnListElement : public crElement
{
public:
	crColumnListElement():m_select(0),m_backup_select(0),m_rowHeight(15),m_useLeftBtnSelect(true){}
	crColumnListElement(const crColumnListElement& element);
	META_Element(CRGUI,crColumnListElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	void clearData();
	void addData(crColumnNode *data);
	void deleteCurrentRow();
	void insertData(unsigned int index, crColumnNode *data);
	int getDataSize();
	bool isDataEmpty();
	crColumnNode *getData(unsigned int index);
	void select(int index);
	void setSelect(int index);
	int getSelect();
	void setRowHeight(int height);
	int getRowHeight();
	virtual void backup(){ m_backup_select = m_select; }
	virtual void resumeBackupData(){ m_select = m_backup_select; }
	virtual CRCore::crVector4f getScrollRange();
	void setUseLeftBtnSelect(bool leftBtnSelect);
	void checkSelect();

	void addColumn(const std::string &columnName,int width);
protected:
	int getMaxRowCanBeDisplay();
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	typedef std::pair<std::string, int> ColumnPair;//columnName, width
	typedef std::vector<ColumnPair> ColumnList;
	ColumnList m_columnList;
	typedef std::vector< CRCore::ref_ptr<crColumnNode> > DataList;
	DataList m_dataList;
	int m_select;
	DataList m_backup_dataList;
	int m_backup_select;
	int m_rowHeight;
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	bool m_useLeftBtnSelect;

	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;

	CRCore::crMutex m_dataMutex;
};

class CRGUI_EXPORT crListNode : public CRCore::Referenced
{
public:
	crListNode();
	crListNode(const crListNode& treenode);
	virtual crListNode* clone() const { return new crListNode(*this); }

	void setTitle(const std::string &title);
	const std::string &getTitle() const;
	void setTitle2(const std::string &title2);
	const std::string &getTitle2() const;
	void setSelect(bool select);
	bool getSelect();
	typedef std::vector< HBITMAP > BitMapArray;
	void addBitMapFile(const std::string& file);
	void addBitMap(HBITMAP bitmap);
	void clearBitMap();
	void setCurrentBitmap(unsigned char index);
	unsigned char getCurentBitmap();
	void setData(void *data);
	void *getData();

	void draw(Display dc,Display mdc,int xpos,int ypos,const CRCore::crVector2i& nodeSize,int titleHeight,int format,int format2);
protected:
	virtual ~crListNode();
	std::string m_title;
	std::string m_title2;
	bool m_selected;//是否被选中
	BitMapArray m_bitmapArray;
	unsigned char m_currentBitmap;
	void *m_data;
};
class CRGUI_EXPORT crListControlElement : public crElement
{
public:
	crListControlElement();
	crListControlElement(const crListControlElement& element);
	META_Element(CRGUI,crListControlElement)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	virtual void updateData();
	virtual bool inputChar(wchar_t c);
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);
	virtual CRCore::crVector4f getScrollRange();

	typedef std::list< CRCore::ref_ptr<crListNode> > ListNodeVec;
	void addListNode(crListNode *listNode);
	void removeSelectListNode();
	void clear();
	void lockList();
    ListNodeVec &getListNodeVec();
	void unlockList();
	crListNode *getSelectNode();
protected:
	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }
	void rootCalcSize(int& xsize, int& ysize);
	crListNode *rootSelect(int x, int y);
	void rootDraw(Display dc,Display mdc,int xpos,int ypos);
	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	CRCore::crVector2i m_nodeSize;
	CRCore::crVector2i m_spaceBetween;
	int m_titleHeight;
	int m_titleFormat;
	int m_titleFormat2;

	CRCore::ref_ptr<crListNode> m_selectNode;
	ListNodeVec m_listNodeVec;
	CRCore::crMutex m_listMutex;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
};

}
#endif