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
#ifndef CRUI_CREXTENDWIDGET
#define CRUI_CREXTENDWIDGET 1

#include <CRUI/crExport.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crMatrixTransform.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRText/crText.h>
#include <CRCore/crMultiSwitch.h>
#include <CRCore/crScissor.h>
#include "rcfg/ConfigScriptOO.h"
#include <string.h>
namespace CRUI {
class CRUI_EXPORT crButtonWidgetNode : public CRCore::crWidgetNode
{
public:
	crButtonWidgetNode();
	crButtonWidgetNode(const crButtonWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crButtonWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	virtual void kernelKeyUp(int key,unsigned int modKeyMask);
	virtual void kernelGetFocus();
	virtual void kernelLostFocus();
	virtual void initWindow();
	virtual void setEnable(bool enable);
	CRCore::crMultiSwitch *getButton(){ return m_button.get(); }
protected:
	virtual ~crButtonWidgetNode();
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_button;
};

class CRUI_EXPORT crScrollBarWidgetNode : public CRCore::crWidgetNode
{
public:
	crScrollBarWidgetNode();
	crScrollBarWidgetNode(const crScrollBarWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crScrollBarWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	virtual void kernelLostFocus();
	virtual void kernelLostCapture();
	virtual void initWindow();
	virtual void updateData();
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
	void outerSetValue(float value);
	float getValue();
	void setLineValue(float lineValue);
	float getLineValue();
	void setPageValue(float pageValue);
	float getPageValue();
protected:
	virtual ~crScrollBarWidgetNode();
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_button0;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_button1;
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_scrollBar;
	float m_value;
	CRCore::crVector2f m_range;
	float m_lineValue;
	float m_pageValue;
	ScrollBarType m_scrollBarType;
	CRCore::crVector2 m_lastmouse;
	bool m_drag;
	float m_lmax;
	int m_step;
};
class CRUI_EXPORT crTextAttrWidgetNode : public CRCore::crWidgetNode
{
public:
	crTextAttrWidgetNode();
	crTextAttrWidgetNode(const crTextAttrWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crTextAttrWidgetNode)
	void setNeedScissor(bool need);
	bool getNeedScissor();
	virtual void initWindow();
	virtual void resize();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	virtual void validFocusPos(){}
	inline void dirtyText() { m_textDirty = true; validFocusPos(); }
	inline virtual void dirtyTextAttr(){}

	virtual void setVisiable( bool visiable );
	void setDrawMode(unsigned int mode);
	unsigned int getDrawMode();
	void setLineSpacing(float lineSpacing);
	float getLineSpacing();
	void setUnderLineSpacing(float underLineSpacing);
	float getUnderLineSpacing();
	void setUpLineSpacing(float upLineSpacing);
	float getUpLineSpacing();
	void setCharacterSize(float characterSize);
	float getCharacterSize();
	void setFontSize(const CRCore::crVector2s& fontSize);
	const CRCore::crVector2s& getFontSize();
	void setFont(const std::string &font);
	const std::string &getFont()const;
	void setColor(const CRCore::crVector4 &color);
	const CRCore::crVector4 &getColor();
	void setAlignment(unsigned char alignment);
	unsigned char getAlignment();
	void setBackdropType(unsigned char backdroptype);
	unsigned char getBackdropType();
	void setBackdropOffset(const CRCore::crVector2& offset);
	const CRCore::crVector2& getBackdropOffset();
	void setBackdropColor(const CRCore::crVector4 &color);
	const CRCore::crVector4 &getBackdropColor();

	const CRCore::crVector4f &getRect() const;
	void setRectOffset(const CRCore::crVector4f &offset);
	const CRCore::crVector4f &getRectOffset() const;

	void setVScrollBarName(const std::string &name);
	const std::string &getVScrollBarName()const;
	static void parseText(std::string &text);//将/n,/r,/t转换回到\n,\r,\t
	static void transformText(std::string &text);//将\n,\r,\t转换成/n,/r,/t

	CRCore::crObject *getTextObject();
	CRCore::crMatrixTransform *getTextTransform();
	float getLineHeight();
	float getUpLineHeight();
	void setDefaultScrollPos(char pos);
	char getDefaultScrollPos();
	crScrollBarWidgetNode *getVScrollBar();
	void scrollChanged();
	void setVScrollBar(crScrollBarWidgetNode* scrollbar);

	void computeRect();
protected:
	virtual ~crTextAttrWidgetNode();
	virtual bool computeBound() const;
	void computeScissor();
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_mvpwNode;
	CRCore::ref_ptr<CRCore::crObject> m_textObject;
	CRCore::ref_ptr<CRCore::crScissor> m_scissor;
	/////Text Attr
	unsigned int m_drawMode;
	CRCore::crVector2s m_fontSize;
	std::string m_font;
	CRCore::crVector4 m_color;
	unsigned char m_alignment;
	unsigned char m_backdropType;
	CRCore::crVector2 m_backdropOffset;
	CRCore::crVector4 m_backdropColor;
	////
	CRCore::crVector4f m_rect;
	CRCore::crVector4f m_rectOffset;
	bool m_scrollDirty;
	bool m_scrollChanged;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarWidgetNode> m_vScrollBar;
	bool m_textDirty;
	char m_defaultScrollPos;//默认为0，1表示最大值,2表示不改变
	bool m_needScissor;//
private:
	float m_characterSize;
	float m_lineSpacing;
	float m_uplineSpacing;
	float m_underLineSpacing;
};
class CRUI_EXPORT crStaticTextWidgetNode : public crTextAttrWidgetNode
{
public:
	crStaticTextWidgetNode();
	crStaticTextWidgetNode(const crStaticTextWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crStaticTextWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void initWindow();
	virtual void resize();
	virtual void updateData();
	inline virtual void dirtyTextAttr() { m_textAttrDirty = true; }
	void setPassword(bool password);
	bool getPassword();
	virtual void setString(std::string &str);
	virtual void setString(const std::wstring &wstr);
	virtual void addString(std::string &str);
	virtual void addString(const std::wstring &wstr);
	const std::wstring& getString()const;
	std::string getUTF8String();
	virtual void clearString();

	void setCanSelectText(bool canSelectText);
	bool getCanSelectText();
	void selectText(int from, int to);
	void selectAllText();
	std::wstring getSelectString();
	virtual void setFocusPos(int pos){}
protected:
	virtual ~crStaticTextWidgetNode();
	bool m_password;
    bool m_canSelectText;
	bool m_textAttrDirty;
	int dirtyImages;
	CRCore::ref_ptr<CRText::crText> m_text;
	CRCore::ref_ptr<CRText::crText> m_seltext;
	////
	CRCore::crVector2i m_textSelect;
	int m_startSelectText;
	std::wstring m_wstring;
};

class CRUI_EXPORT crHypertextWidgetNode : public crTextAttrWidgetNode
{
public:
	crHypertextWidgetNode();
	crHypertextWidgetNode(const crHypertextWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crHypertextWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void initWindow();
	virtual void resize();
	virtual void updateData();

	void setHypertext(std::string filename,bool app = false);
	void setHypertext(rcfg::ConfigScript& hypertext,bool app = false);
	virtual void releaseObjects(CRCore::crState* = 0);
	void getString(unsigned int i,std::wstring &outstr);
	void modifyString(unsigned int i,const std::wstring &instr);
	void modifyString(unsigned int i,const std::string &instr);
	int getValidTextSize();
	void popTexts(unsigned int count);//去掉前面几个文字
	void clearText();
	void setSingleLine(bool bIsSingle = true);
	bool isSingleLine()const;
	typedef std::pair< std::wstring,CRCore::ref_ptr<CRText::crText> > LinkInfoPair;
protected:
	virtual ~crHypertextWidgetNode();
	typedef std::pair< CRCore::ref_ptr<CRText::crText>, std::wstring > TextPair;
	typedef std::vector<TextPair> TextVec;
	typedef std::vector< std::pair<CRCore::ref_ptr<CRCore::crMatrixTransform>, int>>ImageNodeVec;
	typedef std::vector< std::pair<CRCore::ref_ptr<CRCore::crMatrixTransform>, int>>ModelNodeVec;
	TextVec m_textArray;
	CRCore::crCriticalMutex m_textArrayMutex;
	typedef std::map< CRCore::ref_ptr<CRText::crText>, int > LinkMap;
	LinkMap m_linkMap;
	CRCore::ref_ptr<CRText::crText> m_lastHitText;
	int m_validTextSize;
	int m_validImageSize;
	int m_validModelSize;
	std::string m_hyperFile;
	CRCore::ref_ptr<CRCore::crGroup> m_imageRoot;
	CRCore::ref_ptr<CRCore::crGroup> m_modelRoot;
	ImageNodeVec m_imageNodeVec;
	ModelNodeVec m_modelNodeVec;
	CRCore::ref_ptr<CRText::crText> m_text;
	bool m_bSingleLine;
//private:
//	typedef std::vector<crDrawable *> Line;
//	typedef std::vector<std::pair<ref_ptr<Line>, int>> LineTable;
};

class CRUI_EXPORT crEditWidgetNode : public crStaticTextWidgetNode
{
public:
	crEditWidgetNode();
	crEditWidgetNode(const crEditWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crEditWidgetNode)
	virtual void initWindow();
	virtual void resize();
	virtual void updateData();
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelInputChar(wchar_t c);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	virtual void setFocusPos(int pos);
	virtual void kernelGetCapture();
	virtual void kernelLostCapture();
	void setMultiLine(bool multiline);
	bool getMultiLine();
	void setNumeric(bool numeric);
	bool getNumeric();
	void setAutoSelectAll(bool autoselect);
	bool getAutoSelectAll();
	virtual void clearString();
	virtual void validFocusPos();
	virtual void setAlignment(unsigned char alignment);
	virtual void setString(std::string &str);
	virtual void setString(const std::wstring &wstr);
	virtual void addString(std::string &str);
	virtual void addString(const std::wstring &wstr);
protected:
	virtual ~crEditWidgetNode();
	bool m_multiLine;
	bool m_numeric;
	bool m_autoSelectAll;
	CRCore::ref_ptr<CRText::crText> m_focusCursor;
	int m_focusPos;
	float m_focusInterval;
	bool m_focusChanged;
	static float s_focusInterval;
};

class CRUI_EXPORT crListBoxWidgetNode : public crStaticTextWidgetNode
{
public:
	crListBoxWidgetNode();
	crListBoxWidgetNode(const crListBoxWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crListBoxWidgetNode)
	virtual void initWindow();
	virtual void updateData();
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);

	void clearData();
	void addData(std::string data);
	void deleteCurrentRow();
	void insertData(unsigned int index, const std::wstring &data);
	int getDataSize();
	bool isDataEmpty();
	bool getData(unsigned int index,std::wstring &data);
	bool modifyData(unsigned int index,const std::wstring &data);
	void select(unsigned int index);
	unsigned int getSelect();
	void setMouseMoveSelect(bool mouseMoveSelect);
	bool validSelect();
protected:
	virtual ~crListBoxWidgetNode();
	std::wstring getStringArrayInString();

	typedef std::vector<std::wstring> DataList;
	DataList m_dataList;
	unsigned int m_select;
	bool m_mouseMoveSelect;
};

class CRUI_EXPORT crImageBoxWidgetNode : public CRCore::crWidgetNode
{
public:
	crImageBoxWidgetNode();
	crImageBoxWidgetNode(const crImageBoxWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crImageBoxWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	virtual void initWindow();
	virtual void updateData();
	void setImageName(const std::string &image,unsigned int i = 0);
	void setImage(CRCore::crImage *image,unsigned int i = 0);
	const std::string &getImageName(unsigned int i = 0)const;
	void clearImage();
	//void setRectOffset(const CRCore::crVector4f &offset);
	//const CRCore::crVector4f &getRectOffset() const;
	void setCanDragImage(bool canDrag);
	bool getCanDragImage() const;
	CRCore::crObject *getImageObject();
	CRCore::crStateSet *getImageStateSet();
	CRCore::crGeometry *getImageGeometry();
	void dirtyImage();
protected:
	virtual ~crImageBoxWidgetNode();
	std::string m_imageFile[8];
	CRCore::ref_ptr<CRCore::crObject> m_imageObject;
	CRCore::ref_ptr<CRCore::crGeometry> m_imageQuad;
	CRCore::ref_ptr<CRCore::crStateSet> m_imageStateSet;
	//CRCore::crVector4f m_rectOffset;
	bool m_canDragImage;
	bool m_imageDirty;
};

class CRUI_EXPORT crCheckBoxWidgetNode : public CRCore::crWidgetNode
{
public:
	crCheckBoxWidgetNode();
	crCheckBoxWidgetNode(const crCheckBoxWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crCheckBoxWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyUp(int key,unsigned int modKeyMask);
	virtual void initWindow();
	virtual void updateData();
	void select(bool sel);
	bool getSelect() { return m_select; }
	CRCore::crMultiSwitch *getButton(){ return m_button.get(); }
protected:
	virtual ~crCheckBoxWidgetNode();
	bool m_select;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_button;
};

class CRUI_EXPORT crRadioWidgetNode : public CRCore::crWidgetNode
{
	friend class crRadioGroupWidgetNode;
public:
	crRadioWidgetNode();
	crRadioWidgetNode(const crRadioWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crRadioWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyUp(int key,unsigned int modKeyMask);
	virtual void initWindow();
	virtual void updateData();
	virtual void setEnable(bool enable);
	CRCore::crMultiSwitch *getButton(){ return m_button.get(); }
	inline bool getSelect() { return m_select; }
	void select(bool sel);
	void setAcceptDrag(bool acceptDrag);
	void setMouseMoveSelect(bool mouseMoveSelect);
protected:
	virtual ~crRadioWidgetNode();
	void kernelSelect(bool sel);
	void setRaidoGroup(crRadioGroupWidgetNode *radioGroup);
	bool m_select;
	bool m_acceptDrag;
	bool m_mouseMoveSelect;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_button;
};
class CRUI_EXPORT crRadioGroupWidgetNode : public CRCore::crWidgetNode
{
public:
	crRadioGroupWidgetNode();
	crRadioGroupWidgetNode(const crRadioGroupWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crRadioGroupWidgetNode)
	void addRadioName(const std::string &name);
	virtual void initWindow();
	void select(int index);
	int getSelect();
	crRadioWidgetNode *getCurrentSelectedRadio();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	typedef std::pair< std::string, crRadioWidgetNode* > RadioNamePair;
	typedef std::vector<RadioNamePair> RadioGroup;
	RadioGroup &getRadioGroup();
	crRadioWidgetNode* getRadio(int i);
	int getRadioIndex(crRadioWidgetNode* radio);
	virtual void clear(){ m_radioGroup.clear(); }
protected:
	virtual ~crRadioGroupWidgetNode();
	RadioGroup m_radioGroup;
	int m_currentSelect;
};
class CRUI_EXPORT crProgressWidgetNode : public CRCore::crWidgetNode
{
public:
	crProgressWidgetNode();
	crProgressWidgetNode(const crProgressWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crProgressWidgetNode)
	virtual void initWindow();
	virtual void resize();
	virtual void updateData();
	enum ProgressBarType
	{
		HORIZONTALBAR,
		VERTICALBAR
	};
	void setProgressBarType(ProgressBarType bar);
	ProgressBarType getProgressBarType();
	void updateProgress(float rate);
	void setProgress(float progress);
	float getProgress();
	void resetProgress();
	void fetchEnd();
	bool isProgressFetchEnd();
	CRCore::crVector2 getBarPosition();
	CRCore::crVector2 getBarOffsetPosition();
protected:
	virtual ~crProgressWidgetNode();
	void computeScissor();
	ProgressBarType m_progressBarType;
	float m_progress;
	CRCore::crVector4f m_rectOffset;

	CRCore::crVector4f m_rect;
	CRCore::ref_ptr<CRCore::crNode> m_upObject;
	CRCore::ref_ptr<CRCore::crScissor> m_scissor;
};
class CRUI_EXPORT crSliderWidgetNode : public crProgressWidgetNode
{
public:
	crSliderWidgetNode();
	crSliderWidgetNode(const crSliderWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crSliderWidgetNode)
	virtual void initWindow();
	virtual void updateData();
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	virtual void kernelLostFocus();
	virtual void kernelLostCapture();
	void setValue(float value);
	float getValue();
	void setLineValue(float lineValue);
	float getLineValue();
protected:
	virtual ~crSliderWidgetNode();
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_sliderBar;
	float m_lineValue;
	CRCore::crVector2 m_lastmouse;
	bool m_drag;
};
class CRUI_EXPORT crChooseColorWidgetNode : public CRCore::crWidgetNode
{
public:
	crChooseColorWidgetNode();
	crChooseColorWidgetNode(const crChooseColorWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crChooseColorWidgetNode)
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void updateData();
	virtual void initWindow();
	void setColor(const CRCore::crVector4 &color);
	const CRCore::crVector4 &getColor()const;
protected:
	virtual ~crChooseColorWidgetNode();
	CRCore::crVector4 m_color;
	CRCore::ref_ptr<CRCore::crDrawable> m_drawable;
};
class CRUI_EXPORT crTableWidgetNode : public CRCore::crWidgetNode
{//代替crColumnListElement
public:
	crTableWidgetNode();
	crTableWidgetNode(const crTableWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crTableWidgetNode)
	virtual void initWindow();
	virtual void resize();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	virtual void updateData();
	void addData(unsigned int col, unsigned int row, const std::string &data);
	bool getData(unsigned int col, unsigned int row, std::wstring &outdata);
	void selectRow(int row);
	void clearData();
	void clearColumn();
	void addColumn(const std::string &columnName);
	unsigned int getSelect();
	void setDefNull(int defnull);
	void addData(int row,int data);
	int getData(int row);
	int getSelectData();
	void setHasTitle(bool hasTitle);
	bool getHasTitle();
	int getRowCount();
	void setVScrollBarName(const std::string &name);
	const std::string &getVScrollBarName()const;
	void setMouseMoveSelect(bool mouseMoveSelect);
protected:
	virtual ~crTableWidgetNode();
	typedef std::pair< std::string, crListBoxWidgetNode* > ListNamePair;
	typedef std::vector<ListNamePair> ListGroup;
	typedef std::vector<int> DataVec;
	ListGroup m_listGroup;
	DataVec m_dataVec;
	unsigned int m_currentSelectRow;
	bool m_hasTitle;//是否有title
	CRCore::ref_ptr<CRCore::crNode> m_selectMask;
	void computeMaskScissor();
	CRCore::ref_ptr<CRCore::crScissor> m_maskScissor;
	float m_maskHeight;
	int m_defnull;
	bool m_mouseMoveSelect;
	//
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarWidgetNode> m_vScrollBar;
};
class CRUI_EXPORT crComboBoxWidgetNode : public CRCore::crWidgetNode
{
public:
	crComboBoxWidgetNode();
	crComboBoxWidgetNode(const crComboBoxWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crComboBoxWidgetNode)
		virtual void initWindow();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	void select(unsigned int index);
	std::wstring getValue();
	unsigned int getSelectIndex();
	void setComboTextBoxName(const std::string &textbox);
	void setComboButtonName(const std::string &button);
	void setComboListBoxName(const std::string &list);
protected:
	virtual ~crComboBoxWidgetNode();
	std::string m_comboTextBoxName;
	std::string m_comboButtonName;
	std::string m_comboListBoxName;
	CRCore::ref_ptr<crStaticTextWidgetNode> m_comboTextBox;
	CRCore::ref_ptr<crButtonWidgetNode> m_comboButton;
	CRCore::ref_ptr<crTableWidgetNode> m_comboList;
};
class CRUI_EXPORT crListControlNode : public crTextAttrWidgetNode
{
public:
	crListControlNode();
	crListControlNode(const crListControlNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	const CRCore::crVector4f &getRect() const;
	META_Node(CRUI,crListControlNode)
	void setRect(CRCore::crVector4f &rect);
	virtual void updateData();
	///////Image
	void setImageName(const std::string &image,unsigned int i = 0);
	const std::string &getImageName(unsigned int i = 0)const;
	void setRectOffset(const CRCore::crVector4f &offset);
	const CRCore::crVector4f &getRectOffset() const;
	CRCore::crObject *getImageObject();
	CRCore::crGeometry *getImageQuad();
	void dirtyImage();
	////////Text
	void setTextPosOffset(CRCore::crVector2 &offset);
	CRText::crText *getTitleText();
	CRText::crText *getTitleText2();
	//list node
	void setSelect(bool select);
	bool getSelect();
	void setData(void* data);
	void setData(int data);
	int getData();
	virtual void setVisiable( bool visiable );

	void setTitle(const std::string &title);
	const std::string &getTitle() const;
	void setTitle2(const std::string &title2);
	const std::string &getTitle2() const;
protected:
	virtual ~crListControlNode();
	std::string m_imageFile[8];
	CRCore::ref_ptr<CRCore::crObject> m_imageObject;
	CRCore::ref_ptr<CRCore::crGeometry> m_imageQuad;
	CRCore::ref_ptr<CRCore::crStateSet> m_imageStateSet;
	bool m_imageDirty;

	//CRCore::ref_ptr<CRText::crText> m_text;
	//std::wstring m_wstring;
	CRCore::crVector2 m_texPosOffset;
	//list node
	bool m_selected;//是否被选中
	//void *m_data;
	int m_data;
	std::string m_title;
	std::string m_title2;
	CRCore::ref_ptr<CRText::crText> m_titleText;
	CRCore::ref_ptr<CRText::crText> m_titleText2;
};
class CRUI_EXPORT crListControlWidgetNode : public CRCore::crWidgetNode
{//
public:
	crListControlWidgetNode();
	crListControlWidgetNode(const crListControlWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crListControlWidgetNode)
	int MouseOnNodeIndex(float mx, float my);
	virtual void kernelMouseEvent(float mx, float my,void *ea);
	virtual void initWindow();
	virtual void resize();
	virtual void updateData();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	void setStartYPos(int pagenum);
	short getListNodeCount();
	void setListNodeCount(short count);
	typedef std::vector< CRCore::ref_ptr<crListControlNode> > ListNodeVec;
	void removeSelectListNode();
	void clearList();
    ListNodeVec &getListNodeVec();
	crListControlNode *getSelectNode();
	crListControlNode *getListNode(int i);
	void select(short i);
	void setDefaultScrollPos(char pos);
	char getDefaultScrollPos();
	const CRCore::crVector4f &getRect() const;
	void setRectOffset(const CRCore::crVector4f &offset);
	const CRCore::crVector4f &getRectOffset() const;
	void setVScrollBarName(const std::string &name);
	const std::string &getVScrollBarName()const;
	void setNodeSize(CRCore::crVector2f &nodesize);
	const CRCore::crVector2f &getNodeSize()const;
	void setSpaceBetween(CRCore::crVector2f &space);
	const CRCore::crVector2f &getSpaceBetween()const;
	void setSelectColor(const CRCore::crVector4f &color);
	const CRCore::crVector4f &getSelectColor()const;
	void setCanDragNode(bool can);
	bool getCanaDragNode();
	void setMouseMoveSelect(bool mouseMoveSelect);
	CRCore::crMatrixTransform *getMvpwNode();
protected:
	virtual ~crListControlWidgetNode();
	virtual bool computeBound() const;
	void computeScissor();
	//crListControlNode *select(int x, int y);
	CRCore::crVector2f m_nodeSize;
	CRCore::crVector2f m_spaceBetween;
	CRCore::crVector2f m_startOffset;

	ListNodeVec m_listNodeVec;
	short m_validNodeCount;//
	short m_select;

	CRCore::ref_ptr<CRCore::crMatrixTransform> m_mvpwNode;
	CRCore::ref_ptr<CRCore::crGroup> m_imageGroup;
	CRCore::ref_ptr<CRCore::crGroup> m_textGroup;
	CRCore::ref_ptr<CRCore::crScissor> m_scissor;

	CRCore::crVector4f m_rect;
	CRCore::crVector4f m_rectOffset;
	bool m_scrollDirty;
	bool m_scrollChanged;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarWidgetNode> m_vScrollBar;
	char m_defaultScrollPos;//默认为0，1表示最大值,2表示不改变
	CRCore::ref_ptr<crMatrixTransform> m_currentDragNode;
	CRCore::crVector4 m_selectColor;
	bool m_canDragNode;
	bool m_mouseMoveSelect;
	float m_startYPos;
};
class CRUI_EXPORT crScissorWidgetNode : public CRCore::crWidgetNode
{
public:
	crScissorWidgetNode();
	crScissorWidgetNode(const crScissorWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crScissorWidgetNode)
	virtual void updateData();
	virtual void initWindow();
	virtual void resize();
protected:
	virtual ~crScissorWidgetNode();
	virtual bool computeBound() const;
	void computeScissor();
	CRCore::crVector4f m_rect;
	CRCore::ref_ptr<CRCore::crScissor> m_scissor;
};
class CRUI_EXPORT crGroupWidgetNode : public CRCore::crWidgetNode
{
public:
	crGroupWidgetNode();
	crGroupWidgetNode(const crGroupWidgetNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Node(CRUI,crGroupWidgetNode)
	void addWidgetName(const std::string &name);
	virtual void initWindow();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0);
	typedef std::pair< std::string, crWidgetNode* > WidgetNamePair;
	typedef std::vector<WidgetNamePair> WidgetGroup;
	WidgetGroup &getWidgetGroup();
	crWidgetNode* getWidget(int i);
	int getWidgetIndex(crWidgetNode* widget);
	virtual void clear(){ m_widgetGroup.clear(); }
protected:
	virtual ~crGroupWidgetNode();
	WidgetGroup m_widgetGroup;
};
//class CRUI_EXPORT crTreeNode : public crTextAttrWidgetNode
//{
//public:
//	crTreeNode();
//	crTreeNode(const crTreeNode&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
//	META_Node(CRUI,crTreeNode)
//	void setRect(CRCore::crVector4f &rect);
//	virtual void updateData();
//
//	///////Image
//	void setImageName(const std::string &image);
//	const std::string &getImageName()const;
//	void setRectOffset(const CRCore::crVector4f &offset);
//	const CRCore::crVector4f &getRectOffset() const;
//	CRCore::crObject *getImageObject();
//	CRCore::crGeometry *getImageQuad();
//	void dirtyImage();
//
//	void setTitle(const std::string &title);
//	const std::string &getTitle() const;
//	void setSelect(bool select);
//	bool getSelect();
//	void expand();
//	bool getExpand();
//	void setData(void *data);
//	void setData(int data);
//	int getData();
//	virtual void setVisiable( bool visiable );
//
//	typedef std::vector< CRCore::ref_ptr<crTreeNode> > TreeNodeArray;
//	void addChild(crTreeNode *child);
//	crTreeNode *getChild(unsigned int index);
//	TreeNodeArray &getChildArray();
//	unsigned int getChildIndex( const crTreeNode* node ) const;
//	void removeChild(unsigned int index);
//	void removeChild(crTreeNode *child);
//	crTreeNode *findChild(const std::string &title);
//	crTreeNode *findChildByData(void *data);
//	void removeAllChild();
//
//	void setRowHeight(int rowHeight);
//	void setColumnOffset(int columnOffset);
//	void setWidgetWidth(int width);
//	int getRowHeight();
//	int getColumnOffset();
//	int getWidgetWidth();
//
//	void rootDraw(Display dc,int xpos,int ypos,int elementBottom);
//	void rootCalcSize(int& xsize, int& ysize);
//	crTreeNode *rootSelect(int x, int y);
//
//	void setParent(crTreeNode *parent);
//	crTreeNode *getParent();
//protected:
//	virtual ~crTreeNode();
//	void draw(Display dc,int& xpos,int& ypos,int elementBottom);
//	void calcSize(int& xsize, int& ysize);
//	crTreeNode *select(int x, int y, int& xpos, int& ypos);
//	std::string m_title;
//	bool m_selected;//是否被选中
//	bool m_expand;//是否展开
//	BitMapArray m_bitmapArray;
//	unsigned char m_currentBitmap;
//	void *m_data;
//	TreeNodeArray m_childArray;
//	crTreeNode *m_parent;
//	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
//	int m_rowHeight;
//	int m_columnOffset;
//	int m_elementWidth;
//};
}

#endif
