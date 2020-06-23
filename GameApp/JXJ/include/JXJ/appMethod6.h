/**********************************************************************
*
*	文件:	   appMethod6.h
*
*	描述:	   
*
*	作者:	   李婧
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD6_H
#define JXJ_APPMETHOD6_H
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crRange.h>
#include <CREncapsulation/crLoader.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRText/crText.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <CRUI/crWidgetExtend.h>
namespace JXJ{
class crJXJSmelterOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSmelterOnShowMethod();
	crJXJSmelterOnShowMethod(const crJXJSmelterOnShowMethod& handle);
	MethodClass(JXJ, JXJSmelterOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_scrollbar;
};
class crJXJSmelterUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSmelterUpdateMethod();
	crJXJSmelterUpdateMethod(const crJXJSmelterUpdateMethod& handle);
	MethodClass(JXJ, JXJSmelterUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myBagList;
	std::string m_pagenum;
	std::string m_itemtype;
	std::string m_defaulticon;
	std::string m_mateicon;
	std::string m_matecountinput;
	//std::string m_copperbtn;
	//std::string m_goldbtn;
	std::string m_smeltSW;
	std::string m_addbtn;
	std::string m_deductbtn;
};
class crJXJSmelterEditCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSmelterEditCheckMethod();
	crJXJSmelterEditCheckMethod(const crJXJSmelterEditCheckMethod& handle);
	MethodClass(JXJ, JXJSmelterEditCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	std::string m_canvas;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_edit;
};
class crJXJUIPutMateOnSmelterMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIPutMateOnSmelterMethod();
	crJXJUIPutMateOnSmelterMethod(const crJXJUIPutMateOnSmelterMethod& handle);
	MethodClass(JXJ, JXJUIPutMateOnSmelter)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_itemlist;
	std::string m_inputcountcanvas;
	std::string m_inputedit;
};
class crJXJPutMateOnSmelterMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJPutMateOnSmelterMethod();
	crJXJPutMateOnSmelterMethod(const crJXJPutMateOnSmelterMethod& handle);
	MethodClass(JXJ, JXJPutMateOnSmelter)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_countinput;
	std::string m_gongfangcanvas;
	std::string m_itemtyperadio;
};
class crJXJCancelPutMateOnSmelterMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCancelPutMateOnSmelterMethod();
	crJXJCancelPutMateOnSmelterMethod(const crJXJCancelPutMateOnSmelterMethod& handle);
	MethodClass(JXJ, JXJCancelPutMateOnSmelter)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
};
class crJXJUISmeltingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISmeltingMethod();
	crJXJUISmeltingMethod(const crJXJUISmeltingMethod& handle);
	MethodClass(JXJ, JXJUISmelting)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_font;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
};
class crJXJSmeltingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSmeltingMethod();
	crJXJSmeltingMethod(const crJXJSmeltingMethod& handle);
	MethodClass(JXJ, JXJSmelting)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvSmeltingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSmeltingMethod();
	crJXJRecvSmeltingMethod(const crJXJRecvSmeltingMethod& handle);
	MethodClass(JXJ, JXJRecvSmelting)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_gongfangcanvas;
};
class crJXJAttachStoveUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAttachStoveUpdateMethod();
	crJXJAttachStoveUpdateMethod(const crJXJAttachStoveUpdateMethod& handle);
	MethodClass(JXJ, JXJAttachStoveUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_myStoreList;
	std::string m_pagenum;
	std::string m_equipicon;
	std::string m_mateicon;
	std::string m_attachbtn;
	std::string m_defaulticon;
};
class crJXJUISetInputNumMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetInputNumMethod();
	crJXJUISetInputNumMethod(const crJXJUISetInputNumMethod& handle);
	MethodClass(JXJ, JXJUISetInputNum)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	std::string m_step;
};
class crJXJAttachStoreMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAttachStoreMethod();
	crJXJAttachStoreMethod(const crJXJAttachStoreMethod& handle);
	MethodClass(JXJ, JXJAttachStore)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvAttachStoreMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAttachStoreMethod();
	crJXJRecvAttachStoreMethod(const crJXJRecvAttachStoreMethod& handle);
	MethodClass(JXJ, JXJRecvAttachStore)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_gongfangcanvas;
	std::string m_successdonghua;
};
class crJXJPutMateOnStoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJPutMateOnStoveMethod();
	crJXJPutMateOnStoveMethod(const crJXJPutMateOnStoveMethod& handle);
	MethodClass(JXJ, JXJPutMateOnStove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
	std::string m_gamelist;
};
class crJXJCancelPutMateOnStoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCancelPutMateOnStoveMethod();
	crJXJCancelPutMateOnStoveMethod(const crJXJCancelPutMateOnStoveMethod& handle);
	MethodClass(JXJ, JXJCancelPutMateOnStove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
};
class crJXJPutEquipOnStoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJPutEquipOnStoveMethod();
	crJXJPutEquipOnStoveMethod(const crJXJPutEquipOnStoveMethod& handle);
	MethodClass(JXJ, JXJPutEquipOnStove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
	std::string m_gamelist;
	std::string m_gongfangradio;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_gamelist2;
	std::string m_workshopRadio[2];
	int m_index;
};
class crJXJCancelPutEquipOnStoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCancelPutEquipOnStoveMethod();
	crJXJCancelPutEquipOnStoveMethod(const crJXJCancelPutEquipOnStoveMethod& handle);
	MethodClass(JXJ, JXJCancelPutEquipOnStove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
	std::string m_gongfangradio;
};
class crJXJClearRefineStoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJClearRefineStoveMethod();
	crJXJClearRefineStoveMethod(const crJXJClearRefineStoveMethod& handle);
	MethodClass(JXJ, JXJClearRefineStove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_gongfangcanvas;
	std::string m_gongfangradio;
};
class crJXJUIEquipRefineMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIEquipRefineMethod();
	crJXJUIEquipRefineMethod(const crJXJUIEquipRefineMethod& handle);
	MethodClass(JXJ, JXJUIEquipRefine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_font;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
};
class crJXJEquipRefineMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJEquipRefineMethod();
	crJXJEquipRefineMethod(const crJXJEquipRefineMethod& handle);
	MethodClass(JXJ, JXJEquipRefine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_lock[3];
};
class crJXJRecvEquipRefineMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvEquipRefineMethod();
	crJXJRecvEquipRefineMethod(const crJXJRecvEquipRefineMethod& handle);
	MethodClass(JXJ, JXJRecvEquipRefine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	unsigned char getRefineID(int refineindex,CRCore::crVector2i vec2,CRCore::crVector3ub refine3ub);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_gongfangcanvas;
};
class crJXJSaveEquipRefineMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSaveEquipRefineMethod();
	crJXJSaveEquipRefineMethod(const crJXJSaveEquipRefineMethod& handle);
	MethodClass(JXJ, JXJSaveEquipRefine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvSaveEquipRefineMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSaveEquipRefineMethod();
	crJXJRecvSaveEquipRefineMethod(const crJXJRecvSaveEquipRefineMethod& handle);
	MethodClass(JXJ, JXJRecvSaveEquipRefine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_gongfangcanvas;
};
class crJXJRefineFreeCountProduceMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJRefineFreeCountProduceMethod();
	crJXJRefineFreeCountProduceMethod(const crJXJRefineFreeCountProduceMethod& handle);
	MethodClass(JXJ, JXJRefineFreeCountProduce)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_delta;
	//float m_dt_record;
};
class crJXJRecvRefineFreeCountProduceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvRefineFreeCountProduceMethod();
	crJXJRecvRefineFreeCountProduceMethod(const crJXJRecvRefineFreeCountProduceMethod& handle);
	MethodClass(JXJ, JXJRecvRefineFreeCountProduce)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRefineStoveUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRefineStoveUpdateMethod();
	crJXJRefineStoveUpdateMethod(const crJXJRefineStoveUpdateMethod& handle);
	MethodClass(JXJ, JXJRefineStoveUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_equipicon;
	std::string m_equiplv[2];
	std::string m_freecountinput;
	std::string m_dataniput[2];
	std::string m_btn[4];
	std::string m_upswitch;
	std::string m_tiaomuStr[3];
	std::string m_tiaomuNewStr[3];
	std::string m_lockSwitchStr[3];
	std::string m_changegoldsw;
	std::string m_goldinput;
	std::string m_refinebtnsw;
	std::string m_refinebtn;
	std::string m_higherrefinebtn;
	std::string m_refinelockcount[3];
	std::string m_goldrefinebtn;
	int m_zbjlIcon;
};
class crJXJGongFangCloseCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGongFangCloseCheckMethod();
	crJXJGongFangCloseCheckMethod(const crJXJGongFangCloseCheckMethod& handle);
	MethodClass(JXJ, JXJGongFangCloseCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
};
//class crJXJOneKeyUpgradeBuildinglvMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJOneKeyUpgradeBuildinglvMethod();
//	crJXJOneKeyUpgradeBuildinglvMethod(const crJXJOneKeyUpgradeBuildinglvMethod& handle);
//	MethodClass(JXJ, JXJOneKeyUpgradeBuildinglv)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_upgradebtn;
//};
class crJXJOneKeyBuildingLevelUpMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOneKeyBuildingLevelUpMethod();
	crJXJOneKeyBuildingLevelUpMethod(const crJXJOneKeyBuildingLevelUpMethod& handle);
	MethodClass(JXJ, JXJOneKeyBuildingLevelUp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvOneKeyBuildingLevelUpMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvOneKeyBuildingLevelUpMethod();
	crJXJRecvOneKeyBuildingLevelUpMethod(const crJXJRecvOneKeyBuildingLevelUpMethod& handle);
	MethodClass(JXJ, JXJRecvOneKeyBuildingLevelUp)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJGoldCostHindCanvasOnshowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGoldCostHindCanvasOnshowMethod();
	crJXJGoldCostHindCanvasOnshowMethod(const crJXJGoldCostHindCanvasOnshowMethod& handle);
	MethodClass(JXJ, JXJGoldCostHindCanvasOnshow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::vector<std::string> m_checkboxArray;
	//std::string m_check[22];
};
class crJXJGoldCostHindSelectAllMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGoldCostHindSelectAllMethod();
	crJXJGoldCostHindSelectAllMethod(const crJXJGoldCostHindSelectAllMethod& handle);
	MethodClass(JXJ, JXJGoldCostHindSelectAll)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crCheckBoxWidgetNode* m_this;
	std::vector<std::string> m_checkboxArray;
};
class crJXJChangeGoldCostHindMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJChangeGoldCostHindMethod();
	crJXJChangeGoldCostHindMethod(const crJXJChangeGoldCostHindMethod& handle);
	MethodClass(JXJ, JXJChangeGoldCostHind)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	//std::string m_check[22];
	std::vector<std::string> m_checkboxArray;
};
class crJXJRecvChangeGoldCostHindMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvChangeGoldCostHindMethod();
	crJXJRecvChangeGoldCostHindMethod(const crJXJRecvChangeGoldCostHindMethod& handle);
	MethodClass(JXJ, JXJRecvChangeGoldCostHind)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuyNewBuildingMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyNewBuildingMethod();
	crJXJUIBuyNewBuildingMethod(const crJXJUIBuyNewBuildingMethod& handle);
	MethodClass(JXJ, JXJUIBuyNewBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_tips;
	std::string m_tipcontent;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
};
class crJXJBuyNewBuildingMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBuyNewBuildingMethod();
	crJXJBuyNewBuildingMethod(const crJXJBuyNewBuildingMethod& handle);
	MethodClass(JXJ, JXJBuyNewBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_check[13];
};
class crJXJRecvBuyNewBuildingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBuyNewBuildingMethod();
	crJXJRecvBuyNewBuildingMethod(const crJXJRecvBuyNewBuildingMethod& handle);
	MethodClass(JXJ, JXJRecvBuyNewBuilding)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuyLordEquipMaterialsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyLordEquipMaterialsMethod();
	crJXJUIBuyLordEquipMaterialsMethod(const crJXJUIBuyLordEquipMaterialsMethod& handle);
	MethodClass(JXJ, JXJUIBuyLordEquipMaterials)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	int m_index;
};
class crJXJBuyLordEquipMaterialsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBuyLordEquipMaterialsMethod();
	crJXJBuyLordEquipMaterialsMethod(const crJXJBuyLordEquipMaterialsMethod& handle);
	MethodClass(JXJ, JXJBuyLordEquipMaterials)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_check[13];
};
class crJXJRecvBuyLordEquipMaterialsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBuyLordEquipMaterialsMethod();
	crJXJRecvBuyLordEquipMaterialsMethod(const crJXJRecvBuyLordEquipMaterialsMethod& handle);
	MethodClass(JXJ, JXJRecvBuyLordEquipMaterials)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUpgradeVipAffairsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUpgradeVipAffairsMethod();
	crJXJUpgradeVipAffairsMethod(const crJXJUpgradeVipAffairsMethod& handle);
	MethodClass(JXJ, JXJUpgradeVipAffairs)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_index;
};
class crJXJRecvUpgradeVipAffairsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvUpgradeVipAffairsMethod();
	crJXJRecvUpgradeVipAffairsMethod(const crJXJRecvUpgradeVipAffairsMethod& handle);
	MethodClass(JXJ, JXJRecvUpgradeVipAffairs)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSetProgressBarTypeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetProgressBarTypeMethod();
	crJXJSetProgressBarTypeMethod(const crJXJSetProgressBarTypeMethod& handle);
	MethodClass(JXJ, JXJSetProgressBarType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_progressbar;
	int m_bartype;
};
class crJXJStopBatchUseItemMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJStopBatchUseItemMethod();
	crJXJStopBatchUseItemMethod(const crJXJStopBatchUseItemMethod& handle);
	MethodClass(JXJ, JXJStopBatchUseItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_index;
};
class crJXJSetCheckBoxSelectMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetCheckBoxSelectMethod();
	crJXJSetCheckBoxSelectMethod(const crJXJSetCheckBoxSelectMethod& handle);
	MethodClass(JXJ, JXJSetCheckBoxSelect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crCheckBoxWidgetNode* m_this;
	std::string m_canvas;
	std::string m_checkbox;
	int m_index;
};
class crJXJCheckHuoDongBtnMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckHuoDongBtnMethod();
	crJXJCheckHuoDongBtnMethod(const crJXJCheckHuoDongBtnMethod& handle);
	MethodClass(JXJ, JXJCheckHuoDongBtn)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_LoginEveryDay;
	std::string m_strOnlineBtn;
	std::string m_jinglianbtn;
	std::string m_guanzhibtn;
	std::string m_shuishoubtn;
	std::string m_guoZhan;
	std::string m_strLunHui;
	std::string m_onlineInput;
	std::string m_sequence[2];
	std::string m_govsequence;
	std::string m_onlineDongHua;
	std::string m_huodongBtn;
	std::string m_smallManager;
	std::string m_foundCountry;
	std::string m_7DaysLogin;
	std::string m_7DaysLoginAnimation;
	std::string m_firstRecharge;
	std::string m_yuekabtn;
	std::string m_zixuanlibaobtn;
	std::string m_chongzhihuikuibtn;
	std::string m_danbichongzhibtn;
	std::string m_xiaofeihuikuibtn;
	std::string m_zhengzhanshopbtn;
	std::string m_zhoukabtn;
	std::string m_danbichongzhitimer;
	std::string m_xiaofeihuikuitimer;
	std::string m_chongzhihuikuitimer;
	std::string m_zixuanlibaotimer;
	std::string m_pvpbtn;
	std::string m_houxuanren;
	std::string m_guanyuanxuanju;
	std::string m_toupiaojieguo;
	std::string m_panguobtn;
	std::string m_YellowNewbieBtn;
	std::string m_YellowBtn;
	std::string m_meirihaili;
	std::string m_meirihailisq;
	std::string m_xinshouBaoDian;
	std::string m_junTuanHuoDong;
	std::string m_junTuanHuoDongSequece;
	std::string m_zixuanlibaoInput;
	std::string m_zhoukaInput;
};
class crJXJUISetNoTipsSwitchMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISetNoTipsSwitchMethod();
	crJXJUISetNoTipsSwitchMethod(const crJXJUISetNoTipsSwitchMethod& handle);
	MethodClass(JXJ, JXJUISetNoTipsSwitch)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_notipsw;
	std::string m_checkbox;
};
class crJXJUIClearNoTipsParamMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIClearNoTipsParamMethod();
	crJXJUIClearNoTipsParamMethod(const crJXJUIClearNoTipsParamMethod& handle);
	MethodClass(JXJ, JXJUIClearNoTipsParam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJUIListControlLostFocusMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIListControlLostFocusMethod();
	crJXJUIListControlLostFocusMethod(const crJXJUIListControlLostFocusMethod& handle);
	MethodClass(JXJ, JXJUIListControlLostFocus)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
};
class crJXJCheckMarketLimitBuyMethod :public CRCore::crMethod
{// Client & GameServer Method
public:
	crJXJCheckMarketLimitBuyMethod();
	crJXJCheckMarketLimitBuyMethod(const crJXJCheckMarketLimitBuyMethod& handle);
	MethodClass(JXJ, JXJCheckMarketLimitBuy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_output;
};
class crJXJUIOperateInitMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIOperateInitMethod();
	crJXJUIOperateInitMethod(const crJXJUIOperateInitMethod& handle);
	MethodClass(JXJ, JXJUIOperateInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_list1;//主菜单
	std::string m_list2;//二级菜单
	std::string m_onlinelist;
};
class crJXJUIOperateUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIOperateUpdateMethod();
	crJXJUIOperateUpdateMethod(const crJXJUIOperateUpdateMethod& handle);
	MethodClass(JXJ, JXJUIOperateUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_list1;
	std::string m_list2;
	std::string m_pagesw;
	std::string m_sepagesw[4];
};
class crJXJUIOnlinePageUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIOnlinePageUpdateMethod();
	crJXJUIOnlinePageUpdateMethod(const crJXJUIOnlinePageUpdateMethod& handle);
	MethodClass(JXJ, JXJUIOnlinePageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_list;
	std::string m_title[4];
	int m_index;
};
class crJXJCheckOnlineCountMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckOnlineCountMethod();
	crJXJCheckOnlineCountMethod(const crJXJCheckOnlineCountMethod& handle);
	MethodClass(JXJ, JXJCheckOnlineCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvCheckOnlineCountMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckOnlineCountMethod();
	crJXJRecvCheckOnlineCountMethod(const crJXJRecvCheckOnlineCountMethod& handle);
	MethodClass(JXJ, JXJRecvCheckOnlineCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJTestInitMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJTestInitMethod();
//	crJXJTestInitMethod(const crJXJTestInitMethod& handle);
//	MethodClass(JXJ, JXJTestInit)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//};
//class crJXJRecvTestInitMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvTestInitMethod();
//	crJXJRecvTestInitMethod(const crJXJRecvTestInitMethod& handle);
//	MethodClass(JXJ, JXJRecvTestInit)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJCheckEditBoxInputCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckEditBoxInputCountMethod();
	crJXJCheckEditBoxInputCountMethod(const crJXJCheckEditBoxInputCountMethod& handle);
	MethodClass(JXJ, JXJCheckEditBoxInputCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	int m_inputmaxcount;
	std::string m_lastinput;
};
class crJXJRecvCheckPlayerDataMethod :public CRCore::crMethod
{//GameServer Method & Client Method
public:
	crJXJRecvCheckPlayerDataMethod();
	crJXJRecvCheckPlayerDataMethod(const crJXJRecvCheckPlayerDataMethod& handle);
	MethodClass(JXJ, JXJRecvCheckPlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCheckPlayerDataDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckPlayerDataDisposeMethod();
	crJXJCheckPlayerDataDisposeMethod(const crJXJCheckPlayerDataDisposeMethod& handle);
	MethodClass(JXJ, JXJCheckPlayerDataDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> getPlayerData(int playerid);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_taskcount;
	std::vector< std::pair<int,CRCore::ref_ptr<crPlayerDataQuery> > > m_taskVec;
};
class crJXJShowGroupMenuMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJShowGroupMenuMethod();
	crJXJShowGroupMenuMethod(const crJXJShowGroupMenuMethod& handle);
	MethodClass(JXJ, JXJShowGroupMenu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_canvas;
	std::string m_memberlist;
	std::string m_addfriend;
	std::string m_deletefriend;
	std::string m_kickmember;
};
class crJXJFormationGetLeaderPowerMethod :public CRCore::crMethod
{// GameServer Method && Client Method
public:
	crJXJFormationGetLeaderPowerMethod();
	crJXJFormationGetLeaderPowerMethod(const crJXJFormationGetLeaderPowerMethod& handle);
	MethodClass(JXJ, JXJFormationGetLeaderPower)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_output;
};
class crJXJChangePlayerDataMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJChangePlayerDataMethod();
	crJXJChangePlayerDataMethod(const crJXJChangePlayerDataMethod& handle);
	MethodClass(JXJ, JXJChangePlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_needChange;
	unsigned char m_type;
};
class crJXJCheckPlayerDataMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJCheckPlayerDataMethod();
	crJXJCheckPlayerDataMethod(const crJXJCheckPlayerDataMethod& handle);
	MethodClass(JXJ, JXJCheckPlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int* m_needdeduct;
	unsigned char m_type;
};
class crJXJRecvChangePlayerDataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvChangePlayerDataMethod();
	crJXJRecvChangePlayerDataMethod(const crJXJRecvChangePlayerDataMethod& handle);
	MethodClass(JXJ, JXJRecvChangePlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvDeductItemsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvDeductItemsMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvDeductItemsMethod();
	crJXJRecvDeductItemsMethod(const crJXJRecvDeductItemsMethod& handle);
	MethodClass(JXJ, JXJRecvDeductItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	DeductItemMap* m_deductItems;
	unsigned char *m_type ;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvDeductItemsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvDeductItemsMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvDeductItemsMethod();
	crJXJClientRecvDeductItemsMethod(const crJXJClientRecvDeductItemsMethod& handle);
	MethodClass(JXJ, JXJClientRecvDeductItems)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRollCardCountProduceMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJRollCardCountProduceMethod();
	crJXJRollCardCountProduceMethod(const crJXJRollCardCountProduceMethod& handle);
	MethodClass(JXJ, JXJRollCardCountProduce)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_delta;
	//float m_dt_record;
};
class crJXJRecvRollCardCountProduceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvRollCardCountProduceMethod();
	crJXJRecvRollCardCountProduceMethod(const crJXJRecvRollCardCountProduceMethod& handle);
	MethodClass(JXJ, JXJRecvRollCardCountProduce)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
};
class crJXJUIJiusiSetOnShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiusiSetOnShowMethod();
	crJXJUIJiusiSetOnShowMethod(const crJXJUIJiusiSetOnShowMethod& handle);
	MethodClass(JXJ, JXJUIJiusiSetOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_checkbox[3];
};
class crJXJUIChangeJiusiSetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeJiusiSetMethod();
	crJXJUIChangeJiusiSetMethod(const crJXJUIChangeJiusiSetMethod& handle);
	MethodClass(JXJ, JXJUIChangeJiusiSet)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_check[3];
};
class crJXJRecvChangeJiusiSetMethod :public CRCore::crMethod
{//GameServer Method && Client Method
public:
	crJXJRecvChangeJiusiSetMethod();
	crJXJRecvChangeJiusiSetMethod(const crJXJRecvChangeJiusiSetMethod& handle);
	MethodClass(JXJ, JXJRecvChangeJiusiSet)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTroopsTechUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTroopsTechUpdateMethod();
	crJXJTroopsTechUpdateMethod(const crJXJTroopsTechUpdateMethod& handle);
	MethodClass(JXJ, JXJTroopsTechUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_armytyperadio;
	//int m_index;
};
class crJXJUpgradeTroopsTechMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUpgradeTroopsTechMethod();
	crJXJUpgradeTroopsTechMethod(const crJXJUpgradeTroopsTechMethod& handle);
	MethodClass(JXJ, JXJUpgradeTroopsTech)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_armytyperadio;
	//int m_index;
};
class crJXJRecvUpgradeTroopsTechMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvUpgradeTroopsTechMethod();
	crJXJRecvUpgradeTroopsTechMethod(const crJXJRecvUpgradeTroopsTechMethod& handle);
	MethodClass(JXJ, JXJRecvUpgradeTroopsTech)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTroopsTechTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTroopsTechTipsMethod();
	crJXJTroopsTechTipsMethod(const crJXJTroopsTechTipsMethod& handle);
	MethodClass(JXJ, JXJTroopsTechTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	bool setInfo();
	//std::string getMagicAttrTipsStr(int magicstartid,const CREncapsulation::crTableIO::StrVec &magicrecord);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tipscanvas;
	std::string m_typeradio;
	std::string m_icon;
	std::string m_input[2];
	std::string m_infocanvas;
	CRCore::crVector4 m_colorred;
	CRCore::crVector4 m_colorgreen;
	std::string m_infobk;
	std::string m_infodi;
};
class crJXJJiusiCritTimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJJiusiCritTimerMethod();
	crJXJJiusiCritTimerMethod(const crJXJJiusiCritTimerMethod& handle);
	MethodClass(JXJ, JXJJiusiCritTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_critsw;
	std::string m_sequence[4];
	int m_lastpunishtime;
	int m_lasteffect;
};
class crJXJAddBlackNameMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAddBlackNameMethod();
	crJXJAddBlackNameMethod(const crJXJAddBlackNameMethod& handle);
	MethodClass(JXJ, JXJAddBlackName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_armytyperadio;
	//int m_index;
};
class crJXJRecvAddBlackNameMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvAddBlackNameMethod();
	crJXJRecvAddBlackNameMethod(const crJXJRecvAddBlackNameMethod& handle);
	MethodClass(JXJ, JXJRecvAddBlackName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJDeleteBlackNameMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJDeleteBlackNameMethod();
	crJXJDeleteBlackNameMethod(const crJXJDeleteBlackNameMethod& handle);
	MethodClass(JXJ, JXJDeleteBlackName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_armytyperadio;
	std::string m_blacklist;
	//int m_index;
};
class crJXJRecvDeleteBlackNameMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvDeleteBlackNameMethod();
	crJXJRecvDeleteBlackNameMethod(const crJXJRecvDeleteBlackNameMethod& handle);
	MethodClass(JXJ, JXJRecvDeleteBlackName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
}; 
class crJXJUIUpdateTechLvMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateTechLvMethod();
	crJXJUIUpdateTechLvMethod(const crJXJUIUpdateTechLvMethod& handle);
	MethodClass(JXJ, JXJUIUpdateTechLv)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_index;
};
class crJXJRecvUpdateTechLvMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvUpdateTechLvMethod();
	crJXJRecvUpdateTechLvMethod(const crJXJRecvUpdateTechLvMethod& handle);
	MethodClass(JXJ, JXJRecvUpdateTechLv)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUpdateTechCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateTechCanvasMethod();
	crJXJUpdateTechCanvasMethod(const crJXJUpdateTechCanvasMethod& handle);
	MethodClass(JXJ, JXJUpdateTechCanvas)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_techtitleinput[7];
	std::string m_techdetailinput[7];
	std::string m_techupgradebtn[7];
	std::string m_techCDinput[7];
	std::string m_techCDclearbtn[7];
	std::string m_techstatesw[7];
	std::string m_techCDsw[7];
	std::string m_techpageinput;
	std::string m_techicon[7];
	int m_lastclienttime[7];
	int m_lastservertime[7];
};
class crJXJTechDevelopUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJTechDevelopUpdateMethod();
	crJXJTechDevelopUpdateMethod(const crJXJTechDevelopUpdateMethod& handle);
	MethodClass(JXJ, JXJTechDevelopUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_delta;
	//float m_dt_record;
};
class crJXJRecvTechDevelopUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvTechDevelopUpdateMethod();
	crJXJRecvTechDevelopUpdateMethod(const crJXJRecvTechDevelopUpdateMethod& handle);
	MethodClass(JXJ, JXJRecvTechDevelopUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIClearTechDevelopTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIClearTechDevelopTimeMethod();
	crJXJUIClearTechDevelopTimeMethod(const crJXJUIClearTechDevelopTimeMethod& handle);
	MethodClass(JXJ, JXJUIClearTechDevelopTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	int m_index;
};
class crJXJClearTechDevelopTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClearTechDevelopTimeMethod();
	crJXJClearTechDevelopTimeMethod(const crJXJClearTechDevelopTimeMethod& handle);
	MethodClass(JXJ, JXJClearTechDevelopTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvClearTechDevelopTimeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvClearTechDevelopTimeMethod();
	crJXJRecvClearTechDevelopTimeMethod(const crJXJRecvClearTechDevelopTimeMethod& handle);
	MethodClass(JXJ, JXJRecvClearTechDevelopTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSetMaxArmyCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetMaxArmyCountMethod();
	crJXJSetMaxArmyCountMethod(const crJXJSetMaxArmyCountMethod& handle);
	MethodClass(JXJ, JXJSetMaxArmyCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationcanvas;
	std::string m_countinput[3];
};
class crJXJSetArmyPowerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetArmyPowerMethod();
	crJXJSetArmyPowerMethod(const crJXJSetArmyPowerMethod& handle);
	MethodClass(JXJ, JXJSetArmyPower)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationcanvas;
	std::string m_powerinput[3];
	std::string m_totalpowerinput;
};
class crJXJChangeSavedFormationVecMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeSavedFormationVecMethod();
	crJXJChangeSavedFormationVecMethod(const crJXJChangeSavedFormationVecMethod& handle);
	MethodClass(JXJ, JXJChangeSavedFormationVec)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationcanvas;
	int m_index;
};
class crJXJResetTempFormationVecMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetTempFormationVecMethod();
	crJXJResetTempFormationVecMethod(const crJXJResetTempFormationVecMethod& handle);
	MethodClass(JXJ, JXJResetTempFormationVec)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationcanvas;
	std::string m_inputsw[4];
	std::string m_savedradio;
};
class crJXJSetTempFormationVecMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetTempFormationVecMethod();
	crJXJSetTempFormationVecMethod(const crJXJSetTempFormationVecMethod& handle);
	MethodClass(JXJ, JXJSetTempFormationVec)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationcanvas;
	std::string m_inputradio;
};
class crJXJUISetTempFormationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetTempFormationMethod();
	crJXJUISetTempFormationMethod(const crJXJUISetTempFormationMethod& handle);
	MethodClass(JXJ, JXJUISetTempFormation)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_nameedit[4];
	std::string m_editbtn[4];
	std::string m_addbtn[4];
	std::string m_backboard[4];
	std::string m_nameinput[4];
	std::string m_nameradio[4];
};
class crJXJUIBuySavedFormationPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuySavedFormationPageMethod();
	crJXJUIBuySavedFormationPageMethod(const crJXJUIBuySavedFormationPageMethod& handle);
	MethodClass(JXJ, JXJUIBuySavedFormationPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_font;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
};
class crJXJBuySavedFormationPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuySavedFormationPageMethod();
	crJXJBuySavedFormationPageMethod(const crJXJBuySavedFormationPageMethod& handle);
	MethodClass(JXJ, JXJBuySavedFormationPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvBuySavedFormationPageMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvBuySavedFormationPageMethod();
	crJXJRecvBuySavedFormationPageMethod(const crJXJRecvBuySavedFormationPageMethod& handle);
	MethodClass(JXJ, JXJRecvBuySavedFormationPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIChangeFormationEditMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeFormationEditMethod();
	crJXJUIChangeFormationEditMethod(const crJXJUIChangeFormationEditMethod& handle);
	MethodClass(JXJ, JXJUIChangeFormationEdit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_editinput;
	std::string m_inputsw[4];
	std::string m_staticinput;
	int m_index;
};
class crJXJUIFormationEditLostFocusMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFormationEditLostFocusMethod();
	crJXJUIFormationEditLostFocusMethod(const crJXJUIFormationEditLostFocusMethod& handle);
	MethodClass(JXJ, JXJUIFormationEditLostFocus)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_editinput;
	std::string m_inputsw[4];
	std::string m_staticinput;
	int m_index;
};
class crJXJUISetAddFriendTypeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetAddFriendTypeMethod();
	crJXJUISetAddFriendTypeMethod(const crJXJUISetAddFriendTypeMethod& handle);
	MethodClass(JXJ, JXJUISetAddFriendType)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJCalculatePowerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCalculatePowerMethod();
	crJXJCalculatePowerMethod(const crJXJCalculatePowerMethod& handle);
	MethodClass(JXJ, JXJCalculatePower)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::crVector4i m_troopinfo;
	CRCore::crVector2i *m_output;
	std::string m_strCanvas;
};
class crJXJCheckEditMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckEditMethod();
	crJXJCheckEditMethod(const crJXJCheckEditMethod& handle);
	MethodClass(JXJ, JXJCheckEdit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode *m_this;
	int m_index;
	std::string m_laststring;
};
class crJXJCheckRemainPlayerCountMethod : public CRCore::crMethod
{//Client Method，获取玩家留存率
public:
	crJXJCheckRemainPlayerCountMethod();
	crJXJCheckRemainPlayerCountMethod(const crJXJCheckRemainPlayerCountMethod& handle);
	MethodClass(JXJ, JXJCheckRemainPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvCheckRemainPlayerCountMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckRemainPlayerCountMethod();
	crJXJRecvCheckRemainPlayerCountMethod(const crJXJRecvCheckRemainPlayerCountMethod& handle);
	MethodClass(JXJ, JXJRecvCheckRemainPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCheckLostPlayerCountMethod : public CRCore::crMethod
{//Client Method,获取玩家流失率
public:
	crJXJCheckLostPlayerCountMethod();
	crJXJCheckLostPlayerCountMethod(const crJXJCheckLostPlayerCountMethod& handle);
	MethodClass(JXJ, JXJCheckLostPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvCheckLostPlayerCountMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckLostPlayerCountMethod();
	crJXJRecvCheckLostPlayerCountMethod(const crJXJRecvCheckLostPlayerCountMethod& handle);
	MethodClass(JXJ, JXJRecvCheckLostPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCheckTatolPlayerCountMethod : public CRCore::crMethod
{//Client Method,获取玩家总注册 总创建
public:
	crJXJCheckTatolPlayerCountMethod();
	crJXJCheckTatolPlayerCountMethod(const crJXJCheckTatolPlayerCountMethod& handle);
	MethodClass(JXJ, JXJCheckTatolPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvCheckTatolPlayerCountMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckTatolPlayerCountMethod();
	crJXJRecvCheckTatolPlayerCountMethod(const crJXJRecvCheckTatolPlayerCountMethod& handle);
	MethodClass(JXJ, JXJRecvCheckTatolPlayerCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSkipNewHandIntroMethod : public CRCore::crMethod
{//Client Method,获取玩家流失率
public:
	crJXJSkipNewHandIntroMethod();
	crJXJSkipNewHandIntroMethod(const crJXJSkipNewHandIntroMethod& handle);
	MethodClass(JXJ, JXJSkipNewHandIntro)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvSkipNewHandIntroMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvSkipNewHandIntroMethod();
	crJXJRecvSkipNewHandIntroMethod(const crJXJRecvSkipNewHandIntroMethod& handle);
	MethodClass(JXJ, JXJRecvSkipNewHandIntro)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJGetShiliPeoPleMapMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJGetShiliPeoPleMapMethod();
//	crJXJGetShiliPeoPleMapMethod(const crJXJGetShiliPeoPleMapMethod& handle);
//	MethodClass(JXJ, JXJGetShiliPeoPleMap)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//};
//class crJXJRecvGetShiliPeoPleMapMethod :public CRCore::crMethod
//{//GameServer Method 会导致数据包越界
//public:
//	crJXJRecvGetShiliPeoPleMapMethod();
//	crJXJRecvGetShiliPeoPleMapMethod(const crJXJRecvGetShiliPeoPleMapMethod& handle);
//	MethodClass(JXJ, JXJRecvGetShiliPeoPleMap)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_gmcanvas;
//};
class crJXJCheckMoneyRecordMethod : public CRCore::crMethod
{//Client Method,获取玩家充值记录
public:
	crJXJCheckMoneyRecordMethod();
	crJXJCheckMoneyRecordMethod(const crJXJCheckMoneyRecordMethod& handle);
	MethodClass(JXJ, JXJCheckMoneyRecord)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvCheckMoneyRecordMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckMoneyRecordMethod();
	crJXJRecvCheckMoneyRecordMethod(const crJXJRecvCheckMoneyRecordMethod& handle);
	MethodClass(JXJ, JXJRecvCheckMoneyRecord)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTechTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTechTipsMethod();
	crJXJTechTipsMethod(const crJXJTechTipsMethod& handle);
	MethodClass(JXJ, JXJTechTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tipscanvas;
	std::string m_icon;
	std::string m_input[2];
};
class crJXJCheckJunTuanBtnMethod :public CRCore::crMethod
{
	// client
public:
	crJXJCheckJunTuanBtnMethod ();
	crJXJCheckJunTuanBtnMethod (const crJXJCheckJunTuanBtnMethod & handle);
	MethodClass(JXJ, JXJCheckJunTuanBtn)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_dongHua1;
	std::string m_juntunfbtip;
};
class crJXJCheckKejiBtnMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckKejiBtnMethod();
	crJXJCheckKejiBtnMethod(const crJXJCheckKejiBtnMethod& handle);
	MethodClass(JXJ, JXJCheckKejiBtn)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_kejisw;
	std::string m_sequence;
};
class crJXJCheckChengkaDonghuaMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckChengkaDonghuaMethod();
	crJXJCheckChengkaDonghuaMethod(const crJXJCheckChengkaDonghuaMethod& handle);
	MethodClass(JXJ, JXJCheckChengkaDonghua)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_donghua[5];
	std::string m_successcanvas;
};
class crJXJResetMapPositionMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetMapPositionMethod();
	crJXJResetMapPositionMethod(const crJXJResetMapPositionMethod& handle);
	MethodClass(JXJ, JXJResetMapPosition)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJFirstGameVideoSettingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFirstGameVideoSettingMethod();
	crJXJFirstGameVideoSettingMethod(const crJXJFirstGameVideoSettingMethod& handle);
	MethodClass(JXJ, JXJFirstGameVideoSetting)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJGMGetPlayerDataMethod :public CRCore::crMethod
{//GameServer Method & Client Method
public:
	crJXJGMGetPlayerDataMethod();
	crJXJGMGetPlayerDataMethod(const crJXJGMGetPlayerDataMethod& handle);
	MethodClass(JXJ, JXJGMGetPlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	CRNetApp::crGameServerPlayerData* m_playerdata;
	int  m_playerid;
};
class crJXJGMChangePlayerDataMethod :public CRCore::crMethod
{//GameServer Method & Client Method
public:
	crJXJGMChangePlayerDataMethod();
	crJXJGMChangePlayerDataMethod(const crJXJGMChangePlayerDataMethod& handle);
	MethodClass(JXJ, JXJGMChangePlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvGMChangePlayerDataMethod :public CRCore::crMethod
{//GameServer Method & Client Method
public:
	crJXJRecvGMChangePlayerDataMethod();
	crJXJRecvGMChangePlayerDataMethod(const crJXJRecvGMChangePlayerDataMethod& handle);
	MethodClass(JXJ, JXJRecvGMChangePlayerData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void ChangeResource(int changetype,int changecount);
	void ChangeItems(int changeid,int changecount);
	void ChangeTroops(int changeid,int changecount);
	void ChangeGold(unsigned char goldtype,int changecount);//充值或非充值
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJResetFormationPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetFormationPageMethod();
	crJXJResetFormationPageMethod(const crJXJResetFormationPageMethod& handle);
	MethodClass(JXJ, JXJResetFormationPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJResetTechNewBtnFlagMethod : public CRCore::crMethod
{//Client Method,重置科技图标闪烁
public:
	crJXJResetTechNewBtnFlagMethod();
	crJXJResetTechNewBtnFlagMethod(const crJXJResetTechNewBtnFlagMethod& handle);
	MethodClass(JXJ, JXJResetTechNewBtnFlag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvResetTechNewBtnFlagMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvResetTechNewBtnFlagMethod();
	crJXJRecvResetTechNewBtnFlagMethod(const crJXJRecvResetTechNewBtnFlagMethod& handle);
	MethodClass(JXJ, JXJRecvResetTechNewBtnFlag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSetVipCanvasPageNumMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetVipCanvasPageNumMethod();
	crJXJSetVipCanvasPageNumMethod(const crJXJSetVipCanvasPageNumMethod& handle);
	MethodClass(JXJ, JXJSetVipCanvasPageNum)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJSearchBuildingLvCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchBuildingLvCompleteCheckMethod();
	crJXJSearchBuildingLvCompleteCheckMethod(const crJXJSearchBuildingLvCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchBuildingLvCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_buildingid;//
};
class crJXJSearchBuildingLvMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchBuildingLvMethod();
	crJXJSearchBuildingLvMethod(const crJXJSearchBuildingLvMethod& handle);
	MethodClass(JXJ, JXJSearchBuildingLv)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_buildingid;//
};
class crJXJSearchResourceCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchResourceCompleteCheckMethod();
	crJXJSearchResourceCompleteCheckMethod(const crJXJSearchResourceCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchResourceCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_resourceid;//
};
class crJXJSearchResourceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchResourceMethod();
	crJXJSearchResourceMethod(const crJXJSearchResourceMethod& handle);
	MethodClass(JXJ, JXJSearchResource)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_resourceid;//
};
class crJXJSearchGovPostCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchGovPostCompleteCheckMethod();
	crJXJSearchGovPostCompleteCheckMethod(const crJXJSearchGovPostCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchGovPostCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJSearchGovPostMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchGovPostMethod();
	crJXJSearchGovPostMethod(const crJXJSearchGovPostMethod& handle);
	MethodClass(JXJ, JXJSearchGovPost)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
class crJXJSearchChapterPassCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchChapterPassCompleteCheckMethod();
	crJXJSearchChapterPassCompleteCheckMethod(const crJXJSearchChapterPassCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchChapterPassCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJSearchChapterPassMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchChapterPassMethod();
	crJXJSearchChapterPassMethod(const crJXJSearchChapterPassMethod& handle);
	MethodClass(JXJ, JXJSearchChapterPass)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
class crJXJSearchFubenStarCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method 成就关于副本星级
public:
	crJXJSearchFubenStarCompleteCheckMethod();
	crJXJSearchFubenStarCompleteCheckMethod(const crJXJSearchFubenStarCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchFubenStarCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJSearchFubenStarMethod :public CRCore::crMethod
{//Client Method 成就关于副本星级
public:
	crJXJSearchFubenStarMethod();
	crJXJSearchFubenStarMethod(const crJXJSearchFubenStarMethod& handle);
	MethodClass(JXJ, JXJSearchFubenStar)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
class crJXJSearchEquipColorCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchEquipColorCompleteCheckMethod();
	crJXJSearchEquipColorCompleteCheckMethod(const crJXJSearchEquipColorCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchEquipColorCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_itemtype;
	int m_equipcolor;
};
class crJXJSearchEquipColorMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchEquipColorMethod();
	crJXJSearchEquipColorMethod(const crJXJSearchEquipColorMethod& handle);
	MethodClass(JXJ, JXJSearchEquipColor)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_itemtype;
	int m_equipcolor;
};
class crJXJSearchItemCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchItemCompleteCheckMethod();
	crJXJSearchItemCompleteCheckMethod(const crJXJSearchItemCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchItemCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	std::vector<int> m_itemidvec;
	std::vector<int> m_itemcountvec;
};
class crJXJSearchItemColorMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchItemColorMethod();
	crJXJSearchItemColorMethod(const crJXJSearchItemColorMethod& handle);
	MethodClass(JXJ, JXJSearchItemColor)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	std::vector<int> m_itemidvec;
	std::vector<int> m_itemcountvec;
};
class crJXJUIFubenRushQuickEndMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFubenRushQuickEndMethod();
	crJXJUIFubenRushQuickEndMethod(const crJXJUIFubenRushQuickEndMethod& handle);
	MethodClass(JXJ, JXJUIFubenRushQuickEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
};
class crJXJFubenRushQuickEndMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJFubenRushQuickEndMethod();
	crJXJFubenRushQuickEndMethod(const crJXJFubenRushQuickEndMethod& handle);
	MethodClass(JXJ, JXJFubenRushQuickEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvFubenRushQuickEndMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvFubenRushQuickEndMethod();
	crJXJRecvFubenRushQuickEndMethod(const crJXJRecvFubenRushQuickEndMethod& handle);
	MethodClass(JXJ, JXJRecvFubenRushQuickEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQueryLoseCountryMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQueryLoseCountryMethod();
	crJXJQueryLoseCountryMethod(const crJXJQueryLoseCountryMethod& handle);
	MethodClass(JXJ, JXJQueryLoseCountry)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
//class crJXJRecvQueryLoseCountryMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvQueryLoseCountryMethod();
//	crJXJRecvQueryLoseCountryMethod(const crJXJRecvQueryLoseCountryMethod& handle);
//	MethodClass(JXJ, JXJRecvQueryLoseCountry)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJSearchMyCountryCityCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method 成就关于国战城池归属
public:
	crJXJSearchMyCountryCityCompleteCheckMethod();
	crJXJSearchMyCountryCityCompleteCheckMethod(const crJXJSearchMyCountryCityCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchMyCountryCityCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJSearchMyCountryCityMethod :public CRCore::crMethod
{//Client Method 成就关于国战城池归属
public:
	crJXJSearchMyCountryCityMethod();
	crJXJSearchMyCountryCityMethod(const crJXJSearchMyCountryCityMethod& handle);
	MethodClass(JXJ, JXJSearchMyCountryCity)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};
class crJXJLoadingImageMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingImageMethod();
	crJXJLoadingImageMethod(const crJXJLoadingImageMethod& handle);
	MethodClass(JXJ, JXJLoadingImage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crImageBoxElement* m_element;
	float m_updateInterval;
	float m_dt;
	float m_timer;
	int m_currentid;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_ImgTab;
};
class crJXJLoadingImage1Method : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingImage1Method();
	crJXJLoadingImage1Method(const crJXJLoadingImage1Method& handle);
	MethodClass(JXJ, JXJLoadingImage1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_imgWidget;
	float m_updateInterval;
	float *m_dt;
	float m_timer;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_ImgTab;
};
class crJXJSetLoadingTextMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJSetLoadingTextMethod();
	crJXJSetLoadingTextMethod(const crJXJSetLoadingTextMethod& handle);
	MethodClass(JXJ, JXJSetLoadingText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crStaticTextBoxElement* m_element;
};
class crJXJUIJiusiAutoRollCardMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIJiusiAutoRollCardMethod();
	crJXJUIJiusiAutoRollCardMethod(const crJXJUIJiusiAutoRollCardMethod& handle);
	MethodClass(JXJ, JXJUIJiusiAutoRollCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_checkbox[3];
};
class crJXJUIAutoUseBagItemMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAutoUseBagItemMethod();
	crJXJUIAutoUseBagItemMethod(const crJXJUIAutoUseBagItemMethod& handle);
	MethodClass(JXJ, JXJUIAutoUseBagItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_checkbox[3];
};
/////////////////////////////////////////
//
//crJXJUIAchievementPromptUIShowMethod
//成就弹出框[UI]
//
/////////////////////////////////////////
class crJXJUIAchievementPromptUIShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIAchievementPromptUIShowMethod();
	crJXJUIAchievementPromptUIShowMethod(const crJXJUIAchievementPromptUIShowMethod& handle);
	MethodClass(JXJ, JXJUIAchievementPromptUIShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_tipsCanvas;
	std::string m_icon;
	std::string m_input1;
	std::string m_input2;
	std::string m_sequence;
	std::string m_backboard;
	std::string m_sw;
};
class crJXJUIRotateClockMethod:public CRCore::crMethod
{// client
public:
	crJXJUIRotateClockMethod();
	crJXJUIRotateClockMethod(const crJXJUIRotateClockMethod& handle);
	MethodClass(JXJ, JXJUIRotateClock)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rotatestr;
	std::string m_canvas;
	//std::string m_sw;
	std::string m_timeinput;
	int m_unrealsec;
	float m_lastangle;
};
class crJXJAutoBattleSettingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJAutoBattleSettingMethod();
	crJXJAutoBattleSettingMethod(const crJXJAutoBattleSettingMethod& handle);
	MethodClass(JXJ, JXJAutoBattleSetting)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_interval;
	float m_time;
	int m_minframe;
	std::string m_strMsgCanvas;
	std::string m_strMsg;
};
}
#endif
