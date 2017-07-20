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
#ifndef CRENCAPSULATION_CRMETHODCLASS_H
#define CRENCAPSULATION_CRMETHODCLASS_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crMultiSwitch.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRGUI/crElement.h>
//#include <CRAL/crSoundManager.h>
#include <CRCore/crAITaskManager.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <vector>
#include <list>

namespace CREncapsulation{
class crExcEventContainerMethod : public CRCore::crMethod
{//
public:
	crExcEventContainerMethod();
	crExcEventContainerMethod(const crExcEventContainerMethod& handle);
	MethodClass(CREncapsulation, ExcEventContainer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crBase* m_this;
	_crInt64 m_param;
	_crInt32 m_lmsg;
	_crInt32 m_hmsg;
};

class crAITaskMethod : public CRCore::crMethod
{//
public:
	crAITaskMethod();
	crAITaskMethod(const crAITaskMethod& handle);
	MethodClass(CREncapsulation, AITask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	_crInt32 m_param;
	std::string m_taskName;
	CRCore::ref_ptr<CRCore::crAITask> m_task;
};

class crInputAITaskMethod : public CRCore::crMethod
{//
public:
	crInputAITaskMethod();
	crInputAITaskMethod(const crInputAITaskMethod& handle);
	MethodClass(CREncapsulation, InputAITask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	_crInt32 m_param;
	CRCore::ref_ptr<CRCore::crAITask> m_task;
	bool m_rec;
	CRCore::crMatrix m_recMatrix;
};

class crDoAITaskMethod : public CRCore::crMethod
{//
public:
	crDoAITaskMethod();
	crDoAITaskMethod(const crDoAITaskMethod& handle);
	MethodClass(CREncapsulation, DoAITask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	CRCore::ref_ptr<CRCore::crAITask> m_task;
	/////////////
	float computeAngle(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir);
	void turnToDir(const CRCore::crVector3f &targetDir,const CRCore::crVector3f &myDir);
    //void reset();

	//bool m_immediatelyTurn;
	//bool m_isAircraft;

	//float m_time_aimsg;
	//int m_currentIndex;
	//unsigned int m_currentMsgIndex;
	//float m_interval_aimsg;
	//CRCore::crControlPoint *m_preControlPoint;
};

class crGetRadarInfoMethod : public CRCore::crMethod
{// 
public:
	crGetRadarInfoMethod();
	crGetRadarInfoMethod(const crGetRadarInfoMethod& handle);
	MethodClass(CREncapsulation, GetRadarInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRPhysics::crViewMatterObject* m_this;
	float m_maskDistance;
};

class crAttachNodeUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crAttachNodeUpdateMethod();
	crAttachNodeUpdateMethod(const crAttachNodeUpdateMethod& handle);
	MethodClass(CREncapsulation, AttachNodeUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	//CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
	CRCore::ref_ptr<CRCore::crNode> m_target;
	CRCore::crVector3 m_offset;
	CRCore::crVector3 m_scale;
	float m_rotAngle;
	std::string m_boneName;
	CRCore::crVector3 m_dir;
};
////////////////////////////////////////////////////////////////////////////////////////
////PlayAmbSound
//class crPlayAmbSoundMethod : public CRCore::crMethod
//{
//public:
//	crPlayAmbSoundMethod();
//	crPlayAmbSoundMethod(const crPlayAmbSoundMethod& handle);
//    MethodClass(CREncapsulation, PlayAmbSound)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_soundName;
//private:
//	CRAL::crSoundState* m_soundState;
//};
/////////////CRGUI Method
//CloseDlg
class crCloseDlgMethod : public CRCore::crMethod
{//关闭对话框
public:
	crCloseDlgMethod();
	crCloseDlgMethod(const crCloseDlgMethod& handle);
	MethodClass(CREncapsulation, CloseDlg)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
////HideDlg
class crHideDlgMethod : public CRCore::crMethod
{//隐藏对话框
public:
	crHideDlgMethod();
	crHideDlgMethod(const crHideDlgMethod& handle);
	MethodClass(CREncapsulation, HideDlg)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crCloseStageMethod : public CRCore::crMethod
{//关闭对话框
public:
	crCloseStageMethod();
	crCloseStageMethod(const crCloseStageMethod& handle);
	MethodClass(CREncapsulation, CloseStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stage;
};
//Pause
class crPauseMethod : public CRCore::crMethod
{//重新开始运行
public:
	crPauseMethod();
	crPauseMethod(const crPauseMethod& handle);
	MethodClass(CREncapsulation, Pause)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_threadType;
};
//Resume
class crResumeMethod : public CRCore::crMethod
{//重新开始运行
public:
	crResumeMethod();
	crResumeMethod(const crResumeMethod& handle);
	MethodClass(CREncapsulation, Resume)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//Exit
class crExitMethod : public CRCore::crMethod
{//退出系统
public:
	crExitMethod();
	crExitMethod(const crExitMethod& handle);
	MethodClass(CREncapsulation, Exit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//HideCursor
class crHideCursorMethod : public CRCore::crMethod
{//HideCursor
public:
	crHideCursorMethod();
	crHideCursorMethod(const crHideCursorMethod& handle);
	MethodClass(CREncapsulation, HideCursor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_capture;
};
//ShowStage
class crShowStageMethod : public CRCore::crMethod
{//
public:
	crShowStageMethod();
	crShowStageMethod(const crShowStageMethod& handle);
	MethodClass(CREncapsulation, ShowStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageDlgString;
	int m_show;//0 = close, 1= showstage, 2 = doModal
	//int m_cursor;
};
class crCloseAllModelessStageMethod : public CRCore::crMethod
{//
public:
	crCloseAllModelessStageMethod();
	crCloseAllModelessStageMethod(const crCloseAllModelessStageMethod& handle);
	MethodClass(CREncapsulation, CloseAllModelessStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crRedrawStageMethod : public CRCore::crMethod
{//
public:
	crRedrawStageMethod();
	crRedrawStageMethod(const crRedrawStageMethod& handle);
	MethodClass(CREncapsulation, RedrawStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageDlgString;
};
class crUpdateAndRedrawStageMethod : public CRCore::crMethod
{//
public:
	crUpdateAndRedrawStageMethod();
	crUpdateAndRedrawStageMethod(const crUpdateAndRedrawStageMethod& handle);
	MethodClass(CREncapsulation, UpdateAndRedrawStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageDlgString;
};
class crInitStageMethod : public CRCore::crMethod
{//
public:
	crInitStageMethod();
	crInitStageMethod(const crInitStageMethod& handle);
	MethodClass(CREncapsulation, InitStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageDlgString;
};
//ScrollBarInit
class crScrollBarInitMethod : public CRCore::crMethod
{//
public:
	crScrollBarInitMethod();
	crScrollBarInitMethod(const crScrollBarInitMethod& handle);
	MethodClass(CREncapsulation, ScrollBarInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	CRCore::ref_ptr<CRGUI::crScrollBarElement> m_scrollbar;
};
//MultiSwitchListBoxUpdate
class crMultiSwitchListBoxUpdateMethod : public CRCore::crMethod
{//
public:
	crMultiSwitchListBoxUpdateMethod();
	crMultiSwitchListBoxUpdateMethod(const crMultiSwitchListBoxUpdateMethod& handle);
	MethodClass(CREncapsulation, MultiSwitchListBoxUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_multiSwitch;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};

//ListBoxFileInit
class crListBoxFileInitMethod : public CRCore::crMethod
{//ListBox InitWindow Callback
public:
	crListBoxFileInitMethod();
	crListBoxFileInitMethod(const crListBoxFileInitMethod& handle);
	MethodClass(CREncapsulation, ListBoxFileInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool checkExt(const std::string &filename, const std::string &ext);
	void findFileInDir(const char* rootDir, const std::string &ext, std::vector<std::string>& fileNameVec);
	std::string m_filePath;
	std::string m_fileExt;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
//ListBoxTextUpdateEx
class crListBoxTextUpdateExMethod : public CRCore::crMethod
{//ListBox UpdateData Callback
public:
	crListBoxTextUpdateExMethod();
	crListBoxTextUpdateExMethod(const crListBoxTextUpdateExMethod& handle);
	MethodClass(CREncapsulation, ListBoxTextUpdateEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_staticTextBoxName;
	typedef std::map<std::string,std::string> IDTextMap;//id,text
	IDTextMap m_idTextMap;
	std::string m_lastid;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
//ListBoxImageInit
class crListBoxImageInitMethod : public CRCore::crMethod
{//
public:
	crListBoxImageInitMethod();
	crListBoxImageInitMethod(const crListBoxImageInitMethod& callback);
	MethodClass(CREncapsulation, ListBoxImageInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
//ListBoxImageUpdate
class crListBoxImageUpdateMethod : public CRCore::crMethod
{//
public:
	crListBoxImageUpdateMethod();
	crListBoxImageUpdateMethod(const crListBoxImageUpdateMethod& handle);
	MethodClass(CREncapsulation, ListBoxImageUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	typedef std::vector<std::string> ImageNameList;
	ImageNameList m_imageNameList;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
//ListBoxImageUpdateEx
class crListBoxImageUpdateExMethod : public CRCore::crMethod
{//ListBox UpdateData Callback
public:
	crListBoxImageUpdateExMethod();
	crListBoxImageUpdateExMethod(const crListBoxImageUpdateExMethod& handle);
	MethodClass(CREncapsulation, ListBoxImageUpdateEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_imageBoxName;
	typedef std::map<std::string,std::string> IDImageMap;//id,imagefilename
	IDImageMap m_idImageMap;
	std::string m_lastid;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
//CheckBoxPlayPreDraw
class crCheckBoxPlayPreDrawMethod : public CRCore::crMethod
{//
public:
	crCheckBoxPlayPreDrawMethod();
	crCheckBoxPlayPreDrawMethod(const crCheckBoxPlayPreDrawMethod& handle);
	MethodClass(CREncapsulation, CheckBoxPlayPreDraw)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	float m_timeInterval;
	CRCore::Timer_t m_lastUpdateTime;
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkbox;
};

class crButtonBoxNextPageMethod : public CRCore::crMethod
{//
public:
	crButtonBoxNextPageMethod();
	crButtonBoxNextPageMethod(const crButtonBoxNextPageMethod& handle);
	MethodClass(CREncapsulation, ButtonBoxNextPage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	CRCore::ref_ptr<CRGUI::crButtonElement> m_buttonbox;
};

class crButtonBoxPrePageMethod : public CRCore::crMethod
{//
public:
	crButtonBoxPrePageMethod();
	crButtonBoxPrePageMethod(const crButtonBoxPrePageMethod& handle);
	MethodClass(CREncapsulation, ButtonBoxPrePage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	CRCore::ref_ptr<CRGUI::crButtonElement> m_buttonbox;
};

class crLoadingProgressInitMethod : public CRCore::crMethod
{//crProgressElement InitWindow Callback
public:
	crLoadingProgressInitMethod();
	crLoadingProgressInitMethod(const crLoadingProgressInitMethod& handle);
	MethodClass(CREncapsulation, LoadingProgressInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crProgressElement> m_progress;
};

class crClearStaticTextMethod : public CRCore::crMethod
{//
public:
	crClearStaticTextMethod();
	crClearStaticTextMethod(const crClearStaticTextMethod& handle);
	MethodClass(CREncapsulation, ClearStaticText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crStaticTextBoxElement> m_element;
};

class crClearEditTextMethod : public CRCore::crMethod
{//
public:
	crClearEditTextMethod();
	crClearEditTextMethod(const crClearEditTextMethod& handle);
	MethodClass(CREncapsulation, ClearEditText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_element;
};

////////////////////////init.cfg
class crDSFullScreenInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSFullScreenInitMethod();
	crDSFullScreenInitMethod(const crDSFullScreenInitMethod& handle);
	MethodClass(CREncapsulation, DSFullScreenInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};
class crDSVsyncInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSVsyncInitMethod();
	crDSVsyncInitMethod(const crDSVsyncInitMethod& handle);
	MethodClass(CREncapsulation, DSVsyncInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};
class crDSShaderInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSShaderInitMethod();
	crDSShaderInitMethod(const crDSShaderInitMethod& handle);
	MethodClass(CREncapsulation, DSShaderInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};
class crDSResolutionComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSResolutionComboInitMethod();
	crDSResolutionComboInitMethod(const crDSResolutionComboInitMethod& handle);
	MethodClass(CREncapsulation, DSResolutionComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSBrightnessSliderInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSBrightnessSliderInitMethod();
	crDSBrightnessSliderInitMethod(const crDSBrightnessSliderInitMethod& handle);
	MethodClass(CREncapsulation, DSBrightnessSliderInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crSliderElement> m_slider;
};
class crDSMouseSpeedSliderInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSMouseSpeedSliderInitMethod();
	crDSMouseSpeedSliderInitMethod(const crDSMouseSpeedSliderInitMethod& handle);
	MethodClass(CREncapsulation, DSMouseSpeedSliderInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crSliderElement> m_slider;
};
class crDSTextureDetailComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSTextureDetailComboInitMethod();
	crDSTextureDetailComboInitMethod(const crDSTextureDetailComboInitMethod& handle);
	MethodClass(CREncapsulation, DSTextureDetailComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSTextureAnisotropyComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSTextureAnisotropyComboInitMethod();
	crDSTextureAnisotropyComboInitMethod(const crDSTextureAnisotropyComboInitMethod& handle);
	MethodClass(CREncapsulation, DSTextureAnisotropyComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSShadowDetailComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSShadowDetailComboInitMethod();
	crDSShadowDetailComboInitMethod(const crDSShadowDetailComboInitMethod& handle);
	MethodClass(CREncapsulation, DSShadowDetailComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSAlphaShadowComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSAlphaShadowComboInitMethod();
	crDSAlphaShadowComboInitMethod(const crDSAlphaShadowComboInitMethod& handle);
	MethodClass(CREncapsulation, DSAlphaShadowComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSMotionBlurComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSMotionBlurComboInitMethod();
	crDSMotionBlurComboInitMethod(const crDSMotionBlurComboInitMethod& handle);
	MethodClass(CREncapsulation, DSMotionBlurComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSHDRComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSHDRComboInitMethod();
	crDSHDRComboInitMethod(const crDSHDRComboInitMethod& handle);
	MethodClass(CREncapsulation, DSHDRComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSDepthOfFieldComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSDepthOfFieldComboInitMethod();
	crDSDepthOfFieldComboInitMethod(const crDSDepthOfFieldComboInitMethod& handle);
	MethodClass(CREncapsulation, DSDepthOfFieldComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSAntialiasingComboInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crDSAntialiasingComboInitMethod();
	crDSAntialiasingComboInitMethod(const crDSAntialiasingComboInitMethod& handle);
	MethodClass(CREncapsulation, DSAntialiasingComboInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crComboBoxElement> m_combo;
};
class crDSResetDisplaySettingMethod : public CRCore::crMethod
{//ButtonClick Callback
public:
	crDSResetDisplaySettingMethod();
	crDSResetDisplaySettingMethod(const crDSResetDisplaySettingMethod& handle);
	MethodClass(CREncapsulation, DSResetDisplaySetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_fullScreenElement;
	std::string m_vsyncElement;
	std::string m_resolutionComboElement;
	std::string m_brightnessSliderElement;
	std::string m_textureDetailComboElement;
	std::string m_textureAnisotropyComboElement;
	std::string m_shadowDetailComboElement;
	std::string m_alphaShadowComboElement;
	std::string m_motionBlurComboElement;
	std::string m_hdrComboElement;
	std::string m_depthOfFieldComboElement;
	std::string m_antialiasingComboElement;
	std::string m_mouseSpeedSliderElement;
	std::string m_disableShaderElement;
	bool m_fullScreen;
	bool m_vsync;
	int m_resolution;
	float m_brightness;
	int m_textureDetail;
	int m_textureAnisotropy;
	int m_shadowDetail;
	int m_alphaShadow;
	int m_motionBlur;
	int m_hdr;
	int m_depthOfField;
	int m_antialiasing;
	float m_mouseSpeed;
	bool m_disableShader;
};

class crDSUpdateInitCfgMethod : public CRCore::crMethod
{//ButtonClick Callback
public:
	crDSUpdateInitCfgMethod();
	crDSUpdateInitCfgMethod(const crDSUpdateInitCfgMethod& handle);
	MethodClass(CREncapsulation, DSUpdateInitCfg)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_fullScreenElement;
	std::string m_vsyncElement;
	std::string m_resolutionComboElement;
	std::string m_brightnessSliderElement;
	std::string m_textureDetailComboElement;
	std::string m_textureAnisotropyComboElement;
	std::string m_shadowDetailComboElement;
	std::string m_alphaShadowComboElement;
	std::string m_motionBlurComboElement;
	std::string m_hdrComboElement;
	std::string m_depthOfFieldComboElement;
	std::string m_antialiasingComboElement;
	std::string m_mouseSpeedSliderElement;
	std::string m_disableShaderElement;
	//typedef std::vector<std::string> ExitString;
	//ExitString m_exitStrings;
};
////////////////////////save.cfg
class crSaveListInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crSaveListInitMethod();
	crSaveListInitMethod(const crSaveListInitMethod& handle);
	MethodClass(CREncapsulation, SaveListInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crSaveListUpdateMethod : public CRCore::crMethod
{//UpdateData Callback
public:
	crSaveListUpdateMethod();
	crSaveListUpdateMethod(const crSaveListUpdateMethod& handle);
	MethodClass(CREncapsulation, SaveListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_imageBoxName;
	std::string m_staticTextBoxName;
	typedef std::vector<std::string> ImageNameList;
	ImageNameList m_imageNameList;
};
//////////////////
class crValidEditStrMethod : public CRCore::crMethod
{//UpdateData Callback
public:
	crValidEditStrMethod();
	crValidEditStrMethod(const crValidEditStrMethod& handle);
	MethodClass(CREncapsulation, ValidEditStr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editbox;
	std::string m_elementName;
};

class crCreateSaveMethod : public CRCore::crMethod
{//LBtnUp Callback
public:
	crCreateSaveMethod();
	crCreateSaveMethod(const crCreateSaveMethod& handle);
	MethodClass(CREncapsulation, CreateSave)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_name;
	std::string m_botName;
	crSave::GameState m_gameState;
	int m_sceneid;
	std::string m_editBoxName;
};
//delete current save
class crDeleteSaveMethod : public CRCore::crMethod
{//
public:
	crDeleteSaveMethod();
	crDeleteSaveMethod(const crDeleteSaveMethod& handle);
	MethodClass(CREncapsulation, DeleteSave)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
//class crSaveMethod : public CRCore::crMethod
//{//
//public:
//	crSaveMethod();
//	crSaveMethod(const crSaveMethod& handle);
//	MethodClass(CREncapsulation, Save)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};
////////////////////
class crSceneMapInitMethod : public CRCore::crMethod
{//ImageBox InitWindow Callback
public:
	crSceneMapInitMethod();
	crSceneMapInitMethod(const crSceneMapInitMethod& handle);
	MethodClass(CREncapsulation, SceneMapInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageBoxElement> m_imagebox;
	typedef std::map<std::string,std::string> SceneIDMapFileMap;//sceneid,imagefilename
	SceneIDMapFileMap m_sceneidMapFileMap;
	std::string m_lastsceneid;
};
class crSceneInfoInitMethod : public CRCore::crMethod
{//StaticTextBox InitWindow Callback
public:
	crSceneInfoInitMethod();
	crSceneInfoInitMethod(const crSceneInfoInitMethod& handle);
	MethodClass(CREncapsulation, SceneInfoInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crStaticTextBoxElement> m_staticTextBox;
	typedef std::map<std::string,std::string> SceneIDInfoMap;//sceneid,infotext
	SceneIDInfoMap m_sceneidInfoMap;
	std::string m_lastsceneid;
};
/////////////////场景选择
class crSceneListUpdateMethod : public CRCore::crMethod
{//UpdateData Callback
public:
	crSceneListUpdateMethod();
	crSceneListUpdateMethod(const crSceneListUpdateMethod& handle);
	MethodClass(CREncapsulation, SceneListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listBox;
};
/////////////////飞机选择
class crAircraftListInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crAircraftListInitMethod();
	crAircraftListInitMethod(const crAircraftListInitMethod& handle);
	MethodClass(CREncapsulation, AircraftListInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listBox;
	typedef std::multimap<std::string,std::string> SceneIDAircraftMap;//sceneid,aircraftname
	SceneIDAircraftMap m_sceneidAircraftMap;
};

class crAircraftListUpdateMethod : public CRCore::crMethod
{//UpdateData Callback
public:
	crAircraftListUpdateMethod();
	crAircraftListUpdateMethod(const crAircraftListUpdateMethod& handle);
	MethodClass(CREncapsulation, AircraftListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listBox;
};
/////////////////武器选择
class crWeaponListInitMethod : public CRCore::crMethod
{//InitWindow Callback
public:
	crWeaponListInitMethod();
	crWeaponListInitMethod(const crWeaponListInitMethod& handle);
	MethodClass(CREncapsulation, WeaponListInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listBox;
	typedef std::multimap<std::string,std::string> AircraftWeaponMap;//aircraftname,weaponname
	AircraftWeaponMap m_aircraftWeaponMap;
};
class crInitGameMethod : public CRCore::crMethod
{//LBtnUp Callback
public:
	crInitGameMethod();
	crInitGameMethod(const crInitGameMethod& handle);
	MethodClass(CREncapsulation, InitGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_weaponComboName[3];
};
//class crInitModelViewMethod : public CRCore::crMethod
//{//LBtnUp Callback
//public:
//	crInitModelViewMethod();
//	crInitModelViewMethod(const crInitModelViewMethod& callback);
//	MethodClass(CREncapsulation, InitModelView)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRGUI::crElement> m_element;
//	std::string m_modelListName;
//	std::string m_typeListName;
//};

class crPreviewImageMethod : public CRCore::crMethod
{//
public:
	crPreviewImageMethod();
	crPreviewImageMethod(const crPreviewImageMethod& handle);
	MethodClass(CREncapsulation, PreviewImage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_elementNameID;
	std::string m_filePath;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};

class crAddListDataMethod : public CRCore::crMethod
{//
public:
	crAddListDataMethod();
	crAddListDataMethod(const crAddListDataMethod& handle);
	MethodClass(CREncapsulation, AddListData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	typedef std::list<std::string> StrList;
	StrList m_strList;
};

class crInitEditorMethod : public CRCore::crMethod
{//
public:
	crInitEditorMethod();
	crInitEditorMethod(const crInitEditorMethod& handle);
	MethodClass(CREncapsulation, InitEditor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_sceneListName;
};

//class crSetRunModeMethod : public CRCore::crMethod
//{//
//public:
//	crSetRunModeMethod();
//	crSetRunModeMethod(const crSetRunModeMethod& handle);
//	MethodClass(CREncapsulation, SetRunMode)
//    virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_runMode;
//};

class crAddModelMethod : public CRCore::crMethod
{//
public:
	crAddModelMethod();
	crAddModelMethod(const crAddModelMethod& handle);
	MethodClass(CREncapsulation, AddModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_dataFile;
	std::string *m_addDescription;
};
class crAddUIModelMethod : public CRCore::crMethod
{//
public:
	crAddUIModelMethod();
	crAddUIModelMethod(const crAddUIModelMethod& handle);
	MethodClass(CREncapsulation, AddUIModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_dataFile;
	std::string *m_addDescription;
};
class crReloadUIModelMethod : public CRCore::crMethod
{//
public:
	crReloadUIModelMethod();
	crReloadUIModelMethod(const crReloadUIModelMethod& handle);
	MethodClass(CREncapsulation, ReloadUIModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crLoadUISceneMethod : public CRCore::crMethod
{//
public:
	crLoadUISceneMethod();
	crLoadUISceneMethod(const crLoadUISceneMethod& handle);
	MethodClass(CREncapsulation, LoadUIScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string *m_dataFile;
};
class crSaveUISceneMethod : public CRCore::crMethod
{//
public:
	crSaveUISceneMethod();
	crSaveUISceneMethod(const crSaveUISceneMethod& handle);
	MethodClass(CREncapsulation, SaveUIScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fileName;
};
class crCreateModelMethod : public CRCore::crMethod
{//
public:
	crCreateModelMethod();
	crCreateModelMethod(const crCreateModelMethod& handle);
	MethodClass(CREncapsulation, CreateModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crGroup> m_root;
	std::string m_createNodeType;

};
class crInitSceneNameMethod : public CRCore::crMethod
{//
public:
	crInitSceneNameMethod();
	crInitSceneNameMethod(const crInitSceneNameMethod& handle);
	MethodClass(CREncapsulation, InitSceneName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editbox;
};
class crSaveSceneMethod : public CRCore::crMethod
{//
public:
	crSaveSceneMethod();
	crSaveSceneMethod(const crSaveSceneMethod& handle);
	MethodClass(CREncapsulation, SaveScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_fileNameElement;
	std::string m_filePath;
};
class crSaveScene2Method : public CRCore::crMethod
{//
public:
	crSaveScene2Method();
	crSaveScene2Method(const crSaveScene2Method& handle);
	MethodClass(CREncapsulation, SaveScene2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_filePath;
};
class crUpdateSelectNodeListMethod : public CRCore::crMethod
{//
public:
	crUpdateSelectNodeListMethod();
	crUpdateSelectNodeListMethod(const crUpdateSelectNodeListMethod& handle);
	MethodClass(CREncapsulation, UpdateSelectNodeList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crDisplayNodeAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayNodeAttrMethod();
	crDisplayNodeAttrMethod(const crDisplayNodeAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayNodeAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_filePathElement;
	std::string m_nameElement;
	std::string m_descriptionElement;
	std::string m_objSelectedLightElement;
	std::string m_avoidLightElement;
	std::string m_needLightElement;
	std::string m_positionTextElement;
	std::string m_posxElement;
	std::string m_posyElement;
	std::string m_poszElement;
	std::string m_staticMeshElement;
	std::string m_freezeElement;
};
class crModifyNodeAttrMethod : public CRCore::crMethod
{//
public:
	crModifyNodeAttrMethod();
	crModifyNodeAttrMethod(const crModifyNodeAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyNodeAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listBoxElement;
	std::string m_filePathElement;
	std::string m_nameElement;
	std::string m_descriptionElement;
	std::string m_posxElement;
	std::string m_posyElement;
	std::string m_poszElement;
	std::string m_staticMeshElement;
};
class crShowScaleInfoMethod : public CRCore::crMethod
{//
public:
	crShowScaleInfoMethod();
	crShowScaleInfoMethod(const crShowScaleInfoMethod& handle);
	MethodClass(CREncapsulation, ShowScaleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crElement* m_this;
	std::string m_listBoxElement;
	std::string m_scaleAllElement;
	std::string m_scalexElement;
	std::string m_scaleyElement;
	std::string m_scalezElement;
};
class crShowBoundLengthMethod : public CRCore::crMethod
{//
public:
	crShowBoundLengthMethod();
	crShowBoundLengthMethod(const crShowBoundLengthMethod& handle);
	MethodClass(CREncapsulation, ShowBoundLength)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crElement* m_this;
	std::string m_listBoxElement;
	std::string m_scaleAllElement;
	std::string m_scalexElement;
	std::string m_scaleyElement;
	std::string m_scalezElement;
};
//class crSaveNodeAttrMethod : public CRCore::crMethod
//{//
//public:
//	crSaveNodeAttrMethod();
//	crSaveNodeAttrMethod(const crSaveNodeAttrMethod& handle);
//	MethodClass(CREncapsulation, SaveNodeAttr)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRGUI::crElement> m_element;
//	std::string m_listBoxElement;
//	std::string m_filePathElement;
//	std::string m_nameElement;
//	std::string m_descriptionElement;
//	std::string m_avoidLightElement;
//};
class crInitSelectTypeMethod : public CRCore::crMethod
{//
public:
	crInitSelectTypeMethod();
	crInitSelectTypeMethod(const crInitSelectTypeMethod& handle);
	MethodClass(CREncapsulation, InitSelectType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crUpdateSelectTypeMethod : public CRCore::crMethod
{//
public:
	crUpdateSelectTypeMethod();
	crUpdateSelectTypeMethod(const crUpdateSelectTypeMethod& handle);
	MethodClass(CREncapsulation, UpdateSelectType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crSetSelectTypeMethod : public CRCore::crMethod
{//
public:
	crSetSelectTypeMethod();
	crSetSelectTypeMethod(const crSetSelectTypeMethod& handle);
	MethodClass(CREncapsulation, SetSelectType)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_selectType;
};
class crLoadDesMethod : public CRCore::crMethod
{//
public:
	crLoadDesMethod();
	crLoadDesMethod(const crLoadDesMethod& handle);
	MethodClass(CREncapsulation, LoadDes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_desEditElement;
	std::string m_filePath;
	int m_flg;//0:Add, 1:Replace
};
class crInitTransformNodeModeMethod : public CRCore::crMethod
{//
public:
	crInitTransformNodeModeMethod();
	crInitTransformNodeModeMethod(const crInitTransformNodeModeMethod& handle);
	MethodClass(CREncapsulation, InitTransformNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crInitTransformNodeSpeedMethod : public CRCore::crMethod
{//
public:
	crInitTransformNodeSpeedMethod();
	crInitTransformNodeSpeedMethod(const crInitTransformNodeSpeedMethod& handle);
	MethodClass(CREncapsulation, InitTransformNodeSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editbox;
};
class crSetTransformNodeModeMethod : public CRCore::crMethod
{//
public:
	crSetTransformNodeModeMethod();
	crSetTransformNodeModeMethod(const crSetTransformNodeModeMethod& handle);
	MethodClass(CREncapsulation, SetTransformNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_transformNodeMode;
};
class crInitPanNodeModeMethod : public CRCore::crMethod
{//
public:
	crInitPanNodeModeMethod();
	crInitPanNodeModeMethod(const crInitPanNodeModeMethod& handle);
	MethodClass(CREncapsulation, InitPanNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crSetPanNodeModeMethod : public CRCore::crMethod
{//
public:
	crSetPanNodeModeMethod();
	crSetPanNodeModeMethod(const crSetPanNodeModeMethod& handle);
	MethodClass(CREncapsulation, SetPanNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_panNodeMode;
};
class crWalkPanNodeModeMethod : public CRCore::crMethod
{//
public:
	crWalkPanNodeModeMethod();
	crWalkPanNodeModeMethod(const crWalkPanNodeModeMethod& handle);
	MethodClass(CREncapsulation, WalkPanNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_panNodeModeCount;
};
class crInitRotNodeModeMethod : public CRCore::crMethod
{//
public:
	crInitRotNodeModeMethod();
	crInitRotNodeModeMethod(const crInitRotNodeModeMethod& handle);
	MethodClass(CREncapsulation, InitRotNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crSetRotNodeModeMethod : public CRCore::crMethod
{//
public:
	crSetRotNodeModeMethod();
	crSetRotNodeModeMethod(const crSetRotNodeModeMethod& handle);
	MethodClass(CREncapsulation, SetRotNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_rotNodeMode;
};
class crWalkRotNodeModeMethod : public CRCore::crMethod
{//
public:
	crWalkRotNodeModeMethod();
	crWalkRotNodeModeMethod(const crWalkRotNodeModeMethod& handle);
	MethodClass(CREncapsulation, WalkRotNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_rotNodeModeCount;
};
class crInitScaleNodeModeMethod : public CRCore::crMethod
{//
public:
	crInitScaleNodeModeMethod();
	crInitScaleNodeModeMethod(const crInitScaleNodeModeMethod& handle);
	MethodClass(CREncapsulation, InitScaleNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crSetScaleNodeModeMethod : public CRCore::crMethod
{//
public:
	crSetScaleNodeModeMethod();
	crSetScaleNodeModeMethod(const crSetScaleNodeModeMethod& handle);
	MethodClass(CREncapsulation, SetScaleNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_scaleNodeMode;
};
class crWalkScaleNodeModeMethod : public CRCore::crMethod
{//
public:
	crWalkScaleNodeModeMethod();
	crWalkScaleNodeModeMethod(const crWalkScaleNodeModeMethod& handle);
	MethodClass(CREncapsulation, WalkScaleNodeMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_scaleNodeModeCount;
};
class crSetTransformNodeSpeedMethod : public CRCore::crMethod
{//
public:
	crSetTransformNodeSpeedMethod();
	crSetTransformNodeSpeedMethod(const crSetTransformNodeSpeedMethod& handle);
	MethodClass(CREncapsulation, SetTransformNodeSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editbox;
};
class crResetMatrixLogic : public CRCore::crLogic
{//
public:
	crResetMatrixLogic();
	crResetMatrixLogic(const crResetMatrixLogic& handle);
	LogicClass(CREncapsulation, ResetMatrix)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_adjustEditorNodeMathod;
};
class crNewLightMethod : public CRCore::crMethod
{//
public:
	crNewLightMethod();
	crNewLightMethod(const crNewLightMethod& handle);
	MethodClass(CREncapsulation, NewLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crGroup *m_lsRoot;
	CRCore::ref_ptr<CRCore::crLightSource> m_copyData;
};
class crDeleteLightMethod : public CRCore::crMethod
{//
public:
	crDeleteLightMethod();
	crDeleteLightMethod(const crDeleteLightMethod& handle);
	MethodClass(CREncapsulation, DeleteLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crGroup *m_lsRoot;
	std::string *m_editorNodeName;
};
class crDeleteParticleMethod : public CRCore::crMethod
{//
public:
	crDeleteParticleMethod();
	crDeleteParticleMethod(const crDeleteParticleMethod& handle);
	MethodClass(CREncapsulation, DeleteParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string *m_editorNodeName;
};
class crUpdateLightListMethod : public CRCore::crMethod
{//
public:
	crUpdateLightListMethod();
	crUpdateLightListMethod(const crUpdateLightListMethod& handle);
	MethodClass(CREncapsulation, UpdateLightList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crUpdateParticleListMethod : public CRCore::crMethod
{//
public:
	crUpdateParticleListMethod();
	crUpdateParticleListMethod(const crUpdateParticleListMethod& handle);
	MethodClass(CREncapsulation, UpdateParticleList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crSelectEditorNodeMethod : public CRCore::crMethod
{//
public:
	crSelectEditorNodeMethod();
	crSelectEditorNodeMethod(const crSelectEditorNodeMethod& handle);
	MethodClass(CREncapsulation, SelectEditorNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crGroup *m_root;
	std::string m_rootName;
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crSelectEditorNodeExMethod : public CRCore::crMethod
{//
public:
	crSelectEditorNodeExMethod();
	crSelectEditorNodeExMethod(const crSelectEditorNodeExMethod& handle);
	MethodClass(CREncapsulation, SelectEditorNodeEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crGroup *m_root;
	std::string m_rootName;
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_lightListElement;
};

class crDisplayLightAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayLightAttrMethod();
	crDisplayLightAttrMethod(const crDisplayLightAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayLightAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_currentSelectLightElement;
	std::string m_nameElement;
	std::string m_descriptionElement;
	std::string m_positionXElement;
	std::string m_positionYElement;
	std::string m_positionZElement;
	//std::string m_azimuthElement;
	//std::string m_elevationElement;
	std::string m_lightTypeGroupElement;
	std::string m_ambientColorElement;
	std::string m_ambientColorRElement;
	std::string m_ambientColorGElement;
	std::string m_ambientColorBElement;
	std::string m_diffuseColorElement;
	std::string m_diffuseColorRElement;
	std::string m_diffuseColorGElement;
	std::string m_diffuseColorBElement;
	std::string m_specularColorElement;
	std::string m_specularColorRElement;
	std::string m_specularColorGElement;
	std::string m_specularColorBElement;
	std::string m_constantElement;
	std::string m_linearElement;
	std::string m_quadraticElement;
	std::string m_loweastLumElement;
	std::string m_distanceElement;
	std::string m_cutoffElement;
	std::string m_dropoffElement;
	std::string m_enableElement;
	std::string m_calcShadowElement;
	std::string m_calcShadow2Element;
};
class crModifyLightAttrMethod : public CRCore::crMethod
{//
public:
	crModifyLightAttrMethod();
	crModifyLightAttrMethod(const crModifyLightAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyLightAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	CRCore::crGroup *m_editorRoot;
	std::string m_editorRootName;//
	std::string m_lightListElement;
	std::string m_currentSelectLightElement;
	std::string m_nameElement;
	std::string m_descriptionElement;
	std::string m_positionXElement;
	std::string m_positionYElement;
	std::string m_positionZElement;
	//std::string m_azimuthElement;
	//std::string m_elevationElement;
	std::string m_lightTypeGroupElement;
	std::string m_ambientColorElement;
	std::string m_ambientColorRElement;
	std::string m_ambientColorGElement;
	std::string m_ambientColorBElement;
	std::string m_diffuseColorElement;
	std::string m_diffuseColorRElement;
	std::string m_diffuseColorGElement;
	std::string m_diffuseColorBElement;
	std::string m_specularColorElement;
	std::string m_specularColorRElement;
	std::string m_specularColorGElement;
	std::string m_specularColorBElement;
	std::string m_constantElement;
	std::string m_linearElement;
	std::string m_quadraticElement;
	std::string m_loweastLumElement;
	std::string m_distanceElement;
	std::string m_cutoffElement;
	std::string m_dropoffElement;
	std::string m_enableElement;
	std::string m_calcShadowElement;
	std::string m_calcShadow2Element;
};
class crChooseColorMethod : public CRCore::crMethod
{//
public:
	crChooseColorMethod();
	crChooseColorMethod(const crChooseColorMethod& handle);
	MethodClass(CREncapsulation, ChooseColor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crChooseColorElement> m_element;
};
class crUpdateElementMethod : public CRCore::crMethod
{//
public:
	crUpdateElementMethod();
	crUpdateElementMethod(const crUpdateElementMethod& handle);
	MethodClass(CREncapsulation, UpdateElement)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_elementName;
};
class crSetChooseColorMethod : public CRCore::crMethod
{//
public:
	crSetChooseColorMethod();
	crSetChooseColorMethod(const crSetChooseColorMethod& handle);
	MethodClass(CREncapsulation, SetChooseColor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crChooseColorElement> m_element;
	std::string m_elementR;
	std::string m_elementG;
	std::string m_elementB;
};
class crSetComponentColorMethod : public CRCore::crMethod
{//
public:
	crSetComponentColorMethod();
	crSetComponentColorMethod(const crSetComponentColorMethod& handle);
	MethodClass(CREncapsulation, SetComponentColor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crChooseColorElement> m_element;
	std::string m_elementR;
	std::string m_elementG;
	std::string m_elementB;
};
class crDisplayCharacterAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayCharacterAttrMethod();
	crDisplayCharacterAttrMethod(const crDisplayCharacterAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayCharacterAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_scaleElement;
	std::string m_offsetXElement;
	std::string m_offsetYElement;
	std::string m_offsetZElement;
	std::string m_animListElement;
};
class crDoAnimMethod : public CRCore::crMethod
{//
public:
	crDoAnimMethod();
	crDoAnimMethod(const crDoAnimMethod& handle);
	MethodClass(CREncapsulation, DoAnim)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crColumnListElement> m_columnList;
};
class crModifyCharacterAttrMethod : public CRCore::crMethod
{//
public:
	crModifyCharacterAttrMethod();
	crModifyCharacterAttrMethod(const crModifyCharacterAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyCharacterAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_scaleElement;
	std::string m_offsetXElement;
	std::string m_offsetYElement;
	std::string m_offsetZElement;
	std::string m_animListElement;
};
class crAddToListMethod : public CRCore::crMethod
{//
public:
	crAddToListMethod();
	crAddToListMethod(const crAddToListMethod& handle);
	MethodClass(CREncapsulation, AddToList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_fromListbox;
	std::string m_toListBoxElement;
};
class crListDeleteCurrentDataMethod : public CRCore::crMethod
{//
public:
	crListDeleteCurrentDataMethod();
	crListDeleteCurrentDataMethod(const crListDeleteCurrentDataMethod& handle);
	MethodClass(CREncapsulation, ListDeleteCurrentData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
};
class crUpdateObjectAvoidLightListMethod : public CRCore::crMethod
{//
public:
	crUpdateObjectAvoidLightListMethod();
	crUpdateObjectAvoidLightListMethod(const crUpdateObjectAvoidLightListMethod& handle);
	MethodClass(CREncapsulation, UpdateObjectAvoidLightList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_selectListElement;
    std::string m_avoidLightElement;
};
class crUpdateObjectNeedLightListMethod : public CRCore::crMethod
{//
public:
	crUpdateObjectNeedLightListMethod();
	crUpdateObjectNeedLightListMethod(const crUpdateObjectNeedLightListMethod& handle);
	MethodClass(CREncapsulation, UpdateObjectNeedLightList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_selectListElement;
	std::string m_needLightElement;
};

class crDisplayDrawableMaterialMethod : public CRCore::crMethod
{//
public:
	crDisplayDrawableMaterialMethod();
	crDisplayDrawableMaterialMethod(const crDisplayDrawableMaterialMethod& handle);
	MethodClass(CREncapsulation, DisplayDrawableMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static void getFilterModeStr(CRCore::crTexture::FilterMode filterMode,std::string &str);
	static void getWrapModeStr(CRCore::crTexture::WrapMode wrapMode,std::string &str);
	static void getTextureOperateStr(int op,std::string &str);
	static void getAlphaOperateStr(int op,std::string &str);
	static void getBinDetailStr(int binnum,std::string &str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_currentSelectElement;
	std::string m_baseMapElement;
	std::string m_normalMapElement;
	std::string m_lightMapElement;
	std::string m_envMapElement;
	std::string m_diffuseElement;
	std::string m_specularElement;
	std::string m_emissiveElement;
	std::string m_normal2Element;
	std::string m_diffuse2Element;
	std::string m_specular2Element;
	std::string m_emissive2Element;
	std::string m_normal3Element;
	std::string m_specular3Element;
	std::string m_emissive3Element;
	///////ExternTexture
	std::string m_uvsElement;
	std::string m_colorWeightElement;
	////////////////////
	std::string m_ambientColorElement;
	std::string m_ambientColorRElement;
	std::string m_ambientColorGElement;
	std::string m_ambientColorBElement;
	std::string m_diffuseColorElement;
	std::string m_diffuseColorRElement;
	std::string m_diffuseColorGElement;
	std::string m_diffuseColorBElement;
	std::string m_specularColorElement;
	std::string m_specularColorRElement;
	std::string m_specularColorGElement;
	std::string m_specularColorBElement;
	std::string m_shininessElement;
	std::string m_emissiveColorElement;
	std::string m_emissiveColorRElement;
	std::string m_emissiveColorGElement;
	std::string m_emissiveColorBElement;
    std::string m_detailGridElement;
};

class crFocusTextureAttrMethod : public CRCore::crMethod
{//
public:
	crFocusTextureAttrMethod();
	crFocusTextureAttrMethod(const crFocusTextureAttrMethod& handle);
	MethodClass(CREncapsulation, FocusTextureAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
    std::string m_detailGridElement;
	std::string m_texTypeName;
};

class crGetImageNameMethod : public CRCore::crMethod
{//
public:
	crGetImageNameMethod();
	crGetImageNameMethod(const crGetImageNameMethod& handle);
	MethodClass(CREncapsulation, GetImageName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editBox;
	std::string m_filePath;
	bool m_isEnvMap;
	int m_filterIndex;
};

class crModifyMaterialMethod : public CRCore::crMethod
{//
public:
	crModifyMaterialMethod();
	crModifyMaterialMethod(const crModifyMaterialMethod& handle);
	MethodClass(CREncapsulation, ModifyMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static CRCore::crTexture::FilterMode getFilterModeFromStr(const std::string &str);
	static CRCore::crTexture::WrapMode getWrapModeFromStr(const std::string &str);
	static int getTextureOperateFromStr(const std::string &str);
	static int getAlphaOperateFromStr(const std::string &str);
	static int getDetailBin(const std::string &str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_baseMapElement;
	std::string m_normalMapElement;
	std::string m_lightMapElement;
	std::string m_envMapElement;
	std::string m_diffuseElement;
	std::string m_specularElement;
	std::string m_emissiveElement;
	std::string m_normal2Element;
	std::string m_diffuse2Element;
	std::string m_specular2Element;
	std::string m_emissive2Element;
	std::string m_normal3Element;
	std::string m_specular3Element;
	std::string m_emissive3Element;
	///////ExternTexture
	std::string m_uvsElement;
	std::string m_colorWeightElement;
	////////////////////
	std::string m_ambientColorElement;
	std::string m_ambientColorRElement;
	std::string m_ambientColorGElement;
	std::string m_ambientColorBElement;
	std::string m_diffuseColorElement;
	std::string m_diffuseColorRElement;
	std::string m_diffuseColorGElement;
	std::string m_diffuseColorBElement;
	std::string m_specularColorElement;
	std::string m_specularColorRElement;
	std::string m_specularColorGElement;
	std::string m_specularColorBElement;
	std::string m_shininessElement;
	std::string m_emissiveColorElement;
	std::string m_emissiveColorRElement;
	std::string m_emissiveColorGElement;
	std::string m_emissiveColorBElement;
	std::string m_detailGridElement;
};

class crSaveNodeMethod : public CRCore::crMethod
{//
public:
	crSaveNodeMethod();
	crSaveNodeMethod(const crSaveNodeMethod& handle);
	MethodClass(CREncapsulation, SaveNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_filePathElement;
};

class crModifyObjectDrawableMethod : public CRCore::crMethod
{//
public:
	crModifyObjectDrawableMethod();
	crModifyObjectDrawableMethod(const crModifyObjectDrawableMethod& handle);
	MethodClass(CREncapsulation, ModifyObjectDrawable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crObject> m_currentSelectObject;
    CRCore::ref_ptr<CRCore::crDrawable> m_currentSelectDrawable;
};

class crSnapMaterialMethod : public CRCore::crMethod
{//
public:
	crSnapMaterialMethod();
	crSnapMaterialMethod(const crSnapMaterialMethod& handle);
	MethodClass(CREncapsulation, SnapMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
	int m_selectType;
};

class crPickNeedLightMethod : public CRCore::crMethod
{//
public:
	crPickNeedLightMethod();
	crPickNeedLightMethod(const crPickNeedLightMethod& handle);
	MethodClass(CREncapsulation, PickNeedLight)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};

class crCheckBoxSetSelectMethod : public CRCore::crMethod
{//
public:
	crCheckBoxSetSelectMethod();
	crCheckBoxSetSelectMethod(const crCheckBoxSetSelectMethod& handle);
	MethodClass(CREncapsulation, CheckBoxSetSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
	bool m_select;
};
class crCheckBoxSetSelectExMethod : public CRCore::crMethod
{//
public:
	crCheckBoxSetSelectExMethod();
	crCheckBoxSetSelectExMethod(const crCheckBoxSetSelectExMethod& handle);
	MethodClass(CREncapsulation, CheckBoxSetSelectEx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_checkBoxElement;
	bool m_select;
};

class crGetSaveFileNameMethod : public CRCore::crMethod
{//
public:
	crGetSaveFileNameMethod();
	crGetSaveFileNameMethod(const crGetSaveFileNameMethod& handle);
	MethodClass(CREncapsulation, GetSaveFileName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editorBox;
	std::string m_filePath;
	std::string m_fileExt;
};

class crDisplayVolumeAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayVolumeAttrMethod();
	crDisplayVolumeAttrMethod(const crDisplayVolumeAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayVolumeAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_currentSelectElement;
	std::string m_enableElement;
	std::string m_maxTriggerCountElement;
	std::string m_reTriggerDelayElement;
	std::string m_desEditElement;
};

class crModifyVolumeAttrMethod : public CRCore::crMethod
{//
public:
	crModifyVolumeAttrMethod();
	crModifyVolumeAttrMethod(const crModifyVolumeAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyVolumeAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_enableElement;
	std::string m_maxTriggerCountElement;
	std::string m_reTriggerDelayElement;
	std::string m_desEditElement;
};

class crDisplayMoviesAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayMoviesAttrMethod();
	crDisplayMoviesAttrMethod(const crDisplayMoviesAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayMoviesAttr)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_currentSelectElement;
	std::string m_loopMode;
	std::string m_pivotX;
	std::string m_pivotY;
	std::string m_pivotZ;
	std::string m_timeOffset;
	std::string m_timeMult;
	std::string m_keyGrid;
	std::string m_playMode;
};
class crAddKeyFrameMethod : public CRCore::crMethod
{//
public:
	crAddKeyFrameMethod();
	crAddKeyFrameMethod(const crAddKeyFrameMethod& handle);
	MethodClass(CREncapsulation, AddKeyFrame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crModifyMoviesAttrMethod : public CRCore::crMethod
{//
public:
	crModifyMoviesAttrMethod();
	crModifyMoviesAttrMethod(const crModifyMoviesAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyMoviesAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_loopMode;
	std::string m_pivotX;
	std::string m_pivotY;
	std::string m_pivotZ;
	std::string m_timeOffset;
	std::string m_timeMult;
	std::string m_keyGrid;
};
class crDelKeyFrameMethod : public CRCore::crMethod
{//
public:
	crDelKeyFrameMethod();
	crDelKeyFrameMethod(const crDelKeyFrameMethod& handle);
	MethodClass(CREncapsulation, DelKeyFrame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_keyGrid;
};
class crSetAnimationPauseMethod : public CRCore::crMethod
{//
public:
	crSetAnimationPauseMethod();
	crSetAnimationPauseMethod(const crSetAnimationPauseMethod& handle);
	MethodClass(CREncapsulation, SetAnimationPause)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_pause;
};
class crResetAnimationMethod : public CRCore::crMethod
{//
public:
	crResetAnimationMethod();
	crResetAnimationMethod(const crResetAnimationMethod& handle);
	MethodClass(CREncapsulation, ResetAnimation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crSaveMoviesAttrMethod : public CRCore::crMethod
{//
public:
	crSaveMoviesAttrMethod();
	crSaveMoviesAttrMethod(const crSaveMoviesAttrMethod& handle);
	MethodClass(CREncapsulation, SaveMoviesAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crCreateMoviePathMethod : public CRCore::crMethod
{//
public:
	crCreateMoviePathMethod();
	crCreateMoviePathMethod(const crCreateMoviePathMethod& handle);
	MethodClass(CREncapsulation, CreateMoviePath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crGroup *m_root;
	std::string m_rootName;
};
class crModifyKeyFrameMethod : public CRCore::crMethod
{//
public:
	crModifyKeyFrameMethod();
	crModifyKeyFrameMethod(const crModifyKeyFrameMethod& handle);
	MethodClass(CREncapsulation, ModifyKeyFrame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_keyGrid;
};
class crSelectKeyFrameMethod : public CRCore::crMethod
{//
public:
	crSelectKeyFrameMethod();
	crSelectKeyFrameMethod(const crSelectKeyFrameMethod& handle);
	MethodClass(CREncapsulation, SelectKeyFrame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crKeyValueGridElement> m_keyGridElement;
};
class crExcElementHandleMethod : public CRCore::crMethod
{//
public:
	crExcElementHandleMethod();
	crExcElementHandleMethod(const crExcElementHandleMethod& handle);
	MethodClass(CREncapsulation, ExcElementHandle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_targetElementName;
	std::string m_lParam;
    std::string m_wParam;
};
class crInitAnimationModeMethod : public CRCore::crMethod
{//
public:
	crInitAnimationModeMethod();
	crInitAnimationModeMethod(const crInitAnimationModeMethod& handle);
	MethodClass(CREncapsulation, InitAnimationMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_playModeElement;
};
class crRadioGroupSelectMethod : public CRCore::crMethod
{//
public:
	crRadioGroupSelectMethod();
	crRadioGroupSelectMethod(const crRadioGroupSelectMethod& handle);
	MethodClass(CREncapsulation, RadioGroupSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_radioGroupElement;
	int m_select;
};
class crMovePathMethod : public CRCore::crMethod
{//
public:
	crMovePathMethod();
	crMovePathMethod(const crMovePathMethod& handle);
	MethodClass(CREncapsulation, MovePath)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_pivotX;
	std::string m_pivotY;
	std::string m_pivotZ;
};
class crModifyGridMethod : public CRCore::crMethod
{//
public:
	crModifyGridMethod();
	crModifyGridMethod(const crModifyGridMethod& handle);
	MethodClass(CREncapsulation, ModifyGrid)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_rowsElement;
	std::string m_dimensionElement;
	std::string m_zoffsetElement;
};
class crBuildTerrainMethod : public CRCore::crMethod
{//
public:
	crBuildTerrainMethod();
	crBuildTerrainMethod(const crBuildTerrainMethod& handle);
	MethodClass(CREncapsulation, BuildTerrain)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_defaultFileName;
	std::string m_rootName;
	std::string m_columnsElement;
	std::string m_rowsElement;
	std::string m_lengthElement;
	std::string m_widthElement;
	std::string m_heightMapElement;
	std::string m_maxHeightElement;
};
class crModifyTerrainMethod : public CRCore::crMethod
{//
public:
	crModifyTerrainMethod();
	crModifyTerrainMethod(const crModifyTerrainMethod& handle);
	MethodClass(CREncapsulation, ModifyTerrain)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_columnsElement;
	std::string m_rowsElement;
	std::string m_lengthElement;
	std::string m_widthElement;
	std::string m_heightMapElement;
	std::string m_maxHeightElement;
};

class crShowTerrainGridMethod : public CRCore::crMethod
{//
public:
	crShowTerrainGridMethod();
	crShowTerrainGridMethod(const crShowTerrainGridMethod& handle);
	MethodClass(CREncapsulation, ShowTerrainGrid)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkbox;
};

class crHideAllTerrainsGridMethod : public CRCore::crMethod
{//
public:
	crHideAllTerrainsGridMethod();
	crHideAllTerrainsGridMethod(const crHideAllTerrainsGridMethod& handle);
	MethodClass(CREncapsulation, HideAllTerrainsGrid)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crDisplayTerrainAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayTerrainAttrMethod();
	crDisplayTerrainAttrMethod(const crDisplayTerrainAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayTerrainAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_currentSelectElement;
	std::string m_columnsElement;
	std::string m_rowsElement;
	std::string m_lengthElement;
	std::string m_widthElement;
	std::string m_heightMapElement;
	std::string m_maxHeightElement;
	std::string m_showGridElement;
	std::string m_colorWeightElement;
};

class crDisplayPostProcessAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayPostProcessAttrMethod();
	crDisplayPostProcessAttrMethod(const crDisplayPostProcessAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayPostProcessAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_ambientIntensityElement;
	std::string m_ambientIntensityRElement;
	std::string m_ambientIntensityGElement;
	std::string m_ambientIntensityBElement;
	std::string m_brightnessElement;
	std::string m_enableFogElement;
	std::string m_fogColorElement;
	std::string m_fogColorRElement;
	std::string m_fogColorGElement;
	std::string m_fogColorBElement;
	std::string m_fogColorStartElement;
	std::string m_fogColorEndElement;
	std::string m_fogColorDensityElement;
	std::string m_fogColorHeightElement;
	std::string m_maxDepthElement;
	std::string m_nearClipElement;
	std::string m_farClipElement;
	//std::string m_enableMotionBlurElement;
	std::string m_motionBlurFactorElement;
	//std::string m_hdrElement;
	//std::string m_hdrParam1Element;
	//std::string m_hdrParam2Element;
	//std::string m_hdrParam3Element;
	//std::string m_hdrParam4Element;
	//std::string m_glowSizeElement;
	//std::string m_enableDOFElement;
	std::string m_dofBlurStartElement;
	std::string m_dofBlurFactorElement;

	std::string m_enableCollideElement;
	std::string m_groundDetectElement;

	std::string m_upperSkyColorElement;
	std::string m_upperSkyColorRElement;
	std::string m_upperSkyColorGElement;
	std::string m_upperSkyColorBElement;
	std::string m_upperSkyColorLumElement;

	std::string m_lowerSkyColorElement;
	std::string m_lowerSkyColorRElement;
	std::string m_lowerSkyColorGElement;
	std::string m_lowerSkyColorBElement;
	std::string m_lowerSkyColorLumElement;
	std::string m_staticMeshShadowElement;
};

class crSetEnableIntersectMethod : public CRCore::crMethod
{//
public:
	crSetEnableIntersectMethod();
	crSetEnableIntersectMethod(const crSetEnableIntersectMethod& handle);
	MethodClass(CREncapsulation, SetEnableIntersect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkbox;
};

class crSetGroundDetectMethod : public CRCore::crMethod
{//
public:
	crSetGroundDetectMethod();
	crSetGroundDetectMethod(const crSetGroundDetectMethod& handle);
	MethodClass(CREncapsulation, SetGroundDetect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkbox;
};

class crModifyPostProcessAttrMethod : public CRCore::crMethod
{//
public:
	crModifyPostProcessAttrMethod();
	crModifyPostProcessAttrMethod(const crModifyPostProcessAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyPostProcessAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_ambientIntensityElement;
	std::string m_ambientIntensityRElement;
	std::string m_ambientIntensityGElement;
	std::string m_ambientIntensityBElement;
	std::string m_brightnessElement;
	std::string m_enableFogElement;
	std::string m_fogColorElement;
	std::string m_fogColorRElement;
	std::string m_fogColorGElement;
	std::string m_fogColorBElement;
	std::string m_fogColorStartElement;
	std::string m_fogColorEndElement;
	std::string m_fogColorDensityElement;
	std::string m_fogColorHeightElement;
	std::string m_maxDepthElement;
	std::string m_nearClipElement;
	std::string m_farClipElement;
	//std::string m_enableMotionBlurElement;
	std::string m_motionBlurFactorElement;
	//std::string m_hdrElement;
	//std::string m_hdrParam1Element;
	//std::string m_hdrParam2Element;
	//std::string m_hdrParam3Element;
	//std::string m_hdrParam4Element;
	//std::string m_glowSizeElement;
	//std::string m_enableDOFElement;
	std::string m_dofBlurStartElement;
	std::string m_dofBlurFactorElement;

	std::string m_upperSkyColorElement;
	std::string m_upperSkyColorRElement;
	std::string m_upperSkyColorGElement;
	std::string m_upperSkyColorBElement;
	std::string m_upperSkyColorLumElement;

	std::string m_lowerSkyColorElement;
	std::string m_lowerSkyColorRElement;
	std::string m_lowerSkyColorGElement;
	std::string m_lowerSkyColorBElement;
	std::string m_lowerSkyColorLumElement;
	std::string m_staticMeshShadowElement;
};

class crDisplayMultiSwitchAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayMultiSwitchAttrMethod();
	crDisplayMultiSwitchAttrMethod(const crDisplayMultiSwitchAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayMultiSwitchAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_maskCountElement;
	std::string m_currentMaskElement;
	std::string m_childGridElement;
};
class crSetActiveSwitchSetMethod : public CRCore::crMethod
{//
public:
	crSetActiveSwitchSetMethod();
	crSetActiveSwitchSetMethod(const crSetActiveSwitchSetMethod& handle);
	MethodClass(CREncapsulation, SetActiveSwitchSet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crEditBoxElement> m_editbox;
};
class crMultiSwitchPreMaskMethod : public CRCore::crMethod
{//
public:
	crMultiSwitchPreMaskMethod();
	crMultiSwitchPreMaskMethod(const crMultiSwitchPreMaskMethod& handle);
	MethodClass(CREncapsulation, MultiSwitchPreMask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crMultiSwitchNextMaskMethod : public CRCore::crMethod
{//
public:
	crMultiSwitchNextMaskMethod();
	crMultiSwitchNextMaskMethod(const crMultiSwitchNextMaskMethod& handle);
	MethodClass(CREncapsulation, MultiSwitchNextMask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crMultiSwitchAllOnMethod : public CRCore::crMethod
{//
public:
	crMultiSwitchAllOnMethod();
	crMultiSwitchAllOnMethod(const crMultiSwitchAllOnMethod& handle);
	MethodClass(CREncapsulation, MultiSwitchAllOn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crMultiSwitchAllOffMethod : public CRCore::crMethod
{//
public:
	crMultiSwitchAllOffMethod();
	crMultiSwitchAllOffMethod(const crMultiSwitchAllOffMethod& handle);
	MethodClass(CREncapsulation, MultiSwitchAllOff)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crPickChildMethod : public CRCore::crMethod
{//
public:
	crPickChildMethod();
	crPickChildMethod(const crPickChildMethod& handle);
	MethodClass(CREncapsulation, PickChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
	int m_selectType;
};
class crAddChildMethod : public CRCore::crMethod
{//
public:
	crAddChildMethod();
	crAddChildMethod(const crAddChildMethod& handle);
	MethodClass(CREncapsulation, AddChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crRemoveChildMethod : public CRCore::crMethod
{//
public:
	crRemoveChildMethod();
	crRemoveChildMethod(const crRemoveChildMethod& handle);
	MethodClass(CREncapsulation, RemoveChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_childGridElement;
};

class crRemovePagedLodChildMethod : public CRCore::crMethod
{//
public:
	crRemovePagedLodChildMethod();
	crRemovePagedLodChildMethod(const crRemovePagedLodChildMethod& handle);
	MethodClass(CREncapsulation, RemovePagedLodChild)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_childGridElement;
};

class crModifyMultiSwitchAttrMethod : public CRCore::crMethod
{//
public:
	crModifyMultiSwitchAttrMethod();
	crModifyMultiSwitchAttrMethod(const crModifyMultiSwitchAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyMultiSwitchAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_childGridElement;
};

class crAddSwitchSetMethod : public CRCore::crMethod
{//
public:
	crAddSwitchSetMethod();
	crAddSwitchSetMethod(const crAddSwitchSetMethod& handle);
	MethodClass(CREncapsulation, AddSwitchSet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

class crDelSwitchSetMethod : public CRCore::crMethod
{//
public:
	crDelSwitchSetMethod();
	crDelSwitchSetMethod(const crDelSwitchSetMethod& handle);
	MethodClass(CREncapsulation, DelSwitchSet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crUpdateNodeListMethod : public CRCore::crMethod
{//
public:
	crUpdateNodeListMethod();
	crUpdateNodeListMethod(const crUpdateNodeListMethod& handle);
	MethodClass(CREncapsulation, UpdateNodeList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_nodeClassName;
};

class crSelectNodeMethod : public CRCore::crMethod
{//
public:
	crSelectNodeMethod();
	crSelectNodeMethod(const crSelectNodeMethod& handle);
	MethodClass(CREncapsulation, SelectNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_nodeClassName;
};

class crDisplayLodAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayLodAttrMethod();
	crDisplayLodAttrMethod(const crDisplayLodAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayLodAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_centerModeElement;
	std::string m_centerXElement;
	std::string m_centerYElement;
	std::string m_centerZElement;
	std::string m_radiusElement;
	std::string m_childGridElement;
};

class crModifyLodAttrMethod : public CRCore::crMethod
{//
public:
	crModifyLodAttrMethod();
	crModifyLodAttrMethod(const crModifyLodAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyLodAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_centerModeElement;
	std::string m_centerXElement;
	std::string m_centerYElement;
	std::string m_centerZElement;
	std::string m_radiusElement;
	std::string m_childGridElement;
};

class crDisplayPagedLodAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayPagedLodAttrMethod();
	crDisplayPagedLodAttrMethod(const crDisplayPagedLodAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayPagedLodAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_centerModeElement;
	std::string m_centerXElement;
	std::string m_centerYElement;
	std::string m_centerZElement;
	std::string m_radiusElement;
	std::string m_childGridElement;
};

class crModifyPagedLodAttrMethod : public CRCore::crMethod
{//
public:
	crModifyPagedLodAttrMethod();
	crModifyPagedLodAttrMethod(const crModifyPagedLodAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyPagedLodAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_centerModeElement;
	std::string m_centerXElement;
	std::string m_centerYElement;
	std::string m_centerZElement;
	std::string m_radiusElement;
	std::string m_childGridElement;
};

class crFullLoadPagedLodMethod : public CRCore::crMethod
{//
public:
	crFullLoadPagedLodMethod();
	crFullLoadPagedLodMethod(const crFullLoadPagedLodMethod& handle);
	MethodClass(CREncapsulation, FullLoadPagedLod)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crHiddenOrUnHiddenNodes2Method : public CRCore::crMethod
{//
public:
	crHiddenOrUnHiddenNodes2Method();
	crHiddenOrUnHiddenNodes2Method(const crHiddenOrUnHiddenNodes2Method& handle);
	MethodClass(CREncapsulation, HiddenOrUnHiddenNodes2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
	typedef std::vector<std::string> NodeNameArray;
	NodeNameArray m_nodeNameArray;
};

class crWireDrawModeMethod : public CRCore::crMethod
{//
public:
	crWireDrawModeMethod();
	crWireDrawModeMethod(const crWireDrawModeMethod& handle);
	MethodClass(CREncapsulation, WireDrawMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};

class crSetLayeredWindowAttributesMethod : public CRCore::crMethod
{//
public:
	crSetLayeredWindowAttributesMethod();
	crSetLayeredWindowAttributesMethod(const crSetLayeredWindowAttributesMethod& handle);
	MethodClass(CREncapsulation, SetLayeredWindowAttributes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	COLORREF m_keycolor;
	unsigned char m_alpha;
    unsigned char m_flg;//1:LWA_COLORKEY 2:LWA_ALPHA
};

class crCombineObjectsMethod : public CRCore::crMethod
{//将当前所选中的Object合并到第一个Object下
public:
	crCombineObjectsMethod();
	crCombineObjectsMethod(const crCombineObjectsMethod& handle);
	MethodClass(CREncapsulation, CombineObjects)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crCombineDrawablesMethod : public CRCore::crMethod
{//将当前所选中的Object里的Drawable合并到第一个Object下
public:
	crCombineDrawablesMethod();
	crCombineDrawablesMethod(const crCombineDrawablesMethod& handle);
	MethodClass(CREncapsulation, CombineDrawables)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crEditorMoveCameraMethod : public CRCore::crMethod
{//Editor Method 
public:
	crEditorMoveCameraMethod();
	crEditorMoveCameraMethod(const crEditorMoveCameraMethod& handle);
	MethodClass(CREncapsulation, EditorMoveCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_xPos;
	std::string m_yPos;
	std::string m_zPos;
};

class crBuildNodeTreeMethod : public CRCore::crMethod
{//Editor Method 
public:
	crBuildNodeTreeMethod();
	crBuildNodeTreeMethod(const crBuildNodeTreeMethod& handle);
	MethodClass(CREncapsulation, BuildNodeTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	enum BuildType
	{
		SelectNode,
		StaticModel,
		UI,
		NotRender,
		Trigger,
	};
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_nodeTreeElement;
	std::string m_countElement;
	unsigned char m_buildType;
};
class crSeletNodeByNodeTreeMethod : public CRCore::crMethod
{//Editor Method 
public:
	crSeletNodeByNodeTreeMethod();
	crSeletNodeByNodeTreeMethod(const crSeletNodeByNodeTreeMethod& handle);
	MethodClass(CREncapsulation, SeletNodeByNodeTree)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crTreeElement> m_nodeTreeElement;
};

class crInitTerraformModeMethod : public CRCore::crMethod
{//
public:
	crInitTerraformModeMethod();
	crInitTerraformModeMethod(const crInitTerraformModeMethod& handle);
	MethodClass(CREncapsulation, InitTerraformMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crSetTerraformModeMethod : public CRCore::crMethod
{//
public:
	crSetTerraformModeMethod();
	crSetTerraformModeMethod(const crSetTerraformModeMethod& handle);
	MethodClass(CREncapsulation, SetTerraformMode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned char m_terraformMode;
};
class crInitCurrentTerToolMethod : public CRCore::crMethod
{//
public:
	crInitCurrentTerToolMethod();
	crInitCurrentTerToolMethod(const crInitCurrentTerToolMethod& handle);
	MethodClass(CREncapsulation, InitCurrentTerTool)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crRadioGroupElement> m_radioGroupElement;
};
class crSetCurrentTerToolMethod : public CRCore::crMethod
{//
public:
	crSetCurrentTerToolMethod();
	crSetCurrentTerToolMethod(const crSetCurrentTerToolMethod& handle);
	MethodClass(CREncapsulation, SetCurrentTerTool)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned char m_currentTerTool;
};
class crSetTerEditParamMethod : public CRCore::crMethod
{//
public:
	crSetTerEditParamMethod();
	crSetTerEditParamMethod(const crSetTerEditParamMethod& handle);
	MethodClass(CREncapsulation, SetTerEditParam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_radius;
	std::string m_strength;
};
class crDisplayDofParamMethod : public CRCore::crMethod
{//
public:
	crDisplayDofParamMethod();
	crDisplayDofParamMethod(const crDisplayDofParamMethod& handle);
	MethodClass(CREncapsulation, DisplayDofParam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_stage;
	std::string m_gridElement;
};
class crModifyDofParamMethod : public CRCore::crMethod
{//
public:
	crModifyDofParamMethod();
	crModifyDofParamMethod(const crModifyDofParamMethod& handle);
	MethodClass(CREncapsulation, ModifyDofParam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_gridElement;
};
class crFocusDofTexMatMethod : public CRCore::crMethod
{//
public:
	crFocusDofTexMatMethod();
	crFocusDofTexMatMethod(const crFocusDofTexMatMethod& handle);
	MethodClass(CREncapsulation, FocusDofTexMat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_texTypeName;
};
class crSaveDofTexMatMethod : public CRCore::crMethod
{//
public:
	crSaveDofTexMatMethod();
	crSaveDofTexMatMethod(const crSaveDofTexMatMethod& handle);
	MethodClass(CREncapsulation, SaveDofTexMat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_gridElement;
};
class crNextDofTexMatMethod : public CRCore::crMethod
{//
public:
	crNextDofTexMatMethod();
	crNextDofTexMatMethod(const crNextDofTexMatMethod& handle);
	MethodClass(CREncapsulation, NextDofTexMat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crShowEditorMenuMethod : public CRCore::crMethod
{//
public:
	crShowEditorMenuMethod();
	crShowEditorMenuMethod(const crShowEditorMenuMethod& handle);
	MethodClass(CREncapsulation, ShowEditorMenu)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_editorMenu;
};
class crNewParticleMethod : public CRCore::crMethod
{//
public:
	crNewParticleMethod();
	crNewParticleMethod(const crNewParticleMethod& handle);
	MethodClass(CREncapsulation, NewParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rootName;
	CRCore::crGroup* m_root;
	CRCore::ref_ptr<CRCore::crGroup> m_copyData;
};
class crDisplayParticleParamMethod : public CRCore::crMethod
{//
public:
	crDisplayParticleParamMethod();
	crDisplayParticleParamMethod(const crDisplayParticleParamMethod& handle);
	MethodClass(CREncapsulation, DisplayParticleParam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string getParticleDisplayName(const std::string &str);
	std::string getAlignedModeStr(int mode);
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_filePath;
	std::string m_currentSelectElement;
	std::string m_indexElement;
	std::string m_gridElement;
};
class crModifyParticleParamMethod : public CRCore::crMethod
{//
public:
	crModifyParticleParamMethod();
	crModifyParticleParamMethod(const crModifyParticleParamMethod& handle);
	MethodClass(CREncapsulation, ModifyParticleParam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string getParticleClassName(const std::string &str);
	int getAlignedMode(const std::string &str);
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	CRCore::crGroup *m_editorRoot;
	std::string m_editorRootName;//
	std::string m_listElement;
	std::string m_currentSelectElement;
	std::string m_indexElement;
	std::string m_gridElement;
};
class crAddParticleMethod : public CRCore::crMethod
{//
public:
	crAddParticleMethod();
	crAddParticleMethod(const crAddParticleMethod& handle);
	MethodClass(CREncapsulation, AddParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crDelParticleMethod : public CRCore::crMethod
{//
public:
	crDelParticleMethod();
	crDelParticleMethod(const crDelParticleMethod& handle);
	MethodClass(CREncapsulation, DelParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crNextParticleMethod : public CRCore::crMethod
{//
public:
	crNextParticleMethod();
	crNextParticleMethod(const crNextParticleMethod& handle);
	MethodClass(CREncapsulation, NextParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crCopyParticleMethod : public CRCore::crMethod
{//
public:
	crCopyParticleMethod();
	crCopyParticleMethod(const crCopyParticleMethod& handle);
	MethodClass(CREncapsulation, CopyParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crPastParticleMethod : public CRCore::crMethod
{//
public:
	crPastParticleMethod();
	crPastParticleMethod(const crPastParticleMethod& handle);
	MethodClass(CREncapsulation, PastParticle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crResetParticleIndexMethod : public CRCore::crMethod
{//
public:
	crResetParticleIndexMethod();
	crResetParticleIndexMethod(const crResetParticleIndexMethod& handle);
	MethodClass(CREncapsulation, ResetParticleIndex)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};
class crElementDoEventMethod : public CRCore::crMethod
{//
public:
	crElementDoEventMethod();
	crElementDoEventMethod(const crElementDoEventMethod& handle);
	MethodClass(CREncapsulation, ElementDoEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_stageName;
	std::string m_elementName;
	_crInt64 m_msg;
	_crInt64 m_param;
};
class crSelectParticleGroupMethod : public CRCore::crMethod
{//
public:
	crSelectParticleGroupMethod();
	crSelectParticleGroupMethod(const crSelectParticleGroupMethod& handle);
	MethodClass(CREncapsulation, SelectParticleGroup)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
};
class crSelectParticleEffectMethod : public CRCore::crMethod
{//
public:
	crSelectParticleEffectMethod();
	crSelectParticleEffectMethod(const crSelectParticleEffectMethod& handle);
	MethodClass(CREncapsulation, SelectParticleEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_listElement;
	std::string m_indexElement;
};

class crDisplaySequenceAttrMethod : public CRCore::crMethod
{//
public:
	crDisplaySequenceAttrMethod();
	crDisplaySequenceAttrMethod(const crDisplaySequenceAttrMethod& handle);
	MethodClass(CREncapsulation, DisplaySequenceAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_gridElement;
};

class crModifySequenceAttrMethod : public CRCore::crMethod
{//
public:
	crModifySequenceAttrMethod();
	crModifySequenceAttrMethod(const crModifySequenceAttrMethod& handle);
	MethodClass(CREncapsulation, ModifySequenceAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_currentSelectElement;
	std::string m_gridElement;
};
class crSelectNodeIsObjectMethod : public CRCore::crMethod
{//
public:
	crSelectNodeIsObjectMethod();
	crSelectNodeIsObjectMethod(const crSelectNodeIsObjectMethod& handle);
	MethodClass(CREncapsulation, SelectNodeIsObject)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crSaveSequenceMethod : public CRCore::crMethod
{//
public:
	crSaveSequenceMethod();
	crSaveSequenceMethod(const crSaveSequenceMethod& handle);
	MethodClass(CREncapsulation, SaveSequence)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crEditChangeCameraMethod : public CRCore::crMethod
{//
public:
	crEditChangeCameraMethod();
	crEditChangeCameraMethod(const crEditChangeCameraMethod& handle);
	MethodClass(CREncapsulation, EditChangeCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crEditReturnCameraMethod : public CRCore::crMethod
{//
public:
	crEditReturnCameraMethod();
	crEditReturnCameraMethod(const crEditReturnCameraMethod& handle);
	MethodClass(CREncapsulation, EditReturnCamera)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crFreezeModelMethod : public CRCore::crMethod
{//
public:
	crFreezeModelMethod();
	crFreezeModelMethod(const crFreezeModelMethod& handle);
	MethodClass(CREncapsulation, FreezeModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crCheckBoxElement> m_checkBox;
};
class crFreezeModel2Method : public CRCore::crMethod
{//
public:
	crFreezeModel2Method();
	crFreezeModel2Method(const crFreezeModel2Method& handle);
	MethodClass(CREncapsulation, FreezeModel2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_freeze;
};
class crDisconnectDrawableMethod : public CRCore::crMethod
{//
public:
	crDisconnectDrawableMethod();
	crDisconnectDrawableMethod(const crDisconnectDrawableMethod& handle);
	MethodClass(CREncapsulation, DisconnectDrawable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crRunSingleGameMethod : public CRCore::crMethod
{
public:
	crRunSingleGameMethod(){}
	//crRunSingleGameMethod(const crRunSingleGameMethod& handle);
	MethodClass(CREncapsulation, RunSingleGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	//std::string m_loadingDlgID;
	//std::string m_loadingProgress;
};
class crDisplayDuplicateAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayDuplicateAttrMethod();
	crDisplayDuplicateAttrMethod(const crDisplayDuplicateAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayDuplicateAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_currentSelectElement;
	std::string m_gridElement;
};

class crModifyDuplicateAttrMethod : public CRCore::crMethod
{//
public:
	crModifyDuplicateAttrMethod();
	crModifyDuplicateAttrMethod(const crModifyDuplicateAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyDuplicateAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_gridElement;
};
class crSaveDuplicateMethod : public CRCore::crMethod
{//
public:
	crSaveDuplicateMethod();
	crSaveDuplicateMethod(const crSaveDuplicateMethod& handle);
	MethodClass(CREncapsulation, SaveDuplicate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crUpdateNodeList2Method : public CRCore::crMethod
{//
public:
	crUpdateNodeList2Method();
	crUpdateNodeList2Method(const crUpdateNodeList2Method& handle);
	MethodClass(CREncapsulation, UpdateNodeList2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_callbackName;
};
class crSelectNode2Method : public CRCore::crMethod
{//
public:
	crSelectNode2Method();
	crSelectNode2Method(const crSelectNode2Method& handle);
	MethodClass(CREncapsulation, SelectNode2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crListBoxElement> m_listbox;
	std::string m_callbackName;
};
class crDisplayInitSettingMethod : public CRCore::crMethod
{//
public:
	crDisplayInitSettingMethod();
	crDisplayInitSettingMethod(const crDisplayInitSettingMethod& handle);
	MethodClass(CREncapsulation, DisplayInitSetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_gridElement;
};

class crModifyInitSettingMethod : public CRCore::crMethod
{//
public:
	crModifyInitSettingMethod();
	crModifyInitSettingMethod(const crModifyInitSettingMethod& handle);
	MethodClass(CREncapsulation, ModifyInitSetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_gridElement;
};
class crSaveInitSettingMethod : public CRCore::crMethod
{//
public:
	crSaveInitSettingMethod();
	crSaveInitSettingMethod(const crSaveInitSettingMethod& handle);
	MethodClass(CREncapsulation, SaveInitSetting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
};
class crDisplayBrainAttrMethod : public CRCore::crMethod
{//
public:
	crDisplayBrainAttrMethod();
	crDisplayBrainAttrMethod(const crDisplayBrainAttrMethod& handle);
	MethodClass(CREncapsulation, DisplayBrainAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crImageStage> m_imageStage;
	std::string m_gridElement;
};

class crModifyBrainAttrMethod : public CRCore::crMethod
{//
public:
	crModifyBrainAttrMethod();
	crModifyBrainAttrMethod(const crModifyBrainAttrMethod& handle);
	MethodClass(CREncapsulation, ModifyBrainAttr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	std::string m_gridElement;
};

class crGetModKeyMaskMethod : public CRCore::crMethod
{//
public:
	crGetModKeyMaskMethod();
	crGetModKeyMaskMethod(const crGetModKeyMaskMethod& handle);
	MethodClass(CREncapsulation, GetModKeyMask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	_crInt64 m_param;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
};

class crSceneModifiedMethod : public CRCore::crMethod
{//
public:
	crSceneModifiedMethod();
	crSceneModifiedMethod(const crSceneModifiedMethod& handle);
	MethodClass(CREncapsulation, SceneModified)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_modify;
};

class crNodeUpdateMethod : public CRCore::crMethod
{//
public:
	crNodeUpdateMethod();
	crNodeUpdateMethod(const crNodeUpdateMethod& handle);
	MethodClass(CREncapsulation, NodeUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode *m_this;
	CRCore::ref_ptr<CRUtil::crUpdateVisitor> m_updateVisitor;
};
class crAutoSaveSceneMethod : public CRCore::crMethod
{//
public:
	crAutoSaveSceneMethod();
	crAutoSaveSceneMethod(const crAutoSaveSceneMethod& handle);
	MethodClass(CREncapsulation, AutoSaveScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_fileName;
};
class crUnSelectAllMethod : public CRCore::crMethod
{//
public:
	crUnSelectAllMethod();
	crUnSelectAllMethod(const crUnSelectAllMethod& handle);
	MethodClass(CREncapsulation, UnSelectAll)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crExportCharacterPosMethod : public CRCore::crMethod
{//
public:
	crExportCharacterPosMethod();
	crExportCharacterPosMethod(const crExportCharacterPosMethod& handle);
	MethodClass(CREncapsulation, ExportCharacterPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
}
#endif