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
#include <CRGUI/cr2DStageManager.h>
#include <CRGUI/crWindowContainer.h>
#include <CRGUI/crElementNodeManager.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRCore/crBrain.h>
#include <CRCore/crNotify.h>
using namespace CRGUI;
using namespace CRCore;
ref_ptr<cr2DStageManager> cr2DStageManager::m_instance = NULL;

cr2DStageManager::cr2DStageManager():
m_parentWnd(NULL)
{
    m_screenMemoryDC = NULL;
	m_screenBmp = NULL;
}

cr2DStageManager::~cr2DStageManager()
{
	closeAll();
	if(m_screenMemoryDC)
		DeleteDC(m_screenMemoryDC);
	if(m_screenBmp)
		DeleteObject(m_screenBmp);
}

cr2DStageManager* cr2DStageManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new cr2DStageManager;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void cr2DStageManager::clear()
{//cr2DStageManager 不需要重新载入
	m_instance = NULL;
	crWindowContainer::shutdown();
	crImageStage::shutdown();
	crElementNodeManager::getInstance()->clear();
}

void cr2DStageManager::addStage(crImageStage *stage)
{
	m_stageMap[stage->getName()] = stage;
}

bool cr2DStageManager::removeStage(const std::string& id)
{
	StageMap::iterator itr = m_stageMap.find(id);
	if (itr == m_stageMap.end())
		return false;

	m_stageMap.erase(itr);
	return true;
}

bool cr2DStageManager::removeStage(crImageStage *stage)
{
	bool found = false;
	StageMap::iterator it = m_stageMap.begin();
	for(; it != m_stageMap.end(); it++) 
	{
		if (it->second == stage) 
		{
			m_stageMap.erase(it);
			found = true;
			break;
		}
	}

	return found;
}

crImageStage *cr2DStageManager::findStage(const std::string& id)
{
	StageMap::iterator itr = m_stageMap.find(id);
	if (itr == m_stageMap.end())
		return NULL;

	return (itr->second).get();
}

void cr2DStageManager::showStage(const std::string& id, bool show/*, int cursor*/)
{
    crImageStage *stage = findStage(id);
	if(stage)
        showStage(stage,show/*,cursor*/);
}

void cr2DStageManager::showStage(crImageStage *stage, bool show/*, int cursor*/)
{
    if(!stage) return;
	if(show)
	{
		crWindowContainer *windowContainer;
		windowContainer = crWindowContainer::getWindowThread(stage);
		if(windowContainer)
		{
			//stage->showWindow(show);
			windowContainer->showWindow(show);
		}
		else
		{
			windowContainer = crWindowContainer::getOrCreateAnIdleWindowThread(stage);
			if(windowContainer->attach(stage))
			{
				stage->backup();
				windowContainer->showWindow(show);
			}
		}
	}
	else
	{
        //stage->showWindow(false);
		close(stage);
	}
}

void cr2DStageManager::close(const std::string& id)
{
	crImageStage *stage = findStage(id);
	if(stage)
		close(stage);
}

void cr2DStageManager::close(crImageStage *stage)
{
	crWindowContainer *container = crWindowContainer::getWindowThread(stage);
	if(container) 
	{
		container->close();
	}
}

void cr2DStageManager::closeAllModelessStage(crImageStage *stage)
{
	crWindowContainer *container;
	for( StageMap::iterator it = m_stageMap.begin(); 
		it != m_stageMap.end(); 
		it++ ) 
	{
		if(it->second != stage)
		{
			container = crWindowContainer::getWindowThread(it->second.get());
			if(container && container->isRunning()) 
			{
				container->close();
			}
		}
	}
}

void cr2DStageManager::closeAll(crImageStage *stage)
{
	crWindowContainer *container;
	for( StageMap::iterator it = m_stageMap.begin(); 
		 it != m_stageMap.end(); 
		 it++ ) 
	{
		if(it->second != stage)
		{
			container = crWindowContainer::getWindowThread(it->second.get());
			if(container)
			{
				container->close();
			}
		}
	}
}
bool cr2DStageManager::hasCanDragStageOnShow()
{
	crImageStage *stage;
	for( StageMap::iterator itr = m_stageMap.begin(); 
		itr != m_stageMap.end(); 
		itr++ ) 
	{
		stage = itr->second.get();
		if(stage && !stage->isHide() && stage->getCanDrag())
		{
			return true;
		}
	}
	return false;
}
void cr2DStageManager::snapScreen()
{
	if(m_screenMemoryDC)
		DeleteDC(m_screenMemoryDC);
	if(m_screenBmp)
		DeleteObject(m_screenBmp);
	CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(bindview && bindview->isInited())
	{
		Window parent = bindview->getCameraWindow();
		Display dc = GetDC(parent);
		int width = GetDeviceCaps(dc, HORZRES);
		int height = GetDeviceCaps(dc, VERTRES);
		m_screenMemoryDC = CreateCompatibleDC(dc);
		m_screenBmp = CreateCompatibleBitmap(dc,width,height);
		SelectObject(m_screenMemoryDC, m_screenBmp);
		BitBlt(m_screenMemoryDC,0,0,width,height,dc,0,0,SRCCOPY);
		ReleaseDC(parent,dc);
	}
}

void cr2DStageManager::refreshScreen()
{
	if(m_screenMemoryDC)
	{
		CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			Window parent =  bindview->getCameraWindow();
			Display dc = GetDC(parent);
			BitBlt(dc,0,0,GetDeviceCaps(dc, HORZRES),GetDeviceCaps(dc, VERTRES),m_screenMemoryDC,0,0,SRCCOPY);
			ReleaseDC(parent,dc);
		}
	}
}

std::string cr2DStageManager::doModal(const std::string& id)
{
	crImageStage *stage = findStage(id);
	if(stage)
		return doModal(stage);
	return " ";
}

std::string cr2DStageManager::doModal(crImageStage *stage)
{
	std::string id = " ";

	crWindowContainer *windowContainer = crWindowContainer::getOrCreateAnIdleWindowThread(stage);
	if(windowContainer->attach(stage))
	{
		CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			//bindview->showCursor("Default");
			if(stage->needScreenImage()) snapScreen();
			id = windowContainer->doModal();
			//bindview->pause();
			//CRAI::crKeyboardMouseEventGroup::getInstance()->getBindViewer()->getKeyboardMouseCallback()->clear();
		}
		else id = windowContainer->doModal();
	}

	return id;
}
void cr2DStageManager::setParentWnd(Producer::Window parentWnd)
{
	m_parentWnd = parentWnd;
}
Producer::Window cr2DStageManager::getParentWnd()
{
	return m_parentWnd;
}