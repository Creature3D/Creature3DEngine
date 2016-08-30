/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRGUI_CR2DSTAGEMANAGER_H
#define CRGUI_CR2DSTAGEMANAGER_H 1

#include <CRGUI\crTypes.h>
#include <CRGUI/crWin32.h>
#include <CRGUI/crExport.h>
#include <CRGUI/crImageStage.h>
#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRProducer/crViewer.h>
#include <map>
#include <string>
namespace CRGUI{

class CRGUI_EXPORT cr2DStageManager : public CRCore::Referenced
{
public:
	cr2DStageManager();
	
    static cr2DStageManager *getInstance();
	typedef std::map<std::string, CRCore::ref_ptr<crImageStage> > StageMap;
	void addStage(crImageStage *stage);
	bool removeStage(const std::string& id);
	bool removeStage(crImageStage *stage);
	crImageStage *findStage(const std::string& id);
	//can not be used to show Draw_Mask Dialogs
	void showStage(const std::string& id, bool show/*, int cursor = 0*/);
	void showStage(crImageStage *stage, bool show/*, int cursor = 0*/);

	//can be used to show Draw_Mask Dialogs
	std::string doModal(const std::string& id);
    std::string doModal(crImageStage *stage);

	void close(const std::string& id);
	void close(crImageStage *stage);
	void closeAll(crImageStage *stage = NULL);
	void closeAllModelessStage(crImageStage *stage = NULL);

	void refreshScreen();
	Display getScreenMemorryDC(){ return m_screenMemoryDC; }
	virtual void clear();

	bool hasCanDragStageOnShow();
	void setParentWnd(Producer::Window parentWnd);
	Producer::Window getParentWnd(); 
protected:
	virtual ~cr2DStageManager();
	static CRCore::ref_ptr<cr2DStageManager> m_instance;
	void snapScreen();

	StageMap m_stageMap;
    Display  m_screenMemoryDC;
	HBITMAP m_screenBmp;
	Producer::Window m_parentWnd;
};

}
#endif