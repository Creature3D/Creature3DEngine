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
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRPhysics/crCamera.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crStatistics.h>
using namespace CRCore;
using namespace CRProducer;
using namespace CRPhysics;
ref_ptr<crKeyboardMouseHandle> crKeyboardMouseHandle::m_instance = NULL;

crKeyboardMouseHandle* crKeyboardMouseHandle::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crKeyboardMouseHandle;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crKeyboardMouseHandle::clear()
{
	m_instance = NULL;
}

crKeyboardMouseHandle::crKeyboardMouseHandle()
	:m_done(true),
	m_bindViewer(NULL),
	m_pressedUI(NULL),
	m_lockinput(false)
{
	m_updateBarrier = new CRCore::crBarrier(2);
	m_frameBarrier = new CRCore::crBarrier(2);
}

crKeyboardMouseHandle::~crKeyboardMouseHandle() 
{
	m_done = true;
	if(m_updateBarrier) m_updateBarrier->invalidate();
	if(m_frameBarrier) m_frameBarrier->invalidate();
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}

	if(m_updateBarrier)
	{
		delete m_updateBarrier; 
		m_updateBarrier = NULL;
	}
	if(m_frameBarrier)
	{
		delete m_frameBarrier;
		m_frameBarrier = NULL;
	}
	m_bindViewer = NULL;
	m_pressedUI = NULL;
}

void crKeyboardMouseHandle::done()
{
	m_done = true;
	//m_physicsUpdateBarrier->release();
	//m_frameBarrier->release();
	//m_physicsUpdateBarrier->block(1);
	//m_frameBarrier->block(1);
}

CRCore::crBarrier *crKeyboardMouseHandle::getUpdateBlock()
{
	return m_updateBarrier;
}

CRCore::crBarrier *crKeyboardMouseHandle::getFrameBlock()
{
	return m_frameBarrier;
}

void crKeyboardMouseHandle::run()
{
#if _DEBUG
	try
	{
#endif
		m_done = false;
		CRCore::Timer_t start_tick,end_tick;
		while(!m_done)
		{
			updateBlock();
			frameBlock();
			if(!m_done)
			{
				if(crStatistics::getInstance()->getStat())
				{
					start_tick = CRCore::Timer::instance()->tick();
				}
				if (m_bindViewer->m_kbmcb.valid()) 
				{
					m_bindViewer->m_kbmcb->getEventQueue(m_queue);
				}
				//if(CRCore::crFrameStamp::getInstance()->getFrameNumber()>20)
				//{
					for( CRProducer::crKeyboardMouseCallback::EventQueue::iterator event_itr=m_queue.begin();
						event_itr!=m_queue.end();
						++event_itr )
					{
						handle(event_itr->get());
					}
				//}
				if(crStatistics::getInstance()->getStat())
				{
					end_tick = CRCore::Timer::instance()->tick();
					float ms = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
					crStatistics::getInstance()->setKeyboardMouseTime(ms);
				}
			}
		}
#if _DEBUG
		CRCore::notify(CRCore::ALWAYS)<<"crKeyboardMouseHandle end"<<std::endl;
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crKeyboardMouseHandle::run() error "<<std::endl;
	}
#endif
}

//void crKeyboardMouseHandle::clear()
//{
//	m_instance = NULL;
//}
//
//crKeyboardMouseHandle::crKeyboardMouseHandle():
//m_bindViewer(NULL),
//m_pressedUI(NULL),
//m_lockinput(false)
//{
//}
//crKeyboardMouseHandle::~crKeyboardMouseHandle()
//{
//	m_bindViewer = NULL;
//	m_pressedUI = NULL;
//}
void crKeyboardMouseHandle::lockInput()
{
	m_lockinput = true;
}
void crKeyboardMouseHandle::unlockInput()
{
	m_lockinput = false;
}
void crKeyboardMouseHandle::handle(CRProducer::crGUIEventAdapter* ea)
{
	bool lock = m_lockinput && crFilterRenderManager::getInstance()->getModalCanvas()==NULL;
    if(!lock && m_bindViewer.valid())
	{
		//if(ea->getEventType() == crGUIEventAdapter::MOVE)
		//{//3DUI消息处理
		//	crVector2 mouse(ea->getXnormalized(),ea->getYnormalized()+m_bindViewer->getUIMouseOffset());
		//	crNode *pickUI = NULL;
		//	crNode *windowUI = NULL;
		//	crFilterRenderManager::getInstance()->mousePick(mouse,pickUI,windowUI);
		//	if(pickUI)
		//	{
		//		crFilterRenderManager::getInstance()->setFocusNode(pickUI);
		//		pickUI->doEvent(MAKEINT32(NULL,ea->getEventType()),MAKEINT64(ea,NULL));
		//		return;
		//	}
		//	else if(windowUI)
		//	{
		//		crFilterRenderManager::getInstance()->setFocusNode(windowUI);
		//		windowUI->doEvent(MAKEINT32(NULL,ea->getEventType()),MAKEINT64(ea,NULL));
		//		return;
		//	}
		//	else
		//	{
		//		crFilterRenderManager::getInstance()->setFocusNode(NULL);
		//	}
		//}
		//else if(ea->getButton()!=crGUIEventAdapter::RIGHT_MOUSE_BUTTON && ea->getEventType() >= crGUIEventAdapter::PUSH && ea->getEventType() <= crGUIEventAdapter::DRAG)
		//{//3DUI消息处理
		//	crVector2 mouse(ea->getXnormalized(),ea->getYnormalized()+m_bindViewer->getUIMouseOffset());
		//	crNode *pickUI = NULL;
		//	crNode *windowUI = NULL;
		//	crFilterRenderManager::getInstance()->mousePick(mouse,pickUI,windowUI);
		//	if(pickUI)
		//	{
		//		pickUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
		//		return;
		//	}
		//	else if(windowUI)
		//	{
		//		windowUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
		//		return;
		//	}
		//}

		//3DUI Event
		if(/*ea->getButton()!=crGUIEventAdapter::RIGHT_MOUSE_BUTTON && */ea->getEventType() >= crGUIEventAdapter::PUSH && ea->getEventType() <= crGUIEventAdapter::MOVE)
		{//3DUI消息处理
			crVector2 mouse(ea->getXnormalized(),ea->getYnormalized()/*+m_bindViewer->getUIMouseOffset()*/);
			crFilterRenderManager::getInstance()->setMousePosition(mouse);
			crWidgetNode *pickUI = NULL;
			crFilterRenderManager::getInstance()->mousePick(mouse,pickUI);
			if(pickUI)
			{
				do 
				{
					//if(ea->getEventType() == crGUIEventAdapter::MOVE)
						crFilterRenderManager::getInstance()->setFocusNode(pickUI);
					if (pickUI->getEnable() && ea->getEventType() == crGUIEventAdapter::PUSH && ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
					{
						m_pressedUI = pickUI;
						if(pickUI->getCanCaptureMouse())
							crFilterRenderManager::getInstance()->setMouseCapturer(pickUI);
						if(pickUI->getCanCaptureInput())
							crFilterRenderManager::getInstance()->setInputCapturer(pickUI);
						else
							crFilterRenderManager::getInstance()->setInputCapturer(NULL);

						crCanvasNode *pickCanvas = dynamic_cast<crCanvasNode *>(pickUI);
						if(!pickCanvas)
							pickCanvas = pickUI->getParentCanvas();
						crFilterRenderManager::getInstance()->setActiveCanvas(pickCanvas);
					}
					else if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON && ea->getEventType() == crGUIEventAdapter::RELEASE)
					{
						crWidgetNode *capturer = crFilterRenderManager::getInstance()->getMouseCapturer();
						if(capturer == pickUI)
							crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
					}
					if (pickUI->getEnable())
					{
						pickUI->kernelMouseEvent(mouse[0],mouse[1],ea);
						if(ea->getEventType() == crGUIEventAdapter::MOVE)
							pickUI->doEvent(MAKEINT32(NULL,ea->getEventType()),MAKEINT64(ea,NULL));
						else if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON && ea->getEventType() == crGUIEventAdapter::RELEASE)
						{
							if(m_pressedUI == pickUI)
								pickUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
						}
						/*else if(ea->getEventType() == crGUIEventAdapter::DRAG)
						pickUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));*/
						else
						{
							pickUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
						}
					}
					else if (ea->getEventType() == crGUIEventAdapter::MOVE)
						pickUI->doEvent(MAKEINT32(NULL, ea->getEventType()), MAKEINT64(ea, NULL));
					//else if(ea->getEventType() == crGUIEventAdapter::DRAG || ea->getEventType() == crGUIEventAdapter::RELEASE)
					//	pickUI->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				} while (0);
				////////Mouse Butten Event
				if(ea->getEventType() == crGUIEventAdapter::PUSH || ea->getEventType()== crGUIEventAdapter::RELEASE || ea->getEventType()== crGUIEventAdapter::DOUBLECLICK)
				{
					unsigned short msgflg = pickUI->getMsgTraverse();
					if(msgflg == KM_None)
					{
						if(pickUI->getParentCanvas())
						{
							msgflg = pickUI->getParentCanvas()->getMsgTraverse();
						}
					}
					if(!(msgflg & CRCore::KM_LeftMouse) && ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
					{
						if(!((msgflg & CRCore::KM_LBtnUp) && ea->getEventType() == crGUIEventAdapter::RELEASE))
							return;
					}
					if(!(msgflg & CRCore::KM_MiddleMouse) && ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
					{
						return;
					}
					if(!(msgflg & CRCore::KM_RightMouse) && ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
					{
						if(!((msgflg & CRCore::KM_RBtnUp) && ea->getEventType() == crGUIEventAdapter::RELEASE))
							return;
					}
				}
				if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON && (ea->getEventType() == crGUIEventAdapter::PUSH || ea->getEventType() == crGUIEventAdapter::RELEASE))
					return;
			}
			else
			{
				//if(ea->getEventType() == crGUIEventAdapter::MOVE)
					crFilterRenderManager::getInstance()->setFocusNode(NULL);
				/*else */if (ea->getEventType() == crGUIEventAdapter::PUSH && ea->getButton()==crGUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
					crFilterRenderManager::getInstance()->setInputCapturer(NULL);
					m_pressedUI = NULL;
				}
			}
		}
		else if(ea->getEventType() > crGUIEventAdapter::MOVE)
		{
			crWidgetNode *inputWidget = crFilterRenderManager::getInstance()->getCurrentInputWidget();
			if(inputWidget/* && inputWidget->getEnable()*/)
			{
				switch(ea->getEventType())
				{
				case(crGUIEventAdapter::KEYDOWN):
					inputWidget->kernelKeyDown(ea->getKey(),ea->getModKeyMask());
					inputWidget->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
					//if (ea->getKey() == crGUIEventAdapter::KEY_Escape)
					//{
					//	m_bindViewer->showCursor("Default");
					//	crFilterRenderManager::getInstance()->setInputCapturer(NULL);
					//}
					break;
				case(crGUIEventAdapter::KEYUP):
					inputWidget->kernelKeyUp(ea->getKey(),ea->getModKeyMask());
					inputWidget->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
					//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
					break;
				case(crGUIEventAdapter::INPUTCHAR):
					inputWidget->kernelInputChar(ea->getChar());
					inputWidget->doEvent(MAKEINT64(WCH_UI_InputChar,NULL),MAKEINT64(ea->getChar(),NULL));
					//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
					break;
				case(crGUIEventAdapter::SCROLLUP):
					//inputWidget->kernelKeyEvent(WCH_UI_SCROLLUP);
					inputWidget->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
					break;
				case(crGUIEventAdapter::SCROLLDOWN):
					//inputWidget->kernelKeyEvent(WCH_UI_SCROLLDOWN);
					inputWidget->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
					break;
				}
				crCanvasNode *canvas = inputWidget->getParentCanvas();
				if(canvas)
				{
					switch(ea->getEventType())
					{
					case(crGUIEventAdapter::KEYDOWN):
						canvas->kernelKeyDown(ea->getKey(),ea->getModKeyMask());
						canvas->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
						break;
					case(crGUIEventAdapter::KEYUP):
						canvas->kernelKeyUp(ea->getKey(),ea->getModKeyMask());
						canvas->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
						//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
						break;
					case(crGUIEventAdapter::INPUTCHAR):
						canvas->kernelInputChar(ea->getChar());
						canvas->doEvent(MAKEINT64(WCH_UI_InputChar,NULL),MAKEINT64(ea->getChar(),NULL));
						//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
						break;
					case(crGUIEventAdapter::SCROLLUP):
						//inputWidget->kernelKeyEvent(WCH_UI_SCROLLUP);
						canvas->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
						break;
					case(crGUIEventAdapter::SCROLLDOWN):
						//inputWidget->kernelKeyEvent(WCH_UI_SCROLLDOWN);
						canvas->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
						break;
					}
				}
				unsigned short msgflg = inputWidget->getMsgTraverse();
				if(msgflg == KM_None)
				{
					if(inputWidget->getParentCanvas())
					{
						msgflg = inputWidget->getParentCanvas()->getMsgTraverse();
					}
				}
				if( !(msgflg & CRCore::KM_Keyboard)
					&&(ea->getEventType() == crGUIEventAdapter::KEYDOWN 
					|| ea->getEventType() == crGUIEventAdapter::KEYUP
					|| ea->getEventType() == crGUIEventAdapter::INPUTCHAR
					|| ea->getEventType() == crGUIEventAdapter::SCROLLUP
					|| ea->getEventType() == crGUIEventAdapter::SCROLLDOWN))
				{
					return;
				}
			}
		}
		//crBrain::getInstance()->doEvent
		{
			switch(ea->getEventType())
			{
			case(crGUIEventAdapter::KEYDOWN):
 				crBrain::getInstance()->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
				if (crDisplaySettings::instance()->getRunMode() == 0 && ea->getKey() == crGUIEventAdapter::KEY_Escape)
				{
					m_bindViewer->showCursor("Default");
				}
				break;
			case(crGUIEventAdapter::KEYUP):
				crBrain::getInstance()->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
				//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
				break;
			case(crGUIEventAdapter::MOVE):
				crBrain::getInstance()->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::SCROLLUP):
				crBrain::getInstance()->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::SCROLLDOWN):
				crBrain::getInstance()->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::PUSH):
				crBrain::getInstance()->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::RELEASE):
				crBrain::getInstance()->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::DOUBLECLICK):
				crBrain::getInstance()->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::DRAG):
				crBrain::getInstance()->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			//case(crGUIEventAdapter::RESIZE):
			//	node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
			//	break;
			//case (crGUIEventAdapter::WINFOCUS):
			//	node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
			//	break;
			}
		}

        CRPhysics::crCamera *camera = m_bindViewer->getViewCameraNode();
		crNode *node = camera->getAttachedNode();
        if(node && node->getDataClass()) 
		{
			//switch(ea->getEventType())
			//{
			//case(crGUIEventAdapter::RESIZE):
			//	node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
			//	break;
			//case (crGUIEventAdapter::WINFOCUS):
			//	node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
			//	break;
			//}
			unsigned short keyboardMouseMode = camera->getKeyboardMouseMode();
			if(keyboardMouseMode == CRCore::KM_None)
				return;

			switch(ea->getEventType())
			{
			case(crGUIEventAdapter::MOVE):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
				node->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::SCROLLUP):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
				node->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::SCROLLDOWN):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(0,ea->getEventType()),ea));
				node->doEvent(MAKEINT32(0,ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			}

			if(keyboardMouseMode & CRCore::KM_Keyboard)
			{
				switch(ea->getEventType())
				{
				case(crGUIEventAdapter::KEYDOWN):
					//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getKey(),ea->getEventType()),ea));
					node->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
					//CRCore::notify(CRCore::ALWAYS)<<"KEYDOWN: "<<ea->getKey()<<" EventType:"<<ea->getEventType()<<std::endl;
					break;
				case(crGUIEventAdapter::KEYUP):
					//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getKey(),ea->getEventType()),ea));
					node->doEvent(MAKEINT32(ea->getKey(),ea->getEventType()),MAKEINT64(ea,NULL));
					//CRCore::notify(CRCore::ALWAYS)<<"KEYUP: "<<ea->getKey()<<std::endl;
					break;
				}
			}
			////////Mouse Butten Event
			if(!(keyboardMouseMode & CRCore::KM_LeftMouse) && ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if(!((keyboardMouseMode & CRCore::KM_LBtnUp) && ea->getEventType() == crGUIEventAdapter::RELEASE))
					return;
			}
			if(!(keyboardMouseMode & CRCore::KM_MiddleMouse) && ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			{
				return;
			}
			if(!(keyboardMouseMode & CRCore::KM_RightMouse) && ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				if(!((keyboardMouseMode & CRCore::KM_RBtnUp) && ea->getEventType() == crGUIEventAdapter::RELEASE))
					return;
			}

			switch(ea->getEventType())
			{
			case(crGUIEventAdapter::PUSH):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getButton(),ea->getEventType()),ea));
				node->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::RELEASE):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getButton(),ea->getEventType()),ea));
				node->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::DOUBLECLICK):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getButton(),ea->getEventType()),ea));
				node->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			case(crGUIEventAdapter::DRAG):
				//node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKEINT64(MAKEINT32(ea->getButton(),ea->getEventType()),ea));
				node->doEvent(MAKEINT32(ea->getButton(),ea->getEventType()),MAKEINT64(ea,NULL));
				break;
			}
		}
	}
}
_crInt32 crKeyboardMouseHandle::getMouseMsg(CRProducer::crGUIEventAdapter* ea)
{
	switch (ea->getEventType())
	{
	case crGUIEventAdapter::PUSH:
		if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			return WCH_UI_LBtnDown;
		else if(ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
			return WCH_UI_RBtnDown;
		else if(ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			return WCH_UI_MBtnDown;
		break;
	case crGUIEventAdapter::RELEASE:
		if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			return WCH_UI_LBtnUp;
		else if(ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
			return WCH_UI_RBtnUp;
		else if(ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			return WCH_UI_MBtnUp;
		break;
	case crGUIEventAdapter::DOUBLECLICK:
		if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			return WCH_UI_LBtnDblClk;
		else if(ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
			return WCH_UI_RBtnDblClk;
		else if(ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			return WCH_UI_MBtnUp;
		break;
	case crGUIEventAdapter::DRAG:
		if(ea->getButton() == crGUIEventAdapter::LEFT_MOUSE_BUTTON)
			return WCH_UI_LBtnDrag;
		else if(ea->getButton() == crGUIEventAdapter::RIGHT_MOUSE_BUTTON)
			return WCH_UI_RBtnDrag;
		else if(ea->getButton() == crGUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			return WCH_UI_MBtnDrag;
		break;
	case crGUIEventAdapter::MOVE:
		return WCH_UI_MouseMove;
	}
	return NULL;
}
