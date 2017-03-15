/**********************************************************************
*
*	文件:	   appMethod9.h
*
*	描述:	   
*
*	作者:		刘超 
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD9_H
#define JXJ_APPMETHOD9_H
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

class crJXJChangeGroupLeaderMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeGroupLeaderMethod ();
	crJXJChangeGroupLeaderMethod (const crJXJChangeGroupLeaderMethod & handle);
	MethodClass(JXJ, JXJChangeGroupLeader )
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_groupList;
	int m_newLeader;
};

class crJXJRecvChangeGroupLeaderMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeGroupLeaderMethod();
	crJXJRecvChangeGroupLeaderMethod(const crJXJRecvChangeGroupLeaderMethod & handle);
	MethodClass(JXJ, JXJRecvChangeGroupLeader)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJDismissGroupMethod : public CRCore::crMethod
{//Client Method
public:
	crJXJDismissGroupMethod ();
	crJXJDismissGroupMethod (const crJXJDismissGroupMethod & handle);
	MethodClass(JXJ, JXJDismissGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_btn;
	int m_type;
};

class crJXJRecvDismissGroupMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvDismissGroupMethod();
	crJXJRecvDismissGroupMethod (const crJXJRecvDismissGroupMethod & handle);
	MethodClass(JXJ,JXJRecvDismissGroup)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJCancelDismissGroupMethod: public CRCore::crMethod
{//Client Method
public:
	crJXJCancelDismissGroupMethod ();
	crJXJCancelDismissGroupMethod (const crJXJCancelDismissGroupMethod & handle);
	MethodClass(JXJ, JXJCancelDismissGroup)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	int m_netType;
	std::string m_btn;
};

class crJXJRecvCancelDismissGroupMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvCancelDismissGroupMethod();
	crJXJRecvCancelDismissGroupMethod (const crJXJRecvCancelDismissGroupMethod & handle);
	MethodClass(JXJ,JXJRecvCancelDismissGroup)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJAddGroupMemberMethod: public CRCore::crMethod
{//Client Method
public:
	crJXJAddGroupMemberMethod();
	crJXJAddGroupMemberMethod(const crJXJAddGroupMemberMethod& handle);
	MethodClass(JXJ, JXJAddGroupMember)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	int m_netType;
	std::string m_input;
};

class crJXJRecvAddGroupMemberMethod: public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAddGroupMemberMethod();
	crJXJRecvAddGroupMemberMethod(const crJXJRecvAddGroupMemberMethod& handle);
	MethodClass(JXJ,JXJRecvAddGroupMember)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};


class crJXJAddGroupMemberDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJAddGroupMemberDisposeMethod();
	crJXJAddGroupMemberDisposeMethod(const crJXJAddGroupMemberDisposeMethod& handle);
	MethodClass(JXJ, JXJAddGroupMemberDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJRecvAddGroupMemberResultMethod:public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvAddGroupMemberResultMethod();
	crJXJRecvAddGroupMemberResultMethod(const crJXJRecvAddGroupMemberResultMethod& handle);
	MethodClass(JXJ, JXJRecvAddGroupMemberResult)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};


class crJXJClientInvitedAddGroupMemberMethod: public CRCore::crMethod
{//Client 
public:
	crJXJClientInvitedAddGroupMemberMethod();
	crJXJClientInvitedAddGroupMemberMethod(const crJXJClientInvitedAddGroupMemberMethod& handle);
	MethodClass(JXJ,JXJClientInvitedAddGroupMember)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	//CRNetApp::crPlayerGameData *m_this;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	bool m_agree;
};

class crJXJClientChangeGroupNameMethod: public CRCore::crMethod
{//Client 
public:
	crJXJClientChangeGroupNameMethod();
	crJXJClientChangeGroupNameMethod(const crJXJClientChangeGroupNameMethod& handle);
	MethodClass(JXJ,JXJClientChangeGroupName)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_nameedit;
	int m_validtime;
	int m_lasttime;
};

class crJXJRecvChangeGroupNameMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeGroupNameMethod();
	crJXJRecvChangeGroupNameMethod(const crJXJRecvChangeGroupNameMethod& handle);
	MethodClass(JXJ, JXJRecvChangeGroupName)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};


class crJXJRecvInvitedAddGroupMemberMethod: public CRCore::crMethod
{//Client 
public:
	crJXJRecvInvitedAddGroupMemberMethod();
	crJXJRecvInvitedAddGroupMemberMethod(const crJXJRecvInvitedAddGroupMemberMethod& handle);
	MethodClass(JXJ,JXJRecvInvitedAddGroupMember)
		virtual void operator() (crHandle & handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_lastname ;
	std::string m_input;
	bool m_agree;
};


/************************************************************************/
/*  Group UI                                                                     */
/************************************************************************/

class crJXJAddGroupMemberRequestUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJAddGroupMemberRequestUpdateMethod ();
	crJXJAddGroupMemberRequestUpdateMethod (const crJXJAddGroupMemberRequestUpdateMethod & handle);
	MethodClass(JXJ, JXJAddGroupMemberRequestUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_text;
};

class crJXJMyGroupCanvasRadioUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyGroupCanvasRadioUpdateMethod ();
	crJXJMyGroupCanvasRadioUpdateMethod (const crJXJMyGroupCanvasRadioUpdateMethod & handle);
	MethodClass(JXJ, JXJMyGroupCanvasRadioUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;  
	std::string m_pageradio; 
	std::string m_pagesw; 
};

class crJXJUIChangeGroupNameInitMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeGroupNameInitMethod();
	crJXJUIChangeGroupNameInitMethod(const crJXJUIChangeGroupNameInitMethod& handle);
	MethodClass(JXJ, JXJUIChangeGroupNameInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_info;
	std::string m_nameEdit;
	std::string m_confirmBtn;
};



class crJXJUIMyGroupInfoUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIMyGroupInfoUpdateMethod();
	crJXJUIMyGroupInfoUpdateMethod(const crJXJUIMyGroupInfoUpdateMethod& handle);
	MethodClass(JXJ, JXJUIMyGroupInfoUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_groupName;
	std::string m_groupLevel;
	std::string m_memberCount;
	std::string m_rank;
	std::string m_changeNameBtn;
	std::string m_lookRankBtn;
	std::string m_gameList;
	std::string m_editNoticeBtn;
	std::string m_publishNoticeBtn;
	std::string m_input;
	std::string m_tuiweiBtn;
	std::string m_weirenBtn;
	std::string m_leftBtn;
	std::string m_rightBtn;
	std::string m_yichuBtn;
	std::string m_leavegroupBtn;
	std::string m_sw;
	std::string m_gonggao;
	std::string m_jiechufutuanBtn;
};



class crJXJUIMyGroupManageUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIMyGroupManageUpdateMethod();
	crJXJUIMyGroupManageUpdateMethod(const crJXJUIMyGroupManageUpdateMethod& handle);
	MethodClass(JXJ, JXJUIMyGroupManageUpdate)
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_groupPos;
	std::string m_activeValue;
	std::string m_switch;
	std::string m_notice;
	std::string m_editBtn;
	std::string m_publish;
	std::string m_memberList;
	std::string m_leftPage;
	std::string m_pageNum;
	std::string m_rightPage;
	std::string m_leaveGroup;
	std::string m_changeLeader;
	std::string m_dismissGroup;
	std::string m_kickOutGroup;
	std::string m_dismissAtOnce;
	std::string m_cancelDismiss;
	std::string m_changeLeader_sw;
	std::string m_leaveGroup_sw;
	std::string m_dismissGroup_sw;
	std::string m_kickOutGroup_sw;
	std::string m_editBtn_sw;
	std::string m_publish_sw;
	std::string m_cancelDismiss_sw;
	std::string m_dismissAtOnce_sw;
};

class crJXJUIMyGroupShenHeUpdateMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIMyGroupShenHeUpdateMethod();
	crJXJUIMyGroupShenHeUpdateMethod(const crJXJUIMyGroupShenHeUpdateMethod& handle);
	MethodClass(JXJ, JXJUIMyGroupShenHeUpdate)
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_applyMemberList;
	std::string m_rejectApply;
	std::string m_addOther;
	std::string m_allowApply;
	std::string m_pageInput;
	std::string m_addOther_sw;
	std::string m_rejectApply_sw;
	std::string m_allowApply_sw;
};

class crJXJUILookGroupUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUILookGroupUpdateMethod ();
	crJXJUILookGroupUpdateMethod (const crJXJUILookGroupUpdateMethod & handle);
	MethodClass(JXJ, JXJUILookGroupUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_apply;
	std::string m_cancelApply;
	std::string m_pageinput;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_memberList;
	std::string m_canvas;
	std::string m_leaderName;
	std::string m_groupLevel;
	std::string m_groupRank;
	std::string m_fengyi;
};



class crJXJMyGroupMemberInfoPageUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyGroupMemberInfoPageUpdateMethod();
	crJXJMyGroupMemberInfoPageUpdateMethod(const crJXJMyGroupMemberInfoPageUpdateMethod& handle);
	MethodClass(JXJ, JXJMyGroupMemberInfoPageUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_btnsw; 
	std::string m_leavegroupbtn; 
	std::string m_changecreatorbtn; 
	std::string m_memberlist; 
	std::string m_pageinput; 
	std::string m_farleftbtn; 
	std::string m_leftbtn; 
	std::string m_rightbtn; 
	std::string m_farrightbtn; 
	std::string m_mygroupposinput; 
};

class crJXJChangeGroupNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJChangeGroupNoticeMethod();
	crJXJChangeGroupNoticeMethod(const crJXJChangeGroupNoticeMethod& handle);
	MethodClass(JXJ, JXJChangeGroupNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeedit;
	int m_validtime;
	int m_lasttime;
	unsigned char m_noticetype;
};


class crJXJClientClickChangeGroupNoticeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientClickChangeGroupNoticeMethod ();
	crJXJClientClickChangeGroupNoticeMethod (const crJXJClientClickChangeGroupNoticeMethod & handle);
	MethodClass(JXJ, JXJClientClickChangeGroupNotice)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_noticeedit;
};


class crJXJRecvChangeGroupNoticeMethodMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeGroupNoticeMethodMethod();
	crJXJRecvChangeGroupNoticeMethodMethod(const crJXJRecvChangeGroupNoticeMethodMethod& handle);
	MethodClass(JXJ, JXJRecvChangeGroupNoticeMethod)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};


class crJXJUIGroupListButtonUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupListButtonUpdateMethod();
	crJXJUIGroupListButtonUpdateMethod(const crJXJUIGroupListButtonUpdateMethod& handle);
	MethodClass(JXJ, JXJUIGroupListButtonUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_applybtn[10];
	std::string m_disapplybtn[10];
	std::string m_checkinfobtn[10];
	std::string m_grouplist;
	std::string m_groupcheck;
	std::string m_pageinput;
	std::string m_farleftbtn;
	std::string m_leftbtn;
	std::string m_rightbtn;
	std::string m_farrightbtn;
	int m_checkselect;
};




class crJXJUIMyGroupFuLiUpdateMethod :public CRCore::crMethod
{
public:
	crJXJUIMyGroupFuLiUpdateMethod ();
	crJXJUIMyGroupFuLiUpdateMethod (const crJXJUIMyGroupFuLiUpdateMethod & handle);
	MethodClass(JXJ, JXJUIMyGroupFuLiUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_icon[8];
	std::string m_groupLevel;
	std::string m_groupCurZijin;
	std::string m_groupLvUp;
	std::string m_Crops3Raido;
	std::string m_Money;
	std::string m_LiangShi;
	std::string m_MuCai;
	std::string m_TieKuang;
	std::string m_MaPi;
	std::string m_JuanXian;
	std::string m_Tishi1;
	std::string m_JunTuanFuBen_1;
	std::string m_JunTuanFuBenname;
	std::string m_icon2[4];
	std::string m_QianwangfubenBtn;
	std::string m_Crops3_Radio;
	std::string m_Tishi2;
	std::string m_input1[8];
	std::string m_input2[8];
	std::string m_sw135;
	std::string m_goumai[8];
	std::string m_quchu[8];
	std::string m_jiluBtn;
	std::string m_cunbingBtn;
	std::string m_ListScroll1;
	std::string m_Bar;
	std::string m_curgongxian;
	std::string m_shuaxinBtn;
	std::string m_sw164;
	std::string m_sw163;
};

class crJXJUIGroupContributeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupContributeMethod ();
	crJXJUIGroupContributeMethod (const crJXJUIGroupContributeMethod & handle);
	MethodClass(JXJ,JXJUIGroupContribute)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_resource;
	std::string m_yuanBao1;
	std::string m_yuanBao2;
	std::string m_yuanBao3;
	std::string m_yuanBao4;
	std::string m_canvas;
};

class crJXJRecvGroupContributeMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGroupContributeMethod();
	crJXJRecvGroupContributeMethod(const crJXJRecvGroupContributeMethod& handle);
	MethodClass(JXJ, JXJRecvGroupContribute)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvasName;
};

class crJXJUIGroupIntoFuBenMethod :public CRCore::crMethod
{
	//client
public:
	crJXJUIGroupIntoFuBenMethod ();
	crJXJUIGroupIntoFuBenMethod (const crJXJUIGroupIntoFuBenMethod & handle);
	MethodClass(JXJ, JXJUIGroupIntoFuBen)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio1;
};

class crJXJRecvGroupIntoFuBenMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGroupIntoFuBenMethod();
	crJXJRecvGroupIntoFuBenMethod(const crJXJRecvGroupIntoFuBenMethod& handle);
	MethodClass(JXJ, JXJRecvGroupIntoFuBen)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	//std::string m_canvasName;
};

class crJXJUIGroupFunBenStartMethod :public CRCore::crMethod
{
	//client
public:
	crJXJUIGroupFunBenStartMethod ();
	crJXJUIGroupFunBenStartMethod (const crJXJUIGroupFunBenStartMethod & handle);
	MethodClass(JXJ, JXJUIGroupFunBenStart)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	unsigned char m_type;
	std::string m_radio1;
};


class crJXJRecvGroupStartFuBenMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGroupStartFuBenMethod();
	crJXJRecvGroupStartFuBenMethod(const crJXJRecvGroupStartFuBenMethod& handle);
	MethodClass(JXJ, JXJRecvGroupStartFuBen)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_canvasname;
	std::string m_chatmodesw;
	std::string m_chatinput;
	std::string m_sendbtn;
	std::string m_switch;
};

class crJXJCheckCantonChengChiMethod : public CRCore::crMethod
{
	// client
public:
	crJXJCheckCantonChengChiMethod ();
	crJXJCheckCantonChengChiMethod (const crJXJCheckCantonChengChiMethod & handle);
	MethodClass(JXJ,JXJCheckCantonChengChi)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);

protected:
	std::string m_canvas;

};

class crJXJMyGroupChengChiCanvasSwitchChangeMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJMyGroupChengChiCanvasSwitchChangeMethod ();
	crJXJMyGroupChengChiCanvasSwitchChangeMethod (const crJXJMyGroupChengChiCanvasSwitchChangeMethod & handle);
	MethodClass(JXJ, JXJMyGroupChengChiCanvasSwitchChange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_canvas;
	std::string m_switch;
	int m_value;
};

class crJXJUIResetPageMethod :public CRCore::crMethod
{
	// client
public:
	crJXJUIResetPageMethod ();
	crJXJUIResetPageMethod (const crJXJUIResetPageMethod & handle);
	MethodClass(JXJ, JXJUIResetPage)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	//CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	int m_curPage;
	int m_totalPage;
};

class crJXJQueryMyCantonGroupMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyCantonGroupMapMethod ();
	crJXJQueryMyCantonGroupMapMethod (const crJXJQueryMyCantonGroupMapMethod & handle);
	MethodClass(JXJ, JXJQueryMyCantonGroupMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
};

class crJXJRecvQueryMyCantonGroupMapMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyCantonGroupMapMethod ();
	crJXJRecvQueryMyCantonGroupMapMethod (const crJXJRecvQueryMyCantonGroupMapMethod & handle);
	MethodClass(JXJ, JXJRecvQueryMyCantonGroupMap)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJUIChangeMyChengChiMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIChangeMyChengChiMethod ();
	crJXJUIChangeMyChengChiMethod (const crJXJUIChangeMyChengChiMethod & handle);
	MethodClass(JXJ, JXJUIChangeMyChengChi)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
};

class crJXJRecvChangeMyChengChiMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChangeMyChengChiMethod ();
	crJXJRecvChangeMyChengChiMethod (const crJXJRecvChangeMyChengChiMethod & handle);
	MethodClass(JXJ, JXJRecvChangeMyChengChi)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJUIRemoveChengChiGroupMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIRemoveChengChiGroupMethod();
	crJXJUIRemoveChengChiGroupMethod(const crJXJUIRemoveChengChiGroupMethod& handle);
	MethodClass(JXJ, JXJUIRemoveChengChiGroup)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupList;
};

class crJXJRecvRemoveChengChiGroupMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvRemoveChengChiGroupMethod ();
	crJXJRecvRemoveChengChiGroupMethod(const crJXJRecvRemoveChengChiGroupMethod& handle);
	MethodClass(JXJ, JXJRecvRemoveChengChiGroup)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJUIFenFengChengChiMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIFenFengChengChiMethod();
	crJXJUIFenFengChengChiMethod(const crJXJUIFenFengChengChiMethod& handle);
	MethodClass(JXJ, JXJUIFenFengChengChi)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupList1;
	std::string m_groupList2;
	std::string m_groupRadio;
	unsigned char m_type;
};


class crJXJRecvChengChiFenFengMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvChengChiFenFengMethod ();
	crJXJRecvChengChiFenFengMethod (const crJXJRecvChengChiFenFengMethod & handle);
	MethodClass(JXJ, JXJRecvChengChiFenFeng)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};


class crJXJUIAllocateEarningsMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIAllocateEarningsMethod();
	crJXJUIAllocateEarningsMethod(const crJXJUIAllocateEarningsMethod& handle);
	MethodClass(JXJ, JXJUIAllocateEarnings)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupList;
	std::string m_edit[10];
	std::string m_lock;
};

class crJXJRecvAllocateEarningsMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAllocateEarningsMethod ();
	crJXJRecvAllocateEarningsMethod (const crJXJRecvAllocateEarningsMethod & handle);
	MethodClass(JXJ, JXJRecvAllocateEarnings)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};


class crJXJUIChengChiGroupUpdateMethod :public CRCore::crMethod
{
	// client
public:
	crJXJUIChengChiGroupUpdateMethod ();
	crJXJUIChengChiGroupUpdateMethod (const crJXJUIChengChiGroupUpdateMethod & handle);
	MethodClass(JXJ, JXJUIChengChiGroupUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;

	std::string m_icon[8];
	std::string m_input[8];
	std::string m_goumaiBtn[8];
	std::string m_curgongxiantext;
	std::string m_shuaxinBtn;
	std::string m_chengchiname[3];
	std::string m_chakanBtn[3];
	std::string m_renmingBtn;
	std::string m_Description;
	std::string m_ZhoujuntechanyilanBtn;
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
	std::string m_itemcount[8];
};

class crJXJQueryMyGroupFenFengChengChiMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyGroupFenFengChengChiMapMethod ();
	crJXJQueryMyGroupFenFengChengChiMapMethod (const crJXJQueryMyGroupFenFengChengChiMapMethod & handle);
	MethodClass(JXJ, JXJQueryMyGroupFenFengChengChiMap)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};


class crJXJRecvQueryMyGroupFenFengChengChiMapMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyGroupFenFengChengChiMapMethod();
	crJXJRecvQueryMyGroupFenFengChengChiMapMethod(const crJXJRecvQueryMyGroupFenFengChengChiMapMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyGroupFenFengChengChiMap)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};


class crJXJUIAddGroupMemberInitMethod:public CRCore::crMethod
{//Client Method
public:
	crJXJUIAddGroupMemberInitMethod();
	crJXJUIAddGroupMemberInitMethod(const crJXJUIAddGroupMemberInitMethod& handle);
	MethodClass(JXJ,JXJUIAddGroupMemberInit)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_info;
	std::string m_nameEdit;
};


class crJXJUIGroupChengChiEarningsLockMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIGroupChengChiEarningsLockMethod();
	crJXJUIGroupChengChiEarningsLockMethod(const crJXJUIGroupChengChiEarningsLockMethod& handle);
	MethodClass(JXJ, JXJUIGroupChengChiEarningsLock)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupList;
	std::string m_edit[10];
	std::string m_progressBar[10];
	std::string m_confirm;
	std::string m_confirm_sw;
};


class crJXJUIScrollChangePageMethod:public CRCore::crMethod
{//Client 
public:
	crJXJUIScrollChangePageMethod();
	crJXJUIScrollChangePageMethod(const crJXJUIScrollChangePageMethod& handle);
	MethodClass(JXJ, JXJUIScrollChangePage)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	int m_step;
	int m_curPage;
	int m_totalPage;
	int m_perpageNum;
	std::string m_canvas;
};


class crJXJResetScrollPageMethod:public CRCore::crMethod
{//Client 
public:
	crJXJResetScrollPageMethod();
	crJXJResetScrollPageMethod(const crJXJResetScrollPageMethod& handle);
	MethodClass(JXJ, JXJResetScrollPage)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	int m_curPage;
	int m_totalPage;
	int m_perpageNum;
	std::string m_canvas;
	std::string m_scroll;
};


class crJXJSetRankUIRadioSelectMethod:public CRCore::crMethod
{//Client 
public:
	crJXJSetRankUIRadioSelectMethod();
	crJXJSetRankUIRadioSelectMethod(const crJXJSetRankUIRadioSelectMethod& handle);
	MethodClass(JXJ, JXJSetRankUIRadioSelect)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_rankCanvas;
	std::string m_rankRadio;
	std::string m_page_sw;
	int m_radioPage;
};

class crJXJShowGroupConfirmMethod:public CRCore::crMethod
{//Client 
public:
	crJXJShowGroupConfirmMethod();
	crJXJShowGroupConfirmMethod(const crJXJShowGroupConfirmMethod& handle);
	MethodClass(JXJ, JXJShowGroupConfirm)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode* m_this;
	std::string m_msgbox;
	std::string m_msg;
	std::string m_ok;
	int m_idx;
};

class crJXJGameServerDayZeroMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerDayZeroMethod ();
	crJXJGameServerDayZeroMethod (const crJXJGameServerDayZeroMethod & handle);
	MethodClass(JXJ, JXJGameServerDayZero)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	time_t m_lasttimer;
};


class crJXJGameServerGroupZeroMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerGroupZeroMethod ();
	crJXJGameServerGroupZeroMethod (const crJXJGameServerGroupZeroMethod & handle);
	MethodClass(JXJ, JXJGameServerGroupZero)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	unsigned char m_zeroType;
	
};

class crJXJMyGroupCanvasGlobalUpdateMethod :public CRCore::crMethod
{
	// client
public:
	crJXJMyGroupCanvasGlobalUpdateMethod ();
	crJXJMyGroupCanvasGlobalUpdateMethod (const crJXJMyGroupCanvasGlobalUpdateMethod & handle);
	MethodClass(JXJ, JXJMyGroupCanvasGlobalUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_title;
	std::string m_dongHua1;
};


class crJXJDismissGroupDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJDismissGroupDisposeMethod ();
	crJXJDismissGroupDisposeMethod (const crJXJDismissGroupDisposeMethod & handle);
	MethodClass(JXJ, JXJDismissGroupDispose)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJDismissTimerMethod :public CRCore::crMethod
{
	// client
public:
	crJXJDismissTimerMethod ();
	crJXJDismissTimerMethod (const crJXJDismissTimerMethod & handle);
	MethodClass(JXJ, JXJDismissTimer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJGameServerDismissGroupDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGameServerDismissGroupDisposeMethod ();
	crJXJGameServerDismissGroupDisposeMethod (const crJXJGameServerDismissGroupDisposeMethod & handle);
	MethodClass(JXJ, JXJGameServerDismissGroupDispose)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};



class crJXJGroupNotExistMethod :public CRCore::crMethod
{//Client 
public:
	crJXJGroupNotExistMethod ();
	crJXJGroupNotExistMethod (const crJXJGroupNotExistMethod & handle);
	MethodClass(JXJ, JXJGroupNotExist)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//int m_netType;
	//std::string m_mygroupcanvas;
};

class crJXJSaveConsortiaLvRankMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJSaveConsortiaLvRankMethod ();
	crJXJSaveConsortiaLvRankMethod (const crJXJSaveConsortiaLvRankMethod & handle);
	MethodClass(JXJ, JXJSaveConsortiaLvRank)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//std::vector<std::string> m_expRankingFiles;
	std::string m_rankFile;
};

//class crJXJGetChengChiEarningsMethod :public CRCore::crMethod
//{
//	// client
//public:
//	crJXJGetChengChiEarningsMethod ();
//	crJXJGetChengChiEarningsMethod (const crJXJGetChengChiEarningsMethod & handle);
//	MethodClass(JXJ, JXJGetChengChiEarnings)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRCore::crWidgetNode *m_this;
//};

class crJXJRecvGetChengChiEarningsMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvGetChengChiEarningsMethod();
	crJXJRecvGetChengChiEarningsMethod(const crJXJRecvGetChengChiEarningsMethod& handle);
	MethodClass(JXJ, JXJRecvGetChengChiEarnings)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJDeductResourceMethod :public CRCore::crMethod
{// GameServer Method
public:
	crJXJDeductResourceMethod ();
	crJXJDeductResourceMethod (const crJXJDeductResourceMethod & handle);
	MethodClass(JXJ, JXJDeductResource)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	ResourceChangeData * m_resourceData;
	bool* m_output;
};

class crJXJRecvDeductThingMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvDeductThingMethod ();
	crJXJRecvDeductThingMethod (const crJXJRecvDeductThingMethod & handle);
	MethodClass(JXJ, JXJRecvDeductThing)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJRecvDeductResourceMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvDeductResourceMethod ();
	crJXJRecvDeductResourceMethod (const crJXJRecvDeductResourceMethod & handle);
	MethodClass(JXJ, JXJRecvDeductResource)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJFullOfCountryCanvasZhoujunMapMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFullOfCountryCanvasZhoujunMapMethod ();
	crJXJFullOfCountryCanvasZhoujunMapMethod (const crJXJFullOfCountryCanvasZhoujunMapMethod & handle);
	MethodClass(JXJ, JXJFullOfCountryCanvasZhoujunMap)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_myCountryCanvas;
};

class crJXJQueryMyCounrtyCantonChengChiINfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyCounrtyCantonChengChiINfoMethod ();
	crJXJQueryMyCounrtyCantonChengChiINfoMethod (const crJXJQueryMyCounrtyCantonChengChiINfoMethod & handle);
	MethodClass(JXJ, JXJQueryMyCounrtyCantonChengChiINfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupRadio;
};

class crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod();
	crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod(const crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod& handle);
	MethodClass(JXJ, JXJRecvQueryMyCounrtyCantonChengChiINfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_myCountryCanvas;
	std::string m_myCountry_Apply;
	std::string m_myCountry_Cancle;
	std::string m_myCountry_Apply_sw;
	std::string m_myCountry_Cancle_sw;
};


class crJXJMyGroupCanvasDongHuaUpdateMethod :public CRCore::crMethod
{
	// client
public:
	crJXJMyGroupCanvasDongHuaUpdateMethod ();
	crJXJMyGroupCanvasDongHuaUpdateMethod (const crJXJMyGroupCanvasDongHuaUpdateMethod & handle);
	MethodClass(JXJ, JXJMyGroupCanvasDongHuaUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};


class crJXJClearGroupMemberRankMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJClearGroupMemberRankMethod ();
	crJXJClearGroupMemberRankMethod (const crJXJClearGroupMemberRankMethod & handle);
	MethodClass(JXJ, JXJClearGroupMemberRank)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};


class crJXJRecvChatMsgMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvChatMsgMethod ();
	crJXJRecvChatMsgMethod (const crJXJRecvChatMsgMethod & handle);
	MethodClass(JXJ, JXJRecvChatMsg)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};


class JXJGroupExpChatMsg  : public JXJGroupChatMsgBase
{
public:
	JXJGroupExpChatMsg ();
	virtual void getCompleteString(std::string &); 
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	int m_exp;
};

class JXJGroupAddNewmemberMsg : public JXJGroupChatMsgBase
{
public:
	JXJGroupAddNewmemberMsg ();
	virtual void getCompleteString(std::string &);
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	std::string m_name;
};

class JXJChangeGroupLeaderMsg : public JXJGroupAddNewmemberMsg 
{
public:
	JXJChangeGroupLeaderMsg ()
	{
		m_infoType = JXJGroupChatMsgType_changeGroupLeader;
		m_msgId = 4050;
	}
};

class JXJGroupKickeMemberMsg : public JXJGroupAddNewmemberMsg 
{
public:
	JXJGroupKickeMemberMsg ()
	{
		m_infoType = JXJGroupChatMsgType_removeMember;
		m_msgId = 4071;
	}
	//virtual void getCompleteString(std::string &);
	//virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	//virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	//std::string m_name;
};

class JXJGroupContributeSourceMsg : public JXJGroupChatMsgBase
{
public:
	JXJGroupContributeSourceMsg()
	{
		m_infoType = JXJGroupChatMsgType_contribute_source;
		m_msgId = 4076;
	}
	virtual void getCompleteString(std::string &);
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	std::string m_name;
	int m_copper;
	int m_food;
	int m_wood;
	int m_iron;
	int m_horse;
};

class JXJGroupContributeYuanbaoMsg : public JXJGroupChatMsgBase
{
public:
	JXJGroupContributeYuanbaoMsg ()
	{
		m_infoType = JXJGroupChatMsgType_contribute_yuanbao;
		m_msgId = 4074;
	}
	virtual void getCompleteString(std::string &);
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	std::string m_name;
	int m_yuanbao;
};

class JXJGroupStartFuBenMsg: public JXJGroupChatMsgBase
{
public:
	JXJGroupStartFuBenMsg()
	{
		m_infoType = JXJGroupChatMsgType_startFuBen;
		m_msgId = 4075;
	}
	virtual void getCompleteString(std::string &);
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	
	std::string m_name;
	std::string m_fubenName;
};

class JXJGroupChatMsgManager
{
public:
	JXJGroupChatMsgBase * operator () (unsigned char _type)
	{
		switch (_type)
		{
		case JXJGroupChatMsgType_exp:
			return new JXJGroupExpChatMsg;
		case JXJGroupChatMsgType_addNewMember:
			return new JXJGroupAddNewmemberMsg ;
		case JXJGroupChatMsgType_changeGroupLeader:
			return new JXJChangeGroupLeaderMsg ;
		case JXJGroupChatMsgType_removeMember:
			return new JXJGroupKickeMemberMsg ;
		default:
			return NULL;
		}
	}
};


class crJXJPutIntoGroupChatMsgQueueMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJPutIntoGroupChatMsgQueueMethod ();
	crJXJPutIntoGroupChatMsgQueueMethod (const crJXJPutIntoGroupChatMsgQueueMethod & handle);
	MethodClass(JXJ,JXJPutIntoGroupChatMsgQueue )
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	CRCore::ref_ptr<JXJGroupChatMsgBase > m_msgPtr;
	int m_groupId;
};

class crJXJGroupChatMsgQueueDisposeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGroupChatMsgQueueDisposeMethod ();
	crJXJGroupChatMsgQueueDisposeMethod (const crJXJGroupChatMsgQueueDisposeMethod & handle);
	MethodClass(JXJ,JXJGroupChatMsgQueueDispose)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
};

class crJXJGroupFundsUpgradeMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJGroupFundsUpgradeMethod ();
	crJXJGroupFundsUpgradeMethod (const crJXJGroupFundsUpgradeMethod & handle);
	MethodClass(JXJ,JXJGroupFundsUpgrade)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	int *m_funds;
	int m_groupId ;
};

class crJXJClientRecvCreateGroupMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJClientRecvCreateGroupMethod();
	crJXJClientRecvCreateGroupMethod(const crJXJClientRecvCreateGroupMethod& handle);
	MethodClass(JXJ, JXJClientRecvCreateGroup)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};


class crJXJUIGroupListNoticeUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIGroupListNoticeUpdateMethod ();
	crJXJUIGroupListNoticeUpdateMethod (const crJXJUIGroupListNoticeUpdateMethod & handle);
	MethodClass(JXJ, JXJUIGroupListNoticeUpdate)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_grouplist;
	std::string m_recuitNotice;
	std::string m_canvas;
};

struct FubenKillAndStarInfo
{
	int npcCount;
	int bossCount;
	int starCount;
};

class crJXJJunTuanFuBenSettleMethod :public CRCore::crMethod
{//GameServer Method
public:

	struct FuBenPlayerInfo
	{
		FuBenPlayerInfo() :win(false),fubenID(0),playerMap(NULL){}
		bool win;
		unsigned short fubenID;

		std::map< int, FubenKillAndStarInfo > *playerMap;
	};

	crJXJJunTuanFuBenSettleMethod ();
	crJXJJunTuanFuBenSettleMethod (const crJXJJunTuanFuBenSettleMethod & handle);
	MethodClass(JXJ,JXJJunTuanFuBenSettle)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	FuBenPlayerInfo * m_fubenPlayerInfo;
	float m_circle;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//int m_fuBenId;
};


class crJXJJunTuanFuBenSettleClientMethod:public CRCore::crMethod
{//Client  Method
public:
	crJXJJunTuanFuBenSettleClientMethod();
	crJXJJunTuanFuBenSettleClientMethod(const crJXJJunTuanFuBenSettleClientMethod& handle);
	MethodClass(JXJ, JXJJunTuanFuBenSettleClient)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJJunTuanFuBenSettleCanvasUpdateMethod :public CRCore::crMethod
{ // client
public:
	crJXJJunTuanFuBenSettleCanvasUpdateMethod ();
	crJXJJunTuanFuBenSettleCanvasUpdateMethod (const crJXJJunTuanFuBenSettleCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJJunTuanFuBenSettleCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_input;
	std::string m_btn;
	std::string m_btn_sw;
	std::string m_icon[3];
	std::string m_icon_image[3];
	std::string m_input2;
	std::string m_input3;
	std::string m_star_sw;
};

class crJXJJunTuanFuBenSettleGetRewardMethod :public CRCore::crMethod
{ // client
public:
	crJXJJunTuanFuBenSettleGetRewardMethod ();
	crJXJJunTuanFuBenSettleGetRewardMethod (const crJXJJunTuanFuBenSettleGetRewardMethod & handle);
	MethodClass(JXJ, JXJJunTuanFuBenSettleGetReward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJRecvJunTuanFuBenSettleGetRewardMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvJunTuanFuBenSettleGetRewardMethod();
	crJXJRecvJunTuanFuBenSettleGetRewardMethod(const crJXJRecvJunTuanFuBenSettleGetRewardMethod& handle);
	MethodClass(JXJ, JXJRecvJunTuanFuBenSettleGetReward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
};

class crJXJJunTuanFuBenSettleDoubleMethod :public CRCore::crMethod
{ // client
public:
	crJXJJunTuanFuBenSettleDoubleMethod ();
	crJXJJunTuanFuBenSettleDoubleMethod (const crJXJJunTuanFuBenSettleDoubleMethod & handle);
	MethodClass(JXJ, JXJJunTuanFuBenSettleDouble)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
};

class crJXJRecvJunTuanFuBenSettleDoubleMethod:public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvJunTuanFuBenSettleDoubleMethod();
	crJXJRecvJunTuanFuBenSettleDoubleMethod(const crJXJRecvJunTuanFuBenSettleDoubleMethod& handle);
	MethodClass(JXJ, JXJRecvJunTuanFuBenSettleDouble)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_animationCanvas;
	std::string m_animation[2];
};

class crJXJQueryMyCantonInfoMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryMyCantonInfoMethod ();
	crJXJQueryMyCantonInfoMethod (const crJXJQueryMyCantonInfoMethod & handle);
	MethodClass(JXJ, JXJQueryMyCantonInfo)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crWidgetNode *m_this;
	//std::string m_canvas;
	//std::string m_groupRadio;
};

class crJXJRecvQueryMyCantonInfoMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvQueryMyCantonInfoMethod ();
	crJXJRecvQueryMyCantonInfoMethod (const crJXJRecvQueryMyCantonInfoMethod & handle);
	MethodClass(JXJ, JXJRecvQueryMyCantonInfo)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_mygroupcanvas;
};

class crJXJNameLimitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJNameLimitMethod ();
	crJXJNameLimitMethod (const crJXJNameLimitMethod & handle);
	MethodClass(JXJ, JXJNameLimit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_input;
	unsigned short m_length;
};

class crJXJCheckNameLimitMethod :public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJCheckNameLimitMethod ();
	crJXJCheckNameLimitMethod (const crJXJCheckNameLimitMethod & handle);
	MethodClass(JXJ, JXJCheckNameLimit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	std::string m_checkstring;
	int *m_success;
};

class crJXJGroupSetSelectedCantonIDMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupSetSelectedCantonIDMethod ();
	crJXJGroupSetSelectedCantonIDMethod (const crJXJGroupSetSelectedCantonIDMethod & handle);
	MethodClass(JXJ, JXJGroupSetSelectedCantonID)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_radio;
};

//class crJXJGroupAllocateChengChiMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJGroupAllocateChengChiMethod ();
//	crJXJGroupAllocateChengChiMethod (const crJXJGroupAllocateChengChiMethod & handle);
//	MethodClass(JXJ, JXJGroupAllocateChengChi)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRCore::crWidgetNode *m_this;
//	unsigned char m_offset;
//	std::string m_canvas;
//	std::string m_table;
//	unsigned char m_type;
//};
//
//class crJXJRecvGroupAllocateChengChiMethod:public CRCore::crMethod
//{//GameServer Method & client
//public:
//	crJXJRecvGroupAllocateChengChiMethod();
//	crJXJRecvGroupAllocateChengChiMethod(const crJXJRecvGroupAllocateChengChiMethod& handle);
//	MethodClass(JXJ, JXJRecvGroupAllocateChengChi)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_canvas;
//};

//class crJXJResetGroupChengChiIdVecMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJResetGroupChengChiIdVecMethod ();
//	crJXJResetGroupChengChiIdVecMethod (const crJXJResetGroupChengChiIdVecMethod & handle);
//	MethodClass(JXJ, JXJResetGroupChengChiIdVec)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRCore::crWidgetNode *m_this;
//	std::string m_canvas;
//	std::string m_scroll;
//};

class crJXJMaintainShiliPeopleMapMethod:public CRCore::crMethod
{//GameServer Method 
public:
	crJXJMaintainShiliPeopleMapMethod();
	crJXJMaintainShiliPeopleMapMethod(const crJXJMaintainShiliPeopleMapMethod& handle);
	MethodClass(JXJ, JXJMaintainShiliPeopleMap)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	unsigned char m_isOnline;					// 0，下线 ，1 上线
};

class crJXJGroupFuLiRadioUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupFuLiRadioUpdateMethod ();
	crJXJGroupFuLiRadioUpdateMethod (const crJXJGroupFuLiRadioUpdateMethod & handle);
	MethodClass(JXJ,JXJGroupFuLiRadioUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
		virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_contribute[5];
	//std::string m_contribute_sw[5];
	std::string m_canvas;
	std::string m_contributeBtn;
	std::string m_contributeBtn_sw;
	std::string m_radioGroup ;
	unsigned char m_type ;
};

class crJXJCheckConsortiaDayZeroMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJCheckConsortiaDayZeroMethod ();
	crJXJCheckConsortiaDayZeroMethod (const crJXJCheckConsortiaDayZeroMethod & handle);
	MethodClass(JXJ,JXJCheckConsortiaDayZero)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crServerBrainHandle *m_this;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//int m_fuBenId;
};

class crJXJRecvPingMsgMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPingMsgMethod ();
	crJXJRecvPingMsgMethod (const crJXJRecvPingMsgMethod & handle);
	MethodClass(JXJ, JXJRecvPingMsg)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

class crJXJStartFuBenConfirmMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJStartFuBenConfirmMethod ();
	crJXJStartFuBenConfirmMethod (const crJXJStartFuBenConfirmMethod & handle);
	MethodClass(JXJ, JXJStartFuBenConfirm)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_groupCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_canvas ;
	unsigned char m_idx; 
};

class crJXJExtraCritPercentFixedMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraCritPercentFixedMethod ();
	crJXJExtraCritPercentFixedMethod (const crJXJExtraCritPercentFixedMethod & handle);
	MethodClass(JXJ, JXJExtraCritPercentFixed)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};


class crJXJExtraSightRangeMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraSightRangeMethod ();
	crJXJExtraSightRangeMethod (const crJXJExtraSightRangeMethod & handle);
	MethodClass(JXJ, JXJExtraSightRange)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};


class crJXJExtraStealthMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraStealthMethod ();
	crJXJExtraStealthMethod (const crJXJExtraStealthMethod & handle);
	MethodClass(JXJ, JXJExtraStealth)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};


class crJXJExtraStealthSpyMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJExtraStealthSpyMethod ();
	crJXJExtraStealthSpyMethod (const crJXJExtraStealthSpyMethod & handle);
	MethodClass(JXJ, JXJExtraStealthSpy)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
	//short m_extraIncreesValue;
};


class crJXJIgnoreDodgeMethod : public CRCore::crMethod
{//SceneServer & GameClient Method
public:
	crJXJIgnoreDodgeMethod ();
	crJXJIgnoreDodgeMethod (const crJXJIgnoreDodgeMethod & handle);
	MethodClass(JXJ, JXJIgnoreDodge)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	bool m_start;
	float m_timer;
};

class crGetRadioGroupValue2Method : public CRCore::crMethod
{
public:
	crGetRadioGroupValue2Method ();
	crGetRadioGroupValue2Method (const crGetRadioGroupValue2Method& handle);
	MethodClass(CRNetApp, GetRadioGroupValue2)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	//CRCore::crWidgetNode*m_this;
	std::string m_canvas;
	std::string m_radioGroup;
};

//class crJXJExtraOverlapMethod: public CRCore::crMethod
//{
//public:
//	crJXJExtraOverlapMethod();
//	crJXJExtraOverlapMethod(const crJXJExtraOverlapMethod& handle);
//	int getTimes(){return m_times;}
//protected:
//	int m_times;
//};

class crJXJExtraAttackSpeedOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少点 攻速
public:
	crJXJExtraAttackSpeedOverlapMethod();
	crJXJExtraAttackSpeedOverlapMethod(const crJXJExtraAttackSpeedOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraAttackSpeedOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraMoveSpeedPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 移速
public:
	crJXJExtraMoveSpeedPercentOverlapMethod();
	crJXJExtraMoveSpeedPercentOverlapMethod(const crJXJExtraMoveSpeedPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraMoveSpeedPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraAttackPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 攻击力
public:
	crJXJExtraAttackPercentOverlapMethod();
	crJXJExtraAttackPercentOverlapMethod(const crJXJExtraAttackPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraAttackPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraBuildingAttackPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 攻城攻击力
public:
	crJXJExtraBuildingAttackPercentOverlapMethod();
	crJXJExtraBuildingAttackPercentOverlapMethod(const crJXJExtraBuildingAttackPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraBuildingAttackPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraParrayPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 格挡
public:
	crJXJExtraParrayPercentOverlapMethod();
	crJXJExtraParrayPercentOverlapMethod(const crJXJExtraParrayPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraParrayPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraDodgePercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 闪避
public:
	crJXJExtraDodgePercentOverlapMethod();
	crJXJExtraDodgePercentOverlapMethod(const crJXJExtraDodgePercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraDodgePercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraHitRatioPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 击中
public:
	crJXJExtraHitRatioPercentOverlapMethod();
	crJXJExtraHitRatioPercentOverlapMethod(const crJXJExtraHitRatioPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraHitRatioPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraCritPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 暴击
public:
	crJXJExtraCritPercentOverlapMethod();
	crJXJExtraCritPercentOverlapMethod(const crJXJExtraCritPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraCritPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraAttackDistOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 攻击距离
public:
	crJXJExtraAttackDistOverlapMethod();
	crJXJExtraAttackDistOverlapMethod(const crJXJExtraAttackDistOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraAttackDistOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraIgnoreDefendOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 物理穿透
public:
	crJXJExtraIgnoreDefendOverlapMethod();
	crJXJExtraIgnoreDefendOverlapMethod(const crJXJExtraIgnoreDefendOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraIgnoreDefendOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraIgnoreMagicOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 法术穿透
public:
	crJXJExtraIgnoreMagicOverlapMethod();
	crJXJExtraIgnoreMagicOverlapMethod(const crJXJExtraIgnoreMagicOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraIgnoreMagicOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraAttackDamageBackOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 物伤反弹 
public:
	crJXJExtraAttackDamageBackOverlapMethod();
	crJXJExtraAttackDamageBackOverlapMethod(const crJXJExtraAttackDamageBackOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraAttackDamageBackOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraSkillDamageBackOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 法术反弹 
public:
	crJXJExtraSkillDamageBackOverlapMethod();
	crJXJExtraSkillDamageBackOverlapMethod(const crJXJExtraSkillDamageBackOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraSkillDamageBackOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraAttackAbsorbHPOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 物理 吸血
public:
	crJXJExtraAttackAbsorbHPOverlapMethod();
	crJXJExtraAttackAbsorbHPOverlapMethod(const crJXJExtraAttackAbsorbHPOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraAttackAbsorbHPOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};


class crJXJExtraSkillAbsorbHPOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 法术吸血
public:
	crJXJExtraSkillAbsorbHPOverlapMethod();
	crJXJExtraSkillAbsorbHPOverlapMethod(const crJXJExtraSkillAbsorbHPOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraSkillAbsorbHPOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

class crJXJExtraMagicDefendPercentOverlapMethod: public CRCore::crMethod
{//SceneServer & GameClient Method每秒增加/减少多少 魔甲
public:
	crJXJExtraMagicDefendPercentOverlapMethod();
	crJXJExtraMagicDefendPercentOverlapMethod(const crJXJExtraMagicDefendPercentOverlapMethod& handle);
	MethodClass(JXJ, JXJExtraMagicDefendPercentOverlap)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	short m_extraValue;
	short m_addValue;
	float m_interval;

	bool m_start;
	float m_timer;
	float m_dtrec;
};

//////////////////////////////////////////////////////////////////////////overlap method over
class crJXJSetAndCheckPreSelectedUIEarningsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetAndCheckPreSelectedUIEarningsMethod ();
	crJXJSetAndCheckPreSelectedUIEarningsMethod (const crJXJSetAndCheckPreSelectedUIEarningsMethod & handle);
	MethodClass(JXJ, JXJSetAndCheckPreSelectedUIEarnings)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_groupList;
	std::string m_progressBar[9];
	std::string m_edit[9];
	std::string m_confirm;
	std::string m_staticText;
};

class crJXJUIFuBenSettleTimerUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFuBenSettleTimerUpdateMethod ();
	crJXJUIFuBenSettleTimerUpdateMethod (const crJXJUIFuBenSettleTimerUpdateMethod & handle);
	MethodClass(JXJ, JXJUIFuBenSettleTimerUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_timerEdit;
	unsigned char m_op;
	unsigned char m_time ;
};

class crJXJClearTargetExtraMethod : public CRCore::crMethod
{//SceneServer ChildItem Method				清除负面效果
public: 
	crJXJClearTargetExtraMethod ();
	crJXJClearTargetExtraMethod (const crJXJClearTargetExtraMethod & handle);
	MethodClass(JXJ, JXJClearTargetExtra)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	char m_exceptBufType;
};


class crJXJGroupResetGroupIdMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupResetGroupIdMethod ();
	crJXJGroupResetGroupIdMethod (const crJXJGroupResetGroupIdMethod & handle);
	MethodClass(JXJ, JXJGroupResetGroupId)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

typedef std::vector<int> ExceptCardVec;
class crJXJClearSkillCDMethod : public CRCore::crMethod
{//SceneServer Method				清技能CD
public:
	crJXJClearSkillCDMethod ();
	crJXJClearSkillCDMethod (const crJXJClearSkillCDMethod & handle);
	MethodClass(JXJ, JXJClearSkillCD)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	bool isExceptCard(int itemId);
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	int m_flg;//0攻击者，1表示受击者
	ExceptCardVec m_exceptVec;
};

class crJXJFPSAndPingUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJFPSAndPingUpdateMethod ();
	crJXJFPSAndPingUpdateMethod (const crJXJFPSAndPingUpdateMethod & handle);
	MethodClass(JXJ, JXJFPSAndPingUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_fps;
	std::string m_netPing;
	std::string m_url;
	std::string m_type;
	int m_xumusceneid;
	static bool s_recored;
};

class crJXJRecvPingRetMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRecvPingRetMethod ();
	crJXJRecvPingRetMethod (const crJXJRecvPingRetMethod & handle);
	MethodClass(JXJ, JXJRecvPingRet)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_manorCanvas;
	std::string m_battleCanvas;
};


class crJXJRelocateFPSAndPingCanvasMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJRelocateFPSAndPingCanvasMethod ();
	crJXJRelocateFPSAndPingCanvasMethod (const crJXJRelocateFPSAndPingCanvasMethod & handle);
	MethodClass(JXJ, JXJRelocateFPSAndPingCanvas)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_manorCanvas;
	std::string m_battleCanvas;
};

class crJXJUIFuBenSettleTimerCheckMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJUIFuBenSettleTimerCheckMethod ();
	crJXJUIFuBenSettleTimerCheckMethod (const crJXJUIFuBenSettleTimerCheckMethod & handle);
	MethodClass(JXJ, JXJUIFuBenSettleTimerCheck)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_canvas;
	std::string m_receiveBtn;
};

class crJXJCheckIsJunTuanFuBenMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJCheckIsJunTuanFuBenMethod ();
	crJXJCheckIsJunTuanFuBenMethod (const crJXJCheckIsJunTuanFuBenMethod & handle);
	MethodClass(JXJ, JXJCheckIsJunTuanFuBen)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
};

class crJXJGroupCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJGroupCanvasInitMethod ();
	crJXJGroupCanvasInitMethod (const crJXJGroupCanvasInitMethod & handle);
	MethodClass(JXJ, JXJGroupCanvasInit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
};

//class crJXJHuodongUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJHuodongUpdateMethod ();
//	crJXJHuodongUpdateMethod (const crJXJHuodongUpdateMethod & handle);
//	MethodClass(JXJ, JXJHuodongUpdate)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	float* m_dt;
//	std::string m_canvas;
//};

//class crJXJRecvHuodongChangeStatusMethod :public CRCore::crMethod
//{//GameServer & GameClient Method
//public:
//	crJXJRecvHuodongChangeStatusMethod ();
//	crJXJRecvHuodongChangeStatusMethod (const crJXJRecvHuodongChangeStatusMethod & handle);
//	MethodClass(JXJ,JXJRecvHuodongChangeStatus)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	float m_time;
//	std::string m_canvas;
//};

class crJXJRecvHuodongGetAwardMethod :public CRCore::crMethod
{//GameServer & GameClient Method
public:
	crJXJRecvHuodongGetAwardMethod ();
	crJXJRecvHuodongGetAwardMethod (const crJXJRecvHuodongGetAwardMethod & handle);
	MethodClass(JXJ, JXJRecvHuodongGetAward)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_input;
	std::string m_sequence ;
	std::string m_animation;
};

class crJXJRecvClearSkillCDMethod : public CRCore::crMethod
{//Client Method					// Client清技能CD
public:
	crJXJRecvClearSkillCDMethod ();
	crJXJRecvClearSkillCDMethod (const crJXJRecvClearSkillCDMethod & handle);
	MethodClass(JXJ, JXJRecvClearSkillCD)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJRecvClearTargetExtraMethod : public CRCore::crMethod
{//Client Method					// client清负面效果
public:
	crJXJRecvClearTargetExtraMethod ();
	crJXJRecvClearTargetExtraMethod (const crJXJRecvClearTargetExtraMethod & handle);
	MethodClass(JXJ, JXJRecvClearTargetExtra)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	ExceptCardVec m_exceptVec;
};
//
//class crJXJRecvHuodongSynMethod :public CRCore::crMethod
//{//GameServer 
//public:
//	crJXJRecvHuodongSynMethod ();
//	crJXJRecvHuodongSynMethod (const crJXJRecvHuodongSynMethod & handle);
//	MethodClass(JXJ, JXJRecvHuodongSyn)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//};

class crJXJHuodongCanvasInitMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHuodongCanvasInitMethod ();
	crJXJHuodongCanvasInitMethod (const crJXJHuodongCanvasInitMethod & handle);
	MethodClass(JXJ, JXJHuodongCanvasInit)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
};

class crJXJHuodongCanvasUpdateMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJHuodongCanvasUpdateMethod ();
	crJXJHuodongCanvasUpdateMethod (const crJXJHuodongCanvasUpdateMethod & handle);
	MethodClass(JXJ, JXJHuodongCanvasUpdate)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crCanvasNode *m_this;
	std::string m_sw;
	std::string m_timeInput;
	std::string m_contentInput;
	std::string m_title[7];
	std::string m_imageBox[5];
	std::string m_getBtn;
	std::string m_nullImage;
	std::string m_num[5];
	std::string m_title_sw[7];
	std::string m_hadGottenBtn;
	std::string m_scroll;
	std::string m_code_sw;
	std::string m_codeText;
	std::string m_Mzth_Input3;
	std::string m_Mzth_Input4;
	std::string m_Mzth_Input8;
	std::string m_Mzth_Input10;
	std::string m_Mzth_Icon1;
	std::string m_Mzth_Icon2;
	std::string m_Mzth_Donghua1;
	std::string m_Mzth_Donghua2;
};

class crJXJSetCurrentHuodongIdMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetCurrentHuodongIdMethod ();
	crJXJSetCurrentHuodongIdMethod (const crJXJSetCurrentHuodongIdMethod & handle);
	MethodClass(JXJ, JXJSetCurrentHuodongId)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	unsigned char m_idx ;
	std::string m_scroll;
};

//class crJXJCheckHuodongTimeIntervalMethod :public CRCore::crMethod
//{
//public:
//	crJXJCheckHuodongTimeIntervalMethod ();
//	crJXJCheckHuodongTimeIntervalMethod (const crJXJCheckHuodongTimeIntervalMethod & handle);
//	MethodClass(JXJ, JXJCheckHuodongTimeInterval)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	crJXJHuodong *m_this;
//	CRNetApp::crPlayerGameData *m_playerGameData;
//};

class crJXJEquipAttackExtraTimeLimitTestMethod : public CRCore::crMethod
{//SceneServer ChildItem Method
public:
	crJXJEquipAttackExtraTimeLimitTestMethod ();
	crJXJEquipAttackExtraTimeLimitTestMethod (const crJXJEquipAttackExtraTimeLimitTestMethod & handle);
	MethodClass(JXJ, JXJEquipAttackExtraTimeLimitTest)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crInstanceItem* m_this;
	CRCore::ref_ptr<CRNetApp::HitParam> m_hitParam;
	int m_hitrate;
	int m_timeInterval;
	time_t m_preTime_t;
};

//class crJXJUIHuodongToolTipsMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIHuodongToolTipsMethod ();
//	crJXJUIHuodongToolTipsMethod (const crJXJUIHuodongToolTipsMethod & handle);
//	MethodClass(JXJ, JXJUIHuodongToolTips)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
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
//	int m_festivalActivity_index;//该值为非0时代表节日活动，默认值为0
//};


class crJXJQueryTabVersionMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJQueryTabVersionMethod();
	crJXJQueryTabVersionMethod(const crJXJQueryTabVersionMethod & handle);
	MethodClass(JXJ, JXJQueryTabVersion)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::vector<int> m_tabids;
};

class crJXJRecvQueryTabVersionMethod :public CRCore::crMethod
{//GameServer Method & client
public:
	crJXJRecvQueryTabVersionMethod();
	crJXJRecvQueryTabVersionMethod(const crJXJRecvQueryTabVersionMethod& handle);
	MethodClass(JXJ, JXJRecvQueryTabVersion)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
};

//class crJXJClientReloadFileMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJClientReloadFileMethod ();
//	crJXJClientReloadFileMethod (const crJXJClientReloadFileMethod & handle);
//	MethodClass(JXJ, JXJClientReloadFile)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	std::string m_fileName;
//	unsigned int m_fileId;
//};

class crJXJCheckHuodongSingleValueChangeMethod :public CRCore::crMethod
{//GameServer Method
	// 单个值突变
public:
	crJXJCheckHuodongSingleValueChangeMethod ();
	crJXJCheckHuodongSingleValueChangeMethod (const crJXJCheckHuodongSingleValueChangeMethod & handle);
	MethodClass(JXJ, JXJCheckHuodongSingleValueChange)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
	int m_WCHDATA_ID;
	unsigned char m_dataType;
	int m_lastvalue;
};

class crJXJClientHuodongGetAwardsMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientHuodongGetAwardsMethod ();
	crJXJClientHuodongGetAwardsMethod (const crJXJClientHuodongGetAwardsMethod & handle);
	MethodClass(JXJ, JXJClientHuodongGetAwards)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode * m_this;
	std::string m_infoCanvas;
	std::string m_information;
	std::string m_okbtn;
	std::string m_input;
	unsigned char m_festivalHuodongIndex;//0，1，2依次对应节日活动页面的第一，二，三个奖励领取按钮,默认值为0
};

class crJXJCheckHuodongAddUpValueChangeMethod :public CRCore::crMethod
{//GameServer Method
	//单个值累计变化
public:
	crJXJCheckHuodongAddUpValueChangeMethod ();
	crJXJCheckHuodongAddUpValueChangeMethod (const crJXJCheckHuodongAddUpValueChangeMethod & handle);
	MethodClass(JXJ, JXJCheckHuodongAddUpValueChange)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
	int m_WCHDATA_ID;
	unsigned char m_dataType;
	JXJDataStruct m_dataStruct;
	int m_lastdata;
};

class crJXJCheckHuodongIsValueChangeMethod :public CRCore::crMethod
{//GameServer Method
	//单个值是否达到某个值
public:
	crJXJCheckHuodongIsValueChangeMethod ();
	crJXJCheckHuodongIsValueChangeMethod (const crJXJCheckHuodongIsValueChangeMethod & handle);
	MethodClass(JXJ, JXJCheckHuodongIsValueChange)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
	int m_WCHDATA_ID;
	unsigned char m_dataType;
	JXJDataStruct m_dataStruct;
};
class crJXJShiliOccupyCityCountMethod :public CRCore::crMethod
{//GameServer Method
	//势力占领城市数量，强国之路活动
public:
	crJXJShiliOccupyCityCountMethod ();
	crJXJShiliOccupyCityCountMethod (const crJXJShiliOccupyCityCountMethod & handle);
	MethodClass(JXJ, JXJShiliOccupyCityCount)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
};

//class crJXJMainRoleHuodongInitMethod :public CRCore::crMethod
//{// GameClient Method
//public:
//	crJXJMainRoleHuodongInitMethod ();
//	crJXJMainRoleHuodongInitMethod (const crJXJMainRoleHuodongInitMethod & handle);
//	MethodClass(JXJ, JXJMainRoleHuodongInit)
//	virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crRole *m_this;
//	CRNetApp::crPlayerGameData *m_playerGameData;
//};

class crJXJReloadHuodongTabDataMethod :public CRCore::crMethod
{//Client Method & GameServer Method
public:
	crJXJReloadHuodongTabDataMethod ();
	crJXJReloadHuodongTabDataMethod (const crJXJReloadHuodongTabDataMethod & handle);
	MethodClass(JXJ, JXJReloadHuodongTabData)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData * m_this;
};

//class crJXJReloadHuodongScriptMethod :public CRCore::crMethod
//{//Client Method 
//public:
//	crJXJReloadHuodongScriptMethod ();
//	crJXJReloadHuodongScriptMethod (const crJXJReloadHuodongScriptMethod & handle);
//	MethodClass(JXJ, JXJReloadHuodongScript)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	std::string m_canvas;
//	short m_huodongId;
//};

class crJXJReloadHuodongTabMethod :public CRCore::crMethod
{//Client Method & GameServer Method
public:
	crJXJReloadHuodongTabMethod ();
	crJXJReloadHuodongTabMethod (const crJXJReloadHuodongTabMethod & handle);
	MethodClass(JXJ, JXJReloadHuodongTab)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
};

class crJXJCheckHuodongServerOpenDayMethod :public CRCore::crMethod
{//GameServer Method
	// 检查开服时间
public:
	crJXJCheckHuodongServerOpenDayMethod ();
	crJXJCheckHuodongServerOpenDayMethod (const crJXJCheckHuodongServerOpenDayMethod & handle);
	MethodClass(JXJ, JXJCheckHuodongServerOpenDay)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
};

class crJXJSYNShiliDataMethod :public CRCore::crMethod
{//Client Method 
public:
	crJXJSYNShiliDataMethod ();
	crJXJSYNShiliDataMethod(const crJXJSYNShiliDataMethod& handle);
	MethodClass(JXJ, JXJSYNShiliData)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData * m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class crJXJAddNoChatPlayerMethod :public CRCore::crMethod
{//Client Method 
public:
	crJXJAddNoChatPlayerMethod ();
	crJXJAddNoChatPlayerMethod (const crJXJAddNoChatPlayerMethod & handle);
	MethodClass(JXJ, JXJAddNoChatPlayer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	std::string m_nameInput;
	unsigned char m_op;
};

class crJXJRecvAddNoChatPlayerMethod:public CRCore::crMethod
{//Client & GameServer Method
public:
	crJXJRecvAddNoChatPlayerMethod();
	crJXJRecvAddNoChatPlayerMethod(const crJXJRecvAddNoChatPlayerMethod& handle);
	MethodClass(JXJ, JXJRecvAddNoChatPlayer)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRNetApp::crPlayerGameData *m_this;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	int m_netType;
	std::string m_canvas;
	std::string m_nameInput;
};

struct ActiveCodeData
{
	std::string m_activeCode;
	int m_huodongID;
};

class crJXJGetAwardsByActiveCodeMethod: public CRCore::crMethod
{
public:
	crJXJGetAwardsByActiveCodeMethod();
	crJXJGetAwardsByActiveCodeMethod(const crJXJGetAwardsByActiveCodeMethod & handle);
	MethodClass(JXJ,JXJGetAwardsByActiveCode)

	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
private:
	CRNetApp::crPlayerGameData *m_this;
	ActiveCodeData * m_activeCodeData;
	char *m_retVal;
};


//class crJXJRecvHuodongGetAwardByActiveCodeMethod :public CRCore::crMethod
//{//GameServer & GameClient Method
//public:
//	crJXJRecvHuodongGetAwardByActiveCodeMethod ();
//	crJXJRecvHuodongGetAwardByActiveCodeMethod (const crJXJRecvHuodongGetAwardByActiveCodeMethod & handle);
//	MethodClass(JXJ, JXJRecvHuodongGetAwardByActiveCode)
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//	virtual void operator()(crHandle &handle);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	std::string m_canvas;
//};

class crJXJCheckPlayerLoginDayMethod :public CRCore::crMethod
{//GameServer Method
	// 检查是否是活动要求天数登陆
public:
	crJXJCheckPlayerLoginDayMethod ();
	crJXJCheckPlayerLoginDayMethod (const crJXJCheckPlayerLoginDayMethod & handle);
	MethodClass(JXJ, JXJCheckPlayerLoginDay)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	crJXJNewHuodong *m_this;
	CRNetApp::crPlayerGameData *m_playerGameData;
};

class crJXJHuodongJumpMethod :public CRCore::crMethod
{
public:
	crJXJHuodongJumpMethod ();
	crJXJHuodongJumpMethod (const crJXJHuodongJumpMethod & handle);
	MethodClass(JXJ, JXJHuodongJump)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	unsigned char m_linkType;
};

class crJXJSetCurrentHuodongIdxMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJSetCurrentHuodongIdxMethod ();
	crJXJSetCurrentHuodongIdxMethod (const crJXJSetCurrentHuodongIdxMethod & handle);
	MethodClass(JXJ, JXJSetCurrentHuodongIdx)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	CRCore::crWidgetNode *m_this;
	std::string m_canvas;
	std::string m_scroll;
};

class crJXJClientRecvServerLogMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientRecvServerLogMethod ();
	crJXJClientRecvServerLogMethod(const crJXJClientRecvServerLogMethod& handle);
	MethodClass(JXJ, JXJClientRecvServerLog)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	std::string m_canvas;
	std::string m_input;
};

class crJXJClientDownLoadLogFilesMethod :public CRCore::crMethod
{//Client Method
public:
	crJXJClientDownLoadLogFilesMethod ();
	crJXJClientDownLoadLogFilesMethod (const crJXJClientDownLoadLogFilesMethod & handle);
	MethodClass(JXJ, JXJClientDownLoadLogFiles)
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void operator()(crHandle &handle);
protected:
	int m_msgid;
	std::string m_fileName;
};

class crJXJExtraCardAddRTMPMethod : public CRCore::crMethod
{//SceneServer & GameClient Method		将卡增加rtmp
public:
	crJXJExtraCardAddRTMPMethod ();
	crJXJExtraCardAddRTMPMethod (const crJXJExtraCardAddRTMPMethod & handle);
	MethodClass(JXJ, JXJExtraCardAddRTMP)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crInstanceItem* m_this;
	float *m_dt;
	float m_duration;
	float m_extraValue;
	bool m_start;
	float m_timer;
};


}
#endif
