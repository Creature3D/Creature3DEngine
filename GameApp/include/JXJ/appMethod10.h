/**********************************************************************
*
*	文件:	   appMethod10.h
*
*	描述:	   
*
*	作者:	   刘超
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD10_H
#define JXJ_APPMETHOD10_H
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
class crJXJUIChooseCheckBoxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIChooseCheckBoxMethod();
	crJXJUIChooseCheckBoxMethod(const crJXJUIChooseCheckBoxMethod& handle);
	MethodClass(JXJ, JXJUIChooseCheckBox)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox[15];
	std::string m_checkbox_sw[15];
	std::string m_Recommend;
	std::string m_Recommend_sw;
};
class crJXJCountryRadioAddZhouJunNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryRadioAddZhouJunNameMethod();
	crJXJCountryRadioAddZhouJunNameMethod(const crJXJCountryRadioAddZhouJunNameMethod& handle);
	MethodClass(JXJ, JXJCountryRadioAddZhouJunName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_radio;
	std::string m_zhoujunname[20];
};


class crJXJCountryCantonFenFengUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryCantonFenFengUpdateMethod();
	crJXJCountryCantonFenFengUpdateMethod(const crJXJCountryCantonFenFengUpdateMethod& handle);
	MethodClass(JXJ, JXJCountryCantonFenFengUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_countryCanvas;
	std::string m_tableWidget;
	std::string m_info[2][9];						// 界面特殊，只有9行
	std::string m_fenfeng[9];
	std::string m_jiechu[9];
	std::string m_fenfeng_sw[9];
	std::string m_jiechu_sw[9];
	std::string m_pageNum;
	std::string m_notice;
	std::string m_title;
};

class crJXJHistoryInfoChangeCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHistoryInfoChangeCanvasUpdateMethod();
	crJXJHistoryInfoChangeCanvasUpdateMethod(const crJXJHistoryInfoChangeCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJHistoryInfoChangeCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_canvas;
	std::string m_radio;
	std::string m_history;
};

class crJXJBuildInfoChangeCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildInfoChangeCanvasUpdateMethod();
	crJXJBuildInfoChangeCanvasUpdateMethod(const crJXJBuildInfoChangeCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJBuildInfoChangeCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_canvas;
	std::string m_radio;
	std::string m_explain;
	std::string m_percent;
	std::string m_buttonBuiding;
	std::string m_image_sw;
};

class crJXJMyCounrtyInfoCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyCounrtyInfoCanvasUpdateMethod();
	crJXJMyCounrtyInfoCanvasUpdateMethod(const crJXJMyCounrtyInfoCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyCounrtyInfoCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_countryiconsw; 
	std::string m_EditNotice;//编辑公告
	std::string m_PublishNotice;//发布公告
	std::string m_leadername1;  
	std::string m_leadername2;  
	std::string m_leadername3; 
	std::string m_nextopentime; 
	std::string m_getcity[12];
	std::string m_getcity_sw[12];
	std::string m_losecity[12];
	std::string m_notice; 
	std::string m_donghua[12];
	std::string m_getCityControl;
	std::string m_lostCityControl;
};
class crJXJBuildMyCountryPageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildMyCountryPageUpdateMethod();
	crJXJBuildMyCountryPageUpdateMethod(const crJXJBuildMyCountryPageUpdateMethod& handle);
	MethodClass(JXJ, JXJBuildMyCountryPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_add[5];
	std::string m_selectRadio;
	std::string m_mycontributeinput;//拥有国家贡献
	std::string m_store;
	std::string m_sijintai;
	std::string m_shenlongtai;
	std::string m_simajian;
	std::string m_lubanci;
	std::string m_dianjiangtai;
	std::string m_explain;
	std::string m_percent;
	std::string m_building;
	std::string m_add_text[5];
	std::string m_guojiazijin;
};
class crJXJMyLeaderManagerCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyLeaderManagerCanvasUpdateMethod();
	crJXJMyLeaderManagerCanvasUpdateMethod(const crJXJMyLeaderManagerCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyLeaderManagerCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_PrimeMinister;//a丞相
	std::string m_GreatMinister;//大司马
	std::string m_Daisyougun;//大将军
	std::string m_Obligation;//官员职责
	std::string m_Rule;//选举规则
	std::string m_Crops;//信息列表
	std::string m_Select[15];
	std::string m_Recommend;//选举按钮
	std::string m_checkbox_sw[15];

};
class crJXJMyZhoujunManagerCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyZhoujunManagerCanvasUpdateMethod();
	crJXJMyZhoujunManagerCanvasUpdateMethod(const crJXJMyZhoujunManagerCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyZhoujunManagerCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_Country_State[20];
	std::string m_Country_State_sw[20];
	std::string m_Country_newState_sw[20];//镇守驻扎图标
	std::string m_Country_zhoujun;//州郡
	std::string m_Country_ZhouMu;//州牧
	std::string m_Country_Corps;//镇守军团
	std::string m_Country_Enfeoff;//州郡分封
	std::string m_Country_Apply;//驻扎申请
	std::string m_Country_Affiliation;//郡属
	std::string m_Country_CityName;//城池名称
	std::string m_Country_Cash;//铜钱
	std::string m_Country_Horse;//马匹
	std::string m_Country_Material;//兵种材料
	std::string m_Country_GarrisonCorps;//驻守军团
	std::string m_Country_Satrap;//城池太守
	std::string m_Country_ListScroll;//
	std::string m_Country_Bar;//
	std::string m_CantonRadio;
	std::string m_chengChiList;
	std::string m_Country_Enfeoff_sw;
	std::string m_Country_Apply_sw;
	std::string m_Battleground_map;
	std::string m_Country_Cancle;
};
//class crJXJMyCounrtyShopCanvasUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJMyCounrtyShopCanvasUpdateMethod();
//	crJXJMyCounrtyShopCanvasUpdateMethod(const crJXJMyCounrtyShopCanvasUpdateMethod& handle);
//	MethodClass(JXJ, JXJMyCounrtyShopCanvasUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this; 
//	std::string m_Country_Image[5];
//	std::string m_Country_Name[5];
//	std::string m_Country_Explain[5];
//	std::string m_Country_Position[5];
//	std::string m_Country_Number[5];
//	std::string m_Country_Category[5];
//	std::string m_Country_Purchase[5];
//	std::string m_Country_LeftPage;
//	std::string m_Country_PageNumber;
//	std::string m_Country_RightPage;
//	std::string m_Country_Select;
//	std::string m_Country_Contribute;
//	std::string m_Country_shopRadio;
//	std::string m_ShopRaido_guanZhi;
//	std::string m_ShopRadio_shenMi;
//};
class crJXJMyCounrtyFuliCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyCounrtyFuliCanvasUpdateMethod();
	crJXJMyCounrtyFuliCanvasUpdateMethod(const crJXJMyCounrtyFuliCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyCounrtyFuliCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_sw111_12;
	std::string m_sw111_13;
	std::string m_Country7_Choose1;
	std::string m_Country7_Fete;
	std::string m_Country7_Worship;
	std::string m_Country7_Choose2;
	std::string m_Country7_Economy;
	std::string m_Country7_Interface2;
	std::string m_Country7_Function1;
	std::string m_Country7_Function2;
	std::string m_Country7_Function3;
};
class crJXJMyCounrtyHistoryCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyCounrtyHistoryCanvasUpdateMethod();
	crJXJMyCounrtyHistoryCanvasUpdateMethod(const crJXJMyCounrtyHistoryCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyCounrtyHistoryCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_CountryRadio;//国家历史切换
	std::string m_CountryRadioButton1;//大事件
	std::string m_CountryRadioButton2;//官员变迁
	std::string m_CountryRadioButton3;//国家福利
	std::string m_CountryRadioButton4;//城池
	std::string m_CountryRadioButton5;//州牧任免
	std::string m_CountryRadioButton6;//国家建设
	std::string m_CountryHistory;
	std::string m_scroll;
};

class crJXJBuildMyCountryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildMyCountryMethod();
	crJXJBuildMyCountryMethod(const crJXJBuildMyCountryMethod& handle);
	MethodClass(JXJ, JXJBuildMyCountry)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio;
};
class crJXJCountryEditNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryEditNoticeMethod();
	crJXJCountryEditNoticeMethod(const crJXJCountryEditNoticeMethod& handle);
	MethodClass(JXJ, JXJCountryEditNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_notice;
	std::string m_noticeEdit;
	std::string m_noticePublish;
};

class crJXJCountryPublishNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryPublishNoticeMethod();
	crJXJCountryPublishNoticeMethod(const crJXJCountryPublishNoticeMethod& handle);
	MethodClass(JXJ, JXJCountryPublishNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_notice;
	std::string m_noticeEdit;
	std::string m_noticePublish;
};
class crJXJSelectCountryLeaderMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSelectCountryLeaderMethod();
	crJXJSelectCountryLeaderMethod(const crJXJSelectCountryLeaderMethod& handle);
	MethodClass(JXJ, JXJSelectCountryLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox[15];
	std::string m_Recommend;
	std::string m_tableWidget;
};

class crJXJCounrtyShopLeftPageCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCounrtyShopLeftPageCanvasUpdateMethod();
	crJXJCounrtyShopLeftPageCanvasUpdateMethod(const crJXJCounrtyShopLeftPageCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJCounrtyShopLeftPageCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_canvas;
	std::string m_Country_Image[5];
	std::string m_Country_Name[5];
	std::string m_Country_Explain[5];
	std::string m_Country_Position[5];
	std::string m_Country_Number[5];
	std::string m_Country_Category[5];
	std::string m_Country_Purchase[5];
	std::string m_Country_LeftPage;
	std::string m_Country_PageNumber;
	std::string m_Country_RightPage;
	std::string m_Country_Select;
};

class crJXJCounrtyShopRightPageCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCounrtyShopRightPageCanvasUpdateMethod();
	crJXJCounrtyShopRightPageCanvasUpdateMethod(const crJXJCounrtyShopRightPageCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJCounrtyShopRightPageCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_canvas;
	std::string m_Country_Image[5];
	std::string m_Country_Name[5];
	std::string m_Country_Explain[5];
	std::string m_Country_Position[5];
	std::string m_Country_Number[5];
	std::string m_Country_Category[5];
	std::string m_Country_Purchase[5];
	std::string m_Country_LeftPage;
	std::string m_Country_PageNumber;
	std::string m_Country_RightPage;
	std::string m_Country_Select;
};
class crJXJRecvModifyCountryNoticeMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvModifyCountryNoticeMethod();
	crJXJRecvModifyCountryNoticeMethod(const crJXJRecvModifyCountryNoticeMethod& handle);
	MethodClass(JXJ,JXJRecvModifyCountryNotice)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJQueryCountryHistoryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryHistoryMethod();
	crJXJQueryCountryHistoryMethod(const crJXJQueryCountryHistoryMethod& handle);
	MethodClass(JXJ, JXJQueryCountryHistory)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJQueryCountryToupiaoInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryToupiaoInfoMethod();
	crJXJQueryCountryToupiaoInfoMethod(const crJXJQueryCountryToupiaoInfoMethod& handle);
	MethodClass(JXJ, JXJQueryCountryToupiaoInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJQueryCountryWillBeLeaderMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryWillBeLeaderMethod();
	crJXJQueryCountryWillBeLeaderMethod(const crJXJQueryCountryWillBeLeaderMethod& handle);
	MethodClass(JXJ, JXJQueryCountryWillBeLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJRecvCountryHistoryMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCountryHistoryMethod();
	crJXJRecvCountryHistoryMethod(const crJXJRecvCountryHistoryMethod& handle);
	MethodClass(JXJ,JXJRecvCountryHistory)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJRecvToupiaoMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvToupiaoMethod();
	crJXJRecvToupiaoMethod(const crJXJRecvToupiaoMethod& handle);
	MethodClass(JXJ,JXJRecvToupiao)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};
class crJXJRecvCountryWillBeLeaderMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCountryWillBeLeaderMethod();
	crJXJRecvCountryWillBeLeaderMethod(const crJXJRecvCountryWillBeLeaderMethod& handle);
	MethodClass(JXJ,JXJRecvCountryWillBeLeader)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJQueryMyCountryInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyCountryInfoMethod();
	crJXJQueryMyCountryInfoMethod(const crJXJQueryMyCountryInfoMethod& handle);
	MethodClass(JXJ, JXJQueryMyCountryInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJRecvQueryMyCountryInfoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyCountryInfoMethod();
	crJXJRecvQueryMyCountryInfoMethod(const crJXJRecvQueryMyCountryInfoMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyCountryInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};
class crJXJUIBuildMyCountryMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildMyCountryMethod();
	crJXJUIBuildMyCountryMethod(const crJXJUIBuildMyCountryMethod& handle);
	MethodClass(JXJ, JXJUIBuildMyCountry)
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
class crJXJRecvBuildMyCountryMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuildMyCountryMethod();
	crJXJRecvBuildMyCountryMethod(const crJXJRecvBuildMyCountryMethod& handle);
	MethodClass(JXJ, JXJRecvBuildMyCountry)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJMyCounrtyCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyCounrtyCanvasUpdateMethod();
	crJXJMyCounrtyCanvasUpdateMethod(const crJXJMyCounrtyCanvasUpdateMethod& handle);
	MethodClass(JXJ, JXJMyCounrtyCanvasUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;  
	std::string m_pageradio; 
	std::string m_pagesw; 
};
class crJXJOnCountryBuildingPageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOnCountryBuildingPageMethod();
	crJXJOnCountryBuildingPageMethod(const crJXJOnCountryBuildingPageMethod& handle);
	MethodClass(JXJ, JXJOnCountryBuildingPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJRecvOnCountryBuildingPageMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOnCountryBuildingPageMethod();
	crJXJRecvOnCountryBuildingPageMethod(const crJXJRecvOnCountryBuildingPageMethod& handle);
	MethodClass(JXJ, JXJRecvOnCountryBuildingPage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQueryCountryBuildingDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryCountryBuildingDisposeMethod();
	crJXJQueryCountryBuildingDisposeMethod(const crJXJQueryCountryBuildingDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryCountryBuildingDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_validterm;//
	typedef std::map< CRCore::crVector2i,std::pair<CRCore::ref_ptr<CRCore::crStreamBuf>,float> > GroupRecordMap;//startid,count
	GroupRecordMap m_groupRecordMap;
	GroupRecordMap m_mygroupRecordMap;
	float *m_dt;
};

class crJXJCountryCantonFenFengMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryCantonFenFengMethod ();
	crJXJCountryCantonFenFengMethod (const crJXJCountryCantonFenFengMethod & handle);
	MethodClass(JXJ, JXJCountryCantonFenFeng)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_tabelWidget;
	unsigned char m_idx;
	unsigned char m_op;					// 0 解除，1 分封
};


class crJXJCountrySetSelectedCantonIdMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountrySetSelectedCantonIdMethod ();
	crJXJCountrySetSelectedCantonIdMethod (const crJXJCountrySetSelectedCantonIdMethod & handle);
	MethodClass(JXJ, JXJCountrySetSelectedCantonId)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
};

class crJXJQueryMyCounrtyCantonFenFengGroupListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyCounrtyCantonFenFengGroupListMethod ();
	crJXJQueryMyCounrtyCantonFenFengGroupListMethod (const crJXJQueryMyCounrtyCantonFenFengGroupListMethod & handle);
	MethodClass(JXJ, JXJQueryMyCounrtyCantonFenFengGroupList)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
//	std::string m_groupRadio;
};

class crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod();
	crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod(const crJXJRecvQueryMyCounrtyCantonFenFengGroupListMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyCounrtyCantonFenFengGroupList)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
	std::string m_fenFengCanvas;
};


class crJXJRecvCountryCantonAllocateMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCountryCantonAllocateMethod();
	crJXJRecvCountryCantonAllocateMethod(const crJXJRecvCountryCantonAllocateMethod& handle);
	MethodClass(JXJ, JXJRecvCountryCantonAllocate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
	std::string m_cantonCanvas;
};

class crJXJRecvSelectCountryLeaderMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSelectCountryLeaderMethod();
	crJXJRecvSelectCountryLeaderMethod(const crJXJRecvSelectCountryLeaderMethod& handle);
	MethodClass(JXJ, JXJRecvSelectCountryLeader)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};

class crJXJUICheckBoxUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICheckBoxUpdateMethod ();
	crJXJUICheckBoxUpdateMethod (const crJXJUICheckBoxUpdateMethod & handle);
	MethodClass(JXJ, JXJUICheckBoxUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode*m_this;
	std::string m_checkbox[15];
	std::string m_checkbox_sw[15];
	std::string m_Recommend;
	std::string m_Recommend_sw;
};

class crJXJGenerateCountryOfficialMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGenerateCountryOfficialMethod ();
	crJXJGenerateCountryOfficialMethod (const crJXJGenerateCountryOfficialMethod & handle);
	MethodClass(JXJ, JXJGenerateCountryOfficial)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//time_t m_lasttimer;
};

class crJXJCountryCanvasApplyStationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryCanvasApplyStationMethod ();
	crJXJCountryCanvasApplyStationMethod (const crJXJCountryCanvasApplyStationMethod & handle);
	MethodClass(JXJ, JXJCountryCanvasApplyStation)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
};


class crJXJRecvJCountryCanvasApplyStationMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvJCountryCanvasApplyStationMethod();
	crJXJRecvJCountryCanvasApplyStationMethod(const crJXJRecvJCountryCanvasApplyStationMethod& handle);
	MethodClass(JXJ, JXJRecvJCountryCanvasApplyStation)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_myCountryCanvas;
};


class crJXJResetGroupRadioMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJResetGroupRadioMethod ();
	crJXJResetGroupRadioMethod (const crJXJResetGroupRadioMethod & handle);
	MethodClass(JXJ, JXJResetGroupRadio)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
	unsigned char m_val;
};

class crJXJInitGroupRadioMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJInitGroupRadioMethod ();
	crJXJInitGroupRadioMethod (const crJXJInitGroupRadioMethod & handle);
	MethodClass(JXJ, JXJInitGroupRadio)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
	std::string m_radio[20];
};


class crJXJQueryPeopleInfoListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryPeopleInfoListMethod ();
	crJXJQueryPeopleInfoListMethod (const crJXJQueryPeopleInfoListMethod & handle);
	MethodClass(JXJ, JXJQueryPeopleInfoList)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_lastMinlv;
	unsigned char m_lastMaxLv;
	std::string m_minLv;
	std::string m_maxLv;
	std::string m_page;
};


class crJXJRecvQueryPeopleInfoListMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryPeopleInfoListMethod();
	crJXJRecvQueryPeopleInfoListMethod(const crJXJRecvQueryPeopleInfoListMethod& handle);
	MethodClass(JXJ, JXJRecvQueryPeopleInfoList)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};


class crJXJRecvPeopleInfoListMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJRecvPeopleInfoListMethod();
	crJXJRecvPeopleInfoListMethod(const crJXJRecvPeopleInfoListMethod& handle);
	MethodClass(JXJ, JXJRecvPeopleInfoList)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};

class crJXJQueryPeopleInfoDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryPeopleInfoDisposeMethod();
	crJXJQueryPeopleInfoDisposeMethod(const crJXJQueryPeopleInfoDisposeMethod& handle);
	MethodClass(JXJ, JXJQueryPeopleInfoDispose)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_count;
};


class crJXJUISmallAccUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISmallAccUpdateMethod ();
	crJXJUISmallAccUpdateMethod (const crJXJUISmallAccUpdateMethod & handle);
	MethodClass(JXJ, JXJUISmallAccUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode*m_this;
	std::string m_shili;
	std::string m_filter;
	std::string m_query;
	std::string m_table;
	std::string m_pageInput;
	std::string m_confirm;
	std::string m_checkbox[9];
};

class crJXJUIConfirmSmallAccMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIConfirmSmallAccMethod ();
	crJXJUIConfirmSmallAccMethod (const crJXJUIConfirmSmallAccMethod & handle);
	MethodClass(JXJ, JXJUIConfirmSmallAcc)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_checkbox[9];
	std::string m_canvas;
};

class crJXJRecvUIConfirmSmallAccMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvUIConfirmSmallAccMethod();
	crJXJRecvUIConfirmSmallAccMethod(const crJXJRecvUIConfirmSmallAccMethod& handle);
	MethodClass(JXJ, JXJRecvUIConfirmSmallAcc)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_myCountryCanvas;
};

class crJXJQueryCountryBenefitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryBenefitMethod ();
	crJXJQueryCountryBenefitMethod (const crJXJQueryCountryBenefitMethod & handle);
	MethodClass(JXJ, JXJQueryCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJRecvQueryCountryBenefitMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryCountryBenefitMethod();
	crJXJRecvQueryCountryBenefitMethod(const crJXJRecvQueryCountryBenefitMethod& handle);
	MethodClass(JXJ, JXJRecvQueryCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};

class crJXJUICountryBenefitUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICountryBenefitUpdateMethod ();
	crJXJUICountryBenefitUpdateMethod (const crJXJUICountryBenefitUpdateMethod & handle);
	MethodClass(JXJ, JXJUICountryBenefitUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode*m_this;
	std::string m_fete;
	std::string m_enrich;
	std::string m_fete_sw;
	std::string m_enrich_sw;
	std::string m_doFete;
	std::string m_doFete_sw;
	std::string m_doEnrich[3];
	std::string m_doEnrich_sw[3];
	std::string m_describe_title[2];
	std::string m_describe_text[2];
};

class crJXJStartCountryBenefitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJStartCountryBenefitMethod ();
	crJXJStartCountryBenefitMethod (const crJXJStartCountryBenefitMethod & handle);
	MethodClass(JXJ, JXJStartCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_type;
};

class crJXJRecvStartCountryBenefitMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvStartCountryBenefitMethod();
	crJXJRecvStartCountryBenefitMethod(const crJXJRecvStartCountryBenefitMethod& handle);
	MethodClass(JXJ, JXJRecvStartCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};


class crJXJEnjoyCountryBenefitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJEnjoyCountryBenefitMethod ();
	crJXJEnjoyCountryBenefitMethod (const crJXJEnjoyCountryBenefitMethod & handle);
	MethodClass(JXJ, JXJEnjoyCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_type;
};

class crJXJRecvEnjoyCountryBenefitMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvEnjoyCountryBenefitMethod();
	crJXJRecvEnjoyCountryBenefitMethod(const crJXJRecvEnjoyCountryBenefitMethod& handle);
	MethodClass(JXJ, JXJRecvEnjoyCountryBenefit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
};

class crJXJCountryCanvasDataInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCountryCanvasDataInitMethod ();
	crJXJCountryCanvasDataInitMethod (const crJXJCountryCanvasDataInitMethod & handle);
	MethodClass(JXJ, JXJCountryCanvasDataInit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode*m_this;
};

class crJXJQueryCountryPeopleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryCountryPeopleMethod ();
	crJXJQueryCountryPeopleMethod (const crJXJQueryCountryPeopleMethod & handle);
	MethodClass(JXJ, JXJQueryCountryPeople)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_edit;
	std::string m_table;
	std::string m_checkbox[9];
};

class crJXJRecvQueryCountryPeopleMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryCountryPeopleMethod();
	crJXJRecvQueryCountryPeopleMethod(const crJXJRecvQueryCountryPeopleMethod& handle);
	MethodClass(JXJ, JXJRecvQueryCountryPeople)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_table;
	std::string m_checkbox[9];
};

class crJXJCountryVoteGMData; 
class crJXJGMOrderCountryVoteMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGMOrderCountryVoteMethod ();
	crJXJGMOrderCountryVoteMethod (const crJXJGMOrderCountryVoteMethod & handle);
	MethodClass(JXJ, JXJGMOrderCountryVote)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	crJXJCountryVoteGMData * m_gmData;
};


class crJXJCountryBenefitRefreshMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCountryBenefitRefreshMethod ();
	crJXJCountryBenefitRefreshMethod (const crJXJCountryBenefitRefreshMethod & handle);
	MethodClass(JXJ, JXJCountryBenefitRefresh)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJCountryCantonMapRefreshMethod:public CRCore::crMethod
{//GameServer Method
public:
	crJXJCountryCantonMapRefreshMethod();
	crJXJCountryCantonMapRefreshMethod(const crJXJCountryCantonMapRefreshMethod& handle);
	MethodClass(JXJ, JXJCountryCantonMapRefresh)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	unsigned char m_shiliId;
	int m_chengchiId;
};

class crJXJSYNChengChiToCantonMethod:public CRCore::crMethod
{//GameServer Method
public:
	crJXJSYNChengChiToCantonMethod();
	crJXJSYNChengChiToCantonMethod(const crJXJSYNChengChiToCantonMethod& handle);
	MethodClass(JXJ, JXJSYNChengChiToCanton)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};


class crJXJSmallAccCheckBoxUpdateMethod:public CRCore::crMethod
{//GameClient Method
public:
	crJXJSmallAccCheckBoxUpdateMethod();
	crJXJSmallAccCheckBoxUpdateMethod(const crJXJSmallAccCheckBoxUpdateMethod& handle);
	MethodClass(JXJ, JXJSmallAccCheckBoxUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_checkBox;
};

class crJXJStaticTextIsNumberCheckMethod:public CRCore::crMethod
{//GameClient Method
public:
	crJXJStaticTextIsNumberCheckMethod();
	crJXJStaticTextIsNumberCheckMethod(const crJXJStaticTextIsNumberCheckMethod& handle);
	MethodClass(JXJ, JXJStaticTextIsNumberCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_staticText;
};


//class crJXJUICountryShopItemTipsMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUICountryShopItemTipsMethod ();
//	crJXJUICountryShopItemTipsMethod (const crJXJUICountryShopItemTipsMethod & handle);
//	MethodClass(JXJ, JXJUICountryShopItemTips)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_tips;
//	std::string m_name;
//	std::string m_info2;
//	std::string m_itemlv;
//	std::string m_itemicon;
//	std::string m_itemequipon;
//	std::string m_infocanvas;
//	std::string m_infobk;
//	std::string m_infodi;
//	CRCore::crVector3 m_offsets;
//	CRCore::crVector4f m_colorred;
//	int m_index;
//};


class crJXJCheckCountryOfficialMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckCountryOfficialMethod ();
	crJXJCheckCountryOfficialMethod (const crJXJCheckCountryOfficialMethod & handle);
	MethodClass(JXJ,JXJCheckCountryOfficial)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJOnlineSynToRankMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJOnlineSynToRankMethod ();
	crJXJOnlineSynToRankMethod (const crJXJOnlineSynToRankMethod & handle);
	MethodClass(JXJ,JXJOnlineSynToRank)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJQueryCityMapChengChiInfoMethod:public CRCore::crMethod
{//GameClient Method
public:
	crJXJQueryCityMapChengChiInfoMethod();
	crJXJQueryCityMapChengChiInfoMethod(const crJXJQueryCityMapChengChiInfoMethod& handle);
	MethodClass(JXJ, JXJQueryCityMapChengChiInfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
};

class crJXJRecvQueryCityMapChengChiInfoMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryCityMapChengChiInfoMethod();
	crJXJRecvQueryCityMapChengChiInfoMethod(const crJXJRecvQueryCityMapChengChiInfoMethod& handle);
	MethodClass(JXJ, JXJRecvQueryCityMapChengChiInfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};


class crJXJGameServerThrowMethod:public CRCore::crMethod
{//GameClient Method 用于gm运营工具界面 使服务器重启
public:
	crJXJGameServerThrowMethod();
	crJXJGameServerThrowMethod(const crJXJGameServerThrowMethod& handle);
	MethodClass(JXJ, JXJGameServerThrow)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
};

class crJXJGameServerTimeCountMethod:public CRCore::crMethod
{//GameClient Method 用于gm运营工具界面 使服务器重启
public:
	crJXJGameServerTimeCountMethod();
	crJXJGameServerTimeCountMethod(const crJXJGameServerTimeCountMethod& handle);
	MethodClass(JXJ, JXJGameServerTimeCount)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	char m_interval;
};
			
class crJXJGameServerButtonClickMethod:public CRCore::crMethod
{//GameClient Method 用于gm运营工具界面 使服务器重启
public:
	crJXJGameServerButtonClickMethod();
	crJXJGameServerButtonClickMethod(const crJXJGameServerButtonClickMethod & handle);
	MethodClass(JXJ, JXJGameServerButtonClick)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_input;
	std::string m_confirm;
};

class crJXJInitClientConnectGatewayMethod:public CRCore::crMethod
{//GameClient Method  运营工具show出来的时候初始化，只初始化一次
public:
	crJXJInitClientConnectGatewayMethod();
	crJXJInitClientConnectGatewayMethod(const crJXJInitClientConnectGatewayMethod& handle);
	MethodClass(JXJ, JXJInitClientConnectGateway)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	int m_serverGroupNum;
	int m_port;
	char m_gatewayNum;
};

class crJXJCountMonitorPacketMethod:public CRCore::crMethod
{//GameClient Method  
public:
	crJXJCountMonitorPacketMethod();
	crJXJCountMonitorPacketMethod(const crJXJCountMonitorPacketMethod& handle);
	MethodClass(JXJ, JXJCountMonitorPacket)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData * m_this;
	std::string m_canvas;
	int m_netType;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;

};

class crJXJMonitorDisplayAndRefreshMethod:public CRCore::crMethod
{//GameClient Method  
public:
	crJXJMonitorDisplayAndRefreshMethod();
	crJXJMonitorDisplayAndRefreshMethod(const crJXJMonitorDisplayAndRefreshMethod& handle);
	MethodClass(JXJ,JXJMonitorDisplayAndRefresh)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	std::string m_tabList;
	unsigned char m_serverNum;
};

class crJXJRestartServerCheckMethod : public CRCore::crMethod
{////GameClient Method
public:
	crJXJRestartServerCheckMethod ();
	crJXJRestartServerCheckMethod (const crJXJRestartServerCheckMethod & handle);
	MethodClass(JXJ, JXJRestartServerCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode* m_this;
	//std::string m_userNameWidget;
	std::string m_userPasswordWidget;
};

class crJXJRecvRestartServerCheckMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvRestartServerCheckMethod();
	crJXJRecvRestartServerCheckMethod(const crJXJRecvRestartServerCheckMethod& handle);
	MethodClass(JXJ, JXJRecvRestartServerCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJRemainStatisticsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRemainStatisticsMethod ();
	crJXJRemainStatisticsMethod (const crJXJRemainStatisticsMethod & handle);
	MethodClass(JXJ,JXJRemainStatistics)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJLevelStatisticsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJLevelStatisticsMethod ();
	crJXJLevelStatisticsMethod (const crJXJLevelStatisticsMethod & handle);
	MethodClass(JXJ,JXJLevelStatistics)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJNewerStepStatisticsMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJNewerStepStatisticsMethod ();
	crJXJNewerStepStatisticsMethod (const crJXJNewerStepStatisticsMethod & handle);
	MethodClass(JXJ,JXJNewerStepStatistics)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJRegisterStatisticsMethod :public CRCore::crMethod
{//GameServer Method				// 注册统计
public:
	crJXJRegisterStatisticsMethod ();
	crJXJRegisterStatisticsMethod (const crJXJRegisterStatisticsMethod & handle);
	MethodClass(JXJ,JXJRegisterStatistics)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJUIButtonNewTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIButtonNewTipsMethod ();
	crJXJUIButtonNewTipsMethod (const crJXJUIButtonNewTipsMethod & handle);
	MethodClass(JXJ, JXJUIButtonNewTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_noticeid;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

//class crJXJCheckServerVersionMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJCheckServerVersionMethod ();
//	crJXJCheckServerVersionMethod (const crJXJCheckServerVersionMethod & handle);
//	MethodClass(JXJ, JXJCheckServerVersion)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};

//class crJXJRecvCheckServerVersionMethod:public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvCheckServerVersionMethod();
//	crJXJRecvCheckServerVersionMethod(const crJXJRecvCheckServerVersionMethod& handle);
//	MethodClass(JXJ, JXJRecvCheckServerVersion)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_canvas;
//};

class crJXJReduceRTMpMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJReduceRTMpMethod ();
	crJXJReduceRTMpMethod (const crJXJReduceRTMpMethod & handle);
	MethodClass(JXJ, JXJReduceRTMp)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem* m_this;
	//float m_damage;
	//DamagePair m_damagePair;//damage,type
	float m_reduce;
	//CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};


class crJXJCureRTMpMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJCureRTMpMethod ();
	crJXJCureRTMpMethod (const crJXJCureRTMpMethod & handle);
	MethodClass(JXJ, JXJCureRTMp)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem* m_this;
	float m_cureMP;
	//CRCore::ref_ptr<CRNetApp::crInstanceItem> m_fireItem;
};

class crJXJRecvRedeceRTMpMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvRedeceRTMpMethod ();
	crJXJRecvRedeceRTMpMethod (const crJXJRecvRedeceRTMpMethod & handle);
	MethodClass(JXJ, JXJRecvRedeceRTMp)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);

protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvas;
};

class crJXJRecvCureMPMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvCureMPMethod();
	crJXJRecvCureMPMethod(const crJXJRecvCureMPMethod& handle);
	MethodClass(CRNetApp, JXJRecvCureMP)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJRevertUpdateMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJRevertUpdateMethod ();
	crJXJRevertUpdateMethod (const crJXJRevertUpdateMethod & handle);
	MethodClass(JXJ, JXJRevertUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem *m_this;
	float m_dt;
	float m_interval;
	float m_time;
};

class crJXJRestartServerShowCanvasMethod : public CRCore::crMethod
{////GameClient Method
public:
	crJXJRestartServerShowCanvasMethod ();
	crJXJRestartServerShowCanvasMethod (const crJXJRestartServerShowCanvasMethod & handle);
	MethodClass(JXJ, JXJRestartServerShowCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	std::string m_passwordCanvas;
	std::string m_userPasswordWidget;
	int m_netType;
};

class crJXJLoadGameServerRankFilesMethod : public CRCore::crMethod
{////GameClient Method
public:
	crJXJLoadGameServerRankFilesMethod ();
	crJXJLoadGameServerRankFilesMethod (const crJXJLoadGameServerRankFilesMethod & handle);
	MethodClass(JXJ, JXJLoadGameServerRankFiles)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_serverName;
	std::string m_exploitRank;
	std::string m_levelRank;
	std::string m_govpostRank;
	std::string m_accomplishRank;
	std::string m_troopsRank;
	std::string m_countrycontributeRank;
	std::string m_manorlvRank;
	std::string m_consortiaLvRank;
	std::string m_strServerCycleRank;
	std::string m_onlinePlayeridTab;
};

class crJXJQueryMyShiliDataMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyShiliDataMethod ();
	crJXJQueryMyShiliDataMethod (const crJXJQueryMyShiliDataMethod & handle);
	MethodClass(JXJ, JXJQueryMyShiliData)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
};

class crJXJRecvQueryMyShiliDataMethod:public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvQueryMyShiliDataMethod();
	crJXJRecvQueryMyShiliDataMethod(const crJXJRecvQueryMyShiliDataMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyShiliData)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
};

class crJXJShowVersionCanvasMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJShowVersionCanvasMethod ();
	crJXJShowVersionCanvasMethod (const crJXJShowVersionCanvasMethod & handle);
	MethodClass(JXJ, JXJShowVersionCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crBrain *m_this;
	std::string m_version;
};

class crJXJCheckWetherChangeGroupLeaderMethod : public CRCore::crMethod
{////GameServer Method
public:
	crJXJCheckWetherChangeGroupLeaderMethod ();
	crJXJCheckWetherChangeGroupLeaderMethod (const crJXJCheckWetherChangeGroupLeaderMethod & handle);
	MethodClass(JXJ, JXJCheckWetherChangeGroupLeader)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJBuildPlayerDataSaveStreamTabMethod : public CRCore::crMethod
{////GameServer Method
public:
	crJXJBuildPlayerDataSaveStreamTabMethod ();
	crJXJBuildPlayerDataSaveStreamTabMethod (const crJXJBuildPlayerDataSaveStreamTabMethod & handle);
	MethodClass(JXJ, JXJBuildPlayerDataSaveStreamTab)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_gmID;
};

//class crJXJBuildMainRoleDataSaveStreamTabMethod : public CRCore::crMethod
//{////GameServer Method
//public:
//	crJXJBuildMainRoleDataSaveStreamTabMethod ();
//	crJXJBuildMainRoleDataSaveStreamTabMethod (const crJXJBuildMainRoleDataSaveStreamTabMethod & handle);
//	MethodClass(JXJ, JXJBuildMainRoleDataSaveStreamTab)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//	CRNetApp::crPlayerGameData *m_player;
//};

class crJXJWebQueryPlayerDataDisposeMethod : public CRCore::crMethod
{////GameServer Method
public:
	crJXJWebQueryPlayerDataDisposeMethod ();
	crJXJWebQueryPlayerDataDisposeMethod (const crJXJWebQueryPlayerDataDisposeMethod & handle);
	MethodClass(JXJ, JXJWebQueryPlayerDataDispose)
	CRCore::ref_ptr<CRNetApp::crGameServerPlayerData> getPlayerData(int playerid,int serverid);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJSavePeopleMapDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSavePeopleMapDisposeMethod ();
	crJXJSavePeopleMapDisposeMethod (const crJXJSavePeopleMapDisposeMethod & handle);
	MethodClass(JXJ, JXJSavePeopleMapDispose)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJSaveLatestMousePushPosMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveLatestMousePushPosMethod ();
	crJXJSaveLatestMousePushPosMethod (const crJXJSaveLatestMousePushPosMethod & handle);
	MethodClass(JXJ, JXJSaveLatestMousePushPos)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crMatrixTransform *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};


class crJXJShowFloadAnimationTipsMethod :public CRCore::crMethod
{//Client  Method		显示浮动tips动画
public:
	crJXJShowFloadAnimationTipsMethod ();
	crJXJShowFloadAnimationTipsMethod (const crJXJShowFloadAnimationTipsMethod & handle);
	MethodClass(JXJ, JXJShowFloadAnimationTips)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crBrain *m_this;
	std::string m_canvasInput;
	std::string m_input;
	CRCore::crVector3 m_pos;
	float m_speed;
	CRCore::crVector3 m_inputPos;
	CRCore::crVector3 m_inputTargetPos;
	float m_inputSpeed;
};

class crJXJPutFloatContentIntoDeqMethod :public CRCore::crMethod
{//Client  Method		将浮动文字放入deq
public:
	crJXJPutFloatContentIntoDeqMethod ();
	crJXJPutFloatContentIntoDeqMethod (const crJXJPutFloatContentIntoDeqMethod & handle);
	MethodClass(JXJ, JXJPutFloatContentIntoDeq)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crBrain *m_this;
	int m_textID;
};


class crJXJFoundCountryGetAwardMethod :public CRCore::crMethod
{//Client  Method		// 建国系统领取奖励
public:
	crJXJFoundCountryGetAwardMethod ();
	crJXJFoundCountryGetAwardMethod (const crJXJFoundCountryGetAwardMethod & handle);
	MethodClass(JXJ, JXJFoundCountryGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	unsigned char m_awardType;			// 0 表示一次领取一个，1 表示一次全部领取
	unsigned char m_idx;				// 领取的奖励所在的城池索引 
};

class crJXJRecvFoundCountryGetAwardMethod :public CRCore::crMethod
{//Client  Method	& server	 
public:
	crJXJRecvFoundCountryGetAwardMethod ();
	crJXJRecvFoundCountryGetAwardMethod (const crJXJRecvFoundCountryGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecvFoundCountryGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJNewerTaskHadAllCompletedMethod :public CRCore::crMethod
{//Client  Method	& server	 
public:
	crJXJNewerTaskHadAllCompletedMethod ();
	crJXJNewerTaskHadAllCompletedMethod (const crJXJNewerTaskHadAllCompletedMethod & handle);
	MethodClass(JXJ, JXJNewerTaskHadAllCompleted)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	int m_lastTaskID;
	CRNetApp::crPlayerGameData *m_this;
	bool *m_isCompleted;
};

//class crJXJFoundCountryCanvasUpdateMethod :public CRCore::crMethod
//{//Client  Method		建国系统更新canvas
//public:
//	crJXJFoundCountryCanvasUpdateMethod ();
//	crJXJFoundCountryCanvasUpdateMethod (const crJXJFoundCountryCanvasUpdateMethod & handle);
//	MethodClass(JXJ, JXJFoundCountryCanvasUpdate)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_bckSw;
//	std::string m_pageSw;
//	std::string m_citySw[3][8];
//	std::string m_cityBtnSw[3][8];
//	std::string m_cityInput[3][9];
//	std::string m_topInput;
//	std::string m_getAllAwardBtn;
//	std::string m_getAllAwardBtnSw;
//	std::string m_goToGuozhanBtn;
//	std::string m_cityBtn[3][8];
//	std::string m_arrow[3];
//};

class crJXJFoundCountryCanvasSwitchBoxAndBtnMethod :public CRCore::crMethod
{//Client  Method		建国系统 鼠标悬浮在宝箱上的切换
public:
	crJXJFoundCountryCanvasSwitchBoxAndBtnMethod ();
	crJXJFoundCountryCanvasSwitchBoxAndBtnMethod (const crJXJFoundCountryCanvasSwitchBoxAndBtnMethod & handle);
	MethodClass(JXJ, JXJFoundCountryCanvasSwitchBoxAndBtn)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	bool m_focus;
	std::string m_sw;
	std::string m_btnSw;
	std::string m_btn;
};

class crJXJFirstWeekGenerateCountryOfficialMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJFirstWeekGenerateCountryOfficialMethod ();
	crJXJFirstWeekGenerateCountryOfficialMethod (const crJXJFirstWeekGenerateCountryOfficialMethod & handle);
	MethodClass(JXJ, JXJFirstWeekGenerateCountryOfficial)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	bool m_isFirstWeek;
	unsigned char m_shiliID;
};

class crJXJKictOutGroupMemberByGMMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJKictOutGroupMemberByGMMethod ();
	crJXJKictOutGroupMemberByGMMethod (const crJXJKictOutGroupMemberByGMMethod & handle);
	MethodClass(JXJ, JXJKictOutGroupMemberByGM)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_playerid ;
};

class crJXJSaveOnlinePlayerEveryDayMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveOnlinePlayerEveryDayMethod ();
	crJXJSaveOnlinePlayerEveryDayMethod (const crJXJSaveOnlinePlayerEveryDayMethod & handle);
	MethodClass(JXJ, JXJSaveOnlinePlayerEveryDay)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_onlinePlayerTab;
};

class crJXJ7DaysGetAwardMethod :public CRCore::crMethod
{//Client  Method		// 7日登陆领取奖励
public:
	crJXJ7DaysGetAwardMethod ();
	crJXJ7DaysGetAwardMethod (const crJXJ7DaysGetAwardMethod & handle);
	MethodClass(JXJ, JXJ7DaysGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	//unsigned char m_awardType;			// 0 表示一次领取一个，1 表示一次全部领取
	unsigned char m_idx;				 
};

class crJXJRecv7DaysGetAwardMethod :public CRCore::crMethod
{//Client  Method	& server	 
public:
	crJXJRecv7DaysGetAwardMethod ();
	crJXJRecv7DaysGetAwardMethod (const crJXJRecv7DaysGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecv7DaysGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJ7DaysCanvasUpdateMethod :public CRCore::crMethod
{//Client  Method		7日登陆
public:
	crJXJ7DaysCanvasUpdateMethod ();
	crJXJ7DaysCanvasUpdateMethod (const crJXJ7DaysCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJ7DaysCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_7Button[7];
	std::string m_animation[7];
	std::string m_sw[7];
};

class crJXJUI7DaysAwardTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUI7DaysAwardTipsMethod ();
	crJXJUI7DaysAwardTipsMethod (const crJXJUI7DaysAwardTipsMethod & handle);
	MethodClass(JXJ, JXJUI7DaysAwardTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info2;
	std::string m_itemlv;
	std::string m_itemicon;
	std::string m_itemequipon;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_index;
};

class crJXJSetBuildingFlagVisiableMethod :public CRCore::crMethod
{//Client
public:
	crJXJSetBuildingFlagVisiableMethod ();
	crJXJSetBuildingFlagVisiableMethod (const crJXJSetBuildingFlagVisiableMethod & handle);
	MethodClass(JXJ, JXJSetBuildingFlagVisiable)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_buildingID[4];
	CRCore::NodeArray m_nodeSwArray[4];
	CRCore::ref_ptr<CRNetApp::crInstanceItem >  m_buildings[4];
	bool m_init;
};


};
#endif
