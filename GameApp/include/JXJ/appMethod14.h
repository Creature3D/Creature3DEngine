/**********************************************************************
*
*	文件:	   appMethod14.h
*
*	描述:
*
*	作者:	   吴桐
*
*
**********************************************************************/
#ifndef JXJ_APPMETHOD14_H
#define JXJ_APPMETHOD14_H
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

/////////////////////////////////////////
//
//crJXJUIQiangZhengTipMethod
//强征提示
//
/////////////////////////////////////////
class crJXJUIQiangZhengTipMethod : public CRCore::crMethod
{//Client Method
	public:
		crJXJUIQiangZhengTipMethod ();
		 crJXJUIQiangZhengTipMethod (const  crJXJUIQiangZhengTipMethod & handle);
		MethodClass(JXJ, JXJUIQiangZhengTip)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode* m_this;
		std::string m_tipsCanvas;
		std::string m_tipsTextWidget;
		std::string m_id;
};


/////////////////////////////////////////
//
//crJXJUIJunTuanFuBenKaiQiTipMethod 
//军团副本开启提示
//
/////////////////////////////////////////
class crJXJUIJunTuanFuBenKaiQiTipMethod : public CRCore::crMethod
{//Client Method
	public:
		crJXJUIJunTuanFuBenKaiQiTipMethod  ();
		crJXJUIJunTuanFuBenKaiQiTipMethod  (const  crJXJUIJunTuanFuBenKaiQiTipMethod  & handle);
		MethodClass(JXJ, JXJUIJunTuanFuBenKaiQiTip)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode* m_this;
		std::string m_tipsCanvas;
		std::string m_tipsTextWidget;
		std::string m_groupCanvas;
		int m_idx;
};

/////////////////////////////////////////
//
//crJXJUIGerenJuanXianTipMethod
//个人捐献提示
//
/////////////////////////////////////////
class crJXJUIGerenJuanXianTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGerenJuanXianTipMethod ();
	crJXJUIGerenJuanXianTipMethod (const  crJXJUIGerenJuanXianTipMethod & handle);
	MethodClass(JXJ, JXJUIGerenJuanXianTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
	std::string m_resource;
	std::string m_yuanBao1;
	std::string m_yuanBao2;
	std::string m_yuanBao3;
	std::string m_yuanBao4;
	std::string m_canvas;
};

/////////////////////////////////////////
//
//crJXJUIShangBiaoJunGongTipMethod
//上表军功提示
//
/////////////////////////////////////////
class crJXJUIShangBiaoJunGongTipMethod :public CRCore::crMethod
{ // client
public:
	crJXJUIShangBiaoJunGongTipMethod ();
	crJXJUIShangBiaoJunGongTipMethod (const crJXJUIShangBiaoJunGongTipMethod & handle);
	MethodClass(JXJ, JXJUIShangBiaoJunGongTip)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

/////////////////////////////////////////
//
//crJXJUIDuoBeiHuoQvQueRenMethod
//多倍获取确认
//
/////////////////////////////////////////
class crJXJUIDuoBeiHuoQvQueRenMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIDuoBeiHuoQvQueRenMethod();
	crJXJUIDuoBeiHuoQvQueRenMethod(const crJXJUIDuoBeiHuoQvQueRenMethod& handle);
	MethodClass(JXJ, JXJUIDuoBeiHuoQvQueRen)
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

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattleBuffMethod
//战场犒赏确认框
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIBattleBuffMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIBattleBuffMethod();
	crJXJUIBattleBuffMethod(const crJXJUIBattleBuffMethod& handle);
	MethodClass(JXJ,JXJUIBattleBuff)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleBuffMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleBuffMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJBattleBuffMethod();
	crJXJBattleBuffMethod(const crJXJBattleBuffMethod &handle);
	MethodClass(JXJ,JXJBattleBuff)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRUI::crButtonWidgetNode *m_this;
	int m_index;
};


//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleBuffMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvBattleRBuffMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvBattleRBuffMethod();
	crJXJRecvBattleRBuffMethod(const crJXJRecvBattleRBuffMethod &handle);
	MethodClass(JXJ,JXJRecvBattleRBuff)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	//std::string m_strInput;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJKaoShangCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJKaoShangCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJKaoShangCanvasUpdateMethod ();
	crJXJKaoShangCanvasUpdateMethod (const crJXJKaoShangCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJKaoShangCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_kaoshangA;
	std::string m_kaoshangD;
	std::string m_inspireButton_attack;
	std::string m_inspireButton_defend;	
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUILunHuiStoreAffirmMethod
//轮回仓库存储确认框
//
//////////////////////////////////////////////////////////////////////////
class crJXJUILunHuiStoreAffirmMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUILunHuiStoreAffirmMethod();
	crJXJUILunHuiStoreAffirmMethod(const crJXJUILunHuiStoreAffirmMethod& handle);
	MethodClass(JXJ,JXJUILunHuiStoreAffirm)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
	int         m_index;
};

/////////////////////////////////////////
//
//crJXJUIUndoStoreJiangKaDragMethod
//
/////////////////////////////////////////
class crJXJUIUndoStoreJiangKaDragMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUndoStoreJiangKaDragMethod();
	crJXJUIUndoStoreJiangKaDragMethod(const crJXJUIUndoStoreJiangKaDragMethod& handle);
	MethodClass(JXJ, JXJUIUndoStoreJiangKaDrag)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_cardcanvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUILunHuiStoreTiHuanAffirmMethod
//轮回仓库存储替换确认框
//
//////////////////////////////////////////////////////////////////////////
class crJXJUILunHuiStoreTiHuanAffirmMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUILunHuiStoreTiHuanAffirmMethod();
	crJXJUILunHuiStoreTiHuanAffirmMethod(const crJXJUILunHuiStoreTiHuanAffirmMethod& handle);
	MethodClass(JXJ,JXJUILunHuiStoreTiHuanAffirm)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crWidgetNode *m_this;
	CRUI::crListControlWidgetNode* m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
	std::string m_canvas;
	int m_index;
};

/////////////////////////////////////////
//
//crJXJUIUpgradeResAddBuildingTipMethod
//资源加成筑升级提示
//
/////////////////////////////////////////
class crJXJUIUpgradeResAddBuildingTipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpgradeResAddBuildingTipMethod();
	crJXJUIUpgradeResAddBuildingTipMethod(const crJXJUIUpgradeResAddBuildingTipMethod& handle);
	MethodClass(JXJ, JXJUIUpgradeResAddBuildingTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

/////////////////////////////////////////
//
//crJXJUILunHuidaojishiTipMethod
//轮回倒计时提示
//
/////////////////////////////////////////
class crJXJUILunHuidaojishiTipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUILunHuidaojishiTipMethod();
	crJXJUILunHuidaojishiTipMethod(const crJXJUILunHuidaojishiTipMethod& handle);
	MethodClass(JXJ, JXJUILunHuidaojishiTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_tipsCanvas;
	std::string m_tipsname;
	std::string m_tipsTextWidget;
};

/////////////////////////////////////////
//
//crJXJLunHuidaojishiCanvasUpdateMethod
//轮回倒计时更新
//
/////////////////////////////////////////
class crJXJLunHuidaojishiCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLunHuidaojishiCanvasUpdateMethod();
	crJXJLunHuidaojishiCanvasUpdateMethod(const crJXJLunHuidaojishiCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJLunHuidaojishiCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};

/////////////////////////////////////////
//
//crJXJLunHuiRemainTimeQueryMethod
//轮回剩余时间查询
//
/////////////////////////////////////////
class crJXJLunHuiRemainTimeQueryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLunHuiRemainTimeQueryMethod();
	crJXJLunHuiRemainTimeQueryMethod(const crJXJLunHuiRemainTimeQueryMethod& handle);
	MethodClass(JXJ, JXJLunHuiRemainTimeQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	
};

//////////////////////////////////////////
//
//crJXJRecvLunHuiRemainTimerMethod
//轮回剩余时间接收
//
//////////////////////////////////////////
class crJXJRecvLunHuiRemainTimerMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvLunHuiRemainTimerMethod();
	crJXJRecvLunHuiRemainTimerMethod(const crJXJRecvLunHuiRemainTimerMethod &handle);
	MethodClass(JXJ,JXJRecvLunHuiRemainTimer)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strInput;
};

/////////////////////////////////////////
//
//crJXJServerUseJianZhuLingMethod
//
/////////////////////////////////////////
class crJXJServerUseJianZhuLingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseJianZhuLingMethod();
	crJXJServerUseJianZhuLingMethod(const crJXJServerUseJianZhuLingMethod& handle);
	MethodClass(JXJ, JXJServerUseJianZhuLing)
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

/////////////////////////////////////////
//
//crJXJServerUseZhengMuLingMethod
//
/////////////////////////////////////////
class crJXJServerUseZhengMuLingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseZhengMuLingMethod();
	crJXJServerUseZhengMuLingMethod(const crJXJServerUseZhengMuLingMethod& handle);
	MethodClass(JXJ, JXJServerUseZhengMuLing)
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

//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckJianZhuLingCountMethod
//检查背包里是否有建筑令/征募令/召集令
//
//////////////////////////////////////////////////////////////////////////
class crJXJUICheckJianZhuLingCountMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUICheckJianZhuLingCountMethod();
	crJXJUICheckJianZhuLingCountMethod(const crJXJUICheckJianZhuLingCountMethod& handle);
	MethodClass(JXJ,JXJUICheckJianZhuLingCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
	int         m_index;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJConfirmUseJianZhuLingMethod
//建筑令/征募令/召集令,使用确认
//
//////////////////////////////////////////////////////////////////////////
class crJXJConfirmUseJianZhuLingMethod:public CRCore::crMethod
{//Client 
public:
	crJXJConfirmUseJianZhuLingMethod();
	crJXJConfirmUseJianZhuLingMethod(const crJXJConfirmUseJianZhuLingMethod& handle);
	MethodClass(JXJ,JXJConfirmUseJianZhuLing)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int         m_index;
};

//////////////////////////////////////////
//
//crJXJRecvConfirmUseJianZhuLingMethod
//
//////////////////////////////////////////
class crJXJRecvConfirmUseJianZhuLingMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvConfirmUseJianZhuLingMethod();
	crJXJRecvConfirmUseJianZhuLingMethod(const crJXJRecvConfirmUseJianZhuLingMethod &handle);
	MethodClass(JXJ,JXJRecvConfirmUseJianZhuLing)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strInput;
};

/////////////////////////////////////////
//
//crJXJServerUseZhaoJiLingMethod
//
/////////////////////////////////////////
class crJXJServerUseZhaoJiLingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseZhaoJiLingMethod();
	crJXJServerUseZhaoJiLingMethod(const crJXJServerUseZhaoJiLingMethod& handle);
	MethodClass(JXJ, JXJServerUseZhaoJiLing)
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

/////////////////////////////////////////
//
//crJXJZhengZhanShopCanvasUpdateMethod
//
/////////////////////////////////////////
class crJXJZhengZhanShopCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJZhengZhanShopCanvasUpdateMethod ();
	crJXJZhengZhanShopCanvasUpdateMethod (const crJXJZhengZhanShopCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJZhengZhanShopCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw[8];
	std::string m_imageBox[8];
	std::string m_count[8];
	std::string m_goumaiBtn[8];
	std::string m_needLvInput[8];
	std::string m_priceInput[8];
	std::string m_cZhangQuan;
	std::string m_scroll;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJZhengZhanShopBuyMethod
//征战商店购买物品
//
//////////////////////////////////////////////////////////////////////////
class crJXJZhengZhanShopBuyMethod:public CRCore::crMethod
{//Client 
public:
	crJXJZhengZhanShopBuyMethod();
	crJXJZhengZhanShopBuyMethod(const crJXJZhengZhanShopBuyMethod& handle);
	MethodClass(JXJ,JXJZhengZhanShopBuy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	//std::string           m_priceInput;
	std::string           m_imageBox;
};

//////////////////////////////////////////
//
//crJXJRecvZhengZhanShopBuyMethod
//
//////////////////////////////////////////
class crJXJRecvZhengZhanShopBuyMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvZhengZhanShopBuyMethod();
	crJXJRecvZhengZhanShopBuyMethod(const crJXJRecvZhengZhanShopBuyMethod &handle);
	MethodClass(JXJ,JXJRecvZhengZhanShopBuy)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_animation;
	std::string m_sequence;
};

/////////////////////////////////////////
//
//crJXJRecvZhengZanQuanMethod
//
/////////////////////////////////////////
class crJXJRecvZhengZanQuanMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvZhengZanQuanMethod();
	crJXJRecvZhengZanQuanMethod(const crJXJRecvZhengZanQuanMethod& handle);
	MethodClass(JXJ, JXJRecvZhengZanQuan)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_zhengzhanquan;
};

/////////////////////////////////////////
//
//crJXJClientRecvZhengZanQuanMethod
//
/////////////////////////////////////////
class crJXJClientRecvZhengZanQuanMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvZhengZanQuanMethod();
	crJXJClientRecvZhengZanQuanMethod(const crJXJClientRecvZhengZanQuanMethod& handle);
	MethodClass(JXJ, JXJClientRecvZhengZanQuan)
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
//crJXJChangeFrameAndDelayHindMethod
//
/////////////////////////////////////////
class crJXJChangeFrameAndDelayHindMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJChangeFrameAndDelayHindMethod();
	crJXJChangeFrameAndDelayHindMethod(const crJXJChangeFrameAndDelayHindMethod& handle);
	MethodClass(JXJ, JXJChangeFrameAndDelayHind)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;

};

/////////////////////////////////////////
//
//crJXJCheckGameDalyTaskMethod
//
/////////////////////////////////////////
class crJXJCheckGameDalyTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGameDalyTaskMethod();
	crJXJCheckGameDalyTaskMethod(const crJXJCheckGameDalyTaskMethod& handle);
	MethodClass(JXJ, JXJCheckGameDalyTask)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvGuoZhanJiFenMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvGuoZhanJiFenMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvGuoZhanJiFenMethod();
	crJXJRecvGuoZhanJiFenMethod(const crJXJRecvGuoZhanJiFenMethod& handle);
	MethodClass(JXJ, JXJRecvGuoZhanJiFen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_guozhanjifen;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvGuoZhanJiFenMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClientRecvGuoZhanJiFenMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJClientRecvGuoZhanJiFenMethod();
	crJXJClientRecvGuoZhanJiFenMethod(const crJXJClientRecvGuoZhanJiFenMethod& handle);
	MethodClass(JXJ, JXJClientRecvGuoZhanJiFen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	float m_time;
	std::string m_canvas;
};

/////////////////////////////////////////
//
//crJXJCheckGameZhouChangTaskMethod
//
/////////////////////////////////////////
class crJXJCheckGameZhouChangTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGameZhouChangTaskMethod();
	crJXJCheckGameZhouChangTaskMethod(const crJXJCheckGameZhouChangTaskMethod& handle);
	MethodClass(JXJ, JXJCheckGameZhouChangTask)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJClientRecvZhanLingCityCountMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJClientRecvZhanLingCityCountMethod();
	crJXJClientRecvZhanLingCityCountMethod(const crJXJClientRecvZhanLingCityCountMethod& handle);
	MethodClass(JXJ, JXJClientRecvZhanLingCityCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	float m_time;
	std::string m_canvas;
};

class crJXJActiveZhouChangTaskMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJActiveZhouChangTaskMethod ();
	crJXJActiveZhouChangTaskMethod (const crJXJActiveZhouChangTaskMethod & handle);
	MethodClass(JXJ, JXJActiveZhouChangTask)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//time_t m_lasttimer;
};

/////////////////////////////////////////
//
//crJXJUIMingJiangZhiLuJiangLiTipMethod
//名将之路奖励提示
//
/////////////////////////////////////////
class crJXJUIMingJiangZhiLuJiangLiTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIMingJiangZhiLuJiangLiTipMethod ();
	crJXJUIMingJiangZhiLuJiangLiTipMethod (const  crJXJUIMingJiangZhiLuJiangLiTipMethod & handle);
	MethodClass(JXJ, JXJUIMingJiangZhiLuJiangLiTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

class crJXJSearchSceneRoomMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchSceneRoomMethod();
	crJXJSearchSceneRoomMethod(const crJXJSearchSceneRoomMethod& handle);
	MethodClass(JXJ, JXJSearchSceneRoom)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJLoginWorldFuBenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLoginWorldFuBenMethod();
	crJXJLoginWorldFuBenMethod(const crJXJLoginWorldFuBenMethod& handle);
	MethodClass(JXJ, JXJLoginWorldFuBen)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	/*std::string m_canvas;
	std::string m_button;*/
};

//class crJXJSetWorldFubenIDMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJSetWorldFubenIDMethod();
//	crJXJSetWorldFubenIDMethod(const crJXJSetWorldFubenIDMethod& handle);
//	MethodClass(JXJ, JXJSetWorldFubenID)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};

class crJXJIsWorldFuBenOrChengZhanMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJIsWorldFuBenOrChengZhanMethod();
	crJXJIsWorldFuBenOrChengZhanMethod(const crJXJIsWorldFuBenOrChengZhanMethod& handle);
	MethodClass(JXJ, JXJIsWorldFuBenOrChengZhan)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_battlelist;
	std::string m_canvas;
	std::string m_battlename;
	int m_index;
};

class crJXJSetWorldFubenBirthPointMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetWorldFubenBirthPointMethod();
	crJXJSetWorldFubenBirthPointMethod(const crJXJSetWorldFubenBirthPointMethod& handle);
	MethodClass(JXJ, JXJSetWorldFubenBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_birthpoint;
};

class crJXJRecvWorldFubenBirthPointMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRecvWorldFubenBirthPointMethod();
	crJXJRecvWorldFubenBirthPointMethod(const crJXJRecvWorldFubenBirthPointMethod& handle);
	MethodClass(JXJ, JXJRecvWorldFubenBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

//////////////////////////////////////////////////////////////////////////
//crJXJInitWorldFubenPlayerReliveCountMethod
//////////////////////////////////////////////////////////////////////////
class crJXJInitWorldFubenPlayerReliveCountMethod:public CRCore::crMethod
{//scene  Method
public:
	crJXJInitWorldFubenPlayerReliveCountMethod();
	crJXJInitWorldFubenPlayerReliveCountMethod(const crJXJInitWorldFubenPlayerReliveCountMethod &handle);
	MethodClass(JXJ,JXJInitWorldFubenPlayerReliveCount)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
};

class crJXJUIZhengZhanShopCommodityTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIZhengZhanShopCommodityTipsMethod();
	crJXJUIZhengZhanShopCommodityTipsMethod(const crJXJUIZhengZhanShopCommodityTipsMethod& handle);
	MethodClass(JXJ, JXJUIZhengZhanShopCommodityTips)
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
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_imageindex;
};

class crJXJShowMonsterInforMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJShowMonsterInforMethod();
	crJXJShowMonsterInforMethod(const crJXJShowMonsterInforMethod& handle);
	MethodClass(JXJ, JXJShowMonsterInfor)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_zhengtingid;
	float m_lastperHP;
	int m_lastlivecount;
	//int m_npctableid;//
	/*std::string m_valname;*/
	//unsigned char m_rewardtype;
	//CRCore::crVector3i m_rewarditem;
	//std::string m_rewarditem;
};

class crJXJRecvShowMonsterInforMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvShowMonsterInforMethod();
	crJXJRecvShowMonsterInforMethod(const crJXJRecvShowMonsterInforMethod& handle);
	MethodClass(JXJ, JXJRecvShowMonsterInfor)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_XinxiCanvas;
	std::string m_boshu;
	std::string m_guaiwu;
	std::string m_zhengting;
	std::string m_fancanvas;
	std::string m_valname;
	bool m_isstart;
	std::string m_KillTimeText;
};

class crJXJActiveTaskAfterCountryFoundedMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJActiveTaskAfterCountryFoundedMethod ();
	crJXJActiveTaskAfterCountryFoundedMethod (const crJXJActiveTaskAfterCountryFoundedMethod & handle);
	MethodClass(JXJ, JXJActiveTaskAfterCountryFounded)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	bool m_isFirstWeek;
	unsigned char m_shiliID;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJTreasonCountryTiShiCanvasShowMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTreasonCountryTiShiCanvasShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTreasonCountryTiShiCanvasShowMethod();
	crJXJTreasonCountryTiShiCanvasShowMethod(const crJXJTreasonCountryTiShiCanvasShowMethod& handle);
	MethodClass(JXJ, JXJTreasonCountryTiShiCanvasShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_strcaenvas;
	std::string m_input;
	int m_type;
};

class crJXJtreasonbuffTimeupdataMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJtreasonbuffTimeupdataMethod();
	crJXJtreasonbuffTimeupdataMethod(const crJXJtreasonbuffTimeupdataMethod& handle);
	MethodClass(JXJ, JXJtreasonbuffTimeupdata)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_totaltime;
};

class crJXJRecvtreasonbuffTimeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRecvtreasonbuffTimeMethod();
	crJXJRecvtreasonbuffTimeMethod(const crJXJRecvtreasonbuffTimeMethod& handle);
	MethodClass(JXJ, JXJRecvtreasonbuffTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

/////////////////////////////////////////
//
//crJXJtreasonbuffCanvasUpdateMethod
//叛国缓冲期更新
//
/////////////////////////////////////////
class crJXJtreasonbuffCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJtreasonbuffCanvasUpdateMethod();
	crJXJtreasonbuffCanvasUpdateMethod(const crJXJtreasonbuffCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJtreasonbuffCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerCancleChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerCancleChangeShiliMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerCancleChangeShiliMethod();
	crJXJPlayerCancleChangeShiliMethod(const crJXJPlayerCancleChangeShiliMethod& handle);
	MethodClass(JXJ, JXJPlayerCancleChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	//std::string m_strRadioGroup;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerCancleChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvPlayerCancleChangeShiliMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvPlayerCancleChangeShiliMethod();
	crJXJRecvPlayerCancleChangeShiliMethod(const crJXJRecvPlayerCancleChangeShiliMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerCancleChangeShili)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string  m_huodongcanvas;
	std::string	 m_chengzhanbtn;
	std::string  m_panguocanvas;
};

//////////////////////////////////////////////////////////////////////////
//crJXJClienttreasoncooltimeUpdateMethod
//////////////////////////////////////////////////////////////////////////
class crJXJClienttreasoncooltimeUpdateMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClienttreasoncooltimeUpdateMethod();
	crJXJClienttreasoncooltimeUpdateMethod(const crJXJClienttreasoncooltimeUpdateMethod & handle);
	MethodClass(JXJ, JXJClienttreasoncooltimeUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};

/////////////////////////////////////////
//
//crJXJtreasonpunishCanvasUpdateMethod
//叛国惩罚期更新
//
/////////////////////////////////////////
class crJXJtreasonpunishCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJtreasonpunishCanvasUpdateMethod();
	crJXJtreasonpunishCanvasUpdateMethod(const crJXJtreasonpunishCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJtreasonpunishCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJClearTreasonBuffTimeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJClearTreasonBuffTimeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClearTreasonBuffTimeMethod();
	crJXJClearTreasonBuffTimeMethod(const crJXJClearTreasonBuffTimeMethod& handle);
	MethodClass(JXJ, JXJClearTreasonBuffTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	//std::string m_strRadioGroup;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvClearTreasonBuffTimeMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvClearTreasonBuffTimeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvClearTreasonBuffTimeMethod();
	crJXJRecvClearTreasonBuffTimeMethod(const crJXJRecvClearTreasonBuffTimeMethod& handle);
	MethodClass(JXJ, JXJRecvClearTreasonBuffTime)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string  m_huodongcanvas;
	//std::string	 m_chengzhanbtn;
	//std::string  m_panguocanvas;
};

class crJXJIsMyCountryLostMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJIsMyCountryLostMethod();
	crJXJIsMyCountryLostMethod(const crJXJIsMyCountryLostMethod& handle);
	MethodClass(JXJ, JXJIsMyCountryLost)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static unsigned char m_isFirstEnter;
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
};

class crJXJCheckInTreasonPunishTimeMethod: public CRCore::crMethod
{//GameClient Method
public:
	crJXJCheckInTreasonPunishTimeMethod();
	crJXJCheckInTreasonPunishTimeMethod(const crJXJCheckInTreasonPunishTimeMethod& handle);
	MethodClass(JXJ,JXJCheckInTreasonPunishTime)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crJXJViewPlayerInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJViewPlayerInfoMethod();
	crJXJViewPlayerInfoMethod(const crJXJViewPlayerInfoMethod& handle);
	MethodClass(JXJ, JXJViewPlayerInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJRecvPlayerInfoMethod :public CRCore::crMethod
{//Client Method && Sever Method
public:
	crJXJRecvPlayerInfoMethod();
	crJXJRecvPlayerInfoMethod(const crJXJRecvPlayerInfoMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	
	std::string m_name;
	std::string m_guoJia;
	std::string m_level;
	std::string m_guanzhi;
	std::string m_gongxun;
	std::string m_bingfa;
	std::string m_zhenfa;
	std::string m_tongshuai;
	std::string m_dongcha;
	std::string m_equip[5];
	std::string m_touxiang;
	std::string m_exp;
	std::string m_defaultname[5];
	std::string m_btnAdd[4];
	std::string m_radioBtn[4];
	std::string m_radioGroup;
};

class crJXJNewRecvBattleBrocastMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJNewRecvBattleBrocastMethod();
	crJXJNewRecvBattleBrocastMethod(const crJXJNewRecvBattleBrocastMethod &handle);
	MethodClass(JXJ,JXJNewRecvBattleBrocast)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJIsLostCountryofFoundMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJIsLostCountryofFoundMethod();
	crJXJIsLostCountryofFoundMethod(const crJXJIsLostCountryofFoundMethod& handle);
	MethodClass(JXJ, JXJIsLostCountryofFound)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_citywei;
	std::string m_cityshu;
	std::string m_citywu;
};

//class crJXJIsMyCountryInBattleMethod :public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJIsMyCountryInBattleMethod();
//	crJXJIsMyCountryInBattleMethod(const crJXJIsMyCountryInBattleMethod& handle);
//	MethodClass(JXJ, JXJIsMyCountryInBattle)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_battlecanvas;
//};

class crBigMapMoveToSelectedCityPosMethod :public CRCore::crMethod
{//Client Method
public:
	crBigMapMoveToSelectedCityPosMethod();
	crBigMapMoveToSelectedCityPosMethod(const crBigMapMoveToSelectedCityPosMethod& handle);
	MethodClass(JXJ, BigMapMoveToSelectedCityPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_strBigMapImage;
	std::string m_strOtherMapCanvas;
	std::string m_strOtherMapImage;
	std::string m_strFrame;
	std::string m_strBigMapCanvas;
	std::string m_battleList;
	int m_cityindex;
};

class crJXJUISetGuoZhanGuiZheMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetGuoZhanGuiZheMethod();
	crJXJUISetGuoZhanGuiZheMethod(const crJXJUISetGuoZhanGuiZheMethod& handle);
	MethodClass(JXJ, JXJUISetGuoZhanGuiZhe)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strInput;
};

class crJXJSelectAttactListQieYeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectAttactListQieYeMethod();
	crJXJSelectAttactListQieYeMethod(const crJXJSelectAttactListQieYeMethod& handle);
	MethodClass(JXJ, JXJSelectAttactListQieYe)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_qieyesw;
	
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleOpenCenterNoticeCreateMethod
//persuade
//////////////////////////////////////////////////////////////////////////
class crJXJBattleOpenCenterNoticeCreateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJBattleOpenCenterNoticeCreateMethod();
	crJXJBattleOpenCenterNoticeCreateMethod(const crJXJBattleOpenCenterNoticeCreateMethod& handle);
	MethodClass(JXJ, JXJBattleOpenCenterNoticeCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_time;
	//bool m_iswin;
};

class crJXJRecvBattleOpenTiShiMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleOpenTiShiMethod();
	crJXJRecvBattleOpenTiShiMethod(const crJXJRecvBattleOpenTiShiMethod &handle);
	MethodClass(JXJ,JXJRecvBattleOpenTiShi)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvCityInBAattleMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvCityInBAattleMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCityInBAattleMethod();
	crJXJRecvCityInBAattleMethod(const crJXJRecvCityInBAattleMethod& handle);
	MethodClass(JXJ, JXJRecvCityInBAattle)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string  m_huodongcanvas;
	//std::string	 m_chengzhanbtn;
	//std::string  m_panguocanvas;
};

class crJXJUISetDefaultAttactCountryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetDefaultAttactCountryMethod();
	crJXJUISetDefaultAttactCountryMethod(const crJXJUISetDefaultAttactCountryMethod& handle);
	MethodClass(JXJ, JXJUISetDefaultAttactCountry)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_radiowei;
	std::string m_radioshu;
	std::string m_radiowu;
	
};

class crJXJServerRessetDailyTaskMethod :public CRCore::crMethod
{//GameServer Method 玩家等级升级的时候检查一次
public:
	crJXJServerRessetDailyTaskMethod();
	crJXJServerRessetDailyTaskMethod(const crJXJServerRessetDailyTaskMethod& handle);
	MethodClass(JXJ, JXJServerRessetDailyTask)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
};

class crJXJRecvResetDailyTaskMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvResetDailyTaskMethod();
	crJXJRecvResetDailyTaskMethod(const crJXJRecvResetDailyTaskMethod &handle);
	MethodClass(JXJ,JXJRecvResetDailyTask)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJReBuildAttactListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJReBuildAttactListMethod();
	crJXJReBuildAttactListMethod(const crJXJReBuildAttactListMethod& handle);
	MethodClass(JXJ, JXJReBuildAttactList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	//static unsigned char m_isFirstEnter;
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_battleList;
	std::string m_qiehuanshilisw;
	std::string m_radiowei;
	std::string m_radioshu;
	std::string m_radiowu;
	std::string m_enterbtnA[7];
};

class crJXJJunZiArrivedDestinationMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJJunZiArrivedDestinationMethod();
	crJXJJunZiArrivedDestinationMethod(const crJXJJunZiArrivedDestinationMethod& handle);
	MethodClass(JXJ, JXJJunZiArrivedDestination)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_type; //1、资源车抵达目的地 2、资源车刷新 3、1点半boss刷新 4/6点半boss刷新
	std::string m_countid;
};

class crJXJRecvJunZiArriveDestiMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvJunZiArriveDestiMethod();
	crJXJRecvJunZiArriveDestiMethod(const crJXJRecvJunZiArriveDestiMethod &handle);
	MethodClass(JXJ,JXJRecvJunZiArriveDesti)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJGuoJiaZiJinJiangLiMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJGuoJiaZiJinJiangLiMethod();
	crJXJGuoJiaZiJinJiangLiMethod(const crJXJGuoJiaZiJinJiangLiMethod& handle);
	MethodClass(JXJ, JXJGuoJiaZiJinJiangLi)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_dead;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fire;
	int m_count; //奖励数量
	std::string m_item; //物品奖励
	int m_points;    //积分奖励
};

class crJXJGuoJiaZiJinAwardSourceCarMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJGuoJiaZiJinAwardSourceCarMethod();
	crJXJGuoJiaZiJinAwardSourceCarMethod(const crJXJGuoJiaZiJinAwardSourceCarMethod& handle);
	MethodClass(JXJ, JXJGuoJiaZiJinAwardSourceCar)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_dead;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fire;
	int m_shiliid; //1、势力id
	int m_count; //奖励数量
	std::string m_item; //物品奖励
	int m_points;    //积分奖励
};

class crJXJGameServerRecvCountryCapitalMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerRecvCountryCapitalMethod();
	crJXJGameServerRecvCountryCapitalMethod(const crJXJGameServerRecvCountryCapitalMethod& handle);
	MethodClass(JXJ, JXJGameServerRecvCountryCapital)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJCheckGuoZhanOrJianGuoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckGuoZhanOrJianGuoMethod();
	crJXJCheckGuoZhanOrJianGuoMethod(const crJXJCheckGuoZhanOrJianGuoMethod& handle);
	MethodClass(JXJ, JXJCheckGuoZhanOrJianGuo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_guozhansw;
	std::string m_guozhan;
	std::string m_guanai;
	std::string m_sweep;
	std::string m_battlejr;
};

class crJXJMyCountryHaveCityInBattleMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJMyCountryHaveCityInBattleMethod();
	crJXJMyCountryHaveCityInBattleMethod(const crJXJMyCountryHaveCityInBattleMethod& handle);
	MethodClass(JXJ, JXJMyCountryHaveCityInBattle)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static unsigned char m_isFirstEnter;
protected:
	CRCore::crCanvasNode* m_this;
};

class crJXJSortAttactListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSortAttactListMethod();
	crJXJSortAttactListMethod(const crJXJSortAttactListMethod& handle);
	MethodClass(JXJ, JXJSortAttactList)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_battleList;
	std::string m_qiehuanshilisw;
	std::string m_radiowei;
	std::string m_radioshu;
	std::string m_radiowu;
	std::string m_enterbtnA[7];
};

class crJXJCityHaveDequeUpdateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCityHaveDequeUpdateMethod();
	crJXJCityHaveDequeUpdateMethod(const crJXJCityHaveDequeUpdateMethod& handle);
	MethodClass(JXJ, JXJCityHaveDequeUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	unsigned short m_chengchiid;
	unsigned short m_type;
	unsigned char m_attactshili;
	unsigned char m_defenedshili;
};

class crJXJRecvUpdataBattleDequeMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUpdataBattleDequeMethod();
	crJXJRecvUpdataBattleDequeMethod(const crJXJRecvUpdataBattleDequeMethod &handle);
	MethodClass(JXJ,JXJRecvUpdataBattleDeque)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJIntoWorldBFConfirmMethod
//进入世界战场确认框
//
//////////////////////////////////////////////////////////////////////////
class crJXJIntoWorldBFConfirmMethod:public CRCore::crMethod
{//Client 
public:
	crJXJIntoWorldBFConfirmMethod();
	crJXJIntoWorldBFConfirmMethod(const crJXJIntoWorldBFConfirmMethod& handle);
	MethodClass(JXJ,JXJIntoWorldBFConfirm)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeCanvas;
	std::string m_input;
	std::string m_drag;
	std::string m_okbtn;
};

class crJXJBattleBaseNoticeUpdataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleBaseNoticeUpdataMethod();
	crJXJBattleBaseNoticeUpdataMethod(const crJXJBattleBaseNoticeUpdataMethod& handle);
	MethodClass(JXJ, JXJBattleBaseNoticeUpdata)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strInput;
};

class crJXJBattleBehaveFrameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleBehaveFrameMethod();
	crJXJBattleBehaveFrameMethod(const crJXJBattleBehaveFrameMethod& handle);
	MethodClass(JXJ, JXJBattleBehaveFrame)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_canvas;
};

class crJXJBattleBehaveOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleBehaveOnShowMethod();
	crJXJBattleBehaveOnShowMethod(const crJXJBattleBehaveOnShowMethod& handle);
	MethodClass(JXJ, JXJBattleBehaveOnShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_canvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleNoticeHideMethod
//隐藏战场基础信息通告
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleNoticeHideMethod:public CRCore::crMethod
{//Client 
public:
	crJXJBattleNoticeHideMethod();
	crJXJBattleNoticeHideMethod(const crJXJBattleNoticeHideMethod& handle);
	MethodClass(JXJ,JXJBattleNoticeHide)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_kaiguan;
	std::string m_text;

};

//////////////////////////////////////////////////////////////////////////
//crJXJBattleShowNoticeCanvasMethod
//////////////////////////////////////////////////////////////////////////
class crJXJBattleShowNoticeCanvasMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJBattleShowNoticeCanvasMethod();
	crJXJBattleShowNoticeCanvasMethod(const crJXJBattleShowNoticeCanvasMethod &handle);
	MethodClass(JXJ,JXJBattleShowNoticeCanvas)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_noticeCanvas;
	std::string m_showCanvas;
};

class crJXJCountryOfficialLeaveMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCountryOfficialLeaveMethod();
	crJXJCountryOfficialLeaveMethod(const crJXJCountryOfficialLeaveMethod& handle);
	MethodClass(JXJ, JXJCountryOfficialLeave)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	int m_playerid;
	CRCore::ref_ptr<CRNetApp::crPlayerGameData> m_playerGameData;

};

class crJXJCountryOfficialEnterMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCountryOfficialEnterMethod();
	crJXJCountryOfficialEnterMethod(const crJXJCountryOfficialEnterMethod& handle);
	MethodClass(JXJ, JXJCountryOfficialEnter)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crSceneServerPlayerData> m_playerData;

};

class crJXJBattleNoticeMouseMoveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBattleNoticeMouseMoveMethod();
	crJXJBattleNoticeMouseMoveMethod(const crJXJBattleNoticeMouseMoveMethod& handle);
	MethodClass(JXJ, JXJBattleNoticeMouseMove)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_input;
	std::string m_hide;
	std::string m_show;
	std::string m_type;
};

class crJXJNewClientRecvDamageUIUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJNewClientRecvDamageUIUpdateMethod();
	crJXJNewClientRecvDamageUIUpdateMethod(const crJXJNewClientRecvDamageUIUpdateMethod& handle);
	MethodClass(JXJ, JXJNewClientRecvDamageUIUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_offset;
	float *m_dt;
	float m_maxoffset;
	float m_duration;
	float m_lasttimer;
	float m_speed;
	float m_deltax;
	float m_deltay;
	char m_count;
};

class crJXJNewClientRecvDamageUIUpdate2Method : public CRCore::crMethod
{//Client Method
public:
	crJXJNewClientRecvDamageUIUpdate2Method();
	crJXJNewClientRecvDamageUIUpdate2Method(const crJXJNewClientRecvDamageUIUpdate2Method& handle);
	MethodClass(JXJ, JXJNewClientRecvDamageUIUpdate2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_offset;
	float *m_dt;
	float m_maxoffset;
	float m_duration;
	float m_lasttimer;
	float m_speed;
	float m_deltax;
	float m_deltay;
	char m_count;
};

//////////////////////////////////////////////////////////////////////////
//crJXJShowZhenCanvasMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowZhenCanvasMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJShowZhenCanvasMethod();
	crJXJShowZhenCanvasMethod(const crJXJShowZhenCanvasMethod &handle);
	MethodClass(JXJ,JXJShowZhenCanvas)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_noticeCanvas;
	std::string m_showCanvas;
};

class crJXJAddSightRangeMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJAddSightRangeMethod();
	crJXJAddSightRangeMethod(const crJXJAddSightRangeMethod& handle);
	MethodClass(JXJ, JXJAddSightRange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crSceneServerPlayerData> m_playerData;
	float m_extra;
};
class crJXJClientAddSightRangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientAddSightRangeMethod();
	crJXJClientAddSightRangeMethod(const crJXJClientAddSightRangeMethod& handle);
	MethodClass(JXJ, JXJClientAddSightRange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float m_extra;
};

class crJXJRecvOpenTimersMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvOpenTimersMethod();
	crJXJRecvOpenTimersMethod(const crJXJRecvOpenTimersMethod& handle);
	MethodClass(JXJ, JXJRecvOpenTimers)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crPunishCanvasTipsShowMethod :public CRCore::crMethod
{//Client Method
public:
	crPunishCanvasTipsShowMethod();
	crPunishCanvasTipsShowMethod(const crPunishCanvasTipsShowMethod& handle);
	MethodClass(JXJ, PunishCanvasTipsShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

class crJXJRecvWorldFuBenDataMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJRecvWorldFuBenDataMethod();
	crJXJRecvWorldFuBenDataMethod(const crJXJRecvWorldFuBenDataMethod &handle);
	MethodClass(JXJ,JXJRecvWorldFuBenData)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJWorldHuoDongStartOrEndMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJWorldHuoDongStartOrEndMethod();
	crJXJWorldHuoDongStartOrEndMethod(const crJXJWorldHuoDongStartOrEndMethod& handle);
	MethodClass(JXJ, JXJWorldHuoDongStartOrEnd)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_dead;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fire;
	int m_startorend; //开始或结束标志位
	int m_exercisetype; //0、上午活动，1、下午活动，2、晚上活动,3、活动未开始
	
};

class crJXJWorldEventNoticeMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJWorldEventNoticeMethod();
	crJXJWorldEventNoticeMethod(const crJXJWorldEventNoticeMethod& handle);
	MethodClass(JXJ, JXJWorldEventNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	//CRCore::ref_ptr<CRNetApp::crInstanceItem> m_dead;
	//CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fire;
	//int m_startorend; //开始或结束标志位
	//int m_exercisetype; //0、上午活动，1、下午活动，2、晚上活动

};

class crJXJgServerRecvWorldFuBenAwardMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJgServerRecvWorldFuBenAwardMethod();
	crJXJgServerRecvWorldFuBenAwardMethod(const crJXJgServerRecvWorldFuBenAwardMethod& handle);
	MethodClass(JXJ, JXJgServerRecvWorldFuBenAward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJQueryWorldFuBenDataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryWorldFuBenDataMethod();
	crJXJQueryWorldFuBenDataMethod(const crJXJQueryWorldFuBenDataMethod& handle);
	MethodClass(JXJ, JXJQueryWorldFuBenData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crJXJDisposeQueryWorldFuBenDataMethod :public CRCore::crMethod
{//SceneServer Method、Gameserver Method、Client Method
public:
	crJXJDisposeQueryWorldFuBenDataMethod();
	crJXJDisposeQueryWorldFuBenDataMethod(const crJXJDisposeQueryWorldFuBenDataMethod& handle);
	MethodClass(JXJ, JXJDisposeQueryWorldFuBenData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJInitWorldFuBenRankDataMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJInitWorldFuBenRankDataMethod();
	crJXJInitWorldFuBenRankDataMethod(const crJXJInitWorldFuBenRankDataMethod& handle);
	MethodClass(JXJ, JXJInitWorldFuBenRankData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRoom *m_this;
	CRCore::ref_ptr<CRNetApp::crSceneServerPlayerData> m_playerData;
};

class crJXJWorldFuBenStartBrocastMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJWorldFuBenStartBrocastMethod();
	crJXJWorldFuBenStartBrocastMethod(const crJXJWorldFuBenStartBrocastMethod& handle);
	MethodClass(JXJ, JXJWorldFuBenStartBrocast)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJClientWorldFuBenBrocastMethod :public CRCore::crMethod
{//SceneServer Method、Gameserver Method、Client Method
public:
	crJXJClientWorldFuBenBrocastMethod();
	crJXJClientWorldFuBenBrocastMethod(const crJXJClientWorldFuBenBrocastMethod& handle);
	MethodClass(JXJ, JXJClientWorldFuBenBrocast)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_text;
};

class crJXJDirectlyGetAwardWindowpopMethod :public CRCore::crMethod
{//Client 
public:
	crJXJDirectlyGetAwardWindowpopMethod();
	crJXJDirectlyGetAwardWindowpopMethod(const crJXJDirectlyGetAwardWindowpopMethod& handle);
	MethodClass(JXJ, JXJDirectlyGetAwardWindowpop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_rankcanvas;
};

class crJXJButtonGetJiFenAwardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJButtonGetJiFenAwardMethod();
	crJXJButtonGetJiFenAwardMethod(const crJXJButtonGetJiFenAwardMethod& handle);
	MethodClass(JXJ, JXJButtonGetJiFenAward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_id;
	std::string m_rankcanvas;
};

class crJXJRecvButtonGetJiFenAwardMethod :public CRCore::crMethod
{//Client server method
public:
	crJXJRecvButtonGetJiFenAwardMethod();
	crJXJRecvButtonGetJiFenAwardMethod(const crJXJRecvButtonGetJiFenAwardMethod& handle);
	MethodClass(JXJ, JXJRecvButtonGetJiFenAward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_directcanvas;
	std::string m_canvas;
	CRCore::crVector3i m_huodongdengji;
};

class crJXJServerDisposeShowPraiseMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJServerDisposeShowPraiseMethod();
	crJXJServerDisposeShowPraiseMethod(const crJXJServerDisposeShowPraiseMethod& handle);
	MethodClass(JXJ, JXJServerDisposeShowPraise)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	CRCore::crVector2i m_dengji1;
	CRCore::crVector2i m_dengji2;
	CRCore::crVector2i m_dengji3;
	CRCore::crVector2i m_dengji4;
	CRCore::crVector2i m_level1;
	CRCore::crVector2i m_level2;
	CRCore::crVector2i m_level3;
	//std::string m_canvas;
};

class crJXJWorldHuoDongStartClearDataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldHuoDongStartClearDataMethod();
	crJXJWorldHuoDongStartClearDataMethod(const crJXJWorldHuoDongStartClearDataMethod& handle);
	MethodClass(JXJ, JXJWorldHuoDongStartClearData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJWorldRankButtonMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJWorldRankButtonMethod:public CRCore::crMethod
{//Client 
public:
	crJXJWorldRankButtonMethod();
	crJXJWorldRankButtonMethod(const crJXJWorldRankButtonMethod& handle);
	MethodClass(JXJ,JXJWorldRankButton)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_kaiguan;

};

class crJXJWorldRankCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldRankCanvasUpdateMethod ();
	crJXJWorldRankCanvasUpdateMethod (const crJXJWorldRankCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJWorldRankCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_ranktext[10];
	std::string m_imageBox[20];
	std::string m_zanbutton[10];
	std::string m_wopaiming;
	std::string m_woshadi;
	std::string m_wosishang;
	std::string m_wojifen;
	std::string m_showbtn;
	std::string m_sw;
	std::string m_radiogroup;
	std::string m_itemcount[20];
	std::string m_paihanglist;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJWorldRankDoPraiseMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJWorldRankDoPraiseMethod:public CRCore::crMethod
{//Client 
public:
	crJXJWorldRankDoPraiseMethod();
	crJXJWorldRankDoPraiseMethod(const crJXJWorldRankDoPraiseMethod& handle);
	MethodClass(JXJ,JXJWorldRankDoPraise)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_radiogroup;
	int m_index;

};

class crJXJPlayerShowWorldRankDataMethod:public CRCore::crMethod
{//Client 
public:
	crJXJPlayerShowWorldRankDataMethod();
	crJXJPlayerShowWorldRankDataMethod(const crJXJPlayerShowWorldRankDataMethod& handle);
	MethodClass(JXJ,JXJPlayerShowWorldRankData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_radiogroup;
	int m_index;

};

class crJXJGameServerDisposeShowMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerDisposeShowMethod();
	crJXJGameServerDisposeShowMethod(const crJXJGameServerDisposeShowMethod& handle);
	MethodClass(JXJ, JXJGameServerDisposeShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_canvas;
};

class crJXJpraiseLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJpraiseLinkClickMethod();
	crJXJpraiseLinkClickMethod(const crJXJpraiseLinkClickMethod& handle);
	MethodClass(JXJ, JXJpraiseLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	int m_playerid;
	int m_count;
	
	
};

class crJXJDirecGetAwardCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDirecGetAwardCanvasUpdateMethod ();
	crJXJDirecGetAwardCanvasUpdateMethod (const crJXJDirecGetAwardCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJDirecGetAwardCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_rankcanvas;
	std::string m_jingyan;
	std::string m_liangshi;
	std::string m_mucai;
	std::string m_tiekuang;
	std::string m_mapi;
	std::string m_getsw;
	CRCore::crVector3i m_huodongdengji;
	std::string m_tishi;
};

class crJXJGetAwardCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGetAwardCanvasUpdateMethod ();
	crJXJGetAwardCanvasUpdateMethod (const crJXJGetAwardCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJGetAwardCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_rankcanvas;
	std::string m_jingyan[3];
	std::string m_liangshi[3];
	std::string m_mucai[3];
	std::string m_tiekuang[3];
	std::string m_mapi[3];
	std::string m_getbtn[3];
	CRCore::crVector3i m_huodongdengji;
	std::string m_totaljifen[3];
	std::string m_dangci[3];
	
};

class crJXJWorldFuBenStatusUpdataMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldFuBenStatusUpdataMethod();
	crJXJWorldFuBenStatusUpdataMethod(const crJXJWorldFuBenStatusUpdataMethod& handle);
	MethodClass(JXJ, JXJWorldFuBenStatusUpdata)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_rankcanvas;
	std::string m_jifen;
	std::string m_status;
};

class crJXJWorldEventRemainTimeQueryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldEventRemainTimeQueryMethod();
	crJXJWorldEventRemainTimeQueryMethod(const crJXJWorldEventRemainTimeQueryMethod& handle);
	MethodClass(JXJ, JXJWorldEventRemainTimeQuery)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_birthpoint;
};

class crJXJWorldEventRemainTimeSendMethod :public CRCore::crMethod
{//SceneServer Method、Client Method
public:
	crJXJWorldEventRemainTimeSendMethod();
	crJXJWorldEventRemainTimeSendMethod(const crJXJWorldEventRemainTimeSendMethod& handle);
	MethodClass(JXJ, JXJWorldEventRemainTimeSend)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJWorldEventTimeCorrectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldEventTimeCorrectMethod();
	crJXJWorldEventTimeCorrectMethod(const crJXJWorldEventTimeCorrectMethod& handle);
	MethodClass(JXJ, JXJWorldEventTimeCorrect)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	//std::string m_rankcanvas;
	//std::string m_jifen;
	//std::string m_status;
};

class crJXJResetWorldCanvasPositionMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJResetWorldCanvasPositionMethod();
	crJXJResetWorldCanvasPositionMethod(const crJXJResetWorldCanvasPositionMethod &handle);
	MethodClass(JXJ,JXJResetWorldCanvasPosition)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode * m_this;
	//std::string m_disposedCanvas;
	std::string m_baseCanvas;
	int m_index;
};

class crJXJRecvGuoZhanAchieveinfoMethod :public CRCore::crMethod
{//Client server method
public:
	crJXJRecvGuoZhanAchieveinfoMethod();
	crJXJRecvGuoZhanAchieveinfoMethod(const crJXJRecvGuoZhanAchieveinfoMethod& handle);
	MethodClass(JXJ, JXJRecvGuoZhanAchieveinfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_directcanvas;
	//std::string m_canvas;
	CRCore::crVector4i m_danchangjisha;
	CRCore::crVector4i m_killdoor;
	CRCore::crVector4i m_killtower;
	CRCore::crVector4i m_killzhengti;
	CRCore::crVector4i m_killboss;
};

class crJXJDetroyOppositeCountryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJDetroyOppositeCountryMethod();
	crJXJDetroyOppositeCountryMethod(const crJXJDetroyOppositeCountryMethod& handle);
	MethodClass(JXJ, JXJDetroyOppositeCountry)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	unsigned short m_achieveid1;
	unsigned short m_achieveid2;
	unsigned short m_achieveid3;
	unsigned short m_achieveid4;
	unsigned short m_achieveid5;
};

class crJXJDetroyOppositeCountrCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJDetroyOppositeCountrCheckMethod();
	crJXJDetroyOppositeCountrCheckMethod(const crJXJDetroyOppositeCountrCheckMethod& handle);
	MethodClass(JXJ, JXJDetroyOppositeCountrCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	unsigned short m_achieveid1;
	unsigned short m_achieveid2;
	unsigned short m_achieveid3;
	unsigned short m_achieveid4;
	unsigned short m_achieveid5;
};

class crJXJTroopsTechExamineMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTroopsTechExamineMethod();
	crJXJTroopsTechExamineMethod(const crJXJTroopsTechExamineMethod& handle);
	MethodClass(JXJ, JXJTroopsTechExamine)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};

class crJXJTroopsTechExamineCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJTroopsTechExamineCheckMethod();
	crJXJTroopsTechExamineCheckMethod(const crJXJTroopsTechExamineCheckMethod& handle);
	MethodClass(JXJ, JXJTroopsTechExamineCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};

class crJXJTroopsTechAllFinishMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTroopsTechAllFinishMethod();
	crJXJTroopsTechAllFinishMethod(const crJXJTroopsTechAllFinishMethod& handle);
	MethodClass(JXJ, JXJTroopsTechAllFinish)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};

class crJXJTroopsTechAllFinishCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJTroopsTechAllFinishCheckMethod();
	crJXJTroopsTechAllFinishCheckMethod(const crJXJTroopsTechAllFinishCheckMethod& handle);
	MethodClass(JXJ, JXJTroopsTechAllFinishCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};

class crJXJRecvPassWorldFuBenCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPassWorldFuBenCountMethod();
	crJXJRecvPassWorldFuBenCountMethod(const crJXJRecvPassWorldFuBenCountMethod& handle);
	MethodClass(JXJ, JXJRecvPassWorldFuBenCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJMingJiangWorldPassCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMingJiangWorldPassCountMethod();
	crJXJMingJiangWorldPassCountMethod(const crJXJMingJiangWorldPassCountMethod& handle);
	MethodClass(JXJ, JXJMingJiangWorldPassCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_achieveid1;
	int m_achieveid2;
	int m_achieveid3;
	CRCore::crVector3i m_achieveid4;
};

class crJXJMingJiangWorldPassCountCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJMingJiangWorldPassCountCheckMethod();
	crJXJMingJiangWorldPassCountCheckMethod(const crJXJMingJiangWorldPassCountCheckMethod& handle);
	MethodClass(JXJ, JXJMingJiangWorldPassCountCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_achieveid1;
	int m_achieveid2;
	int m_achieveid3;
	CRCore::crVector3i m_achieveid4;
};

class crJXJMingJiangFinishMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMingJiangFinishMethod();
	crJXJMingJiangFinishMethod(const crJXJMingJiangFinishMethod& handle);
	MethodClass(JXJ, JXJMingJiangFinish)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};

class crJXJMingJiangFinishCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJMingJiangFinishCheckMethod();
	crJXJMingJiangFinishCheckMethod(const crJXJMingJiangFinishCheckMethod& handle);
	MethodClass(JXJ, JXJMingJiangFinishCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};

class crJXJContinueFinishHuYueDuMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJContinueFinishHuYueDuMethod();
	crJXJContinueFinishHuYueDuMethod(const crJXJContinueFinishHuYueDuMethod& handle);
	MethodClass(JXJ, JXJContinueFinishHuYueDu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_canvas;
};

class crJXJZiYuanCheArriveAddPointMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJZiYuanCheArriveAddPointMethod();
	crJXJZiYuanCheArriveAddPointMethod(const crJXJZiYuanCheArriveAddPointMethod& handle);
	MethodClass(JXJ, JXJZiYuanCheArriveAddPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveShopBuyMethod
//成就商店购买物品
//
//////////////////////////////////////////////////////////////////////////
class crJXJAchieveShopBuyMethod:public CRCore::crMethod
{//Client 
public:
	crJXJAchieveShopBuyMethod();
	crJXJAchieveShopBuyMethod(const crJXJAchieveShopBuyMethod& handle);
	MethodClass(JXJ,JXJAchieveShopBuy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_scrollbar;
	int                   m_index;
};

//////////////////////////////////////////
//
//crJXJRecvAchieveShopBuyMethod
//
//////////////////////////////////////////
class crJXJRecvAchieveShopBuyMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvAchieveShopBuyMethod();
	crJXJRecvAchieveShopBuyMethod(const crJXJRecvAchieveShopBuyMethod &handle);
	MethodClass(JXJ,JXJRecvAchieveShopBuy)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	//std::string m_animation;
	//std::string m_sequence;
};

/////////////////////////////////////////
//
//crJXJChengjiuShopScrollInitMethod
//
//
/////////////////////////////////////////
class crJXJChengjiuShopScrollInitMethod:public CRCore::crMethod
{
public:
	crJXJChengjiuShopScrollInitMethod();
	crJXJChengjiuShopScrollInitMethod(const crJXJChengjiuShopScrollInitMethod& handle);
	MethodClass(JXJ, JXJChengjiuShopScrollInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_scrollbar;
};

class crJXJGuoZhanAchieveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGuoZhanAchieveMethod();
	crJXJGuoZhanAchieveMethod(const crJXJGuoZhanAchieveMethod& handle);
	MethodClass(JXJ, JXJGuoZhanAchieve)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
};

class crJXJGuoZhanAchieveCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGuoZhanAchieveCheckMethod();
	crJXJGuoZhanAchieveCheckMethod(const crJXJGuoZhanAchieveCheckMethod& handle);
	MethodClass(JXJ, JXJGuoZhanAchieveCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
};
class crJXJFoundCountryCanvasUpdateNewMethod :public CRCore::crMethod
{//Client  Method		建国系统更新canvas
public:
	crJXJFoundCountryCanvasUpdateNewMethod ();
	crJXJFoundCountryCanvasUpdateNewMethod (const crJXJFoundCountryCanvasUpdateNewMethod & handle);
	MethodClass(JXJ, JXJFoundCountryCanvasUpdateNew)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_pageSw;
	std::string m_citybckSw[3][8];//左边地图上的
	std::string m_citySw[3][8];//右边城，0已占领，1未占领
	std::string m_BtnSw[8];
	std::string m_goalInput1;
	std::string m_goalInput2;
	std::string m_goalInput3;
	std::string m_getAllAwardBtn;
	std::string m_goToGuozhanBtn;
	std::string m_ditu;
	std::string m_icon[16];
	std::string m_num[16];
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJSetIsClickHuoDongSignMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJSetIsClickHuoDongSignMethod:public CRCore::crMethod
{//Client 
public:
	crJXJSetIsClickHuoDongSignMethod();
	crJXJSetIsClickHuoDongSignMethod(const crJXJSetIsClickHuoDongSignMethod& handle);
	MethodClass(JXJ,JXJSetIsClickHuoDongSign)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static unsigned char getIsclickhudongbtn(int i){return s_isclickhudongbtn[i];}
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_canvas;
	int                   m_index;
	static unsigned char s_isclickhudongbtn[2];
};

//class crJXJChengjiuButtonReClickMethod:public CRCore::crMethod
//{
//public:
//	crJXJChengjiuButtonReClickMethod();
//	crJXJChengjiuButtonReClickMethod(const crJXJChengjiuButtonReClickMethod& handle);
//	MethodClass(JXJ, JXJChengjiuButtonReClick)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//	std::string m_canvas;
//	std::string m_Button[11];
//	int m_index;
//	//CREncapsulation::crTableIO *accomplish_table;
//	int achievementTabInfo(CREncapsulation::crTableIO *accomplish_table);
//};

class crJXJRecvAchievePointMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvAchievePointMethod();
	crJXJRecvAchievePointMethod(const crJXJRecvAchievePointMethod& handle);
	MethodClass(JXJ, JXJRecvAchievePoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_achievepoint;
};
class crJXJClientRecvAchievePointMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvAchievePointMethod();
	crJXJClientRecvAchievePointMethod(const crJXJClientRecvAchievePointMethod& handle);
	MethodClass(JXJ, JXJClientRecvAchievePoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJListCtrlScrollExMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJListCtrlScrollExMethod();
	crJXJListCtrlScrollExMethod(const crJXJListCtrlScrollExMethod& handle);
	MethodClass(JXJ, JXJListCtrlScrollEx)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_srollbar1;
	std::string m_srollbar2;
	unsigned char m_type;
};

class crJXJUIAchieveShopCommodityTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIAchieveShopCommodityTipsMethod();
	crJXJUIAchieveShopCommodityTipsMethod(const crJXJUIAchieveShopCommodityTipsMethod& handle);
	MethodClass(JXJ, JXJUIAchieveShopCommodityTips)
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
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_imageindex;
};

class crJXJXinShouGuoZhanJieSuanMethod :public CRCore::crMethod
{//GameServer Method
public:

	crJXJXinShouGuoZhanJieSuanMethod ();
	crJXJXinShouGuoZhanJieSuanMethod (const crJXJXinShouGuoZhanJieSuanMethod & handle);
	MethodClass(JXJ,JXJXinShouGuoZhanJieSuan)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_playerid;
	int m_fubenid;
	//float m_circle;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//int m_fuBenId;
};

class crJXJRecvXinShouGuoZhanJieSuanMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJRecvXinShouGuoZhanJieSuanMethod();
	crJXJRecvXinShouGuoZhanJieSuanMethod(const crJXJRecvXinShouGuoZhanJieSuanMethod& handle);
	MethodClass(JXJ, JXJRecvXinShouGuoZhanJieSuan)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_frameTitle;
	std::string m_settleList;
	//BattleSettleMap m_BattleSettleMapWinner;
	//BattleSettleMap m_BattleSettleMapLoser;
	std::string m_Title1;
	std::string m_Title2;
	std::string m_Title3;
	std::string m_Title4;
	std::string m_count1;
	std::string m_count2;
	std::string m_count3;
	std::string m_count4;
	std::string m_switch;
	float m_charactersize;
	float m_sizex,m_sizey;
	float m_linespacing;
	float m_underlinespacing;
	float m_uplinespacing;
	std::string m_list[5];
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckCanvasStatusOfShowMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJCheckCanvasStatusOfShowMethod:public CRCore::crMethod
{//Client 
public:
	crJXJCheckCanvasStatusOfShowMethod();
	crJXJCheckCanvasStatusOfShowMethod(const crJXJCheckCanvasStatusOfShowMethod& handle);
	MethodClass(JXJ,JXJCheckCanvasStatusOfShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_canvas;
	//int                   m_index;
};


//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveButtonShowMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJAchieveButtonShowMethod:public CRCore::crMethod
{//Client 
public:
	crJXJAchieveButtonShowMethod();
	crJXJAchieveButtonShowMethod(const crJXJAchieveButtonShowMethod& handle);
	MethodClass(JXJ,JXJAchieveButtonShow)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_canvas;
	int                   m_index;
	std::string           m_scroll;
	std::string           m_chatCanvas;
	std::string           m_chatInput;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
	//int                   m_index;
};

//////////////////////////////////////////////////////////////////////////
//crJXJShowAchieveLinkClickMethod
//////////////////////////////////////////////////////////////////////////
class crJXJShowAchieveLinkClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJShowAchieveLinkClickMethod();
	crJXJShowAchieveLinkClickMethod(const crJXJShowAchieveLinkClickMethod& handle);
	MethodClass(JXJ, JXJShowAchieveLinkClick)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	CRUI::crHypertextWidgetNode::LinkInfoPair *m_linkInfo;
	std::string m_achievetips;
	std::string m_icon;
	std::string m_achievename;
	std::string m_achievemiaoshu;
	std::string m_achievejindu;
	std::string m_achievepoints;
	int m_achieveid;
	int m_completestep;
	std::string m_playername;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveShopBuyButtonTipsMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJAchieveShopBuyButtonTipsMethod:public CRCore::crMethod
{//Client 
public:
	crJXJAchieveShopBuyButtonTipsMethod();
	crJXJAchieveShopBuyButtonTipsMethod(const crJXJAchieveShopBuyButtonTipsMethod& handle);
	MethodClass(JXJ,JXJAchieveShopBuyButtonTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_tipcanvas;
	std::string           m_tiptext;
	std::string           m_shopscroll;
	int                   m_index;
	std::string           m_tipcanvas2;
	std::string           m_tiptext2;
};

class crJXJUISetFuBenScrollValueMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetFuBenScrollValueMethod();
	crJXJUISetFuBenScrollValueMethod(const crJXJUISetFuBenScrollValueMethod& handle);
	MethodClass(JXJ, JXJUISetFuBenScrollValue)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_scroll;
	//unsigned int m_index;
	//std::string m_radiogroup_2;
};

class crJXJSetNewGuoZhanBirthPointMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetNewGuoZhanBirthPointMethod();
	crJXJSetNewGuoZhanBirthPointMethod(const crJXJSetNewGuoZhanBirthPointMethod& handle);
	MethodClass(JXJ, JXJSetNewGuoZhanBirthPoint)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryCountryStrengthMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJQueryCountryStrengthMethod:public CRCore::crMethod
{//Client 
public:
	crJXJQueryCountryStrengthMethod();
	crJXJQueryCountryStrengthMethod(const crJXJQueryCountryStrengthMethod& handle);
	MethodClass(JXJ,JXJQueryCountryStrength)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_scrollbar;
	int                   m_index;
};

//////////////////////////////////////////
//
//crJXJRecvCountryStrengthMethod
//
//////////////////////////////////////////
class crJXJRecvCountryStrengthMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvCountryStrengthMethod();
	crJXJRecvCountryStrengthMethod(const crJXJRecvCountryStrengthMethod &handle);
	MethodClass(JXJ,JXJRecvCountryStrength)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	//std::string m_animation;
	//std::string m_sequence;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJLunHuiCanvasUpdateNewMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJLunHuiCanvasUpdateNewMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLunHuiCanvasUpdateNewMethod();
	crJXJLunHuiCanvasUpdateNewMethod(const crJXJLunHuiCanvasUpdateNewMethod& handle);
	MethodClass(JXJ, JXJLunHuiCanvasUpdateNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode * m_this;
	std::string m_strRadioGroup;
	std::string m_strSwitch;

	std::string m_strCardQuality[10];
	std::string m_strCardImage[10];
	std::string m_strItemImage[10];
	std::string m_strCardStatus[10];
	std::string m_strItemStatus[10];

	std::string m_stritemnote;
	std::string m_strcardnote;
	std::string m_strgoldnote;
	std::string m_strlijinnote;

	std::string m_strgoldimage;
	std::string m_strlijinimage;
	std::string m_strgoldcount;
	std::string m_strlijincount;
	std::string m_strgoldtext;
	std::string m_strlijintext;

	std::string m_strDefaultCardImg;
	std::string m_strDefaultItemImg;

	std::string m_cunchucountcard;
	std::string m_cunchucountitem;
	//std::string m_strlist;

};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckLunHuiDaoJuCountMethod
//检查背包里是否有建筑令/征募令/召集令
//
//////////////////////////////////////////////////////////////////////////
class crJXJUICheckLunHuiDaoJuCountMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUICheckLunHuiDaoJuCountMethod();
	crJXJUICheckLunHuiDaoJuCountMethod(const crJXJUICheckLunHuiDaoJuCountMethod& handle);
	MethodClass(JXJ,JXJUICheckLunHuiDaoJuCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int         m_itemid;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJConfirmUseLunHuiDaoJuMethod
//轮回道具符使用确认
//
//////////////////////////////////////////////////////////////////////////
class crJXJConfirmUseLunHuiDaoJuMethod:public CRCore::crMethod
{//Client 
public:
	crJXJConfirmUseLunHuiDaoJuMethod();
	crJXJConfirmUseLunHuiDaoJuMethod(const crJXJConfirmUseLunHuiDaoJuMethod& handle);
	MethodClass(JXJ,JXJConfirmUseLunHuiDaoJu)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int         m_index;
};

//////////////////////////////////////////
//
//crJXJRecvConfirmUseLunHuiDaoJuMethod
//
//////////////////////////////////////////
class crJXJRecvConfirmUseLunHuiDaoJuMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvConfirmUseLunHuiDaoJuMethod();
	crJXJRecvConfirmUseLunHuiDaoJuMethod(const crJXJRecvConfirmUseLunHuiDaoJuMethod &handle);
	MethodClass(JXJ,JXJRecvConfirmUseLunHuiDaoJu)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_strCanvas;
	std::string m_strInput;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIQuChuLunHuiThingMethod
//取出轮回物品
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIQuChuLunHuiThingMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIQuChuLunHuiThingMethod();
	crJXJUIQuChuLunHuiThingMethod(const crJXJUIQuChuLunHuiThingMethod& handle);
	MethodClass(JXJ,JXJUIQuChuLunHuiThing)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

//////////////////////////////////////////
//
//crJXJRecvQuChuLunHuiTingMethod
//
//////////////////////////////////////////
class crJXJRecvQuChuLunHuiTingMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvQuChuLunHuiTingMethod();
	crJXJRecvQuChuLunHuiTingMethod(const crJXJRecvQuChuLunHuiTingMethod &handle);
	MethodClass(JXJ,JXJRecvQuChuLunHuiTing)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckCharacterLevelMethod
//检查领主等级
//
//////////////////////////////////////////////////////////////////////////
class crJXJUICheckCharacterLevelMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUICheckCharacterLevelMethod();
	crJXJUICheckCharacterLevelMethod(const crJXJUICheckCharacterLevelMethod& handle);
	MethodClass(JXJ,JXJUICheckCharacterLevel)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

//////////////////////////////////////////////////////////////////////////
//crJXJAdvanceLunHuiRePlaceCountMethod
//////////////////////////////////////////////////////////////////////////
class crJXJAdvanceLunHuiRePlaceCountMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAdvanceLunHuiRePlaceCountMethod();
	crJXJAdvanceLunHuiRePlaceCountMethod(const crJXJAdvanceLunHuiRePlaceCountMethod& handle);
	MethodClass(JXJ, JXJAdvanceLunHuiRePlaceCount)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode * m_this;
	std::string m_strCanvas;
	//std::string m_strJKSwitch[6];
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJGetViplvRewardMethod
//领取vip等级礼包
//
//////////////////////////////////////////////////////////////////////////
class crJXJGetViplvRewardMethod:public CRCore::crMethod
{//Client 
public:
	crJXJGetViplvRewardMethod();
	crJXJGetViplvRewardMethod(const crJXJGetViplvRewardMethod& handle);
	MethodClass(JXJ,JXJGetViplvReward)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

//////////////////////////////////////////
//
//crJXJRecvGetViplvRewardMethod
//
//////////////////////////////////////////
class crJXJRecvGetViplvRewardMethod:public CRCore::crMethod
{//Game Method
public:
	crJXJRecvGetViplvRewardMethod();
	crJXJRecvGetViplvRewardMethod(const crJXJRecvGetViplvRewardMethod &handle);
	MethodClass(JXJ,JXJRecvGetViplvReward)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJUIViplvRewardTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIViplvRewardTipsMethod();
	crJXJUIViplvRewardTipsMethod(const crJXJUIViplvRewardTipsMethod& handle);
	MethodClass(JXJ, JXJUIViplvRewardTips)
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
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_imageindex;
};

class crJXJUIShuoMingTipInfoNewMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShuoMingTipInfoNewMethod();
	crJXJUIShuoMingTipInfoNewMethod(const crJXJUIShuoMingTipInfoNewMethod& handle);
	MethodClass(JXJ, JXJUIShuoMingTipInfoNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_header;
	std::string m_trunk;
	std::string m_title;
	std::string m_text;
	std::string m_textbk;
	std::string m_textdi;
	int m_titleid;
	int m_textid;
};

/////////////////////////////////////////
//
//crJXJServerUseRefineItemMethod
//
/////////////////////////////////////////
class crJXJServerUseRefineItemMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseRefineItemMethod();
	crJXJServerUseRefineItemMethod(const crJXJServerUseRefineItemMethod& handle);
	MethodClass(JXJ, JXJServerUseRefineItem)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
	CRCore::crVector3i m_stoneid;
};

class crJXJWorldRankRewardImageTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJWorldRankRewardImageTipsMethod();
	crJXJWorldRankRewardImageTipsMethod(const crJXJWorldRankRewardImageTipsMethod& handle);
	MethodClass(JXJ, JXJWorldRankRewardImageTips)
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
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	CRCore::crVector2i m_imageindex;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJEquipeRefineAttributeTipMethod
//装备精炼属性提示
//
//////////////////////////////////////////////////////////////////////////
class crJXJEquipeRefineAttributeTipMethod:public CRCore::crMethod
{//Client 
public:
	crJXJEquipeRefineAttributeTipMethod();
	crJXJEquipeRefineAttributeTipMethod(const crJXJEquipeRefineAttributeTipMethod& handle);
	MethodClass(JXJ,JXJEquipeRefineAttributeTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tips2;
	std::string m_tipstext;
	std::string m_name;

};

//////////////////////////////////////////////////////////////////////////
//
//crJXJDetachCardOrItemForSavingMethod
//取下将要存储的将卡或装备
//
//////////////////////////////////////////////////////////////////////////
class crJXJDetachCardOrItemForSavingMethod:public CRCore::crMethod
{//Client 
public:
	crJXJDetachCardOrItemForSavingMethod();
	crJXJDetachCardOrItemForSavingMethod(const crJXJDetachCardOrItemForSavingMethod& handle);
	MethodClass(JXJ,JXJDetachCardOrItemForSaving)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_jiangkacanvas;
	std::string m_text;

};


//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeWindowPopMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJCustomContributeWindowPopMethod:public CRCore::crMethod
{//Client 
public:
	crJXJCustomContributeWindowPopMethod();
	crJXJCustomContributeWindowPopMethod(const crJXJCustomContributeWindowPopMethod& handle);
	MethodClass(JXJ,JXJCustomContributeWindowPop)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tips2;
	std::string m_tipstext;
	std::string m_name;

};

//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeBtnTipMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJCustomContributeBtnTipMethod:public CRCore::crMethod
{//Client 
public:
	crJXJCustomContributeBtnTipMethod();
	crJXJCustomContributeBtnTipMethod(const crJXJCustomContributeBtnTipMethod& handle);
	MethodClass(JXJ,JXJCustomContributeBtnTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips2;
	std::string m_tipstext;

};

//////////////////////////////////////////////////////////////////////////
//
//crJXJExcuteCustomContributeMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJExcuteCustomContributeMethod:public CRCore::crMethod
{//Client 
public:
	crJXJExcuteCustomContributeMethod();
	crJXJExcuteCustomContributeMethod(const crJXJExcuteCustomContributeMethod& handle);
	MethodClass(JXJ,JXJExcuteCustomContribute)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio;
	std::string m_input;
};

class crJXJUICheckContributeInputNumMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICheckContributeInputNumMethod();
	crJXJUICheckContributeInputNumMethod(const crJXJUICheckContributeInputNumMethod& handle);
	MethodClass(JXJ, JXJUICheckContributeInputNum)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJRefineButtonTipMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJRefineButtonTipMethod:public CRCore::crMethod
{//Client 
public:
	crJXJRefineButtonTipMethod();
	crJXJRefineButtonTipMethod(const crJXJRefineButtonTipMethod& handle);
	MethodClass(JXJ,JXJRefineButtonTip)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tipstext;
	int m_index;

};

//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeAddOrReduceMethod
//
//
//////////////////////////////////////////////////////////////////////////
class crJXJCustomContributeAddOrReduceMethod:public CRCore::crMethod
{//Client 
public:
	crJXJCustomContributeAddOrReduceMethod();
	crJXJCustomContributeAddOrReduceMethod(const crJXJCustomContributeAddOrReduceMethod& handle);
	MethodClass(JXJ,JXJCustomContributeAddOrReduce)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_input;
	int m_index;

};

}
#endif