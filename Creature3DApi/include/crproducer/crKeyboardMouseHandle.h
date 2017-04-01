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
#ifndef CRPRODUCER_CRKEYBOARDMOUSEHANDLE_H
#define CRPRODUCER_CRKEYBOARDMOUSEHANDLE_H 1

#include <CRProducer\crExport.h>
#include <CRCore\crVector2.h>
#include <CRCore\Referenced.h>
#include <CRCore\ref_ptr.h>
#include <CRCore/crWchMsg.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRProducer/crKeyboardMouseCallback.h>
namespace CRProducer{
class crViewer;
class CRPRODUCER_EXPORT crKeyboardMouseHandle: public CRCore::crThread, public CRCore::Referenced
{
public:
	crKeyboardMouseHandle();
	static crKeyboardMouseHandle *getInstance();

	virtual void clear();

	virtual void run();

	inline void updateBlock() { if(m_updateBarrier) m_updateBarrier->block(); }
	inline void frameBlock() { if(m_frameBarrier) m_frameBarrier->block(); }
	CRCore::crBarrier *getUpdateBlock();
	CRCore::crBarrier *getFrameBlock();
	void done();

	void handle(CRProducer::crGUIEventAdapter* ea);
	void setBindViewer(CRProducer::crViewer *viewer){ m_bindViewer = viewer; }
	CRProducer::crViewer *getBindViewer(){ return m_bindViewer.get(); }
	_crInt32 getMouseMsg(CRProducer::crGUIEventAdapter* ea);
	void lockInput();
	void unlockInput();
	bool isLocked(){ return m_lockinput; }
protected:
	virtual ~crKeyboardMouseHandle();
	static CRCore::ref_ptr<crKeyboardMouseHandle> m_instance;
	CRCore::ref_ptr<CRProducer::crViewer> m_bindViewer;
	CRCore::ref_ptr<CRCore::crWidgetNode> m_pressedUI;
	bool m_lockinput;
	CRProducer::crKeyboardMouseCallback::EventQueue  m_queue;
private:
	CRCore::crBarrier *m_updateBarrier;
	CRCore::crBarrier *m_frameBarrier;
	bool m_done;
};	
//class CRPRODUCER_EXPORT crKeyboardMouseHandle : public CRCore::Referenced
//{
//public :
//  crKeyboardMouseHandle();
//  static crKeyboardMouseHandle *getInstance();
//  virtual void clear();
//
//  virtual void handle(CRProducer::crGUIEventAdapter* ea);
//
//  void setBindViewer(CRProducer::crViewer *viewer){ m_bindViewer = viewer; }
//  CRProducer::crViewer *getBindViewer(){ return m_bindViewer; }
//  _crInt32 getMouseMsg(CRProducer::crGUIEventAdapter* ea);
//  void lockInput();
//  void unlockInput();
//  bool isLocked(){ return m_lockinput; }
//protected:
//  virtual ~crKeyboardMouseHandle();
//  static CRCore::ref_ptr<crKeyboardMouseHandle> m_instance;
//  CRProducer::crViewer *m_bindViewer;
//  CRCore::crWidgetNode *m_pressedUI;
//  bool m_lockinput;
//};

}

#endif