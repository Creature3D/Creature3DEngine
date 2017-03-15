/**********************************************************************
*
*	文件:	   appMethod4.h
*
*	描述:	   
*
*	作者:	   杨勇
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD4_H
#define JXJ_APPMETHOD4_H
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

namespace JXJ
{
class crJXJUIRecruitTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIRecruitTipMethod();
	crJXJUIRecruitTipMethod(const crJXJUIRecruitTipMethod& handle);
	MethodClass(JXJ, JXJUIRecruitTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_armInput;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};
class crJXJUIBuildingLevelUpTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBuildingLevelUpTipMethod();
	crJXJUIBuildingLevelUpTipMethod(const crJXJUIBuildingLevelUpTipMethod& handle);
	MethodClass(JXJ, JXJUIBuildingLevelUpTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

class crJXJUIGongFangBDDZTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangBDDZTipMethod();
	crJXJUIGongFangBDDZTipMethod(const crJXJUIGongFangBDDZTipMethod& handle);
	MethodClass(JXJ,JXJUIGongFangBDDZTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_inputCount;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
};

class crJXJUIGongFangLZDZTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangLZDZTipMethod();
	crJXJUIGongFangLZDZTipMethod(const crJXJUIGongFangLZDZTipMethod& handle);
	MethodClass(JXJ,JXJUIGongFangLZDZTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tipsCanvas;
	std::string m_tipsTextWidget;
	int m_lordAbstractID;
};

class crJXJUIGongfangEquipPageResetMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongfangEquipPageResetMethod();
	crJXJUIGongfangEquipPageResetMethod(const crJXJUIGongfangEquipPageResetMethod& handle);
	MethodClass(JXJ,JXJUIGongfangEquipPageReset)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRUI::crRadioGroupWidgetNode* m_this;
	std::string m_pageBudui;
	std::string m_pageLingzhu;
};

class crJXJUISystemIconTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUISystemIconTipMethod();
	crJXJUISystemIconTipMethod(const crJXJUISystemIconTipMethod& handle);
	MethodClass(JXJ,JXJUISystemIconTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tiptext;
	int m_str;
	int m_timeid;
	int m_cdhintid;
};

//class crJXJUITavernCardTicketsTipMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUITavernCardTicketsTipMethod();
//	crJXJUITavernCardTicketsTipMethod(const crJXJUITavernCardTicketsTipMethod& handle);
//	MethodClass(JXJ,JXJUITavernCardTicketsTip)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_tips;
//	int m_type;
//	std::string m_raido;
//	std::string m_jinengmiaoshu;
//	std::string m_card;
//	std::string m_nameimage;
//	std::string m_backboard;
//};

class crJXJUIPostRankingChangePageMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIPostRankingChangePageMethod();
	crJXJUIPostRankingChangePageMethod(const crJXJUIPostRankingChangePageMethod& handle);
	MethodClass(JXJ,JXJUIPostRankingChangePage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_step;
};


class crJXJUIPostRankingPageShowMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIPostRankingPageShowMethod();
	crJXJUIPostRankingPageShowMethod(const crJXJUIPostRankingPageShowMethod& handle);
	MethodClass(JXJ,JXJUIPostRankingPageShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_pageNum;
	std::string m_rankingList;
	std::string m_rankingbtn;
	std::string m_tips;
	std::string m_tiptext;
	std::string m_officeShopbtn;
	std::string m_officeListbtn;
	std::string m_mygroupcanvas;
};

class crJXJUIMyExplotRankingMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIMyExplotRankingMethod();
	crJXJUIMyExplotRankingMethod(const crJXJUIMyExplotRankingMethod& handle);
	MethodClass(JXJ,JXJUIMyExplotRanking)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_pageNum;
	std::string m_tips;
	std::string m_tiptext;
};

//class crJXJUIFuYaKeJiTipMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIFuYaKeJiTipMethod();
//	crJXJUIFuYaKeJiTipMethod(const crJXJUIFuYaKeJiTipMethod& handle);
//	MethodClass(JXJ,JXJUIFuYaKeJiTip)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_tips;
//	int m_type;
//	std::string m_attributename;
//	std::string m_attributeinfo;
//	std::string m_lordlv;
//	std::string m_achievementcount;
//};

class crJXJTestFontSizeMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJTestFontSizeMethod();
	crJXJTestFontSizeMethod(const crJXJTestFontSizeMethod& handle);
	MethodClass(JXJ, JXJTestFontSize)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	float m_charactersize;
	float m_sizex,m_sizey;
	float m_linespacing;
	float m_underlinespacing;
	float m_uplinespacing;
	std::string m_scrollbar;
	std::string m_table;
	std::string m_equipList[9];
};

class crJXJUINoOpenIconTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUINoOpenIconTipMethod();
	crJXJUINoOpenIconTipMethod(const crJXJUINoOpenIconTipMethod& handle);
	MethodClass(JXJ,JXJUINoOpenIconTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tiptext;
	int m_str;
};

class crJXJUIUpgradePromptMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIUpgradePromptMethod();
	crJXJUIUpgradePromptMethod(const crJXJUIUpgradePromptMethod& handle);
	MethodClass(JXJ,JXJUIUpgradePrompt)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_LevelType;
	std::string m_LevelType_One;
	std::string m_LevelType_Two;
};

class crJXJCheckRecruitCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckRecruitCountMethod();
	crJXJCheckRecruitCountMethod(const crJXJCheckRecruitCountMethod& handle);
	MethodClass(JXJ, JXJCheckRecruitCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_armid;
	unsigned char m_count;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckTempFormationInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTempFormationInfoMethod();
	crJXJCheckTempFormationInfoMethod(const crJXJCheckTempFormationInfoMethod& handle);
	MethodClass(JXJ, JXJCheckTempFormationInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	bool m_flag;
	int m_armid;
	unsigned char m_count;
	std::string m_formationCanvas;
	std::string m_formationInput;
	unsigned char m_newCount;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckFormationInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationInfoMethod();
	crJXJCheckFormationInfoMethod(const crJXJCheckFormationInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFormationInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	bool m_flag;
	int m_armid;
	unsigned char m_count;
	unsigned char m_newCount;
	char m_trueExecCount;
};

class crJXJGetTaskProgressMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJGetTaskProgressMethod();
	crJXJGetTaskProgressMethod(const crJXJGetTaskProgressMethod& handle);
	MethodClass(JXJ, JXJGetTaskProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
};

class crJXJCheckBuildingLvTwoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckBuildingLvTwoMethod();
	crJXJCheckBuildingLvTwoMethod(const crJXJCheckBuildingLvTwoMethod& handle);
	MethodClass(JXJ, JXJCheckBuildingLvTwo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_buildingid;
	unsigned char m_lv;
};

class crJXJCheckZhengShuiCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckZhengShuiCountMethod();
	crJXJCheckZhengShuiCountMethod(const crJXJCheckZhengShuiCountMethod& handle);
	MethodClass(JXJ, JXJCheckZhengShuiCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_zhengShuiCanvas;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckFormationJKInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationJKInfoMethod();
	crJXJCheckFormationJKInfoMethod(const crJXJCheckFormationJKInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFormationJKInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	char m_trueExecCount;//-1表示无穷
	int m_index;
};

class crJXJCheckWeiRenJKInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckWeiRenJKInfoMethod();
	crJXJCheckWeiRenJKInfoMethod(const crJXJCheckWeiRenJKInfoMethod& handle);
	MethodClass(JXJ, JXJCheckWeiRenJKInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
};

class crJXJUIGetRenWuRewardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUIGetRenWuRewardMethod();
	crJXJUIGetRenWuRewardMethod(const crJXJUIGetRenWuRewardMethod& handle);
	MethodClass(JXJ, JXJUIGetRenWuReward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	std::string m_canvas;
	std::string m_expCount;
	std::string m_copCashCount;
	std::string m_oneCount;
	std::string m_twoCount;
	std::string m_rewardstr[2];
	int m_taskid;
};

class crJXJUIPlayerFriendListSelectMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPlayerFriendListSelectMethod();
	crJXJUIPlayerFriendListSelectMethod(const crJXJUIPlayerFriendListSelectMethod& handle);
	MethodClass(JXJ, JXJUIPlayerFriendListSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_friendlist;
	int m_iconid;
};

class crJXJUIXinShouYingDaoMoveMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIXinShouYingDaoMoveMethod();
	crJXJUIXinShouYingDaoMoveMethod(const crJXJUIXinShouYingDaoMoveMethod& handle);
	MethodClass(JXJ, JXJUIXinShouYingDaoMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_moveArrows;
};

class crJXJAdvanceTask2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAdvanceTask2Method();
	crJXJAdvanceTask2Method(const crJXJAdvanceTask2Method& handle);
	MethodClass(JXJ, JXJAdvanceTask2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_taskid;
	unsigned char m_stepType;
};

//class crJXJBuildingUpgradePromptEffectLogic : public CRCore::crLogic
//{//Client Method
//public:
//	crJXJBuildingUpgradePromptEffectLogic();
//	crJXJBuildingUpgradePromptEffectLogic(const crJXJBuildingUpgradePromptEffectLogic& handle);
//	LogicClass(JXJ, JXJBuildingUpgradePromptEffect)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void inputHandle(int i, void *param);
//	virtual void outputParam(int i, void *param);
//protected:
//	CRCore::ref_ptr<CRCore::crHandle> m_playSceneFx;
//	CRCore::ref_ptr<CRCore::crNode> m_decalNode;
//	float m_zoffset;
//	std::string m_filename;
//	int m_buildingid;
//};

//class crJXJBuildingEffectCloseMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJBuildingEffectCloseMethod();
//	crJXJBuildingEffectCloseMethod(const crJXJBuildingEffectCloseMethod& handle);
//	MethodClass(JXJ, JXJBuildingEffectClose)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//};
class crJXJBuildingTaskEffectMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJBuildingTaskEffectMethod();
	crJXJBuildingTaskEffectMethod(const crJXJBuildingTaskEffectMethod& handle);
	MethodClass(JXJ, JXJBuildingTaskEffect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_buildingid;
	bool m_visiable;
};
class crJXJCheckPutOnEquipInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckPutOnEquipInfoMethod();
	crJXJCheckPutOnEquipInfoMethod(const crJXJCheckPutOnEquipInfoMethod& handle);
	MethodClass(JXJ, JXJCheckPutOnEquipInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_LordCanvas;
	char m_trueExecCount;
};

class crJXJArrowsUITypeSelectMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJArrowsUITypeSelectMethod();
	crJXJArrowsUITypeSelectMethod(const crJXJArrowsUITypeSelectMethod& handle);
	MethodClass(JXJ, JXJArrowsUITypeSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_RenWuCanvas;
	int m_Arrows;
};

class crJXJCheckFormationCeKaInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationCeKaInfoMethod();
	crJXJCheckFormationCeKaInfoMethod(const crJXJCheckFormationCeKaInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFormationCeKaInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	char m_trueExecCount;
};

class crJXJCheckRecruitTimesMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckRecruitTimesMethod();
	crJXJCheckRecruitTimesMethod(const crJXJCheckRecruitTimesMethod& handle);
	MethodClass(JXJ, JXJCheckRecruitTimes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
};

class crJXJCheckMasterFormationInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckMasterFormationInfoMethod();
	crJXJCheckMasterFormationInfoMethod(const crJXJCheckMasterFormationInfoMethod& handle);
	MethodClass(JXJ, JXJCheckMasterFormationInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_armid;
	unsigned char m_count;
	int m_newArmid;
	unsigned char m_newCount;
	char m_trueExecCount;
};

class crJXJCheckCurrentRecruitCountInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentRecruitCountInfoMethod();
	crJXJCheckCurrentRecruitCountInfoMethod(const crJXJCheckCurrentRecruitCountInfoMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentRecruitCountInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
};


class crJXJCheckSupplementRecruitCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckSupplementRecruitCountMethod();
	crJXJCheckSupplementRecruitCountMethod(const crJXJCheckSupplementRecruitCountMethod& handle);
	MethodClass(JXJ, JXJCheckSupplementRecruitCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
};

class crJXJCheckCurrentCardCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentCardCountMethod();
	crJXJCheckCurrentCardCountMethod(const crJXJCheckCurrentCardCountMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentCardCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_renWuCanvas;
};

class crJXJCheckExtractJiangKaInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckExtractJiangKaInfoMethod();
	crJXJCheckExtractJiangKaInfoMethod(const crJXJCheckExtractJiangKaInfoMethod& handle);
	MethodClass(JXJ, JXJCheckExtractJiangKaInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_renWuCanvas;
	char m_trueExecCount;
};

class crJXJCheckDirectlySubmitTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckDirectlySubmitTaskMethod();
	crJXJCheckDirectlySubmitTaskMethod(const crJXJCheckDirectlySubmitTaskMethod& handle);
	MethodClass(JXJ, JXJCheckDirectlySubmitTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_taskid;
};

class crJXJCheckOpenCanvasInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckOpenCanvasInfoMethod();
	crJXJCheckOpenCanvasInfoMethod(const crJXJCheckOpenCanvasInfoMethod& handle);
	MethodClass(JXJ, JXJCheckOpenCanvasInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_targetCanvas;
	int m_buildingID;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckMasterFormationMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckMasterFormationMethod();
	crJXJCheckMasterFormationMethod(const crJXJCheckMasterFormationMethod& handle);
	MethodClass(JXJ, JXJCheckMasterFormation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationCanvas;
	char m_trueExecCount;
};

class crJXJCheckPutOnJiangKaTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckPutOnJiangKaTaskMethod();
	crJXJCheckPutOnJiangKaTaskMethod(const crJXJCheckPutOnJiangKaTaskMethod& handle);
	MethodClass(JXJ, JXJCheckPutOnJiangKaTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationCanvas;
};

class crJXJCheckTaskFuBenSuccessInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTaskFuBenSuccessInfoMethod();
	crJXJCheckTaskFuBenSuccessInfoMethod(const crJXJCheckTaskFuBenSuccessInfoMethod& handle);
	MethodClass(JXJ, JXJCheckTaskFuBenSuccessInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned short m_fubenid;
	char m_trueExecCount;
};

class crJXJCheckCloseCanvasInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCloseCanvasInfoMethod();
	crJXJCheckCloseCanvasInfoMethod(const crJXJCheckCloseCanvasInfoMethod& handle);
	MethodClass(JXJ, JXJCheckCloseCanvasInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_targetCanvas;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckCloseCanvasInfo2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCloseCanvasInfo2Method();
	crJXJCheckCloseCanvasInfo2Method(const crJXJCheckCloseCanvasInfo2Method& handle);
	MethodClass(JXJ, JXJCheckCloseCanvasInfo2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_targetCanvas;
};

class crJXJCheckOpenCanvasInfo2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckOpenCanvasInfo2Method();
	crJXJCheckOpenCanvasInfo2Method(const crJXJCheckOpenCanvasInfo2Method& handle);
	MethodClass(JXJ, JXJCheckOpenCanvasInfo2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_targetCanvas;
	int m_buildingID;
};

class crJXJCheckMasterFormation2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckMasterFormation2Method();
	crJXJCheckMasterFormation2Method(const crJXJCheckMasterFormation2Method& handle);
	MethodClass(JXJ, JXJCheckMasterFormation2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationCanvas;
	char m_trueExecCount;
	int m_index;
};

class crJXJCheckAlreadyInBattleInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckAlreadyInBattleInfoMethod();
	crJXJCheckAlreadyInBattleInfoMethod(const crJXJCheckAlreadyInBattleInfoMethod& handle);
	MethodClass(JXJ, JXJCheckAlreadyInBattleInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	char m_trueExecCount;
	int m_selectCount;
};

class crJXJCheckCurrentResourcesCardCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentResourcesCardCountMethod();
	crJXJCheckCurrentResourcesCardCountMethod(const crJXJCheckCurrentResourcesCardCountMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentResourcesCardCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_KnapsackCanvas;
	std::string m_myBagList;
	std::string m_rectCanvasName;
	char m_trueExecCount;
	int m_taskid;
	int m_itemid;
};

class crJXJCheckResourcesCardCountInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckResourcesCardCountInfoMethod();
	crJXJCheckResourcesCardCountInfoMethod(const crJXJCheckResourcesCardCountInfoMethod& handle);
	MethodClass(JXJ, JXJCheckResourcesCardCountInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
	int m_itemid;
	int m_count;
};

class crJXJCheckUpdateTechLevelTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckUpdateTechLevelTaskMethod();
	crJXJCheckUpdateTechLevelTaskMethod(const crJXJCheckUpdateTechLevelTaskMethod& handle);
	MethodClass(JXJ, JXJCheckUpdateTechLevelTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;//-1表示无穷
	int m_techid;
};

class crJXJCheckUpdateGovernmentPostTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckUpdateGovernmentPostTaskMethod();
	crJXJCheckUpdateGovernmentPostTaskMethod(const crJXJCheckUpdateGovernmentPostTaskMethod& handle);
	MethodClass(JXJ, JXJCheckUpdateGovernmentPostTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckUpdateCardConditionInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckUpdateCardConditionInfoMethod();
	crJXJCheckUpdateCardConditionInfoMethod(const crJXJCheckUpdateCardConditionInfoMethod& handle);
	MethodClass(JXJ, JXJCheckUpdateCardConditionInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
	int m_cardid;
};

class crJXJCheckFormationCardTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationCardTaskMethod();
	crJXJCheckFormationCardTaskMethod(const crJXJCheckFormationCardTaskMethod& handle);
	MethodClass(JXJ, JXJCheckFormationCardTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_cardid;
	char m_trueExecCount;
};

class crJXJCheckCurrentTargetCardCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentTargetCardCountMethod();
	crJXJCheckCurrentTargetCardCountMethod(const crJXJCheckCurrentTargetCardCountMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentTargetCardCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_cardid;
	std::string m_renWuCanvas;
};

class crJXJCheckTargetCardCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTargetCardCountMethod();
	crJXJCheckTargetCardCountMethod(const crJXJCheckTargetCardCountMethod& handle);
	MethodClass(JXJ, JXJCheckTargetCardCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_cardid;
	char m_trueExecCount;
	std::string m_renWuCanvas;
};

class crJXJCheckCurSelRecruitArmAIDMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurSelRecruitArmAIDMethod();
	crJXJCheckCurSelRecruitArmAIDMethod(const crJXJCheckCurSelRecruitArmAIDMethod& handle);
	MethodClass(JXJ, JXJCheckCurSelRecruitArmAID)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
	int m_recruitArmID;
};

class crJXJCheckForgeEquipConditionInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckForgeEquipConditionInfoMethod();
	crJXJCheckForgeEquipConditionInfoMethod(const crJXJCheckForgeEquipConditionInfoMethod& handle);
	MethodClass(JXJ, JXJCheckForgeEquipConditionInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	//CRCore::crCanvasNode *m_this;
	std::string m_gongfang;
	char m_trueExecCount;
	int m_Equipid;
};

class crJXJCheckCurrentItemKindCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentItemKindCountMethod();
	crJXJCheckCurrentItemKindCountMethod(const crJXJCheckCurrentItemKindCountMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentItemKindCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_itemid;
};

class crJXJCheckCurrentSelectRadioMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentSelectRadioMethod();
	crJXJCheckCurrentSelectRadioMethod(const crJXJCheckCurrentSelectRadioMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentSelectRadio)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_GongFangCanvas;
	std::string m_GongFangRaido;
	int m_select;
	char m_trueExecCount;
};

class crJXJCheckCurrentItemsCountInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentItemsCountInfoMethod();
	crJXJCheckCurrentItemsCountInfoMethod(const crJXJCheckCurrentItemsCountInfoMethod& handle);
	MethodClass(JXJ, JXJCheckCurrentItemsCountInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;
	int m_itemid;
	int m_count;
};

class crJXJCheckFormationRaidoSelectMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationRaidoSelectMethod();
	crJXJCheckFormationRaidoSelectMethod(const crJXJCheckFormationRaidoSelectMethod& handle);
	MethodClass(JXJ, JXJCheckFormationRaidoSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationCanvas;
	std::string m_formationRaido;
	int m_count;
	char m_trueExecCount;
};

class crJXJCheckTaskTempFormationInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTaskTempFormationInfoMethod();
	crJXJCheckTaskTempFormationInfoMethod(const crJXJCheckTaskTempFormationInfoMethod& handle);
	MethodClass(JXJ, JXJCheckTaskTempFormationInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_formationCanvas;
	int m_armid;
	char m_trueExecCount;
};

class crJXJCheckCurrentSelectRadio2Method : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckCurrentSelectRadio2Method();
	crJXJCheckCurrentSelectRadio2Method(const crJXJCheckCurrentSelectRadio2Method& handle);
	MethodClass(JXJ, JXJCheckCurrentSelectRadio2)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_GongFangCanvas;
	std::string m_GongFangRaido;
	int m_select;
};

class crJXJUILordPropertyTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUILordPropertyTipsMethod();
	crJXJUILordPropertyTipsMethod(const crJXJUILordPropertyTipsMethod& handle);
	MethodClass(JXJ,JXJUILordPropertyTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_propertyName;
	std::string m_propertySW;
	std::string m_textInputOne;
	std::string m_textInputTwo;
	std::string m_textInputThree;
	int m_str;
	int m_index;
};

class crJXJCheckGameMainLineTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGameMainLineTaskMethod();
	crJXJCheckGameMainLineTaskMethod(const crJXJCheckGameMainLineTaskMethod& handle);
	MethodClass(JXJ, JXJCheckGameMainLineTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJAutoMainLineTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoMainLineTaskMethod();
	crJXJAutoMainLineTaskMethod(const crJXJAutoMainLineTaskMethod& handle);
	MethodClass(JXJ, JXJAutoMainLineTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
};

class crJXJAutoCompensationTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoCompensationTaskMethod();
	crJXJAutoCompensationTaskMethod(const crJXJAutoCompensationTaskMethod& handle);
	MethodClass(JXJ, JXJAutoCompensationTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
};

class crJXJZhengZhanTianXiaTaskRaidoSelectMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJZhengZhanTianXiaTaskRaidoSelectMethod();
	crJXJZhengZhanTianXiaTaskRaidoSelectMethod(const crJXJZhengZhanTianXiaTaskRaidoSelectMethod& handle);
	MethodClass(JXJ, JXJZhengZhanTianXiaTaskRaidoSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	std::string m_fubenCanvas;
	std::string m_zhengzhanfubenRaido;
	std::string m_chapterRaido;
	std::string m_fubenRaido;
	std::string m_chapterscroll;
	int m_taskid;
};

class crJXJNewHandFuBenInitMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewHandFuBenInitMethod();
	crJXJNewHandFuBenInitMethod(const crJXJNewHandFuBenInitMethod& handle);
	MethodClass(JXJ, JXJNewHandFuBenInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_rectCanvas;
	std::string m_targetCanvasName;
	std::string m_targetWidgetName;
};

class crJXJUIManorStairJueSeTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIManorStairJueSeTipMethod();
	crJXJUIManorStairJueSeTipMethod(const crJXJUIManorStairJueSeTipMethod& handle);
	MethodClass(JXJ,JXJUIManorStairJueSeTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_tips;
	std::string m_tipstext;
	std::string m_jueSeCanvas;
	int m_str;
};

class crJXJUIFuKuProResourceCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUIFuKuProResourceCountMethod();
	crJXJUIFuKuProResourceCountMethod(const crJXJUIFuKuProResourceCountMethod& handle);
	MethodClass(JXJ, JXJUIFuKuProResourceCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jueSeCanvas;
};

class crJXJCheckSelFuBenCompleteInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckSelFuBenCompleteInfoMethod();
	crJXJCheckSelFuBenCompleteInfoMethod(const crJXJCheckSelFuBenCompleteInfoMethod& handle);
	MethodClass(JXJ, JXJCheckSelFuBenCompleteInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJSelFuBenCompleteTipsMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJSelFuBenCompleteTipsMethod();
	crJXJSelFuBenCompleteTipsMethod(const crJXJSelFuBenCompleteTipsMethod& handle);
	MethodClass(JXJ, JXJSelFuBenCompleteTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_fubenTipCanvas;
	std::string m_tipInput;
};

class crJXJCardShopOnShowMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardShopOnShowMethod();
	crJXJCardShopOnShowMethod(const crJXJCardShopOnShowMethod& handle);
	MethodClass(JXJ, JXJCardShopOnShow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
};
class crJXJCardShopUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCardShopUpdateMethod();
	crJXJCardShopUpdateMethod(const crJXJCardShopUpdateMethod& handle);
	MethodClass(JXJ, JXJCardShopUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_typeradio;   //将卡类型
	std::string m_colorradio;    //将卡颜色
	std::string m_cardpiecesinput;   //将魂数量
	std::string m_pagenuminput;   //页码
	std::string m_priceinput[12];	//出售价格	
	std::string m_purchasebtn[12]; //兑换按钮
	std::string m_cardicon[12];	//将卡icon		
	std::string m_colorswitch[12];
	std::string m_foreignCardSwitch;
	std::string m_foreignCardImg[3];
	std::string m_foreignCardInfo[3];
	std::string m_foreignCardPrice[3];
	std::string m_foreignCardRadio;
};
class crJXJUIPurChaseCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIPurChaseCardMethod();
	crJXJUIPurChaseCardMethod(const crJXJUIPurChaseCardMethod& handle);
	MethodClass(JXJ, JXJUIPurChaseCard)
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
class crJXJPurChaseCardMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJPurChaseCardMethod();
	crJXJPurChaseCardMethod(const crJXJPurChaseCardMethod& handle);
	MethodClass(JXJ, JXJPurChaseCard)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};
class crJXJRecvPurChaseCardMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvPurChaseCardMethod();
	crJXJRecvPurChaseCardMethod(const crJXJRecvPurChaseCardMethod& handle);
	MethodClass(JXJ, JXJRecvPurChaseCard)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJFuBenResetInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFuBenResetInfoMethod();
	crJXJFuBenResetInfoMethod(const crJXJFuBenResetInfoMethod& handle);
	MethodClass(JXJ, JXJFuBenResetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJUIFuBenResetInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIFuBenResetInfoMethod();
	crJXJUIFuBenResetInfoMethod(const crJXJUIFuBenResetInfoMethod& handle);
	MethodClass(JXJ, JXJUIFuBenResetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_promptCanvas;
	std::string m_input;
	std::string m_okbtn;
	CRCore::crVector4f m_textColor;
	int m_index;
};

class crJXJRecvFuBenResetInfoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvFuBenResetInfoMethod();
	crJXJRecvFuBenResetInfoMethod(const crJXJRecvFuBenResetInfoMethod& handle);
	MethodClass(JXJ, JXJRecvFuBenResetInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJUIGongFangTipsMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangTipsMethod();
	crJXJUIGongFangTipsMethod(const crJXJUIGongFangTipsMethod& handle);
	MethodClass(JXJ, JXJUIGongFangTips)
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
	std::string m_gongFangTypeRadio;
	std::string m_tuZhiLevelRadio;
};

class crJXJUIForgeLordEquipTipsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIForgeLordEquipTipsMethod();
	crJXJUIForgeLordEquipTipsMethod(const crJXJUIForgeLordEquipTipsMethod& handle);
	MethodClass(JXJ, JXJUIForgeLordEquipTips)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRProducer::crGUIEventAdapter* m_ea;
	CRCore::crWidgetNode* m_this;
	std::string m_tips;
	std::string m_name;
	std::string m_info1;
	std::string m_info2;
	std::string m_itemlv;
	std::string m_itemicon;
	std::string m_itemequipon;
	std::string m_canvas;
	std::string m_infocanvas;
	std::string m_infobk;
	std::string m_infodi;
	CRCore::crVector3 m_offsets;
	CRCore::crVector4f m_colorred;
	int m_imageindex;
};

class crJXJCheckTaskResourceCountMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTaskResourceCountMethod();
	crJXJCheckTaskResourceCountMethod(const crJXJCheckTaskResourceCountMethod& handle);
	MethodClass(JXJ, JXJCheckTaskResourceCount)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jueSeCanvas;
	int m_resourceCount;
	int m_index;
	char m_trueExecCount;//-1表示无穷
};

class crJXJUIGongFangRefineEquipUpdateMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangRefineEquipUpdateMethod();
	crJXJUIGongFangRefineEquipUpdateMethod(const crJXJUIGongFangRefineEquipUpdateMethod& handle);
	MethodClass(JXJ, JXJUIGongFangRefineEquipUpdate)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	//////////////////////////////////////////////////////////////////////////
	std::string m_equipRaido;
	std::string m_equip[5];
	std::string m_defaultname[5];
	std::string m_equipListctrl;
	std::string m_equipList_Left;
	std::string m_equipList_Right;
	std::string m_equipPageInput;
	std::string m_pagesw;
	std::string m_gongfangradio;
	std::string m_defaulticon;
	std::string m_lordequipradio;
	//std::string m_switchRadio[5];
};

class crJXJCheckJoinLegionTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckJoinLegionTaskMethod();
	crJXJCheckJoinLegionTaskMethod(const crJXJCheckJoinLegionTaskMethod& handle);
	MethodClass(JXJ, JXJCheckJoinLegionTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJTerritoryMouseOnRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJTerritoryMouseOnRoleMethod();
	crJXJTerritoryMouseOnRoleMethod(const crJXJTerritoryMouseOnRoleMethod& handle);
	MethodClass(JXJ, JXJTerritoryMouseOnRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem *m_this;
	_crInt64 m_param;
	CRProducer::crGUIEventAdapter* m_ea;
};

class crJXJCheckRefineEquipTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckRefineEquipTaskMethod();
	crJXJCheckRefineEquipTaskMethod(const crJXJCheckRefineEquipTaskMethod& handle);
	MethodClass(JXJ, JXJCheckRefineEquipTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gongFangCanvas;
	char m_trueExecCount;
};

class crJXJCheckFuLianEquipTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFuLianEquipTaskMethod();
	crJXJCheckFuLianEquipTaskMethod(const crJXJCheckFuLianEquipTaskMethod& handle);
	MethodClass(JXJ, JXJCheckFuLianEquipTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gongFangCanvas;
	char m_trueExecCount;
};

class crJXJGiveBuildingTime1Method :public CRCore::crMethod
{//Client Method
public:
	crJXJGiveBuildingTime1Method();
	crJXJGiveBuildingTime1Method(const crJXJGiveBuildingTime1Method& handle);
	MethodClass(JXJ, JXJGiveBuildingTime1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJRecvGiveBuildingTime1Method :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvGiveBuildingTime1Method();
	crJXJRecvGiveBuildingTime1Method(const crJXJRecvGiveBuildingTime1Method& handle);
	MethodClass(JXJ, JXJRecvGiveBuildingTime1)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJNewHandBattleTroopSelectCanvasMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJNewHandBattleTroopSelectCanvasMethod();
	crJXJNewHandBattleTroopSelectCanvasMethod(const crJXJNewHandBattleTroopSelectCanvasMethod& handle);
	MethodClass(JXJ, JXJNewHandBattleTroopSelectCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_selectTroopCanvas;
	int m_instanceitemid;
};

class crJXJCheckNewHandBattleTroopSelectCanvasMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckNewHandBattleTroopSelectCanvasMethod();
	crJXJCheckNewHandBattleTroopSelectCanvasMethod(const crJXJCheckNewHandBattleTroopSelectCanvasMethod& handle);
	MethodClass(JXJ, JXJCheckNewHandBattleTroopSelectCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_selectTroopCanvas;
};

class crJXJXuMuDialogueProgressMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJXuMuDialogueProgressMethod();
	crJXJXuMuDialogueProgressMethod(const crJXJXuMuDialogueProgressMethod& handle);
	MethodClass(JXJ, JXJXuMuDialogueProgress)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_curtalkid;
	int m_curtextid;
	std::string m_input;
	std::string m_duihuaswitch;
};

class crJXJXuMuDialogueInitMethod :public CRCore::crMethod
{//Client
public:
	crJXJXuMuDialogueInitMethod();
	crJXJXuMuDialogueInitMethod(const crJXJXuMuDialogueInitMethod& handle);
	MethodClass(JXJ, JXJXuMuDialogueInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_xumucanvas;
	int m_talkid;
};

class crJXJUIXuMuCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIXuMuCanvasMethod();
	crJXJUIXuMuCanvasMethod(const crJXJUIXuMuCanvasMethod& handle);
	MethodClass(JXJ, JXJUIXuMuCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_tipcanvas;
	std::string m_mousecanvas;
	int m_noticeindex;
	std::string m_jiangkacanvas;
};

class crJXJCheckNewHandBattleTroopMoveCanvasMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckNewHandBattleTroopMoveCanvasMethod();
	crJXJCheckNewHandBattleTroopMoveCanvasMethod(const crJXJCheckNewHandBattleTroopMoveCanvasMethod& handle);
	MethodClass(JXJ, JXJCheckNewHandBattleTroopMoveCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crRole *m_this;
	std::string m_moveTroopCanvas;
};

class crJXJCheckNewHandBattleUseJiangKaMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckNewHandBattleUseJiangKaMethod();
	crJXJCheckNewHandBattleUseJiangKaMethod(const crJXJCheckNewHandBattleUseJiangKaMethod& handle);
	MethodClass(JXJ, JXJCheckNewHandBattleUseJiangKa)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaCanvas;
	std::string m_jiangKaPlay1;
	char m_trueExecCount;
};

class crJXJCheckGameIntroduceCanvasInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGameIntroduceCanvasInfoMethod();
	crJXJCheckGameIntroduceCanvasInfoMethod(const crJXJCheckGameIntroduceCanvasInfoMethod& handle);
	MethodClass(JXJ, JXJCheckGameIntroduceCanvasInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_targetCanvas;
	std::string m_playdraw;
	float m_delayTime;
	float m_dt;
	unsigned char m_scale;
};

class crJXJNewHandFocusMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNewHandFocusMethod();
	crJXJNewHandFocusMethod(const crJXJNewHandFocusMethod& handle);
	MethodClass(JXJ, JXJNewHandFocus)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crJXJCheckChuZhengBtnMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckChuZhengBtnMethod();
	crJXJCheckChuZhengBtnMethod(const crJXJCheckChuZhengBtnMethod& handle);
	MethodClass(JXJ, JXJCheckChuZhengBtn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_chuzhengbtn;
	std::string m_bianduibtn;
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIForcedOffLineExitMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJUIForcedOffLineExitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIForcedOffLineExitMethod();
	crJXJUIForcedOffLineExitMethod(const crJXJUIForcedOffLineExitMethod& handle);
	MethodClass(JXJ, JXJUIForcedOffLineExit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_msgCanvas;
	std::string m_message;
	int m_messageIndex;
};

/////////////////////////////////////////
//
//crJXJUIBattleSkillBuffTipMethod
//
/////////////////////////////////////////
class crJXJUIBattleSkillBuffTipMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIBattleSkillBuffTipMethod();
	crJXJUIBattleSkillBuffTipMethod(const crJXJUIBattleSkillBuffTipMethod& handle);
	MethodClass(JXJ, JXJUIBattleSkillBuffTip)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_tipsCanvas;
	std::string m_tipsName;
	std::string m_tipsSkillBuffInfo;
	int m_index_1;
	int m_index_2;
};

class crJXJAutoFuBenRaidoSelectMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoFuBenRaidoSelectMethod();
	crJXJAutoFuBenRaidoSelectMethod(const crJXJAutoFuBenRaidoSelectMethod& handle);
	MethodClass(JXJ, JXJAutoFuBenRaidoSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	std::string m_fubenCanvas;
	std::string m_zhengzhanfubenRaido;
	std::string m_chapterRaido;
	std::string m_fubenRaido;
	std::string m_chapterscroll;
};

/////////////////////////////////////////
//
//crJXJGameDefaultChatModeSelectMethod
//
/////////////////////////////////////////
class crJXJGameDefaultChatModeSelectMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJGameDefaultChatModeSelectMethod();
	crJXJGameDefaultChatModeSelectMethod(const crJXJGameDefaultChatModeSelectMethod& handle);
	MethodClass(JXJ, JXJGameDefaultChatModeSelect)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_defaultmodeSW;
};

class crJXJAllSelectCanvasOpenInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAllSelectCanvasOpenInfoMethod();
	crJXJAllSelectCanvasOpenInfoMethod(const crJXJAllSelectCanvasOpenInfoMethod& handle);
	MethodClass(JXJ, JXJAllSelectCanvasOpenInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::vector<std::string> m_canvasVec;
};

class crJXJUITheSmallMapPointInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUITheSmallMapPointInitMethod();
	crJXJUITheSmallMapPointInitMethod(const crJXJUITheSmallMapPointInitMethod& handle);
	MethodClass(JXJ, JXJUITheSmallMapPointInit)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_cityHallOrBossIcon;
	std::string m_battleMap;
	std::string m_strRotMap;
};

class crJXJUINewHandTaskBuildingLocationMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandTaskBuildingLocationMethod();
	crJXJUINewHandTaskBuildingLocationMethod(const crJXJUINewHandTaskBuildingLocationMethod& handle);
	MethodClass(JXJ, JXJUINewHandTaskBuildingLocation)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_itemid;
	std::string m_canvas;
	std::string m_renwuCanvas;
};

class crJXJCheckTroopsTechTaskInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckTroopsTechTaskInfoMethod();
	crJXJCheckTroopsTechTaskInfoMethod(const crJXJCheckTroopsTechTaskInfoMethod& handle);
	MethodClass(JXJ, JXJCheckTroopsTechTaskInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gongfangcanvas;
	std::string m_armytyperadio;
	int m_troopstechid;
	char m_trueExecCount;
};

class crJXJFormationTaskPositionMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJFormationTaskPositionMethod();
	crJXJFormationTaskPositionMethod(const crJXJFormationTaskPositionMethod& handle);
	MethodClass(JXJ, JXJFormationTaskPosition)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	std::string m_formationList;
	std::string m_rectCanvasName;
	char m_trueExecCount;
	int m_taskid;
	int m_abstractid;
};

class crJXJCheckLingZhuLvInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckLingZhuLvInfoMethod();
	crJXJCheckLingZhuLvInfoMethod(const crJXJCheckLingZhuLvInfoMethod& handle);
	MethodClass(JXJ, JXJCheckLingZhuLvInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	unsigned char m_mylevel;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckTriggerGuideZhuangBeiDZInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckTriggerGuideZhuangBeiDZInfoMethod();
	crJXJCheckTriggerGuideZhuangBeiDZInfoMethod(const crJXJCheckTriggerGuideZhuangBeiDZInfoMethod& handle);
	MethodClass(JXJ, JXJCheckTriggerGuideZhuangBeiDZInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_gongFangCanvas;
	std::string m_radioGroup;
	char m_trueExecCount;//-1表示无穷
	int m_index;
};

class crJXJCheckFirstGetChengKaInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFirstGetChengKaInfoMethod();
	crJXJCheckFirstGetChengKaInfoMethod(const crJXJCheckFirstGetChengKaInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFirstGetChengKaInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckFirstFuBenWinInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFirstFuBenWinInfoMethod();
	crJXJCheckFirstFuBenWinInfoMethod(const crJXJCheckFirstFuBenWinInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFirstFuBenWinInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	unsigned short m_fubenid;
	char m_trueExecCount;//-1表示无穷
};

class crJXJCheckFirstFuBenOpenBoxInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFirstFuBenOpenBoxInfoMethod();
	crJXJCheckFirstFuBenOpenBoxInfoMethod(const crJXJCheckFirstFuBenOpenBoxInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFirstFuBenOpenBoxInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	unsigned short m_fubenid;
	std::string m_canvas;
	char m_trueExecCount;//-1表示无穷
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJButtonRoleToZhenTingMethod
//
//////////////////////////////////////////////////////////////////////////
class crJXJButtonRoleToZhenTingMethod:public CRCore::crMethod
{
public:
	crJXJButtonRoleToZhenTingMethod::crJXJButtonRoleToZhenTingMethod();
	crJXJButtonRoleToZhenTingMethod::crJXJButtonRoleToZhenTingMethod(const crJXJButtonRoleToZhenTingMethod &handle);
	MethodClass(JXJ,JXJButtonRoleToZhenTing);
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
private:
};

class crJXJUINewHandBuildingClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandBuildingClickMethod();
	crJXJUINewHandBuildingClickMethod(const crJXJUINewHandBuildingClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandBuildingClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_renwuCanvas;
};

class crJXJUINewHandFormationTroopsClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandFormationTroopsClickMethod();
	crJXJUINewHandFormationTroopsClickMethod(const crJXJUINewHandFormationTroopsClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandFormationTroopsClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	std::string m_formationList;
	std::string m_renWuCanvas;
};

class crJXJUINewHandKnapsackItemClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandKnapsackItemClickMethod();
	crJXJUINewHandKnapsackItemClickMethod(const crJXJUINewHandKnapsackItemClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandKnapsackItemClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_knapsackCanvas;
	std::string m_knapsackList;
};

class crJXJUINewHandFormationJiangKaClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandFormationJiangKaClickMethod();
	crJXJUINewHandFormationJiangKaClickMethod(const crJXJUINewHandFormationJiangKaClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandFormationJiangKaClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaBagList;
	std::string m_renWuCanvas;
};

class crJXJUINewHandWeiRenJiangKaClickMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUINewHandWeiRenJiangKaClickMethod();
	crJXJUINewHandWeiRenJiangKaClickMethod(const crJXJUINewHandWeiRenJiangKaClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandWeiRenJiangKaClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_weiRenBagCanvas;
	std::string m_weiRenBagList;
	std::string m_rectCanvasName;
	std::string m_renWuCanvas;
	char m_trueExecCount;
	int m_taskid;
	int m_jiangKaid;
};

class crJXJUINewHandUpgradeJiangKaClickMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUINewHandUpgradeJiangKaClickMethod();
	crJXJUINewHandUpgradeJiangKaClickMethod(const crJXJUINewHandUpgradeJiangKaClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandUpgradeJiangKaClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaBagList;
	std::string m_rectCanvasName;
	std::string m_renWuCanvas;
	char m_trueExecCount;
	int m_taskid;
	int m_jiangKaid;
};

class crJXJCheckFormationTemporaryJKInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckFormationTemporaryJKInfoMethod();
	crJXJCheckFormationTemporaryJKInfoMethod(const crJXJCheckFormationTemporaryJKInfoMethod& handle);
	MethodClass(JXJ, JXJCheckFormationTemporaryJKInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	char m_trueExecCount;//-1表示无穷
	int m_index;
};

class crJXJUINewHandKnapsackEquipClickMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUINewHandKnapsackEquipClickMethod();
	crJXJUINewHandKnapsackEquipClickMethod(const crJXJUINewHandKnapsackEquipClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandKnapsackEquipClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_knapsackCanvas;
	std::string m_knapsackList;
	std::string m_rectCanvasName;
	std::string m_renWuCanvas;
	char m_trueExecCount;
	int m_taskid;
	int m_itemid;
};

class crJXJUINewHandUpgradeCardClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandUpgradeCardClickMethod();
	crJXJUINewHandUpgradeCardClickMethod(const crJXJUINewHandUpgradeCardClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandUpgradeCardClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaBagList;
	std::string m_renWuCanvas;
	std::string m_jiangKaColorRadio;
	std::string m_jiangKaSystemRadio;
};

class crJXJSkipNewHandInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJSkipNewHandInfoMethod();
	crJXJSkipNewHandInfoMethod(const crJXJSkipNewHandInfoMethod& handle);
	MethodClass(JXJ, JXJSkipNewHandInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
};

class crJXJUINewHandTroopUndoClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandTroopUndoClickMethod();
	crJXJUINewHandTroopUndoClickMethod(const crJXJUINewHandTroopUndoClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandTroopUndoClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_formationCanvas;
	std::string m_imageBoxWidget;
};

class crJXJUINewHandFormationJiangKaMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUINewHandFormationJiangKaMethod();
	crJXJUINewHandFormationJiangKaMethod(const crJXJUINewHandFormationJiangKaMethod& handle);
	MethodClass(JXJ, JXJUINewHandFormationJiangKa)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaBagList;
	std::string m_rectCanvasName;
	std::string m_renWuCanvas;
	char m_trueExecCount;
	int m_taskid;
	int m_jiangKaid;
};

class crJXJUINewHandFormationCardClickMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUINewHandFormationCardClickMethod();
	crJXJUINewHandFormationCardClickMethod(const crJXJUINewHandFormationCardClickMethod& handle);
	MethodClass(JXJ, JXJUINewHandFormationCardClick)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_jiangKaBagCanvas;
	std::string m_jiangKaBagList;
	std::string m_renWuCanvas;
};

class crJXJCheckGuanZhiOpenInfoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGuanZhiOpenInfoMethod();
	crJXJCheckGuanZhiOpenInfoMethod(const crJXJCheckGuanZhiOpenInfoMethod& handle);
	MethodClass(JXJ, JXJCheckGuanZhiOpenInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	char m_trueExecCount;//-1表示无穷
};

class crJXJUIGongFangOpenLvInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangOpenLvInfoMethod();
	crJXJUIGongFangOpenLvInfoMethod(const crJXJUIGongFangOpenLvInfoMethod& handle);
	MethodClass(JXJ, JXJUIGongFangOpenLvInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	int m_index;
	char m_trueExecCount;
};

class crJXJUIGongFangJingLianInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangJingLianInfoMethod();
	crJXJUIGongFangJingLianInfoMethod(const crJXJUIGongFangJingLianInfoMethod& handle);
	MethodClass(JXJ, JXJUIGongFangJingLianInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gongFangCanvas;
	char m_trueExecCount;
};

class crJXJUIGongFangSaveJingLianInfoMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJUIGongFangSaveJingLianInfoMethod();
	crJXJUIGongFangSaveJingLianInfoMethod(const crJXJUIGongFangSaveJingLianInfoMethod& handle);
	MethodClass(JXJ, JXJUIGongFangSaveJingLianInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_gongFangCanvas;
	char m_trueExecCount;
};

class crJXJCheckGameSaoDangTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckGameSaoDangTaskMethod();
	crJXJCheckGameSaoDangTaskMethod(const crJXJCheckGameSaoDangTaskMethod& handle);
	MethodClass(JXJ, JXJCheckGameSaoDangTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	char m_trueExecCount;
};

class crJXJAutoCheckSaoDangTaskMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJAutoCheckSaoDangTaskMethod();
	crJXJAutoCheckSaoDangTaskMethod(const crJXJAutoCheckSaoDangTaskMethod& handle);
	MethodClass(JXJ, JXJAutoCheckSaoDangTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJUISaoDangRenWuRewardMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJUISaoDangRenWuRewardMethod();
	crJXJUISaoDangRenWuRewardMethod(const crJXJUISaoDangRenWuRewardMethod& handle);
	MethodClass(JXJ, JXJUISaoDangRenWuReward)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crGameTask *m_this;
	std::string m_canvas;
	std::string m_expCount;
	std::string m_copCashCount;
	std::string m_oneCount;
	std::string m_twoCount;
	std::string m_rewardOne;
	std::string m_rewardTwo;
	int m_taskid;
};

class crJXJUISelectSaoDangFubenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISelectSaoDangFubenMethod();
	crJXJUISelectSaoDangFubenMethod(const crJXJUISelectSaoDangFubenMethod& handle);
	MethodClass(JXJ, JXJUISelectSaoDangFuben)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_rushCanvas;
	std::string m_rushCountInput;
	int m_taskid;
};

class crJXJUISetSaoDangFubenInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUISetSaoDangFubenInfoMethod();
	crJXJUISetSaoDangFubenInfoMethod(const crJXJUISetSaoDangFubenInfoMethod& handle);
	MethodClass(JXJ, JXJUISetSaoDangFubenInfo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode* m_this;
	int m_taskid;
};

class crJXJCheckOpenFubenTipCanvasMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckOpenFubenTipCanvasMethod();
	crJXJCheckOpenFubenTipCanvasMethod(const crJXJCheckOpenFubenTipCanvasMethod& handle);
	MethodClass(JXJ, JXJCheckOpenFubenTipCanvas)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static char m_trueExecCount;
protected:
	std::string m_targetCanvas;
};

class crJXJCheckOpenFubenTipCanvasTwoMethod : public CRCore::crMethod
{//GameClient Method 
public:
	crJXJCheckOpenFubenTipCanvasTwoMethod();
	crJXJCheckOpenFubenTipCanvasTwoMethod(const crJXJCheckOpenFubenTipCanvasTwoMethod& handle);
	MethodClass(JXJ, JXJCheckOpenFubenTipCanvasTwo)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	static char m_trueExecCount;
protected:
	std::string m_targetCanvas;
};

class crJXJCheckFirstBuildingTaskMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckFirstBuildingTaskMethod();
	crJXJCheckFirstBuildingTaskMethod(const crJXJCheckFirstBuildingTaskMethod& handle);
	MethodClass(JXJ, JXJCheckFirstBuildingTask)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	int m_buildingid;
};

//class crJXJResetRenWuScrollBarMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJResetRenWuScrollBarMethod();
//	crJXJResetRenWuScrollBarMethod(const crJXJResetRenWuScrollBarMethod& handle);
//	MethodClass(JXJ, JXJResetRenWuScrollBar)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::string m_scrollbar;
//};

class crJXJXuMuInitSelectRoleMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJXuMuInitSelectRoleMethod();
	crJXJXuMuInitSelectRoleMethod(const crJXJXuMuInitSelectRoleMethod& handle);
	MethodClass(JXJ, JXJXuMuInitSelectRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};

class crJXJCheckManorStairTaskbarBtnMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJCheckManorStairTaskbarBtnMethod();
	crJXJCheckManorStairTaskbarBtnMethod(const crJXJCheckManorStairTaskbarBtnMethod& handle);
	MethodClass(JXJ, JXJCheckManorStairTaskbarBtn)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode* m_this;
	std::string m_canvas;
	std::string m_CountryBtn;
	std::string m_GroupBtn;
	std::string m_WeiRenBtn;
};

}
#endif 