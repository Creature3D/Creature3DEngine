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
#ifndef CRNETAPP_UIMETHODCLASS_H
#define CRNETAPP_UIMETHODCLASS_H 1

#include <CRNetApp/appCharacterUpdateMethod.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRText/crText.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRCore/crUniversalHandles.h>
#include <CRPhysics/crCamera.h>
#include <CRCore/crMaterial.h>
#include <CRUtil/crSetObjectColorVisitor.h>
#include <CREncapsulation/crTableIO.h>
#include <CRUI/crWidgetExtend.h>
#include <ode/ode.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{
class CRNETAPP_EXPORT crUIShowNpcNameMethod : public CRCore::crMethod
{//Client UI Method
public:
	crUIShowNpcNameMethod();
	crUIShowNpcNameMethod(const crUIShowNpcNameMethod& handle);
	MethodClass(CRNetApp, UIShowNpcName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_widgetName;
};
class CRNETAPP_EXPORT crUIShowHyperTextMethod : public CRCore::crMethod
{//Client UI Method
public:
	crUIShowHyperTextMethod();
	crUIShowHyperTextMethod(const crUIShowHyperTextMethod& handle);
	MethodClass(CRNetApp, UIShowHyperText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_widgetName;
	std::string m_hypertextFile;
};
class CRNETAPP_EXPORT crCreateItemTalkMethod : public CRCore::crMethod
{//Client Player Method
public:
	crCreateItemTalkMethod();
	crCreateItemTalkMethod(const crCreateItemTalkMethod& handle);
	MethodClass(CRNetApp, CreateItemTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_tablefile;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_tasktable;
};
class CRNETAPP_EXPORT crShowItemTalkMethod : public CRCore::crMethod
{//Client UI Method
public:
	crShowItemTalkMethod();
	crShowItemTalkMethod(const crShowItemTalkMethod& handle);
	MethodClass(CRNetApp, ShowItemTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
	std::string m_canvasName;
	std::string m_widgetName;
	bool m_talkoropt;//ture = talk, false = opt
};
//class CRNETAPP_EXPORT crUIShowTaskMethod : public CRCore::crMethod
//{//Client UI Method
//public:
//	crUIShowTaskMethod();
//	crUIShowTaskMethod(const crUIShowTaskMethod& handle);
//	MethodClass(CRNetApp, UIShowTask)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	int m_taskid;
//	std::string* m_canvas;
//	std::string m_frameStaticText;
//	std::string m_desTextHypertext;
//	std::string m_progressHypertext;
//	std::string m_item1ImageBox;
//	std::string m_item2ImageBox;
//	std::string m_item3ImageBox;
//	std::string m_item4ImageBox;
//	std::string m_expStaticText;
//	std::string m_exp2StaticText;
//	std::string m_goldStaticText;
//	int m_waitTime;
//};class CRNETAPP_EXPORT crUIAwardItemTipInfoMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crUIAwardItemTipInfoMethod();
//	crUIAwardItemTipInfoMethod(const crUIAwardItemTipInfoMethod& handle);
//	MethodClass(CRNetApp, UIAwardItemTipInfo)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	char m_id;
//	std::string m_tipCanvasName;
//	std::string m_tipWidgetName;
//	CRProducer::crGUIEventAdapter* m_ea;
//};
class CRNETAPP_EXPORT crUIShowTask2Method : public CRCore::crMethod
{//Client UI Method
public:
	crUIShowTask2Method();
	crUIShowTask2Method(const crUIShowTask2Method& handle);
	MethodClass(CRNetApp, UIShowTask2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListBoxWidgetNode* m_this;
	int m_taskid;
	std::string m_canvas;
};
class CRNETAPP_EXPORT crSetCurrentTaskMethod : public CRCore::crMethod
{//Client UI Method
public:
	crSetCurrentTaskMethod();
	crSetCurrentTaskMethod(const crSetCurrentTaskMethod& handle);
	MethodClass(CRNetApp, SetCurrentTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_taskid;
};
class CRNETAPP_EXPORT cr3DUINotifyMethod : public CRCore::crMethod
{//GameClient Method 
public:
	cr3DUINotifyMethod();
	cr3DUINotifyMethod(const cr3DUINotifyMethod& handle);
	MethodClass(CRNetApp, 3DUINotify)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::wstring m_notice;
};
class CRNETAPP_EXPORT cr3DUINotify2Method : public CRCore::crMethod
{//GameClient Method 
public:
	cr3DUINotify2Method();
	cr3DUINotify2Method(const cr3DUINotify2Method& handle);
	MethodClass(CRNetApp, 3DUINotify2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::wstring m_notice;
};
class CRNETAPP_EXPORT crUIListControlTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIListControlTipInfoMethod();
	crUIListControlTipInfoMethod(const crUIListControlTipInfoMethod& handle);
	MethodClass(CRNetApp, UIListControlTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRProducer::crGUIEventAdapter* m_ea;
	bool m_emporium;
};
class CRNETAPP_EXPORT crUIItemListTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIItemListTipInfoMethod();
	crUIItemListTipInfoMethod(const crUIItemListTipInfoMethod& handle);
	MethodClass(CRNetApp, UIItemListTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRProducer::crGUIEventAdapter* m_ea;
};
class CRNETAPP_EXPORT crUIUpdateRole2Method : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateRole2Method();
	crUIUpdateRole2Method(const crUIUpdateRole2Method& handle);
	MethodClass(CRNetApp, UIUpdateRole2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_name;
	std::string m_lv;
	std::string m_metier;
	std::string m_exp;
	std::string m_gold;
	std::string m_hp;
	std::string m_mp;
	std::string m_attack;
	std::string m_defend;
	std::string m_strength;
	std::string m_agility;
	std::string m_intellect;
	std::string m_attrpoint;
	std::string m_skillpoint;
	std::string m_strengthL;
	std::string m_strengthR;
	std::string m_agilityL;
	std::string m_agilityR;
	std::string m_intellectL;
	std::string m_intellectR;
	std::string m_attrok;
};
class CRNETAPP_EXPORT crUIUpdateExpProgressMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateExpProgressMethod();
	crUIUpdateExpProgressMethod(const crUIUpdateExpProgressMethod& handle);
	MethodClass(CRNetApp, UIUpdateExpProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_expprogress;
};
//class CRNETAPP_EXPORT crBeginDragItemMethod : public CRCore::crMethod
//{
//public:
//	crBeginDragItemMethod();
//	crBeginDragItemMethod(const crBeginDragItemMethod& handle);
//	MethodClass(CRNetApp, BeginDragItem)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CREncapsulation::crListControlWidgetNode> m_this;
//};
class CRNETAPP_EXPORT crBuildTaskListMethod : public CRCore::crMethod
{
public:
	crBuildTaskListMethod();
	crBuildTaskListMethod(const crBuildTaskListMethod& handle);
	MethodClass(CRNetApp, BuildTaskList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class CRNETAPP_EXPORT crUIUpdateMyInfoMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateMyInfoMethod();
	crUIUpdateMyInfoMethod(const crUIUpdateMyInfoMethod& handle);
	MethodClass(CRNetApp, UIUpdateMyInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_headImg;
	std::string m_lv;
	std::string m_stats;
	std::string m_name;
	std::string m_hpProgress;
	std::string m_mpProgress;
	std::string m_stateBufList;
};
class CRNETAPP_EXPORT crUIUpdateTargetInfoMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateTargetInfoMethod();
	crUIUpdateTargetInfoMethod(const crUIUpdateTargetInfoMethod& handle);
	MethodClass(CRNetApp, UIUpdateTargetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_headImg;
	std::string m_lv;
	std::string m_name;
	std::string m_metier;
	std::string m_hpProgress;
};
class CRNETAPP_EXPORT crMyHPTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crMyHPTipInfoMethod();
	crMyHPTipInfoMethod(const crMyHPTipInfoMethod& handle);
	MethodClass(CRNetApp, MyHPTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRCore::crVector2 m_coord;
};
class CRNETAPP_EXPORT crMyMPTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crMyMPTipInfoMethod();
	crMyMPTipInfoMethod(const crMyMPTipInfoMethod& handle);
	MethodClass(CRNetApp, MyMPTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRCore::crVector2 m_coord;
};
class CRNETAPP_EXPORT crTargetHPTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crTargetHPTipInfoMethod();
	crTargetHPTipInfoMethod(const crTargetHPTipInfoMethod& handle);
	MethodClass(CRNetApp, TargetHPTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRCore::crVector2 m_coord;
};
class CRNETAPP_EXPORT crTargetMPTipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crTargetMPTipInfoMethod();
	crTargetMPTipInfoMethod(const crTargetMPTipInfoMethod& handle);
	MethodClass(CRNetApp, TargetMPTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_tipCanvasName;
	std::string m_tipWidgetName;
	CRCore::crVector2 m_coord;
};
class CRNETAPP_EXPORT crInitSceneMapMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crInitSceneMapMethod();
	crInitSceneMapMethod(const crInitSceneMapMethod& handle);
	MethodClass(CRNetApp, InitSceneMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_mapWidgetName;
	std::string m_imageFile;
	float m_scale;
	CRCore::crVector2 m_scaleRange;
	//std::string m_myIconWidget;
	float m_imageSize;
	bool m_staticmap;
};
class CRNETAPP_EXPORT crUIUpdateMapInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUpdateMapInfoMethod();
	crUIUpdateMapInfoMethod(const crUIUpdateMapInfoMethod& handle);
	MethodClass(CRNetApp, UIUpdateMapInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_nameWidget;
	std::string m_mapWidget;
	std::string m_myIconWidget;
	std::string m_friendPlayerIconWidget;
	std::string m_threadPlayerIconWidget;
	std::string m_enemyPlayerIconWidget;

	std::string m_friendNpcIconWidget;
	std::string m_threadNpcIconWidget;
	std::string m_enemyNpcIconWidget;
	std::string m_enemyBossIconWidget;

	std::string m_itemIconWidget;
	typedef std::vector< CRCore::ref_ptr<CRCore::crWidgetNode> >WidgetNodeVec;
	WidgetNodeVec m_friendPlayerWidgetVec;
	WidgetNodeVec m_threadPlayerWidgetVec;
	WidgetNodeVec m_enemyPlayerWidgetVec;
	WidgetNodeVec m_friendNpcWidgetVec;
	WidgetNodeVec m_threadNpcWidgetVec;
	WidgetNodeVec m_enemyNpcWidgetVec;
	WidgetNodeVec m_enemyBossWidgetVec;
	WidgetNodeVec m_itemWidgetVec;
};
class CRNETAPP_EXPORT crMapZoomMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crMapZoomMethod();
	crMapZoomMethod(const crMapZoomMethod& handle);
	MethodClass(CRNetApp, MapZoom)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_step;
};
class CRNETAPP_EXPORT crMapHitMoveMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crMapHitMoveMethod();
	crMapHitMoveMethod(const crMapHitMoveMethod& handle);
	MethodClass(CRNetApp, MapHitMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crImageBoxWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
};
//class CRNETAPP_EXPORT crInitBuyCountDlgMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crInitBuyCountDlgMethod();
//	crInitBuyCountDlgMethod(const crInitBuyCountDlgMethod& handle);
//	MethodClass(CRNetApp, InitBuyCountDlg)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRUI::crListControlWidgetNode* m_this;
//	//int m_selectIndex;
//	std::string m_canvasWidget;
//	std::string m_buyCountWidget;
//	std::string m_priceWidget;
//	bool m_emporium;
//};
//class CRNETAPP_EXPORT crInitSaleCountDlgMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crInitSaleCountDlgMethod();
//	crInitSaleCountDlgMethod(const crInitSaleCountDlgMethod& handle);
//	MethodClass(CRNetApp, InitSaleCountDlg)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRUI::crListControlWidgetNode* m_this;
//	std::string m_canvasWidget;
//	std::string m_saleCountWidget;
//	std::string m_priceWidget;
//};
//class CRNETAPP_EXPORT crBuyCountChangeMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crBuyCountChangeMethod();
//	crBuyCountChangeMethod(const crBuyCountChangeMethod& handle);
//	MethodClass(CRNetApp, BuyCountChange)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRUI::crEditWidgetNode> m_this;
//	std::string m_priceWidget;
//};
//class CRNETAPP_EXPORT crSaleCountChangeMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crSaleCountChangeMethod();
//	crSaleCountChangeMethod(const crSaleCountChangeMethod& handle);
//	MethodClass(CRNetApp, SaleCountChange)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::ref_ptr<CRUI::crEditWidgetNode> m_this;
//	std::string m_priceWidget;
//};
class CRNETAPP_EXPORT crUIUpdateTaskTraceMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crUIUpdateTaskTraceMethod();
	crUIUpdateTaskTraceMethod(const crUIUpdateTaskTraceMethod& handle);
	MethodClass(CRNetApp, UIUpdateTaskTrace)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_traceWidget;
};
class CRNETAPP_EXPORT crBackPackItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crBackPackItemDragEventMethod();
	crBackPackItemDragEventMethod(const crBackPackItemDragEventMethod& handle);
	MethodClass(CRNetApp, BackPackItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_storeCanvasName;
	std::string m_storageCanvasName;
	std::string m_panelCanvasName;
	std::string m_bodyCanvasName;
};
class CRNETAPP_EXPORT crSaleItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crSaleItemDragEventMethod();
	crSaleItemDragEventMethod(const crSaleItemDragEventMethod& handle);
	MethodClass(CRNetApp, SaleItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_backpackCanvasName;
};
class CRNETAPP_EXPORT crSkillItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crSkillItemDragEventMethod();
	crSkillItemDragEventMethod(const crSkillItemDragEventMethod& handle);
	MethodClass(CRNetApp, SkillItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_panelCanvasName;
};
class CRNETAPP_EXPORT crQuickItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crQuickItemDragEventMethod();
	crQuickItemDragEventMethod(const crQuickItemDragEventMethod& handle);
	MethodClass(CRNetApp, QuickItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
};
class CRNETAPP_EXPORT crEquipOnItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crEquipOnItemDragEventMethod();
	crEquipOnItemDragEventMethod(const crEquipOnItemDragEventMethod& handle);
	MethodClass(CRNetApp, EquipOnItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_backpackCanvasName;
};
class CRNETAPP_EXPORT crStorageItemDragEventMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crStorageItemDragEventMethod();
	crStorageItemDragEventMethod(const crStorageItemDragEventMethod& handle);
	MethodClass(CRNetApp, StorageItemDragEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_backpackCanvasName;
};
class CRNETAPP_EXPORT crUIUpdateFaceframeMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateFaceframeMethod();
	crUIUpdateFaceframeMethod(const crUIUpdateFaceframeMethod& handle);
	MethodClass(CRNetApp, UIUpdateFaceframe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_headImg;
	std::string m_lv;
	std::string m_name;
};
class CRNETAPP_EXPORT crUIUpdateHPProgressMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIUpdateHPProgressMethod();
	crUIUpdateHPProgressMethod(const crUIUpdateHPProgressMethod& handle);
	MethodClass(CRNetApp, UIUpdateHPProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_hpProgress;
	std::string m_hpValue;
};
class CRNETAPP_EXPORT crUIShowBattleTimeLimitMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUIShowBattleTimeLimitMethod();
	crUIShowBattleTimeLimitMethod(const crUIShowBattleTimeLimitMethod& handle);
	MethodClass(CRNetApp, UIShowBattleTimeLimit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_timeLimit;
};
class CRNETAPP_EXPORT crUISwapEnableMethod : public CRCore::crMethod
{//GameClient Method
public:
	crUISwapEnableMethod();
	crUISwapEnableMethod(const crUISwapEnableMethod& handle);
	MethodClass(CRNetApp, UISwapEnable)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	float m_interval;
};
class CRNETAPP_EXPORT crCommandEventMethod : public CRCore::crMethod
{//GameClient Method
public:
	crCommandEventMethod();
	crCommandEventMethod(const crCommandEventMethod& handle);
	MethodClass(CRNetApp, CommandEvent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	_crInt32 m_msg;
};
class crAddRadioMethod : public CRCore::crMethod
{
public:
	crAddRadioMethod();
	crAddRadioMethod(const crAddRadioMethod& handle);
	MethodClass(CRNetApp, AddRadio)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode* m_this;
	typedef std::vector<std::string> StrVec;
	StrVec m_strvec;
};
class crPageValueChangeMethod : public CRCore::crMethod
{
public:
	crPageValueChangeMethod();
	crPageValueChangeMethod(const crPageValueChangeMethod& handle);
	MethodClass(CRNetApp, PageValueChange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode* m_this;
	std::string m_pageSW;
};
class crGetRadioGroupValueMethod : public CRCore::crMethod
{
public:
	crGetRadioGroupValueMethod();
	crGetRadioGroupValueMethod(const crGetRadioGroupValueMethod& handle);
	MethodClass(CRNetApp, GetRadioGroupValue)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_radioGroup;
};
class crEnableRTTMethod : public CRCore::crMethod
{
public:
	crEnableRTTMethod();
	crEnableRTTMethod(const crEnableRTTMethod& handle);
	MethodClass(CRNetApp, EnableRTT)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rttname;
	bool m_enable;
};
class crTopViewRTTMethod : public CRCore::crMethod
{
public:
	crTopViewRTTMethod();
	crTopViewRTTMethod(const crTopViewRTTMethod& handle);
	MethodClass(CRNetApp, TopViewRTT)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_imageBox;
	float m_updInterval;
};
class crCameraViewRTTMethod : public CRCore::crMethod
{
public:
	crCameraViewRTTMethod();
	crCameraViewRTTMethod(const crCameraViewRTTMethod& handle);
	MethodClass(CRNetApp, CameraViewRTT)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_imageBox;
	CRCore::crVector2i m_texSize;
	float m_updInterval;
};
class crCollideMapRTTMethod : public CRCore::crMethod
{
public:
	crCollideMapRTTMethod();
	crCollideMapRTTMethod(const crCollideMapRTTMethod& handle);
	MethodClass(CRNetApp, CollideMapRTT)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_imageBox;
	float m_updInterval;
};
class crExportRTT : public CRCore::crCameraNode::DrawCallback
{
	virtual void operator () (CRCore::crCameraNode& camera);
};
class crExportRTTMethod : public CRCore::crMethod
{
public:
	crExportRTTMethod();
	crExportRTTMethod(const crExportRTTMethod& handle);
	MethodClass(CRNetApp, ExportRTT)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_rttmodeWidget;
	typedef std::vector<std::string> StrVec;
	StrVec m_rttNameVec;
};
}
#endif