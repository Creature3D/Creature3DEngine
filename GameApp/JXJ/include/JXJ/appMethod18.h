/**********************************************************************
*
*	文件:	   appMethod18.h
*
*	描述:
*
*	作者:	   吴财华
*
*
**********************************************************************/
#ifndef JXJ_APPMETHOD18_H
#define JXJ_APPMETHOD18_H
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
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <JXJ/appData3.h>
#include <JXJ/appMethod4.h>
namespace JXJ {
class crJXJLoginCrossGameMethod : public CRCore::crMethod
{//GameClient Method 登陆到跨服游戏服务器
public:
	crJXJLoginCrossGameMethod();
	crJXJLoginCrossGameMethod(const crJXJLoginCrossGameMethod& handle);
	MethodClass(JXJ, JXJLoginCrossGame)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_callback;
	std::string m_servername;
	std::string m_password;
};
class crJXJUISuperEquipCanvasUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJUISuperEquipCanvasUpdateMethod();
	crJXJUISuperEquipCanvasUpdateMethod(const crJXJUISuperEquipCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJUISuperEquipCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_equipRadioGroup;
	std::string m_equipPageRadioGroup;
	std::string m_equipPageSW;
	std::string m_equipImage;
	std::string m_equipLevelText;
	std::string m_equipQualityText;
	std::string m_equipLvupInfoText;
	std::string m_equipAttrInfoText;
	std::string m_expProgress;
	std::string m_expText;
	std::string m_expBuyButton;
	std::string m_jiangkaImage[5];
	std::string m_jiangkaText[5];
	std::string m_activeButton;
	std::string m_scroll;
	std::string m_lunhuiDistillButton;
	std::string m_lunhuiEquipParamText;
	std::string m_lunhuiEquipExpText[SuperEquipCount];
	std::string m_equipZhulinglvText[SuperEquipZhulingCount];
	std::string m_equipZhulinglvAddButton[SuperEquipZhulingCount];
	std::string m_equipZhulingPointText;
	std::string m_equipZhulingResetButton;
	std::string m_activeSW;
};
class crJXJSuperEquipZhulingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJSuperEquipZhulingMethod();
	crJXJSuperEquipZhulingMethod(const crJXJSuperEquipZhulingMethod& handle);
	MethodClass(JXJ, JXJSuperEquipZhuling)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_pername;
};
/////////////////////////////////////////
//
//crJXJSuperEquipZhulingTipMethod
//
/////////////////////////////////////////
class crJXJSuperEquipZhulingTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSuperEquipZhulingTipMethod();
	crJXJSuperEquipZhulingTipMethod(const crJXJSuperEquipZhulingTipMethod& handle);
	MethodClass(JXJ, JXJSuperEquipZhulingTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
	std::string m_canvasName;
	std::string m_pername;
};
class crJXJRecvSuperEquipZhulingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSuperEquipZhulingMethod();
	crJXJRecvSuperEquipZhulingMethod(const crJXJRecvSuperEquipZhulingMethod& handle);
	MethodClass(JXJ, JXJRecvSuperEquipZhuling)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJResetZhulingMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJResetZhulingMsgBoxMethod();
	crJXJResetZhulingMsgBoxMethod(const crJXJResetZhulingMsgBoxMethod& handle);
	MethodClass(JXJ, JXJResetZhulingMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_msgtext;
};
class crJXJResetZhulingMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJResetZhulingMethod();
	crJXJResetZhulingMethod(const crJXJResetZhulingMethod& handle);
	MethodClass(JXJ, JXJResetZhuling)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvResetZhulingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvResetZhulingMethod();
	crJXJRecvResetZhulingMethod(const crJXJRecvResetZhulingMethod& handle);
	MethodClass(JXJ, JXJRecvResetZhuling)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerUseExpPelletMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseExpPelletMethod();
	crJXJServerUseExpPelletMethod(const crJXJServerUseExpPelletMethod& handle);
	MethodClass(JXJ, JXJServerUseExpPellet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
};
class crJXJRecvUseExpPelletMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseExpPelletMethod();
	crJXJRecvUseExpPelletMethod(const crJXJRecvUseExpPelletMethod& handle);
	MethodClass(JXJ, JXJRecvUseExpPellet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUseExpPelletMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJUseExpPelletMethod();
	crJXJUseExpPelletMethod(const crJXJUseExpPelletMethod& handle);
	MethodClass(JXJ, JXJUseExpPellet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_equipRadioGroup;
	std::string m_expPelletCanvas;
};
class crJXJUIExpPelletCanvasUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJUIExpPelletCanvasUpdateMethod();
	crJXJUIExpPelletCanvasUpdateMethod(const crJXJUIExpPelletCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJUIExpPelletCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_buyButton[3];
	std::string m_priceText[3];
	std::string m_imageBox[3];
	std::string m_countText[3];
};
class crJXJBuyExpPelletMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJBuyExpPelletMsgBoxMethod();
	crJXJBuyExpPelletMsgBoxMethod(const crJXJBuyExpPelletMsgBoxMethod& handle);
	MethodClass(JXJ, JXJBuyExpPelletMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_pername;
	std::string m_msgbox;
	std::string m_msgtext;
};
class crJXJBuyExpPelletMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJBuyExpPelletMethod();
	crJXJBuyExpPelletMethod(const crJXJBuyExpPelletMethod& handle);
	MethodClass(JXJ, JXJBuyExpPellet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_pername;
};
class crJXJRecvBuyExpPelletMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyExpPelletMethod();
	crJXJRecvBuyExpPelletMethod(const crJXJRecvBuyExpPelletMethod& handle);
	MethodClass(JXJ, JXJRecvBuyExpPellet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJExpPelletTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJExpPelletTipsMethod();
	crJXJExpPelletTipsMethod(const crJXJExpPelletTipsMethod& handle);
	MethodClass(JXJ, JXJExpPelletTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	//std::string m_info1;
	std::string m_info2;
	//std::string m_itemlv;
	std::string m_itemicon;
	//std::string m_itemequipon;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	std::string m_pername;
};
class crJXJActiveSuperEquipMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJActiveSuperEquipMethod();
	crJXJActiveSuperEquipMethod(const crJXJActiveSuperEquipMethod& handle);
	MethodClass(JXJ, JXJActiveSuperEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
};
class crJXJRecvActiveSuperEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvActiveSuperEquipMethod();
	crJXJRecvActiveSuperEquipMethod(const crJXJRecvActiveSuperEquipMethod& handle);
	MethodClass(JXJ, JXJRecvActiveSuperEquip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvDistillSuperEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvDistillSuperEquipMethod();
	crJXJRecvDistillSuperEquipMethod(const crJXJRecvDistillSuperEquipMethod& handle);
	MethodClass(JXJ, JXJRecvDistillSuperEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJDistillSuperEquipMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJDistillSuperEquipMethod();
	crJXJDistillSuperEquipMethod(const crJXJDistillSuperEquipMethod& handle);
	MethodClass(JXJ, JXJDistillSuperEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJGainSuperEquipLvupPointMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGainSuperEquipLvupPointMethod();
	crJXJGainSuperEquipLvupPointMethod(const crJXJGainSuperEquipLvupPointMethod& handle);
	MethodClass(JXJ, JXJGainSuperEquipLvupPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	unsigned char m_lvuppoint;
};
class crJXJRecvGainSuperEquipLvupPointMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvGainSuperEquipLvupPointMethod();
	crJXJRecvGainSuperEquipLvupPointMethod(const crJXJRecvGainSuperEquipLvupPointMethod& handle);
	MethodClass(JXJ, JXJRecvGainSuperEquipLvupPoint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIClearTextMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJUIClearTextMethod();
	crJXJUIClearTextMethod(const crJXJUIClearTextMethod& handle);
	MethodClass(JXJ, JXJUIClearText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::vector<std::string>m_textVec;
};
/////////////////////////////////////////
//
//crJXJRecvSuperEquipCoinMethod
//
/////////////////////////////////////////
class crJXJRecvSuperEquipCoinMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvSuperEquipCoinMethod();
	crJXJRecvSuperEquipCoinMethod(const crJXJRecvSuperEquipCoinMethod& handle);
	MethodClass(JXJ, JXJRecvSuperEquipCoin)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_coin;
};
/////////////////////////////////////////
//
//crJXJClientRecvSuperEquipCoinMethod
//
/////////////////////////////////////////
class crJXJClientRecvSuperEquipCoinMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvSuperEquipCoinMethod();
	crJXJClientRecvSuperEquipCoinMethod(const crJXJClientRecvSuperEquipCoinMethod& handle);
	MethodClass(JXJ, JXJClientRecvSuperEquipCoin)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
/////////////////////////////////////////
//
//crJXJSuperEquipShopCanvasUpdateMethod
//
/////////////////////////////////////////
class crJXJSuperEquipShopCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSuperEquipShopCanvasUpdateMethod();
	crJXJSuperEquipShopCanvasUpdateMethod(const crJXJSuperEquipShopCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJSuperEquipShopCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	void init();
	CRCore::crCanvasNode *m_this;
	std::string m_priceInput[12];
	std::string m_imageBox[12];
	std::string m_countInput[12];
	std::string m_goumaiBtn[12];
	std::string m_scroll;
	std::string m_coin;
	CRCore::ref_ptr<CRUI::crImageBoxWidgetNode > m_imageBoxWidget[12];
	CRCore::ref_ptr<CRUI::crStaticTextWidgetNode > m_priceInputWidget[12];
	CRCore::ref_ptr<CRUI::crStaticTextWidgetNode > m_countInputWidget[12];
	CRCore::ref_ptr<CRUI::crButtonWidgetNode> m_gouMaiBtnWidget[12];
	CRCore::ref_ptr<CRUI::crStaticTextWidgetNode > m_coinTextWidget;
	CRCore::ref_ptr<CRUI::crScrollBarWidgetNode > m_scrollWidget;
	bool m_init;
};
/////////////////////////////////////////
//
//crJXJSetSuperEquipShopRadioEnableMethod
//
/////////////////////////////////////////
class crJXJSetSuperEquipShopRadioEnableMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetSuperEquipShopRadioEnableMethod();
	crJXJSetSuperEquipShopRadioEnableMethod(const crJXJSetSuperEquipShopRadioEnableMethod & handle);
	MethodClass(JXJ, JXJSetSuperEquipShopRadioEnable)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	void init();
	CRCore::crCanvasNode *m_this;
	std::string m_radio;
	CRCore::ref_ptr<CRUI::crRadioWidgetNode > m_radioWidget;
	bool m_init;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSuperEquipShopBuyMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSuperEquipShopBuyMethod :public CRCore::crMethod
{//Client 
public:
	crJXJSuperEquipShopBuyMethod();
	crJXJSuperEquipShopBuyMethod(const crJXJSuperEquipShopBuyMethod& handle);
	MethodClass(JXJ, JXJSuperEquipShopBuy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////
//
//crJXJRecvSuperEquipShopBuyMethod
//
//////////////////////////////////////////
class crJXJRecvSuperEquipShopBuyMethod :public CRCore::crMethod
{//Game Method & Client Method
public:
	crJXJRecvSuperEquipShopBuyMethod();
	crJXJRecvSuperEquipShopBuyMethod(const crJXJRecvSuperEquipShopBuyMethod &handle);
	MethodClass(JXJ, JXJRecvSuperEquipShopBuy)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
//////////////////////////////////////////
//
//crJXJAddTaskInfoMethod
//
//////////////////////////////////////////
class crJXJAddTaskInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAddTaskInfoMethod();
	crJXJAddTaskInfoMethod(const crJXJAddTaskInfoMethod &handle);
	MethodClass(JXJ, JXJAddTaskInfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvasName;
	std::vector<int> m_taskVec;
	int m_texid;
};
class crJXJCheckTaskValueChangeMethod :public CRCore::crMethod
{//Client Method
 //单个值累计变化
public:
	crJXJCheckTaskValueChangeMethod();
	crJXJCheckTaskValueChangeMethod(const crJXJCheckTaskValueChangeMethod & handle);
	MethodClass(JXJ, JXJCheckTaskValueChange)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crGameTask *m_this;
	int m_WCHDATA_ID;
	unsigned char m_dataType;
	JXJDataStruct m_dataStruct;
	int m_lastdata;
};
class crJXJRecvTaskProgressMethod : public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvTaskProgressMethod();
	crJXJRecvTaskProgressMethod(const crJXJRecvTaskProgressMethod& handle);
	MethodClass(JXJ, JXJRecvTaskProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJAddTaskProgressMethod : public CRCore::crMethod
{//GameClient & GameServer Method 
public:
	crJXJAddTaskProgressMethod();
	crJXJAddTaskProgressMethod(const crJXJAddTaskProgressMethod& handle);
	MethodClass(JXJ, JXJAddTaskProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_taskid;
	int m_addProgress;
};
class crJXJIsCanBuyFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsCanBuyFubenMethod();
	crJXJIsCanBuyFubenMethod(const crJXJIsCanBuyFubenMethod& handle);
	MethodClass(JXJ, JXJIsCanBuyFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJBuyFubenMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJBuyFubenMsgBoxMethod();
	crJXJBuyFubenMsgBoxMethod(const crJXJBuyFubenMsgBoxMethod& handle);
	MethodClass(JXJ, JXJBuyFubenMsgBox)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_pername;
	std::string m_msgbox;
	std::string m_msgtext;
};
class crJXJBuyFubenMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJBuyFubenMethod();
	crJXJBuyFubenMethod(const crJXJBuyFubenMethod& handle);
	MethodClass(JXJ, JXJBuyFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvBuyFubenMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyFubenMethod();
	crJXJRecvBuyFubenMethod(const crJXJRecvBuyFubenMethod& handle);
	MethodClass(JXJ, JXJRecvBuyFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRefineDiscountTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJRefineDiscountTimerMethod();
	crJXJRefineDiscountTimerMethod(const crJXJRefineDiscountTimerMethod& handle);
	MethodClass(JXJ, JXJRefineDiscountTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvRefineDiscountTimerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRefineDiscountTimerMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJRecvRefineDiscountTimerMethod();
	crJXJRecvRefineDiscountTimerMethod(const crJXJRecvRefineDiscountTimerMethod & handle);
	MethodClass(JXJ, JXJRecvRefineDiscountTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerUseRefineDiscountMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseRefineDiscountMethod();
	crJXJServerUseRefineDiscountMethod(const crJXJServerUseRefineDiscountMethod& handle);
	MethodClass(JXJ, JXJServerUseRefineDiscount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
};
class crJXJRecvUseRefineDiscountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseRefineDiscountMethod();
	crJXJRecvUseRefineDiscountMethod(const crJXJRecvUseRefineDiscountMethod& handle);
	MethodClass(JXJ, JXJRecvUseRefineDiscount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSearchVipLevelMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchVipLevelMethod();
	crJXJSearchVipLevelMethod(const crJXJSearchVipLevelMethod& handle);
	MethodClass(JXJ, JXJSearchVipLevel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};

class crJXJSearchVipLevelCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchVipLevelCompleteCheckMethod();
	crJXJSearchVipLevelCompleteCheckMethod(const crJXJSearchVipLevelCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchVipLevelCompleteCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJGetCureEffectPercentMethod : public CRCore::crMethod
{//SceneServer & Client Method
public:
	crJXJGetCureEffectPercentMethod();
	crJXJGetCureEffectPercentMethod(const crJXJGetCureEffectPercentMethod& handle);
	MethodClass(JXJ, JXJGetCureEffectPercent)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_output;
};
class crJXJMoreOpenDetectMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJMoreOpenDetectMethod();
	crJXJMoreOpenDetectMethod(const crJXJMoreOpenDetectMethod& handle);
	MethodClass(JXJ, JXJMoreOpenDetect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJAutoTouchItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoTouchItemMethod();
	crJXJAutoTouchItemMethod(const crJXJAutoTouchItemMethod& handle);
	MethodClass(JXJ, JXJAutoTouchItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_dist;
};
class crJXJQueryItemRTHPMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQueryItemRTHPMethod();
	crJXJQueryItemRTHPMethod(const crJXJQueryItemRTHPMethod& handle);
	MethodClass(JXJ, JXJQueryItemRTHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
};
class crJXJRecvQueryItemRTHPMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryItemRTHPMethod();
	crJXJRecvQueryItemRTHPMethod(const crJXJRecvQueryItemRTHPMethod& handle);
	MethodClass(JXJ, JXJRecvQueryItemRTHP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
}
#endif