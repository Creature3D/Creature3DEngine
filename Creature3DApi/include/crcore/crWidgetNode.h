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
#ifndef CRCORE_CRWIDGETNODE
#define CRCORE_CRWIDGETNODE 1

#include <CRCore/crMatrixTransform.h>

namespace CRCore {
class crCanvasNode;
enum KeyboardMouseMode
{
	KM_None = 0x0,
	KM_Keyboard = 0x01,
	KM_LeftMouse = 0x02,
	KM_MiddleMouse = 0x04,
	KM_RightMouse = 0x08,
	KM_LBtnUp = 0x10,
	KM_RBtnUp = 0x20,
	KM_NotTrans = 0x40,
	KM_MouseUp = KM_LBtnUp | KM_RBtnUp,
	KM_Mouse = KM_LeftMouse | KM_MiddleMouse | KM_RightMouse,
	KM_AllButRM = KM_Keyboard | KM_LeftMouse | KM_MiddleMouse,
	KM_All = KM_Keyboard | KM_Mouse,
	KM_KeyboardAndLBtnUp = KM_Keyboard | KM_LBtnUp,
	KM_KeyboardAndRBtnUp = KM_Keyboard | KM_RBtnUp,
	KM_KeyboardAndMouseUp = KM_Keyboard | KM_MouseUp,
	KM_KeyboardAndLMouse = KM_Keyboard | KM_LeftMouse,
	KM_KeyboardAndRMouse = KM_Keyboard | KM_RightMouse,
};
class CR_EXPORT crWidgetNode : public crMatrixTransform
{
public :
	crWidgetNode();
	crWidgetNode(const crWidgetNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

	META_Node(CRCore, crWidgetNode);
	virtual void traverse(crNodeVisitor& nv);
	virtual void mouseTrans(float &mx, float &my);
	virtual void kernelMouseEvent(float mx, float my,void *ea){}
	virtual void kernelKeyDown(int key, unsigned int modKeyMask){}
	virtual void kernelKeyUp(int key, unsigned int modKeyMask){}
	virtual void kernelGetFocus(){}
	virtual void kernelLostFocus(){}
	virtual void kernelGetCapture(){}
	virtual void kernelLostCapture(){}
	virtual void kernelInputChar(wchar_t c){}
	virtual void initWindow();
	virtual void updateData();
	virtual void setEnable(bool enable);
	inline bool getEnable(){ return m_enable; }

	void setCanFocus(bool canFocus);
	inline bool getCanFocus() { return/* m_enable && */m_canFocus; }

	void setEnableMatrix(bool enable);
	inline bool getEnableMatrix() { return m_matrixEnable; }

	void setCanCaptureMouse(bool canCapture);
	inline bool getCanCaptureMouse(){ return m_canCaptureMouse; }

	void setCanCaptureInput(bool canCapture);
	inline bool getCanCaptureInput(){ return m_canCaptureInput; }

	virtual void setParentCanvas(crCanvasNode *canvas) { m_parentCanvas = canvas; }
	virtual crCanvasNode *getParentCanvas(){ return m_parentCanvas; }

	virtual bool computeLocalToWorldMatrix(crMatrix& matrix, crNodeVisitor*) const;
	virtual bool computeWorldToLocalMatrix(crMatrix& matrix, crNodeVisitor*) const;
	void setParentWidget(crWidgetNode *parent);
	crWidgetNode *getParentWidget();
	virtual void updateParentWidget(crWidgetNode *widget,_crInt32 msg = 0){}
	void swapEnable(float interval);
	void setMsgTraverse(unsigned short mode);
	unsigned short getMsgTraverse();
	void setScaled(bool bln);
	bool getScaled();
protected :
	virtual ~crWidgetNode();
	virtual bool computeBound() const;
	bool   m_canFocus;
	bool   m_enable;
	bool   m_canCaptureMouse;
	bool   m_canCaptureInput;
	bool   m_matrixEnable;//
	crCanvasNode *m_parentCanvas;
	crWidgetNode *m_parentWidget;
	bool m_inited;
	bool m_scaled;
	float m_swapEnableTime;
	KeyboardMouseMode m_msgtraverse;
};
enum CanvasPos
{//窗口层级顺序
	CANVAS_BOTTOM,
	CANVAS_NOTOPMOST,
	CANVAS_TOP,
	CANVAS_TOPMOST,
};
class CR_EXPORT crCanvasNode : public crWidgetNode
{
public :
    crCanvasNode();
    crCanvasNode(const crCanvasNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    META_Node(CRCore, crCanvasNode);
	virtual void initWindow();

	virtual void releaseObjects(crState* = 0);
	crWidgetNode *getWidget(const std::string &name);
	crNode *getChildNode(const std::string &name);
	void setCanvasPos(CanvasPos pos){ m_canvasPos = pos; }
	inline CanvasPos getCanvasPos(){ return m_canvasPos; }
	typedef std::vector< ref_ptr<crWidgetNode> >TabWidgetVec;
	void addTabWidget(const std::string &name);
	virtual void kernelKeyDown(int key,unsigned int modKeyMask);
	void setDefFocusIndex(int index);
protected :
    virtual ~crCanvasNode();
	CanvasPos m_canvasPos;
	TabWidgetVec m_tabWidgetVec;
};
class CR_EXPORT crSetParentCanvasVisitor : public CRCore::crNodeVisitor
{
public:
	crSetParentCanvasVisitor(crCanvasNode *canvas):m_parentCanvas(canvas),crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crSetParentCanvasVisitor(){}

	virtual void apply(crWidgetNode& node){node.setParentCanvas(m_parentCanvas); traverse(node);}
protected:
	crCanvasNode *m_parentCanvas;
};
class CR_EXPORT crAcceptMatrixVisitor : public CRCore::crNodeVisitor
{
public:
	crAcceptMatrixVisitor(const crMatrix &mat):m_mat(mat),crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crAcceptMatrixVisitor(){}

	virtual void apply(crObject& node);
	void setMatrix(const crMatrix &mat){ m_mat = mat; }
protected:
	crMatrix m_mat;
};
}

#endif
