/**********************************************************************
*
*	文件:	   appMethod.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD_H
#define JXJ_APPMETHOD_H
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
void getDesstring(std::string &desstr,std::string &param1,std::string &param2,std::string &output, std::string param3="");
namespace JXJ{
class crJXJUISelectPlayerIconUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectPlayerIconUpdateMethod();
	crJXJUISelectPlayerIconUpdateMethod(const crJXJUISelectPlayerIconUpdateMethod& handle);
	MethodClass(JXJ, JXJUISelectPlayerIconUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_playerIconRadioGroup;
	std::string m_iconimagebox;
	std::string m_sexradio;
};
class crJXJUISelectIconSexChangeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectIconSexChangeMethod();
	crJXJUISelectIconSexChangeMethod(const crJXJUISelectIconSexChangeMethod& handle);
	MethodClass(JXJ, JXJUISelectIconSexChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_sex;
};
class crJXJUISelectIconWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectIconWalkMethod();
	crJXJUISelectIconWalkMethod(const crJXJUISelectIconWalkMethod& handle);
	MethodClass(JXJ, JXJUISelectIconWalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_step;
};
class crJXJUISetPlayerIconMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISetPlayerIconMethod();
	crJXJUISetPlayerIconMethod(const crJXJUISetPlayerIconMethod& handle);
	MethodClass(JXJ, JXJUISetPlayerIcon)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_playerIconRadioGroup;
	std::string m_nameWidget;
};
class crJXJUISelectPowerInitMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectPowerInitMethod();
	crJXJUISelectPowerInitMethod(const crJXJUISelectPowerInitMethod& handle);
	MethodClass(JXJ, JXJUISelectPowerInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_selectPowerCanvas;
	std::string m_powerRaidoGroup;
};
class crJXJUICreatePlayerMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUICreatePlayerMethod();
	crJXJUICreatePlayerMethod(const crJXJUICreatePlayerMethod& handle);
	MethodClass(JXJ, JXJUICreatePlayer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_selectPowerCanvas;
	std::string m_powerRaidoGroup;
};
class crJXJPlayerLoginMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJPlayerLoginMethod();
	crJXJPlayerLoginMethod(const crJXJPlayerLoginMethod& handle);
	MethodClass(JXJ, JXJPlayerLogin)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJSetCountryFlagMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetCountryFlagMethod();
	crJXJSetCountryFlagMethod(const crJXJSetCountryFlagMethod& handle);
	MethodClass(JXJ, JXJSetCountryFlag)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_flagname;
	std::string m_weiflag;
	std::string m_shuflag;
	std::string m_wuflag;
	std::string m_npcflag;
};
class crJXJSetItemFlagMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetItemFlagMethod();
	crJXJSetItemFlagMethod(const crJXJSetItemFlagMethod& handle);
	MethodClass(JXJ, JXJSetItemFlag)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_flagname;
	std::string m_weiflag;
	std::string m_shuflag;
	std::string m_wuflag;
	std::string m_npcflag;
};
class crJXJBingzhengCampFlagMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBingzhengCampFlagMethod();
	crJXJBingzhengCampFlagMethod(const crJXJBingzhengCampFlagMethod& handle);
	MethodClass(JXJ, JXJBingzhengCampFlag)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_nodeName;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_body;
};
class crJXJBingzhengCanAddArmyMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBingzhengCanAddArmyMethod();
	crJXJBingzhengCanAddArmyMethod(const crJXJBingzhengCanAddArmyMethod& handle);
	MethodClass(JXJ, JXJBingzhengCanAddArmy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	std::string m_nodeName;
	CRCore::ref_ptr<CRCore::crMultiSwitch> m_body;
};
class crJXJSetConstructtionLevelMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetConstructtionLevelMethod();
	crJXJSetConstructtionLevelMethod(const crJXJSetConstructtionLevelMethod& handle);
	MethodClass(JXJ, JXJSetConstructtionLevel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void setTerritoryBuildingModel(std::vector<CRCore::crMultiSwitch *> modelvec,std::vector<int> buildinglvvec,float factor,float limit,int flag);
	void setFunctionalBuildingModel(CRCore::crMultiSwitch *model,int buildinglv,float factor,float limit,int flag);
};
class crJXJUIUpdateMyInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateMyInfoMethod();
	crJXJUIUpdateMyInfoMethod(const crJXJUIUpdateMyInfoMethod& handle);
	MethodClass(JXJ, JXJUIUpdateMyInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void ifOverhundredthousand(int i, CRUI::crStaticTextWidgetNode* textnode);
protected:
	CRCore::crCanvasNode* m_this;
	//CRCore::crVector4 m_normalColor;
	//CRCore::crVector4 m_overColor;
	std::string m_playerFace;
	std::string m_nickname;
	std::string m_flag;
	std::string m_gold;
	std::string m_govPosition;
	std::string m_cashGift;
	std::string m_money;
	std::string m_food;
	std::string m_wood;
	std::string m_iron;
	std::string m_horse;
	std::string m_lv;
	std::string m_lijin;
	std::string m_junling;
	std::string m_exp;
	std::string m_expprogress;
	std::string m_junlingPro;
	std::string m_vipflashsw;
	std::string m_mailplay;
	std::string m_qiyubtn;
	std::string m_gzphCanvas;
	std::string m_guozhanDonghuaSW;
};
class crJXJGameServerUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerUpdateMethod();
	crJXJGameServerUpdateMethod(const crJXJGameServerUpdateMethod& handle);
	MethodClass(JXJ, JXJGameServerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void zeroRefresh(time_t t1, bool bZero,bool initZero = false);
	//void mainrolerefesh();
	CRNetApp::crPlayerGameData* m_this;
	CRNetApp::crGameServerPlayerData* m_player;
	int m_maxInterval;
};
class crJXJResUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJResUpdateMethod();
	crJXJResUpdateMethod(const crJXJResUpdateMethod& handle);
	MethodClass(JXJ, JXJResUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float getExtraProduce(unsigned char type,float base,float dt,ExtraProduceMap *extraProduceMap);
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_foodDelta;
	float m_woodDelta;
	float m_ironDelta;
	float m_horseDelta;
};
class crJXJJunlingProduceMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJJunlingProduceMethod();
	crJXJJunlingProduceMethod(const crJXJJunlingProduceMethod& handle);
	MethodClass(JXJ, JXJJunlingProduce)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
	float m_delta;
	//float m_dt_record;
};
class crJXJRecvJunlingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvJunlingMethod();
	crJXJRecvJunlingMethod(const crJXJRecvJunlingMethod& handle);
	MethodClass(JXJ, JXJRecvJunling)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCameraMouseMoveMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraMouseMoveMethod();
	crJXJCameraMouseMoveMethod(const crJXJCameraMouseMoveMethod& handle);
	MethodClass(JXJ, JXJCameraMouseMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	float m_delta;
	float m_delta_extra;
};
class crJXJCameraMoveUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraMoveUpdateMethod();
	crJXJCameraMoveUpdateMethod(const crJXJCameraMoveUpdateMethod& handle);
	MethodClass(JXJ, JXJCameraMoveUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void domove(CRCore::crMatrix &mat,CRCore::crVector3 &campos,CRProducer::crViewer *viewer);
	CRCore::crMatrixTransform* m_this;
	CRUtil::crUpdateVisitor* m_updateVisitor;
	float m_speed;
	CRCore::crVector2 m_limitOffset;
	CRCore::crIntersectVisitor               m_iv;
	CRCore::ref_ptr<CRCore::crLineSegment>   m_lineSegment;
};
class crJXJCameraScrollMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraScrollMethod();
	crJXJCameraScrollMethod(const crJXJCameraScrollMethod& handle);
	MethodClass(JXJ, JXJCameraScroll)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_step;
	float m_minDist;
	static float s_value;
	CRCore::crIntersectVisitor               m_iv;
	CRCore::ref_ptr<CRCore::crLineSegment>   m_lineSegment;
};
class crJXJCameraRotMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraRotMethod();
	crJXJCameraRotMethod(const crJXJCameraRotMethod& handle);
	MethodClass(JXJ, JXJCameraRot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	unsigned char m_flg;
};
class crJXJCameraRotUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCameraRotUpdateMethod();
	crJXJCameraRotUpdateMethod(const crJXJCameraRotUpdateMethod& handle);
	MethodClass(JXJ, JXJCameraRotUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crMatrixTransform *m_this;
	float m_speed;
	float m_recoverSpeed;
	float m_cameraRot;
};
struct BuildingLevelUpCheckResult
{
	BuildingLevelUpCheckResult():
		m_code(0),
		m_coppercashNeed(0),
		m_timeNeed(0),
		m_woodNeed(0),
		m_ironNeed(0),
		m_foodNeed(0)
	{}
	char m_code;
	int m_timeNeed;
	int m_coppercashNeed;
	int m_woodNeed;
	int m_ironNeed;
	int m_foodNeed;
};
class crJXJBuildingLevelUpCheckMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJBuildingLevelUpCheckMethod();
	crJXJBuildingLevelUpCheckMethod(const crJXJBuildingLevelUpCheckMethod& handle);
	MethodClass(JXJ, JXJBuildingLevelUpCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_buildingid;
	BuildingLevelUpCheckResult *m_result;
};
class crJXJBuildingLevelUpMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildingLevelUpMethod();
	crJXJBuildingLevelUpMethod(const crJXJBuildingLevelUpMethod& handle);
	MethodClass(JXJ, JXJBuildingLevelUp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJUIBuildLevelUpCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildLevelUpCheckMethod();
	crJXJUIBuildLevelUpCheckMethod(const crJXJUIBuildLevelUpCheckMethod& handle);
	MethodClass(JXJ, JXJUIBuildLevelUpCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_levelup;
	std::string m_prompt;
	std::string m_time;
	std::string m_need;
	std::string m_money;
	std::string m_wood;
	std::string m_iron;
	std::string m_food;
	std::string m_canvasname;
	std::string m_level;
	std::string m_nowproduce;
	std::string m_nextproduce;
	std::string m_onekeylevelup;
};
class crJXJRecvBuildingLevelUpMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuildingLevelUpMethod();
	crJXJRecvBuildingLevelUpMethod(const crJXJRecvBuildingLevelUpMethod& handle);
	MethodClass(JXJ, JXJRecvBuildingLevelUp)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJBuildingLevelChangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildingLevelChangeMethod();
	crJXJBuildingLevelChangeMethod(const crJXJBuildingLevelChangeMethod& handle);
	MethodClass(JXJ, JXJBuildingLevelChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_buildingid;
	unsigned char m_buildinglv;
	std::string m_shijiCanvas;
};
class crJXJBuildingLevelCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuildingLevelCanvasMethod();
	crJXJBuildingLevelCanvasMethod(const crJXJBuildingLevelCanvasMethod& handle);
	MethodClass(JXJ, JXJBuildingLevelCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRProducer::crGUIEventAdapter* m_ea;
	std::string m_canvasName;
	std::string m_title;
	std::string m_lv;
	std::string m_nowlv;
	std::string m_nextlv;
	std::string m_nowproduce;
	std::string m_nextproduce;
	std::string m_timing;
	std::string m_info;
	std::string m_need;
	std::string m_money;
	std::string m_wood;
	std::string m_iron;
	std::string m_food;
	std::string m_warning;
};
//class crJXJBuildingLevelCanvas2Method :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJBuildingLevelCanvas2Method();
//	crJXJBuildingLevelCanvas2Method(const crJXJBuildingLevelCanvas2Method& handle);
//	MethodClass(JXJ, JXJBuildingLevelCanvas2)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	CRProducer::crGUIEventAdapter* m_ea;
//	std::string m_canvasName;
//	std::string m_title;
//	std::string m_lv;
//	std::string m_nowlv;
//	std::string m_nextlv;
//	std::string m_nowproduce;
//	std::string m_nextproduce;
//	std::string m_timing;
//	std::string m_info;
//	std::string m_need;
//	std::string m_money;
//	std::string m_wood;
//	std::string m_iron;
//	std::string m_food;
//	std::string m_warning;
//	std::string m_buildingname;
//};
//class crJXJUIBuildingCanvasUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIBuildingCanvasUpdateMethod();
//	crJXJUIBuildingCanvasUpdateMethod(const crJXJUIBuildingCanvasUpdateMethod& handle);
//	MethodClass(JXJ, JXJUIBuildingCanvasUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_title;
//	std::string m_lv;
//	std::string m_nowproduce;
//	std::string m_nextproduce;
//	std::string m_info;
//	std::string m_timing;
//	std::string m_need;
//	std::string m_money;
//	std::string m_wood;
//	std::string m_iron;
//	std::string m_food;
//	std::string m_warning;
//};
typedef std::pair<char,int> TechLevelUpCheckResult;
class crJXJTechLevelUpCheckMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJTechLevelUpCheckMethod();
	crJXJTechLevelUpCheckMethod(const crJXJTechLevelUpCheckMethod& handle);
	MethodClass(JXJ, JXJTechLevelUpCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_techid;
	TechLevelUpCheckResult *m_result;
};
//class crJXJTechLevelUpMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJTechLevelUpMethod();
//	crJXJTechLevelUpMethod(const crJXJTechLevelUpMethod& handle);
//	MethodClass(JXJ, JXJTechLevelUp)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode* m_this;
//};
//class crJXJRecvTechLevelUpMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvTechLevelUpMethod();
//	crJXJRecvTechLevelUpMethod(const crJXJRecvTechLevelUpMethod& handle);
//	MethodClass(JXJ, JXJRecvTechLevelUp)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJUITimerUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITimerUpdateMethod();
	crJXJUITimerUpdateMethod(const crJXJUITimerUpdateMethod& handle);
	MethodClass(JXJ, JXJUITimerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	std::string IfHourOverOneDay(int hour);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_ui_building1;
	std::string m_ui_buildingSpeed1;
	std::string m_ui_building2;
	std::string m_ui_buildingSpeed2;
	std::string m_building3Back;
	std::string m_ui_building3;
	std::string m_ui_buildingSpeed3;
	std::string m_building4Back;
	std::string m_ui_building4;
	std::string m_ui_buildingSpeed4;
	std::string m_ui_conscriptionCD1;
	std::string m_ui_speed_01;
	std::string m_ui_conscriptionCD2;
	std::string m_ui_speed_02;
	std::string m_conscriptionBack3;
	std::string m_ui_conscriptionCD3;
	std::string m_ui_speed_03;
	std::string m_conscriptionBack4;
	std::string m_ui_conscriptionCD4;
	std::string m_ui_speed_04;
	std::string m_ui_buildingSpeedAdd1;
	std::string m_ui_buildingSpeedAdd2;
	std::string m_ui_buildingSpeedAdd3;
	std::string m_ui_buildingSpeedAdd4;
	std::string m_ui_SpeedAdd1;
	std::string m_ui_SpeedAdd2;
	std::string m_ui_SpeedAdd3;
	std::string m_ui_SpeedAdd4;
};
class crJXJUIRecruitUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitUpdateMethod();
	crJXJUIRecruitUpdateMethod(const crJXJUIRecruitUpdateMethod& handle);
	MethodClass(JXJ, JXJUIRecruitUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_armyRadio;
	std::string m_armyTypeRadio;
	std::string m_armUp;
	std::string m_armDown;
};
class crJXJUIRecruitSelInfoUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitSelInfoUpdateMethod();
	crJXJUIRecruitSelInfoUpdateMethod(const crJXJUIRecruitSelInfoUpdateMethod& handle);
	MethodClass(JXJ, JXJUIRecruitSelInfoUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_condition;
	std::string m_armyInfo;
	std::string m_armyInput;
	std::string m_equip;
	std::string m_equipTotal;
	std::string m_coppercashTotal;
	std::string m_foodTotal;
	std::string m_timeTotal;
	std::string m_recruitBtn;
	std::string m_equipForge;
	std::string m_inputInc;
	std::string m_inputDec;
	CRCore::crVector4 m_namecolor;
	std::string m_ironinput;
	std::string m_woodinput;
	std::string m_horseinput;
};
class crJXJUITroopsListUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITroopsListUpdateMethod();
	crJXJUITroopsListUpdateMethod(const crJXJUITroopsListUpdateMethod& handle);
	MethodClass(JXJ, JXJUITroopsListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_troopsList;
};

class crJXJRecruitCheckMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecruitCheckMethod();
	crJXJRecruitCheckMethod(const crJXJRecruitCheckMethod& handle);
	MethodClass(JXJ, JXJRecruitCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_troopsAbstractID;
	RecruitCheckResult *m_result;
};
class crJXJUIRecruitArmWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitArmWalkMethod();
	crJXJUIRecruitArmWalkMethod(const crJXJUIRecruitArmWalkMethod& handle);
	MethodClass(JXJ, JXJUIRecruitArmWalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_step;
	std::string m_armyTypeRadio;
};
class crJXJUIRecruitCountStepMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitCountStepMethod();
	crJXJUIRecruitCountStepMethod(const crJXJUIRecruitCountStepMethod& handle);
	MethodClass(JXJ, JXJUIRecruitCountStep)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_step;
	int m_min;
	std::string m_input;
};

///////////////////////////////////////////////////////////////////////
class crJXJUISetCountScrolPosMethod:public CRCore::crMethod
{
public:
	crJXJUISetCountScrolPosMethod();
	crJXJUISetCountScrolPosMethod(const crJXJUISetCountScrolPosMethod& handle);
	MethodClass(JXJ,JXJUISetCountScrolPos);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	float m_min;
	float m_max;
	int	  m_type;
	std::string m_scrollbar;
	std::string m_input_f;
	std::string m_input_w;
	std::string m_input_i;
	std::string m_input_h;
	CRUI::crEditWidgetNode *m_sourceCount;
};
////////////////////////////////////////////////////////////////////////
class crJXJUIRecruitCountInputMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitCountInputMethod();
	crJXJUIRecruitCountInputMethod(const crJXJUIRecruitCountInputMethod& handle);
	MethodClass(JXJ, JXJUIRecruitCountInput)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crEditWidgetNode* m_this;
};
class crJXJUIRecruitArmResetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitArmResetMethod();
	crJXJUIRecruitArmResetMethod(const crJXJUIRecruitArmResetMethod& handle);
	MethodClass(JXJ, JXJUIRecruitArmReset)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJUIRecruitMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitMethod();
	crJXJUIRecruitMethod(const crJXJUIRecruitMethod& handle);
	MethodClass(JXJ, JXJUIRecruit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_armyInput;
	std::string m_recruiteCanvas;
};
class crJXJRecvRecruitMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvRecruitMethod();
	crJXJRecvRecruitMethod(const crJXJRecvRecruitMethod& handle);
	MethodClass(JXJ, JXJRecvRecruit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUITechUpdateInitMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITechUpdateInitMethod();
	crJXJUITechUpdateInitMethod(const crJXJUITechUpdateInitMethod& handle);
	MethodClass(JXJ, JXJUITechUpdateInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_techid;
	std::string m_canvasName;
};
class crJXJUITechTipInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITechTipInfoMethod();
	crJXJUITechTipInfoMethod(const crJXJUITechTipInfoMethod& handle);
	MethodClass(JXJ, JXJUITechTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	int m_techid;
	std::string m_canvasName;
	std::string m_titleName;
	std::string m_info;
	std::string m_money;
	std::string m_nextlvDes;
};
class crJXJUITechUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITechUpdateMethod();
	crJXJUITechUpdateMethod(const crJXJUITechUpdateMethod& handle);
	MethodClass(JXJ, JXJUITechUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	int m_techstartid;
	typedef std::vector<std::string> StrVec;
	StrVec m_techlvStrVec;
	StrVec m_techbtnStrVec;
};
class crJXJUIFormationArmyUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFormationArmyUpdateMethod();
	crJXJUIFormationArmyUpdateMethod(const crJXJUIFormationArmyUpdateMethod& handle);
	MethodClass(JXJ, JXJUIFormationArmyUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_armyRadio;
	std::string m_phalanxRadio;
	std::string m_army01;
	std::string m_army02;
	std::string m_army03;
	std::string m_army04;
	std::string m_army05;
	std::string m_army06;
	std::string m_phalanx01;
	std::string m_phalanx02;
	std::string m_phalanx03;
};
class crJXJUIFormationArmyWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFormationArmyWalkMethod();
	crJXJUIFormationArmyWalkMethod(const crJXJUIFormationArmyWalkMethod& handle);
	MethodClass(JXJ, JXJUIFormationArmyWalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_step;
};
class crJXJUIFormationMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFormationMethod();
	crJXJUIFormationMethod(const crJXJUIFormationMethod& handle);
	MethodClass(JXJ, JXJUIFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_input;
};
class crJXJRecvFormationMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFormationMethod();
	crJXJRecvFormationMethod(const crJXJRecvFormationMethod& handle);
	MethodClass(JXJ, JXJRecvFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJCreateRolesMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCreateRolesMethod();
	crJXJCreateRolesMethod(const crJXJCreateRolesMethod& handle);
	MethodClass(JXJ, JXJCreateRoles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvGameRoleListStreamMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvGameRoleListStreamMethod();
	crJXJRecvGameRoleListStreamMethod(const crJXJRecvGameRoleListStreamMethod& handle);
	MethodClass(CRNetApp, JXJRecvGameRoleListStream)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRNetApp::crRecvDataStream> m_recvDataStream;
};
class crJXJCheckRolesCreatedMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckRolesCreatedMethod();
	crJXJCheckRolesCreatedMethod(const crJXJCheckRolesCreatedMethod& handle);
	MethodClass(JXJ, JXJCheckRolesCreated)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_count;
	char m_trueExecCount;
};
class crJXJUIRevocationMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRevocationMethod();
	crJXJUIRevocationMethod(const crJXJUIRevocationMethod& handle);
	MethodClass(JXJ, JXJUIRevocation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_phalanxRadio;
};
class crJXJRecvRevocationMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvRevocationMethod();
	crJXJRecvRevocationMethod(const crJXJRecvRevocationMethod& handle);
	MethodClass(JXJ, JXJRecvRevocation)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUICheckAndSetCDPromptMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUICheckAndSetCDPromptMethod();
	crJXJUICheckAndSetCDPromptMethod(const crJXJUICheckAndSetCDPromptMethod& handle);
	MethodClass(JXJ, JXJUICheckAndSetCDPrompt)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_cdhintid;
	int m_timeid;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_hintCheck;
	std::string m_font;
	std::string m_hintbackboard;
	int m_characterSize;
	CRCore::crVector4f m_textColor;
	CRCore::crVector4f m_goldColor;
	std::string m_timeindex;
};
class crJXJBuyCDMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBuyCDMethod();
	crJXJBuyCDMethod(const crJXJBuyCDMethod& handle);
	MethodClass(JXJ, JXJBuyCD)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_price;
};
class crJXJRecvBuyCDMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyCDMethod();
	crJXJRecvBuyCDMethod(const crJXJRecvBuyCDMethod& handle);
	MethodClass(JXJ, JXJRecvBuyCD)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuyCDMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyCDMethod();
	crJXJUIBuyCDMethod(const crJXJUIBuyCDMethod& handle);
	MethodClass(JXJ, JXJUIBuyCD)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_checkbox;
};
class crJXJUICDHintMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUICDHintMethod();
	crJXJUICDHintMethod(const crJXJUICDHintMethod& handle);
	MethodClass(JXJ, JXJUICDHint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crCheckBoxWidgetNode* m_this;
};
//class crJXJRecvCDHintMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvCDHintMethod();
//	crJXJRecvCDHintMethod(const crJXJRecvCDHintMethod& handle);
//	MethodClass(JXJ, JXJRecvCDHint)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJUIResetCDHintMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIResetCDHintMethod();
	crJXJUIResetCDHintMethod(const crJXJUIResetCDHintMethod& handle);
	MethodClass(JXJ, JXJUIResetCDHint)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJBuyTimersMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBuyTimersMethod();
	crJXJBuyTimersMethod(const crJXJBuyTimersMethod& handle);
	MethodClass(JXJ, JXJBuyTimers)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_price;
};
class crJXJRecvBuyTimersMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyTimersMethod();
	crJXJRecvBuyTimersMethod(const crJXJRecvBuyTimersMethod& handle);
	MethodClass(JXJ, JXJRecvBuyTimers)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIBuildFubenListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildFubenListMethod();
	crJXJUIBuildFubenListMethod(const crJXJUIBuildFubenListMethod& handle);
	MethodClass(JXJ, JXJUIBuildFubenList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_modeRaido;
};
class crJXJUIFubenSelectChangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFubenSelectChangeMethod();
	crJXJUIFubenSelectChangeMethod(const crJXJUIFubenSelectChangeMethod& handle);
	MethodClass(JXJ, JXJUIFubenSelectChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode* m_this;
	std::string m_strJiangLing;
	std::string m_strNeedLv;
	std::string m_strCanRelive;
	std::string m_strTechImg;
	std::string m_strDrop;
	std::string m_strDefaultImage;
// 	std::string	m_jiangling;
// 	std::string	m_binzhong;
// 	std::string	m_dixing;
// 	std::string	m_diaoluo;
// 	std::string	m_shuomin;
// 	std::string	m_headimg;
// 	std::string	m_jindu;
// 	std::string	m_relivecount;
};
class crJXJUIFubenClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFubenClickMethod();
	crJXJUIFubenClickMethod(const crJXJUIFubenClickMethod& handle);
	MethodClass(JXJ, JXJUIFubenClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_enterFuben;
	std::string m_info;
};
class crJXJCheckSelectRolesMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckSelectRolesMethod();
	crJXJCheckSelectRolesMethod(const crJXJCheckSelectRolesMethod& handle);
	MethodClass(JXJ, JXJCheckSelectRoles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_chekPostArmCount;
	bool m_sendtoserver;
};
class crJXJRecvCheckRolesMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvCheckRolesMethod();
	crJXJRecvCheckRolesMethod(const crJXJRecvCheckRolesMethod& handle);
	MethodClass(JXJ, JXJRecvCheckRoles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSyncFormationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSyncFormationMethod();
	crJXJSyncFormationMethod(const crJXJSyncFormationMethod& handle);
	MethodClass(JXJ, JXJSyncFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvSyncFormationMethod :public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJRecvSyncFormationMethod();
	crJXJRecvSyncFormationMethod(const crJXJRecvSyncFormationMethod& handle);
	MethodClass(JXJ, JXJRecvSyncFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJLoginGameSceneMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLoginGameSceneMethod();
	crJXJLoginGameSceneMethod(const crJXJLoginGameSceneMethod& handle);
	MethodClass(JXJ, JXJLoginGameScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_button;
};
class crJXJUIIsMultiFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIIsMultiFubenMethod();
	crJXJUIIsMultiFubenMethod(const crJXJUIIsMultiFubenMethod& handle);
	MethodClass(JXJ, JXJUIIsMultiFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
};
class crJXJUIInitMultiFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIInitMultiFubenMethod();
	crJXJUIInitMultiFubenMethod(const crJXJUIInitMultiFubenMethod& handle);
	MethodClass(JXJ, JXJUIInitMultiFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_multiFubenCanvas;
	std::string m_frame;
};
class crJXJHasTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHasTeamMethod();
	crJXJHasTeamMethod(const crJXJHasTeamMethod& handle);
	MethodClass(JXJ, JXJHasTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJCreateTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCreateTeamMethod();
	crJXJCreateTeamMethod(const crJXJCreateTeamMethod& handle);
	MethodClass(JXJ, JXJCreateTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvCreateTeamMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCreateTeamMethod();
	crJXJRecvCreateTeamMethod(const crJXJRecvCreateTeamMethod& handle);
	MethodClass(JXJ, JXJRecvCreateTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJLeaveTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeaveTeamMethod();
	crJXJLeaveTeamMethod(const crJXJLeaveTeamMethod& handle);
	MethodClass(JXJ, JXJLeaveTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvLeaveTeamMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvLeaveTeamMethod();
	crJXJRecvLeaveTeamMethod(const crJXJRecvLeaveTeamMethod& handle);
	MethodClass(JXJ, JXJRecvLeaveTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTeamClosedMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTeamClosedMethod();
	crJXJTeamClosedMethod(const crJXJTeamClosedMethod& handle);
	MethodClass(JXJ, JXJTeamClosed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvTeamClosedMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvTeamClosedMethod();
	crJXJRecvTeamClosedMethod(const crJXJRecvTeamClosedMethod& handle);
	MethodClass(JXJ, JXJRecvTeamClosed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQueryTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryTeamMethod();
	crJXJQueryTeamMethod(const crJXJQueryTeamMethod& handle);
	MethodClass(JXJ, JXJQueryTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_queryPage;
};
class crJXJRecvQueryTeamMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryTeamMethod();
	crJXJRecvQueryTeamMethod(const crJXJRecvQueryTeamMethod& handle);
	MethodClass(JXJ, JXJRecvQueryTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJJoinTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJJoinTeamMethod();
	crJXJJoinTeamMethod(const crJXJJoinTeamMethod& handle);
	MethodClass(JXJ, JXJJoinTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_teamTable;
};
class crJXJRecvJoinTeamMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvJoinTeamMethod();
	crJXJRecvJoinTeamMethod(const crJXJRecvJoinTeamMethod& handle);
	MethodClass(JXJ, JXJRecvJoinTeam)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	//std::string m_canvas2;
};
class crJXJRecvNewTeamMemberMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvNewTeamMemberMethod();
	crJXJRecvNewTeamMemberMethod(const crJXJRecvNewTeamMemberMethod& handle);
	MethodClass(JXJ, JXJRecvNewTeamMember)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJKickMemberFromTeamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJKickMemberFromTeamMethod();
	crJXJKickMemberFromTeamMethod(const crJXJKickMemberFromTeamMethod& handle);
	MethodClass(JXJ, JXJKickMemberFromTeam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_nameinput;
};
class crJXJRecvKickMemberFromTeamMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvKickMemberFromTeamMethod();
	crJXJRecvKickMemberFromTeamMethod(const crJXJRecvKickMemberFromTeamMethod& handle);
	MethodClass(JXJ, JXJRecvKickMemberFromTeam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTeamMemberReadyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTeamMemberReadyMethod();
	crJXJTeamMemberReadyMethod(const crJXJTeamMemberReadyMethod& handle);
	MethodClass(JXJ, JXJTeamMemberReady)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_value;
};
class crJXJRecvMemberReadyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvMemberReadyMethod();
	crJXJRecvMemberReadyMethod(const crJXJRecvMemberReadyMethod& handle);
	MethodClass(JXJ, JXJRecvMemberReady)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvMemberCancelReadyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvMemberCancelReadyMethod();
	crJXJRecvMemberCancelReadyMethod(const crJXJRecvMemberCancelReadyMethod& handle);
	MethodClass(JXJ, JXJRecvMemberCancelReady)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIUpdateTeamInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateTeamInfoMethod();
	crJXJUIUpdateTeamInfoMethod(const crJXJUIUpdateTeamInfoMethod& handle);
	MethodClass(JXJ, JXJUIUpdateTeamInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_join;
	std::string m_create;
	std::string m_player0;
	std::string m_player1;
	std::string m_player2;
	std::string m_player3;
	std::string m_player4;
	std::string m_switch;
	std::string m_playerReady1;
	std::string m_playerReady2;
	std::string m_playerReady3;
	std::string m_playerReady4;
	std::string m_btswitch;
	std::string m_btStart;
	std::string m_btStartSwitch;
	std::string m_btStartBattle;
};
class crJXJUpdateTeamListMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateTeamListMethod();
	crJXJUpdateTeamListMethod(const crJXJUpdateTeamListMethod& handle);
	MethodClass(JXJ, JXJUpdateTeamList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
    std::string m_canvas;
	std::string m_teamTable;
};
class crJXJUIFukuUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFukuUpdateMethod();
	crJXJUIFukuUpdateMethod(const crJXJUIFukuUpdateMethod& handle);
	MethodClass(JXJ, JXJUIFukuUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	CRCore::crVector4 m_normalColor;
	CRCore::crVector4 m_overColor;
	std::string m_fuKuList;
	std::string m_coppercash;
	std::string m_food;
	std::string m_wood;
	std::string m_horse;
	std::string m_iron;
	std::string m_jueSeCanvas;

};
//class crJXJJiangkaTimerMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJJiangkaTimerMethod();
//	crJXJJiangkaTimerMethod(const crJXJJiangkaTimerMethod& handle);
//	MethodClass(JXJ, JXJJiangkaTimer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	int m_dt;
//};
//class crJXJRecvHallJiangkaMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvHallJiangkaMethod();
//	crJXJRecvHallJiangkaMethod(const crJXJRecvHallJiangkaMethod& handle);
//	MethodClass(JXJ, JXJRecvHallJiangka)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJRecvPrivateJiangkaMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvPrivateJiangkaMethod();
//	crJXJRecvPrivateJiangkaMethod(const crJXJRecvPrivateJiangkaMethod& handle);
//	MethodClass(JXJ, JXJRecvPrivateJiangka)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJUIJiushiUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIJiushiUpdateMethod();
//	crJXJUIJiushiUpdateMethod(const crJXJUIJiushiUpdateMethod& handle);
//	MethodClass(JXJ, JXJUIJiushiUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_tavernRaido;
//	std::string m_tavernRoom;
//	std::string m_input;
//	std::string m_luckybtn;
//	std::string m_zhitianming;
//	//std::string m_card1;
//	//std::string m_card2;
//	//std::string m_card3;
//	//std::string m_card4;
//	//std::string m_card5;
//	//std::string m_cardBtn1;
//	//std::string m_cardBtn2;
//	//std::string m_cardBtn3;
//	//std::string m_cardBtn4;
//	//std::string m_cardBtn5;
//	//std::string m_pcard1;
//	//std::string m_pcard2;
//	//std::string m_pcard3;
//	//std::string m_pcardBtn1;
//	//std::string m_pcardBtn2;
//	//std::string m_pcardBtn3;
//	//std::string m_timer;
//	//std::string m_coppercashRefresh;
//	//std::string m_goldingotRefresh;
//};
//class crJXJLevyTimerMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJLevyTimerMethod();
//	crJXJLevyTimerMethod(const crJXJLevyTimerMethod& handle);
//	MethodClass(JXJ, JXJLevyTimer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	int m_dt;
//	int m_sec;
//};
class crJXJRecvZeroRefreshMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvZeroRefreshMethod();
	crJXJRecvZeroRefreshMethod(const crJXJRecvZeroRefreshMethod& handle);
	MethodClass(JXJ, JXJRecvZeroRefresh)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJRecvMainroleZeroRefreshMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvMainroleZeroRefreshMethod();
//	crJXJRecvMainroleZeroRefreshMethod(const crJXJRecvMainroleZeroRefreshMethod& handle);
//	MethodClass(JXJ, JXJRecvMainroleZeroRefresh)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
// class crJXJOnlineRewardTimerMethod : public CRCore::crMethod
// {//GameServer Method
// public:
// 	crJXJOnlineRewardTimerMethod();
// 	crJXJOnlineRewardTimerMethod(const crJXJOnlineRewardTimerMethod& handle);
// 	MethodClass(JXJ, JXJOnlineRewardTimer)
// 	virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// 	CRNetApp::crPlayerGameData* m_this;
// 	int m_dt;
// };
// class crJXJRecvOnlineRewardCountMethod :public CRCore::crMethod
// {//Client Method
// public:
// 	crJXJRecvOnlineRewardCountMethod();
// 	crJXJRecvOnlineRewardCountMethod(const crJXJRecvOnlineRewardCountMethod& handle);
// 	MethodClass(JXJ, JXJRecvOnlineRewardCount)
// 	virtual void operator()(crHandle &handle);
// 	virtual void inputParam(int i, void *param);
// 	virtual void addParam(int i, const std::string& str);
// protected:
// 	CRNetApp::crPlayerGameData *m_this;
// 	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
// 	int m_netType;
// };
//class crJXJBuyJunlingTimerMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJBuyJunlingTimerMethod();
//	crJXJBuyJunlingTimerMethod(const crJXJBuyJunlingTimerMethod& handle);
//	MethodClass(JXJ, JXJBuyJunlingTimer)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData* m_this;
//	int m_dt;
//	int m_sec;
//};
//class crJXJRecvBuyJunlingCountMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvBuyJunlingCountMethod();
//	crJXJRecvBuyJunlingCountMethod(const crJXJRecvBuyJunlingCountMethod& handle);
//	MethodClass(JXJ, JXJRecvBuyJunlingCount)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJUIRevenueUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIRevenueUpdateMethod();
	crJXJUIRevenueUpdateMethod(const crJXJUIRevenueUpdateMethod& handle);
	MethodClass(JXJ, JXJUIRevenueUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_mingjuCount;
	std::string m_populations;
	std::string m_coppercash;
	std::string m_revenueCount;
	std::string m_levy;
	std::string m_impress;
};
class crJXJLevyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLevyMethod();
	crJXJLevyMethod(const crJXJLevyMethod& handle);
	MethodClass(JXJ, JXJLevy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvLevyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvLevyMethod();
	crJXJRecvLevyMethod(const crJXJRecvLevyMethod& handle);
	MethodClass(JXJ, JXJRecvLevy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJImpressMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJImpressMethod();
	crJXJImpressMethod(const crJXJImpressMethod& handle);
	MethodClass(JXJ, JXJImpress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_goldColor;
};
class crJXJImpress2Method :public CRCore::crMethod
{//Client Method
public:
	crJXJImpress2Method();
	crJXJImpress2Method(const crJXJImpress2Method& handle);
	MethodClass(JXJ, JXJImpress2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//class crJXJRecvImpressMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvImpressMethod();
//	crJXJRecvImpressMethod(const crJXJRecvImpressMethod& handle);
//	MethodClass(JXJ, JXJRecvImpress)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJOnlineRewardMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJOnlineRewardMethod();
//	crJXJOnlineRewardMethod(const crJXJOnlineRewardMethod& handle);
//	MethodClass(JXJ, JXJOnlineReward)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crJXJRecvOnlineRewardMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvOnlineRewardMethod();
//	crJXJRecvOnlineRewardMethod(const crJXJRecvOnlineRewardMethod& handle);
//	MethodClass(JXJ, JXJRecvOnlineReward)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJHallRefreshMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJHallRefreshMethod();
//	crJXJHallRefreshMethod(const crJXJHallRefreshMethod& handle);
//	MethodClass(JXJ, JXJHallRefresh)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_msgbox;
//	std::string m_title;
//	std::string m_msg;
//	std::string m_ok;
//	CRCore::crVector4 m_copperColor;
//};
//class crJXJHallRefresh2Method :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJHallRefresh2Method();
//	crJXJHallRefresh2Method(const crJXJHallRefresh2Method& handle);
//	MethodClass(JXJ, JXJHallRefresh2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crJXJRecvHallRefreshMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvHallRefreshMethod();
//	crJXJRecvHallRefreshMethod(const crJXJRecvHallRefreshMethod& handle);
//	MethodClass(JXJ, JXJRecvHallRefresh)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
//class crJXJPrivateRefreshMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPrivateRefreshMethod();
//	crJXJPrivateRefreshMethod(const crJXJPrivateRefreshMethod& handle);
//	MethodClass(JXJ, JXJPrivateRefresh)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_msgbox;
//	std::string m_title;
//	std::string m_msg;
//	std::string m_ok;
//	CRCore::crVector4 m_goldColor;
//};
//class crJXJPrivateRefresh2Method :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPrivateRefresh2Method();
//	crJXJPrivateRefresh2Method(const crJXJPrivateRefresh2Method& handle);
//	MethodClass(JXJ, JXJPrivateRefresh2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crJXJRecvPrivateRefreshMethod :public CRCore::crMethod
//{//Client & GameServer Method
//public:
//	crJXJRecvPrivateRefreshMethod();
//	crJXJRecvPrivateRefreshMethod(const crJXJRecvPrivateRefreshMethod& handle);
//	MethodClass(JXJ, JXJRecvPrivateRefresh)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJUIGovPostOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGovPostOnShowMethod();
	crJXJUIGovPostOnShowMethod(const crJXJUIGovPostOnShowMethod& handle);
	MethodClass(JXJ, JXJUIGovPostOnShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_posticon;
	std::string m_upgrade;
	std::string m_curpost;
	std::string m_curarms;
	std::string m_exploit;
	std::string m_nextpost;
	std::string m_nextarms;
	std::string m_nextexploit;
	std::string m_lingqufenglubtn;
};
//class crJXJExploitRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJExploitRankingMethod();
//	crJXJExploitRankingMethod(const crJXJExploitRankingMethod& handle);
//	MethodClass(JXJ, JXJExploitRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveExploitRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveExploitRankingMethod();
	crJXJSaveExploitRankingMethod(const crJXJSaveExploitRankingMethod& handle);
	MethodClass(JXJ, JXJSaveExploitRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//std::vector<std::string> m_expRankingFiles;
	std::string m_expRankingFile;
};
//class crJXJLevelRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJLevelRankingMethod();
//	crJXJLevelRankingMethod(const crJXJLevelRankingMethod& handle);
//	MethodClass(JXJ, JXJLevelRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveLevelRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveLevelRankingMethod();
	crJXJSaveLevelRankingMethod(const crJXJSaveLevelRankingMethod& handle);
	MethodClass(JXJ, JXJSaveLevelRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
//class crJXJGovpostRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJGovpostRankingMethod();
//	crJXJGovpostRankingMethod(const crJXJGovpostRankingMethod& handle);
//	MethodClass(JXJ, JXJGovpostRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveGovpostRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveGovpostRankingMethod();
	crJXJSaveGovpostRankingMethod(const crJXJSaveGovpostRankingMethod& handle);
	MethodClass(JXJ, JXJSaveGovpostRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
//class crJXJAccomplishRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJAccomplishRankingMethod();
//	crJXJAccomplishRankingMethod(const crJXJAccomplishRankingMethod& handle);
//	MethodClass(JXJ, JXJAccomplishRanking)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveAccomplishRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveAccomplishRankingMethod();
	crJXJSaveAccomplishRankingMethod(const crJXJSaveAccomplishRankingMethod& handle);
	MethodClass(JXJ, JXJSaveAccomplishRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
//class crJXJTroopsRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJTroopsRankingMethod();
//	crJXJTroopsRankingMethod(const crJXJTroopsRankingMethod& handle);
//	MethodClass(JXJ, JXJTroopsRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveTroopsRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveTroopsRankingMethod();
	crJXJSaveTroopsRankingMethod(const crJXJSaveTroopsRankingMethod& handle);
	MethodClass(JXJ, JXJSaveTroopsRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
//class crJXJCountrycontributeRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJCountrycontributeRankingMethod();
//	crJXJCountrycontributeRankingMethod(const crJXJCountrycontributeRankingMethod& handle);
//	MethodClass(JXJ, JXJCountrycontributeRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveCountrycontributeRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveCountrycontributeRankingMethod();
	crJXJSaveCountrycontributeRankingMethod(const crJXJSaveCountrycontributeRankingMethod& handle);
	MethodClass(JXJ, JXJSaveCountrycontributeRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
//class crJXJManorlvRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJManorlvRankingMethod();
//	crJXJManorlvRankingMethod(const crJXJManorlvRankingMethod& handle);
//	MethodClass(JXJ, JXJManorlvRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveManorlvRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveManorlvRankingMethod();
	crJXJSaveManorlvRankingMethod(const crJXJSaveManorlvRankingMethod& handle);
	MethodClass(JXJ, JXJSaveManorlvRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_rankingFile;
};
class crJXJQueryGovPostRankingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryGovPostRankingMethod();
	crJXJQueryGovPostRankingMethod(const crJXJQueryGovPostRankingMethod& handle);
	MethodClass(JXJ, JXJQueryGovPostRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode *m_this;
	//int m_queryInterval;
	//unsigned char m_queryCount;
};
class crJXJRecvExploitRankingMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvExploitRankingMethod();
	crJXJRecvExploitRankingMethod(const crJXJRecvExploitRankingMethod& handle);
	MethodClass(JXJ, JXJRecvExploitRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
	std::string m_canvasName1;
};
class crJXJUIExploitRankingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIExploitRankingMethod();
	crJXJUIExploitRankingMethod(const crJXJUIExploitRankingMethod& handle);
	MethodClass(JXJ, JXJUIExploitRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_upgradeRequire;
	std::string m_upgrade;
	std::string m_rankingList;
};
struct SaleCountCheck
{
	SaleCountCheck():
		m_gtypeid(0),
		m_goodsid(0),
		m_count(0),
		m_code(0){}
	int m_gtypeid;
	int m_goodsid;
	int m_count;
	char m_code;//-1寄售条数上限 -2寄售数量上限 -3超出库存
};
class crJXJSaleCountValidCheckMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJSaleCountValidCheckMethod();
	crJXJSaleCountValidCheckMethod(const crJXJSaleCountValidCheckMethod& handle);
	MethodClass(JXJ, JXJSaleCountValidCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	SaleCountCheck *m_check;
};
class crJXJUISalePageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISalePageUpdateMethod();
	crJXJUISalePageUpdateMethod(const crJXJUISalePageUpdateMethod& handle);
	MethodClass(JXJ, JXJUISalePageUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_foodInventory;
	std::string m_woodInventory;
	std::string m_ironInventory;
	std::string m_horseInventory;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	std::string m_foodPrice;
	std::string m_woodPrice;
	std::string m_ironPrice;
	std::string m_horsePrice;
	std::string m_foodTotal;
	std::string m_woodTotal;
	std::string m_ironTotal;
	std::string m_horseTotal;
	std::string m_remainCount;
	std::string m_nextlvCount;
	std::string m_foodBtn;
	std::string m_woodBtn;
	std::string m_ironBtn;
	std::string m_horseBtn;
	//控制控件
	std::string m_foodCountControl;
	std::string m_woodCountControl;
	std::string m_ironCountControl;
	std::string m_horseCountControl;
	std::string m_foodPriceReduce;
	std::string m_woodPriceReduce;
	std::string m_ironPriceReduce;
	std::string m_horsePriceReduce;
	std::string m_foodPriceAdd;
	std::string m_woodPriceAdd;
	std::string m_ironPriceAdd;
	std::string m_horsePriceAdd;
};
class crJXJUISaleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISaleMethod();
	crJXJUISaleMethod(const crJXJUISaleMethod& handle);
	MethodClass(JXJ, JXJUISale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::crVector2i m_goodsType;
	std::string m_priceInput;
	std::string m_countInput;
};
class crJXJRecvSaleMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvSaleMethod();
	crJXJRecvSaleMethod(const crJXJRecvSaleMethod& handle);
	MethodClass(JXJ, JXJRecvSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_foodinput;
	std::string m_woodinput;
	std::string m_ironinput;
	std::string m_horseinput;
};
class crJXJSaleDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaleDisposeMethod();
	crJXJSaleDisposeMethod(const crJXJSaleDisposeMethod& handle);
	MethodClass(JXJ, JXJSaleDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_availableTime;
};
//class crJXJRefreshMySaleMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRefreshMySaleMethod();
//	crJXJRefreshMySaleMethod(const crJXJRefreshMySaleMethod& handle);
//	MethodClass(JXJ, JXJRefreshMySale)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//};
//class crJXJRecvRefreshMySaleMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvRefreshMySaleMethod();
//	crJXJRecvRefreshMySaleMethod(const crJXJRecvRefreshMySaleMethod& handle);
//	MethodClass(JXJ, JXJRecvRefreshMySale)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJOnMarketMySalePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOnMarketMySalePageMethod();
	crJXJOnMarketMySalePageMethod(const crJXJOnMarketMySalePageMethod& handle);
	MethodClass(JXJ, JXJOnMarketMySalePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvOnMarketMySalePageMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOnMarketMySalePageMethod();
	crJXJRecvOnMarketMySalePageMethod(const crJXJRecvOnMarketMySalePageMethod& handle);
	MethodClass(JXJ, JXJRecvOnMarketMySalePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJLeaveMarketMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJLeaveMarketMethod();
	crJXJLeaveMarketMethod(const crJXJLeaveMarketMethod& handle);
	MethodClass(JXJ, JXJLeaveMarket)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvLeaveMarketMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvLeaveMarketMethod();
	crJXJRecvLeaveMarketMethod(const crJXJRecvLeaveMarketMethod& handle);
	MethodClass(JXJ, JXJRecvLeaveMarket)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIMySalePageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMySalePageUpdateMethod();
	crJXJUIMySalePageUpdateMethod(const crJXJUIMySalePageUpdateMethod& handle);
	MethodClass(JXJ, JXJUIMySalePageUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	std::string showtime(int times);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_mySaleList;
	std::string m_pagenum;
};
class crJXJUICancelSaleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICancelSaleMethod();
	crJXJUICancelSaleMethod(const crJXJUICancelSaleMethod& handle);
	MethodClass(JXJ, JXJUICancelSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_mySaleList;
};
class crJXJRecvCancelSaleMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCancelSaleMethod();
	crJXJRecvCancelSaleMethod(const crJXJRecvCancelSaleMethod& handle);
	MethodClass(JXJ, JXJRecvCancelSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSaleListTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaleListTimerMethod();
	crJXJSaleListTimerMethod(const crJXJSaleListTimerMethod& handle);
	MethodClass(JXJ, JXJSaleListTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
};
class crJXJRecvSaleListTimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvSaleListTimerMethod();
	crJXJRecvSaleListTimerMethod(const crJXJRecvSaleListTimerMethod& handle);
	MethodClass(JXJ, JXJRecvSaleListTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJUIQueryPlayerSaleMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIQueryPlayerSaleMethod();
//	crJXJUIQueryPlayerSaleMethod(const crJXJUIQueryPlayerSaleMethod& handle);
//	MethodClass(JXJ, JXJUIQueryPlayerSale)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_goodsTypeRadio;
//	std::string m_pagenum;
//};
//class crJXJUIQueryPlayerSale2Method :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIQueryPlayerSale2Method();
//	crJXJUIQueryPlayerSale2Method(const crJXJUIQueryPlayerSale2Method& handle);
//	MethodClass(JXJ, JXJUIQueryPlayerSale2)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_goodsTypeRadio;
//};
//class crJXJRecvQueryPlayerSaleMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvQueryPlayerSaleMethod();
//	crJXJRecvQueryPlayerSaleMethod(const crJXJRecvQueryPlayerSaleMethod& handle);
//	MethodClass(JXJ, JXJRecvQueryPlayerSale)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJOnMarketPlayerSalePageMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJOnMarketPlayerSalePageMethod();
	crJXJOnMarketPlayerSalePageMethod(const crJXJOnMarketPlayerSalePageMethod& handle);
	MethodClass(JXJ, JXJOnMarketPlayerSalePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_goodsTypeRadio;
	std::string m_pagenum;
};
class crJXJRecvOnMarketPlayerSalePageMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvOnMarketPlayerSalePageMethod();
	crJXJRecvOnMarketPlayerSalePageMethod(const crJXJRecvOnMarketPlayerSalePageMethod& handle);
	MethodClass(JXJ, JXJRecvOnMarketPlayerSalePage)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQuerySaleDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJQuerySaleDisposeMethod();
	crJXJQuerySaleDisposeMethod(const crJXJQuerySaleDisposeMethod& handle);
	MethodClass(JXJ, JXJQuerySaleDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_availableTime;
	typedef std::map< CRCore::crVector2i,std::pair<int,float> > GoodsCountMap;
	GoodsCountMap m_goodsCountMap;
	float m_validterm;//
	typedef std::map< CRCore::crVector3i,std::pair<CRCore::ref_ptr<CRCore::crStreamBuf>,float> > GoodsRecordMap;
	GoodsRecordMap m_goodsRecordMap;
	float *m_dt;
};
class crJXJRecvPlayerSaleRecordsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPlayerSaleRecordsMethod();
	crJXJRecvPlayerSaleRecordsMethod(const crJXJRecvPlayerSaleRecordsMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerSaleRecords)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUIPlayerSaleListUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPlayerSaleListUpdateMethod();
	crJXJUIPlayerSaleListUpdateMethod(const crJXJUIPlayerSaleListUpdateMethod& handle);
	MethodClass(JXJ, JXJUIPlayerSaleListUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_playerSaleList;
	std::string m_pageNum;
	std::string m_buycountmaxinput;
};
class crJXJUIPlayerSaleListSelectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPlayerSaleListSelectMethod();
	crJXJUIPlayerSaleListSelectMethod(const crJXJUIPlayerSaleListSelectMethod& handle);
	MethodClass(JXJ, JXJUIPlayerSaleListSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crTableWidgetNode *m_this;
	std::string m_buyBtn;
};
class crJXJUIMySaleListSelectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMySaleListSelectMethod();
	crJXJUIMySaleListSelectMethod(const crJXJUIMySaleListSelectMethod& handle);
	MethodClass(JXJ, JXJUIMySaleListSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crTableWidgetNode *m_this;
	std::string m_cancelBtn;
};
class crJXJUIBuyPlayerSaleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyPlayerSaleMethod();
	crJXJUIBuyPlayerSaleMethod(const crJXJUIBuyPlayerSaleMethod& handle);
	MethodClass(JXJ, JXJUIBuyPlayerSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_copperColor;
	std::string m_playerSaleList;
	std::string m_goodsTypeRadio;
};
class crJXJUIBuyPlayerSale2Method :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuyPlayerSale2Method();
	crJXJUIBuyPlayerSale2Method(const crJXJUIBuyPlayerSale2Method& handle);
	MethodClass(JXJ, JXJUIBuyPlayerSale2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_playerSaleList;
};
class crJXJRecvBuyPlayerSaleMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvBuyPlayerSaleMethod();
	crJXJRecvBuyPlayerSaleMethod(const crJXJRecvBuyPlayerSaleMethod& handle);
	MethodClass(JXJ, JXJRecvBuyPlayerSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJBuySaleDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJBuySaleDisposeMethod();
	crJXJBuySaleDisposeMethod(const crJXJBuySaleDisposeMethod& handle);
	MethodClass(JXJ, JXJBuySaleDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float m_availableTime;
	typedef std::map<int,float> SaledMap;//saleid,validterm
	SaledMap m_saledMap;
	float m_validterm;//
	float *m_dt;
};
class crJXJUIGoldingotBuyStepCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGoldingotBuyStepCountMethod();
	crJXJUIGoldingotBuyStepCountMethod(const crJXJUIGoldingotBuyStepCountMethod& handle);
	MethodClass(JXJ, JXJUIGoldingotBuyStepCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_step;
	std::string m_input;
	CRCore::crVector2i m_goods;
};
class crJXJUIGoldingotBuyUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGoldingotBuyUpdateMethod();
	crJXJUIGoldingotBuyUpdateMethod(const crJXJUIGoldingotBuyUpdateMethod& handle);
	MethodClass(JXJ, JXJUIGoldingotBuyUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_foodPrice;
	std::string m_woodPrice;
	std::string m_ironPrice;
	std::string m_horsePrice;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	std::string m_foodTotal;
	std::string m_woodTotal;
	std::string m_ironTotal;
	std::string m_horseTotal;
	std::string m_allTotal;
	std::string m_buyBtn;
	std::string m_buyMaxInput;
	GoodsVec m_goodsVec;
};
class crJXJUICopperSaleStepCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICopperSaleStepCountMethod();
	crJXJUICopperSaleStepCountMethod(const crJXJUICopperSaleStepCountMethod& handle);
	MethodClass(JXJ, JXJUICopperSaleStepCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_step;
	CRCore::crVector2i m_goods;
	std::string m_input;
};
class crJXJUICopperSalePageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICopperSalePageUpdateMethod();
	crJXJUICopperSalePageUpdateMethod(const crJXJUICopperSalePageUpdateMethod& handle);
	MethodClass(JXJ, JXJUICopperSalePageUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_foodPrice;
	std::string m_woodPrice;
	std::string m_ironPrice;
	std::string m_horsePrice;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	std::string m_foodTotal;
	std::string m_woodTotal;
	std::string m_ironTotal;
	std::string m_horseTotal;
	std::string m_allTotal;
	std::string m_buyBtn;
	std::string m_foodInventory;
	std::string m_woodInventory;
	std::string m_ironInventory;
	std::string m_horseInventory;
	GoodsVec m_goodsVec;
};
class crJXJUIGoldBuyMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGoldBuyMethod();
	crJXJUIGoldBuyMethod(const crJXJUIGoldBuyMethod& handle);
	MethodClass(JXJ, JXJUIGoldBuy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_goldColor;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	std::string m_total;
	GoodsVec m_goodsVec;
};
class crJXJUIGoldBuy2Method :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGoldBuy2Method();
	crJXJUIGoldBuy2Method(const crJXJUIGoldBuy2Method& handle);
	MethodClass(JXJ, JXJUIGoldBuy2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	GoodsVec m_goodsVec;
};
class crJXJRecvGoldBuyMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGoldBuyMethod();
	crJXJRecvGoldBuyMethod(const crJXJRecvGoldBuyMethod& handle);
	MethodClass(JXJ, JXJRecvGoldBuy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_foodinput;
	std::string m_woodinput;
	std::string m_ironinput;
	std::string m_horseinput;

};
class crJXJMarketPageCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJMarketPageCheckMethod();
	crJXJMarketPageCheckMethod(const crJXJMarketPageCheckMethod& handle);
	MethodClass(JXJ, JXJMarketPageCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_marketCanvas;
	std::string m_pageRadio;
	int m_page;
};
class crJXJUICopperSaleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUICopperSaleMethod();
	crJXJUICopperSaleMethod(const crJXJUICopperSaleMethod& handle);
	MethodClass(JXJ, JXJUICopperSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_title;
	std::string m_msg;
	std::string m_ok;
	CRCore::crVector4 m_goldColor;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	std::string m_total;
	GoodsVec m_goodsVec;
};
class crJXJUICopperSale2Method :public CRCore::crMethod
{//Client Method
public:
	crJXJUICopperSale2Method();
	crJXJUICopperSale2Method(const crJXJUICopperSale2Method& handle);
	MethodClass(JXJ, JXJUICopperSale2)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_foodCount;
	std::string m_woodCount;
	std::string m_ironCount;
	std::string m_horseCount;
	GoodsVec m_goodsVec;
};
class crJXJRecvCopperSaleMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCopperSaleMethod();
	crJXJRecvCopperSaleMethod(const crJXJRecvCopperSaleMethod& handle);
	MethodClass(JXJ, JXJRecvCopperSale)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_foodinput;
	std::string m_woodinput;
	std::string m_ironinput;
	std::string m_horseinput;
};
class crJXJUITroopsEquipUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITroopsEquipUpdateMethod();
	crJXJUITroopsEquipUpdateMethod(const crJXJUITroopsEquipUpdateMethod& handle);
	MethodClass(JXJ, JXJUITroopsEquipUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode* m_this;
	std::string m_armyRadio;
};
class crJXJUITroopsEquipSelInfoUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITroopsEquipSelInfoUpdateMethod();
	crJXJUITroopsEquipSelInfoUpdateMethod(const crJXJUITroopsEquipSelInfoUpdateMethod& handle);
	MethodClass(JXJ, JXJUITroopsEquipSelInfoUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_equipList;
	std::string m_makeCount;
	std::string m_needCount[4];
	std::string m_needTotal[4];
	std::string m_makeBtn;
	std::string m_info;
};
class crJXJUIMakeTroopsEquipMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIMakeTroopsEquipMethod();
	crJXJUIMakeTroopsEquipMethod(const crJXJUIMakeTroopsEquipMethod& handle);
	MethodClass(JXJ, JXJUIMakeTroopsEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_equipList;
	std::string m_makeCount;
};
class crJXJRecvMakeTroopsEquipMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvMakeTroopsEquipMethod();
	crJXJRecvMakeTroopsEquipMethod(const crJXJRecvMakeTroopsEquipMethod& handle);
	MethodClass(JXJ, JXJRecvMakeTroopsEquip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_input;
	std::string m_canvas;
};
class crJXJUITroopsEquipTipInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUITroopsEquipTipInfoMethod();
	crJXJUITroopsEquipTipInfoMethod(const crJXJUITroopsEquipTipInfoMethod& handle);
	MethodClass(JXJ, JXJUITroopsEquipTipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRUI::crListControlWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info1;
	std::string m_info2;
};
class crJXJUIBuildBattlegroundListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildBattlegroundListMethod();
	crJXJUIBuildBattlegroundListMethod(const crJXJUIBuildBattlegroundListMethod& handle);
	MethodClass(JXJ, JXJUIBuildBattlegroundList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_battleList;
	std::string m_battleList2;
	std::string m_mapcanvas;
	std::string m_play;
	std::string m_enterbtn[7];
	std::string m_jiaobtn[7];
	std::string m_paibtn[7];
	float m_alpha;
	std::string m_roadimage[4];
	std::string m_border[3];
	std::string m_backboard;
	std::string m_bigmapimg;
	std::string m_colormask[3];
};
class crJXJUIBattlegroundUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBattlegroundUpdateMethod();
	crJXJUIBattlegroundUpdateMethod(const crJXJUIBattlegroundUpdateMethod& handle);
	MethodClass(JXJ, JXJUIBattlegroundUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_battleTimer;
	std::string m_battleList;
	std::string m_chengchiName;
	std::string m_gong[3];
	std::string m_shou;
	std::string m_enterBtn;
	short m_mintime;
	std::string m_select;
	std::string m_show_text;
	std::string m_scrollbarA;
	std::string m_scrollbarD;
	std::string m_shilisw[7];
	std::string m_canvastime;
	std::string m_canvasmap;
	std::string m_playfire;
	std::string m_battlelistD;
	std::string m_enterbtn[7];
	std::string m_chengzhanCanvas;
	std::string m_chengzhanSwitch;
	std::string m_attacksw[7];
	std::string m_qiehuan;
};
class crJXJUIBattlegroundUpdate1Method :public CRCore::crMethod
{//Client Method
public:
	crJXJUIBattlegroundUpdate1Method();
	crJXJUIBattlegroundUpdate1Method(const crJXJUIBattlegroundUpdate1Method& handle);
	MethodClass(JXJ, JXJUIBattlegroundUpdate1)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_battleTimer;
	std::string m_battleList;
	std::string m_chengchiName;
	std::string m_gong[3];
	std::string m_shou;
	std::string m_enterBtn;
	short m_mintime;
	std::string m_select;
	std::string m_show_text;
	std::string m_scrollbarA;
	std::string m_scrollbarD;
	std::string m_shilisw[7];
	std::string m_canvastime;
	std::string m_canvasmap;
	std::string m_playfire;
	std::string m_battlelistD;
	std::string m_enterbtn[7];
	std::string m_chengzhanCanvas;
	std::string m_chengzhanSwitch;
	std::string m_qiehuanshilisw;
	std::string m_radiowei;
	std::string m_radioshu;
	std::string m_radiowu;
	std::string m_attacksw[7];
};
class crJXJOpenBattleTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJOpenBattleTimerMethod();
	crJXJOpenBattleTimerMethod(const crJXJOpenBattleTimerMethod& handle);
	MethodClass(JXJ, JXJOpenBattleTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	time_t m_timer;
	//std::string m_chengchi2TabFile;
	//short m_settleTime;
};
class crJXJSaveChengchiMapMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveChengchiMapMethod();
	crJXJSaveChengchiMapMethod(const crJXJSaveChengchiMapMethod& handle);
	MethodClass(JXJ, JXJSaveChengchiMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//std::string m_chengchi2TabFile;
};
//class crJXJImmediatelySaveMethod : public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJImmediatelySaveMethod();
//	crJXJImmediatelySaveMethod(const crJXJImmediatelySaveMethod& handle);
//	MethodClass(JXJ, JXJImmediatelySave)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJSaveConsortiaMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveConsortiaMethod();
	crJXJSaveConsortiaMethod(const crJXJSaveConsortiaMethod& handle);
	MethodClass(JXJ, JXJSaveConsortia)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_saveCount;
};
class crJXJSaveShiliMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveShiliMethod();
	crJXJSaveShiliMethod(const crJXJSaveShiliMethod& handle);
	MethodClass(JXJ, JXJSaveShili)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void saveShili(crJXJShili *shili,CRNet::crNetConductor *dbConductor);
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJIsBattleOpenedMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJIsBattleOpenedMethod();
	crJXJIsBattleOpenedMethod(const crJXJIsBattleOpenedMethod& handle);
	MethodClass(JXJ, JXJIsBattleOpened)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};
class crJXJUIQueryBattleTimerMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIQueryBattleTimerMethod();
	crJXJUIQueryBattleTimerMethod(const crJXJUIQueryBattleTimerMethod& handle);
	MethodClass(JXJ, JXJUIQueryBattleTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};
class crJXJUIBuildFrientListMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildFrientListMethod();
	crJXJUIBuildFrientListMethod(const crJXJUIBuildFrientListMethod& handle);
	MethodClass(JXJ, JXJUIBuildFrientList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	//std::string m_friendlist;
};
class crJXJUIInviteUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIInviteUpdateMethod();
	crJXJUIInviteUpdateMethod(const crJXJUIInviteUpdateMethod& handle);
	MethodClass(JXJ, JXJUIInviteUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_inviteBtn;
	std::string m_friendlist;
	std::string m_selectInfo;
	std::string m_invitebtn[5];
	std::string m_pagenuminpue;
};
class crJXJUIBattlegroundInviteMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBattlegroundInviteMethod();
	crJXJUIBattlegroundInviteMethod(const crJXJUIBattlegroundInviteMethod& handle);
	MethodClass(JXJ, JXJUIBattlegroundInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_friendlist;
	int m_inviteindex;
};
class crJXJRecvInviteMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvInviteMethod();
	crJXJRecvInviteMethod(const crJXJRecvInviteMethod& handle);
	MethodClass(JXJ, JXJRecvInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJSetInviteMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetInviteMethod();
	crJXJSetInviteMethod(const crJXJSetInviteMethod& handle);
	MethodClass(JXJ, JXJSetInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
	std::string m_canvasFrame;
	std::string m_playername;
	std::string m_placername;
	std::string m_inviteyouenter;
	std::string m_myteam;
	std::string m_invitecanvas;
	std::string m_invitename;
};
class crJXJAcceptInviteMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAcceptInviteMethod();
	crJXJAcceptInviteMethod(const crJXJAcceptInviteMethod& handle);
	MethodClass(JXJ, JXJAcceptInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_punishcanvas;
	//std::string m_tipcanvas;
	//std::string m_tipcontent;
};
class crJXJRefuseInviteMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRefuseInviteMethod();
	crJXJRefuseInviteMethod(const crJXJRefuseInviteMethod& handle);
	MethodClass(JXJ, JXJRefuseInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//class crJXJRecvAcceptInviteMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvAcceptInviteMethod();
//	crJXJRecvAcceptInviteMethod(const crJXJRecvAcceptInviteMethod& handle);
//	MethodClass(JXJ, JXJRecvAcceptInvite)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJRecvRefuseInviteMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvRefuseInviteMethod();
	crJXJRecvRefuseInviteMethod(const crJXJRecvRefuseInviteMethod& handle);
	MethodClass(JXJ, JXJRecvRefuseInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJSetInvalidRequestMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJSetInvalidRequestMethod();
//	crJXJSetInvalidRequestMethod(const crJXJSetInvalidRequestMethod& handle);
//	MethodClass(JXJ, JXJSetInvalidRequest)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_canvasName;
//	std::string m_canvasFrame;
//	std::string m_text;
//};
class crJXJSetRefuseInviteMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetRefuseInviteMethod();
	crJXJSetRefuseInviteMethod(const crJXJSetRefuseInviteMethod& handle);
	MethodClass(JXJ, JXJSetRefuseInvite)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
//class crJXJSetAcceptInviteMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJSetAcceptInviteMethod();
//	crJXJSetAcceptInviteMethod(const crJXJSetAcceptInviteMethod& handle);
//	MethodClass(JXJ, JXJSetAcceptInvite)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_canvasName;
//	std::string m_canvasFrame;
//	std::string m_text;
//};
class crJXJPlayerLoginGameMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJPlayerLoginGameMethod();
	crJXJPlayerLoginGameMethod(const crJXJPlayerLoginGameMethod& handle);
	MethodClass(JXJ, JXJPlayerLoginGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRNetApp::crGameServerPlayerData *m_player;
};
class crJXJPlayerLoginSceneMethod :public CRCore::crMethod
{//SceneServer Method
public:
	crJXJPlayerLoginSceneMethod();
	crJXJPlayerLoginSceneMethod(const crJXJPlayerLoginSceneMethod& handle);
	MethodClass(JXJ, JXJPlayerLoginScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRNetApp::crSceneServerPlayerData *m_player;
};
class crJXJUISetBattlegroundGroupMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetBattlegroundGroupMethod();
	crJXJUISetBattlegroundGroupMethod(const crJXJUISetBattlegroundGroupMethod& handle);
	MethodClass(JXJ, JXJUISetBattlegroundGroup)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_inviteBtn;
	std::string m_frame;
	std::string m_players[5];
	std::string m_btStart;
	std::string m_playericon[5];
};
class crJXJUIShowBattlegroundBackboardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIShowBattlegroundBackboardMethod();
	crJXJUIShowBattlegroundBackboardMethod(const crJXJUIShowBattlegroundBackboardMethod& handle);
	MethodClass(JXJ, JXJUIShowBattlegroundBackboard)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	bool m_show;
	std::string m_canvas;
	std::string m_nodeName;
	std::string m_widgetName;
	std::string m_canvas2;//精彩活动
	std::string m_button;//城战按钮
	std::string m_DataCanvas;//
	std::string m_GuanAi;
	std::string m_jianguo;
};
class crJXJUISetBattlegroundNameMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetBattlegroundNameMethod();
	crJXJUISetBattlegroundNameMethod(const crJXJUISetBattlegroundNameMethod& handle);
	MethodClass(JXJ, JXJUISetBattlegroundName)
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

class crJXJUpdateGovPosMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUpdateGovPosMethod();
	crJXJUpdateGovPosMethod(const crJXJUpdateGovPosMethod& handle);
	MethodClass(JXJ, JXJUpdateGovPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvUpdateGovPosMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvUpdateGovPosMethod();
	crJXJRecvUpdateGovPosMethod(const crJXJRecvUpdateGovPosMethod& handle);
	MethodClass(JXJ, JXJRecvUpdateGovPos)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_donghua;
	std::string m_sequence;
};
class crJXJUIUpdateGovPosMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpdateGovPosMethod();
	crJXJUIUpdateGovPosMethod(const crJXJUIUpdateGovPosMethod& handle);
	MethodClass(JXJ,JXJUIUpdateGovPos)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_curpost;
	std::string m_curarms;
	std::string m_exploit;
	std::string m_nextpost;
	std::string m_nextarms;
	std::string m_nextexploit;
	std::string m_canvasName;
	std::string m_updateButton;
	std::string m_govpostpic;
};
class crJXJEnterBattleDequeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJEnterBattleDequeMethod();
	crJXJEnterBattleDequeMethod(const crJXJEnterBattleDequeMethod& handle);
	MethodClass(JXJ, JXJEnterBattleDeque)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	unsigned char m_shiliid;
};
class crJXJRecvEnterBattleDequeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvEnterBattleDequeMethod();
	crJXJRecvEnterBattleDequeMethod(const crJXJRecvEnterBattleDequeMethod& handle);
	MethodClass(JXJ, JXJRecvEnterBattleDeque)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJBattleDequeDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJBattleDequeDisposeMethod();
	crJXJBattleDequeDisposeMethod(const crJXJBattleDequeDisposeMethod& handle);
	MethodClass(JXJ, JXJBattleDequeDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	void playerEnterBattle(CRNet::crNetConductor *gameServer,CRNetApp::crGameServerPlayerData *playerData,int sceneid,int roomid,int battleid,int playerid);
	bool isBattleClosed(int roomid, int battleid);
	//void createBattle(CRNet::crNetConductor *gameServer,int sceneid,_crInt32 id);
	CRNetApp::crServerBrainHandle *m_this;
	float *m_dt;
	float m_removeDelay;
};
class crJXJSceneServerRemovedMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSceneServerRemovedMethod();
	crJXJSceneServerRemovedMethod(const crJXJSceneServerRemovedMethod& handle);
	MethodClass(JXJ, JXJSceneServerRemoved)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int m_sceneid;
};
class crJXJGameServerCreateRoomReturnMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerCreateRoomReturnMethod();
	crJXJGameServerCreateRoomReturnMethod(const crJXJGameServerCreateRoomReturnMethod& handle);
	MethodClass(JXJ, JXJGameServerCreateRoomReturn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crJXJCreateBattleDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCreateBattleDisposeMethod();
	crJXJCreateBattleDisposeMethod(const crJXJCreateBattleDisposeMethod& handle);
	MethodClass(JXJ, JXJCreateBattleDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	float *m_dt;
};
class crJXJRecvEnterBattleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvEnterBattleMethod();
	crJXJRecvEnterBattleMethod(const crJXJRecvEnterBattleMethod& handle);
	MethodClass(JXJ, JXJRecvEnterBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvBattleDequeWaitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleDequeWaitMethod();
	crJXJRecvBattleDequeWaitMethod(const crJXJRecvBattleDequeWaitMethod& handle);
	MethodClass(JXJ, JXJRecvBattleDequeWait)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
	int m_mintime;
	std::string m_canvas;
	std::string m_chenzhan;
	std::string m_jianguo;
};
class crJXJRecvBattlePunishMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattlePunishMethod();
	crJXJRecvBattlePunishMethod(const crJXJRecvBattlePunishMethod& handle);
	MethodClass(JXJ, JXJRecvBattlePunish)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvBattleCloseMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleCloseMethod();
	crJXJRecvBattleCloseMethod(const crJXJRecvBattleCloseMethod& handle);
	MethodClass(JXJ, JXJRecvBattleClose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvBattleCannotCreateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleCannotCreateMethod();
	crJXJRecvBattleCannotCreateMethod(const crJXJRecvBattleCannotCreateMethod& handle);
	MethodClass(JXJ, JXJRecvBattleCannotCreate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvBattleCapturedMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleCapturedMethod();
	crJXJRecvBattleCapturedMethod(const crJXJRecvBattleCapturedMethod& handle);
	MethodClass(JXJ, JXJRecvBattleCaptured)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJRecvBattleCreateFaildMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvBattleCreateFaildMethod();
	crJXJRecvBattleCreateFaildMethod(const crJXJRecvBattleCreateFaildMethod& handle);
	MethodClass(JXJ, JXJRecvBattleCreateFaild)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_netType;
	std::string m_noticeCanvas;
	std::string m_textWidget;
};
class crJXJExitBattleDequeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJExitBattleDequeMethod();
	crJXJExitBattleDequeMethod(const crJXJExitBattleDequeMethod& handle);
	MethodClass(JXJ, JXJExitBattleDeque)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvExitBattleDequeMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvExitBattleDequeMethod();
	crJXJRecvExitBattleDequeMethod(const crJXJRecvExitBattleDequeMethod& handle);
	MethodClass(JXJ, JXJRecvExitBattleDeque)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRefuseEnterBattleMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJRefuseEnterBattleMethod();
	crJXJRefuseEnterBattleMethod(const crJXJRefuseEnterBattleMethod& handle);
	MethodClass(JXJ, JXJRefuseEnterBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvRefuseEnterBattleMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvRefuseEnterBattleMethod();
	crJXJRecvRefuseEnterBattleMethod(const crJXJRecvRefuseEnterBattleMethod& handle);
	MethodClass(JXJ, JXJRecvRefuseEnterBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJAgreeEnterBattleMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJAgreeEnterBattleMethod();
	crJXJAgreeEnterBattleMethod(const crJXJAgreeEnterBattleMethod& handle);
	MethodClass(JXJ, JXJAgreeEnterBattle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//class crJXJBattleRemovedMethod : public CRCore::crMethod
//{//SceneServer Method
//public:
//	crJXJBattleRemovedMethod();
//	crJXJBattleRemovedMethod(const crJXJBattleRemovedMethod& handle);
//	MethodClass(JXJ, JXJBattleRemoved)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRoom *m_this;
//};
//class crJXJRecvBattleRemovedMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJRecvBattleRemovedMethod();
//	crJXJRecvBattleRemovedMethod(const crJXJRecvBattleRemovedMethod& handle);
//	MethodClass(JXJ, JXJRecvBattleRemoved)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
//class crJXJClientRecvBattleRemovedMethod :public CRCore::crMethod
//{//GameClient Method
//public:
//	crJXJClientRecvBattleRemovedMethod();
//	crJXJClientRecvBattleRemovedMethod(const crJXJClientRecvBattleRemovedMethod& handle);
//	MethodClass(JXJ, JXJClientRecvBattleRemoved)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJBuildTaskListMethod : public CRCore::crMethod
{
public:
	crJXJBuildTaskListMethod();
	crJXJBuildTaskListMethod(const crJXJBuildTaskListMethod& handle);
	MethodClass(JXJ, JXJBuildTaskList)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;
	std::string m_listName;
};
class crJXJUIUpdateTaskTraceMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUIUpdateTaskTraceMethod();
	crJXJUIUpdateTaskTraceMethod(const crJXJUIUpdateTaskTraceMethod& handle);
	MethodClass(JXJ, JXJUIUpdateTaskTrace)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_traceWidget;
	std::string m_newhandName;
	std::string m_taskswt;
	std::string m_skipTheBoot;
	std::string m_renWuCanvas;
	std::string m_zhezhaoCanvas;
	std::string m_scrollbar;
};
class crJXJUseBagItemMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJUseBagItemMethod();
	crJXJUseBagItemMethod(const crJXJUseBagItemMethod& handle);
	MethodClass(JXJ, JXJUseBagItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_itemid;
	int m_selectindex;
	bool m_isusing;
};
class crJXJRecvUseBagItemMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvUseBagItemMethod();
	crJXJRecvUseBagItemMethod(const crJXJRecvUseBagItemMethod& handle);
	MethodClass(JXJ, JXJRecvUseBagItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_bagcanvas;
	std::string m_tipcanvas;
	std::string m_tipcontent;
};
class crJXJServerOpenTreasureChestMethod :public CRCore::crMethod
{//GameServer Method,宝箱、礼包的开启（礼包里的掉落概率设定为100）
public:
	crJXJServerOpenTreasureChestMethod();
	crJXJServerOpenTreasureChestMethod(const crJXJServerOpenTreasureChestMethod& handle);
	MethodClass(JXJ, JXJServerOpenTreasureChest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crDataObject *m_this;
	CRNetApp::crPlayerGameData *m_user;
	int m_itemid;
	int m_selectid;
	char *m_useResult;
	char m_chestdroptype;	//是否根据VIP等级获取
};
class crJXJRecvOpenTreasureChestMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvOpenTreasureChestMethod();
	crJXJRecvOpenTreasureChestMethod(const crJXJRecvOpenTreasureChestMethod& handle);
	MethodClass(JXJ, JXJRecvOpenTreasureChest)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_tipcanvas;
	//std::string m_tipcontent;
};
class crJXJServerUseProduceCardMethod :public CRCore::crMethod
{//GameServer Method 一种道具只能对一类资源进行加成
public:
	crJXJServerUseProduceCardMethod();
	crJXJServerUseProduceCardMethod(const crJXJServerUseProduceCardMethod& handle);
	MethodClass(JXJ, JXJServerUseProduceCard)
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
class crJXJRecvUseProduceCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseProduceCardMethod();
	crJXJRecvUseProduceCardMethod(const crJXJRecvUseProduceCardMethod& handle);
	MethodClass(JXJ, JXJRecvUseProduceCard)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerUseVipCardMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJServerUseVipCardMethod();
	crJXJServerUseVipCardMethod(const crJXJServerUseVipCardMethod& handle);
	MethodClass(JXJ, JXJServerUseVipCard)
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
//class crJXJRecvUseVipCardMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJRecvUseVipCardMethod();
//	crJXJRecvUseVipCardMethod(const crJXJRecvUseVipCardMethod& handle);
//	MethodClass(JXJ, JXJRecvUseVipCard)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};
class crJXJServerUseResPackMethod :public CRCore::crMethod
{//GameServer Method 
public:
	crJXJServerUseResPackMethod();
	crJXJServerUseResPackMethod(const crJXJServerUseResPackMethod& handle);
	MethodClass(JXJ, JXJServerUseResPack)
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
class crJXJRecvUseResPackMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseResPackMethod();
	crJXJRecvUseResPackMethod(const crJXJRecvUseResPackMethod& handle);
	MethodClass(JXJ, JXJRecvUseResPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerUseTroopsEquipPackMethod :public CRCore::crMethod
{//GameServer Method 
public:
	crJXJServerUseTroopsEquipPackMethod();
	crJXJServerUseTroopsEquipPackMethod(const crJXJServerUseTroopsEquipPackMethod& handle);
	MethodClass(JXJ, JXJServerUseTroopsEquipPack)
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
class crJXJRecvUseTroopsEquipPackMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseTroopsEquipPackMethod();
	crJXJRecvUseTroopsEquipPackMethod(const crJXJRecvUseTroopsEquipPackMethod& handle);
	MethodClass(JXJ, JXJRecvUseTroopsEquipPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJServerUseTroopsPackMethod :public CRCore::crMethod
{//GameServer Method 
public:
	crJXJServerUseTroopsPackMethod();
	crJXJServerUseTroopsPackMethod(const crJXJServerUseTroopsPackMethod& handle);
	MethodClass(JXJ, JXJServerUseTroopsPack)
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
class crJXJRecvUseTroopsPackMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvUseTroopsPackMethod();
	crJXJRecvUseTroopsPackMethod(const crJXJRecvUseTroopsPackMethod& handle);
	MethodClass(JXJ, JXJRecvUseTroopsPack)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJGetRecruitSpeedMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetRecruitSpeedMethod();
	crJXJGetRecruitSpeedMethod(const crJXJGetRecruitSpeedMethod& handle);
	MethodClass(JXJ, JXJGetRecruitSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
};
class crJXJGetRecruitConsumeMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetRecruitConsumeMethod();
	crJXJGetRecruitConsumeMethod(const crJXJGetRecruitConsumeMethod& handle);
	MethodClass(JXJ, JXJGetRecruitConsume)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	double *m_output;
};
class crJXJGetGetBuildingLevelUpSpeedMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetGetBuildingLevelUpSpeedMethod();
	crJXJGetGetBuildingLevelUpSpeedMethod(const crJXJGetGetBuildingLevelUpSpeedMethod& handle);
	MethodClass(JXJ, JXJGetGetBuildingLevelUpSpeed)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
};
class crJXJGetGetBuildingLevelUpConsumeMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetGetBuildingLevelUpConsumeMethod();
	crJXJGetGetBuildingLevelUpConsumeMethod(const crJXJGetGetBuildingLevelUpConsumeMethod& handle);
	MethodClass(JXJ, JXJGetGetBuildingLevelUpConsume)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
};
class crJXJGetRevenueIncreaseMethod : public CRCore::crMethod
{//GameServer & Client Method
public:
	crJXJGetRevenueIncreaseMethod();
	crJXJGetRevenueIncreaseMethod(const crJXJGetRevenueIncreaseMethod& handle);
	MethodClass(JXJ, JXJGetRevenueIncrease)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	float *m_output;
};
class crJXJIsTaskCanActiveMethod : public CRCore::crMethod
{//GameServer & Client Metier Method
public:
	crJXJIsTaskCanActiveMethod();
	crJXJIsTaskCanActiveMethod(const crJXJIsTaskCanActiveMethod& handle);
	MethodClass(JXJ, JXJIsTaskCanActive)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRNetApp::crGameTask> m_task;
	bool *m_output;
};
class crJXJDisplayTaskGoalMethod : public CRCore::crMethod
{//Client Metier Method
public:
	crJXJDisplayTaskGoalMethod();
	crJXJDisplayTaskGoalMethod(const crJXJDisplayTaskGoalMethod& handle);
	MethodClass(JXJ, JXJDisplayTaskGoal)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRNetApp::crGameTask> m_task;
	CRUI::crHypertextWidgetNode *m_textWidget;
};
class crJXJGetTaskMethod : public CRCore::crMethod
{//GameClient & GameServer Method 
public:
	crJXJGetTaskMethod();
	crJXJGetTaskMethod(const crJXJGetTaskMethod& handle);
	MethodClass(JXJ, JXJGetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
};
class crJXJAdvanceTaskMethod : public CRCore::crMethod
{//GameClient & GameServer Method 
public:
	crJXJAdvanceTaskMethod();
	crJXJAdvanceTaskMethod(const crJXJAdvanceTaskMethod& handle);
	MethodClass(JXJ, JXJAdvanceTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_taskid;
	unsigned char m_stepType;
};
class crJXJReferTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJReferTaskMethod();
	crJXJReferTaskMethod(const crJXJReferTaskMethod& handle);
	MethodClass(JXJ, JXJReferTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_taskid;
	bool m_scriptadd;
};
class crJXJRecvGetTaskMethod : public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvGetTaskMethod();
	crJXJRecvGetTaskMethod(const crJXJRecvGetTaskMethod& handle);
	MethodClass(JXJ, JXJRecvGetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvAdvanceTaskMethod : public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvAdvanceTaskMethod();
	crJXJRecvAdvanceTaskMethod(const crJXJRecvAdvanceTaskMethod& handle);
	MethodClass(JXJ, JXJRecvAdvanceTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJTaskAwardMethod : public CRCore::crMethod
{//GameServer Method 
public:
	crJXJTaskAwardMethod();
	crJXJTaskAwardMethod(const crJXJTaskAwardMethod& handle);
	MethodClass(JXJ, JXJTaskAward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRNetApp::crGameTask> m_gameTask;
	bool *m_output;
};
class crJXJRecvReferTaskMethod : public CRCore::crMethod
{//GameServer && GameClient Method 
public:
	crJXJRecvReferTaskMethod();
	crJXJRecvReferTaskMethod(const crJXJRecvReferTaskMethod& handle);
	MethodClass(JXJ, JXJRecvReferTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_tipcanvas;
	std::string m_tipcontent;
};
class crJXJResetTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJResetTaskMethod();
	crJXJResetTaskMethod(const crJXJResetTaskMethod& handle);
	MethodClass(JXJ, JXJResetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_taskid;
	bool m_scriptadd;
};
class crJXJRecvResetTaskMethod : public CRCore::crMethod
{//GameServer 
public:
	crJXJRecvResetTaskMethod();
	crJXJRecvResetTaskMethod(const crJXJRecvResetTaskMethod& handle);
	MethodClass(JXJ, JXJRecvResetTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvCoppercashMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvCoppercashMethod();
	crJXJRecvCoppercashMethod(const crJXJRecvCoppercashMethod& handle);
	MethodClass(JXJ, JXJRecvCoppercash)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_coppercash;
};
class crJXJClientRecvCoppercashMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvCoppercashMethod();
	crJXJClientRecvCoppercashMethod(const crJXJClientRecvCoppercashMethod& handle);
	MethodClass(JXJ, JXJClientRecvCoppercash)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvGiftgoldMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvGiftgoldMethod();
	crJXJRecvGiftgoldMethod(const crJXJRecvGiftgoldMethod& handle);
	MethodClass(JXJ, JXJRecvGiftgold)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::MoneyChangeData* m_moneydata;
};
class crJXJClientRecvGiftgoldMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvGiftgoldMethod();
	crJXJClientRecvGiftgoldMethod(const crJXJClientRecvGiftgoldMethod& handle);
	MethodClass(JXJ, JXJClientRecvGiftgold)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvCountryContributeMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvCountryContributeMethod();
	crJXJRecvCountryContributeMethod(const crJXJRecvCountryContributeMethod& handle);
	MethodClass(JXJ, JXJRecvCountryContribute)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_countryContribute;
};
class crJXJClientRecvCountryContributeMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvCountryContributeMethod();
	crJXJClientRecvCountryContributeMethod(const crJXJClientRecvCountryContributeMethod& handle);
	MethodClass(JXJ, JXJClientRecvCountryContribute)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvCardPiecesMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJRecvCardPiecesMethod();
	crJXJRecvCardPiecesMethod(const crJXJRecvCardPiecesMethod& handle);
	MethodClass(JXJ, JXJRecvCardPieces)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	int m_jianghun;
};
class crJXJClientRecvCardPiecesMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvCardPiecesMethod();
	crJXJClientRecvCardPiecesMethod(const crJXJClientRecvCardPiecesMethod& handle);
	MethodClass(JXJ, JXJClientRecvCardPieces)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvActiveTaskVecMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJRecvActiveTaskVecMethod();
	crJXJRecvActiveTaskVecMethod(const crJXJRecvActiveTaskVecMethod& handle);
	MethodClass(JXJ, JXJRecvActiveTaskVec)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJNewHandUIInitMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewHandUIInitMethod();
	crJXJNewHandUIInitMethod(const crJXJNewHandUIInitMethod& handle);
	MethodClass(JXJ, JXJNewHandUIInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rectCanvasName;
	int m_taskid;
	std::string m_targetCanvasName;
	std::string m_targetWidgetName;
	std::string m_targetCanvasPos;
};
class crJXJSwitchModelMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJSwitchModelMethod();
	crJXJSwitchModelMethod(const crJXJSwitchModelMethod& handle);
	MethodClass(JXJ, JXJSwitchModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	CRCore::crMultiSwitch *m_swNode;
	std::string m_switchNodeName;
	int m_swvalue;
};
class crJXJTouchItemMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTouchItemMethod();
	crJXJTouchItemMethod(const crJXJTouchItemMethod& handle);
	MethodClass(JXJ, JXJTouchItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
};
//class crJXJAutoCheckAndReferTaskMethod : public CRCore::crMethod
//{//GameClient Method 
//public:
//	crJXJAutoCheckAndReferTaskMethod();
//	crJXJAutoCheckAndReferTaskMethod(const crJXJAutoCheckAndReferTaskMethod& handle);
//	MethodClass(JXJ, JXJAutoCheckAndReferTask)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//};
class crJXJCheckBuildingLvMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckBuildingLvMethod();
	crJXJCheckBuildingLvMethod(const crJXJCheckBuildingLvMethod& handle);
	MethodClass(JXJ, JXJCheckBuildingLv)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_buildingid;
	unsigned char m_lv;
	char m_trueExecCount;//-1表示无穷
};
class crJXJAutoReferMeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoReferMeMethod();
	crJXJAutoReferMeMethod(const crJXJAutoReferMeMethod& handle);
	MethodClass(JXJ, JXJAutoReferMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJAutoAdvanceMeMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoAdvanceMeMethod();
	crJXJAutoAdvanceMeMethod(const crJXJAutoAdvanceMeMethod& handle);
	MethodClass(JXJ, JXJAutoAdvanceMe)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	unsigned char m_stepType;
};

class crJXJNewHandBtnClickMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewHandBtnClickMethod();
	crJXJNewHandBtnClickMethod(const crJXJNewHandBtnClickMethod& handle);
	MethodClass(JXJ, JXJNewHandBtnClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_renwucanvas;
	std::string m_Barrackscanvas;
	std::string m_ArmyRaido;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaColorRadio;
	unsigned char m_flag;
	std::string m_jiangKaSystemRadio;
	std::string m_ArmySelRaido;
	std::string m_gongfangCanvas;
	std::string m_gongfangRaido;
	std::string m_kejiRaido;
};
class crJXJMainRoleInitMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJMainRoleInitMethod();
	crJXJMainRoleInitMethod(const crJXJMainRoleInitMethod& handle);
	MethodClass(JXJ, JXJMainRoleInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
};
class crJXJGameServerInitMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJGameServerInitMethod();
	crJXJGameServerInitMethod(const crJXJGameServerInitMethod& handle);
	MethodClass(JXJ, JXJGameServerInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int readVersion(const std::string &filename);
	std::string m_version;
};
class crJXJQuickUseBagItemMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQuickUseBagItemMethod();
	crJXJQuickUseBagItemMethod(const crJXJQuickUseBagItemMethod& handle);
	MethodClass(JXJ, JXJQuickUseBagItem)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crListControlWidgetNode *m_this;
	std::string m_tipcanvas;
	std::string m_tiptitle;
};
class crJXJAccomplishUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAccomplishUpdateMethod();
	crJXJAccomplishUpdateMethod(const crJXJAccomplishUpdateMethod& handle);
	MethodClass(JXJ, JXJAccomplishUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	float* m_dt;
};
class crJXJSearchFubenPassCountMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchFubenPassCountMethod();
	crJXJSearchFubenPassCountMethod(const crJXJSearchFubenPassCountMethod& handle);
	MethodClass(JXJ, JXJSearchFubenPassCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_mode;//0征战天下
};
class crJXJSearchFubenPassCountCompleteCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchFubenPassCountCompleteCheckMethod();
	crJXJSearchFubenPassCountCompleteCheckMethod(const crJXJSearchFubenPassCountCompleteCheckMethod& handle);
	MethodClass(JXJ, JXJSearchFubenPassCountCompleteCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_mode;//0征战天下
};
class crJXJRecvAccomplishCompleteMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvAccomplishCompleteMethod();
	crJXJRecvAccomplishCompleteMethod(const crJXJRecvAccomplishCompleteMethod& handle);
	MethodClass(JXJ, JXJRecvAccomplishComplete)
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
class crJXJUpdateRankingDataMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJUpdateRankingDataMethod();
	crJXJUpdateRankingDataMethod(const crJXJUpdateRankingDataMethod& handle);
	MethodClass(JXJ, JXJUpdateRankingData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	//CRNetApp::crGameServerPlayerData *m_playerData;
};
class crJXJUIAttachUpdateMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJUIAttachUpdateMethod();
	crJXJUIAttachUpdateMethod(const crJXJUIAttachUpdateMethod& handle);
	MethodClass(JXJ, JXJUIAttachUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	CRCore::ref_ptr<CRCore::crWidgetNode> m_targetWidget;
};
class crJXJEnterBattleTimerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJEnterBattleTimerMethod();
	crJXJEnterBattleTimerMethod(const crJXJEnterBattleTimerMethod& handle);
	MethodClass(JXJ, JXJEnterBattleTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_timer;
	int m_interval;
	std::string m_timerText;
};
class crJXJResetEnterBattleTimerMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJResetEnterBattleTimerMethod();
	crJXJResetEnterBattleTimerMethod(const crJXJResetEnterBattleTimerMethod& handle);
	MethodClass(JXJ, JXJResetEnterBattleTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_interval;
};
class crJXJCreateItemTipsMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJCreateItemTipsMethod();
	crJXJCreateItemTipsMethod(const crJXJCreateItemTipsMethod& handle);
	MethodClass(JXJ, JXJCreateItemTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static std::string getMagicAttrTipsStr(int magicstartid, const CREncapsulation::crTableIO::StrVec &magicrecord, char sign = ',');
protected:
	void setMagicAttrTips(int &nContent,int magicstartid,const CREncapsulation::crTableIO::StrVec &magicrecord,const std::vector<float> &color);
	//int getBaseAttrPercent(int startid,const CREncapsulation::crTableIO::StrVec &recrod);
	void createContent(int &nContent,std::string perstr,int base,int add,bool percent,bool baseattr,const std::vector<float>&color);
	CRCore::crVector2i m_item;
	rcfg::ConfigScript *m_outscript;
	JXJLordEquipVec m_otherLordEquipVec;
};
class crJXJCreateItemRefineTipsMethod : public crJXJCreateItemTipsMethod
{//GameClient Method
public:
	crJXJCreateItemRefineTipsMethod();
	crJXJCreateItemRefineTipsMethod(const crJXJCreateItemRefineTipsMethod& handle);
	MethodClass(JXJ, JXJCreateItemRefineTips)
	virtual void operator()(crHandle &handle);
};
//class crJXJConsortiaRankingMethod :public CRCore::crMethod
//{//GameServer Method
//public:
//	crJXJConsortiaRankingMethod();
//	crJXJConsortiaRankingMethod(const crJXJConsortiaRankingMethod& handle);
//	MethodClass(JXJ, JXJConsortiaRanking)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crServerBrainHandle *m_this;
//};
class crJXJPeopleModelGroupInitMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJPeopleModelGroupInitMethod();
	crJXJPeopleModelGroupInitMethod(const crJXJPeopleModelGroupInitMethod& handle);
	MethodClass(JXJ, JXJPeopleModelGroupInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_titleText;
	std::string m_enterSwitch;
};
class crExternSelectedGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crExternSelectedGameMethod();
	crExternSelectedGameMethod(const crExternSelectedGameMethod& handle);
	MethodClass(JXJ, ExternSelectedGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJGameLogMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameLogMethod();
	crJXJGameLogMethod(const crJXJGameLogMethod& handle);
	MethodClass(JXJ, JXJGameLog)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_playerid;
	CRNetApp::GameLogData m_gameLogData;
	//typedef std::set<unsigned short> LogSet;
	//LogSet m_logset;
};
class crJXJSceneLogMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJSceneLogMethod();
	crJXJSceneLogMethod(const crJXJSceneLogMethod& handle);
	MethodClass(JXJ, JXJSceneLog)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_playerid;
	CRNetApp::GameLogData m_gameLogData;
	//typedef std::set<unsigned short> LogSet;
	//LogSet m_logset;
};
class crJXJRechargeDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRechargeDisposeMethod();
	crJXJRechargeDisposeMethod(const crJXJRechargeDisposeMethod& handle);
	MethodClass(JXJ, JXJRechargeDispose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_count;
};
class crJXJMoneyChangeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJMoneyChangeMethod();
	crJXJMoneyChangeMethod(const crJXJMoneyChangeMethod& handle);
	MethodClass(JXJ, JXJMoneyChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::MoneyChangeData m_moneydata;//+表示获得，－表示消费
	bool *m_output;
};
class crJXJRecvMoneyChangeMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvMoneyChangeMethod();
	crJXJRecvMoneyChangeMethod(const crJXJRecvMoneyChangeMethod& handle);
	MethodClass(JXJ, JXJRecvMoneyChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJVipChangeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJVipChangeMethod();
	crJXJVipChangeMethod(const crJXJVipChangeMethod& handle);
	MethodClass(JXJ, JXJVipChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRNetApp::VipChangeData m_vipdata;//+表示获得，－表示消费
	bool *m_output;
};
class crJXJRecvVipChangeMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvVipChangeMethod();
	crJXJRecvVipChangeMethod(const crJXJRecvVipChangeMethod& handle);
	MethodClass(JXJ, JXJRecvVipChange)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvCheckPlayerGameNameMethod :public CRCore::crMethod
{//GameClient Method
public:
	crJXJRecvCheckPlayerGameNameMethod();
	crJXJRecvCheckPlayerGameNameMethod(const crJXJRecvCheckPlayerGameNameMethod& handle);
	MethodClass(JXJ, JXJRecvCheckPlayerGameName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_canvas;
	std::string m_widget;
};
class crJXJSendServerTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJSendServerTimerMethod();
	crJXJSendServerTimerMethod(const crJXJSendServerTimerMethod& handle);
	MethodClass(JXJ, JXJSendServerTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
};
class crJXJRecvServerTimerMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvServerTimerMethod();
	crJXJRecvServerTimerMethod(const crJXJRecvServerTimerMethod& handle);
	MethodClass(JXJ, JXJRecvServerTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	float m_delta;
};
class crJXJOnlineTimerUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJOnlineTimerUpdateMethod();
	crJXJOnlineTimerUpdateMethod(const crJXJOnlineTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJOnlineTimerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	char m_child;
	CRCore::crVector2i m_childinterval;
	float *m_dt;
};
class crJXJRecvChildCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvChildCheckMethod();
	crJXJRecvChildCheckMethod(const crJXJRecvChildCheckMethod& handle);
	MethodClass(JXJ, JXJRecvChildCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas1;//未验证
	std::string m_canvas2;//未成年
	std::string m_canvas3;//下线倒计时
	std::string m_timeText1;
	std::string m_timeText2;
};
class crJXJRobotMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRobotMethod();
	crJXJRobotMethod(const crJXJRobotMethod& handle);
	MethodClass(JXJ, JXJRobot)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	void battleAI();
protected:
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_robotTab;
	int m_currentState;
	int m_currentStep;
	time_t m_lastupdtime;
	std::string m_canvas;
	std::string m_cameraLockBtn;
};
class crJXJLoadingTipsMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingTipsMethod();
	crJXJLoadingTipsMethod(const crJXJLoadingTipsMethod& handle);
	MethodClass(JXJ, JXJLoadingTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRGUI::crStaticTextBoxElement* m_element;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_tipsTab;
	float m_updateInterval;
	float m_dt;
	float m_timer;
	int m_currentid;
};
class crJXJLoadingTips1Method : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingTips1Method();
	crJXJLoadingTips1Method(const crJXJLoadingTips1Method& handle);
	MethodClass(JXJ, JXJLoadingTips1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_tipsWidget;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_tipsTab;
	float m_updateInterval;
	float* m_dt;
	float m_timer;
};
class crJXJLoginMethod : public CRCore::crMethod
{////GameClient Method
public:
	crJXJLoginMethod();
	crJXJLoginMethod(const crJXJLoginMethod& handle);
	MethodClass(JXJ, JXJLogin)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
};
class crJXJLoginGameMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoginGameMethod();
	crJXJLoginGameMethod(const crJXJLoginGameMethod& handle);
	MethodClass(JXJ, JXJLoginGame)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_callback;
	std::string m_servername;
	std::string m_password;
};
class crJXJRegisterAccountMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJRegisterAccountMethod();
	crJXJRegisterAccountMethod(const crJXJRegisterAccountMethod& handle);
	MethodClass(JXJ, JXJRegisterAccount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_userNameWidget;
	std::string m_userPasswordWidget;
	std::string m_password2Widget;
	std::string m_emailWidget;
	std::string m_cdkeyWidget;
};
class crJXJDownloadFilesMethod : public CRCore::crMethod
{////GameClient Method下载文件
public:
	crJXJDownloadFilesMethod();
	crJXJDownloadFilesMethod(const crJXJDownloadFilesMethod& handle);
	MethodClass(JXJ, JXJDownloadFiles)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_msgid;//回调ClientEvent.cfg
	std::string m_filename;
};
class crJXJLoadUISceneMethod : public CRCore::crMethod
{////GameClient Method下载文件
public:
	crJXJLoadUISceneMethod();
	crJXJLoadUISceneMethod(const crJXJLoadUISceneMethod& handle);
	MethodClass(JXJ, JXJLoadUIScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_filename;
};
//class crJXJCheckAndDownFrontMethod : public CRCore::crMethod
//{////GameClient Method下载文件
//public:
//	crJXJCheckAndDownFrontMethod();
//	crJXJCheckAndDownFrontMethod(const crJXJCheckAndDownFrontMethod& handle);
//	MethodClass(JXJ, JXJCheckAndDownFront)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_frontname;
//};
class crJXJQueryGmMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryGmMethod();
	crJXJQueryGmMethod(const crJXJQueryGmMethod& handle);
	MethodClass(JXJ, JXJQueryGm)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_serverName;
	int m_serverid;
};
class crJXJQueryGmMapMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJQueryGmMapMethod();
	crJXJQueryGmMapMethod(const crJXJQueryGmMapMethod& handle);
	MethodClass(JXJ, JXJQueryGmMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJQueryNoChatMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJQueryNoChatMethod();
	crJXJQueryNoChatMethod(const crJXJQueryNoChatMethod& handle);
	MethodClass(JXJ, JXJQueryNoChat)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_serverid;
};
class crJXJQueryNoChatSetMethod : public CRCore::crMethod
{//SceneServer Method
public:
	crJXJQueryNoChatSetMethod();
	crJXJQueryNoChatSetMethod(const crJXJQueryNoChatSetMethod& handle);
	MethodClass(JXJ, JXJQueryNoChatSet)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJLoadingProgressMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingProgressMethod();
	crJXJLoadingProgressMethod(const crJXJLoadingProgressMethod& handle);
	MethodClass(JXJ, JXJLoadingProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	float *m_dt;
	float m_speed;
	std::string m_progressWidget;
	std::string m_donghua;
};
class crJXJLoadingProgressInitMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJLoadingProgressInitMethod();
	crJXJLoadingProgressInitMethod(const crJXJLoadingProgressInitMethod& handle);
	MethodClass(JXJ, JXJLoadingProgressInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_progressWidget;
};
class crJXJDisplayGameStartImgMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJDisplayGameStartImgMethod();
	crJXJDisplayGameStartImgMethod(const crJXJDisplayGameStartImgMethod& handle);
	MethodClass(JXJ, JXJDisplayGameStartImg)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gamestartImg;
};
class crJXJPlayerDataMonitorMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJPlayerDataMonitorMethod();
	crJXJPlayerDataMonitorMethod(const crJXJPlayerDataMonitorMethod& handle);
	MethodClass(JXJ, JXJPlayerDataMonitor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	bool m_init;
};
class crJXJDataMonitorMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJDataMonitorMethod();
	crJXJDataMonitorMethod(const crJXJDataMonitorMethod& handle);
	MethodClass(JXJ, JXJDataMonitor)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerShouchongMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerShouchongMethod :public CRCore::crMethod
{//Gameserver Method
public:
	crJXJPlayerShouchongMethod();
	crJXJPlayerShouchongMethod(const crJXJPlayerShouchongMethod& handle);
	MethodClass(JXJ, JXJPlayerShouchong)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJGetShouchongAwardMethod :public CRCore::crMethod
{//GameClient Method 领取首充奖励
public:
	crJXJGetShouchongAwardMethod();
	crJXJGetShouchongAwardMethod(const crJXJGetShouchongAwardMethod& handle);
	MethodClass(JXJ, JXJGetShouchongAward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJRecvShouchongAwardMethod :public CRCore::crMethod
{//Client  Method	& GameServer	 
public:
	crJXJRecvShouchongAwardMethod ();
	crJXJRecvShouchongAwardMethod (const crJXJRecvShouchongAwardMethod & handle);
	MethodClass(JXJ, JXJRecvShouchongAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJGainCountryCandidateVecTMethod: public CRCore::crMethod
{//GameServer Method
public:
	crJXJGainCountryCandidateVecTMethod();
	crJXJGainCountryCandidateVecTMethod(const crJXJGainCountryCandidateVecTMethod& handle);
	MethodClass(JXJ,JXJGainCountryCandidateVecT)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};
class crJXJCheckInManorMethod: public CRCore::crMethod
{//GameClient Method
public:
	crJXJCheckInManorMethod();
	crJXJCheckInManorMethod(const crJXJCheckInManorMethod& handle);
	MethodClass(JXJ,JXJCheckInManor)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJBattleRecruitCheckMethod :public CRCore::crMethod
{//Client & GameServer Method 战场征兵
public:
	crJXJBattleRecruitCheckMethod();
	crJXJBattleRecruitCheckMethod(const crJXJBattleRecruitCheckMethod& handle);
	MethodClass(JXJ, JXJBattleRecruitCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	TroopsMap* m_recruitTroopsMap;
	BattleRecruitCheckResult *m_result;
	//0:兵源充足VIP0补兵需要额外粮草 1:兵源充足 2:兵力不足，资源充足，直接扣除资源补兵 3:资源不足，扣除元宝补兵
	//-1:不能补兵 -2：粮草不足 -3:元宝不足 -4:资源不足 -6:铜钱不足 -7:今日可购资源量不足，不能购买！ -8:资源够买失败
};
class crJXJHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJHuodongUpdateMethod ();
	crJXJHuodongUpdateMethod (const crJXJHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJHuodongUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	float* m_dt;
};
class crJXJCheckPlayerGameNameMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckPlayerGameNameMethod();
	crJXJCheckPlayerGameNameMethod(const crJXJCheckPlayerGameNameMethod& handle);
	MethodClass(JXJ, JXJCheckPlayerGameName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_nameWidget;
};
class crJXJRecvNewHuodongActiveMethod :public CRCore::crMethod
{//Client  Method	 
public:
	crJXJRecvNewHuodongActiveMethod ();
	crJXJRecvNewHuodongActiveMethod (const crJXJRecvNewHuodongActiveMethod & handle);
	MethodClass(JXJ, JXJRecvNewHuodongActive)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJRecvNewHuodongStepMethod :public CRCore::crMethod
{//Client  Method	 
public:
	crJXJRecvNewHuodongStepMethod ();
	crJXJRecvNewHuodongStepMethod (const crJXJRecvNewHuodongStepMethod & handle);
	MethodClass(JXJ, JXJRecvNewHuodongStep)
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
//crJXJPlayerFriendListInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJPlayerFriendListInitMethod :public CRCore::crMethod
{//Gameserver Method
public:
	crJXJPlayerFriendListInitMethod();
	crJXJPlayerFriendListInitMethod(const crJXJPlayerFriendListInitMethod& handle);
	MethodClass(JXJ, JXJPlayerFriendListInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
};
class crJXJTaskCheckFormationMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJTaskCheckFormationMethod();
	crJXJTaskCheckFormationMethod(const crJXJTaskCheckFormationMethod& handle);
	MethodClass(JXJ, JXJTaskCheckFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	CRCore::crVector3i m_formation[3];//兵种id，是否有主卡，是否有被卡
};
class crJXJSetWindowsTitleMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJSetWindowsTitleMethod();
	crJXJSetWindowsTitleMethod(const crJXJSetWindowsTitleMethod& handle);
	MethodClass(JXJ, JXJSetWindowsTitle)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJHuodongExtUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJHuodongExtUpdateMethod();
	crJXJHuodongExtUpdateMethod(const crJXJHuodongExtUpdateMethod& handle);
	MethodClass(JXJ, JXJHuodongExtUpdate)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_step;
};
class crJXJDeductHuodongUpdateMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJDeductHuodongUpdateMethod();
	crJXJDeductHuodongUpdateMethod(const crJXJDeductHuodongUpdateMethod& handle);
	MethodClass(JXJ, JXJDeductHuodongUpdate)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	int m_step;
};
class crJXJRechargeHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//累计充值
public:
	crJXJRechargeHuodongUpdateMethod ();
	crJXJRechargeHuodongUpdateMethod (const crJXJRechargeHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJRechargeHuodongUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_addstep;
	int m_playerid;
};
class crJXJSingleRechargeHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//单笔充值
public:
	crJXJSingleRechargeHuodongUpdateMethod ();
	crJXJSingleRechargeHuodongUpdateMethod (const crJXJSingleRechargeHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJSingleRechargeHuodongUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
};
class crJXJSingleRechargeBetweenHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//单笔充值
public:
	crJXJSingleRechargeBetweenHuodongUpdateMethod ();
	crJXJSingleRechargeBetweenHuodongUpdateMethod (const crJXJSingleRechargeBetweenHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJSingleRechargeBetweenHuodongUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
};
class crJXJConsumptionHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//累计消耗
public:
	crJXJConsumptionHuodongUpdateMethod ();
	crJXJConsumptionHuodongUpdateMethod (const crJXJConsumptionHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJConsumptionHuodongUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
	int m_type;////0表示礼金+元宝消费，1表示元宝消费
};
class crJXJZhengZhanTianXiaHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//征战天下
public:
	crJXJZhengZhanTianXiaHuodongUpdateMethod ();
	crJXJZhengZhanTianXiaHuodongUpdateMethod (const crJXJZhengZhanTianXiaHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJZhengZhanTianXiaHuodongUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
};
class crJXJMingJiangZhiLuHuoDongMethod :public CRCore::crMethod
{//GameServer Method
	//名将之路 攻城掠地
public:
	crJXJMingJiangZhiLuHuoDongMethod ();
	crJXJMingJiangZhiLuHuoDongMethod (const crJXJMingJiangZhiLuHuoDongMethod & handle);
	MethodClass(JXJ, JXJMingJiangZhiLuHuoDong)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
};
class crJXJXueZhanShaChangUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//血战沙场 视死如归
public:
	crJXJXueZhanShaChangUpdateMethod ();
	crJXJXueZhanShaChangUpdateMethod (const crJXJXueZhanShaChangUpdateMethod & handle);
	MethodClass(JXJ, JXJXueZhanShaChangUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_step;
	int m_playerid;
};

class crJXJLianXuChongZhiHuodongUpdateMethod :public CRCore::crMethod
{//GameServer Method
	//
public:
	crJXJLianXuChongZhiHuodongUpdateMethod ();
	crJXJLianXuChongZhiHuodongUpdateMethod (const crJXJLianXuChongZhiHuodongUpdateMethod & handle);
	MethodClass(JXJ, JXJLianXuChongZhiHuodongUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	int m_addstep;
	int m_playerid;
};
class crJXJCheckFormationMethod :public CRCore::crMethod
{//Client Method 判断编队是否满
public:
	crJXJCheckFormationMethod();
	crJXJCheckFormationMethod(const crJXJCheckFormationMethod& handle);
	MethodClass(JXJ, JXJCheckFormation)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class crJXJCheckFormationBtnMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckFormationBtnMethod();
	crJXJCheckFormationBtnMethod(const crJXJCheckFormationBtnMethod& handle);
	MethodClass(JXJ, JXJCheckFormationBtn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_sequence;
};
class crJXJPlatformCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPlatformCheckMethod();
	crJXJPlatformCheckMethod(const crJXJPlatformCheckMethod& handle);
	MethodClass(JXJ, JXJPlatformCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_platformstr;
};
class crJXJOnlineStateMethod :public CRCore::crMethod
{//GameServer Method
	//
public:
	crJXJOnlineStateMethod ();
	crJXJOnlineStateMethod (const crJXJOnlineStateMethod & handle);
	MethodClass(JXJ, JXJOnlineState)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	bool m_online;
};
class crJXJClientChangeNameMethod: public CRCore::crMethod
{//Client 
public:
	crJXJClientChangeNameMethod();
	crJXJClientChangeNameMethod(const crJXJClientChangeNameMethod& handle);
	MethodClass(JXJ,JXJClientChangeName)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_nameedit;
	int m_maxsize;
};
class crJXJRecvChangeNameMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeNameMethod();
	crJXJRecvChangeNameMethod(const crJXJRecvChangeNameMethod& handle);
	MethodClass(JXJ, JXJRecvChangeName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	int m_maxsize;
};
class crJXJUIJKSetShiliSWMethod: public CRCore::crMethod
{//Client 
public:
	crJXJUIJKSetShiliSWMethod();
	crJXJUIJKSetShiliSWMethod(const crJXJUIJKSetShiliSWMethod& handle);
	MethodClass(JXJ,JXJUIJKSetShiliSW)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw1;
	std::string m_sw2;
};

class crJXJUIJKComposeUpdateMethod: public CRCore::crMethod
{//Client 
public:
	crJXJUIJKComposeUpdateMethod();
	crJXJUIJKComposeUpdateMethod(const crJXJUIJKComposeUpdateMethod& handle);
	MethodClass(JXJ,JXJUIJKComposeUpdate)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jkGuojieRaido;
	std::string m_jtouxiangRaido;
	std::string m_jiangkaImage;
	std::string m_jiangkaInfo;
	std::string m_jiangkaText;
	std::string m_jk_DikaRaido;
	std::string m_jkzu_Jineng[3];
	std::string m_jkzh_Wenb[3];
	std::string m_jkzh_Wenb_X[3];
	std::string m_jkzh_Wenb_S[3];
	std::string m_jk_Tianming[3];
	std::string m_jkzh_JinDu3[3];
	std::string m_jkTiankaPer;
	std::string m_jkTiankaNext;
	std::string m_jkDikaPer;
	std::string m_jkDikaNext;
	std::string m_jiangkaName[10];
	std::string m_jihuosw[3];
	std::string m_jihuobtn[3];
	std::string m_jihuoTiaojianSw[3];
	std::string m_jihuoTiaojianText[3];
	std::string m_tiananniu[3];
};
class crJXJUITiankaWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUITiankaWalkMethod();
	crJXJUITiankaWalkMethod(const crJXJUITiankaWalkMethod& handle);
	MethodClass(JXJ, JXJUITiankaWalk)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_step;
};
class crJXJUIDikaWalkMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIDikaWalkMethod();
	crJXJUIDikaWalkMethod(const crJXJUIDikaWalkMethod& handle);
	MethodClass(JXJ, JXJUIDikaWalk)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_step;
};
class crJXJUIDikaWalkResetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIDikaWalkResetMethod();
	crJXJUIDikaWalkResetMethod(const crJXJUIDikaWalkResetMethod& handle);
	MethodClass(JXJ, JXJUIDikaWalkReset)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJUIActiveCardExtraMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIActiveCardExtraMethod();
	crJXJUIActiveCardExtraMethod(const crJXJUIActiveCardExtraMethod& handle);
	MethodClass(JXJ, JXJUIActiveCardExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_promptCanvas;
	std::string m_information;
	std::string m_okbtn;
};
class crJXJActiveCardExtraMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJActiveCardExtraMethod();
	crJXJActiveCardExtraMethod(const crJXJActiveCardExtraMethod& handle);
	MethodClass(JXJ, JXJActiveCardExtra)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
};
class crJXJRecvActiveCardExtraMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvActiveCardExtraMethod();
	crJXJRecvActiveCardExtraMethod(const crJXJRecvActiveCardExtraMethod& handle);
	MethodClass(JXJ, JXJRecvActiveCardExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJUICardShopOpenCheckMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUICardShopOpenCheckMethod();
	crJXJUICardShopOpenCheckMethod(const crJXJUICardShopOpenCheckMethod& handle);
	MethodClass(JXJ, JXJUICardShopOpenCheck)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_cardshop;
};
class crJXJGetMultiSwitchValueMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGetMultiSwitchValueMethod();
	crJXJGetMultiSwitchValueMethod(const crJXJGetMultiSwitchValueMethod& handle);
	MethodClass(JXJ, JXJGetMultiSwitchValue)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
};
class crJXJSetCanvasPositionMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSetCanvasPositionMethod();
	crJXJSetCanvasPositionMethod(const crJXJSetCanvasPositionMethod& handle);
	MethodClass(JXJ, JXJSetCanvasPosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	CRCore::crVector3 m_pos;
};
class crJXJCreateCharacterTimerMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCreateCharacterTimerMethod();
	crJXJCreateCharacterTimerMethod(const crJXJCreateCharacterTimerMethod& handle);
	MethodClass(JXJ, JXJCreateCharacterTimer)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_setupBtn;
	std::string m_timetext;
	CRCore::Timer_t m_timer;
};
class crJXJCreateCharacterTimerCancelMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCreateCharacterTimerCancelMethod();
	crJXJCreateCharacterTimerCancelMethod(const crJXJCreateCharacterTimerCancelMethod& handle);
	MethodClass(JXJ, JXJCreateCharacterTimerCancel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJUIPlayFxMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIPlayFxMethod();
	crJXJUIPlayFxMethod(const crJXJUIPlayFxMethod& handle);
	MethodClass(JXJ, JXJUIPlayFx)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_interval;
	std::string m_donghua;
};
class crJXJOpinionMethod: public CRCore::crMethod
{//Client 
public:
	crJXJOpinionMethod();
	crJXJOpinionMethod(const crJXJOpinionMethod& handle);
	MethodClass(JXJ,JXJOpinion)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_edit;
	CRCore::crVector2i m_sizeRange;
};
class crJXJRecvOpinionMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvOpinionMethod();
	crJXJRecvOpinionMethod(const crJXJRecvOpinionMethod& handle);
	MethodClass(JXJ, JXJRecvOpinion)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	CRCore::crVector2i m_sizeRange;
	std::string m_canvas;
	std::string m_edit;
	CRCore::crVector4 m_color;
};
class crJXJTextColorEqualMethod: public CRCore::crMethod
{//Client 
public:
	crJXJTextColorEqualMethod();
	crJXJTextColorEqualMethod(const crJXJTextColorEqualMethod& handle);
	MethodClass(JXJ,JXJTextColorEqual)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crTextAttrWidgetNode* m_this;
	CRCore::crVector4 m_color;
};
class crJXJClearTextMethod: public CRCore::crMethod
{//Client 
public:
	crJXJClearTextMethod();
	crJXJClearTextMethod(const crJXJClearTextMethod& handle);
	MethodClass(JXJ,JXJClearText)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crStaticTextWidgetNode* m_this;
};
class crJXJTextEmptyMethod: public CRCore::crMethod
{//Client 
public:
	crJXJTextEmptyMethod();
	crJXJTextEmptyMethod(const crJXJTextEmptyMethod& handle);
	MethodClass(JXJ,JXJTextEmpty)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_textNode;
};
class crJXJSetCodeTextMethod: public CRCore::crMethod
{//Client 
public:
	crJXJSetCodeTextMethod();
	crJXJSetCodeTextMethod(const crJXJSetCodeTextMethod& handle);
	MethodClass(JXJ,JXJSetCodeText)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_textNode;
	std::string m_textid;
	CRCore::crVector4 m_color;
};
class crJXJSetHypertextMethod: public CRCore::crMethod
{//Client 
public:
	crJXJSetHypertextMethod();
	crJXJSetHypertextMethod(const crJXJSetHypertextMethod& handle);
	MethodClass(JXJ,JXJSetHypertext)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crHypertextWidgetNode* m_this;
	std::string m_textFile;
};
class crJXJRadioWidgetInitSelectMethod: public CRCore::crMethod
{//Client 
public:
	crJXJRadioWidgetInitSelectMethod();
	crJXJRadioWidgetInitSelectMethod(const crJXJRadioWidgetInitSelectMethod& handle);
	MethodClass(JXJ,JXJRadioWidgetInitSelect)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_radioGroup;
	int m_selectid;//-1表示不选
};
class crJXJPlatformCaseMethod : public CRCore::crMethod
{//GameClient Method
public:
	crJXJPlatformCaseMethod();
	crJXJPlatformCaseMethod(const crJXJPlatformCaseMethod& handle);
	MethodClass(JXJ, JXJPlatformCase)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::map<std::string,int> m_platformmap;
};
class crJXJBuyGoldURLMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBuyGoldURLMethod();
	crJXJBuyGoldURLMethod(const crJXJBuyGoldURLMethod& handle);
	MethodClass(JXJ, JXJBuyGoldURL)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_recharge;
};
class crJXJUIGoToSiteURLMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGoToSiteURLMethod();
	crJXJUIGoToSiteURLMethod(const crJXJUIGoToSiteURLMethod& handle);
	MethodClass(JXJ, JXJUIGoToSiteURL)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_title;
};
//class crJXJBuyGoldURLQQMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJBuyGoldURLQQMethod();
//	crJXJBuyGoldURLQQMethod(const crJXJBuyGoldURLQQMethod& handle);
//	MethodClass(JXJ, JXJBuyGoldURLQQ)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_recharge;
//};
//class crJXJBuyGoldURLSiteMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJBuyGoldURLSiteMethod();
//	crJXJBuyGoldURLSiteMethod(const crJXJBuyGoldURLSiteMethod& handle);
//	MethodClass(JXJ, JXJBuyGoldURLSite)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	std::string m_url;
//};
class crJXJSetUIScaleFactorTextMethod: public CRCore::crMethod
{//Client 
public:
	crJXJSetUIScaleFactorTextMethod();
	crJXJSetUIScaleFactorTextMethod(const crJXJSetUIScaleFactorTextMethod& handle);
	MethodClass(JXJ,JXJSetUIScaleFactorText)
	virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crSliderWidgetNode* m_this;
	std::string m_text;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTimerCanvasInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTimerCanvasInitMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJTimerCanvasInitMethod();
	crJXJTimerCanvasInitMethod(const crJXJTimerCanvasInitMethod &handle);
	MethodClass(JXJ,JXJTimerCanvasInit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode *m_this;
	short m_interval;
	std::string m_text;
	int m_texid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTimerCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTimerCanvasUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJTimerCanvasUpdateMethod();
	crJXJTimerCanvasUpdateMethod(const crJXJTimerCanvasUpdateMethod &handle);
	MethodClass(JXJ,JXJTimerCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode *m_this;
	std::string m_text;
	std::string m_button;
	int m_texid;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJTelhuodongMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJTelhuodongMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJTelhuodongMethod();
	crJXJTelhuodongMethod(const crJXJTelhuodongMethod &handle);
	MethodClass(JXJ,JXJTelhuodong)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crWidgetNode *m_this;
	std::string m_text;
};
class crJXJRecvTelhuodongMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvTelhuodongMethod();
	crJXJRecvTelhuodongMethod(const crJXJRecvTelhuodongMethod& handle);
	MethodClass(JXJ, JXJRecvTelhuodong)
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
//crJXJQiyuCanvasInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJQiyuCanvasInitMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJQiyuCanvasInitMethod();
	crJXJQiyuCanvasInitMethod(const crJXJQiyuCanvasInitMethod &handle);
	MethodClass(JXJ,JXJQiyuCanvasInit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRCore::crCanvasNode *m_this;
	std::string m_taskname;
	std::string m_taskdes;
	std::string m_taskaward;
};
/////////////////////////////////////////
//
//crJXJCheckQiyuTaskMethod
//
/////////////////////////////////////////
class crJXJCheckQiyuTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckQiyuTaskMethod();
	crJXJCheckQiyuTaskMethod(const crJXJCheckQiyuTaskMethod& handle);
	MethodClass(JXJ, JXJCheckQiyuTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJAddExtraProduceMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJAddExtraProduceMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJAddExtraProduceMethod();
	crJXJAddExtraProduceMethod(const crJXJAddExtraProduceMethod& handle);
	MethodClass(JXJ, JXJAddExtraProduce)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::crVector3i m_award;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvAddExtraProduceMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvAddExtraProduceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvAddExtraProduceMethod();
	crJXJRecvAddExtraProduceMethod(const crJXJRecvAddExtraProduceMethod& handle);
	MethodClass(JXJ, JXJRecvAddExtraProduce)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};
class crJXJQiyuTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJQiyuTipsMethod();
	crJXJQiyuTipsMethod(const crJXJQiyuTipsMethod& handle);
	MethodClass(JXJ,JXJQiyuTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_taskname;
	std::string m_tiptext;
};
class crJXJSearchRoleDataParamMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSearchRoleDataParamMethod();
	crJXJSearchRoleDataParamMethod(const crJXJSearchRoleDataParamMethod& handle);
	MethodClass(JXJ, JXJSearchRoleDataParam)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	int m_paramid;
	unsigned char m_datatype;//
};

class crJXJSearchRoleDataParamCheckMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSearchRoleDataParamCheckMethod();
	crJXJSearchRoleDataParamCheckMethod(const crJXJSearchRoleDataParamCheckMethod& handle);
	MethodClass(JXJ, JXJSearchRoleDataParamCheck)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crJXJAccomplish *m_this;
	CRNetApp::crPlayerGameData *m_player;
	int m_paramid;
	unsigned char m_datatype;//
};
class crJXJMyCounrtyShopCanvasUpdateNewMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyCounrtyShopCanvasUpdateNewMethod();
	crJXJMyCounrtyShopCanvasUpdateNewMethod(const crJXJMyCounrtyShopCanvasUpdateNewMethod& handle);
	MethodClass(JXJ, JXJMyCounrtyShopCanvasUpdateNew)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this; 
	std::string m_icon[12];
	std::string m_num[12];
	std::string m_price[12];
	std::string m_buybtn[12];
	std::string m_scroll;
	std::string m_typeRadio;
	std::string m_contribute;
	std::string m_govpostcheck;

};
class crJXJScrollMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJScrollMethod();
	crJXJScrollMethod(const crJXJScrollMethod& handle);
	MethodClass(JXJ, JXJScroll)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crScrollBarWidgetNode *m_this;
	unsigned char m_type;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCountryShopTipsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJCountryShopTipsMethod:public CRCore::crMethod
{//Client 
public:
	crJXJCountryShopTipsMethod();
	crJXJCountryShopTipsMethod(const crJXJCountryShopTipsMethod& handle);
	MethodClass(JXJ,JXJCountryShopTips)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string           m_tipcanvas;
	std::string           m_tiptext;
	int                   m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJReloadVersionTabMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJReloadVersionTabMethod:public CRCore::crMethod
{//Client 
public:
	crJXJReloadVersionTabMethod();
	crJXJReloadVersionTabMethod(const crJXJReloadVersionTabMethod& handle);
	MethodClass(JXJ,JXJReloadVersionTab)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int                   m_id;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJAddTroopsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJAddTroopsMethod :public CRCore::crMethod
{//Server Method
public:
	crJXJAddTroopsMethod();
	crJXJAddTroopsMethod(const crJXJAddTroopsMethod& handle);
	MethodClass(JXJ, JXJAddTroops)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::crVector2i m_award;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvTroopsMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvTroopsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvTroopsMethod();
	crJXJRecvTroopsMethod(const crJXJRecvTroopsMethod& handle);
	MethodClass(JXJ, JXJRecvTroops)
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
//crJXJIsNewGuideTaskMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJIsNewGuideTaskMethod:public CRCore::crMethod
{//Client 判断是否是新手任务
public:
	crJXJIsNewGuideTaskMethod();
	crJXJIsNewGuideTaskMethod(const crJXJIsNewGuideTaskMethod& handle);
	MethodClass(JXJ,JXJIsNewGuideTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crCanvasNode> m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJSetAutoBuildingMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJSetAutoBuildingMethod:public CRCore::crMethod
{//Client 设置自动升级建筑
public:
	crJXJSetAutoBuildingMethod();
	crJXJSetAutoBuildingMethod(const crJXJSetAutoBuildingMethod& handle);
	MethodClass(JXJ,JXJSetAutoBuilding)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRUI::crCheckBoxWidgetNode> m_this;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvSetAutoBuildingMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvSetAutoBuildingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvSetAutoBuildingMethod();
	crJXJRecvSetAutoBuildingMethod(const crJXJRecvSetAutoBuildingMethod& handle);
	MethodClass(JXJ, JXJRecvSetAutoBuilding)
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
//crJXJAutoBuildingMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJAutoBuildingMethod:public CRCore::crMethod
{//Client 设置自动升级建筑
public:
	crJXJAutoBuildingMethod();
	crJXJAutoBuildingMethod(const crJXJAutoBuildingMethod& handle);
	MethodClass(JXJ,JXJAutoBuilding)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	typedef std::vector<int> BuildingIDArray;
	BuildingIDArray m_buildingids;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJAutoBuildingInitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJAutoBuildingInitMethod:public CRCore::crMethod
{//Client
public:
	crJXJAutoBuildingInitMethod();
	crJXJAutoBuildingInitMethod(const crJXJAutoBuildingInitMethod& handle);
	MethodClass(JXJ,JXJAutoBuildingInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crCanvasNode> m_this;
	std::string m_autoBuildWidget;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryBattleRankingMapMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJQueryBattleRankingMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryBattleRankingMapMethod();
	crJXJQueryBattleRankingMapMethod(const crJXJQueryBattleRankingMapMethod& handle);
	MethodClass(JXJ, JXJQueryBattleRankingMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvasName;//UI_GZPH
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvQueryBattleRankingMapMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvQueryBattleRankingMapMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryBattleRankingMapMethod();
	crJXJRecvQueryBattleRankingMapMethod(const crJXJRecvQueryBattleRankingMapMethod& handle);
	MethodClass(JXJ, JXJRecvQueryBattleRankingMap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
	std::string m_table;
	std::string m_jueSeCanvas;
	std::string m_guozhanDonghuaSW;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleRankingCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBattleRankingCanvasUpdateMethod:public CRCore::crMethod
{//Client
public:
	crJXJBattleRankingCanvasUpdateMethod();
	crJXJBattleRankingCanvasUpdateMethod(const crJXJBattleRankingCanvasUpdateMethod& handle);
	MethodClass(JXJ,JXJBattleRankingCanvasUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crCanvasNode> m_this;
	std::string m_table;
	std::string m_scroll;
	std::string m_paimingimg[5];
	std::string m_iconimg[5][3];
	std::string m_num[5][3];
	std::string m_myicon[3];
	std::string m_mynum[3];
	std::string m_killcount;
	std::string m_deadcount;
	std::string m_mypos;
	std::string m_myname;
	std::string m_myjifen;
	std::string m_mykill;
	std::string m_mydead;
	std::string m_myexploit;
	std::string m_lingqu;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGetBattleRankingRewardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGetBattleRankingRewardMethod:public CRCore::crMethod
{//Client
public:
	crJXJGetBattleRankingRewardMethod();
	crJXJGetBattleRankingRewardMethod(const crJXJGetBattleRankingRewardMethod& handle);
	MethodClass(JXJ,JXJGetBattleRankingReward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvGetBattleRankingRewardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvGetBattleRankingRewardMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvGetBattleRankingRewardMethod();
	crJXJRecvGetBattleRankingRewardMethod(const crJXJRecvGetBattleRankingRewardMethod& handle);
	MethodClass(JXJ, JXJRecvGetBattleRankingReward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_lingqubtn;
	std::string m_jueSeCanvas;
	std::string m_guozhanDonghuaSW;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJScrollValueInit
//
//////////////////////////////////////////////////////////////////////////
class crJXJScrollValueInitMethod :public CRCore::crMethod
{//Clint Method
public:
	crJXJScrollValueInitMethod();
	crJXJScrollValueInitMethod(const crJXJScrollValueInitMethod& handle);
	MethodClass(JXJ, JXJScrollValueInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRCore::crCanvasNode> m_this;
	std::string m_scroll;
};

class crJXJSaveBattleRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveBattleRankingMethod();
	crJXJSaveBattleRankingMethod(const crJXJSaveBattleRankingMethod & handle);
	MethodClass(JXJ, JXJSaveBattleRanking)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_file;
};
class crJXJLoadBattleRankingMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJLoadBattleRankingMethod();
	crJXJLoadBattleRankingMethod(const crJXJLoadBattleRankingMethod & handle);
	MethodClass(JXJ, JXJLoadBattleRanking)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	std::string m_serverName;
	std::string m_file;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRefreshForeignCardMsgBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRefreshForeignCardMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJRefreshForeignCardMsgBoxMethod();
	crJXJRefreshForeignCardMsgBoxMethod(const crJXJRefreshForeignCardMsgBoxMethod& handle);
	MethodClass(JXJ, JXJRefreshForeignCardMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_msgtext;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRefreshForeignCardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRefreshForeignCardMethod :public CRCore::crMethod
{//Client
public:
	crJXJRefreshForeignCardMethod();
	crJXJRefreshForeignCardMethod(const crJXJRefreshForeignCardMethod& handle);
	MethodClass(JXJ, JXJRefreshForeignCard)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvRefreshForeignCardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvRefreshForeignCardMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvRefreshForeignCardMethod();
	crJXJRecvRefreshForeignCardMethod(const crJXJRecvRefreshForeignCardMethod& handle);
	MethodClass(JXJ, JXJRecvRefreshForeignCard)
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
//crJXJExchangeForeignCardMsgBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJExchangeForeignCardMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJExchangeForeignCardMsgBoxMethod();
	crJXJExchangeForeignCardMsgBoxMethod(const crJXJExchangeForeignCardMsgBoxMethod& handle);
	MethodClass(JXJ, JXJExchangeForeignCardMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	unsigned char m_index;
	std::string m_msgbox;
	std::string m_msgtext;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJExchangeForeignCardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJExchangeForeignCardMethod :public CRCore::crMethod
{//Client
public:
	crJXJExchangeForeignCardMethod();
	crJXJExchangeForeignCardMethod(const crJXJExchangeForeignCardMethod& handle);
	MethodClass(JXJ, JXJExchangeForeignCard)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned char m_index;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvExchangeForeignCardMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvExchangeForeignCardMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvExchangeForeignCardMethod();
	crJXJRecvExchangeForeignCardMethod(const crJXJRecvExchangeForeignCardMethod& handle);
	MethodClass(JXJ, JXJRecvExchangeForeignCard)
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
//crJXJGoldShopCanvasUpdateMethod
//
/////////////////////////////////////////
class crJXJGoldShopCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGoldShopCanvasUpdateMethod();
	crJXJGoldShopCanvasUpdateMethod(const crJXJGoldShopCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJGoldShopCanvasUpdate)
		virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_scroll;
	std::string m_moneyText;
	std::string m_priceInput[12];
	std::string m_imageBox[12];
	std::string m_countInput[12];
	std::string m_goumaiBtn[12];
	std::string m_sw[12];
	std::string m_needLvInput[12];
	std::string m_strFoodImage;//粮草
	std::string m_strWoodImage;//木材
	std::string m_strIronImage;//铁矿
	std::string m_strHorseImage;//马匹
	std::string m_strCopperImage;//铜钱
	std::string m_strExpImage;//经验
	std::string m_strAchievementImage;//功勋
	std::string m_strGiftGoldImage;//礼金
	std::string m_strContributeImage;//城防值
	std::string m_strExploitImage;//战功
	std::string m_strVipImage;//vip经验
	std::string m_strZhanquanImage;//战券
};
/////////////////////////////////////////
//
//crJXJBlackGoldShopCanvasUpdateMethod
//
/////////////////////////////////////////
class crJXJBlackGoldShopCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJBlackGoldShopCanvasUpdateMethod();
	crJXJBlackGoldShopCanvasUpdateMethod(const crJXJBlackGoldShopCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJBlackGoldShopCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_moneyText;
	std::string m_priceInput[5];
	std::string m_imageBox[5];
	std::string m_countInput[5];
	std::string m_goumaiBtn[5];
	std::string m_temai[5];
	std::string m_refreshBtn;
	std::string m_refreshTimer;
	std::string m_strFoodImage;//粮草
	std::string m_strWoodImage;//木材
	std::string m_strIronImage;//铁矿
	std::string m_strHorseImage;//马匹
	std::string m_strCopperImage;//铜钱
	std::string m_strExpImage;//经验
	std::string m_strAchievementImage;//功勋
	std::string m_strGiftGoldImage;//礼金
	std::string m_strContributeImage;//城防值
	std::string m_strExploitImage;//战功
	std::string m_strVipImage;//vip经验
	std::string m_strZhanquanImage;//战券
};
class crJXJBlackShopRefreshTimerMethod : public CRCore::crMethod
{//GameServer Method
public:
	crJXJBlackShopRefreshTimerMethod();
	crJXJBlackShopRefreshTimerMethod(const crJXJBlackShopRefreshTimerMethod& handle);
	MethodClass(JXJ, JXJBlackShopRefreshTimer)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData* m_this;
	int m_dt;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBlackShopRefreshMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJRecvBlackShopRefreshMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvBlackShopRefreshMethod();
	crJXJRecvBlackShopRefreshMethod(const crJXJRecvBlackShopRefreshMethod& handle);
	MethodClass(JXJ, JXJRecvBlackShopRefresh)
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
//crJXJBlackShopRefreshMsgBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopRefreshMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJBlackShopRefreshMsgBoxMethod();
	crJXJBlackShopRefreshMsgBoxMethod(const crJXJBlackShopRefreshMsgBoxMethod& handle);
	MethodClass(JXJ, JXJBlackShopRefreshMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_msgtext;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBlackShopRefreshMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopRefreshMethod :public CRCore::crMethod
{//Client
public:
	crJXJBlackShopRefreshMethod();
	crJXJBlackShopRefreshMethod(const crJXJBlackShopRefreshMethod& handle);
	MethodClass(JXJ, JXJBlackShopRefresh)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGlodShopBuyMsgBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJGlodShopBuyMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJGlodShopBuyMsgBoxMethod();
	crJXJGlodShopBuyMsgBoxMethod(const crJXJGlodShopBuyMsgBoxMethod& handle);
	MethodClass(JXJ, JXJGlodShopBuyMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_msgtext;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJGlodShopBuyMethod
//元宝商店购买物品
//
//////////////////////////////////////////////////////////////////////////
class crJXJGlodShopBuyMethod :public CRCore::crMethod
{//Client 
public:
	crJXJGlodShopBuyMethod();
	crJXJGlodShopBuyMethod(const crJXJGlodShopBuyMethod& handle);
	MethodClass(JXJ, JXJGlodShopBuy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////
//
//crJXJRecvGlodShopBuyMethod
//
//////////////////////////////////////////
class crJXJRecvGlodShopBuyMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvGlodShopBuyMethod();
	crJXJRecvGlodShopBuyMethod(const crJXJRecvGlodShopBuyMethod &handle);
	MethodClass(JXJ, JXJRecvGlodShopBuy)
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
//////////////////////////////////////////////////////////////////////////
//
//crJXJBlackShopBuyMsgBoxMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopBuyMsgBoxMethod :public CRCore::crMethod
{//Client
public:
	crJXJBlackShopBuyMsgBoxMethod();
	crJXJBlackShopBuyMsgBoxMethod(const crJXJBlackShopBuyMsgBoxMethod& handle);
	MethodClass(JXJ, JXJBlackShopBuyMsgBox)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_msgbox;
	std::string m_msgtext;
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBlackShopBuyMethod
//元宝商店购买物品
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopBuyMethod :public CRCore::crMethod
{//Client 
public:
	crJXJBlackShopBuyMethod();
	crJXJBlackShopBuyMethod(const crJXJBlackShopBuyMethod& handle);
	MethodClass(JXJ, JXJBlackShopBuy)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
//////////////////////////////////////////
//
//crJXJRecvBlackShopBuyMethod
//
//////////////////////////////////////////
class crJXJRecvBlackShopBuyMethod :public CRCore::crMethod
{//Clint & GameServer Method
public:
	crJXJRecvBlackShopBuyMethod();
	crJXJRecvBlackShopBuyMethod(const crJXJRecvBlackShopBuyMethod &handle);
	MethodClass(JXJ, JXJRecvBlackShopBuy)
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
//////////////////////////////////////////////////////////////////////////
//
//crJXJBlackShopRefreshTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopRefreshTimerUpdateMethod :public CRCore::crMethod
{//Client 
public:
	crJXJBlackShopRefreshTimerUpdateMethod();
	crJXJBlackShopRefreshTimerUpdateMethod(const crJXJBlackShopRefreshTimerUpdateMethod& handle);
	MethodClass(JXJ, JXJBlackShopRefreshTimerUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBlackShopRefreshTimerDisplayMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJBlackShopRefreshTimerDisplayMethod :public CRCore::crMethod
{//Client 
public:
	crJXJBlackShopRefreshTimerDisplayMethod();
	crJXJBlackShopRefreshTimerDisplayMethod(const crJXJBlackShopRefreshTimerDisplayMethod& handle);
	MethodClass(JXJ, JXJBlackShopRefreshTimerDisplay)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crStaticTextWidgetNode *m_this;
};
//////////////////////////////////////////////////////////////////////////
//crJXJRecvBlackShopRefreshTimerMethod
//////////////////////////////////////////////////////////////////////////
class crJXJRecvBlackShopRefreshTimerMethod :public CRCore::crMethod
{//Client  Method
public:
	crJXJRecvBlackShopRefreshTimerMethod();
	crJXJRecvBlackShopRefreshTimerMethod(const crJXJRecvBlackShopRefreshTimerMethod & handle);
	MethodClass(JXJ, JXJRecvBlackShopRefreshTimer)
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
