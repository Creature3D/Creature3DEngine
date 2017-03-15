// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// File: appMethod3.h
// Date: 2013.5.31
// Auth: Luke.Lu
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef JXJ_APPMETHOD3_H
#define JXJ_APPMETHOD3_H

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// header files
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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// script method declare
namespace JXJ
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailShowMethod
	// On  : Showing Mail Canvas
	// Desp:	List mails
	//			prepare widgets
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailShowMethod:public CRCore::crMethod
	{
	public:
		crJXJUIOnMailShowMethod();
		crJXJUIOnMailShowMethod(const crJXJUIOnMailShowMethod& handle);
		MethodClass(JXJ, JXJUIOnMailShow)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_friend_canvas;
		std::string m_send_to;
		std::string m_title;
		std::string m_list;
		std::string m_switch;
		std::string m_mailchoice[5];//
		std::string m_mailinput[5];
		std::string m_itemicon[6];
		std::string m_mailsize;
		std::string m_mailbody;
		std::string m_deletebtn;
		std::string m_allselbtn;
		std::string m_pickupitem;
		std::string m_iconSwitch[5];
		std::string m_MailListScroll;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailFriendListMethod
	// On  : Showing Mail Friend List
	// Desp:	read friend list
	//			display friend list
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailFriendListMethod:public CRCore::crMethod
	{
	public:
		crJXJUIOnMailFriendListMethod();
		crJXJUIOnMailFriendListMethod(const crJXJUIOnMailFriendListMethod& handle);

		MethodClass(JXJ, JXJUIOnMailFriendList)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_friend_canvas;
		std::string m_friend_lbox;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailSendingMethod
	// On  : Push Sending Button
	// Desp:	check friend
	//			send info to server
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailSendingMethod:public CRCore::crMethod
	{
	public:
		crJXJUIOnMailSendingMethod();
		crJXJUIOnMailSendingMethod(const crJXJUIOnMailSendingMethod& handle);

		MethodClass(JXJ, JXJUIOnMailSending)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_txt_friend;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnChatFadeTimerMethod
	// On  : 聊天窗口 Frame Update
	// Desp: 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnChatFadeTimerMethod:public CRCore::crMethod
	{
	public:
		crJXJUIOnChatFadeTimerMethod();
		crJXJUIOnChatFadeTimerMethod(const crJXJUIOnChatFadeTimerMethod& handle);

		MethodClass(JXJ, JXJUIOnChatFadeTimer)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		float *m_dt; // 经过的时间
		const float m_fade_line;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationShowMethod
	// On  : 打开编队界面
	// Desp: 初始化编队界面
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationShowMethod:public CRCore::crMethod
	{// client method
	public:
		crJXJUIOnFormationShowMethod();
		crJXJUIOnFormationShowMethod(const crJXJUIOnFormationShowMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationShow)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_left_heads[8];
		std::string m_left_names[8];
		std::string m_left_nums[8];
		std::string m_title_lead;
		std::string m_title_used;
		std::string m_title_remain;
		std::string m_right_heads[3];
		std::string m_right_trp_total[3];
		std::string m_right_trp_used[3];
		std::string m_right_trp_plead[3];
		std::string m_right_trp_ulead[3];
		std::string m_right_eqs[9];
		std::string m_right_slide[3];
		std::string m_pic;
		std::string m_leftpage;
		std::string m_rightpage;
		std::string m_colorbox[6];
		std::string m_pic2;
		std::string m_radio;
		std::string m_editbox[3];
		std::string m_levelsw;
		std::string m_formationgamelist;
		std::string m_newleftnum[9];
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationArmSelectingMethod
	// On  : 编队界面选择
	// Desp: 初始化编队兵种选择
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationArmSelectingMethod:public CRCore::crMethod
	{// client method
	public:
		crJXJUIOnFormationArmSelectingMethod();
		crJXJUIOnFormationArmSelectingMethod(const crJXJUIOnFormationArmSelectingMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationArmSelecting)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		char m_selecting;
		std::string m_left_heads[8];
		std::string m_left_names[8];
		std::string m_left_nums[8];
		std::string m_leftbtn;
		std::string m_rightbtn;
		std::string m_formationradio;
		std::string m_formationgamelist;
		std::string m_newleftnum[9];
		std::string m_tempformaionstr;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationArmCancelMethod
	// On  : 编制撤销
	// Desp: 初始化编队兵种选择
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationArmCancelMethod:public CRCore::crMethod
	{// client method
	public:
		crJXJUIOnFormationArmCancelMethod();
		crJXJUIOnFormationArmCancelMethod(const crJXJUIOnFormationArmCancelMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationArmCancel)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_index;
		std::string m_title_lead;
		std::string m_title_used;
		std::string m_title_remain;
		std::string m_num_total;
		std::string m_num_used;
		std::string m_num_plead;
		std::string m_num_ulead;
		std::string m_slide;
		std::string m_arm_head;
		std::string m_eqs[3];
		std::string m_card;
		std::string m_pic;
		std::string m_colorbox[6];
		std::string m_pic2;
		std::string m_editbox[3];
		std::string m_tipcanvas;
		std::string m_tiptitle;
		std::string m_tipcontent;
	};

	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationArmAddMethod
	// On  : 兵种添加
	// Desp: 判断编制最大 并加入编制
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationArmAddMethod:public CRCore::crMethod
	{// client method
	public:
		crJXJUIOnFormationArmAddMethod();
		crJXJUIOnFormationArmAddMethod(const crJXJUIOnFormationArmAddMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationArmAdd)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode	*m_this;
		std::string m_left_heads[8];
		std::string m_left_names[8];
		std::string m_left_nums[8];
		std::string m_title_lead;
		std::string m_title_used;
		std::string m_title_remain;
		std::string m_right_heads[3];
		std::string m_right_trp_total[3];
		std::string m_right_trp_used[3];
		std::string m_right_trp_plead[3];
		std::string m_right_trp_ulead[3];
		std::string m_right_eqs[9];
		std::string m_right_slide[3];
		std::string m_tip;
		std::string m_tip_title;
		std::string m_editbox[3];
		int m_index;
		std::string m_formationlist;
		CRProducer::crGUIEventAdapter* m_ea;
		int m_delayHideWidgetTime;
		std::string m_taskCanvas;//任务栏窗口，用于调整提示窗口位置
	};
	

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationStoreMethod
	// On  : 编制保存
	// Desp: 获取数据保存
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationStoreMethod:public CRCore::crMethod
	{// client method
	public:
		crJXJUIOnFormationStoreMethod();
		crJXJUIOnFormationStoreMethod(const crJXJUIOnFormationStoreMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationStore)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode	*m_this;
		std::string m_right_trp_used[3];
		std::string m_pageradio;
		std::string m_nameedit[4];
		int m_index;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnFormationStoreRecvMethod
	// On  : 发包处理
	// Desp: 获取数据保存，对照，保存
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnFormationStoreRecvMethod:public CRCore::crMethod
	{// client server method
	public:
		crJXJUIOnFormationStoreRecvMethod();
		crJXJUIOnFormationStoreRecvMethod(const crJXJUIOnFormationStoreRecvMethod& handle);

		MethodClass(JXJ, JXJUIOnFormationStoreRecv)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRNetApp::crPlayerGameData				*m_this;
		CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
		int										m_net_type;
		std::string m_formation_canvas;
		std::string m_jkcnavase;
	};


	// - - - - - - - - - - - - - - - - Mail Methods - - - - - - - - - - - - - - - - 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailPagingMethod
	// On  : push
	// Desp: 邮件翻页
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailPagingMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIOnMailPagingMethod();
		crJXJUIOnMailPagingMethod(const crJXJUIOnMailPagingMethod& handle);

		MethodClass(JXJ, JXJUIOnMailPaging)

		virtual void operator()(crHandle &handle);

		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_dir; // left and right
		std::string m_input_name;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailPagingMethod
	// On  : update data
	// Desp: 更新数据
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailUpdateDataMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIOnMailUpdateDataMethod();
		crJXJUIOnMailUpdateDataMethod(const crJXJUIOnMailUpdateDataMethod& handle);
		MethodClass(JXJ, JXJUIOnMailUpdateData)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_page_show;
		std::string m_statesw;
		std::string m_leftinput[15];
		std::string m_checkbox[5];
		std::string m_rightinput[5];
		std::string m_righticon[5];
		std::string m_righticonsw[5];
		std::string m_allselectbtn;
		std::string m_deletebtn;
		std::string m_getbtn;
		std::string m_scrollbar;
		std::string m_pagesw;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIOnMailTabSelectingMethod
	// On  : tab select
	// Desp: 数据更新
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIOnMailTabSelectingMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIOnMailTabSelectingMethod();
		crJXJUIOnMailTabSelectingMethod(const crJXJUIOnMailTabSelectingMethod& handle);
		MethodClass(JXJ, JXJUIOnMailTabSelecting)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_selecting;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJOnMailRequestMethod
	// On  : 邮件更新
	// Desp: 数据数据和请求封装、解析
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJOnMailRequestMethod:public CRCore::crMethod
	{// client and server
	public:
		crJXJOnMailRequestMethod();
		crJXJOnMailRequestMethod(const crJXJOnMailRequestMethod& handle);
		MethodClass(JXJ, JXJOnMailRequest)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRNetApp::crPlayerGameData				*m_this;
		CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
		int										m_net_type;
		std::string m_mail_canvas;
// 		std::string m_input_page;
// 		std::string m_inputname[5];
// 		std::string m_inputdata[5];
// 		std::string m_inputtitle[5];
// 		std::string m_mailchoice[5];
// 		std::string m_mailflag[5];
// 		std::string m_allselbtn;
// 		std::string m_deletebtn;
// 		std::string m_mailname;
// 		std::string m_mailtitle;
// 		std::string m_mailtext;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJOnMailRecvMethod
	// On  : mail
	// Desp: 数据数据和请求封装、解析
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJOnMailRecvMethod:public CRCore::crMethod
	{// client and server
	public:
		crJXJOnMailRecvMethod();
		crJXJOnMailRecvMethod(const crJXJOnMailRecvMethod& handle);
		MethodClass(JXJ, JXJOnMailRecv)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRNetApp::crPlayerGameData				*m_this;
		CRCore::ref_ptr<CRCore::crStreamBuf>	m_stream;
		int										m_net_type;
		std::string m_mailcanvas;
		std::string m_receiver;
		std::string m_title;
		std::string m_content;
		std::string m_contentsize;
		std::string m_tipcanvas;
		std::string m_tiptitle;
		std::string m_tipcontent;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJOnMailSendingMethod
	// On  : mail send 
	// Desp: 数据数据和请求封装、解析
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJOnMailSendingMethod:public CRCore::crMethod
	{// client and server
	public:
		crJXJOnMailSendingMethod();
		crJXJOnMailSendingMethod(const crJXJOnMailSendingMethod& handle);
		MethodClass(JXJ, JXJOnMailSending)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_receiver;
		std::string m_title;
		std::string m_content;
		std::string m_tipcanvas;
		std::string m_tiptitle;
		std::string m_tipcontent;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJMailInsertingMethod
	// On  : mail inserting into database
	// Desp: 邮件数据库操作
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJMailInsertingMethod:public CRCore::crMethod
	{// server
	public:
		crJXJMailInsertingMethod();
		crJXJMailInsertingMethod(const crJXJMailInsertingMethod& handle);
		MethodClass(JXJ, JXJMailInserting)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		//float m_avail_time;
		CRNetApp::crServerBrainHandle *m_this;
		int m_taskcount;
		std::vector< CRCore::ref_ptr<crMailInfo> >m_taskVec;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJMailQueryingMethod
	// On  : mail query list
	// Desp: 邮件数据库操作
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//typedef std::vector< CRCore::ref_ptr<CRCore::crStreamBuf> > JXJMailBodyVec;//itemid//装备、将卡等

	class crJXJMailQueryingMethod:public CRCore::crMethod
	{// server
	public:
		crJXJMailQueryingMethod();
		crJXJMailQueryingMethod(const crJXJMailQueryingMethod& handle);
		MethodClass(JXJ, JXJMailQuerying)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		//float m_avail_time;
		CRNetApp::crServerBrainHandle *m_this;
		int m_taskcount;
		std::vector< CRCore::ref_ptr<crMailQuery> >m_taskVec;
		//JXJMailBodyVec m_mailbodyvec;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJFormationSlidingMethod
	// On  : sliding
	// Desp: 更新数据
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJFormationSlidingMethod:public CRCore::crMethod
	{// client
	public:
		crJXJFormationSlidingMethod();
		crJXJFormationSlidingMethod(const crJXJFormationSlidingMethod& handle);
		MethodClass(JXJ, JXJFormationSliding)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRUI::crScrollBarWidgetNode *m_this;
		int m_index;
		std::string m_title_lead;
		std::string m_title_used;
		std::string m_title_remain;
		std::string m_right_trp_total[3];
		std::string m_right_trp_used[3];
		std::string m_right_trp_plead[3];
		std::string m_right_trp_ulead[3];
		std::string m_editbox;
	};
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJBattleTimterRuntimeMethod
	// On  : Battle Timer Runtime
	// Desp: 更新战场计时器
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJBattleTimterRuntimeMethod:public CRCore::crMethod
	{// client
	public:
		crJXJBattleTimterRuntimeMethod();
		crJXJBattleTimterRuntimeMethod(const crJXJBattleTimterRuntimeMethod& handle);
		MethodClass(JXJ, JXJBattleTimterRuntime)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		float m_dt;
		std::string m_canvas;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJChatReshowMethod
	// On  : reshow 
	// Desp: 重新显示chat 内容
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJChatReshowMethod:public CRCore::crMethod
	{// client
	public:
		crJXJChatReshowMethod();
		crJXJChatReshowMethod(const crJXJChatReshowMethod& handle);
		MethodClass(JXJ, JXJChatReshow)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
		//void analysisChatText(std::string text);
	protected:
		std::string m_chat_canvas;
		std::string m_chat_show;
		std::string m_chat_mode;
		std::string m_chat_cfg;
		std::string m_input;
		std::string m_scroll;
		std::string m_RoomChatCanvas;
		int m_textRow;
// 		typedef std::vector<std::pair<std::pair<int,int>,std::string>> ChatItemInfoVec;//itemid,equipmagic,name
// 		typedef std::vector<std::pair<unsigned char,std::string>> SystemChatInfoVec;//超链接ID，text
// 		typedef std::vector<std::pair<InviteInfo,std::string>> InviteChatInfoVec;//超链接ID，text
// 		ChatItemInfoVec m_strVec; //是否为item名，字符串--
// 		SystemChatInfoVec m_strNoticeVec;
// 		InviteChatInfoVec m_strInviteVec;
// 		std::map<int,std::pair<int,int>> m_linkIdMap;
// 		std::map<int,InviteInfo> m_inviteIdMap;
// 		static int m_slinkIndex;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJChatParserMethod
	// On  : parse input text
	// Desp: 绘制表情图
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJChatParserMethod:public CRCore::crMethod
	{// client
	public:
		crJXJChatParserMethod();
		crJXJChatParserMethod(const crJXJChatParserMethod& handle);
		MethodClass(JXJ, JXJChatParser)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode* m_this;
		std::string m_chat_show;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJBattleChatShowMethod
	// On  : chat canvas show in battle
	// Desp: 战场聊天框初始化
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJBattleChatShowMethod:public CRCore::crMethod
	{// client
	public:
		crJXJBattleChatShowMethod();
		crJXJBattleChatShowMethod(const crJXJBattleChatShowMethod& handle);
		MethodClass(JXJ, JXJBattleChatShow)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		std::string m_canvas;
	};


	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJFormationCloseMethod
	// On  : reset flag
	// Desp: 编队关闭
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJFormationCloseMethod:public CRCore::crMethod
	{// client
	public:
		crJXJFormationCloseMethod();
		crJXJFormationCloseMethod(const crJXJFormationCloseMethod& handle);
		MethodClass(JXJ, JXJFormationClose)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_cards;
	};

	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIJangkaListShowMethod
	// On  : update card canvas
	// Desp: 更新将卡数据
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIJangkaListShowMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIJangkaListShowMethod();
		crJXJUIJangkaListShowMethod(const crJXJUIJangkaListShowMethod& handle);
		MethodClass(JXJ, JXJUIJangkaListShow)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_formation;
	//	std::string m_for_equips[9];
		std::string m_colorradio;
		std::string m_cardtyperadio;
		std::string m_bagtyperadio;
		std::string m_arrangebtn;
	};
	// - - - - - - - - - - - - - - - - - - - - - - - - 关隘战 - - - - - - - - - - - - - - - - - - - - - - - - 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIBattleOpenMethod
	// On  : 
	// Desp: 客户端计时器更新
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIBattleOpenMethod:public CRCore::crMethod
	{// server
	public:
		crJXJUIBattleOpenMethod();
		crJXJUIBattleOpenMethod(const crJXJUIBattleOpenMethod& handle);
		MethodClass(JXJ, JXJUIBattleOpen)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		int m_select;
		std::string m_canvas[8];
		std::string m_jr1switch;
		std::string m_zoom;
		std::string m_enlarge;

	};
	//// - - - - - - - - - - - - - - - - - - - - - - - - 关隘战Battle Field - - - - - - - - - - - - - - - - - - - - - - - - 
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJPassSendingRuntimeMethod
	//// On  : 
	//// Desp: 服务器定时向客户端发送数据
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJPassSendingRuntimeMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJPassSendingRuntimeMethod();
	//	crJXJPassSendingRuntimeMethod(const crJXJPassSendingRuntimeMethod& handle);
	//	MethodClass(JXJ, JXJPassSendingRuntime)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crRoom *m_this;
	//};
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJPassMainRuntimeMethod
	//// On  : 
	//// Desp: 游戏占领检测
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJPassMainRuntimeMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJPassMainRuntimeMethod();
	//	crJXJPassMainRuntimeMethod(const crJXJPassMainRuntimeMethod& handle);
	//	MethodClass(JXJ, JXJPassMainRuntime)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crRoom *m_this;
	//	CREncapsulation::crTableIO *m_checks;
	//	float *m_dt;
	//};



	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJPassCheckGameOverMethod
	//// On  : 
	//// Desp: 判断游戏是否结束 - 关隘一方资源消耗为零就结束
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJPassCheckGameOverMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJPassCheckGameOverMethod();
	//	crJXJPassCheckGameOverMethod(const crJXJPassCheckGameOverMethod& handle);
	//	MethodClass(JXJ, JXJPassCheckGameOver)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crRoom *m_this;
	//};


	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJPassScoreRuntimeMethod
	//// On  : 
	//// Desp: 关隘中每10秒减少粮草逻辑
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJPassScoreRuntimeMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJPassScoreRuntimeMethod();
	//	crJXJPassScoreRuntimeMethod(const crJXJPassScoreRuntimeMethod& handle);
	//	MethodClass(JXJ, JXJPassScoreRuntime)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crRoom *m_this;
	//	std::vector<short> m_downs;
	//};


	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJRecvPassServerSendingMethod
	//// On  : 
	//// Desp: 关隘游戏中的数据同步接收方法
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJRecvPassServerSendingMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJRecvPassServerSendingMethod();
	//	crJXJRecvPassServerSendingMethod(const crJXJRecvPassServerSendingMethod& handle);
	//	MethodClass(JXJ, JXJRecvPassServerSending)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crPlayerGameData *m_this;
	//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	//	int	m_net_type;
	//	std::string m_canvas;
	//};

	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJPassArmRemoveMethod
	//// On  : 
	//// Desp: 损失部队减少粮草
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJPassArmRemoveMethod:public CRCore::crMethod
	//{// server
	//public:
	//	crJXJPassArmRemoveMethod();
	//	crJXJPassArmRemoveMethod(const crJXJPassArmRemoveMethod& handle);
	//	MethodClass(JXJ, JXJPassArmRemove)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crInstanceItem* m_this;
	//	short m_factor; // 减少粮草的因素
	//};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIBattleAdjustChatMethod
	// On  : 
	// Desp: 调整聊天框位置
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIBattleAdjustChatMethod:public CRCore::crMethod
	{// server
	public:
		crJXJUIBattleAdjustChatMethod();
		crJXJUIBattleAdjustChatMethod(const crJXJUIBattleAdjustChatMethod& handle);
		MethodClass(JXJ, JXJUIBattleAdjustChat)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		std::string m_chat;
		float m_pos_x;
		float m_pos_y;
	};

	class crJXJSetBuildCanvasMethod:public CRCore::crMethod
	{// client
	public:
		crJXJSetBuildCanvasMethod();
		crJXJSetBuildCanvasMethod(const crJXJSetBuildCanvasMethod& handle);
		MethodClass(JXJ, JXJSetBuildCanvas)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_rtt_widget;
	};
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJUIShowCanvasMethod
	//// On  : 
	//// Desp: canvas 显示
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJUIShowCanvasMethod:public CRCore::crMethod
	//{// client
	//public:
	//	crJXJUIShowCanvasMethod();
	//	crJXJUIShowCanvasMethod(const crJXJUIShowCanvasMethod& handle);
	//	MethodClass(JXJ, JXJUIShowCanvas)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	std::string m_canvas;
	//	std::string m_rtt_widget;
	//	short m_param;
	//	CRNetApp::crInstanceItem *m_this;
	//};
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIManiorRttRuntimeMethod
	// On  : 
	// Desp: 主界面RTT显示
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIManiorRttRuntimeMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIManiorRttRuntimeMethod();
		crJXJUIManiorRttRuntimeMethod(const crJXJUIManiorRttRuntimeMethod& handle);
		MethodClass(JXJ, JXJUIManiorRttRuntime)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crMatrixTransform *m_this;
		std::string m_backModel;
		CRCore::crVector2i m_texSize;
		float m_updInterval;
		CRCore::crVector2 m_distScale;
		float m_rotspeed;
		float m_rot;
		CRCore::crRenderToTexture* m_rtt;
		CRCore::crBoundingSphere m_boundSphere;
	};


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Clas: crJXJUIBattleShowChatMethod
	// On  : 
	// Desp: 战斗界面 聊天框显示
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	class crJXJUIBattleShowChatMethod:public CRCore::crMethod
	{// client
	public:
		crJXJUIBattleShowChatMethod();
		crJXJUIBattleShowChatMethod(const crJXJUIBattleShowChatMethod& handle);
		MethodClass(JXJ, JXJUIBattleShowChat)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		std::string m_chat;
		std::string m_minmap;
		float m_y_offset;
		std::string m_face;
	};


	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//// Clas: crJXJUIHeadItemInputMethod
	//// On  : 
	//// Desp: 输入item 数据 RTT 显示
	//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//class crJXJUIHeadItemInputMethod:public CRCore::crMethod
	//{// client
	//public:
	//	crJXJUIHeadItemInputMethod();
	//	crJXJUIHeadItemInputMethod(const crJXJUIHeadItemInputMethod& handle);
	//	MethodClass(JXJ, JXJUIHeadItemInput)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	std::string m_target_canvas;
	//	CRCore::crWidgetNode *m_this;
	//};
	class crJXJCountryKingUpdateMethod :public CRCore::crMethod
	{//GameServer Method
	public:
		crJXJCountryKingUpdateMethod();
		crJXJCountryKingUpdateMethod(const crJXJCountryKingUpdateMethod& handle);
		MethodClass(JXJ, JXJCountryKingUpdate)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRNetApp::crServerBrainHandle *m_this;
	};
	class crJXJQueryMailShowMethod :public CRCore::crMethod
	{//Client Method
	public:
		crJXJQueryMailShowMethod();
		crJXJQueryMailShowMethod(const crJXJQueryMailShowMethod& handle);
		MethodClass(JXJ, JXJQueryMailShow)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crCanvasNode *m_this;
		std::string m_input_page;
		std::string m_inputname[5];
		std::string m_inputdata[5];
		std::string m_inputtitle[5];
		std::string m_mailchoice[5];
		std::string m_mailflag[5];
		std::string m_allselbtn;
		std::string m_deletebtn;
		std::string m_mailname;
		std::string m_mailtitle;
		std::string m_mailtext;
	};
}
#endif 